
/*
** $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/private/TR69_64/cwmp/Includes/cwmp.h#1 $
*/
/************************************************************************
 *
 *	Copyright (C) 2005 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/
#ifndef __CWMP_H__
#define __CWMP_H__
//#include "AsTypes.h"
//#include "AsEngine.h"
//#include "WcProtos.h"
//#include "Global.h"
//#include "wcClient.h"

//#include "Global_res.h"



/************************************************************************
*                        			MACRO							
************************************************************************
*/

#define CwmpInformPeriodOndDay "86400"     //sec
#define CwmpURLMaxLen 256
#define CwmpWebClientBufferSize 1600 //512->1024->1600 ,hqfeng,2006.03.02

#define CWMP_REDUCE_CODE  0  //Added for reduce code, hqfeng, 2006.08.07

#define CWMP_MAGIC_NUMBER 6964

#define CWMP_FIRMWARE 1
#define CWMP_WEB_CONTENT 2
#define CWMP_CONFIGURATION_FILE 3

#define CWMP_SYSLOG_FILE 4

#define CWMP_MAC2NAME_DISABLE			0
#define CWMP_MAC2NAME_ENABLE			1
#define CWMP_LOAD_DEFAULT_FLAG_CLEAR	0
#define CWMP_LOAD_DEFAULT_FLAG_SET		1

/*add by sam 20080707 begin*/
#define TR69_OUI_LEN 3
/*add by sam 20080707 end*/

/*add by brian====begin*/
//pthread_mutex_t value_change_lock;

#define SYSROUTE_CMD "/sbin/route -n > /tmp/cwmproute"
#define SYSROUTE_PATH "/tmp/cwmproute"
/*add by brian====end*/

#define NASNAME "nas"
#define PPPNAME "ppp"
#define PVCFLAG "UG"
	
//add by bria for atm f5 loopback diagnostic
#define ATM_F5LOOPBACK_DIAGNOSTIC_PATH "/proc/tc3162/atm_f5_loopback_diagnostic"
#define ATM_F5LOOPBACK_DIAGNOSTIC_RESET_PATH "/proc/tc3162/atm_f5_loopback_reset_diagnostic"

extern uint8 redirectcloseses;

//modify by brian
//#if defined(CSE_TANGO) ||defined (SZTELE_WORKSSYS)||defined(MAX_TR69_PARAM)
typedef enum
{
	eDiagnosticsStateNone,
	eDiagnosticsStateComplete,
	eDiagnosticsStateRequested,
	eDiagnosticsStateError_internal,
	eDiagnosticsStateError_other
} F5DiagnosticsState;
typedef struct atmF5LoopbackDiagnostis_s{
	F5DiagnosticsState DiagnosticsState;
	uint32 NumberOfRepetitions;
	uint32 Timeout;
	uint32 SuccessCount;
	uint32 FailureCount;
	uint32 AverageResponseTime;
	uint32 MinimumResponseTime;
	uint32 MaximumResponseTime;
} atmF5LoopbackDiagnostis_t;
//#endif
/*
	TransferComplete (0x01)
	InternalError = 9002 (0x01<<1)
	DownloadFailure = 9010 (0x01<<2)
	FileTransferAuthenFailure = 9012 (0x01<<3)
	UnsupportedProtocol = 9013 (0x01<<4)
*/
typedef enum
{
    eTcapiSyncMode = 0,
    eTcapiAsyncMode = 1,
    eTcapiDelayMode = 2,
} tr069TcapiMode;

#define EVENT_NoTransferComplete (0x00)
#define EVENT_TransferComplete (0x01)
#define TransferComplete_Error9002  (0x01<<1)
#define TransferComplete_Error9010 (0x01<<2)
#define TransferComplete_Error9012  (0x01<<3)
#define TransferComplete_Error9013 (0x01<<4)
#define TransferComplete_Error9011 (0x01<<5)
#define TransferComplete_Error9018 (0x01<<6)
#define ErrorCodeMask (0x7e)      //ErrorCodeMask (0x1e)

