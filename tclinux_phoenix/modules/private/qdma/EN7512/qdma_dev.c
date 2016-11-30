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
#include <linux/delay.h>
#include <linux/cdev.h>

#include "qdma_dev.h"
#include "qdma_bmgr.h"
#include "qdma_dvt.h"


/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
#define GDM2_CHNL_RELEASE_CMD						(1<<0)
#define GDM2_CHNL_RELEASE_DONE						(1<<1)
#define GDM2_CHNK_RELEASE_CHN_SHIFT					(4)
#define GDM2_CHNK_RELEASE_CHN_MASK					(0xF<<GDM2_CHNK_RELEASE_CHN_SHIFT)

#define CDM_TX_ENCODING_SHIFT					(0)
#define CDM_TX_ENCODING_MASK					(0xF<<CDM_TX_ENCODING_SHIFT)
#define CDM_RX_DECODING_SHIFT					(4)
#define CDM_RX_DECODING_MASK					(0xF<<CDM_RX_DECODING_SHIFT)
#define GDM_RX_DECODING_SHIFT					(8)
#define GDM_RX_DECODING_MASK					(0xF<<GDM_RX_DECODING_SHIFT)

#define DEF_PRIORITY_PKT_CHK_LEN     100


/************************************************************************
*                  M A C R O S
*************************************************************************
*/


/************************************************************************
*                  D A T A	 T Y P E S
*************************************************************************
*/

/************************************************************************
*                  E X T E R N A L	 D A T A   D E C L A R A T I O N S
*************************************************************************
*/
#ifdef TCSUPPORT_QOS
extern int qos_flag;
#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_XPON_HAL_API_QOS_CA) || defined(TCSUPPORT_CT_PON)
extern int gQueueMask;
#endif
#endif
extern int qos_wrr_info[5];
extern atomic_t chnlLimit[CONFIG_QDMA_CHANNEL];
extern atomic_t queueLimit[CONFIG_QDMA_CHANNEL][CONFIG_QDMA_QUEUE];

extern qdma_api_op_t qdma_operation[];

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/
struct timer_list dma_busy_timer ;
int dma_busy_expires = 10; /* 10ms */
int dma_busy_round_cnt = 0;
int tx_dma_busy_enable_cnt=0;
int rx_dma_busy_enable_cnt=0;

struct timer_list reg_polling_timer ;
int reg_polling_expires = 10; /* 10ms */
int reg_polling_round_cnt = 0;
int reg_polling_offset = 0;
uint qdmaRegValBuff[100];
    
uint channel_limit_threshold = 0 ;
uint queue_limit_threshold = 0 ;

QDMA_Dev_T *gpQdmaDev = NULL ;

unchar qdma_vip_num = 0 ;/*vip rule num*/
QDMA_VIP_INFO_T qdma_vip_info[QDMA_VIP_PACKET_NUM] ; /*sync with fe info,just for port compare*/

/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/
int priPktChkLen = DEF_PRIORITY_PKT_CHK_LEN;
module_param(priPktChkLen, int, 0);

#ifdef PURE_BRIDGE
int priPktChk = 0;
#else
int priPktChk = 1;
#endif
module_param(priPktChk, int, 0);


/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
#ifdef CONFIG_NEW_QDMA_CSR_OFFSET
static int __pseChannelRelease(unchar channel)
{
	uint regAddr = 0xBFB51528 ;
	uint chnRelease = 0 ;
	int RETRY = 3 ;
	
	chnRelease = (GDM2_CHNL_RELEASE_CMD | ((channel<<GDM2_CHNK_RELEASE_CHN_SHIFT)&GDM2_CHNK_RELEASE_CHN_MASK)) ;
	IO_SREG(regAddr, chnRelease) ;

	while(RETRY--) {
		chnRelease = IO_GREG(regAddr) ;
		QDMA_MSG(DBG_MSG, "Read PSE Channel Release: %.8x\n", chnRelease) ;
		if(chnRelease&GDM2_CHNL_RELEASE_DONE) {
			/* Clear the channel release */
			chnRelease = ((channel<<GDM2_CHNK_RELEASE_CHN_SHIFT)&GDM2_CHNK_RELEASE_CHN_MASK) ;
			IO_SREG(regAddr, chnRelease) ;
			QDMA_MSG(DBG_MSG, "Write PSE Channel Release: %.8x\n", chnRelease) ;
			break ;
		}
		mdelay(5) ;
	}
	if(RETRY < 0) {
		return -ETIME ;
	}
	return 0 ;
}

int __inline__ pseSetWanPcpConfig(unchar type, unchar mode)
{
	uint regAddr = 0xBFB51514 ;
	
	if(type == 0) { /* CDM Tx Encoding */
		IO_SMASK(regAddr, CDM_TX_ENCODING_MASK, CDM_TX_ENCODING_SHIFT, mode) ;
	} else if(type == 1) { /* CDM Rx Decoding */
		IO_SMASK(regAddr, CDM_RX_DECODING_MASK, CDM_RX_DECODING_SHIFT, mode) ;
	} else if(type == 2) { /* GDM Rx Decoding */
		IO_SMASK(regAddr, GDM_RX_DECODING_MASK, GDM_RX_DECODING_SHIFT, mode) ;
	} 

	return 0 ;
}

int __inline__ pseGetWanPcpConfig(unchar type)
{
	uint regAddr = 0xBFB51514 ;
	uint pcpMode = 0 ;
	
	if(type == 0) { /* CDM Tx Encoding */
		pcpMode = IO_GMASK(regAddr, CDM_TX_ENCODING_MASK, CDM_TX_ENCODING_SHIFT) ;
	} else if(type == 1) { /* CDM Rx Decoding */
		pcpMode = IO_GMASK(regAddr, CDM_RX_DECODING_MASK, CDM_RX_DECODING_SHIFT) ;
	} else if(type == 2) { /* GDM Rx Decoding */
		pcpMode = IO_GMASK(regAddr, GDM_RX_DECODING_MASK, GDM_RX_DECODING_SHIFT) ;
	} 

	return pcpMode ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaSetChannelRetire(unchar channel)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint chnRetire = 0 ;
	int ret = 0, RETRY = 3 ;
	int i = 0 ;

	qdmaDisableTxDma(base);

	if(!qdmaIsTxDmaBusy(base))
	{
        qdmaDisableTxDma(base) ;

        /* Release the GDM2 channel */
        for(i=0 ; i<2 ; i++) 
        {
            ret = __pseChannelRelease(channel) ;
            mdelay(10) ;
        }

        /* Release the QDMA channel */
        chnRetire = (LMGR_CHNL_RETIRE_CMD | ((channel<<LMGR_CHNL_RETIRE_CHN_SHIFT)&LMGR_CHNL_RETIRE_CHN_MASK)) ;
        IO_SREG(QDMA_CSR_LMGR_CHNL_RETIRE(base), chnRetire) ;
        while(RETRY--)
        {
            chnRetire = IO_GREG(QDMA_CSR_LMGR_CHNL_RETIRE(base)) ;
            QDMA_MSG(DBG_MSG, "Read QDMA Channel Release: %.8x\n", chnRetire) ;
            if(chnRetire&LMGR_CHNL_RETIRE_DONE) 
            {			
                /* Clear the channel retire */
                chnRetire = ((channel<<LMGR_CHNL_RETIRE_CHN_SHIFT)&LMGR_CHNL_RETIRE_CHN_MASK) ;
                IO_SREG(QDMA_CSR_LMGR_CHNL_RETIRE(base), chnRetire) ;
                QDMA_MSG(DBG_MSG, "Set QDMA Channel Release: %.8x\n", chnRetire) ;
                break ;
            }
            mdelay(5) ;
        }
        if(RETRY < 0) 
        {
            ret = -ETIME ;
        }

        qdmaEnableTxDma(base) ;
	}
	else
	{
	    ret = -EADV; /* need to use qdmaSetRetire to retire all used channels */
	}
	
    qdmaEnableTxDma(base);

	return ret ;
}

int __inline__ qdmaSetRetire(unchar maxChannel)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint chnRetire = 0 ;
	int ret = 0, RETRY = 3 ;
	int i = 0 ;
	unchar channel = 0;
	uint regval = 0;
	
    qdmaDisableTxDma(base);
    
    regval = IO_GREG(0xBFB51844);
    /*disable all LMGR Tx*/
    IO_SREG(0xBFB51844, regval | 0xFFFF);
	
	/* Release the GDM2 channel */
    for(i=0 ; i<2 ; i++)
    {
        for(channel =0 ; channel < maxChannel; channel++)
        {
            ret = __pseChannelRelease(channel) ;
            mdelay(5) ;
        }
    }
    
	/* Release the QDMA channel */
    for(channel =0 ; channel < maxChannel; channel++)
    {
        chnRetire = (LMGR_CHNL_RETIRE_CMD | ((channel<<LMGR_CHNL_RETIRE_CHN_SHIFT)&LMGR_CHNL_RETIRE_CHN_MASK)) ;
        IO_SREG(QDMA_CSR_LMGR_CHNL_RETIRE(base), chnRetire) ;
        RETRY = 3;
        while(RETRY--) 
        {
            chnRetire = IO_GREG(QDMA_CSR_LMGR_CHNL_RETIRE(base)) ;
            QDMA_MSG(DBG_MSG, "Read QDMA Channel Release: %.8x\n", chnRetire) ;
            if(chnRetire&LMGR_CHNL_RETIRE_DONE) 
            {			
                /* Clear the channel retire */
                chnRetire = ((channel<<LMGR_CHNL_RETIRE_CHN_SHIFT)&LMGR_CHNL_RETIRE_CHN_MASK) ;
                IO_SREG(QDMA_CSR_LMGR_CHNL_RETIRE(base), chnRetire) ;
                QDMA_MSG(DBG_MSG, "Set QDMA Channel Release: %.8x\n", chnRetire) ;
                break ;
            }
            mdelay(5) ;
        }
        if(RETRY < 0) 
        {
            ret = -ETIME ;
        }
    }

    /* enable all LMGR TX */
    IO_SREG(0xBFB51844, regval);

    qdmaEnableTxDma(base);
	
	return ret ;
}

#endif /* CONFIG_NEW_QDMA_CSR_OFFSET */

static int __inline__ qdmaChecConfigDone(uint reg, uint doneBit)
{
	int RETRY = 3 ;
	volatile uint regValue = 0 ;
	
	while(RETRY--) {
		regValue = IO_GREG(reg) ;
		
		if(regValue & doneBit) {
			break ;
		}
		mdelay(1) ;
	}
	if(RETRY < 0) {
		return -ETIME ;
	}

	return 0;
}

