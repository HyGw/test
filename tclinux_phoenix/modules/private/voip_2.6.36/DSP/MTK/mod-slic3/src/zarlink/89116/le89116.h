/*
 * le89116drv.h
 *
 *  Created on: Nov 11, 2010
 *      Author: pork
 */
#ifndef _LE89116DRV_H_
#define _LE89116DRV_H_

#define LE89116_FXS_NUM	1
#define LE89116_FXO_NUM	0

int le89116_deviceVerify(char* buf);
int le89116_deviceInit(char* buf);//int _TIC_initHw(void)
int le89116_paramReset(char* buf);// part of init
int le89116_phoneRingStart(char* buf);

int le89116_phoneRingStop(char* buf);

int le89116_eventProcess(char* buf);//void _TIC_legerityRun(vint   lineCnt)

int le89116_dumpregister(char* buf);

/*=======New function======*/


int le89116_shutdownHw(char* buf);
int le89116_fxsSetLineState(char* buf);

int le89116_fxsGetLineState(char* buf);


int le89116_fxsGetBatteryPol(char* buf);

int le89116_fxsSetBatteryPol(char* buf);

int le89116_fxsGetHookState(char* buf);

int le89116_ringState(char* buf);

int le89116_fxsGetPulseStatus(char* buf);

int le89116_fxsSetGainLv(char* buf);

int le89116_fxoGetRingCadence(char* buf);
int le89116_fxoGetHookState(char* buf);
int le89116_fxoSetHookState(char* buf);
int le89116_fxoGetLineStats(char* buf);
int le89116_fxoGetPSTNStats(char* buf);
int le89116_fxoSetMonitorMode(char* buf);
/*=====New function over=======*/
int le89116_LtTestStart(char* buf);//Roger_lt
int* le89116_getLtTestResult(void);//Roger_lt 
int le89116_SetLoopback(char* buf);
int le89116_SetRingParam(char* buf);	//mtk07059_20150423
int le89116_SetDcFeedParam(char* buf);	//mtk07059_20150616

#endif


