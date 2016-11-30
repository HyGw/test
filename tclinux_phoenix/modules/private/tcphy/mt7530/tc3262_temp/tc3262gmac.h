/*
** $Id: //BBN_Linux/Branch/Branch_for_MT7520_20120528/tclinux_phoenix/modules/private/tc3262/tc3262gmac.h#2 $
*/
/************************************************************************
 *
 *	Copyright (C) 2008 Trendchip Technologies, Corp.
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
** $Log: tc3262gmac.h,v $
** Revision 1.2  2011/03/17 11:36:17  shnwind
** Add a switch to vlanpt disable
**
** Revision 1.1.1.1  2010/09/30 21:14:53  josephxu
** modules/public, private
**
** Revision 1.4  2010/09/25 15:04:39  here
** [Ehancement]TCConsole to support capture the console log message and set command into cpe.
**
** Revision 1.3  2010/08/26 06:29:51  lino
** 1. When detecting PHY change speed/mode, do GMAC software reset.
** 2. Adjust TX/RX ring size from 64/256 to 128/128, NAPI weight from 64 to 32.
** 3. Dump EEE register in eth_reg
** 4. Fixed compiler errors when enable TCPHY_DEBUG.
** 5. Disable flow control.
** 6. Can load GMAC1 driver when LDV or 216-pin package.
** 7. GMAC1 PHY address fixed to 9.
** 8. Skip 4port switch searching in GMAC1.
**
** Revision 1.2  2010/06/15 10:55:18  lino
** add tc3182 support
**
** Revision 1.1.1.1  2010/04/09 09:34:42  feiyan
** New TC Linux Make Flow Trunk
**
** Revision 1.1.1.1  2009/12/17 01:47:37  josephxu
** 20091217, from Hinchu ,with VoIP
**
 */
#ifndef _TC3262GMAC_H
#define _TC3262GMAC_H

/* scatter & gather mode */
//#define TC3262_GMAC_SG_MODE

#define SG_MAX_PKT_LEN	   		(128)

#define MAC_TXDESCP_NO			128		/* Max tx buffer Cnts , default=128 */
#define MAC_RXDESCP_NO			128 	/* Max rx buffer Cnts , default=128 */

#define MAC_NAPI_WEIGHT			32

#define MAC_RXDESCP_SIZE		32		/* 8 DWords */
#define MAC_TXDESCP_SIZE		16		/* 4 DWords */

#define TX_BUF_RELEASE_THRESHOLD 4  	/* default:4 */

#define TX_QUEUE_NUM 			8

#define K0_TO_K1(x)				(((uint32)x) | 0xa0000000)
#define K1_TO_PHY(x)			(((uint32)x) & 0x1fffffff)

#define MAC_STATISTIC_ON		1
#define MAC_STATISTIC_OFF		0

#define GMAC_PRIORITY_MASK  	(0x7)

/*****************************
 * Ethernet Module Registers *
 *****************************/
