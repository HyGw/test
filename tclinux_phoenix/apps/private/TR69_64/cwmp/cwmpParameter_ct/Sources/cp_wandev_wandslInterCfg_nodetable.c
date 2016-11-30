#include "Global_res.h"
#include "cp_wandev_wandslInterCfg.h"

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
static NodeFunc Enable=
{
	NULL,
	NULL,
	getWANDSLInterCfg_EnableValue, 
	setWANDSLInterCfg_EnableValue, 
	getWANDSLInterCfg_EnableAttribute, 
	setWANDSLInterCfg_EnableAttribute, 
	NULL
};

static NodeFunc LinkEncapsulationSupported=
{
	NULL,
	NULL,
	getWANDSLInterCfg_LinkEncapsulationSupportedValue,
	NULL, 
	getWANDSLInterCfg_LinkEncapsulationSupportedAttribute, 
	setWANDSLInterCfg_LinkEncapsulationSupportedAttribute, 
	NULL,
};

static NodeFunc LinkEncapsulationRequested=
{
	NULL,
	NULL,
	getWANDSLInterCfg_LinkEncapsulationRequestedValue,
	setWANDSLInterCfg_LinkEncapsulationRequestedValue, 
	getWANDSLInterCfg_LinkEncapsulationRequestedAttribute, 
	setWANDSLInterCfg_LinkEncapsulationRequestedAttribute, 
	NULL,
};

static NodeFunc LinkEncapsulationUsed=
{
	NULL,
	NULL,
	getWANDSLInterCfg_LinkEncapsulationUsedValue,
	NULL, 
	getWANDSLInterCfg_LinkEncapsulationUsedAttribute, 
	setWANDSLInterCfg_LinkEncapsulationUsedAttribute, 
	NULL,
};

static NodeFunc StandardsSupported=
{
	NULL,
	NULL,
	getStandardsSupportedValue,
	NULL,
	getStandardsSupportedAttribute, 
	setStandardsSupportedAttribute, 
	NULL,
};

static NodeFunc AllowedProfiles=
{
	NULL,
	NULL,
	getAllowedProfilesValue,
	NULL,
	getAllowedProfilesAttribute, 
	setAllowedProfilesAttribute, 
	NULL,
};

static NodeFunc CurrentProfiles=
{
	NULL,
	NULL,
	getCurrentProfilesValue,
	NULL,
	getCurrentProfilesAttribute, 
	setCurrentProfilesAttribute, 
	NULL,
};

static NodeFunc UPBOKLE=
{
	NULL,
	NULL,
	getUPBOKLEValue,
	NULL,
	getUPBOKLEAttribute, 
	setUPBOKLEAttribute, 
	NULL,
};

static NodeFunc TRELLISds=
{
	NULL,
	NULL,
	getTRELLISdsValue,
	NULL,
	getTRELLISdsAttribute, 
	setTRELLISdsAttribute, 
	NULL,
};

static NodeFunc TRELLISus=
{
	NULL,
	NULL,
	getTRELLISusValue,
	NULL,
	getTRELLISusAttribute, 
	setTRELLISusAttribute, 
	NULL,
};

static NodeFunc ACTSNRMODEds=
{
	NULL,
	NULL,
	getACTSNRMODEdsValue,
	NULL,
	getACTSNRMODEdsAttribute, 
	setACTSNRMODEdsAttribute, 
	NULL,
};

static NodeFunc ACTSNRMODEus=
{
	NULL,
	NULL,
	getACTSNRMODEusValue,
	NULL,
	getACTSNRMODEusAttribute, 
	setACTSNRMODEusAttribute, 
	NULL,
};

static NodeFunc ACTUALCE=
{
	NULL,
	NULL,
	getACTUALCEValue,
	NULL,
	getACTUALCEAttribute, 
	setACTUALCEAttribute, 
	NULL,
};

