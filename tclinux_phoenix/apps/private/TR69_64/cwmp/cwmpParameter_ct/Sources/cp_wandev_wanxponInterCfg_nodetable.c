#include "Global_res.h"
#include "cp_wandev_wanxponInterCfg.h"


static NodeFunc Status=
{
	NULL,
	NULL,
	getWANPONInterCfg_StatusValue,
	NULL, 
	getWANPONInterCfg_StatusAttribute, 
	setWANPONInterCfg_StatusAttribute, 
	NULL,
};

static NodeFunc TXPower=
{
	NULL,
	NULL,
	getWANPONInterCfg_TXPowerValue,
	NULL, 
	getWANPONInterCfg_TXPowerAttribute, 
	setWANPONInterCfg_TXPowerAttribute, 
	NULL,
};

static NodeFunc RXPower=
{
	NULL,
	NULL,
	getWANPONInterCfg_RXPowerValue,
	NULL, 
	getWANPONInterCfg_RXPowerAttribute, 
	setWANPONInterCfg_RXPowerAttribute, 
	NULL,
};

static NodeFunc TransTemperature=
{
	NULL,
	NULL,
	getWANPONInterCfg_TransTemperatureValue,
	NULL, 
	getWANPONInterCfg_TransTemperatureAttribute, 
	setWANPONInterCfg_TransTemperatureAttribute, 
	NULL,
};

static NodeFunc SupplyVottage=
{
	NULL,
	NULL,
	getWANPONInterCfg_SupplyVottageValue,
	NULL, 
	getWANPONInterCfg_SupplyVottageAttribute, 
	setWANPONInterCfg_SupplyVottageAttribute, 
	NULL,
};

