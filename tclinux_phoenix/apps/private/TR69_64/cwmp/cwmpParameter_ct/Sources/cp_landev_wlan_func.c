#include "Global_res.h"
#include "cp_landev_wlan.h"

static  char  gTransmitRatesList[MAX_LEN_OF_SUPPORTED_RATES + 1][5] = {"Auto", "1","2","5.5","11","6","9","12","18","24","36","48","54"};
static  char  gBasicDataTransmitRates[3][64] = {"1,2,5.5,11", "6,12,24","1,2,5.5,6,11,12,24"};
static  char  gOperationalDataTransmitRates[3][64] = {"1,2,5.5,11", "6,12,24", "1,2,5.5,6,11,12,24"};
static  char  gPossibleDataTransmitRates[3][64] = {"1,2,5.5,11", "6,9,12,18,24,36,48,54", "1,2,5.5,6,9,11,12,18,24,36,48,54"};
static  uint8 AutoRateFallBackFlag[4] = {0, 0};

#if !defined(TCSUPPORT_C1_CUC)
int8 getWLANCfgEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char wlanEnable[10]= {'\0'};

	tcapi_get("WLan_Common","APOn",wlanEnable);
	
	return getTheValue(value, type, deviceNode, wlanEnable);
}
int8 setWLANCfgEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 flag;
	
	flag = *((uint8*)value);

	SetCfgNodeFlag ( "WLan", -1 );
	
	if(flag == 1){
		tcapi_set("WLan_Common","APOn","1");
	} 
	else if (flag == 0){
		tcapi_set("WLan_Common","APOn","0");
	}
	else{
		tc_printf("==>setWLANCfgEnableValue:value should be 0 or 1.\n");
		return -1;
	}	
	
	return 0;
}

int8 getWLANCfgEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNodePtr deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcWlanAttr.aWLanEnable[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWLANCfgEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNodePtr deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aWLanEnable[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
#endif
#if defined(TCSUPPORT_CT_WLAN_NODE)
int8 getWLANCfgSSIDEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ssidEnable[10]= {'\0'};
	char nodeName[34];
	int instance;

	memset(nodeName, 0, sizeof(nodeName));
	memset(ssidEnable, 0, sizeof(ssidEnable));
	instance = atoi(deviceNode->parentNode->name);
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	tcapi_get(nodeName,"EnableSSID",ssidEnable);
	
	return getTheValue(value, type, deviceNode, ssidEnable);
}

int8 setWLANCfgSSIDEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 flag;
	char nodeName[34];
	int instance;
	
	flag = *((uint8*)value);
	
	memset(nodeName, 0, sizeof(nodeName));	
	instance = atoi(deviceNode->parentNode->name);
	sprintf(nodeName, "WLan_Entry%d", instance - 1);
	
	if(flag == 1){
		tcapi_set(nodeName,"EnableSSID","1");
	} 
	else if (flag == 0){
		tcapi_set(nodeName,"EnableSSID","0");
	}
	else{
		tc_printf("==>setWLANCfgSSIDEnableValue:value should be 0 or 1.\n");
		return -1;
	}	
        
        SetCfgNodeFlag ( "WLan", instance - 1 );	
	return 0;
}

int8 getWLANCfgSSIDEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNodePtr deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcWlanAttr.aSSIDEnable[instance - 1];
	
#if defined(TCSUPPORT_CT)
	if (attribute & 0x01)
		attribute &= 0xfe;
	else
		attribute |= 0x1;
#endif

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWLANCfgSSIDEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNodePtr deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aSSIDEnable[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

#if defined(TCSUPPORT_CT)
	if (*attribute & 0x01)
		*attribute &= 0xfe;
	else
		*attribute |= 0x1;
#endif

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

#if !defined(TCSUPPORT_C1_CUC)
int8 getWLANCfgWPSKeyWordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WPSKeyWord[10]= {'\0'};

	tcapi_get("WLan_Common", "WPSKeyWord", WPSKeyWord);
	
	return getTheValue(value, type, deviceNode, WPSKeyWord);
}

int8 setWLANCfgWPSKeyWordValue(void* value, struct DeviceNode*  deviceNode)
{
	char WPSKeyWord[10] = {'\0'};
	int key;
	
	key = *((uint32*)value);
	if(key > 128 || key < 0){
		tc_printf("==>setWLANCfgWPSKeyWordValue: 0~128\n");
		return -1;
	}
	
	sprintf(WPSKeyWord, "%lu", key);
	tcapi_set("WLan_Common","WPSKeyWord",WPSKeyWord);
	
	SetCfgNodeFlag( "WLan", -1 );
	return 0;
}

int8 getWLANCfgWPSKeyWordAttribute(uint8* notification, AccessEntry** acl, DeviceNodePtr deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcWlanAttr.aWPSKeyWord;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	return result;
}

int8 setWLANCfgWPSKeyWordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNodePtr deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcWlanAttr.aWPSKeyWord;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWLANCfgPowerlevelValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char powerlevel[10]= {'\0'};
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	char power[8] = {0};
#endif

	tcapi_get("WLan_Common","TxPowerLevel",powerlevel);
		
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	switch(atoi(powerlevel)){
		case 5:
			strcpy(power,"15");
			break;
		case 4:
			strcpy(power,"25");
			break;
		case 3:
			strcpy(power,"50");
			break;
		case 2:
			strcpy(power,"75");
			break;
		case 1:
			strcpy(power,"100");
			break;
		default:
			break;
	}
	strcpy(powerlevel,power);
#endif
		
	return getTheValue(value, type, deviceNode, powerlevel);
}

int8 setWLANCfgPowerlevelValue(void* value, struct DeviceNode*  deviceNode)
{
	char powerlevel[10] = {'\0'};
	int level;
	
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	level = *((uint32*)value);
	if(level > 5 || level < 1){
		tc_printf("==>setWLANCfgPowerlevelValue: 1~5\n");
		return -1;
	}
#else
	switch(*((uint32*)value)){
		case 100:
			level = 1;
			break;
		case 75:
			level = 2;
			break;
		case 50:
			level = 3;
			break;
		case 25:
			level = 4;
			break;
		case 15:
			level = 5;
			break;
		default:
			return -1;
	}
#endif
	
	sprintf(powerlevel, "%lu", level);
	tcapi_set("WLan_Common","TxPowerLevel",powerlevel);
	
	SetCfgNodeFlag( "WLan", -1 );
	return 0;
}

int8 getWLANCfgPowerlevelAttribute(uint8* notification, AccessEntry** acl, DeviceNodePtr deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcWlanAttr.aPowerlevel;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	return result;
}

int8 setWLANCfgPowerlevelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNodePtr deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcWlanAttr.aPowerlevel;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWLANCfgPowerValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char powerValue[10]= {'\0'};

	strcpy(powerValue,"20");
		
	return getTheValue(value, type, deviceNode, powerValue);
}

int8 getWLANCfgPowerValueAttribute(uint8* notification, AccessEntry** acl, DeviceNodePtr deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcWlanAttr.aPowerValue;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	return result;
}

int8 setWLANCfgPowerValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNodePtr deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcWlanAttr.aPowerValue;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
#endif
#endif

int8 getWLANCfgStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char status_String[10] = {'\0'};
	char isExist_buf[10] = {'\0'};
	char active_buf[10] = {'\0'};
	
	tcapi_get("Info_WLan","isExist",isExist_buf);
	
	tcapi_get("WLan_Common","APOn",active_buf);

	if(strcmp(isExist_buf, "On") != 0 || strcmp(active_buf, "1") != 0){
		strcpy(status_String,"Disabled");
	}
	else{
		strcpy(status_String,"Up");
	}

	return getTheValue(value, type, deviceNode, status_String);
}
int8 getWLANCfgStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcWlanAttr.aWLanStatus[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
	
}
int8 setWLANCfgStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{

	uint8*  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aWLanStatus[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgBSSIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34];
	char macInfo[32];
	int instance;
	
	memset(nodeName, 0, sizeof(nodeName));
	memset(macInfo, 0, sizeof(macInfo));
	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "Info_ra%d", instance - 1);

	tcapi_get(nodeName, "hwaddr", macInfo);
	
	return getTheValue(value, type, deviceNode, macInfo);
}
int8 getWLANCfgBSSIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcWlanAttr.aWLanBSSID[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgBSSIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{

	uint8*  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aWLanBSSID[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
//how to support this node,we should now how to use txRate.
int8 getWLANCfgMaxBitRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34];
	char maxBitRate[5] = {'\0'};

	int instance;

	memset(nodeName, 0, sizeof(nodeName));
	memset(maxBitRate, 0, sizeof(maxBitRate));
	instance = atoi(deviceNode->parentNode->name);
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	tcapi_get(nodeName, "TXrate", maxBitRate); //add a attribute "Txrate" in WLan_Entry{i} node.
	return getTheValue(value, type, deviceNode, maxBitRate);
}

int8 setWLANCfgMaxBitRateValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34];
	int instance;
	int i;

	memset(nodeName, 0, sizeof(nodeName));	
	instance = atoi(deviceNode->parentNode->name);
	sprintf(nodeName, "WLan_Entry%d", instance - 1);
	
	SetCfgNodeFlag ( "WLan", instance - 1 );
	
	if(strlen((char*)value) > 4){
		tc_printf("==>setWLANCfgMaxBitRateValue: the length of value is too long.\n");
		return -1;
	}
	
	for(i = 0; i<=MAX_LEN_OF_SUPPORTED_RATES; i++){
		if(strcmp(gTransmitRatesList[i], (char*)value) == 0){
			tcapi_set(nodeName, "Txrate", (char*)value);
			return 0;
		}
	}
	return -1;
}
int8 getWLANCfgMaxBitRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanMaxBitRate[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgMaxBitRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanMaxBitRate[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgChannelValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char channel[10]= {'\0'};

	tcapi_get("WLan_Common","Channel",channel);
	
	return getTheValue(value, type, deviceNode, channel);	
}
int8 setWLANCfgChannelValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 channel;
	char wlanChannel[10] = {'\0'};
	uint32 channelStart = 0 , channelEnd = 13; 

	SetCfgNodeFlag( "WLan", -1 );
	
	if(strlen((char*)value) > 9){
		return -1;
	}
	
	channel = *((uint32*)value);
	if(channel < channelStart || channel > channelEnd){
		return -1;
	}
	
	sprintf(wlanChannel, "%lu", channel);
	tcapi_set("WLan_Common","Channel",wlanChannel);
	
	return 0;
}
int8 getWLANCfgChannelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanChannel[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgChannelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanChannel[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgAutoChannelEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char channel[10];
	char autoChannel[4];

	memset( channel, 0, sizeof(channel) );
	memset( autoChannel, 0, sizeof(autoChannel) );

	tcapi_get("WLan_Common","Channel",channel);
	if ( !strcmp(channel, "0") ) {
		strcpy( autoChannel, "1" );
	}
	else {
		strcpy( autoChannel, "0" );
	}
	
	return getTheValue(value, type, deviceNode, autoChannel);
}
int8 setWLANCfgAutoChannelEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 autoChannel;

	autoChannel = *((uint8*)value);

	if ( autoChannel == 1 ) {
		/* if the channel value is 0, means open Auto Channel function*/
		tcapi_set( "WLan_Common", "Channel", "0" );
	}

	SetCfgNodeFlag( "WLan", -1 );

	return 0;
}
int8 getWLANCfgAutoChannelEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanChannel[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgAutoChannelEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanChannel[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}


int8 getWLANCfgSSIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char wlanSsid[33] = {'\0'};
	char nodeName[34] = {'\0'};
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);


	tcapi_get(nodeName, "SSID", wlanSsid);
	return getTheValue(value, type, deviceNode, wlanSsid);
	
	
}
int8 setWLANCfgSSIDValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	int instance, len;
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	char *ssidValue = NULL;
#endif

	instance = atoi(deviceNode->parentNode->name);	
	
	SetCfgNodeFlag( "WLan", instance - 1 );
	
	len = strlen((char*)value);
	if(len > 32){

		return -1;
	}

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	if(instance == 1)
	{
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
		ssidValue = strstr((char*)value, "CU-");
#else
		ssidValue = strstr((char*)value, "ChinaNet-");
#endif
		if(ssidValue == NULL || ssidValue != (char*)value)
			return -1;
	}
