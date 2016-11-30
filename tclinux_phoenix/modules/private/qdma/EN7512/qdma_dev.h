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
#ifndef _QDMA_DEV_H_
#define _QDMA_DEV_H_


/************************************************************************
*                  I N C L U D E S
*************************************************************************
*/
#include <linux/netdevice.h>
#include <linux/foe_hook.h>
#include "qdma_reg.h"
#include "qdma_glb.h"
#include "qdma_api.h"


/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
#ifdef TCSUPPORT_DRIVER_API
#ifdef QDMA_LAN
#define COSNT_QDMA_MAJOR_NUN			119
#define CONST_QDMA_DEV_NAME				"qdma_lan"
#else
#define COSNT_QDMA_MAJOR_NUN			120
#define CONST_QDMA_DEV_NAME				"qdma_wan"
#endif
#endif

#define QDMA_RX_DSCP_MSG_LENS			16
#define QDMA_TX_DSCP_MSG_LENS			8
#define QDMA_HWFWD_DSCP_MSG_LENS		8
#define QDMA_DSCP_MSG_WORD_LENS			4
#define QDMA_DSCP_MSG_TOTAL_WORD_LENS	(QDMA_DSCP_MSG_WORD_LENS+(QDMA_RX_DSCP_MSG_LENS>>2))


/************************************************************************
*                  M A C R O S
*************************************************************************
*/
#define qdmaGetOffsetVal(base, offset)	IO_GREG(QDMA_CSR_OFFSET_ADDR(base, offset))

#define qdmaGetQdmaInfo(base)			IO_GREG(QDMA_CSR_INFO(base))
#define qdmaSetGlbCfg(base, val)		IO_SREG(QDMA_CSR_GLB_CFG(base), val)
#define qdmaGetGlbCfg(base)				IO_GREG(QDMA_CSR_GLB_CFG(base))
#define qdmaIsSetRx2bOffset(base)		(IO_GREG(QDMA_CSR_GLB_CFG(base)) & GLB_CFG_RX_2B_OFFSET)
#define qdmaIsSetMsgWordSwap(base)		(IO_GREG(QDMA_CSR_GLB_CFG(base)) & GLB_CFG_MSG_WORD_SWAP)
#define qdmaIsSetDscpByteSwap(base)		(IO_GREG(QDMA_CSR_GLB_CFG(base)) & GLB_CFG_DSCP_BYTE_SWAP)
#define qdmaIsSetPayloadByteSwap(base)	(IO_GREG(QDMA_CSR_GLB_CFG(base)) & GLB_CFG_PAYLOAD_BYTE_SWAP)
#define qdmaEnableVChnlMapEn(base)		IO_SBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_VCHNL_MAP_EN)
#define qdmaDisableVChnlMapEn(base)		IO_CBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_VCHNL_MAP_EN)
#define qdmaIsVChnlMapEnable(base)		(IO_GREG(QDMA_CSR_GLB_CFG(base)) & GLB_CFG_VCHNL_MAP_EN)
#define qdmaSetVChnlMap32Queue(base)	IO_SBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_VCHNL_MAP_MODE)
#define qdmaSetVChnlMap16Queue(base)	IO_CBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_VCHNL_MAP_MODE)
#define qdmaIsVChnlMap32Queue(base)		(IO_GREG(QDMA_CSR_GLB_CFG(base)) & GLB_CFG_VCHNL_MAP_MODE)
#define qdmaSetLpbkToRX0(base)			IO_CBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_LPBK_RXQ_SEL)
#define qdmaSetLpbkToRX1(base)			IO_SBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_LPBK_RXQ_SEL)
#define qdmaIsEnableSLMRelease(base)	(IO_GREG(QDMA_CSR_GLB_CFG(base)) & GLB_CFG_SLM_RELEASE_EN)
#define qdmaEnableSLMRelease(base)		IO_SBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_SLM_RELEASE_EN)
#define qdmaDisableSLMRelease(base)		IO_CBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_SLM_RELEASE_EN)
#define qdmaIsEnableIrqEn(base)			(IO_GREG(QDMA_CSR_GLB_CFG(base)) & GLB_CFG_IRQ_EN)
#define qdmaEnableIrqEn(base)			IO_SBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_IRQ_EN)
#define qdmaDisableIrqEn(base)			IO_CBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_IRQ_EN)
#define qdmaIsEnableLoopCnt(base)		(IO_GREG(QDMA_CSR_GLB_CFG(base)) & GLB_CFG_LOOPCNT_EN)
#define qdmaEnableLoopCnt(base)			IO_SBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_LOOPCNT_EN)
#define qdmaDisableLoopCnt(base)		IO_CBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_LOOPCNT_EN)
#define qdmaEnableUmacLoopback(base)	IO_SBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_UMAC_LOOPBACK)
#define qdmaDisableUmacLoopback(base)	IO_CBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_UMAC_LOOPBACK)
#define qdmaEnableQdmaLoopback(base)	IO_SBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_QDMA_LOOPBACK)
#define qdmaDisableQdmaLoopback(base)	IO_CBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_QDMA_LOOPBACK)
#define qdmaIsSetCheckDone(base)		(IO_GREG(QDMA_CSR_GLB_CFG(base)) & GLB_CFG_CHECK_DONE)	
#define qdmaIsSetTxWbDone(base)			(IO_GREG(QDMA_CSR_GLB_CFG(base)) & GLB_CFG_TX_WB_DONE)	
#define qdmaSetBurstSize(base, val)		IO_SMASK(QDMA_CSR_GLB_CFG(base), GLB_CFG_BST_SE_MASK, GLB_CFG_BST_SE_SHIFT, val)
#define qdmaGetBurstSize(base)			IO_GMASK(QDMA_CSR_GLB_CFG(base), GLB_CFG_BST_SE_MASK, GLB_CFG_BST_SE_SHIFT)
#define qdmaIsRxDmaBusy(base)			(IO_GREG(QDMA_CSR_GLB_CFG(base)) & GLB_CFG_RX_DMA_BUSY)
#define qdmaIsSetRxDmaEnable(base)		(IO_GREG(QDMA_CSR_GLB_CFG(base)) & GLB_CFG_RX_DMA_EN)
#define qdmaEnableRxDma(base)			IO_SBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_RX_DMA_EN)
#define qdmaDisableRxDma(base)			IO_CBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_RX_DMA_EN)
#define qdmaIsTxDmaBusy(base)			(IO_GREG(QDMA_CSR_GLB_CFG(base)) & GLB_CFG_TX_DMA_BUSY)
#define qdmaIsSetTxDmaEnable(base)		(IO_GREG(QDMA_CSR_GLB_CFG(base)) & GLB_CFG_TX_DMA_EN)
#define qdmaEnableTxDma(base)			IO_SBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_TX_DMA_EN)
#define qdmaDisableTxDma(base)			IO_CBITS(QDMA_CSR_GLB_CFG(base), GLB_CFG_TX_DMA_EN)

