#include "Global_res.h"
#include "cp_landev_wlan.h"

#if defined(TCSUPPORT_CT_WLAN_NODE)
#if !defined(TCSUPPORT_C1_CUC)
static NodeFunc SSIDHide=
{
	NULL,
	NULL,
	getWLANCfgSSIDHideValue,
	setWLANCfgSSIDHideValue,
	getWLANCfgSSIDHideAttribute, 
	setWLANCfgSSIDHideAttribute, 
	NULL

};

static NodeFunc PowerValue=
{
	NULL,
	NULL,
	getWLANCfgPowerValueValue,
	NULL,
	getWLANCfgPowerValueAttribute, 
	setWLANCfgPowerValueAttribute, 
	NULL

};

static NodeFunc Powerlevel=
{
	NULL,
	NULL,
	getWLANCfgPowerlevelValue,
	setWLANCfgPowerlevelValue,
	getWLANCfgPowerlevelAttribute, 
	setWLANCfgPowerlevelAttribute, 
	NULL

};

static NodeFunc WPSKeyWord=
{
	NULL,
	NULL,
	getWLANCfgWPSKeyWordValue,
	setWLANCfgWPSKeyWordValue,
	getWLANCfgWPSKeyWordAttribute, 
	setWLANCfgWPSKeyWordAttribute, 
	NULL

};

static NodeFunc APModuleEnable=
{
	NULL,
	NULL,
	getWLANCfgEnableValue,
	setWLANCfgEnableValue, 
	getWLANCfgEnableAttribute, 
	setWLANCfgEnableAttribute, 
	NULL
};
#endif
static NodeFunc ChannelsInUse=
{
	NULL,
	NULL,
	getWLANCfgChannelsInUseValue,
	NULL,
	getWLANCfgChannelsInUseAttribute, 
	setWLANCfgChannelsInUseAttribute, 
	NULL
};

//modified by xmdai_nj:per ssid enable/disable
static NodeFunc Enable=
{
	NULL,
	NULL,
	getWLANCfgSSIDEnableValue,
	setWLANCfgSSIDEnableValue, 
	getWLANCfgSSIDEnableAttribute, 
	setWLANCfgSSIDEnableAttribute, 
	NULL
};
#else
static NodeFunc Enable=
{
	NULL,
	NULL,
	getWLANCfgEnableValue,
	setWLANCfgEnableValue, 
	getWLANCfgEnableAttribute, 
	setWLANCfgEnableAttribute, 
	NULL
};
#endif

static NodeFunc Status=
{
	NULL,
	NULL,
	getWLANCfgStatusValue,
	NULL,
	getWLANCfgStatusAttribute, 
	setWLANCfgStatusAttribute, 
	NULL
};

static NodeFunc BSSID=
{
	NULL,
	NULL,
	getWLANCfgBSSIDValue,
	NULL,
	getWLANCfgBSSIDAttribute, 
	setWLANCfgBSSIDAttribute, 
	NULL
};

static NodeFunc MaxBitRate=
{
	NULL,
	NULL,
	getWLANCfgMaxBitRateValue,
	setWLANCfgMaxBitRateValue,
	getWLANCfgMaxBitRateAttribute, 
	setWLANCfgMaxBitRateAttribute, 
	NULL
};

static NodeFunc Channel=
{
	NULL,
	NULL,
	getWLANCfgChannelValue,
	setWLANCfgChannelValue,
	getWLANCfgChannelAttribute, 
	setWLANCfgChannelAttribute, 
	NULL
};

static NodeFunc AutoChannelEnable=
{
	NULL,
	NULL,
	getWLANCfgAutoChannelEnableValue,
	setWLANCfgAutoChannelEnableValue,
	getWLANCfgAutoChannelEnableAttribute, 
	setWLANCfgAutoChannelEnableAttribute, 
	NULL
};

static NodeFunc SSID=
{
	NULL,
	NULL,
	getWLANCfgSSIDValue,
	setWLANCfgSSIDValue,
	getWLANCfgSSIDAttribute, 
	setWLANCfgSSIDAttribute, 
	NULL
};

static NodeFunc BeaconType=
{
	NULL,
	NULL,
	getWLANCfgBeaconTypeValue,
	setWLANCfgBeaconTypeValue,
	getWLANCfgBeaconTypeAttribute, 
	setWLANCfgBeaconTypeAttribute, 
	NULL
};

