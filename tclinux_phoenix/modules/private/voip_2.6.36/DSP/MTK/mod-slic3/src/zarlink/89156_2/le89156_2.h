/*
 * le89156_2drv.h
 *
 *  Created on: Nov 11, 2010
 *      Author: pork
 */
#ifndef _LE89156_2DRV_H_
#define _LE89156_2DRV_H_

#define LE89156_2_FXS_NUM	1
#define LE89156_2_FXO_NUM	0

int le89156_2_deviceVerify(char* buf);
int le89156_2_deviceInit(char* buf);//int _TIC_initHw(void)
int le89156_2_paramReset(char* buf);// part of init
int le89156_2_phoneRingStart(char* buf);

int le89156_2_phoneRingStop(char* buf);

int le89156_2_eventProcess(char* buf);//void _TIC_legerityRun(vint   lineCnt)

int le89156_2_dumpregister(char* buf);

/*=======New function======*/


int le89156_2_shutdownHw(char* buf);
int le89156_2_fxsSetLineState(char* buf);

int le89156_2_fxsGetLineState(char* buf);


int le89156_2_fxsGetBatteryPol(char* buf);

int le89156_2_fxsSetBatteryPol(char* buf);

int le89156_2_fxsGetHookState(char* buf);

int le89156_2_ringState(char* buf);

int le89156_2_fxsGetPulseStatus(char* buf);

int le89156_2_fxsSetGainLv(char* buf);

int le89156_2_fxoGetRingCadence(char* buf);
int le89156_2_fxoGetHookState(char* buf);
int le89156_2_fxoSetHookState(char* buf);
int le89156_2_fxoGetLineStats(char* buf);
int le89156_2_fxoGetPSTNStats(char* buf);
int le89156_2_fxoSetMonitorMode(char* buf);
/*=====New function over=======*/
int le89156_2_LtTestStart(char* buf);//Roger_lt
int* le89156_2_getLtTestResult(void);//Roger_lt
int le89156_2_SetLoopback(char* buf);
int le89156_2_SetRingParam(char* buf);	//mtk07059_20150423
int le89156_2_SetDcFeedParam(char* buf);	//mtk07059_20150616


#endif


