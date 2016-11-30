#include "Global_res.h"
#include "cp_Services_user.h"

/*Acs is deleteing X_CU_User object*/
int8 DeviceNodeDelXCUUserObject(uint32 num, DeviceNode *deviceNode)
{
	char nodeName[32] = {0};;
	char val[8];
	uint32 del_index, i, ret = 0;
	char objectName[100] = {0};
	DeviceNodePtr UserNode = NULL;

	if (deviceNode == NULL) {
		return -1;
	}
	
	del_index = num - 1;

	if(del_index >= MAX_USER_NUM || del_index < 0)
	{
		tc_printf("%s:Invalid  %s index.\n",__func__, deviceNode->parentNode->name);
		return -1;
	}
	
	/* check if the Entry is exist */
	sprintf(nodeName, "User_Entry%d", del_index);	
	/* this Entry is not exist, just return */
	ret = tcapi_get(nodeName, "enable", val);
	if (ret < 0) {
		return ret;
	}

	tcapi_unset(nodeName);	

	if(!strcmp(deviceNode->parentNode->name, "X_CU_User"))
		strcpy(objectName, "InternetGatewayDevice.Services.User.");
	else if(!strcmp(deviceNode->parentNode->name, "User"))
		strcpy(objectName, "InternetGatewayDevice.Services.X_CU_User.");
	
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &UserNode);
	if (UserNode) {
		cwmpFreeDeviceTable(UserNode);
		ret = cwmpInitXCUUserTable(UserNode);
	}
	
	cwmpFreeDeviceTable(deviceNode->parentNode);
	ret = cwmpInitXCUUserTable(deviceNode->parentNode);
	
	return ret;
}

/*Acs is adding X_CU_User object*/
int8 XCUUserNodeAddObject(uint32 *num, DeviceNode*  deviceNode)
{
	char add_index[10] = {0};
	char nodeValue[10] = {0};
	char nodeName[32] = {0};
	int ret = 0, i;
	char objectName[100] = {0};
	DeviceNodePtr UserNode = NULL;
	
	if (NULL == deviceNode) {
		tcdbg_printf("top node not found!\n");
		return -1;
	}
	
	for(i=0; i<MAX_USER_NUM; i++)
	{
		sprintf(nodeName, "User_Entry%d", i);
		if(tcapi_get(nodeName, "enable",nodeValue) < 0)
			break;
	}
	
	if(i >= MAX_USER_NUM)
	{
		tc_printf("%s:There is no free %s for adding.\n", __func__, deviceNode->name);
		return -1;
	}
	
	tcapi_set(nodeName, "enable", "0");
	tcapi_set(nodeName, "name", "");
	*num = i + 1;
	
	cwmpFreeDeviceTable(deviceNode);
	ret = cwmpInitXCUUserTable(deviceNode);
	
	if(!strcmp(deviceNode->name, "X_CU_User"))
		strcpy(objectName, "InternetGatewayDevice.Services.User.");
	else if(!strcmp(deviceNode->name, "User"))
		strcpy(objectName, "InternetGatewayDevice.Services.X_CU_User.");

	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &UserNode);
	if (UserNode) {
		cwmpFreeDeviceTable(UserNode);
		ret = cwmpInitXCUUserTable(UserNode);
	}

	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\ncp_Service_user_func.c---[%s]---*num = ", __func__, *num);
	}

	return ret;
}

int cwmpInitXCUUserTable(DeviceNodePtr deviceNode)
{
	int i,rtnCode=0;
	char nodeName[32] = {0};
	char nodeValue[32] = {0};
	char index[10] = {0};

	for(i=0; i<MAX_USER_NUM; i++)
	{
		sprintf(nodeName, "User_Entry%d", i);
		if(tcapi_get(nodeName, "enable",nodeValue) < 0)
			continue;

		sprintf(index, "%d", i+1);
		if(!strcmp(deviceNode->name, "X_CU_User"))
		{
			X_CU_UserIndex[0].name = index;	
			/*register this node*/
			if ( RegisterNodeFromTable( deviceNode, X_CU_UserIndex ) == NULL ) {
				rtnCode = -1;		//register Node fail.
				return rtnCode;
			}
		}
		else if(!strcmp(deviceNode->name, "User"))
		{
			usrIndex[0].name = index;	
			/*register this node*/
			if ( RegisterNodeFromTable( deviceNode, usrIndex ) == NULL ) {
				rtnCode = -1;		//register Node fail.
				return rtnCode;
			}
		}
	}

	return rtnCode;
}

