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
#if defined (TCSUPPORT_GPON_DUAL_IMAGE) || defined (TCSUPPORT_EPON_DUAL_IMAGE)
#include "flash_layout/tc_partition.h"
struct mtd_info {
	unsigned long offset; /* Offset within the memory */
	unsigned long size;   /* Total size of the MTD */
	unsigned long erasesize;
};

extern struct mtd_info mtd;
#endif
#ifdef TCSUPPORT_BB_NAND
#include <mt7510_nandflash.h>
extern flashdev_info devinfo;
#endif

#ifdef TCSUPPORT_MTD_PARTITIONS_CMDLINE
#include "mtd_partition_parse.h"
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

#ifdef TR068_LED
#define LED_OEN(x)		do { 	if(x > 31){							\
									if(x > 47){						\
										regWrite32(CR_GPIO_CTRL3,regRead32(CR_GPIO_CTRL3)|(1<<((x-48)*2)));	\
									}else{							\
										regWrite32(CR_GPIO_CTRL2,regRead32(CR_GPIO_CTRL2)|(1<<((x-32)*2)));	\
									}								\
									regWrite32(CR_GPIO_ODRAIN1,regRead32(CR_GPIO_ODRAIN1)|(1<<(x-32)));	\
								}									\
								else{								\
									if(x > 15){						\
										regWrite32(CR_GPIO_CTRL1,regRead32(CR_GPIO_CTRL1)|(1<<((x-16)*2)));	\
									}else{							\
										regWrite32(CR_GPIO_CTRL,regRead32(CR_GPIO_CTRL)|(1<<(x*2)));	\
									}								\
									regWrite32(CR_GPIO_ODRAIN,regRead32(CR_GPIO_ODRAIN)|(1<<(x)));	\
								}									\
				} while(0)

#define LED_OFF(x) 	do {								\
								if(x > 31){					\
									regWrite32(CR_GPIO_DATA1,regRead32(CR_GPIO_DATA1)|(1<<(x-32)));	\
								}else{						\
									regWrite32(CR_GPIO_DATA,regRead32(CR_GPIO_DATA)|(1<<x));	\
								}						\		
						} while (0)
#define LED_ON(x) 	do {								\
								if(x > 31){					\
									regWrite32(CR_GPIO_DATA1,regRead32(CR_GPIO_DATA1)& ~(1<<(x-32)));	\
								}else{						\
									regWrite32(CR_GPIO_DATA,regRead32(CR_GPIO_DATA)& ~(1<<x));	\
								}						\
						} while (0)
#endif
unsigned char init_task_union[SYS_STACK_SIZE];

unsigned long kernelsp;
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
int authed=0; /*flag for tftp upload*/
#endif

extern void __init exception_init(void);
extern void init_IRQ(void);

extern void uart_init(void);
#ifdef TCSUPPORT_UART2
extern void uart2_init(void);
#endif
extern int eth_init(unsigned char *mac_addr);
extern int arp_init(void);
extern int ip_init(unsigned long ip);
extern int udp_init(void);
extern int arp_add_entry(unsigned char *ha, unsigned long ip);

#ifdef TR068_LED
void power_gpioInit(uint8 ledColor);
#endif

#ifdef MT7505_SWITCH_TBL_VERIRY
extern void MT7505_Switch_Tbl_Test(void);
#endif

/*<< BosaZhong@30Apr2014, add, for led */
unsigned int led_init(void);
void led_restore(unsigned int restoreReg);
void led_all_mode(int ledOn);
/*>> endof BosaZhong@30Apr2014, add, for led */

#if defined(TCSUPPORT_ETH4_WAN_PORT)
#define tcMiiStationRead(x, y)   miiStationRead(x, y)
#define tcMiiStationWrite(x, y, z) miiStationWrite(x, y, z)
#endif
static unsigned char mac_addr[6] = {0x00, 0xaa, 0xbb, 0x01, 0x23, 0x45};
//static unsigned long ip_addr = 0xc0a801fe;
static unsigned long ip_addr = 0xc0a80101;

extern unsigned long Jiffies;

/* frankliao modify 20100806 */
extern int set_lzma_addr(char* lzma_data_start, char* lzma_data_end);
extern unsigned long decompress_kernel(unsigned long output_start, unsigned long free_mem_ptr_p, unsigned long free_mem_ptr_end_p);
/* frankliao modify 20100805 for bootloader checksum */
#if defined(CONFIG_DUAL_IMAGE) || defined(TR068_LED) || defined(BOOT_LZMA_SUPPORT) || defined(TCSUPPORT_MULTI_UPGRADE) || defined(TCSUPPORT_MULTI_BOOT)
/**********************************************************************/
/* The following was grabbed and tweaked from the old snippets collection
 * of public domain C code. */

/**********************************************************************\
|* Demonstration program to compute the 32-bit CRC used as the frame  *|
|* check sequence in ADCCP (ANSI X3.66, also known as FIPS PUB 71     *|
|* and FED-STD-1003, the U.S. versions of CCITT's X.25 link-level     *|
|* protocol).  The 32-bit FCS was added via the Federal Register,     *|
|* 1 June 1982, p.23798.  I presume but don't know for certain that   *|
|* this polynomial is or will be included in CCITT V.41, which        *|
|* defines the 16-bit CRC (often called CRC-CCITT) polynomial.  FIPS  *|
|* PUB 78 says that the 32-bit FCS reduces otherwise undetected       *|
|* errors by a factor of 10^-5 over 16-bit FCS.                       *|
\**********************************************************************/

/* Copyright (C) 1986 Gary S. Brown.  You may use this program, or
   code or tables extracted from it, as desired without restriction.*/

/* First, the polynomial itself and its table of feedback terms.  The  */
/* polynomial is                                                       */
/* X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0 */
/* Note that we take it "backwards" and put the highest-order term in  */
/* the lowest-order bit.  The X^32 term is "implied"; the LSB is the   */
/* X^31 term, etc.  The X^0 term (usually shown as "+1") results in    */
/* the MSB being 1.                                                    */

/* Note that the usual hardware shift register implementation, which   */
/* is what we're using (we're merely optimizing it by doing eight-bit  */
/* chunks at a time) shifts bits into the lowest-order term.  In our   */
/* implementation, that means shifting towards the right.  Why do we   */
/* do it this way?  Because the calculated CRC must be transmitted in  */
/* order from highest-order term to lowest-order term.  UARTs transmit */
/* characters in order from LSB to MSB.  By storing the CRC this way,  */
/* we hand it to the UART in the order low-byte to high-byte; the UART */
/* sends each low-bit to hight-bit; and the result is transmission bit */
/* by bit from highest- to lowest-order term without requiring any bit */
/* shuffling on our part.  Reception works similarly.                  */

