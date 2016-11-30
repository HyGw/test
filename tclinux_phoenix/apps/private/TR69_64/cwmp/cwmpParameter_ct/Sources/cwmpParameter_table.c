/*
** $Id$
*/
/************************************************************************
 *
 *	Copyright (C) 2003 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/
#include "Global_res.h"
//#include "AsEngine.h"
//#include "cp_core.h"
#include "cp_topnodetable.h"
#include "cp_dynamicnodetable.h"
#ifdef CWMP
#ifdef TR143
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#endif
#endif

#define TOTAL_REMOTE_DEVICE_NODE 	8
#define MAX_VIRSERV_RULE 10
#define COMMIT_THIRD_ALIAS   100
#define WAN_ENTRY_UNSET_ALIAS  64
#define WAN_PVC_UNSET_ALIAS  8

#if defined(TCSUPPORT_CT_PON_NX)
int voip_itms_flag = 0;
#endif

extern spSysParam_t spSysParam;
extern int itms_reboot_flag;

extern int OperateRoute(int index);
extern void FreeAllRouteMemory();

void    cwmpFreeDeviceTable(DeviceNodePtr deviceNode);
//only support two ssid,how to support four ssid???
static  char gMaxBitRate[2][8] = {"11","54"};
//merge from linos HW Trunk.
static uint8 vcPoolProbeAction = 0;

//static uint16 ACSaddingPVCFlag = 0; //flag for ACS adding PVC

/*attribute struct definition.*/

#if defined(TCSUPPORT_CMCC)
LANInterfacesAttr 		tcLANInterfacesAttr;
#endif

#if defined(TCSUPPORT_CT_PHONEAPP)
X_CTCOM_MgtAttr 		tcX_CTCOM_MgtAttr;
#endif
deviceInfoAttr			tcDeviceInfoAttr;

cwmpAttr 				tcCwmpAttr;
#ifdef CT_COM_DEVICEREG
deviceRegistrationAttr	tcDeviceRegistrationAttr;
#endif
#if !defined(TCSUPPORT_CT_PON)
dslLinkAttr 			tcDslLinkAttr;
#endif
wlanAttr				tcWlanAttr;
ipConAttr 				tcIpConAttr;
//add by brian for ip ping diagnostic
ipPingDiagnosticAttr 	tcIpPingDiagnosticAttrAttr;
#ifdef TCSUPPORT_TRACEROUTE
traceRouteDiagnosticAttr 	tcTraceRouteDiagnosticAttrAttr;
#endif
#if !defined(TCSUPPORT_CT_PON)
//add by brian for dsl diagnostic
dslDiagnosticAttr 		tcdslDiagnosticAttr;
//add by brian for atm f5 loopback diagnostic
atmDiagnosticAttr 		tcatmDiagnosticAttr;
#endif
//add by renyu
dslInterCfgAttr tcdslInterCfgAttr;
dslComInterCfgAttr tcdslComInterCfgAttr;
#if !defined(TCSUPPORT_CT_PON)
dslConnMgmtAttr tcdslConnMgmtAttr;
dslConnMgmtSvrAttr tcdslConnMgmtSvrAttr;
#endif
showtimeAttr tcshowtimeAttr;
totalAttr tctotalAttr;
lastShowtimeAttr tclastShowtimeAttr;
currentDayAttr tccurrentDayAttr;
quarterHourAttr tcquarterHourAttr;

#if defined(TCSUPPORT_CT_IPTV_NODE)
iptvAttr tcIPTVAttr;
#endif

#ifdef TR143
tr143Attr 				tcTr143Attr;
#endif

#ifdef TR111
tr111Attr tcTR111Attr;
#endif
#if VOIP
tr104Attr tcTR104Attr;
#endif
timeAttr tcTimeAttr;
devConfigAttr tcDevConfigAttr;
//add by brian for layer3forwarding
layer3ForwardingAttr tclayer3ForwardingAttr;

lanconfigsecurityAttr tclanconfigsecurityAttr;

lanDeviceAttr tcLanDeviceAttr;
ipInterfaceAttr tcipInterfaceAttr;
etherInterCfgAttr tcetherInterCfgAttr;
hostAttr tchostAttr;

qosAttr	tcQoSAttr;
userInterfaceAttr	tcUserInterfaceAttr;

#ifdef DHCP_PROFILE
dhcpOptionAttr tcdhcpOptionAttr;
dhcpConSerAttr tcdhcpConSerAttr;
#endif
#if defined(TCSUPPORT_CT_ACCESSLIMIT)
accessLimitAttr tcaccessLimitAttr;
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
ctcomIpPingDiagnosticAttr 	ctIpPingDiagnosticAttr;
#endif

#if defined(TCSUPPORT_CT_ALARMMONITOR)
deviceAlarmAttr		tcdeviceAlarmAttr;
deviceMonitorAttr	tcdeviceMonitorAttr;
#endif

#ifdef TCSUPPORT_SYSLOG
sysLogAttr tcsysLogAttr;
#endif

#if defined(TCSUPPORT_CT_ALGABILITY)
algSwitchAttr tcalgSwitchAttr;
#endif

#ifdef TCSUPPORT_IPV6
sysAttr tcSysAttr;
#endif

#if defined(TCSUPPORT_CT_USB_BACKUPRESTORE)
usbRestoreAttr tcusbRestoreAttr;
#endif
#if defined(TCSUPPORT_C1_CUC) || defined(TCSUPPORT_CUC_CWMP_PARAMETER)
X_CU_AccoutAttr cuAccountAttr;
X_CU_LogAttr cuLogAttr;
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
cuAttr tcCUAttr;
#endif
#if defined(TCSUPPORT_CT_MIDWARE)
CT_MidwareAttr ctmidwareAttr;
#endif
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
CT_UPNPAttr ctUPNPAttr;
ctcomdhcpv6Attr tcctcomdhcpv6Attr;
ctcomrouteradvAttr tcctcomrouteradvAttr;
LoopbackDetectAttr loopbackdetectAttr;
DataSpeedLimitAttr dataspeedlimitAttr;
#endif
#if defined(TCSUPPORT_CT_PON)
CT_PonLinkAttr ctPonLinkAttr;
CT_PonInterCfgAttr ctPonInterCfgAttr;
CT_PonInterCfgStatsAttr ctPonInterCfgStatsAttr;
#endif
#if defined(TCSUPPORT_CT_WAN_PTM)
CT_VdslLinkAttr ctVdslLinkAttr;
ptmLinkAttr tcPtmLinkAttr;
#endif

#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
MonitorCollectorAttr	tcMonitorCollectorAttr;
#endif

#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
pppoeEmulatorAttr tcPppoeEmulatorAttr;
pppoeEmulatorBridgeAttr tcPppoeEmulatorBridgeAttr;
#endif
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
ipoeEmulatorAttr tcIpoeEmulatorAttr;
#endif
#if defined(TCSUPPORT_CT_JOYME)
dnsSpeedLimitAttr tcDnsSpeedLimitAttr;
#endif

static uint32 g_uDelaySetTimerID = 0;
QueueNodePtr g_pAsynQueue = NULL;
extern uint8 g_tr069TcapiMode;

