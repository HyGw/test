#ifndef __MAC_H
#define __MAC_H

#include <asm/tc3162.h>
#include <flashhal.h>

#include "skbuff.h"

//#define TX_DESCP_NO		64	/* 5 descriptors, = Max tx buffer Cnts ,default=64 */
//#define RX_DESCP_NO		64	/* 5 descriptors ,default=64 */

#define TX_DESCP_NO		8	/* 5 descriptors, = Max tx buffer Cnts ,default=64 */
#define RX_DESCP_NO		8	/* 5 descriptors ,default=64 */

#define MAC_DESCP_SIZE	16	/* 4 DWords */

#define TX_BUF_RELEASE_THRESHOLD 8  // default:8

#define K0_TO_K1(x)		( ((uint32)x) | 0xa0000000)
#define K1_TO_PHY(x)	( ((uint32)x) & 0x1fffffff)

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

#define DEBUGMSG(dbgs,format) 	do { } while(0)
#define DEBUGMSGW(dbgs,format)	do { } while(0)
#define DEBUGMSGB(dbgs,format)	do { } while(0)
#define DEBUGMSGL(dbgs,format)	do { } while(0)
#define ETH_DUMP(dbgs,format )	do { } while(0)

#if  defined(TCSUPPORT_CT_PON) || (defined(TCSUPPORT_MULTI_UPGRADE) && defined(TR068_LED) && (defined(TCSUPPORT_CT_PON_CY) || defined(TCSUPPORT_CT_PON_C7))) || defined(TCSUPPORT_CT_C5_HEN_SFU) 
#define LED_OEN(x)		do { 	if(x > 31){							\
									if(x > 47){						\
										regWrite32(CR_GPIO_CTRL3,regRead32(CR_GPIO_CTRL3)|(1<<((x-48)*2)));	\
									}else{							\
										regWrite32(CR_GPIO_CTRL2,regRead32(CR_GPIO_CTRL2)|(1<<((x-32)*2)));	\
									}								\
									regWrite32(CR_GPIO_ODRAIN1,regRead32(CR_GPIO_ODRAIN1)|(1<<(x-32)));	\
								}									\
								else{								\
									if(x > 15){						\
										regWrite32(CR_GPIO_CTRL1,regRead32(CR_GPIO_CTRL1)|(1<<((x-16)*2)));	\
									}else{							\
										regWrite32(CR_GPIO_CTRL,regRead32(CR_GPIO_CTRL)|(1<<(x*2)));	\
									}								\
									regWrite32(CR_GPIO_ODRAIN,regRead32(CR_GPIO_ODRAIN)|(1<<(x)));	\
								}									\
				} while(0)

#define LED_OFF(x) 	do {								\
								if(x > 31){					\
									regWrite32(CR_GPIO_DATA1,regRead32(CR_GPIO_DATA1)|(1<<(x-32)));	\
								}else{						\
									regWrite32(CR_GPIO_DATA,regRead32(CR_GPIO_DATA)|(1<<x));	\
								}						\		
						} while (0)
#define LED_ON(x) 	do {								\
								if(x > 31){					\
									regWrite32(CR_GPIO_DATA1,regRead32(CR_GPIO_DATA1)& ~(1<<(x-32)));	\
								}else{						\
									regWrite32(CR_GPIO_DATA,regRead32(CR_GPIO_DATA)& ~(1<<x));	\
								}						\
						} while (0)
#else
#if !defined(TC3262) || !defined(RT63365)
#define LED_OFF(x)      VPint(CR_GPIO_DATA) |= (1<<(x))
#define LED_ON(x)  	VPint(CR_GPIO_DATA) &= ~(1<<(x))
#define LED_OEN(x)      do { VPint(CR_GPIO_CTRL) |= (1<<((x)*2)); VPint(CR_GPIO_ODRAIN) |= (1<<(x)); } while(0)
#else
#define LED_OFF(x) do { ledTurnOff(x); } while(0)
#define LED_ON(x)  do { ledTurnOn(x);  } while(0)
#define LED_OEN(x) do { ledTurnOen(x); } while(0)
#endif
#endif
#if defined(TCSUPPORT_MULTI_UPGRADE) || defined(TCSUPPORT_MULTI_BOOT)
#define INTERNET_GPIO	(0xff8a | flash_base)
#define ADSL_GPIO		(0xff89 | flash_base)
#ifdef TCSUPPORT_CPU_MT7520
#define SYSTEM_GPIO 	21
#else
#define SYSTEM_GPIO 	3
#endif
#endif

