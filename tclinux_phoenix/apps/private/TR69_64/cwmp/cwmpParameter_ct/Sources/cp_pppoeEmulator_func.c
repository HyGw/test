#include "Global_res.h"
#include "cp_pppoeEmulator.h"

/*
struct tag_stPppoeEmulator{
	char		DiagnosticsState[16];
	char		Username[65];
	char		Password[65];
	char		WANInterface[257];
	char		PppAuthProto[5];
	int		RetryTimes;
	
}  */


#define MAX_USER_NAME_LEN 512
#define MAX_USER_ID_LEN 512

/*****************PppoeEmulator DiagnosticsState***********************/
int8 getPppoeEmulatorDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempStatus[16];

	memset( tempStatus, 0, sizeof(tempStatus) );	
	tcapi_get( PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_DIAGNOSTICS_STATE_STR, tempStatus );
	
	return getTheValue(value, type, deviceNode, tempStatus);
}
int8 setPppoeEmulatorDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode)
{
	char oldStatus[10]={0}, newStatus[10]={0};
	
	memset( oldStatus, 0, sizeof(oldStatus) );
	memset( newStatus, 0, sizeof(newStatus) );
	if(strlen((char*)value) > 8 )
		return -1;
	strcpy(newStatus, (char*)value);
	
	if ( (strcmp( "Start", newStatus ) != 0) &&  (strcmp( "Stop", newStatus ) !=0))
	{
		return -1;
	}
	
	tcapi_get(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_DIAGNOSTICS_STATE_STR, oldStatus );

	if ( strcmp( oldStatus, newStatus ) != 0 ) {
		
		tcapi_set(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_DIAGNOSTICS_STATE_STR, newStatus );
	}
	
	SetCfgNodeFlag( "PppoeEmulator", -1 );

	return 0;
}
int8 getPppoeEmulatorDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPppoeEmulatorAttr.aDiagState;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcPppoeEmulatorAttr.aDiagState;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}


/*****************PppoeEmulator Username***********************/
int8 getPppoeEmulatorUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempUsername[65];

	memset( tempUsername, 0, sizeof(tempUsername) );	
	tcapi_get( PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_USERNAME_STR, tempUsername );
	
	return getTheValue(value, type, deviceNode, tempUsername);
}
int8 setPppoeEmulatorUsernameValue(void* value, struct DeviceNode*  deviceNode)
{
	char oldUsername[65]={0}, newUsername[65]={0};
	
	memset( oldUsername, 0, sizeof(oldUsername) );
	memset( newUsername, 0, sizeof(newUsername) );
	if(strlen((char*)value) > 64 )
		return -1;
	strcpy(newUsername, (char*)value);
	
	tcapi_get(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_USERNAME_STR, oldUsername );

	if ( strcmp( oldUsername, newUsername ) != 0 ) {
		
		tcapi_set(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_USERNAME_STR, newUsername );
	}

	SetCfgNodeFlag ("PppoeEmulator", -1);	

	return 0;
}
int8 getPppoeEmulatorUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPppoeEmulatorAttr.aUsername;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcPppoeEmulatorAttr.aUsername;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}


/*****************PppoeEmulator Password***********************/
int8 getPppoeEmulatorPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempPassword[65];

	memset( tempPassword, 0, sizeof(tempPassword) );	
	tcapi_get( PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_PASSWORD_STR, tempPassword );

	if (strlen(tempPassword) > 0)	
		return getTheValue(value, type, deviceNode, "******");
	else
		return getTheValue(value, type, deviceNode, "");
}
int8 setPppoeEmulatorPasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	char oldPassword[65]={0}, newPassword[65]={0};
	
	memset( oldPassword, 0, sizeof(oldPassword) );
	memset( newPassword, 0, sizeof(newPassword) );
	if(strlen((char*)value) > 64 )
		return -1;
	strcpy(newPassword, (char*)value);
	
	tcapi_get(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_PASSWORD_STR, oldPassword );

	if ( strcmp( oldPassword, newPassword ) != 0 ) {
		
		tcapi_set(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_PASSWORD_STR, newPassword );
	}

	SetCfgNodeFlag ("PppoeEmulator", -1);	

	return 0;
}
int8 getPppoeEmulatorPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPppoeEmulatorAttr.aPassword;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcPppoeEmulatorAttr.aPassword;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}


