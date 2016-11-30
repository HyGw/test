#ifndef CP_SERVICES_USER_H_
#define CP_SERVICES_USER_H_

#include "cp_core.h"

extern CwmpParameterStruct X_CU_UserIndex[];
extern CwmpParameterStruct usrIndex[];
extern CwmpParameterStruct MacFilterIndex[];
extern CwmpParameterStruct IpFilterIndex[];
extern CwmpParameterStruct WhiteListIndex[];
extern CwmpParameterStruct BlackListIndex[];


int8 DeviceNodeDelXCUUserObject(uint32 num, DeviceNode*  deviceNode);
int8 XCUUserNodeAddObject(uint32 *num, DeviceNode*  deviceNode);
int cwmpInitXCUUserTable(DeviceNodePtr deviceNode);

int8	getX_CU_UserEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_UserEnableValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_UserEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_UserEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_UserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_UserNameValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_UserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_UserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_UserDespValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_UserDespValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_UserDespAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_UserDespAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_UserNum0fSubUserValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_UserNum0fSubUserValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_UserNum0fSubUserAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_UserNum0fSubUserAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_UserNum0fMACFilterValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	getX_CU_UserNum0fMACFilterAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_UserNum0fMACFilterAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_UserNum0fIPFilterValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	getX_CU_UserNum0fIPFilterAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_UserNum0fIPFilterAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_UserNum0fWhiteListValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	getX_CU_UserNum0fWhiteListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_UserNum0fWhiteListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_UserNum0fBlackListValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	getX_CU_UserNum0fBlackListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_UserNum0fBlackListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_UserURLListFilterModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_UserURLListFilterModeValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_UserURLListFilterModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_UserURLListFilterModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_TimeEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_TimeEnableValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_TimeEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_TimeEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_TimeRulesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_TimeRulesValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_TimeRulesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_TimeRulesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 DeviceNodeDelMacFilterObject(uint32 num, DeviceNode*  deviceNode);
int8 MacFilterNodeAddObject(uint32 *num, DeviceNode*  deviceNode);
int cwmpInitMacFilterTable(DeviceNodePtr deviceNode);

int8	getX_CU_FilterMacValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_FilterMacValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_FilterMacAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_FilterMacAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 DeviceNodeDelIpFilterObject(uint32 num, DeviceNode*  deviceNode);
int8 IpFilterNodeAddObject(uint32 *num, DeviceNode*  deviceNode);
int cwmpInitIpFilterTable(DeviceNodePtr deviceNode);

int8	getX_CU_FilterIPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_FilterIPValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_FilterIPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_FilterIPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 DeviceNodeDelWhiteListObject(uint32 num, DeviceNode*  deviceNode);
int8 WhiteListNodeAddObject(uint32 *num, DeviceNode*  deviceNode);
int cwmpInitWhiteListTable(DeviceNodePtr deviceNode);

int8	getX_CU_WhiteURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_WhiteURLValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_WhiteURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_WhiteURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 DeviceNodeDelBlackListObject(uint32 num, DeviceNode*  deviceNode);
int8 BlackListNodeAddObject(uint32 *num, DeviceNode*  deviceNode);
int cwmpInitBlackListTable(DeviceNodePtr deviceNode);

int8	getX_CU_BlackURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_BlackURLValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_BlackURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_BlackURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_UsrPortalEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_UsrPortalEnableValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_UsrPortalEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_UsrPortalEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_UsrPortalComputerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_UsrPortalComputerValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_UsrPortalComputerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_UsrPortalComputerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_UsrPortalIPTVValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_UsrPortalIPTVValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_UsrPortalIPTVAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_UsrPortalIPTVAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getX_CU_UsrPortalPhoneValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setX_CU_UsrPortalPhoneValue(void* value, struct DeviceNode*  deviceNode);
int8	getX_CU_UsrPortalPhoneAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setX_CU_UsrPortalPhoneAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
