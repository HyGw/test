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

/************************************************************************
*                  I N C L U D E S
*************************************************************************
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/dma-mapping.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>

#include "qdma_dev.h"
#include "qdma_bmgr.h"
#include "qdma_api.h"
#include <linux/foe_hook.h>
#include <linux/libcompileoption.h>
#include <ecnt_hook/ecnt_hook_traffic_classify.h>
#include <ecnt_hook/ecnt_hook_ether.h>

#include <linux/if_vlan.h>
/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
#define HIGH_QUEUE_BUDGETS	5
#define LOW_QUEUE_BUDGETS	2

#define HIGHSET_PRIORITY    7
#define QUQUE_REMARK_MASK   		(0x7)
#define QUEUE_REMARK_OFFSET 		(11)


#define SWITCH_BUFFER_THRESHOLD 40 /* unit is 40kByte */
#define CHANNEL_RATELIMIT_THRESHOLD 300000 /* unit is 1kbps */

/************************************************************************
*                  M A C R O S
*************************************************************************
*/
#define MAX(a, b) ((a>b)?(a):(b))

/************************************************************************
*                  D A T A	 T Y P E S
*************************************************************************
*/

/************************************************************************
*                  E X T E R N A L	 D A T A   D E C L A R A T I O N S
*************************************************************************
*/
#ifdef CONFIG_SUPPORT_SELF_TEST
extern uint selfTestFlag;
#endif
extern uint physical_size;

extern struct timer_list dma_busy_timer ;
extern int dma_busy_expires;
extern int dma_busy_round_cnt;
extern int tx_dma_busy_enable_cnt;
extern int rx_dma_busy_enable_cnt;

extern struct timer_list reg_polling_timer ;
extern int reg_polling_expires;
extern int reg_polling_round_cnt;
extern int reg_polling_offset;
extern uint qdmaRegValBuff[100];

extern uint channel_limit_threshold;
extern uint queue_limit_threshold;

#if defined(TCSUPPORT_QDMA_VERIFY) && !defined(QDMA_LAN)
extern int wanTestFlag;
extern int highQueueFlag;
extern int forceRing0Flag;
extern int channelIdx;
extern int queueIdx;
#endif

extern unchar qdma_vip_num ;/*vip rule num*/
extern QDMA_VIP_INFO_T qdma_vip_info[QDMA_VIP_PACKET_NUM] ; 

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/
	
int isForceRX1=0;

atomic_t chnlLimit[CONFIG_QDMA_CHANNEL];
atomic_t queueLimit[CONFIG_QDMA_CHANNEL][CONFIG_QDMA_QUEUE];


/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/
static unchar qosTypeStr[QDMA_TXQOS_TYPE_NUMS][16] = {
	"WRR",
	"SP",
	"SPWRR7", 
	"SPWRR6", 
	"SPWRR5", 
	"SPWRR4", 
	"SPWRR3",
	"SPWRR2",
};

static unchar vChnlQosTypeStr[QDMA_VCHNL_TXQOS_TYPE_NUMS][16] = {
	"WRR",
	"SP",
	"SPWRR3",
	"SPWRR2",
};

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

