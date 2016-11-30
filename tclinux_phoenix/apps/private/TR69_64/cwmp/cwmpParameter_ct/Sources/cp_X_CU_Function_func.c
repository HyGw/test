#include "Global_res.h"
#include "cp_X_CU_Function.h"


#define MAX_ARG_NUM	3
#define MAX_NODE_NAME 32


int8 getAdminNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{

	char  name[20] = {'\0'};

	tcapi_get(ADMIN_NODE, ADMINNAME, name);
	
	return getTheValue(value, type, deviceNode, name);
}

int8 getAdminNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = cuAccountAttr.aAdminName;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setAdminNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &cuAccountAttr.aAdminName;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CUACCOUNT_ATTR_FLAG;
	return result;
}

int8 getAdminPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  psw[36] = {'\0'};
	
	tcapi_get(ADMIN_NODE, ADMINPSW, psw);
	
	return getTheValue(value, type, deviceNode, psw);
}
int8 setAdminPasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	char *psw = NULL;
	
	psw = (char*)value;

	SetCfgNodeFlag( "Account", -1);
	
	if(strlen(psw) > 32){
		tc_printf("==>setPasswordValue: the length of password should not larger than 32.\n");
		return -1;
	}	
	tcapi_set(ADMIN_NODE, ADMINPSW, psw);
	
	return 0;
	
}
int8 getAdminPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = cuAccountAttr.aAdminPasswd;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setAdminPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &cuAccountAttr.aAdminPasswd;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CUACCOUNT_ATTR_FLAG;
	return result;
}
int8 getAdminWebIPValue(char * * value, ParameterType * type, DeviceNode * deviceNode)
{

	char  webip[64] = {'\0'};
	char  ip[20] ={'\0'};

	tcapi_get("Lan_Entry0", "IP", ip);
	sprintf(webip, "http://%s/cu.html",ip);
	
	return getTheValue(value, type, deviceNode, webip);
}
int8 getAdminWebIPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = cuAccountAttr.aAdminWebIP;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setAdminWebIPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &cuAccountAttr.aAdminWebIP;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CUACCOUNT_ATTR_FLAG;
	return result;
}

int8 getUserNameValue(char * * value, ParameterType * type, DeviceNode * deviceNode)
{

	char  name[20] = {'\0'};

	tcapi_get(USER_NODE, USERNAME, name);
	
	return getTheValue(value, type, deviceNode, name);
}

int8 getUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = cuAccountAttr.aUserName;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &cuAccountAttr.aUserName;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CUACCOUNT_ATTR_FLAG;
	return result;
}

int8 getUserPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  psw[36] = {'\0'};
	
	tcapi_get(USER_NODE, USERPSW, psw);
	
	return getTheValue(value, type, deviceNode, psw);
}
int8 getUserPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = cuAccountAttr.aUserPasswd;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setUserPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &cuAccountAttr.aUserPasswd;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CUACCOUNT_ATTR_FLAG;
	return result;
}

int8 getUserWebIPValue(char * * value, ParameterType * type, DeviceNode * deviceNode)
{

	char  webip[64] = {'\0'};
	char  ip[20] ={'\0'};

	tcapi_get("Lan_Entry0", "IP", ip);
	sprintf(webip, "http://%s",ip);
	
	return getTheValue(value, type, deviceNode, webip);
}

int8 getUserWebIPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = cuAccountAttr.aUserWebIP;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setUserWebIPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &cuAccountAttr.aUserWebIP;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CUACCOUNT_ATTR_FLAG;
	return result;
}

int8   RebuildAlarmTable()
{
	uint32 number;
	DeviceNodePtr AlarmLog = NULL;
	char *objectName = "InternetGatewayDevice.X_CU_Function.Log.AlarmInfo";
	int rtnCode = 0;
	DeviceNodePtr rootptr = NULL;

	
	
	if( rpc.IGDDeviceRootPtr == NULL)
		return -1;
	
	pthread_mutex_lock(&value_change_lock);
	if(inform_task_start == 1){
		pthread_mutex_unlock(&value_change_lock);
		return -1;
	}else{
		rootptr = rpc.IGDDeviceRootPtr;
		rpc.IGDDeviceRootPtr = NULL;
		pthread_mutex_unlock(&value_change_lock);
	}
		
	cwmpSearchDeviceNodeByName(objectName, rootptr, &AlarmLog);
	if(AlarmLog == NULL){
		pthread_mutex_unlock(&value_change_lock);
		return -1;
	}
	
	cwmpFreeDeviceTable(AlarmLog);
	rtnCode = cwmpInitAlarmInfoSubTree(AlarmLog);

	pthread_mutex_lock(&value_change_lock);
	rpc.IGDDeviceRootPtr = rootptr;	
	pthread_mutex_unlock(&value_change_lock);

	
	return rtnCode;
}


int	cwmpInitAlarmInfoSubTree( DeviceNodePtr deviceNode )
{
	int rtnCode = 0;
	char tmp[16] = {0};
	int iNum = 0;
	
	tcapi_get("AlarmLog_Common", "AlarmNum", tmp);
	iNum = atoi(tmp);
	rtnCode = RegisterDynNodeFromIndex( deviceNode, AlarmInfoIndex, iNum );
	
	return rtnCode;



}

int8 getLogEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	tcapi_get("SysLog_Entry", "LogEnable", tempValue);
	if(strcmp(tempValue, "Yes") == 0)
	{
		strcpy(tempValue, "1"); 
	}
	else
	{
		strcpy(tempValue, "0"); 
	}
	return getTheValue(value, type, deviceNode,tempValue);
}

int8 setLogEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 *enable = NULL;
	char tmpBuf[8] = {0};
	enable = (uint8*)value;
		
	if( *enable == 1 )
		strcpy(tmpBuf, "Yes");
	else if( *enable == 0 )
		strcpy(tmpBuf, "No");
	else
		return -1;
	tcapi_set("SysLog_Entry", "LogEnable", tmpBuf);	
	SetCfgNodeFlag( "SysLog", -1 );
	return 0;
}

int8 getLogEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = cuLogAttr.aLogEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLogEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &cuLogAttr.aLogEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CULOG_ATTR_FLAG ;
	return result;
}


int8 getLogLevelValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	
	tcapi_get("SysLog_Entry", "WriteLevel", tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setLogLevelValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 level = 0;
	char tmpBuf[8] = {0};
	
	level = *(uint32*)value;
	if(level > 7)
		return -1;
	sprintf(tmpBuf, "%d", level);
	tcapi_set("SysLog_Entry", "WriteLevel", tmpBuf);	
	SetCfgNodeFlag( "SysLog", -1 );
	
	return 0;
	
}
int8 getLogLevelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = cuLogAttr.aLogLevel;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}

