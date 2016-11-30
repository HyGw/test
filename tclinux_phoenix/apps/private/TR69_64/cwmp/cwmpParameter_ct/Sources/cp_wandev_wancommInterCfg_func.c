#include "Global_res.h"
#include "cp_wandev_wancommInterCfg.h"

int8 getEnabledForInternetValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	sprintf(result,"%d",1);	
	return getTheValue(value, type, deviceNode, result);
}

int8 setEnabledForInternetValue(void* value, struct DeviceNode*  deviceNode)
{	
	uint8 iEnable;
	iEnable = *((uint8*)value);

	if (!iEnable)
	{
		return -1; 
	}
	return 0;
}

int8 getEnabledForInternetAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslComInterCfgAttr.aEnabledForInternet;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setEnabledForInternetAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslComInterCfgAttr.aEnabledForInternet;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANAccessTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char linkSta[4] = "0";
	int ponsw = 0;

#if defined(TCSUPPORT_CT_PON)
	tcapi_get("XPON_LinkCfg", "LinkSta", linkSta);
	ponsw = atoi(linkSta);
	if ( 2 == ponsw ) // epon
		snprintf(result, sizeof(result), "EPON");
	else
		snprintf(result, sizeof(result), "GPON");
#else
	strcpy(result,"DSL");	
#endif
	return getTheValue(value, type, deviceNode, result);
}

int8 getWANAccessTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslComInterCfgAttr.aWANAccessType;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANAccessTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslComInterCfgAttr.aWANAccessType;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8 getLayer1UpstreamMaxBitRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char linkSta[4] = "0";
	int ponsw = 0;
	char Opmode[40] = {0};
	
#if defined(TCSUPPORT_CT_PON)
	tcapi_get("XPON_LinkCfg", "LinkSta", linkSta);
	ponsw = atoi(linkSta);
	if ( 2 == ponsw ) // epon
		sprintf(result, "%u", 1000000000);
	else
		sprintf(result, "%u", 1244160000);
#else
#if defined(TCSUPPORT_CT_WAN_PTM)	
	tcapi_get("Info_Adsl","Opmode",Opmode);
	if(strstr(Opmode,"VDSL") != NULL)
		sprintf(result, "%u", 100000000);
	else
#endif
	sprintf(result, "%u", 3480000); 	
#endif
	return getTheValue(value, type, deviceNode, result);
}

int8 getLayer1UpstreamMaxBitRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslComInterCfgAttr.aLayer1UpstreamMaxBitRate;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLayer1UpstreamMaxBitRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslComInterCfgAttr.aLayer1UpstreamMaxBitRate;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8 getLayer1DownstreamMaxBitRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char linkSta[4] = "0";
	int ponsw = 0;
	char Opmode[40] = {0};
	
#if defined(TCSUPPORT_CT_PON)
	tcapi_get("XPON_LinkCfg", "LinkSta", linkSta);
	ponsw = atoi(linkSta);
	if ( 2 == ponsw ) // epon
		sprintf(result, "%u", 1000000000);
	else
		sprintf(result, "%u", 2488320000);
#else
#if defined(TCSUPPORT_CT_WAN_PTM)	
	tcapi_get("Info_Adsl","Opmode",Opmode);
	if(strstr(Opmode,"VDSL") != NULL)
		sprintf(result, "%u", 100000000);
	else
#endif
	sprintf(result, "%u", 28800000);		
#endif

	return getTheValue(value, type, deviceNode, result);
}

int8 getLayer1DownstreamMaxBitRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslComInterCfgAttr.aLayer1DownstreamMaxBitRate;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLayer1DownstreamMaxBitRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslComInterCfgAttr.aLayer1DownstreamMaxBitRate;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8 getPhysicalLinkStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
			
#if defined(TCSUPPORT_CT_PON)
	tcapi_get("XPON_LinkCfg","LinkSta",strTemp);
	if(!strcmp(strTemp, "0"))
	{
		tcapi_get("XPON_Common", "phyStatus",result);
		if(!strstr(result,"up"))
			sprintf(result,"Initializing");
		else
			sprintf(result,"NoSignal");			
	}
	else
	{
		sprintf(result,"Up");
	}
