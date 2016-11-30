#include "Global_res.h"
#include "cp_Services.h"
#include "cp_Services_PhyInterface.h"

static NodeFunc InterfaceIDFuncs =
{
	NULL,
	NULL,
	getInterfaceIDValue,
	setInterfaceIDValue,
	getInterfaceIDAttribute,
	setInterfaceIDAttribute,
	NULL
};

static NodeFunc PhyPortFuncs =
{
	NULL,
	NULL,
	getPhyPortValue,
	setPhyPortValue,
	getPhyPortAttribute,
	setPhyPortAttribute,
	NULL
};

static NodeFunc TestStateFuncs = 
{
	NULL,
	NULL,
	getTestStateValue,
	setTestStateValue,
	getTestStateAttribute,
	setTestStateAttribute,
	NULL
};
static NodeFunc TestSelectorFuncs = 
{
	NULL,
	NULL,
	getTestSelectorValue,
	setTestSelectorValue,
	getTestSelectorAttribute,
	setTestSelectorAttribute,
	NULL
};
static NodeFunc PhoneConnectivityFuncs = 
{
	NULL,
	NULL,
	getPhoneConnectivityValue,
	NULL,
	getPhoneConnectivityAttribute,
	setPhoneConnectivityAttribute,
	NULL
};

#if defined(TCSUPPORT_CT)
static NodeFunc TestTypeFuncs = 
{
	NULL,
	NULL,
	getTestTypeValue,
	setTestTypeValue,
	getTestTypeAttribute,
	setTestTypeAttribute,
	NULL
};
static NodeFunc CalledNumberFuncs = 
{
	NULL,
	NULL,
	getCalledNumberValue,
	setCalledNumberValue,
	getCalledNumberAttribute,
	setCalledNumberAttribute,
	NULL
};

static NodeFunc CallHoldTimerFuncs = 
{
	NULL,
	NULL,
	getCallHoldTimerValue,
	setCallHoldTimerValue,
	getCallHoldTimerAttribute,
	setCallHoldTimerAttribute,
	NULL
};

static NodeFunc CalledWaitTimerFuncs = 
{
	NULL,
	NULL,
	getCalledWaitTimerValue,
	setCalledWaitTimerValue,
	getCalledWaitTimerAttribute,
	setCalledWaitTimerAttribute,
	NULL
};

static NodeFunc DailDTMFConfirmEnableFuncs = 
{
	NULL,
	NULL,
	getDailDTMFConfirmEnableValue,
	setDailDTMFConfirmEnableValue,
	getDailDTMFConfirmEnableAttribute,
	setDailDTMFConfirmEnableAttribute,
	NULL
};
static NodeFunc DailDTMFConfirmNumberFuncs = 
{
	NULL,
	NULL,
	getDailDTMFConfirmNumberValue,
	setDailDTMFConfirmNumberValue,
	getDailDTMFConfirmNumberAttribute,
	setDailDTMFConfirmNumberAttribute,
	NULL
};
static NodeFunc DailDTMFConfirmResultFuncs = 
{
	NULL,
	NULL,
	getDailDTMFConfirmResultValue,
	NULL,
	getDailDTMFConfirmResultAttribute,
	setDailDTMFConfirmResultAttribute,
	NULL
};
static NodeFunc StatusFuncs = 
{
	NULL,
	NULL,
	getStatusValue,
	NULL,
	getStatusAttribute,
	setStatusAttribute,
	NULL
};
static NodeFunc ConclusionFuncs = 
{
	NULL,
	NULL,
	getConclusionValue,
	NULL,
	getConclusionAttribute,
	setConclusionAttribute,
	NULL
};
static NodeFunc CallerFailReasonFuncs = 
{
	NULL,
	NULL,
	getCallerFailReasonValue,
	NULL,
	getCallerFailReasonAttribute,
	setCallerFailReasonAttribute,
	NULL
};
static NodeFunc CalledFailReasonFuncs = 
{
	NULL,
	NULL,
	getCalledFailReasonValue,
	NULL,
	getCalledFailReasonAttribute,
	setCalledFailReasonAttribute,
	NULL
};
static NodeFunc FailedResponseCodeFuncs = 
{
	NULL,
	NULL,
	getFailedResponseCodeValue,
	NULL,
	getFailedResponseCodeAttribute,
	setFailedResponseCodeAttribute,
	NULL
};

static NodeFunc StatTimeFuncs =
{
	NULL,
	NULL,
	getStatTimeValue,
	NULL,
	getStatTimeAttribute,
	setStatTimeAttribute,
	NULL
};

