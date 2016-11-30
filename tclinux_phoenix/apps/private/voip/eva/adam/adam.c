/*********************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY 
* ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
* DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED FROM MEDIATEK AND/OR 
* ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS ONLY. 
* MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR 
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
* WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH 
* MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK 
* SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY
* WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE 
* RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S 
* SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND 
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED 
* HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE 
* MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR 
* SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE
* AT ISSUE.
*
* THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN 
* ACCORDANCE WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING
* ITS CONFLICT OF LAWS PRINCIPLES.
**********************************************************************
*/

/*
 *	File:	adam.c
 *	 
 *	Author: Serena Huang, Quark Li
 * 		
 *	Last Revision Date: 2011.9.29
 *	 
 *	Description:
 *	Adaptive DSP Access Manager API layer implementation to control DSP
 *	and Telephony Interface.  
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "adam.h"


#ifdef DSPID_VIKING
#include "gdi_viking.h"

#define DSP_ID DSP_VIKING
#elif DSPID_MTK
#include "gdi_mtk.h"

#define DSP_ID DSP_MTK
#endif

#define NET_LOG_FILE_NAME "/var/voice_netlog"

#define adamLog(_r, _msg) pAdamDspSet[DSP_MTK]->dspLog(_r,(char*)__FUNCTION__, __LINE__, _msg)


/* ADAM Configurations 
 * Provide the information of ADAM version, number of DSP and Interface controlled, and 
 * active status.
 * 
 * Note: A global _gAdamConfig is declared int adam_main.c to hold the information.
 * Its attributes were initialized at declaration with defined value.
 *
 */
static adamConfig_t _gAdamConfig = {
	.version  = ADAM_VERSION,
	.dspNum = ADAM_DSP_NUM,
	.infcNum = ADAM_INFC_NUM
};

static activeState_e _gAdamActive = STATE_INACTIVE;


dspObject_t	*pAdamDspSet[ADAM_DSP_NUM] = {NULL};
dspFeature_t gDspFeature[ADAM_DSP_NUM];
activeState_e gDspActive[ADAM_DSP_NUM] = {STATE_INACTIVE};

/* ==================================== *
 *
 * ADAM Generic Management Section
 *
 * ==================================== */

/* Allocate ADAM resource and activate ADAM process. */
exCode_e 
adamInit(void) {
	if (STATE_ACTIVE == _gAdamActive) {
		return EXEC_SUCCESS;
	}

#ifdef DSPID_VIKING
	pAdamDspSet[DSP_VIKING] = getDspViking();
#elif DSPID_MTK
	pAdamDspSet[DSP_MTK] = getDspMtk();
#endif
	adamLog(EVA_LOG_DBG, "ENTER");
	if (NULL == pAdamDspSet[DSP_ID]) {
		adamLog(EVA_LOG_ERR, "EXIT");
		return EXEC_FAIL;
	}

	adamDspQuery(DSP_ID, &(gDspActive[DSP_ID]),  &(gDspFeature[DSP_ID]));

	_gAdamActive = STATE_ACTIVE;
	adamLog(EVA_LOG_DBG, "EXIT");
	return EXEC_SUCCESS;
}

/*Deactivate ADAM process and release resource. */
exCode_e 
adamExit(void) {
	adamLog(EVA_LOG_DBG, "ENTER");

	int i;

	if (STATE_ACTIVE == _gAdamActive) {
		for (i = 0; i < ADAM_DSP_NUM; i++) {
			if(pAdamDspSet[i]!=NULL){
			if (pAdamDspSet[i]->dspRevoke) {
				if (EXEC_SUCCESS != pAdamDspSet[i]->dspRevoke()) {
					adamLog(EVA_LOG_ERR, "EXIT");
					return UNKNOWN_ERROR;
				}
			}
			else {
				adamLog(EVA_LOG_ERR, "EXIT");
				return FUNC_UNSUPPORT;
			}
		}
	}
	}

	_gAdamActive = STATE_INACTIVE;
	adamLog(EVA_LOG_DBG, "EXIT");
	return EXEC_SUCCESS;
}

/* Get ADAM's configuration (_gAdamConfig) informaiton */
exCode_e 
adamQuery(adamConfig_t *pAdamConf) {
	adamLog(EVA_LOG_DBG, "ENTER");

	memcpy(pAdamConf, &_gAdamConfig, sizeof(adamConfig_t));
	adamLog(EVA_LOG_DBG, "EXIT");
	return EXEC_SUCCESS;
}

