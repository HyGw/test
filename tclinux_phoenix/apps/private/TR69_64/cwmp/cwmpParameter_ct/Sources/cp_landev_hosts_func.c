#include "Global_res.h"
#include "cp_landev_hosts.h"

int8 getHostNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	//tcapi_get("DhcpLease", "LeaseNum", result);
	tcapi_get(LANHOST_NODE, LEASENUM_ATTR, result);

	return getTheValue(value, type, deviceNode, result);
}

int8 getHostNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tchostAttr.aHostNumber;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setHostNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tchostAttr.aHostNumber;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= HOST_ATTR_FLAG;
	return result;
}

int8   RebuildHostTable()
{
	uint32 number;
	DeviceNodePtr hostDevNode = NULL;
	char *objectName = "InternetGatewayDevice.LANDevice.1.Hosts.Host";
	int rtnCode = 0;
	DeviceNodePtr rootptr = NULL;
	
	if( rpc.IGDDeviceRootPtr == NULL)
		return -1;
	
	pthread_mutex_lock(&value_change_lock);
	if(inform_task_start == 1){
		pthread_mutex_unlock(&value_change_lock);
		return -1;
	}else{
		rootptr = rpc.IGDDeviceRootPtr;
		rpc.IGDDeviceRootPtr = NULL;
		pthread_mutex_unlock(&value_change_lock);
	}
		
	cwmpSearchDeviceNodeByName(objectName, rootptr, &hostDevNode);
	if(hostDevNode == NULL){
		tc_printf("\r\nRebuildHostTable:hostDevNode == NULL");
		return -1;
	}
	
	cwmpFreeDeviceTable(hostDevNode);
	rtnCode = cwmpInitHostSubTree(hostDevNode);

	pthread_mutex_lock(&value_change_lock);
	rpc.IGDDeviceRootPtr = rootptr;	
	pthread_mutex_unlock(&value_change_lock);
	
	return rtnCode;
}

int	cwmpInitHostSubTree( DeviceNodePtr deviceNode ){
	int rtnCode = 0;
	char tmp[20] = {0};
	int iNum = 0;

	//tcapi_get("DhcpLease", "LeaseNum", tmp);
#if 1
	iNum = getLANHostCount();
#else
	tcapi_get(LANHOST_NODE, LEASENUM_ATTR, tmp);
	iNum = atoi(tmp);
#endif
	rtnCode = RegisterDynNodeFromIndex( deviceNode, HostIndex, iNum );
	
	return rtnCode;
}

int8 getIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[40] = {0};
	char nodeName[32] = {0};
	int nodeIndex = 0;
	char temp[128] = {0};

	nodeIndex = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, LANHOST_ENTRY, nodeIndex);
	tcapi_get(nodeName, "IP", temp);
	memcpy(result, temp, sizeof(result));

	return getTheValue(value, type, deviceNode, result);
}

int8 getIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tchostAttr.aIPAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tchostAttr.aIPAddress;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= HOST_ATTR_FLAG;
	return result;
}

int8 getAddressSourceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char nodeName[32] = {0};
	int nodeIndex = 0;

	nodeIndex = atoi(deviceNode->parentNode->name) - 1;
	#if 0
	sprintf(nodeName, DHCPLEASE_NODE, nodeIndex);
	tcapi_get(nodeName, "MAC", tmp);
	if(0 == strcmp(tmp, BLANK_MAC))
		strcpy(result, "Static");
	else
		strcpy(result, "DHCP");
	#endif
	sprintf(nodeName, LANHOST_ENTRY, nodeIndex);
	tcapi_get(nodeName, "AddressSrc", result);

	return getTheValue(value, type, deviceNode, result);
}

int8 getAddressSourceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tchostAttr.aAddressSource;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setAddressSourceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tchostAttr.aAddressSource;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= HOST_ATTR_FLAG;
	return result;
}

int8 getLeaseTimeRemainingValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char nodeName[32] = {0};
	char tmp[20] = {0};
	int nodeIndex = 0;
	int days = 0,hours = 0, minutes = 0, seconds = 0, time = 0;

	nodeIndex = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, LANHOST_ENTRY, nodeIndex);
	tcapi_get(nodeName, "ExpireDay", tmp);
	days = atoi(tmp);
	tcapi_get(nodeName, "ExpireTime", tmp);
	//ExpireDay Format: 23:55:50
	sscanf(tmp, "%d:%d:%d", &hours, &minutes, &seconds);
	time += days*60*60*24;
	time += hours*60*60;
	time += minutes*60;
	time += seconds;
	sprintf(result, "%d", time);

