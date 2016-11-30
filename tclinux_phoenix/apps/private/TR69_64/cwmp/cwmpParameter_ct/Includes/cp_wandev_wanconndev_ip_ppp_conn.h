#ifndef CP_WANDEV_WANCONNDEV_IP_PPP_CONN_H_
#define CP_WANDEV_WANCONNDEV_IP_PPP_CONN_H_

#include "cp_core.h"

extern spSysParam_t spSysParam;
//extern uint16 ACSaddingPVCFlag;
#define MAX_VIRSERV_RULE 10

#define COMMIT_THIRD_ALIAS   100
#define WAN_ENTRY_UNSET_ALIAS  64
#define WAN_PVC_UNSET_ALIAS  8
#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_E8DDNS)
#define PVC_ENTRY_NUM 8
#endif
#endif

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.IPConnection.1.PortMapping.
	Parent Node Table Location: cp_wandev_wanconndev_subtree_nodetable.c
*/
extern CwmpParameterStruct PortMappingIndex[];
#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_E8DDNS)
/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANPPPConnection.{i}.X_CT-Com_DDNSConfiguration
	Parent Node Table Location: cp_wandev_wanconndev_ip_ppp_conn_notetable.c  
*/
extern CwmpParameterStruct CTComDDNSConfigurationIndex[];
#endif
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
extern CwmpParameterStruct cwmpInitDhcpOption60Index[];
extern CwmpParameterStruct cwmpInitDhcpOption125Index[];
extern CwmpParameterStruct cwmpInitDhcpv6Option17Index[];
extern CwmpParameterStruct cwmpInitDhcpv6Option16Index[];
#endif
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
/*
 get wan dslite state: 1->on , 0->off
*/
int get_dslite_state(char *nodeName);
/*
 check dslite state: 1->up , 0->down
*/
int is_dslite_up(char *nodeName);
/*
 check ipv6cp state: 1->up , 0->down
*/
int is_ipv6cp_up(int idx);
#endif


/*rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.IPConnection.1.*/
int8 getWANCnt1_1EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1StatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1StatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1StatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1PsbTypesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1PsbTypesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1PsbTypesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1TypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1TypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1TypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1TypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1NameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1NameValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1NameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1NameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1UptimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1UptimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1UptimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1LErrorValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1LErrorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1LErrorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANIPCnt1_1RSIPAvailableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1RSIPAvailableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1RSIPAvailableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1NATEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1NATEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1NATEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1NATEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANIPCnt1_1AddrTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANIPCnt1_1AddrTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1AddrTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1AddrTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1ExtIPAddrValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1ExtIPAddrValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1ExtIPAddrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1ExtIPAddrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANIPCnt1_1SubMaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANIPCnt1_1SubMaskValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1SubMaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1SubMaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANIPCnt1_1DefGatewayValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANIPCnt1_1DefGatewayValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1DefGatewayAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1DefGatewayAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1DNSEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1DNSEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1DNSEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1DNSEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1DNSOvAllowedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1DNSOvAllowedValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1DNSOvAllowedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1DNSOvAllowedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1DNSServersValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1DNSServersValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1DNSServersAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1DNSServersAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1MACAddrValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1MACAddrValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1MACAddrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1MACAddrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1CntTriggerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1CntTriggerValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1CntTriggerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1CntTriggerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
int8 getWANCnt1_IdleDisconnectTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_IdleDisconnectTimeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANCnt1_IdleDisconnectTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANCnt1_IdleDisconnectTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

int8 getWANCnt1_1RProRxValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1RProRxValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1RProRxAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1RProRxAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1PMapNumOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1PMapNumOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1PMapNumOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#if !defined(TCSUPPORT_C1_CUC)
int8 getWANCnt1_CTComServiceListValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_CTComServiceListValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_CTComServiceListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_CTComServiceListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_CTComInterfaceBondValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_CTComInterfaceBondValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_CTComInterfaceBondAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_CTComInterfaceBondAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getWANCnt1_CTComDHCPEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_CTComDHCPEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_CTComDHCPEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_CTComDHCPEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
/*rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.IPConnection.1.PortMapping.{i}.*/
int	cwmpInitWANGetPortMappingFromFlash( DeviceNodePtr deviceNode );
int8 DeviceNodeAddPMappingObject(uint32* num, DeviceNode*  deviceNode);
int8 DeviceNodeDelPMappingObject(uint32 num, DeviceNode*  deviceNode);

