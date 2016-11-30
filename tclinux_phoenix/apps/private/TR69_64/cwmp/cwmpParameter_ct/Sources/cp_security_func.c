#include "Global_res.h"
#include "cp_security.h"

int type = 0;

int8 getSecurity_IPFilterInEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char attrValue[32] = {0};
	char retValue[32] = {0};

	tcapi_get("IpMacFilter_Common","ActivePortIn",attrValue);
	if(strcmp(attrValue,"1")==0)
		strcpy(retValue,"1");
	else
		strcpy(retValue,"0");
	
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_IPFilterInEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char attrValue[32] = {0};
	boolean setValue = 0;	

	setValue = *(boolean*)(value);
	if ( setValue == 1 ) 
		strcpy(attrValue,"1");
	else
		strcpy(attrValue,"0");
	
	tcapi_set("IpMacFilter_Common","ActivePortIn",attrValue);
	SetCfgNodeFlag( "IpMacFilter", -1);
	return 0;
}
int8 getSecurity_IPFilterInEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aIPFilterInEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_IPFilterInEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aIPFilterInEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}

int8 getSecurity_IPFilterInPolicyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char attrValue[32] = {0};
	char retValue[32] = {0};

	tcapi_get("IpMacFilter_Common","ListTypeIpDown",attrValue);
	if(strcmp(attrValue,"black")==0)
		strcpy(retValue,"0");
	else
		strcpy(retValue,"1");
	
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_IPFilterInPolicyValue(void* value, struct DeviceNode*  deviceNode)
{
	char attrValue[32] = {0};
	uint32 setValue = 0;	

	setValue = *(uint32*)(value);
	if ( setValue == 0 ) 
		strcpy(attrValue,"black");
	else
		strcpy(attrValue,"white");
	
	tcapi_set("IpMacFilter_Common","ListTypeIpDown",attrValue);
	SetCfgNodeFlag( "IpMacFilter", -1);
	return 0;
}
int8 getSecurity_IPFilterInPolicyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aIPFilterInPolicy;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_IPFilterInPolicyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aIPFilterInPolicy;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}

int8 getSecurity_IPFilterOutEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char attrValue[32] = {0};
	char retValue[32] = {0};

	tcapi_get("IpMacFilter_Common","ActivePortOut",attrValue);
	if(strcmp(attrValue,"1")==0)
		strcpy(retValue,"1");
	else
		strcpy(retValue,"0");
	
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_IPFilterOutEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char attrValue[32] = {0};
	boolean setValue = 0;	

	setValue = *(boolean*)(value);
	if ( setValue == 1 ) 
		strcpy(attrValue,"1");
	else
		strcpy(attrValue,"0");
	
	tcapi_set("IpMacFilter_Common","ActivePortOut",attrValue);
	SetCfgNodeFlag( "IpMacFilter", -1);
	return 0;
}
int8 getSecurity_IPFilterOutEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aIPFilterOutEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_IPFilterOutEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aIPFilterOutEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}

int8 getSecurity_IPFilterOutPolicyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char attrValue[32] = {0};
	char retValue[32] = {0};

	tcapi_get("IpMacFilter_Common","ListTypeIpUp",attrValue);
	if(strcmp(attrValue,"black")==0)
		strcpy(retValue,"0");
	else
		strcpy(retValue,"1");
	
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_IPFilterOutPolicyValue(void* value, struct DeviceNode*  deviceNode)
{
	char attrValue[32] = {0};
	uint32 setValue = 0;	

	setValue = *(uint32*)(value);
	if ( setValue == 0 ) 
		strcpy(attrValue,"black");
	else
		strcpy(attrValue,"white");
	
	tcapi_set("IpMacFilter_Common","ListTypeIpUp",attrValue);
	SetCfgNodeFlag( "IpMacFilter", -1);
	return 0;
}
int8 getSecurity_IPFilterOutPolicyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aIPFilterOutPolicy;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_IPFilterOutPolicyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aIPFilterOutPolicy;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}

