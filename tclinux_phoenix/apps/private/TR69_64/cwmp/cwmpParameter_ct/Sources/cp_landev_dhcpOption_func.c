#include "Global_res.h"
#include "cp_landev_dhcpOption.h"
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>

static int DHCPConSerOptionNum = 1; //only 1 instance, corresponding to DHCP option 240

#define VENDORCLASSID_MAXBASE64 (((VENDORCLASSID_MAX + 2)/3 )*4)  //The base64 encoded size will be at most 4/3 the size of the decoded
char base64EncodeChrSet[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

static char secondPoolStartIP[20] = {0};
static char secondPoolEndIP[20] = {0};
static char secondPoolgateway[20] = {0};
static char secondPoolNetMask[20] = {0};

int8 getdhcpOption_EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name);	
	/* instance 1: option 60, 
	 * instance 2: option 240,
	 */
	 if(1 == instance)
		tcapi_get(DHCPD_OPT60, "Active", tmp);
	 else if(2 == instance)
		tcapi_get(DHCPD_OPT240, "Active", tmp);
	if(0 == strcmp(tmp, "Yes"))
		sprintf(result, "%d", 1);
	else if(0 == strcmp(tmp, "No"))
		sprintf(result, "%d", 0);

	return getTheValue(value, type, deviceNode, result);
}

int8 setdhcpOption_EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[20] = {0};
	char nodeName[20] = {0};
	int instance = 0;	
	uint8 iEnable;
	
	iEnable = *((uint8*)value);
	if(1 == iEnable){
		sprintf(tmp, "%s", "Yes");
	} 
	else if (0 == iEnable){
		sprintf(tmp, "%s", "No");
	}
	else{
		tc_printf("==>setdhcpOption_EnableValue:value should be 0 or 1.\n");
		return -1;
	}	
	
	instance = atoi(deviceNode->parentNode->name);	
	/* instance 1: option 60, 
	 * instance 2: option 240,
	 */
	 if(1 == instance)
		strcpy(nodeName, DHCPD_OPT60);
	 else if(2 == instance)
		strcpy(nodeName, DHCPD_OPT240);
	 else{
		tc_printf("==>setdhcpOption_EnableValue:instance value should be 0 or 1.\n");
		return -1;
	}	
	tcapi_set(nodeName, "Active", tmp);
	
	SetCfgNodeFlag(DHCPD_NODENAME, -1);
        return 0;
}

int8 getdhcpOption_EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpOptionAttr.aEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setdhcpOption_EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpOptionAttr.aEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPOPTION_ATTR_FLAG;
	return result;
}

int8 getdhcpOption_TagValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[4] = {0};	
	int instance = 1;
	
	instance = atoi(deviceNode->parentNode->name);	
	if(instance == 1){
		strcpy(result, "60"); 
	}
	else if(instance == 2){
		strcpy(result, "240"); 
	}

	return getTheValue(value, type, deviceNode, result);
}

int8 setdhcpOption_TagValue(void* value, struct DeviceNode*  deviceNode)
{
        return 0;
}

int8 getdhcpOption_TagAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpOptionAttr.aTag;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setdhcpOption_TagAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpOptionAttr.aTag;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPOPTION_ATTR_FLAG;
	return result;
}

int8 getdhcpOption_ValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  result[VENDORCLASSID_MAXBASE64] = {0};	
	char originValue[40] = {0};
	int instance= 0;
	int len = 0;
	
	instance = atoi(deviceNode->parentNode->name);	
	if(1 == instance)
		tcapi_get(DHCPD_OPT60, "VendorID", originValue);
	else if(2 == instance)
		tcapi_get(DHCPD_OPT240, "Value", originValue);
	else{
		tc_printf("==>getdhcpOption_ValueValue:instance value should be 0 or 1.\n");
		return -1;
	}	
	base64Encode(originValue, result, strlen(originValue), &len);

	return getTheValue(value, type, deviceNode, result);
}

int8 setdhcpOption_ValueValue(void* value, struct DeviceNode*  deviceNode)
{
	char * tempValue = NULL;
	char venderID[VENDORCLASSID_MAXBASE64] = {0};
	int instance = 0;

	tempValue = (char*)(value);
	if(strlen(tempValue) >= VENDORCLASSID_MAXBASE64)
		return -1;
	strcpy(venderID, tempValue);
	decodeBase64(venderID);

	instance = atoi(deviceNode->parentNode->name);	
	if(instance == 1){
		tcapi_set(DHCPD_OPT60, "VendorID", venderID);
	}
	else if(instance == 2){
		tcapi_set(DHCPD_OPT240, "Value", venderID); 
	}
	 else{
		tc_printf("==>setdhcpOption_ValueValue:instance value should be 0 or 1.\n");
		return -1;
	}	
	SetCfgNodeFlag(DHCPD_NODENAME, -1);
        return 0;
}

int8 getdhcpOption_ValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpOptionAttr.aValue;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setdhcpOption_ValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpOptionAttr.aValue;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPOPTION_ATTR_FLAG;
	return result;
}

