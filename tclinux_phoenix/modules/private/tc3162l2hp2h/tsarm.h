/*
** $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/private/tc3162l2hp2h/tsarm.h#1 $
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
** $Log: tsarm.h,v $
** Revision 1.4  2011/08/05 03:14:01  xyzhu_nj
** #11006: ct com merge to main trunk.(phase 3)
** 1. Add TCSUPPORT_COMPILE tag for ct com compile option;
** 2. Delete files in filesystem that CT_COM not used;
** 3. Merge the lastes bugfix from ct com branch to main trunk.(TLB7.1.2.20-51 -- 57);
** 4. update mic.
**
** Revision 1.3  2011/07/24 12:00:14  xyzhu_nj
** #11006: Merge China telecom branch to Main trunk.
**
** Revision 1.2  2010/12/31 10:33:34  xyzhu_nj
** #8261: sp / wrr qos enhancement.
**
** Revision 1.1.1.1  2010/09/30 21:14:55  josephxu
** modules/public, private
**
** Revision 1.1.1.1  2010/04/09 09:35:29  feiyan
** New TC Linux Make Flow Trunk
**
** Revision 1.3  2010/01/15 09:38:45  here
** [BugFix]Fixed TC3162U Platform the sar interface's rx descriptor is mismatch at SMB Routing throughput(WiFi interface is up)
**
** Revision 1.2  2009/12/30 12:33:21  yzwang_nj
** [Bug#4409] Add TR064 Parameters
**
** Revision 1.1.1.1  2009/12/17 01:48:05  josephxu
** 20091217, from Hinchu ,with VoIP
**
** Revision 1.1.1.1  2007/04/12 09:42:03  ian
** TCLinuxTurnkey2007
**
** Revision 1.3  2006/07/06 07:24:57  lino
** update copyright year
**
** Revision 1.2  2006/02/17 04:19:09  lino
** correct qos type nrtVBR and GFR's value
**
** Revision 1.1.1.1  2005/11/02 05:45:38  lino
** no message
**
** Revision 1.8  2005/10/03 02:59:07  bread.hsu
** 1. reduce ATM_VC_MUX from 10 to 8
** 2. move the new 3 item for 3162L2 on qosProfile_t to buttom to be compatible with old definition
**
** Revision 1.7  2005/09/30 02:02:00  bread.hsu
** new definition for 3162L2 sar driver tx priority
**
** Revision 1.6  2005/09/22 13:21:59  jasonlin
** Add nrtVBR selection on CI command and TC new web page
**
** Revision 1.5  2005/08/23 11:19:10  lino
** pass chann for pkt qos when open vc
**
** Revision 1.4  2005/08/19 14:36:25  jasonlin
** Merge Huawei's code into new main trunk
**
** Revision 1.3  2005/06/22 14:23:38  jasonlin
** Sync with main trunk version 2.2.28
**
** Revision 1.2  2005/06/14 10:02:02  jasonlin
** Merge TC3162L2 source code into new main trunk
**
** Revision 1.1.1.1  2005/03/30 14:04:22  jasonlin
** Import Linos source code
**
** Revision 1.5  2005/01/19 03:23:48  lino
** adjust new MIB fields position to be compatible with the old one to prevent OAM F5 test fake report
**
** Revision 1.4  2004/11/15 03:42:40  lino
** add new define for ATM SAR TX max queue length to prevent mbuf saturation
**
** Revision 1.3  2004/10/12 06:42:21  lino
** add more sar discard counters and enlarge sar tx descriptor number from 16  to 64
**
** Revision 1.2  2004/08/27 12:19:36  lino
** add qos calculation and clean code
**
** Revision 1.1  2004/07/02 08:03:05  lino
** tc3160 and tc3162 code merge
**
*/

#ifndef _TSARM_H_
#define _TSARM_H_

/**********************************
 * General limitation of total VC *
 **********************************/
#ifdef SAR_VERIFY
	#define ATM_VC_MAX						10