/*****************PppoeEmulator WANInterface***********************/
int8 getPppoeEmulatorWanInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempWanInterface[257];

	memset( tempWanInterface, 0, sizeof(tempWanInterface) );	
	tcapi_get( PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_WANINTERFACE_STR, tempWanInterface );
	
	return getTheValue(value, type, deviceNode, tempWanInterface);
}

static int8 checkWanInterface(char* setvalue, char* nodeName)
{
	char DefaultIface_WAN[]="InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";
	char tempvalue[2];
	int len;
	int pvcnode;	
	DeviceNodePtr Node = NULL;
	int entrynode;
	char *tmp = NULL;
	char tmpstr[16] = {0};
	char vlanMode[16] = {0};
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
	char barrier[12] = {0};
#endif

	len = strlen(DefaultIface_WAN);

	if(strncmp(DefaultIface_WAN, setvalue, len) == 0){//WAN interface, get the gateway.
		/*check the value is valid or not.*/
		cwmpSearchDeviceNodeByName(setvalue, rpc.IGDDeviceRootPtr, &Node);
		if(Node == NULL){
			tc_printf("\r\nparameter.c---[setPppoeEmulatorWanInterfaceValue]---Invalid Value!");
			return -1;
		}
		
		if(setvalue[len]<'1'||setvalue[len]>'8'){
			tc_printf("\r\nparameter.c---[setPppoeEmulatorWanInterfaceValue]---Invalid pvc number!");
			return -1;
		}
		
		tempvalue[0] = setvalue[len];
		tempvalue[1] = '\0';
		pvcnode = atoi(tempvalue) - 1;
		
		if((tmp = strstr(setvalue, "WANPPPConnection")) != NULL){
			memset(tempvalue, 0, sizeof(tempvalue));
			tempvalue[0] = *(tmp + strlen("WANPPPConnection") + 1);
			tempvalue[1] = '\0';
			entrynode = atoi(tempvalue) - 1;
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
#if defined(TCSUPPORT_CT_PON)
			sprintf(nodeName, "Wan_PVC%d", pvcnode);
			tcapi_get(nodeName, "VLANMode", vlanMode);
			tcapi_set(PPPOEEMULATOR_ENTRY_STR, "VLANMode", vlanMode);
			tcapi_get(nodeName, "VLANID", tmpstr);
			tcapi_set(PPPOEEMULATOR_ENTRY_STR, "VLANID", tmpstr);
			tcapi_get(nodeName, "DOT1P", tmpstr);
			tcapi_set(PPPOEEMULATOR_ENTRY_STR, "DOT1P", tmpstr);
			tcapi_set(PPPOEEMULATOR_ENTRY_STR, "NAS_IF", "pon");
#else
#endif
			sprintf(nodeName,"Wan_PVC%d_Entry%d", pvcnode, entrynode);
			tcapi_get(nodeName, "IPVERSION", tmpstr);
			tcapi_set(PPPOEEMULATOR_ENTRY_STR, "IPVERSION", tmpstr);
			if ( 0 == strcmp(tmpstr, "IPv4/IPv6") )
				tcapi_set(PPPOEEMULATOR_ENTRY_STR, "WANPPPConnectionIPMode", "3");
			else if ( 0 == strcmp(tmpstr, "IPv6") )
				tcapi_set(PPPOEEMULATOR_ENTRY_STR, "WANPPPConnectionIPMode", "2");
			else
				tcapi_set(PPPOEEMULATOR_ENTRY_STR, "WANPPPConnectionIPMode", "1");

			if ( 0 == tcapi_get(nodeName, "DsliteEnable", tmpstr)
				&& 0 == strcmp(tmpstr, "Yes") )
				tcapi_set(PPPOEEMULATOR_ENTRY_STR, "Dslite_Enable", "1");
			else
				tcapi_set(PPPOEEMULATOR_ENTRY_STR, "Dslite_Enable", "0");
			tcapi_get(nodeName, "DHCPv6", tmpstr);
			tcapi_set(PPPOEEMULATOR_ENTRY_STR, "DHCPv6", tmpstr);
			tcapi_get(nodeName, "WanMode", tmpstr);
			tcapi_set(PPPOEEMULATOR_ENTRY_STR, "WanMode", tmpstr);
#else
			sprintf(nodeName,"Wan_PVC%d_Entry%d", pvcnode, entrynode);	
			tcapi_get(nodeName, "VLANID", tmpstr);
			tcapi_set(PPPOEEMULATOR_ENTRY_STR, "VLANID", tmpstr);
			tcapi_get(nodeName, "VLANMode", vlanMode);
			tcapi_set(PPPOEEMULATOR_ENTRY_STR, "VLANMode", vlanMode);
#endif
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

int8 setPppoeEmulatorWanInterfaceValue(void* value, struct DeviceNode*  deviceNode)
{	
	char *setvalue;
	char nodeName[32] = {0};
	
	setvalue=(char*)value; 

	if (0 == checkWanInterface(setvalue, nodeName))
	{
		tcapi_set(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_WANINTERFACE_STR, setvalue );
		tcapi_set(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_WANINTERFACE_STR"_node", nodeName );
		return 0;
	}

	return -1;
}
int8 getPppoeEmulatorWanInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPppoeEmulatorAttr.aWANInterface;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorWanInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcPppoeEmulatorAttr.aWANInterface;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}


/*****************PppoeEmulator PPPAuthenticationProtocol****/
int8 getPppoeEmulatorPppAuthProtoValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempPppAuthProto[5];
 
	memset( tempPppAuthProto, 0, sizeof(tempPppAuthProto) );	
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
	tcapi_get(PPPOEEMULATOR_ENTRY_STR
			, "fakePPPAuthenticationProtocol", tempPppAuthProto);
#else
	tcapi_get( PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_PPPAUTHPROTO_STR,  tempPppAuthProto );
#endif

	return getTheValue(value, type, deviceNode, tempPppAuthProto);
}
int8 setPppoeEmulatorPppAuthProtoValue(void* value, struct DeviceNode*  deviceNode)
{
	char oldPppAuthProto[5]={0}, newPppAuthProto[5]={0};
	
	memset( oldPppAuthProto, 0, sizeof(oldPppAuthProto) );
	memset( newPppAuthProto, 0, sizeof(newPppAuthProto) );
	
	if(strlen((char*)value) > 4 )
	{
		return -1;
	}
	
	strcpy(newPppAuthProto, (char*)value);
	
	if ( (strcmp( "PAP", newPppAuthProto ) != 0) &&  (strcmp( "CHAP", newPppAuthProto ) !=0) &&
		(strcmp( "AUTO", newPppAuthProto ) != 0) && (strcmp( "", newPppAuthProto ) != 0))
	{
		return -1;
	}

#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
	tcapi_set(PPPOEEMULATOR_ENTRY_STR
			, "fakePPPAuthenticationProtocol", newPppAuthProto);
#else
	tcapi_get(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_PPPAUTHPROTO_STR, oldPppAuthProto );

	if ( strcmp( oldPppAuthProto, newPppAuthProto ) != 0 ) {
		
		tcapi_set(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_PPPAUTHPROTO_STR, newPppAuthProto );
	}
#endif

	return 0;
}
int8 getPppoeEmulatorPppAuthProtoAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPppoeEmulatorAttr.aPPPAuthProto;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorPppAuthProtoAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcPppoeEmulatorAttr.aPPPAuthProto;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}