#ifdef TC3262_GMAC1
#define CR_GMAC_BASE     	0xBFB58000
#else
#define CR_GMAC_BASE     	0xBFB50000
#endif
#define CR_GMAC_GCTR      	(0x00 | CR_GMAC_BASE)
#define CR_GMAC_GIMR      	(0x04 | CR_GMAC_BASE)
#define CR_GMAC_GISR  	   	(0x08 | CR_GMAC_BASE)
#define CR_GMAC_GSPR     	(0x0c | CR_GMAC_BASE)
#define CR_GMAC_GMAR     	(0x10 | CR_GMAC_BASE)
#define CR_GMAC_RBFR     	(0x14 | CR_GMAC_BASE)
#define CR_GMAC_SMDR     	(0x18 | CR_GMAC_BASE)
#define CR_GMAC_GMAR1     	(0x1c | CR_GMAC_BASE)
#define CR_GMAC_DPSR     	(0x20 | CR_GMAC_BASE)
#define CR_GMAC_PCRR     	(0x24 | CR_GMAC_BASE)
#define CR_GMAC_VPMR     	(0x28 | CR_GMAC_BASE)
#define CR_GMAC_TPR0     	(0x2c | CR_GMAC_BASE)
#define CR_GMAC_TPR1     	(0x30 | CR_GMAC_BASE)
#define CR_GMAC_TPR2     	(0x34 | CR_GMAC_BASE)
#define CR_GMAC_TPR3     	(0x38 | CR_GMAC_BASE)
#define CR_GMAC_TCPR     	(0x3c | CR_GMAC_BASE)
#define CR_GMAC_GSGCR     	(0x40 | CR_GMAC_BASE)
#define CR_GMAC_GRRBR     	(0x44 | CR_GMAC_BASE)
#define CR_GMAC_GTRBR     	(0x48 | CR_GMAC_BASE)
#define CR_GMAC_GRRSR     	(0x4c | CR_GMAC_BASE)
#define CR_GMAC_GTRS01R    	(0x50 | CR_GMAC_BASE)
#define CR_GMAC_GTRS23R    	(0x54 | CR_GMAC_BASE)
#define CR_GMAC_GTRS45R    	(0x58 | CR_GMAC_BASE)
#define CR_GMAC_GTRS67R    	(0x5c | CR_GMAC_BASE)
#define CR_GMAC_GRHPR    	(0x60 | CR_GMAC_BASE)
#define CR_GMAC_GRTPR    	(0x64 | CR_GMAC_BASE)
#define CR_GMAC_GTHPR(ring)    	((0x68 | CR_GMAC_BASE) + (ring) * 4)
#define CR_GMAC_GTTPR(ring)   	((0x88 | CR_GMAC_BASE) + (ring) * 4)
#define CR_GMAC_GTLWR    	(0xa8 | CR_GMAC_BASE)
#define CR_GMAC_GTUSR    	(0xac | CR_GMAC_BASE)
#define CR_GMAC_GRXCR    	(0xb0 | CR_GMAC_BASE)
#define CR_GMAC_GPRCTR    	(0xb4 | CR_GMAC_BASE)
#define CR_GMAC_GRIETR    	(0xb8 | CR_GMAC_BASE)
#define CR_GMAC_GTIETR    	(0xbc | CR_GMAC_BASE)
#define CR_GMAC_GTQCR    	(0xc0 | CR_GMAC_BASE)
#define CR_GMAC_GTSLR    	(0xc4 | CR_GMAC_BASE)
#define CR_GMAC_GSBCR0    	(0xc8 | CR_GMAC_BASE)
#define CR_GMAC_GSBCR1    	(0xcc | CR_GMAC_BASE)
#define CR_GMAC_GSBCR2    	(0xd0 | CR_GMAC_BASE)
#define CR_GMAC_GSBCR3    	(0xd4 | CR_GMAC_BASE)
#define CR_GMAC_GWWR0    	(0xd8 | CR_GMAC_BASE)
#define CR_GMAC_GWWR1    	(0xdc | CR_GMAC_BASE)
#define CR_GMAC_GWWR2    	(0xe0 | CR_GMAC_BASE)
#define CR_GMAC_GWWR3    	(0xe4 | CR_GMAC_BASE)
#define CR_GMAC_RTRR    	(0xe8 | CR_GMAC_BASE)
#define CR_GMAC_RTSR    	(0xec | CR_GMAC_BASE)
#define CR_GMAC_RTNSR    	(0xf0 | CR_GMAC_BASE)
#define CR_GMAC_TTRR    	(0xf4 | CR_GMAC_BASE)
#define CR_GMAC_TTSR    	(0xf8 | CR_GMAC_BASE)
#define CR_GMAC_TTNSR    	(0xfc | CR_GMAC_BASE)
#define CR_GMAC_ANPLR    	(0x100 | CR_GMAC_BASE)
#define CR_GMAC_ANER    	(0x104 | CR_GMAC_BASE)
#define CR_GMAC_PHYOR    	(0x108 | CR_GMAC_BASE)
#define CR_GMAC_ANTPR    	(0x10c | CR_GMAC_BASE)
#define CR_GMAC_ANRPR    	(0x110 | CR_GMAC_BASE)
#define CR_GMAC_ANSR    	(0x114 | CR_GMAC_BASE)
#define CR_GMAC_ANDPR    	(0x118 | CR_GMAC_BASE)
#define CR_GMAC_ANLUR    	(0x11c | CR_GMAC_BASE)
#define CR_GMAC_ANDNR    	(0x120 | CR_GMAC_BASE)
#define CR_GMAC_GRBSR    	(0x12c | CR_GMAC_BASE)
#define CR_GMAC_GRDLR     	(0x130 | CR_GMAC_BASE)
#define CR_GMAC_GTDLR     	(0x134 | CR_GMAC_BASE)
#define CR_GMAC_TXPCR     	(0x140 | CR_GMAC_BASE)
#define CR_GMAC_TXECCR     	(0x144 | CR_GMAC_BASE)
#define CR_GMAC_TXLCCR     	(0x148 | CR_GMAC_BASE)
#define CR_GMAC_TXSCCR     	(0x14c | CR_GMAC_BASE)
#define CR_GMAC_TXMCCR     	(0x150 | CR_GMAC_BASE)
#define CR_GMAC_TXPPCR     	(0x154 | CR_GMAC_BASE)
#define CR_GMAC_TXDCR     	(0x158 | CR_GMAC_BASE)
#define CR_GMAC_TXBCR     	(0x15c | CR_GMAC_BASE)
#define CR_GMAC_RXPCR     	(0x160 | CR_GMAC_BASE)
#define CR_GMAC_RXBPCR     	(0x164 | CR_GMAC_BASE)
#define CR_GMAC_RXMPCR     	(0x168 | CR_GMAC_BASE)
#define CR_GMAC_RXRPCR     	(0x16c | CR_GMAC_BASE)
#define CR_GMAC_RXLPCR     	(0x170 | CR_GMAC_BASE)
#define CR_GMAC_RXCECR     	(0x174 | CR_GMAC_BASE)
#define CR_GMAC_RXAECR     	(0x178 | CR_GMAC_BASE)
#define CR_GMAC_RXRECR     	(0x17c | CR_GMAC_BASE)
#define CR_GMAC_RXPPCR     	(0x180 | CR_GMAC_BASE)
#define CR_GMAC_RXBCR     	(0x184 | CR_GMAC_BASE)
#define CR_GMAC_TXPUCR     	(0x188 | CR_GMAC_BASE)
#define CR_GMAC_GDRPCR     	(0x18c | CR_GMAC_BASE)
#define CR_GMAC_GRPPCR     	(0x190 | CR_GMAC_BASE)
#define CR_GMAC_GESR     	(0x194 | CR_GMAC_BASE)
#define CR_GMAC_EEE     	(0x198 | CR_GMAC_BASE)
#define CR_GMAC_GNFER     	(0x19c | CR_GMAC_BASE)

