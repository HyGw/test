#include <math.h>
#include "Global_res.h"
#include "cp_wandev_wanxponInterCfg.h"

/*
    rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.X_CT-COM_EponInterfaceConfig.
	                                 InternetGatewayDevice.WANDevice.1.X_CT-COM_GponInterfaceConfig.
*/

int8 getWANPONInterCfg_StatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeValue[33] = {0};
	char status[32] = "NoSignal";
	tcapi_get("XPON_LinkCfg","LinkSta",nodeValue);
	tcapi_get("XPON_Common", "phyStatus",status);
	if(!strcmp(nodeValue, "0"))
	{
		if(!strstr(status,"up"))
			sprintf(status,"Initializing");
	}
	else
	{
		sprintf(status,"Up");
	}

	return getTheValue(value, type, deviceNode, status);
}
int8 getWANPONInterCfg_StatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgStatus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_StatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgStatus;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_TXPowerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeValue[33] = {0};
	char txPower[32] = "0";
	tcapi_get("XPON_LinkCfg","LinkSta",nodeValue);
	
	if(strcmp(nodeValue, "0"))
	{
		tcapi_get("Info_PonPhy","TxPower",txPower);
#if defined(TCSUPPORT_CT_FJ)
		sprintf(txPower,"%.f",(round(log10((double)(atoi(txPower))/10000) * 100)/10));				
#endif
	}
	return getTheValue(value, type, deviceNode, txPower);
}
int8 getWANPONInterCfg_TXPowerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgTXPower;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_TXPowerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgTXPower;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_RXPowerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeValue[33] = {0};
	char rxPower[32] = "0";
	tcapi_get("XPON_LinkCfg","LinkSta",nodeValue);
	
	if(strcmp(nodeValue, "0"))
	{
		tcapi_get("Info_PonPhy","RxPower",rxPower);
#if defined(TCSUPPORT_CT_FJ)
		sprintf(rxPower,"%.f",(round(log10((double)(atoi(rxPower))/10000) * 100)/10));				
#endif
	}

	return getTheValue(value, type, deviceNode, rxPower);
}
int8 getWANPONInterCfg_RXPowerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgRXPower;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_RXPowerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgRXPower;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_TransTemperatureValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeValue[33] = {0};
	char temperature[32] = "0";
	int tmp = 0;
	
	tcapi_get("XPON_LinkCfg","LinkSta",nodeValue);
	
	if(strcmp(nodeValue, "0"))
	{	
		tcapi_get("Info_PonPhy","Temperature",temperature);
		if(strcmp(temperature,"N/A"))
		{
			tmp = atoi(temperature);
			if(tmp >= (1<<15))
				tmp-=(1<<15);			
			sprintf(temperature,"%d",tmp);
		}	
	}

	return getTheValue(value, type, deviceNode, temperature);
}
int8 getWANPONInterCfg_TransTemperatureAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgTransTemperature;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_TransTemperatureAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgTransTemperature;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_SupplyVottageValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeValue[33] = {0};
	char supplyVottage[32] = "0";
	tcapi_get("XPON_LinkCfg","LinkSta",nodeValue);
	
	if(strcmp(nodeValue, "0"))
	{	
		tcapi_get("Info_PonPhy","SupplyVoltage",supplyVottage);
	}

	return getTheValue(value, type, deviceNode, supplyVottage);
}
int8 getWANPONInterCfg_SupplyVottageAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgSupplyVottage;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_SupplyVottageAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgSupplyVottage;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_BiasCurrentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeValue[33] = {0};
	char biasCurrent[32] = "0";
	tcapi_get("XPON_LinkCfg","LinkSta",nodeValue);
	
	if(strcmp(nodeValue, "0"))
	{	
		tcapi_get("Info_PonPhy","TxBiasCurrent",biasCurrent);
	}

	return getTheValue(value, type, deviceNode, biasCurrent);
}
int8 getWANPONInterCfg_BiasCurrentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgBiasCurrent;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_BiasCurrentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgBiasCurrent;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

#if defined(TCSUPPORT_CT_PMINFORM)