static NodeFunc TxPacketsFuncs =
{
	NULL,
	NULL,
	getTxPacketsValue,
	NULL,
	getTxPacketsAttribute,
	setTxPacketsAttribute,
	NULL
};

static NodeFunc RxPacketsFuncs =
{
	NULL,
	NULL,
	getRxPacketsValue,
	NULL,
	getRxPacketsAttribute,
	setRxPacketsAttribute,
	NULL
};

static NodeFunc MeanDelayFuncs =
{
	NULL,
	NULL,
	getMeanDelayValue,
	NULL,
	getMeanDelayAttribute,
	setMeanDelayAttribute,
	NULL
};

static NodeFunc MeanJitterFuncs =
{
	NULL,
	NULL,
	getMeanJitterValue,
	NULL,
	getMeanJitterAttribute,
	setMeanJitterAttribute,
	NULL
};

static NodeFunc FractionLossFuncs =
{
	NULL,
	NULL,
	getFractionLossValue,
	NULL,
	getFractionLossAttribute,
	setFractionLossAttribute,
	NULL
};

static NodeFunc LocalIPAddressFuncs =
{
	NULL,
	NULL,
	getLocalIPAddressValue,
	NULL,
	getLocalIPAddressAttribute,
	setLocalIPAddressAttribute,
	NULL
};

static NodeFunc LocalUDPPortFuncs =
{
	NULL,
	NULL,
	getLocalUDPPortValue,
	NULL,
	getLocalUDPPortAttribute,
	setLocalUDPPortAttribute,
	NULL
};

static NodeFunc FarEndIPAddressFuncs =
{
	NULL,
	NULL,
	getFarEndIPAddressValue,
	NULL,
	getFarEndIPAddressAttribute,
	setFarEndIPAddressAttribute,
	NULL
};

static NodeFunc FarEndUDPPortFuncs =
{
	NULL,
	NULL,
	getFarEndUDPPortValue,
	NULL,
	getFarEndUDPPortAttribute,
	setFarEndUDPPortAttribute,
	NULL
};

static NodeFunc MosLqFuncs =
{
	NULL,
	NULL,
	getMosLqValue,
	NULL,
	getMosLqAttribute,
	setMosLqAttribute,
	NULL
};

static NodeFunc CodecFuncs =
{
	NULL,
	NULL,
	getListCodecValue,
	NULL,
	getListCodecAttribute,
	setListCodecAttribute,
	NULL
};

static NodeFunc IsTestOnBusyFuncs = 
{
	NULL,
	NULL,
	getIsTestOnBusyValue,
	setIsTestOnBusyValue,
	getIsTestOnBusyAttribute,
	setIsTestOnBusyAttribute,
	NULL
};

static NodeFunc LineTestConclusionFuncs =
{
	NULL,
	NULL,
	getLineTestConclusionValue,
	NULL,
	getLineTestConclusionAttribute,
	setLineTestConclusionAttribute,
	NULL
};

static NodeFunc AGACVoltageFuncs =
{
	NULL,
	NULL,
	getAGACVoltageValue,
	NULL,
	getAGACVoltageAttribute,
	setAGACVoltageAttribute,
	NULL
};

static NodeFunc BGACVoltageFuncs =
{
	NULL,
	NULL,
	getBGACVoltageValue,
	NULL,
	getBGACVoltageAttribute,
	setBGACVoltageAttribute,
	NULL
};

static NodeFunc ABACVoltageFuncs =
{
	NULL,
	NULL,
	getABACVoltageValue,
	NULL,
	getABACVoltageAttribute,
	setABACVoltageAttribute,
	NULL
};

static NodeFunc AGDCVoltageFuncs =
{
	NULL,
	NULL,
	getAGDCVoltageValue,
	NULL,
	getAGDCVoltageAttribute,
	setAGDCVoltageAttribute,
	NULL
};

static NodeFunc BGDCVoltageFuncs =
{
	NULL,
	NULL,
	getBGDCVoltageValue,
	NULL,
	getBGDCVoltageAttribute,
	setBGDCVoltageAttribute,
	NULL
};

static NodeFunc ABDCVoltageFuncs =
{
	NULL,
	NULL,
	getABDCVoltageValue,
	NULL,
	getABDCVoltageAttribute,
	setABDCVoltageAttribute,
	NULL
};

static NodeFunc AGInsulationResistanceFuncs =
{
	NULL,
	NULL,
	getAGInsulationResistanceValue,
	NULL,
	getAGInsulationResistanceAttribute,
	setAGInsulationResistanceAttribute,
	NULL
};

