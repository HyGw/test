#include "Global_res.h"
#include "cp_wandev_wandslInterCfg.h"

/*-----------------------------------------dsl interface config relation function begin-----------------------------*/

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 ADSLPARASETTING(cfgNode_ptr pCfgNode)
{
	if ( pCfgNode == NULL ) 
	{
		return -1;
	}

	tcapi_commit( pCfgNode->nodeName );
	return 0;
}

int8 getWANDSLInterCfg_EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char DSLLineEnableValue[10] = {0};
	char enableValue[8] =  {0};

        tcapi_get(ADSL_NODE_ENTRY,ADSL_ACTIVE, DSLLineEnableValue);

	if(strcmp(DSLLineEnableValue, "No") == 0)
	{
		sprintf(enableValue,"%d",0);
	} 
	//else if (strcmp(DSLLineEnableValue, "No") == 0)
	else
	{
		sprintf(enableValue,"%d",1);
	}
	
	return getTheValue(value, type, deviceNode,enableValue);
}

int8 setWANDSLInterCfg_EnableValue(void* value, struct DeviceNode* deviceNode)
{
	uint8 flag;
	
	flag = *((uint8*)value);
	
	if(flag == 1){
		tcapi_set(ADSL_NODE_ENTRY,ADSL_ACTIVE,"Yes");
	} 
	else if (flag == 0){
		tcapi_set(ADSL_NODE_ENTRY,ADSL_ACTIVE,"No");
	}
	else{
		tc_printf("==>setWANDSLInterCfg_EnableValue:value should be 0 or 1.\n");
		return -1;
	}	
	
	/*For delay commit*/
	SetCfgNodeFlag( ADSL_NODE_ENTRY, -1 );

	return 0;
}

int8 getWANDSLInterCfg_EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLInterCfg_EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANDSLInterCfg_LinkEncapsulationSupportedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{

	char supportLinkEncapsulation[] = "G.992.3_Annex_K_ATM,G.993.2_Annex_K_PTM,G.994.1";

	return getTheValue(value, type, deviceNode, supportLinkEncapsulation);
}

int8 getWANDSLInterCfg_LinkEncapsulationSupportedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aLinkEncapsulationSupported;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLInterCfg_LinkEncapsulationSupportedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aLinkEncapsulationSupported;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANDSLInterCfg_LinkEncapsulationUsedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char supportLinkEncapsulation[32];
	char StandardsSupportedStr[20] = {0};	
	
	memset(StandardsSupportedStr , 0 , sizeof(StandardsSupportedStr));	
	tcapi_get("Info_Adsl","AdslStandard",StandardsSupportedStr);

	if(!strcmp(StandardsSupportedStr,"VDSL2"))
		strcpy( supportLinkEncapsulation , "G.993.2_Annex_K_PTM");
	else
		strcpy( supportLinkEncapsulation , "G.992.3_Annex_K_ATM");

	return getTheValue(value, type, deviceNode, supportLinkEncapsulation);	
}


int8 getWANDSLInterCfg_LinkEncapsulationUsedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aLinkEncapsulationUsed;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLInterCfg_LinkEncapsulationUsedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aLinkEncapsulationUsed;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANDSLInterCfg_LinkEncapsulationRequestedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char supportLinkEncapsulation[32];
	char strTemp[20] = {0};

	tcapi_get(ADSL_NODE_ENTRY, ADSL_ATTR_MODE, strTemp);

	if(strcmp(strTemp,"VDSL2") == 0){
		strcpy(supportLinkEncapsulation,"G.993.2_Annex_K_PTM");
	}
	else if(strcmp(strTemp,"Auto Sync-Up") == 0){
		strcpy(supportLinkEncapsulation,"G.994.1");
	}
	else
		strcpy(supportLinkEncapsulation,"G.992.3_Annex_K_ATM");

	return getTheValue(value, type, deviceNode, supportLinkEncapsulation);

}

