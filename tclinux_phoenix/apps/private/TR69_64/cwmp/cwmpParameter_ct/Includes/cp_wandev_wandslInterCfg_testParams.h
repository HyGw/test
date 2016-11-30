#ifndef CP_WANDEV_WANDSLINTERCFG_TESTPARAMS_H_
#define CP_WANDEV_WANDSLINTERCFG_TESTPARAMS_H_

#include "cp_core.h"
#include "cp_wandev_wandsldiag.h"

extern int8 getWANDSLDiagnositcs_HLOGGdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_HLOGGdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_HLOGGdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_HLOGGusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_HLOGGusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_HLOGGusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_HLOGpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_HLOGpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_HLOGpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_HLOGpsusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_HLOGpsusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_HLOGpsusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_HLOGMTdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_HLOGMTdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_HLOGMTdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_HLOGMTusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_HLOGMTusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_HLOGMTusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_QLNGdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_QLNGdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_QLNGdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_QLNGusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_QLNGusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_QLNGusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_QLNpsusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_QLNpsusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_QLNpsusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_QLNMTdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_QLNMTdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_QLNMTdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_QLNMTusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_QLNMTusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_QLNMTusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_SNRGdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_SNRGdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_SNRGdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_SNRGusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_SNRGusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_SNRGusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_SNRpsusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_SNRpsusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_SNRpsusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_SNRMTdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_SNRMTdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_SNRMTdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_SNRMTusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_SNRMTusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_SNRMTusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_LATNpbdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_LATNpbdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_LATNpbdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_LATNpbusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_LATNpbusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_LATNpbusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_SATNdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_SATNdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_SATNdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_SATNusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_SATNusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_SATNusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_QLNpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_QLNpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_QLNpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

extern int8 getWANDSLDiagnositcs_SNRpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
extern int8 getWANDSLDiagnositcs_SNRpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
extern int8 setWANDSLDiagnositcs_SNRpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

