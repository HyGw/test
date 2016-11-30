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
#ifndef _API_LIB_QDMAMGR_H
#define _API_LIB_QDMAMGR_H

/************************************************************************
*                  D A T A	 T Y P E S
************************************************************************/
typedef unsigned char			unchar;
typedef unsigned short			ushort;
typedef unsigned int			uint;
typedef unsigned long			ulong;
struct sk_buff;

/************************************************************************
*                  I N C L U D E S
************************************************************************/
#include "qdma_type.h"
#include "qdma_wan_lan.h"

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
************************************************************************/
#define QDMAMGR_SUCCESS	(0)
#define QDMAMGR_FAIL	(-1)

/************************************************************************
*                  M A C R O S
************************************************************************/
#if defined(QDMA_LAN)
#define CONFIG_QDMA_CHANNEL 8
#define qdma_path "/dev/qdma_lan"
#define qdmamgr_name "qdmamgr_lan"
#else
#define CONFIG_QDMA_CHANNEL 32
#define qdma_path "/dev/qdma_wan"
#define qdmamgr_name "qdmamgr_wan"
#endif
#define QDMA_TYPE_LAN "qdma_lan"
#define QDMA_TYPE_WAN "qdma_wan"

#define QDMA_LAN_FLAG 0
#define QDMA_WAN_FLAG 1

#define CONFIG_QDMA_LAN_CHANNEL 8
#define CONFIG_QDMA_WAN_CHANNEL 32

#define QDMA_LAN_BASE 0x80
#define QDMA_LAN_MAX (0xFF)
#define QDMA_WAN_BASE 0x180
#define QDMA_WAN_MAX (0x1FF)
#define QDMA_LAN_WAN_MASK (0X7F)