/* Call adamPollEvent() to poll DSP and Interface event. 
 * When blockMode is set to NONBLOCK, the function returns
 * NULL immediately when there is no event available, or returns
 * the event when available..
 * or
 * when blockMode is set to BLOCK, the function queued
 * until a valid event available then returns the event.
 * 
 */
exCode_e 
adamPollEvent(blockMode_e mode, event_t *pEvent) {
	adamLog(EVA_LOG_INFO, "ENTER");

	if ((BLOCKING == mode) || (NON_BLOCKING == mode)) {
		if (pAdamDspSet[DSP_ID]->dspPollEvent) {
			adamLog(EVA_LOG_INFO, "EXIT");
			return (pAdamDspSet[DSP_ID]->dspPollEvent(mode, pEvent));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}	

/* ==================================== *
 *
 * ADAM DSP Control Section
 *
 * ==================================== */

/* Invoke (load and initialize) DSP by DSP ID*/
exCode_e 
adamDspInvoke(dspId_e dsp) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM) {
		if (pAdamDspSet[dsp]->dspInvoke) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->dspInvoke());
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* Revoke (terminate and unload) DSP by DSP ID */
exCode_e 
adamDspRevoke(dspId_e dsp) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM) {
		if (pAdamDspSet[dsp]->dspRevoke) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->dspRevoke());
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* Query DSP support feature by DSP ID */
exCode_e 
adamDspQuery(dspId_e dsp, activeState_e *pDspActive, dspFeature_t *pFeature) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM) {
		if (pAdamDspSet[dsp]->dspQuery) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->dspQuery(pDspActive, pFeature));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

exCode_e adamDspConfigTone(dspId_e dsp, uint16 toneId, tone_t *pTone) {
	adamLog(EVA_LOG_DBG, "ENTER");
	
	if (dsp < ADAM_DSP_NUM) {
		if (pAdamDspSet[dsp]->dspConfigTone) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return  (pAdamDspSet[dsp]->dspConfigTone(toneId, pTone));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

exCode_e adamDspConfigCpt(dspId_e dsp, cpt_t *pCpt) {
	adamLog(EVA_LOG_DBG, "ENTER");
	
	if (dsp < ADAM_DSP_NUM) {
		if (pAdamDspSet[dsp]->dspConfigCpt) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return  (pAdamDspSet[dsp]->dspConfigCpt(pCpt));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* ==================================== *
 *
 * ADAM Channel Control Section
 *
 * ==================================== */

/* Query Channel configuration by DSP ID and Channel ID */
exCode_e 
adamChanQuery(dspId_e dsp, chanId_e ch, chanConfig_t *pChanConf) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan) {
		if (pAdamDspSet[dsp]->chanQuery) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->chanQuery(ch, pChanConf));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* Configure Channel by DSP ID and Channel ID */
exCode_e 
adamChanConfig(dspId_e dsp, chanId_e ch, chanConfig_t *pChanConf) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan) {
		if (pAdamDspSet[dsp]->chanConfig) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->chanConfig(ch, pChanConf));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}
/*for dsp replace pcm data to a special file*/
exCode_e adamDSPReplaceData(dspId_e dsp, chanId_e ch, replaceData_t *pReplaceConf)
{
	adamLog(EVA_LOG_DBG, "ENTER");
	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan) {
		if (pAdamDspSet[dsp]->chanDspReplaceData) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->chanDspReplaceData(ch, pReplaceConf));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;

}
/* For tone_stop_event */
exCode_e adamChanPlayTone(dspId_e dsp, chanId_e ch, toneSeq_t *pToneSeq, uint32 repeat, uint32 dur) 
{
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan) {
		if (pAdamDspSet[dsp]->chanPlayTone) {
			adamLog(EVA_LOG_DBG, "EXIT");
			/* For tone_stop_event */
			return (pAdamDspSet[dsp]->chanPlayTone(ch, pToneSeq, repeat, dur));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* For pcm loopback test. */
exCode_e adamPcmLoopback(dspId_e dsp, enableControl_e en) 
{
	if (dsp < ADAM_DSP_NUM ){
		return (pAdamDspSet[dsp]->chanPcmLoopback(en));
	}

}

/*Support rtp/dsp loopback*/
/* rtp loopback for a channel, work like a echo, the incomming rtp data is echoed back
** without dsp processing if rtp loopback of the channel is enabled*/
exCode_e adamRtpLoopback(dspId_e dsp, chanId_e ch, enableControl_e en)
{
	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan){
		return (pAdamDspSet[dsp]->chanRtpLoopback(ch, en));
	}
}

/* dsp loopback for a channel, work like a echo, the incomming rtp data is echoed back
** after dsp processing if dsp loopback of the channel is enabled*/
exCode_e adamDspLoopback(dspId_e dsp, chanId_e ch, enableControl_e en)
{
	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan){
		return (pAdamDspSet[dsp]->chanDspLoopback(ch, en));
	}
}