int8 getWANPONInterCfg_DownDataValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char DownData[32] = {0};

	tcapi_get("Info_PonWanStats","RxFrameLen",DownData);
	double temp = atof(DownData)/(1024*1024);
	sprintf(DownData,"%.3f" ,temp);

	return getTheValue(value, type, deviceNode, DownData);
}
int8 getWANPONInterCfg_DownDataAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgDownData;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_DownDataAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgDownData;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_UpDataValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char UpData[32] = {0};

	tcapi_get("Info_PonWanStats","TxFrameLen",UpData);
	
	double temp = atof(UpData)/(1024*1024);
	sprintf(UpData,"%.3f" ,temp);
	return getTheValue(value, type, deviceNode, UpData);
}
int8 getWANPONInterCfg_UpDataAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgUpData;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_UpDataAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgUpData;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_TEMPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeValue[33] = {0};
	char TEMP[32] = {0};

	GetTEMP(TEMP);

	return getTheValue(value, type, deviceNode, TEMP);
}
int8 getWANPONInterCfg_TEMPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgTEMP;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_TEMPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgTEMP;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PacketLostValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char PacketLost[32] = {0};

	tcapi_get("Info_PonWanStats","TxDropCnt",PacketLost);

	return getTheValue(value, type, deviceNode, PacketLost);
	
}
int8 getWANPONInterCfg_PacketLostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPacketLost;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PacketLostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPacketLost;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PLRValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char PLR[32] = {0};
	
	GetPLR(PLR);

	return getTheValue(value, type, deviceNode, PLR);
	
}
int8 getWANPONInterCfg_PLRAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPLR;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PLRAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPLR;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_ErrorCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char ErrorCode[32] = {0};

	tcapi_get("Info_PonWanStats","RxCrcCnt",ErrorCode);

	return getTheValue(value, type, deviceNode, ErrorCode);
	
}
int8 getWANPONInterCfg_ErrorCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgErrorCode;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_ErrorCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgErrorCode;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_SERValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char SER[32] = {0};	
	
	GetSER(SER);

	return getTheValue(value, type, deviceNode, SER);
	
}
int8 getWANPONInterCfg_SERAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgSER;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_SERAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgSER;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_MemRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	
	char nodeName[33] = {0};
	char MemRate[32] = {0};

	GetMemRate(MemRate);

	return getTheValue(value, type, deviceNode, MemRate);


}
int8 getWANPONInterCfg_MemRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgMemRate;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_MemRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgMemRate;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_CPURateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char CPURate[32] = {0};

	GetCPURate(CPURate);	

	return getTheValue(value, type, deviceNode, CPURate);


}
int8 getWANPONInterCfg_CPURateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgCPURate;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_CPURateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgCPURate;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PMInformEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char vEnable[4] = {0};

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_ENABLE, tmp);
	strcpy(vEnable, ( 0 == strcmp(tmp, PMINFORM_VAL_YES) ) ?
								PMINFORM_VAL_1 : PMINFORM_VAL_0 );
    g_PMInformEnable = ( 0 == strcmp(tmp, PMINFORM_VAL_YES) ) ? 1 : 0;
	return getTheValue(value, type, deviceNode, vEnable);
}
int8 setWANPONInterCfg_PMInformEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	uint8 i_enable = *((uint8*)value);

	SetCfgNodeFlag("PMInform", -1);

	if( 1 == i_enable )
		strcpy(tmp, PMINFORM_VAL_YES);
	else if( 0 == i_enable )
		strcpy(tmp, PMINFORM_VAL_NO);
	else
		return -1;

	tcapi_set(PMINFORM_NODENAME, PMINFORM_ATTR_ENABLE, tmp);
	return 0;
}
int8 getWANPONInterCfg_PMInformEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPMInformEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PMInformEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPMInformEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PMInformLSURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char url[256] = {0};

	if ( 0 != tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_LSURL, url) )
		url[0] = 0;

	return getTheValue(value, type, deviceNode, url);
}
int8 setWANPONInterCfg_PMInformLSURLValue(void* value, struct DeviceNode*  deviceNode)
{
	char url[256] = {0};
	char *purl = (char*)value;

	SetCfgNodeFlag("PMInform", -1);
	
	if ( NULL == purl
		|| strlen(purl) >= sizeof(url) )
		return -1;
	strcpy(url, purl);

	tcapi_set(PMINFORM_NODENAME, PMINFORM_ATTR_LSURL, url);
	return 0;
}
int8 getWANPONInterCfg_PMInformLSURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPMInformLSURL;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PMInformLSURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPMInformLSURL;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PMInformLUIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char val[16] = {0};

	if ( 0 != tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_LUINTVAL, val) )
		strcpy(val, "3600");

	return getTheValue(value, type, deviceNode, val);
}
int8 setWANPONInterCfg_PMInformLUIntervalValue(void* value, struct DeviceNode*  deviceNode)
{
	char val[16] = {0};
	uint32 uVal = *((uint32*)value);

	SetCfgNodeFlag("PMInform", -1);

	if ( uVal < PMINFORM_VAL_DEFLCINTVAL
		|| 0 != (uVal % PMINFORM_VAL_DEFLCINTVAL) )
		return -1;

	sprintf(val, "%lu", uVal);

	tcapi_set(PMINFORM_NODENAME, PMINFORM_ATTR_LUINTVAL, val);
	return 0;
}
int8 getWANPONInterCfg_PMInformLUIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPMInformLUInterval;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PMInformLUIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPMInformLUInterval;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PMInformLCIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char val[16] = {0};

	if ( 0 != tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_LCINTVAL, val) )
		strcpy(val, "300");

	return getTheValue(value, type, deviceNode, val);
}
int8 setWANPONInterCfg_PMInformLCIntervalValue(void* value, struct DeviceNode*  deviceNode)
{
	char val[16] = {0};
	uint32 uVal = *((uint32*)value);

	SetCfgNodeFlag("PMInform", -1);

	if ( uVal < PMINFORM_VAL_DEFLCINTVAL
		|| 0 != (uVal % PMINFORM_VAL_DEFLCINTVAL) )
		return -1;

	sprintf(val, "%lu", uVal);

	tcapi_set(PMINFORM_NODENAME, PMINFORM_ATTR_LCINTVAL, val);
	return 0;
}
int8 getWANPONInterCfg_PMInformLCIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPMInformLCInterval;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PMInformLCIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPMInformLCInterval;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PMInformCREnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char vEnable[4] = {0};

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_CPURENABLE, tmp);
	strcpy(vEnable, ( 0 == strcmp(tmp, PMINFORM_VAL_YES) ) ?
								PMINFORM_VAL_1 : PMINFORM_VAL_0 );

	return getTheValue(value, type, deviceNode, vEnable);
}
int8 setWANPONInterCfg_PMInformCREnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	uint8 i_enable = *((uint8*)value);

	SetCfgNodeFlag("PMInform", -1);

	if( 1 == i_enable )
		strcpy(tmp, PMINFORM_VAL_YES);
	else if( 0 == i_enable )
		strcpy(tmp, PMINFORM_VAL_NO);
	else
		return -1;

	tcapi_set(PMINFORM_NODENAME, PMINFORM_ATTR_CPURENABLE, tmp);
	return 0;
}
int8 getWANPONInterCfg_PMInformCREnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPMInformCREnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PMInformCREnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPMInformCREnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PMInformMREnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char vEnable[4] = {0};

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_MEMRENABLE, tmp);
	strcpy(vEnable, ( 0 == strcmp(tmp, PMINFORM_VAL_YES) ) ?
								PMINFORM_VAL_1 : PMINFORM_VAL_0 );

	return getTheValue(value, type, deviceNode, vEnable);
}
int8 setWANPONInterCfg_PMInformMREnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	uint8 i_enable = *((uint8*)value);

	SetCfgNodeFlag("PMInform", -1);

	if( 1 == i_enable )
		strcpy(tmp, PMINFORM_VAL_YES);
	else if( 0 == i_enable )
		strcpy(tmp, PMINFORM_VAL_NO);
	else
		return -1;

	tcapi_set(PMINFORM_NODENAME, PMINFORM_ATTR_MEMRENABLE, tmp);
	return 0;
}
int8 getWANPONInterCfg_PMInformMREnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPMInformMREnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PMInformMREnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPMInformMREnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PMInformSEREnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char vEnable[4] = {0};

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_SERENABLE, tmp);
	strcpy(vEnable, ( 0 == strcmp(tmp, PMINFORM_VAL_YES) ) ?
								PMINFORM_VAL_1 : PMINFORM_VAL_0 );

	return getTheValue(value, type, deviceNode, vEnable);
}
int8 setWANPONInterCfg_PMInformSEREnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	uint8 i_enable = *((uint8*)value);

	SetCfgNodeFlag("PMInform", -1);

	if( 1 == i_enable )
		strcpy(tmp, PMINFORM_VAL_YES);
	else if( 0 == i_enable )
		strcpy(tmp, PMINFORM_VAL_NO);
	else
		return -1;

	tcapi_set(PMINFORM_NODENAME, PMINFORM_ATTR_SERENABLE, tmp);
	return 0;
}
int8 getWANPONInterCfg_PMInformSEREnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPMInformSEREnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PMInformSEREnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPMInformSEREnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PMInformECEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char vEnable[4] = {0};

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_ERRCODEENABLE, tmp);
	strcpy(vEnable, ( 0 == strcmp(tmp, PMINFORM_VAL_YES) ) ?
								PMINFORM_VAL_1 : PMINFORM_VAL_0 );

	return getTheValue(value, type, deviceNode, vEnable);
}
int8 setWANPONInterCfg_PMInformECEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	uint8 i_enable = *((uint8*)value);

	SetCfgNodeFlag("PMInform", -1);
	
	if( 1 == i_enable )
		strcpy(tmp, PMINFORM_VAL_YES);
	else if( 0 == i_enable )
		strcpy(tmp, PMINFORM_VAL_NO);
	else
		return -1;

	tcapi_set(PMINFORM_NODENAME, PMINFORM_ATTR_ERRCODEENABLE, tmp);
	return 0;
}
int8 getWANPONInterCfg_PMInformECEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPMInformECEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PMInformECEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPMInformECEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PMInformPLREnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char vEnable[4] = {0};

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_PLRENABLE, tmp);
	strcpy(vEnable, ( 0 == strcmp(tmp, PMINFORM_VAL_YES) ) ?
								PMINFORM_VAL_1 : PMINFORM_VAL_0 );

	return getTheValue(value, type, deviceNode, vEnable);
}
int8 setWANPONInterCfg_PMInformPLREnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	uint8 i_enable = *((uint8*)value);

	SetCfgNodeFlag("PMInform", -1);
	
	if( 1 == i_enable )
		strcpy(tmp, PMINFORM_VAL_YES);
	else if( 0 == i_enable )
		strcpy(tmp, PMINFORM_VAL_NO);
	else
		return -1;

	tcapi_set(PMINFORM_NODENAME, PMINFORM_ATTR_PLRENABLE, tmp);
	return 0;
}
int8 getWANPONInterCfg_PMInformPLREnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPMInformPLREnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PMInformPLREnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPMInformPLREnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PMInformPLEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char vEnable[4] = {0};

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_PKTLOSTENABLE, tmp);
	strcpy(vEnable, ( 0 == strcmp(tmp, PMINFORM_VAL_YES) ) ?
								PMINFORM_VAL_1 : PMINFORM_VAL_0 );

	return getTheValue(value, type, deviceNode, vEnable);
}
int8 setWANPONInterCfg_PMInformPLEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	uint8 i_enable = *((uint8*)value);

	SetCfgNodeFlag("PMInform", -1);

	if( 1 == i_enable )
		strcpy(tmp, PMINFORM_VAL_YES);
	else if( 0 == i_enable )
		strcpy(tmp, PMINFORM_VAL_NO);
	else
		return -1;

	tcapi_set(PMINFORM_NODENAME, PMINFORM_ATTR_PKTLOSTENABLE, tmp);
	return 0;
}
int8 getWANPONInterCfg_PMInformPLEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPMInformPLEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PMInformPLEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPMInformPLEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PMInformTMPEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char vEnable[4] = {0};

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_TEMPENABLE, tmp);
	strcpy(vEnable, ( 0 == strcmp(tmp, PMINFORM_VAL_YES) ) ?
								PMINFORM_VAL_1 : PMINFORM_VAL_0 );

	return getTheValue(value, type, deviceNode, vEnable);
}
int8 setWANPONInterCfg_PMInformTMPEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	uint8 i_enable = *((uint8*)value);

	SetCfgNodeFlag("PMInform", -1);

	if( 1 == i_enable )
		strcpy(tmp, PMINFORM_VAL_YES);
	else if( 0 == i_enable )
		strcpy(tmp, PMINFORM_VAL_NO);
	else
		return -1;

	tcapi_set(PMINFORM_NODENAME, PMINFORM_ATTR_TEMPENABLE, tmp);
	return 0;
}
int8 getWANPONInterCfg_PMInformTMPEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPMInformTMPEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PMInformTMPEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPMInformTMPEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PMInformUpDEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char vEnable[4] = {0};

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_UPDATAENABLE, tmp);
	strcpy(vEnable, ( 0 == strcmp(tmp, PMINFORM_VAL_YES) ) ?
								PMINFORM_VAL_1 : PMINFORM_VAL_0 );

	return getTheValue(value, type, deviceNode, vEnable);
}
int8 setWANPONInterCfg_PMInformUpDEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	uint8 i_enable = *((uint8*)value);

	SetCfgNodeFlag("PMInform", -1);

	if( 1 == i_enable )
		strcpy(tmp, PMINFORM_VAL_YES);
	else if( 0 == i_enable )
		strcpy(tmp, PMINFORM_VAL_NO);
	else
		return -1;

	tcapi_set(PMINFORM_NODENAME, PMINFORM_ATTR_UPDATAENABLE, tmp);
	return 0;
}
int8 getWANPONInterCfg_PMInformUpDEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPMInformUpDEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PMInformUpDEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPMInformUpDEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PMInformDwDEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char vEnable[4] = {0};

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_DWDATAENABLE, tmp);
	strcpy(vEnable, ( 0 == strcmp(tmp, PMINFORM_VAL_YES) ) ?
								PMINFORM_VAL_1 : PMINFORM_VAL_0 );

	return getTheValue(value, type, deviceNode, vEnable);
}
int8 setWANPONInterCfg_PMInformDwDEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	uint8 i_enable = *((uint8*)value);

	SetCfgNodeFlag("PMInform", -1);

	if( 1 == i_enable )
		strcpy(tmp, PMINFORM_VAL_YES);
	else if( 0 == i_enable )
		strcpy(tmp, PMINFORM_VAL_NO);
	else
		return -1;

	tcapi_set(PMINFORM_NODENAME, PMINFORM_ATTR_DWDATAENABLE, tmp);
	return 0;
}
int8 getWANPONInterCfg_PMInformDwDEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPMInformDwDEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PMInformDwDEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPMInformDwDEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PMInformLanDevMacEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char vEnable[4] = {0};

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_LANDEVMACENABLE, tmp);
	strcpy(vEnable, ( 0 == strcmp(tmp, PMINFORM_VAL_YES) ) ?
								PMINFORM_VAL_1 : PMINFORM_VAL_0 );

	return getTheValue(value, type, deviceNode, vEnable);
}
int8 setWANPONInterCfg_PMInformLanDevMacEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	uint8 i_enable = *((uint8*)value);

	SetCfgNodeFlag("PMInform", -1);

	if( 1 == i_enable )
		strcpy(tmp, PMINFORM_VAL_YES);
	else if( 0 == i_enable )
		strcpy(tmp, PMINFORM_VAL_NO);
	else
		return -1;

	tcapi_set(PMINFORM_NODENAME, PMINFORM_ATTR_LANDEVMACENABLE, tmp);
	return 0;
}
int8 getWANPONInterCfg_PMInformLanDevMacEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPMInformLanDevMACEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PMInformLanDevMacEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPMInformLanDevMACEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