#else
	#ifdef PURE_BRIDGE
		#define ATM_VC_MAX                      4
	#else
		#define ATM_VC_MAX						8
	#endif
#endif
#define ATM_DUMMY_VC					21
#define IRQ_MAX_ENTRY					512	//256

#define RX_BUF_LEN 	(2048 - 16 - 64 - (sizeof(struct skb_shared_info)))	

#ifdef CONFIG_CPU_TC3162
#define TC3162L2		1
#endif

#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
	#ifdef PURE_BRIDGE
		#define ATM_TX_PRIORITY_MAX         1
	#else
		#define ATM_TX_PRIORITY_MAX			4
	#endif	
#endif

/******************************************************
 * Cacheable address to non-cacheable address mapping *
 ******************************************************/
#define K0_TO_K1(x)       				((uint32)(x) | 0x20000000)  /* kseg0 to kseg1 */
#define K1_TO_K0(x)       				((uint32)(x) & 0x9fffffff)  /* kseg1 to kseg0 */
#define K0_TO_PHYSICAL(x) 				((uint32)(x) & 0x1fffffff)  /* kseg0 to physical */
#define K1_TO_PHYSICAL(x) 				((uint32)(x) & 0x1fffffff)  /* kseg1 to physical */
#define PHYSICAL_TO_K0(x) 				((uint32)(x) | 0x80000000)  /* physical to kseg0 */
#define PHYSICAL_TO_K1(x) 				((uint32)(x) | 0xa0000000)  /* physical to kseg1 */
#define CACHE_TO_NONCACHE(addr)			((uint32)(addr) | 0xa0000000)

/*********************************
 * Constant of ATM SAR registers *
 *********************************/
/* RAI */
#define RAI_RESET_ENB(x)				(x) << 0
/* GFR */
#define GFR_TXENB						1 << 0
#define GFR_RXENB						1 << 1
#define GFR_GIRQEN						1 << 2
#define GFR_BIGENDIAN					1 << 3
#define GFR_UL_TX_FIX					1 << 4
#define GFR_UL_RX_FIX					1 << 5
#define GFR_REMOTE_LPK					1 << 6
#define GFR_LOCAL_LPK					3 << 6
#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
#define GFR_ACTIVE_MIS					1 << 9
#else
#define GFR_BEST_EFFORT					1 << 8
#endif
#define GFR_TSA_WRR_EN					(1<<14)
#define GFR_DMT_WRR_EN                  1 << 15  /*Rodney_20090724*/
#define GFR_RX_INACT_VC_M				1 << 30
#define GFR_IRQ_Q_FULL_M				1 << 31

#define VCCFGR_VALID					1 << 0
#define VCCFGR_RXRAW					1 << 1

#define VCCFGR_PORT(x)					(x) << 2
#define VCCFGR_ATM_PHY0					0x0
#define VCCFGR_ATM_PHY1					0x1

#define VCCFGR_VCI(x)					(x)<<4
#define VCCFGR_VPI(x)					(x)<<20

/**********************************
 * Constant of ATM SAR IRQ Status *
 **********************************/
#define LIRQ_TX_U_DONE					(1 << 0)
#define LIRQ_TX_M_DONE					(1 << 1)
#define LIRQ_RX_U_DONE					(1 << 2)
#define LIRQ_RX_M_DONE					(1 << 3)
#define LIRQ_TX_BUF_DONE				(1 << 4)
#define LIRQ_TX_U_BD_UF					(1 << 8)
#define LIRQ_TX_M_BD_UF					(1 << 9)
#define LIRQ_TX_SW_DIS					(1 << 10)
#define LIRQ_RX_U_BD_OV					(1 << 11)
#define LIRQ_RX_M_BD_OV					(1 << 12)
#define LIRQ_RX_MAXLENE					(1 << 13)
#define LIRQ_RX_U_BOV					(1 << 14)
#define LIRQ_RX_M_BOV					(1 << 15)
#define LIRQ_RX_CRC10E					(1 << 16)
#define LIRQ_RX_CRC32E					(1 << 17)
#define LIRQ_RX_LENE					(1 << 18)
#define LIRQ_GET_VC_NO(x)				(uint8) (((x) & 0x01f00000) >> 20)
#define LIRQ_RX_INACT_VC				(1 << 30)
#define LIRQ_IRQ_Q_FULL					(1 << 31)