/* Play DTMF tone to specific channel by DSP ID and Channel ID */
exCode_e 
adamChanPlayDtmf(dspId_e dsp, chanId_e ch, char digit, uint32 dur) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan) {
		if (pAdamDspSet[dsp]->chanPlayDtmf) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->chanPlayDtmf(ch, digit, dur));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* Play (generate) tone sequence to specific channel by DSP ID and Channel ID */
exCode_e 
adamChanStopTone(dspId_e dsp, chanId_e ch) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan) {
		if (pAdamDspSet[dsp]->chanStopTone) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->chanStopTone(ch));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* Play (generate) Caller ID to specific channel by DSP ID and Channel ID */
exCode_e 
adamChanPlayCid(dspId_e dsp, chanId_e ch, cid_t *pCid) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan) {
		if (pAdamDspSet[dsp]->chanPlayCid) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->chanPlayCid(ch, pCid));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* Play (generate) Caller ID to specific channel by DSP ID and Channel ID */
exCode_e 
adamChanPlayType2Cid(dspId_e dsp, chanId_e ch, cid_t *pCid) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan) {
		if (pAdamDspSet[dsp]->chanPlayType2Cid) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->chanPlayType2Cid(ch, pCid));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* Play (generate) Dtmf Caller ID to specific channel by DSP ID and Channel ID */
exCode_e 
adamChanPlayDtmfCid(dspId_e dsp, chanId_e ch, cid_t *pCid) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan) {
		if (pAdamDspSet[dsp]->chanPlayDtmfCid) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->chanPlayDtmfCid(ch, pCid,CONTROL_DISABLE));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* Play (generate) Caller ID to specific channel by DSP ID and Channel ID */
exCode_e 
adamChanDumpPcm(dspId_e dsp, chanId_e ch, netAddr_t *pDstAddr) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan) {
		if (pAdamDspSet[dsp]->chanDumpPcm) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->chanDumpPcm(ch, pDstAddr));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* ==================================== *
 *
 * ADAM Stream Control Section
 *
 * ==================================== */

/* Query Stream configuration by DSP ID, Channel ID and Stream ID */
exCode_e 
adamStrmQuery(dspId_e dsp, chanId_e ch, strmId_e strm, activeState_e *pStrmActive, strmConfig_t *pStrmConf) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan \
		&& strm < gDspFeature[dsp].strmsPerChan) {
		if (pAdamDspSet[dsp]->strmQuery) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->strmQuery(ch, strm, pStrmActive, pStrmConf));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* Configure Channel by DSP ID, Channel ID and Stream ID */
exCode_e 
adamStrmConfig(dspId_e dsp, chanId_e ch, strmId_e strm, strmConfig_t *pStrmConf) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan \
		&& strm < gDspFeature[dsp].strmsPerChan) {
		if (pAdamDspSet[dsp]->strmConfig) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->strmConfig(ch, strm, pStrmConf));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* Enable a streamming process by the configuration of the Stream */
exCode_e 
adamStrmStart(dspId_e dsp, chanId_e ch, strmId_e strm) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan \
		&& strm < gDspFeature[dsp].strmsPerChan) {
		if (pAdamDspSet[dsp]->strmStart) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->strmStart(ch, strm));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* Disable a streamming process */
exCode_e 
adamStrmStop(dspId_e dsp, chanId_e ch, strmId_e strm) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan \
		&& strm < gDspFeature[dsp].strmsPerChan) {
		if (pAdamDspSet[dsp]->strmStop) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->strmStop(ch, strm));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* Disable all streamming process */
exCode_e 
adamStrmStopAll(dspId_e dsp) {
	int ch = 0, strm = 0;
	exCode_e ret;
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM) {
		for (ch = 0; ch < gDspFeature[dsp].numOfChan; ch++)
			for (strm = 0; strm < gDspFeature[dsp].strmsPerChan; strm++) {
				if (pAdamDspSet[dsp]->strmStop) {
					adamLog(EVA_LOG_DBG, "EXIT");
					if ((ret = pAdamDspSet[dsp]->strmStop(ch, strm)) < 0) {
						continue;
					}
				}
				else {
					adamLog(EVA_LOG_ERR, "EXIT");
					return FUNC_UNSUPPORT;
				}
		}
		adamLog(EVA_LOG_DBG, "EXIT"); /*upgrade_to_4_channel*/
		return ret;
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}
/* Send DTMF relay packet (RFC2833/4733) to the session destination */
exCode_e 
adamStrmSendDtmfr(dspId_e dsp, chanId_e ch, strmId_e strm, dtmf_e dtmf, uint32  dur) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan \
		&& strm < gDspFeature[dsp].strmsPerChan) {
		if (pAdamDspSet[dsp]->strmSendDtmfr) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->strmSendDtmfr(ch, strm, dtmf, dur));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

