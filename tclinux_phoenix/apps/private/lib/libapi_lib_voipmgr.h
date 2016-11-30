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
#ifndef _API_LIB_VOIPMGR_H
#define _API_LIB_VOIPMGR_H

/************************************************************************
*                  D A T A	 T Y P E S
************************************************************************/
typedef unsigned char			unchar;
typedef unsigned short			ushort;
typedef unsigned int			uint;
typedef unsigned long			ulong;

/************************************************************************
*                  I N C L U D E S
************************************************************************/

#include <adam.h>


/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
************************************************************************/
//#define VOIPMGR_SUCCESS	          ( 0)
//#define VOIPMGR_FAIL	          (-1)
//#define VOIPMGR_INVALID_PARAM	  (-2)
//#define VOIPMGR_FUNC_UNSUPPORT	  (-3)
//#define VOIPMGR_DEVICE_BUSY	      (-4)
//#define VOIPMGR_UNKNOWN_ERROR	  (-5)
/* Execution Return Code */
typedef enum { 
	ECNT_E_SUCCESS = 0,
	ECNT_E_FAIL = -1,
	ECNT_E_INVALID_PARAM = -2,
	ECNT_E_FUNC_UNSUPPORT = -3,
	ECNT_E_DEVICE_BUSY = -4,
	ECNT_E_UNKNOWN_ERROR = -5
} ECNT_LIB_RET;


typedef struct {
	dspId_e   dspId;
	chanId_e  chanId;
	strmId_e  strmId;
} dspContext_t;

/************************************************************************
*                  M A C R O S
************************************************************************/

/************************************************************************
*                  E X T E R N A L	 D A T A   D E C L A R A T I O N S
************************************************************************/

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
************************************************************************/
/* ==================================== *
*
* VoIPMGR Generic Management Section
*
* ==================================== */

/* Allocate VoIPMGR resource and activate VoIPMGR process. */
ECNT_LIB_RET voipmgr_lib_init(void);

/*Deactivate VoIPMGR process and release resource. */
ECNT_LIB_RET voipmgr_lib_exit(void);

/* Get VoIPMGR's configuration (adamConfig) informaiton */
ECNT_LIB_RET voipmgr_lib_query(adamConfig_t *pAdamConf);

ECNT_LIB_RET voipmgr_lib_poll_event(blockMode_e mode, event_t *pEvent);

/* ==================================== *
 *
 * DSP Control Section
 *
 * ==================================== */
/* Invoke (load and initialize) DSP by DSP ID*/
ECNT_LIB_RET voipmgr_lib_dsp_invoke(dspContext_t *dspContext);

/* Revoke (terminate and unload) DSP by DSP ID */
ECNT_LIB_RET voipmgr_lib_dsp_revoke(dspContext_t *dspContext);

/* Query DSP support feature by DSP ID */
ECNT_LIB_RET voipmgr_lib_dsp_query(dspContext_t *dspContext, activeState_e *dspActive, dspFeature_t *pFeature);

/* Configure Tone profile by DSP ID */
ECNT_LIB_RET voipmgr_lib_dsp_config_tone(dspContext_t *dspContext, uint16 toneId, tone_t *pTone);

/* Configure CPT profile by DSP ID */
ECNT_LIB_RET voipmgr_lib_dsp_config_cpt(dspContext_t *dspContext, cpt_t *pCpt);

/* ==================================== *
 *
 * Channel Control Section
 *
 * ==================================== */
/* Query Channel configuration by DSP ID and Channel ID */
ECNT_LIB_RET voipmgr_lib_chan_query(dspContext_t *dspContext, chanConfig_t *pChanConf);

/* Configure Channel by DSP ID and Channel ID */
ECNT_LIB_RET voipmgr_lib_chan_config(dspContext_t *dspContext, chanConfig_t *pChanConf);

/*for dsp replace pcm data to a special file*/
ECNT_LIB_RET voipmgr_lib_dsp_replace_data(dspContext_t *dspContext, replaceData_t *pReplaceConf);

/* Play (generate) tone to specific channel by DSP ID and Channel ID */
/*for tone_stop_event */
ECNT_LIB_RET voipmgr_lib_chan_play_tone(dspContext_t *dspContext, toneSeq_t *pToneSeq, uint32 repeat, uint32 dur);

/* for pcm loopback test. */
/* pcm loopback between ch 0 and ch 1, no dsp process and rtp process is involved 
** if pcm loopback is enabled*/
ECNT_LIB_RET voipmgr_lib_pcm_loopback(dspContext_t *dspContext, enableControl_e en); 

/* rtp loopback for a channel, work like a echo, the incomming rtp data is echoed back
** without dsp processing if rtp loopback of the channel is enabled*/
ECNT_LIB_RET voipmgr_lib_rtp_loopback(dspContext_t *dspContext, enableControl_e en);

/* dsp loopback for a channel, work like a echo, the incomming rtp data is echoed back
** after dsp processing if dsp loopback of the channel is enabled*/
ECNT_LIB_RET voipmgr_lib_dsp_loopback(dspContext_t *dspContext, enableControl_e en) ;

