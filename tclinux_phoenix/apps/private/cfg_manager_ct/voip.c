#include "cfg_manager.h"
#include "utility.h"

#ifdef VP_MEGACO
#undef TCSUPPORT_VOIP_IMSSIP
#include "sp_ipc_msg.h"
#include "pmegaco_support.h"
#else
#include "voip_data_util.h"
#endif

#if VOIP
/* jrchen add 20100223 */
#define LOG_NODE_MAX_LEN 18
#define LINE_TEST_PROC_NAME "/proc/fxs/sliclinetest"
typedef struct log_node_s
{
	char attr[LOG_NODE_MAX_LEN];
	char value[LOG_NODE_MAX_LEN];
}log_node_t;

log_node_t log_default_setting[] = {
	{LOGENABLE, "No"}, 
	{LOGSERVERIP, "192.168.1.100"},
	{LOGSERVERPORT, "5678"},
	{LOGDISPLAY, "Console"},
	{"", ""},
};

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

char *
voip_advanced_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
char *
voip_advanced_merge_table[MAX_NODE_NAME]=
{
	"LineNum","",
};
char *
voip_call_ctrl_merge_table[MAX_NODE_NAME]=
{
	"SIPIndexCall","",
};
char *
voip_media_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
char *
voip_media_merge_table[MAX_NODE_NAME]=
{
	"SIPIndexMedia","",
};
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
char *
voip_h248_adv_table[MAX_NODE_NAME]=
{
	"Common","",
};
	
	char sip_default_para[][2][25]={
		{"DebugLevel"			,"2"},
		{"Linenum"			,"2"},
		{"MaxCallLegs"			,"10"},
		{"ReferTimeout"			,"120000"},
		{"DialToneDuration"		,"15000"},
		//{"CallWaitingReply"		,"180"},
		{"WatchdogInterval"		,"180"},
		{"DtmfRelay"			,"2"},
		{"EnableSdpLogs"		,"1"},
		{"connectMediaOn180"		,"1"},
		{"autoAnswer"			,"0"},
		{"autoDisconnect"		,"0"},
		{"AddUpdateSupport"		,"1"},
		{"UpdateRetryAfterTimeout"	,"10"},
		{"CallerUpdateResendTimeout"	,"4000"},
		{"CalleeUpdateResendTimeout"	,"2000"},
		{"AutoRegistration"		,"1"},
/* jrchen modify enlarge SIP registra expiration time*/		
//{"RegistrationExpire"		,"20"},
		{"RegistrationExpire"		,"3600"},
/* jrchen modify end */		
		{"UnregistrationExpire"		,"10"},
		{"MaxRegisterClients"		,"5"},
		{"RemoveOldAuthHeaders"		,"1"},
		//{"EppPort"			,"3044"},
		{"localIpPrefix"		,"127.0.0.0"},
		{"localIpMask"			,"255.0.0.0"},
		{"CallForwardNoReplyTimeout"	,"15"},
		{"disableGuiMedia","1"},
		{"",""},
	};
	
	char ipp_log_default_para[][2][47]={
		{"IPP_UTIL"	,"ERROR,EXCEPTION,WARNING,INFO"},
		{"IPP_USERAPP"	,"ERROR,EXCEPTION,WARNING,INFO,DEBUG,ENTER,LEAVE"},
		{"IPP_CALLCON"	,"ERROR,EXCEPTION,WARNING,INFO,DEBUG"},
		{"IPP_MDM"	,"ERROR,EXCEPTION,WARNING,INFO"},
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
		{"msgLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"transportLogFilters"		,"INFO,ERROR,EXCEPTION,WARNING,DEBUG"},
		{"parserLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"stackLogFilters"		,"ERROR,EXCEPTION,WARNING,INFO"},
		{"msgBuilderLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"authenticatorLogFilters"	,"ERROR,EXCEPTION,WARNING"},
		{"regClientLogFilters"		,"ERROR,EXCEPTION,WARNING,INFO"},
		{"subscriptionLogFilters"	,"ERROR,EXCEPTION,WARNING"},
		{"compartmentLogFilters"	,"ERROR,EXCEPTION,WARNING"},
		{"transmitterLogFilters"	,"ERROR,EXCEPTION,WARNING"},
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
		{"coreLogFilters"		,"ERROR,EXCEPTION,WARNING"},
		{"",""},
	};
	
	char media_ctrl_default_para[][2][ATTR_SIZE]={
		{"DefaultDevice"	,"1"},
		{"RTPAddress"		,"0.0.0.0"},
		{"RTPPortRange"		,"8"},
		//{"VideoFrameSize"	,"2"},
		//{"VideoFrameRate"	,"30"},
		{"RecvAudioPolling_ms"	,"20"},
		{"PlayAudioPolling_ms"	,"20"},
		//{"RecvVideoPolling_ms"	,"32"},
		//{"PlayVideoPolling_ms"	,"30"},
		{"MfLogFilters"		,"ERROR,EXCEPTION,WARNING,INFO"},
		{"",""},
	};

int VOIPLineNum = 1;
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

#ifdef TCSUPPORT_VOIP_IMSSIP
        tcdbg_printf("LabelDeubg func:%s line:%d \n",__FUNCTION__,__LINE__);
        init_communicate_2voip();
#endif
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
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPBASIC);
	
	updateVoipLineNum(top);
	
	//sip basic username password
	for(i=0 ; i<VOIPLineNum ; i++){
		sprintf(nodeName[1],"Entry%d",i);
		if(getAttrValue(top, nodeName, "SIPEnable", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
				enablenum++;
			}
		}
	}
	if(enablenum > 0){
		system("/userfs/bin/TC_MTF &");
		pre_sys_state.voip_service = 1;
	}
	
	return SUCCESS;
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
	char buf[16]="";
	mxml_node_t *parentNode = NULL;
	
	parentNode = mxmlFindElement(top, top, DEVICEINFO,
				NULL, NULL, MXML_DESCEND);
	if(parentNode==NULL){
		parentNode =mxmlNewElement(top,  DEVICEINFO);
	}
	
	fp=fopen("/etc/sip_status","r");
	if(fp != NULL){
		fgets(buf,sizeof(buf),fp);
		fclose(fp);
	}

	if(strlen(buf) > 0){
		mxmlElementSetAttr(parentNode, "ConnStat", buf);
	}else{
		mxmlElementSetAttr(parentNode, "ConnStat", "Not Connected");
	}
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
	};

	return register_cfg_node(&node);
}
int voip_test_write(mxml_node_t *top, mxml_node_t *parant)
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
				&& (getAttrValue(top, nodeName, "IsTestOnBusy", tmpStr3) == TCAPI_PROCESS_OK)){
			if((!strcmp(tmpStr,LINE_TEST_REQ))&&(!strcmp(tmpStr2,LINE_TEST_PHONE_CONN))&&(!strcmp(tmpStr3,"0"))){
				memset(testCmd, 0, sizeof(testCmd));
				sprintf(testCmd,"echo \"%d 3\" > %s",i,lineProcFile);
				system(testCmd);				
				setAttrValue(top, nodeName, "IsTestOnBusy", "1");
			}
		}
	}
	
    return SUCCESS;
}