int8 getX_CU_UserEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char enbl[8] = "0";
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	//sprintf(nodeName, "User_Entry%d",user_index);
	tcapi_get("Accesslimit_Common","mode",enbl);
	if(strcmp(enbl,"1"))
		sprintf(enbl, "%d", 0);
	
	return getTheValue(value, type, deviceNode, enbl);
}
int8 setX_CU_UserEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char enbl[8] = {0};
	uint8 val = *((uint8*)value);
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	//sprintf(nodeName, "User_Entry%d",user_index);
	
	if(val == 1 )
	{
		tcapi_set("Accesslimit_Common","mode","1");
	}
	else if(val == 0 )
	{
		tcapi_set("Accesslimit_Common","mode","0");
	}
	else
		return -1;
	
	SetCfgNodeFlag( "Accesslimit", user_index);
	
	return 0;
}
int8 getX_CU_UserEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcCUAttr.aUserEnable[user_index];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_UserEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcCUAttr.aUserEnable[user_index];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_UserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[64] = {0};
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, "User_Entry%d",user_index);
	tcapi_get(nodeName,"name", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setX_CU_UserNameValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, "User_Entry%d",user_index);
	
	tcapi_set(nodeName,"name",(char*)value);
	
	SetCfgNodeFlag( "User", user_index);
	
	return 0;
}
int8 getX_CU_UserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcCUAttr.aUserName[user_index];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_UserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcCUAttr.aUserName[user_index];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_UserDespValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[64] = {0};
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, "User_Entry%d",user_index);
	
	tcapi_get(nodeName,"description",tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setX_CU_UserDespValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, "User_Entry%d",user_index);
	
	tcapi_set(nodeName,"description",(char*)value);

	SetCfgNodeFlag( "User", user_index);
	
	return 0;
}
int8 getX_CU_UserDespAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcCUAttr.aUserDescription[user_index];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_UserDespAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcCUAttr.aUserDescription[user_index];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_UserNum0fSubUserValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[64] = {0};
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	//sprintf(nodeName, "User_Entry%d",user_index);
	
	tcapi_get("Accesslimit_Common","totalnum",tempValue);
	if(!strlen(tempValue))
		sprintf(tempValue, "%d", 4);	
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setX_CU_UserNum0fSubUserValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char subusernum[12] = {0};
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	//sprintf(nodeName, "User_Entry%d",user_index);
	sprintf(subusernum, "%d", *((uint*)value));	
	tcapi_set("Accesslimit_Common","totalnum",subusernum);

	SetCfgNodeFlag( "Accesslimit", user_index);
	
	return 0;
}
int8 getX_CU_UserNum0fSubUserAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcCUAttr.aNumberOfSubuser[user_index];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_UserNum0fSubUserAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcCUAttr.aNumberOfSubuser[user_index];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_UserNum0fMACFilterValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char iNum[8] = {0};
	char nodeName[32] = {0};
	char nodeValue[32] = {0};
	int i, macfilternum = 0;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	
	for(i=0; i<MAC_FILTER_NUM_PER_USER; i++)
	{
		sprintf(nodeName, "MACFilter_Entry%d_Entry%d", user_index,i);
		if(tcapi_get(nodeName, "MAC", nodeValue) == 0)
			macfilternum++;
	}
	
	sprintf(iNum, "%d", macfilternum);

	return getTheValue(value, type, deviceNode, iNum);
}
int8 getX_CU_UserNum0fMACFilterAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcCUAttr.aMACFilterNumberOfEntries[user_index];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_UserNum0fMACFilterAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcCUAttr.aMACFilterNumberOfEntries[user_index];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_UserNum0fIPFilterValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char iNum[8] = {0};
	char nodeName[32] = {0};
	char nodeValue[32] = {0};
	int i, ipfilternum = 0;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	
	for(i=0; i<IP_FILTER_NUM_PER_USER; i++)
	{
		sprintf(nodeName, "IPFilter_Entry%d_Entry%d", user_index,i);
		if(tcapi_get(nodeName, "IP", nodeValue) == 0)
			ipfilternum++;
	}
	
	sprintf(iNum, "%d", ipfilternum);

	return getTheValue(value, type, deviceNode, iNum);
}
int8 getX_CU_UserNum0fIPFilterAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcCUAttr.aIPFilterNumberOfEntries[user_index];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_UserNum0fIPFilterAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcCUAttr.aIPFilterNumberOfEntries[user_index];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_UserNum0fWhiteListValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char iNum[8] = {0};
	char nodeName[32] = {0};
	char nodeValue[32] = {0};
	int i, whiteurlrnum = 0;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	
	for(i=0; i<WHITE_URL_NUM_PER_USER; i++)
	{
		sprintf(nodeName, "WhiteURL_Entry%d_Entry%d", user_index,i);
		if(tcapi_get(nodeName, "URL", nodeValue) == 0)
			whiteurlrnum++;
	}

	sprintf(iNum, "%d", whiteurlrnum);

	return getTheValue(value, type, deviceNode, iNum);
}
int8 getX_CU_UserNum0fWhiteListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcCUAttr.aWhiteListNumberOfEntries[user_index];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_UserNum0fWhiteListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcCUAttr.aWhiteListNumberOfEntries[user_index];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_UserNum0fBlackListValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char iNum[8] = {0};
	char nodeName[32] = {0};
	char nodeValue[32] = {0};
	int i, blackurlrnum = 0;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	
	for(i=0; i<BLACK_URL_NUM_PER_USER; i++)
	{
		sprintf(nodeName, "BlackURL_Entry%d_Entry%d", user_index,i);
		if(tcapi_get(nodeName, "URL", nodeValue) == 0)
			blackurlrnum++;
	}

	sprintf(iNum, "%d", blackurlrnum);

	return getTheValue(value, type, deviceNode, iNum);
}
int8 getX_CU_UserNum0fBlackListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcCUAttr.aBlackListNumberOfEntries[user_index];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_UserNum0fBlackListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcCUAttr.aBlackListNumberOfEntries[user_index];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}
int8 getX_CU_UserURLListFilterModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
    char tempValue[64] = {0};
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, "User_Entry%d",user_index);
	
	tcapi_get(nodeName,"urllistfiltermode",tempValue);
	if(!strlen(tempValue))
		sprintf(tempValue, "%d", 0);	
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setX_CU_UserURLListFilterModeValue(void* value, struct DeviceNode*  deviceNode)
{
    char nodeName[32] = {0};
	char filter_mode[12] = {0};
	int user_index,tmValue;
     
	user_index = atoi(deviceNode->parentNode->name) - 1;

    tmValue = *(int*)(value);
    
	if(tmValue != 0 && tmValue != 1)
		return -1;
	
	sprintf(nodeName, "User_Entry%d",user_index);
	sprintf(filter_mode, "%d", *((uint*)value));	
	tcapi_set(nodeName,"urllistfiltermode",filter_mode);

	SetCfgNodeFlag( "User", user_index);
	
	return 0;
}
int8 getX_CU_UserURLListFilterModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcCUAttr.aURLListFilterMode[user_index];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_UserURLListFilterModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcCUAttr.aURLListFilterMode[user_index];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}
int8 getX_CU_TimeEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char enab[8] = "0";
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	sprintf(nodeName, "User_Entry%d",user_index);

	tcapi_get(nodeName,"timeEnable",enab);
	if(strcmp(enab, "1"))
		strcpy(enab,"0");

	return getTheValue(value, type, deviceNode, enab);
}
int8 setX_CU_TimeEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 enab = *((uint8*)value);
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	sprintf(nodeName, "User_Entry%d",user_index);
	
	if(enab == 1)
	{
		tcapi_set(nodeName,"timeEnable","1");
	}
	else if(enab == 0)
	{
		tcapi_set(nodeName,"timeEnable","0");
	}
	else
		return -1;

	SetCfgNodeFlag( "User", user_index);
	
	return 0;
}
int8 getX_CU_TimeEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	attribute = tcCUAttr.aTimeEnable[user_index];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_TimeEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	attribute = &tcCUAttr.aTimeEnable[user_index];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_TimeRulesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char timeRule[262] = {};
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	sprintf(nodeName, "User_Entry%d",user_index);

	tcapi_get(nodeName,"timeRule",timeRule);
	
	return getTheValue(value, type, deviceNode, timeRule);
}
int8 setX_CU_TimeRulesValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	sprintf(nodeName, "User_Entry%d",user_index);

	tcapi_set(nodeName,"timeRule",(char*)value);

	SetCfgNodeFlag( "User", user_index);
	
	return 0;
}
int8 getX_CU_TimeRulesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	attribute = tcCUAttr.aTimeRules[user_index];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_TimeRulesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	attribute = &tcCUAttr.aTimeRules[user_index];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