int8 setWANDSLInterCfg_LinkEncapsulationRequestedValue(void* value, struct DeviceNode* deviceNode)
{
	if(strlen((char*)value) > 32)
	{
		tc_printf("==>setWANDSLInterCfg_LinkEncapsulationRequestedValue: the length of value is too long.\n");
		return -1;
	}

	if(strcmp((char*)value,"G.994.1") == 0 )//will add a new ADSL multi item
	{
		tcapi_set(ADSL_NODE_ENTRY, ADSL_ATTR_MODE, "Auto Sync-Up");
	}
	else if(strcmp((char*)value,"G.992.3_Annex_K_ATM") == 0)//will add a new ADSL multi item
	{
		tcapi_set(ADSL_NODE_ENTRY, ADSL_ATTR_MODE, "ADSL Auto Sync-Up");
	}
	else if(strcmp((char*)value,"G.993.2_Annex_K_PTM") == 0)
	{
		tcapi_set(ADSL_NODE_ENTRY, ADSL_ATTR_MODE, "VDSL2");
	}
	else
	{
		tc_printf("==>setWANDSLInterCfg_LinkEncapsulationRequestedValue: only support G.992.3_Annex_K_ATM,G.993.2_Annex_K_PTM.\n");
		return -1;
	}

	SetCfgNodeFlag( ADSL_NODE_ENTRY, -1 );

	return 0;
}

int8 getWANDSLInterCfg_LinkEncapsulationRequestedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aLinkEncapsulationUsed;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLInterCfg_LinkEncapsulationRequestedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aLinkEncapsulationUsed;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8	getStandardsSupportedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char StandardsSupportedStrPtr[] = "G.992.1_Annex_A,G.992.1_Annex_B,T1.413,\
					G.992.2,G.992.3_Annex_A,G.992.3_Annex_B,\
					G.992.3_Annex_I,G.992.3_Annex_J,G.992.3_Annex_L,G.992.3_Annex_M,\
					G.992.5_Annex_A,G.992.5_Annex_B,G.992.5_Annex_I,G.992.5_Annex_J, \
					G.992.5_Annex_M,G.993.2_Annex_A,G.993.2_Annex_B";
	return getTheValue(value, type, deviceNode, StandardsSupportedStrPtr);
}

int8	getStandardsSupportedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.StandardsSupported;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setStandardsSupportedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.StandardsSupported;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8	getAllowedProfilesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char AllowedProfilesStrPtr[] = "8a,8b,8c,8d,12a,12b,17a,17b,30a";

	return getTheValue(value, type, deviceNode, AllowedProfilesStrPtr);
}

int8	getAllowedProfilesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aAllowedProfiles;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setAllowedProfilesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aAllowedProfiles;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8	getCurrentProfilesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	tcapi_get(INFOADSL_NODE, "CurrentProfiles", result);

	return getTheValue(value, type, deviceNode, result);
}

int8	getCurrentProfilesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aCurrentProfiles;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setCurrentProfilesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aCurrentProfiles;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8	getUPBOKLEValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	tcapi_get(INFOADSL_NODE, "UPBOKLE", result);

	return getTheValue(value, type, deviceNode, result);
}

int8	getUPBOKLEAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aUPBOKLE;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setUPBOKLEAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aUPBOKLE;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8	getTRELLISdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	tcapi_get(INFOADSL_NODE, "TRELLISds", result);

	return getTheValue(value, type, deviceNode, result);
}

int8	getTRELLISdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aTRELLISds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setTRELLISdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aTRELLISds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8	getTRELLISusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	tcapi_get(INFOADSL_NODE, "TRELLISus", result);

	return getTheValue(value, type, deviceNode, result);
}

int8	getTRELLISusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aTRELLISus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setTRELLISusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aTRELLISus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8	getACTSNRMODEdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	tcapi_get(INFOADSL_NODE, "ACTSNRMODEds", result);

	return getTheValue(value, type, deviceNode, result);
}

