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
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <asm/tc3162/tc3162.h>

#include "qdma_bmgr.h"
#include "qdma_dev.h"
#include "qdma_dvt.h"
#include <ecnt_hook/ecnt_hook_fe.h>


/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
#define BROADCAST_OFFSET 0x10
#define MULTICAST_OFFSET 0x20
#define UNKNOWN_UNICAST_OFFSET 0x30

/*  the scale limitValue is kbps */
#define RATELIMIT_MAX 0xFFFF
#define UNIT_MAX 64

#define QOS_DMAWRR_USERDEFINE        0x01
#define ATM_VC_MAX				10

#if defined(TCSUPPORT_QDMA_VERIFY) && defined(QDMA_LAN)
#define SWITCH_PROT6_VLAN_CONTROL	 (0xbfb5a610)
#define SWITCH_PORT_STAG 	(1<<5)
#endif


/************************************************************************
*                  M A C R O S
*************************************************************************
*/	
#define atoi(x)  simple_strtoul(x, NULL,10)
#define QDMA_STOP_TIMER(timer)			{ del_timer_sync(&timer) ; }
#define QDMA_START_TIMER(timer)			{ mod_timer(&timer, (jiffies + ((timer.data*HZ)/1000))) ; }


/************************************************************************
*                  D A T A	 T Y P E S
*************************************************************************
*/

/************************************************************************
*                  E X T E R N A L	 D A T A   D E C L A R A T I O N S
*************************************************************************
*/
#ifdef CONFIG_SUPPORT_SELF_TEST
extern uint loopCnt;
extern QDMA_DbgCounters_T counters[CONFIG_QDMA_CHANNEL][CONFIG_QDMA_QUEUE] ;
#endif

#if defined(TCSUPPORT_CT_SWQOS)
extern void (*sw_PKTQOS_CLEAR_STOP) (void);
#endif

extern atomic_t chnlLimit[CONFIG_QDMA_CHANNEL];
extern atomic_t queueLimit[CONFIG_QDMA_CHANNEL][CONFIG_QDMA_QUEUE];

extern uint channel_limit_threshold;
extern uint queue_limit_threshold;

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
/* Warning: same sequence with enum 'QDMA_HookFunction_t' */
qdma_api_op_t
qdma_operation[]=
{
	/* init */
	qdma_init,
	qdma_deinit,
	qdma_tx_dma_mode,
	qdma_rx_dma_mode,
	qdma_loopback_mode,
	qdma_register_callback_function,
	qdma_unregister_callback_function,
	qdma_enable_rxpkt_int,
	qdma_disable_rxpkt_int,
	qdma_receive_packets,
	qdma_transmit_packet,
	qdma_set_tx_qos_weight,
	qdma_get_tx_qos_weight,
	qdma_set_tx_qos,
	qdma_get_tx_qos,
	qdma_set_mac_limit_threshold,
	qdma_get_mac_limit_threshold,
	
	/* other */
	qdma_set_txbuf_threshold,
	qdma_get_txbuf_threshold,
	qdma_set_prefetch_mode,
	qdma_set_pktsize_overhead_en,
	qdma_get_pktsize_overhead_en,
	qdma_set_pktsize_overhead_value,
	qdma_get_pktsize_overhead_value, 
	qdma_set_lmgr_low_threshold,
	qdma_get_lmgr_low_threshold,
	qdma_get_lmgr_status,

	/* test */
	qdma_set_dbg_level,
	qdma_dump_dma_busy,
	qdma_dump_reg_polling,
	qdma_set_force_receive_rx_ring1,
	
	/* tx rate limit */
	qdma_set_tx_drop_en,
	qdma_get_tx_drop_en,
	qdma_set_tx_ratemeter,
	qdma_get_tx_ratemeter,
	qdma_enable_tx_ratelimit,
	qdma_set_tx_ratelimit_cfg,
	qdma_get_tx_ratelimit_cfg,
	qdma_set_tx_ratelimit,
	qdma_get_tx_ratelimit,
	qdma_set_tx_dba_report,
	qdma_get_tx_dba_report,
	
	/* rx rate limit */
	qdma_set_rx_protect_en,
	qdma_get_rx_protect_en,
	qdma_set_rx_low_threshold,
	qdma_get_rx_low_threshold,
	qdma_set_rx_ratelimit_en,
	qdma_set_rx_ratelimit_pkt_mode,
	qdma_get_rx_ratelimit_cfg,
	qdma_set_rx_ratelimit,
	qdma_get_rx_ratelimit,
	
	/* txq cngst */
	qdma_set_txq_dei_drop_mode,
	qdma_get_txq_dei_drop_mode,
	qdma_set_txq_cngst_mode,
	qdma_get_txq_cngst_mode,
	qdma_set_txq_dei_threshold_scale,
	qdma_get_txq_dei_threshold_scale,
	qdma_set_txq_cngst_auto_config,
	qdma_get_txq_cngst_auto_config,
	qdma_set_txq_cngst_dynamic_threshold,
	qdma_get_txq_cngst_dynamic_threshold,
	qdma_set_txq_cngst_total_threshold,
	qdma_get_txq_cngst_total_threshold,
	qdma_set_txq_cngst_channel_threshold,
	qdma_get_txq_cngst_channel_threshold,
	qdma_set_txq_cngst_queue_threshold,
	qdma_get_txq_cngst_queue_threshold,
	qdma_set_txq_peekrate_params,
	qdma_get_txq_peekrate_params,
	qdma_set_txq_cngst_static_queue_normal_threshold,
	qdma_set_txq_cngst_static_queue_dei_threshold,
	qdma_get_txq_cngst_dynamic_info,
	qdma_get_txq_cngst_static_info,
	qdma_set_txq_cngst_queue_nonblocking,
	qdma_get_txq_cngst_queue_nonblocking,
	qdma_set_txq_cngst_channel_nonblocking,
	qdma_get_txq_cngst_channel_nonblocking,
		
	/* virtual channel */
	qdma_set_virtual_channel_mode,
	qdma_get_virtual_channel_mode,
	qdma_set_virtual_channel_qos,
	qdma_get_virtual_channel_qos,

	/* dbg cntr */
	qdma_clear_and_set_dbg_cntr_channel_group,
	qdma_clear_and_set_dbg_cntr_queue_group,
	qdma_clear_dbg_cntr_value_all,
	qdma_dump_dbg_cntr_value,
	
	/* dump */
	qdma_dump_tx_qos,
	qdma_dump_virtual_channel_qos,
	qdma_dump_tx_ratelimit,
	qdma_dump_rx_ratelimit,
	qdma_dump_tx_dba_report,
	qdma_dump_txq_cngst,
	
	qdma_clear_cpu_counters,
	qdma_dump_cpu_counters,
	qdma_dump_register_value,
	qdma_dump_descriptor_info,
	qdma_dump_irq_info,
	qdma_dump_hwfwd_info,
	qdma_dump_info_all,

	qdma_read_vip_info,
};

/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/
QDMA_Private_T *gpQdmaPriv = NULL ;
int storm_ctrl_shrehold = 120;
int hwfwd_payload_max_size=0;
EXPORT_SYMBOL(storm_ctrl_shrehold);

int qdma_fwd_timer = 100; /* 100ms */
EXPORT_SYMBOL(qdma_fwd_timer);

uint physical_size=0;
uint macType=0;

#ifdef TCSUPPORT_QOS
#ifndef QDMA_LAN
int qos_flag = NULLQOS;
#endif
#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_XPON_HAL_API_QOS_CA) || defined(TCSUPPORT_CT_PON)
int gQueueMask = 0;
int isNeedHWQoS = 1;
#endif
#endif

#if defined(QOS_REMARKING) || defined(TCSUPPORT_HW_QOS)
int qos_wrr_info[5] = {0};
int max_prio = 3;
unchar qos_wrr_user = 0x00;
#endif

#if defined(TCSUPPORT_QDMA_VERIFY) && defined(QDMA_LAN)
int lanTestFlag=0;
int isDeiFlag=0;
int wanTestFlag=0;
int highQueueFlag=0;
int forceRing0Flag=0;
int channelIdx=0;
int queueIdx=0;
EXPORT_SYMBOL(wanTestFlag);
EXPORT_SYMBOL(highQueueFlag);
EXPORT_SYMBOL(forceRing0Flag);
EXPORT_SYMBOL(channelIdx);
EXPORT_SYMBOL(queueIdx);
#endif

/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/


/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
int inline qdma_kfree_skb(struct sk_buff *skb, uint msg0)
{
	uint channel=0, queue=0, ret=0 ;
    
    channel = ((txMsgWord0_t)msg0).bits.channel ;
    queue = ((txMsgWord0_t)msg0).bits.queue ;
    atomic_dec(&chnlLimit[channel]);
    atomic_dec(&queueLimit[channel][queue]);
	
	gpQdmaPriv->counters.txPktsFreeCounts++ ;
    if(gpQdmaPriv->devCfg.bmRecyPktCallbackFunction) {
        if(gpQdmaPriv->devCfg.bmRecyPktCallbackFunction(skb, msg0) != 0) 
        {
			ret = -1;
        } 
    } else {
        dev_kfree_skb_any(skb) ;
    }
    return ret;
}

int qdmaRateMeterValueCalCulate(int unit, ushort *timeDivisor, ushort *timeSlice)
{
	long closeUnitVal=0x7FFFFFFF;
	long curTimeDivisor=0, curTimeSlice=0, curCloseUnitVal=0 ;
	
	if(unit<2 || unit>UNIT_MAX) {
		printk("Fault: Rate Meter unit is %d, which should between 2 and %d\n", unit, UNIT_MAX) ;
		return -1;
	}

	for(curTimeDivisor=0; curTimeDivisor<QDMA_TX_RATE_METER_TIME_DIVISOR_ITEMS; curTimeDivisor++) {
		curTimeSlice = ((1<<3)<<4)*1000*(1<<curTimeDivisor)/unit ;
		if(curTimeSlice>0 && curTimeSlice<=RATELIMIT_MAX) {
			curCloseUnitVal = ((1<<3)<<4)*1000*(1<<curTimeDivisor)/curTimeSlice;
			if(abs(curCloseUnitVal)<abs(closeUnitVal)) {
				closeUnitVal = curCloseUnitVal ;
				*timeDivisor = (ushort)curTimeDivisor ;
				*timeSlice = (ushort)curTimeSlice ;
				if(closeUnitVal == 0)
					return 1;
			}
		}
	}
	
	return 0;
}

/******************************************************************************
 Descriptor:	It's used to configure the TX interrupt delay parameters.
 Input Args:	arg1: irq queue threshold (0~irqDepth). When valid irq entry 
 				len > irq queue threshold, the hardware will generate an interrupt. 
 				arg2: irq pending time (0~255). This argument is specified max 
 				pending for the irq queue interrupt. The uint of the pending time 
 				is 20us.
 Ret Value:		0: setting successful otherwise failed.
******************************************************************************/
int qdma_set_tx_delay(unchar txIrqThreshold, ushort txIrqPtime)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	
	if(txIrqThreshold<=0 || txIrqThreshold>gpQdmaPriv->irqDepth || txIrqPtime>0xFFFF) {
		return -EINVAL ;
	}
	
	qdmaSetIrqThreshold(base, txIrqThreshold) ;
	qdmaSetIrqPtime(base, txIrqPtime) ;
	
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to configure the RX interrupt delay parameters.
 Input Args:	arg1: Specified max number of pending interrupts. When the 
 				number of pending interrupts equal or greater than the arg1 or 
 				interrupt pending time reach the limit (arg2*20us), a final
 				RX_DLY_INT is generated.
 				arg2: Specified max pending time for the internal RX_DONE_INT. 
 				When the pending time equal or greater arg2*20us or the number
 				of RX_DONT_INT equal or greater than arg1, an final RX_DLY_INT 
 				is generated.
 Ret Value:		0: setting successful otherwise failed.
******************************************************************************/
int qdma_set_rx_delay(unchar rxMaxInt, unchar rxMaxPtime, int ringIdx)
{
	uint base = gpQdmaPriv->csrBaseAddr ;

	if(rxMaxInt != 0) {
		qdmaSetRxDelayIntCfg(base, ringIdx, (DLY_INT_RXDLY_INT_EN | (rxMaxInt<<DLY_INT_RXMAX_PINT_SHIFT) | (rxMaxPtime<<DLY_INT_RXMAX_PTIME_SHIFT))) ;
	} else {
		qdmaSetRxDelayIntCfg(base, ringIdx, 0) ;
	}

	return 0 ;
}

static inline int qdma_has_free_rxdscp(int ringIdx)
{
	return (gpQdmaPriv->rxEndPtr[ringIdx]->next != gpQdmaPriv->rxStartPtr[ringIdx]) ;
}

int qdma_bm_tx_polling_mode(QDMA_Mode_t txMode, unchar txThreshold)
{
	uint base = gpQdmaPriv->csrBaseAddr ;

	if(txMode == QDMA_ENABLE) {
		gpQdmaPriv->devCfg.txRecycleThreshold = txThreshold ;
		
		qdmaDisableInt(base, (INT_MASK_TX0_DONE|INT_MASK_TX1_DONE|INT_STATUS_IRQ_FULL)) ;
		gpQdmaPriv->devCfg.flags.isTxPolling = QDMA_ENABLE ;
	} else {
		gpQdmaPriv->devCfg.flags.isTxPolling = QDMA_DISABLE ;
		qdmaEnableInt(base, (INT_MASK_TX0_DONE|INT_MASK_TX1_DONE|INT_STATUS_IRQ_FULL)) ;
	}
	
	return 0 ;
}

int qdma_receive_packet_mode(QDMA_RecvMode_t rxMode)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	
	if(rxMode == QDMA_RX_POLLING) {
		qdmaDisableInt(base, (INT_MASK_NO_RX0_CPU_DSCP|INT_MASK_RX0_DONE|INT_MASK_NO_RX1_CPU_DSCP|INT_MASK_RX1_DONE)) ;
		gpQdmaPriv->devCfg.flags.isRxPolling = QDMA_ENABLE ;
		gpQdmaPriv->devCfg.flags.isRxNapi = QDMA_DISABLE ;
	} else {
		gpQdmaPriv->devCfg.flags.isRxPolling = QDMA_DISABLE ;
	
		if(rxMode == QDMA_RX_NAPI) {
			gpQdmaPriv->devCfg.flags.isRxNapi = QDMA_ENABLE ;
		} else {
			gpQdmaPriv->devCfg.flags.isRxNapi = QDMA_DISABLE ;
		}
		qdmaEnableInt(base, (INT_MASK_NO_RX0_CPU_DSCP|INT_MASK_RX0_DONE|INT_MASK_NO_RX1_CPU_DSCP|INT_MASK_RX1_DONE)) ;
	}
	
	return 0 ;
}

int qdma_recycle_packet_mode(QDMA_RecycleMode_t txMode, unchar txThrh)
{
	int ret = 0;
	
	if(txMode == QDMA_TX_POLLING) {
		if(txThrh > 0) {
			ret = qdma_bm_tx_polling_mode(QDMA_ENABLE, txThrh) ;
		} else {
			ret = qdma_bm_tx_polling_mode(QDMA_ENABLE, 32) ;
		}
	} else {
		ret = qdma_bm_tx_polling_mode(QDMA_DISABLE, 0) ;
	}
	
	return ret ;
}

__IMEM struct sk_buff * allocate_skb_buffer(void)
{
	int offset=0 ;
	struct sk_buff * skb=NULL ;
	
	/* allocate the packet buffer */
	skb = skbmgr_dev_alloc_skb2k() ;
	if(skb == NULL) {
		QDMA_MSG(DBG_ERR, "Allocate socket buffer failed.\n") ;
		gpQdmaPriv->counters.rxAllocFailErrs++ ;
		goto finish ;
	}
	
	/* Shift to 4 byte alignment */
	offset = ((uint)(skb->tail) & (SKB_ALIGNMENT-1)) ;
	if(offset) {
		skb_reserve(skb, (SKB_ALIGNMENT - offset)) ;
	}
	
#ifdef CONFIG_RX_2B_OFFSET
	/* Reserve 2 bytes for zero copy */
	skb_reserve(skb, NET_IP_ALIGN) ;
#endif
	
finish:
	return skb;
}

int qdma_prepare_rx_buffer(int ringIdx)
{
	int ret = 0 ;
	struct sk_buff *skb = NULL ;

	skb = allocate_skb_buffer();
	if(skb == NULL) {
		goto err ;
	}
		
	ret = qdma_bm_hook_receive_buffer(skb, ringIdx) ;
	if(ret != 0) {
		ret = -ENODEV ;
		goto err ;
	}
	
	return ret ;
	
err:
	if(skb) 		dev_kfree_skb_any(skb) ;
	return ret ;
}

/******************************************************************************
 Packet Receive
******************************************************************************/
/******************************************************************************
******************************************************************************/
static inline void qdma_bm_add_rx_dscp(struct QDMA_DscpInfo_S *diPtr, int ringIdx) 
{
	if(!gpQdmaPriv->rxStartPtr[ringIdx]) {
		gpQdmaPriv->rxStartPtr[ringIdx] = diPtr ;
		diPtr->next = gpQdmaPriv->rxStartPtr[ringIdx] ;
	} else {
		diPtr->next = gpQdmaPriv->rxStartPtr[ringIdx]->next ;
		gpQdmaPriv->rxStartPtr[ringIdx]->next = diPtr ;
		gpQdmaPriv->rxStartPtr[ringIdx] = diPtr ;
	}
}

/******************************************************************************
******************************************************************************/
static inline struct QDMA_DscpInfo_S *qdma_bm_remove_rx_dscp(int ringIdx)
{
	struct QDMA_DscpInfo_S *diPtr = NULL ;

	if(gpQdmaPriv->rxStartPtr[ringIdx]) {
		diPtr = gpQdmaPriv->rxStartPtr[ringIdx]->next ;
		
		if(gpQdmaPriv->rxStartPtr[ringIdx] == diPtr) {
			gpQdmaPriv->rxStartPtr[ringIdx] = NULL ;
		} else {
			gpQdmaPriv->rxStartPtr[ringIdx]->next = diPtr->next ;
		}
	}

	return diPtr ;
}