/******************************************************************************
******************************************************************************/
int qdmaSetTxQosScheduler(unchar channel, unchar mode, unchar weight[8])
{
	int i = 0 ;
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint wrrCfg = 0 ;

	qdmaSetPerChnlQosMode(base, channel, mode) ;
	
	for(i=0 ; i<8 ; i++) {
		if(weight[i] != 255) {
			wrrCfg = (TXWRR_RWCMD | 
					  ((weight[i]<<TXWRR_WRR_VALUE_SHIFT)&TXWRR_WRR_VALUE_MASK) |
					  ((channel<<TXWRR_CHNL_IDX_SHIFT)&TXWRR_CHNL_IDX_MASK) |
					  ((i<<TXWRR_QUEUE_IDX_SHIFT)&TXWRR_QUEUE_IDX_MASK)) ;
			IO_SREG(QDMA_CSR_TXWRR_WEIGHT_CFG(base), wrrCfg) ;
		
			if(qdmaChecConfigDone(QDMA_CSR_TXWRR_WEIGHT_CFG(base), TXWRR_RWCMD_DONE) < 0) {
				QDMA_ERR("Timeout for setting WRR configuration, channel:%d, queue:%d.\n", channel, i) ;
				return -ETIME ;
			}
		}
	}
	
	return 0 ;
}

/******************************************************************************
******************************************************************************/
int qdmaGetTxQosScheduler(unchar channel, unchar *pMode, unchar weight[8])
{
	int i = 0 ;
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint wrrCfg = 0 ;
	
	*pMode = qdmaGetPerChnlQosMode(base, channel) ;

	for(i=0 ; i<8 ; i++) {
		wrrCfg = (((channel<<TXWRR_CHNL_IDX_SHIFT)&TXWRR_CHNL_IDX_MASK) |
				  ((i<<TXWRR_QUEUE_IDX_SHIFT)&TXWRR_QUEUE_IDX_MASK)) ;
		IO_SREG(QDMA_CSR_TXWRR_WEIGHT_CFG(base), wrrCfg) ;
	
		if(qdmaChecConfigDone(QDMA_CSR_TXWRR_WEIGHT_CFG(base), TXWRR_RWCMD_DONE) < 0) {
			QDMA_ERR("Timeout for getting WRR configuration, channel:%d, queue:%d.\n", channel, i) ;
			return -ETIME ;
		}
		wrrCfg = IO_GREG(QDMA_CSR_TXWRR_WEIGHT_CFG(base)) ;
		weight[i] =  ((wrrCfg&TXWRR_WRR_VALUE_MASK)>>TXWRR_WRR_VALUE_SHIFT) ;
	}
	return 0 ;
}


/******************************************************************************
******************************************************************************/
int qdmaSetRxRateLimitConfig(unchar ringIdx, unchar type, ushort value) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint rxRateLimitCfg = 0 ;
	
	rxRateLimitCfg = (RX_RATELIMIT_RWCMD | 
					((value<<RX_RATELIMIT_VALUE_SHIFT)&RX_RATELIMIT_VALUE_MASK) |
					(ringIdx?(RX_RATELIMIT_RING_IDX):(0)) |
					(type?(RX_RATELIMIT_TYPE):(0))) ;
	QDMA_MSG(DBG_MSG, "rxRateLimitCfg is 0x%08x.\n", rxRateLimitCfg) ;
	IO_SREG(QDMA_CSR_RX_RATELIMIT_CFG(base), rxRateLimitCfg) ;

	if(qdmaChecConfigDone(QDMA_CSR_RX_RATELIMIT_CFG(base), RX_RATELIMIT_RWCMD_DONE) < 0) {
		QDMA_ERR("Timeout for set Rx Rate Limit configuration.\n") ;
		return -ETIME ;
	}
	
	return 0 ;
}

int qdmaGetRxRateLimitConfig(unchar ringIdx, unchar type)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint rxRateLimitCfg = 0 ;
	
	rxRateLimitCfg = ((ringIdx?(RX_RATELIMIT_RING_IDX):(0)) |
					(type?(RX_RATELIMIT_TYPE):(0))) ;
	IO_SREG(QDMA_CSR_RX_RATELIMIT_CFG(base), rxRateLimitCfg) ;
	
	if(qdmaChecConfigDone(QDMA_CSR_RX_RATELIMIT_CFG(base), RX_RATELIMIT_RWCMD_DONE) < 0) {
		QDMA_ERR("Timeout for get Rx Rate Limit configuration.\n") ;
		return -ETIME ;
	}
	
	rxRateLimitCfg = IO_GREG(QDMA_CSR_RX_RATELIMIT_CFG(base)) ;	
	return (rxRateLimitCfg&RX_RATELIMIT_VALUE_MASK)>>RX_RATELIMIT_VALUE_SHIFT ;
}


/******************************************************************************
******************************************************************************/
int qdmaSetTxRateLimitConfig(unchar chnlIdx, unchar type, ushort value) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint txRateLimitCfg = 0 ;
	
	txRateLimitCfg = (EGRESS_RATELIMIT_PARA_RWCMD | 
					((value<<EGRESS_RATELIMIT_PARA_VALUE_SHIFT)&EGRESS_RATELIMIT_PARA_VALUE_MASK) |
					((chnlIdx<<EGRESS_RATELIMIT_PARA_IDX_SHIFT)&EGRESS_RATELIMIT_PARA_IDX_MASK) |
					((type<<EGRESS_RATELIMIT_PARA_TYPE_SHIFT)&EGRESS_RATELIMIT_PARA_TYPE_MASK)) ;
	QDMA_MSG(DBG_MSG, "txRateLimitCfg is 0x%08x.\n", txRateLimitCfg) ;
	IO_SREG(QDMA_CSR_RATELIMIT_PARAMETER_CFG(base), txRateLimitCfg) ;
	
	if(qdmaChecConfigDone(QDMA_CSR_RATELIMIT_PARAMETER_CFG(base), EGRESS_RATELIMIT_PARA_RWCMD_DONE) < 0) {
		QDMA_ERR("Timeout for set Tx Rate Limit configuration.\n") ;
		return -ETIME ;
	}
	
	return 0 ;
}

int qdmaGetTxRateLimitConfig(unchar chnlIdx, unchar type)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint txRateLimitCfg = 0 ;
	
	txRateLimitCfg = (((chnlIdx<<EGRESS_RATELIMIT_PARA_IDX_SHIFT)&EGRESS_RATELIMIT_PARA_IDX_MASK) |
					((type<<EGRESS_RATELIMIT_PARA_TYPE_SHIFT)&EGRESS_RATELIMIT_PARA_TYPE_MASK)) ;
	IO_SREG(QDMA_CSR_RATELIMIT_PARAMETER_CFG(base), txRateLimitCfg) ;
	
	if(qdmaChecConfigDone(QDMA_CSR_RATELIMIT_PARAMETER_CFG(base), EGRESS_RATELIMIT_PARA_RWCMD_DONE) < 0) {
		QDMA_ERR("Timeout for get Tx Rate Limit configuration.\n") ;
		return -ETIME ;
	}
	
	txRateLimitCfg = IO_GREG(QDMA_CSR_RATELIMIT_PARAMETER_CFG(base)) ;	
	return ((txRateLimitCfg&EGRESS_RATELIMIT_PARA_VALUE_MASK)>>EGRESS_RATELIMIT_PARA_VALUE_SHIFT) ;
}

/******************************************************************************
******************************************************************************/
int qdmaSetVirtualChannelQos(unchar phyChnl, unchar virChnlMax, unchar mode, unchar weight[4])
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint wrrCfg = 0 ;
	int virChnl = 0 ;
		
	IO_SMASK(QDMA_CSR_VCHNL_QOS_MODE(base), PHYSICAL_CHNL_QOS_MODE_MASK(phyChnl), PHYSICAL_CHNL_QOS_MODE_SHIFT(phyChnl), mode) ;
	
	for(virChnl=0 ; virChnl<virChnlMax ; virChnl++) {
		if(weight[virChnl] != 255) {
			wrrCfg = (VCHNL_WEIGHT_RWCMD | 
					  ((weight[virChnl]<<VCHNL_WRR_VALUE_SHIFT)&VCHNL_WRR_VALUE_MASK) |
					  ((phyChnl<<PHYSICAL_CHNL_IDX_SHIFT)&PHYSICAL_CHNL_IDX_MASK) |
					  ((virChnl<<VIRTUAL_CHNL_IDX_SHIFT)&VIRTUAL_CHNL_IDX_MASK)) ;
			IO_SREG(QDMA_CSR_VCHNL_WEIGHT_CFG(base), wrrCfg) ;
			
			if(qdmaChecConfigDone(QDMA_CSR_VCHNL_WEIGHT_CFG(base), VCHNL_WEIGHT_DONE) < 0) {
				QDMA_ERR("Timeout for setting virtual channel configuration, physical channel:%d, virtual channel:%d.\n", phyChnl, virChnl) ;
				return -ETIME ;
			}
		}
	}
	
	return 0 ;
}

/******************************************************************************
******************************************************************************/
int qdmaGetVirtualChannelQos(unchar phyChnl, unchar virChnlMax, unchar *pMode, unchar weight[4])
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint wrrCfg = 0 ;
	int virChnl = 0;
	
	*pMode = IO_GMASK(QDMA_CSR_VCHNL_QOS_MODE(base), PHYSICAL_CHNL_QOS_MODE_MASK(phyChnl), PHYSICAL_CHNL_QOS_MODE_SHIFT(phyChnl)) ;

	for(virChnl=0 ; virChnl<virChnlMax ; virChnl++) {
		wrrCfg = (((phyChnl<<PHYSICAL_CHNL_IDX_SHIFT)&PHYSICAL_CHNL_IDX_MASK) |
				  ((virChnl<<VIRTUAL_CHNL_IDX_SHIFT)&VIRTUAL_CHNL_IDX_MASK)) ;
		IO_SREG(QDMA_CSR_VCHNL_WEIGHT_CFG(base), wrrCfg) ;
	
		if(qdmaChecConfigDone(QDMA_CSR_VCHNL_WEIGHT_CFG(base), VCHNL_WEIGHT_DONE) < 0) {
			QDMA_ERR("Timeout for getting virtual channel configuration, physical channel:%d, virtual channel:%d.\n", phyChnl, virChnl) ;
			return -ETIME ;
		}
		wrrCfg = IO_GREG(QDMA_CSR_VCHNL_WEIGHT_CFG(base)) ;
		weight[virChnl] =  ((wrrCfg&VCHNL_WRR_VALUE_MASK)>>VCHNL_WRR_VALUE_SHIFT) ;
	}
	return 0 ;
}

int qdmaGetLimitRateMax(void)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	int index=0, curRate=0, maxLimitRate=0;

	for(index=0; index<CONFIG_QDMA_CHANNEL; index++) {
		if(qdmaIsChnlRateLimitEnable(base, index)) {
			curRate = qdmaGetTxRateLimitConfig(index, QDMA_Tx_Rate_Limit_PIR) ;
			if(curRate < 0)
				return curRate ;
			if(curRate > maxLimitRate)
				maxLimitRate = curRate ;
		}
	}
	
	return maxLimitRate ;
}

int qdmaUpdateAllTxRateLimitValue(ushort curUnit, ushort newUnit)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	int ret=0, index=0, curPirValue=0, newPirValue=0;

	if(newUnit == 0)
		return -1;
	
	for(index=0; index<CONFIG_QDMA_CHANNEL; index++) {
		curPirValue = qdmaGetTxRateLimitConfig(index, QDMA_Tx_Rate_Limit_PIR) ;
		if(curPirValue < 0)
			return curPirValue ;			
		newPirValue = curPirValue * curUnit / newUnit;
		if((ret = qdmaSetTxRateLimitConfig(index, QDMA_Tx_Rate_Limit_PIR, newPirValue)) < 0) {
			return ret ;
		}
	}
	
	return 0 ;
}

