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
 *	File:	eva_struct.h
 *	 
 *	Author: Serena Huang, Quark Li
 *	 		
 *	Last Revision Date: 2011.9.29
 *	 
 *	Description:
 *	Enhance VoIP Architecture (EVA) generic data structure definitions.
 *
 */

#ifndef _EVA_STRUCT_H_
#define _EVA_STRUCT_H_

#include "eva_constant.h"

/* ========================================
 *	Generic data context section 
 * ======================================== */
#define MAX_CID_CHAR_LEN		(32)

typedef struct {
	uint32 	addrV4;
	uint16 	addrV6[8];
	ipVer_e 	ver; 	/* ipv4/ipv6 */
	uint16 	port; /* 1024~65535, 0 for any */
	uint16	rtcpPort;
}netAddr_t;

typedef struct {
	cidFormat_e format;
	char number[MAX_CID_CHAR_LEN];
	char name[MAX_CID_CHAR_LEN];
	char dateTime[MAX_CID_CHAR_LEN];
	char blockInfo[MAX_CID_CHAR_LEN];
	MWIValue_e mwi;
	int  fskPower;
	int  dtmfPower;
	uint16 makeTime;
	uint16 breakTime;
	uint32 dtmfT3;
	struct{
		uint32 T9; /*unit:10ms*/
		uint32 T10;
		uint32 T11;
		uint32 T12;
		uint32 T13;
		uint32 T14;
	}cid2timing;
struct{
		uint32 T2;
		uint32 T3;
		uint32 T4;
	}cid1timing;
}cid_t;

typedef union {
	struct{
		lineState_e status;
		int ringCount;
	}line; /* EVENT_CODE_LINE */

	struct{
		hookState_e status;
		int pulseCount;
	}hook; /*EVENT_CODE_HOOK*/
	
	struct{
		uint32 dspExecTimes;
		uint32 averageMhz;
	}performanceIdx; /*EVENT_CODE_PERFORMANCE*/
	
	struct{
		int8 number[MAX_CID_CHAR_LEN];
		int8 name[MAX_CID_CHAR_LEN];
		int8 dateTime[MAX_CID_CHAR_LEN];
	}cidData;/*EVENT_CODE_CID_DETECTED*/
	
	struct{
		toneCode_e code;
		toneDir_e dir;
		
		/* For jiffies,volume report. */
		unsigned long jiff;
		double volume;
		int dur;/*SIPINFO*/
		
	}tone; /*EVENT_CODE_TONE_DETECTED*/
	
	struct{
		uint8 streamId;
		t38State_e status;
	}t38; /*EVENT_CODE_T38*/
	
	struct{
		uint8 streamId;
		uint64 total;
		uint64 drop;
		uint32 plc;
		uint32 jbSize;
		uint32 avgJitter;
	}jb; /*EVENT_CODE_JB_UPDATE*/
	
	struct{
		netAddr_t srcAddr;
		netAddr_t dstAddr;
		uint8 payload[MAX_PACKET_SZ];
	}packet; /*EVENT_CODE_NON_RTP_RECVD*/
	struct{
		int cid;
		int sid;
		int notifyFlag;
	}voiceJb;/*EVENT_CODE_JB_CHANGE*/
}eventContext_u;

typedef struct {
	eventEdge_e 		edge;
	eventCode_e 		evtCode;
	uint32 			dspTick; /*dspTicks*/
	infcId_e			infcId;
	chanId_e			chanId;
	eventContext_u 	context;
	void (*eventNotify)(void *argv);
} event_t;

typedef struct {
	uint16 onTime;
	uint16 offTime;
} cadence_t;

/* ========================================
 *	Interface Section 
 * ======================================== */
typedef struct {
	cadence_t cad[MAX_CADENCE];
	uint32	dur;
	cid_t	cid;
	uint8	cidAt;
	uint32	cidWaitTime;
} ringProfile_t;

typedef struct {
	uint32	genSeize;
	uint32	genRelease;
	uint32	detSeizeMin;
	uint32	detSeizeMax;
	uint32	detReleaseMin;
	uint32	detReleaseMax;
} pulseTime_t;

typedef struct {
    uint32	flashMin;
    uint32	flashMax;
    uint32	releaseMin;
	uint32	autoFlashDur;
	pulseTime_t pulseTime;
	uint8		pulseEnable;
	uint32  seizeMin;
} hookThreshold_t;

/*Support ring parameters set. */
typedef struct {
	infcRingType_e	type;
	int32 frequence;
	int32 amplitude;
	int32 dcBias;
	int32 ringTripThreshold;                			  
	int32 amplitudeSlab;								  
}infcRingParams_t;

