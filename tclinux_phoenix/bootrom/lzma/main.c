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

#include "commands.h"
#include "./banner/mk_time"
#include "../net/eth.h"
#include <flashhal.h>
#if defined(TCSUPPORT_CPU_EN7512)||defined(TCSUPPORT_CPU_EN7521)
#include <spi_nand_flash.h>
#endif


#define SYS_STACK_SIZE 		(4096 * 2)
#if 0
#define	BOOTIMG_ADDR	(0x80800000 - 0x40000)
#endif

#ifdef SDRAM_8M
#define	BOOTIMG_ADDR	(0x80800000 - 0x50000)
#endif
#ifdef SDRAM_16M
#define	BOOTIMG_ADDR	(0x81000000 - 0x50000)
#endif
#ifdef SDRAM_32M
#define	BOOTIMG_ADDR	(0x82000000 - 0x50000)
#endif
#ifdef SDRAM_64M
#define	BOOTIMG_ADDR	(0x84000000 - 0x50000)
#endif

#ifndef TCSUPPORT_BB_NAND
#define nandflash_init(i)		do {} while(0)
#endif

typedef unsigned long ulg;

unsigned char init_task_union[SYS_STACK_SIZE];

unsigned long kernelsp;

/* frankliao modify 20100805 */
extern int set_lzma_addr(char* lzma_data_start, char* lzma_data_end);
extern ulg decompress_kernel(ulg output_start, ulg free_mem_ptr_p, ulg free_mem_ptr_end_p);

/*______________________________________________________________________________
**  start_lzma
**
**  descriptions: start to decompression booloader to booting address, then jump
**				  to booting address
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
//void start_kernel(void)
/* frankliao modify function name 20100805 */
void 
start_lzma(void)
{
	unsigned long output_data;
	unsigned long free_mem_ptr;
	unsigned long free_mem_ptr_end;
	int *boot_start = (int*)(0x80010000), *boot_end = (int*)(0x80010020);
    void (*jumpAddr)();

	output_data = BOOTIMG_ADDR | 0xa0000000;
	free_mem_ptr = 0x80600000;
	free_mem_ptr_end = 0x80700000;

	/* Disable all hardware interrupts */
	change_cp0_status(ST0_IM, 0x00);

	tc_outl(CR_INTC_IMR, 0x0);
	tc_outl(CR_TIMER_CTL, 0x0);
	tc_outl(CR_MAC_MACCR, 0x0);

//	uart_init();  						// for debug, frankliao modify 20110111
#if defined(TCSUPPORT_CPU_EN7512)||defined(TCSUPPORT_CPU_EN7521)
	if (IS_NANDFLASH) 
	{
	    SPI_NAND_Flash_Init(0);
	}
        else
        {
 	    VPint(0xBFA10014) =0;    /* Auto read mode */
	    VPint(0xBFA10020) =0;    /* Auto read mode */      
        }
#else					
	if (IS_NANDFLASH) {
		nandflash_init(0);
	}	
#endif	
	#ifdef BOOTROM_LARGE_SIZE
	/*boot.img offset is 64k*/
	*boot_end = ((*boot_start)&0xffff0000)+0x10000 + (*boot_end - *boot_start);
	*boot_start = ((*boot_start)&0xffff0000)+0x10000;
	#endif
	set_lzma_addr((char*)(*boot_start), (char*)(*boot_end));
	decompress_kernel(output_data, free_mem_ptr, free_mem_ptr_end);
	jumpAddr = (void (*)(void))BOOTIMG_ADDR;
	(*jumpAddr)();
} /* end of start_lzma() */
