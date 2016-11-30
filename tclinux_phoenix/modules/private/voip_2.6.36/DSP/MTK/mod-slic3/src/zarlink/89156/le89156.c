/*
 * le89156drv.c
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
#include <Le89156_100V_IB_Profile.h>
#include <vp_api_cfg.h>
#include <linux/jiffies.h>     //dyma_140619
#include "vp890_api.h"
#include "vp890_api_int.h"
#include "_TDI.h"
#include "TDI.h"
#include "le89156.h"
#include "ddr_slic3.h"
#include <linux/libcompileoption.h>


/************************************************************************
 *				GLOBE VARIABLES
 *************************************************************************
 */
static VpDevCtxType            devCtx_ptr;
static Vp890DeviceObjectType   devObj_ptr;
static VpLineCtxType           lineCtx_ptr;
static Vp890LineObjectType     lineObj_ptr;
int le89156_FXS_Init_Cmp = _TDI_FALSE;
int le89156_Get_Mask_Cmp = _TDI_FALSE;
int le89156_Line_Cal_Cmp= _TDI_FALSE;
static atomic_t le89156_init_ongoing=ATOMIC_INIT(0);

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

int le89156_immediateHookState;

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31)
extern void VpSysSemaphoreInit(int semaphoredId);
#endif

slicFuncTbl le89156Func[]={
		le89156_deviceVerify,
		le89156_deviceInit,
		le89156_shutdownHw,
		le89156_paramReset,
		le89156_phoneRingStart,
		le89156_phoneRingStop,
		le89156_eventProcess,
		le89156_fxsSetLineState,
		le89156_fxsGetLineState,
		le89156_fxsSetBatteryPol,
		le89156_fxsGetBatteryPol,
		le89156_fxsGetHookState,
		le89156_ringState,
		le89156_fxsGetPulseStatus,
		le89156_fxsSetGainLv,
		slicFunc_dummy,
		slicFunc_dummy,
		slicFunc_dummy,
		slicFunc_dummy,
		slicFunc_dummy,
		slicFunc_dummy,
		le89156_dumpregister,
		slicFunc_dummy,
		le89156_LtTestStart,//Roger_lt
		le89156_getLtTestResult,//Roger_lt
		slicFunc_dummy,  //dyma_20140428 metering
		le89156_SetLoopback,  //FXS_SET_LOOPBACK
		le89156_SetRingParam,	//mtk07059_20150423
		le89156_SetDcFeedParam,	//mtk07059_20150616
};

/*20110714 PTChen add for slic country*/
const VpProfileDataType *DEVICE_PRO_89156;
const VpProfileDataType *AC_PRO_FXS_89156;
const VpProfileDataType *DC_PRO_FXS_89156;
const VpProfileDataType *RINGING_PRO_FXS_89156;
const VpProfileDataType *AC_PRO_FXO_89156;
const VpProfileDataType *CONFIG_PRO_FXO_89156;
const VpProfileDataType *RINGING_CADENCE_PRO_89156;
const VpProfileDataType *CALL_ID_PRO_89156;

hwProfList_t *country_mapping_89156;

