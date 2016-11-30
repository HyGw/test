/***************************************************************
Copyright Statement:

This software/firmware and related documentation (¡°EcoNet Software¡±) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (¡°EcoNet¡±) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (¡°ECONET SOFTWARE¡±) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN ¡°AS IS¡± 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVER¡¯S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVER¡¯S SPECIFICATION OR CONFORMING TO A PARTICULAR 
STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND 
ECONET'S ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE ECONET 
SOFTWARE RELEASED HEREUNDER SHALL BE, AT ECONET'S SOLE OPTION, TO 
REVISE OR REPLACE THE ECONET SOFTWARE AT ISSUE OR REFUND ANY SOFTWARE 
LICENSE FEES OR SERVICE CHARGES PAID BY RECEIVER TO ECONET FOR SUCH 
ECONET SOFTWARE.
***************************************************************/
#ifndef _FE_en7512_H
#define _FE_en7512_H



/************************************************************************
*               I N C L U D E S
*************************************************************************
*/

/************************************************************************
*               D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
	
/********************************
	 * SCU Module Registers *
********************************/
#define SCU_PROBE_MODE_CFG 0xbfb00080

/*****************************
 * FE Registers *
 *****************************/

#define FE_BASE     		0xBFB50000
#define FE_DMA_GLO_CFG      (FE_BASE + 0x00)
#define FE_RST_GLO          (FE_BASE + 0x04)
#define FE_INT_STATUS       (FE_BASE + 0x08)
#define FE_INT_ENABLE       (FE_BASE + 0x0c)
#define FE_FOE_TS            (FE_BASE + 0x10)
#define FE_IPV6_EXT            (FE_BASE + 0x14)
#define FE_CNT_CFG          (FE_BASE + 0x18)

/********************************
 *        PSE  Register         *
 ********************************/

#define PSE_BASE     		(FE_BASE + 0x0100)
#define PSE_FQFC_CFG        (PSE_BASE + 0x00)
#define PSE_FQFC_CFG_PPELTH_OFFSET	0
#define PSE_FQFC_CFG_PPEIQ_OFFSET		8
#define PSE_FQFC_CFG_FQMAX_OFFSET		16
#define PSE_FQFC_CFG_FQMAX_LENGTH		8
#define PSE_FQFC_CFG_FQCURR_OFFSET	24
#define PSE_IQ_REV_VIP      (PSE_BASE + 0x04)
#define PSE_IQ_VIP_P1_OFFSET		0
#define PSE_IQ_VIP_P2_OFFSET		8
#define PSE_Q_PORT_LENGTH			8
#define PSE_IQ_REV1         (PSE_BASE + 0x08)
#define PSE_IQ_RES1_P0_OFFSET	0
#define PSE_IQ_RES1_P1_OFFSET	8
#define PSE_IQ_RES1_P2_OFFSET	16
#define PSE_IQ_REV2         (PSE_BASE + 0x0c)
#define PSE_IQ_RES2_P4_OFFSET	0
#define PSE_IQ_RES2_P5_OFFSET	8
#define PSE_IQ_RES2_P6_OFFSET	16
#define PSE_IQ_STA1        	(PSE_BASE + 0x10)
#define PSE_IQ_P0_OFFSET	0
#define PSE_IQ_P1_OFFSET	8
#define PSE_IQ_P2_OFFSET	16

#define PSE_IQ_STA2        	(PSE_BASE + 0x14)
#define PSE_IQ_P4_OFFSET	0
#define PSE_IQ_P5_OFFSET	8
#define PSE_IQ_P6_OFFSET	16

#define PSE_OQ_STA1         (PSE_BASE + 0x18)
#define PSE_OQ_P0_OFFSET	0
#define PSE_OQ_P1_OFFSET	8
#define PSE_OQ_P2_OFFSET	16

