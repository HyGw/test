#include "Global_res.h"
#include "cp_wandev_wandsldiag.h"

#if !defined(TCSUPPORT_CT_PON)

static NodeFunc LoopDiagnosticsState=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_LoopDiagnosticsStateValue,
	setWANDSLDiagnositcs_LoopDiagnosticsStateValue, 
	getWANDSLDiagnositcs_LoopDiagnosticsStateAttribute, 
	setWANDSLDiagnositcs_LoopDiagnosticsStateAttribute, 
	NULL,
};

static NodeFunc ACTPSDds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_ACTPSDdsValue,
	NULL, 
	getWANDSLDiagnositcs_ACTPSDdsAttribute, 
	setWANDSLDiagnositcs_ACTPSDdsAttribute, 
	NULL,
};

static NodeFunc ACTPSDus=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_ACTPSDusValue,
	NULL, 
	getWANDSLDiagnositcs_ACTPSDusAttribute, 
	setWANDSLDiagnositcs_ACTPSDusAttribute, 
	NULL,
};

static NodeFunc ACTATPds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_ACTATPdsValue,
	NULL, 
	getWANDSLDiagnositcs_ACTATPdsAttribute, 
	setWANDSLDiagnositcs_ACTATPdsAttribute, 
	NULL,
};

static NodeFunc ACTATPus=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_ACTATPusValue,
	NULL, 
	getWANDSLDiagnositcs_ACTATPusAttribute, 
	setWANDSLDiagnositcs_ACTATPusAttribute, 
	NULL,
};

static NodeFunc HLINSCds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_HLINSCdsValue,
	NULL, 
	getWANDSLDiagnositcs_HLINSCdsAttribute, 
	setWANDSLDiagnositcs_HLINSCdsAttribute, 
	NULL,
};

static NodeFunc HLINpsds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_HLINpsdsValue,
	NULL, 
	getWANDSLDiagnositcs_HLINpsdsAttribute, 
	setWANDSLDiagnositcs_HLINpsdsAttribute, 
	NULL,
};

static NodeFunc QLNpsds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_QLNpsdsValue,
	NULL, 
	getWANDSLDiagnositcs_QLNpsdsAttribute, 
	setWANDSLDiagnositcs_QLNpsdsAttribute, 
	NULL,
};

static NodeFunc SNRpsds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_SNRpsdsValue,
	NULL, 
	getWANDSLDiagnositcs_SNRpsdsAttribute, 
	setWANDSLDiagnositcs_SNRpsdsAttribute, 
	NULL,
};

static NodeFunc BITSpsds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_BITSpsdsValue,
	NULL, 
	getWANDSLDiagnositcs_BITSpsdsAttribute, 
	setWANDSLDiagnositcs_BITSpsdsAttribute, 
	NULL,
};

static NodeFunc GAINSpsds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_GAINSpsdsValue,
	NULL, 
	getWANDSLDiagnositcs_GAINSpsdsAttribute, 
	setWANDSLDiagnositcs_GAINSpsdsAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CT_WAN_PTM)
static NodeFunc HLINSCus=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_HLINSCusValue,
	NULL, 
	getWANDSLDiagnositcs_HLINSCusAttribute, 
	setWANDSLDiagnositcs_HLINSCusAttribute, 
	NULL,
};

static NodeFunc HLINGds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_HLINGdsValue,
	NULL, 
	getWANDSLDiagnositcs_HLINGdsAttribute, 
	setWANDSLDiagnositcs_HLINGdsAttribute, 
	NULL,
};

static NodeFunc HLINGus=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_HLINGusValue,
	NULL, 
	getWANDSLDiagnositcs_HLINGusAttribute, 
	setWANDSLDiagnositcs_HLINGusAttribute, 
	NULL,
};

static NodeFunc HLINpsus=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_HLINpsusValue,
	NULL, 
	getWANDSLDiagnositcs_HLINpsusAttribute, 
	setWANDSLDiagnositcs_HLINpsusAttribute, 
	NULL,
};

