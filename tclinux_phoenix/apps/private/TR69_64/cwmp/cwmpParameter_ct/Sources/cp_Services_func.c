#include "Global_res.h"
#include "cp_Services.h"

#if VOIP
int8 getVoiceProfileNmberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	sprintf(tempValue, "%d", 1);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getVoiceProfileNmberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aVoiceProfileNmberOfEntries;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setVoiceProfileNmberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aVoiceProfileNmberOfEntries;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int	cwmpInitVoiceServiceSubTree( DeviceNodePtr deviceNode )
{
	int num = VOICESERVICESNUM, rtnCode = 0;

	rtnCode = RegisterDynNodeFromIndex( deviceNode, VoiceServiceIndex, num );

	return rtnCode;
}

int8 cwmpVoiceProfileAddObject(uint32 *num, DeviceNode*  deviceNode)
{
	*num = 1;
	return 0;
}

int8 cwmpVoiceProfileDelObject(uint32 num, DeviceNode*  deviceNode)
{

	return 0;
}

int	cwmpInitVoiceProfileSubTree( DeviceNodePtr deviceNode )
{
	int num = VOICEPROFILENUM, rtnCode = 0;

	rtnCode = RegisterDynNodeFromIndex( deviceNode, VoiceProfileIndex, num );

	return rtnCode;
}

int8 cwmpPhyInterfaceAddObject(uint32 *num, DeviceNode*  deviceNode)
{
	*num = 1;
	return 0;
}

int8 cwmpPhyInterfaceDelObject(uint32 num, DeviceNode*  deviceNode)
{
	return 0;
}

int	cwmpInitPhyInterfaceSubTree( DeviceNodePtr deviceNode )
{
	int num = PHYINTERFACENUM, rtnCode = 0;

	rtnCode = RegisterDynNodeFromIndex( deviceNode, PhyInterfaceIndex, num );

	return rtnCode;
}
#endif
#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_ACCESSLIMIT)
int8	getModeServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char modeservices[15] = {0};
	tcapi_get(ACCESS_LIMIT_COMMOM_ENTRY,"mode",modeservices);
	
	return getTheValue(value, type, deviceNode, modeservices);
}

int8	setModeServicesValue(void* value, struct DeviceNode*  deviceNode)
{
	int modeservices = 0;
	char tmpbuf[15] = {0};
	
	SetCfgNodeFlag (ACCESS_LIMIT_COMMOM_ENTRY, -1);
	
	if( value == NULL)
		return -1;
	
	modeservices=*((int *)value);

	if ( modeservices < 0 || modeservices > 3 ) 
	{
		tc_printf("\r\n==>setModeServicesValue:mode is not valid!");
		return -1;
	
	}
	else
	{
		sprintf(tmpbuf, "%d", modeservices);
	}
	
	tcapi_set(ACCESS_LIMIT_COMMOM_ENTRY,"mode", tmpbuf);
	return 0;
}

int8	getModeServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcaccessLimitAttr.aMode;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setModeServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcaccessLimitAttr.aMode;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8	getTotalTerminalNumberServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char totalnumberervices[15] = {0};

	tcapi_get(ACCESS_LIMIT_COMMOM_ENTRY,"totalnum",totalnumberervices);
	
	return getTheValue(value, type, deviceNode, totalnumberervices);
}

int8	setTotalTerminalNumberServicesValue(void* value, struct DeviceNode*  deviceNode)
{
	int totalnumberservices = 0;
	char tmpbuf[15] = {0};
	
	SetCfgNodeFlag (ACCESS_LIMIT_COMMOM_ENTRY, -1);
	
	if( value == NULL)
		return -1;
	
	totalnumberservices=*((int *)value);
	if(totalnumberservices < -1)
	{
		tc_printf("\r\ntotalnumber should not be negative!");
		return -1;
	}
	
	sprintf(tmpbuf, "%d", totalnumberservices);
	
	tcapi_set(ACCESS_LIMIT_COMMOM_ENTRY,"totalnum", tmpbuf);
	return 0;
}

