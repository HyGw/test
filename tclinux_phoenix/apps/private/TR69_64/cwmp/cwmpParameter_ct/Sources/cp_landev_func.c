#include "Global_res.h"
#include "cp_landev.h"
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>

static int LANEthernetInterfaceNumberOfEntries = 1;
static int LANUSBInterfaceNumberOfEntries = 0;
static int LANIPInterfaceNumberOfEntries = 1;

#ifdef DHCP_PROFILE
static int LANDHCPOptionNumberOfEntries = 2; //option 60 and option 240
static int LANDHCPConSerPoolNumOfEntries = 1; //only 1 instance, corresponding to DHCP option 60
#endif
#ifdef TCSUPPORT_WLAN
int	cwmpInitWLanTableFromFlash(DeviceNodePtr deviceNode)
{
	int totalnum = 0,userdefnum = 0;
	char nodeIndex[4] = {0};
	char nodeName[32] = {0};
	char ssid[36] = {0};
	int i,j = 0,rtnCode = 0;
	int index = 0;
	int ret = 0;
	
	if(deviceNode == NULL){
		return -1;
	}
	
	
	//we only fetch user define WLAN entry
	for(i = 0; i < MAX_WLAN_NODES ; i++ )
	{
		//memset(temp,0,sizeof(temp));
		sprintf(nodeName,"%s_%s%d","WLan", "Entry", i);

		/*check if this route rule is exist*/
		ret = tcapi_get(nodeName,"SSID",ssid);
		if ( ret != 0 ) 
		{
			continue;
		}
		
		sprintf( nodeIndex, "%d", i+1);
		WLANConfigurationIndex[0].name = nodeIndex;
	
		/*register this node*/
		if ( RegisterNodeFromTable( deviceNode, WLANConfigurationIndex ) == NULL )
		{
			rtnCode = -1;		//registerNode fail.
			return rtnCode;
		}			
	}

	return rtnCode;
}

int8 addWLANConfigurationObject(uint32* num,  DeviceNode*  deviceNode)
{
	uint32 number;
	DeviceNodePtr wlanNode = NULL;
	char *objectName = "InternetGatewayDevice.LANDevice.1.WLANConfiguration.";
	int rtnCode = 0;
	int i = 0;
	char tmp[16] = {0};
	char ssid[36] = {0};
	int bssidnum = 0;
	
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &wlanNode);
	if(wlanNode == NULL){
		return -1;
	}
	
	tcapi_get("WLan_Common", "BssidNum", tmp);
	bssidnum = atoi(tmp);
	
	for(i = 0; i < bssidnum; i++)
	{
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp,"WLan_Entry%d",i);
		if( 0 != tcapi_get(tmp,"SSID",ssid) )
			break;
	}
	if(i < bssidnum)
	{
		tcdbg_printf("case 1!\nsleeping\n");
		//sleep(5);
		// set some default value for wifi parameters	
		tcapi_set(tmp, "EnableSSID", "0");		
		memset(ssid, 0, sizeof(ssid));
		sprintf(ssid, "TCAP%d",i);
		tcapi_set(tmp, "SSID", ssid);
		tcapi_set(tmp, "HideSSID", "0");
		tcapi_set(tmp, "AuthMode", "OPEN");
		tcapi_set(tmp, "EncrypType", "NONE");
		tcapi_set(tmp, "AccessPolicy", "0");
		tcapi_set(tmp, "WMM", "0");
		tcapi_set(tmp, "HT_MCS", "33");
		tcapi_set(tmp, "HT_RATE", "Auto");
#ifdef IGMP_SNOOP_SUPPORT//add by xyyou
#if (defined(RT3390) || defined(RT3090)) && !defined(WLAN_MULTIDRIVER)//just for 3390&3090, 5392 used as common attribute
		tcapi_set(tmp, "IgmpSnEn", "1");
#endif
#endif
#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)		
		tcapi_set(tmp, "MaxStaNum", DEFAULT_MAX_STA_NUM);
#endif
		//if SSID 1, set some WPS parameters
#ifdef WSC_AP_SUPPORT//add by xyyou		
		if(i == 0)
		{
			tcapi_set(tmp, "WPSConfStatus", "1");
			tcapi_set(tmp, "WPSMode", "1");
			tcapi_set(tmp, "WPSKeyASCII", "8");
			tcapi_set(tmp, "WPSConfMode", "7");
			tcapi_set(tmp, "OOB_SSID", "Ralink_AP");
			tcapi_set(tmp, "OOB_AuthMode", "OPEN");
			tcapi_set(tmp, "OOB_EncrypType", "NONE");
			tcapi_set(tmp, "OOB_Key", "");			
		}
#endif
		//*num = i + 1;
	}
	else if(i < MAX_WLAN_NODES )
	{
		char buf[5] = {0};
		tc_printf("case 2!\n");
		sprintf(buf,"%d", i+1);
		tcapi_set("WLan_Common","BssidNum", buf);
		sprintf(tmp, "WLan_Entry%d", i);
		// set some default value for wifi parameters		
		tcapi_set(tmp, "EnableSSID", "0");
		tcapi_commit("WLan");

		tcdbg_printf("sleeping\n");
		//sleep(5);
		memset(ssid, 0, sizeof(ssid) );
		sprintf(ssid, "TCAP%d",i);
		tcapi_set(tmp, "SSID", ssid);
		tcapi_set(tmp, "HideSSID", "0");
		tcapi_set(tmp, "AuthMode", "OPEN");
		tcapi_set(tmp, "EncrypType", "NONE");
		tcapi_set(tmp, "AccessPolicy", "0");
		tcapi_set(tmp, "WMM", "0");
		tcapi_set(tmp, "HT_MCS", "33");
		tcapi_set(tmp, "HT_RATE", "Auto");
#ifdef IGMP_SNOOP_SUPPORT//add by xyyou
#if (defined(RT3390) || defined(RT3090)) && !defined(WLAN_MULTIDRIVER)//just for 3390&3090, 5392 used as common attribute
		tcapi_set(tmp, "IgmpSnEn", "1");
#endif
#endif
#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)		
		tcapi_set(tmp, "MaxStaNum", DEFAULT_MAX_STA_NUM);
#endif
		//if SSID 1, set some WPS parameters
#ifdef WSC_AP_SUPPORT//add by xyyou		
		if(i == 0)
		{
			tcapi_set(tmp, "WPSConfStatus", "1");
			tcapi_set(tmp, "WPSMode", "1");
			tcapi_set(tmp, "WPSKeyASCII", "8");
			tcapi_set(tmp, "WPSConfMode", "7");
			tcapi_set(tmp, "OOB_SSID", "Ralink_AP");
			tcapi_set(tmp, "OOB_AuthMode", "OPEN");
			tcapi_set(tmp, "OOB_EncrypType", "NONE");
			tcapi_set(tmp, "OOB_Key", "");			
		}
#endif		
		tcdbg_printf("%s set to Active\n", tmp);
		
	}
	else
	{
		tcdbg_printf("\n\rWLan Node number exceed limit!\n\r");
		return -1;
	}

	*num = i + 1;
	cwmpFreeDeviceTable(wlanNode);
	rtnCode = cwmpInitWLANConfiguration1SubTree(wlanNode);

	if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\ncp_landev_func.c---[addWLANConfigurationObject]---*num = ", *num);
	}
	
	return rtnCode;
	
}


int8 delWLANConfigurationObject(uint32 num, DeviceNode*  deviceNode){
	char rtnCode = 0;
	DeviceNodePtr wlanNode = NULL;
	char *objectName = "InternetGatewayDevice.LANDevice.1.WLANConfiguration.";
//	int rtnCode = 0;
	char buf[16] = {0};
	
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &wlanNode);
	if(wlanNode == NULL){
		return -1;
	}

	if( (num-1 < 0) || (num > MAX_WLAN_NODES) )
	{
		tc_printf("\n\r[cp_landev_func.c] :delWLANConfigurationObject  -- number passed in fail!\n\r");
		return -1;
	}

	sprintf(buf,"WLan_Entry%d",num-1);
	tcapi_unset(buf);
	tc_printf("\n%s unset!\n",buf);
	//deleteMyForwardingTable(num - 1);//InstanceNumber - 1

#if defined(TCSUPPORT_CT_PON_CZ_GD)
	tcapi_get("WebCurSet_Entry", "wlan_id", buf);
	if(atoi(buf) == (num-1))
		tcapi_set("WebCurSet_Entry", "wlan_id", "0");
#endif

	cwmpFreeDeviceTable(wlanNode);
	rtnCode = cwmpInitWLANConfiguration1SubTree(wlanNode);

	return rtnCode;

}
int	cwmpInitWLANConfiguration1SubTree( DeviceNodePtr deviceNode )
{
	int rtnCode = 0, BssidNum;
	char buf[10];
	
	if ( deviceNode == NULL ) {
		return -1;
	}

	/*check if wifi module is exist*/
#if !defined(TCSUPPORT_CT_PON_SC)
	tcapi_get( "Info_WLan","isExist",buf );
	
	if ( strcmp( buf,"On" ) != 0 ) {
		
		tc_printf( "Wifi module is not exist. so we can not build TR069 Paramters tree for it.\n" );
		
		if ( deviceNode->firstChild != NULL ) {
			cwmpNodeFree( deviceNode->firstChild );
			deviceNode->firstChild = NULL;
		}
		
		return rtnCode;
	}
#endif

	rtnCode = cwmpInitWLanTableFromFlash(deviceNode);
	if(rtnCode !=  0)
		return rtnCode;

#ifdef WSC_AP_SUPPORT
	rtnCode = cwmpInitWLANWPS( deviceNode );
#endif

	return rtnCode;
}
#ifdef WSC_AP_SUPPORT
/*only first ssid support wps, so register wps to first ssid here*/
int cwmpInitWLANWPS( DeviceNodePtr deviceNode )
{
	DeviceNodePtr WLANDeviceNode_1 = NULL;
	char *objectName = "WLANConfiguration.1.";
	int rtnCode = 0;

	cwmpSearchDeviceNodeByName(objectName, deviceNode, &WLANDeviceNode_1);
	if ( WLANDeviceNode_1 == NULL ) {
		tc_printf("==>cwmpInitWLANWPS():search node fail.\n");
		return -1;
	}

	if ( RegisterNodeFromTable( WLANDeviceNode_1, WPSTable) == NULL ) {
		rtnCode = -1;
	}

	return rtnCode;
}
#endif
#endif

int8 getLANEthIfNumOfEntries1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  numOfEntries[4];
	sprintf(numOfEntries,"%d",LANEthernetInterfaceNumberOfEntries);	
	return getTheValue(value, type, deviceNode, numOfEntries);
}

int8 getLANEthIfNumOfEntries1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
	uint8  attribute;
	int8  result;

	attribute = tcLanDeviceAttr.aLanEthIfNumOfEntries;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLANEthIfNumOfEntries1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
	uint8*  attribute;
	int8  result;

	attribute = &tcLanDeviceAttr.aLanEthIfNumOfEntries;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getLANUSBIfNumOfEntries1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  numOfEntries[4];

	sprintf(numOfEntries,"%d",LANUSBInterfaceNumberOfEntries);	
	return getTheValue(value, type, deviceNode, numOfEntries);
}

