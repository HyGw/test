/***************************************************************
Copyright Statement:

This software/firmware and related documentation (��EcoNet Software��) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (��EcoNet��) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (��ECONET SOFTWARE��) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN ��AS IS�� 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVER��S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVER��S SPECIFICATION OR CONFORMING TO A PARTICULAR 
STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND 
ECONET'S ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE ECONET 
SOFTWARE RELEASED HEREUNDER SHALL BE, AT ECONET'S SOLE OPTION, TO 
REVISE OR REPLACE THE ECONET SOFTWARE AT ISSUE OR REFUND ANY SOFTWARE 
LICENSE FEES OR SERVICE CHARGES PAID BY RECEIVER TO ECONET FOR SUCH 
ECONET SOFTWARE.
***************************************************************/
#ifndef _ETH_EN7512_H
#define _ETH_EN7512_H


/************************************************************************
*               I N C L U D E S
*************************************************************************
*/

/************************************************************************
*               D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
/*****************************
* Frame Engine Registers *
*****************************/
#define FE_BASE     		0xBFB50000

#define CDMA_BASE     		(FE_BASE + 0x400)
#define CDMA_CSG_CFG        (CDMA_BASE + 0x00)

#define GDMA1_BASE     		(FE_BASE + 0x0500)
#define GDMA1_FWD_CFG       (GDMA1_BASE + 0x00)
#define GDMA1_LEN_CFG       (GDMA1_BASE + 0x14)
#define GDMA1_VLAN_GEN      (GDMA1_BASE + 0x10)


/********************************
* Giga Switch Module Registers *
********************************/
    
#define GSW_BASE     		0xBFB58000
#define GSW_ARL_BASE     	(GSW_BASE + 0x0000)
#define GSW_BMU_BASE     	(GSW_BASE + 0x1000)
#define GSW_PORT_BASE     	(GSW_BASE + 0x2000)
#define GSW_MAC_BASE     	(GSW_BASE + 0x3000)
#define GSW_MIB_BASE     	(GSW_BASE + 0x4000)
#define GSW_CFG_BASE     	(GSW_BASE + 0x7000)
    
#define GSW_MFC     		(GSW_ARL_BASE + 0x10)
#define GSW_VLAN_REG		(GSW_ARL_BASE + 0x94)
#define GSW_ATA1_REG		(GSW_ARL_BASE + 0x74)
#define GSW_ATC_REG			(GSW_ARL_BASE + 0x80)
#define GSW_IMC     		(GSW_ARL_BASE + 0x1c)
#define IMC_IGMP_RPT_FW_SHIFT			(12)

/*ACL reg*/
#define VTCR                (GSW_BASE + 0x90)
#define VAWD1               (GSW_BASE + 0x94)
#define VAWD2               (GSW_BASE + 0x98)

#define GSW_PORT_CTL(n)		(GSW_BASE + (n)*0x100 + 0x2004)

#define GSW_GERLC           (GSW_BMU_BASE + 0xE0)
#define GSW_FC_CTL          (GSW_BMU_BASE + 0xFE0)
#define GSW_GIRLC           (GSW_BMU_BASE + 0xFF0)

#define GSW_PCR(n)          (GSW_PORT_BASE + (n)*0x100 + 0x04)
#define GSW_PSC(n)     		(GSW_PORT_BASE + (n)*0x100 + 0x0C)
#define GSW_PVC(n)     	    (GSW_PORT_BASE + (n)*0x100 + 0x10)
#define GSW_PPBV1(n)     	(GSW_PORT_BASE + (n)*0x100 + 0x14)

    
#define GSW_PMCR(n)     	(GSW_MAC_BASE + (n)*0x100)
#define LINK_UP             0x1
#define LINK_DOWN           0xFFFFFFFE

#define GSW_PMSR(n)     	(GSW_MAC_BASE + (n)*0x100 + 0x08)
#define GSW_PINT_EN(n)     	(GSW_MAC_BASE + (n)*0x100 + 0x10)
#define GSW_SMACCR0     	(GSW_MAC_BASE + 0xe4)
#define GSW_SMACCR1     	(GSW_MAC_BASE + 0xe8)
#define GSW_CKGCR		    (GSW_MAC_BASE + 0xf0)
    