int8 DHCPProfileCheckIP(cfgNode_ptr pCfgNode)
{
	char vStartIP[20] = {0};
	char vPoolCount[8] = {0};
	char vGateWay[20] = {0};
	char vSubnetMask[20] = {0};
	char vLanIP[20] = {0};
	char subnetMaskValue[20] = {0};
	char poolCountValue[8] = {0};
	struct in_addr vMaxAddress;
	struct in_addr netmask;
	struct in_addr lanStartIP;
	struct in_addr lanEndIP;
	struct in_addr gatewayIP;

	tcapi_get(DHCPD_OPT60, "start", vStartIP);
	tcapi_get(DHCPD_OPT60, "pool_count", vPoolCount);
	tcapi_get(DHCPD_OPT60, "router", vGateWay);
	tcapi_get(DHCPD_OPT60, "subnetMask", vSubnetMask);
	tcapi_get("Lan_Entry0", "IP", vLanIP);

	if(strlen(secondPoolStartIP) == 0){ // ACS didn't set this value, fetch from flash
		strcpy(secondPoolStartIP, vStartIP);
	}
	if(strlen(secondPoolEndIP) == 0){ // ACS didn't set this value, fetch from flash
		inet_aton(vStartIP, &vMaxAddress);
		vMaxAddress.s_addr += atoi(vPoolCount) - 1;
		strcpy(secondPoolEndIP, inet_ntoa(vMaxAddress));
	}
	if(strlen(secondPoolgateway) == 0){// ACS didn't set this value, fetch from flash
		if(strlen(vGateWay) == 0
		|| strcmp(vGateWay, "0.0.0.0") == 0)
			strcpy(secondPoolgateway, vLanIP);
		else
			strcpy(secondPoolgateway, vGateWay);
	}

	if(strlen(secondPoolNetMask) == 0)
		strcpy(secondPoolNetMask, vSubnetMask);
	
	/*check parameters value */
	if(strcmp(secondPoolStartIP, secondPoolgateway) == 0) { //equal to gateway, Invalid
		tc_printf("\n==>Conditional pool start IP can't be equal to gateway\n");
		goto End;
	}

	inet_aton(secondPoolStartIP, &lanStartIP);
	inet_aton(secondPoolEndIP, &lanEndIP);
	inet_aton(secondPoolgateway, &gatewayIP);
	inet_aton(secondPoolNetMask, &netmask);

	if((gatewayIP.s_addr & netmask.s_addr) != (lanStartIP.s_addr & netmask.s_addr)) {// not in the same subnet, Invalid
		tc_printf("\n==>Conditional pool start IP should be in the same subnet with gateway\n");
		goto End;

	}
	if(strcmp(secondPoolEndIP, secondPoolgateway) == 0) {//equal to gateway, illage
		tc_printf("\n==>Conditional pool end IP can't be equal to gateway\n");
		goto End;

	}
	if((gatewayIP.s_addr & netmask.s_addr) != (lanEndIP.s_addr & netmask.s_addr)) {// not in the same subnet, Invalid
		tc_printf("\n==>Conditional pool end IP should be in the same subnet with gateway\n");
		goto End;

	}
	if(lanStartIP.s_addr > lanEndIP.s_addr) {
		tc_printf("\n==>Conditional pool end IP should large than start IP\n");
		goto End;
	}
	sprintf(poolCountValue, "%lu", lanEndIP.s_addr - lanStartIP.s_addr +1 );

	/*set option60 parameters together*/
	tcapi_set(DHCPD_OPT60, "start", secondPoolStartIP);
	tcapi_set(DHCPD_OPT60, "pool_count", poolCountValue);
	tcapi_set(DHCPD_OPT60, "router", secondPoolgateway);
	tcapi_set(DHCPD_OPT60, "subnetMask", secondPoolNetMask);

	tcapi_commit(DHCPD_OPT60);

End:
	//reset
	memset(secondPoolStartIP, 0, sizeof(secondPoolStartIP));
	memset(secondPoolEndIP, 0, sizeof(secondPoolEndIP));
	memset(secondPoolgateway, 0, sizeof(secondPoolgateway));
	memset(secondPoolNetMask, 0, sizeof(secondPoolNetMask));
	
	return 0;
}

int8 getdhcpConSer_EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char vEnable[4];	
	char tmpBuf[8];

	memset(vEnable, 0, sizeof(vEnable));
	memset(tmpBuf, 0, sizeof(tmpBuf));

	tcapi_get(DHCPD_OPT60, "Active", tmpBuf);
	if(strcmp(tmpBuf, "Yes") == 0)
		strcpy(vEnable, "1"); 
	else
		strcpy(vEnable, "0"); 

	return getTheValue(value, type, deviceNode, vEnable);	

}

int8 setdhcpConSer_EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	boolean* optionEnable = NULL;
	char tmpBuf[8];

	optionEnable = (boolean*)value;

	memset(tmpBuf, 0, sizeof(tmpBuf));

	if( *optionEnable == (boolean)0)
		sprintf(tmpBuf, "%s", "No");
	else if( *optionEnable == (boolean)1 ) 
		sprintf(tmpBuf, "%s", "Yes");
	else 
		return -1;
	tcapi_set(DHCPD_OPT60, "Active", tmpBuf);
	
	SetCfgNodeFlag( DHCPD_NODENAME, -1);

       return 0;
}

int8 getdhcpConSer_EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setdhcpConSer_EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getPoolOrderValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	uint32 nPoolOrder = 1;
	char  vPoolOrder[12] = {0};		
	
	sprintf(vPoolOrder,"%lu",nPoolOrder);
	return getTheValue(value, type, deviceNode, vPoolOrder);

}

int8 setPoolOrderValue(void* value, struct DeviceNode*  deviceNode)
{
        return 0;
}

int8 getPoolOrderAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aPoolOrder;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setPoolOrderAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aPoolOrder;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getSourceInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vPoolSrcIfc[4] = {0};	
	
	return getTheValue(value, type, deviceNode, vPoolSrcIfc);
}

int8 setSourceInterfaceValue(void* value, struct DeviceNode*  deviceNode)
{
        return 0;
}

int8 getSourceInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aSourceInterface;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setSourceInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aSourceInterface;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getVendorClassIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmpBuf[VENDORCLASSID_MAX];

	memset(tmpBuf, 0, sizeof(tmpBuf));
	tcapi_get(DHCPD_OPT60, "VendorID", tmpBuf);

	return getTheValue(value, type, deviceNode, tmpBuf);
}

int8 setVendorClassIDValue(void* value, struct DeviceNode*  deviceNode)
{
	char* tempValue = NULL;	
	int len = 0;

	tempValue = (char*)(value);
	len = strlen(tempValue);
	if(len >= VENDORCLASSID_MAX) { //TR098 indicate that the max length is 256, but in our code there only VENDORCLASSID_MAX
		tc_printf("\n==>VendorClassID length can't be larger than %d\n", VENDORCLASSID_MAX);
		return -1;
	}
	tcapi_set(DHCPD_OPT60, "VendorID", tempValue );

	SetCfgNodeFlag( DHCPD_NODENAME, -1);

       return 0;
}

int8 getVendorClassIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aVendorClassID;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setVendorClassIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aVendorClassID;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getVendorClassIDExcludeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	boolean nVendorClassIDExc = 0;
	char  vVendorClassIDExc[6];

	memset(vVendorClassIDExc, 0,  sizeof(vVendorClassIDExc));
	
	nVendorClassIDExc = 0;//always return false.
       sprintf(vVendorClassIDExc,"%u",nVendorClassIDExc);	
	return getTheValue(value, type, deviceNode, vVendorClassIDExc);
}

