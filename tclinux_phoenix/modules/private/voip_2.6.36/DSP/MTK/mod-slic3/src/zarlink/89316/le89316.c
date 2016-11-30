/*
 * le89316.c
 *
 *  Created on: Nov 30, 2010
 *      Author: pork
 */
/*
 * le89316drv.c
 *
 *  Created on: Nov 11, 2010
 *      Author: pork
 */

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <vp_api.h>
#include <VE890_ACA_Rev2_5profiles8.h>
#include <vp_api_cfg.h>
#include <linux/jiffies.h>     //dyma_140619
#include "vp890_api.h"
#include "vp890_api_int.h"
#include "_TDI.h"
#include "TDI.h"
#include "le89316.h"
#include "ddr_slic3.h" //dyma_130823


/************************************************************************
 *				GLOBE VARIABLES
 *************************************************************************
 */
extern slicFuncTbl* slicFunc[];
static VpDevCtxType            devCtx_ptr;
static Vp890DeviceObjectType   devObj_ptr;
static VpLineCtxType           lineCtx_ptr[LE89316_CHAN_NUM];
static Vp890LineObjectType     lineObj_ptr[LE89316_CHAN_NUM];
VpEventType   legerityEventObj;
le89316LineStat_t le89316LineStat;
int le89316_Init_Cmp = _TDI_FALSE;
int le89316_Init_Line_Cmp = _TDI_FALSE;
int le89316_Get_Mask_Cmp = _TDI_FALSE;
int le89316_Line_Cal_Cmp[LE89316_CHAN_NUM]= {_TDI_FALSE};
static atomic_t le89316_init_ongoing=ATOMIC_INIT(0);

//dyma_130306
#ifdef __KERNEL__      
#define PRINT					printk
#define MALLOC(size)			kmalloc(size, GFP_ATOMIC)
#define FREE(ptr)				kfree(ptr)
#else
#define PRINT					printf
#define MALLOC(size)			malloc(size)
#define FREE(ptr)				free(ptr)
#endif /*__KERNEL__*/

/*20110714 PTChen add for slic country*/
VpProfileDataType *DEVICE_PRO_le89316;
VpProfileDataType *AC_PRO_FXS_le89316;
VpProfileDataType *DC_PRO_FXS_le89316;
VpProfileDataType *RINGING_PRO_FXS_le89316;
VpProfileDataType *AC_PRO_FXO_le89316;
VpProfileDataType *CONFIG_PRO_FXO_le89316;
VpProfileDataType *RINGING_CADENCE_PRO_le89316;
VpProfileDataType *CALL_ID_PRO_le89316;



/* FXS on device 0, FXO on device 0 */
#define FXS_LINE    0
#define FXO_LINE    1

slicFuncTbl le89316Func[]={
		le89316_deviceVerify,
		le89316_deviceInit,
		le89316_shutdownHw,
		le89316_paramReset,
		le89316_phoneRingStart,
		le89316_phoneRingStop,
		le89316_eventProcess,
		le89316_fxsSetLineState,
		le89316_fxsGetLineState,
		le89316_fxsSetBatteryPol,
		le89316_fxsGetBatteryPol,
		le89316_fxsGetHookState,
		le89316_ringState,
		le89316_fxsGetPulseStatus,
		le89316_fxsSetGainLv,
		le89316_fxoGetRingCadence,
		le89316_fxoGetHookState,
		le89316_fxoSetHookState,
		le89316_fxoGetLineStats,
		le89316_fxoGetPSTNStats,
		le89316_fxoSetMonitorMode,
		le89316_dumpregister,
		le89316_fxoGetLIUStats,
		le89316_LtTestStart,//Roger_lt
		le89316_getLtTestResult,//Roger_lt
		slicFunc_dummy,  //dyma_20140428 metering
		le89316_SetLoopback,  //FXS_SET_LOOPBACK
		le89316_SetRingParam,	//mtk07059_20150423
		le89316_SetDcFeedParam,	//mtk07059_20150423
};




/* Termination type: VP_TERM_FXS_GENERIC or VP_TERM_FXS_LOW_PWR */
#define TERM_TYPE   VP_TERM_FXS_GENERIC
#define TERM_TYPE_T "VP_TERM_FXS_GENERIC"


char *vpStatusString[] = {
    "VP_STATUS_SUCCESS",
    "VP_STATUS_FAILURE",
    "VP_STATUS_FUNC_NOT_SUPPORTED",
    "VP_STATUS_INVALID_ARG",
    "VP_STATUS_MAILBOX_BUSY",
    "VP_STATUS_ERR_VTD_CODE",
    "VP_STATUS_OPTION_NOT_SUPPORTED",
    "VP_STATUS_ERR_VERIFY",
    "VP_STATUS_DEVICE_BUSY",
    "VP_STATUS_MAILBOX_EMPTY",
    "VP_STATUS_ERR_MAILBOX_DATA",
    "VP_STATUS_ERR_HBI",
    "VP_STATUS_ERR_IMAGE",
    "VP_STATUS_IN_CRTCL_SECTN",
    "VP_STATUS_DEV_NOT_INITIALIZED",
    "VP_STATUS_ERR_PROFILE",
    "VP_STATUS_INVALID_VOICE_STREAM"
};


#ifdef OSBN_VOIP
int test = 0x61;
int testCh = 0;
int16 testMpiData;
int8 testWMpiData = 0xbf;
int8 testRMpiData = 0;

/*20110714 PTChen add for slic country*/ //dyma_20140428 
hwProfList_t le89316_country_mapping[] =
{
	{"Default"    	  , C_DEF, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF	, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_600R_DEF	, FXO_DIALING_US 	, NULL, NULL },
	{"Australia"  	  ,  C_AU, DEV_PROFILE_BuckBoost, AC_FXS_RF50_AU 		, DC_FXS_DEF, RING_25HZ_DEF	, AC_FXO_LC_AU			, FXO_DIALING_AU 	, NULL, NULL },
	{"Austria"   	  ,  C_AT, DEV_PROFILE_BuckBoost, AC_FXS_RF50_AT 		, DC_FXS_DEF, RING_AT  		, AC_FXO_LC_AT			, FXO_DIALING_AT 	, NULL, NULL },
	{"BEL-Belgium"    ,  C_BE, DEV_PROFILE_BuckBoost, AC_FXS_RF50_BE 		, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_BE			, FXO_DIALING_BE 	, NULL, NULL },
	{"BRA-Brazil"     ,  C_BR, DEV_PROFILE_BuckBoost, AC_FXS_RF50_BR 		, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_600R_DEF	, FXO_DIALING_DEF	, NULL, NULL },
	{"Canada"    	  ,  C_CA, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF	, DC_FXS_DEF, RING_CA  		, AC_FXO_LC_600R_DEF	, FXO_DIALING_CA 	, NULL, NULL },
	{"CHN-China"      ,  C_CN, DEV_PROFILE_BuckBoost, AC_FXS_RF50_CN 		, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_CN			, FXO_DIALING_CN 	, NULL, NULL },
	{"Czech"       	  ,  C_CZ, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF	, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_600R_DEF	, FXO_DIALING_DEF	, NULL, NULL },
	{"DNK-Denmark"    ,  C_DK, DEV_PROFILE_BuckBoost, AC_FXS_RF50_DK 		, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_DK			, FXO_DIALING_DK 	, NULL, NULL },
	{"ETS-ETSI"       ,  C_EU, DEV_PROFILE_BuckBoost, AC_FXS_RF50_ETSI 		, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_ETSI		, FXO_DIALING_ETSI	, NULL, NULL },
	{"FIN-Finland"    ,  C_FI, DEV_PROFILE_BuckBoost, AC_FXS_RF50_FI 		, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_FI			, FXO_DIALING_FI 	, NULL, NULL },
	{"FRA-France"     ,  C_FR, DEV_PROFILE_BuckBoost, AC_FXS_RF50_FR 		, DC_FXS_DEF, RING_FR  		, AC_FXO_LC_FR			, FXO_DIALING_FR  	, NULL, NULL },
	{"DEU-Germany"    ,  C_DE, DEV_PROFILE_BuckBoost, AC_FXS_RF50_DE 		, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_DE			, FXO_DIALING_DE 	, NULL, NULL },
	{"Greece"     	  ,  C_GR, DEV_PROFILE_BuckBoost, AC_FXS_RF50_GR57 		, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_600R_DEF	, FXO_DIALING_DEF 	, NULL, NULL },
	{"HongKong"  	  ,  C_HK, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF	, DC_FXS_DEF, RING_HK 		 , AC_FXO_LC_HK			, FXO_DIALING_HK 	, NULL, NULL },
	{"HUN-Hungary"    ,  C_HU, DEV_PROFILE_BuckBoost, AC_FXS_RF50_HU 		, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_HU			, FXO_DIALING_HU 	, NULL, NULL },
	{"Ireland"   	  ,  C_IE, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF	, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_IE			, FXO_DIALING_IE 	, NULL, NULL },
	{"Israel"    	  ,  C_IL, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF	, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_600R_DEF	, FXO_DIALING_DEF 	, NULL, NULL },
	{"India"      	  ,  C_IN, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF 	, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_600R_DEF	, FXO_DIALING_DEF 	, NULL, NULL },
	{"ITA-Italy"      ,  C_IT, DEV_PROFILE_BuckBoost, AC_FXS_RF50_IT 		, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_IT			, FXO_DIALING_IT 	, NULL, NULL },
	{"JPN-Japan"      ,  C_JP, DEV_PROFILE_BuckBoost, AC_FXS_RF50_JP 		, DC_FXS_DEF, RING_JP 		 , AC_FXO_LC_JP			, FXO_DIALING_JP	, NULL, NULL },
	{"Singapore" 	  ,  C_SG, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF	, DC_FXS_DEF, RING_SG 		 , AC_FXO_LC_600R_DEF	, FXO_DIALING_SG	, NULL, NULL },
	{"S.Korea"   	  ,  C_KR, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF 	, DC_FXS_DEF, RING_KR  		 , AC_FXO_LC_600R_DEF	, FXO_DIALING_KR 	, NULL, NULL },
	{"Mexico"     	  ,  C_MX, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF 	, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_600R_DEF	, FXO_DIALING_DEF 	, NULL, NULL },
	{"NLD-Netherlands",  C_NL, DEV_PROFILE_BuckBoost, AC_FXS_RF50_NL 		, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_NL			, FXO_DIALING_NL 	, NULL, NULL },
	{"NewZealand" 	  ,  C_NZ, DEV_PROFILE_BuckBoost, AC_FXS_RF50_NZ		, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_NZ			, FXO_DIALING_NZ 	, NULL, NULL },
	{"Norway"     	  ,  C_NO, DEV_PROFILE_BuckBoost, AC_FXS_RF50_NO 		, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_NO			, FXO_DIALING_NO 	, NULL, NULL },
	{"Poland"   	  ,  C_PL, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF	, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_600R_DEF	, FXO_DIALING_DEF 	, NULL, NULL },
	{"Portugal"   	  ,  C_PT, DEV_PROFILE_BuckBoost, AC_FXS_RF50_PT 		, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_PT			, FXO_DIALING_PT 	, NULL, NULL },
	{"Slovenia"   	  ,  C_SI, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF  , DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_600R_DEF	, FXO_DIALING_DEF 	, NULL, NULL },
	{"Taiwan"     	  ,  C_TW, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF	, DC_FXS_DEF, RING_TW  		, AC_FXO_LC_TW			, FXO_DIALING_TW 	, NULL, NULL },
	{"Spain"     	  ,  C_ES, DEV_PROFILE_BuckBoost, AC_FXS_RF50_ES 		, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_ES			, FXO_DIALING_ES 	, NULL, NULL },
	{"SWE-Sweden"     ,  C_SE, DEV_PROFILE_BuckBoost, AC_FXS_RF50_SE 		, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_SE			, FXO_DIALING_SE 	, NULL, NULL },
	{"GBR-Great Britain" ,C_GB, DEV_PROFILE_BuckBoost, AC_FXS_RF50_GB 		, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_GB			, FXO_DIALING_GB 	, NULL, NULL },
	{"USA-United States" ,C_US, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF	, DC_FXS_DEF, RING_US  		, AC_FXO_LC_600R_DEF	, FXO_DIALING_US 	, NULL, NULL },
	{"S.Africa"   	  ,  C_ZA, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF	, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_ZA			, FXO_DIALING_ZA 	, NULL, NULL },
	{"Vietnam"		 ,C_VN, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF	, DC_FXS_DEF, RING_US  		, AC_FXO_LC_600R_DEF	, FXO_DIALING_US 	, NULL, NULL },
	{"RSV1"			  ,C_RSV1, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF	, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_600R_DEF	, FXO_DIALING_US	, NULL, NULL },
	{"RSV2"			  ,C_RSV2, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF	, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_600R_DEF	, FXO_DIALING_US	, NULL, NULL },
	{"RSV3"			  ,C_RSV3, DEV_PROFILE_BuckBoost, AC_FXS_RF50_600R_DEF	, DC_FXS_DEF, RING_25HZ_DEF , AC_FXO_LC_600R_DEF	, FXO_DIALING_US	, NULL, NULL }
};

