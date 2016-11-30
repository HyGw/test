#include "Global_res.h"
#include "cp_wandev_wandslConnMgmt.h"

#if !defined(TCSUPPORT_CT_PON)

static NodeFunc WANConnectionDevice=
{
	NULL,
	NULL,
	getWANConnectionDeviceValue,
	NULL, 
	getWANConnectionDeviceAttribute, 
	setWANConnectionDeviceAttribute, 
	NULL,
};

static NodeFunc WANConnectionService=
{
	NULL,
	NULL,
	getWANConnectionServiceValue,
	NULL, 
	getWANConnectionServiceAttribute, 
	setWANConnectionServiceAttribute, 
	NULL,
};

static NodeFunc DestinationAddress=
{
	NULL,
	NULL,
	getDestinationAddressValue,
	NULL, 
	getDestinationAddressAttribute, 
	setDestinationAddressAttribute, 
	NULL,
};

static NodeFunc LinkType=
{
	NULL,
	NULL,
	getLinkTypeValue,
	NULL, 
	getLinkTypeAttribute, 
	setLinkTypeAttribute, 
	NULL,
};

static NodeFunc ConnectionType=
{
	NULL,
	NULL,
	getConnectionTypeValue,
	NULL, 
	getConnectionTypeAttribute, 
	setConnectionTypeAttribute, 
	NULL,
};

static NodeFunc ConnSerNumbeOfEntries=
{
	NULL,
	NULL,
	getConnSerNumbeOfEntriesValue,
	NULL, 
	getConnSerNumbeOfEntriesAttribute, 
	setConnSerNumbeOfEntriesAttribute, 
	NULL,
};

static NodeFunc ConnectionServiceFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitConnectionServiceSubTree,
};

/*
      This Node: InternetGatewayDevice.WANDevice.1.WANDSLConnectionManagement.ConnectionService.{i}.
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLConnectionManagement.ConnectionService.
	Parent Node Table Location: cp_wandev_wandslConnMgmt_nodetable.c
*/

CwmpParameterStruct ConnectionService[]= 
{
	{"WANConnectionDevice", StrR, NoLimite, &WANConnectionDevice, NULL},
	{"WANConnectionService", StrR, NoLimite, &WANConnectionService, NULL},
	{"DestinationAddress", StrR, NoLimite, &DestinationAddress, NULL},
	{"LinkType", StrR, NoLimite, &LinkType, NULL},
	{"ConnectionType", StrR, NoLimite, &ConnectionType, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
      This Node: InternetGatewayDevice.WANDevice.1.WANDSLConnectionManagement.ConnectionService.
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLConnectionManagement.
	Parent Node Table Location: cp_wandev_wandslConnMgmt_nodetable.c
*/

CwmpParameterStruct ConnectionServiceIndex[]= 
{
	{NULL, ObjectR, NoLimite, NULL, ConnectionService},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLConnectionManagement.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
CwmpParameterStruct WANDSLConnectionManagement[] = 
{
	{"ConnectionServiceNumbeOfEntries", UnsignIntR, NoLimite, &ConnSerNumbeOfEntries, NULL},
	{"ConnectionService", ObjectR, NoLimite, &ConnectionServiceFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};

#endif

