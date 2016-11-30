#include "Global_res.h"
#include "cp_Services_ftp.h"


int8 getX_CU_FTPSvrUserRightValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char right[32] = {0};
	
	if(tcapi_get("Account_FtpEntry","ftp_right", right)<0)
		strcpy(right, "READONLY");

	return getTheValue(value, type, deviceNode, right);
}
int8 setX_CU_FTPSvrUserRightValue(void* value, struct DeviceNode*  deviceNode)
{
	
	if(strcasecmp((char*)value, "READONLY")
		&& strcasecmp((char*)value, "READWRITE")
		&& strcasecmp((char*)value, "ADMINISTRATOR")
	)
		return -1;
	
	tcapi_set("Account_FtpEntry", "ftp_right", (char*)value);
	
	//SetCfgNodeFlag( "Account", -1);
	return 0;
}
int8 getX_CU_FTPSvrUserRightAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aFTPSvrUserRight;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_FTPSvrUserRightAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcCUAttr.aFTPSvrUserRight;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_FTPSvrPathValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char path[32] = {0};

	if(tcapi_get("Account_FtpEntry","ftp_server_path", path)<0)
		strcpy(path, "/mnt/dev/usb1_1");
	
	return getTheValue(value, type, deviceNode, path);
}
int8 setX_CU_FTPSvrPathValue(void* value, struct DeviceNode*  deviceNode)
{
	char* tempValue = NULL; 
	
	tempValue = (char*)(value);
	
	if(strlen(tempValue) > 31){
			return -1;
		}

	tcapi_set("Account_FtpEntry", "ftp_server_path",tempValue);

	return 0;
}
int8 getX_CU_FTPSvrPathAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aFTPSvrPath;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_FTPSvrPathAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcCUAttr.aFTPSvrPath;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

/*Acs is deleteing FTPClient object*/
int8 DeviceNodeDelFTPClientObject(uint32 num, DeviceNode *deviceNode)
{
	char nodeName[32] = {0};;
	char val[8];
	uint32 del_index, i, ret = 0;

	DeviceNodePtr FTPClientNode = NULL;
	char *objectName = "InternetGatewayDevice.Services.X_CU_FTPFTPClient.";

	tcdbg_printf("Deleting FTPClientNode!\n");
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &FTPClientNode);
	if (FTPClientNode == NULL) {
		return -1;
	}
	del_index = num - 1;

	if(del_index >= APPFTPCLIENT_NUM || del_index < 0)
	{
		tcdbg_printf("%s:Invalid  FTPClient index.\n",__func__);
		return -1;
	}
	
	/* check if the Entry is exist */
	sprintf(nodeName, "AppFTP_Entry%d", (del_index + APPFTPHISTORY_NUM));	
	/* this Entry is not exist, just return */
	ret = tcapi_get(nodeName, "Enable", val);
	if (ret < 0) {
		return ret;
	}


	tcapi_unset(nodeName);
	
	cwmpFreeDeviceTable(FTPClientNode);
	ret = cwmpInitFTPClientTable(FTPClientNode);
	
	return ret;
}

/*Acs is adding FTPClient object*/
int8 FTPClientNodeAddObject(uint32 *num, DeviceNode*  deviceNode)
{
	char add_index[10] = {0};
	char nodeValue[10] = {0};
	char nodeName[32] = {0};
	int ret = 0, i;
	
	DeviceNodePtr FTPClientNode = NULL;
	char *objectName = "InternetGatewayDevice.Services.X_CU_FTPFTPClient.";

	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &FTPClientNode);
	if (NULL == FTPClientNode) {
		tcdbg_printf("top node not found!\n");
		return -1;
	}
	
	for(i=APPFTPHISTORY_NUM; i<MAX_APPFTP_ENTRY; i++)
	{
		sprintf(nodeName, "appFTP_Entry%d", i);
		if(tcapi_get(nodeName, "Enable",nodeValue) < 0)
			break;
	}
	
	if(i >= MAX_APPFTP_ENTRY)
	{
		tcdbg_printf("%s:There is no free FTPClient for adding.\n", __func__);
		return -1;
	}
	
	tcapi_set(nodeName, "Enable", "N/A");
	*num = i + 1 - APPFTPHISTORY_NUM;
	cwmpFreeDeviceTable(FTPClientNode);
	ret = cwmpInitFTPClientTable(FTPClientNode);

	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\ncp_Service_ftp_func.c---[FTPClientNodeAddObject]---*num = ", *num);
	}

	return ret;
}

int cwmpInitFTPClientTable(DeviceNodePtr deviceNode)
{
	int i,rtnCode=0;
	char nodeName[32] = {0};
	char nodeValue[32] = {0};
	char index[10] = {0};
	
	for(i=APPFTPHISTORY_NUM; i<MAX_APPFTP_ENTRY; i++)
	{
		sprintf(nodeName, "appFTP_Entry%d", i);
		if(tcapi_get(nodeName, "Enable",nodeValue) < 0)
			continue;

		sprintf(index, "%d", i+1-APPFTPHISTORY_NUM);
		X_CU_FTPCltIndex[0].name = index;	
		/*register this node*/
		if ( RegisterNodeFromTable( deviceNode, X_CU_FTPCltIndex ) == NULL ) {
			rtnCode = -1;		//register Node fail.
			return rtnCode;
		}
	}

	return rtnCode;
}