static NodeFunc SNRMpbds=
{
	NULL,
	NULL,
	getSNRMpbdsValue,
	NULL,
	getSNRMpbdsAttribute, 
	setSNRMpbdsAttribute, 
	NULL,
};

static NodeFunc SNRMpbus=
{
	NULL,
	NULL,
	getSNRMpbusValue,
	NULL,
	getSNRMpbusAttribute, 
	setSNRMpbusAttribute, 
	NULL,
};

#if !defined(TCSUPPORT_CT_PON)
static NodeFunc ModulationMode=
{
	NULL,
	NULL,
	getModulationModeValue,
	setModulationModeValue,
	getModulationModeAttribute,
	setModulationModeAttribute,
	NULL
};
#endif
#endif


static NodeFunc Status=
{
	NULL,
	NULL,
	getWANDSLInterCfg_StatusValue,
	NULL, 
	getWANDSLInterCfg_StatusAttribute, 
	setWANDSLInterCfg_StatusAttribute, 
	NULL,
};

static NodeFunc ModulationType=
{
	NULL,
	NULL,
	getModulationTypeValue,
	NULL, 
	getModulationTypeAttribute, 
	setModulationTypeAttribute, 
	NULL,
};

static NodeFunc LineEncoding=
{
	NULL,
	NULL,
	getLineEncodingValue,
	NULL, 
	getLineEncodingAttribute, 
	setLineEncodingAttribute, 
	NULL,
};

static NodeFunc DataPath=
{
	NULL,
	NULL,
	getDataPathValue,
	NULL, 
	getDataPathAttribute, 
	setDataPathAttribute, 
	NULL,
};

static NodeFunc UpstreamCurrRate=
{
	NULL,
	NULL,
	getUpstreamCurrRateValue,
	NULL, 
	getUpstreamCurrRateAttribute, 
	setUpstreamCurrRateAttribute, 
	NULL,
};

static NodeFunc DownstreamCurrRate=
{
	NULL,
	NULL,
	getDownstreamCurrRateValue,
	NULL, 
	getDownstreamCurrRateAttribute, 
	setDownstreamCurrRateAttribute, 
	NULL,
};

static NodeFunc UpstreamMaxRate=
{
	NULL,
	NULL,
	getUpstreamMaxRateValue,
	NULL, 
	getUpstreamMaxRateAttribute, 
	setUpstreamMaxRateAttribute, 
	NULL,
};

static NodeFunc DownstreamMaxRate=
{
	NULL,
	NULL,
	getDownstreamMaxRateValue,
	NULL, 
	getDownstreamMaxRateAttribute, 
	setDownstreamMaxRateAttribute, 
	NULL,
};

static NodeFunc UpstreamNoiseMargin=
{
	NULL,
	NULL,
	getUpstreamNoiseMarginValue,
	NULL, 
	getUpstreamNoiseMarginAttribute, 
	setUpstreamNoiseMarginAttribute, 
	NULL,
};

static NodeFunc DownstreamNoiseMargin=
{
	NULL,
	NULL,
	getDownstreamNoiseMarginValue,
	NULL, 
	getDownstreamNoiseMarginAttribute, 
	setDownstreamNoiseMarginAttribute, 
	NULL,
};

static NodeFunc UpstreamAttenuation=
{
	NULL,
	NULL,
	getUpstreamAttenuationValue,
	NULL, 
	getUpstreamAttenuationAttribute, 
	setUpstreamAttenuationAttribute, 
	NULL,
};

static NodeFunc DownstreamAttenuation=
{
	NULL,
	NULL,
	getDownstreamAttenuationValue,
	NULL, 
	getDownstreamAttenuationAttribute, 
	setDownstreamAttenuationAttribute, 
	NULL,
};

static NodeFunc UpstreamPower=
{
	NULL,
	NULL,
	getUpstreamPowerValue,
	NULL, 
	getUpstreamPowerAttribute, 
	setUpstreamPowerAttribute, 
	NULL,
};

