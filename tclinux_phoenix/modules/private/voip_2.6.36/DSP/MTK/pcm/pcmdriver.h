/************************************************************************
 *
 *	Copyright (C) 2006 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/
#ifndef PCM_INTERNAL_H
#define PCM_INTERNAL_H
#include <asm/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/dma-mapping.h>
#include <linux/timer.h>
#include <linux/wait.h>
#include <asm/addrspace.h>
#include <asm/tc3162/cmdparse.h>
#include <asm/tc3162/tc3162.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31)
#include <linux/slab.h>
#endif
#include  "pcm_ext_conf.h"
/*variable type definition*/
#ifndef UINT32
#define UINT32
typedef unsigned long int uint32; 		 //32-bit unsigned integer
#endif

#ifndef UINT16
#define UINT16
typedef unsigned short uint16;         //  16-bit unsigned integer
#endif

#ifndef UINT8
#define UINT8
typedef unsigned char uint8;            // 8-bit unsigned integer
#endif

#ifndef UINT4
#define UINT4
typedef unsigned long uint4;            // 8-bit unsigned integer
#endif

#ifndef VPint
#define VPint			*(volatile unsigned long int *)
#endif

#ifndef timerVlrGet
#define timerVirGet
#define CR_TIMER_BASE  		0xBFBF0100
#define CR_TIMER0_VLR    	(CR_TIMER_BASE + 0x08)
#define timerVlrGet(timer_no,val) (val)=*(volatile uint32 *)(CR_TIMER0_VLR+timer_no*0x08)
#endif

#define regRead(reg) 			regRead32(reg)
#define regWrite(reg,regValue) 	regWrite32(reg,regValue)
/*************************************************************************/
/*     			CHIP 				Authentication		                 */
/*************************************************************************/
#ifndef isTC3182
#define isTC3182   (((regRead32(0xbfb00064)&0xffff0000))==0x00010000)
#endif

#ifndef isRT65168
#define isRT65168   (((regRead32(0xbfb00064)&0xffff0000))==0x00020000)
#endif

#ifndef isRT63365
#define isRT63365   (((regRead32(0xbfb00064)&0xffff0000))==0x00040000)
#endif

#ifndef isRT63368
#define isRT63368   (isRT63365 ? ((((regRead32(0xbfb0008c)>>8) & 0x3) == 0x3) ? 1 : 0): 0)
#endif

#ifndef isMT751020
#define isMT751020  (((regRead32(0xbfb00064)&0xffff0000))==0x00050000)
#endif

#ifndef isEN7526C
#define isEN7526C  (((regRead32(0xbfb00064)&0xffff0000))==0x00080000)
#endif

#ifndef isEN751221
#define isEN751221  ((((regRead32(0xbfb00064)&0xffff0000))==0x00070000) || isEN7526C)
#endif

/*milisecond delay*/
#define pause(x)		mdelay(x)

/*Register addr base*/

#ifndef TCSUPPORT_PCM2
#define OPERATIONAL_BASE 				0xbfbd0000
#define PCM_ID 0
#define pcmReset 0x800
#else
#define OPERATIONAL_BASE 0xbfbd2000
#define PCM_ID 1
#define pcmReset 0x10
#endif

#define IOMUX_CONTROL1    0xbfa20104
#define PCM_CLK_OUTPUT    0xbfa200d8
#define PCM_CLK_SOURCE_SELECT    0xbfa20148

#define PCM_ZSI_CLK_SOURCE    0xc
#define PCM_PIN_MODE      0x1000
#define GPIO_ZSI_ISI_1st      0x2000
#define GPIO_ZSI_ISI_2nd      0x4000
#define GPIO_SPI_SLIC_1st         0x7400
#define GPIO_SPI_SLIC_2nd         0x100
#define GPIO_PCM_RESET     0x400



#define AHB_BUS_ADDR 				0xbfb000b8
#define IMR 0x28
#define TX_POLLING_OFF 0x2c
#define RX_POLLING_OFF 0x30
#define TX_RX_DMA_CTRL 0x40
/*Register number*/
enum pcm_reg_num{
	PCM_INTFACE_Ctrl=0,//value=0x0100_040a
	PCM_TX_TIME_SLOT_CFG0, // value=0x0008_0000
	PCM_TX_TIME_SLOT_CFG1, // value=0x0018_0010
	PCM_TX_TIME_SLOT_CFG2, // value=0x0028_0020
	PCM_TX_TIME_SLOT_CFG3,// value=0x0038_0030
	PCM_RX_TIME_SLOT_CFG0, // value=0x0008_0000
	PCM_RX_TIME_SLOT_CFG1, // value=0x0018_0010
	PCM_RX_TIME_SLOT_CFG2, // value=0x0028_0020
	PCM_RX_TIME_SLOT_CFG3, // value=0x0038_0030
	PCM_ISR,  			// value=0x0000_0000
	PCM_IMR,  				// value=0x0000_0000
	PCM_TX_POLLING_DEMAND,	// value=0x0000_0000
	PCM_RX_POLLING_DEMAND,  		// value=0x0000_0000
	PCM_TX_DESC_RING_BASE,  			 // value=0x0000_0000
	PCM_RX_DESC_RING_BASE,  			 // value=0x0000_0000
	PCM_TX_RX_DESC_RING_SIZE_OFFSET,  	// value=0x0000_00c0
	PCM_TX_RX_DMA_Ctrl, 	 			 // value=0x0f00_0000
};



/*Variable Definition*/
#ifdef CONFIG_MIPS_TC3262
#ifndef TCSUPPORT_PCM2
#define PCM_INT	12
#else
#define PCM_INT	34
#endif
#else
#define PCM_INT 28
#endif

#define PCM_1 0
#define PCM_2 1

#define BYTEALIMENT	32
#define MAX_REG_NUM 17
#define TX 0
#define RX 1
#define DISABLE 0
#define ENABLE 1
#define MAX_TX_DESC_NUM 15
#define MAX_RX_DESC_NUM 15
#define MAX_BUF_NUM 8
#define MAX_CH_NUM 8
#define MAX_CH_NUM_REG ((1<<MAX_CH_NUM)-1)


#define DEFAULT_BIT_ORDER 0
#define DEFAULT_SAMPLE_RATE 0
#define MAX_TIME_POOL_NUM 5
#define MAX_SAMPLE_POOL_NUM 4

#define MAX_SAMPLE_COUNT 1020
#define MAX_BIT_WIDTH 16
#define MAX_BUF_SIZE ((MAX_BIT_WIDTH>>3)*MAX_SAMPLE_COUNT)
#define MAX_DATA_PAT 4
#define MAX_FRAME_COUNT 31
#define MAX_FRAME_COUNT_REG (MAX_FRAME_COUNT<<18)
#define RAND_MAX   0x000000ff
/*the interrupt mask*/

#define SSR3 0xBFB00094
#define EXTERN_SLIC_INIT 0x4000
#define SFC_INT 0x2000
#define ISI_INT 0x1000
#define ZSI_INT 0x800

#define HUNT_ERR_AFTER_FINISH_INT 0x400
#define HUNT_OVERTIME_INT 0x200
#define AHB_BUS_ERR_INT 0x100
#define RX_BUF_OVER_RUN_INT 0x80
#define TX_BUF_UNDER_RUN_INT 0x40

#define END_OF_RX_DESC_INT 0x20
#define END_OF_TX_DESC_INT 0x10
#define RX_DESC_UPDATE_COMPLETED_INT 0x8
#define TX_DESC_UPDATE_COMPLETED_INT 0x4
/*#define RX_FRAME_BOUNDARY_INT  0x2*/
#define TX_RX_FRAME_BOUNDARY_INT  0x1
/*Return Error definition*/
#define TX_DESC_OVER_TEN 4 /*20110217_serena_modify for pcm loopbakc test*/

#define DESC_FULL 1
#define CHANNEL_BUF_LACK 2
#define SUCCESS 1
#define FAIL 0
/*Loop back Mode*/
#define NO_LOOP_BACK 0
#define BACK_TO_BACK 1
#define ONE_BY_ONE 2
/*bit width mix mode*/
#define MIX_EIGHT_SIXTEEN 0
#define MIX_SIXTEEN_EIGHT 1
#define MIX_ALL_EIGHT 2
#define MIX_ALL_SIXTEEN 3
/*bit and byte ordering*/
#define LITTLE_ENDIAN_LSB 0
#define LITTLE_ENDIAN_MSB 1
#define BIG_ENDIAN_LSB 2
#define BIG_ENDIAN_MSB 3
/*header pattern*/
#define HEADER_PATTERN 0x12345678
#define HEADER_PATTERN_FIRST_BYTE 0x12
#define HEADER_PATTERN_SECOND_BYTE 0x34
#define HEADER_PATTERN_THIRD_BYTE 0x56
#define HEADER_PATTERN_FOUTH_BYTE 0x78

#define MAX_LOOP_BACK_COUNTER 50000
/*
 * Next Descriptor's offset.
 * Its value depends on MAX_BUF_NUM used in DESC structure
 */
#define MAX_DESC_OFF (MAX_BUF_NUM+1)
#define READ_ONLY 1
#define WRITE_ONLY (1<<1)
#define K1_TO_PHYSICAL(x) 				((uint32)(x) & 0x1fffffff)  /* kseg1 to physical */
#define CACHE_TO_NONCACHE(addr)			((uint32)(addr) | 0xa0000000)
#define NONCACHE_TO_CACHE(addr)			((uint32)(addr) & 0xdfffffff)
#define PHYSICAL_TO_K1(x) 				((uint32)(x) | 0xa0000000)  /* physical to kseg1 */

#define DBG_TIME     (1 << 19)
#define DBG_MSG      (1 << 20)
#define DBG_DES      (1 << 21)
#define DBG_REG      (1 << 22)
#define DBG_BUF      (1 << 23)
#define DBG_HEAD      (1 << 24)
#define DBG_FUNC     (1 << 25)
#define DBG_WARN     (1 << 26)
#define DBG_ERR      (1 << 27)
#define DBG_INT		(1 << 28)
#define DBG_DESC_CHECK      (1 << 29)

#define MEOMORY_LACK 0
#define MIN_FRAME_COUNT 0
/*Err msg*/
#define BUFFER_MORE_THAN_CHANNEL 2
#define TX_SAMPLESIZE_RANGE_EXCCEDED 3
#define DESC_READY 1
/*config err */
#define CONFIG_FRAMECOUNT_ERR 1
#define CONFIG_BYTEORDER_ERR 2
#define CONFIG_BITORDER_ERR 3
#define CONFIG_FSEDGE_ERR 4
#define CONFIG_FSLEN_ERR 5
#define CONFIG_SAMPLECLOCK_ERR 6
#define CONFIG_BITCLOCK_ERR 7
#define CONFIG_PCMMODE_ERR 8
#define CONFIG_CHNUM_ERR 9
#define CONFIG_RX_SAMPLESIZE_ERR 10
#define CONFIG_CHANNEL_ERR 11

