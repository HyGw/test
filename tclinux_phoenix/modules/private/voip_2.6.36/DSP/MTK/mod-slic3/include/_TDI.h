/*
 * _TDI.h
 *
 *  Created on: Nov 9, 2010
 *      Author: pork
 */
#ifndef __TDI_H_
#define __TDI_H_
#include "TDI.h"


/********Definition********/
#if 0
#define DBG_NONE 0
#define DBG_ERROR 1
#define DBG_LOG 2
#define DBG_LEVEL DBG_ERROR
#define dbg_printf(fmt, val...) {if(DBG_ERROR <= DBG_LEVEL){printk("[DBG %s:L%d]", __func__, __LINE__);printk(fmt, ##val);printk("\n");}}
#define event_printf(fmt, val...) {if(DBG_LOG <= DBG_LEVEL){printk("[%s:L%d]", __func__, __LINE__);printk(fmt, ##val);printk("\n");}}
#endif

#define _TDI_SUCCESS    1
#define _TDI_FAIL      0

#define _TDI_TRUE    (1)     /* Boolean constant */
#define _TDI_FALSE   (0)     /* Boolean constant */

#define _TDI_CHANNEL_0	   0
#define _TDI_CHANNEL_1     1
#define _TDI_CHANNEL_2     2

#define _TDI_DEVICE_NUM	  			TIC_DEVICE_NUM
#define _TDI_NUM_LINES_PER_DEVICE     	2
#define _TDI_MAX_LINES	  			MAX_DEVICES*NUM_LINES_PER_DEVICE

#define _TDI_FXS_NUM	  			TIC_INFC_FXS_NUM
#define _TDI_FXO_NUM	  			TIC_INFC_FXO_NUM
#define _TDI_INFC_NUM				TIC_INFC_NUM
#define _TDI_INVALID_FXS_NUM		255

#define _TDI_HW_PROF_STR_LEN 64
/* Flash Time [msec] */
//#define _TDI_FLASH_COUNTER  	100 /*msec*/ the sams as flash min/max
/*
 * FXS interrupt bit location in the interrupt status word
 */
#define _TDI_FXS_INTR_RINGTRIP        (0)
#define _TDI_FXS_INTR_LOOPCLOSURE     (1)
#define _TDI_FXS_INTR_RING_ACTIVE     (2)
#define _TDI_FXS_INTR_RING_INACTIVE   (3)

/*
 * Minimum, maximum and default times for hook state and ringing
 */
#define _TDI_SEIZE_REAL_MIN            (8)    /* Determine a 'real' seizing */
#define _TDI_SEIZE_TIME_MIN            (20)   /* In # TIC_run() calls units */
#define _TDI_RELEASE_TIME_MIN          (70)   /* (150)  EIA 464 */
#define _TDI_FLASH_TIME_MIN            (25)   /* (30            */
#define _TDI_FLASH_TIME_MAX            (60)   /* (100)  EIA 464 */
#define _TDI_RING_BREAK_MAX            (800)  /* In # TIC_run() calls units */
#define _TDI_RING_MAKE_MAX             (800)
#define _TDI_RING_CADS_MAX             (3)
#define _TDI_RING_CADS_MIN             (1)
#define _TDI_RING_BREAK_DEF            (400)  /* Standard USA */
#define _TDI_RING_MAKE_DEF             (200)  /* Standard USA */
#define _TDI_RING_RET_DEF				(60)
#define _TDI_RING_CADS_DEF             (1)
#define _TDI_IVSETTLE_TIME             (10)

#define FXS_PULSE_OFFHOOK_MAX      (10000)
#define FXS_PULSE_ONHOOK_MAX    (10000)
#define FXS_PULSE_SEIZE_MAX       (35)
#define FXS_PULSE_SEIZE_MIN       (2)
#define FXS_PULSE_RELEASE_MAX      (10)
#define FXS_PULSE_RELEASE_MIN      (4)

#define FXS_PULSE_STATE_OFFHOOK	(0)
#define FXS_PULSE_STATE_ONHOOK	(1)
#define FXS_PULSE_STATE_SEIZE	(2)
#define FXS_PULSE_STATE_RELEASE	(3)

/*
 * Different states the ring generator can be in.
 */
#define _TDI_RING_STATE_DONE           (0)
#define _TDI_RING_STATE_MAK1           (1)
#define _TDI_RING_STATE_BRK1           (2)
#define _TDI_RING_STATE_MAK2           (4)
#define _TDI_RING_STATE_BRK2           (8)
#define _TDI_RING_STATE_MAK3           (16)
#define _TDI_RING_STATE_BRK3           (32)
#define _TDI_RING_STATE_INIT           (64)


