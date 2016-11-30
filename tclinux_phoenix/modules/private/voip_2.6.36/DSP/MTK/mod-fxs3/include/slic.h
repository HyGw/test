/**
 *	@author Theresa Yang, Howard Chen
 *
 *	@note 2009/02/23		reviewed by Theresa	
 *	@note 2009/03/16		refined by Chi Yen Yu
 *    @note 2009/03/16		refined by Chi Yen Yu
 *
 */
#include "cp_det_exp.h"

#ifndef SLIC_1126_DEF
#define SLIC_1126_DEF

#define FXS_FMTD_FC_BIT          (0)
#define FXS_FMTD_MC_BIT          (1)
#define FXS_FMTD_ANSWER_BIT      (2)
#define FXS_FMTD_PHASE_BIT       (3)
#define FXS_FMTD_V21_BIT         (4)
#define FXS_FMTD_BA1_BIT         (5)
#define FXS_FMTD_BA2_BIT         (6)

#define FXS_FMTD_SIL             (0)
#define FXS_FMTD_FC_WORD         (1 << FXS_FMTD_FC_BIT)
#define FXS_FMTD_MC_WORD         (1 << FXS_FMTD_MC_BIT)       
#define FXS_FMTD_ANSWER_WORD     (1 << FXS_FMTD_ANSWER_BIT)     
#define FXS_FMTD_PHASE_WORD      (1 << FXS_FMTD_PHASE_BIT)      
#define FXS_FMTD_V21_WORD        (1 << FXS_FMTD_V21_BIT)
#define FXS_FMTD_BA1_WORD        (1 << FXS_FMTD_BA1_BIT)
#define FXS_FMTD_BA2_WORD        (1 << FXS_FMTD_BA2_BIT)

#define CID_MAX_STRLEN			(256)											/* Maximum callerID string length */
#define CID_MAX_FRAMELEN		(CID_MAX_STRLEN + 2/* Maximum checksum size */)	/* Maximum frame length */
#if defined(FIVE_CADENCE_SUPPORT) || defined(CJ_DSP_SUPPORT) 
#define MAX_CADENCE				(5)
#else
#define MAX_CADENCE             (3)
#endif

typedef enum {
	CADENCE_ONE_E,
	CADENCE_TWO_E,
	CADENCE_THREE_E,
	CADENCE_FOUR_E,
	CADENCE_FIVE_E,
	CADENCE_MAX_E	
} SerialNumOfCadence_e;


#if defined(C7_DSP_SUPPORT)||defined(CZ_DSP_SUPPORT) || defined(FAX_REMOTE_DET)
#define FXS_TONE_DIR			 (1 << 12)	/* 0 is local, 1 is remote */
#endif

/*add for play tone to peer stream by weifeng*/
#define TONE_UL 0 
#define TONE_DL 1
/*end  of add for play tone to peer stream by weifeng*/
/*add for detect dtmf from network by james*/
#define DTMF_UL_START	0 
#define DTMF_UL_END	1 
#define DTMF_DL_START	2
#define DTMF_DL_END	3
/*end of for detect dtmf from network by james*/


