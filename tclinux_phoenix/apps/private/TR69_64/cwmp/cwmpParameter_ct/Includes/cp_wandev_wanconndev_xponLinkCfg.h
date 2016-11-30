#ifndef CP_WANDEV_WANCONNDEV_XPONLINKCFG_H_
#define CP_WANDEV_WANCONNDEV_XPONLINKCFG_H_
#include "cp_core.h"


/*
    rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.X_CT-COM_WANEponLinkConfig.
	                                 InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.X_CT-COM_WANGponLinkConfig.
*/
int8 getWANPonLinkCfg1EPonEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPonLinkCfg1EPonEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPonLinkCfg1EPonEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPonLinkCfg1EPonEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPonLinkCfg1GPonEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPonLinkCfg1GPonEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPonLinkCfg1GPonEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPonLinkCfg1GPonEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPonLinkCfg1ModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPonLinkCfg1ModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPonLinkCfg1ModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPonLinkCfg1ModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPonLinkCfg1VLANIDMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPonLinkCfg1VLANIDMarkValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPonLinkCfg1VLANIDMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPonLinkCfg1VLANIDMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPonLinkCfg18021PMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPonLinkCfg18021PMarkValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPonLinkCfg18021PMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPonLinkCfg18021PMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif
