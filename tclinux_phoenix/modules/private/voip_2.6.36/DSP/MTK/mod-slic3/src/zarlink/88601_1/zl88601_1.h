/*
 * ZL88601.h
 *
 *  Created on: Nov 30, 2010
 *      Author: pork
 */
#ifndef _ZL88601DRV_H_
#define _ZL88601DRV_H_

#define ZL88601_1_CHAN_NUM 2
#define ZL88601_1_FXS_NUM	2
#define ZL88601_1_FXO_NUM	0

#define FXO_RING_ON 1
#define FXO_RING_OFF 0

#define FXO_LINE_CONNECTED 1
#define FXO_LINE_DISABLED 0

#define FXO_OFF_HOOK 1
#define FXO_ON_HOOK 0

#define ZL88601_1_SELF_TEST  0

int zl88601_1_deviceVerify(char* buf);
int zl88601_1_deviceInit(char* buf);//int _TIC_initHw(void)
int zl88601_1_paramReset(char* buf);// part of init
int zl88601_1_phoneRingStart(char* buf);

int zl88601_1_phoneRingStop(char* buf);

int zl88601_1_eventProcess(char* buf);//void _TIC_legerityRun(vint   lineCnt)

int zl88601_1_dumpregister(char* buf);

/*=======New function======*/


int zl88601_1_shutdownHw(char* buf);
int zl88601_1_fxsSetLineState(char* buf);

int zl88601_1_fxsGetLineState(char* buf);


int zl88601_1_fxsGetBatteryPol(char* buf);

int zl88601_1_fxsSetBatteryPol(char* buf);

int zl88601_1_fxsGetHookState(char* buf);

int zl88601_1_ringState(char* buf);

int zl88601_1_fxsGetPulseStatus(char* buf);

int zl88601_1_fxsSetGainLv(char* buf);
/*FXO function*/
int zl88601_fxoGetRingCadence(char* buf);
int zl88601_fxoGetHookState(char* buf);
int zl88601_fxoSetHookState(char* buf);
int zl88601_fxoGetLineStats(char* buf);

int zl88601_fxoGetPSTNStats(char* buf);
int zl88601_fxoSetMonitorMode(char* buf);

int zl88601_1_LtTestStart(char* buf);//Roger_lt
int* zl88601_1_getLtTestResult(void);//Roger_lt 
int zl88601_1_fxsSetMetering(char* buf); //dyma_20140428 metering
int zl88601_1_SetLoopback(char* buf);
int zl88601_1_SetRingParam(char* buf);	//mtk07059_20150423 
int zl88601_1_SetDcFeedParam(char* buf);	//mtk07059_20150616

/**Data Strcuture****/
typedef struct zl88601_1_LineState_s{
	int fxsHookStat;
	int fxoRingStat;
	int fxoLineConn;
	int fxoHookStat;
	int polRev;
}zl88601_1_LineStat_t;
#endif