#define PSE_OQ_STA2        	(PSE_BASE + 0x1c)
#define PSE_OQ_P4_OFFSET	0
#define PSE_OQ_P5_OFFSET	8
#define PSE_OQ_P6_OFFSET	16
#define PSE_MISC_CFG        (PSE_BASE + 0x20)
#define FE_PROBE_L          (PSE_BASE + 0x30)
#define FE_PROBE_H          (PSE_BASE + 0x34)
#define PSE_FQ_MIN          (PSE_BASE + 0x40)
#define PSE_FQ_MIN_CUR_OFFSET	16
#define PSE_FQ_MIN_CUR_LENGTH	9
#define PSE_FQ_MIN_MIN_OFFSET	0
#define PSE_FQ_MIN_MIN_LENGTH	9
#define PSE_IQ_MAX1         (PSE_BASE + 0x50)
#define PSE_IQ_MAX1_P0_OFFSET	0
#define PSE_IQ_MAX1_P1_OFFSET	8
#define PSE_IQ_MAX1_P2_OFFSET	16
#define PSE_IQ_MAX2         (PSE_BASE + 0x54)
#define PSE_IQ_MAX2_P4_OFFSET	0
#define PSE_IQ_MAX2_P5_OFFSET	8
#define PSE_IQ_MAX2_P6_OFFSET	16
#define PSE_DROP_COUNT_0    (PSE_BASE + 0x80)
#define PSE_DROP_COUNT_1    (PSE_BASE + 0x84)
#define PSE_DROP_COUNT_2    (PSE_BASE + 0x88)
#define PSE_DROP_COUNT_4    (PSE_BASE + 0x90)
#define PSE_DROP_COUNT_5    (PSE_BASE + 0x94)

#define PSE_L2LU_KEY0(x)		(FE_BASE + 0x0200 + ((x) << 3))
#define L2LU_KEY0_CVID_OFFSET			0
#define L2LU_KEY0_CVID_LENGTH			12
#define L2LU_KEY0_CPCP_OFFSET			13
#define L2LU_KEY0_CPCP_LENGTH			3
#define L2LU_KEY0_SVID_OFFSET			16
#define L2LU_KEY0_SVID_LENGTH			12
#define L2LU_KEY0_SPCP_OFFSET			29
#define L2LU_KEY0_SPCP_LENGTH			3

#define PSE_L2LU_KEY1(x)		(FE_BASE + 0x0204 + ((x) << 3))
#define L2LU_KEY1_DSCP_MASK_OFFSET	0
#define L2LU_KEY1_DSCP_MASK_LENGTH	8
#define L2LU_KEY1_CVID_EN_OFFSET		8
#define L2LU_KEY1_CVID_EN_LENGTH		1
#define L2LU_KEY1_CPCP_EN_OFFSET		9
#define L2LU_KEY1_CPCP_EN_LENGTH		1
#define L2LU_KEY1_SVID_EN_OFFSET		12
#define L2LU_KEY1_SVID_EN_LENGTH		1
#define L2LU_KEY1_SPCP_EN_OFFSET		13
#define L2LU_KEY1_SPCP_EN_LENGTH		1
#define L2LU_KEY1_DSCP_OFFSET			16
#define L2LU_KEY1_DSCP_LENGTH			8

#define L2BR_CFG			(FE_BASE + 0x280)
#define L2BR_ETYPE_EN			(FE_BASE + 0x284)
#define L2BR_ETYPE_N(x)			(FE_BASE + 0x290 + 4*(x/2))

#define PPE_AC_BASE			(FE_BASE + 0x2000)
#define PPE_AC_BCNT_L(x)		(PPE_AC_BASE + ((x) << 4))
#define PPE_AC_BCNT_H(x)		(PPE_AC_BASE + 0x04 + ((x) << 4))
#define PPE_AC_PCNT(x)			(PPE_AC_BASE + 0x08 + ((x) << 4))
#define PPE_MTR_CNT(x)			(PPE_AC_BASE + 0x0c + ((x) << 4))

/********************************
 *        VIP  Register         *
 ********************************/
#define FE_VIP_EN(x)			(FE_BASE + 0x300 + ((x) << 3))
#define FE_VIP_PATN(x)			(FE_BASE + 0x304 + ((x) << 3))