/******************************************************************************
******************************************************************************/
static inline struct QDMA_DscpInfo_S *qdma_bm_get_unused_rx_dscp(int ringIdx)
{
	struct QDMA_DscpInfo_S *diPtr = NULL ;
	ulong flags=0 ;
	
	if(gpQdmaPriv->rxStartPtr[ringIdx]) {
		if(!gpQdmaPriv->rxEndPtr[ringIdx]) {
			diPtr = gpQdmaPriv->rxStartPtr[ringIdx] ;
			gpQdmaPriv->rxEndPtr[ringIdx] = diPtr ;
		} else if(gpQdmaPriv->rxEndPtr[ringIdx]->next != gpQdmaPriv->rxStartPtr[ringIdx]) {
			diPtr = gpQdmaPriv->rxEndPtr[ringIdx]->next ;
			gpQdmaPriv->rxEndPtr[ringIdx] = diPtr ; ;
		}
	} 

	return diPtr ;
}

/******************************************************************************
******************************************************************************/
__IMEM int qdma_bm_receive_packets(uint maxPkts, int ringIdx) 
{
	QDMA_DMA_DSCP_T rxDscp ;
	struct QDMA_DscpInfo_S *dscpInfoPtr=NULL ;
	ulong flags=0 ;
	uint cnt = maxPkts ;
	uint pktCount = 0 ;
	int retValue = 0 ;
	struct sk_buff *newSkb = NULL ;
	uint base = gpQdmaPriv->csrBaseAddr ;
    int (*bmRecvCallbackFun)(void *, uint, struct sk_buff *, uint) = NULL ;
#if defined(TCSUPPORT_FTTDP_V2) && defined(QDMA_LAN)
    unsigned int *msgPtr=NULL;
#endif
#if defined(TCSUPPORT_QDMA_VERIFY) && defined(QDMA_LAN)
	txMsgWord0_t txMsgW0;
	txMsgWord1_t txMsgW1;
	int error = 0 ;
#endif
	
	do {
		if(!gpQdmaPriv->rxStartPtr[ringIdx] || 
            gpQdmaPriv->rxStartPtr[ringIdx]== gpQdmaPriv->rxEndPtr[ringIdx] || 
            !gpQdmaPriv->rxStartPtr[ringIdx]->dscpPtr->ctrl.done)
		{
			QDMA_MSG(DBG_WARN, "gpQdmaPriv->rxStartPtr[%d]:%d, gpQdmaPriv->rxEndPtr[%d]:%d, \
                crtl.done:%d, cpuIdx:%d, dmaIdx:%d\n"
				, ringIdx, gpQdmaPriv->rxStartPtr[ringIdx]->dscpIdx
				, ringIdx, gpQdmaPriv->rxEndPtr[ringIdx]->dscpIdx
				, gpQdmaPriv->rxStartPtr[ringIdx]->dscpPtr->ctrl.done
				, qdmaGetRxCpuIdx(base, ringIdx), qdmaGetRxDmaIdx(base, ringIdx)) ;
			return pktCount ;
		}
		memcpy(&rxDscp, gpQdmaPriv->rxStartPtr[ringIdx]->dscpPtr, sizeof(QDMA_DMA_DSCP_T)) ;
		dscpInfoPtr = gpQdmaPriv->rxStartPtr[ringIdx];
		
		gpQdmaPriv->rxStartPtr[ringIdx] = gpQdmaPriv->rxStartPtr[ringIdx]->next ;
		
		pktCount++ ;
		
		/* check DSCP cotent: if pkt_addr is NULL or pkt_len is 0, then some big issue happens, must stop running and check why */
		if(!rxDscp.pkt_addr || !rxDscp.ctrl.pkt_len)
		{
			QDMA_ERR("The content of the RX DSCP is incorrect.\n") ;
			gpQdmaPriv->counters.rxDscpIncorrect[ringIdx]++ ; 
			break ;
		}
		/* overflow is not critical issue, drop this packet and go-head */
		if(rxDscp.ctrl.overflow)
		{
			QDMA_ERR("The pkt size is overflow: ready pkt_len is %d, received pkt_len is %d.\n", skb_tailroom(dscpInfoPtr->skb), rxDscp.ctrl.pkt_len) ;
			gpQdmaPriv->counters.rxDscpIncorrect[ringIdx]++ ;
			newSkb = dscpInfoPtr->skb;
			goto next;
		}
		dma_unmap_single(NULL, rxDscp.pkt_addr, rxDscp.ctrl.pkt_len, DMA_FROM_DEVICE) ;
		
		newSkb = allocate_skb_buffer();
		if(newSkb == NULL) {
			QDMA_ERR("Allocate skb fail, Revert received packet as new skb.\n") ;
			gpQdmaPriv->counters.rxAllocFailDropErrs[ringIdx]++ ;
			newSkb = dscpInfoPtr->skb;
			goto next;
		}
		
#if defined(TCSUPPORT_QDMA_VERIFY) && defined(QDMA_LAN)
		if(wanTestFlag) {
			skb_push(dscpInfoPtr->skb, 2);
			dscpInfoPtr->skb->data[0] = ringIdx;
			skb_pull(dscpInfoPtr->skb, 2);
		}
		if(lanTestFlag) {			
			skb_put(dscpInfoPtr->skb, rxDscp.ctrl.pkt_len);
			
			/**************** prepare Tx Msg *****************/ 
			txMsgW0.word=0;
			txMsgW1.word=0;
			txMsgW0.bits.channel = channelIdx;
			txMsgW0.bits.queue = queueIdx;
			if(isDeiFlag)
				txMsgW0.bits.dei = 1;
			
			/* recycle TX DSCP when send packets in tx polling mode */
			if(gpQdmaPriv->devCfg.flags.isTxPolling == QDMA_ENABLE) {
				if(qdmaGetIrqEntryLen(base) >= gpQdmaPriv->devCfg.txRecycleThreshold) {
					qdma_bm_transmit_done(gpQdmaPriv->devCfg.txRecycleThreshold<<2) ;
				}
			}
			
			error = qdma_bm_transmit_packet(dscpInfoPtr->skb, ringIdx, txMsgW0.word, txMsgW1.word);
			if(error){
				dev_kfree_skb_any(dscpInfoPtr->skb);
				gpQdmaPriv->counters.rxPktErrs[ringIdx]++ ;
				break ;
			}
			gpQdmaPriv->counters.rxCounts[ringIdx]++ ;
			goto next;
		}
#endif

    #if defined(TCSUPPORT_FTTDP_V2) && defined(QDMA_LAN)
    /* In FTTdp, channel 0 is for ethLan and channel 1~4 are for ptmLan */
        msgPtr = (unsigned int*)rxDscp.msg;
        if (((*msgPtr)>>3) & 0xff)
            bmRecvCallbackFun = gpQdmaPriv->devCfg.bmRecvCallbackPtmLanFunction;
        else
            bmRecvCallbackFun = gpQdmaPriv->devCfg.bmRecvCallbackFunction;
    #else
            bmRecvCallbackFun = gpQdmaPriv->devCfg.bmRecvCallbackFunction;
    #endif
    
        if(bmRecvCallbackFun) {
			retValue = bmRecvCallbackFun((void *)rxDscp.msg, QDMA_RX_DSCP_MSG_LENS, dscpInfoPtr->skb, rxDscp.ctrl.pkt_len);
			if(retValue< 0)
			{
				QDMA_MSG(DBG_WARN, "qdma_bm_receive_packets: retValue = %d, EFAULT = %d, line = %d\n", 
                    retValue, EFAULT, __LINE__);
				if ((retValue + EFAULT) == -BROADCAST_OFFSET)
				{
					gpQdmaPriv->counters.rxBroadcastCounts[ringIdx]++;
					QDMA_MSG(DBG_WARN, "qdma_bm_receive_packets:rxBroadcastCounts[%d]=%d, line = %d\n", 
                        ringIdx, gpQdmaPriv->counters.rxBroadcastCounts[ringIdx], __LINE__);
				}
				else if ((retValue + EFAULT) == -MULTICAST_OFFSET)
				{
					gpQdmaPriv->counters.rxMulticastCounts[ringIdx]++;					
					QDMA_MSG(DBG_WARN, "qdma_bm_receive_packets: rxMulticastCounts[%d]=%d, line = %d\n", 
                        ringIdx, gpQdmaPriv->counters.rxMulticastCounts[ringIdx], __LINE__);
				}
				else if ((retValue + EFAULT) == -UNKNOWN_UNICAST_OFFSET)
				{
					gpQdmaPriv->counters.rxUnknownUnicastCounts[ringIdx]++;					
					QDMA_MSG(DBG_WARN, "qdma_bm_receive_packets: rxUnknownUnicastCounts[%d]=%d, line = %d\n", 
                        ringIdx, gpQdmaPriv->counters.rxUnknownUnicastCounts[ringIdx], __LINE__);
				}
				gpQdmaPriv->counters.rxPktErrs[ringIdx]++ ;
			} else {
				QDMA_MSG(DBG_WARN, "qdma_bm_receive_packets: retValue = %d, line = %d\n", retValue, __LINE__);
				if (retValue == BROADCAST_OFFSET)
				{
					gpQdmaPriv->counters.rxBroadcastCounts[ringIdx]++;					
					QDMA_MSG(DBG_WARN, "qdma_bm_receive_packets: rxBroadcastCounts[%d]=%d, line = %d\n", 
                        ringIdx, gpQdmaPriv->counters.rxBroadcastCounts[ringIdx],  __LINE__);
				}
				else if (retValue == MULTICAST_OFFSET)
				{
					gpQdmaPriv->counters.rxMulticastCounts[ringIdx]++;					
					QDMA_MSG(DBG_WARN, "qdma_bm_receive_packets:rxMulticastCounts[%d]=%d, line = %d\n", 
                        ringIdx, gpQdmaPriv->counters.rxMulticastCounts[ringIdx],__LINE__);
				}
				else if (retValue == UNKNOWN_UNICAST_OFFSET)	
				{
					gpQdmaPriv->counters.rxUnknownUnicastCounts[ringIdx]++;					
					QDMA_MSG(DBG_WARN, "qdma_bm_receive_packets: rxUnknownUnicastCounts[%d]=%d, line = %d\n", 
                        ringIdx, gpQdmaPriv->counters.rxUnknownUnicastCounts[ringIdx], __LINE__);
				}
				gpQdmaPriv->counters.rxCounts[ringIdx]++ ;
			} 
		} else {
			QDMA_ERR("\nRX Error: rx callback function is NULL, kfree current skb\n");
			gpQdmaPriv->counters.noRxCbErrs[ringIdx]++ ;
			dev_kfree_skb_any(dscpInfoPtr->skb);
		}
next:
		if (qdma_has_free_rxdscp(ringIdx))
		{
			qdma_bm_hook_receive_buffer(newSkb, ringIdx);
		} else {
			QDMA_ERR("\nRX Error: no available QDMA RX descritor\n");
			gpQdmaPriv->counters.noRxDscps[ringIdx]++ ;
			dev_kfree_skb_any(newSkb);
		}
	} while((!maxPkts) || (--cnt)) ;

	return pktCount ;
}

/******************************************************************************
******************************************************************************/
__IMEM int qdma_bm_hook_receive_buffer(struct sk_buff *skb, int ringIdx)
{
	struct QDMA_DscpInfo_S *pNewDscpInfo=NULL ;
	QDMA_DMA_DSCP_T *pRxDscp=NULL ;
	dma_addr_t dmaPktAddr=0 ;
	uint base = gpQdmaPriv->csrBaseAddr ;
	ulong flags=0 ;
	int i=0, ret=0 ;

	if(!skb)
	{
		QDMA_ERR("The input arguments are wrong, skb:%.8x, skbLen:%d.\n", (uint)skb, skb_tailroom(skb)) ; 
		return -EFAULT ;
	}
	
	pNewDscpInfo = qdma_bm_get_unused_rx_dscp(ringIdx) ;
	if(pNewDscpInfo == NULL) {
		QDMA_ERR("There is not any free RX DSCP.\n") ; 
		gpQdmaPriv->counters.noRxDscps[ringIdx]++ ;
		return -ENOMEM ;
	}
	
#ifdef CONFIG_RX_2B_OFFSET
	QDMA_MSG(DBG_MSG, "Adjust the skb->tail location for net IP alignment\n") ;
	if(((uint)skb->tail & 3) == 0) {
		skb_reserve(skb, NET_IP_ALIGN) ;
	}
	dmaPktAddr = dma_map_single(NULL, (void *)((uint)skb->tail-NET_IP_ALIGN), (size_t)skb_tailroom(skb), DMA_FROM_DEVICE) ;
#else
	dmaPktAddr = dma_map_single(NULL, (void *)((uint)skb->tail), (size_t)skb_tailroom(skb), DMA_FROM_DEVICE) ;
#endif /* CONFIG_RX_2B_OFFSET */
	
	pRxDscp = K1_TO_K0((uint32)gpQdmaPriv->rxUsingPtr[ringIdx]->dscpPtr);
	for(i=0; i<QDMA_DSCP_MSG_WORD_LENS; i++)
		pRxDscp->msg[i] = 0;
	pRxDscp->pkt_addr = dmaPktAddr ;
	pRxDscp->next_idx = pNewDscpInfo->dscpIdx ;
	pRxDscp->ctrl.pkt_len = skb_tailroom(skb) ;
#ifdef CONFIG_SUPPORT_SELF_TEST
	pRxDscp->ctrl.done = gpQdmaPriv->devCfg.rxDscpDoneBit ;
#else
	pRxDscp->ctrl.done = 0 ;
#endif /* CONFIG_SUPPORT_SELF_TEST */

	QDMA_MSG(DBG_MSG, "Hook RX DSCP to RXDMA. RX_CPU_IDX:%.8x, RX_NULL_IDX:%.8x\n", 
	    gpQdmaPriv->rxUsingPtr[ringIdx]->dscpIdx, pNewDscpInfo->dscpIdx) ;
	QDMA_MSG(DBG_MSG, "RXDSCP: DONE:%d, PKT:%.8x, PKTLEN:%d, NEXT_IDX:%d\n", 
													(uint)pRxDscp->ctrl.done, 
													(uint)pRxDscp->pkt_addr,
													(uint)pRxDscp->ctrl.pkt_len,
													(uint)pRxDscp->next_idx) ;
	
	protected_cache_op(Hit_Writeback_Inv_D, ((unsigned long)(pRxDscp)));
	gpQdmaPriv->rxUsingPtr[ringIdx]->skb = skb ;
	gpQdmaPriv->rxUsingPtr[ringIdx] = pNewDscpInfo ;

	wmb() ;
	
	/* Setting DMA Rx Descriptor Register */
	qdmaSetRxCpuIdx(base, ringIdx, pNewDscpInfo->dscpIdx) ;
	
	return ret ;
}

/******************************************************************************
******************************************************************************/
int qdma_bm_recycle_receive_buffer(int ringIdx) 
{
	QDMA_DMA_DSCP_T *pRxDscp=NULL ;
	struct QDMA_DscpInfo_S *diPtr=NULL ;
	uint base = gpQdmaPriv->csrBaseAddr ;
	ulong flags=0 ;

	if(!gpQdmaPriv->rxStartPtr[ringIdx] || !gpQdmaPriv->rxEndPtr[ringIdx]) {
		return -EFAULT ;
	}

	
	while(gpQdmaPriv->rxStartPtr[ringIdx] != gpQdmaPriv->rxEndPtr[ringIdx]) {
		diPtr = gpQdmaPriv->rxStartPtr[ringIdx] ;
		pRxDscp = gpQdmaPriv->rxStartPtr[ringIdx]->dscpPtr ;

		if(diPtr->skb && pRxDscp->pkt_addr) {
			dma_unmap_single(NULL, pRxDscp->pkt_addr, skb_tailroom(diPtr->skb), DMA_FROM_DEVICE) ;	
			if(gpQdmaPriv->devCfg.bmRecvCallbackFunction) {
				gpQdmaPriv->devCfg.bmRecvCallbackFunction((void *)pRxDscp->msg, QDMA_RX_DSCP_MSG_LENS, diPtr->skb, pRxDscp->ctrl.pkt_len) ;
			} else {
				QDMA_MSG(DBG_WARN, "\nrx callback function is NULL, kfree current skb\n");
				gpQdmaPriv->counters.noRxCbErrs[ringIdx]++ ;
				dev_kfree_skb_any(diPtr->skb);
			}
		}
		
		gpQdmaPriv->rxStartPtr[ringIdx] = diPtr->next ;		
	}

	diPtr = gpQdmaPriv->rxStartPtr[ringIdx] ;
	gpQdmaPriv->rxUsingPtr[ringIdx] = diPtr ;
	qdmaSetRxCpuIdx(base, ringIdx, diPtr->dscpIdx) ;
	qdmaSetRxDmaIdx(base, ringIdx, diPtr->dscpIdx) ;
	
	
	return 0 ;
}

/******************************************************************************
 Packet Transmit
******************************************************************************/
/******************************************************************************
******************************************************************************/
static inline int qdma_bm_push_tx_dscp(struct QDMA_DscpInfo_S *diPtr, int ringIdx) 
{
	if(diPtr->next != NULL) {
		QDMA_ERR("The TX DSCP is not return from tx used pool\n") ;
		return -1 ;
	}

	diPtr->skb = NULL ;
	if(!gpQdmaPriv->txHeadPtr[ringIdx]) {
		gpQdmaPriv->txHeadPtr[ringIdx] = diPtr ;
		gpQdmaPriv->txTailPtr[ringIdx] = diPtr ;
	} else {
		gpQdmaPriv->txTailPtr[ringIdx]->next = diPtr ;
		gpQdmaPriv->txTailPtr[ringIdx] = gpQdmaPriv->txTailPtr[ringIdx]->next ;
	}
	
	return 0 ;
}