int8 setVendorClassIDExcludeValue(void* value, struct DeviceNode*  deviceNode)
{
	boolean nVendorClassIDExc = 0;
	uint8* temp = NULL;
	char tmpBuf[8];
	
	temp = (uint8*)value;
	nVendorClassIDExc = *temp;
	if(nVendorClassIDExc > 1)
		return -1;

	memset(tmpBuf, 0, sizeof(tmpBuf));

	sprintf(tmpBuf, "%d", nVendorClassIDExc);
	tcapi_set(DHCPD_OPT60, "vendorIDEx", tmpBuf );

	SetCfgNodeFlag( DHCPD_NODENAME, -1);
	return 0;
}

int8 getVendorClassIDExcludeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aVendorClassIDExclude;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setVendorClassIDExcludeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aVendorClassIDExclude;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getVendorClassIDModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	//uint8  nVendorClassIDMode = 0;
	char  vVendorClassIDMode[20];

	memset(vVendorClassIDMode, 0, sizeof(vVendorClassIDMode));
	sprintf(vVendorClassIDMode, "%s", "Substring");

#if 0
	nVendorClassIDMode = VENDORCLASSIDMODE_SUBSTRING;//need return "Substring"
	switch(nVendorClassIDMode){
		case VENDORCLASSIDMODE_EXACT:
			sprintf(vVendorClassIDMode, "%s", "Exact");
			break;
		case VENDORCLASSIDMODE_PREFIX:
			sprintf(vVendorClassIDMode, "%s", "Prefix");
			break;
		case VENDORCLASSIDMODE_SUFFIX:
			sprintf(vVendorClassIDMode, "%s", "Suffix");
			break;
		case VENDORCLASSIDMODE_SUBSTRING:
			sprintf(vVendorClassIDMode, "%s", "Substring");
			break;
		default:
			sprintf(vVendorClassIDMode, "%s", "Exact");
			break;
	}
#endif
	return getTheValue(value, type, deviceNode, vVendorClassIDMode);
}

int8 setVendorClassIDModeValue(void* value, struct DeviceNode*  deviceNode)
{	
	char* tempValue = NULL;	
	uint8  nVendorClassIDMode = 0;
	char tmpBuf[16];

	memset(tmpBuf, 0, sizeof(tmpBuf));

	tempValue = (char*)(value);
	if(strlen(tempValue) >= 10){//max length is 9(the length of "Substring").
		return -1;
	}

	if(strcmp(tempValue, "Exact") ==0
	|| strcmp(tempValue, "Prefix") ==0
	|| strcmp(tempValue, "Suffix") ==0
	|| strcmp(tempValue, "Substring") ==0 ){

		strcpy(tmpBuf, tempValue);
	}
	else 
		return -1;
#if 0
	if(strcmp(tempValue, "Exact") ==0){
		nVendorClassIDMode = VENDORCLASSIDMODE_EXACT;
	} 
	else if(strcmp(tempValue, "Prefix") ==0){
		nVendorClassIDMode = VENDORCLASSIDMODE_PREFIX;
	}
	else if(strcmp(tempValue, "Suffix") ==0){
		nVendorClassIDMode = VENDORCLASSIDMODE_SUFFIX;
	}  
	else if(strcmp(tempValue, "Substring") ==0){
		nVendorClassIDMode = VENDORCLASSIDMODE_SUBSTRING;
	} 
	else 
		return -1;
	sprintf(tmpBuf, "%d", nVendorClassIDMode);
#endif	

	tcapi_set(DHCPD_OPT60, "vendorIDMode", tmpBuf );

	SetCfgNodeFlag( DHCPD_NODENAME, -1);

	return 0;
}

int8 getVendorClassIDModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aVendorClassIDMode;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setVendorClassIDModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aVendorClassIDMode;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getClientIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vClientID[CLIENTID_MAX];
	
	memset(vClientID, 0, CLIENTID_MAX);
	tcapi_get(DHCPD_OPT60, "clientID", vClientID);
	
	return getTheValue(value, type, deviceNode, vClientID);
}

int8 setClientIDValue(void* value, struct DeviceNode*  deviceNode)
{	
	char* tempValue = NULL;	
	
	tempValue = (char*)value;
	if(strlen((char*)value) >= CLIENTID_MAX){
		tc_printf("\n==>ClientIDValue length can't be larger than %d\n", CLIENTID_MAX);
		return -1;
	}
	tcapi_set(DHCPD_OPT60, "clientID", tempValue);

	SetCfgNodeFlag( DHCPD_NODENAME, -1);

       return 0;
}

int8 getClientIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aClientID;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setClientIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aClientID;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getClientIDExcludeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	boolean nClientIDExc = 0;
	char  vClientIDExc[6];

	memset(vClientIDExc, 0, sizeof(vClientIDExc));
	nClientIDExc = 0;//always return false.
       sprintf(vClientIDExc,"%u",nClientIDExc);	
	return getTheValue(value, type, deviceNode, vClientIDExc);
}

int8 setClientIDExcludeValue(void* value, struct DeviceNode*  deviceNode)
{	
	boolean nClientIDExc = 0;
	uint8* temp = NULL;
	char tmpBuf[8];
	
	temp = (uint8*)value;
	nClientIDExc = *temp;
	if(nClientIDExc > 1)
		return -1;

	memset(tmpBuf, 0, sizeof(tmpBuf));
	
	sprintf(tmpBuf, "%d", nClientIDExc);
	tcapi_set(DHCPD_OPT60, "clientIDEx", tmpBuf );

	SetCfgNodeFlag( DHCPD_NODENAME, -1);
	
       return 0;
}

int8 getClientIDExcludeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aClientIDExclude;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setClientIDExcludeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aClientIDExclude;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getUserClassIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vUserClassID[USERCLASSID_MAX];
	
	memset(vUserClassID, 0, USERCLASSID_MAX);
	tcapi_get(DHCPD_OPT60, "userClassID", vUserClassID);
	
	return getTheValue(value, type, deviceNode, vUserClassID);
}

int8 setUserClassIDValue(void* value, struct DeviceNode*  deviceNode)
{
	char* tempValue = NULL;	
	
	tempValue = (char*)value;
	if(strlen((char*)value) >= USERCLASSID_MAX){
		tc_printf("\n==>UserClassIDValue length can't be larger than %d\n", USERCLASSID_MAX);
		return -1;
	}
	tcapi_set(DHCPD_OPT60, "userClassID", tempValue);

	SetCfgNodeFlag( DHCPD_NODENAME, -1);
	
       return 0;
}