/*****************PppoeEmulator RetryTimes****/
int8 getPppoeEmulatorRetryTimesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempRetryTimes[5];

	memset( tempRetryTimes, 0, sizeof(tempRetryTimes) );	
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
	if ( 0 != tcapi_get( PPPOEEMULATOR_ENTRY_STR
			, "fakeRetryTimes",  tempRetryTimes )
		|| 0 == tempRetryTimes[0] )
		strcpy(tempRetryTimes, "0");
#else
	tcapi_get( PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_RETRYTIMES_STR,  tempRetryTimes );
	tempRetryTimes[0]--;
#endif

	return getTheValue(value, type, deviceNode, tempRetryTimes);
}
int8 setPppoeEmulatorRetryTimesValue(void* value, struct DeviceNode*  deviceNode)
{
	int RetryTimes = 0;
	char tmpbuf[15] = {0};
	
	RetryTimes = *((int *)value);

#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
	if ( RetryTimes < 0 )
		return -1;
	sprintf(tmpbuf, "%d", RetryTimes);
	tcapi_set(PPPOEEMULATOR_ENTRY_STR, "fakeRetryTimes", tmpbuf);
#else
	if (RetryTimes <0 || RetryTimes > 3)
	{
		return -1;
	}
	else
	{
		RetryTimes++;
		sprintf(tmpbuf, "%d", RetryTimes);
		tcapi_set(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_RETRYTIMES_STR, tmpbuf );
	}
#endif

	return 0;
}
int8 getPppoeEmulatorRetryTimesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPppoeEmulatorAttr.aRetryTimes;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorRetryTimesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcPppoeEmulatorAttr.aRetryTimes;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}


