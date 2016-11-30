#include "cfg_manager.h"
#include "utility.h"
#include <sys/un.h>

#if defined(TCSUPPORT_ECN_MEGACO)
#include <netinet/in.h>
#endif

#if VOIP
/* jrchen add 20100223 */
#define NODE_MAX_LEN 32
#define NET_LOG_FILE_NAME "/var/voice_netlog"
#define LINE_TEST_PROC_NAME "/proc/fxs/sliclinetest"



typedef struct single_node_s
{
	char attr[NODE_MAX_LEN];
	char value[NODE_MAX_LEN];
}single_node_t;

single_node_t log_default_setting[] = {
	{LOGENABLE, "No"}, 
	{LOGSERVERIP, "192.168.1.100"},
	{LOGSERVERPORT, "5678"},
	{LOGDISPLAY, "Console"},
	{"", ""},
};
char *
voip_sys_param_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
/* 20101220 PTchen modified for per ling setting */
char *
voip_sys_param_merge_table[MAX_NODE_NAME]=
{
	"LineNum","",
};

char *
voip_advanced_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
/* 20101220 PTchen modified for per ling setting */
char *
voip_advanced_merge_table[MAX_NODE_NAME]=
{
	"LineNum","",
};

char *
voip_call_ctrl_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};

char *
voip_call_ctrl_merge_table[MAX_NODE_NAME]=
{
	"LineNum","",
};


/* 20101220 PTchen modified end */



char *
voip_basic_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
char *
voip_basic_merge_table[MAX_NODE_NAME]=
{
	"SIPIndexBasic","",
};
// char *
// voip_call_ctrl_merge_table[MAX_NODE_NAME]=
// {
// 	"SIPIndexCall","",
// };
char *
voip_media_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
/* 20101220 PTchen modified for per line setting */
char *
voip_media_merge_table[MAX_NODE_NAME]=
{
	"LineNum","",
		
};
/* 20101220 PTchen modified end */
/* serena_modify */
char *
voip_speed_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
/* serena_modify end*/
char *
voip_speed_dial_merge_table[MAX_NODE_NAME]=
{
	"SIPIndexSpeed","",
};

	char sip_default_para[][2][25]={
		{"DebugLevel"			,"2"},
		{"Linenum"			,"2"},
		{"MaxCallLegs"			,"10"},
		{"ReferTimeout"			,"120000"},
		{"DialToneDuration"		,"15000"},
		//{"CallWaitingReply"		,"180"},
		{"WatchdogInterval"		,"180"},
		{"DtmfRelay"			,"0"},
		{"EnableSdpLogs"		,"1"},
		{"connectMediaOn180"		,"1"},
		{"autoAnswer"			,"0"},
		{"autoDisconnect"		,"0"},
		{"AddUpdateSupport"		,"1"},
		{"UpdateRetryAfterTimeout"	,"10"},
		{"CallerUpdateResendTimeout"	,"4000"},
		{"CalleeUpdateResendTimeout"	,"2000"},
		{"AutoRegistration"		,"1"},	
/*20100821 serena_modify*/
		/*2010/12/20 enhance by Sam*/
		//{"RegistrationExpire"		,"20"},	
		{"UnregistrationExpire"		,"10"},
		{"MaxRegisterClients"		,"5"},
		{"RemoveOldAuthHeaders"		,"1"},
		//{"EppPort"			,"3044"},
		{"localIpPrefix"		,"127.0.0.0"},
		{"localIpMask"			,"255.0.0.0"},
		{"CallForwardTimeout"	,"15"},	/*20101216_serena_modify for CFW*/
		{"disableGuiMedia","1"},
		{"",""},
	};
	
/*20110218_serena_modify for mtf log*/
	char ipp_log_default_para[][2][47]={
		{"IPP_UTIL"	,"ERROR,EXCEPTION,WARNING,INFO,DEBUG"},
		{"IPP_USERAPP"	,"ERROR,EXCEPTION,WARNING,INFO,DEBUG,ENTER,LEAVE"},
		{"IPP_CALLCON"	,"ERROR,EXCEPTION,WARNING,INFO,DEBUG"},
		{"IPP_MDM"	,"ERROR,EXCEPTION,WARNING,INFO,DEBUG"},
		{"IPP_MDMCONT"	,"ERROR,EXCEPTION,WARNING,INFO,DEBUG"},
		{"IPP_SIPCONT"	,"ERROR,EXCEPTION,WARNING,INFO,DEBUG"},
		{"MTF_BASE"	,"ERROR,EXCEPTION,WARNING,INFO,DEBUG"},
		{"RPOOL"	,"ERROR,EXCEPTION,WARNING"},
		{"RA"		,"ERROR,EXCEPTION,WARNING"},
		{"SOCKET"	,"ERROR,EXCEPTION,WARNING"},
		{"THREAD"	,"ERROR,EXCEPTION,WARNING"},
		{"MUTEX"	,"ERROR,EXCEPTION,WARNING"},
		{"SEMA4"	,"ERROR,EXCEPTION,WARNING"},
		{"LOCK"		,"ERROR,EXCEPTION,WARNING"},
		{"MEMORY"	,"ERROR,EXCEPTION,WARNING,DEBUG"},
		{"TIMER"	,"ERROR,EXCEPTION,WARNING"},
		{"QUEUE"	,"ERROR,EXCEPTION,WARNING"},
		{"SELECT"	,"ERROR,EXCEPTION,WARNING"},
		{"CLOCK"	,"ERROR,EXCEPTION,WARNING"},
		{"TIMESTAMP"	,"ERROR,EXCEPTION,WARNING"},
		{"ALLOC"	,"ERROR,EXCEPTION,WARNING"},
		{"",""},
	};
	char sip_log_default_para[][2][36]={
		{"callLogFilters"		,"INFO,ERROR,EXCEPTION,WARNING,DEBUG"},
		{"transactionLogFilters"	,"INFO,ERROR,EXCEPTION,WARNING,DEBUG"},
		{"msgLogFilters"		,"ERROR,EXCEPTION,WARNING,INFO,DEBUG"},
		{"transportLogFilters"		,"INFO,ERROR,EXCEPTION,WARNING,DEBUG"},
		{"parserLogFilters"		,"ERROR,EXCEPTION,WARNING,INFO,DEBUG"},
		{"stackLogFilters"		,"ERROR,EXCEPTION,WARNING,INFO,DEBUG"},
		{"msgBuilderLogFilters"		,"ERROR,EXCEPTION,WARNING,INFO,DEBUG"},
		{"authenticatorLogFilters"	,"ERROR,EXCEPTION,WARNING,INFO,DEBUG"},
		{"regClientLogFilters"		,"ERROR,EXCEPTION,WARNING,INFO,DEBUG"},
		{"subscriptionLogFilters"	,"ERROR,EXCEPTION,WARNING,INFO,DEBUG"},
		{"compartmentLogFilters"	,"ERROR,EXCEPTION,WARNING,INFO,DEBUG"},
		{"transmitterLogFilters"	,"ERROR,EXCEPTION,WARNING,INFO,DEBUG"},
		{"ads_rlistLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"ads_raLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"ads_rpoolLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"ads_hashLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"ads_pqueueLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"core_semaphoreLogFilters"	,"ERROR,EXCEPTION,WARNING"},
		{"core_mutexLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"core_lockLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"core_memoryLogFilters"	,"ERROR,EXCEPTION,WARNING"},
		{"core_threadLogFilters"	,"ERROR,EXCEPTION,WARNING"},
		{"core_queueLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"core_timerLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"core_timestampLogFilters"	,"ERROR,EXCEPTION,WARNING"},
		{"core_clockLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"core_tmLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"core_socketLogFilters"	,"ERROR,EXCEPTION,WARNING"},
		{"core_portrangeLogFilters"	,"ERROR,EXCEPTION,WARNING"},
		{"core_selectLogFilters"	,"ERROR,EXCEPTION,WARNING"},
		{"core_hostLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"core_tlsLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"core_aresLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"adsLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"coreLogFilters"		,"ERROR,EXCEPTION,WARNING,INFO,DEBUG"},
		{"",""},
	};
/*End 20110218_serena_modify for mtf log*/
	
	char media_ctrl_default_para[][2][ATTR_SIZE]={
		{"DefaultDevice"	,"1"},
		{"RTPAddress"		,"0.0.0.0"},
/*20100821 serena_modify: extend port range due to rtcp port*/
		{"RTPPortRange"		,"16"},
		//{"VideoFrameSize"	,"2"},
		//{"VideoFrameRate"	,"30"},
		{"RecvAudioPolling_ms"	,"20"},
		{"PlayAudioPolling_ms"	,"20"},
		//{"RecvVideoPolling_ms"	,"32"},
		//{"PlayVideoPolling_ms"	,"30"},
		{"MfLogFilters"		,"ERROR,EXCEPTION,WARNING,INFO"},
		{"",""},
	};

single_node_t digitmap_default_setting[] = {
	{DIGITMAPENABLE, "1"}, 
	{DIGITMAP, "x."},
	{DIALTIMEOUT, "4000"}, 
	{DIGITTIMEOUT, "15000"},
	{"", ""},
};

char *
voip_h248_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};

char *
info_voip_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
#if defined(TCSUPPORT_ECN_MEGACO)
#define  SOCKSIZE (sizeof(struct sockaddr))
#define HTTP_SOCKET_ERROR	-1
typedef unsigned long   StcpIpAddress;
typedef unsigned short  StcpPort;

int voipCmdSend(char * SendData);
#endif

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
/* mtk06404 20140102 */
static simulate_result_entry_t g_emulate_result_tbl[] = {
	{SIMULATE_NO_ERROR,                     "Error_None",       "Success"},
	{SIMULATE_CALLER_NO_DIAL_TONE,          "NoDialTone",       "Fail"},
	{SIMULATE_CALLER_OFFHOOK_RELEASE,       "OffhookRelease",   "Fail"},
	{SIMULATE_CALLER_DIALING_REALEASE,      "DialingRelease",   "Fail"},
	{SIMULATE_CALLER_AFTER_DIAL_REALEASE,   "AfterDialRelease", "Fail"},
	{SIMULATE_CALLER_NO_ANSWER,             "NoAnswer",         "Fail"},
	{SIMULATE_CALLER_KEY_ERROR,             "Key error",        "Fail"},
	{SIMULATE_CALLED_NO_INCOMING_CALL,      "NoIncomingCall",   "Fail"},
	{SIMULATE_CALLED_OFFHOOK_RELEASE,       "OffhookRelease",   "Fail"},
	{SIMULATE_CALLED_NO_ANSWER,             "NoAnswer",         "Fail"},
	{SIMULATE_BLOCK_REASON_RESULT,          "",         ""},
	{SIMULATE_ERROR_MAX,                    "default",          "Fail"},	
};

static simulate_status_entry_t g_emulate_status_tbl[] = {
	{SIMULATE_STATUS_IDLE,              "Idle"},
	{SIMULATE_STATUS_OFFHOOK,           "Off-hook"},
	{SIMULATE_STATUS_DIAL_TONE,         "Dialtone"},
	{SIMULATE_STATUS_RECEIVING,         "Receiving"},
	{SIMULATE_STATUS_RECEIVE_END,       "ReceiveEnd"},
	{SIMULATE_STATUS_RINGING_BACK,      "Ringing-back"},
	{SIMULATE_STATUS_CONNECTED,         "Connected"},
	{SIMULATE_STATUS_TEST_END,          "Testend"},
	{SIMULATE_STATUS_TEST_MAX,          "default"},	
};

int VOIPLineNum = 1;
extern voip_simulate_state_t gVoipSimulateState[VOIP_LINE_NUM];

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
int prevSipStatus=0;
int sipStatusFlag=0;
int getSipStatus(void){
	return prevSipStatus;
}
#endif

void deRegBeforeReboot(mxml_node_t *top){
    int i = 0;
    char cmd[64] = {0};
    char tmp[32] = {0};
    
    char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
    
    
    memset(nodeName,0,sizeof(nodeName));
    strcpy(nodeName[0],VOIPBASIC);
    
    for(i=0 ; i<VOIPLineNum ; i++){
        sprintf(nodeName[1],"Entry%d",i);
        if(getAttrValue(top, nodeName, "Enable", tmp) == TCAPI_PROCESS_OK){
            memset(cmd, 0, sizeof(cmd));			
            if(strcmp(tmp,SIP_YES) == 0){
                sprintf(cmd,"/userfs/bin/sipclient -d %d",i);
                tcdbg_printf("Before reboot, deregister [cmd:%s]\n", cmd);
                system(cmd);
            }
        }
    }

    return;
}

void updateVoipLineNum(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[32];

	memset(nodeName,0,sizeof(nodeName));
	memset(tmp, 0, sizeof(tmp));
	
	strcpy(nodeName[0],VOIPBASIC);
	sprintf(nodeName[1],"Common");

	if(getAttrValue(top, nodeName, "VoIPLine2Enable", tmp) == TCAPI_PROCESS_OK)
	{
		if(strcmp(tmp,"Yes") == 0)
		{
			VOIPLineNum = 2;
		}
		else
		{
			VOIPLineNum = 1;
		}
	}
}

void simulate_result_set(int line_id, int errorCode, char *confirmResult)
{
	simulate_result_entry_t *pEntry = NULL;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tmpStr[MAX_NODE_NAME] = {0};
	mxml_node_t * top = NULL;
	char tmpStr1[MAX_VALUE_SIZE] = {0};

	top = get_node_top();
	
	memset(nodeName,0,sizeof(nodeName));	
	strncpy(nodeName[0], VOIPSIMULATETEST, sizeof(nodeName[0])-1);
	snprintf(nodeName[1], sizeof(nodeName[1])-1, "Entry%d", line_id);
	snprintf(tmpStr1, sizeof(tmpStr1)-1, "%d", errorCode);
	strncpy(tmpStr, "CallerFailReason", sizeof(tmpStr)-1);
		
	if (SIMULATE_CALLED == gVoipSimulateState[line_id].testType){
		strncpy(tmpStr, "CalledFailReason", sizeof(tmpStr)-1);
	}
	
	pEntry = &g_emulate_result_tbl[0];
	while (SIMULATE_ERROR_MAX != pEntry->errorCode){
		if (pEntry->errorCode == errorCode){			
			setAttrValue(top, nodeName, tmpStr, pEntry->errorStr);
			setAttrValue(top, nodeName, "Conclusion", pEntry->conclusion);
			/* [OSBNB00052025] ADD by Eric.guo@20151109 modify the simulate procedure,the node FailedResponseCode only save the error code from server*/
			//setAttrValue(top, nodeName, "FailedResponseCode", tmpStr1);
			setAttrValue(top, nodeName, "DailDTMFConfirmResult", confirmResult);			
			
			tcdbg_printf("[Simulate Test] %s [%d], errorCode:%d, errorStr:%s\n", 
						__func__, line_id, errorCode, pEntry->errorStr);
			break;
		}
		
		pEntry++;
	}

	setAttrValue(top,nodeName, LINE_SIMULATE_TYPE, "");

	return;
}

void simulate_status_set(int line_id, int statusCode){
	simulate_status_entry_t *pEntry = NULL;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t * top = NULL;

	top = get_node_top();
	memset(nodeName,0,sizeof(nodeName));	
	strncpy(nodeName[0], VOIPSIMULATETEST, sizeof(nodeName[0])-1);
	snprintf(nodeName[1], sizeof(nodeName[1])-1, "Entry%d", line_id);

	pEntry = &g_emulate_status_tbl[0];
	while (SIMULATE_STATUS_TEST_MAX != pEntry->statusCode){
		if (pEntry->statusCode == statusCode){
			setAttrValue(top, nodeName, "Status", pEntry->statusStr);
			gVoipSimulateState[line_id].line_state = statusCode;
			tcdbg_printf("[Simulate Test] %s [%d], statusCode:%d, statusStr:%s\n", 
							__func__, line_id, statusCode, pEntry->statusStr);
			break;
		}
		
		pEntry++;
	}
	return;
}

void simulate_testState_set(int line_id, char *state){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t * top = NULL;

	top = get_node_top();
	memset(nodeName,0,sizeof(nodeName));	
	strncpy(nodeName[0], VOIPTEST, sizeof(nodeName[0])-1);
	snprintf(nodeName[1], sizeof(nodeName[1])-1, "Entry%d", line_id);
	setAttrValue(top, nodeName, "TestState", state);
	setAttrValue(top, nodeName, "IsTestOnBusy", "0");
#if defined(TCSUPPORT_ECN_MEGACO)
#if defined(TCSUPPORT_ECN_SIP)
	if(getSipStatus() == 1) {
#endif
		memset(nodeName,0,sizeof(nodeName));
		strncpy(nodeName[0], VOIPINFO_H248, sizeof(nodeName[0])-1);
		snprintf(nodeName[1], sizeof(nodeName[1])-1, "Entry%d", line_id);
		setAttrValue(top, nodeName, SIMULATE_START, SIMULATE_STOP);
#if defined(TCSUPPORT_ECN_SIP)
	}
#endif
#endif
	return;
}
#endif