int qdmaUpdateAllRxRateLimitValue(ushort curUnit, ushort newUnit)
{
	int ret=0, index=0, curPirValue=0, newPirValue=0;

	if(newUnit == 0)
		return -1;
	
	for(index=0; index<RX_RING_NUM; index++) {
		curPirValue = qdmaGetRxRateLimitConfig(index, QDMA_Rx_Rate_Limit_PIR) ;
		if(curPirValue < 0)
			return curPirValue ;			
		newPirValue = curPirValue * curUnit / newUnit;
		if((ret = qdmaSetRxRateLimitConfig(index, QDMA_Rx_Rate_Limit_PIR, newPirValue)) < 0) {
			return ret ;
		}
	}
	
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to configure dbg counter, which has 40 groups to set.
 Input Args:		- cntrIdx:
 				- cntrEn:
 				- cntrSrc:
 				- chnlIdx:
 				- queueIdx:
 				- dscpRingIdx:
 				- isChnlAll:
 				- isQueueAll:
 				- isDscpRingAll:
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_clear_and_set_dbg_cntr_info(QDMA_DBG_CNTR_T *dbgCntrPtr)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	unchar idx = dbgCntrPtr->cntrIdx;
	
	if(dbgCntrPtr->cntrIdx<0 || dbgCntrPtr->cntrIdx>=CONFIG_MAX_CNTR_NUM) {
		QDMA_ERR("Fault: dbg Cntr counter index shoule between 0 and %d\n", CONFIG_MAX_CNTR_NUM-1) ; 
		return -EINVAL ;
	}
	if(dbgCntrPtr->cntrSrc<0 || dbgCntrPtr->cntrSrc>=QDMA_DBG_CNTR_SRC_ITEMS) {
		QDMA_ERR("Fault: dbg Cntr source shoule between 0 and %d\n", QDMA_DBG_CNTR_SRC_ITEMS-1) ; 
		return -EINVAL ;
	}
	if((dbgCntrPtr->isChnlAll == QDMA_DISABLE) && (dbgCntrPtr->chnlIdx<0 || dbgCntrPtr->chnlIdx>=CONFIG_QDMA_CHANNEL)) {
		QDMA_ERR("Fault: Channel index shoule between 0 and %d\n", CONFIG_QDMA_CHANNEL-1) ; 
		return -EINVAL ;
	}
	if((dbgCntrPtr->isQueueAll == QDMA_DISABLE) && (dbgCntrPtr->queueIdx<0 || dbgCntrPtr->queueIdx>=CONFIG_QDMA_QUEUE)) {
		QDMA_ERR("Fault: Queue index shoule between 0 and %d\n", CONFIG_QDMA_QUEUE-1) ; 
		return -EINVAL ;
	}
	if((dbgCntrPtr->isDscpRingAll == QDMA_DISABLE) && (dbgCntrPtr->dscpRingIdx<0 || dbgCntrPtr->dscpRingIdx>1)) {
		QDMA_ERR("Fault: ringIdx shoule be 0 or 1\n") ; 
		return -EINVAL ;
	}

	qdmaSetCntrSrc(base, idx, dbgCntrPtr->cntrSrc);
	qdmaSetCntrChnlIdx(base, idx, dbgCntrPtr->chnlIdx);
	qdmaSetCntrQueueIdx(base, idx, dbgCntrPtr->queueIdx);
	qdmaSetCntrDscpIdx(base, idx, dbgCntrPtr->dscpRingIdx);
	
	if(dbgCntrPtr->isChnlAll == QDMA_DISABLE) {
		qdmaSetCntrChnlSingle(base,idx);
	} else {
		qdmaSetCntrChnlAll(base,idx);
	}
	
	if(dbgCntrPtr->isQueueAll == QDMA_DISABLE) {
		qdmaSetCntrQueueSingle(base,idx);
	} else {
		qdmaSetCntrQueueAll(base,idx);
	}
	
	if(dbgCntrPtr->isDscpRingAll == QDMA_DISABLE) {
		qdmaSetCntrDscpSingle(base,idx);
	} else {
		qdmaSetCntrDscpAll(base,idx);
	}
	
	qdmaClearCntrCounter(base, idx);
	if(dbgCntrPtr->cntrEn == QDMA_DISABLE) {
		qdmaDisableCntrCfg(base, idx);
	} else {
		qdmaEnableCntrCfg(base, idx);
	}
	
	return 0 ;
}

/******************************************************************************
 Descriptor:	It's used to configure dbg counter, which has 32 groups to set.
 Input Args:		- cntrIdx:
 Output Args:	- cntrEn:
 				- cntrSrc:
 				- chnlIdx:
 				- queueIdx:
 				- dscpRingIdx:
 				- isChnlAll:
 				- isQueueAll:
 				- isDscpRingAll:
 				- cntrVal:
 Ret Value:		0: successful, otherwise failed.
******************************************************************************/
int qdma_get_dbg_cntr_info(QDMA_DBG_CNTR_T *dbgCntrPtr)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	unchar idx = dbgCntrPtr->cntrIdx ;

	if(dbgCntrPtr->cntrIdx<0 || dbgCntrPtr->cntrIdx>=CONFIG_MAX_CNTR_NUM) {
		QDMA_ERR("Fault: dbg Cntr counter index shoule between 0 and %d\n", CONFIG_MAX_CNTR_NUM-1) ; 
		return -EINVAL ;
	}

	dbgCntrPtr->cntrEn = qdmaIsCntrEnable(base, idx);
	dbgCntrPtr->cntrSrc = qdmaGetCntrSrc(base,idx);
	dbgCntrPtr->chnlIdx = qdmaGetCntrChnlIdx(base,idx);
	dbgCntrPtr->queueIdx = qdmaGetCntrQueueIdx(base,idx);
	dbgCntrPtr->dscpRingIdx = qdmaGetCntrDscpIdx(base,idx);
	
	dbgCntrPtr->isChnlAll = qdmaIsCntrChnlAll(base,idx);
	dbgCntrPtr->isQueueAll = qdmaIsCntrQueueAll(base,idx);
	dbgCntrPtr->isDscpRingAll = qdmaIsCntrDscpRingAll(base,idx);
	dbgCntrPtr->cntrVal = qdmaGetCntrCounter(base, idx);

	return 0;
}

int qdma_set_dbg_cntr_default_config(void)
{
	int index=0 ;
    QDMA_DBG_CNTR_T dbgCntr ;
	dbgCntr.cntrEn = DBG_CNTR_ENABLE;
	dbgCntr.isChnlAll = 1;
	dbgCntr.isQueueAll = 1;
	dbgCntr.isDscpRingAll = 1;
	dbgCntr.queueIdx = 0 ;
	dbgCntr.chnlIdx = 0 ;
	dbgCntr.dscpRingIdx = 0;

	for(index = 0; index<DBG_CNTR_DEFAULT_CONFIG_NUM; index++) {
		switch(index) {
			case 0:		// tx cpu-path total count
        		dbgCntr.cntrSrc = DBG_CNTR_SRC_CPU_TX;			
				break;
				
			case 1:		// tx total count
        		dbgCntr.cntrSrc = DBG_CNTR_SRC_FWD_TX;	
				break;
				
			case 2:		// rx cpu-path total count
        		dbgCntr.cntrSrc = DBG_CNTR_SRC_CPU_RX;			
				break;
				
			case 3:		// rx fwd-path total count
        		dbgCntr.cntrSrc = DBG_CNTR_SRC_FWD_RX;	
				break;
				
			case 4:		// tx cpu-path Ring0 count
			    dbgCntr.isDscpRingAll = 0;
			    dbgCntr.dscpRingIdx = 0;
        		dbgCntr.cntrSrc = DBG_CNTR_SRC_CPU_TX;			
				break;
				
			case 5:		// tx cpu-path Ring1 count
			    dbgCntr.isDscpRingAll = 0;
			    dbgCntr.dscpRingIdx = 1;
        		dbgCntr.cntrSrc = DBG_CNTR_SRC_CPU_TX;	
				break;
				
			case 6:		// rx cpu-path Ring0 count
			    dbgCntr.isDscpRingAll = 0;
			    dbgCntr.dscpRingIdx = 0;
        		dbgCntr.cntrSrc = DBG_CNTR_SRC_CPU_RX;			
				break;
				
			case 7:		// rx cpu-path Ring1 count
			    dbgCntr.isDscpRingAll = 0;
			    dbgCntr.dscpRingIdx = 1;
        		dbgCntr.cntrSrc = DBG_CNTR_SRC_CPU_RX;	
				break;
		}
		dbgCntr.cntrIdx = index+DBG_CNTR_DEFAULT_CONFIG_OFFSET ;
        qdma_clear_and_set_dbg_cntr_info(&dbgCntr);		
	}
	return 0;
}

#if 0
/******************************************************************************
******************************************************************************/
int __inline__ qdmaSetTrtcm(unchar channel, unchar type, ushort value)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint trtcmCfg = 0 ;
	int RETRY = 3 ;
	
	trtcmCfg = (GPON_TRTCM_RWCMD | 
				((value<<GPON_TRTCM_VALUE_SHIFT)&GPON_TRTCM_VALUE_MASK) |
				((channel<<GPON_TRTCM_CHN_SHIFT)&GPON_TRTCM_CHN_MASK) |
				((type<<GPON_TRTCM_TYPE_SHIFT)&GPON_TRTCM_TYPE_MASK)) ;
#ifdef CONFIG_NEW_QDMA_CSR_OFFSET
	IO_SREG(QDMA_CSR_GPON_TCONT_TRTCM_CFG(base), trtcmCfg) ;
#else
    IO_SREG(QDMA_CSR_GPON_TRTCM_CFG(base), trtcmCfg) ;
#endif

	while(RETRY--) {
    #ifdef CONFIG_NEW_QDMA_CSR_OFFSET
		trtcmCfg = IO_GREG(QDMA_CSR_GPON_TCONT_TRTCM_CFG(base)) ;
    #else
        trtcmCfg = IO_GREG(QDMA_CSR_GPON_TRTCM_CFG(base)) ;
    #endif
		
		if(trtcmCfg&GPON_TRTCM_DONE) {
			return 0 ;
		}
		mdelay(1) ;
	}

	QDMA_ERR("Timeout for set trTCM configuration.\n") ;
	
	return -ETIME ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaGetTrtcm(unchar channel, unchar type)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint trtcmCfg = 0 ;
	int RETRY = 3 ;
	
	trtcmCfg = (((channel<<GPON_TRTCM_CHN_SHIFT)&GPON_TRTCM_CHN_MASK) |
				((type<<GPON_TRTCM_TYPE_SHIFT)&GPON_TRTCM_TYPE_MASK)) ;
#ifdef CONFIG_NEW_QDMA_CSR_OFFSET
        IO_SREG(QDMA_CSR_GPON_TCONT_TRTCM_CFG(base), trtcmCfg) ;
#else
        IO_SREG(QDMA_CSR_GPON_TRTCM_CFG(base), trtcmCfg) ;