//dyma_20140428 
hwProfList_t country_mapping_89156_a[] =
{
	{"Default"    	  , C_DEF, 	DEV_PROFILE_Inverting_Boost_100V_89156_A,		AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Australia"  	  ,  C_AU, 	DEV_PROFILE_Inverting_Boost_100V_89156_A,		AC_FXS_RF50_AU_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Austria"   	  ,  C_AT, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_AT_89156 		, DC_FXS_DEF_89156, RING_AT_89156  		, NULL	, NULL, NULL, NULL },
	{"BEL-Belgium"    ,  C_BE, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_BE_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"BRA-Brazil"     ,  C_BR, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_600R_DEF_89156 	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Canada"    	  ,  C_CA, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"CHN-China"      ,  C_CN, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_CN_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Czech"       	  ,  C_CZ, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"DNK-Denmark"    ,  C_DK,  DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_DK_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"ETS-ETSI"       ,  C_EU, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_ETSI_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"FIN-Finland"    ,  C_FI, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_FI_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"FRA-France"     ,  C_FR, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_FR_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"DEU-Germany"    ,  C_DE,  DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_DE_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Greece"     	  ,  C_GR, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_GR57_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"HongKong"  	  ,  C_HK, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"HUN-Hungary"    ,  C_HU, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_HU_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Ireland"   	  ,  C_IE, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_IE_89156		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Israel"    	  ,  C_IL, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"India"      	  ,  C_IN, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_600R_DEF_89156 	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"ITA-Italy"      ,  C_IT, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_IT_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"JPN-Japan"      ,  C_JP, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_JP_89156 		, DC_FXS_DEF_89156, RING_JP_89156 		, NULL	, NULL, NULL, NULL },
	{"Singapore" 	  ,  C_SG, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_SG_89156 		, NULL	, NULL, NULL, NULL },
	{"S.Korea"   	  ,  C_KR, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_600R_DEF_89156 	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Mexico"     	  ,  C_MX, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_600R_DEF_89156 	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"NLD-Netherlands",  C_NL, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_NL_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"NewZealand" 	  ,  C_NZ, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_NZ_89156		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Norway"     	  ,  C_NO, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_NO_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Poland"   	  ,  C_PL, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Portugal"   	  ,  C_PT, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_PT_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Slovenia"   	  ,  C_SI, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_600R_DEF_89156  , DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Taiwan"     	  ,  C_TW, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Spain"     	  ,  C_ES, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_ES_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"SWE-Sweden"     ,  C_SE, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_SE_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"GBR-Great Britain" ,  C_GB, DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_GB_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"USA-United States" ,  C_US, DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"S.Africa"   	  ,  C_ZA, 	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_ZA_89156		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Vietnam"		  ,C_VN,	DEV_PROFILE_Inverting_Boost_100V_89156_A, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"RSV1"			  ,C_RSV1,	DEV_PROFILE_Inverting_Boost_100V_89156_A,		AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"RSV2"			  ,C_RSV2,	DEV_PROFILE_Inverting_Boost_100V_89156_A,		AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"RSV3"			  ,C_RSV3,	DEV_PROFILE_Inverting_Boost_100V_89156_A,		AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },

};

hwProfList_t country_mapping_89156_b[] =
{
	{"Default"    	  , C_DEF, 	DEV_PROFILE_Inverting_Boost_100V_89156_B,		AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Australia"  	  ,  C_AU, 	DEV_PROFILE_Inverting_Boost_100V_89156_B,		AC_FXS_RF50_AU_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Austria"   	  ,  C_AT, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_AT_89156 		, DC_FXS_DEF_89156, RING_AT_89156  		, NULL	, NULL, NULL, NULL },
	{"BEL-Belgium"    ,  C_BE, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_BE_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"BRA-Brazil"     ,  C_BR, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_600R_DEF_89156 	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Canada"    	  ,  C_CA, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"CHN-China"      ,  C_CN, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_CN_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Czech"       	  ,  C_CZ, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"DNK-Denmark"    ,  C_DK,  DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_DK_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"ETS-ETSI"       ,  C_EU, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_ETSI_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"FIN-Finland"    ,  C_FI, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_FI_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"FRA-France"     ,  C_FR, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_FR_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"DEU-Germany"    ,  C_DE,  DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_DE_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Greece"     	  ,  C_GR, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_GR57_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"HongKong"  	  ,  C_HK, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"HUN-Hungary"    ,  C_HU, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_HU_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Ireland"   	  ,  C_IE, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_IE_89156		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Israel"    	  ,  C_IL, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"India"      	  ,  C_IN, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_600R_DEF_89156 	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"ITA-Italy"      ,  C_IT, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_IT_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"JPN-Japan"      ,  C_JP, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_JP_89156 		, DC_FXS_DEF_89156, RING_JP_89156 		, NULL	, NULL, NULL, NULL },
	{"Singapore" 	  ,  C_SG, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_SG_89156 		, NULL	, NULL, NULL, NULL },
	{"S.Korea"   	  ,  C_KR, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_600R_DEF_89156 	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Mexico"     	  ,  C_MX, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_600R_DEF_89156 	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"NLD-Netherlands",  C_NL, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_NL_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"NewZealand" 	  ,  C_NZ, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_NZ_89156		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Norway"     	  ,  C_NO, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_NO_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Poland"   	  ,  C_PL, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Portugal"   	  ,  C_PT, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_PT_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Slovenia"   	  ,  C_SI, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_600R_DEF_89156  , DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Taiwan"     	  ,  C_TW, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Spain"     	  ,  C_ES, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_ES_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"SWE-Sweden"     ,  C_SE, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_SE_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"GBR-Great Britain" ,  C_GB, DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_GB_89156 		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"USA-United States" ,  C_US, DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"S.Africa"   	  ,  C_ZA, 	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_ZA_89156		, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"Vietnam"		  ,C_VN,	DEV_PROFILE_Inverting_Boost_100V_89156_B, 	AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"RSV1"			  ,C_RSV1,	DEV_PROFILE_Inverting_Boost_100V_89156_B,		AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"RSV2"			  ,C_RSV2,	DEV_PROFILE_Inverting_Boost_100V_89156_B,		AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	{"RSV3"			  ,C_RSV3,	DEV_PROFILE_Inverting_Boost_100V_89156_B,		AC_FXS_RF50_600R_DEF_89156	, DC_FXS_DEF_89156, RING_25HZ_DEF_89156 , NULL	, NULL, NULL, NULL },
	
};

/******************************************************************************
*	Function:	 slicParamReset
*
*	Description: Reset the parameters used to init slic 
*
*	Parameters:
*
*	Returns:
*
*******************************************************************************/
void slicParamReset_89156(int idx){
	/*====== decide profile according to Country code =====*/
	/*Reset by default table*/
	if (TCSUPPORT_ZARLINK_LE89156A_VAL){
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "TCSUPPORT_ZARLINK_LE89156A\n");
		DEVICE_PRO_89156 = DEV_PROFILE_Inverting_Boost_100V_89156_A;
		country_mapping_89156 = country_mapping_89156_a;
	}
	else{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "TCSUPPORT_ZARLINK_LE89156B\n");
		DEVICE_PRO_89156 = DEV_PROFILE_Inverting_Boost_100V_89156_B;
		country_mapping_89156 = country_mapping_89156_b;
	}
	
	AC_PRO_FXS_89156 = AC_FXS_RF50_600R_DEF_89156;
	DC_PRO_FXS_89156 = DC_FXS_DEF_89156;
	RINGING_PRO_FXS_89156 = RING_25HZ_DEF_89156;
	AC_PRO_FXO_89156 = VP_PTABLE_NULL;
	CONFIG_PRO_FXO_89156 = VP_PTABLE_NULL;
	CALL_ID_PRO_89156 = VP_PTABLE_NULL;
	if((idx >= C_DEF) && (idx < C_NUM)){
		DEVICE_PRO_89156 = country_mapping_89156[idx].pDevice;
		AC_PRO_FXS_89156 = country_mapping_89156[idx].pAcFXS;
		DC_PRO_FXS_89156 = country_mapping_89156[idx].pDcFXS;
		RINGING_PRO_FXS_89156 = country_mapping_89156[idx].pRingingFXS;
		AC_PRO_FXO_89156 = country_mapping_89156[idx].pAcFXO;
		CONFIG_PRO_FXO_89156 = country_mapping_89156[idx].pConfigFXO;
		CALL_ID_PRO_89156 = country_mapping_89156[idx].pCallID;
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "%s Successful(CountryCode=%s)\n",__func__,\
				country_mapping_89156[idx].name);
	}
	else{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "err: slicParamReset idx(%d) UNKNOW\n",idx);
	}
}


