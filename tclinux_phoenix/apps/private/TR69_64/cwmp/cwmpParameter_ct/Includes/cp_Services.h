#ifndef CP_SERVICES_H_
#define CP_SERVICES_H_

#include "cp_core.h"

#if VOIP
#define VOICESERVICESNUM		1
#define VOICEPROFILENUM		1
#endif

#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_ACCESSLIMIT)
#define ACCESS_LIMIT_COMMOM_ENTRY "Accesslimit_Common"
#endif
#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
extern NodeFunc X_CU_UserFun[];
extern CwmpParameterStruct X_CU_FTPService[];
extern NodeFunc X_CU_FTPCltFun[];
#endif

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceService
	Parent Node Table Location: cp_Services_nodetable.c
*/
extern CwmpParameterStruct Capabilities[];

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}
	Parent Node Table Location: cp_Services_nodetable.c
*/
extern CwmpParameterStruct VoiceProfile[];

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.
	Parent Node Table Location: cp_Services_nodetable.c
*/
extern CwmpParameterStruct VoiceServiceIndex[];

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.
	Parent Node Table Location: cp_Services_nodetable.c
*/
extern CwmpParameterStruct VoiceProfileIndex[];

int8 getVoiceProfileNmberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getVoiceProfileNmberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVoiceProfileNmberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.Phyinterface.{i}
	Parent Node Table Location: cp_Services_nodetable.c
*/

extern CwmpParameterStruct PhyInterface[];


/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.Phyinterface.
	Parent Node Table Location: cp_Services_nodetable.c
*/

extern CwmpParameterStruct PhyInterfaceIndex[];

int8 cwmpPhyInterfaceAddObject(uint32 *num, DeviceNode*  deviceNode);
int8 cwmpPhyInterfaceDelObject(uint32 num, DeviceNode*  deviceNode);
int cwmpInitPhyInterfaceSubTree( DeviceNodePtr deviceNode );

int8 cwmpVoiceProfileAddObject(uint32 *num, DeviceNode*  deviceNode);
int8 cwmpVoiceProfileDelObject(uint32 num, DeviceNode*  deviceNode);
int	cwmpInitVoiceServiceSubTree( DeviceNodePtr deviceNode );
int	cwmpInitVoiceProfileSubTree( DeviceNodePtr deviceNode );

#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_ACCESSLIMIT)
int8	getModeServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setModeServicesValue(void* value, struct DeviceNode*  deviceNode);
int8	getModeServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setModeServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getTotalTerminalNumberServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setTotalTerminalNumberServicesValue(void* value, struct DeviceNode*  deviceNode);
int8	getTotalTerminalNumberServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setTotalTerminalNumberServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CMCC)
int8	getTerminalTypeServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setTerminalTypeServicesValue(void* value, struct DeviceNode*  deviceNode);
int8	getTerminalTypeServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setTerminalTypeServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#endif
#endif

#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_IPTV_NODE)
int8 getIGMPEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIGMPEnableValue(void* value, struct DeviceNode* deviceNode);
int8 getIGMPEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIGMPEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getProxyEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setProxyEnableValue(void* value, struct DeviceNode* deviceNode);
int8 getProxyEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setProxyEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSnoopingEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSnoopingEnableValue(void* value, struct DeviceNode* deviceNode);
int8 getSnoopingEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSnoopingEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSTBNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSTBNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSTBNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_STBMAC_REPORT)
int8 getSTBMACValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSTBMACAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSTBMACAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#endif
#endif


#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8	getSTBRestrictEnableServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setSTBRestrictEnableServicesValue(void* value, struct DeviceNode*  deviceNode);
int8	getSTBRestrictEnableServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setSTBRestrictEnableServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8	getSTBNumberServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setSTBNumberServicesValue(void* value, struct DeviceNode*  deviceNode);
int8	getSTBNumberServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setSTBNumberServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getCameraRestrictEnableServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setCameraRestrictEnableServicesValue(void* value, struct DeviceNode*  deviceNode);
int8	getCameraRestrictEnableServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setCameraRestrictEnableServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8	getCameraNumberServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setCameraNumberServicesValue(void* value, struct DeviceNode*  deviceNode);
int8	getCameraNumberServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setCameraNumberServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getComputerRestrictEnableServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setComputerRestrictEnableServicesValue(void* value, struct DeviceNode*  deviceNode);
int8	getComputerRestrictEnableServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setComputerRestrictEnableServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8	getComputerNumberServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setComputerNumberServicesValue(void* value, struct DeviceNode*  deviceNode);
int8	getComputerNumberServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setComputerNumberServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getPhoneRestrictEnableServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setPhoneRestrictEnableServicesValue(void* value, struct DeviceNode*  deviceNode);
int8	getPhoneRestrictEnableServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setPhoneRestrictEnableServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8	getPhoneNumberServicesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setPhoneNumberServicesValue(void* value, struct DeviceNode*  deviceNode);
int8	getPhoneNumberServicesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setPhoneNumberServicesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8	getUserNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setUserNumberOfEntriesValue(void* value, struct DeviceNode*  deviceNode);
int8	getUserNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setUserNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getFTPServerNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setFTPServerNumberOfEntriesValue(void* value, struct DeviceNode*  deviceNode);
int8	getFTPServerNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setFTPServerNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getFTPClientNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setFTPClientNumberOfEntriesValue(void* value, struct DeviceNode*  deviceNode);
int8	getFTPClientNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8	setFTPClientNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif
#endif
