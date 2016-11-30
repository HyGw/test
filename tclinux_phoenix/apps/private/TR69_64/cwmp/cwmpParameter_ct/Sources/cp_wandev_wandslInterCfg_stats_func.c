#include "Global_res.h"
#include "cp_wandev_wandslInterCfg_stats.h"

int8 getReceiveBlock_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOSHOW_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up")){
		if(0 != tcapi_get(INFOSHOW_NODE, "ReceiveBlock", result))
			memset(result, 0, sizeof(result));
	}

	return getTheValue(value, type, deviceNode, result);
}

int8 getReceiveBlock_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcshowtimeAttr.aReceiveBlock;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setReceiveBlock_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcshowtimeAttr.aReceiveBlock;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= SHOWTIME_ATTR_FLAG;
	return result;
}

int8 getTransmitBlock_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOSHOW_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up")){
		if(0 != tcapi_get(INFOSHOW_NODE, "TransmitBlock", result))
			memset(result, 0, sizeof(result));
	}
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getTransmitBlock_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcshowtimeAttr.aTransmitBlock;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTransmitBlock_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcshowtimeAttr.aTransmitBlock;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= SHOWTIME_ATTR_FLAG;
	return result;
}

int8 getCellDelin_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOSHOW_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up")){
		if(0 != tcapi_get(INFOSHOW_NODE, "CellDelin", result))
			memset(result, 0, sizeof(result));
	}
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getCellDelin_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcshowtimeAttr.aCellDelin;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCellDelin_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcshowtimeAttr.aCellDelin;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= SHOWTIME_ATTR_FLAG;
	return result;
}

int8 getLinkRetrain_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOSHOW_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up")){
		if(0 != tcapi_get(INFOSHOW_NODE, "LinkRetrain", result))
			memset(result, 0, sizeof(result));
	}
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getLinkRetrain_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcshowtimeAttr.aLinkRetrain;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLinkRetrain_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcshowtimeAttr.aLinkRetrain;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= SHOWTIME_ATTR_FLAG;
	return result;
}

int8 getInitErrors_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{

	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOSHOW_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up")){
		if(0 != tcapi_get(INFOSHOW_NODE, "InitErrors", result))
			memset(result, 0, sizeof(result));
	}
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getInitErrors_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcshowtimeAttr.aInitErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setInitErrors_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcshowtimeAttr.aInitErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= SHOWTIME_ATTR_FLAG;
	return result;
}

int8 getInitTimeouts_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOSHOW_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up")){
		if(0 != tcapi_get(INFOSHOW_NODE, "InitTimeouts", result))
			memset(result, 0, sizeof(result));
	}
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getInitTimeouts_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcshowtimeAttr.aInitTimeouts;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setInitTimeouts_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcshowtimeAttr.aInitTimeouts;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= SHOWTIME_ATTR_FLAG;
	return result;
}

int8 getLossOfFraming_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOSHOW_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up")){
		if(0 != tcapi_get(INFOSHOW_NODE, "LossOfFraming", result))
			memset(result, 0, sizeof(result));
	}
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getLossOfFraming_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcshowtimeAttr.aLossOfFraming;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLossOfFraming_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcshowtimeAttr.aLossOfFraming;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= SHOWTIME_ATTR_FLAG;
	return result;
}

int8 getErroredSecs_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOSHOW_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up")){
		if(0 != tcapi_get(INFOSHOW_NODE, "ErroredSecs", result))
			memset(result, 0, sizeof(result));
	}
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getErroredSecs_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcshowtimeAttr.aErroredSecs;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setErroredSecs_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcshowtimeAttr.aErroredSecs;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= SHOWTIME_ATTR_FLAG;
	return result;
}

int8 getSeveErrSecs_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{

	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOSHOW_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up")){
		if(0 != tcapi_get(INFOSHOW_NODE, "SeveErrSecs", result))
			memset(result, 0, sizeof(result));
	}
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getSeveErrSecs_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcshowtimeAttr.aSeverelyErroredSecs;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setSeveErrSecs_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcshowtimeAttr.aSeverelyErroredSecs;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= SHOWTIME_ATTR_FLAG;
	return result;
}

int8 getFecErrors_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOSHOW_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up")){
		if(0 != tcapi_get(INFOSHOW_NODE, "FECDown", result))
			memset(result, 0, sizeof(result));
	}

	return getTheValue(value, type, deviceNode, result);
}

int8 getFecErrors_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcshowtimeAttr.aFecErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setFecErrors_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcshowtimeAttr.aFecErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= SHOWTIME_ATTR_FLAG;
	return result;
}

int8 getATUCFECErrors_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOSHOW_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up")){
		if(0 != tcapi_get(INFOSHOW_NODE, "FECUp", result))
			memset(result, 0, sizeof(result));
	}
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getATUCFECErrors_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcshowtimeAttr.aATUCFECErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATUCFECErrors_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcshowtimeAttr.aATUCFECErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= SHOWTIME_ATTR_FLAG;
	return result;
}

int8 getHECErrors_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOSHOW_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up")){
		if(0 != tcapi_get(INFOSHOW_NODE, "HECDown", result))
			memset(result, 0, sizeof(result));
	}
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getHECErrors_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcshowtimeAttr.aHECErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setHECErrors_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcshowtimeAttr.aHECErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= SHOWTIME_ATTR_FLAG;
	return result;
}

