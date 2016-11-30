#include "Global_res.h"
#include "cp_landev_dhcpv6.h"

int8 getDHCPv6ServerEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_enable[4] = {0};

	if ( tcapi_get("Dhcp6s_Entry","Enable", s_enable) < 0 )
		strcpy(s_enable, "1");
	
	return getTheValue(value, type, deviceNode, s_enable);
}
int8 setDHCPv6ServerEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char s_enable[4] = {0};
	uint8 i_enable = *((uint8*)value);

	switch ( i_enable )
	{
		case 0:
			strcpy(s_enable, "0");
			break;
		case 1:
			strcpy(s_enable, "1");
			tcapi_set("Dhcp6s_Entry", "Mode", "0");
			break;
		default:
		return -1;
	}
	
	tcapi_set("Dhcp6s_Entry", "Enable", s_enable);
	SetCfgNodeFlag( "Dhcp6s", -1);
    return 0;
}
int8 getDHCPv6ServerEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcctcomdhcpv6Attr.aDhcpv6Enable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDHCPv6ServerEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcctcomdhcpv6Attr.aDhcpv6Enable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= X_CT_COM_DHCPV6_ATTR_FLAG;
	return result;
}

int8 getDHCPv6ServerPoolEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	if ( tcapi_get("Dhcp6s_Entry", "AddrFormat", result) < 0 )
		strcpy(result, "AddrPool");

	if( strcmp(result, "AddrPool") == 0 )
		strcpy(result, "1");
	else
		strcpy(result, "0");

	return getTheValue(value, type, deviceNode, result);
}

int8 setDHCPv6ServerPoolEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 enable;
	char result[20] = {0};
	enable = *((uint8*)value);

	if(enable == 1)	
		strcpy(result, "AddrPool");
	else if(enable == 0)
		strcpy(result, "EUI64");
	else
		return -1;

	tcapi_set("Dhcp6s_Entry", "AddrFormat", result);
	SetCfgNodeFlag( "Dhcp6s", -1 );

    return 0;
}

int8 getDHCPv6ServerPoolEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcctcomdhcpv6Attr.aDhcpv6PoolEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDHCPv6ServerPoolEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcctcomdhcpv6Attr.aDhcpv6PoolEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= X_CT_COM_DHCPV6_ATTR_FLAG;
	return result;
}

int8 getdhcpV6Mode()
{
	char enable[4] = {0};

	if ( tcapi_get("Dhcp6s_Entry", "Enable", enable) < 0 )
		strcpy(enable, "0");

	if ( 0 == strcmp(enable, "1"))
		return 1;
	else
		return 0;
}
int8 dhcpV6MinAddr_check(char *v6addr)
{
	int addrLen = 0;
	char start_addr[32] = {0};
	char end_addr[32] = {0};
	char max_addr[24] = {0};
	struct in6_addr in_s_v6addr = {0};
	struct in6_addr in_e_v6addr = {0};

	if ( NULL == v6addr
		|| '\0' == v6addr[0])
		return -1;

	addrLen = strlen(v6addr);
	if ( addrLen > 19 )
		return -1;

	snprintf(start_addr, sizeof(start_addr), "%s::", v6addr);

	if ( 1 != inet_pton(AF_INET6, start_addr, &in_s_v6addr) )
			return -1;

	if ( tcapi_get("Dhcp6s_Entry", "MaxAddress", max_addr) >= 0 )
	{
		snprintf(end_addr, sizeof(end_addr), "%s::", max_addr);
		if ( 1 == inet_pton(AF_INET6, end_addr, &in_e_v6addr) )
		{
			if ( memcmp(&in_s_v6addr, &in_e_v6addr, sizeof(struct in6_addr)) > 0 )
				tcapi_set("Dhcp6s_Entry", "MaxAddress", v6addr);
		}
	}

	return 0;

}
int8 getDHCPv6ServerPoolMinAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char minAddrValue[32] = {0};

	if ( tcapi_get("Dhcp6s_Entry", "MinAddress", minAddrValue) < 0 )
		minAddrValue[0] = '\0';
	
	return getTheValue(value, type, deviceNode, minAddrValue);
}
int8 setDHCPv6ServerPoolMinAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char *pMinAddr = (char *)value;

	if ( NULL == pMinAddr
		|| 1 != getdhcpV6Mode() )
		return -1;
	if ( 0 != dhcpV6MinAddr_check(pMinAddr) )
		return -1;

	tcapi_set("Dhcp6s_Entry", "MinAddress", pMinAddr);
	SetCfgNodeFlag( "Dhcp6s", -1 );
    return 0;
}
int8 getDHCPv6ServerPoolMinAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcctcomdhcpv6Attr.aDhcpv6PoolMinAddr;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDHCPv6ServerPoolMinAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcctcomdhcpv6Attr.aDhcpv6PoolMinAddr;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= X_CT_COM_DHCPV6_ATTR_FLAG;
	return result;
}