/* The feedback terms table consists of 256, 32-bit entries.  Notes:   */
/*                                                                     */
/*  1. The table can be generated at runtime if desired; code to do so */
/*     is shown later.  It might not be obvious, but the feedback      */
/*     terms simply represent the results of eight shift/xor opera-    */
/*     tions for all combinations of data and CRC register values.     */
/*                                                                     */
/*  2. The CRC accumulation logic is the same for all CRC polynomials, */
/*     be they sixteen or thirty-two bits wide.  You simply choose the */
/*     appropriate table.  Alternatively, because the table can be     */
/*     generated at runtime, you can start by generating the table for */
/*     the polynomial in question and use exactly the same "updcrc",   */
/*     if your application needn't simultaneously handle two CRC       */
/*     polynomials.  (Note, however, that XMODEM is strange.)          */
/*                                                                     */
/*  3. For 16-bit CRCs, the table entries need be only 16 bits wide;   */
/*     of course, 32-bit entries work OK if the high 16 bits are zero. */
/*                                                                     */
/*  4. The values must be right-shifted by eight bits by the "updcrc"  */
/*     logic; the shift must be unsigned (bring in zeroes).  On some   */
/*     hardware you could probably optimize the shift in assembler by  */
/*     using byte-swap instructions.                                   */
static const uint32_t crc_32_tab[] = { /* CRC polynomial 0xedb88320 */
0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

#define UPDC32(octet,crc) (crc_32_tab[((crc) ^ (octet)) & 0xff] ^ ((crc) >> 8))

uint32_t crc32buf(char *buf, size_t len, int flags)
{
      uint32_t crc;
	  /* frank added 20101208 */
	  char ch;
	#ifdef TCSUPPORT_NEW_SPIFLASH
	#define READBUFSIZE 4096
	  unsigned long retlen=0;
	  unsigned char membuf[READBUFSIZE];
	  uint32 i=0, index=0;
	  uint32 curLen= len;
	  uint32 from = buf;
	#endif

      crc = 0xFFFFFFFF;

	  /* for read data from flash */
	  if (flags == 0) {
	#ifdef TCSUPPORT_NEW_SPIFLASH
		from &= 0x03FFFFFF;
		curLen = len % READBUFSIZE;
		index = 0;
		while(len) {			
			flash_read((unsigned long)(from+index), curLen, &retlen, membuf);
			for (i=0; i<curLen; i++)
			{
				crc = UPDC32(membuf[i], crc);
			}
			len -= curLen;
			index += curLen;
			curLen = READBUFSIZE;
		}
	#else
    	  for ( ; len; --len, ++buf)
	      {
				ch = READ_FLASH_CACHE(buf);
				crc = UPDC32(ch, crc);
//				crc = UPDC32(*buf, crc);
      	  }
	#endif
	  /* for read data from sdram */  
	  }	else {
    	  for ( ; len; --len, ++buf)
	      {
				crc = UPDC32(*buf, crc);
      	  }
	  }
      return (uint32_t)crc;
}
#endif
unsigned long calculate_dram_size(void){
#ifdef TC3262
	if(isEN751221){
		if( isFPGA){
                        return 8 * (1 << (((VPint(0xbfb0008c) >> 13) & 0x7) - 1));
                }else{
                		return GET_DRAM_SIZE;
		}
	}else if(isMT751020){
			return 8 * (1 << (((VPint(0xbfb0008c) >> 13) & 0x7) - 1));
	}else if(isMT7505){
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
	else if (isRT63165 || isRT63365) {
		//if (VPint(CR_AHB_HWCONF) & (0x3<<24)) {
		if (VPint(CR_AHB_HWCONF) & (1<<25)) {
			return 8 * (1 << (((VPint(CR_DMC_DDR_CFG1) >> 18) & 0x7) - 1));

		/* SDRAM */
		} else {
			unsigned long sdram_cfg1;
			unsigned long col;
			unsigned long row;
			unsigned long bank = 4;
			
			/* calculate SDRAM size */
			sdram_cfg1 = VPint(0xbfb20004);
			row = 11 + ((sdram_cfg1>>16) & 0x3);
			col = 8 + ((sdram_cfg1>>20) & 0x3);
			if ((row == 11) && (col == 8))
				bank = 2;
			/* 4 banks and 16 bit width */
			return (((1 << row) * (1 << col) * bank * 2)>>20);
		}
	} else {
		return (8 * (1 << (((VPint(CR_DMC_CTL1) >> 18) & 0x7) - 1)));
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

#if defined(CONFIG_DUAL_IMAGE) || defined(TR068_LED)
int checkimage(int imageflag)
{
	struct trx_header *trx = NULL;
	char *input_data = NULL;
	char *trx_addr;	//frankliao added
	unsigned int len, crc32, magic;	// frankliao added 20101208
	
#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
    input_data = (char *) (flash_base + 0x280000);
#elif defined(TCSUPPORT_NAND_RT63368)
    input_data = (char *) (flash_base + NAND_KERNEL_OFFSET);
#else
#if defined(TCSUPPORT_CPU_EN7512)||defined(TCSUPPORT_CPU_EN7521)

	if(IS_SPIFLASH)
	{	
		flash_tclinux_start = 0x30000;
	}
#endif

	input_data = (char *) (flash_base + flash_tclinux_start);
#endif

	if(MAIN_IMAGE == imageflag)
	{
//		input_data = (char *) 0xb0020000;		
		trx = (struct trx_header *)input_data;
		trx_addr = &(trx->len);
		len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx->crc32);
		crc32 = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx->magic);
		magic = READ_FLASH_DWORD(trx_addr);

//		if ((trx->magic != TRX_MAGIC2) || (trx->len < sizeof(struct trx_header))) 
		if ((magic != TRX_MAGIC2) || (len < sizeof(struct trx_header))) 
		{
			prom_printf("\r\n main tclinux.bin magic or len check error!");
			return -1;
		}
		else
		{
//			if(trx->crc32 != crc32buf((char *)(input_data+sizeof(struct trx_header)), (trx->len-sizeof(struct trx_header))))
			if(crc32 != crc32buf((char *)(input_data+sizeof(struct trx_header)), (len-sizeof(struct trx_header)), 0))
			{
				prom_printf("\r\n main tclinux.bin crc check error!");
				return -1;
			}
		}
	}
	else if(SLAVE_IMAGE == imageflag)
	{
		//input_data = (char *) (0xb0020000+0x500000);
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
  #ifdef TCSUPPORT_NAND_RT63368
        input_data = (char *) (input_data + offset - 0x20000);
  #else
		input_data = (char *) (input_data+offset);
  #endif		
#elif defined(TCSUPPORT_NAND_BADBLOCK_CHECK)
        input_data = (char *) (flash_base + 0x2280000);
#else
		input_data = (char *) (input_data+TCLINUX_SLAVE_FLASH_START);
#endif
		trx = (struct trx_header *)input_data;
		trx_addr = &(trx->len);
		len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx->crc32);
		crc32 = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx->magic);
		magic = READ_FLASH_DWORD(trx_addr);

//		if ((trx->magic != TRX_MAGIC2) || (trx->len < sizeof(struct trx_header))) 
		if ((magic != TRX_MAGIC2) || (len < sizeof(struct trx_header))) 
		{
			prom_printf("\r\n slave tclinux.bin magic or len check error!");
			return -1;
		}
		else
		{
//			if(trx->crc32 != crc32buf((char *)(input_data+sizeof(struct trx_header)), (trx->len-sizeof(struct trx_header))))
			if(crc32 != crc32buf((char *)(input_data+sizeof(struct trx_header)), (len-sizeof(struct trx_header)), 0))
			{
				prom_printf("\r\n slave tclinux.bin crc check error!");
				return -1;
			}
		}
	}
	else
	{
		prom_printf("\r\n no choose any image ,error!");
		return -1;
	}

	return 0;	
}
#if defined(TCSUPPORT_DUAL_IMAGE_ENHANCE) || defined(TCSUPPORT_NAND_BADBLOCK_CHECK)
unsigned short CheckSum(unsigned short *szBUF,int iSize)
{
	unsigned long ckSum=0;
	for(;iSize>1;iSize-=sizeof(unsigned short))
		ckSum+=*szBUF++;
	if(iSize==1)
		ckSum+=*(unsigned char *)szBUF;
	ckSum=(ckSum>>16)+(ckSum&0xffff);
	ckSum+=(ckSum>>16);
	return(unsigned short )(~ckSum);
}
#endif
#endif

#if defined (TCSUPPORT_GPON_DUAL_IMAGE) || defined (TCSUPPORT_EPON_DUAL_IMAGE)
#ifdef TCSUPPORT_BB_NAND
#if defined(TCSUPPORT_CT_PON)
extern int nand_logic_size;
extern int nand_flash_avalable_size;
extern u32 reservearea_size;
#endif
#endif
char readBootFlagFromFlash(void)
{
	unsigned long retlen = 0;
	unsigned int boot_flag_addr = 0;
	char flag = 0;
#if defined(TCSUPPORT_CT)	
#ifdef TCSUPPORT_BB_NAND
#if defined(TCSUPPORT_CT_PON)
	boot_flag_addr =  flash_base + nand_flash_avalable_size - reservearea_size * TCSUPPORT_RESERVEAREA_BLOCK + IMG_BOOT_FLAG_OFFSET;
#else
	boot_flag_addr =  flash_base + mtd.size - mtd.erasesize * TCSUPPORT_RESERVEAREA_BLOCK + CERM1_RA_OFFSET -1;
#endif
#else
#if defined(TCSUPPORT_RESERVEAREA_EXTEND)
	boot_flag_addr =  flash_base + mtd.size - mtd.erasesize * TCSUPPORT_RESERVEAREA_BLOCK + IMG_BOOT_FLAG_OFFSET;
#else
    boot_flag_addr =  flash_base + mtd.size - mtd.erasesize * TCSUPPORT_RESERVEAREA_BLOCK + CERM1_RA_OFFSET -1;
#endif
#endif
#else
#ifdef TCSUPPORT_BB_NAND
	if (IS_NANDFLASH)
		boot_flag_addr =  flash_base + (devinfo.totalsize << MB_SHIFT) - (devinfo.blocksize << KB_SHIFT) * TCSUPPORT_RESERVEAREA_BLOCK + MRD_RA_OFFSET -1;
	else
#endif
#if (TCSUPPORT_RESERVEAREA_BLOCK==4)
		boot_flag_addr =  flash_base + mtd.size - mtd.erasesize * TCSUPPORT_RESERVEAREA_BLOCK + DEFAULTROMFILE_RA_OFFSET;
#else
		boot_flag_addr =  flash_base + mtd.size - mtd.erasesize * TCSUPPORT_RESERVEAREA_BLOCK + MRD_RA_OFFSET -1;
#endif
#endif
#if 0	
	prom_printf("\n>>>> CTC: flash_base: %x; mtd.size: %x, erasesize: %x, block size: %d, MRD_RA_OFFSET: %x\n",\
			flash_base, mtd.size, mtd.erasesize, TCSUPPORT_RESERVEAREA_BLOCK, MRD_RA_OFFSET);
#endif
	if (IS_NANDFLASH)
		flash_read(boot_flag_addr, 1, &retlen, &flag);
	else	
		memcpy(&flag, boot_flag_addr, 1);
	
	flag = flag - '0';
//	prom_printf("\n>>>> CTC: boot_flag_addr: %x; flag: %d\n", boot_flag_addr, flag);
	if (flag != 0 && flag != 1){
		flag = 0;
	}

	return flag;
}


#define COMMIT_FLAG_ADDR           0x80001ffe
#define DEFAULTROMFILE_RA_SIZE     0x10000
#define DEFAULTROMFILE_RA_OFFSET   0x10000