/******************************************************************************
 Descriptor:	It's used to init the QDMA software driver and hardware device.
 				This function must be called if the upper layer application wanna
 				use the QDMA to send/receive packets.
 Input Args:	The pointer of the QDMA_InitCfg_t
 Ret Value:		0: init successful otherwise failed.
******************************************************************************/
int qdma_init(struct ECNT_QDMA_Data *qdma_data)
{
	QDMA_InitCfg_t *pInitCfg = qdma_data->qdma_private.pInitCfg ;
    uint base = gpQdmaPriv->csrBaseAddr;

    gpQdmaPriv->devCfg.macType = pInitCfg->macType;
	if(gpQdmaPriv->devCfg.flags.isTxPolling == QDMA_ENABLE)
   		gpQdmaPriv->devCfg.txRecycleThreshold = pInitCfg->txRecycleThrh ;
	qdma_data->qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_RX_PACKET;
	qdma_data->qdma_private.qdma_reg_cb.pCbFun = pInitCfg->cbRecvPkts;
	qdma_register_callback_function(qdma_data) ; 
#if defined(TCSUPPORT_FTTDP_V2) && defined(QDMA_LAN)
	qdma_data->qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_RX_PACKET_PTM_LAN;
	qdma_data->qdma_private.qdma_reg_cb.pCbFun = pInitCfg->cbRecvPktsPtmLan;
	qdma_register_callback_function(qdma_data) ;
#endif
	qdma_data->qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_EVENT_HANDLER;
	qdma_data->qdma_private.qdma_reg_cb.pCbFun = pInitCfg->cbEventHandler;
	qdma_register_callback_function(qdma_data) ;
	qdma_data->qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_RECYCLE_PACKET;
	qdma_data->qdma_private.qdma_reg_cb.pCbFun = pInitCfg->cbRecyclePkts;
	qdma_register_callback_function(qdma_data) ; 
	qdma_data->qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_GPON_MAC_HANDLER;
	qdma_data->qdma_private.qdma_reg_cb.pCbFun = pInitCfg->cbGponMacHandler;
	qdma_register_callback_function(qdma_data) ;
	qdma_data->qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_EPON_MAC_HANDLER;
	qdma_data->qdma_private.qdma_reg_cb.pCbFun = pInitCfg->cbEponMacHandler;
	qdma_register_callback_function(qdma_data) ;
	qdma_data->qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_XPON_PHY_HANDLER;
	qdma_data->qdma_private.qdma_reg_cb.pCbFun = pInitCfg->cbXponPhyHandler;
	qdma_register_callback_function(qdma_data) ;

	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to deinit the QDMA software driver and hardware device.
 				This function must be called if the upper layer application wanna
 				transfer to another application.
 Input Args:	None
 Ret Value:		0: init successful otherwise failed.
******************************************************************************/
int qdma_deinit(struct ECNT_QDMA_Data *qdma_data)
{
	qdma_data->qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_RX_PACKET;
	qdma_unregister_callback_function(qdma_data) ; 
#if defined(TCSUPPORT_FTTDP_V2) && defined(QDMA_LAN)
	qdma_data->qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_RX_PACKET_PTM_LAN;
	qdma_unregister_callback_function(qdma_data) ;
#endif
	qdma_data->qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_EVENT_HANDLER;
	qdma_unregister_callback_function(qdma_data) ;
	qdma_data->qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_RECYCLE_PACKET;
	qdma_unregister_callback_function(qdma_data) ; 
	qdma_data->qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_GPON_MAC_HANDLER;
	qdma_unregister_callback_function(qdma_data) ;
	qdma_data->qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_EPON_MAC_HANDLER;
	qdma_unregister_callback_function(qdma_data) ;
	qdma_data->qdma_private.qdma_reg_cb.type = QDMA_CALLBACK_XPON_PHY_HANDLER;
	qdma_unregister_callback_function(qdma_data) ;

	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to enable/disable TXDMA mode
 Input Args:	arg1: TX DMA mode (QDMA_ENABLE/QDMA_DISABLE)
 Ret Value: 	No
******************************************************************************/
int qdma_tx_dma_mode(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_Mode_t txMode = qdma_data->qdma_private.mode ;

	if(txMode == QDMA_ENABLE) {
		qdmaEnableTxDma(base) ;
	} else {
		qdmaDisableTxDma(base) ;
	}
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to enable/disable the RXDMA mode
 Input Args:	arg1: RX DMA mode (QDMA_ENABLE/QDMA_DISABLE)
 Ret Value: 	No
******************************************************************************/
int qdma_rx_dma_mode(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_Mode_t rxMode = qdma_data->qdma_private.mode ;
	
	if(rxMode == QDMA_ENABLE) {
		qdmaEnableRxDma(base) ;
	} else {
		qdmaDisableRxDma(base) ;
	}
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to enable/disable the QDMA loopback mode
 Input Args:	arg1: loopback mode (QDMA_LOOPBACK_DISABLE/QDMA_LOOPBACK_QDMA/QDMA_LOOPBACK_UMAC)
 Ret Value: 	No
******************************************************************************/
int qdma_loopback_mode(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_LoopbackMode_t lbMode = qdma_data->qdma_private.lbMode ;

	if(lbMode == QDMA_LOOPBACK_DISABLE) {
		qdmaDisableUmacLoopback(base) ;
		qdmaDisableQdmaLoopback(base) ;
	} else if(lbMode == QDMA_LOOPBACK_QDMA) {
		qdmaDisableUmacLoopback(base) ;
		qdmaEnableQdmaLoopback(base) ;
	} else if(lbMode == QDMA_LOOPBACK_UMAC) {
		qdmaDisableQdmaLoopback(base) ;
		qdmaEnableUmacLoopback(base) ;
	}
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to register the QDMA callback functions. The QDMA 
 				driver support several callback function type that is define 
 				in QDMA_CbType_t enum.
 Input Args:	arg1: callback function type that is define in QDMA_CbType_t enum.
 				arg2: the pointer of the callback function.
 Ret Value:		0: register successful otherwise failed.
******************************************************************************/
int qdma_register_callback_function(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_CbType_t type = qdma_data->qdma_private.qdma_reg_cb.type ;
	void *pCbFun = qdma_data->qdma_private.qdma_reg_cb.pCbFun ;

	if(!pCbFun) {
		return -EINVAL ;	
	}
	
	switch(type) {
		case QDMA_CALLBACK_RX_PACKET:
			gpQdmaPriv->devCfg.bmRecvCallbackFunction = (qdma_callback_recv_packet_t)pCbFun ;	
			break ;
    #if defined(TCSUPPORT_FTTDP_V2) && defined(QDMA_LAN)
        case QDMA_CALLBACK_RX_PACKET_PTM_LAN:
            gpQdmaPriv->devCfg.bmRecvCallbackPtmLanFunction = (qdma_callback_recv_packet_t)pCbFun ;   
            break ;
    #endif
			
		case QDMA_CALLBACK_EVENT_HANDLER:
			gpQdmaPriv->devCfg.bmEventCallbackFunction = (qdma_callback_event_handler_t)pCbFun ;	
			break ;
			
		case QDMA_CALLBACK_RECYCLE_PACKET:
			gpQdmaPriv->devCfg.bmRecyPktCallbackFunction = (qdma_callback_recycle_packet_t)pCbFun ;	
			break ;
			
		case QDMA_CALLBACK_GPON_MAC_HANDLER :
			gpQdmaPriv->devCfg.bmGponMacIntHandler = (qdma_callback_int_handler_t)pCbFun ;
			qdmaEnableInt(base, INT_MASK_GPON_MAC) ;
			break ;
			
		case QDMA_CALLBACK_EPON_MAC_HANDLER :
			gpQdmaPriv->devCfg.bmEponMacIntHandler = (qdma_callback_int_handler_t)pCbFun ;
			qdmaEnableInt(base, INT_MASK_EPON_MAC) ;
			break ;
			
		case QDMA_CALLBACK_XPON_PHY_HANDLER :
			gpQdmaPriv->devCfg.bmXponPhyIntHandler = (qdma_callback_int_handler_t)pCbFun ;
			qdmaEnableInt(base, INT_MASK_XPON_PHY) ;
			break ;
			
		default:
			return -EFAULT ;	
	}
	return 0 ;
}

/******************************************************************************
 Description:	It's used to unregister the QDMA callback functions.
 Input Args:	arg1: callback function type that is define in QDMA_CbType_t enum.
 Ret Value:		0: unregister successful otherwise failed.
******************************************************************************/
int qdma_unregister_callback_function(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_CbType_t type = qdma_data->qdma_private.qdma_reg_cb.type ;

	switch(type) {
		case QDMA_CALLBACK_RX_PACKET:
			gpQdmaPriv->devCfg.bmRecvCallbackFunction = NULL ;	
			break ;
	#if defined(TCSUPPORT_FTTDP_V2) && defined(QDMA_LAN)
        case QDMA_CALLBACK_RX_PACKET_PTM_LAN:
            gpQdmaPriv->devCfg.bmRecvCallbackPtmLanFunction = NULL ;  
            break ;
    #endif
		case QDMA_CALLBACK_EVENT_HANDLER:
			gpQdmaPriv->devCfg.bmEventCallbackFunction = NULL ;	
			break ;
			
		case QDMA_CALLBACK_RECYCLE_PACKET:
			gpQdmaPriv->devCfg.bmRecyPktCallbackFunction = NULL ;	
			break ;
			
		case QDMA_CALLBACK_GPON_MAC_HANDLER :
			qdmaDisableInt(base, INT_MASK_GPON_MAC) ;
			gpQdmaPriv->devCfg.bmGponMacIntHandler = NULL ;
			break ;
			
		case QDMA_CALLBACK_EPON_MAC_HANDLER :
			qdmaDisableInt(base, INT_MASK_EPON_MAC) ;
			gpQdmaPriv->devCfg.bmEponMacIntHandler = NULL ;
			break ;
			
		case QDMA_CALLBACK_XPON_PHY_HANDLER :
			qdmaDisableInt(base, INT_MASK_XPON_PHY) ;
			gpQdmaPriv->devCfg.bmXponPhyIntHandler = NULL ;
			break ;
			
		default:
			return -EFAULT ;	
	}
	return 0 ;
}

/******************************************************************************
 Descriptor:
 Input Args:
 Ret Value:	
******************************************************************************/
__IMEM int qdma_enable_rxpkt_int(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;	
	qdmaEnableInt(base, (INT_MASK_NO_RX0_CPU_DSCP|INT_MASK_RX0_DONE|INT_MASK_NO_RX1_CPU_DSCP|INT_MASK_RX1_DONE)) ;
	return 0 ;
}

/******************************************************************************
 Descriptor:
 Input Args:
 Ret Value:	
******************************************************************************/
__IMEM int qdma_disable_rxpkt_int(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;	
	qdmaDisableInt(base, (INT_MASK_NO_RX0_CPU_DSCP|INT_MASK_RX0_DONE|INT_MASK_NO_RX1_CPU_DSCP|INT_MASK_RX1_DONE)) ;
	return 0 ;
}

/******************************************************************************
 Descriptor:
 Input Args:
 Ret Value:	
******************************************************************************/

__IMEM int qdma_receive_packets(struct ECNT_QDMA_Data *qdma_data)
{
	int nums=0, curNum=0, nPkts=0 ;
	int ring1_empty_flag=0, ring0_empty_flag=0 ;
	int maxPkts = qdma_data->qdma_private.cnt ;
	
	if(maxPkts == 0){
#ifdef CONFIG_SUPPORT_SELF_TEST
		maxPkts = CONFIG_RX0_DSCP_NUM + CONFIG_RX1_DSCP_NUM + CONFIG_HWFWD_DSCP_NUM;
#else
		maxPkts = CONFIG_RX0_DSCP_NUM + CONFIG_RX1_DSCP_NUM;
#endif
    }
	do {
		ring1_empty_flag = ring0_empty_flag = 0;
		
		nPkts = min(maxPkts-nums, HIGH_QUEUE_BUDGETS);
		if(nPkts) {
			curNum = qdma_bm_receive_packets(nPkts, RING_IDX_1) ;
			if(curNum<nPkts)
				ring1_empty_flag = 1;
			if(curNum>nPkts)
				QDMA_ERR("Error-Ring1: curNum:%d, nPkts:%d, maxPkts:%d, nums:%d.\n", curNum, nPkts, maxPkts, nums) ;
			nums += curNum;
		}

		if((isForceRX1 == 1) && (ring1_empty_flag == 0))	/*  only get pkts from rx1 when ring1 has enough pkts */
			continue;
		
		nPkts = min(maxPkts-nums, LOW_QUEUE_BUDGETS);
		if(nPkts) {
			curNum = qdma_bm_receive_packets(nPkts, RING_IDX_0) ;
			if(curNum<nPkts)
				ring0_empty_flag = 1;
			if(curNum>nPkts)
				QDMA_ERR("Error-Ring0: curNum:%d, nPkts:%d, maxPkts:%d, nums:%d.\n", curNum, nPkts, maxPkts, nums) ;
			nums += curNum;
		}

		if((ring1_empty_flag == 1) && (ring0_empty_flag == 1))
			break;
	} while(nums<maxPkts) ;
	
	if(nums>maxPkts)
		QDMA_ERR("Error: nums:%d, maxPkts:%d.\n", nums, maxPkts) ; 
	
	return nums;
}

static inline unsigned char getQueueInfoBySkb(struct sk_buff *skb)
{	
	char *vlan_p = NULL, *ether_type_ptr = NULL;
	unsigned char txq=0;

	if( skb->mark & (1 << QUQUE_REMARK_EXIST_OFFSET) )
	{
		/*if queue is remark, get txq according to queue filed in skb->mark*/
		txq = (skb->mark >> QUEUE_REMARK_OFFSET) & QUQUE_REMARK_MASK;
	}
	else
	{
		/*if queue is not remark, get txq according to pbit*/	
		ether_type_ptr = skb->data + 2 * ETH_ALEN ;/*skip mac header*/
		if( *(unsigned short *)ether_type_ptr == htons(ETH_P_8021Q) )/*if has vlan layer*/
		{
			vlan_p = ether_type_ptr + 2 ; /*pointer to priority*/
			txq = ( (*(unsigned short*)vlan_p) >> VLAN_PRIO_SHIFT ) & PRIO_REMARK_MASK ;/*use 3 bits priority as Q*/
		}
	}
	
	//printk("getQueueInfoBySkb:---skb->mark = 0x%x, get bind txq = %d\r\n", skb->mark, txq);
	return txq;
}

/******************************************************************************
 Descriptor:
 Input Args:
 Ret Value:	
******************************************************************************/
__IMEM int qdma_transmit_packet(struct ECNT_QDMA_Data *qdma_data)
{
	struct sk_buff *skb = qdma_data->qdma_private.qdma_transmit.skb ;
	uint msg0 = qdma_data->qdma_private.qdma_transmit.msg0 ;
	uint msg1 = qdma_data->qdma_private.qdma_transmit.msg1 ;
	struct port_info *pMacInfo = qdma_data->qdma_private.qdma_transmit.pMacInfo ;
    int ringIdx = 0 ;
	int priority = 0;
	uint base = gpQdmaPriv->csrBaseAddr ;
    txMsgWord_t txMsg, *pTxMsg = &txMsg ;
    uint channel = 0, queue = 0 ;

    pTxMsg->pTxMsgW0 = (txMsgWord0_t *)&msg0 ;
    pTxMsg->pTxMsgW1 = (txMsgWord1_t *)&msg1 ;

#ifdef CONFIG_SUPPORT_SELF_TEST
	if((selfTestFlag == 1) && (skb->dropcount != 0xdeadbeef)) 
	{
		printk("catch a packet from normal path, selfTestFlag:%d, dropcount:0x%08x, loopcnt:%d\n", selfTestFlag, skb->dropcount, msg0>>24);
		return -EFAULT ;
	}
	skb->dropcount = 0;
#endif

	if(!skb)
	{
		QDMA_ERR("The input arguments are wrong, skb:%.8x, skbLen:%d.\n", (uint)skb, skb->len) ; 
		return -EFAULT ;
	}
	
#ifdef CONFIG_SUPPORT_SELF_TEST
    if(pTxMsg->pTxMsgW0->bits.oam) {
        ringIdx = RING_IDX_1 ;
    } 
#else
    if(pMacInfo && pMacInfo->txq_is_valid) {
        pTxMsg->pTxMsgW0->bits.queue = pMacInfo->txq;	/*if txq which is set by pon driver is valid , txq(HW) will overlap the txmsg(SW)*/
    } else {
#if defined(QDMA_LAN)
    ringIdx = RING_IDX_0 ;
    if(TCSUPPORT_FWC_QOS_VAL)
    {
        priority = getQueueInfoBySkb(skb);
        pTxMsg->pTxMsgW0->bits.queue = priority ;
    }
    else
    pTxMsg->pTxMsgW0->bits.queue = 0 ;
#else
    if(pTxMsg->pTxMsgW0->bits.oam) {    /* oam==1 equal to pMacInfo==NULL */
        ringIdx = RING_IDX_1 ;
        priority = HIGHSET_PRIORITY ;
    } else {
        #ifdef TCSUPPORT_QOS
        ringIdx = qdma_get_ringIdx(skb, pMacInfo) ;
        if(ringIdx == RING_IDX_1) {
            priority = HIGHSET_PRIORITY ;
        } else {    
			if(TCSUPPORT_FWC_QOS_VAL)
			{
				priority = getQueueInfoBySkb(skb);
			}
			else
			{
            priority = qdma_qos_mark(skb);
			}
			
            if(priority == -EFAULT) {
        		QDMA_ERR("priority is wrong: the value of priority is %d.\n", priority) ; 
                return -EFAULT;
            }
        }
        #else
        priority = qdma_qos_remark(skb);
        if(priority == -1) {
            ringIdx = qdma_get_ringIdx(skb, pMacInfo) ;
            if(ringIdx == RING_IDX_1) {
                priority = HIGHSET_PRIORITY ;
            } else {
                priority = 0 ;
            }
        }
        #endif
    }

    pTxMsg->pTxMsgW0->bits.queue = priority ;
#endif
    }
#endif
   
#if defined(TCSUPPORT_QDMA_VERIFY) && !defined(QDMA_LAN)
	if(wanTestFlag) {
		skb_push(skb, 2);
		if((skb->data[0] == 0) || (skb->data[0] == 1)) {
			ringIdx = skb->data[0];
		} else {
			QDMA_ERR("Fault: skb->data[0] is %d\n", skb->data[0]);
		}
		skb_pull(skb, 2);
			
		if(forceRing0Flag)
			ringIdx = 0;
		
		pTxMsg->pTxMsgW0->bits.channel = channelIdx;
		pTxMsg->pTxMsgW0->bits.queue = queueIdx;
		
		if(highQueueFlag) {
			if(ringIdx == 1)
				pTxMsg->pTxMsgW0->bits.queue = 7;
			else if(ringIdx == 0)
				pTxMsg->pTxMsgW0->bits.queue = 6;
		}
	}
#endif

	/* recycle TX DSCP when send packets in tx polling mode */
	if(gpQdmaPriv->devCfg.flags.isTxPolling == QDMA_ENABLE) {
		if(qdmaGetIrqEntryLen(base) >= gpQdmaPriv->devCfg.txRecycleThreshold) {
			qdma_bm_transmit_done(gpQdmaPriv->devCfg.txRecycleThreshold<<2) ;
		}
	}
    
    channel = pTxMsg->pTxMsgW0->bits.channel ;
    queue = pTxMsg->pTxMsgW0->bits.queue ;    
    if(macResourceLimit(skb, channel, queue) != 0) {
		QDMA_MSG(DBG_ST, "resource limit: chnlLimit[%d]:%d, queueLimit[%d][%d]:%d, skb->len:%d, msg0:0x%08x.\n", 
            channel, atomic_read(&chnlLimit[channel]), channel, queue, atomic_read(&queueLimit[channel][queue]), skb->len, msg0) ;
        return -EFAULT;
    }

    if(pMacInfo && ra_sw_nat_hook_tx) {
        pMacInfo->txq = pTxMsg->pTxMsgW0->bits.queue ;
        
        /* move foe hook function to here */
		if (ra_sw_nat_hook_tx(skb, pMacInfo, pMacInfo->magic) == 0) {
			QDMA_MSG(DBG_ST, "this is duplicate packet in keepalive new header mode, just drop it \n") ;
			return -EBADR;
		}
    }
    
	return qdma_bm_transmit_packet(skb, ringIdx, msg0, msg1) ;
}

/******************************************************************************
 Descriptor:	It's used to configure the TXQOS weight type and scale.
 Input Args:	arg1: setting the WRR weighting value is base on packet or byte
 					  (QDMA_TXQOS_WEIGHT_BY_PACKET/QDMA_TXQOS_WEIGHT_BY_BYTE)
 				arg2: setting the byte weighting scale(QDMA_TXQOS_WEIGHT_SCALE_64B
 				      /QDMA_TXQOS_WEIGHT_SCALE_16B). when the weigthing value 
 				      is base on packet, these argument is don't care.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_tx_qos_weight(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxQosWeightType_t weightBase = qdma_data->qdma_private.qdma_tx_qos.weightBase ;
	QDMA_TxQosWeightScale_t weightScale = qdma_data->qdma_private.qdma_tx_qos.weightScale ;

	if(weightBase == QDMA_TXQOS_WEIGHT_BY_PACKET) {
		qdmaSetTxQosWeightByPacket(base) ;
	} else if(weightBase == QDMA_TXQOS_WEIGHT_BY_BYTE) {
		qdmaSetTxQosWeightByByte(base) ;
		if(weightScale == QDMA_TXQOS_WEIGHT_SCALE_64B) {
			qdmaSetTxQosWeightScale64(base) ;
		} else if(weightScale == QDMA_TXQOS_WEIGHT_SCALE_16B) {
			qdmaSetTxQosWeightScale16(base) ;
		} else {
			return -EINVAL ;
		}
	} else {
		return -EINVAL ;
	}
	
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to get the TXQOS weight type and scale.
 Input Args:	arg1: the pointer of the weight base value 
 				      (QDMA_TXQOS_WEIGHT_BY_PACKET/QDMA_TXQOS_WEIGHT_BY_BYTE)
 				arg2: the pointer of the weight scale value
 					  (QDMA_TXQOS_WEIGHT_SCALE_64B/QDMA_TXQOS_WEIGHT_SCALE_16B)
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_get_tx_qos_weight(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;

	if(qdmaIsTxQosWeightByByte(base)) {
		qdma_data->qdma_private.qdma_tx_qos.weightBase = QDMA_TXQOS_WEIGHT_BY_BYTE ;
	} else {
		qdma_data->qdma_private.qdma_tx_qos.weightBase = QDMA_TXQOS_WEIGHT_BY_PACKET ;
	}
	
	if(qdmaIsTxQosWeightScale16(base)) {
		qdma_data->qdma_private.qdma_tx_qos.weightScale = QDMA_TXQOS_WEIGHT_SCALE_16B ;
	} else {
		qdma_data->qdma_private.qdma_tx_qos.weightScale = QDMA_TXQOS_WEIGHT_SCALE_64B ;
	}
	
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to configure the tx queue scheduler and queue weigth
 				for specific channel.
 Input Args:	The pointer of the tx qos scheduler struct. It includes:
 				- channel: specific the channel ID (0~15)
 				- qosType: The QoS type is define in QDMA_TxQosType_t enum.
 				- weight: The unit of WRR weight is packets (0~100, 255 means don't care).
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_tx_qos(struct ECNT_QDMA_Data *qdma_data) 
{
	int ret = 0, i = 0 ;
	unchar qosType = 0 ;
	unchar weight[CONFIG_QDMA_QUEUE], qosStr[128], qosStrTmp[16] ;
	int weightNum[QDMA_TXQOS_TYPE_NUMS]={8, 0, 7, 6, 5, 4, 3, 2};
	QDMA_TxQosScheduler_T *pTxQos = qdma_data->qdma_private.qdma_tx_qos.pTxQos ;
	
	if(pTxQos->channel >= CONFIG_QDMA_CHANNEL) {
		return -EINVAL ;
	}
	
	if(pTxQos->qosType >= QDMA_TXQOS_TYPE_NUMS) {
		return -EINVAL ;
	}
	qosType = (unchar)pTxQos->qosType;

	for(i=0 ; i<weightNum[pTxQos->qosType] ; i++) {
		if(pTxQos->queue[i].weight == 0) {	/* if fixed weight has value 0 in new qosType, then config weight, but use old qosType  */
			ret = qdmaGetTxQosScheduler(pTxQos->channel, &qosType, weight) ;
			if(ret < 0) {
				QDMA_ERR("qdmaGetTxQosScheduler error\n");
				return -EFAULT ;
			}
			for(i=0 ; i<weightNum[qosType] ; i++) {
				if(pTxQos->queue[i].weight == 0) {	/* if fixed weight has value 0 in old qosType, then exit  */
					QDMA_ERR("channel:%d, old qosTYpe:%d, qosWeight[%d] is %d\n", pTxQos->channel, qosType, i, pTxQos->queue[i].weight);
					return -EFAULT ;
				}
			}
			sprintf(qosStr, "channel is %d, old qosType is %d, new qosType is %d, Q0:Q1:...:Q7=", pTxQos->channel, qosType, pTxQos->qosType);
			for(i=0 ; i<CONFIG_QDMA_QUEUE ; i++) {
				sprintf(qosStrTmp, "%d ", pTxQos->queue[i].weight);
				strcat(qosStr, qosStrTmp);
			}
			strcat(qosStr, "\n");
			QDMA_MSG(DBG_ST, "%s", qosStr);
		}
	}
	
	for(i=0 ; i<CONFIG_QDMA_QUEUE ; i++) {
		if((pTxQos->queue[i].weight<0 || pTxQos->queue[i].weight>100) && pTxQos->queue[i].weight!=255) {
			return -EINVAL ;
		}
	
		weight[i] = pTxQos->queue[i].weight ;
	}
	
	return qdmaSetTxQosScheduler(pTxQos->channel, qosType, weight) ;
}

/******************************************************************************
 Descriptor:	It's used to set the tx queue scheduler and queue weigth
 				for specific channel.
 Input Args:	The pointer of the tx qos scheduler struct. It includes:
 				- channel: specific the channel ID (0~15)
 				- qosType: The QoS type is define in QDMA_TxQosType_t enum.
 				- weight: The unit of WRR weight is packets.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_get_tx_qos(struct ECNT_QDMA_Data *qdma_data)
{
	int ret = 0, i = 0 ;
	unchar qosType = 0 ;
	unchar weight[CONFIG_QDMA_QUEUE] ;
	QDMA_TxQosScheduler_T *pTxQos = qdma_data->qdma_private.qdma_tx_qos.pTxQos ;
	
	if(pTxQos->channel >= CONFIG_QDMA_CHANNEL) {
		return -EINVAL ;
	}

	ret = qdmaGetTxQosScheduler(pTxQos->channel, &qosType, weight) ;
	if(ret < 0) {
		return -EFAULT ;
	}
	
	pTxQos->qosType = qosType ;
	
	for(i=0 ; i<CONFIG_QDMA_QUEUE ; i++) {
		pTxQos->queue[i].weight = weight[i] ;
	}

	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to set channel/queue limit threshold for mac driver.
 Input Args:	arg1: channel limit threshold
                     arg2: queue limit threshold
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_mac_limit_threshold(struct ECNT_QDMA_Data *qdma_data)
{
    channel_limit_threshold = qdma_data->qdma_private.qdma_mac_limit_thrh->chnlThrh ;
    queue_limit_threshold = qdma_data->qdma_private.qdma_mac_limit_thrh->queueThrh ;    
    return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to get channel/queue limit threshold for mac driver.
 Input Args:	arg1: channel limit threshold
                     arg2: queue limit threshold
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_get_mac_limit_threshold(struct ECNT_QDMA_Data *qdma_data)
{
    qdma_data->qdma_private.qdma_mac_limit_thrh->chnlThrh = channel_limit_threshold ;
    qdma_data->qdma_private.qdma_mac_limit_thrh->queueThrh = queue_limit_threshold ;    
    return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to configure the tx buffer threshold. For the buffer 
 				management, the total available on-chip buffer is 64Kbyte (256 
 				Blocks, 256 bytes per block). It is shared among WAN and LAN 
 				Tx/Rx interface. If the buffer usage exceeds the threshold, the
 				Tx DMA will stop retrieving packets.
 Input Args:	The pointer of the tx buffer control struct. It includes:
 				- mode: Eanble/Disable tx buffer usage control
 				- chnThreshold: Per tx per channel block usage threshold.
 				- totalThreshold: Total tx block usage threshold.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_txbuf_threshold(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxBufCtrl_T *pTxBufCtrl = qdma_data->qdma_private.pQdmaTxBufCtrl ;

	if(pTxBufCtrl->mode == QDMA_ENABLE) {
		qdmaEnableTxBufCtrl(base) ;
		
		qdmaSetTxBufChnnelThreshold(base, pTxBufCtrl->chnThreshold) ;
		qdmaSetTxBufTotalThreshold(base, pTxBufCtrl->totalThreshold) ;
	} else if(pTxBufCtrl->mode == QDMA_DISABLE) {
		qdmaDisableTxBufCtrl(base) ;
	} else {
		return -EINVAL ;
	}

	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to get the tx buffer threshold.
 Input Args:	The pointer of the tx buffer control struct. It includes:
 				- mode: Eanble/Disable tx buffer usage control
 				- chnThreshold: Per tx per channel block usage threshold.
 				- totalThreshold: Total tx block usage threshold.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_get_txbuf_threshold(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxBufCtrl_T *pTxBufCtrl = qdma_data->qdma_private.pQdmaTxBufCtrl ;

	pTxBufCtrl->mode = (qdmaIsTxBufCtrlEnable(base)?(QDMA_ENABLE):(QDMA_DISABLE));
	pTxBufCtrl->chnThreshold = qdmaGetTxBufChnnelThreshold(base);
	pTxBufCtrl->totalThreshold = qdmaGetTxBufTotalThreshold(base);

	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to enable/disable QDMA pre-fetch function. Since the
                on-chip buffer resource is limited, to prevent from Head-Of-Line
                blocking issue, 'when' to retrieving packet from External DRAM 
                to on-chip buffer will be application dependent.
 Input Args:	arg1: Enable/disable TXDMA pre-fetch function (QDMA_ENABLE/QDMA_DISABLE)
 			arg2: Enable/disable TXDMA pre-fetch overdrag function (QDMA_ENABLE/QDMA_DISABLE)
 			arg3: set TXDMA pre-fetch overdrag count value
 Ret Value:	No return value
******************************************************************************/
int qdma_set_prefetch_mode(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_PrefetchMode_T *qdma_prefetch = qdma_data->qdma_private.qdma_prefetch;

	if(qdma_prefetch->prefecthMode == QDMA_DISABLE) {
		qdmaDisableTxDmaPrefetch(base) ;
	} else {
		qdmaEnableTxDmaPrefetch(base) ;
	}

	if(qdma_prefetch->overDragMode == QDMA_DISABLE) {
		qdmaDisablePrefetchOverDrag(base) ;
	} else {
		qdmaEnablePrefetchOverDrag(base) ;
	}
	qdmaSetPrefetchOverDragCnt(base, qdma_prefetch->overDragCnt) ;

	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to enable or disable pktsize overhead function.
 Input Args:	arg1: mode means enable or disable.
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_pktsize_overhead_en(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_Mode_t mode = qdma_data->qdma_private.mode ;

	if(mode == QDMA_DISABLE) {
		qdmaDisablePktSizeOverHead(base) ;
	} else {
		qdmaEnablePktSizeOverHead(base) ;
	}

	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to get pktsize overhead function is enable or disable.
 Input Args:	None
 Ret Value:	0: Disable, 1: Enable.
******************************************************************************/
int qdma_get_pktsize_overhead_en(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;

	if(qdmaIsPktSizeOverHeadEnable(base)) {
		qdma_data->qdma_private.mode = QDMA_ENABLE ;
	} else {
		qdma_data->qdma_private.mode = QDMA_DISABLE ;
	}

	return qdma_data->qdma_private.mode ;
}

/******************************************************************************
 Descriptor:	It's used to set the value of pktsize overhead.
 Input Args:	arg1: pktsize overhead value
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_pktsize_overhead_value(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint cnt = qdma_data->qdma_private.cnt ;
	qdmaSetPktSizeOverHeadVal(base, cnt);
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to get the value of pktsize overhead.
 Input Args:	None
 Ret Value:	pktsize overhead value.
******************************************************************************/
int qdma_get_pktsize_overhead_value(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	qdma_data->qdma_private.cnt = qdmaGetPktSizeOverHeadVal(base) ;
	return qdma_data->qdma_private.cnt ;
}

/******************************************************************************
 Descriptor:	It's used to set the value of lmgr low threshold.
 Input Args:	arg1: lmgr low threshold value
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_lmgr_low_threshold(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint lowThrh = qdma_data->qdma_private.threshold ;
	qdmaSetHwLowThrshld(base, lowThrh);
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to get the value of lmgr low threshold.
 Input Args:	None
 Ret Value:	lmgr low threshold value.
******************************************************************************/
int qdma_get_lmgr_low_threshold(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	qdma_data->qdma_private.threshold = qdmaGetHwLowThrshld(base) ;
	return qdma_data->qdma_private.threshold ;
}

/******************************************************************************
 Descriptor:	It's used to get lmgr status: free lmgr dscp, used lmgr dscp, 
 			and used total buffer usage.
 Input Args:	arg1: to get free lmgr dscp counter
 			arg2: to get used lmgr dscp counter
 			arg3: to get used total buffer usage
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_get_lmgr_status(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint freeLmgr = qdmaLmgrFreeCount(base) ;

	qdma_data->qdma_private.qdma_lmgr_status->freeLmgr = freeLmgr;
	qdma_data->qdma_private.qdma_lmgr_status->usedLmgr = CONFIG_HWFWD_DSCP_NUM - freeLmgr;
	qdma_data->qdma_private.qdma_lmgr_status->usedBuf = qdmaLmgrTotalBufUsage(base);

	return 0 ;
}


/******************************************************************************
 Descriptor:	It's used to clear all the qdma cpu counters.
 Input Args:	None
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_clear_cpu_counters(struct ECNT_QDMA_Data *qdma_data)
{
	memset(&gpQdmaPriv->counters, 0, sizeof(BM_Counters_T)) ;
	
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to dump all the qdma cpu counters.
 Input Args:	None
 Ret Value:	No return value
******************************************************************************/
int qdma_dump_cpu_counters(struct ECNT_QDMA_Data *qdma_data)
{
	int ringIdx=0 ;
	
	printk("IRQ Queue Asynchronous             %u\n", gpQdmaPriv->counters.IrqQueueAsynchronous) ;
	printk("IRQ Queue Index Errors:            %u\n", gpQdmaPriv->counters.txIrqQueueIdxErrs) ;
	printk("IRQ Queue Full Interrupt:          %u\n", gpQdmaPriv->counters.intIrqFull) ;
	printk("IRQ Queue Error Number:            %u\n", gpQdmaPriv->counters.IrqErr) ;
	printk("RX Allocate Fail Errors:           %u\n", gpQdmaPriv->counters.rxAllocFailErrs) ;
	printk("No Link DSCP Interrupt:            %u\n", gpQdmaPriv->counters.intNoLinkDscp) ;
	printk("Low Link DSCP Interrupt:           %u\n", gpQdmaPriv->counters.intLowLinkDscp) ;
	printk("--------------------------------------------\n") ;

	for(ringIdx=0; ringIdx<TX_RING_NUM; ringIdx++) {
		printk("Tx%d DSCP Counts:                    %u\n", ringIdx, gpQdmaPriv->counters.txCounts[ringIdx]) ;
		printk("Tx%d DSCP Recycle Counts:            %u\n", ringIdx, gpQdmaPriv->counters.txRecycleCounts[ringIdx]) ;
		printk("Tx%d DSCP Content Incorrect:         %u\n", ringIdx, gpQdmaPriv->counters.txDscpIncorrect[ringIdx]) ;
		printk("Tx%d Done Bit Errors:                %u\n", ringIdx, gpQdmaPriv->counters.txDscpDoneErrs[ringIdx]) ;
		printk("No Tx%d DSCP Errors:                 %u\n", ringIdx, gpQdmaPriv->counters.noTxDscps[ringIdx]) ;
		printk("Tx%d Chorent Interrupt:              %u\n", ringIdx, gpQdmaPriv->counters.intTxCoherent[ringIdx]) ;
		printk("No Tx%d Buffer Interrupt:            %u\n", ringIdx, gpQdmaPriv->counters.intNoTxBuff[ringIdx]) ;
		printk("No Tx%d DSCP Interrupt:              %u\n", ringIdx, gpQdmaPriv->counters.intNoTxDscp[ringIdx]) ;
		printk("Tx%d Done Interrupt:                 %u\n", ringIdx, gpQdmaPriv->counters.intTxDone[ringIdx]) ;
		printk("--------------------------------------------\n") ;
	}

	for(ringIdx=0; ringIdx<RX_RING_NUM; ringIdx++) {
		printk("Rx%d DSCP Counts:                    %u\n", ringIdx, gpQdmaPriv->counters.rxCounts[ringIdx]) ;
		printk("Rx%d DSCP Content Incorrect:         %u\n", ringIdx, gpQdmaPriv->counters.rxDscpIncorrect[ringIdx]) ;
		printk("Rx%d Packet Errors:                  %u\n", ringIdx, gpQdmaPriv->counters.rxPktErrs[ringIdx]) ;
		printk("No Rx%d Callback Errors:             %u\n", ringIdx, gpQdmaPriv->counters.noRxCbErrs[ringIdx]) ;
		printk("No Rx%d DSCP Errors:                 %u\n", ringIdx, gpQdmaPriv->counters.noRxDscps[ringIdx]) ;
		printk("Rx%d Allocate Fail Drop Errors:      %u\n", ringIdx, gpQdmaPriv->counters.rxAllocFailDropErrs[ringIdx]) ;
		printk("Rx%d Chorent Interrupt:              %u\n", ringIdx, gpQdmaPriv->counters.intRxCoherent[ringIdx]) ;
		printk("No Rx%d Buffer Interrupt:            %u\n", ringIdx, gpQdmaPriv->counters.intNoRxBuff[ringIdx]) ;
		printk("No Rx%d DSCP Interrupt:              %u\n", ringIdx, gpQdmaPriv->counters.intNoRxDscp[ringIdx]) ;
		printk("Rx%d Broadcast Interrupt:            %u\n", ringIdx, gpQdmaPriv->counters.rxBroadcastCounts[ringIdx]) ;
		printk("Rx%d Unknown Unicast Interrupt:      %u\n", ringIdx, gpQdmaPriv->counters.rxUnknownUnicastCounts[ringIdx]) ;
		printk("Rx%d Multicast Interrupt:            %u\n", ringIdx, gpQdmaPriv->counters.rxMulticastCounts[ringIdx]) ;
		printk("Rx%d Done Interrupt:                 %u\n", ringIdx, gpQdmaPriv->counters.intRxDone[ringIdx]) ;
		printk("--------------------------------------------\n") ;
	}
	
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to dump all qdma related register values.
 Input Args:	None
 Ret Value:	No return value
******************************************************************************/
int qdma_dump_register_value(struct ECNT_QDMA_Data *qdma_data)
{
	int i=0 ;
	uint base = gpQdmaPriv->csrBaseAddr ;
	printk("Tx Mode %s \n",gpQdmaPriv->devCfg.flags.isTxPolling?"Polling":"Interrupt");
	printk("Rx Mode %s \n",gpQdmaPriv->devCfg.flags.isRxPolling?"Polling":
		gpQdmaPriv->devCfg.flags.isRxNapi?"Napi":"Interrupt");
	printk("gpQdmaPriv->csrBaseAddr:					 0x%08x\n", gpQdmaPriv->csrBaseAddr) ;
	printk("gpQdmaPriv->hwFwdDscpNum:					 %u\n", gpQdmaPriv->hwFwdDscpNum) ;
	printk("gpQdmaPriv->irqDepth:						 %u\n", gpQdmaPriv->irqDepth) ;
	printk("gpQdmaPriv->hwPktSize:						 %u\n", gpQdmaPriv->hwPktSize) ;
	printk("gpQdmaPriv->dscpInfoAddr:					 0x%08x\n", gpQdmaPriv->dscpInfoAddr) ;
	printk("gpQdmaPriv->irqQueueAddr:					 0x%08x\n", gpQdmaPriv->irqQueueAddr) ;
	printk("gpQdmaPriv->hwFwdBaseAddr:					 0x%08x\n", gpQdmaPriv->hwFwdBaseAddr) ;
	printk("gpQdmaPriv->hwFwdBuffAddr:					 0x%08x\n", gpQdmaPriv->hwFwdBuffAddr) ;
	printk("gpQdmaPriv->hwFwdPayloadSize:				 %u\n\n", gpQdmaPriv->hwFwdPayloadSize) ;
	for(i=0; i<TX_RING_NUM; i++) {
		printk("gpQdmaPriv->txDscpNum[%d]:					  %u\n", i, gpQdmaPriv->txDscpNum[i]) ;
		printk("gpQdmaPriv->txBaseAddr[%d]: 				  0x%08x\n", i, gpQdmaPriv->txBaseAddr[i]) ;
		printk("gpQdmaPriv->txHeadPtr[%d]->dscpIdx: 		  %d\n", i, (gpQdmaPriv->txHeadPtr[i]?(gpQdmaPriv->txHeadPtr[i]->dscpIdx):(-1))) ;
		printk("gpQdmaPriv->txTailPtr[%d]->dscpIdx: 		  %d\n", i, (gpQdmaPriv->txTailPtr[i]?(gpQdmaPriv->txTailPtr[i]->dscpIdx):(-1))) ;
		printk("gpQdmaPriv->txUsingPtr[%d]->dscpIdx:		  %d\n\n", i, (gpQdmaPriv->txUsingPtr[i]?(gpQdmaPriv->txUsingPtr[i]->dscpIdx):(-1))) ;
	}
	for(i=0; i<RX_RING_NUM; i++) {
		printk("gpQdmaPriv->rxDscpNum[%d]:					  %u\n", i, gpQdmaPriv->rxDscpNum[i]) ;
		printk("gpQdmaPriv->rxBaseAddr[%d]: 				  0x%08x\n", i, gpQdmaPriv->rxBaseAddr[i]) ;
		printk("gpQdmaPriv->rxStartPtr[%d]->dscpIdx:		  %d\n", i, (gpQdmaPriv->rxStartPtr[i]?(gpQdmaPriv->rxStartPtr[i]->dscpIdx):(-1))) ;
		printk("gpQdmaPriv->rxEndPtr[%d]->dscpIdx:			  %d\n", i, (gpQdmaPriv->rxEndPtr[i]?(gpQdmaPriv->rxEndPtr[i]->dscpIdx):(-1))) ;
		printk("gpQdmaPriv->rxUsingPtr[%d]->dscpIdx:		  %d\n\n", i, (gpQdmaPriv->rxUsingPtr[i]?(gpQdmaPriv->rxUsingPtr[i]->dscpIdx):(-1))) ;
	}

	printk("0x0000: QDMA_CSR_INFO                           :%.8x\n", IO_GREG(QDMA_CSR_INFO(base))) ;
	printk("0x0004: QDMA_CSR_GLB_CFG                        :%.8x\n", IO_GREG(QDMA_CSR_GLB_CFG(base))) ;
	for(i=0; i<TX_RING_NUM; i++) {
		printk("0x%04X: QDMA_CSR_TX_DSCP_BASE[%d]                :%.8x\n", 0x8+(i?0x100:0), i, IO_GREG(QDMA_CSR_TX_DSCP_BASE(base, i))) ;
		printk("0x%04X: QDMA_CSR_TX_CPU_IDX[%d]                  :%.8x\n", 0x10+(i?0x100:0), i, IO_GREG(QDMA_CSR_TX_CPU_IDX(base, i))) ;
		printk("0x%04X: QDMA_CSR_TX_DMA_IDX[%d]                  :%.8x\n", 0x14+(i?0x100:0), i, IO_GREG(QDMA_CSR_TX_DMA_IDX(base, i))) ;
	}
	for(i=0; i<RX_RING_NUM; i++) {
		printk("0x%04X: QDMA_CSR_RX_DSCP_BASE[%d]                :%.8x\n", 0xC+(i?0x100:0), i, IO_GREG(QDMA_CSR_RX_DSCP_BASE(base, i))) ;
		printk("0x%04X: QDMA_CSR_RX_CPU_IDX[%d]                  :%.8x\n", 0x18+(i?0x100:0), i, IO_GREG(QDMA_CSR_RX_CPU_IDX(base, i))) ;
		printk("0x%04X: QDMA_CSR_RX_DMA_IDX[%d]                  :%.8x\n", 0x1C+(i?0x100:0), i, IO_GREG(QDMA_CSR_RX_DMA_IDX(base, i))) ;
	}
	printk("0x0020: QDMA_CSR_HWFWD_DSCP_BASE                :%.8x\n", IO_GREG(QDMA_CSR_HWFWD_DSCP_BASE(base))) ;
	printk("0x0024: QDMA_CSR_HWFWD_BUFF_BASE                :%.8x\n", IO_GREG(QDMA_CSR_HWFWD_BUFF_BASE(base))) ;
	printk("0x0028: QDMA_CSR_HWFWD_DSCP_CFG                 :%.8x\n", IO_GREG(QDMA_CSR_HWFWD_DSCP_CFG(base))) ;
	printk("0x0030: QDMA_CSR_LMGR_INIT_CFG                  :%.8x\n", IO_GREG(QDMA_CSR_LMGR_INIT_CFG(base))) ;
	printk("0x0040: QDMA_CSR_LMGR_CHNL_RETIRE               :%.8x\n", IO_GREG(QDMA_CSR_LMGR_CHNL_RETIRE(base))) ;
	printk("0x0050: QDMA_CSR_INT_STATUS                     :%.8x\n", IO_GREG(QDMA_CSR_INT_STATUS(base))) ;
	printk("0x0054: QDMA_CSR_INT_ENABLE                     :%.8x\n", IO_GREG(QDMA_CSR_INT_ENABLE(base))) ;
	printk("0x0058: QDMA_CSR_TX_DELAY_INT_CFG               :%.8x\n", IO_GREG(QDMA_CSR_TX_DELAY_INT_CFG(base))) ;
	printk("0x005C: QDMA_CSR_RX_DELAY_INT_CFG               :%.8x\n", IO_GREG(QDMA_CSR_RX_DELAY_INT_CFG(base))) ;
	printk("0x0060: QDMA_CSR_IRQ_BASE                       :%.8x\n", IO_GREG(QDMA_CSR_IRQ_BASE(base))) ;
	printk("0x0064: QDMA_CSR_IRQ_CFG                        :%.8x\n", IO_GREG(QDMA_CSR_IRQ_CFG(base))) ;
	printk("0x0068: QDMA_CSR_IRQ_CLEAR_LEN                  :%.8x\n", IO_GREG(QDMA_CSR_IRQ_CLEAR_LEN(base))) ;
	printk("0x006C: QDMA_CSR_IRQ_STATUS                     :%.8x\n", IO_GREG(QDMA_CSR_IRQ_STATUS(base))) ;
	printk("0x0070: QDMA_CSR_IRQ_PTIME                      :%.8x\n", IO_GREG(QDMA_CSR_IRQ_PTIME(base))) ;
	printk("0x0080: QDMA_CSR_TXWRR_MODE_CFG                 :%.8x\n", IO_GREG(QDMA_CSR_TXWRR_MODE_CFG(base))) ;
	printk("0x0088: QDMA_CSR_TXWRR_WEIGHT_CFG               :%.8x\n", IO_GREG(QDMA_CSR_TXWRR_WEIGHT_CFG(base))) ;
	printk("0x0090: QDMA_CSR_PSE_BUF_USAGE_CFG              :%.8x\n", IO_GREG(QDMA_CSR_PSE_BUF_USAGE_CFG(base))) ;
	printk("0x0094: QDMA_CSR_EGRESS_RATEMETER_CFG           :%.8x\n", IO_GREG(QDMA_CSR_EGRESS_RATEMETER_CFG(base))) ;
	printk("0x0098: QDMA_CSR_EGRESS_RATELIMIT_CFG           :%.8x\n", IO_GREG(QDMA_CSR_EGRESS_RATELIMIT_CFG(base))) ;
	printk("0x009C: QDMA_CSR_RATELIMIT_PARAMETER_CFG        :%.8x\n", IO_GREG(QDMA_CSR_RATELIMIT_PARAMETER_CFG(base))) ;
	printk("0x00A0: QDMA_CSR_TXQ_CNGST_CFG                  :%.8x\n", IO_GREG(QDMA_CSR_TXQ_CNGST_CFG(base))) ;
	printk("0x00A4: QDMA_CSR_TXQ_DYN_TOTALTHR               :%.8x\n", IO_GREG(QDMA_CSR_TXQ_DYN_TOTALTHR(base))) ;
	printk("0x00A8: QDMA_CSR_TXQ_DYN_CHNLTHR_CFG            :%.8x\n", IO_GREG(QDMA_CSR_TXQ_DYN_CHNLTHR_CFG(base))) ;
	printk("0x00AC: QDMA_CSR_TXQ_DYN_QUEUETHR_CFG           :%.8x\n", IO_GREG(QDMA_CSR_TXQ_DYN_QUEUETHR_CFG(base))) ;
	for(i=0; i<8; i++)
		printk("0x%04X: QDMA_CSR_STATIC_QUEUE_THR_%d             :%.8x\n", 0xB0+(i<<2), i, IO_GREG(QDMA_CSR_STATIC_QUEUE_THR(base, i))) ;
	printk("0x00F0: QDMA_CSR_DBG_LMGR_STATUS                :%.8x\n", IO_GREG(QDMA_CSR_DBG_LMGR_STATUS(base))) ;
	printk("0x00F4: QDMA_CSR_DBG_LMGR_USAGE                 :%.8x\n", IO_GREG(QDMA_CSR_DBG_LMGR_USAGE(base))) ;
	printk("0x00F8: QDMA_CSR_DBG_QDMA_PROBE_LO              :%.8x\n", IO_GREG(QDMA_CSR_DBG_QDMA_PROBE_LO(base))) ;
	printk("0x00FC: QDMA_CSR_DBG_QDMA_PROBE_HI              :%.8x\n", IO_GREG(QDMA_CSR_DBG_QDMA_PROBE_HI(base))) ;
	printk("0x0100: QDMA_CSR_RX_RING_CFG                    :%.8x\n", IO_GREG(QDMA_CSR_RX_RING_CFG(base))) ;
	printk("0x0104: QDMA_CSR_RX_RING_THR                    :%.8x\n", IO_GREG(QDMA_CSR_RX_RING_THR(base))) ;
	printk("0x0120: QDMA_CSR_RX_PROTECT_CFG                 :%.8x\n", IO_GREG(QDMA_CSR_RX_PROTECT_CFG(base))) ;
	printk("0x0124: QDMA_CSR_RX_RATELIMIT_CFG               :%.8x\n", IO_GREG(QDMA_CSR_RX_RATELIMIT_CFG(base))) ;
	for(i=0; i<(CONFIG_QDMA_CHANNEL>>3); i++)
		printk("0x%04x: QDMA_CSR_PERCHNL_QOS_MODE_%02d_%02d         :%.8x\n", 0x180+(i<<2), (i<<3)+7, i<<3, IO_GREG(QDMA_CSR_PERCHNL_QOS_MODE(base, i))) ;
	for(i=0; i<(CONFIG_QDMA_CHANNEL>>1); i++)
		printk("0x%04x: QDMA_CSR_PERCHNL_DATARATE_%02d_%02d         :%.8x\n", 0x200+(i<<2), (i<<1)+1, i<<1, IO_GREG(QDMA_CSR_PERCHNL_DATARATE(base, i))) ;
	for(i=0; i<(CONFIG_QDMA_CHANNEL>>2); i++)
		printk("0x%04x: QDMA_CSR_PERCHNL_QUEUE_NORM_DROP_%02d_%02d  :%.8x\n", 0x280+(i<<2), (i<<2)+3, i<<2, IO_GREG(QDMA_CSR_PERCHNL_QUEUE_NORM_DROP(base, i))) ;
	for(i=0; i<(CONFIG_QDMA_CHANNEL>>2); i++)
		printk("0x%04x: QDMA_CSR_PERCHNL_QUEUE_DEI_DROP_%02d_%02d   :%.8x\n", 0x2C0+(i<<2), (i<<2)+3, i<<2, IO_GREG(QDMA_CSR_PERCHNL_QUEUE_DEI_DROP(base, i))) ;
	
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to dump all cpu tx/rx dscp information.
 Input Args:	None
 Ret Value:	No return value
******************************************************************************/
int qdma_dump_descriptor_info(struct ECNT_QDMA_Data *qdma_data)
{
	struct QDMA_DscpInfo_S *diPtr=NULL ;
	int i=0, idx=1, ringIdx=0;

	for(ringIdx=0; ringIdx<TX_RING_NUM ; ringIdx++) {
		idx = 1 ;
		diPtr = gpQdmaPriv->txHeadPtr[ringIdx] ;
		printk("Unused Tx%d DSCP Link List:\n", ringIdx) ;
		
		while(diPtr) {
			printk("%d: DSCP Idx:%d, DSCP Ptr:%.8x, Done Bit:%d\n", idx, diPtr->dscpIdx, 
																	(uint)diPtr->dscpPtr, 
																	diPtr->dscpPtr->ctrl.done) ;
			diPtr = diPtr->next ;
			idx++ ;
		} 
		
		idx = 1 ;
		printk("\nUsing Tx%d DSCP Set:\n", ringIdx) ;
		for(i=0 ; i<gpQdmaPriv->txDscpNum[ringIdx] ; i++) {
			diPtr = (struct QDMA_DscpInfo_S *)gpQdmaPriv->txBaseAddr[ringIdx] + i ;
			if(diPtr->next == NULL && diPtr!=gpQdmaPriv->txTailPtr[ringIdx]) {
				printk("%d: DSCP Idx:%d, DSCP Ptr:%.8x, Done Bit:%d, PktLen:%d, PktAddr:%.8x(%.8x), Next Idx:%d\n", 
																	idx, diPtr->dscpIdx, 
																	(uint)diPtr->dscpPtr, 
																	diPtr->dscpPtr->ctrl.done,
																	diPtr->dscpPtr->ctrl.pkt_len, 
																	(uint)diPtr->skb, 
																	(uint)diPtr->dscpPtr->pkt_addr,
																	diPtr->dscpPtr->next_idx) ;
				idx++ ;	
			}
			
			if((i&0xFF) == 0xFF) {
				msleep(0) ;
			}
		}
	}
	
	for(ringIdx=0; ringIdx<RX_RING_NUM ; ringIdx++) {
		idx = 1 ;
		diPtr = gpQdmaPriv->rxStartPtr[ringIdx] ;
		printk("\nRx%d DSCP Ring: RxStartIdx:%d, RxEndIdx:%d\n", ringIdx, gpQdmaPriv->rxStartPtr[ringIdx]->dscpIdx, gpQdmaPriv->rxEndPtr[ringIdx]->dscpIdx) ;
		do {
			if(diPtr) {
				printk("%d: DSCP Idx:%d, DSCP Ptr:%.8x, Done Bit:%d, PktLen:%d, PktAddr:%.8x(%.8x), Next Idx:%d\n", 
																	idx, diPtr->dscpIdx, 
																	(uint)diPtr->dscpPtr, 
																	diPtr->dscpPtr->ctrl.done,
																	diPtr->dscpPtr->ctrl.pkt_len, 
																	(uint)diPtr->skb, 
																	(uint)diPtr->dscpPtr->pkt_addr,
																	diPtr->dscpPtr->next_idx) ;
				diPtr = diPtr->next ;
				idx++ ;
			}
		} while(diPtr!=NULL && diPtr!=gpQdmaPriv->rxStartPtr[ringIdx]) ;
	}
	
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to dump all IRQ information.
 Input Args:	None
 Ret Value:	No return value
******************************************************************************/
int qdma_dump_irq_info(struct ECNT_QDMA_Data *qdma_data)
{
	int i=0 ;
	uint *irqPtr=NULL, irqValue=0 ;
	
	for(i=0 ; i<gpQdmaPriv->irqDepth ; i++) {
		irqPtr = (uint *)gpQdmaPriv->irqQueueAddr + i ;
		irqValue = *irqPtr ;
		printk("IRQ Queue:%.4x,     Content:%.8x\n", i, irqValue) ;
		
		if((i&0xFF) == 0xFF) {
			msleep(0) ;
		}
	}
	
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to dump all hwfwd dscp information.
 Input Args:	None
 Ret Value:	No return value
******************************************************************************/
int qdma_dump_hwfwd_info(struct ECNT_QDMA_Data *qdma_data)
{
	int i=0 ;
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_HWFWD_DMA_DSCP_T *pHwDscp=NULL ;
	uint *pHwMsg=NULL, *pHwPkt=NULL ;
	
	printk("Hardware Forwarding DSCP Link List:\n") ;
	for(i=0 ; i<gpQdmaPriv->hwFwdDscpNum ; i++) {
		pHwDscp = (QDMA_HWFWD_DMA_DSCP_T *)(gpQdmaPriv->hwFwdBaseAddr) + i ;
		pHwMsg = pHwDscp->msg ;
		pHwPkt = (uint *)(gpQdmaPriv->hwFwdBuffAddr) + (gpQdmaPriv->hwFwdPayloadSize/sizeof(uint))*i ;
		
		printk("%.4d: DSCP:%.8x(%.8x), ctx:%s, ctx_idx:%d, MsgAddr:%.8x, PktLen:%d, PktAddr:%.8x(%.8x)\n", 
															i, (uint)pHwDscp, 
															qdmaGetHwDscpBase(base)+i*sizeof(QDMA_HWFWD_DMA_DSCP_T),
															pHwDscp->ctrl.ctx?("HWFWD"):("CPU"),
															pHwDscp->ctrl.ctx_idx,
															(uint)pHwMsg,
															pHwDscp->ctrl.pkt_len,
															(uint)pHwPkt,
															(uint)pHwDscp->pkt_addr) ;
		printk("      Msg Content:%.8x, %.8x\n", (uint)(*pHwMsg), (uint)(*(pHwMsg+1))) ;

		if((i&0xFF) == 0xFF) {
			msleep(0) ;
		}
	}
	
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to set dbg level for qdma debug.
 Input Args:	arg1: dbgLevel: 0 for ERROR; 1 for ST; 2 for WARN; 3 for MSG
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_dbg_level(struct ECNT_QDMA_Data *qdma_data)
{
#ifdef CONFIG_DEBUG
	uint dbgLevel = qdma_data->qdma_private.qdma_old_proc.dbgLevel ;

	if(dbgLevel < DBG_LEVEL_MAX) {
		gpQdmaPriv->devCfg.dbgLevel = dbgLevel ;
		printk("Debug Level: %d\n", gpQdmaPriv->devCfg.dbgLevel) ;
		return 0;
	} else {
		printk("Debug Level shoule less than %d\n", DBG_LEVEL_MAX) ;
		return -EINVAL;
	}
#else
	return 0;
#endif
}

/******************************************************************************
 Descriptor:	It's used to dump tx/rx dma busy bit in several seconds.
 Input Args:	arg1: busyDuration means the seconds of dma busy bit print out. 
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_dump_dma_busy(struct ECNT_QDMA_Data *qdma_data)
{
	uint busyDuration = qdma_data->qdma_private.qdma_old_proc.busyDuration ;
	
	if((busyDuration < 0) || (busyDuration > QDMA_BUSY_TIMER_MAX)) {
		printk("dma busy duration force setting to %d seconds.", QDMA_BUSY_TIMER_MAX);
		busyDuration = QDMA_BUSY_TIMER_MAX;
	}
	dma_busy_round_cnt = busyDuration*100 ;
	tx_dma_busy_enable_cnt = 0 ;
	rx_dma_busy_enable_cnt = 0 ;
	
	init_timer(&dma_busy_timer) ;
	dma_busy_timer.data = dma_busy_expires ; /* 10 ms */
	dma_busy_timer.function = qdma_dma_busy_timer ;
	QDMA_START_TIMER(dma_busy_timer) ;

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to dump the value of specific qdma register in several seconds.
 Input Args:	arg1: regOffset means the offset of specific qdma register. 
			arg2: pollingDuration means the seconds of polling time. 
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_dump_reg_polling(struct ECNT_QDMA_Data *qdma_data)
{
	uint regOffset = qdma_data->qdma_private.qdma_old_proc.regOffset ;
	uint pollingDuration = qdma_data->qdma_private.qdma_old_proc.pollingDuration ;
	
	if((regOffset & 0x3) != 0) {
		printk("register offset is %u, which should be an integral multiple of 4.", regOffset);
		return -EINVAL;
	}
	
	if((pollingDuration < 0) || (pollingDuration > QDMA_REG_POLLING_MAX)) {
		printk("register polling duration force setting to %d seconds.", QDMA_REG_POLLING_MAX);
		pollingDuration = QDMA_REG_POLLING_MAX;
	}	
	reg_polling_round_cnt = pollingDuration*100 ;
	reg_polling_offset = regOffset ;
	
	init_timer(&reg_polling_timer) ;
	reg_polling_timer.data = reg_polling_expires ; /* 10 ms */
	reg_polling_timer.function = qdma_reg_polling_timer ;
	QDMA_START_TIMER(reg_polling_timer) ;
	
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to set receive rx ring1 packets only: rx1:rx0 from 5:2 to 5:0.
 Input Args:	arg1: forceEn means enable/disable force receive rx1 function.
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_force_receive_rx_ring1(struct ECNT_QDMA_Data *qdma_data)
{
	uint forceEn = qdma_data->qdma_private.qdma_old_proc.forceEn ;
	
	isForceRX1 = ((forceEn==QDMA_DISABLE)?(0):(1));
	printk("isForceRX1: %d\n", isForceRX1) ;
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to enable or disable TX_DROP_EN.
 Input Args:	arg1: mode means enable or disable.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_tx_drop_en(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_Mode_t txDropMode = qdma_data->qdma_private.mode;

	if(txDropMode == QDMA_DISABLE) {
		qdmaDisableTxqCngstDrop(base) ;
	} else {
		qdmaEnableTxqCngstDrop(base) ;
	}

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get the value of TX_DROP_EN.
 Ret Value:		0: Disable, 1: Enable.
******************************************************************************/
int qdma_get_tx_drop_en(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;

	if(qdmaIsTxqCngstDropEnable(base)) {
		qdma_data->qdma_private.mode = QDMA_ENABLE ;
	} else {
		qdma_data->qdma_private.mode = QDMA_DISABLE ;
	}
	
	return qdma_data->qdma_private.mode ;
}

/******************************************************************************
 Descriptor:	It's used to set the value of tx ratemeter config. It includes
 			timeDivisor and timeSlice, while tx_rateMeter_En always enable.
 Input Args:	arg1: The pointer of the  Tx RateMeter Config struct.
                     tx ratemeter default enable.
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_tx_ratemeter(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxRateMeter_T *txRateMeterPtr = qdma_data->qdma_private.txRateMeterPtr;

	if(txRateMeterPtr->txRateMeterTimeDivisor<0 || txRateMeterPtr->txRateMeterTimeDivisor>=QDMA_TX_RATE_METER_TIME_DIVISOR_ITEMS) {
		QDMA_ERR("Fault: TX Rate Meter Time Divisor value shoule between 0 and %d\n", QDMA_TX_RATE_METER_TIME_DIVISOR_ITEMS-1) ; 
		return -EINVAL ;
	}

	qdmaSetTxRateMeterTimeDivisor(base, txRateMeterPtr->txRateMeterTimeDivisor) ;
	qdmaSetTxRateMeterTimeSlice(base, txRateMeterPtr->txRateMeterTimeSlice) ;
	
	return 0 ;
}


/******************************************************************************
 Descriptor:	It's used to get the value of tx ratemeter config. It includes
 			timeDivisor and timeSlice, while tx_rateMeter_En always enable.
 Input Args:	arg1: The pointer of the  Tx RateMeter Config struct. It includes
 				- timeDivisor: the divisor of timeSlice, which will affect datarate.
 				- timeSlice: ratemeter calculate duration.
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_get_tx_ratemeter(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxRateMeter_T *txRateMeterPtr = qdma_data->qdma_private.txRateMeterPtr;

	txRateMeterPtr->txRateMeterTimeDivisor = qdmaGetTxRateMeterTimeDivisor(base) ;
	txRateMeterPtr->txRateMeterTimeSlice = qdmaGetTxRateMeterTimeSlice(base) ;
	
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get tx channel datarate.
 Input Args:	arg1: channel index
 Ret Value:		return tx datarate by channel, unit is bps
******************************************************************************/
int qdma_get_tx_channel_datarate(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxRateMeter_T txRateMeter;
	long dataRate = 0;
	int chnlIdx = qdma_data->qdma_private.channel;

	dataRate = qdmaGetTxPerChnlDataRate(base, chnlIdx);
	qdma_data->qdma_private.txRateMeterPtr = &txRateMeter;
	qdma_get_tx_ratemeter(qdma_data);
	if(txRateMeter.txRateMeterTimeSlice)
		dataRate = ((dataRate<<3)<<4) * 1000000 * (1<<txRateMeter.txRateMeterTimeDivisor) / txRateMeter.txRateMeterTimeSlice ;
	else 
		dataRate = -1;
	
	return (int)dataRate ;
}

/******************************************************************************
 Descriptor:	It's used to enable tx global ratelimit function. 
 Input Args:	No input arguments
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_enable_tx_ratelimit(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;	
	qdmaEnableTxRateLimit(base) ;	
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to read VIP info from fe. 
 Input Args:	None
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_read_vip_info(void)
{
	unsigned int en_val;
	unsigned int patn_val;
	int i;
	/*clear info*/
	qdma_vip_num = 0 ;
	memset((char *)qdma_vip_info, 0, sizeof(QDMA_VIP_INFO_T)*QDMA_VIP_PACKET_NUM);
	
	for( i = 0 ; i < QDMA_VIP_PACKET_NUM ; i++ )
	{
		en_val = read_reg_word(FE_VIP_EN(i));
		if ((en_val & 1) == 0)/*disable ,not care */
			continue;
		
		/*0: ETH_TYPE ; 1:PPPOE protocol ; 2:IP protocol; 3:TCP ports; 4:UDP ports*/
		if ( ((en_val >> VIP_EN_TYPE_OFFSET) & 0x07) < 3 )/*only care TCP ports & UDP ports*/
			continue;	

		qdma_vip_info[qdma_vip_num].ip_type = (((en_val >> VIP_EN_TYPE_OFFSET) & 0x07) == 3)? 6:17 ;/*tcp or udp*/
		/*swap the mode ,for qdma->sw and sw->qdma,are opposite*/
		qdma_vip_info[qdma_vip_num].sport_en = (en_val >> VIP_EN_DPEN_OFFSET ) & 0x01 ;
		qdma_vip_info[qdma_vip_num].dport_en = (en_val >> VIP_EN_SPEN_OFFSET ) & 0x01 ;
		qdma_vip_info[qdma_vip_num].swap_en = (en_val >> VIP_EN_SWEN_OFFSET ) & 0x01 ;

		patn_val = read_reg_word(FE_VIP_PATN(i));

		/*swap the sport and dport ,for qdma->sw and sw->qdma,are opposite*/
		qdma_vip_info[qdma_vip_num].sport = (patn_val >> 16) & 0xffff ;
		qdma_vip_info[qdma_vip_num].dport =  patn_val & 0xffff ;

		qdma_vip_num ++ ;
	}

#if 0
	/*show all info for debug*/
	printk("\n\t------------qdma vip packet info show------------\n");
	printk("\tsport \tdport \tsp_en \tdp_en \tsw_en \tip_type\n");
	for( i = 0 ; i < qdma_vip_num ; i++ )
	{
		printk("\t%d \t%d \t%d \t%d \t%d \t%d\n",qdma_vip_info[i].sport,qdma_vip_info[i].dport,
			qdma_vip_info[i].sport_en, qdma_vip_info[i].dport_en, qdma_vip_info[i].swap_en,qdma_vip_info[i].ip_type);
	}
#endif

	return 0 ;
}

#if 1
/******************************************************************************
 Descriptor:	It's used to set the value of tx rate limit config. It includes bucketScale and ticksel.
 Input Args:	arg1: The pointer of the  Tx Rate Limit Config struct.It includes
 			- txRateLimitUnit: unit is 1kbps, scope is (1kbps~65kbps). 0 means not change.
 			- txRateLimitBucketScale: ratemeter calculate duration.
 			- txRateLimitEn: default enable, ratelimit enable or not decide by chnlRateLimitEn.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_tx_ratelimit_cfg(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	int maxLimitRate=0, curUnit=0;
	int curTicksel=0, newLimitRate=0;
	QDMA_TxRateLimitCfg_T *txRateLimitCfgPtr = qdma_data->qdma_private.txRateLimitCfgPtr;
	
	if(txRateLimitCfgPtr->txRateLimitBucketScale<0 || txRateLimitCfgPtr->txRateLimitBucketScale>=QDMA_TRTCM_SCALE_MAX_ITEMS) {
		QDMA_ERR("Fault: TX Rate Limit Bucket Scale shoule between 0 and %d\n", QDMA_TRTCM_SCALE_MAX_ITEMS-1) ; 
		return -EINVAL ;
	}
	if(txRateLimitCfgPtr->txRateLimitUnit<1 || txRateLimitCfgPtr->txRateLimitUnit>65) {
		QDMA_ERR("Fault: TX Rate Limit unit shoule between 1kbps and 65kbps\n") ; 
		return -EINVAL ;
	}
	
	/* get current rate limit unit */
	curTicksel = qdmaGetTxRateLimitTicksel(base);
	if(curTicksel == 0) 
		curUnit = 0 ;
	else
		curUnit = 8000/curTicksel ;
	
	/* if setting ratelimit unit is not equal with current unit, should update all channel's rate limit value */
	if(curUnit != txRateLimitCfgPtr->txRateLimitUnit) {
		/* get max tx rate limit value, scope is (0~65535Byte) */
		maxLimitRate = qdmaGetLimitRateMax();
		if(maxLimitRate < 0) {
			QDMA_ERR("Fault: maxLimitRate is not correct\n") ; 
			return -EINVAL ;
		}
		
		/* if setting unit is too small, so that pir would larger than 0xFFFF, then exit*/
		newLimitRate = maxLimitRate*curUnit/txRateLimitCfgPtr->txRateLimitUnit;
		if(newLimitRate > 0xFFFF) {
			QDMA_ERR("Fault: new pir would be 0x%X, should not larger than 0xFFFF\n", newLimitRate) ; 
			return -EINVAL ;
		}
		
		/* update all channel's rate limit value */
		qdmaUpdateAllTxRateLimitValue(curUnit, txRateLimitCfgPtr->txRateLimitUnit) ;
	}
	
	qdmaSetTxRateLimitBucketScale(base, txRateLimitCfgPtr->txRateLimitBucketScale) ;
	if(txRateLimitCfgPtr->txRateLimitUnit) {
		qdmaSetTxRateLimitTicksel(base, 8000/txRateLimitCfgPtr->txRateLimitUnit) ;
	}
	
	return 0 ;
}
#endif

/******************************************************************************
 Descriptor:	It's used to get the value of tx rate limit config. It includes
 			tx_rateLimit_En, bucketScale and ticksel.
 Input Args:	arg1: The pointer of the  Tx Rate Limit Config struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_get_tx_ratelimit_cfg(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	int curTicksel=0;
	QDMA_TxRateLimitCfg_T *txRateLimitCfgPtr = qdma_data->qdma_private.txRateLimitCfgPtr;

	curTicksel = qdmaGetTxRateLimitTicksel(base);
	if(curTicksel == 0)
		txRateLimitCfgPtr->txRateLimitUnit = 0 ;
	else
		txRateLimitCfgPtr->txRateLimitUnit = 8000/curTicksel ;
	txRateLimitCfgPtr->txRateLimitBucketScale = qdmaGetTxRateLimitBucketScale(base) ;
	
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to set the value of tx rate limit parameter. It includes chnlRateLimitEn and PIR.
 Input Args:	arg1: The pointer of the TX RateLimit Parameter struct.It includes
 			- chnlRateLimitEn: means current channel ratelimit enable or disable.
 			- PIR: unit is kbps, scope is [1kbps~1Gbps]
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_tx_ratelimit(struct ECNT_QDMA_Data *qdma_data)
{
	int ret=0 , enMode=0;
	int curTicksel=0, curUnit=0, newUnit=0, switchLeastUnit=0 ;
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxRateLimitSet_T *txRateLimitPtr = qdma_data->qdma_private.txRateLimitSetPtr;
	int port = 0;
	int switch_port =0;
	
	if(txRateLimitPtr->chnlIdx<0 || txRateLimitPtr->chnlIdx>=CONFIG_QDMA_CHANNEL) {
		QDMA_ERR("Fault: channel index shoule between 0 and %d\n", CONFIG_QDMA_CHANNEL-1) ; 
		return -EINVAL ;
	}
	
	if(TCSUPPORT_FWC_ENV_VAL)
	{
		if( 0 == txRateLimitPtr->chnlIdx ) /*FWC only use channel 0*/
		{
			if( ENABLE == txRateLimitPtr->chnlRateLimitEn && txRateLimitPtr->rateLimitValue < CHANNEL_RATELIMIT_THRESHOLD )
				ETHER_SET_RATELIMIT_SWITCH( ENABLE );
			else
				ETHER_SET_RATELIMIT_SWITCH( DISABLE );
		}

#ifdef QDMA_LAN
		if(0 != txRateLimitPtr->chnlIdx)
		{
			port = txRateLimitPtr->chnlIdx - 1;   
#if defined(TCSUPPORT_CPU_MT7520)
			if(MT7530LanPortMap2Switch_hook)
			{
				switch_port = MT7530LanPortMap2Switch_hook(port);
			}
#endif
			txRateLimitPtr->chnlIdx = switch_port;
		}  
#endif

	}
	
	if(txRateLimitPtr->chnlRateLimitEn == QDMA_DISABLE) {
		qdmaDisableChnlRateLimit(base, txRateLimitPtr->chnlIdx) ;
	} else {
		enMode = qdmaIsChnlRateLimitEnable(base, txRateLimitPtr->chnlIdx) ;
		qdmaEnableChnlRateLimit(base, txRateLimitPtr->chnlIdx) ;
		if(!enMode) {	/* change enMode from disable to enable */
			curTicksel = qdmaGetTxRateLimitTicksel(base) ;
			qdmaSetTxRateLimitTicksel(base, 1) ;
			qdmaSetTxRateLimitConfig(txRateLimitPtr->chnlIdx, QDMA_Tx_Rate_Limit_PIR, 0xFFFF) ;
			mdelay(100) ;
			qdmaSetTxRateLimitTicksel(base, curTicksel) ;
		}
	}

	/* get new unit */
	newUnit = 1 + txRateLimitPtr->rateLimitValue / 0xFFFF ;
#ifdef QDMA_LAN
	/* to make sure qdma_lan burst size smaller than 40Kbyte */
	switchLeastUnit = txRateLimitPtr->rateLimitValue / (SWITCH_BUFFER_THRESHOLD*1000) ;
	newUnit = MAX(newUnit, switchLeastUnit) ;
#endif

	/* get current unit */
	curTicksel = qdmaGetTxRateLimitTicksel(base) ;
	if(curTicksel == 0)
		curUnit = 0 ;
	else
		curUnit = 8000 / curTicksel ;
	
	/* update ratelimit unit and all channel's pir value */		
	if(newUnit > curUnit)
	{
		curTicksel = 8000 / newUnit;
		qdmaSetTxRateLimitTicksel(base, curTicksel) ;
		qdmaUpdateAllTxRateLimitValue(curUnit, newUnit) ;
		curUnit = newUnit;
	}
	
	if((ret = qdmaSetTxRateLimitConfig(txRateLimitPtr->chnlIdx, QDMA_Tx_Rate_Limit_PIR, txRateLimitPtr->rateLimitValue / curUnit)) < 0) {
		return ret ;
	}
	/* pbs = 2*0.625*pir */
	if((ret = qdmaSetTxRateLimitConfig(txRateLimitPtr->chnlIdx, QDMA_Tx_Rate_Limit_PBS, ((txRateLimitPtr->rateLimitValue>>1) + (txRateLimitPtr->rateLimitValue>>3)) / curUnit)) < 0) {
		return ret ;
	}

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get the value of tx rate limit parameter. It includes PIR and PBS.
 Input Args:	arg1: The pointer of the  TX RateLimit Parameter struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_get_tx_ratelimit(struct ECNT_QDMA_Data *qdma_data) 
{
	int ret = 0 ;
	uint base = gpQdmaPriv->csrBaseAddr ;
	int curTicksel=0, curUnit=0 ;
	QDMA_TxRateLimitGet_T *txRateLimitPtr = qdma_data->qdma_private.txRateLimitGetPtr;
	int port = 0;
	int switch_port =0;
	
	if(txRateLimitPtr->chnlIdx<0 || txRateLimitPtr->chnlIdx>=CONFIG_QDMA_CHANNEL) {
		QDMA_ERR("Fault: channel index shoule between 0 and %d\n", CONFIG_QDMA_CHANNEL-1) ; 
		return -EINVAL ;
	}

	if(TCSUPPORT_FWC_ENV_VAL)
	{
#ifdef QDMA_LAN
		if(0 != txRateLimitPtr->chnlIdx)
		{
			port = txRateLimitPtr->chnlIdx - 1;   
#if defined(TCSUPPORT_CPU_MT7520)
			if(MT7530LanPortMap2Switch_hook)
			{
				switch_port = MT7530LanPortMap2Switch_hook(port);
			}
#endif
			txRateLimitPtr->chnlIdx = switch_port;
		}  
#endif
	}  

	curTicksel = qdmaGetTxRateLimitTicksel(base);
	if(curTicksel == 0) 
		curUnit = 0 ;
	else
		curUnit = 8000 / curTicksel ;
	
	if(qdmaIsChnlRateLimitEnable(base, txRateLimitPtr->chnlIdx)) {
		txRateLimitPtr->chnlRateLimitEn = QDMA_ENABLE ;
	} else {
		txRateLimitPtr->chnlRateLimitEn = QDMA_DISABLE ;
	}
	
	ret = qdmaGetTxRateLimitConfig(txRateLimitPtr->chnlIdx, QDMA_Tx_Rate_Limit_PIR) ;
	if(ret < 0)
		return ret ;
	txRateLimitPtr->rateLimitValue = ret * curUnit ;
	
	ret = qdmaGetTxRateLimitConfig(txRateLimitPtr->chnlIdx, QDMA_Tx_Rate_Limit_PBS) ;
	if(ret < 0)
		return ret ;
	txRateLimitPtr->pbsParamValue = ret * (1<<qdmaGetTxRateLimitBucketScale(base)) ;
		
	return 0 ;
}


/******************************************************************************
 Descriptor:	It's used to set the value of tx dba report parameter. It includes CIR, CBS, PIR and PBS.
 Input Args:	arg1: The pointer of the TX RateLimit Parameter struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_tx_dba_report(struct ECNT_QDMA_Data *qdma_data)
{
	int ret = 0 ;
	uint base = gpQdmaPriv->csrBaseAddr, maxPirValue=0;
	int curTicksel=0, curUnit=0, newUnit=0 ;
	QDMA_TxDbaReport_T *txDbaReportPtr = qdma_data->qdma_private.txDbaReportPtr;
	
	if(txDbaReportPtr->channel<0 || txDbaReportPtr->channel>=CONFIG_QDMA_CHANNEL) {
		QDMA_ERR("Fault: channel index shoule between 0 and %d\n", CONFIG_QDMA_CHANNEL-1) ; 
		return -EINVAL ;
	}
	
	if(txDbaReportPtr->pirParamValue > txDbaReportPtr->cirParamValue)
		maxPirValue = txDbaReportPtr->pirParamValue ;
	else
		maxPirValue = txDbaReportPtr->cirParamValue ;
	
	/* get new unit and current unit */
	newUnit = 1 + maxPirValue / 0xFFFF;
	curTicksel = qdmaGetTxRateLimitTicksel(base);
	if(curTicksel == 0)
		curUnit = 0 ;
	else
		curUnit = 8000 / curTicksel ;
	
	/* update ratelimit unit and all channel's pir value */		
	if(newUnit > curUnit) {
		curTicksel = 8000 / newUnit;
		qdmaSetTxRateLimitTicksel(base, curTicksel) ;
		qdmaUpdateAllTxRateLimitValue(curUnit, newUnit) ;
		curUnit = newUnit;
	}
	
	if((ret = qdmaSetTxRateLimitConfig(txDbaReportPtr->channel, QDMA_Tx_Rate_Limit_CIR, txDbaReportPtr->cirParamValue / curUnit)) < 0) {
		return ret ;
	}
	if((ret = qdmaSetTxRateLimitConfig(txDbaReportPtr->channel, QDMA_Tx_Rate_Limit_CBS, txDbaReportPtr->cbsParamValue / (1<<qdmaGetTxRateLimitBucketScale(base)))) < 0) {
		return ret ;
	}
	if((ret = qdmaSetTxRateLimitConfig(txDbaReportPtr->channel, QDMA_Tx_Rate_Limit_PIR, txDbaReportPtr->pirParamValue / curUnit)) < 0) {
		return ret ;
	}
	if((ret = qdmaSetTxRateLimitConfig(txDbaReportPtr->channel, QDMA_Tx_Rate_Limit_PBS, txDbaReportPtr->pbsParamValue) / (1<<qdmaGetTxRateLimitBucketScale(base))) < 0) {
		return ret ;
	}
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get the value of tx rate limit parameter. It includes PIR and PBS.
 Input Args:	arg1: The pointer of the  TX RateLimit Parameter struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_get_tx_dba_report(struct ECNT_QDMA_Data *qdma_data) 
{
	int ret = 0 ;
	uint base = gpQdmaPriv->csrBaseAddr ;
	int curTicksel=0, curUnit=0 ;
	QDMA_TxDbaReport_T *txDbaReportPtr = qdma_data->qdma_private.txDbaReportPtr;
	
	if(txDbaReportPtr->channel<0 || txDbaReportPtr->channel>=CONFIG_QDMA_CHANNEL) {
		QDMA_ERR("Fault: channel index shoule between 0 and %d\n", CONFIG_QDMA_CHANNEL-1) ; 
		return -EINVAL ;
	}
	
	curTicksel = qdmaGetTxRateLimitTicksel(base);
	if(curTicksel == 0) 
		curUnit = 0 ;
	else
		curUnit = 8000 / curTicksel ;
			
	ret = qdmaGetTxRateLimitConfig(txDbaReportPtr->channel, QDMA_Tx_Rate_Limit_CIR) ;
	if(ret < 0)
		return ret ;
	txDbaReportPtr->cirParamValue = ret * curUnit ;
	
	ret = qdmaGetTxRateLimitConfig(txDbaReportPtr->channel, QDMA_Tx_Rate_Limit_CBS) ;
	if(ret < 0)
		return ret ;
	txDbaReportPtr->cbsParamValue = ret * (1<<qdmaGetTxRateLimitBucketScale(base)) ;
	
	ret = qdmaGetTxRateLimitConfig(txDbaReportPtr->channel, QDMA_Tx_Rate_Limit_PIR) ;
	if(ret < 0)
		return ret ;
	txDbaReportPtr->pirParamValue = ret * curUnit ;
	
	ret = qdmaGetTxRateLimitConfig(txDbaReportPtr->channel, QDMA_Tx_Rate_Limit_PBS) ;
	if(ret < 0)
		return ret ;
	txDbaReportPtr->pbsParamValue = ret * (1<<qdmaGetTxRateLimitBucketScale(base)) ;
		
	return 0 ;
}


/******************************************************************************
 Descriptor:	It's used to enable/disable rx protect mode. 
 Input Args:	arg1: Enable/disable rx protect mode (QDMA_ENABLE/QDMA_DISABLE)
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_rx_protect_en(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_Mode_t rxDropMode = qdma_data->qdma_private.mode;
	
	if(rxDropMode == QDMA_ENABLE) {
		qdmaEnableRxDrop(base) ;
	} else {
		qdmaDisableRxDrop(base) ;
	}
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to get rx protect mode:enable/disable.
 Input Args:	No input arguments
 Ret Value:		return rx protect mode (QDMA_ENABLE/QDMA_DISABLE)
******************************************************************************/
int qdma_get_rx_protect_en(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	if(qdmaIsRxDropEnable(base)) {
		qdma_data->qdma_private.mode = QDMA_ENABLE ;
	} else {
		qdma_data->qdma_private.mode = QDMA_DISABLE ;
	}

	return qdma_data->qdma_private.mode ;
}

/******************************************************************************
 Descriptor:	It's used to set the value of rx low threshold. 
 Input Args:	arg1: The pointer of the RX Low Threshold struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_rx_low_threshold(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_RxLowThreshold_T *rxLowThresholdPtr = qdma_data->qdma_private.rxLowThresholdPtr;

	if((rxLowThresholdPtr->rxRing0LowThrh > qdmaGetRxRingSize(base, RING_IDX_0))
	|| (rxLowThresholdPtr->rxRing1LowThrh > qdmaGetRxRingSize(base, RING_IDX_1)))
	{
		QDMA_ERR("Fault: rxRing0LowThrh is %d, should less than %d; rxRing1LowThrh is %d, should less than %d\n"
			, rxLowThresholdPtr->rxRing0LowThrh, qdmaGetRxRingSize(base, RING_IDX_0)
			, rxLowThresholdPtr->rxRing1LowThrh, qdmaGetRxRingSize(base, RING_IDX_1)) ; 
		return -EINVAL ;
	}

	qdmaSetRxRingThrh(base, RING_IDX_0, rxLowThresholdPtr->rxRing0LowThrh) ;
	qdmaSetRxRingThrh(base, RING_IDX_1, rxLowThresholdPtr->rxRing1LowThrh) ;
	
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get the value of rx low threshold. 
 Input Args:	arg1: The pointer of the RX Low Threshold struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_get_rx_low_threshold(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_RxLowThreshold_T *rxLowThresholdPtr = qdma_data->qdma_private.rxLowThresholdPtr;

	rxLowThresholdPtr->rxRing0LowThrh = qdmaGetRxRingThrh(base, RING_IDX_0) ;
	rxLowThresholdPtr->rxRing1LowThrh = qdmaGetRxRingThrh(base, RING_IDX_1) ;
	
	return 0;
}

#if 0
/******************************************************************************
 Descriptor:	It's used to set the value of rx rate limit config. It includes
 			rx_rateLimit_En, bucketScale and tickSel.
 Input Args:	arg1: The pointer of the RX Rate Limit config struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_rx_ratelimit_cfg(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	int maxLimitRate=0, curUnit=0, unitBase=0;	//unitBase means the unit when ticksel is 1us.
	int curTicksel=0, newLimitRate=0, ringIdx=0;
	uint rxRateLimitPir[RX_RING_NUM], rxRateLimitPbs[RX_RING_NUM] ;
	QDMA_RxRateLimitCfg_T *rxRateLimitCfgPtr = qdma_data->qdma_private.rxRateLimitCfgPtr;

	if(rxRateLimitCfgPtr->rxRateLimitBucketScale<0 || rxRateLimitCfgPtr->rxRateLimitBucketScale>=QDMA_TRTCM_SCALE_MAX_ITEMS) {
		QDMA_ERR("Fault: Rx Rate Limit Bucket Scale shoule between 0 and %d\n", QDMA_TRTCM_SCALE_MAX_ITEMS-1) ; 
		return -EINVAL ;
	}
	
	if(rxRateLimitCfgPtr->rxRateLimitPktMode == QDMA_RX_RATE_LIMIT_BY_PACKET) {
		qdmaSetRxRateLimitByPacket(base) ;
		if(rxRateLimitCfgPtr->rxRateLimitUnit<16 || rxRateLimitCfgPtr->rxRateLimitUnit>10000) {
			QDMA_ERR("Fault: RX Rate Limit unit shoule between 16pps and 10000pps\n") ; 
			return -EINVAL ;
		}
		unitBase = 1000000;		//unitBase equal 1000000 means 1000000pps when tickel is 1us.
	} else {
		qdmaSetRxRateLimitByByte(base) ;
		if(rxRateLimitCfgPtr->rxRateLimitUnit<1 || rxRateLimitCfgPtr->rxRateLimitUnit>65) {
			QDMA_ERR("Fault: RX Rate Limit unit shoule between 1kbps and 65kbps\n") ; 
			return -EINVAL ;
		}
		unitBase = 8000;	//unitBase equal 8000 means 8000kbps when tickel is 1us
	}
	
	/* get current rate limit unit */
	curTicksel = qdmaGetRxRateLimitTicksel(base);
	if(curTicksel == 0) 
		curUnit = 0 ;
	else
		curUnit = unitBase/curTicksel ;
	
	/* if setting ratelimit unit is not equal with current unit, should update all channel's rate limit value */
	if(curUnit != rxRateLimitCfgPtr->rxRateLimitUnit) {
		/* get max tx rate limit value, scope is (0~65535Byte) */
		maxLimitRate = qdmaGetLimitRateMax();
		if(maxLimitRate < 0) {
			QDMA_ERR("Fault: maxLimitRate is not correct\n") ; 
			return -EINVAL ;
		}
		
		/* if setting unit is too small, so that pir would larger than 0xFFFF, then exit*/
		newLimitRate = maxLimitRate*curUnit/rxRateLimitCfgPtr->rxRateLimitUnit;
		if(newLimitRate > 0xFFFF) {
			QDMA_ERR("Fault: new pir would be 0x%X, should not larger than 0xFFFF\n", newLimitRate) ; 
			return -EINVAL ;
		}
		
		/* update all channel's rate limit value */
		qdmaUpdateAllRxRateLimitValue(curUnit, rxRateLimitCfgPtr->rxRateLimitUnit) ;
	}
	
	qdmaSetRxRateLimitTicksel(base, unitBase/rxRateLimitCfgPtr->rxRateLimitUnit) ;
	qdmaSetRxRateLimitBucketScale(base, rxRateLimitCfgPtr->rxRateLimitBucketScale) ;
	
	if(rxRateLimitCfgPtr->rxRateLimitEn == QDMA_DISABLE) {
	/* fix EN7512 hw bug: enable rx ratelimit and then disable it will cause rx traffic not work */
	#if defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
		curTicksel = qdmaGetRxRateLimitTicksel(base);
		qdmaSetRxRateLimitTicksel(base, 500) ;
		for(ringIdx=0; ringIdx<RX_RING_NUM; ringIdx++) {
			rxRateLimitPir[ringIdx] = qdmaGetRxRateLimitConfig(ringIdx, QDMA_Rx_Rate_Limit_PIR) ;
			rxRateLimitPbs[ringIdx] = qdmaGetRxRateLimitConfig(ringIdx, QDMA_Rx_Rate_Limit_PBS) ;
			qdmaSetRxRateLimitConfig(ringIdx, QDMA_Rx_Rate_Limit_PIR, 0xFFFF) ;
			qdmaSetRxRateLimitConfig(ringIdx, QDMA_Rx_Rate_Limit_PBS, 0xFFFF) ;
		}
		mdelay(2) ;
		qdmaDisableRxRateLimit(base) ;
		for(ringIdx=0; ringIdx<RX_RING_NUM; ringIdx++) {
			qdmaSetRxRateLimitConfig(ringIdx, QDMA_Rx_Rate_Limit_PIR, rxRateLimitPir[ringIdx]) ;
			qdmaSetRxRateLimitConfig(ringIdx, QDMA_Rx_Rate_Limit_PBS, rxRateLimitPbs[ringIdx]) ;
		}
		qdmaSetRxRateLimitTicksel(base, curTicksel) ;
	#else
		qdmaDisableRxRateLimit(base) ;
	#endif
	} else {
		qdmaEnableRxRateLimit(base) ;
	}
	return 0;
}
#endif


/******************************************************************************
 Descriptor:	It's used to set the value of rx rate limit config. It includes
 			rx_rateLimit_En, bucketScale and tickSel.
 Input Args:	arg1: The pointer of the RX Rate Limit config struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_rx_ratelimit_en(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	int curTicksel=0, ringIdx=0;
	uint rxRateLimitPir[RX_RING_NUM], rxRateLimitPbs[RX_RING_NUM] ;
	QDMA_Mode_t rxRateLimitEn = qdma_data->qdma_private.mode;
		
	if(rxRateLimitEn == QDMA_DISABLE) {
	/* fix EN7512 hw bug: enable rx ratelimit and then disable it will cause rx traffic not work */
#if defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
		curTicksel = qdmaGetRxRateLimitTicksel(base);
		qdmaSetRxRateLimitTicksel(base, 500) ;
		for(ringIdx=0; ringIdx<RX_RING_NUM; ringIdx++) {
			rxRateLimitPir[ringIdx] = qdmaGetRxRateLimitConfig(ringIdx, QDMA_Rx_Rate_Limit_PIR) ;
			rxRateLimitPbs[ringIdx] = qdmaGetRxRateLimitConfig(ringIdx, QDMA_Rx_Rate_Limit_PBS) ;
			qdmaSetRxRateLimitConfig(ringIdx, QDMA_Rx_Rate_Limit_PIR, 0xFFFF) ;
			qdmaSetRxRateLimitConfig(ringIdx, QDMA_Rx_Rate_Limit_PBS, 0xFFFF) ;
		}
		mdelay(2) ;
		qdmaDisableRxRateLimit(base) ;
		for(ringIdx=0; ringIdx<RX_RING_NUM; ringIdx++) {
			qdmaSetRxRateLimitConfig(ringIdx, QDMA_Rx_Rate_Limit_PIR, rxRateLimitPir[ringIdx]) ;
			qdmaSetRxRateLimitConfig(ringIdx, QDMA_Rx_Rate_Limit_PBS, rxRateLimitPbs[ringIdx]) ;
		}
		qdmaSetRxRateLimitTicksel(base, curTicksel) ;
#else
		qdmaDisableRxRateLimit(base) ;
#endif
	} else {
		qdmaEnableRxRateLimit(base) ;
	}
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to set the value of rx rate limit config. It includes
 			rx_rateLimit_En, bucketScale and tickSel.
 Input Args:	arg1: The pointer of the RX Rate Limit config struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_rx_ratelimit_pkt_mode(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_RxPktMode_t pktMode = qdma_data->qdma_private.pktMode;

	if(pktMode == QDMA_RX_RATE_LIMIT_BY_PACKET) {
		qdmaSetRxRateLimitByPacket(base) ;
		/* set packet mode unit to 16pps */
		qdmaSetRxRateLimitTicksel(base, 62500) ;
	} else {
		qdmaSetRxRateLimitByByte(base) ;
		/* set byte mode unit to 8kbps */
		qdmaSetRxRateLimitTicksel(base, 1000) ;
	}
	
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get the value of rx rate limit config. It includes
 			rx_rateLimit_En, bucketScale and tickSel.
 Input Args:	arg1: The pointer of the RX Rate Limit config struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_get_rx_ratelimit_cfg(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	int curTicksel=0, unitBase=0;	//unitBase means the unit when ticksel is 1us.
	QDMA_RxRateLimitCfg_T *rxRateLimitCfgPtr = qdma_data->qdma_private.rxRateLimitCfgPtr;

	if(qdmaIsRxRateLimitEnable(base)) {
		rxRateLimitCfgPtr->rxRateLimitEn = QDMA_ENABLE ;
	} else {
		rxRateLimitCfgPtr->rxRateLimitEn = QDMA_DISABLE ;
	}
	
	if(qdmaIsRxRateLimitByPacket(base)) {
		rxRateLimitCfgPtr->rxRateLimitPktMode = QDMA_RX_RATE_LIMIT_BY_PACKET ;
		unitBase = 1000000 ;
	} else {
		rxRateLimitCfgPtr->rxRateLimitPktMode = QDMA_RX_RATE_LIMIT_BY_BYTE ;
		unitBase = 8000 ;
	}
	
	curTicksel = qdmaGetRxRateLimitTicksel(base);
	if(curTicksel == 0)
		rxRateLimitCfgPtr->rxRateLimitUnit = 0 ;
	else
		rxRateLimitCfgPtr->rxRateLimitUnit = unitBase/curTicksel ;
	rxRateLimitCfgPtr->rxRateLimitBucketScale = qdmaGetRxRateLimitBucketScale(base) ;
	
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to set the value of rx rate limit parameter. It includes PIR and PBS.
 Input Args:	arg1: The pointer of the RX RateLimit Parameter struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_rx_ratelimit(struct ECNT_QDMA_Data *qdma_data)
{
	int ret = 0, unitBase=0;	//unitBase means the unit when ticksel is 1us.
	int curTicksel=0, curUnit=0, newUnit=0 ;
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_RxRateLimitSet_T *rxRateLimitPtr = qdma_data->qdma_private.rxRateLimitSetPtr;
	
	if(rxRateLimitPtr->ringIdx<0 || rxRateLimitPtr->ringIdx>=RX_RING_NUM) {
		QDMA_ERR("Fault: ringIdx shoule be 0 or 1\n") ; 
		return -EINVAL ;
	}
	
	if(qdmaIsRxRateLimitByPacket(base)) {
		unitBase = 1000000 ;
	} else {
		unitBase = 8000 ;
	}
	
	/* get new unit and current unit */
	newUnit = 1 + rxRateLimitPtr->rateLimitValue / 0xFFFF;
	curTicksel = qdmaGetRxRateLimitTicksel(base);
	if(curTicksel == 0) 
		curUnit = 0 ;
	else
		curUnit = unitBase / curTicksel ;
	
	/* update ratelimit unit and all channel's pir value */		
	if(newUnit > curUnit) {
		curTicksel = unitBase / newUnit;
		qdmaSetRxRateLimitTicksel(base, curTicksel) ;
		qdmaUpdateAllRxRateLimitValue(curUnit, newUnit) ;
		curUnit = newUnit;
	}
	
	if((ret = qdmaSetRxRateLimitConfig(rxRateLimitPtr->ringIdx, QDMA_Rx_Rate_Limit_PIR, rxRateLimitPtr->rateLimitValue / curUnit)) < 0) {
		return ret ;
	}
	if((ret = qdmaSetRxRateLimitConfig(rxRateLimitPtr->ringIdx, QDMA_Rx_Rate_Limit_PBS, rxRateLimitPtr->rateLimitValue / curUnit)) < 0) {
		return ret ;
	}

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get the value of rx rate limit parameter. It includes PIR and PBS.
 Input Args:	arg1: The pointer of the  RX RateLimit Parameter struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_get_rx_ratelimit(struct ECNT_QDMA_Data *qdma_data) 
{
	int ret = 0 , unitBase=0;	/* unitBase means the unit when ticksel is 1us. */
	uint base = gpQdmaPriv->csrBaseAddr ;
	int curTicksel=0, curUnit=0 ;
	QDMA_RxRateLimitGet_T *rxRateLimitPtr = qdma_data->qdma_private.rxRateLimitGetPtr;
	
	if(rxRateLimitPtr->ringIdx<0 || rxRateLimitPtr->ringIdx>=RX_RING_NUM) {
		QDMA_ERR("Fault: ringIdx shoule be 0 or 1\n") ; 
		return -EINVAL ;
	}
	
	if(qdmaIsRxRateLimitByPacket(base)) {
		unitBase = 1000000 ;
	} else {
		unitBase = 8000 ;
	}
	
	curTicksel = qdmaGetRxRateLimitTicksel(base);
	if(curTicksel == 0) 
		curUnit = 0 ;
	else
		curUnit = unitBase / curTicksel ;
	
	ret = qdmaGetRxRateLimitConfig(rxRateLimitPtr->ringIdx, QDMA_Rx_Rate_Limit_PIR) ;
	if(ret < 0)
		return ret ;
	rxRateLimitPtr->rateLimitValue = ret * curUnit ;
	
	ret = qdmaGetRxRateLimitConfig(rxRateLimitPtr->ringIdx, QDMA_Rx_Rate_Limit_PBS) ;
	if(ret < 0)
		return ret ;
	rxRateLimitPtr->pbsParamValue = ret * (1<<qdmaGetRxRateLimitBucketScale(base));
		
	return 0 ;
}


/******************************************************************************
 Descriptor:	It's used to enable/disable tx queue DEI dropped function. 
 Input Args:	arg1: Enable/disable tx queue DEI function (QDMA_ENABLE/QDMA_DISABLE)
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_txq_dei_drop_mode(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_Mode_t deiDropMode = qdma_data->qdma_private.mode ;

	if(deiDropMode == QDMA_ENABLE) {
		qdmaEnableTxqCngstDeiDrop(base) ;
	} else {
		qdmaDisableTxqCngstDeiDrop(base) ;
	}
	
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get tx queue DEI dropped mode. 
 Input Args:	No input arguments
 Ret Value:		return tx queue DEI mode (QDMA_ENABLE/QDMA_DISABLE)
******************************************************************************/
int qdma_get_txq_dei_drop_mode(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;

	if(qdmaIsTxqCngstDeiDropEnable(base)) {
		qdma_data->qdma_private.mode = QDMA_ENABLE ;
	} else {
		qdma_data->qdma_private.mode = QDMA_DISABLE ;
	}

	return qdma_data->qdma_private.mode ;
}

/******************************************************************************
 Descriptor:	It's used to enable/disable tx queue dynamic cngst mode. 
 Input Args:	arg1: Enable/disable tx queue dynamic cngst (QDMA_ENABLE/QDMA_DISABLE)
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_txq_cngst_mode(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_Mode_t dynCngstEn = qdma_data->qdma_private.mode ;

	if(dynCngstEn == QDMA_ENABLE) {
		qdmaEnableTxqDynCngstEn(base) ;
	} else {
		qdmaDisableTxqDynCngstEn(base) ;
	}

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get tx queue dynamic cngst mode. 
 Input Args:	No input arguments
 Ret Value:		return tx queue dynamic cngst mode.  (QDMA_ENABLE/QDMA_DISABLE)
******************************************************************************/
int qdma_get_txq_cngst_mode(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;

	if(qdmaIsTxqDynCngstEnable(base)) {
		qdma_data->qdma_private.mode = QDMA_ENABLE ;
	} else {
		qdma_data->qdma_private.mode = QDMA_DISABLE ;
	}
	
	return qdma_data->qdma_private.mode ;
}

/******************************************************************************
 Descriptor:	It's used to set tx queue dei threshold scale, scope is (1/2 ~ 1/16) 
 Input Args:	arg1: set tx queue dei threshold scale  (1/2 ~ 1/16)
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_txq_dei_threshold_scale(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint threshold = qdma_data->qdma_private.threshold;
	qdmaSetDynCngstDeiThrhScale(base, threshold);
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get tx queue dei threshold scale, scope is (1/2 ~ 1/16) 
 Input Args:	No input arguments
 Ret Value:	return tx queue dei threshold scale  (1/2 ~ 1/16)
******************************************************************************/
int qdma_get_txq_dei_threshold_scale(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	qdma_data->qdma_private.threshold = qdmaGetDynCngstDeiThrhScale(base) ;
	return qdma_data->qdma_private.threshold ;
}

/* need modify later */
int qdma_set_txq_cngst_auto_config(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxQDynCngstThrh_T txqDynCngstThrh;
	QDMA_txCngstCfg_t *pTxCngstCfg = qdma_data->qdma_private.pTxCngstCfg;

	/********************************************
	* Setting tx dynamic cngst register               *
	*********************************************/
	qdmaDisableCngstMaxThrhTx1En(base);
	qdmaDisableCngstMinThrhTx1En(base);
	qdmaDisableCngstMaxThrhTx0En(base);
	qdmaDisableCngstMinThrhTx0En(base);
	qdmaEnableCngstModeConfigTrig(base);
	qdmaEnableCngstModePacketTrig(base);
	qdmaEnableCngstModeTimeTrig(base);

	qdmaEnableTxRateMeter(base);

	if(CONFIG_HWFWD_DSCP_NUM <= 256) {
		txqDynCngstThrh.dynCngstTotalMinThrh = 48;
		txqDynCngstThrh.dynCngstChnlMinThrh = 2;
		txqDynCngstThrh.dynCngstQueueMinThrh = 2;
	} else if(CONFIG_HWFWD_DSCP_NUM <= 4096){
		txqDynCngstThrh.dynCngstTotalMinThrh = CONFIG_HWFWD_DSCP_NUM/5; //(CONFIG_HWFWD_DSCP_NUM*96+256)>>9;
		txqDynCngstThrh.dynCngstChnlMinThrh = CONFIG_HWFWD_DSCP_NUM/14; //(CONFIG_HWFWD_DSCP_NUM*3+256)>>9;
		txqDynCngstThrh.dynCngstQueueMinThrh = CONFIG_HWFWD_DSCP_NUM/170; //(CONFIG_HWFWD_DSCP_NUM*3+256)>>9;
	} else {
		printk("Error: CONFIG_HWFWD_DSCP_NUM should less than 4096\n");
		return -1;
	}
	txqDynCngstThrh.dynCngstTotalMaxThrh = (physical_size - (physical_size>>4) )>>8;	/* (physical_size*15/16)/256Byte */
	txqDynCngstThrh.dynCngstChnlMaxThrh = txqDynCngstThrh.dynCngstTotalMaxThrh;
	txqDynCngstThrh.dynCngstQueueMaxThrh = txqDynCngstThrh.dynCngstTotalMinThrh;

	if(pTxCngstCfg->txCngstMode == QDMA_TXCNGST_STATIC) {
		qdmaDisableTxqDynCngstEn(base);
		qdmaDisableTxRateMeterPeakRate(base);
		/*  open api for mac setting */
	} else if(pTxCngstCfg->txCngstMode == QDMA_TXCNGST_DYNAMIC_NORMAL) {
		qdmaEnableTxqDynCngstEn(base);
		qdmaDisableTxRateMeterPeakRate(base);
		qdma_data->qdma_private.dynCngstThrhPtr = &txqDynCngstThrh;
		qdma_set_txq_cngst_dynamic_threshold(qdma_data);
	} else if(pTxCngstCfg->txCngstMode == QDMA_TXCNGST_DYNAMIC_PEAKRATE_MARGIN){
		qdmaEnableTxqDynCngstEn(base);
		qdmaEnableTxRateMeterPeakRate(base);
		qdmaSetTxPeekRateMargin(base, pTxCngstCfg->peekRateMargin);
		qdmaSetTxPeekRateDuration(base, pTxCngstCfg->peekRateDuration>>4);
		qdma_data->qdma_private.dynCngstThrhPtr = &txqDynCngstThrh;
		qdma_set_txq_cngst_dynamic_threshold(qdma_data);	/*  notice that peekrate mode should set different threshold */
	}

	return 0;
}

/* need modify later */
int qdma_get_txq_cngst_auto_config(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_txCngstCfg_t *pTxCngstCfg = qdma_data->qdma_private.pTxCngstCfg;
	
	if(qdmaIsTxqDynCngstEnable(base)) {
		if(qdmaIsTxRateMeterPeakRateEnable(base)) {
			pTxCngstCfg->txCngstMode = QDMA_TXCNGST_DYNAMIC_PEAKRATE_MARGIN;
			pTxCngstCfg->peekRateDuration = qdmaGetTxPeekRateDuration(base);
			pTxCngstCfg->peekRateMargin = qdmaGetTxPeekRateMargin(base);
		} else {
			pTxCngstCfg->txCngstMode = QDMA_TXCNGST_DYNAMIC_NORMAL;
		}
	} else {
		pTxCngstCfg->txCngstMode = QDMA_TXCNGST_STATIC;
	}

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to set the value of tx dynamic threshold. It includes 
 			total max threshold, total min Threshold, channel max threshold, 
 			channel min threshold, queue max threshold, queue min threshold.
 Input Args:	arg1: The pointer of the TX dynamic threshold struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_txq_cngst_dynamic_threshold(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxQDynCngstThrh_T *txqCngstThrhPtr = qdma_data->qdma_private.dynCngstThrhPtr;
	
	if(txqCngstThrhPtr->dynCngstTotalMinThrh > txqCngstThrhPtr->dynCngstTotalMaxThrh) {
		QDMA_ERR("Fault: Dynamic Total Min Threshold should less than Dynamic Total Max Threshold.\n") ; 
		return -EINVAL ;
	}
	
	if(txqCngstThrhPtr->dynCngstChnlMinThrh > txqCngstThrhPtr->dynCngstChnlMaxThrh) {
		QDMA_ERR("Fault: Dynamic Channel Min Threshold should less than Dynamic Channel Max Threshold.\n") ; 
		return -EINVAL ;
	}
	
	if(txqCngstThrhPtr->dynCngstQueueMinThrh > txqCngstThrhPtr->dynCngstQueueMaxThrh) {
		QDMA_ERR("Fault: Dynamic Queue Min Threshold should less than Dynamic Queue Max Threshold.\n") ; 
		return -EINVAL ;
	}

	qdmaSetTxqDynTotalMaxThrh(base, txqCngstThrhPtr->dynCngstTotalMaxThrh) ;
	qdmaSetTxqDynTotalMinThrh(base, txqCngstThrhPtr->dynCngstTotalMinThrh) ;
	
	qdmaSetTxqDynChnlMaxThrh(base, txqCngstThrhPtr->dynCngstChnlMaxThrh) ;
	qdmaSetTxqDynChnlMinThrh(base, txqCngstThrhPtr->dynCngstChnlMinThrh) ;
	
	qdmaSetTxqDynQueueMaxThrh(base, txqCngstThrhPtr->dynCngstQueueMaxThrh) ;
	qdmaSetTxqDynQueueMinThrh(base, txqCngstThrhPtr->dynCngstQueueMinThrh) ;

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get the value of tx dynamic threshold. It includes 
 			total max threshold, total min Threshold, channel max threshold, 
 			channel min threshold, queue max threshold, queue min threshold.
 Input Args:	arg1: The pointer of the TX dynamic threshold struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_get_txq_cngst_dynamic_threshold(struct ECNT_QDMA_Data *qdma_data) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxQDynCngstThrh_T *txqCngstThrhPtr = qdma_data->qdma_private.dynCngstThrhPtr;
	
	txqCngstThrhPtr->dynCngstTotalMaxThrh = qdmaGetTxqDynTotalMaxThrh(base) ;
	txqCngstThrhPtr->dynCngstTotalMinThrh = qdmaGetTxqDynTotalMinThrh(base) ;
	
	txqCngstThrhPtr->dynCngstChnlMaxThrh = qdmaGetTxqDynChnlMaxThrh(base) ;
	txqCngstThrhPtr->dynCngstChnlMinThrh = qdmaGetTxqDynChnlMinThrh(base) ;
	
	txqCngstThrhPtr->dynCngstQueueMaxThrh = qdmaGetTxqDynQueueMaxThrh(base) ;
	txqCngstThrhPtr->dynCngstQueueMinThrh = qdmaGetTxqDynQueueMinThrh(base) ;
		
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to set txQ total max/min threshold, unit is kByte.
 Input Args:	arg1: set txQ total max/min threshold,
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_txq_cngst_total_threshold(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxQDynCngstTotalThrh_T *totalThrhPtr = qdma_data->qdma_private.totalThrhPtr;
	
	if(totalThrhPtr->dynCngstTotalMinThrh > totalThrhPtr->dynCngstTotalMaxThrh) {
		QDMA_ERR("Fault: Dynamic Total Min Threshold should less than Dynamic Total Max Threshold.\n") ; 
		return -EINVAL ;
	}
	
	qdmaSetTxqDynTotalMaxThrh(base, totalThrhPtr->dynCngstTotalMaxThrh<<2) ;
	qdmaSetTxqDynTotalMinThrh(base, totalThrhPtr->dynCngstTotalMinThrh<<2) ;
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get txQ total max/min threshold, unit is kByte.
 Input Args:	No input arguments
 Ret Value:	return txQ total max/min threshold,
******************************************************************************/
int qdma_get_txq_cngst_total_threshold(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxQDynCngstTotalThrh_T *totalThrhPtr = qdma_data->qdma_private.totalThrhPtr;
	
	totalThrhPtr->dynCngstTotalMaxThrh = qdmaGetTxqDynTotalMaxThrh(base)>>2 ;
	totalThrhPtr->dynCngstTotalMinThrh = qdmaGetTxqDynTotalMinThrh(base)>>2 ;

	return 0;
}



/******************************************************************************
 Descriptor:	It's used to set txQ channel  max/min threshold, unit is kByte.
 Input Args:	arg1: set txQ channel max/min threshold,
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_txq_cngst_channel_threshold(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxQDynCngstChnlThrh_T *chnlThrhPtr = qdma_data->qdma_private.chnlThrhPtr;
	
	if(chnlThrhPtr->dynCngstChnlMinThrh > chnlThrhPtr->dynCngstChnlMaxThrh) {
		QDMA_ERR("Fault: Dynamic Channel Min Threshold should less than Dynamic Channel Max Threshold.\n") ; 
		return -EINVAL ;
	}	

	qdmaSetTxqDynChnlMaxThrh(base, chnlThrhPtr->dynCngstChnlMaxThrh<<2) ;
	qdmaSetTxqDynChnlMinThrh(base, chnlThrhPtr->dynCngstChnlMinThrh<<2) ;
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get txQ channel  max/min threshold, unit is kByte.
 Input Args:	No input arguments
 Ret Value:	return txQ channel max/min threshold,
******************************************************************************/
int qdma_get_txq_cngst_channel_threshold(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxQDynCngstChnlThrh_T *chnlThrhPtr = qdma_data->qdma_private.chnlThrhPtr;
	
	chnlThrhPtr->dynCngstChnlMaxThrh = qdmaGetTxqDynChnlMaxThrh(base)>>2 ;
	chnlThrhPtr->dynCngstChnlMinThrh = qdmaGetTxqDynChnlMinThrh(base)>>2 ;

	return 0;
}


/******************************************************************************
 Descriptor:	It's used to set txQ queue max/min threshold, unit is kByte.
 Input Args:	arg1: set txQ queue max/min threshold,
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_txq_cngst_queue_threshold(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxQDynCngstQueueThrh_T *queueThrhPtr = qdma_data->qdma_private.queueThrhPtr;
	
	if(queueThrhPtr->dynCngstQueueMinThrh > queueThrhPtr->dynCngstQueueMaxThrh) {
		QDMA_ERR("Fault: Dynamic Queue Min Threshold should less than Dynamic Queue Max Threshold.\n") ; 
		return -EINVAL ;
	}	

	qdmaSetTxqDynQueueMaxThrh(base, queueThrhPtr->dynCngstQueueMaxThrh<<2) ;
	qdmaSetTxqDynQueueMinThrh(base, queueThrhPtr->dynCngstQueueMinThrh<<2) ;
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get txQ queue max/min threshold, unit is kByte.
 Input Args:	No input arguments
 Ret Value:	return txQ queue max/min threshold,
******************************************************************************/
int qdma_get_txq_cngst_queue_threshold(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxQDynCngstQueueThrh_T *queueThrhPtr = qdma_data->qdma_private.queueThrhPtr;
	
	queueThrhPtr->dynCngstQueueMaxThrh = qdmaGetTxqDynQueueMaxThrh(base)>>2 ;
	queueThrhPtr->dynCngstQueueMinThrh = qdmaGetTxqDynQueueMinThrh(base)>>2 ;

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to set txQ queue nonblocking or not.
 Input Args:	arg1: The pointer of the TXQ Cngst Queue Cfg struct.
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_txq_cngst_queue_nonblocking(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxQCngstQueueCfg_T *txqCngstQueueCfgPtr = qdma_data->qdma_private.txqCngstQueueCfgPtr;
	
	if (txqCngstQueueCfgPtr->queueMode == QDMA_QUEUE_NONBLOCKING) {
		qdmaEnableTxqCngstQueueNonblocking(base, txqCngstQueueCfgPtr->queue);
	}
	else {
		qdmaDisableTxqCngstQueueNonblocking(base, txqCngstQueueCfgPtr->queue);
	}

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get txQ queue nonblocking or not.
 Input Args:	No input arguments
 Ret Value:	return txQ queue nonblocking or not.
******************************************************************************/
int qdma_get_txq_cngst_queue_nonblocking(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxQCngstQueueCfg_T *txqCngstQueueCfgPtr = qdma_data->qdma_private.txqCngstQueueCfgPtr;

	if(qdmaIsTxqCngstQueueNonblockingEnable(base, txqCngstQueueCfgPtr->queue)) {
		txqCngstQueueCfgPtr->queueMode = QDMA_QUEUE_NORMAL ;
	} else {
		txqCngstQueueCfgPtr->queueMode = QDMA_QUEUE_NONBLOCKING ;
	}
	
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to set txQ channel nonblocking or not.
 Input Args:	arg1: The pointer of the TXQ Cngst Channel Cfg struct.
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_txq_cngst_channel_nonblocking(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxQCngstChannelCfg_T *txqCngstChannelCfgPtr = qdma_data->qdma_private.txqCngstChannelCfgPtr;
	
	if (txqCngstChannelCfgPtr->channelMode == QDMA_CHANNEL_NONBLOCKING) {
		qdmaEnableTxqCngstChannelNonblocking(base, txqCngstChannelCfgPtr->channel);
	}
	else {
		qdmaDisableTxqCngstChannelNonblocking(base, txqCngstChannelCfgPtr->channel);
	}

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get txQ channel nonblocking or not.
 Input Args:	No input arguments
 Ret Value:	return txQ channel nonblocking or not.
******************************************************************************/
int qdma_get_txq_cngst_channel_nonblocking(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxQCngstChannelCfg_T *txqCngstChannelCfgPtr = qdma_data->qdma_private.txqCngstChannelCfgPtr;

	if(qdmaIsTxqCngstChannelNonblockingEnable(base, txqCngstChannelCfgPtr->channel)) {
		txqCngstChannelCfgPtr->channelMode = QDMA_CHANNEL_NORMAL ;
	} else {
		txqCngstChannelCfgPtr->channelMode = QDMA_CHANNEL_NONBLOCKING ;
	}
	
	return 0;
}


/******************************************************************************
 Descriptor:	It's used to set txQ peekrate parameters.
 Input Args:	arg1: set txQ peekrate parameters, it includes
 			- peekRateEn: peekrate enable or disable.
 			- peekRateMargin: scope is [0~3], means (0, 25%, 50%, 100%)
 			- peekRateDuration: unit is ms
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_txq_peekrate_params(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_PeekRateCfg_t *peekrateCfgPtr = qdma_data->qdma_private.peekrateCfgPtr;
	
	if(peekrateCfgPtr->peekRateEn == QDMA_ENABLE) {
		qdmaEnableTxRateMeterPeakRate(base) ;
	} else {
		qdmaDisableTxRateMeterPeakRate(base) ;
	}
	qdmaSetTxPeekRateMargin(base, peekrateCfgPtr->peekRateMargin) ;
	qdmaSetTxPeekRateDuration(base, peekrateCfgPtr->peekRateDuration>>4) ;
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get txQ peekrate parameters.
 Input Args:	No input arguments
 Ret Value:	return txQ peekrate parameters.
******************************************************************************/
int qdma_get_txq_peekrate_params(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_PeekRateCfg_t *peekrateCfgPtr = qdma_data->qdma_private.peekrateCfgPtr;

	if(qdmaIsTxRateMeterPeakRateEnable(base))
		peekrateCfgPtr->peekRateEn = QDMA_ENABLE ;
	else
		peekrateCfgPtr->peekRateEn = QDMA_DISABLE ;
	
	peekrateCfgPtr->peekRateMargin = qdmaGetTxPeekRateMargin(base) ;
	peekrateCfgPtr->peekRateDuration = qdmaGetTxPeekRateDuration(base)<<4 ;

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to set txQ static queue normal threshold.
 Input Args:	arg1: set txQ static queue normal threshold, it includes
 			- normalThrh[8]: unit is byte, scope is 256Byte~16MByte.
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_txq_cngst_static_queue_normal_threshold(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	int index = 0 ;
	QDMA_TxQStaticNormalCfg_T *normThrhPtr = qdma_data->qdma_private.normThrhPtr;

	for(index=0; index<CONFIG_QDMA_QUEUE; index++) {
		 qdmaSetTxqStaticQueueDeiThrh(base, index, normThrhPtr->normalThrh[index]<<2) ;
	}

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to set txQ static queue dei threshold.
 Input Args:	arg1: set txQ static queue dei threshold, it includes
 			- deiThrh[8]: unit is byte, scope is 256Byte~16MByte.
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_txq_cngst_static_queue_dei_threshold(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	int index = 0 ;
	QDMA_TxQStaticDeiCfg_T *deiThrhPtr = qdma_data->qdma_private.deiThrhPtr;

	for(index=0; index<CONFIG_QDMA_QUEUE; index++) {
		 qdmaSetTxqStaticQueueDeiThrh(base, index, deiThrhPtr->deiThrh[index]<<2) ;
	}

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get txQ dynamic cngst infomation.
 Input Args:	No input arguments
 Ret Value:	return txQ dynamic cngst infomation.
******************************************************************************/
int qdma_get_txq_cngst_dynamic_info(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxQDynCngstTotalThrh_T *totalThrhPtr = qdma_data->qdma_private.dynCfgPtr->totalThrhPtr;
	QDMA_TxQDynCngstChnlThrh_T *chnlThrhPtr = qdma_data->qdma_private.dynCfgPtr->chnlThrhPtr;
	QDMA_TxQDynCngstQueueThrh_T *queueThrhPtr = qdma_data->qdma_private.dynCfgPtr->queueThrhPtr;
	QDMA_PeekRateCfg_t *peekrateCfgPtr = qdma_data->qdma_private.dynCfgPtr->peekrateCfgPtr;
	
	totalThrhPtr->dynCngstTotalMaxThrh = qdmaGetTxqDynTotalMaxThrh(base)>>2 ;
	totalThrhPtr->dynCngstTotalMinThrh = qdmaGetTxqDynTotalMinThrh(base)>>2 ;
	
	chnlThrhPtr->dynCngstChnlMaxThrh = qdmaGetTxqDynChnlMaxThrh(base)>>2 ;
	chnlThrhPtr->dynCngstChnlMinThrh = qdmaGetTxqDynChnlMinThrh(base)>>2 ;
	
	queueThrhPtr->dynCngstQueueMaxThrh = qdmaGetTxqDynQueueMaxThrh(base)>>2 ;
	queueThrhPtr->dynCngstQueueMinThrh = qdmaGetTxqDynQueueMinThrh(base)>>2 ;
	
	if(qdmaIsTxRateMeterPeakRateEnable(base))
		peekrateCfgPtr->peekRateEn = QDMA_ENABLE ;
	else
		peekrateCfgPtr->peekRateEn = QDMA_DISABLE ;
	
	peekrateCfgPtr->peekRateMargin = qdmaGetTxPeekRateMargin(base) ;
	peekrateCfgPtr->peekRateDuration = qdmaGetTxPeekRateDuration(base)<<4 ;
	
	return 0;
}


/******************************************************************************
 Descriptor:	It's used to get txQ static cngst infomation.
 Input Args:	No input arguments
 Ret Value:	return txQ static cngst infomation.
******************************************************************************/
int qdma_get_txq_cngst_static_info(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	int index = 0 ;
	QDMA_TxQDynCngstTotalThrh_T *totalThrhPtr = qdma_data->qdma_private.staticCfgPtr->totalThrhPtr;
	QDMA_TxQStaticDeiCfg_T *deiThrhPtr = qdma_data->qdma_private.staticCfgPtr->deiThrhPtr;
	QDMA_TxQStaticNormalCfg_T *normThrhPtr = qdma_data->qdma_private.staticCfgPtr->normThrhPtr;
	
	totalThrhPtr->dynCngstTotalMaxThrh = qdmaGetTxqDynTotalMaxThrh(base)>>2 ;
	totalThrhPtr->dynCngstTotalMinThrh = qdmaGetTxqDynTotalMinThrh(base)>>2 ;
	
	for(index=0; index<CONFIG_QDMA_QUEUE; index++) {
		deiThrhPtr->deiThrh[index] = qdmaGetTxqStaticQueueDeiThrh(base, index)>>2 ;
	}
	for(index=0; index<CONFIG_QDMA_QUEUE; index++) {
		normThrhPtr->normalThrh[index] = qdmaGetTxqStaticQueueNormalThrh(base, index)>>2 ;
	}
	
	return 0;
}


/******************************************************************************
 Descriptor:	It's used to set the value of TXQ configuration. It includes
 			txqDropEn, txqDeiDropEn, dynCngstEn, MaxThrhTx1En, MinThrhTx1En, 
 			MaxThrhTx0En, MinThrhTx0En, modeConfigTriggerEn, modePacketTriggerEn, 
 			modeTimeTriggerEn, deiThrhScale, dynCngstTicksel
 Input Args:	arg1: The pointer of the  TXQ configuration struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_txq_cngst_cfg(QDMA_TxQCngstCfg_T *txqCngstCfgPtr)
{
	uint base = gpQdmaPriv->csrBaseAddr ;

	if(txqCngstCfgPtr->dynCngstDeiThrhScale<0 || txqCngstCfgPtr->dynCngstDeiThrhScale>=QDMA_DYNCNGST_DEI_THRH_SCALE_ITEMS) {
		QDMA_ERR("Fault: Tx Dynamic Congestion DEI Threshold Scale shoule between 0 and %d\n", QDMA_DYNCNGST_DEI_THRH_SCALE_ITEMS-1) ; 
		return -EINVAL ;
	}

	if(txqCngstCfgPtr->cngstDropEn == QDMA_DISABLE) {
		qdmaDisableTxqCngstDrop(base) ;
	} else {
		qdmaEnableTxqCngstDrop(base) ;
	}	
	if(txqCngstCfgPtr->cngstDeiDropEn == QDMA_DISABLE) {
		qdmaDisableTxqCngstDeiDrop(base) ;
	} else {
		qdmaEnableTxqCngstDeiDrop(base) ;
	}	
	if(txqCngstCfgPtr->dynCngstEn == QDMA_DISABLE) {
		qdmaDisableTxqDynCngstEn(base) ;
	} else {
		qdmaEnableTxqDynCngstEn(base) ;
	}
	
	if(txqCngstCfgPtr->dynCngstMaxThrhTx1En == QDMA_DISABLE) {
		qdmaDisableCngstMaxThrhTx1En(base) ;
	} else {
		qdmaEnableCngstMaxThrhTx1En(base) ;
	}
	if(txqCngstCfgPtr->dynCngstMinThrhTx1En == QDMA_DISABLE) {
		qdmaDisableCngstMinThrhTx1En(base) ;
	} else {
		qdmaEnablengstMinThrhTx1En(base) ;
	}
	if(txqCngstCfgPtr->dynCngstMaxThrhTx0En == QDMA_DISABLE) {
		qdmaDisableCngstMaxThrhTx0En(base) ;
	} else {
		qdmaEnableCngstMaxThrhTx0En(base) ;
	}
	if(txqCngstCfgPtr->dynCngstMinThrhTx0En == QDMA_DISABLE) {
		qdmaDisableCngstMinThrhTx0En(base) ;
	} else {
		qdmaEnableCngstMinThrhTx0En(base) ;
	}

	if(txqCngstCfgPtr->dynCngstModeConfigTrigEn == QDMA_DISABLE) {
		qdmaDisableCngstModeConfigTrig(base) ;
	} else {
		qdmaEnableCngstModeConfigTrig(base) ;
	}
	if(txqCngstCfgPtr->dynCngstModePacketTrigEn == QDMA_DISABLE) {
		qdmaDisableCngstModePacketTrig(base) ;
	} else {
		qdmaEnableCngstModePacketTrig(base) ;
	}
	if(txqCngstCfgPtr->dynCngstModeTimeTrigEn == QDMA_DISABLE) {
		qdmaDisableCngstModeTimeTrig(base) ;
	} else {
		qdmaEnableCngstModeTimeTrig(base) ;
	}
	
	qdmaSetDynCngstDeiThrhScale(base, txqCngstCfgPtr->dynCngstDeiThrhScale) ;
	qdmaSetTxqDynCngstTicksel(base, txqCngstCfgPtr->dynCngstTicksel) ;
	
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to get the value of TXQ configuration. It includes
 			txqDropEn, txqDeiDropEn, dynCngstEn, MaxThrhTx1En, MinThrhTx1En, 
 			MaxThrhTx0En, MinThrhTx0En, modeConfigTriggerEn, modePacketTriggerEn, 
 			modeTimeTriggerEn, deiThrhScale, dynCngstTicksel
 Input Args:	arg1: The pointer of the  TXQ configuration struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_get_txq_cngst_cfg(QDMA_TxQCngstCfg_T *txqCngstCfgPtr)
{
	uint base = gpQdmaPriv->csrBaseAddr ;

	if(qdmaIsTxqCngstDropEnable(base)) {
		txqCngstCfgPtr->cngstDropEn = QDMA_ENABLE ;
	} else {
		txqCngstCfgPtr->cngstDropEn = QDMA_DISABLE ;
	}
	if(qdmaIsTxqCngstDeiDropEnable(base)) {
		txqCngstCfgPtr->cngstDeiDropEn = QDMA_ENABLE ;
	} else {
		txqCngstCfgPtr->cngstDeiDropEn = QDMA_DISABLE ;
	}
	if(qdmaIsTxqDynCngstEnable(base)) {
		txqCngstCfgPtr->dynCngstEn = QDMA_ENABLE ;
	} else {
		txqCngstCfgPtr->dynCngstEn = QDMA_DISABLE ;
	}
	
	if(qdmaIsCngstMaxThrhTx1Enable(base)) {
		txqCngstCfgPtr->dynCngstMaxThrhTx1En = QDMA_ENABLE ;
	} else {
		txqCngstCfgPtr->dynCngstMaxThrhTx1En = QDMA_DISABLE ;
	}
	if(qdmaIsCngstMinThrhTx1Enable(base)) {
		txqCngstCfgPtr->dynCngstMinThrhTx1En = QDMA_ENABLE ;
	} else {
		txqCngstCfgPtr->dynCngstMinThrhTx1En = QDMA_DISABLE ;
	}
	if(qdmaIsCngstMaxThrhTx0Enable(base)) {
		txqCngstCfgPtr->dynCngstMaxThrhTx0En = QDMA_ENABLE ;
	} else {
		txqCngstCfgPtr->dynCngstMaxThrhTx0En = QDMA_DISABLE ;
	}
	if(qdmaIsCngstMinThrhTx0Enable(base)) {
		txqCngstCfgPtr->dynCngstMinThrhTx0En = QDMA_ENABLE ;
	} else {
		txqCngstCfgPtr->dynCngstMinThrhTx0En = QDMA_DISABLE ;
	}

	if(qdmaIsCngstModeConfigTrigEnable(base)) {
		txqCngstCfgPtr->dynCngstModeConfigTrigEn = QDMA_ENABLE ;
	} else {
		txqCngstCfgPtr->dynCngstModeConfigTrigEn = QDMA_DISABLE ;
	}
	if(qdmaIsCngstModePacketgTrigEnable(base)) {
		txqCngstCfgPtr->dynCngstModePacketTrigEn = QDMA_ENABLE ;
	} else {
		txqCngstCfgPtr->dynCngstModePacketTrigEn = QDMA_DISABLE ;
	}
	if(qdmaIsCngstModeTimeTrigEnable(base)) {
		txqCngstCfgPtr->dynCngstModeTimeTrigEn = QDMA_ENABLE ;
	} else {
		txqCngstCfgPtr->dynCngstModeTimeTrigEn = QDMA_DISABLE ;
	}
	
	txqCngstCfgPtr->dynCngstDeiThrhScale = qdmaGetDynCngstDeiThrhScale(base) ;
	txqCngstCfgPtr->dynCngstTicksel = qdmaGetTxqDynCngstTicksel(base) ;
	
	return 0;
}


#if 0
/******************************************************************************
 Descriptor:	It's used to set the value of tx static threshold. It includes
 			dei packet threshold and Normal packet threshold.
 Input Args:	arg1: The pointer of the TX static threshold struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_set_txq_static_queue_threshold(QDMA_TxQStaticCngstThrh_T *txqCngstThrhPtr)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	
	if(txqCngstThrhPtr->queueIdx<0 || txqCngstThrhPtr->queueIdx>=CONFIG_QDMA_QUEUE) {
		QDMA_ERR("Fault: queue index shoule between 0 and %d\n", CONFIG_QDMA_QUEUE-1) ; 
		return -EINVAL ;
	}
	
	if(txqCngstThrhPtr->staticDeiThrh > txqCngstThrhPtr->staticNormalThrh) {
		QDMA_ERR("Fault: static DEI packet threshold should less than static Normal packet threshold.\n") ; 
		return -EINVAL ;
	}
	
	qdmaSetTxqStaticQueueDeiThrh(base, txqCngstThrhPtr->queueIdx, txqCngstThrhPtr->staticDeiThrh) ;
	qdmaSetTxqStaticQueueNormalThrh(base, txqCngstThrhPtr->queueIdx, txqCngstThrhPtr->staticNormalThrh) ;
	
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get the value of tx static threshold. It includes
 			dei packet threshold and Normal packet threshold.
 Input Args:	arg1: The pointer of the TX static threshold struct.
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_get_txq_static_queue_threshold(QDMA_TxQStaticCngstThrh_T *txqCngstThrhPtr) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	
	if(txqCngstThrhPtr->queueIdx<0 || txqCngstThrhPtr->queueIdx>=CONFIG_QDMA_QUEUE) {
		QDMA_ERR("Fault: queue index shoule between 0 and %d\n", CONFIG_QDMA_QUEUE-1) ; 
		return -EINVAL ;
	}
	
	txqCngstThrhPtr->staticDeiThrh = qdmaGetTxqStaticQueueDeiThrh(base, txqCngstThrhPtr->queueIdx) ;
	txqCngstThrhPtr->staticNormalThrh = qdmaGetTxqStaticQueueNormalThrh(base, txqCngstThrhPtr->queueIdx) ;
	
	return 0 ;
}
#endif


int qdma_set_tx_cngst_mode(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxQDynCngstThrh_T txqDynCngstThrh;
	QDMA_txCngstCfg_t *pTxCngstCfg = qdma_data->qdma_private.pTxCngstCfg;

	/********************************************
	* Setting tx dynamic cngst register               *
	*********************************************/
	qdmaDisableCngstMaxThrhTx1En(base);
	qdmaDisableCngstMinThrhTx1En(base);
	qdmaDisableCngstMaxThrhTx0En(base);
	qdmaDisableCngstMinThrhTx0En(base);
	qdmaEnableCngstModeConfigTrig(base);
	qdmaEnableCngstModePacketTrig(base);
	qdmaEnableCngstModeTimeTrig(base);

	qdmaEnableTxRateMeter(base);

	if(CONFIG_HWFWD_DSCP_NUM <= 256) {
		txqDynCngstThrh.dynCngstTotalMinThrh = 48;
		txqDynCngstThrh.dynCngstChnlMinThrh = 2;
		txqDynCngstThrh.dynCngstQueueMinThrh = 2;
	} else if(CONFIG_HWFWD_DSCP_NUM <= 4096){
		txqDynCngstThrh.dynCngstTotalMinThrh = CONFIG_HWFWD_DSCP_NUM/5; //(CONFIG_HWFWD_DSCP_NUM*96+256)>>9;
		txqDynCngstThrh.dynCngstChnlMinThrh = CONFIG_HWFWD_DSCP_NUM/14; //(CONFIG_HWFWD_DSCP_NUM*3+256)>>9;
		txqDynCngstThrh.dynCngstQueueMinThrh = CONFIG_HWFWD_DSCP_NUM/170; //(CONFIG_HWFWD_DSCP_NUM*3+256)>>9;
	} else {
		printk("Error: CONFIG_HWFWD_DSCP_NUM should less than 4096\n");
		return -1;
	}
	txqDynCngstThrh.dynCngstTotalMaxThrh = (physical_size - (physical_size>>4) )>>8;	/* (physical_size*15/16)/256Byte */
	txqDynCngstThrh.dynCngstChnlMaxThrh = txqDynCngstThrh.dynCngstTotalMaxThrh;
	txqDynCngstThrh.dynCngstQueueMaxThrh = txqDynCngstThrh.dynCngstTotalMinThrh;

	if(pTxCngstCfg->txCngstMode == QDMA_TXCNGST_STATIC) {
		qdmaDisableTxqDynCngstEn(base);
		qdmaDisableTxRateMeterPeakRate(base);
		/*  open api for mac setting */
	} else if(pTxCngstCfg->txCngstMode == QDMA_TXCNGST_DYNAMIC_NORMAL) {
		qdmaEnableTxqDynCngstEn(base);
		qdmaDisableTxRateMeterPeakRate(base);
		qdma_data->qdma_private.dynCngstThrhPtr = &txqDynCngstThrh;
		qdma_set_txq_cngst_dynamic_threshold(qdma_data);
	} else if(pTxCngstCfg->txCngstMode == QDMA_TXCNGST_DYNAMIC_PEAKRATE_MARGIN){
		qdmaEnableTxqDynCngstEn(base);
		qdmaEnableTxRateMeterPeakRate(base);
		qdmaSetTxPeekRateDuration(base, pTxCngstCfg->peekRateDuration>>4);
		qdmaSetTxPeekRateMargin(base, pTxCngstCfg->peekRateMargin);
		qdma_data->qdma_private.dynCngstThrhPtr = &txqDynCngstThrh;
		qdma_set_txq_cngst_dynamic_threshold(qdma_data);	/*  notice that peekrate mode should set different threshold */
	}

	return 0;
}

int qdma_get_tx_cngst_mode(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_txCngstCfg_t *pTxCngstCfg = qdma_data->qdma_private.pTxCngstCfg;
	
	if(qdmaIsTxqDynCngstEnable(base)) {
		if(qdmaIsTxRateMeterPeakRateEnable(base)) {
			pTxCngstCfg->txCngstMode = QDMA_TXCNGST_DYNAMIC_PEAKRATE_MARGIN;
			pTxCngstCfg->peekRateDuration = qdmaGetTxPeekRateDuration(base);
			pTxCngstCfg->peekRateMargin = qdmaGetTxPeekRateMargin(base);
		} else {
			pTxCngstCfg->txCngstMode = QDMA_TXCNGST_DYNAMIC_NORMAL;
		}
	} else {
		pTxCngstCfg->txCngstMode = QDMA_TXCNGST_STATIC;
	}

	return 0;
}



/******************************************************************************
 Descriptor:	It's used to set virtual channel mode.
 Input Args:	arg1: set virtual channel mode, it includes
 			- virChnlEn: virtual channel mapping mode enable or disable.
 			- virChnlMode: 16 Queue or 32 Queue.
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_virtual_channel_mode(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_VirtualChannelMode_T *virChnlModePtr = qdma_data->qdma_private.virChnlModePtr;

	if(virChnlModePtr->virChnlEn == QDMA_ENABLE) {
		qdmaEnableVChnlMapEn(base) ;
	} else {
		qdmaDisableVChnlMapEn(base) ;
	}

	if(virChnlModePtr->virChnlMode == QDMA_VIRTUAL_CHANNEL_32Queue) {
		qdmaSetVChnlMap32Queue(base) ;
	} else {
		qdmaSetVChnlMap16Queue(base) ;
	}
	
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to get virtual channel mode.
 Input Args:	No input arguments
 Ret Value:	return virtual channel mode.
******************************************************************************/
int qdma_get_virtual_channel_mode(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_VirtualChannelMode_T *virChnlModePtr = qdma_data->qdma_private.virChnlModePtr;

	if(qdmaIsVChnlMapEnable(base)) {
		virChnlModePtr->virChnlEn = QDMA_ENABLE ;
	} else { 
		virChnlModePtr->virChnlEn = QDMA_DISABLE ;
	}

	if(qdmaIsVChnlMap32Queue(base)) {
		virChnlModePtr->virChnlMode = QDMA_VIRTUAL_CHANNEL_32Queue ;
	} else { 
		virChnlModePtr->virChnlMode = QDMA_VIRTUAL_CHANNEL_16Queue ;
	}

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to set single physical channel qos value.
 Input Args:	arg1: set single physical channel qos value, it includes
 			- phyChnl: specific physical channel index, scope is (0~7) or (0~15).
 			- qosType: means SP / WRR / SPWRR3 / SPWRR2.
 			- queue[4]: means weight of 2 or 4 queue.
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_set_virtual_channel_qos(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	unchar weight[CONFIG_QDMA_QUEUE] ;
	int virChnl = 0, phyChnlMax=0, virChnlMax=0;
	QDMA_VirtualChannelQoS_T *virChnlQoSPtr = qdma_data->qdma_private.virChnlQoSPtr;

	if(qdmaIsVChnlMapEnable(base) == QDMA_DISABLE) {
		QDMA_ERR("virtual channel mapping mode is disable now.\n") ;
		return -EINVAL;
	}
	
	/* get 16queue or 32queue */
	if(qdmaIsVChnlMap32Queue(base))
		virChnlMax = 4 ;
	else 
		virChnlMax = 2 ;
	phyChnlMax = CONFIG_QDMA_CHANNEL / virChnlMax ;

	if((virChnlQoSPtr->phyChnl >= phyChnlMax) || (virChnlQoSPtr->qosType >= virChnlMax)) {		
		QDMA_ERR("physical channel:%d, should less than %d; mode:%d, should less than %d.\n", virChnlQoSPtr->phyChnl, phyChnlMax, virChnlQoSPtr->qosType, virChnlMax) ;
		return -EINVAL;
	}
	
	for(virChnl=0 ; virChnl<virChnlMax ; virChnl++) {
		if((virChnlQoSPtr->queue[virChnl].weight<0 || virChnlQoSPtr->queue[virChnl].weight>100) && virChnlQoSPtr->queue[virChnl].weight!=255) {
			return -EINVAL ;
		}	
		weight[virChnl] = virChnlQoSPtr->queue[virChnl].weight ;
	}
	
	return qdmaSetVirtualChannelQos(virChnlQoSPtr->phyChnl, virChnlMax, (unchar)virChnlQoSPtr->qosType, weight) ;
}

/******************************************************************************
 Descriptor:	It's used to get single physical channel qos value.
 Input Args:	No input arguments
 Ret Value:	return single physical channel qos value.
******************************************************************************/
int qdma_get_virtual_channel_qos(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_VirtualChannelQoS_T *virChnlQoSPtr = qdma_data->qdma_private.virChnlQoSPtr ;
	int ret = 0, virChnl = 0, phyChnlMax = 0, virChnlMax = 0 ;
	unchar qosType = 0, weight[CONFIG_QDMA_QUEUE] ;	

	if(qdmaIsVChnlMapEnable(base) == QDMA_DISABLE) {
		QDMA_ERR("virtual channel mapping mode is disable now.\n") ;
		return -EINVAL;
	}
	
	/* get 16queue or 32queue */
	if(qdmaIsVChnlMap32Queue(base))
		virChnlMax = 4 ;
	else 
		virChnlMax = 2 ;
	phyChnlMax = CONFIG_QDMA_CHANNEL / virChnlMax ;
	
	if(virChnlQoSPtr->phyChnl >= phyChnlMax) {		
		QDMA_ERR("physical channel:%d, should less than %d.\n", virChnlQoSPtr->phyChnl, phyChnlMax) ;
		return -EINVAL;
	}

	ret = qdmaGetVirtualChannelQos(virChnlQoSPtr->phyChnl, virChnlMax, &qosType, weight) ;
	if(ret < 0) {
		return -EFAULT ;
	}
	
	virChnlQoSPtr->qosType = qosType ;
	
	for(virChnl=0 ; virChnl<virChnlMax ; virChnl++) {
		virChnlQoSPtr->queue[virChnl].weight = weight[virChnl] ;
	}

	return 0 ;
}


int qdma_clear_dbg_cntr_value_all(struct ECNT_QDMA_Data *qdma_data)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	int index = 0 ;
	
	for(index=0; index<DBG_CNTR_INDEX_MAX; index++) {
		qdmaClearCntrCounter(base, index);
	}
	
	return 0;
}

int qdma_clear_and_set_dbg_cntr_channel_group(struct ECNT_QDMA_Data *qdma_data)
{
	int index = 0, offset = 0 ;
	QDMA_DbgCntrChnlGroup_t cntrSrc = qdma_data->qdma_private.cntrSrc;
    QDMA_DBG_CNTR_T dbgCntr;
    dbgCntr.cntrEn = DBG_CNTR_ENABLE;
    dbgCntr.isChnlAll = 0;
    dbgCntr.isDscpRingAll = 1;
    dbgCntr.isQueueAll = 1;
    dbgCntr.queueIdx = 0 ;
    dbgCntr.dscpRingIdx = 0;

	if(cntrSrc >= QDMA_DBG_CNTR_QUEUE){
		QDMA_ERR("channel config type is error, should between 0 and 2\n") ;
		return -EINVAL;
	}
#ifdef QDMA_LAN
	if(CONFIG_QDMA_CHANNEL > 16) {
		QDMA_ERR("CONFIG_QDMA_CHANNEL is larger than 16, must modify this function!\n") ;
		return -EINVAL;
	}
	
	for(index=0; index<CONFIG_QDMA_CHANNEL; index++) {		
        dbgCntr.chnlIdx = index ;
		
        dbgCntr.cntrIdx= index;
        dbgCntr.cntrSrc = DBG_CNTR_SRC_CPU_TX;
        qdma_clear_and_set_dbg_cntr_info(&dbgCntr);
        dbgCntr.cntrIdx= index+CONFIG_QDMA_CHANNEL;
        dbgCntr.cntrSrc = DBG_CNTR_SRC_FWD_TX;
        qdma_clear_and_set_dbg_cntr_info(&dbgCntr);
	}
#else
	if(cntrSrc == QDMA_DBG_CNTR_CHNL_TXCPU) {	
		for(index=0; index<CONFIG_QDMA_CHANNEL; index++) {			
	        dbgCntr.chnlIdx = index ;
	        dbgCntr.cntrIdx = index;
	        dbgCntr.cntrSrc = DBG_CNTR_SRC_CPU_TX;
	        qdma_clear_and_set_dbg_cntr_info(&dbgCntr);
		}
	} else if(cntrSrc == QDMA_DBG_CNTR_CHNL_TXFWD1) {
		offset = 0 ;
	} else if(cntrSrc == QDMA_DBG_CNTR_CHNL_TXFWD2) {
		offset = DBG_CNTR_FWD_CHNL_GROUP_NUM;
	}

	if((cntrSrc == QDMA_DBG_CNTR_CHNL_TXFWD1) || (cntrSrc == QDMA_DBG_CNTR_CHNL_TXFWD2)) {
		for(index=0; index<DBG_CNTR_FWD_CHNL_GROUP_NUM; index++) {			
	        dbgCntr.chnlIdx = index+offset ;
			
	        dbgCntr.cntrIdx = index;
	        dbgCntr.cntrSrc = DBG_CNTR_SRC_CPU_TX;
	        qdma_clear_and_set_dbg_cntr_info(&dbgCntr);
	        dbgCntr.cntrIdx = index+DBG_CNTR_FWD_CHNL_GROUP_NUM;
	        dbgCntr.cntrSrc = DBG_CNTR_SRC_FWD_TX;
	        qdma_clear_and_set_dbg_cntr_info(&dbgCntr);
		}
	}
#endif
	gpQdmaPriv->dbgCntrType = cntrSrc ;

	return 0 ;
}

int qdma_clear_and_set_dbg_cntr_queue_group(struct ECNT_QDMA_Data *qdma_data)
{
	int index = 0 ;
    QDMA_DBG_CNTR_T dbgCntr;
    dbgCntr.cntrEn = DBG_CNTR_ENABLE;
    dbgCntr.isChnlAll = 1;
    dbgCntr.isDscpRingAll = 1;
    dbgCntr.isQueueAll = 0;
    dbgCntr.chnlIdx = 0 ;
    dbgCntr.dscpRingIdx = 0;
	
	if(CONFIG_QDMA_QUEUE > 16) {
		QDMA_ERR("CONFIG_QDMA_QUEUE is larger than 16, must modify this function!\n") ;
		return -EINVAL;
	}
	
	for(index=0; index<CONFIG_QDMA_QUEUE; index++) {		
        dbgCntr.queueIdx = index ;
		
        dbgCntr.cntrIdx= index;
        dbgCntr.cntrSrc = DBG_CNTR_SRC_CPU_TX;
        qdma_clear_and_set_dbg_cntr_info(&dbgCntr);
        dbgCntr.cntrIdx= index+CONFIG_QDMA_QUEUE;
        dbgCntr.cntrSrc = DBG_CNTR_SRC_FWD_TX;
        qdma_clear_and_set_dbg_cntr_info(&dbgCntr);
	}
	gpQdmaPriv->dbgCntrType = QDMA_DBG_CNTR_QUEUE ;

	return 0 ;
}

/*  int qdma_dump_dbg_cntr_config(void)
{
	int index = 0 ;
    QDMA_DBG_CNTR_T dbgCntr;
	for(index=0; index<DBG_CNTR_INDEX_MAX; index++) {
		qdma_get_dbg_cntr_info(&dbgCntr);
		printk("index:%d, srcchannel", dbgCntr.cntrIdx, );
	}
	
	return 0;
}  */

int qdma_dump_dbg_cntr_value(struct ECNT_QDMA_Data *qdma_data)
{
	int index = 0, offset = 0 ;
	uint queueTxCpuVal[CONFIG_QDMA_QUEUE];
	uint chnlTxCpuVal[CONFIG_QDMA_CHANNEL];
	uint defCfgVal[DBG_CNTR_DEFAULT_CONFIG_NUM];
    QDMA_DBG_CNTR_T dbgCntr;

	if(gpQdmaPriv->dbgCntrType >= QDMA_DBG_CNTR_MAX){
		QDMA_ERR("gpQdmaPriv->dbgCntrType is %d, should between 0 and 3\n", gpQdmaPriv->dbgCntrType) ;
		return -EINVAL;
	}
	
	if(gpQdmaPriv->dbgCntrType == QDMA_DBG_CNTR_QUEUE) {
		printk("Dump Queue %d~%d TX CPU & FWD Counter:\n", 0, CONFIG_QDMA_QUEUE-1);
		for(index=0; index<CONFIG_QDMA_QUEUE; index++) {
			dbgCntr.cntrIdx = index ;
			qdma_get_dbg_cntr_info(&dbgCntr);
			if((dbgCntr.cntrSrc != QDMA_DBG_CNTR_SRC_CPU_TX) || (dbgCntr.isQueueAll == 1) || (dbgCntr.queueIdx != index)) {
				QDMA_ERR("cntrIdx_%d: cntrSrc is %d, isQueueAll is %d\n", dbgCntr.cntrIdx , dbgCntr.cntrSrc, dbgCntr.isQueueAll) ;
				return -EINVAL;
			}
			queueTxCpuVal[index] = dbgCntr.cntrVal;
		}
		
		for(index=0; index<CONFIG_QDMA_QUEUE; index++) {
			dbgCntr.cntrIdx = index+CONFIG_QDMA_QUEUE;
			qdma_get_dbg_cntr_info(&dbgCntr);
			if((dbgCntr.cntrSrc != QDMA_DBG_CNTR_SRC_FWD_TX) || (dbgCntr.isQueueAll == 1) || (dbgCntr.queueIdx != index)) {
				QDMA_ERR("cntrIdx_%d: cntrSrc is %d, isQueueAll is %d\n", dbgCntr.cntrIdx , dbgCntr.cntrSrc, dbgCntr.isQueueAll) ;
				return -EINVAL;
			}
			printk("queue_%d: tx cpu cnt:%x, tx fwd cnt:%u\n", index, queueTxCpuVal[index], dbgCntr.cntrVal-queueTxCpuVal[index]);
		}
	}
#ifdef QDMA_LAN
	else {
		printk("Dump Channel %d~%d TX CPU & FWD Counter:\n", 0, CONFIG_QDMA_CHANNEL-1);
		for(index=0; index<CONFIG_QDMA_CHANNEL; index++) {
			dbgCntr.cntrIdx = index ;
			qdma_get_dbg_cntr_info(&dbgCntr);
			if((dbgCntr.cntrSrc != QDMA_DBG_CNTR_SRC_CPU_TX) || (dbgCntr.isChnlAll == 1) || (dbgCntr.chnlIdx != index)) {
				QDMA_ERR("cntrIdx_%d: cntrSrc is %d, isChnlAll is %d\n", dbgCntr.cntrIdx , dbgCntr.cntrSrc, dbgCntr.isChnlAll) ;
				return -EINVAL;
			}
			chnlTxCpuVal[index] = dbgCntr.cntrVal;
		}
		
		for(index=0; index<CONFIG_QDMA_CHANNEL; index++) {
			dbgCntr.cntrIdx = index+CONFIG_QDMA_CHANNEL;
			qdma_get_dbg_cntr_info(&dbgCntr);
			if((dbgCntr.cntrSrc != QDMA_DBG_CNTR_SRC_FWD_TX) || (dbgCntr.isChnlAll == 1) || (dbgCntr.chnlIdx != index)) {
				QDMA_ERR("cntrIdx_%d: cntrSrc is %d, isChnlAll is %d\n", dbgCntr.cntrIdx , dbgCntr.cntrSrc, dbgCntr.isChnlAll) ;
				return -EINVAL;
			}
			printk("channel_%d: tx cpu cnt:%x, tx fwd cnt:%u\n", index, chnlTxCpuVal[index], dbgCntr.cntrVal-chnlTxCpuVal[index]);
		}
	}
#else
	else if (gpQdmaPriv->dbgCntrType == QDMA_DBG_CNTR_CHNL_TXCPU) {
		printk("Dump Channel %d~%d TX CPU Counter:\n", 0, CONFIG_QDMA_CHANNEL-1);
		for(index=0; index<CONFIG_QDMA_CHANNEL; index++) {
			dbgCntr.cntrIdx = index ;
			qdma_get_dbg_cntr_info(&dbgCntr);
			if((dbgCntr.cntrSrc != QDMA_DBG_CNTR_SRC_CPU_TX) || (dbgCntr.isChnlAll == 1) || (dbgCntr.chnlIdx != index)) {
				QDMA_ERR("cntrIdx_%d: cntrSrc is %d, isChnlAll is %d\n", dbgCntr.cntrIdx , dbgCntr.cntrSrc, dbgCntr.isChnlAll) ;
				return -EINVAL;
			}
			printk("channel_%d: tx cpu cnt:%u\n", index, dbgCntr.cntrVal);
		}
	} else if (gpQdmaPriv->dbgCntrType == QDMA_DBG_CNTR_CHNL_TXFWD1) {
		offset = 0 ;
	} else if (gpQdmaPriv->dbgCntrType == QDMA_DBG_CNTR_CHNL_TXFWD2) {
		offset = DBG_CNTR_FWD_CHNL_GROUP_NUM ;
	}

	if((gpQdmaPriv->dbgCntrType == QDMA_DBG_CNTR_CHNL_TXFWD1) || (gpQdmaPriv->dbgCntrType == QDMA_DBG_CNTR_CHNL_TXFWD1)) {
		printk("Dump Channel %d~%d TX CPU & FWD Counter:\n", offset, offset+DBG_CNTR_FWD_CHNL_GROUP_NUM-1);
		for(index=0; index<DBG_CNTR_FWD_CHNL_GROUP_NUM; index++) {
			dbgCntr.cntrIdx = index ;
			qdma_get_dbg_cntr_info(&dbgCntr);
			if((dbgCntr.cntrSrc != QDMA_DBG_CNTR_SRC_CPU_TX) || (dbgCntr.isChnlAll == 1) || (dbgCntr.chnlIdx != index+offset)) {
				QDMA_ERR("cntrIdx_%d: cntrSrc is %d, isChnlAll is %d\n", dbgCntr.cntrIdx , dbgCntr.cntrSrc, dbgCntr.isChnlAll) ;
				return -EINVAL;
			}
			chnlTxCpuVal[index] = dbgCntr.cntrVal;
		}
		
		for(index=0; index<DBG_CNTR_FWD_CHNL_GROUP_NUM; index++) {
			dbgCntr.cntrIdx = index+DBG_CNTR_FWD_CHNL_GROUP_NUM;
			qdma_get_dbg_cntr_info(&dbgCntr);
			if((dbgCntr.cntrSrc != QDMA_DBG_CNTR_SRC_FWD_TX) || (dbgCntr.isChnlAll == 1) || (dbgCntr.chnlIdx != index+offset)) {
				QDMA_ERR("cntrIdx_%d: cntrSrc is %d, isChnlAll is %d\n", dbgCntr.cntrIdx , dbgCntr.cntrSrc, dbgCntr.isChnlAll) ;
				return -EINVAL;
			}
			printk("channel_%d: tx cpu cnt:%x, tx fwd cnt:%u\n", index+offset, chnlTxCpuVal[index], dbgCntr.cntrVal-chnlTxCpuVal[index]);
		}
	}
#endif

	for(index=0; index<DBG_CNTR_DEFAULT_CONFIG_NUM; index++) {
		dbgCntr.cntrIdx = index+DBG_CNTR_DEFAULT_CONFIG_OFFSET;
		qdma_get_dbg_cntr_info(&dbgCntr);
		defCfgVal[index] = dbgCntr.cntrVal;
	}
	printk("\nTX count: cpu-path:%u, fwd-path:%u, total:%u\n", defCfgVal[0], defCfgVal[1]-defCfgVal[0], defCfgVal[1]);
	printk("TX cpu-path count: Ring0:%u, Ring1:%u\n", defCfgVal[4], defCfgVal[5]);
	printk("RX count: cpu-path:%u, fwd-path:%u, total:%u\n", defCfgVal[2], defCfgVal[3], defCfgVal[2]+defCfgVal[3]);
	printk("RX cpu-path count: Ring0:%u, Ring1:%u\n", defCfgVal[6], defCfgVal[7]);

	return 0;
}

/******************************************************************************
 Descriptor:	It's used to dump the tx qos type and queue weigth for all channel.
 Input Args:		chnlIdx: specific the channel ID (0~31) and chnl
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_dump_tx_qos(struct ECNT_QDMA_Data *qdma_data)
{
	int ret = 0 ;
	unchar chnlIdx=0, qosType = 0 ;
	unchar weight[CONFIG_QDMA_QUEUE] ;
	QDMA_TxQosWeightType_t weightBase=0;
	QDMA_TxQosWeightScale_t weightScale=0;	
	int channel = qdma_data->qdma_private.channel ;
	
	ret = qdma_get_tx_qos_weight(qdma_data) ;
	weightBase = qdma_data->qdma_private.qdma_tx_qos.weightBase ;
	weightScale = qdma_data->qdma_private.qdma_tx_qos.weightScale ;	
	printk("weightBase is %s, weightScale is %s\n"
		, ((weightBase==QDMA_TXQOS_WEIGHT_BY_PACKET)?("packet mode"):("byte mode"))
		, ((weightScale==QDMA_TXQOS_WEIGHT_SCALE_64B)?("64Byte"):("16Byte")));

	for(chnlIdx=0; chnlIdx<CONFIG_QDMA_CHANNEL; chnlIdx++) {
		if((channel!=CONFIG_QDMA_CHANNEL) && (channel!=chnlIdx))
			continue;
		
		ret = qdmaGetTxQosScheduler(chnlIdx, &qosType, weight) ;
		if((ret < 0) || (qosType>QDMA_TXQOS_TYPE_NUMS)) {
			QDMA_ERR("qdma_dump_tx_qos Fail at channel-%d: qosType is %d\r\n", chnlIdx, qosType);
			return -EFAULT ;
		}
		printk("channel-%d: qosType is %s", chnlIdx, qosTypeStr[qosType]);
		switch(qosType) {
			case QDMA_TXQOS_TYPE_WRR:
				printk(", q7:q6:q5:q4:q3:q2:q1:q0=%d:%d:%d:%d:%d:%d:%d:%d\r\n", weight[7], weight[6], weight[5], weight[4], weight[3], weight[2], weight[1], weight[0]);
				break;
			case QDMA_TXQOS_TYPE_SP:
				printk("\r\n");
				break;
			case QDMA_TXQOS_TYPE_SPWRR7:
				printk(", q6:q5:q4:q3:q2:q1:q0=%d:%d:%d:%d:%d:%d:%d\r\n", weight[6], weight[5], weight[4], weight[3], weight[2], weight[1], weight[0]);
				break;
			case QDMA_TXQOS_TYPE_SPWRR6:
				printk(", q5:q4:q3:q2:q1:q0=%d:%d:%d:%d:%d:%d\r\n", weight[5], weight[4], weight[3], weight[2], weight[1], weight[0]);
				break;
			case QDMA_TXQOS_TYPE_SPWRR5:
				printk(", q4:q3:q2:q1:q0=%d:%d:%d:%d:%d\r\n", weight[4], weight[3], weight[2], weight[1], weight[0]);
				break;
			case QDMA_TXQOS_TYPE_SPWRR4:
				printk(", q3:q2:q1:q0=%d:%d:%d:%d\r\n", weight[3], weight[2], weight[1], weight[0]);
				break;
			case QDMA_TXQOS_TYPE_SPWRR3:
				printk(", q2:q1:q0=%d:%d:%d\r\n", weight[2], weight[1], weight[0]);
				break;
			case QDMA_TXQOS_TYPE_SPWRR2:
				printk(", q1:q0=%d:%d\r\n", weight[1], weight[0]);
				break;
			default:
				QDMA_ERR("\r\nqosType is not correct at channel-%d\r\n", chnlIdx);
				return -EFAULT ;
		}
	}
	
	return 0 ;
}

int qdma_dump_virtual_channel_qos(struct ECNT_QDMA_Data *qdma_data) 
{	
	uint base = gpQdmaPriv->csrBaseAddr ;
	int ret = 0 ;
	unchar qosType = 0 ;
	unchar weight[CONFIG_QDMA_QUEUE] ;	
	int phyChnl=0, phyChnlMax=0, virChnlMax=0;
	int channel = qdma_data->qdma_private.channel ;

	if(qdmaIsVChnlMapEnable(base) == QDMA_DISABLE) {
		printk("virtual channel mapping mode is disable.\n") ;
		return 0;
	}
	
	/* get 16queue or 32queue */
	if(qdmaIsVChnlMap32Queue(base))
		virChnlMax = 4 ;
	else 
		virChnlMax = 2 ;
	phyChnlMax = CONFIG_QDMA_CHANNEL / virChnlMax ;

	if(channel>phyChnlMax) {
		QDMA_ERR("physical channel:%d, should less than %d.\n", channel, phyChnlMax) ;
	}
	
	printk("virtual channel mapping mode is enable, virChnlMode is %s.\n", ((virChnlMax==4)?("32Queue"):("16Queue")));
	
	for(phyChnl=0 ; phyChnl<phyChnlMax ; phyChnl++) {
		if((channel!=CONFIG_QDMA_CHANNEL) && (channel!=phyChnl))
			continue;
		
		ret = qdmaGetVirtualChannelQos(phyChnl, virChnlMax, &qosType, weight) ;
		if((ret < 0) || (qosType > virChnlMax)) {
			QDMA_ERR("qdma_dump_virtual_channel_qos Fail at physical channel-%d: qosType is %d\r\n", phyChnl, qosType);
			return -EFAULT ;
		}
		
		printk("physical channel-%d: qosType:%s", phyChnl, vChnlQosTypeStr[qosType]);
		switch(qosType) {
			case QDMA_VCHNL_TXQOS_TYPE_WRR:
				if(virChnlMax == 4)
					printk(", vChnl3:vChnl2:vChnl1:vChnl0=%d:%d:%d:%d\r\n", weight[3], weight[2], weight[1], weight[0]);
				else
					printk(", vChnl1:vChnl0=%d:%d\r\n", weight[1], weight[0]);
				break;
			case QDMA_VCHNL_TXQOS_TYPE_SP:
				printk("\r\n");
				break;
			case QDMA_VCHNL_TXQOS_TYPE_SPWRR3:
				printk(", vChnl2:vChnl1:vChnl0=%d:%d:%d\r\n", weight[2], weight[1], weight[0]);
				break;
			case QDMA_VCHNL_TXQOS_TYPE_SPWRR2:
				printk(", vChnl1:vChnl0=%d:%d\r\n", weight[1], weight[0]);
				break;
			default:
				QDMA_ERR("\r\nqosType is not correct at physical channel-%d\r\n", phyChnl);
				return -EFAULT ;
		}
	}
	
	return 0;
}

int qdma_dump_tx_ratelimit(struct ECNT_QDMA_Data *qdma_data)
{
	int ret = 0, chnlIdx=0;
	uint base = gpQdmaPriv->csrBaseAddr ;
	int curTicksel=0, curUnit=0, enMode=0;
	uint rateLimitValue=0, pbsParamValue=0, bucketScale=0 ;
	int channel = qdma_data->qdma_private.channel ;
	
	curTicksel = qdmaGetTxRateLimitTicksel(base);
	if(curTicksel == 0) 
		curUnit = 0 ;
	else
		curUnit = 8000 / curTicksel ;

	enMode = qdmaIsTxRateLimitEnable(base) ;
	bucketScale = qdmaGetTxRateLimitBucketScale(base) ;
	printk("TX-RateLimit: mode:%s, tickSel:%dus, unit:%dkbps, bucksetScale:%uByte\r\n", (enMode)?("Enable"):("Disable"), curTicksel, curUnit, 1<<bucketScale);

	if(enMode == QDMA_DISABLE) {
		return 0;
	}
		
	for(chnlIdx=0; chnlIdx<CONFIG_QDMA_CHANNEL; chnlIdx++) {
		if((channel!=CONFIG_QDMA_CHANNEL) && (channel!=chnlIdx))
			continue;
		
		enMode = qdmaIsChnlRateLimitEnable(base, chnlIdx) ;
		ret = qdmaGetTxRateLimitConfig(chnlIdx, QDMA_Tx_Rate_Limit_PIR) ;
		if(ret < 0)
			return ret ;
		rateLimitValue = ret * curUnit ;
		
		ret = qdmaGetTxRateLimitConfig(chnlIdx, QDMA_Tx_Rate_Limit_PBS) ;
		if(ret < 0)
			return ret ;
		pbsParamValue = ret << bucketScale ;
		
		printk("\r\nChannel-%d: mode:%s", chnlIdx, (enMode)?("Enable"):("Disable"));
		if(enMode)
			printk(", ratelimit value:%ukbps, PBS:%uByte", rateLimitValue, pbsParamValue);
	}
	printk("\r\n");
	return 0 ;
}

int qdma_dump_rx_ratelimit(struct ECNT_QDMA_Data *qdma_data)
{
	int ret=0, ringIdx=0, unitBase=0;	/* unitBase means the unit when ticksel is 1us. */
	uint base = gpQdmaPriv->csrBaseAddr ;
	int curTicksel=0, curUnit=0 ;
	QDMA_Mode_t enMode=0 ;
	QDMA_RxPktMode_t pktMode=0 ;
	uint rateLimitValue=0, pbsParamValue=0, bucketScale=0 ;
	int ringid = qdma_data->qdma_private.channel ;
		
	enMode = qdmaIsRxRateLimitEnable(base) ;
	bucketScale = qdmaGetRxRateLimitBucketScale(base) ;
	pktMode = qdmaIsRxRateLimitByPacket(base) ;
	if(pktMode) {
		unitBase = 1000000 ;
	} else {
		unitBase = 8000 ;
	}
	
	curTicksel = qdmaGetRxRateLimitTicksel(base);
	if(curTicksel == 0) 
		curUnit = 0 ;
	else
		curUnit = unitBase / curTicksel ;

	printk("RX-RateLimit: enMode:%s, pktMode:%s, tickSel:%dus, unit:%d%s, bucksetScale:%uByte\r\n", ((enMode)?("Enable"):("Disable"))
		, ((pktMode)?("packet"):("byte")), curTicksel, curUnit, ((pktMode)?("pps"):("kbps")), 1<<bucketScale);
	
	if(enMode == QDMA_DISABLE) {
		return 0;
	}
	
	for(ringIdx=0; ringIdx<RX_RING_NUM; ringIdx++) {
		if((ringid!=RX_RING_NUM) && (ringid!=ringIdx))
			continue;
		
		ret = qdmaGetRxRateLimitConfig(ringIdx, QDMA_Rx_Rate_Limit_PIR) ;
		if(ret < 0)
			return ret ;
		rateLimitValue = ret * curUnit ;
		
		ret = qdmaGetRxRateLimitConfig(ringIdx, QDMA_Rx_Rate_Limit_PBS) ;
		if(ret < 0)
			return ret ;
		pbsParamValue = ret << bucketScale ;
		
		printk("RingIdx-%d: ratelimit value:%u%s, PBS:%uByte\r\n", ringIdx, rateLimitValue, ((pktMode)?("pps"):("kbps")), pbsParamValue);
	}
	return 0 ;
}

int qdma_dump_tx_dba_report(struct ECNT_QDMA_Data *qdma_data)
{
	int ret = 0, chnlIdx=0;
	uint base = gpQdmaPriv->csrBaseAddr ;
	int curTicksel=0, curUnit=0;
	uint pirValue=0, pbsValue=0, cirValue=0, cbsValue=0, bucketScale=0 ;
	int channel = qdma_data->qdma_private.channel ;
	
	curTicksel = qdmaGetTxRateLimitTicksel(base);
	if(curTicksel == 0) 
		curUnit = 0 ;
	else
		curUnit = 8000 / curTicksel ;

	bucketScale = qdmaGetTxRateLimitBucketScale(base) ;
	printk("TX-DBA Report: tickSel:%dus, unit:%dkbps, bucksetScale:%uByte\r\n", curTicksel, curUnit, 1<<bucketScale);
	
	for(chnlIdx=0; chnlIdx<CONFIG_QDMA_CHANNEL; chnlIdx++) {
		if((channel!=CONFIG_QDMA_CHANNEL) && (channel!=chnlIdx))
			continue;
		
		ret = qdmaGetTxRateLimitConfig(chnlIdx, QDMA_Tx_Rate_Limit_PIR) ;
		if(ret < 0)
			return ret ;
		pirValue = ret * curUnit ;
		
		ret = qdmaGetTxRateLimitConfig(chnlIdx, QDMA_Tx_Rate_Limit_PBS) ;
		if(ret < 0)
			return ret ;
		pbsValue = ret << bucketScale ;
		
		ret = qdmaGetTxRateLimitConfig(chnlIdx, QDMA_Tx_Rate_Limit_CIR) ;
		if(ret < 0)
			return ret ;
		cirValue = ret * curUnit ;
		
		ret = qdmaGetTxRateLimitConfig(chnlIdx, QDMA_Tx_Rate_Limit_CBS) ;
		if(ret < 0)
			return ret ;
		cbsValue = ret << bucketScale ;
		
		printk("Channel-%d: CIR:%ukbps, CBS:%uByte, PIR:%ukbps, PBS:%uByte\r\n", chnlIdx, cirValue, cbsValue, pirValue, pbsValue);
	}
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to dump QDMA txq cngst information.
 Input Args:	No input arguments
 Ret Value:	0: successful, otherwise failed.
******************************************************************************/
int qdma_dump_txq_cngst(struct ECNT_QDMA_Data *qdma_data)
{
#if 1
	int i=0;
	uint base = gpQdmaPriv->csrBaseAddr ;
	QDMA_TxQCngstCfg_T txqCngstCfg;
	QDMA_TxQDynCngstTotalThrh_T totalThrh;
	QDMA_TxQDynCngstChnlThrh_T chnlThrh;
	QDMA_TxQDynCngstQueueThrh_T queueThrh;
	QDMA_PeekRateCfg_t peekrateCfg;
	QDMA_TxQStaticDeiCfg_T deiThrh;
	QDMA_TxQStaticNormalCfg_T normThrh;
	
	qdma_get_txq_cngst_cfg(&txqCngstCfg);
	printk("/*************************************** TXQ CNGST BEGIN ************************************/\n");
	printk("txq cngst drop is %s\n", ((txqCngstCfg.cngstDropEn)?("Enable"):("Disable")));
	printk("txq cngst dei drop is %s\n", ((txqCngstCfg.cngstDeiDropEn)?("Enable"):("Disable")));
	printk("when total tx buffer usage greater than total_max_threshold, tx ring1 will be blocked:  %s\n"
		, ((txqCngstCfg.dynCngstMaxThrhTx1En)?("Enable"):("Disable")));
	printk("when total tx buffer usage greater than total_min_threshold, tx ring1 will be blocked:  %s\n"
		, ((txqCngstCfg.dynCngstMinThrhTx1En)?("Enable"):("Disable")));
	printk("when total tx buffer usage greater than total_max_threshold, tx ring0 will be blocked:  %s\n"
		, ((txqCngstCfg.dynCngstMaxThrhTx0En)?("Enable"):("Disable")));
	printk("when total tx buffer usage greater than total_min_threshold, tx ring0 will be blocked:  %s\n"
		, ((txqCngstCfg.dynCngstMinThrhTx0En)?("Enable"):("Disable")));
	
	if(qdmaIsTxqDynCngstEnable(base)) {
		qdma_data->qdma_private.dynCfgPtr->totalThrhPtr = &totalThrh ;
		qdma_data->qdma_private.dynCfgPtr->chnlThrhPtr = &chnlThrh ;
		qdma_data->qdma_private.dynCfgPtr->queueThrhPtr = &queueThrh ;
		qdma_data->qdma_private.dynCfgPtr->peekrateCfgPtr = &peekrateCfg ;
		qdma_get_txq_cngst_dynamic_info(qdma_data);
		printk("\ntxq cngst mode is dynamic\n");
		printk("total max threshold: %ukByte\n", totalThrh.dynCngstTotalMaxThrh);
		printk("total min threshold: %ukByte\n", totalThrh.dynCngstTotalMinThrh);
		printk("channel max threshold: %ukByte\n", chnlThrh.dynCngstChnlMaxThrh);
		printk("channel min threshold: %ukByte\n", chnlThrh.dynCngstChnlMinThrh);
		printk("queue max threshold: %ukByte\n", queueThrh.dynCngstQueueMaxThrh);
		printk("queue min threshold: %ukByte\n", queueThrh.dynCngstQueueMinThrh);
		printk("DEI packet threshold equal 1/%d normal packet\n", 1<<(txqCngstCfg.dynCngstDeiThrhScale+1));
		printk("dynamic congetion threshold will be updateed after WRR weight changed:  %s\n"
			, ((txqCngstCfg.dynCngstModeConfigTrigEn)?("Enable"):("Disable")));
		printk("dynamic congetion threshold will be updateed after packet receive or transmit:  %s\n"
			, ((txqCngstCfg.dynCngstModePacketTrigEn)?("Enable"):("Disable")));
		printk("dynamic congetion threshold will be updateed after ticksel %sus arrived:  %s\n"
			, txqCngstCfg.dynCngstTicksel, ((txqCngstCfg.dynCngstModeTimeTrigEn)?("Enable"):("Disable")));
		if(peekrateCfg.peekRateEn == QDMA_ENABLE) {
			printk("Peek Rate Function is Enable, Duration is %ums, Margin is %d\n"
				, peekrateCfg.peekRateDuration, peekrateCfg.peekRateMargin);
		} else {
			printk("Peek Rate Function is Disable\n");
		}
	} else {
		qdma_data->qdma_private.staticCfgPtr->totalThrhPtr = &totalThrh ;
		qdma_data->qdma_private.staticCfgPtr->deiThrhPtr = &deiThrh;
		qdma_data->qdma_private.staticCfgPtr->normThrhPtr = &normThrh;
		qdma_get_txq_cngst_static_info(qdma_data);
		printk("\ntxq cngst mode is static\n");
		printk("total max threshold: %ukByte\n", totalThrh.dynCngstTotalMaxThrh);
		printk("total min threshold: %ukByte\n", totalThrh.dynCngstTotalMinThrh);
		printk("\tNormal Threshold\tDEI Threshold\n");
		for(i=0; i<CONFIG_QDMA_QUEUE; i++) {
			printk("Q%d\t%ukByte\t\t\t%ukByte\n", i, normThrh.normalThrh[i], deiThrh.deiThrh[i]);
		}
	}
	printk("/**************************************** TXQ CNGST END *************************************/\n");

#endif

	return 0;
}

int qdma_dump_info_all(struct ECNT_QDMA_Data *qdma_data)
{
	int ret=0;
	
	printk("############ dump qdma all information begin ############\r\n");
	printk("\r\n\r\n\r\n************ dump register value ************\r\n\r\n");
	ret |= qdma_dump_register_value(qdma_data);
	printk("\r\n\r\n\r\n************ dump tx/rx descriptor ************\r\n\r\n");
	ret |= qdma_dump_descriptor_info(qdma_data);
	printk("\r\n\r\n\r\n************ dump hwfwd descriptor ************\r\n\r\n");
	ret |= qdma_dump_hwfwd_info(qdma_data);
	printk("\r\n\r\n\r\n************ dump irq information ************\r\n\r\n");
	ret |= qdma_dump_irq_info(qdma_data);
	
	printk("\r\n\r\n\r\n************ dump cpu counters ************\r\n\r\n");
	ret |= qdma_dump_cpu_counters(qdma_data);
	printk("\r\n\r\n\r\n************ dump tx qos ************\r\n\r\n");
	qdma_data->qdma_private.channel = CONFIG_QDMA_CHANNEL;
	ret |= qdma_dump_tx_qos(qdma_data);
	printk("\r\n\r\n\r\n************ dump virtual channel qos ************\r\n\r\n");
	qdma_data->qdma_private.channel = CONFIG_QDMA_CHANNEL;
	ret |= qdma_dump_virtual_channel_qos(qdma_data);
	printk("\r\n\r\n\r\n************ dump tx ratelimit ************\r\n\r\n");
	qdma_data->qdma_private.channel = CONFIG_QDMA_CHANNEL;
	ret |= qdma_dump_tx_ratelimit(qdma_data);
	printk("\r\n\r\n\r\n************ dump rx ratelimit ************\r\n\r\n");
	qdma_data->qdma_private.channel = RX_RING_NUM;
	ret |= qdma_dump_rx_ratelimit(qdma_data);
	printk("\r\n\r\n\r\n************ dump tx dba report ************\r\n\r\n");
	qdma_data->qdma_private.channel = CONFIG_QDMA_CHANNEL;
	ret |= qdma_dump_tx_dba_report(qdma_data);
	printk("\r\n\r\n############ dump qdma all information end ############\r\n");
	
	return ret;
}


#if 0
void qdmaSetDbgCntrCfg_init(void)
{
    QDMA_DBG_CNTR_T dbgCntrPtr;
    int i = 0;
    int  dbgCntsrc[4] = 
            {DBG_CNTR_SRC_CPU_TX, 
            DBG_CNTR_SRC_CPU_RX, 
            DBG_CNTR_SRC_FWD_TX, 
            DBG_CNTR_SRC_FWD_RX};
    
    for(i = 0 ; i < 4 ; i++)
    {
        dbgCntrPtr.cntrIdx= i;
        dbgCntrPtr.cntrEn = DBG_CNTR_ENABLE;
        dbgCntrPtr.cntrSrc = dbgCntsrc[i];
        dbgCntrPtr.isChnlAll = 1;
        dbgCntrPtr.isDscpRingAll= 1;
        dbgCntrPtr.isQueueAll= 1;
        dbgCntrPtr.chnlIdx  = random32() % CONFIG_QDMA_CHANNEL ;
        dbgCntrPtr.queueIdx = random32() % CONFIG_QDMA_QUEUE ;
        dbgCntrPtr.dscpRingIdx = 0;
        qdma_set_dbg_cntr_info(&dbgCntrPtr);
    }
}

void qdmaGetDbgCntrCfg_Val(uint *cpuTxCnt, uint *cpuRxCnt, uint *fwdTxCnt, uint *fwdRxCnt)
{
    uint base = gpQdmaPriv->csrBaseAddr;
    *cpuTxCnt = qdmaGetCntrCounter(base, 0);
    *cpuRxCnt = qdmaGetCntrCounter(base, 1);
    *fwdTxCnt = qdmaGetCntrCounter(base, 2);
    *fwdRxCnt = qdmaGetCntrCounter(base, 3);
}

/******************************************************************************
 Descriptor:	It's used to retire the DSCP for specific channel.
 Input Args:	arg1: channel number (0~15)
 Ret Value:		0: successful, otherwise failed.
                if( -EADV, it is TxDMA bus busy, must use qdma_set_retire to retire all used channel)
******************************************************************************/
int qdma_set_channel_retire(uint channel)
{
	if(channel >= CONFIG_QDMA_CHANNEL) {
		return -EINVAL ;
	}

	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to retire the DSCP for all channel.
 Input Args:	arg1: maxinum  used channel 
 Ret Value:		0: successful, otherwise failed.                
******************************************************************************/
int qdma_set_retire(uint maxChannel)
{
	if(maxChannel > CONFIG_QDMA_CHANNEL) {
		return -EINVAL ;
	}

	return 0 ;
}
#endif