int checkFreeSecurity_IPFilterEntry()
{
	char temp[4] = {0};
	int addnum = 0;
	int i = 0;

	if(type == 0){
		tcapi_get("IpMacFilter","add_ipdownnum",temp);
		addnum = atoi(temp);
		if(addnum >= MAX_IPMACFILTER_RULE)
		{
			tc_printf("\r\n[cwmp]the max Security_IPFilterIn Entry number is %d", MAX_IPMACFILTER_RULE);
			return -1;
		}		
	}
	else{
		tcapi_get("IpMacFilter","add_ipupnum",temp);
		addnum = atoi(temp);
		if(addnum >= MAX_IPMACFILTER_RULE - MAX_IPDOWN_FILTER_RULE)
		{
			tc_printf("\r\n[cwmp]the max Security_IPFilterOut Entry number is %d", MAX_IPMACFILTER_RULE - MAX_IPDOWN_FILTER_RULE);
			return -1;
		}		
	}
	return addnum;
}

void InitSecurity_IPFilterTable(uint32 number)
{
	char nodeName[32] = {0};
	char attrValue[260] = {0};

	sprintf(nodeName,"IpMacFilter_Entry%d", number);
	if(type == 0){
		tcapi_set("IpMacFilter_Common","IpMacType","IpDown");
		strcpy(attrValue,"IpDownNewRule");
		tcapi_set(nodeName,"Interface","");
		tcapi_set(nodeName,"Direction","Incoming");
	}
	else{
		tcapi_set("IpMacFilter_Common","IpMacType","IpUp");
		strcpy(attrValue,"IpUpNewRule");
		tcapi_set(nodeName,"Interface","br0");
		tcapi_set(nodeName,"Direction","Outgoing");
	}
	tcapi_set(nodeName,"IPName",attrValue);
	tcapi_set(nodeName,"RuleType","IP");
	tcapi_set("IpMacFilter_Common","Action","Add");
	return;
}

int	cwmpInitSecurity_IPFilterTableFromFlash(DeviceNodePtr deviceNode)
{
	char attrValue[260] = {0};
	char nodeIndex[4] = {0};
	char nodeName[32] = {0};
	int i = 0;
	int rtnCode = 0;
	int ret = 0;
	int startIndex = 0;
	int endIndex = 0;
	
	if(deviceNode == NULL){
		return -1;
	}
	if(type == 0){
		startIndex = MAX_IPMACFILTER_RULE - MAX_IPDOWN_FILTER_RULE;
		endIndex = MAX_IPMACFILTER_RULE;
	}
	else{
		startIndex = MAX_IPMACFILTER_RULE - MAX_IPDOWN_FILTER_RULE - MAX_IPUP_FILTER_RULE;
		endIndex = MAX_IPMACFILTER_RULE - MAX_IPDOWN_FILTER_RULE;		
	}
	for(i = startIndex; i < endIndex; i++)
	{
		sprintf(nodeName,"IpMacFilter_Entry%d",i);

		/*check if this rule is exist*/
		ret = tcapi_get( nodeName, "IPName", attrValue);
		if ( ret != 0 ) 
		{
			continue;
		}
		
		sprintf( nodeIndex, "%d", i+1-startIndex);
		Security_IPFilterIndex[0].name = nodeIndex;
		/*register this node*/
		if ( RegisterNodeFromTable( deviceNode, Security_IPFilterIndex ) == NULL )
		{
			rtnCode = -1;/*registerNode fail.*/
			return rtnCode;
		}		
	}

	return rtnCode;
}

int8 addSecurity_IPFilterObject(uint32* num, DeviceNode*  deviceNode)
{
	uint32 number;
	DeviceNodePtr securityNode = NULL;
	int rtnCode = 0;
	int startIndex = 0;

	if(strstr(deviceNode->name, "IPFilterIn") != NULL){
		cwmpSearchDeviceNodeByName("InternetGatewayDevice.X_CMCC_Security.IPFilterIn.", rpc.IGDDeviceRootPtr, &securityNode);
		type = 0;
	}
	else{
		cwmpSearchDeviceNodeByName("InternetGatewayDevice.X_CMCC_Security.IPFilterOut.", rpc.IGDDeviceRootPtr, &securityNode);
		type = 1;
	}
	if(securityNode == NULL){
		return -1;
	}
	
	if ((number = checkFreeSecurity_IPFilterEntry()) < 0) {
		tc_printf("\r\nCan't find free Security_IPFilter Entry for use!");
		return -1;
	}
	else{
		InitSecurity_IPFilterTable(number);
		cwmpFreeDeviceTable(securityNode);
		rtnCode = cwmpInitSecurity_IPFilterTableFromFlash(securityNode);
		if(type == 0){
			startIndex = MAX_IPMACFILTER_RULE - MAX_IPDOWN_FILTER_RULE;
		}
		else{
			startIndex = MAX_IPMACFILTER_RULE - MAX_IPDOWN_FILTER_RULE - MAX_IPUP_FILTER_RULE;
		}		
		*num = number + 1 - startIndex;
		if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\n---[addSecurity_IPFilterObject]---*num = %d", *num);
		}
	}
	
	return rtnCode;
}

