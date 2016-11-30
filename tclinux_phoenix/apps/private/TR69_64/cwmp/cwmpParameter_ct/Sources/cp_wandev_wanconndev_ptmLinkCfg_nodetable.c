#include "Global_res.h"
#include "cp_wandev_wanconndev_ptmLinkCfg.h"

static NodeFunc Enable=
{
	NULL,
	NULL,
	getWANPTMLinkCfgEnableValue,
	setWANPTMLinkCfgEnableValue,
	getWANPTMLinkCfgEnableAttribute, 
	setWANDPTMLinkCfgEnableAttribute, 
	NULL
};

static NodeFunc LinkStatus=
{
	NULL,
	NULL,
	getWANPTMLinkCfgLinkStatusValue,
	NULL,
	getWANPTMLinkCfgLinkStatusAttribute, 
	setWANPTMLinkCfgLinkStatusAttribute, 
	NULL
};

static NodeFunc MACAddress=
{
	NULL,
	NULL,
	getWANPTMLinkCfgMACAddressValue,
	NULL,
	getWANPTMLinkCfgMACAddressAttribute, 
	setWANPTMLinkCfgMACAddressAttribute, 
	NULL
};

static NodeFunc BytesSent=
{
	NULL,
	NULL,
	getWANPTMLinkCfgBytesSentValue,
	NULL,
	getWANPTMLinkCfgBytesSentAttribute, 
	setWANPTMLinkCfgBytesSentAttribute, 
	NULL
};

static NodeFunc BytesReceived=
{
	NULL,
	NULL,
	getWANPTMLinkCfgBytesReceivedValue,
	NULL,
	getWANPTMLinkCfgBytesReceivedAttribute, 
	setWANPTMLinkCfgBytesReceivedAttribute, 
	NULL
};

static NodeFunc FramesSent=
{
	NULL,
	NULL,
	getWANPTMLinkCfgFramesSentValue,
	NULL,
	getWANPTMLinkCfgFramesSentAttribute, 
	setWANPTMLinkCfgFramesSentAttribute, 
	NULL
};

static NodeFunc FramesReceived=
{
	NULL,
	NULL,
	getWANPTMLinkCfgFramesReceivedValue,
	NULL,
	getWANPTMLinkCfgFramesReceivedAttribute, 
	setWANPTMLinkCfgFramesReceivedAttribute, 
	NULL
};

static NodeFunc OOSNearEnd=
{
	NULL,
	NULL,
	getWANPTMLinkCfgOOSNearEndValue,
	NULL,
	getWANPTMLinkCfgOOSNearEndAttribute, 
	setWANPTMLinkCfgOOSNearEndAttribute, 
	NULL
};

static NodeFunc OOSFarEnd=
{
	NULL,
	NULL,
	getWANPTMLinkCfgOOSFarEndValue,
	NULL,
	getWANPTMLinkCfgOOSFarEndAttribute, 
	setWANPTMLinkCfgOOSFarEndAttribute, 
	NULL
};

static NodeFunc ErrorsSend=
{
	NULL,
	NULL,
	getWANPTMLinkCfgErrorsSendValue,
	NULL,
	getWANPTMLinkCfgErrorsSendAttribute, 
	setWANPTMLinkCfgErrorsSendAttribute, 
	NULL
};

static NodeFunc ErrorsReceived=
{
	NULL,
	NULL,
	getWANPTMLinkCfgErrorsReceivedValue,
	NULL,
	getWANPTMLinkCfgErrorsReceivedAttribute, 
	setWANPTMLinkCfgErrorsReceivedAttribute, 
	NULL
};

static NodeFunc UnicastPacketsSent=
{
	NULL,
	NULL,
	getWANPTMLinkCfgUnicastPacketsSentValue,
	NULL,
	getWANPTMLinkCfgUnicastPacketsSentAttribute, 
	setWANPTMLinkCfgUnicastPacketsSentAttribute, 
	NULL
};

static NodeFunc UnicastPacketsReceived=
{
	NULL,
	NULL,
	getWANPTMLinkCfgUnicastPacketsReceivedValue,
	NULL,
	getWANPTMLinkCfgUnicastPacketsReceivedAttribute, 
	setWANPTMLinkCfgUnicastPacketsReceivedAttribute, 
	NULL
};

static NodeFunc DiscardPacketsSent=
{
	NULL,
	NULL,
	getWANPTMLinkCfgDiscardPacketsSentValue,
	NULL,
	getWANPTMLinkCfgDiscardPacketsSentAttribute, 
	setWANPTMLinkCfgDiscardPacketsSentAttribute, 
	NULL
};

