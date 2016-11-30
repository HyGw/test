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
#ifndef _ETH_GEN_H
#define _ETH_GEN_H

/************************************************************************
*               I N C L U D E S
*************************************************************************
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/dma-mapping.h>
#include <linux/mii.h>
#include <linux/if_vlan.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/rt_flash.h>
#include <linux/version.h> 
#include <asm/io.h>
#include <asm/tc3162/tc3162.h>
#include <asm/tc3162/ledcetrl.h>
#include <asm/tc3162/TCIfSetQuery_os.h>

#ifdef TCPHY_SUPPORT
#include <asm/tc3162/cmdparse.h>
#include "./tcphy/tcswitch.h"
#endif


#ifdef TCSUPPORT_RA_HWNAT
#include <linux/foe_hook.h>
#endif
#define KERNEL_2_6_36 		(LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31))

#include "eth_en7512.h"
#include <ecnt_hook/ecnt_hook_ether.h>
#include <ecnt_hook/ecnt_hook_fe.h>
#include <ecnt_hook/ecnt_hook_ephy.h>
#include <ecnt_hook/ecnt_hook_qdma.h>

//#define LOOPBACK_SUPPORT

/************************************************************************
*               D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
#ifndef TCSUPPORT_QOS
#define QOS_REMARKING  1  
#endif
#define TCSUPPORT_HW_QOS

#ifdef QOS_REMARKING  
#define QOS_REMARKING_MASK    0x00000007
#define QOS_REMARKING_FLAG    0x00000001
/*#define QOS_DMAWRR_USERDEFINE  0x01*/
#endif
#if defined (QOS_REMARKING) || defined(TCSUPPORT_HW_QOS)
#define QOS_DMAWRR_USERDEFINE 0x1
#define PTQCR_WRR_EN			(1<<4)
#define PTQCR_WRR_SELECT 	 	(1<<6)
#endif


#ifdef TCSUPPORT_QOS
#define		QOS_FILTER_MARK		0xf0
#define 	QOS_HH_PRIORITY		0x10
#define 	QOS_H_PRIORITY		0x20
#define 	QOS_M_PRIORITY		0x30
#define		QOS_L_PRIORITY		0x40
	
#define		NULLQOS				-1
#define 	QOS_SW_PQ			0	//will use hw at the same time
#define		QOS_SW_WRR			1
#define		QOS_SW_CAR			2
#define 	QOS_HW_WRR			3
#define		QOS_HW_PQ			4
	
#endif
	
#define LAN_ST_100MB        0x01
#define LAN_ST_FULL_DUPLEX	0x02
#define LAN_ST_LINK_UP      0x04
#define LAN_ST_1000MB       0x08

#define SUCCESS 0
#define FAIL 1

#define RX_MIB 1
#define TX_MIB 0

#define DROP 1

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


#define EXTERNAL 1
#define INTERNAL 0

#define SOC_TO_EXT 0
#define EXT_TO_SOC 1

#define FHEWAN_RX_HOOK 0
#define FHEWAN_TX_HOOK 1
#define STAG_REPLACE_MODE 0
#define STAG_INSERT_MODE 1

/************************************************************************
*               M A C R O S
*************************************************************************
*/
#define CHK_BUF() pos = begin + index; if (pos < off) { index = 0; begin = pos; }; if (pos > off + count) goto done;
#define read_reg_word(reg) 		regRead32(reg)
#define write_reg_word(reg, wdata) 	regWrite32(reg, wdata)

	
#define RX_BUF_LEN 			(2048 - NET_SKB_PAD - 64 - (sizeof(struct skb_shared_info)))

#if defined(TCSUPPORT_FTTDP_V2)
#define INSERT_STAG(skb) skb
#define REMOVE_STAG(skb) 0
#else
#define INSERT_STAG(skb) macEN7512STagInsert(skb)
#define REMOVE_STAG(skb) macEN7512STagRemove(skb)
#endif