extern uint8 dbg_cwmp;
extern pthread_mutex_t midware_task_lock;
extern sem_t SEM_MW_IMFORM;
extern int iTr069Enable;
extern uint8 MWValueChangeFlag;
extern uint8 MWActiveNotifyFlag;
extern uint8 ITMSActiveNotifyFlag;
extern uint8 MWExitFlag;
extern pthread_t p_MonitorCollector;
extern uint8 button_detect_flag[2];
#define MAX_ATTRIBUTE_LEN	256
void checkAndDoMonitorCollectorTask(void);
uint8 GetTr069Enable();
void setTr069Enable(uint8 value);
int8   RebuildAlarmTable();
#if defined(TCSUPPORT_CT)
void setCwmpInformTaskStart(int value);
#endif
int nmInitCi(void);
void cwmpInit(void);
char * getCwmpUrl(void);
//char* cwmpGetURL(void);
int setCwmpUrl(char *value);
char * getCwmpPath(void);
void  setCwmpPath(char *value);//added by hqfeng,2006.06.06
int  getCwmpPort(void);
void  setCwmpPort(int value);//added by hqfeng,2006.06.06
char * getCwmpManufacture(void);
char * getCwmpOUI(void);
char * getCwmpProductClass(void);
char * getCwmpSerialNum(void);
char * getHardwareVer(void);
char * getUpDateTime(void);
char * getDefaultWANExternalIPAddr(void);
char * getConRequestURL(void);
//void cwmpResetRequestBuffer(wcRequestPtr);
char * getCwmpUserName(void);
char * getCwmpPassword(void);
uint8 cwmpGetAuthenFlag(void);
void cwmpSetAuthenFlag(uint8 value);
void cwmpResetAuthenFlag(void);

char * getModemFirmwareVer(char* fwVer);

#ifndef CODE_SHRINK  //qingpu.20080110
uint8 cwmpGetSerialNumFlag(void);
#endif
int cwmpGetInformTimerID(void);
void cwmpSetInformTimerID(int value);
uint32 GetInformTaskID(void);
void cwmpBootInform(void);
//added by xyzhu_20091209

void wifiMacTabChanged(void);
void wifiMacTabTimerStart(void);

void cwmpPeriodInform(void);
int cwmpBuildInformPkt(void);
int cwmpInformTaskStart(void);

void *cwmpInformTask(void);

#ifndef THREAD_REDUCE
int cwmpValueChangeTaskStart(void);
//modify by brian
void *cwmpValueChangeTask(void);
uint32 GetValueChangeTaskID(void);
#else
int cwmpValueChangeRoutine(void);
#endif
/*add by robbie.jin*/
int cwmpAsynTcapiTaskStart(void);
void *cwmpAsynTcapiTask(void);

boolean getCwmpInitFlag(void);
void setCwmpInitFlag(boolean value);

int8 getforceInformFlag(void);
void setforceInformFlag(int8 value);

void  setInformPeriod(uint32 value);
uint32  getInformPeriod(void);

void  setInformPeriodEnable(boolean value);
boolean  getInformPeriodEnable(void);

#if defined(TCSUPPORT_RANDOM_INFORM)
boolean getRandomInformFlag(void);
#endif

uint8 getIP_BridgedTypeFlag(void);
void setIP_BridgedTypeFlag(uint8 value);

int cwmpSetCwmpUrl_CT(char * URL);
int cwmpSetCwmpUrl(char * URL,int flag);

int cwmpSetRedirectCwmpUrl(char *URL);
char* GetCwmpRedirectURL(void);
uint8 GetCwmpRedirectCnt(void);
void cwmpCalculateReirectCnt(void);
void ClearCwmpRedirectCnt(void);
void SetCwmpRedirectFlag(uint8 flag);
uint8 GetCwmpRedirectFlag(void);
uint8 GetCwmpRedirectHttpFlag(void);
uint32 GetCwmpRedirectHostAddress(void);
uint16 GetCwmpRedirectPort(void);
char* GetCwmpRedirectPath(void);
char* GetCwmpRedirectHostName(void);
uint8 GetCwmpRedirectConnReqFlag(void);
void SetCwmpRedirectConnReqFlag(uint8 value);