typedef enum e_fxs_event
{
	/* SLIC HW event : Should sync with E_PHONE_EVENT in SLIC driver and mq_eventId_t in ddr_mq.h of mod-misc */
	FXS_EVENT_SLIC_MIN			= 0,
	/* ----------------------------------- */
	FXS_EVENT_ONHOOK				= 1,
	FXS_EVENT_OFFHOOK			= 2,
	FXS_EVENT_DIGIT				= 3,
	FXS_EVENT_RING_TA				= 4,
	FXS_EVENT_RING_TI				= 5,
	FXS_EVENT_FSKBUF_AVAIL		= 6,
	FXS_EVENT_OSC_TA				= 7,
	/* ----------------------------------- */
	FXS_EVENT_SLIC_MAX,

	/* Pseudo event generated in FXS module : Start from 16 */
	FXS_EVENT_FXS_MIN				= 16,
	/* ----------------------------------- */
	FXS_EVENT_PULSE,				
	FXS_EVENT_FLASHHOOK,
	/* ----------------------------------- */
	/* --- Fax tone detection --- */
	FXS_EVENT_FAX_TONE_DETECTED,
	FXS_EVENT_MODEM_TONE_DETECTED,
	FXS_EVENT_DISC_FAX,
	FXS_EVENT_END_FAX,
	FXS_EVENT_FAX_VOICE2VBD,
	FXS_EVENT_FAX_VBD2VOICE,
	/* ----------------------------------- */
	FXS_EVENT_FXS_MAX,
	
	FXO_EVENT_RING_ACTIVE,
	FXO_EVENT_RING_INACTIVE,
	FXO_EVENT_LINE_DOWN,
	FXO_EVENT_LINE_FWD,
	FXO_EVENT_LINE_REV,
	FXS_EVENT_CPT_DETECTED,
	FXS_EVENT_CID_DETECTED,
	/*add by mtk69097*/
	FXO_EVENT_LINE_LIU,
	FXO_EVENT_LINE_LNIU,
	/*end of add by mtk69097*/
	FXS_EVENT_JITTER_CHANGE,
	/* by mtk07059_20140521 ,for tone_stop_event */
	FXS_EVENT_TONE_END,	
	FXS_EVENT_CALLERID_SEND,
	FXS_REPLACE_DATA_START,
	FXS_REPLACE_DATA_END,
	/* ------------------------------------------------------- */
	FXS_EVENT_LAST
} E_FXS_EVENT;


/* by mtk07059_20140521 ,for tone_stop_event */
typedef enum tone_control_e
{
	TONE_CTRL_DISABLE = 0	,	/* Stop tone */
	TONE_CTRL_ENABLE = 1,	/* Generate tone */
	TONE_CTRL_END = 2	/* Trigger end event  */
};



typedef struct slic_listener 
{
	void (*event_on)(void* o, unsigned cid, unsigned e, unsigned e_data);
	void* priv;
}slic_listener_t;

typedef enum
{
   SLIC_COUNTRY_USA = 0,
   SLIC_COUNTRY_MY = 1,
   LASTCOUNTRY
}slic_country_t;

enum cid_property_e
{
	CID_GEN_OFFHOOK		= (1 << 1),	/* Generate CID during off-hook status (type II) */
	CID_GEN_NOW 			=(1 << 2),	/* Generate CID NOW */
	CID_GEN_OFFHOOK_NOACK		=(1 << 3)	/* Generate CID type II without ack */
};

/* 
** Hook states 
*/
enum {
SLIC_OFFHOOK,
SLIC_ONHOOK
};

enum fax_detection_e
{
	FAX_DET_DIS,
	FAX_DET_EN,
	FAX_DET_REFRESH,
};
#ifndef FXS_MAX_TONE_NUM
#define FXS_MAX_TONE_NUM	40//changed from 24 to 40
#endif
#define FXS_MAX_TONESEQ_NUM 12

#if FXS_MAX_TONE_NUM < 16
#error: maximum tone number defie too short
#endif
typedef enum
{
	SLIC_DIAL_TONE,
	SLIC_BUSY_TONE,
	SLIC_CONGESTION_TONE,
	SLIC_RINGBACK_TONE,
	SLIC_REORDER_TONE,
	SLIC_CALLWAITING_TONE,
	SLIC_IDENTIFICAION_TONE,
	SLIC_NEGATIVE_IND_TONE,
	SLIC_POSITIVE_IND_TONE,
	SLIC_STUTTER_DIAL_TONE,
	SLIC_HORNING_TONE,
	SLIC_MWI_TONE,
	SLIC_STUTTER_TONE,
	SLIC_1KHZ_TONE,  /* mtk07059_20150415, add for Fiberhome. */
	SLIC_CALLWAITING_RINGBACK_TONE,/*mtk07231_callwaiting_ringback_tone_20151014*/
	/* ------------------------------------ */
	SLIC_USER_TONE0,//user define tone
	SLIC_USER_TONE1,//user define tone
	SLIC_USER_TONE2,//user define tone
	SLIC_USER_TONE3,//user define tone
	SLIC_USER_TONE4,//user define tone
	/*-------------------------------------*/
	SLIC_DTMF_TONE_0=34,
	SLIC_MAX_TONE=FXS_MAX_TONE_NUM,	/* NOT TO USE : Maximum count of tone */
} slic_tone_t;

