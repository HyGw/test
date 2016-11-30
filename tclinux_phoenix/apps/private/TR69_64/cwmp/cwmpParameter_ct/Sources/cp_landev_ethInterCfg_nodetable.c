#include "Global_res.h"
#include "cp_landev_ethInterCfg.h"

static NodeFunc EthInterCfg_Enable=
{
	NULL,
	NULL, 
	getEthInterCfg_EnableValue,
	setEthInterCfg_EnableValue,
	getEthInterCfg_EnableAttribute, 
	setEthInterCfg_EnableAttribute, 
	NULL,
};
static NodeFunc EthInterCfg_Status=
{
	NULL,
	NULL,
	getEthInterCfg_StatusValue,
	NULL, 
	getEthInterCfg_StatusAttribute, 
	setEthInterCfg_StatusAttribute, 
	NULL,
};

static NodeFunc EthInterCfg_MACAddress=
{
	NULL,
	NULL, 
	getEthInterCfg_MACAddressValue,
	NULL, 
	getEthInterCfg_MACAddressAttribute, 
	setEthInterCfg_MACAddressAttribute, 
	NULL,
};

static NodeFunc Stats_BytesSt=
{
	NULL,
	NULL, 
	getStats_BytesStValue,
	NULL, 
	getStats_BytesStAttribute, 
	setStats_BytesStAttribute, 
	NULL,
};

static NodeFunc Stats_BytesRvd=
{
	NULL,
	NULL, 
	getStats_BytesRvdValue,
	NULL, 
	getStats_BytesRvdAttribute, 
	setStats_BytesRvdAttribute, 
	NULL,
};

static NodeFunc Stats_PacketsSt=
{
	NULL,
	NULL, 
	getStats_PacketsStValue,
	NULL, 
	getStats_PacketsStAttribute, 
	setStats_PacketsStAttribute, 
	NULL,
};

static NodeFunc Stats_PacketsRvd=
{
	NULL,
	NULL, 
	getStats_PacketsRvdValue,
	NULL, 
	getStats_PacketsRvdAttribute, 
	setStats_PacketsRvdAttribute, 
	NULL,
};

static NodeFunc EthInterCfg_MaxBitRate=
{
	NULL,
	NULL, 
	getEthInterCfg_MaxBitRateValue,
	setEthInterCfg_MaxBitRateValue,
	getEthInterCfg_MaxBitRateAttribute, 
	setEthInterCfg_MaxBitRateAttribute, 
	NULL,
};

static NodeFunc EthInterCfg_DuplexMode=
{
	NULL,
	NULL, 
	getEthInterCfg_DuplexModeValue,
	setEthInterCfg_DuplexModeValue,
	getEthInterCfg_DuplexModeAttribute, 
	setEthInterCfg_DuplexModeAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
static NodeFunc EthInterCfg_X_CTCOM_Mode=
{
	NULL,
	NULL, 
	getEthInterCfg_X_CTCOM_ModeValue,
	setEthInterCfg_X_CTCOM_ModeValue,
	getEthInterCfg_X_CTCOM_ModeAttribute, 
	setEthInterCfg_X_CTCOM_ModeAttribute, 
	NULL,
};

static NodeFunc EthInterCfg_X_CTCOM_VLAN=
{
	NULL,
	NULL, 
	getEthInterCfg_X_CTCOM_VLANValue,
	setEthInterCfg_X_CTCOM_VLANValue,
	getEthInterCfg_X_CTCOM_VLANAttribute, 
	setEthInterCfg_X_CTCOM_VLANAttribute, 
	NULL,
};

static NodeFunc EthInterCfg_DetectionStatus=
{
	NULL,
	NULL, 
	getEthInterCfg_DetectionStatusValue,
	NULL, 
	getEthInterCfg_DetectionStatusAttribute, 
	setEthInterCfg_DetectionStatustAttribute, 
	NULL,
};
#endif

#if defined(TCSUPPORT_CT_PMINFORM)
static NodeFunc EthInterCfg_AccessEquipmentMac=
{
	NULL,
	NULL, 
	getEthInterCfg_AccessEquipmentMacValue,
	NULL, 
	getEthInterCfg_AccessEquipmentMacAttribute, 
	setEthInterCfg_AccessEquipmentMacAttribute, 
	NULL,
};
#endif

#if defined(TCSUPPORT_CMCC)
static NodeFunc BSREnable=
{
	NULL,
	NULL, 
	getBSREnableValue,
	setBSREnableValue, 
	getBSREnableAttribute, 
	setBSREnableAttribute, 
	NULL,
};
#endif


/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANEthernetInterfaceConfig.{i}.Stats.
	Parent Node Table Location: cp_landev_ethInterCfg_nodetable.c
*/

CwmpParameterStruct EthInterCfg_Stats[]= 
{
	{"BytesSent", UnsignIntR, NoLimite, &Stats_BytesSt, NULL},
	{"BytesReceived", UnsignIntR, NoLimite, &Stats_BytesRvd, NULL},
	{"PacketsSent", UnsignIntR, NoLimite, &Stats_PacketsSt, NULL},
	{"PacketsReceived", UnsignIntR, NoLimite, &Stats_PacketsRvd, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANEthernetInterfaceConfig.{i}.
	Parent Node Table Location: cp_landev_ethInterCfg_nodetable.c
*/

CwmpParameterStruct EthInterCfg[]= 
{
	{"Enable", BooleanW, NoLimite, &EthInterCfg_Enable, NULL},
	{"Status", StrR, NoLimite, &EthInterCfg_Status, NULL},
	{"MACAddress", StrR, NoLimite, &EthInterCfg_MACAddress, NULL},
	{"MaxBitRate", StrW, NoLimite, &EthInterCfg_MaxBitRate, NULL},
	{"DuplexMode", StrW, NoLimite, &EthInterCfg_DuplexMode, NULL},
	{"Stats", ObjectR, NoLimite, NULL, EthInterCfg_Stats},
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	{X_CT_COM"Mode", UnsignIntW, NoLimite, &EthInterCfg_X_CTCOM_Mode, NULL},
	{X_CT_COM"VLAN", StrW, NoLimite, &EthInterCfg_X_CTCOM_VLAN, NULL},
	{X_CT_COM"DetectionStatus", UnsignIntR, NoLimite, &EthInterCfg_DetectionStatus, NULL},
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CT_COM"LoopStatus", UnsignIntR, NoLimite, &EthInterCfg_DetectionStatus, NULL},
#endif
#if defined(TCSUPPORT_CT_PMINFORM)
	{"AccessEquipmentMac", StrR, NoLimite, &EthInterCfg_AccessEquipmentMac, NULL},
#endif
#if defined(TCSUPPORT_CMCC)
	{X_CT_COM"BSREnable", BooleanW, NoLimite, &BSREnable, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};

/*
      This Node: InternetGatewayDevice.LANDevice.{i}.LANEthernetInterfaceConfig.{i}.
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANEthernetInterfaceConfig.
	Parent Node Table Location: cp_landev_nodetable.c
*/

CwmpParameterStruct EthInterCfgIndex[]= 
{
	{"1", ObjectR, NoLimite, NULL, EthInterCfg},
	{NULL, -1, -1, NULL, NULL}
};

