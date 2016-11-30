#ifndef CT_IPPINGDIAGNOSTICS_H_
#define CT_IPPINGDIAGNOSTICS_H_

#include "cp_core.h"
#if !defined(TCSUPPORT_C1_CUC)
#define CTCOM_PING_PID_PATH "/tmp/cwmp/ct_ping%d.pid"
#define MAX_IPPINGDIAGNOSTIC_NUM 3
#define IPPINGDIAGNOSTIC_ENTRY "CtDiagnostic_Entry%d"
#define IPPINGDIAGNOSTIC "CtDiagnostic"
#define IPPINGDIAGNOSTIC_COMMON "CtDiagnostic_Common"
#define CTCOM_PING_ROUTE_PATH "/tmp/cwmp/ct_ping_route"
/*
       This Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_Ping.PingConfig.{i}
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_Ping.PingConfig.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
extern CwmpParameterStruct PingConfigIndex[];
int getpingNumberEntries();
int8 checkFreePingEntryRule(void);
int	cwmpInitIpPingDiagnosticTable( DeviceNodePtr deviceNode );
int8 DeviceNodeAddPingInedxObject(uint32* num, DeviceNode*  deviceNode);
int8 deleteIpPingDiagnosticEntryByIndex(uint32 num);
int8 DeviceNodeDeletePingInedxObject(uint32 num, DeviceNode*  deviceNode);

int8 getCTCOMDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTCOMDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTCOMDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTCOMDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTCOMIPPingInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTCOMIPPingInterfaceValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTCOMIPPingInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTCOMIPPingInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTCOMIPPingHostValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTCOMIPPingHostValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTCOMIPPingHostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTCOMIPPingHostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTCOMNumberOfRepetitionsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTCOMNumberOfRepetitionsValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTCOMNumberOfRepetitionsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTCOMNumberOfRepetitionsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTCOMIPPingTimeoutValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTCOMIPPingTimeoutValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTCOMIPPingTimeoutAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTCOMIPPingTimeoutAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTCOMDataBlockSizeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTCOMDataBlockSizeValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTCOMDataBlockSizeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTCOMDataBlockSizeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTCOMDSCPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTCOMDSCPValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTCOMDSCPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTCOMDSCPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTCOMIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTCOMIntervalValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTCOMIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTCOMIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTCOMStopValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTCOMStopValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTCOMStopAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTCOMStopAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

void stopCwmpPing(int entryindex);
#endif
#endif
