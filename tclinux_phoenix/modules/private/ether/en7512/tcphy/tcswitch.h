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
#ifndef _TCSWITCH_H_
#define _TCSWITCH_H_


/************************************************************************
*               I N C L U D E S
*************************************************************************
*/

/************************************************************************
*               D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
    /* auto detect 4 port switch ic */
#define SWIC_ADM6996M		1
#define SWIC_IP175C			2
#define SWIC_MARVEL6060     3
#define SWIC_RTL8305        4
#define SWIC_RTL8306SD		5
#define SWIC_TC2206	7
#define SWIC_RT63365		8
#define SWIC_RT62806		9
#define SWIC_MT7530		10
#define HW_IGMPSNOOP	(1<<0)
    
#define RX_STAG_LEN 8
#define TX_STAG_LEN 6
#define DEF_STAG_VALUE 0x8901
#define IPPROTOL	0x0800
#if 0
#define SKBUF_COPYTOLAN (1 << 22)
#define SKBUF_TCCONSOLE (1 << 23)
#else
#ifdef TCSUPPORT_XPON_HAL_API_EXT
#define SKBUF_COPYTOLAN (1 << 24)
#else
#define SKBUF_COPYTOLAN (1 << 26)
#endif
#define SKBUF_TCCONSOLE (1 << 27)
#if !defined(TCSUPPORT_CT)
#if 1//def VPORT
#define SKBUF_VLAN 		(1 << 28)
#endif
#endif
    
#endif
    
#define MODEL_V0	(1<<0)	/*TC2206A1*/
#define MODEL_V1	(1<<1)	/*TC2206B2*/
#define MODEL_V2	(1<<2)	/*TC2206F*/
#if !defined(TCSUPPORT_CT)
#if 1//def VPORT
#define VLAN_HLEN 4
#define BASE_VID_IDX 1
#endif
#endif
    
#define RT63365_SWIC_PORTNUM	7
#define MT7530_SWIC_PORTNUM	7

#define RAETH_GSW_PHY_BP_READ           0x89FE
#define RAETH_GSW_PHY_BP_WRITE          0x89FF
#define TCSUPPORT_CPU_MT7510_FAKE

#ifdef EXT_SWITCH_PHY_CONNECT_INT_MDIO
#define EXT_SWITCH_PHY_ADDR_OFFSET	(0)
#define INT_SWITCH_PHY_ADDR_OFFSET	(8)

#endif	//EXT_SWITCH_PHY_CONNECT_INT_MDIO
/******************************
* Giga Switch IOCTL Commands *
******************************/
/* ioctl commands */
#define RAETH_DSTQ              0x89F0
#define RAETH_GSW_REG_READ      0x89F1
#define RAETH_GSW_REG_WRITE     0x89F2
#define RAETH_REG_READ          0x89F3
#define RAETH_REG_WRITE         0x89F4
#define RAETH_GSW_PHY_READ      0x89F5
#define RAETH_GSW_PHY_WRITE     0x89F6

#define RAETH_GSWEXT_REG_READ       0x89F7
#define RAETH_GSWEXT_REG_WRITE      0x89F8
#define RAETH_GSWEXT_PHY_READ       0x89F9
#define RAETH_GSWEXT_PHY_WRITE      0x89FA
 
#ifdef TCSUPPORT_MT7530_SWITCH_API
#define RAETH_GSW_CTLAPI        0x89FB
#endif
 
#define MT7530_PHY_VERIFY
#ifdef MT7530_PHY_VERIFY
#define RAETH_GSWEXT_PHY_READ_45    0x89FC
#define RAETH_GSWEXT_PHY_WRITE_45   0x89FD
#endif


/*-----------------ACL define-----------------*/
#define ACL_MAX_NUM 32

#define SPECIAL_RULE_NUM 	2		/*general set: 0*/
#define DMAC_RULE_NUM		0		/*general set: 3*/
#define SMAC_RULE_NUM		0		/*general set: 3*/
#define VLAN_RULE_NUM		16		/*general set: 8*/

#define PATTERN_FOR_BC_MC_NUM       5
#define PATTERN_FOR_SPECIAL_NUM		(PATTERN_FOR_BC_MC_NUM + SPECIAL_RULE_NUM)
#define PATTERN_FOR_DMAC_NUM		(3 * DMAC_RULE_NUM)
#define PATTERN_FOR_SAMC_NUM		(3 * SMAC_RULE_NUM)
#define PATTERN_FOR_VLAN_NUM		(1 + VLAN_RULE_NUM)
#define PATTERN_ALL_NUM				(PATTERN_FOR_SPECIAL_NUM + PATTERN_FOR_DMAC_NUM + PATTERN_FOR_SAMC_NUM + PATTERN_FOR_VLAN_NUM)
/*-----------------ACL index define, strat from 0-----------------*/
#define RGMII_TRAFFIC 5
#define TRGMII_TRAFFIC 6

#define ACL_PATTERN_WRITE(n)     	(0x80005000 + n)
#define ACL_PATTERN_READ(n)     	(0x80004000 + n)
#define ACL_MASK_WRITE(n)     		(0x80009000 + n)
#define ACL_MASK_READ(n)     		(0x80008000 + n)
#define ACL_RULE_WRITE(n)     		(0x8000b000 + n)
#define ACL_RULE_READ(n)     		(0x8000a000 + n)
#define ACL_RATE_WRITE(n)     		(0x8000d000 + n)
#define ACL_RATE_READ(n)     		(0x8000c000 + n)