uint8 cwmpCheckValidURL(char* URL);

void setCPEPath(char* value);
char* getCPEPath(void);

int setCPEPort(uint32 value);
uint32 getCPEPort(void);

char* getCPEUserName(void);
void setCPEUserName(char* value);

char* getDeviceregUserName(void);
char* getDeviceregUserPasswd(void);

char *getECNDeviceType();
char *getECNAccessType();
char *getLoid(void);

char* getCPEPassword(void);
void setCPEPassword(char* value);

uint8 GetRetryCount(void);

#ifndef CODE_SHRINK  //qingpu.20080110
void SetRetryCount(uint8 value);

uint8 getWaitingReceiveFlag(void);
void setWaitingReceiveFlag(uint8 value);

uint8 getCwmpLanTestFlag(void);
void setCwmpLanTestFlag(uint8 value);
#endif

//void cwmpResetSession(wcHttpSessionPtr  theSessionPtr);

#ifndef CODE_SHRINK  //qingpu.20080110
uint8 cwmpGetSessionCompleteFlag(void);
void cwmpSetSessionCompleteFlag(uint8 value);
#endif

//modify by brian
int cwmpPerformWANADSLDiagnostics(void);
int cwmpPerformF5LoopbackDiagnostics(void);

uint8 cwmpGetF5LoopbackDiagnosticsResetFlag(void);
void cwmpSetF5LoopbackDiagnosticsResetFlag(uint8 value);

uint8 cwmpGetF5LoopbackDiagnosticsWannodeindex(void);
void cwmpSetF5LoopbackDiagnosticsWannodeindex(uint8 value);

uint8 cwmpGetF5LoopbackDiagnosingFlag(void);
void cwmpSetF5LoopbackDiagnosingFlag(uint8 value);

uint8 cwmpGetF5LoopbackDiagnosticsWannodeindex(void);
void cwmpSetF5LoopbackDiagnosticsWannodeindex(uint8 value);

void cwmpSetF5LoopbackDiagnosticsNumOfRep(uint32 NumOfRepetions, uint8 wannode);
void cwmpSetF5LoopbackDiagnosticsTimeout(uint32 Timeout, uint8 wannode);

#ifdef TR143
int cwmpPerformDownloadDiagnostics(void);
uint8 cwmpInitDiagnosticsStruct(void);
#endif
/* perform download test via wget. */
void cwmpPerformWgetDownloadTask(void);
/* perform upload test via curl. */
void cwmpPerformCurlUploadTask(void);

/*TCSUPPORT_PON_ROSTELECOM begin*/
void cwmpPerformWgetFirmwareDownloadTask();
int8 cwmpGetWgetFileTransferArgument(char* url);
/*TCSUPPORT_PON_ROSTELECOM end*/

int  getUnLockRealmsUserFlag(void);
void setUnLockRealmsUserFlag(int value);

uint8 getHttpsFlag(void);
void setHttpsFlag(uint8 value);
//uint8 getSessionClearFlag(void);
//void setSessionClearFlag(uint8 value);
uint8 getSendHostPortNumFlag(void);
void setSendHostPortNumFlag(uint8 value);

char* getParameterKey(void);
void cwmpResetSessionRetryWaitInterval(void);
void cwmpSetSessionRetryFlag(int value);
void cwmpDoSessionRetry(void);
void cwmpReInitHostTableTaskStart();

#ifndef CODE_SHRINK  //qingpu.20080110
int getGetRPCMethodsFlag(void);
int setGetRPCMethodsFlag(int value);

uint8 getInitEmptyPktFlag(void);
void setInitEmptyPktFlag(uint8 value);
#endif

void cwmpSetNotifyFlag(uint8 value);
uint8 cwmpGetNotifyFlag(void);

