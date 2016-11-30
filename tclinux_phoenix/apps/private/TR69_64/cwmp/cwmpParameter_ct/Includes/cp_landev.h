#ifndef CP_LANDEV_H_
#define CP_LANDEV_H_

#include "cp_core.h"
#define MAX_WLAN_NODES	4
#define DHCPD_COMMON "Dhcpd_Common"
#define DHCPD_NODENAME "Dhcpd"
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#define DHCPD_NODEOPT60 "Dhcpd_Option60"
#define DHCPD_NODEOPT125 "Dhcpd_Option125"
#define DHCP6S_NODEOPT16 "Dhcp6s_Option16"
#define DHCP6S_NODEOPT17 "Dhcp6s_Option17"
#define DHCP6S_NODENAME "Dhcp6s"
#endif

#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)		
#define DEFAULT_MAX_STA_NUM "16"
#endif
/*
	This Node:    InternetGatewayDevice.LANDevice.{i}
	Parent Node: InternetGatewayDevice.LANDevice.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct LanDeviceIndex[];
extern CwmpParameterStruct WLANConfiguration[];

#if 0
/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
extern CwmpParameterStruct WLANConfiguration[];
#endif

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.1.WPS
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.1.
	Parent Node Table Location: cp_landevice_nodetable.c
*/

#ifdef WSC_AP_SUPPORT
extern CwmpParameterStruct WPSTable[];

int cwmpInitWLANWPS(DeviceNodePtr deviceNode);
#endif

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.
	Parent Node Table Location: cp_landevice_nodetable.c
*/

extern CwmpParameterStruct WLANConfigurationIndex[];

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.IPInterface.{i}
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landev_nodetable.c
*/

extern CwmpParameterStruct IPInterfaceIndex[];

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANEthernetInterfaceConfig.{i}
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANEthernetInterfaceConfig.
	Parent Node Table Location: cp_landev_nodetable.c
*/

extern CwmpParameterStruct EthInterCfgIndex[];
/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.Hosts.
	Parent Node: InternetGatewayDevice.LANDevice.{i}.
	Parent Node Table Location: cp_landevice_nodetable.c
*/

extern CwmpParameterStruct Hosts[];

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
extern CwmpParameterStruct Lan_X_CTCOM_DHCPv6[];
extern CwmpParameterStruct Lan_X_CTCOM_RouterAdvertisement[];
extern CwmpParameterStruct LANLoopbackDetection[];
extern CwmpParameterStruct LANDataSpeedLimit[];
extern CwmpParameterStruct LANDHCPv6Server[];
#endif


int	cwmpInitWLANConfiguration1SubTree( DeviceNodePtr deviceNode );

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANEthernetInterfaceNumberOfEntries
	Parent Node: InternetGatewayDevice.LANDevice.{i}.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getLANEthIfNumOfEntries1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLANEthIfNumOfEntries1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLANEthIfNumOfEntries1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANUSBInterfaceNumberOfEntries
	Parent Node: InternetGatewayDevice.LANDevice.{i}.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getLANUSBIfNumOfEntries1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLANUSBIfNumOfEntries1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLANUSBIfNumOfEntries1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANWLANConfigurationNumberOfEntries
	Parent Node: InternetGatewayDevice.LANDevice.{i}.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getLANWLANConfigNumOfEntries1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLANWLANConfigNumOfEntries1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLANWLANConfigNumOfEntries1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 addWLANConfigurationObject(uint32* num,  DeviceNode*  deviceNode);
