#include "Global_res.h"
#include "cp_devConfig.h"

static NodeFunc PersistentData=
{
	NULL,
	NULL,
	getPersistentDataValue,
	setPersistentDataValue, 
	getPersistentDataAttribute, 
	setPersistentDataAttribute, 
	NULL,
};

static NodeFunc ConfigFile=
{
	NULL,
	NULL,
	getConfigFileValue, 
	setConfigFileValue, 
	getConfigFileAttribute, 
	setConfigFileAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.DeviceConfig.
	Parent Node Table Location: cp_topnodetable.c
*/
CwmpParameterStruct DeviceConfig[] = 
{
	{"PersistentData", StrW, NoLimite, &PersistentData, NULL},	
	{"ConfigFile", StrW, NoLimite, &ConfigFile, NULL},

	{NULL, -1, -1, NULL, NULL}
};
