#include "Global_res.h"
#include "cp_X_CTCOM_Mgt.h"

int8 getMgtURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char MgtURL[256];
	
	memset( MgtURL, 0, sizeof(MgtURL) );
	tcapi_get(MOBILE_ENTRY,"MgtURL",MgtURL);
	
	return getTheValue(value, type, deviceNode, MgtURL);
}
int8 setMgtURLValue(void* value, struct DeviceNode*  deviceNode)
{
	char* mgturl;
	char buffer[256] = {0};
	if(strlen((char*)value)>0)
	{
		mgturl= (char*)value;
		tcapi_get(MOBILE_ENTRY, DISTRIADDR, buffer);
		if(strcmp(buffer, mgturl)){
			tcapi_set(MOBILE_ENTRY, DISTRIADDR, mgturl);
			tcapi_set(MOBILE_ENTRY, OPERATADDR, "");  //clear operatr ip 
		}
	}
	else
		return -1;
	
	return 0;
}
int8 getMgtURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcX_CTCOM_MgtAttr.aMgtURL;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMgtURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcX_CTCOM_MgtAttr.aMgtURL;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= X_CTCOM_MGT_ATTR_FLAG;
	return result;
}

int8 getMgtPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char Port[10]={0};
		
	memset( Port, 0, sizeof(Port) );
	tcapi_get(MOBILE_ENTRY,"Port",Port);
	
	return getTheValue(value, type, deviceNode, Port);
}
int8 setMgtPortValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 i_Port = 0;
	char s_Port[10] = {0};

	i_Port = *((int*)value);
	if(i_Port > 65535)
		return -1;
	sprintf(s_Port, "%d", i_Port);
	tcapi_set(MOBILE_ENTRY, "Port", s_Port);

	return 0;
}

int8 getMgtPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcX_CTCOM_MgtAttr.aPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMgtPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcX_CTCOM_MgtAttr.aPort;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= X_CTCOM_MGT_ATTR_FLAG;
	return result;
}

int8 getMgtHeartbeatValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char MgtHeartbeat[10]={0};
		
	memset( MgtHeartbeat, 0, sizeof(MgtHeartbeat) );
	tcapi_get(MOBILE_ENTRY,"Heartbeat",MgtHeartbeat);
	
	return getTheValue(value, type, deviceNode, MgtHeartbeat);
}
int8 setMgtHeartbeatValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 i_Heartbeat = 0;
	char s_Heartbeat[10] = {0};

	i_Heartbeat = *((int*)value);
	sprintf(s_Heartbeat, "%d", i_Heartbeat);
	tcapi_set(MOBILE_ENTRY, "Heartbeat", s_Heartbeat);

	return 0;
}
int8 getMgtHeartbeatAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcX_CTCOM_MgtAttr.aHeartbeat;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMgtHeartbeatAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcX_CTCOM_MgtAttr.aHeartbeat;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= X_CTCOM_MGT_ATTR_FLAG;
	
	return result;
}

int8 getMgtAbilityValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char MgtAbility[10]={0};
		
	memset( MgtAbility, 0, sizeof(MgtAbility) );
	tcapi_get(MOBILE_ENTRY,"Ability",MgtAbility);
	
	return getTheValue(value, type, deviceNode, MgtAbility);
}
int8 setMgtAbilityValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 i_Ability = 0;
	char s_Ability[10] = {0};

	i_Ability = *((int*)value);
	if(i_Ability>1)
		return -1;
	sprintf(s_Ability, "%d", i_Ability);
	tcapi_set(MOBILE_ENTRY, "Ability", s_Ability);

	return 0;
}
int8 getMgtAbilityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcX_CTCOM_MgtAttr.aAbility;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMgtAbilityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcX_CTCOM_MgtAttr.aAbility;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= X_CTCOM_MGT_ATTR_FLAG;
	
	return result;
}

int8 getMgtLocatePortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char MgtLocatePort[10]={0};
		
	memset( MgtLocatePort, 0, sizeof(MgtLocatePort) );
	tcapi_get(MOBILE_ENTRY,"LocatePort",MgtLocatePort);
	
	return getTheValue(value, type, deviceNode, MgtLocatePort);
}
int8 setMgtLocatePortValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 i_LocatePort = 0;
	char s_LocatePort[10] = {0};

	i_LocatePort = *((int*)value);
	if(i_LocatePort>65535)
		return -1;
	sprintf(s_LocatePort, "%d", i_LocatePort);
	tcapi_set(MOBILE_ENTRY, "LocatePort", s_LocatePort);

	return 0;
}
int8 getMgtLocatePortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcX_CTCOM_MgtAttr.aLocatePort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMgtLocatePortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcX_CTCOM_MgtAttr.aLocatePort;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= X_CTCOM_MGT_ATTR_FLAG;
	
	return result;
}

int8 getMgtVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char MgtVersion[256];
	
	memset( MgtVersion, 0, sizeof(MgtVersion) );
	tcapi_get(MOBILE_ENTRY,"Version",MgtVersion);
	return getTheValue(value, type, deviceNode, MgtVersion);
}
int8 getMgtVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcX_CTCOM_MgtAttr.aVersion;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMgtVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcX_CTCOM_MgtAttr.aVersion;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= X_CTCOM_MGT_ATTR_FLAG;
	
	return result;
}

#if defined(TCSUPPORT_CT_JOYME)
int8 getMgtSSNValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char MgtSSN[256];
	
	bzero(MgtSSN, sizeof(MgtSSN));
	if ( 0 != tcapi_get(MOBILE_ENTRY, "SSN", MgtSSN) )
		MgtSSN[0] = 0;

	return getTheValue(value, type, deviceNode, MgtSSN);
}
int8 getMgtSSNAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcX_CTCOM_MgtAttr.aSSN;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMgtSSNAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;

	attribute = &tcX_CTCOM_MgtAttr.aSSN;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= X_CTCOM_MGT_ATTR_FLAG;

	return result;
}
#endif