/*Support DC feed parameters set. */
typedef struct {
    int32 ila;
    int32 ilaSlab;					 
}infcDcFeedParams_t;

typedef struct {
	infcType_e	type;
	lineState_e	lineState;
	hookState_e hookState;
	polDir_e		pol;
	ringProfile_t	ring;
	hookThreshold_t hookTs;
	hwProf_e hwProfId;
	infcRingParams_t ringParams;	
	infcDcFeedParams_t dcFeedParams;	
}infcConfig_t;

/*Support line test. */
typedef struct {
	infcId_e infc;
	uint8 lineTestId;
	char lineTestData[5000];
}infcLineTest_t;

/*Support line test. */
typedef struct {
	uint8 fxsNum;
	uint8 fxoNum;
	char slicType[15];
}slicParams_t;

typedef struct {
	int slicType;
	char slic_name[16];;
}slicType_t;

/*zarlink*/
typedef enum {
    EVA_LT_TID_RSVD1,           /* Reserved TID */
    EVA_LT_TID_LINE_V,          /* Line Voltage Test */
    EVA_LT_TID_ROH,             /* Receiver Off-Hook indication */
    EVA_LT_TID_RINGERS,         /* Ringers test per FCC Part 68 REN def.*/
    EVA_LT_TID_RES_FLT,         /* Resistive Fault */
    EVA_LT_TID_MSOCKET,         /* Master Socket detection */
    EVA_LT_TID_XCONNECT,        /* Cross Connect detection */
    EVA_LT_TID_CAP,             /* Capacitance measurement test */
    EVA_LT_TID_ALL_GR_909,      /* All GR-909 fault tests in predefined order*/
    EVA_LT_TID_LOOPBACK,        /* Loopback test */
    EVA_LT_TID_DC_FEED_ST,      /* DC Feed Self Test */
    EVA_LT_TID_RD_LOOP_COND,    /* Read Loop Condition */
    EVA_LT_TID_DC_VOLTAGE,      /* DC VOLTAGE Test */
    EVA_LT_TID_RINGING_ST,      /* Ringing Self Test */
    EVA_LT_TID_ON_OFF_HOOK_ST,  /* On/Off hook Self Test */
    EVA_LT_TID_RD_BAT_COND,     /* Read battery conditions */
    EVA_LT_TID_PRE_LINE_V,      /* Pre Line Voltage Test */
    EVA_LT_TID_FLT_DSCRM,       /* Fault Discrimination Test */
    EVA_LT_NUM_TID_CODES
} EVA_LtTestIdType;

/*siliconlab*/
typedef enum {
    EVA_LT_TID_RSVD1_SLAB,           /* Reserved TID */
    EVA_LT_TID_HAZARDOUS_V,          /* Hazardous Voltages Test */
    EVA_LT_TID_FOREIGN_V,              /* Foreign Voltages Test */
    EVA_LT_TID_RESISTIVE_FAULTS,         /* Resistive Faults Test.*/
    EVA_LT_TID_RES_OFFHOOK,         /* Receiver Offhook Test*/
    EVA_LT_TID_REN,         /* REN Test */
    EVA_LT_TID_CAP_FAULTS,        /* Capacitive Faults Test */
    EVA_LT_TID_REN_CAP,             /* REN Capacitive Test */
    EVA_ProSLIC_LineMonitor,		/* Inline Monitor*/				/* mtk08176_20150824 */
    EVA_LT_NUM_TID_CODES_SLAB
} EVA_LtTestIdTypeSLAB;



/***************line test struct******************/
/*zarlink*/
#define EVA_INT_MAX 	0x7fffffff
#define EVA_FORCE_STANDARD_C_ENUM_SIZE  (EVA_INT_MAX)


/* REN Measurement test type */
typedef enum {
    EVA_LT_RINGER_REGULAR_PHNE_TEST,            /* Perform REN test on regular phone */
    EVA_LT_RINGER_ELECTRONIC_PHNE_TEST,         /* Perform REN test on electronic phone */
    EVA_LT_RINGER_REGULAR_PHNE_TEST_3_ELE,      /* Perform 3 ELE REN test on regular phone */
    EVA_LT_RINGER_ELECTRONIC_PHNE_TEST_3_ELE,   /* Perform 3 ELE REN test on electronic phone */
    EVA_LT_RINGER_TEST_NUM_TYPES,
    EVA_LT_RINGER_BIT_ENUM_SIZE = EVA_FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} EVA_LtRingerTestType;