static NodeFunc MACAddressControlEnabled=
{
	NULL,
	NULL,
	getWLANCfgMACAddressControlEnabledValue,
	setWLANCfgMACAddressControlEnabledValue,
	getWLANCfgMACAddressControlEnabledAttribute, 
	setWLANCfgMACAddressControlEnabledAttribute, 
	NULL
};

static NodeFunc Standard=
{
	NULL,
	NULL,
	getWLANCfgStandardValue,
#if defined(TCSUPPORT_CT_WLAN_NODE) && !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	setWLANCfgStandardValue,
#else
	NULL,
#endif
	getWLANCfgStandardAttribute, 
	setWLANCfgStandardAttribute, 
	NULL
};

static NodeFunc WEPKeyIndex=
{
	NULL,
	NULL,
	getWLANCfgWEPKeyIndexValue,
	setWLANCfgWEPKeyIndexValue,
	getWLANCfgWEPKeyIndexAttribute, 
	setWLANCfgWEPKeyIndexAttribute, 
	NULL
};

static NodeFunc WEPEncryptionLevel=
{
	NULL,
	NULL,
	getWLANCfgWEPEncryptionLevelValue,
#if defined(TCSUPPORT_CT_WLAN_NODE) && !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	setWLANCfgWEPEncryptionLevelValue,
#else
	NULL,
#endif
	getWLANCfgWEPEncryptionLevelAttribute, 
	setWLANCfgWEPEncryptionLevelAttribute, 
	NULL
};

static NodeFunc BasicEncryptionModes=
{
	NULL,
	NULL,
	getWLANCfgBasicEncryptionModesValue,
	setWLANCfgBasicEncryptionModesValue,
	getWLANCfgBasicEncryptionModesAttribute, 
	setWLANCfgBasicEncryptionModesAttribute, 
	NULL
};

static NodeFunc BasicAuthenticationMode=
{
	NULL,
	NULL,
	getWLANCfgBasicAuthenticationModeValue,
	setWLANCfgBasicAuthenticationModeValue,
	getWLANCfgBasicAuthenticationModeAttribute, 
	setWLANCfgBasicAuthenticationModeAttribute, 
	NULL
};

static NodeFunc WPAEncryptionModes=
{
	NULL,
	NULL,
	getWLANCfgWPAEncryptionModesValue,
	setWLANCfgWPAEncryptionModesValue,
	getWLANCfgWPAEncryptionModesAttribute, 
	setWLANCfgWPAEncryptionModesAttribute, 
	NULL
};

static NodeFunc WPAAuthenticationMode=
{
	NULL,
	NULL,
	getWLANCfgWPAAuthenticationModeValue,
	setWLANCfgWPAAuthenticationModeValue,
	getWLANCfgWPAAuthenticationModeAttribute, 
	setWLANCfgWPAAuthenticationModeAttribute, 
	NULL
};

static NodeFunc IEEE11iEncryptionModes=
{
	NULL,
	NULL,
	getWLANCfgIEEE11iEncryptionModesValue,
	setWLANCfgIEEE11iEncryptionModesValue,
	getWLANCfgIEEE11iEncryptionModesAttribute, 
	setWLANCfgIEEE11iEncryptionModesAttribute, 
	NULL
};

static NodeFunc IEEE11iAuthenticationMode=
{
	NULL,
	NULL,
	getWLANCfgIEEE11iAuthenticationModeValue,
	setWLANCfgIEEE11iAuthenticationModeValue,
	getWLANCfgIEEE11iAuthenticationModeAttribute, 
	setWLANCfgIEEE11iAuthenticationModeAttribute, 
	NULL
};

static NodeFunc PossibleChannels=
{
	NULL,
	NULL,
	getWLANCfgPossibleChannelsValue,
	NULL,
	getWLANCfgPossibleChannelsAttribute, 
	setWLANCfgPossibleChannelsAttribute, 
	NULL
};

static NodeFunc BasicDataTransmitRates=
{
	NULL,
	NULL,
	getWLANCfgBasicDataTransmitRatesValue,
	setWLANCfgBasicDataTransmitRatesValue,
	getWLANCfgBasicDataTransmitRatesAttribute, 
	setWLANCfgBasicDataTransmitRatesAttribute, 
	NULL
};

static NodeFunc OperationalDataTransmitRates=
{
	NULL,
	NULL,
	getWLANCfgOperationalDataTransmitRatesValue,
	setWLANCfgOperationalDataTransmitRatesValue,
	getWLANCfgOperationalDataTransmitRatesAttribute, 
	setWLANCfgOperationalDataTransmitRatesAttribute, 
	NULL
};

