#include "Global_res.h"
#include "cp_landev_dataspeedlimit.h"

static NodeFunc SpeedLimitModeUP=
{
	NULL,
	NULL,
	getSpeedLimitModeUPValue, 
	setSpeedLimitModeUPValue, 
	getSpeedLimitModeUPAttribute, 
	setSpeedLimitModeUPAttribute, 
	NULL,
};

static NodeFunc SpeedLimitModeDOWN=
{
	NULL,
	NULL,
	getSpeedLimitModeDOWNValue, 
	setSpeedLimitModeDOWNValue, 
	getSpeedLimitModeDOWNAttribute, 
	setSpeedLimitModeDOWNAttribute, 
	NULL,
};

static NodeFunc InterfaceLimitUP=
{
	NULL,
	NULL,
	getInterfaceLimitUPValue,
	setInterfaceLimitUPValue,
	getInterfaceLimitUPAttribute,
	setInterfaceLimitUPAttribute,
	NULL,
};

static NodeFunc InterfaceLimitDOWN=
{
	NULL,
	NULL,
	getInterfaceLimitDOWNValue,
	setInterfaceLimitDOWNValue,
	getInterfaceLimitDOWNAttribute,
	setInterfaceLimitDOWNAttribute,
	NULL,
};

static NodeFunc VlanTagLimitUP=
{
	NULL,
	NULL,
	getVlanTagLimitUPValue,
	setVlanTagLimitUPValue,
	getVlanTagLimitUPAttribute,
	setVlanTagLimitUPAttribute,
	NULL,
};

static NodeFunc VlanTagLimitDOWN=
{
	NULL,
	NULL,
	getVlanTagLimitDOWNValue,
	setVlanTagLimitDOWNValue,
	getVlanTagLimitDOWNAttribute,
	setVlanTagLimitDOWNAttribute,
	NULL,
};

static NodeFunc IPLimitUP=
{
	NULL,
	NULL,
	getIPLimitUPValue,
	setIPLimitUPValue,
	getIPLimitUPAttribute,
	setIPLimitUPAttribute,
	NULL,
};

static NodeFunc IPLimitDOWN=
{
	NULL,
	NULL,
	getIPLimitDOWNValue,
	setIPLimitDOWNValue,
	getIPLimitDOWNAttribute,
	setIPLimitDOWNAttribute,
	NULL,
};

CwmpParameterStruct LANDataSpeedLimit[] =
{
	{"SpeedLimitMode_UP", IntW, NoLimite, &SpeedLimitModeUP, NULL},
	{"SpeedLimitMode_DOWN", IntW, NoLimite, &SpeedLimitModeDOWN, NULL},
	{"InterfaceLimit_UP", StrW, NoLimite, &InterfaceLimitUP, NULL},
	{"InterfaceLimit_DOWN", StrW, NoLimite, &InterfaceLimitDOWN, NULL},
	{"VlanTagLimit_UP", StrW, NoLimite, &VlanTagLimitUP, NULL},
	{"VlanTagLimit_DOWN", StrW, NoLimite, &VlanTagLimitDOWN, NULL},
	{"IPLimit_UP", StrW, NoLimite, &IPLimitUP, NULL},
	{"IPLimit_DOWN", StrW, NoLimite, &IPLimitDOWN, NULL},
	{NULL, -1, -1, NULL, NULL}
};