#if 0
	sprintf(nodeName, DHCPLEASE_NODE, nodeIndex);
	tcapi_get(nodeName, "MAC", tmp);
	if(0 == strcmp(tmp, BLANK_MAC))//case1: Address source is Static
		sprintf(result, "%d", 0);
	else	{						//case2: Address source is DHCP	
		tcapi_get(nodeName, "ExpireDay", tmp);
		days = atoi(tmp);
		tcapi_get(nodeName, "ExpireTime", tmp);
		//ExpireDay Format: 23:55:50
		sscanf(tmp, "%d:%d:%d", &hours, &minutes, &seconds);
		time += days*60*60*24;
		time += hours*60*60;
		time += minutes*60;
		time += seconds;
		sprintf(result, "%d", time);
	}

#endif
	return getTheValue(value, type, deviceNode, result);
}

int8 getLeaseTimeRemainingAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tchostAttr.aLeaseTimeRemaining;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLeaseTimeRemainingAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tchostAttr.aLeaseTimeRemaining;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= HOST_ATTR_FLAG;
	return result;
}

int8 getMACAddress_hostValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if 0
	char result[20] = {0};
	char tmp[20] = {0};
	char cmd[60] = {0};
	char nodeName[20] = {0};
	int nodeIndex = 0;
	FILE *fp=NULL;  
	char buf[60] = {0};
	char *start = NULL;
	
	int i = 0;
	int macLen = strlen(BLANK_MAC);

	nodeIndex = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, DHCPLEASE_NODE, nodeIndex);
	tcapi_get(nodeName, "MAC", result);
	if(0 == strcmp(result, BLANK_MAC)){//case1: Address source is Static
	//get valid mac address from arp table
	/*arp.tmp format: 
	* 1. device with the IP in network: 
	*    "? (192.168.1.2) at 00:11:22:33:44:55 [ether] on br0"
	* 2. device with the IP not in network, but still in arp table:
	*    "? (192.168.1.2) at <incomplete> on br0"
	* 3. device not in arp table:
	*    "in x entries no match found."
	*/	
		tcapi_get(nodeName, "IP", tmp);
		sprintf(cmd, "arp -i br0 -a %s > /tmp/arp.tmp", tmp);
		system(cmd);
		fp = fopen("/tmp/arp.tmp", "r");			
		if(fp){
			fgets(buf, 60, fp);
			fclose(fp);
			system("rm -r /tmp/arp.tmp");
			tcdbg_printf("\n****buf: %s*****\n", buf);
			if(!strstr(buf, "incomplete")){
				start = strstr(buf, tmp);
				if(start){
					start = strstr(buf, "at");
					start += 3;
					while(macLen--){
						result[i++] = *start++;
						result[i] =  '\0';
					}
				}
			}
		}
	}
	#endif

	char result[20] = {0};
	char nodeName[32] = {0};
	int nodeIndex = 0;

	nodeIndex = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, LANHOST_ENTRY, nodeIndex);
	tcapi_get(nodeName, "MAC", result);

	return getTheValue(value, type, deviceNode, result);
}

int8 getMACAddress_hostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tchostAttr.aMACAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setMACAddress_hostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tchostAttr.aMACAddress;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= HOST_ATTR_FLAG;
	return result;
}

int8 getHostNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[65] = {0};
	char nodeName[32] = {0};
	int nodeIndex = 0;

	nodeIndex = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, LANHOST_ENTRY, nodeIndex);
	tcapi_get(nodeName, "HostName", result);
	result[64] = '\0';

	return getTheValue(value, type, deviceNode, result);
}

int8 getHostNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tchostAttr.aHostName;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setHostNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tchostAttr.aHostName;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= HOST_ATTR_FLAG;
	return result;
}

int8 getInterfaceTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if 0
	char result[20] = {0};
	char tmp[20] = {0};
	char mac[20] = {0};
	char nodeName[20] = {0};
	int nodeIndex = 0;
	int ssidNum = 0, ssidClientNum = 0;
	int i = 0, j = 0;

	strcpy(result, "Ethernet");
	nodeIndex = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, DHCPLEASE_NODE, nodeIndex);
	tcapi_get(nodeName, "MAC", mac);
	//look for mac table to check if it is 802.11
	tcapi_set( "wifiMacTab_Common", "flag", "1" ); //set flag to 1 if we want to get wifi mac table info
	tcapi_get("WLan_Common", "BssidNum", tmp);
	ssidNum = atoi(tmp);
	for(i = 0; i < ssidNum; i++){
		sprintf(tmp, "%d", i);
		tcapi_set("WLan_Common", "index", tmp);
		memset(tmp, 0, sizeof(tmp));
		tcapi_get("wifiMacTab_Common", "NUM", tmp);
		ssidClientNum = atoi(tmp);
		if(0 != ssidClientNum){
			for(j = 0; j < ssidClientNum; j++){
				sprintf(nodeName, "wifiMacTab_Entry%d", j);
				tcapi_get(nodeName, "MAC", tmp);
				if(0 == strcmp(tmp, mac)){
					strcpy(result, "802.11");
					return getTheValue(value, type, deviceNode, result);
				}
			}
		}
	}