/*******************************
 * Data type of ATM vc control *
 *******************************/
#if defined (TC3162L2) || defined(CONFIG_MIPS_TC3262)
#define ENCAP_NONE			0
#define ENCAP_RAW			ENCAP_NONE /* raw, or packet, mode */
#define ENCAP_PPP			1		/* PPP, or RFC-1661, not currently used *//* dial backup */
#define ENCAP_POE			2		/* PPPoE, or RFC-2516 */
#define ENCAP_RFC1483		3		/* MPOA */
#define ENCAP_RFC2364		4		/* PPP over AAL5 */
#define ENCAP_MER			5		/* MAC-Encap-routing */
#define ENCAP_ETHER			8		/* PPP over ethernet */

#define MUX_NONE   		0
#define MUX_LLC	  		1
#define MUX_VC	  		2

#define MODE_NONE		0
#define MODE_ROUTER	1
#define MODE_BRIDGE	2
#endif
typedef struct qosProfile_s {
	uint16 type;	/* CBR, VBR, UBR */
	uint16 pcr;
	uint16 scr;
	uint16 mbs;
#if defined (TC3162L2) || defined(CONFIG_MIPS_TC3262)
	uint16	mode;					/* mode, router or bridge */
	uint16	encapType;				/* encapsulation type */
	uint16	muxType;				/* multiplex type */
#endif
} qosProfile_t;

typedef struct atmConfig_s {
	uint8 vcNumber;						/* Number of opened VC (0-20) */
	uint8 openFlag[ATM_VC_MAX];			/* Flag to indicate if this vc is opened */
	uint8 vpi[ATM_VC_MAX];				/* VPI value of each VC */
	uint16 vci[ATM_VC_MAX];				/* VCI value of each VC */
	struct atm_vcc *vcc[ATM_VC_MAX];	/* Link to vcc structure */
} atmConfig_t;

/********************************************
 * Data type and constants of ATM Tx Module *
 ********************************************/
#define TSARM_TX_DESCR_VALID			1 << 0
#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262)
#define TSARM_TX_RAW_EN				(1 << 3)
#define TSARM_TX_VLAN_EN				1 << 13
#define TSARM_TX_DESCR_EOR			1 << 15
#define TSARM_TX_DESCR_BD_GAP(x)		(x)<<6	
#define TSARM_TX_VLAN_TAG(x)			(x)<<0

#define TSARM_TX_DATA_BD_GAP				1
#define TSARM_TX_CC_BD_GAP				0
#else
#define TSARM_TX_DESCR_SOS				1 << 1
#define TSARM_TX_DESCR_EOS				1 << 2
#endif

#define ATM_TX_CC_DESCR_NUMMAX			4
#if defined (TC3162L2) || defined(CONFIG_MIPS_TC3262)
#if defined(TCSUPPORT_CT)
#if defined(TCSUPPORT_CT_QOS)
#define ATM_TX_VC_DESCR_P0_NUMMAX			8
#define ATM_TX_VC_DESCR_P1_NUMMAX			8
#define ATM_TX_VC_DESCR_P2_NUMMAX			48//72//16//48//16
#define ATM_TX_VC_DESCR_P3_NUMMAX			48//64//240//160//64//80
#else
#define ATM_TX_VC_DESCR_P0_NUMMAX			16
#define ATM_TX_VC_DESCR_P1_NUMMAX			16
#define ATM_TX_VC_DESCR_P2_NUMMAX			16
#define ATM_TX_VC_DESCR_P3_NUMMAX			16
#endif
#else
#ifdef TCSUPPORT_QOS
#define ATM_TX_VC_DESCR_P0_NUMMAX			16
#define ATM_TX_VC_DESCR_P1_NUMMAX			40//16
#define ATM_TX_VC_DESCR_P2_NUMMAX			16
#define ATM_TX_VC_DESCR_P3_NUMMAX			48//16
#else
#define ATM_TX_VC_DESCR_P0_NUMMAX			16
#define ATM_TX_VC_DESCR_P1_NUMMAX			16
#define ATM_TX_VC_DESCR_P2_NUMMAX			16
#define ATM_TX_VC_DESCR_P3_NUMMAX			16
#endif
#endif

