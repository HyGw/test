#ifndef CP_LANDEV_WLAN_H_
#define CP_LANDEV_WLAN_H_
#include "cp_core.h"
//#ifdef RT3390
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7592)
#define  PHY_MODE_BG_MIXED		0
#define	 PHY_MODE_B_ONLY		1
#define	 PHY_MODE_G_ONLY		4
#define	 PHY_MODE_N_ONLY		6
#define	 PHY_MODE_GN_MIXED		7
#define	 PHY_MODE_BGN_MIXED		9
#else
#define  PHY_MODE_BG_MIXED		0
#define	 PHY_MODE_B_ONLY		1
#define	 PHY_MODE_G_ONLY		2
#endif

#ifdef WSC_AP_SUPPORT
extern CwmpParameterStruct RegistrarIndex[];

#define WPS_FILE				"/tmp/wpsInfo"
#define WSC_DISABLE 			"0"

#if defined(RT3390) || defined(RT5392) || defined(MT7592)
void getWpsInfo( char *ioctlCmd,  /*ioctl cmd like get_wps_regs_dyn_info*/
				 char *keyword,   /*get value accoding to this keyword like DeviceName = */
				 char *info,       /*return value*/
				 int infoLen
			   );
#endif

#endif

#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
#if (defined(RT3390) && defined(VENDOR_FEATURE3_SUPPORT)) || (defined(RT5392) && defined(BB_SOC))
#define SSID_MAX_STA_NUM 8
#else
#define SSID_MAX_STA_NUM 32
#endif
#endif


/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}.AssociatedDevice.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
extern CwmpParameterStruct AssociatedDeviceIndex[];

/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}.WEPKey.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
extern CwmpParameterStruct WEPKeyeIndex[];
/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}.PreSharedKey.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
extern CwmpParameterStruct PreSharedKeyIndex[];

int cwmpInitAssociatedDeviceTable( DeviceNodePtr deviceNode);
int cwmpInitWEPKeyTable( DeviceNodePtr deviceNode );
int cwmpInitPreSharedKey( DeviceNodePtr deviceNode );

#if !defined(TCSUPPORT_C1_CUC)
int8 getWLANCfgEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CT_WLAN_NODE)
#if !defined(TCSUPPORT_C1_CUC)
int8 getWLANCfgSSIDHideValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgSSIDHideValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgSSIDHideAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgSSIDHideAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

int8 getWLANCfgSSIDEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgSSIDEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgSSIDEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgSSIDEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if !defined(TCSUPPORT_C1_CUC)
int8 getWLANCfgWPSKeyWordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgWPSKeyWordValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgWPSKeyWordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgWPSKeyWordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgPowerlevelValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgPowerlevelValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgPowerlevelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgPowerlevelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgPowerValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgPowerValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgPowerValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

int8 getWLANCfgChannelsInUseValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgChannelsInUseAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgChannelsInUseAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

int8 getWLANCfgStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgBSSIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgBSSIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgBSSIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgMaxBitRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgMaxBitRateValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgMaxBitRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgMaxBitRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgChannelValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgChannelValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgChannelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgChannelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getWLANCfgAutoChannelEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgAutoChannelEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgAutoChannelEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgAutoChannelEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgSSIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgSSIDValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgSSIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgSSIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgBeaconTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgBeaconTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgBeaconTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgBeaconTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgMACAddressControlEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgMACAddressControlEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgMACAddressControlEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgMACAddressControlEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgStandardValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
#if defined(TCSUPPORT_CT_WLAN_NODE)
int8 setWLANCfgStandardValue(void* value, struct DeviceNode*  deviceNode);
#endif
int8 getWLANCfgStandardAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgStandardAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgWEPKeyIndexValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgWEPKeyIndexValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgWEPKeyIndexAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgWEPKeyIndexAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getWLANCfgWEPEncryptionLevelValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
#if defined(TCSUPPORT_CT_WLAN_NODE)
int8 setWLANCfgWEPEncryptionLevelValue(void* value, struct DeviceNode*  deviceNode);
#endif
int8 getWLANCfgWEPEncryptionLevelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgWEPEncryptionLevelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgBasicEncryptionModesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgBasicEncryptionModesValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgBasicEncryptionModesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgBasicEncryptionModesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgBasicAuthenticationModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgBasicAuthenticationModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgBasicAuthenticationModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgBasicAuthenticationModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgWPAEncryptionModesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgWPAEncryptionModesValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgWPAEncryptionModesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgWPAEncryptionModesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgWPAAuthenticationModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgWPAAuthenticationModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgWPAAuthenticationModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgWPAAuthenticationModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getWLANCfgIEEE11iEncryptionModesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgIEEE11iEncryptionModesValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgIEEE11iEncryptionModesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgIEEE11iEncryptionModesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgIEEE11iAuthenticationModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgIEEE11iAuthenticationModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgIEEE11iAuthenticationModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgIEEE11iAuthenticationModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgPossibleChannelsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgPossibleChannelsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgPossibleChannelsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgBasicDataTransmitRatesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgBasicDataTransmitRatesValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgBasicDataTransmitRatesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgBasicDataTransmitRatesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgOperationalDataTransmitRatesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgOperationalDataTransmitRatesValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgOperationalDataTransmitRatesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgOperationalDataTransmitRatesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgPossibleDataTransmitRatesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgPossibleDataTransmitRatesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgPossibleDataTransmitRatesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgRadioEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgRadioEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgRadioEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgRadioEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgAutoRateFallBackEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgAutoRateFallBackEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgAutoRateFallBackEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgAutoRateFallBackEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgTotalBytesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgTotalBytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgTotalBytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgTotalBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgTotalBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgTotalBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgTotalPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgTotalPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgTotalPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgTotalPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgTotalPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgTotalPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgTotalAssociationsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgTotalAssociationsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgTotalAssociationsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