/********************************
 * Giga Switch Module Registers *
 ********************************/

#if (0)	// move to tc3162.h
// yh, rt62806: reg. address define
#define RAETH_GSW_REG_READ		0x89F1
#define RAETH_GSW_REG_WRITE		0x89F2
#define RAETH_REG_READ			0x89F3
#define RAETH_REG_WRITE			0x89F4
#define RAETH_GSW_PHY_READ		0x89F5
#define RAETH_GSW_PHY_WRITE		0x89F6

#define RAETH_GSWEXT_REG_READ		0x89F7 //MTK120625 ///YM
#define RAETH_GSWEXT_REG_WRITE		0x89F8 //MTK120625 ///YM
#define RAETH_GSWEXT_PHY_READ		0x89F9 //MTK120625 ///YM
#define RAETH_GSWEXT_PHY_WRITE		0x89FA //MTK120625 ///YM

#ifdef TCSUPPORT_MT7530_SWITCH_API
#define RAETH_GSW_CTLAPI		0x89FB
#endif
#endif

#define MIB_BASE_REG 			0x4000
#define MIB_PORT_OFFSET 		0x0100
#define GSW_BASE     			(0x00)

#define MIB_TX_DROP_REG     	(MIB_BASE_REG + 0x0000)
#define MIB_TX_CRC_REG			(MIB_BASE_REG + 0x0004)
#define MIB_TX_UCAST_REG 		(MIB_BASE_REG + 0x0008)
#define MIB_TX_MCAST_REG 		(MIB_BASE_REG + 0x000c)
#define MIB_TX_BCAST_REG 		(MIB_BASE_REG + 0x0010)
#define MIB_TX_COL_REG 			(MIB_BASE_REG + 0x0014)
#define MIB_TX_SCOL_REG 		(MIB_BASE_REG + 0x0018)
#define MIB_TX_MCOL_REG 		(MIB_BASE_REG + 0x001c)
#define MIB_TX_DEFER_REG 		(MIB_BASE_REG + 0x0020)
#define MIB_TX_LCOL_REG 		(MIB_BASE_REG + 0x0024)
#define MIB_TX_XCOL_REG 		(MIB_BASE_REG + 0x0028)
#define MIB_TX_PAUSE_REG 		(MIB_BASE_REG + 0x002c)
#define MIB_TX_PKT_64_CNT		(MIB_BASE_REG + 0x0030)
#define MIB_TX_PKT_65TO127_CNT	(MIB_BASE_REG + 0x0034)
#define MIB_TX_PKT_128TO255_CNT	(MIB_BASE_REG + 0x0038)
#define MIB_TX_PKT_256TO511_CNT (MIB_BASE_REG + 0x003c)
#define MIB_TX_PKT_512TO1023_CNT (MIB_BASE_REG + 0x0040)
#define MIB_TX_PKT_1024TOMAX_CNT (MIB_BASE_REG + 0x0044)
#define MIB_TX_OCT_CNT_L		(MIB_BASE_REG + 0x0048)
#define MIB_TX_OCT_CNT_H		(MIB_BASE_REG + 0x004c)