#define qdmaSetTxDscpBase(base, idx, val)	IO_SREG(QDMA_CSR_TX_DSCP_BASE(base, idx), val)
#define qdmaGetTxDscpBase(base, idx)		IO_GREG(QDMA_CSR_TX_DSCP_BASE(base, idx))
#define qdmaSetRxDscpBase(base, idx, val)	IO_SREG(QDMA_CSR_RX_DSCP_BASE(base, idx), val)
#define qdmaGetRxDscpBase(base, idx)		IO_GREG(QDMA_CSR_RX_DSCP_BASE(base, idx))
#define qdmaSetTxCpuIdx(base, idx, val)		IO_SMASK(QDMA_CSR_TX_CPU_IDX(base, idx), TX_CPU_IDX_MASK, TX_CPU_IDX_SHIFT, val)
#define qdmaGetTxCpuIdx(base, idx)			IO_GMASK(QDMA_CSR_TX_CPU_IDX(base, idx), TX_CPU_IDX_MASK, TX_CPU_IDX_SHIFT)
#define qdmaSetTxDmaIdx(base, idx, val)		IO_SMASK(QDMA_CSR_TX_DMA_IDX(base, idx), TX_DMA_IDX_MASK, TX_DMA_IDX_SHIFT, val)
#define qdmaGetTxDmaIdx(base, idx)			IO_GMASK(QDMA_CSR_TX_DMA_IDX(base, idx), TX_DMA_IDX_MASK, TX_DMA_IDX_SHIFT)
#define qdmaSetRxCpuIdx(base, idx, val)		IO_SMASK(QDMA_CSR_RX_CPU_IDX(base, idx), RX_CPU_IDX_MASK, RX_CPU_IDX_SHIFT, val)
#define qdmaGetRxCpuIdx(base, idx)			IO_GMASK(QDMA_CSR_RX_CPU_IDX(base, idx), RX_CPU_IDX_MASK, RX_CPU_IDX_SHIFT)
#define qdmaSetRxDmaIdx(base, idx, val)		IO_SMASK(QDMA_CSR_RX_DMA_IDX(base, idx), RX_DMA_IDX_MASK, RX_DMA_IDX_SHIFT, val)
#define qdmaGetRxDmaIdx(base, idx)			IO_GMASK(QDMA_CSR_RX_DMA_IDX(base, idx), RX_DMA_IDX_MASK, RX_DMA_IDX_SHIFT)

#define qdmaSetHwDscpBase(base, val)	IO_SREG(QDMA_CSR_HWFWD_DSCP_BASE(base), val)
#define qdmaGetHwDscpBase(base)			IO_GREG(QDMA_CSR_HWFWD_DSCP_BASE(base))
#define qdmaSetHwBuffBase(base, val)	IO_SREG(QDMA_CSR_HWFWD_BUFF_BASE(base), val)
#define qdmaGetHwBuffBase(base)			IO_GREG(QDMA_CSR_HWFWD_BUFF_BASE(base))
#define qdmaSetHwPayloadSize(base, val)	IO_SMASK(QDMA_CSR_HWFWD_DSCP_CFG(base), HWFWD_PAYLOAD_SIZE_MASK, HWFWD_PAYLOAD_SIZE_SHIFT, val)
#define qdmaGetHwPayloadSize(base)		IO_GMASK(QDMA_CSR_HWFWD_DSCP_CFG(base), HWFWD_PAYLOAD_SIZE_MASK, HWFWD_PAYLOAD_SIZE_SHIFT)
#define qdmaSetHwLowThrshld(base, val)	IO_SMASK(QDMA_CSR_HWFWD_DSCP_CFG(base), HWFWD_DSCP_LOW_THRSHLD_MASK, HWFWD_DSCP_LOW_THRSHLD_SHIFT, val)
#define qdmaGetHwLowThrshld(base)		IO_GMASK(QDMA_CSR_HWFWD_DSCP_CFG(base), HWFWD_DSCP_LOW_THRSHLD_MASK, HWFWD_DSCP_LOW_THRSHLD_SHIFT)

#define qdmaSetHWInitStart(base)		IO_SBITS(QDMA_CSR_LMGR_INIT_CFG(base), LMGR_INIT_START)
#define qdmaGetHWInitStart(base)		(IO_GREG(QDMA_CSR_LMGR_INIT_CFG(base)) & LMGR_INIT_START)
#define qdmaSetHwDscpNum(base, val)		IO_SMASK(QDMA_CSR_LMGR_INIT_CFG(base), HWFWD_DSCP_NUM_MASK, HWFWD_DSCP_NUM_SHIFT, val)
#define qdmaGetHwDscpNum(base)			IO_GMASK(QDMA_CSR_LMGR_INIT_CFG(base), HWFWD_DSCP_NUM_MASK, HWFWD_DSCP_NUM_SHIFT)
#define qdmaEnablePktSizeOverHead(base)		IO_SBITS(QDMA_CSR_LMGR_INIT_CFG(base), HWFWD_PKTSIZE_OVERHEAD_EN)
#define qdmaDisablePktSizeOverHead(base)	IO_CBITS(QDMA_CSR_LMGR_INIT_CFG(base), HWFWD_PKTSIZE_OVERHEAD_EN)
#define qdmaIsPktSizeOverHeadEnable(base)	(IO_GREG(QDMA_CSR_LMGR_INIT_CFG(base)) & HWFWD_PKTSIZE_OVERHEAD_EN)
#define qdmaSetPktSizeOverHeadVal(base, val)	IO_SMASK(QDMA_CSR_LMGR_INIT_CFG(base), HWFWD_PKTSIZE_OVERHEAD_VALUE_MASK, HWFWD_PKTSIZE_OVERHEAD_VALUE_SHIFT, val)
#define qdmaGetPktSizeOverHeadVal(base)			IO_GMASK(QDMA_CSR_LMGR_INIT_CFG(base), HWFWD_PKTSIZE_OVERHEAD_VALUE_MASK, HWFWD_PKTSIZE_OVERHEAD_VALUE_SHIFT)

