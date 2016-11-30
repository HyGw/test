#include "Global_res.h"
#include "cp_ctqos.h"


/* qos parameters */
int8 getCTQoSModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char modeVal[MAX_BUF_LEN];

	memset(modeVal, 0, sizeof(modeVal));
	
	strcpy(nodeName, QOS_COMMON);
	tcapi_get(nodeName, "Mode", modeVal);
	if(modeVal[0] == '\0')
	{
		strcpy(modeVal,OTHER);
	}

	return getTheValue(value, type, deviceNode, modeVal);
}
int8 setCTQoSModeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char *pModeVal = (char*)value;
	char mode[64] = {0};
	int len = 0;

	strcpy(nodeName, QOS_COMMON);
	len = strlen(pModeVal);

	if(!strcmp(pModeVal, OTHER)){
		tcapi_set(nodeName, "Mode", pModeVal);
	}
	else if((len != 0) && (len < 64)){
		char *p = NULL;
		int tr69_flag = 0, voip_flag = 0, internet_flag = 0, iptv_flag = 0; 

		strcpy(mode, pModeVal);
		p=strtok(pModeVal, ",");
		while(p){
			if((strstr(p, "TR069"))&& (tr69_flag == 0)){
				tr69_flag = 1;
			}
			else if(strstr(p, "VOIP") && (voip_flag == 0)){
				voip_flag =1;
			}
			else if(strstr(p, "IPTV") && (iptv_flag == 0)){
				iptv_flag =1;
			}
			else if(strstr(p, "INTERNET") && (internet_flag == 0)){
				internet_flag =1;
			}
			else{
				tcdbg_printf("\nsetCTQoSModeValue, mode error!!");
				return -1;
			}
			p=strtok(NULL, ",");
		}
		tcapi_set(nodeName, "Mode", mode);
	}
	else {
		tcdbg_printf("\nsetCTQoSModeValue, string null or too long!!");
		return -1;
	}

	tcapi_set(nodeName, "Active", "No");
	SetCfgNodeFlag(QOS_COMMON, -1);

	return 0;
}
int8 getCTQoSModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcQoSAttr.aMode;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcQoSAttr.aMode;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;

	return result;
}

int8 getCTQoSEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char EnableVal[MAX_BUF_LEN];

	memset(EnableVal, 0, sizeof(EnableVal));
	
	strcpy(nodeName, QOS_COMMON);
	tcapi_get(nodeName, "Active", EnableVal);
	if(strcmp(EnableVal, "Yes") == 0)
	{
		strcpy(EnableVal, "1");
	}
	else
	{
		strcpy(EnableVal, "0");
	}

	return getTheValue(value, type, deviceNode, EnableVal);
}
int8 setCTQoSEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	uint8 *enable = (uint8 *)value;
	
	strcpy(nodeName, QOS_COMMON);

	if (*enable == 1) {
		tcapi_set(nodeName, "Active", "Yes");
	}
	else if (*enable == 0) {
		tcapi_set(nodeName, "Active", "No");
	}
	else {
		return -1;
	}

	SetCfgNodeFlag(QOS_COMMON, -1);
	
	return 0;
}

int8 getCTQoSEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcQoSAttr.aEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcQoSAttr.aEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;

	return result;
}

int8 getCTQoSBandwidthValue(char** value, ParameterType* type, DeviceNode* deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char bwValArr[20];

	memset(bwValArr, 0, sizeof(bwValArr));
	
	strcpy(nodeName, QOS_COMMON);
	tcapi_get(nodeName, "UplinkBandwidth", bwValArr);
	if(bwValArr[0] == '\0')
	{
		strcpy(bwValArr,"0");
	}
	else{
		sprintf(bwValArr, "%d", atoi(bwValArr)/1024);
	}

	return getTheValue(value, type, deviceNode, bwValArr);
}
int8 setCTQoSBandwidthValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 *pbwVal;
	char pbwValArr[10];
	char nodeName[MAX_NODE_NAME_SIZE];

	strcpy(nodeName, QOS_COMMON);
	pbwVal = (uint32 *)value;

	/* the max bandwidth value is 1024000 */
	if (*pbwVal > 1024000) {
		return -1;
	}

	sprintf(pbwValArr, "%u", *pbwVal*1024);
	/*  pbwVal is based bits */
	if (pbwVal > 0) {
		tcapi_set(nodeName, "UplinkBandwidth", pbwValArr);
	}
	else {
		return -1;
	}

	SetCfgNodeFlag(QOS_COMMON, -1);
	
	return 0;
}
int8 getCTQoSBandwidthAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
		
	attribute = tcQoSAttr.aBandwidth;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSBandwidthAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &tcQoSAttr.aBandwidth;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;

	return result;
}

int8 getCTQoSPlanValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char qosPlan[MAX_BUF_LEN];

	memset(qosPlan, 0, sizeof(qosPlan));

	strcpy(nodeName, QOS_COMMON);
	tcapi_get(nodeName, "Discipline", qosPlan);

	if (!strcmp(qosPlan, "PQ")) {
		strcpy(qosPlan, "priority");
	}
	else if (!strcmp(qosPlan, "WRR")) {
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
		strcpy(qosPlan, "weight");
#else
		strcpy(qosPlan, "wrr");
#endif
	}
	else if (!strcmp(qosPlan, "CAR")) {
		strcpy(qosPlan, "car");
	}
	else
	{
		strcpy(qosPlan, "priority");
	}

	return getTheValue(value, type, deviceNode, qosPlan);
}
int8 setCTQoSPlanValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char *qosPlan = (char*)value;

	strcpy(nodeName, QOS_COMMON);

	if (!strcmp(qosPlan, "priority")) {
		tcapi_set(nodeName, "Discipline", "PQ");
	}
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
    else if (!strcmp(qosPlan, "weight")) {
		tcapi_set(nodeName, "Discipline", "WRR");
	}
#else
	else if (!strcmp(qosPlan, "wrr")) {
		tcapi_set(nodeName, "Discipline", "WRR");
	}
#endif
	else if (!strcmp(qosPlan, "car")) {
		tcapi_set(nodeName, "Discipline", "CAR");
	}
	else {
		return -1;
	}

	SetCfgNodeFlag(QOS_COMMON, -1);
	
	return 0;
}
int8 getCTQoSPlanAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcQoSAttr.aPlan;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSPlanAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &tcQoSAttr.aPlan;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;

	return result;
}

int8 getCTQoSEnableForceWeightValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char enableFw[10];

	memset(enableFw, 0, sizeof(enableFw));
	
	strcpy(nodeName, QOS_COMMON);
	tcapi_get(nodeName, "EnableForceWeight", enableFw);

	if (!strcmp(enableFw, "Yes")) {
		strcpy(enableFw, "1");
	}
	
	#if 0
	else if (!strcmp(enableFw, "No")) {
		strcpy(enableFw, "0");
	}
	#else
	else{
		strcpy(enableFw, "0");
	}
	#endif
	
	return getTheValue(value, type, deviceNode, enableFw);
}
int8 setCTQoSEnableForceWeightValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	uint8 *enableFw = (uint8 *)value;

	strcpy(nodeName, QOS_COMMON);

	if (*enableFw == 1) {
		tcapi_set(nodeName, "EnableForceWeight", "Yes");
	}
	else if (*enableFw == 0) {
		tcapi_set(nodeName, "EnableForceWeight", "No");
	}
	else {
		return -1;
	}

	SetCfgNodeFlag(QOS_COMMON, -1);
	return 0;
}

int8 getCTQoSEnableForceWeightAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
			
	attribute = tcQoSAttr.aEnableForceWeight;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSEnableForceWeightAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
		
	attribute = &tcQoSAttr.aEnableForceWeight;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;

	return result;
}

int8 getCTQoSEnableDSCPMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char enableDm[10];

	memset(enableDm, 0, sizeof(enableDm));
	
	strcpy(nodeName, QOS_COMMON);
	tcapi_get(nodeName, "EnableDSCPMark", enableDm);

	if (!strcmp(enableDm, "Yes")) {
		strcpy(enableDm, "1");
	}
	#if 0
	else if (!strcmp(enableDm, "No")) {
		strcpy(enableDm, "0");
	}
	#else
	else{
		strcpy(enableDm, "0");
	}
	#endif

	return getTheValue(value, type, deviceNode, enableDm);
	
}
int8 setCTQoSEnableDSCPMarkValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	uint8 *enableDm = (uint8 *)value;

	//strcmp(nodeName, QOS_COMMON);
	strcpy(nodeName, QOS_COMMON);

	if (*enableDm == 1) {
		tcapi_set(nodeName, "EnableDSCPMark", "Yes");
	}
	else if (*enableDm == 0) {
		tcapi_set(nodeName, "EnableDSCPMark", "No");
	}
	else {
		return -1;
	}

	SetCfgNodeFlag(QOS_COMMON, -1);

	return 0;
}
int8 getCTQoSEnableDSCPMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
			
	attribute = tcQoSAttr.aEnableDSCPMark;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSEnableDSCPMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
		
	attribute = &tcQoSAttr.aEnableDSCPMark;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;

	return result;
}

int8 getCTQoSEnable8021PValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char enabledotp[10];
	
	memset(enabledotp, 0, sizeof(enabledotp));

	strcpy(nodeName, QOS_COMMON);
	tcapi_get(nodeName, "En8021PRemark", enabledotp);
	
	if ((0 != strcmp(enabledotp, "0")) && (0 != strcmp(enabledotp, "1")) && (0 != strcmp(enabledotp, "2"))) {
		#if 0
		memset(enabledotp, 0, sizeof(enabledotp));
		#else
		strcpy(enabledotp,"0");
		#endif
	}

	return getTheValue(value, type, deviceNode, enabledotp);
}
int8 setCTQoSEnable8021PValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char dotArr[4];
	uint32 *enabledotp = (uint32 *)value;

	strcpy(nodeName, QOS_COMMON);
	if (*enabledotp >=0 && *enabledotp < 3) {
		sprintf(dotArr, "%d", *enabledotp);
		tcapi_set(nodeName, "En8021PRemark", dotArr);
	}

	SetCfgNodeFlag(QOS_COMMON, -1);

	return 0;
}
int8 getCTQoSEnable8021PAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
			
	attribute = tcQoSAttr.aEnable8021P;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSEnable8021PAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
		
	attribute = &tcQoSAttr.aEnable8021P;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;

	return result;
}

/* app parameters */
int8 getCTQoSAppNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char appName[20];
	int instance;

	memset(appName, 0, sizeof(appName));

	instance = atoi(deviceNode->parentNode->name) + MAX_TYPE_ENTRY_NUM - 1;
	sprintf(nodeName, QOS_ENTRY, instance);

	tcapi_get(nodeName, "AppName", appName);

	return getTheValue(value, type, deviceNode, appName);
}
int8 setCTQoSAppNameValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char cfg_app_name[20];
	char *appName = (char *)value;
	int ret, i, instance;
	
	memset(cfg_app_name, 0, sizeof(cfg_app_name));
	instance = atoi(deviceNode->parentNode->name) + MAX_TYPE_ENTRY_NUM - 1;
	sprintf(nodeName, QOS_ENTRY, instance);

	/* now we only support three kinds of app */
	if (!strcmp(appName, "VOIP") ||
		!strcmp(appName, "TR069") || 
		!strcmp(appName, "IPTV")) {		
		sprintf(nodeName, QOS_ENTRY, instance);
		tcapi_set(nodeName, "AppName", appName);
	}
	else {
		return -1;
	}

	SetCfgNodeFlag("QoS", instance);

	return 0;
}
int8 getCTQoSAppNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aAppName[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSAppNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aAppName[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getCTQoSAppClassQueueValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char appQueue[4];
	int instance;

	memset(appQueue, 0, sizeof(appQueue));
	
	instance = atoi(deviceNode->parentNode->name) + MAX_TYPE_ENTRY_NUM -1;
	sprintf(nodeName, QOS_ENTRY, instance);

	tcapi_get(nodeName, "ActQueue", appQueue);
	if(appQueue[0] == '\0')
	{	
		strcpy(appQueue,"1");
	}
	
	return getTheValue(value, type, deviceNode, appQueue);
}
int8 setCTQoSAppClassQueueValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char acqArr[4];
	char queuebuf[10];
	uint32 *acq = (uint32 *)value;
	int instance, queuenum = 0;

	memset(acqArr, 0, sizeof(acqArr));

	strcpy(nodeName, QOS_COMMON);
	tcapi_get(nodeName, "Discipline", queuebuf);
	if (!strcmp(queuebuf, "PQ") || !strcmp(queuebuf, "WRR")) {
		queuenum = 4;
	}
	else if (!strcmp(queuebuf, "CAR")) {
		queuenum = 6;
	}
	else {
		return -1;
	}
	
	instance = atoi(deviceNode->parentNode->name) + MAX_TYPE_ENTRY_NUM -1;
	sprintf(nodeName, QOS_ENTRY, instance);

	if (*acq > 0 && *acq <= queuenum) {
		sprintf(acqArr, "%d", *acq);
		tcapi_set(nodeName, "ActQueue", acqArr);
	}
	else {
		return -1;
	}

	SetCfgNodeFlag("QoS", instance);

	return 0;
}
int8 getCTQoSAppClassQueueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aAppClassQueue[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSAppClassQueueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aAppClassQueue[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

/* classqueue parameters */
int8 getCTQoSCSFClassQueueValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char csfQueue[4];
	int instance;

	instance = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, QOS_ENTRY, instance);

	tcapi_get(nodeName, "ActQueue", csfQueue);
	if(csfQueue[0] == '\0')
	{	
		strcpy(csfQueue,"1");
	}

	return getTheValue(value, type, deviceNode, csfQueue);
}
int8 setCTQoSCSFClassQueueValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char csfQueueArr[4];
	char queuebuf[10];
	uint32 *csfQueue = (uint32 *)value;
	int instance, queuenum;

	strcpy(nodeName, QOS_COMMON);
	tcapi_get(nodeName, "Discipline", queuebuf);
	if (!strcmp(queuebuf, "PQ") || !strcmp(queuebuf, "WRR") || !strcmp(queuebuf, "SP")) {
		queuenum = 4;
	}
	else if (!strcmp(queuebuf, "CAR")) {
		queuenum = 6;
	}
	else {
		return -1;
	}
	
	instance = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, QOS_ENTRY, instance);

	if (*csfQueue > 0 && *csfQueue <= queuenum) {
		sprintf(csfQueueArr, "%d", *csfQueue);
		tcapi_set(nodeName, "ActQueue", csfQueueArr);
	}
	else {
		return -1;
	}

	SetCfgNodeFlag("QoS", instance);

	return 0;
}
int8 getCTQoSCSFClassQueueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aCSFClassQueue[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSCSFClassQueueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aCSFClassQueue[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getCTQoSDSCPMarkValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char dscpMarkVal[4];
	int instance;

	instance = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, QOS_ENTRY, instance);

	tcapi_get(nodeName, "ActDSCPRemarking", dscpMarkVal);
	
	return getTheValue(value, type, deviceNode, dscpMarkVal);
}
int8 setCTQoSDSCPMarkValueValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char dscpMarkValArr[4];
	uint32 *dscpMarkVal = (uint32 *)value;
	int instance;

	instance = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, QOS_ENTRY, instance);

	if (*dscpMarkVal >= 0 && *dscpMarkVal <= 63) {
		sprintf(dscpMarkValArr, "%u", *dscpMarkVal);
		tcapi_set(nodeName, "ActDSCPRemarking", dscpMarkValArr);
	}
	else {
		return -1;
	}

	SetCfgNodeFlag("QoS", instance);

	return 0;
	
}
int8 getCTQoSDSCPMarkValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aDSCPMarkValue[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSDSCPMarkValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aDSCPMarkValue[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getCTQoSCSF8021PValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char dot1p[4];
	int instance;

	instance = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, QOS_ENTRY, instance);

	tcapi_get(nodeName, "Act8021pRemarkingNum", dot1p);

	return getTheValue(value, type, deviceNode, dot1p);
}
int8 setCTQoSCSF8021PValueValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char dot1pArr[4];
	uint32 *dot1p = (uint32*)value;
	int instance;

	instance = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, QOS_ENTRY, instance);

	if (*dot1p >= 0 && *dot1p <= 7) {
		sprintf(dot1pArr, "%u", *dot1p);
		tcapi_set(nodeName, "Act8021pRemarkingNum", dot1pArr);
	}
	else {
		return -1;
	}

	SetCfgNodeFlag("QoS", instance);
	return 0;
}
int8 getCTQoSCSF8021PValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.a8021PValue[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSCSF8021PValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.a8021PValue[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getCTQoSCSFTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char typeName[MAX_BUF_LEN], typeVal[MAX_BUF_LEN];
	int i, instance1, instance2;

	instance1 = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	instance2 = atoi(deviceNode->parentNode->name) - 1;

	sprintf(nodeName, QOS_ENTRY, instance1);
	sprintf(typeName, "Type%d", instance2 + 1);

	memset(typeVal, 0, sizeof(typeVal));
	tcapi_get(nodeName, typeName, typeVal);
	
	return getTheValue(value, type, deviceNode, typeVal);
}

int8 setCTQoSCSFTypeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char typeName[MAX_BUF_LEN];
	char *typeVal = (char *)value;
	int i, instance1, instance2;

	instance1 = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	instance2 = atoi(deviceNode->parentNode->name) - 1;

	sprintf(nodeName, QOS_ENTRY, instance1);
	sprintf(typeName, "Type%d", instance2 + 1);

	if (!strcmp(typeVal, "SMAC") ||
		!strcmp(typeVal, "8021P") ||
		!strcmp(typeVal, "SIP") ||
		!strcmp(typeVal, "DIP") ||
		!strcmp(typeVal, "SPORT") ||
		!strcmp(typeVal, "DPORT") ||
		!strcmp(typeVal, "DSCP") ||
		!strcmp(typeVal, "WANInterface") ||
		!strcmp(typeVal, "LANInterface")) {
		tcapi_set(nodeName, typeName, typeVal);
	}
	else {
		return -1;
	}

	SetCfgNodeFlag("QoS", instance1);
	return 0;
	
}
int8 getCTQoSCSFTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance1, instance2;

	instance1 = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	instance2 = atoi(deviceNode->parentNode->name);
	attribute = tcQoSAttr.aType[instance1 - 1][instance2 - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSCSFTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance1, instance2;

	instance1 = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	instance2 = atoi(deviceNode->parentNode->name);
	attribute = &tcQoSAttr.aType[instance1 - 1][instance2 - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getCTQoSMaxValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char typeName[MAX_BUF_LEN], typeVal[MAX_BUF_LEN];
	char typeMaxVal[128];
	int i, instance1, instance2, ret;

	instance1 = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	instance2 = atoi(deviceNode->parentNode->name) - 1;

	/* 128 length is enough */
	memset(typeMaxVal, 0, sizeof(typeMaxVal));
	sprintf(nodeName, QOS_ENTRY, instance1);

	sprintf(typeName, "Type%d", instance2 + 1);
	ret = tcapi_get(nodeName, typeName, typeVal);

	if (!strcmp(typeVal, "WANInterface")) {
		get_wan_if_name(typeMaxVal, instance1, instance2, IF_END);
	}
	else if (!strcmp(typeVal, "LANInterface")) {
		get_lan_if_name(typeMaxVal, instance1, instance2, IF_END);
	}
	else {
		sprintf(typeName, "Max%d", instance2 + 1);
		tcapi_get(nodeName, typeName, typeMaxVal);
	}
	
	return getTheValue(value, type, deviceNode, typeMaxVal);
}
int8 setCTQoSMaxValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char typeName[MAX_BUF_LEN], typeVal[MAX_BUF_LEN];
	char *typeMaxVal = (char *)value;
	int i, instance1, instance2, ret;

	instance1 = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	instance2 = atoi(deviceNode->parentNode->name) - 1;

	sprintf(nodeName, QOS_ENTRY, instance1);

	sprintf(typeName, "Type%d", instance2 + 1);
	ret = tcapi_get(nodeName, typeName, typeVal);

	if (!strcmp(typeVal, "SMAC")){
		if (checkMACFormat(typeMaxVal) < 0) {
			tc_printf("%s: source mac address format is wrong.\n", __FUNCTION__);
			return -1;
		}
	}
	else if (!strcmp(typeVal, "8021P")) {
		if (atoi(typeMaxVal) < 0 || atoi(typeMaxVal) > 7) {
			tc_printf("%s: 802.1p format is wrong.\n", __FUNCTION__);
			return -1;
		}
	}
	else if (!strcmp(typeVal, "SIP")) {
		if (_checkIp(typeMaxVal) < 0) {
			tc_printf("%s: source mac address format is wrong.\n", __FUNCTION__);
			return -1;
		}
	}
	else if (!strcmp(typeVal, "DIP")) {
		if (_checkIp(typeMaxVal) < 0) {
			tc_printf("%s: des mac address format is wrong.\n", __FUNCTION__);
			return -1;
		}
	}
	else if (!strcmp(typeVal, "SPORT")) {
		if (atoi(typeMaxVal) <-1 || atoi(typeMaxVal) > 65535) {
			tc_printf("%s: source port format is wrong.\n", __FUNCTION__);
			return -1;
		}
	}
	else if (!strcmp(typeVal, "DPORT")) {
		if (atoi(typeMaxVal) < -1 || atoi(typeMaxVal) > 65535) {
			tc_printf("%s: des port format is wrong.\n", __FUNCTION__);
			return -1;
		}
	}
	else if (!strcmp(typeVal, "DSCP")) {
		if (atoi(typeMaxVal) < 0 || atoi(typeMaxVal) > 63) {
			tc_printf("%s: dscp format is wrong.\n", __FUNCTION__);
			return -1;
		}
	}
	else if (!strcmp(typeVal, "WANInterface")) {
		ret = chk_and_set_wanif(typeMaxVal, instance1, instance2, IF_END);
		if (0 == ret) {
			SetCfgNodeFlag("QoS", instance1);
		}
		return ret;
	}
	else if (!strcmp(typeVal, "LANInterface")) {
		ret = chk_and_set_lanif(typeMaxVal, instance1, instance2, IF_END);
		if (0 == ret) {
			SetCfgNodeFlag("QoS", instance1);
		}
		return ret;
	}
	else {
		return -1;
	}

	sprintf(typeName, "Max%d", instance2 + 1);
	tcapi_set(nodeName, typeName, typeMaxVal);

	SetCfgNodeFlag("QoS", instance1);

	return 0;
}
int8 getCTQoSMaxAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance1, instance2;

	instance1 = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	instance2 = atoi(deviceNode->parentNode->name);
	attribute = tcQoSAttr.aMax[instance1 - 1][instance2 - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSMaxAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance1, instance2;

	instance1 = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	instance2 = atoi(deviceNode->parentNode->name);
	attribute = &tcQoSAttr.aMax[instance1 - 1][instance2 - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getCTQoSMinValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char typeName[MAX_BUF_LEN], typeVal[MAX_BUF_LEN];
	char typeMaxVal[128];
	int i, instance1, instance2, ret;

	instance1 = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	instance2 = atoi(deviceNode->parentNode->name) - 1;

	/* 128 length is enough */
	memset(typeMaxVal, 0, sizeof(typeMaxVal));
	sprintf(nodeName, QOS_ENTRY, instance1);

	sprintf(typeName, "Type%d", instance2 + 1);
	ret = tcapi_get(nodeName, typeName, typeVal);

	if (!strcmp(typeVal, "WANInterface")) {
		get_wan_if_name(typeMaxVal, instance1, instance2, IF_BEGIN);
	}
	else if (!strcmp(typeVal, "LANInterface")) {
		get_lan_if_name(typeMaxVal, instance1, instance2, IF_BEGIN);
	}
	else {
		sprintf(typeName, "Min%d", instance2 + 1);
		tcapi_get(nodeName, typeName, typeMaxVal);
	}
	
	return getTheValue(value, type, deviceNode, typeMaxVal);
}

int8 setCTQoSMinValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char typeName[MAX_BUF_LEN], typeVal[MAX_BUF_LEN];
	char *typeMinVal = (char *)value;
	int i, instance1, instance2, ret;

	instance1 = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	instance2 = atoi(deviceNode->parentNode->name) - 1;

	sprintf(nodeName, QOS_ENTRY, instance1);

	sprintf(typeName, "Type%d", instance2 + 1);
	ret = tcapi_get(nodeName, typeName, typeVal);

	if (!strcmp(typeVal, "SMAC")){
		if (checkMACFormat(typeMinVal) < 0) {
			tc_printf("%s: source mac address format is wrong.\n", __FUNCTION__);
			return -1;
		}
	}
	else if (!strcmp(typeVal, "8021P")) {
		if (atoi(typeMinVal) < 0 || atoi(typeMinVal) > 7) {
			tc_printf("%s: 802.1p format is wrong.\n", __FUNCTION__);
			return -1;
		}
	}
	else if (!strcmp(typeVal, "SIP")) {
		if (_checkIp(typeMinVal) < 0) {
			tc_printf("%s: source mac address format is wrong.\n", __FUNCTION__);
			return -1;
		}
	}
	else if (!strcmp(typeVal, "DIP")) {
		if (_checkIp(typeMinVal) < 0) {
			tc_printf("%s: des mac address format is wrong.\n", __FUNCTION__);
			return -1;
		}
	}
	else if (!strcmp(typeVal, "SPORT")) {
		if (atoi(typeMinVal) <-1 || atoi(typeMinVal) > 65535) {
			tc_printf("%s: source port format is wrong.\n", __FUNCTION__);
			return -1;
		}
	}
	else if (!strcmp(typeVal, "DPORT")) {
		if (atoi(typeMinVal) < -1 || atoi(typeMinVal) > 65535) {
			tc_printf("%s: des port format is wrong.\n", __FUNCTION__);
			return -1;
		}
	}
	else if (!strcmp(typeVal, "DSCP")) {
		if (atoi(typeMinVal) < 0 || atoi(typeMinVal) > 63) {
			tc_printf("%s: dscp format is wrong.\n", __FUNCTION__);
			return -1;
		}
	}
	else if (!strcmp(typeVal, "WANInterface")) {
		ret = chk_and_set_wanif(typeMinVal, instance1, instance2, IF_BEGIN);
		if (0 == ret) {
			SetCfgNodeFlag("QoS", instance1);
		}
		return ret;
	}
	else if (!strcmp(typeVal, "LANInterface")) {
		ret = chk_and_set_lanif(typeMinVal, instance1, instance2, IF_BEGIN);
		if (0 == ret) {
			SetCfgNodeFlag("QoS", instance1);
		}
		return ret;
	}
	else {
		return -1;
	}

	sprintf(typeName, "Min%d", instance2 + 1);
	tcapi_set(nodeName, typeName, typeMinVal);

	SetCfgNodeFlag("QoS", instance1);

	return 0;
}
int8 getCTQoSMinAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance1, instance2;

	instance1 = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	instance2 = atoi(deviceNode->parentNode->name);
	attribute = tcQoSAttr.aMin[instance1 - 1][instance2 - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSMinAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance1, instance2;

	instance1 = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	instance2 = atoi(deviceNode->parentNode->name);
	attribute = &tcQoSAttr.aMin[instance1 - 1][instance2 - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getCTQoSProtocolListValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char protoList[32];
	int instance;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	sprintf(nodeName, QOS_ENTRY, instance);

	tcapi_get(nodeName, "ProtocolID", protoList);

#if 0
	if (!strcmp(protoList, "TCP/UDP")) {
		strcpy(protoList, "TCP,UDP");
	}
#else
	if (!strcmp(protoList, "rtp")) {
		strcpy(protoList, "RTP");
	}
#endif
	
	return getTheValue(value, type, deviceNode, protoList);
}
int8 setCTQoSProtocolListValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char *protoList = (char *)value;
	int instance;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	sprintf(nodeName, QOS_ENTRY, instance);

	if (!strcmp(protoList, "TCP") ||
		!strcmp(protoList, "UDP") ||
		!strcmp(protoList, "TCP,UDP") ||
		!strcmp(protoList, "ICMP") ||
		!strcmp(protoList, "IGMP")) {
		tcapi_set(nodeName, "ProtocolID", protoList);
	}
#if 0
	else if (!strcmp(protoList, "TCP,UDP")) {
		tcapi_set(nodeName, "ProtocolID", "TCP/UDP");
	}
#endif
	else if (!strcmp(protoList, "RTP")) {
		tcapi_set(nodeName, "ProtocolID", "rtp");
	}
	else if (!strcmp(protoList, "ALL")) {
		tcapi_set(nodeName, "ProtocolID", "all");
	}
	else {
		return -1;
	}

	SetCfgNodeFlag("QoS", instance);

	return 0;
}
int8 getCTQoSProtocolListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance1, instance2;

	instance1 = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	instance2 = atoi(deviceNode->parentNode->name);
	attribute = tcQoSAttr.aProtocolList[instance1 - 1][instance2 - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSProtocolListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance1, instance2;

	instance1 = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	instance2 = atoi(deviceNode->parentNode->name);
	attribute = &tcQoSAttr.aProtocolList[instance1 - 1][instance2 - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getCTQoSPQEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char queueBuf[10];
	char queueSW[10];
	int instance;

	instance = atoi(deviceNode->parentNode->name);

	strcpy(nodeName, QOS_COMMON);

	sprintf(queueBuf, "QueueSW%d", instance);

	tcapi_get(nodeName, queueBuf, queueSW);

	if (!strcmp(queueSW, "Yes")) {
		strcpy(queueSW, "1");
	}
	#if 0
	else if (!strcmp(queueSW, "No")) {
		strcpy(queueSW, "0");
	}
	#else
	else{
		strcpy(queueSW, "0");
	}
	#endif

	return getTheValue(value, type, deviceNode, queueSW);	
}
int8 setCTQoSPQEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char queueBuf[10];
	uint8 *pqenable = (uint8 *)value;
	int instance;

	instance = atoi(deviceNode->parentNode->name);

	strcpy(nodeName, QOS_COMMON);
	
	sprintf(queueBuf, "QueueSW%d", instance);
	
	if (*pqenable == 1) {
		tcapi_set(nodeName, queueBuf, "Yes");
	}
	else if (*pqenable == 0) {
		tcapi_set(nodeName, queueBuf, "No");
	}
	else {
		return -1;
	}

	SetCfgNodeFlag(QOS_COMMON, -1);

	return 0;
}

