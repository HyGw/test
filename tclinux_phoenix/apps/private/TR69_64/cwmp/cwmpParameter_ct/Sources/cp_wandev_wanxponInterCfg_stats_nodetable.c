#include "Global_res.h"
#include "cp_wandev_wanxponInterCfg_stats.h"


static NodeFunc BytesSent=
{
	NULL,
	NULL,
	getWANPONInterCfg_BytesSentValue,
	NULL, 
	getWANPONInterCfg_BytesSentAttribute, 
	setWANPONInterCfg_BytesSentAttribute, 
	NULL,
};

static NodeFunc BytesReceived=
{
	NULL,
	NULL,
	getWANPONInterCfg_BytesReceivedValue,
	NULL, 
	getWANPONInterCfg_BytesReceivedAttribute, 
	setWANPONInterCfg_BytesReceivedAttribute, 
	NULL,
};

static NodeFunc PacketsSent=
{
	NULL,
	NULL,
	getWANPONInterCfg_PacketsSentValue,
	NULL, 
	getWANPONInterCfg_PacketsSentAttribute, 
	setWANPONInterCfg_PacketsSentAttribute, 
	NULL,
};

static NodeFunc PacketsReceived=
{
	NULL,
	NULL,
	getWANPONInterCfg_PacketsReceivedValue,
	NULL, 
	getWANPONInterCfg_PacketsReceivedAttribute, 
	setWANPONInterCfg_PacketsReceivedAttribute, 
	NULL,
};

static NodeFunc SUnicastPackets=
{
	NULL,
	NULL,
	getWANPONInterCfg_SUnicastPacketsValue,
	NULL, 
	getWANPONInterCfg_SUnicastPacketsAttribute, 
	setWANPONInterCfg_SUnicastPacketsAttribute, 
	NULL,
};

static NodeFunc RUnicastPackets=
{
	NULL,
	NULL,
	getWANPONInterCfg_RUnicastPacketsValue,
	NULL, 
	getWANPONInterCfg_RUnicastPacketsAttribute, 
	setWANPONInterCfg_RUnicastPacketsAttribute, 
	NULL,
};

static NodeFunc SMulticastPackets=
{
	NULL,
	NULL,
	getWANPONInterCfg_SMulticastPacketsValue,
	NULL, 
	getWANPONInterCfg_SMulticastPacketsAttribute, 
	setWANPONInterCfg_SMulticastPacketsAttribute, 
	NULL,
};

static NodeFunc RMulticastPackets=
{
	NULL,
	NULL,
	getWANPONInterCfg_RMulticastPacketsValue,
	NULL, 
	getWANPONInterCfg_RMulticastPacketsAttribute, 
	setWANPONInterCfg_RMulticastPacketsAttribute, 
	NULL,
};

static NodeFunc SBroadcastPackets=
{
	NULL,
	NULL,
	getWANPONInterCfg_SBroadcastPacketsValue,
	NULL, 
	getWANPONInterCfg_SBroadcastPacketsAttribute, 
	setWANPONInterCfg_SBroadcastPacketsAttribute, 
	NULL,
};

static NodeFunc RBroadcastPackets=
{
	NULL,
	NULL,
	getWANPONInterCfg_RBroadcastPacketsValue,
	NULL, 
	getWANPONInterCfg_RBroadcastPacketsAttribute, 
	setWANPONInterCfg_RBroadcastPacketsAttribute, 
	NULL,
};

static NodeFunc FECError=
{
	NULL,
	NULL,
	getWANPONInterCfg_FECErrorValue,
	NULL, 
	getWANPONInterCfg_FECErrorAttribute, 
	setWANPONInterCfg_FECErrorAttribute, 
	NULL,
};

static NodeFunc HECError=
{
	NULL,
	NULL,
	getWANPONInterCfg_HECErrorValue,
	NULL, 
	getWANPONInterCfg_HECErrorAttribute, 
	setWANPONInterCfg_HECErrorAttribute, 
	NULL,
};

static NodeFunc DropPackets=
{
	NULL,
	NULL,
	getWANPONInterCfg_DropPacketsValue,
	NULL, 
	getWANPONInterCfg_DropPacketsAttribute, 
	setWANPONInterCfg_DropPacketsAttribute, 
	NULL,
};

static NodeFunc SPausePackets=
{
	NULL,
	NULL,
	getWANPONInterCfg_SPausePacketsValue,
	NULL, 
	getWANPONInterCfg_SPausePacketsAttribute, 
	setWANPONInterCfg_SPausePacketsAttribute, 
	NULL,
};

