#include "Global_res.h"
#include "cp_wandev_wanconndev.h"


static NodeFunc WANIPConnectionNumberOfEntries=
{
	NULL,
	NULL,
	getWANIPCntNumOfEntries1_1Value,
	NULL,
	getWANIPCntNumOfEntries1_1Attribute, 
	setWANIPCntNumOfEntries1_1Attribute, 
	NULL
};

static NodeFunc WANPPPConnectionNumberOfEntries=
{
	NULL,
	NULL,
	getWANPPPCntNumOfEntries1_1Value,
	NULL,
	getWANPPPCntNumOfEntries1_1Attribute, 
	setWANPPPCntNumOfEntries1_1Attribute, 
	NULL
};

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
static NodeFunc WANVLANEnabled=
{
	NULL,
	NULL,
	getWANVLANEnabledValue,
	setWANVLANEnabledValue,
	getWANVLANEnabledAttribute,
	setWANVLANEnabledAttribute,
	NULL
};

static NodeFunc WANVLAN=
{
	NULL,
	NULL,
	getWANVLANValue,
	setWANVLANValue,
	getWANVLANAttribute,
	setWANVLANAttribute,
	NULL
};

static NodeFunc WAN8021p=
{
	NULL,
	NULL,
	getWAN8021PValue,
	setWAN8021PValue,
	getWAN8021PAttribute,
	setWAN8021PAttribute,
	NULL
};

#endif


/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
static NodeFunc WANConnectionDeviceIndexFuncs=
{
	NULL,
	DeviceNodeDelPVCObject,
	NULL,
	NULL,
	NULL, 
	NULL, 
#if defined(TCSUPPORT_CT_PON)
	cwmpInitWANConnectionObjectSubtree
#else
	NULL
#endif
};

static NodeFunc WANIPConnectionFuncs=
{
	cwmpAddIPConnectionObject,
	NULL,
	NULL,
	NULL,
	NULL, 
	NULL, 
	cwmpInitIPConnectionObjectSubtree
};


static NodeFunc WANPPPConnectionFuncs=
{
	cwmpAddPPPConnectionObject,
	NULL,
	NULL,
	NULL,
	NULL, 
	NULL, 
	cwmpInitPPPConnectionObjectSubtree
};
/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
CwmpParameterStruct WANConnectionDevice_table[] =
{
	{"WANIPConnectionNumberOfEntries", UnsignIntR, NoLimite, &WANIPConnectionNumberOfEntries, NULL},
	{"WANPPPConnectionNumberOfEntries", UnsignIntR, NoLimite, &WANPPPConnectionNumberOfEntries, NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
  {"X_CU_VLANEnabled",BooleanW,NoLimite,&WANVLANEnabled},
  {"X_CU_VLAN",UnsignIntW,NoLimite,&WANVLAN},
  {"X_CU_802_1p",UnsignIntW,NoLimite,&WAN8021p},
#endif
#if !defined(TCSUPPORT_CT_PON)
	{"WANDSLLinkConfig", ObjectR, NoLimite, NULL, WANDSLLinkConfig},
	{"WANATMF5LoopbackDiagnostics", ObjectR, NoLimite, NULL, &WANATMF5LoopbackDiagnostics},
#endif
	{"WANIPConnection", ObjectW, NoLimite, &WANIPConnectionFuncs, NULL},
	{"WANPPPConnection", ObjectW, NoLimite, &WANPPPConnectionFuncs, NULL},
#if defined(TCSUPPORT_CT_WAN_PTM)
	{X_CT_COM"WANVdslLinkConfig", ObjectR, NoLimite, NULL, WANVdslLinkConfig},
	{"WANPTMLinkConfig", ObjectR, NoLimite, NULL, WANPTMLinkConfig},
#endif
	{NULL, -1, -1, NULL, NULL}
};

#if defined(TCSUPPORT_CT_PON)
CwmpParameterStruct WANEponLinkConfigNode[] =
{
	{X_CT_COM"WANEponLinkConfig", ObjectR, NoLimite, NULL, WANEPonLinkConfig},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct WANGponLinkConfigNode[] =
{
	{X_CT_COM"WANGponLinkConfig", ObjectR, NoLimite, NULL, WANGEPonLinkConfig},
	{NULL, -1, -1, NULL, NULL}
};
#endif


static NodeFunc WANIPConnectionIndexFuncs=
{
	NULL,
	cwmpDeleteConnectionObject,
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL
};

/*
	This Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANIPConnection.1.
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANIPConnection.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
CwmpParameterStruct WANIPConnectionIndex[] =
{
	{NULL, ObjectW, NoLimite, &WANIPConnectionIndexFuncs, WANIPConnection},
	{NULL, -1, -1, NULL, NULL}
};



/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANPPPConnection.
	Parent Node Table Location: cp_wandev_nodetable.c 
*/
CwmpParameterStruct WANPPPConnectionIndex[] =
{
	{NULL, ObjectW, NoLimite, &WANIPConnectionIndexFuncs, WANPPPConnection},
	{NULL, -1, -1, NULL, NULL}
};

/*

	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
CwmpParameterStruct WANConnectionDeviceIndex[] = 
{
#if defined(TCSUPPORT_CT_PON)
	{NULL,ObjectW, NoLimite, &WANConnectionDeviceIndexFuncs, NULL},
#else
	{NULL,ObjectW, NoLimite, &WANConnectionDeviceIndexFuncs, WANConnectionDevice_table},
#endif
	{NULL, -1, -1, NULL, NULL}
};