int8 getCRCErrors_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOSHOW_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up")){
		if(0 != tcapi_get(INFOSHOW_NODE, "CRCDown", result))
			memset(result, 0, sizeof(result));
	}
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getCRCErrors_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcshowtimeAttr.aCRCErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCRCErrors_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcshowtimeAttr.aCRCErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= SHOWTIME_ATTR_FLAG;
	return result;
}

int8 getATUCCRCErrors_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{

	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOSHOW_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up")){
		if(0 != tcapi_get(INFOSHOW_NODE, "CRCUp", result))
			memset(result, 0, sizeof(result));
	}
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getATUCCRCErrors_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcshowtimeAttr.aATUCCRCErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATUCCRCErrors_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcshowtimeAttr.aATUCCRCErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= SHOWTIME_ATTR_FLAG;
	return result;
}

int8 getReceiveBlock_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 showtime= 0, total = 0;

	if(0 != tcapi_get(INFOTOTAL_NODE, "TotalReceiveBlock", tmp))
		memset(result, 0, sizeof(tmp));
	else
		total = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
	{
		tcapi_get(INFOSHOW_NODE, "ReceiveBlock", tmp);
		showtime = atolu(tmp);
		total += showtime;
		sprintf(result, "%lu", total);
	}
	else
		sprintf(result, "%lu", total);
	return getTheValue(value, type, deviceNode, result);
}

int8 getReceiveBlock_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tctotalAttr.aReceiveBlock;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setReceiveBlock_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tctotalAttr.aReceiveBlock;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= TOTAL_ATTR_FLAG;
	return result;
}

int8 getTransmitBlock_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 showtime= 0, total = 0;

	if(0 != tcapi_get(INFOTOTAL_NODE, "TotalTransmitBlock", tmp))
		memset(result, 0, sizeof(tmp));
	else
		total = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
	{	
		tcapi_get(INFOSHOW_NODE, "TransmitBlock", tmp);
		showtime = atolu(tmp);
		total += showtime;
		sprintf(result, "%lu", total);
	}
	else
		sprintf(result, "%lu", total);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getTransmitBlock_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tctotalAttr.aTransmitBlock;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTransmitBlock_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tctotalAttr.aTransmitBlock;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= TOTAL_ATTR_FLAG;
	return result;
}

int8 getCellDelin_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 showtime= 0, total = 0;

	if(0 != tcapi_get(INFOTOTAL_NODE, "TotalCellDelin", tmp))
		memset(result, 0, sizeof(tmp));
	else
		total = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
	{		
		tcapi_get(INFOSHOW_NODE, "CellDelin", tmp);
		showtime = atolu(tmp);
		total += showtime;
		sprintf(result, "%lu", total);
	}
	else
		sprintf(result, "%lu", total);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getCellDelin_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tctotalAttr.aCellDelin;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCellDelin_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tctotalAttr.aCellDelin;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= TOTAL_ATTR_FLAG;
	return result;
}

int8 getLinkRetrain_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 showtime= 0, total = 0;

	if(0 != tcapi_get(INFOTOTAL_NODE, "TotalLinkRetrain", tmp))
		memset(result, 0, sizeof(tmp));
	else
		total = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
	{
		tcapi_get(INFOSHOW_NODE, "LinkRetrain", tmp);
		showtime = atolu(tmp);
		total += showtime;
		sprintf(result, "%lu", total);
	}
	else
		sprintf(result, "%lu", total);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getLinkRetrain_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tctotalAttr.aLinkRetrain;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLinkRetrain_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tctotalAttr.aLinkRetrain;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= TOTAL_ATTR_FLAG;
	return result;
}

int8 getInitErrors_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 showtime= 0, total = 0;

	if(0 != tcapi_get(INFOTOTAL_NODE, "TotalInitErrors", tmp))
		memset(result, 0, sizeof(tmp));
	else
		total = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
	{
		tcapi_get(INFOSHOW_NODE, "InitErrors", tmp);
		showtime = atolu(tmp);
		total += showtime;
		sprintf(result, "%lu", total);
	}
	else
		sprintf(result, "%lu", total);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getInitErrors_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tctotalAttr.aInitErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setInitErrors_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tctotalAttr.aInitErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= TOTAL_ATTR_FLAG;
	return result;
}

int8 getInitTimeouts_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 showtime= 0, total = 0;

	if(0 != tcapi_get(INFOTOTAL_NODE, "TotalInitTimeouts", tmp))
		memset(result, 0, sizeof(tmp));
	else
		total = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
	{
		tcapi_get(INFOSHOW_NODE, "InitTimeouts", tmp);
		showtime = atolu(tmp);
		total += showtime;
		sprintf(result, "%lu", total);
	}
	else
		sprintf(result, "%lu", total);	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getInitTimeouts_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tctotalAttr.aInitTimeouts;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setInitTimeouts_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tctotalAttr.aInitTimeouts;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= TOTAL_ATTR_FLAG;
	return result;
}

