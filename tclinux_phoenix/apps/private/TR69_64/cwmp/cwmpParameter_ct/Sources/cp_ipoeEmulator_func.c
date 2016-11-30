#include "Global_res.h"
#include "cp_ipoeEmulator.h"

int8 getIPoeEmulatorDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char diagStatus[33] = {0};

	memset(diagStatus, 0, sizeof(diagStatus));
	if ( 0 != tcapi_get(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_DIAGNOSTICS_STATE_STR, diagStatus) )
		strcpy(diagStatus, "None");

	return getTheValue(value, type, deviceNode, diagStatus);
}
int8 setIPoeEmulatorDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode)
{
	char diagStatus[33] = {0};
	
	memset(diagStatus, 0, sizeof(diagStatus));
	if( strlen((char*)value) > (sizeof(diagStatus) - 1) )
		return -1;
	strcpy(diagStatus, (char*)value);
	
	if ( 0 != strcmp(diagStatus, "Start") && 0 != strcmp(diagStatus, "Stop") )
		return -1;

	tcapi_set(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_DIAGNOSTICS_STATE_STR, diagStatus);

	SetCfgNodeFlag(IPOEEMULATOR_ENTRY_STR, -1);
	return 0;
}
int8 getIPoeEmulatorDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIpoeEmulatorAttr.aDiagnosticsState;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPoeEmulatorDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIpoeEmulatorAttr.aDiagnosticsState;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= IPOE_EMULATOR_ATTR_FLAG;
	return result;
}

static int8 checkIPoEWanInterface(char* setvalue)
{
	char DefaultIface_WAN[] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";
	char tempvalue[2] = {0};
	int len = 0;
	int pvcnode = 0, itmpV = 0;
	DeviceNodePtr Node = NULL;
	char *tmp = NULL, *entryPtr = NULL;
	char tmpstr[32] = {0};
	char nodeName[32] = {0};
	int isBrdige = 0;

	len = strlen(DefaultIface_WAN);

	if ( 0 == strncmp(DefaultIface_WAN, setvalue, len) ) //WAN interface, get the gateway.
	{
		/*check the value is valid or not.*/
		cwmpSearchDeviceNodeByName(setvalue, rpc.IGDDeviceRootPtr, &Node);
		if ( NULL == Node )
			return -1;

		if ( setvalue[len] < '1' || setvalue[len] > '8' )
			return -1;

		tempvalue[0] = setvalue[len];
		tempvalue[1] = '\0';
		pvcnode = atoi(tempvalue) - 1;

		if ( NULL != (entryPtr = strstr(setvalue, "WANPPPConnection")) )
		{
			entryPtr += sizeof("WANPPPConnection");
			itmpV = atoi(entryPtr);
			if ( itmpV >= 1 && itmpV <= MAX_SMUX_NUM )
			{
				sprintf(nodeName, "Wan_PVC%d_Entry%d", pvcnode, itmpV - 1);
				if ( 0 == tcapi_get(nodeName, "WanMode", tmpstr)
					&& 0 == strcmp(tmpstr, "Bridge"))
					isBrdige = 1;
			}
		}
		
		if ( NULL != (tmp = strstr(setvalue, "WANIPConnection")) || isBrdige )
		{
			sprintf(nodeName, "Wan_PVC%d", pvcnode);
			if ( 0 != tcapi_get(nodeName, "VLANMode", tmpstr) )
				strcpy(tmpstr, "UNTAG");
			tcapi_set(IPOEEMULATOR_ENTRY_STR, "VLANMode", tmpstr);

			if ( 0 != tcapi_get(nodeName, "VLANID", tmpstr) )
				strcpy(tmpstr, "4096");
			tcapi_set(IPOEEMULATOR_ENTRY_STR, "VLANID", tmpstr);

			if ( 0 != tcapi_get(nodeName, "DOT1P", tmpstr) )
				strcpy(tmpstr, "0");
			tcapi_set(IPOEEMULATOR_ENTRY_STR, "DOT1P", tmpstr);
		}
		else
		{
			return -1;
		}
	}
	else
	{//error value
		return -1;
	}

	return 0;
}
int8 getIPoeEmulatorWANInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char wanInterface[257] = {0};

	memset(wanInterface, 0, sizeof(wanInterface));
	if ( 0 != tcapi_get(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_WANINTERFACE_STR, wanInterface) )
		memset(wanInterface, 0, sizeof(wanInterface));

	return getTheValue(value, type, deviceNode, wanInterface);
}
int8 setIPoeEmulatorWANInterfaceValue(void* value, struct DeviceNode*  deviceNode)
{
	char wanInterface[257] = {0};
	
	memset(wanInterface, 0, sizeof(wanInterface));
	if( strlen((char*)value) > (sizeof(wanInterface) - 1) )
		return -1;
	strcpy(wanInterface, (char*)value);
	if ( 0 != checkIPoEWanInterface(wanInterface) )
		return -1;

	tcapi_set(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_WANINTERFACE_STR, wanInterface);
	SetCfgNodeFlag(IPOEEMULATOR_ENTRY_STR, -1);
	return 0;
}
int8 getIPoeEmulatorWANInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIpoeEmulatorAttr.aWANInterface;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPoeEmulatorWANInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIpoeEmulatorAttr.aWANInterface;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= IPOE_EMULATOR_ATTR_FLAG;
	return result;
}

