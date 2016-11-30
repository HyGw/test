#include "Global_res.h"
#include "cp_landev_ipInterface.h"

static NodeFunc IPInterface_Enable=
{
	NULL,
	NULL,
	getIPInterface_EnableValue,
	setIPInterface_EnableValue, 
	getIPInterface_EnableAttribute, 
	setIPInterface_EnableAttribute, 
	NULL,
};

static NodeFunc IPInterfaceIPAddress=
{
	NULL,
	NULL, 
	getIPInterfaceIPAddressValue,
	setIPInterfaceIPAddressValue, 
	getIPInterfaceIPAddressAttribute, 
	setIPInterfaceIPAddressAttribute, 
	NULL,
};

static NodeFunc IPInterfaceSubnetMask=
{
	NULL,
	NULL,
	getIPInterfaceSubnetMaskValue, 
	setIPInterfaceSubnetMaskValue, 
	getIPInterfaceSubnetMaskAttribute, 
	setIPInterfaceSubnetMaskAttribute, 
	NULL,
};

static NodeFunc IPInterfaceAddressingType=
{
	NULL,
	NULL,
	getIPInterfaceAddressingTypeValue, 
	setIPInterfaceAddressingTypeValue, 
	getIPInterfaceAddressingTypeAttribute, 
	setIPInterfaceAddressingTypeAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
static NodeFunc IPv6IPAddress=
{
	NULL,
	NULL,
	getCTComIPv6IPAddressValue, 
	setCTComIPv6IPAddressValue, 
	getCTComIPv6IPAddressAttribute, 
	setCTComIPv6IPAddressAttribute, 
	NULL,
};

CwmpParameterStruct CTComIPv6Address[]=
{
	{"IPAddress", StrW, NoLimite, &IPv6IPAddress, NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc CTComIPv6AddressIndexFuncs=
{
	NULL,
	cwmpDelObjectIPv6Address,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL
};

CwmpParameterStruct cwmpInitIPv6AddressIndex[]= 
{
	{NULL, ObjectW, NoLimite, &CTComIPv6AddressIndexFuncs, CTComIPv6Address},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc CTComIPv6AddressFuncs=
{
	cwmpAddObjectIPv6Address,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitIPv6AddressSubTree
};

CwmpParameterStruct Lan_X_CTCOM_IPv6[] =
{
	{"IPv6Address", ObjectW, NoLimite, &CTComIPv6AddressFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc LanIPv6LocalAddress=
{
	NULL,
	NULL,
	getLanIPv6LocalAddressValue, 
	setLanIPv6LocalAddressValue, 
	getLanIPv6LocalAddressAttribute, 
	setLanIPv6LocalAddressAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
static NodeFunc IPv6DelegatedAddressMode=
{
	NULL,
	NULL,
	getLanIPv6CfgRadvdPrefixModeValue,
	setLanIPv6CfgRadvdPrefixModeValue,
	getLanIPv6CfgRadvdPrefixModeAttribute, 
	setLanIPv6CfgRadvdPrefixModeAttribute, 
	NULL
};

static NodeFunc IPv6DelegatedWanConn=
{
	NULL,
	NULL,
	getLanIPv6CfgRadvdDelegatedWanConnValue,
	setLanIPv6CfgRadvdDelegatedWanConnValue,
	getLanIPv6CfgRadvdDelegatedWanConnAttribute, 
	setLanIPv6CfgRadvdDelegatedWanConnAttribute, 
	NULL
};

static NodeFunc IPv6InterfaceAddress=
{
	NULL,
	NULL,
	getIPv6InterfaceAddressValue,
	setIPv6InterfaceAddressValue,
	getIPv6InterfaceAddressAttribute, 
	setIPv6InterfaceAddressAttribute, 
	NULL
};

CwmpParameterStruct IPv6DelegatedAddress[] =
{
	{"Mode", StrW, NoLimite, &IPv6DelegatedAddressMode,  NULL},
	{"DelegatedConnection", StrW, NoLimite, &IPv6DelegatedWanConn, NULL},
	{"IPv6InterfaceAddress", StrW, NoLimite, &IPv6InterfaceAddress, NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct LanIPv6DeleAddrIdx[]= 
{
	{"1", ObjectR, NoLimite, NULL, IPv6DelegatedAddress},
	{NULL, -1, -1, NULL, NULL}
};
#endif


CwmpParameterStruct Lan_IPv6LocalAddress[] =
{
#if 0
	{"UniqueLocalAddress", StrW, NoLimite, &LanIPv6LocalAddress, NULL},
	{"DelegatedAddress", ObjectR, NoLimite,  NULL, LanIPv6DeleAddrIdx},
#endif
	{"LocalAddress", StrW, NoLimite, &LanIPv6LocalAddress, NULL},
	{NULL, -1, -1, NULL, NULL}
};

#endif


/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.Hosts.Host.{i}
	Parent Node Table Location: cp_landev_hosts_nodetable.c
*/

CwmpParameterStruct IPInterface[]= 
{
	{"Enable", BooleanW, NoLimite, &IPInterface_Enable, NULL},
	{"IPInterfaceIPAddress", StrW, NoLimite, &IPInterfaceIPAddress, NULL},
	{"IPInterfaceSubnetMask", StrW, NoLimite, &IPInterfaceSubnetMask, NULL},
	{"IPInterfaceAddressingType", StrW, NoLimite, &IPInterfaceAddressingType, NULL},
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CT_COM"IPv6", ObjectR, NoLimite,  NULL, Lan_X_CTCOM_IPv6},
#endif
	{X_CT_COM"IPv6Address", ObjectR, NoLimite,  NULL, Lan_IPv6LocalAddress},
#endif
	{NULL, -1, -1, NULL, NULL}
};

/*
      This Node: InternetGatewayDevice.LANDevice.{i}.Hosts.{i}
	Parent Node: InternetGatewayDevice.LANDevice.{i}.Hosts.
	Parent Node Table Location: cp_landev_hosts_nodetable.c
*/

CwmpParameterStruct IPInterfaceIndex[]= 
{
	{"1", ObjectR, NoLimite, NULL, IPInterface},
	{NULL, -1, -1, NULL, NULL}
};

