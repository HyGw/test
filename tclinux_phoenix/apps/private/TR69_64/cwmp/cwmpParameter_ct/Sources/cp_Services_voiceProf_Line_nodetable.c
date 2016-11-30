#include "Global_res.h"
#include "cp_Services_voiceProf_Line.h"


static NodeFunc Enable =
{
	NULL,
	NULL,
	getLineEnableValue,
	setLineEnableValue,
	getLineEnableAttribute,
	setLineEnableAttribute,
	NULL
};

static NodeFunc AuthUserName =
{
	NULL,
	NULL,
	getLineSIPAuthUserNameValue,
	setLineSIPAuthUserNameValue,
	getLineSIPAuthUserNameAttribute,
	setLineSIPAuthUserNameAttribute,
	NULL
};

static NodeFunc AuthPassword =
{
	NULL,
	NULL,
	getLineSIPAuthPasswordValue,
	setLineSIPAuthPasswordValue,
	getLineSIPAuthPasswordAttribute,
	setLineSIPAuthPasswordAttribute,
	NULL
};
static NodeFunc AuthURI =
{
	NULL,
	NULL,
	getLineSIPAuthURIValue,
	setLineSIPAuthURIValue,
	getLineSIPAuthURIAttribute,
	setLineSIPAuthURIAttribute,
	NULL
};

#if defined(TCSUPPORT_CT)
static NodeFunc PBXPrefixEnableFuncs =
{
	NULL,
	NULL,
	getPBXPrefixEnableValue,
	setPBXPrefixEnableValue,
	getPBXPrefixEnableAttribute,
	setPBXPrefixEnableAttribute,
	NULL
};

static NodeFunc PBXPrefixFuncs =
{
	NULL,
	NULL,
	getPBXPrefixValue,
	setPBXPrefixValue,
	getPBXPrefixAttribute,
	setPBXPrefixAttribute,
	NULL
};

static NodeFunc PBXDialSecondaryEnableFuncs =
{
	NULL,
	NULL,
	getPBXDialSecondaryEnableValue,
	setPBXDialSecondaryEnableValue,
	getPBXDialSecondaryEnableAttribute,
	setPBXDialSecondaryEnableAttribute,
	NULL
};


static NodeFunc PhysicalTermID =
{
	NULL,
	NULL,
	getLineH248PhysicalTermIDValue,
	setLineH248PhysicalTermIDValue,
	getLineH248PhysicalTermIDAttribute,
	setLineH248PhysicalTermIDAttribute,
	NULL
};
#endif

static NodeFunc ResetStatistics =
{
	NULL,
	NULL,
	getLineStatsResetStatisticsValue,
	setLineStatsResetStatisticsValue,
	getLineStatsResetStatisticsAttribute,
	setLineStatsResetStatisticsAttribute,
	NULL
};

static NodeFunc LinePacketsSent =
{
	NULL,
	NULL,
	getLineStatsPacketsSentValue,
	NULL,
	getLineStatsPacketsSentAttribute,
	setLineStatsPacketsSentAttribute,
	NULL
};


static NodeFunc LinePacketsReceived =
{
	NULL,
	NULL,
	getLineStatsPacketsReceivedValue,
	NULL,
	getLineStatsPacketsReceivedAttribute,
	setLineStatsPacketsReceivedAttribute,
	NULL
};


static NodeFunc LineBytesSent =
{
	NULL,
	NULL,
	getLineStatsBytesSentValue,
	NULL,
	getLineStatsBytesSentAttribute,
	setLineStatsBytesSentAttribute,
	NULL
};


static NodeFunc LineBytesReceived =
{
	NULL,
	NULL,
	getLineStatsBytesReceivedValue,
	NULL,
	getLineStatsBytesReceivedAttribute,
	setLineStatsBytesReceivedAttribute,
	NULL
};

static NodeFunc PacketsLost =
{
	NULL,
	NULL,
	getLineStatsPacketsLostValue,
	NULL,
	getLineStatsPacketsLostAttribute,
	setLineStatsPacketsLostAttribute,
	NULL
};