#define VIP_EN_ENABLE_OFFSET		0
#define VIP_EN_TYPE_OFFSET		1
#define VIP_EN_TYPE_LENGTH		3
#define VIP_EN_SPEN_OFFSET		4
#define VIP_EN_DPEN_OFFSET		5
#define VIP_EN_SWEN_OFFSET		6
#define VIP_EN_CPU_OFFSET			7
#define VIP_PATN_SP_OFFSET		0
#define VIP_PATN_DP_OFFSET		16
#define VIP_PATN_PORT_LENGTH		16

/********************************
 *        CDM1 Register         *
 ********************************/
#define CDMA1_BASE     		(FE_BASE + 0x400)
#define CDMA1_VLAN_CTRL      (CDMA1_BASE + 0x00)
#define CDMA_VLAN_CTRL_SP_OFFSET			0
#define CDMA_VLAN_CTRL_UNTAG_OFFSET		1
#define CDMA_VLAN_CTRL_TPID_OFFSET		16
#define CDMA1_PPP_GEN        (CDMA1_BASE + 0x04)
#define CDMA_PPP_GEN_INS		16
#define CDMA_PPP_GEN_ID		0

#define CDMA1_FWD_CFG        (CDMA1_BASE + 0x08)
#define CDM_FWD_CFG_Q0_OFFSET		30
#define CDM_FWD_CFG_Q1L_OFFSET		28
#define CDM_FWD_CFG_Q1H_OFFSET		26
#define CDM_FWD_CFG_Q_LENGTH			2
#define CDM_FWD_CFG_Q_MASK  		0x3
#define CDM_FWD_CFG_RXMSG_OFFSET	24
#define CDM_FWD_CFG_RXMSG_LENGTH	2
#define CDM_FWD_CFG_ARB_OFFSET		20
#define CDM_FWD_CFG_ARB_LENGTH		2
#define CDM_FWD_CFG_FP_OFFSET		16
#define CDM_FWD_CFG_FP_LENGTH		1
#define CDM_FWD_CFG_PAD_OFFSET		0
#define CDM_FWD_CFG_PAD_LENGTH		1

#define CDMA1_CRSN_QSEL      (CDMA1_BASE + 0x14)

#define CDMA1_CRSN_QSEL_CPU_REASON_16_OFFSET 0
#define CDMA1_CRSN_QSEL_CPU_REASON_QUEUE_MASK 3
#define CDMA1_CRSN_QSEL_CPU_REASON_16_MASK (CDMA1_CRSN_QSEL_CPU_REASON_QUEUE_MASK<<CDMA1_CRSN_QSEL_CPU_REASON_16_OFFSET)
#define CDMA1_CRSN_QSEL_Q1L 2
#define CDMA1_CRSN_QSEL_Q1H 3

#define CDMA1_HWF_CHN_EN     (CDMA1_BASE + 0x0c)
#define CDMA2_CRSN_QSEL0     (CDMA2_BASE + 0x10)
#define CDMA2_CRSN_QSEL1     (CDMA2_BASE + 0x14)

/********************************
 *        GDM1 Register         *
 ********************************/
#define GDMA1_BASE     		(FE_BASE + 0x0500)