int8 getLossOfFraming_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 showtime= 0, total = 0;

	if(0 != tcapi_get(INFOTOTAL_NODE, "TotalLossOfFraming", tmp))
		memset(result, 0, sizeof(tmp));
	else
		total = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
	{
		tcapi_get(INFOSHOW_NODE, "LossOfFraming", tmp);
		showtime = atolu(tmp);
		total += showtime;
		sprintf(result, "%lu", total);
	}
	else
		sprintf(result, "%lu", total);	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getLossOfFraming_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tctotalAttr.aLossOfFraming;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLossOfFraming_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tctotalAttr.aLossOfFraming;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= TOTAL_ATTR_FLAG;
	return result;
}

int8 getErroredSecs_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 showtime= 0, total = 0;

	if(0 != tcapi_get(INFOTOTAL_NODE, "TotalErroredSecs", tmp))
		memset(result, 0, sizeof(tmp));
	else
		total = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
	{
		tcapi_get(INFOSHOW_NODE, "ErroredSecs", tmp);
		showtime = atolu(tmp);
		total += showtime;
		sprintf(result, "%lu", total);
	}
	else
		sprintf(result, "%lu", total);	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getErroredSecs_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tctotalAttr.aErroredSecs;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setErroredSecs_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tctotalAttr.aErroredSecs;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= TOTAL_ATTR_FLAG;
	return result;
}

int8 getSeveErrSecs_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 showtime= 0, total = 0;

	if(0 != tcapi_get(INFOTOTAL_NODE, "TotalErroredSecs", tmp))
		memset(result, 0, sizeof(tmp));
	else
		total = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
	{
		tcapi_get(INFOSHOW_NODE, "ErroredSecs", tmp);
		showtime = atolu(tmp);
		total += showtime;
		sprintf(result, "%lu", total);
	}
	else
		sprintf(result, "%lu", total);	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getSeveErrSecs_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tctotalAttr.aSeverelyErroredSecs;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setSeveErrSecs_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tctotalAttr.aSeverelyErroredSecs;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= TOTAL_ATTR_FLAG;
	return result;
}

int8 getFecErrors_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	int showtime= 0, total = 0;

	if(0 != tcapi_get(INFOTOTAL_NODE, "TotalFECDown", tmp))
		memset(result, 0, sizeof(tmp));
	else
		total = atoi(tmp);

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
	{		
		tcapi_get(INFOSHOW_NODE, "FECDown", tmp);
		showtime = atoi(tmp);
		total += showtime;
		sprintf(result, "%d", total);
	}
	else
		sprintf(result, "%d", total);
		
	return getTheValue(value, type, deviceNode, result);
}

int8 getFecErrors_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tctotalAttr.aFecErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setFecErrors_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tctotalAttr.aFecErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= TOTAL_ATTR_FLAG;
	return result;
}

int8 getATUCFECErrors_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	int showtime= 0, total = 0;

	if(0 != tcapi_get(INFOTOTAL_NODE, "TotalFECUp", tmp))
		memset(result, 0, sizeof(tmp));
	else
		total = atoi(tmp);
		
	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
	{
		tcapi_get(INFOSHOW_NODE, "FECUp", tmp);
		showtime = atoi(tmp);
		total += showtime;
		sprintf(result, "%d", total);
	}
	else
		sprintf(result, "%d", total);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getATUCFECErrors_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tctotalAttr.aATUCFECErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATUCFECErrors_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tctotalAttr.aATUCFECErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= TOTAL_ATTR_FLAG;
	return result;
}

int8 getHECErrors_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	int showtime= 0, total = 0;

	if(0 != tcapi_get(INFOTOTAL_NODE, "TotalHECDown", tmp))
		memset(result, 0, sizeof(tmp));
	else
		total = atoi(tmp);

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
	{
		tcapi_get(INFOSHOW_NODE, "HECDown", tmp);
		showtime = atoi(tmp);
		total += showtime;
		sprintf(result, "%d", total);
	}
	else
		sprintf(result, "%d", total);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getHECErrors_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tctotalAttr.aHECErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setHECErrors_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tctotalAttr.aHECErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= TOTAL_ATTR_FLAG;
	return result;
}

int8 getCRCErrors_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	int showtime= 0, total = 0;
	
	if(0 != tcapi_get(INFOTOTAL_NODE, "TotalCRCDown", tmp))
		memset(result, 0, sizeof(tmp));
	else
		total = atoi(tmp);
	
	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
	{
		tcapi_get(INFOSHOW_NODE, "CRCDown", tmp);
		showtime = atoi(tmp);
		total += showtime;
		sprintf(result, "%d", total);
	}
	else
		sprintf(result, "%d", total);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getCRCErrors_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tctotalAttr.aCRCErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCRCErrors_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tctotalAttr.aCRCErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= TOTAL_ATTR_FLAG;
	return result;
}

int8 getATUCCRCErrors_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	int showtime= 0, total = 0;

	if(0 != tcapi_get(INFOTOTAL_NODE, "TotalCRCUp", tmp))
		memset(result, 0, sizeof(tmp));
	else
		total = atoi(tmp);
			
	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
	{
		tcapi_get(INFOSHOW_NODE, "CRCUp", tmp);
		showtime = atoi(tmp);
		total += showtime;
		sprintf(result, "%d", total);
	}
	else
		sprintf(result, "%d", total);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getATUCCRCErrors_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tctotalAttr.aATUCCRCErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATUCCRCErrors_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tctotalAttr.aATUCCRCErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= TOTAL_ATTR_FLAG;
	return result;
}

