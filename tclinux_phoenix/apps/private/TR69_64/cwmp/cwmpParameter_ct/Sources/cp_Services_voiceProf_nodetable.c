
#include "Global_res.h"
#include "cp_Services_voiceProf.h"


#if defined(TCSUPPORT_CUC_VOIP_NODE)
NodeFunc LocalPortMin =
{
	NULL,
	NULL,
	getLocalPortMinValue,
	setLocalPortMinValue,
	getLocalPortMinAttribute,
	setLocalPortMinAttribute,
	NULL
};

NodeFunc LocalPortMax =
{
	NULL,
	NULL,
	getLocalPortMaxValue,
	setLocalPortMaxValue,
	getLocalPortMaxAttribute,
	setLocalPortMaxAttribute,
	NULL
};

NodeFunc TelephoneEventPayloadType =
{
	NULL,
	NULL,
	getTelephoneEventPayloadTypeValue,
	setTelephoneEventPayloadTypeValue,
	getTelephoneEventPayloadTypeAttribute,
	setTelephoneEventPayloadTypeAttribute,
	NULL

};

NodeFunc X_CU_WANName =
{
	NULL,
	NULL,
	getRTPX_CU_WANNameValue,
       NULL,
       getRTPX_CU_WANNameAttribute,
	setRTPX_CU_WANNameAttribute,
	NULL

};

NodeFunc RtcpEnable =
{
	NULL,
	NULL,
	getRtcpEnableValue,
	setRtcpEnableValue,
	getRtcpEnableAttribute,
	setRtcpEnableAttribute,
	NULL

};

NodeFunc TxRepeatInterval =
{
	NULL,
	NULL,
	getTxRepeatIntervalValue,
	setTxRepeatIntervalValue,
	getTxRepeatIntervalAttribute,
	setTxRepeatIntervalAttribute,
	NULL

};

NodeFunc X_CU_RtcpxrFlag =
{
	NULL,
	NULL,
	getX_CU_RtcpxrFlagValue,
	setX_CU_RtcpxrFlagValue,
	getX_CU_RtcpxrFlagAttribute,
	setX_CU_RtcpxrFlagAttribute,
	NULL

};

NodeFunc RdnEnable =
{
	NULL,
	NULL,
	getRdnEnableValue,
	setRdnEnableValue,
	getRdnEnableAttribute,
	setRdnEnableAttribute,
	NULL

};

NodeFunc PayloadType =
{
	NULL,
	NULL,
	getPayloadTypeValue,
	setPayloadTypeValue,
	getPayloadTypeAttribute,
	setPayloadTypeAttribute,
	NULL

};

NodeFunc X_CU_EnableFixedStart =
{
	NULL,
	NULL,
	getX_CU_EnableFixedStartValue,
	setX_CU_EnableFixedStartValue,
	getX_CU_EnableFixedStartAttribute,
	setX_CU_EnableFixedStartAttribute,
	NULL

};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.VoiceProfile.RTP.
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/

