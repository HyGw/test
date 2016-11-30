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
#include <ecnt_lib_dbg.h>
#include "libapi_lib_voipmgr.h"
#include <adam.h>

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
************************************************************************/

/************************************************************************
*                  M A C R O S
************************************************************************/
#define ECNT_LIB_VOIP_PRINT_ERROR(fmt, ...)  ECNT_LIB_DBG_PRINT(ECNT_DBG_ERROR,  ECNT_DBG_TYPE_VOIP, fmt, ##__VA_ARGS__)
#define ECNT_LIB_VOIP_PRINT_WARN(fmt, ...)   ECNT_LIB_DBG_PRINT(ECNT_DBG_WARN,   ECNT_DBG_TYPE_VOIP, fmt, ##__VA_ARGS__)
#define ECNT_LIB_VOIP_PRINT_NOTICE(fmt, ...) ECNT_LIB_DBG_PRINT(ECNT_DBG_NOTICE, ECNT_DBG_TYPE_VOIP, fmt, ##__VA_ARGS__)
#define ECNT_LIB_VOIP_PRINT_DEBUG(fmt, ...)  ECNT_LIB_DBG_PRINT(ECNT_DBG_DEBUG,  ECNT_DBG_TYPE_VOIP, fmt, ##__VA_ARGS__)

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
 Descriptor:	It's used to allocate ADAM resource and activate ADAM process.
 Input Args:	                   
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_init(void)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamInit();
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Deactivate ADAM process and release resource.
 Input Args:	                   
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_exit(void)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamExit();
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Get ADAM's configuration (adamConfig) informaiton.
 Input Args:	pAdamConf: Provide the information of ADAM version, number of DSP and Interface controlled, and 
                    active status
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_query(adamConfig_t *pAdamConf)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamQuery(pAdamConf);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	It's used to poll DSP and Interface event.
 Input Args:	mode: When blockMode is set to NONBLOCK, the function returns NULL immediately 
                    when there is no event available, or returns the event when available;
                    Or when blockMode is set to BLOCK, the function queued until a valid event available 
                    then returns the event
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_poll_event(blockMode_e mode, event_t *pEvent)
{
	int ret = -1;
	ret = adamPollEvent(mode, pEvent);
	//ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	//ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Invoke (load and initialize) DSP by DSP ID.
 Input Args:	dspContext: used to set dsp id
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_dsp_invoke(dspContext_t *dspContext)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamDspInvoke(dspContext->dspId);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Revoke (terminate and unload) DSP by DSP ID.
 Input Args:	dspContext: used to set dsp id
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_dsp_revoke(dspContext_t *dspContext)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamDspRevoke(dspContext->dspId);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	It's used to query DSP support feature by DSP ID.
 Input Args:	dspContext: used to set dsp id
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_dsp_query(dspContext_t *dspContext, activeState_e *dspActive, dspFeature_t *pFeature)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamDspQuery(dspContext->dspId, dspActive, pFeature);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Configure Tone profile by DSP ID.
 Input Args:	dspContext: used to set dsp id
                    toneId:Tone[0] is reserved as silence tone, can not be configured
                    pTone:used to set tone configuration,parameters and attributes
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_dsp_config_tone(dspContext_t *dspContext, uint16 toneId, tone_t *pTone)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamDspConfigTone(dspContext->dspId, toneId, pTone);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Configure CPT profile by DSP ID.Change DSP call progress tone detection configration
 Input Args:	dspContext: used to set dsp id
                    pCpt:used to set tone detection configration
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_dsp_config_cpt(dspContext_t *dspContext, cpt_t *pCpt)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamDspConfigCpt(dspContext->dspId, pCpt);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Query Channel configuration by DSP ID and Channel ID.
 Input Args:	dspContext: used to set dsp id channel id.
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_chan_query(dspContext_t *dspContext, chanConfig_t *pChanConf)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamChanQuery(dspContext->dspId, dspContext->chanId, pChanConf);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Configure Channel by DSP ID and Channel ID.
 Input Args:	dspContext: used to set dsp id channel id
                    pChanConf:used to set channel configuration 
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_chan_config(dspContext_t *dspContext, chanConfig_t *pChanConf)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamChanConfig(dspContext->dspId, dspContext->chanId, pChanConf);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	For dsp replace pcm data to a special file.
 Input Args:	dspContext: used to set dsp id channel id
                    pReplaceConf:special file used to replace pcm data
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_dsp_replace_data(dspContext_t *dspContext, replaceData_t *pReplaceConf)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamDSPReplaceData(dspContext->dspId, dspContext->chanId, pReplaceConf);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}