/*______________________________________________________________________________
**	voip_sys_param_init
**
**	descriptions:
**		the init function of voipbasic.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int getTokens(const char *inStr, const char* delim, char *pArgv[]) {
	int numOfToken = 0;
	static char tmp[128] = {0};
	char *t;

	strcpy(tmp, inStr);
	t = strtok(tmp, delim);
	
	while(t && numOfToken < 5)	{
		pArgv[numOfToken++] = t;
		t = strtok(NULL, delim);
	}

	return numOfToken;
}

int
voip_sys_param_init(void){
	cfg_node_t node={
		.name=VOIPSYSPARAM,
		.type=ADV_FLAG|ENTRY_TYPE|UPDATE_TYPE|VOIP_LINE_NUM|WEB_TYPE,
		.adv_table=voip_sys_param_adv_table,
		.merge_table=voip_sys_param_merge_table,
		.cfg_read=voip_sys_param_read,
		.cfg_write=voip_sys_param_write,
		.cfg_verify=voip_sys_param_verify,
		.cfg_execute=voip_sys_param_execute,
		.cfg_boot=voip_sys_param_boot,
	};
	return register_cfg_node(&node);
}
/*______________________________________________________________________________
**	voip_basic_read
**
**	descriptions:
**		update registrar connection status.
**	parameters:
**		top: 	Specify the root of mxml_node_t structure.
**		parant: Specify the parant node of voip_basic cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/





int
voip_sys_param_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
	/*

	char dev_pvc_attr[][16]=
	{
		{"SC_SYS_CFG_MAX_CALL"},{"SC_SYS_CFG_MAX_ACCT"},{"SC_SYS_CFG_MAX_LINE"},
		{"SC_SYS_CFG_MAX_CALL_PER_ACCT"},{"SC_SYS_LED_CFG"},
		{"SC_SYS_SIP_T1_INTERVAL"},{"SC_SYS_SIP_TRANSPORT_TYPE"},
		{"SC_SYS_SIP_SUPPORTED"},{"SC_SYS_SIP_PRIORITY_URGENT_NUMS"},
		{"SC_SYS_SIP_PRIORITY_NORMAL_NUMS"},{"SC_SYS_SIP_PRIORITY_NON_URGENT_NUMS"},
		{"SC_SYS_QOS_SIP_TOS"},{"SC_SYS_QOS_RTP_TOS"},
		{"SC_SYS_PLUGIN_FILE"},{"SC_SYS_SPEED_UP_DIALING"},
		{"SC_SYS_SPEED_UP_DIALING_STR"},{"SC_SYS_MAGIC_NUM"},
		{"SC_SF_CS_PROTOCOL"},{"SC_SF_CS_QOS"},
		{"SC_SF_CS_TOS"},{"SC_SF_CS_SRC_PORT"},
		{"SC_SF_CS_DST_PORT"},{"SC_SF_CS_SRC_IP"},
		{"SC_SF_CS_DST_IP"},{"SC_FAX_LEC_FORCE_OFF"},
		{"SC_FAX_PASSTHRU_PCMU"},{"SC_FAX_ONLY_TIMER"},
		{"SC_FAX_REINVITE_ONLY_FROM_RECEIVER"},{"SC_MEDIA_CODEC_G726_16_PT"},
		{"SC_MEDIA_CODEC_G726_24_PT"},{"SC_MEDIA_CODEC_G726_32_PT"},
		{"SC_MEDIA_CODEC_G726_40_PT"},{"SC_MEDIA_CODEC_ILBC_PT"},
		{"SC_MEDIA_CODEC_TELEVT_PT"},{"SC_MEDIA_CODEC_ILBC_PTIME"},
		{"SC_MEDIA_CODEC_G723_FMTP"},{"SC_MEDIA_CODEC_SINGLE_CODEC"},
		{"SC_EMERG_ENABLE"},{"SC_EMERG_NUM_GENERIC"},
		{"SC_EMERG_NUM_POLICE"},{"SC_EMERG_NUM_MEDICAL"},
		{"SC_EMERG_NUM_FIRE"},{"SC_EMERG_REGISTRATION"},
		{"SC_EMERG_TIMER_WMX_CONNECT"},{"SC_EMERG_FORCE_WMX_AUTO_RECONNECT"},
		{"SC_ACCT_SYS_STAND_ALONE"},{"SC_ACCT_SIP_DOMAIN"},
		{"SC_ACCT_SIP_SEP_REG_ADDR"},{"SC_ACCT_SIP_SEP_REG_PORT"},
		{"SC_ACCT_SIP_USER_AGENT"},{"SC_ACCT_SIP_INFO_ENABLE"},
		{"SC_ACCT_SIP_MWI_INTERVAL"},{"SC_ACCT_SIP_MWI_EVENT"},
		{"SC_ACCT_SIP_SESSION_FLAG"},{"SC_ACCT_SIP_SESSION_REFRESHER"},
		{"SC_ACCT_SIP_SESSION_METHOD"},{"SC_ACCT_SIP_SESSION_MIN_EXP"},
		{"SC_ACCT_SIP_SESSION_TIMER"},{"SC_ACCT_SIP_P2P_CALL"},
		{"SC_ACCT_SIP_DELAY_MEDIA"},{"SC_ACCT_SIP_PRIORITY"},
		{"SC_ACCT_SIP_PRI_OTHER"},{"SC_ACCT_NAT_SRV_ADDR"},
		{"SC_ACCT_NAT_SRV_PORT"},{"SC_ACCT_NAT_TYPE"},
		{"SC_ACCT_MEDIA_RTP_DETECT"},{"SC_ACCT_MEDIA_RTP_DETECT_INTERVAL"},
		{"SC_ACCT_MEDIA_RTP_DETECT_PKT_LOSS"},{"SC_ACCT_MEDIA_6TH_CODEC"},
		{"SC_ACCT_MEDIA_7TH_CODEC"},{"SC_ACCT_MEDIA_8TH_CODEC"},
		{"SC_ACCT_MEDIA_9TH_CODEC"},{"SC_ACCT_MEDIA_G723_RATE"},
		{"SC_ACCT_MEDIA_ILBC_RATE"},{"SC_ACCT_DP_COMMON"},
		{""},{""},
		{""},{""},
		{""},{""},
		{""},{""},
	};
*/
	return SUCCESS;

}



int
voip_sys_param_write(mxml_node_t *top, mxml_node_t *parant){

	return voip_real_write(top);
}
/*______________________________________________________________________________
**	voip_basic_verify
**
**	descriptions:
**		To check the system parameter of voip_basic element is validly or not.
**
**	parameters:
**		node: 		Specify the root of mxml_node_t structure.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
voip_sys_param_verify(mxml_node_t *node){

	return SUCCESS;
}

int
voip_sys_param_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){

	return voip_real_execute(top,name);
}

int
voip_sys_param_boot(mxml_node_t *top){
	
	FILE *fp;
	char strLine[128];
	char *param[5];
	mxml_node_t *parentNode = NULL;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], VOIPSYSPARAM);

	fp = fopen("/etc/voip_sys.cfg","r");
	if(fp == NULL)
		return -1;
	
	while (fgets(strLine, sizeof(strLine)-1, fp) != NULL) {
		strLine[strlen(strLine) - 1] = '\0';
		getTokens(strLine, "=", param);

		if ((NULL == strstr(param[0], "_ACCT_")) && (NULL == strstr(param[0], "_LINE_"))) {
			strcpy(nodeName[1], "Common");
			parentNode=get_web_node(top,nodeName);
			mxmlElementSetAttr(parentNode,param[0],param[1]);
		}
		else {
			strcpy(nodeName[1], "Entry0");
			parentNode=get_web_node(top,nodeName);
			mxmlElementSetAttr(parentNode,param[0],param[1]);
			
			strcpy(nodeName[1], "Entry1");
			parentNode=get_web_node(top,nodeName);
			mxmlElementSetAttr(parentNode,param[0],param[1]);	
		}
	}
	return SUCCESS;
}

/*______________________________________________________________________________
**	voip_basic_init
**
**	descriptions:
**		the init function of voipbasic.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/

int
voip_advanced_init(void){

	cfg_node_t node={
		.name=VOIPADVANCED,
/* 20101220 PTchen modified  UPDATE_TYPE not use  */
		.type=ADV_FLAG|ENTRY_TYPE|VOIP_LINE_NUM,
		.adv_table=voip_advanced_adv_table,
		.merge_table=voip_advanced_merge_table,
		.cfg_read=NULL, /*20110527_serena_modify it doesn't need to register this function*/
		.cfg_write=voip_advanced_write,
		.cfg_verify=voip_advanced_verify,
		.cfg_execute=voip_advanced_execute,
		//.cfg_boot=voip_advanced_boot,
	};

	return register_cfg_node(&node);
}

int
voip_advanced_write(mxml_node_t *top, mxml_node_t *parant){
	
	return voip_real_write(top);
}
/*______________________________________________________________________________
**	voip_basic_verify
**
**	descriptions:
**		To check the system parameter of voip_basic element is validly or not.
**
**	parameters:
**		node: 		Specify the root of mxml_node_t structure.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
voip_advanced_verify(mxml_node_t *node){
	
	return SUCCESS;
}

int
voip_advanced_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	
	return voip_real_execute(top,name);
}








int
voip_basic_init(void){

	cfg_node_t node={
		.name=VOIPBASIC,
		.type=ADV_FLAG|ENTRY_TYPE|UPDATE_TYPE|VOIP_LINE_NUM,
		.adv_table=voip_basic_adv_table,
		.merge_table=voip_basic_merge_table,
		.cfg_read=voip_basic_read,
		.cfg_write=voip_basic_write,
		.cfg_verify=voip_basic_verify,
		.cfg_execute=voip_basic_execute,
		.cfg_boot=voip_basic_boot,
	};

	return register_cfg_node(&node);
}

/*______________________________________________________________________________
**	voip_basic_boot
**
**	descriptions:
**		the boot function of voipbasic.
**	parameters:
**		top: 	Specify the root of mxml_node_t structure.
**	return:
**		Success: 	0
**		Otherwise:     -1
**_______________________________________________________________________________
*/
int
voip_basic_boot(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[32];
	int i, enablenum=0;
	char tmpStr[64];
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPBASIC);
	
	updateVoipLineNum(top);

	
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
	memset(nodeName,0,sizeof(nodeName));	
	strcpy(nodeName[0],VOIPBASIC);
	strcpy(nodeName[1],"Common");
	getAttrValue(top, nodeName, "SIPProtocol", tmp);
	if(strcmp(tmp,"H.248") == 0)
		prevSipStatus = 1;
	else 
		prevSipStatus = 0;
	tcdbg_printf("VoIPBasic_Common SIPProtocol is %s\r\n", tmp);
	tcdbg_printf("voip_basic_boot: prevSipStatus is %d\r\n", prevSipStatus);
#endif

	
	//system("/userfs/bin/sipclient &");	
	//sleep(1);

#if 0	
	//sip basic username password
	for(i=0 ; i<VOIP_LINE_NUM ; i++){
		sprintf(nodeName[1],"Entry%d",i);
		if(getAttrValue(top, nodeName, "SIPEnable", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
				enablenum++;
				if(!pre_sys_state.voip_service){
					memset(tmpStr,0,sizeof(tmpStr));
					sprintf(tmpStr,"/userfs/bin/sipclient -d %d",i);
					system(tmpStr);
					
					sleep(1);
					sprintf(tmpStr,"/userfs/bin/sipclient -u %d",i);
					system(tmpStr);

					sleep(1);
					sprintf(tmpStr,"/userfs/bin/sipclient -c %d",i);
					system(tmpStr);
					
				}
			}
		}
	}
	if(enablenum > 0){
		pre_sys_state.voip_service = 1;
	}
#endif	
//	system("/userfs/bin/sipclient &");		
	return SUCCESS;
}

/*______________________________________________________________________________
**function :isVoIPAppDown
**
**description:
*    check sipclient or mgapp  crash or not
**parameters:
*    type: 0 - mean check sipclient
*              1 - mean check mgapp
**global:
*    none
**return: 
*    ret : 0 -voip app run ok 
*            1 - voip app crash
**call:
*    none
**revision:
*  
**____________________________________________________________________________*/

static int isVoIPAppDown(int type)
{
    FILE *fp = NULL;
    char cmdbuf[128] = {0};
    char voip_pid_tmp[32] = {0};
    char path[] = "/tmp/voipapp.pid";
    int  ret = 0;

    enum{
	    TYPE_SIP,
	    TYPE_MGAPP,    
    };
    
    fp = fopen(path, "r");
    if(fp == NULL){
        /*get sipclient or mgapp  process pid*/
        if (type == TYPE_SIP)
            sprintf(cmdbuf, "pidof %s > %s", "sipclient", path);
        else if (type == TYPE_MGAPP)
            sprintf(cmdbuf, "pidof %s > %s", "mgapp", path);
        else
            return ret;
        
        system(cmdbuf);  
        fp = fopen(path, "r");
    }
    if(fp == NULL){
        tcdbg_printf("open %s fail\n", path);
        return ret;
    }
    fgets(voip_pid_tmp, sizeof(voip_pid_tmp), fp);
    fclose(fp);
    unlink(path);
    if(atoi(voip_pid_tmp) == 0){  /*if sip_pid_tmp == 0, that's mean need restart sipclient*/
        tcdbg_printf("\n%s crash!!\n", (type == TYPE_SIP?"sipclient":"mgapp"));
        ret = 1;
    }
    return ret;
}

/*______________________________________________________________________________
**	voip_basic_read
**
**	descriptions:
**		update registrar connection status.
**	parameters:
**		top: 	Specify the root of mxml_node_t structure.
**		parant: Specify the parant node of voip_basic cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
voip_basic_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr){
	
	FILE *fp;
	/*20110527_serena_add for setting ConnState by line*/
	char tmpName[30]="";
	int i;
	/*End 20110527_serena_add for setting ConnState by line*/
	char buf[16]="";
	mxml_node_t *parentNode = NULL;
	
	parentNode = mxmlFindElement(top, top, DEVICEINFO,
				NULL, NULL, MXML_DESCEND);
	if(parentNode==NULL){
		parentNode =mxmlNewElement(top,  DEVICEINFO);
	}
	
	
	/*20110527_serena_modify for setting ConnState by line*/
	updateVoipLineNum(top);
	for(i=0; i<VOIPLineNum; i++){
		sprintf(tmpName,"/etc/sip_status%d",i);
		fp=fopen(tmpName,"r");
	
		memset(buf,0,sizeof(buf));
		if(fp != NULL){
			fgets(buf,sizeof(buf),fp);
			fclose(fp);
		}

		sprintf(tmpName,"ConnStat%d",i);
		if(strlen(buf) > 0){
			mxmlElementSetAttr(parentNode, tmpName, buf);
		}else{
			mxmlElementSetAttr(parentNode, tmpName, "Not Connected");
		}
	}
	/*End 20110527_serena_modify for setting ConnState by line*/
	return 0;
}
/*______________________________________________________________________________
**	voip_basic_write
**
**	descriptions:
**		Find voip_basic element from mxml tree then write voip_basic's system 
		parameter to specify configuration files.
**	parameters:
**		top: 	Specify the root of mxml_node_t structure.
**		parant: Specify the parant node of voip_basic cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
voip_basic_write(mxml_node_t *top, mxml_node_t *parant){
	
	return voip_real_write(top);
}
/*______________________________________________________________________________
**	voip_basic_verify
**
**	descriptions:
**		To check the system parameter of voip_basic element is validly or not.
**
**	parameters:
**		node: 		Specify the root of mxml_node_t structure.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
voip_basic_verify(mxml_node_t *node){
	
	return SUCCESS;
}
/*_____________________________________________________________________________
**      function name: voip_basic_execute
**      descriptions:
**            To execute wan node service.
**
**      parameters:
**            node:   Put the voip_basic of cfg node tree that is used to execute VoIP service.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**____________________________________________________________________________
*/
int
voip_basic_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	
	return voip_real_execute(top,name);
}


/*______________________________________________________________________________
**	voip_call_ctrl_init
**
**	descriptions:
**		the init function of voip call ctrl.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
voip_call_ctrl_init(void){


	cfg_node_t node={
		.name=VOIPCALLCTRL,
/* 20101220 PTchen modified  for per line setting */
/* 1. add adv_table and merge_table */
/* 2. SINGLE_TYPE modified to ENTRY_TYPE */
		.type=ENTRY_TYPE|VOIP_LINE_NUM,
		.adv_table=voip_call_ctrl_adv_table,
		.merge_table=voip_call_ctrl_merge_table,
		.cfg_read=NULL,
		.cfg_write=voip_call_ctrl_write,
		.cfg_verify=voip_call_ctrl_verify,
		.cfg_execute=voip_call_ctrl_execute,
		.cfg_boot=NULL,
	};

	return register_cfg_node(&node);
}

/*______________________________________________________________________________
**	voip_call_ctrl_write
**
**	descriptions:
**		Find voip_call_ctrl element from mxml tree then write voip_call_ctrl's system 
		parameter to specify configuration files.
**	parameters:
**		top: 	Specify the root of mxml_node_t structure.
**		parant: Specify the parant node of voip_call_ctrl cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
voip_call_ctrl_write(mxml_node_t *top, mxml_node_t *parant){

	return voip_real_write(top);
}
/*______________________________________________________________________________
**	voip_call_ctrl_verify
**
**	descriptions:
**		To check the system parameter of voip_call_ctrl element is validly or not.
**
**	parameters:
**		node: 		Specify the root of mxml_node_t structure.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
voip_call_ctrl_verify(mxml_node_t *node){

	return SUCCESS;
}
/*_____________________________________________________________________________
**      function name: voip_call_ctrl_execute
**      descriptions:
**            To execute voip_call_ctrl node service.
**
**      parameters:
**            node:   Put the voip_call_ctrl of cfg node tree that is used to execute VoIP service.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**____________________________________________________________________________
*/
int
voip_call_ctrl_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){

	return voip_real_execute(top,name);
}