static NodeFunc PossibleDataTransmitRates=
{
	NULL,
	NULL,
	getWLANCfgPossibleDataTransmitRatesValue,
	NULL,
	getWLANCfgPossibleDataTransmitRatesAttribute, 
	setWLANCfgPossibleDataTransmitRatesAttribute, 
	NULL
};

static NodeFunc RadioEnabled=
{
	NULL,
	NULL,
	getWLANCfgRadioEnabledValue,
	setWLANCfgRadioEnabledValue,
	getWLANCfgRadioEnabledAttribute, 
	setWLANCfgRadioEnabledAttribute, 
	NULL
};

static NodeFunc AutoRateFallBackEnabled=
{
	NULL,
	NULL,
	getWLANCfgAutoRateFallBackEnabledValue,
	setWLANCfgAutoRateFallBackEnabledValue,
	getWLANCfgAutoRateFallBackEnabledAttribute, 
	setWLANCfgAutoRateFallBackEnabledAttribute, 
	NULL
};

static NodeFunc TotalBytesSent=
{
	NULL,
	NULL,
	getWLANCfgTotalBytesSentValue,
	NULL,
	getWLANCfgTotalBytesSentAttribute, 
	setWLANCfgTotalBytesSentAttribute, 
	NULL
};

static NodeFunc TotalBytesReceived=
{
	NULL,
	NULL,
	getWLANCfgTotalBytesReceivedValue,
	NULL,
	getWLANCfgTotalBytesReceivedAttribute, 
	setWLANCfgTotalBytesReceivedAttribute, 
	NULL
};

static NodeFunc TotalPacketsSent=
{
	NULL,
	NULL,
	getWLANCfgTotalPacketsSentValue,
	NULL,
	getWLANCfgTotalPacketsSentAttribute, 
	setWLANCfgTotalPacketsSentAttribute, 
	NULL
};

static NodeFunc TotalPacketsReceived=
{
	NULL,
	NULL,
	getWLANCfgTotalPacketsReceivedValue,
	NULL,
	getWLANCfgTotalPacketsReceivedAttribute, 
	setWLANCfgTotalPacketsReceivedAttribute, 
	NULL
};

static NodeFunc TotalAssociations=
{
	NULL,
	NULL,
	getWLANCfgTotalAssociationsValue,
	NULL,
	getWLANCfgTotalAssociationsAttribute, 
	setWLANCfgTotalAssociationsAttribute, 
	NULL
};

static NodeFunc AssociatedDeviceMACAddress=
{
	NULL,
	NULL,
	getAssociatedDeviceMACAddressValue,
	NULL,
	getAssociatedDeviceMACAddressAttribute, 
	setAssociatedDeviceMACAddressAttribute, 
	NULL
};

static NodeFunc AssociatedDeviceIPAddress=
{
	NULL,
	NULL,
	getAssociatedDeviceIPAddressValue,
	NULL,
	getAssociatedDeviceIPAddressAttribute, 
	setAssociatedDeviceIPAddressAttribute, 
	NULL
};

static NodeFunc AssociatedDeviceAuthenticationState=
{
	NULL,
	NULL,
	getAssociatedDeviceAuthenticationStateValue,
	NULL,
	getAssociatedDeviceAuthenticationStateAttribute, 
	setAssociatedDeviceAuthenticationStateAttribute, 
	NULL
};


#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
static NodeFunc Name=
{
	NULL,
	NULL,
	getWlanCfgNameValue,
	NULL,
	getWlanCfgNameAttribute, 
	setWlanCfgNameAttribute, 
	NULL
};

static NodeFunc DualBandSupport=
{
	NULL,
	NULL,
	getWlanCfgDualBandSupportValue,
	NULL,
	getWlanCfgDualBandSupportAttribute, 
	setWlanCfgDualBandSupportAttribute, 
	NULL
};

static NodeFunc WlanKeyPassphrase=
{
	NULL,
	NULL,
	getWLANCfgKeyPassphraseValue,
	setWLANCfgKeyPassphraseValue,
	getWLANCfgKeyPassphraseAttribute, 
	setWLANCfgKeyPassphraseAttribute, 
	NULL,
};

static NodeFunc PowerSupported=
{
	NULL,
	NULL,
	getWlanCfgPowerSupportedValue,
	NULL,
	getWlanCfgPowerSupportedAttribute, 
	setWlanCfgPowerSupportedAttribute, 
	NULL,
};