int8	getACTSNRMODEdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aACTSNRMODEds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setACTSNRMODEdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aACTSNRMODEds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8	getACTSNRMODEusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	tcapi_get(INFOADSL_NODE, "ACTSNRMODEus", result);

	return getTheValue(value, type, deviceNode, result);
}

int8	getACTSNRMODEusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aACTSNRMODEus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setACTSNRMODEusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aACTSNRMODEus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8	getACTUALCEValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};

	tcapi_get(INFOADSL_NODE, "ACTUALCE", result);

	return getTheValue(value, type, deviceNode, result);
}

int8	getACTUALCEAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aACTUALCE;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setACTUALCEAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aACTUALCE;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8	getSNRMpbdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[32] = {0};

	tcapi_get(INFOADSL_NODE, "SNRMpbds", result);

	return getTheValue(value, type, deviceNode, result);
}

int8	getSNRMpbdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aSNRMpbds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setSNRMpbdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aSNRMpbds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

int8	getSNRMpbusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[32] = {0};

	tcapi_get(INFOADSL_NODE, "SNRMpbus", result) ;

	return getTheValue(value, type, deviceNode, result);
}

int8	getSNRMpbusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aSNRMpbus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setSNRMpbusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aSNRMpbus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCOMINTERCFG_ATTR_FLAG;
	return result;
}

#if !defined(TCSUPPORT_CT_PON)
int8 getModulationModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char mode[8] = "0";

	strcpy(nodeName, ADSL_NODE_ENTRY);
	if ( 0 != tcapi_get(nodeName, "ModulationMode", mode) )
		strcpy(mode, "0");

	return getTheValue(value, type, deviceNode, mode);
}
int8 setModulationModeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char modeStr[20] = {0};
	uint32 mode = *((uint32*)value);

	if ( !( mode >= 0 && mode <=2 ) )
		return -1;

	sprintf(modeStr, "%d", mode);
	strcpy(nodeName, ADSL_NODE_ENTRY);
	tcapi_set(nodeName, "ModulationMode", modeStr);
	SetCfgNodeFlag( ADSL_NODE_ENTRY, -1);
	return 0;
}
int8 getModulationModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcdslInterCfgAttr.aModulationMode;
	result = GetTheAttributes(attribute, notification, acl, deviceNode);
	return result;
}
int8 setModulationModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcdslInterCfgAttr.aModulationMode;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}
#endif
#endif

int8 getWANDSLInterCfg_StatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};

	if(tcapi_get(INFOADSL_NODE, "lineState", strTemp) != 0)
	{
		strcpy(result,"NoSignal");
		return getTheValue(value, type, deviceNode, result);
	}
	
	if(0 == strcmp(strTemp, "down"))
		strcpy(result,  "NoSignal" );
	else if(0 == strcmp(strTemp, "wait for init") || 0 == strcmp(strTemp, "initializing"))
		strcpy(result,  "Initializing" );
	else if(0 == strcmp(strTemp, "up"))
		strcpy(result,  "Up" );
	else
		strcpy(result, "Error");

	return getTheValue(value, type, deviceNode, result);
}

int8 getWANDSLInterCfg_StatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aStatus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLInterCfg_StatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aStatus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