#if defined(TCSUPPORT_MULTI_UPGRADE)
#if !defined(TCSUPPORT_C1_TRUE)
#define MULTI_UPGRADE_GPIO	(0xff8b | flash_base)
#endif
#endif

/***************************
 * Mac Transmit Descriptor *
 ***************************/

//------------------------------------------------
//
//   TC3162
//

typedef union {
	struct {
		uint32 ownerDma: 1;
		uint32 reserved:  29;
		uint32 txPktExsCol: 1;
		uint32 txPktLateCol: 1;
	} bits;
	uint32 word;
} tdes0_t;


typedef union {
	struct {
		uint32 edotr: 1;
		uint32 txic: 1;
		uint32 tx2fic: 1;
		uint32 fts: 1;
		uint32 lts: 1;
		uint32 reserved: 16;
		uint32 txBufSize: 11;
	} bits;
	uint32 word;
} tdes1_t;

typedef union {
	uint32 buf_p;
	uint32 word;
} tdes2_t;

typedef struct macTxDescr_s {
	tdes0_t tdes0;
	tdes1_t tdes1;
	tdes2_t tdes2;
	sk_buff *skb;
} macTxDescr_t;

/**************************
 * Mac Receive Descriptor *
 **************************/

typedef union {
	struct {
		uint32 ownerDma: 1;
		uint32 reserved: 1;
		uint32 frs: 1;
		uint32 lrs: 1;
		uint32 reserved_1: 5;
		uint32 txOddNb: 1;
		uint32 runt: 1;
		uint32 ftl: 1;
		uint32 crc_err: 1;
		uint32 rx_err: 1;
		uint32 broadcast: 1;
		uint32 multicast: 1;
		uint32 reserved_2: 5;
		uint32 rfl: 11;	
	} bits;
	uint32 word;
} rdes0_t;

typedef union {
	struct {
		uint32 edorr: 1;
		uint32 reserved: 20;
		uint32 rxBufSize:11;
	} bits;
	uint32 word;
} rdes1_t;

typedef union {
	uint32 buf_p;
	uint32 word;
} rdes2_t;

typedef struct macRxDescr_s {
	rdes0_t rdes0;
	rdes1_t rdes1;
	rdes2_t rdes2;
	sk_buff *skb;
} macRxDescr_t;

//----------------------------------------------
//
//  CR_MAC_PHYCR ---
//
//----------------------------------------------

 
typedef union {
	struct {
	     uint32 reserved: 4; // reserved
	     uint32  miiwr    : 1;   // MIIWR
	     uint32  miird    : 1;   // MIIRD
	     uint32  regad    : 5;   // REGAD
	     uint32  phyad    : 5;   // PHYAD
	     uint32  miirdata : 16;  // MIIRDATA
    }bits;
    uint32  value;
}macPhyCtrlReg_t;
 

//
// --- CR_MAC_PHYWDATA ---
//

typedef  union{
    struct
    {
    	   uint32 reserved: 16;  // reserved
	   uint32  miiwdata : 16;  // MIIWDATA
    }bits;
    uint32  value;
}macPhyWDataReg_t;



// --- CR_MAC_MACCR ---
typedef union {
    struct {
      uint32  reserved    	: 14;
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
      uint32  rsv           : 1;   // reserved
      uint32  enrx_in_halftx: 1;   // ENRX_IN_HALFTX
      uint32  xmt_en        : 1;   // XMT_EN
      uint32  crc_dis       : 1;   // CRC_DIS
      uint32  loop_en       : 1;   // LOOP_EN
      uint32  sw_rst        : 1;   // SW_RST
      uint32  rdma_en       : 1;   // RDMA_EN
      uint32  xdma_en       : 1;   // XDMA_EN
    }bits;
    uint32  value;
}macCtrlReg_t;


