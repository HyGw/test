/*
 * le89316.h
 *
 *  Created on: Nov 30, 2010
 *      Author: pork
 */
#ifndef _LE89316DRV_H_
#define _LE89316DRV_H_

#define LE89316_CHAN_NUM 2
#define LE89316_FXS_NUM	1
#define LE89316_FXO_NUM	1

#define FXO_RING_ON 1
#define FXO_RING_OFF 0

#define FXO_LINE_CONNECTED 1
#define FXO_LINE_DISABLED 0
#define FXO_LINE_LIU 1 //Roger_130530
#define FXO_LINE_LNIU 0 //Roger_130530

#define FXO_OFF_HOOK 1
#define FXO_ON_HOOK 0

int le89316_deviceVerify(char* buf);
int le89316_deviceInit(char* buf);//int _TIC_initHw(void)
int le89316_paramReset(char* buf);// part of init
int le89316_phoneRingStart(char* buf);

int le89316_phoneRingStop(char* buf);

int le89316_eventProcess(char* buf);//void _TIC_legerityRun(vint   lineCnt)

int le89316_dumpregister(char* buf);

/*=======New function======*/


int le89316_shutdownHw(char* buf);
int le89316_fxsSetLineState(char* buf);

int le89316_fxsGetLineState(char* buf);


int le89316_fxsGetBatteryPol(char* buf);

int le89316_fxsSetBatteryPol(char* buf);

int le89316_fxsGetHookState(char* buf);

int le89316_ringState(char* buf);

int le89316_fxsGetPulseStatus(char* buf);

int le89316_fxsSetGainLv(char* buf);
/*FXO function*/
int le89316_fxoGetRingCadence(char* buf);
int le89316_fxoGetHookState(char* buf);
int le89316_fxoSetHookState(char* buf);
int le89316_fxoGetLineStats(char* buf);

int le89316_fxoGetPSTNStats(char* buf);
int le89316_fxoSetMonitorMode(char* buf);
int le89316_fxoGetLIUStats(char* buf); /*add by mtk69097*/

int le89316_LtTestStart(char* buf);//Roger_lt
int* le89316_getLtTestResult(void);//Roger_lt
int le89316_SetLoopback(char* buf);
int le89316_SetRingParam(char* buf);	//mtk07059_20150423
int le89316_SetDcFeedParam(char* buf);	//mtk07059_20150423



/**Data Strcuture****/
typedef struct le89316LineState_s{
	int fxsHookStat;
	int fxoRingStat;
	int fxoLineConn;
	int fxoHookStat;
	int fxoLiuStat;//Roger_130530
	int polRev;
}le89316LineStat_t;
#endif
