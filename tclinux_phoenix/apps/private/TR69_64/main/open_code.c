//#include "AsEngine.h"
#include "../../../version/tcversion.h"
//#include "cwmpRpc.h"
//#include "cwmp.h"
#include "Global_res.h"
#if defined(TCSUPPORT_RANDOM_INFORM)
#include <time.h>
#endif
#if defined(TCSUPPORT_CT_JOYME)
#include <sys/socket.h>
#include <sys/un.h>
#endif
typedef unsigned short      Unsigned16;
typedef unsigned long       Unsigned32;
#if defined(TCSUPPORT_RANDOM_INFORM)
static int startTimerID = 0;
boolean  getRandomInformFlag(void);
#endif

#ifdef TCSUPPORT_RANDOM_INFORM
int randomInformFlag=1;// 1:defined TCSUPPORT_RANDOM_INFORM, 0:no define TCSUPPORT_RANDOM_INFORM
#else
int randomInformFlag=0;// 1:defined TCSUPPORT_RANDOM_INFORM, 0:no define TCSUPPORT_RANDOM_INFORM
#endif

int connection_req_flag = 0;

static volatile uint8 DownloadContinueFlag = 1;
static volatile uint8 AlarmBuildFlag = 0;
#if !defined(TCSUPPORT_BHARTI) 
boolean noValueTypeFlag = 0;
#endif
int cwmpFastGet = 0;
static boolean cwmpRepeatFlag = 0; 
static int customTimerID = 0;
static uint8 repeatTimes = 2;
static uint8 repeatPeriodicVal = 25;
static int customStopTimerID = 0;
uint8 redirectcloseses=0;   /* 1: ready to close redirect session*/
unsigned long HostAddress_mib = 0;
#ifdef TCSUPPORT_WLAN
static uint32 wifiMacTabTimerID = 0;
#endif
#ifdef WLAN_RT2561_SUPPORT	
volatile uint32 cwmpWLANTaskId = 0;
#endif
int g_MonitorCollectorEnable = 0;
int g_PMInformEnable = 0;

#if defined(TCSUPPORT_TR69_BIND_PVC)
typedef enum {
	eSendPending,
	eSendStarted,
	eSendError
} SendStatus_t;

typedef struct {
	int	fClientSocket;
	void	*fReceiveBuffer;
	SendStatus_t fSendStatus;
	char	*fPendingSendBuffer;
	int		fPendingSendBufferLen;
} StcpConnectInfo_t, *StcpConnectInfoPtr;

#define HTTP_SOCKET_ERROR	-1
#endif

extern int informTimerID;
#if defined(TCSUPPORT_CT_FJ)
extern int ifSendTimesFlag;
#endif

#if defined(TCSUPPORT_CMCC)
extern void processTheHostAddressExt(Unsigned32* theHostAddress, Unsigned16* theHostPort, char* theHostPath, int8 saveType);
#endif

void stopRepeatInform(void);

void printLogsysfile(char* logsysfile)
{
#if defined(TCSUPPORT_CT)
		#ifndef TCSUPPORT_SYSLOG
			sprintf(logsysfile, "dmesg > %s", UPLOAD_LOG_FILE_PATH);
			system(logsysfile);
		#else
			get_syslog_file(UPLOAD_LOG_FILE_PATH);
		#endif
#else
	#ifdef TCSUPPORT_SYSLOG
		sprintf(logsysfile, "cat %s > %s", DEFAULT_LOG_FILE, UPLOAD_LOG_FILE_PATH);
        #else
		sprintf(logsysfile, "dmesg > %s", UPLOAD_LOG_FILE_PATH);
	#endif
	     system(logsysfile);  
#endif
	return;
}

int cwmpGetFileType(void){
#ifdef TCSUPPORT_SYSLOG
	if((rpc.fileTransferStruct != NULL) && (rpc.fileTransferStruct->FileType != NULL))
	{
		if(rpc.fileTransferStruct->transferType == Download)
		{
			if(strcmp(rpc.fileTransferStruct->FileType, "1 Firmware Upgrade Image") == 0)
				return CWMP_FIRMWARE;
			if(strcmp(rpc.fileTransferStruct->FileType, "2 Web Content") == 0)
				return CWMP_WEB_CONTENT;
			if(strcmp(rpc.fileTransferStruct->FileType, "3 Vendor Configuration File") == 0)
				return CWMP_CONFIGURATION_FILE;
		}
		else if(rpc.fileTransferStruct->transferType == Upload)
		{
			if(strcmp(rpc.fileTransferStruct->FileType, "1 Vendor Configuration File") == 0)
				return CWMP_CONFIGURATION_FILE;
			if(strcmp(rpc.fileTransferStruct->FileType, "2 Vendor Log File") == 0)
				return CWMP_SYSLOG_FILE;
		}
	}
#endif
	return -1;
}

void traceRoute(cwmp_rpc_t* rpc)
{
/*added for trace route, by xyyou*/
	#ifdef TCSUPPORT_TRACEROUTE
        if(rpc->flags.theTraceRouteFlag & CWMP_TRACEROUTE_ACTIVE){
		doCwmpTraceRoute();
		rpc->flags.theTraceRouteFlag &= ~CWMP_TRACEROUTE_ACTIVE;
		tcdbg_printf("do trace Route");
	 }
	#endif
	return;
}

void BindTR69ToServerSocket(void){
#if defined(TCSUPPORT_TR69_BIND_PVC)
	char conntype[10];
	char deviceNodeName[30];
	char wanNodeName[30];
	char if_name[10] = {0};
	char bind_PVC[4]={0};
	char bindactive[4]={0};
	char value[10]={0};
	int tmp=0;
	int bind_PVC_Index=-1;
	
	if(gTcpListenerPorts[gCwmpTcpListener] != getCPEPort() ||
		gTcpListenerSockets[gCwmpTcpListener] == HTTP_SOCKET_ERROR){
		return ;
	}
	/*1.Get TR69 Interface name first*/
	
	if(tcapi_get("Cwmp_Entry","BindActive",bindactive)!=0)
		return;
	if(strcmp(bindactive,"Yes"))//bind to default route
	{
#if !defined(TCSUPPORT_ARC_CWMP) 
		for(tmp = 0; tmp < 8; tmp++) 
#endif
		
		{

					sprintf(wanNodeName, "Wan_PVC%d",tmp);
					tcapi_get(wanNodeName,"DEFAULTROUTE",value);
					if(!strcmp(value,"Yes"))
					{
						bind_PVC_Index=tmp;
						break;
				
					}
		}
		
	}
	else
	{
		if(tcapi_get("Cwmp_Entry","wan_PVC",bind_PVC)!=0)
			return;
		bind_PVC_Index=atoi(bind_PVC);
		
	}
	memset(deviceNodeName,0,sizeof(deviceNodeName));
	sprintf(deviceNodeName,"DeviceInfo_PVC%d",bind_PVC_Index);
	
	if(tcapi_get(deviceNodeName,"connType",conntype)!=0)
		return;
	memset(if_name, 0, sizeof(if_name));
	if(strstr(conntype,"PPP")!=NULL)
		sprintf(if_name,"ppp%d",bind_PVC_Index);
	else
		sprintf(if_name,"nas%d",bind_PVC_Index);

	/*2.Rebind to device*/
	if(setsockopt(gTcpListenerSockets[gCwmpTcpListener] , SOL_SOCKET, SO_BINDTODEVICE, if_name, 32) == -1) {
		tcdbg_printf("[ReBindToDevice]: bind fail,socket=%d\n", gTcpListenerSockets[gCwmpTcpListener]);
	}
#endif

	return ;
}

void setSocketOpt_Bind(int fd)
{
#if defined(TCSUPPORT_TR69_BIND_PVC)
#endif

	return;
}

int8 forceInformFlag = 0; 
int8 getforceInformFlag(void)
{
	return forceInformFlag;
}

void setforceInformFlag(int8 value)
{
	forceInformFlag = value;
}


uint8 getDownloadContinueFlag(void)
{
	return DownloadContinueFlag;
}

void setDownloadContinueFlag( uint8 value)
{
	DownloadContinueFlag = value;
	return;
}

uint8 setDownloadStartFlag(uint32 value)
{
#ifdef TCSUPPORT_CROSS_REBOOT
	char temp[8];

	sprintf(temp, "%ld", value);
	tcapi_set("Cwmp_Entry", "DownloadStartFlag", temp);
	tcapi_commit("Cwmp_Entry");
#endif
	return 0;
}

uint32 getDownloadStartFlag(void)
{	
#ifdef TCSUPPORT_CROSS_REBOOT
	char temp[8];

	tcapi_get("Cwmp_Entry", "DownloadStartFlag", temp);
	return (unsigned int)atoi(temp);
#endif
	return 0;
}

/*______________________________________________________________________________
**  SetFileTransferStartTime_improve
**
**  descriptions: 
		used to set the file transfer start time by current CPE time
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
		cwmpPerformFileTransfer
**  call:
**____________________________________________________________________________*/
uint8 SetFileTransferStartTime_improve( )
{
#if defined(TCSUPPORT_CWMP_PRECISE_TIME)
	time_t tm;
	uint32  endTime;	

	time(&tm);
	SetFileTransferStartTime((uint32)tm);
#endif	
	return 0;
}

/*______________________________________________________________________________
**  SetFileTransferCompleteTime_improve
**
**  descriptions: 
		used to set the file transfer Complete time by current CPE time
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
		WcHttpFiniteStateMachine
		ProcessHttpVersion
**  call:
**____________________________________________________________________________*/
uint8 SetFileTransferCompleteTime_improve( )
{
#if defined(TCSUPPORT_CWMP_PRECISE_TIME)

	time_t tm;
	uint32  endTime;

	time(&tm);
	SetFileTransferCompleteTime((uint32)tm);
#endif
	return 0;
}

int isDslOpmodeChange(){
#if defined(TCSUPPORT_CT_WAN_PTM)
	char Opmode[32] = {0};
	char OldOpmode[32] = {0};
	
	if(tcapi_get("Info_Adsl", "Opmode", Opmode) != 0)
	{
		strcpy(Opmode,"");
	}

	
	if(tcapi_get("Adsl_Entry","OldOpmode",OldOpmode)!=0){
		strcpy(OldOpmode,"");
	}

	if(strcmp(Opmode,OldOpmode) == 0){
		//is the same, not change
		return 0;
	}
#endif

	return 1;
}

uint8 getAlarmBuildFlag(void)
{
	return AlarmBuildFlag;
}

void setAlarmBuildFlag(uint8 value)
{
	AlarmBuildFlag = value;
	return;
}

#define LINK_UP		1
#define LINK_DOWN	0
#define LINK_INIT   2

#define TPSTC_ATM_MODE      1
#define TPSTC_PTM_MODE      2
#define TPSTC_OTHER_MODE    3

#define ETHER_WAN_UP      1
#define ETHER_WAN_DOWN    0

#define WAN_TYPE_PTM		0
#define WAN_TYPE_ATM		1
#define WAN_TYPE_ETHER		2
#define WAN_TYPE_OTHER  	3

#define ISP_BRIDGE		"3"

int getAllISPIndex(int *pWanType)
{
	int pvcIndex = -1;
	char nodeName[32];
	char ispValue[8];
	char defaultRoute[8];
	char string[16];
	char linkUpStatus = 0;
	char tpstcMode = 0;
	char etherWanStatus = 0;
	int wanType = 0;

	/*
		Add your handler code here.
	*/
	tcapi_get("Wan_Common", "LinkUpStatus", string);
	linkUpStatus = atoi(string);
	tcapi_get("Wan_Common", "TpstcMode", string);
	tpstcMode = atoi(string);
	tcapi_get("Wan_Common", "EtherWanStatus", string);
	etherWanStatus = atoi(string);

	if((linkUpStatus == LINK_UP) && (tpstcMode == TPSTC_PTM_MODE))
		wanType = WAN_TYPE_PTM;
	else if((linkUpStatus == LINK_UP) && (tpstcMode == TPSTC_ATM_MODE))
		wanType = WAN_TYPE_ATM;
	else if(etherWanStatus == ETHER_WAN_UP)
		wanType = WAN_TYPE_ETHER;
	else {
		*pWanType = WAN_TYPE_OTHER;
		return -1;
	}
	*pWanType = wanType;
	
	memset(nodeName, 0, sizeof(nodeName));
	memset(ispValue, 0, sizeof(ispValue));
	memset(defaultRoute, 0, sizeof(defaultRoute));

	/*find default router pvc*/
	for(pvcIndex=0; pvcIndex<8; pvcIndex++){
		if(wanType == WAN_TYPE_PTM)
			sprintf(nodeName, "WanExt_PVC8e%d", pvcIndex);
		else if(wanType == WAN_TYPE_ATM)
			sprintf(nodeName, "Wan_PVC%d", pvcIndex);
		else if(wanType == WAN_TYPE_ETHER)
			sprintf(nodeName, "WanExt_PVC10e%d", pvcIndex);
		tcapi_get(nodeName, "ISP", ispValue);
		tcapi_get(nodeName, "DEFAULTROUTE", defaultRoute);
		
		if(strcmp(ispValue, ISP_BRIDGE) == 0)
			continue;

		if(strcmp(defaultRoute, "Yes") == 0){
			break;
		}

	}
	if(pvcIndex == 8)
		return -1;
	return pvcIndex;	
}

#if !defined(TCSUPPORT_CT)
void sendInform(){
		/* ToT bug #2705
		 * customer ACS server is unstable, sometimes CPE must repeat send inform in small interval for 
		 * several times to connect ACS when CPE boot-strap
		 * This feature is close by default, customer can open it with CI-Command
		 * Liutao@20090702
		 */
		uint32 second = 1000;
		
		if(cwmpRepeatFlag && repeatTimes && repeatPeriodicVal){
			if(!customTimerID && !customStopTimerID){
			tc_printf("\r\ncwmpInit");
				customTimerID = AddPeriodicInformTimer( repeatPeriodicVal * second,  (TimerCallback)cwmpPeriodInform, NULL );
				customStopTimerID = timerStartS((repeatPeriodicVal * repeatTimes)*second, (TimerCallback)stopRepeatInform, NULL );
			}
		}
		else{//sam_20081225 merge from huawei
				startPeriodicInform();
		}
		return;
}

static char pDeviceSummary[256]; 
char* getDeviceSummary(void)
{
	/*here we should handle other device like usb device in future.*/
	sprintf(pDeviceSummary, "InternetGatewayDevice:1.0[](Baseline:1, EthernetLAN:1, ADSLWAN:1, Time:1, IPPing:1,ATMLoopback:1, DSLDiagnostics:1");
#ifdef TCSUPPORT_WLAN
	sprintf(pDeviceSummary, "%s, WiFiLAN:1", pDeviceSummary);
#endif
#ifdef TR111
	sprintf(pDeviceSummary, "%s, DeviceAssociation:1", pDeviceSummary);
#endif
	sprintf(pDeviceSummary, "%s)", pDeviceSummary);
	return pDeviceSummary;
}

void wifiMacTabTimerStart(void){
#ifdef TCSUPPORT_WLAN
		//added by xyzhu_nj_20091209
		if(!wifiMacTabTimerID){
			wifiMacTabTimerID = timerPeriodicStartS(5000, (TimerCallback)wifiMacTabChanged, NULL);
		}
		return;
#else
		return;
#endif
}

int getISPIndex(void)
{
	int pvcIndex = -1;
	char nodeName[32];
	char ispValue[8];
	char defaultRoute[8];

	/*
		Add your handler code here.
	*/
	memset(nodeName, 0, sizeof(nodeName));
	memset(ispValue, 0, sizeof(ispValue));
	memset(defaultRoute, 0, sizeof(defaultRoute));

	/*find default router pvc*/
	for(pvcIndex=0; pvcIndex<8; pvcIndex++){
		sprintf(nodeName,"Wan_PVC%d", pvcIndex);
		tcapi_get(nodeName, "ISP", ispValue);
		tcapi_get(nodeName, "DEFAULTROUTE", defaultRoute);
		
		if(strcmp(ispValue, ISP_BRIDGE) == 0)
			continue;

		if(strcmp(defaultRoute, "Yes") == 0){
			break;
		}

	}
	if(pvcIndex == 8)
		return -1;
	return pvcIndex;	
}

void checkAndDoAlarmMonitorTask(void) {
}

void setDeviceAlarmFlag(boolean value){
}

int cwmpSetCwmpUrl_CT(char * URL){
	return 0;
}
#else
int cwmpSetCwmpUrl(char * URL,int flag){
	return 0;
}

#ifndef CLMP
typedef struct {
} upnpNotifyRequest, *upnpNotifyRequestPtr;

typedef struct rpData {
} rpData, *rpDataPtr;

typedef struct rpConnection {
} rpConnection, *rpConnectionPtr;

typedef struct rpHttpRequest {
} rpHttpRequest, *rpHttpRequestPtr;

typedef struct {
} rpSoapRequest, *rpSoapRequestPtr;


typedef unsigned char   Boolean;
typedef unsigned char       Unsigned8,  *Unsigned8Ptr;
typedef signed short        Signed16,   *Signed16Ptr;
#define kUpnpUuidSize               40

typedef enum {
    eSoapOk,
} ruSoapProcessingStatus;

typedef enum {
    eRpNoError              = 0,
} RpErrorCode;

upnpNotifyRequestPtr RuCaptureNotifyRequest(rpDataPtr theTaskDataPtr) {
	return (upnpNotifyRequestPtr) 0;
}

extern void RuCheckForDiscovery(rpConnectionPtr theConnectionPtr){
}

Boolean RuCheckUpnpListener(rpDataPtr theTaskDataPtr){
	return 0;
}

extern void RuHandleNotifyTimers(rpDataPtr theTaskDataPtr){
}

RpErrorCode RuInitUpnpDevice(rpDataPtr theTaskDataPtr){
	return 0;
}

void RuUpnpDeviceDeInit(rpDataPtr theTaskDataPtr){
	return;
}

extern void RuSendNotification(rpConnectionPtr theConnectionPtr,
                        upnpNotifyRequestPtr theNotifyRequestPtr) {
}

void RuStartUpnpListener(rpConnectionPtr theConnectionPtr) {
}

void RuInitUpnpData(void){
	return;
}

void RuInitUpnpNotify(void *theTaskDataPtr){
	return;
}

void RuDeliverNextEventMessage(void * theConnectionPtr) {
}

ruSoapProcessingStatus RuGetSoapProcessingData(rpDataPtr theRpDataPtr,
								rpSoapRequestPtr theSoapRequestPtr) {
	return 0;
}

Unsigned8 getClmpFlag(void){
	return 0;
}

Boolean
getTR64LockFlag(void){
	return 0;
}

char*
getTR64SessionID(void){
	return 0;
}

void 
resetTR64Timer(void){
}

void RuBuildResponseHeader(void * theRequestPtr, 
						   char *theResponsePtr) {
}

void RuBuildSubscriptionHeaders(rpHttpRequestPtr theRequestPtr,
										char *theResponsePtr) {
}

void RuHandleSubscribeCommand(rpHttpRequestPtr theRequestPtr) {
}

void RuHandleUnsubscribeCommand(rpHttpRequestPtr theRequestPtr) {
}

void RuBuildEventMessageHeaders(char * theBufferPtr, void * theSubPtr) {
}

Unsigned8 gUpnpState = 0;

void gEndUpnp(void *theTaskDataPtr, Signed16Ptr theIndexValuesPtr) {
}

void gStartUpnp(void *theTaskDataPtr) {
}

Boolean rpLanIPChange_upnp = 0;

void defRouteConnectStatus(){
}

int PortMapInit(){
}

char *device_UDN_IGD = "11111111-";
char *device_UDN_WAN = "22222222-";
char *device_UDN_WANConnection = "33333333-";
char *device_UDN_LAN = "44444444-";
#ifdef WSC_SUPPORT
char *device_UDN_WLAN = "55555555-";
#endif
char *device_UDN_IGD_CLMP = "66666666-";

char gRootUDN[kUpnpUuidSize] = "11111111-1111-1111-1111-111111111111";
char gWANUDN[kUpnpUuidSize] = "22222222-2222-2222-2222-222222222222";
char gWANConnectionUDN[kUpnpUuidSize] = "33333333-3333-3333-3333-333333333333";
char gLANUDN[kUpnpUuidSize] = "44444444-4444-4444-4444-444444444444";
#ifdef WSC_SUPPORT
char gWLANUDN[kUpnpUuidSize] = "55555555-5555-5555-5555-555555555555";
#endif
char gCLMPUDN[kUpnpUuidSize] = "66666666-6666-6666-6666-666666666666"; 
char gURLBase[4] = "/";
char gpresentationURL[4] = "/";
#endif/*end CLMP*/

#endif

uint8 getAccountChangeFlag(void)
{
	char tmp[16] = {0};
	uint8 ret_val = 0;

#if defined(TCSUPPORT_CT_INFORM_NODE)
	if( tcapi_get("Cwmp_Entry", "account_change_flag",tmp) == 0){
		ret_val = atoi(tmp);
	}
#endif

	return ret_val;
}

void setAccountChangeFlag(uint8 value)
{
#if defined(TCSUPPORT_CT_INFORM_NODE)
	if(getAccountChangeFlag() != value){

	if(value == 1){
		//AccountChangeFlag = value;
		tcapi_set("Cwmp_Entry","account_change_flag", "1");
	}else{
		tcapi_set("Cwmp_Entry","account_change_flag", "0");
	}
	//tcapi_save();
	spSave(); 
	}
#endif
	return;
}

/*______________________________________________________________________________
**	getDefine_BACKUPROMFILE_RA_SIZE
**
**	descriptions:
**		used by unopen code
**	parameters:
**
**	return:
**		Success: 	BACKUPROMFILE_RA_SIZE
**		FAIL: it should never be happened
**_______________________________________________________________________________
*/
int getDefine_BACKUPROMFILE_RA_SIZE(){
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT) && !defined(TCSUPPORT_CT_CWMP_ZIPROMFILE)
	return BACKUPROMFILE_RA_SIZE;
#else
#if defined(TCSUPPORT_BACKUPROMFILE)
	return BACKUPROMFILE_RA_SIZE;
