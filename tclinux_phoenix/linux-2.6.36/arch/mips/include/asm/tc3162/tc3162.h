/*
** $Id: tc3162.h,v 1.7 2011/01/07 06:05:58 pork Exp $
*/
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
/*
** $Log: tc3162.h,v $
** Revision 1.7  2011/01/07 06:05:58  pork
** add the definition of INT!16,INT32,SINT15,SINT7
**
** Revision 1.6  2010/09/20 07:08:02  shnwind
** decrease nf_conntrack buffer size
**
** Revision 1.5  2010/09/03 16:43:07  here
** [Ehance] TC3182 GMAC Driver is support TC-Console & WAN2LAN function & update the tc3182 dmt version (3.12.8.83)
**
** Revision 1.4  2010/09/02 07:04:50  here
** [Ehance] Support TC3162U/TC3182 Auto-Bench
**
** Revision 1.3  2010/08/30 07:53:02  lino
** add power saving mode kernel module support
**
** Revision 1.2  2010/06/05 05:40:29  lino
** add tc3182 asic board support
**
** Revision 1.1.1.1  2010/04/09 09:39:21  feiyan
** New TC Linux Make Flow Trunk
**
** Revision 1.4  2010/01/14 10:56:42  shnwind
** recommit
**
** Revision 1.3  2010/01/14 08:00:10  shnwind
** add TC3182 support
**
** Revision 1.2  2010/01/10 15:27:26  here
** [Ehancement]TC3162U MAC EEE is operated at 100M-FD, SAR interface is accroding the SAR_CLK to calculate atm rate.
**
** Revision 1.1.1.1  2009/12/17 01:42:47  josephxu
** 20091217, from Hinchu ,with VoIP
**
** Revision 1.2  2006/07/06 07:24:57  lino
** update copyright year
**
** Revision 1.1.1.1  2005/11/02 05:45:38  lino
** no message
**
** Revision 1.5  2005/09/27 08:01:38  bread.hsu
** adding IMEM support for Tc3162L2
**
** Revision 1.4  2005/09/14 11:06:20  bread.hsu
** new definition for TC3162L2
**
** Revision 1.3  2005/06/17 16:26:16  jasonlin
** Remove redundant code to gain extra 100K bytes free memory.
** Add "CODE_REDUCTION" definition to switch
**
** Revision 1.2  2005/06/14 10:02:01  jasonlin
** Merge TC3162L2 source code into new main trunk
**
** Revision 1.1.1.1  2005/03/30 14:04:22  jasonlin
** Import Linos source code
**
** Revision 1.4  2004/11/15 03:43:17  lino
** rename ATM SAR max packet length register
**
** Revision 1.3  2004/09/01 13:15:47  lino
** fixed when pc shutdown, system will reboot
**
** Revision 1.2  2004/08/27 12:16:37  lino
** change SYS_HCLK to 96Mhz
**
** Revision 1.1  2004/07/02 08:03:04  lino
** tc3160 and tc3162 code merge
**
*/

#ifndef _TC3162_H_
#define _TC3162_H_

#ifdef CONFIG_MIPS_TC3262 
#include "./tc3182_int_source.h"
#else
#include "./int_source.h" /*shnwind add*/
#endif


#ifndef INT32
#define INT32
typedef signed long int int32;    		/* 32-bit signed integer        */
#endif

#ifndef SINT31
#define SINT31
typedef signed long int sint31;        	/* 32-bit signed integer        */
#endif

#ifndef UINT32
#define UINT32
typedef unsigned long int uint32; 		/* 32-bit unsigned integer      */
#endif

#ifndef SINT15
#define SINT15
typedef signed short sint15;            /* 16-bit signed integer        */
#endif

#ifndef INT16
#define INT16
typedef signed short int int16;         /* 16-bit signed integer        */
#endif

#ifndef UINT16
#define UINT16
typedef unsigned short uint16;          /* 16-bit unsigned integer      */
#endif

#ifndef SINT7
#define SINT7
typedef signed char sint7;              /* 8-bit signed integer         */
#endif


#ifndef UINT8
#define UINT8
typedef unsigned char uint8;            /* 8-bit unsigned integer       */
#endif

#ifndef VPint
#define VPint			*(volatile unsigned long int *)
#endif
#ifndef VPshort
#define VPshort			*(volatile unsigned short *)
#endif
#ifndef VPchar
#define VPchar			*(volatile unsigned char *)
#endif
#ifdef TCSUPPORT_MT7510_E1
static inline unsigned long int regRead32(uint32 reg)	\
{       						\
	uint32 tmp;					\
	tmp = VPint((reg & 0xf) + 0xbfb003a0);   	\
	__asm__ __volatile("sync");			\
	tmp = VPint(reg);				\
	__asm__ __volatile("sync");			\
        return tmp;             	           	\
}
static inline void regWrite32(uint32 reg, uint32 vlaue) \
{                                                       \
        VPint(reg) = vlaue;                             \
	__asm__ __volatile("sync");			\
}

#else
static inline uint32 regRead32(uint32 reg)		\
{						  	\
	return VPint(reg);			  	\
}		
static inline void regWrite32(uint32 reg, uint32 vlaue)	\
{                                                	\
        VPint(reg) = vlaue;                      	\
}
#endif
static inline unsigned long int regReadPhy32(uint32 reg)	\
{       						\
	uint32 tmp;					\
	tmp = VPint(reg);	  	\
	tmp = VPint(reg);				\
        return tmp;             	           	\
}

#ifdef CONFIG_CPU_TC3162
#define TC3162L2		1
#endif

#define isTC3162L2P2 ((((unsigned char)(regRead32(0xbfb0008c)>>12)&0xff)!=0)&&(((regRead32(0xbfb00064)&0xffffffff))==0x00000000)?1:0)
#define isTC3162L3P3 ((((unsigned char)(regRead32(0xbfb0008c)>>12)&0xff)==7)&&(((regRead32(0xbfb00064)&0xffffffff))==0x00000000)?1:0)
#define isTC3162L4P4 ((((unsigned char)(regRead32(0xbfb0008c)>>12)&0xff)==8)&&(((regRead32(0xbfb00064)&0xffffffff))==0x00000000)?1:0)
#define isTC3162L5P5E2 ((((unsigned char)(regRead32(0xbfb0008c)>>12)&0xff)==0xa)&&(((regRead32(0xbfb00064)&0xffffffff))==0x00000000)?1:0)
#define isTC3162L5P5E3 ((((unsigned char)(regRead32(0xbfb0008c)>>12)&0xff)==0xb)&&(((regRead32(0xbfb00064)&0xffffffff))==0x00000000)?1:0)
#define isTC3162L5P5 (isTC3162L5P5E2 || isTC3162L5P5E3)
#define isTC3162U ((((unsigned char)(regRead32(0xbfb0008c)>>12)&0xff)==0x10)&&(((regRead32(0xbfb00064)&0xffffffff))==0x00000000)?1:0)
#define isRT63260 ((((unsigned char)(regRead32(0xbfb0008c)>>12)&0xff)==0x20)&&(((regRead32(0xbfb00064)&0xffffffff))==0x00000000)?1:0)

#define isTC3169 	(((regRead32(0xbfb00064)&0xffff0000))==0x00000000)
#define isTC3182 	(((regRead32(0xbfb00064)&0xffff0000))==0x00010000)
#define isRT65168 	(((regRead32(0xbfb00064)&0xffff0000))==0x00020000)
#define isRT63165 	(((regRead32(0xbfb00064)&0xffff0000))==0x00030000)
#define isRT63365 	(((regRead32(0xbfb00064)&0xffff0000))==0x00040000)
#define isRT63368   (isRT63365 ? ((((regRead32(0xbfb0008c)>>8) & 0x3) == 0x3) ? 1 : 0): 0)
#define isRT62806   (((gswPbusRead(0x7ffc))&0xffff0000)==0x28060000)

#define isENP_MOD	(((regRead32(0xBFBF8214)&0x8)==0x8)?(((regRead32(0xBFBF8214)&0x20)==0x20)?1:0):(((regRead32(0xBFBF8214)&0x2)==0x2)?1:0))
#define isENS_MOD	(((regRead32(0xBFBF8214)&0x8)==0x8)?((regRead32(0xBFBF8214)&0x40)==0x40):((regRead32(0xBFBF8214)&0x4)==0x4))
#define isMT751020 	(((regRead32(0xbfb00064)&0xffff0000))==0x00050000)
#define isMT7505 	(((regRead32(0xbfb00064)&0xffff0000))==0x00060000)
#define isEN7526c	(((VPint(0xbfb00064)&0xffff0000))==0x00080000)
#define isEN751221 	((((VPint(0xbfb00064)&0xffff0000))==0x00070000) || isEN7526c)
#define isMT7510	(isMT751020 && ((regRead32(0xbfb000f8)&0x3)==0) && (isENP_MOD))
#define isMT7511	(isMT751020 && ((regRead32(0xbfb000f8)&0x3)==0)&& (!isENP_MOD))
#define isMT7520	(((regRead32(0xbfb000f8)&0x3)==0x2) && isMT751020 && (isENP_MOD) && (!isENS_MOD))
#define isMT7520G	(((regRead32(0xbfb000f8)&0x3)==0x3) && isMT751020 && (isENP_MOD))
#define isMT7525	(((regRead32(0xbfb000f8)&0x3)==0x2) && isMT751020 && (!isENP_MOD) && (!isENS_MOD))
#define isMT7525G	(((regRead32(0xbfb000f8)&0x3)==0x3) && isMT751020 && (!isENP_MOD))
#define isMT7520S       (((regRead32(0xbfb000f8)&0x3)==0x2) && isMT751020 && (isENS_MOD))
#define isMT7520E2E3 ((regRead32(0xbfb00064) & 0xFF) <= 0x2 && (isMT7520 || isMT7520G || isMT7525 || isMT7525G || isMT7520S))
#define isEPONFWID   (((regRead32(0xbfb00064) & 0xF) == 0x3) && ((regRead32(0xbfb5fffc) & 0xF) >= 0x1) && (isMT7520 || isMT7520G || isMT7525 || isMT7525G || isMT7520S))


#define isEN751221FPGA      ((regRead32(CR_AHB_HWCONF)&(1<<29)) ? 0 : 1) //used for 7512/7521
#define isGenernalFPGA          ((regRead32(CR_AHB_HWCONF)&(1<<31)) ? 1 : 0) //used for 63365/751020
#define isGenernalFPGA_2        (((VPint(CR_AHB_SSTR) & 0x1) == 0) ? 1 : 0) /*used for EN7526c and later version*/
#define isFPGA                 (isEN7526c ? isGenernalFPGA_2 : (isEN751221 ? isEN751221FPGA : isGenernalFPGA) )