/*****************PppoeEmulator Result***********************/
int8 getPppoeEmulatorResultValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempResult[128];

	memset( tempResult, 0, sizeof(tempResult) );	
	tcapi_get( PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_RESULT_STR, tempResult );
	
	return getTheValue(value, type, deviceNode, tempResult);
}
int8 setPppoeEmulatorResultValue(void* value, struct DeviceNode*  deviceNode)
{
	return 0;
}
int8 getPppoeEmulatorResultAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPppoeEmulatorAttr.aResult;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorResultAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcPppoeEmulatorAttr.aResult;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}

/*****************PppoeEmulator PPPsessionID***********************/
int8 getPppoeEmulatorPppSessionIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempPppSessionID[32];

	memset( tempPppSessionID, 0, sizeof(tempPppSessionID) );	
	tcapi_get( PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_PPPSESSIONID_STR, tempPppSessionID );
	
	return getTheValue(value, type, deviceNode, tempPppSessionID);
}
int8 setPppoeEmulatorPppSessionIDValue(void* value, struct DeviceNode*  deviceNode)
{
	return 0;
}
int8 getPppoeEmulatorPppSessionIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPppoeEmulatorAttr.aPPPSessionID;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorPppSessionIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcPppoeEmulatorAttr.aPPPSessionID;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}

/*****************PppoeEmulator ExternalIPAddress***********************/
int8 getPppoeEmulatorExternalIPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempExternalIP[32];

	memset( tempExternalIP, 0, sizeof(tempExternalIP) );	
	tcapi_get( PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_EXTERNALIPADDRESS_STR, tempExternalIP );
	
	return getTheValue(value, type, deviceNode, tempExternalIP);
}
int8 setPppoeEmulatorExternalIPValue(void* value, struct DeviceNode*  deviceNode)
{
	return 0;
}
int8 getPppoeEmulatorExternalIPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPppoeEmulatorAttr.aExternalIP;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorExternalIPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcPppoeEmulatorAttr.aExternalIP;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}