#define GSW_TX_DROC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x00)
#define GSW_TX_CRC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x04)
#define GSW_TX_UNIC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x08)
#define GSW_TX_MULC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x0c)
#define GSW_TX_BROC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x10)
#define GSW_TX_COLC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x14)
#define GSW_TX_SCOLC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x18)
#define GSW_TX_MCOLC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x1c)
#define GSW_TX_DEFC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x20)
#define GSW_TX_LCOLC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x24)
#define GSW_TX_ECOLC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x28)
#define GSW_TX_PAUC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x2c)
#define GSW_TX_OCL(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x48)
#define GSW_TX_OCH(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x4c)
    
#define GSW_RX_DROC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x60)
#define GSW_RX_FILC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x64)
#define GSW_RX_UNIC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x68)
#define GSW_RX_MULC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x6c)
#define GSW_RX_BROC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x70)
#define GSW_RX_ALIGE(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x74)
#define GSW_RX_CRC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x78)
#define GSW_RX_RUNT(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x7c)
#define GSW_RX_FRGE(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x80)
#define GSW_RX_LONG(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x84)
#define GSW_RX_JABE(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x88)
#define GSW_RX_PAUC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x8c)
#define GSW_RX_OCL(n)    		(GSW_MIB_BASE + (n)*0x100 + 0xa8)
#define GSW_RX_OCH(n)    		(GSW_MIB_BASE + (n)*0x100 + 0xac)
#define GSW_RX_INGC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0xb4)
#define GSW_RX_ARLC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0xb8)
    
#define EXT_GSW_TX_DROC(n)    		(0x4000 + (n)*0x100)
#define EXT_GSW_TX_CRC(n)    		(0x4004 + (n)*0x100)
#define EXT_GSW_TX_UNIC(n)    		(0x4008 + (n)*0x100)
#define EXT_GSW_TX_MULC(n)    		(0x400c + (n)*0x100)
#define EXT_GSW_TX_BROC(n)    		(0x4010 + (n)*0x100)
#define EXT_GSW_TX_COLC(n)    		(0x4014 + (n)*0x100)
#define EXT_GSW_TX_SCOLC(n)    		(0x4018 + (n)*0x100)
#define EXT_GSW_TX_MCOLC(n)    		(0x401c + (n)*0x100)
#define EXT_GSW_TX_DEFC(n)    		(0x4020 + (n)*0x100)
#define EXT_GSW_TX_LCOLC(n)    		(0x4024 + (n)*0x100)
#define EXT_GSW_TX_ECOLC(n)    		(0x4028 + (n)*0x100)
#define EXT_GSW_TX_PAUC(n)    		(0x402c + (n)*0x100)
#define EXT_GSW_TX_OCL(n)    		(0x4048 + (n)*0x100)
#define EXT_GSW_TX_OCH(n)    		(0x404c + (n)*0x100)
    
#define EXT_GSW_RX_DROC(n)    		(0x4060 + (n)*0x100)
#define EXT_GSW_RX_FILC(n)    		(0x4064 + (n)*0x100)
#define EXT_GSW_RX_UNIC(n)    		(0x4068 + (n)*0x100)
#define EXT_GSW_RX_MULC(n)    		(0x406c + (n)*0x100)
#define EXT_GSW_RX_BROC(n)    		(0x4070 + (n)*0x100)
#define EXT_GSW_RX_ALIGE(n)    		(0x4074 + (n)*0x100)
#define EXT_GSW_RX_CRC(n)    		(0x4078 + (n)*0x100)
#define EXT_GSW_RX_RUNT(n)    		(0x407c + (n)*0x100)
#define EXT_GSW_RX_FRGE(n)    		(0x4080 + (n)*0x100)
#define EXT_GSW_RX_LONG(n)    		(0x4084 + (n)*0x100)
#define EXT_GSW_RX_JABE(n)    		(0x4088 + (n)*0x100)
#define EXT_GSW_RX_PAUC(n)    		(0x408c + (n)*0x100)
#define EXT_GSW_RX_OCL(n)    		(0x40a8 + (n)*0x100)
#define EXT_GSW_RX_OCH(n)    		(0x40ac + (n)*0x100)
#define EXT_GSW_RX_INGC(n)    		(0x40b4 + (n)*0x100)
#define EXT_GSW_RX_ARLC(n)    		(0x40b8 + (n)*0x100)
    