void deleteSecurity_IPFilterTable(uint32 number)
{
	char attrValue[32] = {0};
	int startIndex = 0;

	if(type == 0){
		strcpy(attrValue,"IpDown");
		startIndex = MAX_IPMACFILTER_RULE - MAX_IPDOWN_FILTER_RULE;
	}
	else{
		strcpy(attrValue,"IpUp");
		startIndex = MAX_IPMACFILTER_RULE - MAX_IPDOWN_FILTER_RULE - MAX_IPUP_FILTER_RULE;
	}	
	tcapi_set("IpMacFilter_Common","Action","Del");
	tcapi_set("IpMacFilter_Common","IpMacType",attrValue);
	sprintf(attrValue,"%d,",startIndex+number);
	tcapi_set("IpMacFilter_Common","DeleteIndex",attrValue);
	tcapi_commit("IpMacFilter_Entry");
	return;
}
int8 deleteSecurity_IPFilterObject(uint32 num, DeviceNode*  deviceNode){
	DeviceNodePtr securityNode = NULL;
	int rtnCode = 0;

	if(strstr(deviceNode->name, "IPFilterIn") != NULL){
		cwmpSearchDeviceNodeByName("InternetGatewayDevice.X_CMCC_Security.IPFilterIn.", rpc.IGDDeviceRootPtr, &securityNode);
		type = 0;
	}
	else{
		cwmpSearchDeviceNodeByName("InternetGatewayDevice.X_CMCC_Security.IPFilterOut.", rpc.IGDDeviceRootPtr, &securityNode);
		type = 1;
	}	
	if(securityNode == NULL){
		return -1;
	}	
	deleteSecurity_IPFilterTable(num - 1);/*InstanceNumber - 1*/
	cwmpFreeDeviceTable(securityNode);
	rtnCode = cwmpInitSecurity_IPFilterTableFromFlash(securityNode);

	return rtnCode;
}

int getSecurity_IPFilter_Index(DeviceNode*  deviceNode)
{
	int entry = -1;
	
	if(strcmp(deviceNode->parentNode->parentNode->name,"IPFilterOut")==0)
 		entry = MAX_IPMACFILTER_RULE - MAX_IPDOWN_FILTER_RULE - MAX_IPUP_FILTER_RULE + atoi(deviceNode->parentNode->name) - 1;
	else
		entry = MAX_IPMACFILTER_RULE - MAX_IPDOWN_FILTER_RULE + atoi(deviceNode->parentNode->name) - 1;
	return entry;
}

int8 getSecurity_IPFilter_NameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char retValue[260] = {0};
	int entry = -1;

	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_get(nodeName,"IPName",retValue);
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_IPFilter_NameValue(void* value, struct DeviceNode*  deviceNode)
{
	char *setValue = NULL;
	char nodeName[32] = {0};
	int entry = -1;

	setValue=(char*)value;
	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_set(nodeName,"IPName",setValue);
	SetCfgNodeFlag( "IpMacFilter", -1);
	return 0;
}
int8 getSecurity_IPFilter_NameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aName;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_IPFilter_NameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aName;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}

int8 getSecurity_IPFilter_EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char retValue[32] = {0};
	int entry = -1;

	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_get(nodeName,"Active",retValue);
	if(strcmp(retValue,"Yes")==0)
		strcpy(retValue,"1");
	else
		strcpy(retValue,"0");
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_IPFilter_EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	boolean setValue = 0;
	char nodeName[32] = {0};
	int entry = -1;
	char attrValue[32] = {0};

	setValue = *(boolean*)(value);
	if ( setValue == 1 ) 
		strcpy(attrValue,"Yes");
	else
		strcpy(attrValue,"No");
	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_set(nodeName,"Active",attrValue);	
	SetCfgNodeFlag( "IpMacFilter", -1);
	return 0;
}
int8 getSecurity_IPFilter_EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_IPFilter_EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}