int8 getCTQoSPQEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	attribute = tcQoSAttr.aPQEnable[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSPQEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcQoSAttr.aPQEnable[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getCTQoSPriorityValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char queueBuf[10];
	char queueP[10];
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);

	strcpy(nodeName, QOS_COMMON);
	
	sprintf(queueBuf, "QueueP%d", instance);

	tcapi_get(nodeName, queueBuf, queueP);

	return getTheValue(value, type, deviceNode, queueP);	
}
int8 setCTQoSPriorityValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char queueBuf[10];
	char pqbuf[10];
	uint32 *pq = (uint32 *)value;
	int instance;

	instance = atoi(deviceNode->parentNode->name);

	strcpy(nodeName, QOS_COMMON);
	sprintf(queueBuf, "QueueP%d", instance);
	
	if (*pq > 0 && *pq <= 6) {
		sprintf(pqbuf, "%d", *pq);
		tcapi_set(nodeName, queueBuf, pqbuf);
	}

	SetCfgNodeFlag(QOS_COMMON, -1);

	return 0;
}

int8 getCTQoSPriorityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	attribute = tcQoSAttr.aPriority[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSPriorityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcQoSAttr.aPriority[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getCTQoSWeightValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char queueBuf[10];
	char queueBW[10];
	char discpline[10];
	int instance;

	memset(queueBW, 0, sizeof(queueBW));
	
	instance = atoi(deviceNode->parentNode->name);

	strcpy(nodeName, QOS_COMMON);
	
	sprintf(queueBuf, "QueueBW%d", instance);

	tcapi_get(nodeName, "Discipline", discpline);

	/* only wrr discpline the weight make sense */
	//if (!strcmp(discpline, "WRR")) {
		tcapi_get(nodeName, queueBuf, queueBW);
	//}

	return getTheValue(value, type, deviceNode, queueBW);
}
int8 setCTQoSWeightValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char queueBuf[10];
	char qbwBuf[10];
	uint32 *qbw = (uint32 *)value;
	int instance;

	instance = atoi(deviceNode->parentNode->name);

	strcpy(nodeName, QOS_COMMON);

	sprintf(queueBuf, "QueueBW%d", instance);

	//if (*qbw >= 0 && *qbw <= 100) {
		sprintf(qbwBuf, "%d", *qbw);
		tcapi_set(nodeName, queueBuf, qbwBuf);
	//}
	//else {
	//	return -1;
	//}

	SetCfgNodeFlag(QOS_COMMON, -1);

	return 0;
}