#define MIB_RX_DROP_REG 		(MIB_BASE_REG + 0x0060)
#define MIB_RX_FILTER_REG 		(MIB_BASE_REG + 0x0064)
#define MIB_RX_UCAST_REG 		(MIB_BASE_REG + 0x0068)
#define MIB_RX_MCAST_REG 		(MIB_BASE_REG + 0x006c)
#define MIB_RX_BCAST_REG 		(MIB_BASE_REG + 0x0070)

#define MIB_RX_ALIGN_ERR_REG 	(MIB_BASE_REG + 0x0074)
#define MIB_RX_FCS_ERR_REG 		(MIB_BASE_REG + 0x0078)
#define MIB_RX_UNDERSIZE_REG 	(MIB_BASE_REG + 0x007c)
#define MIB_RX_FRAG_ERR_REG 	(MIB_BASE_REG + 0x0080)
#define MIB_RX_OVERSIZE_REG 	(MIB_BASE_REG + 0x0084)
#define MIB_RX_JABB_ERR_REG 	(MIB_BASE_REG + 0x0088)
#define MIB_RX_PAUSE_REG 		(MIB_BASE_REG + 0x008c)

#define MIB_RX_PKT_64_CNT 		(MIB_BASE_REG + 0x0090)
#define MIB_RX_PKT_65TO127_CNT 	(MIB_BASE_REG + 0x0094)
#define MIB_RX_PKT_128TO255_CNT (MIB_BASE_REG + 0x0098)
#define MIB_RX_PKT_256TO511_CNT (MIB_BASE_REG + 0x009c)
#define MIB_RX_PKT_512TO1023_CNT (MIB_BASE_REG + 0x00a0)
#define MIB_RX_PKT_1024TOMAX_CNT (MIB_BASE_REG + 0x00a4)

#define MIB_RX_OCT_CNT_L 		(MIB_BASE_REG + 0x00a8)
#define MIB_RX_OCT_CNT_H 		(MIB_BASE_REG + 0x00ac)

#define MIB_RX_CTRL_CNT			(MIB_BASE_REG + 0x00b0)
#define MIB_RX_INGRESS_DROP_CNT (MIB_BASE_REG + 0x00b4)
#define MIB_RX_ARL_DROP_CNT		(MIB_BASE_REG + 0x00b8)

//#define MIB_RX_CTRL_DROP_CNT 0x
//#define MIB_RX_INGR_DROP_CNT 0x
//#define MIB_RX_ARL_DROP_CNT 0x