/******************************************************************************
******************************************************************************/
static inline struct QDMA_DscpInfo_S *qdma_bm_pop_tx_dscp(int ringIdx)
{
	struct QDMA_DscpInfo_S *diPtr=NULL ;
	
	diPtr = gpQdmaPriv->txHeadPtr[ringIdx] ;
	if(gpQdmaPriv->txHeadPtr[ringIdx] == gpQdmaPriv->txTailPtr[ringIdx]) {
		gpQdmaPriv->txHeadPtr[ringIdx] = NULL ;
		gpQdmaPriv->txTailPtr[ringIdx] = NULL ;
	} else {
		gpQdmaPriv->txHeadPtr[ringIdx] = gpQdmaPriv->txHeadPtr[ringIdx]->next ;
	}

	if(diPtr) {
		diPtr->next = NULL ;
	}
	
	return diPtr ;
}

/******************************************************************************
******************************************************************************/
__IMEM int qdma_bm_transmit_done(int amount) 
{
	QDMA_DMA_DSCP_T *txDscpPtr=NULL ;
	struct QDMA_DscpInfo_S *diPtr=NULL ;
	int i=0, j=0, idx=0, ringIdx=0, ret=0;
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint entryLen=0, headIdx=0, irqValue=0;
	uint RETRY=5, irqStatus=0 ; 
	volatile uint *irqPtr=NULL ;
	ulong flags=0 ;
#ifdef CONFIG_SUPPORT_SELF_TEST
	txMsgWord0_t txMsgW0;
	uint channel=0, queue=0 ;
#endif
	struct ECNT_QDMA_Data qdma_data;

	if(gpQdmaPriv->devCfg.flags.isTxPolling == QDMA_ENABLE)
		spin_lock_irqsave(&gpQdmaPriv->txDoneLock, flags) ;
	irqStatus = qdmaGetIrqStatus(base) ;
	headIdx = (irqStatus & IRQ_STATUS_HEAD_IDX_MASK) >> IRQ_STATUS_HEAD_IDX_SHIFT ;
	entryLen = (irqStatus & IRQ_STATUS_ENTRY_LEN_MASK) >> IRQ_STATUS_ENTRY_LEN_SHIFT ;
	if(entryLen == 0) {
		gpQdmaPriv->counters.IrqErr++ ;
		goto out2 ;
	}

	entryLen = (amount && amount<entryLen) ? amount : entryLen ;
	for(i=0 ; i<entryLen ; i++) {
		irqPtr = (uint *)gpQdmaPriv->irqQueueAddr + ((headIdx+i) % CONFIG_IRQ_DEPTH) ;
		
		RETRY = 5 ;
		while(RETRY--) {
			irqValue = *irqPtr ;
			if(irqValue == CONFIG_IRQ_DEF_VALUE) {
				if(RETRY == 0)
					QDMA_ERR("There is no data available in IRQ queue. irq value:%.8x, irq ptr:%.8x TIMEs:%d\n", (uint)irqValue, (uint)irqPtr, RETRY) ;
				if(RETRY <= 0) {
					gpQdmaPriv->counters.IrqQueueAsynchronous++ ;
					ret = -ENODATA ;
					goto out1 ;
				}
			} else {
				*irqPtr = CONFIG_IRQ_DEF_VALUE ;
				break ;
			}
		}
		
		idx = (irqValue & IRQ_CFG_IDX_MASK) ;
		ringIdx = (irqValue & IRQ_CFG_RINGIDX_MASK) >> IRQ_CFG_RINGIDX_SHIFT;
		if(idx<0 || idx>=gpQdmaPriv->txDscpNum[ringIdx]) {
			QDMA_ERR("The TX DSCP index %d is invalid.\n", idx) ;
			gpQdmaPriv->counters.txIrqQueueIdxErrs++ ;
			ret = -EFAULT ;
			continue ;
		}
		
		diPtr = (struct QDMA_DscpInfo_S *)gpQdmaPriv->txBaseAddr[ringIdx] + idx ;
		if(diPtr->dscpIdx!=idx || diPtr->next!=NULL) {
			QDMA_ERR("The content of the TX DSCP_INFO(%.8x) is incorrect. ENTRY_LEN:%d, HEAD_IDX:%d, IRQ_VALUE:%.8x.\n", (uint)diPtr, entryLen, headIdx+i, irqValue) ;
			gpQdmaPriv->counters.txDscpIncorrect[ringIdx]++ ;
			ret = -EFAULT ;
			continue ;
		}
		
		txDscpPtr = diPtr->dscpPtr;

#ifdef CONFIG_TX_WB_DONE
		if(!txDscpPtr->ctrl.done) {
			QDMA_ERR("The done bit of TX DSCP is incorrect. ADDR:%.8x, IDX:%d.\n", (uint)diPtr->dscpPtr, diPtr->dscpIdx) ;
			gpQdmaPriv->counters.txDscpDoneErrs[ringIdx]++ ;
			ret = -EFAULT ;
			continue ;
		} 
#endif /* CONFIG_TX_WB_DONE */
		
#ifdef CONFIG_SUPPORT_SELF_TEST
		txMsgW0.word = txDscpPtr->msg[0] ;
		channel = txMsgW0.bits.channel ;
		queue = txMsgW0.bits.queue ;
		counters[channel][queue].tx_frames++;
#endif
		gpQdmaPriv->counters.txRecycleCounts[ringIdx]++ ;

		if(diPtr->skb == NULL)
			QDMA_ERR("diPtr->skb is NULL.\n") ;			

		if(qdma_kfree_skb(diPtr->skb, txDscpPtr->msg[0]) != 0) {
			QDMA_ERR("qdma_kfree_skb fail:headIdx:%d, dscpIdx:%d, ringIdx:%d.\n", headIdx+i, idx, ringIdx) ;
			qdma_dump_irq_info(&qdma_data) ;
			ret = -EFAULT ;	
			goto out1 ;		
		}
		txDscpPtr->msg[0] = 0;
        txDscpPtr->msg[1] = 0;
		
	    spin_lock(&gpQdmaPriv->txLock[ringIdx]) ;
		qdma_bm_push_tx_dscp(diPtr, ringIdx) ;
	    spin_unlock(&gpQdmaPriv->txLock[ringIdx]) ;
	}

out1:
    if(i!=0) {
    	for(j=0 ; j<(i>>7) ; j++) {
    		qdmaSetIrqClearLen(base, 0x80) ;
    	}
    	qdmaSetIrqClearLen(base, (i&0x7F)) ;
    }

out2:
	if(gpQdmaPriv->devCfg.flags.isTxPolling == QDMA_ENABLE)
    		spin_unlock_irqrestore(&gpQdmaPriv->txDoneLock, flags) ;
	return ret ;
}

/******************************************************************************
******************************************************************************/
__IMEM int qdma_bm_transmit_packet(struct sk_buff *skb, int ringIdx, uint msg0, uint msg1)
{
	struct QDMA_DscpInfo_S *pNewDscpInfo=NULL ;
	QDMA_DMA_DSCP_T *pTxDscp=NULL ;
	uint base = gpQdmaPriv->csrBaseAddr ;
    txMsgWord_t txMsg, *pTxMsg = &txMsg ;
    uint channel = 0, queue = 0 ;
	ulong flags = 0 ;
	int ret = 0 ;
    
    pTxMsg->pTxMsgW0 = (txMsgWord0_t *)&msg0 ;
    pTxMsg->pTxMsgW1 = (txMsgWord1_t *)&msg1 ;
	
	if(unlikely(!skb || skb->len<=0 || skb->len>CONFIG_MAX_PKT_LENS)) 
	{
		QDMA_ERR("The input arguments are wrong, skb:%.8x, skbLen:%d.\n", (uint)skb, skb->len) ; 
		return -EFAULT ;
	}

	spin_lock_irqsave(&gpQdmaPriv->txLock[ringIdx], flags);	
	/* Get unused TX DSCP from TX unused DSCP link list */	
	pNewDscpInfo = qdma_bm_pop_tx_dscp(ringIdx) ;
	if(unlikely(pNewDscpInfo == NULL)) {
		gpQdmaPriv->counters.noTxDscps[ringIdx]++ ;
		QDMA_MSG(DBG_MSG,"pNewDscpInfo is NULL\n") ; 
		spin_unlock_irqrestore(&gpQdmaPriv->txLock[ringIdx], flags);
		return -ENOSR ;
	}
    
	channel = pTxMsg->pTxMsgW0->bits.channel ;
	queue = pTxMsg->pTxMsgW0->bits.queue ;
	atomic_inc(&chnlLimit[channel]);
	atomic_inc(&queueLimit[channel][queue]);

	pTxDscp = K1_TO_K0((uint32)gpQdmaPriv->txUsingPtr[ringIdx]->dscpPtr);
	pTxDscp->msg[0] = msg0;
	pTxDscp->msg[1] = msg1;
	pTxDscp->next_idx = pNewDscpInfo->dscpIdx ;
	pTxDscp->pkt_addr = dma_map_single(NULL, skb->data, skb->len, DMA_TO_DEVICE) ;
	pTxDscp->ctrl.pkt_len = skb->len ; 
#ifdef CONFIG_SUPPORT_SELF_TEST
	pTxDscp->ctrl.done = gpQdmaPriv->devCfg.txDscpDoneBit ;
#else
	pTxDscp->ctrl.done = 0 ;
#endif /* CONFIG_SUPPORT_SELF_TEST */

	QDMA_MSG(DBG_MSG, "Hook TX DSCP to TXDMA. TX_CPU_IDX:%d, TX_NULL_IDX:%d\n", gpQdmaPriv->txUsingPtr[ringIdx]->dscpIdx, pNewDscpInfo->dscpIdx) ;
	QDMA_MSG(DBG_MSG, "TXDSCP: DONE:%d, PKT:%.8x, PKTLEN:%d, NEXT_IDX:%d, loopcnt:%d\n", 
																(uint)pTxDscp->ctrl.done, 
																(uint)pTxDscp->pkt_addr,
																(uint)pTxDscp->ctrl.pkt_len,
																(uint)pTxDscp->next_idx,
																(uint)pTxDscp->msg[1]>>24) ;
	
	protected_cache_op(Hit_Writeback_Inv_D, ((unsigned long)(pTxDscp)));
	gpQdmaPriv->txUsingPtr[ringIdx]->skb = skb ;
	gpQdmaPriv->txUsingPtr[ringIdx] = pNewDscpInfo ;

	wmb() ;

#ifdef CONFIG_SUPPORT_SELF_TEST
	gpQdmaPriv->devCfg.countDown-- ;
	if(!gpQdmaPriv->devCfg.countDown) {
		gpQdmaPriv->devCfg.countDownRound++ ;
		qdmaSetTxCpuIdx(base, ringIdx, pNewDscpInfo->dscpIdx) ;
		gpQdmaPriv->devCfg.countDown = (gpQdmaPriv->devCfg.waitTxMaxNums) ? (gpQdmaPriv->devCfg.waitTxMaxNums) : ((random32()%(gpQdmaPriv->txDscpNum[ringIdx]-1))+1) ;
	} 
#else
	qdmaSetTxCpuIdx(base, ringIdx, pNewDscpInfo->dscpIdx) ;
#endif /* CONFIG_SUPPORT_SELF_TEST */
	
	gpQdmaPriv->counters.txCounts[ringIdx]++ ;
	spin_unlock_irqrestore(&gpQdmaPriv->txLock[ringIdx], flags);

	QDMA_MSG(DBG_MSG, "RingIdx:%d, GLG:%.8x, IRQStatus:%.8x, CSR info: RX_CPU_IDX:%d, RX_DMA_IDX:%d, TX_CPU_IDX:%d, TX_DMA_IDX:%d\n", 
																ringIdx, 
																qdmaGetGlbCfg(base), 
																qdmaGetIrqStatus(base), 
																qdmaGetRxCpuIdx(base, ringIdx), 
																qdmaGetRxDmaIdx(base, ringIdx), 
																qdmaGetTxCpuIdx(base, ringIdx), 
																qdmaGetTxDmaIdx(base, ringIdx)) ;

    return ret ;
}

/******************************************************************************
******************************************************************************/
int qdma_bm_recycle_transmit_buffer(int ringIdx)
{
	struct QDMA_DscpInfo_S *diPtr=NULL ;
	QDMA_DMA_DSCP_T *pTxDscp=NULL ;
	uint base = gpQdmaPriv->csrBaseAddr ;
	void *msgPtr=NULL;
	ulong flags=0 ;
	int i=0 ;

	spin_lock_irqsave(&gpQdmaPriv->txLock[ringIdx], flags) ;

	for(i=0 ; i<gpQdmaPriv->txDscpNum[ringIdx] ; i++) {
		diPtr = (struct QDMA_DscpInfo_S *)gpQdmaPriv->txBaseAddr[ringIdx] + i ;
		if(diPtr->next==NULL && diPtr!=gpQdmaPriv->txTailPtr[ringIdx] && diPtr!=gpQdmaPriv->txUsingPtr[ringIdx]) {
			pTxDscp = diPtr->dscpPtr ;
		
			if(diPtr->skb && pTxDscp->pkt_addr) {
				dma_unmap_single(NULL, pTxDscp->pkt_addr, pTxDscp->ctrl.pkt_len, DMA_TO_DEVICE) ;
				qdma_kfree_skb(diPtr->skb, pTxDscp->msg[0]);
				msgPtr = (void *)pTxDscp->msg ;
				if(msgPtr)
				    memset((void *)pTxDscp->msg, 0, QDMA_TX_DSCP_MSG_LENS);
			}
		
			qdma_bm_push_tx_dscp(diPtr, ringIdx) ;
		}
	}

	spin_unlock_irqrestore(&gpQdmaPriv->txLock[ringIdx], flags) ;

	diPtr = gpQdmaPriv->txUsingPtr[ringIdx] ;
	qdmaSetTxCpuIdx(base, ringIdx, diPtr->dscpIdx) ;
	qdmaSetTxDmaIdx(base, ringIdx, diPtr->dscpIdx) ;
	
	return 0 ;
}

#if 0
/******************************************************************************
 Proc function for QDMA debug
******************************************************************************/
/******************************************************************************
******************************************************************************/

/*32 means echo all queue info */
static int dump_channel_or_queue_Info(uint chnlIdx, uint queIdx, char *cmd)
{
	uint channelIdx=0, queueIdx=0;	
	uint totalcnt=0, channelDscpCnt=0;
	uint qAddr=0, queueInfoMemXs0=0;
	uint queueNum[QUEUE_ALL_NUM]={0}, channelNum[CONFIG_QDMA_CHANNEL]={0};
		
	if(chnlIdx == CONFIG_QDMA_CHANNEL){
		for(channelIdx=0; channelIdx<CONFIG_QDMA_CHANNEL; channelIdx++){
			for(queueIdx=0; queueIdx<CONFIG_QDMA_QUEUE; queueIdx++){
				
				qAddr = (channelIdx<<3) + queueIdx;
				
                qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_LOCAL_QUEUE_INFO, 0, qAddr, &queueInfoMemXs0);				
				totalcnt = queueInfoMemXs0 & DBG_MEM_XS_QUEUE_INFO_CNT_MASK;			
				queueNum[qAddr] = totalcnt;
				channelDscpCnt += totalcnt;
			}
			
			channelNum[channelIdx]=channelDscpCnt;
			channelDscpCnt = 0;			
		}
		
		if(!strcmp(cmd, "queueinfo")){
			for(channelIdx=0; channelIdx<CONFIG_QDMA_CHANNEL; channelIdx++)
				for(queueIdx=0; queueIdx<CONFIG_QDMA_QUEUE; queueIdx++){
					qAddr = (channelIdx<<3) + queueIdx;
					printk("channel %d queue %d : totalcnt %d\r\n", channelIdx, queueIdx, queueNum[qAddr]);
				}
		}else /*"channelinfo"*/{
			for(channelIdx=0; channelIdx<CONFIG_QDMA_CHANNEL; channelIdx++)
				printk("channel %d :  channel dscp num: %d\r\n", channelIdx, channelNum[channelIdx]);		
		}

	}else{
	/*channelIdx < CONFIG_QDMA_CHANNEL && channelIdx >= 0 &&  queueIdx <= CONFIG_QDMA_QUEUE && queueIdx >= 0*/
		if(!strcmp(cmd, "queueinfo")){
			channelIdx=chnlIdx;
			queueIdx=queIdx;
			qAddr = (channelIdx<<3) + queueIdx;
        	qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_LOCAL_QUEUE_INFO, 0, qAddr, &queueInfoMemXs0);
			totalcnt = queueInfoMemXs0 & DBG_MEM_XS_QUEUE_INFO_CNT_MASK;	 
			printk("channel %d queue %d : totalcnt %d\r\n", channelIdx, queueIdx, totalcnt);
			
		}else{/*channelinfo*/
			for(queueIdx=0; queueIdx<CONFIG_QDMA_QUEUE; queueIdx++){
				qAddr = (channelIdx<<3) + queueIdx;
	            qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_LOCAL_QUEUE_INFO, 0, qAddr, &queueInfoMemXs0);
			    totalcnt = queueInfoMemXs0 & DBG_MEM_XS_QUEUE_INFO_CNT_MASK;
				channelDscpCnt += totalcnt;
			}
			printk("channel %d :  channel dscp num: %d\r\n", channelIdx, channelDscpCnt);
		
		}
	}

	return 0;		
}

