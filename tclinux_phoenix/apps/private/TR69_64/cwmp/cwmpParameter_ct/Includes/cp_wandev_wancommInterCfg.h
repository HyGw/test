#ifndef CP_WANDEV_WANCOMMINTERCFG_H_
#define CP_WANDEV_WANCOMMINTERCFG_H_

#include "cp_core.h"


/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig.
	Parent Node Table Location: cp_wandslInterCfg_nodetable.c
*/

int8 getEnabledForInternetValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEnabledForInternetValue(void* value, struct DeviceNode*  deviceNode);
int8 getEnabledForInternetAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEnabledForInternetAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANAccessTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANAccessTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANAccessTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLayer1UpstreamMaxBitRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLayer1UpstreamMaxBitRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLayer1UpstreamMaxBitRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLayer1DownstreamMaxBitRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLayer1DownstreamMaxBitRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLayer1DownstreamMaxBitRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPhysicalLinkStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getPhysicalLinkStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPhysicalLinkStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTotalBytesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getTotalBytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTotalBytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTotalBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getTotalBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTotalBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTotalPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getTotalPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTotalPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTotalPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getTotalPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTotalPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT_PON_SC)
int8 getEnabledBRDHCPCheckValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEnabledBRDHCPCheckValue(void* value, struct DeviceNode*  deviceNode);
int8 getEnabledBRDHCPCheckAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEnabledBRDHCPCheckAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#endif