static NodeFunc BGInsulationResistanceFuncs =
{
	NULL,
	NULL,
	getBGInsulationResistanceValue,
	NULL,
	getBGInsulationResistanceAttribute,
	setBGInsulationResistanceAttribute,
	NULL
};

static NodeFunc ABInsulationResistanceFuncs =
{
	NULL,
	NULL,
	getABInsulationResistanceValue,
	NULL,
	getABInsulationResistanceAttribute,
	setABInsulationResistanceAttribute,
	NULL
};

static NodeFunc AGCapacitanceFuncs =
{
	NULL,
	NULL,
	getAGCapacitanceValue,
	NULL,
	getAGCapacitanceAttribute,
	setAGCapacitanceAttribute,
	NULL
};

static NodeFunc BGCapacitanceFuncs =
{
	NULL,
	NULL,
	getBGCapacitanceValue,
	NULL,
	getBGCapacitanceAttribute,
	setBGCapacitanceAttribute,
	NULL
};

static NodeFunc ABCapacitanceFuncs =
{
	NULL,
	NULL,
	getABCapacitanceValue,
	NULL,
	getABCapacitanceAttribute,
	setABCapacitanceAttribute,
	NULL
};

static NodeFunc DialModeFuncs = 
{
	NULL,
	NULL,
	getDialModeValue,
	setDialModeValue,
	getDialModeAttribute,
	setDialModeAttribute,
	NULL
};

static NodeFunc HookFlashUpTimeFuncs = 
{
	NULL,
	NULL,
	getHookFlashUpTimeValue,
	setHookFlashUpTimeValue,
	getHookFlashUpTimeAttribute,
	setHookFlashUpTimeAttribute,
	NULL
};

static NodeFunc HookFlashDownTimeFuncs = 
{
	NULL,
	NULL,
	getHookFlashDownTimeValue,
	setHookFlashDownTimeValue,
	getHookFlashDownTimeAttribute,
	setHookFlashDownTimeAttribute,
	NULL
};

static NodeFunc ReversePolePulseFuncs = 
{
	NULL,
	NULL,
	getReversePolePulseValue,
	setReversePolePulseValue,
	getReversePolePulseAttribute,
	setReversePolePulseAttribute,
	NULL
};

static NodeFunc KcTypeFuncs = 
{
	NULL,
	NULL,
	getKcTypeValue,
	setKcTypeValue,
	getKcTypeAttribute,
	setKcTypeAttribute,
	NULL
};

static NodeFunc ClipTransWhenFuncs = 
{
	NULL,
	NULL,
	getClipTransWhenValue,
	setClipTransWhenValue,
	getClipTransWhenAttribute,
	setClipTransWhenAttribute,
	NULL
};


static NodeFunc ClipFormatFuncs = 
{
	NULL,
	NULL,
	getClipFormatValue,
	setClipFormatValue,
	getClipFormatAttribute,
	setClipFormatAttribute,
	NULL
};

static NodeFunc ClipSendDateTimeEnableFuncs = 
{
	NULL,
	NULL,
	getClipSendDateTimeEnableValue,
	setClipSendDateTimeEnableValue,
	getClipSendDateTimeEnableAttribute,
	setClipSendDateTimeEnableAttribute,
	NULL
};

static NodeFunc ClipSendDateTimeFuncs = 
{
	NULL,
	NULL,
	getClipSendDateTimeValue,
	setClipSendDateTimeValue,
	getClipSendDateTimeAttribute,
	setClipSendDateTimeAttribute,
	NULL
};

static NodeFunc RingDCVoltageOverlappedFuncs = 
{
  NULL,
  	NULL,
  	getRingDCVoltageOverlappedValue,
  	setRingDCVoltageOverlappedValue,
  	getRingDCVoltageOverlappedAttribute,
  setRingDCVoltageOverlappedAttribute,
  NULL
};

static NodeFunc DataServiceModeFuncs = 
{
  NULL,
  	NULL,
  	getDataServiceModeValue,
  	setDataServiceModeValue,
  	getDataServiceModeAttribute,
  	setDataServiceModeAttribute,
  	NULL
};


static NodeFunc ClipReversePoleFuncs = 
{
	NULL,
	NULL,
	getClipReversePoleValue,
	setClipReversePoleValue,
	getClipReversePoleAttribute,
	setClipReversePoleAttribute,
	NULL
};

static NodeFunc MWIRingFlagFuncs = 
{
	NULL,
	NULL,
	getMWIRingFlagValue,
	setMWIRingFlagValue,
	getMWIRingFlagAttribute,
	setMWIRingFlagAttribute,
	NULL
};