void le89316_slicParamReset(int idx){
	/*====== decide profile according to Country code =====*/
	/*Reset by default table*/
	DEVICE_PRO_le89316 = DEV_PROFILE_BuckBoost;
	AC_PRO_FXS_le89316 = AC_FXS_RF50_600R_DEF;
	DC_PRO_FXS_le89316 = DC_FXS_DEF;
	RINGING_PRO_FXS_le89316 = RING_25HZ_DEF;
	AC_PRO_FXO_le89316 = AC_FXO_LC_600R_DEF;
	CONFIG_PRO_FXO_le89316 = FXO_DIALING_US;
	CALL_ID_PRO_le89316 = VP_PTABLE_NULL;

	if((idx >= C_DEF) && (idx < C_NUM)){
		DEVICE_PRO_le89316 = le89316_country_mapping[idx].pDevice;
		AC_PRO_FXS_le89316 = le89316_country_mapping[idx].pAcFXS;
		DC_PRO_FXS_le89316 = le89316_country_mapping[idx].pDcFXS;
		RINGING_PRO_FXS_le89316 = le89316_country_mapping[idx].pRingingFXS;
		AC_PRO_FXO_le89316 = le89316_country_mapping[idx].pAcFXO;
		CONFIG_PRO_FXO_le89316 = le89316_country_mapping[idx].pConfigFXO;
		CALL_ID_PRO_le89316 = le89316_country_mapping[idx].pCallID;
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "%s Successful(CountryCode=%s)\n",__func__,\
				le89316_country_mapping[idx].name);
	}
	else{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "%s:err: slicParamReset UNKNOW\n",__func__);
	}
}

static int testCheck_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data){
	int len = 0;

	//VpSetLineState(&(lineCtx_ptr[FXO_LINE]),VP_LINE_FXO_LOOP_OPEN);
	//VpMpiCmdWrapper(0, testCh, test,
	//				VP890_NO_UL_SIGREG_LEN, &testMpiData);
	int16 testdata = 0;
	/*VpMpiCmd(0, 0x1, 0x61,0x1, &testdata);
	VpMpiCmd(0, 0x2, 0x61,0x1, &testdata);*/
	VpMpiCmd(0, 0x1, test,0x2, &testdata);

	printk("data:%x \n",testdata);
	VpMpiCmd(0, 0x2, test,0x2, &testdata);

	printk("data:%x \n",testdata);
	//printk("data:%x \n",mpiData);
	//len += sprintf(page+len, "mpi: %x\n",testMpiData);

	len -= off;
	*start = page + off;

	if (len > count)
		len = count;
	else
		*eof = 1;

	if (len < 0)
		len = 0;

	return len;
}
static int testCheck_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){
	char val_string[40];
	int devIdx;
	int16 tmpValue;
	if (count > sizeof(val_string) - 1)
    		return -EINVAL;
	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;
	val_string[count] = '\0';
	sscanf(val_string,"%x %d %hd\n",&test,&testCh,&testMpiData);
	printk("-->write %d ch:%d mipdata:%hd\n",test,testCh,testMpiData);

	return count;
}
#endif
/************************************************************************
 *				FUNCTION DECLARATION
 *************************************************************************
 */
/*_____________________________________________________________________________
 **      function name: _TDI_paramReset
 **      descriptions:
 **            allocate no side effect K1 buffer and record what had been allocated
 **      parameters:
 **				size: buffer size
 **				gfpFlag: used to indicate GFP flags
 **				type: what have been allocated
 **      global:
 **
 **      return:
 **	      buffer address
 **      call:
 **      revision:
 **      		1.1 2008/08/13 18:00  pork
 **      		1.2 2008/08/14 18:00  pork
 **____________________________________________________________________________
 */
int le89316_paramReset(char* buf){
//	int country = *(int *)buf;
	return _TDI_SUCCESS;
}

/*_____________________________________________________________________________
 **      function name: le89316_deviceVerify
 **      descriptions:
 **            driver verify
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	retVal
 **      call:
 **      revision:
 **____________________________________________________________________________
 */
int le89316_deviceVerify(char* buf){
    VpStatusType          status;
	deviceInfo_t* devInfo = (deviceInfo_t*)buf;
	uint8	res_arr[2] = {0};

    /* Initialize the Device */
    status = VpMakeDeviceObject(VP_DEV_890_SERIES,devInfo->deviceNum, &devCtx_ptr, &devObj_ptr);
    if (VP_STATUS_SUCCESS != status) {
        dbg_printf("Error making the device object 0, Status: %s\n", vpStatusString[status]);
        return _TDI_FAIL;
    }
	
	//VpMpiCmd(devInfo->deviceNum, VP890_EC_CH1, VP890_DEVTYPE_CMD, 0x02, res_arr);  //dyma_130118
	VpMpiCmd(devInfo->deviceNum, VP890_EC_CH1, VP890_DEVTYPE_RD, 0x02, res_arr); 
	if(res_arr[0] != VP890_DEVTYPE_LEN || res_arr[1] != VP890_DEV_PCN_89316){
		dbg_printf("devInfo->deviceNum=%d res_arr[0]=%x res_arr[0]=%x\n",devInfo->deviceNum,res_arr[0],res_arr[1]);
		return _TDI_FAIL;
	}
	
	devInfo->deviceNum += 1;
	devInfo->fxsNum += LE89316_FXS_NUM;
	devInfo->fxoNum += LE89316_FXO_NUM;
	devInfo->slicType = ZARLINK_89316;
	devInfo->pcm1ChNum += LE89316_FXS_NUM+LE89316_FXO_NUM;
	devInfo->pcm2ChNum += 0;

	return _TDI_SUCCESS;
}

/*_____________________________________________________________________________
 **      function name: le89316_deviceInit
 **      descriptions:
 **            driver init
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	retVal
 **      call:
 **      revision:
 **      		1.1 2010/11/10 17:00  pork
 **____________________________________________________________________________
 */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31)
extern void VpSysSemaphoreInit(int semaphoredId);
extern void  slicParamReset(void);
#endif
int le89316_deviceInit(char* buf){
	int                  devCnt;
//	int                  lineCnt;
    VpStatusType          status;
    VpOptionCodecType     codecOption;
    VpOptionPulseModeType pulseMode;
    VpOptionTimeslotType  timeSlots;
    VpOptionEventMaskType eventMask;
    //VpEventType   legerityEventObj;
    int timeSlotIdx;

    _TDI_initSetting_t* pinitSetting = ( _TDI_initSetting_t*)buf;
    devCnt = pinitSetting->chan;
    timeSlotIdx = devCnt;
    SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "% devCnt:%d \n",devCnt);


	le89316_Init_Line_Cmp	 = _TDI_FALSE;
	le89316_Init_Cmp = _TDI_FALSE;
	le89316_Line_Cal_Cmp[0]= _TDI_FALSE;
	le89316_Line_Cal_Cmp[1]= _TDI_FALSE;
	atomic_set(&le89316_init_ongoing,1);

	VpSysSemaphoreInit(0);


    /* Initialize the Device */
    status = VpMakeDeviceObject(VP_DEV_890_SERIES,devCnt, &devCtx_ptr, &devObj_ptr);
    if (VP_STATUS_SUCCESS != status) {
        dbg_printf("Error making the device object 0, Status: %s\n", vpStatusString[status]);
		atomic_set(&le89316_init_ongoing,0);
        return _TDI_FAIL;
    }

    /* VP_TERM_FXS_GENERIC / VP_TERM_FXS_LOW_PWR / VP_TERM_FXS_SPLITTER_LP */
    status = VpMakeLineObject(VP_TERM_FXS_GENERIC, FXS_LINE, &(lineCtx_ptr[FXS_LINE]), &(lineObj_ptr[FXS_LINE]), &devCtx_ptr);
    if (VP_STATUS_SUCCESS != status) {
    	dbg_printf("Error making the FXS line object 0, Status: %s\n", vpStatusString[status]);
		atomic_set(&le89316_init_ongoing,0);
    	return _TDI_FAIL;
    }