//#ifndef CD_WEBPAGE
int8 getAssociatedDeviceMACAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAssociatedDeviceMACAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAssociatedDeviceMACAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAssociatedDeviceIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAssociatedDeviceIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAssociatedDeviceIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAssociatedDeviceAuthenticationStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAssociatedDeviceAuthenticationStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAssociatedDeviceAuthenticationStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
//#endif

int8 getWEPKeyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWEPKeyValue(void* value, struct DeviceNode*  deviceNode);
int8 getWEPKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWEPKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPreSharedKeyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPreSharedKeyValue(void* value, struct DeviceNode*  deviceNode);
int8 getPreSharedKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPreSharedKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getBeaconAdvertisementEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setBeaconAdvertisementEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getBeaconAdvertisementEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNodePtr deviceNode);
int8 setBeaconAdvertisementEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNodePtr deviceNode);

#ifdef WSC_AP_SUPPORT
int8 getWPSEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWPSEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWPSEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWPSEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWPSDeviceNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWPSDeviceNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWPSDeviceNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWPSDevicePasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWPSDevicePasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getWPSDevicePasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWPSDevicePasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWPSUUIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWPSUUIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWPSUUIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWPSVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWPSVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWPSVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWPSConfigMethodsSupportedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWPSConfigMethodsSupportedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWPSConfigMethodsSupportedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWPSConfigMethodsEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWPSConfigMethodsEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getWPSConfigMethodsEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWPSConfigMethodsEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWPSSetupLockedStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWPSSetupLockedStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWPSSetupLockedStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWPSSetupLockValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWPSSetupLockValue(void* value, struct DeviceNode*  deviceNode);
int8 getWPSSetupLockAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWPSSetupLockAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWPSConfigurationStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWPSConfigurationStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWPSConfigurationStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWPSLastConfigurationErrorValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWPSLastConfigurationErrorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWPSLastConfigurationErrorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWPSRegistrarNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWPSRegistrarNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWPSRegistrarNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWPSRegistrarEstablishedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWPSRegistrarEstablishedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWPSRegistrarEstablishedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int cwmpInitWPSRegistrar( DeviceNodePtr deviceNode );

int8 getRegistrarEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRegistrarEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getRegistrarEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRegistrarEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRegistrarUUIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getRegistrarUUIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRegistrarUUIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRegistrarDeviceNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getRegistrarDeviceNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRegistrarDeviceNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getWLANCfgRFBandValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgRFBandValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgRFBandAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgRFBandAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgChannelWidthValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgChannelWidthValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgChannelWidthAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgChannelWidthAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgGuardIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgGuardIntervalValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgGuardIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgGuardIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgRetryTimeoutValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgRetryTimeoutValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgRetryTimeoutAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgRetryTimeoutAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLanCfg_X_CTCOM_ModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLanCfg_X_CTCOM_ModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLanCfg_X_CTCOM_ModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLanCfg_X_CTCOM_ModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLanCfg_X_CTCOM_VLANValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLanCfg_X_CTCOM_VLANValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLanCfg_X_CTCOM_VLANAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLanCfg_X_CTCOM_VLANAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLanCfg_DetectionStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLanCfg_DetectionStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLanCfg_DetectionStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
int8 getSSIDMaxUserNumValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSSIDMaxUserNumValue(void* value, struct DeviceNode*  deviceNode);
int8 getSSIDMaxUserNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSSIDMaxUserNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CT_PMINFORM)
int8 getWlanAccessEquipmentMacValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWlanAccessEquipmentMacAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWlanAccessEquipmentMacAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getWlanCfgNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWlanCfgNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWlanCfgNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWlanCfgDualBandSupportValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWlanCfgDualBandSupportAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWlanCfgDualBandSupportAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgKeyPassphraseValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgKeyPassphraseValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgKeyPassphraseAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgKeyPassphraseAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWlanCfgPowerSupportedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWlanCfgPowerSupportedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWlanCfgPowerSupportedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWlanCfgWMMSupportedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWlanCfgWMMSupportedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWlanCfgWMMSupportedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWlanCfgUAPSDSupportedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWlanCfgUAPSDSupportedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWlanCfgUAPSDSupportedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWlanCfgWMMEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWlanCfgWMMEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWlanCfgWMMEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWlanCfgWMMEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWlanCfgUAPSDEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWlanCfgUAPSDEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWlanCfgUAPSDEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWlanCfgUAPSDEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWlanCfgPreShareKeyPassphraseValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWlanCfgPreShareKeyPassphraseValue(void* value, struct DeviceNode*  deviceNode);
int8 getWlanCfgPreShareKeyPassphraseAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWlanCfgPreShareKeyPassphraseAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#if defined(TCSUPPORT_CT_FJ)
int8 getErrorsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getErrorsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setErrorsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getErrorsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getErrorsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setErrorsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getDiscardPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDiscardPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDiscardPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#endif

