#ifndef CP_TIME_H_
#define CP_TIME_H_

#include "cp_core.h"

extern CwmpParameterStruct Time[];

#define  NTP_TIME        	      "0"             /* NTP Server Automatically*/
#define  COMPUTER_TIME        "1"             /* PC's Clock*/
#define  MANUAL_TIME            "2"            /* Manually*/


int8 getTimeNTPServer1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTimeNTPServer1Value(void* value, struct DeviceNode*  deviceNode);
int8 getTimeNTPServer1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTimeNTPServer1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTimeCurrentLocalTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getTimeCurrentLocalTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTimeCurrentLocalTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTimeLocalTimeZoneValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTimeLocalTimeZoneValue(void* value, struct DeviceNode*  deviceNode);
int8 getTimeLocalTimeZoneAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTimeLocalTimeZoneAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getTimeNTPServer2Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTimeNTPServer2Value(void* value, struct DeviceNode*  deviceNode);
int8 getTimeNTPServer2Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTimeNTPServer2Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTimeNTPServer3Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTimeNTPServer3Value(void* value, struct DeviceNode*  deviceNode);
int8 getTimeNTPServer3Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTimeNTPServer3Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTimeNTPServer4Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTimeNTPServer4Value(void* value, struct DeviceNode*  deviceNode);
int8 getTimeNTPServer4Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTimeNTPServer4Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTimeNTPServer5Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTimeNTPServer5Value(void* value, struct DeviceNode*  deviceNode);
int8 getTimeNTPServer5Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTimeNTPServer5Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getNTPIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setNTPIntervalValue(void* value, struct DeviceNode*  deviceNode);
int8 getNTPIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setNTPIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getNTPServerTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setNTPServerTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getNTPServerTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setNTPServerTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER) || defined(TCSUPPORT_CT_PON_GD)
int8 getNTPEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setNTPEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getNTPEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setNTPEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTimeLocalTimeZoneNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTimeLocalTimeZoneNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getTimeLocalTimeZoneNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTimeLocalTimeZoneNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getNTPStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getNTPStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setNTPStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

#endif