//#ifdef OSBN_VOIP
#if 0
    struct proc_dir_entry *testCheck_proc;
    printk("Create PSTN check proc \n");
    testCheck_proc = create_proc_entry("test_stats", 0, NULL);
    testCheck_proc->read_proc = testCheck_read_proc;
    testCheck_proc->write_proc = testCheck_write_proc;
#endif

    SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "--- Initialize the FXO line (VP_TERM_FXO_GENERIC) ---\n");
    status = VpMakeLineObject(VP_TERM_FXO_GENERIC, FXO_LINE, &(lineCtx_ptr[FXO_LINE]), &(lineObj_ptr[FXO_LINE]), &devCtx_ptr);
    if (VP_STATUS_SUCCESS != status) {
    	dbg_printf("Error making the FXO line object 1, Status: %s\n", vpStatusString[status]);
		atomic_set(&le89316_init_ongoing,0);
    	return _TDI_FAIL;
    }
    /*20110714 PTChen modified for slic country*/
    le89316_slicParamReset(pinitSetting->hwProfId);
    status = VpInitDevice(&devCtx_ptr , DEVICE_PRO_le89316, AC_PRO_FXS_le89316, \
    		DC_PRO_FXS_le89316, RINGING_PRO_FXS_le89316, AC_PRO_FXO_le89316,\
    		CONFIG_PRO_FXO_le89316);
    if (VP_STATUS_SUCCESS != status) {
    	dbg_printf("Device 0 not properly initialized, Status: %s\n", vpStatusString[status]);
		atomic_set(&le89316_init_ongoing,0);
    	return _TDI_FAIL;
    }
    while (le89316_Init_Cmp != TRUE) {
		mdelay(10);
		le89316_eventProcess((char *)&devCnt);
    }
#if 0
    le89316_Init_Cmp = FALSE;
    status = VpInitLine(&(lineCtx_ptr[FXS_LINE]),AC_ETSI_2,DC_FXS_DEF,RING_US);
    if (VP_STATUS_SUCCESS != status) {
    	dbg_printf("FXS Init line failed :%d\n", status);
    	return _TDI_FAIL;
    }


    status = VpInitLine(&(lineCtx_ptr[FXO_LINE]),AC_FXO_LC_600R_DEF,FXO_DIALING_US,VP_PTABLE_NULL);
    if (VP_STATUS_SUCCESS != status) {
    	dbg_printf("Init line failed :%d\n", status);
    	return _TDI_FAIL;
    }
    while (le89316_Init_Line_Cmp != TRUE) {

		mdelay(10);
		le89316_eventProcess((char *)&devCnt);
    }
#endif

    le89316_Init_Cmp = FALSE;
    //status = VpInitLine(&(lineCtx_ptr[FXO_LINE]),AC_FXO_LC_600R_DEF,FXO_DIALING_US,VP_PTABLE_NULL);
    status = VpInitLine(&(lineCtx_ptr[FXO_LINE]),AC_PRO_FXO_le89316,\
    		CONFIG_PRO_FXO_le89316,VP_PTABLE_NULL);
    
    if (VP_STATUS_SUCCESS != status) {
    	dbg_printf("Init line failed :%d\n", status);
		atomic_set(&le89316_init_ongoing,0);
    	return _TDI_FAIL;
    }
    while (le89316_Init_Line_Cmp != TRUE) {

		mdelay(10);
		le89316_eventProcess((char *)&devCnt);
    }

    status = VpSetLineState(&(lineCtx_ptr[FXO_LINE]), VP_LINE_FXO_OHT);
	if ( status != VP_STATUS_SUCCESS ){
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "Le89010: VpSetLineState failed (%d) \n", status);
		atomic_set(&le89316_init_ongoing,0);
		return _TDI_FAIL;
	}
    status = VpGetOption(&(lineCtx_ptr[FXO_LINE]), VP_NULL, VP_OPTION_ID_EVENT_MASK, 10);
	if ( status != VP_STATUS_SUCCESS )
	{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "Le89010: VpGetOption failed (%d) \n", status);
		atomic_set(&le89316_init_ongoing,0);
		return _TDI_FAIL;
	}
    while (le89316_Get_Mask_Cmp != TRUE) {
    	//event_printf(">>1.5\n");
        /*
         * Wait for event indicating that the initialization is complete
         */
		mdelay(10);
		le89316_eventProcess((char *)&devCnt);  /* Only run for one line per device */
    }



	status = VpGetResults(&legerityEventObj, (void*)&eventMask);
	if (status != VP_STATUS_SUCCESS) {
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "Le89010: VpGetResults failed (%d) \n", status);
		atomic_set(&le89316_init_ongoing,0);
		return _TDI_FAIL;
	}

    /*
     * Set event mask for Legerity device.
     * Configure to detect ONHOOK and OFFHOOK only. TIC software will debounce
     * these signals, and detect FLASH. In this way, TIC has access to the
     * immediate state of the line, as required by the Caller ID Send (CIDS)
     * module.
     */
    /*
     * XXX This should be done by using VpGetOption() to unmask only the
     * options used by this application.
     * For an example, see qs_le71he0824_dat.c in
     * C:\Zarlink\VoicePath API-II 2.13.0\apps\qs_le71he0824_dat
     */
    SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, ">>3\n");
    eventMask.signaling = 0xffff;
    eventMask.signaling &= ~(VP_LINE_EVID_HOOK_OFF | VP_LINE_EVID_HOOK_ON);	/*SERENA_MODIFY*/

	eventMask.response &= ~(VP_EVID_CAL_CMP);
	eventMask.process &= ~(VP_LINE_EVID_SIGNAL_CMP);
	eventMask.fxo &= ~(VP_LINE_EVID_RING_ON | VP_LINE_EVID_RING_OFF);
	eventMask.fxo &= ~(VP_LINE_EVID_DISCONNECT | VP_LINE_EVID_RECONNECT);
	eventMask.fxo &= ~(VP_LINE_EVID_FEED_DIS | VP_LINE_EVID_FEED_EN);
	eventMask.fxo &= ~(VP_LINE_EVID_LIU | VP_LINE_EVID_LNIU);
	eventMask.fxo &= ~VP_LINE_EVID_POLREV;

    if (VP_STATUS_SUCCESS != VpSetOption(VP_NULL, &devCtx_ptr,
            VP_OPTION_ID_EVENT_MASK, &eventMask)) {
    	dbg_printf("%s:%d VpSetOption: FXS_LINE VP_OPTION_ID_EVENT_MASK failed\n",
                __FILE__, __LINE__);
		atomic_set(&le89316_init_ongoing,0);
        return _TDI_FAIL;
    }
	/*20110509 PTChen add for AC, now D2 use CODEC_LINEAR, AC use CODEC_ALAW*/
	if(pinitSetting->slicCodecOption == CODEC_LINEAR){
		dbg_printf("CODEC_LINEAR setting\n");
		codecOption = VP_OPTION_LINEAR;
	}
	else if(pinitSetting->slicCodecOption == CODEC_ALAW){
		dbg_printf("CODEC_ALAW setting\n");
		codecOption = VP_OPTION_ALAW;

	}
	else if(pinitSetting->slicCodecOption == CODEC_ULAW){
		dbg_printf("CODEC_ULAW setting\n");
		codecOption = VP_OPTION_MLAW;

	}


		
		if (VP_STATUS_SUCCESS != VpSetOption(&(lineCtx_ptr[FXS_LINE]),NULL,
					VP_OPTION_ID_CODEC, &codecOption)) {
			dbg_printf("%s:%d Error: failed to set codec type!\n", __FILE__, __LINE__);
			atomic_set(&le89316_init_ongoing,0);
			return _TDI_FAIL;
		}

		if (VP_STATUS_SUCCESS != VpSetOption(&(lineCtx_ptr[FXO_LINE]),NULL,
					VP_OPTION_ID_CODEC, &codecOption)) {
			dbg_printf("%s:%d Error: failed to set codec type!\n", __FILE__, __LINE__);
			atomic_set(&le89316_init_ongoing,0);
			return _TDI_FAIL;
		}
		/*codecOption = VP_OPTION_LINEAR;
		if (VP_STATUS_SUCCESS != VpSetOption(&(lineCtx_ptr[FXO_LINE]), NULL,
					VP_OPTION_ID_CODEC, &codecOption)) {
			dbg_printf("%s:%d Error: failed to set codec type!\n", __FILE__, __LINE__);
			return _TDI_FAIL;
		}*/
	//}
    /*
     * Disable Pulse Dial Detection
     */
   // pulseMode = VP_OPTION_PULSE_DECODE_ON;
   // VpSetOption(VP_NULL, &devCtx_ptr, VP_OPTION_ID_PULSE_MODE, &pulseMode);

    /*
     * Set timeslot line option, and activate lines
     */
	/*20110509 PTChen add for 16 bit linear and 8 bit ALAW ULAW setting*/
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "Le89316 FXS timeslot:%d \n",timeSlotIdx); //dyma_130823
	if(pinitSetting->slicCodecOption == CODEC_LINEAR){
	timeSlots.tx = ((timeSlotIdx) << 1);
	timeSlots.rx = ((timeSlotIdx) << 1);
	}
	else{
		timeSlots.tx = ((timeSlotIdx));
		timeSlots.rx = ((timeSlotIdx));
	}
	VpSetOption(&(lineCtx_ptr[FXS_LINE]), VP_NULL, VP_OPTION_ID_TIMESLOT,&timeSlots);

	//timeSlotIdx = _TDI_INFC_NUM - 1;
	timeSlotIdx = SLIC_getDeviceInfo(TOTALCH_NUM)-1;  //dyma_130823
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "Le89316 FXO timeslot:%d \n",timeSlotIdx); //dyma_130823
	if(pinitSetting->slicCodecOption == CODEC_LINEAR){
	timeSlots.tx = ((timeSlotIdx) << 1);
	timeSlots.rx = ((timeSlotIdx) << 1);
	}
	else{
		timeSlots.tx = ((timeSlotIdx));
		timeSlots.rx = ((timeSlotIdx));
	}
	VpSetOption(&(lineCtx_ptr[FXO_LINE]), VP_NULL, VP_OPTION_ID_TIMESLOT,&timeSlots);
