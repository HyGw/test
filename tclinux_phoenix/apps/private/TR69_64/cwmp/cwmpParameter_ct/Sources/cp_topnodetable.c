#include "Global_res.h"
//#include "AsEngine.h"
#include "cp_topnodetable.h"

#if 0
#include "cp_core.h"
#include "cp_managerment.h"
#include "cp_IPPingDiagnostics.h"
#include "cp_landev.h"
#include "cp_wandev.h"
#endif

static NodeFunc DeviceSummary=
{
	NULL,
	NULL,
	getDeviceSummaryValue, 
	NULL, 
	getDeviceSummaryAttribute, 
	setDeviceSummaryAttribute, 
	NULL,
};

static NodeFunc LANDeviceNumberOfEntries=
{
	NULL,
	NULL,
	getLanDeviceNumberOfEntriesValue, 
	NULL, 
	getLanDeviceNumberOfEntriesAttribute, 
	setLanDeviceNumberOfEntriesAttribute, 
	NULL,
};

static NodeFunc WANDeviceNumberOfEntries=
{
	NULL,
	NULL,
	getWanDeviceNumberOfEntries, 
	NULL, 
	getWanDeviceNumberOfEntriesAttribute, 
	setWanDeviceNumberOfEntriesAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
static NodeFunc POTSDeviceNumber=
{
        NULL,
        NULL,
        getPOTSDeviceNumberValue,
        NULL,
        getPOTSDeviceNumberAttribute,
        setPOTSDeviceNumberAttribute,
        NULL,
};
#endif

/*
	Parent Node: InternetGatewayDevice.
	Parent Node Table Location: cp_topnodetable.c 
*/
CwmpParameterStruct InternetGatewayDevice[] = 
{
	{"DeviceSummary", StrR, NoLimite, &DeviceSummary, NULL},
#ifdef CT_COM_DEVICEREG
	{X_CT_COM"UserInfo", ObjectR, NoLimite, NULL, DeviceRegistration},
#endif
#if !defined(TCSUPPORT_CT_PON_JX) || defined(TCSUPPORT_CT_PON_HUB)
	{"LANDeviceNumberOfEntries", UnsignIntR, NoLimite, &LANDeviceNumberOfEntries, NULL},
	{"WANDeviceNumberOfEntries", UnsignIntR, NoLimite, &WANDeviceNumberOfEntries, NULL},
#endif
	{"DeviceInfo", ObjectR, NoLimite, NULL, DeviceInfo},
	{"ManagementServer", ObjectR, NoLimite, NULL, ManagementServer},	
	{"IPPingDiagnostics", ObjectR, NoLimite, NULL, IPPingDiagnostics},
#ifdef TCSUPPORT_TRACEROUTE
	{"TraceRouteDiagnostics", ObjectR, NoLimite, NULL, TraceRouteDiagnostics},
#endif
	{"LANDevice", ObjectR, NoLimite, NULL, LanDeviceIndex},	
	{"WANDevice", ObjectR, NoLimite, NULL, WANDeviceIndex},
	{"Services", ObjectR, NoLimite, NULL, Services},
#if !defined(TCSUPPORT_CT_PON_JX) || defined(TCSUPPORT_CT_PON_HUB)
	{"Time", ObjectR, NoLimite, NULL, Time},
	{"DeviceConfig", ObjectR, NoLimite, NULL, DeviceConfig},
	{"Layer3Forwarding", ObjectR, NoLimite, NULL, Layer3Forwarding},
	{"LANConfigSecurity", ObjectR, NoLimite, NULL, LanConfigSecurity},	
#endif
#if defined(TCSUPPORT_CT_QOS)
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{QOS_NAME_UPLINK, ObjectR, NoLimite, NULL, X_CTCOM_UplinkQoS},
#endif
#else
	{"QueueManagement", ObjectR, NoLimite, NULL, QueueManagement},
#endif
#if !defined(TCSUPPORT_CT_PON_JX) || defined(TCSUPPORT_CT_PON_HUB)
	{"UserInterface", ObjectR, NoLimite, NULL, UserInterface},
#endif
#ifdef TR143
	{"Capabilities", ObjectR, NoLimite, NULL, tr143_Capabilities},
	{"DownloadDiagnostics", ObjectR, NoLimite, NULL, DownloadDiagnostics},
	{"UploadDiagnostics", ObjectR, NoLimite, NULL, UploadDiagnostics},
	{"UDPEchoConfig", ObjectR, NoLimite, NULL, UDPEchoConfig},
#endif
#if defined(TCSUPPORT_C1_CUC) || defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CU_FUNCTION, ObjectR, NoLimite, NULL, X_CU_Function},
#endif
#if defined(TCSUPPORT_CT_UPNP_DM)
	{X_CT_PROXYDEVICE, ObjectR, NoLimite, NULL, X_CTCOM_ProxyDevice},
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CU"POTSDeviceNumber", UnsignIntR, NoLimite, &POTSDeviceNumber, NULL},
#endif
#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
	{X_CT_COM"PPPOE_EMULATOR", ObjectR, NoLimite, NULL, PppoeEmulator},
	{X_CT_COM"PPPOE_BridgeAutoEmulator", ObjectR, NoLimite, NULL, PppoeEmulatorBridge},
#endif
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
	{X_CT_COM"IPoEDiagnostics", ObjectR, NoLimite, NULL, IPoeEmulator},
#endif
#if defined(TCSUPPORT_CT_PHONEAPP)
	{X_CT_COM"Mgt", ObjectR, NoLimite, NULL, X_CTCOM_Mgt},
#endif
#if defined(TCSUPPORT_CMCC)
	{"LANInterfaces", ObjectR, NoLimite, NULL, LANInterfaces},
	{X_CMCC"Security", ObjectR, NoLimite, NULL, Security},
#ifdef TCSUPPORT_IPV6
	{X_CMCC"IPv6Layer3Forwarding", ObjectR, NoLimite, NULL, IPv6Layer3Forwarding},
#endif
#endif
#if defined(TCSUPPORT_CT_JOYME)
	{X_CT_COM"DNSSpeedLimit", ObjectR, NoLimite, NULL, DnsSpeedLimit},
#endif
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: NULL
	Parent Node Table Location: 
*/
CwmpParameterStruct TopIGDNode[] = 
{
	{"InternetGatewayDevice", ObjectR, NoLimite, NULL, InternetGatewayDevice},	
	{NULL, -1, -1, NULL, NULL}
};
int8 getDeviceSummaryValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* tempValue;

	tempValue = getDeviceSummary();

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getDeviceSummaryAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aDeviceSummary;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDeviceSummaryAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aDeviceSummary;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getLanDeviceNumberOfEntriesValue(char** value, ParameterType* type,  DeviceNode*  deviceNode)
{
	return getTheValue(value, type, deviceNode, "1");
}
int8 getLanDeviceNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aLANDeviceNumberOfEntries;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLanDeviceNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode)
{
	uint8*  attribute;
	int8  result;
	
	attribute = &tcDeviceInfoAttr.aLANDeviceNumberOfEntries;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getWanDeviceNumberOfEntries(char** value, ParameterType* type,  DeviceNode*  deviceNode)
{
	return getTheValue(value, type, deviceNode, "1");
}
int8 getWanDeviceNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aWANDeviceNumberOfEntries;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWanDeviceNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aWANDeviceNumberOfEntries;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getPOTSDeviceNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if defined(TCSUPPORT_VOIP)
	char s_POTSDeviceNumber[4] = {0};
	sprintf(s_POTSDeviceNumber, "%d", LINENUM);
#else
	char s_POTSDeviceNumber[4] = "0";
#endif

	return getTheValue(value, type, deviceNode, s_POTSDeviceNumber);
}
int8 getPOTSDeviceNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
    uint8 attribute = 0;
    int8 result = 0;

    attribute = tcDeviceInfoAttr.aPOTSDeviceNumber;
    result = GetTheAttributes(attribute,notification, acl, deviceNode);
    return result;
}
int8 setPOTSDeviceNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
    uint8* attribute = NULL;
    int8 result = 0;

    attribute = &tcDeviceInfoAttr.aPOTSDeviceNumber;
    result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
    return result;
}
#endif

