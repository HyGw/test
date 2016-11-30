#include "Global_res.h"
#include "cp_landev.h"

#ifdef TCSUPPORT_WLAN
static NodeFunc WLANConfigurationFuncs=
{
	addWLANConfigurationObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitWLANConfiguration1SubTree,
};

static NodeFunc WLANConfiguration_D=
{
	NULL,
	delWLANConfigurationObject,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL,
};

/*
      This Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}
        Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.
        Parent Node Table Location: cp_landevice_nodetable.c
*/
CwmpParameterStruct WLANConfigurationIndex[]=
{
        {NULL,ObjectW, NoLimite, &WLANConfiguration_D, WLANConfiguration},
        {NULL, -1, -1, NULL, NULL}
};
#endif

static NodeFunc LANEthIfNumOfEntries1=
{
	NULL,
	NULL,
	getLANEthIfNumOfEntries1Value,	 
	NULL,
	getLANEthIfNumOfEntries1Attribute, 
	setLANEthIfNumOfEntries1Attribute,	
	NULL,
};

static NodeFunc LANUSBIfNumOfEntries1=
{
        NULL,
        NULL,
        getLANUSBIfNumOfEntries1Value,
        NULL,
        getLANUSBIfNumOfEntries1Attribute,
        setLANUSBIfNumOfEntries1Attribute,
        NULL,
};

static NodeFunc LANWLANConfigNumOfEntries1=
{
        NULL,
        NULL,
        getLANWLANConfigNumOfEntries1Value,
        NULL,
        getLANWLANConfigNumOfEntries1Attribute,
        setLANWLANConfigNumOfEntries1Attribute,
        NULL,
};

static NodeFunc DHCPServerCfg1=
{
        NULL,
        NULL,
        getDHCPServerCfg1Value,
        setDHCPServerCfg1Value,
        getDHCPServerCfg1Attribute,
        setDHCPServerCfg1Attribute,
        NULL,
};

static NodeFunc DHCPServerEnable1=
{
        NULL,
        NULL,
        getDHCPServerEnable1Value,
        setDHCPServerEnable1Value,
        getDHCPServerEnable1Attribute,
        setDHCPServerEnable1Attribute,
        NULL,
};

static NodeFunc DHCPRelay1=
{
        NULL,
        NULL,
        getDHCPRelay1Value,
        NULL,
        getDHCPRelay1Attribute,
        setDHCPRelay1Attribute,
        NULL,
};

static NodeFunc MinAddress1=
{
        NULL,
        NULL,
        getMinAddress1Value,
        setMinAddress1Value,
        getMinAddress1Attribute,
        setMinAddress1Attribute,
        NULL,
};

static NodeFunc MaxAddress1=
{
        NULL,
        NULL,
        getMaxAddress1Value,
        setMaxAddress1Value,
        getMaxAddress1Attribute,
        setMaxAddress1Attribute,
        NULL,
};

static NodeFunc ReservedAddr1=
{
        NULL,
        NULL,
        getReservedAddr1Value,
        setReservedAddr1Value,
        getReservedAddr1Attribute,
        setReservedAddr1Attribute,
        NULL,
};

static NodeFunc SubnetMask1=
{
        NULL,
        NULL,
        getSubnetMask1Value,
        setSubnetMask1Value,
        getSubnetMask1Attribute,
        setSubnetMask1Attribute,
        NULL,
};

static NodeFunc DNSServers1=
{
        NULL,
        NULL,
        getDNSServers1Value,
        setDNSServers1Value,
        getDNSServers1Attribute,
        setDNSServers1Attribute,
        NULL,
};

static NodeFunc DomainName1=
{
        NULL,
        NULL,
        getDomainName1Value,
        setDomainName1Value,
        getDomainName1Attribute,
        setDomainName1Attribute,
        NULL,
};

