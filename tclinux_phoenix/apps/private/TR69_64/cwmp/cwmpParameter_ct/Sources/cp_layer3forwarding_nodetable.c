#include "Global_res.h"
#include "cp_layer3forwarding.h"

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
static NodeFunc StaticRoute=
{
	NULL,
	NULL,
	getForwarding1StaticRouteValue,
	NULL,
	getForwarding1StaticRouteAttribute, 
	setForwarding1StaticRouteAttribute, 
	NULL,
};
#endif

static NodeFunc Enable=
{
	NULL,
	NULL,
	getForwarding1EnableValue,
	setForwarding1EnableValue, 
	getForwarding1EnableAttribute, 
	setForwarding1EnableAttribute, 
	NULL,
};

static NodeFunc Status=
{
	NULL,
	NULL,
	getForwarding1StatusValue,
	NULL, 
	getForwarding1StatusAttribute, 
	setForwarding1StatusAttribute, 
	NULL,
};

static NodeFunc Type=
{
	NULL,
	NULL,
	getForwarding1TypeValue,
	setForwarding1TypeValue, 
	getForwarding1TypeAttribute, 
	setForwarding1TypeAttribute, 
	NULL,
};

static NodeFunc DestIPAddress=
{
	NULL,
	NULL,
	getForwarding1DestIPAddrValue,
	setForwarding1DestIPAddrValue, 
	getForwarding1DestIPAddrAttribute, 
	setForwarding1DestIPAddrAttribute, 
	NULL,
};

static NodeFunc DestSubnetMask=
{
	NULL,
	NULL,
	getForwarding1DestSubMaskValue,
	setForwarding1DestSubMaskValue, 
	getForwarding1DestSubMaskAttribute, 
	setForwarding1DestSubMaskAttribute, 
	NULL,
};

static NodeFunc SourceIPAddress=
{
	NULL,
	NULL,
	getForwarding1SrcIPAddrValue,
	setForwarding1SrcIPAddrValue, 
	getForwarding1SrcIPAddrAttribute, 
	setForwarding1SrcIPAddrAttribute, 
	NULL,
};

static NodeFunc SourceSubnetMask=
{
	NULL,
	NULL,
	getForwarding1SrcSubMaskValue,
	setForwarding1SrcSubMaskValue, 
	getForwarding1SrcSubMaskAttribute, 
	setForwarding1SrcSubMaskAttribute, 
	NULL,
};

static NodeFunc GatewayIPAddress=
{
	NULL,
	NULL,
	getForwarding1GtwIPAddrValue,
	setForwarding1GtwIPAddrValue, 
	getForwarding1GtwIPAddrAttribute, 
	setForwarding1GtwIPAddrAttribute, 
	NULL,
};

static NodeFunc ForwardingMetric=
{
	NULL,
	NULL,
	getForwarding1FwMetricValue,
	setForwarding1FwMetricValue, 
	getForwarding1FwMetricAttribute, 
	setForwarding1FwMetricAttribute, 
	NULL,
};

static NodeFunc Interface=
{
	NULL,
	NULL,
	getForwarding1IfaceValue,
	setForwarding1IfaceValue, 
	getForwarding1IfaceAttribute, 
	setForwarding1IfaceAttribute, 
	NULL,
};

static NodeFunc DefaultConnectionService=
{
	NULL,
	NULL,
	getDefaultConnectionServiceValue,
	setDefaultConnectionServiceValue, 
	getDefaultConnectionServiceAttribute, 
	setDefaultConnectionServiceAttribute, 
	NULL,
};

static NodeFunc ForwardNumberOfEntries=
{
	NULL,
	NULL,
	getForwardNumberOfEntriesValue,
	NULL, 
	getForwardNumberOfEntriesAttribute, 
	setForwardNumberOfEntriesAttribute, 
	NULL,
};

/*
	Parent Node:  InternetGatewayDevice.Layer3Forwarding.Forwarding.{i}.
	Parent Node Table Location: cp_layer3forwarding_nodetable.c
*/
CwmpParameterStruct Forwarding[] =
{
	{"Enable", BooleanW, NoLimite, &Enable, NULL},
#if !defined(TCSUPPORT_CMCC)
	{"Status", StrR, NoLimite, &Status, NULL},
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"StaticRoute", BooleanR, NoLimite, &StaticRoute, NULL},
#endif
#if !defined(TCSUPPORT_CMCC)
	{"Type", StrW, NoLimite, &Type, NULL},
#endif
	{"DestIPAddress", StrW, NoLimite, &DestIPAddress, NULL},
	{"DestSubnetMask", StrW, NoLimite, &DestSubnetMask, NULL},
#if !defined(TCSUPPORT_CMCC)
	{"SourceIPAddress", StrW, NoLimite, &SourceIPAddress, NULL},
	{"SourceSubnetMask", StrW, NoLimite, &SourceSubnetMask, NULL},