static NodeFunc WMMSupported=
{
	NULL,
	NULL,
	getWlanCfgWMMSupportedValue,
	NULL,
	getWlanCfgWMMSupportedAttribute, 
	setWlanCfgWMMSupportedAttribute, 
	NULL,
};

static NodeFunc UAPSDSupported=
{
	NULL,
	NULL,
	getWlanCfgUAPSDSupportedValue,
	NULL,
	getWlanCfgUAPSDSupportedAttribute, 
	setWlanCfgUAPSDSupportedAttribute, 
	NULL,
};

static NodeFunc WMMEnable=
{
	NULL,
	NULL,
	getWlanCfgWMMEnableValue,
	setWlanCfgWMMEnableValue,
	getWlanCfgWMMEnableAttribute, 
	setWlanCfgWMMEnableAttribute, 
	NULL,
};

static NodeFunc UAPSDEnable=
{
	NULL,
	NULL,
	getWlanCfgUAPSDEnableValue,
	setWlanCfgUAPSDEnableValue,
	getWlanCfgUAPSDEnableAttribute, 
	setWlanCfgUAPSDEnableAttribute, 
	NULL,
};

static NodeFunc PreShareKeyPassphrase=
{
	NULL,
	NULL,
	getWlanCfgPreShareKeyPassphraseValue,
	setWlanCfgPreShareKeyPassphraseValue,
	getWlanCfgPreShareKeyPassphraseAttribute, 
	setWlanCfgPreShareKeyPassphraseAttribute, 
	NULL,
};
#endif


#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
static NodeFunc CTComRFBand=
{
	NULL,
	NULL,
	getWLANCfgRFBandValue,
	setWLANCfgRFBandValue,
	getWLANCfgRFBandAttribute,
	setWLANCfgRFBandAttribute,
	NULL
};

static NodeFunc CTComChannelWidth=
{
	NULL,
	NULL,
	getWLANCfgChannelWidthValue,
	setWLANCfgChannelWidthValue,
	getWLANCfgChannelWidthAttribute,
	setWLANCfgChannelWidthAttribute,
	NULL
};

static NodeFunc CTComGuardInterval=
{
	NULL,
	NULL,
	getWLANCfgGuardIntervalValue,
	setWLANCfgGuardIntervalValue,
	getWLANCfgGuardIntervalAttribute,
	setWLANCfgGuardIntervalAttribute,
	NULL
};

static NodeFunc CTComRetryTimeout=
{
	NULL,
	NULL,
	getWLANCfgRetryTimeoutValue,
	setWLANCfgRetryTimeoutValue,
	getWLANCfgRetryTimeoutAttribute,
	setWLANCfgRetryTimeoutAttribute,
	NULL
};


#endif

/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}.AssociatedDevice.{i}.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
CwmpParameterStruct AssociatedDevice[] = 
{
	{"AssociatedDeviceMACAddress", StrR, NoLimite, &AssociatedDeviceMACAddress, NULL},
	{"AssociatedDeviceIPAddress", StrR, NoLimite, &AssociatedDeviceIPAddress, NULL},
	{"AssociatedDeviceAuthenticationState", BooleanR, NoLimite, &AssociatedDeviceAuthenticationState, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}.AssociatedDevice.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
CwmpParameterStruct AssociatedDeviceIndex[]= 
{
	{NULL, ObjectR, NoLimite, NULL, AssociatedDevice},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
static NodeFunc AssociatedDeviceFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitAssociatedDeviceTable,
};

static NodeFunc WEPKey=
{
	NULL,
	NULL,
	getWEPKeyValue,
	setWEPKeyValue,
	getWEPKeyAttribute, 
	setWEPKeyAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}.WEPKey.{i}.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
CwmpParameterStruct WEPKeyArray[] = 
{
	{"WEPKey", StrW, NoLimite, &WEPKey, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}.WEPKey.{i}
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}.WEPKey.
	Parent Node Table Location: cp_landevice_nodetable.c
*/

CwmpParameterStruct WEPKeyeIndex[]= 
{
	{NULL, ObjectR, NoLimite, NULL, WEPKeyArray},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
static NodeFunc WEPKeyFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitWEPKeyTable
};

static NodeFunc PreSharedKey=
{
	NULL,
	NULL,
	getPreSharedKeyValue,
	setPreSharedKeyValue,
	getPreSharedKeyAttribute, 
	setPreSharedKeyAttribute, 
	NULL,
};

static NodeFunc BeaconAdvertisementEnabled=
{
	NULL,
	NULL,
	getBeaconAdvertisementEnabledValue,
	setBeaconAdvertisementEnabledValue,
	getBeaconAdvertisementEnabledAttribute, 
	setBeaconAdvertisementEnabledAttribute, 
	NULL,
};


/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}.PreSharedKey.{i}.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
CwmpParameterStruct PreSharedKeyArray[] = 
{
	{"PreSharedKey", StrW, NoLimite, &PreSharedKey, NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"KeyPassphrase", StrW, NoLimite, &PreShareKeyPassphrase, NULL},
#endif

	{NULL, -1, -1, NULL, NULL}
};