#define qdmaSetTxDelayIntCfg(base, idx, val)	IO_SMASK(QDMA_CSR_TX_DELAY_INT_CFG(base), DLY_INT_TXDLY_MASK(idx), DLY_INT_TXDLY_SHIFT(idx), val)
#define qdmaSetTxDelayIntVal(base, val)			IO_SREG(QDMA_CSR_TX_DELAY_INT_CFG(base), val)
#define qdmaGetTxDelayIntCfg(base)				IO_GREG(QDMA_CSR_TX_DELAY_INT_CFG(base))
#define qdmaSetRxDelayIntCfg(base, idx, val)	IO_SMASK(QDMA_CSR_RX_DELAY_INT_CFG(base), DLY_INT_RXDLY_MASK(idx), DLY_INT_RXDLY_SHIFT(idx), val)
#define qdmaSetRxDelayIntVal(base, val)			IO_SREG(QDMA_CSR_RX_DELAY_INT_CFG(base), val)
#define qdmaGetRxDelayIntCfg(base)				IO_GREG(QDMA_CSR_RX_DELAY_INT_CFG(base))
//#define qdmaClearIntStatus(base, bit)	IO_SBITS(QDMA_CSR_INT_STATUS(base), bit)
//#define qdmaSetIntStatus(base, val)		IO_SREG(QDMA_CSR_INT_STATUS(base), val)
#define qdmaClearIntStatus(base, val)	IO_SREG(QDMA_CSR_INT_STATUS(base), val)
#define qdmaGetIntStatus(base)			IO_GREG(QDMA_CSR_INT_STATUS(base))
#define qdmaSetIrqBase(base, val)		IO_SREG(QDMA_CSR_IRQ_BASE(base), val)
#define qdmaGetIrqBase(base)			IO_GREG(QDMA_CSR_IRQ_BASE(base))
#define qdmaSetIrqConfig(base, val)		IO_SREG(QDMA_CSR_IRQ_CFG(base), val)
#define qdmaGetIrqConfig(base)			IO_GREG(QDMA_CSR_IRQ_CFG(base))
#define qdmaSetIrqThreshold(base, val)	IO_SMASK(QDMA_CSR_IRQ_CFG(base), IRQ_CFG_THRESHOLD_MASK, IRQ_CFG_THRESHOLD_SHIFT, val)
#define qdmaGetIrqThreshold(base)		IO_GMASK(QDMA_CSR_IRQ_CFG(base), IRQ_CFG_THRESHOLD_MASK, IRQ_CFG_THRESHOLD_SHIFT)
#define qdmaSetIrqDepth(base, val)		IO_SMASK(QDMA_CSR_IRQ_CFG(base), IRQ_CFG_DEPTH_MASK, IRQ_CFG_DEPTH_SHIFT, val)
#define qdmaGetIrqDepth(base)			IO_GMASK(QDMA_CSR_IRQ_CFG(base), IRQ_CFG_DEPTH_MASK, IRQ_CFG_DEPTH_SHIFT)
#define qdmaSetIrqClearLen(base, val)	IO_SMASK(QDMA_CSR_IRQ_CLEAR_LEN(base), IRQ_CLEAR_LEN_MASK, IRQ_CLEAR_LEN_SHIFT, val)
#define qdmaGetIrqClearLen(base)		IO_GMASK(QDMA_CSR_IRQ_CLEAR_LEN(base), IRQ_CLEAR_LEN_MASK, IRQ_CLEAR_LEN_SHIFT)
#define qdmaGetIrqStatus(base)			IO_GREG(QDMA_CSR_IRQ_STATUS(base))
#define qdmaGetIrqEntryLen(base)		IO_GMASK(QDMA_CSR_IRQ_STATUS(base), IRQ_STATUS_ENTRY_LEN_MASK, IRQ_STATUS_ENTRY_LEN_SHIFT)
#define qdmaGetIrqHeadIdx(base)			IO_GMASK(QDMA_CSR_IRQ_STATUS(base), IRQ_STATUS_HEAD_IDX_MASK, IRQ_STATUS_HEAD_IDX_SHIFT)
#define qdmaSetIrqPtime(base, val)		IO_SMASK(QDMA_CSR_IRQ_PTIME(base), IRQ_PTIME_MASK, IRQ_PTIME_SHIFT, val)
#define qdmaGetIrqPtime(base)			IO_GMASK(QDMA_CSR_IRQ_PTIME(base), IRQ_PTIME_MASK, IRQ_PTIME_SHIFT)

#define qdmaSetTxQosWeightByPacket(base)	IO_CBITS(QDMA_CSR_TXWRR_MODE_CFG(base), TXWRR_WEIGHT_BASE)
#define qdmaSetTxQosWeightByByte(base)		IO_SBITS(QDMA_CSR_TXWRR_MODE_CFG(base), TXWRR_WEIGHT_BASE)
#define qdmaIsTxQosWeightByByte(base)		(IO_GREG(QDMA_CSR_TXWRR_MODE_CFG(base)) & TXWRR_WEIGHT_BASE)
#define qdmaSetTxQosWeightScale64(base)		IO_CBITS(QDMA_CSR_TXWRR_MODE_CFG(base), TXWRR_WEIGHT_SCALE)
#define qdmaSetTxQosWeightScale16(base)		IO_SBITS(QDMA_CSR_TXWRR_MODE_CFG(base), TXWRR_WEIGHT_SCALE)
#define qdmaIsTxQosWeightScale16(base)		(IO_GREG(QDMA_CSR_TXWRR_MODE_CFG(base)) & TXWRR_WEIGHT_SCALE)

#define qdmaEnablePrefetchOverDrag(base)	IO_SBITS(QDMA_CSR_DMA_PREFETCH_CFG(base), TXBUF_PREFETCH_OVERDRAG_EN)
#define qdmaDisablePrefetchOverDrag(base)	IO_CBITS(QDMA_CSR_DMA_PREFETCH_CFG(base), TXBUF_PREFETCH_OVERDRAG_EN)
#define qdmaSetPrefetchOverDragCnt(base, val)	IO_SMASK(QDMA_CSR_DMA_PREFETCH_CFG(base), TXBUF_PREFETCH_OVERDRAG_CNT_MASK, TXBUF_PREFETCH_OVERDRAG_CNT_SHIFT, val)
#define qdmaGetPrefetchOverDragCnt(base, val)	IO_GMASK(QDMA_CSR_DMA_PREFETCH_CFG(base), TXBUF_PREFETCH_OVERDRAG_CNT_MASK, TXBUF_PREFETCH_OVERDRAG_CNT_SHIFT, val)

#define qdmaEnableTxDmaPrefetch(base)		IO_SBITS(QDMA_CSR_PSE_BUF_USAGE_CFG(base), PSE_BUF_PREFETCH_EN)
#define qdmaDisableTxDmaPrefetch(base)		IO_CBITS(QDMA_CSR_PSE_BUF_USAGE_CFG(base), PSE_BUF_PREFETCH_EN)
#define qdmaEnableTxBufCtrl(base)			IO_SBITS(QDMA_CSR_PSE_BUF_USAGE_CFG(base), PSE_BUF_CTRL_EN)
#define qdmaDisableTxBufCtrl(base)			IO_CBITS(QDMA_CSR_PSE_BUF_USAGE_CFG(base), PSE_BUF_CTRL_EN)
#define qdmaIsTxBufCtrlEnable(base)			(IO_GREG(QDMA_CSR_PSE_BUF_USAGE_CFG(base)) & PSE_BUF_CTRL_EN)
#define qdmaEnableTxBufEstimate(base)		IO_SBITS(QDMA_CSR_PSE_BUF_USAGE_CFG(base), PSE_BUF_ESTIMATE_EN)
#define qdmaDisableTxBufEstimate(base)		IO_CBITS(QDMA_CSR_PSE_BUF_USAGE_CFG(base), PSE_BUF_ESTIMATE_EN)
#define qdmaSetTxBufChnnelThreshold(base, val)	IO_SMASK(QDMA_CSR_PSE_BUF_USAGE_CFG(base), TXBUF_CHNL_THRSHLD_MASK, TXBUF_CHNL_THRSHLD_SHIFT, val)
#define qdmaGetTxBufChnnelThreshold(base)		IO_GMASK(QDMA_CSR_PSE_BUF_USAGE_CFG(base), TXBUF_CHNL_THRSHLD_MASK, TXBUF_CHNL_THRSHLD_SHIFT)
#define qdmaSetTxBufTotalThreshold(base, val)	IO_SMASK(QDMA_CSR_PSE_BUF_USAGE_CFG(base), TXBUF_TOTAL_THRSHLD_MASK, TXBUF_TOTAL_THRSHLD_SHIFT, val)
#define qdmaGetTxBufTotalThreshold(base)		IO_GMASK(QDMA_CSR_PSE_BUF_USAGE_CFG(base), TXBUF_TOTAL_THRSHLD_MASK, TXBUF_TOTAL_THRSHLD_SHIFT)