int8 setLogLevelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &cuLogAttr.aLogLevel;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CULOG_ATTR_FLAG ;
	return result;
}

int8 getLogUploadEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	
	tcapi_get("SysLog_Entry", "LogUploadEnable", tempValue);
	if(strcmp(tempValue, "Yes") == 0){
		strcpy(tempValue, "1"); 
	}
	else{
	strcpy(tempValue, "0"); 
	}
	return getTheValue(value, type, deviceNode, tempValue);
	
}

int8 setLogUploadEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 *enable = NULL;
	char tmpBuf[8] = {0};
	enable = (uint8*)value;
	
	if( *enable == 1 )
		strcpy(tmpBuf, "Yes");
	else if( *enable == 0 )
		strcpy(tmpBuf, "No");
	else
		return -1;
	tcapi_set("SysLog_Entry", "LogUploadEnable", tmpBuf);	
	SetCfgNodeFlag( "SysLog", -1 );
	return 0;
	
}
int8 getLogUploadEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = cuLogAttr.aLogUploadEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLogUploadEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &cuLogAttr.aLogUploadEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CULOG_ATTR_FLAG ;
	return result;
}

int8 getLogUploadIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[16] = {0};
	
	tcapi_get("SysLog_Entry", "LogUploadInterval", tempValue);
	return getTheValue(value, type, deviceNode, tempValue);

}

int8 setLogUploadIntervalValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 interval = 0;
	char tmpBuf[16] = {0};
	
	interval = *(uint32*)value;
	sprintf(tmpBuf, "%ld", interval);
	tcapi_set("SysLog_Entry", "LogUploadInterval", tmpBuf);	
	SetCfgNodeFlag( "SysLog", -1 );
	return 0;
}

int8 getLogUploadIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{

	uint8  attribute;
	int8  result;
	
	attribute = cuLogAttr.aLogUploadInterval;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLogUploadIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &cuLogAttr.aLogUploadInterval;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CULOG_ATTR_FLAG ;
	return result;
}


int8 getAlarmEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	
	tcapi_get("SysLog_Entry", "AlarmEnable", tempValue);
	if(strcmp(tempValue, "Yes") == 0)
	{
		strcpy(tempValue, "1"); 
	}
	else
	{
		strcpy(tempValue, "0"); 
	}
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setAlarmEnableValue(void* value, struct DeviceNode*  deviceNode)
{

	uint8 *enable = NULL;
	char tmpBuf[8] = {0};
	
	enable = (uint8*)value;
	if( *enable == 1 )
		strcpy(tmpBuf, "Yes");
	else if( *enable == 0 )
		strcpy(tmpBuf, "No");
	else
		return -1;
	tcapi_set("SysLog_Entry", "AlarmEnable", tmpBuf);	
	
	SetCfgNodeFlag( "SysLog", -1 );
	
	return 0;
}

int8 getAlarmEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = cuLogAttr.aAlarmEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setAlarmEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &cuLogAttr.aAlarmEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CULOG_ATTR_FLAG ;
	return result;
}


int8 getAlarmLevelValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	
	tcapi_get("SysLog_Entry", "AlarmLevel", tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setAlarmLevelValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 level = 0;
	char tmpBuf[8] = {0};
	
	level = *(uint32*)value;
	if(level > 3)
	{
		return -1;
	}
	sprintf(tmpBuf, "%d", level);
	tcapi_set("SysLog_Entry", "AlarmLevel", tmpBuf);	

	SetCfgNodeFlag( "SysLog", -1 );
	
	return 0;
	
}
int8 getAlarmLevelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = cuLogAttr.aAlarmLevel;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setAlarmLevelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &cuLogAttr.aAlarmLevel;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CULOG_ATTR_FLAG ;
	return result;
}


int8 getUploadServerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  server[32] = {0};
	
	tcapi_get("SysLog_Entry", "UploadServer", server);
	return getTheValue(value, type, deviceNode, server);
	
}

int8 setUploadServerValue(void* value, struct DeviceNode*  deviceNode)
{
	char *server = NULL;
	
	server = (char*)value;
	if(strlen(server) > 16)
	{
		return -1;
	}	
	tcapi_set("SysLog_Entry", "UploadServer", server);
	SetCfgNodeFlag( "SysLog", -1 );
	return 0;
	
}

int8 getUploadServerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = cuLogAttr.aUploadServer;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setUploadServerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &cuLogAttr.aUploadServer;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CULOG_ATTR_FLAG ;
	return result;
}

int8 getCuLogUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  name[132] = {0};
	
	tcapi_get("SysLog_Entry", "Username", name);
	
	return getTheValue(value, type, deviceNode, name);
}

int8 setCuLogUsernameValue(void* value, struct DeviceNode*  deviceNode)
{
	char *name = NULL;
	
	name = (char*)value;
	if(strlen(name) > 128)
	{
		return -1;
	}	
	tcapi_set("SysLog_Entry", "Username", name);
	SetCfgNodeFlag( "SysLog", -1 );
	
	return 0;
	
}

int8 getCuLogUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = cuLogAttr.aUsername;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCuLogUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &cuLogAttr.aUsername;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CULOG_ATTR_FLAG ;
	return result;
}

int8 getCuLogPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  password[132] = {0};
	
	tcapi_get("SysLog_Entry", "Password", password);
	return getTheValue(value, type, deviceNode, password);

}

int8 setCuLogPasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	char *password = NULL;
	
	password = (char*)value;
	if(strlen(password) > 128)
	{
		return -1;
	}	
	tcapi_set("SysLog_Entry", "Password", password);
	SetCfgNodeFlag( "SysLog", -1 );
	
	return 0;
}
int8 getCuLogPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = cuLogAttr.aPassword;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCuLogPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &cuLogAttr.aPassword;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CULOG_ATTR_FLAG ;
	return result;
}

int8 getNumberOfAlarmInfoValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	
	tcapi_get("SysLog_Entry", "NumberOfAlarmInfo", tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}


int8 setNumberOfAlarmInfoValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 number = 0;
	char tmpBuf[8] = {0};
	
	number = *(uint32*)value;
	if(number > 100)
	{
		return -1;
	}
	sprintf(tmpBuf, "%d", number);
	tcapi_set("SysLog_Entry", "NumberOfAlarmInfo", tmpBuf);	
	SetCfgNodeFlag( "SysLog", -1 );
	return 0;
}

int8 getNumberOfAlarmInfoAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = cuLogAttr.aNumberOfAlarmInfo;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setNumberOfAlarmInfoAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &cuLogAttr.aNumberOfAlarmInfo;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CULOG_ATTR_FLAG ;
	return result;
}