int8 getReceiveBlock_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOLAST_NODE, "LastShowtimeFlag", tmp);
	if(0 == strcmp(tmp, "0"))
	{
		tcapi_get(INFOLAST_NODE, "lineState", tmp);
		if(0 == strcmp(tmp, "up"))
			tcapi_get(INFOSHOW_NODE, "ReceiveBlock", result);
	}
	else
		tcapi_get(INFOLAST_NODE, "LastReceiveBlock", result);
	return getTheValue(value, type, deviceNode, result);
}

int8 getReceiveBlock_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclastShowtimeAttr.aReceiveBlock;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setReceiveBlock_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tclastShowtimeAttr.aReceiveBlock;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= LASTSHOWTIME_ATTR_FLAG;
	return result;
}

int8 getTransmitBlock_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOLAST_NODE, "LastShowtimeFlag", tmp);
	if(0 == strcmp(tmp, "0"))
	{
		tcapi_get(INFOLAST_NODE, "lineState", tmp);
		if(0 == strcmp(tmp, "up"))
			tcapi_get(INFOSHOW_NODE, "TransmitBlock", result);
	}
	else
		tcapi_get(INFOLAST_NODE, "LastTransmitBlock", result);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getTransmitBlock_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclastShowtimeAttr.aTransmitBlock;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTransmitBlock_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tclastShowtimeAttr.aTransmitBlock;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= LASTSHOWTIME_ATTR_FLAG;
	return result;
}

int8 getCellDelin_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOLAST_NODE, "LastShowtimeFlag", tmp);
	if(0 == strcmp(tmp, "0"))
	{
		tcapi_get(INFOLAST_NODE, "lineState", tmp);
		if(0 == strcmp(tmp, "up"))
			tcapi_get(INFOSHOW_NODE, "CellDelin", result);
	}
	else
		tcapi_get(INFOLAST_NODE, "LastCellDelin", result);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getCellDelin_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclastShowtimeAttr.aCellDelin;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCellDelin_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tclastShowtimeAttr.aCellDelin;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= LASTSHOWTIME_ATTR_FLAG;
	return result;
}

int8 getLinkRetrain_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOLAST_NODE, "LastShowtimeFlag", tmp);
	if(0 == strcmp(tmp, "0"))
	{
		tcapi_get(INFOLAST_NODE, "lineState", tmp);
		if(0 == strcmp(tmp, "up"))
			tcapi_get(INFOSHOW_NODE, "LinkRetrain", result);
	}
	else
		tcapi_get(INFOLAST_NODE, "LastLinkRetrain", result);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getLinkRetrain_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclastShowtimeAttr.aLinkRetrain;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLinkRetrain_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tclastShowtimeAttr.aLinkRetrain;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= LASTSHOWTIME_ATTR_FLAG;
	return result;
}

int8 getInitErrors_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOLAST_NODE, "LastShowtimeFlag", tmp);
	if(0 == strcmp(tmp, "0"))
	{
		tcapi_get(INFOLAST_NODE, "lineState", tmp);
		if(0 == strcmp(tmp, "up"))
			tcapi_get(INFOSHOW_NODE, "InitErrors", result);
	}
	else
		tcapi_get(INFOLAST_NODE, "LastInitErrors", result);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getInitErrors_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclastShowtimeAttr.aInitErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setInitErrors_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tclastShowtimeAttr.aInitErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= LASTSHOWTIME_ATTR_FLAG;
	return result;
}

int8 getInitTimeouts_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOLAST_NODE, "LastShowtimeFlag", tmp);
	if(0 == strcmp(tmp, "0"))
	{
		tcapi_get(INFOLAST_NODE, "lineState", tmp);
		if(0 == strcmp(tmp, "up"))
			tcapi_get(INFOSHOW_NODE, "InitTimeouts", result);
	}
	else
		tcapi_get(INFOLAST_NODE, "LastInitTimeouts", result);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getInitTimeouts_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclastShowtimeAttr.aInitTimeouts;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setInitTimeouts_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tclastShowtimeAttr.aInitTimeouts;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= LASTSHOWTIME_ATTR_FLAG;
	return result;
}

int8 getLossOfFraming_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOLAST_NODE, "LastShowtimeFlag", tmp);
	if(0 == strcmp(tmp, "0"))
	{
		tcapi_get(INFOLAST_NODE, "lineState", tmp);
		if(0 == strcmp(tmp, "up"))
			tcapi_get(INFOSHOW_NODE, "LossOfFraming", result);
	}
	else
		tcapi_get(INFOLAST_NODE, "LastLossOfFraming", result);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getLossOfFraming_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclastShowtimeAttr.aLossOfFraming;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLossOfFraming_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tclastShowtimeAttr.aLossOfFraming;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= LASTSHOWTIME_ATTR_FLAG;
	return result;
}

int8 getErroredSecs_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOLAST_NODE, "LastShowtimeFlag", tmp);
	if(0 == strcmp(tmp, "0"))
	{
		tcapi_get(INFOLAST_NODE, "lineState", tmp);
		if(0 == strcmp(tmp, "up"))
			tcapi_get(INFOSHOW_NODE, "ErroredSecs", result);
	}
	else
		tcapi_get(INFOLAST_NODE, "LastErroredSecs", result);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getErroredSecs_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclastShowtimeAttr.aErroredSecs;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setErroredSecs_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tclastShowtimeAttr.aErroredSecs;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= LASTSHOWTIME_ATTR_FLAG;
	return result;
}