#define MIB_AE0_CNT				(MIB_BASE_REG + 0xF00)
#define MIB_AE1_CNT 			(MIB_BASE_REG + 0xF04)
#define MIB_AE2_CNT 			(MIB_BASE_REG + 0xF08)
#define MIB_AE3_CNT 			(MIB_BASE_REG + 0xF0c)
#define MIB_AE4_CNT 			(MIB_BASE_REG + 0xF10)
#define MIB_AE5_CNT 			(MIB_BASE_REG + 0xF14)
#define MIB_AE6_CNT 			(MIB_BASE_REG + 0xF18)
#define MIB_AE7_CNT 			(MIB_BASE_REG + 0xF1c)

#if 0
#define GSW_BASE     		0xBFB58000
#define GSW_ARL_BASE     	(GSW_BASE + 0x0000)
#define GSW_BMU_BASE     	(GSW_BASE + 0x1000)
#define GSW_PORT_BASE     	(GSW_BASE + 0x2000)
#define GSW_MAC_BASE     	(GSW_BASE + 0x3000)
#define GSW_MIB_BASE     	(GSW_BASE + 0x4000)
#define GSW_CFG_BASE     	(GSW_BASE + 0x7000)

#define GSW_MFC     		(GSW_ARL_BASE + 0x10)

#define GSW_PMCR(n)     	(GSW_MAC_BASE + (n)*0x100)
#define GSW_PMSR(n)     	(GSW_MAC_BASE + (n)*0x100 + 0x08)
#define GSW_SMACCR0     	(GSW_MAC_BASE + 0xe4)
#define GSW_SMACCR1     	(GSW_MAC_BASE + 0xe8)

#define GSW_ESR(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x00)
#define GSW_INTS(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x04)
#define GSW_TGPC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x10)
#define GSW_TBOC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x14)
#define GSW_TGOC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x18)
#define GSW_TEPC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x1C)
#define GSW_RGPC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x20)
#define GSW_RBOC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x24)
#define GSW_RGOC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x28)
#define GSW_REPC1(n)   		(GSW_MIB_BASE + (n)*0x100 + 0x2C)
#define GSW_REPC2(n)   		(GSW_MIB_BASE + (n)*0x100 + 0x30)
#define GSW_MIBCNTEN  		(GSW_MIB_BASE + 0x800)
#define GSW_AECNT1  		(GSW_MIB_BASE + 0x804)
#define GSW_AECNT2  		(GSW_MIB_BASE + 0x808)

#define GSW_CFG_PPSC     	(GSW_CFG_BASE + 0x0)
#define GSW_CFG_PIAC     	(GSW_CFG_BASE + 0x4)
#endif

/***************************************
 * Ethernet Module Register Definition *
 ***************************************/

/* CR_GMAC_GCTR */
#define GCTR_TX_DMA_EN				(1<<0)
#define GCTR_RX_DMA_EN				(1<<1)
#define GCTR_SW_RESET				(1<<2)
#define GCTR_LOOP_EN				(1<<3)
#define GCTR_RX_CRC_DIS				(1<<4)
#define GCTR_TX_EN				(1<<5)
#define GCTR_HW_RESET				(1<<6)
#define GCTR_TX_PADDING				(1<<7)
#define GCTR_RX_EN				(1<<8)
#define GCTR_DROP_COL				(1<<9)
#define GCTR_RX_RUNT				(1<<10)
#define GCTR_RX_LONG				(1<<11)
#define GCTR_RX_ALL				(1<<12)
#define GCTR_TX_FCS				(1<<14)
#define GCTR_FULL_DUPLEX			(1<<15)
#define GCTR_RX_BCAST				(1<<16)
#define GCTR_RX_MCAST				(1<<17)
#define GCTR_RX_UNTAG				(1<<19)
#define GCTR_BACKOFF_DIS			(1<<29)
#define GCTR_RX_MAX_LEN				(3<<30)
#define GCTR_RX_MAX_LEN_SHIFT			(30)
#define GCTR_RX_MAX_LEN_1522			(0)
#define GCTR_RX_MAX_LEN_1536			(1)
#define GCTR_RX_MAX_LEN_1552			(2)
#define GCTR_RX_MAX_LEN_1792			(3)