#define GDMA1_FWD_CFG       (GDMA1_BASE + 0x00)
#define GDMA1_FWD_CFG_UCDP_OFFSET		0
#define GDMA1_FWD_CFG_MCDP_OFFSET		4
#define GDMA1_FWD_CFG_BCDP_OFFSET		8
#define GDMA1_FWD_CFG_MYDP_OFFSET		12
#define GDMA1_FWD_CFG_DP_LENGTH			4
#define GDMA1_FWD_CFG_STRIP_OFFSET	16
#define GDMA1_FWD_CFG_RUT_OFFSET		17
#define GDMA1_FWD_CFG_BRG_OFFSET		18
#define GDMA1_FWD_CFG_UDP_OFFSET		20
#define GDMA1_FWD_CFG_TCP_OFFSET		21
#define GDMA1_FWD_CFG_IP_OFFSET			22
#define GDMA1_FWD_CFG_ETH_OFFSET		23
#define GDMA1_FWD_CFG_SHORT_OFFSET	24
#define GDMA1_FWD_CFG_LONG_OFFSET		25
#define GDMA1_FWD_CFG_RETRY_OFFSET	27
#define GDMA1_FWD_CFG_STAG_OFFSET		28
#define GDMA1_FWD_CFG_CTAG_OFFSET		29
#define GDMA1_FWD_CFG_DSCP_OFFSET		30
#define GDMA1_FWD_CFG_VIP_OFFSET		31
#define GDMA1_SHRP_CFG      (GDMA1_BASE + 0x04)
#define GDMA_SHRP_CFG_EN_OFFSET		31
#define GDMA_SHRP_CFG_TICK_OFFSET	29
#define GDMA_SHRP_CFG_RATE_OFFSET	8
#define GDMA_SHRP_CFG_RATE_LENGTH	21
#define GDMA_SHRP_CFG_BUCK_OFFSET	1
#define GDMA1_MAC_ADRL      (GDMA1_BASE + 0x08)
#define GDMA1_MAC_ADRH      (GDMA1_BASE + 0x0c)
#define GDMA1_VLAN_CHECK    (GDMA1_BASE + 0x10)
#define GDMA_VLAN_CHECK_SP_OFFSET	0
#define GDMA1_LEN_CFG       (GDMA1_BASE + 0x14)
#define GDMA_LEN_CFG_RUNT_OFFSET	0
#define GDMA_LEN_CFG_LONG_OFFSET	16
#define GDMA_LEN_CFG_LENGTH			14
#define GDMA1_LAN_PCP       (GDMA1_BASE + 0x18)
#define GDMA_PCP_CDM_TX_8P0D_OFFSET  0
#define GDMA_PCP_CDM_TX_7P1D_OFFSET  1
#define GDMA_PCP_CDM_TX_6P2D_OFFSET  2
#define GDMA_PCP_CDM_TX_5P3D_OFFSET  3
#define GDMA_PCP_CDM_RX_8P0D_OFFSET  4
#define GDMA_PCP_CDM_RX_7P1D_OFFSET  5
#define GDMA_PCP_CDM_RX_6P2D_OFFSET  6
#define GDMA_PCP_CDM_RX_5P3D_OFFSET  7
#define GDMA_PCP_GDM_TX_8P0D_OFFSET  8
#define GDMA_PCP_GDM_TX_7P1D_OFFSET  9
#define GDMA_PCP_GDM_TX_6P2D_OFFSET  10
#define GDMA_PCP_GDM_TX_5P3D_OFFSET  11
#define GDMA_PCP_GDM_RX_8P0D_OFFSET  12
#define GDMA_PCP_GDM_RX_7P1D_OFFSET  13
#define GDMA_PCP_GDM_RX_6P2D_OFFSET  14
#define GDMA_PCP_GDM_RX_5P3D_OFFSET  15
#define GDMA1_LPBK_CFG      (GDMA1_BASE + 0x1c)
#define GDMA1_CHN_RLS       (GDMA1_BASE + 0x20)
#define GDMA_CHN_RLS_EN_OFFSET		0
#define GDMA_CHN_RLS_STAT_OFFSET	1
#define GDMA_CHN_RLS_CHN_OFFSET		4     
#define GDMA_CHN_RLS_TIMEOUT			10
#define GDMA1_TXCHN_EN      (GDMA1_BASE + 0x24)
#define GDMA1_RXCHN_EN      (GDMA1_BASE + 0x28)
#define GDMA1_RXCHN_FP      (GDMA1_BASE + 0x2c)
#define FE_CPORT_CFG        (GDMA1_BASE + 0x40)
#define FE_CPORT_CHN_MAP    (GDMA1_BASE + 0x44)
#define FE_CPORT_SHRP_CFG   (GDMA1_BASE + 0x48)
#define GDMA1_MIB_CLER			(GDMA1_BASE + 0xf0)	

#define GDMA1_TX_CHN_VLD     (GDMA1_BASE + 0x70)
#define GDMA1_RX_CHN_VLD     (GDMA1_BASE + 0x74)