/*****************PppoeEmulator DefaultGateway***********************/
int8 getPppoeEmulatorDefGwValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempDefGw[32];

	memset( tempDefGw, 0, sizeof(tempDefGw) );	
	tcapi_get( PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_DEFAULTGATEWAY_STR, tempDefGw );
	
	return getTheValue(value, type, deviceNode, tempDefGw);
}
int8 setPppoeEmulatorDefGwValue(void* value, struct DeviceNode*  deviceNode)
{
	return 0;
}
int8 getPppoeEmulatorDefGwAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPppoeEmulatorAttr.aDefGw;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorDefGwAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcPppoeEmulatorAttr.aDefGw;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}

#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
int8 getPppoeEmulatorErrCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char errCode[256] = {0};

	memset(errCode, 0, sizeof(errCode));
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
	if ( 0 != tcapi_get(PPPOEEMULATOR_ENTRY_STR
		, "ConnectionErrorCode", errCode) )
		memset(errCode, 0, sizeof(errCode));
#else
	if ( 0 != tcapi_get(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_ERRORCODE_STR, errCode) )
		memset(errCode, 0, sizeof(errCode));
#endif

	return getTheValue(value, type, deviceNode, errCode);
}
int8 getPppoeEmulatorErrCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcPppoeEmulatorAttr.aErrorCode;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorErrCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcPppoeEmulatorAttr.aErrorCode;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getPppoeEmulatorWanIPModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char wanipmode[32] = {0};

	memset(wanipmode, 0, sizeof(wanipmode));
	if ( 0 != tcapi_get(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_WANIPMODE_STR, wanipmode) )
		strcpy(wanipmode, "1");

	return getTheValue(value, type, deviceNode, wanipmode);
}
int8 getPppoeEmulatorWanIPModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcPppoeEmulatorAttr.aWanIPMode;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorWanIPModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcPppoeEmulatorAttr.aWanIPMode;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getPppoeEmulatorDsliteEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char dsliteEnable[32] = {0};

	memset(dsliteEnable, 0, sizeof(dsliteEnable));
	if ( 0 != tcapi_get(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_DSLITEENABLE_STR, dsliteEnable) )
		strcpy(dsliteEnable, "0");

	return getTheValue(value, type, deviceNode, dsliteEnable);
}
int8 getPppoeEmulatorDsliteEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcPppoeEmulatorAttr.aDsliteEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorDsliteEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcPppoeEmulatorAttr.aDsliteEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getPppoeEmulatorAftrAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char aftraddr[64] = {0};

	memset(aftraddr, 0, sizeof(aftraddr));
	if ( 0 != tcapi_get(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_AFTRADDRESS_STR, aftraddr) )
		strcpy(aftraddr, "::");

	return getTheValue(value, type, deviceNode, aftraddr);
}
int8 getPppoeEmulatorAftrAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcPppoeEmulatorAttr.aAftrAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorAftrAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcPppoeEmulatorAttr.aAftrAddress;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getPppoeEmulatorWANIPv6AddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char wanIPv6Addr[40] = {0};

	memset(wanIPv6Addr, 0, sizeof(wanIPv6Addr));
	if ( 0 != tcapi_get(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_WANIPV6ADDRESS_STR, wanIPv6Addr) )
		strcpy(wanIPv6Addr, "::");

	return getTheValue(value, type, deviceNode, wanIPv6Addr);
}
int8 getPppoeEmulatorWANIPv6AddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcPppoeEmulatorAttr.aWANIPv6IPAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorWANIPv6AddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcPppoeEmulatorAttr.aWANIPv6IPAddress;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getPppoeEmulatorWANIPv6DNSAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char wanIPv6DNSAddr[40] = {0};

	memset(wanIPv6DNSAddr, 0, sizeof(wanIPv6DNSAddr));
	if ( 0 != tcapi_get(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_WANIPV6DNSADDRESS_STR, wanIPv6DNSAddr) )
		strcpy(wanIPv6DNSAddr, "::");

	return getTheValue(value, type, deviceNode, wanIPv6DNSAddr);
}
int8 getPppoeEmulatorWANIPv6DNSAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcPppoeEmulatorAttr.aWANIPv6DNSServers;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorWANIPv6DNSAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcPppoeEmulatorAttr.aWANIPv6DNSServers;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getPppoeEmulatorWANIPv6PrefixValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char wanIPv6Prefix[40] = {0};

	memset(wanIPv6Prefix, 0, sizeof(wanIPv6Prefix));
	if ( 0 != tcapi_get(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_WANIPV6PREFIX_STR, wanIPv6Prefix) )
		strcpy(wanIPv6Prefix, "::");

	return getTheValue(value, type, deviceNode, wanIPv6Prefix);
}
int8 getPppoeEmulatorWANIPv6PrefixAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcPppoeEmulatorAttr.aWANIPv6Prefix;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorWANIPv6PrefixAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcPppoeEmulatorAttr.aWANIPv6Prefix;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getPppoeEmulatorWANDefIPv6GatewayValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char wanDefIPv6GW[40] = {0};

	memset(wanDefIPv6GW, 0, sizeof(wanDefIPv6GW));
	if ( 0 != tcapi_get(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_WANDEFIPV6GATEWAY_STR, wanDefIPv6GW) )
		strcpy(wanDefIPv6GW, "::");

	return getTheValue(value, type, deviceNode, wanDefIPv6GW);
}
int8 getPppoeEmulatorWANDefIPv6GatewayAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcPppoeEmulatorAttr.aWANDefaultIPv6GW;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorWANDefIPv6GatewayAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcPppoeEmulatorAttr.aWANDefaultIPv6GW;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}

