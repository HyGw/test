#include "Global_res.h"
#include "cp_deviceregistration.h"
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
#include "../lib/simcardlib.h"
#endif


#define MAX_USER_NAME_LEN 512
#define MAX_USER_ID_LEN 512

/*Add by lansy*/
#if defined(TCSUPPORT_CT_PON_JX) && !defined(TCSUPPORT_CT_PON_HUB)	
uint8 ResultFlag = 0;
#endif

#if defined(TCSUPPORT_CT_FJ)
extern int g_ITMSRegStartTime;
int g_ITMSServiceStartTime;
int ifSendTimesFlag = 0 ;
#endif

#if defined(TCSUPPORT_CT_ADSL_BIND1)
void saveXBINDResult(int state)
{
	char v_value[256] = {0};
	char bindType[64] = {0};
	char webCurSetNode[32] = "WebCurSet_Entry";
	char deviceAccountNode[32] = "deviceAccount_Entry";

	/* bind succeed. */
	if ( 1 == state )
	{
		if( 0 == tcapi_get(webCurSetNode, ATTR_XBIND_USERNAME, v_value) )
			tcapi_set(deviceAccountNode, ATTR_XBIND_USERNAME, v_value);

		tcapi_set(deviceAccountNode, ATTR_XBIND_RESULT, "SUCC");
	}
	else
		tcapi_set(deviceAccountNode, ATTR_XBIND_RESULT, "REGFAIL");

	return;
}
#endif

/* start of device registration implementations */
int8 getDeviceRegistrationDeviceStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempStatus[16];

	memset( tempStatus, 0, sizeof(tempStatus) );
	tcapi_get( "deviceAccount_Entry", "registerStatus", tempStatus );
	
	return getTheValue(value, type, deviceNode, tempStatus);
}
int8 setDeviceRegistrationDeviceStatusValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 tempStatus = 0;
	char oldStatus[16]={0}, newStatus[16]={0};
#if defined(TCSUPPORT_CT_FJ)
	struct timespec timeSpec;
	char ItmsRegTime[20] = {0};
	int regtime;
#endif
	
	memset( oldStatus, 0, sizeof(oldStatus) );
	memset( newStatus, 0, sizeof(newStatus) );

	tempStatus= *((uint32*)value);
	sprintf( newStatus, "%lu", tempStatus);
	
	tcapi_get( "deviceAccount_Entry", "registerStatus", oldStatus );

	tcdbg_printf( "oldStatus is %s, and newStatus is %s\n", oldStatus,newStatus );

#if defined(TCSUPPORT_CT_ADSL_HN)
	if ( strcmp( "0", newStatus  ) == 0 )
	{
		tcapi_set( "deviceAccount_Entry", "registerResult", "99");
	}
#endif

	/*if port is changed, we should close the old socket*/
	if ( strcmp( oldStatus, newStatus ) != 0 ) {
		
		tcapi_set( "deviceAccount_Entry", "registerStatus", newStatus );
	}

	tcapi_set( "deviceAccount_Entry", "newStatus", "1" );

#if defined(TCSUPPORT_CT_PON)
	if( 0 != strcmp(newStatus, "0"))
		tcapi_set("WebCustom_Entry","NoReboot","0");
#endif

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	if(isCardExist()){
		/* tony add for Sim Card config led */
		if (!strcmp(newStatus, "5")) {/*register ok*/
			simcard_ledconf(SIMCARD_LED_CONFIG, SIMCARD_CONF_SUCCESS);
		}
		else if (!strcmp(newStatus, "0")) {/*start register */
		simcard_ledconf(SIMCARD_LED_CONFIG, SIMCARD_CONF_RUNNING);
		}
		else {/*register fail */
			simcard_ledconf(SIMCARD_LED_CONFIG, SIMCARD_CONF_FAIL);
		}
		tcapi_set("SimCard_Entry","itmsActionStart", "1");
	}
#endif

#if defined(TCSUPPORT_CT_FJ)
if (0 != g_ITMSRegStartTime
	&& 0 == strcmp("0", newStatus)){
	clock_gettime(CLOCK_MONOTONIC,&timeSpec);
	//regtime = timeSpec.tv_sec - g_ITMSRegStartTime + getDevOltRegTime();
	regtime = timeSpec.tv_sec - g_ITMSRegStartTime;
	sprintf(ItmsRegTime,"%d",regtime); 
	tcapi_set("deviceAccount_Entry", "ITMSRegTime", ItmsRegTime);
}
#endif