int voip_test_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int testState[VOIP_LINE_NUM] = {-1,-1};
	int i;
	memset(nodeName,0,sizeof(nodeName));	
	strcpy(nodeName[0],VOIPTEST);
	FILE *fp = NULL;
	char* lineProcFile = LINE_TEST_PROC_NAME;
	
	if((fp = fopen(lineProcFile,"r"))!=NULL){	
		fscanf(fp,"%d %d",&testState[0], &testState[1]);
		fclose(fp);
		updateVoipLineNum(top);
		for(i=0 ; i<VOIPLineNum ; i++){
			//tcdbg_printf("state %d=%d\r\n",i,testState[i]);
			sprintf(nodeName[1],"Entry%d",i);
			if(testState[i] == 1){
				setAttrValue(top, nodeName, "PhoneConnectivity", "1");
				setAttrValue(top, nodeName, "TestState", "Complete");
				setAttrValue(top, nodeName, "IsTestOnBusy", "0");
			}
			else if (testState[i] == 0){
				setAttrValue(top, nodeName, "PhoneConnectivity", "0");
				setAttrValue(top, nodeName, "TestState", "Complete");
				setAttrValue(top, nodeName, "IsTestOnBusy", "0");
			}
		}	
		
	}
	else
		tcdbg_printf("Read %s Failed!!\r\n",lineProcFile);
	
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


int voip_h248_boot(mxml_node_t *top)
{
    #ifdef VP_MEGACO
    system("/userfs/bin/voip -d &");
    
    pmegaco_cfg_InitMsg();
    #endif
    
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

int voip_h248_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
    
    #ifdef VP_MEGACO 
    
    int ret = 0;
    char strVoiceWan[64] = {0};
    char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {{0},{0},{0}};
    
    ret = voip_GetVoiceWanAddr(strVoiceWan);
    
    if(ret != 0)
    {
        tcdbg_printf("Frank voip_GetVoiceWanAddr error return %d\n",ret);
    }
    else
    {
        //ret = tcapi_set("VoIPH248_Common", "VoiceWanAddr", strVoiceWan);
        
        sprintf(nodeName[0], "VoIPH248");
        sprintf(nodeName[1], "Common");
    
        setAttrValue(top, nodeName, "VoiceWanAddr", strVoiceWan);
    }
    
    pmegaco_cfg_Notice2megaco();
    
    #endif
    
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
		.type=WEB_TYPE|ENTRY_TYPE|VOIP_POOR_QL_NUM,
		.adv_table=NULL,
		.merge_table=NULL,
		.cfg_read=voip_poor_ql_read, 
		.cfg_write=NULL,
		.cfg_verify=NULL,
		.cfg_execute=NULL,
	};

	return register_cfg_node(&node);
}
int voip_poor_ql_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char* attr)
{
    return SUCCESS;
}