/* CR_GMAC_GIMR or CR_GMAC_GISR */
#define GIMR_TX_DONE				(1<<0)
#define GIMR_RX_DONE				(1<<1)
#define GIMR_RX_FIFO_OVR			(1<<2)
#define GIMR_DMA_ERR				(1<<3)
#define GIMR_TX_OWNER_ERR			(1<<4)
#define GIMR_RX_OWNER_ERR			(1<<5)
#define GIMR_RX_RING_FULL			(1<<6)
#define GIMR_PHY_INT				(1<<7)

/* CR_GMAC_GSPR */
#define GSPR_FC_EN				(1<<0)
#define GSPR_CFG_SPD_100M			(1<<1)
#define GSPR_MAC_SPD_GIGA			(1<<2)
#define GSPR_COL_ABT_DIS			(1<<3)
#define GSPR_MTCC_LMT_SHIFT			(4)
#define GSPR_MTCC_LMT				(0xf<<GSPR_MTCC_LMT_SHIFT)
#define GSPR_CFG_CR_IFG_SHIFT			(8)
#define GSPR_CFG_CR_IFG				(0xf<<GSPR_CFG_CR_IFG_SHIFT)
#define GSPR_ADJ_IFG_SHIFT			(12)
#define GSPR_ADJ_IFG				(0xf<<GSPR_ADJ_IFG_SHIFT)

/* CR_GMAC_GSGCR */
#define GSGCR_SG_EN				(1<<31)
#define GSGCR_SG_PKT_LEN_MASK			(0x7ff)

/* CR_GMAC_PHYOR */
#define PHYOR_DATA				(0xffff<<0)
#define PHYOR_REG_SHIFT				16
#define PHYOR_ADDR_SHIFT			21
#define PHYOR_WRITE				(1<<26)
#define PHYOR_START				(1<<27)
#define PHYOR_ERR				(1<<28)

#define NEW_PHYOR_DATA				(0xffff<<0)
#define NEW_PHYOR_REG_SHIFT			16
#define NEW_PHYOR_ADDR_SHIFT			21
#define NEW_PHYOR_OP_SHIFT			26
#define NEW_PHYOR_OP_WR				(1)
#define NEW_PHYOR_OP_RD				(2)
#define NEW_PHYOR_OP_EEE_FA			(0)/*Filed address*/
#define NEW_PHYOR_OP_EEE_WR			(1)
#define NEW_PHYOR_OP_EEE_POST_RD		(2)
#define NEW_PHYOR_OP_EEE_RD			(3)
#define NEW_PHYOR_ST_SHIFT			28
#define NEW_PHYOR_ST_IEEE_NEW			(0)
#define NEW_PHYOR_ST_IEEE_OLD			(1)
#define NEW_PHYOR_ERR				(1<<30)
#define NEW_PHYOR_BUSY				(1<<31)

/* CR_GMAC_GPRCTR */
#define GPRCTR_RX_PKT_DONE_SHIFT		(16)
#define GPRCTR_TX_PKT_DONE_SHIFT		(0)

/* CR_GMAC_GNFER */
#define GNFER_HIGH_SPD_DROP				(1<<13)
#define GNFER_NAT_EN					(1<<12)
#define GNFER_WRR_NFE_SHIFT				8
#define GNFER_WRR_CPU_SHIFT				4
#define GNFER_TX_CPU_SP					(0)
#define GNFER_TX_NFE_SP					(2)
#define GNFER_TX_WRR					(3)
#define GNFER_TX_MODE					(0xf)

/***************************
 * Mac Transmit Descriptor *
 ***************************/

typedef union {
	struct {
    	uint32 owner      : 1;
    	uint32 reserved   : 12;
    	uint32 txhdr_size : 11;
    	uint32 reserved_1 : 4;
    	uint32 ur_abort   : 1;
    	uint32 txpkt_ur   : 1;
    	uint32 reserved_2 : 2;
	} bits;
	uint32 word;
} tdes0_t;

typedef union {
	struct {
	  	uint32 reserved   : 1;
    	uint32 time_pkt   : 1;
    	uint32 txic       : 1;
    	uint32 ipcs_ins   : 1;
    	uint32 vlan_ins   : 1;
    	uint32 vlan_tag   : 16;
    	uint32 txbuf_size : 11;
	} bits;
	uint32 word;
} tdes1_t;

