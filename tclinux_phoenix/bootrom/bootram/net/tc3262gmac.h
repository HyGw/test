/*
** $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/bootrom/bootram/net/tc3262gmac.h#1 $
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
** Revision 1.1.1.1  2010/04/09 09:31:07  feiyan
** New TC Linux Make Flow Trunk
**
** Revision 1.1.1.1  2009/12/17 01:47:29  josephxu
** 20091217, from Hinchu ,with VoIP
**
 */
#ifndef _TC3262GMAC_H
#define _TC3262GMAC_H

#include <asm/tc3162.h>

#include "skbuff.h"

#define MAC_TXDESCP_NO		64	/* Max tx buffer Cnts , default=64 */
#define MAC_RXDESCP_NO		64	/* Max rx buffer Cnts , default=64 */

#define MAC_RXDESCP_SIZE	32	/* 8 DWords */
#define MAC_TXDESCP_SIZE	16	/* 4 DWords */

#define K0_TO_K1(x)			(((uint32)x) | 0xa0000000)
#define K1_TO_PHY(x)		(((uint32)x) & 0x1fffffff)

/* ----- PHY-Chip Definition ----- */
#define PHY_CONTROL_REG 		0		/* MII Control Register */
#define PHY_RESET				0x8000
#define PHY_POWER_DOWN			0x0800
#define PHY_AN_ENABLE 			0x1000
#define PHY_RESTART_AN 			0x200
#define PHY_SPEED_SELECT_100 	0x2000
#define PHY_FULL_DUPLEX			0x100
#define PHY_ISOLATE 			0x400
#define PHY_LOOPBACK 			0x4000

#define PHY_STATUS_REG 			1		/* MII Status Register */
#define PHY_AN_COMPLETE			0x0020
#define PHY_LINK_STATUS			0x0004

#define PHY_ID_REG1 			2		/* Identifier register 1 */
#define PHY_ID_REG2 			3		/* Identifier register 2 */

#define PHY_REMOTE_CAP_REG 		5		/* Auto-Negotiation Link Partner Ability Reg */
#define PHY_100_BASE_T4			0x200
#define PHY_100_BASE_TX_FD 		0x100
#define PHY_100_BASE_TX_HD 		0x80
#define PHY_10_BASE_T_FD 		0x40
#define PHY_10_BASE_T_HD 		0x20

#define PHY_CONFIG_REG			16
#define PHY_TP_LOOPBACK			0x0100

#define MAC_STATISTIC_ON		1
#define MAC_STATISTIC_OFF		0

/*****************************
 * Ethernet Module Registers *
 *****************************/

#define CR_GMAC_BASE     	0xBFB50000
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
#define CR_GMAC_GDRPCR     	(0x18c | CR_GMAC_BASE)
#define CR_GMAC_GRPPCR     	(0x190 | CR_GMAC_BASE)

/***************************************
 * Ethernet Module Register Definition *
 ***************************************/

/* CR_GMAC_GCTR */
#define GCTR_TX_DMA_EN					(1<<0)
#define GCTR_RX_DMA_EN					(1<<1)
#define GCTR_SW_RESET					(1<<2)
#define GCTR_LOOP_EN					(1<<3)
#define GCTR_RX_CRC_DIS					(1<<4)
#define GCTR_TX_EN						(1<<5)
#define GCTR_HW_RESET					(1<<6)
#define GCTR_TX_PADDING					(1<<7)
#define GCTR_RX_EN						(1<<8)
#define GCTR_RX_RUNT					(1<<10)
#define GCTR_RX_LONG					(1<<11)
#define GCTR_RX_ALL						(1<<12)
#define GCTR_TX_FCS						(1<<14)
#define GCTR_FULL_DUPLEX				(1<<15)
#define GCTR_RX_BCAST					(1<<16)
#define GCTR_RX_MCAST					(1<<17)
#define GCTR_RX_UNTAG					(1<<19)
#define GCTR_BACKOFF_DIS				(1<<29)
#define GCTR_RX_MAX_LEN					(3<<30)
#define GCTR_RX_MAX_LEN_1522			(0)
#define GCTR_RX_MAX_LEN_1536			(1)
#define GCTR_RX_MAX_LEN_1552			(2)
#define GCTR_RX_MAX_LEN_1792			(3)