#define DEFAULT_TPID  	(0x8100)
    
#define GSW_CFG_PPSC     	(GSW_CFG_BASE + 0x18)
#define GSW_CFG_PIAC     	(GSW_CFG_BASE + 0x1c)
#define GSW_CFG_GPC     	(GSW_CFG_BASE + 0x14)
#define GSW_CFG_CREV        (GSW_CFG_BASE + 0xFFC)
    
#define EXT_GSW_PCR(x)         (0x2000 + (x)*0x100 + 0x04)
#define EXT(x)              (x - GSW_BASE)
#define EXT_GSW_FC_CTL      (0x1FE0)
#define EXT_GSW_CFG     	(0x7000)

/***************************************
 * Ethernet Module Register Definition *
 ***************************************/
    
    /* RSTCTRL2 */
//no EPHY RESET, SET to the same with PDMA_REST
#define EPHY_RST						(1<<1)
#define ESW_RST							(1<<23)
#define FE_RST							(1<<1)//means PDMA RESET
    
    /* FE_VLAN_ID */
#define VLAN_ID1_SHIFT   				(16)
#define VLAN_IDl       					(0xfff<<VLAN_ID1_SHIFT)
#define VLAN_ID0_SHIFT   				(0)
#define VLAN_ID0       					(0xfff<<VLAN_ID0_SHIFT)
    
    /* PDMA_GLO_CFG */
#define RX_2BYTE_OFFSET					(1<<31)
#define CSR_CLKGATE						(1<<30)
#define PDMA_BYTE_SWAP    				(1<<29)
#define PDMA_BIG_ENDIAN    				(1<<7)
#define TX_WB_DDONE       				(1<<6)
#define PDMA_BT_SIZE_SHIFT  			(4)
#define PDMA_BT_SIZE       				(0x3<<PDMA_BT_SIZE_SHIFT)
#define RX_DMA_BUSY       				(1<<3)
#define RX_DMA_EN         				(1<<2)
#define TX_DMA_BUSY       				(1<<1)
#define TX_DMA_EN         				(1<<0)
    
#define PDMA_BT_SIZE_4DW   				(0)
#define PDMA_BT_SIZE_8DW   				(1)
#define PDMA_BT_SIZE_16DW   			(2)
#define PDMA_BT_SIZE_32DW   			(3)
    
    /* PDMA_RST_IDX */
#define RST_DRX_IDX(n)					(1<<(16+(n)))
#define RST_DTX_IDX(n)					(1<<(n))
    
    /* DLY_INT_CFG */
#define TXDLY_INT_EN					(1<<31)
#define TXMAX_PINT_SHIFT				(24)
#define TXMAX_PINT						(0x7f<<TXMAX_PINT_SHIFT)
#define TXMAX_PTIME_SHIFT				(16)
#define TXMAX_PTIME						(0xff<<TXMAX_PTIME_SHIFT)
#define RXDLY_INT_EN					(1<<15)
#define RXMAX_PINT_SHIFT				(8)
#define RXMAX_PINT						(0x7f<<RXMAX_PINT_SHIFT)
#define RXMAX_PTIME_SHIFT				(0)
#define RXMAX_PTIME						(0xff<<RXMAX_PTIME_SHIFT)
    
    /* INT_STATUS or INT_MASK */
#define RX_COHERENT						(1<<31)
#define RX_DLY_INT						(1<<30)
#define TX_COHERENT						(1<<29)
#define TX_DLY_INT						(1<<28)
#define RX_DONE_INT1					(1<<17)
#define RX_DONE_INT0					(1<<16)
#define TX_DONE_INT3					(1<<3)
#define TX_DONE_INT2					(1<<2)
#define TX_DONE_INT1					(1<<1)
#define TX_DONE_INT0					(1<<0)
    
    /* SCH_Q01_CFG */
