#include "Global_res.h"
#include "cp_wandev_wanconndev_ip_ppp_conn.h"

static NodeFunc IP_PPP_Enable=
{
	NULL,
	NULL,
	getWANCnt1_1EnableValue,
	setWANCnt1_1EnableValue,
	getWANIPCnt1_1EnableAttribute, 
	setWANIPCnt1_1EnableAttribute, 
	NULL
};

static NodeFunc ConnectionStatus=
{
	NULL,
	NULL,
	getWANCnt1_1StatusValue,
	NULL,
	getWANIPCnt1_1StatusAttribute, 
	setWANIPCnt1_1StatusAttribute, 
	NULL
};

static NodeFunc PossibleConnectionTypes=
{
	NULL,
	NULL,
	getWANCnt1_1PsbTypesValue,
	NULL,
	getWANIPCnt1_1PsbTypesAttribute, 
	setWANIPCnt1_1PsbTypesAttribute, 
	NULL
};

static NodeFunc ConnectionType=
{
	NULL,
	NULL,
	getWANCnt1_1TypeValue,
	setWANCnt1_1TypeValue,
	getWANIPCnt1_1TypeAttribute, 
	setWANIPCnt1_1TypeAttribute, 
	NULL
};

static NodeFunc Name=
{
	NULL,
	NULL,
	getWANCnt1_1NameValue,
	setWANCnt1_1NameValue,
	getWANIPCnt1_1NameAttribute, 
	setWANIPCnt1_1NameAttribute, 
	NULL
};

static NodeFunc Uptime=
{
	NULL,
	NULL,
	getWANCnt1_1UptimeValue,
	NULL,
	getWANIPCnt1_1UptimeAttribute, 
	setWANIPCnt1_1UptimeAttribute, 
	NULL
};

static NodeFunc LastConnectionError=
{
	NULL,
	NULL,
	getWANCnt1_1LErrorValue,
	NULL,
	getWANIPCnt1_1LErrorAttribute, 
	setWANIPCnt1_1LErrorAttribute, 
	NULL
};

static NodeFunc RSIPAvailable=
{
	NULL,
	NULL,
	getWANIPCnt1_1RSIPAvailableValue,
	NULL,
	getWANIPCnt1_1RSIPAvailableAttribute, 
	setWANIPCnt1_1RSIPAvailableAttribute, 
	NULL
};

static NodeFunc NATEnabled=
{
	NULL,
	NULL,
	getWANCnt1_1NATEnabledValue,
	setWANCnt1_1NATEnabledValue,
	getWANIPCnt1_1NATEnabledAttribute, 
	setWANIPCnt1_1NATEnabledAttribute, 
	NULL
};

static NodeFunc AddressingType=
{
	NULL,
	NULL,
	getWANIPCnt1_1AddrTypeValue,
	setWANIPCnt1_1AddrTypeValue,
	getWANIPCnt1_1AddrTypeAttribute, 
	setWANIPCnt1_1AddrTypeAttribute, 
	NULL
};

static NodeFunc IPExternalIPAddress=
{
	NULL,
	NULL,
	getWANCnt1_1ExtIPAddrValue,
	setWANCnt1_1ExtIPAddrValue,
	getWANIPCnt1_1ExtIPAddrAttribute, 
	setWANIPCnt1_1ExtIPAddrAttribute, 
	NULL
};
static NodeFunc PPPExternalIPAddress=
{
	NULL,
	NULL,
	getWANCnt1_1ExtIPAddrValue,
	NULL,
	getWANIPCnt1_1ExtIPAddrAttribute, 
	setWANIPCnt1_1ExtIPAddrAttribute, 
	NULL
};


static NodeFunc SubnetMask=
{
	NULL,
	NULL,
	getWANIPCnt1_1SubMaskValue,
	setWANIPCnt1_1SubMaskValue,
	getWANIPCnt1_1SubMaskAttribute, 
	setWANIPCnt1_1SubMaskAttribute, 
	NULL
};

static NodeFunc DefaultGateway=
{
	NULL,
	NULL,
	getWANIPCnt1_1DefGatewayValue,
	setWANIPCnt1_1DefGatewayValue,
	getWANIPCnt1_1DefGatewayAttribute, 
	setWANIPCnt1_1DefGatewayAttribute, 
	NULL
};

static NodeFunc DNSEnabled=
{
	NULL,
	NULL,
	getWANCnt1_1DNSEnabledValue,
	setWANCnt1_1DNSEnabledValue,
	getWANIPCnt1_1DNSEnabledAttribute, 
	setWANIPCnt1_1DNSEnabledAttribute, 
	NULL
};

static NodeFunc DNSOverrideAllowed=
{
	NULL,
	NULL,
	getWANCnt1_1DNSOvAllowedValue,
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	setWANCnt1_1DNSOvAllowedValue,
#else
	NULL,
#endif
	getWANIPCnt1_1DNSOvAllowedAttribute, 
	setWANIPCnt1_1DNSOvAllowedAttribute, 
	NULL
};

static NodeFunc DNSServers=
{
	NULL,
	NULL,
	getWANCnt1_1DNSServersValue,
	setWANCnt1_1DNSServersValue,
	getWANIPCnt1_1DNSServersAttribute, 
	setWANIPCnt1_1DNSServersAttribute, 
	NULL
};

static NodeFunc MACAddress=
{
	NULL,
	NULL,
	getWANCnt1_1MACAddrValue,
	NULL,
	getWANIPCnt1_1MACAddrAttribute, 
	setWANIPCnt1_1MACAddrAttribute, 
	NULL
};

static NodeFunc RouteProtocolRx=
{
	NULL,
	NULL,
	getWANCnt1_1RProRxValue,
	setWANCnt1_1RProRxValue,
	getWANIPCnt1_1RProRxAttribute, 
	setWANIPCnt1_1RProRxAttribute, 
	NULL
};

static NodeFunc PortMappingNumberOfEntries=
{
	NULL,
	NULL,
	getWANCnt1_1PMapNumOfEntriesValue,
	NULL,
	getWANIPCnt1_1PMapNumOfEntriesAttribute, 
	setWANIPCnt1_1PMapNumOfEntriesAttribute, 
	NULL
};

#if !defined(TCSUPPORT_C1_CUC)
static NodeFunc CTComServiceList=
{
	NULL,
	NULL,
	getWANCnt1_CTComServiceListValue,
	setWANCnt1_CTComServiceListValue,
	getWANIPCnt1_CTComServiceListAttribute, 
	setWANIPCnt1_CTComServiceListAttribute, 
	NULL
};

static NodeFunc CTComInterfaceBond=
{
	NULL,
	NULL,
	getWANCnt1_CTComInterfaceBondValue,
	setWANCnt1_CTComInterfaceBondValue,
	getWANIPCnt1_CTComInterfaceBondAttribute, 
	setWANIPCnt1_CTComInterfaceBondAttribute, 
	NULL
};