int8 getCTQoSWeightAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	attribute = tcQoSAttr.aWeight[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTQoSWeightAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcQoSAttr.aWeight[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 DeviceNodeAddTypeObject(uint32 *num, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char typeName[MAX_BUF_LEN], type_max_min[MAX_BUF_LEN];
	char typeVal[MAX_BUF_LEN];
	int i, ret, instance;

	instance = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, QOS_ENTRY, instance);
	
	for (i = 0; i < MAX_TYPE_NUM; i++) {
		sprintf(typeName, "Type%d", i + 1);
		ret = tcapi_get(nodeName, typeName, typeVal);
		/* if the type name is 'N/A', it means this type can be used */
		if (ret < 0 || (ret == 0 && !strcmp(typeVal, "N/A"))) {
			break;
		}
	}

	if (i < MAX_TYPE_NUM) {
		tcapi_set(nodeName, typeName, "");
		
		sprintf(type_max_min, "Max%d", i + 1);
		tcapi_set(nodeName, type_max_min, "");
		
		sprintf(type_max_min, "Min%d", i + 1);
		tcapi_set(nodeName, type_max_min, "");
		*num = i + 1;
	}
	else {
		return -1;
	}

	/* init the type tree again */
	cwmpFreeDeviceTable(deviceNode);
	ret = cwmpInitTypeSubTree(deviceNode);

	return 0;
}
int8 DeviceNodeDelTypeObject(uint32 num, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char typeName[MAX_BUF_LEN];
	int instance, ret;

	instance = atoi(deviceNode->parentNode->parentNode->name) - 1;
	sprintf(nodeName, QOS_ENTRY, instance);

	sprintf(typeName, "Type%d", num);
	/* if there type name is 'N/A', it means this type is invalid */
	tcapi_set(nodeName, typeName, "N/A");

	/* delete commit */
	
	/* init the app tree again */
	cwmpFreeDeviceTable(deviceNode->parentNode);
	ret = cwmpInitTypeSubTree(deviceNode);

	return 0;
}
int	cwmpInitTypeSubTree(DeviceNodePtr deviceNode)
{
	int i, instance, ret;
	char typeName[MAX_BUF_LEN];
	char typeVal[MAX_BUF_LEN];
	char nodeName[MAX_NODE_NAME_SIZE];
	char index[4];

	instance = atoi(deviceNode->parentNode->name);
	sprintf(nodeName, "QoS_Entry%d", instance - 1);

	for (i = 0; i < MAX_TYPE_NUM; i++) {
		sprintf(typeName, "Type%d", i + 1);
		ret = tcapi_get(nodeName, typeName, typeVal);
		if (ret < 0 || ((ret == 0) && !strcmp(typeVal, "N/A"))) {
			continue;
		}

		sprintf(index, "%d", i + 1);
		TypeIndex[0].name = index;
		if (RegisterNodeFromTable(deviceNode, TypeIndex) == NULL) {
			return -1;
		}
	}

	return 0;
}

/* app init function here */
int8 DeviceNodeAddAppObject(uint32 *num, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char appName[20];
	int i, ret;

	for (i = 0; i < MAX_APP_ENTRY_NUM; i++) {
		sprintf(nodeName, QOS_ENTRY, i + MAX_TYPE_ENTRY_NUM);
		ret = tcapi_get(nodeName, "AppName", appName);
		if (ret < 0) {
			break;
		}
	}

	if (i == MAX_APP_ENTRY_NUM) {
		return -1;
	}
	
	*num = i + 1;

	tcapi_set(nodeName, "AppName", "");
	tcapi_set(nodeName, "ActQueue", "1");

	/* init the app tree again */
	cwmpFreeDeviceTable(deviceNode);
	ret = cwmpInitAppSubTree(deviceNode);

	return ret;
}

int8 DeviceNodeDelAppObject(uint32 num, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	int ret;

	sprintf(nodeName, QOS_ENTRY, num - 1 + MAX_TYPE_ENTRY_NUM);

	tcapi_unset(nodeName);
	tcapi_set(QOS_COMMON, "QoSOptType", "appRule");
	tcapi_commit(nodeName);

	/* init the app tree again */
	cwmpFreeDeviceTable(deviceNode->parentNode);
	ret = cwmpInitAppSubTree(deviceNode->parentNode);

	return ret;
}

int	cwmpInitAppSubTree( DeviceNodePtr deviceNode )
{
	int i, ret, rtnCode = 0;
	char nodeName[MAX_NODE_NAME_SIZE];
	char appName[MAX_BUF_LEN];
	char index[4];

	memset(nodeName, 0, sizeof(nodeName));
	memset(appName, 0, sizeof(appName));
	memset(index, 0, sizeof(index));
	
	for (i = 0; i < MAX_APP_ENTRY_NUM; i++) {
		/* app entry index is from 10 to 13 */
		sprintf(nodeName, QOS_ENTRY, i + MAX_TYPE_ENTRY_NUM);
		ret = tcapi_get(nodeName, "AppName", appName);
		if (ret < 0) {
			continue;
		}

		sprintf(index, "%d", i+1);

		AppIndex[0].name = index;

		if (RegisterNodeFromTable(deviceNode, AppIndex) == NULL) {
			rtnCode = -1;
			return rtnCode;
		}
	}

	return rtnCode;
}

/* class queue init function here */
int8 DeviceNodeAddClassificationObject(uint32 *num, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char actQueue[10];
	int i, ret;

	for (i = 0; i < MAX_TYPE_ENTRY_NUM; i++) {
		sprintf(nodeName, QOS_ENTRY, i);
		ret = tcapi_get(nodeName, "ActQueue", actQueue);
		if (ret < 0) {
			break;
		}
	}

	if (i == MAX_TYPE_ENTRY_NUM) {
		return -1;
	}

	*num = i + 1;

	tcapi_set(nodeName, "ActQueue", "");
	//tcapi_commit(nodeName);

	cwmpFreeDeviceTable(deviceNode);
	ret = cwmpInitClassificationSubTree(deviceNode);

	return ret;
}

int8 DeviceNodeDelClassificationObject(uint32 num, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	int i, ret;

	sprintf(nodeName, QOS_ENTRY, num - 1);

	tcapi_unset(nodeName);
	tcapi_set(QOS_COMMON, "QoSOptType", "typeRule");
	tcapi_commit(nodeName);

	/* init classification object again */
	cwmpFreeDeviceTable(deviceNode->parentNode);
	ret = cwmpInitClassificationSubTree(deviceNode->parentNode);

	return ret;
}

int cwmpInitClassificationSubTree( DeviceNodePtr deviceNode )
{
	int i, ret, rtnCode = 0;
	char nodeName[MAX_NODE_NAME_SIZE];
	char actQueue[MAX_BUF_LEN];
	char index[4];

	memset(nodeName, 0, sizeof(nodeName));
	memset(actQueue, 0, sizeof(actQueue));
	memset(index, 0, sizeof(index));
	
	for (i = 0; i < MAX_TYPE_ENTRY_NUM; i++) {
		/* type entry index is from 0 to 10 */
		sprintf(nodeName, QOS_ENTRY, i);
		ret = tcapi_get(nodeName, "ActQueue", actQueue);
		if (ret < 0) {
			continue;
		}

		sprintf(index, "%d", i+1);

		ClassificationIndex[0].name = index;

		if (RegisterNodeFromTable(deviceNode, ClassificationIndex) == NULL) {
			rtnCode = -1;
			return rtnCode;
		}
	}

	return rtnCode;
}

/* priority queue init function */
int8 DeviceNodeAddPriorityQueueObject(uint32 *num, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char disc[10], queueBuf[10], queueVal[10];
	int i, ret, queuenum = 0;

	memset(queueBuf, 0, sizeof(queueBuf));
	memset(queueVal, 0, sizeof(queueVal));
	memset(disc, 0, sizeof(disc));
	strcpy(nodeName, QOS_COMMON);

	ret = tcapi_get(nodeName, "Discipline", disc);

	if (!strcmp(disc, "PQ") || !strcmp(disc, "WRR") || !strcmp(disc, "SP")) {
		queuenum = 4;
	}
	else if (!strcmp(disc, "CAR")) {
		queuenum = 6;
	}

	for (i = queuenum + 1; i < MAX_QUEUE_NUM; i++) {
		sprintf(queueBuf, "QueueP%d", i);
		ret = tcapi_get(nodeName, queueBuf, queueVal);
		if (ret < 0 || (0 == ret && !strcmp(queueVal, "N/A"))) {
			break;
		}
	}
	
	if (MAX_QUEUE_NUM == i) {
		return -1;
	}
	
	*num = i;
	
	tcapi_set(nodeName, queueBuf, "");

	/* init the app tree again */
	cwmpFreeDeviceTable(deviceNode);
	ret = cwmpInitPriorityQueueSubTree(deviceNode);
	return ret;
}

int8 DeviceNodeDelPriorityQueueObject(uint32 num, DeviceNode*  deviceNode)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char disc[10], queueBuf[10];
	int i, ret, queueNum = 0;
	
	memset(disc, 0, sizeof(disc));
	memset(queueBuf, 0, sizeof(queueBuf));
	
	strcpy(nodeName, QOS_COMMON);
	tcapi_get(nodeName, "Discipline", disc);
	if (!strcmp(disc, "PQ") || !strcmp(disc, "WRR")) {
		queueNum = 4;
	}
	else if (!strcmp(disc, "CAR")) {
		queueNum = 6;
	}

	if (num <= queueNum) {
		tc_printf("==>DeviceNodeDelPriorityQueueObject(): default queue can not be deleted.\n");
		return -1;
	}

	/* if queue val is 'N/A', it means this queue is deleted */
	sprintf(queueBuf, "QueueP%d", num);
	tcapi_set(nodeName, queueBuf, "N/A");

	sprintf(queueBuf, "QueueSW%d", num);
	tcapi_set(nodeName, queueBuf, "No");
	
	tcapi_set(nodeName, "QoSOptType", "discRule");
	tcapi_commit(nodeName);

	/* init the app tree again */
	cwmpFreeDeviceTable(deviceNode->parentNode);
	ret = cwmpInitPriorityQueueSubTree(deviceNode->parentNode);

	return ret;
}