#if defined(TCSUPPORT_CT_ADSL_TJ)
	if( 0 == strcmp(newStatus, "0") )
	{
		/* clear retry times */
		tcapi_set( "deviceAccount_Entry", "retryTimes", "0");
	}
#endif
#if defined(TCSUPPORT_CT_ADSL_BIND1)
	if( 0 != strcmp(newStatus, "0") )
	{
		/* already registered. */
		if( 0 == strcmp(newStatus, "5") )
			saveXBINDResult(1);
		else
			saveXBINDResult(0);

	}
#endif


	SetCfgNodeFlag("deviceAccount", -1);
	return 0;
}
int8 getDeviceRegistrationDeviceStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceRegistrationAttr.aStatus;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDeviceRegistrationDeviceStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcDeviceRegistrationAttr.aStatus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= CWMP_DEVICEREGISTRATION_ATTR_FLAG;
	return result;
}

#if !defined(TCSUPPORT_CUC)
void saveDeviceInfo()
{
	char v_value[128] = {0};
	char node_name[64] = {0};
	char logic_node[64] = {0};
#if defined(TCSUPPORT_CT_PON_SC)
	char webCurSetNode[32] = "WebCurSet_Entry";
#endif

	strcpy(node_name, "deviceAccount_Entry");
	strcpy(logic_node, "LogicID_Entry");

	memset(v_value, 0, sizeof(v_value) );
	if( 0 == tcapi_get(node_name, DEVACCOUNT_STATUS, v_value) )
		tcapi_set(logic_node, LOGICID_REG_STATUS, v_value);
	memset(v_value, 0, sizeof(v_value) );
	if( 0 == tcapi_get(node_name, DEVACCOUNT_RESULT, v_value) )
		tcapi_set(logic_node, LOGICID_REG_RESULT, v_value);
	memset(v_value, 0, sizeof(v_value) );
	if( 0 == tcapi_get(node_name, DEVACCOUNT_USERNAME, v_value) )
		tcapi_set(logic_node, LOGICID_USERNAME, v_value);
	memset(v_value, 0, sizeof(v_value) );
	if( 0 == tcapi_get(node_name, DEVACCOUNT_USERPASSWD, v_value) )
		tcapi_set(logic_node, LOGICID_PSW, v_value);

#if defined(TCSUPPORT_CT_PON_SC)
	tcapi_set(webCurSetNode, "ITMS_ST", "1");
#endif
}
#endif

int8 getDeviceRegistrationDeviceResultValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempResult[16];

	memset( tempResult, 0, sizeof(tempResult) );

	tcapi_get( "deviceAccount_Entry", "registerResult", tempResult );
	
	return getTheValue(value, type, deviceNode, tempResult);
}

#if defined(TCSUPPORT_CT_FJ)
int getDevOltRegTime(void)
{
	int ret = 0;
	char tmp[20] = {0};
	if(0 == tcapi_get("deviceAccount_Entry", "OltRegTime", tmp))
		ret = atoi(tmp);
	return ret;
}

int getDevITMSRegTime(void)
{
	int ret = 0;
	char tmp[20] = {0};
	if(0 == tcapi_get("deviceAccount_Entry", "ITMSRegTime", tmp))
		ret = atoi(tmp);
	return ret;
}
#endif

int8 setDeviceRegistrationDeviceResultValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 tempResult = 0;
	char oldResult [16]={0}, newResult [16]={0};
	char regStatus[16] = {0};
#if defined(TCSUPPORT_CT_FJ)
	struct timespec timeSpec;
	char ItmsRegTime[20] = {0};
	char ItmsServiceTime[20] = {0};
	int regtime;
#endif
#if defined(TCSUPPORT_ITMS_CONFIG_AS_DEFAULT)
	char bootCmd[128]={0};
#endif

	memset( oldResult , 0, sizeof(oldResult ) );
	memset( newResult , 0, sizeof(newResult ) );

	tempResult = *((uint32*)value);
	sprintf( newResult , "%lu", tempResult );
	
	tcapi_get( "deviceAccount_Entry", "registerResult", oldResult  );

	tcdbg_printf( "oldResult  is %s, and newResult  is %s\n", oldResult ,newResult );