/**
 * @brief   Defines structure for configuring 1 oscillator
 */
typedef struct osc_s
{
	unsigned int    freq;
	unsigned int    amp;
	unsigned int    phas;
	unsigned int   	activePeriod;
	unsigned int   	inactivePeriod;
	/* Padding to keep size being 4-byte aligned */
} osc_t;

/**
 * @brief   Defines structure for tone sequence, by mtk07059_20150505, bug fix for tone discontinuos.
 */
typedef struct tone_seq_s
{
	unsigned int    numOfTone;
	uint8     toneSeq[12];
	int				dur;
	int				repeat;
} tone_seq_t;

/**
 * @brief   Defines structure for configuring tone generator
 */
#ifdef OSBN_VOIP
typedef struct tone_gen_s
{
	osc_t osc1;
	osc_t osc2;
} tone_gen_t;
#else
typedef struct tone_s
{
	osc_t osc1;
	osc_t osc2;
} tone_t;
#endif 

/* Country tone setting layout:
  SLIC_DIAL_TONE,
  SLIC_BUSY_TONE,
  SLIC_CONGESTION_TONE,	
  SLIC_RINGBACK_TONE,
  SLIC_REORDER_TONE,
  SLIC_CALLWAITING_TONE,
  SLIC_IDENTIFICAION_TONE,
  SLIC_NEGATIVE_IND_TONE,
  SLIC_POSITIVE_IND_TONE
  */	
typedef struct country_tone_s{
#ifdef OSBN_VOIP
	tone_gen_t tone[SLIC_MAX_TONE];
#else
	tone_t tone[SLIC_MAX_TONE];
#endif
}country_tone_t;

//Frequency and Power of tone
typedef struct Tone_Attribute
{
    unsigned int  tone1;    /* Frequency of tone1 to generate */
    unsigned int  t1Pw;     /* Power in 0.5db units for tone1 */
    unsigned int  tone2;    /* Frequency of tone2 to generate */
    unsigned int  t2Pw;     /* Power in 0.5db units for tone2 */
    unsigned int  tone3;    /* Frequency of tone3 to generate */
    unsigned int  t3Pw;     /* Power in 0.5db units for tone3 */
    unsigned int  tone4;    /* Frequency of tone4 to generate */
    unsigned int  t4Pw;     /* Power in 0.5db units for tone4 */
    unsigned int  ctrlWord; /* Control Word */
}Tone_Attribute_t;

/*--------------------SW tone structure-----------------*/
typedef struct {
    
    #ifdef JUST_SUPPORT_FXSANDGDI  //default open in fxs and gdi layer(gdi and fxs),,but close in sip app 
    Tone_Attribute_t tone_attribute[MAX_CADENCE]; 
    #else
    unsigned int  tone1;    /* Frequency of tone1 to generate */
    unsigned int  t1Pw;     /* Power in 0.5db units for tone1 */
    unsigned int  tone2;    /* Frequency of tone2 to generate */
    unsigned int  t2Pw;     /* Power in 0.5db units for tone2 */
    unsigned int  tone3;    /* Frequency of tone3 to generate */
    unsigned int  t3Pw;     /* Power in 0.5db units for tone3 */
    unsigned int  tone4;    /* Frequency of tone4 to generate */
    unsigned int  t4Pw;     /* Power in 0.5db units for tone4 */
    unsigned int  ctrlWord; /* Control Word */
    #endif
    unsigned int     numCads;  /* Number of on/off pairs */
    unsigned int     make1;    /* 1st on  time in 1 ms units */
    unsigned int     break1;   /* 1st off time in 1 ms units */
    unsigned int     repeat1;  /* 1st cadence cycles */ 
    unsigned int     make2;    /* 2nd on  time in 1 ms units */
    unsigned int     break2;   /* 2nd off time in 1 ms units */
    unsigned int     repeat2;  /* 2nd cadence cycles */ 
    unsigned int     make3;    /* 3rd on  time in 1 ms units */
    unsigned int     break3;   /* 3rd off time in 1 ms units */
    unsigned int     repeat3;  /* 3rd cadence cycles */ 
    #if defined(FIVE_CADENCE_SUPPORT) || defined(CJ_DSP_SUPPORT) 
    unsigned int     make4;    /* 4rd on  time in 1 ms units */
    unsigned int     break4;   /* 4rd off time in 1 ms units */
    unsigned int     repeat4;  /* 4rd cadence cycles */
    unsigned int     make5;    /* 5rd on  time in 1 ms units */
    unsigned int     break5;   /* 5rd off time in 1 ms units */
    unsigned int     repeat5;  /* 5rd cadence cycles */
    #endif
    unsigned int  repeat;   /* Number of cycles of tones */
} sw_tone_t;

