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
************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "libapi_lib_qdmamgr.h"


/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
************************************************************************/

/************************************************************************
*                  M A C R O S
************************************************************************/

/************************************************************************
*                  D A T A	 T Y P E S
************************************************************************/

/************************************************************************
*                  E X T E R N A L	 D A T A   D E C L A R A T I O N S
************************************************************************/

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
************************************************************************/

/************************************************************************
*                  P U B L I C   D A T A
************************************************************************/

/************************************************************************
*                  P R I V A T E   D A T A
************************************************************************/

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
************************************************************************/

/******************************************************************************
 Descriptor:	It's used to ioctl to qdma device.
 Input Args:	cmd: ioctl command.
                     p: pointer to qdma driver structure, it is used to 
 Ret Value:	success: QDMAMGR_SUCCESS
                       fail: QDMAMGR_FAIL
******************************************************************************/
static int qdmamgr_lib_ioctl(struct ECNT_QDMA_Data *qdma_data)
{
	int fd, ret;
	
	fd = open(qdma_path, O_RDWR);
    if (fd < 0) {
		printf("Open %s pseudo device failed\n", qdma_path);
		return QDMAMGR_FAIL;
    }

	ret = ioctl(fd, IO_QDMA_CMD_OPT, qdma_data);
	close(fd);

	if(ret >= 0) {
		return QDMAMGR_SUCCESS;
	} else {
		return QDMAMGR_FAIL;
	}
}

