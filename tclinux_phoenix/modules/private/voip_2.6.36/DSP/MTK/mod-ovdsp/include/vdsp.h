
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
/**
 *  @file           vdsp.h
 *  @author         mtk02142
 *  @brief          vdsp header file used to communicate with user application,
 *                  mostly backward compatible with previous version of code.
 *  @date           2008.07.31
 */
#ifndef __VDSP_H__
#define __VDSP_H__

#include "vdsp_type.h"

/* CONSTANTS AND MACROS
*/

/* direction: uplink or downlink
*/
#define DIR_UL 0
#define DIR_DL 1

#define SW_ON	1
#define SW_OFF	0


#ifndef UINT8
#define UINT8
typedef unsigned char   uint8;
#endif

#ifndef UINT16
#define UINT16
typedef unsigned short  uint16;
#endif

#ifndef UINT32
#define UINT32
typedef unsigned long   uint32;
#endif

#ifndef UINT64
#define UINT64
typedef unsigned long long  uint64;
#endif


#ifndef INT8
#define INT8
typedef signed char    int8;
#endif

#ifndef INT16
#define INT16
typedef signed short int int16;
#endif

#ifndef INT32
#define INT32
typedef signed long  int32;
#endif

#ifndef INT64
#define INT64
typedef signed long long  int64;
#endif



/* DVCC control
*/
typedef enum e_dvcc_action {
	DVCC_NONE=0x0, /* nothing to do when detect, like off dvcc */
	DVCC_CODEC_CHANGE= (1<<0), /* default */
	DVCC_INDICATION= (1<<1),
	DVCC_BOTH= (1<<2), /* not suggestion to use this mode, cos' user process not to be garanteed */
} dvcc_action_e;

/* T38 related information
*/
#ifdef CONFIG_FAX_V34
/*pork*/
#define T38_FAX_VERSION		0
#define T38_MAX_BUFFER		72
#define T38_MAX_DATAGRAM	316
#define T38_JITTER_MIN		80	/* in ms */
#define T38_JITTER_MAX		500	/* in ms */
#define T38_DEF_MAX_RATE	T38_RATE_14400_BPS
#define T38_DEF_JITTER		210
#define T38_DEF_ECC_TYPE	T38_UDP_REDUNDANCY
#define T38_DEF_ECC_NUM		T38_REDUNDENT_TWO
#define T38_DEF_RATE_MGMT	T38_TRANSFERRED_TCF
#define T38_DEF_ECM_ENABLE	1
#define T38_DEF_PKT_SIZE    0
#define t38_DEF_REMOTE_UDPTL T38_MAX_DATAGRAM

#else
#define T38_FAX_VERSION		0
#define T38_MAX_BUFFER		72
#define T38_MAX_DATAGRAM	316
#define T38_JITTER_MIN		80	/* in ms */
#define T38_JITTER_MAX		500	/* in ms */
#define T38_DEF_MAX_RATE	T38_RATE_14400_BPS
#define T38_DEF_JITTER		210
#define T38_DEF_ECC_TYPE	T38_UDP_REDUNDANCY
#define T38_DEF_ECC_NUM		T38_REDUNDENT_TWO
#define T38_DEF_RATE_MGMT	T38_TRANSFERRED_TCF
#define T38_DEF_ECM_ENABLE	1
#define T38_DEF_PKT_SIZE    0
#define t38_DEF_REMOTE_UDPTL T38_MAX_DATAGRAM

#endif

