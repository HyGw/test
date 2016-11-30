#include "Global_res.h"
#include "cp_wandev_wanconndev_atmF5LBDiag.h"

#if !defined(TCSUPPORT_CT_PON)

int8 getWANATMF5LoopbackDiagnostics_DiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{

	char DiagnosticsState[20];	
	atmF5LoopbackDiagnostis_t F5LoopbackDiagnositcs;
	
	cwmpGetF5LoopbackDiagnosticsResult(&F5LoopbackDiagnositcs, getWANLinkIndex(deviceNode));
	switch(F5LoopbackDiagnositcs.DiagnosticsState)
	{
		case eDiagnosticsStateComplete:
			sprintf(DiagnosticsState, "Complete");
			break;
		case eDiagnosticsStateRequested:
			sprintf(DiagnosticsState, "Requested");
			break;
		case eDiagnosticsStateError_internal:
			sprintf(DiagnosticsState, "Error_Internal");
			break;
		case eDiagnosticsStateError_other:
			sprintf(DiagnosticsState, "Error_Other");
			break;
		case eDiagnosticsStateNone:
		default:
			sprintf(DiagnosticsState, "None");
			break;			
	}
	return getTheValue(value, type, deviceNode, DiagnosticsState);	
}

int8 setWANATMF5LoopbackDiagnostics_DiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode)
{
	if(strcmp((char*)value,"Requested") == 0)
	{
		cwmpSetF5LoopbackDiagnosticsWannodeindex(getWANLinkIndex(deviceNode));

		rpc.flags.theLoopBackFlag |= CWMP_LOOPBACK_ACTIVE;
		return 0;
	}		
	else //Invalid DiagnosticsState value
		return -1;
}

int8 getWANATMF5LoopbackDiagnostics_DiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcatmDiagnosticAttr.aWANATMF5Loopback_DiagnosticsState[getWANLinkIndex(deviceNode)];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANATMF5LoopbackDiagnostics_DiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcatmDiagnosticAttr.aWANATMF5Loopback_DiagnosticsState[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANATMF5LoopbackDiagnostics_NumberOfRepetitionsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{

	char NumberOfRepetitions[20];
	atmF5LoopbackDiagnostis_t F5LoopbackDiagnositcs; 
	
	cwmpGetF5LoopbackDiagnosticsResult(&F5LoopbackDiagnositcs, getWANLinkIndex(deviceNode));
	if(F5LoopbackDiagnositcs.NumberOfRepetitions < 1)  
		F5LoopbackDiagnositcs.NumberOfRepetitions = 1;
	sprintf(NumberOfRepetitions, "%d", F5LoopbackDiagnositcs.NumberOfRepetitions);
	return getTheValue(value, type, deviceNode, NumberOfRepetitions);
}

int8 setWANATMF5LoopbackDiagnostics_NumberOfRepetitionsValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32* NumberOfRepetitions;

	NumberOfRepetitions = (uint32*)(value);
	if(*NumberOfRepetitions < 1)
		return -1;
	cwmpSetF5LoopbackDiagnosticsNumOfRep(*NumberOfRepetitions,getWANLinkIndex(deviceNode));
	/*if the Diagnosis is in process, set F5LoopbackDiagnoseResetFlag*/
	if(cwmpGetF5LoopbackDiagnosticsResetFlag() == 0 && cwmpGetF5LoopbackDiagnosingFlag() == 1)
	{
		cwmpSetF5LoopbackDiagnosticsResetFlag(1);
	}
	return 0;
}

int8 getWANATMF5LoopbackDiagnostics_NumberOfRepetitionsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcatmDiagnosticAttr.aWANATMF5Loopback_NumberOfRepetitions[getWANLinkIndex(deviceNode)];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANATMF5LoopbackDiagnostics_NumberOfRepetitionsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcatmDiagnosticAttr.aWANATMF5Loopback_NumberOfRepetitions[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANATMF5LoopbackDiagnostics_TimeoutValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{

	char Timeout[20];
	atmF5LoopbackDiagnostis_t F5LoopbackDiagnositcs; 
	
	cwmpGetF5LoopbackDiagnosticsResult(&F5LoopbackDiagnositcs, getWANLinkIndex(deviceNode));
	if(F5LoopbackDiagnositcs.Timeout < 1)  //qingpu.20080402
		F5LoopbackDiagnositcs.Timeout =1;
	sprintf(Timeout, "%d", F5LoopbackDiagnositcs.Timeout);
	return getTheValue(value, type, deviceNode, Timeout);	
}

int8 setWANATMF5LoopbackDiagnostics_TimeoutValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32* timeout;
	timeout = (uint32*)(value);
	if(*timeout < 1)
		return -1;
	
	cwmpSetF5LoopbackDiagnosticsTimeout(*timeout,getWANLinkIndex(deviceNode));
	
	/*if the Diagnosis is in process, set F5LoopbackDiagnoseResetFlag*/
	if(cwmpGetF5LoopbackDiagnosticsResetFlag() == 0 && cwmpGetF5LoopbackDiagnosingFlag() == 1)
	{
		cwmpSetF5LoopbackDiagnosticsResetFlag(1);
	}
	
	return 0;
}