static int dump_queue_dscplist(uint chnlIdx, uint queIdx)
{
	uint queueDscpCnt=0, headDscpIdx=0, tailDscpIdx=0, nextDscpIdx0=0;
	uint channelIdx=chnlIdx, queueIdx=queIdx;
	uint qAddr=0;
	uint queueInfoMemXs0=0, queueInfoMemXs1=0;
	int *queueDscpIdxArray=0;
	int i=0;
	
	/*get queue dscp counter, head*/
	qAddr = (channelIdx << 3)+ queueIdx;
	qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_LOCAL_QUEUE_INFO, 0, qAddr, &queueInfoMemXs0);
	queueDscpCnt = (queueInfoMemXs0 >>  DBG_MEM_XS_QUEUE_INFO_CNT_SHIFT  ) & DBG_MEM_XS_QUEUE_INFO_CNT_MASK;
	
	/*QDMA_LAN CONFIG_HWFWD_DSCP_NUM 1024, QDMA_WAN CONFIG_HWFWD_DSCP_NUM 4096*/
	if(queueDscpCnt>=CONFIG_HWFWD_DSCP_NUM || queueDscpCnt < 0){
		QDMA_ERR("invalid Dscp cnt : %d \r\n!", queueDscpCnt);
		return -EFAULT;/*need reslove*/
	}
	
	tailDscpIdx = (queueInfoMemXs0 >> DBG_MEM_XS_QUEUE_INFO_TAIL_SHIFT) & DBG_MEM_XS_QUEUE_INFO_TAIL_MASK;
	
	#if defined(QDMA_LAN)
	headDscpIdx = ((queueInfoMemXs0 >> DBG_MEM_XS_QUEUE_INFO_HEAD_SHIFT) & DBG_MEM_XS_QUEUE_INFO_HEAD_MASK);
	#else
	qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_LOCAL_QUEUE_INFO, 1, qAddr, &queueInfoMemXs1);
	headDscpIdx = (((queueInfoMemXs0 >> DBG_MEM_XS_QUEUE_INFO_HEAD_SHIFT) & 0x7F)|((queueInfoMemXs1 & 0x1F) << 7)) & DBG_MEM_XS_QUEUE_INFO_HEAD_MASK;
	#endif
	queueDscpIdxArray=(int*)kzalloc(queueDscpCnt, sizeof(uint));
	if(!queueDscpIdxArray){
		QDMA_ERR("Allocate memory for queue DscpIdx Array failed.\n") ;
		return -ENOMEM ;
	}
	
	queueDscpIdxArray[0]=headDscpIdx;
	for(i=1; i<queueDscpCnt; i++){
		qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_LOCAL_DSCP_INFO, 0, queueDscpIdxArray[i-1], &nextDscpIdx0);
		queueDscpIdxArray[i]=nextDscpIdx0 & DBG_MEM_XS_DSCP_IDX_MASK;	
	}

	printk("channel%d queue%d", channelIdx, queueIdx);
	for(i=0; i<queueDscpCnt; i++){
		if(i%10 == 0){
			printk("\r\n");
			printk("%-4d - %-4d : %-4d ", i, i+9, queueDscpIdxArray[i]);
		}else
			printk("%-4d ",queueDscpIdxArray[i]);		
	}
	kfree(queueDscpIdxArray);
	
	return 0;
}

static int get_lmgr_queue_dscpIdx(uint dscpIdx){
	uint nextDscpIdx = 0;
	if(dscpIdx >= CONFIG_DSCP_NUM_MAX || dscpIdx < 0){
		QDMA_ERR("Invalid dscpIdx.\n");
		return -EINVAL;
	}
	qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_LOCAL_DSCP_INFO, 0, dscpIdx, &nextDscpIdx);
	return nextDscpIdx;
	
}

static int get_lmgr_queue_info(uint channelIdx, uint queueIdx, QDMA_Lmgr_queueInfo_T queueInfo){
	
	uint queueInfoMemXs0 = 0, queueInfoMemXs1 = 0;
	uint qAddr=0;

	if(channelIdx >= CONFIG_QDMA_CHANNEL || channelIdx < 0 ){
		QDMA_ERR("Invalid Channel Index %d\n", channelIdx);
		return -EINVAL;
	}else if(queueIdx >= CONFIG_QDMA_QUEUE || queueIdx < 0){
		QDMA_ERR("Invalid Queue Index %d\n", queueIdx);
		return -EINVAL;
	}
	
	qAddr = (channelIdx<<3) + queueIdx;
	#if defined(QDMA_LAN)
	qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_LOCAL_QUEUE_INFO, 0, qAddr, &queueInfoMemXs0);
	queueInfo.cntr = queueInfoMemXs0 & DBG_MEM_XS_QUEUE_INFO_CNT_MASK;
	queueInfo.tailIdx = (queueInfoMemXs0 >> DBG_MEM_XS_QUEUE_INFO_TAIL_SHIFT) & DBG_MEM_XS_QUEUE_INFO_TAIL_MASK;
	queueInfo.headIdx = (queueInfoMemXs0 >> DBG_MEM_XS_QUEUE_INFO_HEAD_SHIFT) & DBG_MEM_XS_QUEUE_INFO_HEAD_MASK;
	#else
	qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_LOCAL_QUEUE_INFO, 0, qAddr, &queueInfoMemXs0);
	qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_LOCAL_QUEUE_INFO, 1, qAddr, &queueInfoMemXs1);
	queueInfo.cntr = queueInfoMemXs0 & DBG_MEM_XS_QUEUE_INFO_CNT_MASK;
	queueInfo.tailIdx = (queueInfoMemXs0 >> DBG_MEM_XS_QUEUE_INFO_TAIL_SHIFT) & DBG_MEM_XS_QUEUE_INFO_TAIL_MASK;
	queueInfo.headIdx = (((queueInfoMemXs0 >> DBG_MEM_XS_QUEUE_INFO_HEAD_SHIFT) & 0x7F)|((queueInfoMemXs1 & 0x1F) << 7)) & DBG_MEM_XS_QUEUE_INFO_HEAD_MASK;
	#endif

	return 0;		
}

static int get_lmgr_chnl_queue_bufsize(uint channelIdx, uint queueBuf[]){

	uint queueBufMemXs0=0, queueBufMemXs1=0, queueBufMemXs2=0, queueBufMemXs3=0, queueBufMemXs4=0, queueBufMemXs5=0;
	if(queueBuf == NULL){
		QDMA_ERR("NULL ptr.\n");
		return EFAULT;
	}
	if(channelIdx >= CONFIG_QDMA_CHANNEL || channelIdx < 0 ){
		QDMA_ERR("Invalid Channel Index %d\n", channelIdx);
		return -EINVAL;
	}
	/*get queue bufsize from reg*/
	qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_BUFFER_MONITOR, 0, channelIdx, &queueBufMemXs0);
	qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_BUFFER_MONITOR, 1, channelIdx, &queueBufMemXs1);
	qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_BUFFER_MONITOR, 2, channelIdx, &queueBufMemXs2);
	qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_BUFFER_MONITOR, 3, channelIdx, &queueBufMemXs3);
	qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_BUFFER_MONITOR, 4, channelIdx, &queueBufMemXs4);
	qdmaGetDbgMemXsConfig(QDMA_DBG_MEM_XS_MEMSEL_BUFFER_MONITOR, 5, channelIdx, &queueBufMemXs5);

	queueBuf[0] = queueBufMemXs0 & 0xFFFFFF;
	queueBuf[1] = ((queueBufMemXs0 >> 24)| (queueBufMemXs1 & 0xFFFF)) & 0xFFFFFF;
	queueBuf[2] = ((queueBufMemXs1 >> 16)| (queueBufMemXs2 & 0xFF)) & 0xFFFFFF;
	queueBuf[3] = (queueBufMemXs2 >> 8) & 0xFFFFFF;

	queueBuf[4] = queueBufMemXs3 & 0xFFFFFF;
	queueBuf[5] = ((queueBufMemXs3 >> 24)| (queueBufMemXs4 & 0xFFFF)) & 0xFFFFFF;
	queueBuf[6] = ((queueBufMemXs4 >> 16)| (queueBufMemXs5 & 0xFF)) & 0xFFFFFF;
	queueBuf[7] = (queueBufMemXs5 >> 8) & 0xFFFFFF;

	return 0;
}
#endif

/******************************************************************************
******************************************************************************/
static int qdma_bm_counters_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	qdma_dump_cpu_counters(NULL);
	return 0;
}

/******************************************************************************
******************************************************************************/
static int qdma_bm_counters_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char val_string[64] ;
	uint cmd=0 ;
	
	if (count > sizeof(val_string) - 1)
		return -EINVAL ;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT ;

	sscanf(val_string, "%d", &cmd) ;

	if(cmd == 1) {
		qdma_clear_cpu_counters(NULL);
	}
	
	return count ;
}

/******************************************************************************
******************************************************************************/
static int qdma_bm_debug_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	return 0 ;
}

/******************************************************************************
******************************************************************************/
static int qdma_bm_debug_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char val_string[64], cmd[32], subcmd[32] ;
	uint value=0, cnt=0, offset=0;
	struct ECNT_QDMA_Data qdma_data;

	if (count > sizeof(val_string) - 1)
		return -EINVAL ;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT ;

	sscanf(val_string, "%s %s %d %x", cmd, subcmd, &cnt, &offset) ;

	if(!strcmp(cmd, "level")) {
#ifdef CONFIG_DEBUG
		value = subcmd[0] - 48 ;
		qdma_data.qdma_private.qdma_old_proc.dbgLevel = value;
		qdma_set_dbg_level(&qdma_data);
#endif /* CONFIG_DEBUG */
	} else if(!strcmp(cmd, "dump")) {
		if(!strcmp(subcmd, "csr")) {
			qdma_dump_register_value(NULL);
		} else if(!strcmp(subcmd, "dscp")) {
			qdma_dump_descriptor_info(NULL);
		} else if(!strcmp(subcmd, "irq")) {
			qdma_dump_irq_info(NULL);
		} else if(!strcmp(subcmd, "hwfwd")) {
			qdma_dump_hwfwd_info(NULL);
		}
	} else if(!strcmp(cmd, "dbgcntr")) {
		if(!strcmp(subcmd, "dump")) {
			qdma_dump_dbg_cntr_value(NULL);
		} else if(!strcmp(subcmd, "clear")) {
			qdma_clear_dbg_cntr_value_all(NULL);
		} else if(!strcmp(subcmd, "queue")) {
			qdma_clear_and_set_dbg_cntr_queue_group(NULL);
		} else if(!strcmp(subcmd, "channel_cpu")) {
			qdma_data.qdma_private.cntrSrc = QDMA_DBG_CNTR_CHNL_TXCPU;
			qdma_clear_and_set_dbg_cntr_channel_group(&qdma_data);
		} else if(!strcmp(subcmd, "channel_fwd1")) {
			qdma_data.qdma_private.cntrSrc = QDMA_DBG_CNTR_CHNL_TXFWD1;
			qdma_clear_and_set_dbg_cntr_channel_group(&qdma_data);
		} else if(!strcmp(subcmd, "channel_fwd2")) {
			qdma_data.qdma_private.cntrSrc = QDMA_DBG_CNTR_CHNL_TXFWD2;
			qdma_clear_and_set_dbg_cntr_channel_group(&qdma_data);
		}
	} else if(!strcmp(cmd, "test")) {
		if(!strcmp(subcmd, "wrr")) {
			int i, j ;
			QDMA_TxQosScheduler_T txQos ;
			
			for(i=0 ; i<CONFIG_QDMA_CHANNEL ; i++) {
				txQos.channel = i ;
				txQos.qosType = i%QDMA_TXQOS_TYPE_NUMS ;
				for(j=0 ; j<CONFIG_QDMA_QUEUE ; j++) {
					txQos.queue[j].weight = i*10+j ;
				}
				qdma_data.qdma_private.qdma_tx_qos.pTxQos = &txQos;
				qdma_set_tx_qos(&qdma_data) ;
			}
			
			for(i=0 ; i<CONFIG_QDMA_CHANNEL ; i++) {
				memset(&txQos, 0, sizeof(QDMA_TxQosScheduler_T)) ;
				
				txQos.channel = i ;
				qdma_data.qdma_private.qdma_tx_qos.pTxQos = &txQos;
				qdma_get_tx_qos(&qdma_data) ;
				printk("Channel:%d, Type:%d, Q0:%d, Q1:%d, Q2:%d, Q3:%d, Q4:%d, Q5:%d, Q6:%d, Q7:%d\n",
									i, txQos.qosType, 
									txQos.queue[0].weight,
									txQos.queue[1].weight,
									txQos.queue[2].weight,
									txQos.queue[3].weight,
									txQos.queue[4].weight,
									txQos.queue[5].weight,
									txQos.queue[6].weight,
									txQos.queue[7].weight) ;									
			}
		} else if(!strcmp(subcmd, "dma_busy")) {
			qdma_data.qdma_private.qdma_old_proc.busyDuration = cnt;
			qdma_dump_dma_busy(&qdma_data); //the unit of cnt is second.
		} else if(!strcmp(subcmd, "reg_polling")) {
			qdma_data.qdma_private.qdma_old_proc.regOffset = offset;
			qdma_data.qdma_private.qdma_old_proc.pollingDuration = cnt;
			qdma_dump_reg_polling(&qdma_data);	//the unit of cnt is second.
		} else if(!strcmp(subcmd, "forceRX1")) { /* force rx1:rx0=5:0 */
			qdma_data.qdma_private.qdma_old_proc.forceEn = cnt;
			qdma_set_force_receive_rx_ring1(&qdma_data);
		}
	}

	return count ;
}

static int qdma_bm_ratelimit_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int index=0 ;
	off_t pos=0, begin=0 ;

	index += sprintf(buf+ index, "echo ratelimit [tx/rx] r > /proc/%s\n", ratelimit_path) ;
	CHK_BUF() ;
	index += sprintf(buf+ index, "echo ratelimit [tx/rx] w [Enable/Disable] [unit:(1~%dkbps)] > /proc/%s\n", UNIT_MAX, ratelimit_path) ;
	CHK_BUF() ;
	index += sprintf(buf+ index, "echo ratelimit [tx/rx] w [chanIdx/ringIdx] [pir:(kbps)] [dropEn:Enable/Disable]> /proc/%s\n", ratelimit_path) ;
	CHK_BUF() ;
	index += sprintf(buf+ index, "echo ratemeter tx r > /proc/%s\n", ratelimit_path) ;
	CHK_BUF() ;
	/*  the unit can not be 1kbps for the minimum rate meter unit is 2. */
	index += sprintf(buf+ index, "echo ratemeter tx w [Enable/Disable] [unit:(2~%dkbps)] > /proc/%s\n", UNIT_MAX, ratelimit_path) ;
	CHK_BUF() ;
	
	*eof = 1 ;

done:
	*start = buf + (off - begin) ;
	index -= (off - begin) ;
	if(index<0) 	index = 0 ;
	if(index>count) 	index = count ;
	return index ;
}

