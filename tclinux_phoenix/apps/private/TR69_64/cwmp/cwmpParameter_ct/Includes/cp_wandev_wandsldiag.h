#ifndef CP_WANDEV_WANDSLDIAG_H_
#define CP_WANDEV_WANDSLDIAG_H_

#include "cp_core.h"

#if !defined(TCSUPPORT_CT_PON)

/*----------------------dsl diagnostic relation function begin----------------------*/
int8 getWANDSLDiagnositcs_LoopDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLDiagnositcs_LoopDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_LoopDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_LoopDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_ACTPSDdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_ACTPSDdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_ACTPSDdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_ACTPSDusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_ACTPSDusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_ACTPSDusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_ACTATPdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_ACTATPdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_ACTATPdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_ACTATPusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_ACTATPusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_ACTATPusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_HLINSCdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_HLINSCdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_HLINSCdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_HLINpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_HLINpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_HLINpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_QLNpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_QLNpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_QLNpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_SNRpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_SNRpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_SNRpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_BITSpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_BITSpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_BITSpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_GAINSpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_GAINSpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_GAINSpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT_WAN_PTM)
int8 getWANDSLDiagnositcs_HLINSCusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_HLINSCusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_HLINSCusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_HLINGdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_HLINGdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_HLINGdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_HLINGusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_HLINGusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_HLINGusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_HLINpsusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_HLINpsusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_HLINpsusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_HLOGGdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_HLOGGdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_HLOGGdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_HLOGGusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_HLOGGusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_HLOGGusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_HLOGpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_HLOGpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_HLOGpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_HLOGpsusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_HLOGpsusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_HLOGpsusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_HLOGMTdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_HLOGMTdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_HLOGMTdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_HLOGMTusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_HLOGMTusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_HLOGMTusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_QLNGdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_QLNGdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_QLNGdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_QLNGusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_QLNGusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_QLNGusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_QLNpsusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_QLNpsusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_QLNpsusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_QLNMTdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_QLNMTdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_QLNMTdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_QLNMTusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_QLNMTusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_QLNMTusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_SNRGdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_SNRGdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_SNRGdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_SNRGusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_SNRGusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_SNRGusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_SNRpsusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_SNRpsusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_SNRpsusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_SNRMTdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_SNRMTdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_SNRMTdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_SNRMTusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_SNRMTusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_SNRMTusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_LATNpbdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_LATNpbdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_LATNpbdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_LATNpbusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_LATNpbusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_LATNpbusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_SATNdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_SATNdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_SATNdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_SATNusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_SATNusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_SATNusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

/*--------------------dsl diagnostic relation function end-----------------------------*/

//add by brian for dsl diagnostic
//static void resetCWMPPingResult(void);
//static void doCwmpIPPingError(void);
int8 NT_ADSL_def_Diagnostics_poll(char *attrName,char *p_diagnostic_set_buf);

#endif

#endif

