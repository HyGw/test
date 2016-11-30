#ifndef _TCMACDRV_H
#define _TCMACDRV_H


#ifdef TC3162_MAC_NAPI
#define MAC_NAPI_WEIGHT     32
#define MAC_TXDESCP_NO		64	/* Max tx buffer Cnts , default=64 */
#define MAC_RXDESCP_NO		80	/* Max rx buffer Cnts , default=64 */
#else
#define MAC_TXDESCP_NO		64	/* Max tx buffer Cnts , default=64 */
#define MAC_RXDESCP_NO		64	/* Max rx buffer Cnts , default=64 */
#endif

#define MAC_RXDESCP_SIZE	20	/* 5 DWords */
#define MAC_TXDESCP_SIZE	20	/* 5 DWords */

#define TX_BUF_RELEASE_THRESHOLD 8  // default:8
#define RX_DMA_THRESHOLD  8  // must be multiples of 4

#define K0_TO_K1(x)		( ((uint32)x) | 0xa0000000)
#define K1_TO_PHY(x)	( ((uint32)x) & 0x1fffffff)
#define PCI_MAP_HIGH				0
#define PHY_ACCESS_CHECK			0
#define NEGOTIATION_CHECK_BY_LED	0
#define ENET_SIM_SETTING			0
#define DMA_TEST_MODE_OFF			0
#define MAC_CONN_MII_FORCE			0
#define CAM_PROMISCUOUS_MODE		0
#define B2LHWSUPPORT				0
#define BSLIOSWAP					0

/* ----- 100/10Mbps Mode ----- */
//#define AUTO_SENSE		  			0
//#define TEN_MBPS             		100000
//#define ONE_HUNDRED_MBPS    		1000000
#define MIIDR_RESTART_AUTO_NEGOTIATE	0x0200
#define MIIDR_ENABLE_AUTO_NEGOTIATE		0x1000
#define MIIDR_10MBps					0x0000
#define MIIDR_100MBps					0x2000
#define MIIDR_AUTO_NEGOTIATE			( MIIDR_RESTART_AUTO_NEGOTIATE| MIIDR_ENABLE_AUTO_NEGOTIATE )
#define MIIDR_FULL_DUPLEX				0x0100
#define MIIDR_HALF_DUPLEX				0x0000
#define MIIAR_WRITE						0x2
#define MIIAR_READ						0x0
#define MIIAR_IS_BUSY					0x1
#define	LINK_SPEED						AUTO_SENSE


#define MACCR_TE	( 1<<13 )
#define MACCR_RE	( 1<<1 )

/* ----- Receive Data-Block Num ----- */
#define MAX_DATA_BLOCK		32

/* ----- Transmit Packet-Block Num ----- */
#define TXSAVE_MAXNUM		48

#define BOOLEAN				uint8

/* ----- Error number for initializing Mac ----- */
#define SUCCESS_INIT		0
#define ERR_INIT_PCI		1
#define ERR_INIT_MEM		2
#define ERR_PHY_LINK		3
#define ERR_REG_INIT		4   // chiawei

/* ----- Ethernet Common Definition ----- */
#define ETH_CRC_SIZE  			4		/* CRC Field Size */

/* ----- PHY-Chip Definition ----- */
#define PHY_WRITE 				0x0400
#define PHY_BUSY 				0x0800

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

/************************************************************************
*        i n S i l c o n   R E L A T E D   I N F O R M A T I O N
*************************************************************************
*/

/* ----- PCI-Configuration Registers ----- */
#define TC35815_VENDOR_ID				0x102F		/* Vendor-ID */
#define TC35815_DEVICE_ID				0x0030		/* Device-ID */
#define PCI_COMMAND_DEFAULT_VALUE		0x05		/* Command Register Value */
#define PCI_LATENCY_TIMER_DEFAULT_VALUE	0x0000ff40	/* Latency & CacheLine Size Value */