#define CDMA1_TX_OK_CNT			(GDMA1_BASE + 0x80)
#define CDMA1_RXCPU_OK_CNT		(GDMA1_BASE + 0x90)
#define CDMA1_RXHWF_OK_CNT		(GDMA1_BASE + 0x94)
#define CDMA1_RXCPU_KA_CNT		(GDMA1_BASE + 0x98)
#define CDMA1_RXCPU_DROP_CNT	(GDMA1_BASE + 0xa0)
#define CDMA1_RXHWF_DROP_CNT	(GDMA1_BASE + 0xa4)		

/* GDMA1 count define */
#define GDMA1_COUNT_BASE 	    (FE_BASE + 0x600)
#define GDMA1_TX_GET_CNT        (GDMA1_COUNT_BASE + 0x00)
#define GDMA1_TX_OK_CNT         (GDMA1_COUNT_BASE + 0x04)
#define GDMA1_TX_DROP_CNT 	    (GDMA1_COUNT_BASE + 0x08)
#define GDMA1_TX_OK_BYTE_CNT    (GDMA1_COUNT_BASE + 0x0c)

#define GDMA1_RX_OK_CNT         (GDMA1_COUNT_BASE + 0x48)
#define GDMA1_RX_FC_DROP_CNT    (GDMA1_COUNT_BASE + 0x4c)
#define GDMA1_RX_RC_DROP_CNT    (GDMA1_COUNT_BASE + 0x50)
#define GDMA1_RX_OVER_DROP_CNT  (GDMA1_COUNT_BASE + 0x54)
#define GDMA1_RX_ERROR_DROP_CNT (GDMA1_COUNT_BASE + 0x58)
#define GDMA1_RX_BYTECNT        (GDMA1_COUNT_BASE + 0x5c)

/********************************
 *        CDM2 Register         *
 ********************************/
#define CDMA2_BASE     		 (FE_BASE + 0x1400)
#define CDMA2_VLAN_CTRL      (CDMA2_BASE + 0x00)
#define CDMA2_PPP_GEN        (CDMA2_BASE + 0x04)
#define CDMA2_FWD_CFG        (CDMA2_BASE + 0x08)
#define CDMA2_HWF_CHN_EN     (CDMA2_BASE + 0x0c)
#define CDMA2_CRSN_QSEL0     (CDMA2_BASE + 0x10)
#define CDMA2_CRSN_QSEL1     (CDMA2_BASE + 0x14)

/********************************
 *        GDM2 Register         *
 ********************************/
#define GDMA2_BASE     		(FE_BASE + 0x1500)
#define GDMA2_FWD_CFG       (GDMA2_BASE + 0x00)
#define GDMA2_TX_FAVOR_OAM_OFFSET		19
#define GDMA2_SHRP_CFG      (GDMA2_BASE + 0x04)
#define GDMA2_MAC_ADRL      (GDMA2_BASE + 0x08)
#define GDMA2_MAC_ADRH      (GDMA2_BASE + 0x0c)
#define GDMA2_VLAN_CHECK    (GDMA2_BASE + 0x10)
#define GDMA2_LEN_CFG    	(GDMA2_BASE + 0x14)
#define GDMA2_WAN_PCP     	(GDMA2_BASE + 0x18)
#define GDMA2_LPBP_CFG      (GDMA2_BASE + 0x1c)
#define GDMA2_CHN_RLS       (GDMA2_BASE + 0x20)
#define GDMA2_TXCHN_EN       (GDMA2_BASE + 0x24)
#define GDMA2_RXCHN_EN       (GDMA2_BASE + 0x28)
#define GDMA2_RXCHN_FP       (GDMA2_BASE + 0x2c)

#define GDMA2_RXUC_SHPR_CFG  (GDMA2_BASE + 0x30)
#define GDMA2_RXBC_SHPR_CFG  (GDMA2_BASE + 0x34)
#define GDMA2_RXMC_SHPR_CFG  (GDMA2_BASE + 0x38)
#define GDMA2_RXOC_SHPR_CFG  (GDMA2_BASE + 0x3c)
#define GDMA2_MIB_CLER			(GDMA2_BASE + 0xf0)	
#define GDMA2_TX_CHN_VLD     (GDMA2_BASE + 0x70)
#define GDMA2_RX_CHN_VLD     (GDMA2_BASE + 0x74)