#define qdmaEnableTxRateMeter(base)			IO_SBITS(QDMA_CSR_EGRESS_RATEMETER_CFG(base), EGRESS_RATEMETER_EN)
#define qdmaDisableTxRateMeter(base)		IO_CBITS(QDMA_CSR_EGRESS_RATEMETER_CFG(base), EGRESS_RATEMETER_EN)
#define qdmaIsTxRateMeterEnable(base)		(IO_GREG(QDMA_CSR_EGRESS_RATEMETER_CFG(base)) & EGRESS_RATEMETER_EN)
#define qdmaEnableTxRateMeterPeakRate(base)			IO_SBITS(QDMA_CSR_EGRESS_RATEMETER_CFG(base), EGRESS_RATEMETER_PEEKRATE_EN)
#define qdmaDisableTxRateMeterPeakRate(base)		IO_CBITS(QDMA_CSR_EGRESS_RATEMETER_CFG(base), EGRESS_RATEMETER_PEEKRATE_EN)
#define qdmaIsTxRateMeterPeakRateEnable(base)		(IO_GREG(QDMA_CSR_EGRESS_RATEMETER_CFG(base)) & EGRESS_RATEMETER_PEEKRATE_EN)
#define qdmaSetTxPeekRateDuration(base, val)		IO_SMASK(QDMA_CSR_EGRESS_RATEMETER_CFG(base), EGRESS_RATEMETER_PEEKRATE_DURATION_MASK, EGRESS_RATEMETER_PEEKRATE_DURATION_SHIFT, val)
#define qdmaGetTxPeekRateDuration(base)			IO_GMASK(QDMA_CSR_EGRESS_RATEMETER_CFG(base), EGRESS_RATEMETER_PEEKRATE_DURATION_MASK, EGRESS_RATEMETER_PEEKRATE_DURATION_SHIFT)
#define qdmaSetTxRateMeterTimeDivisor(base, val)	IO_SMASK(QDMA_CSR_EGRESS_RATEMETER_CFG(base), EGRESS_RATEMETER_TIMEDIVESEL_MASK, EGRESS_RATEMETER_TIMEDIVESEL_SHIFT, val)
#define qdmaGetTxRateMeterTimeDivisor(base)			IO_GMASK(QDMA_CSR_EGRESS_RATEMETER_CFG(base), EGRESS_RATEMETER_TIMEDIVESEL_MASK, EGRESS_RATEMETER_TIMEDIVESEL_SHIFT)
#define qdmaSetTxRateMeterTimeSlice(base, val)		IO_SMASK(QDMA_CSR_EGRESS_RATEMETER_CFG(base), EGRESS_RATEMETER_TIMESLICE_MASK, EGRESS_RATEMETER_TIMESLICE_SHIFT, val)
#define qdmaGetTxRateMeterTimeSlice(base)			IO_GMASK(QDMA_CSR_EGRESS_RATEMETER_CFG(base), EGRESS_RATEMETER_TIMESLICE_MASK, EGRESS_RATEMETER_TIMESLICE_SHIFT)

#define qdmaEnableTxRateLimit(base)			IO_SBITS(QDMA_CSR_EGRESS_RATELIMIT_CFG(base), EGRESS_RATELIMIT_EN)
#define qdmaDisableTxRateLimit(base)		IO_CBITS(QDMA_CSR_EGRESS_RATELIMIT_CFG(base), EGRESS_RATELIMIT_EN)
#define qdmaIsTxRateLimitEnable(base)		(IO_GREG(QDMA_CSR_EGRESS_RATELIMIT_CFG(base)) & EGRESS_RATELIMIT_EN)
#define qdmaSetTxRateLimitBucketScale(base, val)	IO_SMASK(QDMA_CSR_EGRESS_RATELIMIT_CFG(base), EGRESS_RATELIMIT_BUCKETSCALE_MASK, EGRESS_RATELIMIT_BUCKETSCALE_SHIFT, val)
#define qdmaGetTxRateLimitBucketScale(base)			IO_GMASK(QDMA_CSR_EGRESS_RATELIMIT_CFG(base), EGRESS_RATELIMIT_BUCKETSCALE_MASK, EGRESS_RATELIMIT_BUCKETSCALE_SHIFT)
#define qdmaSetTxRateLimitTicksel(base, val)		IO_SMASK(QDMA_CSR_EGRESS_RATELIMIT_CFG(base), EGRESS_RATELIMIT_TICKSEL_MASK, EGRESS_RATELIMIT_TICKSEL_SHIFT, val)
#define qdmaGetTxRateLimitTicksel(base)				IO_GMASK(QDMA_CSR_EGRESS_RATELIMIT_CFG(base), EGRESS_RATELIMIT_TICKSEL_MASK, EGRESS_RATELIMIT_TICKSEL_SHIFT)


#define qdmaEnableTxqCngstDrop(base)		IO_SBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_CNGST_DROP_EN)
#define qdmaDisableTxqCngstDrop(base)		IO_CBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_CNGST_DROP_EN)
#define qdmaIsTxqCngstDropEnable(base)		(IO_GREG(QDMA_CSR_TXQ_CNGST_CFG(base)) & TXQ_CNGST_DROP_EN)
#define qdmaEnableTxqCngstDeiDrop(base)		IO_SBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_CNGST_DEI_DROP_EN)
#define qdmaDisableTxqCngstDeiDrop(base)	IO_CBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_CNGST_DEI_DROP_EN)
#define qdmaIsTxqCngstDeiDropEnable(base)	(IO_GREG(QDMA_CSR_TXQ_CNGST_CFG(base)) & TXQ_CNGST_DEI_DROP_EN)
#define qdmaEnableTxqDynCngstEn(base)		IO_SBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_EN)
#define qdmaDisableTxqDynCngstEn(base)		IO_CBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_EN)
#define qdmaIsTxqDynCngstEnable(base)		(IO_GREG(QDMA_CSR_TXQ_CNGST_CFG(base)) & TXQ_DYN_CNGSTCTL_EN)