#define MAX_BLK_SIZE1					(1<<31)
#define MAX_RATE_ULMT1					(1<<30)
#define MAX_WEIGHT1_SHIFT				(28)
#define MAX_WEIGHT1						(0x3<<MAX_WEIGHT1_SHIFT)
#define MIN_RATE_RATIO1_SHIFT			(26)
#define MIN_RATE_RATIO1					(0x3<<MIN_RATE_RATIO1_SHIFT)
#define MAX_RATE1_SHIFT					(16)
#define MAX_RATE1						(0x3ff<<MAX_RATE_RATIO1_SHIFT)
#define MAX_BLK_SIZE0					(1<<15)
#define MAX_RATE_ULMT0					(1<<14)
#define MAX_WEIGHT0_SHIFT				(12)
#define MAX_WEIGHT0						(0x3<<MAX_WEIGHT0_SHIFT)
#define MIN_RATE_RATIO0_SHIFT			(10)
#define MIN_RATE_RATIO0					(0x3<<MIN_RATE_RATIO0_SHIFT)
#define MAX_RATE0_SHIFT					(0)
#define MAX_RATE0						(0x3ff<<MAX_RATE0_SHIFT)
    
    /* SCH_Q23_CFG */
#define MAX_BLK_SIZE3					(1<<31)
#define MAX_RATE_ULMT3					(1<<30)
#define MAX_WEIGHT3_SHIFT				(28)
#define MAX_WEIGHT3						(0x3<<MAX_WEIGHT3_SHIFT)
#define MIN_RATE_RATIO3_SHIFT			(26)
#define MIN_RATE_RATIO3					(0x3<<MIN_RATE_RATIO3_SHIFT)
#define MAX_RATE3_SHIFT					(16)
#define MAX_RATE3						(0x3ff<<MAX_RATE_RATIO3_SHIFT)
#define MAX_BLK_SIZE2					(1<<15)
#define MAX_RATE_ULMT2					(1<<14)
#define MAX_WEIGHT2_SHIFT				(12)
#define MAX_WEIGHT2						(0x3<<MAX_WEIGHT2_SHIFT)
#define MIN_RATE_RATIO2_SHIFT			(10)
#define MIN_RATE_RATIO2					(0x3<<MIN_RATE_RATIO2_SHIFT)
#define MAX_RATE2_SHIFT					(0)
#define MAX_RATE2						(0x3ff<<MAX_RATE2_SHIFT)
    
#define MAX_WEIGHT_1023 				(0)
#define MAX_WEIGHT_2047 				(1)
#define MAX_WEIGHT_4095 				(2)
#define MAX_WEIGHT_8191 				(3)
    
#define MIN_RATIO0 						(0)
#define MIN_RATIO1 						(1)
#define MIN_RATIO2 						(2)
#define MIN_RATIO3 						(3)
    
    /* GDMA1_FWD_CFG or GDMA2_FWD_CFG */
#define GDM_JMB_LEN_SHIFT				(28)
#define GDM_JMB_LEN						(0xf<<GDM_JMB_LEN_SHIFT)
#define GDM_20US_TICK_SLT				(1<<25)
    
#define GDM_INSV_EN						(1<<26)
#define GDM_UNTAG_EN					(1<<25)
#define GDM_STAG_EN						(1<<24)
#define GDM_ICS_EN						(1<<22)
#define GDM_TCS_EN						(1<<21)
#define GDM_UCS_EN						(1<<20)
#define GDM_DROP_256B					(1<<19)
#define GDM_DISPAD						(1<<18)
#define GDM_DISCRC						(1<<17)
#define GDM_STRPCRC						(1<<16)
#define GDM_UFRC_P_SHIFT				(12)
#define GDM_UFRC_P						(0xf<<GDM_UFRC_P_SHIFT)
#define GDM_BFRC_P_SHIFT				(8)
#define GDM_BFRC_P						(0xf<<GDM_BFRC_P_SHIFT)
#define GDM_MFRC_P_SHIFT				(4)
#define GDM_MFRC_P						(0xf<<GDM_MFRC_P_SHIFT)
#define GDM_OFRC_P_SHIFT				(0)
#define GDM_OFRC_P						(0xf<<GDM_OFRC_P_SHIFT)
    