static int testCheck_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data){
	int len = 0;
	//VpSetLineState(&(lineCtx_ptr[FXO_LINE]),VP_LINE_FXO_LOOP_OPEN);
	//VpMpiCmdWrapper(0, testCh, test,
	//				VP890_NO_UL_SIGREG_LEN, &testMpiData);
	int8 testdata = 0;
	VpMpiCmd(0, 0x1, 0x61,0x1, &testdata);

	printk("data:%x \n",testdata);
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
	return count;
}
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
int le89156_paramReset(char* buf){
//	int country = *(int *)buf;
	return _TDI_SUCCESS;
}

/*_____________________________________________________________________________
 **      function name: le89156_deviceVerify
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
int le89156_deviceVerify(char* buf){
	deviceInfo_t* devInfo = (deviceInfo_t*)buf;
	uint8	res_arr[2] = {0};
    /*
     * Create the device object and device context
     */
	if (VP_STATUS_SUCCESS != VpMakeDeviceObject(VP_DEV_890_SERIES, devInfo->deviceNum, &devCtx_ptr, &devObj_ptr)) {
		event_printf("%s:%d Error returned from VpMakeDeviceObject!\n",__FILE__, __LINE__);
       	return _TDI_FAIL;
	}
	
	/* printing the SLIC properties */
	//VpMpiCmd(devInfo->deviceNum, VP890_EC_CH1, VP890_DEVTYPE_CMD, 0x02, res_arr); //dyma_130118
	VpMpiCmd(devInfo->deviceNum, VP890_EC_CH1, VP890_DEVTYPE_RD, 0x02, res_arr);

	if(res_arr[0] != VP890_DEVTYPE_LEN || res_arr[1] != VP890_DEV_PCN_89116){
		dbg_printf("devInfo->deviceNum=%d res_arr[0]=%x res_arr[0]=%x\n",devInfo->deviceNum,res_arr[0],res_arr[1]);
		return _TDI_FAIL;
	}
	devInfo->deviceNum += 1;
	devInfo->fxsNum += LE89156_FXS_NUM;
	devInfo->fxoNum += LE89156_FXO_NUM;
	devInfo->slicType = ZARLINK_89156;
	devInfo->pcm1ChNum += LE89156_FXS_NUM+LE89156_FXO_NUM;
	devInfo->pcm2ChNum += 0;
	return _TDI_SUCCESS;
}

