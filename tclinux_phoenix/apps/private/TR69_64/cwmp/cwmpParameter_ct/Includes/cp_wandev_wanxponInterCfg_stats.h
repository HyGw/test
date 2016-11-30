#ifndef CP_WANDEV_WANXPONINTERCFG_STATS_H_
#define CP_WANDEV_WANXPONINTERCFG_STATS_H_

#include "cp_core.h"

/*
    rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.X_CT-COM_EponInterfaceConfig.Stats.
	                                 InternetGatewayDevice.WANDevice.1.X_CT-COM_GponInterfaceConfig.Stats.
*/

int8 getWANPONInterCfg_BytesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_BytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_BytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_BytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_BytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_BytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_SUnicastPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_SUnicastPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_SUnicastPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_RUnicastPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_RUnicastPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_RUnicastPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_SMulticastPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_SMulticastPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_SMulticastPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_RMulticastPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_RMulticastPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_RMulticastPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_SBroadcastPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_SBroadcastPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_SBroadcastPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_RBroadcastPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_RBroadcastPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_RBroadcastPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_FECErrorValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_FECErrorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_FECErrorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_HECErrorValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_HECErrorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_HECErrorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_DropPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_DropPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_DropPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_SPausePacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_SPausePacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_SPausePacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_RPausePacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_RPausePacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_RPausePacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getWANPONInterCfg_ReceiveBlocksValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_ReceiveBlocksAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_ReceiveBlocksAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_TransmitBlocksValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_TransmitBlocksAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_TransmitBlocksAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_FCSErrorValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_FCSErrorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_FCSErrorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#endif

