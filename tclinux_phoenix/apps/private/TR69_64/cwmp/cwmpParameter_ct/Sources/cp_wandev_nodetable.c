#include "Global_res.h"
#include "cp_wandev.h"

static NodeFunc WANConnectionDeviceFuncs =
{
	DeviceNodeAddPVCObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitWANConnectionDeviceTable
};

#if defined(TCSUPPORT_CT_PON)
static NodeFunc WANDeviceFuncs=
{
	NULL,
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	cwmpInitWANDeviceTable,
};
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
static NodeFunc X_CTCOMWANIndex=
{
	NULL,
	NULL,
	getX_CTCOMWANIndexValue, 
	NULL, 
	getX_CTCOMWANIndexAttribute, 
	setX_CTCOMWANIndexAttribute, 
	NULL,
};
#endif


/*
	Parent Node: InternetGatewayDevice.WANDevice.1.
	Parent Node Table Location: cp_wandev_nodetable.c 
*/
CwmpParameterStruct WANDevice[] =
{
/*	//all sub node under WANDeviceNode
	{"WANConnectionNumberOfEntries", UnsignIntR, NoLimite, &WANConnectionNumberOfEntries, NULL},
	{"WANCommonInterfaceConfig", ObjectR, NoLimite, NULL, WANCommonInterfaceConfig},
	{"WANDSLInterfaceConfig", ObjectR, NoLimite, NULL, WANDSLInterfaceConfig},
	{"WANEthernetInterfaceConfig", ObjectR, NoLimite, NULL, WANEthernetInterfaceConfig},
	{"WANDSLConnectionManagement", ObjectR, NoLimite, NULL, WANDSLConnectionManagement},
*/	
#if !defined(TCSUPPORT_CT_PON)
	{"WANDSLDiagnostics", ObjectR, NoLimite, NULL, WANDSLDiagnostics},
	{"WANDSLInterfaceConfig", ObjectR, NoLimite, NULL, WANDSLInterfaceConfig},
	{"WANDSLConnectionManagement", ObjectR, NoLimite, NULL, WANDSLConnectionManagement},
#endif
	{"WANCommonInterfaceConfig", ObjectR, NoLimite, NULL, WANCommonInterfaceConfig},
	{"WANConnectionDevice", ObjectW, NoLimite, &WANConnectionDeviceFuncs, NULL},
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	{X_CT_COM"WANIndex", StrR, NoLimite, &X_CTCOMWANIndex, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};

#if defined(TCSUPPORT_CT_PON)
CwmpParameterStruct EponInterfaceConfig[] =
{
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CU"WANEPONInterfaceConfig", ObjectR, NoLimite, NULL, WANEPONInterfaceConfig},
#else
	{X_CT_COM"EponInterfaceConfig", ObjectR, NoLimite, NULL, WANEPONInterfaceConfig},
#if defined(TCSUPPORT_CT_FJ)
	{X_CT_COM"EponInterafceConfig", ObjectR, NoLimite, NULL, WANEPONInterfaceConfig},
#endif
#endif
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct GponInterfaceConfig[] =
{
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CU"WANGPONInterfaceConfig", ObjectR, NoLimite, NULL, WANGPONInterfaceConfig},
#else
	{X_CT_COM"GponInterfaceConfig", ObjectR, NoLimite, NULL, WANGPONInterfaceConfig},
#endif
	{NULL, -1, -1, NULL, NULL}
};
#endif



/*
	Parent Node: InternetGatewayDevice.WANDevice.
	Parent Node Table Location: cp_topnodetable.c 
*/
CwmpParameterStruct WANDeviceIndex[] =
{
#if defined(TCSUPPORT_CT_PON)
	{"1", ObjectR, NoLimite, &WANDeviceFuncs, NULL},
#else
	{"1", ObjectR, NoLimite, NULL, WANDevice},
#endif
	{NULL, -1, -1, NULL, NULL}
};