attr_t tcAttrs[] = {
	{"cwmpAttr", 				&tcCwmpAttr, 						sizeof(cwmpAttr), 					CWMP_MAGIC_ATTR,   				ATTRFLAG0,    	CWMP_ATTR_FLAG},
#if !defined(TCSUPPORT_CT_PON)
	{"dslLinkAttr", 			&tcDslLinkAttr, 					sizeof(dslLinkAttr),				DSLLink_MAGIC_ATTR,				ATTRFLAG0,	 	DSLLINK_ATTR_FLAG},
#endif
	{"wlanAttr",				&tcWlanAttr,						sizeof(wlanAttr),					WLAN_MAGIC_ATTR,				ATTRFLAG0,	 	WLAN_ATTR_FLAG},
	{"ipConAttr",				&tcIpConAttr,						sizeof(ipConAttr),					IPCON_MAGIC_ATTR,				ATTRFLAG0,	 	IPCON_ATTR_FLAG},
	{"ipPingDiagnosticAttr",	&tcIpPingDiagnosticAttrAttr,		sizeof(ipPingDiagnosticAttr),		IPPINGDIAGNOSTIC_MAGIC_ATTR,	ATTRFLAG0,	 	IPPINGDIAGNOSTIC_ATTR_FLAG},
#if !defined(TCSUPPORT_CT_PON)
	{"dslDiagnosticAttr",		&tcdslDiagnosticAttr,				sizeof(dslDiagnosticAttr),			DSLDIAGNOSTIC_MAGIC_ATTR,		ATTRFLAG0,	 	DSLDIAGNOSTIC_ATTR_FLAG},
	{"atmDiagnosticAttr",		&tcatmDiagnosticAttr,				sizeof(atmDiagnosticAttr),			ATMDIAGNOSTIC_MAGIC_ATTR,		ATTRFLAG0,	 	ATMDIAGNOSTIC_ATTR_FLAG},
#endif
#ifdef TR111	
	{"tr111Atttr",				&tcTR111Attr,						sizeof(tr111Attr),					TR111_MAGIC_ATTR,				ATTRFLAG0,	 	TR111_ATTR_FLAG},
#endif
#if VOIP
	{"tr104Atttr",				&tcTR104Attr,						sizeof(tr104Attr),					TR104_MAGIC_ATTR,				ATTRFLAG0,	 	TR104_ATTR_FLAG},
#endif
	{"deviceInfoAttr",			&tcDeviceInfoAttr,					sizeof(deviceInfoAttr),				DEVICEINF_MAGIC_ATTR,			ATTRFLAG0,		DEVICEINFO_ATTR_FLAG},
	{"timeAtttr",				&tcTimeAttr,					    sizeof(timeAttr),				    TIME_MAGIC_ATTR,				ATTRFLAG0,	 	TIME_ATTR_FLAG},

	{"dslInterCfgAttr",			&tcdslInterCfgAttr,					sizeof(dslInterCfgAttr),			DSLINTERCFG_MAGIC_ATTR,			ATTRFLAG0,	 	DSLINTERCFG_ATTR_FLAG},
	{"dslComInterCfgAttr",		&tcdslComInterCfgAttr,				sizeof(dslComInterCfgAttr),			DSLCOMINTERCFG_MAGIC_ATTR,		ATTRFLAG0,	 	DSLCOMINTERCFG_ATTR_FLAG},
#if !defined(TCSUPPORT_CT_PON)
	{"dslConnMgmtAttr",			&tcdslConnMgmtAttr,					sizeof(dslConnMgmtAttr),			DSLCONNMGMT_MAGIC_ATTR,			ATTRFLAG0,	 	DSLCONNMGMT_ATTR_FLAG},
	{"dslConnMgmtSvrAttr",		&tcdslConnMgmtSvrAttr,				sizeof(dslConnMgmtSvrAttr),			DSLCONNMGMTSVR_MAGIC_ATTR,		ATTRFLAG0,	 	DSLCONNMGMTSVR_ATTR_FLAG},
#endif
    {"devConfigAttr",			&tcDevConfigAttr,					sizeof(devConfigAttr),				DEVCONFIG_MAGIC_ATTR,			ATTRFLAG0,	 	DEVCONFIG_ATTR_FLAG},
    {"layer3ForwardingAttr",	&tclayer3ForwardingAttr,			sizeof(layer3ForwardingAttr),		LAYER3FORWARDING_MAGIC_ATTR,	ATTRFLAG0,		LAYER3FORWARDING_ATTR_FLAG},
	{"showtimeAttr",			&tcshowtimeAttr,					sizeof(showtimeAttr),				SHOWTIME_MAGIC_ATTR,			ATTRFLAG0,	 	SHOWTIME_ATTR_FLAG},
	{"totalAttr",				&tctotalAttr,						sizeof(totalAttr),					TOTAL_MAGIC_ATTR,				ATTRFLAG0,	 	TOTAL_ATTR_FLAG},
	{"lastShowtimeAttr",		&tclastShowtimeAttr,				sizeof(lastShowtimeAttr),			LASTSHOWTIME_MAGIC_ATTR,		ATTRFLAG0,	 	LASTSHOWTIME_ATTR_FLAG},
	{"currentDayAttr",			&tccurrentDayAttr,					sizeof(currentDayAttr),				CURRENTDAY_MAGIC_ATTR,			ATTRFLAG0,	 	CURRENTDAY_ATTR_FLAG},
	{"quarterHourAttr",			&tcquarterHourAttr,					sizeof(quarterHourAttr),			QUARTERHOUR_MAGIC_ATTR,			ATTRFLAG0,	 	QUARTERHOUR_ATTR_FLAG},
	{"lanconfigsecurityAttr",	&tclanconfigsecurityAttr,			sizeof(lanconfigsecurityAttr),		LANCONFIGSECURITY_MAGIC_ATTR,	ATTRFLAG0,	 	LANCONFIGSECURITY_ATTR_FLAG},
	{"lanDeviceAttr",			&tcLanDeviceAttr,					sizeof(lanDeviceAttr),				LANDEVICE_MAGIC_ATTR,			ATTRFLAG0,	 	LANDEVICE_ATTR_FLAG},
	{"ipInterfaceAttr",			&tcipInterfaceAttr,					sizeof(ipInterfaceAttr),			IPINTERFACE_MAGIC_ATTR,			ATTRFLAG0,	 	IPINTERFACE_ATTR_FLAG},
	{"etherInterCfgAttr",		&tcetherInterCfgAttr,				sizeof(etherInterCfgAttr),			ETHERINTERCFG_MAGIC_ATTR,		ATTRFLAG0,	 	ETHERINTERCFG_ATTR_FLAG},
	{"hostAttr",				&tchostAttr,						sizeof(hostAttr),					HOST_MAGIC_ATTR,				ATTRFLAG0,	 	HOST_ATTR_FLAG},
//	{"qosAttr",					&tcQoSAttr,							sizeof(qosAttr),					QOS_MAGIC_ATTR,					ATTRFLAG0,	 	QOS_ATTR_FLAG},
	{"userInterfaceAttr",		&tcUserInterfaceAttr,				sizeof(userInterfaceAttr),			USERITF_MAGIC_ATTR,				ATTRFLAG0,	 	USERITF_ATTR_FLAG},
#ifdef TR143_UDP
	{"tr143Attr",				&tcTr143Attr,						sizeof(tr143Attr),					TR143_MAGIC_ATTR,				ATTRFLAG0,		TR143_ATTR_FLAG},
#endif
#ifdef DHCP_PROFILE	
	{"dhcpOptionAttr",			&tcdhcpOptionAttr,			sizeof(dhcpOptionAttr),			DHCPOPTION_MAGIC_ATTR,			ATTRFLAG0,	 	DHCPOPTION_ATTR_FLAG},
	{"dhcpConSerAttr",		&tcdhcpConSerAttr,			sizeof(dhcpConSerAttr),			DHCPCONSER_MAGIC_ATTR,		ATTRFLAG0,	 	DHCPCONSER_ATTR_FLAG},
#endif
#ifdef CT_COM_DEVICEREG
	{"deviceRegistration",		&tcDeviceRegistrationAttr,			sizeof(deviceRegistrationAttr),			CWMP_DEVICEREGISTRATION_ATTR,		ATTRFLAG1,	 	CWMP_DEVICEREGISTRATION_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_CT_ACCESSLIMIT)
	{"accessLimitAttr",		&tcaccessLimitAttr,			sizeof(accessLimitAttr),			ACCESSLIMIT_MAGIC_ATTR,		ATTRFLAG1,	 	ACCESSLIMIT_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_CT_QOS)
	{"qosAttr",					&tcQoSAttr,							sizeof(qosAttr),					QOS_MAGIC_ATTR,					ATTRFLAG0,	 	QOS_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
	{"IpPingDiagnosticAttr",		&ctIpPingDiagnosticAttr,			sizeof(ctcomIpPingDiagnosticAttr),			CTCOM_IPPINGDIAGNOSTIC_MAGIC_ATTR,		ATTRFLAG1,	 	CTCOM_IPPINGDIAGNOSTIC_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_CT_IPTV_NODE)
	{"iptvAttr",				&tcIPTVAttr,						sizeof(iptvAttr),					IPTV_MAGIC_ATTR,				ATTRFLAG1,	 	IPTV_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_CT_ALARMMONITOR)
	{"deviceAlarmAttr",		&tcdeviceAlarmAttr,			sizeof(deviceAlarmAttr),			DEVICEALARM_MAGIC_ATTR,		ATTRFLAG0,	 	DEVICEALARM_ATTR_FLAG},
	{"deviceMonitorAttr",		&tcdeviceMonitorAttr,			sizeof(deviceMonitorAttr),		DEVICEMONITOR_MAGIC_ATTR,		ATTRFLAG0,	 	DEVICEMONITOR_ATTR_FLAG},
#endif
#ifdef TCSUPPORT_SYSLOG
	{"sysLogAttr",				&tcsysLogAttr,					sizeof(sysLogAttr),				SYSLOG_MAGIC_ATTR,				ATTRFLAG1,	 	SYSLOG_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_CT_ALGABILITY)
	{"algSwitchAttr",				&tcalgSwitchAttr,					sizeof(algSwitchAttr),		ALGSWITCH_MAGIC_ATTR,				ATTRFLAG1,	 	ALGSWITCH_ATTR_FLAG},
#endif
#ifdef TCSUPPORT_IPV6
	{"sysAttr",				&tcSysAttr,					sizeof(sysAttr),		SYS_MAGIC_ATTR,				ATTRFLAG1,	 	SYS_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_CT_USB_BACKUPRESTORE)
	{"usbRestoreAttr",				&tcusbRestoreAttr,					sizeof(usbRestoreAttr),		USBRESTORE_MAGIC_ATTR,				ATTRFLAG1,	 	USBRESTORE_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_C1_CUC) || defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{CU_ACCOUNT_ATTR,				&cuAccountAttr,					sizeof(X_CU_AccoutAttr),		CUACCOUNT_MAGIC_ATTR,				ATTRFLAG1,	 	CUACCOUNT_ATTR_FLAG},
    {CU_LOG_ATTR,				&cuLogAttr,					sizeof(X_CU_LogAttr),		CULOG_MAGIC_ATTR,				ATTRFLAG1,	 	CULOG_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{CU_ATTR,				&tcCUAttr,					sizeof(cuAttr),		CU_MAGIC_ATTR,				ATTRFLAG1,	 	CU_ATTR_FLAG},
#endif
#ifdef TCSUPPORT_TRACEROUTE
	{"traceRouteDiagnosticAttr",	&tcTraceRouteDiagnosticAttrAttr,		sizeof(traceRouteDiagnosticAttr),		TRACEROUTEDIAGNOSTIC_MAGIC_ATTR,	ATTRFLAG1,	 	TRACEROUTEDIAGNOSTIC_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_CT_MIDWARE)
	{CT_MIDWARE_ATTR,				&ctmidwareAttr,					sizeof(CT_MidwareAttr),		CT_MIDWARE_MAGIC_ATTR,				ATTRFLAG1,	 	CT_MIDWARE_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	{CT_UPNP_ATTR, &ctUPNPAttr, sizeof(CT_UPNPAttr), CT_UPNP_MAGIC_ATTR, ATTRFLAG1, CT_UPNP_ATTR_FLAG},
	{"ctcomdhcpv6Attr",				&tcctcomdhcpv6Attr, 				sizeof(ctcomdhcpv6Attr), 	X_CT_COM_DHCPV6_MAGIC_ATTR,				ATTRFLAG1,		X_CT_COM_DHCPV6_ATTR_FLAG},
	{"ctcomrouteradvAttr",				&tcctcomrouteradvAttr, 				sizeof(ctcomrouteradvAttr), 	X_CT_COM_ROUTERADV_MAGIC_ATTR,				ATTRFLAG1,		X_CT_COM_ROUTERADV_ATTR_FLAG},
	{"loopbackdetectAttr", &loopbackdetectAttr, sizeof(LoopbackDetectAttr), LOOPBACKDETECT_MAGIC_ATTR, ATTRFLAG1, LOOPBACKDETECT_ATTR_FLAG},
	{"dataspeedlimitAttr", &dataspeedlimitAttr, sizeof(DataSpeedLimitAttr), DATASPEEDLIMIT_MAGIC_ATTR, ATTRFLAG1, DATASPEEDLIMIT_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_CT_PON)
	{CT_PONLINK_ATTR, &ctPonLinkAttr, sizeof(CT_PonLinkAttr), CT_PONLLINK_MAGIC_ATTR, ATTRFLAG1, CT_PONLINK_ATTR_FLAG},
	{CT_PONINTERCFG_ATTR, &ctPonInterCfgAttr, sizeof(CT_PonInterCfgAttr), CT_PONINTERCFG_MAGIC_ATTR, ATTRFLAG1, CT_PONINTERCFG_ATTR_FLAG},
	{CT_PONINTERCFGSTATS_ATTR, &ctPonInterCfgStatsAttr, sizeof(CT_PonInterCfgStatsAttr), CT_PONINTERCFGSTATS_MAGIC_ATTR, ATTRFLAG1, CT_PONINTERCFGSTATS_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_CT_WAN_PTM)
	{CT_VDSLLINK_ATTR, &ctVdslLinkAttr, sizeof(CT_VdslLinkAttr), CT_VDSLLINK_MAGIC_ATTR, ATTRFLAG1, CT_VDSLLINK_ATTR_FLAG},
	{"ptmLinkAttr", &tcPtmLinkAttr, sizeof(ptmLinkAttr),PTMLink_MAGIC_ATTR, ATTRFLAG1,PTMLINK_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
	{"MonitorCollectorAttr", &tcMonitorCollectorAttr, sizeof(MonitorCollectorAttr),	MONITOR_COLLECTOR_MAGIC_ATTR,	ATTRFLAG1,	MONITOR_COLLECTOR_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
	{"PppoeEmulator", &tcPppoeEmulatorAttr, sizeof(pppoeEmulatorAttr), PPPOEEMULATOR_MAGIC_ATTR, ATTRFLAG1, PPPOE_EMULATOR_ATTR_FLAG},
	{"PppoeEmulatorBridge", &tcPppoeEmulatorBridgeAttr, sizeof(pppoeEmulatorBridgeAttr), PPPOEEMULATOR_BRIDGE_MAGIC_ATTR, ATTRFLAG1, PPPOE_EMULATOR_BRIDGE_ATTR_FLAG},		
#endif
#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
	{"PppoeEmulatorAttr", &tcPppoeEmulatorAttr, sizeof(pppoeEmulatorAttr),	PPPOEEMULATOR_MAGIC_ATTR,	ATTRFLAG1,	PPPOE_EMULATOR_ATTR_FLAG},
	{"PppoeEmulatorBridgeAttr", &tcPppoeEmulatorBridgeAttr, sizeof(pppoeEmulatorBridgeAttr), PPPOEEMULATOR_BRIDGE_MAGIC_ATTR,	ATTRFLAG1,	PPPOE_EMULATOR_BRIDGE_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
	{"IpoeEmulatorAttr", &tcIpoeEmulatorAttr, sizeof(ipoeEmulatorAttr),	IPOEEMULATOR_MAGIC_ATTR,	ATTRFLAG1,	IPOE_EMULATOR_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_CMCC)
	{"LANInterfacesAttr", &tcLANInterfacesAttr, sizeof(LANInterfacesAttr), LANINTERFACES_MAGIC_ATTR,	ATTRFLAG1,	LANINTERFACES_ATTR_FLAG},
#endif
#if defined(TCSUPPORT_CT_JOYME)
	{"DnsSpeedLimitAttr", &tcDnsSpeedLimitAttr, sizeof(dnsSpeedLimitAttr), DNS_SPEED_LIMIT_MAGIC_ATTR,	ATTRFLAG2,	DNS_SPEED_LIMIT_ATTR_FLAG},
#endif

	{NULL, NULL, 0, 0,	0, 0}
};
attr_ptr pTcAttrs = tcAttrs;

static cfgNode_t tcCfgNodes[] = {
	{"Wan", 				"wan_pvc",		NOBITSET,	NOBITSET,	NULL,	0,  	MAX_WAN_NODE, 			CFG_NO,		WanCommitCfgNode},
	{"WLan",    			"wlan_id",		NOBITSET,	NOBITSET,	NULL,   	0,	MAXSSIDNUM, 			CFG_NO,		NULL},
	{"Cwmp",    			NULL,			NOBITSET,	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		NULL},
	{"AutoPVC", 			NULL,			NOBITSET,	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		NULL},
	{"VirServer",		  	"virServ_id",		NOBITSET,	NOBITSET,	NULL,   	0,	MAX_VIRSERV_RULE, 		CFG_NO,		VirserverCommitCfgNode},
#if 0
	{"VirServer_PVC0",  	"virServ_id",		NOBITSET,	NOBITSET,	NULL,   	0,	MAX_VIRSERV_RULE, 		CFG_NO,		NULL},
	{"VirServer_PVC1",  	"virServ_id",		NOBITSET,	NOBITSET,	NULL,   	0,	MAX_VIRSERV_RULE, 		CFG_NO,		NULL},
	{"VirServer_PVC2",  	"virServ_id",		NOBITSET,	NOBITSET,	NULL,   	0,	MAX_VIRSERV_RULE, 		CFG_NO,		NULL},
	{"VirServer_PVC3",  	"virServ_id",		NOBITSET,   	NOBITSET,	NULL,   	0,	MAX_VIRSERV_RULE, 		CFG_NO,		NULL},
	{"VirServer_PVC4",  	"virServ_id",		NOBITSET,   	NOBITSET,	NULL,   	0,	MAX_VIRSERV_RULE, 		CFG_NO,		NULL},
	{"VirServer_PVC5",  	"virServ_id",		NOBITSET,   	NOBITSET,	NULL,   	0,	MAX_VIRSERV_RULE, 		CFG_NO,		NULL},
	{"VirServer_PVC6",  	"virServ_id",		NOBITSET,   	NOBITSET,	NULL,   	0,	MAX_VIRSERV_RULE, 		CFG_NO,		NULL},
	{"VirServer_PVC7",  	"virServ_id",		NOBITSET,   	NOBITSET,	NULL,   	0,	MAX_VIRSERV_RULE, 		CFG_NO,		NULL},
#endif
	{"Lan",				NULL,			NOBITSET,   	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		NULL},
	{"Dproxy",			NULL,			NOBITSET,   	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		NULL},
#ifdef TR111
	{"DhcpClientLimit",   		NULL,			NOBITSET,   	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		NULL},
#endif
#if VOIP
	{"VoIPBasic",   		NULL,			NOBITSET,   	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		tr069CommitCfgNode},
	{"VoIPMedia",  	 	NULL,			NOBITSET,   	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		tr069CommitCfgNode},
	{"VoIPCodecs",   		NULL,			NOBITSET,   	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		tr069CommitCfgNode},
	{"VoIPDigitMap",   			NULL,			NOBITSET,   	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		tr069CommitCfgNode},
	{"InfoVoIP",   			NULL,			NOBITSET,   	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		tr069CommitCfgNode},
	{"VoIPAdvanced",   			NULL,			NOBITSET,   	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		tr069CommitCfgNode},
    {"VoIPH248",            NULL,           NOBITSET,       NOBITSET,   NULL,       0,  NONECHILD,              CFG_NO,     tr069CommitCfgNode},
	{"VoIPSimulateTest",   			NULL,			NOBITSET,   	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		NULL},
	{"VoIPCallCtrl",   			NULL,			NOBITSET,   	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		tr069CommitCfgNode},
	{"VoIPDiagnostic",   			NULL,			NOBITSET,   	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		tr069CommitCfgNode},	
	{"VoIPTest",            NULL,           NOBITSET,       NOBITSET,   NULL,       0,  NONECHILD,              CFG_NO,     NULL},
#endif
	{"Timezone_Entry",   		NULL,			NOBITSET,   	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		tr069CommitCfgNode},
	{"ConfigFile",   		NULL,			NOBITSET,   	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		updateConfigFile},
	{"Route",   			"route_id",		NOBITSET,   	NOBITSET,	NULL,   	0,	MAX_ROUTE_NUM, 			CFG_NO,		RouteCommitCfgNode},

#if defined(TCSUPPORT_CT_NETWORKMANAGESERVICE)
	{"Account",   			"account_id",		NOBITSET,       NOBITSET,	NULL,		0,	MAX_ACCOUNT_ITEM,  			CFG_NO, 	NULL},
	{"Account_FtpEntry",   	NULL,				NOBITSET,       NOBITSET,	NULL,		0,	NONECHILD,  				CFG_NO, 	NULL},
	{"Account_TelnetEntry",   	NULL,			NOBITSET,       NOBITSET,	NULL,		0,	NONECHILD,  				CFG_NO, 	NULL},
#else
	{"Account",   			NULL,			NOBITSET,       NOBITSET,	NULL,		0,	NONECHILD,  			CFG_NO, 	NULL},
#endif
	{"Dhcpd",   			"dhcpd_id",		NOBITSET,   	NOBITSET,	NULL,		0,	MAX_DHCP_STATIC_LEASE, 	CFG_NO,		NULL},	
	{"IPInterface",   		NULL,			NOBITSET,   	NOBITSET,	NULL,		0,	NONECHILD, 				CFG_NO,		NULL},
//	{"QoS",   		        "qos_id",		NOBITSET,	NOBITSET,	NULL,		0,	PKT_QOS_RULE_MAX,		CFG_NO,		queueManagementCommit},
#ifdef TR143
	{"Cwmp_tr143UDPEcho", 		NULL,			NOBITSET,   	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		tr143UDPEchoCommitFunc},

#endif
#if defined(TCSUPPORT_CT_MIDWARE)
	{"Cwmp_Tr069Enable", 		NULL,			NOBITSET,   	NOBITSET,	NULL,   	0,	NONECHILD, 				CFG_NO,		tr069EnableCommitFunc},
#endif

#ifdef DHCP_PROFILE	
	{"Dhcpd_Opt60", 		NULL,			NOBITSET,	NOBITSET,	NULL,		0,	NONECHILD,				CFG_NO,		DHCPProfileCheckIP },
#endif
#if defined(TCSUPPORT_CT_ACCESSLIMIT)
	{"Accesslimit_Common",     		NULL,			NOBITSET,      NOBITSET,	NULL,		0,	 NONECHILD,  			CFG_NO, 	NULL},
#endif
#if defined(TCSUPPORT_CT_QOS)
	{"QoS",   			"qos_id",		NOBITSET,   	NOBITSET,	NULL,		0,	PKT_QOS_RULE_MAX, 		CFG_NO,		ctqosCommit},

	{"QoS_Common",			NULL,			NOBITSET,   	NOBITSET,	NULL,		0,	NONECHILD, 			CFG_NO,		ctqos_common_commit},
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
	{"CtDiagnostic",    		"ippdiag_id",		NOBITSET,   	NOBITSET,	NULL,		0,	MAX_CTCOM_PING_NUM, 			CFG_NO,		CtDiagnosticCommitNode},
	{"CtDiagnostic_Common",    	NULL,			NOBITSET,   	NOBITSET,	NULL,		0,	NONECHILD, 			CFG_NO,		pingCommitFunc},
#endif
#if defined(TCSUPPORT_CT_ADSLSETTING) && !defined(TCSUPPORT_CT_PON) && !defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)	
	{"CT_StandardUsed",   		NULL,			NOBITSET,   	NOBITSET,	NULL,		0,	NONECHILD, 				CFG_NO,		StanddardUsedCommitFunc},
#endif
#if defined(TCSUPPORT_CT_IPTV_NODE) || defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"IGMPproxy",   		NULL,			NOBITSET,   	NOBITSET,	NULL,		0,	NONECHILD, 				CFG_NO,		NULL},	
#endif
#if defined(TCSUPPORT_CT_ALARMMONITOR)
	{"DeviceAlarm",   		NULL,			NOBITSET,   	NOBITSET,	NULL,		0,	NONECHILD, 				CFG_NO,		NULL},
	{"DeviceMonitor",   		NULL,			NOBITSET,   	NOBITSET,	NULL,		0,	NONECHILD, 				CFG_NO,		NULL},
#endif
#ifdef TCSUPPORT_SYSLOG
	{"SysLog",   			NULL,			NOBITSET,   	NOBITSET,	NULL,		0,	NONECHILD, 				CFG_NO,		NULL},		
#if defined(TCSUPPORT_C1_CUC)
	{"AlarmLog",   			NULL,			NOBITSET,   	NOBITSET,	NULL,		0,	NONECHILD, 				CFG_NO,		NULL},		
#endif
#endif
#if defined(TCSUPPORT_CT_ALGABILITY)
	{"ALGSwitch",   		NULL,			NOBITSET,   	NOBITSET,	NULL,		0,	NONECHILD, 				CFG_NO,		NULL},		
#endif
#ifdef CT_COM_DEVICEREG
	{"deviceAccount",   		NULL,			NOBITSET,       NOBITSET,	NULL, 		0,  	NONECHILD, 				CFG_NO, 	NULL},
#endif
#if defined(TCSUPPORT_CT_E8DDNS)
	{"Ddns",   		"ddns_id",			NOBITSET,       NOBITSET,	NULL, 		0,  	MAX_DDNS_RULE, 				CFG_NO, 	NULL},
#endif
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT) || defined(TCSUPPORT_TRUE_LANDING_PAGE)
	{"Portal",   			NULL,			NOBITSET,       NOBITSET,	NULL, 		0, 	NONECHILD, 				CFG_NO, 	NULL},
#endif
#ifdef TCSUPPORT_IPV6
	{"Sys",   		NULL,			NOBITSET,   	NOBITSET,	NULL,		0,	NONECHILD, 				CFG_NO,		NULL},		
#endif
#if defined(TCSUPPORT_CT_USB_BACKUPRESTORE)
	{"usbRestore",   			NULL,			NOBITSET,   	NOBITSET,	NULL,		0,	NONECHILD, 				CFG_NO,		NULL},		
#endif
	{"SwitchPara",   		"switchpara_id",			NOBITSET,   	NOBITSET,	NULL,		0,	SWITCHPARA_MAX_ENTRY, 				CFG_NO,		NULL},
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	{"Upnpd_Entry", NULL, NOBITSET, NOBITSET, NULL, 0, NONECHILD, CFG_NO, NULL},
	{"Accesslimit",    			"accesslimit_id",			NOBITSET,	NOBITSET,	NULL,   	0,	MAX_ACCESSLIMIT_NUM, 				CFG_NO,		NULL},
	{"Dhcp6s",   		NULL,			NOBITSET,   	NOBITSET,	NULL,		0,	NONECHILD, 				CFG_NO,		NULL},
	{"Radvd",   		NULL,			NOBITSET,   	NOBITSET,	NULL,		0,	NONECHILD, 				CFG_NO,		NULL},
#endif
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)	
	{"Adsl_Entry",  NULL, NOBITSET, NOBITSET, NULL, 0, NONECHILD, CFG_NO, ADSLPARASETTING},
	{"DMS", NULL, NOBITSET, NOBITSET, NULL, 0, NONECHILD, CFG_NO, NULL},
	{"LoopDetect", NULL, NOBITSET, NOBITSET, NULL, 0, NONECHILD, CFG_NO, NULL},
	{"DataSpeedLimit", NULL, NOBITSET, NOBITSET, NULL, 0, NONECHILD, CFG_NO, NULL},
	{"VlanBind", "VlanBind_id", NOBITSET, NOBITSET,	NULL, 0, MAX_LAN_PORT_NUM, CFG_NO, NULL},
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"MLDproxy",				NULL,			NOBITSET,		NOBITSET,	NULL,		0,	NONECHILD,				CFG_NO, 	NULL},	
	{"PPPoESimulate",			NULL,			NOBITSET,		NOBITSET,	NULL,		0,	NONECHILD,				CFG_NO, 	NULL},	
	{"Firewall",			NULL,			NOBITSET,		NOBITSET,	NULL,		0,	NONECHILD,				CFG_NO, 	NULL},	
	{"appFTP",			"tr69FTPClient_id",			NOBITSET,		NOBITSET,	NULL,		0,	MAX_APPFTP_ENTRY,				CFG_NO, 	cuappFTPCommit},	
	{"User",   			"user_id",			NOBITSET,   	NOBITSET,	NULL,		0,	MAX_USER_NUM, 				CFG_NO,		NULL},
	{"MACFilter",   	"macfilter_user",			NOBITSET,   	NOBITSET,	NULL,		0,	MAX_USER_NUM, 				CFG_NO,		NULL},
	{"IPFilter",   		"ipfilter_user",			NOBITSET,   	NOBITSET,	NULL,		0,	MAX_USER_NUM, 				CFG_NO,		NULL},
	{"WhiteURL",   		"whiteurl_user",			NOBITSET,   	NOBITSET,	NULL,		0,	MAX_USER_NUM, 				CFG_NO,		NULL},
	{"BlackURL",   		"blackurl_user",			NOBITSET,   	NOBITSET,	NULL,		0,	MAX_USER_NUM, 				CFG_NO,		NULL},
	{"Dmz",			NULL,			NOBITSET,		NOBITSET,	NULL,		0,	NONECHILD,				CFG_NO, 	NULL},	
#endif
#if !defined(TCSUPPORT_CUC)
	{"LogicID", NULL, NOBITSET, NOBITSET, NULL, 0, NONECHILD, CFG_NO, tr069CommitCfgNode},
#endif
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
	{"MonitorCollector",	NULL,	NOBITSET,	NOBITSET,	NULL,	0,	NONECHILD,	CFG_NO, NULL},
#endif
#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
	{"PppoeEmulator", NULL, NOBITSET, NOBITSET, NULL, 0, NONECHILD, CFG_NO, PppoeEmulatorCommit},
	{"PppoeEmulatorBridge", NULL, NOBITSET, NOBITSET, NULL, 0, NONECHILD, CFG_NO, PppoeEmulatorBridgeCommit},
#endif
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
	{"IpoeEmulator_Entry", NULL, NOBITSET, NOBITSET, NULL, 0, NONECHILD, CFG_NO, tr069CommitCfgNode},
#endif
#if defined(TCSUPPORT_CT_PON_SC)
	{"Wan_Common", NULL, NOBITSET, NOBITSET, NULL, 0, NONECHILD, CFG_NO, NULL},
	{"PMInform", NULL, NOBITSET, NOBITSET, NULL, 0, NONECHILD, CFG_NO, NULL},
#endif
#if defined(TCSUPPORT_CMCC)
	{"IpMacFilter", 			NULL,			NOBITSET,		NOBITSET,	NULL,		0,	NONECHILD,				CFG_NO, 	NULL},
	{"UrlFilter", 			NULL,			NOBITSET,		NOBITSET,	NULL,		0,	NONECHILD,				CFG_NO, 	NULL},
#ifdef TCSUPPORT_IPV6
	{ROUTE6_NODE, "route6_id", NOBITSET, NOBITSET, NULL, 0, MAX_STATIC_ROUTE6_NUM, CFG_NO, NULL},
#endif
	{"Lan_BSR",			NULL,			NOBITSET,		NOBITSET,	NULL,		0,	NONECHILD,				CFG_NO, 	NULL},	
#endif
#if defined(TCSUPPORT_CT_JOYME)
	{"DnsSpeedLimit", NULL, NOBITSET, NOBITSET, NULL, 0, NONECHILD, CFG_NO, NULL},
#endif
	{NULL, NULL, NOBITSET, NOBITSET,NULL,0,NONECHILD, CFG_NO}
};

cfgNode_ptr pTcCfgNodes = tcCfgNodes;

int HandleParametersSetting( cfgNode_t *gCfgNode, int type, int delayFlag )
{
	cfgNode_ptr pCfgNode = NULL;
	int commitFlag = 0, rtnCode = 0;
	
	/*check input para valid*/
	if ( gCfgNode == NULL ) {
		return 0;
	}
	
	if ( type != READ && type != COMMIT ) {
		return 0;
	}

	if ( delayFlag != DELAY && delayFlag != NODELAY ) {
		return 0;
	}

#if defined(TCSUPPORT_CT_PON_BIND2_WEB)
	if ( COMMIT == type && delayFlag == NODELAY )
	{
		tcapi_set("deviceAccount_Entry", "registerStatus", "0");
		tcapi_set("deviceAccount_Entry", "newStatus", "1");
	}
#endif

	for ( pCfgNode = gCfgNode; pCfgNode->nodeName != NULL; pCfgNode++ ) {

		if ( pCfgNode->settingFlag == CFG_YES ) {
			
			if ( type == COMMIT ) {
#if defined(TCSUPPORT_CT_PON_NX)
				if(!strcmp(pCfgNode->nodeName,"Wan") && delayFlag == DELAY && voip_itms_flag == 1) {
					commitFlag = 1;
					voip_itms_flag = 0;
					continue;
				}
#endif
#if defined(TCSUPPORT_CT_PON_CY_JX)
				if (!strcmp(pCfgNode->nodeName,"Wan") && delayFlag == DELAY) {
					commitFlag = 1;
					continue;
				}
#endif
                               if (itms_reboot_flag == 1) {
					if (!strcmp(pCfgNode->nodeName, "Wan") || strstr(pCfgNode->nodeName, "VoIP") != NULL) {
						commitFlag = 1;
						rtnCode = 1;
						continue;
					}
				}
				/*if commitFunc is not null, it means this cfg node need delay commit*/
				if ( pCfgNode->commitFunc != NULL ) {
#if defined(TCSUPPORT_CT_PON_GD)
					if(0 != strcmp(pCfgNode->nodeName,"Wan"))
#endif
					{
					rtnCode = 1;
				}
#if defined(TCSUPPORT_C7)
					if ( 0 == strcmp(pCfgNode->nodeName, "Wan")
						|| 0 == strcmp(pCfgNode->nodeName, "PppoeEmulator")
						|| 0 == strcmp(pCfgNode->nodeName, "PppoeEmulatorBridge") )
						rtnCode = 0;
#endif
				}

				/*if commit delay cfgNode, we call customer define function*/
				if ( delayFlag == DELAY && pCfgNode->commitFunc != NULL ) {
					commitFlag = 1;
					pCfgNode->commitFunc( pCfgNode );

					/*clean the flag info of this entry*/
					ClearCfgNodeFlag( pCfgNode );
				}
				
				/*if commit not delay cfg node, just call tr069CommitCfgNode function*/
				else if ( delayFlag == NODELAY && pCfgNode->commitFunc == NULL ) {
					commitFlag = 1;
					tr069CommitCfgNode( pCfgNode );

					/*clean the flag info of this entry*/
					ClearCfgNodeFlag( pCfgNode );
				}
				
			}
			else {
				tc_printf( "tcapi_read(): pCfgNode->nodeName is %s\n", pCfgNode->nodeName );
				tcapi_read( pCfgNode->nodeName );
				ClearCfgNodeFlag( pCfgNode );
			}
		}
		
	}
	
	/* if we call commit function, call spSave() to save setting value to flash */
	if ( commitFlag == 1 ) {
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
		if(isNeedWriteSim()){
			tr069SetNode("SimCard_Entry","needWriteSim","1");
		}
#endif
#if !defined(TCSUPPORT_CT_PON_GD)
		tr069SpSave();
#endif
	}
	
	return rtnCode;
}

/*for speed search for cfg node*/
cfgNode_ptr pSpeedNode = NULL;

cfgNode_ptr SearchCfgNode( char *cfgNodeName )
{
	cfgNode_ptr pCfgNode = NULL;
	
	if ( cfgNodeName == NULL ) {
		return NULL;
	}

	if ( pSpeedNode != NULL && !strcmp( pSpeedNode->nodeName, cfgNodeName ) ) {
		return pSpeedNode;
	}
	
	for ( pCfgNode = pTcCfgNodes; pCfgNode->nodeName != NULL; pCfgNode++ ) {

		if ( !strcmp(pCfgNode->nodeName, cfgNodeName) ) {
			pSpeedNode = pCfgNode;
			return pCfgNode;
		}
	}

	return NULL;
}

#if defined(TCSUPPORT_CT_FJ)
int isInternetWanConn(int pvcIndex, int entryIndex){	
	char nodeName[34] = {'\0'};
	char serviceList[SERVICE_LIST_LEN] = {'\0'};

	sprintf(nodeName, "Wan_PVC%d_Entry%d", pvcIndex,entryIndex);
	tcapi_get(nodeName, "ServiceList", serviceList);
	if( strstr( serviceList, "INTERNET" ) != NULL ){
		return 1;
	}

	return 0;	
}
#endif


void SetCfgNodeFlag( char *cfgNode, int nodeIndex )
{
	cfgNode_ptr pCfgNode = NULL;
	
	int childNodeNum = 0, i;

	pCfgNode = SearchCfgNode( cfgNode );

	tcdbg_printf("Enter SetCfgNodeFlag, nodeIndex:%d!\n", nodeIndex);
	if ( pCfgNode == NULL ) {
		return;
	}
    
#if defined(TCSUPPORT_CT_PON_CQ)
	if (!strcmp(cfgNode, "Wan")) {
		//tcdbg_printf("\r\nDon't set wan parameter");
		return;
	}
#endif

#if defined(TCSUPPORT_CT_PON_NX)
	if (strstr(cfgNode, "VoIP") != NULL) {
		voip_itms_flag = 1;
	}
#endif

#if !defined(TCSUPPORT_CT_PON_JS) && !defined(TCSUPPORT_CT_PON_CZ_GD)
	pCfgNode->bitMask =  NULL;
#endif

	pCfgNode->settingFlag = CFG_YES;

	/*if this node is not Entry node or PVC node, just return, eg:WLan_Common*/
	if ( nodeIndex < 0 ) {
		pCfgNode->comFlag |= 0x01;
		return;
	}
	if ( nodeIndex == 99 ) {
		pCfgNode->comFlag |= 0x02;
		return;
	}
	else if(nodeIndex < 32)
	{
		/*if this cfg node is commit by the one of the layer-2 entry.*/
		if ( pCfgNode->node_id == NULL ) {
			return;
		}
		pCfgNode->bitMap |= (1 << nodeIndex);
	}
	else if (nodeIndex < 64)
	{
		nodeIndex -= 32;
		
		/*if this cfg node is commit by the one of the layer-2 entry.*/
		//if ( pCfgNode->node_id == NULL ) {
		//	return;
		//}
		pCfgNode->bitMap1 |= (1 << nodeIndex);
	}
	else if( (nodeIndex >= COMMIT_THIRD_ALIAS) )
	{
		int uper = 0, lower = 0;

		if(nodeIndex > COMMIT_THIRD_ALIAS+ 64*16 - 1)
			return;
		/*if this cfg node is commit by the one of the layer-3 entry.*/
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_PON_CZ_GD)
		if ( !pCfgNode->bitMask )
		{
			pCfgNode->bitMask =  (uint16 *)malloc(sizeof(uint16) * 64);
			memset(pCfgNode->bitMask, 0, sizeof(uint16) * 64);
		}
#else
		pCfgNode->bitMask =  (uint16 *)malloc(sizeof(uint16) * 64);
#endif
		if(NULL ==pCfgNode->bitMask )
		{
			ClearCfgNodeFlag(pCfgNode);
			return;
		}
#if !defined(TCSUPPORT_CT_PON_JS) && !defined(TCSUPPORT_CT_PON_CZ_GD)
		memset(pCfgNode->bitMask, 0, sizeof(uint16) * 64);
#endif
		
		nodeIndex -= COMMIT_THIRD_ALIAS;
		uper = nodeIndex / 16;
		lower = nodeIndex % 16;
		
		//if(nodeIndex < 32)
		{
			pCfgNode->bitMask[uper] |= (1 << lower);
		}
		//else
		{
		//	nodeIndex -= 32;
		//	pCfgNode->bitMask[1] |= (1 << nodeIndex);
		}

#if defined(TCSUPPORT_CT_FJ)
		if (!strcmp(cfgNode, "Wan")) {		
			if(isInternetWanConn(uper,lower)){				
				setProvisioningCode("INTERNET");
			}
		}
#endif

		
	}
	#if 0
	else if( (nodeIndex > 200) )
	{
		int uper = 0, lower = 0;
		nodeIndex -= 200;
		pCfgNode->bitArray = (uint16 *)malloc(sizeof(uint16) * 64);

		memset(pCfgNode->bitArray, 0, sizeof(uint16) * 64);

		uper = nodeIndex / 16;
		lower = nodeIndex % 16;

		if( (uper > 63) ||(lower >15) )
		{
			return;
		}		
			pCfgNode->bitArray[uper] |= (1 << lower);
	}
	#endif
	return;
}

void ClearCfgNodeFlag( cfgNode_ptr pCfgNode )
{

	if ( pCfgNode == NULL ) {
		return;
	}

	/*clear setting Flag*/
	pCfgNode->settingFlag = CFG_NO;

	pCfgNode->comFlag = 0;
	pCfgNode->bitMap = NOBITSET;
	pCfgNode->bitMap1 = NOBITSET;

	if( NULL != pCfgNode->bitMask )
	{
		free(pCfgNode->bitMask);
		pCfgNode->bitMask = NULL;
	}
	
	return;
}

int delayCommitCfgNode( char *cfgNode )
{
	cfgNode_ptr pCfgNode = NULL;

	if ( cfgNode == NULL ) {
		return -1;
	}
	
	pCfgNode = SearchCfgNode( cfgNode );
	
	if ( pCfgNode == NULL ) {
		tc_printf( " ==> delayCommitCfgNode(): can not find cfg node.\n" );
		return -1;
	}

	return tr069CommitCfgNode( pCfgNode );
	
}

/*Add by robbie.jin to enhance TR069 SetParameterValues RPC performance*/
unsigned long getTickCount()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

int getTr069TcapiMode()
{
    char buf[8] = {0};
    int mode_val= 0;

/*adapt to Jiangsu CT feature*/
#if defined(TCSUPPORT_CT_PON_CN_JS)
    if(0 == tcapi_get("Cwmp_Entry", "delayFlag", buf))
    {
        mode_val = atoi(buf);
        if( 1 == mode_val)
        {
            mode_val = eTcapiDelayMode;
            tc_printf("\r\n tr069 get tcapiMode return delay mode!\n");
            return mode_val;
        }
    }
#endif

    if(tcapi_get("Cwmp_Entry", "tcapiMode", buf) != 0)
    {
        tc_printf("\r\n tr069 get tcapiMode error, set default async mode!\n");
        mode_val = eTcapiAsyncMode;
    }
    else
    {
        mode_val = atoi(buf);
        if( (mode_val < eTcapiSyncMode) || (mode_val > eTcapiDelayMode) )
        {
            mode_val = eTcapiAsyncMode;
        }
    }
	tc_printf("\r\n tr069 get tcapiMode return %d!\n", mode_val);
    return mode_val;
}

int asynInQueue(QueueNodePtr pNode)
{
	QueueNodePtr pNewQueueNode = NULL;
	QueueNodePtr pTemp = g_pAsynQueue;
	if(NULL == pNode)
    {
		tcdbg_printf("The commit node for push into queue is NULL ");
		return -1;
	}
	if(NULL == (pNewQueueNode = (QueueNodePtr)os_malloc(sizeof(QueueNode))))
    {
		tcdbg_printf("malloc commit queue node fails");
		return ResourceExceed;
	}
    
	memset(pNewQueueNode,0, sizeof(QueueNode));
    memmove(pNewQueueNode, pNode, sizeof(QueueNode));
    
    pNewQueueNode->next = NULL;
	if(NULL == g_pAsynQueue)
	{
		g_pAsynQueue = pNewQueueNode;
		return 0;
    }
    
	while(pTemp->next)
	{
		pTemp = pTemp->next;
	}
	pTemp->next = pNewQueueNode;
    
	return 0;
}

int asynOutQueue()
{
	QueueNodePtr pTemp = g_pAsynQueue;
	if(g_pAsynQueue)
	{	
		g_pAsynQueue = g_pAsynQueue->next;
		free(pTemp);
	}
    
	return 0;			
}

int tr069AsynTcapi(QueueNodePtr pNode)
{
    if(ASYN_SAVE != pNode->opType)
    {
        tc_printf("asyn tcapi,opType %u, node %s, current time is %u\n", pNode->opType, pNode->node, getTickCount()/1000);
    }
    else
    {
        tc_printf("asyn save, current time is %u\n", getTickCount()/1000);
    }
    pthread_mutex_lock(&asyn_tcapi_lock);
    asynInQueue(pNode);
    pthread_mutex_unlock(&asyn_tcapi_lock);
    sem_post(&SEM_ASYNTCAPI);
    if(ASYN_SAVE != pNode->opType)
    {
        tc_printf("finish asyn tcapi,opType %u, node %s, current time is %u\n", pNode->opType, pNode->node, getTickCount()/1000);
    }
    else
    {
        tc_printf("finish asyn save, current time is %u\n", getTickCount()/1000);
    }
	return 0;
}

int tr069AsynCommitNode(char *node)
{
    QueueNode queue;

    memset(&queue, 0, sizeof(QueueNode));
    queue.opType = ASYN_COMMIT;
    strncpy(queue.node, node, sizeof(queue.node));
    return tr069AsynTcapi(&queue);
}

int tr069AsynSetNode(char* node, char* attr, char* value)
{
    QueueNode queue;

    memset(&queue, 0, sizeof(QueueNode));
    queue.opType = ASYN_SET;
    strncpy(queue.node, node, sizeof(queue.node));
    strncpy(queue.attr, attr, sizeof(queue.attr));
    strncpy(queue.value, value, sizeof(queue.value));
    return tr069AsynTcapi(&queue);
}

int tr069AsynSave()
{
    QueueNode queue;

    memset(&queue, 0, sizeof(QueueNode));
    queue.opType = ASYN_SAVE;
    strncpy(queue.node, "tcapi_save", sizeof(queue.node));
    return tr069AsynTcapi(&queue);
}

int tr069AsynUnsetNode(char *node)
{
    QueueNode queue;

    memset(&queue, 0, sizeof(QueueNode));
    queue.opType = ASYN_UNSET;
    strncpy(queue.node, node, sizeof(queue.node));
    return tr069AsynTcapi(&queue);
}

void print_time(char *func, unsigned int line)
{
    tc_printf("%s[%u]: current time is %u\n", func, line, getTickCount()/1000);
}

int tr069AsynNodeExecute()
{

    if(NULL == g_pAsynQueue)
    {
        return -1;
    }
    
    switch(g_pAsynQueue->opType)
    {
        case ASYN_COMMIT:
        {
            tc_printf("begin to tcapi commit %s, current time is %u\n", g_pAsynQueue->node, getTickCount()/1000);
            tcapi_commit(g_pAsynQueue->node);
            tc_printf("tcapi commit %s success, current time is %u\n",g_pAsynQueue->node, getTickCount()/1000);
            break;
        }
        case ASYN_SET:
        {
            tc_printf("begin to tcapi set %s, current time is %u\n", g_pAsynQueue->node, getTickCount()/1000);
            tcapi_set(g_pAsynQueue->node, g_pAsynQueue->attr, g_pAsynQueue->value);
            tc_printf("tcapi set %s success, current time is %u\n",g_pAsynQueue->node, getTickCount()/1000);
            break;
        }
        case ASYN_SAVE:
        {
            tc_printf("begin to tcapi save, current time is %u\n", getTickCount()/1000);
            tcapi_save();
            tc_printf("tcapi save success, current time is %u\n", getTickCount()/1000);
            break;
        }
        case ASYN_UNSET:
        {
            tc_printf("begin to tcapi unset %s, current time is %u\n", g_pAsynQueue->node, getTickCount()/1000);
            tcapi_unset(g_pAsynQueue->node);
            tc_printf("tcapi unset %s success, current time is %u\n",g_pAsynQueue->node, getTickCount()/1000);
            break;
        }
        default:
        {
            tcdbg_printf("tcapi opType invalid!\n");
        }
    }
    return 0;
}

void cwmpDelayFunction()
{
    if(eTcapiDelayMode != g_tr069TcapiMode)
    {
        return;
    }
    if(g_uDelaySetTimerID)
    {
        tc_printf("\r\n commit delay time out, tcapi commit delayed node, current time is %u!\n", getTickCount()/1000);
        while(0 == tr069AsynNodeExecute())
        {
            pthread_mutex_lock(&asyn_tcapi_lock);
            asynOutQueue();
            pthread_mutex_unlock(&asyn_tcapi_lock);
        }
        RemoveTimer(g_uDelaySetTimerID);
        g_uDelaySetTimerID = 0;
    }
    return;
}

int getTr069DelayTime()
{
    int time_val = 30;
    char buf[8] = {0};
    int tmp_val = 0;
    
#if defined(TCSUPPORT_CT_PON_CN_JS)
    time_val = 40;
#endif

    if(0 == tcapi_get("Cwmp_Entry", "delayTime", buf))
    {
        tmp_val = atoi(buf);
        if( 0 < tmp_val)
        {
            time_val = tmp_val;
        }
    }
    tc_printf("\r\n get attribute(delayTime) return %d!\n", time_val);
    return time_val;
}

int tr069DelayCommitNode(char *node)
{
    QueueNode queue;

    memset(&queue, 0, sizeof(QueueNode));
    queue.opType = ASYN_COMMIT;
    strncpy(queue.node, node, sizeof(queue.node));

    tcapi_save();
    pthread_mutex_lock(&asyn_tcapi_lock);
    asynInQueue(&queue);
    pthread_mutex_unlock(&asyn_tcapi_lock);
    
    if(g_uDelaySetTimerID)
    {
        RemoveTimer(g_uDelaySetTimerID);
    }
    
    g_uDelaySetTimerID = timerStartS(getTr069DelayTime()*1000,  (TimerCallback)cwmpDelayFunction, NULL);;
    tc_printf("\r\n commit delay timer is up, current time is %u!\n",getTickCount()/1000);
}

int tr069CommitNode(char *node)
{
    if(eTcapiAsyncMode == g_tr069TcapiMode)        
    {
        return tr069AsynCommitNode(node);
    }
    else if(eTcapiDelayMode == g_tr069TcapiMode)
    {
        if((NULL != strstr(node,"Wan"))
            || (NULL != strstr(node, "VoIP")))
        {
            return tr069DelayCommitNode(node);
        }
    }

    return tcapi_commit(node);
}

int tr069SetNode(char* node, char* attr, char* value)
{
    if(eTcapiAsyncMode == g_tr069TcapiMode)        
    {
        return tr069AsynSetNode(node, attr, value);
    }

    return tcapi_set(node, attr, value);
}

int tr069Save()
{
    if(eTcapiAsyncMode == g_tr069TcapiMode)        
    {
        return tr069AsynSave();
    }

    return tcapi_save();
}

int tr069UnsetNode(char* node)
{
    if(eTcapiAsyncMode == g_tr069TcapiMode)        
    {
        return tr069AsynUnsetNode(node);
    }

    return tcapi_unset(node);
}
/*End add by robbie.jin*/

int WanCommitCfgNode(cfgNode_ptr pCfgNode)
{
	char webId[8] = {0}, nodeId[8] = {0};
	int i,j;
	int up = 0, low = 0;
	char WanBuf[32] = {0};
	char service_app[32]  = {0};
	DeviceNodePtr WANDeviceNode = NULL;
	char *objectName = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";
	int wanInitFlag = 0;
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	char cardExist[8] = {0};
	char simAuthRet[8] = {0};
#endif

	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &WANDeviceNode);

	if ( pCfgNode == NULL ) {
		return -1;
	}
	
	if ( pCfgNode->settingFlag == CFG_YES ) {

		/* in common cases, WAN top node is commited*/
		if(pCfgNode->comFlag & 0x01)
		{
			tcdbg_printf("Commit Wan top node\n!");
			tr069CommitNode("Wan");
			ClearCfgNodeFlag( pCfgNode );
			return 0;

		}

		/* when PVC flag is set, the respective PVC is commited and the relative Entrys are cleared*/
		for(i = 0; i < MAX_WAN_NODE; i++)
		{
			if( pCfgNode->bitMap & (1<<i) )
			{
				char buf[32] = {0};
				sprintf(buf, "Wan_PVC%d",i);
				tcdbg_printf("Commit Wan %s\n!",buf);
				tr069SetNode("WebCustom_Entry","tr69CommitWan","1");
                
				tr069CommitNode(buf);
                
				if(pCfgNode->bitMask)
					pCfgNode->bitMask[i/2]  &=  ~(0xFF << ( (i%2)*8) );
			}
		}
	
		/* when the entry flag is set, they are commited*/
		if(pCfgNode->bitMask != NULL)
		{
			for(i = 0; i < MAX_WAN_NODE; i++)
			{
				for(j = 0; j < MAX_WAN_NODEL2; j++)
				{
					up = (i * MAX_WAN_NODEL2 + j) / 16;
					low = (i * MAX_WAN_NODEL2 + j) % 16;

					// operations carried out for the Wan_PVCX_EntryX commit operation
					if( pCfgNode->bitMask[up] & (1<<low) )
					{
						memset(service_app, 0, sizeof(service_app));
						sprintf(WanBuf, "Wan_PVC%d_Entry%d", i, j);
						tcapi_get(WanBuf, "ServiceList", service_app);

						if (NULL == strstr(service_app, TR069))
						{
							tcdbg_printf("Commit Wan %s\n!", WanBuf);
							tr069CommitNode(WanBuf);
						}
						else
						{
							tcdbg_printf("\r\n don't commit TR069 Wan %s !", WanBuf);
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
							tcapi_get("SimCard_Entry","simAuthRet",simAuthRet);
							tcapi_get("SimCard_Entry","cardExist",cardExist);
							if(strcmp(cardExist,"1") == 0 && strcmp(simAuthRet,"1") == 0){
								tcdbg_printf("TR069 Wan set modifyFlag\n!");
								tr069SetNode("SimCard_Entry", "modifyFlag0", "128");
								tr069SetNode("SimCard_Entry", "modifyFlag1", "128");
								tr069SetNode("SimCard_Entry", "modifyFlag9", "64");
							}
#endif
#if defined(TCSUPPORT_CT_DSL_EX)
							tr069SetNode("WebCustom_Entry","WanExecuted","1");
#endif
						}
					}
					

					// operations carried out for the Wan_PVCX_EntryX unset operation
					if (WANDeviceNode == NULL) {
						continue;
					}
					
					if( pCfgNode->bitMask[up + 64/16] & (1<<low) )
					{
						int ret;
						sprintf(WanBuf, "Wan_PVC%d_Entry%d", i, j);
						tr069UnsetNode(WanBuf);
						tcdbg_printf("Unset Wan %s\n!", WanBuf);
						
						//reinit Wan parameter tree flag
						tcdbg_printf("Deleting Wan_PVC%d_Entry%d device!\n", i, j);
#ifdef  TCSUPPORT_CT_WANNODE_MAPPING//tony add
						UpdateEntryMapping(i, j);
#endif
						wanInitFlag = 1;						
					}
				}
			}
				
		}

		// operations carried out for the Wan_PVCX unset operation
		for(i = 0; i < MAX_WAN_NODE; i++)
		{
			if( pCfgNode->bitMap & (1<<(i+ MAX_WAN_NODE)) )
			{
				char buf[32] = {0};
				sprintf(buf, "Wan_PVC%d",i);
				tr069UnsetNode(buf);
				tcdbg_printf("Unset Wan %s\n!",buf);
				
				//reinit Wan parameter tree flag
				tcdbg_printf("Deleting Wan_PVC%d device!\n", i);
#ifdef  TCSUPPORT_CT_WANNODE_MAPPING//tony add
				UpdatePVCMapping(i);
#endif
				wanInitFlag = 1;
			}
		}

		// operations carried out for the Wan unset operation
		if(pCfgNode->comFlag & 0x02)
		{
			tcdbg_printf("Unset Wan top node\n!");
			
			//reinit Wan parameter tree flag
			tcdbg_printf("Deleting Wan device!\n");
			wanInitFlag = 1;
		}

		//reinit Wan parameter tree
		if( (wanInitFlag == 1) && (WANDeviceNode != NULL) )
		{
			int ret;
		
			setCwmpInformTaskStart(1); 
			
			cwmpFreeDeviceTable(WANDeviceNode);
			ret = cwmpInitWANConnectionDeviceTable(WANDeviceNode);

			setCwmpInformTaskStart(0); 
			
			if(ret  < 0 ){
				tcdbg_printf("\r\ncwmpParameter_table.c [WanCommitCfgNode]cwmpInitIPConnectionObjectSubtree failed\n");
			}
		}
		ClearCfgNodeFlag( pCfgNode );
	}
	else {
		return -1;
	}

	return 0;

}

int VirserverCommitCfgNode(cfgNode_ptr pCfgNode)
{
	//char webId[8] = {0}, nodeId[8] = {0};
	int i,j;
	int up = 0, low = 0;
	int num;

	char serverBuf[32] = {0};

	if ( pCfgNode == NULL ) {
		return -1;
	}
	
	if ( pCfgNode->settingFlag == CFG_YES ) {

		/* in common cases, VirServer top node is commited*/
		if(pCfgNode->comFlag)
		{
			tr069CommitNode("VirServer");
			ClearCfgNodeFlag( pCfgNode );
			tcdbg_printf("commit VirServer top node\n!");
			return 0;
			
			//pCfgNode->comFlag = 0;
		//	pCfgNode->bitMap = NOBITSET;
			//pCfgNode->bitMap1 = NOBITSET;

			//if(pCfgNode->bitMask)
			//{
			//	free(pCfgNode->bitMask);
			//	pCfgNode->bitMask = NULL;
			//}
		}

		/* when PVC flag is set, the respective PVC is commited and the relative Entrys are cleared*/
		for(i = 0; i < MAX_VIRSERVER_NODE; i++)
		{
			if(
				( (i < 32) && (pCfgNode->bitMap & (1<<i))) ||
				( (i>=32) && (pCfgNode->bitMap1 & (1<<(i-32))) )
			  )
			{
				char buf[32] = {0};				
				
				sprintf(buf, "VirServer_Entry%d",i);
				tr069CommitNode(buf);
				tcdbg_printf("commit VirServer Entry%d node\n!",i);
				
				if(pCfgNode->bitMask)
				{
					uint32 mask = 0x3ff;
					uint16 mask1 = 0xffff, mask2 = 0xffff; 
								
					up =  (i*10) /16; 
					low = (i*10) % 16;
					
					mask1 = (  ~(mask << low) ) & 0xffff;	
					mask2 = ( ( ~(mask << low) ) >> 16 ) & 0xffff;	
					
					pCfgNode->bitMask[up] &= mask1;					
					if(~mask2)
						pCfgNode->bitMask[up+1] &= mask2 ;				
					
				}
				
			}
		}

		/* when the entry flag is set, they are commited*/
		if(pCfgNode->bitMask != NULL)
		{
			for(i = 0; i < MAX_VIRSERVER_NODE; i++)
			{
				for(j = 0; j < MAX_VIRSERVER_NODEL2; j++)
				{
					up = (i * MAX_VIRSERVER_NODEL2 + j) / 16;
					low = (i * MAX_VIRSERVER_NODEL2 + j) % 16;

					if( pCfgNode->bitMask[up] & (1<<low) )
					{
						sprintf(serverBuf, "VirServer_Entry%d_Entry%d", i, j);
						tr069CommitNode(serverBuf);
						tcdbg_printf("commit VirServer %s\n!",serverBuf);
					}
				}
			}
			
			//free(pCfgNode->bitMask);
			//pCfgNode->bitMask = NULL;
		}

		ClearCfgNodeFlag( pCfgNode );
		
		
		
#if 0		
		
		if ( pCfgNode->node_id == NULL || pCfgNode->bitMap == NOBITSET ) {

			tc_printf( "==>tr069CommitCfgNode():commit none child node. node name is %s\n", pCfgNode->nodeName );
			tr069CommitNode( pCfgNode->nodeName );

			/*clear the flags that set to this cfg node*/
			ClearCfgNodeFlag( pCfgNode );
			return 0;
		}

		/*save current web id*/
		tcapi_get( "WebCurSet_Entry", pCfgNode->node_id, webId );
		
		for ( i = 0; i < pCfgNode->bitNum; i++ ) {
			
			if ( pCfgNode->bitMap & ( 1 << i ) ) {		
				
				sprintf( nodeId, "%d", i );
				tr069SetNode( "WebCurSet_Entry", pCfgNode->node_id, nodeId );
				tr069CommitNode( pCfgNode->nodeName );

				//tc_printf( "==>tr069CommitCfgNode():commit the specific child node, node name is %s\n", pCfgNode->nodeName );
			}
		}

		/*restore web id*/
		tr069SetNode ( "WebCurSet_Entry", pCfgNode->node_id, webId );
		
		/*clear the flags that set to this cfg node*/
		ClearCfgNodeFlag( pCfgNode );
#endif
	}
	else {
		return -1;
	}

	return 0;

}

int tr069CommitCfgNode(cfgNode_ptr pCfgNode)
{
	char webId[8] = {0}, nodeId[8] = {0};
	int i;

	if ( pCfgNode == NULL ) {
		return -1;
	}
	
	if ( pCfgNode->settingFlag == CFG_YES ) {
		
		if ( pCfgNode->node_id == NULL || pCfgNode->bitMap == NOBITSET ) {

			tcdbg_printf( "==>tr069CommitCfgNode():commit none child node. node name is %s\n", pCfgNode->nodeName );
			tr069CommitNode( pCfgNode->nodeName );

			/*clear the flags that set to this cfg node*/
			ClearCfgNodeFlag( pCfgNode );
			return 0;
		}

		/*save current web id*/
		tcapi_get( "WebCurSet_Entry", pCfgNode->node_id, webId );
		
		for ( i = 0; i < pCfgNode->bitNum; i++ ) {
			
			if ( pCfgNode->bitMap & ( 1 << i ) ) {		
				
				sprintf( nodeId, "%d", i );
				tr069SetNode( "WebCurSet_Entry", pCfgNode->node_id, nodeId );
				tr069CommitNode( pCfgNode->nodeName );

				tcdbg_printf( "==>tr069CommitCfgNode():commit the specific child node, node name is %s\n", pCfgNode->nodeName );
			}
		}

		/*restore web id*/
		tr069SetNode ( "WebCurSet_Entry", pCfgNode->node_id, webId );
		
		/*clear the flags that set to this cfg node*/
		ClearCfgNodeFlag( pCfgNode );
	}
	else {
		return -1;
	}

	return 0;

}

#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
int CtDiagnosticCommitNode(cfgNode_ptr pCfgNode)
{
	char webId[8] = {0};//, nodeId[8] = {0};
	char NodeBuf[32] = {0};

	if ( pCfgNode->settingFlag == CFG_NO
		|| pCfgNode->node_id == NULL || pCfgNode->bitMap == NOBITSET )
		return -1;

	if ( 0 != tcapi_get("WebCurSet_Entry", pCfgNode->node_id, webId) )
		return -1;

	memset(NodeBuf, 0, sizeof(NodeBuf));
	sprintf(NodeBuf, "CtDiagnostic_Entry%s", webId);
	tr069SetNode(NodeBuf, "DNS_WTime", "20" );

	return tr069CommitCfgNode(pCfgNode);
}
#endif


#if defined(TCSUPPORT_CT_MIDWARE)
/*adding telecom new node*/
DeviceNodePtr AddTeleNode()
{
	DeviceNodePtr topCTUserIpAddr = NULL;
	DeviceNodePtr top = NULL;
	DeviceNodePtr temp = NULL;
	DeviceNodePtr pre = NULL;
	int i = 0, number=0;
	
	getdbgflag();
	number = countNewTeleNode();
	for(i=0; i<number; i++){
		temp = DeviceNodeAlloc(TeleNode[i].name, TeleNode[i].type, TeleNode[i].attrFlag, TeleNode[i].func);
		if(temp==NULL){
			if(dbg_cwmp >= DBG_CWMP_L2){
			tcdbg_printf("DeviceNodeAlloc:error!\n");
			}
			goto error_handle;
		}
			
		if(top==NULL){
			top = temp;
			top->firstChild = NULL;
			top->parentNode = NULL;
			top->nextSibling = NULL;
			top->prevSibling = NULL;
		}
		else{
			pre->nextSibling = temp;
			temp->prevSibling = pre;
			temp->firstChild = NULL;
			temp->parentNode = NULL;
		}
		pre = temp;	
	}

	return top;
	
	error_handle:
		if(top!=NULL){
			cwmpNodeFree(top);
			top = NULL;
		}
		return NULL;	
}

#endif

int cwmpInitParameterTree()
{
	DeviceNodePtr	TempLevel1 = NULL;
	DeviceNodePtr	TempLevel2 = NULL;
	char deviceName[PARAMETER_NAME_LEN];
	int rtnCode = 0;

	DeviceNodePtr top = NULL;
	top = RegisterNodeFromTable(NULL, TopIGDNode);
	if(top == NULL)
	{
		tc_printf("cwmpInitParameterTree: RegisterNodeFromTable: ERROR\n");
		return -1;
	}
	
	if(InitDynamicNode(top, "InternetGatewayDevice.") != 0)
	{
		tc_printf("cwmpInitParameterTree: RegisterNode Dynamic ERROR\n");
	}
	
	rpc.IGDDeviceRootPtr = top;

	return 0;	
	
}



int8 getTheValue(char** value, ParameterType* type, DeviceNode*  deviceNode, char *valueString)
{
	char *tempValue;
	char errorMsg[256]; 
	
#if defined(TCSUPPORT_CT)
	if((deviceNode->type==IntType || deviceNode->type==BooleanType || deviceNode->type==UnsignedIntType)&&strlen(valueString)==0){
		if((tempValue = (char*)os_malloc(2)) == NULL){
			sprintf(errorMsg, "\r\nos_malloc fails for %s !", deviceNode->name);
			tc_printf(errorMsg);
			return -1;
		}
		strcpy(tempValue, "0");
	}
	else{
#endif
	if((tempValue = (char*)os_malloc(strlen(valueString)+1)) == NULL){
		sprintf(errorMsg, "\r\nos_malloc fails for %s !", deviceNode->name);
		tc_printf(errorMsg);
		return -1;
	}
	strcpy(tempValue, valueString);
#if defined(TCSUPPORT_CT)
	}
#endif

	*value = tempValue;
	*type = deviceNode->type; 	

	return 0;
}
int8 GetTheAttributes(uint8 attribute,uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
	AccessEntry* retAcl;
	uint8      temp;
	
	temp = attribute & (0x01);
	if(temp){
		if((retAcl = (AccessEntry*)os_malloc(sizeof(AccessEntry))) == NULL){
			return -1;
		}
		strcpy(retAcl->entryName, "Subscriber");
		retAcl->next = NULL;
		*acl = retAcl;
	}else{
		*acl = NULL;		
	}
#if defined(TCSUPPORT_CT_MIDWARE)
	if(iTr069Enable != 1){
		temp = attribute & (0x01);
		temp = temp<<2;
		temp |= (attribute>>4)&(0x03);
		temp |= (attribute & (0x40))>>3;	
	}
	else{
#endif
	temp = attribute>>4;
	temp = temp & (0x03);
#if defined(TCSUPPORT_CT_MIDWARE)
	}
#endif
	*notification = temp; 

	return 0;
}
int8 SetTheAttributes(uint8* attribute,uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode){
	uint8      temp;
	
		if(notificationChange){
#if defined(TCSUPPORT_CT_MIDWARE)
		if(iTr069Enable != 1){
			if((notification & 0x03) == 0x03){//3
				return -1;
			}
			temp = notification;
			temp = (temp & (0x03))<<4;
			temp |= (notification & (0x04))>>2;
			*attribute = temp |((notification & (0x08))<<3);		
	}
	else{
#endif
		if(notification > 2) 	//notification = 0~2
			return -1; 	//Error.
		temp = notification;
		temp = temp<<4;
		*attribute = *attribute & (0x0f);
		*attribute = *attribute | temp;
#if defined(TCSUPPORT_CT_MIDWARE)
		}
#endif
	}
	if(accessListChange){
		if(acl == NULL) return 0;  //xzwang_060818
		
		if(strcmp(acl->entryName, "Subscriber") == 0)
			*attribute = *attribute | (0x01);
		else
			*attribute = *attribute & (0xfe);		
	}
	return 0;
}
int RpHex2dec(char c)
{
	if ( (c >= '0') && (c <= '9') )
		return (int) (c - '0');

	if ( (c >= 'a') && (c <= 'f') )
		return (int) ((c -'a') + 10);

	if ( (c >= 'A') && (c <= 'F') )
		return (int) ((c - 'A') + 10);

	return -1;
}
 /*______________________________________________________________________________
**  QuotationMarksExchange
**
**  descriptions:Exchange the quotationmarks to (\")  and add quotation marks at the head and end of the string
**  parameters: SrcPtr(the string you want to do quotation mark exchange)
**  			   SrcLength(the SrcPtr's length)
** 			   DestPtr(the target string)
** 			   DestLength(the DestPtr's length)
**  return: -1(failed)  0(success)
**  revision:
**____________________________________________________________________________*/
int  QuotationMarksExchange(char * SrcPtr, int SrcLength , char* DestPtr , int DestLength)
{
	int  i = 0 , j = 0;
	char Temp1[QUOTATIONMARKSLEN] = "";
	char Temp2[QUOTATIONMARKSLEN] = "";

	if((SrcPtr == NULL) || (DestPtr == NULL))
	{
		return -1;
	}
	
	if((strlen(SrcPtr)*2 + 3 >  DestLength) || (strlen(SrcPtr) != SrcLength) || (strlen(SrcPtr)*2 + 3 > QUOTATIONMARKSLEN))
	{
		return -1;
	}
	
	strcpy(Temp1 , SrcPtr);

	j = 1;
	// quotion marks exchange
	for(i = 0; i < SrcLength; i++ , j++)
	{
		if(Temp1[i] == '"')
		{
			Temp2[j] = 92;        // 92 is the ascll num of '\'
			Temp2[j + 1] = '"';
			j += 1;
		}
		else
		{
			Temp2[j] = Temp1[i];
		}
	}
	// add quotion marks at the head and the end of the string
	Temp2[0] = '"';
	Temp2[j] = '"';
	Temp2[j + 1] = '\0';

	strcpy(DestPtr, Temp2);

	return 0;
}

int8 cwmpGetVpiVciStr(char* pvcStr,  char* vpiStr, char* vciStr){
	int i = 0, vpitag = 0, vcitag = 0;
	char *tempStr = pvcStr;

	while(*tempStr != 0){
		if(*tempStr == ':' ){
			tempStr++;
			i++;
			if(*tempStr >= '0' && *tempStr <= '9'){
				vpitag = i; 
			}
			else if (*tempStr ==' '){
				tempStr++;
				i++;
				if(*tempStr >= '0' && *tempStr <= '9'){
					vpitag = i; 
				}
				else
					return -1;
			}
			else 
				return -1;
		}	
		if(*tempStr == '/' ){
			tempStr++;
			i++;
			vcitag = i;
		}

		tempStr++;
		i++;
	}

	if(vpitag == 0 || vcitag == 0 || vpitag == vcitag)
		return -2;


	if(vcitag < vpitag)
		return -1;

	strncpy(vpiStr, &pvcStr[vpitag], vcitag-vpitag-1);
	strcpy(vciStr, &pvcStr[vcitag]);
	
	return 0;
}
int CheckVPIVCI(int pvcIndex, char*vpistr, char* vcistr)
{
	char nodeName[33] = {'\0'};
	int i;
	char vpi[10] = {'\0'};
	char vci[10] = {'\0'};
	//char vpitemp[10] = {'\0'};
	//char vcitemp[10] = {'\0'};
	
	
	sprintf(nodeName, "Wan_PVC%d", pvcIndex);
	tcapi_get(nodeName, "VPI", vpi);
	tcapi_get(nodeName, "VCI", vci);
	
	for(i = 0; i < MAX_WAN_NODE; i++){
		if(i != pvcIndex ){
			sprintf(nodeName, "Wan_PVC%d", i); 
			tcapi_get(nodeName, "VPI", vpi);
			tcapi_get(nodeName, "VCI", vci);
			if(strcmp(vpi, vpistr) == 0 && strcmp(vci, vcistr) == 0){
				printf("vpi/vci collision.\n");
				return -1;
			}
		}
	}
	return 0;
}

int8 cwmpGetVpiVciStrFromList(char* pvcStr,  char* vpiStr, char* vciStr, int *len)
{
	int i = 0, vpitag = 0, vcitag = 0;
	char *tempStr = pvcStr;

	
	if(*tempStr >= '0' && *tempStr <= '9'){
		vpitag = i; 
		tempStr++;
		i++;
	}
	else{
		return -1;
	}
	
	while(*tempStr != ',' && *tempStr != 0){		
		if(*tempStr == '/' ){
			tempStr++;
			i++;
			vcitag = i;
		}
		else if(!(*tempStr >= '0' && *tempStr <= '9')){
			return -1;
		}
		
		tempStr++;
		i++;
	}

	if( vcitag == 0 || vpitag == vcitag)
		return -2;


	if(vcitag < vpitag){
		return -1;
	}
	
	strncpy(vpiStr, &pvcStr[vpitag], vcitag-vpitag-1);
	strncpy(vciStr, &pvcStr[vcitag], i-vcitag);
	*len = i;
			
	return 0;
}

void	cwmpFreeDeviceTable(DeviceNodePtr deviceNode)
{
	DeviceNodePtr		tempPtr = NULL;

	if(deviceNode == NULL){
		return;
	}

	if(deviceNode->firstChild != NULL){
		cwmpNodeFree(deviceNode->firstChild);
		deviceNode->firstChild = NULL;
	}
	return;
}
void cwmpFreeParameterTree(DeviceNodePtr rootPtr)
{
#if 0
	cwmpFreeDeviceTable(rpc.IGDDeviceRootPtr);
	if(rpc.IGDDeviceRootPtr != NULL){
		os_free(rpc.IGDDeviceRootPtr);
		rpc.IGDDeviceRootPtr = NULL;
	}
#else
	if(rootPtr == NULL)
		return;

	/* hjzhang: free all the children of the rootptr except the rootptr itself */
	cwmpFreeDeviceTable(rootPtr);
	/* free the rootptr itself */
	if(rootPtr != NULL){
		os_free(rootPtr);
		rootPtr = NULL; //actually, it is useless. the rootptr should be set to NULL out of this function
	}
#endif

	return;
}

uint8 GetVcPoolProbeAction(void)
{
	return vcPoolProbeAction;
}

int InternalClientIpCheck(char *st)
{
	int i, dot= 0;
	int i1 =0 ,i2 =0 ,i3=0,i4= 0;
	int len = 0;
	
	/* Modified by hqfeng, 2006.09.14*/
	if(strcmp(st, "0.0.0.0") == 0)
		return 1;

	len = strlen(st);
	if(len < 7 || len > 15)
		return 0;
	
	for (i = 0; i < len;i++)
	{
		if (((*(st+i)) <= '9')&&((*(st+i)) >= '0')) {
		if (dot == 0) {
			i1 = i1 *10 + (*(st+i)-'0');
		}
		else if (dot ==1) {
			i2 = i2 *10 + (*(st+i)-'0');
		}
		else if (dot ==2) {
			i3 = i3 *10 + (*(st+i)-'0');
		}		
		else if (dot ==3) {
			i4 = i4 *10 + (*(st+i)-'0');
		}
		}
	 	else if ((*(st+i)) == '.')
	 		dot++;
	 	else if ((*(st+i)) > '9')//error
	 	 	return 0;
	 	else if ((*(st+i)) < '0')//error
	 	 	return 0;
 		 	 	
	}	
	//printf("i = %d, dot = %d, i1(%d), i2(%d), i3(%d), i4(%d)\n",i, dot, i1,i2,i3,i4);
	if (dot != 3)
		return 0;

	if(i1 == 0 || i4 == 255 || i1 == 127 )	
			return 0;

	if ((i1 > 223) || (i2 > 255) || (i3>255) || (i4>255))
		return 0;

	return 1;
}

int SubnetLegalCheck(char *mask)
{	
	ip4a netmask, broadcast;
	int i;

	if(mask == NULL)
		return 0;
	
	netmask = aton(mask);
	broadcast = aton("255.255.255.255");

	for (i = 0; i < 32; i++) {
		if (netmask & (1 << i)) break;
		broadcast &= ~(1 << i);
	}

	if (broadcast != netmask) {
		return 0;
	}
	
	return 1;
}

int  _checkIp(char *ipstr)
{

	int i=0;
	uint32 digit;
	char *index=NULL;
	char * cur=NULL;
	int dots=0;

	if( ipstr == NULL || strlen(ipstr)>15 || strlen(ipstr) ==0 )
	{
		return -1;
	}
	for( cur=ipstr; *cur!='\0';++cur)
	{
		if( *cur>='0' && *cur <='9' )
		{
			continue;
		}
		else
		{	
			if ( *cur == '.' )
			{
				++dots;
				continue;
			}

			return -1;
		}
	}
	if( dots!=3)
	{
		return -1;
	}
	for (i = 0; i < 3; i++) 
	{
		index = strchr(ipstr, '.');
		if (index == NULL) 
		{
			break;
		}
		*index = '\0';

		digit = atoi(ipstr);
		if( ipstr==NULL || strlen(ipstr)==0 || (digit ==0 && ( i==0 || strlen(ipstr)>3  || strncmp(ipstr,"000",strlen(ipstr))!=0) ))
		{
			*index = '.';			
			return -1;
		}
		*index = '.';
		if (digit > 255) return -1;

		ipstr = index + 1;
	}

	if( i!=3 )
	{
		return -1;
	}
	
	digit = atoi(ipstr);
	if( digit >255 || (digit ==0 && ( strlen(ipstr)>3  || strncmp(ipstr,"000",strlen(ipstr))!=0) ))
	{	
		return -1;
	}

	return 0;
}

int getCU_WANLinkIndex(DeviceNode*  deviceNode)
{
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	uint32 i;
	i = atoi(deviceNode->parentNode->name);
	FindPVCMapping(&i);
	return i;
#else
 	return atoi(deviceNode->parentNode->name)-1;
#endif
}


int getWANLinkIndex(DeviceNode*  deviceNode)
{
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	uint32 i;
	i = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&i);
	return i;
#else
 	return atoi(deviceNode->parentNode->parentNode->name)-1;
#endif
}
int getWANConnectionIndex(DeviceNode*  deviceNode){
	uint32 i,j; 
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
 	 i = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	 j = atoi(deviceNode->parentNode->name);
	 FindWanNodeMapping(&i, &j);
#else
 	 i = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	 j = atoi(deviceNode->parentNode->name)-1;
#endif
	 return (i<<3|j);
}
int getWANNodeIndexForPortMapping(DeviceNode*  deviceNode)
{
	uint32 i, j;
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
 	i = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	j = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&i, &j);
