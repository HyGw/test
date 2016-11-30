/*
 * LE9641.h
 *
 *  Created on: Nov 30, 2010
 *      Author: pork
 */
#ifndef _LE9641DRV_H_
#define _LE9641DRV_H_

#define LE9641_CHAN_NUM 1
#define LE9641_FXS_NUM	1
#define LE9641_FXO_NUM	0

#define FXO_RING_ON 1
#define FXO_RING_OFF 0

#define FXO_LINE_CONNECTED 1
#define FXO_LINE_DISABLED 0

#define FXO_OFF_HOOK 1
#define FXO_ON_HOOK 0

#define LE9641_SELF_TEST  0

int le9641_deviceVerify(char* buf);
int le9641_deviceInit(char* buf);//int _TIC_initHw(void)
int le9641_paramReset(char* buf);// part of init
int le9641_phoneRingStart(char* buf);

int le9641_phoneRingStop(char* buf);

int le9641_eventProcess(char* buf);//void _TIC_legerityRun(vint   lineCnt)

int le9641_dumpregister(char* buf);

/*=======New function======*/


int le9641_shutdownHw(char* buf);
int le9641_fxsSetLineState(char* buf);

int le9641_fxsGetLineState(char* buf);


int le9641_fxsGetBatteryPol(char* buf);

int le9641_fxsSetBatteryPol(char* buf);

int le9641_fxsGetHookState(char* buf);

int le9641_ringState(char* buf);

int le9641_fxsGetPulseStatus(char* buf);

int le9641_fxsSetGainLv(char* buf);
/*FXO function*/
int le9641_fxoGetRingCadence(char* buf);
int le9641_fxoGetHookState(char* buf);
int le9641_fxoSetHookState(char* buf);
int le9641_fxoGetLineStats(char* buf);

int le9641_fxoGetPSTNStats(char* buf);
int le9641_fxoSetMonitorMode(char* buf);

int le9641_LtTestStart(char* buf);//Roger_lt
int* le9641_getLtTestResult(void);//Roger_lt 
int le9641_fxsSetMetering(char* buf); //dyma_20140428 metering
int le9641_SetLoopback(char* buf);
int le9641_SetRingParam(char* buf);	//mtk07059_20150423
int le9641_SetDcFeedParam(char* buf);	//mtk07059_20150616


/**Data Strcuture****/
typedef struct le9641LineState_s{
	int fxsHookStat;
	int fxoRingStat;
	int fxoLineConn;
	int fxoHookStat;
	int polRev;
}le9641LineStat_t;
#endif