int cwmpInitPriorityQueueSubTree(DeviceNodePtr deviceNode)
{
	int i, ret, queueNum = 0, rtnCode = 0;
	char nodeName[MAX_NODE_NAME_SIZE];
	char index[4];
	char queueBuf[10], queueVal[10];

	memset(nodeName, 0, sizeof(nodeName));
	memset(index, 0, sizeof(index));
	memset(queueBuf, 0, sizeof(queueBuf));
	memset(queueVal, 0, sizeof(queueVal));

	strcpy(nodeName, QOS_COMMON);
	
	for (i = 0; i < MAX_QUEUE_NUM; i++) {
		sprintf(queueBuf, "QueueP%d", i + 1);
		ret = tcapi_get(nodeName, queueBuf, queueVal);

		if (0 == ret && 0 != strcmp(queueVal, "N/A")) {

			sprintf(index, "%d", i + 1);

			PriorityQueueIndex[0].name = index;

			if (RegisterNodeFromTable(deviceNode, PriorityQueueIndex) == NULL) {
				rtnCode = -1;
				return rtnCode;
			}
		}
	}

	return rtnCode;
}


int ctqosCommit(cfgNode_ptr pCfgNode)
{
	int ruleIndex;
	int i = 0, j = 0;
	char rollbackEntry[PKT_QOS_RULE_MAX][32];
	char nodeName[32];
	char classQueue[16];
	int ret = 0;
	
	memset(rollbackEntry, 0, sizeof(rollbackEntry));
	
	for (ruleIndex = 0; ruleIndex < MAX_TYPE_ENTRY_NUM + MAX_APP_ENTRY_NUM; ruleIndex++) {
		strcpy(nodeName, QOS_COMMON);
		if (ruleIndex < MAX_TYPE_ENTRY_NUM) {
			tcapi_set(nodeName, "QoSOptType", "typeRule");		
		}
		else {
			tcapi_set(nodeName, "QoSOptType", "appRule");
		}
		
		if (pCfgNode->bitMap & (1<<ruleIndex)) {

			sprintf(nodeName, QOS_ENTRY, ruleIndex);
			ret = tcapi_get(nodeName, "ActQueue", classQueue);
			if (ret < 0) {
				sprintf(rollbackEntry[i], QOS_ENTRY, ruleIndex);
				i++;
				tc_printf("\n==>The class queue of this rule %d is not set , do not save\n", ruleIndex);
				continue;
			}

			tcapi_commit(nodeName);
		}
	}
	
	for(j=0; j<i; j++){
		tc_printf("\n==>For not queue value set, just rollback %s\n", rollbackEntry[j]);
		tcapi_read(rollbackEntry[j]);
	}

	return 0;
}

