#ifndef CP_SERVICES_VOICEPROF_LINE_H_
#define CP_SERVICES_VOICEPROF_LINE_H_

#include "cp_core.h"

#if VOIP
#if defined(TCSUPPORT_CT)	
#define CODECLIST_NUMBER 9
#else
#define CODECLIST_NUMBER 8
#endif
#define CODECENTRY_NUMBER 4
#endif

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.VoiceProfile.Line.{i}.Codec.List.
	Parent Node Table Location: cp_Services_voiceProf_Line_nodetable.c
*/
extern CwmpParameterStruct ListIndex[];

int	cwmpInitLineCodecListSubTree( DeviceNodePtr deviceNode );

int8 getLineEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLineEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getLineEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getLineSIPAuthUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLineSIPAuthUserNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getLineSIPAuthUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineSIPAuthUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineSIPAuthPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLineSIPAuthPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getLineSIPAuthPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineSIPAuthPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT)
int8 getPBXPrefixEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPBXPrefixEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getPBXPrefixEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPBXPrefixEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPBXPrefixValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPBXPrefixValue(void* value, struct DeviceNode*  deviceNode);
int8 getPBXPrefixAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPBXPrefixAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPBXDialSecondaryEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPBXDialSecondaryEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getPBXDialSecondaryEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPBXDialSecondaryEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineH248PhysicalTermIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLineH248PhysicalTermIDValue(void* value, struct DeviceNode*  deviceNode);
int8 getLineH248PhysicalTermIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineH248PhysicalTermIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

int8 getLineSIPAuthURIValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLineSIPAuthURIValue(void* value, struct DeviceNode*  deviceNode);
int8 getLineSIPAuthURIAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineSIPAuthURIAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if CHINA_UNICOM_ENABLE
int8 getLineCodecListEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLineCodecListEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getLineCodecListEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineCodecListEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

int8 getEntryIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEntryIDValue(void* value, struct DeviceNode*  deviceNode);
int8 getEntryIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEntryIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEchoCancellationInUseValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEchoCancellationInUseValue(void* value, struct DeviceNode*  deviceNode);
int8 getEchoCancellationInUseAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEchoCancellationInUseAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTransmitGainValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTransmitGainValue(void* value, struct DeviceNode*  deviceNode);
int8 getTransmitGainAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTransmitGainAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getReceiveGainValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setReceiveGainValue(void* value, struct DeviceNode*  deviceNode);
int8 getReceiveGainAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setReceiveGainAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEchoCancellationTailValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEchoCancellationTailValue(void* value, struct DeviceNode*  deviceNode);
int8 getEchoCancellationTailAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEchoCancellationTailAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineStatsResetStatisticsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLineStatsResetStatisticsValue(void* value, struct DeviceNode*  deviceNode);
int8 getLineStatsResetStatisticsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineStatsResetStatisticsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineStatsPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLineStatsPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineStatsPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineStatsPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLineStatsPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineStatsPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineStatsBytesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLineStatsBytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineStatsBytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineStatsBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLineStatsBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineStatsBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineStatsPacketsLostValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLineStatsPacketsLostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineStatsPacketsLostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineStatsIncamingCallsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLineStatsIncamingCallsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineStatsIncamingCallsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineStatsIncamingCallsAnsweredValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLineStatsIncamingCallsAnsweredAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineStatsIncamingCallsAnsweredAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineStatsIncamingCallsConnectedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLineStatsIncamingCallsConnectedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineStatsIncamingCallsConnectedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineStatsIncamingCallsFailedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLineStatsIncamingCallsFailedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineStatsIncamingCallsFailedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getLineStatsOutgoingCallsAttempedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLineStatsOutgoingCallsAttempedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineStatsOutgoingCallsAttempedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineStatsOutgoingCallsAnsweredValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLineStatsOutgoingCallsAnsweredAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineStatsOutgoingCallsAnsweredAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getLineStatsOutgoingCallsConnectedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLineStatsOutgoingCallsConnectedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineStatsOutgoingCallsConnectedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineStatsOutgoingCallsFailedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLineStatsOutgoingCallsFailedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineStatsOutgoingCallsFailedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineStatsTotalCallTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLineStatsTotalCallTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineStatsTotalCallTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CUC_VOIP_NODE)
int8 getReceivePacketLossRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getReceivePacketLossRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setReceivePacketLossRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFarEndPacketLossRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getFarEndPacketLossRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFarEndPacketLossRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getReceiveInterarrivalJitterValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getReceiveInterarrivalJitterAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setReceiveInterarrivalJitterAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFarEndInterarrivalJitterValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getFarEndInterarrivalJitterAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFarEndInterarrivalJitterAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRoundTripDelayValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getRoundTripDelayAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRoundTripDelayAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCallsDroppedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCallsDroppedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCallsDroppedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getServerDownTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getServerDownTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setServerDownTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif


int8 getEchoCancellationEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEchoCancellationEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getEchoCancellationEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEchoCancellationEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCodecValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCodecValue(void* value, struct DeviceNode*  deviceNode);
int8 getCodecAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCodecAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPacketizationPeriodValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPacketizationPeriodValue(void* value, struct DeviceNode*  deviceNode);
int8 getPacketizationPeriodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPacketizationPeriodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPriorityValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPriorityValue(void* value, struct DeviceNode*  deviceNode);
int8 getPriorityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPriorityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLineStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


#if defined(TCSUPPORT_CT_PON_SC)
int8 getSubsUAValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSubsUAValue(void* value, struct DeviceNode*  deviceNode);
int8 getSubsUAAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSubsUAAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSubsRegValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSubsRegValue(void* value, struct DeviceNode*  deviceNode);
int8 getSubsRegAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSubsRegAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif


#if defined(TCSUPPORT_CT)
int8 getLineLastRegisterErrorReasonValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLineLastRegisterErrorReasonValue(void* value, struct DeviceNode*  deviceNode);
int8 getLineLastRegisterErrorReasonAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineLastRegisterErrorReasonAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
int8 getdial_tone_patternValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getdial_tone_patternAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setdial_tone_patternAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getmcid_serviceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getmcid_serviceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setmcid_serviceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getno_dialing_behaviourValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getno_dialing_behaviourAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setno_dialing_behaviourAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 gethold_serviceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 gethold_serviceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 sethold_serviceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getthree_party_serviceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getthree_party_serviceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setthree_party_serviceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getconference_uriValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setconference_uriValue(void* value, struct DeviceNode*  deviceNode);
int8 getconference_uriAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setconference_uriAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 gethotline_uriValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 sethotline_uriValue(void* value, struct DeviceNode*  deviceNode);
int8 gethotline_uriAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 sethotline_uriAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getcw_serviceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getcw_serviceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setcw_serviceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CUC_VOIP_NODE) || defined(TCSUPPORT_CT_PON_SC)
int8 getCallWaitingEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCallWaitingEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getCallWaitingEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCallWaitingEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if !defined(TCSUPPORT_CT_PON_SC)

int8 getCallForwardUnconditionalEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCallForwardUnconditionalEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getCallForwardUnconditionalEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCallForwardUnconditionalEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCallForwardUnconditionalNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCallForwardUnconditionalNumberValue(void* value, struct DeviceNode*  deviceNode);
int8 getCallForwardUnconditionalNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCallForwardUnconditionalNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCallForwardOnBusyEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCallForwardOnBusyEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getCallForwardOnBusyEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCallForwardOnBusyEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCallForwardOnBusyNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCallForwardOnBusyNumberValue(void* value, struct DeviceNode*  deviceNode);
int8 getCallForwardOnBusyNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCallForwardOnBusyNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCallForwardOnNoAnswerEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCallForwardOnNoAnswerEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getCallForwardOnNoAnswerEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCallForwardOnNoAnswerEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCallForwardOnNoAnswerNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCallForwardOnNoAnswerNumberValue(void* value, struct DeviceNode*  deviceNode);
int8 getCallForwardOnNoAnswerNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCallForwardOnNoAnswerNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCallForwardOnNoAnswerRingCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCallForwardOnNoAnswerRingCountValue(void* value, struct DeviceNode*  deviceNode);
int8 getCallForwardOnNoAnswerRingCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCallForwardOnNoAnswerRingCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCallTransferEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCallTransferEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getCallTransferEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCallTransferEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMWIEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMWIEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getMWIEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMWIEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getX_CU_HotlineEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setX_CU_HotlineEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getX_CU_HotlineEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setX_CU_HotlineEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getX_CU_HotlineNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setX_CU_HotlineNumberValue(void* value, struct DeviceNode*  deviceNode);
int8 getX_CU_HotlineNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setX_CU_HotlineNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getX_CU_HotlineTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setX_CU_HotlineTimerValue(void* value, struct DeviceNode*  deviceNode);
int8 getX_CU_HotlineTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setX_CU_HotlineTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getX_CU_conference_uriValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setX_CU_conference_uriValue(void* value, struct DeviceNode*  deviceNode);
int8 getX_CU_conference_uriAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setX_CU_conference_uriAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPhyReferenceListValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPhyReferenceListValue(void* value, struct DeviceNode*  deviceNode);
int8 getPhyReferenceListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPhyReferenceListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

#endif

#ifdef TCSUPPORT_PON_ROSTELECOM
int8 getX_RTK_3PTYValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setX_RTK_3PTYValue(void* value, struct DeviceNode*  deviceNode);
int8 getX_RTK_3PTYAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setX_RTK_3PTYAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CUC_VOIP_NODE) || defined(TCSUPPORT_CT_PON_SC)
int8 getCallStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCallStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCallStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CUC_VOIP_NODE) || defined(TCSUPPORT_CT_PON_GD)
int8 getDirectoryNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDirectoryNumberValue(void* value, struct DeviceNode*  deviceNode);
int8 getDirectoryNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDirectoryNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

#endif 