/* CR_GMAC_GIMR or CR_GMAC_GISR */
#define GIMR_TX_DONE					(1<<0)
#define GIMR_RX_DONE					(1<<1)
#define GIMR_RX_FIFO_OVR				(1<<2)
#define GIMR_DMA_ERR					(1<<3)
#define GIMR_TX_OWNER_ERR				(1<<4)
#define GIMR_RX_OWNER_ERR				(1<<5)
#define GIMR_RX_RING_FULL				(1<<6)
#define GIMR_PHY_INT					(1<<7)

/* CR_GMAC_GSPR */
#define GSPR_FC_EN						(1<<0)
#define GSPR_CFG_SPD_100M				(1<<1)
#define GSPR_MAC_SPD_GIGA				(1<<2)
#define GSPR_COL_ABT_DIS				(1<<3)
#define GSPR_MTCC_LMT_SHIFT				(4)
#define GSPR_MTCC_LMT					(0xf<<GSPR_MTCC_LMT_SHIFT)
#define GSPR_CFG_CR_IFG_SHIFT			(8)
#define GSPR_CFG_CR_IFG					(0xf<<GSPR_CFG_CR_IFG_SHIFT)
#define GSPR_ADJ_IFG_SHIFT				(12)
#define GSPR_ADJ_IFG					(0xf<<GSPR_ADJ_IFG_SHIFT)

/* CR_GMAC_PHYOR */
#define PHYOR_DATA						(0xffff<<0)
#define PHYOR_REG_SHIFT					16
#define PHYOR_ADDR_SHIFT				21
#define PHYOR_WRITE						(1<<26)
#define PHYOR_START						(1<<27)
#define PHYOR_ERR						(1<<28)

#define NEW_PHYOR_DATA					(0xffff<<0)
#define NEW_PHYOR_REG_SHIFT				16
#define NEW_PHYOR_ADDR_SHIFT			21
#define NEW_PHYOR_OP_SHIFT				26
#define NEW_PHYOR_OP_WR					(1)
#define NEW_PHYOR_OP_RD					(2)
#define NEW_PHYOR_ST_SHIFT				28
#define NEW_PHYOR_ST_IEEE_NEW			(0)
#define NEW_PHYOR_ST_IEEE_OLD			(1)
#define NEW_PHYOR_ERR					(1<<30)
#define NEW_PHYOR_BUSY					(1<<31)

/* CR_GMAC_GPRCTR */
#define GPRCTR_RX_PKT_DONE_SHIFT		(16)
#define GPRCTR_TX_PKT_DONE_SHIFT		(0)

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
//	sk_buff *skb;
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
//	sk_buff *skb;
} macRxDescr_t;

/*********************
 * Mac Control Block *
 *********************/

typedef struct macDrvCtrl_s
{
	macTxDescr_t *txFreeDescr_p;	/* Tx free descriptor */

	macTxDescr_t *txBusyDescr_p;	/* Tx busy descriptor */
	uint16 txQueuedFrameCnt;		/* cnts of frames haven't freed yet */
	uint32 macDrvResetCnt;			/* reset counts */
} macDrvCtrl_t;


/* ----- MIB-II ----- */
typedef struct macMIB_II_s
{
	uint32 ifadminstatus;		/* MIB adminstatus */
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

	uint32 rxPktIPChkSumErr;

	uint32 rxEnQueueNum;          /* Number of packets enqueued in macRxRingproc() */
	uint32 rxDeQueueNum;          /* Number of packets dequeued in macRxToUpperTask() */
	uint32 txEnQueueNum;          /* Number of packets enqueued in macSend() */
  	uint32 txDeQueueNum;          /* Number of packets dequeued in macTxRingproc() */

} inSiliconStat_t;


