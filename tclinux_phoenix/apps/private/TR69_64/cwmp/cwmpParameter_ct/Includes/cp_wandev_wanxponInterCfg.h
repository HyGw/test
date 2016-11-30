#ifndef CP_WANDEV_WANXPONINTERCFG_H_
#define CP_WANDEV_WANXPONINTERCFG_H_

#include "cp_core.h"

/*
    rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.X_CT-COM_EponInterfaceConfig.
	                                 InternetGatewayDevice.WANDevice.1.X_CT-COM_GponInterfaceConfig.
*/
extern CwmpParameterStruct WanEPonInterCfg_Stats[];
extern CwmpParameterStruct WanGPonInterCfg_Stats[];
extern CwmpParameterStruct ponOpticalTransceiver[];

int8 getWANPONInterCfg_StatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_StatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_StatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_TXPowerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_TXPowerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_TXPowerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_RXPowerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_RXPowerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_RXPowerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_TransTemperatureValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_TransTemperatureAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_TransTemperatureAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_SupplyVottageValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_SupplyVottageAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_SupplyVottageAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_BiasCurrentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_BiasCurrentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_BiasCurrentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT_PMINFORM)
int8 getWANPONInterCfg_DownDataValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_DownDataAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_DownDataAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_UpDataValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_UpDataAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_UpDataAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_TEMPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_TEMPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_TEMPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PacketLostValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PacketLostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PacketLostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PLRValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PLRAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PLRAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_ErrorCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_ErrorCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_ErrorCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_SERValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_SERAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_SERAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_MemRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_MemRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_MemRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_CPURateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_CPURateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_CPURateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PMInformEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPONInterCfg_PMInformEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PMInformEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PMInformEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PMInformLSURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPONInterCfg_PMInformLSURLValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PMInformLSURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PMInformLSURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PMInformLUIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPONInterCfg_PMInformLUIntervalValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PMInformLUIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PMInformLUIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PMInformLCIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPONInterCfg_PMInformLCIntervalValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PMInformLCIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PMInformLCIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PMInformCREnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPONInterCfg_PMInformCREnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PMInformCREnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PMInformCREnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PMInformMREnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPONInterCfg_PMInformMREnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PMInformMREnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PMInformMREnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PMInformSEREnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPONInterCfg_PMInformSEREnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PMInformSEREnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PMInformSEREnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PMInformECEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPONInterCfg_PMInformECEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PMInformECEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PMInformECEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PMInformPLREnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPONInterCfg_PMInformPLREnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PMInformPLREnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PMInformPLREnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PMInformPLEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPONInterCfg_PMInformPLEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PMInformPLEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PMInformPLEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PMInformTMPEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPONInterCfg_PMInformTMPEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PMInformTMPEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PMInformTMPEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PMInformUpDEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPONInterCfg_PMInformUpDEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PMInformUpDEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PMInformUpDEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PMInformDwDEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPONInterCfg_PMInformDwDEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PMInformDwDEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PMInformDwDEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PMInformLanDevMacEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPONInterCfg_PMInformLanDevMacEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PMInformLanDevMacEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PMInformLanDevMacEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PMInformLanDevMacValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PMInformLanDevMacAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PMInformLanDevMacAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PMInformWlanDevMacEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPONInterCfg_PMInformWlanDevMacEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PMInformWlanDevMacEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PMInformWlanDevMacEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_PMInformWlanDevMacValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_PMInformWlanDevMacAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_PMInformWlanDevMacAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getWANPONInterCfg_EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPONInterCfg_EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPONInterCfg_EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_StandardsSupportedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_StandardsSupportedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_StandardsSupportedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_StandardUsedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_StandardUsedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_StandardUsedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_UpstreamMaxRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_UpstreamMaxRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_UpstreamMaxRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPONInterCfg_DownstreamMaxRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPONInterCfg_DownstreamMaxRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPONInterCfg_DownstreamMaxRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#endif

