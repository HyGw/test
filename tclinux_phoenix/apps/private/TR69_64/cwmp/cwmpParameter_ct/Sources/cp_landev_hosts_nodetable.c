#include "Global_res.h"
#include "cp_landev_hosts.h"

static NodeFunc HostNumberOfEntries=
{
	NULL,
	NULL,
	getHostNumberOfEntriesValue,
	NULL, 
	getHostNumberOfEntriesAttribute, 
	setHostNumberOfEntriesAttribute, 
	NULL
};

static NodeFunc HostFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitHostSubTree
};

static NodeFunc IPAddress=
{
	NULL,
	NULL,
	getIPAddressValue, 
	NULL, 
	getIPAddressAttribute, 
	setIPAddressAttribute, 
	NULL
};

static NodeFunc AddressSource=
{
	NULL,
	NULL,
	getAddressSourceValue, 
	NULL, 
	getAddressSourceAttribute, 
	setAddressSourceAttribute, 
	NULL
};

static NodeFunc LeaseTimeRemaining=
{
	NULL,
	NULL,
	getLeaseTimeRemainingValue, 
	NULL, 
	getLeaseTimeRemainingAttribute, 
	setLeaseTimeRemainingAttribute, 
	NULL
};

static NodeFunc MACAddress_host=
{
	NULL,
	NULL,
	getMACAddress_hostValue, 
	NULL, 
	getMACAddress_hostAttribute, 
	setMACAddress_hostAttribute, 
	NULL
};

static NodeFunc HostName=
{
	NULL,
	NULL,
	getHostNameValue, 
	NULL, 
	getHostNameAttribute, 
	setHostNameAttribute, 
	NULL
};

static NodeFunc InterfaceType=
{
	NULL,
	NULL,
	getInterfaceTypeValue, 
	NULL, 
	getInterfaceTypeAttribute, 
	setInterfaceTypeAttribute, 
	NULL
};

static NodeFunc Active_host=
{
	NULL,
	NULL,
	getActive_hostValue, 
	NULL, 
	getActive_hostAttribute, 
	setActive_hostAttribute, 
	NULL
};

static NodeFunc Layer2Interface=
{
	NULL,
	NULL,
	getLayer2InterfaceValue, 
	NULL, 
	getLayer2InterfaceAttribute, 
	setLayer2InterfaceAttribute, 
	NULL
};

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
static NodeFunc Hosttype=
{
	NULL,
	NULL,
	getHosttypeValue, 
	NULL, 
	getHosttypeAttribute, 
	setHosttypeAttribute, 
	NULL
};
#endif

/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.Hosts.Host.{i}
	Parent Node Table Location: cp_landev_hosts_nodetable.c
*/

CwmpParameterStruct Host[]= 
{
	{"IPAddress", StrR, NoLimite, &IPAddress, NULL},
	{"AddressSource", StrR, NoLimite, &AddressSource, NULL},
	{"LeaseTimeRemaining", IntR, NoLimite, &LeaseTimeRemaining, NULL},
	{"MACAddress", StrR, NoLimite, &MACAddress_host, NULL},
	{"HostName", StrR, NoLimite, &HostName, NULL},
	{"InterfaceType", StrR, NoLimite, &InterfaceType, NULL},
	{"Active", BooleanR, NoLimite, &Active_host, NULL},
	{"Layer2Interface", StrR, NoLimite, &Layer2Interface, NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CU"Hosttype", StrR, NoLimite, &Hosttype, NULL},
#endif

	{NULL, -1, -1, NULL, NULL}
};

/*
      This Node: InternetGatewayDevice.LANDevice.{i}.Hosts.{i}
	Parent Node: InternetGatewayDevice.LANDevice.{i}.Hosts.
	Parent Node Table Location: cp_landev_hosts_nodetable.c
*/

CwmpParameterStruct HostIndex[]= 
{
	{NULL, ObjectR, NoLimite, NULL, Host},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.
	Parent Node Table Location: cp_landev_nodetable.c
*/
CwmpParameterStruct Hosts[] = 
{
	{"HostNumberOfEntries", UnsignIntR, NoLimite, &HostNumberOfEntries, NULL},
	{"Host", ObjectR, NoLimite, &HostFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};