int8 getX_CU_FTPCltEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char enable[8] = {0};
	char nodeName[32] = {0};
	uint8 ftpClientIndex, entryIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;
	
	entryIndex = ftpClientIndex + APPFTPHISTORY_NUM;
	sprintf(nodeName, "appFTP_Entry%d", entryIndex);	
	tcapi_get(nodeName, "Enable", enable);

	if(strcmp(enable, "1"))
		strcpy(enable,"0");
	
	return getTheValue(value, type, deviceNode, enable);
}
int8 setX_CU_FTPCltEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char enable[8] = {0};
	char nodeName[32] = {0};
	uint8 ftpClientIndex, entryIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;
	
	entryIndex = ftpClientIndex + APPFTPHISTORY_NUM;
	sprintf(nodeName, "appFTP_Entry%d", entryIndex);	
	if(*((uint8*)value) == 1)		
		tcapi_set(nodeName, "Enable", "1");
	else if(*((uint8*)value) == 0)		
		tcapi_set(nodeName, "Enable", "0");
	else
		return -1;
	
	tcapi_set(nodeName, "deviceid", "usb1_1");
	tcapi_set(nodeName, "savepath", "tr69FTPDownload");
	SetCfgNodeFlag( "appFTP", entryIndex);
	
	return 0;
}
int8 getX_CU_FTPCltEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint8 ftpClientIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;
	
	attribute = tcCUAttr.aFTPCltEnable[ftpClientIndex];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_FTPCltEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	uint8 ftpClientIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;

	attribute = &tcCUAttr.aFTPCltEnable[ftpClientIndex];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_FTPCltFtpServerURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char url[260] = {0};
	char nodeName[32] = {0};
	uint8 ftpClientIndex, entryIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;
	
	entryIndex = ftpClientIndex + APPFTPHISTORY_NUM;
	sprintf(nodeName, "appFTP_Entry%d", entryIndex);	
	
	tcapi_get(nodeName,"url", url);
	
	return getTheValue(value, type, deviceNode, url);
}
int8 setX_CU_FTPCltFtpServerURLValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	uint8 ftpClientIndex, entryIndex;
	int i, len;
	char* p = (char*)value;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;	

	len = strlen(p);
	
	if(len > 256 || len <= 6)
		return -1;

	if(strncmp(p, "ftp://",6))
		return -1;

	if(*(p+len-1) == '/')
		return -1;
	
	if(!strchr(p+6, '/'))
		return -1;
	
	for(i=6; i<len-1; i++)
	{
		if(*(p+i) == '\'')
			return -1;
	}
	
	entryIndex = ftpClientIndex + APPFTPHISTORY_NUM;
	sprintf(nodeName, "appFTP_Entry%d", entryIndex);	
	tcapi_set(nodeName,"url", p);
	
	SetCfgNodeFlag( "appFTP", entryIndex);
	return 0;
}
int8 getX_CU_FTPCltFtpServerURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint8 ftpClientIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;
	
	attribute = tcCUAttr.aFTPCltFtpServerURL[ftpClientIndex];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_FTPCltFtpServerURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	uint8 ftpClientIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;
	
	attribute = &tcCUAttr.aFTPCltFtpServerURL[ftpClientIndex];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_FTPCltSvrPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char port[260] = {0};
	char nodeName[32] = {0};
	uint8 ftpClientIndex, entryIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;
	
	entryIndex = ftpClientIndex + APPFTPHISTORY_NUM;
	sprintf(nodeName, "appFTP_Entry%d", entryIndex);	
	
	if(tcapi_get(nodeName,"port", port)<0)
		strcpy(port, "21");
	
	return getTheValue(value, type, deviceNode, port);
}
int8 setX_CU_FTPCltSvrPortValue(void* value, struct DeviceNode*  deviceNode)
{
	int port = *((int*)value);
	char portStr[20] = {0};
	char nodeName[32] = {0};
	uint8 ftpClientIndex, entryIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;	
	
	if(port > 63365 || port < 1)
		return -1;
	
	entryIndex = ftpClientIndex + APPFTPHISTORY_NUM;
	sprintf(nodeName, "appFTP_Entry%d", entryIndex);	

	sprintf(portStr, "%d", port);
	tcapi_set(nodeName,"port", portStr);
	
	SetCfgNodeFlag( "appFTP", entryIndex);
	return 0;
}
int8 getX_CU_FTPCltSvrPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint8 ftpClientIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;
	
	attribute = tcCUAttr.aFTPCltSvrPort[ftpClientIndex];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_FTPCltSvrPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	uint8 ftpClientIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;
	
	attribute = &tcCUAttr.aFTPCltSvrPort[ftpClientIndex];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_FTPCltSvrUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char username[260] = {0};
	char nodeName[32] = {0};
	uint8 ftpClientIndex, entryIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;
	
	entryIndex = ftpClientIndex + APPFTPHISTORY_NUM;
	sprintf(nodeName, "appFTP_Entry%d", entryIndex);	
	
	tcapi_get(nodeName,"username", username);
	
	return getTheValue(value, type, deviceNode, username);
}
int8 setX_CU_FTPCltSvrUserNameValue(void* value, struct DeviceNode*  deviceNode)
{
	char* p = (char*)value;
	char nodeName[32] = {0};
	uint8 ftpClientIndex, entryIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;	
	
	int i, len;
	len = strlen(p);
	
	if(len > 256)
		return -1;

	for(i=0; i<len; i++)
	{
		if(*(p+i) == '\'' || *(p+i) == '\"' || *(p+i) < 32 || *(p+i) > 126)
			return -1;
	}

	entryIndex = ftpClientIndex + APPFTPHISTORY_NUM;
	sprintf(nodeName, "appFTP_Entry%d", entryIndex);	
	tcapi_set(nodeName,"username", p);
	
	SetCfgNodeFlag( "appFTP", entryIndex);
	return 0;
}
int8 getX_CU_FTPCltSvrUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint8 ftpClientIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;
	
	attribute = tcCUAttr.aFTPCltSvrUserName[ftpClientIndex];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_FTPCltSvrUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	uint8 ftpClientIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;
	
	attribute = &tcCUAttr.aFTPCltSvrUserName[ftpClientIndex];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_FTPCltSvrPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char password[260] = {0};
	char nodeName[32] = {0};
	uint8 ftpClientIndex, entryIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;
	
	entryIndex = ftpClientIndex + APPFTPHISTORY_NUM;
	sprintf(nodeName, "appFTP_Entry%d", entryIndex);	
	
	tcapi_get(nodeName,"password", password);
	
	return getTheValue(value, type, deviceNode, password);
}
int8 setX_CU_FTPCltSvrPasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	char* p = (char*)value;
	char nodeName[32] = {0};
	uint8 ftpClientIndex, entryIndex;
	int i, len;

	len = strlen(p);
	
	if(len > 256)
		return -1;

	for(i=0; i<len; i++)
	{
		if(*(p+i) == '\'' || *(p+i) == '\"' || *(p+i) < 32 || *(p+i) > 126)
			return -1;
	}
	
	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;	

	entryIndex = ftpClientIndex + APPFTPHISTORY_NUM;
	sprintf(nodeName, "appFTP_Entry%d", entryIndex);	
	tcapi_set(nodeName,"password", p);
	
	SetCfgNodeFlag( "appFTP", entryIndex);
	return 0;
}
int8 getX_CU_FTPCltSvrPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint8 ftpClientIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;
	
	attribute = tcCUAttr.aFTPCltSvrPassword[ftpClientIndex];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_FTPCltSvrPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	uint8 ftpClientIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;

	attribute = &tcCUAttr.aFTPCltSvrPassword[ftpClientIndex];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_FTPCltStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char status[20] = {0};
	char nodeName[32] = {0};
	uint8 ftpClientIndex, entryIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;
	
	entryIndex = ftpClientIndex + APPFTPHISTORY_NUM;
	sprintf(nodeName, "appFTP_Entry%d", entryIndex);	

	entryIndex = ftpClientIndex + APPFTPHISTORY_NUM;
	sprintf(nodeName, "appFTP_Entry%d", entryIndex);	
	
	tcapi_get(nodeName,"result", result);
	if(strlen(result)){
		if(!strcmp(result,"0") || !strcmp(result,"10"))
			strcpy(status, "Success");
		else
			strcpy(status, "Fail");
	}
	
	return getTheValue(value, type, deviceNode, status);
}
int8 getX_CU_FTPCltStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint8 ftpClientIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;
	
	attribute = tcCUAttr.aFTPCltStatus[ftpClientIndex];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_FTPCltStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	uint8 ftpClientIndex;

	ftpClientIndex = atoi(deviceNode->parentNode->name) - 1;
	if(ftpClientIndex < 0 || ftpClientIndex >= APPFTPCLIENT_NUM)
		return -1;
	
	attribute = &tcCUAttr.aFTPCltStatus[ftpClientIndex];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}


int cuappFTPCommit(cfgNode_ptr pCfgNode)
{
	int ruleIndex;
	int i = 0, ret = 0;
	char nodeName[32];

	for (ruleIndex = APPFTPCLIENT_NUM; ruleIndex < MAX_APPFTP_ENTRY; ruleIndex++) {		
		if (pCfgNode->bitMap & (1<<ruleIndex)) {
			sprintf(nodeName, "appFTP_Entry%d", ruleIndex);
			tcapi_commit(nodeName);
		}
	}

	return 0;
}

