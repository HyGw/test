/***************************************************************
Copyright Statement:

This software/firmware and related documentation (EcoNet Software) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (EcoNet) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (ECONET SOFTWARE) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN AS IS 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVERS SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVERS SPECIFICATION OR CONFORMING TO A PARTICULAR 
STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND 
ECONET'S ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE ECONET 
SOFTWARE RELEASED HEREUNDER SHALL BE, AT ECONET'S SOLE OPTION, TO 
REVISE OR REPLACE THE ECONET SOFTWARE AT ISSUE OR REFUND ANY SOFTWARE 
LICENSE FEES OR SERVICE CHARGES PAID BY RECEIVER TO ECONET FOR SUCH 
ECONET SOFTWARE.
***************************************************************/
#ifndef _QDMA_BMGR_H_
#define _QDMA_BMGR_H_

/************************************************************************
*                  I N C L U D E S
*************************************************************************
*/	
#include <linux/netdevice.h>
#include <linux/types.h>
#include <linux/string.h>
#include <asm/r4kcache.h>
#include "qdma_dev.h"
#include "qdma_api.h"
#include <asm/tc3162/tc3162.h>


/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
#ifndef QDMA_LAN
#undef __IMEM
#define __IMEM
#endif

#define CONFIG_IRQ_DEF_VALUE			(0xFFFFFFFF)
#define HWFWD_PHYSICAL_SIZE_MAX			(0x400000)

#define QDMA_RX_MODE 	QDMA_RX_NAPI
#define QDMA_TX_MODE 	QDMA_TX_INTERRUPT
#define QDMA_TX_THRESHOLD	(16)

#define SKB_ALIGNMENT		(8)

#define TCSUPPORT_HW_QOS
#ifdef TCSUPPORT_QOS
//#define		QOS_FILTER_MARK		0xf0
#define 	QOS_HH_PRIORITY		0x10
#define 	QOS_H_PRIORITY		0x20
#define 	QOS_M_PRIORITY		0x30
#define		QOS_L_PRIORITY		0x40
#define		NULLQOS				-1
#define 	QOS_SW_PQ			0	/* will use hw at the same time */
#define		QOS_SW_WRR			1
#define		QOS_SW_CAR			2
#define 	QOS_HW_WRR			3
#define		QOS_HW_PQ			4
#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_XPON_HAL_API_QOS_CA) 
#define		QOS_HW_CAR			5
#endif
#else
#define     QOS_REMARKING       1
#define     QOS_REMARKING_MASK    0x00000007
#define     QOS_REMARKING_FLAG    0x00000001
#endif

#define 	QDMA_VIP_PACKET_NUM		32
/********************************
 *        VIP  Register         *
 ********************************/
#define FE_BASE     			0xBFB50000
#define FE_VIP_EN(x)			(FE_BASE + 0x300 + ((x) << 3))
#define FE_VIP_PATN(x)			(FE_BASE + 0x304 + ((x) << 3))

#define VIP_EN_ENABLE_OFFSET	0
#define VIP_EN_TYPE_OFFSET		1
#define VIP_EN_TYPE_LENGTH		3
#define VIP_EN_SPEN_OFFSET		4
#define VIP_EN_DPEN_OFFSET		5
#define VIP_EN_SWEN_OFFSET		6
#define VIP_EN_CPU_OFFSET		7
#define VIP_PATN_SP_OFFSET		0
#define VIP_PATN_DP_OFFSET		16
#define VIP_PATN_PORT_LENGTH	16

#define read_reg_word(reg) 			regRead32(reg)
#define write_reg_word(reg, wdata) 	regWrite32(reg, wdata)

/***************************************
    proc name
***************************************/

#if defined(QDMA_LAN)
#define qdma_path "qdma_lan"
#define counters_path "qdma_lan/counters"
#define debug_path "qdma_lan/debug"
#define ratelimit_path "qdma_lan/ratelimit"
#define qdma_verify_path "qdma_lan/qdma_verify"
#define qos_path "qdma_lan/qos"
#define memxs_path "qdma_lan/mem_xs"
#define isr_name "qdma_lan"
#define driver_qdma_hook_name "driver_qdma_lan_hook"
#define ecnt_qdma_hook ecnt_qdma_lan_hook
#else
#define qdma_path "qdma_wan"
#define counters_path "qdma_wan/counters"
#define debug_path "qdma_wan/debug"
#define ratelimit_path "qdma_wan/ratelimit"
#define qos_path "qdma_wan/qos"
#define mac_qos_path "qdma_wan/mac_qos"
#define mac_qos_flag_path "qdma_wan/mac_qos_flag"
#define mac_default_mask_path "qdma_wan/mac_default_queuemask"
#define memxs_path "qdma_wan/mem_xs"
#define mac_resource_limit_path "qdma_wan/mac_resource_limit"
#define isr_name "qdma_wan"
#define driver_qdma_hook_name "driver_qdma_wan_hook"
#define ecnt_qdma_hook ecnt_qdma_wan_hook
#endif