int8 getUserClassIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aUserClassID;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setUserClassIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aUserClassID;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getUserClassIDExcludeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	boolean  nUserClassIDExc = 0;
	char  vUserClassIDExc[6];
	
	memset(vUserClassIDExc, 0, sizeof(vUserClassIDExc));
	nUserClassIDExc = 0;//always return false.
       sprintf(vUserClassIDExc,"%u",nUserClassIDExc);	
	return getTheValue(value, type, deviceNode, vUserClassIDExc);
}

int8 setUserClassIDExcludeValue(void* value, struct DeviceNode*  deviceNode)
{
	boolean nUserClassIDExc = 0;
	uint8* temp = NULL;
	char tmpBuf[8];
	
	temp = (uint8*)value;
	nUserClassIDExc = *temp;
	if(nUserClassIDExc > 1){
		tc_printf("\n==>UserClassIDExcludeValue length can't be larger than 1\n");
		return -1;
	}

	memset(tmpBuf, 0, sizeof(tmpBuf));	
	sprintf(tmpBuf, "%d", nUserClassIDExc);
	tcapi_set(DHCPD_OPT60, "userClassIDEx", tmpBuf );

	SetCfgNodeFlag( DHCPD_NODENAME, -1);
	
       return 0;
}

int8 getUserClassIDExcludeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aUserClassIDExclude;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setUserClassIDExcludeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aUserClassIDExclude;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getChaddrValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char vChaddr[20];

	memset(vChaddr, 0, sizeof(vChaddr));

	tcapi_get(DHCPD_OPT60, "chaddr", vChaddr);
	if(isValidMacAddr(vChaddr) != 0)
		vChaddr[0] = '\0';//return empty string
	
	return getTheValue(value, type, deviceNode, vChaddr);
}

int8 setChaddrValue(void* value, struct DeviceNode*  deviceNode)
{
	char* temp = NULL;
	char vChaddr[20];

	memset(vChaddr, 0, sizeof(vChaddr));

	temp = (char*)value;
	if(strlen(temp) != 0){
		if(isValidMacAddr(temp) != 0){//valid format is xx:xx:xx:xx:xx:xx
			tc_printf("\n===>Invalid MAC format\n");
			return -1;
		}
		memcpy(vChaddr, temp, 17);
	}
	
	tcapi_set(DHCPD_OPT60, "chaddr", vChaddr);

	SetCfgNodeFlag( DHCPD_NODENAME, -1);

	return 0;
}

int8 getChaddrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aChaddr;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setChaddrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aChaddr;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getChaddrMaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vChaddrMask[20];
	
	memset(vChaddrMask, 0, sizeof(vChaddrMask));
	tcapi_get(DHCPD_OPT60, "chaddrMask", vChaddrMask);	
	
	if((isValidMacAddrMask(vChaddrMask) != 0) || (strcmp(vChaddrMask, "00:00:00:00:00:00") == 0)){
		vChaddrMask[0] = '\0';//return empty string;
	}
	
	return getTheValue(value, type, deviceNode, vChaddrMask);
}

int8 setChaddrMaskValue(void* value, struct DeviceNode*  deviceNode)
{
	char  vChaddrMask[20];
	char* temp = NULL;

	memset(vChaddrMask, 0, sizeof(vChaddrMask));

	temp = (char*)value;
	if(strlen(temp) != 0){
		if(isValidMacAddrMask(temp) != 0) {//valid format is xx:xx:xx:xx:xx:xx
			tc_printf("\n==>Invalid MAC format\n");
			return -1;
		}
		memcpy(vChaddrMask, temp, 17);
	}

	tcapi_set(DHCPD_OPT60, "chaddrMask", vChaddrMask);	

	SetCfgNodeFlag( DHCPD_NODENAME, -1);

	return 0;
}

int8 getChaddrMaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aChaddrMask;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setChaddrMaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aChaddrMask;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getChaddrExcludeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	boolean  nChaddrExc = 0;
	char  vChaddrExc[4];

	memset(vChaddrExc, 0, sizeof(vChaddrExc));
	nChaddrExc = 0;//always return false.
       sprintf(vChaddrExc,"%u",nChaddrExc);	
	return getTheValue(value, type, deviceNode, vChaddrExc);
}

int8 setChaddrExcludeValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8* temp = NULL;
	boolean nChaddrExc = 0;
	char tmpValue[8];
	
	temp = (uint8*)value;
	nChaddrExc = *temp;
	if(nChaddrExc > 1){
		tc_printf("\n==>ChaddrExcludeValue length can't be larger than 1\n");
		return -1;
	}

	memset(tmpValue, 0, sizeof(tmpValue));

	sprintf(tmpValue, "%d", nChaddrExc);
	tcapi_set(DHCPD_OPT60, "chaddrEx", tmpValue);

	SetCfgNodeFlag( DHCPD_NODENAME, -1);
	
       return 0;
}

int8 getChaddrExcludeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aChaddrExclude;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setChaddrExcludeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aChaddrExclude;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getLocallyServedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	boolean  nLocallyServed = 0;
	char  vLocallyServed[4];
	
	memset(vLocallyServed, 0, sizeof(vLocallyServed));
	nLocallyServed = 1;//always return true.
       sprintf(vLocallyServed,"%u",nLocallyServed);	
	return getTheValue(value, type, deviceNode, vLocallyServed);
}

int8 setLocallyServedValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8* temp = NULL;
	boolean nLocallyServed = 0;
	char tmpValue[8];
	
	temp = (uint8*)value;
	nLocallyServed = *temp;
	if(nLocallyServed > 1){
		return -1;
	}

	memset(tmpValue, 0, sizeof(tmpValue));

	sprintf(tmpValue, "%d", nLocallyServed);
	tcapi_set(DHCPD_OPT60, "locallyServed", tmpValue);

	SetCfgNodeFlag( DHCPD_NODENAME, -1);
       return 0;
}