#endif

	while(RETRY--) {
    #ifdef CONFIG_NEW_QDMA_CSR_OFFSET
            trtcmCfg = IO_GREG(QDMA_CSR_GPON_TCONT_TRTCM_CFG(base)) ;
    #else
            trtcmCfg = IO_GREG(QDMA_CSR_GPON_TRTCM_CFG(base)) ;
    #endif
		
		if(trtcmCfg&GPON_TRTCM_DONE) {
			return ((trtcmCfg&GPON_TRTCM_VALUE_MASK)>>GPON_TRTCM_VALUE_SHIFT) ;
		} 
		mdelay(1) ;
	}

	QDMA_ERR("Timeout for get trTCM configuration.\n") ;
	
	return -ETIME ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaSetTxQueueCngsScale(unchar maxScale, unchar minScale) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint txqCngsCfg = 0 ;

	txqCngsCfg = IO_GREG(QDMA_CSR_TXQ_CNGST_CFG(base)) ;
	txqCngsCfg = (txqCngsCfg&~TXQ_MAX_THRSHLD_SCALE_MASK) | ((maxScale<<TXQ_MAX_THRSHLD_SCALE_SHIFT)&TXQ_MAX_THRSHLD_SCALE_MASK) ;
	txqCngsCfg = (txqCngsCfg&~TXQ_MIN_THRSHLD_SCALE_MASK) | ((minScale<<TXQ_MIN_THRSHLD_SCALE_SHIFT)&TXQ_MIN_THRSHLD_SCALE_MASK) ;
	
	IO_SREG(QDMA_CSR_TXQ_CNGST_CFG(base), txqCngsCfg) ;

	return 0 ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaGetTxQueueCngsScale(unchar *pMaxScale, unchar *pMinScale) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint txqCngsCfg = 0 ;

	txqCngsCfg = IO_GREG(QDMA_CSR_TXQ_CNGST_CFG(base)) ;
	
	*pMaxScale = (txqCngsCfg & TXQ_MAX_THRSHLD_SCALE_MASK) >> TXQ_MAX_THRSHLD_SCALE_SHIFT ;
	*pMinScale = (txqCngsCfg & TXQ_MIN_THRSHLD_SCALE_MASK) >> TXQ_MIN_THRSHLD_SCALE_SHIFT ;

	return 0 ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaSetTxQueueDropProbability(unchar grnDrop, unchar ylwDrop) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint txqCngsCfg = 0 ;

	txqCngsCfg = IO_GREG(QDMA_CSR_TXQ_CNGST_CFG(base)) ;
	txqCngsCfg = (txqCngsCfg&~TXQ_YLW_DROP_PROBABILITY_MASK) | ((ylwDrop<<TXQ_YLW_DROP_PROBABILITY_SHIFT)&TXQ_YLW_DROP_PROBABILITY_MASK) ;
	txqCngsCfg = (txqCngsCfg&~TXQ_GRN_DROP_PROBABILITY_MASK) | ((grnDrop<<TXQ_GRN_DROP_PROBABILITY_SHIFT)&TXQ_GRN_DROP_PROBABILITY_MASK) ;
	
	IO_SREG(QDMA_CSR_TXQ_CNGST_CFG(base), txqCngsCfg) ;

	return 0 ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaGetTxQueueDropProbability(unchar *pGrnDrop, unchar *pYlwDrop) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint txqCngsCfg = 0 ;

	txqCngsCfg = IO_GREG(QDMA_CSR_TXQ_CNGST_CFG(base)) ;
	
	*pYlwDrop = (txqCngsCfg & TXQ_YLW_DROP_PROBABILITY_MASK) >> TXQ_YLW_DROP_PROBABILITY_SHIFT ;
	*pGrnDrop = (txqCngsCfg & TXQ_GRN_DROP_PROBABILITY_MASK) >> TXQ_GRN_DROP_PROBABILITY_SHIFT ;
	
	return 0 ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaSetTxQueueGreenMaxThreshold(unchar txqIdx[CONFIG_QDMA_QUEUE]) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint grnMaxCfg = 0 ;
	
	grnMaxCfg = (((txqIdx[0]<<TXQ0_GRN_MAX_THRESHOLD_SHIFT)&TXQ0_GRN_MAX_THRESHOLD_MASK) |
				 ((txqIdx[1]<<TXQ1_GRN_MAX_THRESHOLD_SHIFT)&TXQ1_GRN_MAX_THRESHOLD_MASK) |
				 ((txqIdx[2]<<TXQ2_GRN_MAX_THRESHOLD_SHIFT)&TXQ2_GRN_MAX_THRESHOLD_MASK) |
				 ((txqIdx[3]<<TXQ3_GRN_MAX_THRESHOLD_SHIFT)&TXQ3_GRN_MAX_THRESHOLD_MASK) |
				 ((txqIdx[4]<<TXQ4_GRN_MAX_THRESHOLD_SHIFT)&TXQ4_GRN_MAX_THRESHOLD_MASK) |
				 ((txqIdx[5]<<TXQ5_GRN_MAX_THRESHOLD_SHIFT)&TXQ5_GRN_MAX_THRESHOLD_MASK) |
				 ((txqIdx[6]<<TXQ6_GRN_MAX_THRESHOLD_SHIFT)&TXQ6_GRN_MAX_THRESHOLD_MASK) |
				 ((txqIdx[7]<<TXQ7_GRN_MAX_THRESHOLD_SHIFT)&TXQ7_GRN_MAX_THRESHOLD_MASK)) ;
	
	IO_SREG(QDMA_CSR_TXQ_GRN_MAX_THRSHLD(base), grnMaxCfg) ;
	
	return 0 ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaGetTxQueueGreenMaxThreshold(unchar txqIdx[CONFIG_QDMA_QUEUE]) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint grnMaxCfg = 0 ;
	
	grnMaxCfg = IO_GREG(QDMA_CSR_TXQ_GRN_MAX_THRSHLD(base)) ;
	txqIdx[0] = ((grnMaxCfg & TXQ0_GRN_MAX_THRESHOLD_MASK) >> TXQ0_GRN_MAX_THRESHOLD_SHIFT) ;
	txqIdx[1] = ((grnMaxCfg & TXQ1_GRN_MAX_THRESHOLD_MASK) >> TXQ1_GRN_MAX_THRESHOLD_SHIFT) ;
	txqIdx[2] = ((grnMaxCfg & TXQ2_GRN_MAX_THRESHOLD_MASK) >> TXQ2_GRN_MAX_THRESHOLD_SHIFT) ;
	txqIdx[3] = ((grnMaxCfg & TXQ3_GRN_MAX_THRESHOLD_MASK) >> TXQ3_GRN_MAX_THRESHOLD_SHIFT) ;
	txqIdx[4] = ((grnMaxCfg & TXQ4_GRN_MAX_THRESHOLD_MASK) >> TXQ4_GRN_MAX_THRESHOLD_SHIFT) ;
	txqIdx[5] = ((grnMaxCfg & TXQ5_GRN_MAX_THRESHOLD_MASK) >> TXQ5_GRN_MAX_THRESHOLD_SHIFT) ;
	txqIdx[6] = ((grnMaxCfg & TXQ6_GRN_MAX_THRESHOLD_MASK) >> TXQ6_GRN_MAX_THRESHOLD_SHIFT) ;
	txqIdx[7] = ((grnMaxCfg & TXQ7_GRN_MAX_THRESHOLD_MASK) >> TXQ7_GRN_MAX_THRESHOLD_SHIFT) ;
	
	return 0 ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaSetTxQueueGreenMinThreshold(unchar txqIdx[CONFIG_QDMA_QUEUE]) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint grnMinCfg = 0 ;
	
	grnMinCfg = (((txqIdx[0]<<TXQ0_GRN_MIN_THRESHOLD_SHIFT)&TXQ0_GRN_MIN_THRESHOLD_MASK) |
				 ((txqIdx[1]<<TXQ1_GRN_MIN_THRESHOLD_SHIFT)&TXQ1_GRN_MIN_THRESHOLD_MASK) |
				 ((txqIdx[2]<<TXQ2_GRN_MIN_THRESHOLD_SHIFT)&TXQ2_GRN_MIN_THRESHOLD_MASK) |
				 ((txqIdx[3]<<TXQ3_GRN_MIN_THRESHOLD_SHIFT)&TXQ3_GRN_MIN_THRESHOLD_MASK) |
				 ((txqIdx[4]<<TXQ4_GRN_MIN_THRESHOLD_SHIFT)&TXQ4_GRN_MIN_THRESHOLD_MASK) |
				 ((txqIdx[5]<<TXQ5_GRN_MIN_THRESHOLD_SHIFT)&TXQ5_GRN_MIN_THRESHOLD_MASK) |
				 ((txqIdx[6]<<TXQ6_GRN_MIN_THRESHOLD_SHIFT)&TXQ6_GRN_MIN_THRESHOLD_MASK) |
				 ((txqIdx[7]<<TXQ7_GRN_MIN_THRESHOLD_SHIFT)&TXQ7_GRN_MIN_THRESHOLD_MASK)) ;
	
	IO_SREG(QDMA_CSR_TXQ_GRN_MIN_THRSHLD(base), grnMinCfg) ;
			
	return 0 ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaGetTxQueueGreenMinThreshold(unchar txqIdx[CONFIG_QDMA_QUEUE]) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint grnMinCfg = 0 ;
	
	grnMinCfg = IO_GREG(QDMA_CSR_TXQ_GRN_MIN_THRSHLD(base)) ;
	txqIdx[0] = ((grnMinCfg & TXQ0_GRN_MIN_THRESHOLD_MASK) >> TXQ0_GRN_MIN_THRESHOLD_SHIFT) ;
	txqIdx[1] = ((grnMinCfg & TXQ1_GRN_MIN_THRESHOLD_MASK) >> TXQ1_GRN_MIN_THRESHOLD_SHIFT) ;
	txqIdx[2] = ((grnMinCfg & TXQ2_GRN_MIN_THRESHOLD_MASK) >> TXQ2_GRN_MIN_THRESHOLD_SHIFT) ;
	txqIdx[3] = ((grnMinCfg & TXQ3_GRN_MIN_THRESHOLD_MASK) >> TXQ3_GRN_MIN_THRESHOLD_SHIFT) ;
	txqIdx[4] = ((grnMinCfg & TXQ4_GRN_MIN_THRESHOLD_MASK) >> TXQ4_GRN_MIN_THRESHOLD_SHIFT) ;
	txqIdx[5] = ((grnMinCfg & TXQ5_GRN_MIN_THRESHOLD_MASK) >> TXQ5_GRN_MIN_THRESHOLD_SHIFT) ;
	txqIdx[6] = ((grnMinCfg & TXQ6_GRN_MIN_THRESHOLD_MASK) >> TXQ6_GRN_MIN_THRESHOLD_SHIFT) ;
	txqIdx[7] = ((grnMinCfg & TXQ7_GRN_MIN_THRESHOLD_MASK) >> TXQ7_GRN_MIN_THRESHOLD_SHIFT) ;
	
	return 0 ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaSetTxQueueYellowMaxThreshold(unchar txqIdx[CONFIG_QDMA_QUEUE]) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint ylwMaxCfg = 0 ;
	
	ylwMaxCfg = (((txqIdx[0]<<TXQ0_YLW_MAX_THRESHOLD_SHIFT)&TXQ0_YLW_MAX_THRESHOLD_MASK) |
				 ((txqIdx[1]<<TXQ1_YLW_MAX_THRESHOLD_SHIFT)&TXQ1_YLW_MAX_THRESHOLD_MASK) |
				 ((txqIdx[2]<<TXQ2_YLW_MAX_THRESHOLD_SHIFT)&TXQ2_YLW_MAX_THRESHOLD_MASK) |
				 ((txqIdx[3]<<TXQ3_YLW_MAX_THRESHOLD_SHIFT)&TXQ3_YLW_MAX_THRESHOLD_MASK) |
				 ((txqIdx[4]<<TXQ4_YLW_MAX_THRESHOLD_SHIFT)&TXQ4_YLW_MAX_THRESHOLD_MASK) |
				 ((txqIdx[5]<<TXQ5_YLW_MAX_THRESHOLD_SHIFT)&TXQ5_YLW_MAX_THRESHOLD_MASK) |
				 ((txqIdx[6]<<TXQ6_YLW_MAX_THRESHOLD_SHIFT)&TXQ6_YLW_MAX_THRESHOLD_MASK) |
				 ((txqIdx[7]<<TXQ7_YLW_MAX_THRESHOLD_SHIFT)&TXQ7_YLW_MAX_THRESHOLD_MASK)) ;
	
	IO_SREG(QDMA_CSR_TXQ_YLW_MAX_THRSHLD(base), ylwMaxCfg) ;
		
	return 0 ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaGetTxQueueYellowMaxThreshold(unchar txqIdx[CONFIG_QDMA_QUEUE]) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint ylwMaxCfg = 0 ;
	
	ylwMaxCfg = IO_GREG(QDMA_CSR_TXQ_YLW_MAX_THRSHLD(base)) ;
	txqIdx[0] = ((ylwMaxCfg & TXQ0_YLW_MAX_THRESHOLD_MASK) >> TXQ0_YLW_MAX_THRESHOLD_SHIFT) ;
	txqIdx[1] = ((ylwMaxCfg & TXQ1_YLW_MAX_THRESHOLD_MASK) >> TXQ1_YLW_MAX_THRESHOLD_SHIFT) ;
	txqIdx[2] = ((ylwMaxCfg & TXQ2_YLW_MAX_THRESHOLD_MASK) >> TXQ2_YLW_MAX_THRESHOLD_SHIFT) ;
	txqIdx[3] = ((ylwMaxCfg & TXQ3_YLW_MAX_THRESHOLD_MASK) >> TXQ3_YLW_MAX_THRESHOLD_SHIFT) ;
	txqIdx[4] = ((ylwMaxCfg & TXQ4_YLW_MAX_THRESHOLD_MASK) >> TXQ4_YLW_MAX_THRESHOLD_SHIFT) ;
	txqIdx[5] = ((ylwMaxCfg & TXQ5_YLW_MAX_THRESHOLD_MASK) >> TXQ5_YLW_MAX_THRESHOLD_SHIFT) ;
	txqIdx[6] = ((ylwMaxCfg & TXQ6_YLW_MAX_THRESHOLD_MASK) >> TXQ6_YLW_MAX_THRESHOLD_SHIFT) ;
	txqIdx[7] = ((ylwMaxCfg & TXQ7_YLW_MAX_THRESHOLD_MASK) >> TXQ7_YLW_MAX_THRESHOLD_SHIFT) ;
	
	return 0 ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaSetTxQueueYellowMinThreshold(unchar txqIdx[CONFIG_QDMA_QUEUE]) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint ylwMinCfg = 0 ;
	
	ylwMinCfg = (((txqIdx[0]<<TXQ0_YLW_MIN_THRESHOLD_SHIFT)&TXQ0_YLW_MIN_THRESHOLD_MASK) |
				 ((txqIdx[1]<<TXQ1_YLW_MIN_THRESHOLD_SHIFT)&TXQ1_YLW_MIN_THRESHOLD_MASK) |
				 ((txqIdx[2]<<TXQ2_YLW_MIN_THRESHOLD_SHIFT)&TXQ2_YLW_MIN_THRESHOLD_MASK) |
				 ((txqIdx[3]<<TXQ3_YLW_MIN_THRESHOLD_SHIFT)&TXQ3_YLW_MIN_THRESHOLD_MASK) |
				 ((txqIdx[4]<<TXQ4_YLW_MIN_THRESHOLD_SHIFT)&TXQ4_YLW_MIN_THRESHOLD_MASK) |
				 ((txqIdx[5]<<TXQ5_YLW_MIN_THRESHOLD_SHIFT)&TXQ5_YLW_MIN_THRESHOLD_MASK) |
				 ((txqIdx[6]<<TXQ6_YLW_MIN_THRESHOLD_SHIFT)&TXQ6_YLW_MIN_THRESHOLD_MASK) |
				 ((txqIdx[7]<<TXQ7_YLW_MIN_THRESHOLD_SHIFT)&TXQ7_YLW_MIN_THRESHOLD_MASK)) ;
	
	IO_SREG(QDMA_CSR_TXQ_YLW_MIN_THRSHLD(base), ylwMinCfg) ;

	return 0 ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaGetTxQueueYellowMinThreshold(unchar txqIdx[CONFIG_QDMA_QUEUE]) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint ylwMinCfg = 0 ;
	
	ylwMinCfg = IO_GREG(QDMA_CSR_TXQ_YLW_MIN_THRSHLD(base)) ;
	txqIdx[0] = ((ylwMinCfg & TXQ0_YLW_MIN_THRESHOLD_MASK) >> TXQ0_YLW_MIN_THRESHOLD_SHIFT) ;
	txqIdx[1] = ((ylwMinCfg & TXQ1_YLW_MIN_THRESHOLD_MASK) >> TXQ1_YLW_MIN_THRESHOLD_SHIFT) ;
	txqIdx[2] = ((ylwMinCfg & TXQ2_YLW_MIN_THRESHOLD_MASK) >> TXQ2_YLW_MIN_THRESHOLD_SHIFT) ;
	txqIdx[3] = ((ylwMinCfg & TXQ3_YLW_MIN_THRESHOLD_MASK) >> TXQ3_YLW_MIN_THRESHOLD_SHIFT) ;
	txqIdx[4] = ((ylwMinCfg & TXQ4_YLW_MIN_THRESHOLD_MASK) >> TXQ4_YLW_MIN_THRESHOLD_SHIFT) ;
	txqIdx[5] = ((ylwMinCfg & TXQ5_YLW_MIN_THRESHOLD_MASK) >> TXQ5_YLW_MIN_THRESHOLD_SHIFT) ;
	txqIdx[6] = ((ylwMinCfg & TXQ6_YLW_MIN_THRESHOLD_MASK) >> TXQ6_YLW_MIN_THRESHOLD_SHIFT) ;
	txqIdx[7] = ((ylwMinCfg & TXQ7_YLW_MIN_THRESHOLD_MASK) >> TXQ7_YLW_MIN_THRESHOLD_SHIFT) ;
	
	return 0 ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaSetTxQueueTrtcmConfig(unchar idx, unchar type, unchar scale, ushort value) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint trtcmCfg = 0 ;
	int RETRY = 3 ;
	
	trtcmCfg = (TRTCM_RWCMD | 
				((value<<TRTCM_VALUE_SHIFT)&TRTCM_VALUE_MASK) |
				((scale<<TRTCM_SCALE_SHIFT)&TRTCM_SCALE_MASK) |
				((idx<<TRTCM_IDX_SHIFT)&TRTCM_IDX_MASK) |
				((type<<TRTCM_TYPE_SHIFT)&TRTCM_TYPE_MASK)) ;
	IO_SREG(QDMA_CSR_TRTCM_CFG(base), trtcmCfg) ;

	while(RETRY--) {
		trtcmCfg = IO_GREG(QDMA_CSR_TRTCM_CFG(base)) ;
		
		if(trtcmCfg&TRTCM_DONE) {
			return 0 ;
		}
		mdelay(1) ;
	}

	QDMA_ERR("Timeout for set tx queue trTCM configuration.\n") ;
	
	return -ETIME ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaGetTxQueueTrtcmConfig(unchar idx, unchar scale, unchar type) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint trtcmCfg = 0 ;
	int RETRY = 3 ;
	
	trtcmCfg = (((idx<<TRTCM_IDX_SHIFT)&TRTCM_IDX_MASK) |
				((scale<<TRTCM_SCALE_SHIFT)&TRTCM_SCALE_MASK) |
				((type<<TRTCM_TYPE_SHIFT)&TRTCM_TYPE_MASK)) ;
	IO_SREG(QDMA_CSR_TRTCM_CFG(base), trtcmCfg) ;

	while(RETRY--) {
		trtcmCfg = IO_GREG(QDMA_CSR_TRTCM_CFG(base)) ;
		
		if(trtcmCfg&TRTCM_DONE) {
			return ((trtcmCfg&TRTCM_VALUE_MASK)>>TRTCM_VALUE_SHIFT) ;
		} 
		mdelay(1) ;
	}

	QDMA_ERR("Timeout for get tx queue trTCM configuration.\n") ;
	
	return -ETIME ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaSetGponTrtcmConfig(unchar channel, unchar type, unchar scale, ushort value) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint trtcmCfg = 0 ;
	int RETRY = 3 ;
	
	trtcmCfg = (GPON_TRTCM_RWCMD | 
				((value<<GPON_TRTCM_VALUE_SHIFT)&GPON_TRTCM_VALUE_MASK) |
				((scale<<GPON_TRTCM_SCALE_SHIFT)&GPON_TRTCM_SCALE_MASK) |
				((channel<<GPON_TRTCM_CHANNEL_SHIFT)&GPON_TRTCM_CHANNEL_MASK) |
				((type<<GPON_TRTCM_TYPE_SHIFT)&GPON_TRTCM_TYPE_MASK)) ;
	IO_SREG(QDMA_CSR_GPON_TCONT_TRTCM_CFG(base), trtcmCfg) ;

	while(RETRY--) {
		trtcmCfg = IO_GREG(QDMA_CSR_GPON_TCONT_TRTCM_CFG(base)) ;
		
		if(trtcmCfg&GPON_TRTCM_DONE) {
			return 0 ;
		}
		mdelay(1) ;
	}

	QDMA_ERR("Timeout for set GPON trTCM configuration.\n") ;
	
	return -ETIME ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaGetGponTrtcmConfig(unchar channel, unchar type) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint trtcmCfg = 0 ;
	int RETRY = 3 ;
	
	trtcmCfg = (((channel<<GPON_TRTCM_CHANNEL_SHIFT)&GPON_TRTCM_CHANNEL_MASK) |
				((type<<GPON_TRTCM_TYPE_SHIFT)&GPON_TRTCM_TYPE_MASK)) ;
	IO_SREG(QDMA_CSR_GPON_TCONT_TRTCM_CFG(base), trtcmCfg) ;

	while(RETRY--) {
		trtcmCfg = IO_GREG(QDMA_CSR_GPON_TCONT_TRTCM_CFG(base)) ;
		
		if(trtcmCfg&GPON_TRTCM_DONE) {
			return ((trtcmCfg&GPON_TRTCM_VALUE_MASK)>>GPON_TRTCM_VALUE_SHIFT) ;
		} 
		mdelay(1) ;
	}

	QDMA_ERR("Timeout for get GPON trTCM configuration.\n") ;
	
	return -ETIME ;
}