/************************************************************************
*                  M A C R O S
*************************************************************************
*/	
#define CHK_BUF() 		pos = begin + index ; \
							if(pos < off) { \
								index = 0 ; \
								begin = pos ; \
							} \
							if(pos > off + count) \
								goto done ;



#ifdef CONFIG_DEBUG
	#define QDMA_MSG(level, F, B...)	{ \
											if(gpQdmaPriv->devCfg.dbgLevel >= level) 	\
												printk("%s: %s [%d]: " F, qdma_path, strrchr(__FILE__, '/')+1, __LINE__, ##B) ; \
										}
	#define QDMA_ERR(F, B...)			printk("%s: %s [%d]: " F, qdma_path, strrchr(__FILE__, '/')+1, __LINE__, ##B)
	#define QDMA_LOG(F, B...)			printk("%s: %s [%d]: " F, qdma_path, strrchr(__FILE__, '/')+1, __LINE__, ##B)
#else
	#define QDMA_MSG(level, F, B...)	
	#define QDMA_ERR(F,B...)			
	#define QDMA_LOG(F,B...)			printk("%s: " F, qdma_path, ##B)
#endif


/************************************************************************
*                  D A T A	 T Y P E S
*************************************************************************
*/

typedef int (*qdma_api_op_t)(struct ECNT_QDMA_Data *qdma_data);


/***************************************
 struct definition
***************************************/
struct QDMA_DscpInfo_S {
	QDMA_DMA_DSCP_T				*dscpPtr ;
	uint						dscpIdx ;
#if 0
	void						*msgPtr ;
#ifdef CONFIG_SUPPORT_SELF_TEST
	void						*testMsgPtr ;
#endif
#endif
	struct sk_buff				*skb ;
	struct QDMA_DscpInfo_S		*next ;
} ;

typedef struct {
	struct {
		unchar		isTxPolling			:1 ;
		unchar		isRxPolling			:1 ;
		unchar		isRxNapi			:1 ;
		unchar		isIsrRequest		:1 ;
		unchar 		resv1				:4 ;
	} flags ;
	unchar		txRecycleThreshold ;
	unchar		macType;
#ifdef CONFIG_NEW_QDMA_CSR_OFFSET
	unchar		txQueueTrtcmScale ;
	unchar		gponTrtcmScale ;
#endif /* CONFIG_NEW_QDMA_CSR_OFFSET */
#ifdef CONFIG_DEBUG
	unchar		dbgLevel ;
#endif /* CONFIG_DEBUG */
#ifdef CONFIG_SUPPORT_SELF_TEST
	unchar		txDscpDoneBit ;
	unchar		rxDscpDoneBit ;
	ushort		waitTxMaxNums ;
	ushort		countDown ;
	ushort		countDownRound ;
	unchar		rxMsgLens ;
#endif /* CONFIG_SUPPORT_SELF_TEST */	
	int (*bmRecvCallbackFunction)(void *, uint, struct sk_buff *, uint) ;
#if defined(TCSUPPORT_FTTDP_V2) && defined(QDMA_LAN)
    int (*bmRecvCallbackPtmLanFunction)(void *, uint, struct sk_buff *, uint) ;
#endif
	int (*bmEventCallbackFunction)(QDMA_EventType_t) ;
    int (*bmRecyPktCallbackFunction)(struct sk_buff *, uint) ;
	void (*bmGponMacIntHandler)(void) ;
	void (*bmEponMacIntHandler)(void) ;
	void (*bmXponPhyIntHandler)(void) ;
} BM_DevConfig_T ;

typedef struct {
	ushort 	IrqQueueAsynchronous ;
	ushort 	txIrqQueueIdxErrs ;
	uint	rxAllocFailErrs ;
	ushort	intIrqFull ;
	ushort	IrqErr ;
	ushort	intNoLinkDscp ;
	ushort	intLowLinkDscp ;
	ushort	txPktsFreeCounts ;
	ushort	txPktsFreeErrs ;
	ushort	txMacLimitDropCnt ;
	
	uint	txCounts[TX_RING_NUM] ;
	uint	txRecycleCounts[TX_RING_NUM] ;
	uint 	rxCounts[RX_RING_NUM] ;
	ushort 	txDscpIncorrect[TX_RING_NUM] ;
	ushort 	txDscpDoneErrs[TX_RING_NUM] ;
	ushort 	rxDscpIncorrect[RX_RING_NUM] ;
	ushort	rxPktErrs[RX_RING_NUM] ;
	ushort	noTxDscps[TX_RING_NUM] ;
	ushort 	noRxCbErrs[RX_RING_NUM] ;
	ushort	noRxDscps[RX_RING_NUM] ;
	ushort	rxAllocFailDropErrs[RX_RING_NUM] ;
	ushort	intRxCoherent[RX_RING_NUM] ;
	ushort	intTxCoherent[TX_RING_NUM] ;
	ushort	intNoRxBuff[RX_RING_NUM] ;
	ushort	intNoTxBuff[TX_RING_NUM] ;
	ushort	intNoRxDscp[RX_RING_NUM] ;
	ushort	intNoTxDscp[TX_RING_NUM] ;
	ushort 	rxBroadcastCounts[RX_RING_NUM] ;
	ushort 	rxUnknownUnicastCounts[RX_RING_NUM] ;
	ushort  rxMulticastCounts[RX_RING_NUM] ;
	uint	intRxDone[RX_RING_NUM] ;
	uint	intTxDone[TX_RING_NUM] ;
} BM_Counters_T ;