int8 getSecurity_IPFilter_SourceIPStartValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char retValue[32] = {0};
	int entry = -1;

	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_get(nodeName,"SrcIPAddr",retValue);
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_IPFilter_SourceIPStartValue(void* value, struct DeviceNode*  deviceNode)
{
	char *setValue = NULL;
	char nodeName[32] = {0};
	int entry = -1;

	setValue=(char*)value;
	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_set(nodeName,"SrcIPAddr",setValue);
	SetCfgNodeFlag( "IpMacFilter", -1);
	return 0;
}
int8 getSecurity_IPFilter_SourceIPStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aSourceIPStart;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_IPFilter_SourceIPStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aSourceIPStart;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}

int8 getSecurity_IPFilter_SourceIPEndValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char retValue[32] = {0};
	int entry = -1;

	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_get(nodeName,"SrcIPEndAddr",retValue);
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_IPFilter_SourceIPEndValue(void* value, struct DeviceNode*  deviceNode)
{
	char *setValue = NULL;
	char nodeName[32] = {0};
	int entry = -1;

	setValue=(char*)value;
	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_set(nodeName,"SrcIPEndAddr",setValue);
	SetCfgNodeFlag( "IpMacFilter", -1);
	return 0;
}
int8 getSecurity_IPFilter_SourceIPEndAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aSourceIPEnd;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_IPFilter_SourceIPEndAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aSourceIPEnd;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}

int8 getSecurity_IPFilter_DestIPStartValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char retValue[32] = {0};
	int entry = -1;

	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_get(nodeName,"DesIPAddr",retValue);
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_IPFilter_DestIPStartValue(void* value, struct DeviceNode*  deviceNode)
{
	char *setValue = NULL;
	char nodeName[32] = {0};
	int entry = -1;

	setValue=(char*)value;
	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_set(nodeName,"DesIPAddr",setValue);
	SetCfgNodeFlag( "IpMacFilter", -1);
	return 0;
}
int8 getSecurity_IPFilter_DestIPStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aDestIPStart;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_IPFilter_DestIPStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aDestIPStart;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}

int8 getSecurity_IPFilter_DestIPEndValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char retValue[32] = {0};
	int entry = -1;

	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_get(nodeName,"DesIPEndAddr",retValue);
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_IPFilter_DestIPEndValue(void* value, struct DeviceNode*  deviceNode)
{
	char *setValue = NULL;
	char nodeName[32] = {0};
	int entry = -1;

	setValue=(char*)value;
	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_set(nodeName,"DesIPEndAddr",setValue);
	SetCfgNodeFlag( "IpMacFilter", -1);
	return 0;
}
int8 getSecurity_IPFilter_DestIPEndAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aDestIPEnd;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_IPFilter_DestIPEndAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aDestIPEnd;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}

int8 getSecurity_IPFilter_ProtocolValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char retValue[32] = {0};
	int entry = -1;

	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_get(nodeName,"Protocol",retValue);
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_IPFilter_ProtocolValue(void* value, struct DeviceNode*  deviceNode)
{
	char *setValue = NULL;
	char nodeName[32] = {0};
	int entry = -1;

	setValue=(char*)value;
	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_set(nodeName,"Protocol",setValue);
	SetCfgNodeFlag( "IpMacFilter", -1);
	return 0;
}
int8 getSecurity_IPFilter_ProtocolAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aProtocol;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_IPFilter_ProtocolAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aProtocol;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}

int8 getSecurity_IPFilter_SourcePortStartValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char retValue[32] = {0};
	int entry = -1;

	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_get(nodeName,"SrcPort",retValue);
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_IPFilter_SourcePortStartValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 setValue = 0;
	char nodeName[32] = {0};
	int entry = -1;
	char attrValue[32] = {0};		

	setValue = *(uint32*)(value);
	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	sprintf(attrValue,"%d",setValue);
	tcapi_set(nodeName,"SrcPort",attrValue);
	SetCfgNodeFlag( "IpMacFilter", -1);
	return 0;
}
int8 getSecurity_IPFilter_SourcePortStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aSourcePortStart;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_IPFilter_SourcePortStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aSourcePortStart;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}