#define _TDI_TX_GAIN_LEVEL            (0x4000)
#define _TDI_RX_GAIN_LEVEL            (0x4000)

/*
 * TIC command issue flag
 */
#define _TDI_CMD_EXECUTED             (0)
#define _TDI_CMD_ISSUED               (1)


/*
 * Check back time if infinite tone has to be generated.
 */
#define _TDI_RING_INFINITY             (10000)


/*
 * pork 20101201 added:
 * the SLIC SPI ID was indicated to which SPI device num was assigned to SLIC (the real device num would be TDI_SLIC_SPI_ID+devcnt)
 * */
#define _TDI_SLIC_SPI_ID     0
/*
 *  Workaround for VE890 delayed on-hook event.
 *  Check the 0x4D Signaling Register directly to determine on-hook event.
 */
#define _TDI_VP890_SIGREG_LIU         (0x0100)         /*  'Line in Use' bit  */

/*
 * FXO hook flash time
 */
#define _TDI_HOOK_FLASH_TIME           (50)




typedef int (*slicFuncTbl)(char *buf);

/*******Definition over**************/

/*****Enum definition****/
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
	C_US,		/*U.S.*/
	C_ZA,		/*S.Africa*/
	C_VN,		/*Vietnam*/
	C_RU,		/*Russia*/
	C_RSV1, 	/*Reserved Country #1*/
	C_RSV2, 	/*Reserved Country #2*/
	C_RSV3, 	/*Reserved Country #3*/
	C_NUM

}slicHwProf_e;

/*****Data structure*****/

typedef enum {
	CODEC_LINEAR,
	CODEC_ALAW,
	CODEC_ULAW
}_TDI_ESlicCodec;

typedef struct {

	char 		fxsOnHook;
	char 		isHookStateChange;	/* Indicating whether the hook state has changed */
	char 		onHookCount;
	char 		hookStateArray;

	char 		fxsFlash;
	char 		flashCheck;

	char 		polarityReversal;
	char 		fxoRingStart;
	char 		fxoRingEnd;

} _TDI_telephonyEventBuffer_t;

typedef struct _TDI_initSetting_s{
	unsigned char chan;
	int hwProfId;
	_TDI_ESlicCodec slicCodecOption;
	int listenSlicVal;
	int speakSlicVal;

} _TDI_initSetting_t;

typedef struct _TDI_ringSetting_s{
	unsigned char chan;
    int state;
    int count;
    int numCads;
    int make1s;
    int make2s;
    int make3s;
    int break1s;
    int break2s;
    int break3s;
    int rep;
    int numRings;
} _TDI_ringSetting_t;

typedef struct _TDI_gainSetting_s{
	unsigned char chan;
    int speakSlicVal;
    int listenSlicVal;
} _TDI_gainSetting_t;

typedef struct _TDI_lineInfo_s{
    unsigned char chan;
    unsigned char state;
    unsigned char polarity;
    int retTicStat;
} _TDI_lineInfo_t;

typedef struct _TDI_batteryPolInfo_s{
	unsigned char chan;
	unsigned char polarity;
	int retTicStat;
} _TDI_batteryPolInfo_t;

typedef struct _TDI_hookStat_s{
	unsigned char chan;
	int hookStat;
} _TDI_hookStat_t;

typedef struct _TDI_fxoLineStat_s{
	unsigned char chan;
	int lc;
	int lv;
} _TDI_fxoLineStat_t;

typedef struct _TDI_fxoMonitorStat_s{
	unsigned char chan;
	int mode;
} _TDI_fxoMonitorStat_t;

typedef struct hwProfList_s{
	char name[_TDI_HW_PROF_STR_LEN];
	slicHwProf_e code;
	unsigned char *pDevice;
	unsigned char *pAcFXS;
	unsigned char *pDcFXS;
	unsigned char *pRingingFXS;
	unsigned char *pAcFXO;
	unsigned char *pConfigFXO;
	unsigned char *pCallID;
	unsigned char *pMetering; //dyma_20140428 
}hwProfList_t;



typedef struct deviceInfo_s{
	int deviceNum;
	int fxsNum;
	int fxoNum;
	int slicType;
	int pcm1ChNum;
	int pcm2ChNum;
}deviceInfo_t;