extern int getLANDeviceMAC(char *macbuf, int maxlen);
int8 getWANPONInterCfg_PMInformLanDevMacValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char landevmac[512] = {0};
	char lanmacAttr[32] = {0};
	char tmpval[32] = {0};
	int i = 0;

#if 1
	getLANDeviceMAC(landevmac, sizeof(landevmac) - 1);
#else

	for(i=0; i<5; i++){
		memset(tmpval,0,sizeof(tmpval));
		sprintf(lanmacAttr, "Eth%dDevMAC", i);
		tcapi_get("LanHost",lanmacAttr,tmpval);
		if(strlen(tmpval)>0){
			if(strlen(landevmac)>0)
				strcat(landevmac,"-");
			sprintf(lanmacAttr, "LAN%d:", i);
			strcat(landevmac,lanmacAttr);
			strcat(landevmac,tmpval);
		}
	}
#endif

	return getTheValue(value, type, deviceNode, landevmac);	
}
int8 getWANPONInterCfg_PMInformLanDevMacAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPMInformLanDevMAC;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PMInformLanDevMacAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPMInformLanDevMAC;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PMInformWlanDevMacEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char vEnable[4] = {0};

	tcapi_get(PMINFORM_NODENAME, PMINFORM_ATTR_WLANDEVMACENABLE, tmp);
	strcpy(vEnable, ( 0 == strcmp(tmp, PMINFORM_VAL_YES) ) ?
								PMINFORM_VAL_1 : PMINFORM_VAL_0 );

	return getTheValue(value, type, deviceNode, vEnable);
}
int8 setWANPONInterCfg_PMInformWlanDevMacEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	uint8 i_enable = *((uint8*)value);

	SetCfgNodeFlag("PMInform", -1);

	if( 1 == i_enable )
		strcpy(tmp, PMINFORM_VAL_YES);
	else if( 0 == i_enable )
		strcpy(tmp, PMINFORM_VAL_NO);
	else
		return -1;

	tcapi_set(PMINFORM_NODENAME, PMINFORM_ATTR_WLANDEVMACENABLE, tmp);
	return 0;
}
int8 getWANPONInterCfg_PMInformWlanDevMacEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPMInformWlanDevMACEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PMInformWlanDevMacEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPMInformWlanDevMACEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PMInformWlanDevMacValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char DataBuf[512] = {0};

	tcapi_get("LanHost","WlanDevMAC",DataBuf);
	upper(DataBuf);
	
	return getTheValue(value, type, deviceNode, DataBuf);
}
int8 getWANPONInterCfg_PMInformWlanDevMacAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgPMInformWlanDevMAC;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PMInformWlanDevMacAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgPMInformWlanDevMAC;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}
#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getWANPONInterCfg_EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char node_name[32] = {0};
	char vEnable[4] = {0};	

	tcapi_get("XPON_Common", "PONCfgEnable", tmp);
	if(strcmp(tmp, "Yes") == 0 || strcmp(tmp, "1") == 0 || strcmp(tmp, "true") == 0)
		strcpy(vEnable, "1"); 
	else
		strcpy(vEnable, "0"); 
		
	return getTheValue(value, type, deviceNode, vEnable);
}

