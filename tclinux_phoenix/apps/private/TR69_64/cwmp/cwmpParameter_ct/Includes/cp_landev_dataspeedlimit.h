#ifndef CP_LANDEV_DATASPEEDLIMIT_H
#define CP_LANDEV_DATASPEEDLIMIT_H

#include "cp_core.h"

int8 getSpeedLimitModeUPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSpeedLimitModeUPValue(void* value, struct DeviceNode*  deviceNode);
int8 getSpeedLimitModeUPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSpeedLimitModeUPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSpeedLimitModeDOWNValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSpeedLimitModeDOWNValue(void* value, struct DeviceNode*  deviceNode);
int8 getSpeedLimitModeDOWNAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSpeedLimitModeDOWNAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInterfaceLimitUPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setInterfaceLimitUPValue(void* value, struct DeviceNode*  deviceNode);
int8 getInterfaceLimitUPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInterfaceLimitUPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInterfaceLimitDOWNValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setInterfaceLimitDOWNValue(void* value, struct DeviceNode*  deviceNode);
int8 getInterfaceLimitDOWNAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInterfaceLimitDOWNAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getVlanTagLimitUPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setVlanTagLimitUPValue(void* value, struct DeviceNode*  deviceNode);
int8 getVlanTagLimitUPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVlanTagLimitUPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getVlanTagLimitDOWNValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setVlanTagLimitDOWNValue(void* value, struct DeviceNode*  deviceNode);
int8 getVlanTagLimitDOWNAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVlanTagLimitDOWNAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPLimitUPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPLimitUPValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPLimitUPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPLimitUPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPLimitDOWNValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPLimitDOWNValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPLimitDOWNAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPLimitDOWNAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