/*	
	This Node:    InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}.PreSharedKey.{i}
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}.PreSharedKey.
	Parent Node Table Location: cp_landevice_nodetable.c
*/

CwmpParameterStruct PreSharedKeyIndex[]= 
{
	{NULL, ObjectR, NoLimite, NULL, PreSharedKeyArray},
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
	cwmpInitPreSharedKey
};

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
static NodeFunc WLanCfg_X_CTCOM_Mode=
{
	NULL,
	NULL,
	getWLanCfg_X_CTCOM_ModeValue,
	setWLanCfg_X_CTCOM_ModeValue,
	getWLanCfg_X_CTCOM_ModeAttribute, 
	setWLanCfg_X_CTCOM_ModeAttribute, 
	NULL,
};

static NodeFunc WLanCfg_X_CTCOM_VLAN=
{
	NULL,
	NULL,
	getWLanCfg_X_CTCOM_VLANValue,
	setWLanCfg_X_CTCOM_VLANValue,
	getWLanCfg_X_CTCOM_VLANAttribute, 
	setWLanCfg_X_CTCOM_VLANAttribute, 
	NULL,
};

static NodeFunc WLanCfg_DetectionStatus=
{
	NULL,
	NULL,
	getWLanCfg_DetectionStatusValue,
	NULL,
	getWLanCfg_DetectionStatusAttribute, 
	setWLanCfg_DetectionStatusAttribute, 
	NULL,
};
#endif

#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
static NodeFunc SSIDMaxUserNum=
{
	NULL,
	NULL,
	getSSIDMaxUserNumValue,
	setSSIDMaxUserNumValue,
	getSSIDMaxUserNumAttribute, 
	setSSIDMaxUserNumAttribute, 
	NULL,
};	
#endif

#if defined(TCSUPPORT_CT_PMINFORM)
static NodeFunc WlanAccessEquipmentMac=
{
	NULL,
	NULL, 
	getWlanAccessEquipmentMacValue,
	NULL, 
	getWlanAccessEquipmentMacAttribute, 
	setWlanAccessEquipmentMacAttribute, 
	NULL,
};	
#endif

#ifdef WSC_AP_SUPPORT

static NodeFunc WPSEnable=
{
	NULL,
	NULL,
	getWPSEnableValue,
	setWPSEnableValue,
	getWPSEnableAttribute, 
	setWPSEnableAttribute, 
	NULL,
};

static NodeFunc DeviceName=
{
	NULL,
	NULL,
	getWPSDeviceNameValue,
	NULL,
	getWPSDeviceNameAttribute, 
	setWPSDeviceNameAttribute, 
	NULL,
};

static NodeFunc DevicePassword=
{
	NULL,
	NULL,
	getWPSDevicePasswordValue,
	setWPSDevicePasswordValue,
	getWPSDevicePasswordAttribute, 
	setWPSDevicePasswordAttribute, 
	NULL,
};

static NodeFunc UUID=
{
	NULL,
	NULL,
	getWPSUUIDValue,
	NULL,
	getWPSUUIDAttribute, 
	setWPSUUIDAttribute, 
	NULL,
};

static NodeFunc WPSVersion=
{
	NULL,
	NULL,
	getWPSVersionValue,
	NULL,
	getWPSVersionAttribute, 
	setWPSVersionAttribute, 
	NULL,
};

static NodeFunc ConfigMethodsSupported=
{
	NULL,
	NULL,
	getWPSConfigMethodsSupportedValue,
	NULL,
	getWPSConfigMethodsSupportedAttribute, 
	setWPSConfigMethodsSupportedAttribute, 
	NULL,
};

static NodeFunc ConfigMethodsEnabled=
{
	NULL,
	NULL,
	getWPSConfigMethodsEnabledValue,
	setWPSConfigMethodsEnabledValue,
	getWPSConfigMethodsEnabledAttribute, 
	setWPSConfigMethodsEnabledAttribute, 
	NULL,
};