#if defined(TCSUPPORT_CT_FJ)	
	/*state change from offline to online ,send once*/
	if( strcmp("1", newResult) == 0 && strcmp( oldResult, newResult  ) != 0 )
	{
		ifSendTimesFlag = 1 ;
	}
#endif
	
#if defined(TCSUPPORT_ITMS_CONFIG_AS_DEFAULT)
	if(strcmp("1", newResult) == 0){
		tcdbg_printf("\r\nsave romfile to flash: reserve1\r\n");
		memset(bootCmd, 0, sizeof(bootCmd));
		sprintf(bootCmd, TC_FLASH_WRITE_CMD,"/dev/mtdblock1",(unsigned long)getBackupRomfileRaSize(),0, "reserve1");
		system(bootCmd);
	}
#endif
	/*if port is changed, we should close the old socket*/
	if ( strcmp( oldResult, newResult  ) != 0 ) {
		
		tcapi_set( "deviceAccount_Entry", "registerResult", newResult  );

	}
#if defined(TCSUPPORT_CT_ADSL_HN) || defined(TCSUPPORT_CT_FJ)
	if ( strcmp( "1", newResult  ) == 0 ) 
	{
		tcapi_set( "deviceAccount_Entry", "registerStatus", "0" );
	}
#endif
	
#if defined(TCSUPPORT_CT_PON_JX) && !defined(TCSUPPORT_CT_PON_HUB)						
	if ( strcmp( "1", newResult  ) == 0 ) {	
		ResultFlag = 1;
	}else
		ResultFlag = 0;
#endif

	tcapi_set( "deviceAccount_Entry", "newStatus", "1" );
	tcapi_set( "deviceAccount_Entry", "newResult", "1" );

#if defined(TCSUPPORT_CT_PON_GD)
	/* if itms set result to 1, set status to 0 at the same time */
	if ( 0 == strcmp("1", newResult) ) {
		//tcdbg_printf("\r\nsetDeviceRegistrationDeviceResultValue(): set status to 0 when itms set result to 1");
		tcapi_set( "deviceAccount_Entry", "registerStatus", "0" );
	}
#endif

	tcapi_get( "deviceAccount_Entry", "registerStatus", regStatus );

#if !defined(TCSUPPORT_CUC)
	if ( 0 == strcmp("0", regStatus)
		&& 0 == strcmp("1", newResult) )
		saveDeviceInfo();
#endif
#if defined(TCSUPPORT_CT_ADSL_BIND1)
	if ( ( 0 == strcmp("0", regStatus)
		&& 0 == strcmp("1", newResult) ) /* register succeed. */
		/* already registered. */
		|| ( 0 == strcmp(regStatus, "5") ) )
		saveXBINDResult(1);
	else if ( 0 == strcmp("0", regStatus)
		&& 0 == strcmp("2", newResult) ) /* service data error. */
		saveXBINDResult(0);
#endif

#if defined(TCSUPPORT_CT_PON)
	if ( 0 == strcmp("0", regStatus)
		&& 0 == strcmp("0", newResult) ) // no reboot only itms+ is adding service 
		tcapi_set("WebCustom_Entry","NoReboot","1");
	else
		tcapi_set("WebCustom_Entry","NoReboot","0");
#endif

#if defined(TCSUPPORT_CT_FJ)
	if ( 0 == strcmp("0", regStatus)
		&& 0 == strcmp("0", newResult) ){
		clock_gettime(CLOCK_MONOTONIC,&timeSpec);
		g_ITMSServiceStartTime=timeSpec.tv_sec;
	}
	else if(0 != g_ITMSServiceStartTime
		&& 0 == strcmp("0", regStatus)
		&& 0 == strcmp("1", newResult)){
		clock_gettime(CLOCK_MONOTONIC,&timeSpec);
		//regtime = timeSpec.tv_sec - g_ITMSServiceStartTime + getDevITMSRegTime();
		regtime = timeSpec.tv_sec - g_ITMSServiceStartTime;
		sprintf(ItmsServiceTime,"%d",regtime);	
		tcapi_set("deviceAccount_Entry", "ITMSServiceTime", ItmsServiceTime);
	}