static NodeFunc DownstreamPower=
{
	NULL,
	NULL,
	getDownstreamPowerValue,
	NULL, 
	getDownstreamPowerAttribute, 
	setDownstreamPowerAttribute, 
	NULL,
};

static NodeFunc ATURVendor=
{
	NULL,
	NULL,
	getATURVendorValue,
	NULL, 
	getATURVendorAttribute, 
	setATURVendorAttribute, 
	NULL,
};

static NodeFunc ATURCountry=
{
	NULL,
	NULL,
	getATURCountryValue,
	NULL, 
	getATURCountryAttribute, 
	setATURCountryAttribute, 
	NULL,
};

static NodeFunc ATUCVendor=
{
	NULL,
	NULL,
	getATUCVendorValue,
	NULL, 
	getATUCVendorAttribute, 
	setATUCVendorAttribute, 
	NULL,
};

static NodeFunc ATUCCountry=
{
	NULL,
	NULL,
	getATUCCountryValue,
	NULL, 
	getATUCCountryAttribute, 
	setATUCCountryAttribute, 
	NULL,
};

static NodeFunc ShowtimeStart=
{
	NULL,
	NULL,
	getShowtimeStartValue,
	NULL, 
	getShowtimeStartAttribute, 
	setShowtimeStartAttribute, 
	NULL,
};

static NodeFunc TotalStart=
{
	NULL,
	NULL,
	getTotalStartValue,
	NULL, 
	getTotalStartAttribute, 
	setTotalStartAttribute, 
	NULL,
};

static NodeFunc InterleaveDepth=
{
	NULL,
	NULL,
	getInterleaveDepthValue,
	NULL, 
	getInterleaveDepthAttribute, 
	setInterleaveDepthAttribute, 
	NULL,
};

static NodeFunc ATURANSIStd=
{
	NULL,
	NULL,
	getATURANSIStdValue,
	NULL, 
	getATURANSIStdAttribute, 
	setATURANSIStdAttribute, 
	NULL,
};

static NodeFunc ATURANSIRev=
{
	NULL,
	NULL,
	getATURANSIRevValue,
	NULL, 
	getATURANSIRevAttribute, 
	setATURANSIRevAttribute, 
	NULL,
};

static NodeFunc ATUCANSIStd=
{
	NULL,
	NULL,
	getATUCANSIStdValue,
	NULL, 
	getATUCANSIStdAttribute, 
	setATUCANSIStdAttribute, 
	NULL,
};

static NodeFunc ATUCANSIRev=
{
	NULL,
	NULL,
	getATUCANSIRevValue,
	NULL, 
	getATUCANSIRevAttribute, 
	setATUCANSIRevAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CT_ADSLSETTING) || defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