exCode_e 
adamStrmPlayTone(dspId_e dsp, chanId_e ch, strmId_e strm, toneSeq_t *pToneSeq, uint32 repeat) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan \
		&& strm < gDspFeature[dsp].strmsPerChan) {
		if (pAdamDspSet[dsp]->strmPlayTone) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->strmPlayTone(ch, strm, pToneSeq, repeat));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

exCode_e 
adamStrmStopTone(dspId_e dsp, chanId_e ch, strmId_e strm) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan \
		&& strm < gDspFeature[dsp].strmsPerChan) {
		if (pAdamDspSet[dsp]->strmStopTone) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->strmStopTone(ch, strm));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}


exCode_e 
adamStrmPlayDtmf(dspId_e dsp, chanId_e ch, char digit, uint32 duration) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan) 
    {
		if (pAdamDspSet[dsp]->strmPlayDtmf) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->strmPlayDtmf(ch, digit,duration));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

exCode_e 
adamStrmQueryMediaInfo(dspId_e dsp, chanId_e ch, strmId_e strm, mediaInfo_t *pMediaInfo){
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan \
		&& strm < gDspFeature[dsp].strmsPerChan) {
		if (pAdamDspSet[dsp]->strmQueryMediaInfo) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->strmQueryMediaInfo(ch, strm, pMediaInfo));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

exCode_e 
adamStrmResetMediaInfo(dspId_e dsp, chanId_e ch, strmId_e strm){
	adamLog(EVA_LOG_DBG, "ENTER");

	if (dsp < ADAM_DSP_NUM && ch < gDspFeature[dsp].numOfChan \
		&& strm < gDspFeature[dsp].strmsPerChan) {
		if (pAdamDspSet[dsp]->strmResetMediaInfo) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[dsp]->strmResetMediaInfo(ch, strm));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}



/* ==================================== *
 *
 * ADAM Interface Control Section
 *
 * ==================================== */

/* Query Interface configuration by Interface ID */
exCode_e 
adamInfcQuery(infcId_e infc, infcConfig_t *pInfcConf) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (infc < ADAM_INFC_NUM) {
		if (pAdamDspSet[DSP_ID]->infcQuery) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[DSP_ID]->infcQuery(infc, pInfcConf));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* Configure Interface Line state by Interface ID */
exCode_e 
adamInfcConfigLine(infcId_e infc, lineState_e state) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (infc < ADAM_INFC_NUM) {
		if (pAdamDspSet[DSP_ID]->infcConfigLine) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[DSP_ID]->infcConfigLine(infc, state));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* Configure Interface Hook state by Interface ID */
exCode_e 
adamInfcConfigHook(infcId_e infc, hookState_e state) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (infc < ADAM_INFC_NUM) {
		if (pAdamDspSet[DSP_ID]->infcConfigHook) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[DSP_ID]->infcConfigHook(infc, state));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* Configure Interface Ring cadence by Interface ID */
exCode_e 
adamInfcConfigRing(infcId_e infc, ringProfile_t *pRingProf) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (infc < ADAM_INFC_NUM) {
		if (pAdamDspSet[DSP_ID]->infcConfigRing) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[DSP_ID]->infcConfigRing(infc, pRingProf));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

exCode_e 
adamInfcRing(infcId_e infc, uint32 dur, cid_t *pCid) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (infc < ADAM_INFC_NUM) {
		if (pAdamDspSet[DSP_ID]->infcRing) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[DSP_ID]->infcRing(infc, dur, pCid));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

exCode_e 
adamInfcStopRing(infcId_e infc) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (infc < ADAM_INFC_NUM) {
		if (pAdamDspSet[DSP_ID]->infcStopRing) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[DSP_ID]->infcStopRing(infc));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

exCode_e 
adamInfcConfigHookTs(infcId_e infc, hookThreshold_t *pHookTs) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (infc < ADAM_INFC_NUM) {
		if (pAdamDspSet[DSP_ID]->infcConfigHookTs) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[DSP_ID]->infcConfigHookTs(infc, pHookTs));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}
