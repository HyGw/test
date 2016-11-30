#ifndef _CP_DEVICEREGISTRATION_H
#define _CP_DEVICEREGISTRATION_H

#include "cp_core.h"

extern CwmpParameterStruct DeviceRegistration[] ;

#define DEVACCOUNT_STATUS    "registerStatus"
#define DEVACCOUNT_RESULT  "registerResult"
#define DEVACCOUNT_USERNAME    "userName"
#define DEVACCOUNT_USERPASSWD    "userPasswordDEV"
#if !defined(TCSUPPORT_CUC)
#define LOGICID_USERNAME		"Username"
#define LOGICID_PSW				"Password"
#define LOGICID_REG_STATUS    	"registerStatus"
#define LOGICID_REG_RESULT  	"registerResult"
#endif

/*******************  InternetGatewayDevice.Capabilities. start ************************/

int8 getDeviceRegistrationDeviceStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDeviceRegistrationDeviceStatusValue(void* value, struct DeviceNode*  deviceNode);
int8 getDeviceRegistrationDeviceStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceRegistrationDeviceStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getDeviceRegistrationDeviceResultValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDeviceRegistrationDeviceResultValue(void* value, struct DeviceNode*  deviceNode);
int8 getDeviceRegistrationDeviceResultAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceRegistrationDeviceResultAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


 
int8 getDeviceRegistrationRetryTimesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDeviceRegistrationRetryTimesValue(void* value, struct DeviceNode*  deviceNode);
int8 getDeviceRegistrationRetryTimesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceRegistrationRetryTimesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getDeviceRegistrationRetryLimitValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDeviceRegistrationRetryLimitValue(void* value, struct DeviceNode*  deviceNode);
int8 getDeviceRegistrationRetryLimitAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceRegistrationRetryLimitAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDeviceRegistrationDeviceUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDeviceRegistrationDeviceUserNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getDeviceRegistrationDeviceUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceRegistrationDeviceUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

	
int8 getDeviceRegistrationDeviceUserIdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDeviceRegistrationDeviceUserIdValue(void* value, struct DeviceNode*  deviceNode);
int8 getDeviceRegistrationDeviceUserIdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceRegistrationDeviceUserIdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getServiceNumValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setServiceNumValue(void* value, struct DeviceNode*  deviceNode);
int8 getServiceNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setServiceNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getServiceNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setServiceNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getServiceNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setServiceNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_DEVREG_PROV_CODE)
int8 getRegProvisioningCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRegProvisioningCodeValue(void* value, struct DeviceNode*  deviceNode);
int8 getRegProvisioningCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRegProvisioningCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CT_ADSL_HN)
int8 getRouteStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRouteStatusValue(void* value, struct DeviceNode*  deviceNode);
int8 getRouteStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRouteStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#endif