#endif
	return 0x10000;
#endif
}

/*______________________________________________________________________________
**	getDefine_BACKUPROMFILE_RA_OFFSET
**
**	descriptions:
**		used by unopen code
**	parameters:
**
**	return:
**		Success: BACKUPROMFILE_RA_OFFSET
**		FAIL: it should never be happened
**_______________________________________________________________________________
*/
int getDefine_BACKUPROMFILE_RA_OFFSET(){
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT) && !defined(TCSUPPORT_CT_CWMP_ZIPROMFILE)
	return BACKUPROMFILE_RA_OFFSET;
#else
#if defined(TCSUPPORT_BACKUPROMFILE)
	return BACKUPROMFILE_RA_OFFSET;
#endif
	return 0x0;
#endif
}

#if defined(TCSUPPORT_CT)
extern void ReadAttr(attr_ptr tcAttr_ptr, int *saveFlag);
#else
extern void ReadAttr(attr_ptr tcAttr_ptr);
#endif

/*______________________________________________________________________________
**	ReadAttr_OP
**
**	descriptions:
**		used by unopen code
**	parameters:
**
**	return:
**_______________________________________________________________________________
*/
void ReadAttr_OP(attr_ptr tcAttr_ptr, int *saveFlag){
#if defined(TCSUPPORT_CT)
	ReadAttr(tcAttr_ptr, saveFlag);
#else
	ReadAttr(tcAttr_ptr);
#endif
}

char *forcedNotifyNode[] = {
	"InternetGatewayDevice.DeviceInfo.SoftwareVersion",
	"InternetGatewayDevice.DeviceInfo.ProvisioningCode",
	"InternetGatewayDevice.ManagementServer.ConnectionRequestURL",
#ifdef CT_COM_DEVICEREG
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
    "InternetGatewayDevice.X_CU_UserInfo.UserName",
	"InternetGatewayDevice.X_CU_UserInfo.UserId",
#else
	"InternetGatewayDevice.X_CT-COM_UserInfo.UserName",
	"InternetGatewayDevice.X_CT-COM_UserInfo.UserId",
#endif
#endif
	NULL
};

/*______________________________________________________________________________
**	cwmp_update_customer_running_romfile
**
**	descriptions:
**		To store running romfile for TR069 Download RPC
**		Compress the file and write it to running romfile flash sector
**
**	parameters:
**		file: 			Specify the file path.
**
**	return:
**		0: Do nothing
**		1: Do romfile compression and write successfully
**	     -1: Fail
**____________________________________________________________________________
*/
int
cwmp_update_customer_running_romfile(char *file){

	/* Add action to get running romfile, then modify return value as 1*/
	/* Compress the file and write it to running romfile flash sector */

	return 0;
}

#if 1 //def TCSUPPORT_BACKUPROMFILE
/*______________________________________________________________________________
**	cwmp_update_customer_backup_romfile
**
**	descriptions:
**		To store running romfile for TR069 Download RPC
**		Compress the file and write it to backup romfile flash sector
**
**	parameters:
**		file: 			Specify the file path.
**
**	return:
**		0: Do nothing
**		1: Do romfile compression and write successfully
**	     -1: Fail
**____________________________________________________________________________
*/
int
cwmp_update_customer_backup_romfile(char *file){

	/* Add action to get running romfile, then modify return value as 1*/
	/* Compress the file and write it to backup romfile flash sector */

	return 0;
}
#endif

#if defined(TCSUPPORT_RANDOM_INFORM)
void cwmpStartInform(void)
{	
	uint  period = getInformPeriod()*1000;

	/*
		send a random inform first,and then build a timer which send period inform
	*/	
	cwmpPeriodInform();	

//	informTimerID = timerPeriodicStartS( period, (TimerCallback)cwmpPeriodInform, NULL );
	informTimerID = AddTimer_tmp( NULL, period, (TimerCallback)cwmpPeriodInform, NULL, TRUE );
}

boolean getRandomInformFlag(void)
{
	char Buf[10] = {0};
	tcapi_get("Cwmp_Entry","RandomInform",Buf);

	if(!strcmp(Buf,"1"))
		return 1;
	else
		return 0; 
}
#endif

void stopRepeatInform(void)
{
	uint32  period = getInformPeriod()*1000;

	if(customTimerID)
		timerPeriodicStop(customTimerID);
	
	startPeriodicInform();
	return;
}

/*add by robbie.jin*/
uint32 getPeriodicDelay()
{
	char        informPeriodTime[64] = {0};
	time_t      cur_ltime;
    time_t      inform_ltime;
	struct tm  cur_sTime;
	struct tm  inform_sTime;
	uint32       cur_hour = 0, cur_minute = 0, cur_second = 0, cur_month = 0, cur_day = 0;
	uint32       inform_hour = 0, inform_minute = 0, inform_second = 0, inform_month = 0, inform_day = 0;
	uint32      cur_year = 0;
	uint32      inform_year = 0;
    int32       delay = 0;
    int32      period = 0;

    period = (int32)getInformPeriod();
    
    /*get current time*/
	time(&cur_ltime);
	localtime_r(&cur_ltime, &cur_sTime);
	cur_year = cur_sTime.tm_year+1900;
	/*if (cur_year < 2000)
		cur_year = 2000;*/
	cur_month = cur_sTime.tm_mon+1;
	cur_day = cur_sTime.tm_mday;
	cur_hour = cur_sTime.tm_hour;
	cur_minute = cur_sTime.tm_min;
	cur_second = cur_sTime.tm_sec;
    tc_printf("\rcurrent time %4u-%2u-%2uT%2u:%2u:%2u\n",
        cur_year, cur_month, cur_day,
        cur_hour, cur_minute, cur_second);

    /*get periodic inform time*/
    strncpy(informPeriodTime, getcwmpPrdInformTime(), sizeof(informPeriodTime));
    if(!checkPeriodInformTimeFormat(informPeriodTime))
    {
        tcdbg_printf("periodic inform time %s invalid!\r\n",informPeriodTime);
        return period;
    }
    if(isPITUnknownValue(informPeriodTime))
    {
        tcdbg_printf("periodic inform time %s unknown!\r\n",informPeriodTime);
        return period;
    }
	sscanf(informPeriodTime, "%04u-%02u-%02uT%02u:%02u:%02u", 
        &inform_year, &inform_month, &inform_day,
        &inform_hour, &inform_minute, &inform_second);
    tc_printf("\rperiodic inform time %4u-%2u-%2uT%2u:%2u:%2u\n",
        inform_year, inform_month, inform_day,
        inform_hour, inform_minute, inform_second);
    inform_sTime.tm_year = inform_year - 1900;
    inform_sTime.tm_mon = inform_month - 1;
    inform_sTime.tm_mday = inform_day;
    inform_sTime.tm_hour = inform_hour;
    inform_sTime.tm_min = inform_minute;
    inform_sTime.tm_sec = inform_second;
    
    inform_ltime = mktime(&inform_sTime);
    delay = (int32)((inform_ltime - cur_ltime) % period);
    if(delay < 0)
    {
        delay += period;
    }
    tc_printf("\r\ncurrent time in seconds is %ld, inform time in seconds is %ld, delay is %d\n",
        cur_ltime, inform_ltime, delay);

    inform_ltime = cur_ltime + delay;
    localtime_r(&inform_ltime, &inform_sTime);
    tc_printf("\r\nactual inform time %4u-%2u-%2uT%2u:%2u:%2u\n",
        inform_sTime.tm_year + 1900, inform_sTime.tm_mon + 1, inform_sTime.tm_mday,
        inform_sTime.tm_hour, inform_sTime.tm_min, inform_sTime.tm_sec);
   
    return delay;
}

/*added by robbie.jin*/
int AddPeriodicInformTimer(uint32  period, TimerCallback callback, void *param)
{
    uint32  delay = 0;
	uint32	second = 1000;	/* ticks for 1 second */
    delay = getPeriodicDelay();
    
    return AddDelayTimer(NULL, delay * second, period, callback, param, TRUE);
}

int startPeriodicInform(void)
{
	uint32	second = 1000;	/* ticks for 1 second */
	uint32     period = getInformPeriod() *second;

#if defined(TCSUPPORT_RANDOM_INFORM)
	int randtime = 0;
	int randvalue = 0;
	int randinterval = 0;
	/*			
		if enable random inform and enable periodic inform and 
		this is the first time to send random inform			
	*/
	if (getRandomInformFlag())	 
	{	
		if (getInformPeriodEnable() && !startTimerID &&  !checkPeriodicInformTime())
		{
				uint8 macAddr[6];
			int seed;

			randinterval = getInformPeriod();			
			GetEtherAddr(macAddr);
			memcpy(&seed, (macAddr+2),4); /*take the last 4 Byte MAC address as part of seed*/
			srand (seed*time(NULL));
			randvalue = rand();
			//timerVlrGet(1, randvalue);			
#if defined(TCSUPPORT_C1_CUC)
			/* CUC required: random interval must more than 30 seconds and less than periodic
			 * inform interval.
			 * 1. there is such situation: rand() of modem A is 13 seconds, rand() of modem B
			 * is 24 secons, the orignal arithmetic will make them both are 30 seconds;
			 * 2. cwmp initiation will cost serial seconds, so the min random interval maybe 35 
			 * seconds.
			 * */
			if(randinterval > MIN_RANDOM_INTERVAL){
				randinterval -= MIN_RANDOM_INTERVAL;
				randtime = randvalue % randinterval + MIN_RANDOM_INTERVAL;
			}
			else{
				randtime = MIN_RANDOM_INTERVAL;
			}
#else
			randtime = randvalue%randinterval;
			if (randtime < 30) { /*in Bharti branch, the default minimum is 30*/
				randtime = 30;
			}
#endif

			startTimerID = timerStartS(randtime*second,(TimerCallback)cwmpStartInform, NULL );	
		}
	}
	else
	{
		if(getInformPeriodEnable() && period && !informTimerID ) 
			informTimerID = AddPeriodicInformTimer( period, (TimerCallback)cwmpPeriodInform, NULL );
	}
#else
	//sam_20081225 merge from huawei
		tc_printf("startPeriodicInform begin!");
	if(getInformPeriodEnable() && period && !informTimerID ) 
	{
		tc_printf("startPeriodicInform end!");
		informTimerID = AddPeriodicInformTimer( period,  (TimerCallback)cwmpPeriodInform, NULL );
	}
#endif	

	return 0;
	
}

void wifiMacTabChanged(void){
#ifdef TCSUPPORT_WLAN
	char buf[10] = {'\0'};
	tcapi_get("wifiMacTab_Common", "valueChanged", buf);
		
	if(!strcmp(buf, "1")){
		cwmpWLANTask();
	}	
	return;
#endif
}

#ifdef TCSUPPORT_WLAN
/*This task is used to initiate the WLAN Table when any station is added or deleted!by hqfeng,2006.09.05*/
void cwmpWLANTask(void)
{
	DeviceNodePtr WLANConfiguration = NULL;
	DeviceNodePtr rootptr = NULL;
	//int i_state;
	char *objectName = "InternetGatewayDevice.LANDevice.1.WLANConfiguration.";

	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &WLANConfiguration);
	if(WLANConfiguration == NULL){
		return;
	}	
	//mark by xyzhu_nj
	//while(1){
		#if 0
		wwait();
		#else
		//sem_wait(&SEM_VALCHANGE);
		#endif
		//modified by xyzhu_nj_20091209
		#if 0
		i_state = dirps();
		#endif
		pthread_mutex_lock(&value_change_lock);
		
		if(inform_task_start == 1){
			/* if inform task is running, quit and do nothing 
			  * if inform_task_start locked, at the end of current http session will call spSave(), to call cwmpValueChangeTask reInit tree
			  */
#if 0
			restore(i_state);
#endif
			pthread_mutex_unlock(&value_change_lock);

		//	continue;
			return;
		}else{
			rootptr = rpc.IGDDeviceRootPtr;
			rpc.IGDDeviceRootPtr = NULL;
			//restore(i_state);
			pthread_mutex_unlock(&value_change_lock);
		}
		WLANConfiguration = NULL;
		cwmpSearchDeviceNodeByName("InternetGatewayDevice.LANDevice.1.WLANConfiguration.", rootptr, &WLANConfiguration);
		if(WLANConfiguration != NULL){
			popNodeToName(rootptr);
		       cwmpFreeDeviceTable(WLANConfiguration);
		       cwmpInitWLANConfiguration1SubTree(WLANConfiguration);
			pushNameToStack();
	        }
		
	//	i_state = dirps();
		pthread_mutex_lock(&value_change_lock);
		rpc.IGDDeviceRootPtr = rootptr;	
	//	restore(i_state);
		pthread_mutex_unlock(&value_change_lock);
       // }
}

#ifdef WLAN_RT2561_SUPPORT
#if 0//test mark by brian
int cwmpWLANTaskStart(void){

	if (!cwmpWLANTaskId) {
		if ((cwmpWLANTaskId = sysCreateTask("cwmpWLANTask",
									  (void *) cwmpWLANTask,
									  4096, TASK_PRI_NORMAL+2)) != 0) {
			sysStartTask(cwmpWLANTaskId);
		} else {
			tc_printf("\r\nCreate WLAN task fails!");
			return -1;
		}
	}

	return 0;
}

#endif
uint32 getCwmpWLANTaskId(void)
{
	return cwmpWLANTaskId;
}

#endif

#endif

#if !defined(TCSUPPORT_CT) || defined(CLMP)
char PgDeviceDescription_Item_23[] =
	"</UPC>\n"
	"<iconList>\n"
	"<icon>\n"
	"<mimetype>image/png</mimetype>\n"
	"<width>48</width>\n"
	"<height>48</height>\n"
	"<depth>8</depth>\n"
	"<url>/Images/L48x48x8</url>\n"
	"</icon>\n"
	"<icon>\n"
	"<mimetype>image/png</mimetype>\n"
	"<width>32</width>\n"
	"<height>32</height>\n"
	"<depth>8</depth>\n"
	"<url>/Images/L32x32x8</url>\n"
	"</icon>\n"
	"<icon>\n"
	"<mimetype>image/png</mimetype>\n"
	"<width>16</width>\n"
	"<height>16</height>\n"
	"<depth>8</depth>\n"
	"<url>/Images/L16x16x8</url>\n"
	"</icon>\n"
	"</iconList>\n"
	"<serviceList>\n"
	"<service>\n"
	"<serviceType>urn:schemas-upnp-org:service:Layer3Forwarding:1</serviceType>\n"
	"<serviceId>urn:upnp-org:serviceId:L3Forwarding1</serviceId>\n"
	"<SCPDURL>/L3Fwd.xml</SCPDURL>\n"
#if !defined(TCSUPPORT_C1_NEW_GUI) 
	"<controlURL>/UD/?0</controlURL>\n"
#endif
	"<eventSubURL>/?0</eventSubURL>\n"
	"</service>\n"
	"<service>\n"
	"<serviceType>urn:dslforum-org:service:DeviceConfig:1</serviceType>\n"
	"<serviceId>urn:upnp-org:serviceId:DeviceCfg1</serviceId>\n"
	"<SCPDURL>/DeviceCfg.xml</SCPDURL>\n"
#if !defined(TCSUPPORT_C1_NEW_GUI) 
	"<controlURL>/UD/?1</controlURL>\n"
#endif
	"<eventSubURL>/?1</eventSubURL>\n"
	"</service>\n"
	"<service>\n"
	"<serviceType>urn:dslforum-org:service:DeviceInfo:1</serviceType>\n"
	"<serviceId>urn:upnp-org:serviceId:DeviceInfo1</serviceId>\n"
	"<SCPDURL>/DeviceInfo.xml</SCPDURL>\n"
#if !defined(TCSUPPORT_C1_NEW_GUI) 
	"<controlURL>/UD/?2</controlURL>\n";
#endif

char PgDeviceDescription_Item_24[] =
	"<eventSubURL>/?2</eventSubURL>\n"
	"</service>\n"
	"<service>\n"
	"<serviceType>urn:dslforum-org:service:ManagementServer:1</serviceType>\n"
	"<serviceId>urn:upnp-org:serviceId:LANMgmtSrv1</serviceId>\n"
	"<SCPDURL>/LanMgmtSrv.xml</SCPDURL>\n"
#if !defined(TCSUPPORT_C1_NEW_GUI) 
	"<controlURL>/UD/?3</controlURL>\n"
#endif
	"<eventSubURL>/?3</eventSubURL>\n"
	"</service>\n"
	"<service>\n"
	"<serviceType>urn:dslforum-org:service:Time:1</serviceType>\n"
	"<serviceId>urn:upnp-org:serviceId:Time1</serviceId>\n"
	"<SCPDURL>/Time.xml</SCPDURL>\n"
#if !defined(TCSUPPORT_C1_NEW_GUI) 
	"<controlURL>/UD/?4</controlURL>\n"
#endif
	"<eventSubURL>/?4</eventSubURL>\n"
	"</service>\n"
	"</serviceList>\n"
	"<deviceList>\n"
	"<device>\n"
	"<deviceType>urn:schemas-upnp-org:device:WANDevice:1</deviceType>\n"
	"<friendlyName>";

char PgDeviceDescription_Item_44[] =
	"</UPC>\n"
	"<serviceList>\n"
	"<service>\n"
	"<serviceType>urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1</serviceType>\n"
	"<serviceId>urn:upnp-org:serviceId:WANCommonIFC1</serviceId>\n"
	"<SCPDURL>/WanCommonIfc1.xml</SCPDURL>\n"
#if !defined(TCSUPPORT_C1_NEW_GUI) 
	"<controlURL>/UD/?5</controlURL>\n"
#endif
	"<eventSubURL>/?5</eventSubURL>\n"
	"</service>\n"
	"<service>\n"
	"<serviceType>urn:dslforum-org:service:WANDSLInterfaceConfig:1</serviceType>\n"
	"<serviceId>urn:upnp-org:serviceId:WANDslItfCfg1</serviceId>\n"
	"<SCPDURL>/WanDslItfCfg.xml</SCPDURL>\n"
#if !defined(TCSUPPORT_C1_NEW_GUI) 
	"<controlURL>/UD/?6</controlURL>\n"
#endif
	"<eventSubURL>/?6</eventSubURL>\n"
	"</service>\n"
	"<service>\n"
	"<serviceType>urn:dslforum-org:service:WANDSLConnectionManagement:1</serviceType>\n"
	"<serviceId>urn:upnp-org:serviceId:WANDslCntMgmt1</serviceId>\n"
	"<SCPDURL>/WanDslCntMgmt.xml</SCPDURL>\n"
#if !defined(TCSUPPORT_C1_NEW_GUI) 
	"<controlURL>/UD/?7</controlURL>\n"
#endif
	"<eventSubURL>/?7</eventSubURL>\n"
	"</service>\n"
	"</serviceList>\n"
	"<deviceList>\n"
	"<device>\n"
	"<deviceType>urn:schemas-upnp-org:device:WANConnectionDevice:1</deviceType>\n"
	"<friendlyName>";

char PgDeviceDescription_Item_64[] =
	"</UPC>\n"
	"<serviceList>\n"
	"<service>\n"
	"<serviceType>urn:schemas-upnp-org:service:WANDSLLinkConfig:1</serviceType>\n"
	"<serviceId>urn:upnp-org:serviceId:WANDSLLinkC1</serviceId>\n"
	"<SCPDURL>/WanDslLink.xml</SCPDURL>\n"
#if !defined(TCSUPPORT_C1_NEW_GUI) 
	"<controlURL>/UD/?8</controlURL>\n"
#endif
	"<eventSubURL>/?8</eventSubURL>\n"
	"</service>\n"
	"<service>\n"
	"<serviceType>urn:schemas-upnp-org:service:WANIPConnection:1</serviceType>\n"
	"<serviceId>urn:upnp-org:serviceId:WANIPConn1</serviceId>\n"
	"<SCPDURL>/WanIpConn.xml</SCPDURL>\n"
#if !defined(TCSUPPORT_C1_NEW_GUI) 
	"<controlURL>/UD/?9</controlURL>\n"
#endif
	"<eventSubURL>/?9</eventSubURL>\n"
	"</service>\n"
	"<service>\n"
	"<serviceType>urn:dslforum-org:service:WANPPPConnection:1</serviceType>\n"
	"<serviceId>urn:upnp-org:serviceId:WANPPPConn1</serviceId>\n"
	"<SCPDURL>/WanPpp.xml</SCPDURL>\n"
#if !defined(TCSUPPORT_C1_NEW_GUI) 
	"<controlURL>/UD/?10</controlURL>\n"
#endif
	"<eventSubURL>/?10</eventSubURL>\n"
	"</service>\n"
	"</serviceList>\n"
	"</device>\n"
	"</deviceList>\n"
	"</device>\n"
	"<device>\n"
	"<deviceType>urn:schemas-upnp-org:device:LANDevice:1</deviceType>\n"
	"<friendlyName>";

#if 1//xyyou for release un open code issue(TR64)
char PgDeviceDescription_Item_84[] =
	"</UPC>\n"
	"<serviceList>\n"
	"<service>\n"
	"<serviceType>urn:schemas-upnp-org:service:LANHostConfigManagement:1</serviceType>\n"
	"<serviceId>urn:upnp-org:serviceId:LANHostCfg1</serviceId>\n"
	"<SCPDURL>/LanHostCfgMgmt.xml</SCPDURL>\n"
#if !defined(TCSUPPORT_C1_NEW_GUI) 
	"<controlURL>/UD/?11</controlURL>\n"
#endif
	"<eventSubURL>/?11</eventSubURL>\n"
	"</service>\n"
#ifdef TCSUPPORT_WLAN
	"<service>\n"
	"<serviceType>urn:dslforum-org:service:WLANConfiguration:1</serviceType>\n"
	"<serviceId>urn:upnp-org:serviceId:WLANCfg1</serviceId>\n"
	"<SCPDURL>/WLanCfg.xml</SCPDURL>\n"
#if !defined(TCSUPPORT_C1_NEW_GUI) 
	"<controlURL>/UD/?12</controlURL>\n"
#endif
	"<eventSubURL>/?12</eventSubURL>\n"
	"</service>\n"
