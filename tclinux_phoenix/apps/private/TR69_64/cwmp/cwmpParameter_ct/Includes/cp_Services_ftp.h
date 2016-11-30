#ifndef CP_SERVICES_FTP_H_
#define CP_SERVICES_FTP_H_

#include "cp_core.h"

extern 	CwmpParameterStruct X_CU_FTPCltIndex[];

int8	getX_CU_FTPSvrUserRightValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_FTPSvrUserRightValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_FTPSvrUserRightAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_FTPSvrUserRightAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_FTPSvrPathValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_FTPSvrPathValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_FTPSvrPathAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_FTPSvrPathAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 DeviceNodeDelFTPClientObject(uint32 num, DeviceNode*  deviceNode);
int8 FTPClientNodeAddObject(uint32 *num, DeviceNode*  deviceNode);
int cwmpInitFTPClientTable(DeviceNodePtr deviceNode);

int8	getX_CU_FTPCltEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_FTPCltEnableValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_FTPCltEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_FTPCltEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_FTPCltSvrPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_FTPCltSvrPortValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_FTPCltSvrPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_FTPCltSvrPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_FTPCltFtpServerURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_FTPCltFtpServerURLValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_FTPCltFtpServerURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_FTPCltFtpServerURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_FTPCltSvrUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_FTPCltSvrUserNameValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_FTPCltSvrUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_FTPCltSvrUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_FTPCltSvrPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_FTPCltSvrPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_FTPCltSvrPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_FTPCltSvrPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_FTPCltStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	getX_CU_FTPCltStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_FTPCltStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