/* Line Voltage Test  testid=1   EVA_LT_TID_LINE_V*/
typedef struct {
    uint16  fltMask; /* Can be any or'ed comb of this type */
    uint16  measStatus;
    int32  vAcTip;
    int32  vAcRing;
    int32  vAcDiff;
    int32  vDcTip;
    int32  vDcRing;
    int32  vDcDiff;
} EVA_LtLineVResultType;

/* Receiver Off-Hook indication  testid=2  EVA_LT_TID_ROH*/
typedef struct {
    uint16  fltMask;
    int32  rLoop1;
    int32  rLoop2;
    uint16  measStatus;
} EVA_LtRohResultType;

/* Ringers test per FCC Part 68 REN def.  testid=3  EVA_LT_TID_RINGERS*/
typedef struct {
    uint16  fltMask;
    uint16  measStatus;
    int32  ren;      /* REN from the Tip to Ring */
    int32  rentg;    /* REN from the Tip lead to ground */
    int32  renrg;    /* REN from the Ring lead to ground */
    EVA_LtRingerTestType ringerTestType;
} EVA_LtRingersResultType;

/* Resistive Fault  testid=4  EVA_LT_TID_RES_FLT*/
typedef struct {
    uint16  fltMask;

    int32  rtg;    /* Resistance from the Tip lead to ground */
    int32  rrg;    /* Resistance from the Ring lead to ground */
    int32  rtr;    /* Resistance between Tip and Ring leads */

    /* The following resistance is measured only if a fault (or low resistance)
     * exists between the Tip and Ring leads */
    int32  rGnd;   /* Resistance to ground */

    uint16  measStatus;
} EVA_LtResFltResultType;

/* Capacitance measurement test   testid=7  EVA_LT_TID_CAP*/
typedef struct {
    uint16  fltMask;

    int32  ctg;    /* Capacitance from the Tip lead to ground */
    int32  crg;    /* Capacitance from the Ring lead to ground */
    int32  ctr;    /* Capacitance between Tip and Ring leads */

    uint16  measStatus;
} EVA_LtCapResultType;

/*  DC Feed Self Test  testid=10  EVA_LT_TID_DC_FEED_ST*/
typedef struct {
    uint16  fltMask;
    uint16  measStatus;

    /* Measured test load */
    int32  rTestLoad;

    /* Measured voltage across the test load */
    int32  vTestLoad;

    /* Measured current through the test load */
    int32  iTestLoad;

} EVA_LtDcFeedSTResultType;

/*  Read Loop Condition  testid=11  EVA_LT_TID_RD_LOOP_COND*/
typedef struct {
    uint16  fltMask;
    uint16  measStatus;
    int32  rloop;          /* Measured loop resistance */
    int32  imt;              /* Sensed metalic current */
    int32  ilg;              /* Sensed longitudinal current */
    int32  vab;              /* Sensed voltage on AB (tip/ring) leads */
    int32  vag;              /* Sensed voltage on AG (ring/ ground) leads */
    int32  vbg;              /* Sensed voltage on BG (tip/ground) leads */
    int32  vbat1;            /* Battery 1 measured voltage */
    int32  vbat2;            /* Battery 2 measured voltage */
    int32  vbat3;            /* Battery 3 measured voltage */
} EVA_LtRdLoopCondResultType;


/* DC VOLTAGE Test  testid=12  EVA_LT_TID_DC_VOLTAGE*/
typedef struct {
    uint16  fltMask;
    uint16  measStatus;

    /* Measured voltages */
    int32  measuredVoltage1; /* When +testVoltage is applied */
    int32  measuredVoltage2; /* When -testVoltage is applied */

} EVA_LtDcVoltageResultType;

/* Ringing Self Test  testid=13  EVA_LT_TID_RINGING_ST*/
typedef struct {
    uint16  fltMask;

    /* Measured open circuit ringing voltage */
    int32 openCktRingVol;

    /* Measured ringing frequency */
    int32  freq;

    /* Indicates the computed AC test load resistance that was applied during
     * the test. */
    int32  acRload;

    /* Measured current through the rLoad (RMS) */
    int32  iRload;

    uint16  measStatus;
} EVA_LtRingingSTResultType;

/* On/Off hook Self Test  testid=14  EVA_LT_TID_ON_OFF_HOOK_ST*/
typedef struct {
    uint16  fltMask;
    uint16  measStatus;
} EVA_LtOnOffHookSTResultType;

/* Read battery conditions  testid=15  EVA_LT_TID_RD_BAT_COND*/
typedef struct {
    uint16  fltMask;
    uint16  measStatus;

    /* Measured battery voltages */
    int32  bat1;
    int32  bat2;
    int32  bat3;

} EVA_LtReadBatCondResultType;