int8 setWANPONInterCfg_EnableValue(void* value, struct DeviceNode*  deviceNode){
	char tmp[8];
	char node_name[32] = {0};
	boolean* enable;

	enable = (boolean *)value;
	if( *enable == (boolean)0)
		sprintf(tmp, "%s", "0");
	else if( *enable == (boolean)1 ) 
		sprintf(tmp, "%s", "1");
	else 
		return -1;

	tcapi_set("XPON_Common", "PONCfgEnable", tmp);
	return 0;
}

int8 getWANPONInterCfg_EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_StandardsSupportedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char eponStandardsSupportedStrPtr[] = "EPON";
	char gponStandardsSupportedStrPtr[] = "GPON_SYM";
	char nodeValue[33] = {0};
	
	tcapi_get("XPON_LinkCfg","LinkSta",nodeValue);
	if(!strcmp(nodeValue, "2"))
	{	
		return getTheValue(value, type, deviceNode, eponStandardsSupportedStrPtr);
	}
	
	return getTheValue(value, type, deviceNode, gponStandardsSupportedStrPtr);
}
int8 getWANPONInterCfg_StandardsSupportedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgStandardsSupported;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_StandardsSupportedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgStandardsSupported;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_StandardUsedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char gponStandardUsedStrPtr[] = "GPON_SYM";
	char eponStandardUsedStrPtr[] = "EPON";
	char nodeValue[33] = {0};
	
	tcapi_get("XPON_LinkCfg","LinkSta",nodeValue);
	if(!strcmp(nodeValue, "2"))
	{		
		return getTheValue(value, type, deviceNode, eponStandardUsedStrPtr);		
	}
	
	return getTheValue(value, type, deviceNode, gponStandardUsedStrPtr);
}
int8 getWANPONInterCfg_StandardUsedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgStandardUsed;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_StandardUsedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgStandardUsed;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_UpstreamMaxRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeValue[33] = {0};
	char GUpstreamMaxRate[33] = "1244";
	char EUpstreamMaxRate[33] = "1000";
	
	tcapi_get("XPON_LinkCfg","LinkSta",nodeValue);
	if(!strcmp(nodeValue, "2"))
	{	
		getTheValue(value, type, deviceNode, EUpstreamMaxRate);
	}
	
	return getTheValue(value, type, deviceNode, GUpstreamMaxRate);
}
int8 getWANPONInterCfg_UpstreamMaxRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgUpstreamMaxRate;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_UpstreamMaxRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgUpstreamMaxRate;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_DownstreamMaxRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeValue[33] = {0};
	char GDownstreamMaxRate[33] = "2488";
	char EDownstreamMaxRate[33] = "1000";
	
	tcapi_get("XPON_LinkCfg","LinkSta",nodeValue);
	if(!strcmp(nodeValue, "2"))
	{	
		getTheValue(value, type, deviceNode, EDownstreamMaxRate);
	}
	
	return getTheValue(value, type, deviceNode, GDownstreamMaxRate);
}
int8 getWANPONInterCfg_DownstreamMaxRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgAttr.aWANPonInterCfgDownstreamMaxRate;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_DownstreamMaxRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgAttr.aWANPonInterCfgDownstreamMaxRate;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFG_ATTR_FLAG;
	return result;
}

#endif