#else
 	i = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	j = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
#endif
	return (i<<3|j);
}
int getPortMappingIndex(DeviceNode*  deviceNode)
{
#ifndef PORTMAPPING_ONLY_VIRTUAL_SERVER	
 	return atoi(deviceNode->parentNode->name)-1;
 #else	
	return atoi(deviceNode->parentNode->name);   //begin from virtual server.
#endif
}

/*_____________________________________________________________________________
**      function name: ResetAttr
**      descriptions:
**           reset the TR069 attribute.
**		
**      parameters:
**            tcAttr_ptr: pointer to the attribute value
**			 
**      global:
**             None
**
**      return:
**             none
**		
**      revision:
**      1. rclv_nj_20100927 
**          remove the tcapi_save operation 
**____________________________________________________________________________
*/
void ResetAttr(attr_ptr tcAttr_ptr)
{
	int attrLen = tcAttr_ptr->attrLen;
	char *attrPtr= (char*)tcAttr_ptr->attrPtr;
	char *attrName = tcAttr_ptr->attrName;
	uint32 *magicPtr = NULL;

	char attrNameBuf[64] = {'\0'};
	char attrBuf[576] = {'\0'};
	int attrRealLen = 0;
	char attrNumBuf[10] = {'\0'};
	int attrNum = 0;
	int i = 0;
	char index[10] = {'\0'};

	int len = attrLen + attrLen;//(attrLen >> 1); //len equals attrLen*1.5
	char *buf = (char*)os_malloc(len);

	if(buf == NULL){
		tc_printf("os_malloc fails.\n");
		return;
	}
	memset(buf, 0, len);
	
	magicPtr = (uint32*)(attrPtr + attrLen - sizeof(uint32));
	*magicPtr = tcAttr_ptr->magic;
	AsBuildBase64String(attrPtr, attrLen, buf);

	attrRealLen = strlen(buf);
	if(attrRealLen <= MAX_ATTR_LEN){ //avoid use ">>" operation.
		attrNum = 1;
	}
	else{
		attrNum = (attrRealLen % MAX_ATTR_LEN) ? (attrRealLen >> MAX_ATTR_OFFSET) + 1 : (attrRealLen >> MAX_ATTR_OFFSET) ;
	}	
	tc_printf("==>ResetAttr():attrNum is %d\n", attrNum);

	if(attrNum == 1){
		tc_printf("\r\n==>ResetAttrs: convert flag to base64 ok,String len is [%d] Now String is \n[%s]\n", strlen(buf), buf);
		tr069SetNode("TR069Attr_Entry",attrName, buf);
	}
	else{
		strcpy(attrNameBuf, attrName);
		strcat(attrNameBuf, "Len");
		sprintf(attrNumBuf, "%d", attrNum);
		
		tr069SetNode("TR069Attr_Entry", attrNameBuf, attrNumBuf); // set attNum to cfg node
		
		for(i = 0; i < attrNum; i++){
			if(i == attrNum - 1){
				strncpy(attrBuf, buf + i*MAX_ATTR_LEN, attrRealLen % MAX_ATTR_LEN);
			}
			else{
				strncpy(attrBuf, buf + i*MAX_ATTR_LEN, MAX_ATTR_LEN);
			}
			strcpy(attrNameBuf, attrName);
			sprintf(index, "%d", i);
			strcat(attrNameBuf, index);
			tr069SetNode("TR069Attr_Entry", attrNameBuf, attrBuf);//make sure attr'len less than 513.
			memset(attrBuf, 0, MAX_ATTR_LEN + 1);
		}
	}

	os_free(buf);
	//tcapi_save();
}/* end ResetAttr */

