#include "Global_res.h"
#include "cp_landev_ipInterface.h"

int8 getIPInterface_EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	tcapi_get("IPInterface_Entry", "enable", result);

	return getTheValue(value, type, deviceNode, result);
}

int8 setIPInterface_EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	bool enable;
	char tmp[4] = {0};
	enable = *((bool*)value);
	
	if(enable != 1 && enable != 0){
		tc_printf("==> setIPInterface_EnableValue: value should be  0 or 1.\n");
		return -1;
	}
	sprintf(tmp,"%d",enable);	
	tcapi_set("IPInterface_Entry", "enable", tmp);
	
	SetCfgNodeFlag( "IPInterface", -1 );

        return 0;
}

int8 getIPInterface_EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcipInterfaceAttr.aEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPInterface_EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcipInterfaceAttr.aEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPINTERFACE_ATTR_FLAG;
	return result;
}

int8 getIPInterfaceIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	tcapi_get("Lan_Entry0", "IP", result);

	return getTheValue(value, type, deviceNode, result);
}

int8 setIPInterfaceIPAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char ip[20] = {0};
	strcpy(ip, (char*)value);
	//ip = (char*)value;
	
	if(1 != InternalClientIpCheck(ip)){
		tc_printf("==> setIPInterfaceIPAddressValue: value inLegal.\n");
		return -1;
	}
		
	tcapi_set("Lan_Entry0", "IP", ip);
	
	SetCfgNodeFlag( "Lan", -1 );

        return 0;
}

int8 getIPInterfaceIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcipInterfaceAttr.aIPInterfaceIPAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPInterfaceIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcipInterfaceAttr.aIPInterfaceIPAddress;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPINTERFACE_ATTR_FLAG;
	return result;
}

int8 getIPInterfaceSubnetMaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	tcapi_get("Lan_Entry0", "netmask", result);

	return getTheValue(value, type, deviceNode, result);
}

int8 setIPInterfaceSubnetMaskValue(void* value, struct DeviceNode*  deviceNode)
{
	int maskbits =24;
	char netmask[20] = {0};
	strcpy(netmask, (char*)value);
	
	//Can not set the LAN Mask to "255.255.255.255"
	if(strcmp(netmask, "255.255.255.255") == 0){
		tc_printf("==> setIPInterfaceIPAddressValue: value inLegal.\n");
		return -1;
	}
	
	if(SubnetLegalCheck(netmask) != 1){
		tc_printf("==> setIPInterfaceIPAddressValue: value inLegal.\n");
		return -1;
	}
	maskbits = cnt_mask_bits (aton (netmask));		/* count mask bits */
	if ((maskbits < 0 )||(maskbits > 32)){
		tc_printf("==> setIPInterfaceIPAddressValue: value inLegal.\n");
		return -1;
	}
		
	tcapi_set("Lan_Entry0", "netmask", netmask);
	
	SetCfgNodeFlag( "Lan", -1 );

        return 0;
}

int8 getIPInterfaceSubnetMaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcipInterfaceAttr.aIPInterfaceSubnetMask;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPInterfaceSubnetMaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcipInterfaceAttr.aIPInterfaceSubnetMask;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPINTERFACE_ATTR_FLAG;
	return result;
}

int8 getIPInterfaceAddressingTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[10] = {0};

	strcpy(result, "Static");

	return getTheValue(value, type, deviceNode, result);
}

int8 setIPInterfaceAddressingTypeValue(void* value, struct DeviceNode*  deviceNode)
{
	char type[20] = {0};
	strcpy(type, (char*)value);
	
	if(0 != strcmp(type,"Static")){ //only support Static type
		tc_printf("==> setIPInterfaceAddressingTypeValue: value inLegal.\n");
		return -1;
	}
	return 0;
}

int8 getIPInterfaceAddressingTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcipInterfaceAttr.aIPInterfaceAddressingType;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPInterfaceAddressingTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcipInterfaceAttr.aIPInterfaceAddressingType;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPINTERFACE_ATTR_FLAG;
	return result;
}

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getCTComIPv6IPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ip[65] = {0};
	uint8 instance = 0;
	char attrName[33] = {0};	
	
	instance = atoi(deviceNode->parentNode->name) - 1;

	if(instance == 0)
		sprintf(attrName, "%s", "IP6");
	else
		sprintf(attrName, "IP6_%d", instance + 1);
	
	tcapi_get("Lan_Entry0", attrName, ip);

	return getTheValue(value, type, deviceNode, ip);

}

int8 setCTComIPv6IPAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char ip[65] = {0};
	unsigned char tmp[100] = {0};
	uint8 instance = 0;
	char attrName[33] = {0};	
	
	instance = atoi(deviceNode->parentNode->name) - 1;

	if(inet_pton6((const char*)value, tmp)!= 1)
	{
		tcdbg_printf("IPv6 Address Error.\n");
		return -1;	
	}	

	if(instance == 0)
		sprintf(attrName, "%s", "IP6");
	else
		sprintf(attrName, "IP6_%d", instance + 1);

	sprintf(ip, "%s", (char*)value);
	
	tcapi_set("Lan_Entry0", attrName, ip);
	
	SetCfgNodeFlag( "Lan", -1 );

    return 0;
}

int8 getCTComIPv6IPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint8 instance = 0;

	instance = atoi(deviceNode->parentNode->name) - 1;

	attribute = tcipInterfaceAttr.aIPInterfaceIPv6Address[instance];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCTComIPv6IPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint8 instance = 0;

	instance = atoi(deviceNode->parentNode->name) - 1;

	attribute = &tcipInterfaceAttr.aIPInterfaceIPv6Address[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPINTERFACE_ATTR_FLAG;
	return result;
}

int cwmpInitIPv6AddressSubTree(DeviceNodePtr deviceNode)
{
	char ip[65] = {0};
	char node_index[4] = {0};
	char attrName[33] = {0};
	int ret = 0, i = 0;
	
	for (i = 0; i < MAX_LAN_IPV6_DEVICE_NODE; i++)
	{
		if(i == 0)
			sprintf(attrName, "%s", "IP6");
		else
			sprintf(attrName, "IP6_%d", i + 1);
		
		ret = tcapi_get("Lan_Entry0", attrName, ip);

		if (ret < 0 || strcmp(ip, "N/A") == 0) {
			continue;
		}

		/* register this node */
		snprintf(node_index, sizeof(node_index), "%d",  i + 1);
		cwmpInitIPv6AddressIndex[0].name = node_index;
		if (RegisterNodeFromTable(deviceNode, cwmpInitIPv6AddressIndex) == NULL) {
			tcdbg_printf("%s:error!.\n", __FUNCTION__);
			return -1;
		}

	}

	return 0;

}

int8 cwmpAddObjectIPv6Address(uint32 *num, DeviceNode *deviceNode)
{
	char ip[65] = {0};
	char attrName[33] = {0};
	int ret = 0, i = 0;

	DeviceNodePtr LanIpv6DeviceNode = NULL;
	char *objectName = "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.X_CT-COM_IPv6.IPv6Address.";

	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &LanIpv6DeviceNode);
	if (NULL == LanIpv6DeviceNode) {
		tcdbg_printf("top node not found!\n");
		return -1;
	}
	for (i = 0; i < MAX_LAN_IPV6_DEVICE_NODE; i++) {
		if(i == 0)
			sprintf(attrName, "%s", "IP6");
		else
			sprintf(attrName, "IP6_%d", i + 1);
		
		ret = tcapi_get("Lan_Entry0", attrName, ip);

		if (ret < 0 || strcmp(ip, "N/A") == 0) {
			break;
		}
	}

	/* there is no free ipv6 address for adding */
	if (i == MAX_LAN_IPV6_DEVICE_NODE) {
		tc_printf("%s:There is no free Ipv6 Address for adding.\n");
		return -1;
	}
	*num = i + 1;

	/* init new Lan Ipv6 address */
	tcapi_set("Lan_Entry0", attrName, "");
	
	cwmpFreeDeviceTable(LanIpv6DeviceNode);
	ret = cwmpInitIPv6AddressSubTree(LanIpv6DeviceNode);

	if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\ncp_landev_ipInterface_func.c---[cwmpAddObjectIPv6Address]---*num = ", *num);
	}
	
	return ret;
	
	ret = 0;
	return ret;
}

