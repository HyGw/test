#ifndef CP_X_CU_FUNCTION_H_
#define CP_X_CU_FUNCTION_H_

#include "cp_core.h"

#define ADMIN_NODE "Account_Entry0"
#define ADMINNAME "username"
#define ADMINPSW "web_passwd"
#define USER_NODE "Account_Entry1"
#define USERNAME "username"
#define USERPSW "web_passwd"

extern CwmpParameterStruct X_CU_Function[];
extern CwmpParameterStruct X_CU_Web[];

extern CwmpParameterStruct AlarmInfoIndex[];

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
extern CwmpParameterStruct ALGAbility[];
extern CwmpParameterStruct X_CTCOM_UplinkQoS[];
#endif

int	cwmpInitAlarmInfoSubTree( DeviceNodePtr deviceNode );
	
int8 getAdminNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAdminNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAdminNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAdminPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAdminPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getAdminPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAdminPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAdminWebIPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAdminWebIPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAdminWebIPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUserPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUserPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUserPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUserWebIPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUserWebIPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUserWebIPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLogEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLogEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getLogEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLogEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLogLevelValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLogLevelValue(void* value, struct DeviceNode*  deviceNode);
int8 getLogLevelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLogLevelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLogUploadEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLogUploadEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getLogUploadEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLogUploadEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLogUploadIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLogUploadIntervalValue(void* value, struct DeviceNode*  deviceNode);
int8 getLogUploadIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLogUploadIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAlarmEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAlarmEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getAlarmEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAlarmEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAlarmLevelValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAlarmLevelValue(void* value, struct DeviceNode*  deviceNode);
int8 getAlarmLevelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAlarmLevelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUploadServerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUploadServerValue(void* value, struct DeviceNode*  deviceNode);
int8 getUploadServerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUploadServerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCuLogUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCuLogUsernameValue(void* value, struct DeviceNode*  deviceNode);
int8 getCuLogUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCuLogUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCuLogPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCuLogPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getCuLogPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCuLogPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getNumberOfAlarmInfoValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setNumberOfAlarmInfoValue(void* value, struct DeviceNode*  deviceNode);
int8 getNumberOfAlarmInfoAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setNumberOfAlarmInfoAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAlarmIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAlarmIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAlarmIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAlarmCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAlarmCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAlarmCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAlarmStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAlarmStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAlarmStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPerceivedSeverityValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getPerceivedSeverityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPerceivedSeverityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAlarmRaisedTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAlarmRaisedTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAlarmRaisedTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAlarmClearedTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAlarmClearedTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAlarmClearedTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAlarmDetailValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAlarmDetailAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAlarmDetailAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getAdminaccountChangeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAdminaccountChangeValue(void* value, struct DeviceNode*  deviceNode);
int8 getAdminaccountChangeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAdminaccountChangeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPPPoEDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPPPoEDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode);
int8 getPPPoEDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPPPoEDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPPPoEDiagUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPPPoEDiagUsernameValue(void* value, struct DeviceNode*  deviceNode);
int8 getPPPoEDiagUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPPPoEDiagUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPPPoEDiagPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPPPoEDiagPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getPPPoEDiagPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPPPoEDiagPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPPPoEDiagVLANIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPPPoEDiagVLANIDValue(void* value, struct DeviceNode*  deviceNode);
int8 getPPPoEDiagVLANIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPPPoEDiagVLANIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPPPoEDiagPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPPPoEDiagPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getPPPoEDiagPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPPPoEDiagPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPPPoEDiagAuthProtocolValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPPPoEDiagAuthProtocolValue(void* value, struct DeviceNode*  deviceNode);
int8 getPPPoEDiagAuthProtocolAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPPPoEDiagAuthProtocolAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPPPoEDiagRetryTimesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPPPoEDiagRetryTimesValue(void* value, struct DeviceNode*  deviceNode);
int8 getPPPoEDiagRetryTimesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPPPoEDiagRetryTimesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPPPoEDiagResultValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getPPPoEDiagResultAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPPPoEDiagResultAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPPPoEDiagSessionIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPPPoEDiagSessionIDValue(void* value, struct DeviceNode*  deviceNode);
int8 getPPPoEDiagSessionIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPPPoEDiagSessionIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPPPoEDiagExtIPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPPPoEDiagExtIPValue(void* value, struct DeviceNode*  deviceNode);
int8 getPPPoEDiagExtIPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPPPoEDiagExtIPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPPPoEDiagDefGWValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPPPoEDiagDefGWValue(void* value, struct DeviceNode*  deviceNode);
int8 getPPPoEDiagDefGWAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPPPoEDiagDefGWAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPPPoEDiagWanInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPPPoEDiagWanInterfaceValue(void* value, struct DeviceNode*  deviceNode);
int8 getPPPoEDiagWanInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPPPoEDiagWanInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPPPoEDiagLanInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPPPoEDiagLanInterfaceValue(void* value, struct DeviceNode*  deviceNode);
int8 getPPPoEDiagLanInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPPPoEDiagLanInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);



extern int8 getSnoopingEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 setSnoopingEnableValue(void* value, struct DeviceNode* deviceNode);
extern int8 getSnoopingEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setSnoopingEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFWPacketofAPPFilterEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setFWPacketofAPPFilterEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getFWPacketofAPPFilterEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFWPacketofAPPFilterEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFWPortScanEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setFWPortScanEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getFWPortScanEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFWPortScanEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFWInvalidPktDenyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setFWInvalidPktDenyValue(void* value, struct DeviceNode*  deviceNode);
int8 getFWInvalidPktDenyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFWInvalidPktDenyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFWDoSEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setFWDoSEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getFWDoSEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFWDoSEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getTelnetEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 setTelnetEnableValue(void* value, struct DeviceNode*  deviceNode);
extern int8 getTelnetEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
extern int8 setTelnetEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getTelnetUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 setTelnetUserNameValue(void* value, struct DeviceNode*  deviceNode);
extern int8 getTelnetUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
extern int8 setTelnetUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getTelnetPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 setTelnetPasswordValue(void* value, struct DeviceNode*  deviceNode);
extern int8 getTelnetPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
extern int8 setTelnetPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getTelnetPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 setTelnetPortValue(void* value, struct DeviceNode*  deviceNode);
extern int8 getTelnetPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
extern int8 setTelnetPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDdnsEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDdnsEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getDdnsEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDdnsEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDdnsProviderValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDdnsProviderValue(void* value, struct DeviceNode*  deviceNode);
int8 getDdnsProviderAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDdnsProviderAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDdnsUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDdnsUsernameValue(void* value, struct DeviceNode*  deviceNode);
int8 getDdnsUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDdnsUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDdnsPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDdnsPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getDdnsPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDdnsPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDdnsServicePortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDdnsServicePortValue(void* value, struct DeviceNode*  deviceNode);
int8 getDdnsServicePortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDdnsServicePortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDdnsDomainNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDdnsDomainNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getDdnsDomainNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDdnsDomainNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDdnsHostNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDdnsHostNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getDdnsHostNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDdnsHostNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDdnsWANPathValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDdnsWANPathValue(void* value, struct DeviceNode*  deviceNode);
int8 getDdnsWANPathAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDdnsWANPathAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getdmzEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setdmzEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getdmzEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setdmzEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getdmzHostIPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setdmzHostIPValue(void* value, struct DeviceNode*  deviceNode);
int8 getdmzHostIPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setdmzHostIPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFactoryResetModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setFactoryResetModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getFactoryResetModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFactoryResetModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#endif