int8 getLANUSBIfNumOfEntries1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
	uint8  attribute;
	int8  result;

	attribute = tcLanDeviceAttr.aLanUSBIfNumOfEntries;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLANUSBIfNumOfEntries1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){

	uint8*  attribute;
	int8  result;

	attribute = &tcLanDeviceAttr.aLanUSBIfNumOfEntries;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}

int8 getLANWLANConfigNumOfEntries1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char bssidNum[4] = {0};
#ifdef TCSUPPORT_WLAN
	if ( tcapi_get("WLan_Common", "BssidNum", bssidNum) < 0 )
#endif		
		strcpy(bssidNum, "0");
	
	return getTheValue(value, type, deviceNode, bssidNum);
}

int8 getLANWLANConfigNumOfEntries1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
	uint8  attribute;
	int8  result;

	attribute = tcLanDeviceAttr.aLanWLANConfigNumOfEntries;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}

int8 setLANWLANConfigNumOfEntries1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
	uint8*  attribute;
	int8  result;

	attribute = &tcLanDeviceAttr.aLanWLANConfigNumOfEntries;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}

int8 getDHCPServerCfg1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char serverCfg[4] = {'\0'};
	tcapi_get("Dhcpd_Common", "configurable", serverCfg);
	if(strcmp(serverCfg,"") == 0)
		strcpy(serverCfg,"0");

	return getTheValue(value, type, deviceNode, serverCfg);
}

int8 setDHCPServerCfg1Value(void* value, struct DeviceNode*  deviceNode)
{
	bool serverCfg;
	char cfgValue[4] = {'\0'};
	
	serverCfg = *((bool*)value);
	
	if(serverCfg != 1 && serverCfg != 0){
		tc_printf("==> setDHCPServerCfg1Value: value should be  0 or 1.\n");
		return -1;
	}
	sprintf(cfgValue,"%d",serverCfg);	
	tcapi_set("Dhcpd_Common", "configurable", cfgValue);
	SetCfgNodeFlag("Dhcpd", -1);

        return 0;
}

int8 getDHCPServerCfg1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
        uint8  attribute;
        int8  result;

        attribute = tcLanDeviceAttr.aDHCPServerCfg1;
        result=GetTheAttributes(attribute,notification, acl, deviceNode);
        return result;

}

int8 setDHCPServerCfg1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
        uint8*  attribute;
        int8  result;

        attribute = &tcLanDeviceAttr.aDHCPServerCfg1;
        result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
        return result;
}

int8 getDHCPServerEnable1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char enable[4] = {'\0'};
	tcapi_get("Lan_Dhcp", "type", enable);
	
	if(!strcmp(enable, "2")){ /*Relay mode*/
		strcpy(enable, "1");
	}
	
	return getTheValue(value, type, deviceNode, enable);
}

int8 setDHCPServerEnable1Value(void* value, struct DeviceNode*  deviceNode)
{
	bool enable;
	char buf[4] = {'\0'};
	tcapi_get("Dhcpd_Common", "configurable", buf);
	if(!strcmp(buf,"0") || !strcmp(buf, "")){   /*Server configureable is disable*/
		return -1;
	}	
	
	enable = *((bool*)value);
	
	if(enable != 1 && enable != 0){
		tc_printf("==> setDHCPServerEnable1Value: value should be  0 or 1.\n");
		return -1;
	}
	sprintf(buf,"%d",enable);	
	tcapi_set("Lan_Dhcp", "type", buf);
	SetCfgNodeFlag( "Lan", -1 );
	SetCfgNodeFlag( "Dhcpd", -1 );

        return 0;
}

int8 getDHCPServerEnable1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
        uint8  attribute;
        int8  result;

        attribute = tcLanDeviceAttr.aDHCPServerEnable1;
        result=GetTheAttributes(attribute,notification, acl, deviceNode);
        return result;

}

int8 setDHCPServerEnable1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
        uint8*  attribute;
        int8  result;

        attribute = &tcLanDeviceAttr.aDHCPServerEnable1;
        result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
        return result;
}

int8 getDHCPRelay1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char dhcpType[4] = {0}, relay[4] = {0};
	bool vRelay;
	
	tcapi_get("Lan_Dhcp", "type", dhcpType);
	if(!strcmp(dhcpType, "2"))
		vRelay = 1;
	else
		vRelay = 0;
	
	sprintf(relay, "%d", vRelay);
	return getTheValue(value, type, deviceNode, relay);
		
}

int8 getDHCPRelay1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
        uint8  attribute;
        int8  result;

        attribute = tcLanDeviceAttr.aDHCPRelay1;
        result=GetTheAttributes(attribute,notification, acl, deviceNode);
        return result;

}

int8 setDHCPRelay1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
        uint8*  attribute;
        int8  result;

        attribute = &tcLanDeviceAttr.aDHCPRelay1;
        result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
        return result;
}

static int8 dhcpPoolCheck(char* lanIp, char* lanMask, char* dhcpPoolIp)
{
	struct in_addr lanIpAddr, lanMaskAddr, poolIpAddr;	

	if(lanIp == NULL || lanMask == NULL || dhcpPoolIp == NULL)
		return -1;	

	inet_aton(lanIp, &lanIpAddr);
	inet_aton(lanMask, &lanMaskAddr);	
	inet_aton(dhcpPoolIp, &poolIpAddr);

	/*check subnet*/
	if((poolIpAddr.s_addr & lanMaskAddr.s_addr) != (lanIpAddr.s_addr & lanMaskAddr.s_addr))
		return -1;
		
	/*illegal address*/
	if(poolIpAddr.s_addr == lanIpAddr.s_addr || poolIpAddr.s_addr == (lanIpAddr.s_addr & lanMaskAddr.s_addr))
		return -1;

	return 0;	
}

int8 getMinAddress1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char minAddr[20] = {0}, dhcpType[4] = {0};
	
	tcapi_get("Lan_Dhcp", "type", dhcpType);
	if(!strcmp(dhcpType, "1")){	/*dhcp server mode*/
		tcapi_get("Dhcpd_Common", "start", minAddr);
	}
	else{
		strcpy(minAddr, "0.0.0.0");
	}
	return getTheValue(value, type, deviceNode, minAddr);
}

int8 setMinAddress1Value(void* value, struct DeviceNode*  deviceNode)
{
	char buf[4] = {'\0'};
	char arNewMinAddress1[64], arOldMinAddress1[64];
	char lanIp[20] = {0}, lanMask[20] = {0};
	int oldPoolCount = 0, newPoolCount = 0;
	struct in_addr newMinAddr, oldMinAddr;

	tcapi_get("Dhcpd_Common", "configurable", buf);
	if(!strcmp(buf,"0") || !strcmp(buf, "")){   /*Server configureable is disable*/
		return -1;
	}
	tcapi_get("Lan_Dhcp", "type", buf);
	if(strcmp(buf, "1")){	/*not dhcp server mode*/
		return -1;
	}
	
	sprintf(arNewMinAddress1,"%s", (char*)(value));
	/*MinAddress is Internal Client IP */	
	if( InternalClientIpCheck(arNewMinAddress1) != 1 ) {
		return -1;
	}
	
	tcapi_get("Lan_Entry0", "IP", lanIp);
	tcapi_get("Lan_Entry0", "netmask", lanMask);
	
	if(dhcpPoolCheck(lanIp, lanMask, arNewMinAddress1) != 0){
		return -1;
	}
	
	tcapi_get("Dhcpd_Common", "pool_count", buf);
	oldPoolCount = atoi(buf);
	
	tcapi_get("Dhcpd_Common", "start", arOldMinAddress1);
	inet_aton(arNewMinAddress1, &newMinAddr);
	inet_aton(arOldMinAddress1, &oldMinAddr);
	
	newPoolCount = oldMinAddr.s_addr - newMinAddr.s_addr + oldPoolCount;
	if(newPoolCount <= 0){
		newPoolCount =1;
	}
	sprintf(buf, "%d", newPoolCount);
	tcapi_set("Dhcpd_Common", "start", arNewMinAddress1);
	tcapi_set("Dhcpd_Common", "pool_count", buf);
	SetCfgNodeFlag( "Dhcpd", -1 );
		
        return 0;
}

int8 getMinAddress1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
        uint8  attribute;
        int8  result;

        attribute = tcLanDeviceAttr.aMinAddress1;
        result=GetTheAttributes(attribute,notification, acl, deviceNode);
        return result;

}

int8 setMinAddress1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
        uint8*  attribute;
        int8  result;

        attribute = &tcLanDeviceAttr.aMinAddress1;
        result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
        return result;
}

int8 getMaxAddress1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char maxAddr[20] = {0}, minAddr[20] = {0}, dhcpType[4] = {0}, poolCnt[4] = {0};
	struct in_addr inMaxAddr;
	
	tcapi_get("Lan_Dhcp", "type", dhcpType);
	if(!strcmp(dhcpType, "1")){	/*dhcp server mode*/
		tcapi_get("Dhcpd_Common", "pool_count", poolCnt);
		tcapi_get("Dhcpd_Common", "start", minAddr);
		inet_aton(minAddr, &inMaxAddr);
		inMaxAddr.s_addr += atoi(poolCnt) - 1;
		strcpy(maxAddr, inet_ntoa(inMaxAddr));
	}
	else{
		strcpy(maxAddr, "0.0.0.0");
	}
	return getTheValue(value, type, deviceNode, maxAddr);
}

int8 setMaxAddress1Value(void* value, struct DeviceNode*  deviceNode)
{
	char buf[4] = {'\0'};
	char arNewMaxAddress1[64], arOldMinAddress1[64];
	char lanIp[20] = {0}, lanMask[20] = {0};
	int oldPoolCount = 0, newPoolCount = 0;
	struct in_addr newMaxAddr, oldMinAddr;

	tcapi_get("Dhcpd_Common", "configurable", buf);
	if(!strcmp(buf,"0") || !strcmp(buf, "")){   /*Server configureable is disable*/
		return -1;
	}
	tcapi_get("Lan_Dhcp", "type", buf);
	if(strcmp(buf, "1")){	/*not dhcp server mode*/
		return -1;
	}
	
	sprintf(arNewMaxAddress1,"%s", (char*)(value));
	/*MaxAddress is Internal Client IP */	
	if( InternalClientIpCheck(arNewMaxAddress1) != 1 ) {
		return -1;
	}
	tcapi_get("Lan_Entry0", "IP", lanIp);
	tcapi_get("Lan_Entry0", "netmask", lanMask);
	
	if(dhcpPoolCheck(lanIp, lanMask, arNewMaxAddress1) != 0){
		return -1;
	}
	
	tcapi_get("Dhcpd_Common", "pool_count", buf);
	oldPoolCount = atoi(buf);
	
	tcapi_get("Dhcpd_Common", "start", arOldMinAddress1);
	inet_aton(arNewMaxAddress1, &newMaxAddr);
	inet_aton(arOldMinAddress1, &oldMinAddr);
	
	//newPoolCount = newMaxAddr.s_addr - oldMinAddr.s_addr;  //suxia.wang
	newPoolCount = newMaxAddr.s_addr - oldMinAddr.s_addr+1;
	
	if(newPoolCount <= 0){
		newPoolCount = 1;
		strcpy(arOldMinAddress1, arNewMaxAddress1);
	}
	
	sprintf(buf, "%d", newPoolCount);
	tcapi_set("Dhcpd_Common", "start", arOldMinAddress1);
	tcapi_set("Dhcpd_Common", "pool_count", buf);
	SetCfgNodeFlag( "Dhcpd", -1 );
	
        return 0;
}