int8 getSeveErrSecs_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOLAST_NODE, "LastShowtimeFlag", tmp);
	if(0 == strcmp(tmp, "0"))
	{
		tcapi_get(INFOLAST_NODE, "lineState", tmp);
		if(0 == strcmp(tmp, "up"))
			tcapi_get(INFOSHOW_NODE, "SeveErrSecs", result);
	}
	else
		tcapi_get(INFOLAST_NODE, "LastSeveErrSecs", result);
	return getTheValue(value, type, deviceNode, result);	
}

int8 getSeveErrSecs_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclastShowtimeAttr.aSeverelyErroredSecs;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setSeveErrSecs_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tclastShowtimeAttr.aSeverelyErroredSecs;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= LASTSHOWTIME_ATTR_FLAG;
	return result;
}

int8 getFecErrors_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOLAST_NODE, "LastShowtimeFlag", tmp);
	if(0 == strcmp(tmp, "0"))
	{
		tcapi_get(INFOLAST_NODE, "lineState", tmp);
		if(0 == strcmp(tmp, "up"))
			tcapi_get(INFOSHOW_NODE, "FECDown", result);
	}
	else
		tcapi_get(INFOLAST_NODE, "LastFECDown", result);

	return getTheValue(value, type, deviceNode, result);
}

int8 getFecErrors_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclastShowtimeAttr.aFecErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setFecErrors_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tclastShowtimeAttr.aFecErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= LASTSHOWTIME_ATTR_FLAG;
	return result;
}

int8 getATUCFECErrors_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOLAST_NODE, "LastShowtimeFlag", tmp);
	if(0 == strcmp(tmp, "0"))
	{
		tcapi_get(INFOLAST_NODE, "lineState", tmp);
		if(0 == strcmp(tmp, "up"))
			tcapi_get(INFOSHOW_NODE, "FECUp", result);
	}
	else
		tcapi_get(INFOLAST_NODE, "LastFECUp", result);
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getATUCFECErrors_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclastShowtimeAttr.aATUCFECErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATUCFECErrors_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tclastShowtimeAttr.aATUCFECErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= LASTSHOWTIME_ATTR_FLAG;
	return result;
}

int8 getHECErrors_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOLAST_NODE, "LastShowtimeFlag", tmp);
	if(0 == strcmp(tmp, "0"))
	{
		tcapi_get(INFOLAST_NODE, "lineState", tmp);
		if(0 == strcmp(tmp, "up"))
			tcapi_get(INFOSHOW_NODE, "HECDown", result);
	}
	else
		tcapi_get(INFOLAST_NODE, "LastHECDown", result);
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getHECErrors_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclastShowtimeAttr.aHECErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setHECErrors_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tclastShowtimeAttr.aHECErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= LASTSHOWTIME_ATTR_FLAG;
	return result;
}

int8 getCRCErrors_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOLAST_NODE, "LastShowtimeFlag", tmp);
	if(0 == strcmp(tmp, "0"))
	{
		tcapi_get(INFOLAST_NODE, "lineState", tmp);
		if(0 == strcmp(tmp, "up"))
			tcapi_get(INFOSHOW_NODE, "CRCDown", result);
	}
	else
		tcapi_get(INFOLAST_NODE, "LastCRCDown", result);

	return getTheValue(value, type, deviceNode, result);	
}

int8 getCRCErrors_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclastShowtimeAttr.aCRCErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCRCErrors_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tclastShowtimeAttr.aCRCErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= LASTSHOWTIME_ATTR_FLAG;
	return result;
}

int8 getATUCCRCErrors_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};

	tcapi_get(INFOLAST_NODE, "LastShowtimeFlag", tmp);
	if(0 == strcmp(tmp, "0"))
	{
		tcapi_get(INFOLAST_NODE, "lineState", tmp);
		if(0 == strcmp(tmp, "up"))
			tcapi_get(INFOSHOW_NODE, "CRCUp", result);
	}
	else
		tcapi_get(INFOLAST_NODE, "LastCRCUp", result);
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getATUCCRCErrors_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclastShowtimeAttr.aATUCCRCErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATUCCRCErrors_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tclastShowtimeAttr.aATUCCRCErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= LASTSHOWTIME_ATTR_FLAG;
	return result;
}

int8 getReceiveBlock_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 total= 0, curdaySt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "ReceiveBlock", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastReceiveBlock", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalReceiveBlock", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOCURDAYST_NODE, "CurdayStReceiveBlock", tmp);
	curdaySt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-curdaySt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getReceiveBlock_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tccurrentDayAttr.aReceiveBlock;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setReceiveBlock_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tccurrentDayAttr.aReceiveBlock;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= CURRENTDAY_ATTR_FLAG;
	return result;
}

int8 getTransmitBlock_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 total= 0, curdaySt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "TransmitBlock", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastTransmitBlock", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalTransmitBlock", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOCURDAYST_NODE, "CurdayStTransmitBlock", tmp);
	curdaySt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-curdaySt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getTransmitBlock_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tccurrentDayAttr.aTransmitBlock;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTransmitBlock_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tccurrentDayAttr.aTransmitBlock;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= CURRENTDAY_ATTR_FLAG;
	return result;
}