#define IFDESCR 				"PCI-Based 10/100 Mbps LAN"
#define MAX_CARDNUM				1					/* Support Device Number */


/* ----- ENET_IER Register parameter ----- */
#define IER_NIS			(1<<16)
#define IER_RS			(1<<8)
#define IER_RU			(1<<7)
#define IER_RI			(1<<6)
#define IER_TI			(1)

/* ----- ENET_SR Register parameter ----- */
#define SR_ERR_INDICATION ((1<<13)|(1<<7))	/* fatal bus err, receive buffer unvaliable */

/* ----- ENET_CR Register parameter ----- */
#define MAC_TX_ENABLE	(1<<13)
#define MAC_RX_ENABLE	(1<<1)

/* ----- MAC_Ctl Register parameter ----- */
#define MAC_HALF_DUPLEX			0x00
#define MAC_FULL_DUPLEX			0x08
#define MAC_FULLDUP				MAC_HALF_DUPLEX

/* ----- MACCR Register's parameter ----- */
#define MACCR_RA				(1<<31)
#define MACCR_INT_LOOPBACK		(1<<21)
#define MACCR_FULLDUPLX_MODE	(1<<20)
#define MACCR_PASS_ALL_MULTICAST	(1<<19)
#define MACCR_PROMISCUOUS_MODE	(1<<18)
#define MACCR_INVERT_FILTERING	(1<<17)
#define MACCR_PASS_BAD_FRAME	(1<<16)
#define MACCR_HASH_ONLY			(1<<15)
#define MACCR_DISABLE_BROADCAST (1<<11)
#define MACCR_RX_ENABLE			(1<<2)

/*PHYCR Control Register*/
#define PHYOR_DATA 		(0xffff<<0)	
#define PHYOR_ADDR_SHIFT	16
#define PHYOR_REG_SHIFT		21
#define PHYOR_READ		(1<<26)
#define PHYOR_WRITE		(1<<27)
#define PHYOR_MDC		28

#define NEW_PHYOR_DATA		(0xffff<<0)	
#define NEW_PHYOR_REG_SHIFT	16
#define NEW_PHYOR_ADDR_SHIFT	21
#define NEW_PHYOR_OP_SHIFT	26
#define NEW_PHYOR_OP_WR		(1)
#define NEW_PHYOR_OP_RD		(2)
#define NEW_PHYOR_OP_EEE_FA				(0)/*Filed address*/
#define NEW_PHYOR_OP_EEE_WR				(1)
#define NEW_PHYOR_OP_EEE_POST_RD		(2)
#define NEW_PHYOR_OP_EEE_RD				(3)
#define NEW_PHYOR_ST_SHIFT	28
#define NEW_PHYOR_ST_IEEE_NEW	(0)
#define NEW_PHYOR_ST_IEEE_OLD	(1)
#define NEW_PHYOR_BUSY		(1<<31)

#define writeMacReg(reg, value) writePciMapReg(PCI_IO_PHYSADDR, (reg), (value))
//#define readMacReg(reg)	readPciMapReg(PCI_IO_PHYSADDR, (reg))

#define MACCTL_LOOPBACK		0x00000010

/* ----- Descriptor ----- */
#define DESCP_OWNED_BY_DMA_BIT		1
#define DESCP_OWNED_BY_DMA_WORD		0x00000080
#define DESCP_OWNED_BY_CPU_WORD		0x00000000

#define MAC_STATISTIC_ON	1
#define MAC_STATISTIC_OFF	0


#define RX_ALIGN_ERR		0x00000006
#define RX_SYMBOL_ERR		0x0000000A

/* ----- Number of Ethernet address to be filtered by CAM ----- */
#define ENET_CAM_FILTER_ENTRIES			1


/*****************************************************************
**     ETH debug print message function
******************************************************************/


extern int DbgSettings;

