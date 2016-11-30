#include "Global_res.h"
#include "cp_wandev_wanconndev_dslLinkCfg.h"

#if !defined(TCSUPPORT_CT_PON)

static NodeFunc Enable=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1EnableValue,
	setWANDSLLinkCfg1EnableValue,
	getWANDSLLinkCfg1EnableAttribute, 
	setWANDSLLinkCfg1EnableAttribute, 
	NULL
};

static NodeFunc LinkStatus=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1LinkStatusValue,
	NULL,
	getWANDSLLinkCfg1LinkStatusAttribute, 
	setWANDSLLinkCfg1LinkStatusAttribute, 
	NULL
};

static NodeFunc LinkType=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1LinkTypeValue,
	setWANDSLLinkCfg1LinkTypeValue,
	getWANDSLLinkCfg1LinkTypeAttribute, 
	setWANDSLLinkCfg1LinkTypeAttribute, 
	NULL
};

static NodeFunc AutoConfig=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1AutoConfigValue,
	NULL,
	getWANDSLLinkCfg1AutoConfigAttribute, 
	setWANDSLLinkCfg1AutoConfigAttribute, 
	NULL
};

static NodeFunc ModulationType=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1ModulationTypeValue,
	NULL,
	getWANDSLLinkCfg1ModulationTypeAttribute, 
	setWANDSLLinkCfg1ModulationTypeAttribute, 
	NULL
};

static NodeFunc DestinationAddress=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1DestinationAddressValue,
	setWANDSLLinkCfg1DestinationAddressValue,
	getWANDSLLinkCfg1DestinationAddressAttribute, 
	setWANDSLLinkCfg1DestinationAddressAttribute, 
	NULL
};

static NodeFunc ATMEncapsulation=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1ATMEncapsulationValue,
	setWANDSLLinkCfg1ATMEncapsulationValue,
	getWANDSLLinkCfg1ATMEncapsulationAttribute, 
	setWANDSLLinkCfg1ATMEncapsulationAttribute, 
	NULL
};

static NodeFunc FCSPreserved=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1FCSPreservedValue,
	setWANDSLLinkCfg1FCSPreservedValue,
	getWANDSLLinkCfg1FCSPreservedAttribute, 
	setWANDSLLinkCfg1FCSPreservedAttribute, 
	NULL
};

static NodeFunc VCSearchList=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1VCSearchListValue,
	setWANDSLLinkCfg1VCSearchListValue,
	getWANDSLLinkCfg1VCSearchListAttribute, 
	setWANDSLLinkCfg1VCSearchListAttribute, 
	NULL
};

static NodeFunc ATMTransmittedBlocks=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1ATMTransmittedBlocksValue,
	NULL,
	getWANDSLLinkCfg1ATMTransmittedBlocksAttribute, 
	setWANDSLLinkCfg1ATMTransmittedBlocksAttribute, 
	NULL
};

static NodeFunc ATMReceivedBlocks=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1ATMReceivedBlocksValue,
	NULL,
	getWANDSLLinkCfg1ATMReceivedBlocksAttribute, 
	setWANDSLLinkCfg1ATMReceivedBlocksAttribute, 
	NULL
};

static NodeFunc ATMQoS=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1ATMQoSValue,
	setWANDSLLinkCfg1ATMQoSValue,
	getWANDSLLinkCfg1ATMQoSAttribute, 
	setWANDSLLinkCfg1ATMQoSAttribute, 
	NULL
};

static NodeFunc ATMPeakCellRate=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1ATMPeakCellRateValue,
	setWANDSLLinkCfg1ATMPeakCellRateValue,
	getWANDSLLinkCfg1ATMPeakCellRateAttribute, 
	setWANDSLLinkCfg1ATMPeakCellRateAttribute, 
	NULL
};

static NodeFunc ATMMaximumBurstSize=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1ATMMaximumBurstSizeValue,
	setWANDSLLinkCfg1ATMMaximumBurstSizeValue,
	getWANDSLLinkCfg1ATMMaximumBurstSizeAttribute, 
	setWANDSLLinkCfg1ATMMaximumBurstSizeAttribute, 
	NULL
};