/*______________________________________________________________________________
**	voip_media_init
**
**	descriptions:
**		the init function of voip media.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/

int
voip_media_init(void){
	
	cfg_node_t node={
		.name=VOIPMEDIA,
/* 20101220 PTchen modified  codec num to line num*/
/* before, codec setting placed in 4 entry*/
/* now, all codec setting placed in same entry and 2 entry used for 2 line setting*/
		.type=ADV_FLAG|ENTRY_TYPE|VOIP_LINE_NUM,
		.adv_table=voip_media_adv_table,
		.merge_table=voip_media_merge_table,
		.cfg_read=NULL,
		.cfg_write=voip_media_write,
		.cfg_verify=voip_media_verify,
		.cfg_execute=voip_media_execute,
		.cfg_boot=NULL,
	};

	return register_cfg_node(&node);
}


/*______________________________________________________________________________
**	voip_media_write
**
**	descriptions:
**		Find voip_media element from mxml tree then write voip_media's system 
		parameter to specify configuration files.
**	parameters:
**		top: 	Specify the root of mxml_node_t structure.
**		parant: Specify the parant node of voip_media cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
voip_media_write(mxml_node_t *top, mxml_node_t *parant){

	return voip_real_write(top);
}
/*______________________________________________________________________________
**	voip_media_verify
**
**	descriptions:
**		To check the system parameter of voip_media element is validly or not.
**
**	parameters:
**		node: 		Specify the root of mxml_node_t structure.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
voip_media_verify(mxml_node_t *node){

	return SUCCESS;
}
/*_____________________________________________________________________________
**      function name: voip_media_execute
**      descriptions:
**            To execute voip_media node service.
**
**      parameters:
**            node:   Put the voip_media of cfg node tree that is used to execute VoIP service.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**____________________________________________________________________________
*/
int
voip_media_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	
	return voip_real_execute(top,name);
}


/*______________________________________________________________________________
**	voip_speed_dial_init
**
**	descriptions:
**		the init function of voip speed dial. 
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
voip_speed_dial_init(void){
	
	cfg_node_t node={
		.name=VOIPSPEED,
/* serena_modify */

		.type=ADV_FLAG|ENTRY_TYPE|PHONE_BOOK_NUM,
		.adv_table=voip_speed_adv_table,
/* serena_modify end */
		
		.merge_table=voip_speed_dial_merge_table,
		.cfg_read=NULL,
		.cfg_write=voip_speed_dial_write,
		.cfg_verify=voip_media_verify,
		.cfg_execute=voip_speed_dial_execute,
		.cfg_boot=NULL,
	};

	return register_cfg_node(&node);
}
/*______________________________________________________________________________
**	voip_speed_dial_write
**
**	descriptions:
**		Find voip_media element from mxml tree then write voip_speed_dial's system 
		parameter to specify configuration files.
**	parameters:
**		top: 	Specify the root of mxml_node_t structure.
**		parant: Specify the parant node of voip_speed_dial cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
voip_speed_dial_write(mxml_node_t *top, mxml_node_t *parant){

	return voip_real_write(top);
}
/*______________________________________________________________________________
**	voip_speed_dial_verify
**
**	descriptions:
**		To check the system parameter of voip_speed_dial element is validly or not.
**
**	parameters:
**		node: 		Specify the root of mxml_node_t structure.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
voip_speed_dial_verify(mxml_node_t *node){

	return SUCCESS;
}
/*_____________________________________________________________________________
**      function name: voip_speed_dial_execute
**      descriptions:
**            To execute speed_dial node service.
**
**      parameters:
**            node:   Put the voip_speed_dial of cfg node tree that is used to execute VoIP service.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**____________________________________________________________________________
*/
int
voip_speed_dial_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){

	return voip_real_execute(top,name);
}
/*______________________________________________________________________________
**	voip_real_write
**
**	descriptions:
**		Find all voip element from mxml tree then write voip's system 
		parameter to specify configuration files.
**	parameters:
**		top: 	Specify the root of mxml_node_t structure.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
voip_real_write(mxml_node_t *top){
	//tcdbg_printf("%s \n",__func__);
	
	FILE *fp;
	int i;
	char local_rtp_port[6], local_addr[18];
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	fp = fopen("/etc/SIPPhone.cfg","w");
	if(fp == NULL)
		return -1;
	
	fputs("[SIPParameters]\n",fp);
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"Lan");
	strcpy(nodeName[1],"Entry0");
	getAttrValue(top, nodeName, "IP", local_addr);
	voip_put_str(fp,"LocalAddress",local_addr);
	voip_put_str(fp,"UserDomain",local_addr);
	
	
	for(i=0 ; strlen(sip_default_para[i][0]) != 0 ; i++){
		voip_put_str(fp,sip_default_para[i][0],sip_default_para[i][1]);
	}
	put_basic(fp,top);		
	
	
	put_log(fp, top); /* jrchen add 20100226 */

	/* PT Chen 20101027 */
	fputs("\n\n[CallControl]\n",fp);
	put_call_ctrl(fp,top);
	fputs("\n\n[Advanced]\n",fp);
	put_advanced(fp,top);

	fputs("\n\n[DigitMap]\n", fp);
	put_digitmap(fp, top);
	
	fputs("\n\n[IppLogOptions]\n",fp);
	
	for(i=0 ; strlen(ipp_log_default_para[i][0]) != 0 ; i++){
		voip_put_str(fp,ipp_log_default_para[i][0],ipp_log_default_para[i][1]);
	}
	
	fputs("\n\n[SipLogOptions]\n",fp);
	
	for(i=0 ; strlen(sip_log_default_para[i][0]) != 0 ; i++){
		voip_put_str(fp,sip_log_default_para[i][0],sip_log_default_para[i][1]);
	}
	
	fputs("\n\n[MediaParam]\n",fp);
/* serena_modify */
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"VoIPBasic");
	strcpy(nodeName[1],"Common");		
/* serena_modify end*/
	if(getAttrValue(top, nodeName, "LocalRTPPort", local_rtp_port) != TCAPI_PROCESS_OK){
		strcpy(local_rtp_port,"4000");
	}
	voip_put_str(fp,"RTPPortBase",local_rtp_port);
		
	for(i=0 ; strlen(media_ctrl_default_para[i][0]) != 0 ; i++){
		voip_put_str(fp,media_ctrl_default_para[i][0],media_ctrl_default_para[i][1]);
	}
	put_media(fp, top, local_addr, local_rtp_port);
	put_phone_number(fp, top);
	
	fputs("\n[SIPStackParameters]\n",fp);
	/* Enhance by sam 2010/12/20 */
	put_SIPStackParameters(fp, top);

	fclose(fp);
	
	return SUCCESS;
}
/*_____________________________________________________________________________
**      function name: voip_real_execute
**      descriptions:
**            All voip execute function will call this function to restart 
**		voip service.
**      parameters:
**            node:   Put the voip of cfg node tree that is used to execute VoIP service.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**____________________________________________________________________________
*/
int
voip_real_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	//tcdbg_printf("%s \n",__func__);
	FILE *fp;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[32],tmpStr[64],tmpStr2[64],mtkSDStr[128],mtkSDNum[16];
	int i, enablenum=0;
	struct stat stat_buf;
	mxml_node_t *parentNode = NULL;
	char* netlogFile = NET_LOG_FILE_NAME;

#if defined(TCSUPPORT_ECN_SIP)
	char InfoVoIP_node[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char InfoVoIP_node_wanset[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char InfoVoIP_node_tmp[32] = {0};
#endif
#if defined(TCSUPPORT_ECN_SIP) && defined(TCSUPPORT_ECN_MEGACO)
	int curSipStatus=0;
#endif
tcdbg_printf("%s in\n",__func__);

	
	/*for(i=0;i<RETRY_TIME;i++){
		if(stat("/etc/restart_voip",&stat_buf) < 0){
			break;
		}else{
			sleep(1);
		}
	}*/

    updateVoipLineNum(top);

#if defined(TCSUPPORT_ECN_SIP) && defined(TCSUPPORT_ECN_MEGACO)
	memset(nodeName,0,sizeof(nodeName));	
	strcpy(nodeName[0],VOIPBASIC);
	strcpy(nodeName[1],"Common");
	getAttrValue(top, nodeName, "SIPProtocol", tmp);
	tcdbg_printf("VoIPBasic_Common SIPProtocol is %s\r\n", tmp);
	if(strcmp(tmp,"H.248") == 0)
		curSipStatus = 1;
	else 
		curSipStatus = 0;
	if(prevSipStatus != curSipStatus) {
		prevSipStatus = curSipStatus;
		if(curSipStatus == 0) {		// sip IMS
			//tcdbg_printf(">>>>>sip IMS enable now\r\n");
			system("killall -9 mgapp");
			system("killall -9 tcVoIPApiServer");
			system("/userfs/bin/sipclient &");
			sipStatusFlag = 1;
		} else {		// sip h.248
			//tcdbg_printf(">>>>>sip h.248 enable now\r\n");
			system("killall -9 sipclient");
			system("/userfs/bin/tcVoIPApiServer &");
			system("/userfs/bin/mgapp -id 0 &");
		}
	}
#endif

#if defined(TCSUPPORT_ECN_SIP)
#if defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus() == 0) {
#endif
	memset(InfoVoIP_node_wanset,0,sizeof(InfoVoIP_node_wanset));	
	strcpy(InfoVoIP_node_wanset[0],"InfoVoIP");
	strcpy(InfoVoIP_node_wanset[1],"Common");
	getAttrValue(top, InfoVoIP_node_wanset, "WAN_SET_ENABLE", InfoVoIP_node_tmp);
    
    if (0 == strcasecmp(InfoVoIP_node_tmp, "Yes")){
        tcdbg_printf("[%s]=line:%d=reboot===>\n", __func__, __LINE__);
        for(i=0 ; i<VOIPLineNum ; i++){
            memset(tmpStr, 0, sizeof(tmpStr));
            sprintf(tmpStr,"/userfs/bin/sipclient -d %d",i);
		    system(tmpStr);
        }
			//tcdbg_printf(">>>>>test-1111111111\r\n");
        setAttrValue(top, InfoVoIP_node_wanset, "WAN_SET_ENABLE", "");
        return SUCCESS;
    }
#if defined(TCSUPPORT_ECN_MEGACO)
	}
#endif
#endif


	memset(nodeName,0,sizeof(nodeName));	
	strcpy(nodeName[0],VOIPSPEED);
	strcpy(nodeName[1],"Common");
	setAttrValue(top, nodeName, "MTKSDEnable", "0");
	
	for(i=0 ; i<10 ; i++){
		sprintf(nodeName[1],"Entry%d",i);
		if((getAttrValue(top, nodeName, "SpeedDialNumber", tmpStr) == TCAPI_PROCESS_OK)\
			&& (getAttrValue(top, nodeName, "UserId", tmpStr2) == TCAPI_PROCESS_OK)){
			//tcdbg_printf(">>>>>test-2222222222\r\n");

		strcpy(nodeName[1],"Common");
		setAttrValue(top, nodeName, "MTKSDEnable", "1");
			break;
		}
	}
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPBASIC);

	//sip basic username password
	//updateVoipLineNum(top);
	//tcdbg_printf("%s 1111\n",__func__);
	for(i=0 ; i<VOIPLineNum ; i++){
		sprintf(nodeName[1],"Entry%d",i);
		if(getAttrValue(top, nodeName, "Enable", tmp) == TCAPI_PROCESS_OK){
			memset(tmpStr,0,sizeof(tmpStr));
			if(strcmp(tmp,SIP_YES) == 0){
#if defined(TCSUPPORT_ECN_SIP)
#if defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus() == 0) {
#endif
				strcpy(InfoVoIP_node[0],"InfoVoIP");
				sprintf(InfoVoIP_node[1],"Entry%d",i);
				
		if(getAttrValue(top,InfoVoIP_node,"RegFailReason",tmp)== TCAPI_PROCESS_OK)
		{
		    if(0 != strcmp(tmp,"6")){
			setAttrValue(top, InfoVoIP_node, "Status", "Initializing");
		    }
		}else{
		    setAttrValue(top, InfoVoIP_node, "Status", "Initializing");
		}
				
#if defined(TCSUPPORT_ECN_MEGACO)
	}
#endif
#endif
				
				enablenum++;
/*				sprintf(tmpStr,"/userfs/bin/sipclient -d %d",i);
				system(tmpStr);

				//sleep(1);
				sprintf(tmpStr,"/userfs/bin/sipclient -u %d",i);
				system(tmpStr);
*/
				
			//tcdbg_printf(">>>>>test-4444444444aaaaa, i is %d, enablenum is %d\r\n", i, enablenum);
				//sleep(1);
				/*sipclient -c means dereg + reg*/
				sprintf(tmpStr,"/userfs/bin/sipclient -c %d",i);
				system(tmpStr);
				//sleep(1);
			}
			else{
#if defined(TCSUPPORT_ECN_SIP)
#if defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus() == 0) {
#endif
				strcpy(InfoVoIP_node[0],"InfoVoIP");
				sprintf(InfoVoIP_node[1],"Entry%d",i);
				setAttrValue(top, InfoVoIP_node, "Status", "Disabled");
#if defined(TCSUPPORT_ECN_MEGACO)
	}
#endif
#endif
				/*sipclient -d means dereg*/
				sprintf(tmpStr,"/userfs/bin/sipclient -d %d",i);
				system(tmpStr);
				//sleep(1);
			}
		}
	}
	{
		memset(tmpStr,0,sizeof(tmpStr));
		sprintf(tmpStr,"/userfs/bin/sipclient -r");
		system(tmpStr);
	}
	#if 0
	if(enablenum > 0){
		pre_sys_state.voip_service = 1;
		memset(tmpStr,0,sizeof(tmpStr));
		sprintf(tmpStr,"/userfs/bin/sipclient -r");
		system(tmpStr);
		//sleep(1);
			//tcdbg_printf(">>>>>test-55555555555aaaaa\r\n");
	}
	else{
		sprintf(tmpStr,"/userfs/bin/sipclient -c 102");
		system(tmpStr);
		//sleep(1);
		system("killall -9 sipclient");
		pre_sys_state.voip_service = 0;
		system("/userfs/bin/sipclient &");
			//tcdbg_printf(">>>>>test-5555555555bbbbb\r\n");
	}
	#endif

	parentNode = mxmlFindElement(top, top, DEVICEINFO,
						NULL, NULL, MXML_DESCEND);
	if(parentNode==NULL){
	     parentNode =mxmlNewElement(top,  DEVICEINFO);
			//tcdbg_printf(">>>>>test-6666666666\r\n");
	}
	/*20110527_serena_modify for setting ConnState by line*/
	remove("/etc/sip_status0");
	remove("/etc/sip_status1");
	/*End 20110527_serena_modify for setting ConnState by line*/
/* 20110127 PTChen add for voice net log*/
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPLOG);
	strcpy(nodeName[1],VOIPLOGSUB);
	if(getAttrValue(top, nodeName, "VoiceNetLogEnable", tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp,SIP_YES) == 0){
			if(getAttrValue(top, nodeName, "VoiceNetLogServerIP", tmp) == TCAPI_PROCESS_OK){
				if((fp = fopen(netlogFile,"w"))){
			//tcdbg_printf(">>>>>test-7777777777aaaaa\r\n");
					fprintf(fp,"0x%08lx",inet_addr(tmp));	
					fclose(fp);
				}
			}
		}
		else{
			if((fp = fopen(netlogFile,"w"))){
			//tcdbg_printf(">>>>>test-7777777777bbbbb\r\n");
			fprintf(fp,"0x0"); 
			fclose(fp);
			}
		}
	}
/* 20110127 PTChen add for voice net log end*/
	//tcdbg_printf("%s out\n",__func__);

	
	//system("echo \"0xc0a80111\" >> ./var/voice_netlog");
	//reset connect status	
	return SUCCESS;
}

void voip_put_str(FILE *fp, char *name, char *value){
	char tmp[1024] = {0};
	
	sprintf(tmp,"%s=%s\n",name,value);
	fputs(tmp,fp);
}
int voip_get_payload_id(char *name){
	int ret=0;
	
	if(strcmp(name,"G.711 A-law") == 0){
		ret=8;
	}else if(strcmp(name,"G.711 U-law") == 0){
		ret=0;
	}else if(strcmp(name,"G.729") == 0){
		ret=18;
	}else if(strcmp(name,"G.723") == 0){
		ret=4;
	}else if(strcmp(name,"G.726 - 16") == 0){
		ret=23;
	}else if(strcmp(name,"G.726 - 24") == 0){
		ret=22;
	}else if(strcmp(name,"G.726 - 32") == 0){
		ret=2;
	}else if(strcmp(name,"G.726 - 40") == 0){	
		ret=21;
	}else if(strcmp(name,"G.722") == 0){
		ret = 9;
	}else if(strcmp(name,"AMR") == 0){
		ret = 14;
	}else{
		ret = -1;
 		fprintf(stderr,"unknown codec\n");
	}
	return ret;
}