CwmpParameterStruct PoorQualityListIndex[] = 
{
	{NULL, ObjectR, NoLimite, NULL, PoorQualityList},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc PoorQualityListFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitPoorQualityListSubTree
};
//#endif

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.PhyInterface.{i}.Tests.X_CU_LineTest
	Parent Node Table Location: cp_Services_PhyInterface_nodetable.c
*/
CwmpParameterStruct X_CU_LineTest[] = 
{
	{"IsTestOnBusy", BooleanW, NoLimite, &IsTestOnBusyFuncs, NULL},
	{"Conclusion", StrR, NoLimite, &LineTestConclusionFuncs, NULL},	
	{"AGACVoltage", UnsignIntR, NoLimite, &AGACVoltageFuncs, NULL},
	{"BGACVoltage", UnsignIntR, NoLimite, &BGACVoltageFuncs, NULL},
	{"ABACVoltage", UnsignIntR, NoLimite, &ABACVoltageFuncs,NULL},
	{"AGDCVoltage", UnsignIntR,	NoLimite, &AGDCVoltageFuncs,NULL},
	{"BGDCVoltage", UnsignIntR, NoLimite, &BGDCVoltageFuncs,NULL},
	{"ABDCVoltage", UnsignIntR, NoLimite, &ABDCVoltageFuncs, NULL},
	{"AGInsulationResistance", UnsignIntR, NoLimite, &AGInsulationResistanceFuncs, NULL},
	{"BGInsulationResistance", UnsignIntR, NoLimite, &BGInsulationResistanceFuncs, NULL},
	{"ABInsulationResistance", UnsignIntR, NoLimite, &ABInsulationResistanceFuncs, NULL},
	{"AGCapacitance", UnsignIntR, NoLimite, &AGCapacitanceFuncs, NULL},
	{"BGCapacitance", UnsignIntR, NoLimite, &BGCapacitanceFuncs, NULL},
	{"ABCapacitance", UnsignIntR, NoLimite, &ABCapacitanceFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.PhyInterface.{i}.Tests.X_CT_COM_SimulateTest
	Parent Node Table Location: cp_Services_PhyInterface_nodetable.c
*/
CwmpParameterStruct COM_SimulateTest[] = 
{
	{"TestType", StrW, NoLimite, &TestTypeFuncs, NULL},
	{"CalledNumber", StrW, NoLimite, &CalledNumberFuncs, NULL},
	{"CallHoldTimer", UnsignIntW, NoLimite, &CallHoldTimerFuncs, NULL},
	{"CalledWaitTimer", UnsignIntW, NoLimite, &CalledWaitTimerFuncs, NULL},
	{"DailDTMFConfirmEnable", BooleanW, NoLimite, &DailDTMFConfirmEnableFuncs,NULL},
	{"DailDTMFConfirmNumber", StrW,	NoLimite, &DailDTMFConfirmNumberFuncs,NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"DailDTMFConfirmResult", BooleanR, NoLimite, &DailDTMFConfirmResultFuncs,NULL},
#else
	{"DailDTMFConfirmResult", StrR, NoLimite, &DailDTMFConfirmResultFuncs,NULL},
#endif
	{"Status", StrR, NoLimite, &StatusFuncs, NULL},
	{"Conclusion", StrR, NoLimite, &ConclusionFuncs, NULL},
	{"CallerFailReason", StrR, NoLimite, &CallerFailReasonFuncs, NULL},
	{"CalledFailReason", StrR, NoLimite, &CalledFailReasonFuncs, NULL},
	{"FailedResponseCode", UnsignIntR, NoLimite, &FailedResponseCodeFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};
#endif

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.PhyInterface.{i}.Tests
	Parent Node Table Location: cp_Services_PhyInterface_nodetable.c
*/
CwmpParameterStruct Tests[] = 
{
	{"TestState", StrW, NoLimite, &TestStateFuncs, NULL},
	{"TestSelector", StrW, NoLimite, &TestSelectorFuncs, NULL},
	{"PhoneConnectivity", BooleanR, NoLimite, &PhoneConnectivityFuncs, NULL},
#if defined(TCSUPPORT_CT)
	{X_CT_COM"SimulateTest", ObjectR, NoLimite,	NULL, COM_SimulateTest},
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"X_CU_LineTest", ObjectR, NoLimite, NULL, X_CU_LineTest},	
#endif
	{NULL, -1, -1, NULL, NULL}
};

#if defined(TCSUPPORT_CT)
/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.PhyInterface.{i}.X_CT-COM_Stats.PoorQualityList.{i}.
	Parent Node Table Location: cp_Services_PhyInterface.c
*/
CwmpParameterStruct PoorQualityList[] = 
{

	{"StatTime", DateTimeTypeR, NoLimite, &StatTimeFuncs, NULL},
	{"TxPackets", UnsignIntR, NoLimite, &TxPacketsFuncs, NULL},
	{"RxPackets", UnsignIntR, NoLimite,	&RxPacketsFuncs, NULL},
	{"MeanDelay", UnsignIntR, NoLimite,	&MeanDelayFuncs, NULL},
	{"MeanJitter", UnsignIntR, NoLimite, &MeanJitterFuncs, NULL},
	{"FractionLoss", UnsignIntR, NoLimite, &FractionLossFuncs, NULL},
	{"LocalIPAddress", StrR, NoLimite, &LocalIPAddressFuncs, NULL},
	{"LocalUDPPort", UnsignIntR, NoLimite, &LocalUDPPortFuncs, NULL},
	{"FarEndIPAddress",	StrR, NoLimite,	&FarEndIPAddressFuncs, NULL},
	{"FarEndUDPPort", UnsignIntR, NoLimite,	&FarEndUDPPortFuncs, NULL},
	{"MosLq", UnsignIntR, NoLimite,	&MosLqFuncs, NULL},
	{"Codec", StrR,	NoLimite, &CodecFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.PhyInterface.{i}.X_CT-COM_Stats
	Parent Node Table Location: cp_Services_PhyInterface_nodetable.c
*/
CwmpParameterStruct COM_Stats[] = 
{
	{"PoorQualityList",	ObjectR, NoLimite, &PoorQualityListFuncs, NULL},		
	{NULL, -1, -1, NULL, NULL}
};
#endif

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.PhyInterface.{i}.X_CU_Extend
	Parent Node Table Location: cp_Services_PhyInterface_nodetable.c
*/
CwmpParameterStruct X_CU_Extend[] = 
{
	{"DialMode",	StrW, NoLimite, &DialModeFuncs, NULL},
	{"HookFlashUpTime",	UnsignIntW, NoLimite, &HookFlashUpTimeFuncs, NULL},
	{"HookFlashDownTime",	UnsignIntW, NoLimite, &HookFlashDownTimeFuncs, NULL},
	{"ReversePolePulse",	BooleanW, NoLimite, &ReversePolePulseFuncs, NULL},
	{"KcType",	StrW, NoLimite, &KcTypeFuncs, NULL},
	{"ClipTransWhen",	StrW, NoLimite, &ClipTransWhenFuncs, NULL},
	{"ClipReversePole",	BooleanW, NoLimite, &ClipReversePoleFuncs, NULL},	
	{"MWIRingFlag",	BooleanW, NoLimite, &MWIRingFlagFuncs, NULL},	
	{"ClipSendDateTimeEnable",	BooleanW, NoLimite, &ClipSendDateTimeEnableFuncs, NULL},	
	{"ClipFormat",	StrW, NoLimite, &ClipFormatFuncs, NULL},
	{"HookFlashUpTime",	UnsignIntW, NoLimite, &HookFlashUpTimeFuncs, NULL},
	{"ClipSendDateTime",	UnsignIntW, NoLimite, &ClipSendDateTimeFuncs, NULL},
	{"RingDCVoltageOverlapped",	UnsignIntW, NoLimite, &RingDCVoltageOverlappedFuncs, NULL},
	{"DataServiceMode",	UnsignIntW, NoLimite, &DataServiceModeFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.PhyInterface.{i}
	Parent Node Table Location: cp_Services_nodetable.c
*/
CwmpParameterStruct PhyInterface[] = 
{

	{"InterfaceID", UnsignIntW,	NoLimite, &InterfaceIDFuncs, NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"PhyPort", StrW, NoLimite, &PhyPortFuncs, NULL},
#else
#if !defined(TCSUPPORT_CT_PON_JX) || defined(TCSUPPORT_CT_PON_HUB)
	{"PhyPort", UnsignIntW, NoLimite, &PhyPortFuncs, NULL},
#endif
#endif
	{"Tests", ObjectR, NoLimite, NULL, Tests},
#if defined(TCSUPPORT_CT)
	{X_CT_COM"Stats", ObjectR, NoLimite, NULL, COM_Stats},
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"X_CU_Extend", ObjectR, NoLimite, NULL, X_CU_Extend},
#endif
	{NULL, -1, -1, NULL, NULL}
};