static NodeFunc ATMSustainableCellRate=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1ATMSustainableCellRateValue,
	setWANDSLLinkCfg1ATMSustainableCellRateValue,
	getWANDSLLinkCfg1ATMSustainableCellRateAttribute, 
	setWANDSLLinkCfg1ATMSustainableCellRateAttribute, 
	NULL
};

static NodeFunc AAL5CRCErrors=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1AAL5CRCErrorsValue,
	NULL,
	getWANDSLLinkCfg1AAL5CRCErrorsAttribute, 
	setWANDSLLinkCfg1AAL5CRCErrorsAttribute, 
	NULL
};

static NodeFunc ATMCRCErrors=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1ATMCRCErrorsValue,
	NULL,
	getWANDSLLinkCfg1ATMCRCErrorsAttribute, 
	setWANDSLLinkCfg1ATMCRCErrorsAttribute, 
	NULL
};

static NodeFunc ATMHECErrors=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1ATMHECErrorsValue,
	NULL,
	getWANDSLLinkCfg1ATMHECErrorsAttribute, 
	setWANDSLLinkCfg1ATMHECErrorsAttribute, 
	NULL
};
#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_QOS)
static NodeFunc CT_COM_VLAN=
{
	NULL,
	NULL,
	getWANDSLLinkCfg1CT_COM_VLANValue,
	setWANDSLLinkCfg1CT_COM_VLANValue,
	getWANDSLLinkCfg1CT_COM_VLANAttribute, 
	setWANDSLLinkCfg1CT_COM_VLANAttribute, 
	NULL
};
#endif
#endif

#if defined(TCSUPPORT_CT_WAN_PTM)
static NodeFunc Mode=
{
	NULL,
	NULL,
	getWANVdslLinkCfg1ModeValue,
	setWANVdslLinkCfg1ModeValue,
	getWANVdslLinkCfg1ModeAttribute,
	setWANVdslLinkCfg1ModeAttribute,
	NULL
};
#endif


/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANDSLLinkConfig.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
CwmpParameterStruct WANDSLLinkConfig[] =
{
	{"Enable", BooleanW, NoLimite, &Enable, NULL},
	{"LinkStatus", StrR, NoLimite, &LinkStatus, NULL},
	{"LinkType", StrW, NoLimite, &LinkType, NULL},
	{"AutoConfig", BooleanR, NoLimite, &AutoConfig, NULL},
	{"ModulationType", StrR, NoLimite, &ModulationType, NULL},
	{"DestinationAddress", StrW, NoLimite, &DestinationAddress, NULL},
	{"ATMEncapsulation", StrW, NoLimite, &ATMEncapsulation, NULL},
	{"FCSPreserved", BooleanW, NoLimite, &FCSPreserved, NULL},
	{"VCSearchList", StrW, NoLimite, &FCSPreserved, NULL},
	{"ATMTransmittedBlocks", UnsignIntR, NoLimite, &ATMTransmittedBlocks, NULL},
	{"ATMReceivedBlocks", UnsignIntR, NoLimite, &ATMReceivedBlocks, NULL},
	{"ATMQoS", StrW, NoLimite, &ATMQoS, NULL},
	{"ATMPeakCellRate", UnsignIntW, NoLimite, &ATMPeakCellRate, NULL},
	{"ATMMaximumBurstSize", UnsignIntW, NoLimite, &ATMMaximumBurstSize, NULL},
	{"ATMSustainableCellRate", UnsignIntW, NoLimite, &ATMSustainableCellRate, NULL},
	{"AAL5CRCErrors", UnsignIntR, NoLimite, &AAL5CRCErrors, NULL},
	{"ATMCRCErrors", UnsignIntR, NoLimite, &ATMCRCErrors, NULL},
	{"ATMHECErrors", UnsignIntR, NoLimite, &ATMHECErrors, NULL},
#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_QOS)
	{X_CT_COM"VLAN", UnsignIntW, NoLimite, &CT_COM_VLAN, NULL},
#endif
#endif
#if defined(TCSUPPORT_CT_WAN_PTM)
	{X_CT_COM"Mode", UnsignIntW, NoLimite, &Mode, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};

#endif

