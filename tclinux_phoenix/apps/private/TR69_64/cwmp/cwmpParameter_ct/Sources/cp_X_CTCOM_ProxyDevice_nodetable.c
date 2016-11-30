#include "Global_res.h"
#include "cp_X_CTCOM_ProxyDevice.h"

CwmpParameterStruct cwmpInitDeviceInfoTable[] =
{
	{"FrendlyName", StrW, NoLimite, NULL, NULL},	
	{"DeviceType", StrR, NoLimite, NULL, NULL},	
	{"ProvisioningCode", StrW, NoLimite, NULL, NULL},	
	{"SoftwareVersion", StrR, NoLimite, NULL, NULL},	
	{"SoftwareDescription", StrR, NoLimite, NULL, NULL},	
	{"UpTime", UnsignIntR, NoLimite, NULL, NULL},	
	{"Mode", UnsignIntR, NoLimite, NULL, NULL},	
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc DeviceListNum=
{
	NULL,
	NULL,
	getDeviceListNum,
	NULL,
	getDeviceListNumAttribute, 
	setDeviceListNumAttribute, 
	NULL
};


static NodeFunc BMS_InvokeAction=
{
	NULL,
	NULL,
	getBMS_InvokeActionValue,
	setBMS_InvokeActionValue,
	getUPnPDMNodeValueAttribute, 
	setUPnPDMNodeValueAttribute, 
	NULL
};

static NodeFunc BMS_RebootStatus=
{
	NULL,
	NULL,
	getBMS_RebootStatusValue,
	NULL,
	getUPnPDMNodeValueAttribute, 
	setUPnPDMNodeValueAttribute, 
	NULL
};


static NodeFunc BMS_DeviceStatus=
{
	NULL,
	NULL,
	getBMS_DeviceStatusValue,
	NULL,
	getUPnPDMNodeValueAttribute, 
	setUPnPDMNodeValueAttribute, 
	NULL
};

static NodeFunc BMS_OperationStatus=
{
	NULL,
	NULL,
	getBMS_OperationStatus,
	NULL,
	getUPnPDMNodeValueAttribute, 
	setUPnPDMNodeValueAttribute, 
	NULL
};

static NodeFunc BMS_FileType=
{
	NULL,
	NULL,
	getBMS_FileType,
	setBMS_FileType,
	getUPnPDMNodeValueAttribute, 
	setUPnPDMNodeValueAttribute,  
	NULL
};

static NodeFunc BMS_URL=
{
	NULL,
	NULL,
	getBMS_URL,
	setBMS_URL,
	getUPnPDMNodeValueAttribute, 
	setUPnPDMNodeValueAttribute, 
	NULL
};


static NodeFunc BMS_SoftwareVersion=
{
	NULL,
	NULL,
	getBMS_SoftwareVersion,
	setBMS_SoftwareVersion,
	getUPnPDMNodeValueAttribute, 
	setUPnPDMNodeValueAttribute, 
	NULL
};

static NodeFunc BMS_HardwareVersion=
{
	NULL,
	NULL,
	getBMS_HardwareVersion,
	setBMS_HardwareVersion,
	getUPnPDMNodeValueAttribute, 
	setUPnPDMNodeValueAttribute,  
	NULL
};

static NodeFunc BMS_Channel=
{
	NULL,
	NULL,
	getBMS_Channel,
	setBMS_Channel,
	getUPnPDMNodeValueAttribute, 
	setUPnPDMNodeValueAttribute, 
	NULL
};

static NodeFunc BMS_TimeWindow=
{
	NULL,
	NULL,
	getBMS_TimeWindow,
	setBMS_TimeWindow,
	getUPnPDMNodeValueAttribute, 
	setUPnPDMNodeValueAttribute,  
	NULL
};


static NodeFunc BMS_Username=
{
	NULL,
	NULL,
	getBMS_Username,
	setBMS_Username,
	getUPnPDMNodeValueAttribute, 
	setUPnPDMNodeValueAttribute, 
	NULL
};

static NodeFunc BMS_Password=
{
	NULL,
	NULL,
	getBMS_Password,
	setBMS_Password,
	getUPnPDMNodeValueAttribute, 
	setUPnPDMNodeValueAttribute, 
	NULL
};

static NodeFunc BMS_DownloadStatus=
{
	NULL,
	NULL,
	getBMS_DownloadStatus,
	NULL,
	getUPnPDMNodeValueAttribute, 
	setUPnPDMNodeValueAttribute,  
	NULL
};

static NodeFunc BMS_OperationID=
{
	NULL,
	NULL,
	getBMS_OperationID,
	NULL,
	getUPnPDMNodeValueAttribute, 
	setUPnPDMNodeValueAttribute, 
	NULL
};

CwmpParameterStruct BMS_REBOOT[] = 
{
	{"InvokeAction", BooleanW, NoLimite, &BMS_InvokeAction, NULL},
	{"RebootStatus", StrR, NoLimite, &BMS_RebootStatus, NULL},
	{NULL, -1, -1, NULL, NULL}
};


CwmpParameterStruct BaselineReset[] = 
{
	{"InvokeAction", BooleanW, NoLimite, &BMS_InvokeAction, NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct GetDeviceStatus[] = 
{
	{"InvokeAction", BooleanW, NoLimite, &BMS_InvokeAction, NULL},
	{"DeviceStatus", StrR, NoLimite, &BMS_DeviceStatus, NULL},
	{NULL, -1, -1, NULL, NULL}
};


CwmpParameterStruct CTCOM_Download[] = 
{
	{"InvokeAction", BooleanW, NoLimite, &BMS_InvokeAction, NULL},
	{"FileType", StrW, NoLimite, &BMS_FileType, NULL},
	{"URL", StrW, NoLimite, &BMS_URL, NULL},
	{"SoftwareVersion", StrW, NoLimite, &BMS_SoftwareVersion, NULL},
	{"HardwareVersion", StrW, NoLimite, &BMS_HardwareVersion, NULL},
	{"Channel", IntW, NoLimite, &BMS_Channel, NULL},
	{"TimeWindow", StrW, NoLimite, &BMS_TimeWindow, NULL},	
	{"Username", StrW, NoLimite, &BMS_Username, NULL},
	{"Password", StrW, NoLimite, &BMS_Password, NULL},
	{"DownloadStatus", UnsignIntR, NoLimite, &BMS_DownloadStatus, NULL},
	{"OperationID", UnsignIntR, NoLimite, &BMS_OperationID, NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct CTCOM_GetDownloadStatus[] = 
{
	{"InvokeAction", BooleanW, NoLimite, &BMS_InvokeAction, NULL},
	{"OperationStatus", StrR, NoLimite, &BMS_OperationStatus, NULL},
	{NULL, -1, -1, NULL, NULL}
};


/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.DeviceList.{i}.ActionList.BMS.
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct UPnPDM_BMS[] = 
{
	{"Reboot", ObjectR, NoLimite, NULL, BMS_REBOOT},		
	{"BaselineReset", ObjectR, NoLimite, NULL, BaselineReset},
	{"GetDeviceStatus", ObjectR, NoLimite, NULL, GetDeviceStatus},
	{"X_CT-COM_Download", ObjectR, NoLimite, NULL, CTCOM_Download},
	{"X_CT-COM_GetDownloadStatus", ObjectR, NoLimite, NULL, CTCOM_GetDownloadStatus},
	{NULL, -1, -1, NULL, NULL}
};


/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.DeviceList.{i}.ActionList.
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct UPnPDM_ActionList[] = 
{
	{"BMS", ObjectR, NoLimite, NULL, UPnPDM_BMS},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.DeviceList.{i}.
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct UPnPDMDeviceList_1[] =
{
	{"ActionList", ObjectR, NoLimite, NULL, UPnPDM_ActionList},
	{NULL, -1, -1, NULL, NULL}
};


static NodeFunc UPnPDMDeviceListIndexFuncs=
{
	NULL,
	DelUPnPDMDeviceListObject,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL
};


/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.DeviceList.
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct UPnPDMDeviceListIndex[] = 
{
	{NULL,ObjectW, NoLimite, &UPnPDMDeviceListIndexFuncs,UPnPDMDeviceList_1},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc UPnPDMDynamicFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL
};

CwmpParameterStruct UPnPDMDynamicNode[] = 
{
	{NULL,ObjectW, NoLimite, &UPnPDMDynamicFuncs,NULL},
	{NULL, -1, -1, NULL, NULL}
};


static NodeFunc UPnPDM_DeviceListFuncs =
{
	AddUPnPDMDeviceListObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitUPnPDMDeviceListTable,
};

static NodeFunc RMSConfigOver=
{
	NULL,
	NULL,
	getRMSConfigOver,
	setRMSConfigOver,
	getRMSConfigOverAttribute, 
	setRMSConfigOverAttribute, 
	NULL
};

static NodeFunc FileProfileNumberOfEntries=
{
	NULL,
	NULL,
	getFileProfileNum,
	NULL,
	getFileProfileNumAttribute, 
	setFileProfileNumAttribute, 
	NULL
};


static NodeFunc FileProfileRule=
{
	NULL,
	NULL,
	getFileProfileRule,
	setFileProfileRule,
	getFileProfileRuleAttribute, 
	setFileProfileRuleAttribute, 
	NULL
};

static NodeFunc FileProfileFileType=
{
	NULL,
	NULL,
	getFileProfileFileType,
	setFileProfileFileType,
	getFileProfileFileTypeAttribute, 
	setFileProfileFileTypeAttribute, 
	NULL
};

static NodeFunc FileProfileURL=
{
	NULL,
	NULL,
	getFileProfileURL,
	setFileProfileURL,
	getFileProfileURLAttribute, 
	setFileProfileURLAttribute, 
	NULL
};

static NodeFunc FileProfileSWVersion=
{
	NULL,
	NULL,
	getFileProfileSWVersion,
	setFileProfileSWVersion,
	getFileProfileSWVersionAttribute, 
	setFileProfileSWVersionAttribute, 
	NULL
};

static NodeFunc FileProfileHWVersion=
{
	NULL,
	NULL,
	getFileProfileHWVersion,
	setFileProfileHWVersion,
	getFileProfileHWVersionAttribute, 
	setFileProfileHWVersionAttribute, 
	NULL
};

static NodeFunc FileProfileChannel=
{
	NULL,
	NULL,
	getFileProfileChannel,
	setFileProfileChannel,
	getFileProfileChannelAttribute, 
	setFileProfileChannelAttribute, 
	NULL
};


static NodeFunc FileProfileUsername=
{
	NULL,
	NULL,
	getFileProfileUsername,
	setFileProfileUsername,
	getFileProfileUsernameAttribute, 
	setFileProfileUsernameAttribute, 
	NULL
};

static NodeFunc FileProfilePassword=
{
	NULL,
	NULL,
	getFileProfilePassword,
	setFileProfilePassword,
	getFileProfilePasswordAttribute, 
	setFileProfilePasswordAttribute, 
	NULL
};


static NodeFunc TimeWindowListNumberOfEntries=
{
	NULL,
	NULL,
	getTimeWindowListNum,
	setTimeWindowListNum,
	getTimeWindowListNumAttribute, 
	setTimeWindowListNumAttribute, 
	NULL
};

static NodeFunc WindowStart=
{
	NULL,
	NULL,
	getWindowStart,
	setWindowStart,
	getWindowStartAttribute, 
	setWindowStartAttribute, 
	NULL
};

static NodeFunc WindowEnd=
{
	NULL,
	NULL,
	getWindowEnd,
	setWindowEnd,
	getWindowEndAttribute, 
	setWindowEndAttribute, 
	NULL
};

static NodeFunc WindowMode=
{
	NULL,
	NULL,
	getWindowMode,
	setWindowMode,
	getWindowModeAttribute, 
	setWindowModeAttribute, 
	NULL
};

static NodeFunc UserMessage=
{
	NULL,
	NULL,
	getUserMessage,
	setUserMessage,
	getUserMessageAttribute, 
	setUserMessageAttribute, 
	NULL
};

static NodeFunc MaxRetries=
{
	NULL,
	NULL,
	getMaxRetries,
	setMaxRetries,
	getMaxRetriesAttribute, 
	setMaxRetriesAttribute, 
	NULL
};

static NodeFunc ConfigProfileNumberOfEntries=
{
	NULL,
	NULL,
	getConfigProfileNum,
	NULL,
	getConfigProfileNumAttribute, 
	setConfigProfileNumAttribute, 
	NULL
};

static NodeFunc ServiceProfileRMSConfigOver=
{
	NULL,
	NULL,
	getSerProfileRMSConfigOver,
	setSerProfileRMSConfigOver,
	getSerProfileRMSConfigOverAttribute, 
	setSerProfileRMSConfigOverAttribute, 
	NULL
};

static NodeFunc ConfigurationMode=
{
	NULL,
	NULL,
	getConfigurationMode,
	setConfigurationMode,
	getConfigurationModeAttribute, 
	setConfigurationModeAttribute, 
	NULL
};


static NodeFunc ConfigProfileRule=
{
	NULL,
	NULL,
	getConfigProfileRule,
	setConfigProfileRule,
	getConfigProfileRuleAttribute, 
	setConfigProfileRuleAttribute, 
	NULL
};

static NodeFunc ConfigurationPath=
{
	NULL,
	NULL,
	getConfigurationPath,
	setConfigurationPath,
	getConfigurationPathAttribute, 
	setConfigurationPathAttribute, 
	NULL
};

static NodeFunc ProvisionCode=
{
	NULL,
	NULL,
	getConfigProvisionCode,
	setConfigProvisionCode,
	getConfigProvisionCodeAttribute, 
	setConfigProvisionCodeAttribute, 
	NULL
};

static NodeFunc AP_InternetNum=
{
	NULL,
	NULL,
	getAP_InternetNum,
	NULL,
	getAP_InternetNumAttribute, 
	setAP_InternetNumAttribute, 
	NULL
};

static NodeFunc AP_IPTVNum=
{
	NULL,
	NULL,
	getAP_IPTVNum,
	NULL,
	getAP_IPTVNumAttribute, 
	setAP_IPTVNumAttribute, 
	NULL
};

static NodeFunc AP_VoIPNum=
{
	NULL,
	NULL,
	getAP_VoIPNum,
	NULL,
	getAP_VoIPNumAttribute, 
	setAP_VoIPNumAttribute, 
	NULL
};

static NodeFunc STB_InternetNum=
{
	NULL,
	NULL,
	getSTB_InternetNum,
	NULL,
	getSTB_InternetNumAttribute, 
	setSTB_InternetNumAttribute, 
	NULL
};

static NodeFunc STB_IPTVNum=
{
	NULL,
	NULL,
	getSTB_IPTVNum,
	NULL,
	getSTB_IPTVNumAttribute, 
	setSTB_IPTVNumAttribute, 
	NULL
};

static NodeFunc STB_VoIPNum=
{
	NULL,
	NULL,
	getSTB_VoIPNum,
	NULL,
	getSTB_VoIPNumAttribute, 
	setSTB_VoIPNumAttribute, 
	NULL
};

static NodeFunc Screen4_IPTVNum=
{
	NULL,
	NULL,
	getScreen4_IPTVNum,
	NULL,
	getScreen4_IPTVNumAttribute, 
	setScreen4_IPTVNumAttribute, 
	NULL
};

static NodeFunc Screen4_InternetNum=
{
	NULL,
	NULL,
	getScreen4_InternetNum,
	NULL,
	getScreen4_InternetNumAttribute, 
	setScreen4_InternetNumAttribute, 
	NULL
};

static NodeFunc Screen4_VoIPNum=
{
	NULL,
	NULL,
	getScreen4_VoIPNum,
	NULL,
	getScreen4_VoIPNumAttribute, 
	setScreen4_VoIPNumAttribute, 
	NULL
};

static NodeFunc HomeCenter_InternetNum=
{
	NULL,
	NULL,
	getHomeCenter_InternetNum,
	NULL,
	getHomeCenter_InternetNumAttribute, 
	setHomeCenter_InternetNumAttribute, 
	NULL
};


static NodeFunc HomeCenter_NASNum=
{
	NULL,
	NULL,
	getHomeCenter_NASNum,
	NULL,
	getHomeCenter_NASNumAttribute, 
	setHomeCenter_NASNumAttribute, 
	NULL
};

static NodeFunc HomeCenter_IPTVNum=
{
	NULL,
	NULL,
	getHomeCenter_IPTVNum,
	NULL,
	getHomeCenter_IPTVNumAttribute, 
	setHomeCenter_IPTVNumAttribute, 
	NULL
};

static NodeFunc HomeCenter_VoIPNum=
{
	NULL,
	NULL,
	getHomeCenter_VoIPNum,
	NULL,
	getHomeCenter_VoIPNumAttribute, 
	setHomeCenter_VoIPNumAttribute, 
	NULL
};

static NodeFunc Plc_Num=
{
	NULL,
	NULL,
	getPlc_Num,
	NULL,
	getPlc_NumAttribute, 
	setPlc_NumAttribute, 
	NULL
};


/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.SoftwareProfile.FileProfile.{i}.TimeWindowList.{i}
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct TimeWindowList_1[] =
{
	{"WindowStart", UnsignIntW, NoLimite, &WindowStart, NULL},		
	{"WindowEnd", UnsignIntW, NoLimite, &WindowEnd, NULL},
	{"WindowMode", StrW, NoLimite, &WindowMode, NULL},
	{"UserMessage", StrW, NoLimite, &UserMessage, NULL},	
	{"MaxRetries", IntW, NoLimite, &MaxRetries, NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc TimeWindowListIndexFuncs=
{
	NULL,
	DeviceNodeDelTimeWindowListObject,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL
};



/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.SoftwareProfile.FileProfile.{i}.TimeWindowList.
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct TimeWindowListIndex[] = 
{
	{NULL,ObjectW, NoLimite, &TimeWindowListIndexFuncs,TimeWindowList_1},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc TimeWindowList =
{
	DeviceNodeAddTimeWindowListObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitTimeWindowListTable,
};



/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.SoftwareProfile.FileProfile.{i}.
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct FileProfileList_1[] =
{
	{"Rule", StrW, NoLimite, &FileProfileRule, NULL},
	{"FileType", StrW, NoLimite, &FileProfileFileType, NULL},
	{"URL", StrW, NoLimite, &FileProfileURL, NULL},
	{"SoftwareVersion", StrW, NoLimite, &FileProfileSWVersion, NULL},	
	{"HardwareVersion", StrW, NoLimite, &FileProfileHWVersion, NULL},
	{"Channel", IntW, NoLimite, &FileProfileChannel, NULL},
	{"Username", StrW, NoLimite, &FileProfileUsername, NULL},
	{"Password", StrW, NoLimite, &FileProfilePassword, NULL},
	{"TimeWindowListNumberOfEntries", UnsignIntR, NoLimite, &TimeWindowListNumberOfEntries, NULL},
	{"TimeWindowList", ObjectW, NoLimite, &TimeWindowList, NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc FileProfileListIndexFuncs=
{
	NULL,
	DeviceNodeDelFileProfileListObject,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL
};


/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.SoftwareProfile.FileProfile.
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct FileProfileListIndex[] = 
{
	{NULL,ObjectW, NoLimite, &FileProfileListIndexFuncs,FileProfileList_1},
	{NULL, -1, -1, NULL, NULL}
};


static NodeFunc FileProfile =
{
	DeviceNodeAddFileProfileListObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitFileProfileTable,
};

/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.SoftwareProfile.
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct SofteWareProfile[] =
{
	{X_CT_COM"RMSConfigOver", UnsignIntW, NoLimite, &RMSConfigOver, NULL},
	{"FileProfileNumberOfEntries", UnsignIntR, NoLimite, &FileProfileNumberOfEntries, NULL},
	{"FileProfile", ObjectW, NoLimite, &FileProfile, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.ConfigProfile.{i}.
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct ConfigProfileList_1[] =
{
	{"Rule", StrW, NoLimite, &ConfigProfileRule, NULL},
	{"ConfigurationPath", StrW, NoLimite, &ConfigurationPath, NULL},
	{"ProvisionCode", StrW, NoLimite, &ProvisionCode, NULL},
	{NULL, -1, -1, NULL, NULL}
};


static NodeFunc ConfigProfileListIndexFuncs=
{
	NULL,
	DeviceNodeDelConfigProfileListObject,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL
};


/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.ConfigProfile.
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct ConfigProfileListIndex[] = 
{
	{NULL,ObjectW, NoLimite, &ConfigProfileListIndexFuncs,ConfigProfileList_1},
	{NULL, -1, -1, NULL, NULL}
};


static NodeFunc ConfigProfile =
{
	DeviceNodeAddConfigProfileListObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitConfigProfileTable,
};


static NodeFunc APWLanEnable=
{
	NULL,
	NULL,
	getAPWLanEnable,
	setAPWLanEnable,
	getAPWLanEnableAttribute, 
	setAPWLanEnableAttribute, 
	NULL
};

static NodeFunc APWLanMaxBitRate=
{
	NULL,
	NULL,
	getAPWLanMaxBitRate,
	setAPWLanMaxBitRate,
	getAPWLanMaxBitRateAttribute, 
	setAPWLanMaxBitRateAttribute, 
	NULL
};

static NodeFunc APWLanChannel=
{
	NULL,
	NULL,
	getAPWLanChannel,
	setAPWLanChannel,
	getAPWLanChannelAttribute, 
	setAPWLanChannelAttribute, 
	NULL
};

static NodeFunc APWLanSSID=
{
	NULL,
	NULL,
	getAPWLanSSID,
	setAPWLanSSID,
	getAPWLanSSIDAttribute, 
	setAPWLanSSIDAttribute, 
	NULL
};

static NodeFunc APWLanBeaconType=
{
	NULL,
	NULL,
	getAPWLanBeaconType,
	setAPWLanBeaconType,
	getAPWLanBeaconTypeAttribute, 
	setAPWLanBeaconTypeAttribute, 
	NULL
};

static NodeFunc MACAddressControlEnabled=
{
	NULL,
	NULL,
	getMACAddressControlEnabled,
	setMACAddressControlEnabled,
	getMACAddressControlEnabledAttribute, 
	setMACAddressControlEnabledAttribute, 
	NULL
};

static NodeFunc APWLanSSIDHide=
{
	NULL,
	NULL,
	getAPWLanSSIDHide,
	setAPWLanSSIDHide,
	getAPWLanSSIDHideAttribute, 
	setAPWLanSSIDHideAttribute, 
	NULL
};

static NodeFunc APWLanPowerlevel=
{
	NULL,
	NULL,
	getAPWLanPowerlevel,
	setAPWLanPowerlevel,
	getAPWLanPowerlevelAttribute, 
	setAPWLanPowerlevelAttribute, 
	NULL
};

static NodeFunc APWLanPowerValue=
{
	NULL,
	NULL,
	getAPWLanPowerValue,
	setAPWLanPowerValue,
	getAPWLanPowerValueAttribute, 
	setAPWLanPowerValueAttribute, 
	NULL
};

static NodeFunc APModuleEnable=
{
	NULL,
	NULL,
	getAPModuleEnable,
	setAPModuleEnable,
	getAPModuleEnableAttribute, 
	setAPModuleEnableAttribute, 
	NULL
};

static NodeFunc WPSKeyWord=
{
	NULL,
	NULL,
	getWPSKeyWord,
	setWPSKeyWord,
	getWPSKeyWordAttribute, 
	setWPSKeyWordAttribute, 
	NULL
};

static NodeFunc APWLanStandard=
{
	NULL,
	NULL,
	getAPWLanStandard,
	setAPWLanStandard,
	getAPWLanStandardAttribute, 
	setAPWLanStandardAttribute, 
	NULL
};

static NodeFunc WEPEncryptionLevel=
{
	NULL,
	NULL,
	getWEPEncryptionLevel,
	setWEPEncryptionLevel,
	getWEPEncryptionLevelAttribute, 
	setWEPEncryptionLevelAttribute, 
	NULL
};


static NodeFunc WEPKeyIndex=
{
	NULL,
	NULL,
	getWEPKeyIndex,
	setWEPKeyIndex,
	getWEPKeyIndexAttribute, 
	setWEPKeyIndexAttribute, 
	NULL
};

static NodeFunc BasicAuthenticationMode=
{
	NULL,
	NULL,
	getBasicAuthenticationMode,
	setBasicAuthenticationMode,
	getBasicAuthenticationModeAttribute, 
	setBasicAuthenticationModeAttribute, 
	NULL
};

static NodeFunc WPAEncryptionModes=
{
	NULL,
	NULL,
	getWPAEncryptionModes,
	setWPAEncryptionModes,
	getWPAEncryptionModesAttribute, 
	setWPAEncryptionModesAttribute, 
	NULL
};

static NodeFunc CTComRFBand=
{
	NULL,
	NULL,
	getCTComRFBand,
	setCTComRFBand,
	getCTComRFBandAttribute,
	setCTComRFBandAttribute,
	NULL
};

static NodeFunc CTComChannelWidth=
{
	NULL,
	NULL,
	getCTComChannelWidth,
	setCTComChannelWidth,
	getCTComChannelWidthAttribute,
	setCTComChannelWidthAttribute,
	NULL
};

static NodeFunc CTComGuardInterval=
{
	NULL,
	NULL,
	getCTComGuardInterval,
	setCTComGuardInterval,
	getCTComGuardIntervalAttribute,
	setCTComGuardIntervalAttribute,
	NULL
};

static NodeFunc CTComRetryTimeout=
{
	NULL,
	NULL,
	getCTComRetryTimeout,
	setCTComRetryTimeout,
	getCTComRetryTimeoutAttribute,
	setCTComRetryTimeoutAttribute,
	NULL
};

static NodeFunc CTComVLAN=
{
	NULL,
	NULL,
	getCTComVLAN,
	setCTComVLAN,
	getCTComVLANAttribute, 
	setCTComVLANAttribute, 
	NULL,
};


static NodeFunc APWEPKey=
{
	NULL,
	NULL,
	getAPWEPKey,
	setAPWEPKey,
	getAPWEPKeyAttribute, 
	setAPWEPKeyAttribute, 
	NULL,
};

static NodeFunc APPreSharedKey=
{
	NULL,
	NULL,
	getAPPreSharedKey,
	setAPPreSharedKey,
	getAPPreSharedKeyAttribute, 
	setAPPreSharedKeyAttribute, 
	NULL,
};

static NodeFunc APPreKeyPassphrase=
{
	NULL,
	NULL,
	getAPPreKeyPassphrase,
	setAPPreKeyPassphrase,
	getAPPreKeyPassphraseAttribute, 
	setAPPreKeyPassphraseAttribute, 
	NULL,
};


/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.ConfigTemplate.X_CT-COM_AP_Internet.{i}.WEPKey.{i}
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct APWEPKeyArray[] = 
{
	{"WEPKey", StrW, NoLimite, &APWEPKey, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.ConfigTemplate.X_CT-COM_AP_Internet.{i}.WEPKey.{i}
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct APWEPKeyeIndex[]= 
{
	{NULL, ObjectR, NoLimite, NULL, APWEPKeyArray},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.ConfigTemplate.X_CT-COM_AP_Internet.{i}.WEPKey
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
static NodeFunc WEPKeyFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitAPWEPKeyTable
};

/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.ConfigTemplate.X_CT-COM_AP_Internet.{i}.PreSharedKey.{i}
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct APPreSharedKeyArray[] = 
{
	{"PreSharedKey", StrW, NoLimite, &APPreSharedKey, NULL},
	{"KeyPassphrase", StrW, NoLimite, &APPreKeyPassphrase, NULL},
	{NULL, -1, -1, NULL, NULL}
};


/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.ConfigTemplate.X_CT-COM_AP_Internet.{i}.PreSharedKey.{i}
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct APPreSharedKeyIndex[]= 
{
	{NULL, ObjectR, NoLimite, NULL, APPreSharedKeyArray},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc PreSharedKeyFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitAPPreSharedKey
};



/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.ConfigTemplate.X_CT-COM_AP_Internet.{i}.
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct APWLanIndex_1[] =
{
	{"Enable", BooleanW, NoLimite, &APWLanEnable, NULL},	
	{"MaxBitRate", StrW, NoLimite, &APWLanMaxBitRate, NULL},
	{"Channel", UnsignIntW, NoLimite, &APWLanChannel, NULL},
	{"SSID", StrW, NoLimite, &APWLanSSID, NULL},
	{"BeaconType", StrW, NoLimite, &APWLanBeaconType, NULL},
	{"MACAddressControlEnabled", BooleanW, NoLimite, &MACAddressControlEnabled, NULL},	
	{X_CT_COM"SSIDHide", BooleanW, NoLimite, &APWLanSSIDHide, NULL},
	{X_CT_COM"Powerlevel", UnsignIntW, NoLimite, &APWLanPowerlevel, NULL},
	{X_CT_COM"PowerValue", UnsignIntW, NoLimite, &APWLanPowerValue, NULL},
	{X_CT_COM"APModuleEnable", BooleanW, NoLimite, &APModuleEnable, NULL},
	{X_CT_COM"WPSKeyWord", UnsignIntW, NoLimite, &WPSKeyWord, NULL},
	{"Standard", StrW, NoLimite, &APWLanStandard, NULL},
	{"WEPEncryptionLevel", StrW, NoLimite, &WEPEncryptionLevel, NULL},
	{"WEPKeyIndex", UnsignIntW, NoLimite, &WEPKeyIndex, NULL},
	{"BasicAuthenticationMode", StrW, NoLimite, &BasicAuthenticationMode, NULL},
	{"WPAEncryptionModes", StrW, NoLimite, &WPAEncryptionModes, NULL},
	{X_CT_COM"RFBand", UnsignIntW, NoLimite, &CTComRFBand, NULL},
	{X_CT_COM"ChannelWidth", UnsignIntW, NoLimite, &CTComChannelWidth, NULL},
	{X_CT_COM"GuardInterval", UnsignIntW, NoLimite, &CTComGuardInterval, NULL},
	{X_CT_COM"RetryTimeout", UnsignIntW, NoLimite, &CTComRetryTimeout, NULL},
	{X_CT_COM"VLAN", IntW, NoLimite, &CTComVLAN, NULL},
	{"WEPKey", ObjectR, NoLimite, &WEPKeyFuncs, NULL},
	{"PreSharedKey", ObjectR, NoLimite, &PreSharedKeyFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc APWLanIndexFuncs=
{
	NULL,
	DeviceNodeDelAPWLanListObject,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL
};


/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.ConfigTemplate.X_CT-COM_AP_Internet.
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct APWLanIndex[] = 
{
	{NULL,ObjectW, NoLimite, &APWLanIndexFuncs,APWLanIndex_1},
	{NULL, -1, -1, NULL, NULL}
};


static NodeFunc AP_WLanFuncs =
{
	DeviceNodeAddAPWLanListObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitAPWLanTable,
};


/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.ConfigTemplate.
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct ConfigTemplate[] =
{
#if !defined(TCSUPPORT_CT_PON_JX) || defined(TCSUPPORT_CT_PON_HUB)
	{X_CT_COM"AP_InternetNumberOfEntries", UnsignIntR, NoLimite, &AP_InternetNum, NULL},
	{X_CT_COM"AP_IPTVNumberOfEntries", UnsignIntR, NoLimite, &AP_IPTVNum, NULL},
	{X_CT_COM"AP_VoIPNumberOfEntries", UnsignIntR, NoLimite, &AP_VoIPNum, NULL},	
#endif
	{X_CT_COM"STB_InternetNumberOfEntries", UnsignIntR, NoLimite, &STB_InternetNum, NULL},
	{X_CT_COM"STB_IPTVNumberOfEntries", UnsignIntR, NoLimite, &STB_IPTVNum, NULL},
	{X_CT_COM"STB_VoIPNumberOfEntries", UnsignIntR, NoLimite, &STB_VoIPNum, NULL},		
	{X_CT_COM"Screen4_IPTVNumberOfEntries", UnsignIntR, NoLimite, &Screen4_IPTVNum, NULL},
	{X_CT_COM"Screen4_InternetNumberOfEntries", UnsignIntR, NoLimite, &Screen4_InternetNum, NULL},	
	{X_CT_COM"Screen4_VoIPNumberOfEntries", UnsignIntR, NoLimite, &Screen4_VoIPNum, NULL},		
	{X_CT_COM"HomeCenter_InternetNumberOfEntries", UnsignIntR, NoLimite, &HomeCenter_InternetNum, NULL},		
	{X_CT_COM"HomeCenter_NASNumberOfEntries", UnsignIntR, NoLimite, &HomeCenter_NASNum, NULL},		
	{X_CT_COM"HomeCenter_IPTVNumberOfEntries", UnsignIntR, NoLimite, &HomeCenter_IPTVNum, NULL},		
	{X_CT_COM"HomeCenter_VoIPNumberOfEntries", UnsignIntR, NoLimite, &HomeCenter_VoIPNum, NULL},			
	{X_CT_COM"Plc_NumberOfEntries", UnsignIntR, NoLimite, &Plc_Num, NULL},		
#if !defined(TCSUPPORT_CT_PON_JX) || defined(TCSUPPORT_CT_PON_HUB)
	{X_CT_COM"AP_Internet", ObjectW, NoLimite, &AP_WLanFuncs, NULL},
	{X_CT_COM"AP_IPTV", ObjectW, NoLimite, &AP_WLanFuncs, NULL},
#endif
	#if 0
	{X_CT_COM"AP_VoIP", ObjectW, NoLimite, &AP_VoIPFuncs, NULL},
	{X_CT_COM"STB_Internet", ObjectW, NoLimite, &STB_InternetFuncs, NULL},
	{X_CT_COM"STB_IPTV", ObjectW, NoLimite, &AP_IPTVFuncs, NULL},
	{X_CT_COM"STB_VoIP", ObjectW, NoLimite, &AP_VoIPFuncs, NULL},
	{X_CT_COM"Screen4_Internet", ObjectW, NoLimite, &Screen4_InternetFuncs, NULL},
	{X_CT_COM"Screen4_IPTV", ObjectW, NoLimite, &Screen4_IPTVFuncs, NULL},
	{X_CT_COM"Screen4_VoIP", ObjectW, NoLimite, &Screen4_VoIPFuncs, NULL},
	{X_CT_COM"HomeCenter_Internet", ObjectW, NoLimite, &HomeCenter_InternetFuncs, NULL},
	{X_CT_COM"HomeCenter_NAS", ObjectW, NoLimite, &HomeCenter_NASFuncs, NULL},
	{X_CT_COM"HomeCenter_IPTV", ObjectW, NoLimite, &HomeCenter_IPTVFuncs, NULL},
	{X_CT_COM"HomeCenter_VoIP", ObjectW, NoLimite, &HomeCenter_VoIPFuncs, NULL},
	{X_CT_COM"Plc", ObjectW, NoLimite, &PlcFuncs, NULL},
	#endif
	{NULL, -1, -1, NULL, NULL}
};


/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.
	Parent Node Table Location: cp_X_CTCOM_ProxyDevice_nodetable.c
*/
CwmpParameterStruct ServiceProfile[] =
{
	{"ConfigProfileNumberOfEntries", UnsignIntR, NoLimite, &ConfigProfileNumberOfEntries, NULL},
	{X_CT_COM"RMSConfigOver", UnsignIntW, NoLimite, &ServiceProfileRMSConfigOver, NULL},
	{X_CT_COM"ConfigurationMode", UnsignIntW, NoLimite, &ConfigurationMode, NULL},
	{"ConfigProfile", ObjectW, NoLimite, &ConfigProfile, NULL},
	{"ConfigTemplate", ObjectR, NoLimite, NULL, ConfigTemplate},
	{NULL, -1, -1, NULL, NULL}
};


/*
	Parent Node: InternetGatewayDevice.X_CT-COM_ProxyDevice.
	Parent Node Table Location: cp_topnodetable.c 
*/
CwmpParameterStruct X_CTCOM_ProxyDevice[] =
{
	{"DeviceListNumberOfEntries", UnsignIntR, NoLimite, &DeviceListNum, NULL},
	{"ServiceProfile", ObjectR, NoLimite, NULL, ServiceProfile},
	{"SoftwareProfile", ObjectR, NoLimite, NULL, SofteWareProfile},
	{UPNPDM_DEVICELIST, ObjectR, NoLimite, &UPnPDM_DeviceListFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};


static NodeFunc UPnPDM_Funcs =
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL,
};

CwmpParameterStruct X_CTCOM_UPnPDM[] =
{
	{NULL, -1, -1, NULL, NULL}
};