#endif
	"<service>\n"
	"<serviceType>urn:dslforum-org:service:LANEthernetInterfaceConfig:1</serviceType>\n"
	"<serviceId>urn:upnp-org:serviceId:LANEnetItfCfg1</serviceId>\n"
	"<SCPDURL>/LanEnetItfCfg.xml</SCPDURL>\n"
#if !defined(TCSUPPORT_C1_NEW_GUI) 
	"<controlURL>/UD/?13</controlURL>\n"
#endif
	"<eventSubURL>/?13</eventSubURL>\n"
	"</service>\n"
	"</serviceList>\n"
	"</device>\n"
	"</deviceList>\n"
	"<presentationURL>";
#endif//xyyou
#else
char gRootfriendlyName[64] = " IGD";
char gWANfriendlyName[64] =  " IGD";
char gWANConnectionfriendlyName[64] = " IGD";
char gLANfriendlyName[64] = " IGD";
#endif

void SetCustomReponseValue(int *status){
#if defined(TCSUPPORT_CT_FJ)
	*status = CWMP_E_SUCCESS;
#endif
}

#if defined(TCSUPPORT_CT_FJ)
int g_ITMSRegStartTime = 0;
#endif

void GetITMSRegStartTime(){
#if defined(TCSUPPORT_CT_FJ)
	struct timespec timeSpec;
	clock_gettime(CLOCK_MONOTONIC,&timeSpec);
	g_ITMSRegStartTime=timeSpec.tv_sec;		
#endif
	return;
}

void cwmpDownloadPreset(int value){
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_REMOTE_UPG_WEBREDIRECT)
	if(value == 1 && cwmpGetDownloadFileType() == 1){
		system("echo \"1\" > /tmp/cwmpfirmwareupgrade");
		system("ebtables -t filter -A FORWARD -p IPV4 --ip-proto udp --ip-dport 53 -j DROP");
		system("iptables -t nat -A PREROUTING -p tcp --dport 80  -j REDIRECT --to-ports 80");
		system("iptables -t nat -A PREROUTING -p udp --dport 53  -j REDIRECT --to-ports 53");
	}
#endif
	return;
}


char gDevMACAddress[32] = {"00CACA120808"};
char* getDevMACAddress(void){
	get_profile_str("WAN_MAC=", gDevMACAddress, sizeof(gDevMACAddress), 0, "/etc/mac.conf");
	return gDevMACAddress;
}
#if defined(TCSUPPORT_CT_FJ)
char gOltRegTime[32] = {0};
char* getOltRegTime(void)
{
	if(tcapi_get("deviceAccount_Entry", "OltRegTime", gOltRegTime) < 0)
		strcpy(gOltRegTime, "0");
	return gOltRegTime;
}

char gITMSRegTime[32] = {0};
char* getITMSRegTime(void)
{
	if(tcapi_get("deviceAccount_Entry", "ITMSRegTime", gITMSRegTime) < 0)
		strcpy(gITMSRegTime, "0");
	return gITMSRegTime;
}

char gITMSServiceTime[32] = {0};
char* getITMSServiceTime(void)
{
	if(tcapi_get("deviceAccount_Entry", "ITMSServiceTime", gITMSServiceTime) < 0)
		strcpy(gITMSServiceTime, "0");
	return gITMSServiceTime;
}