void cwmpSetSpSaveFlag(uint8 value);
uint8 cwmpGetSpSaveFlag(void);

char* getProvisionCode(void);
int setProvisionCode(char * value);

uint8 getCwmpSwitch(void);
void setCwmpSwitch(uint8 value);
uint8 SetDownloadErrType(uint8 flag);
uint8 GetDownloadErrType(void);

uint8 SetUploadErrType(uint8 flag);
uint8 GetUploadErrType(void);

uint8 SetFileTransferCompleteFlag(uint8 flag);
uint8 GetFileTransferCompleteFlag(void);
uint8 SetFileTransferStartTime(uint32 start);
uint32 GetFileTransferStartTime(void);
uint8 SetFileTransferCompleteTime(uint32 end);
uint32 GetFileTransferCompleteTime(void);
uint8 setDownloadStartFlag(uint32 value);
uint32 getDownloadStartFlag(void);
uint8 getDownloadContinueFlag(void);
void setDownloadContinueFlag( uint8 value);
uint8 SetFileTransferStartTime_improve();
uint8 SetFileTransferCompleteTime_improve();
char * getCurrentTime(void);
uint8 SetCommandKey(char* value, int index);
char* GetCommandKey(int index);

uint8 getWANIndex(void);  //qingpu,20070920
char * getWANExternalName(uint8 index);

char * getWANMacName(uint8 index);
char * getCPEMac(int index);

//void  getWANExternalIP(int node, char* externalIP);
void  getWANSubnetMask(int node, char* subnet);
void  getWANExternalGateway(int node, char* externalGateway);
//void  getLANIP(char* LANIPStr);

void cwmpCheckIPValid(void);

//For Session Retry Policy
int  cwmpSessionRetryWaitIntervalCounter(void);
void cwmpSessionResetRetryWaitInterval(void);
int cwmpSessionGetRetryWaitInterval(void);
int cwmpSessionGetRetryFlag(void);
void cwmpSessionSetRetryFlag(int value);
int cwmpGetSessionRetryCount(void);
void cwmpCalculateSessionRetryCount(int value);
void cwmpSessionRetryTask(void);
void cwmpSetInformCompleteFlag(int value);
int cwmpGetInformCompleteFlag(void);

void cwmpResetInformFlag(void);

void cwmpSetCurACSInfo(uint32 theHostAddress, uint16 thePort, char* thePath);
void cwmpGetCurACSInfo(uint32 *theHostAddressP, uint16 *thePortP, char *thePath);

boolean  getHoldRequestFlag(void);
void  setHoldRequestFlag(boolean  value);

#if 	CWMP_USING_MBUF_FOR_TXBUF == 0 //qingpu.20080110
boolean  getPreAllocTxBufFlag(void);
void  setPreAllocTxBufFlag(boolean  value);
#endif

char* cwmp_strdup(char * str);

#if 	CWMP_USING_MBUF_FOR_TXBUF == 0
int cwmpInitTxBuf(void);
#endif
int cwmpConfigInit(void);

uint8 cwmpGetDownloadFileType(void);
char* cwmpGetDownloadTargetFileName(void);
uint8 cwmpSupportDownloadFile(char* downloadFileName);

char*  getCapitalStr(char* str);


void cwmpConfigSSLCA(void);

char* getCustomerCA(void);
void cwmpSignalTimerInform(uint32 time);
void cwmpSignalScheduleInform(unsigned int delaySeconds);
void cwmpSignalInform(void);


#ifdef WLAN_RT2561_SUPPORT	
uint32 getCwmpWLANTaskId(void);
#endif

uint32 getCwmpReInitHostTableTaskId(void); //qingpu.20071129

char* getDeviceSummary(void);
void cwmpWLANTask(void);

#if 	CWMP_USING_MBUF_FOR_TXBUF == 0
uint32 getTxBufLenFromMMCT(void);
#endif