int8 delWLANConfigurationObject(uint32 num, DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPServerConfigurable
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getDHCPServerCfg1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDHCPServerCfg1Value(void* value, struct DeviceNode*  deviceNode);
int8 getDHCPServerCfg1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPServerCfg1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPServerEnable
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getDHCPServerEnable1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDHCPServerEnable1Value(void* value, struct DeviceNode*  deviceNode);
int8 getDHCPServerEnable1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPServerEnable1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPRelay
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getDHCPRelay1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDHCPRelay1Value(void* value, struct DeviceNode*  deviceNode);
int8 getDHCPRelay1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPRelay1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.MinAddress
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getMinAddress1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMinAddress1Value(void* value, struct DeviceNode*  deviceNode);
int8 getMinAddress1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMinAddress1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.MaxAddress
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getMaxAddress1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMaxAddress1Value(void* value, struct DeviceNode*  deviceNode);
int8 getMaxAddress1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMaxAddress1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.ReservedAddresses
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getReservedAddr1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setReservedAddr1Value(void* value, struct DeviceNode*  deviceNode);
int8 getReservedAddr1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setReservedAddr1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.SubnetMask
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getSubnetMask1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSubnetMask1Value(void* value, struct DeviceNode*  deviceNode);
int8 getSubnetMask1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSubnetMask1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DNSServers
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getDNSServers1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDNSServers1Value(void* value, struct DeviceNode*  deviceNode);
int8 getDNSServers1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDNSServers1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DomainName
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getDomainName1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDomainName1Value(void* value, struct DeviceNode*  deviceNode);
int8 getDomainName1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDomainName1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.IPRouters
        Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
        Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getIPRouters1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPRouters1Value(void* value, struct DeviceNode*  deviceNode);
int8 getIPRouters1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPRouters1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPLeaseTime
        Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
        Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getDHCPLeaseTime1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDHCPLeaseTime1Value(void* value, struct DeviceNode*  deviceNode);
int8 getDHCPLeaseTime1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPLeaseTime1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPInterfaceNumValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getIPInterfaceNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPInterfaceNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int	cwmpInitLANIPInterfaceSubTree( DeviceNodePtr deviceNode );
int	cwmpInitLANEthInterCfgSubTree( DeviceNodePtr deviceNode );

#ifdef DHCP_PROFILE
/*
      This Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPOption.{i}.
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPOption.
	Parent Node Table Location: cp_landev_nodetable.c 
*/
extern CwmpParameterStruct DHCPOptionIndex[];

/*
      This Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.
	Parent Node Table Location: cp_landev_nodetable.c
*/
extern CwmpParameterStruct DHCPConSerPoolIndex[];

int8 getDHCPOptionNumValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDHCPOptionNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPOptionNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDHCPConditionalPoolNumOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDHCPConditionalPoolNumOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPConditionalPoolNumOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int cwmpInitDHCPOptionSubTree( DeviceNodePtr deviceNode );
int cwmpInitDHCPConSerPoolSubTree( DeviceNodePtr deviceNode );

#endif
int8 getUseAllocatedWAN1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUseAllocatedWAN1Value(void* value, struct DeviceNode*  deviceNode);
int8 getUseAllocatedWAN1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUseAllocatedWAN1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAssociatedConnection1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAssociatedConnection1Value(void* value, struct DeviceNode*  deviceNode);
int8 getAssociatedConnection1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAssociatedConnection1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPassthroughLease1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPassthroughLease1Value(void* value, struct DeviceNode*  deviceNode);
int8 getPassthroughLease1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPassthroughLease1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPassthroughMACAddress1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPassthroughMACAddress1Value(void* value, struct DeviceNode*  deviceNode);
int8 getPassthroughMACAddress1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPassthroughMACAddress1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAllowedMACAddress1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAllowedMACAddress1Value(void* value, struct DeviceNode*  deviceNode);
int8 getAllowedMACAddress1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAllowedMACAddress1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getHGWMinAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setHGWMinAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getHGWMinAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHGWMinAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHGWMaxAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setHGWMaxAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getHGWMaxAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHGWMaxAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSTBMinAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSTBMinAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getSTBMinAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSTBMinAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSTBMaxAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSTBMaxAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getSTBMaxAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSTBMaxAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPhoneMinAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPhoneMinAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getPhoneMinAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPhoneMinAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPhoneMaxAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPhoneMaxAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getPhoneMaxAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPhoneMaxAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCameraMinAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCameraMinAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getCameraMinAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCameraMinAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCameraMaxAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCameraMaxAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getCameraMaxAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCameraMaxAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getComputerMinAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setComputerMinAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getComputerMinAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setComputerMinAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getComputerMaxAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setComputerMaxAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getComputerMaxAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setComputerMaxAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getOPTION60EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setOPTION60EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getOPTION60EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setOPTION60EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getOPTION125EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setOPTION125EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getOPTION125EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setOPTION125EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getOPTION16EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setOPTION16EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getOPTION16EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setOPTION16EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getOPTION17EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setOPTION17EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getOPTION17EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setOPTION17EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLanIPv6CfgDomainNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLanIPv6CfgDomainNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getLanIPv6CfgDomainNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLanIPv6CfgDomainNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLanIPv6CfgDNSConfigTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLanIPv6CfgDNSConfigTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getLanIPv6CfgDNSConfigTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLanIPv6CfgDNSConfigTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int getIfindexByWanConnection(char *connection);
int createWanConnectionByIfindex(int ifidx, char *out_connection);
int8 getLanIPv6CfgDNSWANConnectionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLanIPv6CfgDNSWANConnectionValue(void* value, struct DeviceNode*  deviceNode);
int8 getLanIPv6CfgDNSWANConnectionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLanIPv6CfgDNSWANConnectionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLanIPv6CfgDNSServersValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLanIPv6CfgDNSServersValue(void* value, struct DeviceNode*  deviceNode);
int8 getLanIPv6CfgDNSServersAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLanIPv6CfgDNSServersAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLanIPv6CfgRadvdPrefixModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLanIPv6CfgRadvdPrefixModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getLanIPv6CfgRadvdPrefixModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLanIPv6CfgRadvdPrefixModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLanIPv6CfgRadvdDelegatedWanConnValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLanIPv6CfgRadvdDelegatedWanConnValue(void* value, struct DeviceNode*  deviceNode);
int8 getLanIPv6CfgRadvdDelegatedWanConnAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLanIPv6CfgRadvdDelegatedWanConnAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLanIPv6CfgRadvdPrefixValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLanIPv6CfgRadvdPrefixValue(void* value, struct DeviceNode*  deviceNode);
int8 getLanIPv6CfgRadvdPrefixAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLanIPv6CfgRadvdPrefixAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLanIPv6CfgRadvdPltimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLanIPv6CfgRadvdPltimeValue(void* value, struct DeviceNode*  deviceNode);
int8 getLanIPv6CfgRadvdPltimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLanIPv6CfgRadvdPltimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLanIPv6CfgRadvdVltimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLanIPv6CfgRadvdVltimeValue(void* value, struct DeviceNode*  deviceNode);
int8 getLanIPv6CfgRadvdVltimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLanIPv6CfgRadvdVltimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER) && defined(TCSUPPORT_WLAN)
int8 getWLANCfgEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER) && defined(TCSUPPORT_CT_USB)
int8 getUSBPortStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUSBPortStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUSBPortStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif


#endif