int get_ErrorsSent(int if_index,char *nodeName, char *tempValue,char *attr,char *end_name)
{
	int ret;
	char wannode[32];
	sprintf(wannode, "Info_ra%d", if_index);
	ret = tcapi_get(wannode, attr, tempValue);
	if(ret < 0)
		return -1;
	sprintf(nodeName, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.%d.Stats.%s ", if_index + 1,end_name);
	return 0;
}

int get_StatsEthernetInfo(int if_index,char *nodeName, char *tempValue, char *attr, char *end_name)
{
	char wan_node[32];
	char Info_node[32] = {'\0'};
	char isp[32], service_app[SERVICE_LIST_LEN];
	uint32 pvc_index, entry_index;
	int ret;
	
	if (NULL == nodeName || NULL == tempValue) {
		return -1;
	}

	if (if_index < 0 || if_index > 63) {
		return -1;
	}

	pvc_index = if_index / MAX_PVC_NUM;
	entry_index = if_index - pvc_index * MAX_PVC_NUM;

	sprintf(wan_node, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
	sprintf(Info_node, "Info_nas%d.%d", pvc_index,entry_index);
	memset(isp, 0, sizeof(isp));
	ret = tcapi_get(wan_node, "ISP", isp);
	if (ret < 0) {
		return -1;
	}
	tcapi_get(Info_node, attr, tempValue);
	if (0 != strcmp(isp, "2"))
	{	
		return -1;
/*	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
		GetWanNodeMapping(&pvc_index, &entry_index);
		sprintf(nodeName, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANIPConnection.%d.Stats.%s", pvc_index, entry_index, end_name);
#else
		sprintf(nodeName, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANIPConnection.%d.Stats.%s", pvc_index + 1, entry_index + 1,end_name);
#endif
*/
	}
	else
	{
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
		GetWanNodeMapping(&pvc_index, &entry_index);
		sprintf(nodeName, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANPPPConnection.%d.Stats.%s", pvc_index, entry_index, end_name);
#else
		sprintf(nodeName, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANPPPConnection.%d.Stats.%s", pvc_index + 1, entry_index + 1,end_name);
#endif
	}
	return 0;
}

int get_ppp_uptime(int if_index,char *nodeName, char *tempValue)
{
	uint32 pvc_index, entry_index;
	pvc_index = if_index/ MAX_PVC_NUM;
	entry_index = if_index - pvc_index * MAX_PVC_NUM;
	char pvc_info[16]={0};
	int now_time,start_time,connect_time;/*for caclated ppp link-up time*/
  	int day,hour,min,sec;
	char pppUptimePath[32]={0};
	time_t timep;
	sprintf(pppUptimePath,"/tmp/pppuptime-ppp%d",if_index);
	fileRead(pppUptimePath, pvc_info, sizeof(pvc_info));
	/*If this file is exist, it's mean the cpe is dialup PPP Connection.*/
	if(strlen(pvc_info) > 0)
	{
		/*Caculate ppp link-up time*/
		now_time=time(&timep);
		start_time=atoi(pvc_info);
		connect_time=now_time-start_time;
		day=connect_time/SEC_PER_DAY;
		hour=(connect_time-SEC_PER_DAY*day)/SEC_PER_HOUR;
		min=(connect_time-SEC_PER_DAY*day-SEC_PER_HOUR*hour)/SEC_PER_MIN;
		sec=connect_time-SEC_PER_DAY*day-SEC_PER_HOUR*hour-SEC_PER_MIN*min;
		sprintf(tempValue,"%d:%2d:%2d:%2d",day,hour,min,sec);				
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add	
		GetWanNodeMapping(&pvc_index, &entry_index);
		sprintf(nodeName, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANPPPConnection.%d.Stats.UpTime", pvc_index, entry_index);
#else
		sprintf(nodeName, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANPPPConnection.%d.Stats.UpTime", pvc_index+1,entry_index+1);
#endif
		return 0;
	}
	else
	{
		return -1;
	}
}
#endif

int CwmpAddCustomInformParameter(){
	int state = 0;
#if defined(TCSUPPORT_CT_FJ)	
	int i = 0, ret =0;
	//Errors
	for(i = 0; i <  4; i++)
	{
		char tempValue[128] = {0};
		char nodeName[128] = {0};
		int reg;
		ret = get_ErrorsSent(i, nodeName, tempValue,"txerrors","ErrorsSent");
		if (0 == ret) {
			state = cwmpAddInformParameter(nodeName, tempValue, StringType);
			if (state!=0)
				return -1;
		}
	}
	//ErrorsReceived
	for(i = 0; i <  4; i++)
	{
		char tempValue[128] = {0};
		char nodeName[128] = {0};
		int reg;
		ret = get_ErrorsSent(i, nodeName, tempValue,"rxerrors","ErrorsReceived");
		if (0 == ret) {
			state = cwmpAddInformParameter(nodeName, tempValue, StringType);
			if (state!=0)
				return -1;
		}
	}
	//DiscardPacketsSent
	for(i = 0; i <  4; i++)
	{
		char tempValue[128] = {0};
		char nodeName[128] = {0};
		int reg;
		ret = get_ErrorsSent(i, nodeName, tempValue,"txdropped","DiscardPacketsSent");
		if (0 == ret) {
			state = cwmpAddInformParameter(nodeName, tempValue, StringType);
			if (state!=0)
				return -1;
		}
	}
	//EthernetBytesSent
	for (i = 0; i < 64; i++) {
		char tempValue[128] = {0};
		char nodeName[128] = {0};
		ret = get_StatsEthernetInfo(i, nodeName, tempValue,"txbytes","EthernetBytesSent");
		if (0 == ret) {
			state = cwmpAddInformParameter(nodeName, tempValue, StringType);
			if (state!=0)
				return -1;
		}
	}
	//EthernetBytesReceived
	for (i = 0; i < 64; i++) {
		char tempValue[128] = {0};
		char nodeName[128] = {0};
		ret = get_StatsEthernetInfo(i, nodeName, tempValue,"rxbytes","EthernetBytesReceived");
		if (0 == ret) {
			state = cwmpAddInformParameter(nodeName, tempValue, StringType);
			if (state!=0)
				return -1;
		}
	}
	//EthernetPacketsSent
	for (i = 0; i < 64; i++) {
		char tempValue[128] = {0};
		char nodeName[128] = {0};
		ret = get_StatsEthernetInfo(i, nodeName, tempValue,"txpackets","EthernetPacketsSent");
		if (0 == ret) {
			state = cwmpAddInformParameter(nodeName, tempValue, StringType);
			if (state!=0)
				return -1;
		}
	}
	//EthernetPacketsReceived
	for (i = 0; i < 64; i++) {
		char tempValue[128] = {0};
		char nodeName[128] = {0};
		ret = get_StatsEthernetInfo(i, nodeName, tempValue,"rxpackets","EthernetPacketsReceived");
		if (0 == ret) {
			state = cwmpAddInformParameter(nodeName, tempValue, StringType);
			if (state!=0)
				return -1;
		}
	}	
	//EthernetErrorsReceived
	for (i = 0; i < 64; i++) {
		char tempValue[128] = {0};
		char nodeName[128] = {0};
		ret = get_StatsEthernetInfo(i, nodeName, tempValue,"rxerrors","EthernetErrorsReceived");
		if (0 == ret) {
			state = cwmpAddInformParameter(nodeName, tempValue, StringType);
			if (state!=0)
				return -1;
		}
	}
	//EthernetDiscardPacketsReceived
	for (i = 0; i < 64; i++) {
		char tempValue[128] = {0};
		char nodeName[128] = {0};
		ret = get_StatsEthernetInfo(i, nodeName, tempValue,"rxdropped","EthernetDiscardPacketsReceived");
		if (0 == ret) {
			state = cwmpAddInformParameter(nodeName, tempValue, StringType);
			if (state!=0)
				return -1;
		}
	}
	//UpTime
	for (i = 0; i < 64; i++) {
		char tempValue[128] = {0};
		char nodeName[128] = {0};
		ret = get_ppp_uptime(i, nodeName, tempValue);
		if (0 == ret) {
			state = cwmpAddInformParameter(nodeName, tempValue, StringType);
			if (state!=0)
				return -1;
		}
	}
	
	/*if ITMS is online, send time once ,otherwise not*/
	if( 1 == ifSendTimesFlag )
	{
		ifSendTimesFlag = 0 ;
		state = cwmpAddInformParameter("InternetGatewayDevice.DeviceInfo.X_CT-COM_RegStatistics.OltRegTime", (char *)getOltRegTime(), UnsignedIntType);
		if (state!=0)
			return -1;

		state = cwmpAddInformParameter("InternetGatewayDevice.DeviceInfo.X_CT-COM_RegStatistics.ITMSRegTime", (char *)getITMSRegTime(), UnsignedIntType);
		if (state!=0)
			return -1;

		state = cwmpAddInformParameter("InternetGatewayDevice.DeviceInfo.X_CT-COM_RegStatistics.ITMSServiceTime", (char *)getITMSServiceTime(), UnsignedIntType);
		if (state!=0)
			return -1;
	}

#endif

	return 0;
}

/* <TCSUPPORT_CT_NAMECHGNOTIFY> not support VDSL now. */
void getWANAccessType(char* accessType)
{
#if defined(TCSUPPORT_CT_NAMECHGNOTIFY)
#if defined(TCSUPPORT_CT_PON)
	char linkSta[4] = "0";
	int ponsw = 0;

	tcapi_get("XPON_LinkCfg", "LinkSta", linkSta);
	ponsw = atoi(linkSta);

	if ( 2 == ponsw ) // epon
		strcpy(accessType, "EPON");
	else
		strcpy(accessType, "GPON");
#else
	strcpy(accessType,"DSL");
#endif
#else
#if defined(TCSUPPORT_CT_PON_JS)
	char linkSta[4] = "0";
	int ponsw = 0;

	tcapi_get("XPON_LinkCfg", "LinkSta", linkSta);
	ponsw = atoi(linkSta);

	if ( 2 == ponsw ) // epon
		strcpy(accessType,"EPON");
	else
		strcpy(accessType,"GPON");
#endif
#endif

	return;
}

int8 addDestinationAddressNode(int pvc_index){	
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_NAMECHGNOTIFY)
	char connection_name[256] = {0};	
	char nodeName[34] = {0};
	char vpi[10] = {0};
	char vci[10] = {0};	
	char WANIPDSLLinkCfg1_1Type[20] = {0};
	int8 ret = 0;
	int8 state = 0;	
	
	sprintf(nodeName, "Wan_PVC%d", pvc_index);		
	ret = tcapi_get(nodeName, "VPI", vpi);
	if(ret != 0){
		//node not exist;
		return 1;
	}
	
	tcapi_get(nodeName, "VCI", vci);		
	sprintf(WANIPDSLLinkCfg1_1Type, "PVC:%s/%s",vpi, vci);
	
	memset(connection_name, 0, sizeof(connection_name));
	sprintf(connection_name, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANDSLLinkConfig.DestinationAddress",pvc_index+1);
	state = cwmpAddInformParameter(connection_name, WANIPDSLLinkCfg1_1Type, StringType);
	if (state!=0)
		return -1;	
#endif

	return 0;
}

int8 addVLANIDMarkNode(int pvc_index, int flag){		
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_NAMECHGNOTIFY)
	char connection_name[256] = {0};	
	char nodeName[34] = {0};
	char mode[20] = {0};	
	char vlanid[20] = "0";
	int8 ret = 0;
	int8 state = 0;	
	
	sprintf(nodeName, "Wan_PVC%d", pvc_index);		
	ret = tcapi_get(nodeName, "VLANMode", mode);
	if(ret != 0){
		//node not exist;
		return 1;
	}
	
	if ( 0 == strcmp(mode, "TAG") ){
		tcapi_get(nodeName, "VLANID", vlanid);
	}	

	memset(connection_name, 0, sizeof(connection_name));
	if(flag == 1){		//EPON
		sprintf(connection_name, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.X_CT-COM_WANEponLinkConfig.VLANIDMark",pvc_index+1);
	}else{				//GPON
		sprintf(connection_name, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.X_CT-COM_WANGponLinkConfig.VLANIDMark",pvc_index+1);
	}
	
	state = cwmpAddInformParameter(connection_name,vlanid , StringType);
	if (state!=0)
		return -1;	
#endif

	return 0;
}

void getWANServiceListName(char* connection_name,int pvcindex, int entryindex)
{
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_NAMECHGNOTIFY)
		char nodeName[32] = {0};
		char isp[4] = {0};
		char bridge_type[32] = {0};

		sprintf(nodeName, "Wan_PVC%d_Entry%d", pvcindex, entryindex);
		tcapi_get(nodeName, "ISP", isp);
	
		if (!strcmp(isp, WAN_ENCAP_PPP)) {
			sprintf(connection_name, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANPPPConnection.%d.%sServiceList", pvcindex + 1, entryindex + 1, X_CT_COM);
		}
		else if (!strcmp(isp, WAN_ENCAP_DYN) || !strcmp(isp, WAN_ENCAP_STATIC)) {
			sprintf(connection_name, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANIPConnection.%d.%sServiceList", pvcindex + 1, entryindex + 1, X_CT_COM);
		}
		else {
			tcapi_get(nodeName, "BridgeMode", bridge_type);
			if (!strcmp(bridge_type, "IP_Bridged")) {
				sprintf(connection_name, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANIPConnection.%d.%sServiceList", pvcindex + 1, entryindex + 1, X_CT_COM);
			}
			else {
				/* default is pppoe bridge */
				sprintf(connection_name, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANPPPConnection.%d.%sServiceList",pvcindex + 1, entryindex + 1, X_CT_COM);
			}
		}
#endif
	
		return;
}


int8 addServiceListNode(int pvc_index, int entry_index)
{
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_NAMECHGNOTIFY)
	char connection_name[256] = {0};		
	char nodeName[49] = {0};
	char  serviceList[SERVICE_LIST_LEN] = {0};
	uint8 wanNodeIndex, pvcNodeEntry;
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	uint8 bufLen = 0;
	uint8 idx = 0;
	char *p = NULL;
	char  tmp_svrList[SERVICE_LIST_LEN] = {0};
#endif
	int8 state = 0;	

	sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
	if( 0 !=  tcapi_get(nodeName, "ServiceList", serviceList))
	{
		return 1;
	}
	else
	{
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
		if ( 0 != strcmp("OTHER", serviceList) )
		{
			bufLen = strlen(serviceList);
			for ( idx = 0; idx < bufLen; idx ++ )
			{
				if ( '_' == serviceList[idx] )
					serviceList[idx] = ',';
			}
			p = strstr(serviceList, "VOICE");
			if(p)
			{		
				strncpy(tmp_svrList,serviceList, (p-serviceList));
				strcat(tmp_svrList, "VOIP");
				strcat(tmp_svrList, (p+strlen("VOICE")));
				//memset(serviceList,0, sizeof(serviceList));
				strcpy(serviceList,tmp_svrList);
			}	
		}
#else
		if(strstr(serviceList, "TR069_INTERNET"))
		{
			memset(serviceList, 0, sizeof(serviceList));
			strcpy(serviceList,"TR069,INTERNET");
		}
#endif
	}

	memset(connection_name, 0, sizeof(connection_name));
	getWANServiceListName(connection_name, pvc_index, entry_index);

	state = cwmpAddInformParameter(connection_name, serviceList, StringType);
	if (state!=0)
		return -1;	
#endif
	return 0;
}


int8 addWanParameterNode(char* accessType){	
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_NAMECHGNOTIFY)
	char connection_name[256] = {0};		
	char nodeName[64] = {0};
	int flag = 0;
	int i = 0;
	int j = 0;
	int8 state = 0;	
	
	 if(strstr(accessType,"EPON") != NULL){
		//EPON
		flag = 1;
	}else if(strstr(accessType,"GPON") != NULL){
		//GPON		
		flag = 2;
	}else if(strstr(accessType,"DSL") != NULL){
		//DSL
		flag = 3;
	}

	for (i = 0; i <  MAX_WAN_NODE; i++){
		state = 0;
		switch(flag){
			case 1: 
			case 2:
				state = addVLANIDMarkNode(i,flag);
				if (state < 0)
					return -1;					
				break;
			case 3:
				state = addDestinationAddressNode(i);	
				if (state < 0)
					return -1;
				break;
			default:
				break;
		}	

		if(state == 1){	//wan is not exist
			continue;
		}

		for (j =0 ; j < TOTAL_REMOTE_ENTRY_NODE; j++) {
			memset(nodeName,sizeof(nodeName),0);
			sprintf(nodeName, "Wan_PVC%d_Entry%d", i, j);
			
			state = addServiceListNode(i,j);
			if (state < 0){
				return -1;	
			}
		}
	}
#endif
	return 0;
}

int8 cwmpBuildNameChangeParameters(void){	
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_NAMECHGNOTIFY)
	char connection_name[256] = {0};		
	char nodeName[64] = {0};
	char tmpBuf[64] = {0};	
	char accessType[64] = {0};
	int8 state = 0;	
	int maxNum = 4;
	int i = 0;
#define MAX_BSSID_NUM_WLAN 4

	//WANAccessType
	memset(accessType,0,sizeof(accessType));
	getWANAccessType(accessType);

#if !defined(TCSUPPORT_CT_ADSL_TJ)
	//tcdbg_printf("[%s]accessType = %s\n",__FUNCTION__,accessType);
	state = cwmpAddInformParameter("InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig.WANAccessType", accessType, StringType);
	if (state!=0)
		return -1;
#endif

	//DestinationAddress,VLANIDMark,X_CT-COM_ServiceList
	if(addWanParameterNode(accessType) != 0)
		return -1;
	
	//MACAddress			
#if defined(TCSUPPORT_CT_2PWIFI) || defined(TCSUPPORT_CUC_2PORT) || defined(TCSUPPORT_CT_2PORTS)
	maxNum = 2;
#else
	maxNum = 4;
#endif
	for(i=0; i<maxNum; i++){
		memset(tmpBuf,0,sizeof(tmpBuf));
		memset(nodeName,0,sizeof(nodeName));		
		sprintf(nodeName, "Info_eth0.%d", i+1);
		tcapi_get(nodeName, "hwaddr", tmpBuf);
		
		memset(connection_name, 0, sizeof(connection_name));
		sprintf(connection_name, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.%d.MACAddress",i+1);
		state = cwmpAddInformParameter(connection_name, tmpBuf, StringType);
		if (state!=0)
			return -1;
	}

#ifdef TCSUPPORT_WLAN
	//ssid
	for(i=0; i<MAX_BSSID_NUM_WLAN; i++){
		memset(tmpBuf,0,sizeof(tmpBuf));
		memset(nodeName,0,sizeof(nodeName));		
		sprintf(nodeName, "WLan_Entry%d", i);
		if(tcapi_get(nodeName, "SSID", tmpBuf) != 0)  //wlan not exist
			continue;
		
		memset(connection_name, 0, sizeof(connection_name));
		sprintf(connection_name, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.%d.SSID",i+1);
		state = cwmpAddInformParameter(connection_name, tmpBuf, StringType);
		if (state!=0)
			return -1;
	}
#endif
#endif
	return 0;
}

#if defined(TCSUPPORT_CT_2PORTS) 
#if defined(TCSUPPORT_CT_1PORT) 
char devreg_devType[32] = "E8C 1 "
#else
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
char devreg_devType[32] = "E8B 2 "
#else
char devreg_devType[32] = "E8C 2+1 "
#endif
#endif
#else
#if defined(TCSUPPORT_CT_PON_CY)
char devreg_devType[32] = "E8C 4+1 "
#else
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_E8B_ADSL)
char devreg_devType[32] = "E8B 4 "
#else
char devreg_devType[32] = "E8C 4+1 "
#endif
#endif
#endif
#if defined(TCSUPPORT_WLAN)
#if defined(TCSUPPORT_CT_PON_YN)
	"AP"
#else
	"wifi"
#endif
#else
#if defined(TCSUPPORT_CT_PON_YN) || defined(TCSUPPORT_CT_PON_FHT)
	"NoAP"
#else
	"nowifi"
#endif
#endif
	;

pthread_mutex_t midware_task_lock;
sem_t SEM_MW_IMFORM;
int iTr069Enable = 0;
uint8 MWValueChangeFlag = 0;
uint8 MWExitFlag = 0;
uint32 MWWanChangeIndex[2] = {0,0};
uint8 operationFlag = 0;
uint8 MWActiveNotifyFlag = 0;
uint8 ITMSActiveNotifyFlag = 0;
pthread_t p_MonitorCollector = 0;
uint8 button_detect_flag[2] = {0};

int processBMSAction_Open(){
	int ret = 0;
	
#if defined(TCSUPPORT_CT_UPNP_DM)
	ret = processBMSAction();
#endif
	return ret;
}

int ProcessUPnPDMMessage_Open(char* buf){
	int ret = 0;
	
#if defined(TCSUPPORT_CT_UPNP_DM)
	ret = ProcessUPnPDMMessage(buf);
#endif
	return ret;
}

int getTR69ErrCodeByUPnPDM_Open(){
	int ret = 0;
	
#if defined(TCSUPPORT_CT_UPNP_DM)
	ret = getTR69ErrCodeByUPnPDM();
#endif
	return ret;
}

int getUPnPDownloadInstance_Open(void){
	int ret = 0;
	
#if defined(TCSUPPORT_CT_UPNP_DM)
	ret = getUPnPDownloadInstance();
#endif
	return ret;
}

char* getOperationStatus_Open(int index){
	char* ret = NULL;
	
#if defined(TCSUPPORT_CT_UPNP_DM)
	ret = getOperationStatus(index);
#endif
	return ret;
}

int create_socket_client_Open(void){
	int ret = 0;

#if defined(TCSUPPORT_CT_UPNP_DM)
	ret = create_socket_client();
#endif
	return ret;
}

int ProcessUPnPAddDelDevAction_Open(int popcode,int devnum){
	int ret = 0;

#if defined(TCSUPPORT_CT_UPNP_DM)
	ret = ProcessUPnPAddDelDevAction(popcode,devnum);
#endif
	return ret;
}

int iUPnPDMModifyFlag  = 0;

int doUPnPRoutineActionList_Open(DeviceNodePtr DeviceListNode){	
	int ret = 0;

#if defined(TCSUPPORT_CT_UPNP_DM)
	ret = doUPnPRoutineActionList(DeviceListNode);
#endif
	return ret;
}

int freeUPnPRoutineActionList_Open(){
	int ret = 0;

#if defined(TCSUPPORT_CT_UPNP_DM)
	ret = freeUPnPRoutineActionList();
#endif
	return ret;
}

void setMWExitFlag(uint8 flag){
	MWExitFlag = flag;
}

static uint8 nameChangeFlag = 0;
static uint8 LoidChangeFlag = 0;
static uint8 loid_Empty_flag = 0;
uint8 getNameChangeFlag(void){
	return nameChangeFlag;
}

void setNameChangeFlag(uint8 value){
	nameChangeFlag = value;
}

uint8 getLoidChangeFlag(void){
	return LoidChangeFlag;
}

void setLoidChangeFlag(uint8 value){
	LoidChangeFlag = value;
}

uint8 getLoidEmptyFlag(void){
	return loid_Empty_flag;
}

void setLoidEmptyFlag(uint8 value){
	loid_Empty_flag = value;
}

int stb_notify = 0;
int stb_count = 0;
char stb_mac_list[256] = {0};
#define STB_LIST_PATH "/proc/br_fdb_stb/stb_list"

int getSTBListNotifyFlag()
{
	return stb_notify;
}

int setSTBListNotifyFlag(int flag)
{
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_STBMAC_REPORT)
	stb_notify = flag;
	if ( stb_notify )
		return updateSTBList();
#endif
	return 0;	
}

int updateSTBList()
{
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_STBMAC_REPORT)
	FILE *f = NULL;
	char tmpbuf[512] = {0};
	char statebuf[12] = {0};
	int stbcount = 0;

	f = fopen(STB_LIST_PATH, "r");
	if ( f )
	{
		memset(tmpbuf, 0 , sizeof(tmpbuf));
		fgets(tmpbuf, sizeof(tmpbuf) - 1, f);
		sscanf(tmpbuf, "%[^=]=%d=%[^=]", statebuf, &stbcount, stb_mac_list);
		fclose(f);
	}

	stb_count = stbcount;
	if ( 0 == stb_count )
		memset(stb_mac_list, 0, sizeof(stb_mac_list));
#endif
	return stb_count;
}

char *getSTBMACList()
{
	return stb_mac_list;
}

int getSTBCount()
{
	return stb_count;
}

char *getSTBCountStr()
{
#if defined(TCSUPPORT_CT_PON_JX)
	static char stb_count_str[12] = {0};

	sprintf(stb_count_str, "%d", getSTBCount());
	return stb_count_str;
#else
	return "";
#endif
}

char *getIGMPEnableStr()
{
#if defined(TCSUPPORT_CT_PON_JX)
	static char igmpEnable[8] = {0};
	char IGMPEnableValue[10] = {0};

	tcapi_get("IGMPproxy_Entry", "IGMPEnable", IGMPEnableValue);
	if ( 0 == strcmp(IGMPEnableValue, "Yes") )
		strcpy(igmpEnable, "true");
	else
		strcpy(igmpEnable, "false");

	return igmpEnable;
#else
	return "";
#endif
}

char *getProxyEnableStr()
{
#if defined(TCSUPPORT_CT_PON_JX)
	static char proxyEnable[8] = {0};
	char IGMPProxyValue[10] = {0};

	tcapi_get("IGMPproxy_Entry", "Active", IGMPProxyValue);
	if ( 0 == strcmp(IGMPProxyValue, "Yes") )
		strcpy(proxyEnable, "true");
	else
		strcpy(proxyEnable, "false");

	return proxyEnable;
#else
	return "";
#endif
}

char *getSnoopingEnableStr()
{
#if defined(TCSUPPORT_CT_PON_JX)
	static char snoopingEnable[8] = {0};
	char IGMPSnoopingValue[10] = {0};

	tcapi_get("Lan_IgmpSnoop","igmpsnoopEnable", IGMPSnoopingValue);	
	if ( 0 == strcmp(IGMPSnoopingValue, "Yes") )
		strcpy(snoopingEnable, "true");
	else
		strcpy(snoopingEnable, "false");

	return snoopingEnable;
#else
	return "";
#endif
}

void checkAndDoMonitorCollectorTask(void) {
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
	if(g_MonitorCollectorEnable == 0) {
		if(p_MonitorCollector == 0) {//monitor collector task is not running
			tcdbg_printf("\r\ncreate thread to do monitor Collector!");
			cwmpMonitorCollectorTaskStart();
		}
	}
#endif
}

int countNewTeleNode_Open()
{
	int ret = 0;

#if defined(TCSUPPORT_CT_MIDWARE)
	ret = countNewTeleNode();
#endif
	return ret;
}

uint8 getTr069RestartItself(void){
#if defined(TCSUPPORT_CT_MIDWARE)
	FILE *fp = NULL;
	if(iTr069Enable == 0 ) return 1;
	if((fp = fopen("/var/run/restart_tr069_itself","r")) == NULL){
		return 1;
	}
	fclose(fp);
#endif
	return 0;
}

void setMWWanChangeIndex(uint16 index){
#if defined(TCSUPPORT_CT_MIDWARE)
	//tc_printf("setMWWanChangeIndex:%d!\n",index);
	//MWWanChangeIndex = index;
	if(index == 0xff){
		MWWanChangeIndex[0] = 0;
		MWWanChangeIndex[1] = 0;
		return;
	}
	MWWanChangeIndex[index/32] |= (1 << (index%32));
	//tc_printf("setMWWanChangeIndex[%d]:%d,%4x!\n",index/32,index%32,MWWanChangeIndex[index/32]);
#endif
	return;
}

void midware_resovle_Open(void){
#if defined(TCSUPPORT_CT_MIDWARE)
	midware_resovle();
#endif
}

void setMWActiveNotifyFlag(uint8 value)
{
#if defined(TCSUPPORT_CT_MIDWARE)
	MWActiveNotifyFlag = value;
#endif
	return;
}

void setITMSActiveNotifyFlag(uint8 value)
{
#if defined(TCSUPPORT_CT_MIDWARE)
	ITMSActiveNotifyFlag = value;
#endif
	return;
}

uint16 getMWWanChangeIndex(uint16 index){
#if defined(TCSUPPORT_CT_MIDWARE)
	if(index == 0xff) return (MWWanChangeIndex[0] | MWWanChangeIndex[1]);
	return (MWWanChangeIndex[index/32] & (1 << index%32));
#endif
	return 0;
}

#if !defined(TCSUPPORT_C1_CUC) && !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8   RebuildAlarmTable(){
	return 0;
}
#endif

void setOperationFlag(uint8 value)
{
	operationFlag = value;
}

uint8 GetTr069Enable()
{
	uint8 flag = 1;
	char CwmpTr069Enable[10]="0";
	
#if defined(TCSUPPORT_CT_MIDWARE)
	if(getCwmpAttrValue(Tr069Enable,CwmpTr069Enable,sizeof(CwmpTr069Enable)) != 0)
	{
		memset(CwmpTr069Enable,'\0',sizeof(CwmpTr069Enable));
		setCwmpAttrValue(Tr069Enable,CwmpTr069Enable);
	}
	if(strcmp(CwmpTr069Enable,"0")==0)
		flag =0 ;
	else if(strcmp(CwmpTr069Enable,"1")==0)
		flag = 1;
	else if(strcmp(CwmpTr069Enable,"2")==0)
		flag = 2;
#endif

	return flag;
}

void ctMidwareInit()
{
#if defined(TCSUPPORT_CT_MIDWARE)
	midwareInit();
#endif
	return;
}

uint8  downloadOK = 0;
uint8 getdownloadOKFlag()
{
	return downloadOK;
}

void setdownloadOKFlag(uint8 value)
{
	downloadOK = value;
}

void ctdoCwmpIAD()
{
#ifdef WITHVOIP 
#if defined(TCSUPPORT_CT)
	doCwmpIAD();
#endif
#endif
	return;
}

uint8 cardNotifyFlag = 0;
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
char gSimCardNo[32] = {0};
static volatile uint8 simCardNofityFlag = 0;
static volatile uint8 simCardWriteFlag = 0;
#endif

char* getSimCardCardNo(void)
{
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	memset(gSimCardNo, 0, sizeof(gSimCardNo));
	tcapi_get(SIM_CARD_NODE_NAME,"CardSN",gSimCardNo);

	return gSimCardNo;
#endif
	return "";
}


int isCardExist(void){
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	char tmp[16] = {0};
	uint8 ret_val = 0;

	if( tcapi_get("SimCard_Entry", "cardExist",tmp) == 0){
		ret_val = atoi(tmp);
	}
	return ret_val;
#endif
	return 0;
}

int isSimCardAuthSuccess(void){
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	char tmp[16] = {0};
	uint8 ret_val = 0;

	if( tcapi_get("SimCard_Entry", "simAuthRet",tmp) == 0){
		ret_val = atoi(tmp);
	}
	return ret_val;
#endif
	return 0;
}

int isSimFileNodeModify(cfgNode_ptr gCfgNode){
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	cfgNode_ptr pCfgNode = NULL;
	int ret = 0;
	
	if(isCardExist() && isSimCardAuthSuccess()){
		for ( pCfgNode = gCfgNode; pCfgNode->nodeName != NULL; pCfgNode++ ) {
			//tcdbg_printf("[%s:%d]pCfgNode->nodeName=%s\n",__FUNCTION__,__LINE__,pCfgNode->nodeName);
			if ( pCfgNode->settingFlag == CFG_YES ) {
				if(isNodeSupportInSimCard(pCfgNode->nodeName)){
					ret = 1;
				}
			}
		}
	}

	return ret;
#endif
	return 0;
}


uint8 getSimFileNeedModifyFlag(void)
{
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	char tmp[16] = {0};
	uint8 ret_val = 0;

	if( tcapi_get("SimCard_Entry", "isNeedUpdateSim",tmp) == 0){
		ret_val = atoi(tmp);
	}
	return ret_val;
#endif
	return 0;
}
	
void setSimFileNeedModifyFlag(uint8 value)
{
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	char cardExist[8] = {0};
	tcapi_get( SIM_CARD_NODE_NAME, "cardExist", cardExist);
	if(strcmp(cardExist,"1") == 0){
		if(value == 1){
			tcapi_set("SimCard_Entry","isNeedUpdateSim", "1");
		}else{
			tcapi_set("SimCard_Entry","isNeedUpdateSim", "0");
		}
	}
#endif
	return;
}

uint8 getSimCardAuthFlag(void)
{
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	char tmp[16] = {0};
	uint8 ret_val = 0;

	if( tcapi_get("SimCard_Entry", "isCardKeySet",tmp) == 0){
		ret_val = atoi(tmp);
	}

	return ret_val;
#endif
	return 0;
}
void setSimCardAuthFlag(uint8 value)
{
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	if(value == 1){
		tcapi_set("SimCard_Entry","isCardKeySet", "1");
	}else{
		tcapi_set("SimCard_Entry","isCardKeySet", "0");
	}
#endif
	return;
}

uint8 getCardNotifyFlag(void)
{
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	return simCardNofityFlag;
#endif
	return 0;
}

void setCardNotifyFlag(uint8 value)
{
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	simCardNofityFlag = value;
#endif
	return;
}

uint8 getCardWriteFlag(void)
{
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	return simCardWriteFlag;
#endif
	return 0;
}

void setCardWriteFlag(uint8 value)
{
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	simCardWriteFlag = value;
#endif
	return;
}

int isNeedWriteSim(){
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	char cardExist[16] = {0};
	char simAuthRet[16] = {0};
	char itmsActionStart[16] = {0};

	//only do write sim file when adminstrator login
	if( tcapi_get("SimCard_Entry", "itmsActionStart",itmsActionStart) == 0 && strcmp(itmsActionStart,"0") == 0 
		&& tcapi_get("SimCard_Entry", "cardExist",cardExist) == 0 && strcmp(cardExist,"1") == 0 
		&& tcapi_get("SimCard_Entry", "simAuthRet",simAuthRet) == 0 && strcmp(simAuthRet,"1") == 0 ){
		return 1;
	}
#endif

	return 0;
}

int getItmsActionStartFlag(void){
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	char tmp[16] = {0};
	uint8 ret_val = 0;

	if( tcapi_get("SimCard_Entry", "itmsActionStart",tmp) == 0){
		ret_val = atoi(tmp);
	}
	return ret_val;
#endif
	return 0;
}

int getCpeFlag(void){
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	char tmp[16] = {0};
	uint8 ret_val = 0;

	if( tcapi_get("SimCard_Entry", "cpeFlag",tmp) == 0){
		ret_val = atoi(tmp);
	}
	return ret_val;
#endif
	return 0;
}

int cwmp_get_dslite_stateviacfg(char *nodeName)
{
#if defined(TCSUPPORT_CT)

	char dslite_enable[8] = {0};
	char serviceList[64] = {0};
	char isp[5] = {0};

	if ( NULL == nodeName )
		return 0;

	if( 0 != tcapi_get(nodeName, "DsliteEnable", dslite_enable) )
		return 0;
	if( 0 != tcapi_get(nodeName, "ServiceList", serviceList) )
		return 0;
	if( 0 != tcapi_get(nodeName, "ISP", isp) )
		return 0;

	if( NULL != strstr(serviceList, "INTERNET")
		&& 0 != strcmp(isp, "3")  // not BRIDGE_MODE
		&& 0 == strcmp("Yes", dslite_enable))
		return 1;
	else
		return 0;
#else
	return 0;
#endif
}

int cwmp_checkWan_by_name(char *wan_if_name, int *ipMode)
{
#if defined(TCSUPPORT_CT)
	int if_index;
	char nodeName[64] = {0};
	char temp[32] = {0};	
	char str_ipversion[16] = {0};
	
	if_index = get_wanindex_by_name(wan_if_name);

	if ( if_index < 0 )
		return -1;

	snprintf(nodeName, sizeof(nodeName) - 1, "Wan_PVC%d_Entry%d",
		if_index/MAX_SMUX_NUM, if_index%MAX_SMUX_NUM);

	if ( 0 != tcapi_get(nodeName, "Active", temp)
		|| 0 != strcmp(temp, "Yes") )
		return -1;


	if ( 0 != tcapi_get(nodeName, "IPVERSION", str_ipversion) )
		strcpy(str_ipversion, "IPv4");
	
	if ( 0 == strcmp(str_ipversion,"IPv4") )
		*ipMode= 1;
	else
	{
		if ( 1 == cwmp_get_dslite_stateviacfg(nodeName) )
			*ipMode = 4;
		else if ( 0 == strcmp(str_ipversion,"IPv6") )
			*ipMode= 2;
		else
			*ipMode= 3;
	}

	memset(nodeName, 0, sizeof(nodeName));
	sprintf(nodeName, "WanInfo_Entry%d", if_index);

	memset(temp, 0, sizeof(temp));
	if ( 2 != *ipMode && 0 == tcapi_get(nodeName, "GateWay", temp)
		&& 0 != temp[0] && 0 != strcmp(temp,"N/A") )
		return 0;

	memset(temp, 0, sizeof(temp));
	if ( 1 != *ipMode && 0 == tcapi_get(nodeName, "GateWay6", temp)
		&& 0 != temp[0] && 0 != strcmp(temp,"N/A") )
		return 0;
#endif

	return -1;	
}

char *X_EVENT_CODE(const char *event)
{
	static char event_code[128] = {0};
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#define CUSTOM_EVENT_CODE "X_CU_"
#else
#if defined(TCSUPPORT_CMCC)
#define CUSTOM_EVENT_CODE "X CMCC "
#else
#define CUSTOM_EVENT_CODE "X CT-COM "
#endif
#endif

	if ( !event )
		return "";

	snprintf(event_code, sizeof(event_code) - 1,
		CUSTOM_EVENT_CODE"%s", event);

	return event_code;
}

#if defined(TCSUPPORT_CT_PMINFORM)
int GetCPURate(char *valueString){

	char CPUName[80] = {0};
	char temp[80] = {0};
	int tmp = 0;
	unsigned int  user = 0;
    unsigned int  nice = 0;
    unsigned int  system = 0;
    unsigned int  idle = 0;
    unsigned int  iowait = 0;
    unsigned int  irq = 0;
    unsigned int  softirq = 0;
    unsigned int  total = 0;	
	
	memset(CPUName,0, sizeof(CPUName));
	memset(temp,0, sizeof(temp));	
	fileRead("/proc/stat", temp, sizeof(temp));
	if(strlen(temp) == 0)
		return 0;
	sscanf(temp, "%s %d %d %d %d %d %d %d", CPUName, &user, &nice,&system, 
			&idle, &iowait, &irq, &softirq);
	total = user + nice + system + idle + iowait + irq + softirq;
	
	tmp=(total-idle)*100/total;
	sprintf(valueString,"%02d" ,tmp);
	return 0;
}

int GetMemRate(char *valueString){

	
	char temp[80] = {0};
	char MemTotalName[80] = {0};
	char MemFreeName[80] = {0};
	char KB1[10] = {0};
	char KB2[10] = {0};
	int MemTotal=0;
	int MemFree=0;	
	int tmp=0;
	
	memset(temp,0, sizeof(temp));	
	fileRead("/proc/meminfo", temp, sizeof(temp));
	if(strlen(temp) == 0)
		return 0;
	sscanf(temp, "%s %d %s %s %d %s ", MemTotalName, &MemTotal, &KB1,&MemFreeName, 
				&MemFree, &KB2);
	
	tmp=(MemTotal-MemFree)*100/MemTotal;

	sprintf(valueString,"%02d" ,tmp);
	return 0;
}

int GetSER(char *valueString){

	
	char ErrorCode[32] = "0";
	char pktsRecv[32] = "0";
	int temp=0;

	tcapi_get("Info_PonWanStats","RxCrcCnt",ErrorCode);
	tcapi_get("Info_PonWanStats","RxFrameCnt",pktsRecv);
	temp = atoi(ErrorCode)*100/atoi(pktsRecv);
	sprintf(valueString,"%02d" ,temp);
	return 0;
}

int GetPLR(char *valueString){

	
	char PacketLost[32] = "0";
	char pktsSend[32] = "0";
	int temp=0;

	tcapi_get("Info_PonWanStats","TxDropCnt",PacketLost);
	tcapi_get("Info_PonWanStats","TxFrameCnt",pktsSend);
	temp = atoi(PacketLost)*100/atoi(pktsSend);
	sprintf(valueString,"%02d" ,temp);
	return 0;
}

int GetTEMP(char *valueString){	
	
	char temperature[32] = "0";
	double tmp = 0;
		
		tcapi_get("Info_PonPhy","Temperature",temperature);
		if(strcmp(temperature,"N/A"))
		{
			tmp = atof(temperature);
			if(tmp >= (1<<15))
				tmp-=(1<<15);	
				tmp/=256;
			sprintf(valueString,"%.1f",tmp);
		}	
	
	return 0;
}

#define FRT_TIME "%4d%02d%02d%02d%02d%02d"
#define NEWLINE "\r\n"
#define FRT_BODYLINE "%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s"NEWLINE
#define FRT_FILE_NAME "%s_"FRT_TIME"_%06d.csv" /* LOID_DATE_LONG*/
#define FRT_FILE_PATH PMINFORM_TFTP_TMP"/%s"
/* LOID|Manufacturer|productClass|SoftwareVersion|HardwareVersion|MACAddress|IPAddress|UserID|RptDataNUM */
#define FRT_HEADER "%s|%s|%s|%s|%s|%s|%s|%s|%lu"NEWLINE
#define ZERO_X(x) memset(x, 0, sizeof(x))
#define FREC_COLLEC_MAXLEN 1024
#define DEF_CAL_TIME 300 /* 60 * 5 */

pthread_t PMInformThread = 0; /* cpe collector thread */

struct frec_collection
{
	char body[FREC_COLLEC_MAXLEN];
	struct frec_collection *next;
};

struct frec_collection* collectfrecHeader = NULL;
/* CPURate(02)|MemRate(02)|SER(02)|ErrorCode|PLR(02)|PacketLost|TEMP(.1)|Startpoint|Endpoint|UpData(.03)|DownData(.03)|WLANDeviceMAC |LANDeviceMAC */
struct frec_collection **addCollectionFrec(struct frec_collection **header
	, int sw_cpurate, uint32_t cpurate
	, int sw_memrate, uint32_t memrate
	, int sw_ser, uint32_t ser
	, int sw_errcode, uint32_t errcode
	, int sw_plr, uint32_t plr
	, int sw_pktlost, uint32_t pktlost
	, int sw_temp, double temp
	, char *start_p
	, char *end_p
	, int sw_updata, double updata
	, int sw_dwdata, double dwdata
	, int sw_wlandevmac, char *wlandevmac
	, int sw_landevmac, char *landevmac)
{
	struct frec_collection **new_frec_collection = header;
	char c_cpurate[8] = {0}, c_memrate[8] = {0}, c_ser[8] = {0}, c_errcode[24] = {0};
	char c_plr[8] = {0}, c_pktlost[24] = {0}, c_temp[24] = {0}, c_updata[24] = {0};
	char c_dwdata[24] = {0};
	
	if ( !header )
		return -1;

	while ( *new_frec_collection )
	{
		new_frec_collection = &((*new_frec_collection)->next);
	}

	*new_frec_collection = malloc(sizeof(struct frec_collection));
	if ( !(*new_frec_collection) )
		return NULL;
	(*new_frec_collection)->next = NULL;
	memset((*new_frec_collection)->body, 0, FREC_COLLEC_MAXLEN);
	/* CPURate(02)|MemRate(02)|SER(02)|ErrorCode|PLR(02)|PacketLost|TEMP(.1)|Startpoint|Endpoint|UpData(.03)|DownData(.03)|WLANDeviceMAC |LANDeviceMAC */
	snprintf(c_cpurate, sizeof(c_cpurate) - 1, "%02u", cpurate);
	snprintf(c_memrate, sizeof(c_memrate) - 1, "%02u", memrate);
	snprintf(c_ser, sizeof(c_ser) - 1, "%02u", ser);
	snprintf(c_errcode, sizeof(c_errcode) - 1, "%lu", errcode);
	snprintf(c_plr, sizeof(c_plr) - 1, "%02u", plr);
	snprintf(c_pktlost, sizeof(c_pktlost) - 1, "%lu", pktlost);
	snprintf(c_temp, sizeof(c_temp) - 1, "%.1f", temp);
	snprintf(c_updata, sizeof(c_updata) - 1, "%.3f", updata);
	snprintf(c_dwdata, sizeof(c_dwdata) - 1, "%.3f", dwdata);
	snprintf((*new_frec_collection)->body, FREC_COLLEC_MAXLEN - 1, FRT_BODYLINE
			, ( sw_cpurate ? c_cpurate : "" )
			, ( sw_memrate ? c_memrate : "" )
			, ( sw_ser ? c_ser : "" )
			, ( sw_errcode ? c_errcode : "" )
			, ( sw_plr ? c_plr : "" )
			, ( sw_pktlost ? c_pktlost : "" )
			, ( sw_temp ? c_temp : "" )
			, start_p
			, end_p
			, ( sw_updata ? c_updata : "" )
			, ( sw_dwdata ? c_dwdata : "" )
			, ( sw_wlandevmac ? wlandevmac : "" )
			, ( sw_landevmac ? landevmac : "" ));
	/*tcdbg_printf("\n[%s]new addr[%X]\n", __FUNCTION__, (*new_frec_collection));*/

	return new_frec_collection;
}
/* Flush collection records to file and free memoy */
int flushCollectionFrec2file(FILE *fp, struct frec_collection **header)
{
	struct frec_collection *cur_frec_collection = *header;
	struct frec_collection *temp = NULL;

	if ( !fp || !(*header) )
		return -1;

	while ( cur_frec_collection )
	{
		fputs(cur_frec_collection->body, fp);

		temp = cur_frec_collection->next;
		/*tcdbg_printf("\n[%s]free addr[%X]\n", __FUNCTION__, cur_frec_collection);*/
		free(cur_frec_collection);
		cur_frec_collection = temp;
	}
	*header = NULL;

	return 0;
}
/* free memoy */
int freeCollectionFrec(struct frec_collection **header)
{
	struct frec_collection *cur_frec_collection = *header;
	struct frec_collection *temp = NULL;

	if ( !(*header) )
		return -1;

	while ( cur_frec_collection )
	{
		temp = cur_frec_collection->next;
		free(cur_frec_collection);
		cur_frec_collection = temp;
	}
	*header = NULL;

	return 0;
}


/* Check PMInform is ntp sync succeed  */
int isNTPSyncSucc(void)
{
	char ret[8]= {0};

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_NTPST, ret);
	if ( 0 == strcmp(ret, PMINFORM_VAL_YES) )
		return 1;

	return 0;
}
/* Upload logfile via tftp  */
int doTftpUpload(char *logfile)
{
	char strServerUrl[256] = {0}, cmdbuf[256] = {0}, cwmp_interface[32] = {0}, strServerIP[32] = {0};
	ip4a dnsQueryResult = 0;


	if ( !logfile )
	{
		tcdbg_printf("\n[%s]logfile NULL!\n", __FUNCTION__);
		return -1;
	}

	if ( 0 != tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_LSURL, strServerUrl)
		|| 0 == strServerUrl[0] )
	{
		tcdbg_printf("\n[%s]LogServerUrl is empty!\n", __FUNCTION__);
		return -1;
	}

	if ( 0 != tcapi_get("CwmpRoute_Entry", "ifName0", cwmp_interface)
		|| 0 == cwmp_interface[0] )
	{
		tcdbg_printf("\n[%s]Cwmp wan interface is empty!\n", __FUNCTION__);
		return -1;
	}

	ZERO_X(cmdbuf);
	snprintf(cmdbuf, sizeof(cmdbuf) - 1, "%s %s\n", strServerUrl, cwmp_interface);
	doValPut(PMINFORM_DOMAIN_CONF_PATH, cmdbuf);

	dnsQueryResult = resolveName(strServerUrl);
	unlink(PMINFORM_DOMAIN_CONF_PATH);
	if ( !dnsQueryResult )
	{
		tcdbg_printf("\n[%s]strServerUrl[%s] resolve failed !\n", __FUNCTION__, strServerUrl);
		return -1;
	}
	else
	{
		strcpy(strServerIP, inet_ntoa(dnsQueryResult));
	}

	/* /usr/bin/tftp -p -r %s %s --> /usr/bin/tftp -p -i $interface -r $fileName $serverIp */
	snprintf(cmdbuf, sizeof(cmdbuf) - 1, PMINFORM_TFTP_CMD
			, cwmp_interface, logfile, strServerIP);
	system(cmdbuf);
	tcdbg_printf("\n[%s]cmdbuf=[%s] !\n", __FUNCTION__, cmdbuf);

	return 0;
}
/* init time and line num  */
int initCollect(uint32 *p_lutime, uint32 *p_lctime, time_t *pmain, time_t *pstart)
{
	char lutime[12] = {0}, lctime[12] = {0};

	if ( !p_lutime || !p_lctime || !pmain || !pstart )
		return -1;
	
	/* LogUploadInterval */
	if ( 0 != tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_LUINTVAL, lutime) )
		strcpy(lutime, "3600");
	sscanf(lutime, "%lu", p_lutime);
	/* LogCountInterval */
	if ( 0 != tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_LCINTVAL, lctime) )
		strcpy(lctime, "300");
	sscanf(lctime, "%lu", p_lctime);

	time(pmain);
	time(pstart);

	return 0;
}
/* init switch for each data  */
int initSWCollect(int *sw_cpurate, int *sw_memrate, int *sw_ser, int *sw_errcode, int *sw_plr,
				   int *sw_pktlost, int *sw_temp, int *sw_updata, int *sw_dwdata, int *sw_wlandevmac, int *sw_landevmac)
{
	char swVal[8] = {0};

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_CPURENABLE, swVal);
	*sw_cpurate = ( 0 == strcmp(PMINFORM_VAL_YES, swVal) ) ? 1 : 0;

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_MEMRENABLE, swVal);
	*sw_memrate = ( 0 == strcmp(PMINFORM_VAL_YES, swVal) ) ? 1 : 0;

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_SERENABLE, swVal);
	*sw_ser = ( 0 == strcmp(PMINFORM_VAL_YES, swVal) ) ? 1 : 0;

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_ERRCODEENABLE, swVal);
	*sw_errcode = ( 0 == strcmp(PMINFORM_VAL_YES, swVal) ) ? 1 : 0;

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_PLRENABLE, swVal);
	*sw_plr = ( 0 == strcmp(PMINFORM_VAL_YES, swVal) ) ? 1 : 0;

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_PKTLOSTENABLE, swVal);
	*sw_pktlost = ( 0 == strcmp(PMINFORM_VAL_YES, swVal) ) ? 1 : 0;

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_TEMPENABLE, swVal);
	*sw_temp = ( 0 == strcmp(PMINFORM_VAL_YES, swVal) ) ? 1 : 0;

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_UPDATAENABLE, swVal);
	*sw_updata = ( 0 == strcmp(PMINFORM_VAL_YES, swVal) ) ? 1 : 0;

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_DWDATAENABLE, swVal);
	*sw_dwdata = ( 0 == strcmp(PMINFORM_VAL_YES, swVal) ) ? 1 : 0;

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_WLANDEVMACENABLE, swVal);
	*sw_wlandevmac = ( 0 == strcmp(PMINFORM_VAL_YES, swVal) ) ? 1 : 0;
	
	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_LANDEVMACENABLE, swVal);
	*sw_landevmac = ( 0 == strcmp(PMINFORM_VAL_YES, swVal) ) ? 1 : 0;

	return 0;
}

