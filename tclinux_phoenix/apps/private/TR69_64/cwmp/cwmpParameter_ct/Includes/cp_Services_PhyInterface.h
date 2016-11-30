#ifndef CP_SERVICES_PHYINTERFACE_H_
#define CP_SERVICES_PHYINTERFACE_H_

#include "cp_core.h"

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.Phyinterface.{i}.poorQualitylist.{i}
	Parent Node Table Location: cp_Services_nodetable.c
*/

extern CwmpParameterStruct PoorQualityList[];


/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.poorQualitylist.
	Parent Node Table Location: cp_Services_nodetable.c
*/

extern CwmpParameterStruct PoorQualityListIndex[];


int cwmpInitPoorQualityListSubTree( DeviceNodePtr deviceNode );

int8 getInterfaceIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setInterfaceIDValue(void* value, struct DeviceNode*  deviceNode);
int8 getInterfaceIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInterfaceIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPhyPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPhyPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getPhyPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPhyPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTestStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTestStateValue(void* value, struct DeviceNode*  deviceNode);
int8 getTestStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTestStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTestSelectorValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTestSelectorValue(void* value, struct DeviceNode*  deviceNode);
int8 getTestSelectorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTestSelectorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPhoneConnectivityValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getPhoneConnectivityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPhoneConnectivityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT)
int8 getTestTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTestTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getTestTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTestTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCalledNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCalledNumberValue(void* value, struct DeviceNode*  deviceNode);
int8 getCalledNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCalledNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCallHoldTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCallHoldTimerValue(void* value, struct DeviceNode*  deviceNode);
int8 getCallHoldTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCallHoldTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCalledWaitTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCalledWaitTimerValue(void* value, struct DeviceNode*  deviceNode);
int8 getCalledWaitTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCalledWaitTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDailDTMFConfirmEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDailDTMFConfirmEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getDailDTMFConfirmEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDailDTMFConfirmEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDailDTMFConfirmNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDailDTMFConfirmNumberValue(void* value, struct DeviceNode*  deviceNode);
int8 getDailDTMFConfirmNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDailDTMFConfirmNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDailDTMFConfirmResultValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDailDTMFConfirmResultAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDailDTMFConfirmResultAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getConclusionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getConclusionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setConclusionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCallerFailReasonValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCallerFailReasonAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCallerFailReasonAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCalledFailReasonValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCalledFailReasonAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCalledFailReasonAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFailedResponseCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getFailedResponseCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFailedResponseCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

int8 getStatTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getStatTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setStatTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTxPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getTxPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTxPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRxPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getRxPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRxPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMeanDelayValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMeanDelayAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMeanDelayAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMeanJitterValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMeanJitterAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMeanJitterAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFractionLossValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getFractionLossAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFractionLossAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLocalIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLocalIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLocalIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLocalUDPPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLocalUDPPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLocalUDPPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFarEndIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getFarEndIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFarEndIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFarEndUDPPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getFarEndUDPPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFarEndUDPPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMosLqValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMosLqAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMosLqAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getListCodecValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getListCodecAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setListCodecAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIsTestOnBusyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIsTestOnBusyValue(void* value, struct DeviceNode*  deviceNode);
int8 getIsTestOnBusyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIsTestOnBusyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineTestConclusionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLineTestConclusionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineTestConclusionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAGACVoltageValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAGACVoltageAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAGACVoltageAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getBGACVoltageValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getBGACVoltageAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setBGACVoltageAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getABACVoltageValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getABACVoltageAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setABACVoltageAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAGDCVoltageValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAGDCVoltageAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAGDCVoltageAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getBGDCVoltageValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getBGDCVoltageAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setBGDCVoltageAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getABDCVoltageValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getABDCVoltageAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setABDCVoltageAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getAGInsulationResistanceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAGInsulationResistanceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAGInsulationResistanceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getBGInsulationResistanceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getBGInsulationResistanceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setBGInsulationResistanceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getABInsulationResistanceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getABInsulationResistanceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setABInsulationResistanceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAGCapacitanceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAGCapacitanceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAGCapacitanceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getBGCapacitanceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getBGCapacitanceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setBGCapacitanceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getABCapacitanceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getABCapacitanceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setABCapacitanceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDialModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDialModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getDialModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDialModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHookFlashUpTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setHookFlashUpTimeValue(void* value, struct DeviceNode*  deviceNode);
int8 getHookFlashUpTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHookFlashUpTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHookFlashDownTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setHookFlashDownTimeValue(void* value, struct DeviceNode*  deviceNode);
int8 getHookFlashDownTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHookFlashDownTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getReversePolePulseValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setReversePolePulseValue(void* value, struct DeviceNode*  deviceNode);
int8 getReversePolePulseAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setReversePolePulseAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getKcTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setKcTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getKcTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setKcTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getClipTransWhenValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setClipTransWhenValue(void* value, struct DeviceNode*  deviceNode);
int8 getClipTransWhenAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setClipTransWhenAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getClipFormatValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setClipFormatValue(void* value, struct DeviceNode*  deviceNode);
int8 getClipFormatAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setClipFormatAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getClipSendDateTimeEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setClipSendDateTimeEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getClipSendDateTimeEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setClipSendDateTimeEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getClipSendDateTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setClipSendDateTimeValue(void* value, struct DeviceNode*  deviceNode);
int8 getClipSendDateTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setClipSendDateTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRingDCVoltageOverlappedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRingDCVoltageOverlappedValue(void* value, struct DeviceNode*  deviceNode);
int8 getRingDCVoltageOverlappedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRingDCVoltageOverlappedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getDataServiceModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDataServiceModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getDataServiceModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDataServiceModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getClipReversePoleValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setClipReversePoleValue(void* value, struct DeviceNode*  deviceNode);
int8 getClipReversePoleAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setClipReversePoleAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMWIRingFlagValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMWIRingFlagValue(void* value, struct DeviceNode*  deviceNode);
int8 getMWIRingFlagAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMWIRingFlagAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


#endif