void exchange_fw(char boot_flag, char commit_flag)
{
	unsigned int flash_dr_addr = 0;
	unsigned long retlen = 0;
	/*set default  romfile physical address*/
	flash_dr_addr = 0xFD0000;
	boot_flag = boot_flag + '0';
	commit_flag = commit_flag + '0';
	prom_printf("\n>>>> MSTC exchange: 3_from %x; len: %d\n", flash_dr_addr, DEFAULTROMFILE_RA_SIZE);
	flash_erase(flash_dr_addr,DEFAULTROMFILE_RA_SIZE);
	/*write boot flag -- image0*/
	flash_write(flash_dr_addr, 1, &retlen, &boot_flag);
	/*write commit flag ---image0*/
	flash_write(flash_dr_addr + 1, 1, &retlen, &commit_flag);
}
unsigned char readCommitFlagFromFlash(void)
{
	unsigned long retlen = 0;
	unsigned int commit_flag_addr = 0;
	unsigned char flag = 0;

	commit_flag_addr =  flash_base + mtd.size - mtd.erasesize * TCSUPPORT_RESERVEAREA_BLOCK + DEFAULTROMFILE_RA_OFFSET + 1;

	memcpy(&flag, commit_flag_addr, 1);
	
	flag = flag - '0';
	prom_printf("\n>>>> MSTC read commit: commit_flag_addr: %x; flag: %d\n", commit_flag_addr, flag);
	if (flag != 0 && flag != 1){
		flag = 0;
	}
	return flag;
}

void writeCommitFlagToFlash(char flag)
{
	unsigned int flash_dr_addr = 0;
	unsigned long retlen = 0;
	/*set default  romfile physical address*/
	flash_dr_addr = 0xFD0000;
	flag = flag + '0';
	prom_printf("\n>>>> MSTC exchange: 2_from %x; len: %d\n", flash_dr_addr, DEFAULTROMFILE_RA_SIZE);
	flash_erase(flash_dr_addr,DEFAULTROMFILE_RA_SIZE);

	/*write commit flag ---image0*/
	flash_write(flash_dr_addr + 1, 1, &retlen, &flag);
	prom_printf("\n>>>> MSTC write flag: commit_flag_addr: %x; flag: %d\n", flash_dr_addr, flag);
	return;
}

#endif

#if defined(TCSUPPORT_CT_UPG_PINGPONG)
void writeBootFlagtoFlash(char flag)
{
	unsigned long retlen = 0;
	unsigned int boot_flag_addr = 0;

	//prom_printf("\n>>>> CTC write flag: boot_flag_addr: %x; flag: %d\n", boot_flag_addr, flag);
	if (flag != 0 && flag != 1){
		prom_printf("\n>>>> boot flag illegal: %d\n", flag);
		return;
	}
	flag = flag + '0';

#if defined(TCSUPPORT_CT)	
#ifdef TCSUPPORT_BB_NAND
#if defined(TCSUPPORT_CT_PON)
	boot_flag_addr =  flash_base + nand_flash_avalable_size - reservearea_size * TCSUPPORT_RESERVEAREA_BLOCK + IMG_BOOT_FLAG_OFFSET;
#else
	boot_flag_addr =  flash_base + mtd.size - mtd.erasesize * TCSUPPORT_RESERVEAREA_BLOCK + CERM1_RA_OFFSET -1;
#endif
#else
	//boot_flag_addr =  flash_base + mtd.size - mtd.erasesize * TCSUPPORT_RESERVEAREA_BLOCK + CERM1_RA_OFFSET -1;
        boot_flag_addr =  flash_base + mtd.size - mtd.erasesize * TCSUPPORT_RESERVEAREA_BLOCK + IMG_BOOT_FLAG_OFFSET;
#endif
#else
#ifdef TCSUPPORT_BB_NAND
	if (IS_NANDFLASH)
		boot_flag_addr =  flash_base + (devinfo.totalsize << MB_SHIFT) - (devinfo.blocksize << KB_SHIFT) * TCSUPPORT_RESERVEAREA_BLOCK + MRD_RA_OFFSET -1;
	else
#endif
		boot_flag_addr =  flash_base + mtd.size - mtd.erasesize * TCSUPPORT_RESERVEAREA_BLOCK + MRD_RA_OFFSET -1;
#endif

#if 0
	prom_printf("\n>>>> CTC: flash_base: %x; mtd.size: %x, erasesize: %x, block size: %d, MRD_RA_OFFSET: %x\n",\
			flash_base, mtd.size, mtd.erasesize, TCSUPPORT_RESERVEAREA_BLOCK, MRD_RA_OFFSET);
#endif
	if (IS_NANDFLASH)
		flash_write(boot_flag_addr, 1, &retlen, &flag);
	else	
		memcpy(boot_flag_addr, &flag, 1);	

	return;
}
#endif

#if defined(TCSUPPORT_IS_FH_PON) || defined(TCSUPPORT_FWC_ENV)
#if defined(TCSUPPORT_FWC_ENV)
static E_FH_NW_PRODUCT_CRC_CHECK_T check_fh_nw_product_kernel_crc(E_FH_NW_PRODUCT_PARTITION_T info)
{
    unsigned long  	 base_addr = 0;
    unsigned long 	    retlen = 0;
    char           *input_data = NULL;
    unsigned int           len = 0;
    unsigned int    kernel_len = 0;
    unsigned int crc_check_sum = 0;
    struct trx_header      trx;
    unsigned char str_name[32];
    memset(str_name, 0, sizeof(str_name));
    switch(info)
    {
        case E_FH_NW_PRODUCT_KERNEL_A: 
        {
            input_data = (char *) (flash_base + FH_NW_PRODUCT_KERNELA_ADDR);
			base_addr = flash_base + FH_NW_PRODUCT_KERNELA_ADDR;
			strcpy(str_name, "kernel A");
            break;
        }
        case E_FH_NW_PRODUCT_KERNEL_B:
        {
            input_data = (char *) (flash_base + FH_NW_PRODUCT_KERNELB_ADDR);
			base_addr = flash_base + FH_NW_PRODUCT_KERNELB_ADDR;
            strcpy(str_name, "kernel B");
            break;
        }
        default:
        {
            prom_printf("\n crc can't support info = %d. line = %d.\n", info, __LINE__);
            return E_FH_NW_PRODUCT_CHECK_WRONG;
        }
    }
    
    memset(&trx, 0, sizeof(trx));
    flash_read(base_addr, sizeof(struct trx_header), &retlen, (unsigned char *)&trx);
    crc_check_sum = trx.crc32;
    len = trx.len;
    
    if((0xFFFFFFFF == len) || (0xFFFFFFFF == crc_check_sum) || (len <= sizeof(struct trx_header)))
    {
		return E_FH_NW_PRODUCT_CHECK_WRONG;
    }
    if(crc_check_sum != crc32buf((char *)(input_data+sizeof(struct trx_header)), (len-sizeof(struct trx_header)), 0))
    {
        prom_printf("\n %s crc check wrong. line = %d.\n", str_name, __LINE__);
        return E_FH_NW_PRODUCT_CHECK_WRONG;
    }
    else
    {
        prom_printf("\n %s crc check right. line = %d.\n", str_name, __LINE__);
    }

    return E_FH_NW_PRODUCT_CHECK_RIGHT;
}

static int kernel_try_start(E_FH_NW_PRODUCT_START_UP_MARK_T flag)
{	
    int                             iret = 0;
    unsigned char               str_name[32];
    unsigned long           output_data  = KERNEL_START_ADDR;;
    unsigned long           free_mem_ptr = KERNEL_RAM_START;
    unsigned long       free_mem_ptr_end = KERNEL_RAM_END;
    unsigned char       k_decompress_ret = 0;
    E_FH_NW_PRODUCT_CRC_CHECK_T  crc_ret = E_FH_NW_PRODUCT_CHECK_RIGHT;
    E_FH_NW_PRODUCT_PARTITION_T     info = E_FH_NW_PRODUCT_KERNEL_A;
	
    memset(str_name, 0, sizeof(str_name));
    if(E_FH_NW_PRODUCT_START_UP_B == flag)
    {
        info = E_FH_NW_PRODUCT_KERNEL_B;
        strcpy(str_name, "kernel B");
    }
    else
    {
        info = E_FH_NW_PRODUCT_KERNEL_A;
        strcpy(str_name, "kernel A");
    }

    crc_ret = check_fh_nw_product_kernel_crc(info);
    if(E_FH_NW_PRODUCT_CHECK_RIGHT != crc_ret)
    {
        return -1;
    }
	
    if(E_FH_NW_PRODUCT_KERNEL_B == info)
    {
        set_lzma_addr((flash_base+FH_NW_PRODUCT_KERNELB_ADDR+sizeof(struct trx_header)), (flash_base+FH_NW_PRODUCT_KERNELB_ADDR+FH_NW_PRODUCT_KERNEL_SIZE-sizeof(struct trx_header)));
        k_decompress_ret = decompress_kernel(output_data, free_mem_ptr, free_mem_ptr_end);
        if(E_FH_NW_PRODUCT_DECOMPRESS_RIGHT != k_decompress_ret)
        {
            prom_printf("\n decompress_kernel %s fail. line = %d.\n", str_name, __LINE__);
            return -1;
        }
    }
    else
    {
        set_lzma_addr((flash_base+FH_NW_PRODUCT_KERNELA_ADDR+sizeof(struct trx_header)),(flash_base+FH_NW_PRODUCT_KERNELA_ADDR+FH_NW_PRODUCT_KERNEL_SIZE-sizeof(struct trx_header)));
        k_decompress_ret = decompress_kernel(output_data, free_mem_ptr, free_mem_ptr_end);
        if(E_FH_NW_PRODUCT_DECOMPRESS_RIGHT != k_decompress_ret)
        {
            prom_printf("\n decompress_kernel %s fail. line = %d.\n", str_name, __LINE__);
            return -1;
        }
    }

    prom_printf("Decompress to %X free_mem_ptr=%X free_mem_ptr_end=%X\n", output_data, free_mem_ptr, free_mem_ptr_end);

    return 0;
}

