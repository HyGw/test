#include "Global_res.h"
#include "cp_wandev_wandsldiag.h"

#if !defined(TCSUPPORT_CT_PON)

/*-----------------------------------------dsl diagnostic relation function end-----------------------------*/
int8 getWANDSLDiagnositcs_LoopDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char DiagnosticsState[20] = {0};
	NT_ADSL_def_Diagnostics_poll("DiagnosticState",DiagnosticsState);
	return getTheValue(value, type, deviceNode, DiagnosticsState);
}

int8 setWANDSLDiagnositcs_LoopDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode)
{
	if(strcmp((char*)value,"Requested") == 0)
	{
		tc_printf("\r\nsetWANDSLDiagnositcs_LoopDiagnosticsStateValue");
		rpc.flags.theDSLLoopFlag |= CWMP_DSLLOOP_ACTIVE;
		return 0;
	}
	else
		return -1;
}

int8 getWANDSLDiagnositcs_LoopDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_LoopDiagnosticsState;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_LoopDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_LoopDiagnosticsState;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_ACTPSDdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ACTPSDds[20] = {0};
	NT_ADSL_def_Diagnostics_poll("ACTPSDds", ACTPSDds);	
	return getTheValue(value, type, deviceNode, ACTPSDds);	
}

int8 getWANDSLDiagnositcs_ACTPSDdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_ACTPSDds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_ACTPSDdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_ACTPSDds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_ACTPSDusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ACTPSDus[20] = {0};
	NT_ADSL_def_Diagnostics_poll("ACTPSDus", ACTPSDus);	
	return getTheValue(value, type, deviceNode, ACTPSDus);	
}

int8 getWANDSLDiagnositcs_ACTPSDusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_ACTPSDus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_ACTPSDusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_ACTPSDus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_ACTATPdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ACTATPds[20] = {0};
	NT_ADSL_def_Diagnostics_poll("ACTATPds", ACTATPds);	
	return getTheValue(value, type, deviceNode, ACTATPds);	
}

int8 getWANDSLDiagnositcs_ACTATPdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_ACTATPds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_ACTATPdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_ACTATPds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_ACTATPusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ACTATPus[20] = {0};
	NT_ADSL_def_Diagnostics_poll("ACTATPus", ACTATPus);	
	return getTheValue(value, type, deviceNode, ACTATPus);	
}

int8 getWANDSLDiagnositcs_ACTATPusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_ACTATPus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_ACTATPusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_ACTATPus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_HLINSCdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char HLINSCds[20] = {0};
	NT_ADSL_def_Diagnostics_poll("HLINSCds", HLINSCds);	
	return getTheValue(value, type, deviceNode, HLINSCds);	
}

int8 getWANDSLDiagnositcs_HLINSCdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_HLINSCds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_HLINSCdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_HLINSCds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}


int8 getWANDSLDiagnositcs_HLINpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char HLINpsds[8000] = {0};
	NT_ADSL_def_Diagnostics_poll("HLINpsds", HLINpsds);	
	return getTheValue(value, type, deviceNode, HLINpsds);	
}

int8 getWANDSLDiagnositcs_HLINpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_HLINpsds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_HLINpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_HLINpsds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_QLNpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char QLINpsds[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("QLNpsds", QLINpsds);	
	return getTheValue(value, type, deviceNode, QLINpsds);	
}

int8 getWANDSLDiagnositcs_QLNpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_QLNpsds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_QLNpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_QLNpsds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_SNRpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char SNRpsds[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("SNRpsds", SNRpsds);	
	return getTheValue(value, type, deviceNode, SNRpsds);	
}

int8 getWANDSLDiagnositcs_SNRpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_SNRpsds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_SNRpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_SNRpsds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_BITSpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char BITSpsds[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("BITSpsds", BITSpsds);	
	return getTheValue(value, type, deviceNode, BITSpsds);
}

