#include "Global_res.h"
#include "cp_landev_dataspeedlimit.h"

int8 getSpeedLimitModeUPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_mode[8] = {0};

	if ( tcapi_get("DataSpeedLimit_Entry","SpeedLimitModeUP", s_mode) < 0 )
		strcpy(s_mode, "0");

	return getTheValue(value, type, deviceNode, s_mode);
}
int8 setSpeedLimitModeUPValue(void* value, struct DeviceNode*  deviceNode)
{
	char s_mode[8] = {0};
	int i_mode = *((int*)value);

	if ( !(i_mode >= 0 && i_mode <=3) )
		return -1;

	snprintf(s_mode, sizeof(s_mode), "%d", i_mode);
	tcapi_set("DataSpeedLimit_Entry", "SpeedLimitModeUP", s_mode);
	SetCfgNodeFlag( "DataSpeedLimit", -1);
    return 0;
}
int8 getSpeedLimitModeUPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = dataspeedlimitAttr.aSpeedLimitModeUP;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSpeedLimitModeUPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &dataspeedlimitAttr.aSpeedLimitModeUP;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DATASPEEDLIMIT_ATTR_FLAG;
	return result;
}

int8 getSpeedLimitModeDOWNValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_mode[8] = {0};

	if ( tcapi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN", s_mode) < 0 )
		strcpy(s_mode, "0");

	return getTheValue(value, type, deviceNode, s_mode);
}
int8 setSpeedLimitModeDOWNValue(void* value, struct DeviceNode*  deviceNode)
{
	char s_mode[8] = {0};
	int i_mode = *((int*)value);

	if ( !(i_mode >= 0 && i_mode <=3) )
		return -1;

	snprintf(s_mode, sizeof(s_mode), "%d", i_mode);
	tcapi_set("DataSpeedLimit_Entry", "SpeedLimitModeDOWN", s_mode);
	SetCfgNodeFlag( "DataSpeedLimit", -1);
    return 0;
}
int8 getSpeedLimitModeDOWNAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = dataspeedlimitAttr.aSpeedLimitModeDOWN;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSpeedLimitModeDOWNAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &dataspeedlimitAttr.aSpeedLimitModeDOWN;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DATASPEEDLIMIT_ATTR_FLAG;
	return result;
}

int8 interfaceLimitVal_check(char *val)
{
	char s_iface[20] = {0};
	char s_limitv[20] = {0};
	char s_iface_c[12] = {0};
	int i_limitv_len = 0;
	int idx = 0;

	if ( NULL == val
		|| '\0' == val[0])
		return -1;

	if ( NULL == strstr(val, "/") )
	{
		return -1;
	}
	else
	{
		sscanf(val, "%[^/]/%s", s_iface, s_limitv);
	
		i_limitv_len = strlen(s_limitv);
		for ( idx = 0; idx < i_limitv_len; idx ++ )
		{
			if ( !(isdigit(s_limitv[idx])) )
				return -1;
		}

		for ( idx = 1; idx <= 4; idx ++ )
		{
			snprintf(s_iface_c, sizeof(s_iface_c), "LAN%d", idx);
			if ( 0 == strcmp(s_iface_c, s_iface) )
				return 0;
			snprintf(s_iface_c, sizeof(s_iface_c), "SSID%d", idx);
			if ( 0 == strcmp(s_iface_c, s_iface) )
				return 0;
		}
	}

	return -1;
}
int8 interfaceLimit_check(char *limitval, int maxlen)
{
	char s_val[256] = {0};
	char *pos = NULL;
	int val_len = 0;
	int idx = 0;
	int limitval_maxlen = 20;

	if ( NULL == limitval
		|| '\0' == limitval[0])
		return -1;

	val_len = strlen(limitval);
	if ( val_len > maxlen
		|| ',' == limitval[val_len - 1])
		return -1;

	strncpy(s_val, limitval, sizeof(s_val) - 1);

	pos = strtok(s_val, ",");
	while ( pos )
	{
		val_len = strlen(pos);
		if ( val_len > limitval_maxlen )
			return -1;
		if ( 0 != interfaceLimitVal_check(pos) )
			return -1;
		pos = strtok(NULL, ",");
	}

	return 0;
}
int8 getInterfaceLimitUPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_val[256] = {0};

	if ( tcapi_get("DataSpeedLimit_Entry","InterfaceLimitUP", s_val) < 0 )
		s_val[0] = '\0';

	return getTheValue(value, type, deviceNode, s_val);
}
int8 setInterfaceLimitUPValue(void* value, struct DeviceNode*  deviceNode)
{
	int maxlen = 255;
	char *s_val = (char *)value;

	if ( NULL == s_val )
		return -1;

	if ( '\0' != s_val[0]
		&& 0 != interfaceLimit_check(s_val, maxlen) )
		return -1;

	tcapi_set("DataSpeedLimit_Entry", "InterfaceLimitUP", s_val);
	SetCfgNodeFlag( "DataSpeedLimit", -1);
    return 0;
}
int8 getInterfaceLimitUPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = dataspeedlimitAttr.aInterfaceLimitUP;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setInterfaceLimitUPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &dataspeedlimitAttr.aInterfaceLimitUP;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DATASPEEDLIMIT_ATTR_FLAG;
	return result;
}

int8 getInterfaceLimitDOWNValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_val[256] = {0};

	if ( tcapi_get("DataSpeedLimit_Entry","InterfaceLimitDOWN", s_val) < 0 )
		s_val[0] = '\0';

	return getTheValue(value, type, deviceNode, s_val);
}
int8 setInterfaceLimitDOWNValue(void* value, struct DeviceNode*  deviceNode)
{
	int maxlen = 255;
	char *s_val = (char *)value;

	if ( NULL == s_val )
		return -1;

	if ( '\0' != s_val[0]
		&& 0 != interfaceLimit_check(s_val, maxlen) )
		return -1;

	tcapi_set("DataSpeedLimit_Entry", "InterfaceLimitDOWN", s_val);
	SetCfgNodeFlag( "DataSpeedLimit", -1);
    return 0;
}
int8 getInterfaceLimitDOWNAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = dataspeedlimitAttr.aInterfaceLimitDOWN;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setInterfaceLimitDOWNAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &dataspeedlimitAttr.aInterfaceLimitDOWN;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DATASPEEDLIMIT_ATTR_FLAG;
	return result;
}

int8 vlanTagLimitVal_check(char *val)
{
	char s_vlan[20] = {0};
	char s_limitv[20] = {0};
	int i_vlantag = 0;
	int tag_len = 0;
	int tag_maxlen = 4;
	int i_limitv_len = 0;
	int idx = 0;

	if ( NULL == val
		|| '\0' == val[0])
		return -1;

	if ( NULL == strstr(val, "/") )
	{
		return -1;
	}
	else
	{
		sscanf(val, "%[^/]/%s", s_vlan, s_limitv);

		i_limitv_len = strlen(s_limitv);
		for ( idx = 0; idx < i_limitv_len; idx ++ )
		{
			if ( !(isdigit(s_limitv[idx])) )
				return -1;
		}

		if ( 0 == strcmp("untagged", s_vlan) )
			return 0;

		tag_len = strlen(s_vlan);
		if ( tag_len <= tag_maxlen )
		{
			i_vlantag = atoi(s_vlan);
			if ( i_vlantag >= 1 && i_vlantag <= 4094 )
				return 0;
		}
	}

	return -1;
}
int8 vlanTagLimit_check(char *limitval, int maxlen)
{
	char s_val[256] = {0};
	char *pos = NULL;
	int val_len = 0;
	int idx = 0;
	int limitval_maxlen = 20;

	if ( NULL == limitval
		|| '\0' == limitval[0])
		return -1;

	val_len = strlen(limitval);
	if ( val_len > maxlen
		|| ',' == limitval[val_len - 1])
		return -1;

	strncpy(s_val, limitval, sizeof(s_val) - 1);

	pos = strtok(s_val, ",");
	while ( pos )
	{
		val_len = strlen(pos);
		if ( val_len > limitval_maxlen )
			return -1;
		if ( 0 != vlanTagLimitVal_check(pos) )
			return -1;
		pos = strtok(NULL, ",");
	}

	return 0;
}
int8 getVlanTagLimitUPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_val[256] = {0};

	if ( tcapi_get("DataSpeedLimit_Entry","VlanTagLimitUP", s_val) < 0 )
		s_val[0] = '\0';

	return getTheValue(value, type, deviceNode, s_val);
}
int8 setVlanTagLimitUPValue(void* value, struct DeviceNode*  deviceNode)
{
	int maxlen = 255;
	char *s_val = (char *)value;

	if ( NULL == s_val )
		return -1;

	if ( '\0' != s_val[0]
		&& 0 != vlanTagLimit_check(s_val, maxlen) )
		return -1;

	tcapi_set("DataSpeedLimit_Entry", "VlanTagLimitUP", s_val);
	SetCfgNodeFlag( "DataSpeedLimit", -1);
    return 0;
}
int8 getVlanTagLimitUPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = dataspeedlimitAttr.aVlanTagLimitUP;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setVlanTagLimitUPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &dataspeedlimitAttr.aVlanTagLimitUP;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DATASPEEDLIMIT_ATTR_FLAG;
	return result;
}

int8 getVlanTagLimitDOWNValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_val[256] = {0};

	if ( tcapi_get("DataSpeedLimit_Entry","VlanTagLimitDOWN", s_val) < 0 )
		s_val[0] = '\0';

	return getTheValue(value, type, deviceNode, s_val);
}
int8 setVlanTagLimitDOWNValue(void* value, struct DeviceNode*  deviceNode)
{
	int maxlen = 255;
	char *s_val = (char *)value;

	if ( NULL == s_val )
		return -1;

	if ( '\0' != s_val[0]
		&& 0 != vlanTagLimit_check(s_val, maxlen) )
		return -1;

	tcapi_set("DataSpeedLimit_Entry", "VlanTagLimitDOWN", s_val);
	SetCfgNodeFlag( "DataSpeedLimit", -1);
    return 0;
}
int8 getVlanTagLimitDOWNAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = dataspeedlimitAttr.aVlanTagLimitDOWN;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setVlanTagLimitDOWNAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &dataspeedlimitAttr.aVlanTagLimitDOWN;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DATASPEEDLIMIT_ATTR_FLAG;
	return result;
}

int8 rangeIPV4addr_check(char *v4addr)
{
	char start_addr[80];
	char end_addr[80];
	struct in_addr in_s_v4addr = {0};
	struct in_addr in_e_v4addr = {0};

	if ( NULL == v4addr
		|| '\0' == v4addr[0])
		return -1;

	if ( NULL == strstr(v4addr, "-") ) // single ip
	{
		return -1;
	}
	else // ip range
	{
		sscanf(v4addr, "%[^-]-%s", start_addr, end_addr);

		if ( 1 != inet_pton(AF_INET, start_addr, &in_s_v4addr) )
			return -1;

		if ( 1 != inet_pton(AF_INET, end_addr, &in_e_v4addr) )
			return -1;

		if ( in_s_v4addr.s_addr > in_e_v4addr.s_addr )
			return -1;
	}

	return 0;
}
int8 rangeIPV6addr_check(char *v6addr)
{
	char start_addr[80];
	char end_addr[80];
	struct in6_addr in_s_v6addr = {0};
	struct in6_addr in_e_v6addr = {0};

	if ( NULL == v6addr
		|| '\0' == v6addr[0])
		return -1;

	if ( NULL == strstr(v6addr, "-") ) // single ip
	{
		return -1;
	}
	else // ip range
	{
		sscanf(v6addr, "%[^-]-%s", start_addr, end_addr);
		if ( 1 != inet_pton(AF_INET6, start_addr, &in_s_v6addr) )
			return -1;

		if ( 1 != inet_pton(AF_INET6, end_addr, &in_e_v6addr) )
			return -1;

		if ( memcmp(&in_s_v6addr, &in_e_v6addr, sizeof(struct in6_addr)) > 0 )
			return -1;
	}

	return 0;

}

