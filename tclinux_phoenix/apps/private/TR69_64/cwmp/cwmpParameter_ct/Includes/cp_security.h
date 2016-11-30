#ifndef _CP_SECURITY_H_
#define _CP_SECURITY_H_

#include "cp_core.h"

#define MAX_IPMACFILTER_RULE 	120
#define MAX_MAC_FILTER_RULE		40
#define MAX_IPUP_FILTER_RULE	40
#define MAX_IPDOWN_FILTER_RULE	40
#define MAX_URL_FILTER_RULE		100

extern CwmpParameterStruct Security_IPFilter[];
extern CwmpParameterStruct Security_IPFilterIndex[];
extern CwmpParameterStruct Security_ParentalCtrl[];
extern CwmpParameterStruct Security_Templates[];
extern CwmpParameterStruct Security_UrlFilter[];
extern CwmpParameterStruct Security_UrlAddress[];	
extern CwmpParameterStruct Security_UrlFilterIndex[];

int8 getSecurity_IPFilterInEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_IPFilterInEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_IPFilterInEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_IPFilterInEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_IPFilterInPolicyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_IPFilterInPolicyValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_IPFilterInPolicyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_IPFilterInPolicyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_IPFilterOutEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_IPFilterOutEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_IPFilterOutEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_IPFilterOutEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_IPFilterOutPolicyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_IPFilterOutPolicyValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_IPFilterOutPolicyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_IPFilterOutPolicyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 addSecurity_IPFilterObject(uint32* num,  DeviceNode*  deviceNode);
int8 deleteSecurity_IPFilterObject(uint32 num, DeviceNode*  deviceNode);
int	cwmpInitSecurity_IPFilterTableFromFlash( DeviceNodePtr deviceNode );

int8 getSecurity_IPFilter_NameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_IPFilter_NameValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_IPFilter_NameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_IPFilter_NameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_IPFilter_EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_IPFilter_EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_IPFilter_EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_IPFilter_EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_IPFilter_SourceIPStartValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_IPFilter_SourceIPStartValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_IPFilter_SourceIPStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_IPFilter_SourceIPStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_IPFilter_SourceIPEndValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_IPFilter_SourceIPEndValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_IPFilter_SourceIPEndAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_IPFilter_SourceIPEndAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_IPFilter_DestIPStartValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_IPFilter_DestIPStartValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_IPFilter_DestIPStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_IPFilter_DestIPStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_IPFilter_DestIPEndValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_IPFilter_DestIPEndValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_IPFilter_DestIPEndAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_IPFilter_DestIPEndAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_IPFilter_DestIPEndValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_IPFilter_DestIPEndValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_IPFilter_DestIPEndAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_IPFilter_DestIPEndAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_IPFilter_ProtocolValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_IPFilter_ProtocolValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_IPFilter_ProtocolAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_IPFilter_ProtocolAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_IPFilter_SourcePortStartValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_IPFilter_SourcePortStartValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_IPFilter_SourcePortStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_IPFilter_SourcePortStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_IPFilter_SourcePortStartValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_IPFilter_SourcePortStartValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_IPFilter_SourcePortStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_IPFilter_SourcePortStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_IPFilter_SourcePortEndValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_IPFilter_SourcePortEndValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_IPFilter_SourcePortEndAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_IPFilter_SourcePortEndAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_IPFilter_DestPortStartValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_IPFilter_DestPortStartValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_IPFilter_DestPortStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_IPFilter_DestPortStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_IPFilter_DestPortEndValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_IPFilter_DestPortEndValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_IPFilter_DestPortEndAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_IPFilter_DestPortEndAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_UrlFilterEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_UrlFilterEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_UrlFilterEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_UrlFilterEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_UrlFilterPolicyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_UrlFilterPolicyValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_UrlFilterPolicyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_UrlFilterPolicyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSecurity_UrlFilterValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSecurity_UrlFilterValue(void* value, struct DeviceNode*  deviceNode);
int8 getSecurity_UrlFilterAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSecurity_UrlFilterAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 addSecurity_UrlFilterObject(uint32* num,  DeviceNode*  deviceNode);
int8 deleteSecurity_UrlFilterObject(uint32 num, DeviceNode*  deviceNode);
int cwmpInitSecurity_UrlFilterTableFromFlash( DeviceNodePtr deviceNode );

#endif
