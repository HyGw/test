#include "Global_res.h"
#include "cp_Services_voiceProf.h"

#if defined(TCSUPPORT_CT)
/***********Support IAD, Add 20140418************************************/
pthread_t p_Cwmp_IAD = 0;
static IADDiagnose_t gCwmpIAD = {"None",};
#endif


int8 setVoIPProtocolToIMSSIP(void)
{
#if defined(TCSUPPORT_CT_PON_CZ_NX) || defined(TCSUPPORT_CT_PON_CZ_GZ)
	tcapi_set(VOIPBASIC_COMMON, "SIPProtocol", "IMSSIP");
	SetCfgNodeFlag( "VoIPBasic", -1 );
#endif

	return 0;
}
int8 setVoIPProtocolToH248(void)
{
#if defined(TCSUPPORT_CT_PON_NX) || defined(TCSUPPORT_CT_PON_SN) || defined(TCSUPPORT_CT_PON_CZ_NX) || defined(TCSUPPORT_CT_PON_CZ_GZ)
	tcapi_set(VOIPBASIC_COMMON, "SIPProtocol", "H.248");
	SetCfgNodeFlag( "VoIPBasic", -1 );
#endif

	return 0;
}



#if defined(TCSUPPORT_CUC_VOIP_NODE)
int8 getLocalPortMinValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	tcapi_get(nodeName, "LocalPortMin", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setLocalPortMinValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iPort;
	char strPort[10] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	iPort = *((uint32*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(iPort > 65535){
		return -1;
	}
	sprintf(strPort, "%lu", iPort);
	
	tcapi_set(nodeName, "LocalPortMin", strPort);

	
	return 0;
}		
int8 getLocalPortMinAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aLocalPortMin[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLocalPortMinAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aLocalPortMin[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getLocalPortMaxValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	tcapi_get(nodeName, "LocalPortMax", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setLocalPortMaxValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iPort;
	char strPort[10] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	iPort = *((uint32*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(iPort > 65535){
		return -1;
	}
	sprintf(strPort, "%lu", iPort);
	
	tcapi_set(nodeName, "LocalPortMax", strPort);

	
	return 0;
}		
int8 getLocalPortMaxAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aLocalPortMax[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLocalPortMaxAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aLocalPortMax[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getTelephoneEventPayloadTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	tcapi_get(nodeName, "TelephoneEventPayloadType", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setTelephoneEventPayloadTypeValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 val;
	char str[10] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	val = *((uint32*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(val <96 ||val > 127){
		return -1;
	}
	sprintf(str, "%lu", val);
	
	tcapi_set(nodeName, "TelephoneEventPayloadType", str);

	
	return 0;
}		
int8 getTelephoneEventPayloadTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aTelephoneEventPayloadType[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTelephoneEventPayloadTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aTelephoneEventPayloadType[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getRTPX_CU_WANNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	tcapi_get(nodeName, "WANName", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}		
		
int8 getRTPX_CU_WANNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aWANName[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setRTPX_CU_WANNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aWANName[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getRtcpEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	tcapi_get(nodeName, "RtcpEnable", tempValue);

	if(!strcmp(tempValue, "Yes")){
		strcpy(tempValue, "1");
	}
	else if(!strcmp(tempValue, "No")){
		strcpy(tempValue, "0");
	}
	else{
		strcpy(tempValue, " ");
	}
	
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setRtcpEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 val;
	char str[10] = {'\0'};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	val = *((uint8*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );

	if(0 == val)
	   strcpy(str, "No");
	else if(1 == val)
	   strcpy(str, "Yes");

	tcapi_set(nodeName, "RtcpEnable", str);
	
	tc_printf("\n==>nodename:%s %s,setval:%s\n",nodeName,"RtcpEnable",str);
	return 0;
}		
int8 getRtcpEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aRtcpEnable[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setRtcpEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aRtcpEnable[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getTxRepeatIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	tcapi_get(nodeName, "TxRepeatInterval", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setTxRepeatIntervalValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 val;
	char str[10] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	val = *((uint32*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(val <1 ||val > 65535){
		return -1;
	}
	sprintf(str, "%lu", val);
	
	tcapi_set(nodeName, "TxRepeatInterval", str);

	
	return 0;
}		
int8 getTxRepeatIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aTelephoneEventPayloadType[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTxRepeatIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aTelephoneEventPayloadType[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	


int8 getX_CU_RtcpxrFlagValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {'\0'};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	tcapi_get(nodeName, "RtcpxrFlag", tempValue);

	if(!strcmp(tempValue, "Yes")){
		strcpy(tempValue, "1");
	}
	else if(!strcmp(tempValue, "No")){
		strcpy(tempValue, "0");
	}
	else{
		strcpy(tempValue, " ");
	}
	
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setX_CU_RtcpxrFlagValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 val;
	char str[10] = {'\0'};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	val = *((uint8*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );

	if(1 == val)
	   strcpy(str, "Yes");
	else if(0 == val)
	   strcpy(str, "No");

	tcapi_set(nodeName, "RtcpxrFlag", str);
	
	tc_printf("\n==>nodename:%s %s,setval:%s\n",nodeName,"RtcpxrFlag",str);
	return 0;
}		
int8 getX_CU_RtcpxrFlagAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aX_CU_RtcpxrFlag[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setX_CU_RtcpxrFlagAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aX_CU_RtcpxrFlag[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getRdnEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {'\0'};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	tcapi_get(nodeName, "RdnEnable", tempValue);
	
	if(!strcmp(tempValue, "Yes")){
		strcpy(tempValue, "1");
	}
	else if(!strcmp(tempValue, "No")){
		strcpy(tempValue, "0");
	}
	else{
		strcpy(tempValue, " ");
	}
	
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setRdnEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 val;
	char str[10] = {'\0'};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	val = *((uint8*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );

	if(1 == val)
	   strcpy(str, "Yes");
	else if(0 == val)
	   strcpy(str, "No");
	
	tcapi_set(nodeName, "RdnEnable", str);

	
	return 0;
}		
int8 getRdnEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aRdnEnable[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setRdnEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aRdnEnable[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getPayloadTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	tcapi_get(nodeName, "PayloadType", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setPayloadTypeValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 val;
	char str[10] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	val = *((uint32*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(val <0 ||val > 127){
		return -1;
	}
	sprintf(str, "%lu", val);
	
	tcapi_set(nodeName, "PayloadType", str);

	
	return 0;
}		
int8 getPayloadTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aPayloadType[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setPayloadTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aPayloadType[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getX_CU_EnableFixedStartValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {'\0'};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	tcapi_get(nodeName, "EnableFixedStart", tempValue);

	if(!strcmp(tempValue, "Yes")){
		strcpy(tempValue, "1");
	}
	else if(!strcmp(tempValue, "No")){
		strcpy(tempValue, "0");
	}
	else{
		strcpy(tempValue, " ");
	}
	
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setX_CU_EnableFixedStartValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 val;
	char str[10] = {'\0'};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	val = *((uint8*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );

	if(1 == val)
	   strcpy(str, "Yes");
	else if(0 == val)
	   strcpy(str, "No");
	
	tcapi_set(nodeName, "EnableFixedStart", str);

	
	return 0;
}		
int8 getX_CU_EnableFixedStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aEnableFixedStart[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setX_CU_EnableFixedStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aEnableFixedStart[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}
#endif

int8 isImssipEnable(void)
{
	int ret=0;
	char tempValue[16] = {0};
	tcapi_get("VoIPBasic_Common", "SIPProtocol", tempValue);
	if(strcmp(tempValue,"IMSSIP")==0)
		ret=1;
	return ret;

}

#if defined(TCSUPPORT_CT_PON_SC)
int8 getCallIdTimePolicyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[32] = {0};
	
	tcapi_set("VoIPCallCtrl_Entry0", "SyncCallerTimeEnable", "tempValue");
	
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setCallIdTimePolicyValue(void* value, struct DeviceNode*  deviceNode)
{

	char val;
	val = *((uint8*)value);

	setVoIPProtocolToIMSSIP();
	if (val == 0)
		tcapi_set("VoIPCallCtrl_Entry0", "SyncCallerTimeEnable", "0");
	else
		tcapi_set("VoIPCallCtrl_Entry0", "SyncCallerTimeEnable", "1");

	SetCfgNodeFlag("VoIPCallCtrl", -1);

	return 0;
}	
#endif


int8 getSIPProxyServerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	tcapi_get("VoIPBasic_Common", "SIPProxyAddr", tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPProxyServerValue(void* value, struct DeviceNode*  deviceNode)
{

        setVoIPProtocolToIMSSIP();
	tcapi_set("VoIPBasic_Common", "SIPProxyAddr", (char*)value);
#if defined(TCSUPPORT_CT_PON_SN)
    if(isImssipEnable()==1){
        tcapi_set("VoIPBasic_Common", "RegistrarServer", (char*)value);
    }
#endif


	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	return 0;
}		
int8 getSIPProxyServerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aSIPProxyServer;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPProxyServerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aSIPProxyServer;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getSIPProxyServerPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};

	tcapi_get("VoIPBasic_Common", "SIPProxyPort", tempValue);

	if(strlen(tempValue) == 0){
		strcpy(tempValue,"0");
	}
	
	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 setSIPProxyServerPortValue(void* value, struct DeviceNode*  deviceNode)
{	
	uint32 iPort;
	char strPort[10] = {0};

        setVoIPProtocolToIMSSIP();
	iPort = *((uint32*)value);
	if(iPort > 65535){
		return -1;
	}
	sprintf(strPort, "%lu", iPort);
	tcapi_set("VoIPBasic_Common", "SIPProxyPort", strPort);
#if defined(TCSUPPORT_CT_PON_SN)
    if(isImssipEnable()==1){
        tcapi_set("VoIPBasic_Common", "RegistrarServerPort", strPort);
    }
#endif
	return 0;
}	
int8 getSIPProxyServerPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aSIPProxyServerPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setSIPProxyServerPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aSIPProxyServerPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getSIPProxyServerTransportValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	tcapi_get("VoIPBasic_Common", "ProxyServerTransport", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPProxyServerTransportValue(void* value, struct DeviceNode*  deviceNode)
{
	setVoIPProtocolToIMSSIP();
	if(strcmp((char*)value, "TCP") != 0 && strcmp((char*)value, "UDP") != 0)
		return -1;
		
    tcapi_set("VoIPBasic_Common", "ProxyServerTransport", (char*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
    
	return 0;
}		
int8 getSIPProxyServerTransportAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aSIPProxyServerTransport;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPProxyServerTransportAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aSIPProxyServerTransport;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	
int8 getSIPRegistrarServerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};

	tcapi_get("VoIPBasic_Common", "RegistrarServer", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 setSIPRegistrarServerValue(void* value, struct DeviceNode*  deviceNode)
{
	char domainValue[256]={0};
	char tempValue[257] = {0};
	
        setVoIPProtocolToIMSSIP();
	if(isImssipEnable()==1){
		tcapi_get("VoIPAdvanced_Common", "SIPDomain", tempValue);
		if(0 == strlen(tempValue)){
            		tcapi_set("VoIPAdvanced_Common", "SIPDomain", (char*)value);
		}
#if !defined(TCSUPPORT_CT_PON_SN)
            tcapi_set("VoIPBasic_Common", "RegistrarServer", (char*)value);
#endif
		
	}
	else
		tcapi_set("VoIPBasic_Common", "RegistrarServer", (char*)value);
#if defined(TCSUPPORT_CT_PON_BIND2_WEB)
	tcapi_set("WebCurSet_Entry", "isVoipSet", "Yes");
#endif
	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	return 0;	
}	
int8 getSIPRegistrarServerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aSIPRegistrarServer;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setSIPRegistrarServerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aSIPRegistrarServer;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getSIPRegistrarServerPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	tcapi_get("VoIPBasic_Common", "RegistrarServerPort", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 setSIPRegistrarServerPortValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iPort;
	char strPort[10] = {0};

        setVoIPProtocolToIMSSIP();
	iPort = *((uint32*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(iPort > 65535){
		return -1;
	}
	sprintf(strPort, "%lu", iPort);	
#if defined(TCSUPPORT_CT_PON_SN)
    if(isImssipEnable() != 1){
        tcapi_set("VoIPBasic_Common", "RegistrarServerPort", strPort);
    }
#else
     tcapi_set("VoIPBasic_Common", "RegistrarServerPort", strPort);
#endif

	return 0;
}	
int8 getSIPRegistrarServerPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aSIPRegistrarServerPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setSIPRegistrarServerPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aSIPRegistrarServerPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	
int8 getSIPRegistrarServerTransportValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	tcapi_get("VoIPBasic_Common", "RegistrarServerTransport", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 setSIPRegistrarServerTransportValue(void* value, struct DeviceNode*  deviceNode)
{
        setVoIPProtocolToIMSSIP();
	tcapi_set("VoIPBasic_Common", "RegistrarServerTransport", (char*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	return 0;
}	
int8 getSIPRegistrarServerTransportAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aSIPRegistrarServerTransport;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setSIPRegistrarServerTransportAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aSIPRegistrarServerTransport;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	


int8 getOutboundProxyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	
	tcapi_get("VoIPBasic_Common", "SIPOutboundProxyAddr", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setOutboundProxyValue(void* value, struct DeviceNode*  deviceNode)
{
        setVoIPProtocolToIMSSIP();
	tcapi_set("VoIPBasic_Common", "SIPOutboundProxyAddr", (char*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	return 0;
}		
int8 getOutboundProxyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aOutboundProxy;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setOutboundProxyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aOutboundProxy;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

#if defined(TCSUPPORT_CT)
/* support Megaco emulate register 20150612 */
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
#define  CWMP_SOCKSIZE (sizeof(struct sockaddr))
#define CWMP_HTTP_SOCKET_ERROR	-1
typedef unsigned long   CwmpStcpIpAddress;
typedef unsigned short  CwmpStcpPort;
int cwmpVoIPCmdSend(
	char* strRemoteAddress,	
	CwmpStcpPort theRemotePort,
	char * SendData) 
{	
	struct sockaddr_in serverAddr;	
	CwmpStcpIpAddress theRemoteAddress;
	int fClientSocket = CWMP_HTTP_SOCKET_ERROR;	
	int status  = CWMP_HTTP_SOCKET_ERROR;
	
	if(getSipStatus_tr69() == 0) {
		return 0;//return 0 if sip enable
	}

	memset(&serverAddr, 0, sizeof(serverAddr));	
	serverAddr.sin_family = AF_INET;	
	inet_aton(strRemoteAddress, &theRemoteAddress);
	serverAddr.sin_addr.s_addr =theRemoteAddress;	
	serverAddr.sin_port = theRemotePort;	

	fClientSocket = socket(AF_INET, SOCK_STREAM, 0);	
	if(fClientSocket == CWMP_HTTP_SOCKET_ERROR){
		tcdbg_printf("StcpOpenActive: socket error!!!\n");		
		return -1;
	}

	status = connect(fClientSocket, (struct sockaddr*)&serverAddr, CWMP_SOCKSIZE);		
	if(status == CWMP_HTTP_SOCKET_ERROR){		
		tcdbg_printf("StcpOpenActive: connect error!!!\n");				
		close(fClientSocket);	
		return -1;
	}	
	
	send(fClientSocket, SendData, strlen(SendData), 0);
		
	close(fClientSocket);	
	return 0;
}/* StcpOpenActive */
#endif

static void resetCwmpIAD(void)
{   
    strncpy(gCwmpIAD.state, "None", sizeof(gCwmpIAD.state)-1);
	if (rpc.flags.theIADFlag & CWMP_IAD_ACTIVE){
        strncpy(gCwmpIAD.state, "Requested", sizeof(gCwmpIAD.state)-1);
    }
	
	if (rpc.flags.theIADFlag & CWMP_IAD_INPROGRESS){
		//cwmpStopIAD();
		tcdbg_printf("\nresetCwmpIAD-----------stop IAD progress!!!\n");		
	
		rpc.flags.theIADFlag &= ~CWMP_IAD_INPROGRESS;
		rpc.flags.theIADFlag |= CWMP_IAD_TERMINATED; //The IAD test is terminated.
	}
    
    return;
}

static void resetCwmpIADResult(void)
{
	gCwmpIAD.result = 0;
	gCwmpIAD.reason = 0;
	return;
}

static uint32 _IADResultGet(int instance)
{
    int    ret = 0;
    FILE   *fp = NULL;
    char   tempValue[16] = {0};
    uint32 tempU32 = 0;
    
    fp = fopen(IAD_TEST_RESULT_PATH, "r");
	
	if(fp == NULL){
		return -1;
	}
	fgets(tempValue, 16, fp);
	fclose(fp);
	
	/*
	tempU32: bit31~bit24  |  bit23~bit16  |  bit15~bit8  | bit7~bit0
                             reserved         reserved               Line2            Line1
	*/
	sscanf(tempValue,"%x",&tempU32);
	tempU32 = ((tempU32 >> (instance << 3)) & 0xff);

    tcdbg_printf("%s %d, result is %x\n", __func__, __LINE__,  tempU32);
    return tempU32;
}

static int _IADReasonGet(int instance)
{
    int    ret = -1;
    char   tempValue[16] = {0};
    char   nodeName[34] = {'\0'};     

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)  
    if(getSipStatus_tr69() == 0){
        sprintf(nodeName, "InfoVoIP_Entry%d", instance);
    }else{
        sprintf(nodeName, "InfoVoIPH248_Entry%d", instance);
    }
    tcapi_get(nodeName, "RegFailReason", tempValue);
    ret = atoi(tempValue);
#endif

    tcdbg_printf("%s %d, reason is %x\n", __func__, __LINE__, ret);
    return ret;
}

static int checkIADModule(int instance)
{
        int    ret = 0;
        char   tempValue[16] = {0};
        char   nodeName[34] = {'\0'}; 
        int    lastErrorReason[16] = {0};
        int    currErrorReason = 0;
    
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)  
        if(getSipStatus_tr69() == 0){
            sprintf(nodeName, "InfoVoIP_Entry%d", instance);
        }else{
            sprintf(nodeName, "InfoVoIPH248_Entry%d", instance);
        }
       
        tcapi_get(nodeName, "RegFailReason", lastErrorReason); /* save  lastErrorReason*/

        tcapi_set(nodeName, "RegFailReason", "0");  /* iad state restore*/
        
        /* check sipclient or  mgapp is down or not when tcapi_get RegFailReason */  
        tcapi_get(nodeName, "RegFailReason", tempValue);       
        currErrorReason = atoi(tempValue);

        if (IAD_INNER_ERROR == currErrorReason)
            ret = -1;
        else
            tcapi_set(nodeName, "RegFailReason", lastErrorReason); /* restore  lastErrorReason */        
#endif
    
        return ret;
}

static int checkIADVoipWan()
{
    int  ret = -1;
    char wanName[32] = {0};
    char IPAddr[32] = {0};

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
	if (getSipStatus_tr69() == 0){
	    tcapi_get(CWMP_VOIPSYSPARAM_COMMON, CWMP_VOIP_WAN_IFNAME, wanName);
	    tcapi_get(CWMP_VOIPSYSPARAM_COMMON, CWMP_VOIP_WAN_IP, IPAddr);
	    if ((0 != strlen(wanName)) && (0 != strlen(IPAddr)) && (strcmp(IPAddr, "0.0.0.0")!=0)){
	        ret = 0;
	    }
	}else{
		ret = 0;
	}
#endif

    return ret;
}

static int checkIADVoipServer()
{
    int  ret = -1;
    char serName[2][64] = {0}; 
    int  i = 0;

    i = gCwmpIAD.testServer;
    
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
	if (getSipStatus_tr69() == 0){
	    tcapi_get(CWMP_VOIPBASIC_COMMON, CWMP_SIP_REG_SERVER, serName[0]);
	    tcapi_get(CWMP_VOIPBASIC_COMMON, CWMP_SIP_PROXY_ADDR, serName[1]);
	    if (strlen(serName[i-1]) && (strcmp((serName[i-1]), "0.0.0.0")!=0)){
	        ret = 0;
	    }
	}else{
		tcapi_get(CWMP_VOIPH248_COMMON, CWMP_H248_MGC, serName[0]);
		tcapi_get(CWMP_VOIPH248_COMMON, CWMP_H248_SB_MGC, serName[1]);
	    if (strlen(serName[i-1]) && (strcmp((serName[i-1]), "0.0.0.0")!=0)){
	        ret = 0;
	    }
	}
#endif

    return ret;
}

static int checkIADParams()
{
    int ret = -1;

    if (!strcmp(gCwmpIAD.state, "Requested")
        && (gCwmpIAD.testServer == 1 || gCwmpIAD.testServer == 2)){
        ret = 0;
    }

    return ret;
}

static uint32 cwmpIADEmulateReg()
{
    char   cmd[64] = {0};
    int    i = 0;
    uint32 tempU32 = 0;
    int    reason = IAD_OTHER_ERROR;
    
    //cwmpStopIAD();
    unlink(IAD_TEST_RESULT_PATH);
    tcdbg_printf("%s%d\n", __func__, __LINE__);
	
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO) 
	if (getSipStatus_tr69() == 0){
	    snprintf(cmd, sizeof(cmd)-1, IAD_SIP_EMULTE_REG_CMD, IAD_SIP_LINE_ID, gCwmpIAD.testServer);
	    system(cmd);
	}else{
		/* Megaco simulate */
		tcapi_set("InfoVoIPH248_Common","is_IADSimulate", "1");//wuchen_20151117
		snprintf(cmd, sizeof(cmd)-1, "%d", gCwmpIAD.testServer);
		tcapi_set("InfoVoIPH248_Common","IAD_testServer", cmd);		
		snprintf(cmd, sizeof(cmd)-1, "VoIPH248");
		tcapi_commit(cmd);
	}
#endif
    
    while (i < IAD_TIMEOUT){     
        if (rpc.flags.theIADFlag & (CWMP_IAD_TERMINATED|CWMP_IAD_ACTIVE)){
            tc_printf("\r\n[cwmpIADTask] TERMINATED   manually! we do not read IAD result from file\n");
            break;
        }       
        if (-1 == (tempU32 = _IADResultGet(IAD_SIP_LINE_ID))){
            usleep(500*1000);
            i++;
        }else{
            reason = IAD_ERROR_NONE;
            if (tempU32 && (-1 == (reason=_IADReasonGet(IAD_SIP_LINE_ID)))){
                reason = IAD_OTHER_ERROR;              
            }
            tcdbg_printf("%s%d, reason is %d\n", __func__, __LINE__, reason);
            break;
        }
    }

    return reason;
}
static int cwmpIADDealResult(int reason)
{
    if (reason)
        gCwmpIAD.result = 1;

    gCwmpIAD.reason = reason;
    strncpy(gCwmpIAD.state, "Complete", sizeof(gCwmpIAD.state)-1);
    
    if (rpc.flags.theIADFlag & CWMP_IAD_TERMINATED){
        tcdbg_printf("\r\n[cwmpIADTask] terminated unusally, does not send inform.!\n");
        strncpy(gCwmpIAD.state, "None", sizeof(gCwmpIAD.state)-1);
        rpc.flags.theIADFlag &= ~CWMP_IAD_TERMINATED; 
    }else{
        tcdbg_printf("%s%d send inform---\n", __func__, __LINE__);
        /*Send the "8 DIAGNOSTICS COMPLETE" Event code*/
        setIADCompleteFlag(1);	
#if defined(TCSUPPORT_CT_MIDWARE)
        setOperationFlag(1);
#endif
        sem_post(&SEM_IMFORM);
    }

    return 0;
}

void *cwmpIADTask(void)
{
    int    ret = 0;
    int    reason = IAD_OTHER_ERROR;
    int    i = 0;
    uint32 tempU32 = 0;
    char    cmd[64] = {0};
    
    resetCwmpIADResult();//reset the IAD Result before diag.
    rpc.flags.theIADFlag &= ~CWMP_IAD_TERMINATED; 
    rpc.flags.theIADFlag |= CWMP_IAD_INPROGRESS;//The cwmp IAD  is in progress.

    /* check itms set params */
    ret = checkIADParams();
    if (-1 == ret){
        goto RESULT_HANDLE;
    }    

    ret = checkIADVoipWan();
    if (-1 == ret){
        reason = IAD_ROUTE_FAIL;
        goto RESULT_HANDLE;
    }

    ret = checkIADVoipServer();
    if (-1 == ret){
        reason = IAD_SERVER_UNREACHABLE;
        goto RESULT_HANDLE;
    }

    /*check IAD module*/
    ret = checkIADModule(IAD_SIP_LINE_ID);
    if (-1 == ret){
        reason = IAD_INNER_ERROR;
        goto RESULT_HANDLE;
    }
    
    /* send cmd to sipclient to emulate register, default line0 */
    reason = cwmpIADEmulateReg();
    
    /*  reset sipclient status*/
    if (rpc.flags.theIADFlag & CWMP_IAD_ACTIVE){
        p_Cwmp_IAD = 0;
        rpc.flags.theIADFlag &= ~CWMP_IAD_INPROGRESS;
        tcdbg_printf("\r\n%s-%d, reason:%d, ret:%d\n", __func__, __LINE__, reason, ret);
        return NULL;
    }else if (rpc.flags.theIADFlag & CWMP_IAD_TERMINATED){
        //cwmpStopIAD();
    }
    //tcapi_commit(CWMP_VOIPSYSPARAM_COMMON);    

RESULT_HANDLE:
    
    rpc.flags.theIADFlag &= ~CWMP_IAD_INPROGRESS;

    tcdbg_printf("%s%d, rpc.flags.theIADFlag:%d, reason:%d, ret:%d\n", 
        __func__, __LINE__, rpc.flags.theIADFlag, reason, ret);
    /* deal result */
    cwmpIADDealResult(reason);

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO) 
	if (getSipStatus_tr69() == 0){    
	    tcdbg_printf("%s%d commit voipsystem_common---\n", __func__, __LINE__);
	    tcapi_commit(CWMP_VOIPSYSPARAM_COMMON); 
	}else{
	    tcapi_set("InfoVoIPH248_Common","is_IADSimulate", "0");//wuchen_20151117
	    tcdbg_printf("%s%d commit VoIPH248_Common---\n", __func__, __LINE__);
	    tcapi_commit(CWMP_VOIPH248_COMMON); 
	}
#endif
    p_Cwmp_IAD = 0;
    return NULL;
}

int cwmpIADTaskStart(void)
{
	int ret = 0;
	//pthread_t p_cwmp_IAD;
	pthread_attr_t p_IAD_attr;

	/*create thread*/
	ret = pthread_attr_init(&p_IAD_attr);
	if(ret != 0)
	{
		printf("\r\nThread attribute creation fail!");
		return -1;
	}
	//need to set deached way
	ret = pthread_attr_setdetachstate(&p_IAD_attr, PTHREAD_CREATE_DETACHED);
	if(ret != 0)
	{
		printf("\r\nSet attribute fail!");
		return -1;
	}

	ret = pthread_create(&p_Cwmp_IAD,&p_IAD_attr, (void *)cwmpIADTask, NULL);
	if(ret != 0)
	{
		printf("\r\nCreate thread fail!");
		return -1;
	}

	pthread_attr_destroy(&p_IAD_attr);
}

void doCwmpIAD(void)
{
    if(p_Cwmp_IAD == 0)
    {
	tcdbg_printf("\r\ncreate thread to do iad diagnostic!\n");
	cwmpIADTaskStart();
    }
    else
    {
	tcdbg_printf("\r\n-------p_Cwmp_IAD is not 0, error occured!-------------\n");
    }
    return;
}

int8 getIADDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
    return getTheValue(value, type, deviceNode, gCwmpIAD.state);   
}		

int8 setIADDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode)
{

    char tempValue[20] = {0};

    sprintf(tempValue, "%s", (char *)value); 
    if( strcmp("None",tempValue) !=0 && strcmp("Requested",tempValue)!=0
		   &&  strcmp("Complete",tempValue)!=0 )
		return -1;
	
    if ( strcmp(tempValue, "Requested") ==0 ){
        resetCwmpIAD();//If the IAD test is in progress, terminate it first.
        rpc.flags.theIADFlag |= CWMP_IAD_ACTIVE;
        tcdbg_printf("%s%d, rpc.flags.theIADFlag:%d\n", __func__, __LINE__, rpc.flags.theIADFlag);
        strncpy(gCwmpIAD.state, "Requested", sizeof(gCwmpIAD.state)-1);
	}
    else {
        tcdbg_printf("\r\nIPPingDiagnosticsStateType parameter is not Requested!");
        return -1;
    }
	
    return 0;
}		

int8 getIADDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;

	attribute = tcTR104Attr.aIADDiagnosticsState[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setIADDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;

	attribute = &tcTR104Attr.aIADDiagnosticsState[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getTestServerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	snprintf(tempValue, sizeof(tempValue)-1, "%u", gCwmpIAD.testServer);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 setTestServerValue(void* value, struct DeviceNode*  deviceNode)
{
	//  1 main server machine 2 backup server machine
	uint32  iServer;
	 
	 iServer = *((uint32*)value);
	 if(iServer != 1&&iServer != 2)
		 return -1; 

	resetCwmpIAD();//If the IAD test is in progress, terminate it first.
	gCwmpIAD.testServer = iServer;

	return 0;	
}	
int8 getTestServerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;

	attribute = tcTR104Attr.aTestServer;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setTestServerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;

	attribute = &tcTR104Attr.aTestServer;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getRegistResultValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	snprintf(tempValue, sizeof(tempValue)-1, "%u", gCwmpIAD.result);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getRegistResultAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;

	attribute = tcTR104Attr.aRegistResult;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setRegistResultAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;

	attribute = &tcTR104Attr.aRegistResult;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getReasonValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	snprintf(tempValue, sizeof(tempValue)-1, "%u", gCwmpIAD.reason);

	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getReasonAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;

	attribute = tcTR104Attr.aReason;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setReasonAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;

	attribute = &tcTR104Attr.aReason;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

#endif

int8 getOutboundProxyPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	tcapi_get("VoIPBasic_Common", "SIPOutboundProxyPort", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 setOutboundProxyPortValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iPort;
	char strPort[10] = {0};

        setVoIPProtocolToIMSSIP();
	iPort = *((uint32*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(iPort > 65535){
		return -1;
	}
	sprintf(strPort, "%lu", iPort);

	tcapi_set("VoIPBasic_Common", "SIPOutboundProxyPort", strPort);

	return 0;
}	
int8 getOutboundProxyPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aOutboundProxyPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setOutboundProxyPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aOutboundProxyPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getProfileFaxT38Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[5] = {0};
	
	tcapi_get("VoIPMedia_Common", "FaxCodec", tempValue);

	if(strcmp(tempValue, "1") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	
	return getTheValue(value, type, deviceNode, result);
}	

int8 setProfileFaxT38Value(void* value, struct DeviceNode*  deviceNode)
{
	uint8 iEnable;
	char strEnable[10] = {0};

	iEnable = *((uint8*)value);

	SetCfgNodeFlag( "VoIPMedia", -1 );
	
	if(0 == iEnable)
		strcpy(strEnable, "0");
	else if(1 == iEnable)
		strcpy(strEnable, "1");
	else 
		return -1;
	tcapi_set("VoIPMedia_Common", "FaxCodec", strEnable);

	return 0;
}		

int8 getProfileFaxT38Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aProfileFaxT38;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setProfileFaxT38Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aProfileFaxT38;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

#if defined(TCSUPPORT_CUC_VOIP_NODE)
int8 getBitRateFaxT38Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	tcapi_get("VoIPMedia_Common", "BitRate", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setBitRateFaxT38Value(void* value, struct DeviceNode*  deviceNode)
{
	uint32 val;
	char str[10] = {0};

	val = *((uint32*)value);

	SetCfgNodeFlag( "VoIPMedia", -1 );
	
	sprintf(str, "%lu", val);
	
	tcapi_set("VoIPMedia_Common", "BitRate", str);

	return 0;
}
int8 getBitRateFaxT38Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aBitRateFaxT38;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setBitRateFaxT38Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aBitRateFaxT38;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getTCFMethodValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};

	tcapi_get("VoIPMedia_Common", "TCFMethod", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);

}

int8 setTCFMethodValue(void* value, struct DeviceNode*  deviceNode)
{
	tcapi_set("VoIPMedia_Common", "TCFMethod", (char*)value);

	SetCfgNodeFlag( "VoIPMedia", -1 );
	
	return 0;	
}

int8 getTCFMethodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aTCFMethod;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTCFMethodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aTCFMethod;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getHighSpeedRedundancyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	tcapi_get("VoIPMedia_Common", "HighSpeedRedundancy", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setHighSpeedRedundancyValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 val;
	char str[10] = {0};

	val = *((uint32*)value);

	SetCfgNodeFlag( "VoIPMedia", -1 );
	
	if(val > 3 || val < 0){
		return -1;
	}
	sprintf(str, "%lu", val);

	tcapi_set("VoIPMedia_Common", "HighSpeedRedundancy", str);

	return 0;
}

int8 getHighSpeedRedundancyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aHighSpeedRedundancy;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setHighSpeedRedundancyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aHighSpeedRedundancy;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getLowSpeedRedundancyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	tcapi_get("VoIPMedia_Common", "LowSpeedRedundancy", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setLowSpeedRedundancyValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 val;
	char str[10] = {0};

	val = *((uint32*)value);

	SetCfgNodeFlag( "VoIPMedia", -1 );
	
	if(val > 5 || val < 0){
		return -1;
	}
	sprintf(str, "%lu", val);

	tcapi_set("VoIPMedia_Common", "LowSpeedRedundancy", str);

	return 0;
}

int8 getLowSpeedRedundancyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aLowSpeedRedundancy;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLowSpeedRedundancyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aLowSpeedRedundancy;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getX_CU_PortAdd2Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {'\0'};
	char result[5] = {0};
	
	tcapi_get("VoIPMedia_Common", "PortAdd2", tempValue);

	if(strcmp(tempValue, "1") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	
	return getTheValue(value, type, deviceNode, result);
}

int8 setX_CU_PortAdd2Value(void* value, struct DeviceNode*  deviceNode)
{
	uint8 iEnable;
	char strEnable[10] = {'\0'};

	iEnable = *((uint8*)value);

	SetCfgNodeFlag( "VoIPMedia", -1 );
	
	if(0 == iEnable)
		strcpy(strEnable, "0");
	else if(1 == iEnable)
		strcpy(strEnable, "1");
	else 
		return -1;
	tcapi_set("VoIPMediac_Common", "FaxCodec", strEnable);

	return 0;

}

int8 getX_CU_PortAdd2Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aPortAdd2;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setX_CU_PortAdd2Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aPortAdd2;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}
#endif

int	cwmpInitLineSubTree( DeviceNodePtr deviceNode )
{
	int num = LINENUM, rtnCode = 0;

	rtnCode = RegisterDynNodeFromIndex( deviceNode, LineIndex, num );

	return rtnCode;
}

int8 addLineObject(uint32* num, DeviceNode*  deviceNode)
{
	int number = -1;
	DeviceNodePtr ruleNode = NULL;
	char *objectName = "InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.";
	int rtnCode = 0;
	
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &ruleNode);
	if(ruleNode == NULL){
		return -1;
	}
	
{
		cwmpFreeDeviceTable(ruleNode);
		rtnCode = cwmpInitLineSubTree(ruleNode);

		*num = number + 1;
		if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\ncwmpParameter.c---[addLineObject]---*num = ", *num);
		}
	}	
	
	return rtnCode;
}


int8 getRTPVLANIDMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	tcapi_get(nodeName, "VLANIDMark", tempValue);
	if(strlen(tempValue) == 0){
		strcpy(tempValue,"-1");		
	}

	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setRTPVLANIDMarkValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 val;
	char setValue[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);

	val= *((int32*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	sprintf(setValue, "%d", val);

	tcapi_set(nodeName, "VLANIDMark", setValue);
	return 0;
}		
int8 getRTPVLANIDMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;

	attribute = tcTR104Attr.aRTPVLANIDMark[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setRTPVLANIDMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;

	attribute = &tcTR104Attr.aRTPVLANIDMark[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}
int8 findVoicePVCEntryIdx(int* pvc_index,int* entry_index)
{
	int  find_voice_pvc_flag				  = 0;
	char str_servicelist[32]				  = {0};
	char str_wanmode[8] 					  = {0};
	char nodeName[128]						  = {0};
	char ifName[32] 						  = {0};
	int pvc = *pvc_index;
	int entry = *entry_index;
	for(pvc=0;pvc<PVC_NUM;(pvc)++)
	{
		for(entry=0;entry<MAX_SMUX_NUM;(entry)++)
		{
			sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc, entry);
			if( 0 == tcapi_get(nodeName, "WanMode", str_wanmode) &&
			0 == strcmp(str_wanmode, "Route") &&
			0 == tcapi_get(nodeName, "ServiceList", str_servicelist) &&
			NULL != strstr(str_servicelist, "VOICE") &&
			0 == tcapi_get(nodeName, "IFName", ifName)){
				find_voice_pvc_flag = 1;
				break;
			}
		}
		if(find_voice_pvc_flag == 1){
			break;
		}
	}
	if(find_voice_pvc_flag == 1){
		find_voice_pvc_flag = 0;
		tcdbg_printf("find voice wan at pvc%d_entry%d\n",pvc,entry);	
		*pvc_index = pvc;
		*entry_index = entry;	
		return 0;
	}else{
		tcdbg_printf("Not find voice wan!\n");
		return -1;
	}		
}

int8 getRTPlpMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	int  pvc = 0, entry = 0, *pvc_index, *entry_index;
	pvc_index = &pvc;
	entry_index = &entry;
	char nodeName[128]                        = {0};

	if(-1 == findVoicePVCEntryIdx(pvc_index,entry_index)){
		return getTheValue(value, type, deviceNode, tempValue);
	}
	pvc = *pvc_index;
	entry = *entry_index;	
	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc, entry);
	tcapi_get(nodeName, "dot1pData", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setRTPlpMarkValue(void* value, struct DeviceNode*  deviceNode)
{	
	int32 val;
	char setValue[10] = {'\0'};
	int  pvc = 0, entry = 0, *pvc_index, *entry_index;
	pvc_index = &pvc;
	entry_index = &entry;
	char nodeName[128]                        = {0};

	val= *((int32*)value);
	
	if(val< 0 || val > 7)
	{
		return -1;
	}	
	sprintf(setValue, "%d", val);

	if(-1 == findVoicePVCEntryIdx(pvc_index,entry_index)){
		return -1;
	}
	
	pvc = *pvc_index;
	entry = *entry_index;	
	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc, entry);
	tc_printf("\n==>nodeName:%s,setval:%s\n",nodeName,setValue);
	tcapi_set(nodeName, "dot1pData", setValue);

	sprintf(nodeName, "Wan_PVC%d", pvc);
	tc_printf("\n==>nodeName:%s,setval:%s\n",nodeName,setValue);
	tcapi_set(nodeName, "DOT1P", setValue);	

	SetCfgNodeFlag( "Wan", pvc);	
	return 0;
}		
int8 getRTPlpMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aRTPlpMark[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setRTPlpMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aRTPlpMark[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getRTPDSCPMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{	
	char tempValue[12] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Common");
	tcapi_get(nodeName, "RTPDSCPMark", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setRTPDSCPMarkValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 dscp;
	char setValue[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Common");

	dscp= *((uint32*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(dscp< 0 || dscp > 63)
	{
		return -1;
	}
	sprintf(setValue, "%lu", dscp);

	tcapi_set(nodeName, "RTPDSCPMark", setValue);
	return 0;
}		
int8 getRTPDSCPMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aRTPDSCPMark[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setRTPDSCPMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aRTPDSCPMark[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

#if defined(TCSUPPORT_CT)
int8 getSIPStandby_ProxyServerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	tcapi_get("VoIPBasic_Common", "SBSIPProxyAddr", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPStandby_ProxyServerValue(void* value, struct DeviceNode*  deviceNode)
{	
        setVoIPProtocolToIMSSIP();
	tcapi_set("VoIPBasic_Common", "SBSIPProxyAddr", (char*)value);
#if defined(TCSUPPORT_CT_PON_SN)
    if(isImssipEnable() == 1){
	    tcapi_set("VoIPBasic_Common", "SBRegistrarServer", (char*)value);
    }
#endif

	SetCfgNodeFlag( "VoIPBasic", -1 );

	return 0;
}		
int8 getSIPStandby_ProxyServerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPStandby_ProxyServer;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPStandby_ProxyServerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPStandby_ProxyServer;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;

	
	return result;
}

int8 getSIPStandby_ProxyServerPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	tcapi_get("VoIPBasic_Common", "SBSIPProxyPort", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPStandby_ProxyServerPortValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iPort;
	char strPort[10] = {0};

        setVoIPProtocolToIMSSIP();
	iPort = *((uint32*)value);
	if(iPort > 65535){
		return -1;
	}
	sprintf(strPort, "%lu", iPort);
	tcapi_set("VoIPBasic_Common", "SBSIPProxyPort", strPort);
#if defined(TCSUPPORT_CT_PON_SN)
    if(isImssipEnable() == 1){
	    tcapi_set("VoIPBasic_Common", "SBRegistrarServerPort", strPort);
    }
#endif
	return 0;
}		
int8 getSIPStandby_ProxyServerPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPStandby_ProxyServerPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPStandby_ProxyServerPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPStandby_ProxyServerPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getSIPStandby_ProxyServerTransportValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPBasic_Common", "SBProxyServerTransport", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPStandby_ProxyServerTransportValue(void* value, struct DeviceNode*  deviceNode)
{
        setVoIPProtocolToIMSSIP();
	tcapi_set("VoIPBasic_Common", "SBProxyServerTransport", (char*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(strcmp((char*)value, "TCP") != 0 && strcmp((char*)value, "UDP") != 0)
		return -1;
	
	return 0;
}		
int8 getSIPStandby_ProxyServerTransportAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPStandby_ProxyServerTransport;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPStandby_ProxyServerTransportAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPStandby_ProxyServerTransport;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getSIPStandby_RegistrarServerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	
	tcapi_get("VoIPBasic_Common", "SBRegistrarServer", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 setSIPStandby_RegistrarServerValue(void* value, struct DeviceNode*  deviceNode)
{
    setVoIPProtocolToIMSSIP();
#if defined(TCSUPPORT_CT_PON_SN)    
    if(isImssipEnable() != 1){
        tcapi_set("VoIPBasic_Common", "SBRegistrarServer", (char*)value);
    }
#else
	tcapi_set("VoIPBasic_Common", "SBRegistrarServer", (char*)value);
#endif
	SetCfgNodeFlag( "VoIPBasic", -1 );
	return 0;
}	
int8 getSIPStandby_RegistrarServerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPStandby_RegistrarServer;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setSIPStandby_RegistrarServerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPStandby_RegistrarServer;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getSIPStandby_RegistrarServerPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPBasic_Common", "SBRegistrarServerPort", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 setSIPStandby_RegistrarServerPortValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iPort;
	char strPort[10] = {0};

        setVoIPProtocolToIMSSIP();
	iPort = *((uint32*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(iPort > 65535){
		return -1;
	}
	sprintf(strPort, "%lu", iPort);
#if defined(TCSUPPORT_CT_PON_SN)    
    if(isImssipEnable() != 1){
        tcapi_set("VoIPBasic_Common", "SBRegistrarServerPort", strPort);
    }
#else
	tcapi_set("VoIPBasic_Common", "SBRegistrarServerPort", strPort);
#endif
    return 0;
}	
int8 getSIPStandby_RegistrarServerPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPStandby_RegistrarServerPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setSIPStandby_RegistrarServerPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPStandby_RegistrarServerPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getSIPStandby_RegistrarServerTransportValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPBasic_Common", "SBRegistrarServerTransport", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 setSIPStandby_RegistrarServerTransportValue(void* value, struct DeviceNode*  deviceNode)
{
        setVoIPProtocolToIMSSIP();
	tcapi_set("VoIPBasic_Common", "SBRegistrarServerTransport", (char*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	return 0;
}	
int8 getSIPStandby_RegistrarServerTransportAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPStandby_RegistrarServerTransport;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setSIPStandby_RegistrarServerTransportAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPStandby_RegistrarServerTransport;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getSIPStandby_OutboundProxyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	
	tcapi_get("VoIPBasic_Common", "SBOutboundProxyAddr", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPStandby_OutboundProxyValue(void* value, struct DeviceNode*  deviceNode)
{
        setVoIPProtocolToIMSSIP();
	tcapi_set("VoIPBasic_Common", "SBOutboundProxyAddr", (char*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	return 0;
}		
int8 getSIPStandby_OutboundProxyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPStandby_OutboundProxy;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPStandby_OutboundProxyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPStandby_OutboundProxy;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getSIPStandby_OutboundProxyPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPBasic_Common", "SBOutboundProxyPort", tempValue);
	if(!strlen(tempValue))
	{
		strcpy(tempValue, "0");
	}

	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPStandby_OutboundProxyPortValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iPort;
	char strPort[10] = {0};

        setVoIPProtocolToIMSSIP();
	iPort = *((uint32*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(iPort > 65535){
		return -1;
	}
	sprintf(strPort, "%lu", iPort);

	tcapi_set("VoIPBasic_Common", "SBOutboundProxyPort", strPort);

	return 0;
}		
int8 getSIPStandby_OutboundProxyPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPStandby_OutboundProxyPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPStandby_OutboundProxyPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPStandby_OutboundProxyPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}	
#endif

int8 getSIPUserAgentDomainValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	
	tcapi_get("VoIPAdvanced_Common", "SIPDomain", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPUserAgentDomainValue(void* value, struct DeviceNode*  deviceNode)
{
        setVoIPProtocolToIMSSIP();
	if(strlen(value)){
		tcapi_set("VoIPAdvanced_Common", "SIPDomain", (char*)value);
	}
	SetCfgNodeFlag( "VoIPBasic", -1 );

	return 0;
}		
int8 getSIPUserAgentDomainAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPUserAgentDomain;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPUserAgentDomainAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPUserAgentDomain;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}	

int8 getSIPUserAgentPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPBasic_Common", "UserAgentPort", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPUserAgentPortValue(void* value, struct DeviceNode*  deviceNode)
{
       uint32 iPort;
	char strPort[10] = {0};

	iPort = *((uint32*)value);

        setVoIPProtocolToIMSSIP();
	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(iPort > 65535){
		return -1;
	}
	sprintf(strPort, "%lu", iPort);

	tcapi_set("VoIPBasic_Common", "UserAgentPort", strPort);
	

	return 0;
}		
int8 getSIPUserAgentPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPUserAgentPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPUserAgentPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPUserAgentPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}	

int8 getSIPUserAgentTransportValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPBasic_Common", "UserAgentTransport", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPUserAgentTransportValue(void* value, struct DeviceNode*  deviceNode)
{
       setVoIPProtocolToIMSSIP();
       tcapi_set("VoIPBasic_Common", "UserAgentTransport", (char*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );

	return 0;
}		
int8 getSIPUserAgentTransportAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPUserAgentTransport;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPUserAgentTransportAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPUserAgentTransport;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}	

int8 getSIPVLANIDMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPBasic_Common", "VLANIDMark", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPVLANIDMarkValue(void* value, struct DeviceNode*  deviceNode)
{
       int mask;
	char setValue[10] = {0};

	mask= *((int32*)value);

        setVoIPProtocolToIMSSIP();
	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(mask < -1)
	{
		return -1;
	}
	sprintf(setValue, "%d", mask);

	tcapi_set("VoIPBasic_Common", "VLANIDMark", setValue);

	return 0;
}		
int8 getSIPVLANIDMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPVLANIDMark;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPVLANIDMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPVLANIDMark;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getSIPlpMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	int  pvc = 0, entry = 0, *pvc_index, *entry_index;
	pvc_index = &pvc;
	entry_index = &entry;
	char nodeName[128]                        = {0};
	
	if(-1 == findVoicePVCEntryIdx(pvc_index,entry_index)){
		return getTheValue(value, type, deviceNode, tempValue);
	}

	pvc = *pvc_index;
	entry = *entry_index;	
	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc, entry);
	tcapi_get(nodeName, "dot1pData", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPlpMarkValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 val;
	char setValue[10] = {'\0'};
	int  pvc = 0, entry = 0, *pvc_index, *entry_index;
	pvc_index = &pvc;
	entry_index = &entry;
	char nodeName[128]                        = {0};

        setVoIPProtocolToIMSSIP();
	val= *((int32*)value);
	
	if(val< 0 || val > 7)
	{
		return -1;
	}
	sprintf(setValue, "%d", val);

	if(-1 == findVoicePVCEntryIdx(pvc_index,entry_index)){
		return -1;
	}
	pvc = *pvc_index;
	entry = *entry_index;	

	sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc, entry);
	tc_printf("\n==>nodeName:%s,setval:%s\n",nodeName,setValue);
	tcapi_set(nodeName, "dot1pData", setValue);

	sprintf(nodeName, "Wan_PVC%d", pvc);
	tc_printf("\n==>nodeName:%s,setval:%s\n",nodeName,setValue);
	tcapi_set(nodeName, "DOT1P", setValue);	

	SetCfgNodeFlag( "Wan", pvc);	
	return 0;
}		
int8 getSIPlpMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPlpMark;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPlpMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPlpMark;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}	

int8 getSIPDSCPMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPBasic_Common", "DSCPMark", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPDSCPMarkValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 dscp;
	char setValue[10] = {0};

	dscp= *((uint32*)value);

        setVoIPProtocolToIMSSIP();
	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(dscp< 0 || dscp > 63)
	{
		return -1;
	}
	sprintf(setValue, "%lu", dscp);

	tcapi_set("VoIPBasic_Common", "DSCPMark", setValue);
	
	return 0;
}		
int8 getSIPDSCPMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPDSCPMark;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPDSCPMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPDSCPMark;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}	

int8 getSIPHeartbeatSwitchValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {'\0'};

	tcapi_get("VoIPBasic_Common", "HeartbeatSwitch", tempValue);	
	if(!strlen(tempValue))
	{
		strcpy(tempValue, "0");
	}

	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPHeartbeatSwitchValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 swEnable;
	int32 swEnable_cuc;
	char enable[10] = {'\0'};

        setVoIPProtocolToIMSSIP();
	swEnable_cuc= *((int32*)value);
  
	SetCfgNodeFlag( "VoIPBasic", -1 );

	sprintf(enable, "%d", swEnable_cuc);

	tcapi_set("VoIPBasic_Common", "HeartbeatSwitch", enable);
	   
	return 0;
}		
int8 getSIPHeartbeatSwitchAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPHeartbeatSwitch;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPHeartbeatSwitchAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPHeartbeatSwitch;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}	

int8 getSIPHeartbearCycleValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPBasic_Common", "HeartbeatCycle", tempValue);
	if(!strlen(tempValue))
	{
		strcpy(tempValue, "0");
	}

	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPHeartbearCycleValue(void* value, struct DeviceNode*  deviceNode)
{
	int cyclevl;
	char setValue[10] = {0};

	cyclevl= *((int32*)value);

        setVoIPProtocolToIMSSIP();
	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(cyclevl < 0)
	{
		return -1;
	}
	sprintf(setValue, "%d", cyclevl);

	tcapi_set("VoIPBasic_Common", "HeartbeatCycle", setValue);
	
	return 0;
}		
int8 getSIPHeartbearCycleAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPHeartbearCycle;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPHeartbearCycleAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPHeartbearCycle;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}	

int8 getSIPHeartbearCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPBasic_Common", "HeartbeatCount", tempValue);
	if(!strlen(tempValue))
	{
		strcpy(tempValue, "0");
	}

	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPHeartbearCountValue(void* value, struct DeviceNode*  deviceNode)
{
	int countvl;
	char setValue[10] = {0};

	countvl= *((int32*)value);

        setVoIPProtocolToIMSSIP();
	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(countvl < 0)
	{
		return -1;
	}
	sprintf(setValue, "%d", countvl);

	tcapi_set("VoIPBasic_Common", "HeartbeatCount", setValue);

	return 0;
}		
int8 getSIPHeartbearCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPHeartbearCount;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPHeartbearCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPHeartbearCount;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}	

int8 getSIPSessionUpdateTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[32] = {0};
	char strTime[32] = {0};
	int iTimer = 0;
	
	tcapi_get("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_TIMER", tempValue);
	if(!strlen(tempValue))
	{
		strcpy(tempValue, "0");
	}

	iTimer= atoi(tempValue);
	if(iTimer > 0)
	{
		iTimer = (iTimer + 30) / 60;
	}
	memset(tempValue, 0, strlen(tempValue));
	sprintf(tempValue, "%d", iTimer);

	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPSessionUpdateTimerValue(void* value, struct DeviceNode*  deviceNode)
{	
	int timevl;
	char setValue[32] = {0};

	timevl= *((int32*)value);

        setVoIPProtocolToIMSSIP();
	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(timevl < 0)
	{
		return -1;
	}
	if(timevl > 35791394)//default:30 mins
	{
		return -1;
	}
	timevl = timevl * 60;
	sprintf(setValue, "%d", timevl);

	tcapi_set("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_TIMER", setValue);
	

	return 0;
}		
int8 getSIPSessionUpdateTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPSessionUpdateTimer;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPSessionUpdateTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPSessionUpdateTimer;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}	

int8 getSIPRegisterRetryIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPAdvanced_Common", "RegisterRetryInterval", tempValue);
	if(!strlen(tempValue))
	{
		strcpy(tempValue, "0");
	}
	
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setSIPRegisterRetryIntervalValue(void* value, struct DeviceNode*  deviceNode)
{	
	int interval;
	char setValue[10] = {0};

	interval= *((int32*)value);
        setVoIPProtocolToIMSSIP();
	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(interval< 0)
	{
		return -1;
	}
	sprintf(setValue, "%d", interval);

	tcapi_set("VoIPBasic_Common", "RegisterRetryInterval", setValue);

	return 0;
}		
int8 getSIPRegisterRetryIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPRegisterRetryInterval;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPRegisterRetryIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPRegisterRetryInterval;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getSIPRegisterExpiresValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
		
	tcapi_get("VoIPAdvanced_Common", "RegistrationExpire", tempValue);
	if(!strlen(tempValue))
	{
		strcpy(tempValue, "0");
	}

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setSIPRegisterExpiresValue(void* value, struct DeviceNode*  deviceNode)
{	
	int expires;
	char setValue[10] = {0};

	expires= *((int32*)value);

        setVoIPProtocolToIMSSIP();
	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(expires< 0)
	{
		return -1;
	}
	sprintf(setValue, "%d", expires);

	tcapi_set("VoIPAdvanced_Common", "RegistrationExpire", setValue);
	

	return 0;
}		
int8 getSIPRegisterExpiresAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPRegisterExpires;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPRegisterExpiresAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPRegisterExpires;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}
#if defined(TCSUPPORT_VOIP_IMSSIP)//xflu_20140228

int8 getSIPImplicitRegistrationEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};

	tcapi_get("VoIPBasic_Common", "ImplicRegEnable", tempValue);

	if(!strcmp(tempValue, "Yes")){
		strcpy(tempValue, "1");
	}
	else{
		strcpy(tempValue, "0");
	}

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setSIPImplicitRegistrationEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char enable[10] = {0};
	uint8 ImplicitRegEnable=0;
	
	ImplicitRegEnable = *((uint8*)value);

        setVoIPProtocolToIMSSIP();
	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(ImplicitRegEnable  == 1){
		strcpy(enable, "Yes");
	}
	else if(ImplicitRegEnable  == 0){
		strcpy(enable, "No");
	}
	else{
		tc_printf("==> setSIPImplicitRegistrationEnableValue: value should be  0 or 1.\n");
		return -1;
	}

	tcapi_set("VoIPBasic_Common", "ImplicRegEnable", enable);

	return 0;
}		
int8 getSIPImplicitRegistrationEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSIPImplicitRegistrationEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setSIPImplicitRegistrationEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSIPImplicitRegistrationEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}
#endif

#if defined(TCSUPPORT_CT_PON_SC)
int8 getRingVoltageValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[32] = {0};
	
	tcapi_get("VoIPAdvanced_Common", "RingVoltage", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setRingVoltageValue(void* value, struct DeviceNode*  deviceNode)
{
   uint32 ringVol = *(uint32*)value;
   char buf[10] = {0};
   
   sprintf(buf, "%u", ringVol);

   if (ringVol < 40 || ringVol > 60)
   	return -1;

   tcapi_set("VoIPAdvanced_Common", "RingVoltage", buf);
	
	return 0;
}	

int8 getCallIDMsgTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[32] = {0};
	
	tcapi_get("VoIPCallCtrl_Entry0", "ComingCallDisplayMode", tempValue);
	if (!strcmp(tempValue, "0")) 
		strcpy(tempValue, "DTMF");
	else
		strcpy(tempValue, "MDMF");

	tcdbg_printf("\r\ngetCallIDMsgTypeValue(): tempValue = %s", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setCallIDMsgTypeValue(void* value, struct DeviceNode*  deviceNode)
{
    char *mode = NULL;
    mode = (char*)value;
   
	if (!strcmp(value, "DTMF"))
		tcapi_get("VoIPCallCtrl_Entry0", "ComingCallDisplayMode", "0");
	else
		tcapi_get("VoIPCallCtrl_Entry0", "ComingCallDisplayMode", "1");

	SetCfgNodeFlag( "VoIPCallCtrl", -1 );
	
	return 0;
}	
#endif

int8 getDigitMapEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "DigitMapEnable", tempValue);

	if(!strcmp(tempValue, "1")){
		strcpy(tempValue, "1");
	}
	else {
		strcpy(tempValue, "0");
	}

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setDigitMapEnableValue(void* value, struct DeviceNode*  deviceNode)
{
      	char enable[10] = {'\0'};
	uint8 DigitMapEnable;
	
	DigitMapEnable = *((uint8*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	
	if(DigitMapEnable  == 1){
		strcpy(enable, "1");
	}
	else if(DigitMapEnable  == 0){
		strcpy(enable, "0");
	}
	else{
		tc_printf("==> setDigitMapEnableValue: value should be  0 or 1.\n");
		return -1;
	}

	tcapi_set("VoIPDigitMap_Entry", "DigitMapEnable", enable);

	return 0;
}	
int8 getDigitMapEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = tcTR104Attr.aDigitMapEnable[instance];

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setDigitMapEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aDigitMapEnable[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

#define VOIP_DIGITAL_MAP_MAXLEN		(4096)
#define DIGITAL_MAP_NUM				(8)
#define DIGITAL_MAP_PART_MAXLEN		(VOIP_DIGITAL_MAP_MAXLEN/DIGITAL_MAP_NUM)

int8 getDigitMapValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)

	{	
		char* tempValue = NULL;
		char attrName[32] = {0};
		char attrValue[DIGITAL_MAP_PART_MAXLEN+1] = {0};
		int partIdx = 0;
		int len = 0;
		int ret = -1;
		
		
		tempValue = (char *)malloc(VOIP_DIGITAL_MAP_MAXLEN+1);
		if(tempValue == NULL){
		tcdbg_printf("getDigitMapValue:malloc tempValue fail!!!\n");
		return -1;
		}
		
		memset(tempValue,0,DIGITAL_MAP_PART_MAXLEN+1);
		for(partIdx = 0; partIdx < DIGITAL_MAP_NUM; partIdx++){ 	
			memset(attrName,0,32);
			memset(attrValue,0,DIGITAL_MAP_PART_MAXLEN+1);
			sprintf(attrName,"DigitMap%d",partIdx+1);
			tcapi_get("VoIPDigitMap_Entry", attrName, attrValue);
			len = strlen(attrValue);
		
			if(len == 0)
				break;
		
			strcat(tempValue,attrValue);
			if(len < DIGITAL_MAP_PART_MAXLEN)
				break;
			}
		
			ret = getTheValue(value, type, deviceNode, tempValue);
			free(tempValue);
			return ret;
			}

#else
{ 
		char tempValue[12] = {0};
	
		tcapi_get("VoIPDigitMap_Entry", "DigitMap", tempValue);
	
		return getTheValue(value, type, deviceNode, tempValue);
}

#endif

}
int8 setDigitMapValue(void* value, struct DeviceNode*  deviceNode)
{
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
	{
		char *p = (char*)value; 
		char attrValue[DIGITAL_MAP_PART_MAXLEN+1] = {0};
		char attrName[32] = {0};
		int len = 0;
		int i = 0;
		
		len = strlen(p);	
		if(len > VOIP_DIGITAL_MAP_MAXLEN){
			tcdbg_printf("setDigitMapValue: len = %d is more than %d!!!\n",len,VOIP_DIGITAL_MAP_MAXLEN);
			return -1;
		}
		
		while(i++ < DIGITAL_MAP_NUM){
			memset(attrName,0,32);
			memset(attrValue,0,DIGITAL_MAP_PART_MAXLEN+1);
			sprintf(attrName,"DigitMap%d",i);
			if(len > DIGITAL_MAP_PART_MAXLEN){
				strncpy(attrValue,p,DIGITAL_MAP_PART_MAXLEN);
				attrValue[DIGITAL_MAP_PART_MAXLEN] = '\0';
				tcapi_set("VoIPDigitMap_Entry", attrName, attrValue);
				p+=DIGITAL_MAP_PART_MAXLEN;
				len -= DIGITAL_MAP_PART_MAXLEN;
			}
			else{
				strncpy(attrValue,p,len);	
				attrValue[len] = '\0';
				tcapi_set("VoIPDigitMap_Entry", attrName, attrValue);
				break;
			}		
		}
		
		if(i<DIGITAL_MAP_NUM){
			memset(attrName,0,32);
			sprintf(attrName,"DigitMap%d",i+1);
			tcapi_set("VoIPDigitMap_Entry", attrName, "");
		}
	}
#else
	{ 
		tcapi_set("VoIPDigitMap_Entry", "DigitMap", (char*)value);
	}
#endif

	SetCfgNodeFlag( "VoIPBasic", -1 );
	return 0;
	
}	
int8 getDigitMapAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aDigitMap[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setDigitMapAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aDigitMap[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

#if defined(TCSUPPORT_CT_PON_GS)
int8 getDigitMapMatchModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "DigitMapMatchMode", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setDigitMapMatchModeValue(void* value, struct DeviceNode*  deviceNode)
{

	tcapi_set("VoIPDigitMap_Entry", "DigitMapMatchMode", (char*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );

	return 0;
}	
int8 getDigitMapMatchModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aDigitMap[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setDigitMapMatchModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aDigitMap[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}
#endif
#if defined(TCSUPPORT_CT)
int8 getDigitMapSpecialEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	tcapi_get("VoIPDigitMap_Entry", "DigitMapSpecialEnable", tempValue);

	if(!strcmp(tempValue, "1")){
		strcpy(tempValue, "1");
	}
	else {
		strcpy(tempValue, "0");
	}


	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setDigitMapSpecialEnableValue(void* value, struct DeviceNode*  deviceNode)
{
      	char enable[10] = {'\0'};
	uint8 DigitMapEnable;
	DigitMapEnable = *((uint8*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	
	if(DigitMapEnable  == 1){
		strcpy(enable, "1");
	}
	else if(DigitMapEnable  == 0){
		strcpy(enable, "0");
	}
	else{
		tc_printf("==> setDigitMapEnableValue: value should be  0 or 1.\n");
		return -1;
	}

	tcapi_set("VoIPDigitMap_Entry", "DigitMapSpecialEnable", enable);

	return 0;
}	
int8 getDigitMapSpecialEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aDigitMapSpecialEnable;

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setDigitMapSpecialEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aDigitMapSpecialEnable;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getDigitMapSpecialValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[DIGITAL_MAP_PART_MAXLEN+1] = {0};

	tcapi_get("VoIPDigitMap_Entry", "DigitMapSpecial", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setDigitMapSpecialValue(void* value, struct DeviceNode*  deviceNode)
{
	SetCfgNodeFlag( "VoIPDigitMap", -1 );	

	tcapi_set("VoIPDigitMap_Entry", "DigitMapSpecial", (char*)value);

	return 0;
}	
int8 getDigitMapSpecialAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aDigitMapSpecial;

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setDigitMapSpecialAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aDigitMapSpecial;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getServerTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[16] = {0};
	char serverType[16]={0};
	char protcVer[16] = {0};
	
	tcapi_get("VoIPBasic_Common", "SIPProtocol",tempValue);
	
#if defined(TCSUPPORT_CUC_VOIP_NODE)
	if(strcmp(tempValue,"IMSSIP")==0){
		strcpy(tempValue,"IMS-SIP");
	}

	tcapi_get("VoIPBasic_Common", "SIPProtocolVersion",protcVer);
	if(strlen(protcVer))
	{
		sprintf(tempValue, "%s/%s", tempValue, protcVer);
	}
	return getTheValue(value, type, deviceNode, tempValue);
#else
	if(strcmp(tempValue,"IMSSIP")==0){
		strcpy(serverType,"0");
	}else if(strcmp(tempValue,"SIP")==0){
		strcpy(serverType,"1");
	}else if(strcmp(tempValue,"H.248")==0){
		strcpy(serverType,"2");
	}
	return getTheValue(value, type, deviceNode, serverType);
#endif
}
int8 setServerTypeValue(void* value, struct DeviceNode*  deviceNode)
{	
	
#if defined(TCSUPPORT_CUC_VOIP_NODE)
	char tmpValue[32] = {0};
	char *pType = NULL;
	char *pVer = NULL;	
	char *p = (char*)value;	
	int len = strlen (p);

	strncpy(tmpValue, p, len);
	if(strstr(tmpValue, "/"))
	{
		pType = strtok(tmpValue, "/");
		pVer = strtok(NULL, "/");
		
		if(strcmp(pType, "SIP") != 0 &&  strcmp(pType, "H.248") != 0 && strcmp(pType, "IMS-SIP") != 0){
			return -1;
			}
		
		if(strcmp(pType,"IMS-SIP")==0){
			tcapi_set("VoIPBasic_Common", "SIPProtocol", "IMSSIP");
		}else{
			tcapi_set("VoIPBasic_Common", "SIPProtocol", pType);
		}
		
		tcapi_set("VoIPBasic_Common", "SIPProtocolVersion", pVer);
	}
	else
	{
		if(strcmp((char*)value, "SIP") != 0 &&  strcmp((char*)value, "H.248") != 0 && strcmp((char*)value, "IMS-SIP") != 0){
			return -1;
			}

		if(strcmp((char*)value,"IMS-SIP")==0){
			tcapi_set("VoIPBasic_Common", "SIPProtocol", "IMSSIP");
		}else{
		tcapi_set("VoIPBasic_Common", "SIPProtocol",(char*)value);
		}
		
		tcapi_set("VoIPBasic_Common", "SIPProtocolVersion", "");
	}
#else
	char serverType[16] = {0};

	if(strcmp((char*)value,"0")==0){
			strcpy(serverType,"IMSSIP");
#if defined(TCSUPPORT_CT_PON_GD)
			tcapi_set("VoIPAdvanced_Common", "SubscribeType","3");
#endif
	}else if(strcmp((char*)value,"1")==0){
			strcpy(serverType,"SIP");
#if defined(TCSUPPORT_CT_PON_GD)
			tcapi_set("VoIPAdvanced_Common", "SubscribeType","0");
#endif
	}else if(strcmp((char*)value,"2")==0){
			strcpy(serverType,"H.248");
	}else{
			return -1;	
	}
	
	tcapi_set("VoIPBasic_Common", "SIPProtocol",serverType);
#endif
	SetCfgNodeFlag( "VoIPBasic", -1 );
	return 0;
}
int8 getServerTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aSrverType[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setServerTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aSrverType[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getH248DMNameFuncsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[64] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "DMName", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);

}

int8 setH248DMNameFuncsValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 tempValue;
	char strValue[10] = {0};

	tcapi_set("VoIPDigitMap_Entry", "DMName", (char*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	return 0;
}
int8 getH248DMNameFuncsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aH248DMName[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setH248DMNameFuncsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aH248DMName[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getH248DigitMapFuncsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[1024] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "DigitMap1", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);

}

int8 setH248DigitMapFuncsValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 tempValue;
	char strValue[10] = {0};

	tcapi_set("VoIPDigitMap_Entry", "DigitMap1", (char*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );

	return 0;
}
int8 getH248DigitMapFuncsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aH248DigitMap[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setH248DigitMapFuncsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aH248DigitMap[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getStartDigitTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "StartDigitTimer", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setStartDigitTimerValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 tempValue;
	char strValue[10] = {0};

	tempValue = *((int32*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	
	if(tempValue <10 ||tempValue > 20){
		return -1;
	}
	sprintf(strValue, "%d", tempValue);

	tcapi_set("VoIPDigitMap_Entry", "StartDigitTimer", strValue);

	return 0;
}
int8 getStartDigitTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aStartDigitTimer[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setStartDigitTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aStartDigitTimer[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getInterDigitTimerShortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "InterDigitTimerShort", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);	
}
int8 setInterDigitTimerShortValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 tempValue;
	char strValue[10] = {0};

	tempValue = *((int32*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	
	sprintf(strValue, "%d", tempValue);

	tcapi_set("VoIPDigitMap_Entry", "InterDigitTimerShort", strValue);

	return 0;
}	
int8 getInterDigitTimerShortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aInterDigitTimerShort[instance];

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setInterDigitTimerShortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aInterDigitTimerShort[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getInterDigitTimerLongValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "InterDigitTimerLong", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);	
}
int8 setInterDigitTimerLongValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 tempValue;
	char strValue[10] = {0};

	tempValue = *((int32*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	
	sprintf(strValue, "%d", tempValue);

	tcapi_set("VoIPDigitMap_Entry", "InterDigitTimerLong", strValue);

	return 0;
}
int8 getInterDigitTimerLongAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aInterDigitTimerLong[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setInterDigitTimerLongAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aInterDigitTimerLong[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getHangingReminderToneTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "HangingReminderToneTimer", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setHangingReminderToneTimerValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 tempValue;
	char strValue[10] = {0};

	tempValue = *((int32*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	
	sprintf(strValue, "%d", tempValue);

	tcapi_set("VoIPDigitMap_Entry", "HangingReminderToneTimer", strValue);

	return 0;
}
int8 getHangingReminderToneTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aHangingReminderToneTimer[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setHangingReminderToneTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aHangingReminderToneTimer[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getBusyToneTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "BusyToneTimer", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);	
}
int8 setBusyToneTimerValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 tempValue;
	char strValue[10] = {0};

	tempValue = *((int32*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	
	sprintf(strValue, "%d", tempValue);

	tcapi_set("VoIPDigitMap_Entry", "BusyToneTimer", strValue);

	return 0;
}
int8 getBusyToneTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aBusyToneTimer[instance];

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setBusyToneTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aBusyToneTimer[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getNoAnswerTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "NoAnswerTimer", tempValue);
	if(!strlen(tempValue))
	{
		strcpy(tempValue, "0");
	}

	return getTheValue(value, type, deviceNode, tempValue);	
}
int8 setNoAnswerTimerValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 tempValue;
	char strValue[10] = {0};

	tempValue = *((int32*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	
	sprintf(strValue, "%d", tempValue);

	tcapi_set("VoIPDigitMap_Entry", "NoAnswerTimer", strValue);

	return 0;
}	
int8 getNoAnswerTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aNoAnswerTimer[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setNoAnswerTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aNoAnswerTimer[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getMediaGatewayControlerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
    
    
	tcapi_get(VOIPH248_COMMON, MEDIA_GATEWAY_CONTROLER, tempValue);
    
   // tcdbg_printf("[%s, %d] [GET]  MEDIA_GATEWAY_CONTROLER = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
			
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setMediaGatewayControlerValue(void* value, struct DeviceNode*  deviceNode)
{
    //tcdbg_printf("[%s, %d] [SET]  MEDIA_GATEWAY_CONTROLER = [%s]...\n", __FUNCTION__, __LINE__, value);
	setVoIPProtocolToH248();    
	tcapi_set(VOIPH248_COMMON, MEDIA_GATEWAY_CONTROLER, (char*)value);
#if defined(TCSUPPORT_CT_PON_BIND2_WEB)
	tcapi_set("WebCurSet_Entry", "isVoipSet", "Yes");
#endif

	SetCfgNodeFlag(VOIPH248, -1);

	return 0;
}		
int8 getMediaGatewayControlerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aMediaGatewayControler;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setMediaGatewayControlerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aMediaGatewayControler;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getMediaGatewayControlerPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	
	tcapi_get(VOIPH248_COMMON, MEDIA_GATEWAY_CONTROLER_PORT, tempValue);
    
   // tcdbg_printf("[%s, %d] [GET]  MEDIA_GATEWAY_CONTROLER_PORT = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
    
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setMediaGatewayControlerPortValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iPort;
	char strPort[10] = {0};

   // tcdbg_printf("[%s, %d] [SET]  MEDIA_GATEWAY_CONTROLER_PORT = [%s]...\n", __FUNCTION__, __LINE__, value);
	setVoIPProtocolToH248();
	iPort = *((uint32*)value);
	if(iPort > 65535){
		return -1;
	}
    
	sprintf(strPort, "%lu", iPort);
    
   // tcdbg_printf("[%s, %d] [SET]  MEDIA_GATEWAY_CONTROLER_PORT = [%s]...\n", __FUNCTION__, __LINE__, strPort);

	tcapi_set(VOIPH248_COMMON, MEDIA_GATEWAY_CONTROLER_PORT, strPort);
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getMediaGatewayControlerPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aMediaGatewayControlerPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setMediaGatewayControlerPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aMediaGatewayControlerPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getStandby_MediaGatewayControlerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	
	tcapi_get(VOIPH248_COMMON, SB_MEDIA_GATEWAY_CONTROLER, tempValue);

  //  tcdbg_printf("[%s, %d] [GET]  SB_MEDIA_GATEWAY_CONTROLER = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
			
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setStandby_MediaGatewayControlerValue(void* value, struct DeviceNode*  deviceNode)
{
	setVoIPProtocolToH248();
	tcapi_set(VOIPH248_COMMON, SB_MEDIA_GATEWAY_CONTROLER, (char *)value);

   // tcdbg_printf("[%s, %d] [SET]  SB_MEDIA_GATEWAY_CONTROLER = [%s]...\n", __FUNCTION__, __LINE__, value);

	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getStandby_MediaGatewayControlerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aStandby_MediaGatewayControler;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setStandby_MediaGatewayControlerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aStandby_MediaGatewayControler;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getStandby_MediaGatewayControlerPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	
	tcapi_get(VOIPH248_COMMON, SB_MEDIA_GATEWAY_CONTROLER_PORT, tempValue);
    
   // tcdbg_printf("[%s, %d] [GET]  SB_MEDIA_GATEWAY_CONTROLER_PORT = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
    
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setStandby_MediaGatewayControlerPortValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iPort;
	char strPort[10] = {0};

	setVoIPProtocolToH248();
	iPort = *((uint32*)value);
	if(iPort > 65535){
		return -1;
	}
    
	sprintf(strPort, "%lu", iPort);
    
	tcapi_set(VOIPH248_COMMON, SB_MEDIA_GATEWAY_CONTROLER_PORT, strPort); 
    
  //  tcdbg_printf("[%s, %d] [SET]  SB_MEDIA_GATEWAY_CONTROLER_PORT = [%s]...\n", __FUNCTION__, __LINE__, strPort);
    
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getStandby_MediaGatewayControlerPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aStandby_MediaGatewayControlerPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setStandby_MediaGatewayControlerPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aStandby_MediaGatewayControlerPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getMediaGatewayPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	
    tcapi_get(VOIPH248_COMMON, MEDIA_GATEWAT_PORT, tempValue);
    
 //   tcdbg_printf("[%s, %d] [GET]  MEDIA_GATEWAT_PORT = [%s]...\n", __FUNCTION__, __LINE__, tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setMediaGatewayPortValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iPort;
	char strPort[10] = {0};
	
	setVoIPProtocolToH248();
	iPort = *((uint32*)value);
	if(iPort > 65535){
		return -1;
	}
    
	sprintf(strPort, "%lu", iPort);

	tcapi_set(VOIPH248_COMMON, MEDIA_GATEWAT_PORT, strPort);
    
  //  tcdbg_printf("[%s, %d] [SET]  MEDIA_GATEWAT_PORT = [%s]...\n", __FUNCTION__, __LINE__, strPort);
    
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getMediaGatewayPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aMediaGatewayPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setMediaGatewayPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aMediaGatewayPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getMessageEncodingTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	
	tcapi_get(VOIPH248_COMMON, MESSAGE_ENCODING_TYPE, tempValue);
    
  //  tcdbg_printf("[%s, %d] [SET]  MESSAGE_ENCODING_TYPE = [%s]...\n", __FUNCTION__, __LINE__, tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setMessageEncodingTypeValue(void* value, struct DeviceNode*  deviceNode)
{
    setVoIPProtocolToH248();
    if(strcmp((char*)value, "ABNF") != 0 && strcmp((char*)value, "ASN.1") != 0)
    {
        printf("\n[%s, %d]ERROR>>>> value[%s], correct Value[ABNF | ASN.1], Return...\n", __FUNCTION__, __LINE__, value);
        return -1;
    }
    
	tcapi_set(VOIPH248_COMMON, MESSAGE_ENCODING_TYPE, value);

  //  tcdbg_printf("[%s, %d] [GET]  MESSAGE_ENCODING_TYPE = [%s]...\n", __FUNCTION__, __LINE__, value);
    
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getMessageEncodingTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aMessageEncodingType;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setMessageEncodingTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aMessageEncodingType;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getDeviceIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
			
	tcapi_get(VOIPH248_COMMON, DEVICE_ID, tempValue);

   // tcdbg_printf("[%s, %d] [GET]  DEVICE_ID = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
    
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setDeviceIDValue(void* value, struct DeviceNode*  deviceNode)
{
	setVoIPProtocolToH248();
	tcapi_set(VOIPH248_COMMON, DEVICE_ID, (char*)value);
    
  //  tcdbg_printf("[%s, %d] [SET]  DEVICE_ID = [%s]...\n", __FUNCTION__, __LINE__, value);

	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getDeviceIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aDeviceID;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setDeviceIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aDeviceID;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getDeviceIDTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
		
	tcapi_get(VOIPH248_COMMON, DEVICE_ID_TYPE, tempValue);

  //  tcdbg_printf("[%s, %d] [GET]  DEVICE_ID_TYPE = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
    
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setDeviceIDTypeValue(void* value, struct DeviceNode*  deviceNode)
{	
	uint32 iType;
	char strType[4] = {0};

	setVoIPProtocolToH248();
	iType = *((uint32*)value);
	if((iType != 0) && (iType != 1) && (iType != 2))
    {
		return -1;
	}
    
	sprintf(strType, "%u", iType);
	tcapi_set(VOIPH248_COMMON, DEVICE_ID_TYPE, strType);
    
  //  tcdbg_printf("[%s, %d] [SET]  DEVICE_ID_TYPE = [%s]...\n", __FUNCTION__, __LINE__, strType);
    
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getDeviceIDTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aDeviceIDType;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setDeviceIDTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aDeviceIDType;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getPhysicalTermIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
					
	tcapi_get(VOIPH248_COMMON, PHYSICAL_TERMID_START, tempValue);

    //tcdbg_printf("[%s, %d] [GET]  PHYSICAL_TERMID = [%s]...\n", __FUNCTION__, __LINE__, tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setPhysicalTermIDValue(void* value, struct DeviceNode*  deviceNode)
{	
	setVoIPProtocolToH248();
	tcapi_set(VOIPH248_COMMON, PHYSICAL_TERMID_START, (char*)value);

    //tcdbg_printf("[%s, %d] [SET]  PHYSICAL_TERMID = [%s]...\n", __FUNCTION__, __LINE__, value);
    
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getPhysicalTermIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aPhysicalTermID;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setPhysicalTermIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aPhysicalTermID;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getPhysicalTermIDConfigMethodValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
		
	tcapi_get(VOIPH248_COMMON, PHYSICAL_TERMID_CONFIG_METHOD, tempValue);

    //tcdbg_printf("[%s, %d] [GET]  PHYSICAL_TERMID_CONFIG_METHOD = [%s]...\n", __FUNCTION__, __LINE__, tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setPhysicalTermIDConfigMethodValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iConfigMethod;
	char strConfigMethod[4] = {0};

	setVoIPProtocolToH248();
	iConfigMethod = *((uint32*)value);
	if((iConfigMethod != 0) && (iConfigMethod != 1))
    {
        printf("\n[%s, %d]ERROR>>>> iConfigMethod[%d], Corret Value[0|1],return...\n", __FUNCTION__, __LINE__, iConfigMethod);
		return -1;
	}
    
	sprintf(strConfigMethod, "%u", iConfigMethod);
	tcapi_set(VOIPH248_COMMON, PHYSICAL_TERMID_CONFIG_METHOD, strConfigMethod);

  //  tcdbg_printf("[%s, %d] [SET]  PHYSICAL_TERMID_CONFIG_METHOD = [%s]...\n", __FUNCTION__, __LINE__, value);

	SetCfgNodeFlag(VOIPH248, -1);
    
	return 0;
}		
int8 getPhysicalTermIDConfigMethodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aPhysicalTermIDConfigMethod;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setPhysicalTermIDConfigMethodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aPhysicalTermIDConfigMethod;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getPhysicalTermIDPrefixValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
			
	tcapi_get(VOIPH248_COMMON, PHYSICAL_TERMID_PREFIX, tempValue);
    
  //  tcdbg_printf("[%s, %d] [GET]  PHYSICAL_TERMID_PREFIX = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
    
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setPhysicalTermIDPrefixValue(void* value, struct DeviceNode*  deviceNode)
{
	setVoIPProtocolToH248();
	tcapi_set(VOIPH248_COMMON, PHYSICAL_TERMID_PREFIX, (char*)value);
    
   // tcdbg_printf("[%s, %d] [SET]  PHYSICAL_TERMID_PREFIX = [%s]...\n", __FUNCTION__, __LINE__, value);
        
	SetCfgNodeFlag(VOIPH248, -1);

	return 0;
}		
int8 getPhysicalTermIDPrefixAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aPhysicalTermIDPrefix;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setPhysicalTermIDPrefixAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aPhysicalTermIDPrefix;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getPhysicalTermIDAddLenValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
		
	tcapi_get(VOIPH248_COMMON, PHYSICAL_TERMID_ADDLEN, tempValue);

 //   tcdbg_printf("[%s, %d] [GET]  PHYSICAL_TERMID_ADDLEN = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
    
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setPhysicalTermIDAddLenValue(void* value, struct DeviceNode*  deviceNode)
{
   	uint32 iTermIDAddLen;
	char strTermIDAddLen[12] = {0};

	setVoIPProtocolToH248();
	iTermIDAddLen = *((uint32*)value);
    
	sprintf(strTermIDAddLen, "%u", iTermIDAddLen);
    
	tcapi_set(VOIPH248_COMMON, PHYSICAL_TERMID_ADDLEN, strTermIDAddLen);
    
   // tcdbg_printf("[%s, %d] [SET]  PHYSICAL_TERMID_ADDLEN = [%s]...\n", __FUNCTION__, __LINE__, strTermIDAddLen);

	SetCfgNodeFlag(VOIPH248, -1);
    
	return 0;
}		
int8 getPhysicalTermIDAddLenAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aPhysicalTermIDAddLen;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setPhysicalTermIDAddLenAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aPhysicalTermIDAddLen;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getRTPPrefixValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
						
	tcapi_get(VOIPH248_COMMON, RTP_PREFIX, tempValue);

  //  tcdbg_printf("[%s, %d] [GET]  RTP_PREFIX = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
    
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setRTPPrefixValue(void* value, struct DeviceNode*  deviceNode)
{
	setVoIPProtocolToH248();
	tcapi_set(VOIPH248_COMMON, RTP_PREFIX, (char*)value);
    
  //  tcdbg_printf("[%s, %d] [SET]  RTP_PREFIX = [%s]...\n", __FUNCTION__, __LINE__, value);
    
	SetCfgNodeFlag(VOIPH248, -1);

	return 0;
}		
int8 getRTPPrefixAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aRTPPrefix;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setRTPPrefixAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aRTPPrefix;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getEphemeralTermIDAddLenValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
			
	tcapi_get(VOIPH248_COMMON, EPHEMERAL_TERMID_ADDLEN, tempValue);

   // tcdbg_printf("[%s, %d] [GET]  EPHEMERAL_TERMID_ADDLEN = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
    
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setEphemeralTermIDAddLenValue(void* value, struct DeviceNode*  deviceNode)
{
   	uint32 iTermIDAddLen;
	char strTermIDAddLen[257] = {0};

	setVoIPProtocolToH248();
	iTermIDAddLen = *((uint32*)value);
    
	sprintf(strTermIDAddLen, "%u", iTermIDAddLen);
	tcapi_set(VOIPH248_COMMON, EPHEMERAL_TERMID_ADDLEN, strTermIDAddLen);

    //tcdbg_printf("[%s, %d] [SET]  EPHEMERAL_TERMID_ADDLEN = [%s]...\n", __FUNCTION__, __LINE__, strTermIDAddLen);

    SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getEphemeralTermIDAddLenAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aEphemeralTermIDAddLen;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setEphemeralTermIDAddLenAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aEphemeralTermIDAddLen;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getEphemeralTermIDUniformValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
		
	tcapi_get(VOIPH248_COMMON, EPHEMERAL_TERMID_UNIFORM, tempValue);

  //  tcdbg_printf("[%s, %d] [GET]  EPHEMERAL_TERMID_UNIFORM = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
    
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setEphemeralTermIDUniformValue(void* value, struct DeviceNode*  deviceNode)
{
   	uint32 iUniform;
	char strUniform[4] = {0};

	setVoIPProtocolToH248();
	iUniform = *((uint32*)value);
	if((iUniform != 0) && (iUniform != 1))
    {
        printf("\n[%s, %d]ERROR>>>> iUniform[%d], Corret Value[0|1],return...\n", __FUNCTION__, __LINE__, iUniform);
		return -1;
	}
    
	sprintf(strUniform, "%u", iUniform);
	tcapi_set(VOIPH248_COMMON, EPHEMERAL_TERMID_UNIFORM, strUniform);
    
   // tcdbg_printf("[%s, %d] [SET]  EPHEMERAL_TERMID_UNIFORM = [%s]...\n", __FUNCTION__, __LINE__, strUniform);
    
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getEphemeralTermIDUniformAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aEphemeralTermIDUniform;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setEphemeralTermIDUniformAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aEphemeralTermIDUniform;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getEphemeralTermIDStartValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
		
	tcapi_get(VOIPH248_COMMON, EPHEMERAL_TERMID_START, tempValue);

   // tcdbg_printf("[%s, %d] [GET]  EPHEMERAL_TERMID_START = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
    
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setEphemeralTermIDStartValue(void* value, struct DeviceNode*  deviceNode)
{
   	uint32 iTermIDStart;
	char strTermIDStart[257] = {0};

	setVoIPProtocolToH248();
	iTermIDStart = *((uint32*)value);
    
	sprintf(strTermIDStart, "%u", iTermIDStart);
	tcapi_set(VOIPH248_COMMON, EPHEMERAL_TERMID_START, strTermIDStart);
    
 //   tcdbg_printf("[%s, %d] [SET]  EPHEMERAL_TERMID_START = [%s]...\n", __FUNCTION__, __LINE__, strTermIDStart);
    
	SetCfgNodeFlag(VOIPH248, -1);
    
	return 0;
}		
int8 getEphemeralTermIDStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aEphemeralTermIDStart;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setEphemeralTermIDStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aEphemeralTermIDStart;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getThreeHandShakeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
		
	tcapi_get(VOIPH248_COMMON, THREE_HAND_SHAKE, tempValue);

    //tcdbg_printf("[%s, %d] [GET]  THREE_HAND_SHAKE = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
    
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setThreeHandShakeValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iThreeHandShake;
	char strThreeHandShake[4] = {0};

	setVoIPProtocolToH248();
	iThreeHandShake = *((uint32*)value);
	if((iThreeHandShake != 0) && (iThreeHandShake != 1))
    {
        printf("\n[%s, %d]ERROR>>>> iType[%d], Corret Value[0|1],return...\n", __FUNCTION__, __LINE__, iThreeHandShake);
		return -1;
	}
    
	sprintf(strThreeHandShake, "%u", iThreeHandShake);
	tcapi_set(VOIPH248_COMMON, THREE_HAND_SHAKE, strThreeHandShake);
    
 //   tcdbg_printf("[%s, %d] [SET]  THREE_HAND_SHAKE = [%s]...\n", __FUNCTION__, __LINE__, strThreeHandShake);

	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getThreeHandShakeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aThreeHandShake;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setThreeHandShakeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aThreeHandShake;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getLongTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
		
	tcapi_get(VOIPH248_COMMON, LONG_TIMER, tempValue);
    
 //   tcdbg_printf("[%s, %d] [GET]  LONG_TIMER = [%s]...\n", __FUNCTION__, __LINE__, tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setLongTimerValue(void* value, struct DeviceNode*  deviceNode)
{
   	uint32 iLongTimer;
	char strLongTimer[12] = {0};

	setVoIPProtocolToH248();
	iLongTimer = *((uint32*)value);
    
	sprintf(strLongTimer, "%u", iLongTimer);
	tcapi_set(VOIPH248_COMMON, LONG_TIMER, strLongTimer);

  //  tcdbg_printf("[%s, %d] [SET]  LONG_TIMER = [%s]...\n", __FUNCTION__, __LINE__, strLongTimer);

	SetCfgNodeFlag(VOIPH248, -1);

	return 0;
}		
int8 getLongTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aLongTimer;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setLongTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aLongTimer;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getPendingTimerInitValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
			
	tcapi_get(VOIPH248_COMMON, PENDING_TIMER_INIT, tempValue);
    
  //  tcdbg_printf("[%s, %d] [GET]  PENDING_TIMER_INIT = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setPendingTimerInitValue(void* value, struct DeviceNode*  deviceNode)
{
   	uint32 iPendingTimer;
	char strPendingTimer[12] = {0};

	setVoIPProtocolToH248();
	iPendingTimer = *((uint32*)value);
    
	sprintf(strPendingTimer, "%u", iPendingTimer);
    
	tcapi_set(VOIPH248_COMMON, PENDING_TIMER_INIT, strPendingTimer);

   // tcdbg_printf("[%s, %d] [SET]  PENDING_TIMER_INIT = [%s]...\n", __FUNCTION__, __LINE__, strPendingTimer);

	SetCfgNodeFlag(VOIPH248, -1);

	return 0;
}		
int8 getPendingTimerInitAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aPendingTimerInit;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setPendingTimerInitAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aPendingTimerInit;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;

	return result;
}

int8 getRetranIntervalTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
			
	tcapi_get(VOIPH248_COMMON, RETRAN_INTERVAL_TIMER, tempValue);
    
  //  tcdbg_printf("[%s, %d] [GET]  RETRAN_INTERVAL_TIMER = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
    
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setRetranIntervalTimerValue(void* value, struct DeviceNode*  deviceNode)
{
   	uint32 iRetranIntervalTimer;
	char strRetranIntervalTimer[12] = {0};

	setVoIPProtocolToH248();
	iRetranIntervalTimer = *((uint32*)value);
    
	sprintf(strRetranIntervalTimer, "%u", iRetranIntervalTimer);
	tcapi_set(VOIPH248_COMMON, RETRAN_INTERVAL_TIMER, strRetranIntervalTimer);
    
  //  tcdbg_printf("[%s, %d] [SET]  RETRAN_INTERVAL_TIMER = [%s]...\n", __FUNCTION__, __LINE__, strRetranIntervalTimer);

	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getRetranIntervalTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aRetranIntervalTimer;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setRetranIntervalTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aRetranIntervalTimer;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getMaxRetranCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
		
	if(tcapi_get(VOIPH248_COMMON, MAX_RETRAN_COUNT, tempValue) <0)
			strcpy(tempValue,"3");

  //  tcdbg_printf("[%s, %d] [GET]  MAX_RETRAN_COUNT = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
    
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setMaxRetranCountValue(void* value, struct DeviceNode*  deviceNode)
{
   	uint32 iMaxRetranCount;
	char strMaxRetranCount[12] = {0};

	setVoIPProtocolToH248();
	iMaxRetranCount = *((uint32*)value);
    
	sprintf(strMaxRetranCount, "%u", iMaxRetranCount);
	tcapi_set(VOIPH248_COMMON, MAX_RETRAN_COUNT, strMaxRetranCount);
    
  //  tcdbg_printf("[%s, %d] [SET]  MAX_RETRAN_COUNT = [%s]...\n", __FUNCTION__, __LINE__, strMaxRetranCount);
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getMaxRetranCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aMaxRetranCount;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setMaxRetranCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aMaxRetranCount;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getRetransmissionTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
			
	if(tcapi_get(VOIPH248_COMMON, RETRANS_MISSION_TIME, tempValue) <0)
		strcpy(tempValue,"25");
    
   // tcdbg_printf("[%s, %d] [GET]  RETRANS_MISSION_TIME = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setRetransmissionTimeValue(void* value, struct DeviceNode*  deviceNode)
{
   	uint32 iRetransmissionTime;
	char strRetransmissionTime[12] = {0};

	setVoIPProtocolToH248();
	iRetransmissionTime = *((uint32*)value);
    
	sprintf(strRetransmissionTime, "%d", iRetransmissionTime);
	tcapi_set(VOIPH248_COMMON, RETRANS_MISSION_TIME, strRetransmissionTime);
    
    //tcdbg_printf("[%s, %d] [SET]  RETRANS_MISSION_TIME = [%s]...\n", __FUNCTION__, __LINE__, strRetransmissionTime);
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getRetransmissionTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aRetransmissionTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setRetransmissionTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aRetransmissionTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getRetransmissionCycleValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
			
	if(tcapi_get(VOIPH248_COMMON, RETRANS_MISSION_CYCLE, tempValue) <0)
		strcpy(tempValue,"4");	
    
  //  tcdbg_printf("[%s, %d] [GET]  RETRANS_MISSION_CYCLE = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setRetransmissionCycleValue(void* value, struct DeviceNode*  deviceNode)
{
   	uint32 iRetransmissionCycle;
	char strRetransmissionCycle[257] = {0};

	setVoIPProtocolToH248();
	iRetransmissionCycle = *((uint32*)value);
    
	sprintf(strRetransmissionCycle, "%d", iRetransmissionCycle);
	tcapi_set(VOIPH248_COMMON, RETRANS_MISSION_CYCLE, strRetransmissionCycle);
    
   // tcdbg_printf("[%s, %d] [SET]  RETRANS_MISSION_CYCLE = [%s]...\n", __FUNCTION__, __LINE__, strRetransmissionCycle);
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getRetransmissionCycleAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aRetransmissionCycle;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setRetransmissionCycleAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aRetransmissionCycle;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getHeartbeatModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
		
	tcapi_get(VOIPH248_COMMON, HEARTBEAT_MODE, tempValue);
    
  //  tcdbg_printf("[%s, %d] [GET]  HEARTBEAT_MODE = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setHeartbeatModeValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iHeartbeatMode;
	char strHeartbeatMode[4] = {0};

	setVoIPProtocolToH248();
	iHeartbeatMode = *((uint32*)value);
	if((iHeartbeatMode != 0) && (iHeartbeatMode != 1))
    {
        printf("\n[%s, %d]ERROR>>>> iHeartbeatMode[%d], Corret Value[0|1],return...\n", __FUNCTION__, __LINE__, iHeartbeatMode);
		return -1;
	}
    
	sprintf(strHeartbeatMode, "%d", iHeartbeatMode);
	tcapi_set(VOIPH248_COMMON, HEARTBEAT_MODE, strHeartbeatMode);
    
  //  tcdbg_printf("[%s, %d] [SET]  HEARTBEAT_MODE = [%s]...\n", __FUNCTION__, __LINE__, strHeartbeatMode);
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getHeartbeatModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aHeartbeatMode;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setHeartbeatModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aHeartbeatMode;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getHeartbeatCycleValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
		
		if(getSipStatus_tr69() == 1){	 /*H.248*/
	
			tcapi_get(VOIPH248_COMMON, HEARTBEAT_CYCLE_H248, tempValue);
		}
#endif
			
    
  //  tcdbg_printf("[%s, %d] [GET]  HEARTBEAT_CYCLE = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setHeartbeatCycleValue(void* value, struct DeviceNode*  deviceNode)
{
   	uint32 iHeartbeatCycle;
	char strHeartbeatCycle[12] = {0};

	setVoIPProtocolToH248();
	iHeartbeatCycle = *((uint32*)value);
    
	sprintf(strHeartbeatCycle, "%d", iHeartbeatCycle);

	
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
		if(getSipStatus_tr69() == 1){	 /*H.248*/
		
			tcapi_set(VOIPH248_COMMON, HEARTBEAT_CYCLE_H248, strHeartbeatCycle);
			SetCfgNodeFlag(VOIPH248, -1);
		}
#endif

    
   // tcdbg_printf("[%s, %d] [SET]  HEARTBEAT_CYCLE = [%s]...\n", __FUNCTION__, __LINE__, strHeartbeatCycle);
	
	return 0;
}		
int8 getHeartbeatCycleAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aHeartbeatCycle;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setHeartbeatCycleAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aHeartbeatCycle;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getHeartbeatCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)

		if(getSipStatus_tr69() == 1){	 /*H.248*/
	
			tcapi_get(VOIPH248_COMMON, HEARTBEAT_COUNT_H248, tempValue);
		}

#endif
    
  //  tcdbg_printf("[%s, %d] [GET]  HEARTBEAT_COUNT = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setHeartbeatCountValue(void* value, struct DeviceNode*  deviceNode)
{
   	uint32 iHeartbeatCount;
	char strHeartbeatCount[12] = {0};

	setVoIPProtocolToH248();
	iHeartbeatCount = *((uint32*)value);
    
	sprintf(strHeartbeatCount, "%d", iHeartbeatCount);
	
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
				
	if(getSipStatus_tr69() == 1){	 /*H.248*/
			
		tcapi_set(VOIPH248_COMMON, HEARTBEAT_COUNT_H248, strHeartbeatCount);
		SetCfgNodeFlag(VOIPH248, -1);
	}
#endif

   
  //  tcdbg_printf("[%s, %d] [SET]  HEARTBEAT_COUNT = [%s]...\n", __FUNCTION__, __LINE__, strHeartbeatCount);
	return 0;
}		
int8 getHeartbeatCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aHeartbeatCount;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setHeartbeatCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aHeartbeatCount;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getRegisterCycleValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
			
	if(tcapi_get(VOIPH248_COMMON, REGISTER_CYCLE, tempValue) <0)
		strcpy(tempValue,"60");	
    
  //  tcdbg_printf("[%s, %d] [GET]  REGISTER_CYCLE = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setRegisterCycleValue(void* value, struct DeviceNode*  deviceNode)
{	
   	uint32 iRegisterCycle;
	char strRegisterCycle[12] = {0};

	setVoIPProtocolToH248();
	iRegisterCycle = *((uint32*)value);
    
	sprintf(strRegisterCycle, "%d", iRegisterCycle);
	tcapi_set(VOIPH248_COMMON, REGISTER_CYCLE, strRegisterCycle);
    
  //  tcdbg_printf("[%s, %d] [SET]  REGISTER_CYCLE = [%s]...\n", __FUNCTION__, __LINE__, strRegisterCycle);
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getRegisterCycleAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aRegisterCycle;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setRegisterCycleAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aRegisterCycle;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getMandatoryRegisterValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	char cFlag[8] =  {0};
			
	tcapi_get(VOIPH248_COMMON, MANDATORY_REGISTER, tempValue);

 //   tcdbg_printf("[%s, %d] [GET]  MANDATORY_REGISTER = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
		
	if(strcmp(tempValue, "Yes") == 0){
		strcpy(cFlag, "true");
	} 
	else{
		strcpy(cFlag, "false");
	}
	return getTheValue(value, type, deviceNode,cFlag);
}
int8 setMandatoryRegisterValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 flag;
	flag = *((uint8*)value);

	setVoIPProtocolToH248();
	if(flag == 1){
		tcapi_set(VOIPH248_COMMON, MANDATORY_REGISTER, "Yes");
	} 
	else if (flag == 0){
		tcapi_set(VOIPH248_COMMON, MANDATORY_REGISTER, "No");
	}
	else{
		tc_printf("==>VoIPH248_Common:value should be 0 or 1.\n");
		return -1;
	}

  //  tcdbg_printf("[%s, %d] [SET]  MANDATORY_REGISTER = [%s]...\n", __FUNCTION__, __LINE__, flag);
	SetCfgNodeFlag(VOIPH248, -1);
    
	return 0;
}		
int8 getMandatoryRegisterAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aMandatoryRegister;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setMandatoryRegisterAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aMandatoryRegister;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getAuthenticationMethIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[16] = {0};
			
	tcapi_get(VOIPH248_COMMON, AUTHENTICATION_METHID, tempValue);
    
 //   tcdbg_printf("[%s, %d] [GET]  AUTHENTICATION_METHID = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
		
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setAuthenticationMethIDValue(void* value, struct DeviceNode*  deviceNode)
{
   	uint32 iAuthenticationMethID;
	char strAuthenticationMethID[16] = {0};

	setVoIPProtocolToH248();
	iAuthenticationMethID = *((uint32*)value);
	if((iAuthenticationMethID != 0) && (iAuthenticationMethID != 1) && (iAuthenticationMethID != 2))
    {
        printf("\n[%s, %d]ERROR>>>> iUniform[%d], Corret Value[0|1],return...\n", __FUNCTION__, __LINE__, iAuthenticationMethID);
		return -1;
	}
    
	sprintf(strAuthenticationMethID, "%u", iAuthenticationMethID);
	tcapi_set(VOIPH248_COMMON, AUTHENTICATION_METHID, strAuthenticationMethID);

   // tcdbg_printf("[%s, %d] [SET]  AUTHENTICATION_METHID = [%s]...\n", __FUNCTION__, __LINE__, strAuthenticationMethID);
	SetCfgNodeFlag(VOIPH248, -1);
    
	return 0;
}		
int8 getAuthenticationMethIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aAuthenticationMethID;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setAuthenticationMethIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aAuthenticationMethID;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getCOM_NumberPlanValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
					
	tcapi_get("VoIPAdvanced_Common", "NumberPlan", tempValue);
					
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setCOM_NumberPlanValue(void* value, struct DeviceNode*  deviceNode)
{
	tcapi_set("VoIPAdvanced_Common", "NumberPlan", (char*)value);

	SetCfgNodeFlag( "VoIPAdvanced", -1 );

	return 0;
}		
int8 getCOM_NumberPlanAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aCOM_NumberPlan;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setCOM_NumberPlanAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aCOM_NumberPlan;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getG711FAXEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};

    /* FaxPassThruCodec:PCMU/PCMA */
	tcapi_get("VoIPAdvanced_Common", "FaxPassThruCodec", tempValue);

	if(!strcmp(tempValue, "PCMA")){
		strcpy(tempValue, "1");
	}
	else{
		strcpy(tempValue, "0");
	}
		
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setG711FAXEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char enable[10] = {'\0'};
	uint8 G711FAXEnable;
	
	G711FAXEnable = *((uint8*)value);	
	
	if(G711FAXEnable  == 1){
		strcpy(enable, "PCMA");
	}
	else if(G711FAXEnable  == 0){
		strcpy(enable, "PCMU");
	}
	else{
		tc_printf("==> setG711FAXEnableValue: value should be  0 or 1.\n");
		return -1;
	}

	tcapi_set("VoIPAdvanced_Common", "FaxPassThruCodec", enable);
	SetCfgNodeFlag( "VoIPAdvanced", -1 );

	return 0;
}
int8 getG711FAXEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aG711FAXEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setG711FAXEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aG711FAXEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getControlTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
			
	tcapi_get("VoIPAdvanced_Common", "FaxCtrlMode", tempValue);
		
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setControlTypeValue(void* value, struct DeviceNode*  deviceNode)
{
	tcapi_set("VoIPAdvanced_Common", "FaxCtrlMode", (char*)value);

	return 0;
}		
int8 getControlTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aControlType;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setControlTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aControlType;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}
#endif

#if defined(TCSUPPORT_CT_PON_GS)
int8 getProfileBodyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setProfileBodyValue(void* value, struct DeviceNode*  deviceNode)
{
	tcdbg_printf("\r\nsetProfileBodyValue(): set failed");
	return 0;
}	
#endif

#if CHINA_UNICOM_ENABLE
int8 getDeviceNameValue(char **value, ParameterType *type, DeviceNode *deviceNode)
{
	char tempValue[257] = {0};
		
	tcapi_get(VOIPH248_COMMON, DEVICE_NAME, tempValue);
    
   // tcdbg_printf("[%s, %d] [GET]  DEVICE_NAME = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setDeviceNameValue(void *value, struct DeviceNode *deviceNode)
{ 
	tcapi_set(VOIPH248_COMMON, DEVICE_NAME, (char*)value);
 
 //   tcdbg_printf("[%s, %d] [SET]  DEVICE_NAME = [%s]...\n", __FUNCTION__, __LINE__, value);
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}
int8 getDeviceNameAttribute(uint8 *notification, AccessEntry **acl, DeviceNode *deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aDeviceName;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDeviceNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aDeviceName;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}


int8 getMIDFormatValue(char **value, ParameterType *type, DeviceNode *deviceNode)
{
	char tempValue1[65] = {0};
	char tempValue2[257] = {0};
		
	tcapi_get(VOIPH248_COMMON, DEVICE_ID_TYPE, tempValue1);
       
  //  tcdbg_printf("[%s, %d] [GET]  DEVICEID_TYPE = [%s]...\n", __FUNCTION__, __LINE__, tempValue1);

    if(strcmp((char*)tempValue1, "0") == 0)
    {
        strcpy((char*)tempValue2, "IP");
    }
    else if(strcmp((char*)tempValue1, "1") == 0)
    {
        strcpy((char*)tempValue2, "DomainName");
    }
    else if(strcmp((char*)tempValue1, "2") == 0)
    {
        strcpy((char*)tempValue2, "DeviceName");
    }
    else;

	return getTheValue(value, type, deviceNode, tempValue2);
}
int8 setMIDFormatValue(void *value, struct DeviceNode *deviceNode)
{
	uint32 iMIDFormat;
	char strMIDFormat[12] = {0};
    
    if((strcmp((char*)value, "DomainName") != 0) && (strcmp((char*)value, "IP") != 0) && (strcmp((char*)value, "DeviceName") != 0))
    {
        printf("\n[%s, %d]ERROR>>>> value[%s], correct Value[DomainName | IP | DeviceName], Return...\n", __FUNCTION__, __LINE__, value);
        return -1;
    }

    if(strcmp((char*)value, "IP") == 0)
    {
        iMIDFormat = 0;
    }
    else if(strcmp((char*)value, "DomainName") == 0)
    {
        iMIDFormat = 1;
    }
    else if(strcmp((char*)value, "DeviceName") == 0)
    {
        iMIDFormat = 2;
    }
    else;
    
	sprintf(strMIDFormat, "%u", iMIDFormat);
    
	tcapi_set(VOIPH248_COMMON, DEVICE_ID_TYPE, (char*)strMIDFormat);
  //  tcdbg_printf("[%s, %d] [SET]  DEVICE_ID_TYPE = [%s]...\n", __FUNCTION__, __LINE__, strMIDFormat);
 
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}
int8 getMIDFormatAttribute(uint8 *notification, AccessEntry **acl, DeviceNode *deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aDeviceIDType;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMIDFormatAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aDeviceIDType;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}


int8 getDSCPMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
		
    tcapi_get(VOIPH248_COMMON, MEGACO_SIGNAL_DSCP, tempValue);
    
  //  tcdbg_printf("[%s, %d] [GET]  MEGACO_SIGNAL_DSCP = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
    
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setDSCPMarkValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iPort;
	char strPort[10] = {0};

	iPort = *((uint32*)value);
	if(iPort > 63){
		return -1;
	}
    
	sprintf(strPort, "%u", iPort);

	tcapi_set(VOIPH248_COMMON, MEGACO_SIGNAL_DSCP, strPort);
    
 //   tcdbg_printf("[%s, %d] [SET]  MEGACO_SIGNAL_DSCP = [%s]...\n", __FUNCTION__, __LINE__, strPort);
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}
int8 getDSCPMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aDSCPMark;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setDSCPMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aDSCPMark;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getSoftswitchVendorValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
			
	if(tcapi_get(VOIPH248_COMMON, "SoftswitchVendor", tempValue)<0)
		strcpy(tempValue,"0");	
				
	return getTheValue(value, type, deviceNode, tempValue);

}
int8 setSoftswitchVendorValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iPort;
	char strPort[10] = {0};
	
	iPort = *((uint32*)value);

	sprintf(strPort, "%u", iPort);
	
	tcapi_set(VOIPH248_COMMON, "SoftswitchVendor", strPort);
		
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;

}
int8 getSoftswitchVendorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	
	attribute = tcTR104Attr.aSoftswitchVendor;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}
int8 setSoftswitchVendorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	
	attribute = &tcTR104Attr.aSoftswitchVendor;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
		

}

int8 getSoftswitchVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[32] = {0};
				
	if(tcapi_get(VOIPH248_COMMON, "SoftswitchVersion", tempValue) <0);
		strcpy(tempValue," ");	
					
	return getTheValue(value, type, deviceNode, tempValue);
}


int8 setSoftswitchVersionValue(void* value, struct DeviceNode*  deviceNode)
{
	char strPort[32] = {0};
		
	strncpy(strPort,value,31);
	tcapi_set(VOIPH248_COMMON, "SoftswitchVersion", strPort);
			
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;

}
int8 getSoftswitchVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
		
	attribute = tcTR104Attr.aSoftswitchVersion;
result=GetTheAttributes(attribute,notification, acl, deviceNode);
		return result;

}
int8 setSoftswitchVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
		
	attribute = &tcTR104Attr.aSoftswitchVersion;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;

}


int8 getauthValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};

	tcapi_get(VOIPH248_COMMON, AUTH, tempValue);
    
 //   tcdbg_printf("[%s, %d] [GET]  AUTH = [%s]...\n", __FUNCTION__, __LINE__, tempValue);

	return getTheValue(value, type, deviceNode,tempValue);
}
int8 setauthValue(void* value, struct DeviceNode*  deviceNode)
{
   	uint32 iauth;
	char strauth[12] = {0};

	iauth = *((uint32*)value);
    
	sprintf(strauth, "%u", iauth);

	tcapi_set(VOIPH248_COMMON, AUTH, strauth);
    
 //   tcdbg_printf("[%s, %d] [SET]  AUTH = [%s]...\n", __FUNCTION__, __LINE__, strauth);
    
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getauthAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aAuth;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setauthAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aAuth;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getauthInitKeyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};

	tcapi_get(VOIPH248_COMMON, AUTH_INIT_KEY, tempValue);
    
  //  tcdbg_printf("[%s, %d] [GET]  AUTH_INIT_KEY = [%s]...\n", __FUNCTION__, __LINE__, tempValue);

	return getTheValue(value, type, deviceNode,tempValue);
}
int8 setauthInitKeyValue(void* value, struct DeviceNode*  deviceNode)
{
	tcapi_set(VOIPH248_COMMON, AUTH_INIT_KEY, (char*)value);
    
  //  tcdbg_printf("[%s, %d] [SET]  AUTH_INIT_KEY = [%s]...\n", __FUNCTION__, __LINE__, value);
    
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getauthInitKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aAuthInitKey;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setauthInitKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aAuthInitKey;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}


int8 getauthmgidValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	tcapi_get(VOIPH248_COMMON, AUTH_MGID, tempValue);
        
   // tcdbg_printf("[%s, %d] [GET]  AUTH_MGID = [%s]...\n", __FUNCTION__, __LINE__, tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setauthmgidValue(void* value, struct DeviceNode*  deviceNode)
{
	tcapi_set(VOIPH248_COMMON, AUTH_MGID, (char*)value);

   // tcdbg_printf("[%s, %d] [SET]  AUTH_MGID = [%s]...\n", __FUNCTION__, __LINE__, value);
    
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}
int8 getauthmgidAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcTR104Attr.aAuthmgid;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setauthmgidAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8   result = 0;

	attribute = &tcTR104Attr.aAuthmgid;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}


int8 getauthHeaderValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	tcapi_get(VOIPH248_COMMON, AUTH_HEADER, tempValue);
    
   // tcdbg_printf("[%s, %d] [GET]  AUTH_HEADER = [%s]...\n", __FUNCTION__, __LINE__, tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setauthHeaderValue(void* value, struct DeviceNode*  deviceNode)
{
    uint32 iauthHeader;
	char strauthHeader[12] = {0};

	iauthHeader = *((uint32*)value);
    
	sprintf(strauthHeader, "%u", iauthHeader);
	tcapi_set(VOIPH248_COMMON, AUTH_HEADER, (char*)value);
    
   // tcdbg_printf("[%s, %d] [SET]  AUTH_HEADER = [%s]...\n", __FUNCTION__, __LINE__, value);

	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getauthHeaderAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcTR104Attr.aAuthHeader;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setauthHeaderAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8   result = 0;

	attribute = &tcTR104Attr.aAuthHeader;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getHeaderSecurityTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	tcapi_get(VOIPH248_COMMON, HEADER_SECURITY_TYPE, tempValue);
	if(strlen(tempValue) == 0){
		strcpy(tempValue,"md5");		
	}
    
  //  tcdbg_printf("[%s, %d] [GET]  HEADER_SECURITY_TYPE = [%s]...\n", __FUNCTION__, __LINE__, tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setHeaderSecurityTypeValue(void* value, struct DeviceNode*  deviceNode)
{
	tcapi_set(VOIPH248_COMMON, HEADER_SECURITY_TYPE, (char*)value);
    
  //  tcdbg_printf("[%s, %d] [SET]  HEADER_SECURITY_TYPE = [%s]...\n", __FUNCTION__, __LINE__, value);

	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getHeaderSecurityTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcTR104Attr.aHeaderSecurityType;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setHeaderSecurityTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8   result = 0;

	attribute = &tcTR104Attr.aHeaderSecurityType;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getRfc2833EncryptKeyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	tcapi_get(VOIPH248_COMMON, RFC2833_ENCRYPT_KEY, tempValue);
    
  //  tcdbg_printf("[%s, %d] [GET]  RFC2833_ENCRYPT_KEY = [%s]...\n", __FUNCTION__, __LINE__, tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setRfc2833EncryptKeyValue(void* value, struct DeviceNode*  deviceNode)
{
	tcapi_set(VOIPH248_COMMON, RFC2833_ENCRYPT_KEY, (char*)value);
    
  //  tcdbg_printf("[%s, %d] [SET]  RFC2833_ENCRYPT_KEY = [%s]...\n", __FUNCTION__, __LINE__, value);

	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getRfc2833EncryptKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcTR104Attr.aRfc2833EncryptKey;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setRfc2833EncryptKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8   result = 0;

	attribute = &tcTR104Attr.aRfc2833EncryptKey;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}


int8 getMsgFormatValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	tcapi_get(VOIPH248_COMMON, MSG_FORMAT, tempValue);
	if(strlen(tempValue) == 0){
		strcpy(tempValue,"Short");		
	}
        
   // tcdbg_printf("[%s, %d] [GET]  MSG_FORMAT = [%s]...\n", __FUNCTION__, __LINE__, tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
    
}
int8 setMsgFormatValue(void* value, struct DeviceNode*  deviceNode)
{
	tcapi_set(VOIPH248_COMMON, MSG_FORMAT, (char *)value);
    
 //   tcdbg_printf("[%s, %d] [SET]  MSG_FORMAT = [%s]...\n", __FUNCTION__, __LINE__, value);

	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getMsgFormatAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcTR104Attr.aRfc2833EncryptKey;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setMsgFormatAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8   result = 0;

	attribute = &tcTR104Attr.aRfc2833EncryptKey;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}


int8 getUdpAlfFlagValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};

	tcapi_get(VOIPH248_COMMON, UDP_ALF_GLAG, tempValue);
    
  //  tcdbg_printf("[%s, %d] [GET]  MSG_FORMAT = [%s]...\n", __FUNCTION__, __LINE__, tempValue);

	return getTheValue(value, type, deviceNode,tempValue);
}
int8 setUdpAlfFlagValue(void* value, struct DeviceNode*  deviceNode)
{
   	uint32 iUdpAlfFlag;
	char strUdpAlfFlag[12] = {0};

	iUdpAlfFlag = *((uint32*)value);
    
	sprintf(strUdpAlfFlag, "%u", iUdpAlfFlag);

	tcapi_set(VOIPH248_COMMON, UDP_ALF_GLAG, strUdpAlfFlag);
    
  //  tcdbg_printf("[%s, %d] [SET]  UDP_ALF_GLAG = [%s]...\n", __FUNCTION__, __LINE__, strUdpAlfFlag);
    
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getUdpAlfFlagAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aUdpAlfFlag;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setUdpAlfFlagAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aUdpAlfFlag;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getResponseAckCtrlValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};

	tcapi_get(VOIPH248_COMMON, THREE_HAND_SHAKE, tempValue);
    
  //  tcdbg_printf("[%s, %d] [GET]  MSG_FORMAT = [%s]...\n", __FUNCTION__, __LINE__, tempValue);

	return getTheValue(value, type, deviceNode,tempValue);
}
int8 setResponseAckCtrlValue(void* value, struct DeviceNode*  deviceNode)
{
   	uint32 iResponseAckCtrl;
	char strResponseAckCtrl[12] = {0};

	iResponseAckCtrl = *((uint32*)value);
    
	sprintf(strResponseAckCtrl, "%u", iResponseAckCtrl);

	tcapi_set(VOIPH248_COMMON, THREE_HAND_SHAKE, strResponseAckCtrl);
    
 //   tcdbg_printf("[%s, %d] [SET]  THREE_HAND_SHAKE = [%s]...\n", __FUNCTION__, __LINE__, strResponseAckCtrl);
    
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getResponseAckCtrlAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aThreeHandShake;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setResponseAckCtrlAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aThreeHandShake;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}


int8 getMaxreTransTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};

        
	tcapi_get(VOIPH248_COMMON, MAX_RETRANS_TIME, tempValue);
    
  //  tcdbg_printf("[%s, %d] [GET]  MAX_RETRANS_TIME = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setMaxreTransTimeValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iMaxreTransTimeMode;
	char strMaxreTransTime[32] = {0};

	iMaxreTransTimeMode = *((uint32*)value);
    
	sprintf(strMaxreTransTime, "%u", iMaxreTransTimeMode);
	tcapi_set(VOIPH248_COMMON, MAX_RETRANS_TIME, strMaxreTransTime);
    
   // tcdbg_printf("[%s, %d] [SET]  MAX_RETRANS_TIME = [%s]...\n", __FUNCTION__, __LINE__, strMaxreTransTime);
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getMaxreTransTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aMaxreTransTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setMaxreTransTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aMaxreTransTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getMinreTransTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};

        
	tcapi_get(VOIPH248_COMMON, MIN_RETRANS_TIME, tempValue);
    
   // tcdbg_printf("[%s, %d] [GET]  MIN_RETRANS_TIME = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setMinreTransTimeValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iMinreTransTime;
	char strMinreTransTime[32] = {0};

	iMinreTransTime = *((uint32*)value);
    
	sprintf(strMinreTransTime, "%u", iMinreTransTime);
	tcapi_set(VOIPH248_COMMON, MIN_RETRANS_TIME, strMinreTransTime);
    
  //  tcdbg_printf("[%s, %d] [SET]  MIN_RETRANS_TIME = [%s]...\n", __FUNCTION__, __LINE__, strMinreTransTime);
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}		
int8 getMinreTransTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aMinreTransTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setMinreTransTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aMinreTransTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getMGProvisionalRespTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};

        
	tcapi_get(VOIPH248_COMMON, MG_PROVISIONAL_RESP_TIME, tempValue);
    
  //  tcdbg_printf("[%s, %d] [GET]  MG_PROVISIONAL_RESP_TIME = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setMGProvisionalRespTimeValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iMGProvisionalRespTime;
	char strMGProvisionalRespTime[4] = {0};

	iMGProvisionalRespTime = *((uint32*)value);
    
	sprintf(strMGProvisionalRespTime, "%u", iMGProvisionalRespTime);
	tcapi_set(VOIPH248_COMMON, MG_PROVISIONAL_RESP_TIME, strMGProvisionalRespTime);
    
  //  tcdbg_printf("[%s, %d] [SET]  MG_PROVISIONAL_RESP_TIME = [%s]...\n", __FUNCTION__, __LINE__, strMGProvisionalRespTime);
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}
int8 getMGProvisionalRespTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aMGProvisionalRespTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMGProvisionalRespTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aMGProvisionalRespTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getMTUValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};

        
	tcapi_get(VOIPH248_COMMON, MTU_VALUE, tempValue);
    
  //  tcdbg_printf("[%s, %d] [GET]  MTU_VALUE = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setMTUValueValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iMTUValue;
	char strMTUValue[32] = {0};

	iMTUValue = *((uint32*)value);
    
	sprintf(strMTUValue, "%u", iMTUValue);
	tcapi_set(VOIPH248_COMMON, MTU_VALUE, strMTUValue);
    
  //  tcdbg_printf("[%s, %d] [SET]  MTU_VALUE = [%s]...\n", __FUNCTION__, __LINE__, strMTUValue);
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}
int8 getMTUValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aMTUValue;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMTUValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aMTUValue;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getMsgSegmentationValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};

	tcapi_get(VOIPH248_COMMON, MSG_SEGMENTATION, tempValue);
        
   // tcdbg_printf("[%s, %d] [GET]  MSG_SEGMENTATION = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setMsgSegmentationValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iMsgSegmentation;
	char strMsgSegmentation[32] = {0};

	iMsgSegmentation = *((uint32*)value);
    
	sprintf(strMsgSegmentation, "%u", iMsgSegmentation);
	tcapi_set(VOIPH248_COMMON, MSG_SEGMENTATION, strMsgSegmentation);
    
  //  tcdbg_printf("[%s, %d] [SET]  MSG_SEGMENTATION = [%s]...\n", __FUNCTION__, __LINE__, strMsgSegmentation);
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}
int8 getMsgSegmentationAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aMsgSegmentation;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMsgSegmentationAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aMsgSegmentation;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getHeartBeatRetransTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};

        
	tcapi_get(VOIPH248_COMMON, HEARTBEAT_RETRANS_TIMER, tempValue);
    
   // tcdbg_printf("[%s, %d] [GET]  HEARTBEAT_RETRANS_TIMER = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setHeartBeatRetransTimerValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iHeartBeatRetransTimer;
	char strHeartBeatRetransTimer[32] = {0};

	iHeartBeatRetransTimer = *((uint32*)value);
    
	sprintf(strHeartBeatRetransTimer, "%u", iHeartBeatRetransTimer);
	tcapi_set(VOIPH248_COMMON, HEARTBEAT_RETRANS_TIMER, strHeartBeatRetransTimer);
    
  //  tcdbg_printf("[%s, %d] [SET]  HEARTBEAT_RETRANS_TIMER = [%s]...\n", __FUNCTION__, __LINE__, strHeartBeatRetransTimer);
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}
int8 getHeartBeatRetransTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aHeartBeatRetransTimer;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setHeartBeatRetransTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aHeartBeatRetransTimer;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getMWDBaseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};

        
	tcapi_get(VOIPH248_COMMON, MWD_BASE_TIME, tempValue);
    
  //  tcdbg_printf("[%s, %d] [GET]  MWD_BASE_TIME = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setMWDBaseTimeValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iMWDBaseTime;
	char strMWDBaseTime[32] = {0};

	iMWDBaseTime = *((uint32*)value);
    
	sprintf(strMWDBaseTime, "%u", iMWDBaseTime);
	tcapi_set(VOIPH248_COMMON, MWD_BASE_TIME, strMWDBaseTime);
    
   // tcdbg_printf("[%s, %d] [SET]   MWD_BASE_TIME = [%s]...\n", __FUNCTION__, __LINE__, strMWDBaseTime);
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}
int8 getMWDBaseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aMWDBaseTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMWDBaseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aMWDBaseTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getMWDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};

	tcapi_get(VOIPH248_COMMON, MAX_START_DELAY, tempValue);
        
 //   tcdbg_printf("[%s, %d] [GET]  MAX_START_DELAY = [%s]...\n", __FUNCTION__, __LINE__, tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setMWDValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 iMWD;
	char strMWD[32] = {0};

	iMWD = *((uint32*)value);
    
	sprintf(strMWD, "%u", iMWD);
	tcapi_set(VOIPH248_COMMON, MAX_START_DELAY, strMWD);
    
  //  tcdbg_printf("[%s, %d] [SET]   MAX_START_DELAY = [%s]...\n", __FUNCTION__, __LINE__, strMWD);
	SetCfgNodeFlag(VOIPH248, -1);
	return 0;
}
int8 getMWDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aMWD;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMWDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aMWD;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getMgcSwitchMethodValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[257] = {0};
	tcapi_get(VOIPH248_COMMON, MGC_SWITCH_MATHOD, tempValue);
	if(strlen(tempValue) == 0){
		strcpy(tempValue,"SupportSwitch");		
	}
        
 //   tcdbg_printf("[%s, %d] [GET]  MGC_SWITCH_MATHOD = [%s]...\n", __FUNCTION__, __LINE__, tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setMgcSwitchMethodValue(void* value, struct DeviceNode*  deviceNode)
{
	tcapi_set(VOIPH248_COMMON, MGC_SWITCH_MATHOD, (char*)value);

  //  tcdbg_printf("[%s, %d] [SET]   MGC_SWITCH_MATHOD = [%s]...\n", __FUNCTION__, __LINE__, value);

	SetCfgNodeFlag(VOIPH248, -1 );
	return 0;
}		
int8 getMgcSwitchMethodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcTR104Attr.aMgcSwitchMethod;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setMgcSwitchMethodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8   result = 0;

	attribute = &tcTR104Attr.aMgcSwitchMethod;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}
#endif

#if defined(TCSUPPORT_CUC_VOIP_NODE)
int8 getEnableVoipValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPBasic_Common", "Enable", tempValue);

	if(strlen(tempValue) == 0){		//default is Disabled
		strcpy(tempValue,"Disabled");
	}

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setEnableVoipValue(void* value, struct DeviceNode*  deviceNode)
{	
	tcapi_set("VoIPBasic_Common", "Enable", (char*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	return 0;
}
int8 getEnableVoipAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aEnable[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setEnableVoipAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aEnable[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getResetValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "Reset", tempValue);

	if(!strcmp(tempValue, "true")){
		strcpy(tempValue, "1");
	}
	else if(!strcmp(tempValue, "false")){
		strcpy(tempValue, "0");
	}
	else{
		strcpy(tempValue, " ");
	}

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setResetValue(void* value, struct DeviceNode*  deviceNode)
{	
	char val[10] = {'\0'};
	uint8 Reset;
	
	Reset = *((uint8*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	
	if(Reset  == 1){
		strcpy(val, "true");
	}
	else if(Reset  == 0){
		strcpy(val, "false");
	}
	else{
		tc_printf("==> setResetValue: value should be  0 or 1.\n");
		return -1;
	}

	tcapi_set("VoIPDigitMap_Entry", "Reset", val);

	return 0;

}
int8 getResetAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aReset[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setResetAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aReset[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getNumberOfLinesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("InfoVoIP_Entry0", "NumberOfLines", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getNumberOfLinesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aNumberOfLines[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setNumberOfLinesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aNumberOfLines[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[65] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "Name", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setNameValue(void* value, struct DeviceNode*  deviceNode)
{	
	SetCfgNodeFlag( "VoIPDigitMap", -1 );

	tcapi_set("VoIPDigitMap_Entry", "Name", (char*)value);

	return 0;
}
int8 getNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aName[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aName[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getSignalingProtocolValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPAdvanced_Common", "SignalingProtocols", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setSignalingProtocolValue(void* value, struct DeviceNode*  deviceNode)
{		
       tcapi_set("VoIPAdvanced_Common", "SignalingProtocols", (char*)value);

	SetCfgNodeFlag( "VoIPAdvanced", -1 );

	return 0;
}
int8 getSignalingProtocolAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aSignalingProtocol[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSignalingProtocolAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aSignalingProtocol[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getMaxSessionsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "MaxSessions", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setMaxSessionsValue(void* value, struct DeviceNode*  deviceNode)
{	
	uint32 MaxSessions;
	char tempValue[10] = {0};

	MaxSessions = *((uint32*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	
	sprintf(tempValue, "%lu", MaxSessions);

	tcapi_set("VoIPDigitMap_Entry", "MaxSessions", tempValue);

	return 0;
}
int8 getMaxSessionsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aMaxSessions[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMaxSessionsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aMaxSessions[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getDTMFMethodValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPAdvanced_Common", "DTMFTransportMode", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setDTMFMethodValue(void* value, struct DeviceNode*  deviceNode)
{	
	tcapi_set("VoIPAdvanced_Common", "DTMFTransportMode", (char*)value);

	SetCfgNodeFlag( "VoIPAdvanced", -1 );
	
	return 0;
}
int8 getDTMFMethodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aDTMFMethod[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDTMFMethodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aDTMFMethod[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getDTMFMethodG711Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[65] = {0};
	
	tcapi_get("VoIPAdvanced_Common", "DTMFMethodG711", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setDTMFMethodG711Value(void* value, struct DeviceNode*  deviceNode)
{		
       tcapi_set("VoIPAdvanced_Common", "DTMFMethodG711", (char*)value);

	SetCfgNodeFlag( "VoIPAdvanced", -1 );

	return 0;
}
int8 getDTMFMethodG711Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aDTMFMethodG711[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDTMFMethodG711Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aDTMFMethodG711[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getX_CU_DigitMapMatchModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "DigitMapMatchMode", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setX_CU_DigitMapMatchModeValue(void* value, struct DeviceNode*  deviceNode)
{	
       tcapi_set("VoIPDigitMap_Entry", "DigitMapMatchMode", (char*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );

	return 0;
}
int8 getX_CU_DigitMapMatchModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aX_CU_DigitMapMatchMode[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_DigitMapMatchModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aX_CU_DigitMapMatchMode[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getX_CU_WANNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[33] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "WANName", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getX_CU_WANNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aX_CU_WANName[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_WANNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aX_CU_WANName[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getX_CU_InterfaceStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	
	tcapi_get("InfoVoIP_Entry0", "InterfaceState", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getX_CU_InterfaceStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aX_CU_InterfaceState[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_InterfaceStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aX_CU_InterfaceState[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}


int8 getServerType_CUValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "ServerType", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setServerType_CUValue(void* value, struct DeviceNode*  deviceNode)
{	
       tcapi_set("VoIPDigitMap_Entry", "ServerType", (char*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	
	return 0;
}
int8 getServerType_CUAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aSrverType_CU[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setServerType_CUAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aSrverType_CU[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getStartDigitTimer_CUValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "StartDigitTimer", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setStartDigitTimer_CUValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 tempValue;
	char strValue[10] = {0};

	tempValue = *((int32*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	
	if(tempValue <10 ||tempValue > 20){
		return -1;
	}
	sprintf(strValue, "%d", tempValue);

	tcapi_set("VoIPDigitMap_Entry", "StartDigitTimer", strValue);

	return 0;
}
int8 getStartDigitTimer_CUAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aStartDigitTimer_CU[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setStartDigitTimer_CUAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aStartDigitTimer_CU[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getInterDigitTimerShort_CUValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "InterDigitTimerShort", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);	
}
int8 setInterDigitTimerShort_CUValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 tempValue;
	char strValue[10] = {0};

	tempValue = *((int32*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	
	sprintf(strValue, "%d", tempValue);

	tcapi_set("VoIPDigitMap_Entry", "InterDigitTimerShort", strValue);

	return 0;
}	
int8 getInterDigitTimerShort_CUAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aInterDigitTimerShort_CU[instance];

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setInterDigitTimerShort_CUAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aInterDigitTimerShort_CU[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getInterDigitTimerLong_CUValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "InterDigitTimerLong", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);	
}
int8 setInterDigitTimerLong_CUValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 tempValue;
	char strValue[10] = {0};

	tempValue = *((int32*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	
	sprintf(strValue, "%d", tempValue);

	tcapi_set("VoIPDigitMap_Entry", "InterDigitTimerLong", strValue);

	return 0;
}
int8 getInterDigitTimerLong_CUAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aInterDigitTimerLong_CU[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setInterDigitTimerLong_CUAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aInterDigitTimerLong_CU[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getHangingReminderToneTimer_CUValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "HangingReminderToneTimer", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setHangingReminderToneTimer_CUValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 tempValue;
	char strValue[10] = {0};

	tempValue = *((int32*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	
	sprintf(strValue, "%d", tempValue);

	tcapi_set("VoIPDigitMap_Entry", "HangingReminderToneTimer", strValue);

	return 0;
}
int8 getHangingReminderToneTimer_CUAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aHangingReminderToneTimer_CU[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setHangingReminderToneTimer_CUAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aHangingReminderToneTimer_CU[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getBusyToneTimer_CUValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "BusyToneTimer", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);	
}
int8 setBusyToneTimer_CUValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 tempValue;
	char strValue[10] = {0};

	tempValue = *((int32*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	
	sprintf(strValue, "%d", tempValue);

	tcapi_set("VoIPDigitMap_Entry", "BusyToneTimer", strValue);

	return 0;
}
int8 getBusyToneTimer_CUAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aBusyToneTimer_CU[instance];

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setBusyToneTimer_CUAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aBusyToneTimer_CU[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getNoAnswerTimer_CUValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "NoAnswerTimer", tempValue);
	return getTheValue(value, type, deviceNode, tempValue);	
}
int8 setNoAnswerTimer_CUValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 tempValue;
	char strValue[10] = {0};

	tempValue = *((int32*)value);

	SetCfgNodeFlag( "VoIPDigitMap", -1 );
	
	sprintf(strValue, "%d", tempValue);

	tcapi_set("VoIPDigitMap_Entry", "NoAnswerTimer", strValue);

	return 0;
}	
int8 getNoAnswerTimer_CUAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aNoAnswerTimer_CU[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setNoAnswerTimer_CUAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aNoAnswerTimer_CU[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getTimerT1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPBasic_Common", "TimerT1", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setTimerT1Value(void* value, struct DeviceNode*  deviceNode)
{	
	uint32 TimerT1;
	char setValue[10] = {0};

	TimerT1= *((uint32*)value);

        setVoIPProtocolToIMSSIP();
	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	sprintf(setValue, "%lu", TimerT1);

	tcapi_set("VoIPBasic_Common", "TimerT1", setValue);
	
	return 0;
}
int8 getTimerT1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aTimerT1;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	
	return result;
}
int8 setTimerT1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	attribute = &tcTR104Attr.aTimerT1;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getTimerT2Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPBasic_Common", "TimerT2", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setTimerT2Value(void* value, struct DeviceNode*  deviceNode)
{	
	uint32 TimerT2;
	char setValue[10] = {0};

	TimerT2= *((uint32*)value);

        setVoIPProtocolToIMSSIP();
	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	sprintf(setValue, "%lu", TimerT2);

	tcapi_set("VoIPBasic_Common", "TimerT2", setValue);
	
	return 0;
}
int8 getTimerT2Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aTimerT2;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	
	return result;
}
int8 setTimerT2Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	attribute = &tcTR104Attr.aTimerT2;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getTimerT4Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPBasic_Common", "TimerT4", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setTimerT4Value(void* value, struct DeviceNode*  deviceNode)
{	
	uint32 TimerT4;
	char setValue[10] = {0};

	TimerT4= *((uint32*)value);

        setVoIPProtocolToIMSSIP();
	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	sprintf(setValue, "%lu", TimerT4);

	tcapi_set("VoIPBasic_Common", "TimerT4", setValue);
	
	return 0;
}
int8 getTimerT4Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aTimerT4;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	
	return result;
}
int8 setTimerT4Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	attribute = &tcTR104Attr.aTimerT4;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getUseCodecPriorityInSDPResponseValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPBasic_Common", "UseCodecPriorityInSDPResponse", tempValue);

	if(!strcmp(tempValue, "Yes")){
		strcpy(tempValue, "1");
	}
	else if(!strcmp(tempValue, "No") || !strlen(tempValue)){
		strcpy(tempValue, "0");
	}
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setUseCodecPriorityInSDPResponseValue(void* value, struct DeviceNode*  deviceNode)
{	
	uint8 val;
	char str[10] = {0};

	val = *((uint8*)value);

        setVoIPProtocolToIMSSIP();
	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	if(1 == val)
		strcpy(str, "Yes");
	else if(0 == val)
		strcpy(str, "No");
	else 
		return -1;
	
	tcapi_set("VoIPBasic_Common", "UseCodecPriorityInSDPResponse", str);

	return 0;
}
int8 getUseCodecPriorityInSDPResponseAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aUseCodecPriorityInSDPResponse;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	
	return result;
}
int8 setUseCodecPriorityInSDPResponseAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	attribute = &tcTR104Attr.aUseCodecPriorityInSDPResponse;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getSIPResponseNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};

       instance = atoi(deviceNode->parentNode->name) - 1;

	snprintf(nodeName, sizeof(nodeName), "VoIPBasic_Entry%d", instance);

	tcapi_get(nodeName, "SIPResponseNumber", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setSIPResponseNumberValue(void* value, struct DeviceNode*  deviceNode)
{	
	uint32 val;
	char setValue[10] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};

       instance = atoi(deviceNode->parentNode->name) - 1;

	snprintf(nodeName, sizeof(nodeName), "VoIPBasic_Entry%d", instance);

	val= *((uint32*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	sprintf(setValue, "%lu", val);

	tcapi_set(nodeName, "SIPResponseNumber", setValue);
	return 0;
}
int8 getSIPResponseNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
	
       instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aSIPResponseNumber[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	
	return result;
}
int8 setSIPResponseNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aSIPResponseNumber[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getToneValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};

       instance = atoi(deviceNode->parentNode->name) - 1;

	snprintf(nodeName, sizeof(nodeName), "VoIPBasic_Entry%d", instance);

	tcapi_get(nodeName, "Tone", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setToneValue(void* value, struct DeviceNode*  deviceNode)
{	
	uint32 val;
	char setValue[10] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};

       instance = atoi(deviceNode->parentNode->name) - 1;

	snprintf(nodeName, sizeof(nodeName), "VoIPBasic_Entry%d", instance);

	val= *((uint32*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	sprintf(setValue, "%lu", val);

	tcapi_set(nodeName, "Tone", setValue);
	
	return 0;
}
int8 getToneAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
	
       instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aTone[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	
	return result;
}
int8 setToneAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aTone[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getX_CU_DurationValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};

       instance = atoi(deviceNode->parentNode->name) - 1;

	snprintf(nodeName, sizeof(nodeName), "VoIPBasic_Entry%d", instance);

	tcapi_get(nodeName, "Duration", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setX_CU_DurationValue(void* value, struct DeviceNode*  deviceNode)
{	
	uint32 val;
	char setValue[10] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};

       instance = atoi(deviceNode->parentNode->name) - 1;

	snprintf(nodeName, sizeof(nodeName), "VoIPBasic_Entry%d", instance);

	val= *((uint32*)value);

	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	sprintf(setValue, "%lu", val);

	tcapi_set(nodeName, "Duration", setValue);
	
	return 0;
}
int8 getX_CU_DurationAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
	
       instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aX_CU_Duration[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	
	return result;
}
int8 setX_CU_DurationAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aX_CU_Duration[instance];
	
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int cwmpInitResponseMapFuncsSubTree( DeviceNodePtr deviceNode )
{
	int num = SIPRESPONSENUM, rtnCode = 0;

	rtnCode = RegisterDynNodeFromIndex( deviceNode, ResponseMapIndex, num );

	return rtnCode;
}
#endif