#endif
	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);
	if(0 == len){
		tcapi_set(nodeName, "SSID", "Wireless");
	}
	else{
		tcapi_set(nodeName, "SSID", (char*)value); 
	}
	
	return 0;
}
int8 getWLANCfgSSIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanSSID[instance - 1];
	
#if defined(TCSUPPORT_CT)
	if (attribute & 0x01)
		attribute &= 0xfe;
	else
		attribute |= 0x1;
#endif

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWLANCfgSSIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanSSID[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

#if defined(TCSUPPORT_CT)
	if (*attribute & 0x01)
		*attribute &= 0xfe;
	else
		*attribute |= 0x1;
#endif

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgBeaconTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char wlanAuthMode[32] = {'\0'};
	char nodeName[34] = {'\0'};
	char beanconType[20] = {'\0'};
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	tcapi_get(nodeName, "AuthMode", wlanAuthMode);
	
	if(strcmp(wlanAuthMode, "WEP-64Bits") == 0 
		|| strcmp(wlanAuthMode, "WEP-128Bits") == 0){
		strcpy(beanconType,"Basic");
	}
	else if(strcmp(wlanAuthMode, "WPAPSK") == 0){
		strcpy(beanconType,"WPA");
	}
	else if(strcmp(wlanAuthMode, "WPA2PSK") == 0){
		strcpy(beanconType,"11i");
	}
	else if ( strcmp(wlanAuthMode, "WPAPSKWPA2PSK") == 0 ) {
		strcpy(beanconType, "WPAand11i");
	}
	else if(strcmp(wlanAuthMode, "OPEN") == 0){
		strcpy(beanconType,"None");
	}

	return getTheValue(value, type, deviceNode, beanconType);
}
int8 setWLANCfgBeaconTypeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char wlanEncType[20] = {'\0'};
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	
	SetCfgNodeFlag( "WLan", instance - 1 );
	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);	

	// Attention: for wlan config, we should call all tcpai_get() before tcapi_set()
	// the wlan_read() would sync data from register/proc file to config nodes
	tcapi_get(nodeName, "EncrypType", wlanEncType);
	
	if(strcmp((char*)value,"Basic") == 0){
		tcapi_set(nodeName, "AuthMode", "WEP-64Bits");
		if ( 0 == instance - 1 )
			tcapi_set(nodeName, "WPSConfMode", "0"); // close wps
	}
	else if(strcmp((char*)value, "WPA") == 0){
		tcapi_set(nodeName, "AuthMode", "WPAPSK");
		
//		tcapi_get(nodeName, "EncrypType", wlanEncType);
		
		if(strcmp(wlanEncType, "TKIP") != 0){
			tcapi_set(nodeName, "EncrypType", "AES");
		}
	}
	else if(strcmp((char*)value, "11i") == 0){
		tcapi_set(nodeName, "AuthMode", "WPA2PSK");
//		tcapi_get(nodeName, "EncrypType", wlanEncType);
		if(strcmp(wlanEncType, "TKIP") != 0){
			tcapi_set(nodeName, "EncrypType", "AES");
		}
	}
	else if ( strcmp((char*)value, "WPAand11i") == 0 ) {
		tcapi_set( nodeName, "AuthMode", "WPAPSKWPA2PSK" );
		tcapi_set( nodeName, "EncrypType", "TKIP" );
	}
	else if(strcmp((char*)value, "None") == 0){
		tcapi_set(nodeName, "AuthMode", "OPEN");
		tcapi_set(nodeName, "EncrypType", "NONE");
	}
	else{
		return -1;
	}

	if ( 0 == instance - 1 && ( 0 != strcmp((char*)value,"Basic") ) )
	{
		tcapi_set(nodeName, "WPSConfMode", "7"); // open wps
		tcapi_set(nodeName, "WPSMode", "1"); // default mode is PushButton
	}

	return 0;
}
int8 getWLANCfgBeaconTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanBeaconType[instance - 1];

#if defined(TCSUPPORT_CT)
	if (attribute & 0x01)
		attribute &= 0xfe;
	else
		attribute |= 0x1;
#endif

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgBeaconTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{

	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanBeaconType[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

#if defined(TCSUPPORT_CT)
	if (*attribute & 0x01)
		*attribute &= 0xfe;
	else
		*attribute |= 0x1;
#endif

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgMACAddressControlEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char macAddrEnable[10]= {'\0'};
	int instance;


	instance = atoi(deviceNode->parentNode->name);
	sprintf(nodeName, "WLan_Entry%d", instance - 1);
	tcapi_get(nodeName,"AccessPolicy",macAddrEnable);
	
	return getTheValue(value, type, deviceNode, macAddrEnable);
}
int8 setWLANCfgMACAddressControlEnabledValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	uint8 macAddrEnable;
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);

	SetCfgNodeFlag( "WLan", instance - 1);
	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);
	
	macAddrEnable = *((uint8*)value);
	if(macAddrEnable == 1){
		tcapi_set(nodeName, "AccessPolicy", "1");
	}
	else if(macAddrEnable == 0){
		tcapi_set(nodeName, "AccessPolicy", "0");
	}
	else{
		return -1;
	}
	
	return 0;
}
int8 getWLANCfgMACAddressControlEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanMACAddressControlEnabled[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgMACAddressControlEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanMACAddressControlEnabled[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgStandardValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char standard[10]= {'\0'};
	int stdValue;

	tcapi_get("WLan_Common","WirelessMode",standard);
	
	stdValue = atoi(standard);

	
#if defined(TCSUPPORT_CT_WLAN_NODE)  
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592)
	if ( stdValue == PHY_MODE_BG_MIXED ) {
		strcpy(standard, "b,g"); //b&g mode
	}
	else if ( stdValue == PHY_MODE_B_ONLY ) {
		strcpy(standard, "b");
	}
	else if ( stdValue == PHY_MODE_G_ONLY ) {
		strcpy(standard, "g only");
	}
	else if ( stdValue == PHY_MODE_N_ONLY ) {
		strcpy(standard, "n only");
	}else if ( stdValue == PHY_MODE_GN_MIXED ) {
		strcpy(standard, "g,n");
	}else if ( stdValue == PHY_MODE_BGN_MIXED ) {
		strcpy(standard, "b,g,n");
	}
#else
	if(stdValue == PHY_MODE_BG_MIXED){
		strcpy(standard, "b,g"); //b&g mode
	}
	else if(stdValue == PHY_MODE_B_ONLY){
		strcpy(standard, "b");
	}
	else if(stdValue == PHY_MODE_G_ONLY){
		strcpy(standard, "g only");
	}
#endif
#else
//#ifdef RT3390
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592)
	if ( stdValue == PHY_MODE_BG_MIXED ) {
		strcpy(standard, "g"); //b&g mode
	}
	else if ( stdValue == PHY_MODE_B_ONLY ) {
		strcpy(standard, "b");
	}
	else if ( stdValue == PHY_MODE_G_ONLY ) {
		strcpy(standard, "g only");
	}
	else if ( stdValue == PHY_MODE_N_ONLY || stdValue == PHY_MODE_GN_MIXED || stdValue == PHY_MODE_BGN_MIXED ) {
		strcpy(standard, "n");
	}
#else
	if(stdValue == PHY_MODE_BG_MIXED){
		strcpy(standard, "g"); //b&g mode
	}
	else if(stdValue == PHY_MODE_B_ONLY){
		strcpy(standard, "b");
	}
	else if(stdValue == PHY_MODE_G_ONLY){
		strcpy(standard, "g only");
	}
#endif
#endif

	return getTheValue(value, type, deviceNode, standard);	
}
#if defined(TCSUPPORT_CT_WLAN_NODE)
int8 setWLANCfgStandardValue(void* value, struct DeviceNode*  deviceNode){
	SetCfgNodeFlag("WLan", -1);
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592)
	if(!strcmp((char*)value,"b")){
		tcdbg_printf("b mode\n");
		tcapi_set("WLan_Common","WirelessMode","1");
	}
	else if(!strcmp((char*)value,"g")){
		tcapi_set("WLan_Common","WirelessMode","4");
	}
	else if(!strcmp((char*)value,"b,g")){
		tcapi_set("WLan_Common","WirelessMode","0");
	}
	else if(!strcmp((char*)value,"n")){
		tcapi_set("WLan_Common","WirelessMode","6");
	}
	else if(!strcmp((char*)value,"g,n")){
		tcapi_set("WLan_Common","WirelessMode","7");
	}
	else if(!strcmp((char*)value,"b,g,n")){
		tcapi_set("WLan_Common","WirelessMode","9");
	}
#else
	if(!strcmp((char*)value,"b")){
		tcdbg_printf("bbb mode\n");
		tcapi_set("WLan_Common","WirelessMode","1");
	}
	else if(!strcmp((char*)value,"g")){
		tcapi_set("WLan_Common","WirelessMode","2");
	}
	else if(!strcmp((char*)value,"b,g")){
		tcapi_set("WLan_Common","WirelessMode","0");
	}

#endif
	else{
		return -1;
	}
	return 0;
}
#endif

int8 getWLANCfgStandardAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{

	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanStandard[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgStandardAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{

	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanStandard[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgWEPKeyIndexValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char keyIndex[10] = {'\0'};
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);
	
	tcapi_get(nodeName, "DefaultKeyID", keyIndex);
	if(strlen(keyIndex)==0)
		strcpy(keyIndex,"1");


	return getTheValue(value, type, deviceNode, keyIndex);
	
}
int8 setWLANCfgWEPKeyIndexValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char keyStr[10] = {'\0'};
	uint32 keyIndex;
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	
	SetCfgNodeFlag( "WLan", instance - 1);
	
	keyIndex = *((uint32*)value);
	if(keyIndex < 1 || keyIndex > 4){
		return -1;
	}
	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	sprintf(keyStr, "%lu", keyIndex);
	
	tcapi_set(nodeName, "DefaultKeyID", keyStr);
	
	return 0;
}
int8 getWLANCfgWEPKeyIndexAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanWEPKeyIndex[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgWEPKeyIndexAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanWEPKeyIndex[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgWEPEncryptionLevelValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34];
	char wlanAuthMode[33];
	char Level[20];
	int instance;

	memset(nodeName, 0, sizeof(nodeName));
	memset(wlanAuthMode, 0, sizeof(wlanAuthMode));
	memset(Level, 0, sizeof(Level));
	
	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	tcapi_get(nodeName, "AuthMode", wlanAuthMode);

	if(strcmp(wlanAuthMode, "WEP-64Bits") == 0){
		strcpy(Level,"40-bit");
	}
	else if(strcmp(wlanAuthMode, "WEP-128Bits") == 0){
		strcpy(Level,"104-bit");
	}
	else{
		strcpy(Level, "Disabled");
	}

	return getTheValue(value, type, deviceNode, Level);
}
#if defined(TCSUPPORT_CT_WLAN_NODE)
int8 setWLANCfgWEPEncryptionLevelValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	if(strcmp((char*)value, "40-bit") == 0){
		tcapi_set(nodeName,"AuthMode","WEP-64Bits");
	}
	else if(strcmp((char*)value, "104-bit") == 0){
		tcapi_set(nodeName,"AuthMode","WEP-128Bits");
	}
	else{
		return -1;
	}

	SetCfgNodeFlag( "WLan", instance - 1);	
	return 0;
}
#endif

int8 getWLANCfgWEPEncryptionLevelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanWEPEncryptionLevel[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgWEPEncryptionLevelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanWEPEncryptionLevel[instance -1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWLANCfgBasicEncryptionModesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char wlanAuthMode[33] = {'\0'};
	char modes[20] = {"None"};
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	tcapi_get(nodeName, "AuthMode", wlanAuthMode);

	if(strcmp(wlanAuthMode, "WEP-64Bits") == 0
	 ||strcmp(wlanAuthMode, "WEP-128Bits") == 0){
		strcpy(modes,"WEPEncryption");
	}

	return getTheValue(value, type, deviceNode, modes);
}
int8 setWLANCfgBasicEncryptionModesValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	

	SetCfgNodeFlag( "WLan", instance - 1);
	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);
	
	if(strcmp((char*)value,"None") == 0){
		tcapi_set(nodeName, "AuthMode", "OPEN");
		tcapi_set(nodeName, "EncrypType", "NONE");
	}
	else if(strcmp((char*)value,"WEPEncryption") == 0){
		tcapi_set(nodeName, "AuthMode", "WEP-64Bits");
		tcapi_set(nodeName, "EncrypType", "NONE");
		tcapi_set(nodeName, "DefaultKeyID", "1");
	}
	else{
		return -1;
	}

	return 0;
}
int8 getWLANCfgBasicEncryptionModesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanBasicEncryptionModes[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgBasicEncryptionModesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanBasicEncryptionModes[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgBasicAuthenticationModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char wlanAuthMode[33] = {'\0'};
	char modes[20] = {'\0'};
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

#if defined(TCSUPPORT_CT_WLAN_NODE)
	tcapi_get(nodeName, "WEPAuthType", wlanAuthMode);
        strcpy(modes,wlanAuthMode);
#else
	tcapi_get(nodeName, "AuthMode", wlanAuthMode);

	if(strcmp(wlanAuthMode, "OPEN") == 0){
		strcpy(modes,"None");
	}
	else if(strcmp(wlanAuthMode, "WEP-64Bits") == 0
		 || strcmp(wlanAuthMode, "WEP-128Bits") == 0){
		strcpy(modes, "SharedAuthentication");
	}
#endif

	return getTheValue(value, type, deviceNode, modes);
}
int8 setWLANCfgBasicAuthenticationModeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
		
	SetCfgNodeFlag( "WLan", instance - 1);
	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

#if defined(TCSUPPORT_CT_WLAN_NODE)
	if(strcmp((char*)value, "OpenSystem") == 0
	|| strcmp((char*)value, "SharedKey") == 0
	|| strcmp((char*)value,"Both") == 0){
		tcapi_set(nodeName, "WEPAuthType", value);
	}
#else
	if(strcmp((char*)value, "None") == 0
	|| strcmp((char*)value, "Open") == 0){
		tcapi_set(nodeName, "AuthMode", "OPEN");
		tcapi_set(nodeName, "EncrypType", "NONE");
	}
	else if(strcmp((char*)value, "SharedAuthentication") == 0
		 || strcmp((char*)value, "Shared") == 0){
		tcapi_set(nodeName, "AuthMode", "WEP-64Bits");
		tcapi_set(nodeName, "EncrypType", "NONE");
		tcapi_set(nodeName, "DefaultKeyID", "1");
	}
#endif
	else{
		return -1;
	}

	return 0;
}
int8 getWLANCfgBasicAuthenticationModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanBasicAuthenticationMode[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgBasicAuthenticationModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanBasicAuthenticationMode[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWLANCfgWPAEncryptionModesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char wlanEncType[20] = {'\0'};
	char EncryptionMode[32] = {'\0'};
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	tcapi_get(nodeName, "EncrypType", wlanEncType);

	if(strcmp(wlanEncType, "TKIP") == 0){
		strcpy(EncryptionMode,"TKIPEncryption");
	}
	else if(strcmp(wlanEncType, "AES") == 0){
		strcpy(EncryptionMode,"AESEncryption");
	}
	else if ( strcmp(wlanEncType, "TKIPAES") == 0 ) {
		strcpy( EncryptionMode, "TKIPandAESEncryption" );
	}

	return getTheValue(value, type, deviceNode, EncryptionMode);
}
int8 setWLANCfgWPAEncryptionModesValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	if(strcmp((char*)value,"TKIPEncryption") == 0){
		tcapi_set(nodeName, "EncrypType", "TKIP");
	}
	else if(strcmp((char*)value,"AESEncryption") == 0){
		tcapi_set(nodeName, "EncrypType", "AES");
	}
	else if ( strcmp((char*)value, "TKIPandAESEncryption") == 0 ) {
		tcapi_set( nodeName, "EncrypType", "TKIPAES" );
	}
	else{
		return -1;
	}

	/* set cfg node flag here is more reasonable*/
	SetCfgNodeFlag( "WLan", instance - 1);
	
	return 0;
}
int8 getWLANCfgWPAEncryptionModesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanWPAEncryptionModes[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
	
}
int8 setWLANCfgWPAEncryptionModesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanWPAEncryptionModes[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgWPAAuthenticationModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char wlanAuthMode[33] = {'\0'};
	char authMode[32] = {'\0'};
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	tcapi_get(nodeName, "AuthMode", wlanAuthMode);
	
	if( strcmp(wlanAuthMode, "WPAPSK") == 0 || 
	    strcmp(wlanAuthMode, "WPA2PSK") == 0 ||
	    strcmp(wlanAuthMode, "WPAPSKWPA2PSK") == 0 ){
		strcpy(authMode,"PSKAuthentication");
	}
	
	return getTheValue(value, type, deviceNode, authMode);
}
int8 setWLANCfgWPAAuthenticationModeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char authMode[32] = {'\0'};
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	SetCfgNodeFlag( "WLan", instance - 1);
	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	if(strcmp((char*)value,"PSKAuthentication") == 0){
		tcapi_get(nodeName, "AuthMode", authMode);
		if( strcmp(authMode, "WPAPSK") == 0 ||
			strcmp(authMode, "WPA2PSK") == 0 ||
			strcmp(authMode, "WPAPSKWPA2PSK") == 0 ) {
			//do nothing, just return. it need do commit here.
			return 0;
		}
		else {
			tcapi_set(nodeName, "AuthMode", "WPAPSK");
		}
	}
	else{
		return -1;
	}
	
	return 0;
}
int8 getWLANCfgWPAAuthenticationModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanWPAAuthenticationMode[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgWPAAuthenticationModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{

	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanWPAAuthenticationMode[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgIEEE11iEncryptionModesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char wlanEncType[20] = {'\0'};
	char EncryptionMode[32] = {'\0'};
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	tcapi_get(nodeName, "EncrypType", wlanEncType);

	if(strcmp(wlanEncType, "TKIP") == 0){
		strcpy(EncryptionMode,"TKIPEncryption");
	}
	else if(strcmp(wlanEncType, "AES") == 0){
		strcpy(EncryptionMode,"AESEncryption");
	}
	else if ( strcmp(wlanEncType, "TKIPAES") == 0 ) {
		strcpy( EncryptionMode, "TKIPandAESEncryption" );
	}

	return getTheValue(value, type, deviceNode, EncryptionMode);
}
int8 setWLANCfgIEEE11iEncryptionModesValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	if(strcmp((char*)value,"TKIPEncryption") == 0){
		tcapi_set(nodeName, "EncrypType", "TKIP");
	}
	else if(strcmp((char*)value,"AESEncryption") == 0){
		tcapi_set(nodeName, "EncrypType", "AES");
	}
	else if ( strcmp((char*)value, "TKIPandAESEncryption") == 0 ) {
		tcapi_set( nodeName, "EncrypType", "TKIPAES" );
	}
	else{
		return -1;
	}

	SetCfgNodeFlag( "WLan", instance - 1);
	
	return 0;
}
int8 getWLANCfgIEEE11iEncryptionModesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanIEEE11iEncryptionModes[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgIEEE11iEncryptionModesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanIEEE11iEncryptionModes[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWLANCfgIEEE11iAuthenticationModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char wlanAuthMode[33] = {'\0'};
	char authMode[32] = {'\0'};
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	tcapi_get(nodeName, "AuthMode", wlanAuthMode);
	
	if( strcmp(wlanAuthMode, "WPAPSK") == 0 || 
	    strcmp(wlanAuthMode, "WPA2PSK") == 0 ||
	    strcmp(wlanAuthMode, "WPAPSKWPA2PSK") == 0 ){
		strcpy(authMode,"PSKAuthentication");
	}
	
	return getTheValue(value, type, deviceNode, authMode);
}
int8 setWLANCfgIEEE11iAuthenticationModeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char authMode[32] = {'\0'};
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	if(strcmp((char*)value,"PSKAuthentication") == 0){
		tcapi_get(nodeName, "AuthMode", authMode);
		if( strcmp(authMode, "WPAPSK") == 0 ||
			strcmp(authMode, "WPA2PSK") == 0 ||
			strcmp(authMode, "WPAPSKWPA2PSK") == 0 ) {
			//do nothing, just return. it need do commit here.
			return 0;
		}
		else {
			tcapi_set(nodeName, "AuthMode", "WPAPSK");
		}
	}
	else{
		return -1;
	}

	SetCfgNodeFlag( "WLan", instance - 1);
	return 0;
}
int8 getWLANCfgIEEE11iAuthenticationModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanIEEE11iAuthenticationMode[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgIEEE11iAuthenticationModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanIEEE11iAuthenticationMode[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWLANCfgPossibleChannelsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char PossibleChannels[64];

	memset(PossibleChannels, 0 ,sizeof(PossibleChannels));
	strcpy(PossibleChannels, "1-11,1-13,10-11,10-13,14,1-14,3-9");
	return getTheValue(value, type, deviceNode, PossibleChannels);
}
int8 getWLANCfgPossibleChannelsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanPossibleChannels[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgPossibleChannelsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanPossibleChannels[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgBasicDataTransmitRatesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char rate[64];
	char bgMode[10];
	unsigned char bgProtection;

	memset(rate, 0, sizeof(rate));
	memset(bgMode, 0, sizeof(bgMode));
	tcapi_get("WLan_Common", "WirelessMode", bgMode);
	bgProtection = atoi(bgMode);
	
	if(bgProtection == 0){//b&g mode
		strcpy(rate, gBasicDataTransmitRates[2]);
	}else if(bgProtection == 1){// b mode
		strcpy(rate, gBasicDataTransmitRates[0]);
	}else if(bgProtection == 2){//g mode
		strcpy(rate, gBasicDataTransmitRates[1]);
	}
	
	return getTheValue(value, type, deviceNode, rate);
}
int8 setWLANCfgBasicDataTransmitRatesValue(void* value, struct DeviceNode*  deviceNode)
{
	char bgMode[10];
	unsigned char bgProtection;

	if(strlen((char*)value) > 63)
		return -1;

	memset(bgMode, 0, sizeof(bgMode));
	tcapi_get("WLan_Common", "WirelessMode", bgMode);
	bgProtection = atoi(bgMode);
	
	if(bgProtection == 0){//b&g mode
		strcpy(gBasicDataTransmitRates[2],(char*)value);
	}else if(bgProtection == 1){// b mode
		strcpy(gBasicDataTransmitRates[0],(char*)value);
	}else if(bgProtection == 2){//g mode
		strcpy(gBasicDataTransmitRates[1],(char*)value);
	}

	return 0;
}
int8 getWLANCfgBasicDataTransmitRatesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanBasicDataTransmitRates[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgBasicDataTransmitRatesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanBasicDataTransmitRates[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgOperationalDataTransmitRatesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char rate[64];
	char bgMode[10];
	unsigned char bgProtection;

	memset(rate, 0, sizeof(rate));
	memset(bgMode, 0, sizeof(bgMode));
	tcapi_get("WLan_Common", "WirelessMode", bgMode);
	bgProtection = atoi(bgMode);
	
	if(bgProtection == 0){//b&g mode
		strcpy(rate, gOperationalDataTransmitRates[2]);
	}else if(bgProtection == 1){// b mode
		strcpy(rate, gOperationalDataTransmitRates[0]);
	}else if(bgProtection == 2){//g mode
		strcpy(rate, gOperationalDataTransmitRates[1]);
	}
	
	return getTheValue(value, type, deviceNode, rate);
}
int8 setWLANCfgOperationalDataTransmitRatesValue(void* value, struct DeviceNode*  deviceNode)
{
	char bgMode[10];
	unsigned char bgProtection;

	if(strlen((char*)value) > 63)
		return -1;

	memset(bgMode, 0, sizeof(bgMode));
	tcapi_get("WLan_Common", "WirelessMode", bgMode);
	bgProtection = atoi(bgMode);
	
	if(bgProtection == 0){//b&g mode
		strcpy(gOperationalDataTransmitRates[2],(char*)value);
	}else if(bgProtection == 1){// b mode
		strcpy(gOperationalDataTransmitRates[0],(char*)value);
	}else if(bgProtection == 2){//g mode
		strcpy(gOperationalDataTransmitRates[1],(char*)value);
	}
	return 0;
}
int8 getWLANCfgOperationalDataTransmitRatesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanOperationalDataTransmitRates[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgOperationalDataTransmitRatesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanOperationalDataTransmitRates[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgPossibleDataTransmitRatesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char rate[64];
	char bgMode[10];
	unsigned char bgProtection;

	memset(rate, 0, sizeof(rate));
	memset(bgMode, 0, sizeof(bgMode));
	tcapi_get("WLan_Common", "WirelessMode", bgMode);
	bgProtection = atoi(bgMode);
	
	if(bgProtection == 0){//b&g mode
		strcpy(rate, gPossibleDataTransmitRates[2]);
	}else if(bgProtection == 1){// b mode
		strcpy(rate, gPossibleDataTransmitRates[0]);
	}else if(bgProtection == 2){//g mode
		strcpy(rate, gPossibleDataTransmitRates[1]);
	}
	
	return getTheValue(value, type, deviceNode, rate);
	
}
int8 getWLANCfgPossibleDataTransmitRatesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanPossibleDataTransmitRates[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgPossibleDataTransmitRatesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanPossibleDataTransmitRates[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgRadioEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char radioEnable[10]= {'\0'};
	char nodeName[33] = {'\0'};
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);
	
	tcapi_get(nodeName, "HideSSID",radioEnable);

	
	return getTheValue(value, type, deviceNode, radioEnable);
}
int8 setWLANCfgRadioEnabledValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	uint8 radio;
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	
	SetCfgNodeFlag( "WLan", instance - 1);
	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	radio = *((uint8*)value);
	if(radio == 0){
		tcapi_set(nodeName, "HideSSID", "0");
	}
	else if(radio == 1){
		tcapi_set(nodeName, "HideSSID", "1");
	}
	else{
		return -1;
	}

	return 0;
}
int8 getWLANCfgRadioEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanRadioEnabled[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgRadioEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanRadioEnabled[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgAutoRateFallBackEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char flag[4] = {'\0'};
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	sprintf(flag ,"%d", AutoRateFallBackFlag[instance - 1]);
	
	return getTheValue(value, type, deviceNode, flag);
}
int8 setWLANCfgAutoRateFallBackEnabledValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 flag;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);	
	
	flag = *((uint8*)value);

	if(flag == 0 || flag == 1){
		AutoRateFallBackFlag[instance - 1] = flag;
	}
	else{
		return -1;
	}
	
	return 0;
}
int8 getWLANCfgAutoRateFallBackEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcWlanAttr.aWLanAutoRateFallBackEnabled[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgAutoRateFallBackEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aWLanAutoRateFallBackEnabled[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgTotalBytesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char nodeValue[10] = {"0"}; //set default value to 0.
	int instance;

	instance = atoi(deviceNode->parentNode->name);
	sprintf(nodeName, "Info_ra%d", instance - 1);

	tcapi_get(nodeName, "txbytes", nodeValue);
	
	return getTheValue(value, type, deviceNode, nodeValue);
}
int8 getWLANCfgTotalBytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcWlanAttr.aWLanTotalBytesSent[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgTotalBytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aWLanTotalBytesSent[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgTotalBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char nodeValue[10] = {"0"};
	int instance;

	instance = atoi(deviceNode->parentNode->name);
	sprintf(nodeName, "Info_ra%d", instance - 1);

	tcapi_get(nodeName, "rxbytes", nodeValue);
	
	return getTheValue(value, type, deviceNode, nodeValue);
}
int8 getWLANCfgTotalBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcWlanAttr.aWLanTotalBytesReceived[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgTotalBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aWLanTotalBytesReceived[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgTotalPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char nodeValue[10] = {"0"}; //set default value to 0
	int instance;

	instance = atoi(deviceNode->parentNode->name);
	sprintf(nodeName, "Info_ra%d", instance - 1);

	tcapi_get(nodeName, "txpackets", nodeValue);
	
	
	return getTheValue(value, type, deviceNode, nodeValue);
}
int8 getWLANCfgTotalPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcWlanAttr.aWLanTotalPacketsSent[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgTotalPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aWLanTotalPacketsSent[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgTotalPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char nodeValue[10] = {"0"}; //set default value to 0
	int instance;

	instance = atoi(deviceNode->parentNode->name);
	sprintf(nodeName, "Info_ra%d", instance - 1);

	tcapi_get(nodeName, "rxpackets", nodeValue);
	
	return getTheValue(value, type, deviceNode, nodeValue);
}
int8 getWLANCfgTotalPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcWlanAttr.aWLanTotalPacketsReceived[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgTotalPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aWLanTotalPacketsReceived[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getWLANCfgTotalAssociationsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char num[10] = {'\0'};
	//char index[10];
	int n = -1;

	int instance;
	
	char NUMAttr[8]={0};

	instance = atoi(deviceNode->parentNode->name);	

	//tcapi_set("wifiMacTab_Common", "flag", "1");
	//sprintf(index, "%d", instance - 1);
	//tcapi_set("wifiMacTab_Common", "index", index);
	sprintf(NUMAttr, "NUM%d", instance - 1);
	tcapi_get("wifiMacTab_Common", NUMAttr, num);
	n = atoi(num);

	if(n < 0 || n > 64)
		sprintf(num, "%d", 0);
	
	return getTheValue(value, type, deviceNode, num);

}
int8 getWLANCfgTotalAssociationsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcWlanAttr.aWLanTotalAssociations[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgTotalAssociationsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aWLanTotalAssociations[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getAssociatedDeviceMACAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	//char index[4] = {'\0'};
	char macAddr[30] = {'\0'};
	int ssidIndex = 0;
	int instance = 0;
	char MACAttr[8]={0};
	
	ssidIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	instance = atoi(deviceNode->parentNode->name) - 1;

	//sprintf(index, "%d", ssidIndex);
	//tcapi_set("wifiMacTab_Common", "flag", "1"); //it means we want to get mac table data.
	//tcapi_set("wifiMacTab_Common", "index", index);
	sprintf(MACAttr, "MAC%d", ssidIndex);

	sprintf(nodeName, "wifiMacTab_Entry%d", instance);
	tcapi_get(nodeName, MACAttr, macAddr);
	
	return getTheValue(value, type, deviceNode, macAddr);	
}

int8 getAssociatedDeviceMACAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	attribute = tcWlanAttr.aWLanAssociatedDevMACAddr[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}

int8 setAssociatedDeviceMACAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	attribute = &tcWlanAttr.aWLanAssociatedDevMACAddr[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;

}

int8 getAssociatedDeviceIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	//char index[4] = {'\0'};
	char ipAddr[30] = {'\0'};
	int instance = 0;
	int ssidIndex = 0;	
	char IPAttr[8]={0};

	ssidIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	instance = atoi(deviceNode->parentNode->name) - 1;

	//sprintf(index, "%d", ssidIndex);
	//tcapi_set("wifiMacTab_Common", "flag", "1"); //it means we want to get mac table info.
	//tcapi_set("wifiMacTab_Common", "index", index);

	sprintf(IPAttr, "IP%d", ssidIndex);	
	sprintf(nodeName, "wifiMacTab_Entry%d", instance);
	tcapi_get(nodeName, IPAttr, ipAddr);
	
	return getTheValue(value, type, deviceNode, ipAddr);
}

int8 getAssociatedDeviceIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	attribute = tcWlanAttr.aWLanAssociatedDevIPAddr[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;

}

int8 setAssociatedDeviceIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	attribute = &tcWlanAttr.aWLanAssociatedDevIPAddr[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;

}

int8 getAssociatedDeviceAuthenticationStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{

	char temp[4] = {'\0'};

	strcpy(temp, "1");
	return getTheValue(value, type, deviceNode, temp);	
}

int8 getAssociatedDeviceAuthenticationStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{

	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	attribute = tcWlanAttr.aWLanAssociatedDevAuthState[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
}

int8 setAssociatedDeviceAuthenticationStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){

	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	attribute = &tcWlanAttr.aWLanAssociatedDevAuthState[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWEPKeyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	/*it should return empty according to spe TR098*/
	char key[128];

	memset(key, 0, sizeof(key));
	return getTheValue(value, type, deviceNode, key);	
}
int8 setWEPKeyValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char keyStr[20] = {'\0'};
	char *keyvalue = NULL;
	char c1, c2;
	int instance;
      int	instanceOfSSID;
	int len;
	int i;
	int nCnt, rtnCode = 0;
	unsigned char temp[64] = {0};

	instanceOfSSID = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	if(instanceOfSSID < 1 || instanceOfSSID > 4)
		return -1;

	instanceOfSSID -= 1; // MBSSID range from 0 to 3

	SetCfgNodeFlag( "WLan", instanceOfSSID );
	
	sprintf(nodeName, "WLan_Entry%d", instanceOfSSID);


	
	if(value == NULL){
		return -1;
	}

	keyvalue = (char *)value;
	len = strlen(keyvalue);
	keyvalue = (char*)os_malloc(len + 1);
	memset(keyvalue, 0, (len+1));
	strncpy(keyvalue, (char *)value, len);
	switch(len)
	{
		case 0:
			tc_printf("\r\nsetWEPKeyValue key is invalid!!");
			free(keyvalue);
			return -1;
		case 5:
		case 13:
			keyvalue[len] = '\0';
			break;
		case 12:
		case 28:
			c1 = *keyvalue;
			c2 = *(keyvalue+1);

			if ( (c1 != '0') || ( (c2 != 'x') && (c2 != 'X') ) ){
				tc_printf("\r\nsetWEPKeyValue error!Should be preceded by the characters '0x'.!!");
				free(keyvalue);
				return -1;
			}
			for(i = 0; i < len-2; i++){
				keyvalue[i] = keyvalue[i+2];
			}
			keyvalue[len-2]='\0';
		case 10:
		case 26:
			if ( (len == 12) || (len == 10) )
				nCnt = 10;
			else
				nCnt = 26;
			for(i = 0;i <nCnt ;i++){
				if(RpHex2dec(keyvalue[i]) < 0){
					tc_printf("\r\nsetWEPKeyValue error!Only characters in the range of 'a-f', 'A-F', '0-9' are allowed.!!");
					free(keyvalue);
					return -1;
				}

			}
			keyvalue[nCnt] = '\0';
			break;
		default:
				tc_printf("\r\nsetWEPKeyValue key is invalid!!");
				free(keyvalue);
				return -1;
		
	}
	len = strlen(keyvalue);

	instance = atoi(deviceNode->parentNode->name);
	/*change authmode according to wepkey.*/
	if (len == 5 || len == 10) {
		tcapi_set(nodeName, "AuthMode", "WEP-64Bits");
		
	}
	else if (len == 13 || len == 26) {
		tcapi_set(nodeName, "AuthMode", "WEP-128Bits");
	}
	
	/*set this wepkey to default key*/
	sprintf(temp, "%d", instance);
	tcapi_set(nodeName, "DefaultKeyID", temp);

	memset(temp, 0, sizeof(temp));
	strcpy(temp, keyvalue);

	/*clear all wepkeys*/
	tcapi_set(nodeName, "Key1Str", "");
	tcapi_set(nodeName, "Key2Str", "");
	tcapi_set(nodeName, "Key3Str", "");
	tcapi_set(nodeName, "Key4Str", "");

	/*set the specific wepkey*/
	if(instance >= 1 && instance <= 4){
		sprintf(keyStr, "Key%dStr", instance);
		tcapi_set(nodeName, keyStr, temp);
	}
	else{
		return -1;
	}

	free(keyvalue);
	return rtnCode;	
	
}
int8 getWEPKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWEPKey[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWEPKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWEPKey[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getPreSharedKeyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	/*it should return empty according to spe TR098*/
	char tempPreSharedKey[64];

	memset(tempPreSharedKey, 0, sizeof(tempPreSharedKey));
	return getTheValue(value, type, deviceNode, tempPreSharedKey);
}
int8 setPreSharedKeyValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char *key = NULL;
	int instance;
      int	instanceOfSSID;
	int len;
	int rtnCode = 0;

	instanceOfSSID = atoi(deviceNode->parentNode->parentNode->parentNode->name);

	SetCfgNodeFlag( "WLan", instanceOfSSID - 1);
	
	sprintf(nodeName, "WLan_Entry%d", instanceOfSSID - 1);

	if(value == NULL){
		return -1;
	}

	key = (char*)value;
	while(*key == kAscii_Space){//Ignore the space.
		key++;
	}
	len = strlen(key);


	if(len < 8 || len > 63){//we only support 8~63 characters.
		return -1;
	}
	
	instance = atoi(deviceNode->parentNode->name);

	if(instance == 1){
		tcapi_set(nodeName, "WPAPSK", (char*)value);
	}else{
		//if the instance is not 1, do nothing.
		rtnCode = 0;
	}
	
	return rtnCode;

}
int8 getPreSharedKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanPreSharedKey[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setPreSharedKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanPreSharedKey[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int  cwmpInitAssociatedDeviceTable( DeviceNodePtr deviceNode)
{
	int i, instance, totalAssociations, rtnCode = 0;
	//char ssidIndex[10];
	char totalEntrys[10];

	char NUMAttr[8]={0};

	//memset( ssidIndex, 0, sizeof(ssidIndex) );
	memset( totalEntrys, 0, sizeof(totalEntrys) );

	/*get current ssid index*/
	
	instance = atoi( deviceNode->parentNode->name );

	/*set flag to 1 if we want to get wifi mac table info*/
	
	//tcapi_set( "wifiMacTab_Common", "flag", "1" );
	
	//sprintf( ssidIndex, "%d", instance - 1 );

	/*set index to ssidIndex if we want to get wifi mac table info for special ssid*/
	
	//tcapi_set( "wifiMacTab_Common", "index", ssidIndex );

	/*get wifi mac table entry num for sepcial ssid index*/
	sprintf( NUMAttr, "NUM%d", instance - 1 );
	tcapi_get( "wifiMacTab_Common", NUMAttr, totalEntrys );

	totalAssociations = atoi( totalEntrys );

	rtnCode = RegisterDynNodeFromIndex( deviceNode, AssociatedDeviceIndex, totalAssociations );

	return rtnCode;
}

int  cwmpInitWEPKeyTable( DeviceNodePtr deviceNode )
{
	int num = 4, rtnCode = 0;

	rtnCode = RegisterDynNodeFromIndex( deviceNode, WEPKeyeIndex, num );

	return rtnCode;
}

int  cwmpInitPreSharedKey( DeviceNodePtr deviceNode )
{
	int num = 10, rtnCode = 0;

	rtnCode = RegisterDynNodeFromIndex( deviceNode, PreSharedKeyIndex, num );

	return rtnCode;
}
#ifdef WSC_AP_SUPPORT
int8 getWPSEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[4];

	memset( tempValue, 0, sizeof(tempValue) );
	
	tcapi_get( "WLan_Entry0", "WPSConfMode", tempValue );

	if ( !strcmp(tempValue, WSC_DISABLE) || !strlen(tempValue) ) {
		strcpy( tempValue, "0" );
	}
	else {
		strcpy( tempValue, "1" );
	}

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setWPSEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	boolean tempEnable;

	tempEnable = *(boolean*)(value);

	if ( tempEnable == 0 ) {
		tcapi_set( "WLan_Entry0", "WPSConfMode", "0" );
	}
	else if ( tempEnable == 1 ) {
		tcapi_set( "WLan_Entry0", "WPSConfMode", "7" );
		
		/*default mode is PushButton*/
		tcapi_set( "WLan_Entry0", "WPSMode", "1" );
	}
	else {
		return -1;
	}

	//SetCfgNodeFlag( "WLan", 0 );

	return 0;
}
int8 getWPSEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;

	attribute = tcWlanAttr.aWPSEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWPSEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcWlanAttr.aWPSEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWPSDeviceNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char info[33];

	memset( info, 0, sizeof(info) );
#if defined(RT3390) || defined(RT5392) || defined(MT7592)
	getWpsInfo( "get_wps_dyn_info", "DeviceName = ", info, sizeof(info) );
#endif
	return getTheValue(value, type, deviceNode, info);
}
int8 getWPSDeviceNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;

	attribute = tcWlanAttr.aWPSDeviceName;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWPSDeviceNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcWlanAttr.aWPSDeviceName;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWPSDevicePasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[4];

	memset( tempValue, 0, sizeof(tempValue) );

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setWPSDevicePasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 pDevicePwd;
	char pwd[16];

	memset( pwd, 0, sizeof(pwd) );

	pDevicePwd = *(uint32*)(value); 
	if ( (pDevicePwd < 10000000) || (pDevicePwd > 99999999) ) {
		tc_printf( "\r\nPinCode length must be 8!\r\n" );
		return -1;
	}

	if ( ValidateChecksum(pDevicePwd) == FALSE ){
		tc_printf( "\r\nPinCode InValid!\r\n" );
		return -1;
	}

	sprintf( pwd, "%lu", pDevicePwd );

	tcapi_set( "WLan_Entry0", "enrolleePinCode", pwd );

	//SetCfgNodeFlag( "WLan", 0 );

	return 0;
}
int8 getWPSDevicePasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;

	attribute = tcWlanAttr.aWPSDevicePassword;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWPSDevicePasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcWlanAttr.aWPSDevicePassword;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWPSUUIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char info[37];

	memset( info, 0, sizeof(info) );
#if defined(RT3390) || defined(RT5392) || defined(MT7592)
	getWpsInfo( "get_wps_dyn_info", "UUID = ", info, sizeof(info) );
#endif
	return getTheValue(value, type, deviceNode, info);
}
int8 getWPSUUIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;

	attribute = tcWlanAttr.aWPSUUID;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWPSUUIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcWlanAttr.aWPSUUID;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWPSVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char info[64];

	memset( info, 0, sizeof(info) );
#if defined(RT3390) || defined(RT5392) || defined(MT7592)
	getWpsInfo( "get_wps_dyn_info", "wpsVersion = ", info, sizeof(info) );

	/*the version is 0x10 in driver, so here we set it to 16 directly*/
	strcpy( info, "16" );
#endif
	
	return getTheValue(value, type, deviceNode, info);
}
int8 getWPSVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;

	attribute = tcWlanAttr.aWPSVersion;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWPSVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcWlanAttr.aWPSVersion;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWPSConfigMethodsSupportedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	/*merge from linos*/
	char tempValue[128] = "Ethernet,Lable,Display,PushButton";	
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getWPSConfigMethodsSupportedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;

	attribute = tcWlanAttr.aWPSConfigMethodsSupported;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWPSConfigMethodsSupportedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcWlanAttr.aWPSConfigMethodsSupported;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWPSConfigMethodsEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if 0
	char info[10];
	char tempValue[128];
	USHORT	ConfigMethods;
	int strLen = 0;

	memset( info, 0, sizeof(info) );
	memset( tempValue, 0, sizeof(tempValue) );
#ifdef RT3390
	getWpsInfo( "get_wps_dyn_info", "ConfigMethods = ", info, sizeof(info) );
#endif
	ConfigMethods = atoi( info );

	if(ConfigMethods & 0x0001){
		strLen += sprintf(tempValue + strLen, "%s,", "USBFlashDrive");
	}
	if(ConfigMethods & 0x0002){
		strLen += sprintf(tempValue + strLen, "%s,", "Ethernet");
	}
	if(ConfigMethods & 0x0004){
		strLen += sprintf(tempValue + strLen, "%s,", "Lable");
	}
	if(ConfigMethods & 0x0008){
		strLen += sprintf(tempValue + strLen, "%s,", "Display");
	}
	if(ConfigMethods & 0x0010){
		strLen += sprintf(tempValue + strLen, "%s,", "ExternalNFCToken");
	}
	if(ConfigMethods & 0x0020){
		strLen += sprintf(tempValue + strLen, "%s,", "IntergratedNFCToken");
	}
	if(ConfigMethods & 0x0040){
		strLen += sprintf(tempValue + strLen, "%s,", "NFCInterface");
	}
	if(ConfigMethods & 0x0080){
		strLen += sprintf(tempValue + strLen, "%s,", "PushButton");
	}
	if(ConfigMethods & 0x0100){
		//notes: when you enhance the config mothod, please expand the array
		if((strLen + strlen("KeyPad")) < sizeof(tempValue) ){
			strLen += sprintf(tempValue + strLen, "%s,", "KeyPad");
		}
		else{
			tc_printf( "\r\narray lenth is not enough!" );
		}
	}
	if(strLen > 0){ // remove the ',' at the end
		tempValue[strLen - 1] = '\0';
	}

	return getTheValue(value, type, deviceNode, tempValue);
#endif
	char wpsMode[10];

	memset( wpsMode, 0, sizeof(wpsMode) );

#if defined(RT3390) || defined(RT5392) || defined(MT7592)
	tcapi_get( "WLan_Entry0", "WPSMode", wpsMode );
	if ( !strcmp(wpsMode, "0") ) {
		strcpy( wpsMode, "Lable" );
	}
	else if ( !strcmp(wpsMode, "1") ) {
		strcpy( wpsMode, "PushButton" );
	}
	else {
		strcpy( wpsMode, "" );
	}
#endif

	return getTheValue(value, type, deviceNode, wpsMode);
	
}
int8 setWPSConfigMethodsEnabledValue(void* value, struct DeviceNode*  deviceNode)
{
#if 0
	return 0;
#else
	char wpsMode[10];
	memset( wpsMode, 0, sizeof(wpsMode) );

	strncpy( wpsMode, (char*)value, sizeof(wpsMode) - 1 );
	
	if ( !strcmp( wpsMode, "Lable" ) ) {
		/*pin code*/
		tcapi_set( "WLan_Entry0", "WPSMode", "0" );
	}
	else if( !strcmp( wpsMode, "PushButton" ) ) {
		/*push button*/
		tcapi_set( "WLan_Entry0", "WPSMode", "1" );
	}
	else {
		return -1;
	}

	//SetCfgNodeFlag( "WLan", -1 );

	return 0;
#endif
}
int8 getWPSConfigMethodsEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;

	attribute = tcWlanAttr.aWPSConfigMethodsEnabled;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWPSConfigMethodsEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcWlanAttr.aWPSConfigMethodsEnabled;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWPSSetupLockedStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[12] = "Unlocked";	
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getWPSSetupLockedStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;

	attribute = tcWlanAttr.aWPSSetupLockedState;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWPSSetupLockedStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcWlanAttr.aWPSSetupLockedState;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWPSSetupLockValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[4] = "0";	
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setWPSSetupLockValue(void* value, struct DeviceNode*  deviceNode)
{
	return 0;
}
int8 getWPSSetupLockAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;

	attribute = tcWlanAttr.aWPSSetupLock;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWPSSetupLockAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcWlanAttr.aWPSSetupLock;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWPSConfigurationStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[16];

	memset( tempValue, 0, sizeof(tempValue) );

	tcapi_get( "WLan_Entry0", "WPSConfStatus", tempValue );

	if ( !strcmp(tempValue, "2") ) {
		strcpy( tempValue, "Configured" );
	}
	else {
		strcpy( tempValue, "Not Configured" );
	}

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getWPSConfigurationStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;

	attribute = tcWlanAttr.aWPSConfigurationState;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWPSConfigurationStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcWlanAttr.aWPSConfigurationState;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWPSLastConfigurationErrorValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = "NoError";	
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getWPSLastConfigurationErrorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;

	attribute = tcWlanAttr.aWPSLastConfigurationError;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWPSLastConfigurationErrorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcWlanAttr.aWPSLastConfigurationError;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWPSRegistrarNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[4] = "1";	//return 1, AP with Registrar
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getWPSRegistrarNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;

	attribute = tcWlanAttr.aWPSRegistrarNumberOfEntries;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWPSRegistrarNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcWlanAttr.aWPSRegistrarNumberOfEntries;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWPSRegistrarEstablishedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[4] = "1";	//return true
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getWPSRegistrarEstablishedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;

	attribute = tcWlanAttr.aWPSRegistrarEstablished;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWPSRegistrarEstablishedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcWlanAttr.aWPSRegistrarEstablished;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int cwmpInitWPSRegistrar( DeviceNodePtr deviceNode )
{
	int rtnCode = -1, regNum = 1;

	rtnCode = RegisterDynNodeFromIndex( deviceNode, RegistrarIndex, regNum );

	return rtnCode;
}

int8 getRegistrarEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[4];

	memset( tempValue, 0, sizeof(tempValue) );
	
	tcapi_get( "WLan_Entry0", "WPSConfMode", tempValue );

	if ( !strcmp(tempValue, WSC_DISABLE) || !strlen(tempValue) ) {
		strcpy( tempValue, "0" );
	}
	else {
		strcpy( tempValue, "1" );
	}

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setRegistrarEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	boolean tempEnable;

	tempEnable = *(boolean*)(value);

	if ( tempEnable == 0 ) {
		tcapi_set( "WLan_Entry0", "WPSConfMode", "0" );
	}
	else if ( tempEnable == 1 ) {
		tcapi_set( "WLan_Entry0", "WPSConfMode", "7" );
		
		/*default mode is PushButton*/
		tcapi_set( "WLan_Entry0", "WPSMode", "1" );
	}
	else {
		return -1;
	}

	//SetCfgNodeFlag( "WLan", 0 );

	return 0;
}
int8 getRegistrarEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcWlanAttr.aWPSRegistrarEnable[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setRegistrarEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aWPSRegistrarEnable[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getRegistrarUUIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char info[37];

	memset( info, 0, sizeof(info) );
	
#if defined(RT3390) || defined(RT5392) || defined(MT7592) || defined(MT7592)
	getWpsInfo( "get_wps_regs_dyn_info", "UUID_R = ", info, sizeof(info) );
#endif

	return getTheValue(value, type, deviceNode, info);   
}
int8 getRegistrarUUIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcWlanAttr.aWPSRegistrarUUID[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setRegistrarUUIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aWPSRegistrarUUID[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getRegistrarDeviceNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char info[33];

	memset( info, 0, sizeof(info) );
#if defined(RT3390) || defined(RT5392)
	getWpsInfo( "get_wps_regs_dyn_info", "DeviceName = ", info, sizeof(info) );
#endif
	
	return getTheValue(value, type, deviceNode, info);   
}
int8 getRegistrarDeviceNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcWlanAttr.aWPSRegistrarDeviceName[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setRegistrarDeviceNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aWPSRegistrarDeviceName[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

#if defined(RT3390) || defined(RT5392) || defined(MT7592)
/*
** (1) Get info from "iwpriv ra0 ar9_show"
** (2) Info include DeviceName / UUID etc.
** (3) The format of info is "[keyword][value][\n]"
** (4) eg: 
**		getWpsInfo( "get_wps_regs_dyn_info", "DeviceName = ", info, sizeof(info) );
*/
void getWpsInfo( char *ioctlCmd,  /*ioctl cmd like get_wps_regs_dyn_info*/
				 char *keyword,   /*get value accoding to this keyword like DeviceName = */
				 char *info,       /*return value*/
				 int infoLen	  /*the length of info*/
			   )
{
	char cmd[128];
	char buf[128];
	char *pStrStart= NULL, *pStrEnd = NULL;
	FILE *fp = NULL;
	int keywordLen;

	/*check input parameters*/
	if (ioctlCmd == NULL || keyword == NULL || info == NULL ) {
		return;
	}

	keywordLen = strlen( keyword );
	
	memset( cmd, 0, sizeof(cmd) );
	memset( buf, 0, sizeof(buf) );

	/* 
	** Output the result of iwpriv cmd to a file, 
	** then parser this file to find the value of the keyword.
	*/
	sprintf( cmd, "/userfs/bin/iwpriv ra0 ar9_show %s > %s", ioctlCmd, WPS_FILE );
	
	system( cmd );
	
	if( (fp = fopen(WPS_FILE, "r" )) == NULL ) {
		tcdbg_printf( "==> getRegistrarUUIDValue(): can not open file\n" );
		unlink( WPS_FILE );
		return;
	}
	
	while ( fgets(buf, sizeof(buf), fp) != NULL ) {
		
		/*find the keyword in this buf*/
		pStrStart = strstr( buf, keyword );
		if ( pStrStart == NULL ) {
			memset( buf, 0, sizeof(buf) );
			continue;
		}
		
		/*the value is end of '\n', in general, it will not happen.*/
		pStrEnd = strchr( pStrStart, '\n' );
		if ( pStrEnd == NULL ) {
			fclose(fp);
			unlink( WPS_FILE );
			return;
		}

		/* if the lenth is more than info,just return, or tr69 will crash*/
		if ( (pStrEnd - (pStrStart + keywordLen)) > infoLen -1  ) {
			fclose(fp);
			unlink( WPS_FILE );
			return;
		}
		
		/*copy the value of this keyword to info*/
		strncpy( info, pStrStart + keywordLen, (pStrEnd - (pStrStart + keywordLen)) );
		
		info[ (pStrEnd - (pStrStart + keywordLen)) ] = '\0';
		memset( buf, 0, sizeof(buf) );
	}
	
	fclose(fp);
	unlink( WPS_FILE );

	return;
}
#endif

#endif

#if defined(TCSUPPORT_CT_WLAN_NODE)
#if !defined(TCSUPPORT_C1_CUC)
int8 getWLANCfgSSIDHideValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char wlanSsidHide[10] = {'\0'};
	char nodeName[34] = {'\0'};
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);
	tcapi_get(nodeName, "HideSSID", wlanSsidHide);

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	if(atoi(wlanSsidHide)==1)
		strcpy(wlanSsidHide,"0");
	else
		strcpy(wlanSsidHide,"1");
#endif

	return getTheValue(value, type, deviceNode, wlanSsidHide);	
}
int8 setWLANCfgSSIDHideValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 flag;
	char nodeName[34] = {'\0'};
	int instance, len;

	flag = *((uint8*)value);
	
	instance = atoi(deviceNode->parentNode->name);	
	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);
	
	if(1 == flag){
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
		tcapi_set(nodeName, "HideSSID", "1");
#else
		tcapi_set(nodeName, "HideSSID", "0"); 
#endif
	}
	else if(0 == flag){
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
		tcapi_set(nodeName, "HideSSID", "0"); 
#else
		tcapi_set(nodeName, "HideSSID", "1"); 
#endif
		
	}
	else{
		tc_printf("==>setWLANCfgSSIDHideValue:value should be 0 or 1. \n");
		return -1;
	}
	SetCfgNodeFlag( "WLan", instance - 1 );
	return 0;
}
int8 getWLANCfgSSIDHideAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aWLanSSIDHiden[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWLANCfgSSIDHideAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aWLanSSIDHiden[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
#endif

int8 getWLANCfgChannelsInUseValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char channel[10]= {'\0'};
	
	tcapi_get("Info_WLan","CurrentChannel",channel);
	
	return getTheValue(value, type, deviceNode, channel);		
}
int8 getWLANCfgChannelsInUseAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcWlanAttr.aChannelInUse;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	return result;
}

int8 setWLANCfgChannelsInUseAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcWlanAttr.aChannelInUse;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getWLANCfgRFBandValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	uint8 instance = 0;
	char rfBand[8] = {0};

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode )
		return -1;

	instance = atoi(deviceNode->parentNode->name) - 1;
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	strcpy(rfBand, "0");
#else
	strcpy(rfBand, "2.4GHz");
#endif
	
	return getTheValue(value, type, deviceNode, rfBand);
}
int8 setWLANCfgRFBandValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	uint8 instance = 0;
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	uint32 rfBand = *((uint32*)value);

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode )
		return -1;

	instance = atoi(deviceNode->parentNode->name) - 1;

	switch ( rfBand )
	{
		case 0:
			break;
		default:
			return -1;
	}