#define SLIC_TONE_INF   (-1)
#ifdef QUADTONE
#define SLIC_TONE_QUAD	(0x0004)
#endif
#define SLIC_TONE_DUAL	(0x0002)
#define SLIC_TONE_MONO	(0x0001)

/* 
  Country tone setting layout:
  SLIC_DIAL_TONE,
  SLIC_BUSY_TONE,
  SLIC_CONGESTION_TONE,
  SLIC_RINGBACK_TONE,
  SLIC_REORDER_TONE,
  SLIC_CALLWAITING_TONE,
  SLIC_IDENTIFICAION_TONE,
  SLIC_NEGATIVE_IND_TONE,
  SLIC_POSITIVE_IND_TONE
*/	
typedef struct sw_country_tone_s{
	sw_tone_t tone[SLIC_MAX_TONE];
}sw_country_tone_t;

typedef struct {
	uint32	genSeize;
	uint32	genRelease;
	uint32	detSeizeMin;
	uint32	detSeizeMax;
	uint32	detReleaseMin;
	uint32	detReleaseMax;
} slic_pulseTime_t;

typedef struct {
    uint32	flashMin;
    uint32	flashMax;
    uint32	releaseMin;
	uint32	autoFlashDur;
	slic_pulseTime_t pulseTime;
	uint8 pulseEnable;
	uint32 seizeMin;
} slic_hookThreshold_t;


typedef struct ring_cadence{
	int active_ms;
	int inactive_ms;
}ring_cadence_t;

typedef enum
{
    DTMF_DEV_15,
    DTMF_DEV_16,
    DTMF_DEV_17,
    DTMF_DEV_18,
    DTMF_DEV_19,
    DTMF_DEV_20,
    DTMF_DEV_21,
    DTMF_DEV_22,
    DTMF_DEV_23,
    DTMF_DEV_24,
    DTMF_DEV_25,
    DTMF_DEV_26,
    DTMF_DEV_27,
    DTMF_DEV_28,
    DTMF_DEV_29,
    DTMF_DEV_30,
    DTMF_DEV_31,
    DTMF_DEV_32,
    DTMF_DEV_33,
    DTMF_DEV_34,
    DTMF_DEV_35,
    DTMF_DEV_36,
    DTMF_DEV_37,
    DTMF_DEV_38,
    DTMF_DEV_39,
    DTMF_DEV_40,
    DTMF_DEV_41,
    DTMF_DEV_42,
    DTMF_DEV_43,
    DTMF_DEV_44,
    DTMF_DEV_45
} DTMF_Deviation;

typedef struct dtmfdet_params{
	int32 power;
	uint32 duration;
	DTMF_Deviation devAcc;
	DTMF_Deviation devRej;
}dtmfdet_params_t;

