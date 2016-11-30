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
 *	File:	eva_constant.h
 *	 
 *	Author: Serena Huang, Quark Li
 *	 
 *	Last Revision Date: 2011.9.29
 *	 
 *	Description:
 *	Enhance VoIP Architecture (EVA) generic Constant and Enumeration
 *	difinitions. 
 *
 */
 
#ifndef _EVA_CONST_H_
#define _EVA_CONST_H_

/* Primitive Types */
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


#ifndef NULL                                
#define NULL (void*)(0)
#endif

#ifdef IPV4
#undef IPV4
#endif

#ifdef IPV6
#undef IPV6
#endif


#define MAX8				(0xff)
#define MAX16				(0xffff)
#define MAX32				(0xffffffff)
#define MAX_CID_CHAR_LEN	(32)
#if defined(FIVE_CADENCE_SUPPORT) || defined(CJ_DSP_SUPPORT) 
#define MAX_CADENCE		(5)
#else
#define MAX_CADENCE		(3)    
#endif
#define MAX_TONE_FREQ		(4)	
#define DUAL_TONE_FREQ		(2)
#define SIT_TONE_FREQ		(5)
#define MAX_PACKET_SZ		(1024)
#define MAX_GAIN_AMP		(24)
#define MIN_GAIN_AMP		(-24)
#define JB_SZ_MAX			(800)
#define JB_SZ_MIN			(0)
#define DTMF_DURATION_MIN  (5)
#define DTMF_DURATION_MAX (16)
#define DTMF_POWER_MIN      (-45)
#define DTMF_POWER_MAX     (-25)
#define DTMF_pDTMFPWR_DEF   (-45)/* Default -45 dBm per frequency */
#define DTMF_tDTMFDUR_DEF   (7) /* Default 7 blocks (35 ms) minimum on dur */

#define TEV_PIGGYBACK_ENABLE 1
#define TEV_TONE_ENABLE		 1<<1
#define TEV_CAS_TONE_ENABLE	 1<<2
#define RING_REP_DEF	  (60)

/* Generic Conditions */
typedef enum {
	EVA_FALSE,
	EVA_TRUE
} evaBool_e;

typedef enum {
	CONTROL_DISABLE,	
	CONTROL_ENABLE
} enableControl_e;

typedef enum {
	VBDFAX,	
	VBDMODEM
} vbdfaxmodemflag;

/* For redundant mode. */
typedef enum {
	EVA_REDUNDANT_AUTOBYRTCP=0,
	EVA_REDUNDANT_FIX1=1,
	EVA_REDUNDANT_FIX2=2,
	EVA_REDUNDANT_FIX3=3,
	EVA_REDUNDANT_DISABLE=4,
} redMode_e;

typedef enum{
	EC_TAIL_LENGTH_DEFAULT=0,
	EC_TAIL_LENGTH_36MS=1,
	EC_TAIL_LENGTH_48MS=2,
	EC_TAIL_LENGTH_60MS=3,
	EC_TAIL_LENGTH_72MS=4
}lecTailMode_e;

/* DSP Identification */
typedef enum {
	DSP_VIKING,
	DSP_MTK
} dspId_e;

/* Channel ID Enumeration */
typedef enum {
	CH0,
	CH1,
	CH2,
	CH3,
	CH4,
	CH5,
	CH6,
	CH7
} chanId_e;

/* Stream ID Enumeration */
typedef enum {
	STRM0,
	STRM1,
	STRM2,
	STRM3
} strmId_e;

/* Interface ID Enumeration */
typedef enum {
	INFC0,
	INFC1,
	INFC2,
	INFC3,
	INFC4,
	INFC5,
	INFC6,
	INFC7
} infcId_e;


/* Execution Return Code */
typedef enum { 
	EXEC_OK=0,
	EXEC_SUCCESS = 1,
	EXEC_FAIL = -1,
	INVALID_PARAM = -2,
	FUNC_UNSUPPORT = -3,
	DEVICE_BUSY = -4,
	UNKNOWN_ERROR = -5
} exCode_e;