#else
	if(strcmp("2.4GHz",(char*)value))
		return -1;
#endif

	//SetCfgNodeFlag( "WLan", instance - 1);

	return 0;
}
int8 getWLANCfgRFBandAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	uint8 instance = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode )
		return -1;

	instance = atoi(deviceNode->parentNode->name) - 1;

	attribute = tcWlanAttr.aRFBand[instance];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgRFBandAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	uint8 instance = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode )
		return -1;

	instance = atoi(deviceNode->parentNode->name) - 1;

	attribute = &tcWlanAttr.aRFBand[instance];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWLANCfgChannelWidthValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	uint8 instance = 0;
	char channelwidth[4] = "0";
	char coexistence[4] = "0";

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode )
		return -1;

	instance = atoi(deviceNode->parentNode->name) - 1;

	tcapi_get( "WLan_Common", "HT_BW", channelwidth );
	tcapi_get( "WLan_Common", "HT_BSSCoexistence", coexistence );
	if ( 1 == atoi(channelwidth) && 1 == atoi(coexistence) )
		strcpy(channelwidth, "2");

	return getTheValue(value, type, deviceNode, channelwidth);
}
int8 setWLANCfgChannelWidthValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	uint8 instance = 0;
	char channelwidthStr[4] = "0";
	char coexistenceStr[4] = "0";
	uint32 channelwidth = *((uint32*)value);

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode )
		return -1;

	instance = atoi(deviceNode->parentNode->name) - 1;

	switch ( channelwidth )
	{
		case 0: // 20MHZ
			strcpy(channelwidthStr, "0");
			break;
		case 1: // 40MHZ
			strcpy(channelwidthStr, "1");
			strcpy(coexistenceStr, "0");
			break;
		case 2: // 20/40MHZ
			strcpy(channelwidthStr, "1");
			strcpy(coexistenceStr, "1");
			break;
		default:
			return -1;
	}
	tcapi_set( "WLan_Common", "HT_BW", channelwidthStr );
	tcapi_set( "WLan_Common", "HT_BSSCoexistence", coexistenceStr );
	SetCfgNodeFlag( "WLan", instance - 1);

	return 0;
}
int8 getWLANCfgChannelWidthAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	uint8 instance = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode )
		return -1;

	instance = atoi(deviceNode->parentNode->name) - 1;

	attribute = tcWlanAttr.aChannelWidth[instance];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgChannelWidthAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	uint8 instance = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode )
		return -1;

	instance = atoi(deviceNode->parentNode->name) - 1;

	attribute = &tcWlanAttr.aChannelWidth[instance];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWLANCfgGuardIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	uint8 instance = 0;
	char interval[4] = {0};
	char intval[4] = "1";

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode )
		return -1;

	instance = atoi(deviceNode->parentNode->name) - 1;
	tcapi_get( "WLan_Common", "HT_GI", intval );
	if ( 0 == strcmp(intval, "0") )
		strcpy(interval, "1");
	else
		strcpy(interval, "0");

	return getTheValue(value, type, deviceNode, interval);
}
int8 setWLANCfgGuardIntervalValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	uint8 instance = 0;
	char intervalStr[4] = "0";
	uint32 interval = *((uint32*)value);

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode )
		return -1;

	instance = atoi(deviceNode->parentNode->name) - 1;

	switch ( interval )
	{
		case 0:
			strcpy(intervalStr, "1");
			break;
		case 1:
			strcpy(intervalStr, "0");
			break;
		default:
			return -1;
	}
	tcapi_set( "WLan_Common", "HT_GI", intervalStr );
	SetCfgNodeFlag( "WLan", instance - 1);

	return 0;
}
int8 getWLANCfgGuardIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;
	uint8 instance = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode )
		return -1;

	instance = atoi(deviceNode->parentNode->name) - 1;

	attribute = tcWlanAttr.aGuardInterval[instance];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgGuardIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	uint8 instance = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode )
		return -1;

	instance = atoi(deviceNode->parentNode->name) - 1;

	attribute = &tcWlanAttr.aGuardInterval[instance];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWLANCfgRetryTimeoutValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	uint8 instance = 0;
	char retryvalue[16] = "20";

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode )
		return -1;

	instance = atoi(deviceNode->parentNode->name) - 1;

	tcapi_get( "WLan_Common", "RetryTimeout", retryvalue );

	return getTheValue(value, type, deviceNode, retryvalue);
}
int8 setWLANCfgRetryTimeoutValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	uint8 instance = 0;
	char retryvalueStr[16] = {0};
	uint32 retryvalue = *((uint32*)value);

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode )
		return -1;

	instance = atoi(deviceNode->parentNode->name) - 1;

	sprintf(retryvalueStr, "%u", retryvalue);
	tcapi_set( "WLan_Common", "RetryTimeout", retryvalueStr );
	SetCfgNodeFlag( "WLan", instance - 1);

	return 0;
}
int8 getWLANCfgRetryTimeoutAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;
	uint8 instance = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode )
		return -1;

	instance = atoi(deviceNode->parentNode->name) - 1;

	attribute = tcWlanAttr.aRetryTimeout[instance];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLANCfgRetryTimeoutAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	uint8 instance = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode )
		return -1;

	instance = atoi(deviceNode->parentNode->name) - 1;

	attribute = &tcWlanAttr.aRetryTimeout[instance];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWLanCfg_X_CTCOM_ModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char modeValue[10] = {0};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "VlanBind_Entry%d", instance + 3);
	
	tcapi_get(nodeName, "Active", modeValue);

	if ( 0 == strcmp(modeValue, "Yes") )
		strcpy(modeValue, "1");
	else
		strcpy(modeValue, "0");
	
	return getTheValue(value, type, deviceNode, modeValue);
}
int8 setWLanCfg_X_CTCOM_ModeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char modeValue[10] = {0};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name) + 3;	

	if(*(uint32*)value != 0 && *(uint32*)value != 1)
		return -1;

	switch ( *(uint32*)value )
	{
		case 1:
			strcpy(modeValue, "Yes");
			break;
		default:
			strcpy(modeValue, "No");
			break;
	}

	sprintf(nodeName, "VlanBind_Entry%d", instance);
	tcapi_set(nodeName, "Active", modeValue);
	SetCfgNodeFlag ( "VlanBind", instance);

	return 0;
}
int8 getWLanCfg_X_CTCOM_ModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aX_CTCOM_Mode[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLanCfg_X_CTCOM_ModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aX_CTCOM_Mode[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWLanCfg_X_CTCOM_VLANValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char VLANValue[256] = {0};
	int instance = 0;
	
	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "VlanBind_Entry%d", instance + 3);
	
	tcapi_get(nodeName, "mn", VLANValue);
	
	return getTheValue(value, type, deviceNode, VLANValue);
}
int8 setWLanCfg_X_CTCOM_VLANValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char VLANValue[256] = {0};
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name) + 3;	

	sprintf(VLANValue, "%s", (char*)value);
	sprintf(nodeName, "VlanBind_Entry%d", instance);
	tcapi_set(nodeName, "mn", VLANValue);

	SetCfgNodeFlag ( "VlanBind", instance);

	return 0;
}