static NodeFunc SetupLockedState=
{
	NULL,
	NULL,
	getWPSSetupLockedStateValue,
	NULL,
	getWPSSetupLockedStateAttribute, 
	setWPSSetupLockedStateAttribute, 
	NULL,
};

static NodeFunc SetupLock=
{
	NULL,
	NULL,
	getWPSSetupLockValue,
	setWPSSetupLockValue,
	getWPSSetupLockAttribute, 
	setWPSSetupLockAttribute, 
	NULL,
};

static NodeFunc ConfigurationState=
{
	NULL,
	NULL,
	getWPSConfigurationStateValue,
	NULL,
	getWPSConfigurationStateAttribute, 
	setWPSConfigurationStateAttribute, 
	NULL,
};

static NodeFunc LastConfigurationError=
{
	NULL,
	NULL,
	getWPSLastConfigurationErrorValue,
	NULL,
	getWPSLastConfigurationErrorAttribute, 
	setWPSLastConfigurationErrorAttribute, 
	NULL,
};

static NodeFunc RegistrarNumberOfEntries=
{
	NULL,
	NULL,
	getWPSRegistrarNumberOfEntriesValue,
	NULL,
	getWPSRegistrarNumberOfEntriesAttribute, 
	setWPSRegistrarNumberOfEntriesAttribute, 
	NULL,
};

static NodeFunc RegistrarEstablished=
{
	NULL,
	NULL,
	getWPSRegistrarEstablishedValue,
	NULL,
	getWPSRegistrarEstablishedAttribute, 
	setWPSRegistrarEstablishedAttribute, 
	NULL,
};

static NodeFunc RegEnable=
{
	NULL,
	NULL,
	getRegistrarEnableValue,
	setRegistrarEnableValue,
	getRegistrarEnableAttribute, 
	setRegistrarEnableAttribute, 
	NULL,
};

static NodeFunc RegUUID=
{
	NULL,
	NULL,
	getRegistrarUUIDValue,
	NULL,
	getRegistrarUUIDAttribute, 
	setRegistrarUUIDAttribute, 
	NULL,
};

static NodeFunc RegDeviceName=
{
	NULL,
	NULL,
	getRegistrarDeviceNameValue,
	NULL,
	getRegistrarDeviceNameAttribute, 
	setRegistrarDeviceNameAttribute, 
	NULL,
};

CwmpParameterStruct Registrar[] = 
{
	{"Enable", 				BooleanW, 		NoLimite, 		&RegEnable, 		NULL},
	{"UUID", 				StrR, 			NoLimite, 		&RegUUID, 			NULL},
	{"DeviceName", 			StrR, 			NoLimite, 		&RegDeviceName, 	NULL},
	{NULL, -1, -1, NULL, NULL}
};
	
CwmpParameterStruct RegistrarIndex[]= 
{
	{NULL, ObjectR, NoLimite, NULL, Registrar},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc RegistrarFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitWPSRegistrar
};
#if defined(TCSUPPORT_CT_FJ)
static NodeFunc ErrorsSent_WlanInfo=
{
	NULL,
	NULL,
	getErrorsSentValue, 
	NULL, 
	getErrorsSentAttribute, 
	setErrorsSentAttribute, 
	NULL,
};
static NodeFunc ErrorsReceived_WlanInfo=
{
	NULL,
	NULL,
	getErrorsReceivedValue, 
	NULL, 
	getErrorsReceivedAttribute, 
	setErrorsReceivedAttribute, 
	NULL,
};
static NodeFunc DiscardPacketsSent_WlanInfo=
{
	NULL,
	NULL,
	getDiscardPacketsSentValue, 
	NULL, 
	getDiscardPacketsSentAttribute, 
	setDiscardPacketsSentAttribute, 
	NULL,
};
CwmpParameterStruct Stats_WlanInfo[]=
{
	{"ErrorsSent", StrR, NoLimite, &ErrorsSent_WlanInfo, NULL},
	{"ErrorsReceived", StrR, NoLimite, &ErrorsReceived_WlanInfo, NULL},
	{"DiscardPacketsSent", StrR, NoLimite, &DiscardPacketsSent_WlanInfo, NULL},
	{NULL, -1, -1, NULL, NULL}
};	
#endif

/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}.WPS
	Parent Node Table Location: cp_landevice_wlan_nodetable.c
