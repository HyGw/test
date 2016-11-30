#ifndef _CP_X_CT_MGT_H_
#define _CP_X_CT_MGT_H_

#include "cp_core.h"

#define MOBILE_ENTRY "mobile_Entry"
#define DISTRIADDR		"MgtURL"
#define DISTRIPORT		"Port"
#define OPERATADDR 		"OperateSAddr"

int8 getMgtURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMgtURLValue(void* value, struct DeviceNode*  deviceNode);
int8 getMgtURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMgtURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMgtPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMgtPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getMgtPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMgtPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMgtHeartbeatValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMgtHeartbeatValue(void* value, struct DeviceNode*  deviceNode);
int8 getMgtHeartbeatAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMgtHeartbeatAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMgtAbilityValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMgtAbilityValue(void* value, struct DeviceNode*  deviceNode);
int8 getMgtAbilityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMgtAbilityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMgtLocatePortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMgtLocatePortValue(void* value, struct DeviceNode*  deviceNode);
int8 getMgtLocatePortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMgtLocatePortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMgtVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMgtVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMgtVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT_JOYME)
int8 getMgtSSNValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMgtSSNAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMgtSSNAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#endif