/*added by hqfeng, 2006.09.14*/
#if defined(TCSUPPORT_CT)
int getUserCustomizeFlag(void);
#endif
int  getGetFromBootbaseFlag(void);
void  setGetFromBootbaseFlag(int  value);
char* getManufacturer(void);
int  setManufacturer(char *value);
#if defined(TCSUPPORT_CT)
char* getSerialNumValue(void);
int  setSerialNumValue(char *value);
#endif
char* getModelName(void);
int  setModelName(char *value);
char* getDescription(void);
int  setDescription(char *value);
char* getProductClass(void);
int  setProductClass(char *value);

char * getCwmpModelName(void);

uint8 getIPPingCompleteFlag(void);
void setIPPingCompleteFlag(uint8 value);
uint8 getDownloadRomFileFlag(void);
void setDownloadRomFileFlag(uint8 value);

//uint8 getInitialDeviceRegisterFlag(void);
int getDevregInformFlag(void);
void setDevregInformFlag(int flag);

int getLongResetBtnFlag();
void setLongResetBtnFlag(int flag);

#define SIM_CARD_NODE_NAME "SimCard_Entry"
char* getSimCardCardNo(void);

uint8 getSimCardFirstBootFlag();	

uint8 getSimFileNeedModifyFlag(void);
void setSimFileNeedModifyFlag(uint8 value);


uint8 getSimCardAuthFlag(void);
void setSimCardAuthFlag(uint8 value);

uint8 getCardNotifyFlag(void);
void setCardNotifyFlag(uint8 value);

uint8 getCardWriteFlag(void);
void setCardWriteFlag(uint8 value);

int isCardExist(void);
int isSimCardAuthSuccess();


#ifdef TR143
//uint8 getDiagnosticsCompleteFlag(void);
void setDiagnosticsCompleteFlag(uint8 value);
uint8 cwmpInitDiagnosticsStruct(void);
#define MAXBUFLEN 		100
#define UDPHEADERLEN 	20
#define PACKNUM			10

uint8 *put32( uint8	*cp, uint32	x);
int getServerIPFromInterface( char *serverIP );
int checkUDPEchoEnable( void );
int checkUDPEchoPlusEnable( void );
int getUDPEchoPort( void );
char *getUDPEchoSourceIP( void );
void clearUDPEchoStats( void );
void UDPEchoStart( void );
void closeUDPSocket( void );
void UDPEchoRecvSendProc( void );

#endif

uint8 getConnRequestAuthFlag(void);
void setConnRequestAuthFlag(uint8 value);

uint8 getNoArrayTypeFlag(void);
void setNoArrayTypeFlag(uint8 value);
boolean getFaultDetailFlag(void);
void setFaultDetailFlag(boolean value);

uint8 getFactoryResetFlag(void);
void setFactoryResetFlag(uint8 value);

char* cwmpGetFileNameFromDownloadURL(void);

uint8 getCwmpUpgradeManaged(void);

void setCwmpConnectionIndex(uint8 index);
uint8 getCwmpConnectionIndex(void);

int getFirmwareVersionFlag(void);
void setFirmwareVersionFlag(uint8 value);

char* getCustomerHardwareVer(void);
int  setCustomerHardwareVer(char *value);

//added for ScheduleInform RPC, hqfeng, 2007.01.08
uint8 getScheduleInformFlag(void);
void setScheduleInformFlag(uint8 value);

void cwmpPeriodicInformTime(void);
int setDefaultPeriodicInformTime(void);
boolean checkPeriodicInformTime(void);
//Boolean checkPeriodicInformTime(void);
char* getPeriodicInformTime(void);
void setPeriodicInformTime(char *strPITValue);
boolean isPITUnknownValue(char *strTime);
//Boolean isPITUnknownValue(char *strTime);
uint8 Get_ACS_PVC_Number(void);

//#ifdef RANDOM_INFORM
#if defined(TCSUPPORT_RANDOM_INFORM)
void cwmpStartInform(void);
#endif

uint8 getMotiveRSTFlag(void);
void setACSAuthSuccess(void);