static NodeFunc IPRouters1=
{
        NULL,
        NULL,
        getIPRouters1Value,
        setIPRouters1Value,
        getIPRouters1Attribute,
        setIPRouters1Attribute,
        NULL,
};

static NodeFunc DHCPLeaseTime1=
{
        NULL,
        NULL,
        getDHCPLeaseTime1Value,
        setDHCPLeaseTime1Value,
        getDHCPLeaseTime1Attribute,
        setDHCPLeaseTime1Attribute,
        NULL,
};

static NodeFunc IPInterfaceNum=
{
        NULL,
        NULL,
        getIPInterfaceNumValue,
        NULL,
        getIPInterfaceNumAttribute,
        setIPInterfaceNumAttribute,
        NULL,
};

static NodeFunc IPInterfaceFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitLANIPInterfaceSubTree,
};

static NodeFunc EthInterCfgFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitLANEthInterCfgSubTree,
};

#ifdef DHCP_PROFILE
static NodeFunc DHCPOptionNum=
{
        NULL,
        NULL,
        getDHCPOptionNumValue,
        NULL,
        getDHCPOptionNumAttribute,
        setDHCPOptionNumAttribute,
        NULL,
};

static NodeFunc DHCPConPoolNum=
{
        NULL,
        NULL,
        getDHCPConditionalPoolNumOfEntriesValue,
        NULL,
        getDHCPConditionalPoolNumOfEntriesAttribute,
        setDHCPConditionalPoolNumOfEntriesAttribute,
        NULL,
};

static NodeFunc DHCPOptionFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitDHCPOptionSubTree,
};

static NodeFunc DHCPConSerPoolFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitDHCPConSerPoolSubTree,
};
#endif
static NodeFunc AssociatedConnection1=
{
        NULL,
        NULL,
        getAssociatedConnection1Value,
        setAssociatedConnection1Value,
        getAssociatedConnection1Attribute,
        setAssociatedConnection1Attribute,
        NULL,
};
static NodeFunc UseAllocatedWAN1=
{
        NULL,
        NULL,
        getUseAllocatedWAN1Value,
        setUseAllocatedWAN1Value,
        getUseAllocatedWAN1Attribute,
        setUseAllocatedWAN1Attribute,
        NULL,
};
static NodeFunc PassthroughLease1=
{
        NULL,
        NULL,
        getPassthroughLease1Value,
        setPassthroughLease1Value,
        getPassthroughLease1Attribute,
        setPassthroughLease1Attribute,
        NULL,
};
static NodeFunc PassthroughMACAddress1=
{
        NULL,
        NULL,
        getPassthroughMACAddress1Value,
        setPassthroughMACAddress1Value,
        getPassthroughMACAddress1Attribute,
        setPassthroughMACAddress1Attribute,
        NULL,
};
static NodeFunc AllowedMACAddress1=
{
        NULL,
        NULL,
        getAllowedMACAddress1Value,
        setAllowedMACAddress1Value,
        getAllowedMACAddress1Attribute,
        setAllowedMACAddress1Attribute,
        NULL,
};

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
static NodeFunc CTComHGWMinAddress=
{
	NULL,
	NULL,
	getHGWMinAddressValue,
	setHGWMinAddressValue,
	getHGWMinAddressAttribute,
	setHGWMinAddressAttribute,
	NULL,
};

static NodeFunc CTComHGWMaxAddress=
{
	NULL,
	NULL,
	getHGWMaxAddressValue,
	setHGWMaxAddressValue,
	getHGWMaxAddressAttribute,
	setHGWMaxAddressAttribute,
	NULL,
};

static NodeFunc CTComSTBMinAddress=
{
	NULL,
	NULL,
	getSTBMinAddressValue,
	setSTBMinAddressValue,
	getSTBMinAddressAttribute,
	setSTBMinAddressAttribute,
	NULL,
};