//now, each priority should have different num of descriptors
//#define ATM_TX_VC_DESCR_NUMMAX			16	//16 * 4 = 64
#else
#define ATM_TX_VC_DESCR_NUMMAX			32
#endif

#define ATM_TX_BUF_RELEASE_THRESHOLD	4
#if defined (TC3162L2)|| defined(CONFIG_MIPS_TC3262)
#define ATM_TX_BUF_MAX_SIZE				100
#else
#define ATM_TX_BUF_MAX_SIZE				160
#endif
#define ATM_TX_QUE_MAX_SIZE				4
#define ATM_TX_FULLQUE_MAX_SIZE			16

#define ATM_TX_DESCR0_AAL5_END			1 << 2
/* The following definition share the same bit with AAL5 */
#define ATM_TX_DESCR0_OAMCRC			1 << 2
#define ATM_TX_DESCR0_OWNBY_DMA			1 << 4

/* definition of QOS, the same definition in smt11_5 */
#define ABR					1
#define CBR					2
#define UBR					3
#define VBR					4
#define nrtVBR				5
#ifdef SAR_VERIFY
#define UBRPlus				6
#define GFR					7
/* thus MFS could be set by user according to standard, but we don't wanna be so */ 
#define ATM_QOS_GFR_MFS	35
#endif

#define TSARM_QOS_CBR						0
#define TSARM_QOS_UBR						1
#if defined (TC3162L2) || defined(CONFIG_MIPS_TC3262)
#define TSARM_QOS_rtVBR						2
#define TSARM_QOS_nrtVBR					4
#define TSARM_QOS_GFR						3
#ifdef SAR_VERIFY
#define TSARM_QOS_UBRPLUS					5
#endif
#else
#define TSARM_QOS_rtVBR						2
#define TSARM_QOS_nrtVBR					3
#endif

typedef struct atmTxCcDescr_s {
	uint32 tdes0;
	uint32 tdes1;
	uint32 tdes2;
	uint32 tdes3;
} atmTxCcDescr_t;

typedef struct atmTxDescr_s {
	uint32 tdes0;
	uint32 tdes1;
	uint32 tdes2;
	uint32 tdes3;
	struct sk_buff *skb;
//because of TSARM_TX_DATA_BD_GAP=1
#if !defined(TC3162L2) && !defined(CONFIG_MIPS_TC3262)
	uint32 rsv[3];
#endif
} atmTxDescr_t;

#if defined (TC3162L2) || defined(CONFIG_MIPS_TC3262) 
typedef struct atmTxCcDescrPool_s {
	atmTxCcDescr_t txCcDescrPool[ATM_TX_CC_DESCR_NUMMAX];
} atmTxCcDescrPool_t;
#else
typedef struct atmTxCcDescrPool_s {
	atmTxCcDescr_t txCcDescrBuf[ATM_TX_CC_DESCR_NUMMAX];
} atmTxCcDescrPool_t;
#endif