#define EFUSE_VERIFY_DATA0 (0xBFBF8214)
#define EFUSE_PKG_MASK          (0x3F)
#define EFUSE_REMARK_BIT        (1<<6)

#define EFUSE_PKG_REMARK_SHITF 7

#define EFUSE_EN7526F   (0x0)
#define EFUSE_EN7521F   (0x10)
#define EFUSE_EN7521S   (0x20)
#define EFUSE_EN7512    (0x4)
#define EFUSE_EN7526D   (0x1)
#define EFUSE_EN7513    (0x5)
#define EFUSE_EN7526G   (0x2)
#define EFUSE_EN7521G   (0x12)
#define EFUSE_EN7513G   (0x6)
#define EFUSE_EN7586    (0xA)
#define EFUSE_EN7586    (0xA)

#define isEN7526F (isEN751221 && ( (regRead32(EFUSE_VERIFY_DATA0)&EFUSE_REMARK_BIT)? \
                        (((regRead32(EFUSE_VERIFY_DATA0)>>EFUSE_PKG_REMARK_SHITF)& EFUSE_PKG_MASK)== EFUSE_EN7526F): \
                        ((regRead32(EFUSE_VERIFY_DATA0)&EFUSE_PKG_MASK)==EFUSE_EN7526F)))
#define isEN7521F (isEN751221 && ( (regRead32(EFUSE_VERIFY_DATA0)&EFUSE_REMARK_BIT)? \
                        (((regRead32(EFUSE_VERIFY_DATA0)>>EFUSE_PKG_REMARK_SHITF)& EFUSE_PKG_MASK)== EFUSE_EN7521F): \
                        ((regRead32(EFUSE_VERIFY_DATA0)&EFUSE_PKG_MASK)==EFUSE_EN7521F)))
#define isEN7521S (isEN751221 && ( (regRead32(EFUSE_VERIFY_DATA0)&EFUSE_REMARK_BIT)? \
                        (((regRead32(EFUSE_VERIFY_DATA0)>>EFUSE_PKG_REMARK_SHITF)& EFUSE_PKG_MASK)== EFUSE_EN7521S): \
                        ((regRead32(EFUSE_VERIFY_DATA0)&EFUSE_PKG_MASK)==EFUSE_EN7521S)))
#define isEN7512 (isEN751221 && ( (regRead32(EFUSE_VERIFY_DATA0)&EFUSE_REMARK_BIT)? \
                        (((regRead32(EFUSE_VERIFY_DATA0)>>EFUSE_PKG_REMARK_SHITF)& EFUSE_PKG_MASK)== EFUSE_EN7512): \
                        ((regRead32(EFUSE_VERIFY_DATA0)&EFUSE_PKG_MASK)==EFUSE_EN7512)))
#define isEN7526D (isEN751221 && ( (regRead32(EFUSE_VERIFY_DATA0)&EFUSE_REMARK_BIT)? \
                        (((regRead32(EFUSE_VERIFY_DATA0)>>EFUSE_PKG_REMARK_SHITF)& EFUSE_PKG_MASK)== EFUSE_EN7526D): \
                        ((regRead32(EFUSE_VERIFY_DATA0)&EFUSE_PKG_MASK)==EFUSE_EN7526D)))
#define isEN7513 (isEN751221 && ( (regRead32(EFUSE_VERIFY_DATA0)&EFUSE_REMARK_BIT)? \
                        (((regRead32(EFUSE_VERIFY_DATA0)>>EFUSE_PKG_REMARK_SHITF)& EFUSE_PKG_MASK)== EFUSE_EN7513): \
                        ((regRead32(EFUSE_VERIFY_DATA0)&EFUSE_PKG_MASK)==EFUSE_EN7513)))
#define isEN7526G (isEN751221 && ( (regRead32(EFUSE_VERIFY_DATA0)&EFUSE_REMARK_BIT)? \
                        (((regRead32(EFUSE_VERIFY_DATA0)>>EFUSE_PKG_REMARK_SHITF)& EFUSE_PKG_MASK)== EFUSE_EN7526G): \
                        ((regRead32(EFUSE_VERIFY_DATA0)&EFUSE_PKG_MASK)==EFUSE_EN7526G)))
#define isEN7521G (isEN751221 && ( (regRead32(EFUSE_VERIFY_DATA0)&EFUSE_REMARK_BIT)? \
                        (((regRead32(EFUSE_VERIFY_DATA0)>>EFUSE_PKG_REMARK_SHITF)& EFUSE_PKG_MASK)== EFUSE_EN7521G): \
                        ((regRead32(EFUSE_VERIFY_DATA0)&EFUSE_PKG_MASK)==EFUSE_EN7521G)))
#define isEN7513G (isEN751221 && ( (regRead32(EFUSE_VERIFY_DATA0)&EFUSE_REMARK_BIT)? \
                        (((regRead32(EFUSE_VERIFY_DATA0)>>EFUSE_PKG_REMARK_SHITF)& EFUSE_PKG_MASK)== EFUSE_EN7513G): \
                        ((regRead32(EFUSE_VERIFY_DATA0)&EFUSE_PKG_MASK)==EFUSE_EN7513G)))
#define isEN7586 (isEN751221 && ( (regRead32(EFUSE_VERIFY_DATA0)&EFUSE_REMARK_BIT)? \
                        (((regRead32(EFUSE_VERIFY_DATA0)>>EFUSE_PKG_REMARK_SHITF)& EFUSE_PKG_MASK)== EFUSE_EN7586): \
                        ((regRead32(EFUSE_VERIFY_DATA0)&EFUSE_PKG_MASK)==EFUSE_EN7586)))


#define EFUSE_DDR3_BIT (1<<23)
#define EFUSE_DDR3_REMARK_BIT (1<<24)
#define EFUSE_IS_DDR3 ( (regRead32(EFUSE_VERIFY_DATA0)&EFUSE_REMARK_BIT)? \
                        ((regRead32(EFUSE_VERIFY_DATA0)& EFUSE_DDR3_REMARK_BIT)): \
                        ((regRead32(EFUSE_VERIFY_DATA0)& EFUSE_DDR3_BIT)))

#define REG_SAVE_INFO 0xBFB00284
#define SET_DRAM_SIZE(x) regWrite32(REG_SAVE_INFO ,((regRead32(REG_SAVE_INFO)&(~0xfff)) | (x&0xfff)))
#define GET_DRAM_SIZE   (regRead32(REG_SAVE_INFO)&0xfff)
#define SET_SYS_CLK(x)  regWrite32(REG_SAVE_INFO ,((regRead32(REG_SAVE_INFO)&~0xfff000) | ((x&0xfff)<<12)))
#define GET_SYS_CLK     ((regRead32(REG_SAVE_INFO)&0xfff000)>>12)




#ifdef TCSUPPORT_MT7510_E1
#define READ_E1(x) do{if (VPint(x) == 0)printk("Error Reg %x\n",x);}while(0)
#endif

#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520)
#define JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL
#ifdef JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL
#define MT751020_SUBMODEL_REG   (0xbfb000f8)

#define ExistExtMT7530		(isMT7520G || isMT7525G)
#define DefaultUseExtMT7530	(isMT7520G || isMT7525G)
#define LanPortIntMT7530	(isMT7510 || isMT7511 || isMT7520 || isMT7525)
#define LanPortExtMT7530	(isMT7520G || isMT7525G)
#endif
#endif

#define isMT7530	(((regRead32(0xbfb58000+0x7ffc)&0xffff0000))==0x75300000)
//#define isMT7530FPGA	(((gswPbusRead(0x7ffc))&0xffff0000)==0x75300000)
#define isMT7530ext	(((gswPbusRead(0x7ffc))&0xffff0000)==0x75300000)

#ifdef TC3162L2
#define RT63260_SYS_HCLK ((12*(((regRead32(0xbfb000b0))&0x1ff)+1)/(((regRead32(0xbfb000b0)>>9)&0x1f)+1))/5)
#define TC3162U_SYS_HCLK (3*(((regRead32(0xbfb000b0)>>16)&0x1ff)+1)/(((regRead32(0xbfb000b0)>>25)&0x1f)+1))
#define SYS_HCLK        (isRT63260 ? RT63260_SYS_HCLK : (isTC3162U ? TC3162U_SYS_HCLK : 133))
#endif

#ifdef CONFIG_MIPS_TC3262
/* RT63165 ASIC */
/* FPGA is 25Mhz, ASIC LQFP128 is 166.67Mhz, others are 200Mhz */
#define	RT63165_SYS_HCLK       	(regRead32(0xbfb0008c)&(1<<31) ? 25 : (regRead32(0xbfb0008c)&(1<<9) ? (200) : (16667/100)))
/* RT63365 ASIC */
/* FPGA is 25/32Mhz 
 * ASIC RT6856/RT63368: DDR(0): 233.33, DDR(1): 175, SDR: 140
 *      RT6855/RT63365: DDR(0): 166.67, DDR(1): 125, SDR: 140 */
#define	RT63365_SYS_HCLK       	(regRead32(0xbfb0008c)&(1<<31) ? (25) : (regRead32(0xbfb0008c)&(1<<9) ? (regRead32(0xbfb0008c)&(1<<25) ? (regRead32(0xbfb0008c)&(1<<26) ? (175) : (23333/100)) : (140)) : (regRead32(0xbfb0008c)&(1<<25) ? (regRead32(0xbfb0008c)&(1<<26) ? (125) : (16667/100)) : (140))))
#define MT751020_CPU_CLK		(((regRead32(0xbfb0008c) >> 0x9) & 0x3) == 0x0) ? (750) : (((regRead32(0xbfb0008c) >> 0x9) & 0x3) == 0x1) ? (650) : (((regRead32(0xbfb0008c) >> 0x9) & 0x3) == 0x2) ? (500) : (250)
#define	MT751020_SYS_HCLK		((regRead32(0xbfb0008c)&(1<<31)) ? (32) : ((((MT751020_CPU_CLK)* 100) / ((regRead32(0xbfb000f8) >> 0x3) & 0x7)) / 100))
#define	MT7505_SYS_HCLK		((regRead32(0xbfb0008c)&(1<<31)) ? (32) : (135))  //MT7505 CPU clock is 540
#define EN7512_SYS_HCLK         ((isFPGA) ? (32) : (GET_SYS_CLK)) //ASIC Clock need Check