typedef enum {
	BLOCKING,
	NON_BLOCKING
} blockMode_e;

/* Generic Active State */
typedef enum {
	STATE_INACTIVE,
	STATE_ACTIVE
} activeState_e;

/* Serial Number of Cadence */
typedef enum {
	CADENCE_ONE,
	CADENCE_TWO,
	CADENCE_THREE,
	CADENCE_FOUR,
	CADENCE_FIVE,
	CADENCE_MAX	
} SerialNumOfCad_e;

/* Serial Number of Frequency */
typedef enum {
	FREQUENCY_ONE,
	FREQUENCY_TWO,
	FREQUENCY_THREE,
	FREQUENCY_FOUR,
	FREQUENCY_MAX	
} SerialNumOfFreq_e;



/* Telephony Device Interface Type */
typedef enum {
	INFC_FXS,
	INFC_FXO,
	INFC_AUDIO,
	INFC_OTHER
} infcType_e;

/*Support ring parameters set. */
typedef enum {
	RING_SINE,
	RING_TRAP
} infcRingType_e;

#define INFCTYPE_NUM	(INFC_OTHER + 1)

/* Polarity Direction */
typedef enum {
	POL_FWD,
	POL_REV
} polDir_e;

/* Line State */
typedef enum {
	LINE_DOWN,
	LINE_ACTIVE_FWD,
	LINE_ACTIVE_REV,
	LINE_RING, 
	LINE_RING_PAUSE,
	LINE_BUSY, 
	LINE_SLEEP, 
	LINE_ERROR,
	LINE_LIU,
	LINE_LNIU,
	LINE_OHT_REV 
} lineState_e;

/* Hook State */
typedef enum {
	HOOK_FLASH,
	HOOK_RELEASE,
	HOOK_SEIZE,
	HOOK_PULSE1,
	HOOK_PULSE2,	
	HOOK_PULSE3,	
	HOOK_PULSE4,	
	HOOK_PULSE5,	
	HOOK_PULSE6,	
	HOOK_PULSE7,	
	HOOK_PULSE8,	
	HOOK_PULSE9,	
	HOOK_PULSE10,	
	HOOK_PULSE11,	
	HOOK_PULSE12,	
	HOOK_PULSE13,	
	HOOK_PULSE14,	
	HOOK_PULSE15,	
	HOOK_PULSE16,	
	HOOK_PULSE17,	
	HOOK_PULSE18,	
	HOOK_PULSE19,	
	HOOK_PULSE20,
	HOOK_ERROR
} hookState_e;

/* CODEC Enumerations */
typedef enum { 
	CODEC_G711A,
	CODEC_G711U,
	CODEC_G722,
	CODEC_G723,
	CODEC_G726_32,
	CODEC_G726_24,
	CODEC_G729,
	CODEC_AMR, 
	CODEC_SILCOMP,
	CODEC_DTMFR,
	CODEC_DTMFR_DL,
	CODEC_T38,
	CODEC_REDUNDANT, 
	CODEC_PCMA_VBD, /*for V152*/
	CODEC_PCMU_VBD, /*for V152*/
	CODEC_INVALID/*invalid can not use, please add codec before invalid*/
} codec_e;

#define MAX_CODEC_NUM         (CODEC_INVALID) 

/*codec bit mask*/
#define MASK_CODEC_G711A	(1 << CODEC_G711A)
#define MASK_CODEC_G711U	(1 << CODEC_G711U)
#define MASK_CODEC_G723	(1 << CODEC_G723)
#define MASK_CODEC_G722	(1 << CODEC_G722)
#define MASK_CODEC_G726_32	(1 << CODEC_G726_32)
#define MASK_CODEC_G726_24	(1 << CODEC_G726_24)
#define MASK_CODEC_G729	(1 << CODEC_G729)
#define MASK_CODEC_AMR	(1 << CODEC_AMR) 
#define MASK_CODEC_SILCOMP (1 << CODEC_SILCOMP)
#define MASK_CODEC_DTMFR (1 << CODEC_DTMFR)
#define MASK_CODEC_DTMFR_DL (1 << CODEC_DTMFR_DL)
#define MASK_CODEC_T38		(1 << CODEC_T38)

