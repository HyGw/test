/*
 *  linux/init/main.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  GK 2/5/95  -  Changed to support mounting root fs via NFS
 *  Added initrd & change_root: Werner Almesberger & Hans Lermen, Feb '96
 *  Moan early if gcc is old, avoiding bogus kernels - Paul Gortmaker, May '96
 *  Simplified starting of init:  Michael A. Griffith <grif@acm.org>
 */
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/random.h>
#include <linux/string.h>	

#include <asm/bitops.h>
#include <asm/bootinfo.h>
#include <asm/irq.h>
#include <asm/mipsregs.h>
#include <asm/system.h>
#include <linux/circ_buf.h>
#include <asm/tc3162.h>
#include <asm/io.h>

#include "../../../version/tcversion.h"
#include "../../../tools/trx/trx.h"

/* frankliao added 20101215 */
#include <flashhal.h>
#include "common.h"
#include "commands.h"
#include "./banner/mk_time"
#include "../net/eth.h"
#if defined(CONFIG_DUAL_IMAGE) || defined(TR068_LED)
#include "spiflash.h"
#endif

#define __KERNEL_SYSCALLS__

#define SYS_STACK_SIZE 		(4096 * 2)
//#define BOOLOADER_VERSION	"1.0"

#define FLASH_ETH_MAC		(0xff48 | flash_base)
#ifdef TR068_LED
#define FLASH_POWERLED_GPIO	(0xff66 | flash_base)
#define POWER_LED_GREEN		0
#define POWER_LED_RED		1
#endif

#if defined(TCSUPPORT_NAND_RT63368)
#define NAND_KERNEL_OFFSET  0x40000
#endif

unsigned char init_task_union[SYS_STACK_SIZE];

unsigned long kernelsp;
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
int authed=0; /*flag for tftp upload*/
#endif

extern void __init exception_init(void);
extern void init_IRQ(void);

extern void uart_init(void);
extern int eth_init(unsigned char *mac_addr);
extern int arp_init(void);
extern int ip_init(unsigned long ip);
extern int udp_init(void);
extern int arp_add_entry(unsigned char *ha, unsigned long ip);



extern unsigned long Jiffies;

/* frankliao modify 20100806 */
extern int set_lzma_addr(char* lzma_data_start, char* lzma_data_end);
extern unsigned long decompress_kernel(unsigned long output_start, unsigned long free_mem_ptr_p, unsigned long free_mem_ptr_end_p);

unsigned long calculate_dram_size(void){
#ifdef TC3262
	if(isMT751020)
			return 8 * (1 << (((VPint(0xbfb0008c) >> 13) & 0x7) - 1));
	if( isMT7505){
		if( isFPGA){
			return 8 * (1 << (((VPint(0xbfb0008c) >> 13) & 0x7) - 1));
		}
		else
		{
			if(VPint(CR_AHB_HWCONF) & (1<<10))
			{
				/* DDR1 */
				return 64 / (1 << ((VPint(CR_AHB_HWCONF) >> 11) & 0x3));
			}
			else{
				/* DDR2 */
				if(!((VPint(CR_AHB_HWCONF) >> 11) & 0x3))
					return 256;
				if(((VPint(CR_AHB_HWCONF) >> 11) & 0x3) == 0x1)
					return 32;
				if(((VPint(CR_AHB_HWCONF) >> 11) & 0x3) == 0x2)
					return 128;
				else
					return 64;
			}
		}

	}
#else
	unsigned long memsize;
	unsigned char samt;
	unsigned long col;
	unsigned long row;
	
	/* calculate SDRAM size */
	samt = VPchar(CR_DMC_SAMT);
	row = 8 + (samt & 0x3);
	col = 11 + ((samt>>2) & 0x3);
	/* 4 bands and 16 bit width */
	memsize = (1 << row) * (1 << col) * 4 * 2;
		
	return (memsize>>20);
#endif	
}

void start_kernel(void)
{
	int i;
	unsigned long start, current, old_current;
	/* frankliao modify 20101208 */
//	uint8 *flash_mac_addr = (uint8 *)FLASH_ETH_MAC;
	uint8 flash_mac_addr[6];

#ifdef TCSUPPORT_AUTOBENCH
	unsigned long val;
#endif


	trap_init();
	uart_init();  
	prom_printf("\n\n");
#ifdef TC3262
	if(isMT7505)
		prom_printf("MT7505");		
	else
		prom_printf("TC3169");
#else
	if (isRT63260)	
		prom_printf("RT63260");
	else if (isTC3162U)
		prom_printf("TC3162U");
	else if (isTC3162L5P5)
		prom_printf("TC3162L5/P5");
	else if (isTC3162L4P4)
		prom_printf("TC3162L4/P4");
	else if (isTC3162L3P3)
		prom_printf("TC3162L2F/P2F");
	else if (isTC3162L2P2)
		prom_printf("TC3162L2/P2");
	else 
		prom_printf("TC3162");
#endif
#ifdef TCSUPPORT_FREE_BOOTBASE
	prom_printf(" at %s version %s free bootbase\n\n",BOOT_CODE_TIME,BOOLOADER_VERSION);
#else
	prom_printf(" at %s version %s\n\n",BOOT_CODE_TIME,BOOLOADER_VERSION);
#endif



	prom_printf("Bootloader in SRAM\n\n");

	setup_arch();    /*setup the BEV0,and IRQ */
	exception_init();/*Copy handler to 0x80000080*/
	init_IRQ();      /*Allocate IRQfinder to Exception 0*/
	/* frankliao added 20101215 */
	flash_init(flash_base);

	time_init();



#if defined(TCSUPPORT_MULTI_UPGRADE)
#if !defined(TCSUPPORT_C1_TRUE) 
	prom_printf("Press any key in 3 secs to enter boot command mode.\n");
#endif
#else
#if !defined(TCSUPPORT_C1_OBM) 
	prom_printf("Press any key in 3 secs to enter boot command mode.\n");
#endif
#endif
	start = old_current = Jiffies;
#ifndef PLL_AUTO_SCAN
	while (serial_tstc() == 0) {
		current = Jiffies;


#if defined(TCSUPPORT_MULTI_UPGRADE)
                /*
	             wait for the first packet of image
                */
		if(startMultiUpgrade == 1){
			continue;
		}
#endif


#if defined(TCSUPPORT_MULTI_BOOT)
		if( startMultiBoot == 1){
			continue;
		}
#endif


		if (((current - start) % 5) == 0 && (current != old_current)) {
			old_current = current;
			prom_printf(".");
		}

		/* check timeout value */
#if defined(TCSUPPORT_MULTI_UPGRADE)
#if !defined(TCSUPPORT_C1_TRUE) 
		if ((current - start) > 300) {
#endif
#else
#ifndef TCSUPPORT_AUTOBENCH
#if !defined(TCSUPPORT_C1_OBM) 
		if ((current - start) > 300) {
#endif
#else
		if ((current - start) > 50) {
#endif			
#endif	
			/* timeout! */
			prom_printf("\n\n");
#ifdef BOOTROM_IN_SRAM
			prom_printf("bootloader in SRAM not support boot kernel\n\n");
#else
			boot_kernel();
#endif
			break;
		}
	}

	/* boot command mode */
	serial_inc();
#endif
	prom_printf("\n");
	/*Read the mac address from flash*/

	cmd_proc();
}