int8 getWANDSLDiagnositcs_BITSpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_BITSpsds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_BITSpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_BITSpsds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_GAINSpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char GAINSpsds[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("GAINSpsds", GAINSpsds);	
	return getTheValue(value, type, deviceNode, GAINSpsds);
}

int8 getWANDSLDiagnositcs_GAINSpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_GAINSpsds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_GAINSpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_GAINSpsds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

#if defined(TCSUPPORT_CT_WAN_PTM)
int8 getWANDSLDiagnositcs_HLINSCusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char HLINSCus[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("HLINSCus", HLINSCus);	
	return getTheValue(value, type, deviceNode, HLINSCus);
}

int8 getWANDSLDiagnositcs_HLINSCusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_HLINSCus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_HLINSCusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_HLINSCus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_HLINGdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char HLINGds[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("HLINGds", HLINGds);	
	return getTheValue(value, type, deviceNode, HLINGds);
}

int8 getWANDSLDiagnositcs_HLINGdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_HLINGds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_HLINGdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_HLINGds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_HLINGusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char HLINGus[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("HLINGus", HLINGus);	
	return getTheValue(value, type, deviceNode, HLINGus);
}

int8 getWANDSLDiagnositcs_HLINGusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_HLINGus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_HLINGusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_HLINGus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_HLINpsusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char HLINpsus[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("HLINpsus", HLINpsus);	
	return getTheValue(value, type, deviceNode, HLINpsus);
}

int8 getWANDSLDiagnositcs_HLINpsusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_HLINpsus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_HLINpsusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_HLINpsus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_HLOGGdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char HLOGGds[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("HLOGGds", HLOGGds);	
	return getTheValue(value, type, deviceNode, HLOGGds);
}

int8 getWANDSLDiagnositcs_HLOGGdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_HLOGGds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_HLOGGdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_HLOGGds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_HLOGGusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char HLOGGus[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("HLOGGus", HLOGGus);	
	return getTheValue(value, type, deviceNode, HLOGGus);
}

int8 getWANDSLDiagnositcs_HLOGGusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_HLOGGus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_HLOGGusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_HLOGGus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_HLOGpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char HLOGpsds[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("HLOGpsds", HLOGpsds);	
	return getTheValue(value, type, deviceNode, HLOGpsds);
}

int8 getWANDSLDiagnositcs_HLOGpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_HLOGpsds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_HLOGpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_HLOGpsds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_HLOGpsusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char HLOGpsus[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("HLOGpsus", HLOGpsus);	
	return getTheValue(value, type, deviceNode, HLOGpsus);
}

int8 getWANDSLDiagnositcs_HLOGpsusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_HLOGpsus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_HLOGpsusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_HLOGpsus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_HLOGMTdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char HLOGMTds[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("HLOGMTds", HLOGMTds);	
	return getTheValue(value, type, deviceNode, HLOGMTds);
}

int8 getWANDSLDiagnositcs_HLOGMTdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_HLOGMTds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_HLOGMTdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_HLOGMTds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_HLOGMTusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char HLOGMTus[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("HLOGMTus", HLOGMTus);	
	return getTheValue(value, type, deviceNode, HLOGMTus);
}

int8 getWANDSLDiagnositcs_HLOGMTusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_HLOGMTus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_HLOGMTusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_HLOGMTus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_QLNGdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char QLNGds[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("QLNGds", QLNGds);	
	return getTheValue(value, type, deviceNode, QLNGds);
}

int8 getWANDSLDiagnositcs_QLNGdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_QLNGds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_QLNGdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_QLNGds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_QLNGusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char QLNGus[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("QLNGus", QLNGus);	
	return getTheValue(value, type, deviceNode, QLNGus);
}

int8 getWANDSLDiagnositcs_QLNGusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_QLNGus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_QLNGusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_QLNGus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_QLNpsusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char QLNpsus[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("QLNpsus", QLNpsus);	
	return getTheValue(value, type, deviceNode, QLNpsus);
}