#endif

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	if((0 == strcmp("1", newResult) || 0 == strcmp("2", newResult)) && isCardExist()){
		tcapi_set("SimCard_Entry","itmsActionStart", "0");		
		tcapi_set("SimCard_Entry","needWriteSim", "1");
		tcapi_set("SimCard_Entry","needNotifyWrite", "1");
		if(0 == strcmp("1", newResult))//success
			/* tony add for Sim Card config led */
			simcard_ledconf(SIMCARD_LED_CONFIG, SIMCARD_CONF_SUCCESS);
		else if(0 == strcmp("2", newResult))//fail
			/* tony add for Sim Card config led */
			simcard_ledconf(SIMCARD_LED_CONFIG, SIMCARD_CONF_FAIL);
	
	}	
#endif

	SetCfgNodeFlag("deviceAccount", -1);
    cwmpDelayFunction();
	return 0;
}


int8 getDeviceRegistrationDeviceResultAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceRegistrationAttr.aResult;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDeviceRegistrationDeviceResultAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcDeviceRegistrationAttr.aResult;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= CWMP_DEVICEREGISTRATION_ATTR_FLAG;
	return result;
}

 
int8 getDeviceRegistrationRetryTimesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempRetryTimes[16];

	memset( tempRetryTimes, 0, sizeof(tempRetryTimes) );

	tcapi_get( "deviceAccount_Entry", "retryTimes", tempRetryTimes );
	
	return getTheValue(value, type, deviceNode, tempRetryTimes);
}
int8 setDeviceRegistrationRetryTimesValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 tempRetryTimes = 0;
	char oldRetryTimes[16]={0}, newRetryTimes [16]={0};
	
	memset( oldRetryTimes , 0, sizeof(oldRetryTimes ) );
	memset( newRetryTimes , 0, sizeof(newRetryTimes ) );

	tempRetryTimes = *((uint32*)value);
	sprintf( newRetryTimes , "%lu", tempRetryTimes );
	
	tcapi_get( "deviceAccount_Entry", "retryTimes", oldRetryTimes  );

	tcdbg_printf( "oldResult  is %s, and newResult  is %s\n", oldRetryTimes ,newRetryTimes );

	/*if port is changed, we should close the old socket*/
	if ( strcmp( oldRetryTimes, newRetryTimes  ) != 0 ) {
		
		tcapi_set( "deviceAccount_Entry", "retryTimes", newRetryTimes  );

	}
	SetCfgNodeFlag("deviceAccount", -1);
	return 0;
}
int8 getDeviceRegistrationRetryTimesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceRegistrationAttr.aRetryTimes;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDeviceRegistrationRetryTimesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcDeviceRegistrationAttr.aRetryTimes;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= CWMP_DEVICEREGISTRATION_ATTR_FLAG;
	return result;
}




int8 getDeviceRegistrationRetryLimitValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempRetryLimit[16];

	memset( tempRetryLimit, 0, sizeof(tempRetryLimit) );

	tcapi_get( "deviceAccount_Entry", "retryLimit", tempRetryLimit);
	
	return getTheValue(value, type, deviceNode, tempRetryLimit);
}
int8 setDeviceRegistrationRetryLimitValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 tempRetryLimit = 0;
	char oldRetryLimit[16]={0}, newRetryLimit [16]={0};
	
	memset( oldRetryLimit , 0, sizeof(oldRetryLimit ) );
	memset( newRetryLimit , 0, sizeof(newRetryLimit ) );

	tempRetryLimit = *((uint32*)value);
	sprintf( newRetryLimit , "%lu", tempRetryLimit );
	
	tcapi_get( "deviceAccount_Entry", "retryLimit", oldRetryLimit  );

	tcdbg_printf( "oldResult  is %s, and newResult  is %s\n", oldRetryLimit ,newRetryLimit );

	/*if port is changed, we should close the old socket*/
	if ( strcmp( oldRetryLimit, newRetryLimit  ) != 0 ) {
		
		tcapi_set( "deviceAccount_Entry", "retryLimit", newRetryLimit  );

	}
	SetCfgNodeFlag("deviceAccount", -1);
	return 0;
}
int8 getDeviceRegistrationRetryLimitAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceRegistrationAttr.aRetryLimit;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDeviceRegistrationRetryLimitAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcDeviceRegistrationAttr.aRetryLimit;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= CWMP_DEVICEREGISTRATION_ATTR_FLAG;
	return result;
}

