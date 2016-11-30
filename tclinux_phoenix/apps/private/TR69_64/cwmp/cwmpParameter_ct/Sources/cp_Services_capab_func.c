#include "Global_res.h"
#include "cp_Services.h"
#include "cp_Services_capab.h"

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
int prevSipStatus_tr69=0;

int getSipStatus_tr69()
{
	char tmp[32]={0};
	tcapi_get("VoIPBasic_Common", "SIPProtocol", tmp);
	if(strcmp(tmp,"H.248") == 0)
		prevSipStatus_tr69 = 1;
	else 
		prevSipStatus_tr69 = 0;
	//tcdbg_printf("func[getSipStatus_tr69]VoIPBasic_Common SIPProtocol is %s\r\n", tmp);
	//tcdbg_printf("func[getSipStatus_tr69]voip_basic_boot: prevSipStatus is %d\r\n", prevSipStatus_tr69);
	return prevSipStatus_tr69;
}
#endif


int8 getMaxProfileCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	sprintf(tempValue, "%d", 1);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getMaxProfileCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aMaxProfileCount;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}		
int8 setMaxProfileCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aMaxProfileCount;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getMaxLineCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	sprintf(tempValue, "%d", 1);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getMaxLineCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aMaxLineCount;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setMaxLineCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aMaxLineCount;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}	

int8 getMaxSessionsPerLineValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	sprintf(tempValue, "%d", 1);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getMaxSessionsPerLineAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aMaxSessionsPerLine;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setMaxSessionsPerLineAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aMaxSessionsPerLine;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getMaxSessionCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	sprintf(tempValue, "%d", 1);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getMaxSessionCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aMaxSessionCount;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setMaxSessionCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aMaxSessionCount;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getRegionsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("InfoVoIP_Common", "Regions", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 getRegionsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aRegions;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setRegionsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aRegions;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);	
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getRTCPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[5] = {0};
	
	tcapi_get("InfoVoIP_Common", "RTCP", tempValue);

	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	
	return getTheValue(value, type, deviceNode, result);
}	
		
int8 getRTCPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aRTCP;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setRTCPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aRTCP;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getSRTPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[5] = {0};
	
	tcapi_get("InfoVoIP_Common", "SRTP", tempValue);

	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	
	return getTheValue(value, type, deviceNode, result);
}	
		
int8 getSRTPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aSRTP;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setSRTPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aSRTP;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getRTPRedundancyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[5] = {0};
	
	tcapi_get("InfoVoIP_Common", "RTPRedundancy", tempValue);

	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	
	return getTheValue(value, type, deviceNode, result);
}	
		
int8 getRTPRedundancyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aRTPRedundancy;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setRTPRedundancyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aRTPRedundancy;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getDSCPCoupledValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[5] = {0};
	
	tcapi_get("InfoVoIP_Common", "DSCPCoupled", tempValue);

	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	
	return getTheValue(value, type, deviceNode, result);
}	
		
int8 getDSCPCoupledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aDSCPCoupled;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setDSCPCoupledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aDSCPCoupled;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getEthernetTaggingCoupledValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[5] = {0};
	
	tcapi_get("InfoVoIP_Common", "EthernetTaggingCoupled", tempValue);

	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	
	return getTheValue(value, type, deviceNode, result);
}	
		
int8 getEthernetTaggingCoupledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aEthernetTaggingCoupled;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setEthernetTaggingCoupledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aEthernetTaggingCoupled;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getPSTNSoftSwitchOverValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char result[5] = {0};
	
	tcapi_get("InfoVoIP_Common", "PSTNSoftSwitchOver", tempValue);

	if(strcmp(tempValue, "Yes") == 0)
		strcpy(result, "1");
	else
		strcpy(result, "0");
	
	return getTheValue(value, type, deviceNode, result);
}	
		
int8 getPSTNSoftSwitchOverAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aPSTNSoftSwitchOver;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setPSTNSoftSwitchOverAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aPSTNSoftSwitchOver;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getFaxT38Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	tcapi_get("InfoVoIP_Common", "FaxT38", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getFaxT38Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aFaxT38;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setFaxT38Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aFaxT38;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getFaxPassThroughValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	tcapi_get("InfoVoIP_Common", "FaxPassThrough", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getFaxPassThroughAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aFaxPassThrough;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setFaxPassThroughAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aFaxPassThrough;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getModemPassThroughValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	tcapi_get("InfoVoIP_Common", "ModemPassThrough", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getModemPassThroughAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aModemPassThrough;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setModemPassThroughAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aModemPassThrough;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getToneGenerationValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	tcapi_get("InfoVoIP_Common", "ToneGeneration", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getToneGenerationAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aToneGeneration;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setToneGenerationAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aToneGeneration;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getRingGenerationValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	tcapi_get("InfoVoIP_Common", "RingGeneration", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getRingGenerationAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aRingGeneration;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setRingGenerationAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aRingGeneration;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getCUNumberingPlanValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	tcapi_get("VoIPAdvanced_Common", "NumberPlan", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getCUNumberingPlanAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aCUNumberingPlan;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setCUNumberingPlanAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aCUNumberingPlan;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getButtonMapValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	tcapi_get("InfoVoIP_Common", "ButtonMap", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getButtonMapAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aButtonMap;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setButtonMapAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aButtonMap;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getVoicePortTestsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	
	tcapi_get("InfoVoIP_Common", "VoicePortTests", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	

int8 getVoicePortTestsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTR104Attr.aVoicePortTests;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	

int8 setVoicePortTestsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTR104Attr.aVoicePortTests;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getCap_DigitMapValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	sprintf(tempValue, "%d", 1);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 getCap_DigitMapAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aCap_DigitMap;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);	
	return result;
}	
int8 setCap_DigitMapAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aCap_DigitMap;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);	
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getSignalingProtocolsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[260] = {0};
	
	//sprintf(tempValue, "%s", "SIP");
	
	tcapi_get("VoIPAdvanced_Common", "SignalingProtocols", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 getSignalingProtocolsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aSignalingProtocols;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setSignalingProtocolsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aSignalingProtocols;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);	
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

#if defined(TCSUPPORT_CT)
int8 getHeartbeatValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = {0};
	
	tcapi_get("InfoVoIP_Common", "Heartbeat", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}	
int8 getHeartbeatAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTR104Attr.aHeartbeat;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setHeartbeatAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTR104Attr.aHeartbeat;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;

	return result;
}
#endif

#if (WITHVOIP != 1)
int8 getEntryIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30]={0};
	
	instance = atoi(deviceNode->parentNode->name);	
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus_tr69() == 0){	  /*SIP IMSSIP*/
		if(instance < 0 || instance > INFOVOIP_COMMONNUMBER)
		return -1;
		sprintf(nodeName,"EntryID%d" , instance-1); 
		tcapi_get("InfoVoIP_Common", nodeName, tempValue);
	}
	else if(getSipStatus_tr69() == 1){ /*H.248*/
		if(instance < 0 || instance > VOIPCODECS_ENTRYNUMBER)
			return -1;
		sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
		tcapi_get(nodeName, "id", tempValue);
	}
#else
	{
		if(instance < 0 || instance > VOIPCODECS_ENTRYNUMBER)
			return -1;
		sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
		tcapi_get(nodeName, "id", tempValue);
	}
#endif
	return getTheValue(value, type, deviceNode, tempValue);

}	
int8 getEntryIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aEntryID[instance];

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setEntryIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aEntryID[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}
#endif

int8 getCs_CodecValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char nodeName[30]={0};
	
	instance = atoi(deviceNode->parentNode->name);	

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus_tr69() == 0){	  /*SIP IMSSIP*/
		if(instance < 0 || instance > INFOVOIP_COMMONNUMBER)
		return -1;
		sprintf(nodeName,"Codec%d" , instance-1); 
		tcapi_get("InfoVoIP_Common", nodeName, tempValue);
	}
	else if(getSipStatus_tr69() == 1){ /*H.248*/
		if(instance < 0 || instance > VOIPCODECS_ENTRYNUMBER)
		return -1;
		sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
		tcapi_get(nodeName, "codec", tempValue);
	}
#else
	{
		if(instance < 0 || instance > VOIPCODECS_ENTRYNUMBER)
		return -1;
		sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
		tcapi_get(nodeName, "codec", tempValue);
	}
#endif
	return getTheValue(value, type, deviceNode, tempValue);

}	
int8 getCs_CodecAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aCs_Codec[instance];

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setCs_CodecAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aCs_Codec[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getBitRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char tempName[30]={0};
	char nodeName[30] = {0};

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus_tr69() == 0){     /*SIP IMSSIP*/
		
		instance = atoi(deviceNode->parentNode->name);	
			
		if(instance < 0 || instance > INFOVOIP_COMMONNUMBER)
			return -1;
		sprintf(tempName,"BitRate%d" , instance-1); 
		tcapi_get("InfoVoIP_Common", tempName, tempValue);
		
		return getTheValue(value, type, deviceNode, tempValue);
	}
	else if(getSipStatus_tr69() == 1){ /*H.248*/

		instance = atoi(deviceNode->parentNode->name);	
	
		if(instance < 0 || instance > VOIPCODECS_ENTRYNUMBER)
			return -1;
		sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
		tcapi_get(nodeName, "BitRate", tempValue);
		
		return getTheValue(value, type, deviceNode, tempValue);
	}