#ifdef TCSUPPORT_AUTOBENCH
#define eth_tx_autobench    eth_mac_tx
#define eth_lan_mac_tx      eth_mac_tx_autobench
#else
#define eth_lan_mac_tx      eth_mac_tx
#endif

#define pause(x)        mdelay(x)
#define atoi(x)         simple_strtoul(x, NULL,10)


/************************************************************************
*               D A T A   T Y P E S
*************************************************************************
*/
typedef struct _gsw_reg {
    __u32 off;
    __u32 val;
} gsw_reg;

/*********************
 * Mac Control Block *
 *********************/

/* ----- MIB-II ----- */
typedef struct macMIB_II_s {
    uint32 inOctets;            /* Receive Octets */
    uint32 inUnicastPkts;       /* Receive Unicast Packets */
    uint32 inMulticastPkts;     /* Receive Multicast Packets */
    uint32 inBroadcastPkts;     /* Receive Broadcast Packets */
    uint32 inDiscards;          /* Receive Discard Packets */
    uint32 inErrors;            /* Receive Error Packets */
    uint32 inUnknownProtocols;  /* Receive Unknown Prototol Packets */
    uint32 outOctets;           /* Transmit Octets */
    uint32 outUnicastPkts;      /* Transmit Unicast Packets */
    uint32 outMulticastPkts;    /* Transmit Multicast Packets */
    uint32 outBroadcastPkts;    /* Transmit Broadcast Packets */
    uint32 outDiscards;         /* Transmit Discard Packets */
    uint32 outErrors;           /* Transmit Error Packets */
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

    uint32 rxPktIPChkSumErr;

    uint32 rxEnQueueNum;          /* Number of packets enqueued in macRxRingproc() */
    uint32 rxDeQueueNum;          /* Number of packets dequeued in macRxToUpperTask() */
    uint32 txEnQueueNum;          /* Number of packets enqueued in macSend() */
    uint32 txDeQueueNum;          /* Number of packets dequeued in macTxRingproc() */
} inSiliconStat_t;


/* ----- Ethernet Link Profile ----- */
typedef struct macPhyLinkProfile_s {
    uint32 linkSpeed;                           /* 10Mbps or 100Mbps */
    uint32 duplexMode;                          /* Half/Full Duplex Mode */
    uint32 enetMode;
    uint32 ANCompFlag;                          /* auto_negotiation complete Flag */
    uint32 PollCount;                           /* auto_negotiation polling check count */
} macPhyLinkProfile_t;


/* ----- Statistics for GMAC ----- */
typedef struct macStat_s {
    macMIB_II_t MIB_II; /* MIB-II */
    inSiliconStat_t inSilicon;
} macStat_t;

/* ----- Adapter Card Table ------ */
typedef struct macAdapter_s {
    uint8  macAddr[6];  /* MAC-Address */
    macPhyLinkProfile_t *macPhyLinkProfile_p;
    macStat_t           macStat;
    uint8 statisticOn;  /* Flag to record statistics or not */

    uint32 resetNum;    /* Number of Reset the LAN Cont. */
    uint32 enetPhyAddr;
    uint8  enetPhyId;

    struct mii_if_info mii_if;

    spinlock_t lock;
    struct net_device_stats stats;
#if KERNEL_2_6_36
    struct net_device   *dev;
    struct napi_struct  napi;
#endif
} macAdapter_t;

typedef struct phyDeviceList_s {
    uint16 companyId;
    char vendorName[30];
} phyDeviceList_t;