/************************************************************************
*                  E X T E R N A L	 D A T A   D E C L A R A T I O N S
************************************************************************/

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
************************************************************************/
/* set */
int qdmamgr_lib_set_qos_config(QDMA_TxQosWeightType_t weightBase, QDMA_TxQosWeightScale_t weightScale) ;
/******************************************************************************
 Descriptor:	It's used to set the QDMA qos parameters.
 Input Args:	QDMA_TxQosScheduler_T
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int qdmamgr_lib_set_qos_value(QDMA_TxQosScheduler_T *pTxQos) ;
int qdmamgr_lib_set_tx_ratemeter(QDMA_TxRateMeter_T *txRateMeterPtr) ;
int qdmamgr_lib_set_tx_ratelimit_config(QDMA_TxRateLimitCfg_T *txRateLimitCfgPtr) ;
/******************************************************************************
 Descriptor:	It's used to set the QDMA tx ratelimit.
 Input Args:	QDMA_TxQosScheduler_T
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int qdmamgr_lib_set_tx_ratelimit_value(QDMA_TxRateLimitSet_T *txRateLimitPtr) ;
int qdmamgr_lib_set_rx_ratelimit_en(QDMA_Mode_t rxRateLimitEn) ;
int qdmamgr_lib_set_rx_ratelimit_pkt_mode(QDMA_RxPktMode_t pktMode) ;
int qdmamgr_lib_set_rx_ratelimit_value(QDMA_RxRateLimitSet_T *rxRateLimitPtr) ;
int qdmamgr_lib_set_virtual_channel_config(QDMA_VirtualChannelMode_T *virChnlModePtr) ;
int qdmamgr_lib_set_virtual_channel_value(QDMA_VirtualChannelQoS_T *virChnlQoSPtr) ;
int qdmamgr_lib_set_rx_low_threshold(QDMA_RxLowThreshold_T *rxLowThresholdPtr) ;
int qdmamgr_lib_set_dei_drop_en(QDMA_Mode_t deiDropMode) ;
int qdmamgr_lib_set_txq_cngst_mode(QDMA_Mode_t dynCngstEn) ;
int qdmamgr_lib_set_txq_cngst_dei_scale(uint threshold) ;
int qdmamgr_lib_set_txq_cngst_autoconfig(QDMA_txCngstCfg_t *pTxCngstCfg) ;
int qdmamgr_lib_set_txq_cngst_total_threshold(QDMA_TxQDynCngstTotalThrh_T *totalThrhPtr) ;
int qdmamgr_lib_set_txq_cngst_channel_threshold(QDMA_TxQDynCngstChnlThrh_T *chnlThrhPtr) ;
int qdmamgr_lib_set_txq_cngst_queue_threshold(QDMA_TxQDynCngstQueueThrh_T *queueThrhPtr) ;
int qdmamgr_lib_set_txq_cngst_peekrate(QDMA_PeekRateCfg_t *peekrateCfgPtr) ;
int qdmamgr_lib_set_static_cngst_normal(QDMA_TxQStaticNormalCfg_T *normThrhPtr) ;
int qdmamgr_lib_set_static_cngst_dei(QDMA_TxQStaticDeiCfg_T *deiThrhPtr) ;
int qdmamgr_lib_set_test_dbglevel(uint level) ;
int qdmamgr_lib_set_test_dma_dusy(uint busyDuration) ;
int qdmamgr_lib_set_test_reg_polling(uint regOffset, uint pollingDuration) ;
int qdmamgr_lib_set_test_force_rx1(uint isForceRX1) ;
int qdmamgr_lib_set_lmgr_low_threshold(uint threshold) ;
int qdmamgr_lib_set_overhead_en(QDMA_Mode_t pMode) ;
int qdmamgr_lib_set_overhead_value(uint cnt) ;
int qdmamgr_lib_set_mac_resource_limit(QDMA_MacLimitThrh_T *qdma_mac_limit_thrh) ;
int qdmamgr_lib_set_txbuf_threshold(QDMA_TxBufCtrl_T *pTxBufCtrl) ;
int qdmamgr_lib_set_prefetch(QDMA_PrefetchMode_T *qdma_prefetch) ;
int qdmamgr_lib_set_dbareport(QDMA_TxDbaReport_T *txDbaReportPtr) ;

/*___________________________________________________________________________
**	function name: qdmamgr_lib_get_qos
**	descriptions:
**		It's used to get the QDMA qos parameters.
**	input parameters:
**		pTxQos->pTxQos->channel: the qdma channel
**	output parameters:
**		pTxQos->pTxQos->qosType: The QoS type defined in QDMA_TxQosType_t enum.
**		pTxQos->pTxQos->queue[i].weight: weight value
**          pTxQos->weightBase: packet or byte mode
**		pTxQos->weightScale: 64byte or 16byte
**
**	return:
**		success:	QDMAMGR_SUCCESS
**		failure:	QDMAMGR_FAIL
**___________________________________________________________________________
*/
int qdmamgr_lib_get_qos(QDMA_TxQos_T *pTxQos);


/*___________________________________________________________________________
**	function name: qdmamgr_lib_get_tx_ratelimit
**	descriptions:
**		It's used to get the value of tx rate limit parameter
**	input parameters:
**		txRateLimitPtr->chnlIdx: the qdma channel index
**	output parameters:
**		txRateLimitPtr->rateLimitValue: tx rate limit value
**		txRateLimitPtr->pbsParamValue: PBS value
**		txRateLimitPtr->chnlRateLimitEn: tx rate limit enable or disable
**
**	return:
**		success:	QDMAMGR_SUCCESS
**		failure:	QDMAMGR_FAIL
**___________________________________________________________________________
*/
int qdmamgr_lib_get_tx_ratelimit(QDMA_TxRateLimitGet_T *txRateLimitPtr);


/*___________________________________________________________________________
**	function name: qdmamgr_lib_get_rx_ratelimit
**	descriptions:
**		It's used to get the value of rx rate limit parameter
**	input parameters:
**		rxRateLimitPtr->ringIdx: ring index
**	output parameters:
**		rxRateLimitPtr->rateLimitValue: tx rate limit value
**		rxRateLimitPtr->pbsParamValue: PBS value
**		rxRateLimitCfgPtr->rxRateLimitEn: enable or disable
**		rxRateLimitCfgPtr->rxRateLimitPktMode: packet or byte
**
**	return:
**		success:	QDMAMGR_SUCCESS
**		failure:	QDMAMGR_FAIL
**___________________________________________________________________________
*/
int qdmamgr_lib_get_rx_ratelimit(QDMA_RxRateLimitGet_T *rxRateLimitPtr,QDMA_RxRateLimitCfg_T *rxRateLimitCfgPtr);


