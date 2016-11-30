#ifndef CP_TOPNODETABLE_H_
#define CP_TOPNODETABLE_H_

#include "cp_core.h"

#if defined(TCSUPPORT_C1_CUC)
#define  QOS_NAME_UPLINK "X_CU_UplinkQoS"
#else
#define  QOS_NAME_UPLINK X_CT_COM"UplinkQoS"
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#define CU_ATTR "cuAttr"
#endif
#if defined(TCSUPPORT_C1_CUC) || defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#define CU_ACCOUNT_ATTR "cuAccountAttr"
#define CU_LOG_ATTR  "cuLogAttr"
#define X_CU_FUNCTION  "X_CU_Function"
#endif
#if defined(TCSUPPORT_CT_MIDWARE)
#define CT_MIDWARE_ATTR  "CTMidwareAttr"
#endif
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#define CT_UPNP_ATTR "CTUPNPAttr"
#endif
#if defined(TCSUPPORT_CT_PON)
#define CT_PONLINK_ATTR "CTPonLinkAttr"
#define CT_PONINTERCFG_ATTR "CTPonInterCfgAttr"
#define CT_PONINTERCFGSTATS_ATTR "CTPonInterCfgStatsAttr"
#endif
#if defined(TCSUPPORT_CT_WAN_PTM)
#define CT_VDSLLINK_ATTR "CTVdslLinkAttr"
#endif
#if defined(TCSUPPORT_CT_UPNP_DM)
#define X_CT_PROXYDEVICE  "X_CT-COM_ProxyDevice"
#endif

int8 getDeviceSummaryValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDeviceSummaryAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceSummaryAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLanDeviceNumberOfEntriesValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 getLanDeviceNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLanDeviceNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getWanDeviceNumberOfEntries(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 getWanDeviceNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWanDeviceNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getPOTSDeviceNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getPOTSDeviceNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPOTSDeviceNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CMCC)
/*
	Parent Node: InternetGatewayDevice.LANInterfaces.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct LANInterfaces[];
/*
	Parent Node: InternetGatewayDevice.Security.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct Security[];
#endif

#if defined(TCSUPPORT_CT_MIDWARE)
extern CwmpParameterStruct TeleNode[];
extern CwmpParameterStruct TopMWNode[];
#endif

extern CwmpParameterStruct InternetGatewayDevice[];
extern CwmpParameterStruct TopIGDNode[];

#if defined(TCSUPPORT_CT_PHONEAPP)
/*
	Parent Node: InternetGatewayDevice.X_CT-COM_Mgt.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct X_CTCOM_Mgt[];
#endif

/*
	Parent Node: InternetGatewayDevice.DeviceInfo.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct DeviceInfo[];
#ifdef CT_COM_DEVICEREG
/*
	Parent Node: InternetGatewayDevice.X_CT-COM_UserInfo.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct  DeviceRegistration[];
#endif

/*
	Parent Node: InternetGatewayDevice.ManagementServer.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct ManagementServer[];

/*
	Parent Node: InternetGatewayDevice.IPPingDiagnostics.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct IPPingDiagnostics[];

/*
	This Node:    InternetGatewayDevice.LANDevice.{i}
	Parent Node: InternetGatewayDevice.LANDevice.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct LanDeviceIndex[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct WANDeviceIndex[];

/*
	Parent Node: InternetGatewayDevice.Services.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct Services[];

/*
	Parent Node: InternetGatewayDevice.Time.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct Time[];

/*
	Parent Node: InternetGatewayDevice.DeviceConfig.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct DeviceConfig[];

/*
	Parent Node: InternetGatewayDevice.Layer3Forwarding.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct Layer3Forwarding[];

/*
	Parent Node: InternetGatewayDevice.LanConfigSecurity.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct LanConfigSecurity[];

/*
	Parent Node: InternetGatewayDevice.QueueManagement.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct QueueManagement[];

/*
	Parent Node: InternetGatewayDevice.UserInterface.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct UserInterface[];

#ifdef TR143
/*
	Parent Node: InternetGatewayDevice.Capabilities.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct tr143_Capabilities[];

/*
	Parent Node: InternetGatewayDevice.DownloadDiagnostics.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct DownloadDiagnostics[];

/*
	Parent Node: InternetGatewayDevice.UploadDiagnostics.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct UploadDiagnostics[];
/*
	Parent Node: InternetGatewayDevice.UDPEchoConfig.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct UDPEchoConfig[];
#endif

#if defined(TCSUPPORT_CT_QOS)
/*
	Parent Node: InternetGatewayDevice.X_CTCOM_UplinkQoS.
	Parent Node Table Location: cp_topnodetable.c 

*/
extern CwmpParameterStruct X_CTCOM_UplinkQoS[];
#endif

#if defined(TCSUPPORT_C1_CUC) || defined(TCSUPPORT_CUC_CWMP_PARAMETER)
/*
	Parent Node: InternetGatewayDevice.X_CU_Function.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct X_CU_Function[];
#endif

#if defined(TCSUPPORT_CT_UPNP_DM)
extern CwmpParameterStruct X_CTCOM_ProxyDevice[];
#endif

#ifdef TCSUPPORT_TRACEROUTE
/*
	Parent Node: InternetGatewayDevice.TraceRouteDiagnostics.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct TraceRouteDiagnostics[];
#endif

#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
extern CwmpParameterStruct PppoeEmulator[];
extern CwmpParameterStruct PppoeEmulatorBridge[];
#endif

#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
extern CwmpParameterStruct IPoeEmulator[];
#endif

#if defined(TCSUPPORT_CMCC)
#ifdef TCSUPPORT_IPV6
/*
	Parent Node: InternetGatewayDevice.IPv6Layer3Forwarding.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct IPv6Layer3Forwarding[];
#endif
#endif

#if defined(TCSUPPORT_CT_JOYME)
extern CwmpParameterStruct DnsSpeedLimit[];
#endif

#endif
