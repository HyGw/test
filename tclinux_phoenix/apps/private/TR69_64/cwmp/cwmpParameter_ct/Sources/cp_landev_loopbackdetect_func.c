#include "Global_res.h"
#include "cp_landev_loopbackdetect.h"

int8 getLoopbackEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_enable[8] = {0};

	if ( tcapi_get("LoopDetect_Entry","Enable", s_enable) < 0 )
		strcpy(s_enable, "Yes");

	strcpy(s_enable, (0 == strcmp(s_enable, "Yes")) ? "1" : "0");
	return getTheValue(value, type, deviceNode, s_enable);
}
int8 setLoopbackEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char s_enable[8] = {0};
	uint8 i_enable = *((uint8*)value);

	switch ( i_enable )
	{
		case 0:
			strcpy(s_enable, "No");
			break;
		case 1:
			strcpy(s_enable, "Yes");
			break;
		default:
			return -1;
	}

	tcapi_set("LoopDetect_Entry", "Enable", s_enable);
	SetCfgNodeFlag( "LoopDetect", -1);
    return 0;
}
int8 getLoopbackEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = loopbackdetectAttr.aLoopbackEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLoopbackEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &loopbackdetectAttr.aLoopbackEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LOOPBACKDETECT_ATTR_FLAG;
	return result;
}

int8 getLoopExistPeriodValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_Val[20] = {0};

	if ( tcapi_get("LoopDetect_Entry","LoopExistPeriod", s_Val) < 0 )
		strcpy(s_Val, "5");

	return getTheValue(value, type, deviceNode, s_Val);
}
int8 setLoopExistPeriodValue(void* value, struct DeviceNode*  deviceNode)
{
	char s_Val[20] = {0};
	uint32 i_val = *((uint32*)value);

	snprintf(s_Val, sizeof(s_Val), "%lu", i_val);
	tcapi_set("LoopDetect_Entry", "LoopExistPeriod", s_Val);
	SetCfgNodeFlag( "LoopDetect", -1);
    return 0;
}
int8 getLoopExistPeriodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = loopbackdetectAttr.aLoopExistPeriod;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLoopExistPeriodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &loopbackdetectAttr.aLoopExistPeriod;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LOOPBACKDETECT_ATTR_FLAG;
	return result;
}

int8 getLoopCancelPeriodValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_Val[20] = {0};

	if ( tcapi_get("LoopDetect_Entry","LoopCancelPeriod", s_Val) < 0 )
		strcpy(s_Val, "300");

	return getTheValue(value, type, deviceNode, s_Val);
}
int8 setLoopCancelPeriodValue(void* value, struct DeviceNode*  deviceNode)
{
	char s_Val[20] = {0};
	uint32 i_val = *((uint32*)value);

	snprintf(s_Val, sizeof(s_Val), "%lu", i_val);
	tcapi_set("LoopDetect_Entry", "LoopCancelPeriod", s_Val);
	SetCfgNodeFlag( "LoopDetect", -1);
    return 0;
}
int8 getLoopCancelPeriodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = loopbackdetectAttr.aLoopCancelPeriod;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLoopCancelPeriodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &loopbackdetectAttr.aLoopCancelPeriod;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LOOPBACKDETECT_ATTR_FLAG;
	return result;
}

int8 vlanTag_check(char *vlantag, int maxlen)
{
	char s_vlantag[256] = {0};
	char *pos = NULL;
	int tag_len = 0;
	int idx = 0;
	int tag_maxlen = 4;
	int i_vlantag = 0;

	if ( NULL == vlantag
		|| '\0' == vlantag[0])
		return -1;

	tag_len = strlen(vlantag);
	if ( tag_len > maxlen
		|| ',' == vlantag[tag_len - 1])
		return -1;

	strncpy(s_vlantag, vlantag, sizeof(s_vlantag) - 1);

	pos = strtok(s_vlantag, ",");
	while ( pos )
	{
		if ( 0 != strcmp(pos, "untagged") )
		{
			tag_len = strlen(pos);
			if ( tag_len > tag_maxlen )
				return -1;
			i_vlantag = atoi(pos);
			if ( !( i_vlantag >= 1 && i_vlantag <= 4094 ) )
				return -1;
		}
		pos = strtok(NULL, ",");
	}

	return 0;
}
int8 getVlanTagValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_Val[256] = {0};

	if ( tcapi_get("LoopDetect_Entry","VlanTag", s_Val) < 0 )
		strcpy(s_Val, "untagged");

	return getTheValue(value, type, deviceNode, s_Val);
}
int8 setVlanTagValue(void* value, struct DeviceNode*  deviceNode)
{
	char *vlantag = (char *)value;
	int maxlen = 255;

	if ( NULL == vlantag
		|| '\0' == vlantag[0] )
		return -1;

	if ( 0 != strcmp(vlantag, "untagged")
		&& 0 != vlanTag_check(vlantag, maxlen))
		return -1;

	tcapi_set("LoopDetect_Entry", "VlanTag", vlantag);
	SetCfgNodeFlag( "LoopDetect", -1);
    return 0;
}
int8 getVlanTagAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = loopbackdetectAttr.aVlanTag;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setVlanTagAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &loopbackdetectAttr.aVlanTag;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LOOPBACKDETECT_ATTR_FLAG;
	return result;
}

int hexch_check(char c)
{
	if ( (c >= '0' && c <= '9')
	 	|| (c >= 'a' && c <= 'f')
		|| (c >= 'A' && c <= 'F') )
		return 0;
	else
		return -1;
}

int8 getEthernetTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_Val[12] = {0};

	if ( tcapi_get("LoopDetect_Entry","EthernetType", s_Val) < 0 )
		strcpy(s_Val, "0xFFFA");

	return getTheValue(value, type, deviceNode, s_Val);
}
int8 setEthernetTypeValue(void* value, struct DeviceNode*  deviceNode)
{
	char *ethtype = (char *)value;
	int ethtype_len = 0;
	int idx = 0;
	int val_x = 0;

	if ( NULL == ethtype
		|| '\0' == ethtype[0])
		return -1;

	ethtype_len = strlen(ethtype);
	if ( ethtype_len > 6 )
		return -1;

	for ( idx = 0; idx < ethtype_len; idx ++ )
	{
		if ( (0 == idx && '0' == ethtype[idx])
			|| (1 == idx && 'x' == ethtype[idx]) )
			continue;

		if ( 0 != hexch_check(ethtype[idx]) )
			return -1;

		sscanf(ethtype, "%x", &val_x);
		if (val_x > 0xFFFF)
			return -1;
	}

	tcapi_set("LoopDetect_Entry", "EthernetType", ethtype);
	SetCfgNodeFlag( "LoopDetect", -1);
    return 0;
}
int8 getEthernetTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = loopbackdetectAttr.aEthernetType;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setEthernetTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &loopbackdetectAttr.aEthernetType;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LOOPBACKDETECT_ATTR_FLAG;
	return result;
}