int8 getCellDelin_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 total= 0, curdaySt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "CellDelin", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastCellDelin", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalCellDelin", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOCURDAYST_NODE, "CurdayStCellDelin", tmp);
	curdaySt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-curdaySt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getCellDelin_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tccurrentDayAttr.aCellDelin;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCellDelin_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tccurrentDayAttr.aCellDelin;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= CURRENTDAY_ATTR_FLAG;
	return result;
}

int8 getLinkRetrain_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 total= 0, curdaySt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "LinkRetrain", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastLinkRetrain", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalLinkRetrain", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOCURDAYST_NODE, "CurdayStLinkRetrain", tmp);
	curdaySt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-curdaySt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getLinkRetrain_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tccurrentDayAttr.aLinkRetrain;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLinkRetrain_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tccurrentDayAttr.aLinkRetrain;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= CURRENTDAY_ATTR_FLAG;
	return result;
}

int8 getInitErrors_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 total= 0, curdaySt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "InitErrors", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastInitErrors", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalInitErrors", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOCURDAYST_NODE, "CurdayStInitErrors", tmp);
	curdaySt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-curdaySt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getInitErrors_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tccurrentDayAttr.aInitErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setInitErrors_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tccurrentDayAttr.aInitErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= CURRENTDAY_ATTR_FLAG;
	return result;
}

int8 getInitTimeouts_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 total= 0, curdaySt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "InitTimeouts", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastInitTimeouts", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalInitTimeouts", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOCURDAYST_NODE, "CurdayStInitTimeouts", tmp);
	curdaySt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-curdaySt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getInitTimeouts_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tccurrentDayAttr.aInitTimeouts;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setInitTimeouts_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tccurrentDayAttr.aInitTimeouts;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= CURRENTDAY_ATTR_FLAG;
	return result;
}

int8 getLossOfFraming_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 total= 0, curdaySt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "LossOfFraming", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastLossOfFraming", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalLossOfFraming", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOCURDAYST_NODE, "CurdayStLossOfFraming", tmp);
	curdaySt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-curdaySt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getLossOfFraming_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tccurrentDayAttr.aLossOfFraming;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLossOfFraming_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tccurrentDayAttr.aLossOfFraming;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= CURRENTDAY_ATTR_FLAG;
	return result;
}

int8 getErroredSecs_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 total= 0, curdaySt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "ErroredSecs", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastErroredSecs", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalErroredSecs", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOCURDAYST_NODE, "CurdayStErroredSecs", tmp);
	curdaySt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-curdaySt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getErroredSecs_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tccurrentDayAttr.aErroredSecs;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setErroredSecs_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tccurrentDayAttr.aErroredSecs;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= CURRENTDAY_ATTR_FLAG;
	return result;
}

int8 getSeveErrSecs_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[32] = {0};
	char tmp[32] = {0};
	uint32 total= 0, curdaySt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "SeveErrSecs", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastSeveErrSecs", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalSeveErrSecs", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOCURDAYST_NODE, "CurdayStSeveErrSecs", tmp);
	curdaySt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-curdaySt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getSeveErrSecs_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tccurrentDayAttr.aSeverelyErroredSecs;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setSeveErrSecs_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tccurrentDayAttr.aSeverelyErroredSecs;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= CURRENTDAY_ATTR_FLAG;
	return result;
}