static int qdma_bm_ratelimit_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char val_string[128], cmd[32], direction[32], action[32], Enable[32], dropEn[32] ;
	int index=0, pir=0, unit=0, dataRate=0 ;
	int enFlag=0, dropEnFlag=0;
	QDMA_TxRateLimitCfg_T txRateLimitCfg ;
	QDMA_RxRateLimitCfg_T rxRateLimitCfg ;
	QDMA_TxRateLimitSet_T txRateLimitSetParam ;
	QDMA_TxRateLimitGet_T txRateLimitGetParam ;
	QDMA_RxRateLimitSet_T rxRateLimitSetParam ;
	QDMA_RxRateLimitGet_T rxRateLimitGetParam ;
	QDMA_TxRateMeter_T txRateMeterCfg ;
	struct ECNT_QDMA_Data qdma_data;
	
	if (count > sizeof(val_string) - 1)
		return -EINVAL ;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT ;

	sscanf(val_string, "%s %s %s %s %d %s", cmd, direction, action, Enable, &pir, dropEn) ;

	if(!strcmp(Enable, "Enable"))
		enFlag = QDMA_ENABLE ;
	else if(!strcmp(Enable, "Disable"))
		enFlag = QDMA_DISABLE ;
	else {
		index = atoi(Enable);
		if((index < 0) || (index > CONFIG_QDMA_CHANNEL && (!strcmp(direction, "tx"))) || (index > RX_RING_NUM && (!strcmp(direction, "rx")))) {
			printk("Fault: index is %d, which should between 0 and %d\n", index, ((!strcmp(direction, "tx"))?(CONFIG_QDMA_CHANNEL):(RX_RING_NUM))) ;
			return -EFAULT ;
		}
	}
	
	if(!strcmp(dropEn, "Enable"))
		dropEnFlag = QDMA_ENABLE ;
	else
		dropEnFlag = QDMA_DISABLE ;

	if((!strcmp(action, "w")) && (!strcmp(cmd, "ratelimit")) && ((!strcmp(Enable, "Enable")) || (!strcmp(Enable, "Disable")))) {
		unit = pir;
		if(unit<1 || unit>UNIT_MAX) {
			printk("Fault: Rate Limit unit is %dkbps, which should between 1 and %d\n", unit, UNIT_MAX) ;
			return -EFAULT ;
		}
	}
	if((!strcmp(action, "w")) && (!strcmp(cmd, "ratemeter"))) {
		unit = pir;
		if(unit<2 || unit>UNIT_MAX) {
			printk("Fault: Rate Meter unit is %dkpbs, which should between 2 and %d\n", unit, UNIT_MAX) ;
			return -EFAULT ;
		}
	}
	
	if(!strcmp(cmd, "ratelimit")) {
		if(!strcmp(direction, "tx")) {
			if(!strcmp(action, "r")) {
				qdma_data.qdma_private.txRateLimitCfgPtr = &txRateLimitCfg;
				qdma_get_tx_ratelimit_cfg(&qdma_data) ;
				printk("txRateLimitEn:Enable, txBucketScale:%d, unit:%dkbps\n", 1<<txRateLimitCfg.txRateLimitBucketScale, txRateLimitCfg.txRateLimitUnit) ;
				if(txRateLimitCfg.txRateLimitUnit) {
					for(index=0; index<CONFIG_QDMA_CHANNEL; index++) {
						txRateLimitGetParam.chnlIdx = (unchar)index;
						qdma_data.qdma_private.txRateLimitGetPtr = &txRateLimitGetParam;
						qdma_get_tx_ratelimit(&qdma_data) ;
						if(txRateLimitGetParam.rateLimitValue || txRateLimitGetParam.pbsParamValue) {
							printk("Channel_%d:  pir:%dkbps, pbs:%dByte\n", index, txRateLimitGetParam.rateLimitValue, 
								txRateLimitGetParam.pbsParamValue<<txRateLimitCfg.txRateLimitBucketScale) ;
						}
					}
				}
			} else if(!strcmp(action, "w")) {
				if(unit != 0) {	/*  enable or disable */
					txRateLimitCfg.txRateLimitBucketScale = QDMA_TRTCM_SCALE_2BYTE ;
					txRateLimitCfg.txRateLimitUnit = unit ;
					qdma_data.qdma_private.txRateLimitCfgPtr = &txRateLimitCfg;
					//qdma_set_tx_ratelimit_cfg(&qdma_data) ;
				} else {		/*  per channel ratelimit config */
					txRateLimitSetParam.chnlIdx = index ;
					//txRateLimitSetParam.pbsParamValue = 10000 ;
					txRateLimitSetParam.rateLimitValue = pir ;
					if(txRateLimitSetParam.rateLimitValue > (4<<20)) {
						printk("Error: ratelimitValue is %dkbps, which shold less than 4gbps!", pir);
						return -EFAULT;
					}
					printk("channel:%d, pirParamValue:%ukbps\n", index, txRateLimitSetParam.rateLimitValue) ;
					qdma_data.qdma_private.txRateLimitSetPtr = &txRateLimitSetParam;
					qdma_set_tx_ratelimit(&qdma_data) ;
				}
			}
		} else if(!strcmp(direction, "rx")) {
			if(!strcmp(action, "r")) {
				qdma_data.qdma_private.rxRateLimitCfgPtr = &rxRateLimitCfg;
				qdma_get_rx_ratelimit_cfg(&qdma_data) ;
				printk("rxDropEn:Enable, rxRateLimitEn:%s, rxBucketScale:%d, rxRateLimitUnit:%dkbps\n", 
					(rxRateLimitCfg.rxRateLimitEn)?("Enable"):("Disable"), 1<<rxRateLimitCfg.rxRateLimitBucketScale, rxRateLimitCfg.rxRateLimitUnit) ;
				if(rxRateLimitCfg.rxRateLimitUnit) {
					for(index=0; index<RX_RING_NUM; index++) {
						rxRateLimitGetParam.ringIdx = index;
						qdma_data.qdma_private.rxRateLimitGetPtr = &rxRateLimitGetParam;
						qdma_get_rx_ratelimit(&qdma_data) ;
						if(rxRateLimitGetParam.rateLimitValue || rxRateLimitGetParam.pbsParamValue) {
							printk("RX%d:  pir:%dkbps, pbs:%dByte\n", index, rxRateLimitGetParam.rateLimitValue, rxRateLimitGetParam.pbsParamValue<<rxRateLimitCfg.rxRateLimitBucketScale) ;
						}
					}
				}
			} else if(!strcmp(action, "w")) {
				if(unit != 0) {
					rxRateLimitCfg.rxRateLimitEn = enFlag ;
					rxRateLimitCfg.rxRateLimitBucketScale = QDMA_TRTCM_SCALE_2BYTE ;
					rxRateLimitCfg.rxRateLimitUnit = unit ;
					qdma_data.qdma_private.mode = enFlag;
					qdma_set_rx_ratelimit_en(&qdma_data) ;
					qdma_data.qdma_private.pktMode = QDMA_RX_RATE_LIMIT_BY_BYTE;
					qdma_set_rx_ratelimit_pkt_mode(&qdma_data) ;
				} else {
					rxRateLimitSetParam.ringIdx = index ;
					//rxRateLimitSetParam.pbsParamValue = 10000 ;
					rxRateLimitSetParam.rateLimitValue = pir ;
					if(rxRateLimitSetParam.rateLimitValue > (4<<20)) {
						printk("Error: ratelimitValue is %dkbps, which shold less than 4gbps!", pir);
						return -EFAULT;
					}
					printk("ringIdx:%d, pirParamValue:%ukbps\n", index, rxRateLimitSetParam.rateLimitValue) ;
					qdma_data.qdma_private.rxRateLimitSetPtr = &rxRateLimitSetParam;
					qdma_set_rx_ratelimit(&qdma_data) ;
				}
			}
		} else {
			printk("Fault: ratelimit only effect on tx/rx Direction\n") ;		
		}
	} else if(!strcmp(cmd, "ratemeter")) {	
		if(!strcmp(direction, "tx")) {
			if(!strcmp(action, "r")) {
				qdma_data.qdma_private.txRateMeterPtr = &txRateMeterCfg;
				qdma_get_tx_ratemeter(&qdma_data) ;
				printk("txRateMeterEn:Enable, txRateMeterDivisor:%d, txRateMeterTimeSlice:%dus\n", 
					1<<txRateMeterCfg.txRateMeterTimeDivisor, txRateMeterCfg.txRateMeterTimeSlice) ;
				for(index=0; index<CONFIG_QDMA_CHANNEL; index++) {
					qdma_data.qdma_private.channel = index;
					dataRate = qdma_get_tx_channel_datarate(&qdma_data);
					if(dataRate != -1) {
						printk("Channel_%d: rateMeter:%dkbps\n", index, dataRate/1000) ;
					}
				}
			} else if(!strcmp(action, "w")) {
				if(qdmaRateMeterValueCalCulate(unit, (ushort *)&txRateMeterCfg.txRateMeterTimeDivisor, (ushort *)&txRateMeterCfg.txRateMeterTimeSlice) != -1) {
					qdma_data.qdma_private.txRateMeterPtr = &txRateMeterCfg;
					qdma_set_tx_ratemeter(&qdma_data) ;
				}
			}
		} else {
			printk("Fault: ratemeter only effect on tx Direction\n") ;		
		}
	}

	return count ;
}

#if defined(TCSUPPORT_QDMA_VERIFY) && defined(QDMA_LAN)
int qdamSetSwitchSpecialTag(QDMA_Mode_t mode)
{
	if(mode == QDMA_DISABLE) {
		IO_CBITS(SWITCH_PROT6_VLAN_CONTROL, SWITCH_PORT_STAG);
	} else {
		IO_SBITS(SWITCH_PROT6_VLAN_CONTROL, SWITCH_PORT_STAG);
	}
	return 0;
}

static int qdma_bm_verify_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int index=0 ;
	off_t pos=0, begin=0 ;

	index += sprintf(buf+index, "echo [lanTestFlag:0~1] [isDeiFlag:0~1] [wanTestFlag:0~1] [highQueueFlag:0~1] ") ;
	index += sprintf(buf+index, "[forceRing0Flag:0~1] [channelIdx:0~7] [queueIdx:0~7] > /proc/%s\n", qdma_verify_path) ;
	index += sprintf(buf+index, "PS: highQueueFlag means force to queue7 and queue6 in wan test\n") ;
	index += sprintf(buf+index, "PS: forceRing0Flag means force set all pkts to TX Ring0 in wan test\n\n") ;
	index += sprintf(buf+index, "lanTestFlag            = 0x%d\n", lanTestFlag);
	index += sprintf(buf+index, "isDeiFlag              = 0x%d\n", isDeiFlag);
	index += sprintf(buf+index, "wanTestFlag            = 0x%d\n", wanTestFlag);
	index += sprintf(buf+index, "highQueueFlag          = 0x%d\n", highQueueFlag);
	index += sprintf(buf+index, "forceRing0Flag         = 0x%d\n", forceRing0Flag);
	index += sprintf(buf+index, "channelIdx             = 0x%d\n", channelIdx);
	index += sprintf(buf+index, "queueIdx               = 0x%d\n", queueIdx);
	CHK_BUF() ;
	
	*eof = 1 ;

done:
	*start = buf + (off - begin) ;
	index -= (off - begin) ;
	if(index<0) 	index = 0 ;
	if(index>count) 	index = count ;
	return index ;
}

static int qdma_bm_verify_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char val_string[128];
	
	if (count > sizeof(val_string) - 1)
		return -EINVAL ;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT ;
	
	sscanf(val_string, "%d %d %d %d %d %d %d", &lanTestFlag, &isDeiFlag, &wanTestFlag, &highQueueFlag, &forceRing0Flag, &channelIdx, &queueIdx) ;

	if(lanTestFlag) {
		lanTestFlag = 1;
		qdamSetSwitchSpecialTag(QDMA_DISABLE);
	} else {
		qdamSetSwitchSpecialTag(QDMA_ENABLE);
	}
	
	if(isDeiFlag)
		isDeiFlag = 1;
	if(wanTestFlag)
		wanTestFlag = 1;
	if(highQueueFlag)
		highQueueFlag = 1;
	if(forceRing0Flag)
		forceRing0Flag = 1;
}
#endif

static int qdma_bm_qos_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int index=0 ;
	off_t pos=0, begin=0 ;

	index += sprintf(buf+ index, "echo [chnlId:0~31] sp > /proc/%s\n", qos_path) ;
	CHK_BUF() ;
	index += sprintf(buf+ index, "echo [chnlId:0~31] wrr w0 w1 w2 w3 w4 w5 w6 w7 > /proc/%s\n", qos_path) ;
	CHK_BUF() ;
	index += sprintf(buf+ index, "echo [chnlId:0~31] wrr2 w0 w1 > /proc/%s\n", qos_path) ;
	CHK_BUF() ;
	index += sprintf(buf+ index, "echo [chnlId:0~31] wrr3 w0 w1 w2 > /proc/%s\n", qos_path) ;
	CHK_BUF() ;
	index += sprintf(buf+ index, "echo [chnlId:0~31] wrr4 w0 w1 w2 w3 > /proc/%s\n", qos_path) ;
	CHK_BUF() ;
	index += sprintf(buf+ index, "echo [chnlId:0~31] wrr5 w0 w1 w2 w3 w4 > /proc/%s\n", qos_path) ;
	CHK_BUF() ;
	index += sprintf(buf+ index, "echo [chnlId:0~31] wrr6 w0 w1 w2 w3 w4 w5 > /proc/%s\n", qos_path) ;
	CHK_BUF() ;
	index += sprintf(buf+ index, "echo [chnlId:0~31] wrr7 w0 w1 w2 w3 w4 w5 w6 > /proc/%s\n", qos_path) ;
	CHK_BUF() ;
	
	*eof = 1 ;

done:
	*start = buf + (off - begin) ;
	index -= (off - begin) ;
	if(index<0) 	index = 0 ;
	if(index>count) 	index = count ;
	return index ;
}

static int qdma_bm_qos_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char val_string[128], mode[16];
	int i=0, chnlID=0, weight[CONFIG_QDMA_QUEUE] ;
	QDMA_TxQosScheduler_T txQos;
	struct ECNT_QDMA_Data qdma_data;
	
	if (count > sizeof(val_string) - 1)
		return -EINVAL ;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT ;

	sscanf(val_string, "%d %s %d %d %d %d %d %d %d %d", &chnlID, mode, &weight[0], &weight[1], &weight[2], &weight[3], &weight[4], &weight[5], &weight[6], &weight[7]) ;
	
	txQos.channel = chnlID ;
	
	for(i=0 ; i<CONFIG_QDMA_QUEUE ; i++) {
		txQos.queue[i].weight = weight[i];
	}

	if(!strcmp(mode, "sp")) {
		txQos.qosType = 1 ;
	} else if(!strcmp(mode, "wrr")) {
		txQos.qosType = 0 ;
	} else if(!strcmp(mode, "wrr2")) {
		txQos.qosType = 7 ;
	} else if(!strcmp(mode, "wrr3")) {
		txQos.qosType = 6 ;
	} else if(!strcmp(mode, "wrr4")) {
		txQos.qosType = 5 ;
	} else if(!strcmp(mode, "wrr5")) {
		txQos.qosType = 4 ;
	} else if(!strcmp(mode, "wrr6")) {
		txQos.qosType = 3 ;
	} else if(!strcmp(mode, "wrr7")) {
		txQos.qosType = 2 ;
	}
	
	qdma_data.qdma_private.qdma_tx_qos.pTxQos = &txQos;
	qdma_set_tx_qos(&qdma_data);

	return count ;
}

#ifndef QDMA_LAN
#if defined (QOS_REMARKING) || defined (TCSUPPORT_HW_QOS)
static int mac_qos_read_proc(char *page, char **start, off_t off,
		int count, int *eof, void *data){

	int len = sprintf(page, "%d %d %d %d %d\r\n",*qos_wrr_info, *(qos_wrr_info + 1), *(qos_wrr_info + 2), *(qos_wrr_info + 3), *(qos_wrr_info + 4));
	return len;
}
static int mac_qos_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){
	int len;
	char get_buf[32];
	int max_wrr_val = 0, i, j;
	QDMA_TxQosScheduler_T txQos;
	struct ECNT_QDMA_Data qdma_data;
	/* do a range checking, don't overflow buffers in kernel modules */
	if(count > 32)
		len = 32;
	else
		len = count;
	/* use the copy_from_user function to copy buffer data to our get_buf */
	if(copy_from_user(get_buf, buffer, len))
		return -EFAULT;
	/* zero terminate get_buf */
	get_buf[len]='\0';

	if(sscanf(get_buf, "%d %d %d %d %d", qos_wrr_info, (qos_wrr_info+1), (qos_wrr_info+2), (qos_wrr_info+3), (qos_wrr_info+4)) != 5)
		return count;
	
	/* find max qos wrr weight */
	for (i = 0; i < 4; i++) {
		if (max_wrr_val < qos_wrr_info[i + 1]) {
			max_wrr_val = qos_wrr_info[i + 1];
			max_prio = i;
		}
	}

	printk("qos_wrr_info: %d %d %d %d %d\n", qos_wrr_info[0], qos_wrr_info[1], qos_wrr_info[2], qos_wrr_info[3], qos_wrr_info[4]);

	qos_wrr_user = QOS_DMAWRR_USERDEFINE;
	if(*qos_wrr_info == 0){  /*strict priority*/
	/*TSARM_GFR &= ~(GFR_DMT_WRR_EN); */

		printk("QDMA QoS: HW SP\n");

        /* set QDMA as Strict Priority Mode,
	 	 * so that p7> ... >p0 for all paths */
		for(i=0 ; i<ATM_VC_MAX ; i++){
			txQos.channel = i ;
			txQos.qosType = QDMA_TXQOS_TYPE_SP ;	/*  Strict Priority */

			/* QDMA has 8 queue */
			for(j=0 ; j<8 ; j++){
				txQos.queue[j].weight = 0;
			}
            
			qdma_data.qdma_private.qdma_tx_qos.pTxQos = &txQos;
			qdma_set_tx_qos(&qdma_data) ;
		}

		for(i=0 ; i<ATM_VC_MAX ; i++){
			memset(&txQos, 0, sizeof(QDMA_TxQosScheduler_T)) ;
			txQos.channel = i ;
			qdma_data.qdma_private.qdma_tx_qos.pTxQos = &txQos;
			qdma_get_tx_qos(&qdma_data) ;
		}
	} else{  /*WRR*/

		printk("QDMA QoS: HW WRR\n");

        /* EN7512:set QDMA's SP & WRR priority registers,
	 	 * so that p7>p6>p5>p4> p3:p2:p1:p0 for all path && 
		 * p3:p2:p1:p0 == qos_wrr_info[1]:qos_wrr_info[2]:
		 * 				  qos_wrr_info[3]:qos_wrr_info[4] */
		for(i=0 ; i<ATM_VC_MAX ; i++){
			txQos.channel = i ;            
			txQos.qosType = QDMA_TXQOS_TYPE_SPWRR4 ;
            
			/* QDMA has 8 queue */
			for(j=0 ; j<4 ; j++){
				txQos.queue[j].weight = qos_wrr_info[4-j];
			}

			for(j=4 ; j<8 ; j++){
				txQos.queue[j].weight = 0;
			}
			
			qdma_data.qdma_private.qdma_tx_qos.pTxQos = &txQos;
			qdma_set_tx_qos(&qdma_data) ;
		}

		for(i=0 ; i<ATM_VC_MAX ; i++){
			memset(&txQos, 0, sizeof(QDMA_TxQosScheduler_T)) ;
			txQos.channel = i ;
			qdma_data.qdma_private.qdma_tx_qos.pTxQos = &txQos;
			qdma_get_tx_qos(&qdma_data) ;
          
            printk("Channel:%d, Type:%d, Q0:%d, Q1:%d, Q2:%d, Q3:%d, Q4:%d, Q5:%d, Q6:%d, Q7:%d\n",
	           i, txQos.qosType,
               txQos.queue[0].weight,
               txQos.queue[1].weight,
               txQos.queue[2].weight,
               txQos.queue[3].weight,
               txQos.queue[4].weight,
               txQos.queue[5].weight,
               txQos.queue[6].weight,
                   txQos.queue[7].weight) ;
		}
	}
	return len;
}
#endif

#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_XPON_HAL_API_QOS_CA) || defined(TCSUPPORT_CT_PON)
static int mac_default_queuemask_read_proc(
			char *page, char **start, off_t off,
			int count, int *eof, void *data
)
{
	printk("\nqos gQueueMask is 0x%x\n",gQueueMask);		
	return 0;
}


static int mac_default_queuemask_write_proc(
		struct file *file, const char *buffer,
		unsigned long count, void *data
)
{
	char valString[4];
	int val = 0;
	
	if (count > sizeof(valString) - 1)
		return -EINVAL;

	if (copy_from_user(valString, buffer, count))
		return -EINVAL;

	valString[count] = '\0';
	sscanf(valString, "%d", &val);
	if (val)
	{
		gQueueMask = val;
		printk("\nqos gQueueMask is 0x%x\n",gQueueMask);
	}

	return count;
}
#endif


#ifdef TCSUPPORT_QOS
static int mac_qos_flag_read_proc(char *page, char **start, off_t off,
		int count, int *eof, void *data){

	if (qos_flag == QOS_SW_PQ) {
		printk("qos discipline is PQ.\n");
	}
	else if (qos_flag == QOS_SW_WRR) {
		printk("qos discipline is WRR.\n");
	}
	else if (qos_flag == QOS_SW_CAR) {
		printk("qos discipline is CAR.\n");
	}
	else if (qos_flag == QOS_HW_WRR) {
		printk("qos discipline is HW WRR.\n");
	}
	else if (qos_flag == QOS_HW_PQ) {
		printk("qos discipline is HW PQ.\n");
	}
	else {
		printk("qos is disabled.\n");
	}

#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_XPON_HAL_API_QOS_CA) 	
	printk("\n isNeedHWQoS =%d.\n\n",isNeedHWQoS);
#endif
	return 0;
}

