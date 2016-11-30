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
#ifndef _TCSWITCH_H_
#define _TCSWITCH_H_

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

#define STAG_PASSTHROUGH (1<<7)

#if 0
#define SKBUF_COPYTOLAN (1 << 22)
#define SKBUF_TCCONSOLE (1 << 23)
#else
#ifdef TCSUPPORT_XPON_HAL_API_EXT
#define SKBUF_COPYTOLAN (1 << 24)
#define DEST_OFFSET		0
#else
#define SKBUF_COPYTOLAN (1 << 26)
#define DEST_OFFSET		28
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

typedef struct swic_reg{
	uint16 model_id;
	uint16 phyaddr;
	uint16 regaddr;
	uint16 value;
}swic_reg_t;

void periodChk(void);
void refillPhyDefVal(uint32 enetPhyAddr, uint32 phyReg, uint32 miiData);
int macRxPortEnable(int ChanID);
int macAdmSTagEnable(int ChanID);
int macIpSTagEnable(int ChanID);
void macIpSTagInsert(struct sk_buff *bp);
void macIpSTagRemove(struct sk_buff *bp);
int macTC2206STagEnable(int chanID);
void macTC2206STagInsert(struct sk_buff *bp);
void macTC2206STagRemove(struct sk_buff *bp);
#if defined(RT62806_VERIFY)
int macRT62806STagEnable(int chanID);
struct sk_buff *macRT62806STagInsert(struct sk_buff *bp);
void macRT62806STagRemove(struct sk_buff *bp);
#endif
void filedSwicDefVal(void);
int getTC2206InitRegFlag(void);
void updatingUpnpGid(void);
void esdSwPatch(void);

#if defined(TCSUPPORT_WAN_ETHER)
int isComeFromWan(struct sk_buff *skb);
void setComeFromWan(struct sk_buff *skb, int yes);
#define CB_MAGIC	0x32620001
#endif
#if defined(TCSUPPORT_SEPERATED_ETH_ITF)
#define CB_MBGIC_ETH_NONE	0x0
#define CB_MBGIC_ETH_0	0x32620000
#define CB_MBGIC_ETH_1	0x32620010
#define CB_MBGIC_ETH_2	0x32620020
#define CB_MBGIC_ETH_3	0x32620030
#define SEP_ETH_ITF_0	(0)
#define SEP_ETH_ITF_1	(1)
#define SEP_ETH_ITF_2	(2)
#define SEP_ETH_ITF_3	(3)
#define SEP_ETH_ITF_UNKNOWN	(-1)
#define MAX_ETH_ITF_NUM	4
#endif /* TCSUPPORT_SEPERATED_ETH_ITF */
#if defined(CONFIG_MIPS_RT63365)
#if defined(TCPHY_SUPPORT)
void enable_abnormal_irq(void);
int polling_abnormal_irq(void);
void duplex_sw_patch(void);
#endif
struct sk_buff *macRT63365STagInsert(struct sk_buff *bp);
void macRT63365STagRemove(struct sk_buff *bp);
#if defined(MT7530_SUPPORT)	
struct sk_buff *macRT63368ExtSTagInsert(struct sk_buff *bp);
void macRT63368ExtSTagRemove(struct sk_buff *bp);
#endif
void init_ethernet_port_map(void);
#endif

/******************************
* Giga Switch IOCTL Commands *
******************************/
/* ioctl commands */
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

typedef struct _gsw_phy_cl45
{
	u32 portAddr;
	u32 devAddr;
	u32 regAddr;
	u32 miiData;
} gsw_phy_cl45;

#endif
#define RAETH_GSW_PHY_BP_READ           0x89FE
#define RAETH_GSW_PHY_BP_WRITE          0x89FF
#define TCSUPPORT_CPU_MT7510_FAKE

#ifdef TCSUPPORT_CPU_MT7510
#undef DEFAULT_USE_EXT_SWIC
#endif
#ifdef TCSUPPORT_CPU_MT7505
#undef DEFAULT_USE_EXT_SWIC
#endif

#if defined(TCSUPPORT_CPU_MT7520) || defined(MT7530_SUPPORT)
#define DEFAULT_USE_EXT_SWIC
#endif
#if defined(TCSUPPORT_CPU_RT65168) || defined(TCSUPPORT_CPU_TC3182)	// for early MT7530 FPGA verification only
#if defined(TCSUPPORT_CT)
#undef DEFAULT_USE_EXT_SWIC
#else
#define DEFAULT_USE_EXT_SWIC
#endif
#endif

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

#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7520E) || defined(TCSUPPORT_CPU_MT751x20G25G) || defined(TCSUPPORT_CPU_MT7511G) || defined(TCSUPPORT_CPU_MT752025)
//#define EXT_SWITCH_PHY_CONNECT_INT_MDIO

#ifdef EXT_SWITCH_PHY_CONNECT_INT_MDIO
#define EXT_SWITCH_PHY_ADDR_OFFSET	(0)
#define INT_SWITCH_PHY_ADDR_OFFSET	(8)

#endif	//EXT_SWITCH_PHY_CONNECT_INT_MDIO

#endif
#endif /* _TCSWITCH_H_ */
