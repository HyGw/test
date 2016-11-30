#ifndef CP_LANDEV_LOOPBACKDETECT_H_
#define CP_LANDEV_LOOPBACKDETECT_H_

#include "cp_core.h"

int8 getLoopbackEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLoopbackEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getLoopbackEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLoopbackEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLoopExistPeriodValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLoopExistPeriodValue(void* value, struct DeviceNode*  deviceNode);
int8 getLoopExistPeriodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLoopExistPeriodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLoopCancelPeriodValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLoopCancelPeriodValue(void* value, struct DeviceNode*  deviceNode);
int8 getLoopCancelPeriodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLoopCancelPeriodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getVlanTagValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setVlanTagValue(void* value, struct DeviceNode*  deviceNode);
int8 getVlanTagAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVlanTagAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEthernetTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEthernetTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getEthernetTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthernetTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