int
voip_info_init(void){
	cfg_node_t node={
		.name=VOIPINFO,
		.type=UPDATE_TYPE|WEB_TYPE|ENTRY_TYPE|VOIP_INFO_NUM,
		.adv_table=NULL,
		.merge_table=NULL,
		.cfg_read=voip_info_read, 
		.cfg_write=NULL,
		.cfg_verify=NULL,
		.cfg_execute=NULL,
	};

	return register_cfg_node(&node);
}

enum attrIdxEnum
{
    PACKETS_RECEIVED  , PACKETS_SENT   ,
    BYTESR_ECEIVED    , BYTES_SENT     ,
    PACKETS_LOST      , VOIP_INFO_ATTR_MAX
};


enum { VOIP_CHANNEL_NUM = 3 };

static int getVoipChannelStatistics(unsigned long long (*data)[VOIP_INFO_ATTR_MAX])
{
    enum { channelLineCount = 7 };
    const char * filePath = "/proc/vdsp/debug/channelStatus";
    int lineCount = 0;
    char * lineBuff = NULL;
    int len;
    int attrIdx;
    int channelIdx = 0;
    char * delPos;
    FILE * filePtr = fopen(filePath, "r");
    if ( NULL == filePtr )
        return FAIL;

    for(; -1 != getline(&lineBuff, &len, filePtr); ++lineCount)
    {
        attrIdx = lineCount % channelLineCount -1;
        if ( 0 > attrIdx )
            continue;
        if ( 5 <= attrIdx )
        {
            if (++ channelIdx > VOIP_CHANNEL_NUM)
                return FAIL;
            continue;
        }

        delPos = strstr(lineBuff, ":");
        if ( NULL == delPos )
            return FAIL;

        sscanf(delPos + 1, "%llu", &data[channelIdx][attrIdx]);
    }
    free(lineBuff);
    return 0;
}


inline mxml_node_t * findOrCreateNode(mxml_node_t *pParent, const char * strName)
{
    mxml_node_t * pNode = mxmlFindElement(pParent, pParent, strName, NULL, NULL, MXML_DESCEND);
    if (NULL == pNode)
    {
        pNode = mxmlNewElement(pParent, strName);
    }
    return pNode;
}

static void calVoipChannelStatistics( unsigned long long  (*data)    [VOIP_INFO_ATTR_MAX], 
                                      unsigned long long  (*preData) [VOIP_INFO_ATTR_MAX],
                                      float *lostRate)
{
    int attrIdx = 0;
    for(; attrIdx < VOIP_INFO_ATTR_MAX; ++ attrIdx)
        if ( (*data)[attrIdx] < (*preData)[attrIdx] ) break;
    
    if ( attrIdx < VOIP_INFO_ATTR_MAX )
        memset(preData, 0, sizeof(int) * VOIP_INFO_ATTR_MAX);       


    for(attrIdx = 0; attrIdx < VOIP_INFO_ATTR_MAX; ++ attrIdx)
        (*data)[attrIdx] -= (*preData)[attrIdx];

    float totalPkg = (*data)[PACKETS_RECEIVED] + (*data)[PACKETS_LOST];
    
    if ( 0 != totalPkg )
        *lostRate = (*data)[PACKETS_LOST] / totalPkg;
    else
         *lostRate = 0.0;
}


