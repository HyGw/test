#ifndef CP_WANDEV_WANCONNDEV_H_
#define CP_WANDEV_WANCONNDEV_H_

#include "cp_core.h"

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANDSLLinkConfig.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
extern CwmpParameterStruct WANDSLLinkConfig[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANATMF5LoopbackDiagnostics.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
extern CwmpParameterStruct WANATMF5LoopbackDiagnostics[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.IPConnection.1.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
extern CwmpParameterStruct WANIPConnection[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.PPPConnection.1.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
extern CwmpParameterStruct WANPPPConnection[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct WANDeviceIndex[];

#if defined(TCSUPPORT_CT_PON)
/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.X_CT-COM_WANEponLinkConfig.
	                    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.X_CT-COM_WANGponLinkConfig.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
extern CwmpParameterStruct WANConnectionDevice_table[];
extern CwmpParameterStruct WANEPonLinkConfig[];
extern CwmpParameterStruct WANGEPonLinkConfig[];
extern CwmpParameterStruct WANEponLinkConfigNode[];
extern CwmpParameterStruct WANGponLinkConfigNode[];
int cwmpInitWANConnectionObjectSubtree(DeviceNodePtr deviceNode);
#endif

#if defined(TCSUPPORT_CT_WAN_PTM)
/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.X_CT-COM_WANVdslLinkConfig.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
extern CwmpParameterStruct WANVdslLinkConfig[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANPTMLinkConfig.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/

extern CwmpParameterStruct WANPTMLinkConfig[];
#endif


int8 DeviceNodeDelPVCObject(uint32 num, DeviceNode*  deviceNode);
int8 cwmpAddIPConnectionObject(uint32* num, DeviceNode*  deviceNode);
int cwmpInitIPConnectionObjectSubtree(DeviceNodePtr deviceNode);
int8 cwmpAddPPPConnectionObject(uint32* num, DeviceNode*  deviceNode);
int cwmpInitPPPConnectionObjectSubtree(DeviceNodePtr deviceNode);
int8 cwmpDeleteConnectionObject(uint32 num, DeviceNode*  deviceNode);

int8 getWANIPCntNumOfEntries1_1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCntNumOfEntries1_1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCntNumOfEntries1_1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPPPCntNumOfEntries1_1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPPPCntNumOfEntries1_1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCntNumOfEntries1_1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getWANVLANEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANVLANEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANVLANEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANVLANEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANVLANValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANVLANValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANVLANAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANVLANAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWAN8021PValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWAN8021PValue(void* value, struct DeviceNode*  deviceNode);
int8 getWAN8021PAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWAN8021PAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#endif