//////////////////////////////////////////////////////////
int8 getAlarmIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[16]={0};
	char entryName[32]={0};
	int  entryId = atoi(deviceNode->parentNode->name)-1;	
	
		
	sprintf(entryName,"AlarmLog_Entry%d",entryId);
	
	tcapi_get(entryName,"AlarmID",tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getAlarmIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
/*
	uint8  attribute;
	int8  result;
	int  instance;
	instance = atoi(deviceNode->parentNode->name);		
	attribute = cuLogAttr.aAlarmID[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
*/
	return 0;
}

int8 setAlarmIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
/*
	int instance;
	uint8*  attribute = NULL;
	int8  result;
	instance = atoi(deviceNode->parentNode->name);		
	attribute = &cuLogAttr.aAlarmID[instance-1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CULOG_ATTR_FLAG ;
	return result;
*/
	return 0;
}

int8 getAlarmCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[16]={0};
	char entryName[32]={0};
	int  entryId = atoi(deviceNode->parentNode->name)-1;	
	
	
	sprintf(entryName,"AlarmLog_Entry%d",entryId);
	
	tcapi_get(entryName,"AlarmCode",tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getAlarmCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
/*
	uint8  attribute;
	int8  result;
	int  instance;
	instance = atoi(deviceNode->parentNode->name);		
	attribute = cuLogAttr.aAlarmCode[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
*/
	return 0;	
}

int8 setAlarmCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
/*
	int instance;
	uint8*  attribute = NULL;
	int8  result;
	instance = atoi(deviceNode->parentNode->name);		
	attribute = &cuLogAttr.aAlarmCode[instance-1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CULOG_ATTR_FLAG ;
	return result;
*/
	return 0;
}

int8 getAlarmStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[16]={0};
	char entryName[32]={0};
	int  entryId = atoi(deviceNode->parentNode->name)-1;	
	

	sprintf(entryName,"AlarmLog_Entry%d",entryId);
	
	tcapi_get(entryName,"AlarmStatus",tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getAlarmStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
/*
	uint8  attribute;
	int8  result;
	int  instance;
	instance = atoi(deviceNode->parentNode->name);		
	attribute = cuLogAttr.aAlarmStatus[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
*/
	return 0;	
}

int8 setAlarmStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
/*
	int instance;
	uint8*  attribute = NULL;
	int8  result;
	instance = atoi(deviceNode->parentNode->name);		
	attribute = &cuLogAttr.aAlarmStatus[instance-1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CULOG_ATTR_FLAG ;
	return result;
*/
	return 0;
}

int8 getPerceivedSeverityValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[16]={0};
	char entryName[32]={0};
	int  entryId = atoi(deviceNode->parentNode->name)-1;	
	
	sprintf(entryName,"AlarmLog_Entry%d",entryId);
	
	tcapi_get(entryName,"PerceivedSeverity",tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getPerceivedSeverityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
/*
	uint8  attribute;
	int8  result;
	int  instance;
	instance = atoi(deviceNode->parentNode->name);		
	attribute = cuLogAttr.aPerceivedServerity[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
*/
	return 0;
}

int8 setPerceivedSeverityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{/*

	int instance;
	uint8*  attribute = NULL;
	int8  result;
	instance = atoi(deviceNode->parentNode->name);		
	attribute = &cuLogAttr.aPerceivedServerity[instance-1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CULOG_ATTR_FLAG ;
	return result;
*/
	return 0;
}

int8 getAlarmRaisedTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[64]={0};
	char entryName[32]={0};
	int  entryId = atoi(deviceNode->parentNode->name)-1;	
	
	sprintf(entryName,"AlarmLog_Entry%d",entryId);
	
	tcapi_get(entryName,"AlarmRaisedTime",tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
	
}

int8 getAlarmRaisedTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
/*	
	uint8  attribute;
	int8  result;
	int  instance;
	tc_printf("==>getAlarmIDAttribute.\n");
	instance = atoi(deviceNode->parentNode->name);		
	attribute = cuLogAttr.aAlarmRaisedTime[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
*/
	return 0;
}

int8 setAlarmRaisedTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
/*
	int instance;
	uint8*  attribute = NULL;
	int8  result;
	instance = atoi(deviceNode->parentNode->name);		
	attribute = &cuLogAttr.aAlarmRaisedTime[instance-1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CULOG_ATTR_FLAG ;
	return result;
*/
	return 0;
}

int8 getAlarmClearedTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{

	char tempValue[64]={0};
	char entryName[32]={0};
	int  entryId = atoi(deviceNode->parentNode->name)-1;	
	
	sprintf(entryName,"AlarmLog_Entry%d",entryId);
	
	tcapi_get(entryName,"AlarmClearedTime",tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getAlarmClearedTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
/*
	uint8  attribute;
	int8  result;
	int  instance;
	instance = atoi(deviceNode->parentNode->name);		
	attribute = cuLogAttr.aAlarmClearedTime[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
*/
	return 0;
}

int8 setAlarmClearedTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
/*
	int instance;
	uint8*  attribute = NULL;
	int8  result;
	instance = atoi(deviceNode->parentNode->name);		
	attribute = &cuLogAttr.aAlarmClearedTime[instance-1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CULOG_ATTR_FLAG ;
	return result;
*/	
	return 0;
}

int8 getAlarmDetailValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257]={0};
	char entryName[32]={0};
	int  entryId = atoi(deviceNode->parentNode->name)-1;	
	
	sprintf(entryName,"AlarmLog_Entry%d",entryId);
	
	tcapi_get(entryName,"AlarmDetail",tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);

}

int8 getAlarmDetailAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
/*
	uint8  attribute;
	int8  result;
	int  instance;
	instance = atoi(deviceNode->parentNode->name);		
	attribute = cuLogAttr.aAlarmDetail[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
*/
	return 0;
}

int8 setAlarmDetailAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
/*	
	int instance;
	uint8*  attribute = NULL;
	int8  result;
	instance = atoi(deviceNode->parentNode->name);		
	attribute = &cuLogAttr.aAlarmDetail[instance-1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CULOG_ATTR_FLAG ;
	return result;
*/
	return 0;
}
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getAdminaccountChangeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char flag[8] = {0};
	tcapi_get("Cwmp_Entry", "account_change_flag",flag);
	
	return getTheValue(value, type, deviceNode, flag);	
}
int8 setAdminaccountChangeValue(void* value, struct DeviceNode*  deviceNode)
{
	int flag = *((int*)value);
	if(flag == 1)
	{
		tcapi_set("Cwmp_Entry","account_change_flag", "1");
	}
	else if(flag == 0)
	{
		tcapi_set("Cwmp_Entry","account_change_flag", "0");
	}
	else
		return -1;
	SetCfgNodeFlag("Cwmp", -1);
	
	return 0;		
}
int8 getAdminaccountChangeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = cuAccountAttr.aAdminaccountChange;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setAdminaccountChangeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &cuAccountAttr.aAdminaccountChange;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CUACCOUNT_ATTR_FLAG;
	return result;
}