#define NO_DBG 0
#define CATEGORY_CONFIG 1
#define CATEGORY_TX		(1 << 1)
#define CATEGORY_RX		(1 << 2)
#define DBG_LEVEL_1			1
#define DBG_LEVEL_2			(1<<1)
#define DBG_LEVEL_3			(1<<2)
#define pcm_dbg_pline(x)		printk("%s", x)
#define pcm_dbg_plineint(x, y)		printk("%s%d", x, (int) y)
#define pcm_dbg_plineb(x, y)	printk("%s%02x", x, (uint8) y)
#define pcm_dbg_plinew(x, y)	printk("%s%04x", x, (uint16) y)
#define pcm_dbg_plinel(x, y)	printk("%s%08lx", x, (uint32) y)
#define PCMDEBUGMSG(dbgCat,dbgLevel,format) if((dbgCat&curConfigNode.debugCategory) && (dbgLevel&curConfigNode.debugLevel)) pcm_dbg_pline format
#define PCMDEBUGMSGINT(dbgCat,dbgLevel,format) if((dbgCat&curConfigNode.debugCategory) && (dbgLevel&curConfigNode.debugLevel)) pcm_dbg_plineint format
#define PCMDEBUGMSGB(dbgCat,dbgLevel,format) if((dbgCat&curConfigNode.debugCategory) && (dbgLevel&curConfigNode.debugLevel)) pcm_dbg_plineb format
#define PCMDEBUGMSGW(dbgCat,dbgLevel,format) if((dbgCat&curConfigNode.debugCategory) && (dbgLevel&curConfigNode.debugLevel)) pcm_dbg_plinew format
#define PCMDEBUGMSGL(dbgCat,dbgLevel,format) if((dbgCat&curConfigNode.debugCategory) && (dbgLevel&curConfigNode.debugLevel)) pcm_dbg_plinel format

#define  ONE_TICK_UINT (SYS_HCLK>>1)

//#define swap32(x) ()
/*
 * Data Structure
 */

/*
 * It is the header attached before the Tx buffer.
 * there would be data pattern firstly.
 * And then this pattern type.
 * finally, it would be the total data length.
 * so If there is 10 bytes in Tx buffer and data pattern is 0x0.
 * the data content would be 0x12345678,0,10,000000000
 *
 * */
typedef struct txBufHeader_s {
	uint32 headerPat;
	uint16 dataPatType;
	uint16 dataLen;
}txBufHeader_t;
typedef struct chDataInfor_s {
	uint16 chDataIdx;
	uint16 chDataLen;
	uint16 dataPatType;
	uint8 randSeed;
	uint8 patMatch;
}chDataInfor_t;
typedef struct desc_s {
	uint32 descStatus;
	uint32 bufAddr[MAX_BUF_NUM];
}desc_t;
typedef struct regAttr_s{
	char* name;
	uint8 rw;
	uint32 mask;
	uint32 addr;
	uint32 defaultValue;
}regAttr_t;

typedef union{
	struct {
		uint32 ownership : 1;
		uint32 reserved : 7;
		uint32 chValid : 8;
		uint32 reserved2 : 6;
		uint32 sampleSize : 10;
	}bits;
	uint32 value;
}descStatus_t;

typedef union{
	struct {
		uint32 reserved : 3;
		uint32 bitWidthNext : 1;
		uint32 reserved2 : 2;
		uint32 bitCounterNext : 10;
		uint32 reserved3 : 3;
		uint32 bitWidth : 1;
		uint32 reserved4 : 2;
		uint32 bitCounter : 10;
	}bits;
	uint32 value;
}slotCfgReg_t;

typedef union {
	struct {
		uint32 a :4;
		uint32 b :4;
	} bits;
	uint8 value;
} bitswap_t;

typedef struct dmaBufferHeader_s {
	dma_addr_t dmaHandle;
	uint32 bufSize;
	char reserved[24];
} dmaBufferHeader_t;

int pcmBufInit(configNode_t* configNode);
int pcmBufReInit(configNode_t* configNode);
void pcmBufRemove(void);
void *pcmTxBufAlloc(void);
void *pcmRxBufAlloc(void);
void pcmTxBufFree(uint8 *pBuf);
void pcmRxBufFree(uint8 *pBuf);
uint32 pcmBufGetValidLen(uint8 *pBuf);
uint8* dequeue(void);
int enqueue(uint8* rxBuf);
int queueSizeGet(void);

#endif
