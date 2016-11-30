#include "Global_res.h"
#include "cp_Services_capab.h"


static NodeFunc MaxProfileCount=
{
	NULL,
	NULL,
	getMaxProfileCountValue, 
	NULL, 
	getMaxProfileCountAttribute, 
	setMaxProfileCountAttribute, 
	NULL
};

static NodeFunc MaxLineCount=
{
	NULL,
	NULL,
	getMaxLineCountValue, 
	NULL, 
	getMaxLineCountAttribute, 
	setMaxLineCountAttribute, 
	NULL
};

static NodeFunc MaxSessionsPerLine=
{
	NULL,
	NULL,
	getMaxSessionsPerLineValue, 
	NULL, 
	getMaxSessionsPerLineAttribute, 
	setMaxSessionsPerLineAttribute, 
	NULL
};

static NodeFunc MaxSessionCount=
{
	NULL,
	NULL,
	getMaxSessionCountValue, 
	NULL, 
	getMaxSessionCountAttribute, 
	setMaxSessionCountAttribute, 
	NULL
};

static NodeFunc Regions=
{
	NULL,
	NULL,
	getRegionsValue, 
	NULL, 
	getRegionsAttribute, 
	setRegionsAttribute, 
	NULL
};

static NodeFunc RTCP=
{
	NULL,
	NULL,
	getRTCPValue, 
	NULL, 
	getRTCPAttribute, 
	setRTCPAttribute, 
	NULL
};

static NodeFunc SRTP=
{
	NULL,
	NULL,
	getSRTPValue, 
	NULL, 
	getSRTPAttribute, 
	setSRTPAttribute, 
	NULL
};

static NodeFunc RTPRedundancy=
{
	NULL,
	NULL,
	getRTPRedundancyValue, 
	NULL, 
	getRTPRedundancyAttribute, 
	setRTPRedundancyAttribute, 
	NULL
};

static NodeFunc DSCPCoupled=
{
	NULL,
	NULL,
	getDSCPCoupledValue, 
	NULL, 
	getDSCPCoupledAttribute, 
	setDSCPCoupledAttribute, 
	NULL
};

static NodeFunc EthernetTaggingCoupled=
{
	NULL,
	NULL,
	getEthernetTaggingCoupledValue, 
	NULL, 
	getEthernetTaggingCoupledAttribute, 
	setEthernetTaggingCoupledAttribute, 
	NULL
};

static NodeFunc PSTNSoftSwitchOver=
{
	NULL,
	NULL,
	getPSTNSoftSwitchOverValue, 
	NULL, 
	getPSTNSoftSwitchOverAttribute, 
	setPSTNSoftSwitchOverAttribute, 
	NULL
};

static NodeFunc FaxT38=
{
	NULL,
	NULL,
	getFaxT38Value, 
	NULL, 
	getFaxT38Attribute, 
	setFaxT38Attribute, 
	NULL
};

static NodeFunc FaxPassThrough=
{
	NULL,
	NULL,
	getFaxPassThroughValue, 
	NULL, 
	getFaxPassThroughAttribute, 
	setFaxPassThroughAttribute, 
	NULL
};
	
static NodeFunc ModemPassThrough=
{
	NULL,
	NULL,
	getModemPassThroughValue, 
	NULL, 
	getModemPassThroughAttribute, 
	setModemPassThroughAttribute, 
	NULL
};
	
static NodeFunc ToneGeneration=
{
	NULL,
	NULL,
	getToneGenerationValue, 
	NULL, 
	getToneGenerationAttribute, 
	setToneGenerationAttribute, 
	NULL
};
	
static NodeFunc RingGeneration=
{
	NULL,
	NULL,
	getRingGenerationValue, 
	NULL, 
	getRingGenerationAttribute, 
	setRingGenerationAttribute, 
	NULL
};
		
static NodeFunc CUNumberingPlan=
{
	NULL,
	NULL,
	getCUNumberingPlanValue, 
	NULL, 
	getCUNumberingPlanAttribute, 
	setCUNumberingPlanAttribute, 
	NULL
};
		
static NodeFunc ButtonMap=
{
	NULL,
	NULL,
	getButtonMapValue, 
	NULL, 
	getButtonMapAttribute, 
	setButtonMapAttribute, 
	NULL
};
		
static NodeFunc VoicePortTests=
{
	NULL,
	NULL,
	getVoicePortTestsValue, 
	NULL, 
	getVoicePortTestsAttribute, 
	setVoicePortTestsAttribute, 
	NULL
};

static NodeFunc Cap_DigitMap=
{
	NULL,
	NULL,
	getCap_DigitMapValue, 
	NULL, 
	getCap_DigitMapAttribute, 
	setCap_DigitMapAttribute, 
	NULL
};