static NodeFunc BiasCurrent=
{
	NULL,
	NULL,
	getWANPONInterCfg_BiasCurrentValue,
	NULL, 
	getWANPONInterCfg_BiasCurrentAttribute, 
	setWANPONInterCfg_BiasCurrentAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
static NodeFunc ponenable=
{
	NULL,
	NULL,
	getWANPONInterCfg_EnableValue,
	setWANPONInterCfg_EnableValue, 
	getWANPONInterCfg_EnableAttribute, 
	setWANPONInterCfg_EnableAttribute, 
	NULL,
};

static NodeFunc ponStandardsSupported=
{
	NULL,
	NULL,
	getWANPONInterCfg_StandardsSupportedValue,
	NULL, 
	getWANPONInterCfg_StandardsSupportedAttribute, 
	setWANPONInterCfg_StandardsSupportedAttribute, 
	NULL,
};

static NodeFunc ponStandardUsed=
{
	NULL,
	NULL,
	getWANPONInterCfg_StandardUsedValue,
	NULL, 
	getWANPONInterCfg_StandardUsedAttribute, 
	setWANPONInterCfg_StandardUsedAttribute, 
	NULL,
};

static NodeFunc ponUpstreamMaxRate=
{
	NULL,
	NULL,
	getWANPONInterCfg_UpstreamMaxRateValue,
	NULL, 
	getWANPONInterCfg_UpstreamMaxRateAttribute, 
	setWANPONInterCfg_UpstreamMaxRateAttribute, 
	NULL,
};

static NodeFunc ponDownstreamMaxRate=
{
	NULL,
	NULL,
	getWANPONInterCfg_DownstreamMaxRateValue,
	NULL, 
	getWANPONInterCfg_DownstreamMaxRateAttribute, 
	setWANPONInterCfg_DownstreamMaxRateAttribute, 
	NULL,
};



CwmpParameterStruct ponOpticalTransceiver[] = 
{
	{"Temperature", IntR, NoLimite, &TransTemperature, NULL},
	{"Vcc", IntR, NoLimite, &SupplyVottage, NULL},
	{"TXBias", IntR, NoLimite, &BiasCurrent, NULL},
	{"TXPower", IntR, NoLimite, &TXPower, NULL},
	{"RXPower", IntR, NoLimite, &RXPower, NULL},

	{NULL, -1, -1, NULL, NULL}
};

#endif

#if defined(TCSUPPORT_CT_PMINFORM)
static NodeFunc DownData=
{
	NULL,
	NULL,
	getWANPONInterCfg_DownDataValue,
	NULL, 
	getWANPONInterCfg_DownDataAttribute, 
	setWANPONInterCfg_DownDataAttribute, 
	NULL,
};


static NodeFunc UpData=
{
	NULL,
	NULL,
	getWANPONInterCfg_UpDataValue,
	NULL, 
	getWANPONInterCfg_UpDataAttribute, 
	setWANPONInterCfg_UpDataAttribute, 
	NULL,
};

static NodeFunc TEMP=
{
	NULL,
	NULL,
	getWANPONInterCfg_TEMPValue,
	NULL, 
	getWANPONInterCfg_TEMPAttribute, 
	setWANPONInterCfg_TEMPAttribute, 
	NULL,
};

static NodeFunc PacketLost=
{
	NULL,
	NULL,
	getWANPONInterCfg_PacketLostValue,
	NULL, 
	getWANPONInterCfg_PacketLostAttribute, 
	setWANPONInterCfg_PacketLostAttribute, 
	NULL,
};

static NodeFunc PLR=
{
	NULL,
	NULL,
	getWANPONInterCfg_PLRValue,
	NULL, 
	getWANPONInterCfg_PLRAttribute, 
	setWANPONInterCfg_PLRAttribute, 
	NULL,
};

static NodeFunc ErrorCode=
{
	NULL,
	NULL,
	getWANPONInterCfg_ErrorCodeValue,
	NULL, 
	getWANPONInterCfg_ErrorCodeAttribute, 
	setWANPONInterCfg_ErrorCodeAttribute, 
	NULL,
};

static NodeFunc SER=
{
	NULL,
	NULL,
	getWANPONInterCfg_SERValue,
	NULL, 
	getWANPONInterCfg_SERAttribute, 
	setWANPONInterCfg_SERAttribute, 
	NULL,
};

static NodeFunc MemRate=
{
	NULL,
	NULL,
	getWANPONInterCfg_MemRateValue,
	NULL, 
	getWANPONInterCfg_MemRateAttribute, 
	setWANPONInterCfg_MemRateAttribute, 
	NULL,
};


static NodeFunc CPURate=
{
	NULL,
	NULL,
	getWANPONInterCfg_CPURateValue,
	NULL, 
	getWANPONInterCfg_CPURateAttribute, 
	setWANPONInterCfg_CPURateAttribute, 
	NULL,
};

static NodeFunc pmInformEnable=
{
	NULL,
	NULL,
	getWANPONInterCfg_PMInformEnableValue,
	setWANPONInterCfg_PMInformEnableValue,
	getWANPONInterCfg_PMInformEnableAttribute,
	setWANPONInterCfg_PMInformEnableAttribute,
	NULL,
};

static NodeFunc pmInformLSURL=
{
	NULL,
	NULL,
	getWANPONInterCfg_PMInformLSURLValue,
	setWANPONInterCfg_PMInformLSURLValue,
	getWANPONInterCfg_PMInformLSURLAttribute,
	setWANPONInterCfg_PMInformLSURLAttribute,
	NULL,
};

static NodeFunc pmInformLUInterval=
{
	NULL,
	NULL,
	getWANPONInterCfg_PMInformLUIntervalValue,
	setWANPONInterCfg_PMInformLUIntervalValue,
	getWANPONInterCfg_PMInformLUIntervalAttribute,
	setWANPONInterCfg_PMInformLUIntervalAttribute,
	NULL,
};

static NodeFunc pmInformLCInterval=
{
	NULL,
	NULL,
	getWANPONInterCfg_PMInformLCIntervalValue,
	setWANPONInterCfg_PMInformLCIntervalValue,
	getWANPONInterCfg_PMInformLCIntervalAttribute,
	setWANPONInterCfg_PMInformLCIntervalAttribute,
	NULL,
};

static NodeFunc pmInformCREnable=
{
	NULL,
	NULL,
	getWANPONInterCfg_PMInformCREnableValue,
	setWANPONInterCfg_PMInformCREnableValue,
	getWANPONInterCfg_PMInformCREnableAttribute,
	setWANPONInterCfg_PMInformCREnableAttribute,
	NULL,
};

static NodeFunc pmInformMREnable=
{
	NULL,
	NULL,
	getWANPONInterCfg_PMInformMREnableValue,
	setWANPONInterCfg_PMInformMREnableValue,
	getWANPONInterCfg_PMInformMREnableAttribute,
	setWANPONInterCfg_PMInformMREnableAttribute,
	NULL,
};

static NodeFunc pmInformSEREnable=
{
	NULL,
	NULL,
	getWANPONInterCfg_PMInformSEREnableValue,
	setWANPONInterCfg_PMInformSEREnableValue,
	getWANPONInterCfg_PMInformSEREnableAttribute,
	setWANPONInterCfg_PMInformSEREnableAttribute,
	NULL,
};

static NodeFunc pmInformECEnable=
{
	NULL,
	NULL,
	getWANPONInterCfg_PMInformECEnableValue,
	setWANPONInterCfg_PMInformECEnableValue,
	getWANPONInterCfg_PMInformECEnableAttribute,
	setWANPONInterCfg_PMInformECEnableAttribute,
	NULL,
};

static NodeFunc pmInformPLREnable=
{
	NULL,
	NULL,
	getWANPONInterCfg_PMInformPLREnableValue,
	setWANPONInterCfg_PMInformPLREnableValue,
	getWANPONInterCfg_PMInformPLREnableAttribute,
	setWANPONInterCfg_PMInformPLREnableAttribute,
	NULL,
};

static NodeFunc pmInformPLEnable=
{
	NULL,
	NULL,
	getWANPONInterCfg_PMInformPLEnableValue,
	setWANPONInterCfg_PMInformPLEnableValue,
	getWANPONInterCfg_PMInformPLEnableAttribute,
	setWANPONInterCfg_PMInformPLEnableAttribute,
	NULL,
};

static NodeFunc pmInformTMPEnable=
{
	NULL,
	NULL,
	getWANPONInterCfg_PMInformTMPEnableValue,
	setWANPONInterCfg_PMInformTMPEnableValue,
	getWANPONInterCfg_PMInformTMPEnableAttribute,
	setWANPONInterCfg_PMInformTMPEnableAttribute,
	NULL,
};

static NodeFunc pmInformUpDEnable=
{
	NULL,
	NULL,
	getWANPONInterCfg_PMInformUpDEnableValue,
	setWANPONInterCfg_PMInformUpDEnableValue,
	getWANPONInterCfg_PMInformUpDEnableAttribute,
	setWANPONInterCfg_PMInformUpDEnableAttribute,
	NULL,
};

static NodeFunc pmInformDwDEnable=
{
	NULL,
	NULL,
	getWANPONInterCfg_PMInformDwDEnableValue,
	setWANPONInterCfg_PMInformDwDEnableValue,
	getWANPONInterCfg_PMInformDwDEnableAttribute,
	setWANPONInterCfg_PMInformDwDEnableAttribute,
	NULL,
};

static NodeFunc pmInformLanDevMacEnable=
{
	NULL,
	NULL,
	getWANPONInterCfg_PMInformLanDevMacEnableValue,
	setWANPONInterCfg_PMInformLanDevMacEnableValue,
	getWANPONInterCfg_PMInformLanDevMacEnableAttribute,
	setWANPONInterCfg_PMInformLanDevMacEnableAttribute,
	NULL,
};

static NodeFunc pmInformLanDevMac=
{
	NULL,
	NULL,
	getWANPONInterCfg_PMInformLanDevMacValue,
	NULL,
	getWANPONInterCfg_PMInformLanDevMacAttribute,
	setWANPONInterCfg_PMInformLanDevMacAttribute,
	NULL,
};

static NodeFunc pmInformWlanDevMacEnable=
{
	NULL,
	NULL,
	getWANPONInterCfg_PMInformWlanDevMacEnableValue,
	setWANPONInterCfg_PMInformWlanDevMacEnableValue,
	getWANPONInterCfg_PMInformWlanDevMacEnableAttribute,
	setWANPONInterCfg_PMInformWlanDevMacEnableAttribute,
	NULL,
};

static NodeFunc pmInformWlanDevMac=
{
	NULL,
	NULL,
	getWANPONInterCfg_PMInformWlanDevMacValue,
	NULL,
	getWANPONInterCfg_PMInformWlanDevMacAttribute,
	setWANPONInterCfg_PMInformWlanDevMacAttribute,
	NULL,
};
#endif

/*
    rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.X_CT-COM_EponInterfaceConfig.
	                                 InternetGatewayDevice.WANDevice.1.X_CT-COM_GponInterfaceConfig.
*/

CwmpParameterStruct WANEPONInterfaceConfig[] = 
{
	{"Status", StrR, NoLimite, &Status, NULL},
#if defined(TCSUPPORT_CT_PMINFORM)
	{"DownData", StrR, NoLimite, &DownData, NULL},
	{"UpData", StrR, NoLimite, &UpData, NULL},
	{"TEMP", StrR, NoLimite, &TEMP, NULL},
	{"PacketLost", UnsignIntR, NoLimite, &PacketLost, NULL},
	{"PLR", UnsignIntR, NoLimite, &PLR, NULL},
	{"ErrorCode", UnsignIntR, NoLimite, &ErrorCode, NULL},
	{"SER", UnsignIntR, NoLimite, &SER, NULL},
	{"MemRate", UnsignIntR, NoLimite, &MemRate, NULL},
	{"CPURate", UnsignIntR, NoLimite, &CPURate, NULL},
	{"Enable", BooleanW, NoLimite, &pmInformEnable, NULL},
	{"LogServerUrl", StrW, NoLimite, &pmInformLSURL, NULL},
	{"LogUploadInterval", UnsignIntW, NoLimite, &pmInformLUInterval, NULL},
	{"LogCountInterval", UnsignIntW, NoLimite, &pmInformLCInterval, NULL},
	{"CPURateEnable", BooleanW, NoLimite, &pmInformCREnable, NULL},
	{"MemRateEnable", BooleanW, NoLimite, &pmInformMREnable, NULL},
	{"SEREnable", BooleanW, NoLimite, &pmInformSEREnable, NULL},
	{"ErrorCodeEnable", BooleanW, NoLimite, &pmInformECEnable, NULL},
	{"PLREnable", BooleanW, NoLimite, &pmInformPLREnable, NULL},
	{"PacketLostEnable", BooleanW, NoLimite, &pmInformPLEnable, NULL},
	{"TEMPEnable", BooleanW, NoLimite, &pmInformTMPEnable, NULL},
	{"UpDataEnable", BooleanW, NoLimite, &pmInformUpDEnable, NULL},
	{"DownDataEnable", BooleanW, NoLimite, &pmInformDwDEnable, NULL},
	{"LANDeviceMACEnable", BooleanW, NoLimite, &pmInformLanDevMacEnable, NULL},
	{"LANDeviceMAC", StrR, NoLimite, &pmInformLanDevMac, NULL},
	{"WLANDeviceMACEnable", BooleanW, NoLimite, &pmInformWlanDevMacEnable, NULL},
	{"WLANDeviceMACData", StrR, NoLimite, &pmInformWlanDevMac, NULL},
#endif
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"TXPower", IntR, NoLimite, &TXPower, NULL},
	{"RXPower", IntR, NoLimite, &RXPower, NULL},
	{"TransceiverTemperature", IntR, NoLimite, &TransTemperature, NULL},
	{"SupplyVottage", IntR, NoLimite, &SupplyVottage, NULL},
	{"BiasCurrent", IntR, NoLimite, &BiasCurrent, NULL},
#else
	{"Enable", BooleanW, NoLimite, &ponenable, NULL},
	{"StandardsSupported", StrR, NoLimite, &ponStandardsSupported, NULL},
	{"StandardUsed", StrR, NoLimite, &ponStandardUsed, NULL},
	{"UpstreamMaxRate", UnsignIntR, NoLimite, &ponUpstreamMaxRate, NULL},
	{"DownstreamMaxRate", UnsignIntR, NoLimite, &ponDownstreamMaxRate, NULL},
	{"OpticalTransceiver", ObjectR, NoLimite, NULL, ponOpticalTransceiver},
#endif
	{"Stats", ObjectR, NoLimite, NULL, WanEPonInterCfg_Stats},

	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct WANGPONInterfaceConfig[] = 
{
	{"Status", StrR, NoLimite, &Status, NULL},
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"TXPower", IntR, NoLimite, &TXPower, NULL},
	{"RXPower", IntR, NoLimite, &RXPower, NULL},
	{"TransceiverTemperature", IntR, NoLimite, &TransTemperature, NULL},
	{"SupplyVottage", IntR, NoLimite, &SupplyVottage, NULL},
	{"BiasCurrent", IntR, NoLimite, &BiasCurrent, NULL},
#else
	{"Enable", BooleanW, NoLimite, &ponenable, NULL},
	{"StandardsSupported", StrR, NoLimite, &ponStandardsSupported, NULL},
	{"StandardUsed", StrR, NoLimite, &ponStandardUsed, NULL},
	{"UpstreamMaxRate", UnsignIntR, NoLimite, &ponUpstreamMaxRate, NULL},
	{"DownstreamMaxRate", UnsignIntR, NoLimite, &ponDownstreamMaxRate, NULL},
	{"OpticalTransceiver", ObjectR, NoLimite, NULL, ponOpticalTransceiver},
#endif
	{"Stats", ObjectR, NoLimite, NULL, WanGPonInterCfg_Stats},

	{NULL, -1, -1, NULL, NULL}
};