// return value:  0->none, 1->adsl, 2->vdsl
int getModulationType(char *retType, int bufsize)
{
	char strTemp[32] = {0};
	char result[32] = {0};
	int dslType = 0;

	if ( NULL == retType || bufsize < 32 )
		return 0;

	if ( 0 != tcapi_get(INFOADSL_NODE, "Opmode", strTemp) )
		strTemp[0] = '\0';

	if(0 == strcmp(strTemp, "ANSI T1.413"))
	{
		dslType = 1;
		strcpy(result, "ADSL_ANSI_T1.413" );
	}
	else if(0 == strcmp(strTemp, "ITU G.992.1(G.DMT)"))
	{
		dslType = 1;
		strcpy(result, "ADSL_G.dmt" );
	}
	else if(0 == strcmp(strTemp, "ITU G.992.2(G.Lite)"))
	{
		dslType = 1;
		strcpy(result, "ADSL_G.lite" );
	}
	else if(0 == strcmp(strTemp, "ITU G.992.3(ADSL2)"))
	{
		dslType = 1;
		strcpy(result, "ADSL_G.dmt.bis" );
	}
	else if(0 == strcmp(strTemp, "ITU G.992.5(ADSL2PLUS)"))
	{
		dslType = 1;
		strcpy(result, "ADSL_2plus" );
	}
#if defined(TCSUPPORT_CT_WAN_PTM)
	else if(0 == strcmp(strTemp, "ITU G.993.2(VDSL2)"))
	{
		dslType = 2;
		strcpy(result, "VDSL" );
	}
#endif
	else
	{
		dslType = 0;
		strcpy(result, "");
	}

	strcpy(retType, result);
	return dslType;
}
int8 getModulationTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[32] = {0};

	getModulationType(result, sizeof(result));
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getModulationTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aModulationType;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setModulationTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aModulationType;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getLineEncodingValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	strcpy(result, "DMT");
	return getTheValue(value, type, deviceNode, result);	
}

int8 getLineEncodingAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aLineEncoding;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLineEncodingAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aLineEncoding;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getDataPathValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
	
	if(tcapi_get(INFOADSL_NODE, "WanListMode", strTemp) != 0)
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}	
	if(0 == strcmp(strTemp, "0"))
		strcpy(result,  "Interleaved" );
	else		
		strcpy(result,  "Fast" );

	return getTheValue(value, type, deviceNode, result);	
}

int8 getDataPathAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aDataPath;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDataPathAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aDataPath;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getUpstreamCurrRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
	
	if(tcapi_get(INFOADSL_NODE, "DataRateUp", strTemp) != 0)
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	if(0 != Get_Value_WithoutKbps(strTemp, result))
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getUpstreamCurrRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aUpstreamCurrRate;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setUpstreamCurrRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aUpstreamCurrRate;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getDownstreamCurrRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
	
	if(tcapi_get(INFOADSL_NODE, "DataRateDown", strTemp) != 0)
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	if(0 != Get_Value_WithoutKbps(strTemp, result))
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getDownstreamCurrRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aDownstreamCurrRate;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDownstreamCurrRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aDownstreamCurrRate;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}


int8 getUpstreamMaxRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
	
	if(tcapi_get(INFOADSL_NODE, "AttainUp", strTemp) != 0)
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	strcpy(result, strTemp);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getUpstreamMaxRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aUpstreamMaxRate;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setUpstreamMaxRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aUpstreamMaxRate;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getDownstreamMaxRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
	
	if(tcapi_get(INFOADSL_NODE, "AttainDown", strTemp) != 0)
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	strcpy(result, strTemp);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getDownstreamMaxRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aDownstreamMaxRate;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDownstreamMaxRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aDownstreamMaxRate;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getUpstreamNoiseMarginValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
	
	if(tcapi_get(INFOADSL_NODE, "SNRMarginUp", strTemp) != 0)
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	if(0 != Get_MultiTen_Value(strTemp, result))
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	return getTheValue(value, type, deviceNode, result);	
}

int8 getUpstreamNoiseMarginAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aUpstreamNoiseMargin;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setUpstreamNoiseMarginAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aUpstreamNoiseMargin;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getDownstreamNoiseMarginValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
	
	if(tcapi_get(INFOADSL_NODE, "SNRMarginDown", strTemp) != 0)
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	if(0 != Get_MultiTen_Value(strTemp, result))
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}	
	return getTheValue(value, type, deviceNode, result);
}

int8 getDownstreamNoiseMarginAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aDownstreamNoiseMargin;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDownstreamNoiseMarginAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aDownstreamNoiseMargin;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getUpstreamAttenuationValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
	
	if(tcapi_get(INFOADSL_NODE, "AttenUp", strTemp) != 0)
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	if(0 != Get_MultiTen_Value(strTemp, result))
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	return getTheValue(value, type, deviceNode, result);
}

int8 getUpstreamAttenuationAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aUpstreamAttenuation;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setUpstreamAttenuationAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aUpstreamAttenuation;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getDownstreamAttenuationValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
	
	if(tcapi_get(INFOADSL_NODE, "AttenDown", strTemp) != 0)
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	if(0 != Get_MultiTen_Value(strTemp, result))
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	return getTheValue(value, type, deviceNode, result);
}

int8 getDownstreamAttenuationAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aDownstreamAttenuation;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDownstreamAttenuationAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aDownstreamAttenuation;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getUpstreamPowerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
	
	if(tcapi_get(INFOADSL_NODE, "PowerUp", strTemp) != 0)
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	if(0 != Get_MultiTen_Value(strTemp, result))
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	return getTheValue(value, type, deviceNode, result);
}

int8 getUpstreamPowerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aUpstreamPower;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setUpstreamPowerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aUpstreamPower;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getDownstreamPowerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
	
	if(tcapi_get(INFOADSL_NODE, "PowerDown", strTemp) != 0)
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	if(0 != Get_MultiTen_Value(strTemp, result))
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	return getTheValue(value, type, deviceNode, result);
}

int8 getDownstreamPowerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aDownstreamPower;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDownstreamPowerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aDownstreamPower;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getATURVendorValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
	
	if(tcapi_get(INFOADSL_NODE, "ATURID", strTemp) != 0)
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	//get nid.ituid[2],nid.ituid[3],nid.ituid[4],nid.ituid[5] from query cmd
	memcpy(result, strTemp+4, 8);
	result[8] = '\0';
	return getTheValue(value, type, deviceNode, result);
}

int8 getATURVendorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aATURVendor;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATURVendorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aATURVendor;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getATURCountryValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
	
	if(tcapi_get(INFOADSL_NODE, "ATURID", strTemp) != 0)
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	//get nid.ituid[0],nid.ituid[1] from query cmd
	memcpy(result, strTemp, 4);
	result[4] = '\0';
	return getTheValue(value, type, deviceNode, result);
}

int8 getATURCountryAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aATURCountry;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATURCountryAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aATURCountry;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getATUCVendorValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
	
	if(tcapi_get(INFOADSL_NODE, "ATUCID", strTemp) != 0)
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	//get nid.ituid[2],nid.ituid[3],nid.ituid[4],nid.ituid[5] from query cmd
	memcpy(result, strTemp+4, 8);
	result[8] = '\0';
	return getTheValue(value, type, deviceNode, result);
}

int8 getATUCVendorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aATUCVendor;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATUCVendorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aATUCVendor;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getATUCCountryValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
	
	if(tcapi_get(INFOADSL_NODE, "ATUCID", strTemp) != 0)
	{
		strcpy(result,"");
		return getTheValue(value, type, deviceNode, result);
	}
	//get nid.ituid[0],nid.ituid[1] from query cmd
	memcpy(result, strTemp, 4);
	result[4] = '\0';
	return getTheValue(value, type, deviceNode, result);
}

int8 getATUCCountryAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aATUCCountry;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATUCCountryAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aATUCCountry;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getShowtimeStartValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};
	
	if(tcapi_get(INFOADSL_NODE, "ShowtimeStart", strTemp) != 0)
	{
		strcpy(result, "0");
	}
	else
		strcpy(result, strTemp);
	return getTheValue(value, type, deviceNode, result);
}