int8 getPPPoEDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char state[20] = {0};
	tcapi_get("PPPoESimulate_Entry", "DiagnosticsState",state);
	
	return getTheValue(value, type, deviceNode, state);	
}
int8 setPPPoEDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode)
{
	char state[20] = {0};
	if(strcasecmp((char*)value, "None")
		&& strcasecmp((char*)value, "Start")
		&& strcasecmp((char*)value, "Stop")
		&& strcasecmp((char*)value, "Complete")
		&& strcasecmp((char*)value, "Running")		
	)
	{
		return -1;

	}

	strcpy(state, (char*)value);
	tcapi_set("PPPoESimulate_Entry","DiagnosticsState", state);
	
	SetCfgNodeFlag("PPPoESimulate", -1);
	
	return 0;		
}

int8 getPPPoEDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aPPPoEDiagState;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPPPoEDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aPPPoEDiagState;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getPPPoEDiagUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char username[72] = {0};
	tcapi_get("PPPoESimulate_Entry", "Username",username);
	
	return getTheValue(value, type, deviceNode, username);	
}
int8 setPPPoEDiagUsernameValue(void* value, struct DeviceNode*  deviceNode)
{
	char username[72] = {0};
	
	if(strlen((char*)value)>64)
		return -1;
	
	strcpy(username,(char*)value);
	tcapi_set("PPPoESimulate_Entry","Username", username);
	
	return 0;		
}
int8 getPPPoEDiagUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aPPPoEDiagUsername;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPPPoEDiagUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aPPPoEDiagUsername;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getPPPoEDiagPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char passwd[72] = {0};
	tcapi_get("PPPoESimulate_Entry", "Password", passwd);
	
	return getTheValue(value, type, deviceNode, passwd);	
}
int8 setPPPoEDiagPasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	char passwd[72] = {0};
	
	if(strlen((char*)value)>64)
		return -1;
	
	strcpy(passwd,(char*)value);
	tcapi_set("PPPoESimulate_Entry","Password", passwd);
	
	return 0;		
}
int8 getPPPoEDiagPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aPPPoEDiagPassword;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPPPoEDiagPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aPPPoEDiagPassword;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getPPPoEDiagVLANIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char vid[20] = {0};
	tcapi_get("PPPoESimulate_Entry", "VLANID",vid);
	
	return getTheValue(value, type, deviceNode, vid);	
}
int8 setPPPoEDiagVLANIDValue(void* value, struct DeviceNode*  deviceNode)
{
	char vidStr[20] = {0};
	uint32 vid = *((uint32*)value);
	
	if(vid < 1 || vid > 4094)
		return -1;
	
	sprintf(vidStr, "%d", vid);
	tcapi_set("PPPoESimulate_Entry","VLANID", vidStr);
	
	return 0;		
}
int8 getPPPoEDiagVLANIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aPPPoEDiagVLANID;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPPPoEDiagVLANIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aPPPoEDiagVLANID;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getPPPoEDiagPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char port[20] = {0};
	tcapi_get("PPPoESimulate_Entry", "Port",port);
	
	return getTheValue(value, type, deviceNode, port);	
}
int8 setPPPoEDiagPortValue(void* value, struct DeviceNode*  deviceNode)
{
	char portStr[20] = {0};
	uint32 port = *((uint32*)value);
	
	if(port < 1 || port > 63365)
		return -1;
	
	sprintf(portStr, "%d", port);
	tcapi_set("PPPoESimulate_Entry","Port", portStr);
	
	return 0;		
}
int8 getPPPoEDiagPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aPPPoEDiagPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPPPoEDiagPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aPPPoEDiagPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getPPPoEDiagAuthProtocolValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char protocol[20] = "CHAP";
	int ret;
	
	if((ret=tcapi_get("PPPoESimulate_Entry", "PPPAuthenticationProtocol",protocol))!=0)
		strcpy(protocol,"CHAP");
	
	
	return getTheValue(value, type, deviceNode, protocol);	
}
int8 setPPPoEDiagAuthProtocolValue(void* value, struct DeviceNode*  deviceNode)
{
	char protocol[20] = {0};
	
	if(strcmp((char*)value,"PAP") && strcmp((char*)value,"CHAP"))
		return -1;
	
	strcpy(protocol, (char*)value);
	tcapi_set("PPPoESimulate_Entry","PPPAuthenticationProtocol", protocol);
	
	return 0;		
}
int8 getPPPoEDiagAuthProtocolAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aPPPoEDiagAuthProtocol;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPPPoEDiagAuthProtocolAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aPPPoEDiagAuthProtocol;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getPPPoEDiagRetryTimesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char retryTime[20] = {0};
	int ret;
	if((ret=tcapi_get("PPPoESimulate_Entry", "RetryTimes",retryTime))!=0)
		strcpy(retryTime,"1");
	
	return getTheValue(value, type, deviceNode, retryTime);	
}
int8 setPPPoEDiagRetryTimesValue(void* value, struct DeviceNode*  deviceNode)
{
	char retryTimeStr[20] = {0};
	uint32 retryTime = *((uint32*)value);
	if(retryTime < 1 || retryTime > 3)
		return -1;
	
	sprintf(retryTimeStr, "%d", retryTime);
	tcapi_set("PPPoESimulate_Entry","RetryTimes", retryTimeStr);
	
	return 0;		
}
int8 getPPPoEDiagRetryTimesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aPPPoEDiagRetryTimes;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPPPoEDiagRetryTimesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aPPPoEDiagRetryTimes;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getPPPoEDiagResultValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[40] = {0};
	int ret;
	if((ret=tcapi_get("PPPoESimulate_Entry", "Result",result))!=0)
		strcpy(result,"Success");
	
	return getTheValue(value, type, deviceNode, result);	
}
int8 getPPPoEDiagResultAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aPPPoEDiagResult;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPPPoEDiagResultAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aPPPoEDiagResult;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getPPPoEDiagSessionIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char sid[20] = {0};
	tcapi_get("PPPoESimulate_Entry", "PPPSessionID",sid);
	
	return getTheValue(value, type, deviceNode, sid);	
}
int8 setPPPoEDiagSessionIDValue(void* value, struct DeviceNode*  deviceNode)
{
	char sidStr[20] = {0};
	uint32 sid = *((uint32*)value);
	
	sprintf(sidStr, "%d", sid);
	tcapi_set("PPPoESimulate_Entry","PPPSessionID", sidStr);
	
	return 0;		
}
int8 getPPPoEDiagSessionIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aPPPoEDiagSessionID;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPPPoEDiagSessionIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aPPPoEDiagSessionID;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getPPPoEDiagExtIPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ipaddr[20] = {0};
	tcapi_get("PPPoESimulate_Entry", "ExternalIPAddress",ipaddr);
	
	return getTheValue(value, type, deviceNode, ipaddr);	
}
int8 setPPPoEDiagExtIPValue(void* value, struct DeviceNode*  deviceNode)
{
	char ipaddr[20] = {0};
	char* tempValue = NULL;	

	tempValue = (char*)(value);
	if(strlen(tempValue) > 15){//IP is xxx.xxx.xxx.xxx, the max length is 15
		return -1;
	}
	
	if( InternalClientIpCheck(tempValue) != 1 ){
		tc_printf("\n==>Invalid PPPoE emluator IP\n");
		return -1;
	}
	
	strcpy(ipaddr, tempValue);
	tcapi_set("PPPoESimulate_Entry","ExternalIPAddress", ipaddr);
	
	return 0;		
}
int8 getPPPoEDiagExtIPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aPPPoEDiagExternalIPAddress;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPPPoEDiagExtIPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aPPPoEDiagExternalIPAddress;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getPPPoEDiagDefGWValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char gwaddr[20] = {0};
	tcapi_get("PPPoESimulate_Entry", "DefaultGateway",gwaddr);
	
	return getTheValue(value, type, deviceNode, gwaddr);	
}
int8 setPPPoEDiagDefGWValue(void* value, struct DeviceNode*  deviceNode)
{
	char gwaddr[20] = {0};
	char* tempValue = NULL;	

	tempValue = (char*)(value);
	if(strlen(tempValue) > 15){//IP is xxx.xxx.xxx.xxx, the max length is 15
		return -1;
	}
	
	if( InternalClientIpCheck(tempValue) != 1 ){
		tc_printf("\n==>Invalid PPPoE emluator gateway\n");
		return -1;
	}
	
	strcpy(gwaddr, tempValue);
	tcapi_set("PPPoESimulate_Entry","DefaultGateway", gwaddr);
	
	return 0;		
}
int8 getPPPoEDiagDefGWAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aPPPoEDiagDefaultGateway;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPPPoEDiagDefGWAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aPPPoEDiagDefaultGateway;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

