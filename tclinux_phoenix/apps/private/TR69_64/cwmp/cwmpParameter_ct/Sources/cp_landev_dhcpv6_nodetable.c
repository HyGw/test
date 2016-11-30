#include "Global_res.h"
#include "cp_landev_dhcpv6.h"
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#include "cp_landev.h"
#endif

static NodeFunc DHCPv6ServerEnable=
{
	NULL,
	NULL,
	getDHCPv6ServerEnableValue, 
	setDHCPv6ServerEnableValue, 
	getDHCPv6ServerEnableAttribute, 
	setDHCPv6ServerEnableAttribute, 
	NULL,
};

static NodeFunc DHCPv6ServerPoolEnable=
{
	NULL,
	NULL,
	getDHCPv6ServerPoolEnableValue, 
	setDHCPv6ServerPoolEnableValue, 
	getDHCPv6ServerPoolEnableAttribute, 
	setDHCPv6ServerPoolEnableAttribute, 
	NULL,
};

static NodeFunc DHCPv6ServerPoolMinAddress=
{
	NULL,
	NULL,
	getDHCPv6ServerPoolMinAddressValue, 
	setDHCPv6ServerPoolMinAddressValue, 
	getDHCPv6ServerPoolMinAddressAttribute, 
	setDHCPv6ServerPoolMinAddressAttribute, 
	NULL,
};

static NodeFunc DHCPv6ServerPoolMaxAddress=
{
	NULL,
	NULL,
	getDHCPv6ServerPoolMaxAddressValue, 
	setDHCPv6ServerPoolMaxAddressValue, 
	getDHCPv6ServerPoolMaxAddressAttribute, 
	setDHCPv6ServerPoolMaxAddressAttribute, 
	NULL,
};

static NodeFunc DHCPv6ServerPreferredLifeTime=
{
	NULL,
	NULL,
	getDHCPv6ServerPreferredLifeTimeValue, 
	setDHCPv6ServerPreferredLifeTimeValue, 
	getDHCPv6ServerPreferredLifeTimeAttribute, 
	setDHCPv6ServerPreferredLifeTimeAttribute, 
	NULL,
};

static NodeFunc DHCPv6ServerValidLifeTime=
{
	NULL,
	NULL,
	getDHCPv6ServerValidLifeTimeValue, 
	setDHCPv6ServerValidLifeTimeValue, 
	getDHCPv6ServerValidLifeTimeAttribute, 
	setDHCPv6ServerValidLifeTimeAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
static NodeFunc DHCPv6ServerDNSConfigType=
{
	NULL,
	NULL,
	getLanIPv6CfgDNSConfigTypeValue,
	setLanIPv6CfgDNSConfigTypeValue,
	getLanIPv6CfgDNSConfigTypeAttribute, 
	setLanIPv6CfgDNSConfigTypeAttribute, 
	NULL
};

static NodeFunc DHCPv6ServerDNSWANConnection=
{
	NULL,
	NULL,
	getLanIPv6CfgDNSWANConnectionValue,
	setLanIPv6CfgDNSWANConnectionValue,
	getLanIPv6CfgDNSWANConnectionAttribute, 
	setLanIPv6CfgDNSWANConnectionAttribute, 
	NULL
};

static NodeFunc DHCPv6ServerDNSServers=
{
	NULL,
	NULL,
	getLanIPv6CfgDNSServersValue,
	setLanIPv6CfgDNSServersValue,
	getLanIPv6CfgDNSServersAttribute, 
	setLanIPv6CfgDNSServersAttribute, 
	NULL
};

static NodeFunc DHCPv6ServerDomainName=
{
	NULL,
	NULL,
	getLanIPv6CfgDomainNameValue,
	setLanIPv6CfgDomainNameValue,
	getLanIPv6CfgDomainNameAttribute, 
	setLanIPv6CfgDomainNameAttribute, 
	NULL
};
#endif

CwmpParameterStruct Lan_X_CTCOM_DHCPv6Pool[] =
{
	{"Enable", BooleanW, NoLimite, &DHCPv6ServerPoolEnable, NULL},
	{"MinAddress", StrW, NoLimite, &DHCPv6ServerPoolMinAddress, NULL},
	{"MaxAddress", StrW, NoLimite, &DHCPv6ServerPoolMaxAddress, NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct Lan_X_CTCOM_DHCPv6Server[]=
{
	{"Enable", BooleanW, NoLimite, &DHCPv6ServerEnable, NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"IPv6DNSConfigType", StrW, NoLimite, &DHCPv6ServerDNSConfigType, NULL},
	{"IPv6DNSWANConnection", StrW, NoLimite, &DHCPv6ServerDNSWANConnection, NULL},
	{"IPv6DNSServers", StrW, NoLimite, &DHCPv6ServerDNSServers, NULL},
	{"DHCPv6LeaseTime", IntW, NoLimite, &DHCPv6ServerValidLifeTime, NULL},
	{"IPv6DomainName", StrW, NoLimite, &DHCPv6ServerDomainName, NULL},
#endif
	{"Pool", ObjectR, NoLimite,  NULL, Lan_X_CTCOM_DHCPv6Pool},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct Lan_X_CTCOM_DHCPv6[] =
{
	{"Server", ObjectR, NoLimite,  NULL, Lan_X_CTCOM_DHCPv6Server},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct LANDHCPv6Server[] =
{
	{"Enable", BooleanW, NoLimite, &DHCPv6ServerEnable, NULL},
	{"MinAddress", StrW, NoLimite, &DHCPv6ServerPoolMinAddress, NULL},
	{"MaxAddress", StrW, NoLimite, &DHCPv6ServerPoolMaxAddress, NULL},
	{"PreferredLifeTime", UnsignIntW, NoLimite, &DHCPv6ServerPreferredLifeTime, NULL},
	{"ValidLifeTime", UnsignIntW, NoLimite, &DHCPv6ServerValidLifeTime, NULL},
	{NULL, -1, -1, NULL, NULL}
};

