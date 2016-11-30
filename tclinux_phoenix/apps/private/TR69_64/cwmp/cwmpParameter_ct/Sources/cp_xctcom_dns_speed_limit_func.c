#include "Global_res.h"
#include "cp_xctcom_dns_speed_limit.h"




int8 getDnsSpeedLimitDomainValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char limit_domain[1024] = {0};

	bzero(limit_domain, sizeof(limit_domain));
	if ( 0 != tcapi_get("DnsSpeedLimit_Common", "domain", limit_domain) )
		strcpy(limit_domain, "NULL");

	return getTheValue(value, type, deviceNode, limit_domain);
}
#define MAX_DNS_LIMIT_RULE 16
int8 removeDomianEntry(int start)
{
	char nodename[64] = {0};
	int idx = 0;

	for ( idx = start; idx < MAX_DNS_LIMIT_RULE; idx ++)
	{
		snprintf(nodename, sizeof(nodename) - 1
					, "DnsSpeedLimit_Entry%d", idx);
		tcapi_unset(nodename);
	}

	return 0;
}
int8 parseLimitDomain(char *domain)
{
	char *pos = NULL;
	char limit_domain[1024] = {0};
	char s_dnsv[1024] = {0};
	char s_limitv[512] = {0};
	char nodename[64] = {0};
	int i_limitv_len = 0;
	int idx = 0, entry_idx = 0;

	if ( !domain )
		return -1;

	if ( '\0' == domain[0] )
		return 0;
	strncpy(limit_domain, domain, sizeof(limit_domain) -1);

	pos = strtok(limit_domain, ",");
	while ( pos )
	{
		bzero(s_dnsv, sizeof(s_dnsv));
		bzero(s_limitv, sizeof(s_limitv));

		if ( NULL == strstr(pos, "/") )
		{
			return -1;
		}
		else
		{
			sscanf(pos, "%[^/]/%s", s_dnsv, s_limitv);
			i_limitv_len = strlen(s_limitv);
			for ( idx = 0; idx < i_limitv_len; idx ++ )
			{
				if ( !(isdigit(s_limitv[idx])) )
					return -1;
			}
			if ( entry_idx >= MAX_DNS_LIMIT_RULE )
			{
				tcdbg_printf("\n dns speed limit pair over range!\n");
				return -1;
			}

			snprintf(nodename, sizeof(nodename) - 1
						, "DnsSpeedLimit_Entry%d", entry_idx);
			tcapi_set(nodename, "domain", s_dnsv);
			tcapi_set(nodename, "limitcnt", s_limitv);

			entry_idx ++;
		}

		pos = strtok(NULL, ",");
	}
	removeDomianEntry(entry_idx);
	tcapi_set("DnsSpeedLimit_Common", "limitmode", "perdomain");

	return 0;
}
int8 setDnsSpeedLimitDomainValue(void* value, struct DeviceNode*  deviceNode)
{
	char limit_domain[1024] = {0};
	char s_dnsv_all[32] = {0};
	char s_limitv[512] = {0};
	int i_limitv_len, idx = 0;

	bzero(limit_domain, sizeof(limit_domain));
	if( strlen((char*)value) > (sizeof(limit_domain) - 1) )
		return -1;
	strcpy(limit_domain, (char*)value);

	if ( strstr(limit_domain, "ALL/")
		|| 0 == strcmp(limit_domain, "ALL") )
	{
		if ( 0 == strcmp(limit_domain, "ALL") )
		{
			removeDomianEntry(0);
			tcapi_set("DnsSpeedLimit_Common", "limitmode", "all");
			tcapi_set("DnsSpeedLimit_Common", "allperlimitcnt", "1200");
		}
		else
		{
			sscanf(limit_domain, "%[^/]/%s", s_dnsv_all, s_limitv);
			i_limitv_len = strlen(s_limitv);
			for ( idx = 0; idx < i_limitv_len; idx ++ )
			{
				if ( !(isdigit(s_limitv[idx])) )
					return -1;
			}
			removeDomianEntry(0);
			tcapi_set("DnsSpeedLimit_Common", "limitmode", "all");
			tcapi_set("DnsSpeedLimit_Common", "allperlimitcnt", s_limitv);
		}
	}
	else if ( strstr(limit_domain, "NULL") )
	{
		removeDomianEntry(0);
		tcapi_set("DnsSpeedLimit_Common", "limitmode", "none");
	}
	else if ( 0 != parseLimitDomain(limit_domain) )
		return -1;

	tcapi_set("DnsSpeedLimit_Common", "domain", limit_domain);

	SetCfgNodeFlag(DNSSPEED_LIMIT_STR, -1);
	return 0;
}
int8 getDnsSpeedLimitDomainAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDnsSpeedLimitAttr.aDnsSpeedLimitDomain;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDnsSpeedLimitDomainAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcDnsSpeedLimitAttr.aDnsSpeedLimitDomain;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag2 |= DNS_SPEED_LIMIT_ATTR_FLAG;
	return result;
}

int8 getDnsSpeedLimitActionValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char action[64] = {0};

	bzero(action, sizeof(action));
	if ( 0 != tcapi_get("DnsSpeedLimit_Common", "limitaction", action) )
		action[0] = 0;

	return getTheValue(value, type, deviceNode, action);
}
int8 setDnsSpeedLimitActionValue(void* value, struct DeviceNode*  deviceNode)
{
	char action[64] = {0};
	
	bzero(action, sizeof(action));
	if( strlen((char*)value) > (sizeof(action) - 1) )
		return -1;
	strcpy(action, (char*)value);

	if ( !(0 == strcmp(action, "Alert")
		|| 0 == strcmp(action, "Drop")) )
		return -1;

	tcapi_set("DnsSpeedLimit_Common", "limitaction", action);
	SetCfgNodeFlag(DNSSPEED_LIMIT_STR, -1);
	return 0;
}
int8 getDnsSpeedLimitActionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDnsSpeedLimitAttr.aDnsSpeedLimitAction;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDnsSpeedLimitActionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcDnsSpeedLimitAttr.aDnsSpeedLimitAction;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag2 |= DNS_SPEED_LIMIT_ATTR_FLAG;
	return result;
}
int getInternetWanHgwInfo(char *wan)
{
	char str_ip[32] = {0}, str_mac[20] = {0};
	char def_route_index_v4[10] = {0}, waninfo_node[32] = {0};

	/* get default route for ipv4 */
	bzero(def_route_index_v4, sizeof(def_route_index_v4));
	if ( 0 == 
		tcapi_get("WanInfo_Common", "DefRouteIndexv4", def_route_index_v4)
		&& 0 != def_route_index_v4[0] )
	{
		bzero(waninfo_node, sizeof(waninfo_node));
		sprintf(waninfo_node, "WanInfo_Entry%d", atoi(def_route_index_v4) );
		tcapi_get(waninfo_node, "IP", str_ip);
		tcapi_get(waninfo_node, "hwaddr", str_mac);
		sprintf(wan, "%s|%s", str_ip, str_mac);
	}

	return 0;
}
int8 getDnsSpeedLimitHgwInfoValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char hgwInfo[64] = {0};

	bzero(hgwInfo, sizeof(hgwInfo));
	getInternetWanHgwInfo(hgwInfo);

	return getTheValue(value, type, deviceNode, hgwInfo);
}
int8 getDnsSpeedLimitHgwInfoAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDnsSpeedLimitAttr.aDnsSpeedLimitHgwInfo;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDnsSpeedLimitHgwInfoAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcDnsSpeedLimitAttr.aDnsSpeedLimitHgwInfo;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag2 |= DNS_SPEED_LIMIT_ATTR_FLAG;
	return result;
}

int getDnsSpeedLimitDeviceInfo(char **info)
{
#define DNS_SPEED_LIMIT_REPORT "/tmp/dns_speed_limit_report"
	FILE *fp = NULL;
	int len = 1;

	fp = fopen(DNS_SPEED_LIMIT_REPORT, "r");
	if ( !fp || !info )
		return -1;

	/* get len first */
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if ( len <= 0 )
		return -2;

	*info = (char*)malloc(len + 1);
	if ( NULL == *info )
		return -1;
	bzero(*info, len + 1);

	fread(*info, sizeof(char), len, fp);
	fclose(fp);

	return 0;
}
int8 getDnsSpeedLimitDeviceInfoValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int ret = -1;
	char *deviceInfo = NULL;

	getDnsSpeedLimitDeviceInfo(&deviceInfo);
	ret = getTheValue(value, type, deviceNode, deviceInfo ? deviceInfo : "");

	if ( deviceInfo )
		free(deviceInfo);
	return ret;
}
int8 getDnsSpeedLimitDeviceInfoAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDnsSpeedLimitAttr.aDnsSpeedLimitDeviceInfo;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDnsSpeedLimitDeviceInfoAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcDnsSpeedLimitAttr.aDnsSpeedLimitDeviceInfo;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag2 |= DNS_SPEED_LIMIT_ATTR_FLAG;
	return result;
}