int8 ipLimitVal_check(char *val)
{
	char s_ip[180] = {0};
	char s_limitv[180] = {0};
	int i_vlantag = 0;
	int tag_len = 0;
	int tag_maxlen = 4;
	int i_limitv_len = 0;
	int idx = 0;

	if ( NULL == val
		|| '\0' == val[0])
		return -1;

	if ( NULL == strstr(val, "/") )
	{
		return -1;
	}
	else
	{
		sscanf(val, "%[^/]/%s", s_ip, s_limitv);

		i_limitv_len = strlen(s_limitv);
		for ( idx = 0; idx < i_limitv_len; idx ++ )
		{
			if ( !(isdigit(s_limitv[idx])) )
				return -1;
		}

		if ( strlen(s_ip) > 79 )
			return -1;

		if ( 0 == rangeIPV4addr_check(s_ip)
			|| 0 == rangeIPV6addr_check(s_ip) )
			return 0;
	}

	return -1;
}
int8 ipLimit_check(char *limitval, int maxlen)
{
	char s_val[513] = {0};
	char *pos = NULL;
	int val_len = 0;
	int idx = 0;
	int limitval_maxlen = 175;

	if ( NULL == limitval
		|| '\0' == limitval[0])
		return -1;

	val_len = strlen(limitval);
	if ( val_len > maxlen
		|| ',' == limitval[val_len - 1])
		return -1;

	strncpy(s_val, limitval, sizeof(s_val) - 1);

	pos = strtok(s_val, ",");
	while ( pos )
	{
		val_len = strlen(pos);
		if ( val_len > limitval_maxlen )
			return -1;
		if ( 0 != ipLimitVal_check(pos) )
			return -1;
		pos = strtok(NULL, ",");
	}

	return 0;
}
int8 getIPLimitUPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_val[513] = {0};

	if ( tcapi_get("DataSpeedLimit_Entry","IPLimitUP", s_val) < 0 )
		s_val[0] = '\0';

	return getTheValue(value, type, deviceNode, s_val);
}
int8 setIPLimitUPValue(void* value, struct DeviceNode*  deviceNode)
{
	int maxlen = 512;
	char *s_val = (char *)value;

	if ( NULL == s_val )
		return -1;

	if ( '\0' != s_val[0]
		&& 0 != ipLimit_check(s_val, maxlen) )
		return -1;

	tcapi_set("DataSpeedLimit_Entry", "IPLimitUP", s_val);
	SetCfgNodeFlag( "DataSpeedLimit", -1);
    return 0;
}
int8 getIPLimitUPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = dataspeedlimitAttr.aIPLimitUP;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPLimitUPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &dataspeedlimitAttr.aIPLimitUP;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DATASPEEDLIMIT_ATTR_FLAG;
	return result;
}

int8 getIPLimitDOWNValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_val[513] = {0};

	if ( tcapi_get("DataSpeedLimit_Entry","IPLimitDOWN", s_val) < 0 )
		s_val[0] = '\0';

	return getTheValue(value, type, deviceNode, s_val);
}
int8 setIPLimitDOWNValue(void* value, struct DeviceNode*  deviceNode)
{
	int maxlen = 512;
	char *s_val = (char *)value;

	if ( NULL == s_val )
		return -1;

	if ( '\0' != s_val[0]
		&& 0 != ipLimit_check(s_val, maxlen) )
		return -1;

	tcapi_set("DataSpeedLimit_Entry", "IPLimitDOWN", s_val);
	SetCfgNodeFlag( "DataSpeedLimit", -1);
    return 0;
}
int8 getIPLimitDOWNAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = dataspeedlimitAttr.aIPLimitDOWN;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPLimitDOWNAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &dataspeedlimitAttr.aIPLimitDOWN;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DATASPEEDLIMIT_ATTR_FLAG;
	return result;
}