int8 getWANDSLDiagnositcs_QLNpsusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_QLNpsus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_QLNpsusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_QLNpsus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_QLNMTdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char QLNMTds[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("QLNMTds", QLNMTds);	
	return getTheValue(value, type, deviceNode, QLNMTds);
}

int8 getWANDSLDiagnositcs_QLNMTdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_QLNMTds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_QLNMTdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_QLNMTds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_QLNMTusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char QLNMTus[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("QLNMTus", QLNMTus);	
	return getTheValue(value, type, deviceNode, QLNMTus);
}

int8 getWANDSLDiagnositcs_QLNMTusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_QLNMTus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_QLNMTusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_QLNMTus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_SNRGdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char SNRGds[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("SNRGds", SNRGds);	
	return getTheValue(value, type, deviceNode, SNRGds);
}

int8 getWANDSLDiagnositcs_SNRGdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_SNRGds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_SNRGdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_SNRGds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_SNRGusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char SNRGus[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("SNRGus", SNRGus);	
	return getTheValue(value, type, deviceNode, SNRGus);
}

int8 getWANDSLDiagnositcs_SNRGusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_SNRGus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_SNRGusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_SNRGus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_SNRpsusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char SNRpsus[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("SNRpsus", SNRpsus);	
	return getTheValue(value, type, deviceNode, SNRpsus);
}

int8 getWANDSLDiagnositcs_SNRpsusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_SNRpsus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_SNRpsusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_SNRpsus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_SNRMTdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char SNRMTds[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("SNRMTds", SNRMTds);	
	return getTheValue(value, type, deviceNode, SNRMTds);
}

int8 getWANDSLDiagnositcs_SNRMTdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_SNRMTds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_SNRMTdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_SNRMTds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}


int8 getWANDSLDiagnositcs_SNRMTusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char SNRMTus[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("SNRMTus", SNRMTus);	
	return getTheValue(value, type, deviceNode, SNRMTus);
}

int8 getWANDSLDiagnositcs_SNRMTusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_SNRMTus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_SNRMTusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_SNRMTus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_LATNpbdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char LATNpbds[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("LATNds", LATNpbds);	
	return getTheValue(value, type, deviceNode, LATNpbds);
}

int8 getWANDSLDiagnositcs_LATNpbdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_LATNpbds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_LATNpbdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_LATNpbds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_LATNpbusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char LATNpbus[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("LATNus", LATNpbus);	
	return getTheValue(value, type, deviceNode, LATNpbus);
}

int8 getWANDSLDiagnositcs_LATNpbusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_LATNpbus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_LATNpbusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_LATNpbus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_SATNdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char SATNds[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("SATNds", SATNds);	
	return getTheValue(value, type, deviceNode, SATNds);
}

int8 getWANDSLDiagnositcs_SATNdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_SATNds;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_SATNdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_SATNds;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANDSLDiagnositcs_SATNusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char SATNus[3000] = {0};
	NT_ADSL_def_Diagnostics_poll("SATNus", SATNus);	
	return getTheValue(value, type, deviceNode, SATNus);
}

int8 getWANDSLDiagnositcs_SATNusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslDiagnosticAttr.aWANDSLDiagnostics_SATNus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANDSLDiagnositcs_SATNusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslDiagnosticAttr.aWANDSLDiagnostics_SATNus;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

#endif

/*-----------------------------------------dsl diagnostic relation function end-----------------------------*/