#define qdmaEnableCngstMaxThrhTx1En(base)		IO_SBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_MAX_THRH_TXR1_EN)
#define qdmaDisableCngstMaxThrhTx1En(base)		IO_CBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_MAX_THRH_TXR1_EN)
#define qdmaIsCngstMaxThrhTx1Enable(base)		(IO_GREG(QDMA_CSR_TXQ_CNGST_CFG(base)) & TXQ_DYN_CNGSTCTL_MAX_THRH_TXR1_EN)
#define qdmaEnablengstMinThrhTx1En(base)		IO_SBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_MIN_THRH_TXR1_EN)
#define qdmaDisableCngstMinThrhTx1En(base)		IO_CBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_MIN_THRH_TXR1_EN)
#define qdmaIsCngstMinThrhTx1Enable(base)		(IO_GREG(QDMA_CSR_TXQ_CNGST_CFG(base)) & TXQ_DYN_CNGSTCTL_MIN_THRH_TXR1_EN)
#define qdmaEnableCngstMaxThrhTx0En(base)		IO_SBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_MAX_THRH_TXR0_EN)
#define qdmaDisableCngstMaxThrhTx0En(base)		IO_CBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_MAX_THRH_TXR0_EN)
#define qdmaIsCngstMaxThrhTx0Enable(base)		(IO_GREG(QDMA_CSR_TXQ_CNGST_CFG(base)) & TXQ_DYN_CNGSTCTL_MAX_THRH_TXR0_EN)
#define qdmaEnableCngstMinThrhTx0En(base)		IO_SBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_MIN_THRH_TXR0_EN)
#define qdmaDisableCngstMinThrhTx0En(base)		IO_CBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_MIN_THRH_TXR0_EN)
#define qdmaIsCngstMinThrhTx0Enable(base)		(IO_GREG(QDMA_CSR_TXQ_CNGST_CFG(base)) & TXQ_DYN_CNGSTCTL_MIN_THRH_TXR0_EN)
#define qdmaSetDynCngstDeiThrhScale(base, val)	IO_SMASK(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_DEI_THR_SCALE_MASK, TXQ_DYN_CNGSTCTL_DEI_THR_SCALE_SHIFT, val)
#define qdmaGetDynCngstDeiThrhScale(base)		IO_GMASK(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_DEI_THR_SCALE_MASK, TXQ_DYN_CNGSTCTL_DEI_THR_SCALE_SHIFT)
#define qdmaSetTxPeekRateMargin(base, val)		IO_SMASK(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_THR_MARGIN_MASK, TXQ_DYN_CNGSTCTL_THR_MARGIN_SHIFT, val)
#define qdmaGetTxPeekRateMargin(base)			IO_GMASK(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_THR_MARGIN_MASK, TXQ_DYN_CNGSTCTL_THR_MARGIN_SHIFT)

#define qdmaEnableCngstModeConfigTrig(base)		IO_SBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_MODE_CONFIG_TRIGGER)
#define qdmaDisableCngstModeConfigTrig(base)	IO_CBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_MODE_CONFIG_TRIGGER)
#define qdmaIsCngstModeConfigTrigEnable(base)	(IO_GREG(QDMA_CSR_TXQ_CNGST_CFG(base)) & TXQ_DYN_CNGSTCTL_MODE_CONFIG_TRIGGER)
#define qdmaEnableCngstModePacketTrig(base)		IO_SBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_MODE_PACKET_TRIGGER)
#define qdmaDisableCngstModePacketTrig(base)	IO_CBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_MODE_PACKET_TRIGGER)
#define qdmaIsCngstModePacketgTrigEnable(base)	(IO_GREG(QDMA_CSR_TXQ_CNGST_CFG(base)) & TXQ_DYN_CNGSTCTL_MODE_PACKET_TRIGGER)
#define qdmaEnableCngstModeTimeTrig(base)		IO_SBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_MODE_TIME_TRIGGER)
#define qdmaDisableCngstModeTimeTrig(base)		IO_CBITS(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_MODE_TIME_TRIGGER)
#define qdmaIsCngstModeTimeTrigEnable(base)		(IO_GREG(QDMA_CSR_TXQ_CNGST_CFG(base)) & TXQ_DYN_CNGSTCTL_MODE_TIME_TRIGGER)
#define qdmaSetTxqDynCngstTicksel(base, val)	IO_SMASK(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_TICKSEL_MASK, TXQ_DYN_CNGSTCTL_TICKSEL_SHIFT, val)
#define qdmaGetTxqDynCngstTicksel(base)			IO_GMASK(QDMA_CSR_TXQ_CNGST_CFG(base), TXQ_DYN_CNGSTCTL_TICKSEL_MASK, TXQ_DYN_CNGSTCTL_TICKSEL_SHIFT)


#define qdmaSetTxqDynTotalMaxThrh(base, val)	IO_SMASK(QDMA_CSR_TXQ_DYN_TOTALTHR(base), TXQ_CNGST_TOTAL_MAX_THR_MASK, TXQ_CNGST_TOTAL_MAX_THR_SHIFT, val)
#define qdmaGetTxqDynTotalMaxThrh(base)			IO_GMASK(QDMA_CSR_TXQ_DYN_TOTALTHR(base), TXQ_CNGST_TOTAL_MAX_THR_MASK, TXQ_CNGST_TOTAL_MAX_THR_SHIFT)
#define qdmaSetTxqDynTotalMinThrh(base, val)	IO_SMASK(QDMA_CSR_TXQ_DYN_TOTALTHR(base), TXQ_CNGST_TOTAL_MIN_THR_MASK, TXQ_CNGST_TOTAL_MIN_THR_SHIFT, val)
#define qdmaGetTxqDynTotalMinThrh(base)			IO_GMASK(QDMA_CSR_TXQ_DYN_TOTALTHR(base), TXQ_CNGST_TOTAL_MIN_THR_MASK, TXQ_CNGST_TOTAL_MIN_THR_SHIFT)

#define qdmaSetTxqDynChnlMaxThrh(base, val)		IO_SMASK(QDMA_CSR_TXQ_DYN_CHNLTHR_CFG(base), TXQ_CNGST_CHNL_MAX_THR_MASK, TXQ_CNGST_CHNL_MAX_THR_SHIFT, val)
#define qdmaGetTxqDynChnlMaxThrh(base)			IO_GMASK(QDMA_CSR_TXQ_DYN_CHNLTHR_CFG(base), TXQ_CNGST_CHNL_MAX_THR_MASK, TXQ_CNGST_CHNL_MAX_THR_SHIFT)
#define qdmaSetTxqDynChnlMinThrh(base, val)		IO_SMASK(QDMA_CSR_TXQ_DYN_CHNLTHR_CFG(base), TXQ_CNGST_CHNL_MIN_THR_MASK, TXQ_CNGST_CHNL_MIN_THR_SHIFT, val)
#define qdmaGetTxqDynChnlMinThrh(base)			IO_GMASK(QDMA_CSR_TXQ_DYN_CHNLTHR_CFG(base), TXQ_CNGST_CHNL_MIN_THR_MASK, TXQ_CNGST_CHNL_MIN_THR_SHIFT)