/*Acs is deleteing Macfilter object*/
int8 DeviceNodeDelMacFilterObject(uint32 num, DeviceNode *deviceNode)
{
	char nodeName[32] = {0};;
	char val[32];
	uint32 del_index, i, ret = 0;
	int user_index;
	
	DeviceNodePtr MacFilterNode = deviceNode->parentNode;
	
	if (MacFilterNode == NULL) {
		return -1;
	}
	
	user_index = atoi(MacFilterNode->parentNode->name)-1;
	if(user_index < 0 || user_index >= MAX_USER_NUM)
	{
		tc_printf("\r\n invalid %s index", MacFilterNode->parentNode->parentNode->name);
		return -1;
	}
	
	del_index = num - 1;

	if(del_index >= MAC_FILTER_NUM_PER_USER || del_index < 0)
	{
		tc_printf("%s:Invalid  Macfilter index.\n",__func__);
		return -1;
	}
	
	/* check if the Entry is exist */	
	sprintf(nodeName, "MACFilter_Entry%d_Entry%d", user_index, del_index);
		
	/* this Entry is not exist, just return */
	ret = tcapi_get(nodeName, "MAC", val);
	if (ret < 0) {
		return ret;
	}

	tcapi_unset(nodeName);

	tcapi_commit(nodeName);
	
	cwmpFreeDeviceTable(MacFilterNode);
	ret = cwmpInitMacFilterTable(MacFilterNode);
	
	return ret;
}