int8 getShowtimeStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aShowtimeStart;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setShowtimeStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aShowtimeStart;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getTotalStartValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};

	if(tcapi_get(INFOADSL_NODE, "TotalStart", strTemp) != 0)
	{
		strcpy(result, "0");
	}
	else
		strcpy(result, strTemp);
	return getTheValue(value, type, deviceNode, result);
}

int8 getTotalStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aTotalStart;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTotalStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aTotalStart;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getInterleaveDepthValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};

	if(tcapi_get(INFOADSL_NODE, "InterleaveDepth", strTemp) != 0)
	{
		strcpy(result, "0");
	}
	else
		strcpy(result, strTemp);
	return getTheValue(value, type, deviceNode, result);
}

int8 getInterleaveDepthAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aInterleaveDepth;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setInterleaveDepthAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aInterleaveDepth;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getATURANSIStdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};

	if(tcapi_get(INFOADSL_NODE, "ATURANSIStd", strTemp) != 0)
	{
		strcpy(result, "0");
	}
	else
		strcpy(result, strTemp);
	return getTheValue(value, type, deviceNode, result);
}

int8 getATURANSIStdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aATURANSIStd;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATURANSIStdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aATURANSIStd;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getATURANSIRevValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};

	if(tcapi_get(INFOADSL_NODE, "ATURANSIRev", strTemp) != 0)
	{
		strcpy(result, "0");
	}
	else
		strcpy(result, strTemp);
	return getTheValue(value, type, deviceNode, result);
}

int8 getATURANSIRevAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aATURANSIRev;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATURANSIRevAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aATURANSIRev;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getATUCANSIStdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};

	if(tcapi_get(INFOADSL_NODE, "ATUCANSIStd", strTemp) != 0)
	{
		strcpy(result, "0");
	}
	else
		strcpy(result, strTemp);
	return getTheValue(value, type, deviceNode, result);
}

int8 getATUCANSIStdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aATUCANSIStd;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATUCANSIStdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aATUCANSIStd;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

int8 getATUCANSIRevValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char strTemp[20] = {0};
	char result[20] = {0};

	if(tcapi_get(INFOADSL_NODE, "ATUCANSIRev", strTemp) != 0)
	{
		strcpy(result, "0");
	}
	else
		strcpy(result, strTemp);
	return getTheValue(value, type, deviceNode, result);
}

int8 getATUCANSIRevAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aATUCANSIStd;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATUCANSIRevAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aATUCANSIStd;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

/*--------------------dsl interface config function end--------------------------------------*/

#if defined(TCSUPPORT_CT_ADSLSETTING) || defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8	getStandardUsedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* temp = NULL;
	char StandardsSupportedStr[20] = {0};	
	char tmpbuf[10] = {0};
	struct adslpara
	{
		char* standard;
		char* type;
		int flag;
	};
	struct adslpara adslparaarray[]=
	{
		{"G.DMT","G.992.1_",1},
		{"G.LITE","G.992.2",0},
		{"ANSI T1.413","T1.413",0},
		{"ETSI","ETSI_101_388",0},
		{"ADSL2","G.992.3_",1},
		{"ADSL2 PLUS","G.992.5_",1},
		{"Multi-Mode","G.992.5_",1},
#if defined(TCSUPPORT_CT_WAN_PTM)
		{"VDSL2","G.993.2_",1},
#endif
		{NULL,NULL,0}
	};

	struct adslpara *adslparaptr = NULL;
	
	memset(StandardsSupportedStr , 0 , sizeof(StandardsSupportedStr));	

	tcapi_get("Info_Adsl","AdslStandard",StandardsSupportedStr);

	for(adslparaptr = &adslparaarray[0];adslparaptr->standard != NULL;adslparaptr++)
	{
		if(!strcmp(StandardsSupportedStr,adslparaptr->standard))
		{
			strcpy( StandardsSupportedStr , adslparaptr->type);
			break;
		}
	}
	if(adslparaptr == NULL)
	{
		tc_printf("\r\nNot support this type:%s",StandardsSupportedStr);
		return -1;
	}
	
	if(1 == adslparaptr->flag)
	{
		tcapi_get("Info_Adsl","AdslType",tmpbuf);
		temp = strstr(tmpbuf , "ANNEX_");
		if(NULL != temp)
		{
			memcpy(tmpbuf, "Annex_", 6);
		}
	}

	strcat(StandardsSupportedStr,tmpbuf);

	return getTheValue(value, type, deviceNode, StandardsSupportedStr);	
}