/* Bit rate*/
typedef enum { 
    EVA_BR_NONE,
	EVA_G7231_5300,
	EVA_G7231_6300,
	EVA_ILBC_20,
	EVA_ILBC_30,	
	EVA_AMR_475,
	EVA_AMR_515,			  
	EVA_AMR_59,
	EVA_AMR_67,
	EVA_AMR_74,
	EVA_AMR_795,
	EVA_AMR_102,
	EVA_AMR_122,			  
	EVA_AMR_SID, 
	EVA_AMR_NONE,
} evaBitrate_e;



/*EC tail length*/
typedef enum { 
	TAIL_16MS,
	TAIL_32MS,
	TAIL_48MS,
	TAIL_64MS,
	TAIL_128MS
} ecTail_e;

/*Event Type*/
typedef enum {
	EDGE_ONCE,
	EDGE_BEGIN,
	EDGE_END
} eventEdge_e;

/*T38 Code*/
typedef enum {
	T38_DISCONN,
	T38_PTCHANGE
} t38State_e;

/*Event Code*/
typedef enum {
	EVENT_CODE_INVALID,
	EVENT_CODE_TONE,
	EVENT_CODE_CID,
	EVENT_CODE_LINE,
	EVENT_CODE_HOOK,
	EVENT_CODE_T38,
	EVENT_CODE_JB_UPDATE,
	EVENT_CODE_NON_RTP_RECVD, 
	EVENT_CODE_RTCP_SEND, /*Not implement yet*/
	EVENT_CODE_RTCP_RECVD, /*Not implement yet*/
	EVENT_CODE_STREAM_UPDATE, /*Not implement yet*/
	EVENT_CODE_TIMER, /*Not implement yet*/
	EVENT_CODE_PERFORMANCE, /*Not implement yet*/
	EVENT_CODE_ERROR, /*Not implement yet*/
	EVENT_CODE_DL_DTMF, /*DL DTMF TONE*/
	EVENT_CODE_JB_CHANGE,/*JITTER CHANGE*/
	
	EVENT_CODE_TONE_END, /* Tone end */
	EVENT_CODE_CALLERID_SEND, /* CallerID send */	
	EVENT_CODE_FIRST_RTP_DELIVERD, /* first rtp packet income*/
	EVENT_CODE_PCM,
} eventCode_e;

/*channel Detectors*/
#define DETECT_TONE_DTMF				(1 << 0)
#define DETECT_TONE_MODEM				(1 << 1)
#define DETECT_TONE_CPT					(1 << 2)	/* FXO only */
#define DETECT_CID    					(1 << 3)  /* FXO only */
#define DETECT_DTMFR					(1 << 4)
#define DETECT_PULSE					(1 << 5)
#define DETECT_JB_EVENT					(1 << 6)



/*Ptime*/
typedef enum { 
	PTIME_10MS,
	PTIME_20MS,
	PTIME_30MS,
	PTIME_40MS,
	PTIME_50MS,
	PTIME_60MS
} pTime_e;



/* faxRate */
typedef enum { 
	FAX_RATE_2400_BPS,
	FAX_RATE_4800_BPS,
	FAX_RATE_7200_BPS,
	FAX_RATE_9600_BPS,
	FAX_RATE_12000_BPS,
	FAX_RATE_14400_BPS,
	FAX_RATE_16800_BPS,
	FAX_RATE_19200_BPS,
	FAX_RATE_21600_BPS,
	FAX_RATE_24000_BPS,
	FAX_RATE_26400_BPS,
	FAX_RATE_28800_BPS,
	FAX_RATE_31200_BPS,
	FAX_RATE_33600_BPS,
	FAX_RATE_BAD,
} faxRate_e;