/* check time data valid in  CPECollection_Entry */
int checkLocalData(uint32 u_lutime, uint32 u_lctime, uint32 *ptotal_cal_num, uint32 *pcal_num, uint32 *ptotal_line_num, uint32 *pcur_line_num)
{
	if ( !pcal_num || !ptotal_line_num || !pcur_line_num )
		return -1;

	if ( 0 == u_lctime )
	{
		tcdbg_printf("\n[%s] error LogCountInterval=[0] !\n", __FUNCTION__);
		return -1;
	}
	*pcal_num = u_lctime / DEF_CAL_TIME;
	if ( *pcal_num < 1 )
	{
		tcdbg_printf("\n[%s] error LogCountInterval=[%u] !\n", __FUNCTION__, u_lctime);
		return -1;
	}
	*ptotal_cal_num = *pcal_num;
	*ptotal_line_num = u_lutime / u_lctime;
	if ( *ptotal_line_num < 1 )
	{
		tcdbg_printf("\n[%s] error data in LogUploadInterval=[%lu],LogCountInterval=[%lu] !\n", __FUNCTION__, u_lutime, u_lctime);
		return -1;
	}
	*pcur_line_num = *ptotal_line_num;

	return 0;
}

/* do upper case */
void upper(char *s)
{
	while (*s != '\0') {
		if (islower(*s))
			*s = toupper(*s);
		s++;
	}
}

/* generate LAN MAC table list */
int getLANDeviceMAC(char *macbuf_p, int maxlen)
{
#define PMIMFORM_BR_FDB_PATH "/proc/br_fdb_host/stb_list"
	FILE *fp_br_fdb = NULL;
	char buf[160]={0}, fdb_mac[20] = {0};
	int lanidx = 0, buf_idx = 0;
	char macbuf[4][160] = {0};
	int i = 0;

	if ( !macbuf )
		return -1;

	fp_br_fdb = fopen(PMIMFORM_BR_FDB_PATH, "r");
	if ( !fp_br_fdb )
		return -1;

	bzero(buf, sizeof(buf));
	while ( fgets(buf, sizeof(buf), fp_br_fdb)
		&& buf_idx <= maxlen )
	{
		bzero(fdb_mac, sizeof(fdb_mac));
		sscanf(buf, "%d=%s", &lanidx, fdb_mac);
		upper(fdb_mac);

		/* lan 1 */
		if (lanidx == 1) {
			if ( 0 == strlen(macbuf[0]) )
			{
				sprintf(macbuf[0], "LAN1:%s", fdb_mac);
			}
			else
			{
				sprintf(macbuf[0], "%s-%s", macbuf[0], fdb_mac);
			}
		}
		/* lan 2 */
		else if (lanidx == 2) {
			if ( 0 == strlen(macbuf[1]) )
		{
				sprintf(macbuf[1], "LAN2:%s", fdb_mac);
			}
			else
			{
				sprintf(macbuf[1], "%s-%s", macbuf[1], fdb_mac);
			}
		}
		/* lan 3 */
		else if (lanidx == 3) {
			if ( 0 == strlen(macbuf[2]) )
			{
				sprintf(macbuf[2], "LAN3:%s", fdb_mac);
			}
			else
			{
				sprintf(macbuf[2], "%s-%s", macbuf[2], fdb_mac);
			}
		}
		/* lan 4 */
		else if (lanidx == 4) {
			if ( 0 == strlen(macbuf[3]) )
			{
				sprintf(macbuf[3], "LAN4:%s", fdb_mac);
		}
		else
		{
				sprintf(macbuf[3], "%s-%s", macbuf[3], fdb_mac);
			}
		}
		}

	/* 4 ports */
	for (i = 0; i < 4; i++) {
		if (strlen(macbuf_p) == 0) {
			strcpy(macbuf_p, macbuf[i]);
		}
		else {
			if (strlen(macbuf[i]) > 0)
				sprintf(macbuf_p, "%s-%s", macbuf_p, macbuf[i]);
		}
	}

	fclose(fp_br_fdb);

	return 0;
}

/* collect cpe info and upload via tftp  */
int cwmpPMInformRoutine(void)
{
	static uint8_t is_start = 0;
	static char file_path[600] = {0}, file_name[512] = {0};
	static uint32_t cur_line_num = 0, total_line_num = 0, cal_num = 0, tatal_cal_num = 0;
	static time_t mainstart_ltime = {0}, start_ltime = {0}, end_ltime = {0};
	static uint32_t cpurate = 0, memrate = 0, ser = 0, errcode = 0, plr = 0, pktlost = 0;
	static int sw_cpurate = 0, sw_memrate = 0, sw_ser = 0, sw_errcode = 0, sw_plr = 0;
	static int sw_pktlost = 0, sw_temp = 0, sw_updata = 0, sw_dwdata = 0;
	static int sw_wlandevmac = 0, sw_landevmac = 0;
	static uint32 u_lutime = 0, u_lctime = 0;
	static double temp = 0.0, updata = 0.0, dwdata = 0.0;
	static struct frec_collection** pcurrCollection = &collectfrecHeader;
	struct tm* pstart_time = NULL;
	struct tm* pmainstart_time = NULL;
	struct tm* pend_time = NULL;
	time_t dbgtemp_ltime = {0};
	char c_start_time[24] = {0};
	char v_loid[128] = {0};
	char manufacturer[128] = {0}, modelname[128] = {0}, softv[128] = {0}, hdv[128] = {0};
	char mac[24] = {0}, cwmpip[20] = {0}, userid[256] = {0}, start_p[24] = {0}, end_p[24] = {0};
	char wlandevmac[512] = {0}, landevmac[512] = {0}, lanmacAttr[32] = {0};
	int i = 0;
	char tmp[1024] = {0}, tmpval[64] = {0};
	uint32_t u_valtmp = 0;
	FILE *fp = NULL;

	if ( 1 != isNTPSyncSucc() )
	{
		tcdbg_printf("\n[%s]ntpsync is not ok!\n", __FUNCTION__);
		return - 1;
	}

	if ( 0 == is_start )
	{
		initCollect(&u_lutime, &u_lctime, &mainstart_ltime, &start_ltime);
		if ( 0 != checkLocalData(u_lutime, u_lctime, &tatal_cal_num, &cal_num, &total_line_num, &cur_line_num) )
			return -1;

		initSWCollect(&sw_cpurate, &sw_memrate, &sw_ser, &sw_errcode, &sw_plr
					, &sw_pktlost, &sw_temp, &sw_updata, &sw_dwdata, &sw_wlandevmac, &sw_landevmac);
		/* clear data*/
		cpurate = memrate = ser = errcode = plr = pktlost =0;
		temp = updata = dwdata = 0.0;

		is_start = 1;

		time(&dbgtemp_ltime);
		tcdbg_printf("\n[%s]-[%u] statistics start !\n", __FUNCTION__, dbgtemp_ltime);
	}
	else
	{
		/* sum data */
		ZERO_X(tmpval);
		GetCPURate(tmpval);
		sscanf(tmpval, "%lu", &u_valtmp);
		cpurate += u_valtmp;

		ZERO_X(tmpval);
		GetMemRate(tmpval);
		sscanf(tmpval, "%lu", &u_valtmp);
		memrate += u_valtmp;

		ZERO_X(tmpval);
		GetSER(tmpval);
		sscanf(tmpval, "%lu", &u_valtmp);
		ser += u_valtmp;

		ZERO_X(tmpval);
		tcapi_get("Info_PonWanStats", "RxCrcCnt", tmpval);
		sscanf(tmpval, "%lu", &u_valtmp);
		errcode += u_valtmp;

		ZERO_X(tmpval);
		GetPLR(tmpval);
		sscanf(tmpval, "%lu", &u_valtmp);
		plr += u_valtmp;

		ZERO_X(tmpval);
		tcapi_get("Info_PonWanStats", "TxDropCnt", tmpval);
		sscanf(tmpval, "%lu", &u_valtmp);
		pktlost += u_valtmp;

		ZERO_X(tmpval);
		GetTEMP(tmpval);
		temp += atof(tmpval);

		ZERO_X(tmpval);
		tcapi_get("Info_PonWanStats", "TxFrameLen", tmpval);
		updata += (double)(atof(tmpval)/(1024*1024));

		ZERO_X(tmpval);
		tcapi_get("Info_PonWanStats", "RxFrameLen", tmpval);
		sscanf(tmpval, "%lu", &u_valtmp);
		dwdata += (double)(atof(tmpval)/(1024*1024));

		--cal_num;
		if ( cal_num <= 0 )
		{
			cpurate /= tatal_cal_num;
			memrate /= tatal_cal_num;
			ser /= tatal_cal_num;
			errcode /= tatal_cal_num;
			plr /= tatal_cal_num;
			pktlost /= tatal_cal_num;
			temp /= (double)tatal_cal_num;
			updata /= (double)tatal_cal_num;
			dwdata /= (double)tatal_cal_num;

			/* add record */
			pstart_time = localtime(&start_ltime);
			snprintf(start_p, sizeof(start_p) - 1, FRT_TIME
					, pstart_time->tm_year + 1900,  pstart_time->tm_mon + 1, pstart_time->tm_mday
					, pstart_time->tm_hour, pstart_time->tm_min, pstart_time->tm_sec);

			end_ltime = start_ltime + u_lctime; /* sometimes is wrong when use time(&end_ltime); */
			
			pend_time = localtime(&end_ltime);
			snprintf(end_p, sizeof(end_p) - 1, FRT_TIME
					, pend_time->tm_year + 1900,  pend_time->tm_mon + 1, pend_time->tm_mday
					, pend_time->tm_hour, pend_time->tm_min, pend_time->tm_sec);

			ZERO_X(wlandevmac);
			tcapi_get("LanHost","WlanDevMAC",wlandevmac);
			ZERO_X(landevmac);
#if 1
			getLANDeviceMAC(landevmac, sizeof(landevmac) - 1);
#else
			for(i=1; i<5; i++){
				ZERO_X(tmpval);
				sprintf(lanmacAttr, "Eth%dDevMAC", i);
				tcapi_get("LanHost",lanmacAttr,tmpval);
				if(strlen(tmpval)>0){
					if(strlen(landevmac)>0)
						strcat(landevmac,"-");
					sprintf(lanmacAttr, "LAN%d:", i);
					strcat(landevmac,lanmacAttr);
					strcat(landevmac,tmpval);
				}
			}
#endif

			/* CPURate(02)|MemRate(02)|SER(02)|ErrorCode|PLR(02)|PacketLost|TEMP(.1)|Startpoint|Endpoint|UpData(.03)|DownData(.03)|WLANDeviceMAC |LANDeviceMAC*/
			pcurrCollection = addCollectionFrec(pcurrCollection
							, sw_cpurate, cpurate
							, sw_memrate, memrate
							, sw_ser, ser
							, sw_errcode, errcode
							, sw_plr, plr
							, sw_pktlost, pktlost
							, sw_temp, temp
							, start_p
							, end_p
							, sw_updata, updata
							, sw_dwdata, dwdata
							, sw_wlandevmac, wlandevmac
							, sw_landevmac, landevmac);
			--cur_line_num;
			start_ltime += u_lctime;/* sometimes is wrong when use time(&start_ltime); */

			time(&dbgtemp_ltime);
			tcdbg_printf("\n[%s]-[%u] remain main-times=[%lu] !\n", __FUNCTION__, dbgtemp_ltime, cur_line_num);
		}

		if ( cal_num <= 0 )
		{
			/* clear data*/
			cpurate = memrate = ser = errcode = plr = pktlost =0;
			temp = updata = dwdata = 0.0;
			cal_num = tatal_cal_num;

			initSWCollect(&sw_cpurate, &sw_memrate, &sw_ser, &sw_errcode, &sw_plr
			, &sw_pktlost, &sw_temp, &sw_updata, &sw_dwdata, &sw_wlandevmac, &sw_landevmac);
		}

		/* collections end */
		if ( cur_line_num <=0 )
		{
			tcapi_get("EPON_LOIDAuth", "LOID0", v_loid);
			pmainstart_time = localtime(&mainstart_ltime);
			snprintf(file_name, sizeof(file_name) - 1, FRT_FILE_NAME
					, v_loid
					, pmainstart_time->tm_year + 1900,	pmainstart_time->tm_mon + 1, pmainstart_time->tm_mday
					, pmainstart_time->tm_hour, pmainstart_time->tm_min,  pmainstart_time->tm_sec
					, u_lctime);
			snprintf(file_path, sizeof(file_path) - 1, FRT_FILE_PATH, file_name);
			fp = fopen(file_path, "w");
			if ( !fp )
			{
				is_start = 0;
				freeCollectionFrec(&collectfrecHeader);
				pcurrCollection = &collectfrecHeader;
				tcdbg_printf("\n[%s] error open file=[s] !\n", __FUNCTION__, file_path);
				return -1;
			}

			/* wirte header */
			ZERO_X(tmp);
			/* LOID|Manufacturer|productClass|SoftwareVersion|HardwareVersion|MACAddress|IPAddress|UserID|RptDataNUM */
			tcapi_get("DeviceInfo_devParaStatic", "Manufacturer", manufacturer);
			tcapi_get("DeviceInfo_devParaStatic", "ModelName", modelname); /* productClass */
			tcapi_get("DeviceInfo_devParaStatic", "CustomerSWVersion", softv);
			tcapi_get("DeviceInfo_devParaStatic", "CustomerHWVersion", hdv);
			tcapi_get("Info_Ether", "mac", mac);
			tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_CWMPIP, cwmpip);
			tcapi_get("WanInfo_INTERNET", PMINFORM_ATTR_USERID, userid);
			snprintf(tmp, sizeof(tmp) - 1, FRT_HEADER
					, v_loid
					, manufacturer
					, modelname
					, softv
					, hdv
					, mac
					, cwmpip
					, userid
					, total_line_num);
			fputs(tmp, fp);

			/* write body */
			flushCollectionFrec2file(fp, &collectfrecHeader);
			pcurrCollection = &collectfrecHeader;

			/* all is done */
			fputs(NEWLINE, fp);
			fclose(fp);
			fp = NULL;

			/* upload file via tftp */
			doTftpUpload(file_name);
			unlink(file_path);

			/* next collection */
			initCollect(&u_lutime, &u_lctime, &mainstart_ltime, &start_ltime);
			if ( 0 != checkLocalData(u_lutime, u_lctime, &tatal_cal_num, &cal_num, &total_line_num, &cur_line_num) )
			{
				is_start = 0;
				return -1;
			}

			/* clear data*/
			cpurate = memrate = ser = errcode = plr = pktlost =0;
			temp = updata = dwdata = 0.0;

			initSWCollect(&sw_cpurate, &sw_memrate, &sw_ser, &sw_errcode, &sw_plr
			, &sw_pktlost, &sw_temp, &sw_updata, &sw_dwdata, &sw_wlandevmac, &sw_landevmac);
		}
	}

	return 0;
}