CwmpParameterStruct RTCP[] = 
{
	{"Enable", BooleanW, NoLimite, &RtcpEnable, NULL},
	{"TxRepeatInterval", UnsignIntW, NoLimite, &TxRepeatInterval, NULL},
	{"X_CU_RtcpxrFlag", BooleanW, NoLimite, &X_CU_RtcpxrFlag, NULL},
       {NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.VoiceProfile.RTP.
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/

CwmpParameterStruct Redundancy[] = 
{
	{"Enable", BooleanW, NoLimite, &RdnEnable, NULL},
	{"PayloadType", UnsignIntW, NoLimite, &PayloadType, NULL},
	{"X_CU_EnableFixedStart", BooleanW, NoLimite, &X_CU_EnableFixedStart, NULL},
       {NULL, -1, -1, NULL, NULL}
};
#endif



NodeFunc RTPVLANIDMarkFuncs =
{
	NULL,
	NULL,
	getRTPVLANIDMarkValue,
	setRTPVLANIDMarkValue,
	getRTPVLANIDMarkAttribute,
	setRTPVLANIDMarkAttribute,
	NULL
};

#if defined(TCSUPPORT_CT)
NodeFunc RTPlpMarkFuncs =
{
	NULL,
	NULL,
	getRTPlpMarkValue,
	setRTPlpMarkValue,
	getRTPlpMarkAttribute,
	setRTPlpMarkAttribute,
	NULL
};
#endif

NodeFunc RTPDSCPMarkFuncs =
{
	NULL,
	NULL,
	getRTPDSCPMarkValue,
	setRTPDSCPMarkValue,
	getRTPDSCPMarkAttribute,
	setRTPDSCPMarkAttribute,
	NULL
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.VoiceProfile.
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
CwmpParameterStruct RTP[] = 
{
	{"VLANIDMark", IntW, NoLimite, &RTPVLANIDMarkFuncs, NULL},
#if defined(TCSUPPORT_CT)
	{X_CT_COM"802-1pMark", IntW, NoLimite, &RTPlpMarkFuncs,	NULL},
#endif
	{"DSCPMark", IntW, NoLimite, &RTPDSCPMarkFuncs, NULL},
#if defined(TCSUPPORT_CUC_VOIP_NODE)
	{"LocalPortMin", UnsignIntW, NoLimite, &LocalPortMin, NULL},
	{"LocalPortMax", UnsignIntW, NoLimite, &LocalPortMax, NULL},
       {"TelephoneEventPayloadType", UnsignIntW, NoLimite, &TelephoneEventPayloadType, NULL},
       {X_CUC_COM"WANName", StrR, NoLimite, &X_CU_WANName, NULL},
       {"RTCP", ObjectR, NoLimite, NULL, RTCP},
       {"Redundancy", ObjectR, NoLimite, NULL, Redundancy},

#endif

	{NULL, -1, -1, NULL, NULL}
};

#if defined(TCSUPPORT_CT_PON_SC)
NodeFunc CallIdTimePolicy =
{
	NULL,
	NULL,
	getCallIdTimePolicyValue,
	setCallIdTimePolicyValue,
	getSIPProxyServerAttribute,
	setSIPProxyServerAttribute,
	NULL
};
#endif

NodeFunc ProxyServer =
{
	NULL,
	NULL,
	getSIPProxyServerValue,
	setSIPProxyServerValue,
	getSIPProxyServerAttribute,
	setSIPProxyServerAttribute,
	NULL
};

NodeFunc ProxyServerPort =
{
	NULL,
	NULL,
	getSIPProxyServerPortValue,
	setSIPProxyServerPortValue,
	getSIPProxyServerPortAttribute,
	setSIPProxyServerPortAttribute,
	NULL
};

NodeFunc ProxyServerTransport =
{
	NULL,
	NULL,
	getSIPProxyServerTransportValue,
	setSIPProxyServerTransportValue,
	getSIPProxyServerTransportAttribute,
	setSIPProxyServerTransportAttribute,
	NULL
};

NodeFunc RegistrarServer =
{
	NULL,
	NULL,
	getSIPRegistrarServerValue,
	setSIPRegistrarServerValue,
	getSIPRegistrarServerAttribute,
	setSIPRegistrarServerAttribute,
	NULL
};

NodeFunc RegistrarServerPort =
{
	NULL,
	NULL,
	getSIPRegistrarServerPortValue,
	setSIPRegistrarServerPortValue,
	getSIPRegistrarServerPortAttribute,
	setSIPRegistrarServerPortAttribute,
	NULL
};

NodeFunc RegistrarServerTransport =
{
	NULL,
	NULL,
	getSIPRegistrarServerTransportValue,
	setSIPRegistrarServerTransportValue,
	getSIPRegistrarServerTransportAttribute,
	setSIPRegistrarServerTransportAttribute,
	NULL
};

NodeFunc OutboundProxy =
{
	NULL,
	NULL,
	getOutboundProxyValue,
	setOutboundProxyValue,
	getOutboundProxyAttribute,
	setOutboundProxyAttribute,
	NULL
};

NodeFunc OutboundProxyPort =
{
	NULL,
	NULL,
	getOutboundProxyPortValue,
	setOutboundProxyPortValue,
	getOutboundProxyPortAttribute,
	setOutboundProxyPortAttribute,
	NULL
};

#if defined(TCSUPPORT_CT)
NodeFunc IADDiagnosticsState =
{
	NULL,
	NULL,
	getIADDiagnosticsStateValue,
	setIADDiagnosticsStateValue,
	getIADDiagnosticsStateAttribute,
	setIADDiagnosticsStateAttribute,
	NULL
};


NodeFunc TestServer =
{
	NULL,
	NULL,
	getTestServerValue,
	setTestServerValue,
	getTestServerAttribute,
	setTestServerAttribute,
	NULL
};

NodeFunc RegistResult =
{
	NULL,
	NULL,
	getRegistResultValue,
	NULL,
	getRegistResultAttribute,
	setRegistResultAttribute,
	NULL
};


NodeFunc Reason =
{
	NULL,
	NULL,
	getReasonValue,
	NULL,
	getReasonAttribute,
	setReasonAttribute,
	NULL
};

NodeFunc Standby_ProxyServer =
{
	NULL,
	NULL,
	getSIPStandby_ProxyServerValue,
	setSIPStandby_ProxyServerValue,
	getSIPStandby_ProxyServerAttribute,
	setSIPStandby_ProxyServerAttribute,
	NULL
};

NodeFunc Standby_ProxyServerPort =
{
	NULL,
	NULL,
	getSIPStandby_ProxyServerPortValue,
	setSIPStandby_ProxyServerPortValue,
	getSIPStandby_ProxyServerPortAttribute,
	setSIPStandby_ProxyServerPortAttribute,
	NULL
};

NodeFunc Standby_ProxyServerTransport =
{
	NULL,
	NULL,
	getSIPStandby_ProxyServerTransportValue,
	setSIPStandby_ProxyServerTransportValue,
	getSIPStandby_ProxyServerTransportAttribute,
	setSIPStandby_ProxyServerTransportAttribute,
	NULL
};

NodeFunc Standby_RegistrarServer =
{
	NULL,
	NULL,
	getSIPStandby_RegistrarServerValue,
	setSIPStandby_RegistrarServerValue,
	getSIPStandby_RegistrarServerAttribute,
	setSIPStandby_RegistrarServerAttribute,
	NULL
};

NodeFunc Standby_RegistrarServerPort =
{
	NULL,
	NULL,
	getSIPStandby_RegistrarServerPortValue,
	setSIPStandby_RegistrarServerPortValue,
	getSIPStandby_RegistrarServerPortAttribute,
	setSIPStandby_RegistrarServerPortAttribute,
	NULL
};

NodeFunc Standby_RegistrarServerTransport =
{
	NULL,
	NULL,
	getSIPStandby_RegistrarServerTransportValue,
	setSIPStandby_RegistrarServerTransportValue,
	getSIPStandby_RegistrarServerTransportAttribute,
	setSIPStandby_RegistrarServerTransportAttribute,
	NULL
};

NodeFunc Standby_OutboundProxy =
{
	NULL,
	NULL,
	getSIPStandby_OutboundProxyValue,
	setSIPStandby_OutboundProxyValue,
	getSIPStandby_OutboundProxyAttribute,
	setSIPStandby_OutboundProxyAttribute,
	NULL
};

NodeFunc Standby_OutboundProxyPort =
{
	NULL,
	NULL,
	getSIPStandby_OutboundProxyPortValue,
	setSIPStandby_OutboundProxyPortValue,
	getSIPStandby_OutboundProxyPortAttribute,
	setSIPStandby_OutboundProxyPortAttribute,
	NULL
};

NodeFunc lpMark =
{
	NULL,
	NULL,
	getSIPlpMarkValue,
	setSIPlpMarkValue,
	getSIPlpMarkAttribute,
	setSIPlpMarkAttribute,
	NULL
};

NodeFunc HeartbeatSwitch =
{
	NULL,
	NULL,
	getSIPHeartbeatSwitchValue,
	setSIPHeartbeatSwitchValue,
	getSIPHeartbeatSwitchAttribute,
	setSIPHeartbeatSwitchAttribute,
	NULL
};

NodeFunc HeartbearCycle =
{
	NULL,
	NULL,
	getSIPHeartbearCycleValue,
	setSIPHeartbearCycleValue,
	getSIPHeartbearCycleAttribute,
	setSIPHeartbearCycleAttribute,
	NULL
};

NodeFunc HeartbearCount =
{
	NULL,
	NULL,
	getSIPHeartbearCountValue,
	setSIPHeartbearCountValue,
	getSIPHeartbearCountAttribute,
	setSIPHeartbearCountAttribute,
	NULL
};

NodeFunc SessionUpdateTimer =
{
	NULL,
	NULL,
	getSIPSessionUpdateTimerValue,
	setSIPSessionUpdateTimerValue,
	getSIPSessionUpdateTimerAttribute,
	setSIPSessionUpdateTimerAttribute,
	NULL
};

#endif

NodeFunc SIPDSCPMark =
{
	NULL,
	NULL,
	getSIPDSCPMarkValue,
	setSIPDSCPMarkValue,
	getSIPDSCPMarkAttribute,
	setSIPDSCPMarkAttribute,
	NULL
};

NodeFunc UserAgentDomain =
{
	NULL,
	NULL,
	getSIPUserAgentDomainValue,
	setSIPUserAgentDomainValue,
	getSIPUserAgentDomainAttribute,
	setSIPUserAgentDomainAttribute,
	NULL
};

NodeFunc UserAgentPort =
{
	NULL,
	NULL,
	getSIPUserAgentPortValue,
	setSIPUserAgentPortValue,
	getSIPUserAgentPortAttribute,
	setSIPUserAgentPortAttribute,
	NULL
};

NodeFunc UserAgentTransport =
{
	NULL,
	NULL,
	getSIPUserAgentTransportValue,
	setSIPUserAgentTransportValue,
	getSIPUserAgentTransportAttribute,
	setSIPUserAgentTransportAttribute,
	NULL
};

NodeFunc SIPVLANIDMark =
{
	NULL,
	NULL,
	getSIPVLANIDMarkValue,
	setSIPVLANIDMarkValue,
	getSIPVLANIDMarkAttribute,
	setSIPVLANIDMarkAttribute,
	NULL
};

NodeFunc RegisterRetryInterval =
{
	NULL,
	NULL,
	getSIPRegisterRetryIntervalValue,
	setSIPRegisterRetryIntervalValue,
	getSIPRegisterRetryIntervalAttribute,
	setSIPRegisterRetryIntervalAttribute,
	NULL
};

NodeFunc RegisterExpires =
{
	NULL,
	NULL,
	getSIPRegisterExpiresValue,
	setSIPRegisterExpiresValue,
	getSIPRegisterExpiresAttribute,
	setSIPRegisterExpiresAttribute,
	NULL
};

#if defined(TCSUPPORT_VOIP_IMSSIP)//xflu_20140228
NodeFunc ImplicitRegistrationEnable =
{
	NULL,
	NULL,
	getSIPImplicitRegistrationEnableValue,
	setSIPImplicitRegistrationEnableValue,
	getSIPImplicitRegistrationEnableAttribute,
	setSIPImplicitRegistrationEnableAttribute,
	NULL
};
#endif
#if defined(TCSUPPORT_CUC_VOIP_NODE)
NodeFunc TimerT1 =
{
       NULL,
	NULL,
	getTimerT1Value,
	setTimerT1Value,
	getTimerT1Attribute,
	setTimerT1Attribute,
	NULL
};
NodeFunc TimerT2 =
{
       NULL,
	NULL,
	getTimerT2Value,
	setTimerT2Value,
	getTimerT2Attribute,
	setTimerT2Attribute,
	NULL
};
NodeFunc TimerT4 =
{
       NULL,
	NULL,
	getTimerT4Value,
	setTimerT4Value,
	getTimerT4Attribute,
	setTimerT4Attribute,
	NULL
};
NodeFunc UseCodecPriorityInSDPResponse =
{
       NULL,
	NULL,
	getUseCodecPriorityInSDPResponseValue,
	setUseCodecPriorityInSDPResponseValue,
	getUseCodecPriorityInSDPResponseAttribute,
	setUseCodecPriorityInSDPResponseAttribute,
	NULL
};

NodeFunc SIPResponseNumber =
{
       NULL,
	NULL,
	getSIPResponseNumberValue,
	setSIPResponseNumberValue,
	getSIPResponseNumberAttribute,
	setSIPResponseNumberAttribute,
	NULL
};

NodeFunc Tone =
{
       NULL,
	NULL,
	getToneValue,
	setToneValue,
	getToneAttribute,
	setToneAttribute,
	NULL
};

NodeFunc X_CU_Duration =
{
       NULL,
	NULL,
	getX_CU_DurationValue,
	setX_CU_DurationValue,
	getX_CU_DurationAttribute,
	setX_CU_DurationAttribute,
	NULL
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.SIP.ResponseMap.{i}
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
CwmpParameterStruct  ResponseMap[]=
{
	{"SIPResponseNumber",	UnsignIntW,	NoLimite, &SIPResponseNumber,	NULL},
	{"Tone", UnsignIntW,	NoLimite, &Tone, NULL},
	{"X_CU_Duration",	UnsignIntW, NoLimite,	&X_CU_Duration, NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct  ResponseMapIndex[]= 
{
	{NULL, ObjectR, NoLimite, NULL,  ResponseMap},
	{NULL, -1, -1, NULL, NULL}
};

NodeFunc ResponseMapFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitResponseMapFuncsSubTree
};

#endif


/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.VoiceProfile.
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
CwmpParameterStruct SIPTab[] = 
{
#if defined(TCSUPPORT_CT_PON_SC)
	{"CallIdTimePolicy", BooleanW, NoLimite, &CallIdTimePolicy, NULL},
#endif
	{"ProxyServer", StrW, NoLimite, &ProxyServer, NULL},
	{"ProxyServerPort", UnsignIntW, NoLimite, &ProxyServerPort, NULL},
	{"ProxyServerTransport", StrW, NoLimite, &ProxyServerTransport, NULL},
	{"RegistrarServer", StrW, NoLimite, &RegistrarServer, NULL},
	{"RegistrarServerPort", UnsignIntW, NoLimite, &RegistrarServerPort, NULL},
	{"RegistrarServerTransport", StrW, NoLimite, &RegistrarServerTransport, NULL},
	{"OutboundProxy", StrW, NoLimite, &OutboundProxy, NULL},
	{"OutboundProxyPort", UnsignIntW, NoLimite, &OutboundProxyPort, NULL},
#if defined(TCSUPPORT_CT)
	{X_CT_COM"Standby-ProxyServer", StrW, NoLimite, &Standby_ProxyServer, NULL},
	{X_CT_COM"Standby-ProxyServerPort", UnsignIntW, NoLimite, &Standby_ProxyServerPort, NULL},
	{X_CT_COM"Standby-ProxyServerTransport", StrW, NoLimite, &Standby_ProxyServerTransport, NULL},
	{X_CT_COM"Standby-RegistrarServer", StrW, NoLimite, &Standby_RegistrarServer, NULL},
	{X_CT_COM"Standby-RegistrarServerPort", UnsignIntW, NoLimite, &Standby_RegistrarServerPort, NULL},
	{X_CT_COM"Standby-RegistrarServerTransport", StrW, NoLimite, &Standby_RegistrarServerTransport, NULL},
	{X_CT_COM"Standby-OutboundProxy", StrW, NoLimite, &Standby_OutboundProxy, NULL},
	{X_CT_COM"Standby-OutboundProxyPort", UnsignIntW, NoLimite, &Standby_OutboundProxyPort, NULL},
	{X_CT_COM"802-1pMark", IntW, NoLimite, &lpMark, NULL},
	
#if defined(TCSUPPORT_CUC_VOIP_NODE)
	{X_CUC_COM"HeartbeatSwitch", IntW, NoLimite, &HeartbeatSwitch, NULL},
#else	
	{X_CT_COM"HeartbeatSwitch", BooleanW, NoLimite, &HeartbeatSwitch, NULL},
#endif

	{X_CT_COM"HeartbeatCycle", IntW, NoLimite, &HeartbearCycle, NULL},
	{X_CT_COM"HeartbeatCount", IntW, NoLimite, &HeartbearCount, NULL},
	{X_CT_COM"SessionUpdateTimer", IntW, NoLimite, &SessionUpdateTimer, NULL},
#endif
	{"DSCPMark", UnsignIntW, NoLimite, &SIPDSCPMark, NULL},
	{"UserAgentDomain", StrW, NoLimite, &UserAgentDomain, NULL},
	{"UserAgentPort", UnsignIntW, NoLimite, &UserAgentPort, NULL},
	{"UserAgentTransport", StrW, NoLimite, &UserAgentTransport, NULL},
	{"VLANIDMark", IntW, NoLimite, &SIPVLANIDMark, NULL},
	{"RegisterRetryInterval", IntW, NoLimite, &RegisterRetryInterval, NULL},
#if !defined(TCSUPPORT_CUC_VOIP_NODE)
	{"RegisterExpires", IntW, NoLimite, &RegisterExpires, NULL},
#endif
#if defined(TCSUPPORT_VOIP_IMSSIP)//xflu_20140228
	{"ImplicitRegistrationEnable", BooleanW, NoLimite, &ImplicitRegistrationEnable, NULL},
#endif
#if defined(TCSUPPORT_CUC_VOIP_NODE)
	{X_CUC_COM"SecondaryProxyServer", StrW, NoLimite, &Standby_ProxyServer, NULL},
	{X_CUC_COM"SecondaryProxyServerPort", UnsignIntW, NoLimite, &Standby_ProxyServerPort, NULL},
	{X_CUC_COM"SecondaryProxyServerTransport", StrW, NoLimite, &Standby_ProxyServerTransport, NULL},
	{X_CUC_COM"SecondaryRegistrarServer", StrW, NoLimite, &Standby_RegistrarServer, NULL},
	{X_CUC_COM"SecondaryRegistrarServerPort", UnsignIntW, NoLimite, &Standby_RegistrarServerPort, NULL},
	{X_CUC_COM"SecondaryRegistrarServerTransport", StrW, NoLimite, &Standby_RegistrarServerTransport, NULL},
	{X_CUC_COM"SecondaryOutboundProxyServer", StrW, NoLimite, &Standby_OutboundProxy, NULL},
	{X_CUC_COM"SecondaryOutboundProxyServerPort", UnsignIntW, NoLimite, &Standby_OutboundProxyPort, NULL},
	{"RegistrationPeriod", IntW, NoLimite, &RegisterExpires, NULL},
       {"TimerT1", UnsignIntW, NoLimite, &TimerT1, NULL},
       {"TimerT2", UnsignIntW, NoLimite, &TimerT2, NULL},
       {"TimerT4", UnsignIntW, NoLimite, &TimerT4, NULL},
       {"UseCodecPriorityInSDPResponse", BooleanW, NoLimite, &UseCodecPriorityInSDPResponse, NULL},
       {"ResponseMap", ObjectR, NoLimite, &ResponseMapFuncs, NULL},
#endif


	{NULL, -1, -1, NULL, NULL}
};

#if defined(TCSUPPORT_CT)
CwmpParameterStruct IADDiagnostics[] = 
{
	{"IADDiagnosticsState", StrW, NoLimite, &IADDiagnosticsState, NULL},
	{"TestServer", UnsignIntW, NoLimite, &TestServer, NULL},
	{"RegistResult", UnsignIntR, NoLimite, &RegistResult, NULL},
	{"Reason", UnsignIntR, NoLimite, &Reason, NULL},
	{NULL, -1, -1, NULL, NULL}
};
#endif

static NodeFunc Enable =
{
	NULL,
	NULL,
	getProfileFaxT38Value,
	setProfileFaxT38Value,
	getProfileFaxT38Attribute,
	setProfileFaxT38Attribute,
	NULL
};

#if defined(TCSUPPORT_CUC_VOIP_NODE)
NodeFunc BitRateFaxT38 =
{
	NULL,
	NULL,
	getBitRateFaxT38Value,
	setBitRateFaxT38Value,
	getBitRateFaxT38Attribute,
	setBitRateFaxT38Attribute,
	NULL

};

NodeFunc TCFMethod =
{
	NULL,
	NULL,
	getTCFMethodValue,
	setTCFMethodValue,
	getTCFMethodAttribute,
	setTCFMethodAttribute,
	NULL

};

NodeFunc HighSpeedRedundancy =
{
	NULL,
	NULL,
	getHighSpeedRedundancyValue,
	setHighSpeedRedundancyValue,
	getHighSpeedRedundancyAttribute,
	setHighSpeedRedundancyAttribute,
	NULL

};

NodeFunc LowSpeedRedundancy =
{
	NULL,
	NULL,
	getLowSpeedRedundancyValue,
	setLowSpeedRedundancyValue,
	getLowSpeedRedundancyAttribute,
	setLowSpeedRedundancyAttribute,
	NULL

};

NodeFunc X_CU_PortAdd2 =
{
	NULL,
	NULL,
	getX_CU_PortAdd2Value,
	setX_CU_PortAdd2Value,
	getX_CU_PortAdd2Attribute,
	setX_CU_PortAdd2Attribute,
	NULL

};
#endif


/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.FaxT38.
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
CwmpParameterStruct FaxT38_Profile[] = 
{
	{"Enable", BooleanW, NoLimite, &Enable, NULL},
#if defined(TCSUPPORT_CUC_VOIP_NODE)
       {"BitRate", UnsignIntW, NoLimite, &BitRateFaxT38, NULL},
       {"TCFMethod", StrW, NoLimite, &TCFMethod, NULL},
       {"HighSpeedRedundancy", UnsignIntW, NoLimite, &HighSpeedRedundancy, NULL},
       {"LowSpeedRedundancy", UnsignIntW, NoLimite, &LowSpeedRedundancy, NULL},
       {"X_CU_PortAdd2", BooleanW, NoLimite, &X_CU_PortAdd2, NULL},
#endif

	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.VoiceProfile.Line.
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
CwmpParameterStruct LineIndex[] = 
{
	{NULL, ObjectR, NoLimite, NULL, Line},
	{NULL, -1, -1, NULL, NULL}
};

NodeFunc LineFuncs =
{
	addLineObject,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	cwmpInitLineSubTree
};

#if defined(TCSUPPORT_CT_PON_SC)
static NodeFunc RingVoltage =
{
	NULL,
	NULL,
	getRingVoltageValue,
	setRingVoltageValue,
	getDigitMapEnableAttribute,
	setDigitMapEnableAttribute,
	NULL
};

static NodeFunc CallIDMsgType =
{
	NULL,
	NULL,
	getCallIDMsgTypeValue,
	setCallIDMsgTypeValue,
	getDigitMapEnableAttribute,
	setDigitMapEnableAttribute,
	NULL
};
#endif

static NodeFunc DigitMapEnableFuncs =
{
	NULL,
	NULL,
	getDigitMapEnableValue,
	setDigitMapEnableValue,
	getDigitMapEnableAttribute,
	setDigitMapEnableAttribute,
	NULL
};

static NodeFunc DigitMapFuncs =
{
	NULL,
	NULL,
	getDigitMapValue,
	setDigitMapValue,
	getDigitMapAttribute,
	setDigitMapAttribute,
	NULL
};
#if defined(TCSUPPORT_CT_PON_GS)
static NodeFunc DigitMapMatchModeFuncs =
{
	NULL,
	NULL,
	getDigitMapMatchModeValue,
	setDigitMapMatchModeValue,
	getDigitMapMatchModeAttribute,
	setDigitMapMatchModeAttribute,
	NULL
};
#endif

#if defined(TCSUPPORT_CT)
static NodeFunc DigitMapSpecialEnableFuncs =
{
	NULL,
	NULL,
	getDigitMapSpecialEnableValue,
	setDigitMapSpecialEnableValue,
	getDigitMapSpecialEnableAttribute,
	setDigitMapSpecialEnableAttribute,
	NULL
};

static NodeFunc DigitMapSpecialFuncs =
{
	NULL,
	NULL,
	getDigitMapSpecialValue,
	setDigitMapSpecialValue,
	getDigitMapSpecialAttribute,
	setDigitMapSpecialAttribute,
	NULL
};


static NodeFunc ServerTypeFuncs =
{
	NULL,
	NULL,
	getServerTypeValue,
	setServerTypeValue,
	getServerTypeAttribute,
	setServerTypeAttribute,
	NULL
};

static NodeFunc H248DMNameFuncs =
{
	NULL,
	NULL,
	getH248DMNameFuncsValue,
	setH248DMNameFuncsValue,
	getH248DMNameFuncsAttribute,
	setH248DMNameFuncsAttribute,
	NULL
};


static NodeFunc H248DigitMapFuncs =
{
	NULL,
	NULL,
	getH248DigitMapFuncsValue,
	setH248DigitMapFuncsValue,
	getH248DigitMapFuncsAttribute,
	setH248DigitMapFuncsAttribute,
	NULL
};

static NodeFunc StartDigitTimerFuncs =
{
	NULL,
	NULL,
	getStartDigitTimerValue,
	setStartDigitTimerValue,
	getStartDigitTimerAttribute,
	setStartDigitTimerAttribute,
	NULL
};

static NodeFunc InterDigitTimerShortFuncs =
{
	NULL,
	NULL,
	getInterDigitTimerShortValue,
	setInterDigitTimerShortValue,
	getInterDigitTimerShortAttribute,
	setInterDigitTimerShortAttribute,
	NULL
};

static NodeFunc InterDigitTimerLongFuncs =
{
	NULL,
	NULL,
	getInterDigitTimerLongValue,
	setInterDigitTimerLongValue,
	getInterDigitTimerLongAttribute,
	setInterDigitTimerLongAttribute,
	NULL
};

static NodeFunc HangingReminderToneTimerFuncs =
{
	NULL,
	NULL,
	getHangingReminderToneTimerValue,
	setHangingReminderToneTimerValue,
	getHangingReminderToneTimerAttribute,
	setHangingReminderToneTimerAttribute,
	NULL
};

static NodeFunc BusyToneTimerFuncs =
{
	NULL,
	NULL,
	getBusyToneTimerValue,
	setBusyToneTimerValue,
	getBusyToneTimerAttribute,
	setBusyToneTimerAttribute,
	NULL
};

static NodeFunc NoAnswerTimerFuncs =
{
	NULL,
	NULL,
	getNoAnswerTimerValue,
	setNoAnswerTimerValue,
	getNoAnswerTimerAttribute,
	setNoAnswerTimerAttribute,
	NULL
};

static NodeFunc MediaGatewayControlerFuncs =
{
	NULL,
	NULL,
	getMediaGatewayControlerValue,
	setMediaGatewayControlerValue,
	getMediaGatewayControlerAttribute,
	setMediaGatewayControlerAttribute,
	NULL
};

static NodeFunc MediaGatewayControlerPortFunc =
{
	NULL,
	NULL,
	getMediaGatewayControlerPortValue,
	setMediaGatewayControlerPortValue,
	getMediaGatewayControlerPortAttribute,
	setMediaGatewayControlerPortAttribute,
	NULL
};

static NodeFunc Standby_MediaGatewayControlerFunc =
{
	NULL,
	NULL,
	getStandby_MediaGatewayControlerValue,
	setStandby_MediaGatewayControlerValue,
	getStandby_MediaGatewayControlerAttribute,
	setStandby_MediaGatewayControlerAttribute,
	NULL
};

static NodeFunc Standby_MediaGatewayControlerPortFunc =
{
	NULL,
	NULL,
	getStandby_MediaGatewayControlerPortValue,
	setStandby_MediaGatewayControlerPortValue,
	getStandby_MediaGatewayControlerPortAttribute,
	setStandby_MediaGatewayControlerPortAttribute,
	NULL
};

static NodeFunc MediaGatewayPortFunc =
{
	NULL,
	NULL,
	getMediaGatewayPortValue,
	setMediaGatewayPortValue,
	getMediaGatewayPortAttribute,
	setMediaGatewayPortAttribute,
	NULL
};

static NodeFunc MessageEncodingTypeFunc =
{
	NULL,
	NULL,
	getMessageEncodingTypeValue,
	setMessageEncodingTypeValue,
	getMessageEncodingTypeAttribute,
	setMessageEncodingTypeAttribute,
	NULL
};

static NodeFunc DeviceIDFunc =
{
	NULL,
	NULL,
	getDeviceIDValue,
	setDeviceIDValue,
	getDeviceIDAttribute,
	setDeviceIDAttribute,
	NULL
};

static NodeFunc DeviceIDTypeFunc =
{
	NULL,
	NULL,
	getDeviceIDTypeValue,
	setDeviceIDTypeValue,
	getDeviceIDTypeAttribute,
	setDeviceIDTypeAttribute,
	NULL
};

static NodeFunc PhysicalTermIDFunc =
{
	NULL,
	NULL,
	getPhysicalTermIDValue,
	setPhysicalTermIDValue,
	getPhysicalTermIDAttribute,
	setPhysicalTermIDAttribute,
	NULL
};

static NodeFunc PhysicalTermIDConfigMethodFunc =
{
	NULL,
	NULL,
	getPhysicalTermIDConfigMethodValue,
	setPhysicalTermIDConfigMethodValue,
	getPhysicalTermIDConfigMethodAttribute,
	setPhysicalTermIDConfigMethodAttribute,
	NULL
};

static NodeFunc PhysicalTermIDPrefixFunc =
{
	NULL,
	NULL,
	getPhysicalTermIDPrefixValue,
	setPhysicalTermIDPrefixValue,
	getPhysicalTermIDPrefixAttribute,
	setPhysicalTermIDPrefixAttribute,
	NULL
};

static NodeFunc PhysicalTermIDAddLenFunc =
{
	NULL,
	NULL,
	getPhysicalTermIDAddLenValue,
	setPhysicalTermIDAddLenValue,
	getPhysicalTermIDAddLenAttribute,
	setPhysicalTermIDAddLenAttribute,
	NULL
};

static NodeFunc RTPPrefixFunc =
{
	NULL,
	NULL,
	getRTPPrefixValue,
	setRTPPrefixValue,
	getRTPPrefixAttribute,
	setRTPPrefixAttribute,
	NULL
};

static NodeFunc EphemeralTermIDAddLenFunc =
{
	NULL,
	NULL,
	getEphemeralTermIDAddLenValue,
	setEphemeralTermIDAddLenValue,
	getEphemeralTermIDAddLenAttribute,
	setEphemeralTermIDAddLenAttribute,
	NULL
};

static NodeFunc EphemeralTermIDUniformFunc =
{
	NULL,
	NULL,
	getEphemeralTermIDUniformValue,
	setEphemeralTermIDUniformValue,
	getEphemeralTermIDUniformAttribute,
	setEphemeralTermIDUniformAttribute,
	NULL
};

static NodeFunc EphemeralTermIDStartFunc =
{
	NULL,
	NULL,
	getEphemeralTermIDStartValue,
	setEphemeralTermIDStartValue,
	getEphemeralTermIDStartAttribute,
	setEphemeralTermIDStartAttribute,
	NULL
};

static NodeFunc ThreeHandShakeFunc =
{
	NULL,
	NULL,
	getThreeHandShakeValue,
	setThreeHandShakeValue,
	getThreeHandShakeAttribute,
	setThreeHandShakeAttribute,
	NULL
};

static NodeFunc LongTimerFunc =
{
	NULL,
	NULL,
	getLongTimerValue,
	setLongTimerValue,
	getLongTimerAttribute,
	setLongTimerAttribute,
	NULL
};

static NodeFunc PendingTimerInitFunc =
{
	NULL,
	NULL,
	getPendingTimerInitValue,
	setPendingTimerInitValue,
	getPendingTimerInitAttribute,
	setPendingTimerInitAttribute,
	NULL
};

static NodeFunc RetranIntervalTimerFunc =
{
	NULL,
	NULL,
	getRetranIntervalTimerValue,
	setRetranIntervalTimerValue,
	getRetranIntervalTimerAttribute,
	setRetranIntervalTimerAttribute,
	NULL
};

static NodeFunc MaxRetranCountFunc =
{
	NULL,
	NULL,
	getMaxRetranCountValue,
	setMaxRetranCountValue,
	getMaxRetranCountAttribute,
	setMaxRetranCountAttribute,
	NULL
};

static NodeFunc RetransmissionTimeFunc =
{
	NULL,
	NULL,
	getRetransmissionTimeValue,
	setRetransmissionTimeValue,
	getRetransmissionTimeAttribute,
	setRetransmissionTimeAttribute,
	NULL
};

static NodeFunc RetransmissionCycleFunc =
{
	NULL,
	NULL,
	getRetransmissionCycleValue,
	setRetransmissionCycleValue,
	getRetransmissionCycleAttribute,
	setRetransmissionCycleAttribute,
	NULL
};

static NodeFunc HeartbeatModeFunc =
{
	NULL,
	NULL,
	getHeartbeatModeValue,
	setHeartbeatModeValue,
	getHeartbeatModeAttribute,
	setHeartbeatModeAttribute,
	NULL
};

static NodeFunc HeartbeatCycleFunc =
{
	NULL,
	NULL,
	getHeartbeatCycleValue,
	setHeartbeatCycleValue,
	getHeartbeatCycleAttribute,
	setHeartbeatCycleAttribute,
	NULL
};

static NodeFunc HeartbeatCountFunc =
{
	NULL,
	NULL,
	getHeartbeatCountValue,
	setHeartbeatCountValue,
	getHeartbeatCountAttribute,
	setHeartbeatCountAttribute,
	NULL
};

static NodeFunc RegisterCycleFunc =
{
	NULL,
	NULL,
	getRegisterCycleValue,
	setRegisterCycleValue,
	getRegisterCycleAttribute,
	setRegisterCycleAttribute,
	NULL
};

static NodeFunc MandatoryRegisterFunc =
{
	NULL,
	NULL,
	getMandatoryRegisterValue,
	setMandatoryRegisterValue,
	getMandatoryRegisterAttribute,
	setMandatoryRegisterAttribute,
	NULL
};

static NodeFunc AuthenticationMethIDFunc =
{
	NULL,
	NULL,
	getAuthenticationMethIDValue,
	setAuthenticationMethIDValue,
	getAuthenticationMethIDAttribute,
	setAuthenticationMethIDAttribute,
	NULL
};

static NodeFunc COM_NumberPlanFuncs =
{
	NULL,
	NULL,
	getCOM_NumberPlanValue,
	setCOM_NumberPlanValue,
	getCOM_NumberPlanAttribute,
	setCOM_NumberPlanAttribute,
	NULL
};

static NodeFunc G711FAXEnableFuncs =
{
	NULL,
	NULL,
	getG711FAXEnableValue,
	setG711FAXEnableValue,
	getG711FAXEnableAttribute,
	setG711FAXEnableAttribute,
	NULL
};

static NodeFunc ControlTypeFuncs =
{
	NULL,
	NULL,
	getControlTypeValue,
	setControlTypeValue,
	getControlTypeAttribute,
	setControlTypeAttribute,
	NULL
};
#endif

#if defined(TCSUPPORT_CT_PON_GS)
static NodeFunc ProfileBodyFunc =
{
	NULL,
	NULL,
	getProfileBodyValue,
	setProfileBodyValue,
	getControlTypeAttribute,
	setControlTypeAttribute,
	NULL
};
#endif

#if CHINA_UNICOM_ENABLE
/* Add for China Unicom Begin. */
static NodeFunc DeviceNameFunc =
{
	NULL,
	NULL,
	getDeviceNameValue,
	setDeviceNameValue,
	getDeviceNameAttribute,
	setDeviceNameAttribute,
	NULL
};

static NodeFunc MIDFormatFunc =
{
	NULL,
	NULL,
	getMIDFormatValue,
	setMIDFormatValue,
	getMIDFormatAttribute,
	setMIDFormatAttribute,
	NULL
};

static NodeFunc DSCPMarkFunc =
{
	NULL,
	NULL,
	getDSCPMarkValue,
	setDSCPMarkValue,
	getDSCPMarkAttribute,
	setDSCPMarkAttribute,
	NULL
};

static NodeFunc SoftswitchVendorFunc =
{
   NULL,
   	NULL,
   	getSoftswitchVendorValue,
   	setSoftswitchVendorValue,
   	getSoftswitchVendorAttribute,
   	setSoftswitchVendorAttribute,
   	NULL
};

static NodeFunc SoftswitchVersionFunc =
{
   NULL,
   	NULL,
   	getSoftswitchVersionValue,
   	setSoftswitchVersionValue,
   	getSoftswitchVersionAttribute,
   	setSoftswitchVersionAttribute,
   	NULL
};



static NodeFunc authFunc =
{
	NULL,
	NULL,
	getauthValue,
	setauthValue,
	getauthAttribute,
	setauthAttribute,
	NULL
};

static NodeFunc authInitKeyFunc =
{
	NULL,
	NULL,
	getauthInitKeyValue,
	setauthInitKeyValue,
	getauthInitKeyAttribute,
	setauthInitKeyAttribute,
	NULL
};

static NodeFunc authmgidFunc =
{
	NULL,
	NULL,
	getauthmgidValue,
	setauthmgidValue,
	getauthmgidAttribute,
	setauthmgidAttribute,
	NULL
};

static NodeFunc authHeaderFunc =
{
	NULL,
	NULL,
	getauthHeaderValue,
	setauthHeaderValue,
	getauthHeaderAttribute,
	setauthHeaderAttribute,
	NULL
};

static NodeFunc HeaderSecurityTypeFunc =
{
	NULL,
	NULL,
	getHeaderSecurityTypeValue,
	setHeaderSecurityTypeValue,
	getHeaderSecurityTypeAttribute,
	setHeaderSecurityTypeAttribute,
	NULL
};

static NodeFunc Rfc2833EncryptKeyFunc =
{
	NULL,
	NULL,
	getRfc2833EncryptKeyValue,
	setRfc2833EncryptKeyValue,
	getRfc2833EncryptKeyAttribute,
	setRfc2833EncryptKeyAttribute,
	NULL
};

static NodeFunc MsgFormatFunc =
{
	NULL,
	NULL,
	getMsgFormatValue,
	setMsgFormatValue,
	getMsgFormatAttribute,
	setMsgFormatAttribute,
	NULL
};

static NodeFunc UdpAlfFlagFunc =
{
	NULL,
	NULL,
	getUdpAlfFlagValue,
	setUdpAlfFlagValue,
	getUdpAlfFlagAttribute,
	setUdpAlfFlagAttribute,
	NULL
};

static NodeFunc ResponseAckCtrlFunc =
{
	NULL,
	NULL,
	getResponseAckCtrlValue,
	setResponseAckCtrlValue,
	getResponseAckCtrlAttribute,
	setResponseAckCtrlAttribute,
	NULL
};

static NodeFunc MaxreTransTimeFunc =
{
	NULL,
	NULL,
	getMaxreTransTimeValue,
	setMaxreTransTimeValue,
	getMaxreTransTimeAttribute,
	setMaxreTransTimeAttribute,
	NULL
};

static NodeFunc MinreTransTimeFunc =
{
	NULL,
	NULL,
	getMinreTransTimeValue,
	setMinreTransTimeValue,
	getMinreTransTimeAttribute,
	setMinreTransTimeAttribute,
	NULL
};

static NodeFunc MGProvisionalRespTimeFunc =
{
	NULL,
	NULL,
	getMGProvisionalRespTimeValue,
	setMGProvisionalRespTimeValue,
	getMGProvisionalRespTimeAttribute,
	setMGProvisionalRespTimeAttribute,
	NULL
};

static NodeFunc MTUValueFunc =
{
	NULL,
	NULL,
	getMTUValueValue,
	setMTUValueValue,
	getMTUValueAttribute,
	setMTUValueAttribute,
	NULL
};

static NodeFunc MsgSegmentationFunc =
{
	NULL,
	NULL,
	getMsgSegmentationValue,
	setMsgSegmentationValue,
	getMsgSegmentationAttribute,
	setMsgSegmentationAttribute,
	NULL
};

static NodeFunc HeartBeatRetransTimerFunc =
{
	NULL,
	NULL,
	getHeartBeatRetransTimerValue,
	setHeartBeatRetransTimerValue,
	getHeartBeatRetransTimerAttribute,
	setHeartBeatRetransTimerAttribute,
	NULL
};

static NodeFunc MWDBaseTimeFunc =
{
	NULL,
	NULL,
	getMWDBaseTimeValue,
	setMWDBaseTimeValue,
	getMWDBaseTimeAttribute,
	setMWDBaseTimeAttribute,
	NULL
};

static NodeFunc MWDFunc =
{
	NULL,
	NULL,
	getMWDValue,
	setMWDValue,
	getMWDAttribute,
	setMWDAttribute,
	NULL
};

static NodeFunc MgcSwitchMethodFunc =
{
	NULL,
	NULL,
	getMgcSwitchMethodValue,
	setMgcSwitchMethodValue,
	getMgcSwitchMethodAttribute,
	setMgcSwitchMethodAttribute,
	NULL
};

/* Add for China Unicom end. */

#endif
#if defined(TCSUPPORT_CUC_VOIP_NODE)
static NodeFunc EnableFuncs = 
{
       NULL,
	NULL,
	getEnableVoipValue, 
	setEnableVoipValue,
	getEnableVoipAttribute, 
	setEnableVoipAttribute, 
	NULL
};

static NodeFunc ResetFuncs = 
{
       NULL,
	NULL,
	getResetValue, 
	setResetValue, 
	getResetAttribute, 
	setResetAttribute, 
	NULL,
};

static NodeFunc NumberOfLinesFuncs = 
{
       NULL,
	NULL,
	getNumberOfLinesValue, 
	NULL, 
	getNumberOfLinesAttribute, 
	setNumberOfLinesAttribute, 
	NULL,

};

static NodeFunc NameFuncs = 
{
	NULL,
	NULL,
	getNameValue,
	setNameValue,
	getNameAttribute,
	setNameAttribute,
	NULL
};

static NodeFunc SignalingProtocolFuncs = 
{
	NULL,
	NULL,
	getSignalingProtocolValue,
	setSignalingProtocolValue,
	getSignalingProtocolAttribute,
	setSignalingProtocolAttribute,
	NULL
};

static NodeFunc MaxSessionsFuncs = 
{
	NULL,
	NULL,
	getMaxSessionsValue,
	setMaxSessionsValue,
	getMaxSessionsAttribute,
	setMaxSessionsAttribute,
	NULL

};

static NodeFunc DTMFMethodFuncs = 
{
	NULL,
	NULL,
	getDTMFMethodValue,
	setDTMFMethodValue,
	getDTMFMethodAttribute,
	setDTMFMethodAttribute,
	NULL

};

static NodeFunc DTMFMethodG711Funcs = 
{
	NULL,
	NULL,
	getDTMFMethodG711Value,
	setDTMFMethodG711Value,
	getDTMFMethodG711Attribute,
	setDTMFMethodG711Attribute,
	NULL

};

static NodeFunc X_CU_DigitMapMatchModeFuncs = 
{
	NULL,
	NULL,
	getX_CU_DigitMapMatchModeValue,
	setX_CU_DigitMapMatchModeValue,
	getX_CU_DigitMapMatchModeAttribute,
	setX_CU_DigitMapMatchModeAttribute,
	NULL

};

static NodeFunc X_CU_WANNameFuncs = 
{
       NULL,
	NULL,
	getX_CU_WANNameValue, 
	NULL, 
	getX_CU_WANNameAttribute, 
	setX_CU_WANNameAttribute, 
	NULL

};

static NodeFunc X_CU_InterfaceStateFuncs = 
{
       NULL,
	NULL,
	getX_CU_InterfaceStateValue, 
	NULL, 
	getX_CU_InterfaceStateAttribute, 
	setX_CU_InterfaceStateAttribute, 
	NULL

};
static NodeFunc ServerType_CUFuncs =
{
	NULL,
	NULL,
	getServerType_CUValue,
	setServerType_CUValue,
	getServerType_CUAttribute,
	setServerType_CUAttribute,
	NULL
};

static NodeFunc StartDigitTimer_CUFuncs =
{
	NULL,
	NULL,
	getStartDigitTimer_CUValue,
	setStartDigitTimer_CUValue,
	getStartDigitTimer_CUAttribute,
	setStartDigitTimer_CUAttribute,
	NULL
};

static NodeFunc InterDigitTimerShort_CUFuncs =
{
	NULL,
	NULL,
	getInterDigitTimerShort_CUValue,
	setInterDigitTimerShort_CUValue,
	getInterDigitTimerShort_CUAttribute,
	setInterDigitTimerShort_CUAttribute,
	NULL
};

static NodeFunc InterDigitTimerLong_CUFuncs =
{
	NULL,
	NULL,
	getInterDigitTimerLong_CUValue,
	setInterDigitTimerLong_CUValue,
	getInterDigitTimerLong_CUAttribute,
	setInterDigitTimerLong_CUAttribute,
	NULL
};

static NodeFunc HangingReminderToneTimer_CUFuncs =
{
	NULL,
	NULL,
	getHangingReminderToneTimer_CUValue,
	setHangingReminderToneTimer_CUValue,
	getHangingReminderToneTimer_CUAttribute,
	setHangingReminderToneTimer_CUAttribute,
	NULL
};

static NodeFunc BusyToneTimer_CUFuncs =
{
	NULL,
	NULL,
	getBusyToneTimer_CUValue,
	setBusyToneTimer_CUValue,
	getBusyToneTimer_CUAttribute,
	setBusyToneTimer_CUAttribute,
	NULL
};

static NodeFunc NoAnswerTimer_CUFuncs =
{
	NULL,
	NULL,
	getNoAnswerTimer_CUValue,
	setNoAnswerTimer_CUValue,
	getNoAnswerTimer_CUAttribute,
	setNoAnswerTimer_CUAttribute,
	NULL
};
#endif


#if defined(TCSUPPORT_CT)
/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.X_CT_COM_H248.
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
CwmpParameterStruct COM_H248[] = 
{
	{"MediaGatewayControler", StrW, NoLimite, &MediaGatewayControlerFuncs, NULL},
	{"MediaGatewayControlerPort", IntW, NoLimite, &MediaGatewayControlerPortFunc, NULL},
	{"Standby-MediaGatewayControler", StrW, NoLimite, &Standby_MediaGatewayControlerFunc, NULL},
	{"Standby-MediaGatewayControlerPort", IntW, NoLimite, &Standby_MediaGatewayControlerPortFunc, NULL},
	{"MediaGatewayPort", IntW, NoLimite, &MediaGatewayPortFunc, NULL},
	{"MessageEncodingType", StrW, NoLimite, &MessageEncodingTypeFunc, NULL},
	{"DeviceID", StrW, NoLimite, &DeviceIDFunc, NULL},
	{"DeviceIDType", UnsignIntW, NoLimite, &DeviceIDTypeFunc, NULL},
	{"PhysicalTermID", StrW, NoLimite, &PhysicalTermIDFunc, NULL},
	{"PhysicalTermIDConfigMethod", UnsignIntW, NoLimite, &PhysicalTermIDConfigMethodFunc, NULL},
	{"PhysicalTermIDPrefix", StrW, NoLimite, &PhysicalTermIDPrefixFunc, NULL},
	{"PhysicalTermIDAddLen", UnsignIntW, NoLimite, &PhysicalTermIDAddLenFunc, NULL},
	{"RTPPrefix", StrW, NoLimite, &RTPPrefixFunc, NULL},
	{"EphemeralTermIDAddLen", UnsignIntW, NoLimite, &EphemeralTermIDAddLenFunc, NULL},
	{"EphemeralTermIDUniform", UnsignIntW, NoLimite, &EphemeralTermIDUniformFunc, NULL},
	{"EphemeralTermIDStart", UnsignIntW, NoLimite, &EphemeralTermIDStartFunc, NULL},
	{"ThreeHandShake", UnsignIntW, NoLimite, &ThreeHandShakeFunc, NULL},
	{"LongTimer", UnsignIntW, NoLimite, &LongTimerFunc, NULL},
	{"PendingTimerInit", UnsignIntW, NoLimite, &PendingTimerInitFunc, NULL},
	{"RetranIntervalTimer", UnsignIntW, NoLimite, &RetranIntervalTimerFunc, NULL},
	{"MaxRetranCount", UnsignIntW, NoLimite, &MaxRetranCountFunc, NULL},
	{"RetransmissionTime", IntW, NoLimite, &RetransmissionTimeFunc, NULL},
	{"RetransmissionCycle", IntW, NoLimite, &RetransmissionCycleFunc, NULL},
	{"HeartbeatMode", UnsignIntW, NoLimite, &HeartbeatModeFunc, NULL},
	{"HeartbeatCycle", IntW, NoLimite, &HeartbeatCycleFunc, NULL},
	{"HeartbeatCount", IntW, NoLimite, &HeartbeatCountFunc, NULL},
	{"RegisterCycle", IntW, NoLimite, &RegisterCycleFunc, NULL},
	{"MandatoryRegister", BooleanW, NoLimite, &MandatoryRegisterFunc, NULL},
	{"AuthenticationMethID", UnsignIntW, NoLimite, &AuthenticationMethIDFunc, NULL},	

	{NULL, -1, -1, NULL, NULL}
};
#endif

#if defined(TCSUPPORT_CT_PON_GS)
CwmpParameterStruct HW_Profile[] = 
{
	{"ProfileBody", StrW, NoLimite, &ProfileBodyFunc, NULL},	
	{NULL, -1, -1, NULL, NULL}
};


CwmpParameterStruct HW_H248[] = 
{
	{"Profile", ObjectR, NoLimite, NULL, HW_Profile},
	{NULL, -1, -1, NULL, NULL}
};
#endif

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.NumberingPlan.
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
CwmpParameterStruct NumberingPlan[] = 
{
#if defined(TCSUPPORT_CT)
	{X_CT_COM"NumberPlan", StrW, NoLimite, &COM_NumberPlanFuncs, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.COM_G711FAXFuncs.
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/

#if defined(TCSUPPORT_CT)
CwmpParameterStruct COM_G711FAX[] = 
{
	{"Enable", BooleanW, NoLimite, &G711FAXEnableFuncs,	NULL},
	{"ControlType",	StrW, NoLimite, &ControlTypeFuncs, NULL},
	
	{NULL, -1, -1, NULL, NULL}
};
#endif

#if CHINA_UNICOM_ENABLE

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.X_CU_H248.auth.
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
CwmpParameterStruct H248_auth[] = 
{
    
	{"auth",                       BooleanW,   NoLimite, &authFunc,                                 NULL},
	{"authInitKey",                StrW,       NoLimite, &authInitKeyFunc,                          NULL},
    {"authmgid",                   StrW,       NoLimite, &authmgidFunc,                             NULL},
    {"authHeader",                 BooleanW,   NoLimite, &authHeaderFunc,                           NULL},
	{"HeaderSecurityType",         StrW,       NoLimite, &HeaderSecurityTypeFunc,                   NULL},
    {"Rfc2833EncryptKey",          StrW,       NoLimite, &Rfc2833EncryptKeyFunc,                    NULL},
    
	{NULL, -1, -1, NULL, NULL}
};

/*
    Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.X_CU_H248.StackInfo.
    Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
CwmpParameterStruct H248_StackInfo[] = 
{
	{"MsgFormat",                  StrW,       NoLimite, &MsgFormatFunc,                            NULL},
	{"UdpAlfFlag",                 BooleanW,   NoLimite, &UdpAlfFlagFunc,                           NULL},
    {"ResponseAckCtrl",            BooleanW,   NoLimite, &ResponseAckCtrlFunc,                      NULL},
    {"LongTimer",                  UnsignIntW, NoLimite, &LongTimerFunc,                            NULL},
	{"MaxreTransTime",             UnsignIntW, NoLimite, &MaxreTransTimeFunc,                       NULL},
    {"MinreTransTime",             UnsignIntW, NoLimite, &MinreTransTimeFunc,                       NULL},
    {"MGProvisionalRespTime",      UnsignIntW, NoLimite, &MGProvisionalRespTimeFunc,                NULL},
    {"MTUValue",                   UnsignIntW, NoLimite, &MTUValueFunc,                             NULL},
    {"MsgSegmentation",            BooleanW,   NoLimite, &MsgSegmentationFunc,                      NULL},
	{"RetranIntervalTimer", UnsignIntW, NoLimite, &RetranIntervalTimerFunc, NULL},
	{"MaxRetranCount", UnsignIntW, NoLimite, &MaxRetranCountFunc, NULL},
	{"RetransTime", UnsignIntW, NoLimite, &RetransmissionTimeFunc, NULL},
	{"RetransCycle", UnsignIntW, NoLimite, &RetransmissionCycleFunc, NULL},
	{"RegisterCycle", UnsignIntW, NoLimite, &RegisterCycleFunc, NULL},
    {NULL, -1, -1, NULL, NULL}
};

/*
    Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.X_CU_H248.H248.Digitmap.
    Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
CwmpParameterStruct H248_Digitmap[] = 
{
	{"DMName",                     StrW,       NoLimite, &H248DMNameFuncs,                                      NULL},
    {"DigitMap",                   StrW,       NoLimite, &H248DigitMapFuncs,                        NULL},
    {"DigitMapStartTimer",         UnsignIntW, NoLimite, &StartDigitTimerFuncs,                     NULL},
    {"DigitMapShortTimer",         UnsignIntW, NoLimite, &InterDigitTimerShortFuncs,                NULL},
    {"DigitMapLongTimer",          UnsignIntW, NoLimite, &InterDigitTimerLongFuncs,                 NULL},
    {NULL, -1, -1, NULL, NULL}
};

/*
    Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.X_CU_H248.H248.Extend.
    Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
CwmpParameterStruct H248_Extend[] = 
{
    {"HeartbeatMode",         UnsignIntW,   NoLimite, &HeartbeatModeFunc,                        NULL},
    {"HeartBeatTimer",             UnsignIntW, NoLimite, &HeartbeatCycleFunc,                       NULL},
    {"HeartBeatRetransTimer",      UnsignIntW, NoLimite, &HeartBeatRetransTimerFunc,                NULL},
    {"HeartBeatRetransTimes",      UnsignIntW, NoLimite, &HeartbeatCountFunc,                       NULL},
    {"MWDBaseTime",                UnsignIntW, NoLimite, &MWDBaseTimeFunc,                          NULL},
    {"MWD",                        UnsignIntW, NoLimite, &MWDFunc,                                  NULL},
    {"MgcSwitchMethod",           StrW,       NoLimite, &MgcSwitchMethodFunc,                      NULL},
    {NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.X_CU_H248. 
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
CwmpParameterStruct CU_H248[] = 
{
	{"CallAgent1",                 StrW,       NoLimite, &MediaGatewayControlerFuncs,               NULL},
	{"CallAgentPort1",             UnsignIntW, NoLimite, &MediaGatewayControlerPortFunc,            NULL},
	{"CallAgent2",                 StrW,       NoLimite, &Standby_MediaGatewayControlerFunc,        NULL},
	{"CallAgentPort2",            UnsignIntW, NoLimite, &Standby_MediaGatewayControlerPortFunc,    NULL},
	{"LocalPort",                  UnsignIntW, NoLimite, &MediaGatewayPortFunc,                     NULL},
	{"Domain",                     StrW,       NoLimite, &DeviceIDFunc,                             NULL},
    {"DeviceName",                 StrW,       NoLimite, &DeviceNameFunc,                           NULL}, 
	{"MIDFormat",                  StrW,       NoLimite, &MIDFormatFunc,                            NULL},
    {"DSCPMark",                   UnsignIntW, NoLimite, &DSCPMarkFunc,                             NULL},
    {"RTPPrefix",                  StrW,       NoLimite, &RTPPrefixFunc,                            NULL},
    {"EphemeralTermIDAddLen",      UnsignIntW, NoLimite, &EphemeralTermIDAddLenFunc,                NULL},
    {"EphemeralTermIDUniform",     UnsignIntW, NoLimite, &EphemeralTermIDUniformFunc,               NULL},
    {"EphemeralTermIDStart",       UnsignIntW, NoLimite, &EphemeralTermIDStartFunc,                 NULL},
    {"auth",                       ObjectR,    NoLimite, NULL,                                      H248_auth},
    {"StackInfo",                  ObjectR,    NoLimite, NULL,                                      H248_StackInfo},
    {"Digitmap",                   ObjectR,    NoLimite, NULL,                                      H248_Digitmap},
    {"Extend",                     ObjectR,    NoLimite, NULL,                                      H248_Extend},
	  {"SoftswitchVendor",				   UnsignIntW, NoLimite, &SoftswitchVendorFunc, 							NULL},
	  {"SoftswitchVersion",			 StrW, NoLimite, &SoftswitchVersionFunc,							  NULL},
	{NULL, -1, -1, NULL, NULL}
};
#endif

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}
	Parent Node Table Location: cp_Services_nodetable.c
*/
CwmpParameterStruct VoiceProfile[] = 
{
#if defined(TCSUPPORT_CT_PON_SC)
	{X_CT_COM"RingVoltage" ,UnsignIntR, NoLimite, &RingVoltage, NULL},
	{"CallIDMsgType", StrW, NoLimite, &CallIDMsgType, NULL},
#endif
	{"RTP", ObjectR, NoLimite, NULL, RTP},
	{"SIP", ObjectR, NoLimite, NULL, SIPTab},
	{"FaxT38", ObjectR, NoLimite, NULL, FaxT38_Profile},
	{"Line", ObjectR, NoLimite, &LineFuncs, NULL},
	{"DigitMapEnable", BooleanW, NoLimite, &DigitMapEnableFuncs, NULL},
	{"DigitMap", StrW, NoLimite, &DigitMapFuncs, NULL},
#if defined(TCSUPPORT_CT_PON_GS)
	{"X_HW_DigitMapMatchMode", StrW, NoLimite, &DigitMapMatchModeFuncs, NULL},
#endif
#if defined(TCSUPPORT_CT)
#if defined(TCSUPPORT_CT_PON_SC)
	{X_CT_COM"DigitMapSpecialEnable", BooleanW, NoLimite, &DigitMapSpecialEnableFuncs, NULL},
	{X_CT_COM"DigitMapSpecial", StrW, NoLimite, &DigitMapSpecialFuncs, NULL},
#else
	{"DigitMapSpecialEnable", BooleanW, NoLimite, &DigitMapSpecialEnableFuncs, NULL},
	{"DigitMapSpecial", StrW, NoLimite, &DigitMapSpecialFuncs, NULL},
#endif
	{X_CT_COM"ServerType", StrW, NoLimite, &ServerTypeFuncs, NULL},
	{X_CT_COM"StartDigitTimer",	IntW, NoLimite,	&StartDigitTimerFuncs, NULL},
	{X_CT_COM"InterDigitTimerShort", IntW, NoLimite, &InterDigitTimerShortFuncs, NULL},
	{X_CT_COM"InterDigitTimerLong",	IntW, NoLimite,	&InterDigitTimerLongFuncs, NULL},
	{X_CT_COM"HangingReminderToneTimer", IntW, NoLimite, &HangingReminderToneTimerFuncs, NULL},
	{X_CT_COM"BusyToneTimer", IntW,	NoLimite, &BusyToneTimerFuncs, NULL},
	{X_CT_COM"NoAnswerTimer", IntW,	NoLimite, &NoAnswerTimerFuncs, NULL},
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER) && defined (TCSUPPORT_ECN_MEGACO)
	{X_CT_COM"H248", ObjectR, NoLimite, NULL, COM_H248},
#endif
#if defined(TCSUPPORT_CT_PON_GS)
	{"X_HW_H248", ObjectR, NoLimite, NULL, HW_H248},
#endif
	{X_CT_COM"G711FAX", ObjectR, NoLimite, NULL, COM_G711FAX},
	{X_CT_COM"IADDiagnostics", ObjectR, NoLimite,NULL ,IADDiagnostics },
#endif
#if CHINA_UNICOM_ENABLE && defined (TCSUPPORT_ECN_MEGACO)
	{X_CU"H248",     ObjectR, NoLimite, NULL, CU_H248},
#endif
	{"NumberingPlan", ObjectR, NoLimite, NULL, NumberingPlan},	
      #if defined(TCSUPPORT_CUC_VOIP_NODE)
       {"Enable", StrW, NoLimite, &EnableFuncs, NULL},
       {"Reset" ,BooleanW, NoLimite, &ResetFuncs, NULL},
       {"NumberOfLines" ,UnsignIntR, NoLimite, &NumberOfLinesFuncs, NULL},
       {"Name" ,StrW, NoLimite, &NameFuncs, NULL},
       {"SignalingProtocol" ,StrW, NoLimite, &SignalingProtocolFuncs, NULL},
       {"MaxSessions" ,UnsignIntW, NoLimite, &MaxSessionsFuncs, NULL},
       {"DTMFMethod" ,StrW, NoLimite, &DTMFMethodFuncs, NULL},
       {"DTMFMethodG711" ,StrW, NoLimite, &DTMFMethodG711Funcs, NULL},
       {X_CUC_COM"DigitMapMatchMode" ,StrW, NoLimite, &X_CU_DigitMapMatchModeFuncs, NULL},
       {X_CUC_COM"WANName" ,StrR, NoLimite, &X_CU_WANNameFuncs, NULL},
       {X_CUC_COM"InterfaceState" ,StrR, NoLimite, &X_CU_InterfaceStateFuncs, NULL},
   #endif
	{NULL, -1, -1, NULL, NULL}
};

