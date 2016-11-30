#include "Global_res.h"
#include "cp_managerment.h"


extern int informTimerID;
int8 getKickURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char URL[256] = {'\0'};

	tcapi_get("Cwmp_Entry", "KickURL", URL);
	return getTheValue(value, type, deviceNode, URL);
}
int8 getKickURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{

	uint8  attribute;
	int8  result;

	attribute = tcCwmpAttr.aKickURL;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setKickURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCwmpAttr.aKickURL;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= CWMP_ATTR_FLAG;
	return result;
}

int8 getURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* tempValue = NULL;
	char URL[256] = {'\0'};

	tcapi_get("Cwmp_Entry", "acsUrl", URL);
	return getTheValue(value, type, deviceNode, URL);
}
int8 setURLValue(void* value, struct DeviceNode*  deviceNode)
{
	SetCfgNodeFlag( "Cwmp", -1 );
	
	if(strlen((char*)value) > 256){
		tc_printf("==>setURLValue(): The Length should not be more than 256.\n");
		return -1;
	}

	tcapi_set("Cwmp_Entry", "acsUrl", (char*)value);
	
	return 0;
}
int8 getURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{

	uint8  attribute;
	int8  result;

	attribute = tcCwmpAttr.aURL;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCwmpAttr.aURL;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= CWMP_ATTR_FLAG;
	return result;
}
int8 getUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char usrName[256] = {'\0'};
	tcapi_get("Cwmp_Entry", "acsUserName", usrName);
	
	return getTheValue(value, type, deviceNode, usrName);
}
int8 setUsernameValue(void* value, struct DeviceNode*  deviceNode)
{
	char *usrName = NULL;

	SetCfgNodeFlag( "Cwmp", -1 );
	
	if(strlen((char*)value) > 256){
		tc_printf("==>setUsernameValue: the length of username should not larger than 256.\n");
		return -1;
	}
	
	usrName = (char*)value;
	tcapi_set("Cwmp_Entry", "acsUserName", usrName);
	return 0;
}
int8 getUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCwmpAttr.aUsername;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCwmpAttr.aUsername;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= CWMP_ATTR_FLAG;
	return result;
}
int8 getPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char psw[64] = {'\0'};

#if defined(TCSUPPORT_C1_CUC) || defined(TCSUPPORT_CT_MIDWARE)
	tcapi_get(CWMP_NODE, ACSPASSWD, psw);