static NodeFunc SignalingProtocols=
{
	NULL,
	NULL,
	getSignalingProtocolsValue, 
	NULL, 
	getSignalingProtocolsAttribute, 
	setSignalingProtocolsAttribute, 
	NULL
};

#if defined(TCSUPPORT_CT)
static NodeFunc Heartbeat=
{
	NULL,
	NULL,
	getHeartbeatValue, 
	NULL, 
	getHeartbeatAttribute, 
	setHeartbeatAttribute, 
	NULL
};
#endif

static NodeFunc EntryID=
{
	NULL,
	NULL,
	getEntryIDValue, 
	NULL, 
	getEntryIDAttribute, 
	setEntryIDAttribute, 
	NULL
};

static NodeFunc Cs_Codec=
{
	NULL,
	NULL,
	getCs_CodecValue, 
	NULL, 
	getCs_CodecAttribute, 
	setCs_CodecAttribute, 
	NULL
};

static NodeFunc BitRate=
{
	NULL,
	NULL,
	getBitRateValue, 
	NULL, 
	getBitRateAttribute, 
	setBitRateAttribute, 
	NULL
};

static NodeFunc Cs_PacketizationPeriod=
{
	NULL,
	NULL,
	getCs_PacketizationPeriodValue, 
	NULL, 
	getCs_PacketizationPeriodAttribute, 
	setCs_PacketizationPeriodAttribute, 
	NULL
};

static NodeFunc SilenceSuppression=
{
	NULL,
	NULL,
	getSilenceSuppressionValue, 
	NULL, 
	getSilenceSuppressionAttribute, 
	setSilenceSuppressionAttribute, 
	NULL
};


CwmpParameterStruct Codecs[]=
{
	{"EntryID",	UnsignIntR,	NoLimite, &EntryID,	NULL},
	{"Codec", StrR,	NoLimite, &Cs_Codec, NULL},
#if !defined(TCSUPPORT_CT_PON_JX) || defined(TCSUPPORT_CT_PON_HUB)
	{"BitRate", UnsignIntR, NoLimite, &BitRate, NULL},
	{"SilenceSuppression", BooleanR, NoLimite, &SilenceSuppression, NULL},
#endif
	{"PacketizationPeriod",	StrR, NoLimite,	&Cs_PacketizationPeriod, NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct CodecsIndex[] = 
{
	{NULL, ObjectR, NoLimite, NULL, Codecs},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc CodecsFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitCodecsSubTree
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.Capabilities
	Parent Node Table Location: cp_Services_nodetable.c
*/
CwmpParameterStruct Capabilities[] = 
{
	{"MaxProfileCount", UnsignIntR, NoLimite, &MaxProfileCount, NULL},
	{"MaxLineCount", UnsignIntR, NoLimite, &MaxLineCount, NULL},
		
	{"MaxSessionsPerLine", UnsignIntR, NoLimite, &MaxSessionsPerLine, NULL},
	{"MaxSessionCount", UnsignIntR, NoLimite, &MaxSessionCount, NULL},
	{"Regions", StrR, NoLimite, &Regions, NULL},
	{"RTCP", BooleanR, NoLimite, &RTCP, NULL},
	{"SRTP", BooleanR, NoLimite, &SRTP, NULL},
	{"RTPRedundancy", BooleanR, NoLimite, &RTPRedundancy, NULL},
	{"DSCPCoupled", BooleanR, NoLimite, &DSCPCoupled, NULL},
	{"EthernetTaggingCoupled", BooleanR, NoLimite, &EthernetTaggingCoupled, NULL},
	{"PSTNSoftSwitchOver", BooleanR, NoLimite, &PSTNSoftSwitchOver, NULL},
	{"FaxPassThrough", BooleanR, NoLimite, &FaxPassThrough, NULL},
	{"ModemPassThrough", BooleanR, NoLimite, &ModemPassThrough, NULL},
	{"ToneGeneration", BooleanR, NoLimite, &ToneGeneration, NULL},
	{"RingGeneration", BooleanR, NoLimite, &RingGeneration, NULL},
	{"NumberingPlan", BooleanR, NoLimite, &CUNumberingPlan, NULL},
	{"ButtonMap", BooleanR, NoLimite, &ButtonMap, NULL},
	{"VoicePortTests", BooleanR, NoLimite, &VoicePortTests, NULL},
	{"FaxT38", BooleanR, NoLimite, &FaxT38, NULL},
	{"DigitMap", BooleanR, NoLimite, &Cap_DigitMap,	NULL},
	{"SignalingProtocols", StrR, NoLimite, &SignalingProtocols,	NULL},
#if defined(TCSUPPORT_CT)
	{X_CT_COM"Heartbeat", BooleanR,	NoLimite, &Heartbeat, NULL},
#endif
	{"Codecs", ObjectR, NoLimite, &CodecsFuncs,	NULL},
	{NULL, -1, -1, NULL, NULL}
};
