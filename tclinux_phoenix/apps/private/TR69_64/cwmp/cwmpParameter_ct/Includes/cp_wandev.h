#ifndef CP_WANDEV_H_
#define CP_WANDEV_H_

#include "cp_core.h"

#define TOTAL_REMOTE_DEVICE_NODE 	8
#define TOTAL_REMOTE_ENTRY_NODE 	8
#define MAX_VIRSERV_RULE 10
#define COMMIT_THIRD_ALIAS   100
#define WAN_ENTRY_UNSET_ALIAS  64
#define WAN_PVC_UNSET_ALIAS  8

extern spSysParam_t spSysParam;

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLDiagnostics.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
extern CwmpParameterStruct WANDSLDiagnostics[];



/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
extern CwmpParameterStruct WANConnectionDeviceIndex[];


/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
//extern CwmpParameterStruct WANConnectionDevice_IP[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
/* for adding pvc */
//extern CwmpParameterStruct WANConnectionDevice_IP_PVCADDING[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
//extern CwmpParameterStruct WANConnectionDevice_PPP[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
/* for adding pvc */
//extern CwmpParameterStruct WANConnectionDevice_PPP_PVCADDING[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANIPConnection.
	Parent Node Table Location: cp_wandev_nodetable.c 
*/
extern CwmpParameterStruct WANIPConnectionIndex[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANPPPConnection.
	Parent Node Table Location: cp_wandev_nodetable.c 
*/
extern CwmpParameterStruct WANPPPConnectionIndex[];


/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
extern CwmpParameterStruct WANDSLInterfaceConfig[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLCommonInterfaceConfig.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
extern CwmpParameterStruct WANCommonInterfaceConfig[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLConnectionManagement.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
extern CwmpParameterStruct WANDSLConnectionManagement[];

#if defined(TCSUPPORT_CT_PON)
/*
	Parent Node:  InternetGatewayDevice.WANDevice.1.X_CT-COM_EponInterfaceConfig.
	                                 InternetGatewayDevice.WANDevice.1.X_CT-COM_GponInterfaceConfig.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
extern CwmpParameterStruct WANEPONInterfaceConfig[];
extern CwmpParameterStruct WANGPONInterfaceConfig[];
extern CwmpParameterStruct EponInterfaceConfig[] ;
extern CwmpParameterStruct GponInterfaceConfig[];
#endif


/*
	Parent Node: InternetGatewayDevice.WANDevice.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct WANDeviceIndex[];

int8 DeviceNodeAddPVCObject(uint32 *num, DeviceNode*  deviceNode);
int cwmpInitWANConnectionDeviceTable(DeviceNodePtr deviceNode);
#if defined(TCSUPPORT_CT_PON)
extern CwmpParameterStruct WANDevice[];
extern CwmpParameterStruct EponInterfaceConfig[];
int cwmpInitWANDeviceTable(DeviceNodePtr deviceNode);
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getX_CTCOMWANIndexValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getX_CTCOMWANIndexAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setX_CTCOMWANIndexAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#endif