#define GDM2_UNDERRUN_RETRY				(1<<27)
#define GDM2_DROP_256B					(1<<26)
#define GDM2_DROP_LONG					(1<<25)
#define GDM2_DROP_RUNT					(1<<24)
#define GDM2_DROP_CRC_ERR				(1<<23)
#define GDM2_JMB_EN						(1<<19)
    
    
    /* define GDMA port */
#define GDM_P_PDMA						(0x0)
#define GDM_P_GDMA1						(0x1)
#define GDM_P_GDMA2						(0x2)
#define GDM_P_PPE						(0x4)
#define GDM_P_QDMA						(0x5)
#define GDM_P_DISCARD					(0x7)
#define GDM_P_CPU						GDM_P_PDMA
    
    
    /* GDMA1_SCH_CFG or GDMA2_SCH_CFG */
#define GDM_SCH_MOD_SHIFT				(24)
#define GDM_SCH_MOD						(0x3<<GDM_SCH_MOD_SHIFT)
#define GDM_WT_Q3_SHIFT					(12)
#define GDM_WT_Q3						(0x7<<GDM_WT_Q3_SHIFT)
#define GDM_WT_Q2_SHIFT					(8)
#define GDM_WT_Q2						(0x7<<GDM_WT_Q2_SHIFT)
#define GDM_WT_Q1_SHIFT					(4)
#define GDM_WT_Q1						(0x7<<GDM_WT_Q1_SHIFT)
#define GDM_WT_Q0_SHIFT					(0)
#define GDM_WT_Q0						(0x7<<GDM_WT_Q0_SHIFT)
    
#define GDM_SCH_MOD_WRR					(0)
#define GDM_SCH_MOD_SP					(1)
    
#define GDM_WT(n)						((n>=8) ? 7 : ((n)-1)&0x7)
    
    /* CDMA_CSG_CFG */
#define INS_VLAN_SHIFT					(16)
#define INS_VLAN						(0xffff<<INS_VLAN_SHIFT)
#define CDM_STAG_EN							(1<<0)
    
    /* GSW_MFC */
#define MFC_BC_FFP_SHIFT				(24)
#define MFC_BC_FFP						(0xff<<MFC_BC_FFP_SHIFT)
#define MFC_UNM_FFP_SHIFT				(16)
#define MFC_UNM_FFP						(0xff<<MFC_UNM_FFP_SHIFT)
#define MFC_UNU_FFP_SHIFT				(8)
#define MFC_UNU_FFP						(0xff<<MFC_UNU_FFP_SHIFT)
#define MFC_CPU_EN						(1<<7)
#define MFC_CPU_PORT_SHIFT				(4)
#define MFC_CPU_PORT					(0x7<<MFC_CPU_PORT_SHIFT)
#define MFC_MIRROR_EN					(1<<3)
#define MFC_MIRROR_PORT_SHIFT			(0)
#define MFC_MIRROT_PORT					(0x7<<MFC_MIRROR_PORT_SHIFT)
    
    /* GSW_PMCR */
#define IPG_CFG_PN_SHIFT				(18)
#define IPG_CFG_PN						(0x3<<IPG_CFG_PN_SHIFT)
#define EXT_PHY_PN						(1<<17)
#define MAC_MODE_PN						(1<<16)
#define FORCE_MODE_PN					(1<<15)
#define MAC_TX_EN_PN					(1<<14)
#define MAC_RX_EN_PN					(1<<13)
#define RGMII_MODE_PN					(1<<12)
#define BKOFF_EN_PN						(1<<9)
#define BACKPR_EN_PN					(1<<8)
#define ENABLE_EEE1G_PN					(1<<7)
#define ENABLE_EEE100_PN				(1<<6)
#define ENABLE_RX_FC_PN					(1<<5)
#define ENABLE_TX_FC_PN					(1<<4)
#define FORCE_SPD_PN_SHIFT				(2)
#define FORCE_SPD_PN					(0x3<<FORCE_SPD_PN_SHIFT)
#define FORCE_DPX_PN					(1<<1)
#define FORCE_LNK_PN					(1<<0)
    