static NodeFunc StandardUsed=
{
	NULL,
	NULL,
	getStandardUsedValue,
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	NULL,
#else
	setStandardUsedValue, 
#endif
	getStandardUsedAttribute, 
	setStandardUsedAttribute, 
	NULL,
};
#endif

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
CwmpParameterStruct WANDSLInterfaceConfig[] = 
{
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	{"Enable", BooleanW, NoLimite, &Enable, NULL},
	{"LinkEncapsulationSupported", StrR, NoLimite, &LinkEncapsulationSupported, NULL},
	{"LinkEncapsulationRequested", StrW, NoLimite, &LinkEncapsulationRequested, NULL},
	{"LinkEncapsulationUsed", StrR, NoLimite, &LinkEncapsulationUsed, NULL},
	{"StandardsSupported", StrR, NoLimite, &StandardsSupported, NULL},
	{"AllowedProfiles", StrR, NoLimite, &AllowedProfiles, NULL},
	{"CurrentProfiles", StrR, NoLimite, &CurrentProfiles, NULL},
	{"UPBOKLE", UnsignIntR, NoLimite, &UPBOKLE, NULL},
	{"TRELLISds", IntR, NoLimite, &TRELLISds, NULL},
	{"TRELLISus", IntR, NoLimite, &TRELLISus, NULL},
	{"ACTSNRMODEds", UnsignIntR, NoLimite, &ACTSNRMODEds, NULL},
	{"ACTSNRMODEus", UnsignIntR, NoLimite, &ACTSNRMODEus, NULL},
	{"ACTUALCE", UnsignIntR, NoLimite, &ACTUALCE, NULL},
	{"SNRMpbds", StrR, NoLimite, &SNRMpbds, NULL},
	{"SNRMpbus", StrR, NoLimite, &SNRMpbus, NULL},
#if defined(TCSUPPORT_CT_WAN_PTM)
	{"TestParams", ObjectR, NoLimite, NULL, WandslInterCfg_TestParams},
#endif
#if !defined(TCSUPPORT_CT_PON)
	{X_CT_COM"ModulationMode", UnsignIntW, NoLimite, &ModulationMode, NULL},
#endif
#endif
	{"Status", StrR, NoLimite, &Status, NULL},
	{"ModulationType", StrR, NoLimite, &ModulationType, NULL},
	{"LineEncoding", StrR, NoLimite, &LineEncoding, NULL},
	{"DataPath", StrR, NoLimite, &DataPath, NULL},
	{"UpstreamCurrRate", UnsignIntR, NoLimite, &UpstreamCurrRate, NULL},
	{"DownstreamCurrRate", UnsignIntR, NoLimite, &DownstreamCurrRate, NULL},
	{"UpstreamMaxRate", UnsignIntR, NoLimite, &UpstreamMaxRate, NULL},
	{"DownstreamMaxRate", UnsignIntR, NoLimite, &DownstreamMaxRate, NULL},
	{"UpstreamNoiseMargin", IntR, NoLimite, &UpstreamNoiseMargin, NULL},
	{"DownstreamNoiseMargin", IntR, NoLimite, &DownstreamNoiseMargin, NULL},
	{"UpstreamAttenuation", IntR, NoLimite, &UpstreamAttenuation, NULL},
	{"DownstreamAttenuation", IntR, NoLimite, &DownstreamAttenuation, NULL},
	{"UpstreamPower", IntR, NoLimite, &UpstreamPower, NULL},
	{"DownstreamPower", IntR, NoLimite, &DownstreamPower, NULL},
	{"ATURVendor", StrR, NoLimite, &ATURVendor, NULL},
	{"ATURCountry", StrR, NoLimite, &ATURCountry, NULL},
	{"ATUCVendor", StrR, NoLimite, &ATUCVendor, NULL},
	{"ATUCCountry", StrR, NoLimite, &ATUCCountry, NULL},
	{"ShowtimeStart", UnsignIntR, NoLimite, &ShowtimeStart, NULL},
	{"TotalStart", UnsignIntR, NoLimite, &TotalStart, NULL},
	{"InterleaveDepth", UnsignIntR, NoLimite, &InterleaveDepth, NULL},
	{"ATURANSIStd", UnsignIntR, NoLimite, &ATURANSIStd, NULL},
	{"ATURANSIRev", UnsignIntR, NoLimite, &ATURANSIRev, NULL},
	{"ATUCANSIStd", UnsignIntR, NoLimite, &ATUCANSIStd, NULL},
	{"ATUCANSIRev", UnsignIntR, NoLimite, &ATUCANSIRev, NULL},

#if defined(TCSUPPORT_CT_ADSLSETTING)
	{"StandardUsed", StrW, NoLimite, &StandardUsed, NULL},
#else
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	{"StandardUsed", StrR, NoLimite, &StandardUsed, NULL},
#endif
#endif
	{"Stats", ObjectR, NoLimite, NULL, WandslInterCfg_Stats},
	{NULL, -1, -1, NULL, NULL}
};