#if 1
#if defined(TCSUPPORT_CT_PPPINFORM)
extern char bridge_pppusername[64];
extern int  br_pppconnection_stat;
#endif
int8 getDeviceRegistrationDeviceUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char name[MAX_USER_NAME_LEN] = {0};
#if defined(TCSUPPORT_CT_PPPINFORM)
	if(br_pppconnection_stat == 1)
		strcpy(name, bridge_pppusername);
#else
	tcapi_get( "deviceAccount_Entry", "userName", name  );
#endif
	return getTheValue(value, type, deviceNode, name);
}
int8 setDeviceRegistrationDeviceUserNameValue(void* value, struct DeviceNode*  deviceNode)
{
	char* pName;

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	char cpeFlag[8] = {0};
	tcapi_get( SIM_CARD_NODE_NAME, "cpeFlag", cpeFlag);
	if(strcmp(cpeFlag,"1") == 0){
		return -12;
	}
#endif

	if(strlen((char*)value) > MAX_USER_NAME_LEN -1 )
		return -1;
	
	pName = (char*)value;
	tcapi_set( "deviceAccount_Entry", "userName", pName);
#if defined(TCSUPPORT_CT_PON)	
	tcapi_set( "GPON_LOIDAuth", "LOID", pName);
	tcapi_set( "EPON_LOIDAuth", "LOID0", pName);
#endif
	
#if !defined(TCSUPPORT_CUC)
	tcapi_set( "WebCurSet_Entry", "isTR69Chg", "1");
	tcapi_set( "LogicID_Entry", LOGICID_USERNAME, pName);
	SetCfgNodeFlag("LogicID", -1);
#endif

	SetCfgNodeFlag("deviceAccount", -1);
	
	return 0;
}
int8 getDeviceRegistrationDeviceUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceRegistrationAttr.aUserName;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDeviceRegistrationDeviceUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceRegistrationAttr.aUserName;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}


	
int8 getDeviceRegistrationDeviceUserIdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char pass[MAX_USER_ID_LEN] = {0};
#if defined(TCSUPPORT_CMCC)
	tcapi_get( "GPON_ONU", "Password", pass);
#else
	tcapi_get( "deviceAccount_Entry", "userPasswordDEV", pass  );
#endif

#if defined(TCSUPPORT_CT_PON_GD)
	if(0 == strlen(pass))
	{
		char nameClone[MAX_USER_NAME_LEN] = {0};         // you'd better use MAX_USER_NAME_LEN here
		if( 0 == tcapi_get( "deviceAccount_Entry", "userName", nameClone ) )
		{
			snprintf(pass,sizeof(pass),"%s",nameClone);
		}
		else
		{
			// do nothing
		}
	}
#endif

	return getTheValue(value, type, deviceNode,pass);
}
int8 setDeviceRegistrationDeviceUserIdValue(void* value, struct DeviceNode*  deviceNode)
{
	/* in this function, the value of retry times will increase by 1 */
	char *pPass;
#if 0
	char *retry[16] = {0};
	int retryNext = 0;
	char *tmp[16] = {0};
#endif
		
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	char cpeFlag[8] = {0};
	tcapi_get( SIM_CARD_NODE_NAME, "cpeFlag", cpeFlag);
	if(strcmp(cpeFlag,"1") == 0){
		return -12;
	}
#endif

		
	if(strlen((char*)value) > MAX_USER_NAME_LEN -1 )
		return -1;
	
	pPass = (char*)value;
#if defined(TCSUPPORT_CMCC)
		tcapi_set( "GPON_ONU", "Password", pPass);
#else
		tcapi_set( "deviceAccount_Entry", "userPasswordDEV", pPass);
#if defined(TCSUPPORT_CT_PON)
		tcapi_set( "GPON_LOIDAuth", "Password", pPass);
		tcapi_set( "EPON_LOIDAuth", "Password0", pPass);
#endif
#endif
#if !defined(TCSUPPORT_CUC) && !defined(TCSUPPORT_CMCC)
	tcapi_set( "WebCurSet_Entry", "isTR69Chg", "1");
	tcapi_set( "LogicID_Entry", LOGICID_PSW, pPass);
	SetCfgNodeFlag("LogicID", -1);
#endif

#if 0
	if( 0 == tcapi_get( "deviceAccount_Entry", "retryTimes",retry) )
	{
		retryNext = atoi(retry) + 1;
		sprintf(tmp, "%d", retryNext);
		tcapi_set("deviceAccount_Entry", "retryTimes", tmp);
	}
#endif	

	SetCfgNodeFlag("deviceAccount", -1);
	
	return 0;
}
int8 getDeviceRegistrationDeviceUserIdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceRegistrationAttr.aUserId;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDeviceRegistrationDeviceUserIdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceRegistrationAttr.aUserId;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getServiceNumValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_serviceNum[16] = {0};

	if ( tcapi_get("deviceAccount_Entry", "serviceNum", s_serviceNum) < 0 )
		strcpy(s_serviceNum, "0");

	return getTheValue(value, type, deviceNode, s_serviceNum);
}
int8 setServiceNumValue(void* value, struct DeviceNode*  deviceNode)
{
	int i_serviceNum = 0;
	char s_serviceNum[16] = {0};

	i_serviceNum = *((int*)value);
	sprintf(s_serviceNum, "%d", i_serviceNum);
	tcapi_set("deviceAccount_Entry", "serviceNum", s_serviceNum);

	SetCfgNodeFlag("deviceAccount", -1);
	return 0;
}
int8 getServiceNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDeviceRegistrationAttr.aServiceNum;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setServiceNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcDeviceRegistrationAttr.aServiceNum;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CWMP_DEVICEREGISTRATION_ATTR_FLAG;
	return result;
}