#ifdef RFC2833_TONE_SUPPORT
#define TEV_DIAL_TONE		(66)
#define TEV_SECOND_DIAL_TONE (69)
#define TEV_RINGBACK_TONE	(70)
#define TEV_BUSY_TONE		(72)
#define TEV_CONGESTION_TONE (73)
#define TEV_CALLWAITING_TONE (79)
#define TEV_POSITIVE_IND_TONE (81)
#define TEV_NEGATIVE_IND_TONE (82)
#define TEV_CAS_TONE		 (87)
#endif
#if defined(VOICE_QUALITY_IMPROVE)
#define NOTIFY_JB_STATIC_FLAG  1
#define NOTIFY_JB_RECOVER_FLAG 2
#endif
/* TYPE DEFINITION
*/
typedef struct s_vdsp_event
{
	union
	{
		struct s_vdsp_event_dtmf
		{
			unsigned digit;
			int direction;
			int play;
			int duration;
			unsigned short volume;
			#if 1 //def CZ_DSP_SUPPORT /* added by mtk07059_20140724 */
			unsigned long jiff;
			#endif
			int sipInfoDur;
		} dtmf;
		
		struct s_vdsp_event_dvcc
		{
			int cid; /* channel id */
			int sid; /* session id */
			int playtime; /* playtime */
			int pt; /* payload type */
			int payload_len; /* received rtp codec payload length */
			int unit_codec_len; /* the codec unit frame length */
			int xmit_mode; /* the session transmit mode */				
			int bitrate; /* bitrate */
			int vad_scheme; /* vad scheme */			
		} dvcc;

		struct s_vdsp_event_jb
		{
			int cid; /* channel id */
			int sid; /* session id */
			uint64 total;
			uint64 drop;
			int plc;
			int jbSize;
			int avgJitter;
		} jb;

		struct s_fax {
			unsigned int evt;	/* detail event of CNG, ANS, ANSbar, v.21 */
			int reserved1;
			int reserved2;
			int reserved3;
		} fax;	
		
#ifdef OSBN_VOIP
		struct s_ringDetect {
			unsigned int ringNum;
			int reserved1;
		} ringDetect;	
#endif	
	
	/* added by mtk07059_20151021, for debug log. */
		struct s_log {
			int module;
			int adamLevel;
			int gdiLevel;
			int printType; /*0:default print, 1:tcdbg_printf */   
		} logSwitch;	

	} event;
} T_VDSP_EVENT_DATA;

#define VDSP_EVENT_DATA_SIZE (sizeof(T_VDSP_EVENT_DATA))

/* T38 related types
*/

typedef enum e_data_rate
{
	T38_RATE_2400_BPS = 0,
	T38_RATE_4800_BPS,
	T38_RATE_7200_BPS,
	T38_RATE_9600_BPS,
	T38_RATE_12000_BPS,
	T38_RATE_14400_BPS,
	T38_RATE_16800_BPS,
	T38_RATE_19200_BPS,
	T38_RATE_21600_BPS,
	T38_RATE_24000_BPS,
	T38_RATE_26400_BPS,	
	T38_RATE_28800_BPS,	
	T38_RATE_31200_BPS,	
	T38_RATE_33600_BPS,		
} E_DATE_RATE;

#ifdef RFC2833_TONE_SUPPORT
typedef enum e_rfc2833_tone_type
{
	TEV_MEDIA_DIAL_TONE = 255,
	TEV_MEDIA_BUSY_TONE,
	TEV_MEDIA_CONGESTION_TONE,
	TEV_MEDIA_RINGBACK_TONE,
	TEV_MEDIA_REORDER_TONE,
	TEV_MEDIA_CALLWAITING_TONE,
	TEV_MEDIA_IDENTIFICAION_TONE,
	TEV_MEDIA_NEGATIVE_IND_TONE,
	TEV_MEDIA_POSITIVE_IND_TONE,
	TEV_MEDIA_STUTTER_DIAL_TONE,
	TEV_MEDIA_HORNING_TONE,
	TEV_MEDIA_SECOND_DIAL_TONE,
	TEV_MEDIA_CAS_TONE,
} E_RFC2833_TONE_TYPE;
#endif

typedef enum e_error_correct
{
	T38_UDP_ECC_TYPE_NONE = 0,
	T38_UDP_REDUNDANCY = 1,
	T38_UDP_FEC /* not suppport */
	
} E_ERROR_CORRECT;

typedef enum e_redundent_num
{
	T38_REDUNDENT_ZERO = 0,
	T38_REDUNDENT_ONE=1,
	T38_REDUNDENT_TWO=2,
	T38_REDUNDENT_3=3,
	T38_REDUNDENT_4=4,
	
} E_REDUNDENT_NUM;

typedef enum e_rate_management
{
	T38_LOCAL_TCF = 1,
	T38_TRANSFERRED_TCF
	
} E_RATE_MANAGEMENT;

typedef enum e_jitter_type
{
	JBT_VOICE=0,
	JBT_T38,
	JBT_FAX
} jitter_type_e;