#define qdmaSetTxqDynQueueMaxThrh(base, val)	IO_SMASK(QDMA_CSR_TXQ_DYN_QUEUETHR_CFG(base), TXQ_CNGST_QUEUE_MAX_THR_MASK, TXQ_CNGST_QUEUE_MAX_THR_SHIFT, val)
#define qdmaGetTxqDynQueueMaxThrh(base)			IO_GMASK(QDMA_CSR_TXQ_DYN_QUEUETHR_CFG(base), TXQ_CNGST_QUEUE_MAX_THR_MASK, TXQ_CNGST_QUEUE_MAX_THR_SHIFT)
#define qdmaSetTxqDynQueueMinThrh(base, val)	IO_SMASK(QDMA_CSR_TXQ_DYN_QUEUETHR_CFG(base), TXQ_CNGST_QUEUE_MIN_THR_MASK, TXQ_CNGST_QUEUE_MIN_THR_SHIFT, val)
#define qdmaGetTxqDynQueueMinThrh(base)			IO_GMASK(QDMA_CSR_TXQ_DYN_QUEUETHR_CFG(base), TXQ_CNGST_QUEUE_MIN_THR_MASK, TXQ_CNGST_QUEUE_MIN_THR_SHIFT)

#define qdmaSetTxqStaticQueueDeiThrh(base, idx, val)		IO_SMASK(QDMA_CSR_STATIC_QUEUE_THR(base, idx), TXQ_STATIC_QUEUE_DEI_THR_MASK, TXQ_STATIC_QUEUE_DEI_THR_SHIFT, val)
#define qdmaGetTxqStaticQueueDeiThrh(base, idx)				IO_GMASK(QDMA_CSR_STATIC_QUEUE_THR(base, idx), TXQ_STATIC_QUEUE_DEI_THR_MASK, TXQ_STATIC_QUEUE_DEI_THR_SHIFT)
#define qdmaSetTxqStaticQueueNormalThrh(base, idx, val)		IO_SMASK(QDMA_CSR_STATIC_QUEUE_THR(base, idx), TXQ_STATIC_QUEUE_NORM_THR_MASK, TXQ_STATIC_QUEUE_NORM_THR_SHIFT, val)
#define qdmaGetTxqStaticQueueNormalThrh(base, idx)			IO_GMASK(QDMA_CSR_STATIC_QUEUE_THR(base, idx), TXQ_STATIC_QUEUE_NORM_THR_MASK, TXQ_STATIC_QUEUE_NORM_THR_SHIFT)

#define qdmaEnableTxqCngstQueueNonblocking(base, idx)		IO_CBITS(QDMA_CSR_TXQ_CNGST_QUEUE_NONBLOCKING_CFG(base), TXQ_CNGST_QUEUE_NONBLOCKING_EN(idx))
#define qdmaDisableTxqCngstQueueNonblocking(base, idx)		IO_SBITS(QDMA_CSR_TXQ_CNGST_QUEUE_NONBLOCKING_CFG(base), TXQ_CNGST_QUEUE_NONBLOCKING_EN(idx))
#define qdmaIsTxqCngstQueueNonblockingEnable(base, idx)	       (IO_GREG(QDMA_CSR_TXQ_CNGST_QUEUE_NONBLOCKING_CFG(base)) & TXQ_CNGST_QUEUE_NONBLOCKING_EN(idx))

#define qdmaEnableTxqCngstChannelNonblocking(base, idx)		IO_CBITS(QDMA_CSR_TXQ_CNGST_CHANNEL_NONBLOCKING_CFG(base), TXQ_CNGST_CHANNEL_NONBLOCKING_EN(idx))
#define qdmaDisableTxqCngstChannelNonblocking(base, idx)		IO_SBITS(QDMA_CSR_TXQ_CNGST_CHANNEL_NONBLOCKING_CFG(base), TXQ_CNGST_CHANNEL_NONBLOCKING_EN(idx))
#define qdmaIsTxqCngstChannelNonblockingEnable(base, idx)	       (IO_GREG(QDMA_CSR_TXQ_CNGST_CHANNEL_NONBLOCKING_CFG(base)) & TXQ_CNGST_CHANNEL_NONBLOCKING_EN(idx))

#define qdmaLmgrFreeCount(base)				IO_GMASK(QDMA_CSR_DBG_LMGR_STATUS(base), DBG_LMGR_FREE_CNT_MASK, DBG_LMGR_FREE_CNT_SHIFT)
#define qdmaLmgrTotalBufUsage(base)			IO_GMASK(QDMA_CSR_DBG_LMGR_USAGE(base), DBG_LMGR_TOTALBUF_USAGE_MASK, DBG_LMGR_TOTALBUF_USAGE_SHIFT)

#define qdmaSetRxRingSize(base, idx, val)	IO_SMASK(QDMA_CSR_RX_RING_CFG(base), RX_RING_SIZE_MASK(idx), RX_RING_SIZE_SHIFT(idx), val)
#define qdmaGetRxRingSize(base, idx)		IO_GMASK(QDMA_CSR_RX_RING_CFG(base), RX_RING_SIZE_MASK(idx), RX_RING_SIZE_SHIFT(idx))
#define qdmaSetRxRingThrh(base, idx, val)	IO_SMASK(QDMA_CSR_RX_RING_THR(base), RX_RING_LOW_THR_MASK(idx), RX_RING_LOW_THR_SHIFT(idx), val)
#define qdmaGetRxRingThrh(base, idx)		IO_GMASK(QDMA_CSR_RX_RING_THR(base), RX_RING_LOW_THR_MASK(idx), RX_RING_LOW_THR_SHIFT(idx))