static int8 PPPoEDiagCheckWanInterface(char* setvalue, char* nodeName)
{
	char DefaultIface_WAN[]="InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";
	char tempvalue[2];
	int len;
	int pvcnode, pvctmp;	
	DeviceNodePtr Node = NULL;
	int entrynode;
	char *tmp = NULL;
	char tmpstr[16] = {0};
	char vlanMode[16] = {0};
		
	len = strlen(DefaultIface_WAN);

	if(strncmp(DefaultIface_WAN, setvalue, len) == 0){//WAN interface, get the gateway.
		/*check the value is valid or not.*/
		cwmpSearchDeviceNodeByName(setvalue, rpc.IGDDeviceRootPtr, &Node);
		if(Node == NULL){
			tc_printf("\r\nparameter.c---[setPppoeEmulatorWanInterfaceValue]---Invalid Value!");
			return -1;
		}

		tempvalue[0] = setvalue[len];
		tempvalue[1] = '\0';
		
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
		pvcnode = atoi(tempvalue);
		pvctmp = pvcnode;
		if(FindPVCMapping(&pvcnode) != 0){
			tc_printf("\r\nparameter.c---[setPppoeEmulatorWanInterfaceValue]---Invalid pvc number!");
			return -1;
		}
#else		
		if(setvalue[len]<'1'||setvalue[len]>'8'){
			tc_printf("\r\nparameter.c---[setPppoeEmulatorWanInterfaceValue]---Invalid pvc number!");
			return -1;
		}

		pvcnode = atoi(tempvalue) - 1;
#endif	
		if((tmp = strstr(setvalue, "WANPPPConnection")) != NULL){
			memset(tempvalue, 0, sizeof(tempvalue));
			tempvalue[0] = *(tmp + strlen("WANPPPConnection") + 1);
			tempvalue[1] = '\0';
		#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
			entrynode = atoi(tempvalue);
			FindWanNodeMapping(&pvctmp, &entrynode);
        #else	 	
			entrynode = atoi(tempvalue) - 1;
		#endif
			sprintf(nodeName,"Wan_PVC%d_Entry%d", pvcnode, entrynode);	
			tcapi_get(nodeName, "VLANID", tmpstr);
		
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
			tcapi_set("PppoeEmulator_Entry", "VLANID", tmpstr);
			
			tcapi_get(nodeName, "VLANMode", vlanMode);
			tcapi_set("PppoeEmulator_Entry", "VLANMode", vlanMode);
#else
		   tcapi_get(nodeName, "VLANMode", vlanMode);
		   tcapi_set("PPPoESimulate_Entry", "VLANMode", vlanMode);
		   
		   if(!strcmp(vlanMode,"TAG") || !strcmp(vlanMode,"tag"))
           tcapi_set("PPPoESimulate_Entry", "VLANID", tmpstr);

#endif
		}
		else
		{
			return -1;
		}
	}
	else
	{//error value
		return -1;
	}	
	
	return 0;
}


int8 getPPPoEDiagWanInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempWanInterface[512] = {0};
	
	tcapi_get("PPPoESimulate_Entry", "WANInterface",tempWanInterface);
		
	return getTheValue(value, type, deviceNode, tempWanInterface);

}
int8 setPPPoEDiagWanInterfaceValue(void* value, struct DeviceNode*  deviceNode)
{
	char* tempValue = NULL;	
	char nodeName[64] = {0};
   int8  ret = 0;
   
	tempValue = (char*)(value);


	tcapi_set("PPPoESimulate_Entry","WANInterface", tempValue);

	memset(nodeName, 0, sizeof(nodeName));
	ret = PPPoEDiagCheckWanInterface(tempValue,nodeName);

	return ret;	
}
int8 getPPPoEDiagWanInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aPPPoEDiagWanInterface;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPPPoEDiagWanInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aPPPoEDiagWanInterface;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getPPPoEDiagLanInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempWanInterface[512] = {0};;
	
	if(tcapi_get("PPPoESimulate_Entry", "LANInterface",tempWanInterface) <0)
		strcpy(tempWanInterface," ");
				
	return getTheValue(value, type, deviceNode, tempWanInterface);

}
int8 setPPPoEDiagLanInterfaceValue(void* value, struct DeviceNode*  deviceNode)
{	
	char* tempValue = NULL;	
	char nodeName[64] = {0};
    int8  ret = 0;
   
	tempValue = (char*)(value);

	tcapi_set("PPPoESimulate_Entry","LANInterface", tempValue);

	return ret;
		
}
int8 getPPPoEDiagLanInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aPPPoEDiagLanInterface;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPPPoEDiagLanInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aPPPoEDiagLanInterface;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}



int8 getFWPacketofAPPFilterEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char fw_status[4] = {0};	
	char level[16] = {0};
	char appFilterEnable[4] = {0};
	
	tcapi_get("Firewall_Entry", "firewall_status",fw_status);
	tcapi_get("Firewall_Entry","firewall_level", level);
	if(!strcmp(fw_status,"0") || !strcmp(level,"low"))
		strcpy(appFilterEnable,"0");
	else
		strcpy(appFilterEnable,"1");	
	
	return getTheValue(value, type, deviceNode, appFilterEnable);	
}
int8 setFWPacketofAPPFilterEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char level[16] = {0};
	uint8 enbl = *((uint8*)value);

	tcapi_get("Firewall_Entry","firewall_level", level);

	if(enbl == 1)
	{
		tcapi_set("Firewall_Entry", "firewall_status","1");
		if(!strcmp(level,"low"))
			tcapi_set("Firewall_Entry", "firewall_level","medium");
	}
	else
	{
		tcapi_set("Firewall_Entry", "firewall_status","0");
	}	
	
	SetCfgNodeFlag("Firewall", -1);
	
	return 0;		
}
int8 getFWPacketofAPPFilterEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aPacketofAPPFilterEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setFWPacketofAPPFilterEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aPacketofAPPFilterEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getFWPortScanEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char fw_status[4] = {0};
	char level[16] = {0};
	char portScan[4] = {0};
	
	tcapi_get("Firewall_Entry", "firewall_status",fw_status);
	tcapi_get("Firewall_Entry","firewall_level", level);

	if(!strcmp(fw_status,"0") || !strcmp(level,"low"))
		strcpy(portScan,"0");
	else
		strcpy(portScan,"1");
	
	return getTheValue(value, type, deviceNode, portScan);	
}
int8 setFWPortScanEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char level[16] = {0};
	uint8 enbl = *((uint8*)value);

	tcapi_get("Firewall_Entry","firewall_level", level);

	if(enbl == 1)
	{
		tcapi_set("Firewall_Entry", "firewall_status","1");
		if(!strcmp(level,"low"))
			tcapi_set("Firewall_Entry", "firewall_level","medium");
	}
	else
	{
		tcapi_set("Firewall_Entry", "firewall_status","0");
	}	
	
	SetCfgNodeFlag("Firewall", -1);
	
	return 0;		
}
int8 getFWPortScanEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aPortScanEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setFWPortScanEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aPortScanEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getFWInvalidPktDenyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char fw_status[4] = {0};
	char level[16] = {0};
	char invidPktDeny[4] = {0};
	
	tcapi_get("Firewall_Entry", "firewall_status",fw_status);
	tcapi_get("Firewall_Entry","firewall_level", level);

	if(!strcmp(fw_status,"0") || !strcmp(level,"low"))
		strcpy(invidPktDeny,"0");
	else
		strcpy(invidPktDeny,"1");
	
	return getTheValue(value, type, deviceNode, invidPktDeny);	
}
int8 setFWInvalidPktDenyValue(void* value, struct DeviceNode*  deviceNode)
{
	char level[16] = {0};
	uint8 enbl = *((uint8*)value);

	tcapi_get("Firewall_Entry","firewall_level", level);

	if(enbl == 1)
	{
		tcapi_set("Firewall_Entry", "firewall_status","1");
		if(!strcmp(level,"low"))
			tcapi_set("Firewall_Entry", "firewall_level","medium");
	}
	else
	{
		tcapi_set("Firewall_Entry", "firewall_status","0");
	}	
	
	SetCfgNodeFlag("Firewall", -1);
	
	return 0;		
}
int8 getFWInvalidPktDenyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aInvalidPacketDenyEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setFWInvalidPktDenyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aInvalidPacketDenyEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getFWDoSEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char fw_status[4] = {0};
	char level[16] = {0};
	char DoSEnable[4] = {0};
	
	tcapi_get("Firewall_Entry", "firewall_status",fw_status);
	tcapi_get("Firewall_Entry","firewall_level", level);

	if(!strcmp(fw_status,"0") || !strcmp(level,"low"))
		strcpy(DoSEnable,"0");
	else
		strcpy(DoSEnable,"1");
	
	return getTheValue(value, type, deviceNode, DoSEnable);	
}
int8 setFWDoSEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char level[16] = {0};
	uint8 enbl = *((uint8*)value);

	tcapi_get("Firewall_Entry","firewall_level", level);

	if(enbl == 1)
	{
		tcapi_set("Firewall_Entry", "firewall_status","1");
		if(!strcmp(level,"low"))
			tcapi_set("Firewall_Entry", "firewall_level","medium");
	}
	else
	{
		tcapi_set("Firewall_Entry", "firewall_status","0");
	}	
	
	SetCfgNodeFlag("Firewall", -1);
	
	return 0;		
}
int8 getFWDoSEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aDoSEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setFWDoSEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aDoSEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getDdnsEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ddns_enbale[12] = "0";
	char nodeValue[32] = {0};

	tcapi_get("Ddns_tr69Entry","Active", nodeValue);
	
	if(!strcmp(nodeValue, "Yes"))
		strcpy(ddns_enbale,"1");
	else
		strcpy(ddns_enbale,"0");
	
	return getTheValue(value, type, deviceNode, ddns_enbale);	
}
int8 setDdnsEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char nodeValue[32] = {0};
	char ifname[20] = {0};
	int pvc, entry, ddns_entry=-1;
	
	uint8 enbl = *((uint8*)value);
	
	if(enbl == 1)
	{
		strcpy(nodeValue, "Yes");
	}
	else if (enbl == 0)
	{
		strcpy(nodeValue, "No");
	}	
	else
		return -1;
	
	tcapi_set("Ddns_tr69Entry","Active", nodeValue);

	tcapi_set("Ddns_Common", "tr69Commit", "1");
	SetCfgNodeFlag ( "Ddns", 0);
	return 0;		
}
int8 getDdnsEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aDdnsEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDdnsEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aDdnsEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getDdnsProviderValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ddns_provider[64] = "CU";
	char nodeValue[64] = {0};

	tcapi_get("Ddns_tr69Entry","DDNS_Name", nodeValue);	

	if(nodeValue[0]!='\0')
		strcpy(ddns_provider, nodeValue);
	
	return getTheValue(value, type, deviceNode, ddns_provider);	
}
int8 setDdnsProviderValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char nodeValue[64] = {0};
	char ifname[20] = {0};
	int pvc, entry, ddns_entry=-1;
	int i, len;
	char *p = (char*)value;	
	
	len = strlen(p);
	if(len >= 64)
		return -1;

	for(i=0;i<len;i++)
	{
		if(*(p+i)<0 || *(p+i)>255)
			return -1;
	}

	strcpy(nodeValue,(char*)value);
	tcapi_set("Ddns_tr69Entry","DDNS_Name", nodeValue);
	
	tcapi_set("Ddns_Common", "tr69Commit", "1");
	SetCfgNodeFlag ( "Ddns", 0);
	return 0;		
}
int8 getDdnsProviderAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aDdnsProvider;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDdnsProviderAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aDdnsProvider;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getDdnsUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ddns_username[64] = {0};
	char nodeValue[64] = {0};
	
	tcapi_get("Ddns_tr69Entry","USERNAME", nodeValue);

	if(nodeValue[0]!='\0')
		strcpy(ddns_username, nodeValue);
	
	return getTheValue(value, type, deviceNode, ddns_username);	
}
int8 setDdnsUsernameValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char nodeValue[64] = {0};
	char ifname[20] = {0};
	int pvc, entry, ddns_entry=-1;
	int i, len;
	char *p = (char*)value;	
	
	len = strlen(p);
	if(len > 32)
		return -1;

	for(i=0;i<len;i++)
	{
		if(*(p+i)<0 || *(p+i)>255)
			return -1;
	}

	strcpy(nodeValue,(char*)value);
	tcapi_set("Ddns_tr69Entry","USERNAME", nodeValue);	
	
	tcapi_set("Ddns_Common", "tr69Commit", "1");
	SetCfgNodeFlag ( "Ddns", 0);
	return 0;		
}
int8 getDdnsUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aDdnsUsername;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDdnsUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aDdnsUsername;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getDdnsPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ddns_passwd[64] = {0};
	char nodeValue[64] = {0};
	
	tcapi_get("Ddns_tr69Entry","PASSWORD", nodeValue);

	if(nodeValue[0]!='\0')
		strcpy(ddns_passwd, nodeValue);
	
	return getTheValue(value, type, deviceNode, ddns_passwd);	
}
int8 setDdnsPasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char nodeValue[64] = {0};
	char ifname[20] = {0};
	int pvc, entry, ddns_entry=-1;
	int i, len;
	char *p = (char*)value;	
	
	len = strlen(p);
	if(len > 32)
		return -1;

	for(i=0;i<len;i++)
	{
		if(*(p+i)<0 || *(p+i)>255)
			return -1;
	}

	strcpy(nodeValue,(char*)value);
	tcapi_set("Ddns_tr69Entry","PASSWORD", nodeValue);	
	
	tcapi_set("Ddns_Common", "tr69Commit", "1");
	SetCfgNodeFlag ( "Ddns", 0);
	return 0;		
}
int8 getDdnsPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aDdnsPassword;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDdnsPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aDdnsPassword;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getDdnsServicePortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ddns_passwd[64] = {0};
	char nodeValue[64] = {0};
	
	tcapi_get("Ddns_tr69Entry","SERVICEPORT", nodeValue);

	if(nodeValue[0]!='\0')
		strcpy(ddns_passwd, nodeValue);
	
	return getTheValue(value, type, deviceNode, ddns_passwd);	
}
int8 setDdnsServicePortValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char nodeValue[64] = {0};
	char ifname[20] = {0};
	int pvc, entry, ddns_entry=-1;
	int port = 0;

	port = atoi((char*)value);

	if(port < 1 || port > 63365)
		return -1;
	
	sprintf(nodeValue, "%d", port);
	
	tcapi_set("Ddns_tr69Entry","SERVICEPORT", nodeValue);	
	
	tcapi_set("Ddns_Common", "tr69Commit", "1");
	SetCfgNodeFlag ( "Ddns", 0);
	return 0;		
}
int8 getDdnsServicePortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aDdnsServicePort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDdnsServicePortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aDdnsServicePort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getDdnsDomainNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ddns_domain[64] = {0};
	char nodeValue[64] = {0};
	
	tcapi_get("Ddns_tr69Entry", "DDNS_Domain",nodeValue);

	if(nodeValue[0]!='\0')
		strcpy(ddns_domain, nodeValue);
	
	return getTheValue(value, type, deviceNode, ddns_domain);	
}
int8 setDdnsDomainNameValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char nodeValue[64] = {0};
	char ifname[20] = {0};
	int pvc, entry, ddns_entry=-1;
	int i, len;
	char *p = (char*)value;	
	
	len = strlen(p);
	if(len >= 64)
		return -1;

	for(i=0;i<len;i++)
	{
		if(*(p+i)<0 || *(p+i)>255)
			return -1;
	}

	strcpy(nodeValue,(char*)value);
	tcapi_set("Ddns_tr69Entry","DDNS_Domain", nodeValue);
	
	tcapi_set("Ddns_Common", "tr69Commit", "1");
	SetCfgNodeFlag ( "Ddns", 0);
	return 0;		
}
int8 getDdnsDomainNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aDdnsDomainName;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDdnsDomainNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aDdnsDomainName;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getDdnsHostNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ddns_host[64] = {0};
	char nodeValue[64] = {0};
	
	tcapi_get("Ddns_tr69Entry", "MYHOST",nodeValue);

	if(nodeValue[0]!='\0')
		strcpy(ddns_host, nodeValue);
	
	return getTheValue(value, type, deviceNode, ddns_host);	
}
int8 setDdnsHostNameValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char nodeValue[64] = {0};
	char ifname[20] = {0};
	int pvc, entry, ddns_entry=-1;
	int i, len;
	char *p = (char*)value;	
	
	len = strlen(p);
	if(len >= 64)
		return -1;

	for(i=0;i<len;i++)
	{
		if(*(p+i)<0 || *(p+i)>255)
			return -1;
	}

	strcpy(nodeValue,(char*)value);
	tcapi_set("Ddns_tr69Entry","MYHOST", nodeValue);

	tcapi_set("Ddns_Common", "tr69Commit", "1");
	SetCfgNodeFlag ( "Ddns", 0);
	return 0;		
}
int8 getDdnsHostNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aDdnsHostName;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDdnsHostNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aDdnsHostName;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getDdnsWANPathValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ddns_wanpath[128] = {0};
	char nodeName[32] = {0};
	char ifname[20] = {0};
	char tmpStr[8] = {0};
	uint32 pvc, entry;
	int ddns_entry=-1;
	int pppflag = 0;
	
	if(tcapi_get("Ddns_tr69Entry", "DDNS_Interface", ifname) == 0)
	{
		if(strstr(ifname, "nas"))
		{
			sscanf(ifname, "nas%d", &ddns_entry);
			pvc = ddns_entry/MAX_WAN_NODE;
			entry = ddns_entry%MAX_WAN_NODE;			
		}
		else if(strstr(ifname, "ppp"))
		{
			sscanf(ifname, "ppp%d", &ddns_entry);
			pvc = ddns_entry/MAX_WAN_NODE;
			entry = ddns_entry%MAX_WAN_NODE;
			pppflag = 1;
		}

		if(ddns_entry != -1)
		{	
				strcpy(ddns_wanpath, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.");
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
				GetWanNodeMapping(&pvc, &entry);
				sprintf(tmpStr, "%d.", pvc);
#else	
				sprintf(tmpStr, "%d.", pvc+1);
#endif
				strcat(ddns_wanpath, tmpStr);
				if(pppflag == 1)
					strcat(ddns_wanpath, "WANPPPConnection.");
				else
					strcat(ddns_wanpath, "WANIPConnection.");
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
				sprintf(tmpStr, "%d.", entry);
#else	
				sprintf(tmpStr, "%d", entry+1);
#endif					
				strcat(ddns_wanpath, tmpStr);
		}	
	}
	
	return getTheValue(value, type, deviceNode, ddns_wanpath);	
}
int8 setDdnsWANPathValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char nodeValue[64] = {0};
	char DefaultIface_WAN[]="InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";
	char tempvalue[2];
	char *setvalue = NULL;
	DeviceNodePtr Node = NULL;
	int len = 0, ddns_entry;
	uint32 pvcnode,entrynode;	
	char *tmp = NULL;	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	uint32 pvctmp;