int8 getMaxAddress1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
        uint8  attribute;
        int8  result;

        attribute = tcLanDeviceAttr.aMaxAddress1;
        result=GetTheAttributes(attribute,notification, acl, deviceNode);
        return result;

}

int8 setMaxAddress1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
        uint8*  attribute;
        int8  result;

        attribute = &tcLanDeviceAttr.aMaxAddress1;
        result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
        return result;
}

int8 getReservedAddr1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ReservedAddr[256] = {0};

	strcpy(ReservedAddr, "0.0.0.0");
	return getTheValue(value, type, deviceNode, ReservedAddr);

}

int8 setReservedAddr1Value(void* value, struct DeviceNode*  deviceNode)
{
	char buf[4] = {'\0'};
	tcapi_get("Dhcpd_Common", "configurable", buf);
	if(!strcmp(buf,"0") || !strcmp(buf, "")){   /*Server configureable is disable*/
		return -1;
	}
	
        return 0;
}

int8 getReservedAddr1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
#if 0
        uint8  attribute;
        int8  result;

        attribute = tcLanDeviceAttr.aReservedAddr1;
        result=GetTheAttributes(attribute,notification, acl, deviceNode);
        return result;
#endif
	return 0;
}

int8 setReservedAddr1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
#if 0
        uint8*  attribute;
        int8  result;

        attribute = &tcLanDeviceAttr.aReservedAddr1;
        result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
        return result;
#endif
	return 0;
}

int8 getSubnetMask1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char subnetMask[20] = {0}, dhcpType[4] = {0};
	
	tcapi_get("Lan_Dhcp", "type", dhcpType);
	if(!strcmp(dhcpType, "1")){	/*dhcp server mode*/
		tcapi_get("Lan_Entry0", "netmask", subnetMask);
	}
	else{
		strcpy(subnetMask, "0.0.0.0");
	}
	return getTheValue(value, type, deviceNode, subnetMask);
}

int8 setSubnetMask1Value(void* value, struct DeviceNode*  deviceNode)
{
	char buf[4] = {'\0'};
	char* vSubnetMask1;

	tcapi_get("Dhcpd_Common", "configurable", buf);
	if(!strcmp(buf,"0") || !strcmp(buf, "")){   /*Server configureable is disable*/
		return -1;
	}
	tcapi_get("Lan_Dhcp", "type", buf);
	if(strcmp(buf, "1")){	/*not dhcp server mode*/
		return -1;
	}
	
	vSubnetMask1=(char*)(value);
	if(strcmp(vSubnetMask1, "255.255.255.255") == 0){
		return -1;
	}
	
	if(SubnetLegalCheck(vSubnetMask1) == 0){
		return -1;
	}
	
	tcapi_set("Lan_Entry0", "netmask", vSubnetMask1);
	SetCfgNodeFlag( "Lan", -1 );
	
        return 0;
}

int8 getSubnetMask1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
        uint8  attribute;
        int8  result;

        attribute = tcLanDeviceAttr.aSubnetMask1;
        result=GetTheAttributes(attribute,notification, acl, deviceNode);
        return result;

}

int8 setSubnetMask1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
        uint8*  attribute;
        int8  result;

        attribute = &tcLanDeviceAttr.aSubnetMask1;
        result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
        return result;
}

int8 getDNSServers1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vDNSServers11[64] = {0};
	char  vDNSServers12[64] = {0};		
	char  DNSServers1[128];
	char dhcpType[4] = {0};		
	
	tcapi_get("Lan_Dhcp", "type", dhcpType);
	if(!strcmp(dhcpType, "1")){	/*dhcp server mode*/
		tcapi_get("Dproxy_Entry","Primary_DNS",vDNSServers11);
		if(strlen(vDNSServers11) == 0){
			strcpy(vDNSServers11, "0.0.0.0");
		}
		tcapi_get("Dproxy_Entry","Secondary_DNS",vDNSServers12);
		if(strlen(vDNSServers12) == 0){
			strcpy(vDNSServers12, "0.0.0.0");
		}
	}
	else{
		strcpy(vDNSServers11, "0.0.0.0");
		strcpy(vDNSServers12, "0.0.0.0");
	}
	
	sprintf(DNSServers1,"%s,%s",vDNSServers11,vDNSServers12);	

	return getTheValue(value, type, deviceNode, DNSServers1);

}

int8 setDNSServers1Value(void* value, struct DeviceNode*  deviceNode)
{
	char buf[4] = {'\0'};
	char* DNSServers1;
	char *temp;
	char  ipAddress1[20];
	char  ipAddress2[20];


	tcapi_get("Dhcpd_Common", "configurable", buf);
	if(!strcmp(buf,"0") || !strcmp(buf, "")){   /*Server configureable is disable*/
		return -1;
	}
	tcapi_get("Lan_Dhcp", "type", buf);
	if(strcmp(buf, "1")){	/*not dhcp server mode*/
		return -1;
	}
	
	DNSServers1=(char*)(value);
	temp = strstr(DNSServers1, ",");
	if(temp == NULL){//Only one DNS Server is set.
		if( InternalClientIpCheck(DNSServers1) == 0 ) {
			return -1;
		}
		tcapi_set("Dproxy_Entry","Primary_DNS",DNSServers1);
		SetCfgNodeFlag( "Dproxy", -1 );
	}else{//There are two DNS servers be set.
		*temp = '\0';
		if( InternalClientIpCheck(DNSServers1) != 1 ){		
			return -1;
		}			
		if( InternalClientIpCheck(temp+1) != 1 ){			
			return -1;
		}
		strcpy(ipAddress1, DNSServers1);
		strcpy(ipAddress2, temp+1);
		
		tcapi_set("Dproxy_Entry","Primary_DNS",ipAddress1);
		tcapi_set("Dproxy_Entry","Secondary_DNS",ipAddress2);
		SetCfgNodeFlag( "Dproxy", -1 );		
	}
		
        return 0;
}

int8 getDNSServers1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
        uint8  attribute;
        int8  result;

        attribute = tcLanDeviceAttr.aDNSServers1;
        result=GetTheAttributes(attribute,notification, acl, deviceNode);
        return result;

}

int8 setDNSServers1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
        uint8*  attribute;
        int8  result;

        attribute = &tcLanDeviceAttr.aDNSServers1;
        result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
        return result;
}

int8 getDomainName1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vDomainName1[65] = {0};
	
	tcapi_get("Dhcpd_Common", "DomainName", vDomainName1);
	return getTheValue(value, type, deviceNode, vDomainName1);
}

int8 setDomainName1Value(void* value, struct DeviceNode*  deviceNode)
{
	char buf[4] = {'\0'};
	char* DomainName1;
	
	tcapi_get("Dhcpd_Common", "configurable", buf);
	if(!strcmp(buf,"0") || !strcmp(buf, "")){   /*Server configureable is disable*/
		return -1;
	}
	
	DomainName1 = (char*)(value);
	if(strlen(DomainName1) >= 64){
		return -1;
	}
	tcapi_set("Dhcpd_Common", "DomainName", DomainName1);
	SetCfgNodeFlag( "Dhcpd", -1 );
	
        return 0;
}

int8 getDomainName1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
        uint8  attribute;
        int8  result;

        attribute = tcLanDeviceAttr.aDomainName1;
        result=GetTheAttributes(attribute,notification, acl, deviceNode);
        return result;

}

int8 setDomainName1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
        uint8*  attribute;
        int8  result;

        attribute = &tcLanDeviceAttr.aDomainName1;
        result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
        return result;
}

int8 getIPRouters1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vIPRouters1[65] = {0};
	tcapi_get("Dhcpd_Common", "router", vIPRouters1);	

	return getTheValue(value, type, deviceNode, vIPRouters1);

}

int8 setIPRouters1Value(void* value, struct DeviceNode*  deviceNode)
{
	char buf[4] = {'\0'};
	char* IPRouters1;
	
	tcapi_get("Dhcpd_Common", "configurable", buf);
	if(!strcmp(buf,"0") || !strcmp(buf, "")){   /*Server configureable is disable*/
		return -1;
	}
	
	IPRouters1=(char*)(value);	

	if( InternalClientIpCheck(IPRouters1) != 1 ) {
		return -1;
	}
	tcapi_set("Dhcpd_Common", "router", IPRouters1);
	SetCfgNodeFlag( "Dhcpd", -1 );
	
        return 0;
}

int8 getIPRouters1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
        uint8  attribute;
        int8  result;

        attribute = tcLanDeviceAttr.aIPRouters1;
        result=GetTheAttributes(attribute,notification, acl, deviceNode);
        return result;

}

int8 setIPRouters1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
        uint8*  attribute;
        int8  result;

        attribute = &tcLanDeviceAttr.aIPRouters1;
        result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
        return result;
}

int8 getDHCPLeaseTime1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vDHCPLeaseTime1[20] = {0};
	uint32 dhcpLeaseTime = 0;
	uint32 vInfiniteTime = 0xffffffff;
	char infiniteTime[20] = {0};
	
	tcapi_get("Dhcpd_Common", "lease", vDHCPLeaseTime1);
	sprintf(infiniteTime, "%u", vInfiniteTime);
	
	if(!strcmp(infiniteTime, vDHCPLeaseTime1)){		
		sprintf(vDHCPLeaseTime1,"%d",-1);
	}

	return getTheValue(value, type, deviceNode, vDHCPLeaseTime1);
}

int8 setDHCPLeaseTime1Value(void* value, struct DeviceNode*  deviceNode)
{
	char buf[4] = {'\0'};
	int* vDHCPLeaseTime1;
	char DHCPLeaseTime[20] = {0};
	uint32 infiniteTime = 0xffffffff;

	tcapi_get("Dhcpd_Common", "configurable", buf);
	if(!strcmp(buf,"0") || !strcmp(buf, "")){   /*Server configureable is disable*/
		return -1;
	}
	
	vDHCPLeaseTime1=(int*)(value);

	if( *vDHCPLeaseTime1 >= 0 ) { 
		sprintf(DHCPLeaseTime, "%d", *vDHCPLeaseTime1);
		tcapi_set("Dhcpd_Common", "lease", DHCPLeaseTime);		
		tcapi_set("Dhcpd_Option60", "lease", DHCPLeaseTime); 
	}else if(*vDHCPLeaseTime1 == -1 ){
		sprintf(DHCPLeaseTime, "%u", infiniteTime);
		tcapi_set("Dhcpd_Common", "lease", DHCPLeaseTime);		
		tcapi_set("Dhcpd_Option60", "lease", DHCPLeaseTime); 
	}else{
		return -1;
	}
	SetCfgNodeFlag( "Dhcpd", -1 );	
        return 0;
}

int8 getDHCPLeaseTime1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
        uint8  attribute;
        int8  result;

        attribute = tcLanDeviceAttr.aDHCPLeaseTime1;
        result=GetTheAttributes(attribute,notification, acl, deviceNode);
        return result;

}