#define	SYS_HCLK		(isEN751221?EN7512_SYS_HCLK:(isMT7505? MT7505_SYS_HCLK:(isMT751020  ? MT751020_SYS_HCLK : (isRT63365 ? RT63365_SYS_HCLK : (isRT63165 ? RT63165_SYS_HCLK : (isRT65168 ? (1024/10) : (isTC3182 ? (1024/10) : (3*((VPint(0xbfb00058)>>16)+1)/(((VPint(0xbfb00058)&0x1f)+1)<<1)))))))))
#endif

#define	SAR_CLK	(SYS_HCLK)/(4.0)		//more accurate if 4.0 not 4

/* define CPU timer clock, FPGA is 50Mhz, ASIC is 250Mhz */
#define	CPUTMR_CLK		(isFPGA ? (50*1000000) : (isEN751221 ? (200*1000000) : (isMT7505 ? (100*1000000) : (isMT751020 ? (800*1000000/3) :(250*1000000)))))

#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520)
#define DSPRAM_BASE		0x9dff8000
#else
#define DSPRAM_BASE		0x9c000000
#endif

#define ENABLE          1
#define DISABLE         0

#define WAN2LAN_CH_ID	(1<<31)

#define tc_inb(offset) 			(*(volatile unsigned char *)(offset))
#define tc_inw(offset) 			(*(volatile unsigned short *)(offset))
#define tc_inl(offset) 			(*(volatile unsigned long *)(offset))

#define tc_outb(offset,val)    	(*(volatile unsigned char *)(offset) = val)
#define tc_outw(offset,val)    	(*(volatile unsigned short *)(offset) = val)
#define tc_outl(offset,val)    	(*(volatile unsigned long *)(offset) = val)

#if defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
#define IS_SPIFLASH		((~(VPint(0xBFA10114))) & 0x2)
#define IS_NANDFLASH		(VPint(0xBFA10114) & 0x2)
#else
#if defined(TCSUPPORT_CPU_MT7505)
#define IS_SPIFLASH		1
#define IS_NANDFLASH	0
#else
#define IS_SPIFLASH				((regRead32(CR_AHB_SSR) & (1<<20)) || !(regRead32(CR_AHB_HWCONF) & 0x1))
#define IS_NANDFLASH			(regRead32(CR_AHB_HWCONF) & 0x1)
#endif
#endif
#define NF_CONNTRACK_BUF_SIZE 4096
/*****************************
 * RBUS CORE Module Registers *
 *****************************/
#define ARB_CFG     		0xBFA00008
#define ROUND_ROBIN_ENABLE  (1<<30)
#define ROUND_ROBIN_DISBALE  ~(1<<30)



/*****************************
 * DMC Module Registers *
 *****************************/

#define CR_DMC_BASE     	0xBFB20000
#define CR_DMC_SRT      	(0x00 | CR_DMC_BASE)
#define CR_DMC_STC      	(0x01 | CR_DMC_BASE)
#define CR_DMC_SAMT      	(0x02 | CR_DMC_BASE)
#define CR_DMC_SCR      	(0x03 | CR_DMC_BASE)

/* RT63165 specific */
/* DDR self refresh control register */
#define CR_DMC_DDR_SR    	(0x18 | CR_DMC_BASE)
/* DDR self refresh target count */
#define CR_DMC_DDR_SR_CNT  	(0x1c | CR_DMC_BASE)
#define CR_DMC_DDR_CFG0    	(0x40 | CR_DMC_BASE)
#define CR_DMC_DDR_CFG1    	(0x44 | CR_DMC_BASE)
#define CR_DMC_DDR_CFG2    	(0x48 | CR_DMC_BASE)
#define CR_DMC_DDR_CFG3    	(0x4c | CR_DMC_BASE)
#define CR_DMC_DDR_CFG4    	(0x50 | CR_DMC_BASE)
#define CR_DMC_DDR_CFG8    	(0x60 | CR_DMC_BASE)
#define CR_DMC_DDR_CFG9    	(0x64 | CR_DMC_BASE)

#define CR_DMC_CTL0      	(0x70 | CR_DMC_BASE)
#define CR_DMC_CTL1      	(0x74 | CR_DMC_BASE)
#define CR_DMC_CTL2      	(0x78 | CR_DMC_BASE)
#define CR_DMC_CTL3     	(0x7c | CR_DMC_BASE)
#define CR_DMC_CTL4     	(0x80 | CR_DMC_BASE)

#define CR_DMC_DCSR     	(0xb0 | CR_DMC_BASE)

#define CR_DMC_ISPCFGR     	(0xc0 | CR_DMC_BASE)
#define CR_DMC_DSPCFGR     	(0xc4 | CR_DMC_BASE)

/* MT7510 */
#define CR_DRAMC_CONF1		(0x04 | CR_DMC_BASE)
#define CR_DRAMC_PADCTL4    (0xe4 | CR_DMC_BASE)

/*****************************
 * GDMA Module Registers *
 *****************************/

#define CR_GDMA_BASE     	0xBFB30000
#define CR_GDMA_DCSA      	(0x00 | CR_GDMA_BASE)
#define CR_GDMA_DCDA      	(0x04 | CR_GDMA_BASE)
#define CR_GDMA_DCBT      	(0x08 | CR_GDMA_BASE)
#define CR_GDMA_DCBL      	(0x0a | CR_GDMA_BASE)
#define CR_GDMA_DCC      	(0x0c | CR_GDMA_BASE)
#define CR_GDMA_DCS      	(0x0e | CR_GDMA_BASE)
#define CR_GDMA_DCKSUM     	(0x10 | CR_GDMA_BASE)

/*****************************
 * SPI Module Registers *
 *****************************/

#define CR_SPI_BASE     	0xBFBC0000
#define CR_SPI_CTL      	(0x00 | CR_SPI_BASE)
#define CR_SPI_OPCODE      	(0x04 | CR_SPI_BASE)
#define CR_SPI_DATA      	(0x08 | CR_SPI_BASE)

/*****************************
 * Ethernet Module Registers *
 *****************************/

#define CR_MAC_BASE     	0xBFB50000
#define CR_MAC_ISR      	(0x00 | CR_MAC_BASE)// --- Interrupt Status Register ---
#define CR_MAC_IMR      	(0x04 | CR_MAC_BASE)// --- Interrupt Mask Register ---
#define CR_MAC_MADR  	   	(0x08 | CR_MAC_BASE)// --- MAC Address Register [47:32] ---
#define CR_MAC_LADR     	(0x0c | CR_MAC_BASE)// --- MAC Address Register [31:0] ---
#define CR_MAC_EEE		(0x10 | CR_MAC_BASE)
#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
// None
#else
  #define CR_MAC_MAHT0         (0x10 | CR_MAC_BASE)// --- MAC Hash Table Address Register [31:0] ---
  #define CR_MAC_MAHT1         (0x14 | CR_MAC_BASE)// --- MAC Hash Table Address Register [31:0] ---
#endif
#define CR_MAC_TXPD     	(0x18 | CR_MAC_BASE)// --- Transmit Poll Demand Register ---
#define CR_MAC_RXPD     	(0x1c | CR_MAC_BASE)// --- Receive Poll Demand Register ---
#define CR_MAC_TXR_BADR 	(0x20 | CR_MAC_BASE)// --- Transmit Ring Base Address Register ---
#define CR_MAC_RXR_BADR 	(0x24 | CR_MAC_BASE)// --- Receive Ring Base Address Register ---
#define CR_MAC_ITC      	(0x28 | CR_MAC_BASE)// --- Interrupt Timer Control Register ---
#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
  #define CR_MAC_TXR_SIZE  	   (0x2c | CR_MAC_BASE)// --- Transmit Ring Size Register ---
  #define CR_MAC_RXR_SIZE      (0x30 | CR_MAC_BASE)// --- Receive Ring Size Register ---
  #define CR_MAC_RXR_SWIDX     (0x34 | CR_MAC_BASE)// --- Receive Ring Software Index Register ---
#else
#define CR_MAC_APTC     	(0x2c | CR_MAC_BASE)// --- Automatic Polling Timer Control Register ---
#define CR_MAC_DBLAC    	(0x30 | CR_MAC_BASE)// --- DMA Burst Length and Arbitration Control Register ---
#endif
#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
  #define CR_MAC_TXDESP_SIZE   (0x38 | CR_MAC_BASE)// --- Transmit Descriptor Size Register ---
  #define CR_MAC_RXDESP_SIZE   (0x3c | CR_MAC_BASE)// --- Receive Descriptor Size Register ---
#else
  #define CR_MAC_TXDESCP_ADR   (0x38 | CR_MAC_BASE)// --- Current Transmit Descriptor Address Register ---
  #define CR_MAC_RXDESCP_ADR   (0x3c | CR_MAC_BASE)// --- Current Receive Descriptor Address Register ---
#endif
#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
  #define CR_MAC_PRIORITY_CFG  (0x50 | CR_MAC_BASE)// --- Priority Configuration Register ---
  #define CR_MAC_VLAN_CFG      (0x54 | CR_MAC_BASE)// --- VLAN Configuration Register ---
  #define CR_MAC_TOS0_CFG      (0x58 | CR_MAC_BASE)// --- TOS 0 Configuration Register ---
  #define CR_MAC_TOS1_CFG      (0x5c | CR_MAC_BASE)// --- TOS 1 Configuration Register ---
  #define CR_MAC_TOS2_CFG      (0x60 | CR_MAC_BASE)// --- TOS 2 Configuration Register ---
  #define CR_MAC_TOS3_CFG      (0x64 | CR_MAC_BASE)// --- TOS 3 Configuration Register ---
  #define CR_MAC_TCP_CFG       (0x68 | CR_MAC_BASE)// --- TCP Configuration Register ---
  #define CR_MAC_SWTAG_CFG     (0x6c | CR_MAC_BASE)// --- Software Tagging Configuration Register ---
  #define CR_MAC_PMBL_CYC_NUM  (0x70 | CR_MAC_BASE)// --- Preamble Cycle Number Register ---
  #define CR_MAC_FCTL_CYC_NUM  (0x74 | CR_MAC_BASE)// --- Flow Control Cycle Number Register ---
  #define CR_MAC_JAM_CYC_NUM   (0x78 | CR_MAC_BASE)// --- JAM Cycle Number Register ---
  #define CR_MAC_DEFER_VAL     (0x7c | CR_MAC_BASE)// --- Defer Value Register ---
  #define CR_MAC_RANDOM_POLY   (0x80 | CR_MAC_BASE)// --- Random Polynomial Register ---