#define qdmaEnableRxDrop(base)				IO_SBITS(QDMA_CSR_RX_PROTECT_CFG(base), RX_DROP_EN)
#define qdmaDisableRxDrop(base)				IO_CBITS(QDMA_CSR_RX_PROTECT_CFG(base), RX_DROP_EN)
#define qdmaIsRxDropEnable(base)			(IO_GREG(QDMA_CSR_RX_PROTECT_CFG(base)) & RX_DROP_EN)
#define qdmaEnableRxRateLimit(base)			IO_SBITS(QDMA_CSR_RX_PROTECT_CFG(base), RX_RATELIMIT_EN)
#define qdmaDisableRxRateLimit(base)		IO_CBITS(QDMA_CSR_RX_PROTECT_CFG(base), RX_RATELIMIT_EN)
#define qdmaIsRxRateLimitEnable(base)		(IO_GREG(QDMA_CSR_RX_PROTECT_CFG(base)) & RX_RATELIMIT_EN)
#define qdmaSetRxRateLimitByPacket(base)		IO_SBITS(QDMA_CSR_RX_PROTECT_CFG(base), RX_RATELIMIT_PKTMODE)
#define qdmaSetRxRateLimitByByte(base)			IO_CBITS(QDMA_CSR_RX_PROTECT_CFG(base), RX_RATELIMIT_PKTMODE)
#define qdmaIsRxRateLimitByPacket(base)			(IO_GREG(QDMA_CSR_RX_PROTECT_CFG(base)) & RX_RATELIMIT_PKTMODE)
#define qdmaSetRxRateLimitBucketScale(base, val)	IO_SMASK(QDMA_CSR_RX_PROTECT_CFG(base), RX_RATELIMIT_BUCKET_SCALE_MASK, RX_RATELIMIT_BUCKET_SCALE_SHIFT, val)
#define qdmaGetRxRateLimitBucketScale(base)			IO_GMASK(QDMA_CSR_RX_PROTECT_CFG(base), RX_RATELIMIT_BUCKET_SCALE_MASK, RX_RATELIMIT_BUCKET_SCALE_SHIFT)
#define qdmaSetRxRateLimitTicksel(base, val)		IO_SMASK(QDMA_CSR_RX_PROTECT_CFG(base), RX_RATELIMIT_TICKSEL_MASK, RX_RATELIMIT_TICKSEL_SHIFT, val)
#define qdmaGetRxRateLimitTicksel(base)				IO_GMASK(QDMA_CSR_RX_PROTECT_CFG(base), RX_RATELIMIT_TICKSEL_MASK, RX_RATELIMIT_TICKSEL_SHIFT)

#define qdmaEnableChnlRateLimit(base, idx)		IO_SBITS(QDMA_CSR_RATELIMIT_EN_CFG(base), TX_CHNL_RATELIMIT_EN(idx))
#define qdmaDisableChnlRateLimit(base, idx)		IO_CBITS(QDMA_CSR_RATELIMIT_EN_CFG(base), TX_CHNL_RATELIMIT_EN(idx))
#define qdmaIsChnlRateLimitEnable(base, idx)	(IO_GREG(QDMA_CSR_RATELIMIT_EN_CFG(base)) & TX_CHNL_RATELIMIT_EN(idx))

#define qdmaSetQosMode(base, idx, val)				IO_SREG(QDMA_CSR_PERCHNL_QOS_MODE(base, idx), val)
#define qdmaSetPerChnlQosMode(base, chnl, val)		IO_SMASK(QDMA_CSR_PERCHNL_QOS_MODE(base, (chnl>>3)), TXQOS_CHNL_QOS_MODE_MASK(chnl), TXQOS_CHNL_QOS_MODE_SHIFT(chnl), val)
#define qdmaGetPerChnlQosMode(base, chnl)			IO_GMASK(QDMA_CSR_PERCHNL_QOS_MODE(base, (chnl>>3)), TXQOS_CHNL_QOS_MODE_MASK(chnl), TXQOS_CHNL_QOS_MODE_SHIFT(chnl))

#define qdmaGetTxPerChnlDataRate(base, chnl)		IO_GMASK(QDMA_CSR_PERCHNL_DATARATE(base, (chnl>>1)), TXQOS_CHNL_EGRESS_DATARATE_MASK(chnl), TXQOS_CHNL_EGRESS_DATARATE_SHIFT(chnl))

#define qdmaIsCntrEnable(base, idx)			(IO_GREG(QDMA_CSR_DBG_CNTR_CFG(base, idx)) & DBG_CNTR_EN)
#define qdmaIsCntrChnlAll(base, idx)		(IO_GREG(QDMA_CSR_DBG_CNTR_CFG(base, idx)) & DBG_CNTR_ALL_CHNL)
#define qdmaIsCntrQueueAll(base, idx)		(IO_GREG(QDMA_CSR_DBG_CNTR_CFG(base, idx)) & DBG_CNTR_ALL_QUEUE)
#define qdmaIsCntrDscpRingAll(base, idx)	(IO_GREG(QDMA_CSR_DBG_CNTR_CFG(base, idx)) & DBG_CNTR_ALL_DSCPRING)
#define qdmaEnableCntrCfg(base, idx)		IO_SBITS(QDMA_CSR_DBG_CNTR_CFG(base, idx), DBG_CNTR_EN)
#define qdmaDisableCntrCfg(base, idx)		IO_CBITS(QDMA_CSR_DBG_CNTR_CFG(base, idx), DBG_CNTR_EN)
#define qdmaGetCntrSrc(base, idx)			IO_GMASK(QDMA_CSR_DBG_CNTR_CFG(base, idx), DBG_CNTR_SRC_MASK, DBG_CNTR_SRC_SHIFT)
#define qdmaSetCntrSrc(base, idx, val)		IO_SMASK(QDMA_CSR_DBG_CNTR_CFG(base, idx), DBG_CNTR_SRC_MASK, DBG_CNTR_SRC_SHIFT, val)
#define qdmaSetCntrChnlAll(base, idx)		IO_SBITS(QDMA_CSR_DBG_CNTR_CFG(base, idx), DBG_CNTR_ALL_CHNL)
#define qdmaSetCntrChnlSingle(base, idx)	IO_CBITS(QDMA_CSR_DBG_CNTR_CFG(base, idx), DBG_CNTR_ALL_CHNL)
#define qdmaSetCntrQueueAll(base, idx)		IO_SBITS(QDMA_CSR_DBG_CNTR_CFG(base, idx), DBG_CNTR_ALL_QUEUE)
#define qdmaSetCntrQueueSingle(base, idx)	IO_CBITS(QDMA_CSR_DBG_CNTR_CFG(base, idx), DBG_CNTR_ALL_QUEUE)
#define qdmaSetCntrDscpAll(base, idx)		IO_SBITS(QDMA_CSR_DBG_CNTR_CFG(base, idx), DBG_CNTR_ALL_DSCPRING)
#define qdmaSetCntrDscpSingle(base, idx)	IO_CBITS(QDMA_CSR_DBG_CNTR_CFG(base, idx), DBG_CNTR_ALL_DSCPRING)
#define qdmaGetCntrDscpIdx(base, idx)		IO_GMASK(QDMA_CSR_DBG_CNTR_CFG(base, idx), DBG_CNTR_DSCPRING_IDX_MASK, DBG_CNTR_DSCPRING_IDX_SHIFT)
#define qdmaSetCntrDscpIdx(base, idx, val)	IO_SMASK(QDMA_CSR_DBG_CNTR_CFG(base, idx), DBG_CNTR_DSCPRING_IDX_MASK, DBG_CNTR_DSCPRING_IDX_SHIFT, val)
#define qdmaGetCntrChnlIdx(base, idx)		IO_GMASK(QDMA_CSR_DBG_CNTR_CFG(base, idx), DBG_CNTR_CHNL_MASK, DBG_CNTR_CHNL_SHIFT)
#define qdmaSetCntrChnlIdx(base, idx, val)	IO_SMASK(QDMA_CSR_DBG_CNTR_CFG(base, idx), DBG_CNTR_CHNL_MASK, DBG_CNTR_CHNL_SHIFT, val)
#define qdmaGetCntrQueueIdx(base, idx)		IO_GMASK(QDMA_CSR_DBG_CNTR_CFG(base, idx), DBG_CNTR_QUEUE_MASK, DBG_CNTR_QUEUE_SHIFT)
#define qdmaSetCntrQueueIdx(base, idx, val)	IO_SMASK(QDMA_CSR_DBG_CNTR_CFG(base, idx), DBG_CNTR_QUEUE_MASK, DBG_CNTR_QUEUE_SHIFT, val)
#define qdmaGetCntrCfg(base, idx)			IO_GREG(QDMA_CSR_DBG_CNTR_CFG(base, idx))
#define qdmaGetCntrCounter(base, idx)		IO_GREG(QDMA_CSR_DBG_CNTR_VAR(base, idx))
#define qdmaClearCntrCounter(base, idx)		IO_SREG(QDMA_CSR_DBG_CNTR_VAR(base, idx), 0)


