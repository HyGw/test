#include "Global_res.h"
#include "cp_landev_routeradv.h"

static NodeFunc RouterAdvEnable=
{
	NULL,
	NULL,
	getRouterAdvEnableValue, 
	setRouterAdvEnableValue, 
	getRouterAdvEnableAttribute, 
	setRouterAdvEnableAttribute, 
	NULL,
};

static NodeFunc RouterAdvMaxRtrAdvInterval=
{
	NULL,
	NULL,
	getRouterAdvMaxRtrAdvIntervalValue, 
	setRouterAdvMaxRtrAdvIntervalValue, 
	getRouterAdvMaxRtrAdvIntervalAttribute, 
	setRouterAdvMaxRtrAdvIntervalAttribute, 
	NULL,
};

static NodeFunc RouterAdvMinRtrAdvInterval=
{
	NULL,
	NULL,
	getRouterAdvMinRtrAdvIntervalValue, 
	setRouterAdvMinRtrAdvIntervalValue, 
	getRouterAdvMinRtrAdvIntervalAttribute, 
	setRouterAdvMinRtrAdvIntervalAttribute, 
	NULL,
};

static NodeFunc RouterAdvAdvManagedFlag=
{
	NULL,
	NULL,
	getRouterAdvAdvManagedFlagValue, 
	setRouterAdvAdvManagedFlagValue, 
	getRouterAdvAdvManagedFlagAttribute, 
	setRouterAdvAdvManagedFlagAttribute, 
	NULL,
};

static NodeFunc RouterAdvAdvOtherConfigFlag=
{
	NULL,
	NULL,
	getRouterAdvAdvOtherConfigFlagValue, 
	setRouterAdvAdvOtherConfigFlagValue, 
	getRouterAdvAdvOtherConfigFlagAttribute, 
	setRouterAdvAdvOtherConfigFlagAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
extern CwmpParameterStruct LanIPv6Config_PrefixInfoIndex[];
#endif

CwmpParameterStruct Lan_X_CTCOM_RouterAdvertisement[] =
{
	{"Enable", BooleanW, NoLimite, &RouterAdvEnable, NULL},
	{"MaxRtrAdvInterval", UnsignIntW, NoLimite, &RouterAdvMaxRtrAdvInterval, NULL},
	{"MinRtrAdvInterval", UnsignIntW, NoLimite, &RouterAdvMinRtrAdvInterval, NULL},
	{"AdvManagedFlag", BooleanW, NoLimite, &RouterAdvAdvManagedFlag, NULL},
	{"AdvOtherConfigFlag", BooleanW, NoLimite, &RouterAdvAdvOtherConfigFlag, NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"prefix", ObjectR, NoLimite, NULL, LanIPv6Config_PrefixInfoIndex},
#endif
	{NULL, -1, -1, NULL, NULL}
};