#if defined (TC3162L2) || defined(CONFIG_MIPS_TC3262) 
typedef struct atmTxDescrPool_s {
	atmTxDescr_t *txP0DescrPool;
	atmTxDescr_t *txP1DescrPool;
	atmTxDescr_t *txP2DescrPool;
	atmTxDescr_t *txP3DescrPool;
	atmTxCcDescr_t *txOamDescrPool;
	uint32 *txPriDescrPoolPhysicalAddr;
} atmTxDescrPool_t;
#else
typedef struct atmTxDescrPool_s {
	atmTxDescr_t txDescrBuf[ATM_TX_VC_DESCR_NUMMAX];
} atmTxDescrPool_t;
#endif

/********************************************
 * Data type and constants of ATM Rx Module *
 ********************************************/
#define TSARM_RX_DESCR_VALID			1 << 0
#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262) 
#define TSARM_RX_DESCR_BD_GAP(x)			(x)<<6
#define TSARM_RX_DESCR_MACFCS		1 << 10
#define TSARM_RX_DESCR_MPOAER		1 << 11
#define TSARM_RX_DESCR_VLAN			1 << 13
#define TSARM_RX_DESCR_EOR			1 << 15

#define TSARM_RX_DATA_BD_GAP				1
#define TSARM_RX_CC_BD_GAP				0
#endif


#if defined(CHINA_NM)
#define ATM_RX_CC_DESCR_NUMMAX			64
#else
#define ATM_RX_CC_DESCR_NUMMAX			64
#endif
#ifdef SAR_POLLING
#define ATM_RX_VC_DESCR_NUMMAX			48
#else
#if defined(TCSUPPORT_CT)
#define ATM_RX_VC_DESCR_NUMMAX			32
#else
#define ATM_RX_VC_DESCR_NUMMAX			20
#endif
#endif

typedef struct atmRxCcDescr_s {
	uint32 rdes0;
	uint32 rdes1;
	uint32 rdes2;
	uint32 rdes3;
} atmRxCcDescr_t;

typedef struct atmRxDescr_s {
	uint32 rdes0;
	uint32 rdes1;
	uint32 rdes2;
	uint32 rdes3;
	struct sk_buff *skb;
//because of TSARM_RX_DATA_BD_GAP=1
#if !defined(TC3162L2) && !defined(CONFIG_MIPS_TC3262)
	uint32 rsv[3];
#endif
} atmRxDescr_t;

#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262) 
typedef struct atmRxCcDescrPool_s {
	atmRxCcDescr_t rxCcDescrPool[ATM_RX_CC_DESCR_NUMMAX];
} atmRxCcDescrPool_t;

typedef struct atmRxDescrPool_s {
	atmRxDescr_t rxDescrPool[ATM_RX_VC_DESCR_NUMMAX];
	atmRxCcDescr_t rxOamDescrPool[ATM_RX_CC_DESCR_NUMMAX];
} atmRxDescrPool_t;
#else
typedef struct atmRxCcDescrPool_s {
	atmRxCcDescr_t rxCcDescrBuf[ATM_RX_CC_DESCR_NUMMAX];
} atmRxCcDescrPool_t;

typedef struct atmRxDescrPool_s {
	atmRxDescr_t rxDescrBuf[ATM_RX_VC_DESCR_NUMMAX];
} atmRxDescrPool_t;
#endif

typedef struct atmCellPayload_s {
	uint32 word[12];
} atmCellPayload_t;

#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262) 
typedef struct atmRxOamDescrPool_s {
	atmCellPayload_t rxOamDescrPool[ATM_RX_CC_DESCR_NUMMAX];
} atmRxOamDescrPool_t;
#else
typedef struct atmRxOamDescrPool_s {
	atmCellPayload_t rxOamDescrBuf[ATM_RX_CC_DESCR_NUMMAX];
} atmRxOamDescrPool_t;
#endif

/****************************************
 * Data type and constants of OAM cells *
 ****************************************/
/* ----- OAM cell type ----- */
#define OAM_FAULT_MANAGEMENT			0x1

/* ----- OAM function type ----- */
#define OAM_AIS							0x0
#define OAM_RDI							0x1
#define OAM_CC							0x4
#define OAM_LOOPBACK					0x8