/*_____________________________________________________________________________
 **      function name: le89156_deviceInit
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
int le89156_deviceInit(char* buf){
	int                  devCnt;
    //int                  lineCnt;
    VpStatusType          status;
    VpOptionCodecType     codecOption;
    VpOptionPulseModeType pulseMode;
    VpOptionTimeslotType  timeSlots;
    VpOptionEventMaskType eventMask;
    _TDI_initSetting_t* pinitSetting = ( _TDI_initSetting_t*)buf;
    devCnt = pinitSetting->chan;
    event_printf("%s",__func__);

	le89156_Line_Cal_Cmp	 = _TDI_FALSE;
	le89156_FXS_Init_Cmp = _TDI_FALSE;
	
	atomic_set(&le89156_init_ongoing,1);

    VpSysSemaphoreInit(0);
    event_printf("--chip select:%d \n",_TDI_SLIC_SPI_ID);

    //for (devCnt = 0; devCnt < _TDI_FXS_NUM; devCnt++) {
    	event_printf(">>dev cnt:%d \n",devCnt);
#if 0
        struct proc_dir_entry *testCheck_proc;
        printk("Create PSTN check proc \n");
        testCheck_proc = create_proc_entry("test_stats", 0, NULL);
        testCheck_proc->read_proc = testCheck_read_proc;
        testCheck_proc->write_proc = testCheck_write_proc;
#endif

        /*
         * Create the device object and device context
         */
        if (VP_STATUS_SUCCESS != VpMakeDeviceObject(VP_DEV_890_SERIES,
        		devCnt, &devCtx_ptr, &devObj_ptr)) {
        	dbg_printf("Error returned from VpMakeDeviceObject!\n");
			atomic_set(&le89156_init_ongoing,0);
            return _TDI_FAIL;
        }
		/*
		 * Create the line object and line context
		 */
		if (VP_STATUS_SUCCESS != VpMakeLineObject(VP_TERM_FXS_GENERIC, 0,
				&lineCtx_ptr, &lineObj_ptr, &devCtx_ptr)) {
			dbg_printf("Error returned from VpMakeLineObject!\n");
			atomic_set(&le89156_init_ongoing,0);
            return _TDI_FAIL;
		}
		/*
		 * Initialize the Device
		 */
		/*20110714 PTChen modified for slic country*/
		slicParamReset_89156(pinitSetting->hwProfId);
		status = VpInitDevice(&devCtx_ptr, DEVICE_PRO_89156, AC_PRO_FXS_89156, DC_PRO_FXS_89156, RINGING_PRO_FXS_89156, VP_PTABLE_NULL, VP_PTABLE_NULL);
		if (VP_STATUS_SUCCESS != status) {
			dbg_printf("Device Did Not Init Properly %d - (dev:%d)\n",status,devCnt);
			atomic_set(&le89156_init_ongoing,0);
			return _TDI_FAIL;
		}
	    while (le89156_FXS_Init_Cmp != TRUE) {
	    	event_printf(">>1.5\n");
	        /*
	         * Wait for event indicating that the initialization is complete
	         */
			mdelay(10); /*SERENA_MODIFY*/
			le89156_eventProcess((char *)&devCnt);  /* Only run for one line per device */
	    }


		if(pinitSetting->slicCodecOption == CODEC_LINEAR){
			event_printf("ODEC_LINEAR setting\n");
			/*
			 * Set 16-bit Linear PMC codec mode for each line
			 */
			codecOption = VP_OPTION_LINEAR;
			if (VP_STATUS_SUCCESS != VpSetOption(VP_NULL, &devCtx_ptr,
						VP_OPTION_ID_CODEC, &codecOption)) {
				dbg_printf("Error: failed to set codec type!\n");
				atomic_set(&le89156_init_ongoing,0);
				return _TDI_FAIL;
			}
		}
		/*20110509 PTChen add for AC, now D2 use CODEC_LINEAR, AC use CODEC_ALAW*/
		else if(pinitSetting->slicCodecOption == CODEC_ALAW){
			event_printf("CODEC_ALAW setting\n");
			/*
			 * Set 16-bit ALAW PMC codec mode for each line
			 */
			codecOption = VP_OPTION_ALAW;
			if (VP_STATUS_SUCCESS != VpSetOption(VP_NULL, &devCtx_ptr,
						VP_OPTION_ID_CODEC, &codecOption)) {
				dbg_printf("Error: failed to set codec type!\n");
				atomic_set(&le89156_init_ongoing,0);
				return _TDI_FAIL;
			}
		}else if(pinitSetting->slicCodecOption == CODEC_ULAW){
			event_printf("CODEC_ALAW setting\n");
			/*
			 * Set 16-bit ULAW PMC codec mode for each line
			 */
			codecOption = VP_OPTION_MLAW;
			if (VP_STATUS_SUCCESS != VpSetOption(VP_NULL, &devCtx_ptr,
						VP_OPTION_ID_CODEC, &codecOption)) {
				dbg_printf("Error: failed to set codec type!\n");
				atomic_set(&le89156_init_ongoing,0);
				return _TDI_FAIL;
			}
		}
		/*20110509 PTChen add for AC end*/
        /*
         * Disable Pulse Dial Detection
         */
        //pulseMode = VP_OPTION_PULSE_DECODE_ON;
        //VpSetOption(VP_NULL, &devCtx_ptr, VP_OPTION_ID_PULSE_MODE, &pulseMode);
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
        event_printf(">>3\n");
        eventMask.signaling = 0xffff;
        eventMask.signaling &= ~(VP_LINE_EVID_HOOK_OFF | VP_LINE_EVID_HOOK_ON);	/*SERENA_MODIFY*/
        /*
         * Configure to detect response of Line Calibration routine
         */
        eventMask.response = 0xffff;
        eventMask.response &= ~(VP_EVID_CAL_CMP | VP_EVID_CAL_BUSY);
	eventMask.signaling &= ~(VP_LINE_EVID_FLASH);
	eventMask.process &= ~(VP_LINE_EVID_SIGNAL_CMP);

        if (VP_STATUS_SUCCESS != VpSetOption(VP_NULL, &devCtx_ptr,
                VP_OPTION_ID_EVENT_MASK, &eventMask)) {
        	dbg_printf("VpSetOption: FXS_LINE VP_OPTION_ID_EVENT_MASK failed\n");
			atomic_set(&le89156_init_ongoing,0);
            return _TDI_FALSE;
        }

        /*
         * Set timeslot line option, and activate lines
         */if(pinitSetting->slicCodecOption == CODEC_LINEAR){
		timeSlots.tx = ((devCnt) << 1);
		timeSlots.rx = ((devCnt) << 1);
        }
		else{
			timeSlots.tx = ((devCnt));
			timeSlots.rx = ((devCnt));
		}
		
		VpSetOption(&lineCtx_ptr, VP_NULL, VP_OPTION_ID_TIMESLOT,
			&timeSlots);

		//VpSetLineState(&lineCtx_ptr, VP_LINE_ACTIVE);/*for COC by jq.zhu , OSBNB00030567, 20130327*/

		/*
		 * Adjust Tx/Rx relative gain levels.
		 */
		event_printf("gain--TX :%x RX: %x \n",pinitSetting->speakSlicVal, pinitSetting->listenSlicVal);
		VpSetRelGain(&lineCtx_ptr,pinitSetting->speakSlicVal, pinitSetting->listenSlicVal, 0);

		/*
		 * Calibrate Lines
		 */
		/*le89156_Line_Cal_Cmp = FALSE;
		VpCalLine(&lineCtx_ptr);
		while (le89156_Line_Cal_Cmp != TRUE) {
			 * Wait for event indicating that calibration is complete
			mdelay(10);
			le89156_eventProcess((char *)&devCnt);
		}*/
        event_printf(">>5\n");
        /*
         * Init the hook state for each FXS channel
         */
        le89156_immediateHookState = TIC_IMMEDIATE_ONHOOK;
		atomic_set(&le89156_init_ongoing,0);
    //}
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
int le89156_shutdownHw(char* buf){
    int    lineCnt;
    event_printf("%s",__func__);
	
	if(atomic_read(&le89156_init_ongoing))
	{
			return _TDI_FAIL;
	}
    /*
     * Power down line
     */
    VpSetLineState(&lineCtx_ptr, VP_LINE_DISCONNECT);
    /*
     * Free the memory
     */
    VpFreeLineCtx(&lineCtx_ptr);
	
    return _TDI_SUCCESS;
}
/*_____________________________________________________________________________
 **      function name: le89156_fxsSetLineState
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
int le89156_fxsSetLineState(char* buf){
    uint8 stateType = 0;
    _TDI_lineInfo_t* plineInfo = (_TDI_lineInfo_t*)buf;
    /*
     * Map to Legerity State Types
     */
     if(atomic_read(&le89156_init_ongoing))
	 {
			return _TDI_FAIL;
	}
    event_printf("%s",__func__);
	
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
    event_printf("---ch:%d status: %d \n",plineInfo->chan,stateType);
    VpSetLineState(&lineCtx_ptr,stateType);
    return _TDI_SUCCESS;
}