int8 getPppoeEmulatorLANIPv6PrefixValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char LANIPv6Prefix[40] = {0};

	memset(LANIPv6Prefix, 0, sizeof(LANIPv6Prefix));
	if ( 0 != tcapi_get(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_LANIPV6PREFIX_STR, LANIPv6Prefix) )
		strcpy(LANIPv6Prefix, "::");

	return getTheValue(value, type, deviceNode, LANIPv6Prefix);
}
int8 getPppoeEmulatorLANIPv6PrefixAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcPppoeEmulatorAttr.aLANIPv6Prefix;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorLANIPv6PrefixAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcPppoeEmulatorAttr.aLANIPv6Prefix;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_ATTR_FLAG;
	return result;
}
#endif


int PppoeEmulatorCommit(cfgNode_ptr pCfgNode)
{
	char acUsername[65] = {0};
	char acPassword[65] = {0};
	char acWanInterface[257] = {0};
	char acAuthProto[5] = {0};
	int iRetryTimes = 0;
	char acDiagStat[16] = {0};
	char acTmp[16] = {0};
	char nodeName[32] = {0};

	tcapi_get (PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_DIAGNOSTICS_STATE_STR, acDiagStat);
	tcapi_get (PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_USERNAME_STR, acUsername);
	tcapi_get (PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_PASSWORD_STR, acPassword);
	tcapi_get (PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_WANINTERFACE_STR, acWanInterface);
	tcapi_get (PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_PPPAUTHPROTO_STR, acAuthProto);
	tcapi_get (PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_RETRYTIMES_STR, acTmp);
	iRetryTimes = atoi(acTmp);
	
	if (strcmp(acDiagStat, "Start") == 0)
	{
		if (strlen(acWanInterface) == 0
#if !defined(TCSUPPORT_C7) && !defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
			|| (1 > iRetryTimes || 4 < iRetryTimes)
#endif
			|| (0 != checkWanInterface(acWanInterface, nodeName)))
		{
			//tcapi_set(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_DIAGNOSTICS_STATE_STR, "None");
			return -1;
		}
		tcapi_set(PPPOEEMULATOR_ENTRY_STR, PPPOEEMULATOR_DIAGNOSTICS_STATE_STR, "Running");
#if defined(TCSUPPORT_CT_PON_CZ_GDCS)
		tcapi_set(PPPOEEMULATOR_ENTRY_STR,"Auth_Success", "0");
#endif
	}
	tcapi_commit( pCfgNode->nodeName );
	return 0;
}


int8 getPppoeEmulatorBridgeEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	/*merge from linos*/
	char Enable[10] = {"0"};
	 
	tcapi_get(PPPOEEMULATOR_BRIDGE_ENTRY_STR, PPPOEEMULATOR_BRIDGE_ENABLE_STR,Enable);
	
	return getTheValue(value, type, deviceNode, Enable);
}
int8 setPppoeEmulatorBridgeEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 Enable;
	
	Enable = *((uint8*)value);
	if(Enable == 0){
		tcapi_set(PPPOEEMULATOR_BRIDGE_ENTRY_STR, PPPOEEMULATOR_BRIDGE_ENABLE_STR,"0");
	}
	else if(Enable == 1){
		tcapi_set(PPPOEEMULATOR_BRIDGE_ENTRY_STR, PPPOEEMULATOR_BRIDGE_ENABLE_STR,"1");
	}
	else{
		return -1;
	}
	
	SetCfgNodeFlag ("PppoeEmulatorBridge", -1);	
	return 1;
}

int8 getPppoeEmulatorBridgeEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPppoeEmulatorBridgeAttr.aEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorBridgeEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcPppoeEmulatorBridgeAttr.aEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_BRIDGE_ATTR_FLAG;
	return result;
}


/*****************PppoeEmulator Bridge Username***********************/
int8 getPppoeEmulatorBridgeUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempUsername[65];

	memset( tempUsername, 0, sizeof(tempUsername) );	
	tcapi_get( PPPOEEMULATOR_BRIDGE_ENTRY_STR, PPPOEEMULATOR_BRIDGE_USERNAME_STR, tempUsername );
	
	return getTheValue(value, type, deviceNode, tempUsername);
}
int8 setPppoeEmulatorBridgeUsernameValue(void* value, struct DeviceNode*  deviceNode)
{
	char oldUsername[65]={0}, newUsername[65]={0};
	
	memset( oldUsername, 0, sizeof(oldUsername) );
	memset( newUsername, 0, sizeof(newUsername) );
	if(strlen((char*)value) > 64 )
		return -1;
	strcpy(newUsername, (char*)value);
	
	tcapi_get(PPPOEEMULATOR_BRIDGE_ENTRY_STR, PPPOEEMULATOR_BRIDGE_USERNAME_STR, oldUsername );

	if ( strcmp( oldUsername, newUsername ) != 0 ) {
		
		tcapi_set(PPPOEEMULATOR_BRIDGE_ENTRY_STR, PPPOEEMULATOR_BRIDGE_USERNAME_STR, newUsername );
	}

	return 0;
}
int8 getPppoeEmulatorBridgeUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPppoeEmulatorBridgeAttr.aUsername;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorBridgeUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcPppoeEmulatorBridgeAttr.aUsername;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_BRIDGE_ATTR_FLAG;
	return result;
}


/*****************PppoeEmulator Bridge Password***********************/
int8 getPppoeEmulatorBridgePasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempPassword[65];

	memset( tempPassword, 0, sizeof(tempPassword) );	
	tcapi_get( PPPOEEMULATOR_BRIDGE_ENTRY_STR, PPPOEEMULATOR_BRIDGE_PASSWORD_STR, tempPassword );
	
	if (strlen(tempPassword) > 0)	
		return getTheValue(value, type, deviceNode, "******");
	else
		return getTheValue(value, type, deviceNode, "");
	
}
int8 setPppoeEmulatorBridgePasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	char oldPassword[65]={0}, newPassword[65]={0};
	
	memset( oldPassword, 0, sizeof(oldPassword) );
	memset( newPassword, 0, sizeof(newPassword) );
	if(strlen((char*)value) > 64 )
		return -1;
	strcpy(newPassword, (char*)value);
	
	tcapi_get(PPPOEEMULATOR_BRIDGE_ENTRY_STR, PPPOEEMULATOR_BRIDGE_PASSWORD_STR, oldPassword );

	if ( strcmp( oldPassword, newPassword ) != 0 ) {
		
		tcapi_set(PPPOEEMULATOR_BRIDGE_ENTRY_STR, PPPOEEMULATOR_BRIDGE_PASSWORD_STR, newPassword );
	}

	return 0;
}
int8 getPppoeEmulatorBridgePasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPppoeEmulatorBridgeAttr.aPassword;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorBridgePasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcPppoeEmulatorBridgeAttr.aPassword;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_BRIDGE_ATTR_FLAG;
	return result;
}