/*_____________________________________________________________________________
**      function name: InitAttrs
**      descriptions:
**           Init the TR069 attributes.
**		
**      parameters:
**            none  
**			 
**      global:
**             None
**
**      return:
**             none
**		
**      revision:
**      1. rclv_nj_20100927 
**          adding the tcapi_save operation according to saveFlag 
**____________________________________________________________________________
*/
void InitAttrs(void)
{
	attr_ptr tcAttr_ptr = NULL;
	int saveFlag = 0, saveOper = 0;
	for(tcAttr_ptr = tcAttrs; tcAttr_ptr->attrName != NULL; tcAttr_ptr++ ){
		ReadAttr(tcAttr_ptr, &saveFlag);
		if(saveFlag == 1)
		{
			saveOper = 1;
		}
	}
	if(1 == saveOper)
	{
		//tcapi_save();
		spSave();
	}
	tc_printf("InitAttrs ok.\n");
	return;
}/* end InitAttrs */

/*_____________________________________________________________________________
**      function name: ReadAttr
**      descriptions:
**           read the attribute value from flash to romfile.
**		
**      parameters:
**            tcAttr_ptr: pointer to the attribute value
**            saveFlag: flag indicating whether tcapi_save opt  
**			 
**      global:
**             None
**
**      return:
**             none
**		
**      revision:
**      1. rclv_nj_20100927 
**          remove the tcapi_save operation and pass it in the saveFlag 
**____________________________________________________________________________
*/
void ReadAttr(attr_ptr tcAttr_ptr, int *saveFlag)
{
	char *attrPtr = (char*)tcAttr_ptr->attrPtr;
	char *attrName = tcAttr_ptr->attrName;
	int attrLen = tcAttr_ptr->attrLen;
	int len = attrLen + attrLen;//(attrLen >> 1); //len equals attrLen*1.5
	char *buf = NULL;
	
	char attrNumBuf[10] = {'\0'};
	int attrNum = 0;
	char attrNameBuf[64] = {'\0'}; //attribute should not be more than 63.
	char attrBuf[576] = {'\0'};
	int i = 0;
	char index[10] = {'\0'};
	uint32 magic = 0;

	buf = (char*)os_malloc(len);
	if(buf == NULL){
		tc_printf("os_malloc fails.\n");
		return;
	}
	memset(buf, 0, len);
	
	strcpy(attrNameBuf, attrName);
	strcat(attrNameBuf, "Len"); //if there are more than 1 attr of this struct,there is a attrbute named attrNameLen
	
	tc_printf("==>ReadAttr():attrNameBuf is %s\n", attrNameBuf);
	
	tcapi_get("TR069Attr_Entry", attrNameBuf, attrNumBuf);
	attrNum = atoi(attrNumBuf); //get attrNum of this struct.

	if(attrNum == 0){//there is only one attr of this struct.
		if(tcapi_get("TR069Attr_Entry", attrName, buf) != 0){
			tc_printf("\r\n==>ReadAttr():InitAttr: read cwmpattrs error, we will reset Attrs");
			ResetAttr(tcAttr_ptr);
			*saveFlag = 1;
			os_free(buf);
			return;
		}
	}
	else{ //there are attrNum attrs of this struct.
		for(i = 0; i < attrNum; i++){
			strcpy(attrNameBuf, attrName);
			sprintf(index, "%d", i);
			strcat(attrNameBuf, index); //if attrNum is larger than 0,attr name is attrName0,attrName1...
			tc_printf("==>ReadAttr():attrNameBuf is %s\n",attrNameBuf);
			if(tcapi_get("TR069Attr_Entry", attrNameBuf, attrBuf) != 0){
				tc_printf("==>ReadAttr():InitAttr: read cwmpattrs error, we will reset Attrs");
				ResetAttr(tcAttr_ptr);
				*saveFlag = 1;
				os_free(buf);
				return;
			}
			strcat(buf, attrBuf);
		}
	}
	
	tc_printf("\r\n==>ReadAttr(): Convert from base64 code to struct!\n");

	RpDecodeBase64DataNoNull(buf, strlen(buf), attrPtr);
	magic = *(uint32*)(attrPtr + attrLen - sizeof(uint32));
	if(magic != tcAttr_ptr->magic){
		tc_printf("\r\n==>ReadAttr(): attr.magic is wrong now is [%04X] should be [%04X]!\n", magic, tcAttr_ptr->magic);	
		ResetAttr(tcAttr_ptr);
		*saveFlag = 1;
		os_free(buf);
		return;
	}
	os_free(buf);
}/* end ReadAttr */

