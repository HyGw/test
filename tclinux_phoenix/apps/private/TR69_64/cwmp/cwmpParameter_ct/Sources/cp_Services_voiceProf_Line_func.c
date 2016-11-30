#include "Global_res.h"
#include "cp_Services_voiceProf_Line.h"

int	cwmpInitLineCodecListSubTree( DeviceNodePtr deviceNode )
{
	int num = CODECLIST_NUMBER, rtnCode = 0;

	rtnCode = RegisterDynNodeFromIndex( deviceNode, ListIndex, num );

	return rtnCode;
}

int8 getLineEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);

	tcapi_get(nodeName, "Enable", tempValue);
	
	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "Enabled");
	else
		strcpy(result, "Disabled");
	return getTheValue(value, type, deviceNode, result);
}

int8 setLineEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);

	SetCfgNodeFlag( "VoIPBasic", 0 );
	
	if(strcmp((char*)value, "Enabled") == 0)
	{
		strcpy(result, "Yes");
		tcapi_set(nodeName, "Enable", result);
	}
	else if(strcmp((char*)value, "Disabled") == 0)
	{
		strcpy(result, "No");
		tcapi_set(nodeName, "Enable", result);
	}else
		return -1;
	
#if defined(TCSUPPORT_CT_ITMS_TMOUT)
    cwmpDelayFunction();