/* get */
/******************************************************************************
 Descriptor:	It's used to dump the QDMA qos parameters.
 Input Args:	channel: the channel of qdma you wan to dump
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int qdmamgr_lib_dump_qos(int channel) ;
int qdmamgr_lib_get_tx_ratemeter(QDMA_TxRateMeter_T *txRateMeterPtr) ;
int qdmamgr_lib_get_tx_ratelimit_config(QDMA_TxRateLimitCfg_T *txRateLimitCfgPtr) ;
/******************************************************************************
 Descriptor:	It's used to dump the QDMA tx ratelimit.
 Input Args:	channel: the channel of qdma you wan to dump
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int qdmamgr_lib_dump_tx_ratelimit(int channel) ;
int qdmamgr_lib_dump_rx_ratelimit(int ringIdx) ;
int qdmamgr_lib_get_virtual_channel(int phyChnl) ;
int qdmamgr_lib_get_rx_low_threshold(QDMA_RxLowThreshold_T *rxLowThresholdPtr) ;
int qdmamgr_lib_get_dei_drop_en(QDMA_Mode_t *pMode) ;
int qdmamgr_lib_get_txq_cngst(void) ;
int qdmamgr_lib_get_lmgr_status(QDMA_LmgrStatus_T *lmgrPtr) ;
int qdmamgr_lib_get_lmgr_low_threshold(uint *threshold) ;
int qdmamgr_lib_get_overhead_en(QDMA_Mode_t *pMode) ;
int qdmamgr_lib_get_overhead_value(uint *cnt) ;
int qdmamgr_lib_get_mac_resource_limit(QDMA_MacLimitThrh_T *qdma_mac_limit_thrh) ;
int qdmamgr_lib_get_txbuf_threshold(QDMA_TxBufCtrl_T *pTxBufCtrl) ;
int qdmamgr_lib_dump_dbareport(int channel) ;

/* other */
int qdmamgr_lib_set_dbgcntr_channel(QDMA_DbgCntrChnlGroup_t cntrSrc) ;
int qdmamgr_lib_set_dbgcntr_queue(void) ;
int qdmamgr_lib_clear_dbgcntr(void) ;
int qdmamgr_lib_dump_dbgcntr(void) ;
int qdmamgr_lib_clear_cpu_counter(void) ;
int qdmamgr_lib_dump_cpu_counter(void) ;
int qdmamgr_lib_dump_register(void) ;
int qdmamgr_lib_dump_descriptor(void) ;
int qdmamgr_lib_dump_irq(void) ;
int qdmamgr_lib_dump_hwfwd(void) ;
int qdmamgr_lib_dump_all(void) ;

/************************************************************************
*                  P U B L I C   D A T A
************************************************************************/

/************************************************************************
*                  P R I V A T E   D A T A
************************************************************************/

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
************************************************************************/

// ====================== redefine api name for FH =========================
#define ecnt_qdmamgr_lan_lib_set_qos_value qdmamgr_lan_lib_set_qos_value
#define ecnt_qdmamgr_lan_lib_set_tx_ratelimit_value qdmamgr_lan_lib_set_tx_ratelimit_value
#define ecnt_qdmamgr_lan_lib_get_qos qdmamgr_lan_lib_get_qos
#define ecnt_qdmamgr_lan_lib_get_tx_ratelimit qdmamgr_lan_lib_get_tx_ratelimit
#define ecnt_qdmamgr_lan_lib_get_rx_ratelimit qdmamgr_lan_lib_get_rx_ratelimit

#define ecnt_qdmamgr_wan_lib_set_qos_value qdmamgr_wan_lib_set_qos_value
#define ecnt_qdmamgr_wan_lib_set_tx_ratelimit_value qdmamgr_wan_lib_set_tx_ratelimit_value
#define ecnt_qdmamgr_wan_lib_get_qos qdmamgr_wan_lib_get_qos
#define ecnt_qdmamgr_wan_lib_get_tx_ratelimit qdmamgr_wan_lib_get_tx_ratelimit
#define ecnt_qdmamgr_wan_lib_get_rx_ratelimit qdmamgr_wan_lib_get_rx_ratelimit

#endif