int8 getLocallyServedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aLocallyServed;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLocallyServedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aLocallyServed;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getMinAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20];	

	memset(tempValue, 0, sizeof(tempValue));

	tcapi_get(DHCPD_OPT60, "start", tempValue);
	
	if(strlen(tempValue) > 15 
	|| InternalClientIpCheck(tempValue) == 0) {
		memset(tempValue, 0, sizeof(tempValue));
		strcpy(tempValue, "0.0.0.0");
	}
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setMinAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char tmpValue[20];
	
	memset(tmpValue, 0, sizeof(tmpValue));
	sprintf(tmpValue,"%s", (char*)(value));	
	/*MinAddress is Internal Client IP */	
	if( InternalClientIpCheck(tmpValue) != 1 ) {
		tc_printf("\n==>Invalid min IPAddress Value\n");
		return -1;
	}
	/*MinAddress must be local network ip address*/		
	if(BeLANIP(tmpValue) < 0){
		tc_printf("\n==>Min IPAddress must be local network ip address\n");
		return -1;
	}

	strcpy(secondPoolStartIP, tmpValue);

	SetCfgNodeFlag( DHCPD_OPT60_NODENAME, -1);

	return 1;	
}

int8 getMinAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aMinAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setMinAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aMinAddress;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getMaxAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};	
	char startIP[16] = {0};
	char poolCount[8] = {0};
	struct in_addr maxAddress;

	tcapi_get(DHCPD_OPT60, "start", startIP);
	tcapi_get(DHCPD_OPT60, "pool_count", poolCount);
	
	if(strlen(startIP) == 0 || strlen(poolCount) == 0)
		strcpy(tempValue, "0.0.0.0");
	else {
		inet_aton(startIP, &maxAddress);
		maxAddress.s_addr += atoi(poolCount) - 1;
		strcpy(tempValue, inet_ntoa(maxAddress));
	}
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setMaxAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char tempValue[20];	
	
	memset(tempValue, 0, sizeof(tempValue));
	sprintf(tempValue,"%s", (char*)(value));
	/*MaxAddress is Internal Client IP */	
	if( InternalClientIpCheck(tempValue) != 1 ) {
		tc_printf("\n==>Invalid max IPAddress Value\n");
		return -1;
	}
	/*MaxAddress must be local network ip address*/		
	if(BeLANIP(tempValue) < 0){
		tc_printf("\nMaxAddress must be local network ip address\n");
		return -1;
	}

	strcpy(secondPoolEndIP, tempValue);

	SetCfgNodeFlag( DHCPD_OPT60_NODENAME, -1);

       return 0;
}

int8 getMaxAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aMaxAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setMaxAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aMaxAddress;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getReservedAddressesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vReservedAddr[4] = {0};	
	
	return getTheValue(value, type, deviceNode, vReservedAddr);
}

int8 setReservedAddressesValue(void* value, struct DeviceNode*  deviceNode)
{
        return 0;
}

int8 getReservedAddressesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aReservedAddresses;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setReservedAddressesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aReservedAddresses;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getdhcpConSer_MaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20];	

	memset(tempValue, 0, sizeof(tempValue));

	tcapi_get(DHCPD_OPT60, "subnetMask", tempValue);

	if(strlen(tempValue) == 0) 
		strcpy(tempValue, "0.0.0.0");
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setdhcpConSer_MaskValue(void* value, struct DeviceNode*  deviceNode)
{	
	char* vSubnetMask1 = NULL;
	struct in_addr subnetMask;

	vSubnetMask1=(char*)(value);
	if(strcmp(vSubnetMask1, "255.255.255.255") == 0){
		return -1;
	}
	
	if (SubnetLegalCheck(vSubnetMask1) == 0) {
		tc_printf("\n==>Invalid netMask Value\n");
		return -1;
	}

	strcpy(secondPoolNetMask, vSubnetMask1);	

	SetCfgNodeFlag( DHCPD_OPT60_NODENAME, -1);

	return 1;

}

int8 getdhcpConSer_MaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aSubnetMask;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setdhcpConSer_MaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aSubnetMask;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getdhcpConSer_DNSValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* tempValue;
	char  DNSServers[128] = {0};
	char  DNSServers1[64] = {0};
	char  DNSServers2[64] = {0};

	tcapi_get(DHCPD_OPT60, "dns1", DNSServers1);
	tcapi_get(DHCPD_OPT60, "dns2", DNSServers2);

	if(strlen(DNSServers1) == 0)
		strcpy(DNSServers1, "0.0.0.0");
	if(strlen(DNSServers2) == 0)
		strcpy(DNSServers2, "0.0.0.0");
	
	strcpy(DNSServers, DNSServers1);
	strcat(DNSServers, ",");
	strcat(DNSServers, DNSServers2);
	
	return getTheValue(value, type, deviceNode, DNSServers);
}

int8 setdhcpConSer_DNSValue(void* value, struct DeviceNode*  deviceNode)
{
	char *DNSServer = NULL;
	char  DNSServers1[64] = {0};
	char  DNSServers2[64] = {0};
	char *temp = NULL;
	
	DNSServer = (char*)value;
	temp = strstr(DNSServer, ",");
	if(temp == NULL){ //Only one DNS Server to set, so set it to the  spSysParam.dhcpServer.dnsServer[0].
		if( InternalClientIpCheck(DNSServer) != 1 ){
			tc_printf("\n==>Invalid DNS Value\n");
			return -1;
		}else{
			strcpy(DNSServers1, DNSServer);
			tcapi_set(DHCPD_OPT60, "dns1", DNSServers1);
		}		
	}else{//two DNS Server
		*temp = '\0';
		if( InternalClientIpCheck(DNSServer) != 1 ){
			tc_printf("\n==>Invalid DNS Value\n");
			return -1;
		}			
		if( InternalClientIpCheck(temp+1) != 1 ){
			tc_printf("\n==>Invalid DNS Value\n");
			return -1;
		}
		strcpy(DNSServers1, DNSServer);
		strcpy(DNSServers2, temp+1);
		tcapi_set(DHCPD_OPT60, "dns1", DNSServers1);
		tcapi_set(DHCPD_OPT60, "dns2", DNSServers2);
	}
	
	SetCfgNodeFlag( DHCPD_NODENAME, -1);

	return 0;
}

int8 getdhcpConSer_DNSAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aDNSServers;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setdhcpConSer_DNSAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aDNSServers;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getDomainNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char vDomainName[DOMAINNAME_MAX];
	
	memset(vDomainName, 0, sizeof(vDomainName));
	tcapi_get(DHCPD_OPT60, "domainName", vDomainName);
	return getTheValue(value, type, deviceNode, vDomainName);
}