#if !defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
char cmdmodebuf[40] = {0};
char cmdtypebuf[40] = {0};
int8	setStandardUsedValue(void* value, struct DeviceNode*  deviceNode)
{
	int i = 0;
	char *tmp;
	#if 0
	char *StandardsSupportedStrPtr[] = {"G.992.1_Annex_A" , "G.992.1_Annex_B" , "G.992.1_Annex_C" , "T1.413" , "T1.413i2" ,
										"ETSI_101_388" , "G.992.2" , "G.992.3_Annex_A" , "G.992.3_Annex_B" , "G.992.3_Annex_C" ,
										"G.992.3_Annex_I" , "G.992.3_Annex_J" , "G.992.3_Annex_L" , "G.992.3_Annex_M" , "G.992.4" ,
										 "G.992.5_Annex_A" , "G.992.5_Annex_B" , "G.992.5_Annex_C" , "G.992.5_Annex_I" , "G.992.5_Annex_J" , 
										"G.992.5_Annex_M" , "G.993.1" , "G.993.1_Annex_A" , "G.993.2_Annex_A" , "G.993.2_Annex_B" ,
										 "G.993.2_Annex_C",""};
	#else//just not support annex c
	char *StandardsSupportedStrPtr[] = {"G.992.1_Annex_A" , "G.992.1_Annex_B" , "T1.413" , "T1.413i2" ,
										"ETSI_101_388" , "G.992.2" , "G.992.3_Annex_A" , "G.992.3_Annex_B" ,
										"G.992.3_Annex_I" , "G.992.3_Annex_J" , "G.992.3_Annex_L" , "G.992.3_Annex_M" , "G.992.4" ,
										 "G.992.5_Annex_A" , "G.992.5_Annex_B" , "G.992.5_Annex_I" , "G.992.5_Annex_J" , 
										"G.992.5_Annex_M" , "G.993.1" , "G.993.1_Annex_A" , "G.993.2_Annex_A" , "G.993.2_Annex_B" ,""};
	#endif
	for(i = 0; strlen(StandardsSupportedStrPtr[i]) > 0 ;i++)
	{
		if(!strcmp((char*)value,StandardsSupportedStrPtr[i]))
			break;
	}
	if(strlen(StandardsSupportedStrPtr[i]) == 0)
	{
		tc_printf("\r\nNot support this type!");
		return -1;
	}

	memset(cmdmodebuf,0,sizeof(cmdmodebuf));
	memset(cmdtypebuf,0,sizeof(cmdtypebuf));
	tmp = strstr((char*)value,ADSLPREFIXNAME);
	if(tmp)
	{
		switch(*(tmp+strlen(ADSLPREFIXNAME)))
		{
			case 'A':
				strcpy(cmdmodebuf,"w ghs set annex a");
				break;
			case 'B':
				strcpy(cmdmodebuf,"w ghs set annex b");
				break;
			#if 0//not support
			case 'C':
				strcpy(cmdmodebuf,"w ghs set annex c");
				break;
			#endif
			case 'I':
				strcpy(cmdmodebuf,"w ghs set annex i");
				break;
			case 'J':
				strcpy(cmdmodebuf,"w ghs set annex j");
				break;
			case 'L':
				strcpy(cmdmodebuf,"w ghs set annex l");
				break;
			case 'M':
				strcpy(cmdmodebuf,"w ghs set annex m");
				break;
			default:
				strcpy(cmdmodebuf,"w ghs set annex a");
				break;
		}
	}
	else
	{
		strcpy(cmdmodebuf,"w ghs set annex a");
	}
	
	if(strstr((char*)value , "G.992.1_"))
		strcpy(cmdtypebuf,"wan adsl opencmd gdmt");
	else if(strstr((char*)value , "G.992.2"))
		strcpy(cmdtypebuf,"wan adsl opencmd glite");
	else if(strstr((char*)value , "T1.413") ||strstr((char*)value , "ETSI_101_388"))	
		strcpy(cmdtypebuf,"wan adsl opencmd t1.413");
	else if(strstr((char*)value , "G.992.3_"))
		strcpy(cmdtypebuf,"wan adsl opencmd adsl2");
	//else if(strstr((char*)value , "G.992.5_"))
	//	strcpy(cmdtypebuf,"wan adsl opencmd adsl2plus");
	else if(strstr((char*)value , "G.992.5_"))
		strcpy(cmdtypebuf,"wan adsl opencmd multimode");
	else
		tc_printf("\r\nNot need to set mode(maybe we not support now)!");
	
	SetCfgNodeFlag("CT_StandardUsed", -1);
	return 0;
}
#endif