int8	getTotalTerminalNumberServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcaccessLimitAttr.aTotalTerminalNum;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setTotalTerminalNumberServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcaccessLimitAttr.aTotalTerminalNum;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

#if defined(TCSUPPORT_CMCC)
int8	getTerminalTypeServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char terminaltype[15] = {0};

	tcapi_get(ACCESS_LIMIT_COMMOM_ENTRY,"terminaltype",terminaltype);
	
	return getTheValue(value, type, deviceNode, terminaltype);
}

int8	setTerminalTypeServicesValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 terminaltype = 0;
	char tmpbuf[15] = {0};

	memset(tmpbuf,0,sizeof(tmpbuf));
	
	if(*(uint32*)value != 0 && *(uint32*)value != 1)
		return -1;
	
	terminaltype=*((uint32 *)value);
	sprintf(tmpbuf, "%lu", terminaltype);
	
	tcapi_set(ACCESS_LIMIT_COMMOM_ENTRY,"terminaltype", tmpbuf);
	
	SetCfgNodeFlag (ACCESS_LIMIT_COMMOM_ENTRY, -1);
	
	return 0;
}

int8	getTerminalTypeServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcaccessLimitAttr.aTerminalType;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setTerminalTypeServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcaccessLimitAttr.aTerminalType;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}
#endif

#endif
#endif

#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_IPTV_NODE)
int8 getIGMPEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char IGMPEnableValue[10] = {0};
	char cFlag[8] =  {0};

        tcapi_get("IGMPproxy_Entry","IGMPEnable", IGMPEnableValue);

	if(strcmp(IGMPEnableValue, "Yes") == 0){
		strcpy(cFlag, "true");
	} 
	else if (strcmp(IGMPEnableValue, "No") == 0){
		strcpy(cFlag, "false");
	}
	
	return getTheValue(value, type, deviceNode,cFlag);
}	

int8 setIGMPEnableValue(void* value, struct DeviceNode* deviceNode)
{
	uint8 flag;
	
	flag = *((uint8*)value);
	
	if(flag == 1){
		tcapi_set("IGMPproxy_Entry","IGMPEnable","Yes");
	} 
	else if (flag == 0){
		tcapi_set("IGMPproxy_Entry","IGMPEnable","No");
	}
	else{
		tc_printf("==>setIGMPEnableValue:value should be 0 or 1.\n");
		return -1;
	}	

	SetCfgNodeFlag ("IGMPproxy", -1);        
	return 0;
}

int8 getIGMPEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIPTVAttr.aIGMPEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	
	return result;
}

int8 setIGMPEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcIPTVAttr.aIGMPEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= IPTV_ATTR_FLAG;
	return result;
}	

int8 getProxyEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char IGMPProxyValue[10] = {0};
	char cFlag[8] =  {0};

	tcapi_get("IGMPproxy_Entry","Active", IGMPProxyValue);

	if(strcmp(IGMPProxyValue, "Yes") == 0){
		strcpy(cFlag, "true");
	} 
	#if 0
	else if (strcmp(IGMPProxyValue, "No") == 0){
		strcpy(cFlag, "false");
	}
	#else
	else{
		strcpy(cFlag, "false");
	}
	#endif
	
	return getTheValue(value, type, deviceNode,cFlag);
}	

int8 setProxyEnableValue(void* value, struct DeviceNode* deviceNode)
{
	uint8 flag;

	flag = *((uint8*)value);
	
	if(flag == 1){
		tcapi_set("IGMPproxy_Entry","Active","Yes");
	} 
	else if (flag == 0){
		tcapi_set("IGMPproxy_Entry","Active","No");
	}
	else{
		tc_printf("==>setProxyEnableValue:value should be 0 or 1.\n");
		return -1;
	}	
	
	SetCfgNodeFlag ("IGMPproxy", -1);
	return 0;
}

int8 getProxyEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIPTVAttr.aProxyEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	return result;
}		
int8 setProxyEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcIPTVAttr.aProxyEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= IPTV_ATTR_FLAG;
	
	return result;
}	