int8 setDomainNameValue(void* value, struct DeviceNode*  deviceNode)
{
	char* temp = NULL;
	
	temp = (char*)value;
	if(strlen(temp) >= DOMAINNAME_MAX){
		return -1;
	}
	tcapi_set(DHCPD_OPT60, "domainName", temp);

	SetCfgNodeFlag( DHCPD_NODENAME, -1);

	return 0;
}

int8 getDomainNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aDomainName;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDomainNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aDomainName;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getIPRoutersValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vIPRouters[20];	

	memset(vIPRouters, 0, sizeof(vIPRouters));
	tcapi_get(DHCPD_OPT60, "router", vIPRouters);

	if(strlen(vIPRouters) == 0)
		strcpy(vIPRouters, "0.0.0.0");
	
	return getTheValue(value, type, deviceNode, vIPRouters);
}

int8 setIPRoutersValue(void* value, struct DeviceNode*  deviceNode)
{
	char vIPRouters[20] = {0};
	char vLanIP[16] = {0};

	sprintf(vIPRouters,"%s", (char*)(value));	
	if( InternalClientIpCheck(vIPRouters) != 1 ) {
		tc_printf("\n==>Invalid IPRouter Value\n");
		return -1;
	}

	getLanIpAddr(vLanIP);

	if(strcmp(vLanIP, vIPRouters) != 0) {//IPRoute must equal route lan IP
		tc_printf("\n==>IPRoute must equal route lan IP\n");
		return -1;

	}
	strcpy(secondPoolgateway, vIPRouters);

	SetCfgNodeFlag( DHCPD_OPT60_NODENAME, -1);

	return 1;

}

int8 getIPRoutersAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aIPRouters;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPRoutersAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aIPRouters;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getDHCPLeaseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vDHCPLeaseTime1[20] = {0};
	uint32 dhcpLeaseTime = 0;
	uint32 vInfiniteTime = 0xffffffff;
	char infiniteTime[20] = {0};
	
	tcapi_get(DHCPD_OPT60, "lease", vDHCPLeaseTime1);
	
	sprintf(infiniteTime, "%u", vInfiniteTime);
	if(!strcmp(infiniteTime, vDHCPLeaseTime1)){		
		sprintf(vDHCPLeaseTime1,"%d",-1);//In TR098 spec, a value of -1 indicates an infinite lease
	}

	return getTheValue(value, type, deviceNode, vDHCPLeaseTime1);
}

int8 setDHCPLeaseTimeValue(void* value, struct DeviceNode*  deviceNode)
{
 	int* vDHCPLeaseTime1 = NULL;		
	char DHCPLeaseTime[20] = {0};
	uint32 infiniteTime = 0xffffffff;
	
	vDHCPLeaseTime1 = (int*)(value);
	if( *vDHCPLeaseTime1 >= 0 ) {
		sprintf(DHCPLeaseTime, "%d", *vDHCPLeaseTime1);
	}
	else if( *vDHCPLeaseTime1 == -1 ){//In TR098 spec, a value of -1 indicates an infinite lease
		sprintf(DHCPLeaseTime, "%lu", infiniteTime);
	}
	else{
		tc_printf("\n==>Invalid DHCP Lease Time Value\n");
		return -1;
	}
	tcapi_set(DHCPD_OPT60, "lease", DHCPLeaseTime);	
	tcapi_set("Dhcpd_Common", "lease", DHCPLeaseTime);
	
	SetCfgNodeFlag( DHCPD_NODENAME, -1);

	return 0;
}

int8 getDHCPLeaseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aDHCPLeaseTime;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDHCPLeaseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aDHCPLeaseTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getUseAllocatedWANValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	uint8 nUseAllocatedWAN = 0;
	char  vUseAllocatedWAN[20] = {0};

	sprintf(vUseAllocatedWAN, "%s", "Normal");
#if 0
	nUseAllocatedWAN = USEALLOCATEDWAN_NORMAL;//always return normal
	switch(nUseAllocatedWAN){
		case USEALLOCATEDWAN_NORMAL:
			sprintf(vUseAllocatedWAN, "%s", "Normal");
			break;
		case USEALLOCATEDWAN_PASSTHROUGH:
			sprintf(vUseAllocatedWAN, "%s", "Passthrough");
			break;
		default:
			sprintf(vUseAllocatedWAN, "%s", "Normal");
			break;
	}
#endif
	return getTheValue(value, type, deviceNode, vUseAllocatedWAN);
}

int8 setUseAllocatedWANValue(void* value, struct DeviceNode*  deviceNode)
{
	char* tempValue = NULL;	
	char  vUseAllocatedWAN[20] = {0};

	tempValue = (char*)(value);
	if(strlen(tempValue) >= 12){//max length is 11(the length of "Passthrough").
		tc_printf("\n==>UseAllocatedWANValue length should not larger than 12\n");
		return -1;
	}

	if(strcmp(tempValue, "Normal") ==0
	|| strcmp(tempValue, "Passthrough") ==0) {
		strcpy(vUseAllocatedWAN, tempValue);
	}
	else {
		tc_printf("\nInvalid UseAllocatedWANValue\n");
		return -1;
	}

	tcapi_set(DHCPD_OPT60, "useAllocatedWAN", tempValue);

	SetCfgNodeFlag( DHCPD_NODENAME, -1);

	return 0;
}

int8 getUseAllocatedWANAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aUseAllocatedWAN;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setUseAllocatedWANAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aUseAllocatedWAN;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getAssociatedConnectionValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vAssociatedConn[4] = {0};	

	return getTheValue(value, type, deviceNode, vAssociatedConn);
}

int8 setAssociatedConnectionValue(void* value, struct DeviceNode*  deviceNode)
{
        return 0;
}

int8 getAssociatedConnectionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aAssociatedConnection;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setAssociatedConnectionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aAssociatedConnection;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getDHCPServerIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vServerIPAddr[20] = {0};	

	tcapi_get(DHCPD_OPT60, "serverIP", vServerIPAddr);
	if(strlen(vServerIPAddr) == 0)
		strcpy(vServerIPAddr, "0.0.0.0");
	
	return getTheValue(value, type, deviceNode, vServerIPAddr);
}

