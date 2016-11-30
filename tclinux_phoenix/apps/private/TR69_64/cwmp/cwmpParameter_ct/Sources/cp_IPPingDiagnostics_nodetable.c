#include "Global_res.h"
#include "cp_IPPingDiagnostics.h"

static NodeFunc DiagnosticsState=
{
	NULL,
	NULL,
	getDiagnosticsStateValue,
	setDiagnosticsStateValue, 
	getDiagnosticsStateAttribute, 
	setDiagnosticsStateAttribute, 
	NULL,
};

static NodeFunc Interface=
{
	NULL,
	NULL,
	getIPPingInterfaceValue,
	setIPPingInterfaceValue, 
	getIPPingInterfaceAttribute, 
	setIPPingInterfaceAttribute, 
	NULL,
};

static NodeFunc Host=
{
	NULL,
	NULL,
	getIPPingHostValue,
	setIPPingHostValue, 
	getIPPingHostAttribute, 
	setIPPingHostAttribute, 
	NULL,
};

static NodeFunc NumberOfRepetitions=
{
	NULL,
	NULL,
	getNumberOfRepetitionsValue,
	setNumberOfRepetitionsValue, 
	getNumberOfRepetitionsAttribute, 
	setNumberOfRepetitionsAttribute, 
	NULL,
};

static NodeFunc Timeout=
{
	NULL,
	NULL,
	getIPPingTimeoutValue,
	setIPPingTimeoutValue, 
	getIPPingTimeoutAttribute, 
	setIPPingTimeoutAttribute, 
	NULL,
};

static NodeFunc DataBlockSize=
{
	NULL,
	NULL,
	getDataBlockSizeValue,
	setDataBlockSizeValue, 
	getDataBlockSizeAttribute, 
	setDataBlockSizeAttribute, 
	NULL,
};

static NodeFunc DSCP=
{
	NULL,
	NULL,
	getDSCPValue,
	setDSCPValue, 
	getDSCPAttribute, 
	setDSCPAttribute, 
	NULL,
};

static NodeFunc SuccessCount=
{
	NULL,
	NULL,
	getSuccessCountValue,
	NULL, 
	getSuccessCountAttribute, 
	setSuccessCountAttribute, 
	NULL,
};

static NodeFunc FailureCount=
{
	NULL,
	NULL,
	getFailureCountValue,
	NULL, 
	getFailureCountAttribute, 
	setFailureCountAttribute, 
	NULL,
};

static NodeFunc AverageResponseTime=
{
	NULL,
	NULL,
	getAverageResponseTimeValue,
	NULL, 
	getAverageResponseTimeAttribute, 
	setAverageResponseTimeAttribute, 
	NULL,
};

static NodeFunc MinimumResponseTime=
{
	NULL,
	NULL,
	getMinimumResponseTimeValue,
	NULL, 
	getMinimumResponseTimeAttribute, 
	setMinimumResponseTimeAttribute, 
	NULL,
};

static NodeFunc MaximumResponseTime=
{
	NULL,
	NULL,
	getMaximumResponseTimeValue,
	NULL, 
	getMaximumResponseTimeAttribute, 
	setMaximumResponseTimeAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.IPPingDiagnostics.
	Parent Node Table Location: cp_topnodetable.c
*/
CwmpParameterStruct IPPingDiagnostics[] = 
{
	{"DiagnosticsState", StrW, NoLimite, &DiagnosticsState, NULL},	
	{"Interface", StrW, NoLimite, &Interface, NULL},
	{"Host", StrW, NoLimite, &Host, NULL},
	{"NumberOfRepetitions", UnsignIntW, NoLimite, &NumberOfRepetitions, NULL},
	{"Timeout", UnsignIntW, NoLimite, &Timeout, NULL},
	{"DataBlockSize", UnsignIntW, NoLimite, &DataBlockSize, NULL},
	{"DSCP", UnsignIntW, NoLimite, &DSCP, NULL},
	{"SuccessCount", UnsignIntR, NoLimite, &SuccessCount, NULL},
	{"FailureCount", UnsignIntR, NoLimite, &FailureCount, NULL},
	{"AverageResponseTime", UnsignIntR, NoLimite, &AverageResponseTime, NULL},
	{"MinimumResponseTime", UnsignIntR, NoLimite, &MinimumResponseTime, NULL},
	{"MaximumResponseTime", UnsignIntR, NoLimite, &MaximumResponseTime, NULL},
	{NULL, -1, -1, NULL, NULL}
};