/*_____________________________________________________________________________
**      function name: SaveAttr
**      descriptions:
**           save the TR069 attribute.
**		
**      parameters:
**            tcAttr_ptr: pointer to the attribute value
**			 
**      global:
**             None
**
**      return:
**             none
**		
**      revision:
**      1. rclv_nj_20100927 
**         adding tcapi_save operation which was oringinally done 
**         within the ReadAttr() function
**____________________________________________________________________________
*/
void SaveAttr(attr_ptr tcAttr_ptr)
{
	char *attrPtr = (char*)tcAttr_ptr->attrPtr;
	char *attrName = tcAttr_ptr->attrName;
	int attrLen = tcAttr_ptr->attrLen;
	int len = attrLen + attrLen;//(attrLen >> 1);
	char *buf;

	char attrNameBuf[64] = {'\0'};
	char attrBuf[576] = {'\0'};
	int attrRealLen = 0;
	char attrNumBuf[10] = {'\0'};
	int attrNum = 0;
	int i = 0;
	char index[10] = {'\0'};
	buf = (char*)os_malloc(len);
	if(buf == NULL){
		tc_printf("os_malloc fails.\n");
		return;
	}
	
	memset(buf, 0, len);
	AsBuildBase64String(attrPtr, attrLen, buf);

	attrRealLen = strlen(buf);
	if(attrRealLen <= MAX_ATTR_LEN){//in general route.
		attrNum = 1;
	}
	else{
		attrNum = (attrRealLen % MAX_ATTR_LEN) ? (attrRealLen >> MAX_ATTR_OFFSET) + 1 : (attrRealLen >> MAX_ATTR_OFFSET) ;
	}
	if(attrNum == 1){ //if there is only one attr of this struct, set buf to cfg node.
		tc_printf("\r\n==>SaveAttr(): convert attr to base64 ok,String len is [%d] Now String is \n[%s]\n", strlen(buf), buf);	
		tr069SetNode("TR069Attr_Entry",attrName, buf);
	}
	else{
		strcpy(attrNameBuf, attrName);
		strcat(attrNameBuf, "Len");
		sprintf(attrNumBuf, "%d", attrNum);
		tr069SetNode("TR069Attr_Entry", attrNameBuf, attrNumBuf); // set attNum to cfg node		
		for(i = 0; i < attrNum; i++){	//set attr like attrName0,attrName1... to cfg node.
			if(i == attrNum - 1){
				strncpy(attrBuf, buf + i*MAX_ATTR_LEN, attrRealLen % MAX_ATTR_LEN);
			}
			else{
				strncpy(attrBuf, buf + i*MAX_ATTR_LEN, MAX_ATTR_LEN);
			}
			strcpy(attrNameBuf, attrName);
			sprintf(index, "%d", i);
			strcat(attrNameBuf, index);
			tr069SetNode("TR069Attr_Entry", attrNameBuf, attrBuf);
			memset(attrBuf, 0, MAX_ATTR_LEN + 1);
		}
	}
	os_free(buf);
}/* end SaveAttr */