#define CDMA2_TX_OK_CNT			(GDMA2_BASE + 0x80)
#define CDMA2_RXCPU_OK_CNT		(GDMA2_BASE + 0x90)
#define CDMA2_RXHWF_OK_CNT		(GDMA2_BASE + 0x94)
#define CDMA2_RXCPU_KA_CNT		(GDMA2_BASE + 0x98)
#define CDMA2_RXCPU_DROP_CNT	(GDMA2_BASE + 0xa0)
#define CDMA2_RXHWF_DROP_CNT	(GDMA2_BASE + 0xa4)		





#define GDMA2_COUNT_BASE 	(FE_BASE + 0x1600)
#define GDMA2_TX_GETCNT     (GDMA2_COUNT_BASE + 0x00)
#define GDMA2_TX_OKCNT     	(GDMA2_COUNT_BASE + 0x4)
#define GDMA2_TX_DROPCNT   	(GDMA2_COUNT_BASE + 0x8)
#define GDMA2_TX_OKBYTE_CNT   	(GDMA2_COUNT_BASE + 0xc)

#define GDMA2_TX_ETHCNT   	(GDMA2_COUNT_BASE + 0x10)
#define GDMA2_TX_ETHLENCNT   	(GDMA2_COUNT_BASE + 0x14)
#define GDMA2_TX_ETHDROPCNT   	(GDMA2_COUNT_BASE + 0x18)
#define GDMA2_TX_ETHBCDCNT   	(GDMA2_COUNT_BASE + 0x1C)
#define GDMA2_TX_ETHMULTICASTCNT   	(GDMA2_COUNT_BASE + 0x20)
#define GDMA2_TX_ETH_LESS64_CNT   	(GDMA2_COUNT_BASE + 0x24)
#define GDMA2_TX_ETH_MORE1518_CNT   	(GDMA2_COUNT_BASE + 0x28)
#define GDMA2_TX_ETH_64_CNT   			(GDMA2_COUNT_BASE + 0x2C)
#define GDMA2_TX_ETH_65_TO_127_CNT   	(GDMA2_COUNT_BASE + 0x30)
#define GDMA2_TX_ETH_128_TO_255_CNT   	(GDMA2_COUNT_BASE + 0x34)
#define GDMA2_TX_ETH_256_TO_511_CNT   	(GDMA2_COUNT_BASE + 0x38)
#define GDMA2_TX_ETH_512_TO_1023_CNT   	(GDMA2_COUNT_BASE + 0x3C)
#define GDMA2_TX_ETH_1024_TO_1518_CNT   	(GDMA2_COUNT_BASE + 0x40)

#define GDMA2_RX_OKCNT     	(GDMA2_COUNT_BASE + 0x48)
#define GDMA2_RX_FCDROPCNT     	(GDMA2_COUNT_BASE + 0x4c)
#define GDMA2_RX_RCDROPCNT     	(GDMA2_COUNT_BASE + 0x50)

#define GDMA2_RX_OVDROPCNT    (GDMA2_COUNT_BASE + 0x54)
#define GDMA2_RX_ERRDROPCNT    (GDMA2_COUNT_BASE + 0x58)
#define GDMA2_RX_OKBYTECNT    (GDMA2_COUNT_BASE + 0x5c)