int8 getWLanCfg_X_CTCOM_VLANAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aX_CTCOM_VLAN[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLanCfg_X_CTCOM_VLANAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aX_CTCOM_VLAN[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWLanCfg_DetectionStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char result[20] = {0};
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	if ( !(instance >=1 && instance <=4) )
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Info_ra%d", instance - 1);
	if ( tcapi_get(nodeName, "detectionStatus", result) < 0 )
		strcpy(result, "0");

	return getTheValue(value, type, deviceNode, result);
}
int8 getWLanCfg_DetectionStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	if ( !(instance >=1 && instance <=4) )
		return -1;

	attribute = tcWlanAttr.aDetectionStatus[instance - 1];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWLanCfg_DetectionStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	if ( !(instance >=1 && instance <=4) )
		return -1;

	attribute = &tcWlanAttr.aDetectionStatus[instance - 1];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
#endif

#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
int8 getSSIDMaxUserNumValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	/*it should return empty according to spe TR098*/
	char nodeName[34] = {'\0'};
	char maxUserNum[10] = {'\0'};
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);
	
	tcapi_get(nodeName, "MaxStaNum", maxUserNum);

	
	return getTheValue(value, type, deviceNode, maxUserNum);
}
int8 setSSIDMaxUserNumValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	uint32 maxUserNum = 0;
	int instance = 0;
	char maxNumStr[10]={'\0'};

	instance = atoi(deviceNode->parentNode->name);	

	SetCfgNodeFlag( "WLan", instance - 1);
	
	if(value == NULL){
		return -1;
	}

	maxUserNum = *((uint32*)value);
	if(maxUserNum < 1 || maxUserNum > SSID_MAX_STA_NUM){//we only support 1~SSID_MAX_STA_NUM.
		return -1;
	}
	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);	
	instance = atoi(deviceNode->parentNode->name);
	sprintf(maxNumStr, "%lu", maxUserNum);
	tcapi_set(nodeName, "MaxStaNum", maxNumStr);
	return 0;	

}
int8 getSSIDMaxUserNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.maxUserNum[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSSIDMaxUserNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.maxUserNum[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
#endif

#if defined(TCSUPPORT_CT_PMINFORM)
int8 getWlanAccessEquipmentMacValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[512] = {0};
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);
	tcapi_get("LanHost","WlanDevMAC",result);
	return getTheValue(value, type, deviceNode, result);	
}
int8 getWlanAccessEquipmentMacAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);
	attribute = tcWlanAttr.aAccessEquipmentMac[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWlanAccessEquipmentMacAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);
	attribute = &tcWlanAttr.aAccessEquipmentMac[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getWlanCfgNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34];
	char name[33] = {0};

	int instance;

	memset(nodeName, 0, sizeof(nodeName));
	instance = atoi(deviceNode->parentNode->name);
	if(instance < 1 || instance > 4)
		return -1;
	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	tcapi_get(nodeName, "SSID", name);
	
	return getTheValue(value, type, deviceNode, name);
}
int8 getWlanCfgNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcWlanAttr.aWLanCfgName[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWlanCfgNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aWLanCfgName[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWlanCfgDualBandSupportValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char dualBandSupport[4]="0";
	return getTheValue(value, type, deviceNode, dualBandSupport);
}
int8 getWlanCfgDualBandSupportAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcWlanAttr.aWlanCfgDualBandSupport;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWlanCfgDualBandSupportAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcWlanAttr.aWlanCfgDualBandSupport;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}


int8 getWLANCfgKeyPassphraseValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char keyPassphraseStr[5] = {'\0'};
	
	return getTheValue(value, type, deviceNode, keyPassphraseStr);
}
int8 setWLANCfgKeyPassphraseValue(void* value, struct DeviceNode*  deviceNode)
{
	char 	nodeName[32] = {0};
	char 	keyPassphrase[64] = {0};
	int 	instance = 0;
	int 	len = 0;
	
	
	instance = atoi(deviceNode->parentNode->name);	
	if(instance < 1 || instance > 4)
		return -1;

	if(value == NULL){
		return -1;
	}
	
	len = strlen((char *)value);

	if(len > 63)
		return -1;

	strcpy(keyPassphrase,(char *)value);	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);	
	tcapi_set(nodeName, "KeyPassphrase", keyPassphrase);	
	//SetCfgNodeFlag( "WLan", instance - 1 );
	return 0;
}
int8 getWLANCfgKeyPassphraseAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcWlanAttr.aWLanKeyPassphrase[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	return result;
}
int8 setWLANCfgKeyPassphraseAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aWLanKeyPassphrase[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWlanCfgPowerSupportedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char powerSupported[64] = "15,25,50,75,100";
	
	return getTheValue(value, type, deviceNode, powerSupported);
}
int8 getWlanCfgPowerSupportedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcWlanAttr.aWlanCfgPowerSupported;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWlanCfgPowerSupportedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcWlanAttr.aWlanCfgPowerSupported;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWlanCfgWMMSupportedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WMMSupported[8] = "1";
	
	return getTheValue(value, type, deviceNode, WMMSupported);
}
int8 getWlanCfgWMMSupportedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcWlanAttr.aWlanCfgWMMSupported;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWlanCfgWMMSupportedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcWlanAttr.aWlanCfgWMMSupported;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWlanCfgUAPSDSupportedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char UAPSDSupported[8] = "1";
	
	return getTheValue(value, type, deviceNode, UAPSDSupported);
}
int8 getWlanCfgUAPSDSupportedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcWlanAttr.aWlanCfgUAPSDSupported;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWlanCfgUAPSDSupportedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	
	attribute = &tcWlanAttr.aWlanCfgUAPSDSupported;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWlanCfgWMMEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	int	instance = 0;
	char WMMEnable[5] = "0";
	
	instance = atoi(deviceNode->parentNode->name);	
	if(instance < 1 || instance > 4)
		return -1;		
		
	sprintf(nodeName, "WLan_Entry%d", instance - 1);
	tcapi_get(nodeName, "WMM", WMMEnable);
	
	return getTheValue(value, type, deviceNode, WMMEnable);
}
int8 setWlanCfgWMMEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char 	nodeName[32] = {0};
	int 	instance = 0;
	uint8 	enb = *((uint8*)value);
	
	
	instance = atoi(deviceNode->parentNode->name);	
	if(instance < 1 || instance > 4)
		return -1;		
		
	sprintf(nodeName, "WLan_Entry%d", instance - 1);
		
	if(1 == enb)
	{
		tcapi_set(nodeName, "WMM", "1");
	}
	else if(0 == enb)
	{
		tcapi_set(nodeName, "WMM", "0"); 
	}
	else
	{
		return -1;
	}	
	SetCfgNodeFlag( "WLan", instance - 1 );
	return 0;
}
int8 getWlanCfgWMMEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcWlanAttr.aWlanCfgWMMEnable[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	return result;
}
int8 setWlanCfgWMMEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aWlanCfgWMMEnable[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWlanCfgUAPSDEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	int	instance = 0;
	char UAPSDEnable[5] = "0";
	
	instance = atoi(deviceNode->parentNode->name);	
	if(instance < 1 || instance > 4)
		return -1;		
		
	sprintf(nodeName, "WLan_Entry%d", instance - 1);
	tcapi_get(nodeName, "UAPSD", UAPSDEnable);	
	
	if(strlen(UAPSDEnable)==0)
		strcpy(UAPSDEnable,"0");
		
	return getTheValue(value, type, deviceNode, UAPSDEnable);
}
int8 setWlanCfgUAPSDEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char 	nodeName[32] = {0};
	int 	instance = 0;
	uint8 	enb = *((uint8*)value);
	
	
	instance = atoi(deviceNode->parentNode->name);	
	if(instance < 1 || instance > 4)
		return -1;		
		
	sprintf(nodeName, "WLan_Entry%d", instance - 1);
		
	if(1 == enb)
	{
		tcapi_set(nodeName, "UAPSD", "1");
	}
	else if(0 == enb)
	{
		tcapi_set(nodeName, "UAPSD", "0"); 
	}
	else
	{
		return -1;
	}
	
	//SetCfgNodeFlag( "WLan", instance - 1 );
	return 0;
}
int8 getWlanCfgUAPSDEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcWlanAttr.aWlanCfgUAPSDEnable[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	return result;
}
int8 setWlanCfgUAPSDEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aWlanCfgUAPSDEnable[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

int8 getWlanCfgPreShareKeyPassphraseValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char PrekeyPassphraseStr[5] = {'\0'};
	
	return getTheValue(value, type, deviceNode, PrekeyPassphraseStr);
}
int8 setWlanCfgPreShareKeyPassphraseValue(void* value, struct DeviceNode*  deviceNode)
{
	char 	nodeName[32] = {0};
	char 	keyPassphrase[64] = {0};
	int 	instance = 0;
	int 	len = 0;
	
	
	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	if(instance < 1 || instance > 4)
		return -1;

	if(value == NULL){
		return -1;
	}
	
	len = strlen((char *)value);

	if(len > 63)
		return -1;

	strcpy(keyPassphrase,(char *)value);	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);

	instance = atoi(deviceNode->parentNode->name);

	if(instance == 1){		
		tcapi_set(nodeName, "KeyPassphrase", keyPassphrase);		
	}
	
	//SetCfgNodeFlag( "WLan", instance - 1 );
	return 0;
}
int8 getWlanCfgPreShareKeyPassphraseAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	attribute = tcWlanAttr.aWLanPreKeyPassphrase[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	return result;
}
int8 setWlanCfgPreShareKeyPassphraseAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;
	
	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aWLanPreKeyPassphrase[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

#endif
#if defined(TCSUPPORT_CT_FJ)
int8 getErrorsSentValue(char** value, ParameterType* type,  DeviceNode*  deviceNode)
{	
	char nodeName[32] = {0};
	int	instance = 0;
	char ErrorsSent[32] = {0};
	
	instance = atoi(deviceNode->parentNode->parentNode->name);	
	if(instance < 1 || instance > 4)
		return -1;		
		
	sprintf(nodeName, "Info_ra%d", instance - 1);
	tcapi_get(nodeName, "txerrors",ErrorsSent );
	
	return getTheValue(value, type, deviceNode,ErrorsSent );
}
int8 getErrorsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;
	int instance;
	
	instance = atoi(deviceNode->parentNode->parentNode->name);	
	attribute = tcWlanAttr.aWlanCfgErrorsSent[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	return result;
}
int8 setErrorsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{    
	uint8*  attribute = NULL;
	int8  result;
	int instance;
	
	instance = atoi(deviceNode->parentNode->parentNode->name);	
	attribute = &tcWlanAttr.aWlanCfgErrorsSent[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getErrorsReceivedValue(char** value, ParameterType* type,  DeviceNode*  deviceNode)
{	
	char nodeName[32] = {0};
	int	instance = 0;
	char ErrorsReceived[32] = {0};
	
	instance = atoi(deviceNode->parentNode->parentNode->name);	
	if(instance < 1 || instance > 4)
		return -1;		
		
	sprintf(nodeName, "Info_ra%d", instance - 1);
	tcapi_get(nodeName, "rxerrors",ErrorsReceived );
	
	return getTheValue(value, type, deviceNode,ErrorsReceived );
}
int8 getErrorsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;
	int instance;
	
	instance = atoi(deviceNode->parentNode->parentNode->name);	
	attribute = tcWlanAttr.aWlanCfgErrorsReceived[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	return result;
}
int8 setErrorsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{    
	uint8*  attribute = NULL;
	int8  result;
	int instance;
	
	instance = atoi(deviceNode->parentNode->parentNode->name);	
	attribute = &tcWlanAttr.aWlanCfgErrorsReceived[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
int8 getDiscardPacketsSentValue(char** value, ParameterType* type,  DeviceNode*  deviceNode)
{	
	char nodeName[32] = {0};
	int	instance = 0;
	char DiscardPackets[32] = {0};
	
	instance = atoi(deviceNode->parentNode->parentNode->name);	
	if(instance < 1 || instance > 4)
		return -1;		
		
	sprintf(nodeName, "Info_ra%d", instance - 1);
	tcapi_get(nodeName, "txdropped",DiscardPackets );
	
	return getTheValue(value, type, deviceNode,DiscardPackets );
}
int8 getDiscardPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	int8  result;
	uint8  attribute;
	int instance;
	
	instance = atoi(deviceNode->parentNode->parentNode->name);	
	attribute = tcWlanAttr.aWlanCfgDiscardPacketsSent[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	return result;
}
int8 setDiscardPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{    
	uint8*  attribute = NULL;
	int8  result;
	int instance;
	
	instance = atoi(deviceNode->parentNode->parentNode->name);	
	attribute = &tcWlanAttr.aWlanCfgDiscardPacketsSent[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}
#endif

int8 getBeaconAdvertisementEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char BeaconAdvertisementEnable[10]= {'\0'};		
	char	nodeName[32] = {0};
	int 	instance = 0;
	int ret = -1;

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	if(instance < 1 || instance > 4)
		return -1;
	
	if(value == NULL){
		return -1;
	}

	sprintf(nodeName, "WLan_Entry%d", instance - 1);


	ret = tcapi_get(nodeName,"BeaconAdvertisementEnable",BeaconAdvertisementEnable);
	if(ret != 0)
		strcpy(BeaconAdvertisementEnable,"0");
	
	return getTheValue(value, type, deviceNode, BeaconAdvertisementEnable);	
}
int8 setBeaconAdvertisementEnabledValue(void* value, struct DeviceNode*  deviceNode)
{
	char	nodeName[32] = {0};
	int 	instance = 0;	
	uint8 flag;	

	instance = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	if(instance < 1 || instance > 4)
		return -1;
	
	sprintf(nodeName, "WLan_Entry%d", instance - 1);
	flag = *((uint8*)value);

	SetCfgNodeFlag ( "WLan", -1 );
	
	if(flag == 1){
		tcapi_set(nodeName,"BeaconAdvertisementEnable","1");
	} 
	else if (flag == 0){
		tcapi_set(nodeName,"BeaconAdvertisementEnable","0");
	}
	else{
		tc_printf("==>setWLANCfgEnableValue:value should be 0 or 1.\n");
		return -1;
	}	
	
	return 0;
}

int8 getBeaconAdvertisementEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNodePtr deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcWlanAttr.aBeaconAdvertisementEnabled[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setBeaconAdvertisementEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNodePtr deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);	
	attribute = &tcWlanAttr.aBeaconAdvertisementEnabled[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= WLAN_ATTR_FLAG;
	return result;
}