typedef union {
	uint32 txbuf_addr;
	uint32 word;
} tdes2_t;

typedef union {
	uint32 txhdr_addr;
	uint32 word;
} tdes3_t;

typedef struct macTxDescr_s {
	tdes0_t tdes0;
	tdes1_t tdes1;
	tdes2_t tdes2;
  	tdes3_t tdes3;
} macTxDescr_t;

/**************************
 * Mac Receive Descriptor *
 **************************/

typedef union {
	struct {
    	uint32 owner  	  : 1;
    	uint32 vlan_hit   : 1;
    	uint32 pppoe_pkt  : 1;
    	uint32 tcp_ack    : 1;
    	uint32 ip_err     : 1;
    	uint32 l3id       : 3;
    	uint32 eth_pkt    : 1;
    	uint32 collision  : 1;
    	uint32 runt       : 1;
    	uint32 ftl        : 1;
    	uint32 crc_err    : 1;
    	uint32 rx_err     : 1;
    	uint32 broadcast  : 1;
    	uint32 multicast  : 1;
    	uint32 l4id       : 3;
    	uint32 priority   : 2;
    	uint32 rx_length  : 11;
	} bits;
	uint32 word;
} rdes0_t;

typedef union {
	struct {
		uint32 ip_fragmt  : 1;
    	uint32 ip_option  : 1;
    	uint32 time_pkt   : 1;
    	uint32 rxlen_err  : 1;
    	uint32 align_err  : 1;
    	uint32 vlan_tag   : 16;
		uint32 rx_buf_size: 11;
	} bits;
	uint32 word;
} rdes1_t;

typedef union {
	uint32 rxbuf_addr;
	uint32 word;
} rdes2_t;

typedef union {
	uint32 rxhdr_addr;
	uint32 word;
} rdes3_t;

typedef union {
	uint32 word;
} rdes4_t;
typedef union {
	uint32 word;
} rdes5_t;
typedef union {
	uint32 word;
} rdes6_t;
typedef union {
	uint32 word;
} rdes7_t;

typedef struct macRxDescr_s {
	rdes0_t rdes0;
	rdes1_t rdes1;
	rdes2_t rdes2;
  	rdes3_t rdes3;
	rdes4_t rdes4;
	rdes5_t rdes5;
	rdes6_t rdes6;
	rdes7_t rdes7;
} macRxDescr_t;

/*********************
 * Mac Control Block *
 *********************/

/* ----- MIB-II ----- */
typedef struct macMIB_II_s {
	uint32 inOctets;			/* Receive Octets */
	uint32 inUnicastPkts;		/* Receive Unicast Packets */
	uint32 inMulticastPkts;		/* Receive Multicast Packets */
	uint32 inDiscards;			/* Receive Discard Packets */
	uint32 inErrors;			/* Receive Error Packets */
	uint32 inUnknownProtocols;	/* Receive Unknown Prototol Packets */
	uint32 outOctets;			/* Transmit Octets */
	uint32 outUnicastPkts;		/* Transmit Unicast Packets */
	uint32 outMulticastPkts;	/* Transmit Multicast Packets */
	uint32 outDiscards;			/* Transmit Discard Packets */
	uint32 outErrors;			/* Transmit Error Packets */
} macMIB_II_t;

/* ----- Adapter Statistics ----- */
typedef struct inSiliconStat_s {
	uint32 txJabberTimeCnt;
	uint32 txLossOfCarrierCnt;
	uint32 txNoCarrierCnt;
	uint32 txLateCollisionCnt;
	uint32 txExCollisionCnt;
	uint32 txHeartbeatFailCnt;
	uint32 txCollisionCnt;
	uint32 txExDeferralCnt;
	uint32 txUnderRunCnt;

	uint32 rxAlignErr;
	uint32 rxSymbolErr;
	uint32 rxMiiErr;
	uint32 rxCrcErr;
	uint32 rxEtherFrameLengthErr; /* 60 > size(Packet) or 1518 < size(Packet) */
	uint32 rx802p3FrameLengthErr; /* value of length field of 802.3 packet is
	                                 larger than real packet payload */
	uint32 rxDribblingErr;
	uint32 rxRuntErr;
	uint32 rxLongErr;
	uint32 rxCollisionErr;
	uint32 rxOverrunInt;

	uint32 rxPktIPChkSumErr;

	uint32 rxEnQueueNum;          /* Number of packets enqueued in macRxRingproc() */
	uint32 rxDeQueueNum;          /* Number of packets dequeued in macRxToUpperTask() */
	uint32 txEnQueueNum;          /* Number of packets enqueued in macSend() */
  	uint32 txDeQueueNum;          /* Number of packets dequeued in macTxRingproc() */
} inSiliconStat_t;