static NodeFunc HLOGGds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_HLOGGdsValue,
	NULL, 
	getWANDSLDiagnositcs_HLOGGdsAttribute, 
	setWANDSLDiagnositcs_HLOGGdsAttribute, 
	NULL,
};

static NodeFunc HLOGGus=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_HLOGGusValue,
	NULL, 
	getWANDSLDiagnositcs_HLOGGusAttribute, 
	setWANDSLDiagnositcs_HLOGGusAttribute, 
	NULL,
};

static NodeFunc HLOGpsds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_HLOGpsdsValue,
	NULL, 
	getWANDSLDiagnositcs_HLOGpsdsAttribute, 
	setWANDSLDiagnositcs_HLOGpsdsAttribute, 
	NULL,
};

static NodeFunc HLOGpsus=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_HLOGpsusValue,
	NULL, 
	getWANDSLDiagnositcs_HLOGpsusAttribute, 
	setWANDSLDiagnositcs_HLOGpsusAttribute, 
	NULL,
};

static NodeFunc HLOGMTds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_HLOGMTdsValue,
	NULL, 
	getWANDSLDiagnositcs_HLOGMTdsAttribute, 
	setWANDSLDiagnositcs_HLOGMTdsAttribute, 
	NULL,
};

static NodeFunc HLOGMTus=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_HLOGMTusValue,
	NULL, 
	getWANDSLDiagnositcs_HLOGMTusAttribute, 
	setWANDSLDiagnositcs_HLOGMTusAttribute, 
	NULL,
};

static NodeFunc QLNGds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_QLNGdsValue,
	NULL, 
	getWANDSLDiagnositcs_QLNGdsAttribute, 
	setWANDSLDiagnositcs_QLNGdsAttribute, 
	NULL,
};

static NodeFunc QLNGus=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_QLNGusValue,
	NULL, 
	getWANDSLDiagnositcs_QLNGusAttribute, 
	setWANDSLDiagnositcs_QLNGusAttribute, 
	NULL,
};

static NodeFunc QLNpsus=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_QLNpsusValue,
	NULL, 
	getWANDSLDiagnositcs_QLNpsusAttribute, 
	setWANDSLDiagnositcs_QLNpsusAttribute, 
	NULL,
};

static NodeFunc QLNMTds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_QLNMTdsValue,
	NULL, 
	getWANDSLDiagnositcs_QLNMTdsAttribute, 
	setWANDSLDiagnositcs_QLNMTdsAttribute, 
	NULL,
};

static NodeFunc QLNMTus=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_QLNMTusValue,
	NULL, 
	getWANDSLDiagnositcs_QLNMTusAttribute, 
	setWANDSLDiagnositcs_QLNMTusAttribute, 
	NULL,
};

static NodeFunc SNRGds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_SNRGdsValue,
	NULL, 
	getWANDSLDiagnositcs_SNRGdsAttribute, 
	setWANDSLDiagnositcs_SNRGdsAttribute, 
	NULL,
};

static NodeFunc SNRGus=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_SNRGusValue,
	NULL, 
	getWANDSLDiagnositcs_SNRGusAttribute, 
	setWANDSLDiagnositcs_SNRGusAttribute, 
	NULL,
};

static NodeFunc SNRpsus=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_SNRpsusValue,
	NULL, 
	getWANDSLDiagnositcs_SNRpsusAttribute, 
	setWANDSLDiagnositcs_SNRpsusAttribute, 
	NULL,
};

static NodeFunc SNRMTds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_SNRMTdsValue,
	NULL, 
	getWANDSLDiagnositcs_SNRMTdsAttribute, 
	setWANDSLDiagnositcs_SNRMTdsAttribute, 
	NULL,
};

static NodeFunc SNRMTus=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_SNRMTusValue,
	NULL, 
	getWANDSLDiagnositcs_SNRMTusAttribute, 
	setWANDSLDiagnositcs_SNRMTusAttribute, 
	NULL,
};

static NodeFunc LATNpbds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_LATNpbdsValue,
	NULL, 
	getWANDSLDiagnositcs_LATNpbdsAttribute, 
	setWANDSLDiagnositcs_LATNpbdsAttribute, 
	NULL,
};