//PT Chen add 20101028
void
put_advanced(FILE *fp,mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[100];
/*20110112_serena_modify for country setting*/
	static char voipRegionNum[3];
	static int regionSaved = 0;
/*End 20110112_serena_modify for country setting*/

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPADVANCED);
	strcpy(nodeName[1],"Common");
	if(getAttrValue(top, nodeName, "VoIPRegion", tmp) == TCAPI_PROCESS_OK){
/*20110112_serena_modify for country setting*/
		if(regionSaved == 0){
			if(strcmp(tmp,"BEL-Belgium") == 0){
				strncpy(voipRegionNum,"0",sizeof(voipRegionNum));
			}
			else if(strcmp(tmp,"BRA-Brazil") == 0){
				strncpy(voipRegionNum,"1",sizeof(voipRegionNum));
			}
			else if(strcmp(tmp,"CHL-Chile") == 0){
				strncpy(voipRegionNum,"2",sizeof(voipRegionNum));
			}
			else if(strcmp(tmp,"CHN-China") == 0){
				strncpy(voipRegionNum,"3",sizeof(voipRegionNum));
			}
			else if(strcmp(tmp,"DNK-Denmark") == 0){
				strncpy(voipRegionNum,"4",sizeof(voipRegionNum));
			}
			else if(strcmp(tmp,"ESP-Spain") == 0){
				strncpy(voipRegionNum,"5",sizeof(voipRegionNum));
			}
			else if(strcmp(tmp,"ETS-ETSI") == 0){
				strncpy(voipRegionNum,"6",sizeof(voipRegionNum));
			}
			else if(strcmp(tmp,"FIN-Finland") == 0){
				strncpy(voipRegionNum,"7",sizeof(voipRegionNum));
			}
			else if(strcmp(tmp,"FRA-France") == 0){
				strncpy(voipRegionNum,"8",sizeof(voipRegionNum));
			}
			else if(strcmp(tmp,"DEU-Germany") == 0){
				strncpy(voipRegionNum,"9",sizeof(voipRegionNum));
			}
			else if(strcmp(tmp,"HUN-Hungaryy") == 0){
				strncpy(voipRegionNum,"10",sizeof(voipRegionNum));
			}
			else if(strcmp(tmp,"ITA-Italy") == 0){
				strncpy(voipRegionNum,"11",sizeof(voipRegionNum));
			}
			else if(strcmp(tmp,"JPN-Japan") == 0){
				strncpy(voipRegionNum,"12",sizeof(voipRegionNum));
			}
			else if(strcmp(tmp,"NLD-Netherlands") == 0){
				strncpy(voipRegionNum,"13",sizeof(voipRegionNum));
			}
			else if(strcmp(tmp,"USA-United States") == 0){
				strncpy(voipRegionNum,"14",sizeof(voipRegionNum));
			}
			else if(strcmp(tmp,"SWE-Sweden") == 0){
				strncpy(voipRegionNum,"15",sizeof(voipRegionNum));
			}
			else if(strcmp(tmp,"CHE-Switzerland") == 0){
				strncpy(voipRegionNum,"16",sizeof(voipRegionNum));
			}
			else if(strcmp(tmp,"GBR-Great Britain") == 0){
				strncpy(voipRegionNum,"17",sizeof(voipRegionNum));
			}
			regionSaved = 1;
		}
		voip_put_str(fp,"VoIPRegion",voipRegionNum);
/*End 20110112_serena_modify for country setting*/
	}
}

//PT Chen add 20101028 end

/* Enhance by sam 2010/12/20 */
void
put_SIPStackParameters(FILE *fp,mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[100];
	unsigned long int  retransMs = 0;

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPADVANCED);
	strcpy(nodeName[1],"Common");
	
	voip_put_str(fp,"bDisableMerging","1");

	if(getAttrValue(top, nodeName, "RetransInterval", tmp) == TCAPI_PROCESS_OK){
		sscanf(tmp,"%d",&retransMs);
		retransMs = retransMs*1000;
		sprintf(tmp,"%d", retransMs);
		voip_put_str(fp,RETRANSMIT_INTERVAL_T1,tmp);
		voip_put_str(fp,RETRANSMIT_INTERVAL_T2,tmp);
	}else{
		voip_put_str(fp,RETRANSMIT_INTERVAL_T1,"500");
		voip_put_str(fp,RETRANSMIT_INTERVAL_T2,"4000");
	}
	fputs("\n",fp);
}
/* Enhance by sam 2010/12/20  end */	


/* 20101220 PTchen modified for per line setting */
void
put_call_ctrl(FILE *fp,mxml_node_t *top){
	
 	int i;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[100], tmp2[100];

	//fputs("\n[CallControll]\n",fp);
	/*call control feature.*/
	for(i=0 ; i<VOIP_LINE_NUM ; i++){
// 	for(i=0 ; i<1 ; i++){
// 		memset(nodeName,0,sizeof(nodeName));
// 		strcpy(nodeName[0],VOIPBASIC);
// 		sprintf(nodeName[1],"Entry%d",i);
// 		if(getAttrValue(top, nodeName, "SIPEnable", tmp) == TCAPI_PROCESS_OK){
// 			if(strcmp(tmp,SIP_YES) != 0){
// 				continue;
// 			}
// 		}else{
// 			continue;
// 		}
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],VOIPCALLCTRL);
 		sprintf(nodeName[1],"Entry%d",i);
//		strcpy(nodeName[1],"Entry");
/* 20101220 PTchen modified for new feature  */
/* enhance by sam - 20110113 */
/* 1.AnonymousCallBlocking  */
/* 2.AnonymousCalling  */
/* 3.DND */

		sprintf(tmp2,"%s%d",ANONY_CALLBLOCK,i);
		if(getAttrValue(top, nodeName, "AnonymousCallBlock", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
				voip_put_str(fp,tmp2,"1");
			}else{			
				voip_put_str(fp,tmp2,"0");
			}	
		}else{
			voip_put_str(fp,tmp2,"0");
		}


		sprintf(tmp2,"%s%d",ANONY_CALL,i);
		if(getAttrValue(top, nodeName, "AnonymousCall", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
				voip_put_str(fp,tmp2,"1");

			}else{
				voip_put_str(fp,tmp2,"0");
			}	
		}else{
			voip_put_str(fp,tmp2,"0");
		 }



/* 20101220 PTchen modified for new feature end */




/* 20101220 PTchen modified for call feature per line setting*/
		sprintf(tmp2,"CallerIdEnable%d",i);
		if(getAttrValue(top, nodeName, "SIPCallerIdEnable", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
				voip_put_str(fp,tmp2,"1");
			}else{
				voip_put_str(fp,tmp2,"0");
			}	
		}else{
			voip_put_str(fp,tmp2,"0");
		}
		/*if(getAttrValue(top, nodeName, "SIPCallerIdType", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
				voip_put_str(fp,CALLERIDTYPE,"1");
			}else{
				voip_put_str(fp,CALLERIDENABLE,"0");
			}	
		}else{
			voip_put_str(fp,CALLERIDENABLE,"0");
		}*/	
		sprintf(tmp2,"%s%d",CALLWAITINGENABLE,i);		
		if(getAttrValue(top, nodeName, "SIPCallWaitingEnable", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
 				voip_put_str(fp,tmp2,"1");
// 				voip_put_str(fp,CALLWAITINGENABLE,"1");

			}else{
 				voip_put_str(fp,tmp2,"0");
// 				voip_put_str(fp,CALLWAITINGENABLE,"0");
			}	
		}else{
 			voip_put_str(fp,tmp2,"0");
// 			voip_put_str(fp,CALLWAITINGENABLE,"0");
	
		}
		sprintf(tmp2,"SIPCallWaitingReply%d",i);
		if(getAttrValue(top, nodeName, "SIPCallWaitingReply", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,"Ringing") == 0){
				voip_put_str(fp,tmp2,"180");
			}else{
				voip_put_str(fp,tmp2,"182");
			}		
		}else{
			voip_put_str(fp,tmp2,"182");
		}
		if(getAttrValue(top, nodeName, "SIPCallForwardEnable", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp, SIP_YES) == 0){
 				sprintf(tmp2, "%s%d", CALLFORWARDENABLE, i);
 				voip_put_str(fp, tmp2, "1");
			} else {
				sprintf(tmp2, "%s%d", CALLFORWARDENABLE, i);
 				voip_put_str(fp, tmp2, "0");
			}
			getAttrValue(top, nodeName, "SIPCFUNumber", tmp);
			sprintf(tmp2, "%s%d", CFUNUMBER, i);
			voip_put_str(fp, tmp2, tmp);
			getAttrValue(top, nodeName, "SIPCFBNumber", tmp);
			sprintf(tmp2, "%s%d", CFBNUMBER, i);
			voip_put_str(fp, tmp2, tmp);
			getAttrValue(top, nodeName, "SIPCFNRNumber", tmp);
			sprintf(tmp2, "%s%d", CFNRNUMBER, i);
			voip_put_str(fp, tmp2, tmp);
		} else {
 			sprintf(tmp2,"%s%d",CALLFORWARDENABLE,i);
 			voip_put_str(fp,tmp2,"0");
			sprintf(tmp2, "%s%d", CFUNUMBER, i);
			voip_put_str(fp, tmp2, "");
			sprintf(tmp2, "%s%d", CFBNUMBER, i);
			voip_put_str(fp, tmp2, "");
			sprintf(tmp2, "%s%d", CFNRNUMBER, i);
			voip_put_str(fp, tmp2, "");
		}
		if(getAttrValue(top, nodeName, "SIPCallTransfer", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp, SIP_YES) == 0){
				sprintf(tmp2,"%s%d",CALLTRANSFER,i);
 				voip_put_str(fp, tmp2, "1");
			} else {
				sprintf(tmp2,"%s%d",CALLTRANSFER,i);
 				voip_put_str(fp, tmp2, "0");
			}
			getAttrValue(top, nodeName, "SIPBlindTransferNumber", tmp);
			sprintf(tmp2, "%s%d", BLINDTRANSFERNUM, i);
			voip_put_str(fp, tmp2, tmp);
			getAttrValue(top, nodeName, "SIPAttendedTransferNumber", tmp);
			sprintf(tmp2, "%s%d", ATTENDEDTRANSFERNUM, i);
			voip_put_str(fp, tmp2, tmp);
		}else{
			sprintf(tmp2,"%s%d",CALLTRANSFER,i);
 			voip_put_str(fp, tmp2, "0");
			sprintf(tmp2, "%s%d", BLINDTRANSFERNUM, i);
			voip_put_str(fp, tmp2, "");
			sprintf(tmp2, "%s%d", ATTENDEDTRANSFERNUM, i);
			voip_put_str(fp, tmp2, "");
		}
		sprintf(tmp2,"%s%d",CALLHOLD,i);
		if(getAttrValue(top, nodeName, "SIPCallHold", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
 				voip_put_str(fp,tmp2,"1");
//				voip_put_str(fp,CALLHOLD,"1");
			}else{
 				voip_put_str(fp,tmp2,"0");
//				voip_put_str(fp,CALLHOLD,"0");
			}	
		}else{
 			voip_put_str(fp,tmp2,"0");
//			voip_put_str(fp,CALLHOLD,"0");
		}
/* 20110110 PTchen modified for call return*/
		if(getAttrValue(top, nodeName, "SIPCallReturnEnable", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
		sprintf(tmp2,"%s%d",CALLRETURN,i);
		
 				voip_put_str(fp,tmp2,"1");
			}else{
				sprintf(tmp2,"%s%d",CALLRETURN,i);
 				voip_put_str(fp,tmp2,"0");
			}
			getAttrValue(top, nodeName, "SIPCallReturnNumber", tmp);
			sprintf(tmp2, "%s%d", CALLRETURNNUMBER, i);
			voip_put_str(fp, tmp2, tmp);
		}else{
			sprintf(tmp2,"%s%d",CALLRETURN,i);
 			voip_put_str(fp,tmp2,"0");
			sprintf(tmp2, "%s%d", CALLRETURNNUMBER, i);
			voip_put_str(fp, tmp2, "");
		}
		