#else
// None
#endif
#define CR_MAC_MACCR    	(0x88 | CR_MAC_BASE)// --- MAC Control Register ---
#define CR_MAC_MACSR    	(0x8c | CR_MAC_BASE)// --- MAC Status Register ---
#define CR_MAC_PHYCR    	(0x90 | CR_MAC_BASE)// --- PHY Control Register ---
#define CR_MAC_PHYWDATA 	(0x94 | CR_MAC_BASE)// --- PHY Write Data Register ---
#define CR_MAC_FCR      	(0x98 | CR_MAC_BASE)// --- Flow Control Register ---
#define CR_MAC_BPR      	(0x9c | CR_MAC_BASE)// --- Back Pressure Register ---
#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
#define CR_MAC_DESP_IDX        (0xc4 | CR_MAC_BASE)// --- Current Tx/Rx Descriptor Index ---
#endif
#define CR_MAC_WOLCR    	(0xa0 | CR_MAC_BASE)// --- Wake-On-LAN Control Register ---
#define CR_MAC_WOLSR    	(0xa4 | CR_MAC_BASE)// --- Wake-On-LAN Status Register ---
#define CR_MAC_WFCRC    	(0xa8 | CR_MAC_BASE)// --- Wake-up Frame CRC Register ---
#define CR_MAC_WFBM1    	(0xb0 | CR_MAC_BASE)// --- Wake-up Frame Byte Mask 1st Double Word Register ---
#define CR_MAC_WFBM2    	(0xb4 | CR_MAC_BASE)// --- Wake-up Frame Byte Mask 2nd Double Word Register ---
#define CR_MAC_WFBM3    	(0xb8 | CR_MAC_BASE)// --- Wake-up Frame Byte Mask 3rd Double Word Register ---
#define CR_MAC_WFBM4    	(0xbc | CR_MAC_BASE)// --- Wake-up Frame Byte Mask 4th Double Word Register ---
#define CR_MAC_DMA_FSM  	(0xc8 | CR_MAC_BASE)// --- DMA State Machine
#define CR_MAC_TM       	(0xcc | CR_MAC_BASE)// --- Test Mode Register ---
#define CR_MAC_XMPG_CNT 	(0xdc | CR_MAC_BASE)// --- XM and PG Counter Register ---
#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
#define CR_MAC_RUNT_TLCC_CNT   (0xe0 | CR_MAC_BASE)// --- Receive Runt and Transmit Late Collision Packet Counter Register ---
#define CR_MAC_RCRC_RLONG_CNT  (0xe4 | CR_MAC_BASE)// --- Receive CRC Error and Long Packet Counter Register ---
#define CR_MAC_RLOSS_RCOL_CNT  (0xe8 | CR_MAC_BASE)// --- Receive Packet Loss and Receive Collision Counter Register ---
#else
#define CR_MAC_RUNT_LCOL_CNT 	(0xe0 | CR_MAC_BASE)// --- Runt and Late Collision Packet Counter Register ---
#define CR_MAC_CRC_LONG_CNT   	(0xe4 | CR_MAC_BASE)// --- CRC and Long Packet Counter Register ---
#define CR_MAC_LOSS_COL_CNT   	(0xe8 | CR_MAC_BASE)// --- Receive Packet Loss and Receive Collision Counter Register ---
#endif
#define CR_MAC_BROADCAST_CNT  	(0xec | CR_MAC_BASE)// --- Receive Broadcast Counter Register ---
#define CR_MAC_MULTICAST_CNT  	(0xf0 | CR_MAC_BASE)// --- Receive Multicast Counter Register ---
#define CR_MAC_RX_CNT   	(0xf4 | CR_MAC_BASE)// --- Receive Good Packet Counter Register ---
#define CR_MAC_TX_CNT   	(0xf8 | CR_MAC_BASE)// --- Transmit Good Packet Counter Register ---

/*************************
 * UART Module Registers *
 *************************/
#ifdef __BIG_ENDIAN
#define CR_UART_OFFSET		(0x03)
#else
#define CR_UART_OFFSET		(0x0)
#endif

#define	CR_UART_BASE    	0xBFBF0000
#define	CR_UART_RBR     	(0x00+CR_UART_BASE+CR_UART_OFFSET)
#define	CR_UART_THR     	(0x00+CR_UART_BASE+CR_UART_OFFSET)
#define	CR_UART_IER     	(0x04+CR_UART_BASE+CR_UART_OFFSET)
#define	CR_UART_IIR     	(0x08+CR_UART_BASE+CR_UART_OFFSET)
#define	CR_UART_FCR     	(0x08+CR_UART_BASE+CR_UART_OFFSET)
#define	CR_UART_LCR     	(0x0c+CR_UART_BASE+CR_UART_OFFSET)
#define	CR_UART_MCR     	(0x10+CR_UART_BASE+CR_UART_OFFSET)
#define	CR_UART_LSR     	(0x14+CR_UART_BASE+CR_UART_OFFSET)
#define	CR_UART_MSR     	(0x18+CR_UART_BASE+CR_UART_OFFSET)
#define	CR_UART_SCR     	(0x1c+CR_UART_BASE+CR_UART_OFFSET)
#define	CR_UART_BRDL    	(0x00+CR_UART_BASE+CR_UART_OFFSET)
#define	CR_UART_BRDH    	(0x04+CR_UART_BASE+CR_UART_OFFSET)
#define	CR_UART_WORDA		(0x20+CR_UART_BASE+0x00)
#define	CR_UART_HWORDA		(0x28+CR_UART_BASE+0x00)
#define	CR_UART_MISCC		(0x24+CR_UART_BASE+CR_UART_OFFSET)
#define	CR_UART_XYD     	(0x2c+CR_UART_BASE)

#define	UART_BRD_ACCESS		0x80
#define	UART_XYD_Y          65000
#define	UART_UCLK_115200    0
#define	UART_UCLK_57600     1
#define	UART_UCLK_38400     2
#define	UART_UCLK_28800		3
#define	UART_UCLK_19200		4
#define	UART_UCLK_14400		5
#define	UART_UCLK_9600		6
#define	UART_UCLK_4800		7
#define	UART_UCLK_2400		8
#define	UART_UCLK_1200		9
#define	UART_UCLK_600		10
#define	UART_UCLK_300		11
#define	UART_UCLK_110		12
#define	UART_BRDL			0x03
#define	UART_BRDH			0x00
#define	UART_LCR			0x03
#define	UART_FCR			0x0f
#define	UART_WATERMARK		(0x0<<6)
#define	UART_MCR			0x0
#define	UART_MISCC			0x0
//#define UART_IER			0x07
//#define	UART_IER			0x05
#define	UART_IER			0x01

#define	IER_RECEIVED_DATA_INTERRUPT_ENABLE	0x01
#define	IER_THRE_INTERRUPT_ENABLE			0x02
#define	IER_LINE_STATUS_INTERRUPT_ENABLE	0x04

#define	IIR_INDICATOR						VPchar(CR_UART_IIR)
#define	IIR_RECEIVED_LINE_STATUS			0x06
#define	IIR_RECEIVED_DATA_AVAILABLE			0x04
#define IIR_RECEIVER_IDLE_TRIGGER			0x0C
#define	IIR_TRANSMITTED_REGISTER_EMPTY		0x02
#define	LSR_INDICATOR						VPchar(CR_UART_LSR)
#define	LSR_RECEIVED_DATA_READY				0x01
#define	LSR_OVERRUN							0x02
#define	LSR_PARITY_ERROR					0x04
#define	LSR_FRAME_ERROR						0x08
#define	LSR_BREAK							0x10
#define	LSR_THRE							0x20
#define	LSR_THE								0x40
#define	LSR_RFIFO_FLAG						0x80

#define uartTxIntOn()		VPchar(CR_UART_IER) |= IER_THRE_INTERRUPT_ENABLE
#define uartTxIntOff()		VPchar(CR_UART_IER) &= ~IER_THRE_INTERRUPT_ENABLE
#define uartRxIntOn()		VPchar(CR_UART_IER) |= IER_RECEIVED_DATA_INTERRUPT_ENABLE
#define	uartRxIntOff()		VPchar(CR_UART_IER) &= ~IER_RECEIVED_DATA_INTERRUPT_ENABLE

/*************************
 * UART2 Module Registers *
 *************************/
#ifdef TCSUPPORT_UART2
#define	CR_UART2_BASE    	0xBFBF0300
#define	CR_UART2_RBR     	(0x00+CR_UART2_BASE+CR_UART_OFFSET)
#define	CR_UART2_THR     	(0x00+CR_UART2_BASE+CR_UART_OFFSET)
#define	CR_UART2_IER     	(0x04+CR_UART2_BASE+CR_UART_OFFSET)
#define	CR_UART2_IIR     	(0x08+CR_UART2_BASE+CR_UART_OFFSET)
#define	CR_UART2_FCR     	(0x08+CR_UART2_BASE+CR_UART_OFFSET)
#define	CR_UART2_LCR     	(0x0c+CR_UART2_BASE+CR_UART_OFFSET)
#define	CR_UART2_MCR     	(0x10+CR_UART2_BASE+CR_UART_OFFSET)
#define	CR_UART2_LSR     	(0x14+CR_UART2_BASE+CR_UART_OFFSET)
#define	CR_UART2_MSR     	(0x18+CR_UART2_BASE+CR_UART_OFFSET)
#define	CR_UART2_SCR     	(0x1c+CR_UART2_BASE+CR_UART_OFFSET)
#define	CR_UART2_BRDL    	(0x00+CR_UART2_BASE+CR_UART_OFFSET)
#define	CR_UART2_BRDH    	(0x04+CR_UART2_BASE+CR_UART_OFFSET)
#define	CR_UART2_WORDA		(0x20+CR_UART2_BASE+0x00)
#define	CR_UART2_HWORDA	    (0x28+CR_UART2_BASE+0x00)
#define	CR_UART2_MISCC		(0x24+CR_UART2_BASE+CR_UART_OFFSET)
#define	CR_UART2_XYD     	(0x2c+CR_UART2_BASE)

#define	IIR_INDICATOR2		VPchar(CR_UART2_IIR)
#define	LSR_INDICATOR2		VPchar(CR_UART2_LSR)
#endif

/*************************
 * HSUART Module Registers *
 *************************/