#ifdef SSL
uint8 get_ssl_check_mem_flag(void);
void set_ssl_check_mem_flag(uint8 value);
#endif

//add by brian
char* getCwmpHostName(void);
void setCwmpHostName(char* value);

void setcwmpAcsUrl(char* value);
char* getcwmpAcsUrl(void);
void setcwmpPrdInformTime(char *value);
char* getcwmpPrdInformTime(void);
#if defined(TCSUPPORT_CT)
#define 	MAX_IF_NUM					8
#define		MAX_WAN_INFO_ENTRY_NUM		64
#define		MAX_PVC_NUM					8

/* support qos and service list */
/*Mark for define in rule.mak*/
//#define		CT_COM
//#define		CT_COM_DBUG

#define		INTERNET			"INTERNET"
#define		TR069          	 	"TR069"
#define		TR069_INTERNET   	"TR069_INTERNET"
#define     OTHER               "OTHER"
#define 	TR069_ROUTE_CONF			"/var/run/cwmp_route"
#define 	MAX_APP_OFFSET				10
#define 	MAX_APP_NUM					4
int update_cwmp_route(uint32 acsHostAddress);
void update_qos_rule(void);
int get_ppp_username(int if_index, char *conn_name, char *user_name);
int isPPPConnectionExist(void);
#if defined(TCSUPPORT_CT_FJ)	
int get_ErrorsSent(int if_index,char *nodeName, char *tempValue,char *attr,char *end_name);
int get_StatsEthernetInfo(int if_index,char *nodeName, char *tempValue, char *attr, char *end_name);
int get_ppp_uptime(int if_index, char *nodeName, char *tempValue);
#endif
#if 1   //IPPING_DIAGNOSTICS
int isInRouteTable();
int get_wanindex_by_name(char *wan_if_name);
int get_entry_number(char *buffer, char *keyword, int *number);
#endif

int cwmpCheckWanIP(void);
int checkDNSExist(void);

#define ALARM_NODENAME		"DeviceAlarm"
#define ALARM_COMMON		"DeviceAlarm_Common"
#define ALARM_ENTRY		"DeviceAlarm_Entry%d"

#define MONITOR_NODENAME	"DeviceMonitor"
#define MONITOR_COMMON		"DeviceMonitor_Common"
#define MONITOR_ENTRY		"DeviceMonitor_Entry%d"

#define ACCUMULATE_MODE		1/*accumulated value*/
#define AVERAGE_MODE		2/*average value*/
#define BURST_MODE		3/*burst value*/
#define TIMECHANGERANGE		600/*10 minutes*/
#if defined(TCSUPPORT_CT_E8GUI)
#define MAXLEN_TCAPI_MSG		1024
#else
#define MAXLEN_TCAPI_MSG		576
#endif

extern DeviceAlarmNodeInfoPtr gAlarmNodeList;
extern uint8 gAlarmEntryNum;

extern DeviceMonitorNodeInfoPtr gMonitorNodeList;
extern uint8 gMonitorEntryNum;

int checkAlarmEnable(void);
int checkAlarmEntryNum(void);
int checkMonitorEnable(void);
int checkMonitorEntryNum(void);

void setDeviceAlarmFlag(boolean value);
boolean getDeviceAlarmFlag(void);
void setDeviceClearAlarmFlag(boolean value);
boolean getDeviceClearAlarmFlag(void);
void setDeviceMonitorFlag(boolean value);
boolean getDeviceMonitorFlag(void);

int getSimCardRstFailAlarmFlag();
void setSimCardRstFailAlarmFlag(int flag);

extern int iUPnPDMModifyFlag;
extern int doUPnPRoutineActionList();
int cwmpUPnPDMRoutine(void);
void setUPnPDMDownloadFlag(boolean value);
boolean getUPnPDMDownloadFlag(void);

enum wan_action_type{
	WAN_TYPE_NOCARE = 0,
	IP_WAN_TYPE,
	PPP_WAN_TYPE,
	WAN_MAX_ACTION_TYPE,
};