int8 setDHCPServerIPAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char* tempValue = NULL;	
	char  vServerIPAddr[20] = {0};	

	tempValue = (char*)(value);
	if(strlen(tempValue) > 15){//IP is xxx.xxx.xxx.xxx, the max length is 15
		return -1;
	}
	if( InternalClientIpCheck(tempValue) != 1){
		tc_printf("\n==>Invalid DHCP Server IP\n");
		return -1;
	}else{
		strcpy(vServerIPAddr, tempValue);
	}
	tcapi_set(DHCPD_OPT60, "serverIP", vServerIPAddr);

	SetCfgNodeFlag( DHCPD_NODENAME, -1);

	return 0;
}

int8 getDHCPServerIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aDHCPServerIPAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDHCPServerIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aDHCPServerIPAddress;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getDHCPStaticAddressNumValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	uint32  nStaticAddrNumOfEntry = 0;
	char  vStaticAddrNumOfEntry[12] = {0};
	
	sprintf(vStaticAddrNumOfEntry,"%lu",nStaticAddrNumOfEntry);	
	return getTheValue(value, type, deviceNode, vStaticAddrNumOfEntry);
}

int8 getDHCPStaticAddressNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aDHCPStaticAddressNum;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDHCPStaticAddressNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aDHCPStaticAddressNum;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getdhcpConSer_OptionNumValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	uint32  nOptionNumOfEntry = 1;
	char  vOptionNumOfEntry[12] = {0};
	
	sprintf(vOptionNumOfEntry,"%lu",nOptionNumOfEntry);	
	return getTheValue(value, type, deviceNode, vOptionNumOfEntry);
}

int8 getdhcpConSer_OptionNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aOptionNum;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setdhcpConSer_OptionNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aOptionNum;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}
int	cwmpInitDHCPConSerOptionSubTree( DeviceNodePtr deviceNode )
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
	
	rtnCode = RegisterDynNodeFromIndex( deviceNode, DHCPConSerOptionIndex, DHCPConSerOptionNum);

	return rtnCode;
}

int8 getconSerOption_EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};	
	char tmp[20] = {0};

	tcapi_get(DHCPD_OPT240, "Active", tmp);
	if(0 == strcmp(tmp, "Yes"))
		sprintf(result, "%d", 1);
	else if(0 == strcmp(tmp, "No"))
		sprintf(result, "%d", 0);

	return getTheValue(value, type, deviceNode, result);
}

int8 setconSerOption_EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[20] = {0};
	uint8 iEnable;
	
	iEnable = *((uint8*)value);
	if(1 == iEnable){
		sprintf(tmp, "%s", "Yes");
	} 
	else if (0 == iEnable){
		sprintf(tmp, "%s", "No");
	}
	else{
		tc_printf("==>setdhcpOption_EnableValue:value should be 0 or 1.\n");
		return -1;
	}	
	
	tcapi_set(DHCPD_OPT240, "Active", tmp);
	
	SetCfgNodeFlag(DHCPD_NODENAME, -1);
       return 0;
}

int8 getconSerOption_EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aconSerOption_Enable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setconSerOption_EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aconSerOption_Enable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getconSerOption_TagValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[4] = {0};	
	strcpy(result, "240"); 

	return getTheValue(value, type, deviceNode, result);
}

int8 setconSerOption_TagValue(void* value, struct DeviceNode*  deviceNode)
{
        return 0;
}

int8 getconSerOption_TagAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aconSerOption_Tag;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setconSerOption_TagAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aconSerOption_Tag;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int8 getconSerOption_ValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  result[VENDORCLASSID_MAXBASE64] = {0};	
	char originValue[40] = {0};
	int len = 0;

	tcapi_get(DHCPD_OPT240, "Value", originValue);	
	base64Encode(originValue, result, strlen(originValue), &len);	

	return getTheValue(value, type, deviceNode, result);
}

int8 setconSerOption_ValueValue(void* value, struct DeviceNode*  deviceNode)
{
	char * tempValue = NULL;
	char venderID[VENDORCLASSID_MAXBASE64] = {0};

	tempValue = (char*)(value);
	if(strlen(tempValue) >= VENDORCLASSID_MAXBASE64)
		return -1;
	strcpy(venderID, tempValue);
	decodeBase64(venderID);
	
	tcapi_set(DHCPD_OPT240, "Value", venderID); 
	SetCfgNodeFlag(DHCPD_NODENAME, -1);
       return 0;
}

int8 getconSerOption_ValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdhcpConSerAttr.aconSerOption_Value;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setconSerOption_ValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdhcpConSerAttr.aconSerOption_Value;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DHCPCONSER_ATTR_FLAG;
	return result;
}

int isHexNum(char c)
{
	if ( (c >= '0') && (c <= '9') )
		return (int) (c - '0');

	if ( (c >= 'a') && (c <= 'f') )
		return (int) ((c -'a') + 10);

	if ( (c >= 'A') && (c <= 'F') )
		return (int) ((c - 'A') + 10);

	return -1;
}

int isValidMacAddr(char* mac)
{
	uint32 macLen = 0;
	int i = 0;
	char ch0, ch1, ch2;

	if (mac == NULL)
		return -1;
	
	macLen = strlen(mac);
	
	if(macLen != 17)
		return -1;
	
	if(strcmp(mac, "00:00:00:00:00:00") == 0)
		return -1;
	
	ch0 = mac[0];
	ch1 = mac[1];
	if ( (isHexNum(ch0) < 0) || (isHexNum(ch1) < 0) )
		return -1;

	i = 2;
	while ( (i+2) < macLen ) {	
		ch0 = mac[i];
		ch1 = mac[i+1];
		ch2 = mac[i+2];

		if ((ch0 != ':') || (isHexNum(ch1) < 0) || (isHexNum(ch2) < 0))
			return -1;

		i = i + 3;
	}
#if 0
	if(macLen != 12 && macLen != 17){
		return -1;
	}
	if ( macLen == 12 ){
		if(strcmp(mac, "000000000000") == 0){
			return -1;
		}
		for ( i = 0; i < macLen; i++ )
		{
			if(isHexNum(mac[i]) < 0){
				return -1;
			}
			
		}
	}else if ( macLen == 17){
		if(strcmp(mac, "00:00:00:00:00:00") == 0){
			return -1;
		}
		ch0 = mac[0];
		ch1 = mac[1];
		if ( (isHexNum(ch0) < 0) || (isHexNum(ch1) < 0) )
		{
			return -1;
		}

		i = 2;
		while ( (i+2) < macLen )
		{	
			ch0 = mac[i];
			ch1 = mac[i+1];
			ch2 = mac[i+2];

			if ((ch0 != ':') || (isHexNum(ch1) < 0) || (isHexNum(ch2) < 0))
			{
				return -1;
			}
			i = i + 3;
		}
	}else{
		return -1;
	}
#endif
	return 0;
}

