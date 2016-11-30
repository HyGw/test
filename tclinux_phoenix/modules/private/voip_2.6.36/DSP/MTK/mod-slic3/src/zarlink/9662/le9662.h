/*
 * LE9662.h
 *
 *  Created on: Nov 30, 2010
 *      Author: pork
 */
#ifndef _LE9662_H_
#define _LE9662_H_

#define LE9662_CHAN_NUM 2
#define LE9662_FXS_NUM	2
#define LE9662_FXO_NUM	0

#define FXO_RING_ON 1
#define FXO_RING_OFF 0

#define FXO_LINE_CONNECTED 1
#define FXO_LINE_DISABLED 0

#define FXO_OFF_HOOK 1
#define FXO_ON_HOOK 0

#define LE9662_SELF_TEST  1

int le9662_deviceVerify(char* buf);
int le9662_deviceInit(char* buf);//int _TIC_initHw(void)
int le9662_paramReset(char* buf);// part of init
int le9662_phoneRingStart(char* buf);

int le9662_phoneRingStop(char* buf);

int le9662_eventProcess(char* buf);//void _TIC_legerityRun(vint   lineCnt)

int le9662_dumpregister(char* buf);

/*=======New function======*/


int le9662_shutdownHw(char* buf);
int le9662_fxsSetLineState(char* buf);

int le9662_fxsGetLineState(char* buf);


int le9662_fxsGetBatteryPol(char* buf);

int le9662_fxsSetBatteryPol(char* buf);

int le9662_fxsGetHookState(char* buf);

int le9662_ringState(char* buf);

int le9662_fxsGetPulseStatus(char* buf);

int le9662_fxsSetGainLv(char* buf);
/*FXO function*/
int le9662_fxoGetRingCadence(char* buf);
int le9662_fxoGetHookState(char* buf);
int le9662_fxoSetHookState(char* buf);
int le9662_fxoGetLineStats(char* buf);

int le9662_fxoGetPSTNStats(char* buf);
int le9662_fxoSetMonitorMode(char* buf);

int le9662_LtTestStart(char* buf);//Roger_lt
int* le9662_getLtTestResult(void);//Roger_lt 
int le9662_fxsSetMetering(char* buf); //dyma_20140428 metering
int le9662_SetLoopback(char *buf);
int le9662_SetRingParam(char *buf);
int le9662_SetDcFeedParam(char* buf);


/**Data Strcuture****/
typedef struct le9662LineState_s{
	int fxsHookStat;
	int fxoRingStat;
	int fxoLineConn;
	int fxoHookStat;
	int polRev;
}le9662LineStat_t;
#endif