int8 getSecurity_IPFilter_SourcePortEndValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char retValue[32] = {0};
	int entry = -1;

	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_get(nodeName,"SrcPortEnd",retValue);
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_IPFilter_SourcePortEndValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 setValue = 0;
	char nodeName[32] = {0};
	int entry = -1;
	char attrValue[32] = {0};		

	setValue = *(uint32*)(value);
	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	sprintf(attrValue,"%d",setValue);
	tcapi_set(nodeName,"SrcPortEnd",attrValue);
	SetCfgNodeFlag( "IpMacFilter", -1);
	return 0;
}
int8 getSecurity_IPFilter_SourcePortEndAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aSourcePortEnd;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_IPFilter_SourcePortEndAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aSourcePortEnd;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}

int8 getSecurity_IPFilter_DestPortStartValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char retValue[32] = {0};
	int entry = -1;

	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_get(nodeName,"DesPort",retValue);
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_IPFilter_DestPortStartValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 setValue = 0;
	char nodeName[32] = {0};
	int entry = -1;
	char attrValue[32] = {0};		

	setValue = *(uint32*)(value);
	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	sprintf(attrValue,"%d",setValue);
	tcapi_set(nodeName,"DesPort",attrValue);
	SetCfgNodeFlag( "IpMacFilter", -1);
	return 0;
}
int8 getSecurity_IPFilter_DestPortStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aDestPortStart;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_IPFilter_DestPortStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aDestPortStart;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}

int8 getSecurity_IPFilter_DestPortEndValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char retValue[32] = {0};
	int entry = -1;

	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	tcapi_get(nodeName,"DesPortEnd",retValue);
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_IPFilter_DestPortEndValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 setValue = 0;
	char nodeName[32] = {0};
	int entry = -1;
	char attrValue[32] = {0};		

	setValue = *(uint32*)(value);
	entry = getSecurity_IPFilter_Index(deviceNode);
	sprintf(nodeName,"IpMacFilter_Entry%d",entry);
	sprintf(attrValue,"%d",setValue);
	tcapi_set(nodeName,"DesPortEnd",attrValue);
	SetCfgNodeFlag( "IpMacFilter", -1);
	return 0;
}
int8 getSecurity_IPFilter_DestPortEndAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aDestPortEnd;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_IPFilter_DestPortEndAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aDestPortEnd;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}