#endif
	
	setvalue=(char*)value;
	len = strlen(DefaultIface_WAN);

	if(strncmp(DefaultIface_WAN, setvalue, len) != 0)
		return -1;	
	//WAN interface check the value is valid or not.
	cwmpSearchDeviceNodeByName(setvalue, rpc.IGDDeviceRootPtr, &Node);
	if(Node == NULL){
		tc_printf("\r\n---[setDdnsWANPathValue]---Invalid Value!");
		return -1;
	}

	tempvalue[0] = setvalue[len];
	tempvalue[1] = '\0';
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcnode = atoi(tempvalue);
	pvctmp = pvcnode;
	if(FindPVCMapping(&pvcnode) != 0){
		tc_printf("\r\n---[setDdnsWANPathValue]---Invalid pvc number!");
		return -1;
	}
#else
	if(setvalue[len]<'1'||setvalue[len]>'8'){
		tc_printf("\r\n---[setDdnsWANPathValue]---Invalid pvc number!");
		return -1;
	}
	pvcnode = atoi(tempvalue) - 1;
#endif
	
	if((tmp = strstr(setvalue, "WANPPPConnection")) != NULL){
		memset(tempvalue, 0, sizeof(tempvalue));
		tempvalue[0] = *(tmp + strlen("WANPPPConnection") + 1);
		tempvalue[1] = '\0';
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
		entrynode = atoi(tempvalue);
		FindWanNodeMapping(&pvctmp, &entrynode);
#else	
		entrynode = atoi(tempvalue) - 1;
#endif
	}
	else if((tmp = strstr(setvalue, "WANIPConnection")) != NULL){
		memset(tempvalue, 0, sizeof(tempvalue));
		tempvalue[0] = *(tmp + strlen("WANIPConnection") + 1);
		tempvalue[1] = '\0';
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
		entrynode = atoi(tempvalue);
		FindWanNodeMapping(&pvctmp, &entrynode);
#else	
		entrynode = atoi(tempvalue) - 1;
#endif
	}
	else{
		tc_printf("\r\n---[setDdnsWANPathValue]---Invalid wan path!");
		return -1;
	}
		
	
	sprintf(nodeName,"Wan_PVC%d_Entry%d", pvcnode, entrynode);
	tcapi_get(nodeName,"IFName", nodeValue);
	tcapi_set("Ddns_tr69Entry", "DDNS_Interface", nodeValue);	

	ddns_entry = pvcnode<<3+entrynode;
	sprintf(nodeValue,"%d", ddns_entry);
	tcapi_set("Ddns_Common", "tr69SetEntry", nodeValue);

	tcapi_set("Ddns_Common", "tr69Commit", "1");
	SetCfgNodeFlag ( "Ddns", 0);
	
	return 0;		
}
int8 getDdnsWANPathAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aDdnsWANPath;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDdnsWANPathAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aDdnsWANPath;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getdmzEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char dmz_enbale[12] = "0";
	char nodeValue[12] = {0};
	tcapi_get("Dmz_Common", "Active", nodeValue);
	if(strcmp(nodeValue,"Yes"))
		strcpy(dmz_enbale, "0");
	else
		strcpy(dmz_enbale, "1");

	return getTheValue(value, type, deviceNode, dmz_enbale);	
}
int8 setdmzEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 enbl = *((uint8*)value);
	
	if(enbl == 1)		
		tcapi_set("Dmz_Common", "Active", "Yes");
	else if(enbl == 0)
		tcapi_set("Dmz_Common", "Active", "No");
	else		
		return -1;
	
	SetCfgNodeFlag( "Dmz", -1);
	
	return 0;		
}
int8 getdmzEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aDmzEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setdmzEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aDmzEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getdmzHostIPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char host[20] = {0};
	
	tcapi_get("Dmz_Common", "DMZ_IP", host);
	
	return getTheValue(value, type, deviceNode, host);	
}
int8 setdmzHostIPValue(void* value, struct DeviceNode*  deviceNode)
{	
	if(_checkIp((char*)value) < 0)	
		return -1;
	tcapi_set("Dmz_Common", "DMZ_IP", (char*)value);
	
	SetCfgNodeFlag( "Dmz", -1);
	
	return 0;		
}
int8 getdmzHostIPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aDmzHostIP;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setdmzHostIPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCUAttr.aDmzHostIP;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getFactoryResetModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[24] = {0};
	
	if ( 0 != tcapi_get("Sys_Entry", "FacResetMode", tempValue)
		|| 0 == tempValue[0] )
		strcpy(tempValue, "1");

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setFactoryResetModeValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 level = 0;
	char tmpBuf[24] = {0};
	
	level = *(uint32*)value;

	sprintf(tmpBuf, "%lu", level);
	tcapi_set("Sys_Entry", "FacResetMode", tmpBuf);	

	return 0;
	
}
int8 getFactoryResetModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute;
	int8 result;

	attribute = tcCUAttr.aFactoryResetMode;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}

int8 setFactoryResetModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result;

	attribute = &tcCUAttr.aFactoryResetMode;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

#endif