#endif
	return getTheValue(value, type, deviceNode, psw);
}
int8 setPasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	char *psw = NULL;
	
	psw = (char*)value;

	SetCfgNodeFlag( "Cwmp", -1 );
	
	if(strlen(psw) > 256){
		tc_printf("==>setPasswordValue: the length of password should not larger than 256.\n");
		return -1;
	}
	
	tcapi_set("Cwmp_Entry", "acsPassword", psw);
	return 0;
}
int8 getPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCwmpAttr.aPassword;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCwmpAttr.aPassword;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= CWMP_ATTR_FLAG;
	return result;
}
int8 getPeriodicInformEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char perInfEnable[10] = {'\0'};
	tcapi_get("Cwmp_Entry", "periodActive", perInfEnable);

	if(!strcmp(perInfEnable, "Yes")){
		strcpy(perInfEnable, "1");
	}
	else if(!strcmp(perInfEnable, "No")){
		strcpy(perInfEnable, "0");
	}
	else{
		strcpy(perInfEnable, " ");
	}

	return getTheValue(value, type, deviceNode, perInfEnable);
}
int8 setPeriodicInformEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 perInfoEnable;
	char enable[10] = {'\0'};
	
	perInfoEnable = *((uint8*)value);

	SetCfgNodeFlag( "Cwmp", -1 );
	
	if(perInfoEnable == 1){
		strcpy(enable, "Yes");
	}
	else if(perInfoEnable == 0){
		strcpy(enable, "No");
	}
	else{
		tc_printf("==> setPeriodicInformEnableValue: value should be  0 or 1.\n");
		return -1;
	}

	tcapi_set("Cwmp_Entry", "periodActive", enable);

	return 0;
}
int8 getPeriodicInformEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCwmpAttr.aPeriodicInformEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPeriodicInformEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCwmpAttr.aPeriodicInformEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= CWMP_ATTR_FLAG;
	return result;
	
}
int8 getPeriodicInformIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char perInfIntval[10] = {'\0'};
	tcapi_get("Cwmp_Entry", "periodInterval", perInfIntval);
	
	return getTheValue(value, type, deviceNode, perInfIntval);
}
int8 setPeriodicInformIntervalValue(void* value, struct DeviceNode*  deviceNode)
{
	char peridicStr[20] = {'\0'};
	uint32 periodic;

	periodic = *((uint32*)value);

	
	if(periodic < 1){
		tc_printf("==> setPeriodicInformIntervalValue: periodic can not small than 1.");
		return -1;
	}

	sprintf(peridicStr, "%lu", periodic);
	tcapi_set("Cwmp_Entry", "periodInterval", peridicStr);
	SetCfgNodeFlag( "Cwmp", -1 );
	return 0;
}
int8 getPeriodicInformIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCwmpAttr.aPeriodicInformInterval;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPeriodicInformIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCwmpAttr.aPeriodicInformInterval;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= CWMP_ATTR_FLAG;
	return result;
}
int8 getPeriodicInformTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* tempValue = NULL;
	char  informPeriodTime[64];

	memset(informPeriodTime, 0, sizeof(informPeriodTime));
	strcpy((char*)informPeriodTime, getcwmpPrdInformTime());
	return getTheValue(value, type, deviceNode, informPeriodTime);
}
int8 setPeriodicInformTimeValue(void* value, struct DeviceNode*  deviceNode)
{
	char *informPeriodTime = NULL;

	informPeriodTime = (char*)(value); 
	if (!checkPeriodInformTimeFormat(informPeriodTime))
	{
		tc_printf("==>setPeriodInformTimeValue: periodInformTime format is wrong.\n");
		return -1;
	}
	setcwmpPrdInformTime(informPeriodTime);
	SetCfgNodeFlag( "Cwmp", -1 );
	if (!isPITUnknownValue(informPeriodTime) && (informTimerID))
	{
		timerPeriodicStop(informTimerID);
		cwmpSetInformTimerID(0);
	}
	else{
		return -1;
	}
	
	return 0;
}
int8 getPeriodicInformTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCwmpAttr.aPeriodicInformTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPeriodicInformTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCwmpAttr.aPeriodicInformTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= CWMP_ATTR_FLAG;
	return result;
}

