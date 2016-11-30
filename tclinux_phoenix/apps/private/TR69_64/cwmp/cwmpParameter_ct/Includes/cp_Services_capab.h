#ifndef CP_SERVICES_CAPAB_H_
#define CP_SERVICES_CAPAB_H_

#include "cp_core.h"

int8 getMaxProfileCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMaxProfileCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMaxProfileCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMaxLineCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMaxLineCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMaxLineCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMaxSessionsPerLineValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMaxSessionsPerLineAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMaxSessionsPerLineAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMaxSessionCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMaxSessionCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMaxSessionCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRegionsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getRegionsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRegionsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRTCPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getRTCPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRTCPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSRTPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSRTPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSRTPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRTPRedundancyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getRTPRedundancyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRTPRedundancyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDSCPCoupledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDSCPCoupledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDSCPCoupledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEthernetTaggingCoupledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthernetTaggingCoupledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthernetTaggingCoupledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPSTNSoftSwitchOverValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getPSTNSoftSwitchOverAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPSTNSoftSwitchOverAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFaxT38Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getFaxT38Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFaxT38Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFaxPassThroughValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getFaxPassThroughAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFaxPassThroughAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getModemPassThroughValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getModemPassThroughAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setModemPassThroughAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getToneGenerationValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getToneGenerationAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setToneGenerationAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
		
int8 getRingGenerationValue(char** value, ParameterType* type, DeviceNode*	deviceNode);
int8 getRingGenerationAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRingGenerationAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*	deviceNode);
		
int8 getButtonMapValue(char** value, ParameterType* type, DeviceNode*	deviceNode);
int8 getButtonMapAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setButtonMapAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*	deviceNode);

int8 getCUNumberingPlanValue(char** value, ParameterType* type, DeviceNode*	deviceNode);
int8 getCUNumberingPlanAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCUNumberingPlanAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*	deviceNode);

int8 getVoicePortTestsValue(char** value, ParameterType* type, DeviceNode*	deviceNode);
int8 getVoicePortTestsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVoicePortTestsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*	deviceNode);

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.Phyinterface.{i}.CodecsList.{i}
	Parent Node Table Location: cp_Services_nodetable.c
*/

extern CwmpParameterStruct CodecsList[];
/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.CodecsList.
	Parent Node Table Location: cp_Services_nodetable.c
*/

extern CwmpParameterStruct CodecsIndex[];
int cwmpInitCodecsSubTree( DeviceNodePtr deviceNode );


int8 getCap_DigitMapValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCap_DigitMapAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCap_DigitMapAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSignalingProtocolsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSignalingProtocolsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSignalingProtocolsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT)
int8 getHeartbeatValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHeartbeatAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHeartbeatAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

int8 getEntryIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEntryIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEntryIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCs_CodecValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCs_CodecAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCs_CodecAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getBitRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getBitRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setBitRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCs_PacketizationPeriodValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCs_PacketizationPeriodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCs_PacketizationPeriodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSilenceSuppressionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSilenceSuppressionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSilenceSuppressionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