#define	CR_HSUART_BASE    	0xBFBF0300
#define	CR_HSUART_RBR     	(0x00+CR_HSUART_BASE+CR_UART_OFFSET)
#define	CR_HSUART_THR     	(0x00+CR_HSUART_BASE+CR_UART_OFFSET)
#define	CR_HSUART_IER     	(0x04+CR_HSUART_BASE+CR_UART_OFFSET)
#define	CR_HSUART_IIR     	(0x08+CR_HSUART_BASE+CR_UART_OFFSET)
#define	CR_HSUART_FCR     	(0x08+CR_HSUART_BASE+CR_UART_OFFSET)
#define	CR_HSUART_LCR     	(0x0c+CR_HSUART_BASE+CR_UART_OFFSET)
#define	CR_HSUART_MCR     	(0x10+CR_HSUART_BASE+CR_UART_OFFSET)
#define	CR_HSUART_LSR     	(0x14+CR_HSUART_BASE+CR_UART_OFFSET)
#define	CR_HSUART_MSR     	(0x18+CR_HSUART_BASE+CR_UART_OFFSET)
#define	CR_HSUART_SCR     	(0x1c+CR_HSUART_BASE+CR_UART_OFFSET)
#define	CR_HSUART_BRDL    	(0x00+CR_HSUART_BASE+CR_UART_OFFSET)
#define	CR_HSUART_BRDH    	(0x04+CR_HSUART_BASE+CR_UART_OFFSET)
#define	CR_HSUART_WORDA		(0x20+CR_HSUART_BASE+0x00)
#define	CR_HSUART_HWORDA	(0x28+CR_HSUART_BASE+0x00)
#define	CR_HSUART_MISCC		(0x24+CR_HSUART_BASE+CR_UART_OFFSET)
#define	CR_HSUART_XYD     	(0x2c+CR_HSUART_BASE)

/**********************************
 * Interrupt Controller Registers *
 **********************************/
#define CR_INTC_BASE    0xBFB40000
			// --- Interrupt Type Register ---
#define CR_INTC_ITR     (CR_INTC_BASE+0x0000)
			// --- Interrupt Mask Register ---
#define CR_INTC_IMR     (CR_INTC_BASE+0x0004)
			// --- Interrupt Pending Register ---
#define CR_INTC_IPR     (CR_INTC_BASE+0x0008)
			// --- Interrupt Set Register ---
#define CR_INTC_ISR    	(CR_INTC_BASE+0x000c)
			// --- Interrupt Priority Register 0 ---
#define CR_INTC_IPR0    (CR_INTC_BASE+0x0010)
			// --- Interrupt Priority Register 1 ---
#define CR_INTC_IPR1    (CR_INTC_BASE+0x0014)
			// --- Interrupt Priority Register 2 ---
#define CR_INTC_IPR2    (CR_INTC_BASE+0x0018)
			// --- Interrupt Priority Register 3 ---
#define CR_INTC_IPR3    (CR_INTC_BASE+0x001c)
			// --- Interrupt Priority Register 4 ---
#define CR_INTC_IPR4    (CR_INTC_BASE+0x0020)
			// --- Interrupt Priority Register 5 ---
#define CR_INTC_IPR5    (CR_INTC_BASE+0x0024)
			// --- Interrupt Priority Register 6 ---
#define CR_INTC_IPR6    (CR_INTC_BASE+0x0028)
			// --- Interrupt Priority Register 7 ---
#define CR_INTC_IPR7    (CR_INTC_BASE+0x002c)
			// --- Interrupt Vector egister ---
#ifdef CONFIG_MIPS_TC3262
			// --- Interrupt VPE and SRS Register 0 ---
#define CR_INTC_IVSR0   (CR_INTC_BASE+0x0030)
			// --- Interrupt VPE and SRS Register 1 ---
#define CR_INTC_IVSR1   (CR_INTC_BASE+0x0034)
			// --- Interrupt VPE and SRS Register 2 ---
#define CR_INTC_IVSR2   (CR_INTC_BASE+0x0038)
			// --- Interrupt VPE and SRS Register 3 ---
#define CR_INTC_IVSR3   (CR_INTC_BASE+0x003c)
			// --- Interrupt VPE and SRS Register 4 ---
#define CR_INTC_IVSR4   (CR_INTC_BASE+0x0040)
			// --- Interrupt VPE and SRS Register 5 ---
#define CR_INTC_IVSR5   (CR_INTC_BASE+0x0044)
			// --- Interrupt VPE and SRS Register 6 ---
#define CR_INTC_IVSR6   (CR_INTC_BASE+0x0048)
			// --- Interrupt VPE and SRS Register 7 ---
#define CR_INTC_IVSR7   (CR_INTC_BASE+0x004c)
			// --- Interrupt Vector egister ---
#define CR_INTC_IVR     (CR_INTC_BASE+0x0050)

/* RT63165 */
			// --- Interrupt Mask Register ---
#define CR_INTC_IMR_1   (CR_INTC_BASE+0x0050)
			// --- Interrupt Pending Register ---
#define CR_INTC_IPR_1   (CR_INTC_BASE+0x0054)
			// --- Interrupt Priority Register 8 ---
#define CR_INTC_IPSR8	(CR_INTC_BASE+0x0058)
			// --- Interrupt Priority Register 9 ---
#define CR_INTC_IPSR9	(CR_INTC_BASE+0x005c)
			// --- Interrupt VPE and SRS Register 8 ---
#define CR_INTC_IVSR8   (CR_INTC_BASE+0x0060)
			// --- Interrupt VPE and SRS Register 9 ---
#define CR_INTC_IVSR9   (CR_INTC_BASE+0x0064)

/*MT7510*/
#define CR_INTC_NMI0IMR0   (CR_INTC_BASE+0x0080)
#define CR_INTC_NMI1IMR0   (CR_INTC_BASE+0x0088)




#else
			// --- Interrupt Vector egister ---
#define CR_INTC_IVR     (CR_INTC_BASE+0x0030)
#endif

#if 0
/*move this to the int_source.h because the definition of unsigned long int...etc has been defined in type.h.
  The ralink driver uses type.h and needs this interrupt_source structure so move this interrupt_source structure
	to int_source.h. shnwind */
enum
interrupt_source
	{
#ifdef CONFIG_MIPS_TC3262
		DUMMY_INT,
#endif
		UART_INT,		//0 	IPL10
		RTC_ALARM_INT,	//1 	IPL29
#ifdef CONFIG_MIPS_TC3262
		SI_SWINT1_INT0,	//2
		SI_SWINT1_INT1,	//3
#else
		RTC_TICK_INT,	//2 	IPL31
		RESERVED0,		//3 	IPL30
#endif
		TIMER0_INT, 	//4 	IPL1
		TIMER1_INT, 	//5 	IPL5
		TIMER2_INT, 	//6 	IPL6
#ifdef CONFIG_MIPS_TC3262
		SI_SWINT_INT0, 	//7
		SI_SWINT_INT1, 	//8
#else
		TIMER3_INT, 	//7 	IPL7
		TIMER4_INT, 	//8 	IPL8
#endif
		TIMER5_INT, 	//9 	IPL9
		GPIO_INT,		//10	IPL11
#ifdef CONFIG_MIPS_TC3262
		RESERVED1,		//11	IPL20
		SI_PC1_INT, 	//12
		SI_PC_INT, 		//13
#else
		PCIE_A_INT,		//11	IPL20
		PCIE_SERR_INT,	//12	IPL21
		RESERVED3,		//13	IPL22
#endif
		APB_DMA0_INT,	//14	IPL12
		APB_DMA1_INT,	//15	IPL13
#ifdef CONFIG_MIPS_TC3262
		HSUART_INT,		//16	IPL23
#else
		RESERVED4,		//16	IPL23
#endif
		RESERVED5,		//17	IPL24
		DYINGGASP_INT,	//18	IPL25
		DMT_INT,		//19	IPL26
		ARBITER_ERR_INT,//20	IPL0
		MAC_INT,		//21	IPL3
		SAR_INT,		//22	IPL2
		USB_INT,		//23	IPL14
		PCI_A_INT,		//24
		PCI_B_INT,		//25
//		  RESERVED8,	  //24	  IPL27
//		  RESERVED9,	  //25	  IPL28
		XSLV0_INT,		//26	IPL15
		XSLV1_INT,		//27	IPL16
		XSLV2_INT,		//28	IPL17
#ifdef CONFIG_MIPS_TC3262
		SI_TIMER1_INT,	//29
		SI_TIMER_INT,	//30
#else
		XAPB0_INT,		//29	IPL18
		XAPB1_INT,		//30	IPL19
#endif
		SWR_INT 		//31	IPL4
	};
#endif

enum
interrupt_priority
{
		IPL0,	IPL1,	IPL2,	IPL3,	IPL4,
		IPL5,	IPL6,	IPL7,	IPL8,	IPL9,
		IPL10,	IPL11,	IPL12,	IPL13,	IPL14,
		IPL15,	IPL16,	IPL17,	IPL18,	IPL19,
		IPL20,	IPL21,	IPL22,	IPL23,	IPL24,
		IPL25,	IPL26,	IPL27,	IPL28,	IPL29,
		IPL30,	IPL31
};

/**************************
 * Timer Module Registers *
 **************************/
#define CR_TIMER_BASE  		0xBFBF0100
#define CR_TIMER_CTL    	(CR_TIMER_BASE + 0x00)
#define CR_TIMER0_LDV   	(CR_TIMER_BASE + 0x04)
#define CR_TIMER0_VLR    	(CR_TIMER_BASE + 0x08)
#define CR_TIMER1_LDV       (CR_TIMER_BASE + 0x0C)
#define CR_TIMER1_VLR       (CR_TIMER_BASE + 0x10)
#define CR_TIMER2_LDV       (CR_TIMER_BASE + 0x14)
#define CR_TIMER2_VLR       (CR_TIMER_BASE + 0x18)
#define CR_TIMER3_LDV       (CR_TIMER_BASE + 0x1C)
#define CR_TIMER3_VLR       (CR_TIMER_BASE + 0x20)
#define CR_TIMER4_LDV       (CR_TIMER_BASE + 0x24)
#define CR_TIMER4_VLR       (CR_TIMER_BASE + 0x28)
#define CR_TIMER5_LDV       (CR_TIMER_BASE + 0x2C)
#define CR_TIMER5_VLR       (CR_TIMER_BASE + 0x30)
/* new watchdog design */
#define CR_WDOG_THSLD       (CR_TIMER_BASE + 0x34)
#define CR_WDOG_RLD         (CR_TIMER_BASE + 0x38)