int8 setDHCPLeaseTime1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
        uint8*  attribute;
        int8  result;

        attribute = &tcLanDeviceAttr.aDHCPLeaseTime1;
        result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
        return result;
}

int8 getIPInterfaceNumValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  result[4] = {0};
	sprintf(result,"%d", LANIPInterfaceNumberOfEntries);

	return getTheValue(value, type, deviceNode, result);
}

int8 getIPInterfaceNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
        uint8  attribute;
        int8  result;

        attribute = tcLanDeviceAttr.aIPInterfaceNum;
        result=GetTheAttributes(attribute,notification, acl, deviceNode);
        return result;

}

int8 setIPInterfaceNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
        uint8*  attribute;
        int8  result;

        attribute = &tcLanDeviceAttr.aIPInterfaceNum;
        result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
        return result;
}

int	cwmpInitLANIPInterfaceSubTree( DeviceNodePtr deviceNode )
{
	int rtnCode = 0;

	if ( deviceNode == NULL ) {
		return -1;
	}
	
	rtnCode = RegisterDynNodeFromIndex( deviceNode, IPInterfaceIndex, LANIPInterfaceNumberOfEntries );

	return rtnCode;
}

int	cwmpInitLANEthInterCfgSubTree( DeviceNodePtr deviceNode )
{
	int rtnCode = 0;

#if 1   //LAN_ETHERNET_INTERFACE
	char *portNum_path = "/tmp/ether_portNum";
	char cmd[64]={'\0'};
	int iNum;	
	FILE *fp = NULL;
#endif

	if ( deviceNode == NULL ) {
		return -1;
	}
#if 1   //LAN_ETHERNET_INTERFACE	
	sprintf(cmd, "/userfs/bin/ethphxcmd eth0 portNum > %s", portNum_path);
	system(cmd);

	fp = fopen(portNum_path,"r");
	if(fp != NULL)
	{
		fscanf(fp,"%d",&iNum);
		fclose(fp);
		if( iNum != 1 ){
#if defined(TCSUPPORT_CT_1PORT)
			LANEthernetInterfaceNumberOfEntries = 1;
#elif defined(TCSUPPORT_CT_2PWIFI) || defined(TCSUPPORT_CUC_2PORT) || defined(TCSUPPORT_CT_2PORTS)
			LANEthernetInterfaceNumberOfEntries = 2;
#else
			LANEthernetInterfaceNumberOfEntries = 4;
#endif
		}
	}
	unlink(portNum_path);
	
#endif

	rtnCode = RegisterDynNodeFromIndex( deviceNode, EthInterCfgIndex, LANEthernetInterfaceNumberOfEntries );

	return rtnCode;
}

#ifdef DHCP_PROFILE
int8 getDHCPOptionNumValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  numOfEntries[4] = {0};
	sprintf(numOfEntries,"%d", LANDHCPOptionNumberOfEntries);		

	return getTheValue(value, type, deviceNode, numOfEntries);
}

int8 getDHCPOptionNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
        uint8  attribute;
        int8  result;

        attribute = tcLanDeviceAttr.aLanDHCPOptionNumOfEntries;
        result=GetTheAttributes(attribute,notification, acl, deviceNode);
        return result;

}

int8 setDHCPOptionNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
        uint8*  attribute;
        int8  result;

        attribute = &tcLanDeviceAttr.aLanDHCPOptionNumOfEntries;
        result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
        return result;
}

int8 getDHCPConditionalPoolNumOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  DHCPConditionalPoolNumOfEntries[4] = {0};	

        sprintf(DHCPConditionalPoolNumOfEntries,"%u",LANDHCPConSerPoolNumOfEntries);	
	return getTheValue(value, type, deviceNode, DHCPConditionalPoolNumOfEntries);
}

int8 getDHCPConditionalPoolNumOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcLanDeviceAttr.aDHCPConditionalPoolNumOfEntries;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDHCPConditionalPoolNumOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcLanDeviceAttr.aDHCPConditionalPoolNumOfEntries;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int	cwmpInitDHCPOptionSubTree( DeviceNodePtr deviceNode )
{
	int rtnCode = 0;
	char dhcpType[4] = {0};
	
	if ( deviceNode == NULL ) {
		return -1;
	}
	/*check if DHCP Server is Enable or not*/
	tcapi_get("Lan_Dhcp", "type", dhcpType);
	if(strcmp(dhcpType, "1") != 0) {
		tc_printf( "DHCP Server is not enable. so we can not build TR069 Paramters tree for it.\n" );
		return rtnCode;
	}
	
	rtnCode = RegisterDynNodeFromIndex( deviceNode, DHCPOptionIndex, LANDHCPOptionNumberOfEntries);

	return rtnCode;
}

int	cwmpInitDHCPConSerPoolSubTree( DeviceNodePtr deviceNode )
{
	int rtnCode = 0;
	char dhcpType[4] = {0};

	if ( deviceNode == NULL ) {
		return -1;
	}

	/*check if DHCP Server is Enable or not*/
	tcapi_get("Lan_Dhcp", "type", dhcpType);
	if(strcmp(dhcpType, "1") != 0) {
		tc_printf( "DHCP Server is not enable. so we can not build TR069 Paramters tree for it.\n" );
		return rtnCode;
	}

	rtnCode = RegisterDynNodeFromIndex( deviceNode, DHCPConSerPoolIndex, LANDHCPConSerPoolNumOfEntries);

	return rtnCode;
}
#endif
int8 getUseAllocatedWAN1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vUseAllocatedWAN[20] = {0};

	tcapi_get(DHCPD_COMMON, "useAllocatedWAN", vUseAllocatedWAN);

	if(strcmp(vUseAllocatedWAN,"") == 0)
		sprintf(vUseAllocatedWAN, "%s", "Normal");
	
	return getTheValue(value, type, deviceNode, vUseAllocatedWAN);
}

int8 setUseAllocatedWAN1Value(void* value, struct DeviceNode*  deviceNode)
{
	char* tempValue = NULL;	
	char  vUseAllocatedWAN[20] = {0};

	tempValue = (char*)(value);
	if(strlen(tempValue) >= 19){//max length is 18(the length of "UseAllocatedSubnet").
		tc_printf("\n==>UseAllocatedWANValue length should not larger than 19\n");
		return -1;
	}

	if(strcmp(tempValue, "Normal") ==0
	|| strcmp(tempValue, "Passthrough") ==0
	|| strcmp(tempValue, "UseAllocatedSubnet") ==0) {
		strcpy(vUseAllocatedWAN, tempValue);
	}
	else {
		tc_printf("\nInvalid UseAllocatedWANValue\n");
		return -1;
	}

	tcapi_set(DHCPD_COMMON, "useAllocatedWAN", vUseAllocatedWAN);

	SetCfgNodeFlag( DHCPD_NODENAME, -1);

	return 0;
}

int8 getUseAllocatedWAN1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcLanDeviceAttr.aUseAllocatedWAN1;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setUseAllocatedWAN1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcLanDeviceAttr.aUseAllocatedWAN1;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	return result;
}

int8 getAssociatedConnection1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vAssociatedConnection1[4] = {0};	

	tcapi_get(DHCPD_COMMON, "AssociatedConnection", vAssociatedConnection1);

	return getTheValue(value, type, deviceNode, vAssociatedConnection1);
}

int8 setAssociatedConnection1Value(void* value, struct DeviceNode*  deviceNode)
{
	char* tempValue = NULL;	
	char  vAssociatedConnection1[20] = {0};

	tempValue = (char*)(value);
	if(tempValue==NULL )
		return -1;

	strcpy(vAssociatedConnection1, tempValue);
	
	tcapi_set(DHCPD_COMMON, "AssociatedConnection", vAssociatedConnection1);

	SetCfgNodeFlag( DHCPD_NODENAME, -1);
       return 0;
}

int8 getAssociatedConnection1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcLanDeviceAttr.aAssociatedConnection1;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setAssociatedConnection1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcLanDeviceAttr.aAssociatedConnection1;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	return result;
}

int8 getPassthroughLease1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vPassthroughLease1[20] = {0};

	tcapi_get(DHCPD_COMMON, "PassthroughLease", vPassthroughLease1);

	if(strcmp(vPassthroughLease1,"") == 0)
		sprintf(vPassthroughLease1, "%s", "600");
	
	return getTheValue(value, type, deviceNode, vPassthroughLease1);
}

int8 setPassthroughLease1Value(void* value, struct DeviceNode*  deviceNode)
{
	char buf[4] = {'\0'};
	int* vPassthroughLease1;
	char PassthroughLease1[20] = {0};
	uint32 infiniteTime = 0xffffffff;

	vPassthroughLease1=(int*)(value);

	if( *vPassthroughLease1 >= 0 ) { 
		sprintf(PassthroughLease1, "%d", *vPassthroughLease1);
		tcapi_set(DHCPD_COMMON, "PassthroughLease", PassthroughLease1);	
	}else if(*vPassthroughLease1 == -1 ){
		sprintf(PassthroughLease1, "%u", infiniteTime);
		tcapi_set(DHCPD_COMMON, "PassthroughLease", PassthroughLease1);
	}else{
		return -1;
	}
	SetCfgNodeFlag( DHCPD_NODENAME, -1);
      return 0;
}

int8 getPassthroughLease1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
        uint8  attribute;
        int8  result;

        attribute = tcLanDeviceAttr.aPassthroughLease1;
        result=GetTheAttributes(attribute,notification, acl, deviceNode);
        return result;

}

int8 setPassthroughLease1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
        uint8*  attribute;
        int8  result;

        attribute = &tcLanDeviceAttr.aPassthroughLease1;
        result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
        return result;
}

int8 getPassthroughMACAddress1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vPassthroughMACAddress1[20] = {0};

	tcapi_get(DHCPD_COMMON, "PassthroughMACAddress", vPassthroughMACAddress1);

	return getTheValue(value, type, deviceNode, vPassthroughMACAddress1);
}

int8 setPassthroughMACAddress1Value(void* value, struct DeviceNode*  deviceNode)
{
	char* tempValue = NULL;	
	char  vPassthroughMACAddress1[20] = {0};

	tempValue = (char*)(value);
	if(tempValue==NULL )
		return -1;

	if(checkMACFormat(tempValue) < 0)
		return -1;

	strcpy(vPassthroughMACAddress1, tempValue);
	
	tcapi_set(DHCPD_COMMON, "PassthroughMACAddress", vPassthroughMACAddress1);

	SetCfgNodeFlag( DHCPD_NODENAME, -1);

	return 0;
}

int8 getPassthroughMACAddress1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcLanDeviceAttr.aPassthroughMACAddress1;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setPassthroughMACAddress1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcLanDeviceAttr.aPassthroughMACAddress1;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	return result;
}

int8 getAllowedMACAddress1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vAllowedMACAddress1[20] = {0};

	tcapi_get(DHCPD_COMMON, "AllowedMACAddress", vAllowedMACAddress1);

	return getTheValue(value, type, deviceNode, vAllowedMACAddress1);
}