#else
	if(tcapi_get(INFOADSL_NODE, "lineState", strTemp) != 0)
	{
		strcpy(result, "Down");
		return getTheValue(value, type, deviceNode, result);
	}
	
	if(0 == strcmp(strTemp, "down"))
		strcpy(result,  "Down" );
	else if(0 == strcmp(strTemp, "wait for init") || 0 == strcmp(strTemp, "initializing"))
		strcpy(result,  "Initializing" );
	else if(0 == strcmp(strTemp, "up"))
		strcpy(result,  "Up" );
	else
		strcpy(result, "Unavailable");
#endif
	return getTheValue(value, type, deviceNode, result);
}

int8 getPhysicalLinkStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslComInterCfgAttr.aPhysicalLinkStatus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setPhysicalLinkStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslComInterCfgAttr.aPhysicalLinkStatus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8 getTotalBytesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};

#if defined(TCSUPPORT_CT_PON)
	if(tcapi_get("Info_PonWanStats","TxFrameLen",strTemp) != 0)
#else
	if(tcapi_get(INFOADSL_NODE, "outBytes", strTemp) != 0)
#endif
	{
		strcpy(result, "0");
		return getTheValue(value, type, deviceNode, result);
	}
	strcpy(result, strTemp);
	return getTheValue(value, type, deviceNode, result);
}

int8 getTotalBytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslComInterCfgAttr.aTotalBytesSent;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTotalBytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslComInterCfgAttr.aTotalBytesSent;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8 getTotalBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
#if defined(TCSUPPORT_CT_PON)
	if(tcapi_get("Info_PonWanStats","RxFrameLen",strTemp)!=0)
#else
	if(tcapi_get(INFOADSL_NODE, "inBytes", strTemp) != 0)
#endif
	{
		strcpy(result, "0");
		return getTheValue(value, type, deviceNode, result);
	}
	strcpy(result, strTemp);

	return getTheValue(value, type, deviceNode, result);
}

int8 getTotalBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslComInterCfgAttr.aTotalBytesReceived;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTotalBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslComInterCfgAttr.aTotalBytesReceived;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8 getTotalPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
#if defined(TCSUPPORT_CT_PON)
	if(tcapi_get("Info_PonWanStats","TxFrameCnt",strTemp) != 0)
#else
	if(tcapi_get(INFOADSL_NODE, "outPkts", strTemp) != 0)
#endif
	{
		strcpy(result, "0");
	}
	else
		strcpy(result, strTemp);

	return getTheValue(value, type, deviceNode, result);
}

int8 getTotalPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslComInterCfgAttr.aTotalPacketsSent;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTotalPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslComInterCfgAttr.aTotalPacketsSent;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8 getTotalPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
	
#if defined(TCSUPPORT_CT_PON)
	if(tcapi_get("Info_PonWanStats","RxFrameCnt",strTemp) != 0)
#else
	if(tcapi_get(INFOADSL_NODE, "inPkts", strTemp) != 0)
#endif
	{
		strcpy(result, "0");
		return getTheValue(value, type, deviceNode, result);
	}
	strcpy(result, strTemp);

	return getTheValue(value, type, deviceNode, result);
}

int8 getTotalPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslComInterCfgAttr.aTotalPacketsReceived;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTotalPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslComInterCfgAttr.aTotalPacketsReceived;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

#if defined(TCSUPPORT_CT_PON_SC)
int8 getEnabledBRDHCPCheckValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	tcapi_get("Wan_Common", "BrDHCPChk", result);
	if ( 0 == strcmp(result, "Yes") )
		strcpy(result, "1");
	else
		strcpy(result, "0");

	return getTheValue(value, type, deviceNode, result);
}
int8 setEnabledBRDHCPCheckValue(void* value, struct DeviceNode*  deviceNode)
{	
	uint8 iEnable;

	iEnable = *((uint8*)value);
	if ( 1 == iEnable )
		tcapi_set("Wan_Common", "BrDHCPChk", "Yes");
	else if ( 0 == iEnable )
		tcapi_set("Wan_Common", "BrDHCPChk", "No");
	else
		return -1;

	SetCfgNodeFlag("Wan_Common", -1);

	return 0;
}
int8 getEnabledBRDHCPCheckAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcdslComInterCfgAttr.aEnabledBRDHCPCheck;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setEnabledBRDHCPCheckAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcdslComInterCfgAttr.aEnabledBRDHCPCheck;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}
#endif

