#include "Global_res.h"
#include "cp_lanconfigsecurity.h"

int8 getConfigPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
        char  configPsw[128] = {0};
	
	configPsw[0] = 0;
	return getTheValue(value, type, deviceNode, configPsw);

}

int8 setConfigPasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	if(strlen((char*)value) >= WEB_PSWD_LEN)
	{
		tc_printf("\r\nthe length is over the max value!");
		return -1;
	}

	tcapi_set("Account_Entry0","web_passwd", (char*)value);
	SetCfgNodeFlag( "Account", -1);
	
	return 0;
}

int8 getConfigPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclanconfigsecurityAttr.aConfigPassword;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setConfigPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tclanconfigsecurityAttr.aConfigPassword;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}