#endif
	{"GatewayIPAddress", StrW, NoLimite, &GatewayIPAddress, NULL},
	{"Interface", StrW, NoLimite, &Interface, NULL},
#if !defined(TCSUPPORT_CMCC)
	{"ForwardingMetric", IntW, NoLimite, &ForwardingMetric, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc ForwardingIndexFuncs=
{
	NULL,
	deleteForwardingObject,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL
};

/*
	Parent Node: InternetGatewayDevice.Layer3Forwarding.Forwarding.
	Parent Node Table Location:  cp_layer3forwarding_nodetable.c
*/
CwmpParameterStruct ForwardingIndex[]= 
{
	{NULL, ObjectW, NoLimite, &ForwardingIndexFuncs, Forwarding},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc ForwardingFuncs =
{
	addForwardingObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitForwardingTableFromFlash
};


/*
	Parent Node: InternetGatewayDevice.Layer3Forwarding.
	Parent Node Table Location: cp_topnodetable.c
*/
CwmpParameterStruct Layer3Forwarding[] = 
{
	{"DefaultConnectionService", StrW, NoLimite, &DefaultConnectionService, NULL},	
	{"ForwardNumberOfEntries", UnsignIntR, NoLimite, &ForwardNumberOfEntries, NULL},
	{"Forwarding", ObjectW, NoLimite, &ForwardingFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};

#if defined(TCSUPPORT_CMCC)
#ifdef TCSUPPORT_IPV6
static NodeFunc Enable6=
{
	NULL,
	NULL,
	getForwardingEnable6Value,
	setForwardingEnable6Value,
	getForwardingEnable6Attribute,
	setForwardingEnable6Attribute,
	NULL,
};

static NodeFunc Alias=
{
	NULL,
	NULL,
	getForwardingAliasValue,
	setForwardingAliasValue, 
	getForwardingAliasAttribute, 
	setForwardingAliasAttribute, 
	NULL,
};

static NodeFunc DestIPPrefix=
{
	NULL,
	NULL,
	getForwardingDestIPPrefixValue,
	setForwardingDestIPPrefixValue, 
	getForwardingDestIPPrefixAttribute, 
	setForwardingDestIPPrefixAttribute, 
	NULL,
};

static NodeFunc NextHop=
{
	NULL,
	NULL,
	getForwardingNextHopValue,
	setForwardingNextHopValue, 
	getForwardingNextHopAttribute, 
	setForwardingNextHopAttribute, 
	NULL,
};

static NodeFunc Interface6=
{
	NULL,
	NULL,
	getForwardingInterface6Value,
	setForwardingInterface6Value, 
	getForwardingInterface6Attribute, 
	setForwardingInterface6Attribute, 
	NULL,
};

/*
	Parent Node:  InternetGatewayDevice.IPv6Layer3Forwarding.Forwarding.{i}.
	Parent Node Table Location: cp_layer3forwarding_nodetable.c
*/
CwmpParameterStruct IPv6Forwarding[] =
{
	{"Enable", BooleanW, NoLimite, &Enable6, NULL},
	{"Alias", StrW, NoLimite, &Alias, NULL},
	{"DestIPPrefix", StrW, NoLimite, &DestIPPrefix, NULL},
	{"NextHop", StrW, NoLimite, &NextHop, NULL},
	{"Interface", StrW, NoLimite, &Interface6, NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc IPv6ForwardNumberOfEntries=
{
	NULL,
	NULL,
	getIPv6ForwardNumberOfEntriesValue,
	NULL,
	getIPv6ForwardNumberOfEntriesAttribute,
	setIPv6ForwardNumberOfEntriesAttribute,
	NULL,
};

static NodeFunc IPv6ForwardingIndexFuncs=
{
	NULL,
	deleteIPv6ForwardingObject,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

/*
	Parent Node: InternetGatewayDevice.IPv6Layer3Forwarding.Forwarding.
	Parent Node Table Location:  cp_layer3forwarding_nodetable.c
*/
CwmpParameterStruct IPv6ForwardingIndex[]= 
{
	{NULL, ObjectW, NoLimite, &IPv6ForwardingIndexFuncs, IPv6Forwarding},
	{NULL, -1, -1, NULL, NULL}
};


static NodeFunc IPv6ForwardingFuncs =
{
	addIPv6ForwardingObject,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	cwmpInitIPv6ForwardingTableFromFlash
};

/*
	Parent Node: InternetGatewayDevice.IPv6Layer3Forwarding.
	Parent Node Table Location: cp_topnodetable.c
*/
CwmpParameterStruct IPv6Layer3Forwarding[] = 
{
	{"ForwardNumberOfEntries", UnsignIntR, NoLimite, &IPv6ForwardNumberOfEntries, NULL},
	{"Forwarding", ObjectW, NoLimite, &IPv6ForwardingFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};
#endif
#endif