#if 0
#if _TDI_FXO_NUM > 0
	VpSetLineState(&(lineCtx_ptr[FXO_LINE]), VP_LINE_FXO_LOOP_OPEN);
    pulseMode = VP_OPTION_PULSE_DECODE_OFF;
    status = VpSetOption(VP_NULL, &devCtx_ptr, VP_OPTION_ID_PULSE_MODE,&pulseMode);
#endif
#endif

	/*
	 * Adjust Tx/Rx relative gain levels.
	 */
   // VpSetLineState(&lineCtx_ptr[FXS_LINE], VP_LINE_ACTIVE);/*for COC by jq.zhu , OSBNB00030567, 20130327*/
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "gain--TX :%x RX: %x \n",pinitSetting->speakSlicVal,pinitSetting->listenSlicVal);
	VpSetRelGain(&(lineCtx_ptr[FXS_LINE]),pinitSetting->speakSlicVal, pinitSetting->listenSlicVal, 0);
	//VpSetRelGain(&(lineCtx_ptr[FXO_LINE]), 0x2013, 0x2013, 0);

	/*
	 * Calibrate Lines
	 */
	/*le89116_Line_Cal_Cmp[devCnt] = FALSE;
	VpCalLine(&lineCtx_ptr[devCnt]);
	while (le89116_Line_Cal_Cmp[devCnt] != TRUE) {
		 * Wait for event indicating that calibration is complete
		mdelay(10);
		le89116_eventProcess((char *)&devCnt);
	}*/
    SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, ">>5\n");
    /*
     * Init the hook state for each FXS channel
     */
    le89316LineStat.fxsHookStat = TIC_IMMEDIATE_ONHOOK;
    le89316LineStat.fxoLineConn = FXO_LINE_DISABLED;
	atomic_set(&le89316_init_ongoing,0);
    return _TDI_SUCCESS;
}

/*_____________________________________________________________________________
 **      function name: _TDI_shutdownHw
 **      descriptions:
 **            driver shutdown
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	void
 **      call:
 **      revision:
 **      		1.1 2010/11/10 17:00  pork
 **____________________________________________________________________________
 */
int le89316_shutdownHw(char* buf){
    int    lineCnt;
	if(atomic_read(&le89316_init_ongoing))
	{
			return _TDI_FAIL;
	}
    dbg_printf("%s",__func__);
	for(lineCnt = 0; lineCnt < LE89316_CHAN_NUM;lineCnt++){
		VpSetLineState(&lineCtx_ptr[lineCnt], VP_LINE_DISCONNECT);
		VpFreeLineCtx(&lineCtx_ptr[lineCnt]);
    }
	le89316_Get_Mask_Cmp = _TDI_FALSE;
	le89316_Init_Line_Cmp= _TDI_FALSE;
    return _TDI_SUCCESS;
}
/*_____________________________________________________________________________
 **      function name: le89316_fxsSetLineState
 **      descriptions:
 **            set ring state
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	void
 **      call:
 **      revision:
 **      		1.1 2010/11/10 17:00  pork
 **____________________________________________________________________________
 */
int le89316_fxsSetLineState(char* buf){
    uint8 stateType = 0;
    _TDI_lineInfo_t* plineInfo = (_TDI_lineInfo_t*)buf;
    /*
     * Map to Legerity State Types
     */
	if(atomic_read(&le89316_init_ongoing))
	{
			return _TDI_FAIL;
	}
    //dbg_printf("%s",__func__);
    switch (plineInfo->state) {
       /*
        * Codec deactivate
        */
        case TIC_STATE_POWER_DOWN:
            stateType = VP_LINE_DISCONNECT;
            break;
       /*
        * Codec activate or ring stop.
        */
        case TIC_STATE_ACTIVE:
            /* fall through */
        case TIC_STATE_RING_PAUSE:
            stateType = VP_LINE_TALK;
            break;
        case TIC_STATE_REV_OHT:
            stateType = VP_LINE_OHT_POLREV;
            break;
        case TIC_STATE_OHT:
			stateType = VP_LINE_OHT;
			break;
       /*
        * Start Ring
        */
        case TIC_STATE_RINGING:
            stateType = VP_LINE_RINGING;
            break;
	/*add for COC by mtk69090 OSBNB00030711*/
	case TIC_STATE_STAND_BY:
		stateType = VP_LINE_STANDBY;
		break;
	/*end of add for COC by mtk69090*/
        default:
            event_printf("%s:%d error, bad state! %d\n",
                    __FILE__, __LINE__,plineInfo->state);
            return _TDI_FAIL;
    }
    /*
     * Set Line State
     */
    //dbg_printf("---ch:%d status: %d \n",plineInfo->chan,stateType);

    VpSetLineState(&(lineCtx_ptr[FXS_LINE]), stateType);
    return _TDI_SUCCESS;
}

/*_____________________________________________________________________________
 **      function name: le89316_fxsGetLineState
 **      descriptions:
 **            set ring state
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	tic state
 **      call:
 **      revision:
 **      		1.1 2010/11/10 17:00  pork
 **____________________________________________________________________________
 */
int le89316_fxsGetLineState(char* buf){
    VpLineStateType  state;
    _TDI_lineInfo_t* plineInfo = (_TDI_lineInfo_t*)buf;
	if(atomic_read(&le89316_init_ongoing))
	{
		plineInfo->retTicStat = TIC_STATUS_ERROR;
		return _TDI_FAIL;
	}
    VpGetLineState(&(lineCtx_ptr[FXS_LINE]), &state);
    event_printf("%s - %d \n",__func__,state);
    switch (state) {
        case VP_LINE_DISCONNECT:
        	plineInfo->retTicStat = TIC_STATE_POWER_DOWN;
        	break;

        case VP_LINE_STANDBY:
            /* fall through */
        case VP_LINE_STANDBY_POLREV:
            /* fall through */
        case VP_LINE_ACTIVE:
            /* fall through */
        case VP_LINE_ACTIVE_POLREV:
            /* fall through */
        case VP_LINE_TALK:
            /* fall through */
        case VP_LINE_TALK_POLREV:
            /* fall through */
        case VP_LINE_OHT:
        	plineInfo->retTicStat = TIC_STATE_OHT;
        	break;
            /* fall through */
        case VP_LINE_OHT_POLREV:
        	plineInfo->retTicStat = TIC_STATE_REV_OHT;
        	break;

        case VP_LINE_RINGING:
            /* fall through */
        case VP_LINE_RINGING_POLREV:
        	plineInfo->retTicStat = TIC_STATE_RINGING;
        	break;

        case VP_LINE_TIP_OPEN:
        	plineInfo->retTicStat = TIC_STATE_GROUND_START;
            break;
        default:
        	plineInfo->retTicStat = TIC_STATUS_ERROR;
        	break;
    }
    return _TDI_SUCCESS;
}

/*_____________________________________________________________________________
 **      function name: le89316_fxsGetBatteryPol
 **      descriptions:
 **            set ring state
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	tic state
 **      call:
 **      revision:
 **      		1.1 2010/11/10 17:00  pork
 **____________________________________________________________________________
 */
int le89316_fxsGetBatteryPol(char* buf){
    VpLineStateType  polarity;
    VpStatusType     status;
    _TDI_batteryPolInfo_t* pBatteryPolInfo = (_TDI_batteryPolInfo_t* )buf;
	if(atomic_read(&le89316_init_ongoing))
	{
		pBatteryPolInfo->retTicStat = TIC_STATUS_ERROR;
		return _TDI_FAIL;
	}
    event_printf("%s",__func__);
    status = VpGetLineState(&(lineCtx_ptr[FXS_LINE]), &polarity);
    if (VP_STATUS_SUCCESS != status) {
        dbg_printf("%s:%d Error!\n", __FILE__, __LINE__);
        return (-1);
    }

    switch (polarity) {
        case VP_LINE_ACTIVE:
            /* fall through */
        case VP_LINE_OHT:
            /* fall through */
        case VP_LINE_TALK:
            /* fall through */
        case VP_LINE_STANDBY:
            /* fall through */
        case VP_LINE_RINGING:
        	pBatteryPolInfo->retTicStat = TIC_BATT_POL_FORWARD;
            return _TDI_SUCCESS;
        case VP_LINE_ACTIVE_POLREV:
            /* fall through */
        case VP_LINE_OHT_POLREV:
            /* fall through */
        case VP_LINE_TALK_POLREV:
            /* fall through */
        case VP_LINE_STANDBY_POLREV:
            /* fall through */
        case VP_LINE_RINGING_POLREV:
        	pBatteryPolInfo->retTicStat = TIC_BATT_POL_REVERSE;
            return _TDI_SUCCESS;
        default:
            dbg_printf("%s:%d Bad polarity... %d\n",
                    __FILE__, __LINE__, polarity);
            pBatteryPolInfo->retTicStat = TIC_STATUS_ERROR;
            return _TDI_SUCCESS;
    }
}
/*_____________________________________________________________________________
 **      function name: le89316_fxsSetBatteryPol
 **      descriptions:
 **            set battery polarity
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	tic state
 **      call:
 **      revision:
 **      		1.1 2010/11/10 17:00  pork
 **____________________________________________________________________________
 */