int RouteCommitCfgNode(cfgNode_ptr pCfgNode)
{
	char webId[8] = {0}, nodeId[8] = {0};
	int i;


	if ( pCfgNode == NULL ) {
		return -1;
	}
	
	if ( pCfgNode->settingFlag == CFG_YES )
	{
		/*save current web id*/
		tcapi_get( "WebCurSet_Entry", pCfgNode->node_id, webId );
		
		for ( i = 0; i < pCfgNode->bitNum; i++ )
		{		
			if ( pCfgNode->bitMap & ( 1 << i ) ) 
			{		
				OperateRoute(i+1);
			}
		}

		FreeAllRouteMemory();
		
		/*restore web id*/
		tr069SetNode ( "WebCurSet_Entry", pCfgNode->node_id, webId );
		
		/*clear the flags that set to this cfg node*/
		ClearCfgNodeFlag( pCfgNode );
	}
	else {
		return -1;
	}

	return 0;

}
BOOLEAN ValidateChecksum(
	IN UINT PIN)
{
	UINT accum = 0;

	accum += 3 * ((PIN / 10000000) % 10); 
	accum += 1 * ((PIN / 1000000) % 10); 
	accum += 3 * ((PIN / 100000) % 10); 
	accum += 1 * ((PIN / 10000) % 10); 
	accum += 3 * ((PIN / 1000) % 10); 
	accum += 1 * ((PIN / 100) % 10); 
	accum += 3 * ((PIN / 10) % 10); 
	accum += 1 * ((PIN / 1) % 10); 
	
    return (0 == (accum % 10));
} 
#ifdef TR143
static char  DldUldDiagStateType[32] = "None";
int8 setDldUldDiagStateType(char * value)
{
	strcpy(DldUldDiagStateType,value);
	return 0;
}