/* 20110110 PTchen modified for call return end*/
/* 20110124 PTchen modified for DND*/
		if(getAttrValue(top, nodeName, "SIPDNDEnable", tmp) == TCAPI_PROCESS_OK){
					if(strcmp(tmp,SIP_YES) == 0){
				sprintf(tmp2,"%s%d",CALLDNDENABLE,i);
				
						voip_put_str(fp,tmp2,"1");
					}else{
						sprintf(tmp2,"%s%d",CALLDNDENABLE,i);
						voip_put_str(fp,tmp2,"0");
					}
					getAttrValue(top, nodeName, "SIPDNDEnableNumber", tmp);
					sprintf(tmp2, "%s%d", CALLDNDENABLENUMBER, i);
					voip_put_str(fp, tmp2, tmp);
					getAttrValue(top, nodeName, "SIPDNDDisableNumber", tmp);
					sprintf(tmp2, "%s%d", CALLDNDDISABLENUMBER, i);
					voip_put_str(fp, tmp2, tmp);
				}else{
					sprintf(tmp2,"%s%d",CALLDNDENABLE,i);
					voip_put_str(fp,tmp2,"0");
					sprintf(tmp2, "%s%d", CALLDNDDISABLENUMBER, i);
					voip_put_str(fp, tmp2, "");
					sprintf(tmp2, "%s%d", CALLDNDDISABLENUMBER, i);
					voip_put_str(fp, tmp2, "");
				}

		if(getAttrValue(top, nodeName, "SIP3wayConf", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
				sprintf(tmp2,"%s%d",THREEWAYCONF,i);
 				voip_put_str(fp,tmp2,"1");
			}else{
				sprintf(tmp2,"%s%d",THREEWAYCONF,i);
 				voip_put_str(fp,tmp2,"0");
			}
			getAttrValue(top, nodeName, "SIP3wayConfNumber", tmp);
			sprintf(tmp2, "%s%d", THREEWAYCONFNUMBER, i);
			voip_put_str(fp, tmp2, tmp);
		}else{
			sprintf(tmp2,"%s%d",THREEWAYCONF,i);
 			voip_put_str(fp,tmp2,"0");
			sprintf(tmp2, "%s%d", THREEWAYCONFNUMBER, i);
			voip_put_str(fp, tmp2, "");
		}
		sprintf(tmp2,"%s%d",MWIENABLE,i);
		if(getAttrValue(top, nodeName, "SIPMWIEnable", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
 				voip_put_str(fp,tmp2,"1");
//				voip_put_str(fp,MWIENABLE,"1");
			}else{
 				voip_put_str(fp,tmp2,"0");
//				voip_put_str(fp,MWIENABLE,"0");
			}	
		}else{
 			voip_put_str(fp,tmp2,"0");
//			voip_put_str(fp,MWIENABLE,"0");
		}
		/* 20110321 Pork add for DOD */
		sprintf(tmp2,"%s%d",CALLDODENABLE,i);
		if(getAttrValue(top, nodeName, "DODEnable", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
 				voip_put_str(fp,tmp2,"1");
			}else{
 				voip_put_str(fp,tmp2,"0");
			}
			getAttrValue(top, nodeName, "DODNumber", tmp);
			sprintf(tmp2, "%s%d", CALLDODNUMBER, i);
			voip_put_str(fp, tmp2, tmp);
		}else{
 			voip_put_str(fp,tmp2,"0");
		}

}
}
/* 20101220 PTchen modified for call feature per line setting end */
void
put_media(FILE *fp,mxml_node_t *top,char *local_addr,char *local_rtp_port){
/*  PT Chen add 20101101 
In case to move FAX and DTMF two parameters from Media item to Advanced item for WEBpage and node information tree, we must adjust thoes related variables to associate with "nodeName_advanced" to get correct value. Therefor, it can be shown as before that those varibles would placed at same location in "SIPPhone.cfg" file.


PT Chen add 20101101  end
*/


/* 20101220 PTchen modified for media per line setting*/
	int i, m, id;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[100], tmp2[100], tmp3[100];
	char nodeName_advanced[MAX_ARG_NUM][MAX_NODE_NAME];
	char nodeName_entry[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPMEDIA);	
	strcpy(nodeName[1],STRING_COMMON);
	memset(nodeName_entry,0,sizeof(nodeName_entry));
	strcpy(nodeName_entry[0],VOIPMEDIA);	
	
	
	//PT Chen modify 20101028
	memset(nodeName_advanced,0,sizeof(nodeName_advanced));
	strcpy(nodeName_advanced[0],VOIPADVANCED);	
	strcpy(nodeName_advanced[1],STRING_COMMON);
	//PT Chen modify 20101028
	
/* 20101220 PTchen modified VAD, Silence Compression per line setting*/
	updateVoipLineNum(top);
	for(i=0 ; i<VOIPLineNum ; i++){

		sprintf(nodeName_entry[1],"Entry%d",i);
		
		sprintf(tmp2,"%s%d","VAD",i);
		if((getAttrValue(top, nodeName_entry, "VAD", tmp) == TCAPI_PROCESS_OK) && (strcmp(tmp,"Yes") == 0)){
			voip_put_str(fp,tmp2,"1");
		}else{
			voip_put_str(fp,tmp2,"0");
		}
/* serena_modify */	
		sprintf(tmp2,"%s%d","SilenceCompressionEnable",i);
		if((getAttrValue(top, nodeName_entry, "SilenceCompressionEnable", tmp) == TCAPI_PROCESS_OK) && (strcmp(tmp,"Yes") == 0)){

			voip_put_str(fp,tmp2,"1");
		}else{
			voip_put_str(fp,tmp2,"0");
		}
	}
/* 20101220 PTchen EchoCancell not per line setting*/
	if((getAttrValue(top, nodeName, "EchoCancellationEnable", tmp) == TCAPI_PROCESS_OK) && (strcmp(tmp,"Yes") == 0)){
		voip_put_str(fp,"EchoCancellationEnable","1");
	}else{
		voip_put_str(fp,"EchoCancellationEnable","0");
	}	

	//PT Chen modify 20101028
	if((getAttrValue(top, nodeName_advanced, "T38Enable", tmp) == TCAPI_PROCESS_OK) && (strcmp(tmp,"Yes") == 0)){
		voip_put_str(fp,"T38Enable","1");
	}else{
		voip_put_str(fp,"T38Enable","0");
	}

	//PT Chen modify 20101028
	
/*20100821 serena_modify: add FaxCodec*/  //PT Chen modify 20101028
	if((getAttrValue(top, nodeName_advanced, "FaxCodec", tmp) == TCAPI_PROCESS_OK) && (strcmp(tmp,"T38") == 0)){
//		tcdbg_printf("%s:T38",__func__);
  		voip_put_str(fp,"FaxCodec","24");
	}else if(strcmp(tmp,"G.711 A-law") == 0){
// 		tcdbg_printf("%s:G.711 A-law",__func__);
		voip_put_str(fp,"FaxCodec","8");
	}else{ //"G.711 U-law"
// 		tcdbg_printf("%s:G.711 U-law",__func__);
		voip_put_str(fp,"FaxCodec","0");
	}	

/*20100821 end_serena_modify*/  //PT Chen modify 20101028
	if((getAttrValue(top, nodeName_advanced, "DTMFTransportMode", tmp) == TCAPI_PROCESS_OK) && (strcmp(tmp,"Inband") == 0)){
		voip_put_str(fp,"DTMFTransportMode","0");
	}else if(strcmp(tmp,"RFC2833") == 0){
		voip_put_str(fp,"DTMFTransportMode","1");
	}else{
		voip_put_str(fp,"DTMFTransportMode","2");
	}
/* serena_modify end */   //PT Chen modify 20101028 end
	
/* 20101220 PTchen modified codec per line setting */

for(m=0 ; m<VOIPLineNum ; m++){

/* 20101220 PTchen modified merge sprintf() and fputs() to fprintf()*/
	fprintf(fp,"\n[MediaParameters%d]\n",m );

//	sprintf(tmp2,"\n[MediaParameters%d]\n",m);

//	fputs(tmp2,fp);	
	
	voip_put_str(fp,"v","0");
/* 20101220 PTchen modified merge sprintf() and fputs() to fprintf()*/
	fprintf(fp, "o= rtp/1 0 0 IN IP4 %s\n",local_addr );
	//sprintf(tmp2, "o= rtp/1 0 0 IN IP4 %s\n",local_addr);
	//fputs(tmp2,fp);
	voip_put_str(fp,"s","-");
	//sprintf(tmp2, "c= IN IP4 %s\n",local_addr);
	//fputs(tmp2,fp);
	voip_put_str(fp,"t","0 0");
	sprintf(tmp2, "m=audio %s RTP/AVP",local_rtp_port);

	//memset(nodeName,0,sizeof(nodeName));
	//strcpy(nodeName[0],VOIPMEDIA);
	sprintf(nodeName[1],"Entry%d",m);
	for(i=0; i<MEDIAENTRY_NUM ; i++){
		sprintf(tmp3,"SIPSupportedCodecs%d",i);
		if((getAttrValue(top, nodeName, tmp3, tmp) == TCAPI_PROCESS_OK)&&(strcmp(tmp,"N/A") != 0)){
			id = voip_get_payload_id(tmp);
			if(id != -1){
				sprintf(tmp," %d",id);
				strcat(tmp2,tmp);
			}else{
				if(i == 0)
					strcat(tmp2," 0");
			}		
		}else{
			//default entry 0 use G.711 a-law
			if(i == 0)
				strcat(tmp2," 0");
		}
	}
/*jrchen modify for DTMF 20100421*/	//PT Chen modify 20101028
	if(getAttrValue(top, nodeName_advanced, "DTMFTransportMode", tmp) == TCAPI_PROCESS_OK){
		if((strcmp(tmp, "RFC2833") == 0) || (strcmp(tmp, "Via SIP") == 0)){//support RFC2833			
			strcat(tmp2, " 97");	/*20101119serena_modify for rfc2833 payload*/
		}
	}
/* jrchen modify end */


	strcat(tmp2,"\n");
	fputs(tmp2,fp);
	//PT Chen modify 20101028 end
	for(i=0; i<MEDIAENTRY_NUM ; i++){
		sprintf(tmp3,"SIPSupportedCodecs%d",i);
		if((getAttrValue(top, nodeName, tmp3, tmp) == TCAPI_PROCESS_OK)&&(strcmp(tmp,"N/A") != 0)){
			id = voip_get_payload_id(tmp);
			sprintf(tmp2,"a=rtpmap:%d ",id);
			switch(id) {
				case 0:
					strcat(tmp2,"PCMU/8000\n");
					break;
				case 2:
					strcat(tmp2,"G726_32/8000\n");
					break;					
				case 4:
					strcat(tmp2,"G723/8000\n");
					break;
				case 8:
					strcat(tmp2,"PCMA/8000\n");
					break;
				case 9:
					strcat(tmp2,"G722/8000\n");
					break;	
				case 18:
					strcat(tmp2,"G729/8000\n");
					break;
				case 21:
					strcat(tmp2,"G726_40/8000\n");
					break;
				case 22:
					strcat(tmp2,"G726_24/8000\n");
					break;
				case 23:
					strcat(tmp2,"G726_16/8000\n");
					break;
				default:
					if(i == 0){
						strcpy(tmp2,"a=rtpmap:0 PCMU/8000\n");
					}
					break;	
			}
			fputs(tmp2,fp);	
		}else{
			//default entry 0 use G.711 a-law
			if(i == 0){
				fputs("a=rtpmap:0 PCMU/8000\n",fp);
			}
		}
		
		strcpy(tmp2,"a=ptime:");
		sprintf(tmp3,"SIPPacketizationTime%d",i);
		if((getAttrValue(top, nodeName, tmp3, tmp) == TCAPI_PROCESS_OK)&&(strcmp(tmp,"N/A") != 0)){
			strcat(tmp2,tmp);
			strcat(tmp2,"\n");
			fputs(tmp2,fp);
		}else{
			//default entry 0 use G.711 a-law
			if(i == 0){
				strcat(tmp2,"20\n");
				fputs(tmp2,fp);
			}
		}
	}

/* jrchen modify for DTMF 20100421 */  //PT Chen modify 20101028
	if(getAttrValue(top, nodeName_advanced, "DTMFTransportMode", tmp) == TCAPI_PROCESS_OK){
		if((strcmp(tmp, "RFC2833") == 0) || (strcmp(tmp, "Via SIP") == 0)){			
/* 20101220 PTchen modified merge sprintf() and fputs() to fprintf()*/
			fprintf(fp, "a=rtpmap:97 telephone-event/8000\n" );

//			strcpy(tmp2,"a=rtpmap:97 telephone-event/8000\n");	/*20101115_serena_modify for rfc2833 payload*/
//			fputs(tmp2,fp);
/* 20101220 PTchen modified merge sprintf() and fputs() to fprintf()*/
			fprintf(fp, "a=fmtp:97 0-15\n" );
//			strcpy(tmp2,"a=fmtp:97 0-15\n");	/*20101119_serena_modify for rfc2833 payload*/
//			fputs(tmp2,fp);
		}
	}
/* jrchen modify end */  //PT Chen modify 20101028 end
}

	
for(m=0 ; m<VOIPLineNum ; m++){
/* 20101220 PTchen modified merge sprintf() and fputs() to fprintf()*/
	fprintf(fp, "\n[MediaParametersFull%d]\n",m );
//	sprintf(tmp2,"\n[MediaParametersFull%d]\n",m);

//	fputs(tmp2,fp);	
	
	voip_put_str(fp,"v","0");
/* 20101220 PTchen modified merge sprintf() and fputs() to fprintf()*/
	fprintf(fp, "o= rtp/1 0 0 IN IP4 %s\n",local_addr);
//	sprintf(tmp2, "o= rtp/1 0 0 IN IP4 %s\n",local_addr);
//	fputs(tmp2,fp);
	voip_put_str(fp,"s","-");
	//sprintf(tmp2, "c= IN IP4 %s\n",local_addr);
	//fputs(tmp2,fp);
	voip_put_str(fp,"t","0 0");
	sprintf(tmp2, "m=audio %s RTP/AVP",local_rtp_port);

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPMEDIA);
	for(i=0; i<MEDIAENTRY_NUM ; i++){
		sprintf(nodeName[1],"Entry%d",m);
		sprintf(tmp3,"SIPSupportedCodecs%d",i);
		if((getAttrValue(top, nodeName, tmp3, tmp) == TCAPI_PROCESS_OK)&&(strcmp(tmp,"N/A") != 0)){
			id = voip_get_payload_id(tmp);
			if(id != -1){
				sprintf(tmp," %d",id);
				strcat(tmp2,tmp);
			}else{
				if(i == 0)
					strcat(tmp2," 0");
			}		
		}else{
			//default entry 0 use G.711 a-law
			if(i == 0)
				strcat(tmp2," 0");
		}
	}
/* jrchen modify for DTMF 20100421 */  //PT Chen modify 20101028
	if(getAttrValue(top, nodeName_advanced, "DTMFTransportMode", tmp) == TCAPI_PROCESS_OK){
		if((strcmp(tmp, "RFC2833") == 0) || (strcmp(tmp, "Via SIP") == 0)){//support RFC2833	
			strcat(tmp2, " 97");	/*20101119_serena_modify for rfc2833 payload*/
		}
	}
/* jrchen modify end */
	
	strcat(tmp2,"\n");
	fputs(tmp2,fp);
	//PT Chen modify 20101028  end
	for(i=0; i<MEDIAENTRY_NUM ; i++){
		sprintf(nodeName[1],"Entry%d",m);
		sprintf(tmp3,"SIPSupportedCodecs%d",i);
		if((getAttrValue(top, nodeName, tmp3, tmp) == TCAPI_PROCESS_OK)&&(strcmp(tmp,"N/A") != 0)){
			id = voip_get_payload_id(tmp);
			sprintf(tmp2,"a=rtpmap:%d ",id);
			switch(id) {
				case 0:
					strcat(tmp2,"PCMU/8000\n");
					break;
				case 2:
					strcat(tmp2,"G726_32/8000\n");
					break;
				case 4:
					strcat(tmp2,"G723/8000\n");
					break;
				case 8:
					strcat(tmp2,"PCMA/8000\n");
					break;	
				case 9:
					strcat(tmp2,"G722/8000\n");
					break;
				case 18:
					strcat(tmp2,"G729/8000\n");
					break;
				case 21:
					strcat(tmp2,"G726_40/8000\n");
					break;
				case 22:
					strcat(tmp2,"G726_24/8000\n");
					break;
				case 23:
					strcat(tmp2,"G726_16/8000\n");
					break;
				default:
					if(i == 0){
						strcpy(tmp2,"a=rtpmap:0 PCMU/8000\n");
					}
					break;	
			}
			fputs(tmp2,fp);	
		}else{
			//default entry 0 use G.711 a-law
			if(i == 0){
				fputs("a=rtpmap:0 PCMU/8000\n",fp);
			}
		}
			
		
		strcpy(tmp2,"a=ptime:");
		sprintf(tmp3,"SIPPacketizationTime%d",i);
		if((getAttrValue(top, nodeName, tmp3, tmp) == TCAPI_PROCESS_OK)&&(strcmp(tmp,"N/A") != 0)){
			strcat(tmp2,tmp);
			strcat(tmp2,"\n");
			fputs(tmp2,fp);
		}else{
			//default entry 0 use G.711 a-law
			if(i == 0){
				strcat(tmp2,"20\n");
				fputs(tmp2,fp);
			}
		}
	}



/* jrchen modify for DTMF 20100421 */   //PT Chen modify 20101028
	if(getAttrValue(top, nodeName_advanced, "DTMFTransportMode", tmp) == TCAPI_PROCESS_OK){
		if((strcmp(tmp, "RFC2833") == 0) || (strcmp(tmp, "Via SIP") == 0)){			
			strcpy(tmp2,"a=rtpmap:97 telephone-event/8000\n");	/*20101115_serena_modify for rfc2833 payload*/
			fputs(tmp2,fp);
			strcpy(tmp2,"a=fmtp:97 0-15\n");	/*20101119_serena_modify for rfc2833 payload*/
			fputs(tmp2,fp);		
		}
	}
/* jrchen modify end */   //PT Chen modify 20101028  end

}
/* 20101220 PTchen modified codec per line setting  end */
}