static NodeFunc RPausePackets=
{
	NULL,
	NULL,
	getWANPONInterCfg_RPausePacketsValue,
	NULL, 
	getWANPONInterCfg_RPausePacketsAttribute, 
	setWANPONInterCfg_RPausePacketsAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
static NodeFunc ReceiveBlocks=
{
	NULL,
	NULL,
	getWANPONInterCfg_ReceiveBlocksValue,
	NULL, 
	getWANPONInterCfg_ReceiveBlocksAttribute, 
	setWANPONInterCfg_ReceiveBlocksAttribute, 
	NULL,
};

static NodeFunc TransmitBlocks=
{
	NULL,
	NULL,
	getWANPONInterCfg_TransmitBlocksValue,
	NULL, 
	getWANPONInterCfg_TransmitBlocksAttribute, 
	setWANPONInterCfg_TransmitBlocksAttribute, 
	NULL,
};

static NodeFunc FCSError=
{
	NULL,
	NULL,
	getWANPONInterCfg_FCSErrorValue,
	NULL, 
	getWANPONInterCfg_FCSErrorAttribute, 
	setWANPONInterCfg_FCSErrorAttribute, 
	NULL,
};
#endif
/*
    rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.X_CT-COM_EponInterfaceConfig.Stats.
	                                 InternetGatewayDevice.WANDevice.1.X_CT-COM_GponInterfaceConfig.Stats.
*/
CwmpParameterStruct WanEPonInterCfg_Stats[] = 
{
	{"BytesSent", UnsignIntR, NoLimite, &BytesSent, NULL},
	{"BytesReceived", UnsignIntR, NoLimite, &BytesReceived, NULL},
	{"PacketsSent", UnsignIntR, NoLimite, &PacketsSent, NULL},
	{"PacketsReceived", UnsignIntR, NoLimite, &PacketsReceived, NULL},
	{"SUnicastPackets", UnsignIntR, NoLimite, &SUnicastPackets, NULL},
	{"RUnicastPackets", UnsignIntR, NoLimite, &RUnicastPackets, NULL},
	{"SMulticastPackets", UnsignIntR, NoLimite, &SMulticastPackets, NULL},
	{"RMulticastPackets", UnsignIntR, NoLimite, &RMulticastPackets, NULL},
	{"SBroadcastPackets", UnsignIntR, NoLimite, &SBroadcastPackets, NULL},
	{"RBroadcastPackets", UnsignIntR, NoLimite, &RBroadcastPackets, NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"FECErrors", UnsignIntR, NoLimite, &FECError, NULL},
	{"FCSErrors", UnsignIntR, NoLimite, &FCSError, NULL},
#else
	{"FECError", UnsignIntR, NoLimite, &FECError, NULL},
#endif
	{"HECError", UnsignIntR, NoLimite, &HECError, NULL},
	{"DropPackets", UnsignIntR, NoLimite, &DropPackets, NULL},
	{"SPausePackets", UnsignIntR, NoLimite, &SPausePackets, NULL},
	{"RPausePackets", UnsignIntR, NoLimite, &RPausePackets, NULL},

	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct WanGPonInterCfg_Stats[] = 
{
	{"BytesSent", UnsignIntR, NoLimite, &BytesSent, NULL},
	{"BytesReceived", UnsignIntR, NoLimite, &BytesReceived, NULL},
	{"PacketsSent", UnsignIntR, NoLimite, &PacketsSent, NULL},
	{"PacketsReceived", UnsignIntR, NoLimite, &PacketsReceived, NULL},
	{"SUnicastPackets", UnsignIntR, NoLimite, &SUnicastPackets, NULL},
	{"RUnicastPackets", UnsignIntR, NoLimite, &RUnicastPackets, NULL},
	{"SMulticastPackets", UnsignIntR, NoLimite, &SMulticastPackets, NULL},
	{"RMulticastPackets", UnsignIntR, NoLimite, &RMulticastPackets, NULL},
	{"SBroadcastPackets", UnsignIntR, NoLimite, &SBroadcastPackets, NULL},
	{"RBroadcastPackets", UnsignIntR, NoLimite, &RBroadcastPackets, NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"ReceiveBlocks", UnsignIntR, NoLimite, &ReceiveBlocks, NULL},
	{"TransmitBlocks", UnsignIntR, NoLimite, &TransmitBlocks, NULL},
	{"FECErrors", UnsignIntR, NoLimite, &FECError, NULL},
	{"FCSErrors", UnsignIntR, NoLimite, &FCSError, NULL},
#else
	{"FECError", UnsignIntR, NoLimite, &FECError, NULL},
#endif
	{"HECError", UnsignIntR, NoLimite, &HECError, NULL},
	{"DropPackets", UnsignIntR, NoLimite, &DropPackets, NULL},
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"SpausePackets", UnsignIntR, NoLimite, &SPausePackets, NULL},
	{"RpausePackets", UnsignIntR, NoLimite, &RPausePackets, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};