//add by brian
/**************************dsl diagnostic begin***********************************/
int8 NT_ADSL_def_Diagnostics_poll(char *attrName,char *p_diagnostic_set_buf)
{
	char attrNumBuf[10] = {'\0'};
	int attrNum = 0;
	int i = 0;
	char index[10] = {'\0'}; 
	char attrBuf[576] = {'\0'};
	char attrNameBuf[64] = {'\0'}; //attribute should not be more than 63.
	if(strcmp(attrName,"DiagnosticState") == 0 || strcmp(attrName,"ACTPSDds") == 0
		|| strcmp(attrName,"ACTPSDus") == 0 || strcmp(attrName,"ACTATPds") == 0
		|| strcmp(attrName,"ACTATPus") == 0 || strcmp(attrName,"HLINSCds") == 0
#if defined(TCSUPPORT_CT_WAN_PTM)
		|| strcmp(attrName,"HLINSCus") == 0
		|| strcmp(attrName,"HLINGds") == 0 || strcmp(attrName,"HLINGus") == 0
		|| strcmp(attrName,"HLOGGds") == 0 || strcmp(attrName,"HLOGGus") == 0
		|| strcmp(attrName,"HLOGMTds") == 0 || strcmp(attrName,"HLOGMTus") == 0
		|| strcmp(attrName,"QLNGds") == 0 || strcmp(attrName,"QLNGus") == 0
		|| strcmp(attrName,"QLNMTds") == 0 || strcmp(attrName,"QLNMTus") == 0
		|| strcmp(attrName,"SNRGds") == 0 || strcmp(attrName,"SNRGus") == 0
		|| strcmp(attrName,"SNRMTds") == 0 || strcmp(attrName,"SNRMTus") == 0
#endif
	)
	{
		if(tcapi_get(CWMPDSLDIAGNOSTIC, attrName, p_diagnostic_set_buf) != 0)
		{
			tcdbg_printf("\r\nget value error(normal attr),the attrName is %s", attrName);
			return -1;
		}
	}
	else if(strcmp(attrName,"HLINpsds") == 0 ||strcmp(attrName,"QLNpsds") == 0 
		||strcmp(attrName,"SNRpsds") == 0 ||strcmp(attrName,"BITSpsds") == 0 
		||strcmp(attrName,"GAINSpsds") == 0
#if defined(TCSUPPORT_CT_WAN_PTM)
		|| strcmp(attrName,"HLOGpsds") == 0	|| strcmp(attrName,"HLOGpsus") == 0
		|| strcmp(attrName,"HLINpsus") == 0 || strcmp(attrName,"QLNpsus") == 0
		|| strcmp(attrName,"SNRpsus") == 0
		|| strcmp(attrName,"SATNds") == 0 || strcmp(attrName,"SATNus") == 0
		|| strcmp(attrName,"LATNds") == 0 || strcmp(attrName,"LATNus") == 0
#endif
		)
	{
		strcpy(attrNameBuf, attrName);
		strcat(attrNameBuf, "Len"); 
		tcdbg_printf("\r\nattrNameBuf is %s", attrNameBuf);
	
		tcapi_get(CWMPDSLDIAGNOSTIC, attrNameBuf, attrNumBuf);
		attrNum = atoi(attrNumBuf); 
		
		if(attrNum == 0)//value is no splited
		{
			if(tcapi_get(CWMPDSLDIAGNOSTIC, attrName, p_diagnostic_set_buf) != 0)
			{
				tcdbg_printf("\r\nget value error,the attrName is %s", attrName);
				return -1;
			}
		}
		else
		{
			for(i = 0; i < attrNum; i++)
			{
				strcpy(attrNameBuf, attrName);
				sprintf(index, "%d", i);
				strcat(attrNameBuf, index); //if attrNum is larger than 0,attr name is attrName0,attrName1...

				tcdbg_printf("\r\nattrNameBuf is %s",attrNameBuf);
				memset(attrBuf,0,sizeof(attrBuf));
				if(tcapi_get(CWMPDSLDIAGNOSTIC, attrNameBuf, attrBuf) != 0)
				{
					tcdbg_printf("\r\nget value error(split),the attrName is %s", attrName);
					return -1;
				}
				strcat(p_diagnostic_set_buf, attrBuf);
			}
		}	
	}
	else
	{
		tcdbg_printf("\r\nattrName is not define here,the attrName is %s", attrName);
		return -1;	
	}
	
	return 0;
}

/**************************dsl diagnostic end***********************************/
#endif

