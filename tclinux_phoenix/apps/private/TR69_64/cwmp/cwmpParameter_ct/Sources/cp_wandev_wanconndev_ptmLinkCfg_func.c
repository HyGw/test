#include "Global_res.h"
#include "cp_wandev_wanconndev_ptmLinkCfg.h"

/*rpc of parameters:InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANPTMLinkConfig.*/
int8 getWANPTMLinkCfgEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char dslCfgEnable[10] = {'\0'};
	uint32 wanNodeIndex;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);
	if ( 0 != tcapi_get(nodeName, "PTMLinkEnable", dslCfgEnable) )
		strcpy(dslCfgEnable, "Yes");

	if( 0 == strcmp(dslCfgEnable, "Yes") )
		strcpy(dslCfgEnable, "1");
	else
		strcpy(dslCfgEnable, "0");

	return getTheValue(value, type, deviceNode, dslCfgEnable);
}
int8 setWANPTMLinkCfgEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char enable[10] = {'\0'};
	uint8 ena;
	uint32 wanNodeIndex;
	int ret;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);
	ret = tcapi_get(nodeName, "Active", enable);
	if(ret){ //this pvc is not exist.
		return -1;
	}

	ena = *((uint8*)value);
	if(ena == 1){
		strcpy(enable, "Yes");
	}
	else if(ena == 0){
		strcpy(enable, "No");
	}
	else{
		tc_printf("==>setWANPTMLinkCfg1EnableValue: Enable value must be 0 or 1.\n");
		return -1;
	}

	
	tcapi_set(nodeName, "PTMLinkEnable", enable);
	SetCfgNodeFlag( "Wan", wanNodeIndex );
	return 0;
}

int8 getWANPTMLinkCfgEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgEnable[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDPTMLinkCfgEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	
	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgEnable[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1|= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgLinkStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char adslStatus[20] = {"Unavailable"};

	tcapi_get("Info_Adsl", "lineState", adslStatus);
	
	if(!strcmp(adslStatus, "up")){
		strcpy(adslStatus, "Up");
	}
	else if(!strcmp(adslStatus, "down")){
		strcpy(adslStatus, "Down");
	}
	else if(!strcmp(adslStatus, "initializing")){
		strcpy(adslStatus, "Initializing");
	}

	return getTheValue(value, type, deviceNode, adslStatus);
}
int8 getWANPTMLinkCfgLinkStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgLinkStatus[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgLinkStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgLinkStatus[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}


int8 getWANPTMLinkCfgMACAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WANCntMACAddress[64] = {0};

	tcapi_get("Info_Ptm", "HWAddr", WANCntMACAddress);

	return getTheValue(value, type, deviceNode, WANCntMACAddress);

}

int8 getWANPTMLinkCfgMACAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgMACAddress[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANPTMLinkCfgMACAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgMACAddress[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgBytesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WANCntByteSent[64] = {0};

	if ( 0 != tcapi_get("Info_Ptm", "BytesSent", WANCntByteSent) )
		strcpy(WANCntByteSent, "0");

	return getTheValue(value, type, deviceNode, WANCntByteSent);
}
int8 getWANPTMLinkCfgBytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgBytesSent[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgBytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgBytesSent[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WANCntByteReceived[64] = {0};

	if ( 0 != tcapi_get("Info_Ptm", "BytesReceived", WANCntByteReceived) )
		strcpy(WANCntByteReceived, "0");

	return getTheValue(value, type, deviceNode, WANCntByteReceived);
}
int8 getWANPTMLinkCfgBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgBytesReceived[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgBytesReceived[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgFramesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WANCntFramesSent[64] = {0};

	if ( 0 != tcapi_get("Info_Ptm", "FramesSent", WANCntFramesSent) )
		strcpy(WANCntFramesSent, "0");

	return getTheValue(value, type, deviceNode, WANCntFramesSent);

}
int8 getWANPTMLinkCfgFramesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgFramesSent[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgFramesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgErrorsSend[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgFramesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WANCntFramesReceived[64] = {0};

	if ( 0 != tcapi_get("Info_Ptm", "FramesReceived", WANCntFramesReceived) )
		strcpy(WANCntFramesReceived, "0");

	return getTheValue(value, type, deviceNode, WANCntFramesReceived);

}
int8 getWANPTMLinkCfgFramesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgFramesReceived[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgFramesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgFramesReceived[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgOOSNearEndValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	strcpy(result, "0");

	return getTheValue(value, type, deviceNode, result);
}
int8 getWANPTMLinkCfgOOSNearEndAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgOOSNearEnd[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgOOSNearEndAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgOOSNearEnd[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgOOSFarEndValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	strcpy(result, "0");

	return getTheValue(value, type, deviceNode, result);
}
int8 getWANPTMLinkCfgOOSFarEndAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgOOSFarEnd[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgOOSFarEndAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgOOSFarEnd[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgErrorsSendValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WANCntErrorsSent[64] = {0};

	if ( 0 != tcapi_get("Info_Ptm", "ErrorsSent", WANCntErrorsSent) )
		strcpy(WANCntErrorsSent, "0");
	
	return getTheValue(value, type, deviceNode, WANCntErrorsSent);	
}
int8 getWANPTMLinkCfgErrorsSendAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgErrorsSend[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgErrorsSendAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgErrorsSend[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgErrorsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WANCntErrorsReceived[64] = {0};

	if ( 0 != tcapi_get("Info_Ptm", "ErrorsReceived", WANCntErrorsReceived) )
		strcpy(WANCntErrorsReceived, "0");

	return getTheValue(value, type, deviceNode, WANCntErrorsReceived);

}
int8 getWANPTMLinkCfgErrorsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgErrorsReceived[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgErrorsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgErrorsReceived[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgUnicastPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WANCntUnicastPacketsSent[64] = {0};
	
	if ( 0 != tcapi_get("Info_Ptm", "UnicastPacketsSent", WANCntUnicastPacketsSent) )
		strcpy(WANCntUnicastPacketsSent, "0");

	return getTheValue(value, type, deviceNode, WANCntUnicastPacketsSent);	

}
int8 getWANPTMLinkCfgUnicastPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgUnicastPacketsSent[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgUnicastPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgUnicastPacketsSent[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgUnicastPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WANCntUnicastPacketsReceived[64] = {0};
	
	if ( 0 != tcapi_get("Info_Ptm", "UnicastPacketsReceived", WANCntUnicastPacketsReceived) )
		strcpy(WANCntUnicastPacketsReceived, "0");

	return getTheValue(value, type, deviceNode, WANCntUnicastPacketsReceived);	

}
int8 getWANPTMLinkCfgUnicastPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgUnicastPacketsReceived[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgUnicastPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgUnicastPacketsReceived[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgDiscardPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WANCntDiscardPacketsSent[64] = {0};

	if ( 0 != tcapi_get("Info_Ptm", "DiscardPacketsSent", WANCntDiscardPacketsSent) )
		strcpy(WANCntDiscardPacketsSent, "0");

	return getTheValue(value, type, deviceNode, WANCntDiscardPacketsSent);	

}
int8 getWANPTMLinkCfgDiscardPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgDiscardPacketsSent[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgDiscardPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgDiscardPacketsSent[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgDiscardPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WANCntDiscardPacketsReceived[64] = {0};

	if ( 0 != tcapi_get("Info_Ptm", "DiscardPacketsReceived", WANCntDiscardPacketsReceived) )
		strcpy(WANCntDiscardPacketsReceived, "0");

	return getTheValue(value, type, deviceNode, WANCntDiscardPacketsReceived);	

}
int8 getWANPTMLinkCfgDiscardPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgDiscardPacketsReceived[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgDiscardPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgDiscardPacketsReceived[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgMulticastPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WANCntMulticastPacketsSent[64] = {0};
	
	if ( 0 != tcapi_get("Info_Ptm", "MulticastPacketsSent", WANCntMulticastPacketsSent) )
		strcpy(WANCntMulticastPacketsSent, "0");

	return getTheValue(value, type, deviceNode, WANCntMulticastPacketsSent);	

}
int8 getWANPTMLinkCfgMulticastPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgMulticastPacketsSent[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgMulticastPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgMulticastPacketsSent[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgMulticastPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WANCntMulticastPacketsReceived[64] = {0};
	
	if ( 0 != tcapi_get("Info_Ptm", "MulticastPacketsReceived", WANCntMulticastPacketsReceived) )
		strcpy(WANCntMulticastPacketsReceived, "0");

	return getTheValue(value, type, deviceNode, WANCntMulticastPacketsReceived);	

}
int8 getWANPTMLinkCfgMulticastPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgMulticastPacketsReceived[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgMulticastPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgMulticastPacketsReceived[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgBroadcastPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	strcpy(result, "0");

	return getTheValue(value, type, deviceNode, result);
}
int8 getWANPTMLinkCfgBroadcastPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgBroadcastPacketsSent[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgBroadcastPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgBroadcastPacketsSent[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgBroadcastPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	strcpy(result, "0");

	return getTheValue(value, type, deviceNode, result);
}
int8 getWANPTMLinkCfgBroadcastPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgBroadcastPacketsReceived[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgBroadcastPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgBroadcastPacketsReceived[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}

int8 getWANPTMLinkCfgUnknownProtoPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	strcpy(result, "0");

	return getTheValue(value, type, deviceNode, result);
}
int8 getWANPTMLinkCfgUnknownProtoPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPtmLinkAttr.aWANPTMLinkCfgUnknownProtoPacketsReceived[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPTMLinkCfgUnknownProtoPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPtmLinkAttr.aWANPTMLinkCfgUnknownProtoPacketsReceived[getWANLinkIndex(deviceNode)%MAX_WAN_NODE];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= PTMLINK_ATTR_FLAG;
	return result;
}
