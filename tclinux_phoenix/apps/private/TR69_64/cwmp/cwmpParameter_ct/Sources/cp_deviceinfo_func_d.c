#include "Global_res.h"
#include "cp_deviceinfo_d.h"

int8 getManufacturerOUI_TestDynamicValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[65] = {0};

	sprintf(tempValue, "ManufacturerOUI_TestDynamic");
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getManufacturerOUI_TestDynamicAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;

	result=0;
	return result;
}

int8 getSerialNumber_TestDynamicValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[65] = {0};

	sprintf(tempValue, "SerialNumber_TestDynamic");
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getSerialNumber_TestDynamicAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;

	result=1;
	return result;
}