int le89316_fxsSetBatteryPol(char* buf){
    VpLineStateType  curState;
    VpStatusType     getStatus;
    VpStatusType     setStatus;
    _TDI_batteryPolInfo_t* pBatteryPolInfo = (_TDI_batteryPolInfo_t* )buf;
	if(atomic_read(&le89316_init_ongoing))
	{
		return _TDI_FAIL;
	}
    event_printf("%s",__func__);
    getStatus = VpGetLineState(&(lineCtx_ptr[FXS_LINE]), &curState);
    if (VP_STATUS_SUCCESS != getStatus) {
        dbg_printf("%s:%d Error!\n", __FILE__, __LINE__);
        return _TDI_FAIL;
    }

    switch (curState) {
        case VP_LINE_ACTIVE:
            /* fall through */
        case VP_LINE_ACTIVE_POLREV:
            if (TIC_BATT_POL_FORWARD == pBatteryPolInfo->polarity) {
                setStatus = VpSetLineState(&(lineCtx_ptr[FXS_LINE]), VP_LINE_ACTIVE);
            }
            else {
                setStatus = VpSetLineState(&(lineCtx_ptr[FXS_LINE]),
                        VP_LINE_ACTIVE_POLREV);
            }
            break;
        case VP_LINE_OHT:
            /* fall through */
        case VP_LINE_OHT_POLREV:
            if (TIC_BATT_POL_FORWARD == pBatteryPolInfo->polarity) {
                setStatus = VpSetLineState(&(lineCtx_ptr[FXS_LINE]), VP_LINE_OHT);
            }
            else {
                setStatus = VpSetLineState(&(lineCtx_ptr[FXS_LINE]),
                        VP_LINE_OHT_POLREV);
            }
            break;
        case VP_LINE_STANDBY:
            /* fall through */
        case VP_LINE_STANDBY_POLREV:
            if (TIC_BATT_POL_FORWARD == pBatteryPolInfo->polarity) {
                setStatus = VpSetLineState(&(lineCtx_ptr[FXS_LINE]), VP_LINE_STANDBY);
            }
            else {
                setStatus = VpSetLineState(&(lineCtx_ptr[FXS_LINE]),
                        VP_LINE_STANDBY_POLREV);
            }
            break;
        case VP_LINE_RINGING:
            /* fall through */
        case VP_LINE_RINGING_POLREV:
            if (TIC_BATT_POL_FORWARD == pBatteryPolInfo->polarity) {
                setStatus = VpSetLineState(&(lineCtx_ptr[FXS_LINE]), VP_LINE_RINGING);
            }
            else {
                setStatus = VpSetLineState(&(lineCtx_ptr[FXS_LINE]),
                        VP_LINE_RINGING_POLREV);
            }
            break;

        case VP_LINE_TALK:
            /* fall through */
        case VP_LINE_TALK_POLREV:
            /* fall through */
        default:
            if (TIC_BATT_POL_FORWARD == pBatteryPolInfo->polarity) {
                setStatus = VpSetLineState(&(lineCtx_ptr[FXS_LINE]), VP_LINE_TALK);
            }
            else {
                setStatus = VpSetLineState(&(lineCtx_ptr[FXS_LINE]),
                        VP_LINE_TALK_POLREV);
            }
            break;
    }
    if (VP_STATUS_SUCCESS != setStatus) {
        dbg_printf("%s:%d Error!\n", __FILE__, __LINE__);
        return _TDI_FAIL;
    }
    return _TDI_SUCCESS;
}

/*_____________________________________________________________________________
 **      function name: le89316_fxsGetHookState
 **      descriptions:
 **            get hook polarity
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	tic state
 **      call:
 **      revision:
 **      		1.1 2010/11/10 17:00  pork
 **____________________________________________________________________________
 */
int le89316_fxsGetHookState(char* buf){
	_TDI_hookStat_t* pHookStat = (_TDI_hookStat_t*)buf;
	if(atomic_read(&le89316_init_ongoing))
	{
		return _TDI_FAIL;
	}
	//dbg_printf("%s",__func__);
	pHookStat->hookStat = le89316LineStat.fxsHookStat;
	return _TDI_SUCCESS;
}

/*_____________________________________________________________________________
 **      function name: le89316_ringState
 **      descriptions:
 **            get ring state
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	ring state
 **      call:
 **      revision:
 **      		1.1 2010/11/10 17:00  pork
 **____________________________________________________________________________
 */
int le89316_ringState(char* buf){
    int state;
    int count;
    int ret;
    _TDI_ringSetting_t *pRingSetting = (_TDI_ringSetting_t *) buf;
	static unsigned long pre_jiffies;

    state = pRingSetting->state;
    count = pRingSetting->count;
    ret   = _TDI_FAIL;
	if(atomic_read(&le89316_init_ongoing))
	{
		return _TDI_FAIL;
	}
    event_printf("%s---%d %d",__func__,state,pRingSetting->count);
    /*
     * Return immediately if done.
     */
    if (state == _TDI_RING_STATE_DONE) {
        return (ret);
    }

LE89_RING_STATE_RUN:

    /*
     * State machine
     */
    switch (state) {

        /*
         * Init state. When new ring starts. Always transitions to make1 state.
         */
        case _TDI_RING_STATE_INIT:
            count = pRingSetting->make1s;
            state = _TDI_RING_STATE_MAK1;
            ret   = _TDI_SUCCESS;
			pre_jiffies=jiffies;   //dyma_140619

        /*
         * Make state always transitions to break state.
         */
        case _TDI_RING_STATE_MAK1:
            /*
             * If time is negative, generate forever
             */
        	//dbg_printf("%s---%d %d",__func__,state,pRingSetting->make1s);
            if (pRingSetting->make1s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            count = pRingSetting->break1s;
            state = _TDI_RING_STATE_BRK1;
            ret   = _TDI_SUCCESS ;

        /*
         * Break state always transitions to make state
         */
        case _TDI_RING_STATE_BRK1:
            /*
             * If time is negative, generate forever
             */
            if (pRingSetting->break1s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            count = pRingSetting->make2s;
            state = _TDI_RING_STATE_MAK2;
            ret   = _TDI_SUCCESS;

        /*
         * Make state always transitions to break state.
         */
        case _TDI_RING_STATE_MAK2:
            /*
             * If time is negative, generate forever
             */
            if (pRingSetting->make2s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            count = pRingSetting->break2s;
            state = _TDI_RING_STATE_BRK2;
            ret   = _TDI_SUCCESS ;

        /*
         * Break state always transitions to make state
         */
        case _TDI_RING_STATE_BRK2:
            /*
             * If time is negative, generate forever
             */
            if (pRingSetting->break2s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            count = pRingSetting->make3s;
            state = _TDI_RING_STATE_MAK3;
            ret   = _TDI_SUCCESS ;

        /*
         * Make state always transitions to break state.
         */
        case _TDI_RING_STATE_MAK3:
            /*
             * If time is negative, generate forever
             */
            if (pRingSetting->make3s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            count = pRingSetting->break3s;
            state = _TDI_RING_STATE_BRK3;
            ret   = _TDI_SUCCESS ;

        /*
         * Break state always transitions to make state
         */
        case _TDI_RING_STATE_BRK3:
            /*
             * If time is negative, generate forever
             */
            if (pRingSetting->break3s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            pRingSetting->numRings++;

            /*
             * Check number of repeats.
             *  <  0 means infinite repeats.
             *  >  0 means repeats not done.
             *  == 0 means repeats done.
             */
            count = pRingSetting->make1s;
            state = _TDI_RING_STATE_MAK1;
            ret   = _TDI_SUCCESS ;
            if (pRingSetting->rep < 0) {
                /*
                 * Run state machine again.
                 */
                goto LE89_RING_STATE_RUN;
            }
            else if (pRingSetting->rep > 0) {
                /*
                 * Run state machine again.
                 */
                pRingSetting->rep--;
                goto LE89_RING_STATE_RUN;
            }

        default:
            count = _TDI_RING_INFINITY;
            state = _TDI_RING_STATE_DONE;
            ret   = _TDI_SUCCESS ;
    }

    count=count-((int)(jiffies-pre_jiffies)>0?((int)(jiffies-pre_jiffies)):1); //dyma_140619
	pre_jiffies=jiffies;   //dyma_140619
    pRingSetting->state = state;
    pRingSetting->count = count;
    event_printf("over: %s---%d %d %d",__func__,state,pRingSetting->count,ret);

    return (ret);

}
int le89316_fxsGetPulseStatus(char* buf){
	if(atomic_read(&le89316_init_ongoing))
	{
		return _TDI_FAIL;
	}
	dbg_printf("%s",__func__);
	return _TDI_SUCCESS;
}

int le89316_fxsSetGainLv(char* buf){
	_TDI_gainSetting_t* gainSetting = (_TDI_gainSetting_t *)buf;
	if(atomic_read(&le89316_init_ongoing))
	{
		return _TDI_FAIL;
	}
	VpSetRelGain(&lineCtx_ptr[FXS_LINE],gainSetting->speakSlicVal,gainSetting->listenSlicVal, 0);
	dbg_printf("%s - %x / %x \n",__func__,gainSetting->speakSlicVal,gainSetting->listenSlicVal);
	return _TDI_SUCCESS;
}
int le89316_phoneRingStart(char* buf){
	if(atomic_read(&le89316_init_ongoing))
	{
		return _TDI_FAIL;
	}
	//int channel = *(int*) buf;
	dbg_printf("%s",__func__);
	return _TDI_SUCCESS;
}

int le89316_phoneRingStop(char* buf){
	if(atomic_read(&le89316_init_ongoing))
	{
		return _TDI_FAIL;
	}
	//int channel = *(int*) buf;
	dbg_printf("%s",__func__);
	return _TDI_SUCCESS;
}

int le89316_eventProcess(char* buf){
    VpDevCtxType *legerityDev_ptr;
    bool          legerityEventFlag = FALSE;
    bool          legerityEventStatus = FALSE;
    //VpEventType   legerityEventObj;
    int          chan;
    uint16        mpiData;      /* Workaround for VE890 delayed on-hook event */
    uint8         ecVal[] =  { VP890_EC_CH1, VP890_EC_CH2 };
    int devCnt = *(int*)buf;

    /*
     * Call the VP API tick
     * and then get status.
     */
    legerityDev_ptr = &devCtx_ptr;
    VpApiTick(legerityDev_ptr, &legerityEventFlag);

    /*
     * If the legerity EventFlag is active, then parse the legerityEventObj
     * for the events of interest to this TIC driver.
     * For now, do some error checking, to help debug problems.
     */
    if (TRUE == legerityEventFlag) {
    	//event_printf("||||%d \n",devCnt);
        legerityEventStatus = VpGetEvent(legerityDev_ptr, &legerityEventObj);
        if ((TRUE == legerityEventStatus) &&
                (VP_STATUS_SUCCESS == legerityEventObj.status)) {
            /*
             * Valid Status received from Legerity,
             * check which line has status, and do some error checking.
             */
            chan = legerityEventObj.channelId;
            /*if (_TDI_FXS_NUM < chan) {
                dbg_printf("%s: %d Legerity line number out of range %d\n",
                        __FILE__, __LINE__, chan);
                return _TDI_FAIL;
            }*/

			//dyma_130823
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"Le89316_Dev[%d]chan[%d]:st0=%d,ed0=%d:Cat=0x%x Id=0x%x\n",
			legerityEventObj.deviceId,
			legerityEventObj.channelId,
			legerityEventObj.status,
			legerityEventObj.eventData,
			legerityEventObj.eventCategory,
			legerityEventObj.eventId); 

            if (VP_EVCAT_RESPONSE == legerityEventObj.eventCategory) {
                switch (legerityEventObj.eventId) {
                    case VP_DEV_EVID_DEV_INIT_CMP:
                    	event_printf("%d set init true\n",devCnt);
                        /* Device Initialization is complete */
                        le89316_Init_Cmp = TRUE;
                        break;
                    case VP_LINE_EVID_RD_OPTION:
                    	event_printf("VP_LINE_EVID_RD_OPTION\n");
                        /* Read the Event Mask */
                        le89316_Get_Mask_Cmp = TRUE;
                        break;
                    case VP_LINE_EVID_LINE_INIT_CMP:
                    	le89316_Init_Line_Cmp = TRUE;
                    	break;
                    case VP_EVID_CAL_CMP:
                        /* Line Calibration is complete */
                    	event_printf("ch %d set init true\n",devCnt);
                        le89316_Line_Cal_Cmp[FXS_LINE] = TRUE;
                        break;
                    case VP_EVID_CAL_BUSY:
                        /* Line Calibration failed */
                        event_printf("%s: %d Legerity line calibration Failed %d\n",
                            __FILE__, __LINE__, chan);
                        break;
                    default:
                        /* Do nothing */
                        break;
                }
            }
            else if (VP_EVCAT_SIGNALING == legerityEventObj.eventCategory) {
                switch (legerityEventObj.eventId) {
                    case VP_LINE_EVID_HOOK_OFF:
                    	event_printf("%d off true\n",devCnt);
                    	le89316LineStat.fxsHookStat = TIC_IMMEDIATE_OFFHOOK;
                        break;
                    case VP_LINE_EVID_HOOK_ON:
                    	event_printf("%d on true\n",devCnt);
                    	le89316LineStat.fxsHookStat = TIC_IMMEDIATE_ONHOOK;
                        break;
                    default:
                        /* Do nothing */
                        break;
                }
            }
            else if(VP_EVCAT_FXO == legerityEventObj.eventCategory){
            	switch (legerityEventObj.eventId) {
                case VP_LINE_EVID_RING_ON:
                	SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG, "Event: VP_LINE_EVID_RING_ON\n");
                	le89316LineStat.fxoRingStat = FXO_RING_ON;
                    break;

                case VP_LINE_EVID_RING_OFF:
                	SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG, "Event: VP_LINE_EVID_RING_OFF\n");
                	le89316LineStat.fxoRingStat = FXO_RING_OFF;
                    break;
                case VP_LINE_EVID_FEED_DIS:
                	if(le89316LineStat.fxoRingStat != FXO_RING_ON){
                		SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG, "Event: VP_LINE_EVID_FEED_DIS\n");
                		le89316LineStat.fxoLineConn = FXO_LINE_DISABLED;
                	}
					break;
				case VP_LINE_EVID_DISCONNECT:
					if(le89316LineStat.fxoRingStat != FXO_RING_ON){
						SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG, "Event: VP_LINE_EVID_DISCONNECT\n");
						le89316LineStat.fxoLineConn = FXO_LINE_DISABLED;
					}
					break;
				case VP_LINE_EVID_FEED_EN:
				/*20110623 PTChen modify, for bug 10695, allow POLREV event*/
				//	if(le89316LineStat.polRev){
						SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG, "Event: VP_LINE_EVID_FEED_EN\n");
						le89316LineStat.fxoLineConn = FXO_LINE_CONNECTED;
				//	}
				  break;

				case VP_LINE_EVID_RECONNECT:
					if(le89316LineStat.polRev){
						SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG, "Event: VP_LINE_EVID_RECONNECT\n");
						le89316LineStat.fxoLineConn = FXO_LINE_CONNECTED;
					}
				  break;

				case VP_LINE_EVID_POH:
					SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG, "Event: VP_LINE_EVID_POH\n");
				  le89316LineStat.fxoHookStat = FXO_OFF_HOOK;
				  break;

				case VP_LINE_EVID_PNOH:
					SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG, "Event: VP_LINE_EVID_PNOH\n");
				  le89316LineStat.fxoHookStat = FXO_ON_HOOK;
				  break;

				case VP_LINE_EVID_POLREV:
					le89316LineStat.polRev = (!le89316LineStat.polRev);
					SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG, "Event: VP_LINE_EVID_POLREV %d (%d)\n",le89316LineStat.polRev,legerityEventObj.eventData);
				  break;
				case VP_LINE_EVID_LIU://Roger_130530
					le89316LineStat.fxoLiuStat=FXO_LINE_LIU;
					SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG, "Event: VP_LINE_EVID_LIU\n");
					break;
				case VP_LINE_EVID_LNIU://Roger_130530					
					le89316LineStat.fxoLiuStat=FXO_LINE_LNIU;
					SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG, "Event: VP_LINE_EVID_LNIU\n");
					break;

				default:
				  dbg_printf("Event: Unhandled FXO event 0x%04X\n",legerityEventObj.eventId);
				  break;
            	}

            }
        }
    }
    /*
     *  Workaround for VE890 delayed on-hook event.
     *  Check the 0x4D Signaling Register directly to determine on-hook event.
     */

    /*VpMpiCmdWrapper(0, 2, 0x60,0x1, 0xbf);
     * VpMpiCmdWrapper(0, 1, 0x60,0x1, 0xbf);*/
	/*VpMpiCmd(devCnt, VP890_EC_CH1, VP890_NO_UL_SIGREG_RD,
					VP890_NO_UL_SIGREG_LEN, &mpiData);
	if (_TDI_VP890_SIGREG_LIU & mpiData ) {     //off-hook state
		if (le89316LineStat.fxsHookStat != TIC_IMMEDIATE_OFFHOOK) {
			le89316LineStat.fxsHookStat  = TIC_IMMEDIATE_OFFHOOK;
		}
		else { }
	} else {                                    // on-hook state
		if (le89316LineStat.fxsHookStat != TIC_IMMEDIATE_ONHOOK) {
			le89316LineStat.fxsHookStat  = TIC_IMMEDIATE_ONHOOK;
		}
	}*/
	/*VpMpiCmdWrapper(0, 0x2, 0x60,0x1, &testRMpiData);
	if(testRMpiData != 0xb7 || testRMpiData != 0xbf){
		testWMpiData = 0xbf;
		VpMpiCmdWrapper(0, 0x1, 0x60,0x1, &testWMpiData);
		testWMpiData = 0xb7;
		VpMpiCmdWrapper(0, 0x2, 0x60,0x1, &testWMpiData);
	}*/
	//VpMpiCmdWrapper(0, 0x3, 0x60,0x1, &testRMpiData);
	//printk("mpi data:%x \n",testRMpiData);
	return _TDI_SUCCESS;
}

