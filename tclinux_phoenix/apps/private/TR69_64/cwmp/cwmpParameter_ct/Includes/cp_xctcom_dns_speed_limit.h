#ifndef _CP_XCTCOM_DNS_SPEED_LIMIT_H_
#define _CP_XCTCOM_DNS_SPEED_LIMIT_H_

#include "cp_core.h"

#define DNSSPEED_LIMIT_STR "DnsSpeedLimit"

/*******************  InternetGatewayDevice.X_CT-COM_DNSSpeedLimit. start ************************/
int8 getDnsSpeedLimitDomainValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDnsSpeedLimitDomainValue(void* value, struct DeviceNode*  deviceNode);
int8 getDnsSpeedLimitDomainAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDnsSpeedLimitDomainAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDnsSpeedLimitActionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDnsSpeedLimitActionValue(void* value, struct DeviceNode*  deviceNode);
int8 getDnsSpeedLimitActionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDnsSpeedLimitActionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDnsSpeedLimitHgwInfoValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDnsSpeedLimitHgwInfoAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDnsSpeedLimitHgwInfoAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDnsSpeedLimitDeviceInfoValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDnsSpeedLimitDeviceInfoAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDnsSpeedLimitDeviceInfoAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