int8 getParameterKeyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* tempValue = NULL;
	char  parameterkey[64];

	memset(parameterkey, 0, sizeof(parameterkey));
	sprintf(parameterkey,"%s",getParameterKey());	
	return getTheValue(value, type, deviceNode, parameterkey);
}
int8 getParameterKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCwmpAttr.aParameterKey;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setParameterKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCwmpAttr.aParameterKey;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= CWMP_ATTR_FLAG;
	return result;
}
int8 getConnectionRequestURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* tempValue = NULL;
	char  ConnectionRequestURL[257] = {'\0'};
	
	strcpy(ConnectionRequestURL,getConRequestURL());	
	return getTheValue(value, type, deviceNode, ConnectionRequestURL);
}
int8 getConnectionRequestURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCwmpAttr.aConnectionRequestURL;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setConnectionRequestURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCwmpAttr.aConnectionRequestURL;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= CWMP_ATTR_FLAG;
	return result;
}
int8 getConnectionRequestUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{

	char* tempValue = NULL;
	char  RequestUsername[257] = {'\0'};

	strcpy(RequestUsername,getCPEUserName());	
	return getTheValue(value, type, deviceNode, RequestUsername);
}
int8 setConnectionRequestUsernameValue(void* value, struct DeviceNode*  deviceNode)
{
	char *RequestUsername = NULL;

	SetCfgNodeFlag( "Cwmp", -1 );
	
	if(strlen((char*)(value)) > 256){
		tc_printf("==>setConnectionRequestUsernameValue: the length of connectionRequestUsername is too long.\n");
		return -1;
	}
	
	RequestUsername = (char*)(value);
	
	setCPEUserName(RequestUsername);
	return 0;
}
int8 getConnectionRequestUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCwmpAttr.aConnectionRequestUsername;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setConnectionRequestUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCwmpAttr.aConnectionRequestUsername;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= CWMP_ATTR_FLAG;
	return result;
}
int8 getConnectionRequestPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{


	char  RequestPassword[128] = {'\0'};

#if defined(TCSUPPORT_C1_CUC) || defined(TCSUPPORT_CT_MIDWARE)
	strcpy(RequestPassword,getCPEPassword());
#else
	RequestPassword[0] = '\0';
#endif
	return getTheValue(value, type, deviceNode, RequestPassword);
}
int8 setConnectionRequestPasswordValue(void* value, struct DeviceNode*  deviceNode)
{

	char *RequestPassword = NULL;

	SetCfgNodeFlag( "Cwmp", -1 );
	
	if(strlen((char*)(value)) > 256){
		tc_printf("==> setConnectionRequestPasswordValue: RequestPassword can not long than 256.\n");
		return -1;
	}
	RequestPassword = (char*)(value); 
	setCPEPassword(RequestPassword);
	return 0;
}
int8 getConnectionRequestPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCwmpAttr.aConnectionRequestPassword;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setConnectionRequestPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCwmpAttr.aConnectionRequestPassword;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= CWMP_ATTR_FLAG;
	return result;
}
int8 getUpgradesManagedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* tempValue = NULL;
	char  upgradesmanaged[6];

	memset(upgradesmanaged, 0, sizeof(upgradesmanaged));
	sprintf(upgradesmanaged,"%d",spSysParam.tcCwmpAttr->vUpgradesManaged);  
	return getTheValue(value, type, deviceNode, upgradesmanaged);
}
int8 setUpgradesManagedValue(void* value, struct DeviceNode*  deviceNode)
{
	boolean* upgradesmanaged;

	upgradesmanaged=(boolean*)value;
	if(*upgradesmanaged==0||*upgradesmanaged==1){	
		spSysParam.tcCwmpAttr->vUpgradesManaged=*upgradesmanaged;
	}
	else {
		return -1; //return fault code
       }
	return 0;
	
}
int8 getUpgradesManagedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcCwmpAttr.aUpgradesManaged;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setUpgradesManagedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcCwmpAttr.aUpgradesManaged;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= CWMP_ATTR_FLAG;
	return result;
}

#ifdef TR111
int	cwmpInitManageableDeviceSubTree(DeviceNodePtr deviceNode)
{	
	int i, rtnCode = 0, instance;
	char buf[10];

	memset( buf, 0, sizeof(buf) );
	
	tcapi_get( "DhcpClient_Info", "Count", buf );
	
	instance = atoi(buf);

	rtnCode = RegisterDynNodeFromIndex( deviceNode, ManageableDeviceIndex, instance );
	
	return rtnCode;
}