/******************************************************************************
 Descriptor:	It's used to set the QDMA qos config.
 Input Args:	weightBase & weightScale.
 Ret Value:	success: QDMAMGR_SUCCESS
                     fail: QDMAMGR_FAIL
******************************************************************************/
int qdmamgr_lib_set_qos_config(QDMA_TxQosWeightType_t weightBase, QDMA_TxQosWeightScale_t weightScale)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_TX_QOS_WEIGHT;
	qdma_data.qdma_private.qdma_tx_qos.weightBase = weightBase;
	qdma_data.qdma_private.qdma_tx_qos.weightScale = weightScale;

	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_qos_value(QDMA_TxQosScheduler_T *pTxQos)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_TX_QOS;
	qdma_data.qdma_private.qdma_tx_qos.pTxQos = pTxQos;

	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_tx_ratemeter(QDMA_TxRateMeter_T *txRateMeterPtr)
{	
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_TX_RATEMETER;
	qdma_data.qdma_private.txRateMeterPtr = txRateMeterPtr;

	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_tx_ratelimit_config(QDMA_TxRateLimitCfg_T *txRateLimitCfgPtr)
{	
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_TX_RATELIMIT_CFG;
	qdma_data.qdma_private.txRateLimitCfgPtr = txRateLimitCfgPtr;

	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_tx_ratelimit_value(QDMA_TxRateLimitSet_T *txRateLimitPtr)
{	
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_TX_RATELIMIT;
	qdma_data.qdma_private.txRateLimitSetPtr = txRateLimitPtr;

	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_rx_ratelimit_en(QDMA_Mode_t rxRateLimitEn)
{	
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_RX_RATELIMIT_EN;
	qdma_data.qdma_private.mode = rxRateLimitEn;

	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_rx_ratelimit_pkt_mode(QDMA_RxPktMode_t pktMode)
{	
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_RX_RATELIMIT_PKT_MODE;
	qdma_data.qdma_private.pktMode = pktMode;

	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_rx_ratelimit_value(QDMA_RxRateLimitSet_T *rxRateLimitPtr)
{	
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_RX_RATELIMIT;
	qdma_data.qdma_private.rxRateLimitSetPtr = rxRateLimitPtr;

	return qdmamgr_lib_ioctl(&qdma_data);
}


int qdmamgr_lib_set_virtual_channel_config(QDMA_VirtualChannelMode_T *virChnlModePtr)
{	
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_VIRTUAL_CHANNEL_MODE;
	qdma_data.qdma_private.virChnlModePtr = virChnlModePtr;

	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_virtual_channel_value(QDMA_VirtualChannelQoS_T *virChnlQoSPtr)
{	
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_VIRTUAL_CHANNEL_QOS;
	qdma_data.qdma_private.virChnlQoSPtr = virChnlQoSPtr;

	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_rx_low_threshold(QDMA_RxLowThreshold_T *rxLowThresholdPtr)
{	
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_RX_LOW_THRESHOLD;
	qdma_data.qdma_private.rxLowThresholdPtr = rxLowThresholdPtr;

	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_dei_drop_en(QDMA_Mode_t deiDropMode)
{	
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_TXQ_DEI_DROP_MODE;
	qdma_data.qdma_private.mode = deiDropMode ;

	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_lmgr_low_threshold(uint threshold)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_LMGR_LOW_THRESHOLD;
	qdma_data.qdma_private.threshold = threshold ;
	
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_overhead_en(QDMA_Mode_t pMode)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_PKTSIZE_OVERHEAD_EN;
	qdma_data.qdma_private.mode = pMode ;
	
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_overhead_value(uint cnt)
{
	struct ECNT_QDMA_Data qdma_data;
	int ret = 0 ;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_PKTSIZE_OVERHEAD_VALUE;
	qdma_data.qdma_private.cnt = cnt ;
	
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_mac_resource_limit(QDMA_MacLimitThrh_T *qdma_mac_limit_thrh)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_MAC_LIMIT_THRESHOLD;
	qdma_data.qdma_private.qdma_mac_limit_thrh = qdma_mac_limit_thrh;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_txbuf_threshold(QDMA_TxBufCtrl_T *pTxBufCtrl)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_TXBUF_THRESHOLD;
	qdma_data.qdma_private.pQdmaTxBufCtrl = pTxBufCtrl;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_prefetch(QDMA_PrefetchMode_T *qdma_prefetch)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_PREFETCH_MODE;
	qdma_data.qdma_private.qdma_prefetch = qdma_prefetch;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_dbareport(QDMA_TxDbaReport_T *txDbaReportPtr)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_TX_DBAREPORT;
	qdma_data.qdma_private.txDbaReportPtr = txDbaReportPtr;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_get_qos(QDMA_TxQos_T *pTxQos)
{
	struct ECNT_QDMA_Data qdma_data;
	
	#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
	#endif

	if (NULL == pTxQos)
	{
		return QDMAMGR_FAIL;
	}

	/* get the qos type and queue weight */
	qdma_data.function_id = QDMA_FUNCTION_GET_TX_QOS;
	qdma_data.qdma_private.qdma_tx_qos.pTxQos = pTxQos->pTxQos;
	if (QDMAMGR_FAIL == qdmamgr_lib_ioctl(&qdma_data))
	{

		printf("qdmamgr_lib_get_qos: GET_TX_QOS failed\n");
		return QDMAMGR_FAIL;
	}

	/* get the weightBase and weightScale */
	qdma_data.function_id = QDMA_FUNCTION_GET_TX_QOS_WEIGHT;
	if (QDMAMGR_FAIL == qdmamgr_lib_ioctl(&qdma_data))
	{
		printf("qdmamgr_lib_get_qos: GET_TX_QOS_WEIGHT failed\n");
		return QDMAMGR_FAIL;
	}

	pTxQos->weightBase = qdma_data.qdma_private.qdma_tx_qos.weightBase;
	pTxQos->weightScale = qdma_data.qdma_private.qdma_tx_qos.weightScale;
	
	return QDMAMGR_SUCCESS;
}

int qdmamgr_lib_get_tx_ratelimit(QDMA_TxRateLimitGet_T *txRateLimitPtr)
{	
	struct ECNT_QDMA_Data qdma_data;
	
	#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
	#endif

	if (NULL == txRateLimitPtr)
	{
		return QDMAMGR_FAIL;
	}
	
	qdma_data.function_id = QDMA_FUNCTION_GET_TX_RATELIMIT;
	qdma_data.qdma_private.txRateLimitGetPtr = txRateLimitPtr;
	
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_get_rx_ratelimit(QDMA_RxRateLimitGet_T *rxRateLimitPtr,QDMA_RxRateLimitCfg_T *rxRateLimitCfgPtr)
{	
	struct ECNT_QDMA_Data qdma_data;
	
	#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
	#endif

	if ((NULL == rxRateLimitPtr) || (NULL == rxRateLimitCfgPtr))
	{
		return QDMAMGR_FAIL;
	}

	/* get rx ratelimit value */
	qdma_data.function_id = QDMA_FUNCTION_GET_RX_RATELIMIT;
	qdma_data.qdma_private.rxRateLimitGetPtr = rxRateLimitPtr;
	if (QDMAMGR_FAIL == qdmamgr_lib_ioctl(&qdma_data))
	{
		printf("qdmamgr_lib_get_rx_ratelimit: GET_RX_RATELIMIT failed\n");
		return QDMAMGR_FAIL;
	}

	/* get rx ratelimit config */
	qdma_data.function_id = QDMA_FUNCTION_GET_RX_RATELIMIT_CFG;
	qdma_data.qdma_private.rxRateLimitCfgPtr = rxRateLimitCfgPtr;
	if (QDMAMGR_FAIL == qdmamgr_lib_ioctl(&qdma_data))
	{
		printf("qdmamgr_lib_get_rx_ratelimit: GET_RX_RATELIMIT_CFG failed\n");
		return QDMAMGR_FAIL;
	}
	
	return QDMAMGR_SUCCESS;
}

int qdmamgr_lib_dump_qos(int channel)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_DUMP_TX_QOS;
	qdma_data.qdma_private.channel = channel;

	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_get_tx_ratemeter(QDMA_TxRateMeter_T *txRateMeterPtr)
{	
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_GET_TX_RATEMETER;
	qdma_data.qdma_private.txRateMeterPtr = txRateMeterPtr;

	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_get_tx_ratelimit_config(QDMA_TxRateLimitCfg_T *txRateLimitCfgPtr)
{	
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_GET_TX_RATELIMIT_CFG;
	qdma_data.qdma_private.txRateLimitCfgPtr = txRateLimitCfgPtr;
	
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_dump_tx_ratelimit(int channel)
{	
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_DUMP_TX_RATELIMIT;
	qdma_data.qdma_private.channel = channel;
	
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_dump_rx_ratelimit(int ringIdx)
{	
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_DUMP_RX_RATELIMIT;
	qdma_data.qdma_private.channel = ringIdx;
	
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_get_virtual_channel(int phyChnl)
{	
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_DUMP_VIRTUAL_CHANNEL_QOS;
	qdma_data.qdma_private.channel = phyChnl;

	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_get_rx_low_threshold(QDMA_RxLowThreshold_T *rxLowThresholdPtr)
{	
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_GET_RX_LOW_THRESHOLD;
	qdma_data.qdma_private.rxLowThresholdPtr = rxLowThresholdPtr;

	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_get_dei_drop_en(QDMA_Mode_t *pMode)
{	
	struct ECNT_QDMA_Data qdma_data;
	int ret = 0 ;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_GET_TXQ_DEI_DROP_MODE;
	ret = qdmamgr_lib_ioctl(&qdma_data);
	*pMode = qdma_data.qdma_private.mode;
	
	return ret ;
}

int qdmamgr_lib_get_lmgr_status(QDMA_LmgrStatus_T *lmgrPtr)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_GET_LMGR_STATUS;
	qdma_data.qdma_private.qdma_lmgr_status = lmgrPtr;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_get_lmgr_low_threshold(uint *threshold)
{
	struct ECNT_QDMA_Data qdma_data;
	int ret = 0 ;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_GET_LMGR_LOW_THRESHOLD;
	ret = qdmamgr_lib_ioctl(&qdma_data);
	*threshold = qdma_data.qdma_private.threshold;
	
	return ret ;
}

int qdmamgr_lib_get_overhead_en(QDMA_Mode_t *pMode)
{
	struct ECNT_QDMA_Data qdma_data;
	int ret = 0 ;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_GET_PKTSIZE_OVERHEAD_EN;
	ret = qdmamgr_lib_ioctl(&qdma_data);
	*pMode = qdma_data.qdma_private.mode;

	return ret ;
}

int qdmamgr_lib_get_overhead_value(uint *cnt)
{
	struct ECNT_QDMA_Data qdma_data;
	int ret = 0 ;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_GET_PKTSIZE_OVERHEAD_VALUE;
	ret = qdmamgr_lib_ioctl(&qdma_data);
	*cnt = qdma_data.qdma_private.cnt;

	return ret;
}

int qdmamgr_lib_get_mac_resource_limit(QDMA_MacLimitThrh_T *qdma_mac_limit_thrh)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_GET_MAC_LIMIT_THRESHOLD;
	qdma_data.qdma_private.qdma_mac_limit_thrh = qdma_mac_limit_thrh;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_get_txbuf_threshold(QDMA_TxBufCtrl_T *pTxBufCtrl)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_GET_TXBUF_THRESHOLD;
	qdma_data.qdma_private.pQdmaTxBufCtrl = pTxBufCtrl;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_dump_dbareport(int channel)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_DUMP_TX_DBA_REPORT;
	qdma_data.qdma_private.channel = channel;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_txq_cngst_mode(QDMA_Mode_t dynCngstEn)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_TXQ_CNGST_MODE;
	qdma_data.qdma_private.mode = dynCngstEn;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_txq_cngst_dei_scale(uint threshold)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_TXQ_DEI_THRH_SCALE;
	qdma_data.qdma_private.threshold = threshold;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_txq_cngst_autoconfig(QDMA_txCngstCfg_t *pTxCngstCfg)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_TXQ_CNGST_AUTO_CONFIG;
	qdma_data.qdma_private.pTxCngstCfg = pTxCngstCfg;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_txq_cngst_total_threshold(QDMA_TxQDynCngstTotalThrh_T *totalThrhPtr)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_TXQ_CNGST_TOTAL_THRESHOLD;
	qdma_data.qdma_private.totalThrhPtr = totalThrhPtr;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_txq_cngst_channel_threshold(QDMA_TxQDynCngstChnlThrh_T *chnlThrhPtr)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_TXQ_CNGST_CHANNEL_THRESHOLD;
	qdma_data.qdma_private.chnlThrhPtr = chnlThrhPtr;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_txq_cngst_queue_threshold(QDMA_TxQDynCngstQueueThrh_T *queueThrhPtr)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_TXQ_CNGST_QUEUE_THRESHOLD;
	qdma_data.qdma_private.queueThrhPtr = queueThrhPtr;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_txq_cngst_peekrate(QDMA_PeekRateCfg_t *peekrateCfgPtr)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_PEEKRATE_PARAMS;
	qdma_data.qdma_private.peekrateCfgPtr = peekrateCfgPtr;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_static_cngst_normal(QDMA_TxQStaticNormalCfg_T *normThrhPtr)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_TXQ_STATIC_QUEUE_NORMAL_THRESHOLD;
	qdma_data.qdma_private.normThrhPtr = normThrhPtr;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_static_cngst_dei(QDMA_TxQStaticDeiCfg_T *deiThrhPtr)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_TXQ_STATIC_QUEUE_DEI_THRESHOLD;
	qdma_data.qdma_private.deiThrhPtr = deiThrhPtr;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_get_txq_cngst(void)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_DUMP_TXQ_CNGST;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_dbgcntr_channel(QDMA_DbgCntrChnlGroup_t cntrSrc)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_DBGCNTR_CHANNEL;
	qdma_data.qdma_private.cntrSrc = cntrSrc;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_dbgcntr_queue(void)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_DBGCNTR_QUEUE;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_clear_dbgcntr(void)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_CLEAR_DBGCNTR;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_dump_dbgcntr(void)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_DUMP_DBGCNTR;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_clear_cpu_counter(void)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_CLEAR_CPU_COUNTER;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_dump_cpu_counter(void)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_DUMP_CPU_COUNTER;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_dump_register(void)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_DUMP_REGISTER_INFO;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_dump_descriptor(void)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_DUMP_DESCRIPTOR_INFO;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_dump_irq(void)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_DUMP_IRQ_INFO;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_dump_hwfwd(void)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_DUMP_HWFWD_INFO;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_dump_all(void)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_DUMP_INFO_ALL;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_test_dbglevel(uint level)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_DEBUG_LEVEL;
	qdma_data.qdma_private.qdma_old_proc.dbgLevel = level ;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_test_dma_dusy(uint busyDuration)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_DUMP_DMA_BUSY;
	qdma_data.qdma_private.qdma_old_proc.busyDuration = busyDuration ;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_test_reg_polling(uint regOffset, uint pollingDuration)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_DUMP_REG_POLLING;
	qdma_data.qdma_private.qdma_old_proc.regOffset = regOffset ;
	qdma_data.qdma_private.qdma_old_proc.pollingDuration = pollingDuration ;
	return qdmamgr_lib_ioctl(&qdma_data);
}

int qdmamgr_lib_set_test_force_rx1(uint forceEn)
{
	struct ECNT_QDMA_Data qdma_data;
	
#if API_LIB_QDMAMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	qdma_data.function_id = QDMA_FUNCTION_SET_FORCE_RX_RING1;
	qdma_data.qdma_private.qdma_old_proc.forceEn = forceEn ;
	return qdmamgr_lib_ioctl(&qdma_data);
}