/******************************************************************************
 Descriptor:	Play (generate) tone to specific channel by DSP ID and Channel ID for tone_stop_event..
 Input Args:	dspContext: used to set dsp id channel id
                    pToneSeq: tone ID to be palyed ,users can put several tone IDs and play in sequence 
                    repeat:times of repeat the tone sequence
                    dur:set the tone duration 
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_chan_play_tone(dspContext_t *dspContext, toneSeq_t *pToneSeq, uint32 repeat, uint32 dur)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamChanPlayTone(dspContext->dspId, dspContext->chanId, pToneSeq, repeat, dur);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	It's used for pcm loopback test. if pcm loopback is enabled, pcm loopback between ch 0 and ch 1, 
                    no dsp process and rtp process is involved .
 Input Args:	dspContext: used to set dsp id
                    en: enable the pcm loopback
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_pcm_loopback(dspContext_t *dspContext, enableControl_e en)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamPcmLoopback(dspContext->dspId, en);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Rtp loopback for a channel, work like a echo, the incomming rtp data is echoed back
                    without dsp processing if rtp loopback of the channel is enabled.
 Input Args:	dspContext: used to set dsp id and channel id
                    en: enable the rtp loopback of the channel 
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_rtp_loopback(dspContext_t *dspContext, enableControl_e en)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamRtpLoopback(dspContext->dspId, dspContext->chanId, en);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Dsp loopback for a channel, work like a echo, the incomming rtp data is echoed back
                    after dsp processing if dsp loopback of the channel is enabled
 Input Args:	dspContext: used to set dsp id and channel id
                    en: enable the dsp loopback of the channel 
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_dsp_loopback(dspContext_t *dspContext, enableControl_e en)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamDspLoopback(dspContext->dspId, dspContext->chanId, en);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Play DTMF tone to specific channel by DSP ID and Channel ID.
 Input Args:	dspContext: used to set dsp id and channel id
                    digit:DTMF digit to be send in DTMF relay packet
                    dur:set the tone duration 
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_chan_play_dtmf(dspContext_t *dspContext, char digit, uint32 dur)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamChanPlayDtmf(dspContext->dspId, dspContext->chanId, digit, dur);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Play (generate) tone sequence to specific channel by DSP ID and Channel ID.
 Input Args:	dspContext: used to set dsp id and channel id
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_chan_stop_tone(dspContext_t *dspContext)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamChanStopTone(dspContext->dspId, dspContext->chanId);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Play (generate) Caller ID to specific channel by DSP ID and Channel ID.
 Input Args:	dspContext: used to set dsp id and channel id
                    pCid:set the number transmitted to the telephone 
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_chan_play_cid(dspContext_t *dspContext, cid_t *pCid)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamChanPlayCid(dspContext->dspId, dspContext->chanId, pCid);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Play (generate) type 2 caller ID to specific channel by DSP ID and Channel ID
 Input Args:	dspContext: used to set dsp id and channel id
                    pCid:set the number transmitted to the telephone 
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_chan_play_type2_cid(dspContext_t *dspContext, cid_t *pCid)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamChanPlayType2Cid(dspContext->dspId, dspContext->chanId, pCid);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Used to dump channel PCM data to designated network address(PC) and capture PCM
                    dump log for debugging with sniffer software, such as Wireshark
 Input Args:	dspContext: used to set dsp id and channel id
                    pDstAddr:IP address of destination PC to receive PCM dump log
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_chan_dump_pcm(dspContext_t *dspContext, netAddr_t *pDstAddr)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamChanDumpPcm(dspContext->dspId, dspContext->chanId, pDstAddr);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Play (generate) Dtmf Caller ID to specific channel by DSP ID and Channel ID 
 Input Args:	dspContext: used to set dsp id and channel id
                    pCid:set the number transmitted to the telephone 
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_chan_play_dtmf_cid(dspContext_t *dspContext, cid_t *pCid)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamChanPlayDtmfCid(dspContext->dspId, dspContext->chanId, pCid);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Query Stream configuration by DSP ID, Channel ID and Stream ID.
 Input Args:	dspContext: used to set dsp id , channel id and stream id
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_strm_query(dspContext_t *dspContext, activeState_e *pStrmActive, strmConfig_t *pStrmConf)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamStrmQuery(dspContext->dspId, dspContext->chanId, dspContext->strmId, pStrmActive, pStrmConf);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Configure Channel by DSP ID, Channel ID and Stream ID anytime ot matter if the stream 
                    is started or not and the configuration change will take effect immedialy.
 Input Args:	dspContext: used to set dsp id , channel id and stream id
                    pStrmConf: set the stream configuration
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_strm_config(dspContext_t *dspContext, strmConfig_t *pStrmConf)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamStrmConfig(dspContext->dspId, dspContext->chanId, dspContext->strmId, pStrmConf);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Enable a streamming process by the configuration of the Stream .
 Input Args:	dspContext: used to set dsp id , channel id and stream id
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_strm_start(dspContext_t *dspContext)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamStrmStart(dspContext->dspId, dspContext->chanId, dspContext->strmId);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Disable a streamming process.
 Input Args:	dspContext: used to set dsp id , channel id and stream id
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_strm_stop(dspContext_t *dspContext)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamStrmStop(dspContext->dspId, dspContext->chanId, dspContext->strmId);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Disable all streamming process.
 Input Args:	dspContext: used to set dsp id
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_strm_stop_all(dspContext_t *dspContext)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamStrmStopAll(dspContext->dspId);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Send DTMF relay packet (RFC2833/4733) to the session destination .The stream
                    has to be started first before executing this command
 Input Args:	dspContext: used to set dsp id , channel id and stream id
                    dtmf:DTMF digit to be send in DTMF relay packet
                    dur:set the tone duration 
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_strm_send_dtmfr(dspContext_t *dspContext, dtmf_e dtmf, uint32 dur)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamStrmSendDtmfr(dspContext->dspId, dspContext->chanId, dspContext->strmId, dtmf, dur);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Play (generate) tone to the stream.
 Input Args:	dspContext: used to set dsp id , channel id and stream id
                    pToneSeq: tone ID to be palyed ,users can put several tone IDs and play in sequence 
                    repeat:times of repeat the tone sequence
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_strm_play_tone(dspContext_t *dspContext, toneSeq_t *pToneSeq, uint32 repeat)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamStrmPlayTone(dspContext->dspId, dspContext->chanId, dspContext->strmId, pToneSeq, repeat);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Stop play tone to the stream.
 Input Args:	dspContext: used to set dsp id , channel id and stream id
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_strm_stop_tone(dspContext_t *dspContext)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamStrmStopTone(dspContext->dspId, dspContext->chanId, dspContext->strmId);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Query current media infomation.
 Input Args:	dspContext: used to set dsp id , channel id and stream id
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_strm_query_media_info(dspContext_t *dspContext, mediaInfo_t *pMediaInfo)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamStrmQueryMediaInfo(dspContext->dspId, dspContext->chanId, dspContext->strmId, pMediaInfo);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Clean current media infomation to zero.
 Input Args:	enable: GPONMGR_ENABLE/GPONMGR_DISABLE act debug level.
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_strm_reset_media_info(dspContext_t *dspContext)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamStrmResetMediaInfo(dspContext->dspId, dspContext->chanId, dspContext->strmId);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Query Interface configuration by Interface ID.
 Input Args:	infc:Depending on the exact interface numbers on the platform
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_infc_query(infcId_e infc, infcConfig_t *pInfcConf)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamInfcQuery(infc, pInfcConf);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Configure Interface Line state by Interface ID.
 Input Args:	infc:Depending on the exact interface numbers on the platform
                    state:set line state
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_infc_config_line(infcId_e infc, lineState_e state)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamInfcConfigLine(infc, state);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Configure Interface Hook state by Interface ID.
 Input Args:	infc:Depending on the exact interface numbers on the platform
                    state:set hook state
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_infc_config_hook(infcId_e infc, hookState_e state)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamInfcConfigHook(infc, state);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Change interface ring profile configuration, only works for FXS interface.
 Input Args:	infc:Depending on the exact interface numbers on the platform
                    pRingProf:set the ring profile configuration
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_infc_config_ring(infcId_e infc, ringProfile_t *pRingProf)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamInfcConfigRing(infc, pRingProf);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Control interface ringing, only work for FXS type interface
 Input Args:	infc:Depending on the exact interface numbers on the platform
                    dur:optional, if not provided, it will use the ring configuration.Set dur=0 to disable ring
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_infc_ring(infcId_e infc, uint32 dur, cid_t *pCid)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamInfcRing(infc, dur, pCid);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Stop ringing
 Input Args:	infc:Depending on the exact interface numbers on the platform
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_infc_stop_ring(infcId_e infc)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamInfcStopRing(infc);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Change interface hook time detection threshold by Interface ID.
 Input Args:	infc:Depending on the exact interface numbers on the platform
                    pHookTs:set hook detection time to meet each hook state
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_infc_config_hook_ts(infcId_e infc, hookThreshold_t *pHookTs)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamInfcConfigHookTs(infc, pHookTs);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Change SLIC country code ID.For user to change the country code ID through API
 Input Args:	infc:Depending on the exact interface numbers on the platform
                    hwProfId:Depending on the exact country code numbers on the platform
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_infc_config_hw_prof(infcId_e infc,hwProf_e hwProfId)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamInfcConfigHwProf(infc, hwProfId);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Support ring parameters set.
 Input Args:	infc:Depending on the exact interface numbers on the platform
                    ringParams: Set ring parameters
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_infc_ring_params(infcId_e infc, infcRingParams_t* ringParams)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamInfcRingParams(infc, ringParams);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Support line test.
 Input Args:	lineTest:set the line test infc, lineTestId and lineTestData
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_infc_line_test(infcLineTest_t*  lineTest)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamInfcLineTest(lineTest);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	Query the slic type by Interface ID
 Input Args:	infc:Depending on the exact interface numbers on the platform
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_infc_slic_type_query(infcId_e infc, slicParams_t* slicParams)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamInfcSlicTypeQuery(infc, slicParams);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	support DC feed parameters set.
 Input Args:	infc:Depending on the exact interface numbers on the platform
                    dcFeedParams:Set DC feed parameters
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET voipmgr_lib_infc_dc_feed_params(infcId_e infc, infcDcFeedParams_t* dcFeedParams)
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamInfcDcFeedParams(infc, dcFeedParams);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

/******************************************************************************
 Descriptor:	support China Unicom inside and outside line test.
 Input Args:	infc:Depending on the exact interface numbers on the platform
                    slicType:Set slic type
 Ret Value:	success: ECNT_E_SUCCESS
                    fail: ECNT_E_FAIL
******************************************************************************/
ECNT_LIB_RET
voipmgr_lib_infc_cu_line_test(infcLineTest_t* lineTest, int slicType, infcUnicomLineTest_t* unicomLineTest) 
{
	int ret = -1;
	ECNT_LIB_VOIP_PRINT_DEBUG("enter %s.\n", __func__);
	ret = adamInfcCULineTest(lineTest, slicType, unicomLineTest);
	if (EXEC_SUCCESS == ret) {
		ret = ECNT_E_SUCCESS;
	}
	ECNT_LIB_VOIP_PRINT_DEBUG("exit %s.\n", __func__);
	return ret;
}