static NodeFunc CTComDHCPEnable=
{
	NULL,
	NULL,
	getWANCnt1_CTComDHCPEnableValue,
	setWANCnt1_CTComDHCPEnableValue,
	getWANIPCnt1_CTComDHCPEnableAttribute, 
	setWANIPCnt1_CTComDHCPEnableAttribute, 
	NULL
};

#endif
static NodeFunc PortMappingEnabled=
{
	NULL,
	NULL,
	getIPPMapping1EnableValue,
	setIPPMapping1EnableValue,
	getIPPMapping1EnableAttribute, 
	setIPPMapping1EnableAttribute, 
	NULL
};

static NodeFunc PortMappingLeaseDuration=
{
	NULL,
	NULL,
	getIPPMapping1LeaseDurValue,
	setIPPMapping1LeaseDurValue,
	getIPPMapping1LeaseDurAttribute, 
	setIPPMapping1LeaseDurAttribute, 
	NULL
};

static NodeFunc RemoteHost=
{
	NULL,
	NULL,
	getIP1RemoteHostValue,
	setIP1RemoteHostValue,
	getIP1RemoteHostAttribute, 
	setIP1RemoteHostAttribute, 
	NULL
};

static NodeFunc ExternalPort=
{
	NULL,
	NULL,
	getIP1ExternalPortValue,
	setIP1ExternalPortValue,
	getIP1ExternalPortAttribute, 
	setIP1ExternalPortAttribute, 
	NULL
};

static NodeFunc ExternalPortEndRange=
{
	NULL,
	NULL,
	getIPExternalPortEndRangeValue,
	setIPExternalPortEndRangeValue,
	getIPExternalPortEndRangeAttribute, 
	setIPExternalPortEndRangeAttribute, 
	NULL
};

static NodeFunc InternalPort=
{
	NULL,
	NULL,
	getIP1InternalPortValue,
	setIP1InternalPortValue,
	getIP1InternalPortAttribute, 
	setIP1InternalPortAttribute, 
	NULL
};

static NodeFunc PortMappingProtocol=
{
	NULL,
	NULL,
	getIPPMapping1ProtocolValue,
	setIPPMapping1ProtocolValue,
	getIPPMapping1ProtocolAttribute, 
	setIPPMapping1ProtocolAttribute, 
	NULL
};

static NodeFunc InternalClient=
{
	NULL,
	NULL,
	getIP1InternalClientValue,
	setIP1InternalClientValue,
	getIP1InternalClientAttribute, 
	setIP1InternalClientAttribute, 
	NULL
};

static NodeFunc PortMappingDescription=
{
	NULL,
	NULL,
	getIPPMapping1DescripValue,
	setIPPMapping1DescripValue,
	getIPPMapping1DescripAttribute, 
	setIPPMapping1DescripAttribute, 
	NULL
};

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.IPConnection.1.PortMapping.{i}.
	Parent Node Table Location: cp_wandevice_nodetable.c