void *cwmpPMInformTask(void)
{
	while ( 1 )
	{
		/* If PMInform are not enable, break */
		if ( 1 != checkPMInformEnable() )
		{
			freeCollectionFrec(&collectfrecHeader);
			break;
		}

		/* Start CPE Collector Routine*/
		if ( 0 == cwmpPMInformRoutine() )
			sleep(DEF_CAL_TIME);
		else
			sleep(5);
	}
	
	/* Set PMInformThread varible to 0 means thread exit */
	tcdbg_printf("\n======>(%s), do thread exit\n", __FUNCTION__);
	PMInformThread = 0;
	return NULL;
}

#endif

/* Check PMInform is enable or not */
int checkPMInformEnable(void)
{
	char enableValue[8]= {0};

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_ENABLE, enableValue);
	if ( 0 == strcmp(enableValue, PMINFORM_VAL_YES) )
		return 1;

	return 0;
}

int cwmpPMInformTaskStart(void)
{
#if defined(TCSUPPORT_CT_PMINFORM)
	pthread_attr_t pminforhandle_attr;
	int ret = 0;

	/* create thread */
	ret = pthread_attr_init(&pminforhandle_attr);
	if ( 0 != ret  )
	{
		tcdbg_printf("\nThread(%s) attribute creation fail!\n", __FUNCTION__);
		return -1;
	}

	ret = pthread_attr_setdetachstate(&pminforhandle_attr, PTHREAD_CREATE_DETACHED);
	if(ret != 0)
	{
		tcdbg_printf("\nThread(%s):Set attribute fail!\n", __FUNCTION__);
		return -1;
	}

	ret = pthread_create(&PMInformThread, &pminforhandle_attr, (void *)cwmpPMInformTask, NULL);
	if ( 0 != ret )
	{
		tcdbg_printf("\nThread(%s):Create thread fail!\n", __FUNCTION__);
		return -1;
	}

	pthread_attr_destroy(&pminforhandle_attr);
#endif

	return 0;
}

void checkAndDoPMInformTask(void)
{
#if defined(TCSUPPORT_CT_PMINFORM)
	if ( 1 == g_PMInformEnable)
	{
		if ( 0 == PMInformThread ) /* if cpe collector task is not running */
		{
			tcdbg_printf("\r\ncreate thread to do cpe Collector!");
			cwmpPMInformTaskStart();
		}
	}
#endif
}

/*TCSUPPORT_PON_ROSTELECOM begin*/
pthread_t gWgetThread = 0; //debug for wget fw /* firmware download wget thread */
/*TCSUPPORT_PON_ROSTELECOM end*/

#if defined(TCSUPPORT_CT_TR143_WGET_DOWNLOAD)
pthread_t gTR143WgetThread = 0; /* tr143 download wget thread */
extern uint16 getEthernetPriority();
extern int8 setDldUldDiagStateType(char * value);
/*
    TR143 statistics info
*/
typedef struct _TR143DiagInfo_
{
	struct timeval sysTimeTCPOpenRequest;
	struct timeval sysTimeTCPOpenResponse;
	struct timeval sysTimeROM;
	struct timeval sysTimeBOM;
	struct timeval sysTimeEOM;
	uint32_t testBytesReceived;
	uint32_t BOMInOctet;
	uint32_t EOMInOctet;
	/*
	1 : Complete
	2 : Error_InitConnectionFailed
	3 : Error_NoResponse
	4 : Error_TransferFailed
	*/
	uint32_t diagtate;
} TR143DiagInfo;

/*
    TR143 error code.
*/
typedef enum {
	eState_Complete = 1,
	eState_Error_InitConnectionFailed = 2,
	eState_Error_NoResponse = 3,
	eState_Error_TransferFailed = 4,
} eTR143ErrorCode;

#define DldDiagnostics_DNSURL "/tmp/ippingdiag.conf"
#define TR143_DW_FILE "/tmp/tmp_tr143download"
#define TR143_DW_STATISTICS "/tmp/tmp_tr143dwstatis"
#define TR143_DW_PIDFILE "/tmp/tmp_tr143_wget.pid"

#if defined(TCSUPPORT_CT_PON_SC) || defined(TCSUPPORT_CT_PON_CY_JX) || defined(TCSUPPORT_CT_PON_GDV20)
get_old_tcp_windows(unsigned int *p1, unsigned int *p2, unsigned int *p3)
{
	int fd, ret = 0;
	char read_buf[128] = {0};
	fd = open("/proc/sys/net/ipv4/tcp_rmem",O_RDONLY|O_NONBLOCK);

	if (fd != -1) {
		ret = read(fd, read_buf, 127);
		if (ret > 0) {
#if defined(TCSUPPORT_CT_PON_GDV20)
			sscanf(read_buf, "%u %u %u", p1, p2, p3);
#else
			/* read ok */
			scanf("%u %u %u", p1, p2, p3);
#endif
		}
		close(fd);
	}

#if 1//
	tcdbg_printf("\r\nget_old_tcp_windows(): p1 = %u, p2 = %u, p3 = %u"
	, *p1, *p2, *p3);
#endif

	return ;
}
#endif
#if defined(TCSUPPORT_CT_PON_GDV20)
/*
* get old value from /proc/sys/net/core/rmem_max
*/
int get_old_rmem_max(unsigned int *p0)
{
	int fd, ret = 0;
	char read_buf[128] = {0};
	fd = open("/proc/sys/net/core/rmem_max", O_RDONLY|O_NONBLOCK);

	if ( fd != -1 )
	{
		ret = read(fd, read_buf, 127);
		if (ret > 0 )
		{
			/* read ok */
			sscanf(read_buf, "%u", p0);
		}
		close(fd);
	}

	tcdbg_printf("\nget_old_rmem_max(): p0 = %u\n", *p0);

	return 0;
}

/*
* get old value from/proc/irq/23/smp_affinity
*/
int get_old_smp_affinity(unsigned int *p0)
{
	int fd, ret = 0;
	char read_buf[128] = {0};
	fd = open("/proc/irq/23/smp_affinity", O_RDONLY|O_NONBLOCK);

	if ( fd != -1 )
	{
		ret = read(fd, read_buf, 127);
		if (ret > 0 )
		{
			/* read ok */
			sscanf(read_buf, "%x", p0);
		}
		close(fd);
	}

	tcdbg_printf("\nget_old_smp_affinity(): p0 = %x\n", *p0);

	return 0;
}

#endif

/* do http download via wget and get file information. */
void *cwmpPerformWgetDownloadDiagnostics(void)
{
	uint32_t theHostAddress = 0;
	uint16_t thePort = 0;
	int rtnCode = 0;
	char cmdbuf[512] = {0}, domain[128]	= {0};
	char *phoststart = NULL, *phostend = NULL;
	TR143DiagInfo gTR143DiagInfo;
	FILE *fp = NULL;
#if defined(TCSUPPORT_CT_PON_SC) || defined(TCSUPPORT_CT_PON_CY_JX) || defined(TCSUPPORT_CT_PON_GDV20)
	unsigned int p1 = 0, p2 = 0, p3 = 0;
#endif
#if defined(TCSUPPORT_CT_PON_GDV20)
	unsigned int p_rmx = 0, p_aff = 0;
	int ipVersion = 1, iface_idx = 0, pvc_idx = 0, entry_idx = 0;
	char dwIfaceName[64] = {0};
	char thePath[256] = {0}, theHost[256] = {0};
#endif

#if defined(TCSUPPORT_CT_PON_CY_JX)
	char wan_buf[32] = {0}, value_buf[32] = {0};
	uint32 value1 = 0, value2 = 0;
	char *ptmp = NULL;
#endif

	if ( dbg_cwmp >= DBG_CWMP_L2 )
	{
		tc_printf("\r\n => %s() \r\n", __FUNCTION__);
		tc_printf("\r\n rpc...URL = %s\n", rpc.loadDiagInfo.transferURL);
	}


	/* add check if interface and url is configured */
	if ( 0 == rpc.loadDiagInfo.transferURL[0]
		|| 0 == rpc.loadDiagInfo.interfaceName[0] )
	{
		tcdbg_printf("==>%s(): interface and url should be configured.\n",
				__FUNCTION__);
		return NULL;
	}

	/* put domain to configfile, let it go through tr69 tunnel */
	phoststart = strstr(rpc.loadDiagInfo.transferURL, "://");
	if ( !phoststart )
	{
		tcdbg_printf("==>%s(): http url is invalid @start.\n", __FUNCTION__);
		return NULL;
	}
	phoststart += 3; /* ignore the symbol '://' */
	phostend = strstr(phoststart, "/");
	if ( !phostend )
	{
		tcdbg_printf("==>%s(): http url is invalid @end.\n", __FUNCTION__);
		return NULL;
	}
	if ( (phostend <= phoststart) ||
		((phostend - phoststart) >= sizeof(domain)) )
	{
		tcdbg_printf("==>%s(): http url is invalid @calculate.\n", __FUNCTION__);
		return NULL;
	}
	memset(cmdbuf, 0, sizeof(cmdbuf));
	memset(domain, 0, sizeof(domain));
	strncpy(domain, phoststart, phostend - phoststart);
	snprintf(cmdbuf, sizeof(cmdbuf) - 1,
			 "%s %s\n", domain, rpc.loadDiagInfo.interfaceName);
	doValPut(DldDiagnostics_DNSURL, cmdbuf);
	if ( dbg_cwmp >= DBG_CWMP_L2 )
	{
		tc_printf("\r\ndomain=[%s]\n", domain);
		tc_printf("\r\ncmdbuf=[%s]\n", cmdbuf);
	}

	/* do http download now */
#if defined(TCSUPPORT_CT_PON_SC) || defined(TCSUPPORT_CT_PON_CY_JX) || defined(TCSUPPORT_CT_PON_GDV20)
	get_old_tcp_windows(&p1, &p2, &p3);
#if defined(TCSUPPORT_CT_PON_GDV20)
	strcpy(cmdbuf, "echo '51200 131072 204800' >  proc/sys/net/ipv4/tcp_rmem");
#else
	strcpy(cmdbuf, "echo 8760 256960 4088000 > /proc/sys/net/ipv4/tcp_rmem");
#endif
	system(cmdbuf);
#endif
#if defined(TCSUPPORT_CT_PON_GDV20)
	get_old_rmem_max(&p_rmx);
	system("echo 819200 > /proc/sys/net/core/rmem_max");
	get_old_smp_affinity(&p_aff);
	system("echo 8 > /proc/irq/23/smp_affinity");

	cwmp_checkWan_by_name(rpc.loadDiagInfo.interfaceName, &ipVersion);
	if ( 4 == ipVersion )
	{
		snprintf( dwIfaceName,
				sizeof(dwIfaceName) - 1,
				"ds.%s", rpc.loadDiagInfo.interfaceName);
	}

	if ( strstr(rpc.loadDiagInfo.interfaceName, "ppp") )
		sscanf(rpc.loadDiagInfo.interfaceName, "ppp%d", &iface_idx);	
	else
	{
			sscanf(rpc.loadDiagInfo.interfaceName, "nas%d_%d",
				&pvc_idx, &entry_idx); 
			iface_idx = pvc_idx * MAX_SMUX_NUM + entry_idx;
	}

	WcParseUrl(rpc.loadDiagInfo.transferURL, theHost
		, &theHostAddress, &thePort, thePath);

	if ( iface_idx >= 0 && iface_idx < MAX_WAN_IF_INDEX )
	{
		snprintf(cmdbuf, sizeof(cmdbuf) - 1,
			"echo '%u %u %u' > /proc/tc3162/tr143_test", 1, iface_idx, thePort);
		system(cmdbuf);
	}
#endif
	
#if defined(TCSUPPORT_CT_PON_CY_JX)
	strcpy(wan_buf, rpc.loadDiagInfo.interfaceName);
	if (strstr(wan_buf, "nas") && (ptmp = strstr(wan_buf, "_")))
			ptmp[0] = '.';					
	snprintf(cmdbuf, sizeof(cmdbuf) - 1, "Info_%s", wan_buf);
	tcapi_get(cmdbuf, "rxbytes", value_buf);
	value1 = atoi(value_buf);
#endif

	snprintf(cmdbuf, sizeof(cmdbuf) - 1, "taskset 0x7 /userfs/bin/wget --tr143-flag 1 "
					"--dns-timeout 5s "
					"--tries 1 "
					"--timeout 3s "
					"--output-document %s "
					"--tr143-pidfile %s "
					"--tr143-ethpriority %d "
					"--tr143-interface %s "
					"--tr143-logfile %s %s "
					, TR143_DW_FILE
					, TR143_DW_PIDFILE
					, getEthernetPriority()
#if defined(TCSUPPORT_CT_PON_GDV20)
			, (4 == ipVersion ? dwIfaceName : rpc.loadDiagInfo.interfaceName )
#else
					, rpc.loadDiagInfo.interfaceName
#endif
					, TR143_DW_STATISTICS
					, rpc.loadDiagInfo.transferURL);
	system(cmdbuf);
	if ( dbg_cwmp >= DBG_CWMP_L2 )
		tcdbg_printf("\r\cmdbuf=[%s]\n", cmdbuf);

#if defined(TCSUPPORT_CT_PON_CY_JX)
	strcpy(wan_buf, rpc.loadDiagInfo.interfaceName);
	if (strstr(wan_buf, "nas") && (ptmp = strstr(wan_buf, "_")))
			ptmp[0] = '.';					
	snprintf(cmdbuf, sizeof(cmdbuf) - 1, "Info_%s", wan_buf);
	tcapi_get(cmdbuf, "rxbytes", value_buf);
	value2 = atoi(value_buf);
#endif

	/* get statistic info */
	fp = fopen(TR143_DW_STATISTICS, "r");
	if ( fp )
	{
		fread(&gTR143DiagInfo, sizeof(gTR143DiagInfo), 1, fp);
		fclose(fp);

		rpc.loadDiagInfo.sysTimeTCPOpenRequest
					= gTR143DiagInfo.sysTimeTCPOpenRequest;
		rpc.loadDiagInfo.sysTimeTCPOpenResponse
					= gTR143DiagInfo.sysTimeTCPOpenResponse;
		rpc.loadDiagInfo.sysTimeROM = gTR143DiagInfo.sysTimeROM;
		rpc.loadDiagInfo.sysTimeBOM = gTR143DiagInfo.sysTimeBOM;
		rpc.loadDiagInfo.sysTimeEOM = gTR143DiagInfo.sysTimeEOM;
#if defined(TCSUPPORT_CT_PON_GDV20)
		/* workaroud for Guangdong V2.0 test. */
		if ( rpc.loadDiagInfo.sysTimeEOM.tv_sec 
			- rpc.loadDiagInfo.sysTimeROM.tv_sec >= 25 )
		{
			rpc.loadDiagInfo.sysTimeEOM.tv_sec -= 1;
		}
		else if ( rpc.loadDiagInfo.sysTimeEOM.tv_sec 
			- rpc.loadDiagInfo.sysTimeROM.tv_sec >= 10 )
		{
			if ( rpc.loadDiagInfo.sysTimeEOM.tv_usec >= 300*1000 )
				rpc.loadDiagInfo.sysTimeEOM.tv_usec -= 300*1000;
			else
			{
				rpc.loadDiagInfo.sysTimeEOM.tv_sec -= 1;
				rpc.loadDiagInfo.sysTimeEOM.tv_usec += 700*1000;
			}
		}
		else if ( rpc.loadDiagInfo.sysTimeEOM.tv_sec 
			- rpc.loadDiagInfo.sysTimeROM.tv_sec >= 8 )
		{
			if ( rpc.loadDiagInfo.sysTimeEOM.tv_usec >= 200*1000 )
				rpc.loadDiagInfo.sysTimeEOM.tv_usec -= 200*1000;
			else
			{
				rpc.loadDiagInfo.sysTimeEOM.tv_sec -= 1;
				rpc.loadDiagInfo.sysTimeEOM.tv_usec += 800*1000;
			}
		}
#endif
		rpc.loadDiagInfo.testBytesReceived = gTR143DiagInfo.testBytesReceived;
		rpc.loadDiagInfo.BOMInOctet = gTR143DiagInfo.BOMInOctet;
		rpc.loadDiagInfo.EOMInOctet = gTR143DiagInfo.EOMInOctet;

#if defined(TCSUPPORT_CT_PON_CY_JX)
		if (value2 - value1 > 0)
			rpc.loadDiagInfo.testBytesReceived = value2 - value1;
#endif

		if ( dbg_cwmp >= DBG_CWMP_L2 )
			tcdbg_printf("\n TR143DiagInfo.diagtate=[%d]\n", gTR143DiagInfo.diagtate);

		switch( gTR143DiagInfo.diagtate )
		{
			case eState_Error_InitConnectionFailed :
				setDldUldDiagStateType("Error_InitConnectionFailed");
				break;
			case eState_Error_NoResponse :
				setDldUldDiagStateType("Error_NoResponse");
				break;
			case eState_Error_TransferFailed :
				setDldUldDiagStateType("Error_TransferFailed");
				break;
			case eState_Complete :
			default :
				setDldUldDiagStateType("Completed");
				break;
		}
	}

	unlink(DldDiagnostics_DNSURL);
	unlink(TR143_DW_FILE);
	unlink(TR143_DW_STATISTICS);
	unlink(TR143_DW_PIDFILE);

#if defined(TCSUPPORT_CT_PON_SC) || defined(TCSUPPORT_CT_PON_CY_JX) || defined(TCSUPPORT_CT_PON_GDV20)
	if (p1 > 0 && p2 > 0 && p3 > 0) {
#if defined(TCSUPPORT_CT_PON_GDV20)
		snprintf(cmdbuf, sizeof(cmdbuf) - 1,
		"echo '%u %u %u' > /proc/sys/net/ipv4/tcp_rmem", p1, p2, p3);
#else
		sprintf(cmdbuf, "echo %u %u %u > /proc/sys/net/ipv4/tcp_rmem", p1, p2, p3);
#endif
	}
	else {
#if defined(TCSUPPORT_CT_PON_GDV20)
		strcpy(cmdbuf, "echo '4096 87380 238144' > /proc/sys/net/ipv4/tcp_rmem");
#else
		strcpy(cmdbuf, "echo 4096 87380 238144 > /proc/sys/net/ipv4/tcp_rmem");
#endif
	}
	system(cmdbuf);
#endif
#if defined(TCSUPPORT_CT_PON_GDV20)
	if ( p_rmx > 0 )
	{
		snprintf(cmdbuf, sizeof(cmdbuf) - 1, 
			"echo %u > /proc/sys/net/core/rmem_max", p_rmx);
	}
	else
	{
		strcpy(cmdbuf, "echo 135168 > /proc/sys/net/core/rmem_max");
	}
	system(cmdbuf);

	if ( p_aff > 0 )
	{
		snprintf(cmdbuf, sizeof(cmdbuf) - 1,
			"echo %x > /proc/irq/23/smp_affinity", p_aff);
	}
	else
	{
		strcpy(cmdbuf, "echo f > /proc/irq/23/smp_affinity");
	}
	system(cmdbuf);

	system("echo '0 0 0' > /proc/tc3162/tr143_test");
#endif

	/* send inform */
	setDiagnosticsCompleteFlag(1);
	cwmpSignalTimerInform(1);

	/* Set gTR143WgetThread varible to 0 means thread exit */
	tcdbg_printf("\n======>%s(), do thread exit\n", __FUNCTION__);
	gTR143WgetThread = 0;
	return NULL;
}