static NodeFunc IncamingCallsReceived =
{
	NULL,
	NULL,
	getLineStatsIncamingCallsReceivedValue,
	NULL,
	getLineStatsIncamingCallsReceivedAttribute,
	setLineStatsIncamingCallsReceivedAttribute,
	NULL
};


static NodeFunc IncamingCallsAnswered =
{
	NULL,
	NULL,
	getLineStatsIncamingCallsAnsweredValue,
	NULL,
	getLineStatsIncamingCallsAnsweredAttribute,
	setLineStatsIncamingCallsAnsweredAttribute,
	NULL
};


static NodeFunc IncamingCallsConnected =
{
	NULL,
	NULL,
	getLineStatsIncamingCallsConnectedValue,
	NULL,
	getLineStatsIncamingCallsConnectedAttribute,
	setLineStatsIncamingCallsConnectedAttribute,
	NULL
};


static NodeFunc IncamingCallsFailed =
{
	NULL,
	NULL,
	getLineStatsIncamingCallsFailedValue,
	NULL,
	getLineStatsIncamingCallsFailedAttribute,
	setLineStatsIncamingCallsFailedAttribute,
	NULL
};


static NodeFunc OutgoingCallsAttemped =
{
	NULL,
	NULL,
	getLineStatsOutgoingCallsAttempedValue,
	NULL,
	getLineStatsOutgoingCallsAttempedAttribute,
	setLineStatsOutgoingCallsAttempedAttribute,
	NULL
};


static NodeFunc OutgoingCallsAnswered =
{
	NULL,
	NULL,
	getLineStatsOutgoingCallsAnsweredValue,
	NULL,
	getLineStatsOutgoingCallsAnsweredAttribute,
	setLineStatsOutgoingCallsAnsweredAttribute,
	NULL
};


static NodeFunc OutgoingCallsConnected =
{
	NULL,
	NULL,
	getLineStatsOutgoingCallsConnectedValue,
	NULL,
	getLineStatsOutgoingCallsConnectedAttribute,
	setLineStatsOutgoingCallsConnectedAttribute,
	NULL
};


static NodeFunc OutgoingCallsFailed =
{
	NULL,
	NULL,
	getLineStatsOutgoingCallsFailedValue,
	NULL,
	getLineStatsOutgoingCallsFailedAttribute,
	setLineStatsOutgoingCallsFailedAttribute,
	NULL
};


static NodeFunc TotalCallTime =
{
	NULL,
	NULL,
	getLineStatsTotalCallTimeValue,
	NULL,
	getLineStatsTotalCallTimeAttribute,
	setLineStatsTotalCallTimeAttribute,
	NULL
};

#if defined(TCSUPPORT_CUC_VOIP_NODE)
static NodeFunc ReceivePacketLossRate =
{
	NULL,
	NULL,
	getReceivePacketLossRateValue,
	NULL,
	getReceivePacketLossRateAttribute,
	setReceivePacketLossRateAttribute,
	NULL
};

static NodeFunc FarEndPacketLossRate =
{
	NULL,
	NULL,
	getFarEndPacketLossRateValue,
	NULL,
	getFarEndPacketLossRateAttribute,
	setFarEndPacketLossRateAttribute,
	NULL
};

static NodeFunc ReceiveInterarrivalJitter =
{
	NULL,
	NULL,
	getReceiveInterarrivalJitterValue,
	NULL,
	getReceiveInterarrivalJitterAttribute,
	setReceiveInterarrivalJitterAttribute,
	NULL
};

static NodeFunc FarEndInterarrivalJitter =
{
	NULL,
	NULL,
	getFarEndInterarrivalJitterValue,
	NULL,
	getFarEndInterarrivalJitterAttribute,
	setFarEndInterarrivalJitterAttribute,
	NULL
};

static NodeFunc RoundTripDelay =
{
	NULL,
	NULL,
	getRoundTripDelayValue,
	NULL,
	getRoundTripDelayAttribute,
	setRoundTripDelayAttribute,
	NULL
};