/*Acs is adding Macfilter object*/
int8 MacFilterNodeAddObject(uint32 *num, DeviceNode*  deviceNode)
{
	char add_index[10] = {0};
	char mac[32] = {0};
	char nodeName[32] = {0};
	int  ret = 0;
	int user_index, i;
	
	DeviceNodePtr MacFilterNode = deviceNode;
	if (NULL == MacFilterNode) {
		tcdbg_printf("top node not found!\n");
		return -1;
	}

	user_index = atoi(MacFilterNode->parentNode->name)-1;

	if(user_index < 0 || user_index >= MAX_USER_NUM)
	{
		tc_printf("\r\n invalid %s index", MacFilterNode->parentNode->parentNode->name);
		return -1;
	}
	
	for(i=0; i<MAC_FILTER_NUM_PER_USER; i++)
	{
		sprintf(nodeName, "MACFilter_Entry%d_Entry%d", user_index, i);
		ret = tcapi_get(nodeName, "MAC", mac);
		if(ret < 0)
			break;
	}
	
	if(i >= MAC_FILTER_NUM_PER_USER)
	{
		tc_printf("%s:There is no free Macfilter for adding.\n", __func__);
		return -1;
	}
	
	*num = i + 1;
	tcapi_set(nodeName, "MAC", "");

	cwmpFreeDeviceTable(MacFilterNode);
	ret = cwmpInitMacFilterTable(MacFilterNode);

	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\ncp_Service_user_func.c---[%s]---*num = ", __func__, *num);
	}

	return ret;
}

int cwmpInitMacFilterTable(DeviceNodePtr deviceNode)
{
	int i,rtnCode=0;
	char nodeName[32] = {0};
	char nodeValue[32] = {0};
	char index[10] = {0};
	int user_index, ret;

	if (NULL == deviceNode) {
		tcdbg_printf("top node not found!\n");
		return -1;
	}

	user_index = atoi(deviceNode->parentNode->name)-1;
	
	for(i=0; i<MAC_FILTER_NUM_PER_USER; i++)
	{
		sprintf(nodeName, "MACFilter_Entry%d_Entry%d", user_index,i);
		ret = tcapi_get(nodeName, "MAC",nodeValue);

		if(ret == 0){
			sprintf(index, "%d", i+1);
			MacFilterIndex[0].name = index;	
			/*register this node*/
			if ( RegisterNodeFromTable( deviceNode, MacFilterIndex ) == NULL ) {
				rtnCode = -1;		//register Node fail.
				return rtnCode;
			}
		}
	}

	return rtnCode;
}

