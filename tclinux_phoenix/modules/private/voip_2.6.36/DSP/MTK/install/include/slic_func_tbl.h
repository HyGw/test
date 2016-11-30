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
 *	File:	flic_func_tbl.h
 *	 
 *	Author: James.Hao
 *	 
 *	Last Revision Date: 2013.6.18
 *	 
 *	Description:
 *	slic func tbl struct Definition
 *	
 *
 */
#include "_TDI.h"
#include <ecnt_hook/ecnt_hook_voip.h>

typedef struct{
	int (*SLIC_SEP_HookStatusGet)(_TDI_hookStat_t* pHookStat);
	int (*SLIC_SEP_AudioGainSet)(int cid, int speakdb, int listendb);
	int (*SLIC_SEP_LFStatusCfg)(unsigned int isSet,_TDI_lineInfo_t* plineInfo);
	int (*SLIC_SEP_RingIntervalSetup)(int cid, int active_ms, int inactive_ms);
#ifdef OSBN_VOIP	
	int (*SLIC_SEP_getDeviceInfo)(int selection);
#else
	int (*SLIC_SEP_TotalChannelGet)(void);	
#endif	
	int (*SLIC_SEP_Dest)(int cid);
	int (*SLIC_SEP_SlicReset)(void);
	int (*SLIC_SEP_ConfQuery)(_TDI_initSetting_t* pinitSetting);
	int (*SLIC_SEP_Init)(_TDI_initSetting_t* pinitSetting);
	int (*SLIC_SEP_DeviceVerify)(void);
	int (*SLIC_SEP_RINGImpSetup)(int cid ,int imp);
	int (*SLIC_SEP_RingProfileSetup)(int cid,int profile);
#ifdef OSBN_VOIP	
	int (*SLIC_SEP_FxoGetRingCadence)(unsigned char chan);
	int (*SLIC_SEP_FxoLineStatusGet)(_TDI_fxoLineStat_t* pFxoLineStat);
	int (*SLIC_SEP_FxoHookStatusSet)(_TDI_hookStat_t* pHookStat);
	int (*SLIC_SEP_FxoPSTNStatusGet)(unsigned char chan);
	int (*SLIC_SEP_DumpRegister_Cat)(char *page, char **start, off_t off, int count, int *eof, void *data);
	int (*SLIC_SEP_DumpRegister_Echo)(struct file *file, const char *buffer,unsigned long count, void *data);
	int (*SLIC_SEP_register_read_proc)(char *page, char **start, off_t off, int count, int *eof, void *data);
	int (*SLIC_SEP_register_write_proc)(struct file *file, const char *buffer,unsigned long count, void *data);
	int (*SLIC_SEP_linetest_cat)(char *page, char **start, off_t off, int count, int *eof, void *data);
	int (*SLIC_SEP_linetest_echo)(struct file *file, const char *buffer,unsigned long count, void *data);	
	int (*SLIC_SEP_lineteststatus)(int chan);
	int (*SLIC_SEP_linetest)(_TDI_infcLineTestParams_t*  lineTest);//mtk08176_20150508
	int (*SLIC_SEP_fxsSetMetering)(_TDI_fxsSetMetering_t* pMeteringSetting); //dyma_20140428 metering
	int (*SLIC_SEP_powerOperation)(struct ECNT_VOIP_Data *voip_data);
#endif
	int (*SLIC_SEP_EventNotify_Register)(void *data);
	int (*SLIC_SEP_RegRead)(int cid, unsigned int reg, unsigned int *value);
	int (*SLIC_SEP_RegWrite)(int cid, unsigned int reg, unsigned int data) ;
	int (*SLIC_SEP_IndregRead)(int cid, unsigned int reg, unsigned int *value);
	int (*SLIC_SEP_IndregWrite)(int cid, unsigned int reg, unsigned int data);
	int (*SLIC_SEP_IRQENABLE)(int en);
	int (*SLIC_SEP_ringState)(_TDI_ringSetting_t *pRingSetting);
	int (*SLIC_SEP_ISR)(int chan);
	int (*SLIC_SEP_FxoLIUStatusGet)(unsigned char chan);
	int (*SLIC_SEP_LoopbackEnable)(unsigned char chan, int en);
	int (*SLIC_SEP_RingParams)( _TDI_fxsSetRingParams_s *ringParams);	//mtk07059_20150423
	int (*SLIC_SEP_DcFeedParams)( _TDI_fxsSetDcFeedParams_s *tdiDcFeedParams);   //mtk07059_20150616
	int (*SLIC_SEP_SpiReset)(void);
}SLIC_FUNC_TBL;
int slic3_func_init(SLIC_FUNC_TBL* pSlicFuncTab);
void slic3_func_uninit(SLIC_FUNC_TBL* pSlicFuncTab);