static int get_partition_start_up_flag(E_FH_NW_PRODUCT_ACTIVE_MARK_T active_flag,  E_FH_NW_PRODUCT_COMMIT_MARK_T commit_flag)
{
	E_FH_NW_PRODUCT_START_UP_MARK_T start_up_flag = E_FH_NW_PRODUCT_START_UP_A;

	if(E_FH_NW_PRODUCT_ACTIVE_DEFAULT == active_flag)
	{
		if(E_FH_NW_PRODUCT_COMMIT_B == commit_flag)
		{
			start_up_flag = E_FH_NW_PRODUCT_START_UP_B;
		}
		else
		{
			start_up_flag = E_FH_NW_PRODUCT_START_UP_A;
		}
	}
	else if(E_FH_NW_PRODUCT_ACTIVE_B == active_flag)
	{
		start_up_flag = E_FH_NW_PRODUCT_START_UP_B;
	}
	else 
	{
		start_up_flag = E_FH_NW_PRODUCT_START_UP_A;
	}

	return start_up_flag;
}

static void prepare_bootenv_2_kernel(void)
{
	int                           				iret = 0;
	dual_image_flag_t                     dual_image_env;
	personality_parm_flash_env_t 			   flash_env;
	personality_parm_memory_env_t 			  memory_env;
	personality_parm_boot_to_kernel_info_t to_kernel_env;
	personality_parm_boot_to_kernel_info_t *to_kernel_env_addr = (personality_parm_boot_to_kernel_info_t *) FH_NW_PRODUCT_BOOT2KRENEL_ENV_ADDR;

	memset(&to_kernel_env, 0, sizeof(to_kernel_env));
	/*prepare dual image flag env info*/
	memset(&dual_image_env, 0, sizeof(dual_image_env));
	iret  = read_fh_nw_product_dual_image_flag_env(&dual_image_env);
	if(0 != iret)
	{
		memset(&dual_image_env, 0, sizeof(dual_image_env));
	}
	to_kernel_env.realtime_env.app1_active   = dual_image_env.app1_active;
	to_kernel_env.realtime_env.app1_commit   = dual_image_env.app1_commit;
	to_kernel_env.realtime_env.app2_active   = dual_image_env.app2_active;
	to_kernel_env.realtime_env.app2_commit   = dual_image_env.app2_commit;
	to_kernel_env.realtime_env.kernel_active = dual_image_env.kernel_active;
	to_kernel_env.realtime_env.kernel_commit = dual_image_env.kernel_commit;
	to_kernel_env.realtime_env.rootfs_active = dual_image_env.rootfs_active;
	to_kernel_env.realtime_env.rootfs_commit = dual_image_env.rootfs_commit;

	/*prepare flash env info*/
	memset(&flash_env, 0, sizeof(flash_env));
	iret = read_fh_nw_product_flash_env(&flash_env);
	if(0 != iret)
	{
		memset(&flash_env, 0, sizeof(flash_env));
		strcpy(flash_env.eth_addr, "00:0A:C2:12:34:56");
		strcpy(flash_env.hw_cfg,   "0x77");
		strcpy(flash_env.sn_oui,   "00");
	}
    memcpy((char *)&to_kernel_env+sizeof(realtime_dual_image_env_t), &flash_env, sizeof(flash_env));

	/*prepare memory env info*/
	memset(&memory_env, 0, sizeof(memory_env));
    iret = read_fh_nw_product_memory_env(&memory_env);
    if(0 != iret)
    {
		memset(&memory_env, 0, sizeof(memory_env));
		strcpy(memory_env.boot_version, "V1");
		strcpy(memory_env.m_reserve0, PERSONALITY_PARM_DUAL_IMAGE_WRITE_PARTITION);
    }

	memcpy((char *)&to_kernel_env+sizeof(dual_image_env)+sizeof(flash_env), &memory_env ,sizeof(memory_env));

	/*prepare env info 2 kernel*/
	memcpy(to_kernel_env_addr, &to_kernel_env, sizeof(to_kernel_env));
	
	return ;
}
void boot_kernel(void)
{
    int                      				   iret = 0;
    unsigned long       				  startAddr = 0x0;
    unsigned int      					kernel_size = 0;
    E_FH_NW_PRODUCT_COMMIT_MARK_T     kernel_commit = E_FH_NW_PRODUCT_COMMIT_A;
    E_FH_NW_PRODUCT_ACTIVE_MARK_T     kernel_active = E_FH_NW_PRODUCT_ACTIVE_DEFAULT;
	E_FH_NW_PRODUCT_COMMIT_MARK_T     rootfs_commit = E_FH_NW_PRODUCT_COMMIT_A;
	E_FH_NW_PRODUCT_ACTIVE_MARK_T     rootfs_active = E_FH_NW_PRODUCT_ACTIVE_DEFAULT;
    E_FH_NW_PRODUCT_START_UP_MARK_T      kernel_run = E_FH_NW_PRODUCT_START_UP_A;
    E_FH_NW_PRODUCT_START_UP_MARK_T      rootfs_run = E_FH_NW_PRODUCT_START_UP_A;
    E_FH_NW_PRODUCT_ROOLBACK_MARK_T kernel_roolback = E_FH_NW_PRODUCT_ROOLBACK_FALSE;
	personality_parm_boot_to_kernel_info_t *to_kernel_env_addr = (personality_parm_boot_to_kernel_info_t *) FH_NW_PRODUCT_BOOT2KRENEL_ENV_ADDR;

    void (*jumpAddr)();
    /* Disable all hardware interrupts */
    change_cp0_status(ST0_IM, 0x00);

    tc_outl(CR_INTC_IMR, 0x0);
    tc_outl(CR_TIMER_CTL, 0x0);
    tc_outl(CR_MAC_MACCR, 0x0);

	/*prepare boot2kernel env*/
	prepare_bootenv_2_kernel();
	kernel_commit = to_kernel_env_addr->realtime_env.kernel_commit;
	kernel_active = to_kernel_env_addr->realtime_env.kernel_active;
	rootfs_commit = to_kernel_env_addr->realtime_env.rootfs_commit;
	rootfs_active = to_kernel_env_addr->realtime_env.rootfs_active;

	/*try start kernel from active && commit flag*/
	kernel_run = get_partition_start_up_flag(kernel_active, kernel_commit);
    iret = kernel_try_start(kernel_run);
    if(0 != iret)
    {
    	kernel_roolback = E_FH_NW_PRODUCT_ROOLBACK_TRUE;
        if(E_FH_NW_PRODUCT_START_UP_A == kernel_run)
        {
            kernel_run = E_FH_NW_PRODUCT_START_UP_B;
        }
        else
        {
            kernel_run = E_FH_NW_PRODUCT_START_UP_A;
        }

        iret = kernel_try_start(kernel_run);
        {
            if(0 != iret)
            {
                prom_printf("kernel A && B can't start up.\n");
                prom_printf("\n -- System halted");
                while(1);
            }
        }
    }

    /*decide mount master/slave rootfs*/
    rootfs_run = get_partition_start_up_flag(rootfs_active, rootfs_commit);
    /*boot2kernel env change*/
    to_kernel_env_addr->realtime_env.kernel_run      = kernel_run;
    to_kernel_env_addr->realtime_env.kernel_roolback = kernel_roolback;
	to_kernel_env_addr->realtime_env.rootfs_run      = rootfs_run;

    kernel_size = FH_NW_PRODUCT_KERNEL_SIZE - sizeof(struct trx_header);
    startAddr	= 0x80002000;
    VPint(0xbfb00240) = 0;
    prom_printf("75xx: 0x%x\n",0);

    if(startAddr != 0)
    {
        jumpAddr = (void (*)(void))(startAddr);
    }
    else
    {
        jumpAddr = (void (*)(void))(0x80020000);
    }

#ifdef TC3262
    if (IS_NANDFLASH )
        flush_icache_range(startAddr, startAddr + kernel_size*4);
#endif

    (*jumpAddr)();
}

#else
char readBootFlagFromFlash_FH(void)
{
	unsigned long retlen = 0;
	unsigned int boot_flag_addr = 0x1e0000;
	unsigned char flag = 1;
	int i=0  ;
	char buf[16];
	
	flash_read(boot_flag_addr, 1, &retlen, &flag);	
	
	flag = flag - '0';
		
	if (flag != 0 && flag != 1){
		flag = 0;
	}

	return flag;
}

char readRootFlagFromFlash_FH(void)
{
	unsigned long retlen = 0;
	unsigned int root_flag_addr = 0x1e0004;
	unsigned char flag = 1;
	int i=0  ;
	char buf[16];
	
	flash_read(root_flag_addr, 1, &retlen, &flag);	
	
	flag = flag - '0';
		
	if (flag != 0 && flag != 1){
		flag = 0;
	}

	return flag;
}

