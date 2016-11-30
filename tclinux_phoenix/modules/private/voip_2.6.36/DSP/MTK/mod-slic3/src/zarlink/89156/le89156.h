/*
 * le89156drv.h
 *
 *  Created on: Nov 11, 2010
 *      Author: pork
 */
#ifndef _LE89156DRV_H_
#define _LE89156DRV_H_

#define LE89156_FXS_NUM	1
#define LE89156_FXO_NUM	0

int le89156_deviceVerify(char* buf);
int le89156_deviceInit(char* buf);//int _TIC_initHw(void)
int le89156_paramReset(char* buf);// part of init
int le89156_phoneRingStart(char* buf);

int le89156_phoneRingStop(char* buf);

int le89156_eventProcess(char* buf);//void _TIC_legerityRun(vint   lineCnt)

int le89156_dumpregister(char* buf);

/*=======New function======*/


int le89156_shutdownHw(char* buf);
int le89156_fxsSetLineState(char* buf);

int le89156_fxsGetLineState(char* buf);


int le89156_fxsGetBatteryPol(char* buf);

int le89156_fxsSetBatteryPol(char* buf);

int le89156_fxsGetHookState(char* buf);

int le89156_ringState(char* buf);

int le89156_fxsGetPulseStatus(char* buf);

int le89156_fxsSetGainLv(char* buf);

int le89156_fxoGetRingCadence(char* buf);
int le89156_fxoGetHookState(char* buf);
int le89156_fxoSetHookState(char* buf);
int le89156_fxoGetLineStats(char* buf);
int le89156_fxoGetPSTNStats(char* buf);
int le89156_fxoSetMonitorMode(char* buf);
/*=====New function over=======*/
int le89156_LtTestStart(char* buf);//Roger_lt
int* le89156_getLtTestResult(void);//Roger_lt
int le89156_SetLoopback(char* buf);
int le89156_SetRingParam(char* buf);	//mtk07059_20150423
int le89156_SetDcFeedParam(char* buf);	//mtk07059_20150616


#endif