static NodeFunc CallsDropped =
{
	NULL,
	NULL,
	getCallsDroppedValue,
	NULL,
	getCallsDroppedAttribute,
	setCallsDroppedAttribute,
	NULL
};

static NodeFunc ServerDownTime =
{
	NULL,
	NULL,
	getServerDownTimeValue,
	NULL,
	getServerDownTimeAttribute,
	setServerDownTimeAttribute,
	NULL
};


#endif


/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.Line.{i}.SIP
	Parent Node Table Location: cp_Services_voiceProf_Line_nodetable.c
*/
CwmpParameterStruct SIP[] = 
{
	{"AuthUserName", StrW, NoLimite, &AuthUserName, NULL},
	{"AuthPassword", StrW, NoLimite, &AuthPassword, NULL},
	{"URI", StrW, NoLimite, &AuthURI, NULL},
	{NULL, -1, -1, NULL, NULL}
};

#if defined(TCSUPPORT_CT)
CwmpParameterStruct H248[] = 
{
	{"PhysicalTermID", StrW, NoLimite, &PhysicalTermID, NULL},
    {"LineName", StrW, NoLimite, &PhysicalTermID, NULL},
	{NULL, -1, -1, NULL, NULL}
};
#endif

CwmpParameterStruct LineStats[] =  
{
	{"ResetStatistics", BooleanW, NoLimite, &ResetStatistics, NULL},
	{"PacketsSent", UnsignIntR, NoLimite, &LinePacketsSent, NULL},
	{"PacketsReceived", UnsignIntR, NoLimite, &LinePacketsReceived, NULL},
	{"BytesSent", UnsignIntR, NoLimite, &LineBytesSent, NULL},
	{"BytesReceived", UnsignIntR, NoLimite, &LineBytesReceived, NULL},
	{"PacketsLost", UnsignIntR, NoLimite, &PacketsLost, NULL},
	{"IncomingCallsReceived", UnsignIntR, NoLimite, &IncamingCallsReceived, NULL},
	{"IncomingCallsAnswered", UnsignIntR, NoLimite, &IncamingCallsAnswered, NULL},
	{"IncomingCallsConnected", UnsignIntR, NoLimite, &IncamingCallsConnected, NULL},
	{"IncomingCallsFailed", UnsignIntR, NoLimite, &IncamingCallsFailed, NULL},
	{"OutgoingCallsAttempted", UnsignIntR, NoLimite, &OutgoingCallsAttemped, NULL},
	{"OutgoingCallsAnswered", UnsignIntR, NoLimite, &OutgoingCallsAnswered, NULL},
	{"OutgoingCallsConnected", UnsignIntR, NoLimite, &OutgoingCallsConnected, NULL},
	{"OutgoingCallsFailed", UnsignIntR, NoLimite, &OutgoingCallsFailed, NULL},
	{"TotalCallTime", UnsignIntR, NoLimite, &TotalCallTime, NULL},

#if defined(TCSUPPORT_CUC_VOIP_NODE)
       {"ReceivePacketLossRate", UnsignIntR, NoLimite, &ReceivePacketLossRate, NULL},
       {"FarEndPacketLossRate", UnsignIntR, NoLimite, &FarEndPacketLossRate, NULL},
       {"ReceiveInterarrivalJitter", UnsignIntR, NoLimite, &ReceiveInterarrivalJitter, NULL},
       {"FarEndInterarrivalJitter", UnsignIntR, NoLimite, &FarEndInterarrivalJitter, NULL},
       {"RoundTripDelay", UnsignIntR, NoLimite, &RoundTripDelay, NULL},
       {"CallsDropped", UnsignIntR, NoLimite, &CallsDropped, NULL},
       {"ServerDownTime", UnsignIntR, NoLimite, &ServerDownTime, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc EchoCancellationEnable =
{
	NULL,
	NULL,
	getEchoCancellationEnableValue,
	setEchoCancellationEnableValue,
	getEchoCancellationEnableAttribute,
	setEchoCancellationEnableAttribute,
	NULL
};

static NodeFunc EchoCancellationInUse =
{
	NULL,
	NULL,
	getEchoCancellationInUseValue,
#if defined(TCSUPPORT_CT)
	NULL,
#else
	setEchoCancellationInUseValue,
#endif
	getEchoCancellationInUseAttribute,
	setEchoCancellationInUseAttribute,
	NULL
};

static NodeFunc TransmitGain =
{
	NULL,
	NULL,
	getTransmitGainValue,
	setTransmitGainValue,
	getTransmitGainAttribute,
	setTransmitGainAttribute,
	NULL
};

static NodeFunc ReceiveGain =
{
	NULL,
	NULL,
	getReceiveGainValue,
	setReceiveGainValue,
	getReceiveGainAttribute,
	setReceiveGainAttribute,
	NULL
};

static NodeFunc EchoCancellationTail =
{
	NULL,
	NULL,
	getEchoCancellationTailValue,
#if defined(TCSUPPORT_CT)
	NULL,
#else
	setEchoCancellationTailValue,
#endif
	getEchoCancellationTailAttribute,
	setEchoCancellationTailAttribute,
	NULL
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.VoiceProfile.Line.{i}.VoiceProcessing
	Parent Node Table Location: cp_Services_voiceProf_Line_nodetable.cs
*/
CwmpParameterStruct VoiceProcessing[] = 
{
	{"EchoCancellationEnable", BooleanW, NoLimite, &EchoCancellationEnable, NULL},
#if defined(TCSUPPORT_CT)
	{"EchoCancellationInUse", BooleanR, NoLimite, &EchoCancellationInUse, NULL},
#else
	{"EchoCancellationInUse", BooleanW, NoLimite, &EchoCancellationInUse, NULL},
#endif
	{"TransmitGain", IntW, NoLimite, &TransmitGain, NULL},
	{"ReceiveGain", IntW, NoLimite, &ReceiveGain, NULL},
#if defined(TCSUPPORT_CT)
	{"EchoCancellationTail", UnsignIntR, NoLimite, &EchoCancellationTail, NULL},
#else
	{"EchoCancellationTail", UnsignIntW, NoLimite, &EchoCancellationTail, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc EntryID =
{
	NULL,
	NULL,
	getEntryIDValue,
	NULL,
	getEntryIDAttribute,
	setEntryIDAttribute,
	NULL
};

#if CHINA_UNICOM_ENABLE
static NodeFunc LineCodecListEnable =
{
	NULL,
	NULL,
	getLineCodecListEnableValue,
	setLineCodecListEnableValue,
	getLineCodecListEnableAttribute,
	setLineCodecListEnableAttribute,
	NULL
	};

#endif

static NodeFunc Codec_1 =
{
	NULL,
	NULL,
	getCodecValue,
#if defined(TCSUPPORT_CT)			
	setCodecValue,
#else
	NULL,
#endif
	getCodecAttribute,
	setCodecAttribute,
	NULL
};

static NodeFunc PacketizationPeriod =
{
	NULL,
	NULL,
	getPacketizationPeriodValue,
	setPacketizationPeriodValue,
	getPacketizationPeriodAttribute,
	setPacketizationPeriodAttribute,
	NULL
};

static NodeFunc Priority =
{
	NULL,
	NULL,
	getPriorityValue,
	setPriorityValue,
	getPriorityAttribute,
	setPriorityAttribute,
	NULL
};
/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.VoiceProfile.Line.{i}.Codec.List.{i}
	Parent Node Table Location: cp_Services_voiceProf_Line_nodetable.c
*/
CwmpParameterStruct List[] = 
{
#if CHINA_UNICOM_ENABLE
	{"EntityID", UnsignIntR, NoLimite, &EntryID, NULL},
	{"Enable", BooleanW, NoLimite, &LineCodecListEnable, NULL},
#endif
#if defined(TCSUPPORT_CT)	
	{"EntryID", UnsignIntR, NoLimite, &EntryID, NULL},
	{"Codec",	StrW, NoLimite, &Codec_1, NULL},
#else
	{"EntryID", UnsignIntR, NoLimite, &EntryID, NULL},
	{"Codec",   StrW, NoLimite, &Codec_1, NULL},
#endif
	{"PacketizationPeriod", StrW, NoLimite, &PacketizationPeriod, NULL},
	{"Priority", UnsignIntW, NoLimite, &Priority, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.VoiceProfile.Line.{i}.Codec.List.
	Parent Node Table Location: cp_Services_voiceProf_Line_nodetable.c
*/
CwmpParameterStruct ListIndex[] = 
{
	{NULL, ObjectR, NoLimite, NULL, List},
	{NULL, -1, -1, NULL, NULL}
};

NodeFunc ListFuncs =
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	cwmpInitLineCodecListSubTree
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.VoiceProfile.Line.{i}.Codec
	Parent Node Table Location: cp_Services_voiceProf_Line_nodetable.c
*/
CwmpParameterStruct Codec[] = 
{
	{"List", ObjectR, NoLimite, &ListFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc StatusFuncs =
{
	NULL,
	NULL,
	getLineStatusValue,
	NULL,
	getLineStatusAttribute,
	setLineStatusAttribute,
	NULL
};


#if defined(TCSUPPORT_CT)
static NodeFunc LastRegisterErrorReason =
{
	NULL,
	NULL,
	getLineLastRegisterErrorReasonValue,
	setLineLastRegisterErrorReasonValue,
	getLineLastRegisterErrorReasonAttribute,
	setLineLastRegisterErrorReasonAttribute,
	NULL
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.VoiceProfile.Line.{i}.X_CT-COM_LastRegisterError.
	Parent Node Table Location: cp_Services_voiceProf_Line_nodetable.c
*/
CwmpParameterStruct LastRegisterErrorFunc[] = 
{
	{"Reason", StrW, NoLimite, &LastRegisterErrorReason, NULL},
	{NULL, -1, -1, NULL, NULL}
};

#endif
static NodeFunc dial_tone_patternFuncs =
{
	NULL,
	NULL,
	getdial_tone_patternValue,
	NULL,
	getdial_tone_patternAttribute,
	setdial_tone_patternAttribute,
	NULL
};

static NodeFunc mcid_serviceFuncs =
{
	NULL,
	NULL,
	getmcid_serviceValue,
	NULL,
	getmcid_serviceAttribute,
	setmcid_serviceAttribute,
	NULL
};

static NodeFunc no_dialing_behaviourFuncs =
{
	NULL,
	NULL,
	getno_dialing_behaviourValue,
	NULL,
	getno_dialing_behaviourAttribute,
	setno_dialing_behaviourAttribute,
	NULL
};

static NodeFunc hold_serviceFuncs =
{
	NULL,
	NULL,
	gethold_serviceValue,
	NULL,
	gethold_serviceAttribute,
	sethold_serviceAttribute,
	NULL
};

static NodeFunc three_party_serviceFuncs =
{
	NULL,
	NULL,
	getthree_party_serviceValue,
	NULL,
	getthree_party_serviceAttribute,
	setthree_party_serviceAttribute,
	NULL
};

static NodeFunc conference_uriFuncs =
{
	NULL,
	NULL,
	getconference_uriValue,
	setconference_uriValue,
	getconference_uriAttribute,
	setconference_uriAttribute,
	NULL
};

static NodeFunc hotline_uriFuncs =
{
	NULL,
	NULL,
	gethotline_uriValue,
	sethotline_uriValue,
	gethotline_uriAttribute,
	sethotline_uriAttribute,
	NULL
};

static NodeFunc cw_serviceFuncs =
{
	NULL,
	NULL,
	getcw_serviceValue,
	NULL,
	getcw_serviceAttribute,
	setcw_serviceAttribute,
	NULL
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.VoiceProfile.Line.{i}.X_CT-COM_IMS.
	Parent Node Table Location: cp_Services_voiceProf_Line_nodetable.c
*/
CwmpParameterStruct COM_IMS[] = 
{
	{"dial-tone-pattern", UnsignIntR, NoLimite, &dial_tone_patternFuncs, NULL},
	{"mcid-service", UnsignIntR, NoLimite, &mcid_serviceFuncs, NULL},	
	{"no-dialing-behaviour", UnsignIntR, NoLimite,	&no_dialing_behaviourFuncs,	NULL},
	{"hold-service", UnsignIntR, NoLimite, &hold_serviceFuncs, NULL},
	{"three-party-service",	UnsignIntR, NoLimite, &three_party_serviceFuncs, NULL},
	{"conference-uri", StrW, NoLimite, &conference_uriFuncs, NULL},	
	{"hotline-uri",	StrW, NoLimite,	&hotline_uriFuncs, NULL},
	{"cw-service", UnsignIntR, NoLimite, &cw_serviceFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};

#if defined(TCSUPPORT_CUC_VOIP_NODE) || defined(TCSUPPORT_CT_PON_SC)
static NodeFunc CallWaitingEnable =
{
	NULL,
	NULL,
	getCallWaitingEnableValue,
	setCallWaitingEnableValue,
	getCallWaitingEnableAttribute,
	setCallWaitingEnableAttribute,
	NULL
};

#if !defined(TCSUPPORT_CT_PON_SC)

static NodeFunc CallForwardUnconditionalEnable =
{
	NULL,
	NULL,
	getCallForwardUnconditionalEnableValue,
	setCallForwardUnconditionalEnableValue,
	getCallForwardUnconditionalEnableAttribute,
	setCallForwardUnconditionalEnableAttribute,
	NULL
};

static NodeFunc CallForwardUnconditionalNumber =
{
	NULL,
	NULL,
	getCallForwardUnconditionalNumberValue,
	setCallForwardUnconditionalNumberValue,
	getCallForwardUnconditionalNumberAttribute,
	setCallForwardUnconditionalNumberAttribute,
	NULL
};

static NodeFunc CallForwardOnBusyEnable =
{
	NULL,
	NULL,
	getCallForwardOnBusyEnableValue,
	setCallForwardOnBusyEnableValue,
	getCallForwardOnBusyEnableAttribute,
	setCallForwardOnBusyEnableAttribute,
	NULL
};

static NodeFunc CallForwardOnBusyNumber =
{
	NULL,
	NULL,
	getCallForwardOnBusyNumberValue,
	setCallForwardOnBusyNumberValue,
	getCallForwardOnBusyNumberAttribute,
	setCallForwardOnBusyNumberAttribute,
	NULL
};

static NodeFunc CallForwardOnNoAnswerEnable =
{
	NULL,
	NULL,
	getCallForwardOnNoAnswerEnableValue,
	setCallForwardOnNoAnswerEnableValue,
	getCallForwardOnNoAnswerEnableAttribute,
	setCallForwardOnNoAnswerEnableAttribute,
	NULL
};

static NodeFunc CallForwardOnNoAnswerNumber =
{
	NULL,
	NULL,
	getCallForwardOnNoAnswerNumberValue,
	setCallForwardOnNoAnswerNumberValue,
	getCallForwardOnNoAnswerNumberAttribute,
	setCallForwardOnNoAnswerNumberAttribute,
	NULL
};

static NodeFunc CallForwardOnNoAnswerRingCount =
{
	NULL,
	NULL,
	getCallForwardOnNoAnswerRingCountValue,
	setCallForwardOnNoAnswerRingCountValue,
	getCallForwardOnNoAnswerRingCountAttribute,
	setCallForwardOnNoAnswerRingCountAttribute,
	NULL
};

static NodeFunc CallTransferEnable =
{
	NULL,
	NULL,
	getCallTransferEnableValue,
	setCallTransferEnableValue,
	getCallTransferEnableAttribute,
	setCallTransferEnableAttribute,
	NULL
};

static NodeFunc MWIEnable =
{	
       NULL,
	NULL,
	getMWIEnableValue,
	setMWIEnableValue,
	getMWIEnableAttribute,
	setMWIEnableAttribute,
	NULL
};

static NodeFunc X_CU_HotlineEnable =
{
	NULL,
	NULL,
	getX_CU_HotlineEnableValue,
	setX_CU_HotlineEnableValue,
	getX_CU_HotlineEnableAttribute,
	setX_CU_HotlineEnableAttribute,
	NULL
};

static NodeFunc X_CU_HotlineNumber =
{
	NULL,
	NULL,
	getX_CU_HotlineNumberValue,
	setX_CU_HotlineNumberValue,
	getX_CU_HotlineNumberAttribute,
	setX_CU_HotlineNumberAttribute,
	NULL
};

static NodeFunc X_CU_HotlineTimer =
{
	NULL,
	NULL,
	getX_CU_HotlineTimerValue,
	setX_CU_HotlineTimerValue,
	getX_CU_HotlineTimerAttribute,
	setX_CU_HotlineTimerAttribute,
	NULL
};

static NodeFunc X_CU_conference_uri =
{
	NULL,
	NULL,
	getX_CU_conference_uriValue,
	setX_CU_conference_uriValue,
	getX_CU_conference_uriAttribute,
	setX_CU_conference_uriAttribute,
	NULL
};
#endif

CwmpParameterStruct CallingFeatures[] = 
{
	{"CallWaitingEnable", BooleanW, NoLimite, &CallWaitingEnable, NULL},
#if !defined(TCSUPPORT_CT_PON_SC)
       {"CallForwardUnconditionalEnable", BooleanW, NoLimite, &CallForwardUnconditionalEnable, NULL},
       {"CallForwardUnconditionalNumber", StrW, NoLimite, &CallForwardUnconditionalNumber, NULL},
       {"CallForwardOnBusyEnable", BooleanW, NoLimite, &CallForwardOnBusyEnable, NULL},
       {"CallForwardOnBusyNumber", StrW, NoLimite, &CallForwardOnBusyNumber, NULL},
       {"CallForwardOnNoAnswerEnable", BooleanW, NoLimite, &CallForwardOnNoAnswerEnable, NULL},
       {"CallForwardOnNoAnswerNumber", StrW, NoLimite, &CallForwardOnNoAnswerNumber, NULL},
       {"CallForwardOnNoAnswerRingCount", UnsignIntW, NoLimite, &CallForwardOnNoAnswerRingCount, NULL},
       {"CallTransferEnable", BooleanW, NoLimite, &CallTransferEnable, NULL},
       {"MWIEnable", BooleanW, NoLimite, &MWIEnable, NULL},
       {X_CUC_COM"HotlineEnable", BooleanW, NoLimite, &X_CU_HotlineEnable, NULL},
       {X_CUC_COM"HotlineNumber", StrW, NoLimite, &X_CU_HotlineNumber, NULL},
       {X_CUC_COM"HotlineTimer", UnsignIntW, NoLimite, &X_CU_HotlineTimer, NULL},
       {X_CUC_COM"conference-uri", StrW, NoLimite, &X_CU_conference_uri, NULL},
 #endif
	{NULL, -1, -1, NULL, NULL}
};

#if !defined(TCSUPPORT_CT_PON_SC)

static NodeFunc PhyReferenceList =
{
	NULL,
	NULL,
	getPhyReferenceListValue,
	setPhyReferenceListValue,
	getPhyReferenceListAttribute,
	setPhyReferenceListAttribute,
	NULL
};
#endif
#else
#ifdef TCSUPPORT_PON_ROSTELECOM
static NodeFunc X_RTK_3PTYFuncs =
{
	NULL,
	NULL,
	getX_RTK_3PTYValue,
	setX_RTK_3PTYValue,
	getX_RTK_3PTYAttribute,
	setX_RTK_3PTYAttribute,
	NULL
};
CwmpParameterStruct X_RTK_3PTY_IMS[] = 
{
    {"conference-uri", StrW, NoLimite, &conference_uriFuncs, NULL},
    {NULL, -1, -1, NULL, NULL}
};


CwmpParameterStruct CallingFeatures[] = 
{
 	{"X_RTK_3PTY", BooleanW, NoLimite, &X_RTK_3PTYFuncs, NULL},
	{"X_RTK_3PTY_IMS", ObjectR, NoLimite, NULL, X_RTK_3PTY_IMS},
	{NULL, -1, -1, NULL, NULL}
};
#endif
#endif

#if defined(TCSUPPORT_CUC_VOIP_NODE) || defined(TCSUPPORT_CT_PON_GD)

static NodeFunc DirectoryNumber =
{
	NULL,
	NULL,
	getDirectoryNumberValue,
	setDirectoryNumberValue,
	getDirectoryNumberAttribute,
	setDirectoryNumberAttribute,
	NULL
};
#endif

#if defined(TCSUPPORT_CT_PON_SC)
static NodeFunc SubsUA =
{
	NULL,
	NULL,
	getSubsUAValue,
	setSubsUAValue,
	getSubsUAAttribute,
	setSubsUAAttribute,
	NULL
};

static NodeFunc SubsReg =
{
	NULL,
	NULL,
	getSubsRegValue,
	setSubsRegValue,
	getSubsRegAttribute,
	setSubsRegAttribute,
	NULL
};
#endif

#if defined(TCSUPPORT_CUC_VOIP_NODE) || defined(TCSUPPORT_CT_PON_SC)
static NodeFunc CallState =
{
	NULL,
	NULL,
	getCallStateValue,
	NULL,
	getCallStateAttribute,
	setCallStateAttribute,
	NULL
};
#endif

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.Line.{i}.
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
CwmpParameterStruct Line[] = 
{
	{"Enable", StrW, NoLimite, &Enable, NULL},
	{"SIP", ObjectR, NoLimite, NULL, SIP},
	{"VoiceProcessing", ObjectR, NoLimite, NULL, VoiceProcessing},
	{"Codec", ObjectR, NoLimite, NULL, Codec},
	{"Status", StrR, NoLimite, &StatusFuncs, NULL},
#if defined(TCSUPPORT_CT)
	{X_CT_COM"PBXPrefixEnable", BooleanW, NoLimite, &PBXPrefixEnableFuncs, NULL},
	{X_CT_COM"PBXPrefix", StrW, NoLimite, &PBXPrefixFuncs, NULL},
	{X_CT_COM"PBXDialSecondaryEnable", BooleanW, NoLimite, &PBXDialSecondaryEnableFuncs, NULL},
	{X_CT_COM"LastRegisterError", ObjectR, NoLimite, NULL, LastRegisterErrorFunc},
	{X_CT_COM"IMS", ObjectR, NoLimite, NULL, COM_IMS},
#if defined(TCSUPPORT_ECN_MEGACO)
	{X_CT_COM"H248", ObjectR, NoLimite, NULL, H248},
#endif
#endif
#if defined(TCSUPPORT_CUC_VOIP_NODE) || defined(TCSUPPORT_CT_PON_SC)
	//{X_CUC_COM"IMS", ObjectR, NoLimite, NULL, COM_IMS}, //
	{"CallingFeatures", ObjectR, NoLimite, NULL, CallingFeatures},
#if !defined(TCSUPPORT_CT_PON_SC)
	{"PhyReferenceList", StrW, NoLimite, &PhyReferenceList,NULL},
#endif
	//{X_CUC_COM"H248", ObjectR, NoLimite, NULL, H248},
#else
#ifdef TCSUPPORT_PON_ROSTELECOM
    {"CallingFeatures", ObjectR, NoLimite, NULL, CallingFeatures},
#endif
#endif

#if defined(TCSUPPORT_CUC_VOIP_NODE) || defined(TCSUPPORT_CT_PON_SC)
	{"CallState", StrR, NoLimite, &CallState,NULL},
#endif
#if defined(TCSUPPORT_CUC_VOIP_NODE) || defined(TCSUPPORT_CT_PON_GD)
	{"DirectoryNumber", StrW, NoLimite, &DirectoryNumber,NULL},
#endif

	{"Stats", ObjectR, NoLimite, NULL, LineStats},	
#if defined(TCSUPPORT_CT_PON_SC)
	{"SubsUA", BooleanW, NoLimite, &SubsUA, NULL},
	{"SubsReg", BooleanW, NoLimite, &SubsReg, NULL},	
#endif

	{NULL, -1, -1, NULL, NULL}
};