int voip_info_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char* attr)
{
    const char * strAttr [] = 
    {
       "PacketsReceived" , "PacketsSent" , 
       "BytesReceived"   , "BytesSent"   ,
       "PacketsLost"
    };
   
    const char * strReset    = "ResetStatistics";
    const char * strLostRate = "ReceivePacketLossRate";
    
    static unsigned long long preStatistic[VOIP_CHANNEL_NUM][VOIP_INFO_ATTR_MAX] = {0};
    unsigned long long statistic[VOIP_CHANNEL_NUM][VOIP_INFO_ATTR_MAX];
    float lostRate[VOIP_CHANNEL_NUM];
    if ( 0 != getVoipChannelStatistics(statistic))
        memset(statistic, 0, sizeof (int) * VOIP_CHANNEL_NUM *VOIP_INFO_ATTR_MAX);
    
    mxml_node_t * voipInfoGroup = findOrCreateNode(top, VOIPINFO);
    if (NULL == voipInfoGroup) return FAIL;

    mxml_node_t * subEntry;
    char buff[20];
    int channelIdx = 0, attrIdx, reset;
    tcapi_msg_t msg;
    char * strResetValue = NULL;
    for( ; channelIdx < VOIP_CHANNEL_NUM; ++ channelIdx )
    {
        sprintf(buff, SUB_NODE_NAME "%d", channelIdx);
        if( NULL == ( subEntry = findOrCreateNode(voipInfoGroup, buff) ))
            return FAIL;

        strResetValue = mxmlElementGetAttr(subEntry, strReset);

        if(NULL == strResetValue)
            reset = 0;
        else
            reset = atoi(strResetValue);

        if (reset) // need to reset the statistics
        {
            for(attrIdx = 0; attrIdx < VOIP_INFO_ATTR_MAX; ++ attrIdx)
            {
                preStatistic[channelIdx][attrIdx] = statistic[channelIdx][attrIdx];
                mxmlElementSetAttr(subEntry, strAttr[attrIdx], "0");
            }
            mxmlElementSetAttr(subEntry, strLostRate, "0");
            mxmlElementSetAttr(subEntry, strReset, "0");
        }
        else
        {
            calVoipChannelStatistics(statistic   [channelIdx], 
                                     preStatistic[channelIdx], 
                                     &lostRate   [channelIdx]);
            
            for(attrIdx = 0; attrIdx < VOIP_INFO_ATTR_MAX; ++ attrIdx)
            {
                sprintf(buff, "%llu", statistic[channelIdx][attrIdx]);
                mxmlElementSetAttr(subEntry, strAttr[attrIdx], buff);
            }

            sprintf(buff, "%f", lostRate[channelIdx]);
            mxmlElementSetAttr(subEntry, strLostRate, buff);
        }
    }
    
    return SUCCESS;
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
		.type=ENTRY_TYPE|VOIP_LINE_NUM,
		//.type=SINGLE_TYPE,
		.adv_table=NULL,
 		.merge_table=voip_call_ctrl_merge_table,
		//.merge_table=NULL,
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
	
	put_call_ctrl(fp,top);
	put_log(fp, top); /* jrchen add 20100226 */

			
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
	voip_put_str(fp,"bDisableMerging","1");
	fputs("\n",fp);
	
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
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[32];
	int i, enablenum=0;
	struct stat stat_buf;
	mxml_node_t *parentNode = NULL;
	
	for(i=0;i<RETRY_TIME;i++){
		if(stat("/etc/restart_voip",&stat_buf) < 0){
			break;
		}else{
			sleep(1);
		}
	}

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPBASIC);
	
	//sip basic username password
	updateVoipLineNum(top);
	for(i=0 ; i<VOIPLineNum ; i++){
		sprintf(nodeName[1],"Entry%d",i);
		if(getAttrValue(top, nodeName, "SIPEnable", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
				enablenum++;
			}
		}
	}
	if(enablenum > 0){
		if(pre_sys_state.voip_service == 1){
			//restart service
			//tcdbg_printf("!!!!!!!!restart Mtf !!!!!!\n");		
			fp = fopen("/etc/restart_voip","w");
			if(fp != NULL){
				fputc('r',fp);
				fclose(fp);
			}
		
		}else{
			//start service
			//tcdbg_printf("!!!!!!!!start Mtf !!!!!!\n");
			remove("/etc/restart_voip");
			system("/userfs/bin/TC_MTF &");
			pre_sys_state.voip_service = 1;
		}
	}else{
		if(pre_sys_state.voip_service == 1){
			//stop service
			//tcdbg_printf("!!!!!!!!stop Mtf !!!!!!\n");
			pre_sys_state.voip_service = 0;
			fp = fopen("/etc/restart_voip","w");
			if(fp != NULL){
				fputc('s',fp);
				fclose(fp);
			}
		}
	}
	parentNode = mxmlFindElement(top, top, DEVICEINFO,
						NULL, NULL, MXML_DESCEND);
	if(parentNode==NULL){
	     parentNode =mxmlNewElement(top,  DEVICEINFO);
	}
	remove("/etc/sip_status");
	//reset connect status
    mxmlElementSetAttr(parentNode, "ConnStat", "Not Connected");
	
#ifdef TCSUPPORT_VOIP_IMSSIP

//VoIPBasic
 send_priary_outband_proxy_ip_2voip();
 send_priary_outband_proxy_port_2voip();
       send_heart_beat_2voip();
       send_sip_transaction_type_2voip();

       for(i=0;i<NUM_CHANNELS;i++)
       {
          send_line_enable(i);
          send_auth_name_2voip(i);   //line : start from 0 
          send_auth_passwd_2voip(i);
          send_telnum_2voip(i);

//VoIPCallCtrl
          send_call_forward_type_2voip(i);
          send_hook_flash_min_time_2voip(i);
          send_hook_flash_max_time_2voip(i);

//VoIPMedia
         send_codec_priority_2voip(i);

         send_codec_pkg_time_2voip(i);
         send_voice_volume_speak_2voip(i);

         send_voice_volume_listen_2voip(i);
         send_echo_cancel_enable_2voip(i);
         send_call_waiting_enable_2voip(i);
 
        }