typedef struct s_t38_ctrl_info
{
#if 1
	int version;
	int connection_type;
	int max_rate;
	int max_jitter;
	int ecc_type;
	int ecc_signal;
	int ecc_num;
	int rate_management;
	int ecm_enable;
	int nsf_enable; 
	int op_mode;
	int imagePktSize;
	int maxRemoteUDPTL;
#else
	int max_rate;
	int max_jitter;
	int ecc_type;
	int ecc_num;
	int rate_management;
	int ecm_enable;
	int op_mode;
#endif	
	
} T_T38_CTRL_INFO;

typedef struct s_xlink_info {
	int codecType;
	int playTime;
	int bitrate;
} xlink_info_t;

typedef struct s_vdsp_features
{
	int dynamic_vcodec_change;
	int rtp_ignore_markbit;
	int rtcp_report_interval;
	int dvcc_action;
} vdsp_features_t;

typedef struct s_jitter {
	int type; /* voice or t38 */
	int len; /* default length */
	int scheme; /* dynamic or static */
} jitterctl_t;

typedef struct {
	uint64	rtpError;
	uint64	packetRecv;
	uint64	packetLoss;
	uint64	packetLossRate;
	/* Add by mtk06404 20160223 */
	uint64  packetOutoftime;  /* number of packets that were received too late */
	uint64  packetDiscarded;  /* incoming packets discarded because the queue exceeds its max size */
	/* Add by mtk06404 20160223 */
	uint64	maxJitter;
	uint64	maxRtcpInterval;
	/*add by kidd OSBNB00030595 20130407*/
	uint64 	packetSent;
	uint64 	bytesSent;		
	uint64 	bytesRecv;
	uint64	farEndpacketLossRate;	
	uint64	avgJitter;	
	uint64	farEndJitter;
	uint64	avgFarEndJitter;
	uint64 	avgRTCPInterval;
	/*end of add by kidd OSBNB00030595 20130407*/	
	uint64	bufUnderflow;
	uint64	bufOverflow;
	uint64	revInternalJitter;
	uint64	avgRevInternalJitter;
	uint64	roundTripDelay;
	uint64 	avgRoundTripDelay;
    	uint64  maxRoundTripDelay;   /*max value of round trip delay */
    	uint64  rtcpPacketRecv;      /*number of ingress rtcp packets*/
    	uint64  rtcpPacketSend;      /*number of egress rtcp packets*/
    	uint64  rtcpXrPacketRecv;    /*number of ingress rtcp XR packets*/
    	uint64  rtcpXrPacketSend;    /*number of egress rtcp XR packets*/
    	float   MOSLQ; /*MOS_Listening quality*/
    	float   MOSCQ; /*MOS_Conversational quality*/
}vdspMediaInfo_t;


typedef struct  s_vdsp_ioctl_param
{
	int cid;
	int sid;
	int ptime;

	/* specify for the codec's payload type setting */
	int codec_pt;
	int codec_type;
	
	unsigned long codec_caps;
	T_NWINFO local;
	T_NWINFO remote;
	int xmit_mode; /* transmit mode */
	int vad_scheme;
	int rfc2833_digit;
	int rfc2833_end;
	unsigned short rfc2833_dtmf_power;
	int dscp;	/* dscp setting for this session (cid, sid) */
	int d802_1p;    /* d802_1p setting for this session (cid, sid) */
	#ifdef CZ_DSP_SUPPORT
	/* Added by mtk07059_20141020, for Qos queue and 802.1p */
	int vlan_prio;
	int queue_num;
	#endif
	int t38; /* indicate if this media is created for T38 FAX relay */
	T_T38_CTRL_INFO t38_ctrl; /* if media is for T38, this field is used to configure it */

	xlink_info_t ul;
	xlink_info_t dl;

	vdsp_features_t features;

	jitterctl_t jitterctl;
	int dvcc_enable;
	int plc_enable;
	int caller; /* only for T38 */
	int redundant_enable; /* mtk69097_for_redundant */
	int redundant_mode; /* mtk07059_20150320 */
	int rfc2833_redundant_enable;
	int rfc2833_redundant_mode;
	vdspMediaInfo_t mediaInfo;
} T_VDSP_IOCTL_PARAM;

typedef struct vdsp_listener 
{
	void (*event_on)(void* o, unsigned cid, unsigned e, unsigned e_data);
	void* priv;
} vdsp_listener_t;