/******************************************************************************
******************************************************************************/
int __inline__ qdmaSetEponReportConfig(unchar channel, unchar mode)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	
	if(channel == 0) {
		IO_SMASK(QDMA_CSR_EPON_RPT_CFG(base), EPON_RPT_CH0_MASK, EPON_RPT_CH0_SHIFT, mode) ;
	} else if(channel == 1) {
		IO_SMASK(QDMA_CSR_EPON_RPT_CFG(base), EPON_RPT_CH1_MASK, EPON_RPT_CH1_SHIFT, mode) ;
	} else if(channel == 2) {
		IO_SMASK(QDMA_CSR_EPON_RPT_CFG(base), EPON_RPT_CH2_MASK, EPON_RPT_CH2_SHIFT, mode) ;
	} else if(channel == 3) {
		IO_SMASK(QDMA_CSR_EPON_RPT_CFG(base), EPON_RPT_CH3_MASK, EPON_RPT_CH3_SHIFT, mode) ;
	} else if(channel == 4) {
		IO_SMASK(QDMA_CSR_EPON_RPT_CFG(base), EPON_RPT_CH4_MASK, EPON_RPT_CH4_SHIFT, mode) ;
	} else if(channel == 5) {
		IO_SMASK(QDMA_CSR_EPON_RPT_CFG(base), EPON_RPT_CH5_MASK, EPON_RPT_CH5_SHIFT, mode) ;
	} else if(channel == 6) {
		IO_SMASK(QDMA_CSR_EPON_RPT_CFG(base), EPON_RPT_CH6_MASK, EPON_RPT_CH6_SHIFT, mode) ;
	} else if(channel == 7) {
		IO_SMASK(QDMA_CSR_EPON_RPT_CFG(base), EPON_RPT_CH7_MASK, EPON_RPT_CH7_SHIFT, mode) ;
	} 
	
	return 0 ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaGetEponReportConfig(unchar channel, unchar *pMode)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	
	if(channel == 0) {
		*pMode = IO_GMASK(QDMA_CSR_EPON_RPT_CFG(base), EPON_RPT_CH0_MASK, EPON_RPT_CH0_SHIFT) ;
	} else if(channel == 1) {
		*pMode = IO_GMASK(QDMA_CSR_EPON_RPT_CFG(base), EPON_RPT_CH1_MASK, EPON_RPT_CH1_SHIFT) ;
	} else if(channel == 2) {
		*pMode = IO_GMASK(QDMA_CSR_EPON_RPT_CFG(base), EPON_RPT_CH2_MASK, EPON_RPT_CH2_SHIFT) ;
	} else if(channel == 3) {
		*pMode = IO_GMASK(QDMA_CSR_EPON_RPT_CFG(base), EPON_RPT_CH3_MASK, EPON_RPT_CH3_SHIFT) ;
	} else if(channel == 4) {
		*pMode = IO_GMASK(QDMA_CSR_EPON_RPT_CFG(base), EPON_RPT_CH4_MASK, EPON_RPT_CH4_SHIFT) ;
	} else if(channel == 5) {
		*pMode = IO_GMASK(QDMA_CSR_EPON_RPT_CFG(base), EPON_RPT_CH5_MASK, EPON_RPT_CH5_SHIFT) ;
	} else if(channel == 6) {
		*pMode = IO_GMASK(QDMA_CSR_EPON_RPT_CFG(base), EPON_RPT_CH6_MASK, EPON_RPT_CH6_SHIFT) ;
	} else if(channel == 7) {
		*pMode = IO_GMASK(QDMA_CSR_EPON_RPT_CFG(base), EPON_RPT_CH7_MASK, EPON_RPT_CH7_SHIFT) ;
	} 
	
	return 0 ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaSetEponThreshold(unchar channel, unchar queue, unchar type, ushort value)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint eponThresholdCfg = 0 ;
	int RETRY = 3 ;
	
	eponThresholdCfg = (EPON_QTHRESHLD_RWCMD | 
				((value<<EPON_QTHRESHLD_VALUE_SHIFT)&EPON_QTHRESHLD_VALUE_MASK) |
				((type<<EPON_QTHRESHLD_TYPE_SHIFT)&EPON_QTHRESHLD_TYPE_MASK) |
				((channel<<EPON_QTHRESHLD_CHANNEL_SHIFT)&EPON_QTHRESHLD_CHANNEL_MASK) |
				((queue<<EPON_QTHRESHLD_QUEUE_SHIFT)&EPON_QTHRESHLD_QUEUE_MASK)) ;
	IO_SREG(QDMA_CSR_EPON_QTHRESHLD_CFG(base), eponThresholdCfg) ;

	while(RETRY--) {
		eponThresholdCfg = IO_GREG(QDMA_CSR_EPON_QTHRESHLD_CFG(base)) ;
		
		if(eponThresholdCfg&EPON_QTHRESHLD_DONE) {
			return 0 ;
		}
		mdelay(1) ;
	}

	QDMA_ERR("Timeout for set EPON Threshold configuration.\n") ;
	
	return -ETIME ;
}