//VoIPAdvanced
 send_dtmf_mode_2voip();
 send_fax_mode_2voip();
        
         send_polarity_reverse_2voip() ;
         send_pack_enable_2voip();
         send_reg_expire_2voip();
         send_reg_retry_timer_2voip();
         send_dns_serv_2voip();


         send_rtp_port_range_max_2voip();
         send_rtp_port_range_min_2voip();

         send_digit_long_timer_2voip();
         send_digit_short_timer_2voip();
         send_digit_start_timer_2voip();
         send_busytone_time_timer_2voip();
         send_hangingremindertone_timeout_timer_2voip();
#endif
	
	return SUCCESS;
}

void voip_put_str(FILE *fp, char *name, char *value){
	char tmp[256];
	
	sprintf(tmp,"%s=%s\n",name,value);
	fputs(tmp,fp);
}
int voip_get_payload_id(char *name){
	int ret=0;
	
	if(strcmp(name,"G.711 a-law") == 0){
		ret=8;
	}else if(strcmp(name,"G.711 u-law") == 0){
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
	}else{
		ret = -1;
		fprintf(stderr,"unknown codec\n");
	}
	return ret;
}

void
put_call_ctrl(FILE *fp,mxml_node_t *top){
	
// 	int i;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[100]/*, tmp2[100]*/;

	//fputs("\n[CallControll]\n",fp);
	/*call control feature.*/
	//for(i=0 ; i<VOIP_LINE_NUM ; i++){
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
// 		sprintf(nodeName[1],"Entry%d",i);
		strcpy(nodeName[1],"Entry");
		if(getAttrValue(top, nodeName, "SIPCallerIdEnable", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
				voip_put_str(fp,CALLERIDENABLE,"1");
			}else{
				voip_put_str(fp,CALLERIDENABLE,"0");
			}	
		}else{
			voip_put_str(fp,CALLERIDENABLE,"0");
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
		if(getAttrValue(top, nodeName, "SIPCallWaitingEnable", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
// 				sprintf(tmp2,"%s%d",CALLWAITINGENABLE,i);
// 				voip_put_str(fp,tmp2,"1");
				voip_put_str(fp,CALLWAITINGENABLE,"1");
			}else{
// 				sprintf(tmp2,"%s%d",CALLWAITINGENABLE,i);
// 				voip_put_str(fp,tmp2,"0");
				voip_put_str(fp,CALLWAITINGENABLE,"0");
			}	
		}else{
// 			sprintf(tmp2,"%s%d",CALLWAITINGENABLE,i);
// 			voip_put_str(fp,tmp2,"0");
			voip_put_str(fp,CALLWAITINGENABLE,"0");
		}
		if(getAttrValue(top, nodeName, "SIPCallWaitingReply", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,"Ringing") == 0){
				voip_put_str(fp,CALLWAITINGREPLY,"180");
			}else{
				voip_put_str(fp,CALLWAITINGREPLY,"182");
			}		
		}else{
			voip_put_str(fp,CALLWAITINGREPLY,"182");
		}
		if(getAttrValue(top, nodeName, "SIPCallForwardEnable", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
// 				sprintf(tmp2,"%s%d",CALLFORWARDENABLE,i);
// 				voip_put_str(fp,tmp2,"1");
				voip_put_str(fp,CALLFORWARDENABLE,"1");
// 				getAttrValue(top, nodeName, "SIPCallForwardType", tmp);
// 				sprintf(tmp2,"%s%d",CALLFORWARDTYPE,i);
// 				voip_put_str(fp,tmp2,tmp);
				voip_put_str(fp,CALLFORWARDTYPE,tmp);
// 				getAttrValue(top, nodeName, "SIPCallForwardingNumber", tmp);
// 				sprintf(tmp2,"%s%d",CALLFORWARDINGNUMBER,i);
// 				voip_put_str(fp,tmp2,tmp);
				voip_put_str(fp,CALLFORWARDINGNUMBER,tmp);
			}else{
// 				sprintf(tmp2,"%s%d",CALLFORWARDENABLE,i);
// 				voip_put_str(fp,tmp2,"0");
				voip_put_str(fp,CALLFORWARDENABLE,"0");
// 				sprintf(tmp2,"%s%d",CALLFORWARDTYPE,i);
// 				voip_put_str(fp,tmp2,"0");
				voip_put_str(fp,CALLFORWARDTYPE,"0");
// 				sprintf(tmp2,"%s%d",CALLFORWARDINGNUMBER,i);
// 				voip_put_str(fp,tmp2,"");
				voip_put_str(fp,CALLFORWARDINGNUMBER,"");
			}	
		}else{
// 			sprintf(tmp2,"%s%d",CALLFORWARDENABLE,i);
// 			voip_put_str(fp,tmp2,"0");
// 			sprintf(tmp2,"%s%d",CALLFORWARDTYPE,i);
// 			voip_put_str(fp,tmp2,"0");
// 			sprintf(tmp2,"%s%d",CALLFORWARDINGNUMBER,i);
// 			voip_put_str(fp,tmp2,"");
			voip_put_str(fp,CALLFORWARDENABLE,"0");
			voip_put_str(fp,CALLFORWARDTYPE,"0");
			voip_put_str(fp,CALLFORWARDINGNUMBER,"");
		}
	
		if(getAttrValue(top, nodeName, "SIPCallTransfer", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
// 				sprintf(tmp2,"%s%d",CALLTRANSFER,i);
// 				voip_put_str(fp,tmp2,"1");
				voip_put_str(fp,CALLTRANSFER,"1");
			}else{
// 				sprintf(tmp2,"%s%d",CALLTRANSFER,i);
// 				voip_put_str(fp,tmp2,"0");
				voip_put_str(fp,CALLTRANSFER,"0");
			}	
		}else{
// 			sprintf(tmp2,"%s%d",CALLTRANSFER,i);
// 			voip_put_str(fp,tmp2,"0");
			voip_put_str(fp,CALLTRANSFER,"0");
		}
		if(getAttrValue(top, nodeName, "SIPCallHold", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
// 				sprintf(tmp2,"%s%d",CALLHOLD,i);
// 				voip_put_str(fp,tmp2,"1");
				voip_put_str(fp,CALLHOLD,"1");
			}else{
// 				sprintf(tmp2,"%s%d",CALLHOLD,i);
// 				voip_put_str(fp,tmp2,"0");
				voip_put_str(fp,CALLHOLD,"0");
			}	
		}else{
// 			sprintf(tmp2,"%s%d",CALLHOLD,i);
// 			voip_put_str(fp,tmp2,"0");
			voip_put_str(fp,CALLHOLD,"0");
		}
		if(getAttrValue(top, nodeName, "SIPCallReturn", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
// 				sprintf(tmp2,"%s%d",CALLRETURN,i);
// 				voip_put_str(fp,tmp2,"1");
				voip_put_str(fp,CALLRETURN,"1");
			}else{
// 				sprintf(tmp2,"%s%d",CALLRETURN,i);
// 				voip_put_str(fp,tmp2,"0");
				voip_put_str(fp,CALLRETURN,"0");
			}	
		}else{
// 			sprintf(tmp2,"%s%d",CALLRETURN,i);
// 			voip_put_str(fp,tmp2,"0");
			voip_put_str(fp,CALLRETURN,"0");
		}
		if(getAttrValue(top, nodeName, "SIP3wayConf", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
// 				sprintf(tmp2,"%s%d",THREEWAYCONF,i);
// 				voip_put_str(fp,tmp2,"1");
				voip_put_str(fp,THREEWAYCONF,"1");
			}else{
// 				sprintf(tmp2,"%s%d",THREEWAYCONF,i);
// 				voip_put_str(fp,tmp2,"0");
				voip_put_str(fp,THREEWAYCONF,"0");
			}	
		}else{
// 			sprintf(tmp2,"%s%d",THREEWAYCONF,i);
// 			voip_put_str(fp,tmp2,"0");
			voip_put_str(fp,THREEWAYCONF,"0");
		}
		if(getAttrValue(top, nodeName, "SIPMWIEnable", tmp) == TCAPI_PROCESS_OK){
			if(strcmp(tmp,SIP_YES) == 0){
// 				sprintf(tmp2,"%s%d",MWIENABLE,i);
// 				voip_put_str(fp,tmp2,"1");
				voip_put_str(fp,MWIENABLE,"1");
			}else{
// 				sprintf(tmp2,"%s%d",MWIENABLE,i);
// 				voip_put_str(fp,tmp2,"0");
				voip_put_str(fp,MWIENABLE,"0");
			}	
		}else{
// 			sprintf(tmp2,"%s%d",MWIENABLE,i);
// 			voip_put_str(fp,tmp2,"0");
			voip_put_str(fp,MWIENABLE,"0");
		}
// 	}
}
void
put_media(FILE *fp,mxml_node_t *top,char *local_addr,char *local_rtp_port){

	int i, id;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME], tmp[100], tmp2[100];

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPMEDIA);	
	strcpy(nodeName[1],STRING_COMMON);
	if((getAttrValue(top, nodeName, "VAD", tmp) == TCAPI_PROCESS_OK) && (strcmp(tmp,"Yes") == 0)){
		voip_put_str(fp,"VAD","1");
	}else{
		voip_put_str(fp,"VAD","0");
	}
	if((getAttrValue(top, nodeName, "T38Enable", tmp) == TCAPI_PROCESS_OK) && (strcmp(tmp,"Yes") == 0)){
		voip_put_str(fp,"T38Enable","1");
	}else{
		voip_put_str(fp,"T38Enable","0");
	}

