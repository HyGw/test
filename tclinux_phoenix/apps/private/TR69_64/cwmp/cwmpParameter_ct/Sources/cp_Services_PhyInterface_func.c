#include "Global_res.h"
#include "cp_Services_PhyInterface.h"


int8 getInterfaceIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->name);	
	
	if(instance < 0 || instance > VOIPBASIC_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance-1); 
	tcapi_get(nodeName, "id", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);

}
int8 setInterfaceIDValue(void* value, struct DeviceNode*  deviceNode)
{
	uint iValue;
	int instance;
	char strValue[33] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->name);	
	if(instance < 0 || instance > VOIPBASIC_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPBasic_Entry%d",  instance-1); 

	iValue = *((uint*)value);
	if(iValue > 65535){
		return -1;
	}

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	sprintf(strValue, "%u", iValue);
	tcapi_set(nodeName, "id", strValue);

	return 0;
}
int8 getInterfaceIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aInterfaceID[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setInterfaceIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aInterfaceID[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getPhyPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{	
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->name);	

	if(instance < 0 || instance > VOIPBASIC_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance-1); 
	tcapi_get(nodeName, "PhyPort", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);

}
int8 setPhyPortValue(void* value, struct DeviceNode*  deviceNode)
{	
	uint iValue;
	int instance;
	char strValue[33] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->name);	
	if(instance < 0 || instance > VOIPBASIC_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPBasic_Entry%d",  instance-1); 

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
   tcapi_set(nodeName, "PhyPort", (char*)value);
#else

	iValue = *((uint*)value);
	if(iValue > 65535){
		return -1;
	}

	sprintf(strValue, "%u", iValue);
	
	tcapi_set(nodeName, "PhyPort", strValue);
#endif

	SetCfgNodeFlag( "VoIPBasic", -1 );

	return 0;
}

int8 getPhyPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aPhyPort[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPhyPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aPhyPort[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getTestStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	int instance;
	char nodeName[30] = {0};
	
	instance = atoi(deviceNode->parentNode->parentNode->name);
	
	if(instance < 0 || instance > VOIPTEST_MAXENTRY)
		return -1;
	sprintf(nodeName, "VoIPTest_Entry%d", instance-1); 
	tcapi_get(nodeName, "TestState", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setTestStateValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance;
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->name);
	
	if(instance < 0 || instance > VOIPTEST_MAXENTRY)
		return -1;
	sprintf(nodeName, "VoIPTest_Entry%d", instance-1); 
	tcapi_set(nodeName, "TestState", (char*)value);

	SetCfgNodeFlag( "VoIPTest", -1 );
	
	return 0;
}
int8 getTestStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	attribute = tcTR104Attr.aTestState;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setTestStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	attribute = &tcTR104Attr.aTestState;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getTestSelectorValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[32] = {0};
	int instance;
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->name);
	
	if(instance < 0 || instance > VOIPTEST_MAXENTRY)
		return -1;
	sprintf(nodeName, "VoIPTest_Entry%d", instance-1); 
	tcapi_get(nodeName, "TestSelector", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setTestSelectorValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance;
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->name);
	
	if(instance < 0 || instance > VOIPTEST_MAXENTRY)
		return -1;
	sprintf(nodeName, "VoIPTest_Entry%d", instance-1); 
	tcapi_set(nodeName, "TestSelector", (char*)value);

	SetCfgNodeFlag( "VoIPTest", -1 );
	
	return 0;
}
int8 getTestSelectorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aTestSelector;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setTestSelectorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aTestSelector;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getPhoneConnectivityValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	int instance;
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->name);
	
	if(instance < 0 || instance > VOIPTEST_MAXENTRY)
		return -1;
	sprintf(nodeName, "VoIPTest_Entry%d", instance-1); 
	
	tcapi_get(nodeName, "PhoneConnectivity", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getPhoneConnectivityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aPhoneConnectivity;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPhoneConnectivityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aPhoneConnectivity;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

#if defined(TCSUPPORT_CT)
int8 getTestTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	
	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPSimulateTest_Entry%d", instance-1); 
	tcapi_get(nodeName, "TestType", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setTestTypeValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance;
	char nodeName[30] = {0};
	char *pTestType = NULL;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;

	pTestType = (char*)value;
	if (strcmp(pTestType, "Caller") && strcmp(pTestType, "Called") 
	    && strcmp(pTestType, "None")){
		return -1;
	}
    
	sprintf(nodeName, "VoIPSimulateTest_Entry%d",  instance-1); 
	
	tcapi_set(nodeName, "TestType", pTestType);	

	SetCfgNodeFlag( "VoIPSimulateTest", -1 );
	
	return 0;
}
int8 getTestTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aTestType;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setTestTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aTestType;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getCalledNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	

	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPSimulateTest_Entry%d", instance-1); 
	tcapi_get(nodeName, "CalledNumber", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setCalledNumberValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance;
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPSimulateTest_Entry%d",  instance-1); 
	
	tcapi_set(nodeName, "CalledNumber", (char*)value);	

	SetCfgNodeFlag( "VoIPSimulateTest", -1 );
	
	return 0;
}
int8 getCalledNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aCalledNumber;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCalledNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aCalledNumber;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getCallHoldTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	

	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPSimulateTest_Entry%d", instance-1); 
	tcapi_get(nodeName, "CallHoldTimer", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setCallHoldTimerValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance;
	uint iValue;
	char nodeName[30] = {0};
	char strValue[33] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPSimulateTest_Entry%d",  instance-1); 
	
	iValue = *((uint*)value);
	
	SetCfgNodeFlag( "VoIPSimulateTest", -1 );
	sprintf(strValue, "%u", iValue);
	tcapi_set(nodeName, "CallHoldTimer", strValue);	
	
	return 0;
}

int8 getCallHoldTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aCallHoldTimer;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCallHoldTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aCallHoldTimer;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getCalledWaitTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	

	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPSimulateTest_Entry%d", instance-1); 
	tcapi_get(nodeName, "CalledWaitTimer", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setCalledWaitTimerValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance;
	uint iValue;
	char nodeName[30] = {0};
	char strValue[33] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPSimulateTest_Entry%d",  instance-1); 
	
	iValue = *((uint*)value);
	
	SetCfgNodeFlag( "VoIPSimulateTest", -1 );
	sprintf(strValue, "%u", iValue);
	tcapi_set(nodeName, "CalledWaitTimer", strValue);	
	
	return 0;
}

int8 getCalledWaitTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aCalledWaitTimer;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCalledWaitTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aCalledWaitTimer;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getDailDTMFConfirmEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};
	char result[5] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	

	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPSimulateTest_Entry%d", instance-1); 
	tcapi_get(nodeName, "DailDTMFConfirmEnable", tempValue);
	
	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");

	return getTheValue(value, type, deviceNode, result);
}
int8 setDailDTMFConfirmEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 iEnable;
	int instance;
	char strEnable[10] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPSimulateTest_Entry%d",  instance-1); 

	iEnable = *((uint8*)value);

	SetCfgNodeFlag( "VoIPSimulateTest", -1 );
	
	if(0 == iEnable)
		strcpy(strEnable, "No");
	else if(1 == iEnable)
		strcpy(strEnable, "Yes");
	else 
		return -1;
	
	tcapi_set(nodeName, "DailDTMFConfirmEnable", strEnable);

	return 0;
}
int8 getDailDTMFConfirmEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aConfirmEnable;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDailDTMFConfirmEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aConfirmEnable;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getDailDTMFConfirmNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	
	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPSimulateTest_Entry%d", instance-1); 
	tcapi_get(nodeName, "DailDTMFConfirmNumber", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setDailDTMFConfirmNumberValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance;
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPSimulateTest_Entry%d",  instance-1); 
	
	tcapi_set(nodeName, "DailDTMFConfirmNumber", (char*)value);	

	SetCfgNodeFlag( "VoIPSimulateTest", -1 );
	
	return 0;
}
int8 getDailDTMFConfirmNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aConfirmNumber;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDailDTMFConfirmNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aConfirmNumber;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getDailDTMFConfirmResultValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	
	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPSimulateTest_Entry%d", instance-1); 
	tcapi_get(nodeName, "DailDTMFConfirmResult", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getDailDTMFConfirmResultAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aConfirmResult;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDailDTMFConfirmResultAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aConfirmResult;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	
	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPSimulateTest_Entry%d", instance-1); 
	tcapi_get(nodeName, "Status", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aStatus;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aStatus;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getConclusionValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	
	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPSimulateTest_Entry%d", instance-1); 
	tcapi_get(nodeName, "Conclusion", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getConclusionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aConclusion;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setConclusionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aConclusion;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getCallerFailReasonValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	
	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPSimulateTest_Entry%d", instance-1); 
	tcapi_get(nodeName, "CallerFailReason", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getCallerFailReasonAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aCallerFailReason;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCallerFailReasonAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aCallerFailReason;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getCalledFailReasonValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	
	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPSimulateTest_Entry%d", instance-1); 
	tcapi_get(nodeName, "CalledFailReason", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getCalledFailReasonAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aCalledFailReason;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCalledFailReasonAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aCalledFailReason;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getFailedResponseCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	
	if(instance < 0 || instance > VOIPSIMULATETEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPSimulateTest_Entry%d", instance-1); 
	tcapi_get(nodeName, "FailedResponseCode", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getFailedResponseCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aFailedResponseCode;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setFailedResponseCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aFailedResponseCode;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}
#endif


static int _poorQlComGetValDeal(DeviceNode*  deviceNode, char *nodeName, int len)
{
    int instance = 0;
    char nodeNameTmp[30] = {0};
    int  pvcNum = 0;
	int voicepoorqlnum = 0;
	int num = 0;
	
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
		
		if(getSipStatus_tr69() == 0){	 /*SIP IMSSIP*/
	
			voicepoorqlnum = VOICEPOORQLNUM_SIP;
			
		}
		else if(getSipStatus_tr69() == 1){	 /*H.248*/
	
			voicepoorqlnum = VOICEPOORQLNUM_NOSIP;
		}
#else
		voicepoorqlnum = VOICEPOORQLNUM_NOSIP;
#endif

    if (NULL == nodeName || len == 0 || NULL == deviceNode){
        tcdbg_printf("invalid param, nodeName is NULL, or len is zero\n");
        return -1;
    }
    pvcNum = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
    instance = atoi(deviceNode->parentNode->name)-1;
    if (pvcNum < 0 || pvcNum > (LINENUM-1)
        || instance < 0 || instance > (voicepoorqlnum-1)){
        tcdbg_printf("===pvc[%d] or instance[%d] is invalid\n", pvcNum, instance);
        return -1;
    }
    sprintf(nodeNameTmp, "InfoVoIPPoorQL_PVC%d_Entry%d", pvcNum, instance); 

    strncpy(nodeName, nodeNameTmp, len);

    return 0;
}

int8 getStatTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
    int ret = 0;
    char tempValue[20] = {0};
    char nodeName[30] = {0}; 
    
    ret = _poorQlComGetValDeal(deviceNode, nodeName, sizeof(nodeName)-1);
    if (ret == -1){
        return ret;
    }
    
    tcapi_get(nodeName, "StatTime", tempValue);
    
    return getTheValue(value, type, deviceNode, tempValue);

}
int8 getStatTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aStatTime[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setStatTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aStatTime[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getTxPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
    int ret = 0;
    char tempValue[20] = {0};
    char nodeName[30] = {0}; 
    
    ret = _poorQlComGetValDeal(deviceNode, nodeName, sizeof(nodeName)-1);
    if (ret == -1){
        return ret;
    }
    tcapi_get(nodeName, "TxPackets", tempValue);

    return getTheValue(value, type, deviceNode, tempValue);
}
int8 getTxPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aTxPackets[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setTxPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aTxPackets[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getRxPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
    int ret = 0;
    char tempValue[20] = {0};
    char nodeName[30] = {0}; 
    
    ret = _poorQlComGetValDeal(deviceNode, nodeName, sizeof(nodeName)-1);
    if (ret == -1){
        return ret;
    } 
    tcapi_get(nodeName, "RxPackets", tempValue);

    return getTheValue(value, type, deviceNode, tempValue);

}
int8 getRxPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aRxPackets[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setRxPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aRxPackets[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getMeanDelayValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
    int ret = 0;
    char tempValue[20] = {0};
    char nodeName[30] = {0}; 
    
    ret = _poorQlComGetValDeal(deviceNode, nodeName, sizeof(nodeName)-1);
    if (ret == -1){
        return ret;
    } 
    tcapi_get(nodeName, "MeanDelay", tempValue);

    return getTheValue(value, type, deviceNode, tempValue);

}
int8 getMeanDelayAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aMeanDelay[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMeanDelayAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aMeanDelay[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getMeanJitterValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
    int ret = 0;
    char tempValue[20] = {0};
    char nodeName[30] = {0}; 
    
    ret = _poorQlComGetValDeal(deviceNode, nodeName, sizeof(nodeName)-1);
    if (ret == -1){
        return ret;
    } 
    tcapi_get(nodeName, "MeanJitter", tempValue);

    return getTheValue(value, type, deviceNode, tempValue);

}
int8 getMeanJitterAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aMeanJitter[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMeanJitterAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aMeanJitter[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getFractionLossValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
    int ret = 0;
    char tempValue[20] = {0};
    char nodeName[30] = {0}; 
    
    ret = _poorQlComGetValDeal(deviceNode, nodeName, sizeof(nodeName)-1);
    if (ret == -1){
        return ret;
    } 
    tcapi_get(nodeName, "FractionLoss", tempValue);

    return getTheValue(value, type, deviceNode, tempValue);

}
int8 getFractionLossAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aFractionLoss[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setFractionLossAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aFractionLoss[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getLocalIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
    int ret = 0;
    char tempValue[20] = {0};
    char nodeName[30] = {0}; 
    
    ret = _poorQlComGetValDeal(deviceNode, nodeName, sizeof(nodeName)-1);
    if (ret == -1){
        return ret;
    }
    tcapi_get(nodeName, "LocalIPAddress", tempValue);
	
    return getTheValue(value, type, deviceNode, tempValue);
}
int8 getLocalIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aLocalIPAddress[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLocalIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aLocalIPAddress[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getLocalUDPPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
    int ret = 0;
    char tempValue[20] = {0};
    char nodeName[30] = {0}; 
    
    ret = _poorQlComGetValDeal(deviceNode, nodeName, sizeof(nodeName)-1);
    if (ret == -1){
        return ret;
    }
    tcapi_get(nodeName, "LocalUDPPort", tempValue);

    return getTheValue(value, type, deviceNode, tempValue);

}
int8 getLocalUDPPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aLocalUDPPort[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLocalUDPPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aLocalUDPPort[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getFarEndIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
    int ret = 0;
    char tempValue[20] = {0};
    char nodeName[30] = {0}; 
    
    ret = _poorQlComGetValDeal(deviceNode, nodeName, sizeof(nodeName)-1);
    if (ret == -1){
        return ret;
    }
    tcapi_get(nodeName, "FarEndIPAddress", tempValue);
	
    return getTheValue(value, type, deviceNode, tempValue);

}
int8 getFarEndIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aFarEndIPAddress[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setFarEndIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aFarEndIPAddress[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getFarEndUDPPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
    int ret = 0;
    char tempValue[20] = {0};
    char nodeName[30] = {0}; 
    
    ret = _poorQlComGetValDeal(deviceNode, nodeName, sizeof(nodeName)-1);
    if (ret == -1){
        return ret;
    }
    tcapi_get(nodeName, "FarEndUDPPort", tempValue);

    return getTheValue(value, type, deviceNode, tempValue);

}
int8 getFarEndUDPPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aFarEndUDPPort[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setFarEndUDPPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aFarEndUDPPort[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getMosLqValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
    int ret = 0;
    char tempValue[20] = {0};
    char nodeName[30] = {0}; 
    
    ret = _poorQlComGetValDeal(deviceNode, nodeName, sizeof(nodeName)-1);
    if (ret == -1){
        return ret;
    }
    tcapi_get(nodeName, "MosLq", tempValue);

    return getTheValue(value, type, deviceNode, tempValue);

}
int8 getMosLqAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aMosLq[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMosLqAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aMosLq[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getListCodecValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
    int ret = 0;
    char tempValue[20] = {0};
    char nodeName[30] = {0}; 
    
    ret = _poorQlComGetValDeal(deviceNode, nodeName, sizeof(nodeName)-1);
    if (ret == -1){
        return ret;
    }
    tcapi_get(nodeName, "Codec", tempValue);

    return getTheValue(value, type, deviceNode, tempValue);

}
int8 getListCodecAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aListCodec[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setListCodecAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aListCodec[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getIsTestOnBusyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};
	char result[5] = {0};

	instance = atoi(deviceNode->parentNode->name);	

	if(instance < 0 || instance > VOIPTEST_MAXENTRY)
		return -1;
	sprintf(nodeName, "VoIPTest_Entry%d", instance-1); 
	tcapi_get(nodeName, "IsTestOnBusy", tempValue);

	if(strcmp(tempValue, "1") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	
	return getTheValue(value, type, deviceNode, result);
}
int8 setIsTestOnBusyValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 iValue;
	int instance;
	char strValue[10] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->name);	
	if(instance < 0 || instance > VOIPTEST_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "VoIPTest_Entry%d",  instance-1); 

	iValue= *((uint8*)value);

	SetCfgNodeFlag( "VoIPTest", -1 );
	
	if(0 == iValue)
		strcpy(strValue, "0");
	else if(1 == iValue)
		strcpy(strValue, "1");
	else 
		return -1;
	
	tcapi_set(nodeName, "IsTestOnBusy", strValue);

	return 0;
}

int8 getIsTestOnBusyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aIsTestOnBusy;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIsTestOnBusyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aIsTestOnBusy;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getLineTestConclusionValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	

	if(instance < 0 || instance > INFOVOIP_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance-1); 
	tcapi_get(nodeName, "Conclusion", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getLineTestConclusionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aLineTestConclusion;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLineTestConclusionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aLineTestConclusion;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getAGACVoltageValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	

	if(instance < 0 || instance > INFOVOIP_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance-1); 
	tcapi_get(nodeName, "AGACVoltage", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getAGACVoltageAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aAGACVoltage;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setAGACVoltageAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aAGACVoltage;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getBGACVoltageValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	

	if(instance < 0 || instance > INFOVOIP_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance-1); 
	tcapi_get(nodeName, "BGACVoltage", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getBGACVoltageAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aBGACVoltage;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setBGACVoltageAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aBGACVoltage;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getABACVoltageValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	

	if(instance < 0 || instance > INFOVOIP_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance-1); 
	tcapi_get(nodeName, "ABACVoltage", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getABACVoltageAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aABACVoltage;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setABACVoltageAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aABACVoltage;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getAGDCVoltageValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	

	if(instance < 0 || instance > INFOVOIP_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance-1); 
	tcapi_get(nodeName, "AGDCVoltage", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getAGDCVoltageAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aAGDCVoltage;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setAGDCVoltageAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aAGDCVoltage;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getBGDCVoltageValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	

	if(instance < 0 || instance > INFOVOIP_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance-1); 
	tcapi_get(nodeName, "BGDCVoltage", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getBGDCVoltageAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aBGDCVoltage;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setBGDCVoltageAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aBGDCVoltage;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getABDCVoltageValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	

	if(instance < 0 || instance > INFOVOIP_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance-1); 
	tcapi_get(nodeName, "ABDCVoltage", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getABDCVoltageAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aABDCVoltage;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setABDCVoltageAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aABDCVoltage;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getAGInsulationResistanceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	

	if(instance < 0 || instance > INFOVOIP_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance-1); 
	tcapi_get(nodeName, "AGInsulationResistance", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getAGInsulationResistanceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aAGInsulationResistance;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setAGInsulationResistanceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aAGInsulationResistance;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getBGInsulationResistanceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	

	if(instance < 0 || instance > INFOVOIP_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance-1); 
	tcapi_get(nodeName, "BGInsulationResistance", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getBGInsulationResistanceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aBGInsulationResistance;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setBGInsulationResistanceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aBGInsulationResistance;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getABInsulationResistanceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	

	if(instance < 0 || instance > INFOVOIP_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance-1); 
	tcapi_get(nodeName, "ABInsulationResistance", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getABInsulationResistanceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aABInsulationResistance;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setABInsulationResistanceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aABInsulationResistance;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getAGCapacitanceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	

	if(instance < 0 || instance > INFOVOIP_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance-1); 
	tcapi_get(nodeName, "AGCapacitance", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getAGCapacitanceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aAGCapacitance;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setAGCapacitanceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aAGCapacitance;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getBGCapacitanceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	

	if(instance < 0 || instance > INFOVOIP_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance-1); 
	tcapi_get(nodeName, "BGCapacitance", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getBGCapacitanceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aBGCapacitance;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setBGCapacitanceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aBGCapacitance;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getABCapacitanceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	

	if(instance < 0 || instance > INFOVOIP_ENTRYNUMBER)
		return -1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance-1); 
	tcapi_get(nodeName, "ABCapacitance", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getABCapacitanceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aABCapacitance;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setABCapacitanceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aABCapacitance;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getDialModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	sprintf(tempValue, "%s", "First");

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setDialModeValue(void* value, struct DeviceNode*  deviceNode)
{
	tcapi_set("VoIPAdvanced_Common", "DialMode", (char*)value);

	SetCfgNodeFlag( "VoIPAdvanced", -1 );
	
	return 0;
}
int8 getDialModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	attribute = tcTR104Attr.aDialMode;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDialModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	attribute = &tcTR104Attr.aDialMode;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getHookFlashUpTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	sprintf(tempValue, "%s", "300");

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setHookFlashUpTimeValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iValue;
	char strValue[10] = {0};
	
	iValue = *((uint32*)value);

	SetCfgNodeFlag( "VoIPAdvanced", -1 );
	
	if(iValue > 1400){
		return -1;
	}
	sprintf(strValue, "%lu", iValue);
	
	tcapi_set("VoIPAdvanced_Common", "HookFlashUpTime", strValue);

	return 0;
}
int8 getHookFlashUpTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	attribute = tcTR104Attr.aHookFlashUpTime;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setHookFlashUpTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	attribute = &tcTR104Attr.aHookFlashUpTime;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}
int8 getHookFlashDownTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	sprintf(tempValue, "%s", "100");

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setHookFlashDownTimeValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iValue;
	char strValue[10] = {0};
	
	iValue = *((uint32*)value);

	SetCfgNodeFlag( "VoIPAdvanced", -1 );
	
	if(iValue > 1400){
		return -1;
	}
	sprintf(strValue, "%lu", iValue);
	
	tcapi_set("VoIPAdvanced_Common", "HookFlashDownTime", strValue);

	return 0;
}
int8 getHookFlashDownTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	attribute = tcTR104Attr.aHookFlashDownTime;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setHookFlashDownTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	attribute = &tcTR104Attr.aHookFlashDownTime;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getReversePolePulseValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[5] = {0};
	
	tcapi_get("VoIPAdvanced_Common", "ReversePolePulse", tempValue);

	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	
	return getTheValue(value, type, deviceNode, result);
}	

int8 setReversePolePulseValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 iEnable;
	char strEnable[10] = {0};

	iEnable = *((uint8*)value);

	SetCfgNodeFlag( "VoIPAdvanced", -1 );
	
	if(0 == iEnable)
		strcpy(strEnable, "No");
	else if(1 == iEnable)
		strcpy(strEnable, "Yes");
	else 
		return -1;
	tcapi_set("VoIPAdvanced_Common", "ReversePolePulse", strEnable);

	return 0;
}		

int8 getReversePolePulseAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aReversePolePulse;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setReversePolePulseAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aReversePolePulse;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getKcTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	sprintf(tempValue, "%s", "16Kc");

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setKcTypeValue(void* value, struct DeviceNode*  deviceNode)
{
	tcapi_set("VoIPAdvanced_Common", "KcType", (char*)value);

	SetCfgNodeFlag( "VoIPAdvanced", -1 );
	
	return 0;
}
int8 getKcTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	attribute = tcTR104Attr.aKcType;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setKcTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	attribute = &tcTR104Attr.aKcType;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getClipTransWhenValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	sprintf(tempValue, "%s", "AfterRing");

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setClipTransWhenValue(void* value, struct DeviceNode*  deviceNode)
{
	tcapi_set("VoIPAdvanced_Common", "ClipTransWhen", (char*)value);

	SetCfgNodeFlag( "VoIPAdvanced", -1 );
	
	return 0;
}
int8 getClipTransWhenAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	attribute = tcTR104Attr.aClipTransWhen;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setClipTransWhenAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	attribute = &tcTR104Attr.aClipTransWhen;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getClipFormatValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};

	if(tcapi_get("VoIPAdvanced_Common", "ClipFormat", tempValue)<0)
	     sprintf(tempValue, "%s", "Mdmf-fsk");

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setClipFormatValue(void* value, struct DeviceNode*  deviceNode)
{
    if(strcasecmp((char*)value, "Sdmf-fsk")
		&& strcasecmp((char*)value, "Mdmf-fsk")
		&& strcasecmp((char*)value, "Dtmf")
	)
		return -1;
	
	tcapi_set("VoIPAdvanced_Common", "ClipFormat", (char*)value);

	SetCfgNodeFlag( "VoIPAdvanced", -1 );
	
	return 0;
}
int8 getClipFormatAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	attribute = tcTR104Attr.aClipFormat;
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setClipFormatAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	attribute = &tcTR104Attr.aClipFormat;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getClipSendDateTimeEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[5] = {0};
	
	tcapi_get("VoIPAdvanced_Common", "ClipSendDateTimeEnable", tempValue);

	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");

	return getTheValue(value, type, deviceNode, result);
}	

int8 setClipSendDateTimeEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 iEnable;
	char strEnable[10] = {0};

	iEnable = *((uint8*)value);

	SetCfgNodeFlag( "VoIPAdvanced", -1 );
	
	if(0 == iEnable)
		strcpy(strEnable, "No");
	else if(1 == iEnable)
		strcpy(strEnable, "Yes");
	else 
		return -1;
	
	tcapi_set("VoIPAdvanced_Common", "ClipSendDateTimeEnable", strEnable);

	return 0;
}		

int8 getClipSendDateTimeEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aClipSendDateTimeEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setClipSendDateTimeEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aClipSendDateTimeEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getClipSendDateTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};

	if(tcapi_set("VoIPAdvanced_Common", "ClipSendDateTime", tempValue)<0)
	    sprintf(tempValue, "%s", "2");

	return getTheValue(value, type, deviceNode, tempValue);

}
int8 setClipSendDateTimeValue(void* value, struct DeviceNode*  deviceNode)
{
	  uint32 iValue;
		char strValue[10] = {0};
		
		iValue = *((uint32*)value);
	
		SetCfgNodeFlag( "VoIPAdvanced", -1 );
		
		if(iValue > 1400){
			return -1;
		}
		sprintf(strValue, "%lu", iValue);
		
		tcapi_set("VoIPAdvanced_Common", "ClipSendDateTime", strValue);
	
		return 0;

}
int8 getClipSendDateTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
		int8  result;
	
		attribute = tcTR104Attr.aClipSendDateTime;
		result=GetTheAttributes(attribute,notification, acl, deviceNode);
		return result;

}
int8 setClipSendDateTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	   uint8*	attribute = NULL;
		int8  result;
	
		attribute = &tcTR104Attr.aClipSendDateTime;
		result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
		return result;

}


int8 getRingDCVoltageOverlappedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};

	if(tcapi_set("VoIPAdvanced_Common", "RingDCVoltageOverlapped", tempValue)<0)
	   sprintf(tempValue, "%s", "0");
	return getTheValue(value, type, deviceNode, tempValue);

}
int8 setRingDCVoltageOverlappedValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 iValue;
	char strValue[10] = {0};
			
    iValue = *((uint32*)value);
		
	SetCfgNodeFlag( "VoIPAdvanced", -1 );
			
	if(iValue > 25){
		return -1;
	}
	sprintf(strValue, "%lu", iValue);
			
	tcapi_set("VoIPAdvanced_Common", "RingDCVoltageOverlapped", strValue);
		
	return 0;
}

int8 getRingDCVoltageOverlappedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
		
	attribute = tcTR104Attr.aRingDCVoltageOverlapped;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}

int8 setRingDCVoltageOverlappedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*	attribute = NULL;
	int8  result;
		
	attribute = &tcTR104Attr.aRingDCVoltageOverlapped;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}

int8 getDataServiceModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	if(tcapi_set("VoIPAdvanced_Common", "DataServiceMode", tempValue)<0)
	   sprintf(tempValue, "%s", "0");
	return getTheValue(value, type, deviceNode, tempValue);

}
int8 setDataServiceModeValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 iValue;
	char strValue[10] = {0};
				
	iValue = *((uint32*)value);

	if(iValue != 0 && iValue != 1 && iValue != 2)
		return -1;
			
	sprintf(strValue, "%lu", iValue);
				
	tcapi_set("VoIPAdvanced_Common", "DataServiceMode", strValue);
			
	SetCfgNodeFlag( "VoIPAdvanced", -1 );
	return 0;

}
int8 getDataServiceModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
			
	attribute = tcTR104Attr.aDataServiceMode;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}
int8 setDataServiceModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*	attribute = NULL;
	int8  result;
			
	attribute = &tcTR104Attr.aDataServiceMode;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}


int8 getClipReversePoleValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[5] = {0};
	
	tcapi_get("VoIPAdvanced_Common", "ClipReversePole", tempValue);

	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");

	return getTheValue(value, type, deviceNode, result);
}	

int8 setClipReversePoleValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 iEnable;
	char strEnable[10] = {0};

	iEnable = *((uint8*)value);

	SetCfgNodeFlag( "VoIPAdvanced", -1 );
	
	if(0 == iEnable)
		strcpy(strEnable, "No");
	else if(1 == iEnable)
		strcpy(strEnable, "Yes");
	else 
		return -1;
	
	tcapi_set("VoIPAdvanced_Common", "ClipReversePole", strEnable);

	return 0;
}		

int8 getClipReversePoleAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aClipReversePole;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setClipReversePoleAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aClipReversePole;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getMWIRingFlagValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[5] = {0};
	
	tcapi_get("VoIPAdvanced_Common", "MWIRingFlag", tempValue);

	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");

	return getTheValue(value, type, deviceNode, result);
}	

int8 setMWIRingFlagValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 iEnable;
	char strEnable[10] = {0};

	iEnable = *((uint8*)value);

	SetCfgNodeFlag( "VoIPAdvanced", -1 );
	
	if(0 == iEnable)
		strcpy(strEnable, "No");
	else if(1 == iEnable)
		strcpy(strEnable, "Yes");
	else 
		return -1;
	
	tcapi_set("VoIPAdvanced_Common", "MWIRingFlag", strEnable);

	return 0;
}		

int8 getMWIRingFlagAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aMWIRingFlag;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setMWIRingFlagAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aMWIRingFlag;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int cwmpInitPoorQualityListSubTree( DeviceNodePtr deviceNode )
{
	int rtnCode = 0;
	int num = 0;
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)

	if(getSipStatus_tr69() == 0){	 /*SIP IMSSIP*/
		num = VOICEPOORQLNUM_SIP;
	}
	else{  /*H.248*/
		num = PHYINTERFACENUM;
	}
#else
    num = PHYINTERFACENUM;
#endif
rtnCode = RegisterDynNodeFromIndex( deviceNode, PoorQualityListIndex, num );

	return rtnCode;
}
