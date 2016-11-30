#include "Global_res.h"
#include "cp_landev_ethInterCfg.h"
static  char  gmaxBitRate[3][8] = {"10", "100","Auto"};
static  char  gduplexMode[3][8] = {"Half", "Full","Auto"};
int8 getEthInterCfg_EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char nodeName[34] = {0};
	char result[20] = {0};
	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "SwitchPara_Entry%d", instance);
	tcapi_get(nodeName, "enable", result);
	if(result[0] == '\0' || !strcmp(result,"Yes"))
		strcpy(result,"1");
	else
		strcpy(result,"0");
	
	return getTheValue(value, type, deviceNode, result);
}

int8 setEthInterCfg_EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 flag;
	int instance;
	char temp[30] = {0};
	
	flag = *((uint8*)value);

	instance = atoi(deviceNode->parentNode->name) - 1;	
	
	SetCfgNodeFlag ( "SwitchPara", instance);
	sprintf(temp,"SwitchPara_Entry%d",instance);
	
	if(flag == 1){
		tcapi_set(temp,"enable","Yes");
	} 
	else if (flag == 0){
		tcapi_set(temp,"enable","No");
	}
	else{
		tc_printf("==>setEthInterCfg_EnableValue:value should be 0 or 1.\n");
		return -1;
	}	

	strcpy(temp, "SwitchPara_Common");
	tcapi_set(temp, "SwitchNeedOperation", "Yes");
	return 0;
}

int8 getEthInterCfg_EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcetherInterCfgAttr.aEnable[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setEthInterCfg_EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcetherInterCfgAttr.aEnable[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
}

int8 getEthInterCfg_StatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if 1   //LAN_ETHERNET_INTERFACE
	char nodeName[34] = {'\0'};
	char result[20] = {0}, tmp[8] = {0};
	int instance;
#if defined(TCSUPPORT_CT_E8GUI)
	char attrName[34] = {0};
#endif
	
	instance = atoi(deviceNode->parentNode->name);
#if defined(TCSUPPORT_CT_E8GUI)
	if(instance < 1 || instance > 4)
		return -1;
	
	sprintf(nodeName, "Info_Ether");
	sprintf(attrName, "Port%dStatus", instance);
	tcapi_get(nodeName, attrName, tmp);
#else
	sprintf(nodeName, "Info_eth0.%d", instance);
	tcapi_get(nodeName, "Status", tmp);
#endif

	if(strcmp(tmp, "0") == 0){
		strcpy(result, "NoLink"); 
	}
	else if(strcmp(tmp, "1") == 0){
		strcpy(result, "Up"); 
	}else{
		strcpy(result, "Disabled"); 
	}
	
	return getTheValue(value, type, deviceNode, result);
#else
	char result[20] = {0};

	tcapi_get("Info_Ether", "status", result);

	return getTheValue(value, type, deviceNode, result);
#endif
}

int8 getEthInterCfg_StatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
#if 1  //LAN_ETHERNET_INTERFACE
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcetherInterCfgAttr.aStatus[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
#else
	uint8  attribute;
	int8  result;

	attribute = tcetherInterCfgAttr.aStatus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
#endif
}

int8 setEthInterCfg_StatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
#if 1  //LAN_ETHERNET_INTERFACE
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcetherInterCfgAttr.aStatus[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
#else
	uint8*  attribute;
	int8  result;

	attribute = &tcetherInterCfgAttr.aStatus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
#endif
}

int8 getEthInterCfg_MACAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if 1   //LAN_ETHERNET_INTERFACE
	char nodeName[34] = {'\0'};
	char result[20] = {0};
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "Info_eth0.%d", instance);
	tcapi_get(nodeName, "hwaddr", result);
	
	return getTheValue(value, type, deviceNode, result);
#else
	char result[20] = {0};

	tcapi_get("Info_Ether", "mac", result);

	return getTheValue(value, type, deviceNode, result);
#endif
}

int8 getEthInterCfg_MACAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
#if 1  //LAN_ETHERNET_INTERFACE
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcetherInterCfgAttr.aMACAddress[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
#else
	uint8  attribute;
	int8  result;

	attribute = tcetherInterCfgAttr.aMACAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
#endif
}

int8 setEthInterCfg_MACAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
#if 1  //LAN_ETHERNET_INTERFACE
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcetherInterCfgAttr.aMACAddress[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
#else
	uint8*  attribute;
	int8  result;

	attribute = &tcetherInterCfgAttr.aMACAddress;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
#endif
}

