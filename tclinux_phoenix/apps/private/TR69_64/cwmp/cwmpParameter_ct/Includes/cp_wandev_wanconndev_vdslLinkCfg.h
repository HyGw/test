#ifndef CP_WANDEV_WANCONNDEV_VDSLLINKCFG_H_
#define CP_WANDEV_WANCONNDEV_VDSLLINKCFG_H_
#include "cp_core.h"


/*
    rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.X_CT-COM_WANVdslLinkConfig.
*/
int8 getWANVdslLinkCfg1EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANVdslLinkCfg1EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANVdslLinkCfg1EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANVdslLinkCfg1EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANVdslLinkCfg1ModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANVdslLinkCfg1ModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANVdslLinkCfg1ModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANVdslLinkCfg1ModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANVdslLinkCfg1VLANIDMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANVdslLinkCfg1VLANIDMarkValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANVdslLinkCfg1VLANIDMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANVdslLinkCfg1VLANIDMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANVdslLinkCfg18021PMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANVdslLinkCfg18021PMarkValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANVdslLinkCfg18021PMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANVdslLinkCfg18021PMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif
