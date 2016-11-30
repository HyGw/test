#ifndef __INFO_H
#define __INFO_H

#include "libtcapi.h"
#include "libcompileoption.h"

#define WANCHANGEFIFO  "/tmp/fifo_wanchange"


#define MOBILE_SAVETIME 0
#define MOBILE_TIMEDBG 0

#define MOBILE_TR069 0

#define MAXGET_PROFILE_SIZE 128
#define MAX_HTTPREQ_URL_SIZE 256

#define MOBILE_ENTRY	"mobile_Entry"
#define MOBILEBG		"debug"
#define DISTRIADDR		"MgtURL"
#define DISTRIPORT		"Port"
#define LOCALPORT		"LocatePort"
#define LOCALABILITY		"Ability"
#define SILENCEPRD		"silenceprd"
#define RESOLVEPRD		"resolveprd"

#define OPERATADDR 		"OperateSAddr"
#define OPERATPORT		"OperateSPort"
#define OPERATINTERVAL	"Heartbeat"

#define CPE_MANU_LEN 16
typedef struct cpeinfo_s{
	char sManufacturer[CPE_MANU_LEN];
	char sModel[6];
	char sSWVer[16];
	char sHWVer[16];
	char sCard[32];
}cpeinfo_t;

extern cpeinfo_t gCpeinfo;
extern char gMac[16];
extern char gSN[32];
extern char gSSN[32];
extern char gDevRNDstr[33] ;
extern char gLOID[40];
extern char gOsVer[20];
extern char gMobileVer[20];

#if _WITHOUT_IPV6_
extern char gWanIP[40]; 
extern char gIfname[10]; 
extern char gPPPname[20];
extern int gWanindex; 
#endif
#define _HAS_V4	(1 << 0)
#define _HAS_V6	(1 << 1)
#define _HAS_IP	(1 << 8)


typedef struct {
	int state; /* wanip state */
	unsigned int ipver; /* mgturl ip version */
	char indexv4;
	char indexv6;
	char wanipv4[20];
	char wanipv6[40];	
	char Ifnamev4[10];
	char Ifnamev6[10];
	char PPPv4[20];
	char PPPv6[20];
}waninfo_t;

extern waninfo_t gWaninfo;

#if EVTIMER_DBG
extern const char * sendtimename;
extern const char * maintimename;
extern const char *posttimename;
#endif

typedef enum mobile_state{
	DISTRI_UNCONNECT = 0,	
	DISTRI_TRYCONNECT,
	DISTRI_CONNECTNG,
	DISTRI_CONNECTED,
	DISTRI_CONNECTERR,
	OPERATE_UNCONNECT,
	OPERATE_TRYCONNECT,
	OPERATE_CONNECTING,
	OPERATE_STARTHB,
	OPERATE_HBGOING,
	OPERATE_CONNECTERR,
	PLUGIN_UNCONNECT,
	PLUGIN_TRYCONNECT,
	PLUGIN_CONNECTING,
	PLUGIN_STARTHB,
	PLUGIN_HBGOING,
	PLUGIN_CONNECTERR
}mobile_state_u;

typedef enum mobile_err{
	ERR_NULL = 0,
	ERR_NOSERVERSET,
	ERR_NOWANIP,
	ERR_RESLOVEIP,
	ERR_SOCKNET
}mobile_err_u;


int GetCPECommInfo(void);
int HourMinute2min(const char *time);
int updateWanIP(char *ifname, char *ipbuf, char *pppname);
int setmobileIPver(int);
char * GetmobileWanip();
char * GetmobileInterface();
char * GetmobilePPPName();
int dnsrequest(const char *url);
char * str_strip(char *in);
char * getSSN(void);
int mobile_log(int state, int errcode);
int mobile_plugin_log(int state, int errcode);
#endif /*__INFO_H*/