/* Pre Line Voltage Test  testid=16  EVA_LT_TID_PRE_LINE_V*/
typedef struct {
    uint16       fltMask;
    uint16   measStatus;
} EVA_LtPreLineVResultType;



/*SLICONLAB*/
#define EVA_MLT_MAX_IV_SAMPLES     6
#define EVA_MLT_MAX_PRESERVE_RAM   128
#define EVA_MLT_MAX_PRESERVE_REG   32


/**
@internal
@brief    Structure to store I/V measurements
*/
typedef struct {
    int32 vtip[EVA_MLT_MAX_IV_SAMPLES];
    int32 vring[EVA_MLT_MAX_IV_SAMPLES];
    int32 itip[EVA_MLT_MAX_IV_SAMPLES];
    int32 iring[EVA_MLT_MAX_IV_SAMPLES];
    int32 vloop[EVA_MLT_MAX_IV_SAMPLES];
    int32 iloop[EVA_MLT_MAX_IV_SAMPLES];
    int32 vlong[EVA_MLT_MAX_IV_SAMPLES];
    int32 ilong[EVA_MLT_MAX_IV_SAMPLES];
} EVA_ProSLIC_iv_t;

/**
@brief    General terminal measurement structure
*/
typedef struct {
    int32 measTR;
    int32 measTG;
    int32 measRG;
    int32 measAUX;
    int32 measTR2;
    int32 measTG2;
    int32 measRG2;
    int   trFlag;
    int   tgFlag;
    int   rgFlag;
    int   auxFlag;
    uint16   time1;
    uint16   time2;
    uint16   time3;
    uint8 resultsValid;
	uint8 ahsFlag;
    EVA_ProSLIC_iv_t term;
    uint8  flags[16];
    int32  slopeErr;
} EVA_ProSLIC_term_meas_t;

typedef struct {
    int rohTrue;
    int resultsValid;
} EVA_ProSLIC_roh_meas_t;

/**
 @brief  Structure for REN test calibration coefficients to be stored
*/
typedef struct {
	int32 highRenSlope; 
	int32 lowRenSlope ; 
	int32 highRenOffs ; 
	int32 lowRenOffs  ;
	int32 renTrans    ; 
	int32 extraLowRenSlope;
	int32 extraLowRenOffset;
} EVA_ProSLIC_rencal_t;


typedef struct {
    EVA_ProSLIC_rencal_t calData;
    int32 renValue;
    int   renFlag;
    int   renCalFlag;
    int   resultsValid;
} EVA_ProSLIC_ren_meas_t;

typedef struct {
	uint16 address;
	uint32 initValue;
} EVA_ProslicRAMInit;

typedef struct {
	uint8 address;
	uint8 initValue;
} EVA_ProslicRegInit;

#ifdef WIN32
typedef struct {
    _int64 time;
} EVA_timeProfile;
#endif

/**
 @brief  Structure for optional runtime/per channel MLT configuration
*/
typedef struct {
    uint16 mlt_poll_rate_offs;
    uint16 mlt_poll_rate;
    int32  mlt_prot_res;
    int32  mlt_line_cap;
    int32  mlt_emi_cap;
    int32  mlt_femf_samples;
    uint8  mlt_ext_v_flag;
}EVA_ProSLIC_mlt_config_t;


typedef struct{
	int * pProslic;                      /**< Linkage to ProSLIC API channel structure */
    EVA_ProSLIC_term_meas_t resFaults;                     /**< Stores results of resistive faults test */
    EVA_ProSLIC_term_meas_t resFaultsRaw;
    EVA_ProSLIC_term_meas_t hazVAC;                        /**< Stores AC results of foreign/hazardous voltage test */
    EVA_ProSLIC_term_meas_t hazVDC;                        /**< Stores DC results of foreign/hazardous voltage test */
    EVA_ProSLIC_term_meas_t capFaults;                     /**< Stores results of capacitance test */
    EVA_ProSLIC_term_meas_t capFaultsRaw;
    EVA_ProSLIC_term_meas_t hazIDC;
    EVA_ProSLIC_term_meas_t hazIAC;
    EVA_ProSLIC_roh_meas_t roh;                               /**< Stores results of receiver offhook test */
    EVA_ProSLIC_ren_meas_t ren;                               /**< Stores results of REN test */
    EVA_ProslicRAMInit preserveRAM[EVA_MLT_MAX_PRESERVE_RAM];     /**<  Storage for resotoring entry conditions */
    EVA_ProslicRegInit preserveReg[EVA_MLT_MAX_PRESERVE_REG];     /**<  Storage for resotoring entry conditions */
    EVA_ProslicRAMInit preserveAuxRAM[EVA_MLT_MAX_PRESERVE_RAM];  /**< Auxillary storage for revision specific locations */
    EVA_ProslicRegInit preserveAuxReg[EVA_MLT_MAX_PRESERVE_REG];  /**< Auxillary storage for revision specific locations */
#ifdef WIN32
    EVA_timeProfile startTime;
    EVA_timeProfile timeDelay;
    int exec_time;
#endif
#ifdef MLT_DLL
    EVA_timeProfile runTime;
#endif
    EVA_ProSLIC_mlt_config_t    mlt_config;
    int      pm_bom_flag;
}EVA_ProSLICMLTType;