int8 getSnoopingEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char IGMPSnoopingValue[10] = {0};
	char cFlag[8] =  {0};

	tcapi_get("Lan_IgmpSnoop","igmpsnoopEnable", IGMPSnoopingValue);	
	if(strcmp(IGMPSnoopingValue, "Yes") == 0){
		strcpy(cFlag, "true");
	} 
	else if (strcmp(IGMPSnoopingValue, "No") == 0){
		strcpy(cFlag, "false");
	}
	
	return getTheValue(value, type, deviceNode,cFlag);
}	

int8 setSnoopingEnableValue(void* value, struct DeviceNode* deviceNode)
{
	uint8 flag;
	flag = *((uint8*)value);
	
	if(flag == 1){
		tcapi_set("Lan_IgmpSnoop","igmpsnoopEnable","Yes");
	} 
	else if (flag == 0){
		tcapi_set("Lan_IgmpSnoop","igmpsnoopEnable","No");
	}
	else{
		tc_printf("==>setSnoopingEnableValue:value should be 0 or 1.\n");
		return -1;
	}

	SetCfgNodeFlag ("Lan", -1);
	return 0;
}

int8 getSnoopingEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIPTVAttr.aSnoopingEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSnoopingEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcIPTVAttr.aSnoopingEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= IPTV_ATTR_FLAG;
	return result;
}	
int8 getSTBNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char stbNum[20] = {'\0'};

#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_STBMAC_REPORT)
	sprintf(stbNum, "%d", getSTBCount());
#else
        tcapi_get("Dhcpd_Option60","STBNum", stbNum);
#endif

	return getTheValue(value, type, deviceNode, stbNum);
}	

int8 getSTBNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIPTVAttr.aSTBNum;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSTBNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcIPTVAttr.aSTBNum;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= IPTV_ATTR_FLAG;
	
	return result;
}	

#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_STBMAC_REPORT)
int8 getSTBMACValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	return getTheValue(value, type, deviceNode, getSTBMACList());
}
int8 getSTBMACAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIPTVAttr.aSTBMAC;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setSTBMACAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIPTVAttr.aSTBMAC;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= IPTV_ATTR_FLAG;

	return result;
}
#endif
#endif
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8	getSTBRestrictEnableServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char enable[15] = {0};

	tcapi_get("Accesslimit_Entry0","Active",enable);

	if(strcmp(enable, "Yes") == 0)
		sprintf(enable, "%s", "1");
	else
		sprintf(enable, "%s", "0");
	
	return getTheValue(value, type, deviceNode, enable);
}

int8	setSTBRestrictEnableServicesValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 enableServices = 0;
	
	if( value == NULL)
		return -1;
	
	enableServices = *((uint8*)value);
	if(enableServices == 1){
		tcapi_set("Accesslimit_Entry0","Active", "Yes");
	}
	else if(enableServices == 0){
		tcapi_set("Accesslimit_Entry0","Active", "No");
	}
	else{
		
		return -1;
	}

	SetCfgNodeFlag("Accesslimit", 0);
	return 0;
}

int8	getSTBRestrictEnableServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcaccessLimitAttr.aSTBEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setSTBRestrictEnableServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcaccessLimitAttr.aSTBEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8	getSTBNumberServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char stbNum[10] = {0};

    tcapi_get("Accesslimit_Entry0","limitnum", stbNum);
	
	return getTheValue(value, type, deviceNode, stbNum);

}

int8	setSTBNumberServicesValue(void* value, struct DeviceNode*  deviceNode)
{
	int number = 0;
	char tmpbuf[15] = {0};
	
	if( value == NULL)
		return -1;
	
	number = *((int*)value);
	if(number < -1)
	{
		tc_printf("\r\nSTBNumber should not be negative!");
		return -1;
	}
	
	sprintf(tmpbuf, "%d", number);
	
	tcapi_set("Accesslimit_Entry0","limitnum", tmpbuf);

	SetCfgNodeFlag("Accesslimit", 0); 
	return 0;
}