int8 getIPPMapping1EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPPMapping1EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPPMapping1EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPPMapping1EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPPMapping1LeaseDurValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPPMapping1LeaseDurValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPPMapping1LeaseDurAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPPMapping1LeaseDurAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIP1RemoteHostValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIP1RemoteHostValue(void* value, struct DeviceNode*  deviceNode);
int8 getIP1RemoteHostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIP1RemoteHostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIP1ExternalPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIP1ExternalPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getIP1ExternalPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIP1ExternalPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPExternalPortEndRangeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPExternalPortEndRangeValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPExternalPortEndRangeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPExternalPortEndRangeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIP1InternalPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIP1InternalPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getIP1InternalPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIP1InternalPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPPMapping1ProtocolValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPPMapping1ProtocolValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPPMapping1ProtocolAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPPMapping1ProtocolAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIP1InternalClientValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIP1InternalClientValue(void* value, struct DeviceNode*  deviceNode);
int8 getIP1InternalClientAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIP1InternalClientAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPPMapping1DescripValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPPMapping1DescripValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPPMapping1DescripAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPPMapping1DescripAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.IPConnection.1.Stats.*/
int8 getEthernetBytesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthernetBytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthernetBytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEthernetBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthernetBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthernetBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEthernetPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthernetPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthernetPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEthernetPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthernetPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthernetPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#if defined(TCSUPPORT_CT_FJ)
int8 getEthernetErrorsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthernetErrorsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthernetErrorsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getEthernetDiscardPktsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthernetDiscardPktsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthernetDiscardPktsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
/*rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.PPPConnection.1.*/
int8 getWANPPPCnt1_1UsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPPPCnt1_1UsernameValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1UsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1UsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPPPCnt1_1PasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPPPCnt1_1PasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1PasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1PasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPPPCnt1_1TransTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1TransTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1TransTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

//#ifdef TC_TR069
int8 getWANPPPCnt1_1AuthenTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPPPCnt1_1AuthenTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1AuthenTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1AuthenTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
//#endif

int8 getWANPPPCnt1_1PPPoEACNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPPPCnt1_1PPPoEACNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1PPPoEACNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1PPPoEACNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPPPCnt1_1PPPoESvcNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPPPCnt1_1PPPoESvcNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1PPPoESvcNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1PPPoESvcNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_PPPOEPROXY)
int8 getWANCnt1_1ProxyEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1ProxyEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1ProxyEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1ProxyEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getWANCnt1_1ProxyMaxUserValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1ProxyMaxUserValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1ProxyMaxUserAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1ProxyMaxUserAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#endif

#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_E8DDNS)
int8 getDDNSCfgEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDDNSCfgEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getDDNSCfgEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDDNSCfgEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDDNSProviderValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDDNSProviderValue(void* value, struct DeviceNode*  deviceNode);
int8 getDDNSProviderAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDDNSProviderAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDDNSUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDDNSUsernameValue(void* value, struct DeviceNode*  deviceNode);
int8 getDDNSUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDDNSUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDDNSPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDDNSPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getDDNSPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDDNSPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getServicePortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setServicePortValue(void* value, struct DeviceNode*  deviceNode);
int8 getServicePortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setServicePortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDDNSDomainNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDDNSDomainNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getDDNSDomainNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDDNSDomainNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDDNSHostNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDDNSHostNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getDDNSHostNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDDNSHostNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int cwmpInitDDNSConfigurationSubTree(DeviceNodePtr deviceNode);
#endif
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getWANCnt1_IPModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_IPModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_IPModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_IPModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_IPv6ConStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_IPv6ConStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_IPv6ConStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_MulticastVlanValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_MulticastVlanValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_MulticastVlanAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_MulticastVlanAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDhcpOption60EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpOption60EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpOption60EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpOption60EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDhcpOption60TypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpOption60TypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpOption60TypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpOption60TypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDhcpOption60ValueModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpOption60ValueModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpOption60ValueModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpOption60ValueModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDhcpOption60ValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpOption60ValueValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpOption60ValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpOption60ValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int cwmpInitDhcpOption60SubTree(DeviceNodePtr deviceNode);

int8 getDhcpOption125EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpOption125EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpOption125EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpOption125EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDhcpOption125ValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpOption125ValueValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpOption125ValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpOption125ValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDhcpOption125SubCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpOption125SubCodeValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpOption125SubCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpOption125SubCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDhcpOption125SubDataValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpOption125SubDataValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpOption125SubDataAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpOption125SubDataAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 ip_ppp_dhcpoption_node_check(DeviceNode*  deviceNode, uint32 *pvcIndex, uint32* entryIndex, uint8 *optionIndex);

int8 getDhcpOption125TypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpOption125TypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpOption125TypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpOption125TypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int cwmpInitDhcpOption125SubTree(DeviceNodePtr deviceNode);

int8 getDhcpv6Option17EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpv6Option17EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpv6Option17EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpv6Option17EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDhcpv6Option17TypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpv6Option17TypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpv6Option17TypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpv6Option17TypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDhcpv6Option17ValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpv6Option17ValueValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpv6Option17ValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpv6Option17ValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDhcpOption17SubCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpOption17SubCodeValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpOption17SubCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpOption17SubCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDhcpOption17SubDataValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpOption17SubDataValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpOption17SubDataAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpOption17SubDataAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int cwmpInitDhcpv6Option17SubTree(DeviceNodePtr deviceNode);

