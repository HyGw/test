#include "Global_res.h"
#include "cp_pppoeEmulator.h"

/*******************  InternetGatewayDevice.UDPEchoConfig. end ************************/

static NodeFunc PppoeEmulatorDiagnosticsState =
{
	NULL,
	NULL,
	getPppoeEmulatorDiagnosticsStateValue, 
	setPppoeEmulatorDiagnosticsStateValue, 
	getPppoeEmulatorDiagnosticsStateAttribute, 
	setPppoeEmulatorDiagnosticsStateAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorUsername =
{
	NULL,
	NULL,
	getPppoeEmulatorUsernameValue, 
	setPppoeEmulatorUsernameValue, 
	getPppoeEmulatorUsernameAttribute, 
	setPppoeEmulatorUsernameAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorPassword =
{
	NULL,
	NULL,
	getPppoeEmulatorPasswordValue, 
	setPppoeEmulatorPasswordValue, 
	getPppoeEmulatorPasswordAttribute, 
	setPppoeEmulatorPasswordAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorWanInterface =
{
	NULL,
	NULL,
	getPppoeEmulatorWanInterfaceValue, 
	setPppoeEmulatorWanInterfaceValue, 
	getPppoeEmulatorWanInterfaceAttribute, 
	setPppoeEmulatorWanInterfaceAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorPppAuthProto =
{
	NULL,
	NULL, 
	getPppoeEmulatorPppAuthProtoValue, 
	setPppoeEmulatorPppAuthProtoValue, 
	getPppoeEmulatorPppAuthProtoAttribute, 
	setPppoeEmulatorPppAuthProtoAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorRetryTimes =
{
	NULL,
	NULL,
	getPppoeEmulatorRetryTimesValue, 
	setPppoeEmulatorRetryTimesValue, 
	getPppoeEmulatorRetryTimesAttribute, 
	setPppoeEmulatorRetryTimesAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorResult =
{
	NULL,
	NULL,
	getPppoeEmulatorResultValue, 
	NULL,
	getPppoeEmulatorResultAttribute, 
	setPppoeEmulatorResultAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorPppSessionID =
{
	NULL,
	NULL,
	getPppoeEmulatorPppSessionIDValue, 
	NULL,
	getPppoeEmulatorPppSessionIDAttribute, 
	setPppoeEmulatorPppSessionIDAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorExternalIP =
{
	NULL,
	NULL,
	getPppoeEmulatorExternalIPValue, 
	NULL,
	getPppoeEmulatorExternalIPAttribute, 
	setPppoeEmulatorExternalIPAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorDefGw =
{
	NULL,
	NULL,
	getPppoeEmulatorDefGwValue, 
	NULL,
	getPppoeEmulatorDefGwAttribute, 
	setPppoeEmulatorDefGwAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
static NodeFunc PppoeEmulatorErrCode =
{
	NULL,
	NULL,
	getPppoeEmulatorErrCodeValue, 
	NULL,
	getPppoeEmulatorErrCodeAttribute, 
	setPppoeEmulatorErrCodeAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorWanIPMode =
{
	NULL,
	NULL,
	getPppoeEmulatorWanIPModeValue, 
	NULL,
	getPppoeEmulatorWanIPModeAttribute, 
	setPppoeEmulatorWanIPModeAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorDsliteEnable =
{
	NULL,
	NULL,
	getPppoeEmulatorDsliteEnableValue, 
	NULL,
	getPppoeEmulatorDsliteEnableAttribute, 
	setPppoeEmulatorDsliteEnableAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorAftrAddress =
{
	NULL,
	NULL,
	getPppoeEmulatorAftrAddressValue, 
	NULL,
	getPppoeEmulatorAftrAddressAttribute, 
	setPppoeEmulatorAftrAddressAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorWANIPv6Address =
{
	NULL,
	NULL,
	getPppoeEmulatorWANIPv6AddressValue, 
	NULL,
	getPppoeEmulatorWANIPv6AddressAttribute, 
	setPppoeEmulatorWANIPv6AddressAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorWANIPv6DNSAddress =
{
	NULL,
	NULL,
	getPppoeEmulatorWANIPv6DNSAddressValue, 
	NULL,
	getPppoeEmulatorWANIPv6DNSAddressAttribute, 
	setPppoeEmulatorWANIPv6DNSAddressAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorWANIPv6Prefix =
{
	NULL,
	NULL,
	getPppoeEmulatorWANIPv6PrefixValue, 
	NULL,
	getPppoeEmulatorWANIPv6PrefixAttribute, 
	setPppoeEmulatorWANIPv6PrefixAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorWANDefIPv6Gateway =
{
	NULL,
	NULL,
	getPppoeEmulatorWANDefIPv6GatewayValue, 
	NULL,
	getPppoeEmulatorWANDefIPv6GatewayAttribute, 
	setPppoeEmulatorWANDefIPv6GatewayAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorLANIPv6Prefix =
{
	NULL,
	NULL,
	getPppoeEmulatorLANIPv6PrefixValue, 
	NULL,
	getPppoeEmulatorLANIPv6PrefixAttribute, 
	setPppoeEmulatorLANIPv6PrefixAttribute, 
	NULL,
};
#endif

CwmpParameterStruct  PppoeEmulator[] =
{ 
	{PPPOEEMULATOR_DIAGNOSTICS_STATE_STR, StrW, NoLimite, &PppoeEmulatorDiagnosticsState, NULL},
	{PPPOEEMULATOR_USERNAME_STR, StrW, NoLimite, &PppoeEmulatorUsername, NULL},
	{PPPOEEMULATOR_PASSWORD_STR, StrW, NoLimite, &PppoeEmulatorPassword, NULL},
	{PPPOEEMULATOR_WANINTERFACE_STR, StrW, NoLimite, &PppoeEmulatorWanInterface, NULL},
#if !defined(TCSUPPORT_C7)
	{PPPOEEMULATOR_PPPAUTHPROTO_STR, StrW, NoLimite, &PppoeEmulatorPppAuthProto, NULL},
	{PPPOEEMULATOR_RETRYTIMES_STR, UnsignIntW, NoLimite, &PppoeEmulatorRetryTimes, NULL},
#endif
	{PPPOEEMULATOR_RESULT_STR, StrR, NoLimite, &PppoeEmulatorResult, NULL},
	{PPPOEEMULATOR_PPPSESSIONID_STR, UnsignIntR, NoLimite, &PppoeEmulatorPppSessionID, NULL},
	{PPPOEEMULATOR_EXTERNALIPADDRESS_STR, StrR, NoLimite, &PppoeEmulatorExternalIP, NULL},
	{PPPOEEMULATOR_DEFAULTGATEWAY_STR, StrR, NoLimite, &PppoeEmulatorDefGw, NULL},
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
	{PPPOEEMULATOR_ERRORCODE_NODESTR, StrR, NoLimite, &PppoeEmulatorErrCode, NULL},
#if !defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
	{PPPOEEMULATOR_WANIPMODE_STR, UnsignIntR, NoLimite, &PppoeEmulatorWanIPMode, NULL},
	{PPPOEEMULATOR_DSLITEENABLE_STR, BooleanR, NoLimite, &PppoeEmulatorDsliteEnable, NULL},
#endif
	{PPPOEEMULATOR_AFTRADDRESS_STR, StrR, NoLimite, &PppoeEmulatorAftrAddress, NULL},
	{PPPOEEMULATOR_WANIPV6ADDRESS_STR, StrR, NoLimite, &PppoeEmulatorWANIPv6Address, NULL},
	{PPPOEEMULATOR_WANIPV6DNSADDRESS_STR, StrR, NoLimite, &PppoeEmulatorWANIPv6DNSAddress, NULL},
	{PPPOEEMULATOR_WANIPV6PREFIX_STR, StrR, NoLimite, &PppoeEmulatorWANIPv6Prefix, NULL},
	{PPPOEEMULATOR_WANDEFIPV6GATEWAY_STR, StrR, NoLimite, &PppoeEmulatorWANDefIPv6Gateway, NULL},
	{PPPOEEMULATOR_LANIPV6PREFIX_STR, StrR, NoLimite, &PppoeEmulatorLANIPv6Prefix, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}	
};

static NodeFunc PppoeEmulatorBridgeEnable =
{
	NULL,
	NULL,
	getPppoeEmulatorBridgeEnableValue, 
	setPppoeEmulatorBridgeEnableValue, 
	getPppoeEmulatorBridgeEnableAttribute, 
	setPppoeEmulatorBridgeEnableAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorBridgeUsername =
{
	NULL,
	NULL,
	getPppoeEmulatorBridgeUsernameValue, 
	setPppoeEmulatorBridgeUsernameValue, 
	getPppoeEmulatorBridgeUsernameAttribute, 
	setPppoeEmulatorBridgeUsernameAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorBridgePassword =
{
	NULL,
	NULL,
	getPppoeEmulatorBridgePasswordValue, 
	setPppoeEmulatorBridgePasswordValue, 
	getPppoeEmulatorBridgePasswordAttribute, 
	setPppoeEmulatorBridgePasswordAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorBridgeTimeList =
{
	NULL,
	NULL,
	getPppoeEmulatorBridgeTimeListValue, 
	setPppoeEmulatorBridgeTimeListValue, 
	getPppoeEmulatorBridgeTimeListAttribute, 
	setPppoeEmulatorBridgeTimeListAttribute, 
	NULL,
};

static NodeFunc PppoeEmulatorBridgeFailRetryTimeList =
{
	NULL,
	NULL,
	getPppoeEmulatorBridgeFailRetryTimeListValue, 
	setPppoeEmulatorBridgeFailRetryTimeListValue, 
	getPppoeEmulatorBridgeFailRetryTimeListAttribute, 
	setPppoeEmulatorBridgeFailRetryTimeListAttribute, 
	NULL,
};

CwmpParameterStruct  PppoeEmulatorBridge[] =
{ 
	{PPPOEEMULATOR_BRIDGE_ENABLE_STR, BooleanW, NoLimite, &PppoeEmulatorBridgeEnable, NULL},
	{PPPOEEMULATOR_BRIDGE_USERNAME_STR, StrW, NoLimite, &PppoeEmulatorBridgeUsername, NULL},
	{PPPOEEMULATOR_BRIDGE_PASSWORD_STR, StrW, NoLimite, &PppoeEmulatorBridgePassword, NULL},
	{PPPOEEMULATOR_BRIDGE_TIMELIST_STR, StrW, NoLimite, &PppoeEmulatorBridgeTimeList, NULL},
	{PPPOEEMULATOR_BRIDGE_FAILRETRYTIMELIST_STR, StrW, NoLimite, &PppoeEmulatorBridgeFailRetryTimeList, NULL},
	{NULL, -1, -1, NULL, NULL}	
};

/*******************  InternetGatewayDevice.X_CT-COM_PPPOE_EMULATOR. end ************************/
