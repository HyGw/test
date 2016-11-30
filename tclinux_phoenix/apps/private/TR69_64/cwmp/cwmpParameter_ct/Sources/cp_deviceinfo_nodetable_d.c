#include "Global_res.h"
#include "cp_deviceinfo_d.h"


static NodeFunc ManufacturerOUI_TestDynamic=
{
	NULL,
	NULL,
	getManufacturerOUI_TestDynamicValue, 
	NULL, 
	getManufacturerOUI_TestDynamicAttribute, 
	NULL, 
	NULL,
};

static NodeFunc SerialNumber_TestDynamic=
{
	NULL,
	NULL,
	getSerialNumber_TestDynamicValue, 
	NULL, 
	getSerialNumber_TestDynamicAttribute, 
	NULL, 
	NULL,
};

/*
	This Node:    InternetGatewayDevice.DeviceInfo.xxx
	Parent Node: InternetGatewayDevice.DeviceInfo.
*/
CwmpParameterStruct DeviceInfo_SUB_D[] = 
{
	{"ManufacturerOUI_TestDynamic", StrR, NoLimite, &ManufacturerOUI_TestDynamic, NULL},
	{"SerialNumber_TestDynamic", StrR, NoLimite, &SerialNumber_TestDynamic, NULL},
	{NULL, -1, -1, NULL, NULL}
};


/*
	This Node:    InternetGatewayDevice.DeviceInfo.
	Parent Node: InternetGatewayDevice.
*/
CwmpParameterStruct DeviceInfo_D[] = 
{
	{"DeviceInfo",ObjectR, NoLimite,NULL,DeviceInfo_SUB_D},
	{NULL, -1, -1, NULL, NULL}	
};