/* Play DTMF tone to specific channel by DSP ID and Channel ID */
ECNT_LIB_RET voipmgr_lib_chan_play_dtmf(dspContext_t *dspContext, char digit, uint32 dur);

/* Play (generate) tone sequence to specific channel by DSP ID and Channel ID */
ECNT_LIB_RET voipmgr_lib_chan_stop_tone(dspContext_t *dspContext);

/* Play (generate) Caller ID to specific channel by DSP ID and Channel ID */
ECNT_LIB_RET voipmgr_lib_chan_play_cid(dspContext_t *dspContext, cid_t *pCid);

ECNT_LIB_RET voipmgr_lib_chan_play_type2_cid(dspContext_t *dspContext, cid_t *pCid);

ECNT_LIB_RET voipmgr_lib_chan_dump_pcm(dspContext_t *dspContext, netAddr_t *pDstAddr);

ECNT_LIB_RET voipmgr_lib_chan_play_dtmf_cid(dspContext_t *dspContext, cid_t *pCid);

/* ==================================== *
 *
 * Stream Control Section
 *
 * ==================================== */
/* Query Stream configuration by DSP ID, Channel ID and Stream ID */
ECNT_LIB_RET voipmgr_lib_strm_query(dspContext_t *dspContext, activeState_e *pStrmActive, strmConfig_t *pStrmConf);

/* Configure Channel by DSP ID, Channel ID and Stream ID */
ECNT_LIB_RET voipmgr_lib_strm_config(dspContext_t *dspContext, strmConfig_t *pStrmConf);

/* Enable a streamming process by the configuration of the Stream */
ECNT_LIB_RET voipmgr_lib_strm_start(dspContext_t *dspContext);

/* Disable a streamming process */
ECNT_LIB_RET voipmgr_lib_strm_stop(dspContext_t *dspContext);

/* Disable all streamming process */
ECNT_LIB_RET voipmgr_lib_strm_stop_all(dspContext_t *dspContext);

/* Send DTMF relay packet (RFC2833/4733) to the session destination */
ECNT_LIB_RET voipmgr_lib_strm_send_dtmfr(dspContext_t *dspContext, dtmf_e dtmf, uint32 dur);

ECNT_LIB_RET voipmgr_lib_strm_play_tone(dspContext_t *dspContext, toneSeq_t *pToneSeq, uint32 repeat);

ECNT_LIB_RET voipmgr_lib_strm_stop_tone(dspContext_t *dspContext);

ECNT_LIB_RET voipmgr_lib_strm_query_media_info(dspContext_t *dspContext, mediaInfo_t *pMediaInfo);

ECNT_LIB_RET voipmgr_lib_strm_reset_media_info(dspContext_t *dspContext);

/* ==================================== *
 *
 * Interface Control Section
 *
 * ==================================== */
/* Query Interface configuration by Interface ID */
ECNT_LIB_RET voipmgr_lib_infc_query(infcId_e infc, infcConfig_t *pInfcConf);

/* Configure Interface Line state by Interface ID */
ECNT_LIB_RET voipmgr_lib_infc_config_line(infcId_e infc, lineState_e state);

/* Configure Interface Hook state by Interface ID */
ECNT_LIB_RET voipmgr_lib_infc_config_hook(infcId_e infc, hookState_e state);

/* Configure Interface Ring cadence by Interface ID */
ECNT_LIB_RET voipmgr_lib_infc_config_ring(infcId_e infc, ringProfile_t *pRingProf);

ECNT_LIB_RET voipmgr_lib_infc_ring(infcId_e infc, uint32 dur, cid_t *pCid);

ECNT_LIB_RET voipmgr_lib_infc_stop_ring(infcId_e infc);

/* Configure Interface Hook threshold by Interface ID */
ECNT_LIB_RET voipmgr_lib_infc_config_hook_ts(infcId_e infc, hookThreshold_t *pHookTs);

ECNT_LIB_RET voipmgr_lib_infc_config_hw_prof(infcId_e infc,hwProf_e hwProfId);

/*support ring parameters set. */
ECNT_LIB_RET voipmgr_lib_infc_ring_params(infcId_e infc, infcRingParams_t* ringParams);

/*support line test. */
ECNT_LIB_RET voipmgr_lib_infc_line_test(infcLineTest_t*  lineTest);


ECNT_LIB_RET voipmgr_lib_infc_slic_type_query(infcId_e infc, slicParams_t* slicParams);

/*support DC feed parameters set. */
ECNT_LIB_RET voipmgr_lib_infc_dc_feed_params(infcId_e infc, infcDcFeedParams_t* dcFeedParams) ;

ECNT_LIB_RET voipmgr_lib_infc_cu_line_test(infcLineTest_t* lineTest, int slicType, infcUnicomLineTest_t* unicomLineTest);
/************************************************************************
*                  P U B L I C   D A T A
************************************************************************/

/************************************************************************
*                  P R I V A T E   D A T A
************************************************************************/

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
************************************************************************/

#endif
