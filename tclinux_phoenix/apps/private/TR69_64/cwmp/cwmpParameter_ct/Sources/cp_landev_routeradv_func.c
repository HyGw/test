#include "Global_res.h"
#include "cp_landev_routeradv.h"

int8 getRouterAdvEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char enableValue[20] = {0};

	tcapi_get("Radvd_Entry","Enable", result);
	
	if(strcmp(result, "1") == 0)
		sprintf(enableValue, "%s", "1");
	else
		sprintf(enableValue, "%s", "0");
	
	return getTheValue(value, type, deviceNode, enableValue);
}

int8 setRouterAdvEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 enable = 0;
	char tmp[4] = {0};
	enable = *((uint8*)value);

	if(enable == 1)	
		sprintf(tmp,"%d",1);
	else if(enable == 0)
		sprintf(tmp,"%d",0);
	else
		return -1;
	
	tcapi_set("Radvd_Entry","Enable", tmp);
	
	SetCfgNodeFlag( "Radvd", -1 );

    return 0;
}

int8 getRouterAdvEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcctcomrouteradvAttr.aRouterAdvEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setRouterAdvEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcctcomrouteradvAttr.aRouterAdvEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= X_CT_COM_ROUTERADV_ATTR_FLAG;
	return result;
}

int8 getRouterAdvMaxRtrAdvIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	tcapi_get("Radvd_Entry","MaxRAInterval", result);
	
	return getTheValue(value, type, deviceNode, result);
}

int8 setRouterAdvMaxRtrAdvIntervalValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 interval;
	uint32 intervalMin;
	char tmp[10] = {0};
	interval = *((uint32*)value);

	if(interval < 4 || interval > 1800)
		return -1;

	tcapi_get("Radvd_Entry","MinRAInterval", tmp);
	intervalMin = atoi(tmp);

	if(intervalMin > interval*0.75)
		return -1;

	memset(tmp,0,sizeof(tmp));	

	sprintf(tmp, "%d", interval);
	
	tcapi_set("Radvd_Entry","MaxRAInterval", tmp);
	
	SetCfgNodeFlag( "Radvd", -1 );

    return 0;
}

int8 getRouterAdvMaxRtrAdvIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcctcomrouteradvAttr.aRouterAdvMaxInterval;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setRouterAdvMaxRtrAdvIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcctcomrouteradvAttr.aRouterAdvMaxInterval;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= X_CT_COM_ROUTERADV_ATTR_FLAG;
	return result;
}

int8 getRouterAdvMinRtrAdvIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	tcapi_get("Radvd_Entry","MinRAInterval", result);
	
	return getTheValue(value, type, deviceNode, result);

}

int8 setRouterAdvMinRtrAdvIntervalValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 interval;
	uint32 intervalMax;
	char tmp[10] = {0};
	interval = *((uint32*)value);

	if(interval < 3 || interval > 1350)
		return -1;

	tcapi_get("Radvd_Entry","MaxRAInterval", tmp);
	intervalMax = atoi(tmp);

	if(interval > intervalMax*0.75)
		return -1;

	memset(tmp,0,sizeof(tmp));
	
	sprintf(tmp, "%d", interval);
	
	tcapi_set("Radvd_Entry","MinRAInterval", tmp);
	
	SetCfgNodeFlag( "Radvd", -1 );

    return 0;

}

int8 getRouterAdvMinRtrAdvIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcctcomrouteradvAttr.aRouterAdvMinInterval;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setRouterAdvMinRtrAdvIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcctcomrouteradvAttr.aRouterAdvMinInterval;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= X_CT_COM_ROUTERADV_ATTR_FLAG;
	return result;
}

int8 getRouterAdvAdvManagedFlagValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char enableValue[20] = {0};

	tcapi_get("Radvd_Entry", "ManagedEnable", result);
	
	if(strcmp(result, "1") == 0)
		sprintf(enableValue, "%s", "1");
	else
		sprintf(enableValue, "%s", "0");
	
	return getTheValue(value, type, deviceNode, enableValue);
}

int8 setRouterAdvAdvManagedFlagValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 enable;
	char tmp[4] = {0};
	enable = *((uint8*)value);

	if(enable == 1)	
		sprintf(tmp,"%d",1);
	else if(enable == 0)
		sprintf(tmp,"%d",0);
	else
		return -1;
	
	tcapi_set("Radvd_Entry", "ManagedEnable", tmp);
	
	SetCfgNodeFlag( "Radvd", -1 );

    return 0;
}

int8 getRouterAdvAdvManagedFlagAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcctcomrouteradvAttr.aRouterAdvManageFlag;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setRouterAdvAdvManagedFlagAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcctcomrouteradvAttr.aRouterAdvManageFlag;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= X_CT_COM_ROUTERADV_ATTR_FLAG;
	return result;
}

int8 getRouterAdvAdvOtherConfigFlagValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char enableValue[20] = {0};

	tcapi_get("Radvd_Entry", "OtherEnable", result);
	
	if(strcmp(result, "1") == 0)
		sprintf(enableValue, "%s", "1");
	else
		sprintf(enableValue, "%s", "0");
	
	return getTheValue(value, type, deviceNode, enableValue);

}

int8 setRouterAdvAdvOtherConfigFlagValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 enable;
	char tmp[4] = {0};
	enable = *((uint8*)value);

	if(enable == 1)	
		sprintf(tmp,"%d",1);
	else if(enable == 0)
		sprintf(tmp,"%d",0);
	else
		return -1;
	
	tcapi_set("Radvd_Entry", "OtherEnable", tmp);
	
	SetCfgNodeFlag( "Radvd", -1 );

    return 0;

}

int8 getRouterAdvAdvOtherConfigFlagAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcctcomrouteradvAttr.aRouterAdvOtherFlag;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setRouterAdvAdvOtherConfigFlagAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcctcomrouteradvAttr.aRouterAdvOtherFlag;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= X_CT_COM_ROUTERADV_ATTR_FLAG;
	return result;
}