/* delete pvc object */
int8 cwmpDelObjectIPv6Address(uint32 num, DeviceNode *deviceNode)
{
	char ip[65] = {0};
	uint8 instance = 0, ret = 0;
	char attrName[33] = {0};	

	DeviceNodePtr LanIpv6DeviceNode = NULL;
	char *objectName = "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.X_CT-COM_IPv6.IPv6Address.";

	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &LanIpv6DeviceNode);
	if (NULL == LanIpv6DeviceNode) {
		tcdbg_printf("top node not found!\n");
		return -1;
	}

	if( (num > 0) && (num <= MAX_LAN_IPV6_DEVICE_NODE) ){
		instance = num - 1;
	}

	if(instance == 0)
		sprintf(attrName, "%s", "IP6");
	else
		sprintf(attrName, "IP6_%d", instance + 1);

	ret=tcapi_get("Lan_Entry0", attrName, ip);

	if(ret < 0 || strcmp(ip, "N/A") == 0)
	{
		tcdbg_printf("node not found!\n");
		return -1;
	}

	tcapi_set("Lan_Entry0", attrName, "N/A");

	SetCfgNodeFlag( "Lan", -1 );

	cwmpFreeDeviceTable(LanIpv6DeviceNode);
	ret = cwmpInitIPv6AddressSubTree(LanIpv6DeviceNode);

	return ret;
}

int8 getLanIPv6LocalAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ip[65] = {0};
	uint8 instance = 0;

	tcapi_get("Lan_Entry0", "IP6", ip);

	return getTheValue(value, type, deviceNode, ip);

}

int8 setLanIPv6LocalAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char ip[65] = {0};
	unsigned char tmp[100] = {0};

	if(inet_pton6((const char*)value, tmp)!= 1)
	{
		tcdbg_printf("IPv6 Address Error.\n");
		return -1;	
	}

	sprintf(ip, "%s", (char*)value);
	
	tcapi_set("Lan_Entry0", "IP6", ip);
	
	SetCfgNodeFlag( "Lan", -1 );

    return 0;
}

int8 getLanIPv6LocalAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcipInterfaceAttr.aIPInterfaceIPv6LocalAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLanIPv6LocalAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcipInterfaceAttr.aIPInterfaceIPv6LocalAddress;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPINTERFACE_ATTR_FLAG;
	return result;
}

#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getIPv6InterfaceAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ip[65] = {0};
	uint8 instance = 0;

	tcapi_get("Lan_Entry0", "IP6_2", ip);

	return getTheValue(value, type, deviceNode, ip);

}
int8 setIPv6InterfaceAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char ip[65] = {0};
	unsigned char tmp[100] = {0};

	if(inet_pton6((const char*)value, tmp)!= 1)
	{
		tcdbg_printf("IPv6 Address Error.\n");
		return -1;	
	}

	sprintf(ip, "%s", (char*)value);
	tcapi_set("Lan_Entry0", "IP6_2", ip);
	SetCfgNodeFlag( "Lan", -1 );

    return 0;
}
int8 getIPv6InterfaceAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcipInterfaceAttr.aIPv6InterfaceAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPv6InterfaceAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcipInterfaceAttr.aIPv6InterfaceAddress;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPINTERFACE_ATTR_FLAG;
	return result;
}

#endif

/*______________________________________________________________________________
**  cnt_mask_bits
**
**  descriptions:
**	zc/smt 7/31/95 begin - We assume that there are no zero's
**	between 1's in the subnet mask count the 1's in the subnet mask
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
uint8
cnt_mask_bits(
	ip4a	subnet_mask
)
{
	uint8	cnt, index;
	ip4a	mask;
	for (cnt = 32, index = 0; cnt > 0; cnt--, index++) {
		mask = 1 << index;
		if ((subnet_mask & mask) == mask)
			break;
	}
	return cnt;
} /* cnt_mask_bits */