char * getDldUldDiagStateType(void)
{
	return DldUldDiagStateType;
}
#endif

/*_____________________________________________________________________________
**      function name: SearchRouteByDesIP
**      descriptions:
**           finde Route entry from cfg node Route.
**		
**      parameters:
**            desIpStr: desIP
**			  re:       Route Entry
**      global:
**             None
**
**      return:
**             0: success
**			  -1: failure
**			  -2: invalid addr
**
**      call:
**   		   aton() 
**		
**      revision:
**      1. xyzhu_nj_20100115
**____________________________________________________________________________
*/

int SearchRouteByDesIP(char* desIp, RouteEntry_t *re)
{
	char Route_num_str[10];
	char nodeName[32];
	int Route_num = 0, i, ret = 0;

	char DST_IP[16];
	char Sub_Mask[16];

	uint32 R_desIp, R_desIP, R_subMask;

	if ( re == NULL ) {
		tcdbg_printf("re is NULL.\n");
		return -1;
	}

	R_desIp = aton(desIp);
	
	if ( R_desIp == 0 ) {
		return -2;
	}
	
	memset( Route_num_str, 0, sizeof(Route_num_str) );
	memset( nodeName, 0, sizeof(nodeName) );
	memset( DST_IP, 0, sizeof(DST_IP) );
	memset( Sub_Mask, 0, sizeof(Sub_Mask) );

	/*get Route Entry num*/
	tcapi_get( "Route", "Route_num", Route_num_str );
	
	Route_num = atoi(Route_num_str);
	tcdbg_printf( "num is %d\n", Route_num );

	/* static route is 0 ~ 15, dynamic route is from 16*/
	for ( i = 0; i < Route_num + 16; i++ ) {
		
		sprintf( nodeName, "Route_Entry%d", i );
		ret = tcapi_get( nodeName, "DST_IP", DST_IP );
		if ( ret != 0 ) {
			continue;
		}
		tcapi_get( nodeName, "Sub_mask", Sub_Mask );

		tcdbg_printf( "desIp is %s\n", desIp );
		tcdbg_printf( "DST_IP is %s\n", DST_IP );
		tcdbg_printf( "Sub_Mask is %s\n", Sub_Mask );
		
		R_desIP = aton( DST_IP );
		R_subMask = aton( Sub_Mask );

		if ( R_subMask == 0 ) {
			continue;
		}

		if ( ( R_desIp & R_subMask ) == R_desIP ) {
			
			/*we finde Route Entry, record it to re.*/
			tcapi_get( nodeName, "DST_IP", re->DST_IP );
			tcapi_get( nodeName, "Gateway", re->GateWay );
			tcapi_get( nodeName, "Sub_mask", re->Sub_Mask );
			tcapi_get( nodeName, "Device", re->Device );

			return 0;
		}
	}

	tcdbg_printf("come to here.\n");
	return -1;
}