/* create thread for TR143 http download. */
int cwmpPerformWgetTaskStart(void)
{
	pthread_attr_t wgethandle_attr;
	int ret = 0;

	/* create thread */
	ret = pthread_attr_init(&wgethandle_attr);
	if ( 0 != ret )
	{
		tcdbg_printf("\nThread(%s) attribute creation fail!\n", __FUNCTION__);
		return -1;
	}

	ret = pthread_attr_setdetachstate(&wgethandle_attr,
									PTHREAD_CREATE_DETACHED);
	if ( ret != 0 )
	{
		tcdbg_printf("\nThread(%s):Set attribute fail!\n", __FUNCTION__);
		return -1;
	}

	ret = pthread_create(&gTR143WgetThread
						, &wgethandle_attr
						, (void *)cwmpPerformWgetDownloadDiagnostics
						, NULL);
	if ( 0 != ret )
	{
		tcdbg_printf("\nThread(%s):Create thread fail!\n", __FUNCTION__);
		return -1;
	}

	pthread_attr_destroy(&wgethandle_attr);

	return 0;
}
#endif


/* download task check: perform download diagnostics via wget. */
void cwmpPerformWgetDownloadTask(void)
{
#if defined(TCSUPPORT_CT_TR143_WGET_DOWNLOAD)
	FILE *fp = NULL;
	char pidbuf[12] = {0}, cmdbuf[512] = {0};
	int pid = 0;

	if ( rpc.flags.theDownloadDiagFlag & CWMP_DIAGNOSTICS_WGET_DOWNLOAD_ACTIVE )
	{
		if ( 0 != gTR143WgetThread )
		{
			/* download isn't done. */
			fp = fopen(TR143_DW_PIDFILE, "r");
			if ( fp )
			{
				fgets(pidbuf, sizeof(pidbuf), fp);
				fclose(fp);
				fp = NULL;
				sscanf(pidbuf, "%d\n", &pid);
				if( 0 != pid )
				{
					sprintf(cmdbuf,"kill -9 %d &", pid);
					system(cmdbuf);
					sleep(1);
				}
				/* wait 1s again if thread isn't exit. */
				if ( 0 != gTR143WgetThread )
					sleep(1);
			}
		}

		cwmpPerformWgetTaskStart();
		rpc.flags.theDownloadDiagFlag &= ~CWMP_DIAGNOSTICS_WGET_DOWNLOAD_ACTIVE;
	}
#endif
	return;
}

/* upload task check: perform upload diagnostics via curl. */
void cwmpPerformCurlUploadTask(void)
{
	return;
}

/* TCSUPPORT_PON_ROSTELECOM begin*/
/* do http download via wget and get file information. */
void *cwmpPerformFirmwareWgetDownload(void)
{
	char cmdbuf[512] = {'\0'}, url[512]	= {'\0'};
	char dwIfaceName[64] = {'\0'};
	char username[256] = {'\0'};
	char password[256] = {'\0'};
	char *dbgtmp = NULL ;

    if(-1 == cwmpGetWgetFileTransferArgument(url))
    {
        return NULL;
    }
    
    tcapi_get("CwmpRoute_Entry", "ifName0", dwIfaceName);
	strcpy( username , rpc.fileTransferStruct->Username );
	strcpy( password , rpc.fileTransferStruct->Password );
    
	/* do http download now , only support http download*/
    //tcdbg_printf("cwmpPerformFirmwareWgetDownload: wget starts.....\n");
	snprintf(cmdbuf, sizeof(cmdbuf) - 1, "taskset 0x7 /userfs/bin/wget "
					"--dns-timeout 5s "
					"--tr069-flag 1 "
					"--http-user %s "
					"--http-password %s "
					"--output-document %s "
					"--tr069-interface %s "
					"\"%s\" "
					,username
					,password
					,"/var/tmp/tclinux.bin"
					,dwIfaceName
                    ,url );
	//show cmd one by one
	/*dbgtmp=cmdbuf;
	tcdbg_printf("cmd = ");
	while(*dbgtmp != '\0')
	{
		 tcdbg_printf("%c" , *dbgtmp);
		 dbgtmp++ ;
	}
	tcdbg_printf("\n");*/

	system(cmdbuf);

	/*no error handle, need to add in future*/
    //tcdbg_printf("\r\n Firmware Download OK, We will check it and Update it ------------------------------\n");
    /*we may move set_file_transfer_flag in if case below to avoid send inform while write firmware to flash*/
    /*If FirmWareUpdate return 0, we will Update firmware and restart CPE*/ 
        
    SetFileTransferCompleteFlag(EVENT_TransferComplete|TransferComplete_Error9010);
    SaveCwmpFlags();
	
    if(FirmWareUpdate() == -1)
    {
        strcpy(rpc.fileTransferStruct->faultStructPtr->faultString, "Firmware File is illegal for CPE!");
        rpc.fileTransferStruct->faultStructPtr->faultCode= DownloadFailure;
        SetFileTransferCompleteFlag(EVENT_TransferComplete|TransferComplete_Error9010);
        set_file_transfer_flag(0);
        SaveCwmpFlags();
        cwmpSignalTimerInform(1);
    }
    else
    {
        /*We set TransferCompleteFlag before Firmware update*/
        /*Set Flag prevent inform send before restart*/
        set_firmwareupgrade_flag();
        //SetFileTransferCompleteFlag(EVENT_TransferComplete);
        //SaveCwmpFlags();                              
    }
    
	/* Set gWgetThread varible to 0 means thread exit */
	//tcdbg_printf("\n======>%s(), do thread exit\n", __FUNCTION__);
	gWgetThread = 0;
	return NULL;
}


/* create thread for firmware http download. */
int cwmpPerformFirmwareWgetTaskStart(void)
{
	pthread_attr_t wgethandle_attr;
	int ret = 0;

	/* create thread */
	ret = pthread_attr_init(&wgethandle_attr);
	if ( 0 != ret )
	{
		tcdbg_printf("\nThread(%s) attribute creation fail!\n", __FUNCTION__);
		return -1;
	}

	ret = pthread_attr_setdetachstate(&wgethandle_attr,
									PTHREAD_CREATE_DETACHED);
	if ( ret != 0 )
	{
		tcdbg_printf("\nThread(%s):Set attribute fail!\n", __FUNCTION__);
		return -1;
	}

	ret = pthread_create(&gWgetThread
						, &wgethandle_attr
						, (void *)cwmpPerformFirmwareWgetDownload
						, NULL);
	if ( 0 != ret )
	{
		tcdbg_printf("\nThread(%s):Create thread fail!\n", __FUNCTION__);
		return -1;
	}

	pthread_attr_destroy(&wgethandle_attr);

	return 0;
}

/* download task check: perform firmware download diagnostics via wget. */
void cwmpPerformWgetFirmwareDownloadTask(void)
{
    cwmpPerformFirmwareWgetTaskStart();
    return;
}
/* TCSUPPORT_PON_ROSTELECOM end*/

static int xbind1_notify = 0;
static int xbind1_wan_node_idx = 0, xbind1_wan_entry_idx = 0;
static char ppp_username[256] = {0};

int getXBIND1NotifyFlag(void)
{
	return xbind1_notify;
}

int setXBIND1NotifyFlag(int flag)
{
#if defined(TCSUPPORT_CT_ADSL_BIND1)
	char brname[32] = {0};

	xbind1_notify = flag;
	if ( xbind1_notify )
	{
		memset(ppp_username, 0, sizeof(ppp_username));
		/* store the username and check wan node index. */
		tcapi_get("WebCurSet_Entry", "AutoBINDUserName", ppp_username);

		tcapi_get("WebCurSet_Entry", "AutoBINDBrName", brname);
		if ( 'n' == brname[0] )
			sscanf(brname, "nas%d_%d",
			&xbind1_wan_node_idx, &xbind1_wan_entry_idx);
	}
#endif
	return 0;	
}

int8 cwmpBuildXBIND1Parameters(void)
{	
#if defined(TCSUPPORT_CT_ADSL_BIND1)
#define FRT_PPP_NODE_USRNAME "InternetGatewayDevice.WANDevice" \
							 ".1.WANConnectionDevice.%d." \
							 "WANPPPConnection.%d.Username"
#define FRT_PVC_NODE_DEST "InternetGatewayDevice.WANDevice" \
						  ".1.WANDSLConnectionManagement." \
						  "ConnectionService.%d.DestinationAddress"
	char connection_name[256] = {0};		
	char nodeName[64] = {0}, result[64] = {0};
	char vpi[10] = {0}, vci[10] = {0};
	int8 state = 0;

	/* get pvc vpi/vci */
	sprintf(nodeName, "Wan_PVC%d", xbind1_wan_node_idx);
	tcapi_get(nodeName, "VPI", vpi);
	tcapi_get(nodeName, "VCI", vci);
	sprintf(result,"PVC:%s/%s",vpi,vci);

	/* format node name and add it to paramter list */
	snprintf(connection_name, sizeof(connection_name) - 1,
			FRT_PVC_NODE_DEST, xbind1_wan_node_idx + 1);
	state = cwmpAddInformParameter(connection_name, result, StringType);
	if ( 0 != state )
		 return -1;

	/* format node name and add it to paramter list */
	snprintf(connection_name, sizeof(connection_name) - 1,
			FRT_PPP_NODE_USRNAME, xbind1_wan_node_idx + 1,
			xbind1_wan_entry_idx + 1);
	state = cwmpAddInformParameter(connection_name, ppp_username, StringType);
	if ( 0 != state )
		 return -1;

#endif
	return 0;
}

#ifdef TCSUPPORT_CWMP_FAST_GET
extern int tcapi_get_fast(char* node, char* attr, char *buf);
/*when use TCSUPPORT_CWMP_FAST_GET,use tcapi_get_fast in cwmp.c replace tcapi_get in tcapi_lib*/
int
tcapi_get(char* node, char* attr, char *buf){
	return tcapi_get_fast(node, attr, buf);
}
#endif

#ifndef CLMP
void initUpnpGlobalValue()
{
}
#endif
#if defined(TCSUPPORT_CT_JOYME)
extern cfgNode_ptr pTcCfgNodes;
extern DeviceNodeStackPtr deviceNodeStackHead;
extern int cwmpSearchDeviceNodeByName
(char* requestParameterName, DeviceNodePtr deviceRoot
, DeviceNodePtr* curDeviceNode);
extern int cwmpSetDeviceNodeAttribute
(ParameterAttributesSetStructPtr reqParameter, DeviceNodePtr deviceNode);
int cwmpPhoneAddObjectResponse
(char *nodename, char *nodevalue, int client);
int cwmpPhoneDeleteObjectResponse
(char *nodename, char *nodevalue, int client);
int cwmpPhoneGetParameterValuesResponse
(char *nodename, char *nodevalue, int client);
int cwmpPhoneSetParameterValuesResponse
(char *nodename, char *nodevalue, int client);

enum OP_CMD
{
	RPC_CMD_REQ = 1,
	RPC_CMD_REPLY,
	RPC_CMD_REPLY_END,
	RPC_CMD_REPLY_ERROR
};

typedef struct _tcphone_rpc_msg_
{
#define RPC_MSG_MAGIC 0x11221122
#define RPC_MAX_NODE_BUFLEN 512
#define ZERO_RPC_MSG(x) { bzero(&x, sizeof(x)); x.magic = RPC_MSG_MAGIC; }
	uint32_t magic;
	uint8_t opcmd;
	uint8_t reserve[3];
	uint32_t valuelen;
	char rpcmethod[64];
	char nodename[RPC_MAX_NODE_BUFLEN];
} tcphone_rpc_msg;

typedef int (*PhoneRPCMethodFun)(char *, char *, int);
typedef struct{
   char* name;
   PhoneRPCMethodFun  phoneRPC_Method;
} cwmp_phone_rpc_methodmap;

#define OSFREE_X(x) { if (x) os_free(x); x = NULL; }


/*
phone RPC method table
*/
static cwmp_rpc_methodmap rpc_PhoneMethodMapTable[] = 
{
	{"ADDOBJECT", cwmpPhoneAddObjectResponse},
	{"DELETEOBJECT", cwmpPhoneDeleteObjectResponse},
	{"GETPARAMETERVALUES", cwmpPhoneGetParameterValuesResponse},
	{"SETPARAMETERVALUES", cwmpPhoneSetParameterValuesResponse},
    {NULL, NULL}
};

/* Get Phone RPC response function. */
int cwmpPhoneGetRPCResponseFun(char* rpcName
							, PhoneRPCMethodFun* RPCMethodPtr)
{
	cwmp_phone_rpc_methodmap* tempRpcM =  &rpc_PhoneMethodMapTable[0];

	if ( !rpcName )
		return -1;

	while ( tempRpcM->name )
	{
		if ( 0 == strcmp(rpcName, tempRpcM->name)
			&& tempRpcM->phoneRPC_Method )
		{
			*RPCMethodPtr = tempRpcM->phoneRPC_Method;
			return 0;
		}
		tempRpcM++;
	}

	return -1;
}

/*
	send error msg to phone
*/
int responseErrorMsg(int client, char *errMSG)
{
	tcphone_rpc_msg msg;
	int msgLen = 0;
	if ( !errMSG )
		return -1;

	msgLen = strlen(errMSG);
	ZERO_RPC_MSG(msg);
	msg.opcmd = RPC_CMD_REPLY_ERROR;
	snprintf(msg.nodename, sizeof(msg.nodename) - 1,
		"%s", errMSG);
	/* use MSG_NOSIGNAL to prevent broken-pipe errer when cpe need restart boa.
	the reason us the same with recv function. */
	if ( send(client, &msg, sizeof(msg), MSG_NOSIGNAL) < 0 )
	{
		tcdbg_printf("\n==>(%s), send msg fail \n", __FUNCTION__);
		return -1;
	}

	return 0;
}

/*
	send end msg to phone
*/
int responseEndMsg(int client)
{
	tcphone_rpc_msg msg;

	ZERO_RPC_MSG(msg);
	msg.opcmd = RPC_CMD_REPLY_END;
	/* use MSG_NOSIGNAL to prevent broken-pipe errer when cpe need restart boa.
	the reason us the same with recv function. */
	if ( send(client, &msg, sizeof(msg), MSG_NOSIGNAL) < 0 )
	{
		tcdbg_printf("\n==>(%s), send msg fail \n", __FUNCTION__);
		return -1;
	}

	return 0;
}

/*
	send cwmp node msg to phone
*/
int responseCwmpNodeMsg
(int client, char *nodename, char *nodevalue)
{
	tcphone_rpc_msg msg;
	int valueLen = 0;

	if ( !nodename || !nodevalue )
		return -1;

	valueLen = strlen(nodevalue);
	ZERO_RPC_MSG(msg);
	msg.opcmd = RPC_CMD_REPLY;
	msg.valuelen = valueLen;
	snprintf(msg.nodename, sizeof(msg.nodename) - 1,
		"%s", nodename);
	/* use MSG_NOSIGNAL to prevent broken-pipe errer when cpe need restart boa.
	the reason us the same with recv function. */
	if ( send(client, &msg, sizeof(msg), MSG_NOSIGNAL) < 0 )
	{
		tcdbg_printf("\n==>(%s), send msg fail \n", __FUNCTION__);
		return -1;
	}
	if ( send(client, nodevalue, valueLen, MSG_NOSIGNAL) < 0 )
	{
		tcdbg_printf("\n==>(%s), send msg value fail \n", __FUNCTION__);
		return -1;
	}

	return 0;
}

/*
	send end msg to phone
*/
int responseInstanceNumMsg(int client, uint32_t instance)
{
	tcphone_rpc_msg msg;

	ZERO_RPC_MSG(msg);
	msg.opcmd = RPC_CMD_REPLY_END;
	snprintf(msg.nodename, sizeof(msg.nodename) - 1,
			"%lu", instance);
	/* use MSG_NOSIGNAL to prevent broken-pipe errer when cpe need restart boa.
	the reason us the same with recv function. */
	if ( send(client, &msg, sizeof(msg), MSG_NOSIGNAL) < 0 )
	{
		tcdbg_printf("\n==>(%s), send msg fail \n", __FUNCTION__);
		return -1;
	}

	return 0;
}

/*
cwmpPhonePushDeviceStack
*/
int cwmpPhonePushDeviceStack(DeviceNodePtr deviceNode)
{
	DeviceNodeStackPtr newStackNode =NULL;

	if(deviceNode == NULL)
		return -1;

	if((newStackNode = (DeviceNodeStackPtr)os_malloc(sizeof(DeviceNodeStack)))
		== NULL)
	{
		return ResourceExceed;
	}

	memset(newStackNode,0, sizeof(DeviceNodeStack));
	newStackNode->deviceNode = deviceNode;

	newStackNode->next = deviceNodeStackHead;
	deviceNodeStackHead = newStackNode;
	return 0;
}
/*
cwmpPhonePopDeviceStack
*/
int cwmpPhonePopDeviceStack(DeviceNodePtr* deviceNode)
{
	DeviceNodeStackPtr tempStackNode = deviceNodeStackHead;
	
	if(deviceNodeStackHead == NULL)
		return -1;

	*deviceNode = deviceNodeStackHead->deviceNode;
	
	deviceNodeStackHead = deviceNodeStackHead->next;
	tempStackNode ->next =NULL;

	if(tempStackNode != NULL)
	{
	      os_free(tempStackNode);
	      tempStackNode = NULL;
	}
 
	return 0;
}

/*
reset attribute for new node
*/
int cwmpPhoneSetDeviceNodeAttribute
(ParameterAttributesSetStructPtr reqParameter, DeviceNodePtr deviceNode)
{
	int rtnStatus = CWMP_E_SUCCESS;
	int rtncode = 0;
	char faultString[FaultStringLength];

	if(reqParameter == NULL)
		return -1;

	while(deviceNode != NULL)
	{
		while(deviceNode != NULL)
		{
			if(deviceNode->type != CWMP_OBJECT)
			{
				if ( reqParameter->notification > 2 ||
					(reqParameter->notificationChange != 0
					&& reqParameter->notificationChange != 1))
				{
					/* The setting Attribute for %s is rejected!*/
					rtnStatus = InvalidParameterValue;
					return rtnStatus;
				}
				
				if((deviceNode->flag == 0 && reqParameter->notification == 2))
				{
					/* The setting Attribute for %s is rejected! */
					rtnStatus = RejectRequestNotification;
					return rtnStatus;
				}

				if ( deviceNode->setAttribute )
				{
					/*multipulate notify node list with new attributes.*/
					if ( reqParameter->notificationChange )
						cwmpMultipulateNotifyListByNewAttr(deviceNode
						, reqParameter->notification);
						
					rtnStatus = deviceNode->setAttribute(
						reqParameter->notificationChange
						, reqParameter->notification
						, reqParameter->accessListChange
						, reqParameter->accessList
						, deviceNode);

				}
			}

			cwmpPhonePushDeviceStack(deviceNode);
			deviceNode = deviceNode->firstChild;
			
		}

		while(deviceNode == NULL && deviceNodeStackHead != NULL)
		{
			cwmpPhonePopDeviceStack(&deviceNode);

			if(deviceNodeStackHead != NULL)
				deviceNode = deviceNode->nextSibling;
			else
				deviceNode = NULL;
		}
	}
	
	return 0;
}

/*
reset attribute for new node
*/
int cwmpPhoneResetAttribute(char *deviceName)
{
	DeviceNodePtr theDeviceNode = NULL;
	ParameterAttributesSetStruct attribute;

	cwmpSearchDeviceNodeByName(deviceName
		, rpc.IGDDeviceRootPtr, &theDeviceNode);

	if ( theDeviceNode )
	{
		attribute.name = deviceName;	
		attribute.notificationChange = 1;
		attribute.notification = 0;
		attribute.accessListChange = 0;
		cwmpPhoneSetDeviceNodeAttribute(&attribute, theDeviceNode);
	}

	return 0;
	
}