int8 getServiceNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_serviceName[512] = {0};

	if ( tcapi_get("deviceAccount_Entry", "serviceName", s_serviceName) < 0 )
		s_serviceName[0] = '\0';

	return getTheValue(value, type, deviceNode, s_serviceName);
}
int8 setServiceNameValue(void* value, struct DeviceNode*  deviceNode)
{
	char s_serviceName[512] = {0};

	if(strlen((char*)value) > 511 )
		return -1;

	snprintf(s_serviceName, sizeof(s_serviceName) -1, "%s", (char *)value);
	tcapi_set("deviceAccount_Entry", "serviceName", s_serviceName);

	SetCfgNodeFlag("deviceAccount", -1);
	return 0;
}
int8 getServiceNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDeviceRegistrationAttr.aServiceName;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setServiceNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcDeviceRegistrationAttr.aServiceName;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CWMP_DEVICEREGISTRATION_ATTR_FLAG;
	return result;
}

#endif

#endif
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_DEVREG_PROV_CODE)
int8 getRegProvisioningCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_ProvisioningCode[512] = {0};

	if ( tcapi_get("deviceAccount_Entry", "ProvisioningCode", s_ProvisioningCode) < 0 )
		s_ProvisioningCode[0] = '\0';

	return getTheValue(value, type, deviceNode, s_ProvisioningCode);
}

int8 setRegProvisioningCodeValue(void* value, struct DeviceNode*  deviceNode)
{
	char s_ProvisioningCode[512] = {0};

	if(strlen((char*)value) > 511 )
		return -1;

	snprintf(s_ProvisioningCode, sizeof(s_ProvisioningCode) -1, "%s", (char *)value);
	tcapi_set("deviceAccount_Entry", "ProvisioningCode", s_ProvisioningCode);

	SetCfgNodeFlag("deviceAccount", -1);
	return 0;
}

int8 getRegProvisioningCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDeviceRegistrationAttr.aProvisioningCode;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setRegProvisioningCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcDeviceRegistrationAttr.aProvisioningCode;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CWMP_DEVICEREGISTRATION_ATTR_FLAG;
	return result;
}

#endif

#if defined(TCSUPPORT_CT_ADSL_HN)
int8 getRouteStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char result[5] = {0};

	tcapi_get("Wan_Common", "ShowEnable", tempValue);
	
	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "0");
	else
		strcpy(result, "1");

	return getTheValue(value, type, deviceNode, result);
}

int8 setRouteStatusValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 iEnable;
	int instance;
	char strEnable[10] = {0};
	char nodeName[30] = {0};

	iEnable = *((uint8*)value);
	
	if(0 == iEnable)
		strcpy(strEnable, "Yes");
	else if(1 == iEnable)
		strcpy(strEnable, "No");
	else 
		return -1;
	
	tcapi_set("Wan_Common", "ShowEnable", strEnable);

	return 0;
}

int8 getRouteStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDeviceRegistrationAttr.aRouteStatus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setRouteStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcDeviceRegistrationAttr.aRouteStatus;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CWMP_DEVICEREGISTRATION_ATTR_FLAG;
	return result;
}
#endif
/* end of device registration implementations */