int8 getFecErrors_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	int total= 0, curdaySt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "FecErrors", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastFecErrors", tmp);
	tmpValue = atoi(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalFecErrors", tmp);	
	total = atoi(tmp);
	
	tcapi_get(INFOCURDAYST_NODE, "CurdayStFecErrors", tmp);
	curdaySt = atoi(tmp);
	
	sprintf(result, "%u", total+tmpValue-curdaySt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getFecErrors_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tccurrentDayAttr.aFecErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setFecErrors_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tccurrentDayAttr.aFecErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= CURRENTDAY_ATTR_FLAG;
	return result;
}

int8 getATUCFECErrors_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	int total= 0, curdaySt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "ATUCFEC", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastATUCFEC", tmp);
	tmpValue = atoi(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalATUCFEC", tmp);
	total = atoi(tmp);
	
	tcapi_get(INFOCURDAYST_NODE, "CurdayStATUCFEC", tmp);
	curdaySt = atoi(tmp);
	
	sprintf(result, "%u", total+tmpValue-curdaySt);
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getATUCFECErrors_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tccurrentDayAttr.aATUCFECErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATUCFECErrors_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tccurrentDayAttr.aATUCFECErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= CURRENTDAY_ATTR_FLAG;
	return result;
}

int8 getHECErrors_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	int total= 0, curdaySt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "HECErrors", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastHECErrors", tmp);
	tmpValue = atoi(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalHECErrors", tmp);
	total = atoi(tmp);
	
	tcapi_get(INFOCURDAYST_NODE, "CurdayStHECErrors", tmp);
	curdaySt = atoi(tmp);
	
	sprintf(result, "%u", total+tmpValue-curdaySt);
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getHECErrors_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tccurrentDayAttr.aHECErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setHECErrors_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tccurrentDayAttr.aHECErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= CURRENTDAY_ATTR_FLAG;
	return result;
}

int8 getCRCErrors_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	int total= 0, curdaySt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "CRCErrors", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastCRCErrors", tmp);
	tmpValue = atoi(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalCRCErrors", tmp);
	total = atoi(tmp);
	
	tcapi_get(INFOCURDAYST_NODE, "CurdayStCRCErrors", tmp);
	curdaySt = atoi(tmp);
	
	sprintf(result, "%u", total+tmpValue-curdaySt);
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getCRCErrors_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tccurrentDayAttr.aCRCErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCRCErrors_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tccurrentDayAttr.aCRCErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= CURRENTDAY_ATTR_FLAG;
	return result;
}

int8 getATUCCRCErrors_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	int total= 0, curdaySt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "ATUCCRCErrors", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastATUCCRCErrors", tmp);
	tmpValue = atoi(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalATUCCRCErrors", tmp);
	total = atoi(tmp);
	
	tcapi_get(INFOCURDAYST_NODE, "CurdayStATUCCRCErrors", tmp);
	curdaySt = atoi(tmp);
	
	sprintf(result, "%u", total+tmpValue-curdaySt);
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getATUCCRCErrors_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tccurrentDayAttr.aATUCCRCErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATUCCRCErrors_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tccurrentDayAttr.aATUCCRCErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= CURRENTDAY_ATTR_FLAG;
	return result;
}

int8 getReceiveBlock_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 total= 0, qutHourSt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "ReceiveBlock", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastReceiveBlock", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalReceiveBlock", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOQUTHOURST_NODE, "QutHrStReceiveBlock", tmp);
	qutHourSt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-qutHourSt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getReceiveBlock_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcquarterHourAttr.aReceiveBlock;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setReceiveBlock_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcquarterHourAttr.aReceiveBlock;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= QUARTERHOUR_ATTR_FLAG;
	return result;
}

int8 getTransmitBlock_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 total= 0, qutHourSt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "TransmitBlock", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastTransmitBlock", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalTransmitBlock", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOQUTHOURST_NODE, "QutHrStTransmitBlock", tmp);
	qutHourSt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-qutHourSt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getTransmitBlock_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcquarterHourAttr.aTransmitBlock;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTransmitBlock_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcquarterHourAttr.aTransmitBlock;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= QUARTERHOUR_ATTR_FLAG;
	return result;
}

int8 getCellDelin_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 total= 0, qutHourSt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "CellDelin", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastCellDelin", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalCellDelin", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOQUTHOURST_NODE, "QutHrStCellDelin", tmp);
	qutHourSt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-qutHourSt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getCellDelin_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcquarterHourAttr.aCellDelin;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCellDelin_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcquarterHourAttr.aCellDelin;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= QUARTERHOUR_ATTR_FLAG;
	return result;
}

int8 getLinkRetrain_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 total= 0, qutHourSt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "LinkRetrain", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastLinkRetrain", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalLinkRetrain", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOQUTHOURST_NODE, "QutHrStLinkRetrain", tmp);
	qutHourSt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-qutHourSt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getLinkRetrain_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcquarterHourAttr.aLinkRetrain;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLinkRetrain_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcquarterHourAttr.aLinkRetrain;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= QUARTERHOUR_ATTR_FLAG;
	return result;
}

int8 getInitErrors_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 total= 0, qutHourSt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "InitErrors", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastInitErrors", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalInitErrors", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOQUTHOURST_NODE, "QutHrStInitErrors", tmp);
	qutHourSt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-qutHourSt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getInitErrors_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcquarterHourAttr.aInitErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setInitErrors_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcquarterHourAttr.aInitErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= QUARTERHOUR_ATTR_FLAG;
	return result;
}

int8 getInitTimeouts_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 total= 0, qutHourSt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "InitTimeouts", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastInitTimeouts", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalInitTimeouts", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOQUTHOURST_NODE, "QutHrStInitTimeouts", tmp);
	qutHourSt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-qutHourSt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getInitTimeouts_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcquarterHourAttr.aInitTimeouts;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setInitTimeouts_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcquarterHourAttr.aInitTimeouts;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= QUARTERHOUR_ATTR_FLAG;
	return result;
}

int8 getLossOfFraming_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 total= 0, qutHourSt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "LossOfFraming", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastLossOfFraming", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalLossOfFraming", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOQUTHOURST_NODE, "QutHrStLossOfFraming", tmp);
	qutHourSt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-qutHourSt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getLossOfFraming_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcquarterHourAttr.aLossOfFraming;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLossOfFraming_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcquarterHourAttr.aLossOfFraming;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= QUARTERHOUR_ATTR_FLAG;
	return result;
}

int8 getErroredSecs_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	uint32 total= 0, qutHourSt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "ErroredSecs", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastErroredSecs", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalErroredSecs", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOQUTHOURST_NODE, "QutHrStErroredSecs", tmp);
	qutHourSt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-qutHourSt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getErroredSecs_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcquarterHourAttr.aErroredSecs;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setErroredSecs_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcquarterHourAttr.aErroredSecs;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= QUARTERHOUR_ATTR_FLAG;
	return result;
}

