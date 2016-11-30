#include "Global_res.h"
#include "cp_wandev_wanconndev_atmF5LBDiag.h"

#if !defined(TCSUPPORT_CT_PON)

static NodeFunc DiagnosticsState=
{
	NULL,
	NULL,
	getWANATMF5LoopbackDiagnostics_DiagnosticsStateValue,
	setWANATMF5LoopbackDiagnostics_DiagnosticsStateValue,
	getWANATMF5LoopbackDiagnostics_DiagnosticsStateAttribute, 
	setWANATMF5LoopbackDiagnostics_DiagnosticsStateAttribute, 
	NULL
};

static NodeFunc NumberOfRepetitions=
{
	NULL,
	NULL,
	getWANATMF5LoopbackDiagnostics_NumberOfRepetitionsValue,
	setWANATMF5LoopbackDiagnostics_NumberOfRepetitionsValue,
	getWANATMF5LoopbackDiagnostics_NumberOfRepetitionsAttribute, 
	setWANATMF5LoopbackDiagnostics_NumberOfRepetitionsAttribute, 
	NULL
};

static NodeFunc Timeout=
{
	NULL,
	NULL,
	getWANATMF5LoopbackDiagnostics_TimeoutValue,
	setWANATMF5LoopbackDiagnostics_TimeoutValue,
	getWANATMF5LoopbackDiagnostics_TimeoutAttribute, 
	setWANATMF5LoopbackDiagnostics_TimeoutAttribute, 
	NULL
};

static NodeFunc SuccessCount=
{
	NULL,
	NULL,
	getWANATMF5LoopbackDiagnostics_SuccessCountValue,
	NULL,
	getWANATMF5LoopbackDiagnostics_SuccessCountAttribute, 
	setWANATMF5LoopbackDiagnostics_SuccessCountAttribute, 
	NULL
};

static NodeFunc FailureCount=
{
	NULL,
	NULL,
	getWANATMF5LoopbackDiagnostics_FailureCountValue,
	NULL,
	getWANATMF5LoopbackDiagnostics_FailureCountAttribute, 
	setWANATMF5LoopbackDiagnostics_FailureCountAttribute, 
	NULL
};

static NodeFunc AverageResponseTime=
{
	NULL,
	NULL,
	getWANATMF5LoopbackDiagnostics_AverageResponseTimeValue,
	NULL,
	getWANATMF5LoopbackDiagnostics_AverageResponseTimeAttribute, 
	setWANATMF5LoopbackDiagnostics_AverageResponseTimeAttribute, 
	NULL
};

static NodeFunc MinimumResponseTime=
{
	NULL,
	NULL,
	getWANATMF5LoopbackDiagnostics_MinimumResponseTimeValue,
	NULL,
	getWANATMF5LoopbackDiagnostics_MinimumResponseTimeAttribute, 
	setWANATMF5LoopbackDiagnostics_MinimumResponseTimeAttribute, 
	NULL
};

static NodeFunc MaximumResponseTime=
{
	NULL,
	NULL,
	getWANATMF5LoopbackDiagnostics_MaximumResponseTimeValue,
	NULL,
	getWANATMF5LoopbackDiagnostics_MaximumResponseTimeAttribute, 
	setWANATMF5LoopbackDiagnostics_MaximumResponseTimeAttribute, 
	NULL
};
/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANATMF5LoopbackDiagnostics.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
CwmpParameterStruct WANATMF5LoopbackDiagnostics[] =
{
	{"DiagnosticsState", StrW, NoLimite, &DiagnosticsState, NULL},
	{"NumberOfRepetitions", UnsignIntW, NoLimite, &NumberOfRepetitions, NULL},
	{"Timeout", UnsignIntW, NoLimite, &Timeout, NULL},
	{"SuccessCount", UnsignIntR, NoLimite, &SuccessCount, NULL},
	{"FailureCount", UnsignIntR, NoLimite, &FailureCount, NULL},
	{"AverageResponseTime", UnsignIntR, NoLimite, &AverageResponseTime, NULL},
	{"MinimumResponseTime", UnsignIntR, NoLimite, &MinimumResponseTime, NULL},
	{"MaximumResponseTime", UnsignIntR, NoLimite, &MaximumResponseTime, NULL},
	{NULL, -1, -1, NULL, NULL}
};

#endif