int8 setAllowedMACAddress1Value(void* value, struct DeviceNode*  deviceNode)
{
	char* tempValue = NULL;	
	char  vAllowedMACAddress1[20] = {0};

	tempValue = (char*)(value);
	if(tempValue==NULL )
		return -1;

	strcpy(vAllowedMACAddress1, tempValue);
	
	tcapi_set(DHCPD_COMMON, "AllowedMACAddress", vAllowedMACAddress1);

	SetCfgNodeFlag( DHCPD_NODENAME, -1);

	return 0;
}

int8 getAllowedMACAddress1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcLanDeviceAttr.aAllowedMACAddress1;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setAllowedMACAddress1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcLanDeviceAttr.aAllowedMACAddress1;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	return result;
}

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int isValidMinMaxAddr(char *ipaddr)
{
	char cfgbuf[4] = {0};
	char lanIp[20] = {0}, lanMask[20] = {0};

	if ( NULL == ipaddr )
		return -1;

	tcapi_get("Dhcpd_Common", "configurable", cfgbuf);
	if ( '\0' == cfgbuf[0] || 0 == strcmp(cfgbuf, "0") ) // server configureable is disable
		return -1;

	memset(cfgbuf, 0, sizeof(cfgbuf));
	tcapi_get("Lan_Dhcp", "type", cfgbuf);
	if ( 0 != strcmp(cfgbuf, "1") ) // not dhcp server mode.
		return -1;

	if( 1 != InternalClientIpCheck(ipaddr) ) // invalid ip
		return -1;

	tcapi_get("Lan_Entry0", "IP", lanIp);
	tcapi_get("Lan_Entry0", "netmask", lanMask);

	if( 0 != dhcpPoolCheck(lanIp, lanMask, ipaddr))
		return -1;

	return 1;
}
int checkMinAddr(char *ipmin, char *minAttr, char *poolAttr, int *newPoolCnt)
{
	char sMinAddr[20] = {0};
	char sMinOldAddr[20] = {0};
	char sPoolStr[8] = {0};
	int iPoolCnt = 1;
	uint32 maxip = 0;
	struct in_addr ipMinOldIP = {0}, ipMinIP = {0};

	if ( NULL == ipmin || NULL == minAttr || NULL == poolAttr || NULL == newPoolCnt )
		return -1;

	snprintf(sMinAddr, sizeof(sMinAddr), "%s", ipmin);
	// check ip valid.
	if ( 1 != isValidMinMaxAddr(ipmin) )
		return -1;

	*newPoolCnt = 1;
	if ( tcapi_get(DHCPD_NODEOPT60, minAttr, sMinOldAddr) < 0
		|| tcapi_get(DHCPD_NODEOPT60, poolAttr, sPoolStr) < 0 )
		return 0; // no need to check

	if ( 1 != inet_aton(sMinAddr, &ipMinIP) || 1 != inet_aton(sMinOldAddr, &ipMinOldIP) )
		return -1;

	iPoolCnt = atoi(sPoolStr);
	iPoolCnt = ( iPoolCnt <= 0 ) ? 1 : iPoolCnt;
	maxip = ipMinOldIP.s_addr + iPoolCnt - 1;

	iPoolCnt = maxip + 1 - ipMinIP.s_addr;
	iPoolCnt = ( iPoolCnt <= 0 ) ? 1 : iPoolCnt;
	*newPoolCnt = iPoolCnt;

	return 0;
}
int checkMaxAddr(char *ipmax, char *minAttr, char *poolAttr, int *newPoolCnt)

{
	char sMinAddr[20] = {0};
	char sMaxAddr[20] = {0};
	char sPoolStr[8] = {0};
	struct in_addr ipMinIP = {0}, ipMaxIP = {0};
	int iPoolCnt = 0;

	if ( NULL == ipmax || NULL == minAttr || NULL == poolAttr || NULL == newPoolCnt )
		return -1;

	snprintf(sMaxAddr, sizeof(sMaxAddr), "%s", ipmax);
	// check ip valid.
	if ( 1 != isValidMinMaxAddr(sMaxAddr) )
		return -1;


	if ( tcapi_get(DHCPD_NODEOPT60, minAttr, sMinAddr) < 0 )
		return -1;
		
	if ( 1 != inet_aton(sMinAddr, &ipMinIP) || 1 != inet_aton(sMaxAddr, &ipMaxIP) )
		return -1;

	iPoolCnt = ipMaxIP.s_addr + 1 - ipMinIP.s_addr;
	if ( iPoolCnt <= 0 )
		return -1;

	*newPoolCnt = iPoolCnt;
	return 0;
}