/* Modified by mtk07059_20160302, this enum should be matched with fxs_slicType_read and convertSlicType. */
typedef enum {
	SLICTYPE_INVALID,
	ZARLINK_88601 = 1,
	ZARLINK_89156,
	ZARLINK_89116,
	ZARLINK_89316,
	ZARLINK_9641,
	ZARLINK_9642,

	SILICON_32176,
	SILICON_32260,
	SILICON_3218X,
	SILICON_3228X,
	
	SLICTYPE_MAX,
}slicTypeInfo;

typedef enum {
	DEVICE_NUM,
	FXS_NUM,
	FXO_NUM,
	TOTALCH_NUM,
	SLIC_TYPE,
	PCM1_CH_NUM,
	PCM2_CH_NUM,
}infoSelection;


//Roger_lt
typedef struct _TDI_devicelinetest_s{
	int chan;
	int testid;
}_TDI_devicelinetest_t;

//mtk08176_20150508
typedef enum {
	_TDI_INFC0,
	_TDI_INFC1,
	_TDI_INFC2,
	_TDI_INFC3,
	_TDI_INFC4,
	_TDI_INFC5,
	_TDI_INFC6,
	_TDI_INFC7
} _TDI_infcId_e;

#ifndef UINT8
#define UINT8
typedef unsigned char           uint8;
#endif

typedef struct {
	_TDI_infcId_e infc;
	uint8 lineTestId;
	char lineTestData[5000];
}_TDI_infcLineTestParams_t;
//mtk08176_20150508

typedef struct {
	int testID;
	int size;
}_TDI_infcLineTestSizeList_t;
//mtk08176_20150508


//dyma_20140428 metering
typedef struct _TDI_fxsSetMetering_s{
    int chan;
    unsigned short onTime;
    unsigned short offTime;
    unsigned short numMeters;
}_TDI_fxsSetMetering_t;

/*[OSBNB00041138] ADD by peter.jiang@20141126, support dsp loopback*/
typedef struct _TDI_fxsSetLoopback_s{
	int chan;
	int enable;
}_TDI_fxsSetLoopback_t;

/*[OSBNB00045371] ADD by mtk07059_20150423 , support ring parameters set. */
typedef struct _TDI_fxsSetRingParams_s{
	int chanId;
	int type;
	int frequency;
	int amplitude;
	int dcBias;
	int ringTripThreshold;	 /* added by mtk08176_20150605  */
	int amplitudeSlab;		/* added by mtk08176_20150627  */
}_TDI_fxsSetRingParams_s;

/*[OSBNB00047036] ADD by mtk07059_20150616 , support dc feed parameters set. */
typedef struct _TDI_fxsSetDcFeedParams_s{
	int chanId;
	int ila;
	int ilaSlab;	/* added by mtk08176_20150627  */
}_TDI_fxsSetDcFeedParams_s;

/*****Data structure*****/


int _TDI_deviceInit(_TDI_initSetting_t* pInitSetting);//int _TIC_initHw(void)

void _TDI_paramReset(int country);// part of init

int _TDI_phoneRingStart(int channel);

int _TDI_phoneRingStop(int channel);

int _TDI_eventProcess(unsigned char chan);//void _TIC_legerityRun(vint   lineCnt)

void _TDI_dumpregister(void);

/*=======New function======*/

void _TDI_shutdownHw(int chan);

void _TDI_fxsSetLineState(_TDI_lineInfo_t* pLineInfo);

int _TDI_fxsGetLineState(_TDI_lineInfo_t* pLineInfo);


int _TDI_fxsGetBatteryPol(_TDI_batteryPolInfo_t* pBatteryPolInfo);

void _TDI_fxsSetBatteryPol(_TDI_batteryPolInfo_t* pBatteryPolInfo);

int _TDI_fxsGetHookState(_TDI_hookStat_t* pHookStat);

int _TDI_ringState(_TDI_ringSetting_t* pRingSetting);

int _TDI_fxsGetPulseStatus(unsigned char chan);

/*=====New function over=======*/

/*FXO function*/
int _TDI_fxoGetRingCadence(unsigned char chan);
int _TDI_fxoGetHookState(unsigned char chan);
void _TDI_fxoSetHookState(_TDI_hookStat_t* pHookStat);
void _TDI_fxoGetLineStats(_TDI_fxoLineStat_t* pFxoLineStat);

int _TDI_fxoGetPSTNStats(unsigned char chan);
void _TDI_fxoSetMonitorMode(_TDI_fxoMonitorStat_t* pMonitorStat);

void _TDI_fxoGetParallelPhoneStatStart(void);
void _TDI_fxoGetParallelPhoneStatEnd(void);
int _TDI_fxoGetParallelPhoneStat(void);

int _TDI_getDeviceInfo(int selection);



#endif