/******************************************************************************
******************************************************************************/
int __inline__ qdmaGetEponThreshold(unchar channel, unchar queue, unchar type)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint eponThresholdCfg = 0 ;
	int RETRY = 3 ;
	
	eponThresholdCfg = (((type<<EPON_QTHRESHLD_TYPE_SHIFT)&EPON_QTHRESHLD_TYPE_MASK) |
				((channel<<EPON_QTHRESHLD_CHANNEL_SHIFT)&EPON_QTHRESHLD_CHANNEL_MASK) |
				((queue<<EPON_QTHRESHLD_QUEUE_SHIFT)&EPON_QTHRESHLD_QUEUE_MASK)) ;
	IO_SREG(QDMA_CSR_EPON_QTHRESHLD_CFG(base), eponThresholdCfg) ;

	while(RETRY--) {
		eponThresholdCfg = IO_GREG(QDMA_CSR_EPON_QTHRESHLD_CFG(base)) ;
		
		if(eponThresholdCfg&EPON_QTHRESHLD_DONE) {
			return ((eponThresholdCfg&EPON_QTHRESHLD_VALUE_MASK)>>EPON_QTHRESHLD_VALUE_SHIFT) ;
		} 
		mdelay(1) ;
	}

	QDMA_ERR("Timeout for get EPON Threshold configuration.\n") ;
	
	return -ETIME ;
}
#endif

/******************************************************************************
******************************************************************************/
int qdmaSetDbgMemXsConfig(QDMA_DbgMemXsMemSel_t memSel, unchar byteSel, ushort addr, uint valueLow, uint valueHigh) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint dbgMemXsCfg = 0 ;

	/*  prepare data for DATA_LO and DATA_HI register */
	IO_SREG(QDMA_CSR_DBG_MEM_XS_DATA_LO(base), valueLow) ;
	IO_SREG(QDMA_CSR_DBG_MEM_XS_DATA_HI(base), valueHigh) ;

	/* write to dbg mem xs config */
	dbgMemXsCfg = (DBG_MEM_XS_RWCMD | 
					((memSel<<DBG_MEM_XS_MEMSEL_SHIFT)&DBG_MEM_XS_MEMSEL_MASK) |
					((byteSel<<DBG_MEM_XS_BYTESEL_SHIFT)&DBG_MEM_XS_BYTESEL_MASK) |
					((addr<<DBG_MEM_XS_ADDR_SHIFT)&DBG_MEM_XS_ADDR_MASK)) ;
	printk("dbgMemXsCfg is 0x%08x, valueLow:0x%08x, valueHigh:0x%08x.\n", dbgMemXsCfg, valueLow, valueHigh) ;
	IO_SREG(QDMA_CSR_DBG_MEM_XS_CFG(base), dbgMemXsCfg) ;
	
	if(qdmaChecConfigDone(QDMA_CSR_DBG_MEM_XS_CFG(base), DBG_MEM_XS_RWCMD_DONE) < 0) {
		QDMA_ERR("Timeout for set dbg mem xs data.\n") ;
		return -ETIME ;
	}
	
	return 0 ;
}

int qdmaGetDbgMemXsConfig(QDMA_DbgMemXsMemSel_t memSel, unchar byteSel, ushort addr, uint *value)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint dbgMemXsCfg = 0 ;
	
	dbgMemXsCfg = (((memSel<<DBG_MEM_XS_MEMSEL_SHIFT)&DBG_MEM_XS_MEMSEL_MASK) |
					((byteSel<<DBG_MEM_XS_BYTESEL_SHIFT)&DBG_MEM_XS_BYTESEL_MASK) |
					((addr<<DBG_MEM_XS_ADDR_SHIFT)&DBG_MEM_XS_ADDR_MASK)) ;
	
	IO_SREG(QDMA_CSR_DBG_MEM_XS_CFG(base), dbgMemXsCfg) ;
	
	if(qdmaChecConfigDone(QDMA_CSR_DBG_MEM_XS_CFG(base), DBG_MEM_XS_RWCMD_DONE) < 0) {
		QDMA_ERR("Timeout for get dbg mem xs value.\n") ;
		return -ETIME ;
	}
	
	*value = IO_GREG(QDMA_CSR_DBG_MEM_XS_DATA_LO(base));
	return 0 ;
}

#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_XPON_HAL_API_QOS_CA) || defined(TCSUPPORT_CT_PON)
int isDefaultQueue(struct sk_buff *skb)
{
	int queue=0;
	queue = skb->mark & QOS_FILTER_MARK;
	if(queue == 0){
		return 1;
	}
	
	if(gQueueMask & (1 << ((queue >> 4) -1 ))){ 	/* default queue */
		return 0;
	}else{
		return 1;
	}
}

int isWeigt0(struct sk_buff *skb)
{
	int queue=0;
	
	queue = (skb->mark & QOS_FILTER_MARK) >> 4;

	if(queue < 1 || queue >4){	/* wrr queue is from 1 to 4 */
		return 0;
	}

	if(qos_wrr_info[queue]){
		return 0;
	}else{				/* weight is 0 */
		return 1;
	}	
}
#endif