int isValidMacAddrMask(char* mask)
{
	uint32 maskLen = 0;
	int i, j;
	int hexNum = -1;
	char maskTemp[13] = "";
	if (mask == NULL){
		return -1;
	}

	maskLen = strlen(mask);

	//if(maskLen != 12 && maskLen != 17){
	//	return -1;
	//}
	
	if(maskLen != 17)
		return -1;

	memset(maskTemp, 0, 13);
	//change format xx:xx:xx:xx:xx:xx to xxxxxxxxxxxx
	if((maskLen == 17) && (mask[2] == ':') && (mask[5] == ':') 
		&& (mask[8] == ':') && (mask[11] == ':') && (mask[14] == ':')){
		sprintf(maskTemp, "%c%c%c%c%c%c%c%c%c%c%c%c", 
			mask[0], mask[1], mask[3], mask[4], mask[6], mask[7], 
			mask[9], mask[10], mask[12], mask[13], mask[15], mask[16]);
	}
	//else if(maskLen == 12){
		//do not need format change
	//	strcpy(maskTemp, mask);
	//}
	else{//the mac mask format is not right
		return -1;
	}
	//check mask
	for(i = 0; i<12; i++){
		hexNum = isHexNum(maskTemp[i]);
		if( hexNum < 0){//must hex number
			return -1;
		}else{
			if(hexNum == 15){
					// the bit of mac mask is F,so check the next bit
			}else if((hexNum == 14) || (hexNum == 12) ||(hexNum == 8) || (hexNum == 0)){//the bit of mac mask is not 'F', it must be 'E','C', '8'
				if(i == 11){//the last bit
					return 0;
				}else{
					for(j = i+1; j< 12; j++){
						if(maskTemp[j] != '0'){//mask must that foamat, eg. FE0000000000
							return -1;
						}
					}
					return 0;
				}
			}else{//valid mask bit
				return -1;
			}
		}
	}
	return 0;
}

int getLanNetMask(char * lanMask)
{
    int ret = 0;
    ret = tcapi_get("Lan_Entry0", "netmask", lanMask);
    if(ret != 0)
		tc_printf("getLanIpNetMask Failed!\n");
    return ret;
}

int BeLANIP(char* RouteGateway)
{
	char IPBuf[16] = {0}, netMaskBuf[16] = {0};
	struct in_addr LANIP;
	struct in_addr LANSubnet;
	struct in_addr GatewayIP;	
	
    	getLanIpAddr(IPBuf);
	getLanNetMask(netMaskBuf);
	
	inet_aton(IPBuf, &LANIP);
	inet_aton(netMaskBuf, &LANSubnet);
	inet_aton(RouteGateway, &GatewayIP);

	/*check subnet*/
	if((GatewayIP.s_addr & LANSubnet.s_addr) != (LANIP.s_addr & LANSubnet.s_addr))
		return -1;

	return 0;
}

void 
base64Encode(uint8 *src, uint8 *dest, int srcLen, int *destLen)
{
	int i, j, mode;
	uint8 *destHead = dest;
	int val;
	
	if ( srcLen <= 0 ) return;
	mode = srcLen % 3;
	srcLen -= mode;

	for(i = 0; i< srcLen ; i += 3) {
		val = (uint8 ) (*src++) & 0xff;
		val <<= 8;
		val |= ((uint8 ) (*src++) & 0xff);
		val <<= 8;
		val |= ((uint8 ) (*src++) & 0xff);

		*(dest +3) = base64EncodeChrSet[val &0x3f];
		val >>= 6;
		*(dest +2) = base64EncodeChrSet[val &0x3f];
		val >>= 6;
		*(dest +1) = base64EncodeChrSet[val &0x3f];
		val >>= 6;
		*(dest +0) = base64EncodeChrSet[val &0x3f];
		dest += 4;
	}

	if(mode == 1) { 
		val = (uint8 ) (*src++) & 0xff;
		val <<= 16;
		*(dest +3) = base64EncodeChrSet[64];
		val >>= 6;
		*(dest +2) = base64EncodeChrSet[64];
		val >>= 6;
		*(dest +1) = base64EncodeChrSet[val &0x3f];
		val >>= 6;
		*(dest +0) = base64EncodeChrSet[val &0x3f];
		dest += 4;
	}
	else if( mode == 2) {
		val = (uint8 ) (*src++) & 0xff;
		val <<= 8;
		val |= ((uint8 ) (*src++) & 0xff);
		val <<= 8;
		*(dest +3) = base64EncodeChrSet[64];
		val >>= 6;
		*(dest +2) = base64EncodeChrSet[val &0x3f];
		val >>= 6;
		*(dest +1) = base64EncodeChrSet[val &0x3f];
		val >>= 6;
		*(dest +0) = base64EncodeChrSet[val &0x3f];
		dest += 4;
	}
	*dest= '\0';
	*destLen = (int)(dest - destHead);
	return;
} /* base64Encode */

void decodeBase64(char *Data)
{
	const unsigned char *in = Data;
	// The decoded size will be at most 3/4 the size of the encoded
	unsigned long ch = 0;
	int i = 0;
	while (*in) {        
		int t = *in++;
		if(t >= '0' && t <= '9')
			t = t - '0' + 52;
		else if(t >= 'A' && t <= 'Z')
			t = t - 'A';
		else if(t >= 'a' && t <= 'z')
			t = t - 'a' + 26;
		else if(t == '+')
			t = 62;
		else if(t == '/')
			t = 63;
		else if(t == '=')
			t = 0;
		else
			continue;
		ch = (ch << 6) | t;
		i++;
		if (i == 4) {
			*Data++ = (char) (ch >> 16);
			*Data++ = (char) (ch >> 8);
			*Data++ = (char) ch;
			i = 0;
		}
	}
	*Data = 0;
}