int8	getSTBNumberServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcaccessLimitAttr.aSTBNumber;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setSTBNumberServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcaccessLimitAttr.aSTBNumber;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8	getCameraRestrictEnableServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char enable[15] = {0};

	tcapi_get("Accesslimit_Entry1","Active",enable);

	if(strcmp(enable, "Yes") == 0)
		sprintf(enable, "%s", "1");
	else
		sprintf(enable, "%s", "0");
		
	return getTheValue(value, type, deviceNode, enable);
}


int8	setCameraRestrictEnableServicesValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 enableServices = 0;
	
	if( value == NULL)
		return -1;
	
	enableServices = *((uint8*)value);
	if(enableServices == 1){
		tcapi_set("Accesslimit_Entry1","Active", "Yes");
	}
	else if(enableServices == 0){
		tcapi_set("Accesslimit_Entry1","Active", "No");
	}
	else{
		
		return -1;
	}


	SetCfgNodeFlag("Accesslimit", 1);
	return 0;

}

int8	getCameraRestrictEnableServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcaccessLimitAttr.aCameraEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setCameraRestrictEnableServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcaccessLimitAttr.aCameraEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8	getCameraNumberServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char cameraNum[10] = {0};

    tcapi_get("Accesslimit_Entry1","limitnum", cameraNum);
	
	return getTheValue(value, type, deviceNode, cameraNum);

}

int8	setCameraNumberServicesValue(void* value, struct DeviceNode*  deviceNode)
{
	int number = 0;
	char tmpbuf[15] = {0};
	
	if( value == NULL)
		return -1;
	
	number = *((int*)value);
	if(number < -1)
	{
		tc_printf("\r\nCameraNumber should not be negative!");
		return -1;
	}
	
	sprintf(tmpbuf, "%d", number);
	
	tcapi_set("Accesslimit_Entry1","limitnum", tmpbuf);

	SetCfgNodeFlag("Accesslimit", 1);
	return 0;

}

int8	getCameraNumberServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcaccessLimitAttr.aCameraNumber;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setCameraNumberServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcaccessLimitAttr.aCameraNumber;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8	getComputerRestrictEnableServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char enable[15] = {0};

	tcapi_get("Accesslimit_Entry2","Active",enable);

	if(strcmp(enable, "Yes") == 0)
		sprintf(enable, "%s", "1");
	else
		sprintf(enable, "%s", "0");
		
	return getTheValue(value, type, deviceNode, enable);

}

int8	setComputerRestrictEnableServicesValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 enableServices = 0;
	
	if( value == NULL)
		return -1;
	
	enableServices = *((uint8*)value);
	if(enableServices == 1){
		tcapi_set("Accesslimit_Entry2","Active", "Yes");
	}
	else if(enableServices == 0){
		tcapi_set("Accesslimit_Entry2","Active", "No");
	}
	else{
		
		return -1;
	}

	SetCfgNodeFlag("Accesslimit", 2);
	return 0;

}

int8	getComputerRestrictEnableServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcaccessLimitAttr.aComputerEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setComputerRestrictEnableServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcaccessLimitAttr.aComputerEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8	getComputerNumberServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char computerNum[10] = {0};

    tcapi_get("Accesslimit_Entry2","limitnum", computerNum);
	
	return getTheValue(value, type, deviceNode, computerNum);

}

int8	setComputerNumberServicesValue(void* value, struct DeviceNode*  deviceNode)
{
	int number = 0;
	char tmpbuf[15] = {0};
	
	if( value == NULL)
		return -1;
	
	number = *((int*)value);
	if(number < -1)
	{
		tc_printf("\r\nComputerNumber should not be negative!");
		return -1;
	}
	
	sprintf(tmpbuf, "%d", number);
	
	tcapi_set("Accesslimit_Entry2","limitnum", tmpbuf);

	SetCfgNodeFlag("Accesslimit", 2);
	return 0;

}