int le89316_dumpregister(char* buf){

	#define PRINTK_BUF_SIZE 	  1024	// It is the static buf size in function vprintk() which called by printk() of /linux/kernel/printk.c
	#define DUMP_PRINTK_BUF_NUM   5
	#define DUMP_PRINTK_BUF_SIZE  (PRINTK_BUF_SIZE*DUMP_PRINTK_BUF_NUM)
	uint8 registerBuffer[20];
	char *print_buffer= MALLOC(DUMP_PRINTK_BUF_SIZE);
	int print_buffer_len=0;
	Vp890DeviceObjectType *pDevObj = devCtx_ptr.pDevObj;
	VpDeviceIdType deviceId = pDevObj->deviceId;
	uint8 channelId, ecVal, registerIndex, registerNumber, maxChannelNum;
	
	event_printf("%s",__func__);

	//dyma_130306
	uint8 dumpPrintkBufCnt=0;
	if(atomic_read(&le89316_init_ongoing))
	{
		return _TDI_FAIL;
	}
	/* Sufficient size to hold a single MPI Read is all that is needed. */

	if(print_buffer==NULL){
	  SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "print_buffer alloc fail\r\n");
	  return _TDI_FAIL;
	}
	else{
	  memset(print_buffer,0,DUMP_PRINTK_BUF_SIZE);
	}		

