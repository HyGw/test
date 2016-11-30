/*
 * le89116_1drv.h
 *
 *  Created on: Nov 11, 2010
 *      Author: pork
 */
#ifndef _LE89116_1_DRV_H_
#define _LE89116_1_DRV_H_

#define LE89116_1_FXS_NUM	1
#define LE89116_1_FXO_NUM	0

int le89116_1_deviceVerify(char* buf);
int le89116_1_deviceInit(char* buf);//int _TIC_initHw(void)
int le89116_1_paramReset(char* buf);// part of init
int le89116_1_phoneRingStart(char* buf);

int le89116_1_phoneRingStop(char* buf);

int le89116_1_eventProcess(char* buf);//void _TIC_legerityRun(vint   lineCnt)

int le89116_1_dumpregister(char* buf);

/*=======New function======*/


int le89116_1_shutdownHw(char* buf);
int le89116_1_fxsSetLineState(char* buf);

int le89116_1_fxsGetLineState(char* buf);


int le89116_1_fxsGetBatteryPol(char* buf);

int le89116_1_fxsSetBatteryPol(char* buf);

int le89116_1_fxsGetHookState(char* buf);

int le89116_1_ringState(char* buf);

int le89116_1_fxsGetPulseStatus(char* buf);

int le89116_1_fxsSetGainLv(char* buf);

int le89116_1_fxoGetRingCadence(char* buf);
int le89116_1_fxoGetHookState(char* buf);
int le89116_1_fxoSetHookState(char* buf);
int le89116_1_fxoGetLineStats(char* buf);
int le89116_1_fxoGetPSTNStats(char* buf);
int le89116_1_fxoSetMonitorMode(char* buf);
/*=====New function over=======*/

int le89116_1_LtTestStart(char* buf); //Roger_lt
int* le89116_1_getLtTestResult(void);//Roger_lt
int le89116_1_SetLoopback(char* buf);
int le89116_1_SetRingParam(char* buf);	//mtk07059_20150423
int le89116_1_SetDcFeedParam(char* buf);	//mtk07059_20150616

#endif