typedef enum
{
    SLIC_IMPC_600ohms,      /*0*/
    SLIC_IMPC_900ohms,      /*1*/
    SLIC_IMPC_Japan,        /*2. (600ohms + 1uF)*/
    SLIC_IMPC_900ohms_216uF,/*3. (900ohms + 2.16uF) */
    SLIC_IMPC_CTR21,        /*4. 270ohms+(750ohms||150nF)*/
    SLIC_IMPC_Australia,    /*5. also _NewZealand, 220ohms+(820ohms||120nF)*/
    SLIC_IMPC_Slovakia,     /*6. also Slovenia/South Africa, 220ohms+(820ohms||115nF)*/
    SLIC_IMPC_China,        /*7. 200ohms+(680ohms||100nF)*/
    SLIC_IMPC_MAX
}slic_impdnc_t;

typedef enum
{
	FXS_RING_PROFILE_DEFAULT=0,
	FXS_RING_PROFILE_AUSTRALIA,
	FXS_MAX_RING_PROFILE,						/* NOT TO USE : Maximum count of profile*/
} fxs_ring_profile_t;

/*	Define currently support SPEC of Caller-ID : CID sub-module would generate
	corresponding header and checksum and send it to PHONE. For NTT spec,
	parity bit would also be handled in CID sub-module.						*/
enum cid_spec_e
{
	CID_NONE,
	CID_MIN,
	/* ----------------------------------- */
	CID_BELLCORE,		// USA 
	CID_NTT,			// JAPAN 
	CID_ETSI,			// EUROPE 
	CID_CHT,			// TAIWAN 
	CID_BT,				// BRITISH 
	CID_ETSI_DTAS,      //EUROPE DTAS
	CID_ETSI_RPAS,      //RUROPE RPAS
	/* ----------------------------------- */
	CID_MAX,
};

typedef struct cid_t2_timing_params
{
	uint32 T9; /*unit:5ms*/
	uint32 T10;
	uint32 T11;
	uint32 T12;
	uint32 T13;
	uint32 T14;
}cid_t2_timing_params_t;

typedef struct cid_t1_timing_params
{
	uint32 T2; 
	uint32 T3;
	uint32 T4;
}cid_t1_timing_params_t;


/*Software LEC config*/
#if 0  //jf_voip_130524
typedef	enum s_slic_sw_lec_mode
{
	SLIC_LEC_DISABLE = 0x1,
	SLIC_LEC_16MS = 0x2,
	SLIC_LEC_32MS = 0x3, 
	SLIC_LEC_48MS = 0x4,
} slic_sw_LEC_MODE;
#else
typedef	enum s_slic_sw_lec_mode
{
	SLIC_LEC_DISABLE = 0x0,
	SLIC_LEC_36MS = 0x1,
	SLIC_LEC_48MS = 0x2, 
	SLIC_LEC_60MS = 0x3,
	SLIC_LEC_72MS = 0x4,
} slic_sw_LEC_MODE;

#endif

#ifdef OSBN_VOIP
typedef	enum
{
	CTRL_INFC_ONHOOK = 50,
	CTRL_INFC_OFFHOOK = 51,
} hook_ctrl_e;
#endif

typedef struct fsk_det_params_s{
    int space_freq;
    int mark_freq;
}fsk_det_params_t;

#define MAX_CID_CHAR_LEN	(32) /* This value is the same as eva_constant.h*/
typedef struct cid_data_s{
	char number[MAX_CID_CHAR_LEN];
	char name[MAX_CID_CHAR_LEN];
	char dateTime[MAX_CID_CHAR_LEN];
}cid_data_t;
#if defined(VOICE_QUALITY_IMPROVE)
typedef struct jitBuf_data_s{
	int sid;
	int changeFlag;
}jitBuf_data_t;
#endif

typedef struct s_fxs_event
{
	union
	{
		struct s_fxs_event_common
		{
			int data;
		} common;

		struct s_fxs_event_tone
		{
			int toneId;
			int end;
			/* added by mtk07059_20140605, for jiffies,volume report. */
			#if 1 //def CZ_DSP_SUPPORT
			unsigned long jiff;
			int energy;
			#endif
		} tone;

		cid_data_t cid;
#if defined(VOICE_QUALITY_IMPROVE)
		jitBuf_data_t jitBuf;
		#endif

	} event;
} T_SLIC_EVENT_DATA;