#else
	{
		instance = atoi(deviceNode->parentNode->name);	
	
		if(instance < 0 || instance > VOIPCODECS_ENTRYNUMBER)
			return -1;
		sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
		tcapi_get(nodeName, "BitRate", tempValue);
		
		return getTheValue(value, type, deviceNode, tempValue);
	}

#endif

}	
int8 getBitRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aBitRate[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setBitRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aBitRate[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getCs_PacketizationPeriodValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char tempName[30]={0};
	char nodeName[30] = {0};

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
	
	if(getSipStatus_tr69() == 0){     /*SIP IMSSIP*/
		
		instance = atoi(deviceNode->parentNode->name);	
		
		if(instance < 0 || instance > INFOVOIP_COMMONNUMBER)
			return -1;
		sprintf(tempName,"PacketizationPeriod%d" , instance-1); 
		tcapi_get("InfoVoIP_Common", tempName, tempValue);
	
		return getTheValue(value, type, deviceNode, tempValue);
	}
	else if(getSipStatus_tr69() == 1){ 		/*H.248*/

		instance = atoi(deviceNode->parentNode->name);	
	
		if(instance < 0 || instance > VOIPCODECS_ENTRYNUMBER)
			return -1;
		sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
		tcapi_get(nodeName, "SIPPacketizationTime", tempValue);
	
		return getTheValue(value, type, deviceNode, tempValue);
	}
#else
{
 		instance = atoi(deviceNode->parentNode->name);	
	
		if(instance < 0 || instance > VOIPCODECS_ENTRYNUMBER)
			return -1;
		sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
		tcapi_get(nodeName, "SIPPacketizationTime", tempValue);
	
		return getTheValue(value, type, deviceNode, tempValue);
}

#endif



}	
int8 getCs_PacketizationPeriodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aCs_PacketizationPeriod[instance];

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setCs_PacketizationPeriodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aCs_PacketizationPeriod[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int8 getSilenceSuppressionValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance;
	char tempValue[20] = {0};
	char tempName[30]={0};
	char result[5] = {0};
	char nodeName[30] = {0};

#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)	
		if(getSipStatus_tr69() == 0){	 /*SIP IMSSIP*/
			
			instance = atoi(deviceNode->parentNode->name);	
			
			if(instance < 0 || instance > INFOVOIP_COMMONNUMBER)
				return -1;
			sprintf(tempName,"SilenceSuppression%d" , instance-1); 
			tcapi_get("InfoVoIP_Common", tempName, tempValue);
		
			return getTheValue(value, type, deviceNode, tempValue);
		}
		else if(getSipStatus_tr69() == 1){		/*H.248*/
		
			instance = atoi(deviceNode->parentNode->name);	
	
			if(instance < 0 || instance > VOIPCODECS_ENTRYNUMBER)
				return -1;
			sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
			tcapi_get(nodeName, "SilenceSuppression", tempValue);
	
			if(strcmp(tempValue, "Yes") == 0)
				strcpy(result, "1");
			else
				strcpy(result, "0");
		
			return getTheValue(value, type, deviceNode, result);
		}
#else
{			
			instance = atoi(deviceNode->parentNode->name);	
	
			if(instance < 0 || instance > VOIPCODECS_ENTRYNUMBER)
				return -1;
			sprintf(nodeName, "VoIPCodecs_Entry%d", instance-1); 
			tcapi_get(nodeName, "SilenceSuppression", tempValue);
	
			if(strcmp(tempValue, "Yes") == 0)
				strcpy(result, "1");
			else
				strcpy(result, "0");
		
			return getTheValue(value, type, deviceNode, result);
}

#endif

}	
int8 getSilenceSuppressionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;
		
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = tcTR104Attr.aSilenceSuppression[instance];
	
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}	
int8 setSilenceSuppressionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;
			
	instance = atoi(deviceNode->parentNode->name) - 1;
	attribute = &tcTR104Attr.aSilenceSuppression[instance];

	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TR104_ATTR_FLAG;
	
	return result;
}

int cwmpInitCodecsSubTree( DeviceNodePtr deviceNode )
{
	int rtnCode = 0;
	int num = 0;
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus_tr69() == 0){	 /*SIP IMSSIP*/

		num = CODECSNUM_SIP;
		
	}
	else if(getSipStatus_tr69() == 1){	 /*H.248*/

		num = CODECSNUM_NOSIP;
	}
#else
		num = CODECSNUM_NOSIP;
#endif

	rtnCode = RegisterDynNodeFromIndex( deviceNode, CodecsIndex, num );

	return rtnCode;
}