*/
CwmpParameterStruct PortMapping[] =
{
	{"PortMappingEnabled", BooleanW, NoLimite, &PortMappingEnabled, NULL},
	{"PortMappingLeaseDuration", UnsignIntW, NoLimite, &PortMappingLeaseDuration, NULL},
	{"RemoteHost", StrW, NoLimite, &RemoteHost, NULL},
	{"ExternalPort", UnsignIntW, NoLimite, &ExternalPort, NULL},
	{"ExternalPortEndRange", UnsignIntW, NoLimite, &ExternalPortEndRange, NULL},
	{"InternalPort", UnsignIntW, NoLimite, &InternalPort, NULL},
	{"PortMappingProtocol", StrW, NoLimite, &PortMappingProtocol, NULL},
	{"InternalClient", StrW, NoLimite, &InternalClient, NULL},
	{"PortMappingDescription", StrW, NoLimite, &PortMappingDescription, NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc PortMappingIndexFuncs=
{
	NULL,
	DeviceNodeDelPMappingObject,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL
};

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.IPConnection.1.PortMapping.
	Parent Node Table Location: cp_wandev_wanconndev_subtree_nodetable.c
*/
CwmpParameterStruct PortMappingIndex[]= 
{
	{NULL, ObjectW, NoLimite, &PortMappingIndexFuncs, PortMapping},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc PortMappingFuncs=
{
	DeviceNodeAddPMappingObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitWANGetPortMappingFromFlash
};

static NodeFunc EthernetBytesSent=
{
	NULL,
	NULL,
	getEthernetBytesSentValue,
	NULL,
	getEthernetBytesSentAttribute, 
	setEthernetBytesSentAttribute, 
	NULL
};

static NodeFunc EthernetBytesReceived=
{
	NULL,
	NULL,
	getEthernetBytesReceivedValue,
	NULL,
	getEthernetBytesReceivedAttribute, 
	setEthernetBytesReceivedAttribute, 
	NULL
};

static NodeFunc EthernetPacketsSent=
{
	NULL,
	NULL,
	getEthernetPacketsSentValue,
	NULL,
	getEthernetPacketsSentAttribute, 
	setEthernetPacketsSentAttribute, 
	NULL
};

static NodeFunc EthernetPacketsReceived=
{
	NULL,
	NULL,
	getEthernetPacketsReceivedValue,
	NULL,
	getEthernetPacketsReceivedAttribute, 
	setEthernetPacketsReceivedAttribute, 
	NULL
};
#if defined(TCSUPPORT_CT_FJ)
static NodeFunc EthernetErrorsReceived=
{
	NULL,
	NULL,
	getEthernetErrorsReceivedValue,
	NULL,
	getEthernetErrorsReceivedAttribute, 
	setEthernetErrorsReceivedAttribute, 
	NULL
};
static NodeFunc EthernetDiscardPktsReceived=
{
	NULL,
	NULL,
	getEthernetDiscardPktsReceivedValue,
	NULL,
	getEthernetDiscardPktsReceivedAttribute, 
	setEthernetDiscardPktsReceivedAttribute, 
	NULL
};
#endif
/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.IPConnection.1.Stats.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
CwmpParameterStruct Stats[] = 
{
	{"EthernetBytesSent", UnsignIntR, NoLimite, &EthernetBytesSent, NULL},
	{"EthernetBytesReceived", UnsignIntR, NoLimite, &EthernetBytesReceived, NULL},
	{"EthernetPacketsSent", UnsignIntR, NoLimite, &EthernetPacketsSent, NULL},
	{"EthernetPacketsReceived", UnsignIntR, NoLimite, &EthernetPacketsReceived, NULL},
#if defined(TCSUPPORT_CT_FJ)
	{"EthernetErrorsReceived", UnsignIntR, NoLimite, &EthernetErrorsReceived, NULL},
	{"EthernetDiscardPacketsReceived", UnsignIntR, NoLimite, &EthernetDiscardPktsReceived, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};

#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_E8DDNS)
static NodeFunc DDNSCfgEnable=
{
	NULL,
	NULL,
	getDDNSCfgEnableValue,
	setDDNSCfgEnableValue,
	getDDNSCfgEnableAttribute, 
	setDDNSCfgEnableAttribute, 
	NULL
};
static NodeFunc DDNSProvider=
{
	NULL,
	NULL,
	getDDNSProviderValue,
	NULL,
	getDDNSProviderAttribute, 
	setDDNSProviderAttribute, 
	NULL
};
static NodeFunc DDNSUsername=
{
	NULL,
	NULL,
	getDDNSUsernameValue,
	setDDNSUsernameValue,
	getDDNSUsernameAttribute, 
	setDDNSUsernameAttribute, 
	NULL
};
static NodeFunc DDNSPassword=
{
	NULL,
	NULL,
	getDDNSPasswordValue,
	setDDNSPasswordValue,
	getDDNSPasswordAttribute, 
	setDDNSPasswordAttribute, 
	NULL
};
static NodeFunc ServicePort=
{
	NULL,
	NULL,
	getServicePortValue,
	setServicePortValue,
	getServicePortAttribute, 
	setServicePortAttribute, 
	NULL
};
static NodeFunc DDNSDomainName=
{
	NULL,
	NULL,
	getDDNSDomainNameValue,
	setDDNSDomainNameValue,
	getDDNSDomainNameAttribute, 
	setDDNSDomainNameAttribute, 
	NULL
};
static NodeFunc DDNSHostName=
{
	NULL,
	NULL,
	getDDNSHostNameValue,
	setDDNSHostNameValue,
	getDDNSHostNameAttribute, 
	setDDNSHostNameAttribute, 
	NULL
};
CwmpParameterStruct CTComDDNSConfiguration[]=
{
	{"DDNSCfgEnabled", BooleanW, NoLimite, &DDNSCfgEnable, NULL},
	{"DDNSProvider", StrR, NoLimite, &DDNSProvider, NULL},
	{"DDNSUsername", StrW, NoLimite, &DDNSUsername, NULL},
	{"DDNSPassword", StrW, NoLimite, &DDNSPassword, NULL},
	{"ServicePort", StrW, NoLimite, &ServicePort, NULL},
	{"DDNSDomainName", StrW, NoLimite, &DDNSDomainName, NULL},
	{"DDNSHostName", StrW, NoLimite, &DDNSHostName, NULL},
	{NULL, -1, -1, NULL, NULL}
};
CwmpParameterStruct CTComDDNSConfigurationIndex[]= 
{
	{NULL, ObjectR, NoLimite, NULL, CTComDDNSConfiguration},
	{NULL, -1, -1, NULL, NULL}
};
static NodeFunc CTComDDNSConfigurationFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitDDNSConfigurationSubTree
};
#endif
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
static NodeFunc CTComIPMode=
{
	NULL,
	NULL,
	getWANCnt1_IPModeValue,
	setWANCnt1_IPModeValue,
	getWANIPCnt1_IPModeAttribute,
	setWANIPCnt1_IPModeAttribute,
	NULL
};

static NodeFunc CTComIPv6ConStatus=
{
	NULL,
	NULL,
	getWANCnt1_IPv6ConStatusValue,
	NULL,
	getWANIPCnt1_IPv6ConStatusAttribute,
	setWANIPCnt1_IPv6ConStatusAttribute,
	NULL
};

static NodeFunc CTComMulticastVlan=
{
	NULL,
	NULL,
	getWANCnt1_MulticastVlanValue,
	setWANCnt1_MulticastVlanValue,
	getWANIPCnt1_MulticastVlanAttribute,
	setWANIPCnt1_MulticastVlanAttribute,
	NULL
};

static NodeFunc DhcpOption60Enable=
{
	NULL,
	NULL,
	getDhcpOption60EnableValue,
	setDhcpOption60EnableValue,
	getDhcpOption60EnableAttribute, 
	setDhcpOption60EnableAttribute, 
	NULL
};
static NodeFunc DhcpOption60Type=
{
	NULL,
	NULL,
	getDhcpOption60TypeValue,
	setDhcpOption60TypeValue,
	getDhcpOption60TypeAttribute, 
	setDhcpOption60TypeAttribute, 
	NULL
};
static NodeFunc DhcpOption60ValueMode=
{
	NULL,
	NULL,
	getDhcpOption60ValueModeValue,
	setDhcpOption60ValueModeValue,
	getDhcpOption60ValueModeAttribute, 
	setDhcpOption60ValueModeAttribute, 
	NULL
};
static NodeFunc DhcpOption60Value=
{
	NULL,
	NULL,
	getDhcpOption60ValueValue,
	setDhcpOption60ValueValue,
	getDhcpOption60ValueAttribute, 
	setDhcpOption60ValueAttribute, 
	NULL
};

CwmpParameterStruct CTComDhcpOption60[]=
{
	{"Enable", BooleanW, NoLimite, &DhcpOption60Enable, NULL},
	{"Type", IntW, NoLimite, &DhcpOption60Type, NULL},
	{"ValueMode", IntW, NoLimite, &DhcpOption60ValueMode, NULL},
	{"Value", StrW, NoLimite, &DhcpOption60Value, NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct cwmpInitDhcpOption60Index[]= 
{
	{NULL, ObjectR, NoLimite, NULL, CTComDhcpOption60},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc CTComDhcpOption60Funcs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitDhcpOption60SubTree
};

static NodeFunc DhcpOption125Enable=
{
	NULL,
	NULL,
	getDhcpOption125EnableValue,
	setDhcpOption125EnableValue,
	getDhcpOption125EnableAttribute, 
	setDhcpOption125EnableAttribute, 
	NULL
};

static NodeFunc DhcpOption125Value=
{
	NULL,
	NULL,
	getDhcpOption125ValueValue,
	setDhcpOption125ValueValue,
	getDhcpOption125ValueAttribute, 
	setDhcpOption125ValueAttribute, 
	NULL
};

static NodeFunc DhcpOption125SubCode=
{
	NULL,
	NULL,
	getDhcpOption125SubCodeValue,
	setDhcpOption125SubCodeValue,
	getDhcpOption125SubCodeAttribute, 
	setDhcpOption125SubCodeAttribute, 
	NULL
};

static NodeFunc DhcpOption125SubData=
{
	NULL,
	NULL,
	getDhcpOption125SubDataValue,
	setDhcpOption125SubDataValue,
	getDhcpOption125SubDataAttribute, 
	setDhcpOption125SubDataAttribute, 
	NULL
};

static NodeFunc DhcpOption125Type=
{
	NULL,
	NULL,
	getDhcpOption125TypeValue,
	setDhcpOption125TypeValue,
	getDhcpOption125TypeAttribute,
	setDhcpOption125TypeAttribute,
	NULL
};

CwmpParameterStruct CTComDhcpOption125[]=
{
	{"Enable", BooleanW, NoLimite, &DhcpOption125Enable, NULL},
	{"Value", StrW, NoLimite, &DhcpOption125Value, NULL},
	{"subopt-code", IntW, NoLimite, &DhcpOption125SubCode, NULL},
	{"sub-option-data", StrW, NoLimite, &DhcpOption125SubData, NULL},
	{"Type", IntW, NoLimite, &DhcpOption125Type, NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct cwmpInitDhcpOption125Index[]= 
{
	{NULL, ObjectR, NoLimite, NULL, CTComDhcpOption125},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc CTComDhcpOption125Funcs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitDhcpOption125SubTree
};

static NodeFunc Dhcpv6Option17Enable=
{
	NULL,
	NULL,
	getDhcpv6Option17EnableValue,
	setDhcpv6Option17EnableValue,
	getDhcpv6Option17EnableAttribute, 
	setDhcpv6Option17EnableAttribute, 
	NULL
};
static NodeFunc Dhcpv6Option17Type=
{
	NULL,
	NULL,
	getDhcpv6Option17TypeValue,
	setDhcpv6Option17TypeValue,
	getDhcpv6Option17TypeAttribute, 
	setDhcpv6Option17TypeAttribute, 
	NULL
};
static NodeFunc Dhcpv6Option17Value=
{
	NULL,
	NULL,
	getDhcpv6Option17ValueValue,
	setDhcpv6Option17ValueValue,
	getDhcpv6Option17ValueAttribute, 
	setDhcpv6Option17ValueAttribute, 
	NULL
};

static NodeFunc DhcpOption17SubCode=
{
	NULL,
	NULL,
	getDhcpOption17SubCodeValue,
	setDhcpOption17SubCodeValue,
	getDhcpOption17SubCodeAttribute, 
	setDhcpOption17SubCodeAttribute, 
	NULL
};

static NodeFunc DhcpOption17SubData=
{
	NULL,
	NULL,
	getDhcpOption17SubDataValue,
	setDhcpOption17SubDataValue,
	getDhcpOption17SubDataAttribute,
	setDhcpOption17SubDataAttribute,
	NULL
};

CwmpParameterStruct CTComDhcpv6Option17[]=
{
	{"Enable", BooleanW, NoLimite, &Dhcpv6Option17Enable, NULL},
	{"Type", IntW, NoLimite, &Dhcpv6Option17Type, NULL},
	{"Value", StrW, NoLimite, &Dhcpv6Option17Value, NULL},
	{"subopt-code", IntW, NoLimite, &DhcpOption17SubCode, NULL},
	{"sub-option-data", StrW, NoLimite, &DhcpOption17SubData, NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct cwmpInitDhcpv6Option17Index[]= 
{
	{NULL, ObjectR, NoLimite, NULL, CTComDhcpv6Option17},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc CTComDhcpv6Option17Funcs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitDhcpv6Option17SubTree
};

static NodeFunc Dhcpv6Option16Enable=
{
	NULL,
	NULL,
	getDhcpv6Option16EnableValue,
	setDhcpv6Option16EnableValue,
	getDhcpv6Option16EnableAttribute, 
	setDhcpv6Option16EnableAttribute, 
	NULL
};

static NodeFunc Dhcpv6Option16Value=
{
	NULL,
	NULL,
	getDhcpv6Option16ValueValue,
	setDhcpv6Option16ValueValue,
	getDhcpv6Option16ValueAttribute, 
	setDhcpv6Option16ValueAttribute, 
	NULL
};

static NodeFunc Dhcpv6Option16Type=
{
	NULL,
	NULL,
	getDhcpOption16TypeValue,
	setDhcpOption16TypeValue,
	getDhcpOption16TypeAttribute,
	setDhcpOption16TypeAttribute,
	NULL
};

static NodeFunc Dhcpv6Option16ValueMode=
{
	NULL,
	NULL,
	getDhcpOption16ValueModeValue,
	setDhcpOption16ValueModeValue,
	getDhcpOption16ValueModeAttribute,
	setDhcpOption16ValueModeAttribute,
	NULL
};

CwmpParameterStruct CTComDhcpv6Option16[]=
{
	{"Enable", BooleanW, NoLimite, &Dhcpv6Option16Enable, NULL},
	{"Value", StrW, NoLimite, &Dhcpv6Option16Value, NULL},
	{"Type", IntW, NoLimite, &Dhcpv6Option16Type, NULL},
	{"ValueMode", IntW, NoLimite, &Dhcpv6Option16ValueMode, NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct cwmpInitDhcpv6Option16Index[]= 
{
	{NULL, ObjectR, NoLimite, NULL, CTComDhcpv6Option16},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc CTComDhcpv6Option16Funcs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitDhcpv6Option16SubTree
};

static NodeFunc IPv6_Alias=
{
	NULL,
	NULL,
	getIPv6AliasValue,
	NULL,
	getIPv6AliasAttribute, 
	setIPv6AliasAttribute, 
	NULL
};

static NodeFunc IPv6_IPAddress=
{
	NULL,
	NULL,
	getIPv6IPAddressValue,
	setIPv6IPAddressValue,
	getIPv6IPAddressAttribute, 
	setIPv6IPAddressAttribute, 
	NULL
};

static NodeFunc IPv6_DNSIPAddress=
{
	NULL,
	NULL,
	getIPv6DNSIPAddressValue,
	setIPv6DNSIPAddressValue,
	getIPv6DNSIPAddressAttribute, 
	setIPv6DNSIPAddressAttribute, 
	NULL
};

static NodeFunc IPv6_Origin=
{
	NULL,
	NULL,
	getIPv6OriginValue,
	setIPv6OriginValue,
	getIPv6OriginAttribute, 
	setIPv6OriginAttribute, 
	NULL
};

static NodeFunc IPv6_ChildPrefixBits=
{
	NULL,
	NULL,
	getIPv6ChildPrefixBitsValue,
	setIPv6ChildPrefixBitsValue,
	getIPv6ChildPrefixBitsAttribute, 
	setIPv6ChildPrefixBitsAttribute, 
	NULL
};

CwmpParameterStruct IPv6Address[] =
{
	{"Alias", StrR, NoLimite, &IPv6_Alias, NULL},
	{"IPAddress", StrW, NoLimite, &IPv6_IPAddress, NULL},
	{"DNSIPAddress", StrW, NoLimite, &IPv6_DNSIPAddress, NULL},
	{"Origin", StrW, NoLimite, &IPv6_Origin, NULL},
	{"ChildPrefixBits", StrW, NoLimite, &IPv6_ChildPrefixBits, NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc IPv6Prefix_Alias=
{
	NULL,
	NULL,
	getIPv6PrefixAliasValue,
	NULL,
	getIPv6PrefixAliasAttribute, 
	setIPv6PrefixAliasAttribute, 
	NULL
};

static NodeFunc IPv6Prefix_Prefix=
{
	NULL,
	NULL,
	getIPv6PrefixValue,
	setIPv6PrefixValue,
	getIPv6PrefixAttribute, 
	setIPv6PrefixAttribute, 
	NULL
};

static NodeFunc IPv6Prefix_Origin=
{
	NULL,
	NULL,
	getIPv6PrefixOriginValue,
	setIPv6PrefixOriginValue,
	getIPv6PrefixOriginAttribute, 
	setIPv6PrefixOriginAttribute, 
	NULL
};

static NodeFunc IPv6Prefix_ChildPrefixBits=
{
	NULL,
	NULL,
	getIPv6PrefixChildPrefixBitsValue,
	setIPv6PrefixChildPrefixBitsValue,
	getIPv6PrefixChildPrefixBitsAttribute, 
	setIPv6PrefixChildPrefixBitsAttribute, 
	NULL
};

static NodeFunc IPv6_PrefixPltime=
{
	NULL,
	NULL,
	getIPv6PrefixPltimeValue,
	setIPv6PrefixPltimeValue,
	getIPv6PrefixPltimeAttribute, 
	setIPv6PrefixPltimeAttribute, 
	NULL
};

static NodeFunc IPv6_PrefixVltime=
{
	NULL,
	NULL,
	getIPv6PrefixVltimeValue,
	setIPv6PrefixVltimeValue,
	getIPv6PrefixVltimeAttribute, 
	setIPv6PrefixVltimeAttribute, 
	NULL
};

static NodeFunc IPv6_PrefixDelegation=
{
	NULL,
	NULL,
	getIPv6PrefixDelegationValue,
	setIPv6PrefixDelegationValue,
	getIPv6PrefixDelegationAttribute, 
	setIPv6PrefixDelegationAttribute, 
	NULL
};

static NodeFunc IPv6_DefGateway=
{
	NULL,
	NULL,
	getIPv6DefGatewayValue,
	setIPv6DefGatewayValue,
	getIPv6DefGatewayAttribute, 
	setIPv6DefGatewayAttribute, 
	NULL
};

static NodeFunc IPv6_DomainName=
{
	NULL,
	NULL,
	getIPv6DomainNameValue,
	setIPv6DomainNameValue,
	getIPv6DomainNameAttribute, 
	setIPv6DomainNameAttribute, 
	NULL
};

CwmpParameterStruct IPv6Prefix[] =
{
	{"Alias", StrR, NoLimite, &IPv6Prefix_Alias, NULL},
	{"Prefix", StrW, NoLimite, &IPv6Prefix_Prefix, NULL},
	{"Origin", StrW, NoLimite, &IPv6Prefix_Origin, NULL},
	{"ChildPrefixBits", StrW, NoLimite, &IPv6Prefix_ChildPrefixBits, NULL},
#if 0
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"PreferredLifeTime", StrW, NoLimite, &IPv6_PrefixPltime, NULL},
	{"ValidLifeTime", StrW, NoLimite, &IPv6_PrefixVltime, NULL},
#endif
#endif
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct X_CTCOM_IPv6[] =
{
	{"IPv6Address", ObjectR, NoLimite,  NULL, IPv6Address},
	{"IPv6Prefix", ObjectR, NoLimite,	NULL, IPv6Prefix},
#if 0
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CU"IPv6ConnStatus", StrR, NoLimite, &CTComIPv6ConStatus, NULL},
	{X_CU"IPv6PrefixDelegationEnabled", BooleanW, NoLimite, &IPv6_PrefixDelegation, NULL},
	{X_CU"DefaultIPv6Gateway", StrW, NoLimite, &IPv6_DefGateway, NULL},
	{X_CU"IPv6DomainName", StrR, NoLimite, &IPv6_DomainName, NULL},
#endif
#endif
	{NULL, -1, -1, NULL, NULL}
};

#ifdef TCSUPPORT_DSLITE

static NodeFunc CTComDslite_Enable=
{
	NULL,
	NULL,
	getCTComDsliteEnableValue,
	setCTComDsliteEnableValue,
	getCTComDsliteEnableAttribute, 
	setCTComDsliteEnableAttribute, 
	NULL
};

static NodeFunc CTComAftrMode=
{
	NULL,
	NULL,
	getCTComAftrModeValue,
	setCTComAftrModeValue,
	getCTComAftrModeAttribute, 
	setCTComAftrModeAttribute, 
	NULL
};

static NodeFunc CTComAftr=
{
	NULL,
	NULL,
	getCTComAftrValue,
	setCTComAftrValue,
	getCTComAftrAttribute, 
	setCTComAftrAttribute, 
	NULL
};

#endif

static NodeFunc IPForwardList=
{
	NULL,
	NULL,
	getIPForwardListValue,
	setIPForwardListValue,
	getIPForwardListAttribute, 
	setIPForwardListAttribute, 
	NULL
};

static NodeFunc CTCom_IPv6_IPAddress=
{
	NULL,
	NULL,
	getXIPv6IPAddressValue,
	setXIPv6IPAddressValue,
	getXIPv6IPAddressAttribute, 
	setXIPv6IPAddressAttribute, 
	NULL
};

static NodeFunc IPv6_IPAddressAlias=
{
	NULL,
	NULL,
	getIPv6IPAddressAliasValue,
	setIPv6IPAddressAliasValue,
	getIPv6IPAddressAliasAttribute, 
	setIPv6IPAddressAliasAttribute, 
	NULL
};

static NodeFunc IPv6_IPAddressOrigin=
{
	NULL,
	NULL,
	getIPAddressOriginValue,
	setIPAddressOriginValue,
	getIPAddressOriginAttribute, 
	setIPAddressOriginAttribute, 
	NULL
};

static NodeFunc IPv6_DNSServers=
{
	NULL,
	NULL,
	getIPv6DNSServersValue,
	setIPv6DNSServersValue,
	getIPv6DNSServersAttribute, 
	setIPv6DNSServersAttribute, 
	NULL
};

static NodeFunc IPv6_PrefixAlias=
{
	NULL,
	NULL,
	getCTComIPv6PrefixAliasValue,
	setCTComIPv6PrefixAliasValue,
	getCTComIPv6PrefixAliasAttribute, 
	setCTComIPv6PrefixAliasAttribute, 
	NULL
};

static NodeFunc IPv6_PrefixOrigin=
{
	NULL,
	NULL,
	getCTComIPv6PrefixOriginValue,
	setCTComIPv6PrefixOriginValue,
	getCTComIPv6PrefixOriginAttribute, 
	setCTComIPv6PrefixOriginAttribute, 
	NULL
};

static NodeFunc IPv6_Prefix=
{
	NULL,
	NULL,
	getCTComIPv6PrefixValue,
	setCTComIPv6PrefixValue,
	getCTComIPv6PrefixAttribute, 
	setCTComIPv6PrefixAttribute, 
	NULL
};

#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
static NodeFunc IP_PPP_Reset=
{
	NULL,
	NULL,
	getWANCnt1_ResetValue,
	setWANCnt1_ResetValue,
	getWANCnt1_ResetAttribute, 
	setWANCnt1_ResetAttribute, 
	NULL
};

static NodeFunc IP_PPP_ForwardModeEnabled=
{
	NULL,
	NULL,
	getWANCnt1_ForwardModeEnabledValue,
	setWANCnt1_ForwardModeEnabledValue,
	getWANCnt1_ForwardModeEnabledAttribute, 
	setWANCnt1_ForwardModeEnabledAttribute, 
	NULL
};


static NodeFunc IP_PPP_Vlan_Enable=
{
	NULL,
	NULL,
	getWANCnt1_VlanEnableValue,
	setWANCnt1_VlanEnableValue,
	getWANCnt1_VlanEnableAttribute, 
	setWANCnt1_VlanEnableAttribute, 
	NULL
};

static NodeFunc IP_PPP_Vlan_ID=
{
	NULL,
	NULL,
	getWANCnt1_VlanIDValue,
	setWANCnt1_VlanIDValue,
	getWANCnt1_VlanIDAttribute, 
	setWANCnt1_VlanIDAttribute, 
	NULL
};

static NodeFunc IP_PPP_Priority=
{
	NULL,
	NULL,
	getWANCnt1_PriorityValue,
	setWANCnt1_PriorityValue,
	getWANCnt1_PriorityAttribute,
	setWANCnt1_PriorityAttribute,
	NULL
};

static NodeFunc IP_PPP_IgmpProxy=
{
	NULL,
	NULL,
	getWANCnt1_IgmpProxyValue,
	setWANCnt1_IgmpProxyValue,
	getWANCnt1_IgmpProxyAttribute, 
	setWANCnt1_IgmpProxyAttribute, 
	NULL
};
#endif

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.IPConnection.1.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
CwmpParameterStruct WANIPConnection[] = 
{
	{"Enable", BooleanW, NoLimite, &IP_PPP_Enable, NULL},
	{"ConnectionStatus", StrR, NoLimite, &ConnectionStatus, NULL},
	{"PossibleConnectionTypes", StrR, NoLimite, &PossibleConnectionTypes, NULL},
	{"ConnectionType", StrW, NoLimite, &ConnectionType, NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"Reset", BooleanW, NoLimite, &IP_PPP_Reset, NULL},
	//{X_CU"VLANEnabled", BooleanW, NoLimite, &IP_PPP_Vlan_Enable, NULL},
	//{X_CU"VLANID", UnsignIntW, NoLimite, &IP_PPP_Vlan_ID, NULL},
	//{X_CU"Priority", UnsignIntW, NoLimite, &IP_PPP_Priority, NULL},
	{X_CU"IGMPProxyEnable", BooleanW, NoLimite, &IP_PPP_IgmpProxy, NULL},
	{X_CU"IPForwardModeEnabled", BooleanW, NoLimite, &IP_PPP_ForwardModeEnabled, NULL},
#endif
	{"Name", StrW, NoLimite, &Name, NULL},
	{"Uptime", UnsignIntR, NoLimite, &Uptime, NULL},
	{"LastConnectionError", StrR, NoLimite, &LastConnectionError, NULL},
	{"RSIPAvailable", BooleanR, NoLimite, &RSIPAvailable, NULL},
	{"NATEnabled", BooleanW, NoLimite, &NATEnabled, NULL},
	{"AddressingType", StrW, NoLimite, &AddressingType, NULL},
	{"ExternalIPAddress", StrW, NoLimite, &IPExternalIPAddress, NULL},
	{"SubnetMask", StrW, NoLimite, &SubnetMask, NULL},
	{"DefaultGateway", StrW, NoLimite, &DefaultGateway, NULL},
	{"DNSEnabled", BooleanW, NoLimite, &DNSEnabled, NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"DNSOverrideAllowed", BooleanW, NoLimite, &DNSOverrideAllowed, NULL},
#else
	{"DNSOverrideAllowed", BooleanR, NoLimite, &DNSOverrideAllowed, NULL},
#endif
	{"DNSServers", StrW, NoLimite, &DNSServers, NULL},
	{"MACAddress", StrR, NoLimite, &MACAddress, NULL},
	{"RouteProtocolRx", StrW, NoLimite, &RouteProtocolRx, NULL},
	{"PortMappingNumberOfEntries", UnsignIntR, NoLimite, &PortMappingNumberOfEntries, NULL},
#if !defined(TCSUPPORT_C1_CUC)
	{X_CT_COM"ServiceList", StrW, NoLimite, &CTComServiceList, NULL},
	{X_CT_COM"LanInterface", StrW, NoLimite, &CTComInterfaceBond, NULL},
	{X_CT_COM"LanInterface-DHCPEnable", BooleanW, NoLimite, &CTComDHCPEnable, NULL},
#endif
	{"PortMapping", ObjectW, NoLimite, &PortMappingFuncs, NULL},
#if !defined(TCSUPPORT_CT_PON_JX) || defined(TCSUPPORT_CT_PON_HUB)
	{"Stats", ObjectR, NoLimite, NULL, Stats},
#endif
#if !defined(TCSUPPORT_C1_CUC) && !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#if defined(TCSUPPORT_CT_E8DDNS)
	{X_CT_COM"DDNSConfiguration", ObjectR, NoLimite, &CTComDDNSConfigurationFuncs, NULL},
#endif
#endif
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#if defined(TCSUPPORT_CT_PON_GDV20)
	{X_CT_COM"IPv6ConnStatus", StrR, NoLimite, &CTComIPv6ConStatus, NULL},
#else
	{X_CT_COM"IPv6ConnectionStatus", StrR, NoLimite, &CTComIPv6ConStatus, NULL},
#endif
	{X_CT_COM"IPMode", StrW, NoLimite, &CTComIPMode, NULL},
#else
	{X_CT_COM"IPv6ConnStatus", StrR, NoLimite, &CTComIPv6ConStatus, NULL},
	{X_CT_COM"IPMode", UnsignIntW, NoLimite, &CTComIPMode, NULL},
#endif
	{X_CT_COM"MulticastVlan", IntW, NoLimite, &CTComMulticastVlan, NULL},
	{X_CT_COM"DHCPOPTION60", ObjectR, NoLimite, &CTComDhcpOption60Funcs, NULL},
	{X_CT_COM"DHCPOPTION125", ObjectR, NoLimite, &CTComDhcpOption125Funcs, NULL},
	{X_CT_COM"DHCPv6OPTION17", ObjectR, NoLimite, &CTComDhcpv6Option17Funcs, NULL},
	{X_CT_COM"DHCPv6OPTION16", ObjectR, NoLimite, &CTComDhcpv6Option16Funcs, NULL},
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CT_COM"IPv6", ObjectR, NoLimite,  NULL, X_CTCOM_IPv6},
#endif
#ifdef TCSUPPORT_DSLITE
	{X_CT_COM"Dslite_Enable", BooleanW, NoLimite, &CTComDslite_Enable, NULL},	
	{X_CT_COM"AftrMode", IntW, NoLimite, &CTComAftrMode, NULL},	
	{X_CT_COM"Aftr", StrW, NoLimite, &CTComAftr, NULL},	
#endif
	{X_CT_COM"IPForwardList", StrW, NoLimite, &IPForwardList, NULL},
	{X_CT_COM"IPv6IPAddress", StrW, NoLimite, &CTCom_IPv6_IPAddress, NULL},
	{X_CT_COM"IPv6IPAddressAlias", StrW, NoLimite, &IPv6_IPAddressAlias, NULL},  //W
	{X_CT_COM"IPv6IPAddressOrigin", StrW, NoLimite, &IPv6_IPAddressOrigin, NULL},
	{X_CT_COM"IPv6DNSServers", StrW, NoLimite, &IPv6_DNSServers, NULL},
	{X_CT_COM"IPv6PrefixDelegationEnabled", BooleanW, NoLimite, &IPv6_PrefixDelegation, NULL},
	{X_CT_COM"IPv6PrefixAlias", StrW, NoLimite, &IPv6_PrefixAlias, NULL},  //W
	{X_CT_COM"IPv6PrefixOrigin", StrW, NoLimite, &IPv6_PrefixOrigin, NULL},
	{X_CT_COM"IPv6Prefix", StrW, NoLimite, &IPv6_Prefix, NULL},
	{X_CT_COM"IPv6PrefixPltime", UnsignIntW, NoLimite, &IPv6_PrefixPltime, NULL},
	{X_CT_COM"IPv6PrefixVltime", UnsignIntW, NoLimite, &IPv6_PrefixVltime, NULL},
	{X_CT_COM"DefaultIPv6Gateway", StrW, NoLimite, &IPv6_DefGateway, NULL},
	{X_CT_COM"IPv6DomainName", StrW, NoLimite, &IPv6_DomainName, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc Username=
{
	NULL,
	NULL,
	getWANPPPCnt1_1UsernameValue,
	setWANPPPCnt1_1UsernameValue,
	getWANPPPCnt1_1UsernameAttribute, 
	setWANPPPCnt1_1UsernameAttribute, 
	NULL
};

static NodeFunc Password=
{
	NULL,
	NULL,
	getWANPPPCnt1_1PasswordValue,
	setWANPPPCnt1_1PasswordValue,
	getWANPPPCnt1_1PasswordAttribute, 
	setWANPPPCnt1_1PasswordAttribute, 
	NULL
};

static NodeFunc TransportType=
{
	NULL,
	NULL,
	getWANPPPCnt1_1TransTypeValue,
	NULL,
	getWANPPPCnt1_1TransTypeAttribute, 
	setWANPPPCnt1_1TransTypeAttribute, 
	NULL
};

static NodeFunc PPPAuthenticationProtocol=
{
	NULL,
	NULL,
	getWANPPPCnt1_1AuthenTypeValue,
	setWANPPPCnt1_1AuthenTypeValue,
	getWANPPPCnt1_1AuthenTypeAttribute, 
	setWANPPPCnt1_1AuthenTypeAttribute, 
	NULL
};

static NodeFunc PPPoEACName=
{
	NULL,
	NULL,
	getWANPPPCnt1_1PPPoEACNameValue,
	setWANPPPCnt1_1PPPoEACNameValue,
	getWANPPPCnt1_1PPPoEACNameAttribute, 
	setWANPPPCnt1_1PPPoEACNameAttribute, 
	NULL
};

static NodeFunc PPPoEServiceName=
{
	NULL,
	NULL,
	getWANPPPCnt1_1PPPoESvcNameValue,
	setWANPPPCnt1_1PPPoESvcNameValue,
	getWANPPPCnt1_1PPPoESvcNameAttribute, 
	setWANPPPCnt1_1PPPoESvcNameAttribute, 
	NULL
};

static NodeFunc ConnectionTrigger=
{
	NULL,
	NULL,
	getWANCnt1_1CntTriggerValue,
	setWANCnt1_1CntTriggerValue,
	getWANIPCnt1_1CntTriggerAttribute, 
	setWANIPCnt1_1CntTriggerAttribute, 
	NULL
};
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
static NodeFunc IdleDisconnectTime=
{
	NULL,
	NULL,
	getWANCnt1_IdleDisconnectTimeValue,
	setWANCnt1_IdleDisconnectTimeValue,
	getWANCnt1_IdleDisconnectTimeAttribute,
	setWANCnt1_IdleDisconnectTimeAttribute,
	NULL
};
#endif

#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_PPPOEPROXY)
static NodeFunc PPP_Proxy_Enable=
{
	NULL,
	NULL,
	getWANCnt1_1ProxyEnableValue,
	setWANCnt1_1ProxyEnableValue,
	getWANIPCnt1_1ProxyEnableAttribute, 
	setWANIPCnt1_1ProxyEnableAttribute, 
	NULL
};
static NodeFunc PPP_Proxy_MaxUser=
{
	NULL,
	NULL,
	getWANCnt1_1ProxyMaxUserValue,
	setWANCnt1_1ProxyMaxUserValue,
	getWANIPCnt1_1ProxyMaxUserAttribute, 
	setWANIPCnt1_1ProxyMaxUserAttribute, 
	NULL
};
#endif
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER) && !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
static NodeFunc PPP_BI_Enable=
{
	NULL,
	NULL,
	getWANPPPCnt1_1PPPoEBIValue,
	setWANPPPCnt1_1PPPoEBIValue,
	getWANPPPCnt1_1PPPoEBIAttribute, 
	setWANPPPCnt1_1PPPoEBIAttribute, 
	NULL
};
#endif


#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
static NodeFunc LastConnectionErrorCode=
{
	NULL,
	NULL,
	getWANPPPCnt1_1LErrorCodeValue,
	NULL,
	getWANPPPCnt1_1LErrorCodeAttribute, 
	setWANPPPCnt1_1LErrorCodeAttribute, 
	NULL
};
#endif


/*
      This Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANPPPConnection.1.
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
CwmpParameterStruct WANPPPConnection[] = 
{
	{"Enable", BooleanW, NoLimite, &IP_PPP_Enable, NULL},
	{"ConnectionStatus", StrR, NoLimite, &ConnectionStatus, NULL},
	{"PossibleConnectionTypes", StrR, NoLimite, &PossibleConnectionTypes, NULL},
	{"ConnectionType", StrW, NoLimite, &ConnectionType, NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"Reset", BooleanW, NoLimite, &IP_PPP_Reset, NULL},
	{X_CU"IPForwardModeEnabled", BooleanW, NoLimite, &IP_PPP_ForwardModeEnabled, NULL},
	//{X_CU"VLANEnabled", BooleanW, NoLimite, &IP_PPP_Vlan_Enable, NULL},
	//{X_CU"VLANID", UnsignIntW, NoLimite, &IP_PPP_Vlan_ID, NULL},
	//{X_CU"Priority", UnsignIntW, NoLimite, &IP_PPP_Priority, NULL},
	{X_CU"IGMPProxyEnable", BooleanW, NoLimite, &IP_PPP_IgmpProxy, NULL},
#endif
	{"Name", StrW, NoLimite, &Name, NULL},
	{"Uptime", UnsignIntR, NoLimite, &Uptime, NULL},
	{"LastConnectionError", StrR, NoLimite, &LastConnectionError, NULL},
	{"RSIPAvailable", BooleanR, NoLimite, &RSIPAvailable, NULL},
	{"NATEnabled", BooleanW, NoLimite, &NATEnabled, NULL},
	{"AddressingType", StrW, NoLimite, &AddressingType, NULL},
	{"ExternalIPAddress", StrR, NoLimite, &PPPExternalIPAddress, NULL},
	{"SubnetMask", StrW, NoLimite, &SubnetMask, NULL},
	{"DefaultGateway", StrW, NoLimite, &DefaultGateway, NULL},
	{"DNSEnabled", BooleanW, NoLimite, &DNSEnabled, NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"DNSOverrideAllowed", BooleanW, NoLimite, &DNSOverrideAllowed, NULL},
#else
	{"DNSOverrideAllowed", BooleanR, NoLimite, &DNSOverrideAllowed, NULL},
#endif
	{"DNSServers", StrW, NoLimite, &DNSServers, NULL},
	{"MACAddress", StrR, NoLimite, &MACAddress, NULL},
	{"RouteProtocolRx", StrW, NoLimite, &RouteProtocolRx, NULL},
#if !defined(TCSUPPORT_C1_CUC)
	{X_CT_COM"ServiceList", StrW, NoLimite, &CTComServiceList, NULL},
	{X_CT_COM"LanInterface", StrW, NoLimite, &CTComInterfaceBond, NULL},
	{X_CT_COM"LanInterface-DHCPEnable", BooleanW, NoLimite, &CTComDHCPEnable, NULL},
#endif
	{"PortMappingNumberOfEntries", UnsignIntR, NoLimite, &PortMappingNumberOfEntries, NULL},
	{"PortMapping", ObjectW, NoLimite, &PortMappingFuncs, NULL},
#if !defined(TCSUPPORT_CT_PON_JX) || defined(TCSUPPORT_CT_PON_HUB)
	{"Stats", ObjectR, NoLimite, NULL, Stats},
#endif
	{"Username", StrW, NoLimite, &Username, NULL},
	{"Password", StrW, NoLimite, &Password, NULL},
	{"TransportType", StrR, NoLimite, &TransportType, NULL},
	{"PPPoEACName", StrW, NoLimite, &PPPoEACName, NULL},
	{"PPPoEServiceName", StrW, NoLimite, &PPPoEServiceName, NULL},
	{"ConnectionTrigger", StrW, NoLimite, &ConnectionTrigger, NULL},
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
	{"IdleDisconnectTime", UnsignIntW, NoLimite, &IdleDisconnectTime, NULL},
#endif
#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_PPPOEPROXY)
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CU"PPPoEProxyEnable", BooleanW, NoLimite, &PPP_Proxy_Enable, NULL},
	{X_CU"PPPoEProxyMaxUser", UnsignIntW, NoLimite, &PPP_Proxy_MaxUser, NULL},
#else
	{X_CT_COM"ProxyEnable", BooleanW, NoLimite, &PPP_Proxy_Enable, NULL},
	{X_CT_COM"MAXUser", UnsignIntW, NoLimite, &PPP_Proxy_MaxUser, NULL},
#endif
#endif
#endif

#if !defined(TCSUPPORT_C1_CUC) && !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#if defined(TCSUPPORT_CT_E8DDNS)
	{X_CT_COM"DDNSConfiguration", ObjectR, NoLimite, &CTComDDNSConfigurationFuncs, NULL},
#endif
#endif
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#if defined(TCSUPPORT_CT_PON_GDV20)
	{X_CT_COM"IPv6ConnStatus", StrR, NoLimite, &CTComIPv6ConStatus, NULL},
#else
	{X_CT_COM"IPv6ConnectionStatus", StrR, NoLimite, &CTComIPv6ConStatus, NULL},
#endif
    {X_CT_COM"IPMode", StrW, NoLimite, &CTComIPMode, NULL},
#else
	{X_CT_COM"IPv6ConnStatus", StrR, NoLimite, &CTComIPv6ConStatus, NULL},	
    {X_CT_COM"IPMode", UnsignIntW, NoLimite, &CTComIPMode, NULL},
#endif
	{X_CT_COM"MulticastVlan", IntW, NoLimite, &CTComMulticastVlan, NULL},
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CT_COM"IPv6", ObjectR, NoLimite,  NULL, X_CTCOM_IPv6},
	{X_CT_COM"AddPppToBridge", BooleanW, NoLimite, &PPP_BI_Enable, NULL},
#endif
#ifdef TCSUPPORT_DSLITE
	{X_CT_COM"Dslite_Enable", BooleanW, NoLimite, &CTComDslite_Enable, NULL},	
	{X_CT_COM"AftrMode", IntW, NoLimite, &CTComAftrMode, NULL},	
	{X_CT_COM"Aftr", StrW, NoLimite, &CTComAftr, NULL},
#endif
	{X_CT_COM"IPForwardList", StrW, NoLimite, &IPForwardList, NULL},
	{X_CT_COM"IPv6IPAddress", StrW, NoLimite, &CTCom_IPv6_IPAddress, NULL},
	{X_CT_COM"IPv6IPAddressAlias", StrW, NoLimite, &IPv6_IPAddressAlias, NULL},	//W
	{X_CT_COM"IPv6IPAddressOrigin", StrW, NoLimite, &IPv6_IPAddressOrigin, NULL},
	{X_CT_COM"IPv6DNSServers", StrW, NoLimite, &IPv6_DNSServers, NULL},
	{X_CT_COM"IPv6PrefixDelegationEnabled", BooleanW, NoLimite, &IPv6_PrefixDelegation, NULL},
	{X_CT_COM"IPv6PrefixAlias", StrW, NoLimite, &IPv6_PrefixAlias, NULL},  //W
	{X_CT_COM"IPv6PrefixOrigin", StrW, NoLimite, &IPv6_PrefixOrigin, NULL},
	{X_CT_COM"IPv6Prefix", StrW, NoLimite, &IPv6_Prefix, NULL},
	{X_CT_COM"IPv6PrefixPltime", UnsignIntW, NoLimite, &IPv6_PrefixPltime, NULL},
	{X_CT_COM"IPv6PrefixVltime", UnsignIntW, NoLimite, &IPv6_PrefixVltime, NULL},
	{X_CT_COM"DefaultIPv6Gateway", StrW, NoLimite, &IPv6_DefGateway, NULL},
	{X_CT_COM"IPv6DomainName", StrW, NoLimite, &IPv6_DomainName, NULL},
#endif
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
	{"LastConnectionErrorCode", StrR, NoLimite, &LastConnectionErrorCode, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};