#define SLIC_EVENT_DATA_SIZE (sizeof(T_SLIC_EVENT_DATA))

typedef enum
{
	FSK_DETECT_DISABLE,
	FSK_DETECT_ON_HOOK,
	FSK_DETECT_OFF_HOOK
}fsk_detect_mode;

/*[OSBNB00045371] ADD by mtk07059_20150423 , support ring parameters set. */
typedef struct {
	int32 type;
	int32 frequency;
	int32 amplitude;
	int32 dcBias;
	int32 ringTripThreshold;	 /* added by mtk08176_20150605  */
	int32 amplitudeSlab;		 /* added by mtk08176_20150627  */	
}_slic_RingParams_s;

/*[OSBNB00047036] ADD by mtk07059_20150616 , support dc feed parameters set. */
typedef struct {
	int32 ila;
	int32 ilaSlab;		/* added by mtk08176_20150627  */
}_slic_DcFeedParams_s;

/*ADD by mtk08176_20150508 , support line test. */
typedef enum {
	SLIC_INFC0,
	SLIC_INFC1,
	SLIC_INFC2,
	SLIC_INFC3,
	SLIC_INFC4,
	SLIC_INFC5,
	SLIC_INFC6,
	SLIC_INFC7
} SLIC_infcId_e;

typedef struct {
	SLIC_infcId_e infc;
	uint8 lineTestId;
	char lineTestData[5000];
}infcLineTestParams_t;


typedef struct slic slic_t;
struct slic
{
	int (*setToneProf)(slic_t* o , int cid, sw_country_tone_t* country);
	
	/**
	 *  @brief flash hook during bound setting
     *	@param en enable/disable flash hook function	 
	 *	@param up_bound detected upper bound,unit 10ms
	 *	@param lw_bound detected lower bound,unit 10ms
	 */
    int (*setHookTs)(slic_t* o, int cid, slic_hookThreshold_t* hookTs);

	/**
	 *  @brief phone impendence setting
	 *	@param type set it as the ringin impendence
	 */
    int (*setRingImpd)(slic_t* o, int cid, slic_impdnc_t type);

	/**
	 *  @brief configure to detect FAX tone or not
	 *	@param en 1:enable/0:disable
	 */
    int (*setFaxDetect)(slic_t* o, int cid, int en);

	/**
	 *  @brief configure to remove DTMF tone
	 *	@param ul 1:enable/0:disable to remove DTMF tone on uplink data path
	 *	@param dl 1:enable/0:disable to remove DTMF tone on downlink data path	 
	 */
#ifndef FXS_DTMF_REMOVE
    int (*setDTMFRemove)(slic_t* o, int cid, int ul, int dl);

	/**
	 *  @brief configure to detect and generate DTMF Tone
	 *	@param en_det 1:enable/0:disable DTMF tone detection
	 */
    int (*setDTMFDetect)(slic_t* o, int cid, int en_det);
#else
    int (*setDTMFDetect)(slic_t* o, int cid, int en_det,int en_remove);
#endif	
	/** 
	 *	@brief the listener shall receive a event to report current hook status 
	 *		right after listener_set
	 */
	int (*setListener)(slic_t* o, slic_listener_t* listener);
	
	int (*ring)(slic_t* o, int cid, int en, int rep);
	
	/* by mtk07059_20140521 ,for tone_stop_event */
	int (*tone)(slic_t* o, int cid, int dir, int en, slic_tone_t type,int periodical, unsigned int total_dur);

	/*   by mtk07059_20150505, bug fix for tone discontinuos. */
	int (*toneSeq)(slic_t* o, int cid, int dir, int en, tone_seq_t* seq);

	/**
	 *	@param	ms,	milli second 
	 */
	int (*toneDTMF)(slic_t* o, int cid, char digit, int digitPower, int ms, int bs,int direction);
		
	/* added by mtk07059_20140610 ,  for pcm loopback test. */
	int (*pcmLoopback)(slic_t* o, int en);
	/*[OSBNB00041138] ADD by peter.jiang@20141126, support rtp/dsp loopback*/
	int (*rtpLoopback)(slic_t* o, int cid, int en);
	int (*dspLoopback)(slic_t* o, int cid, int en);