void
put_phone_number(FILE *fp,mxml_node_t *top){
	int i;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME],nodeName2[MAX_ARG_NUM][MAX_NODE_NAME], tmp[100], tmp2[100];
	
	fputs("\n[PhoneNumbers]\n",fp);
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPSPEED);

	for(i=0; i<MAXSPEED_NUM ; i++){
		sprintf(nodeName[1],"Entry%d",i);
		if(getAttrValue(top, nodeName, "Destination", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,"Proxy") == 0){
				if(getAttrValue(top, nodeName, "SpeedDialNumber", tmp2) == TCAPI_PROCESS_OK){
					getAttrValue(top, nodeName, "UserId", tmp);
					strcat(tmp2," ");
					strcat(tmp2,tmp);
					
					memset(nodeName2,0,sizeof(nodeName2));
					strcpy(nodeName2[0],VOIPBASIC);
					strcpy(nodeName2[1],"Common");
	
					if(getAttrValue(top, nodeName2, "SIPProxyEnable", tmp) == TCAPI_PROCESS_OK){
						if(strcmp(tmp,SIP_YES) == 0){
							getAttrValue(top, nodeName2, "SIPProxyAddr", tmp);
							strcat(tmp2,"@");
							strcat(tmp2,tmp);
							getAttrValue(top, nodeName2, "SIPProxyPort", tmp);
							strcat(tmp2,":");
							strcat(tmp2,tmp);
							strcat(tmp2,"\n");
							fputs(tmp2,fp);
							continue;	
						}
					}
					if(getAttrValue(top, nodeName, "SIPOutboundProxyEnable", tmp) == TCAPI_PROCESS_OK){
						if(strcmp(tmp,SIP_YES) == 0){
							getAttrValue(top, nodeName2, "SIPOutboundProxyAddr", tmp);
							strcat(tmp2,"@");
							strcat(tmp2,tmp);
							getAttrValue(top, nodeName2, "SIPOutboundProxyPort", tmp);
							strcat(tmp2,":");
							strcat(tmp2,tmp);
							strcat(tmp2,"\n");
							fputs(tmp2,fp);
							continue;		
						}	
					}
				}
			}else if(strcmp(tmp,"Local Line") == 0){
				if(getAttrValue(top, nodeName, "SpeedDialNumber", tmp2) == TCAPI_PROCESS_OK){
					tcdbg_printf("Local Line\n");
				}
			}else if(strcmp(tmp,"Direct Call") == 0){
				if(getAttrValue(top, nodeName, "SpeedDialNumber", tmp2) == TCAPI_PROCESS_OK){
					getAttrValue(top, nodeName, "UserId", tmp);
					strcat(tmp2," ");
					strcat(tmp2,tmp);
					getAttrValue(top, nodeName, "IpAddressOrHostName", tmp);
					strcat(tmp2,"@");
					strcat(tmp2,tmp);
					getAttrValue(top, nodeName, "Port", tmp);
					strcat(tmp2,":");
					strcat(tmp2,tmp);
					strcat(tmp2,"\n");
					fputs(tmp2,fp);
				}
			}else{
				//no support 
			}	
		}
	}
}
void
put_basic(FILE *fp,mxml_node_t *top){
	int i;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[100], tmp2[100];
	char nodeName_advanced[MAX_ARG_NUM][MAX_NODE_NAME];
	
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPBASIC);
	//PT Chen modify 20101028
	memset(nodeName_advanced,0,sizeof(nodeName_advanced));
	strcpy(nodeName_advanced[0],VOIPADVANCED);
	//PT Chen modify 20101028 end
		//sip basic username password
	updateVoipLineNum(top);
	for(i=0 ; i<VOIPLineNum ; i++){
		sprintf(nodeName[1],"Entry%d",i);
		sprintf(nodeName_advanced[1],"Entry%d",i);
		if(getAttrValue(top, nodeName, "SIPEnable", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
				sprintf(tmp2,"LineEnable%d",i);
				voip_put_str(fp,tmp2,tmp);
				sprintf(tmp2,"termId%d",i);
				if(getAttrValue(top, nodeName, "SIPDisplayName", tmp) == TCAPI_PROCESS_OK){
					voip_put_str(fp,tmp2,tmp);
				}else{
					voip_put_str(fp,tmp2,"");
				}
				sprintf(tmp2,"username%d",i);
				if(getAttrValue(top, nodeName, "SIPAuthenticationName", tmp) == TCAPI_PROCESS_OK){
					voip_put_str(fp,tmp2,tmp);
				}else{
					voip_put_str(fp,tmp2,"");
				}
				sprintf(tmp2,"password%d",i);
				if(getAttrValue(top, nodeName, "SIPPassword", tmp) == TCAPI_PROCESS_OK){
					voip_put_str(fp,tmp2,tmp);
				}else{
					voip_put_str(fp,tmp2,"");
				}
/* serena_modify */
/*20100821 serena_modify: add voiceVolumeTx/Rx*/    //PT Chen modify 20101028
				
			}else{
				sprintf(tmp2,"LineEnable%d",i);
				voip_put_str(fp,tmp2,"No");
				sprintf(tmp2,"termId%d",i);
				voip_put_str(fp,tmp2,"");
				sprintf(tmp2,"username%d",i);
				voip_put_str(fp,tmp2,"");
				sprintf(tmp2,"password%d",i);
				voip_put_str(fp,tmp2,"");
			}	
/*20110104_serena_modify*/
			sprintf(tmp2,"voiceVolumeListen%d",i);				
			if(getAttrValue(top, nodeName_advanced, "VoiceVolumeListen", tmp) == TCAPI_PROCESS_OK){
/*End 20110104_serena_modify*/
				voip_put_str(fp,tmp2,tmp);
			}else{
				voip_put_str(fp,tmp2,"0");
			}
/*20110104_serena_modify*/
			sprintf(tmp2,"voiceVolumeSpeak%d",i);				
			if(getAttrValue(top, nodeName_advanced, "VoiceVolumeSpeak", tmp) == TCAPI_PROCESS_OK){
/*End 20110104_serena_modify*/
				voip_put_str(fp,tmp2,tmp);
			}else{
				voip_put_str(fp,tmp2,"0");
			}		
			/* serena_modify end*/				//PT Chen modify 20101028 end		
		}
	}
	/* Enhance SIP registration expire by sam 2010/12/20 */
	strcpy(nodeName_advanced[1],"Common");
		if(getAttrValue(top, nodeName_advanced, "RegistrationExpire", tmp) == TCAPI_PROCESS_OK){
		voip_put_str(fp,REGISTRATION_EXPIRE,tmp);
	}else{
		voip_put_str(fp,REGISTRATION_EXPIRE,"3600");
	}
	/* Enhance SIP registration expire by sam 2010/12/20  end */

	//voip basic common parameter
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPBASIC);
	strcpy(nodeName[1],"Common");
	
	if(getAttrValue(top, nodeName, "SIPProxyEnable", tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp,SIP_YES) == 0){
			if(getAttrValue(top, nodeName, "SIPProxyAddr", tmp) == TCAPI_PROCESS_OK){
				voip_put_str(fp,REGISTRARADDR,tmp);
			}else{
				voip_put_str(fp,REGISTRARADDR,"");
			}
			if(getAttrValue(top, nodeName, "SIPProxyPort", tmp) == TCAPI_PROCESS_OK){
				voip_put_str(fp,REGISTRARPORT,tmp);
			}else{
/* serena_modify */
				voip_put_str(fp,REGISTRARPORT,"5060");
/* serena_modify end */				
			}
		}else{
			voip_put_str(fp,REGISTRARADDR,"");
			voip_put_str(fp,REGISTRARPORT,"");
		}
	}
	if(getAttrValue(top, nodeName, "SIPOutboundProxyEnable", tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp,SIP_YES) == 0){
			if(getAttrValue(top, nodeName, "SIPOutboundProxyAddr", tmp) == TCAPI_PROCESS_OK){
				voip_put_str(fp,OUTBOUNDPROXYADDR,tmp);
			}else{
				voip_put_str(fp,OUTBOUNDPROXYADDR,"");
			}
			if(getAttrValue(top, nodeName, "SIPOutboundProxyPort", tmp) == TCAPI_PROCESS_OK){
				voip_put_str(fp,OUTBOUNDPROXYPORT,tmp);
			}else{
				voip_put_str(fp,OUTBOUNDPROXYPORT,"");
			}
		}else{
			voip_put_str(fp,OUTBOUNDPROXYADDR,"");
			voip_put_str(fp,OUTBOUNDPROXYPORT,"");
		}
	}
	
	
	if(getAttrValue(top, nodeName, "SIPTransportProtocol", tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp,"UDP") == 0){
			voip_put_str(fp,SIPTRANSPORTTYPE,"0");
			voip_put_str(fp,SIPTCPENABLED,"0");
		}else{//TCP
			voip_put_str(fp,SIPTRANSPORTTYPE,"1");
			voip_put_str(fp,SIPTCPENABLED,"1");
		}
	}else{
		voip_put_str(fp,SIPTRANSPORTTYPE,"0");
		voip_put_str(fp,SIPTCPENABLED,"1");
	}
	
	if(getAttrValue(top, nodeName, "LocalSIPPort", tmp) == TCAPI_PROCESS_OK){
		voip_put_str(fp,SIPTCPPORT,tmp);
		voip_put_str(fp,SIPUDPPORT,tmp);
	}else{
		voip_put_str(fp,SIPTCPPORT,"5060");
		voip_put_str(fp,SIPUDPPORT,"5060");
	}
	
	if(getAttrValue(top, nodeName, PRACKENABLE, tmp) == TCAPI_PROCESS_OK){
		if(strcmp(tmp,SIP_YES) == 0){
			voip_put_str(fp,PRACKENABLE,"1");
		}else{
			voip_put_str(fp,PRACKENABLE,"0");
		}	
	}else{
		voip_put_str(fp,PRACKENABLE,"0");
	}

}
/*______________________________________________________________________________
**	voip_supportedCodec_init
**
**	descriptions:
**		the init function of voip supported codec.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
voip_codec_init(void){

	cfg_node_t node={
		.name=VOIPCODECS,
		.type=PVC_TYPE|ENTRY_TYPE|MAXCODEC_NUM,
		.adv_table=NULL,
 		.merge_table=NULL,
		.cfg_read=NULL,
		.cfg_write=NULL,
		.cfg_verify=NULL,
		.cfg_execute=NULL,
		.cfg_boot=NULL,
	};

	return register_cfg_node(&node);
}
/* jrchen add 20100222 */
/*_______________________________________________________________________
**	function name: voip_log_init
**
**	descriptions:
**		the init function of VoIPLog.
**	parameters:
**		none
**	return:
**		SUCCESS: 	0
**		FAIL: -1
**	call:
**		register_cfg_node
**
**      revision:
**      1. jrchen 2010/02/23
**_______________________________________________________________________
*/
int 
voip_log_init(void){
	
	cfg_node_t node = {
		.name = VOIPLOG,
//		.type = SINGLE_TYPE | WEB_TYPE,
		.type = SINGLE_TYPE,
		.adv_table = NULL,
 		.merge_table = NULL,
//		.cfg_read = voip_log_read,
		.cfg_read = NULL,
		.cfg_write = voip_log_write,
		.cfg_verify = NULL,
		.cfg_execute = voip_log_execute,
		.cfg_boot = NULL,
	};

	return register_cfg_node(&node);
}


/*____________________________________________________________________
**      function name: voip_log_execute
**      descriptions:
**            Start VoIP Log to UDP or/and Console.
**
**      parameters:
**            top:   top of cfg node tree
**            name: cfg node name.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**			getAttrValue
**
**      revision:
**      1. Jrchen 2010/2/23
**____________________________________________________________________
*/
int
voip_log_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	
	return voip_real_execute(top, name);

}

/*___________________________________________________________________________
**	voip_log_write
**
**	descriptions:
**		Find voip_log element from mxml tree then write voip_log's system 
		parameter to specify configuration files.
**	parameters:
**		top: 	Specify the root of mxml_node_t structure.
**		parant: Specify the parant node of voip_basic cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
voip_log_write(mxml_node_t *top, mxml_node_t *parant){	
	return voip_real_write(top);
}



/*_______________________________________________________________________
**	function name: put_log
**
**	descriptions:
**		internal function called by voip_real_write, to write log setting into SIPPhone.cfg 
**	parameters:
**		fp: file to write
**		name: top of cfg node tree
**	global:
		none
**	return:
**		none
**	call:
**		getAttrValue
**		
**      revision:
**      1. jrchen 2010/02/23
**_______________________________________________________________________
*/

void
put_log(FILE *fp, mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[18];
	int i = 0;
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPLOG);
	strcpy(nodeName[1],VOIPLOGSUB);

	for(i = 0; strlen(log_default_setting[i].attr) != 0; i++){
		if(getAttrValue(top, nodeName, log_default_setting[i].attr, tmp) == TCAPI_PROCESS_OK){
			voip_put_str(fp, log_default_setting[i].attr, tmp);			
		}
		else{
			voip_put_str(fp, log_default_setting[i].attr, log_default_setting[i].value);
		}
	}
}

/*______________________________________________________________________________
**	voip_digitmap_init
**
**	descriptions:
**		the init function of voip numbering plan. 
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
voip_digitmap_init(void){

	cfg_node_t node={
		.name=VOIPDIGITMAP,
		.type=SINGLE_TYPE,
		.adv_table=NULL,
 		.merge_table=NULL,
		.cfg_read=NULL,
		.cfg_write=voip_digitmap_write,
		.cfg_verify=voip_digitmap_verify,
		.cfg_execute=voip_digitmap_execute,
		.cfg_boot=NULL,
	};

	return register_cfg_node(&node);
}
/*______________________________________________________________________________
**	voip_digitmap_write
**
**	descriptions:
**		Find voip_digitmap element from mxml tree then write voip_digitmap's system 
		parameter to specify configuration files.
**	parameters:
**		top: 	Specify the root of mxml_node_t structure.
**		parant: Specify the parant node of voip_digitmap cfg node.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
voip_digitmap_write(mxml_node_t *top, mxml_node_t *parant){

	return voip_real_write(top);
}
/*______________________________________________________________________________
**	voip_digitmap_verify
**
**	descriptions:
**		To check the system parameter of voip_digitmap element is validly or not.
**
**	parameters:
**		node: 		Specify the root of mxml_node_t structure.
**
**	return:
**		Success: 	0
**		Otherwise: 	-1
**____________________________________________________________________________
*/
int
voip_digitmap_verify(mxml_node_t *node){

	return SUCCESS;
}
/*_____________________________________________________________________________
**      function name: voip_digitmap_execute
**      descriptions:
**            To execute voip_digitmap node service.
**
**      parameters:
**            node:   Put the voip_digitmap of cfg node tree that is used to execute VoIP service.
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**
**      revision:
**____________________________________________________________________________
*/
int
voip_digitmap_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){

	return voip_real_execute(top,name);
}

/*_____________________________________________________________________________
**      function name: put_digitmap
**      descriptions:
**            Write digitMap context to configuration file
**
**      parameters:
**            fp:   file pointer of configuration file
**            top: config node tree
**
**      global:
**             None
**
**      return:
**            
**
**      call:
**
**      revision:
**____________________________________________________________________________
*/
void
put_digitmap(FILE *fp, mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char *tmp = NULL;
	int i = 0;

	tmp = (char *)malloc(1024);
	if(tmp == NULL) {
		printf("%s(): Error!! Allocate memory failed.\n", __FUNCTION__);
		return;
	}
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], VOIPDIGITMAP);
	strcpy(nodeName[1], VOIPENTRY);

	for(i = 0; strlen(digitmap_default_setting[i].attr) != 0; i++){
		if(getAttrValue(top, nodeName, digitmap_default_setting[i].attr, tmp) == TCAPI_PROCESS_OK) {
			voip_put_str(fp, digitmap_default_setting[i].attr, tmp);			
		} else {
			voip_put_str(fp, digitmap_default_setting[i].attr, digitmap_default_setting[i].value);
		}
	}

	if(tmp != NULL) {
		free(tmp);
	}
	
	return;
}

int
voip_test_init(void){

	cfg_node_t node={
		.name=VOIPTEST,
		.type=ENTRY_TYPE|VOIP_LINE_NUM,
		.adv_table=NULL,
		.merge_table=NULL,
		.cfg_read=NULL, 
		.cfg_write=voip_test_write,
		.cfg_verify=NULL,
		.cfg_execute=voip_test_execute,
		.cfg_boot=voip_test_boot,
	};

	return register_cfg_node(&node);
}

/* mtk06404 add 20140102*/
static int line_test_write(mxml_node_t *top, mxml_node_t *parant)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmpStr[64],tmpStr2[64],tmpStr3[64],testCmd[64];
	int i;
	char* lineProcFile = LINE_TEST_PROC_NAME;
	memset(nodeName,0,sizeof(nodeName));	
	strcpy(nodeName[0],VOIPTEST);

	updateVoipLineNum(top);
	for(i=0 ; i<VOIPLineNum ; i++){
		sprintf(nodeName[1],"Entry%d",i);
		memset(tmpStr, 0, sizeof(tmpStr));
		memset(tmpStr2, 0, sizeof(tmpStr2));	
		memset(tmpStr3, 0, sizeof(tmpStr3));	
		if((getAttrValue(top, nodeName, "TestState", tmpStr) == TCAPI_PROCESS_OK)\
			&& (getAttrValue(top, nodeName, "TestSelector", tmpStr2) == TCAPI_PROCESS_OK)\
			&& (getAttrValue(top, nodeName, "IsTestOnBusy", tmpStr3) == TCAPI_PROCESS_OK)\
			&& (!strcmp(tmpStr,LINE_TEST_REQ))&&(!strcmp(tmpStr3,"0"))){
			
			if(!strcmp(tmpStr2,LINE_TEST_PHONE_CONN)){
				memset(testCmd, 0, sizeof(testCmd));
				sprintf(testCmd,"echo \"%d 3\" > %s",i,lineProcFile);
				setAttrValue(top, nodeName, "IsTestOnBusy", "1");
				setAttrValue(top, nodeName, "TestState", "None");
			}
			/* Line Voltage Test */
			else if(!strcmp(tmpStr2,LINE_TEST_TID_LINE_V)){
				memset(testCmd, 0, sizeof(testCmd));
				sprintf(testCmd,"echo \"%d 1\" > %s",i,lineProcFile);
				setAttrValue(top, nodeName, "IsTestOnBusy", "1");
				setAttrValue(top, nodeName, "TestState", "None");
			}
			/* Resistive Fault */
			else if(!strcmp(tmpStr2,LINE_TEST_TID_RES_FLT)){
				memset(testCmd, 0, sizeof(testCmd));
				sprintf(testCmd,"echo \"%d 4\" > %s",i,lineProcFile);
				setAttrValue(top, nodeName, "IsTestOnBusy", "1");
				setAttrValue(top, nodeName, "TestState", "None");
			}
			else
				return FAIL;
			
			system(testCmd);				

		}
	}
	
    return SUCCESS;
}
#if defined(TCSUPPORT_ECN_MEGACO)
void getH248PhysicalTermIDValue( mxml_node_t *top,int index, char *termName)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(nodeName, 0, sizeof(nodeName));
	sprintf(nodeName[0],"%s",VOIPH248);
	sprintf(nodeName[1],"Entry%d",index);
	if(getAttrValue(top, nodeName, PHYSICAL_TERMID, termName)!= TCAPI_PROCESS_OK)
		tcdbg_printf("%s get attr failed\n",__func__);		
}
#endif
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
static int line_simulate_type_deal(int i, mxml_node_t *top, mxml_node_t *parant)
{
	char testCmd[MAX_CMD_SIZE] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tmpStr[MAX_VALUE_SIZE]  = {0};
	char tmpStr1[MAX_VALUE_SIZE]  = {0};
	char tmpStr2[MAX_VALUE_SIZE]  = {0};
	char tmpStr3[MAX_VALUE_SIZE]  = {0};
	char tmpStr4[MAX_VALUE_SIZE]  = {0};
	char emuType[MAX_VALUE_SIZE] = {0};
	char *p = NULL;
#if defined(TCSUPPORT_ECN_MEGACO)
	char termName[MAX_VALUE_SIZE] = {0};
#endif

	tcdbg_printf ("[Simulate Test] %s [%d] testType [%d]\n", __func__, i, gVoipSimulateState[i].testType);
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],VOIPSIMULATETEST);
	sprintf(nodeName[1],"Entry%d",i);

	if((getAttrValue(top, nodeName, SIMULATE_CALLED_NUM, tmpStr) != TCAPI_PROCESS_OK)\
		|| (getAttrValue(top, nodeName, "DailDTMFConfirmEnable", tmpStr1) != TCAPI_PROCESS_OK)\
		|| (getAttrValue(top, nodeName, "DailDTMFConfirmNumber", tmpStr2) != TCAPI_PROCESS_OK)\
		|| (getAttrValue(top, nodeName, "CallHoldTimer", tmpStr3) != TCAPI_PROCESS_OK)\
		|| (getAttrValue(top, nodeName, "CalledWaitTimer", tmpStr4) != TCAPI_PROCESS_OK)){
		tcdbg_printf("[Simulate Test] %s [%d] Get %s_%s parameters fail.\n", 
					__func__, i, nodeName[0], nodeName[1]);
		return FAIL;
	}	
#if defined(TCSUPPORT_ECN_MEGACO)
#if defined(TCSUPPORT_ECN_SIP)
	if(getSipStatus() == 1) {
#endif
		getH248PhysicalTermIDValue(top,i,termName);
#if defined(TCSUPPORT_ECN_SIP)
	}