typedef enum {
	TONE_DTMF_1 = 1,
	TONE_DTMF_2,
	TONE_DTMF_3,
	TONE_DTMF_4,
	TONE_DTMF_5,
	TONE_DTMF_6,
	TONE_DTMF_7,
	TONE_DTMF_8,
	TONE_DTMF_9,
	TONE_DTMF_0,
	TONE_DTMF_STAR,
	TONE_DTMF_POUND,
	TONE_DTMF_A,
	TONE_DTMF_B,
	TONE_DTMF_C,
	TONE_DTMF_D,
	TONE_DIAL,
	TONE_RINGBACK,	
	TONE_BUSY,
	TONE_REORDER,
	TONE_SIT,
	TONE_CUSTOM_1,
	TONE_CUSTOM_2,
	TONE_CUSTOM_3,
	TONE_CUSTOM_4,
	TONE_CNG,
	TONE_CED,
	TONE_ANS,
	TONE_ANSAM,
	TONE_V21PREAMBLE,
	TONE_FAX_CM,
	TONE_FAX_TIMEOUT,
	TONE_VBD_FAX_END, 
	TONE_ANSBAR,
	TONE_INVALID
} toneCode_e;

#define NUM_OF_TONE (TONE_INVALID)

typedef enum {
	DTMF_0,
	DTMF_1,
	DTMF_2,
	DTMF_3,
	DTMF_4,
	DTMF_5,
	DTMF_6,
	DTMF_7,
	DTMF_8,
	DTMF_9,
	DTMF_STAR,
	DTMF_POUND,
	DTMF_A,
	DTMF_B,
	DTMF_C,
	DTMF_D,
	RFC2833_DIAL_TONE = 255,
	RFC2833_BUSY_TONE,
	RFC2833_CONGESTION_TONE,
	RFC2833_RINGBACK_TONE,
	RFC2833_REORDER_TONE,
	RFC2833_CALLWAITING_TONE,
	RFC2833_IDENTIFICAION_TONE,
	RFC2833_NEGATIVE_IND_TONE,
	RFC2833_POSITIVE_IND_TONE,
	RFC2833_STUTTER_DIAL_TONE,
	RFC2833_HORNING_TONE,
	RFC2833_SECOND_DIAL_TONE,
	RFC2833_CAS_TONE,
} dtmf_e;

typedef enum {
	STRM_INACTIVE,
	STRM_SENDONLY,
	STRM_RECVONLY,
	STRM_SENDRECV
} strmDir_e;

typedef enum {
	IPV4,
	IPV6
} ipVer_e;


typedef enum {
	TONE_REGULAR,
	TONE_MODULATE
} toneType_e;

/*Jitter buffer mode*/
typedef enum {
	JB_ADAPT,
	JB_FIXED,
	JB_DISABLE
} jbMode_e;

typedef enum {
    CID_FORMAT_BELLCORE_FSK,
    CID_FORMAT_ETSI_FSK,
	CID_FORMAT_NTT,
	CID_FORMAT_ETSI_RPAS,
	CID_FORMAT_ETSI_DTAS,
	CID_FORMAT_DTMF
} cidFormat_e;

/*Support line test. */
typedef enum {
	TYPE_ZARLINK = 1,
	TYPE_SILICON =2,
} slicType_e;


typedef enum {
    MTK_EVA_MWI_VALUE_NONE,
    MTK_EVA_MWI_VALUE_SET,
    MTK_EVA_MWI_VALUE_CLEAR,
    MTK_EVA_MWI_VALUE_MAX
} MWIValue_e;




/*Tone event direction*/
typedef enum{
	TONE_DIR_LOCAL,
	TONE_DIR_REMOTE
} toneDir_e;

/* EVA Log Level */
typedef enum {
	EVA_LOG_NONE = 0,	/* Log level NONE */
	EVA_LOG_ERR = 1,	/* Log level ERROR, for the error that effect the main function. */
	EVA_LOG_WARN = 2,	/* Log level WARNING, for warning of the innormal cases.  */
	EVA_LOG_INFO = 3,	/* Log level INFO, for information of function entry and exit. */
	EVA_LOG_DBG = 4,	/* Log level DEBUG, for other log. */
	EVA_LOG_MAX = 5,	/* Log level MAX */
} evaLogLevel_e;