static NodeFunc DiscardPacketsReceived=
{
	NULL,
	NULL,
	getWANPTMLinkCfgDiscardPacketsReceivedValue,
	NULL,
	getWANPTMLinkCfgDiscardPacketsReceivedAttribute, 
	setWANPTMLinkCfgDiscardPacketsReceivedAttribute, 
	NULL
};

static NodeFunc MulticastPacketsSent=
{
	NULL,
	NULL,
	getWANPTMLinkCfgMulticastPacketsSentValue,
	NULL,
	getWANPTMLinkCfgMulticastPacketsSentAttribute, 
	setWANPTMLinkCfgMulticastPacketsSentAttribute, 
	NULL
};

static NodeFunc MulticastPacketsReceived=
{
	NULL,
	NULL,
	getWANPTMLinkCfgMulticastPacketsReceivedValue,
	NULL,
	getWANPTMLinkCfgMulticastPacketsReceivedAttribute, 
	setWANPTMLinkCfgMulticastPacketsReceivedAttribute, 
	NULL
};

static NodeFunc BroadcastPacketsSent=
{
	NULL,
	NULL,
	getWANPTMLinkCfgBroadcastPacketsSentValue,
	NULL,
	getWANPTMLinkCfgBroadcastPacketsSentAttribute, 
	setWANPTMLinkCfgBroadcastPacketsSentAttribute, 
	NULL
};

static NodeFunc BroadcastPacketsReceived=
{
	NULL,
	NULL,
	getWANPTMLinkCfgBroadcastPacketsReceivedValue,
	NULL,
	getWANPTMLinkCfgBroadcastPacketsReceivedAttribute, 
	setWANPTMLinkCfgBroadcastPacketsReceivedAttribute, 
	NULL
};


static NodeFunc UnknownProtoPacketsReceived=
{
	NULL,
	NULL,
	getWANPTMLinkCfgUnknownProtoPacketsReceivedValue,
	NULL,
	getWANPTMLinkCfgUnknownProtoPacketsReceivedAttribute, 
	setWANPTMLinkCfgUnknownProtoPacketsReceivedAttribute, 
	NULL
};
/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANPTMLinkConfig.Stats
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
CwmpParameterStruct PTMStats[] = 
{
	{"BytesSent", UnsignIntR, NoLimite, &BytesSent, NULL},
	{"BytesReceived", UnsignIntR, NoLimite, &BytesReceived, NULL},
	{"FramesSent", UnsignIntR, NoLimite, &FramesSent, NULL},
	{"FramesReceived", UnsignIntR, NoLimite, &FramesReceived, NULL},
	{"OOSNearEnd", BooleanR, NoLimite, &OOSNearEnd, NULL},
	{"OOSFarEnd", BooleanR, NoLimite, &OOSFarEnd, NULL},
	{"ErrorsSend", UnsignIntR, NoLimite, &ErrorsSend, NULL},
	{"ErrorsReceived", UnsignIntR, NoLimite, &ErrorsReceived, NULL},
	{"UnicastPacketsSent", UnsignIntR, NoLimite, &UnicastPacketsSent, NULL},
	{"UnicastPacketsReceived", UnsignIntR, NoLimite, &UnicastPacketsReceived, NULL},
	{"DiscardPacketsSent", UnsignIntR, NoLimite, &DiscardPacketsSent, NULL},
	{"DiscardPacketsReceived", UnsignIntR, NoLimite, &DiscardPacketsReceived, NULL},
	{"MulticastPacketsSent", UnsignIntR, NoLimite, &MulticastPacketsSent, NULL},
	{"MulticastPacketsReceived", UnsignIntR, NoLimite, &MulticastPacketsReceived, NULL},
	{"BroadcastPacketsSent", UnsignIntR, NoLimite, &BroadcastPacketsSent, NULL},
	{"BroadcastPacketsReceived", UnsignIntR, NoLimite, &BroadcastPacketsReceived, NULL},
	{"UnknownProtoPacketsReceived", UnsignIntR, NoLimite, &UnknownProtoPacketsReceived, NULL},
	{NULL, -1, -1, NULL, NULL}
};
/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANPTMLinkConfig.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
CwmpParameterStruct WANPTMLinkConfig[] =
{
	{"Enable", BooleanW, NoLimite, &Enable, NULL},
	{"LinkStatus", StrR, NoLimite, &LinkStatus, NULL},
	{"MACAddress", StrR, NoLimite, &MACAddress, NULL},
	{"Stats", ObjectR, NoLimite, NULL, PTMStats},
	{NULL, -1, -1, NULL, NULL}
};