extern int checkMACFormat(char *mac);
int8 getIPoeEmulatorUserMacValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char userMAC[33] = {0};

	memset(userMAC, 0, sizeof(userMAC));
	if ( 0 != tcapi_get(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_USERMAC_STR, userMAC) )
		memset(userMAC, 0, sizeof(userMAC));

	return getTheValue(value, type, deviceNode, userMAC);
}
int8 setIPoeEmulatorUserMacValue(void* value, struct DeviceNode*  deviceNode)
{
	char userMAC[33] = {0};
	
	memset(userMAC, 0, sizeof(userMAC));
	if( strlen((char*)value) > (sizeof(userMAC) - 1) )
		return -1;
	strcpy(userMAC, (char*)value);
	if ( 0 != checkMACFormat(userMAC) )
		return -1;

	tcapi_set(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_USERMAC_STR, userMAC);
	SetCfgNodeFlag(IPOEEMULATOR_ENTRY_STR, -1);
	return 0;
}
int8 getIPoeEmulatorUserMacAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIpoeEmulatorAttr.aUserMac;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPoeEmulatorUserMacAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIpoeEmulatorAttr.aUserMac;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= IPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getIPoeEmulatorVendorClassIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char vendorcls[65] = {0};

	memset(vendorcls, 0, sizeof(vendorcls));
	if ( 0 != tcapi_get(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_VENDORCLASS_STR, vendorcls) )
		memset(vendorcls, 0, sizeof(vendorcls));

	return getTheValue(value, type, deviceNode, vendorcls);
}
int8 setIPoeEmulatorVendorClassIDValue(void* value, struct DeviceNode*  deviceNode)
{
	char vendorcls[65] = {0};

	memset(vendorcls, 0, sizeof(vendorcls));
	if( strlen((char*)value) > (sizeof(vendorcls) - 1) )
		return -1;
	strcpy(vendorcls, (char*)value);

	tcapi_set(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_VENDORCLASS_STR, vendorcls);
	SetCfgNodeFlag(IPOEEMULATOR_ENTRY_STR, -1);
	return 0;
}
int8 getIPoeEmulatorVendorClassIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIpoeEmulatorAttr.aVendorClassID;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPoeEmulatorVendorClassIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIpoeEmulatorAttr.aVendorClassID;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= IPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getIPoeEmulatorResultValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[65] = {0};

	memset(result, 0, sizeof(result));
	if ( 0 != tcapi_get(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_RESULT_STR, result) )
		memset(result, 0, sizeof(result));

	return getTheValue(value, type, deviceNode, result);
}
int8 getIPoeEmulatorResultAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIpoeEmulatorAttr.aResult;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPoeEmulatorResultAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIpoeEmulatorAttr.aResult;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= IPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getIPoeEmulatorLocalAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char localAddr[65] = {0};

	memset(localAddr, 0, sizeof(localAddr));
	if ( 0 != tcapi_get(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_LOCALADDRESS_STR, localAddr)
		|| 0 == localAddr[0] )
		strcpy(localAddr, "0.0.0.0");

	return getTheValue(value, type, deviceNode, localAddr);
}
int8 getIPoeEmulatorLocalAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIpoeEmulatorAttr.aLocalAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPoeEmulatorLocalAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIpoeEmulatorAttr.aLocalAddress;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= IPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getIPoeEmulatorDefaultGatewayValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char defGateway[65] = {0};

	memset(defGateway, 0, sizeof(defGateway));
	if ( 0 != tcapi_get(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_DEFGATEWAY_STR, defGateway)
		|| 0 == defGateway[0] )
		strcpy(defGateway, "0.0.0.0");

	return getTheValue(value, type, deviceNode, defGateway);
}
int8 getIPoeEmulatorDefaultGatewayAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIpoeEmulatorAttr.aDefaultGateway;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPoeEmulatorDefaultGatewayAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIpoeEmulatorAttr.aDefaultGateway;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= IPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getIPoeEmulatorPingDestIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char pingDST[17] = {0};

	memset(pingDST, 0, sizeof(pingDST));
	if ( 0 != tcapi_get(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_PINGDSTIP_STR, pingDST) )
		memset(pingDST, 0, sizeof(pingDST));

	return getTheValue(value, type, deviceNode, pingDST);
}
int8 setIPoeEmulatorPingDestIPAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char pingDST[17] = {0};
	struct in_addr in_v4addr = {0};

	memset(pingDST, 0, sizeof(pingDST));
	if( strlen((char*)value) > (sizeof(pingDST) - 1) )
		return -1;
	strcpy(pingDST, (char*)value);
	if ( 0 != pingDST[0] && 1 != inet_pton(AF_INET, pingDST, &in_v4addr) )
		return -1;

	tcapi_set(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_PINGDSTIP_STR, pingDST);
	SetCfgNodeFlag(IPOEEMULATOR_ENTRY_STR, -1);
	return 0;
}
int8 getIPoeEmulatorPingDestIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIpoeEmulatorAttr.aPingDestIPAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPoeEmulatorPingDestIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIpoeEmulatorAttr.aPingDestIPAddress;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= IPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getIPoeEmulatorPingNumberOfRepetitionsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char pingNUM[17] = {0};

	memset(pingNUM, 0, sizeof(pingNUM));
	if ( 0 != tcapi_get(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_PINGNUM_STR, pingNUM) )
		strcpy(pingNUM, "5");

	return getTheValue(value, type, deviceNode, pingNUM);
}
int8 setIPoeEmulatorPingNumberOfRepetitionsValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 num = 0;
	char pingNUM[17] = {0};

	memset(pingNUM, 0, sizeof(pingNUM));
	num = *((uint32*)value);
	if( 0 == num )
		return -1;

	sprintf(pingNUM, "%u", num);
	tcapi_set(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_PINGNUM_STR, pingNUM);
	SetCfgNodeFlag(IPOEEMULATOR_ENTRY_STR, -1);
	return 0;
}
int8 getIPoeEmulatorPingNumberOfRepetitionsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIpoeEmulatorAttr.aPingNumberOfRepetitions;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPoeEmulatorPingNumberOfRepetitionsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIpoeEmulatorAttr.aPingNumberOfRepetitions;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= IPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getIPoeEmulatorTimeoutValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char timeout[17] = {0};

	memset(timeout, 0, sizeof(timeout));
	if ( 0 != tcapi_get(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_PINGTIMEOUT_STR, timeout) )
		strcpy(timeout, "2000");

	return getTheValue(value, type, deviceNode, timeout);
}
int8 setIPoeEmulatorTimeoutValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 timeout = 0;
	char pingTimeout[17] = {0};

	memset(pingTimeout, 0, sizeof(pingTimeout));
	timeout = *((uint32*)value);
	if( 0 == timeout )
		return -1;

	sprintf(pingTimeout, "%u", timeout);
	tcapi_set(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_PINGTIMEOUT_STR, pingTimeout);
	SetCfgNodeFlag(IPOEEMULATOR_ENTRY_STR, -1);
	return 0;
}
int8 getIPoeEmulatorTimeoutAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIpoeEmulatorAttr.aTimeout;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPoeEmulatorTimeoutAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIpoeEmulatorAttr.aTimeout;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= IPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getIPoeEmulatorSuccessCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char succCount[16] = {0};

	memset(succCount, 0, sizeof(succCount));
	if ( 0 != tcapi_get(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_PINGSUCC_COUNT_STR, succCount) )
		strcpy(succCount, "0");

	return getTheValue(value, type, deviceNode, succCount);
}
int8 getIPoeEmulatorSuccessCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIpoeEmulatorAttr.aSuccessCount;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPoeEmulatorSuccessCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIpoeEmulatorAttr.aSuccessCount;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= IPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getIPoeEmulatorFailureCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char failCount[16] = {0};

	memset(failCount, 0, sizeof(failCount));
	if ( 0 != tcapi_get(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_PINGFAIL_COUNT_STR, failCount) )
		strcpy(failCount, "0");

	return getTheValue(value, type, deviceNode, failCount);
}
int8 getIPoeEmulatorFailureCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIpoeEmulatorAttr.aFailureCount;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPoeEmulatorFailureCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIpoeEmulatorAttr.aFailureCount;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= IPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getIPoeEmulatorAverageResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char advReptime[16] = {0};

	memset(advReptime, 0, sizeof(advReptime));
	if ( 0 != tcapi_get(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_PINGAVR_REPTIME_STR, advReptime) )
		strcpy(advReptime, "0");

	return getTheValue(value, type, deviceNode, advReptime);
}
int8 getIPoeEmulatorAverageResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIpoeEmulatorAttr.aAverageResponseTime;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPoeEmulatorAverageResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIpoeEmulatorAttr.aAverageResponseTime;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= IPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getIPoeEmulatorMinimumResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char minReptime[16] = {0};

	memset(minReptime, 0, sizeof(minReptime));
	if ( 0 != tcapi_get(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_PINGMIN_REPTIME_STR, minReptime) )
		strcpy(minReptime, "0");

	return getTheValue(value, type, deviceNode, minReptime);
}
int8 getIPoeEmulatorMinimumResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIpoeEmulatorAttr.aMinimumResponseTime;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPoeEmulatorMinimumResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIpoeEmulatorAttr.aMinimumResponseTime;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= IPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getIPoeEmulatorMaximumResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char maxReptime[16] = {0};

	memset(maxReptime, 0, sizeof(maxReptime));
	if ( 0 != tcapi_get(IPOEEMULATOR_ENTRY_STR, IPOEEMULATOR_PINGMAX_REPTIME_STR, maxReptime) )
		strcpy(maxReptime, "0");

	return getTheValue(value, type, deviceNode, maxReptime);
}
int8 getIPoeEmulatorMaximumResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIpoeEmulatorAttr.aMaximumResponseTime;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPoeEmulatorMaximumResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIpoeEmulatorAttr.aMaximumResponseTime;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= IPOE_EMULATOR_ATTR_FLAG;
	return result;
}

