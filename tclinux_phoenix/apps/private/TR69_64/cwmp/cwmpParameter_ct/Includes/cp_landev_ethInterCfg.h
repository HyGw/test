#ifndef CP_LANDEV_ETHINTERCFG_H_
#define CP_LANDEV_ETHINTERCFG_H_

#include "cp_core.h"
int8 getEthInterCfg_EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEthInterCfg_EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getEthInterCfg_EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthInterCfg_EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEthInterCfg_StatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthInterCfg_StatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthInterCfg_StatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getEthInterCfg_MACAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthInterCfg_MACAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthInterCfg_MACAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getStats_BytesStValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getStats_BytesStAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setStats_BytesStAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getStats_BytesRvdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getStats_BytesRvdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setStats_BytesRvdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getStats_PacketsStValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getStats_PacketsStAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setStats_PacketsStAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getStats_PacketsRvdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getStats_PacketsRvdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setStats_PacketsRvdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getEthInterCfg_MaxBitRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEthInterCfg_MaxBitRateValue(void* value, struct DeviceNode*  deviceNode);
int8 getEthInterCfg_MaxBitRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthInterCfg_MaxBitRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEthInterCfg_DuplexModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEthInterCfg_DuplexModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getEthInterCfg_DuplexModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthInterCfg_DuplexModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getEthInterCfg_X_CTCOM_ModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEthInterCfg_X_CTCOM_ModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getEthInterCfg_X_CTCOM_ModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthInterCfg_X_CTCOM_ModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEthInterCfg_X_CTCOM_VLANValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEthInterCfg_X_CTCOM_VLANValue(void* value, struct DeviceNode*  deviceNode);
int8 getEthInterCfg_X_CTCOM_VLANAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthInterCfg_X_CTCOM_VLANAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEthInterCfg_DetectionStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthInterCfg_DetectionStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthInterCfg_DetectionStatustAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CT_PMINFORM)
int8 getEthInterCfg_AccessEquipmentMacValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthInterCfg_AccessEquipmentMacAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthInterCfg_AccessEquipmentMacAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CMCC)
int8 getBSREnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setBSREnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getBSREnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setBSREnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#endif