static int mac_qos_flag_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){

	char qos_disc[10];
	int len;

	if (count > 10) {
		len = 10;
	}
	else {
		len = count;
	}
	memset(qos_disc, 0, sizeof(qos_disc));
	
	if(copy_from_user(qos_disc, buffer, len))
		return -EFAULT;

#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_XPON_HAL_API_QOS_CA) 
	isNeedHWQoS = 1;
#endif
	qos_disc[len] = '\0';
	if (!strncmp(qos_disc, "PQ", 2)) {
		qos_flag = QOS_SW_PQ;
#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_XPON_HAL_API_QOS_CA) 
		isNeedHWQoS = 0;
#endif
	}
	else if (!strncmp(qos_disc, "WRR", 3)) {
		qos_flag = QOS_SW_WRR;
#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_XPON_HAL_API_QOS_CA) 
		isNeedHWQoS = 0;
#endif
	}
	else if (!strncmp(qos_disc, "CAR", 3)) {
		qos_flag = QOS_SW_CAR;
#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_XPON_HAL_API_QOS_CA) 
		isNeedHWQoS = 0;
#endif
	}
	else if (!strncmp(qos_disc, "HWWRR", 5)) {
		qos_flag = QOS_HW_WRR;
	}
	else if (!strncmp(qos_disc, "HWPQ", 4)) {
		qos_flag = QOS_HW_PQ;
	}
	else {
		qos_flag = NULLQOS;
	}

	return len;
}
#endif

static int mac_resource_limit_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char val_string[64];
	int ret = 0;
		
	if (count > sizeof(val_string) - 1)
	{
		ret = -EFAULT;
		return ret;
	}
	memset(val_string,0,64);
	if (copy_from_user(val_string, buffer, count))
	{
		ret = -EFAULT;
		return ret;
	}
	sscanf(val_string, "%x %x", &channel_limit_threshold, &queue_limit_threshold) ;
	
	return count;
}

static int mac_resource_limit_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	printk("0x%x,0x%x\n",channel_limit_threshold,queue_limit_threshold);
	return 0;
}
#endif

#if 0
static int qdma_bm_memxs_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	return 0;
}


static int qdma_bm_memxs_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char val_string[64], cmd[32];
	uint channelIdx=0, queueIdx=0;
	
	
	if (count > sizeof(val_string) - 1)
		return -EINVAL ;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT ;

	sscanf(val_string, "%s %d %d", cmd, &channelIdx, &queueIdx) ;
			
	if(!strcmp(cmd, "queueinfo")||!strcmp(cmd, "channelinfo")) {
		if(channelIdx > CONFIG_QDMA_CHANNEL|| channelIdx < 0 || queueIdx>=CONFIG_QDMA_QUEUE|| queueIdx < 0){
			QDMA_ERR("invalid input param: channelIdx %d! , queueIdx: %d \r\n", channelIdx, queueIdx);
			return -EINVAL;
	    }else
		    dump_channel_or_queue_Info(channelIdx, queueIdx, cmd);
	}else if(!strcmp(cmd, "dscplist")){
		if(channelIdx >= CONFIG_QDMA_CHANNEL|| channelIdx < 0 || queueIdx>=CONFIG_QDMA_QUEUE|| queueIdx < 0){
			QDMA_ERR("invalid input  channelIdx: %d, queueIdx : %d!  \r\n", channelIdx, queueIdx);
			return -EINVAL;
		}else
			dump_queue_dscplist(channelIdx, queueIdx);
	}
	return count;
	
		
}
#endif



/*****************************************************************
*****************************************************************/
static void qdma_bm_task(unsigned long data)			/* bm_task was not used any more,  we just use NAPI mode for RX Receive packets.*/
{
	uint quota = 128 ; 
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint value = 0 ;
	int ringIdx = 0 ;
	struct ECNT_QDMA_Data qdma_data;

	for(ringIdx=0; ringIdx<RX_RING_NUM; ringIdx++) {
		gpQdmaPriv->counters.rxBroadcastCounts[ringIdx] = 0;
		gpQdmaPriv->counters.rxMulticastCounts[ringIdx] = 0;
		gpQdmaPriv->counters.rxUnknownUnicastCounts[ringIdx] = 0;
	}
	qdma_data.qdma_private.cnt = quota;
	if(qdma_receive_packets(&qdma_data) < quota) {
		qdmaEnableInt(base, (INT_MASK_NO_RX0_CPU_DSCP|INT_MASK_RX0_DONE|INT_MASK_NO_RX1_CPU_DSCP|INT_MASK_RX1_DONE)) ;
	} else {
		QDMA_MSG(DBG_WARN, "qdma_bm_task: detected\n");
		/*GDMA2 drop*/
		for(ringIdx=0; ringIdx<RX_RING_NUM; ringIdx++) {
			if (gpQdmaPriv->counters.rxBroadcastCounts[ringIdx] > storm_ctrl_shrehold) {
				/*Disable GMDA2 broadcast drop*/
				value = IO_GREG(FE_GDMA_FWD_CFG);
				value &= ~(0xf00);
				value |= 0x700;
				IO_SREG(FE_GDMA_FWD_CFG, value);
			} else if (gpQdmaPriv->counters.rxMulticastCounts[ringIdx] > storm_ctrl_shrehold) {
				/*Disable GMDA2 multicast drop*/
				value = IO_GREG(FE_GDMA_FWD_CFG);
				value &= ~(0xf0);
				value |= 0x70;
				IO_SREG(FE_GDMA_FWD_CFG, value);			
			} else if (gpQdmaPriv->counters.rxUnknownUnicastCounts[ringIdx] > storm_ctrl_shrehold) {
				/*Disable GMDA2 unknown unicast drop*/
				value = IO_GREG(FE_GDMA_FWD_CFG);
				value &= ~(0xf);
				value |= 0x7;
				IO_SREG(FE_GDMA_FWD_CFG, value);				
			}
		}
		QDMA_STOP_TIMER(gpQdmaPriv->fwdCfg_timer) ;
		
		gpQdmaPriv->fwdCfg_timer.data = qdma_fwd_timer ; /* 100 ms */
		QDMA_START_TIMER(gpQdmaPriv->fwdCfg_timer) ;
		tasklet_schedule(&gpQdmaPriv->task) ;
	}
}

__IMEM ecnt_ret_val ecnt_qdma_hook(struct ecnt_data *in_data)
{
	struct ECNT_QDMA_Data *qdma_data = (struct ECNT_QDMA_Data *)in_data ;
	//ulong flags=0 ;	
	
	if(qdma_data->function_id >= QDMA_FUNCTION_MAX_NUM) {
		QDMA_ERR("qdma_data->function_id is %d, exceed max number: %d", qdma_data->function_id, QDMA_FUNCTION_MAX_NUM);
		return ECNT_HOOK_ERROR;
	}

	qdma_data->retValue = qdma_operation[qdma_data->function_id](qdma_data) ;
	
	return ECNT_CONTINUE;
}

/*****************************************************************
*****************************************************************/
__IMEM static irqreturn_t qdma_isr(int irq, void *dev_id)
{
	uint intStatus=0 ;
	uint base = gpQdmaPriv->csrBaseAddr ;
#ifdef CONFIG_SUPPORT_SELF_TEST
	struct ECNT_QDMA_Data qdma_data;
	QDMA_DMA_DSCP_T *pTxDscp=NULL, *pRxDscp=NULL ;
	uint idx=0 ;
#endif
	
	intStatus = qdmaGetIntStatus(base) & qdmaGetIntMask(base) ;
	QDMA_MSG(DBG_WARN, "QDMA interrupt service routine is return, Status:%.8x, Mask:%.8x\n", qdmaGetIntStatus(base), qdmaGetIntMask(base)) ;
	
	if(intStatus & INT_STATUS_QDMA_DONE) {
		if((intStatus & (INT_STATUS_RX0_DONE | INT_STATUS_RX1_DONE ))) {
			qdmaClearIntStatus(base, INT_STATUS_RX0_DONE|INT_STATUS_RX1_DONE) ;
			if(intStatus & INT_STATUS_RX_PKT_OVERFLOW){
			    QDMA_ERR("ISR: INT_STATUS_RX_PKT_OVERFLOW\n") ;
			    qdmaClearIntStatus(base, INT_STATUS_RX_PKT_OVERFLOW) ;
			}
			if(gpQdmaPriv->devCfg.flags.isRxNapi == QDMA_ENABLE) {
				if(gpQdmaPriv->devCfg.bmEventCallbackFunction) {
					gpQdmaPriv->devCfg.bmEventCallbackFunction(QDMA_EVENT_RECV_PKTS) ;
				}
			}
			else {	/* only use RX NAPI mode */
				qdmaDisableInt(base, (INT_MASK_NO_RX0_CPU_DSCP|INT_MASK_RX0_DONE|INT_MASK_NO_RX1_CPU_DSCP|INT_MASK_RX1_DONE)) ;
				tasklet_schedule(&gpQdmaPriv->task) ;
			}
			if(intStatus & INT_STATUS_RX0_DONE)
				gpQdmaPriv->counters.intRxDone[RING_IDX_0]++ ;
			if(intStatus & INT_STATUS_RX1_DONE)
				gpQdmaPriv->counters.intRxDone[RING_IDX_1]++ ;
			QDMA_MSG(DBG_WARN, "IRQ: RX DSCP DONE interrupt.\n") ;
		}
		
		if(intStatus & INT_STATUS_TX0_DONE) { /**/
			qdmaClearIntStatus(base, INT_STATUS_TX0_DONE) ;
			qdma_bm_transmit_done(0) ;
#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_CT_SWQOS)
			if(sw_PKTQOS_CLEAR_STOP)
				sw_PKTQOS_CLEAR_STOP();
#endif
#endif

			gpQdmaPriv->counters.intTxDone[RING_IDX_0]++ ;
			QDMA_MSG(DBG_WARN, "IRQ: TX DSCP DONE interrupt\n") ;
		}
	}

	if(intStatus & INT_STATUS_EXTERNAL) {
		if(intStatus & INT_STATUS_XPON_PHY) {
			QDMA_MSG(DBG_WARN, "IRQ: External XPON PHY device interrupt\n") ;
			qdmaClearIntStatus(base, INT_STATUS_XPON_PHY) ;
			if(gpQdmaPriv->devCfg.bmXponPhyIntHandler)
				gpQdmaPriv->devCfg.bmXponPhyIntHandler() ;
		} 
		if(intStatus & INT_STATUS_EPON_MAC) {
			QDMA_MSG(DBG_WARN, "IRQ: External EPON MAC interrupt\n") ;
			qdmaClearIntStatus(base, INT_STATUS_EPON_MAC) ;
			if(gpQdmaPriv->devCfg.bmEponMacIntHandler)
				gpQdmaPriv->devCfg.bmEponMacIntHandler() ;
		} 
		if(intStatus & INT_STATUS_GPON_MAC) {
			QDMA_MSG(DBG_WARN, "IRQ: External GPON MAC interrupt\n") ;
			qdmaClearIntStatus(base, INT_STATUS_GPON_MAC) ;
			if(gpQdmaPriv->devCfg.bmGponMacIntHandler)
				gpQdmaPriv->devCfg.bmGponMacIntHandler() ;
		}
	}
	
	if(intStatus & INT_STATUS_QDMA_FAULT) {
		if(intStatus & INT_STATUS_IRQ_FULL) {
			QDMA_ERR("IRQ: INT_STATUS_IRQ_FULL\n") ;
			qdmaClearIntStatus(base, INT_STATUS_IRQ_FULL) ;
#ifdef CONFIG_SUPPORT_SELF_TEST
			qdma_bm_transmit_done(0) ;
#endif
			gpQdmaPriv->counters.intIrqFull++ ;
			QDMA_ERR("IRQ: IRQ full interrupt, entryLen:%d.\n", qdmaGetIrqEntryLen(base)) ;
		}
		
		
		if(intStatus & INT_STATUS_RX0_COHERENT) {
			QDMA_ERR("IRQ: RX0_DMA finds data coherent event when checking DONE bit, RX_DMA_IDX:%d.\n", 
                qdmaGetRxDmaIdx(base, RING_IDX_0)) ;
			qdmaClearIntStatus(base, INT_STATUS_RX0_COHERENT) ;
#ifdef CONFIG_SUPPORT_SELF_TEST
			qdmaDisableRxDma(base) ;

			idx = qdmaGetRxDmaIdx(base, RING_IDX_0) ;
			if(idx>=0 && idx<gpQdmaPriv->rxDscpNum[RING_IDX_0]) {
				pRxDscp = ((struct QDMA_DscpInfo_S *)gpQdmaPriv->rxBaseAddr[RING_IDX_0] + idx)->dscpPtr ;
				pRxDscp->ctrl.done = 0 ;
			}
			qdmaEnableRxDma(base) ;
#endif /* CONFIG_SUPPORT_SELF_TEST */
			gpQdmaPriv->counters.intRxCoherent[RING_IDX_0]++ ;
		}
		
		if(intStatus & INT_STATUS_RX1_COHERENT) {
			QDMA_ERR("IRQ: RX1_DMA finds data coherent event when checking DONE bit, RX_DMA_IDX:%d.\n", 
                qdmaGetRxDmaIdx(base, RING_IDX_1)) ;
			qdmaClearIntStatus(base, INT_STATUS_RX1_COHERENT) ;
#ifdef CONFIG_SUPPORT_SELF_TEST
			qdmaDisableRxDma(base) ;
			idx = qdmaGetRxDmaIdx(base, RING_IDX_1) ;
			if(idx>=0 && idx<gpQdmaPriv->rxDscpNum[RING_IDX_1]) {
				pRxDscp = ((struct QDMA_DscpInfo_S *)gpQdmaPriv->rxBaseAddr[RING_IDX_1] + idx)->dscpPtr ;
				pRxDscp->ctrl.done = 0 ;
			}
			qdmaEnableRxDma(base) ;
#endif /* CONFIG_SUPPORT_SELF_TEST */
			gpQdmaPriv->counters.intRxCoherent[RING_IDX_1]++ ;
		}
		
		if(intStatus & INT_STATUS_TX0_COHERENT) {
			QDMA_ERR("IRQ: TX0_DMA finds data coherent event when checking DONE bit, TX_DMA_IDX:%d.\n", 
                qdmaGetTxDmaIdx(base, RING_IDX_0)) ;            
			qdmaClearIntStatus(base, INT_STATUS_TX0_COHERENT) ;
#ifdef CONFIG_SUPPORT_SELF_TEST
			qdmaDisableTxDma(base) ;
			idx = qdmaGetTxDmaIdx(base, RING_IDX_0) ;
			if(idx>=0 && idx<gpQdmaPriv->txDscpNum[RING_IDX_0]) {
				pTxDscp = ((struct QDMA_DscpInfo_S *)gpQdmaPriv->txBaseAddr[RING_IDX_0] + idx)->dscpPtr ;
				pTxDscp->ctrl.done = 0 ;
			}
			qdmaEnableTxDma(base) ;
#endif /* CONFIG_SUPPORT_SELF_TEST */
			gpQdmaPriv->counters.intTxCoherent[RING_IDX_0]++ ;
		}
		
		if(intStatus & INT_STATUS_TX1_COHERENT) {
			QDMA_ERR("IRQ: TX1_DMA finds data coherent event when checking DONE bit, TX_DMA_IDX:%d.\n", 
                qdmaGetTxDmaIdx(base, RING_IDX_1)) ;
			qdmaClearIntStatus(base, INT_STATUS_TX1_COHERENT) ;
#ifdef CONFIG_SUPPORT_SELF_TEST
			qdmaDisableTxDma(base) ;
			idx = qdmaGetTxDmaIdx(base, RING_IDX_1) ;
			if(idx>=0 && idx<gpQdmaPriv->txDscpNum[RING_IDX_1]) {
				pTxDscp = ((struct QDMA_DscpInfo_S *)gpQdmaPriv->txBaseAddr[RING_IDX_1] + idx)->dscpPtr ;
				pTxDscp->ctrl.done = 0 ;
			}
			qdmaEnableTxDma(base) ;
#endif /* CONFIG_SUPPORT_SELF_TEST */
			gpQdmaPriv->counters.intTxCoherent[RING_IDX_1]++ ;
		}
		
		if(intStatus & INT_STATUS_HWFWD_DSCP_EMPTY) {
			QDMA_ERR("IRQ: NO HW Link manger descriptor available.\n") ;
			qdmaClearIntStatus(base, INT_STATUS_HWFWD_DSCP_EMPTY) ;
			if(gpQdmaPriv->devCfg.bmEventCallbackFunction) {
				gpQdmaPriv->devCfg.bmEventCallbackFunction(QDMA_EVENT_TX_CROWDED) ;
			}
#ifdef CONFIG_SUPPORT_SELF_TEST
			qdma_data.qdma_private.cnt = 0 ;
			printk("receive %d \n", qdma_receive_packets(&qdma_data)) ; /*  for csr2 test */
#endif
			gpQdmaPriv->counters.intNoLinkDscp++ ;
		}
		
		if(intStatus & INT_STATUS_HWFWD_DSCP_LOW) {
			QDMA_ERR("IRQ: Low HW Link manger descriptor available.\n") ;
			qdmaClearIntStatus(base, INT_STATUS_HWFWD_DSCP_LOW) ;
			if(gpQdmaPriv->devCfg.bmEventCallbackFunction) {
				gpQdmaPriv->devCfg.bmEventCallbackFunction(QDMA_EVENT_TX_CROWDED) ;
			}
#ifdef CONFIG_SUPPORT_SELF_TEST
			qdma_data.qdma_private.cnt = 0 ;
			printk("receive %d \n", qdma_receive_packets(&qdma_data)) ; /*  for csr2 test */
#endif /* CONFIG_SUPPORT_SELF_TEST */
			gpQdmaPriv->counters.intLowLinkDscp++ ;
		}

		if((intStatus & INT_STATUS_NO_RX0_CPU_DSCP) || (intStatus & INT_STATUS_NO_RX1_CPU_DSCP)) {
			QDMA_MSG(DBG_WARN, "IRQ: NO RX CPU descriptor available interrupt.\n") ;
			qdmaClearIntStatus(base, INT_STATUS_NO_RX0_CPU_DSCP|INT_STATUS_NO_RX1_CPU_DSCP) ;
			if((gpQdmaPriv->rxStartPtr[0] == gpQdmaPriv->rxEndPtr[0]) || (gpQdmaPriv->rxStartPtr[1] == gpQdmaPriv->rxEndPtr[1])) {
				/* need inform to high layer driver */ ;
				if(gpQdmaPriv->devCfg.bmEventCallbackFunction) {
					gpQdmaPriv->devCfg.bmEventCallbackFunction(QDMA_EVENT_NO_RX_BUFFER) ;
				} 
			} 
			if(intStatus & INT_STATUS_NO_RX0_CPU_DSCP)
				gpQdmaPriv->counters.intNoRxDscp[0]++ ;
			if(intStatus & INT_STATUS_NO_RX1_CPU_DSCP)
				gpQdmaPriv->counters.intNoRxDscp[1]++ ;
		}
		
		if((intStatus & INT_STATUS_NO_TX0_CPU_DSCP) || (intStatus & INT_STATUS_NO_TX1_CPU_DSCP)) {
			qdmaClearIntStatus(base, INT_STATUS_NO_TX0_CPU_DSCP|INT_STATUS_NO_TX1_CPU_DSCP) ;
			if(intStatus & INT_STATUS_NO_TX0_CPU_DSCP){
				gpQdmaPriv->counters.intNoTxDscp[0]++ ;
				QDMA_MSG(DBG_MSG,"IRQ: NO TX0 CPU descriptor available interrupt.\n") ;
			}
			if(intStatus & INT_STATUS_NO_TX1_CPU_DSCP){
				QDMA_MSG(DBG_MSG,"IRQ: NO TX1 CPU descriptor available interrupt.\n") ;
				gpQdmaPriv->counters.intNoTxDscp[1]++ ;
			}
		}
	}
	
	return IRQ_HANDLED ;
}