/* ----- Ethernet Link Profile ----- */
typedef struct macPhyLinkProfile_s {
	uint32 linkSpeed;							/* 10Mbps or 100Mbps */
	uint32 duplexMode;							/* Half/Full Duplex Mode */
	uint32 enetMode;
	uint32 ANCompFlag;							/* auto_negotiation complete Flag */
	uint32 PollCount;							/* auto_negotiation polling check count */
} macPhyLinkProfile_t;

/* ----- Ethernet private memory pool ----- */
typedef struct macRxMemPool_s {
	macRxDescr_t rxDescpBuf[MAC_RXDESCP_NO];
} macRxMemPool_t;

typedef struct macTxMemPool_s {
	macTxDescr_t txDescpBuf[TX_QUEUE_NUM][MAC_TXDESCP_NO];
} macTxMemPool_t;

/* ----- Statistics for GMAC ----- */
typedef struct macStat_s {
	macMIB_II_t MIB_II;	/* MIB-II */
	inSiliconStat_t inSilicon;
} macStat_t;

/* ----- Adapter Card Table ------ */
typedef struct macAdapter_s {
  	uint8  macAddr[6];  /* MAC-Address */

  	macPhyLinkProfile_t *macPhyLinkProfile_p;

  	macRxMemPool_t      *macRxMemPool_p;
  	dma_addr_t        	macRxMemPool_phys_p;
  	macTxMemPool_t      *macTxMemPool_p;
  	dma_addr_t        	macTxMemPool_phys_p;

  	macStat_t           macStat;
  	uint8 statisticOn;  /* Flag to record statistics or not */

  	uint32 resetNum;    /* Number of Reset the LAN Cont. */
  	uint32 enetPhyAddr;
  	uint8  enetPhyId;

  	uint32 txDescrRingBaseVAddr[TX_QUEUE_NUM];  /* Transmit Descr Ring Virtual Address */
  	uint32 rxDescrRingBaseVAddr;  			  /* Receive Descr Ring Virtual Address */
  	uint32 txCurrentDescp[TX_QUEUE_NUM];        /* index to current tx descriptor */
  	uint32 txUnReleasedDescp[TX_QUEUE_NUM];     /* index to the unreleased descp of Tx */
  	uint32 txUnReleasedBufCnt[TX_QUEUE_NUM];    /* Unreleased buffer cnt hold by Tx	*/
  	uint32 rxCurrentDescp;        			  /* index to current rx descriptor */

  	uint32 rxRingSize;  /* Receive Descr Ring Size */
  	uint32 txRingSize;  /* Receive Descr Ring Size */
  	uint32 rxDescrSize; /* The size of a Rx descr entry in the descr ring */
  	uint32 txDescrSize; /* The size of a Tx descr entry in the descr ring */

	struct sk_buff *txskbs[TX_QUEUE_NUM][MAC_TXDESCP_NO];
	struct sk_buff *rxskbs[MAC_RXDESCP_NO];
#ifdef TC3262_GMAC_SG_MODE
	struct sk_buff *sg_rxskbs[MAC_RXDESCP_NO];
#endif

	struct mii_if_info mii_if;

  	spinlock_t lock;
  	struct net_device_stats stats;
} macAdapter_t;

typedef struct phyDeviceList_s {
	uint16 companyId;
	char vendorName[30];
} phyDeviceList_t;


/* frank added for rt62806 */
typedef struct _gsw_reg {
	__u32 off;
	__u32 val;
} gsw_reg;


/************************************************************************
*        F U N C T I O N   P R O T O T Y P E S
*************************************************************************
*/

#endif /* _TC3262GMAC_H */