void boot_kernel(void)
{
	unsigned long output_data;
	unsigned long free_mem_ptr;
	unsigned long free_mem_ptr_end;
    void (*jumpAddr)();

	output_data = KERNEL_START_ADDR;
	free_mem_ptr = KERNEL_RAM_START;
	free_mem_ptr_end = KERNEL_RAM_END;
	unsigned long retlen;

	/* Disable all hardware interrupts */
	change_cp0_status(ST0_IM, 0x00);

	tc_outl(CR_INTC_IMR, 0x0);
	tc_outl(CR_TIMER_CTL, 0x0);
	tc_outl(CR_MAC_MACCR, 0x0);

	prom_printf("Decompress to %X free_mem_ptr=%X free_mem_ptr_end=%X\n", output_data, free_mem_ptr, free_mem_ptr_end);
	//modify by ywxing 2015-05-05 for dual image
	if(readBootFlagFromFlash_FH()==1)//bootfromB
	{
		if (IS_NANDFLASH)
				flash_read(flash_base+FH_KERNELB_ADDR, FH_KERNEL_LENS, &retlen, 0x80020000);
		else	
				memcpy(0x80020000,flash_base+0x20000, 0x200000);


		set_lzma_addr(flash_base + FH_KERNELB_ADDR,flash_base+FH_KERNELB_ADDR+FH_KERNEL_LENS);
	}
	else
	{
		if (IS_NANDFLASH)
				flash_read(flash_base+FH_KERNELA_ADDR, FH_KERNEL_LENS, &retlen, 0x80020000);
		else	
				memcpy(0x80020000,flash_base+0x20000, 0x200000);


		set_lzma_addr(flash_base + FH_KERNELA_ADDR,flash_base+FH_KERNELA_ADDR+FH_KERNEL_LENS);
	}
	char *bufaddr = (char*)FH_ROOTFLAG_ADDR;
	*bufaddr = readRootFlagFromFlash_FH();
	prom_printf("bootflag=%s\nrootflag=%s\n",readBootFlagFromFlash_FH(),readRootFlagFromFlash_FH() );
	//modify end
#if defined(TCSUPPORT_CPU_MT7510)||defined(TCSUPPORT_CPU_MT7520)
	uint32 tmpId = 0;
	uint8 idx = 0;

	idx = READ_FLASH_BYTE( (0xff9b | flash_base));
	switch(idx){
	case 1: // 7510 F
		tmpId = 0x80; 
		break;
	case 2: // 7520 F
		tmpId = 0x100;
		break;
	case 3: // 7525G 7520G 7510 7511 
		tmpId = 0x53;
		break;
	case 4: // 7511G
		tmpId = 0x4;
		break;
	case 5: // 7525 7520
		tmpId = 0x28;
		break;
	case 6: // 7520 F External
		tmpId = 0x200;
	default:
		break;
	}
	prom_printf("75xx: 0x%x\n",tmpId);
	VPint(0xbfb00240) = tmpId;
#elif defined(TCSUPPORT_CPU_MT7505)
	VPint(0xbfb00240) = 0;
#endif//MT7510

    output_data = 0x80002000;
	decompress_kernel(output_data, free_mem_ptr, free_mem_ptr_end);

	jumpAddr = (void (*)(void))(0x80002000);
			
#ifdef TC3262
	if (IS_NANDFLASH )
		flush_icache_range(0x80002000, 0x80002000 + FH_KERNEL_LENS*4);
#endif
	(*jumpAddr)();
}
#endif
#else
void boot_kernel(void)
{
	unsigned long output_data;
	unsigned long free_mem_ptr;
	unsigned long free_mem_ptr_end;
    void (*jumpAddr)();
	unsigned long retlen = 0;
	
#ifdef CONFIG_DUAL_IMAGE
	int retflag = 0;
	char *bufaddr = (char*)FLAG_ADDR;
	char flag = 0;
	
	unsigned char commit_flag = 0;
	char *commit_buf_addr = NULL;
	
#if defined(TCSUPPORT_DUAL_IMAGE_ENHANCE) || defined(TCSUPPORT_NAND_BADBLOCK_CHECK)
	char* trx_addr = NULL;
	unsigned char slave_ver[32], main_ver[32];
	int index;
	unsigned int header_len, rootfs_len, kernel_len;
	int binlen = 0;
	struct trx_header *trx_main = NULL;
	struct trx_header *trx_slave = NULL;
	unsigned short checkSum_main = 0;
	unsigned short checkSum_slave = 0;
	char checkflag = 0;
#endif
#endif	
	struct trx_header *trx_temp = NULL;
	char* temp_addr = NULL;
	unsigned long startAddr = 0x0;
	unsigned int kernel_size = 0;
	int trx_header_len = sizeof(struct trx_header);

	output_data = KERNEL_START_ADDR;
	free_mem_ptr = KERNEL_RAM_START;
	free_mem_ptr_end = KERNEL_RAM_END;

	/* Disable all hardware interrupts */
	change_cp0_status(ST0_IM, 0x00);

	tc_outl(CR_INTC_IMR, 0x0);
	tc_outl(CR_TIMER_CTL, 0x0);
	tc_outl(CR_MAC_MACCR, 0x0);

#ifdef TR068_LED	
#if !defined(TCSUPPORT_C1_MS) 
	power_gpioInit(POWER_LED_RED);
#endif
#endif	
#if !defined(CONFIG_DUAL_IMAGE) && defined(TR068_LED)
	if( checkimage(MAIN_IMAGE) < 0 ) {
		prom_printf("\r\n==>Main image check failed, turn on red led!");
#if defined(TCSUPPORT_CT)
		power_gpioInit(POWER_LED_RED);//main image CRC check failed, power led turn on red
#endif
		return;
	}
#endif

#ifdef CONFIG_DUAL_IMAGE
	if(checkimage(MAIN_IMAGE) < 0)
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
		trx_temp = (struct trx_header*)(flash_base+flash_tclinux_start+offset);
#elif defined(TCSUPPORT_NAND_BADBLOCK_CHECK)
        trx_temp = (struct trx_header*)(flash_base+0x2280000);  //tclinux.bin slave offset
#else
		trx_temp = (struct trx_header*)(flash_base+flash_tclinux_start+TCLINUX_SLAVE_FLASH_START);
#endif
	else
#endif

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
    trx_temp = (struct trx_header*)(flash_base+0x280000);
#elif defined(TCSUPPORT_NAND_RT63368)
    trx_temp = (struct trx_header*)(flash_base + NAND_KERNEL_OFFSET);
#else
	trx_temp = (struct trx_header*)(flash_base+flash_tclinux_start);
#endif
	
  #if defined(TCSUPPORT_NAND_BADBLOCK_CHECK) || defined(TCSUPPORT_NAND_RT63368)
    temp_addr = &(trx_temp->kernel_len);
    kernel_size = READ_FLASH_DWORD(temp_addr) + 0x100;
  #else
	temp_addr = &(trx_temp->kernel_len);
    kernel_size = READ_FLASH_DWORD(temp_addr) + 0x100;
	kernel_size = (kernel_size+0x10000)&~(0x10000-1);
  #endif
	temp_addr = &(trx_temp->decompAddr);
	startAddr = READ_FLASH_DWORD(temp_addr);
	if(startAddr != 0)
	{
		output_data = startAddr;
#ifdef CONFIG_DUAL_IMAGE
		bufaddr = (char*)(startAddr-1);
#endif
	}
	else
	{
		output_data = 0x80020000;
#ifdef CONFIG_DUAL_IMAGE
		bufaddr = (char*)(0x8001ffff);
#endif
	}
	
#ifdef CONFIG_DUAL_IMAGE
#if defined (TCSUPPORT_GPON_DUAL_IMAGE) || defined (TCSUPPORT_EPON_DUAL_IMAGE)
	flag = readBootFlagFromFlash();

	prom_printf("\r\n==> boot flag = %d",flag);
	
	if(flag == 0)
	{
#endif /*TCSUPPORT_GPON_DUAL_IMAGE||TCSUPPORT_EPON_DUAL_IMAGE*/
	retflag = checkimage(MAIN_IMAGE);
	if(retflag < 0)
	{
		flag = 1;
		retflag = checkimage(SLAVE_IMAGE);
		if(retflag < 0) {
	#ifdef TR068_LED
			prom_printf("\r\n==>Dual image check failed, turn on red led!");
#if defined(TCSUPPORT_CT)
			power_gpioInit(POWER_LED_RED);//both images CRC check are failed, power led turn on red
#endif
	#endif
			return;
		}
#if defined(TCSUPPORT_CT_UPG_PINGPONG)
		writeBootFlagtoFlash(flag);
#endif


	}
#if defined (TCSUPPORT_GPON_DUAL_IMAGE) || defined (TCSUPPORT_EPON_DUAL_IMAGE)
	}
	else
	{
		retflag = checkimage(SLAVE_IMAGE);
		if(retflag < 0)
		{
			flag = 0;
			retflag = checkimage(MAIN_IMAGE);
			if(retflag < 0) {
#ifdef TR068_LED
				prom_printf("\r\n==>Dual image check failed, turn on red led!");
#if defined(TCSUPPORT_CT)
				power_gpioInit(POWER_LED_RED);//both images CRC check are failed, power led turn on red
#endif
#endif
				return;
			}
#if defined(TCSUPPORT_CT_UPG_PINGPONG)
			writeBootFlagtoFlash(flag);
#endif

		}	
	}
#endif /*TCSUPPORT_GPON_DUAL_IMAGE||TCSUPPORT_EPON_DUAL_IMAGE*/

	//store flag information to 0x8001ffff
	*bufaddr = flag;


#endif	
	prom_printf("Decompress to %X free_mem_ptr=%X free_mem_ptr_end=%X\n", output_data, free_mem_ptr, free_mem_ptr_end);

#ifdef CONFIG_DUAL_IMAGE
	/*flag 0:main image;1:backup image*/
	/* frankliao added 20101215 */
	if (flag==1){ 
		prom_printf("from slave\n");
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
//		set_lzma_addr(0xb0020100+offset, 0xb0120000+offset);
	//	set_lzma_addr(flash_base+0x20100+offset, flash_base+0x120000+offset);
		set_lzma_addr(flash_base+flash_tclinux_start+trx_header_len+offset, flash_base+flash_tclinux_start+kernel_size+offset);
	#if !defined(TCSUPPORT_EPON_DUAL_IMAGE) && !defined(TCSUPPORT_GPON_DUAL_IMAGE)
		//copy slave to main
//		trx_slave = (struct trx_header *)(0xb0000000+0x20000+offset);
		trx_slave = (struct trx_header*)(flash_base+flash_tclinux_start+offset);
		trx_addr = &(trx_slave->header_len);
		header_len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx_slave->kernel_len);
		kernel_len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx_slave->rootfs_len);
		rootfs_len = READ_FLASH_DWORD(trx_addr);