typedef struct vdsp vdsp_t;
struct vdsp
{
	/** 
	 *	@brief media initial
	 */
	int (*mediaInit)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief media start
	 */
	int (*mediaStart)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief media start
	 */
	int (*mediaStop)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief media xmit mode set
	 */
	int (*mediaXmitModeSet)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief media xmit mode get
	 */
	int (*mediaXmitModeGet)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief retrieve the voice codec payload type
	 */
	int (*vcodecPayloadTypeGet)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief configure the voice codec payload type
	 */
	int (*vcodecPayloadTypeSet)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);
	
	/** 
	 *	@brief rtp redundant set //mtk69097_for_redundant
	 */
	int (*rtpRedundantSet)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);
	
	/** 
	 *	@brief retrieve the voice codec capability of the device
	 */
	int (*vcodecCapGet)(vdsp_t* o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *  @brief rfc2833 set  
	 */
	int (*rfc2833Set)(vdsp_t *o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *  @brief rfc2833  DTMF volume set  
	 */
	int (*rfc2833DtmfVolumeSet)(vdsp_t *o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief media rtp send count
	 */
	int (*mediaRtpSendCountGet)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief media rtp receive count
	 */
	int (*mediaRtpRecvCountGet)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief media rtp send count
	 */
	int (*mediaRtpSendByteCountGet)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief media rtp receive count
	 */
	int (*mediaRtpRecvByteCountGet)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief calcuate the real packet time for dynamic codec change
	 */
	int (*mediaCurrSessionPtimeCalc)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief the listener shall receive a event to report vdsp event
	 */
	int (*setListener)(vdsp_t* o, vdsp_listener_t* listener);
	/** 
	 *	@clean vdsp event
	 */
	int (*vdspEqClean)(vdsp_t *o);
	
	/** 
	 *	@brief configure the voice maximum jitter size
	 */
	//int (*mediaVoiceJitterEnable)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief configure the voice minimum jitter size
	 */
	int (*mediaVoiceJitterSchemeSet)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief configure the voice jitter buffer dynamic and static
	 */
	int (*mediaVoiceJitterDefaultLengthSet)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief configure the voice maximum jitter size
	 */
	int (*mediaVoiceJitterMaximumLengthGet)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief configure the voice minimum jitter size
	 */
	int (*mediaT38JitterDefaultLengthSet)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief configure the voice jitter buffer dynamic and static
	 */
	int (*mediaVoiceDVCCEnable)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief configure the session ignore the markbit
	 */
	int (*mediaIgnoreMarkbitSet)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/** 
	 *	@brief query Media Info for G.988
	 */
	int (*queryMediaInfo)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);

	/**  
	 *	@brief reset Media Info for G.988
	 */
	int (*resetMediaInfo)(vdsp_t*o, T_VDSP_IOCTL_PARAM *pparams);
	
	void* priv;
};

typedef enum T38_STREAM_FLAG
{
    T38_STREAM_TURE,
    T38_STREAM_FALSE,
    T38_STREAM_ERROR
    
}T38_STREAM_FLAG_E;

#define MAX_CHANNEL_NUMBER 4  /*upgrade_to_4_channel*/


vdsp_t* VDSP_GetInstance(void);





#endif /* __VDSP_H__ */

/* 
 * $Log: vdsp.h,v $
 * Revision 1.21.2.12.2.1  2012-02-15 09:39:44  mtk01662
 * CQ:WIMAX0000????
 * T38v3 check-in
 *
 * Revision 1.21.2.12  2010-12-27 13:53:39  mtk01662
 * CQ:WIMAX00009606 VOIP DVCC indicator
 *
 * Revision 1.21.2.11  2010/12/08 06:33:15  mtk01662
 * CQ:WIMAX0000????
 * 1. add markbit decision for the session
 * 2. the xmit mode change, will concern the ignore bit set
 *
 * Revision 1.21.2.10  2010/10/18 07:30:02  mtk01662
 * CQ:WIMAX00006979 [ORTP] configurable RTCP interval
 *
 * Revision 1.21.2.9  2010/10/07 14:08:42  mtk01662
 * CQ:WIMAX00007825 [Huawei] ignore marker bit to play hold music
 *
 * Revision 1.21.2.8  2010/08/03 13:06:01  mtk01662
 * CQ:WIMAX00007230 allow jitter buffer to be set as 0 to disable
 *
 * Revision 1.21.2.7  2010/05/03 09:08:51  mtk01662
 * CQ:WIMAX00005871 [Huawei] voice static jitter and dynamic jitter buffer size adjust and web configuration
 * CQ:WIMAX00005873 [Huawei] PLC enable/disable web configuration and API
 *
 * Revision 1.21.2.6  2010/02/26 09:11:12  mtk01662
 * CQ:WIMAX00005664
 *
 * Revision 1.21.2.5  2009/12/17 02:21:00  mtk01662
 * CQ:WIMAX00004722
 *
 * Revision 1.21.2.4  2009/11/25 04:13:56  mtk01662
 * CQ:WIMAX00004286
 *
 * Revision 1.21.2.3  2009/08/10 13:33:33  mtk01662
 * *: take off lec from ovdsp
 *
 * Revision 1.21.2.2  2009/06/11 09:45:38  mtk02142
 * Complete rfc2833 and fax disconnect event notification.
 *
 * Revision 1.21.2.1  2009/04/27 03:40:26  mtk01662
 * *: vdsp kpc
 *
 * Revision 1.21  2009/03/09 08:36:10  mtk02142
 * Seperate type definitions for SDK to vdsp_type.h.
 *
 * Revision 1.20  2009/02/20 08:32:42  mtk02142
 * Fix typing error.
 *
 * Revision 1.19  2009/02/16 07:46:46  mtk02142
 * Change default jitter for T38 from 80 to 210.
 *
 * Revision 1.18  2009/02/16 07:03:25  mtk02142
 * Add default constants for use by FAX relay.
 *
 * Revision 1.17  2009/02/13 06:57:55  mtk02142
 * Add needed control information for FAX relay (T38).
 *
 * Revision 1.16  2008/12/24 06:17:12  mtk01662
 * *: VDSP_MEDIA_RTP_SENDCOUNT rename to VDSP_MEDIA_RTP_SEND_COUNT
 *
 * Revision 1.15  2008/12/22 05:28:43  mtk02142
 * Take off unused I/O control and synchronize I/O control definition and function table.
 *
 * Revision 1.14  2008/12/19 09:25:39  mtk01662
 * *: let lec mode set can only configure the lec tail length
 *
 * Revision 1.13  2008/12/15 12:58:48  mtk01662
 * +: add codec payload number configuration api
 *
 * Revision 1.12  2008/12/12 10:33:13  mtk01662
 * *: pass the codec bitrate to voice codec init
 *
 * Revision 1.11  2008/12/12 10:10:34  mtk01662
 * +: add session id to get rtp send/recv count
 *
 * Revision 1.10  2008/12/12 07:29:48  mtk01662
 * 1. merge G.7231 and iLBC voice codec to one and add bitrate parameters to seperate
 * 2. the bitrate define in vdsp.h
 *
 * Revision 1.9  2008/12/10 07:47:22  mtk02142
 * Add support for dscp setting.
 *
 * Revision 1.8  2008/12/10 07:06:41  mtk01662
 * +: add VDSP_LEC_MODE_SET ioctl contrl
 * +: add vad_scheme and lec_mode ioctl parameters
 *
 * Revision 1.7  2008/11/04 05:32:58  mtk01662
 * +: iLBC codec type VOIP_CODEC_ILBC_20 and VOIP_CODEC_ILBC_30
 *
 * Revision 1.6  2008/11/04 05:21:56  mtk01662
 * +: change iLBC payload type to 104, meet with P1
 *
 * Revision 1.5  2008/11/04 04:38:44  mtk01662
 * +: add PT_CODEC_iLBC = 100 and VOIP_CODEC_ILBC = 0X0800
 *
 * Revision 1.4  2008/09/25 14:31:43  mtk01959
 * *** empty log message ***
 *
 * Revision 1.3  2008/09/23 08:14:21  mtk01662
 * *: unmark the vcodec type define for LNX_KERNEL only
 *
 * Revision 1.2  2008/09/15 23:40:22  mtk01662
 * *: modify the vdsp to meet the vcodec interface
 *
 * Revision 1.1.1.1  2008/08/18 06:49:24  mtk02142
 *
 *
 */