int8 getmDevNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{	
	char tempValue[10] = {0};

	tcapi_get("DhcpClient_Info", "Count", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getmDevNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{	
	uint8  attribute;
	int8  result;
	int ret;
	
	char nodeValue[10] = {0};

	ret = tcapi_get("TR069Attr_SpeAttr", "dhcpClientCount", nodeValue);
	// add default value
	if(0 != ret)
	{	
		strcpy(nodeValue, "0");
		tcapi_set("TR069Attr_SpeAttr", "dhcpClientCount", "0");
	}
	
	attribute = atoi(nodeValue);
	
	/*add for debug*/
	tcdbg_printf("\ngetmDevNumberOfEntriesAttribute is %d\n",(int)attribute);
	
	result=GetTheAttributes(attribute, notification, acl, deviceNode);
	
	return result;
}

int8   setmDevNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8  attribute;
	int8  result;
	
	char nodeValue[10] = {0};
	
	tcapi_get("TR069Attr_SpeAttr", "dhcpClientCount", nodeValue);
	
	attribute = atoi(nodeValue);
	
	result=SetTheAttributes(&attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	sprintf(nodeValue,"%d", attribute);
	tcapi_set("TR069Attr_SpeAttr", "dhcpClientCount", nodeValue);
	
	tcapi_set("TR069Attr_SpeAttr", "flag", "1");

	/*add for debug*/
	tcdbg_printf("\nIn setDevNumberOfEntriesAttribute,attribute is %d\n",attribute);
	
	return result;
}

int8 getmDevNotificationLimitValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	tcapi_get("DhcpClientLimit_Entry", "time", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8   setmDevNotificationLimitValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 time;
	char strTime[11] = {0};

	time = *((uint32*)value);
	sprintf(strTime, "%lu", time);
	tcapi_set("DhcpClientLimit_Entry", "time", strTime);

	SetCfgNodeFlag( "DhcpClientLimit", -1 );
	
	return 0;		
}

int8 getmDevNotificationLimitAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR111Attr.amDevNotificationLimit;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8   setmDevNotificationLimitAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcTR111Attr.amDevNotificationLimit;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getmDevOUIValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[7] = {0};
	char nodeStr[20] = {0};
	char attrStr[20] = {0};
	int index;

	index = atoi(deviceNode->parentNode->name);
	if(index < 1)
		return -1;
	sprintf(nodeStr, "DhcpClient_Info");
	sprintf(attrStr, "oui%d", index-1);
	tcapi_get(nodeStr, attrStr, tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getmDevOUIAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;

	result=0;
	return result;
}

int8 getmDevSNValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[65] = {0};
	char nodeStr[20] = {0};
	char attrStr[20] = {0};
	int index;

	index = atoi(deviceNode->parentNode->name);
	if(index < 1)
		return -1;
	sprintf(nodeStr, "DhcpClient_Info");
	sprintf(attrStr, "sn%d", index-1);
	tcapi_get(nodeStr, attrStr, tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getmDevSNAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;

	result=0;
	return result;
}

int8 getmDevClassValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[65] = {0};
	char nodeStr[20] = {0};
	char attrStr[20] = {0};
	int index;
	int ret;

	index = atoi(deviceNode->parentNode->name);
	if(index < 1)
		return -1;
	sprintf(nodeStr, "DhcpClient_Info");
	sprintf(attrStr, "pclass%d", index-1);
	tcapi_get(nodeStr, attrStr, tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getmDevClassAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;

	result=0;
	return result;
}

int8   RebuildmDevObject()
{
	uint32 number;
	DeviceNodePtr mDevNode = NULL;
	char *objectName = "InternetGatewayDevice.ManagementServer.ManageableDevice.";
	int rtnCode = 0;

	if( rpc.IGDDeviceRootPtr == NULL)
		return -1;
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &mDevNode);
	if(mDevNode == NULL){
		return -1;
	}
	
	cwmpFreeDeviceTable(mDevNode);
	rtnCode = cwmpInitManageableDeviceSubTree(mDevNode);
	return rtnCode;
}

#endif //endif TR111

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getBindInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  BindInterface[257] = {0};

	if ( tcapi_get("Cwmp_Entry", "BindInterface", BindInterface) < 0 )
		BindInterface[0] = '\0';

	return getTheValue(value, type, deviceNode, BindInterface);
}
int8 setBindInterfaceValue(void* value, struct DeviceNode*  deviceNode)
{
	char  BindInterface[264] = {0};

	snprintf(BindInterface, sizeof(BindInterface), "%s", (char*)(value));
	if ( strlen(BindInterface) > 256 )
		return -1;

	tcapi_set("Cwmp_Entry", "BindInterface", BindInterface);
	SetCfgNodeFlag( "Cwmp", -1 );
	return 0;
}
int8 getBindInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcCwmpAttr.aBindInterface;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setBindInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcCwmpAttr.aBindInterface;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= CWMP_ATTR_FLAG;
	return result;
}

int8 getRMSFloodingValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char RMSFlooding[10] = {0};

	if ( tcapi_get("Cwmp_Entry", "RandomInform", RMSFlooding) < 0 )
		strcpy(RMSFlooding, "0");

	return getTheValue(value, type, deviceNode, RMSFlooding);
}
int8 setRMSFloodingValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 RMSFlooding;
	char s_RMSFlooding[10] = {0};

	RMSFlooding = *((uint8*)value);

	if( 1 == RMSFlooding )
		strcpy(s_RMSFlooding, "1");
	else if( 0 == RMSFlooding )
		strcpy(s_RMSFlooding, "0");
	else
		return -1;

	tcapi_set("Cwmp_Entry", "RandomInform", s_RMSFlooding);
	SetCfgNodeFlag( "Cwmp", -1 );
	return 0;
}
int8 getRMSFloodingAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcCwmpAttr.aRMSFlooding;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setRMSFloodingAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcCwmpAttr.aRMSFlooding;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= CWMP_ATTR_FLAG;
	return result;
}

#endif

