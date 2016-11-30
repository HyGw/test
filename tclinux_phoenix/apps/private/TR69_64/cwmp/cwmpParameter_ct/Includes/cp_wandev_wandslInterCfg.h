#ifndef CP_WANDEV_WANDSLINTERCFG_H_
#define CP_WANDEV_WANDSLINTERCFG_H_

#include "cp_core.h"

#if defined(TCSUPPORT_CT_ADSLSETTING)
#define ADSLPREFIXNAME "Annex_"
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#define ADSL_NODE_ENTRY "Adsl_Entry"
#define ADSL_ACTIVE "Active"
#define ADSL_LINEENCAPSULATION "LinkEncapsulation"
#define ADSL_ATTR_MODE	"MODULATIONTYPE"

#if defined(TCSUPPORT_CT_WAN_PTM)
extern CwmpParameterStruct WandslInterCfg_TestParams[];
#endif
#endif


/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLCommonInterfaceConfig.
	Parent Node Table Location: cp_wandslInterCfg_nodetable.c
*/
extern CwmpParameterStruct WandslInterCfg_Stats[];

/*----------------------dsl interface config function begin----------------------*/
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getWANDSLInterCfg_EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLInterCfg_EnableValue(void* value, struct DeviceNode* deviceNode);
int8 getWANDSLInterCfg_EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLInterCfg_EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLInterCfg_LinkEncapsulationSupportedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLInterCfg_LinkEncapsulationSupportedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLInterCfg_LinkEncapsulationSupportedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLInterCfg_LinkEncapsulationUsedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLInterCfg_LinkEncapsulationUsedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLInterCfg_LinkEncapsulationUsedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLInterCfg_LinkEncapsulationRequestedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLInterCfg_LinkEncapsulationRequestedValue(void* value, struct DeviceNode* deviceNode);
int8 getWANDSLInterCfg_LinkEncapsulationRequestedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLInterCfg_LinkEncapsulationRequestedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getStandardsSupportedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	getStandardsSupportedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setStandardsSupportedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getAllowedProfilesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	getAllowedProfilesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setAllowedProfilesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getCurrentProfilesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	getCurrentProfilesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setCurrentProfilesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getUPBOKLEValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	getUPBOKLEAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setUPBOKLEAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getTRELLISdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	getTRELLISdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setTRELLISdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getTRELLISusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	getTRELLISusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setTRELLISusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getACTSNRMODEdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	getACTSNRMODEdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setACTSNRMODEdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getACTSNRMODEusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	getACTSNRMODEusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setACTSNRMODEusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getACTUALCEValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	getACTUALCEAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setACTUALCEAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getSNRMpbdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	getSNRMpbdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setSNRMpbdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getSNRMpbusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	getSNRMpbusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setSNRMpbusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if !defined(TCSUPPORT_CT_PON)
int8 getModulationModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setModulationModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getModulationModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setModulationModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#endif

int8 getWANDSLInterCfg_StatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLInterCfg_StatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLInterCfg_StatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getModulationTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getModulationTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setModulationTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineEncodingValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLineEncodingAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineEncodingAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDataPathValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDataPathAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDataPathAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUpstreamCurrRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUpstreamCurrRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUpstreamCurrRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDownstreamCurrRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDownstreamCurrRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDownstreamCurrRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUpstreamMaxRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUpstreamMaxRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUpstreamMaxRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDownstreamMaxRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDownstreamMaxRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDownstreamMaxRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUpstreamNoiseMarginValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUpstreamNoiseMarginAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUpstreamNoiseMarginAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDownstreamNoiseMarginValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDownstreamNoiseMarginAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDownstreamNoiseMarginAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUpstreamAttenuationValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUpstreamAttenuationAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUpstreamAttenuationAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDownstreamAttenuationValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDownstreamAttenuationAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDownstreamAttenuationAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUpstreamPowerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUpstreamPowerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUpstreamPowerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDownstreamPowerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDownstreamPowerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDownstreamPowerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATURVendorValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATURVendorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATURVendorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATURCountryValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATURCountryAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATURCountryAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATUCVendorValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATUCVendorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATUCVendorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATUCCountryValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATUCCountryAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATUCCountryAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getShowtimeStartValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getShowtimeStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setShowtimeStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTotalStartValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getTotalStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTotalStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInterleaveDepthValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getInterleaveDepthAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInterleaveDepthAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATURANSIStdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATURANSIStdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATURANSIStdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATURANSIRevValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATURANSIRevAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATURANSIRevAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATUCANSIStdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATUCANSIStdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATUCANSIStdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATUCANSIRevValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATUCANSIRevAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATUCANSIRevAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*--------------------dsl interface config relation function end--------------------------------------*/

#if defined(TCSUPPORT_CT_ADSLSETTING) || defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8	getStandardUsedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
#if !defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8	setStandardUsedValue(void* value, struct DeviceNode*  deviceNode);
#endif
int8	getStandardUsedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setStandardUsedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
/*--------------------dsl common interface config function end-----------------------------*/
//add by renyu for dsl interface config
uint8 Get_MultiTen_Value(char *str, char *strResult);
uint8 Get_Value_WithoutKbps(char *origin, char*result);
#endif