/* serena_modify */	
	if((getAttrValue(top, nodeName, "SilenceCompressionEnable", tmp) == TCAPI_PROCESS_OK) && (strcmp(tmp,"Yes") == 0)){
		voip_put_str(fp,"SilenceCompressionEnable","1");
	}else{
		voip_put_str(fp,"SilenceCompressionEnable","0");
	}
	if((getAttrValue(top, nodeName, "EchoCancellationEnable", tmp) == TCAPI_PROCESS_OK) && (strcmp(tmp,"Yes") == 0)){
		voip_put_str(fp,"EchoCancellationEnable","1");
	}else{
		voip_put_str(fp,"EchoCancellationEnable","0");
	}	
	if((getAttrValue(top, nodeName, "DTMFTransportMode", tmp) == TCAPI_PROCESS_OK) && (strcmp(tmp,"Inband") == 0)){
		voip_put_str(fp,"DTMFTransportMode","0");
	}else if(strcmp(tmp,"RFC2833") == 0){
		voip_put_str(fp,"DTMFTransportMode","1");
	}else{
		voip_put_str(fp,"DTMFTransportMode","2");
	}
/* serena_modify end */
	
	fputs("\n\n[MediaParameters0]\n",fp);
	
	voip_put_str(fp,"v","0");
	sprintf(tmp2, "o= rtp/1 0 0 IN IP4 %s\n",local_addr);
	fputs(tmp2,fp);
	voip_put_str(fp,"s","-");
	//sprintf(tmp2, "c= IN IP4 %s\n",local_addr);
	//fputs(tmp2,fp);
	voip_put_str(fp,"t","0 0");
	sprintf(tmp2, "m=audio %s RTP/AVP",local_rtp_port);

	//memset(nodeName,0,sizeof(nodeName));
	//strcpy(nodeName[0],VOIPMEDIA);
	for(i=0; i<MEDIAENTRY_NUM ; i++){
		sprintf(nodeName[1],"Entry%d",i);
		if(getAttrValue(top, nodeName, "SIPSupportedCodecs", tmp) == TCAPI_PROCESS_OK){
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
/*jrchen modify for DTMF 20100421*/	
	strcpy(nodeName[1],STRING_COMMON);
	if(getAttrValue(top, nodeName, "DTMFTransportMode", tmp) == TCAPI_PROCESS_OK){
		if((strcmp(tmp, "RFC2833") == 0) || (strcmp(tmp, "Via SIP") == 0)){//support RFC2833			
			strcat(tmp2, " 101");
		}
	}
/* jrchen modify end */


	strcat(tmp2,"\n");
	fputs(tmp2,fp);
	
	for(i=0; i<MEDIAENTRY_NUM ; i++){
		sprintf(nodeName[1],"Entry%d",i);
		if(getAttrValue(top, nodeName, "SIPSupportedCodecs", tmp) == TCAPI_PROCESS_OK){
			id = voip_get_payload_id(tmp);
			sprintf(tmp2,"a=rtpmap:%d ",id);
			switch(id) {
				case 0:
					strcat(tmp2,"PCMU/8000\n");
					break;
				case 4:
					strcat(tmp2,"G723/8000\n");
					break;
				case 8:
					strcat(tmp2,"PCMA/8000\n");
					break;
				case 18:
					strcat(tmp2,"G729/8000\n");
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
		if(getAttrValue(top, nodeName, "SIPPacketizationTime", tmp) == TCAPI_PROCESS_OK){
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

/* jrchen modify for DTMF 20100421 */
	strcpy(nodeName[1],STRING_COMMON);
	if(getAttrValue(top, nodeName, "DTMFTransportMode", tmp) == TCAPI_PROCESS_OK){
		if((strcmp(tmp, "RFC2833") == 0) || (strcmp(tmp, "Via SIP") == 0)){			
			strcpy(tmp2,"a=rtpmap:101 telephone-event/8000\n");
			fputs(tmp2,fp);
			strcpy(tmp2,"a=fmtp:101 0-15\n");
			fputs(tmp2,fp);
		}
	}
/* jrchen modify end */
	
	fputs("\n\n[MediaParametersFull]\n",fp);
	
	voip_put_str(fp,"v","0");
	sprintf(tmp2, "o= rtp/1 0 0 IN IP4 %s\n",local_addr);
	fputs(tmp2,fp);
	voip_put_str(fp,"s","-");
	//sprintf(tmp2, "c= IN IP4 %s\n",local_addr);
	//fputs(tmp2,fp);
	voip_put_str(fp,"t","0 0");
	sprintf(tmp2, "m=audio %s RTP/AVP",local_rtp_port);

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPMEDIA);
	for(i=0; i<MEDIAENTRY_NUM ; i++){
		sprintf(nodeName[1],"Entry%d",i);
		if(getAttrValue(top, nodeName, "SIPSupportedCodecs", tmp) == TCAPI_PROCESS_OK){
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
/* jrchen modify for DTMF 20100421 */
	strcpy(nodeName[1],STRING_COMMON);
	if(getAttrValue(top, nodeName, "DTMFTransportMode", tmp) == TCAPI_PROCESS_OK){
		if((strcmp(tmp, "RFC2833") == 0) || (strcmp(tmp, "Via SIP") == 0)){//support RFC2833	
			strcat(tmp2, " 101");
		}
	}
/* jrchen modify end */
	
	strcat(tmp2,"\n");
	fputs(tmp2,fp);
	
	for(i=0; i<MEDIAENTRY_NUM ; i++){
		sprintf(nodeName[1],"Entry%d",i);
		if(getAttrValue(top, nodeName, "SIPSupportedCodecs", tmp) == TCAPI_PROCESS_OK){
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
		if(getAttrValue(top, nodeName, "SIPPacketizationTime", tmp) == TCAPI_PROCESS_OK){
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
/* jrchen modify for DTMF 20100421 */
	strcpy(nodeName[1],STRING_COMMON);
	if(getAttrValue(top, nodeName, "DTMFTransportMode", tmp) == TCAPI_PROCESS_OK){
		if((strcmp(tmp, "RFC2833") == 0) || (strcmp(tmp, "Via SIP") == 0)){			
			strcpy(tmp2,"a=rtpmap:101 telephone-event/8000\n");
			fputs(tmp2,fp);
			strcpy(tmp2,"a=fmtp:101 0-15\n");
			fputs(tmp2,fp);		
		}
	}
/* jrchen modify end */	
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
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPBASIC);
	
	//sip basic username password
	updateVoipLineNum(top);
	for(i=0 ; i<VOIPLineNum ; i++){
		sprintf(nodeName[1],"Entry%d",i);
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
				sprintf(tmp2,"voiceVolume%d",i);				
				if(getAttrValue(top, nodeName, "VoiceVolume", tmp) == TCAPI_PROCESS_OK){
					voip_put_str(fp,tmp2,tmp);
				}else{
					voip_put_str(fp,tmp2,"17");
				}	
/* serena_modify end*/							
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
		}
	}
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
			voip_put_str(fp,SIPTCPENABLED,"1");
		}else{//TCP
			voip_put_str(fp,SIPTRANSPORTTYPE,"1");
			voip_put_str(fp,SIPTCPENABLED,"0");
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
		.type=ENTRY_TYPE|MAXCODEC_NUM,
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

#define NODE_MAX_LEN 32

typedef struct single_node_s
{
	char attr[NODE_MAX_LEN];
	char value[NODE_MAX_LEN];
}single_node_t;

single_node_t digitmap_default_setting[] = {
	{DIGITMAPENABLE, "1"}, 
	{DIGITMAP, "x."},
	{DIALTIMEOUT, "4000"}, 
	{DIGITTIMEOUT, "15000"},
	{"", ""},
};

/*******************************************************************************************
**function name
	voip_digitmap_boot
**description:
	init VoIPDigitMap_Entry node
 **retrun :
	-1: failure
	0:	success
**parameter:
	top :	top pointer in romfile
********************************************************************************************/
int voip_digitmap_boot(mxml_node_t *top){    
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char buf[20] = {0};
	int dml = 0, dms= 0;
	int setFlag = DIGITMAP_SET_FLAG_0;

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],VOIPDIGITMAP);
	strcpy(nodeName[1],VOIPENTRY);
	/*handle InterDigitTimerLong attribute exception*/
	setFlag = DIGITMAP_SET_FLAG_0;
	if(getAttrValue(top, nodeName, DIGITMAP_DML, buf)  != TCAPI_PROCESS_OK){		
		setFlag = DIGITMAP_SET_FLAG_1;
	}else{
		dml = atoi(buf);
		if((dml < DIGITMAP_MIN_DMLS) || (dml > DIGITMAP_MAX_DMLS)){
			setFlag = DIGITMAP_SET_FLAG_1;
		}
	}
	if(setFlag == DIGITMAP_SET_FLAG_1){
		memset(buf, 0, sizeof(buf));
		sprintf(buf,"%d",DIGITMAP_DEFAULT_DML);
		setAttrValue(top, nodeName, DIGITMAP_DML, buf);
	}
	
	/*handle InterDigitTimerShort attribute exception*/
	setFlag = DIGITMAP_SET_FLAG_0;
	if(getAttrValue(top, nodeName, DIGITMAP_DMS, buf)  != TCAPI_PROCESS_OK){		
		setFlag = DIGITMAP_SET_FLAG_1;
	}else{
		dms = atoi(buf);
		if((dms < DIGITMAP_MIN_DMLS) || (dms > DIGITMAP_MAX_DMLS)){
			setFlag = DIGITMAP_SET_FLAG_1;
		}
	}
	if(setFlag == DIGITMAP_SET_FLAG_1){
		memset(buf, 0, sizeof(buf));
		sprintf(buf,"%d",DIGITMAP_DEFAULT_DMS);
		setAttrValue(top, nodeName, DIGITMAP_DMS, buf);
	}
		
    return SUCCESS;
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
		.cfg_boot=voip_digitmap_boot,
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

#endif