#else
	char result[32] = {0};
	char nodeName[32] = {0};
	int nodeIndex = 0;

	nodeIndex = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, LANHOST_ENTRY, nodeIndex);
	tcapi_get(nodeName, "InterfaceType", result);
#endif
	return getTheValue(value, type, deviceNode, result);
}

int8 getInterfaceTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tchostAttr.aInterfaceType;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setInterfaceTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tchostAttr.aInterfaceType;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= HOST_ATTR_FLAG;
	return result;
}

int8 getActive_hostValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	//char result[20] = {0};
#if 0
	char tmp[20] = {0};
	char cmd[60] = {0};
	char nodeName[20] = {0};
	int nodeIndex = 0;
	FILE *fp=NULL;  
	char buf[60] = {0};
	char *start = NULL;	

	sprintf(result, "%d", 0);

	nodeIndex = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, DHCPLEASE_NODE, nodeIndex);

	//get valid mac address from arp table
	/*arp.tmp format: 
	* 1. device with the IP in network 					=>active 
	*    "? (192.168.1.2) at 00:11:22:33:44:55 [ether] on br0" 
	* 2. device with the IP not in network, but still in arp table=>not active
	*    "? (192.168.1.2) at <incomplete> on br0"
	* 3. device not in arp table							=>not active
	*    "in x entries no match found."
	*/
	tcapi_get(nodeName, "IP", tmp);
	sprintf(cmd, "arp -i br0 -a %s > /tmp/arp.tmp", tmp);
	system(cmd);
	fp = fopen("/tmp/arp.tmp", "r");
	if(!fp){
		fgets(buf, 60, fp);
		fclose(fp);
		system("rm -r /tmp/arp.tmp");
		if(strstr(buf, tmp) && !strstr(buf, "incomplete")){
			sprintf(result, "%d", 1);
		}
	}
#else
	//sprintf(result, "%d", 1);
#endif
	char result[16] = {0};
	char nodeName[32] = {0};
	int nodeIndex = 0;

	nodeIndex = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, LANHOST_ENTRY, nodeIndex);
	tcapi_get(nodeName, "Active", result);

	return getTheValue(value, type, deviceNode, result);
}

int8 getActive_hostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tchostAttr.aActive;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setActive_hostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tchostAttr.aActive;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= HOST_ATTR_FLAG;
	return result;
}

int8 getLayer2InterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[256] = {0};
	char nodeName[32] = {0};
	int nodeIndex = 0;
	char temp[256] = {0};

	nodeIndex = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, LANHOST_ENTRY, nodeIndex);
	tcapi_get(nodeName, "Layer2Interface", temp);
	memcpy(result, temp, sizeof(result));

	return getTheValue(value, type, deviceNode, result);
}

int8 getLayer2InterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tchostAttr.aLayer2Interface;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLayer2InterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tchostAttr.aLayer2Interface;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= HOST_ATTR_FLAG;
	return result;
}


int8   RebuildHostTableObject()
{
	uint32 number;
	DeviceNodePtr mDevNode = NULL;
	char *objectName = "InternetGatewayDevice.LANDevice.1.Hosts.Host.";
	int rtnCode = 0;

	if( rpc.IGDDeviceRootPtr == NULL)
		return -1;
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &mDevNode);
	if(mDevNode == NULL){
		return -1;
	}
	
	cwmpFreeDeviceTable(mDevNode);
	rtnCode = cwmpInitHostSubTree(mDevNode);
	return rtnCode;
}

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getHosttypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[100] = {0};
	char nodeName[32] = {0};
	int nodeIndex = 0;

	nodeIndex = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, LANHOST_ENTRY, nodeIndex);
	if ( tcapi_get(nodeName, "Hosttype", result) < 0 )
		result[0] ='\0';

	return getTheValue(value, type, deviceNode, result);
}
int8 getHosttypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tchostAttr.aHosttype;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setHosttypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result;

	attribute = &tchostAttr.aHosttype;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= HOST_ATTR_FLAG;
	return result;
}
#endif