int8 getDhcpv6Option16EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpv6Option16EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpv6Option16EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpv6Option16EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDhcpv6Option16ValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpv6Option16ValueValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpv6Option16ValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpv6Option16ValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDhcpOption16TypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpOption16TypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpOption16TypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpOption16TypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDhcpOption16ValueModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDhcpOption16ValueModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getDhcpOption16ValueModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDhcpOption16ValueModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int cwmpInitDhcpv6Option16SubTree(DeviceNodePtr deviceNode);

int8 getIPv6AliasValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getIPv6AliasAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPv6AliasAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPv6IPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPv6IPAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPv6IPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPv6IPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPv6DNSIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPv6DNSIPAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPv6DNSIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPv6DNSIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPv6OriginValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPv6OriginValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPv6OriginAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPv6OriginAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPv6ChildPrefixBitsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPv6ChildPrefixBitsValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPv6ChildPrefixBitsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPv6ChildPrefixBitsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPv6PrefixAliasValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getIPv6PrefixAliasAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPv6PrefixAliasAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPv6PrefixValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPv6PrefixValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPv6PrefixAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPv6PrefixAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPv6PrefixOriginValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPv6PrefixOriginValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPv6PrefixOriginAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPv6PrefixOriginAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getIPv6PrefixChildPrefixBitsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPv6PrefixChildPrefixBitsValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPv6PrefixChildPrefixBitsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPv6PrefixChildPrefixBitsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#ifdef TCSUPPORT_DSLITE
int8 getCTComDsliteEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTComDsliteEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTComDsliteEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTComDsliteEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTComAftrModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTComAftrModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTComAftrModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTComAftrModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTComAftrValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTComAftrValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTComAftrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTComAftrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

int8 ip_ppp_node_check(DeviceNode*  deviceNode, uint32 *pvcIndex, uint32* entryIndex);
int8 ip_ppp_IPForwardV4addr_check(char *v4addr);
int8 ip_ppp_IPForwardV4_check(char *v4addr, int maxlen);
int8 ip_ppp_IPForwardV6addr_check(char *v6addr);
int8 ip_ppp_IPForwardV6_check(char *v6addr, int maxlen);
int8 ip_ppp_IPForwardV4V6_check(char *v4v6addr, int maxlen);

int8 getIPForwardListValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPForwardListValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPForwardListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPForwardListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getXIPv6IPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setXIPv6IPAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getXIPv6IPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setXIPv6IPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPv6IPAddressAliasValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPv6IPAddressAliasValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPv6IPAddressAliasAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPv6IPAddressAliasAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPAddressOriginValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPAddressOriginValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPAddressOriginAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPAddressOriginAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPv6DNSServersValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPv6DNSServersValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPv6DNSServersAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPv6DNSServersAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPv6PrefixDelegationValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPv6PrefixDelegationValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPv6PrefixDelegationAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPv6PrefixDelegationAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTComIPv6PrefixAliasValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTComIPv6PrefixAliasValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTComIPv6PrefixAliasAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTComIPv6PrefixAliasAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTComIPv6PrefixOriginValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTComIPv6PrefixOriginValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTComIPv6PrefixOriginAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTComIPv6PrefixOriginAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTComIPv6PrefixValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTComIPv6PrefixValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTComIPv6PrefixAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTComIPv6PrefixAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getIPv6PrefixPltimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPv6PrefixPltimeValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPv6PrefixPltimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPv6PrefixPltimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPv6PrefixVltimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPv6PrefixVltimeValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPv6PrefixVltimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPv6PrefixVltimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPv6DefGatewayValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPv6DefGatewayValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPv6DefGatewayAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPv6DefGatewayAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPv6DomainNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPv6DomainNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPv6DomainNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPv6DomainNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getWANCnt1_ResetValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_ResetValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANCnt1_ResetAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANCnt1_ResetAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_ForwardModeEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_ForwardModeEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANCnt1_ForwardModeEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANCnt1_ForwardModeEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_VlanEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_VlanEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANCnt1_VlanEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANCnt1_VlanEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_VlanIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_VlanIDValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANCnt1_VlanIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANCnt1_VlanIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_PriorityValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_PriorityValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANCnt1_PriorityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANCnt1_PriorityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_IgmpProxyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_IgmpProxyValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANCnt1_IgmpProxyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANCnt1_IgmpProxyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER) && !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getWANPPPCnt1_1PPPoEBIValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPPPCnt1_1PPPoEBIValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1PPPoEBIAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1PPPoEBIAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
int8 getWANPPPCnt1_1LErrorCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1LErrorCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1LErrorCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#endif