/******************************************************************************
******************************************************************************/
#if defined(TCSUPPORT_SLM_EN) && !defined(QDMA_LAN)
int slm_dev_init(uint virAddr, uint virSize, uint phySize)  
{
	int is_idle_flag=0;
	volatile uint slm_cfg = 0;
	dma_addr_t phyDmaAddr=0;
	ulong curTime = jiffies;
	
	/*  wait for SLM is disable or SLM is idle */
	while(time_in_range(jiffies, curTime, curTime+1)) {			/* wait about 10~20 ms */
		slm_cfg = regRead32(SLM_EN);
		if((slm_cfg & 0x3) != 3) {
			is_idle_flag=1;
			break;
		}
	}
	if(is_idle_flag == 0) {
		QDMA_ERR("SLM is busy!\n") ;
	}
	
	/*  disable SLM */
	regWrite32(SLM_EN, 0);
	
	gpQdmaPriv->slmPhyAddr = (uint)dma_alloc_coherent(NULL, phySize, &phyDmaAddr, GFP_KERNEL);
	if(!gpQdmaPriv->slmPhyAddr) {
		QDMA_ERR("Alloc memory for slm physical address failed!\n") ;
		return -1 ;
	}
	gpQdmaPriv->slmPhySize = phySize;
	regWrite32(SLM_VIR_SIZE, virSize/SLM_SECTOR_SIZE) ;
	regWrite32(SLM_PHY_SIZE, phySize/SLM_SECTOR_SIZE) ;
	regWrite32(SLM_VIR_BASE, virAddr) ;
	regWrite32(SLM_PHY_BASE, phyDmaAddr) ;

	/*  enable  SLM */
	regWrite32(SLM_EN, 1);
	return 0;	
}

int slm_dev_exit(void) 
{	
	int is_idle_flag=0;
	volatile uint slm_cfg = 0;
	ulong curTime = jiffies;
	
	/*  disable SLM */
	while(time_in_range(jiffies, curTime, curTime+1)) {			/* wait about 10~20 ms */
		slm_cfg = regRead32(SLM_EN);
		if((slm_cfg & 0x3) != 3) {
			is_idle_flag=1;
			break;
		}
	}
	if(is_idle_flag == 0) {
		QDMA_ERR("SLM is busy!\n") ;
	}
	
	regWrite32(SLM_EN, 0);
	if(gpQdmaPriv->slmPhyAddr) {
		dma_free_coherent(NULL, gpQdmaPriv->slmPhySize, (void *)gpQdmaPriv->slmPhyAddr, regRead32(SLM_PHY_BASE)) ;
		gpQdmaPriv->slmPhySize = 0;
		gpQdmaPriv->slmPhyAddr = 0;
	}
	
	regWrite32(SLM_VIR_SIZE, 0) ;
	regWrite32(SLM_PHY_SIZE, 0) ;
	regWrite32(SLM_VIR_BASE, 0) ;
	regWrite32(SLM_PHY_BASE, 0) ;

	return 0;	
}
#endif

struct ecnt_hook_ops ecnt_driver_qdma_op = {
	.name = driver_qdma_hook_name,
	.hookfn = ecnt_qdma_hook,
	.maintype = ECNT_QDMA,
	.is_execute = 1,
	.subtype = ECNT_DRIVER_API,
	.priority = 1
};

static int qdma_bm_dscp_init(QDMA_Init_T qdmaInit)
{
	struct QDMA_DscpInfo_S *diPtr=NULL ;
	dma_addr_t dscpDmaAddr=0, irqDmaAddr=0, hwFwdDmaAddr=0 ;
	dma_addr_t hwFwdBuffAddr=0 ;
	uint dscpBaseAddr=0, hwTotalDscpSize=0, hwTotalPktSize=0 ;
	uint i=0, base=0;
	int ringIdx=0 ;
	uint txDscpNum[TX_RING_NUM] ;
	uint rxDscpNum[RX_RING_NUM] ;
	uint hwDscpNum=0 ;
	uint irqDepth=0 ;
	uint hwFwdPktLen=0 ;
	uint memsize=0 ;
	int flag=0, cnt=0;

	base = gpQdmaPriv->csrBaseAddr ;
	txDscpNum[1] = qdmaInit.txDscpNum[1] ;
	txDscpNum[0] = qdmaInit.txDscpNum[0] ;
	rxDscpNum[1] = qdmaInit.rxDscpNum[1] ;
	rxDscpNum[0] = qdmaInit.rxDscpNum[0] ;
	hwDscpNum = qdmaInit.hwDscpNum ;
	irqDepth = qdmaInit.irqDepth ;
	hwFwdPktLen = qdmaInit.hwFwdPktLen ;	
	
	if(txDscpNum[1]>CONFIG_DSCP_NUM_MAX || txDscpNum[0]>CONFIG_DSCP_NUM_MAX || rxDscpNum[1]>CONFIG_DSCP_NUM_MAX || rxDscpNum[0]>CONFIG_DSCP_NUM_MAX 
		|| hwDscpNum>CONFIG_DSCP_NUM_MAX || irqDepth>CONFIG_IRQ_NUM_MAX || (hwFwdPktLen>CONFIG_MAX_PKT_LENS || hwFwdPktLen<48))
	{
		QDMA_ERR("The initial parameters are invalid.\n") ;
		return -EFAULT ;
	}
	if((hwFwdPktLen & 0x3) != 0) {
		QDMA_ERR("The max packets lens must be a multiple of 4.\n") ;
		return -EFAULT ;
	}

	memcpy(&gpQdmaPriv->txDscpNum[0], &qdmaInit.txDscpNum[0], sizeof(QDMA_Init_T));
	
	/******************************************
	* Allocate descriptor DMA memory          *
	*******************************************/
	dscpBaseAddr = (uint)dma_alloc_coherent(NULL, sizeof(QDMA_DMA_DSCP_T)*(txDscpNum[1]+txDscpNum[0]+rxDscpNum[1]+rxDscpNum[0]), &dscpDmaAddr, GFP_KERNEL) ;
	if(!dscpBaseAddr) {
		QDMA_ERR("Allocate memory for TX/RX DSCP failed.\n") ; 
		return -ENOMEM ;
	}

	/* Set the TX_DSCP_BASE and RX_DSCP_BASE address */
	qdmaSetTxDscpBase(base, RING_IDX_1, dscpDmaAddr) ;
	qdmaSetTxDscpBase(base, RING_IDX_0, (dscpDmaAddr + sizeof(QDMA_DMA_DSCP_T)*txDscpNum[1])) ;
	qdmaSetRxDscpBase(base, RING_IDX_1, (dscpDmaAddr + sizeof(QDMA_DMA_DSCP_T)*(txDscpNum[1]+txDscpNum[0]))) ;
	qdmaSetRxDscpBase(base, RING_IDX_0, (dscpDmaAddr + sizeof(QDMA_DMA_DSCP_T)*(txDscpNum[1]+txDscpNum[0]+rxDscpNum[1]))) ;	

	qdmaSetRxRingSize(base, RING_IDX_1, rxDscpNum[RING_IDX_1]);
	qdmaSetRxRingSize(base, RING_IDX_0, rxDscpNum[RING_IDX_0]);
#if defined(QDMA_LAN)
	qdmaSetRxRingThrh(base, RING_IDX_1, 32) ;
	qdmaSetPktSizeOverHeadVal(base, 0x14);
#else
	qdmaSetRxRingThrh(base, RING_IDX_1, rxDscpNum[RING_IDX_1]>>2) ;
#endif
	qdmaSetRxRingThrh(base, RING_IDX_0, rxDscpNum[RING_IDX_0]>>2) ;

	/******************************************
	* Allocate memory for IRQ queue           *
	******************************************/
	if(irqDepth) {
		gpQdmaPriv->irqQueueAddr = (uint)dma_alloc_coherent(NULL, irqDepth<<2, &irqDmaAddr, GFP_KERNEL) ;
		if(!gpQdmaPriv->irqQueueAddr) {
			QDMA_ERR("Allocate memory for IRQ queue failed.\n") ;
			return -ENOMEM ;
		}
		memset((void *)gpQdmaPriv->irqQueueAddr, CONFIG_IRQ_DEF_VALUE, irqDepth<<2) ;
	
		/* Setting the IRQ queue information to QDMA register */
		qdmaSetIrqBase(base, irqDmaAddr) ;
		qdmaSetIrqDepth(base, irqDepth) ;
		qdmaSetIrqThreshold(base, QDMA_TX_THRESHOLD) ;
	}

	/***************************************************
	* Allocate memory for TX/RX DSCP Information node  *
	****************************************************/
	gpQdmaPriv->dscpInfoAddr = (uint)kzalloc(sizeof(struct QDMA_DscpInfo_S)*(txDscpNum[1]+txDscpNum[0]+rxDscpNum[1]+rxDscpNum[0]), GFP_KERNEL) ;
	if(!gpQdmaPriv->dscpInfoAddr) {
		QDMA_ERR("Alloc memory for TX/RX DSCP information node failed\n") ;
		return -ENOMEM ;
	}
	gpQdmaPriv->txBaseAddr[1] = gpQdmaPriv->dscpInfoAddr;
	gpQdmaPriv->txBaseAddr[0] = gpQdmaPriv->dscpInfoAddr + sizeof(struct QDMA_DscpInfo_S)*txDscpNum[1];
	gpQdmaPriv->rxBaseAddr[1] = gpQdmaPriv->dscpInfoAddr + sizeof(struct QDMA_DscpInfo_S)*(txDscpNum[1]+txDscpNum[0]);
	gpQdmaPriv->rxBaseAddr[0] = gpQdmaPriv->dscpInfoAddr + sizeof(struct QDMA_DscpInfo_S)*(txDscpNum[1]+txDscpNum[0]+rxDscpNum[1]);
	
	/* Create unused tx descriptor link list and using rx descriptor ring */
	for(i=0 ; i<(txDscpNum[1]+txDscpNum[0]+rxDscpNum[1]+rxDscpNum[0]) ; i++)
	{
		diPtr = (struct QDMA_DscpInfo_S *)gpQdmaPriv->dscpInfoAddr + i ;
		diPtr->dscpPtr = (QDMA_DMA_DSCP_T *)dscpBaseAddr + i ;
		
		if(i < txDscpNum[1]) {
			diPtr->dscpIdx = i ;
			diPtr->next = NULL ;
			diPtr->dscpPtr->ctrl.done = 1 ;
			qdma_bm_push_tx_dscp(diPtr, 1) ;
		} else if(i < txDscpNum[1] + txDscpNum[0]) {
			diPtr->dscpIdx = i - txDscpNum[1] ;
			diPtr->next = NULL ;
			diPtr->dscpPtr->ctrl.done = 1 ;
			qdma_bm_push_tx_dscp(diPtr, 0) ;
		} else if(i < txDscpNum[1] + txDscpNum[0] + rxDscpNum[1]) {
			diPtr->dscpIdx = i - txDscpNum[1] - txDscpNum[0] ;
			qdma_bm_add_rx_dscp(diPtr, 1) ;
		} else {
			diPtr->dscpIdx = i - txDscpNum[1] - txDscpNum[0] - rxDscpNum[1] ;
			qdma_bm_add_rx_dscp(diPtr, 0) ;
		}
	}	

	/***************************************************
	* Initialization first DSCP for Tx1 DMA              *
	****************************************************/
	diPtr = qdma_bm_pop_tx_dscp(RING_IDX_1) ;
	if(!diPtr) {
		QDMA_ERR("There is not any free TX1 DSCP.\n") ; 
		return -ENOSR ;
	}
	gpQdmaPriv->txUsingPtr[RING_IDX_1] = diPtr ;
	qdmaSetTxCpuIdx(base, RING_IDX_1, diPtr->dscpIdx) ;
	qdmaSetTxDmaIdx(base, RING_IDX_1, diPtr->dscpIdx) ;	
	
	/***************************************************
	* Initialization first DSCP for Rx1 DMA              *
	****************************************************/
	diPtr = qdma_bm_get_unused_rx_dscp(RING_IDX_1) ;
	if(diPtr == NULL) {
		QDMA_ERR("There is not any free RX1 DSCP.\n") ;
		return -ENOSR ;
	} 
	gpQdmaPriv->rxUsingPtr[RING_IDX_1] = diPtr ;
	qdmaSetRxCpuIdx(base, RING_IDX_1, diPtr->dscpIdx) ;
	qdmaSetRxDmaIdx(base, RING_IDX_1, diPtr->dscpIdx) ;	

	/***************************************************
	* Initialization first DSCP for Tx0 DMA              *
	****************************************************/
	diPtr = qdma_bm_pop_tx_dscp(RING_IDX_0) ;
	if(!diPtr) {
		QDMA_ERR("There is not any free TX DSCP.\n") ; 
		return -ENOSR ;
	}
	gpQdmaPriv->txUsingPtr[RING_IDX_0] = diPtr ;
	qdmaSetTxCpuIdx(base, RING_IDX_0, diPtr->dscpIdx) ;
	qdmaSetTxDmaIdx(base, RING_IDX_0, diPtr->dscpIdx) ;
	
	/***************************************************
	* Initialization first DSCP for Rx0 DMA              *
	****************************************************/
	diPtr = qdma_bm_get_unused_rx_dscp(RING_IDX_0) ;
	if(diPtr == NULL) {
		QDMA_ERR("There is not any free RX DSCP.\n") ;
		return -ENOSR ;
	} 
	gpQdmaPriv->rxUsingPtr[RING_IDX_0] = diPtr ;
	qdmaSetRxCpuIdx(base, RING_IDX_0, diPtr->dscpIdx) ;
	qdmaSetRxDmaIdx(base, RING_IDX_0, diPtr->dscpIdx) ;
	
	/***************************************************
	* Initialization packets for Rx DMA              *
	****************************************************/
	for(ringIdx=0; ringIdx<RX_RING_NUM; ringIdx++) {
		do {
			if(qdma_prepare_rx_buffer(ringIdx) != 0)
			{
				QDMA_ERR("Attention: qdma init rx buffer undone, please double check! \n");
				break ;
			}
		} while(qdma_has_free_rxdscp(ringIdx)) ;
	}
	
	/***************************************************
	* Initialization DSCP for hardware forwarding      *
	****************************************************/
	
	if(hwDscpNum) { 
        hwTotalDscpSize = sizeof(QDMA_HWFWD_DMA_DSCP_T) * hwDscpNum ;
		hwTotalPktSize = hwFwdPktLen * hwDscpNum ;		
#ifdef QDMA_LAN
        physical_size = hwTotalPktSize;
#else
		if((physical_size <=0) || (physical_size > HWFWD_PHYSICAL_SIZE_MAX)) {
			physical_size = HWFWD_PHYSICAL_SIZE_MAX;
			QDMA_ERR("HW_FWD physical size is not illegal, force setting to 4MB\n");
		}
#endif
		printk("%s: physical_size is 0x%08x\n", qdma_path, physical_size);

		gpQdmaPriv->hwFwdPayloadSize = hwFwdPktLen;

		gpQdmaPriv->hwFwdBaseAddr = (uint)dma_alloc_coherent(NULL, hwTotalDscpSize, &hwFwdDmaAddr, GFP_KERNEL) ;
		if(!gpQdmaPriv->hwFwdBaseAddr) {
			QDMA_ERR("Allocate Descriptor for hardware forwarding failed.\n") ;
			return -ENOMEM ;
		}

#if defined(QDMA_LAN)
		gpQdmaPriv->hwFwdBuffAddr = (uint)dma_alloc_coherent(NULL, hwTotalPktSize, &hwFwdBuffAddr, GFP_KERNEL) ;
		if(!gpQdmaPriv->hwFwdBuffAddr) {
			QDMA_ERR("Allocate Buffer for hardware forwarding failed.\n") ;
			return -ENOMEM ;
		}
#elif defined(TCSUPPORT_SLM_EN)		/* QDMA_WAN & SLM Enable*/
		hwFwdBuffAddr = 0x1b800000; /* 440M ~ 448M */
		if(GET_DRAM_SIZE<=256){	/* means 512M DRAM do not need alloc */
			if(slm_dev_init(hwFwdBuffAddr, hwTotalPktSize, physical_size) == -1) {
				QDMA_ERR("slm device init failed.\n") ;
				return -ENOMEM;
			}
		}
		gpQdmaPriv->hwFwdBuffAddr = (uint)(ioremap_nocache(hwFwdBuffAddr, hwTotalPktSize));
#else	/* QDMA_WAN & SLM Disable*/
		memsize = GET_DRAM_SIZE;
		if(memsize == 512)
			memsize = 440; /* always reserve 8M to QDMA HW when memsize if 512MByte */
		else
			memsize = memsize-8; /* without SLM, we should reserve 8M to QDMA HW */
		hwFwdBuffAddr = memsize << 20;
		gpQdmaPriv->hwFwdBuffAddr = (uint)(ioremap_nocache(hwFwdBuffAddr, hwTotalPktSize));
#endif

		qdmaSetHwDscpBase(base, hwFwdDmaAddr) ;
		qdmaSetHwBuffBase(base, hwFwdBuffAddr) ;
		qdmaSetHwDscpNum(base, hwDscpNum);

		qdmaSetHwPayloadSize(base, HWFWD_PAYLOAD_SIZE_2K);
		qdmaSetHwLowThrshld(base, HWFWD_LOW_THRESHOLD);
		
		qdmaSetHWInitStart(base);
		
		flag = 0 ;

		cnt = 25;
		while((cnt--) > 0) {		
			mdelay(1);
			if(qdmaGetHWInitStart(base) == 0) {
				flag=1;
				break;
			}
			
		}

		if(flag == 0) {
			QDMA_ERR("hw_fwd init fail!\n") ;
			return -1;
		}
	}
	
	return 0 ;
}

