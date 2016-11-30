#include "Global_res.h"
#include "ct_IPPingDiagnostics.h"

#if !defined(TCSUPPORT_C1_CUC)
static NodeFunc DiagnosticsState=
{
	NULL,
	NULL,
	getCTCOMDiagnosticsStateValue,
	setCTCOMDiagnosticsStateValue, 
	getCTCOMDiagnosticsStateAttribute, 
	setCTCOMDiagnosticsStateAttribute, 
	NULL,
};

static NodeFunc Interface=
{
	NULL,
	NULL,
	getCTCOMIPPingInterfaceValue,
	setCTCOMIPPingInterfaceValue, 
	getCTCOMIPPingInterfaceAttribute, 
	setCTCOMIPPingInterfaceAttribute, 
	NULL,
};

static NodeFunc Host=
{
	NULL,
	NULL,
	getCTCOMIPPingHostValue,
	setCTCOMIPPingHostValue, 
	getCTCOMIPPingHostAttribute, 
	setCTCOMIPPingHostAttribute, 
	NULL,
};

static NodeFunc NumberOfRepetitions=
{
	NULL,
	NULL,
	getCTCOMNumberOfRepetitionsValue,
	setCTCOMNumberOfRepetitionsValue, 
	getCTCOMNumberOfRepetitionsAttribute, 
	setCTCOMNumberOfRepetitionsAttribute, 
	NULL,
};

static NodeFunc Timeout=
{
	NULL,
	NULL,
	getCTCOMIPPingTimeoutValue,
	setCTCOMIPPingTimeoutValue, 
	getCTCOMIPPingTimeoutAttribute, 
	setCTCOMIPPingTimeoutAttribute, 
	NULL,
};

static NodeFunc DataBlockSize=
{
	NULL,
	NULL,
	getCTCOMDataBlockSizeValue,
	setCTCOMDataBlockSizeValue, 
	getCTCOMDataBlockSizeAttribute, 
	setCTCOMDataBlockSizeAttribute, 
	NULL,
};

static NodeFunc DSCP=
{
	NULL,
	NULL,
	getCTCOMDSCPValue,
	setCTCOMDSCPValue, 
	getCTCOMDSCPAttribute, 
	setCTCOMDSCPAttribute, 
	NULL,
};

static NodeFunc Interval=
{
	NULL,
	NULL,
	getCTCOMIntervalValue,
	setCTCOMIntervalValue, 
	getCTCOMIntervalAttribute, 
	setCTCOMIntervalAttribute, 
	NULL,
};

static NodeFunc Stop=
{
	NULL,
	NULL,
	getCTCOMStopValue,
	setCTCOMStopValue, 
	getCTCOMStopAttribute, 
	setCTCOMStopAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_Ping.PingConfig.{i}.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct PingConfig[] = 
{
	{"DiagnosticsState", StrW, NoLimite, &DiagnosticsState, NULL},	
	{"Interface", StrW, NoLimite, &Interface, NULL},
	{"Host", StrW, NoLimite, &Host, NULL},
	{"NumberOfRepetitions", UnsignIntW, NoLimite, &NumberOfRepetitions, NULL},
	{"Timeout", UnsignIntW, NoLimite, &Timeout, NULL},
	{"DataBlockSize", UnsignIntW, NoLimite, &DataBlockSize, NULL},
	{"DSCP", UnsignIntW, NoLimite, &DSCP, NULL},
	{"Interval", UnsignIntW, NoLimite, &Interval, NULL},
	{"Stop", BooleanW, NoLimite, &Stop, NULL},
	{NULL, -1, -1, NULL, NULL}
};
#endif