typedef struct atmCell_s {
	uint32 word[13];
	#ifdef DMA_API
	uint32 rsv[3];
	#endif
} atmCell_t;

typedef struct atmRxCcCell_s {
	uint32 word[13];
	uint32 reserved[11];
} atmRxCcCell_t;

typedef struct atmOamCell_s{
	uint32 gfc: 4;
	uint32 vpi: 8;
	uint32 vci: 16;
	uint32 pti: 3;
	uint32 clp: 1;
	uint32 oamCellType: 4;
	uint32 oamFuncType: 4;
	uint8 payload[45];
} atmOamCell_t;

typedef struct atmOamCellPayload_s{
	uint32 oamCellType: 4;
	uint32 oamFuncType: 4;
	uint8 payload[45];
} atmOamCellPayload_t;

/* ----- MIB-II ----- */
typedef struct atmMIB_II_s {
	uint32 ifAdminStatus;		/* MIB adminstatus */
    uint32 inPkts;				/* Receive Pkts */
    uint32 inDiscards;			/* Receive Discard Packets */
    uint32 inErrors;			/* Receive Error Packets */
    uint32 inCrcErr;
    uint32 inBufErr;
    uint32 inDMATaskEnd;
    uint32 outPkts;				/* Transmit Pkts */
    uint32 outDiscards;			/* Transmit Discard Packets */
    uint32 outErrors;			/* Transmit Error Packets */
    uint32 inF4Pkts;			/* Receive F4 Packets */
    uint32 inF5Pkts;			/* Receive F5 Packets */
    uint32 outF4Pkts;			/* Transmit F4 Packets */
    uint32 outF5Pkts;			/* Transmit F5 Packets */
    uint32 softRstCnt;

	/* lino: move new fields here to be compatible with the old structure */
    uint32 inBufOverflow;
    uint32 inBufMaxLenErr;
    uint32 inBufLenErr;
    uint32 inBufDescrOverflow;
#if defined(TC3162L2) || defined(CONFIG_MIPS_TC3262) 
	uint32 inCrc10Err;
	uint32 inMngBufOverflow;
	uint32 inCcBufOverflow;
	uint32 inMngBufDescrOverflow;
	uint32 inCcBufDescrOverflow;
	uint32 inMpoaErr;
	uint32 inVlanHit;
	uint32 inCcUDoneErr;
	uint32 inCcUBDOV;
#endif
#if defined(L2SARMSG)
	uint32 rxEnQueueNum;  /* Number of packets enqueued in atmRxBufEnQue() */
	uint32 rxDeQueueNum;  /* Number of packets dequeued in atmRxBufDeQue() */
	uint32 txEnQueueNum;  /* Number of packets enqueued in atmTxBufEnQue() */
	uint32 txDeQueueNum;  /* Number of packets dequeued in atmTxBufDeQue() */

	uint32	TxUDone;
	uint32	RxUDone;
	uint32	UpperToSar;
	uint32	SarToUpper;
#endif
	//yzwang_20091125
	uint32  inBytes;
	uint32  outBytes;
	uint32  irqFull;
} atmMIB_II_t;

typedef struct atmCtrl_s {
	atmMIB_II_t	MIB_II;		/* MIB-II */	
} atmCtrl_t;
/* ----- atminit.c ----- */
extern atmCtrl_t *atm_p;

/***************************
 * APIs for ATM SAR module *
 ***************************/
void atmInit(void);
int atmAal5VcOpen(uint8 vpi, uint16 vci, qosProfile_t *qos_p, struct atm_vcc *vcc);
uint8 atmAal5VcClose(uint8 vpi, uint16 vci);
uint8 atmAal5DataReq(struct sk_buff *skb, uint8 vpi, uint16 vci);
uint8 atmCcDataReq(uint8 *data_p);
int atmRegDump(char *buf, uint8 vc);

#endif