int8 getStats_BytesStValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if !defined(TCSUPPORT_CT_1PORT)
	char nodeName[34] = {'\0'};
	char result[20] = {0};
	int instance;
	
	instance = atoi(deviceNode->parentNode->parentNode->name);	
	sprintf(nodeName, "Info_eth0.%d", instance);
	tcapi_get(nodeName, "txbytes", result);
	
	return getTheValue(value, type, deviceNode, result);
#else
	char result[20] = {0};

	tcapi_get("Info_Ether", "inOctets", result);

	return getTheValue(value, type, deviceNode, result);
#endif
}

int8 getStats_BytesStAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
#if 1  //LAN_ETHERNET_INTERFACE
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name);
	attribute = tcetherInterCfgAttr.aBytesSent[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
#else
	uint8  attribute;
	int8  result;

	attribute = tcetherInterCfgAttr.aBytesSent;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
#endif
}

int8 setStats_BytesStAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
#if 1  //LAN_ETHERNET_INTERFACE
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name);
	attribute = &tcetherInterCfgAttr.aBytesSent[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
#else
	uint8*  attribute;
	int8  result;

	attribute = &tcetherInterCfgAttr.aBytesSent;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
#endif
}

int8 getStats_BytesRvdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if !defined(TCSUPPORT_CT_1PORT)
	char nodeName[34] = {'\0'};
	char result[20] = {0};
	int instance;
	
	instance = atoi(deviceNode->parentNode->parentNode->name);	
	sprintf(nodeName, "Info_eth0.%d", instance);
	tcapi_get(nodeName, "rxbytes", result);
	
	return getTheValue(value, type, deviceNode, result);
#else
	char result[20] = {0};

	tcapi_get("Info_Ether", "outOctets", result);

	return getTheValue(value, type, deviceNode, result);
#endif
}

int8 getStats_BytesRvdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
#if 1  //LAN_ETHERNET_INTERFACE
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name);
	attribute = tcetherInterCfgAttr.aBytesReceived[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
#else
	uint8  attribute;
	int8  result;

	attribute = tcetherInterCfgAttr.aBytesReceived;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
#endif
}

int8 setStats_BytesRvdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
#if 1  //LAN_ETHERNET_INTERFACE
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name);
	attribute = &tcetherInterCfgAttr.aBytesReceived[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
#else
	uint8*  attribute;
	int8  result;

	attribute = &tcetherInterCfgAttr.aBytesReceived;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
#endif
}

int8 getStats_PacketsStValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if !defined(TCSUPPORT_CT_1PORT)
	char nodeName[34] = {'\0'};
	char result[20] = {0};
	int instance;
	
	instance = atoi(deviceNode->parentNode->parentNode->name);	
	sprintf(nodeName, "Info_eth0.%d", instance);
	tcapi_get(nodeName, "txpackets", result);
	
	return getTheValue(value, type, deviceNode, result);
#else
	char result[20] = {0};

	tcapi_get("Info_Ether", "rxFrames", result);

	return getTheValue(value, type, deviceNode, result);
#endif
}

int8 getStats_PacketsStAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
#if 1  //LAN_ETHERNET_INTERFACE
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name);
	attribute = tcetherInterCfgAttr.aPacketsSent[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
#else
	uint8  attribute;
	int8  result;

	attribute = tcetherInterCfgAttr.aPacketsSent;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
#endif
}

int8 setStats_PacketsStAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
#if 1  //LAN_ETHERNET_INTERFACE
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name);
	attribute = &tcetherInterCfgAttr.aPacketsSent[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
#else
	uint8*  attribute;
	int8  result;

	attribute = &tcetherInterCfgAttr.aPacketsSent;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
#endif
}

int8 getStats_PacketsRvdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if !defined(TCSUPPORT_CT_1PORT)
	char nodeName[34] = {'\0'};
	char result[20] = {0};
	int instance;
	
	instance = atoi(deviceNode->parentNode->parentNode->name);	
	sprintf(nodeName, "Info_eth0.%d", instance);
	tcapi_get(nodeName, "rxpackets", result);
	
	return getTheValue(value, type, deviceNode, result);
#else
	char result[20] = {0};

	tcapi_get("Info_Ether", "txFrames", result);

	return getTheValue(value, type, deviceNode, result);
#endif
}

int8 getStats_PacketsRvdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
#if 1  //LAN_ETHERNET_INTERFACE
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name);
	attribute = tcetherInterCfgAttr.aPacketsReceived[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
#else
	uint8  attribute;
	int8  result;

	attribute = tcetherInterCfgAttr.aPacketsReceived;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
#endif
}