static NodeFunc CTComSTBMaxAddress=
{
	NULL,
	NULL,
	getSTBMaxAddressValue,
	setSTBMaxAddressValue,
	getSTBMaxAddressAttribute,
	setSTBMaxAddressAttribute,
	NULL,
};

static NodeFunc CTComPhoneMinAddress=
{
	NULL,
	NULL,
	getPhoneMinAddressValue,
	setPhoneMinAddressValue,
	getPhoneMinAddressAttribute,
	setPhoneMinAddressAttribute,
	NULL,
};

static NodeFunc CTComPhoneMaxAddress=
{
	NULL,
	NULL,
	getPhoneMaxAddressValue,
	setPhoneMaxAddressValue,
	getPhoneMaxAddressAttribute,
	setPhoneMaxAddressAttribute,
	NULL,
};

static NodeFunc CTComCameraMinAddress=
{
	NULL,
	NULL,
	getCameraMinAddressValue,
	setCameraMinAddressValue,
	getCameraMinAddressAttribute,
	setCameraMinAddressAttribute,
	NULL,
};

static NodeFunc CTComCameraMaxAddress=
{
	NULL,
	NULL,
	getCameraMaxAddressValue,
	setCameraMaxAddressValue,
	getCameraMaxAddressAttribute,
	setCameraMaxAddressAttribute,
	NULL,
};

static NodeFunc CTComComputerMinAddress=
{
	NULL,
	NULL,
	getComputerMinAddressValue,
	setComputerMinAddressValue,
	getComputerMinAddressAttribute,
	setComputerMinAddressAttribute,
	NULL,
};

static NodeFunc CTComComputerMaxAddress=
{
	NULL,
	NULL,
	getComputerMaxAddressValue,
	setComputerMaxAddressValue,
	getComputerMaxAddressAttribute,
	setComputerMaxAddressAttribute,
	NULL,
};

static NodeFunc OPTION60Enable=
{
        NULL,
        NULL,
        getOPTION60EnableValue,
        setOPTION60EnableValue,
        getOPTION60EnableAttribute,
        setOPTION60EnableAttribute,
        NULL,
};

static NodeFunc OPTION125Enable=
{
        NULL,
        NULL,
        getOPTION125EnableValue,
        setOPTION125EnableValue,
        getOPTION125EnableAttribute,
        setOPTION125EnableAttribute,
        NULL,
};

static NodeFunc OPTION16Enable=
{
        NULL,
        NULL,
        getOPTION16EnableValue,
        setOPTION16EnableValue,
        getOPTION16EnableAttribute,
        setOPTION16EnableAttribute,
        NULL,
};

static NodeFunc OPTION17Enable=
{
        NULL,
        NULL,
        getOPTION17EnableValue,
        setOPTION17EnableValue,
        getOPTION17EnableAttribute,
        setOPTION17EnableAttribute,
        NULL,
};


#endif

/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement
	Parent Node Table Location: cp_landev_nodetable.c
*/

