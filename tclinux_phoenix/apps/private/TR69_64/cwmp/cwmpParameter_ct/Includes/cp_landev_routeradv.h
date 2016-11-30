#ifndef CP_LANDEV_ROUTERADV_H_
#define CP_LANDEV_ROUTERADV_H_

#include "cp_core.h"

int8 getRouterAdvEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRouterAdvEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getRouterAdvEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRouterAdvEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRouterAdvMaxRtrAdvIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRouterAdvMaxRtrAdvIntervalValue(void* value, struct DeviceNode*  deviceNode);
int8 getRouterAdvMaxRtrAdvIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRouterAdvMaxRtrAdvIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRouterAdvMinRtrAdvIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRouterAdvMinRtrAdvIntervalValue(void* value, struct DeviceNode*  deviceNode);
int8 getRouterAdvMinRtrAdvIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRouterAdvMinRtrAdvIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRouterAdvAdvManagedFlagValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRouterAdvAdvManagedFlagValue(void* value, struct DeviceNode*  deviceNode);
int8 getRouterAdvAdvManagedFlagAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRouterAdvAdvManagedFlagAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRouterAdvAdvOtherConfigFlagValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRouterAdvAdvOtherConfigFlagValue(void* value, struct DeviceNode*  deviceNode);
int8 getRouterAdvAdvOtherConfigFlagAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRouterAdvAdvOtherConfigFlagAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