#define TIMER_ENABLE         1
#define TIMER_DISABLE        0
#define TIMER_TOGGLEMODE     1
#define TIMER_INTERVALMODE   0
#define TIMER_TICKENABLE     1
#define TIMER_TICKDISABLE    0
#define TIMER_WDENABLE       1
#define TIMER_WDDISABLE      0
#define TIMER_HALTENABLE     1
#define TIMER_HALTDISABLE    0

#define TIMERTICKS_1MS       1
#define TIMERTICKS_10MS      10  // set timer ticks as 10 ms
#define TIMERTICKS_100MS     100
#define TIMERTICKS_1S        1000
#define TIMERTICKS_10S       10000

#define timerCtlSet(timer_no, timer_enable, timer_mode,timer_halt)	timer_Configure(timer_no, timer_enable, timer_mode, timer_halt)
#define timerWdSet(tick_enable, watchdog_enable) timer_WatchDogConfigure(tick_enable,watchdog_enable)
#define timerLdvSet(timer_no,val) *(volatile uint32 *)(CR_TIMER0_LDV+timer_no*0x08) = (val)
#define timerVlrGet(timer_no,val) (val)=*(volatile uint32 *)(CR_TIMER0_VLR+timer_no*0x08)

/**************************
 * Timer Module Registers *
 **************************/
#define CR_CPUTMR_BASE 		0xBFBF0400
#define CR_CPUTMR_CTL    	(CR_CPUTMR_BASE + 0x00)
#define CR_CPUTMR_CMR0    	(CR_CPUTMR_BASE + 0x04)
#define CR_CPUTMR_CNT0    	(CR_CPUTMR_BASE + 0x08)
#define CR_CPUTMR_CMR1    	(CR_CPUTMR_BASE + 0x0c)
#define CR_CPUTMR_CNT1    	(CR_CPUTMR_BASE + 0x10)

/*************************
 * GPIO Module Registers *
 *************************/
#define CR_GPIO_BASE       	0xBFBF0200
#define CR_GPIO_CTRL	    (CR_GPIO_BASE + 0x00)
#define CR_GPIO_DATA	    (CR_GPIO_BASE + 0x04)
#define CR_GPIO_INTS      	(CR_GPIO_BASE + 0x08)
#define CR_GPIO_EDET	    (CR_GPIO_BASE + 0x0C)
#define CR_GPIO_LDET       	(CR_GPIO_BASE + 0x10)
#define CR_GPIO_ODRAIN      (CR_GPIO_BASE + 0x14)
#define CR_GPIO_CTRL1	    (CR_GPIO_BASE + 0x20)
#ifdef TCSUPPORT_GPIO_ECM
#define CR_SGPIO_DATA        (CR_GPIO_BASE + 0x24)
#define CR_SGPIO_CDIV        (CR_GPIO_BASE + 0x28)
#define CR_SGPIO_CDLY        (CR_GPIO_BASE + 0x2C)
#define CR_SGPIO_MODE	    (CR_GPIO_BASE + 0x30)
#define CR_GPIO_FLAMOD	    (CR_GPIO_BASE + 0x34)
#define CR_GPIO_IMME	    (CR_GPIO_BASE + 0x38)
#define CR_GPIO_FLAP0	    (CR_GPIO_BASE + 0x3C)
#define CR_GPIO_FLAP1	    (CR_GPIO_BASE + 0x40)
#define CR_GPIO_FLAP2 	    (CR_GPIO_BASE + 0x44)
#define CR_GPIO_FLAP3	    (CR_GPIO_BASE + 0x48)
#define CR_GPIO_FMAP0	    (CR_GPIO_BASE + 0x4C)
#define CR_GPIO_FMAP1 	    (CR_GPIO_BASE + 0x50)
#define CR_SGPIO_FMAP0	    (CR_GPIO_BASE + 0x54)
#define CR_SGPIO_FMAP1	    (CR_GPIO_BASE + 0x58)
#define CR_SGPIO_FMAP2	    (CR_GPIO_BASE + 0x5C)
#define CR_GPIO_TYPE	    0xBFB000B0
#endif
/* MT7510 */
#define CR_GPIO_CTRL2	    (CR_GPIO_BASE + 0x60)
#define CR_GPIO_CTRL3	    (CR_GPIO_BASE + 0x64)
#define CR_GPIO_FLAMOD_EXT	(CR_GPIO_BASE + 0x68)
#define CR_GPIO_DATA1		(CR_GPIO_BASE + 0x70)
#define CR_GPIO_ODRAIN1     (CR_GPIO_BASE + 0x78)

#define GPIO_IN				0x0
#define GPIO_OUT			0x1
#define GPIO_ALT_IN			0x2
#define GPIO_ALT_OUT		0x3

#define GPIO_E_DIS			0x0
#define GPIO_E_RISE			0x1
#define GPIO_E_FALL			0x2
#define GPIO_E_BOTH			0x3

#define GPIO_L_DIS			0x0
#define GPIO_L_HIGH			0x1
#define GPIO_L_LOW			0x2
#define GPIO_L_BOTH			0x3

/*****************************
 * Arbiter/Decoder Registers *
 *****************************/
#define CR_AHB_BASE       	0xBFB00000
#define CR_AHB_AACS	    	(CR_AHB_BASE + 0x00)
#define CR_AHB_ABEM      	(CR_AHB_BASE + 0x08)
#define CR_AHB_ABEA		    (CR_AHB_BASE + 0x0C)
#define CR_AHB_DMB0       	(CR_AHB_BASE + 0x10)
#define CR_AHB_DMB1       	(CR_AHB_BASE + 0x14)
#define CR_AHB_DMB2       	(CR_AHB_BASE + 0x18)
#define CR_AHB_DMB3       	(CR_AHB_BASE + 0x1C)
#define CR_AHB_SMB0       	(CR_AHB_BASE + 0x20)
#define CR_AHB_SMB1       	(CR_AHB_BASE + 0x24)
#define CR_AHB_SMB2       	(CR_AHB_BASE + 0x28)
#define CR_AHB_SMB3       	(CR_AHB_BASE + 0x2C)
#define CR_AHB_SMB4       	(CR_AHB_BASE + 0x30)
#define CR_AHB_SMB5       	(CR_AHB_BASE + 0x34)

/* RT63165 */
#define CR_ERR_ADDR    		(CR_AHB_BASE + 0x3c)
#define CR_PRATIR      		(CR_AHB_BASE + 0x58)
#define CR_MON_TMR     		(CR_AHB_BASE + 0x60)

/*MT7510*/
#define CR_AHB_NMI_CONF		(CR_AHB_BASE + 0x50)

#define CR_AHB_PMCR       	(CR_AHB_BASE + 0x80)
#define CR_AHB_DMTCR       	(CR_AHB_BASE + 0x84)
#define CR_AHB_PCIC	       	(CR_AHB_BASE + 0x88)
#define CR_AHB_HWCONF       (CR_AHB_BASE + 0x8C)
#define CR_AHB_SSR       	(CR_AHB_BASE + 0x90)
#define CR_AHB_SSTR			(CR_AHB_BASE + 0x9C)
#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
#define CR_IMEM       	(CR_AHB_BASE + 0x9C)
#define CR_DMEM       	(CR_AHB_BASE + 0xA0)
#endif
/* RT63365 */
#define CR_CRCC_REG		(CR_AHB_BASE + 0xA0)
#define CR_AHB_UHCR		(CR_AHB_BASE + 0xA8)
#define CR_AHB_ABMR       	(CR_AHB_BASE + 0xB8)
#define CR_CKGEN_CONF		(CR_AHB_BASE + 0xC0)
#define CR_PSMCR       		(CR_AHB_BASE + 0xCC)
#define CR_PSMDR       		(CR_AHB_BASE + 0xD0)
#define CR_PSMMR       		(CR_AHB_BASE + 0xD0)

/* RT63165 */
#define CR_SRAM       		(CR_AHB_BASE + 0xF4)
#define CR_AHB_HWCONF2      (CR_AHB_BASE + 0xF8)

/* RT63365 */
#define CR_AHB_CLK			(CR_AHB_BASE + 0x1c0)
#define CR_CLK_CFG     		(CR_AHB_BASE + 0x82c)
#define CR_RSTCTRL2    		(CR_AHB_BASE + 0x834)
#define CR_GPIO_SHR			(CR_AHB_BASE + 0x860)			

#define CR_BUSTIMEOUT_SWITCH     (CR_AHB_BASE + 0x92c)
/*************************************************
 * SRAM/FLASH/ROM Controller Operation Registers *
 *************************************************/
#define CR_SMC_BASE       	0xBFB10000
#define CR_SMC_BCR0	    	(CR_SMC_BASE + 0x00)
#define CR_SMC_BCR1	    	(CR_SMC_BASE + 0x04)
#define CR_SMC_BCR2	    	(CR_SMC_BASE + 0x08)
#define CR_SMC_BCR3	    	(CR_SMC_BASE + 0x0C)
#define CR_SMC_BCR4	    	(CR_SMC_BASE + 0x10)
#define CR_SMC_BCR5	    	(CR_SMC_BASE + 0x14)

/************************************************
 * System Control Uint                          *
 ************************************************/
 /* MT7510 */
#define CR_SCU_BASE				0xbfb00000
#define CR_DRAMC_HW_SREF_CONF	(0x44 | CR_SCU_BASE)
#define CR_DRAMC_CONF			(0x74 | CR_SCU_BASE)

/*****************************
 * Clock Generator Registers *
 *****************************/

/****************************
 * USB Module Registers *
 ****************************/

#define CR_USB_BASE     0xBFB70000

        // --- System Control Register ---
#define CR_USB_SYS_CTRL_REG           (0x00 | CR_USB_BASE)

        // --- Device Control Register ---
#define CR_USB_DEV_CTRL_REG           (0x04 | CR_USB_BASE)

        // --- Interrupt Status Register ---
#define CR_USB_INTR_STATUS_REG        (0x08 | CR_USB_BASE)

        // --- Interrupt Mask Register ---
#define CR_USB_INTR_MASK_REG          (0x0c | CR_USB_BASE)

        // --- Control Endpoint I/O Mode Control Register ---
#define CR_USB_CTRL_ENDP_IO_CTRL_REG  (0x10 | CR_USB_BASE)

        // --- Control Endpoint I/O Mode OUT Transfer Data Register #00 ---
#define CR_USB_CTRL_ENDP_IO_OUT_REG0  (0x18 | CR_USB_BASE)

        // --- Control Endpoint I/O Mode OUT Transfer Data Register #01 ---
#define CR_USB_CTRL_ENDP_IO_OUT_REG1  (0x1c | CR_USB_BASE)

        // --- Control Endpoint I/O Mode IN Transfer Data Register #00 ---
#define CR_USB_CTRL_ENDP_IO_IN_REG0   (0x20 | CR_USB_BASE)

        // --- Control Endpoint I/O Mode IN Transfer Data Register #01 ---
#define CR_USB_CTRL_ENDP_IO_IN_REG1   (0x24 | CR_USB_BASE)

        // --- Interrupt IN Endpoint Control Register ---
#define CR_USB_INTR_IN_ENDP_CTRL_REG  (0x30 | CR_USB_BASE)

        // --- Interrupt IN Endpoint IN Transfer Data Register #00 ---
#define CR_USB_INTR_IN_ENDP_IN_REG0   (0x38 | CR_USB_BASE)

        // --- Interrupt IN Endpoint IN Transfer Data Register #01 ---
#define CR_USB_INTR_IN_ENDP_IN_REG1   (0x3c | CR_USB_BASE)

        // --- Bulk/ISO OUT Descriptor Pointer Register ---
#define CR_USB_BULKISO_OUT_DESCP_BASE_REG   (0x40 | CR_USB_BASE)

        // --- Bulk/ISO IN Descriptor Pointer Register ---
#define CR_USB_BULKISO_IN_DESCP_BASE_REG    (0x44 | CR_USB_BASE)

        // --- Bulk/ISO IN/OUT Endpoint Number Register ---
#define CR_USB_BULKISO_INOUT_ENDP_NUM_REG   (0x48 | CR_USB_BASE)

        // --- Bulk/ISO Endpoint DMA Control Register ---
#define CR_USB_BULKISO_ENDP_DMA_CTRL_REG    (0x4c | CR_USB_BASE)

        // --- Bulk/ISO Endpoint DMA Configuration Register ---
#define CR_USB_BULKISO_ENDP_DMA_CONF_REG    (0x50 | CR_USB_BASE)

        // --- ISO Endpoint Transfer Delimiter Register #00 ---
#define CR_USB_ISO_ENDP_DELIMITER_REG0      (0x58 | CR_USB_BASE)

        // --- ISO Endpoint Transfer Delimiter Register #01 ---
#define CR_USB_ISO_ENDP_DELIMITER_REG1      (0x5c | CR_USB_BASE)

        // --- Vendor ID Register ---
#define CR_USB_VENDOR_ID_REG                (0x68 | CR_USB_BASE)

        // --- Product ID Register ---
#define CR_USB_PRODUCT_ID_REG               (0x6c | CR_USB_BASE)

/*************************
 * HOST BRIDGE Registers *
 * ***********************/
#define HOST_BRIDGE_BASE 	0xBFB80000
#define CR_CFG_ADDR_REG 	(HOST_BRIDGE_BASE+0x0020)
#define CR_CFG_DATA_REG 	(HOST_BRIDGE_BASE+0x0024)
/****************************
 * ATM SAR Module Registers *
 ****************************/
#define TSCONTROL_BASE			0xBFB00000
#define TSARM_REGISTER_BASE		(TSCONTROL_BASE + 0x00060000)

/* ----- General configuration registers  ----- */

/* ----- Reset And Identify register  ----- */
#define TSARM_RAI				VPint(TSARM_REGISTER_BASE + 0x0000)
/* ----- General Configuration register  ----- */
#define TSARM_GFR				VPint(TSARM_REGISTER_BASE + 0x0004)
/* ----- Traffic Scheduler Timer Base Counter register  ----- */
#define TSARM_TSTBR				VPint(TSARM_REGISTER_BASE + 0x0008)
/* ----- Receive Maximum Packet Length register  ----- */
#define TSARM_RMPLR				VPint(TSARM_REGISTER_BASE + 0x000c)
#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
//Transmit Priority 0/1 Data Buffer Control and Status Register
#define TSARM_TXDBCSR_P01		VPint(TSARM_REGISTER_BASE + 0x0010)
#else
/* ----- TX Data Buffer Control and Status register  ----- */
#define TSARM_TXDBCSR			VPint(TSARM_REGISTER_BASE + 0x0010)
#endif
/* ----- TX OAM Buffer Control and Status register  ----- */
#define TSARM_TXMBCSR			VPint(TSARM_REGISTER_BASE + 0x0014)
/* ----- RX Data Buffer Control and Status register  ----- */
#define TSARM_RXDBCSR			VPint(TSARM_REGISTER_BASE + 0x0018)
/* ----- RX OAM Buffer Control and Status register  ----- */
#define TSARM_RXMBCSR			VPint(TSARM_REGISTER_BASE + 0x001c)
/* ----- Last IRQ Status register  ----- */
#define TSARM_LIRQ				VPint(TSARM_REGISTER_BASE + 0x0020)
/* ----- IRQ Queue Base Address register  ----- */
#define TSARM_IRQBA				VPint(TSARM_REGISTER_BASE + 0x0024)
/* ----- IRQ Queue Entry Length register  ----- */
#define TSARM_IRQLEN			VPint(TSARM_REGISTER_BASE + 0x0028)
/* ----- IRQ Head Indication register  ----- */
#define TSARM_IRQH				VPint(TSARM_REGISTER_BASE + 0x002c)
/* ----- Clear IRQ Entry register  ----- */
#define TSARM_IRQC				VPint(TSARM_REGISTER_BASE + 0x0030)
#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
//Traffic Scheduler Line Rate Counter Register
#define TSARM_TXSLRC			VPint(TSARM_REGISTER_BASE + 0x0034)
//Transmit Priority 2/3 Data Buffer Control and Status Register
#define TSARM_TXDBCSR_P23		VPint(TSARM_REGISTER_BASE + 0x0038)
#endif

/* ----- VC IRQ Mask register  ----- */
#define TSARM_IRQM_BASE			(TSARM_REGISTER_BASE + 0x0040)
#define TSARM_IRQM(vc)			VPint(TSARM_IRQM_BASE + (vc * 4))
#define TSARM_IRQMCC			VPint(TSARM_IRQM_BASE + 0x0040)
#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
#define TSARM_IRQ_QUE_THRE		VPint(TSARM_REGISTER_BASE + 0x0084)		//IRQ Queue Threshold Register
#define TSARM_IRQ_TIMEOUT_CTRL 	VPint(TSARM_REGISTER_BASE + 0x0088)		//IRQ Timeout Control Register
#endif

/* ----- VC Configuration register  ----- */
#define TSARM_VCCR_BASE			(TSARM_REGISTER_BASE + 0x0100)
#define TSARM_VCCR(vc)			VPint(TSARM_VCCR_BASE + (vc * 4))
#define TSARM_CCCR				VPint(TSARM_VCCR_BASE + 0x0040)
#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
/* ----- DMA WRR Configuration Register (DMA_WRR_WEIT) (for TC3162L4) ----- */
#define TSARM_DMAWRRCR			VPint(TSARM_REGISTER_BASE + 0x0150)
#endif
/* ----- Transmit Buffer Descriptor register  ----- */
#define TSARM_TXDCBDA_BASE		(TSARM_REGISTER_BASE + 0x0200)
#define TSARM_TXDCBDA(vc)		VPint(TSARM_TXDCBDA_BASE + (vc * 4))
#define TSARM_TXMCBDA_BASE		(TSARM_REGISTER_BASE + 0x0240)
#define TSARM_TXMCBDA(vc)		VPint(TSARM_TXMCBDA_BASE + (vc * 4))

#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
#define TSARM_CC_TX_BD_BASE				VPint(TSARM_REGISTER_BASE + 0x0228)		//Control Channel Transmit BD Base Address 0x228
#define TSARM_CC_TX_BD_MNG_BASE			VPint(TSARM_REGISTER_BASE + 0x0268)		//Control Channel Transmit BD Management Base
#define TSARM_VC_TX_BD_PRIORITY01_BASE		(TSARM_REGISTER_BASE + 0x0280)
#define TSARM_VC_TX_BD_PRIORITY01(vc)		VPint(TSARM_VC_TX_BD_PRIORITY01_BASE + vc * 4)		//VC0 Transmit BD Data Priority 0/1 Base 280
#define TSARM_VC_TX_BD_PRIORITY23_BASE		(TSARM_REGISTER_BASE + 0x02c0)
#define TSARM_VC_TX_BD_PRIORITY23(vc)		VPint(TSARM_VC_TX_BD_PRIORITY23_BASE + vc * 4)		//VC0 Transmit BD Data Priority 0/1 Base 280
#else
#define TSARM_TXCCBDA			VPint(TSARM_REGISTER_BASE + 0x0280)
#endif

/* ----- Receive Buffer Descriptor register  ----- */
#define TSARM_RXDCBDA_BASE		(TSARM_REGISTER_BASE + 0x0300)
#define TSARM_RXDCBDA(vc)		VPint(TSARM_RXDCBDA_BASE + (vc * 4))
#define TSARM_RXMCBDA_BASE		(TSARM_REGISTER_BASE + 0x0340)
#define TSARM_RXMCBDA(vc)		VPint(TSARM_RXMCBDA_BASE + (vc * 4))

#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
#define TSARM_CC_RX_BD_BASE			VPint(TSARM_REGISTER_BASE + 0x328)		//Control Channel Receive BD Base Address	0x328
#define TSARM_CC_RX_BD_MNG_BASE		VPint(TSARM_REGISTER_BASE + 0x368)		//Control Channel Receive BD Management Base	0x368
#define TSARM_VC_RX_DATA_BASE				(TSARM_REGISTER_BASE + 0x380)
#define TSARM_VC_RX_DATA(vc)			VPint(TSARM_VC_RX_DATA_BASE + vc * 4)	//VC0 Receive BD Data Base	0x380
#else
#define TSARM_RXCCBDA			VPint(TSARM_REGISTER_BASE + 0x0380)
#endif

/* ----- Traffic Scheduler register  ----- */
#define TSARM_PCR_BASE			(TSARM_REGISTER_BASE + 0x0400)
#define TSARM_PCR(vc)			VPint(TSARM_PCR_BASE + (vc * 4))
#define TSARM_SCR_BASE			(TSARM_REGISTER_BASE + 0x0440)
#define TSARM_SCR(vc)			VPint(TSARM_SCR_BASE + (vc * 4))
#define TSARM_MBSTP_BASE		(TSARM_REGISTER_BASE + 0x0480)
#define TSARM_MBSTP(vc)			VPint(TSARM_MBSTP_BASE + (vc * 4))

#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
#define TSARM_MAX_FRAME_SIZE_BASE	(TSARM_REGISTER_BASE + 0x04c0)
#define TSARM_MAX_FRAME_SIZE(vc)		VPint(TSARM_MAX_FRAME_SIZE_BASE + (vc * 4))
/* define for TC3162L4 */
#define TSARM_TRAFFIC_SHAPER_WEIGHT_BASE (TSARM_REGISTER_BASE + 0x0500)
#define TSARM_TRAFFIC_SHAPER_WEIGHT(vc)     VPint(TSARM_TRAFFIC_SHAPER_WEIGHT_BASE + (vc * 4))
#else
/* ----- Receive Timeout register  ----- */
#define TSARM_RTOCNT_BASE		(TSARM_REGISTER_BASE + 0x0500)
#define TSARM_RTOCNT(vc)		VPint(TSARM_RTOCNT_BASE + (vc * 4))
#endif

/* ----- TX Statistic Counter register  ----- */
#define TSARM_TDCNT_BASE		(TSARM_REGISTER_BASE + 0x0600)
#define TSARM_TDCNT(vc)			VPint(TSARM_TDCNT_BASE + (vc * 4))
#define TSARM_TDCNTCC			VPint(TSARM_TDCNT_BASE + 0x0040)

/* ----- RX Statistic Counter register  ----- */
#define TSARM_RDCNT_BASE		(TSARM_REGISTER_BASE + 0x0700)
#define TSARM_RDCNT(vc)			VPint(TSARM_RDCNT_BASE + (vc * 4))
#define TSARM_RDCNTCC			VPint(TSARM_RDCNT_BASE + 0x0040)
#define TSARM_MISCNT			VPint(TSARM_RDCNT_BASE + 0x0044)

#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
#define TSARM_MPOA_GCR				VPint(TSARM_REGISTER_BASE + 0x0800)			//MPOA global control register
#define TSARM_VC_MPOA_CTRL_BASE			(TSARM_REGISTER_BASE + 0x0810)			//VC0 ~9  MPOA Control register
#define TSARM_VC_MPOA_CTRL(vc)			VPint(TSARM_VC_MPOA_CTRL_BASE + vc * 4)
#define TSARM_MPOA_HFIV11				VPint(TSARM_REGISTER_BASE + 0x0850)			//MPOA header Field1 Insertion Value1
#define TSARM_MPOA_HFIV12				VPint(TSARM_REGISTER_BASE + 0x0854)			//MPOA header Field1 Insertion Value2
#define TSARM_MPOA_HFIV13				VPint(TSARM_REGISTER_BASE + 0x0858)			//MPOA header Field2 Insertion Value1
#define TSARM_MPOA_HFIV21				VPint(TSARM_REGISTER_BASE + 0x0860)			//MPOA header Field2 Insertion Value2
#define TSARM_MPOA_HFIV22				VPint(TSARM_REGISTER_BASE + 0x0864)			//MPOA header Field2 Insertion Value2
#define TSARM_MPOA_HFIV23				VPint(TSARM_REGISTER_BASE + 0x0868)			//MPOA header Field2 Insertion Value2
#define TSARM_MPOA_HFIV31				VPint(TSARM_REGISTER_BASE + 0x0870)			//MPOA header Field3 Insertion Value1
#define TSARM_MPOA_HFIV32				VPint(TSARM_REGISTER_BASE + 0x0874)			//MPOA header Field3 Insertion Value2
#define TSARM_MPOA_HFIV33				VPint(TSARM_REGISTER_BASE + 0x0878)			//MPOA header Field3 Insertion Value3
#define TSARM_MPOA_HFIV41				VPint(TSARM_REGISTER_BASE + 0x0880)			//MPOA header Field4 Insertion Value1
#define TSARM_MPOA_HFIV42				VPint(TSARM_REGISTER_BASE + 0x0884)			//MPOA header Field4 Insertion Value2
#define TSARM_MPOA_HFIV43				VPint(TSARM_REGISTER_BASE + 0x0888)			//MPOA header Field4 Insertion Value2
#endif

/**************************
 * USB Module Registers *
 **************************/

#define LA_DEBUG_TRIGGER(addr,val) VPint(0xbfc00000+addr) = val
/**************************
* USB 2.0 device Register *
**************************/
#define CR_USB20_BASE     0xBFB70000

// --- System Control Register ---
#define CR_USB20_SYS_CTRL_REG			(0x00 | CR_USB20_BASE)

// --- Device Control Register ---
#define CR_USB20_DEV_CTRL_REG			(0x04 | CR_USB20_BASE)

// --- Interrupt Status Register ---
#define CR_USB20_INTR_STATUS_REG		(0x0c | CR_USB20_BASE)

// --- Interrupt ENABLE Register ---
#define CR_USB20_INTR_ENABLE_REG          	(0x10 | CR_USB20_BASE)

// --- Interrupt Timing Control Register ---
#define CR_USB20_INTR_TIMING_CONTROL_REG	(0x14 | CR_USB20_BASE)

// --- SETUP Receive Data Buffer Register ---
#define CR_USB20_SETUP_BASE_REG          	(0x20 | CR_USB20_BASE)

// --- Control OUT Receive Data Buffer Pointer Register ---
#define CR_USB20_CONTROL_OUT_BASE_REG		(0x24 | CR_USB20_BASE)

// --- Control IN Receive Data Buffer Pointer Register ---
#define CR_USB20_CONTROL_IN_BASE_REG		(0x28 | CR_USB20_BASE)

// --- Control IN Receive Data Buffer Pointer Register ---
#define CR_USB20_CONTROL_CONF_REG		(0x2c | CR_USB20_BASE)

// --- CONTROL Endpoint DMA Transfer Control Register ---
#define CR_USB20R_CONTROL_EP_DMA_CTRL_REG	(0x30 | CR_USB20_BASE)

// --- CONTROL Endpoint DMA Transfer Status Register ---
#define CR_USB20_CONTROL_EP_DMA_STATUS_REG	(0x34 | CR_USB20_BASE)

// --- INTERRUPT IN Transmit Data Buffer Pointer  Register ---
#define CR_USB20_INTERRUPT_IN_BASE_REG		(0x38 | CR_USB20_BASE)

// --- INTERRUPT IN Configuration   Register ---
#define CR_USB20_INTERRUPT_IN_CONF_REG		(0x3c | CR_USB20_BASE)

// --- INTERRUPT IN Endpoint DMA Transfer Control Register ---
#define CR_USB20_INTERRUPT_IN_DMA_CTRL_REG	(0x40 | CR_USB20_BASE)

// --- INTERRUPT IN Endpoint DMA Transfer Status Register ---
#define CR_USB20_INTERRUPT_IN_EP_DMA_STATUS_REG	(0x44 | CR_USB20_BASE)

// --- Bulk/Ctrl/Intr IN/OUT Underrun/Overrun Error Counter  Register ---
#define CR_USB20_STATUS_COUNT_REG		(0x48 | CR_USB20_BASE)

// --- BULK OUT Endpoint Transfer DMA Polling Demand Control Register ---
#define CR_USB20_BULK_OUT_DMA_POLLING_REG	(0x60 | CR_USB20_BASE)

// --- BULK IN Endpoint Transfer DMA Polling Demand Control Register ---
#define CR_USB20_BULK_IN_DMA_POLLING_REG	(0x64 | CR_USB20_BASE)

// --- Bulk OUT Endpoint Transfer Dscriptor Base Address Register ---
#define CR_USB20_BULK_OUT_DESC_BASE_REG		(0x68 | CR_USB20_BASE)

// --- Bulk IN Endpoint Transfer Dscriptor Base Address Register ---
#define CR_USB20_BULK_IN_DESC_BASE_REG		(0x6c | CR_USB20_BASE)

// --- Bulk OUT/IN Endpoint Transfer Dscriptor Rinf Size/Offset Register ---
#define CR_USB20_BULK_DESC_SIZE_OFFSET_REG	(0x70 | CR_USB20_BASE)

// --- Bulk OUT/IN Endpoint Configuration Register ---
#define CR_USB20_BULK_EP_CONF_REG		(0x74 | CR_USB20_BASE)

// --- Bulk OUT/IN Endpoint DMA Transfer Control Register ---
#define CR_USB20_BULK_EP_DMA_CTRL_REG          	(0x78 | CR_USB20_BASE)

// --- Bulk OUT/IN Endpoint DMA Transfer Status Register ---
#define CR_USB20_BULK_EP_DMA_STATUS_REG         (0x7c | CR_USB20_BASE)

// --- UDC Setup Command Address Register ---
#define CR_USB20_UDC_SETUP_COMMAND_ADDR_REG	(0x80 | CR_USB20_BASE)

// --- UDC Control Endpoint Information Register ---
#define CR_USB20_UDC_CTRL_EP_INFO_REG          	(0x84 | CR_USB20_BASE)

// --- UDC BULK IN Endpoint Information Register ---
#define CR_USB20_UDC_BULK_IN_EP_INFO_REG    	(0x88 | CR_USB20_BASE)

// --- UUDC BULK OUT Endpoint Information Register ---
#define CR_USB20_UDC_BULK_OUT_EP_INFO_REG	(0x8c | CR_USB20_BASE)

// --- UDC INTERRUPT IN Endpoint Information Register ---
#define CR_USB20_UDC_INTERRUPT_IN_EP_INFO_REG	(0x90 | CR_USB20_BASE)

#if 0
#define TCSUPPORT_CPU_MT7510_FAKE

#ifdef TCSUPPORT_CPU_MT7510
#undef DEFAULT_USE_EXT_SWIC
#endif
#ifdef TCSUPPORT_CPU_MT7520
#define DEFAULT_USE_EXT_SWIC
#endif
#if defined(TCSUPPORT_CPU_RT65168) || defined(TCSUPPORT_CPU_TC3182)	// for early MT7530 FPGA verification only
#define DEFAULT_USE_EXT_SWIC
#endif

#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7520E) || defined(TCSUPPORT_CPU_MT751x20G25G) || defined(TCSUPPORT_CPU_MT7511G) || defined(TCSUPPORT_CPU_MT752025)
//#define EXT_SWITCH_PHY_CONNECT_INT_MDIO

#ifdef EXT_SWITCH_PHY_CONNECT_INT_MDIO
#define EXT_SWITCH_PHY_ADDR_OFFSET	(0)
#define INT_SWITCH_PHY_ADDR_OFFSET	(8)

#endif	//EXT_SWITCH_PHY_CONNECT_INT_MDIO

#endif
#endif
#endif /* _TC3162_H_ */