/******************************************************************************
******************************************************************************/
static int qdma_bm_dscp_deinit(void) 
{
	struct QDMA_DscpInfo_S *diPtr=NULL ;
	uint totalDscpNum=0, base=0 ;
	int i=0, ringIdx=0 ;

	base = gpQdmaPriv->csrBaseAddr ;
	totalDscpNum = gpQdmaPriv->txDscpNum[0] + gpQdmaPriv->txDscpNum[1] + gpQdmaPriv->rxDscpNum[0] + gpQdmaPriv->rxDscpNum[1] ;
	
	qdmaDisableTxDma(base) ;
	qdmaDisableRxDma(base) ;

	QDMA_MSG(DBG_MSG, "qdma_transmit_done\n");
	qdma_bm_transmit_done(gpQdmaPriv->irqDepth) ;

	QDMA_MSG(DBG_MSG, "qdma_recycle_packet_mode\n");
	qdma_recycle_packet_mode(QDMA_TX_INTERRUPT, 0);
	
	QDMA_MSG(DBG_MSG, "qdma_recycle_receive_buffer\n");
	for(ringIdx=0; ringIdx<RX_RING_NUM; ringIdx++)
		qdma_bm_recycle_receive_buffer(ringIdx);

	QDMA_MSG(DBG_MSG, "qdma_recycle_transmit_buffer\n");
	for(ringIdx=0; ringIdx<TX_RING_NUM; ringIdx++)
		qdma_bm_recycle_transmit_buffer(ringIdx);

	if(gpQdmaPriv->dscpInfoAddr) {
		for(i=0 ; i<totalDscpNum ; i++) {
			diPtr = (struct QDMA_DscpInfo_S *)gpQdmaPriv->dscpInfoAddr + i ;
			
			if(diPtr->skb && diPtr->dscpPtr->pkt_addr) {
				dma_unmap_single(NULL, diPtr->dscpPtr->pkt_addr, (size_t)diPtr->dscpPtr->ctrl.pkt_len, DMA_BIDIRECTIONAL) ;
			}
		}

		diPtr = (struct QDMA_DscpInfo_S *)gpQdmaPriv->dscpInfoAddr ;
		if(diPtr->dscpPtr) {
			dma_free_coherent(NULL, sizeof(QDMA_DMA_DSCP_T)*totalDscpNum, (void *)diPtr->dscpPtr, qdmaGetTxDscpBase(base, RING_IDX_1)) ;
			diPtr->dscpPtr = NULL ;
		}
	
		kfree(diPtr) ;
		gpQdmaPriv->dscpInfoAddr = 0 ;
	}
	
	if(gpQdmaPriv->irqQueueAddr) {
		dma_free_coherent(NULL, gpQdmaPriv->irqDepth<<2, (void *)gpQdmaPriv->irqQueueAddr, qdmaGetIrqBase(base)) ;
		gpQdmaPriv->irqQueueAddr = 0 ;
	}
	
	if(gpQdmaPriv->hwFwdBaseAddr) {
		uint maxSize = sizeof(QDMA_HWFWD_DMA_DSCP_T) * qdmaGetHwDscpNum(base) ;
		
		dma_free_coherent(NULL, maxSize, (void *)gpQdmaPriv->hwFwdBaseAddr, qdmaGetHwDscpBase(base)) ;
		gpQdmaPriv->hwFwdBaseAddr = 0 ;
	}
	
#if defined(QDMA_LAN)
	if(gpQdmaPriv->hwFwdBuffAddr) {
		uint maxSize = gpQdmaPriv->hwFwdPayloadSize * qdmaGetHwDscpNum(base) ;
		
		dma_free_coherent(NULL, maxSize, (void *)gpQdmaPriv->hwFwdBuffAddr, qdmaGetHwBuffBase(base)) ;
		gpQdmaPriv->hwFwdBuffAddr = 0 ;
	}
#elif defined(TCSUPPORT_SLM_EN)		/* QDMA_WAN & SLM Enable*/	
	if(GET_DRAM_SIZE<=256){/*means 512M DRAM do not need free*/
		slm_dev_exit();
	}
	iounmap((uint *)gpQdmaPriv->hwFwdBuffAddr) ;
	gpQdmaPriv->hwFwdBuffAddr = 0 ;
#else	/* QDMA_WAN & SLM Disable*/
	iounmap((uint *)gpQdmaPriv->hwFwdBuffAddr) ;
	gpQdmaPriv->hwFwdBuffAddr = 0 ;
#endif

	return 0 ;
}

/******************************************************************************
******************************************************************************/
static void __exit qdma_module_cleanup(void) 
{   
	qdma_dvt_deinit() ;
	
#ifndef QDMA_LAN
    #if defined(QOS_REMARKING) || defined(TCSUPPORT_HW_QOS)
	remove_proc_entry(mac_qos_path, NULL);
	#endif
	
#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_XPON_HAL_API_QOS_CA) || defined(TCSUPPORT_CT_PON)
	remove_proc_entry(mac_default_mask_path, NULL);
#endif

	#ifdef TCSUPPORT_QOS
	remove_proc_entry(mac_qos_flag_path, NULL);
	#endif
	
	remove_proc_entry(mac_resource_limit_path, NULL) ;
#endif
	
	remove_proc_entry(counters_path, NULL) ;
	remove_proc_entry(debug_path, NULL) ;
	remove_proc_entry(ratelimit_path, NULL) ;
	remove_proc_entry(qos_path, NULL) ;
#if 0
	remove_proc_entry(memxs_path, NULL) ;
#endif
	remove_proc_entry(qdma_path, NULL) ;
	
	ecnt_unregister_hook(&ecnt_driver_qdma_op);
	
	QDMA_STOP_TIMER(gpQdmaPriv->fwdCfg_timer) ;
	
	if(gpQdmaPriv->devCfg.flags.isIsrRequest) {
		free_irq(CONFIG_QDMA_IRQ, NULL) ;
	}
	
#ifdef TCSUPPORT_DRIVER_API
	qdma_dev_destroy();
#endif
	
	/* Initial the QDMA tasklet */
	if(QDMA_RX_MODE == QDMA_RX_INTERRUPT) {		/* not use RX interrupt mode, only use RX NAPI mode */
		tasklet_kill(&gpQdmaPriv->task) ;
	}

	qdma_bm_dscp_deinit() ;

	if(gpQdmaPriv->csrBaseAddr) {
		iounmap((uint *)gpQdmaPriv->csrBaseAddr) ;
		gpQdmaPriv->csrBaseAddr = 0 ;
	}

	kfree(gpQdmaPriv) ;
	gpQdmaPriv = NULL ;

	return;
}
void qdma_fwd_cfg_timer_expires(unsigned long arg)
{
	uint regFoeGlbCfg = 0;
	FE_Gdma_Sel_t gdm_sel;

	#if defined(QDMA_LAN)
	gdm_sel = FE_GDM_SEL_GDMA1;
	#else
	gdm_sel = FE_GDM_SEL_GDMA2;
	#endif
	
	regFoeGlbCfg = IO_GREG(0xBFB50E00);
	
	if (regFoeGlbCfg & 0x01){
		FE_API_SET_FW_CFG(gdm_sel, FE_TYPE_OC, FE_DP_PPE);
		FE_API_SET_FW_CFG(gdm_sel, FE_TYPE_MC, FE_DP_PPE);
		FE_API_SET_FW_CFG(gdm_sel, FE_TYPE_BC, FE_DP_PPE);
	}
	else{
		FE_API_SET_FW_CFG(gdm_sel, FE_TYPE_OC, FE_DP_QDMA2_CPU);
		FE_API_SET_FW_CFG(gdm_sel, FE_TYPE_MC, FE_DP_QDMA2_CPU);
		FE_API_SET_FW_CFG(gdm_sel, FE_TYPE_BC, FE_DP_QDMA2_CPU);
	}
}

/******************************************************************************
******************************************************************************/

static int __init qdma_module_init(void)
{
	int idx=0, ret=0 ;
	struct proc_dir_entry *qdma_proc=NULL;
	QDMA_Init_T qdmaInit ;
	uint _reg=0;
	struct ECNT_QDMA_Data qdma_data;
	
	printk("macType = %u\n", macType);
	if(macType == MAC_TYPE_ATM) {
		printk("mac type is MAC_TYPE_ATM\n");
                  _reg  = IO_GREG(0xbfb00070); 
                  _reg &= ~(0x7);
                  _reg  |= 0x3;
		IO_SREG(0xbfb00070, _reg);
                  
	} else if(macType == MAC_TYPE_PTM) {
		printk("mac type is MAC_TYPE_PTM\n");
		_reg  = IO_GREG(0xbfb00070); 
                  _reg &= ~(0x7);
                  _reg  |= 0x2;
		IO_SREG(0xbfb00070, _reg);
	}
	
	/* Initial device private data */
	gpQdmaPriv = (QDMA_Private_T *)kzalloc(sizeof(QDMA_Private_T), GFP_KERNEL) ;
	if(gpQdmaPriv == NULL) {
		QDMA_ERR("Alloc private data memory failed\n") ;
		return -ENOMEM ;
	} 

	gpQdmaPriv->devCfg.txRecycleThreshold = QDMA_TX_THRESHOLD ;
#ifdef CONFIG_QDMA_CSR_OFFSET
	gpQdmaPriv->devCfg.txQueueTrtcmScale = QDMA_TRTCM_SCALE_128BYTE ;
	gpQdmaPriv->devCfg.gponTrtcmScale = QDMA_TRTCM_SCALE_128BYTE ;
#endif /* CONFIG_QDMA_CSR_OFFSET */
#ifdef CONFIG_DEBUG
	gpQdmaPriv->devCfg.dbgLevel = DBG_ERR ;
#endif /* CONFIG_DEBUG */
#ifdef CONFIG_SUPPORT_SELF_TEST
	gpQdmaPriv->devCfg.waitTxMaxNums = 1 ;
	gpQdmaPriv->devCfg.countDown = 1 ;
	gpQdmaPriv->devCfg.countDownRound = 0 ;
#endif /* CONFIG_SUPPORT_SELF_TEST */
	for(idx=0; idx<TX_RING_NUM; idx++)
		spin_lock_init(&gpQdmaPriv->txLock[idx]) ;

	spin_lock_init(&gpQdmaPriv->irqLock) ;
	spin_lock_init(&gpQdmaPriv->txDoneLock) ;
	
	/* Base Register remap of QDMA */
	gpQdmaPriv->csrBaseAddr = (uint)(ioremap_nocache(CONFIG_QDMA_BASE_ADDR, 0x400)) ; 
	if(!gpQdmaPriv->csrBaseAddr) {
		QDMA_ERR("ioremap the QDMA base address failed.\n") ;
		return -EFAULT ;
	}
	
	/* Initial for design and verification function */
	hwfwd_payload_max_size = CONFIG_MAX_PKT_LENS;
	qdmaInit.txDscpNum[1] = CONFIG_TX1_DSCP_NUM;
	qdmaInit.txDscpNum[0] = CONFIG_TX0_DSCP_NUM;
	qdmaInit.rxDscpNum[1] = CONFIG_RX1_DSCP_NUM;
	qdmaInit.rxDscpNum[0] = CONFIG_RX0_DSCP_NUM;
	qdmaInit.hwDscpNum = CONFIG_HWFWD_DSCP_NUM;
	qdmaInit.irqDepth = CONFIG_IRQ_DEPTH;
	qdmaInit.hwFwdPktLen = hwfwd_payload_max_size;
	if((ret = qdma_bm_dscp_init(qdmaInit)) != 0) 
	{
		QDMA_ERR("QDMA DSCP initialization failed.\n") ;
		return ret ;
	}
	
	/* Initial the QDMA tasklet */
	if(QDMA_RX_MODE == QDMA_RX_INTERRUPT) {		/* not use RX interrupt mode, only use RX NAPI mode */
		tasklet_init(&gpQdmaPriv->task, qdma_bm_task, 0) ;
	}
	
	/***************************************************
	* QDMA device initialization                       *
	****************************************************/
	if((ret = qdma_dev_init()) != 0) {
		QDMA_ERR("QDMA hardware device initialization failed.\n") ;
		return ret ;
	}

 	/* Register QDMA interrupt */
	if(request_irq(CONFIG_QDMA_IRQ, qdma_isr, 0, isr_name, NULL) != 0) {
		QDMA_ERR("Request the interrupt service routine fail, irq:%d.\n", CONFIG_QDMA_IRQ) ;
		return -ENODEV ;
	}
	gpQdmaPriv->devCfg.flags.isIsrRequest = 1 ;

	init_timer(&gpQdmaPriv->fwdCfg_timer) ;
	gpQdmaPriv->fwdCfg_timer.data = qdma_fwd_timer ; /* 100 ms */
	gpQdmaPriv->fwdCfg_timer.function = qdma_fwd_cfg_timer_expires ;

	/***************************************************
	* Initialization TX Mode and RX Mode              *
	****************************************************/
	qdma_receive_packet_mode(QDMA_RX_MODE);
	qdma_recycle_packet_mode(QDMA_TX_MODE, QDMA_TX_THRESHOLD);

	/*  enable QDMA Rx Drop  */
	qdma_data.qdma_private.mode = QDMA_ENABLE;
	qdma_set_rx_protect_en(&qdma_data);
	
	/*  enable QDMA Tx Drop  */
	qdma_data.qdma_private.mode = QDMA_ENABLE;
	qdma_set_tx_drop_en(&qdma_data);

	/*  enable QDMA Tx DEI Drop  */
	qdma_data.qdma_private.mode = QDMA_ENABLE;
	qdma_set_txq_dei_drop_mode(&qdma_data);

	if(ecnt_register_hook(&ecnt_driver_qdma_op)) {
		QDMA_ERR("ecnt_dev_qdma_op register fail\n");
		return -ENODEV ;
	}
	
	/* Initial proc file node */
    proc_mkdir(qdma_path, NULL);
	qdma_proc = create_proc_entry(counters_path, 0, NULL) ;
	if(qdma_proc) {
		qdma_proc->read_proc = qdma_bm_counters_read_proc ;
		qdma_proc->write_proc = qdma_bm_counters_write_proc ;
	}
	qdma_proc = create_proc_entry(debug_path, 0, NULL) ;
	if(qdma_proc) {
		qdma_proc->read_proc = qdma_bm_debug_read_proc ;
		qdma_proc->write_proc = qdma_bm_debug_write_proc ;
	}
	qdma_proc = create_proc_entry(ratelimit_path, 0, NULL) ;
	if(qdma_proc) {
		qdma_proc->read_proc = qdma_bm_ratelimit_read_proc ;
		qdma_proc->write_proc = qdma_bm_ratelimit_write_proc ;
	}
#if defined(TCSUPPORT_QDMA_VERIFY) && defined(QDMA_LAN)
	qdma_proc = create_proc_entry(qdma_verify_path, 0, NULL) ;
	if(qdma_proc) {
		qdma_proc->read_proc = qdma_bm_verify_read_proc ;
		qdma_proc->write_proc = qdma_bm_verify_write_proc ;
	}
#endif
	qdma_proc = create_proc_entry(qos_path, 0, NULL) ;
	if(qdma_proc) {
		qdma_proc->read_proc = qdma_bm_qos_read_proc ;
		qdma_proc->write_proc = qdma_bm_qos_write_proc ;
	}

#if 0
	qdma_proc = create_proc_entry(memxs_path, 0, NULL) ;
	if(qdma_proc) {
		qdma_proc->read_proc = qdma_bm_memxs_read_proc ;
		qdma_proc->write_proc = qdma_bm_memxs_write_proc ;
	}
#endif
	
#ifndef QDMA_LAN
#if defined(QOS_REMARKING) || defined(TCSUPPORT_HW_QOS)
    qdma_proc = create_proc_entry(mac_qos_path, 0, NULL);
    if (qdma_proc){
        qdma_proc->read_proc = mac_qos_read_proc;
        qdma_proc->write_proc = mac_qos_write_proc;
    }
#endif

#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_XPON_HAL_API_QOS_CA) || defined(TCSUPPORT_CT_PON)
    qdma_proc = create_proc_entry(mac_default_mask_path, 0, NULL);    
    if (qdma_proc){
        qdma_proc->read_proc = mac_default_queuemask_read_proc;
        qdma_proc->write_proc = mac_default_queuemask_write_proc;
    }
#endif

#ifdef TCSUPPORT_QOS
    qdma_proc = create_proc_entry(mac_qos_flag_path, 0, NULL);
    if (qdma_proc){
        qdma_proc->read_proc = mac_qos_flag_read_proc;
        qdma_proc->write_proc = mac_qos_flag_write_proc;
    }
#endif

	qdma_proc = create_proc_entry(mac_resource_limit_path, 0, NULL);
	if (qdma_proc)
	{
		qdma_proc->write_proc = mac_resource_limit_write_proc;
		qdma_proc->read_proc = mac_resource_limit_read_proc;
	}
#endif

	/* Initial for design and verification function */
	if((ret = qdma_dvt_init()) != 0) {
		QDMA_ERR("QDMA verification test initialization failed.\n") ;
		return ret ;
	}
	return 0 ;
}

module_init(qdma_module_init);
module_exit(qdma_module_cleanup);
module_param(physical_size, uint, 0);
module_param(macType, uint, 0);