/*_____________________________________________________________________________
 **      function name: le89156_fxsGetLineState
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
int le89156_fxsGetLineState(char* buf){
    VpLineStateType  state;
    _TDI_lineInfo_t* plineInfo = (_TDI_lineInfo_t*)buf;
	if(atomic_read(&le89156_init_ongoing))
	{
		plineInfo->retTicStat = TIC_STATUS_ERROR;
		return _TDI_FAIL;
	}

	
    event_printf("%s",__func__);
    VpGetLineState(&lineCtx_ptr, &state);

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
 **      function name: le89156_fxsGetBatteryPol
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
int le89156_fxsGetBatteryPol(char* buf){
    VpLineStateType  polarity;
    VpStatusType     status;
    _TDI_batteryPolInfo_t* pBatteryPolInfo = (_TDI_batteryPolInfo_t* )buf;
	
	if(atomic_read(&le89156_init_ongoing))
	{
		pBatteryPolInfo->retTicStat = TIC_STATUS_ERROR;
		return _TDI_FAIL;
	}
	
    event_printf("%s",__func__);
    status = VpGetLineState(&lineCtx_ptr, &polarity);
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
 **      function name: le89156_fxsSetBatteryPol
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
int le89156_fxsSetBatteryPol(char* buf){
    VpLineStateType  curState;
    VpStatusType     getStatus;
    VpStatusType     setStatus;
    _TDI_batteryPolInfo_t* pBatteryPolInfo = (_TDI_batteryPolInfo_t* )buf;
	if(atomic_read(&le89156_init_ongoing))
	{
		return _TDI_FAIL;
	}
    event_printf("%s",__func__);
    getStatus = VpGetLineState(&lineCtx_ptr, &curState);
    if (VP_STATUS_SUCCESS != getStatus) {
        dbg_printf("%s:%d Error!\n", __FILE__, __LINE__);
        return _TDI_FAIL;
    }

    switch (curState) {
        case VP_LINE_ACTIVE:
            /* fall through */
        case VP_LINE_ACTIVE_POLREV:
            if (TIC_BATT_POL_FORWARD == pBatteryPolInfo->polarity) {
                setStatus = VpSetLineState(&lineCtx_ptr, VP_LINE_ACTIVE);
            }
            else {
                setStatus = VpSetLineState(&lineCtx_ptr,
                        VP_LINE_ACTIVE_POLREV);
            }
            break;
        case VP_LINE_OHT:
            /* fall through */
        case VP_LINE_OHT_POLREV:
            if (TIC_BATT_POL_FORWARD == pBatteryPolInfo->polarity) {
                setStatus = VpSetLineState(&lineCtx_ptr, VP_LINE_OHT);
            }
            else {
                setStatus = VpSetLineState(&lineCtx_ptr,
                        VP_LINE_OHT_POLREV);
            }
            break;
        case VP_LINE_STANDBY:
            /* fall through */
        case VP_LINE_STANDBY_POLREV:
            if (TIC_BATT_POL_FORWARD == pBatteryPolInfo->polarity) {
                setStatus = VpSetLineState(&lineCtx_ptr, VP_LINE_STANDBY);
            }
            else {
                setStatus = VpSetLineState(&lineCtx_ptr,
                        VP_LINE_STANDBY_POLREV);
            }
            break;
        case VP_LINE_RINGING:
            /* fall through */
        case VP_LINE_RINGING_POLREV:
            if (TIC_BATT_POL_FORWARD == pBatteryPolInfo->polarity) {
                setStatus = VpSetLineState(&lineCtx_ptr, VP_LINE_RINGING);
            }
            else {
                setStatus = VpSetLineState(&lineCtx_ptr,
                        VP_LINE_RINGING_POLREV);
            }
            break;

        case VP_LINE_TALK:
            /* fall through */
        case VP_LINE_TALK_POLREV:
            /* fall through */
        default:
            if (TIC_BATT_POL_FORWARD == pBatteryPolInfo->polarity) {
                setStatus = VpSetLineState(&lineCtx_ptr, VP_LINE_TALK);
            }
            else {
                setStatus = VpSetLineState(&lineCtx_ptr,
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
 **      function name: le89156_fxsGetHookState
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
int le89156_fxsGetHookState(char* buf){
	_TDI_hookStat_t* pHookStat = (_TDI_hookStat_t*)buf;
	if(atomic_read(&le89156_init_ongoing))
	{
		return _TDI_FAIL;
	}
	event_printf("%s",__func__);
	pHookStat->hookStat = le89156_immediateHookState;
	return _TDI_SUCCESS;
}

/*_____________________________________________________________________________
 **      function name: le89156_ringState
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
int le89156_ringState(char* buf){
    int state;
    int count;
    int ret;
    _TDI_ringSetting_t *pRingSetting = (_TDI_ringSetting_t *) buf;
	static unsigned long pre_jiffies;

	if(atomic_read(&le89156_init_ongoing))
		return _TDI_FAIL;

	
    event_printf("%s---%d %d",__func__,state,pRingSetting->count);
    state = pRingSetting->state;
    count = pRingSetting->count;
    ret   = _TDI_FAIL;


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
    event_printf("over: %s---%d %d %d \n",__func__,state,pRingSetting->count,ret);

    return (ret);

}
int le89156_fxsGetPulseStatus(char* buf){
	if(atomic_read(&le89156_init_ongoing))
		return _TDI_FAIL;
	event_printf("%s",__func__);
	return _TDI_SUCCESS;
}

int le89156_fxsSetGainLv(char* buf){
	_TDI_gainSetting_t* gainSetting = (_TDI_gainSetting_t *)buf;
	VpStatusType gainSetResult;
	if(atomic_read(&le89156_init_ongoing))
		return _TDI_FAIL;
	gainSetResult = VpSetRelGain(&lineCtx_ptr,gainSetting->speakSlicVal,gainSetting->listenSlicVal, 0);
	dbg_printf("%s %x/%x \n",__func__,gainSetting->speakSlicVal,gainSetting->listenSlicVal);

	if(gainSetResult != VP_STATUS_SUCCESS){
		return _TDI_FAIL;
	}
	
	return _TDI_SUCCESS;
}
int le89156_fxsGetGain(char* buf){
	if(atomic_read(&le89156_init_ongoing))
		return _TDI_FAIL;
	event_printf("%s",__func__);
	return _TDI_SUCCESS;
}


int le89156_phoneRingStart(char* buf){
	//int channel = *(int*) buf;
	if(atomic_read(&le89156_init_ongoing))
		return _TDI_FAIL;
	event_printf("%s",__func__);
	return _TDI_SUCCESS;
}

int le89156_phoneRingStop(char* buf){
	if(atomic_read(&le89156_init_ongoing))
		return _TDI_FAIL;
	//int channel = *(int*) buf;
	event_printf("%s",__func__);
	return _TDI_SUCCESS;
}

int le89156_eventProcess(char* buf){
    VpDevCtxType *legerityDev_ptr;
    VpEventType   legerityEventObj;
    bool          legerityEventFlag = FALSE;
    bool          legerityEventStatus = FALSE;
    int          chan;
    uint16        mpiData;      /* Workaround for VE890 delayed on-hook event */
    uint8         ecVal[] =  { VP890_EC_CH1, VP890_EC_CH2 };
    int devCnt = *(int*)buf;
    event_printf("%s - %d \n",__func__,devCnt);

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
    	event_printf("||||%d \n",devCnt);
        legerityEventStatus = VpGetEvent(legerityDev_ptr, &legerityEventObj);
        if ((TRUE == legerityEventStatus) &&
                (VP_STATUS_SUCCESS == legerityEventObj.status)) {
            /*
             * Valid Status received from Legerity,
             * check which line has status, and do some error checking.
             */
            chan = legerityEventObj.channelId;

            if (VP_EVCAT_RESPONSE == legerityEventObj.eventCategory) {
                switch (legerityEventObj.eventId) {
                    case VP_DEV_EVID_DEV_INIT_CMP:
                    	event_printf("%d set init true\n",devCnt);
                        /* Device Initialization is complete */
                        le89156_FXS_Init_Cmp = TRUE;
                        break;
                    case VP_LINE_EVID_RD_OPTION:
                    	dbg_printf("VP_LINE_EVID_RD_OPTION\n");
                        /* Read the Event Mask */
                        le89156_Get_Mask_Cmp = TRUE;
                        break;
                    case VP_EVID_CAL_CMP:
                        /* Line Calibration is complete */
                    	dbg_printf("ch %d set init true\n",devCnt);
                        le89156_Line_Cal_Cmp = TRUE;
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
                    	le89156_immediateHookState = TIC_IMMEDIATE_OFFHOOK;
                        break;
                    case VP_LINE_EVID_HOOK_ON:
                    	event_printf("%d on true\n",devCnt);
                    	le89156_immediateHookState = TIC_IMMEDIATE_ONHOOK;
                        break;

// 					/*SERENA_MODIFY*/
// 					case VP_LINE_EVID_PULSE_DIG:
// 						VpSetLineState(lineCtx_ptr[lineCnt], VP_LINE_TALK);
// 						if(pEvent->eventData <= 5){
// 							VpSetLineTone(lineCtx_ptr[lineCnt], tonePtrs[]);
// 						}
// 						break;
                    default:
                        /* Do nothing */
                        break;
                }
            }
        }
    }

    /*
     *  Workaround for VE890 delayed on-hook event.
     *  Check the 0x4D Signaling Register directly to determine on-hook event.
     */
	/*VpMpiCmd(devCnt, VP890_EC_CH1, VP890_NO_UL_SIGREG_RD,
					VP890_NO_UL_SIGREG_LEN, &mpiData);
	if (_TDI_VP890_SIGREG_LIU & mpiData ) {     //off-hook state
		if (le89156_immediateHookState != TIC_IMMEDIATE_OFFHOOK) {
			le89156_immediateHookState  = TIC_IMMEDIATE_OFFHOOK;
		}
		else { }
	} else {                                    // on-hook state
		if (le89156_immediateHookState != TIC_IMMEDIATE_ONHOOK) {
			le89156_immediateHookState  = TIC_IMMEDIATE_ONHOOK;
		}
	}*/
	event_printf("%s - %d over\n",__func__,devCnt);
	return _TDI_SUCCESS;
}