#endif
#endif
	
	if (SIMULATE_CALLER == gVoipSimulateState[i].testType ){		
#if defined(TCSUPPORT_ECN_SIP)
#if defined(TCSUPPORT_ECN_MEGACO)
		if(getSipStatus() == 0) {
#endif
			snprintf(testCmd, sizeof(testCmd)-1, SIMULATE_OFFHOOK, i, SIMULATE_CALLER);
			system(testCmd);
#if defined(TCSUPPORT_ECN_MEGACO)
		}
#endif
#endif
	
#if defined(TCSUPPORT_ECN_MEGACO)
#if defined(TCSUPPORT_ECN_SIP)
		if(getSipStatus() == 1) {
#endif
			snprintf(testCmd, sizeof(testCmd)-1, SIMULATE_CMD_PREFIX, termName, OFFHOOK_SG);
			voipCmdSend(testCmd);
#if defined(TCSUPPORT_ECN_SIP)
		}
#endif
#endif
		memset(testCmd,0,sizeof(testCmd));
		sprintf(testCmd,"echo \"2 %d 1\"> /proc/fxs/dtmf",i);//enable line i dtmf detection when simulate off-hook
		system(testCmd);

#if defined(TCSUPPORT_ECN_SIP)
#if defined(TCSUPPORT_ECN_MEGACO)
		if(getSipStatus() == 0) {
#endif
			p = tmpStr;
			while (p != NULL && *p != '\0'){
				snprintf(testCmd, sizeof(testCmd)-1, SIMULATE_DIAL, i, *p);
				system(testCmd);
				p++;
			}
#if defined(TCSUPPORT_ECN_MEGACO)
		}
#endif
#endif
	}
	else if (SIMULATE_NONE == gVoipSimulateState[i].testType){
		tcdbg_printf("[Simulate Test] not supported!\n");
		return FAIL;
	}

	setAttrValue(top, nodeName, "DailDTMFConfirmResult", "");
	setAttrValue(top, nodeName, "Status", "");
	setAttrValue(top, nodeName, "Conclusion", "");
	setAttrValue(top, nodeName, "CallerFailReason", "");
	setAttrValue(top, nodeName, "CalledFailReason", "");
	/* [OSBNB00052025] ADD by Eric.guo@20151109 initialize the node FailedResponseCode once the simulate test start*/
	setAttrValue(top, nodeName, "FailedResponseCode", "0");	
	
	gVoipSimulateState[i].callHoldTime = atoi(tmpStr3);
	gVoipSimulateState[i].calledWaitTime = atoi(tmpStr4);
	if (0 == gVoipSimulateState[i].callHoldTime)
		gVoipSimulateState[i].callHoldTime = SIMULATE_CALL_HOLD_TIME;
	if (0 == gVoipSimulateState[i].calledWaitTime)
		gVoipSimulateState[i].calledWaitTime = SIMULATE_CALLED_WAIT_TIME;
	
	strncpy(gVoipSimulateState[i].confirmNum, tmpStr2, sizeof(gVoipSimulateState[i].confirmNum)-1);
	strncpy(gVoipSimulateState[i].confirmEnable, tmpStr1, sizeof(gVoipSimulateState[i].confirmEnable)-1);

	tcdbg_printf("[Simulate Test] %s [%d] confirmNum:%s, confirmEnable:%s\n", __func__, i, gVoipSimulateState[i].confirmNum,
		gVoipSimulateState[i].confirmEnable);
#if defined(TCSUPPORT_ECN_SIP)
#if defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus() == 0) {
#endif
		gVoipSimulateState[i].state = SIMULATE_WAIT_CONN_STATE;
		simulate_status_set(i, SIMULATE_STATUS_DIAL_TONE);
		snprintf(emuType, sizeof(emuType)-1, "%d", gVoipSimulateState[i].testType);
		setAttrValue(top,nodeName, LINE_SIMULATE_TYPE, emuType); //mtk06404 20140612
#if defined(TCSUPPORT_ECN_MEGACO)
	}
#endif
#endif

#if defined(TCSUPPORT_ECN_MEGACO)
#if defined(TCSUPPORT_ECN_SIP)
	if(getSipStatus() == 1) {
#endif
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],VOIPINFO_H248);
		sprintf(nodeName[1],"Entry%d",i);
		setAttrValue(top,nodeName,SIMULATE_START,SIMULATE_CFG_DONE);
#if defined(TCSUPPORT_ECN_SIP)
	}
#endif
#endif
	return SUCCESS;
}

static int line_simulate_sipstate_check(mxml_node_t *top, mxml_node_t *parant, int line_id)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char nodeName1[MAX_ARG_NUM][MAX_NODE_NAME];
	char tmpStr[MAX_VALUE_SIZE]  = {0};
	char tmpStr1[MAX_VALUE_SIZE]  = {0};
	int i = 0;
#if defined(TCSUPPORT_ECN_SIP)
#if defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus() == 0) {
#endif
		memset(nodeName,0,sizeof(nodeName));	
		strcpy(nodeName[0],VOIPINFO);
		strcpy(nodeName[1],"Common");	

		memset(nodeName1,0,sizeof(nodeName1));	
		strcpy(nodeName1[0],VOIPINFO);	
		
		sprintf(nodeName1[1],"Entry%d",line_id);

		if (getAttrValue(top, nodeName, "SC_START_STATUS", tmpStr) == TCAPI_PROCESS_OK
			&&getAttrValue(top, nodeName1, SC_LINE_REG_STATUS, tmpStr1) == TCAPI_PROCESS_OK
			&& !strcmp(tmpStr, "Yes")
			&& !strcmp(tmpStr1, "Up"))
		{
			return SUCCESS;
		}
		
		return FAIL;
#if defined(TCSUPPORT_ECN_MEGACO)
	}
#endif
#endif

#if defined(TCSUPPORT_ECN_MEGACO)
#if defined(TCSUPPORT_ECN_SIP)
	if(getSipStatus() == 1) {
#endif
		memset(nodeName,0,sizeof(nodeName));	
		strcpy(nodeName[0],VOIPH248);
		sprintf(nodeName[1],"Entry%d",line_id);	

		if (getAttrValue(top, nodeName, H248_REGISTER_STATUS, tmpStr) == TCAPI_PROCESS_OK
			&& 0==strcmp(tmpStr, "1"))
		{
			return SUCCESS;
		}
		
		return FAIL;
#if defined(TCSUPPORT_ECN_SIP)
	}
#endif

#endif
	
}

static int line_simulate_test_write(mxml_node_t *top, mxml_node_t *parant)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char nodeName1[MAX_ARG_NUM][MAX_NODE_NAME];
	char tmpStr[MAX_VALUE_SIZE]  = {0};
	char tmpStr2[MAX_VALUE_SIZE] = {0};
	char tmpStr3[MAX_VALUE_SIZE] = {0};
	char tmpStr4[MAX_VALUE_SIZE] = {0};
	int i = 0;
		
	memset(nodeName,0,sizeof(nodeName));	
	strcpy(nodeName[0],VOIPTEST);

	memset(nodeName1,0,sizeof(nodeName1));	
	strcpy(nodeName1[0],VOIPSIMULATETEST);

	tcdbg_printf ("[Simulate Test] %s \n", __func__);
	updateVoipLineNum(top);
	for(i=0 ; i<VOIPLineNum ; i++){
		sprintf(nodeName[1],"Entry%d",i);
		sprintf(nodeName1[1],"Entry%d",i);
		memset(tmpStr, 0, sizeof(tmpStr));
		memset(tmpStr2, 0, sizeof(tmpStr2));	
		memset(tmpStr3, 0, sizeof(tmpStr3));
		memset(tmpStr4, 0, sizeof(tmpStr4));	
		if((getAttrValue(top, nodeName, "TestState", tmpStr) == TCAPI_PROCESS_OK)\
			&& (getAttrValue(top, nodeName, "TestSelector", tmpStr2) == TCAPI_PROCESS_OK)\
			&& (getAttrValue(top, nodeName, "IsTestOnBusy", tmpStr3) == TCAPI_PROCESS_OK)\
			&& (getAttrValue(top, nodeName1, "TestType", tmpStr4) == TCAPI_PROCESS_OK)){

			if (!strcmp(tmpStr4, LINE_CALLER_SIMULATE)){
				gVoipSimulateState[i].testType = SIMULATE_CALLER;
			}
			else if (!strcmp(tmpStr4, LINE_CALLED_SIMULATE)){
				gVoipSimulateState[i].testType = SIMULATE_CALLED;
			}
			else{
				tcdbg_printf("[Simulate Test] %s [%d] not supported!\n", __func__, i);
				gVoipSimulateState[i].testType = SIMULATE_NONE;
				return FAIL;
			}
			/* prevent to reboot when simulate test progress  */
			if((!strcmp(tmpStr,LINE_TEST_REQ))\
				&&(!strcmp(tmpStr2,LINE_SIMULATE_TEST))\
				&&(!strcmp(tmpStr3,"0"))){
				if (FAIL == line_simulate_sipstate_check(top, parant, i)){
					gVoipSimulateState[i].state = SIMULATE_SIP_NO_OK_STATE;
				
				}else
				{
					if (SUCCESS == line_simulate_type_deal(i, top, parant))
					    setAttrValue(top, nodeName, "IsTestOnBusy", "1");
				}
			}
		}else{
		    tcdbg_printf("[Simulate Test] %s [%d] get test param fail!\n", __func__, i);
		}
	}


    return SUCCESS;
}
#endif


int voip_test_write(mxml_node_t *top, mxml_node_t *parant)
{
	/* check test selector */	
	line_test_write(top, parant);
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
	line_simulate_test_write(top, parant);
#endif
	
    return SUCCESS;
}

/* mtk06404 add emulate test */
int line_test_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int testState[VOIP_LINE_NUM] = {-1,-1};
	int lintestRes[8] = {0};
	int i;
	int test;
	char tmpStr[MAX_VALUE_SIZE]  = {0};
	char testvalue[MAX_VALUE_SIZE] = {0};
	memset(nodeName,0,sizeof(nodeName));	
	strcpy(nodeName[0],VOIPTEST);
	FILE *fp = NULL;
	char* lineProcFile = LINE_TEST_PROC_NAME;
	
	if((fp = fopen(lineProcFile,"r"))!=NULL){	
		updateVoipLineNum(top);
		for(i=0 ; i<VOIPLineNum ; i++){
			sprintf(nodeName[1],"Entry%d",i);
			if ((getAttrValue(top, nodeName, "TestSelector", tmpStr) == TCAPI_PROCESS_OK)){
				if(!strcmp(tmpStr,LINE_TEST_PHONE_CONN)){
					fscanf(fp,"%d %d",&testState[0], &testState[1]);
					if(testState[i] == 1){
						setAttrValue(top, nodeName, "PhoneConnectivity", "1");
						setAttrValue(top, nodeName, "TestState", "Complete");
					}
					else if (testState[i] == 0){
						setAttrValue(top, nodeName, "PhoneConnectivity", "0");
						setAttrValue(top, nodeName, "TestState", "Complete");
					}
					else
						setAttrValue(top, nodeName, "TestState", "Fail");
				}			
				/* Line Voltage Test */
				else if(!strcmp(tmpStr,LINE_TEST_TID_LINE_V)){
					/* voltage test result format :vAcTG  vAcR vDcTG  vDcRG vAcTR */
					fscanf(fp,"%d %d %d %d %d",&lintestRes[0], &lintestRes[1], 
						&lintestRes[2], &lintestRes[3],&lintestRes[4]);
					memset(testvalue, 0, sizeof(testvalue));
					sprintf(testvalue,"%d", lintestRes[4]);
					setAttrValue(top, nodeName, "LineTestData_vDcDiff", testvalue);
					tcdbg_printf("state value %s \r\n",testvalue);
					setAttrValue(top, nodeName, "TestState", "Complete");
				}
				/* Resistive Fault */
				else if(!strcmp(tmpStr,LINE_TEST_TID_RES_FLT)){
					/* resistance test  result format : rTG  rRG rTR */
					fscanf(fp,"%d %d %d",&lintestRes[0], &lintestRes[1], 
						&lintestRes[2]);
					memset(testvalue, 0, sizeof(testvalue));
					testState[0]=lintestRes[2]/10000;
					sprintf(testvalue,"%d",testState[0]);
					setAttrValue(top, nodeName, "LineTestData_rtr", testvalue);
					tcdbg_printf("state value %s \r\n",testvalue);
					setAttrValue(top, nodeName, "TestState", "Complete");
				}
				else {
					setAttrValue(top, nodeName, "TestState", "Error_TestNotSupported");
					fclose(fp);
					return FAIL;
				}
			}			
		}
		setAttrValue(top, nodeName, "IsTestOnBusy", "0");
		fclose(fp);
	}
	else
		tcdbg_printf("Read %s Failed!!\r\n",lineProcFile);
	
	return SUCCESS;
}

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
static int line_simulate_test_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	return SUCCESS;
}
#endif

int voip_test_boot(mxml_node_t *top)
{
	int i = 0;	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
		
	memset(nodeName,0,sizeof(nodeName));	
	strcpy(nodeName[0],VOIPTEST);

	updateVoipLineNum(top);
	for (i=0; i <VOIPLineNum; i++){
		sprintf(nodeName[1],"Entry%d",i);
		setAttrValue(top, nodeName, "IsTestOnBusy", "0");
		setAttrValue(top, nodeName, "TestState", "None");
	}

	return SUCCESS;
}

int voip_test_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	line_test_execute(top, name);

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
	line_simulate_test_execute(top, name);
#endif

    return SUCCESS;
}


int
voip_simulate_test_init(void){

	cfg_node_t node={
		.name=VOIPSIMULATETEST,
		.type=ENTRY_TYPE|VOIP_SIMULATE_TEST_NUM,
		.adv_table=NULL,
		.merge_table=NULL,
		.cfg_read=NULL, 
		.cfg_write=voip_simulate_test_write,
		.cfg_verify=voip_simulate_test_verify,
		.cfg_execute=voip_simulate_test_execute,
	};

	return register_cfg_node(&node);
}
int voip_simulate_test_write(mxml_node_t *top, mxml_node_t *parant)
{
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
	int i = 0;

	updateVoipLineNum(top);
	for(i=0 ; i<VOIPLineNum ; i++){
		if (SIMULATE_INIT_STATE != gVoipSimulateState[i].state){
			gVoipSimulateState[i].state = SIMULATE_BLOCK_STATE;
		}
	}
#endif
    return SUCCESS;
}

int voip_simulate_test_verify(mxml_node_t *node)
{
    return SUCCESS;
}

int voip_simulate_test_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
    return SUCCESS;
}

int
voip_h248_init(void){

	cfg_node_t node={
		.name=VOIPH248,
		.type=ADV_FLAG|ENTRY_TYPE|VOIP_LINE_NUM,
		.adv_table=voip_h248_adv_table,
		.merge_table=NULL,
		.cfg_read=NULL, 
		.cfg_write=voip_h248_write,
		.cfg_verify=voip_h248_verify,
		.cfg_execute=voip_h248_execute,
		.cfg_boot=voip_h248_boot,
	};

	return register_cfg_node(&node);
}
int voip_h248_write(mxml_node_t *top, mxml_node_t *parant)
{
    
    return SUCCESS;
}

int voip_h248_verify(mxml_node_t *node)
{
    return SUCCESS;
}

#if defined(TCSUPPORT_ECN_MEGACO)
int voipCmdSend(char * SendData) 
{	
	struct sockaddr_un serverAddr;	
	StcpIpAddress theRemoteAddress;
	int fClientSocket = HTTP_SOCKET_ERROR;	
	int status  = HTTP_SOCKET_ERROR;
#if defined(TCSUPPORT_ECN_SIP)
	if(getSipStatus() == 0) {
		return 0;//return 0 if sip enable
	}
#endif

	fClientSocket = socket(AF_UNIX, SOCK_STREAM, 0);	
	if(fClientSocket == HTTP_SOCKET_ERROR){
		tcdbg_printf("StcpOpenActive: socket error!!!\n");		
		return -1;
	}

	serverAddr.sun_family 	    = AF_UNIX;
	strncpy(serverAddr.sun_path, "/tmp/mg_sock0", sizeof(serverAddr.sun_path)-1);

	status = connect(fClientSocket, (struct sockaddr*)&serverAddr, SOCKSIZE);		
	if(status == HTTP_SOCKET_ERROR){		
		tcdbg_printf("StcpOpenActive: connect error!!!\n");				
		close(fClientSocket);	
		return -1;
	}	
	
	send(fClientSocket, SendData, strlen(SendData), 0);
		
	close(fClientSocket);	
	return 0;
}/* StcpOpenActive */

void h248RealExecute(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char infoH248Node[MAX_ARG_NUM][MAX_NODE_NAME];
	char MandatoryReg[8] ={0};
	char MandatoryRegFlag[8] ={0};
	int pvc_index = 0, entry_index= 0;	
	char str_servicelist[32] = {0};
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPH248);
	strcpy(nodeName[1],VOIPCOMMON);		
	
	if(getAttrValue(top, nodeName, MANDATORY_REGISTER, MandatoryReg) == TCAPI_PROCESS_OK && strcmp(MandatoryReg,"Yes") == 0){
		memset(infoH248Node,0,sizeof(infoH248Node));
		strcpy(infoH248Node[0],VOIPINFO_H248);
		strcpy(infoH248Node[1],VOIPCOMMON);		
		if(getAttrValue(top, infoH248Node, MANDATORY_REGISTER_FLAG, MandatoryRegFlag) == TCAPI_PROCESS_OK && strcmp(MandatoryRegFlag,"1") == 0){
			tcdbg_printf("h248RealExecute: MandatoryRegister is True!!!\n");\			
			setAttrValue(top, nodeName, MANDATORY_REGISTER_FLAG, "0");
			voipCmdSend("callCmd {test.Restart}");
			return;
		}
	}

	system("/usr/bin/killall -9 mgapp");
	system("/userfs/bin/mgapp -id 0&");
	return;
}
#endif
int voip_h248_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
#if defined(TCSUPPORT_ECN_MEGACO)
	h248RealExecute(top);
