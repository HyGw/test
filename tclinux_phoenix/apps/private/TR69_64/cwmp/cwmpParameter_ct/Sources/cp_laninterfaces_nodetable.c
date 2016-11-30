#include "Global_res.h"
#include "cp_laninterfaces.h"


static NodeFunc MACAddressNum=
{
	NULL,
	NULL,
	getMACAddressNumValue, 
	setMACAddressNumValue, 
	getMACAddressNumAttribute, 
	setMACAddressNumAttribute, 
	NULL,
};

static NodeFunc LanEtherTypeFilter=
{
	NULL,
	NULL,
	getLanEtherTypeFilterValue, 
	setLanEtherTypeFilterValue, 
	getLanEtherTypeFilterAttribute, 
	setLanEtherTypeFilterAttribute, 
	NULL,
};

static NodeFunc EthInterCfg_X_CMCC_Mode=
{
	NULL,
	NULL, 
	getEthInterCfg_X_CMCC_ModeValue,
	setEthInterCfg_X_CMCC_ModeValue,
	getEthInterCfg_X_CMCC_ModeAttribute, 
	setEthInterCfg_X_CMCC_ModeAttribute, 
	NULL,
};

static NodeFunc EthInterCfg_X_CMCC_VLAN=
{
	NULL,
	NULL, 
	getEthInterCfg_X_CMCC_VLANValue,
	setEthInterCfg_X_CMCC_VLANValue,
	getEthInterCfg_X_CMCC_VLANAttribute, 
	setEthInterCfg_X_CMCC_VLANAttribute, 
	NULL,
};

static NodeFunc WLanCfg_X_CMCC_Mode=
{
	NULL,
	NULL,
	getWLanCfg_X_CMCC_ModeValue,
	setWLanCfg_X_CMCC_ModeValue,
	getWLanCfg_X_CMCC_ModeAttribute, 
	setWLanCfg_X_CMCC_ModeAttribute, 
	NULL,
};

static NodeFunc WLanCfg_X_CMCC_VLAN=
{
	NULL,
	NULL,
	getWLanCfg_X_CMCC_VLANValue,
	setWLanCfg_X_CMCC_VLANValue,
	getWLanCfg_X_CMCC_VLANAttribute, 
	setWLanCfg_X_CMCC_VLANAttribute, 
	NULL,
};


static NodeFunc LANEthernetInterfaceConfigfuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitLANEthernetInterfaceConfigfuncsSubTree
};

#ifdef TCSUPPORT_WLAN
static NodeFunc WLANConfigurationFuncsCMCC=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitWLANConfigurationFuncsSubTree
};

static NodeFunc AssociateNum=
{
	NULL,
	NULL,
	getAssociateNumValue, 
	setAssociateNumValue, 
	getAssociateNumAttribute, 
	setAssociateNumAttribute, 
	NULL,
};

static NodeFunc WlanEtherTypeFilter=
{
	NULL,
	NULL,
	getWlanEtherTypeFilterValue, 
	setWlanEtherTypeFilterValue, 
	getWlanEtherTypeFilterAttribute, 
	setWlanEtherTypeFilterAttribute, 
	NULL,
};
#endif

/*
	Parent Node: InternetGatewayDevice.LANInterfaces.
	Parent Node Table Location: cp_topnodetable.c 
*/
CwmpParameterStruct LANInterfaces[] = 
{
	{"LANEthernetInterfaceConfig",ObjectR,NoLimite,&LANEthernetInterfaceConfigfuncs,NULL},
#ifdef TCSUPPORT_WLAN
	{"WLANConfiguration",ObjectR,NoLimite,&WLANConfigurationFuncsCMCC,NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct LANEthernetInterfaceConfigIndex[] = 
{
	{NULL,ObjectR, NoLimite,NULL,LANEthernetInterfaceConfig},
	{NULL, -1, -1, NULL, NULL}	
};

#ifdef TCSUPPORT_WLAN
CwmpParameterStruct WLANConfigurationIndexCMCC[] = 
{
	{NULL,ObjectR, NoLimite,NULL,WLANConfigurationCMCC},
	{NULL, -1, -1, NULL, NULL}	
};

CwmpParameterStruct WLANConfigurationCMCC[] = 
{
	{X_CMCC"AssociateNum",UnsignIntW, NoLimite,&AssociateNum,NULL},
	{X_CMCC"EtherTypeFilter",StrW, NoLimite,&WlanEtherTypeFilter,NULL},
        {X_CMCC"Mode",UnsignIntW, NoLimite,&WLanCfg_X_CMCC_Mode,NULL},
	{X_CMCC"VLAN",StrW, NoLimite,&WLanCfg_X_CMCC_VLAN,NULL},
	{NULL, -1, -1, NULL, NULL}	
};
#endif

CwmpParameterStruct LANEthernetInterfaceConfig[] = 
{
	{X_CMCC"MACAddressNum",UnsignIntW, NoLimite,&MACAddressNum,NULL},
	{X_CMCC"EtherTypeFilter",StrW, NoLimite,&LanEtherTypeFilter,NULL},
	{X_CMCC"Mode",UnsignIntW, NoLimite,&EthInterCfg_X_CMCC_Mode,NULL},
	{X_CMCC"VLAN",StrW, NoLimite,&EthInterCfg_X_CMCC_VLAN,NULL},
	{NULL, -1, -1, NULL, NULL}	
};