enum wan_dev_action{
	WAN_DEV_ADD = 1,
	WAN_DEV_DEL,
	WAN_DEV_MAX_ACTION,
};

enum paraList_wan_item{
	wan_ConnectionStatus = 0,
	wan_LastConnectionError,
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
	wan_IPv6ConnectionStatus,
#if defined(TCSUPPORT_CT_PON_GDV20)
	wan_LastConnectionErrorCode,
#else
	wan_ConnectionType,
#endif
#endif
	max_paraList_wan_item,
};


#define MONITOR_COLLECTOR_NODENAME 		"MonitorCollector"
#define MONITOR_COLLECTOR_COMMON 		"MonitorCollector_Common"
#define MONITOR_COLLECTOR_ENTRY 		"MonitorCollector_Entry%d"

#define MONITOR_COLLECTOR_TFTP_PATH 	"/usr/script/tftp.sh"

#define MONITOR_COLLECTOR_DIR 			"/var/tmp/monitorCollector/"
#define MONITOR_COLLECTOR_PATH_JX		"/var/tmp/monitorCollector/%d/%s_%s_%s-status-%d-%02d-%02d-%02d-%02d-%02d.txt"
#define MONITOR_COLLECTOR_PATH 			"/var/tmp/monitorCollector/%d/%s-status-%d-%02d-%02d-%02d-%02d-%02d.txt"
#define MONITOR_COLLECTOR_CONF_PATH 	"/tmp/monitorCollector.conf"

#define MONITOR_COLLECTOR_MAXNUM	5

extern MonitorCollectorNodeInfoPtr gMonitorCollectorNodeList;
extern uint8 gMonitorCollectorEntryNum;

int checkMonitorCollectorEnable(void);
int checkMonitorCollectorEntryNum(void);

void setMonitorCollectorFlag(boolean value);
boolean getMonitorCollectorFlag(void);
#endif

int cwmpGetFileType(void);
int isDslOpmodeChange();
uint8 getAlarmBuildFlag(void);
void setAlarmBuildFlag(uint8 value);
int getISPIndex(void);

boolean flagAddWanPPPoEUserName(uint8 index);
char * getWANPPPoEUserName(uint8 index);
int cwmpDealWithFaultCode(int errorCode);
int isInRouteTable(char *ip,char *dev);

//#define TCSUPPORT_CT_WANNINDEX_INCREASE 1
#define TOTAL_REMOTE_DEVICE_NODE 	8
#define TOTAL_REMOTE_ENTRY_NODE 	8

void dumpUsedWanIndex();
void initUsedWanIndex();
int findUnusedPvcIndex();
int findUnUsedEntryIndex(int pvcIndex);

uint8 getNameChangeFlag(void);
void setNameChangeFlag(uint8 value);
uint8 getLoidChangeFlag(void);
void setLoidChangeFlag(uint8 value);
uint8 getLoidEmptyFlag(void);
void setLoidEmptyFlag(uint8 value);

int getSTBListNotifyFlag();
int setSTBListNotifyFlag(int flag);
int updateSTBList();
char *getSTBMACList();
int getSTBCount();
char *getSTBCountStr();
char *getIGMPEnableStr();
char *getProxyEnableStr();
char *getSnoopingEnableStr();

char *getBrpppusername(void);
int get_ppp_connectiontype(int if_index, char *conn_type, char *type_name);

int getXBIND1NotifyFlag(void);
int setXBIND1NotifyFlag(int flag);
int8 cwmpBuildXBIND1Parameters(void);

int getDnsSpeedLimitFlag(void);
int setDnsSpeedLimitFlag(int flag);
int8 cwmpBuildDnsSpeedLimitParameters(void);

/*used by unopen code */
int getDefine_BACKUPROMFILE_RA_SIZE();

/*used by unopen code */
int getDefine_BACKUPROMFILE_RA_OFFSET();
int updateCwmpBindInterface(char *oldiface, int nsize);

#endif // __CWMP_H__