#define DBG_LB       (1 << 18)
#define DBG_HW       (1 << 19)
#define DBG_MSG      (1 << 20)
#define DBG_DES      (1 << 21)
#define DBG_REG      (1 << 22)
#define DBG_STAT     (1 << 23)
#define DBG_PNP      (1 << 24)
#define DBG_TIME     (1 << 25)
#define DBG_DBG      (1 << 26)
#define DBG_OUT      (1 << 27)
#define DBG_BUF      (1 << 28)
#define DBG_BUFS     (1 << 28)
#define DBG_FUNCTION (1 << 29)
#define DBG_FUNC     (1 << 29)
#define DBG_WARN     (1 << 30)
#define DBG_WARNING  (1 << 30)
#define DBG_ERROR    (1 << 31)
#define DBG_ERR      (1 << 31)

//#define ETH_DBG			1

//#if defined(ETH_DBG)
#if 1
#define DEBUGMSG(dbgs,format) 	do { } while(0)
#define DEBUGMSGW(dbgs,format)	do { } while(0)
#define DEBUGMSGB(dbgs,format)	do { } while(0)
#define DEBUGMSGL(dbgs,format)	do { } while(0)
#define ETH_DUMP(dbgs,format )	do { } while(0)
#else
#define DEBUGMSG(dbgs,format) if(dbgs&DbgSettings) dbg_pline_1 format
#define DEBUGMSGW(dbgs,format)if(dbgs&DbgSettings) dbg_plinew_1 format
#define DEBUGMSGB(dbgs,format)if(dbgs&DbgSettings) dbg_plineb_1 format
#define DEBUGMSGL(dbgs,format)if(dbgs&DbgSettings) dbg_plinel_1 format
#define ETH_DUMP(dbgs,format )if(dbgs&DbgSettings) ETH_PrintBuf format
#endif

extern int ether_dbg;
#define Ether_DBG if(ether_dbg&0x01) DBG_pline_1
#define Ether_DBG_W if(ether_dbg&0x01) DBG_plinew_1
#define Ether_DBG_B if(ether_dbg&0x01) DBG_plineb_1
#define Ether_DBG_L if(ether_dbg&0x01) DBG_plinel_1

/***************************
 * Mac Transmit Descriptor *
 ***************************/

typedef union
{
	struct
	{
    uint32 ownerDma     : 1;
  #ifdef TC3162L2
    uint32 reserved     : 27;
    uint32 txpktUnderRunAbort : 1;
    uint32 txpktUnderRun 			: 1;
  #else
  	uint32 reserved     : 29;
  #endif
    uint32 txPktExsCol  : 1;
    uint32 txPktLateCol : 1;
	} bits;
	uint32 word;
} tdes0_t;

typedef union
{
	struct
	{
	#ifdef TC3162L2
	  uint32  sw_txtag_en : 1;
	#else
    uint32  edotr     : 1;
  #endif
    uint32  txic      : 1;
    uint32  tx2fic    : 1;
  #ifdef TC3162L2
    uint32  ipcs_en   : 1;
    uint32  vtag_en   : 1;
    uint32  vlan_tag  : 16;
  #else
    uint32  fts       : 1;
    uint32  lts       : 1;
    uint32  reserved  : 16;
  #endif
    uint32  txBufSize : 11;
	} bits;
	uint32 word;
} tdes1_t;

typedef union
{
	uint32 buf_p;
	uint32 word;
} tdes2_t;

typedef union
{
	uint32 sw_txtag;
	uint32 word;
} tdes3_t;

typedef struct macTxDescr_s
{
	tdes0_t tdes0;
	tdes1_t tdes1;
	tdes2_t tdes2;
#ifdef TC3162L2
  tdes3_t tdes3;
#endif
	struct sk_buff *skb;
} macTxDescr_t;

/**************************
 * Mac Receive Descriptor *
 **************************/