int8 setStats_PacketsRvdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
#if 1  //LAN_ETHERNET_INTERFACE
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name);
	attribute = &tcetherInterCfgAttr.aPacketsSent[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
#else
	uint8*  attribute;
	int8  result;

	attribute = &tcetherInterCfgAttr.aPacketsReceived;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
#endif
}

int8 getEthInterCfg_MaxBitRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char result[20] = {0};
	int instance;
	
	instance = atoi(deviceNode->parentNode->name) - 1;	
	sprintf(nodeName, "SwitchPara_Entry%d", instance);
	tcapi_get(nodeName, "maxBitRate", result);
	
	if(result[0] == '\0')
		strcpy(result,"Auto");
	return getTheValue(value, type, deviceNode, result);
}

int8 setEthInterCfg_MaxBitRateValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	int instance,i =0;
	char result[8] = {0};

	instance = atoi(deviceNode->parentNode->name) - 1;	
	
	SetCfgNodeFlag ( "SwitchPara", instance);
	
	if(strlen((char*)value) > 4)
	{
		tc_printf("\r\nsetEthInterCfg_MaxBitRateValue:length is over 4!");
		return -1;
	}

	strcpy(result,(char*)value);
	for(i = 0; i < 3; i++)
	{
		if(!strcmp(result,gmaxBitRate[i]))
			break;
	}
	
	if(i == 3)
	{
		tc_printf("\r\nsetEthInterCfg_MaxBitRateValue:not support this value!");
		return -1;
	}
	
	strcpy(nodeName, "SwitchPara_Common");
	tcapi_set(nodeName, "SwitchNeedOperation", "Yes");
	sprintf(nodeName, "SwitchPara_Entry%d", instance);
	//tcapi_set(nodeName, "enable", "Yes");
	tcapi_set(nodeName, "maxBitRate", result);

	return 0;
	
}

int8 getEthInterCfg_MaxBitRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcetherInterCfgAttr.amaxBitRate[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setEthInterCfg_MaxBitRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcetherInterCfgAttr.amaxBitRate[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
}

int8 getEthInterCfg_DuplexModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char result[20] = {0};
	int instance;
	
	instance = atoi(deviceNode->parentNode->name) - 1;	
	sprintf(nodeName, "SwitchPara_Entry%d", instance);
	tcapi_get(nodeName, "duplexMode", result);
	
	if(result[0] == '\0')
		strcpy(result,"Auto");
	return getTheValue(value, type, deviceNode, result);
}

int8 setEthInterCfg_DuplexModeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	int instance,i =0;
	char result[8] = {0};
	
	instance = atoi(deviceNode->parentNode->name) - 1;	
	SetCfgNodeFlag ( "SwitchPara", instance);
	
	if(strlen((char*)value) > 4)
	{
		tc_printf("\r\nsetEthInterCfg_DuplexModeValue:length is over 4!");
		return -1;
	}

	strcpy(result,(char*)value);
	for(i = 0; i < 3; i++)
	{
		if(!strcmp(result,gduplexMode[i]))
			break;
	}
	
	if(i == 3)
	{
		tc_printf("\r\nsetEthInterCfg_MaxBitRateValue:not support this value!");
		return -1;
	}
	
	strcpy(nodeName, "SwitchPara_Common");
	tcapi_set(nodeName, "SwitchNeedOperation", "Yes");
	sprintf(nodeName, "SwitchPara_Entry%d", instance);
	//tcapi_set(nodeName, "enable", "Yes");
	tcapi_set(nodeName, "duplexMode", result);

	return 0;
	
}

int8 getEthInterCfg_DuplexModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcetherInterCfgAttr.aduplexMode[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setEthInterCfg_DuplexModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcetherInterCfgAttr.aduplexMode[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
}

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getEthInterCfg_X_CTCOM_ModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char result[20] = {0};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name) - 1;	
	sprintf(nodeName, "VlanBind_Entry%d", instance);

	tcapi_get(nodeName, "Active", result);
	if ( 0 == strcmp(result, "Yes") )
		strcpy(result, "1");
	else
		strcpy(result, "0");

	return getTheValue(value, type, deviceNode, result);
}