int8 getX_CU_FilterMacValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char mac[20] = {0};
	char nodeName[32] = {0};
	int instance, user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	instance = atoi(deviceNode->parentNode->name)-1;

	sprintf(nodeName, "MACFilter_Entry%d_Entry%d", user_index,instance);

	tcapi_get(nodeName, "MAC", mac);

	return getTheValue(value, type, deviceNode, mac);
}
int8 setX_CU_FilterMacValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	int instance, i, user_index;
	char *p= (char*)value;

	if(strlen(p) != 17)
		return -1;
	
	for(i=0;i<17;i++)
	{
		if(i%3==2)
		{
			if(*(p+i) != '-')
				return -1;
//			else
//				*(p+i)=':';
		}
		else
		{
			if(RpHex2dec(*(p+i)) < 0 )
				return -1;
		}
	}
	
	user_index = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	instance = atoi(deviceNode->parentNode->name)-1;
	sprintf(nodeName, "MACFilter_Entry%d_Entry%d", user_index,instance);
	tcapi_set(nodeName, "MAC", p);

	SetCfgNodeFlag("MACFilter", user_index);
	
	return 0;
}
int8 getX_CU_FilterMacAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance, user_index;
	
	user_index = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = tcCUAttr.aFilterMac[user_index][instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_FilterMacAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance, user_index;
	
	user_index = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;

	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = &tcCUAttr.aFilterMac[user_index][instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

/*Acs is deleteing IpFilter object*/
int8 DeviceNodeDelIpFilterObject(uint32 num, DeviceNode *deviceNode)
{
	char nodeName[32] = {0};;
	char val[32];
	uint32 del_index, i, ret = 0;
	int user_index;
	
	DeviceNodePtr IpFilterNode = deviceNode->parentNode;
	
	if (IpFilterNode == NULL) {
		return -1;
	}
	
	user_index = atoi(IpFilterNode->parentNode->name)-1;
	if(user_index < 0 || user_index >= MAX_USER_NUM)
	{
		tc_printf("\r\n invalid %s index", IpFilterNode->parentNode->parentNode->name);
		return -1;
	}
	
	del_index = num - 1;

	if(del_index >= IP_FILTER_NUM_PER_USER || del_index < 0)
	{
		tc_printf("%s:Invalid  IpFilterNode index.\n",__func__);
		return -1;
	}
	
	/* check if the Entry is exist */	
	sprintf(nodeName, "IPFilter_Entry%d_Entry%d", user_index, del_index);
		
	/* this Entry is not exist, just return */
	ret = tcapi_get(nodeName, "IP", val);
	if (ret < 0) {
		return ret;
	}

	tcapi_unset(nodeName);

	tcapi_commit(nodeName);
	
	cwmpFreeDeviceTable(IpFilterNode);
	ret = cwmpInitIpFilterTable(IpFilterNode);
	
	return ret;
}

/*Acs is adding IpFilter object*/
int8 IpFilterNodeAddObject(uint32 *num, DeviceNode*  deviceNode)
{
	char add_index[10] = {0};
	char ip[32] = {0};
	char nodeName[32] = {0};
	int  ret = 0;
	int user_index, i;
	
	DeviceNodePtr IpFilterNode = deviceNode;
	if (NULL == IpFilterNode) {
		tcdbg_printf("top node not found!\n");
		return -1;
	}

	user_index = atoi(IpFilterNode->parentNode->name)-1;

	if(user_index < 0 || user_index >= MAX_USER_NUM)
	{
		tc_printf("\r\n invalid %s index", IpFilterNode->parentNode->parentNode->name);
		return -1;
	}
	
	for(i=0; i<IP_FILTER_NUM_PER_USER; i++)
	{
		sprintf(nodeName, "IPFilter_Entry%d_Entry%d", user_index, i);
		ret = tcapi_get(nodeName, "IP", ip);
		if(ret < 0)
			break;
	}
	
	if(i >= IP_FILTER_NUM_PER_USER)
	{
		tc_printf("%s:There is no free IpFilter for adding.\n", __func__);
		return -1;
	}
	
	*num = i + 1;
	tcapi_set(nodeName, "IP", "");

	cwmpFreeDeviceTable(IpFilterNode);
	ret = cwmpInitIpFilterTable(IpFilterNode);

	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\ncp_Service_user_func.c---[%s]---*num = ", __func__, *num);
	}

	return ret;
}

int cwmpInitIpFilterTable(DeviceNodePtr deviceNode)
{
	int i,rtnCode=0;
	char nodeName[32] = {0};
	char nodeValue[32] = {0};
	char index[10] = {0};
	int user_index, ret;

	if (NULL == deviceNode) {
		tcdbg_printf("top node not found!\n");
		return -1;
	}

	user_index = atoi(deviceNode->parentNode->name)-1;
	
	for(i=0; i<IP_FILTER_NUM_PER_USER; i++)
	{
		sprintf(nodeName, "IPFilter_Entry%d_Entry%d", user_index,i);
		ret = tcapi_get(nodeName, "IP",nodeValue);

		if(ret == 0){
			sprintf(index, "%d", i+1);
			IpFilterIndex[0].name = index;	
			/*register this node*/
			if ( RegisterNodeFromTable( deviceNode, IpFilterIndex ) == NULL ) {
				rtnCode = -1;		//register Node fail.
				return rtnCode;
			}
		}
	}

	return rtnCode;
}

int8 getX_CU_FilterIPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ip[20] = {0};
	char nodeName[32] = {0};
	int instance, user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	instance = atoi(deviceNode->parentNode->name)-1;

	sprintf(nodeName, "IPFilter_Entry%d_Entry%d", user_index,instance);

	tcapi_get(nodeName, "IP", ip);

	return getTheValue(value, type, deviceNode, ip);
}
int8 setX_CU_FilterIPValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	int instance, i, user_index;
	char *p= (char*)value;

	if(_checkIp((char*)value) < 0)	
		return -1;
	
	user_index = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	instance = atoi(deviceNode->parentNode->name)-1;
	sprintf(nodeName, "IPFilter_Entry%d_Entry%d", user_index,instance);
	tcapi_set(nodeName, "IP", p);

	SetCfgNodeFlag("IPFilter", user_index);
	
	return 0;
}
int8 getX_CU_FilterIPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance, user_index;
	
	user_index = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = tcCUAttr.aFilterIP[user_index][instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_FilterIPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance, user_index;
	
	user_index = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;

	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = &tcCUAttr.aFilterIP[user_index][instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

/*Acs is deleteing WhiteList object*/
int8 DeviceNodeDelWhiteListObject(uint32 num, DeviceNode *deviceNode)
{
	char nodeName[32] = {0};;
	char val[8];
	uint32 del_index, i, ret = 0;
	int user_index;
	
	DeviceNodePtr WhiteListNode = deviceNode->parentNode;
	
	if (WhiteListNode == NULL) {
		return -1;
	}
	
	user_index = atoi(WhiteListNode->parentNode->name)-1;
	if(user_index < 0 || user_index >= MAX_USER_NUM)
	{
		tc_printf("\r\n invalid %s index", WhiteListNode->parentNode->parentNode->name);
		return -1;
	}
	
	del_index = num - 1;

	if(del_index >= WHITE_URL_NUM_PER_USER || del_index < 0)
	{
		tc_printf("%s:Invalid  WhiteListNode index.\n",__func__);
		return -1;
	}
	
	/* check if the Entry is exist */	
	sprintf(nodeName, "WhiteURL_Entry%d_Entry%d", user_index, del_index);
		
	/* this Entry is not exist, just return */
	ret = tcapi_get(nodeName, "URL", val);
	if (ret < 0) {
		return ret;
	}

	tcapi_unset(nodeName);

	tcapi_commit(nodeName);
	
	cwmpFreeDeviceTable(WhiteListNode);
	ret = cwmpInitWhiteListTable(WhiteListNode);
	
	return ret;
}

/*Acs is adding WhiteList object*/
int8 WhiteListNodeAddObject(uint32 *num, DeviceNode*  deviceNode)
{
	char add_index[10] = {0};
	char mac[32] = {0};
	char nodeName[32] = {0};
	int  ret = 0;
	int user_index, i;
	
	DeviceNodePtr WhiteListNode = deviceNode;
	if (NULL == WhiteListNode) {
		tcdbg_printf("top node not found!\n");
		return -1;
	}

	user_index = atoi(WhiteListNode->parentNode->name)-1;

	if(user_index < 0 || user_index >= MAX_USER_NUM)
	{
		tc_printf("\r\n invalid %s index", WhiteListNode->parentNode->parentNode->name);
		return -1;
	}
	
	for(i=0; i<WHITE_URL_NUM_PER_USER; i++)
	{
		sprintf(nodeName, "WhiteURL_Entry%d_Entry%d", user_index, i);
		ret = tcapi_get(nodeName, "URL", mac);
		if(ret < 0)
			break;
	}
	
	if(i >= WHITE_URL_NUM_PER_USER)
	{
		tc_printf("%s:There is no free WhiteList for adding.\n", __func__);
		return -1;
	}
	
	*num = i + 1;
	tcapi_set(nodeName, "URL", "");

	cwmpFreeDeviceTable(WhiteListNode);
	ret = cwmpInitWhiteListTable(WhiteListNode);

	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\ncp_Service_user_func.c---[%s]---*num = ", __func__, *num);
	}

	return ret;
}

int cwmpInitWhiteListTable(DeviceNodePtr deviceNode)
{
	int i,rtnCode=0;
	char nodeName[32] = {0};
	char nodeValue[32] = {0};
	char index[10] = {0};
	int user_index, ret;

	if (NULL == deviceNode) {
		tcdbg_printf("top node not found!\n");
		return -1;
	}

	user_index = atoi(deviceNode->parentNode->name)-1;
	
	for(i=0; i<WHITE_URL_NUM_PER_USER; i++)
	{
		sprintf(nodeName, "WhiteURL_Entry%d_Entry%d", user_index,i);
		ret = tcapi_get(nodeName, "URL",nodeValue);

		if(ret == 0){
			sprintf(index, "%d", i+1);
			WhiteListIndex[0].name = index;	
			/*register this node*/
			if ( RegisterNodeFromTable( deviceNode, WhiteListIndex ) == NULL ) {
				rtnCode = -1;		//register Node fail.
				return rtnCode;
			}
		}
	}

	return rtnCode;
}

int8 getX_CU_WhiteURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char url[100] = {0};
	char nodeName[32] = {0};
	int instance, user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	instance = atoi(deviceNode->parentNode->name)-1;

	sprintf(nodeName, "WhiteURL_Entry%d_Entry%d", user_index,instance);

	tcapi_get(nodeName, "URL", url);

	return getTheValue(value, type, deviceNode, url);
}
int8 setX_CU_WhiteURLValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	int instance, i, user_index;
	char *p= (char*)value;

	
	user_index = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	instance = atoi(deviceNode->parentNode->name)-1;
	sprintf(nodeName, "WhiteURL_Entry%d_Entry%d", user_index,instance);
	tcapi_set(nodeName, "URL", p);

	SetCfgNodeFlag("WhiteURL", user_index);
	
	return 0;
}
int8 getX_CU_WhiteURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance, user_index;
	
	user_index = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = tcCUAttr.aWhiteURL[user_index][instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_WhiteURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance, user_index;
	
	user_index = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = &tcCUAttr.aWhiteURL[user_index][instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

/*Acs is deleteing BlackList object*/
int8 DeviceNodeDelBlackListObject(uint32 num, DeviceNode *deviceNode)
{
	char nodeName[32] = {0};;
	char val[8];
	uint32 del_index, i, ret = 0;
	int user_index;
	
	DeviceNodePtr BlackListNode = deviceNode->parentNode;
	
	if (BlackListNode == NULL) {
		return -1;
	}
	
	user_index = atoi(BlackListNode->parentNode->name)-1;
	if(user_index < 0 || user_index >= MAX_USER_NUM)
	{
		tc_printf("\r\n invalid %s index", BlackListNode->parentNode->parentNode->name);
		return -1;
	}
	
	del_index = num - 1;

	if(del_index >= BLACK_URL_NUM_PER_USER || del_index < 0)
	{
		tc_printf("%s:Invalid  BlackListNode index.\n",__func__);
		return -1;
	}
	
	/* check if the Entry is exist */	
	sprintf(nodeName, "BlackURL_Entry%d_Entry%d", user_index, del_index);
		
	/* this Entry is not exist, just return */
	ret = tcapi_get(nodeName, "URL", val);
	if (ret < 0) {
		return ret;
	}

	tcapi_unset(nodeName);

	tcapi_commit(nodeName);
	
	cwmpFreeDeviceTable(BlackListNode);
	ret = cwmpInitBlackListTable(BlackListNode);
	
	return ret;
}

/*Acs is adding BlackList object*/
int8 BlackListNodeAddObject(uint32 *num, DeviceNode*  deviceNode)
{
	char add_index[10] = {0};
	char mac[32] = {0};
	char nodeName[32] = {0};
	int  ret = 0;
	int user_index, i;
	
	DeviceNodePtr BlackListNode = deviceNode;
	if (NULL == BlackListNode) {
		tcdbg_printf("top node not found!\n");
		return -1;
	}

	user_index = atoi(BlackListNode->parentNode->name)-1;

	if(user_index < 0 || user_index >= MAX_USER_NUM)
	{
		tc_printf("\r\n invalid %s index", BlackListNode->parentNode->parentNode->name);
		return -1;
	}
	
	for(i=0; i<BLACK_URL_NUM_PER_USER; i++)
	{
		sprintf(nodeName, "BlackURL_Entry%d_Entry%d", user_index, i);
		ret = tcapi_get(nodeName, "URL", mac);
		if(ret < 0)
			break;
	}
	
	if(i >= BLACK_URL_NUM_PER_USER)
	{
		tc_printf("%s:There is no free BlackList for adding.\n", __func__);
		return -1;
	}
	
	*num = i + 1;
	tcapi_set(nodeName, "URL", "");

	cwmpFreeDeviceTable(BlackListNode);
	ret = cwmpInitBlackListTable(BlackListNode);

	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\ncp_Service_user_func.c---[%s]---*num = ", __func__, *num);
	}

	return ret;
}

int cwmpInitBlackListTable(DeviceNodePtr deviceNode)
{
	int i,rtnCode=0;
	char nodeName[32] = {0};
	char nodeValue[32] = {0};
	char index[10] = {0};
	int user_index, ret;

	if (NULL == deviceNode) {
		tcdbg_printf("top node not found!\n");
		return -1;
	}

	user_index = atoi(deviceNode->parentNode->name)-1;
	
	for(i=0; i<BLACK_URL_NUM_PER_USER; i++)
	{
		sprintf(nodeName, "BlackURL_Entry%d_Entry%d", user_index,i);
		ret = tcapi_get(nodeName, "URL",nodeValue);

		if(ret == 0){
			sprintf(index, "%d", i+1);
			BlackListIndex[0].name = index;	
			/*register this node*/
			if ( RegisterNodeFromTable( deviceNode, BlackListIndex ) == NULL ) {
				rtnCode = -1;		//register Node fail.
				return rtnCode;
			}
		}
	}

	return rtnCode;
}

int8 getX_CU_BlackURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char url[100] = {0};
	char nodeName[32] = {0};
	int instance, user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	instance = atoi(deviceNode->parentNode->name)-1;

	sprintf(nodeName, "BlackURL_Entry%d_Entry%d", user_index,instance);

	tcapi_get(nodeName, "URL", url);

	return getTheValue(value, type, deviceNode, url);
}
int8 setX_CU_BlackURLValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	int instance, i, user_index;
	char *p= (char*)value;

	user_index = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	instance = atoi(deviceNode->parentNode->name)-1;
	sprintf(nodeName, "BlackURL_Entry%d_Entry%d", user_index,instance);
	tcapi_set(nodeName, "URL", p);

	SetCfgNodeFlag("BlackURL", user_index);
	
	return 0;
}
int8 getX_CU_BlackURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance, user_index;
	
	user_index = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = tcCUAttr.aBlackURL[user_index][instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_BlackURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance, user_index;
	
	user_index = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;

	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = &tcCUAttr.aBlackURL[user_index][instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_UsrPortalEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char enab[8] = "0";
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	sprintf(nodeName, "User_Entry%d",user_index);

	tcapi_get(nodeName,"portalEnable",enab);
	if(strcmp(enab, "1"))
		strcpy(enab, "0");

	return getTheValue(value, type, deviceNode, enab);
}
int8 setX_CU_UsrPortalEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 enab = *((uint8*)value);
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	sprintf(nodeName, "User_Entry%d",user_index);
	
	if(enab == 1)
	{
		tcapi_set(nodeName,"portalEnable","1");
		tcapi_set("Portal_Entry","Enable","1");
	}
	else if(enab == 0)
	{
		tcapi_set(nodeName,"portalEnable","0");
		tcapi_set("Portal_Entry","Enable","0");
	}
	else
		return -1;

	SetCfgNodeFlag( "User", user_index);
	SetCfgNodeFlag ("Portal", -1);
	
	return 0;
}
int8 getX_CU_UsrPortalEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	attribute = tcCUAttr.aPortalEnable[user_index];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_UsrPortalEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	attribute = &tcCUAttr.aPortalEnable[user_index];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_UsrPortalComputerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char portalComputer[100] = {0};
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	sprintf(nodeName, "User_Entry%d",user_index);

	tcapi_get(nodeName,"portalComputer",portalComputer);

	return getTheValue(value, type, deviceNode, portalComputer);
}
int8 setX_CU_UsrPortalComputerValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	sprintf(nodeName, "User_Entry%d",user_index);
	
	tcapi_set(nodeName,"portalComputer", (char*)value);
	tcapi_set("Portal_Entry","PortalUrl-Computer",(char*)value);

	SetCfgNodeFlag( "User", user_index);
	SetCfgNodeFlag ("Portal", -1);
	
	return 0;
}
int8 getX_CU_UsrPortalComputerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	attribute = tcCUAttr.aPortalComputer[user_index];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_UsrPortalComputerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	attribute = &tcCUAttr.aPortalComputer[user_index];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_UsrPortalIPTVValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char portalIPTV[100] = {0};
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	sprintf(nodeName, "User_Entry%d",user_index);

	tcapi_get(nodeName,"portalIPTV",portalIPTV);

	return getTheValue(value, type, deviceNode, portalIPTV);
}
int8 setX_CU_UsrPortalIPTVValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	sprintf(nodeName, "User_Entry%d",user_index);
	
	tcapi_set(nodeName,"portalIPTV", (char*)value);
	tcapi_set("Portal_Entry","PortalUrl-STB",(char*)value);

	SetCfgNodeFlag( "User", user_index);
	SetCfgNodeFlag ("Portal", -1);
	
	return 0;
}
int8 getX_CU_UsrPortalIPTVAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	attribute = tcCUAttr.aPortalIPTV[user_index];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_UsrPortalIPTVAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	attribute = &tcCUAttr.aPortalIPTV[user_index];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getX_CU_UsrPortalPhoneValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char portalPhone[100] = {0};
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	sprintf(nodeName, "User_Entry%d",user_index);

	tcapi_get(nodeName,"portalPhone",portalPhone);

	return getTheValue(value, type, deviceNode, portalPhone);
}
int8 setX_CU_UsrPortalPhoneValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};	
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	sprintf(nodeName, "User_Entry%d",user_index);
	
	tcapi_set(nodeName,"portalPhone", (char*)value);
	tcapi_set("Portal_Entry","PortalUrl-Phone",(char*)value);

	SetCfgNodeFlag( "User", user_index);
	SetCfgNodeFlag ("Portal", -1);
	
	return 0;
}
int8 getX_CU_UsrPortalPhoneAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	attribute = tcCUAttr.aPortalPhone[user_index];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_UsrPortalPhoneAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int user_index;

	user_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	attribute = &tcCUAttr.aPortalPhone[user_index];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