typedef union
{
	struct
	{
    uint32  ownerDma  : 1;
  #ifdef TC3162L2
    uint32  vlan_hit  : 1;
  #else
    uint32  reserved  : 1;
  #endif
    uint32  frs       : 1;
    uint32  lrs       : 1;
  #ifdef TC3162L2
    uint32  ip_err    : 1;
    uint32  l3id      : 3;
    uint32  eth_pkt   : 1;
  #else
    uint32  reserved_1: 5;
  #endif
    uint32  rx_col    : 1;
    uint32  runt      : 1;
    uint32  ftl       : 1;
    uint32  crc_err   : 1;
    uint32  rx_err    : 1;
    uint32  broadcast : 1;
    uint32  multicast : 1;
  #ifdef TC3162L2
    uint32  l4id      : 3;
    uint32  priority  : 2;
  #else
    uint32  reserved_2: 5;
  #endif
    uint32  rfl       : 11;
	} bits;
	uint32 word;
} rdes0_t;

typedef union
{
	struct
	{
	#ifdef TC3162L2
	  uint32 ipv4_frag : 1;
	#else
		uint32 edorr    : 1;
  #endif
  #ifdef TC3162L2
    uint32 sw_rxtag_hit : 1;
    uint32 pkt_cut  : 1;
    uint32 rxlen_err: 1;
    uint32 drib_err : 1;
    uint32 vlan_tag : 16;
  #else
		uint32 reserved : 20;
  #endif
		uint32 rxBufSize:11;
	} bits;
	uint32 word;
} rdes1_t;

typedef union
{
	uint32 buf_p;
	uint32 word;
} rdes2_t;

#ifdef TC3162L2
typedef union
{
	uint32 sw_rxtag;
	uint32 word;
} rdes3_t;
#endif

typedef struct macRxDescr_s
{
	rdes0_t rdes0;
	rdes1_t rdes1;
	rdes2_t rdes2;
#ifdef TC3162L2
  rdes3_t rdes3;
#endif
	struct sk_buff *skb;
} macRxDescr_t;

// --- CR_MAC_PHYCR ---
typedef union
{
	struct
	{
	uint32 reserved  : 4; // reserved
	uint32  miiwr    : 1; // MIIWR
	uint32  miird    : 1; // MIIRD
	uint32  regad    : 5; // REGAD
	uint32  phyad    : 5; // PHYAD
	uint32  miirdata : 16;  // MIIRDATA
  } bits;
  uint32 value;
} macPhyCtrlReg_t;


// --- CR_MAC_PHYWDATA ---
typedef union
{
  struct
  {
    uint32 reserved : 16; // reserved
    uint32 miiwdata : 16; // MIIWDATA
  } bits;
  uint32 value;
} macPhyWDataReg_t;

// --- CR_MAC_MACCR ---
typedef union
{
  struct
  {
  #ifdef TC3162L2
    uint32  reserved        : 1;
    uint32  rx_flow_ctrl    : 1; // RX FLOW CONTROL
    uint32  mii_tx_edge	    : 1; // MII TX EDGE
    uint32  drop_fragpkt    : 1; // DROP_FRAG_PKT
    uint32  mf_pre_suppress : 1; // MANAGE_FRAME
    uint32  drop_col        : 1; // DROP_COLLISION_PKT
    uint32  rxpause_pass    : 1; // RX_ACCEPT_PAUSE_FRAME
    uint32  no_backoff      : 1; // NO_BACKOFF
    uint32  txfifo_thr      : 4; // TXFIFO_THRESHOLD
    uint32  rx_untag        : 1; // RX_UNTAG_VLAN
    uint32  drop_local      : 1; // DROP_LOCAL_PKT
  #else
    uint32  reserved    	: 14;
  #endif
    uint32  rx_broadpkt   : 1;   // RX_BROADPKT
    uint32  rx_multipkt   : 1;   // RX_MULTIPKT
    uint32  fulltx        : 1;   // FULLTX
    uint32  crc_apd       : 1;   // CRC_APD
    uint32  mdc_sel       : 1;   // MDC_SEL
    uint32  rcv_all       : 1;   // RCV_ALL
    uint32  rx_ftl        : 1;   // RX_FTL
    uint32  rx_runt       : 1;   // RX_RUNT
    uint32  ht_multi_en   : 1;   // HT_MULTI_EN
    uint32  rcv_en        : 1;   // RCV_EN
  #ifdef TC3162L2
    uint32  l2_mode       : 1;   // L2 mode enable
  #else
    uint32  rsv           : 1;   // reserved
  #endif
    uint32  enrx_in_halftx: 1;   // ENRX_IN_HALFTX
    uint32  xmt_en        : 1;   // XMT_EN
    uint32  crc_dis       : 1;   // CRC_DIS
    uint32  loop_en       : 1;   // LOOP_EN
    uint32  sw_rst        : 1;   // SW_RST
    uint32  rdma_en       : 1;   // RDMA_EN
    uint32  xdma_en       : 1;   // XDMA_EN
  } bits;
  uint32 value;
} macCtrlReg_t;