#define VP890_DEVICE_REGISTER_COUNT    18
	uint8 deviceRegs[VP890_DEVICE_REGISTER_COUNT][2] = {
		{VP890_TEST_REG1_RD, VP890_TEST_REG1_LEN},
		{VP890_FUSE_CTRL_REG_RD, VP890_FUSE_CTRL_REG_LEN},
		{VP890_FUSE1_REG_RD, VP890_FUSE1_REG_LEN},
		{VP890_FUSE5_REG_RD, VP890_FUSE5_REG_LEN},
		{VP890_TX_RX_CSLOT_RD, VP890_TX_RX_CSLOT_LEN},
		{VP890_DCR_RD, VP890_DCR_LEN},
		{VP890_OP_MODE_RD, VP890_OP_MODE_LEN},
		{VP890_NO_UL_SIGREG_RD, VP890_NO_UL_SIGREG_LEN},
		{VP890_IODATA_REG_RD, VP890_IODATA_REG_LEN},
		{VP890_IODIR_REG_RD, VP890_IODIR_REG_LEN},
		{VP890_DEV_MODE_RD, VP890_DEV_MODE_LEN},
		{VP890_INT_MASK_RD, VP890_INT_MASK_LEN},
		{VP890_DEVTYPE_RD, VP890_DEVTYPE_LEN},
		{VP890_REV_INFO_RD, VP890_REV_INFO_LEN},
		{VP890_TEST_DATA_RD, VP890_TEST_DATA_LEN},
		{VP890_FUSE6_REG_RD, VP890_FUSE6_REG_LEN},
		{VP890_REGULATOR_TIMING_RD, VP890_REGULATOR_TIMING_LEN},
		{VP890_FUSE7_REG_RD, VP890_FUSE7_REG_LEN}
	};

	char *deviceRegsName[VP890_DEVICE_REGISTER_COUNT] = {
		"VP890_TEST_REG1_RD", "VP890_FUSE_CTRL_REG_RD", "VP890_FUSE1_REG_RD",
		"VP890_FUSE5_REG_RD", "VP890_TX_RX_CSLOT_RD", "VP890_DCR_RD", "VP890_OP_MODE_RD",
		"VP890_NO_UL_SIGREG_RD", "VP890_IODATA_REG_RD", "VP890_IODIR_REG_RD",
		"VP890_DEV_MODE_RD", "VP890_INT_MASK_RD", "VP890_DEVTYPE_RD", "VP890_REV_INFO_RD",
		"VP890_TEST_DATA_RD", "VP890_FUSE6_REG_RD", "VP890_REGULATOR_TIMING_RD",
		"VP890_FUSE7_REG_RD"
	};

	//VP_ERROR(VpDevCtxType, devCtx_ptr,("Device Registers:"));
	print_buffer_len+=sprintf(print_buffer+print_buffer_len,"LE89156Device:");
	ecVal = pDevObj->ecVal;

	for (registerNumber = 0; registerNumber < VP890_DEVICE_REGISTER_COUNT; registerNumber++) {
		VpMpiCmdWrapper(deviceId, ecVal,deviceRegs[registerNumber][0], deviceRegs[registerNumber][1],registerBuffer);

		print_buffer_len+=sprintf(print_buffer+print_buffer_len,"\n\r%s (0x%02X) ",deviceRegsName[registerNumber], deviceRegs[registerNumber][0]);
		for (registerIndex = 0;
			 registerIndex < deviceRegs[registerNumber][1];
			 registerIndex++) {
			print_buffer_len+=sprintf(print_buffer+print_buffer_len,"0x%02X ", registerBuffer[registerIndex]);
		}
	}

	switch(pDevObj->staticInfo.rcnPcn[1]) {
#ifdef VP890_FXS_SUPPORT
		case VP890_DEV_PCN_89116:	/**< FXS - Wideband */
		case VP890_DEV_PCN_89136:	/**< HV FXS - Wideband */
			channelId = 0;
			maxChannelNum = 1;
			break;
#endif

		case VP890_DEV_PCN_89316:	/**< FXO/FXS-Tracker - Wideband */
		case VP890_DEV_PCN_89336:	/**< FXO/FXS-Tracker - Wideband */
			channelId = 0;
			maxChannelNum = 2;
			break;

#ifdef VP890_FXO_SUPPORT
		case VP890_DEV_PCN_89010:	/**< Single Channel FXO */
			channelId = 1;
			maxChannelNum = 2;
			break;
#endif

		default:
			channelId = 0;
			maxChannelNum = 0;
			break;
	}
	//print_buffer_len+=sprintf(print_buffer+print_buffer_len,"\r\nstaticInfo.rcnPcn[1]= %d ,Id= %d ,Num= %d\r\n",pDevObj->staticInfo.rcnPcn[1],channelId,maxChannelNum);

	for (; channelId < maxChannelNum; channelId++) {
		if (channelId == 0) {	/* Always FXS line if exists */
#define VP890_CHANNEL_REGISTER_COUNT    35
			uint8 channelRegs[VP890_CHANNEL_REGISTER_COUNT][2] = {
				{VP890_TX_TS_RD, VP890_TX_TS_LEN},
				{VP890_RX_TS_RD, VP890_RX_TS_LEN},
				{VP890_VP_GAIN_RD, VP890_VP_GAIN_LEN},
				{VP890_SYS_STATE_RD, VP890_SYS_STATE_LEN},
				{VP890_OP_FUNC_RD, VP890_OP_FUNC_LEN},
				{VP890_SS_CONFIG_RD, VP890_SS_CONFIG_LEN},
				{VP890_OP_COND_RD, VP890_OP_COND_LEN},
				{VP890_GX_GAIN_RD, VP890_GX_GAIN_LEN},
				{VP890_GR_GAIN_RD, VP890_GR_GAIN_LEN},
				{VP890_B1_FILTER_RD, VP890_B1_FILTER_LEN},
				{VP890_X_FILTER_RD, VP890_X_FILTER_LEN},
				{VP890_R_FILTER_RD, VP890_R_FILTER_LEN},
				{VP890_B2_FILTER_RD, VP890_B2_FILTER_LEN},
				{VP890_Z1_FILTER_RD, VP890_Z1_FILTER_LEN},
				{VP890_Z2_FILTER_RD, VP890_Z2_FILTER_LEN},
				{VP890_CONV_CFG_RD, VP890_CONV_CFG_LEN},
				{VP890_LOOP_SUP_RD, VP890_LOOP_SUP_LEN},
				{VP890_DC_FEED_RD, VP890_DC_FEED_LEN},
				{VP890_DISN_RD, VP890_DISN_LEN},
				{VP890_TX_PCM_DATA_RD, VP890_TX_PCM_DATA_LEN},
				{VP890_SIGA_PARAMS_RD, VP890_SIGA_PARAMS_LEN},
				{VP890_SIGCD_PARAMS_RD, VP890_SIGCD_PARAMS_LEN},
				{VP890_GEN_CTRL_RD, VP890_GEN_CTRL_LEN},
				{VP890_CADENCE_TIMER_RD, VP890_CADENCE_TIMER_LEN},
				{VP890_CID_DATA_RD, VP890_CID_DATA_LEN},
				{VP890_REGULATOR_PARAM_RD, VP890_REGULATOR_PARAM_LEN},
				{VP890_REGULATOR_CTRL_RD, VP890_REGULATOR_CTRL_LEN},
				{VP890_BAT_CALIBRATION_RD, VP890_BAT_CALIBRATION_LEN},
				{VP890_CID_PARAM_RD, VP890_CID_PARAM_LEN},
				{VP890_ICR1_RD, VP890_ICR1_LEN},
				{VP890_ICR2_RD, VP890_ICR2_LEN},
				{VP890_ICR3_RD, VP890_ICR3_LEN},
				{VP890_ICR4_RD, VP890_ICR4_LEN},
				{VP890_ICR5_RD, VP890_ICR5_LEN},
				{VP890_DC_CAL_REG_RD, VP890_DC_CAL_REG_LEN}
			};

			char *registerName[VP890_CHANNEL_REGISTER_COUNT] = {
				"VP890_TX_TS_RD",			"VP890_RX_TS_RD",			"VP890_VP_GAIN_RD",
				"VP890_SYS_STATE_RD",		"VP890_OP_FUNC_RD", 		"VP890_SS_CONFIG_RD",
				"VP890_OP_COND_RD", 		"VP890_GX_GAIN_RD", 		"VP890_GR_GAIN_RD",
				"VP890_B1_FILTER_RD",		"VP890_X_FILTER_RD",		"VP890_R_FILTER_RD",
				"VP890_B2_FILTER_RD",		"VP890_Z1_FILTER_RD",		"VP890_Z2_FILTER_RD",
				"VP890_CONV_CFG_RD",		"VP890_LOOP_SUP_RD",		"VP890_DC_FEED_RD",
				"VP890_DISN_RD",			"VP890_TX_PCM_DATA_RD", 	"VP890_SIGA_PARAMS_RD",
				"VP890_SIGCD_PARAMS_RD",	"VP890_GEN_CTRL_RD",		"VP890_CADENCE_TIMER_RD",
				"VP890_CID_DATA_RD",		"VP890_REGULATOR_PARAM_RD", "VP890_REGULATOR_CTRL_RD",
				"VP890_BAT_CALIBRATION_RD", "VP890_CID_PARAM_RD",		"VP890_ICR1_RD",
				"VP890_ICR2_RD",			"VP890_ICR3_RD",			"VP890_ICR4_RD",
				"VP890_ICR5_RD",			"VP890_DC_CAL_REG_RD"
			};

			ecVal = (VP890_EC_CH1 | pDevObj->ecVal);
			print_buffer_len+=sprintf(print_buffer+print_buffer_len,"\n\rCHANNEL 0 (FXS)");
			for (registerNumber = 0; registerNumber < VP890_CHANNEL_REGISTER_COUNT; registerNumber++) {
				VpMpiCmdWrapper(deviceId, ecVal,channelRegs[registerNumber][0], channelRegs[registerNumber][1],registerBuffer);

				print_buffer_len+=sprintf(print_buffer+print_buffer_len,"\n\r%s (0x%02X) ",registerName[registerNumber], channelRegs[registerNumber][0]);
				for (registerIndex = 0;
					 registerIndex < channelRegs[registerNumber][1];
					 registerIndex++) {
					print_buffer_len+=sprintf(print_buffer+print_buffer_len,"0x%02X ", registerBuffer[registerIndex]);
				}
			}
		} else if (channelId == 1) {	/* Always FXO line if exists */
#define VP890_FXO_CHANNEL_REGISTER_COUNT    26
			uint8 channelRegs[VP890_FXO_CHANNEL_REGISTER_COUNT][2] = {
				{VP890_PERIOD_DET_RD, VP890_PERIOD_DET_LEN},
				{VP890_TX_TS_RD, VP890_TX_TS_LEN},
				{VP890_RX_TS_RD, VP890_RX_TS_LEN},
				{VP890_VP_GAIN_RD, VP890_VP_GAIN_LEN},
				{VP890_SYS_STATE_RD, VP890_SYS_STATE_LEN},
				{VP890_OP_FUNC_RD, VP890_OP_FUNC_LEN},
				{VP890_SS_CONFIG_RD, VP890_SS_CONFIG_LEN},
				{VP890_OP_COND_RD, VP890_OP_COND_LEN},
				{VP890_LSD_CTL_RD, VP890_LSD_CTL_LEN},
				{VP890_LSD_STAT_RD, VP890_LSD_STAT_LEN},
				{VP890_GX_GAIN_RD, VP890_GX_GAIN_LEN},
				{VP890_GR_GAIN_RD, VP890_GR_GAIN_LEN},
				{VP890_B1_FILTER_RD, VP890_B1_FILTER_LEN},
				{VP890_X_FILTER_RD, VP890_X_FILTER_LEN},
				{VP890_R_FILTER_RD, VP890_R_FILTER_LEN},
				{VP890_B2_FILTER_RD, VP890_B2_FILTER_LEN},
				{VP890_Z1_FILTER_RD, VP890_Z1_FILTER_LEN},
				{VP890_Z2_FILTER_RD, VP890_Z2_FILTER_LEN},
				{VP890_CONV_CFG_RD, VP890_CONV_CFG_LEN},
				{VP890_LOOP_SUP_RD, VP890_LOOP_SUP_LEN},
				{VP890_DISN_RD, VP890_DISN_LEN},
				{VP890_TX_PCM_DATA_RD, VP890_TX_PCM_DATA_LEN},
				{VP890_SIGCD_PARAMS_RD, VP890_SIGCD_PARAMS_LEN},
				{VP890_GEN_CTRL_RD, VP890_GEN_CTRL_LEN},
				{VP890_CADENCE_TIMER_RD, VP890_CADENCE_TIMER_LEN},
				{VP890_ICR1_RD, VP890_ICR1_LEN}
			};

			char *registerName[VP890_CHANNEL_REGISTER_COUNT] = {
				"VP890_PERIOD_DET_RD",		"VP890_TX_TS_RD",			"VP890_RX_TS_RD",
				"VP890_VP_GAIN_RD", 		"VP890_SYS_STATE_RD",		"VP890_OP_FUNC_RD",
				"VP890_SS_CONFIG_RD",		"VP890_OP_COND_RD", 		"VP890_LSD_CTL_RD",
				"VP890_LSD_STAT_RD",		"VP890_GX_GAIN_RD", 		"VP890_GR_GAIN_RD",
				"VP890_B1_FILTER_RD",		"VP890_X_FILTER_RD",		"VP890_R_FILTER_RD",
				"VP890_B2_FILTER_RD",		"VP890_Z1_FILTER_RD",		"VP890_Z2_FILTER_RD",
				"VP890_CONV_CFG_RD",		"VP890_LOOP_SUP_RD",		"VP890_DISN_RD",
				"VP890_TX_PCM_DATA_RD", 	"VP890_SIGCD_PARAMS_RD",	"VP890_GEN_CTRL_RD",
				"VP890_CADENCE_TIMER_RD",	"VP890_ICR1_RD"
			};

			ecVal = (VP890_EC_CH2 | pDevObj->ecVal);
			print_buffer_len+=sprintf(print_buffer+print_buffer_len,"\n\rCHANNEL 1 (FXO)");
			for (registerNumber = 0; registerNumber < VP890_FXO_CHANNEL_REGISTER_COUNT; registerNumber++) {
				VpMpiCmdWrapper(deviceId, ecVal,channelRegs[registerNumber][0], channelRegs[registerNumber][1],
					registerBuffer);

				print_buffer_len+=sprintf(print_buffer+print_buffer_len,"\n\r%s (0x%02X) ",registerName[registerNumber], channelRegs[registerNumber][0]);
				for (registerIndex = 0;
					 registerIndex < channelRegs[registerNumber][1];
					 registerIndex++) {
					print_buffer_len+=sprintf(print_buffer+print_buffer_len,"0x%02X ", registerBuffer[registerIndex]);
				}
			}

		}
	}

	print_buffer_len+=sprintf(print_buffer+print_buffer_len,"\n\r");

	if(print_buffer_len>=DUMP_PRINTK_BUF_SIZE){
	   printk( "Error! buf overflow! print_buffer_len %d > bufSize %d\r\n",print_buffer_len,PRINTK_BUF_SIZE);
	}
	for(dumpPrintkBufCnt=0;dumpPrintkBufCnt<DUMP_PRINTK_BUF_NUM;dumpPrintkBufCnt++){
		printk(print_buffer+dumpPrintkBufCnt*PRINTK_BUF_SIZE);
	}
	if(print_buffer){
	   FREE(print_buffer);
	   print_buffer=NULL;
	}
	return _TDI_SUCCESS;
}