//		binlen = trx_slave->header_len + trx_slave->kernel_len + trx_slave->rootfs_len;
		binlen = header_len + kernel_len + rootfs_len;
  #ifdef TCSUPPORT_NAND_RT63368
        flash_erase(NAND_KERNEL_OFFSET, binlen);
  #else
		flash_erase(flash_tclinux_start, binlen);
  #endif		
		if (IS_NANDFLASH)
			flash_read(flash_base+flash_tclinux_start+offset, binlen, &retlen, 0x80020000);
		else	
			memcpy(0x80020000, flash_base+flash_tclinux_start+offset, binlen);
  #ifdef TCSUPPORT_NAND_RT63368  			
		flash_write(NAND_KERNEL_OFFSET, binlen, &retlen, 0x80020000);
  #else
		flash_write(flash_tclinux_start, binlen, &retlen, 0x80020000);
  #endif
	#endif /*!TCSUPPORT_EPON_DUAL_IMAGE*/

#elif defined(TCSUPPORT_NAND_BADBLOCK_CHECK)
		set_lzma_addr(flash_base+0x2280000+0x100, flash_base+0x2280000+kernel_size);

		trx_slave = (struct trx_header*)(flash_base+0x2280000);
		trx_addr = &(trx_slave->header_len);
		header_len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx_slave->kernel_len);
		kernel_len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx_slave->rootfs_len);
		rootfs_len = READ_FLASH_DWORD(trx_addr);
		binlen = header_len + kernel_len + rootfs_len;

        flash_erase(0x280000, binlen);
        flash_read(flash_base+0x2280000, binlen, &retlen, 0x80020000);
        flash_write(0x280000, binlen, &retlen, 0x80020000);
		
#else
//		set_lzma_addr(0xb0020100+0x500000, 0xb0120000+0x500000);
	//	set_lzma_addr( (flash_base + 0x20100 + 0x500000), (flash_base + 0x120000 + 0x500000) );
		set_lzma_addr(flash_base + flash_tclinux_start+trx_header_len+ TCLINUX_SLAVE_FLASH_START, flash_base + flash_tclinux_start+kernel_size+ TCLINUX_SLAVE_FLASH_START);

#endif
	} else {
		prom_printf("from main\n");
	//	set_lzma_addr( (flash_base + 0x20100), (flash_base + 0x120000) );
#ifdef 	TCSUPPORT_NAND_BADBLOCK_CHECK
        set_lzma_addr( (flash_base + 0x280000 + 0x100), (flash_base + 0x280000 + kernel_size) );  //Pomelo
#elif defined(TCSUPPORT_NAND_RT63368)
        set_lzma_addr( (flash_base + NAND_KERNEL_OFFSET + 0x100), (flash_base + NAND_KERNEL_OFFSET + kernel_size) );
#else
		set_lzma_addr( (flash_base + flash_tclinux_start+trx_header_len), (flash_base + flash_tclinux_start + kernel_size) );
#endif

#if defined (TCSUPPORT_DUAL_IMAGE_ENHANCE) && !defined (TCSUPPORT_EPON_DUAL_IMAGE) && !defined (TCSUPPORT_GPON_DUAL_IMAGE)
		//copy main to slave
//		trx_main = (struct trx_header *)(0xb0000000+0x20000);
  #ifdef TCSUPPORT_NAND_RT63368
        trx_main = (struct trx_header*)(flash_base+NAND_KERNEL_OFFSET);
  #else
		trx_main = (struct trx_header*)(flash_base+flash_tclinux_start);
  #endif		
		trx_addr = &(trx_main->header_len);
		header_len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx_main->kernel_len);
		kernel_len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx_main->rootfs_len);
		rootfs_len = READ_FLASH_DWORD(trx_addr);
//		binlen = trx_main->header_len + trx_main->kernel_len + trx_main->rootfs_len;
		binlen = header_len + kernel_len + rootfs_len;
//		trx_slave = (struct trx_header *)(0xb0000000+0x20000+offset);
		trx_slave = (struct trx_header *)(flash_base+flash_tclinux_start+offset);
		trx_addr = &(trx_slave->version);
		for (index=0; index<32;  index++, trx_addr++){
			slave_ver[index] = READ_FLASH_BYTE(trx_addr);
		}
		trx_addr = &(trx_main->version);
		for (index=0; index<32;  index++, trx_addr++){
			main_ver[index] = READ_FLASH_BYTE(trx_addr);
		}

//		if((checkimage(SLAVE_IMAGE) < 0) || (strcmp(trx_main->version,trx_slave->version) != 0)) {
		if((checkimage(SLAVE_IMAGE) < 0) || (strcmp(main_ver, slave_ver) != 0)) {
			checkflag = 1;
		}
		if(checkflag == 0){
			if (IS_NANDFLASH)
			  #ifdef TCSUPPORT_NAND_RT63368
                flash_read(flash_base+NAND_KERNEL_OFFSET, binlen, &retlen, 0x80020000);
			  #else
				flash_read(flash_base+flash_tclinux_start, binlen, &retlen, 0x80020000);
			  #endif	
			else
				memcpy(0x80020000, flash_base+flash_tclinux_start, binlen);
			checkSum_main = CheckSum(0x80020000,binlen);
			if (IS_NANDFLASH)
				flash_read(flash_base+flash_tclinux_start+offset, binlen, &retlen, 0x80020000);
			else	
				memcpy(0x80020000, flash_base+flash_tclinux_start+offset, binlen);
			checkSum_slave = CheckSum(0x80020000,binlen);
			if(checkSum_main != checkSum_slave){
				checkflag = 1;	
			}	
		}
		if(checkflag == 1){
			flash_erase(flash_tclinux_start+offset, binlen);
			if (IS_NANDFLASH)
			  #ifdef TCSUPPORT_NAND_RT63368
                flash_read(flash_base+NAND_KERNEL_OFFSET, binlen, &retlen, 0x80020000);
			  #else
				flash_read(flash_base+flash_tclinux_start, binlen, &retlen, 0x80020000);
			  #endif	
			else	
				memcpy(0x80020000, flash_base+flash_tclinux_start, binlen);
			flash_write(flash_tclinux_start+offset, binlen, &retlen, 0x80020000);
		}
#elif defined(TCSUPPORT_NAND_BADBLOCK_CHECK)
        trx_main = (struct trx_header*)(flash_base+0x280000);
        trx_addr = &(trx_main->header_len);
		header_len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx_main->kernel_len);
		kernel_len = READ_FLASH_DWORD(trx_addr);
		trx_addr = &(trx_main->rootfs_len);
		rootfs_len = READ_FLASH_DWORD(trx_addr);
		binlen = header_len + kernel_len + rootfs_len;

		trx_slave = (struct trx_header *)(flash_base+0x2280000);
		
		trx_addr = &(trx_slave->version);

		for (index=0; index<32;  index++, trx_addr++){
			slave_ver[index] = READ_FLASH_BYTE(trx_addr);
		}
		trx_addr = &(trx_main->version);
		for (index=0; index<32;  index++, trx_addr++){
			main_ver[index] = READ_FLASH_BYTE(trx_addr);
		}

		if((checkimage(SLAVE_IMAGE) < 0) || (strcmp(main_ver, slave_ver) != 0)) {
			checkflag = 1;
		}

		if(checkflag == 0){
			flash_read(flash_base+0x280000, binlen, &retlen, 0x80020000);

			checkSum_main = CheckSum(0x80020000,binlen);
	
			flash_read(flash_base+0x2280000, binlen, &retlen, 0x80020000);

			checkSum_slave = CheckSum(0x80020000,binlen);
			if(checkSum_main != checkSum_slave){
				checkflag = 1;	
			}	
		}
		
		if(checkflag == 1){
			flash_erase(0x2280000, binlen);
			flash_read(flash_base+0x280000, binlen, &retlen, 0x80020000);
			flash_write(0x2280000, binlen, &retlen, 0x80020000);
		}

#endif
	}
#else
//	set_lzma_addr(KERNEL_FLASH_START, KERNEL_FLASH_END);

 #ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
    set_lzma_addr( (flash_base + 0x280000 + 0x100), (flash_base + 0x280000 + 0x180000) );
 #elif defined(TCSUPPORT_NAND_RT63368)
    set_lzma_addr( (flash_base + NAND_KERNEL_OFFSET + 0x100), (flash_base + NAND_KERNEL_OFFSET + 0x1a0000) );
 #else
	set_lzma_addr( (flash_base + flash_tclinux_start+trx_header_len), (flash_base + + flash_tclinux_start + kernel_size) );
 #endif