// --- CR_MAC_IMR ---
typedef  union
{
  struct
  {
  #ifdef TC3162L2
    uint32 reserved           : 18; // reserved
    uint32 rpkt_ok_by_thrhold : 1;
    uint32 rpkt_ok_by_timeout : 1;
    uint32 xpkt_ok_by_thrhold : 1;
    uint32 xpkt_ok_by_timeout : 1;
  #else
    uint32 reserved    : 22; // reserved
  #endif
    uint32 physts_chg  : 1;  // PHYSTS_CHG
    uint32 ahb_err     : 1;  // AHB_ERR
    uint32 rpkt_lost   : 1;  // RPKT_LOST
    uint32 rpkt_save   : 1;  // RPKT_SAVE
    uint32 xpkt_lost   : 1;  // XPKT_LOST
    uint32 xpkt_finish : 1;  // XPKT_FINISH
    uint32 notxbuf     : 1;  // NOTXBUF
    uint32 xpkt_save   : 1;  // XPKT_SAVE
    uint32 norxbuf     : 1;  // NORXBUF
    uint32 rpkt_finish : 1;  // RPKT_FINISH
  } bits;
  uint32 value;
} macIntrMaskReg_t;

// --- CR_MAC_ISR ---
typedef  union
{
  struct
  {
  #ifdef TC3162L2
    #ifdef EEE_SUPPORT
    uint32 reserved           : 13; // reserved
    uint32 tx_leave_psm : 1;
    uint32 tx_enter_psm : 1;
    uint32 rx_enter_psm : 1;
    uint32 rx_leave_psm : 1;
    uint32 underrun_err : 1;
    #else
    uint32 reserved           : 18; // reserved
    #endif
    uint32 rpkt_ok_by_thrhold : 1;
    uint32 rpkt_ok_by_timeout : 1;
    uint32 xpkt_ok_by_thrhold : 1;
    uint32 xpkt_ok_by_timeout : 1;
  #else
    uint32 reserved    : 22; // reserved
  #endif
    uint32 physts_chg  : 1;  // PHYSTS_CHG
    uint32 ahb_err     : 1;  // AHB_ERR
    uint32 rpkt_lost   : 1;  // RPKT_LOST
    uint32 rpkt_save   : 1;  // RPKT_SAVE
    uint32 xpkt_lost   : 1;  // XPKT_LOST
    uint32 xpkt_finish : 1;  // XPKT_FINISH
    uint32 notxbuf     : 1;  // NOTXBUF
    uint32 xpkt_save   : 1;  // XPKT_SAVE
    uint32 norxbuf     : 1;  // NORXBUF
    uint32 rpkt_finish : 1;  // RPKT_FINISH
  } bits;
  uint32 value;
} macIntrStatusReg_t;


//
// --- PHY Advertisement register---------
//
typedef  union
  {
    uint32 value;
    struct
    {
      uint32  rsv2            	:16;
      uint32  next_page             : 1;
      uint32  acknowledge           : 1;
      uint32  rsv1                  : 3;
      uint32  flow_control          : 1;
      uint32  rsv0                  : 1;
      uint32  full_duplex_100baseT  : 1;
      uint32  half_duplex_100baseT  : 1;
      uint32  full_duplex_10baseT   : 1;
      uint32  half_duplex_10baseT   : 1;
      uint32  selector_field        : 5;
    }bits;
  }macPhyAdvertismentReg_t;