/***************line test struct******************/

/* China Unicom inside and outside line test */

typedef enum {
    OutLine_Normal,          		 /* Normal */
    OutLine_PhoneDisconnet,          /* PhoneDisconnet */
    OutLine_PhoneoffHook,            /* PhoneoffHook*/
    OutLine_PowerLineContacted,      /* PowerLineContacted: AGACVoltage or BGACVoltage is >50Vrms*/
    OutLine_BothLineContacted,       /* BothLineContacted: ALineMixOther && BLineMixOther*/
    OutLine_ALineMixOther ,          /* ALineMixOther: AGDCVoltage >40V*/
    OutLine_BLineMixOther,           /* BLineMixOther: BGDCVoltage >40V*/
    OutLine_BothLineGrounding,       /* BothLineGrounding:ALineGrounding && BLineGrounding*/
    OutLine_ALineGrounding,          /* ALineGrounding:AGInsuluationResistance <20 000 ohm*/
    OutLine_BLineGrounding,          /* BLineGrounding: BGInsuluationResistance <20 000 ohm*/
    OutLine_ABLinePoorInsulation,    /* ABLinePoorInsulation: ABInsuluationResistance is between 2 000 ohm to 20 000 ohm*/
    OutLine_ShortCircuit,    		 /* ShortCircuit: ABInsuluationResistance is <500 ohm*/
    OutLine_BothLineLeakageToGround, /* BothLineLeakageToGround: ALineLeakageToGround && BLineLeakageToGround*/
    OutLine_ALineLeakageToGround,    /* ALineLeakageToGround:AGCapacitance>0.7 uF */
    OutLine_BLineLeakageToGround     /* BLineLeakageToGround:AGCapacitance>0.7 uF*/
} Outline_Status;

typedef struct{
    int32 AGACVoltage;             /* Tip to Ground AC voltage: mVrms */
    int32 BGACVoltage;             /* Ring to Ground AC voltage: mVrms*/
    int32 ABACVoltage;             /* Tip to Ring AC voltage: mVrms*/
    int32 AGDCVoltage;             /* Tip to Ground DC voltage: mV */
    int32 BGDCVoltage;             /* Ring to Ground DC voltage: mV */
    int32 ABDCVoltage;             /* Tip to Ring DC voltage: mV*/
    int32 AGInsuluationResistance; /* Tip to Ground  resistance: ohm */
    int32 BGInsuluationResistance; /* Ring to Ground  resistance: ohm */
    int32 ABInsuluationResistance; /* Tip to Ring  resistance: ohm */
    int32 AGCapacitance;           /* Tip to Ground  Capacitance: pF */
    int32 BGCapacitance;           /* Tip to Ground  Capacitance: pF */
    int32 ABCapacitance;  		   /* Tip to Ground  Capacitance: pF*/
} Outline_Param;


//inline test
typedef enum {
    Inline_Test_Item_Normal,
    Inline_Test_Item_UNNormal
} Inline_Status;

typedef struct {
    signed long DCFeed_Voltage;   		 /*  Tip To Ring Voltage:V,the default vaule is set by interface*/
    signed long Ringing_Voltage;			 /*  Tip To Ring Voltage when ring:Vrms,the default value is set by interface*/
    signed long Loop_Current;			 /* Tip to Ring Current:mA*/	
} Inline_Param;

typedef enum {
    ZarlinkOnHook,
    ZarlinkPhoneDisconnet,
    ZarlinkOffHook
} ZarlinkPhoneStatus;

typedef struct {
    Outline_Param Outline;
    Inline_Param Inline;
    Outline_Status OutlineStatus;
    Inline_Status InlineDCVStatus;
    Inline_Status InlineRingVStatus;
    Inline_Status InlineCurrentStatus;
} infcUnicomLineTest_t;