	int (*ringParams)(slic_t* o, int cid, _slic_RingParams_s* ringParams);	//mtk07059_20150423
	int (*dcFeedParams)(slic_t* o, int cid, _slic_DcFeedParams_s* dcFeedParams);	//mtk07059_20150616
	int (*slicLineTest)(slic_t* o,  infcLineTestParams_t*  lineTest);	//mtk08176_20150508

	
#ifdef V152_SUPPORT
	int (*VBDFlagSet)(slic_t* o, int ch, int en);/*mtk08176 for vbd*/
	int (*VBDSwitchFlagSet)(slic_t* o, int ch, int en);/*mtk08176 for vbd*/
#endif

	/**
	 *	@param tx_pw power in db
	 */
	int (*callidSend)(slic_t* o, int cid, char *callid, int tx_pw, int is_before_ring, enum cid_spec_e spec,int strlen,cid_t1_timing_params_t *cid1timing,cid_t2_timing_params_t *cid2timing);
	int (*audioGain)(slic_t* o, int cid, int rx_gain, int tx_gain);
	int (*audioDigitalGain)(slic_t* o, int cid, int rx_gain, int tx_gain);	

	int (*hookStatusGet)(slic_t* o, int cid,int *hook);
	int (*hookEqClean)(slic_t* o);

	/* DSP related function */
	int (*lec_enable)(slic_t* o, int cid,int enable);

	/* added by mtk07059_20140825,  for V21 damaged by EC.*/	
	int (*lec_aes_enable)(slic_t* o,int cid, int enable);
	int (*lec_bypass_set)(slic_t* o,int cid, int status);
	int (*lec_nlp_enable)(slic_t* o, int cid,int enable);

	int (*lec_config)(slic_t* o, int id,int value);

	int (*lec_enableGet)(slic_t* o, int cid,int *en);

	int (*nr_enable)(slic_t* o, int cid,int enable);

	int (*nr_config)(slic_t* o, int id,int value);

	int (*drc_enable)(slic_t* o, int cid,int enable);

	int (*lecModeSet)(slic_t* o,int cid, int mode);

	int (*nr_enableGet)(slic_t* o, int cid,int *en);
	
	int (*drc_enableGet)(slic_t* o, int cid,int *en);

	int (*setDTMFDetect_remove)(slic_t* o, int cid, int en_det,int en_remove);

	int (*ring_interval)(slic_t* o, int cid, int active_ms,int inactive_ms);//unit ms, negitive number meas reserve previous setting

	int (*slic_line_reverse)(slic_t* o, int cid, int reverse_ms);//unit ms

	int (*ring_cadence)(slic_t* o, int cid, ring_cadence_t* cadence,unsigned cadence_num);//unit ms
	
	int (*ring_profile)(slic_t* o, int cid, int profile);

	int (*before_ring_cid_line_reverse_enable)(slic_t* o, int cid, int en);

	int (*fxs_lineStatusSet)(slic_t* o,int cid, int status);
	
	int (*fxo_hookStatusSet)(slic_t* o,int cid, int status);

	int (*fxo_PSTNStatusGet)(slic_t* o,int cid, int *status);

	int (*cptTemplateConfig)(slic_t* o,int cptProIdx, VTSP_UtdTemplate *template);
	
    int (*cptDetSet)(slic_t* o, int cid, int en_det);

	int (*cidDetSet)(slic_t* o, int cid, int en_det);

	int (*cidDetParamConfig)(slic_t* o, int cid, fsk_det_params_t* config);

	int (*dtmfDetParamConfig)(slic_t* o, int cid, dtmfdet_params_t* config);
/*add for SLIC Reinit by Lisa.Dai,OSBNB00030576,20130329*/
	int (*slicHwProfSet)(slic_t* o,int cid,int hwProfId);
/*end of add for SLIC Reinit by Lisa.Dai*/
	void* priv;
};

slic_t*	SLIC_GetInstance(void);

#endif