// --- PHY Control Register ---
typedef  union
  {
    uint32 value;
    struct
    {
      uint32  rsv1 : 16;
      uint32  phy_reset       : 1;
      uint32  loopback        : 1;
      uint32  speed_select    : 1;
      unsigned  enable_ANEN : 1;
      uint32  power_down      : 1;
      uint32  phy_isolate     : 1;
      uint32  reset_ANEN      : 1;
      uint32  duplex_mode     : 1;
      uint32  collision_test  : 1;
      uint32  rsv             : 7;
    }bits;
}macPhyCR_t;


typedef  union
 {
    uint32  value;
    struct
    {
      uint32  rsv1: 17;
      uint32  extend_capability     : 1;
      uint32  jabber_detect         : 1;
      uint32  link_status           : 1;
      uint32  ANEN_ability          : 1;
      uint32  remote_fault          : 1;
      uint32  ANEN_complete         : 1;
      uint32  MF_preamble_suppress  : 1;
      uint32  rsv                   : 4;
      uint32  half_duplex_10baseT   : 1;
      uint32  full_duplex_10baseT   : 1;
      uint32  half_duplex_100baseT  : 1;
      uint32  full_duplex_100baseT  : 1;
    }bits;
}phyStatusReg_t;


// --- CR_MAC_ITC ---
typedef  union
{
  uint32  value;
  struct
  {
    uint32  reserved       : 16;  // Reserved
    uint32  txint_time_sel : 1;   // TXINT_TIME_SEL
    uint32  txint_thr      : 3;   // TXINT_THR
    uint32  txint_cnt      : 4;   // TXINT_CNT
    uint32  rxint_time_sel : 1;   // RXINT_TIME_SEL
    uint32  rxint_thr      : 3;   // RXINT_THR
    uint32  rxint_cnt      : 4;   // RXINT_CNT
  } bits;
} mac_intr_timer_ctrl_reg_t;

#ifdef TC3162L2
// None
#else
// --- CR_MAC_APTC ---
typedef  union
{
  uint32  value;
  struct
  {
    uint32  reserved        : 9;  // Reserved
    uint32  txpoll_time_sel : 1;  // TXPOLL_TIME_SEL
    uint32  txpoll_cnt      : 4;  // TXPOLL_CNT
    uint32  rsv             : 3;
    uint32  rxpoll_time_sel : 1;  // RXPOLL_TIME_SEL
    uint32  rxpoll_cnt      : 4;  // RXPOLL_CNT
  } bits;
} mac_autopoll_timer_ctrl_reg_t;

// --- CR_MAC_DBLAC ---
typedef union
{
  uint32  value;
  struct
  {
    uint32  reserved   :25;   // Reserved
    uint32  rx_thr_en  : 1;   // RX_THR_EN
    uint32  rxfifo_thr : 3;   // RXFIFO_THR
    uint32  incr16_en  : 1;   // INCR16_EN
    uint32  incr8_en   : 1;   // INCR8_EN
    uint32  incr4_en   : 1;   // INCR4_EN
  } bits;
} mac_dma_arb_ctrl_reg_t;
#endif

// --- Force MAC to poll Tx ---
#define   MAC_TX_POLL_DEMAND_(x) \
          *(volatile unsigned int *)(CR_MAC_TXPD) = (0xffffffff);

// --- Force MAC to poll Rx ---
#define   MAC_RX_POLL_DEMAND_(x) \
          *(volatile unsigned int *)(CR_MAC_RXPD) = (0xffffffff);

// --- setup MAC address ---
          // --- Setup MAC address ---