static unchar isPriorityPkt(unchar *cp, struct port_info *pMacInfo)
{
	ushort etherType;
	unchar ipVerLen;
	unchar ipProtocol;
	unchar tcpFlags;
	ushort pppProtocol;
	unchar ipv6_protocol, ipv6_type;
	ushort dport, sport;
	int i ;

	if (cp == NULL)
	{
		QDMA_ERR("ERROR: cp is NULL at isPriorityPkt\n");
		return 0;
	}

    /*atm router mode, no Mac header and vlan*/
    if((gpQdmaPriv->devCfg.macType == MAC_TYPE_ATM) && ((pMacInfo->atm_pppoa == 1) || (pMacInfo->atm_ipoa == 1))) {
		goto ipv4_header;
    }
	/* skip DA and SA mac address */
	cp += 12;
	/* get ether type */
	etherType = *(ushort *) cp;
	/* skip ether type */
	cp += 2;

	/*parse if vlan exists*/
	if (etherType == 0x8100) {
		/*skip 802.1q tag field*/
		cp += 2;
		/*re-parse ether type*/
		etherType = *(ushort *) cp;
		/* skip ether type */
		cp += 2;
	}

	/*check whether PPP packets*/
	if (etherType == 0x8864) {
		/* skip pppoe head */
		cp += 6; 					/* 6: PPPoE header 2: PPP protocol */
		/* get ppp protocol */
		pppProtocol = *(ushort *) cp;
		/* check if LCP protocol */
		if (pppProtocol == 0xc021) {
			return 1;
		/* check if IP protocol */
		} else if (pppProtocol != 0x0021) {
			return 0;
		}
		/* skip ppp protocol */
		cp += 2; 					/* 6: PPPoE header 2: PPP protocol */
	} else if (etherType == 0x8863) {
		return 1;
	/*check whether arp packet*/	
	} else if (etherType == 0x0806) {
		return 1;	
	} 
	else if (etherType == 0x86dd) {
		cp += 6;
		ipv6_protocol = *(unchar*)cp;
		if (ipv6_protocol == 0x3a) {
			cp += 34;
			ipv6_type = *(unchar*)cp;
			if (ipv6_type == 0x87) {
				/* printk("check ipv6 icmp packet.jiffies is %u\n", jiffies); */
				return 1;
			}
		}
	}
	else {
		/* check if ip packet */
		if (etherType != 0x0800) {
			return 0;
		}
	}

ipv4_header:

	/*  frank add 20121220 for pppoa mode */
	pppProtocol = *(ushort *) cp;
	/* check if LCP protocol, for pppoa control packet */
	if (pppProtocol == 0xc021) {
		return 1;
	} else if(pppProtocol == 0x0021) {
		cp += 2; 					/* 6: PPPoE header 2: PPP protocol */
	}

	/* check if it is a ipv4 packet */
	ipVerLen = *cp;
	if ((ipVerLen & 0xf0) != 0x40) {
		return 0;
	}

	/* get ip protocol */
	ipProtocol = *(cp + 9);

	/* check if ICMP/IGMP protocol */
	if ((ipProtocol == 1) || (ipProtocol == 2)) {
		return 1;
	}

#ifdef TCSUPPORT_FE_VIP_ENHANCEMENT	
	/*vip packet , go ring1 */
	if( (ipProtocol == 6) ||  (ipProtocol == 17) )//tcp or udp
	{
		/*get source port and dest port*/
		sport = *(ushort *)(cp + ((ipVerLen & 0x0f) << 2) );
		dport = *(ushort *)(cp + ((ipVerLen & 0x0f) << 2) + 2);
		for( i = 0 ; i < qdma_vip_num ; i++ )
		{
			if( ipProtocol != qdma_vip_info[i].ip_type )/*compare ip protocol*/
				continue ;
			if( qdma_vip_info[i].sport_en && qdma_vip_info[i].dport_en )
			{
				if( qdma_vip_info[i].swap_en )
				{
					if( ((sport == qdma_vip_info[i].sport) && (dport == qdma_vip_info[i].dport))    /*swap compare*/
						|| ((sport == qdma_vip_info[i].dport) && (dport == qdma_vip_info[i].sport)) )
						return 1 ;
				}
				else
				{
					if( (sport == qdma_vip_info[i].sport) && (dport == qdma_vip_info[i].dport) ) /*compare source and dest port*/
						return 1 ;
				}
			}
			else if( qdma_vip_info[i].sport_en && (sport == qdma_vip_info[i].sport) )/*only compare source port*/
			{
				return 1 ;
			}
			else if( qdma_vip_info[i].dport_en && (dport == qdma_vip_info[i].dport) )/*only compare dest port*/
			{
				return 1 ;
			}
		}
	}
#endif

	/* check if TCP protocol */
	if (ipProtocol != 6) {
		return 0;
	}

	/* align to TCP header */
	cp += (ipVerLen & 0x0f) << 2;
	/* get TCP flags */
	tcpFlags = *(cp + 13);
	
	/* check if TCP syn/ack/psh ack/fin ack */
	if ((((tcpFlags & 0x10) == 0x10) || (tcpFlags == 0x02))) {
		return 1;
	}
    
	return 0;
}

int qdma_get_ringIdx(struct sk_buff *skb, struct port_info *pMacInfo)
{
    int ringIdx = 0 ;
 
#ifndef TCSUPPORT_FE_VIP_ENHANCEMENT
    if (priPktChk && (skb->len < priPktChkLen)) {   /*add "len" to check tcp packet*/
#endif
        if (isPriorityPkt(skb->data, pMacInfo)) {
            QDMA_MSG(DBG_MSG, "cat a VIP packet which will send to TX1 and queue7.\n") ;
            ringIdx = RING_IDX_1;
        }
#ifndef TCSUPPORT_FE_VIP_ENHANCEMENT
    }
#endif
    return ringIdx;
}

#ifdef TCSUPPORT_QOS
#ifndef QDMA_LAN
/******************************************************************************
******************************************************************************/
int qdma_qos_mark(struct sk_buff *skb)
{
	int priority = 0;
    
	switch (qos_flag) {
		case QOS_SW_PQ:
			/* PQ mode */
			if ((skb->mark & QOS_FILTER_MARK) == QOS_HH_PRIORITY) {
				priority = 2;
			}
			else if ((skb->mark & QOS_FILTER_MARK) == QOS_H_PRIORITY) {
				priority = 1;
			}
			break;
		case QOS_HW_WRR:
			/* HW WRR mode */			
#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_XPON_HAL_API_QOS_CA) || defined(TCSUPPORT_CT_PON)
			if(isDefaultQueue(skb)){ 	/* default queue */
				priority = 0;
			}else if(isWeigt0(skb)){
				return -EFAULT;
			}else
#endif
			if ((skb->mark & QOS_FILTER_MARK) == QOS_HH_PRIORITY) {
				/* printk("lalala to first queue.\n"); */
				priority = 3;
			}
			else if ((skb->mark & QOS_FILTER_MARK) == QOS_H_PRIORITY) {
				priority = 2;
			}
			else if ((skb->mark & QOS_FILTER_MARK) == QOS_M_PRIORITY) {
				priority = 1;
			}
			else {
				priority = 0;
			}
			break;
		case QOS_HW_PQ:
			/* HW PQ mode */
#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_XPON_HAL_API_QOS_CA) || defined(TCSUPPORT_CT_PON)
			if(isDefaultQueue(skb)){ 	/* default queue */
				priority = 0;
			}else
#endif
			if ((skb->mark & QOS_FILTER_MARK) == QOS_HH_PRIORITY) {
				priority = 3;
			}
			else if ((skb->mark & QOS_FILTER_MARK) == QOS_H_PRIORITY) {
				priority = 2;
			}
			else if ((skb->mark & QOS_FILTER_MARK) == QOS_M_PRIORITY) {
				priority = 1;
			}
			break;
#if defined(TCSUPPORT_CT_HWQOS) || defined(TCSUPPORT_XPON_HAL_API_QOS_CA) 
		case QOS_HW_CAR:			
			if(isDefaultQueue(skb)){		//default queue
				priority = 0;
			}else{
				priority = (skb->mark & QOS_FILTER_MARK)>> 4;
			}
			break;
#endif
		case NULLQOS:/*It's for putting rtp packets to HH priority when qos_flag not be selected as WRR or PQ*/
			if ((skb->mark & QOS_FILTER_MARK) == QOS_HH_PRIORITY) {
				priority = 3;
			}
			break;

		default:
			break;
	}
    
    return priority;
}
#endif

#else
int qdma_qos_remark(struct sk_buff *skb)
{
	int priority = -1 ;
    
    if((skb->mark & QOS_REMARKING_FLAG)){
		priority = (int)((skb->mark & QOS_REMARKING_MASK) >> 1);
	}
        
    return priority;
}
#endif

/******************************************************************************
******************************************************************************/
__IMEM int macResourceLimit(struct sk_buff *skb, uint channel, uint queue)
{
    if((atomic_read(&chnlLimit[channel]) >= channel_limit_threshold) || (atomic_read(&queueLimit[channel][queue]) >= queue_limit_threshold)) {
        gpQdmaPriv->counters.txMacLimitDropCnt++;
        return -EFAULT;
    }
    return 0;
}


/******************************************************************************
******************************************************************************/
__IMEM int qdmaEnableInt(uint base, uint bit)
{
	ulong flags=0 ;
	uint RETRY=3, t=0 ;
	
	spin_lock_irqsave(&gpQdmaPriv->irqLock, flags) ;
	
	while(RETRY--) {
		t = IO_GREG(QDMA_CSR_INT_ENABLE(base)) ;
		if(t == 0) {
			printk("########WARNING: Enable INT: Get INT_ENABLE:%.8x, RETRY:%d\n", t, 3-RETRY) ;
		} else {
			IO_SREG(QDMA_CSR_INT_ENABLE(base), (t|bit)) ;
			break ;
		}
	}

	spin_unlock_irqrestore(&gpQdmaPriv->irqLock, flags) ;
	
	return 0 ;
}

/******************************************************************************
******************************************************************************/
__IMEM int qdmaDisableInt(uint base, uint bit)
{
	ulong flags=0 ;
	uint RETRY=3, t=0 ;
	
	spin_lock_irqsave(&gpQdmaPriv->irqLock, flags) ;

	while(RETRY--) {
		t = IO_GREG(QDMA_CSR_INT_ENABLE(base)) ;
		if(t == 0) {
			printk("########WARNING: Disable INT: Get INT_ENABLE:%.8x, RETRY:%d\n", t, 3-RETRY) ;
		} else {
			IO_SREG(QDMA_CSR_INT_ENABLE(base), (t&(~bit))) ;
			break ;
		}
	}
		
	spin_unlock_irqrestore(&gpQdmaPriv->irqLock, flags) ;
	
	return 0 ;
}

/******************************************************************************
******************************************************************************/
int qdmaSetIntMask(uint base, uint value)
{
	ulong flags=0 ;
	
	spin_lock_irqsave(&gpQdmaPriv->irqLock, flags) ;
	IO_SREG(QDMA_CSR_INT_ENABLE(base), value) ;
	spin_unlock_irqrestore(&gpQdmaPriv->irqLock, flags) ;
	
	return 0 ;
}

/******************************************************************************
******************************************************************************/
__IMEM int qdmaGetIntMask(uint base)
{
	ulong flags=0, value=0 ;
	
	spin_lock_irqsave(&gpQdmaPriv->irqLock, flags);
	value = IO_GREG(QDMA_CSR_INT_ENABLE(base)) ;
	spin_unlock_irqrestore(&gpQdmaPriv->irqLock, flags);
	
	return value ;
}

/******************************************************************************
******************************************************************************/
void qdma_dma_busy_timer(unsigned long arg)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint qdmaGlbCfgVal = qdmaGetGlbCfg(base) ;
	
	printk("%04x: dma busy register is 0x%08x\r\n", dma_busy_round_cnt, qdmaGlbCfgVal);
		
	dma_busy_timer.data = dma_busy_expires ; /* 10 ms */	
	dma_busy_timer.function = qdma_dma_busy_timer;

	if(qdmaGlbCfgVal & GLB_CFG_TX_DMA_BUSY)
		tx_dma_busy_enable_cnt++ ;
	if(qdmaGlbCfgVal & GLB_CFG_RX_DMA_BUSY)
		rx_dma_busy_enable_cnt++ ;	
	
	if(dma_busy_round_cnt--) {
		QDMA_START_TIMER(dma_busy_timer) ;
	} else {
		printk("\r\ntx_busy enable cnt is %d, rx_busy enable cnt is %d\r\n", tx_dma_busy_enable_cnt, rx_dma_busy_enable_cnt);		
	}
}