int checkCPEIP(char *ipaddr)
{
	int rtnCode = -1, pvcnum = 8, i;
	char nodeName[33] = {0};
	char lanIP[33] = {0};
	char wanIP[33] = {0};
	char active[10];
	char isp[10];	
	
	tcapi_get( "Lan_Entry0","IP",lanIP );

	/*check if ip is lanIP*/
	if ( !strcmp( lanIP, ipaddr ) ) {
		rtnCode = 0;
		return rtnCode;
	}

	for ( i = 0; i < pvcnum; i++ ) {
		
		sprintf( nodeName, "Wan_PVC%d", i );
		tcapi_get( nodeName, "Active", active );
		tcapi_get( nodeName, "ISP", isp );

		if ( !strcmp( active, "Yes" ) ) {
			
			if ( strcmp( isp, "3" ) !=0 ) {
				
				sprintf(nodeName, "DeviceInfo_PVC%d", i);
				memset( wanIP, 0, sizeof(wanIP) );
				tcapi_get(nodeName, "WanIP", wanIP);

				/*check if ip is wanIP*/
				if ( !strcmp( wanIP, ipaddr ) ) {
					rtnCode = 0;
					return rtnCode;
				}
			}
		}
	}

	return rtnCode;
}

int checkPortValid( uint32 portIn )
{

#if 0
	FILE *fp = NULL;
	char portInStr[10];
	char port[10], port_path[32], netstatPort[64], buf[128], *p = NULL;

	sprintf( portInStr, "%lu", portIn );
	
	/*init the data*/
	memset( port, 0, sizeof(port) );
	memset( buf, 0, sizeof(buf) );
	strcpy( port_path, "/tmp/portPath.tmp" );
	strcpy( netstatPort, "netstat -nl > " );

	/*excute the command and redirect the data to a file*/
	strcat( netstatPort, port_path );

	tcdbg_printf( "netstatPort is %s\n", netstatPort );
	system( netstatPort );

	/*parser from file and find the port info*/
	fp = fopen( port_path, "r" );
	if ( fp ) {
		while ( fgets( buf, sizeof(buf), fp ) ) {
			p = strstr( buf, ":" );
			if ( p ) {
				sscanf((p+1), "%s", port);
			
				if ( !strcmp( port, portInStr ) ) {
					fclose( fp );
					unlink( port_path );
					return 0;
				}
			}
		}
		fclose( fp );
		unlink( port_path );
	}
	
	return 1;
#endif

/* if the bind the port fail, it means this port is used,
   so we can not use this port.
*/

	int sockfd = -1;
	struct sockaddr_in my_addr;

	/*check the port*/
	if ( portIn < 1 || portIn > 65535 ) {
		tc_printf( "port is valid.\n" );
		return 0;
	}
	
	/*open udp socket and bind to a specific address*/
	if ( (sockfd = socket( AF_INET, SOCK_DGRAM, 0 )) == -1 ) {
		tcdbg_printf( "open socket fail.\n" );
	    return 0;
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons( portIn );
	my_addr.sin_addr.s_addr = INADDR_ANY;

	/*bind this socket to this address*/
	if ( bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1 ) {
		tcdbg_printf( "bind socket fail.\n" );
		close( sockfd );
	    return 0;
	}

	close( sockfd );
	
	return 1;
}


#ifdef TR143
int tr143UDPEchoCommitFunc(cfgNode_ptr pCfgNode)
{
	return 0;
}
#endif
int checkMACFormat(char *mac)
{
	int i, j=0;
	char newmac[20];
	char tmpS[20];

	if(strlen(mac) == 17) {
		for(i=0; i<17; i++) {
			if(mac[i] != ':') {
				newmac[j] = mac[i]; 
				j++;
			}
		}
		newmac[j] = '\0';
	}
	else if(strlen(mac) == 12)
		strcpy(newmac, mac);
	else
		return -1;

	for(i=0; i<12; i++) {
		if( (newmac[i]>='A' && newmac[i]<='F')
			||(newmac[i]>='a' && newmac[i]<='f')
			||(newmac[i]>='0' && newmac[i]<='9') ) {

			continue;
		}
		else
			return -1;
	}
	
	return 0;
}

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int
inet_pton4(const char *src,u_char *dst)
{
	int saw_digit, octets, ch;
	u_char tmp[4], *tp;

	saw_digit = 0;
	octets = 0;
	*(tp = tmp) = 0;
	while ((ch = *src++) != '\0') {

		if (ch >= '0' && ch <= '9') {
			u_int new = *tp * 10 + (ch - '0');

			if (new>255)
				return (0);
			*tp = new;
			if (! saw_digit) {
				if (++octets>4)
					return (0);
				saw_digit = 1;
			}
		} else if (ch == '.' && saw_digit) {
			if (octets == 4)
				return (0);
			*++tp = 0;
			saw_digit = 0;
		} else
			return (0);
	}
	if (octets < 4)
		return (0);
	memcpy(dst, tmp, 4);
	return (1);
}

int
inet_pton6(src, dst)
	const char *src;
	unsigned char *dst;
{
	static const char xdigits_l[] = "0123456789abcdef",
			  xdigits_u[] = "0123456789ABCDEF";
	unsigned char tmp[NS_IN6ADDRSZ], *tp, *endp, *colonp;
	const char *xdigits, *curtok;
	int ch, saw_xdigit;
	unsigned int val;

	memset((tp = tmp), '\0', NS_IN6ADDRSZ);
	endp = tp + NS_IN6ADDRSZ;
	colonp = NULL;
	/* Leading :: requires some special handling. */
	if (*src == ':')
		if (*++src != ':')
			return (0);
	curtok = src;
	saw_xdigit = 0;
	val = 0;
	while ((ch = *src++) != '\0') {
		const char *pch;

		if ((pch = strchr((xdigits = xdigits_l), ch)) == NULL)
			pch = strchr((xdigits = xdigits_u), ch);
		if (pch != NULL) {
			val <<= 4;
			val |= (pch - xdigits);
			if (val > 0xffff)
				return (0);
			saw_xdigit = 1;
			continue;
		}
		if (ch == ':') {
			curtok = src;
			if (!saw_xdigit) {
				if (colonp)
					return (0);
				colonp = tp;
				continue;
			}
			if (tp + NS_INT16SZ > endp)
				return (0);
			*tp++ = (unsigned char) (val >> 8) & 0xff;
			*tp++ = (unsigned char) val & 0xff;
			saw_xdigit = 0;
			val = 0;
			continue;
		}
		if (ch == '.' && ((tp + NS_INADDRSZ) <= endp) &&
		    inet_pton4(curtok, tp) > 0) {
			tp += NS_INADDRSZ;
			saw_xdigit = 0;
			break;	/* '\0' was seen by inet_pton4(). */
		}
		return (0);
	}
	if (saw_xdigit) {
		if (tp + NS_INT16SZ > endp)
			return (0);
		*tp++ = (unsigned char) (val >> 8) & 0xff;
		*tp++ = (unsigned char) val & 0xff;
	}
	if (colonp != NULL) {
		/*
		 * Since some memmove()'s erroneously fail to handle
		 * overlapping regions, we'll do the shift by hand.
		 */
		const int n = tp - colonp;
		int i;

		for (i = 1; i <= n; i++) {
			endp[- i] = colonp[n - i];
			colonp[n - i] = 0;
		}
		tp = endp;
	}
	if (tp != endp)
		return (0);
	memcpy(dst, tmp, NS_IN6ADDRSZ);
	return (1);
}

#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
static char pDeviceSummary[256];
#endif

char * achieveDeviceSummary()
{	
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	char linkSta[4] = "0";
	int ponsw = 0;
	sprintf(pDeviceSummary, "InternetGatewayDevice:1.0[](Baseline:1, IPPing:1");
#ifdef TCSUPPORT_WLAN
	sprintf(pDeviceSummary, "%s, WiFiLAN:1", pDeviceSummary);
#endif
	sprintf(pDeviceSummary, "%s, EthernetLAN:1, Time:1", pDeviceSummary);
	tcapi_get("XPON_LinkCfg", "LinkSta", linkSta);
	ponsw = atoi(linkSta);
	if ( 2 == ponsw ) // epon
		sprintf(pDeviceSummary, "%s, EPONWAN:1", pDeviceSummary);
	else
		sprintf(pDeviceSummary, "%s, GPONWAN:1", pDeviceSummary);
	sprintf(pDeviceSummary, "%s)", pDeviceSummary);
	return pDeviceSummary;
#else
	return "";
#endif
}

int isCwmpUrlIP(char* name)
{
	struct hostent *h = NULL;
	struct sockaddr_in addr = {0};
	ip4a ipAddr = 0;

	if(name == NULL)
		return 0;
	
	tc_printf("\r\nEnter reslove name: [%s]", name);
	/*add by brian*/
	if(checkName(name))
	{
		resolve((char *)name, &ipAddr, NULL);
		if(ipAddr != 0)
		{
			return 1;
		}
	}

	return 0;
}

int getDevregFlag()
{
#if defined(CT_COM_DEVICEREG)
	return getDevregInformFlag();
#else
	return 0;
#endif
}