#define   MAC_SET_MAC_ADDRESS_(x,y) \
          *(volatile unsigned int *)(CR_MAC_LADR) = (x&0x0000ffff); \
          *(volatile unsigned int *)(CR_MAC_MADR) = (y&0xffffffff);

/*********************
 * Mac Control Block *
 *********************/

typedef struct macDrvCtrl_s
{
	macTxDescr_t *txFreeDescr_p;	/* Tx free descriptor */

	macTxDescr_t *txBusyDescr_p;	/* Tx busy descriptor */
	uint16 txQueuedFrameCnt;		/* cnts of frames haven't freed yet */
	uint32 macDrvResetCnt;			/* reset counts */
	#if 0 	/* mark HTP by kycheng */
	#ifndef HTP
	int (*PktFilter)(uint8 id, uint8 *sharemem, int length);
	int (*DrvErrorLog)(int errType, uint16 *edp);
	void (*DrvTxCompleted)(int ChanID);
	int (*DrvRxUpCall)(int ChanID, mbuf_t *mbuf);
	uint8 DevType;
	uint16 ifaceType;
	uint16 TxSignal;
	chann_t *chann_p;
	#endif
	#endif
} macDrvCtrl_t;


/* ----- Frame Block ----- */
typedef struct macFrame_s
{
	struct macFrame_s *next;
	uint8 *bufAddrp;
} macFrame_t;


/* ----- MIB-II ----- */
typedef struct macMIB_II_s
{
	uint32 ifadminstatus;		/* MIB adminstatus */
//uint8 *ifDescr;				/* MIB Interface Name */
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
typedef struct inSiliconStat_s
{
/*uint32 IntSrc_FDAExhausted;
	uint32 IntSrc_BLExhausted;
	uint32 IntSrc_DMAParity;
	uint32 IntSrc_NRA;*/

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
typedef struct macPhyLinkProfile_s
{
	uint32 linkSpeed;							/* 10Mbps or 100Mbps */
	uint32 duplexMode;							/* Half/Full Duplex Mode */
	uint32 enetMode;
	uint32 ANCompFlag;							/* auto_negotiation complete Flag */
	uint32 PollCount;							/* auto_negotiation polling check count */
} macPhyLinkProfile_t;

/* with 16 byte alignment */
#define DESCRIPTOR_POOL_SIZE	( MAC_RXDESCP_SIZE*MAC_RXDESCP_NO + MAC_TXDESCP_SIZE*MAC_TXDESCP_NO + 16 )

/* ----- Ethernet private memory pool ----- */
typedef struct macMemPool_s
{
	uint8 descrPool[DESCRIPTOR_POOL_SIZE];	/* Descr pool area */
} macMemPool_t;

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
  macStat_t           macStat;
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
#ifdef MII_INTERFACE
  struct mii_if_info mii_if;
#endif  
  spinlock_t lock;
  spinlock_t phy_lock;
  struct net_device_stats stats;
} macAdapter_t;

typedef struct phyDeviceList_s {
	uint16 companyId;
	char vendorName[30];
} phyDeviceList_t;


/************************************************************************
*       D E B U G   M A C R O S
*************************************************************************
*/
#define _DBGPRINT_		0 

#if _DBGPRINT_
#define dbgPrintStr(s)		DBG_pline_1(s)
#define dbgPrint32bit(s,v)	DBG_plinel_1(s,v)
#define dbgPrint16bit(s,v)	DBG_plinew_1(s,v)
#define dbgPrint8bit(s,v)	DBG_plineb_1(s,v)
#else
#define dbgPrintStr(s)
#define dbgPrint32bit(s,v)
#define dbgPrint16bit(s,v)
#define dbgPrint8bit(s,v)
#endif

/************************************************************************
*        F U N C T I O N   P R O T O T Y P E S
*************************************************************************
*/


#ifndef PUBLIC
#define PUBLIC
#endif /* ifndef PUBLIC */
#ifndef PRIVATE
#define PRIVATE static
#endif /* ifndef PRIVATE */

#endif /* _TCMACDRV_H */