typedef struct 
{
    int32  vtr;		/**< Voltage, tip-ring in mV */
    int32  vtip;	/**< Voltage, tip-ground in mV */
    int32  vring;	/**< Voltage, ring-ground in mV */
    int32  vbat;	/**< Voltage, battery in mV */
    int32  vdc;		/**< Voltage, Vdc in mV */
    int32  vlong;	/**< Voltage, longitudinal in mV */
    int32  itr;		/**< Loop current, in uA tip-ring */
    int32  itip;	/**< Loop current, in uA tip */
    int32  iring;	/**< Loop current, in uA ring */
    int32  ilong;	/**< Loop current, in uA longitudinal */
    int32  p_hvic;  /**< On-chip Power Calculation in mw */
} EVA_proslicMonitorType;

/* China Unicom inside and outside line test */


/* ========================================
 *	Stream Section 
 * ======================================== */
typedef struct {
	ipVer_e		ver;
	netAddr_t 	srcAddr;
	netAddr_t 	dstAddr;
	uint8 		encrypt; /*T/F*/ /*reserved for user to select the encypt type*/
	uint32 		dur; /*session time*/ /*reserved for update the session duration*/
	uint32 		rtpTosVal; 
	uint32 		rtcpTosVal; 
	uint32 		rtp8021pVal; 
	uint32 		rtcp8021pVal; 
	int 		vlan_prio; /*qos specific for zte*/
	int 		queue_num; /*qos specific for zte*/
	int 		plc_enable; 
	int 		rtcp_report_interval;/* For rtcp report interval set. */
}session_t;

typedef struct {
	codec_e			payloadSelect;
	uint8			ulPayloadSelect; 
	enableControl_e 	rtpRedundant; /* For_redundant */
	redMode_e		rtpRedundantMode;	
	uint8           	rtpRedundantPt; /* For redundant payload type */
	enableControl_e 	rfc2833Redundant; /*for 2833 redundancy enable*/
	redMode_e		rfc2833RedundantMode;	/* for 2833 redundancy mode */
	pTime_e			ulPtime;
	pTime_e			dlPtime;
	evaBitrate_e    	ulBitrate; /* Bit rate */  
	enableControl_e		dtmfRelay; /*enable/disable*/
	enableControl_e		dtmfRemove; /*enable/disable*/
	enableControl_e		VAD; /*enable/disable*/
	enableControl_e		CNG; /*enable/disable*/
	enableControl_e		silenceComp; /*enable/disable*/
	strmDir_e 		direction;
	uint32 			jbUpdateTime; /*duration of JB_EVENT update, 0 to turn off*/
	uint8			rfc2833DtmfVolume;
	uint8 			tevCtlFlag;/*0x01:piggyback events 0x02:tone events 0x04:CAS tone events*/
	enableControl_e 	vbdEnable;/*For V152*/
	uint8			vbdCodec;/*For V152*/
	uint8			vbdPt; /*For V152*/
	vbdfaxmodemflag		vbdModemFaxFlag; /*For V152*/
	codec_e 		payloadSelectSave; /*For V152*/
	uint8			faxStage;	/* For V152 ZTE. 0,default; 1,faxstart; 2,high speed; 3,low speed; . */
	evaBool_e		isV152;	/* For V152 ZTE. is in V152 stage. */
}strmAttr_t;

typedef struct {
	jbMode_e mode;
	uint32 szJbMax;
	uint32 szJbInit;
	int type;
}jbConfig_t;

typedef struct {
	int32 version;
	int32 maxRate;
	int32 eccType;
	int32 rateMgnt;
	int32 opMode; /*(0: unknown, 1: caller, 2: callee)*/
    /* For control params*/
	int32 connection_type;
	int32 max_jitter;
	int32 ecc_signal;
	int32 ecc_num;
	int32 ecm_enable;
	int32 nsf_enable; 
	int32 imagePktSize;/*not used in v3.12*/
	int32 maxRemoteUDPTL;
}t38Ctrl_t;


typedef enum {
	CONFIG_SUCCESS = 0,	
	CONFIG_FAIL = 1,
} strmConfig_result;

typedef struct {
	session_t 		session;
	strmAttr_t 		strmAttr;
	uint8 			payloadType[MAX_CODEC_NUM];
	jbConfig_t		jbConf;
	t38Ctrl_t		t38Ctrl;
	strmConfig_result preConfigResult;
}strmConfig_t;