int8	getComputerNumberServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcaccessLimitAttr.aComputerNumber;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setComputerNumberServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcaccessLimitAttr.aComputerNumber;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8	getPhoneRestrictEnableServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char enable[15] = {0};

	tcapi_get("Accesslimit_Entry3","Active",enable);

	if(strcmp(enable, "Yes") == 0)
		sprintf(enable, "%s", "1");
	else
		sprintf(enable, "%s", "0");
		
	return getTheValue(value, type, deviceNode, enable);

}

int8	setPhoneRestrictEnableServicesValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 enableServices = 0;
	
	if( value == NULL)
		return -1;
	
	enableServices = *((uint8*)value);
	if(enableServices == 1){
		tcapi_set("Accesslimit_Entry3","Active", "Yes");
	}
	else if(enableServices == 0){
		tcapi_set("Accesslimit_Entry3","Active", "No");
	}
	else{
		
		return -1;
	}

	SetCfgNodeFlag("Accesslimit", 3);
	return 0;

}

int8	getPhoneRestrictEnableServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcaccessLimitAttr.aPhoneEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setPhoneRestrictEnableServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcaccessLimitAttr.aPhoneEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8	getPhoneNumberServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char phoneNum[10] = {0};

    tcapi_get("Accesslimit_Entry3","limitnum", phoneNum);
	
	return getTheValue(value, type, deviceNode, phoneNum);

}

int8	setPhoneNumberServicesValue(void* value, struct DeviceNode*  deviceNode)
{
	int number = 0;
	char tmpbuf[15] = {0};
	
	if( value == NULL)
		return -1;
	
	number = *((int*)value);
	if(number < -1)
	{
		tc_printf("\r\nPhoneNumber should not be negative!");
		return -1;
	}
	
	sprintf(tmpbuf, "%d", number);
	
	tcapi_set("Accesslimit_Entry3","limitnum", tmpbuf);

	SetCfgNodeFlag("Accesslimit", 3);
	return 0;

}

int8	getPhoneNumberServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcaccessLimitAttr.aPhoneNumber;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setPhoneNumberServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcaccessLimitAttr.aPhoneNumber;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getUserNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char nodeName[32] = {0};
	char nodeValue[32] = {0};
	int i, usernum = 0;

	for(i=0; i<MAX_USER_NUM; i++)
	{
		sprintf(nodeName, "User_Entry%d", i);
		if(tcapi_get(nodeName, "enable", nodeValue) == 0)
			usernum++;
	}
	
	sprintf(tempValue, "%d", usernum);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setUserNumberOfEntriesValue(void* value, struct DeviceNode*  deviceNode)
{
	int num = *((int*)value);
	
	
	return 0;
}
int8 getUserNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aUserNumberOfEntries;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setUserNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcCUAttr.aUserNumberOfEntries;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);	
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getFTPServerNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	sprintf(tempValue, "%d", 1);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setFTPServerNumberOfEntriesValue(void* value, struct DeviceNode*  deviceNode)
{
	int num = *((int*)value);
	
	
	return 0;
}
int8 getFTPServerNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aFTPServerNumberOfEntries;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setFTPServerNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcCUAttr.aFTPServerNumberOfEntries;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}

int8 getFTPClientNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	char nodeName[32] = {0};
	char nodeValue[32] = {0};
	int i, ftpcltnum = 0;

	for(i=APPFTPCLIENT_NUM; i<MAX_APPFTP_ENTRY; i++)
	{
		sprintf(nodeName, "appFTP_Entry%d", i);
		if(tcapi_get(nodeName, "Enable", nodeValue) == 0)
			ftpcltnum++;
	}
	
	sprintf(tempValue, "%d", ftpcltnum);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setFTPClientNumberOfEntriesValue(void* value, struct DeviceNode*  deviceNode)
{
	int num = *((int*)value);

	return 0;
}
int8 getFTPClientNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCUAttr.aFTPClientNumberOfEntries;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setFTPClientNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcCUAttr.aFTPClientNumberOfEntries;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CU_ATTR_FLAG;
	return result;
}
#endif