typedef union {
	struct {
#ifdef __BIG_ENDIAN
		uint32 rev		: 4;
		uint32 sp_tag 		: 16;
		uint32 oam			: 1;
		uint32 channel		: 8;
		uint32 queue		: 3;
#else
		uint32 queue		: 3;
		uint32 channel		: 8;
		uint32 oam			: 1;
		uint32 sp_tag		: 16;
		uint32 rev		: 4;
#endif /* __BIG_ENDIAN */
	
#ifdef __BIG_ENDIAN
		uint32 ico			: 1;
		uint32 uco			: 1;
		uint32 tco			: 1;
		uint32 tso			: 1;
		uint32 udf_pmap		: 6;
		uint32 fPort		: 3;
		uint32 vlanEn		: 1;
		uint32 vlanTpID 	: 2;
		uint32 vlanTag		: 16;
#else
		uint32 vlanTag		: 16;
		uint32 vlanTpID 	: 2;
		uint32 vlanEn		: 1;
		uint32 fPort		: 3;
		uint32 udf_pmap		: 6;
		uint32 tso			: 1;
		uint32 tco			: 1;
		uint32 uco			: 1;
		uint32 ico			: 1;
#endif /* __BIG_ENDIAN */
	} raw ;
	uint msg[2] ;
} ethTxMsg_t ;

typedef union
{
        uint32 word;
} rxMsgWord0_t;

typedef union
{
    struct {
#ifdef __BIG_ENDIAN
        uint32 rev          :3;
        uint32 ip6          :1;
        uint32 ip4          :1;
        uint32 ip4f         :1;
        uint32 tack         :1;
        uint32 l2vld        :1;
        uint32 l4f          :1;
        uint32 sport        :4;
        uint32 crsn         :5;
        uint32 ppe_entry    :14;
#else
        uint32 ppe_entry    :14;
        uint32 crsn         :5;
        uint32 sport        :4;
        uint32 l4f          :1;
        uint32 l2vld        :1;
        uint32 tack         :1;
        uint32 ip4f         :1;
        uint32 ip4          :1;
        uint32 ip6          :1;
        uint32 rev          :3;
#endif
    }raw;
    uint32 word;
} rxMsgWord1_t;

typedef union
{
        struct {
#ifdef __BIG_ENDIAN
           uint32 rsv        :31;
           uint32 untag    :1;
#else
           uint32 untag    :1;
           uint32 rsv        :31;
#endif
        }raw;
        uint32 word;
} rxMsgWord2_t;

typedef union
{
        uint32  sp_tag;
        uint32  resv;
} rxMsgWord3_t;

typedef struct ethRxMsg_s
{
        rxMsgWord0_t rxMsgW0;
        rxMsgWord1_t rxMsgW1;
        rxMsgWord2_t rxMsgW2;
        rxMsgWord3_t rxMsgW3;
} ethRxMsg_t;


/************************************************************************
*               D A T A   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*               F U N C T I O N   D E C L A R A T I O N S
                I N L I N E  F U N C T I O N  D E F I N I T I O N S
*************************************************************************
*/

int eth_mac_tx(struct sk_buff *skb, struct net_device *dev);
void update_sw_mib(macAdapter_t *mib_type,struct sk_buff *skb, int direction);
uint32 calcnt(uint32 basereg);

uint32 switch_reg_read(uint32 reg);
uint32 switch_reg_write(uint32 reg, uint32 wdata);
uint32 gswPbusRead(uint32 pbus_addr);
int gswPbusWrite(uint32 pbus_addr, uint32 pbus_data);

uint32 gswPbusRead2(uint32 pbus_addr);
int gswPbusWrite2(uint32 pbus_addr, uint32 pbus_data);

#define LIMIT_PORT_NUM 	5
#define LIMIT_BUFFER_FOR_100MB 	0
#define LIMIT_BUFFER_FOR_1GB 	0
#define WIFI_OFFLOAD_CHANNEL	7

#define DSTQ_BIT_MASK_SIZE (1<<5)

typedef struct{
	int linkState;	/* 0: down, 1: up */
	int linkSpeed;	/* 0: 10m, 1: 100m , 2: 1g */
}portInfo;

#if (__GNUC__ >= 4 && __GNUC_MINOR__ <= 3)
inline unsigned long int getPortMask(struct sk_buff *skb);
inline int setPortMask(struct sk_buff *skb, uint8 port_mask);
#endif

int macEN7526cSTagInsert(ethTxMsg_t * TxMsg,struct sk_buff **bpp);
int macEN7526cSTagRemove(ethRxMsg_t * RxMsg,struct sk_buff *bp);

#endif /*_XXX_H*/