int8 getHGWMinAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char *minAttr = "startHGW";
	char sMinAddr[20] = {0};
	char dhcpType[4] = {0};

	tcapi_get("Lan_Dhcp", "type", dhcpType);
	strcpy(sMinAddr, "0.0.0.0");
	if ( 0 == strcmp(dhcpType, "1") ) // dhcp server mode.
		tcapi_get(DHCPD_NODEOPT60, minAttr, sMinAddr);

	return getTheValue(value, type, deviceNode, sMinAddr);
}
int8 setHGWMinAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char *minAttr = "startHGW";
	char *poolAttr = "pool_countHGW";
	char sMinAddr[20] = {0};
	char sPoolStr[8] = {0};
	int poolcnt = 0;

	snprintf(sMinAddr, sizeof(sMinAddr), "%s", (char *)value);
	if ( 0 != checkMinAddr(sMinAddr, minAttr, poolAttr, &poolcnt) )
		return -1;

	tcapi_set(DHCPD_NODEOPT60, minAttr, sMinAddr);
	snprintf(sPoolStr, sizeof(sPoolStr), "%d", poolcnt);
	tcapi_set(DHCPD_NODEOPT60, poolAttr, sPoolStr);
	SetCfgNodeFlag(DHCPD_NODENAME, -1);

	return 0;
}
int8 getHGWMinAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aHGWMinAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setHGWMinAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aHGWMinAddress;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getHGWMaxAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char *minAttr = "startHGW";
	char *poolAttr = "pool_countHGW";
	char sMinAddr[20] = {0};
	char sMaxAddr[20] = {0};
	char spoolcnt[8] = "1";
	char dhcpType[4] = {0};
	struct in_addr ipAddr = {0};

	tcapi_get("Lan_Dhcp", "type", dhcpType);
	strcpy(sMaxAddr, "0.0.0.0");
	if ( 0 == strcmp(dhcpType, "1") ) // dhcp server mode.
	{
		if ( 0 == tcapi_get(DHCPD_NODEOPT60, minAttr, sMinAddr) )
		{
			tcapi_get(DHCPD_NODEOPT60, poolAttr, spoolcnt);
			if ( 1 == inet_aton(sMinAddr, &ipAddr) )
			{
				ipAddr.s_addr += (atoi(spoolcnt) - 1);
				snprintf(sMaxAddr, sizeof(sMaxAddr), "%s", inet_ntoa(ipAddr));
			}
		}
	}

	return getTheValue(value, type, deviceNode, sMaxAddr);
}
int8 setHGWMaxAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char *minAttr = "startHGW";
	char *poolAttr = "pool_countHGW";
	char sMinAddr[20] = {0};
	char sMaxAddr[20] = {0};
	char sPoolStr[8] = {0};
	struct in_addr ipMinIP = {0}, ipMaxIP = {0};
	int iPoolCnt = 0;

	snprintf(sMaxAddr, sizeof(sMaxAddr), "%s", (char *)value);
	if ( 0 != checkMaxAddr(sMaxAddr, minAttr, poolAttr, &iPoolCnt) )
		return -1;

	SetCfgNodeFlag(DHCPD_NODENAME, -1);
	snprintf(sPoolStr, sizeof(sPoolStr), "%d", iPoolCnt);
	tcapi_set(DHCPD_NODEOPT60, poolAttr, sPoolStr);

	return 0;
}
int8 getHGWMaxAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aHGWMaxAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setHGWMaxAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aHGWMaxAddress;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getSTBMinAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char *minAttr = "startSTB";
	char sMinAddr[20] = {0};
	char dhcpType[4] = {0};

	tcapi_get("Lan_Dhcp", "type", dhcpType);
	strcpy(sMinAddr, "0.0.0.0");
	if ( 0 == strcmp(dhcpType, "1") ) // dhcp server mode.
		tcapi_get(DHCPD_NODEOPT60, minAttr, sMinAddr);

	return getTheValue(value, type, deviceNode, sMinAddr);
}
int8 setSTBMinAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char *minAttr = "startSTB";
	char *poolAttr = "pool_countSTB";
	char sMinAddr[20] = {0};
	char sPoolStr[8] = {0};
	int poolcnt = 0;

	snprintf(sMinAddr, sizeof(sMinAddr), "%s", (char *)value);
	if ( 0 != checkMinAddr(sMinAddr, minAttr, poolAttr, &poolcnt) )
		return -1;

	tcapi_set(DHCPD_NODEOPT60, minAttr, sMinAddr);
	snprintf(sPoolStr, sizeof(sPoolStr), "%d", poolcnt);
	tcapi_set(DHCPD_NODEOPT60, poolAttr, sPoolStr);
	SetCfgNodeFlag(DHCPD_NODENAME, -1);

	return 0;
}
int8 getSTBMinAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aSTBMinAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSTBMinAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aSTBMinAddress;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getSTBMaxAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char *minAttr = "startSTB";
	char *poolAttr = "pool_countSTB";
	char sMinAddr[20] = {0};
	char sMaxAddr[20] = {0};
	char spoolcnt[8] = "1";
	char dhcpType[4] = {0};
	struct in_addr ipAddr = {0};

	tcapi_get("Lan_Dhcp", "type", dhcpType);
	strcpy(sMaxAddr, "0.0.0.0");
	if ( 0 == strcmp(dhcpType, "1") ) // dhcp server mode.
	{
		if ( 0 == tcapi_get(DHCPD_NODEOPT60, minAttr, sMinAddr) )
		{
			tcapi_get(DHCPD_NODEOPT60, poolAttr, spoolcnt);
			if ( 1 == inet_aton(sMinAddr, &ipAddr) )
			{
				ipAddr.s_addr += (atoi(spoolcnt) - 1);
				snprintf(sMaxAddr, sizeof(sMaxAddr), "%s", inet_ntoa(ipAddr));
			}
		}
	}

	return getTheValue(value, type, deviceNode, sMaxAddr);
}
int8 setSTBMaxAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char *minAttr = "startSTB";
	char *poolAttr = "pool_countSTB";
	char sMinAddr[20] = {0};
	char sMaxAddr[20] = {0};
	char sPoolStr[8] = {0};
	struct in_addr ipMinIP = {0}, ipMaxIP = {0};
	int iPoolCnt = 0;

	snprintf(sMaxAddr, sizeof(sMaxAddr), "%s", (char *)value);
	if ( 0 != checkMaxAddr(sMaxAddr, minAttr, poolAttr, &iPoolCnt) )
		return -1;

	SetCfgNodeFlag(DHCPD_NODENAME, -1);
	snprintf(sPoolStr, sizeof(sPoolStr), "%d", iPoolCnt);
	tcapi_set(DHCPD_NODEOPT60, poolAttr, sPoolStr);

	return 0;
}
int8 getSTBMaxAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aSTBMaxAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSTBMaxAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aSTBMaxAddress;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getPhoneMinAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char *minAttr = "startPhone";
	char sMinAddr[20] = {0};
	char dhcpType[4] = {0};

	tcapi_get("Lan_Dhcp", "type", dhcpType);
	strcpy(sMinAddr, "0.0.0.0");
	if ( 0 == strcmp(dhcpType, "1") ) // dhcp server mode.
		tcapi_get(DHCPD_NODEOPT60, minAttr, sMinAddr);

	return getTheValue(value, type, deviceNode, sMinAddr);
}
int8 setPhoneMinAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char *minAttr = "startPhone";
	char *poolAttr = "pool_countPhone";
	char sMinAddr[20] = {0};
	char sPoolStr[8] = {0};
	int poolcnt = 0;

	snprintf(sMinAddr, sizeof(sMinAddr), "%s", (char *)value);
	if ( 0 != checkMinAddr(sMinAddr, minAttr, poolAttr, &poolcnt) )
		return -1;

	tcapi_set(DHCPD_NODEOPT60, minAttr, sMinAddr);
	snprintf(sPoolStr, sizeof(sPoolStr), "%d", poolcnt);
	tcapi_set(DHCPD_NODEOPT60, poolAttr, sPoolStr);
	SetCfgNodeFlag(DHCPD_NODENAME, -1);

	return 0;
}
int8 getPhoneMinAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aPhoneMinAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPhoneMinAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aPhoneMinAddress;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getPhoneMaxAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char *minAttr = "startPhone";
	char *poolAttr = "pool_countPhone";
	char sMinAddr[20] = {0};
	char sMaxAddr[20] = {0};
	char spoolcnt[8] = "1";
	char dhcpType[4] = {0};
	struct in_addr ipAddr = {0};

	tcapi_get("Lan_Dhcp", "type", dhcpType);
	strcpy(sMaxAddr, "0.0.0.0");
	if ( 0 == strcmp(dhcpType, "1") ) // dhcp server mode.
	{
		if ( 0 == tcapi_get(DHCPD_NODEOPT60, minAttr, sMinAddr) )
		{
			tcapi_get(DHCPD_NODEOPT60, poolAttr, spoolcnt);
			if ( 1 == inet_aton(sMinAddr, &ipAddr) )
			{
				ipAddr.s_addr += (atoi(spoolcnt) - 1);
				snprintf(sMaxAddr, sizeof(sMaxAddr), "%s", inet_ntoa(ipAddr));
			}
		}
	}

	return getTheValue(value, type, deviceNode, sMaxAddr);
}
int8 setPhoneMaxAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char *minAttr = "startPhone";
	char *poolAttr = "pool_countPhone";
	char sMinAddr[20] = {0};
	char sMaxAddr[20] = {0};
	char sPoolStr[8] = {0};
	struct in_addr ipMinIP = {0}, ipMaxIP = {0};
	int iPoolCnt = 0;

	snprintf(sMaxAddr, sizeof(sMaxAddr), "%s", (char *)value);
	if ( 0 != checkMaxAddr(sMaxAddr, minAttr, poolAttr, &iPoolCnt) )
		return -1;

	SetCfgNodeFlag(DHCPD_NODENAME, -1);
	snprintf(sPoolStr, sizeof(sPoolStr), "%d", iPoolCnt);
	tcapi_set(DHCPD_NODEOPT60, poolAttr, sPoolStr);

	return 0;
}
int8 getPhoneMaxAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aPhoneMaxAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPhoneMaxAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aPhoneMaxAddress;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getCameraMinAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char *minAttr = "startCamera";
	char sMinAddr[20] = {0};
	char dhcpType[4] = {0};

	tcapi_get("Lan_Dhcp", "type", dhcpType);
	strcpy(sMinAddr, "0.0.0.0");
	if ( 0 == strcmp(dhcpType, "1") ) // dhcp server mode.
		tcapi_get(DHCPD_NODEOPT60, minAttr, sMinAddr);

	return getTheValue(value, type, deviceNode, sMinAddr);
}
int8 setCameraMinAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char *minAttr = "startCamera";
	char *poolAttr = "pool_countCamera";
	char sMinAddr[20] = {0};
	char sPoolStr[8] = {0};
	int poolcnt = 0;

	snprintf(sMinAddr, sizeof(sMinAddr), "%s", (char *)value);
	if ( 0 != checkMinAddr(sMinAddr, minAttr, poolAttr, &poolcnt) )
		return -1;

	tcapi_set(DHCPD_NODEOPT60, minAttr, sMinAddr);
	snprintf(sPoolStr, sizeof(sPoolStr), "%d", poolcnt);
	tcapi_set(DHCPD_NODEOPT60, poolAttr, sPoolStr);
	SetCfgNodeFlag(DHCPD_NODENAME, -1);

	return 0;
}
int8 getCameraMinAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aCameraMinAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCameraMinAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aCameraMinAddress;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getCameraMaxAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char *minAttr = "startCamera";
	char *poolAttr = "pool_countCamera";
	char sMinAddr[20] = {0};
	char sMaxAddr[20] = {0};
	char spoolcnt[8] = "1";
	char dhcpType[4] = {0};
	struct in_addr ipAddr = {0};

	tcapi_get("Lan_Dhcp", "type", dhcpType);
	strcpy(sMaxAddr, "0.0.0.0");
	if ( 0 == strcmp(dhcpType, "1") ) // dhcp server mode.
	{
		if ( 0 == tcapi_get(DHCPD_NODEOPT60, minAttr, sMinAddr) )
		{
			tcapi_get(DHCPD_NODEOPT60, poolAttr, spoolcnt);
			if ( 1 == inet_aton(sMinAddr, &ipAddr) )
			{
				ipAddr.s_addr += (atoi(spoolcnt) - 1);
				snprintf(sMaxAddr, sizeof(sMaxAddr), "%s", inet_ntoa(ipAddr));
			}
		}
	}

	return getTheValue(value, type, deviceNode, sMaxAddr);
}
int8 setCameraMaxAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char *minAttr = "startCamera";
	char *poolAttr = "pool_countCamera";
	char sMinAddr[20] = {0};
	char sMaxAddr[20] = {0};
	char sPoolStr[8] = {0};
	struct in_addr ipMinIP = {0}, ipMaxIP = {0};
	int iPoolCnt = 0;

	snprintf(sMaxAddr, sizeof(sMaxAddr), "%s", (char *)value);
	if ( 0 != checkMaxAddr(sMaxAddr, minAttr, poolAttr, &iPoolCnt) )
		return -1;

	SetCfgNodeFlag(DHCPD_NODENAME, -1);
	snprintf(sPoolStr, sizeof(sPoolStr), "%d", iPoolCnt);
	tcapi_set(DHCPD_NODEOPT60, poolAttr, sPoolStr);

	return 0;
}
int8 getCameraMaxAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aCameraMaxAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCameraMaxAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aCameraMaxAddress;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getComputerMinAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char *minAttr = "start";
	char sMinAddr[20] = {0};
	char dhcpType[4] = {0};

	tcapi_get("Lan_Dhcp", "type", dhcpType);
	strcpy(sMinAddr, "0.0.0.0");
	if ( 0 == strcmp(dhcpType, "1") ) // dhcp server mode.
		tcapi_get(DHCPD_NODEOPT60, minAttr, sMinAddr);

	return getTheValue(value, type, deviceNode, sMinAddr);
}
int8 setComputerMinAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char *minAttr = "start";
	char *poolAttr = "pool_count";
	char sMinAddr[20] = {0};
	char sPoolStr[8] = {0};
	int poolcnt = 0;

	snprintf(sMinAddr, sizeof(sMinAddr), "%s", (char *)value);
	if ( 0 != checkMinAddr(sMinAddr, minAttr, poolAttr, &poolcnt) )
		return -1;

	tcapi_set(DHCPD_NODEOPT60, minAttr, sMinAddr);
	snprintf(sPoolStr, sizeof(sPoolStr), "%d", poolcnt);
	tcapi_set(DHCPD_NODEOPT60, poolAttr, sPoolStr);
	SetCfgNodeFlag(DHCPD_NODENAME, -1);

	return 0;
}
int8 getComputerMinAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aComputerMinAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setComputerMinAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aComputerMinAddress;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getComputerMaxAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char *minAttr = "start";
	char *poolAttr = "pool_count";
	char sMinAddr[20] = {0};
	char sMaxAddr[20] = {0};
	char spoolcnt[8] = "1";
	char dhcpType[4] = {0};
	struct in_addr ipAddr = {0};

	tcapi_get("Lan_Dhcp", "type", dhcpType);
	strcpy(sMaxAddr, "0.0.0.0");
	if ( 0 == strcmp(dhcpType, "1") ) // dhcp server mode.
	{
		if ( 0 == tcapi_get(DHCPD_NODEOPT60, minAttr, sMinAddr) )
		{
			tcapi_get(DHCPD_NODEOPT60, poolAttr, spoolcnt);
			if ( 1 == inet_aton(sMinAddr, &ipAddr) )
			{
				ipAddr.s_addr += (atoi(spoolcnt) - 1);
				snprintf(sMaxAddr, sizeof(sMaxAddr), "%s", inet_ntoa(ipAddr));
			}
		}
	}

	return getTheValue(value, type, deviceNode, sMaxAddr);
}
int8 setComputerMaxAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char *minAttr = "start";
	char *poolAttr = "pool_count";
	char sMinAddr[20] = {0};
	char sMaxAddr[20] = {0};
	char sPoolStr[8] = {0};
	struct in_addr ipMinIP = {0}, ipMaxIP = {0};
	int iPoolCnt = 0;

	snprintf(sMaxAddr, sizeof(sMaxAddr), "%s", (char *)value);
	if ( 0 != checkMaxAddr(sMaxAddr, minAttr, poolAttr, &iPoolCnt) )
		return -1;

	SetCfgNodeFlag(DHCPD_NODENAME, -1);
	snprintf(sPoolStr, sizeof(sPoolStr), "%d", iPoolCnt);
	tcapi_set(DHCPD_NODEOPT60, poolAttr, sPoolStr);

	return 0;
}
int8 getComputerMaxAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aComputerMaxAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setComputerMaxAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aComputerMaxAddress;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getOPTION60EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char active[8] = {0};

	tcapi_get(DHCPD_NODEOPT60, "Active", active);
	strcpy(active, ( 0 == strcmp(active, "Yes") ) ? "1" : "0");

	return getTheValue(value, type, deviceNode, active);
}
int8 setOPTION60EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 b_active;
	char s_active[8] = {0};

	b_active = *((uint8*)value);
	switch ( b_active )
	{
		case 0:
			strcpy(s_active, "No");
			break;
		case 1:
			strcpy(s_active, "Yes");
			break;
		default:
			return -1;
	}

	tcapi_set(DHCPD_NODEOPT60, "Active", s_active);
	SetCfgNodeFlag(DHCPD_NODENAME, -1);

	return 0;
}
int8 getOPTION60EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aOPTION60Enable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setOPTION60EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aOPTION60Enable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getOPTION125EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char active[8] = {0};

	tcapi_get(DHCPD_NODEOPT125, "Active", active);
	strcpy(active, ( 0 == strcmp(active, "Yes") ) ? "1" : "0");

	return getTheValue(value, type, deviceNode, active);
}
int8 setOPTION125EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 b_active;
	char s_active[8] = {0};

	b_active = *((uint8*)value);
	switch ( b_active )
	{
		case 0:
			strcpy(s_active, "No");
			break;
		case 1:
			strcpy(s_active, "Yes");
			break;
		default:
			return -1;
	}

	tcapi_set(DHCPD_NODEOPT125, "Active", s_active);
	SetCfgNodeFlag(DHCPD_NODENAME, -1);

	return 0;
}
int8 getOPTION125EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aOPTION125Enable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setOPTION125EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aOPTION125Enable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getOPTION16EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char active[8] = {0};

	tcapi_get(DHCP6S_NODEOPT16, "Active", active);
	strcpy(active, ( 0 == strcmp(active, "Yes") ) ? "1" : "0");

	return getTheValue(value, type, deviceNode, active);
}
int8 setOPTION16EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 b_active;
	char s_active[8] = {0};

	b_active = *((uint8*)value);
	switch ( b_active )
	{
		case 0:
			strcpy(s_active, "No");
			break;
		case 1:
			strcpy(s_active, "Yes");
			break;
		default:
			return -1;
	}

	tcapi_set(DHCP6S_NODEOPT16, "Active", s_active);
	SetCfgNodeFlag(DHCP6S_NODENAME, -1);

	return 0;
}
int8 getOPTION16EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aOPTION16Enable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setOPTION16EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aOPTION16Enable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getOPTION17EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char active[8] = {0};

	tcapi_get(DHCP6S_NODEOPT17, "Active", active);
	strcpy(active, ( 0 == strcmp(active, "Yes") ) ? "1" : "0");

	return getTheValue(value, type, deviceNode, active);
}
int8 setOPTION17EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 b_active;
	char s_active[8] = {0};

	b_active = *((uint8*)value);
	switch ( b_active )
	{
		case 0:
			strcpy(s_active, "No");
			break;
		case 1:
			strcpy(s_active, "Yes");
			break;
		default:
			return -1;
	}

	tcapi_set(DHCP6S_NODEOPT17, "Active", s_active);
	SetCfgNodeFlag(DHCP6S_NODENAME, -1);

	return 0;
}
int8 getOPTION17EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aOPTION17Enable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setOPTION17EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aOPTION17Enable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}