typedef struct {
	uint64	rtpError;
	uint64	packetRecv;
	uint64	packetLoss;
	uint64	packetLossRate;
	uint64  packetOutoftime;  /* number of packets that were received too late */
	uint64  packetDiscarded;  /* incoming packets discarded because the queue exceeds its max size */    
	uint64	maxJitter;
	uint64	maxRTCPInterval; /*milliseconds*/
	uint64	bufUnderflow;
	uint64	bufOverflow;
	uint64 	packetSent;
	uint64 	bytesSent;		
	uint64 	bytesRecv;
	uint64	farEndpacketLossRate;
	uint64	avgJitter;
	uint64	farEndJitter;
	uint64	avgFarEndJitter;
	uint64 	avgRTCPInterval; /*milliseconds*/
	uint64	revInternalJitter;     /*milliseconds*/
	uint64	avgRevInternalJitter;  /*milliseconds*/
	uint64	roundTripDelay;      /*milliseconds*/
	uint64 	avgRoundTripDelay;   /*milliseconds*/
    uint64  maxRoundTripDelay;   /*max value of round trip delay */
    uint64  rtcpPacketRecv;      /*number of ingress rtcp packets*/
    uint64  rtcpPacketSend;      /*number of egress rtcp packets*/
    uint64  rtcpXrPacketRecv;    /*number of ingress rtcp XR packets*/
    uint64  rtcpXrPacketSend;    /*number of egress rtcp XR packets*/
    float   MOSLQ; /*MOS_Listening quality*/
    float   MOSCQ; /*MOS_Conversational quality*/
}mediaInfo_t;


/* ========================================
 *	Channel Section 
 * ======================================== */
typedef struct {
	uint16 	detectMask; //bitmask
	int8 		ampTx; // speak gain,db, step by 0.5
	int8 		ampRx; // listen gain,db, step by 0.5
	int8		digitTx; //speak gain,,db, step by 0.5
	int8		digitRx; // listen gain,db, step by 0.5

	struct {
		int32 power;
		uint32 duration;/*unit:5ms*/
		Dtmf_Deviation devAcc;
		Dtmf_Deviation devRej;
	}dtmfdet;

	enableControl_e ecEnable;
	enableControl_e ecNlpEnable;
	lecTailMode_e ecTailMode;
	enableControl_e ecAesEnable;
	enableControl_e ecBypass;

}chanConfig_t;

/* ========================================
 *	DSP Section 
 * ======================================== */

typedef struct tone_regular_s
{
    uint16 	toneFreq[MAX_TONE_FREQ];
	int16	tonePwr[MAX_TONE_FREQ]; 
}tone_regular_t;

typedef struct tone_modulate_s
{
    int16   baseFreq;
    int16   modFreq;
    int16   modPwr;
    int16   modDepth;
}tone_modulate_t;


typedef struct {
	toneType_e toneType;

    #if defined(FIVE_CADENCE_SUPPORT) || defined(CJ_DSP_SUPPORT) 
    tone_regular_t regular[MAX_CADENCE];//support config different tone attribute in different cadence
    tone_modulate_t modulate[MAX_CADENCE];
    #else
    tone_regular_t regular;//just support config the same tone attribute in different cadence
    tone_modulate_t modulate;
    #endif
	
	uint16 makeTime[MAX_CADENCE];
	uint16 breakTime[MAX_CADENCE];
	uint16 repeat[MAX_CADENCE];	
}tone_t;

typedef struct {
	uint8 *toneIdSeq;
	uint8 numOfTone;
}toneSeq_t;

typedef struct {
    toneCode_e	type;
	
	union {
		struct {
			int16	toneFreq[DUAL_TONE_FREQ];
			int16	toneDev[DUAL_TONE_FREQ];
			int16	minMake[MAX_CADENCE];
			int16	maxMake[MAX_CADENCE];
			int16	minBreak[MAX_CADENCE];
			int16	maxBreak[MAX_CADENCE];
			int16	power;
		} dual;
		struct {
			int16	toneFreq[SIT_TONE_FREQ];
			int16	toneDev[SIT_TONE_FREQ];
			int16	minShortDur;
			int16	maxShortDur;
			int16	minLongDur;
			int16	maxLongDur;
			int16	power;
		} sit;
	};
}cpt_t;

typedef struct {
	uint16 	dspId;
	uint8 	numOfChan;
	uint8 	strmsPerChan;
	uint32 	codecSupport; /*bitmask*/
	uint32 	ptimeSupport; 	/*bitmask*/
	uint8 	rtpRedundancy; /*T/F*//*not support currently*/
	uint8 	maxEcTailLength;
}dspFeature_t;

typedef struct {
	Dtmf_Deviation devAcc;
	Dtmf_Deviation devRej;
}dtmfdet_t;
typedef enum {
   DSP_RX,
   DSP_TX,
   DSP_INVALID
}dsp_dir;

typedef struct {
	int enable;
	dsp_dir dir;
	int stage;
	char filePath[128];
}replaceData_t;