#define ACL_PATTERN_MASK_OFFSET 		16
#define ACL_PATTERN_ENABLE_OFFSET  		19
#define ACL_PATTERN_OFFSETRANGE_OFFSET	16
#define ACL_PATTERN_PORTMAP_OFFSET		8
#define ACL_PATTERN_WORDOFFSET_OFFSET	1

#define ACL_CTL1_MANG_OFFSET			29
#define	ACL_CTL1_INT_EN_OFFSET			28
#define	ACL_CTL1_CNT_EN_OFFSET			27
#define ACL_CTL1_CNT_INDEX_OFFSET		24
#define ACL_CTL1_VLAN_PORT_EN_OFFSET	23
#define	ACL_CTL1_DA_SWAP_OFFSET			22
#define ACL_CTL1_SA_SWAP_OFFSET			21
#define ACL_CTL1_PPP_RM_OFFSET			20
#define ACL_CTL1_LKY_VLAN_OFFSET		19
#define ACL_CTL1_EG_TAG_OFFSET			16
#define ACL_CTL1_PORT_OFFSET			8
#define ACL_CTL1_PORT_EN_OFFSET			7
#define ACL_CTL1_PRI_USER_OFFSET		4
#define ACL_CTL1_MIR_EN_OFFSET			3

#define ACL_CTL2_CLASS_IDX_OFFSET		19
#define ACL_CTL2_RATE_MODE_OFFSET		16
#define ACL_CTL2_DROP_PCD_G_OFFSET		11
#define ACL_CTL2_DROP_PCD_Y_OFFSET		8
#define ACL_CTL2_DROP_PCD_R_OFFSET		5
#define ACL_CTL2_CLASS_SLR_OFFSET		2
#define ACL_CTL2_CLASS_SLR_SEL_OFFSET	1

#define ACL_RATE_EN_OFFSET				15

/************************************************************************
*               M A C R O S
*************************************************************************
*/

/************************************************************************
*               D A T A   T Y P E S
*************************************************************************
*/
typedef struct swic_reg{
    uint16 model_id;
    uint16 phyaddr;
    uint16 regaddr;
    uint16 value;
}swic_reg_t;


typedef struct _gsw_phy_cl45
{
	u32 portAddr;
	u32 devAddr;
	u32 regAddr;
	u32 miiData;
} gsw_phy_cl45;

#if defined(TCSUPPORT_CT_LOOPDETECT)
struct cthdr
{
	unsigned char name[6];
	unsigned char magic[4];
	__u8	from;
	__u8	to;
	__u8 	ct_pad[68];
};
#endif

/*-----------------ACL struct-----------------*/
/*acl_pattern_t : for pattern information*/
typedef struct{
	uint16 pattern_mask;
	uint16 pattern;

	uint32 reserved :		12 	;
	uint32 enable:			1	;
	uint32 offset_range:	3	;
	uint32 port_map:		8	;
	uint32 word_offset:		7	;
	uint32 compare_mode:	1	;
}acl_pattern_t;

typedef struct{
	uint32 reserved:  		2 ;
	uint32 acl_mang: 		1 ;
	uint32 int_en: 			1 ;
	uint32 acl_cnt_en: 		1 ;
	uint32 cnt_idx:			3 ;
	uint32 vlan_port_en:	1 ;
	uint32 da_swap:			1 ;
	uint32 sa_swap:			1 ;
	uint32 ppp_rm:			1 ;
	uint32 lky_vlan:		1 ;
	uint32 eg_tag:			3 ;
	uint32 port:			8 ;
	uint32 port_en:			1 ;
	uint32 pri_user:		3 ;
	uint32 mir_en:			1 ;
	uint32 port_fw:			3 ;
}acl_control_1_t;

typedef struct{
	uint32 reserved_0: 		8 ;
	uint32 acl_class_idx: 	5 ;
	uint32 reserved_1: 		2 ;
	uint32 rate_mode: 		1 ;	/*1:trtcm, 0:acl rate*/
	uint32 reserved_2: 		2 ;
	uint32 drop_pcd_g: 		3 ;
	uint32 drop_pcd_y: 		3 ;
	uint32 drop_pcd_r: 		3 ;
	uint32 class_slr:  		3 ;
	uint32 class_slr_sel: 	1 ;
	uint32 drop_pcd_sel: 	1 ;

}acl_control_2_t;

typedef struct{
	uint16 rate_en: 	1 ;
	uint16 reserved:	1 ;
	uint16 rate: 		14; /*unit: 64Kbps*/
}acl_rate_t;

/************************************************************************
*               D A T A   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*               F U N C T I O N   D E C L A R A T I O N S
                I N L I N E  F U N C T I O N  D E F I N I T I O N S
******************1234123sdfasdfasdf*******************************************************
*/

void periodChk(void);

int macRxPortEnable(int ChanID);

#if defined(TCSUPPORT_WAN_ETHER)
int isComeFromWan(struct sk_buff *skb);
void setComeFromWan(struct sk_buff *skb, int yes);
#define CB_MAGIC	0x32620001
#endif


struct sk_buff * macEN7512STagInsert(struct sk_buff *bp);
int macEN7512STagRemove(struct sk_buff *bp);
void init_ethernet_port_map(void);



#endif /* _TCSWITCH_H_ */




//  DEFAULT_USE_EXT_SWIC