#endif
	return SUCCESS;
}

int voip_h248_boot(mxml_node_t *top)
{    	
    return SUCCESS;
}

int
voip_ims_init(void){

	cfg_node_t node={
		.name=VOIPIMS,
		.type=ENTRY_TYPE|VOIP_IMS_NUM,
		.adv_table=NULL,
		.merge_table=NULL,
		.cfg_read=NULL, 
		.cfg_write=voip_ims_write,
		.cfg_verify=voip_ims_verify,
		.cfg_execute=voip_ims_execute,
	};

	return register_cfg_node(&node);
}
int voip_ims_write(mxml_node_t *top, mxml_node_t *parant)
{
    return SUCCESS;
}

int voip_ims_verify(mxml_node_t *node)
{
    return SUCCESS;
}

int voip_ims_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
    return SUCCESS;
}

int
voip_diagnostic_init(void){

	cfg_node_t node={
		.name=VOIPDIAGNOSTIC,
		.type=SINGLE_TYPE,
		.adv_table=NULL,
		.merge_table=NULL,
		.cfg_read=NULL, 
		.cfg_write=voip_diagnostic_write,
		.cfg_verify=voip_diagnostic_verify,
		.cfg_execute=voip_diagnostic_execute,
	};

	return register_cfg_node(&node);
}
int voip_diagnostic_write(mxml_node_t *top, mxml_node_t *parant)
{
    return SUCCESS;
}

int voip_diagnostic_verify(mxml_node_t *node)
{
    return SUCCESS;
}

int voip_diagnostic_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
    return SUCCESS;
}

int
voip_poor_ql_init(void){

	cfg_node_t node={
		.name=VOIPPOORQL,
		.type=UPDATE_TYPE|PVC_TYPE|WEB_TYPE|ENTRY_TYPE|VOIP_POOR_QL_NUM,
		.adv_table=NULL,
		.merge_table=NULL,
		.cfg_read=voip_poor_ql_read, 
		.cfg_write=NULL,
		.cfg_verify=NULL,
		.cfg_execute=NULL,
	};

	return register_cfg_node(&node);
}

static void _getPvcAndEntryIdFromPoorQl(char name[][MAX_NODE_NAME], int *pvc, int *entryId)
{
    int i = 0;
    
    for(i=0; i < MAX_ARG_NUM; i++){
        if (strstr(name[i], WAN_PVC)){
            *pvc = atoi(name[i] + strlen(WAN_PVC));
        }
        if (strstr(name[i], SUB_NODE_NAME)){
            *entryId = atoi(name[i] + strlen(SUB_NODE_NAME));
        }
    }
}

static void _voipPoorQlAttrInit(mxml_node_t * curNode){
    char strTmp[] = "0";

    if (curNode == NULL)
        return;
    
    mxmlElementSetAttr(curNode, "StatTime", "");
    mxmlElementSetAttr(curNode, "TxPackets", strTmp);
    mxmlElementSetAttr(curNode, "RxPackets", strTmp);            
    mxmlElementSetAttr(curNode, "MeanDelay", strTmp);
    mxmlElementSetAttr(curNode, "MeanJitter", strTmp);
    mxmlElementSetAttr(curNode, "FractionLoss", strTmp);
    mxmlElementSetAttr(curNode, "LocalIPAddress", strTmp);
    mxmlElementSetAttr(curNode, "LocalUDPPort", strTmp);
    mxmlElementSetAttr(curNode, "FarEndIPAddress", strTmp);
    mxmlElementSetAttr(curNode, "FarEndUDPPort", strTmp);
    mxmlElementSetAttr(curNode, "MosLq", strTmp);
    mxmlElementSetAttr(curNode, "Codec", "");

    return ;
}

int voip_poor_ql_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char* attr)
{
#if defined(TCSUPPORT_ECN_SIP)
/*  //proc/vdsp/debug/poorQlRecord
    poor_quality_index  	 -1~9
    dateTime                             2014-04-14T12:20:30 
    txPkgCnt                              1400 
    rxPkgCnt                              1300 
    meanDelay                          0 
    meanJitter                           0 
    fractionLoss                         20 
    localIPAddr                          192
    localUdpPort                        5060 
    remoteIPAddr                     222
    remoteUdpPort                   5060
    mosLq                                   0 
    codec                                    G722

    //InfoVoIPPoorQL_PVCX_EntryX XXXXX
 */
    int i = 0;
    char cmd[MAX_CMD_SIZE] = {0};
    voip_poor_ql_attr qlAttr = {0};
    FILE *fp = NULL;	
    char strTmp[MAX_VALUE_SIZE] = {0}; 
    char lineStr[1024] = {0};
    int  pvcNum = -1;    /*descript line info */
    int  entryNum = -1;
    mxml_node_t * subPvc = NULL;
    mxml_node_t * subEntry = NULL;
    mxml_node_t * parantNode = NULL;
    mxml_node_t * curNode = NULL;  
    int  initFlg = 0;
    
#if defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus() == 0) {
#endif

    curNode = top;
    for(i=0; i < MAX_ARG_NUM; i++){
		if(strlen(name[i])==0){
			break;
		}
		parantNode = curNode;
		curNode = mxmlFindElement(curNode, curNode, name[i], NULL, NULL, MXML_DESCEND);
		if(curNode == NULL){
			curNode = mxmlNewElement(parantNode, name[i]);
            initFlg = 1;
		}
	}

    _getPvcAndEntryIdFromPoorQl(name, &pvcNum, &entryNum);
    if (pvcNum <0 || pvcNum > VOIP_LINE_NUM-1 || entryNum < 0 || entryNum > VOIP_POOR_QL_NUM-1){
        tcdbg_printf("invalid param, pvcNum is %d, entryNum is %d\n", pvcNum, entryNum);
        return FAIL;
    }

    if (initFlg)
        _voipPoorQlAttrInit(curNode);
    
    snprintf(cmd, sizeof(cmd)-1, "echo \"%d %d\" > /proc/vdsp/debug/poorQlRecord", pvcNum, entryNum);
    //tcdbg_printf("cmd is %s \n", cmd);
    system(cmd);

    if ((fp = fopen(POOR_QL_PROC_PATH,"r")) == NULL){
        tcdbg_printf("=======%s is invalid=======\n", POOR_QL_PROC_PATH);
    }else{
        while(fgets(lineStr, 1024 - 1, fp)){
            if (0 == strlen(lineStr))
                continue;
            
            i = sscanf(lineStr,"%s %u %u %u %u %u %x %u %x %u %u %s", qlAttr.dateTime,
                &qlAttr.txPkgCnt, &qlAttr.rxPkgCnt, &qlAttr.meanDelay, &qlAttr.meanJitter,
                &qlAttr.fractionLoss, &qlAttr.localIPAddr, &qlAttr.localUdpPort,
                &qlAttr.remoteIPAddr, &qlAttr.remoteUdpPort, &qlAttr.mosLq, &qlAttr.codec);
            if (i != 12){
                break;
            }
#if 0            
            tcdbg_printf("%s\n%d\n%d\n%d\n%d\n%d\n%x\n%d\n%x\n%d\n%d\n%s\n", qlAttr.dateTime,
            qlAttr.txPkgCnt, qlAttr.rxPkgCnt, qlAttr.meanDelay,qlAttr.meanJitter,
            qlAttr.fractionLoss, qlAttr.localIPAddr, qlAttr.localUdpPort,
            qlAttr.remoteIPAddr,qlAttr.remoteUdpPort, qlAttr.mosLq, qlAttr.codec);
#endif    
            mxmlElementSetAttr(curNode, "StatTime", qlAttr.dateTime);
            snprintf(strTmp, sizeof(strTmp)-1, "%u", qlAttr.txPkgCnt);
            mxmlElementSetAttr(curNode, "TxPackets", strTmp);
            
            memset(strTmp, 0, sizeof(strTmp));
            snprintf(strTmp, sizeof(strTmp)-1, "%u", qlAttr.rxPkgCnt);
            mxmlElementSetAttr(curNode, "RxPackets", strTmp);
            
            memset(strTmp, 0, sizeof(strTmp));
            snprintf(strTmp, sizeof(strTmp)-1, "%u", qlAttr.meanDelay);
            mxmlElementSetAttr(curNode, "MeanDelay", strTmp);
            
            memset(strTmp, 0, sizeof(strTmp));
            snprintf(strTmp, sizeof(strTmp)-1, "%u", qlAttr.meanJitter);
            mxmlElementSetAttr(curNode, "MeanJitter", strTmp);
            
            memset(strTmp, 0, sizeof(strTmp));
            snprintf(strTmp, sizeof(strTmp)-1, "%.lf", (double)((qlAttr.fractionLoss*100)/(qlAttr.rxPkgCnt+qlAttr.fractionLoss)));
            mxmlElementSetAttr(curNode, "FractionLoss", strTmp);
            
            memset(strTmp, 0, sizeof(strTmp));
            snprintf(strTmp, sizeof(strTmp)-1, "%s", inet_ntoa(qlAttr.localIPAddr));
            mxmlElementSetAttr(curNode, "LocalIPAddress", strTmp);
            
            memset(strTmp, 0, sizeof(strTmp));
            snprintf(strTmp, sizeof(strTmp)-1, "%d", qlAttr.localUdpPort);
            mxmlElementSetAttr(curNode, "LocalUDPPort", strTmp);
            
            memset(strTmp, 0, sizeof(strTmp));
            snprintf(strTmp, sizeof(strTmp)-1, "%s", inet_ntoa(qlAttr.remoteIPAddr));
            mxmlElementSetAttr(curNode, "FarEndIPAddress", strTmp);
            
            memset(strTmp, 0, sizeof(strTmp));
            snprintf(strTmp, sizeof(strTmp)-1, "%d", qlAttr.remoteUdpPort);
            mxmlElementSetAttr(curNode, "FarEndUDPPort", strTmp);
            
            memset(strTmp, 0, sizeof(strTmp));
            snprintf(strTmp, sizeof(strTmp)-1, "%d", qlAttr.mosLq);
            mxmlElementSetAttr(curNode, "MosLq", strTmp);
            
            mxmlElementSetAttr(curNode, "Codec", qlAttr.codec);
        }
        fclose(fp);
    }
	
#if defined(TCSUPPORT_ECN_MEGACO)
	}
#endif

#endif

    return SUCCESS;
}

int
voip_info_init(void){
	cfg_node_t node={
		.name=VOIPINFO,
#ifdef TCSUPPORT_CT_VOIP_SIP
		.type=ADV_FLAG|UPDATE_TYPE|WEB_TYPE|ENTRY_TYPE|VOIP_INFO_NUM,
		.adv_table=info_voip_adv_table,
#else
		.type=UPDATE_TYPE|WEB_TYPE|ENTRY_TYPE|VOIP_INFO_NUM,
		.adv_table=NULL,
#endif
		.merge_table=NULL,
		.cfg_read=voip_info_read, 
		.cfg_write=NULL,
		.cfg_verify=NULL,
		.cfg_execute=voip_info_execute,
	};

	return register_cfg_node(&node);
}

int voip_info_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char* attr)
{
#if defined(TCSUPPORT_ECN_SIP)
#if defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus() == 0) {
#endif
	mxml_node_t * voipInfoGroup =mxmlFindElement(top, top, VOIPINFO, NULL, NULL, MXML_DESCEND);
	if (NULL == voipInfoGroup)    
	{       
		voipInfoGroup = mxmlNewElement(top, VOIPINFO); 
	}
	
	mxml_node_t * subCommon;
	if( NULL == ( subCommon = mxmlFindElement(voipInfoGroup, voipInfoGroup, STRING_COMMON, NULL, NULL, MXML_DESCEND) ))  
	{       
		subCommon = mxmlNewElement(voipInfoGroup, STRING_COMMON); 
	}
	mxmlElementSetAttr(subCommon, "EntryID0", "1");
	mxmlElementSetAttr(subCommon, "Codec0", "G.711MuLaw");
	mxmlElementSetAttr(subCommon, "BitRate0", "16");
	mxmlElementSetAttr(subCommon, "PacketizationPeriod0", "10,20,30");
	mxmlElementSetAttr(subCommon, "SilenceSuppression0", "1");

	mxmlElementSetAttr(subCommon, "EntryID1", "2");
	mxmlElementSetAttr(subCommon, "Codec1", "G.711ALaw");
	mxmlElementSetAttr(subCommon, "BitRate1", "16");
	mxmlElementSetAttr(subCommon, "PacketizationPeriod1", "10,20,30");
	mxmlElementSetAttr(subCommon, "SilenceSuppression1", "1");

	mxmlElementSetAttr(subCommon, "EntryID2", "3");
	mxmlElementSetAttr(subCommon, "Codec2", "G.729");
	mxmlElementSetAttr(subCommon, "BitRate2", "16");
	mxmlElementSetAttr(subCommon, "PacketizationPeriod2", "10,20,30");
	mxmlElementSetAttr(subCommon, "SilenceSuppression2", "1");

	mxmlElementSetAttr(subCommon, "EntryID3", "4");
	mxmlElementSetAttr(subCommon, "Codec3", "G.726");
	mxmlElementSetAttr(subCommon, "BitRate3", "16,24,32");
	mxmlElementSetAttr(subCommon, "PacketizationPeriod3", "10,20,30");
	mxmlElementSetAttr(subCommon, "SilenceSuppression3", "0");

	mxmlElementSetAttr(subCommon, "EntryID4", "5");
	mxmlElementSetAttr(subCommon, "Codec4", "G.723.1");
	mxmlElementSetAttr(subCommon, "BitRate4", "16");
	mxmlElementSetAttr(subCommon, "PacketizationPeriod4", "10,20,30");
	mxmlElementSetAttr(subCommon, "SilenceSuppression4", "0");

	mxmlElementSetAttr(subCommon, "EntryID5", "6");
	mxmlElementSetAttr(subCommon, "Codec5", "G.722");
	mxmlElementSetAttr(subCommon, "BitRate5", "16");
	mxmlElementSetAttr(subCommon, "PacketizationPeriod5", "10,20,30");
	mxmlElementSetAttr(subCommon, "SilenceSuppression5", "0");
#if defined(TCSUPPORT_ECN_MEGACO)
	}
#endif
#endif

    //
    /* update voip line status */    
    if (!strcmp(attr, "RegFailReason") 
        && isVoIPAppDown(0))
    {
        char InfoVoIP_node[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
        int i = 0;

        memset(InfoVoIP_node,0,sizeof(InfoVoIP_node));
    	strcpy(InfoVoIP_node[0], "InfoVoIP");
        
        /* 1. update InfoVoIP_Entry%d Status to be Error
                    2. update InfoVoIP_Entry%d RegFailReason to be 1(IAD error)*/
        for(i=0; i<VOIPLineNum; i++){
            snprintf(InfoVoIP_node[1], sizeof(InfoVoIP_node[1])-1, "Entry%d", i);
            setAttrValue(top, InfoVoIP_node, "Status", "Error");
            setAttrValue(top, InfoVoIP_node, "RegFailReason", "1");
        }
    }
    
	return SUCCESS;
}
int voip_info_execute(mxml_node_t *top)
{
/* use function  getAttrValue to get the value of POTSindex \ CallDirect \ IsSuccess \ CallDuration \ CallNumber in node InfoVoIP_Common */
/* the callNumber may not be all digit */
	
	return SUCCESS;
}


#endif




#if defined(TCSUPPORT_ECN_MEGACO)
char *
voip_info_h248_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};


int
voip_info_248_init(void){
	cfg_node_t node={
		.name=VOIPINFO_H248,
		.type=ADV_FLAG|UPDATE_TYPE|WEB_TYPE|ENTRY_TYPE|VOIP_LINE_NUM,
		.adv_table=voip_info_h248_adv_table,
		.merge_table=NULL,
		.cfg_read=voip_info_248_read, 
		.cfg_write=voip_info_248_write,
		.cfg_verify=NULL,
		.cfg_execute=NULL,
	};

	return register_cfg_node(&node);
}


int voip_info_248_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char* attr)
{
    int i = 0;
    /* update voip line status */    
    if (!strcmp(attr, "RegFailReason") 
        && isVoIPAppDown(1))
    {
        char InfoVoIPH248_node[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
        char VoIPH248_node[MAX_ARG_NUM][MAX_NODE_NAME] = {0};

        memset(InfoVoIPH248_node,0,sizeof(InfoVoIPH248_node));
    	strcpy(InfoVoIPH248_node[0], "InfoVoIPH248");
        
        memset(VoIPH248_node,0,sizeof(VoIPH248_node));
    	strcpy(VoIPH248_node[0], "VoIPH248");
        
        /* 1. update InfoVoIPH248_Entry%d Status to be Error 
                     2. update InfoVoIPH248_Entry%d RegFailReason to be 1(IAD error)*/
        for(i=0; i<VOIPLineNum; i++){
            snprintf(InfoVoIPH248_node[1], sizeof(InfoVoIPH248_node[1])-1, "Entry%d", i);
            snprintf(VoIPH248_node[1], sizeof(VoIPH248_node[1])-1, "Entry%d", i);
            setAttrValue(top, VoIPH248_node, "UserServiceState", "5");/*5-Register fail*/
            setAttrValue(top, InfoVoIPH248_node, "RegFailReason", "1");
        }
    }
    
    return SUCCESS;
}

int voip_info_248_write(mxml_node_t *top, mxml_node_t *parant)
{
	return SUCCESS;
}



#endif