#if defined(TCSUPPORT_CPU_MT7510)||defined(TCSUPPORT_CPU_MT7520)
	uint32 tmpId = 0;
	uint8 idx = 0;

	idx = READ_FLASH_BYTE( (0xff9b | flash_base));
	switch(idx){
	case 1: // 7510 F
		tmpId = 0x80; 
		break;
	case 2: // 7520 F
		tmpId = 0x100;
		break;
	case 3: // 7525G 7520G 7510 7511 
		tmpId = 0x53;
		break;
	case 4: // 7511G
		tmpId = 0x4;
		break;
	case 5: // 7525 7520
		tmpId = 0x28;
		break;
	case 6: // 7520 F External
		tmpId = 0x200;
	default:
		break;
	}
	prom_printf("75xx: 0x%x\n",tmpId);
	VPint(0xbfb00240) = tmpId;
#elif defined(TCSUPPORT_CPU_MT7505)
	VPint(0xbfb00240) = 0;
#endif//MT7510
#endif

	decompress_kernel(output_data, free_mem_ptr, free_mem_ptr_end);

	/* FIXME */
/*Unify the kernel address to jump for all chip, shnwind*/
//#ifdef TC3262
//	jumpAddr = (void (*)(void))0x80020000;
//#else
//	if (isTC3162L3P3 || isTC3162L4P4 || isTC3162L5P5) {
	
		//jumpAddr = (void (*)(void))KERNEL_START_ADDR;
		if(startAddr != 0)
			jumpAddr = (void (*)(void))(startAddr);
		else
			jumpAddr = (void (*)(void))(0x80020000);
//	} else {
		//jumpAddr = (void (*)(void))0x80020404;
		//jumpAddr = (void (*)(void))0x80020420;
		//jumpAddr = (void (*)(void))0x800203fc;
//		jumpAddr = (void (*)(void))0x800205bc;
//	}
//#endif
#ifdef TC3262
	if (IS_NANDFLASH )
		flush_icache_range(startAddr, startAddr + kernel_size*4);
#endif
	(*jumpAddr)();
}
#endif


#if defined(TCSUPPORT_MULTI_UPGRADE)
/*
	this flag will be set when the first multicast packet is received
*/
extern char startMultiUpgrade;
#endif

#if defined(TCSUPPORT_MULTI_BOOT)
extern char startMultiBoot;
#endif

#if defined(TCSUPPORT_MULTI_UPGRADE) || defined(TCSUPPORT_MULTI_BOOT)
#ifdef TC3262
extern void resetSwMAC3262(void);
#else
extern void resetSwMAC(void);
#endif
#endif

static int tc3162wdog_reset()
{
	unsigned int word;

	if (isRT63365){
		/* check if not e1 chip */
		if ((VPint(0xbfb00064) & 0xffff) != 0x0){
			/* check if do watch do reset */
			if (!(VPint(0xbfb00080) >> 31)){

				/* set delay counter */
				VPint(CR_TIMER5_LDV) = 1000;

				/* enable watch dog timer */
				word = VPint(CR_TIMER_CTL);
				word |= ( (1<<5) | (1<<25) );
				VPint (CR_TIMER_CTL) = word;

				while(1);
			}	
		}	
	}	

	return 0;
}

#ifdef MT7550_GPIO
//SUC
//normal mode control mt7505+7550 pll
#define RG_SCU_BASE							0xBFA20000
#define RG_DSLPLL_EN_SCU					(RG_SCU_BASE + 0x01*4)
#define RG_DSL_HRDDS_PCW_NCP0_SCU		(RG_SCU_BASE + 0x04*4)
#define RG_DSL_HRDDS_PCW_NCP0_CHG_SCU	(RG_SCU_BASE + 0x05*4)
#define RG_DMT_CTRL							0xBFB00084
#endif

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
	if(isEN751221){
		if (((VPint(0xbfa20000) & (0x1)) != 0)) {
		VPint(0xbfa20000) &= ~(0x01);
	}
		/*Disable FE Share Memory*/
		VPint(0xbfb00958) = 0;
	}

	tc3162wdog_reset();
	led_init();
	led_all_mode(0);

	trap_init();
	uart_init();
#ifdef TCSUPPORT_UART2
	uart2_init();
#endif
	prom_printf("\n\n");

#if defined(MT7505_SWITCH_TBL_VERIRY)
	MT7505_Switch_Tbl_Test();
#endif

#ifdef TC3262
	if (isTC3182)
		prom_printf("TC3182");
	else if (isRT65168)
		prom_printf("RT65168");
	else if (isRT63165)
		prom_printf("RT63165");
	else if (isRT63365)
		prom_printf("RT63365");
	else if (isMT751020)
		prom_printf("MT751020");
	else if(isMT7505)
		prom_printf("MT7505");		
	else if(isEN751221)
		prom_printf("EN751221");		
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

#ifdef TCSUPPORT_AUTOBENCH
	if(isRT63365){
		val = tc_inl(0xbfb00860);
		tc_outl(0xbfb00860, (val & (~((1<<12) | (1<<13) | (1<<10) | (1<<11) | (1<<9)))));
	}
#endif

#ifdef SIS_DDR_PHY
	if (!isRT63165 && !isRT63365 && !isMT751020 && !isMT7505 && !isEN751221) {
		prom_printf("DDR EDQS scan min=%d max=%d choose EDQS=%d\n\n", 
			VPint(0x8001fffc), VPint(0x8001fff8), VPint(0x8001fff4));
	}
#else
#ifdef PLL_AUTO_SCAN
	prom_printf("DDR PLL possible bitmap=0x%08x choose PLL=%d\n\n", 
			VPint(0x8001fffc), VPint(0x8001fff8));
#endif
#endif

	prom_printf("Memory size %dMB\n\n",calculate_dram_size());

#if defined(TCSUPPORT_POWERSAVE_ENABLE)
	// enable DDR self refresh
	if((!isMT751020) && ((VPint(CR_AHB_HWCONF)&(1<<25))!=0))
    {
		VPint(CR_DMC_DDR_SR_CNT) &= 0xff000000;
		VPint(CR_DMC_DDR_SR_CNT) |= 0x1;
	}
#endif
#if 0
#ifdef TC3262
	prom_printf("Memory size %dMB\n\n", 
			8 * (1 << (((VPint(CR_DMC_CTL1) >> 18) & 0x7) - 1)));
#else
{
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
	
	prom_printf("Memory size %dMB\n\n", memsize>>20);
}
#endif

#endif
	setup_arch();    /*setup the BEV0,and IRQ */
	exception_init();/*Copy handler to 0x80000080*/
	init_IRQ();      /*Allocate IRQfinder to Exception 0*/
	/* frankliao added 20101215 */
	flash_init(flash_base);
#ifndef TCSUPPORT_MTD_PARTITIONS_CMDLINE
	#ifdef BOOTROM_LARGE_SIZE
	flash_firmware_base_init();
	#endif
#else
	ecnt_parse_cmdline_partitions();
#endif

	for (i=0; i<6; i++) {
		flash_mac_addr[i] = READ_FLASH_BYTE(FLASH_ETH_MAC + i);
	}

#ifdef TR068_LED
#if defined(TCSUPPORT_CT)	|| defined(TCSUPPORT_C1_MS)
	power_gpioInit(POWER_LED_GREEN);/*Power led turn green first*/
#endif
#endif

	time_init();

	if(isMT7505){ 

		/*MT7505 input crystall is 40Mhz*/
		if(VPint(CR_AHB_HWCONF)&(1<<1)){
		VPint(0xbfaf2400) = (VPint(0xbfaf2400) | 0x100); /*force mode enable xtal type*/
			#ifdef MT7550_GPIO
			VPint(RG_DSL_HRDDS_PCW_NCP0_SCU) = 0x54c985f0; 
			#endif
	}
	
#ifdef MT7550_GPIO
		/* enable DMT FNPLL */
		VPint(RG_DSLPLL_EN_SCU) = (VPint(RG_DSLPLL_EN_SCU) | 0x01);
		/* change DMT state from reset to normal */
		VPint(RG_DMT_CTRL) = (VPint(RG_DMT_CTRL) & 0xfffffffe);
#endif
	
#if !defined(TCSUPPORT_AUTOBENCH)
		/*MT7505 lan led mode enable, set share pin */
		VPint(0xbfa2007c) = (VPint(0xbfa2007c) | 0x3e00);
#else	
		prom_printf("MT7505_SLT\n");
#endif
}
#if !defined(TCSUPPORT_AUTOBENCH)
#ifdef TC3262
#ifdef RT63365
	if (isRT63365)
		pcie_init();
#endif
#endif
#endif

	eth_latsch_up();

	prom_printf("\n");
#if defined(TCSUPPORT_ETH4_WAN_PORT) || defined(TCSUPPORT_MULTI_UPGRADE) || defined(TCSUPPORT_MULTI_BOOT)
	if( (flash_mac_addr[0] == 0) && (flash_mac_addr[1] == 0) && (flash_mac_addr[2] == 0) &&
	    (flash_mac_addr[3] == 0) && (flash_mac_addr[4] == 0) && (flash_mac_addr[5] == 0) )
		prom_printf("The MAC address in flash is null!\n");    
	else    
		memcpy(mac_addr, flash_mac_addr, 6);

	/*Decide skb start addr according to memory size. shnwind*/	
	skb_init(); 
	eth_init(mac_addr);
#endif

#if defined(TCSUPPORT_ETH4_WAN_PORT)
	//set pvid
	tcMiiStationWrite(20, 17, 0x4001);
	tcMiiStationWrite(20, 18, 0x4002);
	tcMiiStationWrite(20, 19, 0x4003);
	tcMiiStationWrite(20, 20, 0x4004);
#endif