int8 getWANATMF5LoopbackDiagnostics_TimeoutAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcatmDiagnosticAttr.aWANATMF5Loopback_Timeout[getWANLinkIndex(deviceNode)];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANATMF5LoopbackDiagnostics_TimeoutAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcatmDiagnosticAttr.aWANATMF5Loopback_Timeout[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANATMF5LoopbackDiagnostics_SuccessCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char SuccessCount[20];
	atmF5LoopbackDiagnostis_t F5LoopbackDiagnositcs; 
	
	cwmpGetF5LoopbackDiagnosticsResult(&F5LoopbackDiagnositcs, getWANLinkIndex(deviceNode));
	sprintf(SuccessCount, "%lu", F5LoopbackDiagnositcs.SuccessCount);
	return getTheValue(value, type, deviceNode, SuccessCount);
}

int8 getWANATMF5LoopbackDiagnostics_SuccessCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcatmDiagnosticAttr.aWANATMF5Loopback_SuccessCount[getWANLinkIndex(deviceNode)];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANATMF5LoopbackDiagnostics_SuccessCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcatmDiagnosticAttr.aWANATMF5Loopback_SuccessCount[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANATMF5LoopbackDiagnostics_FailureCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char FailureCount[20];
	atmF5LoopbackDiagnostis_t F5LoopbackDiagnositcs; 
	
	cwmpGetF5LoopbackDiagnosticsResult(&F5LoopbackDiagnositcs, getWANLinkIndex(deviceNode));
	sprintf(FailureCount, "%lu", F5LoopbackDiagnositcs.FailureCount);
	return getTheValue(value, type, deviceNode, FailureCount);
}

int8 getWANATMF5LoopbackDiagnostics_FailureCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcatmDiagnosticAttr.aWANATMF5Loopback_FailureCount[getWANLinkIndex(deviceNode)];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANATMF5LoopbackDiagnostics_FailureCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcatmDiagnosticAttr.aWANATMF5Loopback_FailureCount[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANATMF5LoopbackDiagnostics_AverageResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char AverageResponseTime[20];
	atmF5LoopbackDiagnostis_t F5LoopbackDiagnositcs; 
	
	cwmpGetF5LoopbackDiagnosticsResult(&F5LoopbackDiagnositcs, getWANLinkIndex(deviceNode));
	sprintf(AverageResponseTime, "%lu", F5LoopbackDiagnositcs.AverageResponseTime);
	return getTheValue(value, type, deviceNode, AverageResponseTime);
}

int8 getWANATMF5LoopbackDiagnostics_AverageResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcatmDiagnosticAttr.aWANATMF5Loopback_AverageResponseTime[getWANLinkIndex(deviceNode)];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANATMF5LoopbackDiagnostics_AverageResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcatmDiagnosticAttr.aWANATMF5Loopback_AverageResponseTime[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANATMF5LoopbackDiagnostics_MinimumResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char MinimumResponseTime[20];
	atmF5LoopbackDiagnostis_t F5LoopbackDiagnositcs; 
	
	cwmpGetF5LoopbackDiagnosticsResult(&F5LoopbackDiagnositcs, getWANLinkIndex(deviceNode));
	sprintf(MinimumResponseTime, "%lu", F5LoopbackDiagnositcs.MinimumResponseTime);
	return getTheValue(value, type, deviceNode, MinimumResponseTime);	
}

int8 getWANATMF5LoopbackDiagnostics_MinimumResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcatmDiagnosticAttr.aWANATMF5Loopback_MinimumResponseTime[getWANLinkIndex(deviceNode)];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANATMF5LoopbackDiagnostics_MinimumResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcatmDiagnosticAttr.aWANATMF5Loopback_MinimumResponseTime[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getWANATMF5LoopbackDiagnostics_MaximumResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char MaximumResponseTime[20];
	atmF5LoopbackDiagnostis_t F5LoopbackDiagnositcs; 
	
	cwmpGetF5LoopbackDiagnosticsResult(&F5LoopbackDiagnositcs, getWANLinkIndex(deviceNode));
	sprintf(MaximumResponseTime, "%lu", F5LoopbackDiagnositcs.MaximumResponseTime);
	return getTheValue(value, type, deviceNode, MaximumResponseTime);
}

int8 getWANATMF5LoopbackDiagnostics_MaximumResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcatmDiagnosticAttr.aWANATMF5Loopback_MaximumResponseTime[getWANLinkIndex(deviceNode)];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANATMF5LoopbackDiagnostics_MaximumResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcatmDiagnosticAttr.aWANATMF5Loopback_MaximumResponseTime[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLDIAGNOSTIC_ATTR_FLAG;
	return result;
}

#endif