/************************************************************************
*                  D A T A	 T Y P E S
*************************************************************************
*/
typedef struct {
	uint 	resv1 ;
	struct {
#ifdef __BIG_ENDIAN
		uint done				: 1 ;
		uint overflow			: 1 ;
		uint resv2				: 14 ;
		uint pkt_len			: 16 ;
#else
		uint pkt_len			: 16 ;
		uint resv2				: 14 ;
		uint overflow			: 1 ;
		uint done				: 1 ;
#endif /* __BIG_ENDIAN */
	} ctrl ;
	uint 	pkt_addr ;
#ifdef __BIG_ENDIAN
	uint resv3					: 20 ;
	uint next_idx				: 12 ;
#else
	uint next_idx				: 12 ;
	uint resv3					: 20 ;
#endif /* __BIG_ENDIAN */
	uint msg[QDMA_DSCP_MSG_WORD_LENS] ;
} QDMA_DMA_DSCP_T ;

#if 0
typedef struct {
	uint word[QDMA_DSCP_MSG_TOTAL_WORD_LENS] ;
} QDMA_DMA_DSCP_MSG_T ;
#endif

typedef struct {
	uint 	pkt_addr ;
	struct {
#ifdef __BIG_ENDIAN
		uint ctx				: 1 ;
		uint resv				: 2 ;
		uint ctx_ring			: 1 ;
		uint ctx_idx			: 12 ;
		uint pkt_len			: 16 ;
#else
		uint pkt_len			: 16 ;
		uint ctx_idx			: 12 ;
		uint ctx_ring			: 1 ;
		uint resv				: 2 ;
		uint ctx				: 1 ;
#endif /* __BIG_ENDIAN */
	} ctrl ;
	uint msg[2] ;
} QDMA_HWFWD_DMA_DSCP_T ;

typedef union
{
    struct
    {
		uint reserve1     : 7;
		uint dei          : 1;
		uint reserve2     : 12;
		uint oam          : 1;
		uint channel      : 8;
		uint queue        : 3;
    }bits;
    uint word;
} txMsgWord0_t;

typedef struct
{
    uint word;
} txMsgWord1_t;

typedef struct txMsgWord_s{
	txMsgWord0_t *pTxMsgW0;
	txMsgWord1_t *pTxMsgW1;
} txMsgWord_t;

typedef struct {
	struct cdev *pQdmaDev ;
} QDMA_Dev_T ;

/************************************************************************
*                  D A T A   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*                  F U N C T I O N    D E C L A R A T I O N S
#                  I N L I N E    F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

#ifdef CONFIG_NEW_QDMA_CSR_OFFSET
	int __inline__ pseSetWanPcpConfig(unchar type, unchar mode) ;
	int __inline__ pseGetWanPcpConfig(unchar type) ;
	int __inline__ qdmaSetChannelRetire(unchar channel) ;
#endif /* CONFIG_NEW_QDMA_CSR_OFFSET */
int qdmaSetTxQosScheduler(unchar channel, unchar mode, unchar weight[8]) ;
int qdmaGetTxQosScheduler(unchar channel, unchar *pMode, unchar weight[8]) ;
int qdmaSetRxRateLimitConfig(unchar ringIdx, unchar type, ushort value) ;
int qdmaGetRxRateLimitConfig(unchar ringIdx, unchar type) ;
int qdmaSetTxRateLimitConfig(unchar chnlIdx, unchar type, ushort value) ;
int qdmaGetTxRateLimitConfig(unchar chnlIdx, unchar type) ;
int qdmaSetVirtualChannelQos(unchar phyChnl, unchar virChnlMax, unchar mode, unchar weight[4]) ;
int qdmaGetVirtualChannelQos(unchar phyChnl, unchar virChnlMax, unchar *pMode, unchar weight[4]) ;
int qdmaGetLimitRateMax(void) ;
int qdmaUpdateAllTxRateLimitValue(ushort curUnit, ushort newUnit) ;
int qdmaUpdateAllRxRateLimitValue(ushort curUnit, ushort newUnit) ;
int qdma_clear_and_set_dbg_cntr_info(QDMA_DBG_CNTR_T *dbgCntrPtr) ;
int qdma_get_dbg_cntr_info(QDMA_DBG_CNTR_T *dbgCntrPtr) ;
int qdma_set_dbg_cntr_default_config(void) ;

int qdmaSetDbgMemXsConfig(QDMA_DbgMemXsMemSel_t memSel, unchar byteSel, ushort addr, uint valueLow, uint valueHigh) ;
int qdmaGetDbgMemXsConfig(QDMA_DbgMemXsMemSel_t memSel, unchar byteSel, ushort addr, uint *value) ;

int qdma_get_ringIdx(struct sk_buff *skb, struct port_info *pMacInfo) ;
#ifdef TCSUPPORT_QOS
int qdma_qos_mark(struct sk_buff *skb) ;
#else
int qdma_qos_remark(struct sk_buff *skb) ;
#endif
int macResourceLimit(struct sk_buff *skb, uint channel, uint queue) ;

int qdmaEnableInt(uint base, uint bit) ;
int qdmaDisableInt(uint base, uint bit) ;
int qdmaSetIntMask(uint base, uint value) ;
int qdmaGetIntMask(uint base) ;

void qdma_dma_busy_timer(unsigned long arg) ;
void qdma_reg_polling_timer(unsigned long arg) ;
#ifdef TCSUPPORT_DRIVER_API
void qdma_dev_destroy(void) ;
#endif
int qdma_dev_init(void) ;

#endif /* _QDMA_DEV_H_ */