#define IPG_CFG_NORMAL					(0)
#define IPG_CFG_SHORT					(1)
#define IPG_CFG_64BITS                                  (0x2)
    
#define PN_SPEED_10M					(0)
#define PN_SPEED_100M					(1)
#define PN_SPEED_1000M					(2)
    
    /* GSW_PMSR */
#define EEE1G_STS						(1<<7)
#define EEE100_STS						(1<<6)
#define RX_FC_STS						(1<<5)
#define TX_FC_STS						(1<<4)
#define MAC_SPD_STS_SHIFT				(2)
#define MAC_SPD_STS						(0x3<<MAC_SPD_STS_SHIFT)
#define MAC_DPX_STS						(1<<1)
#define MAC_LINK_STS					(1<<0)
    
    
    /* GSW_CFG_PPSC */
#define PHY_AP_EN_SHIFT					(1<<24)
#define PHY_AP_EN						(0x7f<<PHY_END_ADDR_SHIFT)
    
#define PHY_EEE_EN_SHIFT				(1<<16)
#define PHY_EEE_EN						(0x7f<<PHY_END_ADDR_SHIFT)
    
#define PHY_PRE_EN						(1<<15)
#define PHY_END_ADDR_SHIFT				(8)
#define PHY_END_ADDR					(0x1f<<PHY_END_ADDR_SHIFT)
#define PHY_MDC_CFG_SHIFT				(1<<6)
#define PHY_MDC_CFG						(0x3<<PHY_MDC_CFG_SHIFT)
#define PHY_ST_ADDR_SHIFT				(0)
#define PHY_ST_ADDR						(0x1f<<PHY_ST_ADDR_SHIFT)
    
    /* GSW_CFG_PIAC */
#define PHY_ACS_ST						(1<<31)
#define MDIO_REG_ADDR_SHIFT				(25)
#define MDIO_REG_ADDR					(0x1f<<MDIO_REG_ADDR_SHIFT)
#define MDIO_PHY_ADDR_SHIFT				(20)
#define MDIO_PHY_ADDR					(0x1f<<MDIO_PHY_ADDR_SHIFT)
#define MDIO_CMD_SHIFT					(18)
#define MDIO_CMD						(0x3<<MDIO_CMD_SHIFT)
#define MDIO_ST_SHIFT					(16)
#define MDIO_ST							(0x3<<MDIO_ST_SHIFT)
#define MDIO_RW_DATA_SHIFT				(0)
#define MDIO_RW_DATA					(0xffff<<MDIO_RW_DATA_SHIFT)
    
#define PHY_ACS_ST_START				(1)
#define MDIO_CMD_WRITE					(1)
#define MDIO_CMD_READ					(2)
#define MDIO_ST_START					(1)
    
#define MDIO_CL45_CMD_ADDR				(0)
#define MDIO_CL45_CMD_WRITE				(1)
#define MDIO_CL45_CMD_READ				(3)
#define MDIO_CL45_CMD_POSTREAD_INCADDR			(2)
#define MDIO_CL45_ST_START				(0)
    
#define NORMAL_READ					(1<<0)
#define POST_READ					(1<<1)
    
    /* GSW_CFG_GPC */
#define RX_CLK_MODE						(1<<2)
    

//SPORT DEFINE
#define SPORT_QDMA_LAN          0
#define SPORT_GDMA1				1
#define SPORT_GDMA2				2//means wan , like SAR/PTM/xPON
#define SPORT_PPE				4
#define SPORT_QDMA_WAN          5//send to CPU via QDMA
#define SPORT_QDMA_HW			6//send to CPU via QDMA HW
#define SPORT_DISCARD			7
#define SPORT_CPU				DPORT_PDMA


    
//FPORT DEFINE
#define DPORT_PDMA				0
#define DPORT_GDMA1				1
#define DPORT_GDMA2				2//means wan , like SAR/PTM/xPON
#define DPORT_PPE				4
#define DPORT_QDMA				5//send to CPU via QDMA
#define DPORT_QDMA_HW			6//send to CPU via QDMA HW
#define DPORT_DISCARD			7
#define DPORT_CPU				DPORT_PDMA
    
    //PKT_INFO define
#define IPV6_H						(1<<5)
#define IPV4_H						(1<<4)
#define IPV4_H_INV					(1<<3)
#define TU_H						(1<<1)
#define TU_H_C_INV					(1<<0)

/* scatter & gather mode */
#define SG_MAX_PKT_LEN	   		(128)

#ifndef LOOPBACK_SUPPORT
#define MAC_TXDESCP_NO			256		/* Max tx buffer Cnts , default=128 */
#else
#define MAC_TXDESCP_NO			128		/* Max tx buffer Cnts , default=128 */
#endif

#define MAC_RXDESCP_NO_RES		128
#define MAC_RXDESCP_NO_DEFAULT		128

/* Max rx buffer Cnts , default=128 */
#define MAC_RXDESCP_NO			MAC_RXDESCP_NO_DEFAULT + MAC_RXDESCP_NO_RES 	

#ifdef MT7615E
#if (defined(TCSUPPORT_VOIP) || defined(WAN_GPON) || defined(WAN_EPON))
#define MAC_NAPI_WEIGHT			64 
#else  /*TCSUPPORT_VOIP*/
#define MAC_NAPI_WEIGHT			32 
#endif /*TCSUPPORT_VOIP*/

#else
#define MAC_NAPI_WEIGHT			128 
#endif /*MT7615E*/

#define MAC_RECV_THLD			15

#define MAC_RXDESCP_SIZE		16		/* 4 DWords */
#define MAC_TXDESCP_SIZE		16		/* 4 DWords */

#define TX_BUF_RELEASE_THRESHOLD 4  	/* default:4 */

#ifndef LOOPBACK_SUPPORT
#define TX_QUEUE_NUM 			1	//Only need One queue for MT7510
#else
#define TX_QUEUE_NUM 			4

#endif
#define RX_QUEUE_NUM 			1


#define MAC_STATISTIC_ON		1
#define MAC_STATISTIC_OFF		0

#define GMAC_PRIORITY_MASK  	(TX_QUEUE_NUM-1)
#if 0
#ifdef MT7510_DMA_DSCP_CACHE
#ifdef TCSUPPORT_CPU_MT7505
#define TXRXDSP_PADDR_ENABLE_REG	0xbfb00860
#define TXDSP0_PADDR_ENABLE			(1<<0)
#define RXDSP0_PADDR_ENABLE			(1<<1)
#endif
#define TXDSP0_PADDR_START_BASE	0xbfb00864
#define TXDSP0_VADDR_START_BASE	(TXDSP0_PADDR_START_BASE+0x4)
#define TXDSP0_VADDR_END_BASE	(TXDSP0_PADDR_START_BASE+0x8)
#define RXDSP0_PADDR_START_BASE	(TXDSP0_PADDR_START_BASE+0xc)
#define RXDSP0_VADDR_START_BASE	(TXDSP0_PADDR_START_BASE+0x10)
#define RXDSP0_VADDR_END_BASE	(TXDSP0_PADDR_START_BASE+0x14)

#define CACHE_LINE_VADDR_OFFSET 0x8000000 //128M

#define TO_CACHE_LINE_VADDR(x)	((x) & (~(0x1 << 31)))  //Use virtual address > 128M, and clear the 31 bit
#define	CACHE_LINE_SIZE			(32)

#endif
#endif
/************************************************************************
*               M A C R O S
*************************************************************************
*/
#define K0_TO_K1(x)				(((uint32)x) | 0xa0000000)
#define K1_TO_PHY(x)			(((uint32)x) & 0x1fffffff)
#define PHY_TO_K0(x)				(((uint32)x) | 0x80000000)

/************************************************************************
*               D A T A   T Y P E S
*************************************************************************
*/

/************************************************************************
*               D A T A   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*               F U N C T I O N   D E C L A R A T I O N S
                I N L I N E  F U N C T I O N  D E F I N I T I O N S
*************************************************************************
*/

#endif /* _ETH_EN7512_H */