/******************************************************************************
******************************************************************************/
void qdma_reg_polling_timer(unsigned long arg)
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint qdmaRegVal = qdmaGetOffsetVal(base, reg_polling_offset) ;
	static int i=0;
			
	reg_polling_timer.data = reg_polling_expires ; /* 10 ms */	
	reg_polling_timer.function = qdma_reg_polling_timer;
	
	qdmaRegValBuff[i++] = qdmaRegVal;
	if(--reg_polling_round_cnt) {
		if(i==100) {
			while(i>0) {
				i--;
				printk("\r\n%04x: qdma_reg_value[%x]: 0x%08x", reg_polling_round_cnt+i, QDMA_CSR_OFFSET_ADDR(base, reg_polling_offset), qdmaRegValBuff[i]);
			}
		}
		QDMA_START_TIMER(reg_polling_timer) ;
	} else {
		while(i>0) {
			i--;
			printk("\r\n%04x: qdma_reg_value[%x]: 0x%08x", reg_polling_round_cnt+i, QDMA_CSR_OFFSET_ADDR(base, reg_polling_offset), qdmaRegValBuff[i]);
		}
	}
}

#ifdef TCSUPPORT_DRIVER_API
/***************************************************************
***************************************************************/
static long qdma_ioctl(struct file *filp, uint cmd, void *arg) 
{
	int ret = 0 ;
	struct ECNT_QDMA_Data qdma_data;
	
	if( copy_from_user(&qdma_data, (struct ECNT_QDMA_Data*)arg, sizeof(struct ECNT_QDMA_Data)) ) 
    {
		return -EFAULT;
	}
	
    ret = qdma_operation[qdma_data.function_id](&qdma_data);
	
	if( copy_to_user((struct ECNT_QDMA_Data*)arg, &qdma_data, sizeof(struct ECNT_QDMA_Data)) ) 
    {
		return -EFAULT;
	}
	return ret;
}

/***************************************************************
***************************************************************/
struct file_operations qdma_fops = {
	.owner 			= THIS_MODULE,
	.unlocked_ioctl	= qdma_ioctl,
} ;


/***************************************************************
***************************************************************/
void qdma_dev_destroy(void) 
{   
	dev_t devNo ;
		
	if(gpQdmaDev->pQdmaDev != NULL) {
		cdev_del(gpQdmaDev->pQdmaDev) ;
		gpQdmaDev->pQdmaDev = NULL ;
	}
	
	devNo = MKDEV(COSNT_QDMA_MAJOR_NUN, 0) ;
	unregister_chrdev_region(devNo, 1) ;
	
	QDMA_MSG(DBG_ST, "QDMA Device destroy successful\n") ;
}
#endif

/******************************************************************************
******************************************************************************/
int qdma_dev_init(void) 
{
	uint base = gpQdmaPriv->csrBaseAddr ;
	uint glbCfg=0, intEnable=0 ;
    uint channel=0, queue=0 ;
	int ret = 0 ;
	dev_t devNo ;
	struct ECNT_QDMA_Data qdma_data;
	QDMA_txCngstCfg_t txCngstCfg;
	
	/********************************************
	* init the public data *
	*********************************************/
	memset((char *)qdma_vip_info, 0, sizeof(QDMA_VIP_INFO_T)*QDMA_VIP_PACKET_NUM);
	
	qdmaClearIntStatus(base, 0xFFFFFFFF) ;	
	/********************************************
	* enable/disable the qdma interrupt         *
	*********************************************/
	intEnable = (   INT_STATUS_RX1_COHERENT | INT_STATUS_TX1_COHERENT | 
                    INT_STATUS_RX0_COHERENT | INT_STATUS_TX0_COHERENT | 
                    INT_STATUS_RX_PKT_OVERFLOW | INT_STATUS_IRQ_FULL | 
	                INT_STATUS_NO_RX1_CPU_DSCP | INT_STATUS_NO_RX0_CPU_DSCP | 
	                INT_MASK_RX1_DONE | INT_MASK_TX1_DONE | INT_MASK_RX0_DONE |
	                INT_MASK_TX0_DONE) ;
	qdmaSetIntMask(base, intEnable) ;

	/********************************************
	* Setting the global register               *
	*********************************************/
	glbCfg |= (GLB_CFG_DSCP_BYTE_SWAP | GLB_CFG_PAYLOAD_BYTE_SWAP | GLB_CFG_MSG_WORD_SWAP | ((VAL_BST_32_DWARD<<GLB_CFG_BST_SE_SHIFT)&GLB_CFG_BST_SE_MASK)) ;
#if defined(TCSUPPORT_SLM_EN) && !defined(QDMA_LAN)
	glbCfg |= GLB_CFG_SLM_RELEASE_EN;
#endif
	glbCfg |= (PREFER_TX1_FWD_TX0<<GLB_CFG_DMA_PREFERENCE_SHIFT)&GLB_CFG_DMA_PREFERENCE_MASK;

	if(gpQdmaPriv->irqDepth) {
		glbCfg |= GLB_CFG_IRQ_EN ;
	}
	
#ifdef CONFIG_RX_2B_OFFSET
	glbCfg |= GLB_CFG_RX_2B_OFFSET ;
#endif /* CONFIG_RX_2B_OFFSET */

#ifdef CONFIG_TX_WB_DONE
	glbCfg |= GLB_CFG_TX_WB_DONE ;
#endif /* GLB_CFG_TX_WB_DONE */
	
#if defined(QDMA_LAN)
/* WIFI/VPN offload packets' path in PPE learning stage is QDMA1->PPE->QDM1, which doesn't go through GDM1.
 * In this case, QDMA1 can't get Done signal from GDM1, so won't do Tx recycle for those packets. Therefore,
 * GLB_CFG_TX_IMMEDIATE_DONE needs to be enabled, so that QDMA1 can do Tx recycle for those packets without
 * waiting for GDM1's Done signal.*/
    glbCfg |= GLB_CFG_TX_IMMEDIATE_DONE;
#endif
	
	qdmaSetGlbCfg(base, glbCfg) ;

	/********************************************
	* Misc Setting                               *
	*********************************************/
	/* set tx qos weight_scale as 16Byte, and weight_base by byte */
	qdmaSetTxQosWeightScale16(base) ;
	qdmaSetTxQosWeightByByte(base) ;

	/* disable tx buffer estimate function to avoid qmda counting error issue, 20150912 */
	/* now we should enable this function to avoid channel retire fail issue, 20151117 */
#ifdef QDMA_LAN
	qdmaDisableTxBufEstimate(base);
#else	
	qdmaEnableTxBufEstimate(base) ;
#endif

	/* force enable tx ratemeter */
	qdmaEnableTxRateMeter(base) ;
	qdmaSetTxRateMeterTimeDivisor(base, QDMA_TX_RATE_METER_TIME_DIVISOR_4) ;
	qdmaSetTxRateMeterTimeSlice(base, 4000) ;

	/* tx rate limit default setting */
	qdmaSetTxRateLimitBucketScale(base, QDMA_TRTCM_SCALE_2BYTE) ;
	qdmaSetTxRateLimitTicksel(base, 1000) ;	// unit is 8kbps

	/* disable tx0 total_min_thrh blocking, and disable the other three blocking mode */
	qdmaDisableCngstMaxThrhTx1En(base) ;
	qdmaDisableCngstMinThrhTx1En(base) ;
	qdmaDisableCngstMaxThrhTx0En(base) ;
	qdmaDisableCngstMinThrhTx0En(base) ;

	/* set txq dynamic threshold to default value */
	txCngstCfg.txCngstMode = QDMA_TXCNGST_DYNAMIC_NORMAL ;
	qdma_data.qdma_private.pTxCngstCfg = &txCngstCfg ;
	qdma_set_txq_cngst_auto_config(&qdma_data) ;

	/* enable 3 trigger mode, and set time trigger time as 250us */
	qdmaEnableCngstModeConfigTrig(base) ;
	qdmaEnableCngstModePacketTrig(base) ;
	qdmaEnableCngstModeTimeTrig(base) ;
	qdmaSetTxqDynCngstTicksel(base, 250) ;
	
	/* set dei packet threshold equal 1/2 normal packet */
	qdmaSetDynCngstDeiThrhScale(base, QDMA_DYNCNGST_DEI_THRH_SCALE_1_2) ;

	/* rx ratelimit default setting*/
	qdmaEnableTxRateLimit(base) ;	/* global rate limit default enable */
	qdmaSetRxRateLimitBucketScale(base, QDMA_TRTCM_SCALE_2BYTE) ;
	qdmaSetRxRateLimitByPacket(base) ;
	qdmaSetRxRateLimitTicksel(base, 62500) ;	/* unit is 16 pps */

	/* dbg cntr default setting on index 32~39 */
	qdma_set_dbg_cntr_default_config();
	
    /* mac resource limit init */
    channel_limit_threshold=0x10000;
    queue_limit_threshold=0x10000;
    for(channel=0; channel<CONFIG_QDMA_CHANNEL; channel++) {
        atomic_set(&chnlLimit[channel], 0);
        for(queue=0; queue<CONFIG_QDMA_QUEUE; queue++) {
            atomic_set(&queueLimit[channel][queue], 0);
        }
    }
	
    /* disable qdma rx delay interrupt */
	qdmaSetRxDelayIntVal(base,0);
	
#ifdef TCSUPPORT_DRIVER_API
	devNo = MKDEV(COSNT_QDMA_MAJOR_NUN, 0) ;
	ret = register_chrdev_region(devNo, 1, CONST_QDMA_DEV_NAME) ;
	if(ret < 0) {
		QDMA_ERR("Get the QDMA device number (register_chrdev_region()) failed\n") ;
		return ret ;
	}
	
	gpQdmaDev = &gpQdmaPriv->qdmaDev;
	gpQdmaDev->pQdmaDev = cdev_alloc() ;
	if(gpQdmaDev->pQdmaDev == NULL) {
		QDMA_ERR("Alloc character device (cdev_alloc()) failed\n") ;
		ret = -ENOMEM ;
		goto out ;
	}
	
	cdev_init(gpQdmaDev->pQdmaDev, &qdma_fops) ;
	gpQdmaDev->pQdmaDev->owner = THIS_MODULE ;
	gpQdmaDev->pQdmaDev->ops = &qdma_fops ;
	ret = cdev_add(gpQdmaDev->pQdmaDev, devNo, 1) ;
	if(ret < 0) {
		QDMA_MSG(DBG_ST, "QDMA driver register failed\n") ;
		goto out ;
	} 
	
	QDMA_MSG(DBG_ST, "QDMA initialization successful\n") ;    
	return 0 ; 

out:
	qdma_dev_destroy() ;
	return ret ;
	
#else
	return 0 ; 
#endif
}