*/
CwmpParameterStruct WPS[] =
{
	{"Enable", 						BooleanW, 		NoLimite, 		&WPSEnable, 					NULL},
	{"DeviceName", 					StrR, 			NoLimite, 		&DeviceName, 					NULL},
	{"DevicePassword", 		 		UnsignIntW, 	NoLimite, 		&DevicePassword, 				NULL},
	{"UUID", 						StrR, 			NoLimite, 		&UUID, 							NULL},
	{"Version",  					UnsignIntR, 	NoLimite, 		&WPSVersion, 					NULL},
	{"ConfigMethodsSupported", 		StrR, 			NoLimite, 		&ConfigMethodsSupported, 		NULL},
	{"ConfigMethodsEnabled", 		StrW, 			NoLimite, 		&ConfigMethodsEnabled, 			NULL},
	{"SetupLockedState", 			StrR, 			NoLimite, 		&SetupLockedState, 				NULL},
	{"SetupLock", 					BooleanW, 		NoLimite, 		&SetupLock, 					NULL},
	{"ConfigurationState", 			StrR, 			NoLimite, 		&ConfigurationState, 			NULL},
	{"LastConfigurationError", 		StrR, 			NoLimite, 		&LastConfigurationError, 		NULL},
	{"RegistrarNumberOfEntries",	UnsignIntR, 	NoLimite, 		&RegistrarNumberOfEntries, 		NULL},
	{"RegistrarEstablished",		BooleanR, 		NoLimite, 		&RegistrarEstablished, 			NULL},
	{"Registrar", 					ObjectR,	 	NoLimite, 		&RegistrarFuncs, 				NULL},
	{NULL, -1, -1, NULL, NULL}
};
#endif
/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
CwmpParameterStruct WLANConfiguration[] = 
{
	{"Enable", BooleanW, NoLimite, &Enable, NULL},	
	{"Status", StrR, NoLimite, &Status, NULL},
	{"BSSID", StrR, NoLimite, &BSSID, NULL},
	{"MaxBitRate", StrW, NoLimite, &MaxBitRate, NULL},
	{"Channel", UnsignIntW, NoLimite, &Channel, NULL},
	/*support this node in Linux that not support in Linos / 20100310*/
	{"AutoChannelEnable", BooleanW, NoLimite, &AutoChannelEnable, NULL},
	{"SSID", StrW, NoLimite, &SSID, NULL},
	{"BeaconType", StrW, NoLimite, &BeaconType, NULL},
	{"MACAddressControlEnabled", BooleanW, NoLimite, &MACAddressControlEnabled, NULL},	
#if defined(TCSUPPORT_CT_WLAN_NODE)
#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"TransmitPowerSupported", StrR, NoLimite, &PowerSupported, NULL},
	{"TransmitPower", UnsignIntW, NoLimite, &Powerlevel, NULL},
	{"SSIDAdvertisementEnable", BooleanW, NoLimite, &SSIDHide, NULL},
#else
	{X_CT_COM"SSIDHide", BooleanW, NoLimite, &SSIDHide, NULL},
	{X_CT_COM"Powerlevel", UnsignIntW, NoLimite, &Powerlevel, NULL},
#endif
	{X_CT_COM"PowerValue", UnsignIntR, NoLimite, &PowerValue, NULL},
	{X_CT_COM"APModuleEnable", BooleanW, NoLimite, &APModuleEnable, NULL},
	{X_CT_COM"WPSKeyWord", UnsignIntW, NoLimite, &WPSKeyWord, NULL},
#endif
	{"ChannelsInUse", StrR, NoLimite, &ChannelsInUse, NULL},	
#endif
#if defined(TCSUPPORT_CT_WLAN_NODE) && !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"Standard", StrW, NoLimite, &Standard, NULL},
	{"WEPEncryptionLevel", StrW, NoLimite, &WEPEncryptionLevel, NULL},
#else
	{"Standard", StrR, NoLimite, &Standard, NULL},
	{"WEPEncryptionLevel", StrR, NoLimite, &WEPEncryptionLevel, NULL},
#endif
	{"WEPKeyIndex", UnsignIntW, NoLimite, &WEPKeyIndex, NULL},
	{"BasicEncryptionModes", StrW, NoLimite, &BasicEncryptionModes, NULL},
	{"BasicAuthenticationMode", StrW, NoLimite, &BasicAuthenticationMode, NULL},
	{"WPAEncryptionModes", StrW, NoLimite, &WPAEncryptionModes, NULL},
	{"WPAAuthenticationMode", StrW, NoLimite, &WPAAuthenticationMode, NULL},
	{"IEEE11iEncryptionModes", StrW, NoLimite, &IEEE11iEncryptionModes, NULL},
	{"IEEE11iAuthenticationMode", StrW, NoLimite, &IEEE11iAuthenticationMode, NULL},
	{"PossibleChannels", StrR, NoLimite, &PossibleChannels, NULL},
	{"BasicDataTransmitRates", StrW, NoLimite, &BasicDataTransmitRates, NULL},
	{"OperationalDataTransmitRates", StrW, NoLimite, &BasicDataTransmitRates, NULL},
	{"PossibleDataTransmitRates", StrR, NoLimite, &PossibleDataTransmitRates, NULL},
	{"RadioEnabled", BooleanW, NoLimite, &RadioEnabled, NULL},
	
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CT_COM"AutoRateFallBackEnabled", BooleanW, NoLimite, &AutoRateFallBackEnabled, NULL},
#else
	{"AutoRateFallBackEnabled", BooleanW, NoLimite, &AutoRateFallBackEnabled, NULL},