int8 dhcpV6MaxAddr_check(char *v6addr)
{
	int addrLen = 0;
	char start_addr[32] = {0};
	char end_addr[32] = {0};
	char min_addr[24] = {0};
	struct in6_addr in_s_v6addr = {0};
	struct in6_addr in_e_v6addr = {0};

	if ( NULL == v6addr
		|| '\0' == v6addr[0])
		return -1;

	addrLen = strlen(v6addr);
	if ( addrLen > 19 )
		return -1;

	snprintf(end_addr, sizeof(end_addr), "%s::", v6addr);

	if ( 1 != inet_pton(AF_INET6, end_addr, &in_e_v6addr) )
			return -1;

	if ( tcapi_get("Dhcp6s_Entry", "MinAddress", min_addr) >= 0 )
	{
		snprintf(start_addr, sizeof(start_addr), "%s::", min_addr);
		if ( 1 == inet_pton(AF_INET6, start_addr, &in_s_v6addr) )
		{
			if ( memcmp(&in_s_v6addr, &in_e_v6addr, sizeof(struct in6_addr)) > 0 )
				return -1;
		}
	}

	return 0;

}
int8 getDHCPv6ServerPoolMaxAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char maxAddrValue[32] = {0};

	if ( tcapi_get("Dhcp6s_Entry", "MaxAddress", maxAddrValue) < 0 )
		maxAddrValue[0] = '\0';
	
	return getTheValue(value, type, deviceNode, maxAddrValue);

}
int8 setDHCPv6ServerPoolMaxAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char *pMaxAddr = (char *)value;

	if ( NULL == pMaxAddr
		|| 1 != getdhcpV6Mode() )
		return -1;
	if ( 0 != dhcpV6MaxAddr_check(pMaxAddr) )
		return -1;

	tcapi_set("Dhcp6s_Entry", "MaxAddress", pMaxAddr);
	SetCfgNodeFlag( "Dhcp6s", -1 );
    return 0;

}
int8 getDHCPv6ServerPoolMaxAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcctcomdhcpv6Attr.aDhcpv6PoolMaxAddr;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDHCPv6ServerPoolMaxAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcctcomdhcpv6Attr.aDhcpv6PoolMaxAddr;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= X_CT_COM_DHCPV6_ATTR_FLAG;
	return result;
}

int8 getDHCPv6ServerPreferredLifeTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_preferredLifeTime[12] = {0};

	if ( tcapi_get("Dhcp6s_Entry", "PreferredLifetime", s_preferredLifeTime) < 0 )
		s_preferredLifeTime[0] = '\0';
	
	return getTheValue(value, type, deviceNode, s_preferredLifeTime);

}
int8 setDHCPv6ServerPreferredLifeTimeValue(void* value, struct DeviceNode*  deviceNode)
{
	char s_validLifeTime[12] = {0};
	char s_preferredLifeTime[12] = {0};
	uint32 i_validLifeTime = 0;
	uint32 i_preferredLifeTime = *((uint32*)value);

	if ( 1 != getdhcpV6Mode() )
		return -1;
	if ( i_preferredLifeTime < 60 )
		return -1;

	if ( tcapi_get("Dhcp6s_Entry", "ValidLifetime", s_validLifeTime) >= 0 )
	{
		sscanf(s_validLifeTime, "%lu", &i_validLifeTime);
		if ( i_validLifeTime <= i_preferredLifeTime
			&& 0xFFFFFFFF != i_preferredLifeTime)
		{
			snprintf(s_preferredLifeTime, sizeof(s_preferredLifeTime), "%lu", i_preferredLifeTime+1);
			tcapi_set("Dhcp6s_Entry", "ValidLifetime", s_preferredLifeTime);
		}
	}
	
	snprintf(s_preferredLifeTime, sizeof(s_preferredLifeTime), "%lu", i_preferredLifeTime);
	tcapi_set("Dhcp6s_Entry", "PreferredLifetime", s_preferredLifeTime);
	SetCfgNodeFlag( "Dhcp6s", -1);
    return 0;

}
int8 getDHCPv6ServerPreferredLifeTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcctcomdhcpv6Attr.aDHCPv6PreferredLifeTime;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDHCPv6ServerPreferredLifeTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcctcomdhcpv6Attr.aDHCPv6PreferredLifeTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= X_CT_COM_DHCPV6_ATTR_FLAG;
	return result;
}

int8 getDHCPv6ServerValidLifeTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_validLifeTime[12] = {0};

	if ( tcapi_get("Dhcp6s_Entry", "ValidLifetime", s_validLifeTime) < 0 )
		s_validLifeTime[0] = '\0';
	
	return getTheValue(value, type, deviceNode, s_validLifeTime);

}
int8 setDHCPv6ServerValidLifeTimeValue(void* value, struct DeviceNode*  deviceNode)
{
	char s_validLifeTime[12] = {0};
	char s_preferredLifeTime[12] = {0};
	uint32 i_preferredLifeTime = 0;
	uint32 i_validLifeTime = *((uint32*)value);

	if ( 1 != getdhcpV6Mode() )
		return -1;
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	if ( i_validLifeTime < 120 )
		return -1;

	snprintf(s_validLifeTime, sizeof(s_validLifeTime), "%d", i_validLifeTime);
	tcapi_set("Dhcp6s_Entry", "ValidLifetime", s_validLifeTime);
	snprintf(s_validLifeTime, sizeof(s_validLifeTime), "%d", i_validLifeTime/2);
	tcapi_set("Dhcp6s_Entry", "PreferredLifetime", s_validLifeTime);
#else
	if ( i_validLifeTime < 60 )
		return -1;

	if ( tcapi_get("Dhcp6s_Entry", "PreferredLifetime", s_preferredLifeTime) >= 0 )
	{
		sscanf(s_preferredLifeTime, "%lu", &i_preferredLifeTime);
		if ( i_validLifeTime <= i_preferredLifeTime )
			return -1;
	}
	snprintf(s_validLifeTime, sizeof(s_validLifeTime), "%lu", i_validLifeTime);
#endif
	tcapi_set("Dhcp6s_Entry", "ValidLifetime", s_validLifeTime);
	SetCfgNodeFlag( "Dhcp6s", -1);
    return 0;

}
int8 getDHCPv6ServerValidLifeTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcctcomdhcpv6Attr.aDHCPv6ValidLifeTime;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDHCPv6ServerValidLifeTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcctcomdhcpv6Attr.aDHCPv6ValidLifeTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= X_CT_COM_DHCPV6_ATTR_FLAG;
	return result;
}