int8 setEthInterCfg_X_CTCOM_ModeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	int instance = 0;
	char result[8] = {0};
	
	instance = atoi(deviceNode->parentNode->name) - 1;	

	if(*(uint32*)value != 0 && *(uint32*)value != 1)
		return -1;

	switch ( *(uint32*)value )
	{
		case 1:
			strcpy(result, "Yes");
			break;
		default:
			strcpy(result, "No");
			break;
	}

	sprintf(nodeName, "VlanBind_Entry%d", instance);
	tcapi_set(nodeName, "Active", result);
	SetCfgNodeFlag ( "VlanBind", instance);

	return 0;
	
}

int8 getEthInterCfg_X_CTCOM_ModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcetherInterCfgAttr.aX_CTCOM_Mode[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setEthInterCfg_X_CTCOM_ModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcetherInterCfgAttr.aX_CTCOM_Mode[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
}

int8 getEthInterCfg_X_CTCOM_VLANValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char result[256] = {0};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name) - 1;	
	sprintf(nodeName, "VlanBind_Entry%d", instance);
	tcapi_get(nodeName, "mn", result);

	return getTheValue(value, type, deviceNode, result);

}

int8 setEthInterCfg_X_CTCOM_VLANValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	int instance = 0;
	char result[256] = {0};
	
	instance = atoi(deviceNode->parentNode->name) - 1;	

	sprintf(result, "%s", (char*)value);
	sprintf(nodeName, "VlanBind_Entry%d", instance);
	tcapi_set(nodeName, "mn", result);

	SetCfgNodeFlag ( "VlanBind", instance);

	return 0;
	
}

int8 getEthInterCfg_X_CTCOM_VLANAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcetherInterCfgAttr.aX_CTCOM_VLAN[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setEthInterCfg_X_CTCOM_VLANAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcetherInterCfgAttr.aX_CTCOM_VLAN[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
}

int8 getEthInterCfg_DetectionStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char result[20] = {0};
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	if ( !(instance >=1 && instance <=4) )
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Info_eth0.%d", instance);
	if ( tcapi_get(nodeName, "detectionStatus", result) < 0 )
		strcpy(result, "0");

	return getTheValue(value, type, deviceNode, result);
}
int8 getEthInterCfg_DetectionStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	if ( !(instance >=1 && instance <=4) )
		return -1;

	attribute = tcetherInterCfgAttr.aDetectionStatus[instance - 1];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setEthInterCfg_DetectionStatustAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	if ( !(instance >=1 && instance <=4) )
		return -1;

	attribute = &tcetherInterCfgAttr.aDetectionStatus[instance - 1];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
}
#endif

#if defined(TCSUPPORT_CT_PMINFORM)
int8 getEthInterCfg_AccessEquipmentMacValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char lanmacAttr[32] = {0};
	char result[32] = {0};
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	if ( !(instance >=1 && instance <=4) )
		return -1;

	sprintf(lanmacAttr, "Eth%dDevMAC", instance);
	tcapi_get("LanHost",lanmacAttr,result);

	return getTheValue(value, type, deviceNode, result);
}
int8 getEthInterCfg_AccessEquipmentMacAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	if ( !(instance >=1 && instance <=4) )
		return -1;

	attribute = tcetherInterCfgAttr.aAccessEquipmentMac[instance - 1];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setEthInterCfg_AccessEquipmentMacAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	if ( !(instance >=1 && instance <=4) )
		return -1;

	attribute = &tcetherInterCfgAttr.aAccessEquipmentMac[instance - 1];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
}
#endif

#if defined(TCSUPPORT_CMCC)
int8 getBSREnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char result[256] = {0}, attr[10] = {0};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name) - 1;	

	strcpy(nodeName, "Lan_BSR");
	sprintf(attr, "LAN%d", instance);

	tcapi_get(nodeName, attr, result);

	return getTheValue(value, type, deviceNode, result);

}

int8 setBSREnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	int instance = 0, flag = 0;
	char attr[256] = {0};
	
	instance = atoi(deviceNode->parentNode->name) - 1;	

	strcpy(nodeName, "Lan_BSR");
	sprintf(attr, "LAN%d", instance);

	flag = *((uint8*)value);

	if (flag == 1)
		tcapi_set(nodeName, attr, "1");
	else
		tcapi_set(nodeName, attr, "0");
	
	SetCfgNodeFlag ( "Lan_BSR", instance);

	return 0;
	
}

int8 getBSREnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcetherInterCfgAttr.aBSREnable[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setBSREnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcetherInterCfgAttr.aBSREnable[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= ETHERINTERCFG_ATTR_FLAG;
	return result;
}

#endif