typedef struct {
	int module;
	int adamLevel;
	int gdiLevel;
	int printType;
}logctrl_t;


typedef struct {
	/*dsp Function*/
	exCode_e (*dspInvoke)		(void);
	exCode_e (*dspRevoke)		(void);
	exCode_e (*dspQuery)		(activeState_e *pDspActive, dspFeature_t *pFeature);
	exCode_e (*dspPollEvent)	(blockMode_e mode, event_t *pEvent);
	exCode_e (*dspConfigTone)	(uint16 toneId, tone_t *pTone);
	exCode_e (*dspConfigCpt)	(cpt_t *pCpt);
	exCode_e (*dspLog)			(evaLogLevel_e level, char* func, const int line, char *fmt,...);

	/*channel Function*/
	exCode_e (*chanQuery)		(chanId_e ch, chanConfig_t *pChanConf);
	exCode_e (*chanConfig)		(chanId_e ch, chanConfig_t *pChanConf);

	/* For tone_stop_event */
	exCode_e (*chanPlayTone)	(chanId_e ch, toneSeq_t *pToneSeq, uint32 repeat, uint32 totalDur);
	exCode_e (*chanStopTone)	(chanId_e ch);
	exCode_e (*chanPlayCid)		(chanId_e ch, cid_t *pCid);
	exCode_e (*chanPlayType2Cid)(chanId_e ch, cid_t *pCid);
	exCode_e (*chanDumpPcm)	(chanId_e ch, netAddr_t *pAddr);
	exCode_e (*chanPlayDtmf) (chanId_e ch, char digit, uint32 dur);
	exCode_e (*chanPlayDtmfCid) (chanId_e ch, cid_t *pCid, enableControl_e ringEnable);
	/*stream Function*/
	exCode_e (*strmQuery)		(chanId_e ch, strmId_e strm, activeState_e *pStrmActive, strmConfig_t *pStrmConf);
	exCode_e (*strmConfig)		(chanId_e ch, strmId_e strm, strmConfig_t *pStrmConf);
	exCode_e (*strmStart)		(chanId_e ch, strmId_e strm);
	exCode_e (*strmStop)			(chanId_e ch, strmId_e strm);
	exCode_e (*strmSendDtmfr)	(chanId_e ch, strmId_e strm, dtmf_e dtmf, uint32 dur);
	exCode_e (*strmPlayTone)		(chanId_e ch, strmId_e strm, toneSeq_t *pToneSeq, uint32 repeat);
	exCode_e (*strmStopTone)	(chanId_e ch, strmId_e strm);
    exCode_e (*strmPlayDtmf)		(chanId_e channelID,char digit, uint32 duration);
	exCode_e (*strmQueryMediaInfo)		(chanId_e ch, strmId_e strm, mediaInfo_t *pMediaInfo);
	exCode_e (*strmResetMediaInfo)	(chanId_e ch, strmId_e strm);
	
	/*infc Function*/
	exCode_e (*infcQuery)		(infcId_e infc, infcConfig_t *pInfcConf);
	exCode_e (*infcConfigLine)	(infcId_e infc, lineState_e state);
	exCode_e (*infcConfigHook)	(infcId_e infc, hookState_e state);
	exCode_e (*infcConfigRing)	(infcId_e infc, ringProfile_t *pRingProf);
	exCode_e (*infcRing)			(infcId_e infc, uint32 dur, cid_t *pCid);
	exCode_e (*infcStopRing)		(infcId_e infc);
	exCode_e (*infcConfigHookTs)	(infcId_e infc, hookThreshold_t *pHookTs);
	exCode_e (*infcConfigHwProf)	(infcId_e infc, hwProf_e hwProfId);
	exCode_e (*infcRingParams)	(infcId_e infc, infcRingParams_t *ringParams);	
	exCode_e (*infcLineTest)	(infcLineTest_t*  lineTest);	/* Support line test.*/
	exCode_e (*infcSlicTypeQuery)	(infcId_e infc, slicParams_t* slicParams);	/* Support line test.*/
	exCode_e (*infcDcFeedParams)	(infcId_e infc, infcDcFeedParams_t *dcFeedParams);	
	
	/* For pcm loopback test. */	
	exCode_e (*chanPcmLoopback) ( enableControl_e en) ;
	/*Support rtp/dsp loopback*/
	exCode_e (*chanRtpLoopback) (chanId_e ch, enableControl_e en) ;
	exCode_e (*chanDspLoopback) (chanId_e ch, enableControl_e en) ;
	exCode_e (*chanDspReplaceData) (chanId_e ch, replaceData_t *pReplaceConf);

}dspObject_t;

#endif /* End of _EVA_STRUCT_H_ */

