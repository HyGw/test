#ifndef CP_LANDEV_DHCPV6_H_
#define CP_LANDEV_DHCPV6_H_

#include "cp_core.h"

int8 getDHCPv6ServerEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDHCPv6ServerEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getDHCPv6ServerEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPv6ServerEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDHCPv6ServerPoolEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDHCPv6ServerPoolEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getDHCPv6ServerPoolEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPv6ServerPoolEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDHCPv6ServerPoolMinAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDHCPv6ServerPoolMinAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getDHCPv6ServerPoolMinAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPv6ServerPoolMinAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDHCPv6ServerPoolMaxAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDHCPv6ServerPoolMaxAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getDHCPv6ServerPoolMaxAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPv6ServerPoolMaxAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDHCPv6ServerPreferredLifeTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDHCPv6ServerPreferredLifeTimeValue(void* value, struct DeviceNode*  deviceNode);
int8 getDHCPv6ServerPreferredLifeTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPv6ServerPreferredLifeTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDHCPv6ServerValidLifeTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDHCPv6ServerValidLifeTimeValue(void* value, struct DeviceNode*  deviceNode);
int8 getDHCPv6ServerValidLifeTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPv6ServerValidLifeTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