#define GDMA2_RX_ETHERPCNT  (GDMA2_COUNT_BASE + 0x60)
#define GDMA2_RX_ETHERPLEN  (GDMA2_COUNT_BASE + 0x64)
#define GDMA2_RX_ETHDROPCNT (GDMA2_COUNT_BASE + 0x68)
#define GDMA2_RX_ETHBCCNT   (GDMA2_COUNT_BASE + 0x6c)
#define GDMA2_RX_ETHMCCNT   (GDMA2_COUNT_BASE + 0x70)
#define GDMA2_RX_ETHCRCCNT  (GDMA2_COUNT_BASE + 0x74)
#define GDMA2_RX_ETHFRACCNT (GDMA2_COUNT_BASE + 0x78)
#define GDMA2_RX_ETHJABCNT  (GDMA2_COUNT_BASE + 0x7c)
#define GDMA2_RX_ETHRUNTCNT (GDMA2_COUNT_BASE + 0x80)
#define GDMA2_RX_ETHLONGCNT (GDMA2_COUNT_BASE + 0x84)
#define GDMA2_RX_ETH_64_CNT (GDMA2_COUNT_BASE + 0x88)
#define GDMA2_RX_ETH_65_TO_127_CNT (GDMA2_COUNT_BASE + 0x8C)
#define GDMA2_RX_ETH_128_TO_255_CNT (GDMA2_COUNT_BASE + 0x90)
#define GDMA2_RX_ETH_256_TO_511_CNT (GDMA2_COUNT_BASE + 0x94)
#define GDMA2_RX_ETH_512_TO_1023_CNT (GDMA2_COUNT_BASE + 0x98)
#define GDMA2_RX_ETH_1024_TO_1518_CNT (GDMA2_COUNT_BASE + 0x9C)


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

#define GSW_IMC     		(GSW_ARL_BASE + 0x1c)
#define IMC_IGMP_RPT_FW_SHIFT			(12)

#define GSW_PSC(n)     		(GSW_PORT_BASE + (n)*0x100 + 0x0C)

#define GSW_PMCR(n)     	(GSW_MAC_BASE + (n)*0x100)
#define GSW_PMSR(n)     	(GSW_MAC_BASE + (n)*0x100 + 0x08)
#define GSW_PINT_EN(n)     	(GSW_MAC_BASE + (n)*0x100 + 0x10)
#define GSW_SMACCR0     	(GSW_MAC_BASE + 0xe4)
#define GSW_SMACCR1     	(GSW_MAC_BASE + 0xe8)
#define GSW_CKGCR		(GSW_MAC_BASE + 0xf0)

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

#define GSW_PVC(n)     	(0x2010 + (n)*0x100)
#define DEFAULT_TPID  	(0x8100)

#define GSW_CFG_PPSC     	(GSW_CFG_BASE + 0x18)
#define GSW_CFG_PIAC     	(GSW_CFG_BASE + 0x1c)
#define GSW_CFG_GPC     	(GSW_CFG_BASE + 0x14)


#define GSW_VLAN_REG		(GSW_BASE+0x94)
#define GSW_ATA1_REG		(GSW_BASE+0x74)
#define GSW_ATC_REG			(GSW_BASE+0x80)
/***************************************
 * Ethernet Module Register Definition *
 ***************************************/

/* RSTCTRL2 */
#define QDMA1_RST    				(1<<1)
#define QDMA2_RST    				(1<<2)
#define FE_RST    					(1<<21)
#define ESW_RST    					(1<<23)


/* FE_VLAN_ID */
#define VLAN_ID1_SHIFT   				(16)
#define VLAN_IDl       					(0xfff<<VLAN_ID1_SHIFT)
#define VLAN_ID0_SHIFT   				(0)
#define VLAN_ID0       					(0xfff<<VLAN_ID0_SHIFT)


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

/* FPORT DEFINE */
#define DPORT_PDMA				0
#define DPORT_GDMA1				1
#define DPORT_GDMA2				2 /* means wan , like SAR/PTM/xPON */
#define DPORT_PPE				4
#define DPORT_QDMA				5/* send to CPU via QDMA */
#define DPORT_QDMA_HW			6/* send to CPU via QDMA HW */
#define DPORT_DISCARD			7
#define DPORT_CPU				DPORT_PDMA

/* PKT_INFO define */
#define IPV6_H						(1<<5)
#define IPV4_H						(1<<4)
#define IPV4_H_INV					(1<<3)
#define TU_H						(1<<1)
#define TU_H_C_INV					(1<<0)


/************************************************************************
*               M A C R O S
*************************************************************************
*/

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




#endif /* _FEMAC_H */