/* ----- Ethernet Link Profile ----- */
typedef struct macPhyLinkProfile_s
{
	uint32 linkSpeed;							/* 10Mbps or 100Mbps */
	uint32 duplexMode;							/* Half/Full Duplex Mode */
	uint32 enetMode;
	uint32 ANCompFlag;							/* auto_negotiation complete Flag */
	uint32 PollCount;							/* auto_negotiation polling check count */
} macPhyLinkProfile_t;

/* with 16 byte alignment */
#define DESCRIPTOR_POOL_SIZE	( MAC_RXDESCP_SIZE*MAC_RXDESCP_NO + 8*MAC_TXDESCP_SIZE*MAC_TXDESCP_NO + 32 )

/* ----- Ethernet private memory pool ----- */
typedef struct macMemPool_s
{
	uint8 descrPool[DESCRIPTOR_POOL_SIZE];	/* Descr pool area */
} macMemPool_t;

/* ----- Ethernet memory allocation map ----- */
typedef struct macAllocMap_s
{
	uint32	txDescpBaseVA;	/* tx descriptor base virtual address */
	uint32	rxDescpBaseVA;	/* rx descriptor base virtual address */
} macAllocMap_t;

/* ----- Statistics for TC35815 ----- */
typedef struct macStat_s
{
	macMIB_II_t MIB_II;	/* MIB-II */
	inSiliconStat_t inSilicon;	/* TC35815 statistics  */

} macStat_t;

/* ----- Adapter Card Table ------ */
typedef struct macAdapter_s
{
  uint32 ChanID;      /* Channel ID */
//uint32 slotNumber;  /* PCI Slot No.(Indx No.) */
  uint8  macAddr[6];  /* MAC-Address */

  macPhyLinkProfile_t *macPhyLinkProfile_p;
//macAllocMap_t       *macAllocMap_p;
  macMemPool_t        *macMemPool_p;
  macStat_t           *macStat_p;
  uint8 statisticOn;  /* Flag to record statistics or not */

//uint32 ioBaseAddr;  /* PCI I/O Base Address */

  uint32 descrSize;   /* Reserved */
  uint32 rxRingSize;  /* Receive Descr Ring Size */
  #if 0	//050913 bread move these three new entries to be compatible with old definition
  uint32 txRingSize;  /* Receive Descr Ring Size */
  uint32 rxDescrSize; /* The size of a Rx descr entry in the descr ring */
  uint32 txDescrSize; /* The size of a Tx descr entry in the descr ring */
  #endif
  uint32 resetNum;    /* Number of Reset the LAN Cont. */
  uint32 enetPhyAddr;
  uint8  enetPhyId;

	/* zd2001 */
  macTxDescr_t* txCurrDescp;    /* Currently working tx descriptor */
  uint32 txDescrRingBaseVAddr;  /* Transmit Descr Ring Virtual Address with 4byte alignment	*/
  uint32 rxDescrRingBaseVAddr;  /* Receive Descr Ring Virtual Address with 4byte alignment*/
  uint32 txCurrentDescp;        /* index to current tx descriptor */
  uint32 txUnReleasedDescp;     /* index to the unreleased descp of Tx */
  uint32 txUnReleasedBufCnt;    /* Unreleased buffer cnt hold by Tx	*/
  uint32 rxCurrentDescp;        /* index to current rx descriptor */

//050913 bread moved here
  uint32 txRingSize;  /* Receive Descr Ring Size */
  uint32 rxDescrSize; /* The size of a Rx descr entry in the descr ring */
  uint32 txDescrSize; /* The size of a Tx descr entry in the descr ring */
////
  sk_buff *txskbs[MAC_TXDESCP_NO];
  sk_buff *rxskbs[MAC_RXDESCP_NO];
} macAdapter_t;

typedef struct phyDeviceList_s {
	uint16 companyId;
	char vendorName[30];
} phyDeviceList_t;

int tc3162_eth_init(unsigned char *mac_addr);
int tc3162_eth_send(sk_buff *skb);

int tc3162_eth_exit(void);

#endif /* _TC3262GMAC_H */