int8 getLanIPv6CfgDomainNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char val_buf[72] = {0};
	char domainName[72] = {0};

	tcapi_get("Dhcp6s_Entry", "DomainName", val_buf);
	
	return getTheValue(value, type, deviceNode, val_buf);
}

int8 domainname_check(char *domain)
{
	char s_domain[128] = {0};
	char *pos = NULL;
	int domain_len = 0;
	int idx = 0;
	int per_domain_maxlen = 63;

	if ( NULL == domain )
		return -1;

	if ( '\0' == domain[0] )
		return 0; // no need to check.

	if ( !isalpha(domain[0]) )
		return -1;

	strncpy(s_domain, domain, sizeof(s_domain) - 1);
	pos = strtok(s_domain, ".");
	while ( pos )
	{
		domain_len = strlen(pos);
		if ( domain_len > per_domain_maxlen )
			return -1;
		if ( !isalnum(pos[domain_len - 1]) )
			return -1;
		for ( idx = 0; idx < domain_len; idx ++ )
		{
			if ( !isalnum(pos[idx]) && '-' != pos[idx] )
				return -1;
		}

		pos = strtok(NULL, ".");
	}

	return 0;
}

int8 setLanIPv6CfgDomainNameValue(void* value, struct DeviceNode*  deviceNode)
{
	char val_buf[72] = {0};
	
	snprintf(val_buf, sizeof(val_buf), "%s", (char*)(value));

	if(strlen(val_buf) > 64)
		return -1;

	if ( 0 != domainname_check(val_buf) )
		return -1;	
	
	tcapi_set("Dhcp6s_Entry", "DomainName", val_buf);
	
	SetCfgNodeFlag("Dhcp6s", -1);
	
	return 0;
}
int8 getLanIPv6CfgDomainNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aIPv6CfgDomainName;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLanIPv6CfgDomainNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aIPv6CfgDomainName;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getLanIPv6CfgDNSConfigTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char val_buf[8] = {0};
	char dnsType[48] = {0};
	
	tcapi_get("Dhcp6s_Entry", "DNSType", val_buf);

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	if( 0 == strcmp(val_buf, "1") )
		strcpy(dnsType, "Static");
	else
		strcpy(dnsType, "DHCP");
#else
	if(!strcmp(val_buf, "0"))
		strcpy(dnsType, "WANConnection");
	else if(!strcmp(val_buf, "1"))
		strcpy(dnsType, "Static");
	else if(!strcmp(val_buf, "2"))
		strcpy(dnsType, "HGWProxy");
	else		
		strcpy(dnsType, "HGWProxy");
#endif
	
	return getTheValue(value, type, deviceNode, dnsType);
}

int8 setLanIPv6CfgDNSConfigTypeValue(void* value, struct DeviceNode*  deviceNode)
{	
	char dnsType[48] = {0};	
	char val_buf[8] = {0};

	sprintf(dnsType, "%s", (char*)value);
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	if( 0 == strcmp(dnsType, "Static") )
		strcpy(val_buf, "1");
	else if( 0 == strcmp(dnsType, "DHCP") )
		strcpy(val_buf, "0");
	else
		return -1;
#else
	if(!strcmp(dnsType, "WANConnection"))
		strcpy(val_buf, "0");
	else if(!strcmp(dnsType, "Static"))
		strcpy(val_buf, "1");
	else if(!strcmp(dnsType, "HGWProxy"))
		strcpy(val_buf, "2");
	else
		return -1;
#endif
	tcapi_set("Dhcp6s_Entry", "DNSType", val_buf);
	SetCfgNodeFlag("Dhcp6s", -1);
	return 0;
}
int8 getLanIPv6CfgDNSConfigTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aIPv6CfgDNSConfigType;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLanIPv6CfgDNSConfigTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aIPv6CfgDNSConfigType;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int getIfindexByWanConnection(char *connection)
{
	DeviceNodePtr Node = NULL;
	char *tmp = NULL;
	char DefaultIface_WAN[]="InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";
	int len = 0;
	uint32 pvc_idx = 0, entry_idx = 0;
	char tempvalue[2];
	char wan_node[32] = {0};
	char str_servicelist[32] = {0};
	char str_ipversion[16] = {0};
	char str_wanmode[8] = {0};
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	uint32 pvctmp;
#endif
	if ( NULL == connection )
		return -1;

	len = strlen(DefaultIface_WAN);
	if( 0 == strncmp(DefaultIface_WAN, connection, len) )//WAN interface, get the gateway.
	{
		/*check the value is valid or not.*/
		cwmpSearchDeviceNodeByName(connection, rpc.IGDDeviceRootPtr, &Node);
		if( Node == NULL )
			return -1;

		memset(tempvalue, 0, sizeof(tempvalue));
		tempvalue[0] = connection[len];
		tempvalue[1] = '\0';
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
		pvc_idx = atoi(tempvalue);
		pvctmp = pvc_idx;
		if(FindPVCMapping(&pvc_idx) != 0)
			return -1;
#else
		if( connection[len]<'1' || connection[len]>'8' )
			return -1;
		pvc_idx = atoi(tempvalue) - 1;
#endif

		if( (tmp = strstr(connection, "WANPPPConnection")) != NULL )
		{
			memset(tempvalue, 0, sizeof(tempvalue));
			tempvalue[0] = *(tmp + strlen("WANPPPConnection") + 1);
			tempvalue[1] = '\0';
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
			entry_idx = atoi(tempvalue);
			FindWanNodeMapping(&pvctmp, &entry_idx);
#else	
			entry_idx = atoi(tempvalue) - 1;
#endif
		}
		else if ( (tmp = strstr(connection, "WANIPConnection")) != NULL )
		{
			memset(tempvalue, 0, sizeof(tempvalue));
			tempvalue[0] = *(tmp + strlen("WANIPConnection") + 1);
			tempvalue[1] = '\0';
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
			entry_idx = atoi(tempvalue);
			FindWanNodeMapping(&pvctmp, &entry_idx);
#else	
			entry_idx = atoi(tempvalue) - 1;
#endif
		}
		else
			return -1;

		sprintf(wan_node, "Wan_PVC%d_Entry%d", pvc_idx, entry_idx);
		if ( tcapi_get(wan_node, "ServiceList", str_servicelist) < 0 )
			str_servicelist[0] = '\0';
		if ( tcapi_get(wan_node, "IPVERSION", str_ipversion) < 0 )
			str_ipversion[0] = '\0';
		if ( tcapi_get(wan_node, "WanMode", str_wanmode) < 0 )
			str_wanmode[0] = '\0';

		if ( ( NULL == strstr(str_servicelist, "INTERNET")
			&& NULL == strstr(str_servicelist, "OTHER") )
			|| 0 == strcmp(str_ipversion, "IPv4")
			|| 0 != strcmp(str_wanmode, "Route") )
			return -1;

		return (pvc_idx * MAX_SMUX_NUM + entry_idx);
	}

	return -1;
}
int createWanConnectionByIfindex(int ifidx, char *out_connection)
{
	uint32 pvc_idx = 0, entry_idx = 0;
	char isp[10] = {0};
	char wan_node[32] = {0};
	char bridgeType[32] = {0};
	int ppp_ip_flag = 0; // 1: ppp, 0:ip

	if ( NULL == out_connection )
		return -1;

	pvc_idx = ifidx / MAX_SMUX_NUM;
	entry_idx = ifidx % MAX_SMUX_NUM;

	sprintf(wan_node, "Wan_PVC%d_Entry%d", pvc_idx, entry_idx);
	if ( tcapi_get(wan_node, "ISP", isp) < 0 )
		strcpy(isp, "0");

	if ( 0 == strcmp(PPP_MODE, isp) )
		ppp_ip_flag = 1;
	else 
	{
		if ( 0 == strcmp(BRIDGE_MODE, isp) )
		{
			if ( tcapi_get(wan_node, "BridgeMode", bridgeType) < 0 )
				bridgeType[0] = '\0';
			if( 0 == strcmp(bridgeType, "PPPoE_Bridged") )
				ppp_ip_flag = 1;
		}
	}
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	GetWanNodeMapping(&pvc_idx, &entry_idx);
	sprintf(out_connection, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.%s.%d.",
			pvc_idx, ( 1 == ppp_ip_flag ) ? "WANPPPConnection" : "WANIPConnection", entry_idx);
#else
	sprintf(out_connection, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.%s.%d.",
			pvc_idx + 1, ( 1 == ppp_ip_flag ) ? "WANPPPConnection" : "WANIPConnection", entry_idx + 1);
#endif


	return 0;
}
int8 getLanIPv6CfgDNSWANConnectionValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char val_buf[100] = {0};
	int ifidx = 0;

	if ( tcapi_get("Dhcp6s_Entry", "DNSWANConnection", val_buf) < 0 )
		strcpy(val_buf, "");
	else
	{
		if ( '\0' != val_buf[0] )
		{
			ifidx = atoi(val_buf);
			createWanConnectionByIfindex(ifidx, val_buf);
		}
	}

	return getTheValue(value, type, deviceNode, val_buf);
}