typedef struct {
	uint						csrBaseAddr ;
	spinlock_t                      txLock[TX_RING_NUM] ;                           /* spin lock for Tx */
	spinlock_t                      irqLock ;                               /* spin lock for IRQ */
	spinlock_t                      txDoneLock ;                    /* spin lock for tx transmit done */
	ushort						txDscpNum[TX_RING_NUM] ;				/* Total TX DSCP number */
	ushort						rxDscpNum[RX_RING_NUM] ;				/* Total RX DSCP number */
	ushort						hwFwdDscpNum ;
	ushort						irqDepth ;				/* Max depth for IRQ queue */
	ushort						hwPktSize ;
	uint						dscpInfoAddr ; 			/* Start pointer for DSCP information node */
	uint						txBaseAddr[TX_RING_NUM] ;
	uint						rxBaseAddr[RX_RING_NUM] ;
	uint						irqQueueAddr ;			/* IRQ queue address */
	uint						hwFwdBaseAddr ;			/* Base address of the hardware forwarding */
	uint						hwFwdBuffAddr ;			/* Base address of the hardware forwarding Buffer*/
	uint						hwFwdPayloadSize ;		/* Payload size of the hardware forwarding Buffer*/
	struct QDMA_DscpInfo_S		*txHeadPtr[TX_RING_NUM] ;			/* Head node for unused tx desc. */
	struct QDMA_DscpInfo_S		*txTailPtr[TX_RING_NUM] ;			/* Tail node for unused tx desc. */
	struct QDMA_DscpInfo_S		*txUsingPtr[TX_RING_NUM] ;			/* TXDMA using DSCP node. */
	struct QDMA_DscpInfo_S		*rxStartPtr[RX_RING_NUM] ;			/* Start using node for rx desc. */
	struct QDMA_DscpInfo_S		*rxEndPtr[RX_RING_NUM] ;				/* End using node for rx desc. */
	struct QDMA_DscpInfo_S		*rxUsingPtr[RX_RING_NUM] ;			/* RXDMA using DSCP node. */
	struct tasklet_struct 		task ;
	BM_DevConfig_T				devCfg ;
	BM_Counters_T				counters ;
	struct timer_list			fwdCfg_timer ;
#if defined(TCSUPPORT_SLM_EN) && !defined(QDMA_LAN)
	uint						slmPhySize ;
	uint						slmPhyAddr ;
#endif
#ifdef TCSUPPORT_DRIVER_API
	QDMA_Dev_T 					qdmaDev ;
#endif
	QDMA_DbgCntrChnlGroup_t		dbgCntrType;
} QDMA_Private_T ;

typedef struct {
	ushort txDscpNum[TX_RING_NUM] ;
	ushort rxDscpNum[RX_RING_NUM] ;
	ushort hwDscpNum ;
	ushort irqDepth ;
	ushort hwFwdPktLen ;
} QDMA_Init_T ;

typedef struct {
	uint headIdx;
	uint tailIdx;
	uint cntr;
} QDMA_Lmgr_queueInfo_T ;

typedef struct {
	ushort sport ;
	ushort dport ;
	unchar sport_en : 1 ;
	unchar dport_en : 1 ;
	unchar swap_en  : 1 ;
	unchar resv		: 4 ;
	unchar ip_type ;
} QDMA_VIP_INFO_T ;

/************************************************************************
*                  D A T A   D E C L A R A T I O N S
*************************************************************************
*/
extern QDMA_Private_T *gpQdmaPriv ;


/************************************************************************
*                  F U N C T I O N    D E C L A R A T I O N S
#                  I N L I N E    F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
    
int inline qdma_kfree_skb(struct sk_buff *skb, uint msg0) ;
int qdma_set_tx_delay(unchar txIrqThreshold, ushort txIrqPtime);
int qdma_set_rx_delay(unchar rxMaxInt, unchar rxMaxPtime, int ringIdx);
int qdma_receive_packet_mode(QDMA_RecvMode_t rxMode);
int qdma_recycle_packet_mode(QDMA_RecycleMode_t txMode, unchar txThrh);
struct sk_buff * allocate_skb_buffer(void);
int qdma_prepare_rx_buffer(int ringIdx);
int qdma_bm_receive_packets(uint maxPkts, int ringIdx) ;
int qdma_bm_hook_receive_buffer(struct sk_buff *skb, int ringIdx) ;
int qdma_bm_recycle_receive_buffer(int ringIdx) ;
int qdma_bm_transmit_packet(struct sk_buff *skb, int ringIdx, uint msg0, uint msg1) ;
int qdma_bm_transmit_done(int amount) ;
int qdma_bm_recycle_transmit_buffer(int ringIdx) ;
int qdma_bm_tx_polling_mode(QDMA_Mode_t txMode, unchar txThreshold) ;
#endif /* _QDMA_BMGR_H_ */