/* EVA Log Modules */
typedef enum{
	EVA_LOG_MODULE_ADAM=0,
	EVA_LOG_MODULE_GDI,
	EVA_LOG_MODULE_FXS,
	EVA_LOG_MODULE_VDSP,
	EVA_LOG_MODULE_ORTP,
	EVA_LOG_MODULE_LEC,
	EVA_LOG_MODULE_SLIC,
	EVA_LOG_MODULE_PCM,
	EVA_LOG_MODULE_MAX
}evaLogModuleIndex;


/* EVA Log Modules Mask */
#define EVA_MODULE_MASK_ADAM	(0x1 << EVA_LOG_MODULE_ADAM)
#define EVA_MODULE_MASK_GDI 	(0x1 << EVA_LOG_MODULE_GDI)


typedef enum
{
    DEV_15,
    DEV_16,
    DEV_17,
    DEV_18,
    DEV_19,
    DEV_20,
    DEV_21,
    DEV_22,
    DEV_23,
    DEV_24,
    DEV_25,
    DEV_26,
    DEV_27,
    DEV_28,
    DEV_29,
    DEV_30,
    DEV_31,
    DEV_32,
    DEV_33,
    DEV_34,
    DEV_35,
    DEV_36,
    DEV_37,
    DEV_38,
    DEV_39,
    DEV_40,
    DEV_41,
    DEV_42,
    DEV_43,
    DEV_44,
    DEV_45
} Dtmf_Deviation;

typedef enum{
	C_DEF = 0,  /*default*/
	C_AU,		/*Australia*/
	C_AT,		/*Austria*/
	C_BE,       /*Belgium*/
	C_BR,		/*Brazil*/
	C_CA,		/*Canada*/
	C_CN,		/*China*/
	C_CZ,		/*Czech*/
	C_DK,		/*Denmark*/
	C_EU,		/*ETSI*/
	C_FI,		/*FinLand*/
	C_FR,       /*France*/
	C_DE,		/*German*/
	C_GR,		/*Greece*/
	C_HK,		/*HongKong*/
	C_HU,		/*Hungary*/
	C_IE,       /*Ireland*/
	C_IL,       /*Israel*/
	C_IN,		/*India*/
	C_IT,		/*Italy*/
	C_JP,		/*Japan*/
	C_SG,		/*Singapore*/
	C_KR,		/*S.Korea*/
	C_MX,       /*Mexico*/
	C_NL,		/*Netherlands*/
	C_NZ,		/*NewZealand*/
	C_NO,		/*Norway*/
	C_PL,		/*Poland*/
	C_PT,		/*Portugal*/
	C_SI,		/*Slovenia*/
	C_TW,       /*Taiwan*/
	C_ES,		/*Spain*/
	C_SE,		/*Sweden*/
	C_GB,		/*U.K.*/
	C_US,       /*U.S.*/
	C_ZA,       /*S.Africa*/
	C_VN,      /*Vietnam*/
	C_RU,      /*Russia*/
	C_RSV1,     /*Reserved Country #1*/
	C_RSV2,     /*Reserved Country #2*/
	C_RSV3,     /*Reserved Country #3*/
	C_NUM
}hwProf_e;


#define CodecIsNotChangeOrBothCodecAndUlCodecIsChange(preStrmConf,curStrmConf)\
((preStrmConf->strmAttr.payloadSelect == curStrmConf->strmAttr.payloadSelect)\
 ||((preStrmConf->strmAttr.payloadSelect != curStrmConf->strmAttr.payloadSelect)&&(preStrmConf->strmAttr.ulPayloadSelect != curStrmConf->strmAttr.ulPayloadSelect)))



/*UL codec identify*/
#define UL_CODEC_IDENTIFY	(1<<7) 
#endif /* End of _EVA_CONST_H_ */