/*
RPC for GETPARAMETERVALUES
*/
int cwmpPhoneAddObjectResponse
(char *nodename, char *nodevalue, int client)
{
	DeviceNodePtr currentDeviceNode = NULL;
	char faultString[1024] = {0};
	ParameterType paraType = {0};
	uint32_t instanceNumber = 0;
	char tempName[256] = {0}, instanceStr[20] = {0};
	int result = 0;

	if ( !nodename )
		return -1;

	pthread_mutex_lock(&value_change_lock);

	/* Search node and get value */
	currentDeviceNode = NULL;
	cwmpSearchDeviceNodeByName(nodename
							, rpc.IGDDeviceRootPtr
							, &currentDeviceNode);

	if ( !currentDeviceNode
		|| NULL == currentDeviceNode->addObject
		|| CWMP_OBJECT != currentDeviceNode->type )
	{
		snprintf(faultString, sizeof(faultString) - 1
			, "Invalid parameter name:%s", nodename);
		responseErrorMsg(client, faultString);				
		goto ERROR_STATE;
	}

	if ( ReadOnly == currentDeviceNode->accessType )
	{
		snprintf(faultString, sizeof(faultString) - 1
			, "Attemp to set a non-writable parameter:%s", nodename);
		responseErrorMsg(client, faultString);
		goto ERROR_STATE;
	}

	/* add new object */
	cwmpGetDeviceNodeName(currentDeviceNode, tempName);
	cwmpSetSpSaveFlag(0);
	result = currentDeviceNode->addObject(&instanceNumber, currentDeviceNode);
	cwmpSetSpSaveFlag(1);
	if ( result < 0 )
	{
		snprintf(faultString, sizeof(faultString) - 1
			, "Can not find usued instanceNumber for:%s", nodename);
		responseErrorMsg(client, faultString);
		goto ERROR_STATE;
	}

	/* clear attributes for new object */
	sprintf(instanceStr, ".%d", instanceNumber);
	strcat(tempName, instanceStr);
	cwmpPhoneResetAttribute(tempName); /* there is value_change_lock in it */

	if ( 0 != responseInstanceNumMsg(client, instanceNumber) )
	{
		strcpy(faultString, "Internal response error(1)");
		responseErrorMsg(client, faultString);				
		goto ERROR_STATE;
	}
	pthread_mutex_unlock(&value_change_lock);

	HandleParametersSetting( pTcCfgNodes, COMMIT, NODELAY );
	HandleParametersSetting( pTcCfgNodes, COMMIT, DELAY );

	return 0;

ERROR_STATE:
	pthread_mutex_unlock(&value_change_lock);

	return -1;
}

/*
RPC for DELETEOBJECT
*/
int cwmpPhoneDeleteObjectResponse
(char *nodename, char *nodevalue, int client)
{
	DeviceNodePtr currentDeviceNode = NULL;
	char faultString[1024] = {0};
	ParameterType paraType = {0};
	int result = 0, number = 0;

	if ( !nodename )
		return -1;

	pthread_mutex_lock(&value_change_lock);

	/* Search node and get value */
	currentDeviceNode = NULL;
	cwmpSearchDeviceNodeByName(nodename
							, rpc.IGDDeviceRootPtr
							, &currentDeviceNode);

	if ( !currentDeviceNode
		|| NULL == currentDeviceNode->deleteObject
		|| CWMP_OBJECT != currentDeviceNode->type )
	{
		snprintf(faultString, sizeof(faultString) - 1
			, "Invalid parameter name:%s", nodename);
		responseErrorMsg(client, faultString);				
		goto ERROR_STATE;
	}

	if ( ReadOnly == currentDeviceNode->accessType )
	{
		snprintf(faultString, sizeof(faultString) - 1
			, "Attemp to set a non-writable parameter:%s", nodename);
		responseErrorMsg(client, faultString);
		goto ERROR_STATE;
	}

	/* delete the object */
	number = atoi(currentDeviceNode->name);
	if ( number < 0 )
	{
		snprintf(faultString, sizeof(faultString) - 1
			, "Invalid parameter name:%s", nodename);
		responseErrorMsg(client, faultString);
		goto ERROR_STATE;
	}

	cwmpSetSpSaveFlag(0);
	result = currentDeviceNode->deleteObject((uint32)number, currentDeviceNode);
	cwmpSetSpSaveFlag(1);
	if ( result < 0 )
	{
		snprintf(faultString, sizeof(faultString) - 1
			, "Can not find instance number for:%s", nodename);
		responseErrorMsg(client, faultString);
		goto ERROR_STATE;
	}

	if ( 0 != responseEndMsg(client) )
	{
		strcpy(faultString, "Internal response error(2)");
		responseErrorMsg(client, faultString);				
		goto ERROR_STATE;
	}

	pthread_mutex_unlock(&value_change_lock);

	HandleParametersSetting( pTcCfgNodes, COMMIT, NODELAY );
	HandleParametersSetting( pTcCfgNodes, COMMIT, DELAY );

	return 0;

ERROR_STATE:
	pthread_mutex_unlock(&value_change_lock);

	return -1;
}

/*
RPC for object node get
*/
int cwmpPhoneLoopforGetParameter
(DeviceNodePtr node, int client)
{
	DeviceNodePtr currentDeviceNode = NULL;
	char tempName[256] = {0};
	char faultString[1024] = {0};
	char* curParaValue = NULL;
	ParameterType paraType = {0};

	if ( !node )
		return -1;

	currentDeviceNode = node->firstChild;
	while ( currentDeviceNode )
	{
		if ( ObjectType == currentDeviceNode->type )
			cwmpPhoneLoopforGetParameter(currentDeviceNode, client);
		else
		{

		//tcdbg_printf("\ndbg type=[%d], name=[%s]\n", currentDeviceNode->type
		//, tempName);
		
			/* get value */
			if ( currentDeviceNode->getValueInfo )
				currentDeviceNode->getValueInfo(&curParaValue
						, &paraType, currentDeviceNode);
			/* get whole node name */
			cwmpGetDeviceNodeName(currentDeviceNode, tempName);
			if ( !curParaValue )
			{
				snprintf(faultString, sizeof(faultString) - 1
					, "Invalid parameter value:%s", tempName);
				responseErrorMsg(client, faultString);
				return -1;
			}

			if ( 0 != responseCwmpNodeMsg(client, tempName, curParaValue) )
			{
				strcpy(faultString, "Internal response loop error)");
				responseErrorMsg(client, faultString);				
				goto ERROR_STATE;
			}
			OSFREE_X(curParaValue);
		}
		/* next node */
		currentDeviceNode = currentDeviceNode->nextSibling;
	}

	OSFREE_X(curParaValue);

	return 0;

ERROR_STATE:
	OSFREE_X(curParaValue);

	return -1;

}

/*
RPC for GETPARAMETERVALUES
*/
int cwmpPhoneGetParameterValuesResponse
(char *nodename, char *nodevalue, int client)
{
	DeviceNodePtr currentDeviceNode = NULL;
	char faultString[1024] = {0};
	char* curParaValue = NULL;
	ParameterType paraType = {0};

	if ( !nodename )
		return -1;

	/* Search node and get value */
	currentDeviceNode = NULL;
	cwmpSearchDeviceNodeByName(nodename
							, rpc.IGDDeviceRootPtr
							, &currentDeviceNode);

	if ( !currentDeviceNode )
	{
		snprintf(faultString, sizeof(faultString) - 1
			, "Invalid parameter name:%s", nodename);
		responseErrorMsg(client, faultString);				
		return -1;
	}

	if ( ObjectType == currentDeviceNode->type )
	{
		if ( 0 != cwmpPhoneLoopforGetParameter(currentDeviceNode, client) )
			return -1;
	}
	else
	{
		if ( currentDeviceNode->getValueInfo )
			currentDeviceNode->getValueInfo(&curParaValue
					, &paraType, currentDeviceNode);

		if ( !curParaValue )
		{
			snprintf(faultString, sizeof(faultString) - 1
				, "Invalid parameter value:%s", nodename);
			responseErrorMsg(client, faultString);
			return -1;
		}

		if ( 0 != responseCwmpNodeMsg(client, nodename, curParaValue) )
		{
			strcpy(faultString, "Internal response error(3)");
			responseErrorMsg(client, faultString);				
			goto ERROR_STATE;
		}
	}

	if ( 0 != responseEndMsg(client) )
	{
		strcpy(faultString, "Internal response error(4)");
		responseErrorMsg(client, faultString);				
		goto ERROR_STATE;
	}

	OSFREE_X(curParaValue);

	return 0;

ERROR_STATE:
	OSFREE_X(curParaValue);

	return -1;
}

/*
RPC for SETPARAMETERVALUES
*/
int cwmpPhoneSetParameterValuesResponse
(char *nodename, char *nodevalue, int client)
{
	DeviceNodePtr currentDeviceNode = NULL;
	char faultString[1024] = {0};
	ParameterType paraType = {0};
	int32_t ival = 0;
	uint32_t uval = 0;
	int8_t bval = 0;

	if ( !nodename || !nodevalue )
		return -1;

	pthread_mutex_lock(&value_change_lock);
	/* Search node and get value */
	currentDeviceNode = NULL;
	cwmpSearchDeviceNodeByName(nodename
							, rpc.IGDDeviceRootPtr
							, &currentDeviceNode);

	if ( !currentDeviceNode )
	{
		snprintf(faultString, sizeof(faultString) - 1
			, "Invalid parameter name:%s", nodename);
		responseErrorMsg(client, faultString);				
		goto ERROR_STATE;
	}

	if ( currentDeviceNode->setValueInfo )
	{
		switch ( currentDeviceNode->type )
		{
		case Base64Type:
		case StringType:
		case String256Type:
		case DateTimeType:
			if ( CWMP_E_SUCCESS
				!= currentDeviceNode->setValueInfo(nodevalue, currentDeviceNode))
			{
				snprintf(faultString, sizeof(faultString) - 1
					, "Invalid parameter value:%s", nodevalue);
				responseErrorMsg(client, faultString);
				goto ERROR_STATE;
			}
			break;
		case IntType:
			ival = 0;
			if ( -1 == cwmpIsNumeric(nodevalue) )
			{
				snprintf(faultString, sizeof(faultString) - 1
					, "Invalid parameter value:%s", nodevalue);
				responseErrorMsg(client, faultString);
				goto ERROR_STATE;
			}
			sscanf(nodevalue, "%d", &ival);
			if ( CWMP_E_SUCCESS
				!= currentDeviceNode->setValueInfo(&ival, currentDeviceNode))
			{
				snprintf(faultString, sizeof(faultString) - 1
					, "Invalid parameter value:%s", nodevalue);
				responseErrorMsg(client, faultString);
				goto ERROR_STATE;
			}
			break;
		case UnsignedIntType:
			uval = 0;
			if ( -1 == cwmpIsNumeric(nodevalue) )
			{
				snprintf(faultString, sizeof(faultString) - 1
					, "Invalid parameter value:%s", nodevalue);
				responseErrorMsg(client, faultString);
				goto ERROR_STATE;
			}
			sscanf(nodevalue, "%lu", &uval);
			if ( CWMP_E_SUCCESS
				!= currentDeviceNode->setValueInfo(&uval, currentDeviceNode))
			{
				snprintf(faultString, sizeof(faultString) - 1
					, "Invalid parameter value:%s", nodevalue);
				responseErrorMsg(client, faultString);
				goto ERROR_STATE;
			}
			break;
		case BooleanType:
			bval = cwmpGetBooleanValue(nodevalue);
			if ( CWMP_E_SUCCESS
				!= currentDeviceNode->setValueInfo(&bval, currentDeviceNode))
			{
				snprintf(faultString, sizeof(faultString) - 1
					, "Invalid parameter value:%s", nodevalue);
				responseErrorMsg(client, faultString);
				goto ERROR_STATE;
			}
			break;

		default:
			snprintf(faultString, sizeof(faultString) - 1
				, "Do not support the value type: %d", 
				currentDeviceNode->type);
			responseErrorMsg(client, faultString);
			goto ERROR_STATE;
		}
	}
	else
	{
		snprintf(faultString, sizeof(faultString) - 1
			, "Attemp to set a non-writable parameter:%s", nodename);
		responseErrorMsg(client, faultString);
		goto ERROR_STATE;
	}

	if ( 0 != responseEndMsg(client) )
	{
		strcpy(faultString, "Internal response error(5)");
		responseErrorMsg(client, faultString);				
		goto ERROR_STATE;
	}

	pthread_mutex_unlock(&value_change_lock);

	return 0;

ERROR_STATE:
	pthread_mutex_unlock(&value_change_lock);

	return -1;

}

static pthread_t phoneRPCThread = 0; /* cpe RPC task thread */
/*
	cwmp RPC task for phone
*/
void *cwmpPhoneRPCTask(void)
{
#define SOCK_PHONE_RPC_PATH "/tmp/cwmp_phone_rpc_sock"
	int serv_sock = 0, cli_sock = 0, addr_len = 0, len = 0;
	int err_state = -1, isSetProcess = 0;
	struct sockaddr_un local = {0}, remote = {0};
	tcphone_rpc_msg msg;
	uint32_t recv_buf_len = 0;
	char *node_value = NULL;
	PhoneRPCMethodFun RPCMethodResponse = NULL;
	char faultString[1024] = {0};
#define MAX_TCAPI_CLIENT 5
#define PARA_SETCOMMIT_NAME "SETPARAMETERVALUES_COMMIT"

	if ( (serv_sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1 )
	{
	    tcdbg_printf("\n==>(%s), create socket error\n", __FUNCTION__);
	    close(serv_sock);
	    goto PHONE_RPC_EXIT;
	}

	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, SOCK_PHONE_RPC_PATH);
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	if ( bind(serv_sock, (struct sockaddr *)&local, len) == -1 )
	{
		tcdbg_printf("\n==>(%s), bind socket error\n", __FUNCTION__);
		close(serv_sock);
		goto PHONE_RPC_EXIT;
	}

	if ( listen(serv_sock, MAX_TCAPI_CLIENT) == -1 )
	{
		tcdbg_printf("\n==>(%s), listen socket error\n", __FUNCTION__);
		close(serv_sock);
		goto PHONE_RPC_EXIT;
	}

	while ( 1 )
	{
		addr_len = sizeof(remote);
		if ( (cli_sock = 
			accept(serv_sock, (struct sockaddr *)&remote, &addr_len)) == -1)
		{
			tcdbg_printf("\n==>(%s), accept socket error\n", __FUNCTION__);
			close(cli_sock);
			continue;
		}

		isSetProcess = 0;
		while ( 1 )
		{
			/* If the client start sending and server start reciving,
			the server has disappear before finish the process.
			 Kernel will kill the client process by using SIGPIPE.
			 we can use the flag MSG_NOSIGNAL to ignore SIGPIPE. */
			bzero(&msg, sizeof(msg));
			if( recv(cli_sock, &msg, sizeof(msg), MSG_NOSIGNAL) <= 0 )
			{
			/*
				tcdbg_printf("\n==>(%s), recv msg err or client disconnected\n"
					, __FUNCTION__); */
				close(cli_sock);
				cli_sock = 0;
				break;
			}
			else
			{
				if ( RPC_MSG_MAGIC != msg.magic
					|| RPC_CMD_REQ != msg.opcmd )
				{
					tcdbg_printf("\n==>(%s), end recv [%x]-[%x]\n"
						, __FUNCTION__, msg.magic, msg.opcmd );
					close(cli_sock);
					cli_sock = 0;
					break;
				}

				if ( 0 == strcmp(PARA_SETCOMMIT_NAME, msg.nodename) )
				{
				/*
				tcdbg_printf("\n==>(%s), Delay commit handle\n",
							__FUNCTION__);*/
					isSetProcess = 1;
					break;
				}

				recv_buf_len = msg.valuelen;
				/* receive nodevalue if need */
				if ( recv_buf_len )
				{
					if ( !node_value )
						node_value = (char *)
								calloc(recv_buf_len + 1, sizeof(char));

					if( recv(cli_sock, node_value, recv_buf_len
							, MSG_NOSIGNAL) <= 0 )
					{
						tcdbg_printf("\n==>(%s), recv node value error\n",
							__FUNCTION__);
						goto PHONE_RPC_LOOP;
					}
				}

				/* check RPC method */
				if ( 0 != 
					cwmpPhoneGetRPCResponseFun(msg.rpcmethod
											, &RPCMethodResponse) )
				{
					snprintf(faultString, sizeof(faultString) - 1
							, "Not Support the RPC Method:%s"
							, msg.nodename);
					responseErrorMsg(cli_sock, faultString);
					goto PHONE_RPC_LOOP;
				}

				/* send node message to phone */
				if ( RPCMethodResponse )
				{
					err_state = RPCMethodResponse(msg.nodename
						, node_value, cli_sock);
					if ( 0 != err_state )
						goto PHONE_RPC_LOOP;
				}

				if ( node_value )
				{
					free(node_value);
					node_value = NULL;
				}
			}
		}/* while ( 1 ) */

		/* need commit for set parameter */
		if ( isSetProcess )
		{
			HandleParametersSetting( pTcCfgNodes, COMMIT, NODELAY );
			HandleParametersSetting( pTcCfgNodes, COMMIT, DELAY );
		}
		/*tcapi_save();*/
PHONE_RPC_LOOP:
		/* free old buffer if exist. */
		if ( node_value )
		{
			free(node_value);
			node_value = NULL;
		}
		if ( cli_sock )
			close(cli_sock);
	}

PHONE_RPC_EXIT:
	unlink(SOCK_PHONE_RPC_PATH);
	tcdbg_printf("\n==>(%s), do thread exit\n", __FUNCTION__);
	phoneRPCThread = 0;
	return NULL;
}

#endif

/*
	create RPC task for phone
*/
int cwmpPhoneRPCTaskStart(void)
{
#if defined(TCSUPPORT_CT_JOYME)
	pthread_attr_t cwmprpchandle_attr;
	int ret = 0;

	/* create thread */
	ret = pthread_attr_init(&cwmprpchandle_attr);
	if ( 0 != ret  )
	{
		tcdbg_printf("\nThread(%s) attribute creation fail!\n"
			, __FUNCTION__);
		return -1;
	}

	ret = pthread_attr_setdetachstate(&cwmprpchandle_attr
		, PTHREAD_CREATE_DETACHED);
	if(ret != 0)
	{
		tcdbg_printf("\nThread(%s):Set attribute fail!\n", __FUNCTION__);
		return -1;
	}

	ret = pthread_create(&phoneRPCThread, &cwmprpchandle_attr
		, (void *)cwmpPhoneRPCTask, NULL);
	if ( 0 != ret )
	{
		tcdbg_printf("\nThread(%s):Create thread fail!\n", __FUNCTION__);
		return -1;
	}

	pthread_attr_destroy(&cwmprpchandle_attr);
#endif

	return 0;
}

#if defined(TCSUPPORT_CT_JOYME)
static int dns_speed_limit_notify = 0;
extern int getInternetWanHgwInfo(char *wan);
extern int getDnsSpeedLimitDeviceInfo(char **info);
#endif

int getDnsSpeedLimitFlag(void)
{
#if defined(TCSUPPORT_CT_JOYME)
	return dns_speed_limit_notify;
#else
	return 0;
#endif
}
int setDnsSpeedLimitFlag(int flag)
{
#if defined(TCSUPPORT_CT_JOYME)
	dns_speed_limit_notify = flag;
	if ( 0 == dns_speed_limit_notify )
	{
		unlink("/tmp/dns_speed_limit_inform_check");
	}
#endif
	return 0;
}

int8 cwmpBuildDnsSpeedLimitParameters(void)
{	
#if defined(TCSUPPORT_CT_JOYME)	
	char hgwInfo[64] = {0};
	int8 state = 0;
	char limit_domain[1024] = {0};
	char *deviceInfo = NULL;

	bzero(limit_domain, sizeof(limit_domain));
	if ( 0 != tcapi_get("DnsSpeedLimit_Common", "domain", limit_domain) )
		strcpy(limit_domain, "NULL");

	bzero(hgwInfo, sizeof(hgwInfo));
	getInternetWanHgwInfo(hgwInfo);

	/* add node name to parameter list */
	state = cwmpAddInformParameter("InternetGatewayDevice."
	"X_CT-COM_DNSSpeedLimit.Domain", limit_domain, StringType);
	if ( 0 != state )
		 return -1;

	state = cwmpAddInformParameter("InternetGatewayDevice."
	"X_CT-COM_DNSSpeedLimit.HgwInfo", hgwInfo, StringType);
	if ( 0 != state )
		 return -1;

	getDnsSpeedLimitDeviceInfo(&deviceInfo);
	state = cwmpAddInformParameter("InternetGatewayDevice."
	"X_CT-COM_DNSSpeedLimit.DeviceInfo"
	, deviceInfo ? deviceInfo : ""
	, StringType);

	if ( deviceInfo )
		free(deviceInfo);
	if ( 0 != state )
		 return -1;

#endif
	return 0;
}


int8 getManualInformFlag(void)
{
#if defined(TCSUPPORT_CUC_PON_SD)
	char tempbuf[32] = {0};

	tcapi_get("Cwmp_Entry","isManual",tempbuf);
	if(!strcmp(tempbuf,"1"))
		return 1;
	else
#endif
		return 0;
}

void setManualInformFlag(int flag)
{
#if defined(TCSUPPORT_CUC_PON_SD)
	if(flag)
		tcapi_set("Cwmp_Entry", "isManual","1");
	else
		tcapi_set("Cwmp_Entry", "isManual","0");
#endif
	return;
}

int updateCwmpBindInterface(char *oldiface, int nsize)
{
#if defined(TCSUPPORT_CT)
	int i, j;
	char active[12] = {0};
	char nodeName[48] = {0};
	char serviceType[64] = {0};
	char nas_name[64] = {0};

	if ( !oldiface )
		return -1;

	if ( 0 != oldiface[0] )
	{
		/* won't update iface if TR69 wan up. */
		return 0;
	}

	for ( i = 0; i < PVC_NUM; i++ )
	{
		for( j = 0; j < MAX_SMUX_NUM; j++ )
		{
			bzero(active, sizeof(active));
			bzero(serviceType, sizeof(serviceType));
			bzero(nas_name, sizeof(nas_name));
			sprintf(nodeName, "Wan_PVC%d_Entry%d", i, j);
			if ( 0 != tcapi_get(nodeName, "Active", active) )
				continue;
			tcapi_get(nodeName, "ServiceList", serviceType);
			tcapi_get(nodeName, "NASName", nas_name);

			if ( 0 == strcmp(active, "Yes")
				&& NULL != strstr(serviceType, TR069)
				&& 0 != nas_name[0] )
			{
				snprintf(oldiface, nsize - 1, "%s", nas_name);
				tcdbg_printf("\nTR69 not up, bind [%s] wan interface.\n"
					, oldiface);
				return 0;
			}
		}
	}
#endif

	return 0;
}


/* QoE task check: perform download qoe via wget. */
void cwmpPerformQoETask(void)
{
	return;
}

void processTheHostExt(Unsigned32* theHostAddress, Unsigned16* theHostPort, char *theHostPath, int8 saveType)
{
#if defined(TCSUPPORT_CMCC)
	processTheHostAddressExt(theHostAddress, theHostPort, theHostPath, saveType);
#endif
}