int le89156_dumpregister(char* buf){
	#define PRINTK_BUF_SIZE 	  1024	// It is the static buf size in function vprintk() which called by printk() of /linux/kernel/printk.c
	#define DUMP_PRINTK_BUF_NUM   3
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
	if(atomic_read(&le89156_init_ongoing))
		return _TDI_FAIL;

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
	   printk("Error! buf overflow! print_buffer_len %d > bufSize %d\r\n",print_buffer_len,PRINTK_BUF_SIZE);
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

//Roger_lt
int le89156_LtTestStart(char* buf) {
	if(atomic_read(&le89156_init_ongoing))
		return _TDI_FAIL;

	return LtTestStart(&lineCtx_ptr,&devCtx_ptr,buf);
}


int* le89156_getLtTestResult(void)
{
	if(atomic_read(&le89156_init_ongoing))
		return _TDI_FAIL;

	return getLtTestResult();
}

int le89156_SetLoopback(char *buf)
{
	_TDI_fxsSetLoopback_t* loopbackSetting=(_TDI_fxsSetLoopback_t*)buf;
	VpOptionLoopbackType loopBack;
	
	if(atomic_read(&le89156_init_ongoing))	//dyma_20140218
		return _TDI_FAIL;
	if(loopbackSetting->chan >= LE89156_FXS_NUM)
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
int le89156_SetRingParam(char* buf)
{
	_TDI_fxsSetRingParams_s* ringParams=(_TDI_fxsSetRingParams_s*)buf;
	VpOptionRingingParamsType vpParams;
	int ret=0;

	//printk("zl88601_SetRingParam test :\n type:%d;\nfrequency:%d;\namplitude:%d;\ndc:%d;\n",
	//	ringParams->type, ringParams->frequency, ringParams->amplitude, ringParams->dcBias);

	/* check current setting */
	if(0 == ringParams->type){
		ret = VpConfigLine(&(lineCtx_ptr), VP_NULL, VP_NULL, RINGING_PRO_FXS_89156);
	}
	else if(1 == ringParams->type){
		ret = VpConfigLine(&(lineCtx_ptr), VP_NULL, VP_NULL, RING_25HZ_TRAP_89156);
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
	ret = VpSetOption(&(lineCtx_ptr), VP_NULL, VP_OPTION_ID_RINGING_PARAMS, &vpParams);
	if(ret != VP_STATUS_SUCCESS){
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "ERROR file:%s line:%d, ret:%d\n", __FILE__, __LINE__, ret);
		return _TDI_FAIL;
	}
	return _TDI_SUCCESS;

}


/*[OSBNB00047036] ADD by mtk07059_20150616 , support dc feed parameters set. */
int le89156_SetDcFeedParam(char* buf)
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
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "ERROR ret:%d\n", ret);
		return _TDI_FAIL;
	}
	return _TDI_SUCCESS;

}

#if 0
int le89156_fxoGetRingCadence(char* buf){
	event_printf("%s",__func__);
	return _TDI_SUCCESS;
}
int le89156_fxoGetHookState(char* buf){
	event_printf("%s",__func__);
	return _TDI_SUCCESS;
}
int le89156_fxoSetHookState(char* buf){
	event_printf("%s",__func__);
	return _TDI_SUCCESS;
}
int le89156_fxoGetLineStats(char* buf){
	event_printf("%s",__func__);
	return _TDI_SUCCESS;
}
int le89156_fxoGetPSTNStats(char* buf){
	event_printf("%s",__func__);
	return _TDI_SUCCESS;
}

int le89156_fxoSetMonitorMode(char* buf){
	event_printf("%s",__func__);
	return _TDI_SUCCESS;
}
#endif

