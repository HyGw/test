/*
 * LE9662.h
 *
 *  Created on: Nov 30, 2010
 *      Author: pork
 */
#ifndef _LE9662_1_H_
#define _LE9662_1_H_

#define LE9662_1_CHAN_NUM 2
#define LE9662_1_FXS_NUM	2
#define LE9662_1_FXO_NUM	0

#define FXO_RING_ON 1
#define FXO_RING_OFF 0

#define FXO_LINE_CONNECTED 1
#define FXO_LINE_DISABLED 0

#define FXO_OFF_HOOK 1
#define FXO_ON_HOOK 0

#define LE9662_1_SELF_TEST  1

int le9662_1_deviceVerify(char* buf);
int le9662_1_deviceInit(char* buf);//int _TIC_initHw(void)
int le9662_1_paramReset(char* buf);// part of init
int le9662_1_phoneRingStart(char* buf);

int le9662_1_phoneRingStop(char* buf);

int le9662_1_eventProcess(char* buf);//void _TIC_legerityRun(vint   lineCnt)

int le9662_1_dumpregister(char* buf);

/*=======New function======*/


int le9662_1_shutdownHw(char* buf);
int le9662_1_fxsSetLineState(char* buf);

int le9662_1_fxsGetLineState(char* buf);


int le9662_1_fxsGetBatteryPol(char* buf);

int le9662_1_fxsSetBatteryPol(char* buf);

int le9662_1_fxsGetHookState(char* buf);

int le9662_1_ringState(char* buf);

int le9662_1_fxsGetPulseStatus(char* buf);

int le9662_1_fxsSetGainLv(char* buf);
/*FXO function*/
int le9662_fxoGetRingCadence(char* buf);
int le9662_fxoGetHookState(char* buf);
int le9662_fxoSetHookState(char* buf);
int le9662_fxoGetLineStats(char* buf);

int le9662_fxoGetPSTNStats(char* buf);
int le9662_fxoSetMonitorMode(char* buf);

int le9662_1_LtTestStart(char* buf);//Roger_lt
int* le9662_1_getLtTestResult(void);//Roger_lt 
int le9662_1_fxsSetMetering(char* buf); //dyma_20140428 metering
int le9662_1_SetLoopback(char *buf);
int le9662_1_SetRingParam(char *buf);
int le9662_1_SetDcFeedParam(char* buf);


/**Data Strcuture****/
typedef struct le9662_1LineState_s{
	int fxsHookStat;
	int fxoRingStat;
	int fxoLineConn;
	int fxoHookStat;
	int polRev;
}le9662_1LineStat_t;
#endif
