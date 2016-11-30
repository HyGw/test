#include "Global_res.h"
#include "cp_ipoeEmulator.h"

/*******************  InternetGatewayDevice.X_CT-COM_IPoEDiagnostics. start ************************/

static NodeFunc IPoeEmulatorDiagnosticsState =
{
	NULL,
	NULL,
	getIPoeEmulatorDiagnosticsStateValue,
	setIPoeEmulatorDiagnosticsStateValue,
	getIPoeEmulatorDiagnosticsStateAttribute,
	setIPoeEmulatorDiagnosticsStateAttribute,
	NULL,
};

static NodeFunc IPoeEmulatorWANInterface =
{
	NULL,
	NULL,
	getIPoeEmulatorWANInterfaceValue,
	setIPoeEmulatorWANInterfaceValue,
	getIPoeEmulatorWANInterfaceAttribute,
	setIPoeEmulatorWANInterfaceAttribute,
	NULL,
};

static NodeFunc IPoeEmulatorUserMac =
{
	NULL,
	NULL,
	getIPoeEmulatorUserMacValue,
	setIPoeEmulatorUserMacValue,
	getIPoeEmulatorUserMacAttribute,
	setIPoeEmulatorUserMacAttribute,
	NULL,
};

static NodeFunc IPoeEmulatorVendorClassID =
{
	NULL,
	NULL,
	getIPoeEmulatorVendorClassIDValue,
	setIPoeEmulatorVendorClassIDValue,
	getIPoeEmulatorVendorClassIDAttribute,
	setIPoeEmulatorVendorClassIDAttribute,
	NULL,
};

static NodeFunc IPoeEmulatorResult =
{
	NULL,
	NULL,
	getIPoeEmulatorResultValue,
	NULL,
	getIPoeEmulatorResultAttribute,
	setIPoeEmulatorResultAttribute,
	NULL,
};

static NodeFunc IPoeEmulatorLocalAddress =
{
	NULL,
	NULL,
	getIPoeEmulatorLocalAddressValue,
	NULL,
	getIPoeEmulatorLocalAddressAttribute,
	setIPoeEmulatorLocalAddressAttribute,
	NULL,
};

static NodeFunc IPoeEmulatorDefaultGateway =
{
	NULL,
	NULL,
	getIPoeEmulatorDefaultGatewayValue,
	NULL,
	getIPoeEmulatorDefaultGatewayAttribute,
	setIPoeEmulatorDefaultGatewayAttribute,
	NULL,
};

static NodeFunc IPoeEmulatorPingDestIPAddress =
{
	NULL,
	NULL,
	getIPoeEmulatorPingDestIPAddressValue,
	setIPoeEmulatorPingDestIPAddressValue,
	getIPoeEmulatorPingDestIPAddressAttribute,
	setIPoeEmulatorPingDestIPAddressAttribute,
	NULL,
};

static NodeFunc IPoeEmulatorPingNumberOfRepetitions =
{
	NULL,
	NULL,
	getIPoeEmulatorPingNumberOfRepetitionsValue,
	setIPoeEmulatorPingNumberOfRepetitionsValue,
	getIPoeEmulatorPingNumberOfRepetitionsAttribute,
	setIPoeEmulatorPingNumberOfRepetitionsAttribute,
	NULL,
};

static NodeFunc IPoeEmulatorTimeout =
{
	NULL,
	NULL,
	getIPoeEmulatorTimeoutValue,
	setIPoeEmulatorTimeoutValue,
	getIPoeEmulatorTimeoutAttribute,
	setIPoeEmulatorTimeoutAttribute,
	NULL,
};

static NodeFunc IPoeEmulatorSuccessCount =
{
	NULL,
	NULL,
	getIPoeEmulatorSuccessCountValue,
	NULL,
	getIPoeEmulatorSuccessCountAttribute,
	setIPoeEmulatorSuccessCountAttribute,
	NULL,
};

static NodeFunc IPoeEmulatorFailureCount =
{
	NULL,
	NULL,
	getIPoeEmulatorFailureCountValue,
	NULL,
	getIPoeEmulatorFailureCountAttribute,
	setIPoeEmulatorFailureCountAttribute,
	NULL,
};

static NodeFunc IPoeEmulatorAverageResponseTime =
{
	NULL,
	NULL,
	getIPoeEmulatorAverageResponseTimeValue,
	NULL,
	getIPoeEmulatorAverageResponseTimeAttribute,
	setIPoeEmulatorAverageResponseTimeAttribute,
	NULL,
};

static NodeFunc IPoeEmulatorMinimumResponseTime =
{
	NULL,
	NULL,
	getIPoeEmulatorMinimumResponseTimeValue,
	NULL,
	getIPoeEmulatorMinimumResponseTimeAttribute,
	setIPoeEmulatorMinimumResponseTimeAttribute,
	NULL,
};

static NodeFunc IPoeEmulatorMaximumResponseTime =
{
	NULL,
	NULL,
	getIPoeEmulatorMaximumResponseTimeValue,
	NULL,
	getIPoeEmulatorMaximumResponseTimeAttribute,
	setIPoeEmulatorMaximumResponseTimeAttribute,
	NULL,
};


CwmpParameterStruct  IPoeEmulator[] =
{ 
	{IPOEEMULATOR_DIAGNOSTICS_STATE_STR, StrW, NoLimite, &IPoeEmulatorDiagnosticsState, NULL},
	{IPOEEMULATOR_WANINTERFACE_STR, StrW, NoLimite, &IPoeEmulatorWANInterface, NULL},
	{IPOEEMULATOR_USERMAC_STR, StrW, NoLimite, &IPoeEmulatorUserMac, NULL},
	{IPOEEMULATOR_VENDORCLASS_STR, StrW, NoLimite, &IPoeEmulatorVendorClassID, NULL},
	{IPOEEMULATOR_RESULT_STR, StrR, NoLimite, &IPoeEmulatorResult, NULL},
	{IPOEEMULATOR_LOCALADDRESS_STR, StrR, NoLimite, &IPoeEmulatorLocalAddress, NULL},
	{IPOEEMULATOR_DEFGATEWAY_STR, StrR, NoLimite, &IPoeEmulatorDefaultGateway, NULL},
	{IPOEEMULATOR_PINGDSTIP_STR, StrW, NoLimite, &IPoeEmulatorPingDestIPAddress, NULL},
	{IPOEEMULATOR_PINGNUM_STR, UnsignIntW, NoLimite, &IPoeEmulatorPingNumberOfRepetitions, NULL},
	{IPOEEMULATOR_PINGTIMEOUT_STR, UnsignIntW, NoLimite, &IPoeEmulatorTimeout, NULL},
	{IPOEEMULATOR_PINGSUCC_COUNT_STR, UnsignIntR, NoLimite, &IPoeEmulatorSuccessCount, NULL},
	{IPOEEMULATOR_PINGFAIL_COUNT_STR, UnsignIntR, NoLimite, &IPoeEmulatorFailureCount, NULL},
	{IPOEEMULATOR_PINGAVR_REPTIME_STR, UnsignIntR, NoLimite, &IPoeEmulatorAverageResponseTime, NULL},
	{IPOEEMULATOR_PINGMIN_REPTIME_STR, UnsignIntR, NoLimite, &IPoeEmulatorMinimumResponseTime, NULL},
	{IPOEEMULATOR_PINGMAX_REPTIME_STR, UnsignIntR, NoLimite, &IPoeEmulatorMaximumResponseTime, NULL},
	{NULL, -1, -1, NULL, NULL}
};
