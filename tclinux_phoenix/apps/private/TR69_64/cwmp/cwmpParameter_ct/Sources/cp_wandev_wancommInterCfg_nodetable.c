#include "Global_res.h"
#include "cp_wandev_wancommInterCfg.h"

static NodeFunc EnabledForInternet=
{
	NULL,
	NULL,
	getEnabledForInternetValue,
	setEnabledForInternetValue, 
	getEnabledForInternetAttribute, 
	setEnabledForInternetAttribute, 
	NULL,
};

static NodeFunc WANAccessType=
{
	NULL,
	NULL,
	getWANAccessTypeValue,
	NULL, 
	getWANAccessTypeAttribute, 
	setWANAccessTypeAttribute, 
	NULL,
};

static NodeFunc Layer1UpstreamMaxBitRate=
{
	NULL,
	NULL,
	getLayer1UpstreamMaxBitRateValue,
	NULL, 
	getLayer1UpstreamMaxBitRateAttribute, 
	setLayer1UpstreamMaxBitRateAttribute, 
	NULL,
};

static NodeFunc Layer1DownstreamMaxBitRate=
{
	NULL,
	NULL,
	getLayer1DownstreamMaxBitRateValue,
	NULL, 
	getLayer1DownstreamMaxBitRateAttribute, 
	setLayer1DownstreamMaxBitRateAttribute, 
	NULL,
};

static NodeFunc PhysicalLinkStatus=
{
	NULL,
	NULL,
	getPhysicalLinkStatusValue,
	NULL, 
	getPhysicalLinkStatusAttribute, 
	setPhysicalLinkStatusAttribute, 
	NULL,
};

static NodeFunc TotalBytesSent=
{
	NULL,
	NULL,
	getTotalBytesSentValue,
	NULL, 
	getTotalBytesSentAttribute, 
	setTotalBytesSentAttribute, 
	NULL,
};

static NodeFunc TotalBytesReceived=
{
	NULL,
	NULL,
	getTotalBytesReceivedValue,
	NULL, 
	getTotalBytesReceivedAttribute, 
	setTotalBytesReceivedAttribute, 
	NULL,
};

static NodeFunc TotalPacketsSent=
{
	NULL,
	NULL,
	getTotalPacketsSentValue,
	NULL, 
	getTotalPacketsSentAttribute, 
	setTotalPacketsSentAttribute, 
	NULL,
};

static NodeFunc TotalPacketsReceived=
{
	NULL,
	NULL,
	getTotalPacketsReceivedValue,
	NULL, 
	getTotalPacketsReceivedAttribute, 
	setTotalPacketsReceivedAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CT_PON_SC)
static NodeFunc EnabledBRDHCPChk=
{
	NULL,
	NULL,
	getEnabledBRDHCPCheckValue,
	setEnabledBRDHCPCheckValue,
	getEnabledBRDHCPCheckAttribute, 
	setEnabledBRDHCPCheckAttribute, 
	NULL,
};
#endif

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
CwmpParameterStruct WANCommonInterfaceConfig[] = 
{
	{"EnabledForInternet", BooleanR, NoLimite, &EnabledForInternet, NULL},
	{"WANAccessType", StrR, NoLimite, &WANAccessType, NULL},
	{"Layer1UpstreamMaxBitRate", UnsignIntR, NoLimite, &Layer1UpstreamMaxBitRate, NULL},
	{"Layer1DownstreamMaxBitRate", UnsignIntR, NoLimite, &Layer1DownstreamMaxBitRate, NULL},
	{"PhysicalLinkStatus", StrR, NoLimite, &PhysicalLinkStatus, NULL},
	{"TotalBytesSent", UnsignIntR, NoLimite, &TotalBytesSent, NULL},
	{"TotalBytesReceived", UnsignIntR, NoLimite, &TotalBytesReceived, NULL},
	{"TotalPacketsSent", UnsignIntR, NoLimite, &TotalPacketsSent, NULL},
	{"TotalPacketsReceived", UnsignIntR, NoLimite, &TotalPacketsReceived, NULL},
#if defined(TCSUPPORT_CT_PON_SC)
	{X_CT_COM"BRDHCPCheck", BooleanW, NoLimite, &EnabledBRDHCPChk, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};