int ctqos_common_commit(cfgNode_ptr pCfgNode)
{
	char nodeName[32];
	char active[10];
	int rule_index;

	/* commit common node anywhere */
	strcpy(nodeName, QOS_COMMON);
	tcapi_set(nodeName, "QoSOptType", "discRule");
	tcapi_commit(nodeName);

	memset(active, 0, sizeof(active));
	tcapi_get(nodeName, "Active", active);
	if (!strcmp(active, "Yes")) {
		for (rule_index = 0; rule_index < MAX_TYPE_ENTRY_NUM + MAX_APP_ENTRY_NUM; rule_index++) {
			sprintf(nodeName, "QoS_Entry%d", rule_index);
			
			if (rule_index < MAX_TYPE_ENTRY_NUM) {
				tcapi_set(QOS_COMMON, "QoSOptType", "typeRule");		
			}
			else {
				tcapi_set(QOS_COMMON, "QoSOptType", "appRule");
			}
			tcapi_commit(nodeName);
		}
	}

	return 0;
	
}


int chk_and_set_wanif(char *typeSetVal, int entry_index, int type_index, int flag)
{
	char wanif[128] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";
	char nodeName[51];
	char typeName[MAX_BUF_LEN], typeVal[MAX_BUF_LEN];
	int pvc_index, if_index;
	char pvc_arr[4], if_arr[4];
	char *p;
		
	if (typeSetVal == NULL) {
		return -1;
	}

	p = strstr(typeSetVal, wanif);
	if (NULL == p) {
		return -1;
	}

	memset(typeName, 0, sizeof(typeName));
	memset(typeVal, 0, sizeof(typeVal));
	
	/* get wan pvc index */
	p += strlen(wanif);
	memset(pvc_arr, 0, sizeof(pvc_arr));
	//strncpy(pvc_arr, p, 1);// you hua
	pvc_arr[0] = p[0];
	pvc_index = atoi(pvc_arr);

	/* maybe ip connection or ppp connection */
	sprintf(wanif, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANPPPConnection.", pvc_index);
	p = strstr(typeSetVal, wanif);
	if (NULL == p) {
		sprintf(wanif, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANIPConnection.", pvc_index);
		p = strstr(typeSetVal, wanif);
		if (NULL == p) {
			return -1;
		}
		goto get_if_index;
	}
	else {
		goto get_if_index;
	}

	/* get wan interface index */
get_if_index:
	p += strlen(wanif);
	memset(if_arr, 0, sizeof(if_arr));
	//strncpy(if_arr, p, 1);
	if_arr[0] = p[0];
	if_index = atoi(if_arr);

	sprintf(nodeName, QOS_ENTRY, entry_index);
	
	if (IF_BEGIN == flag) {
		sprintf(typeName, "Min%d", type_index + 1);
	}
	else if (IF_END == flag) {
		sprintf(typeName, "Max%d", type_index + 1);
	}
	else {
		return -1;
	}

	sprintf(typeVal, "%s,%s", pvc_arr, if_arr);
	tcapi_set(nodeName, typeName, typeVal);
	
	return 0;
}

int chk_and_set_lanif(char *typeSetVal, int entry_index, int type_index, int flag)
{
	char lanif[128] = "InternetGatewayDevice.LANDevice.1.";
	char nodeName[MAX_NODE_NAME_SIZE];
	char typeName[MAX_BUF_LEN];
	char lanif_arr[4];
	char *p;
	int lanif_index;
	int wlan_flag = 0;

	if (NULL == typeSetVal) {
		return -1;
	}
	
	p = strstr(typeSetVal, lanif);
	if (NULL == p) {
		return -1;
	}

	memset(typeName, 0, sizeof(typeName));
	
	/* may be ehternet interface or wireless interface */
	strcpy(lanif, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.");
	p = strstr(typeSetVal, lanif);
	if (NULL == p) {
		strcpy(lanif, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.");
		p = strstr(typeSetVal, lanif);
		if (NULL == p) {
			return -1;
		}

		goto get_if_index;
	}
	wlan_flag = 1;
	
get_if_index:
	p += strlen(lanif);
	memset(lanif_arr, 0, sizeof(lanif_arr));
	//strncpy(lanif_arr, p, 1);
	lanif_arr[0] = p[0];
	lanif_index = atoi(lanif_arr);

	if (1 == wlan_flag) {
		lanif_index += 4;
	}

	sprintf(nodeName, QOS_ENTRY, entry_index);
	
	if (IF_BEGIN == flag) {
		sprintf(typeName, "Min%d", type_index + 1);
	}
	else if (IF_END == flag) {
		sprintf(typeName, "Max%d", type_index + 1);
	}
	else {
		return -1;
	}

	sprintf(lanif_arr, "%d", lanif_index);
	tcapi_set(nodeName, typeName, lanif_arr);

	return 0;
}
		
void get_wan_if_name(char *typeGetVal, int entry_index, int type_index, int flag)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char typeName[MAX_BUF_LEN], typeVal[MAX_BUF_LEN];
	char isp[4], bridge_type[32];
	char pvc_arr[4], if_arr[4];
	char *p = NULL;
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	uint32 pvcindex, entryindex;
#endif

	memset(typeName, 0, sizeof(typeName));
	memset(typeVal, 0, sizeof(typeVal));
	memset(pvc_arr, 0, sizeof(pvc_arr));
	memset(if_arr, 0, sizeof(if_arr));

	sprintf(nodeName, QOS_ENTRY, entry_index);

	if (IF_BEGIN == flag) {
		sprintf(typeName, "Min%d", type_index + 1);
	}
	else if (IF_END == flag) {
		sprintf(typeName, "Max%d", type_index + 1);
	}
	else {
		return;
	}

	tcapi_get(nodeName, typeName, typeVal);
	//p = strtok(typeVal, ",");
	p = strstr(typeVal, ",");
	if (p == NULL) {
		return;
	}
	strncpy(pvc_arr, typeVal, p - typeVal);
	strcpy(if_arr, p + 1);
		
	/* get interface name for wan node */
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcindex = atoi(pvc_arr);
	entryindex = atoi(if_arr);
	FindWanNodeMapping(&pvcindex, &entryindex);
	sprintf(nodeName, "Wan_PVC%d_Entry%d", pvcindex, entryindex);
#else
	sprintf(nodeName, "Wan_PVC%d_Entry%d", atoi(pvc_arr) - 1, atoi(if_arr) - 1);
#endif
	tcapi_get(nodeName, "ISP", isp);

	if (!strcmp(isp, WAN_ENCAP_PPP)) {
		sprintf(typeGetVal, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%s.WANPPPConnection.%s", pvc_arr, if_arr);
	}
	else if (!strcmp(isp, WAN_ENCAP_DYN) || !strcmp(isp, WAN_ENCAP_STATIC)) {
		sprintf(typeGetVal, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%s.WANIPConnection.%s", pvc_arr, if_arr);
	}
	else {
		tcapi_get(nodeName, "BridgeMode", bridge_type);
		if (!strcmp(bridge_type, "IP_Bridged")) {
			sprintf(typeGetVal, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%s.WANIPConnection.%s", pvc_arr, if_arr);
		}
		else {
			/* default is pppoe bridge */
			sprintf(typeGetVal, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%s.WANPPPConnection.%s", pvc_arr, if_arr);
		}
	}

	return;
}

void get_lan_if_name(char *typeGetVal, int entry_index, int type_index, int flag)
{
	char nodeName[MAX_NODE_NAME_SIZE];
	char typeName[MAX_BUF_LEN], typeVal[MAX_BUF_LEN];
	char if_arr[4];

	memset(typeName, 0, sizeof(typeName));
	memset(typeVal, 0, sizeof(typeVal));
	memset(if_arr, 0, sizeof(if_arr));
	
	sprintf(nodeName, QOS_ENTRY, entry_index);

	if (IF_BEGIN == flag) {
		sprintf(typeName, "Min%d", type_index + 1);
	}
	else if (IF_END == flag) {
		sprintf(typeName, "Max%d", type_index + 1);
	}
	else {
		return;
	}
	tcapi_get(nodeName, typeName, if_arr);

	if (atoi(if_arr) > 4) {
		/* wireless interface */
		sprintf(typeGetVal, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.%d", atoi(if_arr) - 4);
	}
	else {
		/* ethernet interface */
		sprintf(typeGetVal, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.%s", if_arr);
	}

	return;
}