//
// --- CR_MAC_IMR ---
//
typedef  union
  {
     struct
    { 
    	  uint32  reserved	  	: 22;    // reserved
	      uint32  physts_chg 	: 1;  // PHYSTS_CHG
	      uint32  ahb_err     	: 1;  // AHB_ERR
	      uint32  rpkt_lost   	: 1;  // RPKT_LOST
	      uint32  rpkt_save   	: 1;  // RPKT_SAVE
	      uint32  xpkt_lost   	: 1;  // XPKT_LOST
	      uint32  xpkt_finish 	: 1;  // XPKT_FINISH
	      uint32  notxbuf     	: 1;  // NOTXBUF
	      uint32  xpkt_save   	: 1;  // XPKT_SAVE
	      uint32  norxbuf     	: 1;  // NORXBUF
	      uint32  rpkt_finish 	: 1;  // RPKT_FINISH
    } bits;
    uint32  value;
} macIntrMaskReg_t;

//
// --- CR_MAC_ISR ---
//
typedef  union
  {
    struct
    {
    	      uint32  reserved:   22;  // reserved
	      uint32  physts_chg  : 1;  // PHYSTS_CHG
	      uint32  ahb_err     : 1;  // AHB_ERR
	      uint32  rpkt_lost   : 1;  // RPKT_LOST
	      uint32  rpkt_save   : 1;  // RPKT_SAVE
	      uint32  xpkt_lost   : 1;  // XPKT_LOST
	      uint32  xpkt_finish : 1;  // XPKT_FINISH
	      uint32  notxbuf     : 1;  // NOTXBUF
	      uint32  xpkt_save   : 1;  // XPKT_SAVE
	      uint32  norxbuf     : 1;  // NORXBUF
	      uint32  rpkt_finish : 1;  // RPKT_FINISH
    }bits;
    uint32  value;
}macIntrStatusReg_t;


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

    }bits;
  }mac_intr_timer_ctrl_reg_t;

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
    }bits;
  }mac_autopoll_timer_ctrl_reg_t;

// --- CR_MAC_DBLAC ---
typedef union
  {
    uint32  value;
    struct
    {
      uint32    reserved   :24;   // Reserved
      uint32  rx_thr_en  : 1;   // RX_THR_EN
      uint32  rxfifo_thr : 3;   // RXFIFO_THR
      uint32  incr16_en  : 1;   // INCR16_EN
      uint32  incr8_en   : 1;   // INCR8_EN
      uint32  incr4_en   : 1;   // INCR4_EN
    }bits;
 }mac_dma_arb_ctrl_reg_t;
 
 
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

#define DESCRIPTOR_POOL_SIZE	( MAC_DESCP_SIZE * (RX_DESCP_NO + TX_DESCP_NO) + 16 ) /* with 16 byte alignment */

/* ----- Ethernet private memory pool ----- */
typedef struct macMemPool_s {
	uint8 descrPool[DESCRIPTOR_POOL_SIZE];	/* Descr pool area */
} macMemPool_t;
 

/* ----- Adapter Card Table ------ */
typedef struct macAdapter_s {
	uint8 macAddr[6];						/* MAC-Address */

	macMemPool_t *macMemPool_p;

	uint32 enetPhyAddr;

	macTxDescr_t* txCurrDescp;				/* Currently working tx descriptor */
	uint32 txDescrRingBaseVAddr;			/* Transmit Descr Ring Virtual Address with 4byte alignment	*/
	uint32 rxDescrRingBaseVAddr;			/* Receive Descr Ring Virtual Address with 4byte alignment*/
	uint32 txCurrentDescp;					/* index to current tx descriptor */
	uint32 txUnReleasedDescp;				/* index to the unreleased descp of Tx */
	uint32 txUnReleasedBufCnt;				/* Unreleased buffer cnt hold by Tx	*/
	uint32 rxCurrentDescp;					/* index to current rx descriptor */

} macAdapter_t;

/************************************************************************
*        F U N C T I O N   P R O T O T Y P E S
*************************************************************************
*/
#ifndef PUBLIC
#define PUBLIC
#endif /* ifndef PUBLIC */
#ifndef PRIVATE
#define PRIVATE				static
#endif /* ifndef PRIVATE */


int tc3162_eth_init(unsigned char *mac_addr);
int tc3162_eth_send(sk_buff *skb);

int tc3162_eth_exit(void);

#endif /* __MAC_H */