/*****************PppoeEmulator brdige timelist****/
int8 getPppoeEmulatorBridgeTimeListValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempRetryTimes[5];

	memset( tempRetryTimes, 0, sizeof(tempRetryTimes) );	
	tcapi_get( PPPOEEMULATOR_BRIDGE_ENTRY_STR, PPPOEEMULATOR_BRIDGE_TIMELIST_STR,  tempRetryTimes );
	return getTheValue(value, type, deviceNode, tempRetryTimes);
}
int8 setPppoeEmulatorBridgeTimeListValue(void* value, struct DeviceNode*  deviceNode)
{
	int RetryTimes = 0;
	char tmpbuf[15] = {0};
	
	RetryTimes = atoi((char *)value);

	if (RetryTimes <0 || RetryTimes > 9999)
	{
		return -1;
	}
	else
	{
		sprintf(tmpbuf, "%d", RetryTimes);
		tcapi_set(PPPOEEMULATOR_BRIDGE_ENTRY_STR, PPPOEEMULATOR_BRIDGE_TIMELIST_STR, tmpbuf );
	}

	SetCfgNodeFlag ("PppoeEmulatorBridge", -1);	
	
	return 0;
}
int8 getPppoeEmulatorBridgeTimeListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPppoeEmulatorBridgeAttr.aTimeList;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorBridgeTimeListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcPppoeEmulatorBridgeAttr.aTimeList;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_BRIDGE_ATTR_FLAG;
	return result;
}

/*****************PppoeEmulator brdige fail retry timelist****/
int8 getPppoeEmulatorBridgeFailRetryTimeListValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempRetryTimes[5];

	memset( tempRetryTimes, 0, sizeof(tempRetryTimes) );	
	tcapi_get( PPPOEEMULATOR_BRIDGE_ENTRY_STR, PPPOEEMULATOR_BRIDGE_FAILRETRYTIMELIST_STR,  tempRetryTimes );
	return getTheValue(value, type, deviceNode, tempRetryTimes);
}
int8 setPppoeEmulatorBridgeFailRetryTimeListValue(void* value, struct DeviceNode*  deviceNode)
{
	int RetryTimes = 0;
	char tmpbuf[15] = {0};
	
	RetryTimes = atoi((char *)value);

	if (RetryTimes <0 || RetryTimes > 999)
	{
		return -1;
	}
	else
	{
		sprintf(tmpbuf, "%d", RetryTimes);
		tcapi_set(PPPOEEMULATOR_BRIDGE_ENTRY_STR, PPPOEEMULATOR_BRIDGE_FAILRETRYTIMELIST_STR, tmpbuf );
	}

	SetCfgNodeFlag ("PppoeEmulatorBridge", -1);	

	return 0;
}
int8 getPppoeEmulatorBridgeFailRetryTimeListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcPppoeEmulatorBridgeAttr.aFailRetryTimeList;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPppoeEmulatorBridgeFailRetryTimeListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcPppoeEmulatorBridgeAttr.aFailRetryTimeList;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag1 |= PPPOE_EMULATOR_BRIDGE_ATTR_FLAG;
	return result;
}

int PppoeEmulatorBridgeCommit(cfgNode_ptr pCfgNode)
{
	tcapi_commit( pCfgNode->nodeName );
	return 0;
}