CwmpParameterStruct LANHostCfgManagement[] = 
{
#if !defined(TCSUPPORT_CT_PON_JX) || defined(TCSUPPORT_CT_PON_HUB)
	{"DHCPServerConfigurable", BooleanW, NoLimite, &DHCPServerCfg1, NULL},
	{"DHCPServerEnable", BooleanW, NoLimite, &DHCPServerEnable1, NULL},
	{"DHCPRelay", BooleanR, NoLimite, &DHCPRelay1, NULL},
	{"MinAddress", StrW, NoLimite, &MinAddress1, NULL},
	{"MaxAddress", StrW, NoLimite, &MaxAddress1, NULL},
	{"ReservedAddresses", StrW, NoLimite, &ReservedAddr1, NULL},
	{"SubnetMask", StrW, NoLimite, &SubnetMask1, NULL},
	{"DNSServers", StrW, NoLimite, &DNSServers1, NULL},
	{"DomainName", StrW, NoLimite, &DomainName1, NULL},
	{"IPRouters", StrW, NoLimite, &IPRouters1, NULL},
	{"DHCPLeaseTime", IntW, NoLimite, &DHCPLeaseTime1, NULL},
	{"IPInterfaceNumberOfEntries", UnsignIntR, NoLimite, &IPInterfaceNum, NULL},
	{"IPInterface", ObjectR, NoLimite, &IPInterfaceFuncs, NULL},
	#ifdef DHCP_PROFILE
	{"DHCPOptionNumberOfEntries", UnsignIntR, NoLimite, &DHCPOptionNum, NULL},
	{"DHCPConditionalPoolNumberOfEntries", UnsignIntR, NoLimite, &DHCPConPoolNum, NULL},
	{"DHCPOption", ObjectR, NoLimite, &DHCPOptionFuncs, NULL},
	{"DHCPConditionalServingPool", ObjectR, NoLimite, &DHCPConSerPoolFuncs, NULL},
	#endif
	{"AssociatedConnection", StrW, NoLimite, &AssociatedConnection1, NULL},
	{"UseAllocatedWAN", StrW, NoLimite, &UseAllocatedWAN1, NULL},
	{"PassthroughLease", UnsignIntW, NoLimite, &PassthroughLease1, NULL},
	{"PassthroughMACAddress", StrW, NoLimite, &PassthroughMACAddress1, NULL},
	{"AllowedMACAddress", StrW, NoLimite, &AllowedMACAddress1, NULL},
#endif
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	{X_CT_COM"HGW-MinAddress", StrW, NoLimite, &CTComHGWMinAddress, NULL},
	{X_CT_COM"HGW-MaxAddress", StrW, NoLimite, &CTComHGWMaxAddress, NULL},
	{X_CT_COM"STB-MinAddress", StrW, NoLimite, &CTComSTBMinAddress, NULL},
	{X_CT_COM"STB-MaxAddress", StrW, NoLimite, &CTComSTBMaxAddress, NULL},
	{X_CT_COM"Phone-MinAddress", StrW, NoLimite, &CTComPhoneMinAddress, NULL},
	{X_CT_COM"Phone-MaxAddress", StrW, NoLimite, &CTComPhoneMaxAddress, NULL},
	{X_CT_COM"Camera-MinAddress", StrW, NoLimite, &CTComCameraMinAddress, NULL},
	{X_CT_COM"Camera-MaxAddress", StrW, NoLimite, &CTComCameraMaxAddress, NULL},
	{X_CT_COM"Computer-MinAddress", StrW, NoLimite, &CTComComputerMinAddress, NULL},
	{X_CT_COM"Computer-MaxAddress", StrW, NoLimite, &CTComComputerMaxAddress, NULL},
	{X_CT_COM"OPTION60Enable", BooleanW, NoLimite, &OPTION60Enable, NULL},
	{X_CT_COM"OPTION125Enable", BooleanW, NoLimite, &OPTION125Enable, NULL},
	{X_CT_COM"OPTION16Enable", BooleanW, NoLimite, &OPTION16Enable, NULL},
	{X_CT_COM"OPTION17Enable", BooleanW, NoLimite, &OPTION17Enable, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
static NodeFunc LanIPv6Config_DomainName=
{
	NULL,
	NULL,
	getLanIPv6CfgDomainNameValue,
	setLanIPv6CfgDomainNameValue,
	getLanIPv6CfgDomainNameAttribute, 
	setLanIPv6CfgDomainNameAttribute, 
	NULL
};

static NodeFunc LanIPv6Config_DNSConfigType=
{
	NULL,
	NULL,
	getLanIPv6CfgDNSConfigTypeValue,
	setLanIPv6CfgDNSConfigTypeValue,
	getLanIPv6CfgDNSConfigTypeAttribute, 
	setLanIPv6CfgDNSConfigTypeAttribute, 
	NULL
};

static NodeFunc LanIPv6Config_DNSWANConnection=
{
	NULL,
	NULL,
	getLanIPv6CfgDNSWANConnectionValue,
	setLanIPv6CfgDNSWANConnectionValue,
	getLanIPv6CfgDNSWANConnectionAttribute, 
	setLanIPv6CfgDNSWANConnectionAttribute, 
	NULL
};

static NodeFunc LanIPv6Config_DNSServers=
{
	NULL,
	NULL,
	getLanIPv6CfgDNSServersValue,
	setLanIPv6CfgDNSServersValue,
	getLanIPv6CfgDNSServersAttribute, 
	setLanIPv6CfgDNSServersAttribute, 
	NULL
};

static NodeFunc LanRadvdPrefixMode=
{
	NULL,
	NULL,
	getLanIPv6CfgRadvdPrefixModeValue,
	setLanIPv6CfgRadvdPrefixModeValue,
	getLanIPv6CfgRadvdPrefixModeAttribute, 
	setLanIPv6CfgRadvdPrefixModeAttribute, 
	NULL
};

static NodeFunc LanRadvdDelegatedWanConn=
{
	NULL,
	NULL,
	getLanIPv6CfgRadvdDelegatedWanConnValue,
	setLanIPv6CfgRadvdDelegatedWanConnValue,
	getLanIPv6CfgRadvdDelegatedWanConnAttribute, 
	setLanIPv6CfgRadvdDelegatedWanConnAttribute, 
	NULL
};

static NodeFunc LanRadvdPrefix=
{
	NULL,
	NULL,
	getLanIPv6CfgRadvdPrefixValue,
	setLanIPv6CfgRadvdPrefixValue,
	getLanIPv6CfgRadvdPrefixAttribute, 
	setLanIPv6CfgRadvdPrefixAttribute, 
	NULL
};

static NodeFunc LanRadvdPltime=
{
	NULL,
	NULL,
	getLanIPv6CfgRadvdPltimeValue,
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	NULL,
#else
	setLanIPv6CfgRadvdPltimeValue,
#endif
	getLanIPv6CfgRadvdPltimeAttribute, 
	setLanIPv6CfgRadvdPltimeAttribute, 
	NULL
};

static NodeFunc LanRadvdVltime=
{
	NULL,
	NULL,
	getLanIPv6CfgRadvdVltimeValue,
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	NULL,
#else
	setLanIPv6CfgRadvdVltimeValue,
#endif
	getLanIPv6CfgRadvdVltimeAttribute, 
	setLanIPv6CfgRadvdVltimeAttribute, 
	NULL
};

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER) && defined(TCSUPPORT_WLAN)
static NodeFunc WLANEnable=
{
	NULL,
	NULL,
	getWLANCfgEnableValue,
	setWLANCfgEnableValue,
	getWLANCfgEnableAttribute,
	setWLANCfgEnableAttribute,
	NULL,
};
#endif

CwmpParameterStruct LanIPv6Config_PrefixInfo[] =
{
	{"Mode", StrW, NoLimite, &LanRadvdPrefixMode,  NULL},
	{"DelegatedWanConnection", StrW, NoLimite, &LanRadvdDelegatedWanConn, NULL},
	{"Prefix", StrW, NoLimite, &LanRadvdPrefix, NULL},
	{"PreferredLifeTime", UnsignIntW, NoLimite, &LanRadvdPltime, NULL},
	{"ValidLifeTime", UnsignIntW, NoLimite, &LanRadvdVltime, NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct LanIPv6Config_PrefixInfoIndex[] = 
{
	{"1",ObjectR, NoLimite,NULL,LanIPv6Config_PrefixInfo},
	{NULL, -1, -1, NULL, NULL}	
};


CwmpParameterStruct Lan_IPv6Config[] = 
{
	{"DomainName", StrW, NoLimite, &LanIPv6Config_DomainName, NULL},
	{"IPv6DNSConfigType", StrW, NoLimite, &LanIPv6Config_DNSConfigType, NULL},
	{"IPv6DNSWANConnection", StrW, NoLimite, &LanIPv6Config_DNSWANConnection, NULL},
	{"IPv6DNSServers", StrW, NoLimite, &LanIPv6Config_DNSServers, NULL},
	{"PrefixInformation", ObjectR, NoLimite, NULL, LanIPv6Config_PrefixInfoIndex},
	{NULL, -1, -1, NULL, NULL}
};
#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER) && defined(TCSUPPORT_CT_USB)
static NodeFunc USBPortStatus=
{
	NULL,
	NULL,
	getUSBPortStatusValue,
	NULL, 
	getUSBPortStatusAttribute, 
	setUSBPortStatusAttribute, 
	NULL
};

CwmpParameterStruct LANUSBInterfaceConfig[] = 
{
	{X_CU"USBPortStatus", StrR, NoLimite, &USBPortStatus, NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct LANUSBInterfaceConfigIndex[] = 
{
	{"1",ObjectR, NoLimite,NULL,LANUSBInterfaceConfig},
	{NULL, -1, -1, NULL, NULL}
};
#endif

/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.
	Parent Node Table Location: cp_landev_nodetable.c
*/
static CwmpParameterStruct LanDevice[] = 
{
#ifdef TCSUPPORT_WLAN
	{"WLANConfiguration", ObjectW, NoLimite, &WLANConfigurationFuncs, NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CU"WLANEnable", BooleanW, NoLimite, &WLANEnable, NULL},
#endif
#endif
#if !defined(TCSUPPORT_CT_PON_JX) || defined(TCSUPPORT_CT_PON_HUB)
	{"LANEthernetInterfaceNumberOfEntries", UnsignIntR, NoLimite, &LANEthIfNumOfEntries1, NULL},
	{"LANUSBInterfaceNumberOfEntries", UnsignIntR, NoLimite, &LANUSBIfNumOfEntries1, NULL},
	{"LANWLANConfigurationNumberOfEntries", UnsignIntR, NoLimite, &LANWLANConfigNumOfEntries1, NULL},
#endif
	{"LANHostConfigManagement", ObjectR, NoLimite, NULL, LANHostCfgManagement},	
	{"LANEthernetInterfaceConfig", ObjectR, NoLimite, &EthInterCfgFuncs, NULL},	
	{"Hosts",ObjectR, NoLimite,NULL, Hosts},
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CT_COM"DHCPv6", ObjectR, NoLimite,  NULL, Lan_X_CTCOM_DHCPv6},
#endif
	{X_CT_COM"RouterAdvertisement", ObjectR, NoLimite,  NULL, Lan_X_CTCOM_RouterAdvertisement},
	{X_CT_COM"DataSpeedLimit", ObjectR, NoLimite,  NULL, LANDataSpeedLimit},
	{X_CT_COM"DHCPv6Server", ObjectR, NoLimite,  NULL, LANDHCPv6Server},
	{X_CT_COM"IPv6Config",ObjectR, NoLimite,NULL, Lan_IPv6Config},
#endif
#if defined(TCSUPPORT_CT_LOOPDETECT)
	{X_CT_COM"LoopbackDetection", ObjectR, NoLimite,  NULL, LANLoopbackDetection},
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER) && defined(TCSUPPORT_CT_USB)
	{"LANUSBInterfaceConfig", ObjectR, NoLimite,  NULL, LANUSBInterfaceConfigIndex},
#endif

	{NULL, -1, -1, NULL, NULL}
};

/*
	This Node:    InternetGatewayDevice.LANDevice.{i}
	Parent Node: InternetGatewayDevice.LANDevice.
	Parent Node Table Location: cp_topnodetable.c 
*/
CwmpParameterStruct LanDeviceIndex[] = 
{
	{"1",ObjectR, NoLimite,NULL,LanDevice},
	{NULL, -1, -1, NULL, NULL}	
};