#if defined(TCSUPPORT_MULTI_UPGRADE) || defined(TCSUPPORT_MULTI_BOOT)
/*
	init network for multi-upgrade
*/
	arp_init();
	ip_init(ip_addr);
	udp_init();
	arp_add_entry(mac_addr, ip_addr);
#endif

#if defined(MT7530_SLT)
// MT7530 SLT Test Program Start
	MT7530_SLT_Test();
#endif

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
			if(isRT65168){
			#if !defined(MT7530_SUPPORT) && !defined(MT7530_SLT)
				VPint(0xbfb00084) = 0x1; //reset DMT
				pause(20);
				VPint(0xbfb00084) = 0x0; //reset DMT
				pause(1);
				VPint(0xbf500424) = 0x6004; //write AFE reg4
			#endif
			}
#if defined(TCSUPPORT_MULTI_UPGRADE) || defined(TCSUPPORT_MULTI_BOOT)
                        /*
                              close mac before decompress
                        */

			tc_outl(CR_INTC_IMR, 0x0);
#if !defined(TCSUPPORT_CPU_MT7505)
			tc_outl(CR_TIMER_CTL, 0x0);
#endif

			tc_outl(CR_MAC_MACCR, 0x0);

#ifdef TC3262
			
			resetSwMAC3262();
#else
			resetSwMAC();
#endif
#endif		

#if defined(TCSUPPORT_CPU_MT7505)
			tc_outl(CR_TIMER_CTL, 0x0);
#endif
			boot_kernel();
			break;
		}
	}

	/* boot command mode */
	serial_inc();
#endif
	prom_printf("\n");
	/*Read the mac address from flash*/
	
#if !defined(TCSUPPORT_MULTI_UPGRADE) && !defined(TCSUPPORT_MULTI_BOOT)
	if( (flash_mac_addr[0] == 0) && (flash_mac_addr[1] == 0) && (flash_mac_addr[2] == 0) &&
	    (flash_mac_addr[3] == 0) && (flash_mac_addr[4] == 0) && (flash_mac_addr[5] == 0) )
		prom_printf("The MAC address in flash is null!\n");    
	else    
  		memcpy(mac_addr, flash_mac_addr, 6);

	/*Decide skb start addr according to memory size. shnwind*/	
	skb_init(); 
	eth_init(mac_addr);
	arp_init();
	ip_init(ip_addr);
	udp_init();
	arp_add_entry(mac_addr, ip_addr);
#endif

#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
	user_Auth();	
#endif
	cmd_proc();
}

void IP_change(uint32 ipaddr){
	ip_addr = ipaddr;
	arp_init();	
	ip_init(ip_addr);	
	arp_add_entry(mac_addr, ip_addr);	
}

#ifdef TR068_LED
/*_____________________________________________________________________________
**      function name: power_gpioInit
**      descriptions:
**       Accroding the mi.conf gpio value to init power led.
**	   
**      parameters:
**         none
**             
**      global:
**         None
**             
**      return:
**           None
**	     
**      call:
**   	     None
**      		
**      revision:
**      1. yzwang 2010/03/10
**____________________________________________________________________________
*/
void
power_gpioInit(uint8 ledColor)
{
	/*Get the Power GPIO Number*/
	/* frankliao modify 20101208 */
//	uint8 *flash_power_gpio_addr = (uint8 *)FLASH_POWERLED_GPIO;
	uint8 power_gpio_1 = 255;
	uint8 power_gpio_2 = 255;

	power_gpio_1 = READ_FLASH_BYTE(FLASH_POWERLED_GPIO + 0);
	power_gpio_2 = READ_FLASH_BYTE(FLASH_POWERLED_GPIO + 1);
	
	if( power_gpio_1 == 255 || power_gpio_2 == 255 ) {
		prom_printf("\nInvalid Power GPIO, just return and don't turn on Power LED \n");
		return;
	}
        /*configure gpio as ouput*/
	#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520)	|| defined(TCSUPPORT_CPU_MT7505)
	LED_OEN(power_gpio_1);
	LED_OEN(power_gpio_2);

	if(ledColor == POWER_LED_GREEN) {
		/*turn on green led*/
		LED_OFF(power_gpio_2);
		LED_ON(power_gpio_1);		
	}
	else if(ledColor == POWER_LED_RED) {
		/*turn on red led */
		LED_OFF(power_gpio_1);
		LED_ON(power_gpio_2);
	}
	#else
	VPint(CR_GPIO_CTRL) |= (1<<(power_gpio_1<<1));
	VPint(CR_GPIO_CTRL) |= (1<<(power_gpio_2<<1));
	
	/*set it as open drain*/
	VPint(CR_GPIO_ODRAIN) |= (1<<power_gpio_1);
	VPint(CR_GPIO_ODRAIN) |= (1<<power_gpio_2);

	if(ledColor == POWER_LED_GREEN) {
		/*turn on green led*/
		VPint(CR_GPIO_DATA) &= ~(1<<power_gpio_1);//turn on green led
		VPint(CR_GPIO_DATA) |= (1<<power_gpio_2);//turn off red led
	}
	else if(ledColor == POWER_LED_RED) {
		/*turn on red led */
		VPint(CR_GPIO_DATA) |= (1<<power_gpio_1);//turn off green led
		VPint(CR_GPIO_DATA) &= ~(1<<power_gpio_2);//turn on red led
	}
	#endif

}/*end power_gpioInit*/
#endif

/*<< BosaZhong@30Apr2014, add, led control */
#define LED_PIN_PON			15
#define LED_PIN_ALARM		17
#define LED_PIN_LAN2		43
#define LED_PIN_LAN1		42
#define LED_PIN_FXS1		19

#ifndef CR_AHB_BASE
#define CR_AHB_BASE       	0xBFB00000
#endif 
#ifndef CR_GPIO_SHR
#define CR_GPIO_SHR       (CR_AHB_BASE + 0x860)
#endif

#ifndef LED_OEN(x)
#define LED_OEN(x) 	do { \
						if(x > 31){	\
							if(x > 47){						\
								regWrite32(CR_GPIO_CTRL3,regRead32(CR_GPIO_CTRL3)|(1<<((x-48)*2)));	\
							}else{							\
								regWrite32(CR_GPIO_CTRL2,regRead32(CR_GPIO_CTRL2)|(1<<((x-32)*2)));	\
							}								\
															\
							regWrite32(CR_GPIO_ODRAIN1,regRead32(CR_GPIO_ODRAIN1)|(1<<(x-32)));	\
						}else{								\
							if(x > 15){						\
								regWrite32(CR_GPIO_CTRL1,regRead32(CR_GPIO_CTRL1)|(1<<((x-16)*2)));	\
							}else{							\
								regWrite32(CR_GPIO_CTRL,regRead32(CR_GPIO_CTRL)|(1<<(x*2)));	\
							}								\
							regWrite32(CR_GPIO_ODRAIN,regRead32(CR_GPIO_ODRAIN)|(1<<(x)));	\
						} \
					}while(0)
#endif /* LED_OEN */

#ifndef LED_ON(x)
#define LED_ON(x)		do { \
						if(x > 31) \
						{ \
							regWrite32(CR_GPIO_DATA1, regRead32(CR_GPIO_DATA1) & ~(1 << (x-32))); \
						}else{ \
							regWrite32(CR_GPIO_DATA, regRead32(CR_GPIO_DATA) & ~(1 << x)); \
						} \
					}while(0)
#endif  /* LED_ON */

#ifndef LED_OFF(x)
#define LED_OFF(x)	do { \
						if(x > 31) \
						{ \
							regWrite32(CR_GPIO_DATA1, regRead32(CR_GPIO_DATA1) | (1<<(x-32))); \
						}else{ \
							regWrite32(CR_GPIO_DATA, regRead32(CR_GPIO_DATA) | (1<<x)); \
						} \
					}while(0)
#endif /* LED_OFF */


unsigned int led_init(void)
{
	unsigned int word;
	unsigned int ret;
	
	word = regRead32(CR_GPIO_SHR);
	ret  = word;
	word |= (1<<14);

	// enable software control ether led.
	word &= ~(0x1F<<9);

	regWrite32(CR_GPIO_SHR,word);

	LED_OEN(LED_PIN_PON);    /* PON LED */
	LED_OEN(LED_PIN_ALARM);  /* Alarm LED */
	LED_OEN(LED_PIN_FXS1);   /* FXS1 */
	LED_OEN(LED_PIN_LAN1);   /* LAN1 */
	LED_OEN(LED_PIN_LAN2);   /* LAN2 */

	return ret;
}

void led_restore(unsigned int restoreReg)
{
	regWrite32(CR_GPIO_SHR, restoreReg);
}


void led_all_mode(int ledOn)
{
	if (ledOn)
	{
		/* LED ON */
		/* power */

		/* PON */
		LED_OFF(LED_PIN_PON);

		/* ALARM */
		LED_OFF(LED_PIN_ALARM);

		/* LAN2 */
		LED_ON(LED_PIN_LAN2);

		/* LAN1 */
		LED_ON(LED_PIN_LAN1);

		/* FXS1 */
		LED_OFF(LED_PIN_FXS1);
	}
	else
	{
		/* LED OFF */
		/* power */

		/* PON */
		LED_ON(LED_PIN_PON);

		/* ALARM */
		LED_ON(LED_PIN_ALARM);

		/* LAN2 */
		LED_OFF(LED_PIN_LAN2);

		/* LAN1 */
		LED_OFF(LED_PIN_LAN1);

		/* FXS1 */
		LED_ON(LED_PIN_FXS1);
	}

	return ;
}
/*>> endof BosaZhong@30Apr2014, add, led control */