int8	getStandardUsedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslInterCfgAttr.aStandardUsed;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setStandardUsedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslInterCfgAttr.aStandardUsed;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLINTERCFG_ATTR_FLAG;
	return result;
}

#if !defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 StanddardUsedCommitFunc(cfgNode_ptr pCfgNode)
{
	tc_printf("\r\nStanddardUsedCommitFunc:%s",cmdmodebuf);
	tc_printf("\r\nStanddardUsedCommitFunc:%s",cmdtypebuf);
	system(cmdmodebuf);
	system(cmdtypebuf);
	return 0;
}
#endif

#endif

/*-----------------------------------------dsl interface config function end-----------------------------*/
/*_____________________________________________________________________________
**      function name: Get_MultiTen_Value
**      descriptions:
**            Get specific string value from origin string.
**            e.g. input: 3.5; output: 35
**
**      parameters:
**            str:         intput string with symbol
**            strResult: output string
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      revision:
**      1. Renyu 2010/1/13
**____________________________________________________________________________
*/
uint8 Get_MultiTen_Value(char *str, char *strResult)
{
	char *pos;
	char *sub = ".";
	char *curr = str;
	int i = 0;

	char preStr[20] = {0};
	char sufStr[3] = {0};
	int preValue = 0;
	int sufValue = 0;
	int value = 0;
	pos = strstr(str, sub);
	if(pos)
	{
		while(curr < pos)
			preStr[i++] = *curr++;
		preStr[i] = '\0';
		sufStr[0] = *(++curr);

		preValue = atoi(preStr);
		sufValue = atoi(sufStr);
		value = preValue*10 + sufValue;
		sprintf(strResult, "%d", value);
		return 0;
	}else
	{
		tcdbg_printf("\r\nwe cannot found the specific string\n");
		return -1;
	}
}
/*_____________________________________________________________________________
**      function name: Get_Value_WithoutKbps
**      descriptions:
**            Get specific string value from origin string.
**            e.g. input: 345 kbps; output: 345
**
**      parameters:
**            str:         intput string with symbol( kbps)
**            strResult: output string
**
**      global:
**             None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      revision:
**      1. Renyu 2010/1/13
**____________________________________________________________________________
*/
uint8 Get_Value_WithoutKbps(char *origin, char*result)
{
	char *sub = " kbps";
	char *pos;
	char *curr = origin;
	int i = 0;
	pos = strstr(origin, sub);
	if(pos)
	{
		while(curr < pos)
			result[i++] = *curr++;
		result[i] = '\0';
		return 0;
	}else
	{
		tcdbg_printf("\r\nwe cannot found the specific string\n");
		return -1;
	}
}