#endif
	return 0;
}	
int8 getLineEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = tcTR104Attr.aLineEnable[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setLineEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aLineEnable[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

#if defined(TCSUPPORT_CT_PON_SC)
int8 getSubsUAValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	int sbscrb_type = 0;
	
	instance = atoi(deviceNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPAdvanced_Entry%d", instance);

	tcapi_get(nodeName, "SubscribeType", tempValue);
	sbscrb_type = atoi(tempValue);
	if(sbscrb_type & 0x01)
		strcpy(result, "true");
	else
		strcpy(result, "false");
	return getTheValue(value, type, deviceNode, result);
}

int8 setSubsUAValue(void* value, struct DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	int sbscrb_type = 0, ua_val = 0;
	char sbscrb_type_ch = '0';

	instance = atoi(deviceNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPAdvanced_Entry%d", instance);

	tcapi_get(nodeName, "SubscribeType", tempValue);
	sbscrb_type = atoi(tempValue);
	ua_val = *((uint8*)value);

	if(ua_val == 1)
	{
		sbscrb_type |= 0x1;
	}
	else if(ua_val == 0)
	{
		sbscrb_type &= 0xe;
	}else{
	    tc_printf("==> setSubsUAValue: value should be  0 or 1.\n");
		return -1;
	}
	sbscrb_type_ch = '0' + sbscrb_type;
	sprintf(tempValue,"%c",sbscrb_type_ch);
	tcapi_set(nodeName, "SubscribeType", tempValue);
	
	SetCfgNodeFlag( "VoIPAdvanced", instance );
	return 0;
}	

int8 getSubsUAAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = tcTR104Attr.aSubsUA[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setSubsUAAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aSubsUA[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}


int8 getSubsRegValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
		char tempValue[10] = {0};
		char result[10] = {0};
		char nodeName[34] = {'\0'};
		int instance = 0;
		int sbscrb_type = 0;
		
		instance = atoi(deviceNode->parentNode->name)-1;
		sprintf(nodeName, "VoIPAdvanced_Entry%d", instance);
	
		tcapi_get(nodeName, "SubscribeType", tempValue);
		sbscrb_type = atoi(tempValue);
		if(sbscrb_type & 0x02)
			strcpy(result, "true");
		else
			strcpy(result, "false");
		return getTheValue(value, type, deviceNode, result);
}


int8 setSubsRegValue(void* value, struct DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	int sbscrb_type = 0, reg_val = 0;
	char sbscrb_type_ch = '0';

	instance = atoi(deviceNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPAdvanced_Entry%d", instance);

	tcapi_get(nodeName, "SubscribeType", tempValue);
	sbscrb_type = atoi(tempValue);
	reg_val = *((uint8*)value);

	if(reg_val == 1)
	{
		sbscrb_type |= 0x2;
	}
	else if(reg_val == 0)
	{
		sbscrb_type &= 0xd;
	}else{
	    tc_printf("==> setSubsUAValue: value should be  0 or 1.\n");
		return -1;
	}
	sbscrb_type_ch = '0' + sbscrb_type;
	sprintf(tempValue,"%c",sbscrb_type_ch);
	tcapi_set(nodeName, "SubscribeType", tempValue);
	
	SetCfgNodeFlag( "VoIPAdvanced", instance );
	return 0;
}	
int8 getSubsRegAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = tcTR104Attr.aSubsReg[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setSubsRegAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aSubsReg[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}
#endif

int8 getLineSIPAuthUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[129] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);

	tcapi_get(nodeName, "SIPAuthenticationName", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 setLineSIPAuthUserNameValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	int instance = 0;
	char temp[64]={0};
	char *ptr=NULL;

        setVoIPProtocolToIMSSIP();
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);

	tcapi_set(nodeName, "SIPAuthenticationName", (char*)value);

    /*  register account 20150211 */
	if(0/*isImssipEnable()==0*/){
		
		tcapi_set(nodeName, "SIPDisplayName", (char*)value);
	}else{
		ptr=strstr((char*)value,"@");
		if(ptr!=NULL){
			sscanf((char*)value,"%[^@]",temp);
		}else{
			strncpy(temp,(char*)value,64);
		}
		tcapi_set(nodeName, "SIPDisplayName", temp);
	}
    
	SetCfgNodeFlag( "VoIPBasic", instance);
	
	return 0;
}		
int8 getLineSIPAuthUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aLineSIPAuthUserNameaLineEnable[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setLineSIPAuthUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aLineSIPAuthUserNameaLineEnable[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}		

/*[OSBNB00040562]ADD by peter.jiang@20141015, enable password encryption*/
#if defined(TCSUPPORT_CT_VOIP_CRYPT)
/*____________________________________________________________________*
**      function name: privSymmCrypt
**
**      description:
*       private symmetrical crypt.
*
**      parameters:
*     text: text input, may be encoded or decodec.
*     crypt_out: text out after crypting
*     
*     ret:crypt_out
*
**      global:
*     none
**      return:
*     none
**      call:
*     tcWebApi_set
**      revision:
* _____________________________________________________________________*/
static char*
privSymmCrypt (const char* text, char* crypt_out)
{
    char PRIVATE_KEY[2];
    int i = 0;
    int len = 0;

    if (text == NULL || strlen(text) == 0 || crypt_out == NULL) {
        tcdbg_printf ("[ASP] privSymmCrypt: invalid input\n");
        return NULL;
    }

    /*In order to make crypt_out characters printable(<0x80), private key should be <0x80,
    **and not be confict with text, e.g., 'Z'^'Z' = 0, this will truncate crypt_out,
    **set private keys as 0x12, 0x13(<0x20). 
    */
    PRIVATE_KEY[0] = 0x12;
    PRIVATE_KEY[1] = 0x13;

    len = strlen(text);
    if (len%2)
        len--;

    for (i = 0; i < len; i+=2) {
        crypt_out[i] = text[i] ^ PRIVATE_KEY[0];
        crypt_out[i+1] = text[i+1] ^ PRIVATE_KEY[1];
    }

    if (strlen(text)%2) { /*the last byte*/
        crypt_out[i] = text[i] ^ PRIVATE_KEY[0];
    }
/*
    tcdbg_printf("\n+++++++++++++++(ASP text)++++++++++++++++++\n");
    for (i = 0; i < strlen(text); i++)
        tcdbg_printf("%2x ", text[i]);
    tcdbg_printf("\n+++++++++++++(ASP crypt_out)+++++++++++++++\n");
    for (i = 0; i < strlen(crypt_out); i++)
        tcdbg_printf("%2x ", crypt_out[i]);
    tcdbg_printf("\n++++++++++++++++++++++++++++++++++++++++++\n");
*/
    return crypt_out;
}
#endif

int8 getLineSIPAuthPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[129] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	tcapi_get(nodeName, "SIPPassword", tempValue);

/*[OSBNB00040562]ADD by peter.jiang@20141015, enable password encryption*/
#if defined(TCSUPPORT_CT_VOIP_CRYPT)
	{
		char crypt[129] = {0};

		privSymmCrypt(tempValue, crypt);
		memcpy (tempValue, crypt, strlen(tempValue)+1);	
	}
#endif

#if defined(TCSUPPORT_CT_PON_GD)
	if (strlen(tempValue) > 0)
	strcpy(tempValue, "******");
#endif

	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setLineSIPAuthPasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	int instance = 0;

	setVoIPProtocolToIMSSIP();
/*[OSBNB00040562]ADD by peter.jiang@20141015, enable password encryption*/
#if defined(TCSUPPORT_CT_VOIP_CRYPT)
	{
		char crypt[129] = {0};	
	
		privSymmCrypt((char *)value, crypt);
		memcpy ((char *)value, crypt, strlen(value)+1); 
	}
#endif

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);

	tcapi_set(nodeName, "SIPPassword", (char*)value);

	SetCfgNodeFlag( "VoIPBasic", instance);
	
	return 0;
}		
int8 getLineSIPAuthPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aLineSIPAuthPasswordaLineEnable[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setLineSIPAuthPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aLineSIPAuthPasswordaLineEnable[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getLineSIPAuthURIValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[390] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	tcapi_get(nodeName, "SIPDisplayName", tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setLineSIPAuthURIValue(void* value, struct DeviceNode*  deviceNode)
{
	int iURILen=0;
	char nodeName[34] = {'\0'};
	int instance = 0;

	setVoIPProtocolToIMSSIP();	
	iURILen = strlen((char *)value);	
	if(iURILen > 389)
		return -1;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	tcapi_set(nodeName, "SIPDisplayName", (char*)value);

	SetCfgNodeFlag( "VoIPBasic", instance);
	
	return 0;
}		
int8 getLineSIPAuthURIAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aLineSIPAuthURI[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setLineSIPAuthURIAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aLineSIPAuthURI[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

#if defined(TCSUPPORT_CT)
int8 getPBXPrefixEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	tcapi_get("VoIPDigitMap_Entry", "PBXPrefixEnable", tempValue);

	if(!strcmp(tempValue, "1")){
		strcpy(tempValue, "1");
	}
	else {
		strcpy(tempValue, "0");
	}

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setPBXPrefixEnableValue(void* value, struct DeviceNode*  deviceNode)
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

	tcapi_set("VoIPDigitMap_Entry", "PBXPrefixEnable", enable);

	return 0;
}	
int8 getPBXPrefixEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aPBXPrefixEnable;

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setPBXPrefixEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aPBXPrefixEnable;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getPBXPrefixValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[513] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "PBXPrefix", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setPBXPrefixValue(void* value, struct DeviceNode*  deviceNode)
{

	SetCfgNodeFlag( "VoIPDigitMap", -1 );	

	tcapi_set("VoIPDigitMap_Entry", "PBXPrefix", value);

	return 0;
}	
int8 getPBXPrefixAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aPBXPrefix;

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setPBXPrefixAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aPBXPrefix;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getPBXDialSecondaryEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("VoIPDigitMap_Entry", "PBXDialSecondaryEnable", tempValue);

	if(!strcmp(tempValue, "1")){
		strcpy(tempValue, "1");
	}
	else {
		strcpy(tempValue, "0");
	}

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setPBXDialSecondaryEnableValue(void* value, struct DeviceNode*  deviceNode)
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

	tcapi_set("VoIPDigitMap_Entry", "PBXDialSecondaryEnable", enable);

	return 0;
}	
int8 getPBXDialSecondaryEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aPBXDialSecondaryEnable;

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setPBXDialSecondaryEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aPBXDialSecondaryEnable;

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getLineH248PhysicalTermIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[256] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPH248_Entry%d", instance);

	tcapi_get(nodeName, "PhysicalTermID", tempValue);
	
	
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 setLineH248PhysicalTermIDValue(void* value, struct DeviceNode*  deviceNode)
{
    char nodeName[256] = {'\0'};
    int instance = 0;

    setVoIPProtocolToH248();
    instance = atoi(deviceNode->parentNode->parentNode->name)-1;
    sprintf(nodeName, "VoIPH248_Entry%d", instance);
    
    tcapi_set(nodeName, "PhysicalTermID", (char*)value);

    SetCfgNodeFlag( "VoIPH248", instance);
	
	return 0;
}		
int8 getLineH248PhysicalTermIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;

	attribute = tcTR104Attr.aLineH248PhysicalTermID;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setLineH248PhysicalTermIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;

	attribute = &tcTR104Attr.aLineH248PhysicalTermID;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}
#endif


int8 getEchoCancellationEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {'\0'};
	char result[5] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPAdvanced_Entry%d", instance);
	
	tcapi_get(nodeName, "EchoCancellationEnable", tempValue);

	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	
	return getTheValue(value, type, deviceNode, result);
}		
int8 setEchoCancellationEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 iEnable;
	char strEnable[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPAdvanced_Entry%d", instance);

	iEnable = *((uint8*)value);
	//do not support disable echoCancellation now
	if(1 == iEnable)
		strcpy(strEnable, "Yes");
	else
		return -1;
	tcapi_set(nodeName, "EchoCancellationEnable", strEnable);

	SetCfgNodeFlag( "VoIPMedia", -1 );
	
	return 0;
}		
int8 getEchoCancellationEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aEchoCancellationEnable[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setEchoCancellationEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aEchoCancellationEnable[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getEchoCancellationInUseValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[5] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPAdvanced_Entry%d", instance);

	tcapi_get(nodeName, "EchoCancellationInUse", tempValue);
	
	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	
	return getTheValue(value, type, deviceNode, result);
}		
int8 setEchoCancellationInUseValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 iEchoCancellationInUse = 0;
	char strEnable[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPAdvanced_Entry%d", instance);

	iEchoCancellationInUse = *((uint8*)value);
	if(iEchoCancellationInUse != 0 && iEchoCancellationInUse != 1)
		return -1 ;
	
	if(1 == iEchoCancellationInUse)
		strcpy(strEnable, "Yes");
	else
		return -1;
	tcapi_set(nodeName, "EchoCancellationInUse", strEnable);

	SetCfgNodeFlag( "VoIPMedia", -1 );
	
	return 0;
	return 0;
}		
int8 getEchoCancellationInUseAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aEchoCancellationInUse[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setEchoCancellationInUseAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aEchoCancellationInUse[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getCodecValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30] = {0};
#if defined(TCSUPPORT_CT)
	int line=0;
#endif
	instance = atoi(deviceNode->parentNode->name);	

	if(instance < 0 || instance > CODECLIST_NUMBER)
		return -1;
#if defined(TCSUPPORT_CT)
	line = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	sprintf(nodeName, "VoIPCodecs_PVC%d_Entry%d", line-1, instance-1); 
#else
	sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
#endif

	tcapi_get(nodeName, "codec", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 setCodecValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance;
	char nodeName[30] = {0};
#if defined(TCSUPPORT_CT)	
	int line=0;
#endif

	instance = atoi(deviceNode->parentNode->name);	
	if(instance < 0 || instance > CODECLIST_NUMBER)
		return -1;
	
#if defined(TCSUPPORT_CT)	
	line = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	sprintf(nodeName, "VoIPCodecs_PVC%d_Entry%d", line-1, instance-1); 
#else
	sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
#endif
	
	tcapi_set(nodeName, "codec", (char*)value);	

	SetCfgNodeFlag( "VoIPCodecs", -1 );
	
	return 0;
}

int8 getCodecAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcTR104Attr.aCodec[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setCodecAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcTR104Attr.aCodec[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getPacketizationPeriodValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	int i;
	char tempValue[65] = {0};
	char codec_List[20] = {0};
	char codec_Entry[20] = {0};
	char nodeName[30] = {0};
	int inEntryFlag = 0;
#if defined(TCSUPPORT_CT)	
	int line=0;
#endif

	instance = atoi(deviceNode->parentNode->name);	

	if(instance < 0 || instance > CODECLIST_NUMBER)
		return -1;
#if defined(TCSUPPORT_CT)	
	line = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	sprintf(nodeName, "VoIPCodecs_PVC%d", line-1);
#else
	sprintf(nodeName, "VoIPCodecs_Entry%d",  instance-1); 	
#endif

	tcapi_get(nodeName, "SIPPacketizationTime", tempValue);
#if 0
	for(i=0; i<CODECENTRY_NUMBER; i++)
	{
		sprintf(nodeName, "VoIPMedia_Entry%d", i);
		tcapi_get(nodeName, "SIPSupportedCodecs", codec_Entry);
		
		if(strcmp(codec_List, codec_Entry) == 0)
		{	
			tcapi_get(nodeName, "SIPPacketizationTime", tempValue);
			inEntryFlag = 1;
			break;
		}
	}
	
	if(0 == inEntryFlag)
	{
		sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
		tcapi_get(nodeName, "SIPPacketizationTime", tempValue);
	}
#endif		
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 setPacketizationPeriodValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance;
	int i;
	char codec_List[20] = {0};
	char codec_Entry[20] = {0};
	char nodeName[30] = {0};
#if defined(TCSUPPORT_CT)	
	int line=0;
#endif

	instance = atoi(deviceNode->parentNode->name);	
	if(instance < 0 || instance > CODECLIST_NUMBER)
		return -1;
#if defined(TCSUPPORT_CT)	
	line = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	sprintf(nodeName, "VoIPCodecs_PVC%d", line-1); 
#else
	sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
#endif
	//tcapi_get(nodeName, "codec", codec_List);
	
	tcapi_set(nodeName, "SIPPacketizationTime", (char*)value);	
#if 0
	for(i=0; i<CODECENTRY_NUMBER; i++)
	{
		sprintf(nodeName, "VoIPMedia_Entry%d", i);
		tcapi_get(nodeName, "SIPSupportedCodecs", codec_Entry);
		
		if(strcmp(codec_List, codec_Entry) == 0)
		{	
			tcapi_set(nodeName, "SIPPacketizationTime", (char*)value);
			break;
		}
	}

	SetCfgNodeFlag( "VoIPMedia", -1 );

#endif
	SetCfgNodeFlag( "VoIPCodecs", -1 );

	return 0;
}	
int8 getPacketizationPeriodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcTR104Attr.aPacketizationPeriod[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setPacketizationPeriodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcTR104Attr.aPacketizationPeriod[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getPriorityValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	int i;
	char tempValue[5] = {0};
	char codec_List[20] = {0};
	char codec_Entry[20] = {0};
	char nodeName[30] = {0};
	int inEntryFlag = 0;
#if defined(TCSUPPORT_CT)
	int line=0;
#endif

	instance = atoi(deviceNode->parentNode->name);	

	if(instance < 0 || instance > CODECLIST_NUMBER)
		return -1;
#if 0
	sprintf(nodeName, "VoIPCodecs_Entry%d",  instance-1); 
	tcapi_get(nodeName, "codec", codec_List);
	
	for(i=0; i<CODECENTRY_NUMBER; i++)
	{
		sprintf(nodeName, "VoIPMedia_Entry%d", i);
		tcapi_get(nodeName, "SIPSupportedCodecs", codec_Entry);
		
		if(strcmp(codec_List, codec_Entry) == 0)
		{	
			sprintf(tempValue, "%d", i+1); 
			inEntryFlag = 1;
			break;
		}
	}
	if(0 == inEntryFlag)
#endif
	{
#if defined(TCSUPPORT_CT)	
		line = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
		sprintf(nodeName, "VoIPCodecs_PVC%d_Entry%d", line-1, instance-1); 
#else
		sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
#endif
		tcapi_get(nodeName, "priority", tempValue);
	}
		
	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 setPriorityValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance;
	int i;
	char codec_List[20] = {0};
	char period_List[20] = {0};
	char codec_Entry[20] = {0};
	char nodeName[30] = {0};
	int inEntryFlag = 0;
	uint32 priority;
	char strPriority[10] = {0};
#if defined(TCSUPPORT_CT)
	int line=0;
#endif

	instance = atoi(deviceNode->parentNode->name);	
	if(instance < 0 || instance > CODECLIST_NUMBER)
		return -1;

	priority = *((uint32*)value);
	sprintf(strPriority, "%lu", priority);
	//tcdbg_printf("\n int priority value is %lu***\n", priority);
#if defined(TCSUPPORT_CT)	
	line = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	sprintf(nodeName, "VoIPCodecs_PVC%d_Entry%d", line-1, instance-1); 
#else
	sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
#endif


//	tcapi_get(nodeName, "codec", codec_List);
//	tcapi_get(nodeName, "SIPPacketizationTime", period_List);
	//tcdbg_printf("\n codec_List value is %s***\n", codec_List);

	SetCfgNodeFlag( "VoIPCodecs", -1 );
       tcapi_set(nodeName, "priority", strPriority);
			
#if 0
	SetCfgNodeFlag( "VoIPMedia", -1 );
	
	if(0 < priority  && priority < 5)
	{
		sprintf(nodeName, "VoIPMedia_Entry%d", priority-1);
		tcapi_get(nodeName, "SIPSupportedCodecs", codec_Entry);
		//tcdbg_printf("\n codec_Entry value is %s***\n", codec_Entry);
		if(strlen(codec_Entry) != 0) /* the entry already has codec */
		{
			if(strcmp(codec_Entry, codec_List) != 0)
			{
				return -1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			//set the codec item to VoIPMedia_Entry[priority-1]
			//tcdbg_printf("\n ***the entry is empty***\n");
			tcapi_set(nodeName, "SIPSupportedCodecs", codec_List);
			tcapi_set(nodeName, "SIPPacketizationTime", period_List);
			
			sprintf(nodeName, "VoIPCodecs_Entry%d",  instance-1); 
			tcapi_set(nodeName, "priority", strPriority);
		}
		
	}
	else
	{
		for(i=0; i<CODECENTRY_NUMBER; i++)
		{
			sprintf(nodeName, "VoIPMedia_Entry%d", i);
			tcapi_get(nodeName, "SIPSupportedCodecs", codec_Entry);
			tcdbg_printf("\n codec_Entry value is %s***\n", codec_Entry);
			
			if(strcmp(codec_List, codec_Entry) == 0)
			{	
				tcapi_unset(nodeName);
				break;
			}
		}
		sprintf(nodeName, "VoIPCodecs_Entry%d",  instance-1); 
		tcapi_set(nodeName, "priority", strPriority);
	
	}	
#endif

	return 0;
}	
int8 getPriorityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcTR104Attr.aPriority[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setPriorityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcTR104Attr.aPriority[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

#if defined(TCSUPPORT_CT_PON_SC)
static uint32 lineErrorStatusReasonGet(int instance)
{
	char   tempValue[16] = {0};
	uint32 tempU32 = 0;    
	char nodeName[34] = {'\0'};

	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	tcapi_get(nodeName, "RegFailReason", tempValue);

	tempU32 = atoi(tempValue);

	return tempU32;
}
#endif

int8 getLineStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
    char tempValue[32] = {0};
    char nodeName[34] = {'\0'};
    int instance = 0;
    uint32 tempU32 = 0;
    int  iStatus = 0;
    char tempVar[12] = {0};
    char statusStr[][32] = {"Up", "Initializing", "Registering", "Unregistering",
        "Error", "Testing", "Quiescent", "Disabled"};
    
    instance = atoi(deviceNode->parentNode->name)-1;

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)  
    if(getSipStatus_tr69() == 0){
    	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
    	tcapi_get(nodeName, "Enable", tempValue);
        if(strcmp(tempValue,"Yes")==0){//line enable
            memset(nodeName,0,sizeof(nodeName));
            memset(tempValue,0,sizeof(tempValue));
            sprintf(nodeName, "InfoVoIP_Entry%d", instance);
        			
            tcapi_get(nodeName, "Status", tempValue);	
#if defined(TCSUPPORT_CT_PON_SC)
            if (!strcmp(tempValue, "Error")){
                tempU32 = lineErrorStatusReasonGet(instance);
                if (tempU32 == 2 || tempU32 == 3){
                    strncpy(tempValue, "SendFailed", sizeof(tempValue)-1);
                }else if (tempU32 == 4){
                    strncpy(tempValue, "AuthFailed", sizeof(tempValue)-1);
                }
            }
#endif
        }else{
            strcpy(tempValue,"Disabled");
        }
    }else if(getSipStatus_tr69() == 1){		/*H.248*/
        sprintf(nodeName, "VoIPH248_Entry%d", instance);
        tcapi_get(nodeName, "UserServiceState", tempVar);
        iStatus = atoi(tempVar);
        if (iStatus == 1 || iStatus == 3 || iStatus == 5 || iStatus == 8){
            strcpy(tempValue, statusStr[iStatus -1]);
        }else{
            strcpy(tempValue, statusStr[4]);
        } 
    }
#else
    sprintf(nodeName, "VoIPBasic_Entry%d", instance);
    tcapi_get(nodeName, "Enable", tempValue);
    if(strcmp(tempValue,"Yes")==0){//line enable
        memset(nodeName,0,sizeof(nodeName));
        memset(tempValue,0,sizeof(tempValue));
        sprintf(nodeName, "InfoVoIP_Entry%d", instance);
    			
        			
        tcapi_get(nodeName, "Status", tempValue);	
#if defined(TCSUPPORT_CT_PON_SC)
        if (!strcmp(tempValue, "Error")){
            tempU32 = lineErrorStatusReasonGet(instance);
            if (tempU32 == 2 || tempU32 == 3){
                strncpy(tempValue, "SendFailed", sizeof(tempValue)-1);
            }else if (tempU32 == 4){
                strncpy(tempValue, "AuthFailed", sizeof(tempValue)-1);
            }
        }
#endif
    }else{
        strcpy(tempValue,"Disabled");
    }
#endif

	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 getLineStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name)-1;

	attribute = tcTR104Attr.aLineStatus[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setLineStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aLineStatus[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

#if defined(TCSUPPORT_CT)
int8 getLineLastRegisterErrorReasonValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
    char   tempValue[16] = {0};
    uint32 tempU32 = 0;    
    char   nodeName[34] = {'\0'}; 
    int    instance = 0;

    instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	
    if((instance < 0) || (instance > (LINENUM-1)))
    {
        return -1;
    }

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)  
    if(getSipStatus_tr69() == 0){
        sprintf(nodeName, "InfoVoIP_Entry%d", instance);
    }else{
        sprintf(nodeName, "InfoVoIPH248_Entry%d", instance);
    }
    tcapi_get(nodeName, "RegFailReason", tempValue);
    tempU32= atoi(tempValue);
#endif

    memset(tempValue,0,sizeof(tempValue));
    sprintf(tempValue,"%d",tempU32);	

    return getTheValue(value, type, deviceNode, tempValue);
	
}
int8 setLineLastRegisterErrorReasonValue(void* value, struct DeviceNode*  deviceNode)
{

    char   tempValue[16] = {0};   
    char   nodeName[34] = {'\0'};
    int    valueInt = 0;
    int    instance = 0;

    instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	
    if((instance < 0) || (instance > (LINENUM-1)))
    {
        return -1;
    }

    valueInt = atoi(value);
	
    if(valueInt < 0 | valueInt > 5)
        return -1;

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)  
    if(getSipStatus_tr69() == 0){
        sprintf(nodeName, "InfoVoIP_Entry%d", instance);
    }else{
        sprintf(nodeName, "InfoVoIPH248_Entry%d", instance);
    }    
#endif

    sprintf(tempValue, "%d", valueInt);
    tcapi_set(nodeName, "RegFailReason", tempValue);
	
    return 0;
}

int8 getLineLastRegisterErrorReasonAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;	
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	
	attribute = tcTR104Attr.aLastRegisterErrorReason[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setLineLastRegisterErrorReasonAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	
	attribute = &tcTR104Attr.aLastRegisterErrorReason[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;

}
#endif

int8 getdial_tone_patternValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPIMS_Entry%d", instance);

	tcapi_get(nodeName, "dial-tone-pattern", tempValue);	
		
	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 getdial_tone_patternAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;

	attribute = tcTR104Attr.adial_tone_pattern[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setdial_tone_patternAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.adial_tone_pattern[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getmcid_serviceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPIMS_Entry%d", instance);
			
	tcapi_get(nodeName, "mcid-service", tempValue);	
		
	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 getmcid_serviceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.amcid_service[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setmcid_serviceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.amcid_service[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}


int8 getno_dialing_behaviourValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPIMS_Entry%d", instance);
			
	tcapi_get(nodeName, "no-dialing-behaviour", tempValue);	
		
	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 getno_dialing_behaviourAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.ano_dialing_behaviour[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setno_dialing_behaviourAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.ano_dialing_behaviour[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 gethold_serviceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPIMS_Entry%d", instance);
			
	tcapi_get(nodeName, "hold-service", tempValue);	
		
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 gethold_serviceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.ahold_service[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 sethold_serviceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.ahold_service[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getthree_party_serviceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPIMS_Entry%d", instance);
		
	tcapi_get(nodeName, "three-party-service", tempValue);	
	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 getthree_party_serviceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.athree_party_service[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setthree_party_serviceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.athree_party_service[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getconference_uriValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[34] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
			
#ifdef TCSUPPORT_PON_ROSTELECOM
	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance); 	
	tcapi_get(nodeName, "SIP3wayConfServAddr", tempValue);
#else
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPIMS_Entry%d", instance);		
	tcapi_get(nodeName, "conference-uri", tempValue);	
#endif
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setconference_uriValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance;
	char nodeName[30] = {0};
	uint32 uri;
	char struri[34] = {0};
#ifdef TCSUPPORT_PON_ROSTELECOM
	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;	
#else
    instance = atoi(deviceNode->parentNode->parentNode->name)-1;	
#endif
	if(instance < 0 || instance > CODECLIST_NUMBER)
		return -1;

#if 1
	strncpy(struri, value, sizeof(struri) - 1);
#else
	uri= *((uint32*)value);
	sprintf(struri, "%lu", uri);
#endif
#ifdef TCSUPPORT_PON_ROSTELECOM
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);	
	SetCfgNodeFlag( "VoIPCallCtrl", -1 );
	tcapi_set(nodeName, "SIP3wayConfServAddr", struri);
#else
	sprintf(nodeName, "VoIPIMS_Entry%d",  instance); 
	SetCfgNodeFlag( "VoIPIMS", -1 );

	tcapi_set(nodeName, "conference-uri", struri);
#endif

	return 0;
}
int8 getconference_uriAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
#ifdef TCSUPPORT_PON_ROSTELECOM
	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;	
#else
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif
	attribute = tcTR104Attr.aconference_uri[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setconference_uriAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
#ifdef TCSUPPORT_PON_ROSTELECOM
	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;	
#else
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif
	attribute = &tcTR104Attr.aconference_uri[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 gethotline_uriValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPIMS_Entry%d", instance);
		
	tcapi_get(nodeName, "hotline-uri", tempValue);	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 sethotline_uriValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 val;
	char str[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPIMS_Entry%d", instance);

	SetCfgNodeFlag( "VoIPIMS", instance);
	
	tcapi_set(nodeName, "hotline-uri", (char *)value);

	return 0;
}
int8 gethotline_uriAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.ahotline_uri[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 sethotline_uriAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.ahotline_uri[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getcw_serviceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPIMS_Entry%d", instance);
			
	tcapi_get(nodeName, "cw-service", tempValue);	
		
	return getTheValue(value, type, deviceNode, tempValue);
}		
int8 getcw_serviceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.acw_service[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setcw_serviceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.acw_service[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}


#if CHINA_UNICOM_ENABLE
int8 getLineCodecListEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	int i;
	char tempValue[5] = {0};
	char codec_List[20] = {0};
	char codec_Entry[20] = {0};
	char nodeName[30] = {0};
	int inEntryFlag = 0;
#if defined(TCSUPPORT_CT)
	int line=0;
#endif
	
	instance = atoi(deviceNode->parentNode->name);	
	
	if(instance < 0 || instance > CODECLIST_NUMBER)
		return -1;

		{
#if defined(TCSUPPORT_CT)	
		line = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
		sprintf(nodeName, "VoIPCodecs_PVC%d_Entry%d", line-1, instance-1); 
#else
		sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
#endif
		tcapi_get(nodeName, "Enable", tempValue);
		}
			
		return getTheValue(value, type, deviceNode, tempValue);

}



int8 setLineCodecListEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance_enable = 0;
	int instance_priority = 0;
	char tempValue_priority[5] = {0};

#if defined(TCSUPPORT_CT)
	int line_priority=0;
#endif

	instance_priority = atoi(deviceNode->parentNode->name);	

	if(instance_priority < 0 || instance_priority > CODECLIST_NUMBER)
		return -1;

	{
#if defined(TCSUPPORT_CT)	
		line_priority = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
		sprintf(nodeName, "VoIPCodecs_PVC%d_Entry%d", line_priority-1, instance_priority-1); 
#else
		sprintf(nodeName, "VoIPCodecs_Entry%d", instance_priority-1); 
#endif
		tcapi_get(nodeName, "priority", tempValue_priority);
	}


#if defined(TCSUPPORT_CT)
	int line_enable=0;
#endif

	instance_enable = atoi(deviceNode->parentNode->name);	
	if(instance_enable < 0 || instance_enable > CODECLIST_NUMBER)
		return -1;

#if defined(TCSUPPORT_CT)	
	line_enable = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	sprintf(nodeName, "VoIPCodecs_PVC%d_Entry%d", line_enable-1, instance_enable-1); 
#else
	sprintf(nodeName, "VoIPCodecs_Entry%d", instance_enable-1); 
#endif

if(strcmp(tempValue_priority,"0")==0)
strcpy((char*)value,"Disabled");
else
strcpy((char*)value,"Enabled");


if(strcmp((char*)value, "Enabled") == 0)
	{
		strcpy(result, "Yes");
		tcapi_set(nodeName, "Enable", result);
	}
	else if(strcmp((char*)value, "Disabled") == 0)
	{
		strcpy(result, "No");
		tcapi_set(nodeName, "Enable", result);
	}else
		return -1;
	
	return 0;

}


int8 getLineCodecListEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcTR104Attr.LineCodecListEnable[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLineCodecListEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcTR104Attr.LineCodecListEnable[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}

#endif

int8 getEntryIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0} ;
	char nodeName[30] = {0};
#if defined(TCSUPPORT_CT)
	int line=0;	
#endif

	instance = atoi(deviceNode->parentNode->name);	

	if(instance < 0 || instance > CODECLIST_NUMBER)
		return -1;
#if defined(TCSUPPORT_CT)
	line = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	sprintf(nodeName, "VoIPCodecs_PVC%d_Entry%d", line-1, instance-1); 
#else
	sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
#endif

	tcapi_get(nodeName, "id", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 setEntryIDValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance;
	char nodeName[30] = {0};
	uint32 id;
	char strid[10] = {0};
#if defined(TCSUPPORT_CT)
	int line=0;
#endif

	instance = atoi(deviceNode->parentNode->name);	
	if(instance < 0 || instance > CODECLIST_NUMBER)
		return -1;

	id = *((uint32*)value);
	sprintf(strid, "%lu", id);
#if defined(TCSUPPORT_CT)	
	line = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	sprintf(nodeName, "VoIPCodecs_PVC%d_Entry%d", line-1, instance-1); 
#else
	sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
#endif

	SetCfgNodeFlag( "VoIPCodecs", -1 );
       tcapi_set(nodeName, "id", strid);

}	
int8 getEntryIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcTR104Attr.aEntryId[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setEntryIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcTR104Attr.aEntryId[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getTransmitGainValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPAdvanced_Entry%d", instance);
	
	tcapi_get(nodeName, "VoiceVolumeSpeak", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 setTransmitGainValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 val;
	char str[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPAdvanced_Entry%d", instance);

	val = *((int32*)value);	
	
	sprintf(str, "%d", val);

	tcapi_set(nodeName, "VoiceVolumeSpeak", str);

        SetCfgNodeFlag( "VoIPAdvanced", -1);

	return 0;
}	
int8 getTransmitGainAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aTransmitGain[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setTransmitGainAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aTransmitGain[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getReceiveGainValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPAdvanced_Entry%d", instance);
	
	tcapi_get(nodeName, "VoiceVolumeListen", tempValue);
		
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 setReceiveGainValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 val;
	char str[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPAdvanced_Entry%d", instance);

        val = *((int32*)value);	
	
	sprintf(str, "%d", val);

	tcapi_set(nodeName, "VoiceVolumeListen", str);

        SetCfgNodeFlag( "VoIPAdvanced", -1);

	return 0;
}	

int8 getReceiveGainAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aReceiveGain[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setReceiveGainAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aReceiveGain[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	


int8 getEchoCancellationTailValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPAdvanced_Entry%d", instance);
	
	tcapi_get(nodeName, "EchoCancellationTail", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 setEchoCancellationTailValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 val;
	char str[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPAdvanced_Entry%d", instance);
	
	val = *((uint32*)value);

	SetCfgNodeFlag( "VoIPAdvanced", instance);

	sprintf(str, "%lu", val);

	tcapi_set(nodeName, "EchoCancellationTail", str);

	return 0;
}	
int8 getEchoCancellationTailAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aEchoCancellationTail[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setEchoCancellationTailAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aEchoCancellationTail[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getLineStatsResetStatisticsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20]= {0} ;
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "ResetStatistics", tempValue);
		
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 setLineStatsResetStatisticsValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 iReset;
	char strReset[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);

	iReset = *((uint8*)value);
	

	if(1 == iReset)
		strcpy(strReset, "1");
	else
		strcpy(strReset, "0");

	tcapi_set(nodeName, "ResetStatistics", strReset);
	if(1 == iReset){
		tcapi_set(nodeName, "PacketsSent", "0");
		tcapi_set(nodeName, "PacketsReceived", "0");
		tcapi_set(nodeName, "PacketsLost", "0");
		tcapi_set(nodeName, "BytesSent", "0");
		tcapi_set(nodeName, "BytesReceived", "0");
		tcapi_set(nodeName, "IncomingCallsReceived", "0");
		tcapi_set(nodeName, "IncomingCallsAnswered", "0");
		tcapi_set(nodeName, "IncomingCallsConnected", "0");
		tcapi_set(nodeName, "IncomingCallsFailed", "0");
		tcapi_set(nodeName, "OutgoingCallsAttempted", "0");
		tcapi_set(nodeName, "OutgoingCallsAnswered", "0");
		tcapi_set(nodeName, "OutgoingCallsConnected", "0");
		tcapi_set(nodeName, "OutgoingCallsFailed", "0");
		tcapi_set(nodeName, "TotalCallTime", "0");
		tcapi_set(nodeName, "ResetStatData", "1");//inform voip to reset data
		tcapi_set(nodeName, "ResetStatistics", "0");
		
	}
	return 0;	
}	


int8 getLineStatsResetStatisticsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aResetStatistics[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setLineStatsResetStatisticsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aResetStatistics[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	


int8 getLineStatsPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20]= {0} ;
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "PacketsSent", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getLineStatsPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aPacketsSent[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setLineStatsPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aPacketsSent[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	


int8 getLineStatsPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20]= {0} ;
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "PacketsReceived", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getLineStatsPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aPacketsSent[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setLineStatsPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aPacketsSent[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getLineStatsBytesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "BytesSent", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getLineStatsBytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aBytesSent[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setLineStatsBytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aBytesSent[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	


int8 getLineStatsBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "BytesReceived", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getLineStatsBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aBytesReceived[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setLineStatsBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aBytesReceived[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	


int8 getLineStatsPacketsLostValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "PacketsLost", tempValue);
		
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getLineStatsPacketsLostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aPacketsLost[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setLineStatsPacketsLostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aPacketsLost[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	


int8 getLineStatsIncamingCallsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "IncomingCallsReceived", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getLineStatsIncamingCallsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aIncamingCallsReceived[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setLineStatsIncamingCallsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aIncamingCallsReceived[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	


int8 getLineStatsIncamingCallsAnsweredValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "IncomingCallsAnswered", tempValue);
		
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getLineStatsIncamingCallsAnsweredAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aIncamingCallsAnswered[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setLineStatsIncamingCallsAnsweredAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aIncamingCallsAnswered[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	


int8 getLineStatsIncamingCallsConnectedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "IncomingCallsConnected", tempValue);
		
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getLineStatsIncamingCallsConnectedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aIncamingCallsConnected[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setLineStatsIncamingCallsConnectedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aIncamingCallsConnected[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	


int8 getLineStatsIncamingCallsFailedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "IncomingCallsFailed", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getLineStatsIncamingCallsFailedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aIncamingCallsFailed[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setLineStatsIncamingCallsFailedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aIncamingCallsFailed[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	


int8 getLineStatsOutgoingCallsAttempedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "OutgoingCallsAttempted", tempValue);
		
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getLineStatsOutgoingCallsAttempedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aOutgoingCallsAttemped[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setLineStatsOutgoingCallsAttempedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aOutgoingCallsAttemped[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	


int8 getLineStatsOutgoingCallsAnsweredValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "OutgoingCallsAnswered", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getLineStatsOutgoingCallsAnsweredAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aOutgoingCallsAnswered[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setLineStatsOutgoingCallsAnsweredAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aOutgoingCallsAnswered[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	


int8 getLineStatsOutgoingCallsConnectedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "OutgoingCallsConnected", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getLineStatsOutgoingCallsConnectedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aOutgoingCallsConnected[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setLineStatsOutgoingCallsConnectedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aOutgoingCallsConnected[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	


int8 getLineStatsOutgoingCallsFailedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "OutgoingCallsFailed", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getLineStatsOutgoingCallsFailedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aOutgoingCallsFailed[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setLineStatsOutgoingCallsFailedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aOutgoingCallsFailed[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	


int8 getLineStatsTotalCallTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "TotalCallTime", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getLineStatsTotalCallTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aTotalCallTime[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setLineStatsTotalCallTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aTotalCallTime[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

#if defined(TCSUPPORT_CUC_VOIP_NODE)
int8 getReceivePacketLossRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "ReceivePacketLossRate", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);

}

int8 getReceivePacketLossRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aReceivePacketLossRate[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setReceivePacketLossRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aReceivePacketLossRate[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}

int8 getFarEndPacketLossRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "ReceivePacketLossRate", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getFarEndPacketLossRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aFarEndPacketLossRate[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setFarEndPacketLossRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aFarEndPacketLossRate[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getReceiveInterarrivalJitterValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "ReceiveInterarrivalJitter", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getReceiveInterarrivalJitterAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aReceiveInterarrivalJitter[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setReceiveInterarrivalJitterAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aReceiveInterarrivalJitter[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getFarEndInterarrivalJitterValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "FarEndInterarrivalJitter", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getFarEndInterarrivalJitterAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aFarEndInterarrivalJitter[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setFarEndInterarrivalJitterAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aFarEndInterarrivalJitter[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getRoundTripDelayValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "RoundTripDelay", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);

}

int8 getRoundTripDelayAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aRoundTripDelay[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setRoundTripDelayAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aRoundTripDelay[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getCallsDroppedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "CallsDropped", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getCallsDroppedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aCallsDropped[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCallsDroppedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aCallsDropped[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getServerDownTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "InfoVoIP_Entry%d", instance);
	
	tcapi_get(nodeName, "ServerDownTime", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getServerDownTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aServerDownTime[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setServerDownTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aServerDownTime[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}
#endif
#if defined(TCSUPPORT_CUC_VOIP_NODE) || defined(TCSUPPORT_CT_PON_SC)
int8 getCallWaitingEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	
	tcapi_get(nodeName, "SIPCallWaitingEnable", tempValue);
	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	return getTheValue(value, type, deviceNode, result);
}
int8 setCallWaitingEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	SetCfgNodeFlag( "VoIPCallCtr", instance);
	
	if(strcmp((char*)value, "1") == 0)
	{
		strcpy(result, "Yes");
		tcapi_set(nodeName, "SIPCallWaitingEnable", result);
	}
	else if(strcmp((char*)value, "0") == 0)
	{
		strcpy(result, "No");
		tcapi_set(nodeName, "SIPCallWaitingEnable", result);
	}else
		return -1;
	
	return 0;
}
int8 getCallWaitingEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
#if defined(TCSUPPORT_CT_PON_SC)
	attribute = 1;
#else
	attribute = tcTR104Attr.aCallWaitingEnable[instance];
#endif
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
uint8 attr = 1;
int8 setCallWaitingEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
#if defined(TCSUPPORT_CT_PON_SC)
	attribute = &attr;
#else
	attribute = &tcTR104Attr.aCallWaitingEnable[instance];
#endif
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

#if !defined(TCSUPPORT_CT_PON_SC)

int8 getCallForwardUnconditionalEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	tcapi_get(nodeName, "MTKUCFEnable", tempValue);
	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	return getTheValue(value, type, deviceNode, result);
}

int8 setCallForwardUnconditionalEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	SetCfgNodeFlag( "VoIPCallCtr", instance);
	
	if(strcmp((char*)value, "1") == 0)
	{
		strcpy(result, "Yes");
		tcapi_set(nodeName, "MTKUCFEnable", result);
	}
	else if(strcmp((char*)value, "0") == 0)
	{
		strcpy(result, "No");
		tcapi_set(nodeName, "MTKUCFEnable", result);
	}else
		return -1;
	
	return 0;

}
int8 getCallForwardUnconditionalEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aCallForwardUnconditionalEnable[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}
int8 setCallForwardUnconditionalEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aCallForwardUnconditionalEnable[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}

int8 getCallForwardUnconditionalNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[33] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	tcapi_get(nodeName, "CallForwardUnconditionalNumber", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);

}
int8 setCallForwardUnconditionalNumberValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);
	
	tcapi_set(nodeName, "CallForwardUnconditionalNumber", (char*)value);

	SetCfgNodeFlag( "VoIPCallCtrl", instance);
	
	return 0;

}
int8 getCallForwardUnconditionalNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aCallForwardUnconditionalNumber[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}
int8 setCallForwardUnconditionalNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aCallForwardUnconditionalNumber[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}

int8 getCallForwardOnBusyEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	tcapi_get(nodeName, "MTKBCFEnable", tempValue);
	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	return getTheValue(value, type, deviceNode, result);

}
int8 setCallForwardOnBusyEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	SetCfgNodeFlag( "VoIPCallCtr", instance);
	
	if(strcmp((char*)value, "1") == 0)
	{
		strcpy(result, "Yes");
		tcapi_set(nodeName, "CallForwardOnBusyEnable", result);
	}
	else if(strcmp((char*)value, "0") == 0)
	{
		strcpy(result, "No");
		tcapi_set(nodeName, "MTKBCFEnable", result);
	}else
		return -1;
	
	return 0;

}
int8 getCallForwardOnBusyEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aCallForwardOnBusyEnable[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}
int8 setCallForwardOnBusyEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aCallForwardOnBusyEnable[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}

int8 getCallForwardOnBusyNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[33] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	tcapi_get(nodeName, "CallForwardOnBusyNumber", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);

}
int8 setCallForwardOnBusyNumberValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);
	
	tcapi_set(nodeName, "CallForwardOnBusyNumber", (char*)value);

	SetCfgNodeFlag( "VoIPCallCtrl", instance);
	
	return 0;
}
int8 getCallForwardOnBusyNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aCallForwardOnBusyNumber[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}
int8 setCallForwardOnBusyNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aCallForwardOnBusyNumber[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}

int8 getCallForwardOnNoAnswerEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	tcapi_get(nodeName, "MTKNCFEnable", tempValue);
	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	return getTheValue(value, type, deviceNode, result);

}
int8 setCallForwardOnNoAnswerEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	SetCfgNodeFlag( "VoIPCallCtr", instance);
	
	if(strcmp((char*)value, "1") == 0)
	{
		strcpy(result, "Yes");
		tcapi_set(nodeName, "MTKNCFEnable", result);
	}
	else if(strcmp((char*)value, "0") == 0)
	{
		strcpy(result, "No");
		tcapi_set(nodeName, "MTKNCFEnable", result);
	}else
		return -1;
	
	return 0;

}
int8 getCallForwardOnNoAnswerEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aCallForwardOnNoAnswerEnable[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}
int8 setCallForwardOnNoAnswerEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aCallForwardOnNoAnswerEnable[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}

int8 getCallForwardOnNoAnswerNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[33] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	tcapi_get(nodeName, "CallForwardOnNoAnswerNumber", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);

}
int8 setCallForwardOnNoAnswerNumberValue(void* value, struct DeviceNode*  deviceNode)
{
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	SetCfgNodeFlag( "VoIPCallCtr", instance);
	
	if(strcmp((char*)value, "1") == 0)
	{
		strcpy(result, "Yes");
		tcapi_set(nodeName, "CallForwardOnNoAnswerNumber", result);
	}
	else if(strcmp((char*)value, "0") == 0)
	{
		strcpy(result, "No");
		tcapi_set(nodeName, "CallForwardOnNoAnswerNumber", result);
	}else
		return -1;
	
	return 0;

}
int8 getCallForwardOnNoAnswerNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aCallForwardOnNoAnswerNumber[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}
int8 setCallForwardOnNoAnswerNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aCallForwardOnNoAnswerNumber[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}

int8 getCallForwardOnNoAnswerRingCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);
	
	tcapi_get(nodeName, "CallForwardOnNoAnswerRingCount", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);

}
int8 setCallForwardOnNoAnswerRingCountValue(void* value, struct DeviceNode*  deviceNode)
{
	int32 val;
	char str[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	val = *((int32*)value);

	if(val > 64 || val < 1)
	  return -1;

	SetCfgNodeFlag( "VoIPCallCtrl", instance);
	
	sprintf(str, "%d", val);

	tcapi_set(nodeName, "CallForwardOnNoAnswerRingCount", str);

	return 0;

}
int8 getCallForwardOnNoAnswerRingCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aCallForwardOnNoAnswerNumber[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}
int8 setCallForwardOnNoAnswerRingCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aCallForwardOnNoAnswerNumber[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}

int8 getCallTransferEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	
	tcapi_get(nodeName, "SIPCallTransfer", tempValue);
	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	return getTheValue(value, type, deviceNode, result);
}
int8 setCallTransferEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	SetCfgNodeFlag( "VoIPCallCtr", instance);
	
	if(strcmp((char*)value, "1") == 0)
	{
		strcpy(result, "Yes");
		tcapi_set(nodeName, "SIPCallTransfer", result);
	}
	else if(strcmp((char*)value, "0") == 0)
	{
		strcpy(result, "No");
		tcapi_set(nodeName, "SIPCallTransfer", result);
	}else
		return -1;
	
	return 0;

}
int8 getCallTransferEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aCallTransferEnable[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}
int8 setCallTransferEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aCallTransferEnable[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}

int8 getMWIEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	
	tcapi_get(nodeName, "SIPMWIEnable", tempValue);
	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	return getTheValue(value, type, deviceNode, result);
}
int8 setMWIEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	SetCfgNodeFlag( "VoIPCallCtr", instance);
	
	if(strcmp((char*)value, "1") == 0)
	{
		strcpy(result, "Yes");
		tcapi_set(nodeName, "SIPMWIEnable", result);
	}
	else if(strcmp((char*)value, "0") == 0)
	{
		strcpy(result, "No");
		tcapi_set(nodeName, "SIPMWIEnable", result);
	}else
		return -1;
	
	return 0;

}
int8 getMWIEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aMWIEnable[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}
int8 setMWIEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aMWIEnable[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}

int8 getX_CU_HotlineEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	
	tcapi_get(nodeName, "HotLineEnable", tempValue);
	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	return getTheValue(value, type, deviceNode, result);

}
int8 setX_CU_HotlineEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	SetCfgNodeFlag( "VoIPCallCtr", instance);
	
	if(strcmp((char*)value, "1") == 0)
	{
		strcpy(result, "Yes");
		tcapi_set(nodeName, "HotLineEnable", result);
	}
	else if(strcmp((char*)value, "0") == 0)
	{
		strcpy(result, "No");
		tcapi_set(nodeName, "HotLineEnable", result);
	}else
		return -1;
	
	return 0;

}
int8 getX_CU_HotlineEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aX_CU_HotlineEnable[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}
int8 setX_CU_HotlineEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aX_CU_HotlineEnable[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}

int8 getX_CU_HotlineNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[33] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	tcapi_get(nodeName, "HotLineNumber", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setX_CU_HotlineNumberValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);
	
	SetCfgNodeFlag( "VoIPCallCtrl", instance);
	
	tcapi_set(nodeName, "HotLineNumber", (char*)value);

	
	return 0;

}
int8 getX_CU_HotlineNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aX_CU_HotlineNumber[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_HotlineNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aX_CU_HotlineNumber[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getX_CU_HotlineTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);
	
	tcapi_get(nodeName, "HotLineDelayTime", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setX_CU_HotlineTimerValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 val;
	char str[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	val = *((uint32*)value);
       if(val < 0|| val > 255)
	   return -1;
  
	SetCfgNodeFlag( "VoIPCallCtrl", instance);
	
	sprintf(str, "%lu", val);

	tcapi_set(nodeName, "HotLineDelayTime", str);

	return 0;

}
int8 getX_CU_HotlineTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aX_CU_HotlineTimer[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setX_CU_HotlineTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aX_CU_HotlineTimer[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getX_CU_conference_uriValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[129] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	tcapi_get(nodeName, "conference-uri", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);

}
int8 setX_CU_conference_uriValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);
	
	SetCfgNodeFlag( "VoIPCallCtrl", instance);
	
	tcapi_set(nodeName, "conference-uri", (char*)value);
	
	return 0;
}
int8 getX_CU_conference_uriAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = tcTR104Attr.aX_CU_conference_uri[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}
int8 setX_CU_conference_uriAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aX_CU_conference_uri[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}
#endif
#endif      /*end of TCSUPPORT_CUC_VOIP_NODE*/

#ifdef TCSUPPORT_PON_ROSTELECOM
int8 getX_RTK_3PTYValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	
	tcapi_get(nodeName, "SIP3wayConfServerMix", tempValue);

	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	return getTheValue(value, type, deviceNode, result);
}
int8 setX_RTK_3PTYValue(void* value, struct DeviceNode*  deviceNode)
{
	char result[10] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;

	uint8 RTK_3PTY = 0;
	
	RTK_3PTY = *((uint8*)value);

	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
	
	sprintf(nodeName, "VoIPCallCtrl_Entry%d", instance);

	SetCfgNodeFlag( "VoIPCallCtrl", instance);
	
	if(RTK_3PTY  == 0)
	{
		strcpy(result, "No");
		tcapi_set(nodeName, "SIP3wayConfServerMix", result);
	}
	else if(RTK_3PTY  == 1)
	{
		strcpy(result, "Yes");
		tcapi_set(nodeName, "SIP3wayConfServerMix", result);
	}else{
		tc_printf("==> setX_RTK_3PTYValue: value should be  0 or 1.\n");
		return -1;
	}
	return 0;
}
int8 getX_RTK_3PTYAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;

	attribute = tcTR104Attr.aX_RTK_3PTY[instance];

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setX_RTK_3PTYAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;

	attribute = &tcTR104Attr.aX_RTK_3PTY[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}
#endif

#if defined(TCSUPPORT_CUC_VOIP_NODE) || defined(TCSUPPORT_CT_PON_SC)
int8 getCallStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[129] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	int i = 0;
	char callStateStr[64] = {0};

	struct callStateMap{
            char lineState[16];
            char itmsState[16];
	} callStateMap_s[] = { {"Idle", "Idle"},
                           {"Dialing", "Calling"},
                           {"Ringing", "Ringing"},
                           {"Ringback", "Connecting"},
                           {"Connect", "InCall"},
                           {"Disconnect", "Disconnecting"},
                           {"", ""}
                           };
	
	instance = atoi(deviceNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPSysParam_Entry%d", instance);

	tcapi_get(nodeName, "SC_LINE_INFO_STATUS", callStateStr);
	
	while (strlen(callStateMap_s[i].lineState)){
            if (!strcmp(callStateMap_s[i].lineState, callStateStr)){
                strncpy(tempValue, callStateMap_s[i].itmsState, sizeof(tempValue)-1);
                break;
            }
            i++;
	}

	return getTheValue(value, type, deviceNode, tempValue);

}

int8 getCallStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = tcTR104Attr.aCallState[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}

int8 setCallStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aCallState[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}
#endif

#if defined(TCSUPPORT_CUC_VOIP_NODE) || defined(TCSUPPORT_CT_PON_GD)
int8 getDirectoryNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[65] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);

	tcapi_get(nodeName, "SIPDisplayName", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);

}
int8 setDirectoryNumberValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 val;
	char str[10] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);

	tcapi_set(nodeName, "SIPDisplayName", (char*)value);
	SetCfgNodeFlag( "VoIPBasic", -1 );

	return 0;

}
int8 getDirectoryNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = tcTR104Attr.aDirectoryNumber[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}
int8 setDirectoryNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aDirectoryNumber[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}
#endif
#if defined(TCSUPPORT_CUC_VOIP_NODE)
int8 getPhyReferenceListValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[33] = {0};
	char nodeName[34] = {'\0'};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);

	tcapi_get(nodeName, "PhyReferenceList", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);

}
int8 setPhyReferenceListValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 val;
	char str[10] = {0};
	int instance = 0;
	char nodeName[34] = {'\0'};
	
	instance = atoi(deviceNode->parentNode->name)-1;
	sprintf(nodeName, "VoIPBasic_Entry%d", instance);
	
	SetCfgNodeFlag( "VoIPBasic", -1 );
	
	tcapi_set(nodeName, "PhyReferenceList", str);

	return 0;

}
int8 getPhyReferenceListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute=0;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = tcTR104Attr.aPhyReferenceList[instance];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}
int8 setPhyReferenceListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result=0;
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name)-1;
	attribute = &tcTR104Attr.aPhyReferenceList[instance];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;

}

#endif