int8 getSeveErrSecs_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[32] = {0};
	char tmp[32] = {0};
	uint32 total= 0, qutHourSt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "SeveErrSecs", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastSeveErrSecs", tmp);
	tmpValue = atolu(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalSeveErrSecs", tmp);
	total = atolu(tmp);
	
	tcapi_get(INFOQUTHOURST_NODE, "QutHrStSeveErrSecs", tmp);
	qutHourSt = atolu(tmp);
	
	sprintf(result, "%lu", total+tmpValue-qutHourSt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getSeveErrSecs_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcquarterHourAttr.aSeverelyErroredSecs;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setSeveErrSecs_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcquarterHourAttr.aSeverelyErroredSecs;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= QUARTERHOUR_ATTR_FLAG;
	return result;
}

int8 getFecErrors_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	int total= 0, qutHourSt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "FecErrors", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastFecErrors", tmp);
	tmpValue = atoi(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalFecErrors", tmp);	
	total = atoi(tmp);
	
	tcapi_get(INFOQUTHOURST_NODE, "QutHrStFecErrors", tmp);
	qutHourSt = atoi(tmp);
	
	sprintf(result, "%u", total+tmpValue-qutHourSt);

	return getTheValue(value, type, deviceNode, result);
}

int8 getFecErrors_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcquarterHourAttr.aFecErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setFecErrors_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcquarterHourAttr.aFecErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= QUARTERHOUR_ATTR_FLAG;
	return result;
}

int8 getATUCFECErrors_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	int total= 0, qutHourSt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "ATUCFEC", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastATUCFEC", tmp);
	tmpValue = atoi(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalATUCFEC", tmp);
	total = atoi(tmp);
	
	tcapi_get(INFOQUTHOURST_NODE, "QutHrStATUCFEC", tmp);
	qutHourSt = atoi(tmp);
	
	sprintf(result, "%u", total+tmpValue-qutHourSt);
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getATUCFECErrors_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcquarterHourAttr.aATUCFECErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATUCFECErrors_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcquarterHourAttr.aATUCFECErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= QUARTERHOUR_ATTR_FLAG;
	return result;
}

int8 getHECErrors_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	int total= 0, qutHourSt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "HECErrors", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastHECErrors", tmp);
	tmpValue = atoi(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalHECErrors", tmp);
	total = atoi(tmp);
	
	tcapi_get(INFOQUTHOURST_NODE, "QutHrStHECErrors", tmp);
	qutHourSt = atoi(tmp);
	
	sprintf(result, "%u", total+tmpValue-qutHourSt);
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getHECErrors_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcquarterHourAttr.aHECErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setHECErrors_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcquarterHourAttr.aHECErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= QUARTERHOUR_ATTR_FLAG;
	return result;
}

int8 getCRCErrors_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	int total= 0, qutHourSt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "CRCErrors", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastCRCErrors", tmp);
	tmpValue = atoi(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalCRCErrors", tmp);
	total = atoi(tmp);
	
	tcapi_get(INFOQUTHOURST_NODE, "QutHrStCRCErrors", tmp);
	qutHourSt = atoi(tmp);
	
	sprintf(result, "%u", total+tmpValue-qutHourSt);
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getCRCErrors_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcquarterHourAttr.aCRCErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCRCErrors_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcquarterHourAttr.aCRCErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= QUARTERHOUR_ATTR_FLAG;
	return result;
}

int8 getATUCCRCErrors_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[20] = {0};
	char tmp[20] = {0};
	int total= 0, qutHourSt = 0, tmpValue = 0;

	tcapi_get(INFOTOTAL_NODE, "lineState", tmp);
	if(0 == strcmp(tmp, "up"))
		tcapi_get(INFOSHOW_NODE, "ATUCCRCErrors", tmp);
	else
		tcapi_get(INFOLAST_NODE, "LastATUCCRCErrors", tmp);
	tmpValue = atoi(tmp);

	tcapi_get(INFOTOTAL_NODE, "TotalATUCCRCErrors", tmp);
	total = atoi(tmp);
	
	tcapi_get(INFOQUTHOURST_NODE, "QutHrStATUCCRCErrors", tmp);
	qutHourSt = atoi(tmp);
	
	sprintf(result, "%u", total+tmpValue-qutHourSt);
	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getATUCCRCErrors_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcquarterHourAttr.aATUCCRCErrors;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setATUCCRCErrors_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcquarterHourAttr.aATUCCRCErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= QUARTERHOUR_ATTR_FLAG;
	return result;
}

/*_____________________________________________________________________________
**      function name: atolu
**      descriptions:
**            Transfer string to uint32.
**
**      parameters:
**            nptr: string which needs to transfer to uint32
**
**      global:
**             None
**
**      return:
**            Success:        uint32 value
**            Otherwise:      0
**
**      revision:
**      1. renyu 2010/02/01
**____________________________________________________________________________
*/
uint32 atolu(const char* nptr){
#if 0
	int c;
	uint32 total = 0;
	
	while(isspace((int)(unsigned char)*nptr))
		++nptr;

	c = (int)(unsigned char)*nptr++;
	while(isdigit(c)){
		total = 10*total + (c - '0');
		c = (int)(unsigned char)*nptr++;
	}
	return total;
#else
	uint32 result = 0;
	sscanf(nptr, "%lu", &result);
	return result;
#endif
}