int8 getSecurity_UrlFilterEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char attrValue[32] = {0};
	char retValue[32] = {0};

	tcapi_get("UrlFilter_Common", "Activate", attrValue);
	if(!strcmp(attrValue, "1"))
		strcpy(retValue, "1");
	else
		strcpy(retValue, "0");
	
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_UrlFilterEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char attrValue[32] = {0};
	boolean setValue = 0;	

	setValue = *(boolean*)(value);
	if(setValue == 1) 
		strcpy(attrValue, "1");
	else
		strcpy(attrValue, "0");
	
	tcapi_set("UrlFilter_Common", "Activate", attrValue);
	SetCfgNodeFlag("UrlFilter", -1);
	return 0;
}
int8 getSecurity_UrlFilterEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aUrlFilterEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_UrlFilterEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aUrlFilterEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}
int8 getSecurity_UrlFilterPolicyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char attrValue[32] = {0};
	char retValue[32] = {0};

	tcapi_get("UrlFilter_Common", "Filter_Policy", attrValue);
	if(!strcmp(attrValue, "1"))
		strcpy(retValue, "1");
	else
		strcpy(retValue, "0");
	
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_UrlFilterPolicyValue(void* value, struct DeviceNode*  deviceNode)
{
	char attrValue[32] = {0};
	uint32 setValue = 0;	

	setValue = *(uint32*)(value);
	if(setValue == 1) 
		strcpy(attrValue, "1");
	else
		strcpy(attrValue, "0");
	
	tcapi_set("UrlFilter_Common", "Filter_Policy", attrValue);
	SetCfgNodeFlag("UrlFilter", -1);
	return 0;
}
int8 getSecurity_UrlFilterPolicyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aUrlFilterPolicy;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_UrlFilterPolicyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aUrlFilterPolicy;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}
int checkFreeSecurity_UrlFilterEntry()
{
	char temp[4] = {0};
	int addnum = 0;

	tcapi_get("UrlFilter", "add_num", temp);
	addnum = atoi(temp);
	if(addnum >= MAX_URL_FILTER_RULE){
		tc_printf("\r\n[cwmp]the max Security_UrlFilter Entry number is %d", MAX_URL_FILTER_RULE);
		return -1;
	}
	
	return addnum;
}
void InitSecurity_UrlFilterTable(uint32 number)
{
	char nodeName[32] = {0};

	sprintf(nodeName, "UrlFilter_Entry%d", number);
	tcapi_set(nodeName, "Activate", "1");
	tcapi_set(nodeName, "URL", "");
	tcapi_set("UrlFilter_Common", "Action", "Add");
	return;
}
int	cwmpInitSecurity_UrlFilterTableFromFlash(DeviceNodePtr deviceNode)
{
	char attrValue[260] = {0};
	char nodeIndex[4] = {0};
	char nodeName[32] = {0};
	int i = 0;
	int rtnCode = 0;
	int ret = 0;
	
	if(deviceNode == NULL){
		return -1;
	}

	for(i = 0; i < MAX_URL_FILTER_RULE; i++)
	{
		sprintf(nodeName, "UrlFilter_Entry%d", i);

		/*check if this rule is exist*/
		ret = tcapi_get(nodeName, "URL", attrValue);
		if(ret != 0) 
			continue;
		
		sprintf(nodeIndex, "%d", i+1);
		Security_UrlFilterIndex[0].name = nodeIndex;
		/*register this node*/
		if(RegisterNodeFromTable(deviceNode, Security_UrlFilterIndex) == NULL){
			rtnCode = -1;/*registerNode fail.*/
			return rtnCode;
		}		
	}

	return rtnCode;
}
int8 addSecurity_UrlFilterObject(uint32* num, DeviceNode*  deviceNode)
{
	uint32 number;
	DeviceNodePtr securityNode = NULL;
	int rtnCode = 0;

	cwmpSearchDeviceNodeByName("InternetGatewayDevice.X_CMCC_Security.ParentalCtrl.Templates.1.UrlFilter.", rpc.IGDDeviceRootPtr, &securityNode);

	if(securityNode == NULL){
		return -1;
	}
	
	if ((number = checkFreeSecurity_UrlFilterEntry()) < 0){
		tc_printf("\r\nCan't find free Security_UrlFilter Entry for use!");
		return -1;
	}
	else{
		InitSecurity_UrlFilterTable(number);
		cwmpFreeDeviceTable(securityNode);
		rtnCode = cwmpInitSecurity_UrlFilterTableFromFlash(securityNode);
	
		*num = number + 1;
		if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\n---[addSecurity_IPFilterObject]---*num = %d", *num);
		}
	}
	
	return rtnCode;
}
void deleteSecurity_UrlFilterTable(uint32 number)
{
	char nodeName[32] = {0};
	char attrValue[32] = {0};

	sprintf(nodeName, "UrlFilter_Entry%d", number);
	tcapi_set("UrlFilter_Common", "Action", "Del");
	sprintf(attrValue, "%d,", number);
	tcapi_set("UrlFilter_Common", "DeleteIndex", attrValue);
	tcapi_commit(nodeName);
	return;
}
int8 deleteSecurity_UrlFilterObject(uint32 num, DeviceNode*  deviceNode)
{
	DeviceNodePtr securityNode = NULL;
	int rtnCode = 0;

	cwmpSearchDeviceNodeByName("InternetGatewayDevice.X_CMCC_Security.ParentalCtrl.Templates.1.UrlFilter.", rpc.IGDDeviceRootPtr, &securityNode);
	
	if(securityNode == NULL){
		return -1;
	}
	
	deleteSecurity_UrlFilterTable(num - 1);/*InstanceNumber - 1*/
	cwmpFreeDeviceTable(securityNode);
	rtnCode = cwmpInitSecurity_UrlFilterTableFromFlash(securityNode);

	return rtnCode;
}
int8 getSecurity_UrlFilterValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char retValue[64] = {0};
	int entry = -1;

	entry = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, "UrlFilter_Entry%d", entry);
	tcapi_get(nodeName, "URL", retValue);
	return getTheValue(value, type, deviceNode, retValue);
}
int8 setSecurity_UrlFilterValue(void* value, struct DeviceNode*  deviceNode)
{
	char *setValue = NULL;
	char nodeName[32] = {0};
	int entry = -1;

	setValue=(char*)value;
	entry = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, "UrlFilter_Entry%d", entry);
	tcapi_set(nodeName, "URL", setValue);
	SetCfgNodeFlag("UrlFilter", -1);
	return 0;
}
int8 getSecurity_UrlFilterAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcLANInterfacesAttr.aUrlFilter;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSecurity_UrlFilterAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcLANInterfacesAttr.aUrlFilter;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= LANINTERFACES_ATTR_FLAG;
	return result;
}	