int8 setLanIPv6CfgDNSWANConnectionValue(void* value, struct DeviceNode*  deviceNode)
{	
	char dnsType[100] = {0};
	int ifidx = 0;

	sprintf(dnsType, "%s", (char*)value);
	ifidx = getIfindexByWanConnection(dnsType);
	if ( ifidx < 0 )
		return -1;

	sprintf(dnsType, "%d", ifidx);	
	tcapi_set("Dhcp6s_Entry", "DNSWANConnection", dnsType);
	SetCfgNodeFlag("Dhcp6s", -1);

	return 0;
}
int8 getLanIPv6CfgDNSWANConnectionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aIPv6CfgDNSWANConnection;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLanIPv6CfgDNSWANConnectionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aIPv6CfgDNSWANConnection;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getLanIPv6CfgDNSServersValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char val_buf[132] = {0};	
	char dns1[64] = {0};
	char dns2[64] = {0};
	
	tcapi_get("Dhcp6s_Entry", "DNSserver", dns1);
	tcapi_get("Dhcp6s_Entry", "SecDNSserver", dns2);

	if ( '\0' != dns1[0] )
	{
		if ( '\0' == dns2[0] )
			sprintf(val_buf, "%s", dns1);
		else
			sprintf(val_buf, "%s,%s", dns1, dns2);
	}

	return getTheValue(value, type, deviceNode, val_buf);
}

int8 setLanIPv6CfgDNSServersValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char originValue[32] = {0};
	char dnsIpAddrValue[129] = {0};
	char dns1[129] = {0};
	char dns2[129] = {0};
	unsigned char tmp[100] = {0};
	char *tp = NULL;
	uint8 ret = 0;		

	if(strlen((char*)value) > 128)
		return -1;

	ret = tcapi_get("Dhcp6s_Entry", "Mode", originValue);
	if(ret){
		return -1;
	}

	sprintf(dnsIpAddrValue, "%s", (char*)value);

	tp = strstr(dnsIpAddrValue, ",");
	if(tp == NULL)
	{
		if(strlen(dnsIpAddrValue) > 64)
			return -1;
		
		if(inet_pton6((const char*)dnsIpAddrValue, tmp)!= 1)
		{
			tcdbg_printf("IPv6 Address Error.\n");
			return -1;	
		}
			
		tcapi_set("Dhcp6s_Entry", "DNSserver", dnsIpAddrValue);
	}
	else
	{
		if(strstr((tp + 1), ",") != NULL)
			return -1;
		
		strncpy(dns1, dnsIpAddrValue, (tp - dnsIpAddrValue));
		sprintf(dns2, "%s", (tp + 1));

		if(strlen(dns1) > 64 || strlen(dns2) > 64)
			return -1;
		
		if(inet_pton6((const char*)dns1, tmp)!= 1)
		{
			tcdbg_printf("IPv6 Address Error.\n");
			return -1;	
		}	

		if(inet_pton6((const char*)dns2, tmp)!= 1)
		{
			tcdbg_printf("IPv6 Address Error.\n");
			return -1;	
		}	

		tcapi_set("Dhcp6s_Entry", "DNSserver", dns1);
		tcapi_set("Dhcp6s_Entry", "SecDNSserver", dns2);
	}
	
	SetCfgNodeFlag ( "Dhcp6s", -1 );
	
	return 0;
}
int8 getLanIPv6CfgDNSServersAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aIPv6CfgDNSServers;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLanIPv6CfgDNSServersAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aIPv6CfgDNSServers;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getLanIPv6CfgRadvdPrefixModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char val_buf[8] = {0};
	char mode[48] = {0};

	if ( tcapi_get("Radvd_Entry", "AutoPrefix",val_buf) < 0 )
		strcpy(val_buf, "0");

	if( 0 == strcmp(val_buf, "0"))
		strcpy(mode, "WANDelegated");
	else
		strcpy(mode, "Static");	
	
	return getTheValue(value, type, deviceNode, mode);
}

int8 setLanIPv6CfgRadvdPrefixModeValue(void* value, struct DeviceNode*  deviceNode)
{
	char mode[48] = {0};
	
	sprintf(mode, "%s", (char*)(value));
	
	if(!strcmp(mode, "WANDelegated"))
	{
		tcapi_set("Radvd_Entry", "AutoPrefix", "0");
		tcapi_set("Radvd_Entry", "Mode", "0");
	}
	else if(!strcmp(mode, "Static"))
	{
		tcapi_set("Radvd_Entry", "AutoPrefix", "1");
		tcapi_set("Radvd_Entry", "Mode", "1");
	}
	else
		return -1;
	
	SetCfgNodeFlag("Radvd", -1);
	return 0;
}
int8 getLanIPv6CfgRadvdPrefixModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aIPv6CfgRadvdPrefixMode;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLanIPv6CfgRadvdPrefixModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aIPv6CfgRadvdPrefixMode;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getLanIPv6CfgRadvdDelegatedWanConnValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char val_buf[100] = {0};
	int ifidx = 0;

	if ( tcapi_get("Radvd_Entry", "DelegatedWanConnection", val_buf) < 0 )
		strcpy(val_buf, "");
	else
	{
		if ( '\0' != val_buf[0] )
		{
			ifidx = atoi(val_buf);
			createWanConnectionByIfindex(ifidx, val_buf);
		}
	}

	return getTheValue(value, type, deviceNode, val_buf);
}
int8 setLanIPv6CfgRadvdDelegatedWanConnValue(void* value, struct DeviceNode*  deviceNode)
{
	char val_buf[100] = {0};
	int ifidx = 0;

	snprintf(val_buf, sizeof(val_buf), "%s", (char*)(value));
	ifidx = getIfindexByWanConnection(val_buf);
	if ( ifidx < 0 )
		return -1;

	sprintf(val_buf, "%d", ifidx);	
	tcapi_set("Radvd_Entry", "DelegatedWanConnection", val_buf);
	
	SetCfgNodeFlag("Radvd", -1);
	return 0;
}
int8 getLanIPv6CfgRadvdDelegatedWanConnAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aIPv6CfgRadvdDelegatedWanConn;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLanIPv6CfgRadvdDelegatedWanConnAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aIPv6CfgRadvdDelegatedWanConn;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getLanIPv6CfgRadvdPrefixValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char prefix_addr[64] = {0};
	char prefix_len[8] = {0};
	char val_buf[72] = {0};
	
	tcapi_get("Radvd_Entry", "PrefixIPv6", prefix_addr);
	tcapi_get("Radvd_Entry", "Prefixv6Len", prefix_len);

	sprintf(val_buf, "%s/%s",prefix_addr,prefix_len);
	
	return getTheValue(value, type, deviceNode, val_buf);
}

int8 setLanIPv6CfgRadvdPrefixValue(void* value, struct DeviceNode*  deviceNode)
{
	char prefix_addr[64] = {0};
	char prefix_len[8] = {0};
	unsigned char tmp[100] = {0};
	char* ptr = NULL;
	int len = 0;
	ptr = strchr((char*)value, '/');
	if(ptr == NULL)
		return -1;
	*ptr = '\0';

	ptr = ptr + 1;	

	if(strlen((char*)value) > 40)
		return -1;
	
	if(strlen((char*)ptr) > 2)
		return -1;
	
	strcpy(prefix_addr,(char*)value);
	strcpy(prefix_len, ptr);

	len = atoi(prefix_len);
	if(len < 16 || len > 64)		
		return -1;
	
	if(inet_pton6((const char*)prefix_addr, tmp)!= 1)
	{
		tcdbg_printf("IPv6 Address Error.\n");
		return -1;	
	}

	tcapi_set("Radvd_Entry", "PrefixIPv6", prefix_addr);
	tcapi_set("Radvd_Entry", "Prefixv6Len", prefix_len);
	
	SetCfgNodeFlag("Radvd", -1);
	return 0;
}
int8 getLanIPv6CfgRadvdPrefixAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aIPv6CfgRadvdPrefix;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLanIPv6CfgRadvdPrefixAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aIPv6CfgRadvdPrefix;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getLanIPv6CfgRadvdPltimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char Vltime[24] = {0};

	tcapi_get("Radvd_Entry", "PreferredLifetime", Vltime);

	return getTheValue(value, type, deviceNode, Vltime);
}

int8 setLanIPv6CfgRadvdPltimeValue(void* value, struct DeviceNode*  deviceNode)
{
	char var_buf[32] = {0};
	uint32 pltime = *((uint32*)value);
	uint32 vltime = 0;

	if(pltime > 4294967295 || pltime < 60)
		return -1;
	tcapi_get("Radvd_Entry", "ValidLifetime", var_buf);
	
	sscanf(var_buf, "%d", &vltime);

	if(vltime < pltime )
		return -1;
	
	sprintf(var_buf, "%lu", pltime);
	
	tcapi_set("Radvd_Entry", "PreferredLifetime", var_buf);
	
	SetCfgNodeFlag("Radvd", -1);
	return 0;
}
int8 getLanIPv6CfgRadvdPltimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aIPv6CfgRadvdPltime;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLanIPv6CfgRadvdPltimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aIPv6CfgRadvdPltime;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

int8 getLanIPv6CfgRadvdVltimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char Vltime[24] = {0};

	tcapi_get("Radvd_Entry", "ValidLifetime", Vltime);

	return getTheValue(value, type, deviceNode, Vltime);
}

int8 setLanIPv6CfgRadvdVltimeValue(void* value, struct DeviceNode*  deviceNode)
{
	char var_buf[32] = {0};
	uint32 vltime = *((uint32*)value);
	uint32 pltime = 0;

	if(vltime > 4294967295 || vltime < 60)
		return -1;
	tcapi_get("Radvd_Entry", "PreferredLifetime", var_buf);
	
	sscanf(var_buf, "%d", &pltime);

	if(vltime < pltime )
		return -1;
			
	sprintf(var_buf, "%lu", vltime);
	
	tcapi_set("Radvd_Entry", "ValidLifetime", var_buf);
	
	SetCfgNodeFlag("Radvd", -1);
	return 0;
}
int8 getLanIPv6CfgRadvdVltimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcLanDeviceAttr.aIPv6CfgRadvdVltime;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLanIPv6CfgRadvdVltimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcLanDeviceAttr.aIPv6CfgRadvdVltime;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}

#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER) && defined(TCSUPPORT_CT_USB)
int isUSBConnected()
{
	char cmd[64] = {0};
	char buffer[256] = {0};
	char *tmpUsbStatusPath = "/tmp/usb_status";

	sprintf(cmd, "/bin/ls /tmp/mnt > %s", tmpUsbStatusPath);
	system(cmd);
	memset(buffer, 0, sizeof(buffer));
	fileRead(tmpUsbStatusPath, buffer, sizeof(buffer) - 1);
	unlink(tmpUsbStatusPath);

	if ( NULL != strstr(buffer, "usb") )
		return 1;

	return 0;
}
int8 getUSBPortStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	if ( 1 == isUSBConnected())
		strcpy(result, "Harddisk");
	else
		strcpy(result, "None");

	return getTheValue(value, type, deviceNode, result);
}
int8 getUSBPortStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcLanDeviceAttr.aUSBPortStatus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setUSBPortStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result;

	attribute = &tcLanDeviceAttr.aUSBPortStatus;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LANDEVICE_ATTR_FLAG;
	return result;
}
#endif