exCode_e
adamInfcConfigHwProf(infcId_e infc,hwProf_e hwProfId) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (infc < ADAM_INFC_NUM) {
		if (pAdamDspSet[DSP_ID]->infcConfigHwProf) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[DSP_ID]->infcConfigHwProf(infc, hwProfId));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/*Support ring parameters set. */
exCode_e
adamInfcRingParams(infcId_e infc, infcRingParams_t* ringParams) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (infc < ADAM_INFC_NUM) {
		if (pAdamDspSet[DSP_ID]->infcRingParams) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[DSP_ID]->infcRingParams(infc, ringParams));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/*Support line test. */
exCode_e
adamInfcLineTest(infcLineTest_t*  lineTest) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (lineTest->infc  <  ADAM_INFC_NUM) {
		if (pAdamDspSet[DSP_ID]->infcLineTest) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[DSP_ID]->infcLineTest(lineTest));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

exCode_e
adamInfcSlicTypeQuery(infcId_e infc, slicParams_t* slicParams) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (infc < ADAM_INFC_NUM) {
		if (pAdamDspSet[DSP_ID]->infcSlicTypeQuery) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[DSP_ID]->infcSlicTypeQuery(infc, slicParams));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}



/*Support DC feed parameters set. */
exCode_e
adamInfcDcFeedParams(infcId_e infc, infcDcFeedParams_t* dcFeedParams) {
	adamLog(EVA_LOG_DBG, "ENTER");

	if (infc < ADAM_INFC_NUM) {
		if (pAdamDspSet[DSP_ID]->infcDcFeedParams) {
			adamLog(EVA_LOG_DBG, "EXIT");
			return (pAdamDspSet[DSP_ID]->infcDcFeedParams(infc, dcFeedParams));
		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}

/* China Unicom inside and outside line test */
exCode_e
adamInfcCULineTest(infcLineTest_t* lineTest, int slicType, infcUnicomLineTest_t* unicomLineTest) {
	adamLog(EVA_LOG_DBG, "ENTER");
	
	EVA_LtLineVResultType *pLtLineVResultType = NULL;
	EVA_LtRingersResultType *pLtRingersResultType = NULL;
	EVA_LtResFltResultType *pLtResFltResultType = NULL;
	EVA_LtCapResultType *pLtCapResultType = NULL;
	EVA_LtRdLoopCondResultType *pLtRdLoopCondResultType = NULL;
	EVA_ProSLICMLTType *pProSLICMLT = NULL;
	EVA_proslicMonitorType *pproslicMonitor = NULL;
	

	if (lineTest->infc	<  ADAM_INFC_NUM) {
		if (pAdamDspSet[DSP_ID]->infcLineTest){
			adamLog(EVA_LOG_DBG, "EXIT");
			
			if(slicType == TYPE_ZARLINK ){

				lineTest->lineTestId = EVA_LT_TID_RINGERS;
				memset(lineTest->lineTestData, 0, 5000);
				pAdamDspSet[DSP_ID]->infcLineTest(lineTest);
			    pLtRingersResultType=(EVA_LtRingersResultType*)lineTest->lineTestData;
				if(pLtRingersResultType->fltMask == ZarlinkOnHook){
				
					lineTest->lineTestId = EVA_LT_TID_LINE_V;
					memset(lineTest->lineTestData, 0, 5000);
					pAdamDspSet[DSP_ID]->infcLineTest(lineTest);
				    pLtLineVResultType=(EVA_LtLineVResultType*)lineTest->lineTestData;
					unicomLineTest->Outline.AGACVoltage = pLtLineVResultType->vAcTip;
					unicomLineTest->Outline.BGACVoltage = pLtLineVResultType->vAcRing;
					unicomLineTest->Outline.ABACVoltage = pLtLineVResultType->vAcDiff;
					unicomLineTest->Outline.AGDCVoltage = pLtLineVResultType->vDcTip;
					unicomLineTest->Outline.BGDCVoltage = pLtLineVResultType->vDcRing;
					unicomLineTest->Outline.ABDCVoltage = pLtLineVResultType->vDcDiff;
					
					lineTest->lineTestId = EVA_LT_TID_RES_FLT;
					memset(lineTest->lineTestData, 0, 5000);
					pAdamDspSet[DSP_ID]->infcLineTest(lineTest);
					pLtResFltResultType=(EVA_LtResFltResultType*)lineTest->lineTestData;
					unicomLineTest->Outline.AGInsuluationResistance = abs(pLtResFltResultType->rtg/10);
					unicomLineTest->Outline.BGInsuluationResistance = abs(pLtResFltResultType->rrg/10);
					unicomLineTest->Outline.ABInsuluationResistance = abs(pLtResFltResultType->rtr/10);
					
					lineTest->lineTestId = EVA_LT_TID_CAP;
					memset(lineTest->lineTestData, 0, 5000);
					pAdamDspSet[DSP_ID]->infcLineTest(lineTest);
					pLtCapResultType=(EVA_LtCapResultType*)lineTest->lineTestData;
					unicomLineTest->Outline.AGCapacitance = pLtCapResultType->ctg;
					unicomLineTest->Outline.BGCapacitance = pLtCapResultType->crg;
					unicomLineTest->Outline.ABCapacitance = pLtCapResultType->ctr;

					if(unicomLineTest->Outline.AGACVoltage > 50000 || unicomLineTest->Outline.BGACVoltage > 50000){
						unicomLineTest->OutlineStatus=OutLine_PowerLineContacted;
					}
					else if(unicomLineTest->Outline.AGDCVoltage >40000 || unicomLineTest->Outline.BGDCVoltage > 40000){
						if(unicomLineTest->Outline.AGDCVoltage >40000)
							unicomLineTest->OutlineStatus=OutLine_ALineMixOther;
						if(unicomLineTest->Outline.BGDCVoltage >40000)
							unicomLineTest->OutlineStatus=OutLine_BLineMixOther;
						if(unicomLineTest->Outline.AGDCVoltage >40000 && unicomLineTest->Outline.BGDCVoltage > 40000)
							unicomLineTest->OutlineStatus=OutLine_BothLineContacted;
					}
					else if(unicomLineTest->Outline.AGInsuluationResistance < 20000 || unicomLineTest->Outline.BGInsuluationResistance < 20000){
						if(unicomLineTest->Outline.AGInsuluationResistance < 20000)
							unicomLineTest->OutlineStatus=OutLine_ALineGrounding;
						if(unicomLineTest->Outline.BGInsuluationResistance < 20000)
							unicomLineTest->OutlineStatus=OutLine_BLineGrounding;
						if(unicomLineTest->Outline.AGInsuluationResistance < 20000 && unicomLineTest->Outline.BGInsuluationResistance < 20000)
							unicomLineTest->OutlineStatus=OutLine_BothLineGrounding;
					}
					else if(unicomLineTest->Outline.ABInsuluationResistance >= 2000 && unicomLineTest->Outline.ABInsuluationResistance <= 20000){
						unicomLineTest->OutlineStatus=OutLine_ABLinePoorInsulation;
					}
					else if(unicomLineTest->Outline.ABInsuluationResistance < 500){
						unicomLineTest->OutlineStatus=OutLine_ShortCircuit;
					}
					else if(unicomLineTest->Outline.AGCapacitance > 700000 || unicomLineTest->Outline.BGCapacitance > 700000){
						if(unicomLineTest->Outline.AGCapacitance > 700000)
							unicomLineTest->OutlineStatus=OutLine_ALineLeakageToGround;
						if(unicomLineTest->Outline.BGCapacitance > 700000)
							unicomLineTest->OutlineStatus=OutLine_BLineLeakageToGround;
						if(unicomLineTest->Outline.AGCapacitance > 700000 && unicomLineTest->Outline.BGCapacitance > 700000)
							unicomLineTest->OutlineStatus=OutLine_BothLineLeakageToGround;
					}
					else
						unicomLineTest->OutlineStatus=OutLine_Normal;
				}
				else if(pLtRingersResultType->fltMask == ZarlinkPhoneDisconnet){
					unicomLineTest->OutlineStatus=OutLine_PhoneDisconnet;
				}
				else{
					unicomLineTest->OutlineStatus=OutLine_PhoneoffHook;
				}

				lineTest->lineTestId = EVA_LT_TID_RD_LOOP_COND;
				memset(lineTest->lineTestData, 0, 5000);
				pAdamDspSet[DSP_ID]->infcLineTest(lineTest);
				pLtRdLoopCondResultType=(EVA_LtRdLoopCondResultType*)lineTest->lineTestData;
				unicomLineTest->Inline.DCFeed_Voltage = abs(pLtRdLoopCondResultType->vab/1000);
				unicomLineTest->Inline.Ringing_Voltage = abs((pLtRdLoopCondResultType->vbat1 - pLtRdLoopCondResultType->vbat2)/1000);
				unicomLineTest->Inline.Loop_Current = pLtRdLoopCondResultType->imt/1000;
				
				if(unicomLineTest->Inline.DCFeed_Voltage >= 35 && unicomLineTest->Inline.DCFeed_Voltage <= 65)
					unicomLineTest->InlineDCVStatus = Inline_Test_Item_Normal;
				else
					unicomLineTest->InlineDCVStatus = Inline_Test_Item_UNNormal;
				
				if(unicomLineTest->Inline.Ringing_Voltage >= 40 && unicomLineTest->Inline.Ringing_Voltage <= 70)
					unicomLineTest->InlineRingVStatus = Inline_Test_Item_Normal;
				else
					unicomLineTest->InlineRingVStatus = Inline_Test_Item_UNNormal;
				
				if(unicomLineTest->Inline.Loop_Current >= 0 && unicomLineTest->Inline.Loop_Current <= 60)
					unicomLineTest->InlineCurrentStatus = Inline_Test_Item_Normal;
				else
					unicomLineTest->InlineCurrentStatus = Inline_Test_Item_UNNormal;

				return EXEC_SUCCESS;
				
			}
			else if(slicType == TYPE_SILICON ){

				lineTest->lineTestId = EVA_LT_TID_RES_OFFHOOK;
				pAdamDspSet[DSP_ID]->infcLineTest(lineTest);
				pProSLICMLT=(EVA_ProSLICMLTType*)lineTest->lineTestData;
				if(pProSLICMLT->roh.rohTrue == 0){
				
					lineTest->lineTestId = EVA_LT_TID_REN_CAP;
					memset(pProSLICMLT, 0, 5000);
					pAdamDspSet[DSP_ID]->infcLineTest(lineTest);
					pProSLICMLT=(EVA_ProSLICMLTType*)lineTest->lineTestData;
					if(pProSLICMLT->capFaults.measTR2 > 175){
						
						lineTest->lineTestId = EVA_LT_TID_HAZARDOUS_V;
						memset(pProSLICMLT, 0, 5000);
						pAdamDspSet[DSP_ID]->infcLineTest(lineTest);
						pProSLICMLT=(EVA_ProSLICMLTType*)lineTest->lineTestData;
						if(pProSLICMLT->hazVAC.measTG > 0)
						unicomLineTest->Outline.AGACVoltage = sqrt(pProSLICMLT->hazVAC.measTG)*100;
						if(pProSLICMLT->hazVAC.measRG > 0)
						unicomLineTest->Outline.BGACVoltage = sqrt(pProSLICMLT->hazVAC.measRG)*100;
						if(pProSLICMLT->hazVAC.measTR > 0)
						unicomLineTest->Outline.ABACVoltage = sqrt(pProSLICMLT->hazVAC.measTR)*100;						
						unicomLineTest->Outline.AGDCVoltage = pProSLICMLT->hazVDC.measTG;
						unicomLineTest->Outline.BGDCVoltage = pProSLICMLT->hazVDC.measRG;
						unicomLineTest->Outline.ABDCVoltage = pProSLICMLT->hazVDC.measTR;

						lineTest->lineTestId = EVA_LT_TID_RESISTIVE_FAULTS;
						memset(pProSLICMLT, 0, 5000);
						pAdamDspSet[DSP_ID]->infcLineTest(lineTest);
						pProSLICMLT=(EVA_ProSLICMLTType*)lineTest->lineTestData;
						unicomLineTest->Outline.AGInsuluationResistance = abs(pProSLICMLT->resFaults.measTG/10);
						unicomLineTest->Outline.BGInsuluationResistance = abs(pProSLICMLT->resFaults.measRG/10);
						unicomLineTest->Outline.ABInsuluationResistance = abs(pProSLICMLT->resFaults.measTR/10);
						
						lineTest->lineTestId = EVA_LT_TID_CAP_FAULTS;
						memset(pProSLICMLT, 0, 5000);
						pAdamDspSet[DSP_ID]->infcLineTest(lineTest);
						pProSLICMLT=(EVA_ProSLICMLTType*)lineTest->lineTestData;
						unicomLineTest->Outline.AGCapacitance = pProSLICMLT->capFaults.measTG*100;
						unicomLineTest->Outline.BGCapacitance = pProSLICMLT->capFaults.measRG*100;
						unicomLineTest->Outline.ABCapacitance = pProSLICMLT->capFaults.measTR*100;

						if(unicomLineTest->Outline.AGACVoltage > 50000 || unicomLineTest->Outline.BGACVoltage > 50000){
							unicomLineTest->OutlineStatus=OutLine_PowerLineContacted;
						}
						else if(unicomLineTest->Outline.AGDCVoltage >40000 || unicomLineTest->Outline.BGDCVoltage > 40000){
							if(unicomLineTest->Outline.AGDCVoltage >40000)
								unicomLineTest->OutlineStatus=OutLine_ALineMixOther;
							if(unicomLineTest->Outline.BGDCVoltage >40000)
								unicomLineTest->OutlineStatus=OutLine_BLineMixOther;
							if(unicomLineTest->Outline.AGDCVoltage >40000 && unicomLineTest->Outline.BGDCVoltage > 40000)
								unicomLineTest->OutlineStatus=OutLine_BothLineContacted;
						}
						else if(unicomLineTest->Outline.AGInsuluationResistance < 20000 || unicomLineTest->Outline.BGInsuluationResistance < 20000){
							if(unicomLineTest->Outline.AGInsuluationResistance < 20000)
								unicomLineTest->OutlineStatus=OutLine_ALineGrounding;
							if(unicomLineTest->Outline.BGInsuluationResistance < 20000)
								unicomLineTest->OutlineStatus=OutLine_BLineGrounding;
							if(unicomLineTest->Outline.AGInsuluationResistance < 20000 && unicomLineTest->Outline.BGInsuluationResistance < 20000)
								unicomLineTest->OutlineStatus=OutLine_BothLineGrounding;
						}
						else if(unicomLineTest->Outline.ABInsuluationResistance >= 2000 && unicomLineTest->Outline.ABInsuluationResistance <= 20000){
							unicomLineTest->OutlineStatus=OutLine_ABLinePoorInsulation;
						}
						else if(unicomLineTest->Outline.ABInsuluationResistance < 500){
							unicomLineTest->OutlineStatus=OutLine_ShortCircuit;
						}
						else if(unicomLineTest->Outline.AGCapacitance > 700000 || unicomLineTest->Outline.BGCapacitance > 700000){
							if(unicomLineTest->Outline.AGCapacitance > 700000)
								unicomLineTest->OutlineStatus=OutLine_ALineLeakageToGround;
							if(unicomLineTest->Outline.BGCapacitance > 700000)
								unicomLineTest->OutlineStatus=OutLine_BLineLeakageToGround;
							if(unicomLineTest->Outline.AGCapacitance > 700000 && unicomLineTest->Outline.BGCapacitance > 700000)
								unicomLineTest->OutlineStatus=OutLine_BothLineLeakageToGround;
						}
						else
							unicomLineTest->OutlineStatus=OutLine_Normal;
					}
					else{
						unicomLineTest->OutlineStatus=OutLine_PhoneDisconnet;
					}
				}
				else{
					unicomLineTest->OutlineStatus=OutLine_PhoneoffHook;
				}

				lineTest->lineTestId = EVA_ProSLIC_LineMonitor;
				memset(pProSLICMLT, 0, 5000);
				pAdamDspSet[DSP_ID]->infcLineTest(lineTest);
				pproslicMonitor=(EVA_proslicMonitorType*)lineTest->lineTestData;
				unicomLineTest->Inline.DCFeed_Voltage = abs(pproslicMonitor->vtr/1000);
				unicomLineTest->Inline.Ringing_Voltage = abs(pproslicMonitor->vbat/1000);
				unicomLineTest->Inline.Loop_Current = pproslicMonitor->itr/1000;

				if(unicomLineTest->Inline.DCFeed_Voltage >= 35 && unicomLineTest->Inline.DCFeed_Voltage <= 65)
					unicomLineTest->InlineDCVStatus = Inline_Test_Item_Normal;
				else
					unicomLineTest->InlineDCVStatus = Inline_Test_Item_UNNormal;
				
				if(unicomLineTest->Inline.Ringing_Voltage >= 40 && unicomLineTest->Inline.Ringing_Voltage <= 70)
					unicomLineTest->InlineRingVStatus = Inline_Test_Item_Normal;
				else
					unicomLineTest->InlineRingVStatus = Inline_Test_Item_UNNormal;
				
				if(unicomLineTest->Inline.Loop_Current >= 0 && unicomLineTest->Inline.Loop_Current <= 60)
					unicomLineTest->InlineCurrentStatus = Inline_Test_Item_Normal;
				else
					unicomLineTest->InlineCurrentStatus = Inline_Test_Item_UNNormal;
				
				return EXEC_SUCCESS;
				
			}
			else{
				adamLog(EVA_LOG_ERR, "EXIT");
				return EXEC_FAIL;
			}

		}
		else {
			adamLog(EVA_LOG_ERR, "EXIT");
			return FUNC_UNSUPPORT;
		}
	}
	adamLog(EVA_LOG_ERR, "EXIT");
	return INVALID_PARAM;
}