#endif
	{"TotalBytesSent", UnsignIntR, NoLimite, &TotalBytesSent, NULL},
	{"TotalBytesReceived", UnsignIntR, NoLimite, &TotalBytesReceived, NULL},
	{"TotalPacketsSent", UnsignIntR, NoLimite, &TotalPacketsSent, NULL},
	{"TotalPacketsReceived", UnsignIntR, NoLimite, &TotalPacketsReceived, NULL},
	{"TotalAssociations", UnsignIntR, NoLimite, &TotalAssociations, NULL},	
	{"AssociatedDevice", ObjectR, NoLimite, &AssociatedDeviceFuncs, NULL},	
	{"WEPKey", ObjectR, NoLimite, &WEPKeyFuncs, NULL},
	{"PreSharedKey", ObjectR, NoLimite, &PreSharedKeyFuncs, NULL},
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CT_COM"Band", StrW, NoLimite, &CTComRFBand, NULL},
#else
	{X_CT_COM"RFBand", UnsignIntW, NoLimite, &CTComRFBand, NULL},
#endif
	{X_CT_COM"ChannelWidth", UnsignIntW, NoLimite, &CTComChannelWidth, NULL},
	{X_CT_COM"GuardInterval", UnsignIntW, NoLimite, &CTComGuardInterval, NULL},
	{X_CT_COM"RetryTimeout", UnsignIntW, NoLimite, &CTComRetryTimeout, NULL},
	{X_CT_COM"Mode", UnsignIntW, NoLimite, &WLanCfg_X_CTCOM_Mode, NULL},
	{X_CT_COM"VLAN", StrW, NoLimite, &WLanCfg_X_CTCOM_VLAN, NULL},
	{X_CT_COM"DetectionStatus", UnsignIntR, NoLimite, &WLanCfg_DetectionStatus, NULL},
#endif
#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
	{"MaxUserNum", UnsignIntW, NoLimite, &SSIDMaxUserNum, NULL},	
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"Name", StrR, NoLimite, &Name, NULL},
	{X_CT_COM"Dual_bandsupport", BooleanR, NoLimite, &DualBandSupport, NULL},
	{"KeyPassphrase", StrW, NoLimite, &WlanKeyPassphrase, NULL},
	{"WMMSupported", BooleanR, NoLimite, &WMMSupported, NULL},
	{"UAPSDSupported", BooleanR, NoLimite, &UAPSDSupported, NULL},
	{"WMMEnable", BooleanW, NoLimite, &WMMEnable, NULL},
	{"UAPSDEnable", BooleanW, NoLimite, &UAPSDEnable, NULL},
#endif
#if defined(TCSUPPORT_CT_FJ)
	{"Stats", ObjectR, NoLimite, NULL, Stats_WlanInfo},
#endif
	{"BeaconAdvertisementEnabled", BooleanW, NoLimite, &BeaconAdvertisementEnabled, NULL},
#if defined(TCSUPPORT_CT_PMINFORM)
	{"AccessEquipmentMac", StrR, NoLimite, &WlanAccessEquipmentMac, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};

#ifdef WSC_AP_SUPPORT
CwmpParameterStruct WPSTable[] = 
{
	{"WPS", ObjectR, NoLimite, NULL, WPS},
	{NULL, -1, -1, NULL, NULL}
};
#endif

/*
      This Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.
	Parent Node Table Location: cp_landevice_nodetable.c
*/