int le89316_fxoGetRingCadence(char* buf){
	if(atomic_read(&le89316_init_ongoing))
	{
		return _TDI_FAIL;
	}
	event_printf("%s - %d\n",__func__,le89316LineStat.fxoRingStat);
	return le89316LineStat.fxoRingStat;
}
int le89316_fxoGetHookState(char* buf){
	if(atomic_read(&le89316_init_ongoing))
	{
		return _TDI_FAIL;
	}
	event_printf("%s - %d \n",__func__,le89316LineStat.fxoHookStat);
    return le89316LineStat.fxoHookStat;
}
int le89316_fxoSetHookState(char* buf){
	if(atomic_read(&le89316_init_ongoing))
	{
		return _TDI_FAIL;
	}
	int state = *(int *)buf;
	event_printf("%s - %d \n",__func__,state);

    switch (state) {
        case TIC_SEIZE: /*off hook*/
        	VpSetLineState(&(lineCtx_ptr[FXO_LINE]),VP_LINE_FXO_TALK);

            return _TDI_SUCCESS;
        case TIC_RELEASE:
        	VpSetLineState(&(lineCtx_ptr[FXO_LINE]),VP_LINE_FXO_OHT);
            return _TDI_SUCCESS;
    }

}
int le89316_fxoGetLineStats(char* buf){
	if(atomic_read(&le89316_init_ongoing))
	{
		return _TDI_FAIL;
	}
	_TDI_fxoLineStat_t* fxoLineStat = (_TDI_fxoLineStat_t*)buf;
	event_printf("%s - chan:%d \n",__func__,fxoLineStat->chan);
	if(le89316LineStat.polRev){
		fxoLineStat->lc = -1;
		fxoLineStat->lv = -1;
	}
	else{
		fxoLineStat->lc = 1;
		fxoLineStat->lv = 1;
	}
	
	if (FXO_LINE_DISABLED == le89316LineStat.fxoLineConn) {
		fxoLineStat->lc = 0;
		fxoLineStat->lv = 0;
	}
	return _TDI_SUCCESS;

}
int le89316_fxoGetPSTNStats(char* buf){

	if(atomic_read(&le89316_init_ongoing))
	{
		return _TDI_FAIL;
	}
	//printk("%s  \n",__func__);

	if (FXO_LINE_DISABLED == le89316LineStat.fxoLineConn) {
		return 0;
	} else {
		if (le89316LineStat.polRev) {
			return 1;
		} else {
			return -1;
		}
	}
}

int le89316_fxoSetMonitorMode(char* buf){
	if(atomic_read(&le89316_init_ongoing))
	{
		return _TDI_FAIL;
	}
	return _TDI_SUCCESS;
}

int le89316_fxoGetLIUStats(char* buf){
	if(atomic_read(&le89316_init_ongoing))
	{
		return _TDI_FAIL;
	}
    return le89316LineStat.fxoLiuStat;
}

//Roger_lt
int le89316_LtTestStart(char* buf) {
	if(atomic_read(&le89316_init_ongoing))
	{
		return _TDI_FAIL;
	}

	return LtTestStart(&lineCtx_ptr,&devCtx_ptr,buf);
}


int* le89316_getLtTestResult(void)
{
	if(atomic_read(&le89316_init_ongoing))
	{
		return _TDI_FAIL;
	}
	return getLtTestResult();
}

int le89316_SetLoopback(char *buf)
{
	_TDI_fxsSetLoopback_t* loopbackSetting=(_TDI_fxsSetLoopback_t*)buf;
	VpOptionLoopbackType loopBack;
	
	if(atomic_read(&le89316_init_ongoing))	//dyma_20140218
		return _TDI_FAIL;
	if(loopbackSetting->chan >= LE89316_CHAN_NUM)
		return _TDI_FAIL;


	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "%s loopback\n", loopbackSetting->enable?"Enable":"Disable");

	if (loopbackSetting->enable)
		loopBack = VP_OPTION_LB_TIMESLOT;
	else
		loopBack = VP_OPTION_LB_OFF;
	if (VP_STATUS_SUCCESS != VpSetOption(&lineCtx_ptr, VP_NULL, VP_OPTION_ID_LOOPBACK, &loopBack)) {

		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "VpSetOption failed\n");
		return _TDI_FAIL;
	}

	return _TDI_SUCCESS;

}


/*[OSBNB00045371] ADD by mtk07059_20150423 , support ring parameters set. */
int le89316_SetRingParam(char* buf)
{
	_TDI_fxsSetRingParams_s* ringParams=(_TDI_fxsSetRingParams_s*)buf;
	VpOptionRingingParamsType vpParams;
	int ret=0;

	//printk("89318_SetRingParam test :\n type:%d;\nfrequency:%d;\namplitude:%d;\ndc:%d;\n",
	//	ringParams->type, ringParams->frequency, ringParams->amplitude, ringParams->dcBias);

	/* check current setting */
	if(0 == ringParams->type){
		ret = VpConfigLine(&(lineCtx_ptr[ringParams->chanId]), VP_NULL, VP_NULL, RINGING_PRO_FXS_le89316);
	}
	else if(1 == ringParams->type){
		ret = VpConfigLine(&(lineCtx_ptr[ringParams->chanId]), VP_NULL, VP_NULL, RING_25HZ_VE890_IB100V_TRAP);
	}
	else{
		return _TDI_FAIL;
	}
	if(ret != VP_STATUS_SUCCESS){
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "ERROR file:%s line:%d, ret:%d\n", __FILE__, __LINE__, ret);
		return _TDI_FAIL;
	}
	
	vpParams.validMask = VP_OPTION_CFG_FREQUENCY | VP_OPTION_CFG_AMPLITUDE | VP_OPTION_CFG_DC_BIAS	|VP_OPTION_CFG_RINGTRIP_THRESHOLD;
	vpParams.amplitude = ringParams->amplitude;
	vpParams.frequency = ringParams->frequency;
	vpParams.dcBias = ringParams->dcBias;
	vpParams.ringTripThreshold = ringParams->ringTripThreshold;	 /* added by mtk08176_20150605  */
	ret = VpSetOption(&(lineCtx_ptr[ringParams->chanId]), VP_NULL, VP_OPTION_ID_RINGING_PARAMS, &vpParams);
	if(ret != VP_STATUS_SUCCESS){
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "ERROR file:%s line:%d, ret:%d\n", __FILE__, __LINE__, ret);
		return _TDI_FAIL;
	}
	return _TDI_SUCCESS;

}


/*[OSBNB00047036] ADD by mtk07059_20150616 , support dc feed parameters set. */
int le89316_SetDcFeedParam(char* buf)
{
	_TDI_fxsSetDcFeedParams_s* dcFeedParams=(_TDI_fxsSetDcFeedParams_s*)buf;
	VpOptionDcFeedParamsType vpParams;
	int ret=0;

	//printk("zl88601_SetRingParam test :\n type:%d;\nfrequency:%d;\namplitude:%d;\ndc:%d;\n",
	//	ringParams->type, ringParams->frequency, ringParams->amplitude, ringParams->dcBias);
	vpParams.validMask = VP_OPTION_CFG_ILA;
	vpParams.ila = dcFeedParams->ila;
	ret = VpSetOption(&(lineCtx_ptr), VP_NULL, VP_OPTION_ID_DCFEED_PARAMS, &vpParams);
	if(ret != VP_STATUS_SUCCESS){
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "ERROR file:%s line:%d, ret:%d\n", __FILE__, __LINE__, ret);
		return _TDI_FAIL;
	}
	return _TDI_SUCCESS;

}


