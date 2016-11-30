#ifndef CP_LANDEV_IPINTERFACE_H_
#define CP_LANDEV_IPINTERFACE_H_

#include "cp_core.h"

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
extern CwmpParameterStruct cwmpInitIPv6AddressIndex[];
#endif

int8 getIPInterface_EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPInterface_EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPInterface_EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPInterface_EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getIPInterfaceIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPInterfaceIPAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPInterfaceIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPInterfaceIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getIPInterfaceSubnetMaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPInterfaceSubnetMaskValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPInterfaceSubnetMaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPInterfaceSubnetMaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getIPInterfaceAddressingTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPInterfaceAddressingTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPInterfaceAddressingTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPInterfaceAddressingTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getCTComIPv6IPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTComIPv6IPAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTComIPv6IPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTComIPv6IPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int cwmpInitIPv6AddressSubTree(DeviceNodePtr deviceNode);
int8 cwmpAddObjectIPv6Address(uint32 *num, DeviceNode *deviceNode);
int8 cwmpDelObjectIPv6Address(uint32 num, DeviceNode *deviceNode);

int8 getLanIPv6LocalAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLanIPv6LocalAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getLanIPv6LocalAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLanIPv6LocalAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)

int8 getIPv6InterfaceAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPv6InterfaceAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPv6InterfaceAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPv6InterfaceAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLanIPv6CfgRadvdPrefixModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLanIPv6CfgRadvdPrefixModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getLanIPv6CfgRadvdPrefixModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLanIPv6CfgRadvdPrefixModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLanIPv6CfgRadvdDelegatedWanConnValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLanIPv6CfgRadvdDelegatedWanConnValue(void* value, struct DeviceNode*  deviceNode);
int8 getLanIPv6CfgRadvdDelegatedWanConnAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLanIPv6CfgRadvdDelegatedWanConnAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

uint8 cnt_mask_bits(ip4a	subnet_mask);
#endif