static NodeFunc LATNpbus=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_LATNpbusValue,
	NULL, 
	getWANDSLDiagnositcs_LATNpbusAttribute, 
	setWANDSLDiagnositcs_LATNpbusAttribute, 
	NULL,
};

static NodeFunc SATNds=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_SATNdsValue,
	NULL, 
	getWANDSLDiagnositcs_SATNdsAttribute, 
	setWANDSLDiagnositcs_SATNdsAttribute, 
	NULL,
};

static NodeFunc SATNus=
{
	NULL,
	NULL,
	getWANDSLDiagnositcs_SATNusValue,
	NULL, 
	getWANDSLDiagnositcs_SATNusAttribute, 
	setWANDSLDiagnositcs_SATNusAttribute, 
	NULL,
};

#endif


/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLDiagnostics.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
CwmpParameterStruct WANDSLDiagnostics[] = 
{
	{"LoopDiagnosticsState", StrW, NoLimite, &LoopDiagnosticsState, NULL},
	{"ACTPSDds", UnsignIntR, NoLimite, &ACTPSDds, NULL},
	{"ACTPSDus", UnsignIntR, NoLimite, &ACTPSDus, NULL},
	{"ACTATPds", UnsignIntR, NoLimite, &ACTATPds, NULL},
	{"ACTATPus", UnsignIntR, NoLimite, &ACTATPus, NULL},
	{"HLINSCds", UnsignIntR, NoLimite, &HLINSCds, NULL},
	{"HLINpsds", StrR, NoLimite, &HLINpsds, NULL},
	{"QLNpsds", StrR, NoLimite, &QLNpsds, NULL},
	{"SNRpsds", StrR, NoLimite, &SNRpsds, NULL},
	{"BITSpsds", StrR, NoLimite, &BITSpsds, NULL},
	{"GAINSpsds", StrR, NoLimite, &GAINSpsds, NULL},
#if defined(TCSUPPORT_CT_WAN_PTM)
	{"HLINSCus", StrR, NoLimite, &HLINSCus, NULL},
	{"HLINGds", StrR, NoLimite, &HLINGds, NULL},
	{"HLINGus", StrR, NoLimite, &HLINGus, NULL},
	{"HLINpsus", StrR, NoLimite, &HLINpsus, NULL},	
	{"HLOGGds", StrR, NoLimite, &HLOGGds, NULL},
	{"HLOGGus", StrR, NoLimite, &HLOGGus, NULL},
	{"HLOGpsds", StrR, NoLimite, &HLOGpsds, NULL},
	{"HLOGpsus", StrR, NoLimite, &HLOGpsus, NULL},
	{"HLOGMTds", StrR, NoLimite, &HLOGMTds, NULL},
	{"HLOGMTus", StrR, NoLimite, &HLOGMTus, NULL},
	{"QLNGds", StrR, NoLimite, &QLNGds, NULL},
	{"QLNGus", StrR, NoLimite, &QLNGus, NULL},
	{"QLNpsus", StrR, NoLimite, &QLNpsus, NULL},
	{"QLNMTds", StrR, NoLimite, &QLNMTds, NULL},	
	{"QLNMTus", StrR, NoLimite, &QLNMTus, NULL},
	{"SNRGds", StrR, NoLimite, &SNRGds, NULL},
	{"SNRGus", StrR, NoLimite, &SNRGus, NULL},
	{"SNRpsus", StrR, NoLimite, &SNRpsus, NULL},
	{"SNRMTds", StrR, NoLimite, &SNRMTds, NULL},
	{"SNRMTus", StrR, NoLimite, &SNRMTus, NULL},
	{"LATNpbds", StrR, NoLimite, &LATNpbds, NULL},
	{"LATNpbus", StrR, NoLimite, &LATNpbus, NULL},
	{"SATNds", StrR, NoLimite, &SATNds, NULL},
	{"SATNus", StrR, NoLimite, &SATNus, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};

#endif

