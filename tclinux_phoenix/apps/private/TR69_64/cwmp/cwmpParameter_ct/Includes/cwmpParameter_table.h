
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

#ifndef _PARAMETER_C_
#define _PARAMETER_C_

#include "cwmpParameter_table-share.h"

#define CWMP_BOOLEAN   		 		(BooleanType)	
#define CWMP_INTEGER     		 	(IntType)	
#define CWMP_UNSIGNEDINT   		 	(UnsignedIntType)	
#define CWMP_STRING     			(StringType)	
#define CWMP_OBJECT   		        (ObjectType)	
#define CWMP_DATE_TIME    	        (DateTimeType)	
#define CWMP_BASE64     			(Base64Type)	

#define TRUE 1
#define FALSE 0

//add by brian for ip ping diagnostic
#define IPPINGDIAGNOSTICFILE "/tmp/cwmp/ippingdiagnostic_stat"
#ifdef TCSUPPORT_TRACEROUTE
#define NO_QMARKS 0
#define TRACEROUTEDIAGNOSTICFILE "/tmp/cwmp/traceroutediagnostic_stat"
#define MAX_TRACEROUTBUF_LEN 32768 //for read traceroute dignostic result
#endif
#define IMCOMPLETEFLAG "0 packets received"
#define MAX_PINGBUF_LEN 2048 //for read ip ping dignostic result
//add by brian for dsl diagnostic
#define CWMPDSLDIAGNOSTIC "CDSLDiagnostic_Common"
//add by renyu for dsl interface config
#define INFOADSL_NODE "Info_Adsl"
#define INFOSHOW_NODE "Info_Showtime"
#define INFOLAST_NODE "Info_LastShow"
#define INFOTOTAL_NODE "Info_TotalShow"
#define INFOCURDAYST_NODE "Info_CurdayStShow"
#define INFOQUTHOURST_NODE "Info_QutHourStShow"
#define DHCPLEASE_NODE "DhcpLease_Entry%d"
#define PVC_NUMBER 8

#define ReadOnly  						0x00
#define Writable 						0x01
#define SEEK_SET 						0
#define QUOTATIONMARKSLEN				140    /* the length use for QuotationMarksExchange function */
#define DNS_INFO_PATH	"/etc/resolv.conf"

#define MAX_STRING_LENGTH 				256
#define MAX_VIRSERV_RULE 				10
#define MAX_LEN_OF_SUPPORTED_RATES 		12
//#define VOIP  							1 //add by renyu_20091225
#ifdef WITHVOIP
#define VOIP        1 /*use WITHVOIP define to decide using voip function or not. shnwind 20100203*/
#else
#define VOIP        0
#endif
#define PKT_QOS_RULE_MAX	16
#define SERVICE_LIST_LEN  		64


#ifdef TCSUPPORT_CT_2PWIFI
#define SWITCHPARA_MAX_ENTRY 2
#else
#define SWITCHPARA_MAX_ENTRY 4
#endif
#if defined(TCSUPPORT_WLAN_AC)
#define MAX_LAN_PORT_NUM     14
#else
#define MAX_LAN_PORT_NUM     8
#endif
#if defined(TCSUPPORT_CT_NETWORKMANAGESERVICE)
#define MAX_ACCOUNT_ITEM 				3
#endif
#define CWMP_PURE_BRIDGE_TYPE_IP_BRIDGED (1 << 2)

#define MAX_TR69_PARAM

/*wan Encap definition. dynamic/static/pppoe/pppoa/pure bridge mode*/
#define WAN_ENCAP_DYN			"0"
#define WAN_ENCAP_STATIC		"1"
#define WAN_ENCAP_PPP			"2"
#define WAN_ENCAP_BRIDGE		"3"

#define SEC_PER_DAY 86400 /*60*60*24  for ppp link-up time*/
#define SEC_PER_HOUR 3600 /*60*60         shnwind 2008.4.14*/
#define SEC_PER_MIN 60

#define MAX_VIRSERVER_NODE 	64
#define MAX_VIRSERVER_NODEL2 	10

#define MAX_VCPOOL_NUM 			16
#define MAXSSIDNUM				4

#define MAX_FORWARD_NUM 			16
#define MAX_ROUTE_NUM 			16

#if VOIP
#define VOICEPROFILENUM		1
#if defined(TCSUPPORT_CT_PON_CY)||defined (TCSUPPORT_CUC_VOIP_NODE)||defined(TCSUPPORT_CT_PON_C9)
#define PHYINTERFACENUM		1
#else
#if defined(TCSUPPORT_CT_1FXS)
#define PHYINTERFACENUM		1
#else
#define PHYINTERFACENUM		2
#endif
#endif
#define CODECSNUM_SIP			6
#define VOICEPOORQLNUM_SIP      10
#define CODECSNUM_NOSIP		     8
#define VOICEPOORQLNUM_NOSIP     2
#define CODECSNUM	8
#define VOICEPOORQLNUM	10  

#define SIPRESPONSENUM 1

#if defined(TCSUPPORT_CT_CWMP_WORK_COMPATIBLE)
#if defined(TCSUPPORT_CT_1FXS)
#define LINENUM					1
#else
#define LINENUM					2
#endif
#else
#if defined(TCSUPPORT_CT_PON_CY)||defined (TCSUPPORT_CUC_VOIP_NODE)||defined(TCSUPPORT_CT_PON_C9)
#if defined(TCSUPPORT_CT_PON_CY) && !defined(TCSUPPORT_CT_PON_CY_JX) && !defined(TCSUPPORT_CT_PON_JX)
#define LINENUM					2
#else
#define LINENUM					1
#endif
#elif defined(TCSUPPORT_CT_1FXS)
#define LINENUM					1
#else
#define LINENUM					2
#endif
#endif

#define CODECLIST_NUMBER 8

#endif

#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
#define MAX_CTCOM_PING_NUM 3
#endif
#if defined(TCSUPPORT_CT_E8DDNS)
#define MAX_DDNS_RULE 32
#endif
#define 	CFG_YES 			1
#define		CFG_NO				0
#define		NONECHILD			0
#define 	NOBITSET			0
#define		NODELAY				0
#define 	DELAY				1

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#define MAX_ACCESSLIMIT_NUM   4
#define MAX_LAN_IPV6_DEVICE_NODE 3
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#define MAX_USER_NUM		4
#define MAC_FILTER_NUM_PER_USER		8
#define IP_FILTER_NUM_PER_USER		8
#define WHITE_URL_NUM_PER_USER				8
#define BLACK_URL_NUM_PER_USER				8
#define APPFTPHISTORY_NUM 				10
#define APPFTPCLIENT_NUM 				10
#define MAX_APPFTP_ENTRY				(APPFTPHISTORY_NUM + APPFTPCLIENT_NUM)
#endif
#define MAX_DHCP_STATIC_LEASE   8
// #if/*TCSUPPORT       _COMPILE*/ defined(TCSUPPORT_C1_CUC)
// #define MAX_LOG_ALARM_NUM 100
// #endif/*TCSUPPORT       _COMPILE*/
#define MAXLEN_NODE_NAME	32
#define MAXLEN_ATTR_NAME	32
#define MAXLEN_TCAPI_MSG  1024

typedef struct cfgNode_s
{
	char 	 	*nodeName;
	char 	 	*node_id;
	uint32   	bitMap;  
	uint32	bitMap1;
	uint16	*bitMask;          
	uint16 	comFlag;
	uint8    	bitNum;
	uint8    	settingFlag;
	int     	(*commitFunc)(struct cfgNode_s* data);
}cfgNode_t,*cfgNode_ptr;


#define  	ATTRFLAG0 			0
#define 	ATTRFLAG1			1
#define 	ATTRFLAG2			2

typedef struct attr_s
{
	char *attrName;
	void *attrPtr;
	int attrLen;
	uint32 magic;
	int fIndex;
	uint32 fBit;
}attr_t, *attr_ptr;



/***************************************************************************************************/
							/*TR069 attribute struct flag.define here.*/
/***************************************************************************************************/
#define	CWMP_ATTR_FLAG						(1 << 0)
#define DSLLINK_ATTR_FLAG					(1 << 1)
#define WLAN_ATTR_FLAG						(1 << 2)
#define IPCON_ATTR_FLAG						(1 << 3)
//add by brian for ip ping diagnostic
#define IPPINGDIAGNOSTIC_ATTR_FLAG			(1 << 4)
//add by brian for dsl diagnostic
#define DSLDIAGNOSTIC_ATTR_FLAG				(1 << 5)
//add by brian for atm f5 loopback diagnostic
#define ATMDIAGNOSTIC_ATTR_FLAG				(1 << 6)
#ifdef TR111
#define TR111_ATTR_FLAG						(1 << 7)
#endif
#if VOIP
#define TR104_ATTR_FLAG						(1 << 8)
#endif
#define	DEVICEINFO_ATTR_FLAG				(1 << 9)
#define TIME_ATTR_FLAG						(1 << 10)
//add by renyu
#define DSLINTERCFG_ATTR_FLAG				(1 << 11)
#define DSLCOMINTERCFG_ATTR_FLAG			(1 << 12)
#define DSLCONNMGMT_ATTR_FLAG				(1 << 13)
#define DSLCONNMGMTSVR_ATTR_FLAG			(1 << 14)
#define DEVCONFIG_ATTR_FLAG					(1 << 15)

//add by brian for layer3forwarding
#define LAYER3FORWARDING_ATTR_FLAG			(1 << 16)
#define SHOWTIME_ATTR_FLAG					(1 << 17)
#define TOTAL_ATTR_FLAG						(1 << 18)
#define LASTSHOWTIME_ATTR_FLAG				(1 << 19)
#define CURRENTDAY_ATTR_FLAG				(1 << 20)
#define QUARTERHOUR_ATTR_FLAG				(1 << 21)
#define LANCONFIGSECURITY_ATTR_FLAG			(1 << 22)

//add by liujilin
#define LANDEVICE_ATTR_FLAG					(1 << 23)	

#define IPINTERFACE_ATTR_FLAG				(1 << 24)
#define ETHERINTERCFG_ATTR_FLAG				(1 << 25)
#define HOST_ATTR_FLAG						(1 << 26)

//add by yzwang 
#define QOS_ATTR_FLAG						(1 << 27)
#define USERITF_ATTR_FLAG					(1 << 28)
#ifdef TR143
#define	TR143_ATTR_FLAG						(1 << 29)
#endif

#ifdef DHCP_PROFILE
#define DHCPOPTION_ATTR_FLAG					(1 << 30)	
#define DHCPCONSER_ATTR_FLAG					(1 << 31)	
#endif


#if defined(TCSUPPORT_CT_ACCESSLIMIT)
#define	ACCESSLIMIT_ATTR_FLAG					(1 << 0)
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
#define CTCOM_IPPINGDIAGNOSTIC_ATTR_FLAG			(1 << 1)
#endif
#if defined(TCSUPPORT_CT_IPTV_NODE)
#define IPTV_ATTR_FLAG					(1 << 2)
#endif

#if defined(TCSUPPORT_CT_ALARMMONITOR)
#define DEVICEALARM_ATTR_FLAG					(1 << 3)	
#define DEVICEMONITOR_ATTR_FLAG					(1 << 4)	
#endif

#ifdef TCSUPPORT_SYSLOG
#define SYSLOG_ATTR_FLAG					(1 << 5)
#endif
#if defined(TCSUPPORT_CT_ALGABILITY)
#define ALGSWITCH_ATTR_FLAG					(1 << 6)
#endif
#ifdef CT_COM_DEVICEREG
#define CWMP_DEVICEREGISTRATION_ATTR_FLAG                       (1 << 7)
#endif
#ifdef TCSUPPORT_IPV6
#define SYS_ATTR_FLAG                       (1 << 8)
#endif
#if defined(TCSUPPORT_CT_USB_BACKUPRESTORE)
#define USBRESTORE_ATTR_FLAG                       (1 << 9)
#endif
#if defined(TCSUPPORT_C1_CUC) || defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#define CUACCOUNT_ATTR_FLAG                       (1 << 10)
#define CULOG_ATTR_FLAG                       (1 << 11)
#endif
#if defined(TCSUPPORT_CT_MIDWARE)
#define CT_MIDWARE_ATTR_FLAG                  (1 << 12)
#endif
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#define CT_USBRESTORE_ATTR_FLAG               	  (1 << 13)
#define CT_UPNP_ATTR_FLAG               		  (1 << 14)
#endif
#if defined(TCSUPPORT_CT_PON)
#define CT_PONLINK_ATTR_FLAG                	  (1 << 15)
#define CT_PONINTERCFG_ATTR_FLAG                  (1 << 16)
#define CT_PONINTERCFGSTATS_ATTR_FLAG             (1 << 17)
#endif
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#define X_CT_COM_DHCPV6_ATTR_FLAG                  (1 << 18)
#define X_CT_COM_ROUTERADV_ATTR_FLAG                  (1 << 19)
#endif
#if defined(TCSUPPORT_CT_WAN_PTM)
#define CT_VDSLLINK_ATTR_FLAG                	  (1 << 20)
#define PTMLINK_ATTR_FLAG    (1 << 21)
#endif
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#define LOOPBACKDETECT_ATTR_FLAG		(1 << 22)
#define DATASPEEDLIMIT_ATTR_FLAG		(1 << 23)
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#define CU_ATTR_FLAG                       (1 << 24)
#endif
#ifdef TCSUPPORT_TRACEROUTE 
#define TRACEROUTEDIAGNOSTIC_ATTR_FLAG    (1 << 25)
#endif
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
#define MONITOR_COLLECTOR_ATTR_FLAG			(1 << 26)	
#endif
#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
#define PPPOE_EMULATOR_ATTR_FLAG   (1 <<27)
#define PPPOE_EMULATOR_BRIDGE_ATTR_FLAG   (1 <<28)
#endif
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
#define IPOE_EMULATOR_ATTR_FLAG   (1 <<29)
#endif
#if defined(TCSUPPORT_CT_PHONEAPP)
#define X_CTCOM_MGT_ATTR_FLAG (1 <<30)
#endif
#if defined(TCSUPPORT_CMCC)
#define LANINTERFACES_ATTR_FLAG (1 <<31)
#endif


#if defined(TCSUPPORT_CT_JOYME)
#define DNS_SPEED_LIMIT_ATTR_FLAG (1 <<0)
#endif

/***************************************************************************************************/
						/*TR069 attribute struct flag.define here.*/
/***************************************************************************************************/

#define MAX_ATTR_LEN 						512
#define MAX_ATTR_OFFSET 					9

/***************************************************************************************************/
						/*TR069 attribute struct definition*/
/***************************************************************************************************/
#if defined(TCSUPPORT_CMCC)
/*the attribute of TR069 Parameter: InternetGatewayDevice.LANInterfaces.*/
/*the attribute of TR069 Parameter: InternetGatewayDevice.X_CMCC_Security.*/
#define LANINTERFACES_MAGIC_ATTR 	0x14584292
typedef struct LANInterfacesAttr
{
	uint8 aMACAddressNum;
	uint8 aAssociateNum;
	uint8 aLanEtherTypeFilter;
	uint8 aWlanEtherTypeFilter;

	uint8 aIPFilterInEnable;
	uint8 aIPFilterInPolicy;
	uint8 aIPFilterOutEnable;
	uint8 aIPFilterOutPolicy;

	uint8 aName;
	uint8 aEnable;
	uint8 aSourceIPStart;
	uint8 aSourceIPEnd;

	uint8 aDestIPStart;
	uint8 aDestIPEnd;
	uint8 aProtocol;
	uint8 aSourcePortStart;

	uint8 aSourcePortEnd;
	uint8 aDestPortStart;
	uint8 aDestPortEnd;
	uint8 Reserved_1;

	uint8  aX_CTCOM_Mode[4];
	uint8  aX_CTCOM_VLAN[4];
	uint8  aWlanX_CTCOM_Mode[4];
	uint8  aWlanX_CTCOM_VLAN[4];

	uint8 aUrlFilterEnable;
	uint8 aUrlFilterPolicy;
	uint8 aUrlFilter;
	uint8 Reserved_2;

	uint32 magic;  //This variable should be at the end of the structure. 
}LANInterfacesAttr;
#endif

#if defined(TCSUPPORT_CT_PHONEAPP)
/*the attribute of TR069 Parameter: InternetGatewayDevice.X_CT-COM_Mgt*/
#define   X_CTCOM_MGT_ATTR 0x1A2B3C4D
typedef struct X_CTCOM_MgtAttr
{

	uint8  aMgtURL;
	uint8  aPort;
	uint8  aHeartbeat;
	uint8  aAbility;

	uint8  aLocatePort;
	uint8  aVersion;  
	uint8  aSSN;
	uint8  Reserved;
}X_CTCOM_MgtAttr;
#endif

/*the attribute of TR069 Parameter: InternetGatewayDevice.DeviceInfo.*/
#define DEVICEINF_MAGIC_ATTR	0x11221122
typedef struct deviceInfoAttr
{
	uint8  aDeviceSummary;
	uint8  aLANDeviceNumberOfEntries;
	uint8  aWANDeviceNumberOfEntries;
	
	uint8  aManufacturer;
	uint8  aManufacturerOUI;
	uint8  aModelName;
	uint8  aDescription;
	
	uint8  aProductClass;
	uint8  aSerialNumber;
	uint8  aHardwareVersion;
	uint8  aSoftwareVersion;
	
	uint8  aSpecVersion;
	uint8  aProvisioningcode;
	uint8  aUpTime;
	uint8  aDeviceLog;
	

	uint8 aVendorCfgFileName;
	uint8 aVendorCfgFileDescrip;

#if defined(TCSUPPORT_CT_NETWORKMANAGESERVICE)
	uint8 aFtpEnable;
	uint8 aFtpUserName;
	uint8 aFtpPassword;
	uint8 aFtpPort;
	uint8 aTelnetEnable;
	uint8 aTelnetUserName;
	uint8 aTelnetPassword;
	uint8 aTelnetPort;
#if defined(TCSUPPORT_CT_PON_SC)
	uint8 aSshEnable;
	uint8 aSshUserName;
	uint8 aSshPassword;
	uint8 aSshPort;
	uint8 aTftpEnable;
	uint8 aTftpUserName;
	uint8 aTftpPassword;
	uint8 aTftpPort;
	uint8 aSnmpEnable;
	uint8 aSnmpUserName;
	uint8 aSnmpPassword;
	uint8 aSnmpPort;
#endif
	uint8 aMaintenanceAccountEnable;
	uint8 aMaintenanceAccountUsername;
	uint8 aMaintenanceAccountPassword;

	uint8 Reserved1[1];
#endif

#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT) || defined(TCSUPPORT_TRUE_LANDING_PAGE)
	uint8 aMaintenancePortalEnable;
#endif
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
	uint8 aMaintenancePortalComputer;
	uint8 aMaintenancePortalSTB;
	uint8 aMaintenancePortalPhone;
#endif

#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
	uint8 aCTCOMEnable;
	uint8 aCTCOMPingNumberOfEntries;
	
	uint8 Reserved2[2];
#endif

#if defined(TCSUPPORT_CT_MIDWARE)
	uint8  aMiddlewareURL;
	uint8  aTr069Enable;
	uint8  Reserved4[2];
#endif


#if defined(TCSUPPORT_CT_RECONNECT_NODE)
	uint8  aReConnect;
	uint8  Reserved3[3];
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	uint8  aInterfaceVersion;
	uint8  aDeviceType;
	uint8  aUPLink;
	uint8  aCapability;
	uint8  aCard;
	uint8  aCardInterVersion;
	uint8  aDMSEnable;
	uint8  aIPForwardModeEnable;
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	uint8  aMacAddress;
#endif
#if defined(TCSUPPORT_CT_FJ)
	uint8  aVoIPWANStatus;
	uint8  aOltRegTime;
	uint8  aITMSRegTime;
	uint8  aITMSServiceTime;
	uint8  Reserved5[3];
#endif
#if defined(TCSUPPORT_CT_ECN_GZ) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION)  || defined(TCSUPPORT_CT_DEV_ACCESS_TYPE) || defined(TCSUPPORT_CT_PPPINFORM)
	uint8  aECNDeviceType;
	uint8  aAccessType;
	uint8  Reserved6[2];
#endif
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	uint8  aPOTSDeviceNumber;
	uint8  aGPONSerialNumber;
	uint8  aBatteryStatus;
	uint8  Reserved_1;
	uint8  aProcessorLoad;
	uint8  aMemoryLoad;
#endif
#if defined(TCSUPPORT_CT_STB_TEST)
	uint8  aDetectionsState;
	uint8  aDetectionsResult;
	uint8  Reserved7[2];
#endif

	uint8 Reserved[3];
	uint32  magic;  //This variable should be at the end of the structure.
	
}deviceInfoAttr;
#ifdef CT_COM_DEVICEREG
/*the attribute of TR069 Parameter: InternetGatewayDevice.X_CT-COM_UserInfo..*/
#define CWMP_DEVICEREGISTRATION_ATTR 	0xBEEFDEAD 
typedef struct deviceRegistrationAttr
{
	uint8  aStatus;
	uint8  aResult;	
	uint8  aRetryTimes;	
	uint8  aRetryLimit;
	
	uint8  aUserName;	
	uint8  aUserId;			
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	uint8  aServiceNum;
	uint8  aServiceName;
	uint8  Reserved_2[2];
#endif
	/*  there are 11 bytes used, 
	 ** if you add other attribute,please update the stats.
	  */
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_DEVREG_PROV_CODE)
	uint8  aProvisioningCode;
#endif
#if defined(TCSUPPORT_CT_ADSL_HN)
	uint8 aRouteStatus;
	uint8 Reserved_3[3];
#endif
	uint8 Reserved[2];
	
	uint32  magic;  //This variable should be at the end of the structure. 
}deviceRegistrationAttr;
#endif

/*InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANDSLLinkConfig.*/
#define DSLLink_MAGIC_ATTR    0x11223344
typedef struct dslLinkAttr
{
	uint8  aWANDSLLinkCfg1Enable[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1LinkStatus[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1LinkType[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1AutoConfig[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1ModType[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1DestAddr[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1ATMEncap[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1FCSPreserved[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1ATMQoS[MAX_WAN_NODE];
	uint8  aATMPeakCellRate[MAX_WAN_NODE];
	uint8  aATMMaximumBurstSize[MAX_WAN_NODE];
	uint8  aATMSustainableCellRates[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1ATMTransmittedBlocks[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1ATMReceivedBlocks[MAX_WAN_NODE];

	uint8  aWANDSLLinkCfg1VCSearchList;
	uint8  aWANDSLLinkCfg1AAL5CRCErrors;
	uint8  aWANDSLLinkCfg1ATMCRCErrors;
	uint8  aWANDSLLinkCfg1ATMHECErrors;
	
	/*	there are 116 bytes used,
	 **  if you add other attribute,please update the stats.
	  */
#if defined(TCSUPPORT_CT_QOS)
	uint8 aWANDSLLinkCfg1CT_COM_VLAN;
	uint8 Reserved[79];
#else
	uint8 Reserved[80];
#endif
	
	uint32  magic;  //This variable should be at the end of the structure. 
}dslLinkAttr;

//add by brian for ip ping diagnostic
#define IPPINGDIAGNOSTIC_MAGIC_ATTR 0x44332211 
typedef struct ipPingDiagnosticAttr
{
	uint8 aIpPingDiagnosticsState;
	uint8 aIpPingInterface;
	uint8 aIpPingHost;
	uint8 aIpPingNumberOfRepetitions;
	uint8 aIpPingTimeout;
	uint8 aIpPingDataBlockSize;
	uint8 aIpPingDSCP;
	uint8 aIpPingSuccessCount;
	uint8 aIpPingFilureCount;
	uint8 aIpPingAverageResponseTime;
	uint8 aIpPingMinimumResponseTime;
	uint8 aIpPingMaxumumResponseTime;
	uint8 reserved[16];	
	uint32  magic;
}ipPingDiagnosticAttr;


#ifdef TCSUPPORT_TRACEROUTE
#define TRACEROUTEDIAGNOSTIC_MAGIC_ATTR 0x33442211
typedef struct traceRouteDiagnosticAttr
{
	uint8 aTraceRouteDiagnosticsState;
	uint8 aTraceRouteInterface;
	uint8 aTraceRouteHost;
	uint8 aTraceRouteNumberOfTries;
	uint8 aTraceRouteTimeout;
	uint8 aTraceRouteDataBlockSize;
	uint8 aTraceRouteDSCP;
	uint8 aTraceRouteMaxHopCount;
	uint8 aTraceRouteResponseTime;
	uint8 aTraceRouteRouteHopsNumberOfEntries;
	uint8 aTraceRouteHopHost;
	uint8 aTraceRouteHopHostAddress;
	uint8 aTraceRouteHopErrorCode;
	uint8 aTraceRouteHopRTTimes;	
	uint8 reserved[14];	
	uint32  magic;
}traceRouteDiagnosticAttr;

#endif
//add by brian for dsl diagnostic
#define DSLDIAGNOSTIC_MAGIC_ATTR 0x44331122 
typedef struct dslDiagnosticAttr
{
	uint8 aWANDSLDiagnostics_LoopDiagnosticsState;
	uint8 aWANDSLDiagnostics_ACTPSDds;
	uint8 aWANDSLDiagnostics_ACTPSDus;
	uint8 aWANDSLDiagnostics_ACTATPds;
	uint8 aWANDSLDiagnostics_ACTATPus;
	uint8 aWANDSLDiagnostics_HLINSCds;
	uint8 aWANDSLDiagnostics_HLINpsds;
	uint8 aWANDSLDiagnostics_QLNpsds;
	uint8 aWANDSLDiagnostics_SNRpsds;
	uint8 aWANDSLDiagnostics_BITSpsds;
	uint8 aWANDSLDiagnostics_GAINSpsds;
#if defined(TCSUPPORT_CT_WAN_PTM)
	uint8 aWANDSLDiagnostics_HLINSCus;
	uint8 aWANDSLDiagnostics_HLINGds;
	uint8 aWANDSLDiagnostics_HLINGus;
	uint8 aWANDSLDiagnostics_HLINpsus;
	uint8 aWANDSLDiagnostics_HLOGGds;
	uint8 aWANDSLDiagnostics_HLOGGus;
	uint8 aWANDSLDiagnostics_HLOGpsds;
	uint8 aWANDSLDiagnostics_HLOGpsus;
	uint8 aWANDSLDiagnostics_HLOGMTds;
	uint8 aWANDSLDiagnostics_HLOGMTus;
	uint8 aWANDSLDiagnostics_QLNGds;
	uint8 aWANDSLDiagnostics_QLNGus;
	uint8 aWANDSLDiagnostics_QLNpsus;
	uint8 aWANDSLDiagnostics_QLNMTds;
	uint8 aWANDSLDiagnostics_QLNMTus;
	uint8 aWANDSLDiagnostics_SNRGds;
	uint8 aWANDSLDiagnostics_SNRGus;
	uint8 aWANDSLDiagnostics_SNRpsus;
	uint8 aWANDSLDiagnostics_SNRMTds;
	uint8 aWANDSLDiagnostics_SNRMTus;
	uint8 aWANDSLDiagnostics_LATNpbds;
	uint8 aWANDSLDiagnostics_LATNpbus;
	uint8 aWANDSLDiagnostics_SATNds;
	uint8 aWANDSLDiagnostics_SATNus;
#endif
	uint8 reserved[53];	
	uint32  magic;
}dslDiagnosticAttr;

#ifdef TR111
//add by renyu for tr111
#define TR111_MAGIC_ATTR 0x11223344
typedef struct tr111Atttr
{
	uint8 amDevNotificationLimit;

	uint8 Reserved[3];
	
	uint32  magic;  
}tr111Attr;
#endif
#if VOIP
//add by renyu for tr104
#define TR104_MAGIC_ATTR 0x11223344
typedef struct tr104Atttr
{
	uint8 aVoiceProfileNmberOfEntries;
	uint8 aMaxProfileCount;
	uint8 aMaxLineCount;

	uint8 aMaxSessionsPerLine;
	uint8 aMaxSessionCount;
	uint8 aRegions;
	uint8 aRTCP;
	uint8 aSRTP;
	uint8 aRTPRedundancy;
	uint8 aDSCPCoupled;
	uint8 aEthernetTaggingCoupled;
	uint8 aPSTNSoftSwitchOver;
	uint8 aFaxT38;
	uint8 aFaxPassThrough;
	uint8 aModemPassThrough;
	uint8 aToneGeneration;
	uint8 aRingGeneration;
	uint8 aCUNumberingPlan;
	uint8 aButtonMap;
	uint8 aVoicePortTests;
	
#if defined(TCSUPPORT_CUC_VOIP_NODE)
	uint8 aLocalPortMin[VOICEPROFILENUM];
	uint8 aLocalPortMax[VOICEPROFILENUM];
	uint8 aTelephoneEventPayloadType[VOICEPROFILENUM];
	uint8 aWANName[VOICEPROFILENUM];
	uint8 aRtcpEnable[VOICEPROFILENUM];
	uint8 aTxRepeatInterval[VOICEPROFILENUM];
	uint8 aX_CU_RtcpxrFlag[VOICEPROFILENUM];
	uint8 aRdnEnable[VOICEPROFILENUM];
	uint8 aPayloadType[VOICEPROFILENUM];
	uint8 aEnableFixedStart[VOICEPROFILENUM];
#endif

	uint8 aSIPProxyServer;
	uint8 aSIPProxyServerPort;
	uint8 aSIPProxyServerTransport;
	
	uint8 aSIPRegistrarServer;
	uint8 aSIPRegistrarServerPort;
	uint8 aSIPRegistrarServerTransport;
	uint8 aOutboundProxy;
	
	uint8 aOutboundProxyPort;
	uint8 aLineEnable[LINENUM];
	uint8 aLineSIPAuthUserNameaLineEnable[LINENUM];	
	uint8 aLineSIPAuthPasswordaLineEnable[LINENUM];
	uint8 aLineSIPAuthURI[LINENUM];
	
	uint8 aProfileFaxT38;
#if defined(TCSUPPORT_CUC_VOIP_NODE)
       uint8 aBitRateFaxT38;
       uint8 aTCFMethod;
	uint8 aHighSpeedRedundancy;
	uint8 aLowSpeedRedundancy;
	uint8 aPortAdd2;
#endif

	uint8 aEchoCancellationEnable[LINENUM];
	uint8 aCodec[CODECLIST_NUMBER];	
	uint8 aPacketizationPeriod[CODECLIST_NUMBER];	
	
	uint8 aPriority[CODECLIST_NUMBER];
	uint8 aDigitMapEnable[VOICEPROFILENUM];
	uint8 aDigitMap[VOICEPROFILENUM];
#if defined(TCSUPPORT_CT)
	uint8 aDigitMapSpecialEnable;
	uint8 aDigitMapSpecial;
#endif
	uint8 aSrverType[VOICEPROFILENUM];

	uint8 aH248DMName[VOICEPROFILENUM];
	uint8 aH248DigitMap[VOICEPROFILENUM];
	uint8 aStartDigitTimer[VOICEPROFILENUM];
	uint8 aInterDigitTimerShort[VOICEPROFILENUM];
	uint8 aInterDigitTimerLong[VOICEPROFILENUM];
	uint8 aHangingReminderToneTimer[VOICEPROFILENUM];

	uint8 aBusyToneTimer[VOICEPROFILENUM];
	uint8 aNoAnswerTimer[VOICEPROFILENUM];

#if defined(TCSUPPORT_CUC_VOIP_NODE)
       uint8 aEnable[VOICEPROFILENUM];
       uint8 aReset[VOICEPROFILENUM];
	uint8 aNumberOfLines[VOICEPROFILENUM];
	uint8 aName[VOICEPROFILENUM];
	uint8 aSignalingProtocol[VOICEPROFILENUM];
	uint8 aMaxSessions[VOICEPROFILENUM];
	uint8 aDTMFMethod[VOICEPROFILENUM];
	uint8 aDTMFMethodG711[VOICEPROFILENUM];
	uint8 aX_CU_DigitMapMatchMode[VOICEPROFILENUM];
	uint8 aX_CU_WANName[VOICEPROFILENUM];
	uint8 aX_CU_InterfaceState[VOICEPROFILENUM];
	uint8 aSrverType_CU[VOICEPROFILENUM];
	uint8 aStartDigitTimer_CU[VOICEPROFILENUM];
	uint8 aInterDigitTimerShort_CU[VOICEPROFILENUM];
	uint8 aInterDigitTimerLong_CU[VOICEPROFILENUM];
	uint8 aHangingReminderToneTimer_CU[VOICEPROFILENUM];

	uint8 aBusyToneTimer_CU[VOICEPROFILENUM];
	uint8 aNoAnswerTimer_CU[VOICEPROFILENUM];

#endif

	
	uint8 aInterfaceID[PHYINTERFACENUM];
	uint8 aPhyPort[PHYINTERFACENUM];
	uint8 aTestState;

	uint8 aTestSelector;	
	uint8 aPhoneConnectivity;
	uint8 aTestType;
	uint8 aCalledNumber;

	uint8 aCallHoldTimer;
	uint8 aCalledWaitTimer;

	uint8 aConfirmEnable;
	uint8 aConfirmNumber;
	uint8 aConfirmResult;
	uint8 aStatus;
	
	uint8 aConclusion;
	uint8 aCallerFailReason;
	uint8 aCalledFailReason;
	uint8 aFailedResponseCode;

	uint8 aIsTestOnBusy;
	uint8 aLineTestConclusion;
	uint8 aAGACVoltage;
	uint8 aBGACVoltage;
	uint8 aABACVoltage;
	uint8 aAGDCVoltage;
	uint8 aBGDCVoltage;
	uint8 aABDCVoltage;
	uint8 aAGInsulationResistance;
	uint8 aBGInsulationResistance;
	uint8 aABInsulationResistance;
	uint8 aAGCapacitance;
	uint8 aBGCapacitance;
	uint8 aABCapacitance;

	uint8 aDialMode;
	uint8 aHookFlashUpTime;
	uint8 aHookFlashDownTime;
	uint8 aReversePolePulse;
	uint8 aKcType;
	uint8 aClipTransWhen;
	
	uint8 aClipFormat;
	uint8 aClipSendDateTimeEnable;
	uint8 aClipSendDateTime;
	uint8 aRingDCVoltageOverlapped;
	uint8 aDataServiceMode;
	uint8 aClipReversePole;
	uint8 aMWIRingFlag;

	uint8 aStatTime[VOICEPOORQLNUM];
	uint8 aTxPackets[VOICEPOORQLNUM];
	uint8 aRxPackets[VOICEPOORQLNUM];
	uint8 aMeanDelay[VOICEPOORQLNUM];

	uint8 aMeanJitter[VOICEPOORQLNUM];
	uint8 aFractionLoss[VOICEPOORQLNUM];
	uint8 aLocalIPAddress[VOICEPOORQLNUM];
	uint8 aLocalUDPPort[VOICEPOORQLNUM];

	uint8 aFarEndIPAddress[VOICEPOORQLNUM];
	uint8 aFarEndUDPPort[VOICEPOORQLNUM];
	uint8 aMosLq[VOICEPOORQLNUM];
	uint8 aListCodec[VOICEPOORQLNUM];

	uint8 aCap_DigitMap;	
	uint8 aSignalingProtocols;
	uint8 aHeartbeat;

    uint8 LineCodecListEnable[CODECSNUM];
	uint8 aEntryID[CODECSNUM];
	uint8 aCs_Codec[CODECSNUM];	
	uint8 aBitRate[CODECSNUM];
	uint8 aCs_PacketizationPeriod[CODECSNUM];
	uint8 aSilenceSuppression[CODECSNUM];
	
	uint8 aRTPVLANIDMark[VOICEPROFILENUM];
	uint8 aRTPlpMark[VOICEPROFILENUM];

	uint8 aRTPDSCPMark[VOICEPROFILENUM];
	uint8 aSIPStandby_ProxyServer;
	uint8 aSIPStandby_ProxyServerPort;
	uint8 aSIPStandby_ProxyServerTransport;

	uint8 aSIPStandby_RegistrarServer;
	uint8 aSIPStandby_RegistrarServerPort;
	uint8 aSIPStandby_RegistrarServerTransport;
	uint8 aSIPStandby_OutboundProxy;

	uint8 aSIPStandby_OutboundProxyPort;
	uint8 aSIPUserAgentDomain;
	uint8 aSIPUserAgentPort;
	uint8 aSIPUserAgentTransport;

	uint8 aSIPVLANIDMark;
	uint8 aSIPlpMark;
	uint8 aSIPDSCPMark;
	uint8 aSIPHeartbeatSwitch;

	uint8 aSIPHeartbearCycle;
	uint8 aSIPHeartbearCount;
	uint8 aSIPSessionUpdateTimer;
	uint8 aSIPRegisterRetryInterval;

	uint8 aSIPRegisterExpires;
	uint8 aSIPImplicitRegistrationEnable;
	uint8 aMediaGatewayControler;
	uint8 aMediaGatewayControlerPort;

	uint8 aStandby_MediaGatewayControler;
	uint8 aStandby_MediaGatewayControlerPort;
	uint8 aMediaGatewayPort;
	uint8 aMessageEncodingType;

	uint8 aDeviceID;
	uint8 aDeviceIDType;
	uint8 aPhysicalTermID;
	uint8 aPhysicalTermIDConfigMethod;

	uint8 aPhysicalTermIDPrefix;
	uint8 aPhysicalTermIDAddLen;
	uint8 aRTPPrefix;
	uint8 aEphemeralTermIDAddLen;

	uint8 aEphemeralTermIDUniform;
	uint8 aEphemeralTermIDStart;
	uint8 aThreeHandShake;
	uint8 aLongTimer;

	uint8 aPendingTimerInit;
	uint8 aRetranIntervalTimer;
	uint8 aMaxRetranCount;
	uint8 aRetransmissionTime;

	uint8 aRetransmissionCycle;
	uint8 aHeartbeatMode;
	uint8 aHeartbeatCycle;
	uint8 aHeartbeatCount;

	uint8 aRegisterCycle;
	uint8 aMandatoryRegister;
	uint8 aAuthenticationMethID;
	uint8 aCOM_NumberPlan;

	uint8 aG711FAXEnable;
	uint8 aControlType;
	uint8 aLineStatus[LINENUM];	
	uint8 adial_tone_pattern[LINENUM];

	uint8 amcid_service[LINENUM];	
	uint8 ano_dialing_behaviour[LINENUM];
	uint8 ahold_service[LINENUM];
	uint8 athree_party_service[LINENUM];

	uint8 aconference_uri[LINENUM];
	uint8 ahotline_uri[LINENUM];
	uint8 acw_service[LINENUM];
	uint8 aReason;

#if defined(TCSUPPORT_CT)
	uint8 aPBXPrefixEnable;
	uint8 aPBXPrefix;
	uint8 aPBXDialSecondaryEnable;
#endif
	uint8 aLineH248PhysicalTermID;
	uint8 aEntryId[CODECLIST_NUMBER];
	uint8 aRegistResult;
	
	uint8 aEchoCancellationInUse[LINENUM];
	uint8 aTransmitGain[LINENUM];
	uint8 aReceiveGain[LINENUM];
	uint8 aEchoCancellationTail[LINENUM];


	uint8 aResetStatistics[LINENUM];
	uint8 aPacketsSent[LINENUM];
	uint8 aPacketsReceived[LINENUM];
	uint8 aBytesSent[LINENUM];

	uint8 aBytesReceived[LINENUM];	
	uint8 aPacketsLost[LINENUM];	
	uint8 aIncamingCallsReceived[LINENUM];	
	uint8 aIncamingCallsAnswered[LINENUM];

	uint8 aIncamingCallsConnected[LINENUM];
	uint8 aIncamingCallsFailed[LINENUM];
	uint8 aOutgoingCallsAttemped[LINENUM];
	uint8 aOutgoingCallsAnswered[LINENUM];

	uint8 aOutgoingCallsConnected[LINENUM];
	uint8 aOutgoingCallsFailed[LINENUM];
	uint8 aTotalCallTime[LINENUM];
	uint8 aIADDiagnosticsState[LINENUM];

#if defined(TCSUPPORT_CUC_VOIP_NODE)
      uint8 aReceivePacketLossRate[LINENUM];
      uint8 aFarEndPacketLossRate[LINENUM];
      uint8 aReceiveInterarrivalJitter[LINENUM];
      uint8 aFarEndInterarrivalJitter[LINENUM];
      uint8 aRoundTripDelay[LINENUM];
      uint8 aCallsDropped[LINENUM];
      uint8 aServerDownTime[LINENUM];

      uint8 aCallWaitingEnable[LINENUM];
      uint8 aCallForwardUnconditionalEnable[LINENUM];
      uint8 aCallForwardUnconditionalNumber[LINENUM];
      uint8 aCallForwardOnBusyEnable[LINENUM];
      uint8 aCallForwardOnBusyNumber[LINENUM];
      uint8 aCallForwardOnNoAnswerEnable[LINENUM];
      uint8 aCallForwardOnNoAnswerNumber[LINENUM];
      uint8 aCallForwardOnNoAnswerRingCount[LINENUM];
      uint8 aCallTransferEnable[LINENUM];
      uint8 aMWIEnable[LINENUM];
      uint8 aX_CU_HotlineEnable[LINENUM];
      uint8 aX_CU_HotlineNumber[LINENUM];
      uint8 aX_CU_HotlineTimer[LINENUM];
      uint8 aX_CU_conference_uri[LINENUM];
      
      uint8 aPhyReferenceList[LINENUM];
#endif

#if defined(TCSUPPORT_CUC_VOIP_NODE) || defined(TCSUPPORT_CT_PON_SC)
      uint8 aCallState[LINENUM];
#endif

#if defined(TCSUPPORT_CUC_VOIP_NODE) || defined(TCSUPPORT_CT_PON_GD)
      uint8 aDirectoryNumber[LINENUM];
#endif

#if defined(TCSUPPORT_CUC_VOIP_NODE)
       uint8 aTimerT1;
       uint8 aTimerT2;
       uint8 aTimerT4;
	uint8 aUseCodecPriorityInSDPResponse;

       uint8 aSIPResponseNumber[SIPRESPONSENUM];
	uint8 aTone[SIPRESPONSENUM];
	uint8 aX_CU_Duration[SIPRESPONSENUM];
	
	uint8 aSoftswitchVendor;
	uint8 aSoftswitchVersion;
#endif



	uint8 aTestServer;

	uint8 aDeviceName;
	uint8 aDSCPMark;
	uint8 aAuth;
	uint8 aAuthInitKey;
	uint8 aAuthmgid;
	uint8 aAuthHeader;
	uint8 aHeaderSecurityType;
	uint8 aRfc2833EncryptKey;
	uint8 aMsgFormat;
	uint8 aUdpAlfFlag;
	uint8 aMaxreTransTime;
	uint8 aMinreTransTime;
	uint8 aMGProvisionalRespTime;
	uint8 aMTUValue;
	uint8 aMsgSegmentation;
	uint8 aHeartBeatRetransTimer;
	uint8 aMWDBaseTime;
	uint8 aMWD;
	uint8 aMgcSwitchMethod;
	uint8 aLastRegisterErrorReason[LINENUM];
#if defined(TCSUPPORT_CT_PON_SC)
	uint8 aSubsUA[LINENUM];
	uint8 aSubsReg[LINENUM];
#endif
#ifdef TCSUPPORT_PON_ROSTELECOM
	uint8 aX_RTK_3PTY[LINENUM];
#endif
	//199 node in TR104 still not impliment
	uint8 Reserved[65];
	
	uint32  magic;  
}tr104Attr;
#endif


#if defined(TCSUPPORT_CT_IPTV_NODE)
//add by xmdai for IPTV
#define IPTV_MAGIC_ATTR 0x11222233
typedef struct iptvAttr
{
	uint8 aIGMPEnable;
	uint8 aProxyEnable;
	uint8 aSnoopingEnable;
	uint8 aSTBNum;
	uint8 aSTBMAC;
	
	uint8 Reserved[15];
	
	uint32  magic;  
}iptvAttr;
#endif

//add by brian for atm f5 loopback diagnostic
#define ATMDIAGNOSTIC_MAGIC_ATTR 0x44112233
typedef struct atmDiagnosticAttr
{
	uint8 aWANATMF5Loopback_DiagnosticsState[MAX_WAN_NODE];
	uint8 aWANATMF5Loopback_NumberOfRepetitions[MAX_WAN_NODE];
	uint8 aWANATMF5Loopback_Timeout[MAX_WAN_NODE];
	uint8 aWANATMF5Loopback_SuccessCount[MAX_WAN_NODE];
	uint8 aWANATMF5Loopback_FailureCount[MAX_WAN_NODE];
	uint8 aWANATMF5Loopback_AverageResponseTime[MAX_WAN_NODE];
	uint8 aWANATMF5Loopback_MinimumResponseTime[MAX_WAN_NODE];
	uint8 aWANATMF5Loopback_MaximumResponseTime[MAX_WAN_NODE];	
	uint32  magic;
}atmDiagnosticAttr;

//add by yzwang_20100108 for timezone
#define TIME_MAGIC_ATTR	0x12121212
typedef struct timeAttr
{
	uint8  aTimeNTPServer1;
	uint8  aTimeCurrentLocalTime;
	uint8  aTimeLocalTimeZone;
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	uint8  aTimeNTPServer2;
	uint8  aTimeNTPServer3;
	uint8  aTimeNTPServer4;
	uint8  aTimeNTPServer5;
	uint8  aNTPInterval;
	uint8  aNTPServerType;
	uint8  Reserved2[2];
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER) || defined(TCSUPPORT_CT_PON_GD)
	uint8  aEnable;
	uint8  aStatus;
	uint8  aTimeLocalTimeZoneName;
	uint8  Reserved_1[1];
#endif

	uint8  Reserved[13];
	uint32  magic;  
}timeAttr;

//add by renyu for dsl interface config
#define DSLINTERCFG_MAGIC_ATTR 0x44331122 
typedef struct dslInterCfgAttr
{
	uint8 aStatus;
	uint8 aModulationType;
	uint8 aLineEncoding;
	uint8 aDataPath;
	
	uint8 aUpstreamCurrRate;
	uint8 aDownstreamCurrRate;
	uint8 aUpstreamMaxRate;
	uint8 aDownstreamMaxRate;
	
	uint8 aUpstreamNoiseMargin;
	uint8 aDownstreamNoiseMargin;
	uint8 aUpstreamAttenuation;
	uint8 aDownstreamAttenuation;
	
	uint8 aUpstreamPower;
	uint8 aDownstreamPower;
	uint8 aATURVendor;
	uint8 aATURCountry;
	
	uint8 aATUCVendor;
	uint8 aATUCCountry;
	uint8 aShowtimeStart;
	uint8 aTotalStart;

	uint8 aInterleaveDepth;	
	uint8 aATURANSIStd;
	uint8 aATURANSIRev;
	uint8 aATUCANSIStd;
	
	uint8 aATUCANSIRev;
#if defined(TCSUPPORT_CT_ADSLSETTING) || defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	uint8 aStandardUsed;
	uint8 reserved[6];	
#else
	uint8 reserved[7];	
#endif
	
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	uint8 aEnable;
	uint8 aLinkEncapsulationSupported;
	uint8 aLinkEncapsulationUsed;
	uint8 aLinkEncapsulationRequested;

	uint8 StandardsSupported;
	uint8 aAllowedProfiles;
	uint8 aCurrentProfiles;
	uint8 aUPBOKLE; 

	uint8 aTRELLISds;
	uint8 aTRELLISus;
	uint8 aACTSNRMODEds;
	uint8 aACTSNRMODEus;

	uint8 aACTUALCE;
	uint8 aSNRMpbds;
	uint8 aSNRMpbus;
	uint8 aModulationMode;
#endif
	
	uint32  magic;
}dslInterCfgAttr;

//add by renyu for dsl common interface config
#define DSLCOMINTERCFG_MAGIC_ATTR 0x44331122 
typedef struct dslComInterCfgAttr
{
	uint8 aEnabledForInternet;
	uint8 aWANAccessType;
	uint8 aLayer1UpstreamMaxBitRate;
	uint8 aLayer1DownstreamMaxBitRate;
	
	uint8 aPhysicalLinkStatus;
	uint8 aTotalBytesSent;
	uint8 aTotalBytesReceived;
	uint8 aTotalPacketsSent;
	
	uint8 aTotalPacketsReceived;
	uint8 aEnabledBRDHCPCheck;
	
	uint8 reserved[2];	
	uint32  magic;
}dslComInterCfgAttr;

//add by renyu for dsl connection management
#define DSLCONNMGMT_MAGIC_ATTR 0x44331122 
typedef struct dslConnMgmtAttr
{
	uint8 aConnSerNumbeOfEntries;
	
	uint8 reserved[3];	
	uint32  magic;
}dslConnMgmtAttr;

//add by renyu for dsl connection management service
#define DSLCONNMGMTSVR_MAGIC_ATTR 0x44331122 
typedef struct dslConnMgmtSvrAttr
{
	uint8 aWANConnectionDevice;
	uint8 aWANConnectionService;
	uint8 aDestinationAddress;
	uint8 aLinkType;
	
	uint8 aConnectionType;
	
	uint8 reserved[3];	
	uint32  magic;
}dslConnMgmtSvrAttr;

//add by yzwang_20100118 for deviceConfig
#define DEVCONFIG_MAGIC_ATTR	0x12121213
typedef struct devConfigAttr
{
	uint8  aPersistentData;
	uint8  aConfigFile;

	uint8  Reserved[2];
	uint32 magic;  
}devConfigAttr;

#define LAYER3FORWARDING_MAGIC_ATTR 0x44332112
typedef struct layer3ForwardingAttr
{
	uint8  aForwarding1Enable[MAX_FORWARD_NUM];
	uint8  aForwarding1Status[MAX_FORWARD_NUM];	
	uint8  aForwarding1Type[MAX_FORWARD_NUM];
	uint8  aForwarding1DestIPAddress[MAX_FORWARD_NUM];
	
	uint8  aForwarding1SubnetMask[MAX_FORWARD_NUM];
	uint8  aForwarding1SourceIPAddress[MAX_FORWARD_NUM];
	uint8  aForwarding1SourceSubnetMask[MAX_FORWARD_NUM];
	uint8  aForwarding1GatewayIPAddress[MAX_FORWARD_NUM];
	
	uint8  aForwarding1Interface[MAX_FORWARD_NUM];
	uint8  aForwarding1ForwardingMetric[MAX_FORWARD_NUM];
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	uint8  aForwarding1ForwardingStaticRoute[MAX_FORWARD_NUM];
#endif
	uint8  aForwardNumberOfEntries;
	uint8  aDefaultConnectionService;
	uint8  Reserved[2];
#if defined(TCSUPPORT_CMCC)
#ifdef TCSUPPORT_IPV6
#define MAX_STATIC_ROUTE6_NUM 16
	uint8  aIPv6ForwardingEnable[MAX_STATIC_ROUTE6_NUM];
	uint8  aIPv6ForwardingAlias[MAX_STATIC_ROUTE6_NUM];
	uint8  aIPv6ForwardingDestIPPrefix[MAX_STATIC_ROUTE6_NUM];
	uint8  aIPv6ForwardingNextHop[MAX_STATIC_ROUTE6_NUM];
	uint8  aIPv6ForwardingInterface[MAX_STATIC_ROUTE6_NUM];
	uint8  aIPv6ForwardNumberOfEntries;
	uint8  Reserved_3[3];
#endif
#endif

	uint32 magic;  
}layer3ForwardingAttr;
//add by renyu for stats.Showtime
#define SHOWTIME_MAGIC_ATTR 0x44331122 
typedef struct showtimeAttr
{
	uint8 aReceiveBlock;
	uint8 aTransmitBlock;
	uint8 aCellDelin;
	uint8 aLinkRetrain;
	
	uint8 aInitErrors;
	uint8 aInitTimeouts;
	uint8 aLossOfFraming;
	uint8 aErroredSecs;
	
	uint8 aSeverelyErroredSecs;	
	uint8 aFecErrors;
	uint8 aATUCFECErrors;
	uint8 aHECErrors;
	
	uint8 aCRCErrors;	
	uint8 aATUCCRCErrors;
	
	uint8 reserved[2];	
	uint32  magic;
}showtimeAttr;

//add by renyu for stats.Showtime
#define TOTAL_MAGIC_ATTR 0x44331122 
typedef struct totalAttr
{
	uint8 aReceiveBlock;
	uint8 aTransmitBlock;
	uint8 aCellDelin;
	uint8 aLinkRetrain;
	
	uint8 aInitErrors;
	uint8 aInitTimeouts;
	uint8 aLossOfFraming;
	uint8 aErroredSecs;
	
	uint8 aSeverelyErroredSecs;	
	uint8 aFecErrors;
	uint8 aATUCFECErrors;
	uint8 aHECErrors;
	
	uint8 aCRCErrors;	
	uint8 aATUCCRCErrors;
	
	uint8 reserved[2];	
	uint32  magic;
}totalAttr;

//add by renyu for stats.LastShowtime
#define LASTSHOWTIME_MAGIC_ATTR 0x44331122 
typedef struct lastShowtimeAttr
{
	uint8 aReceiveBlock;
	uint8 aTransmitBlock;
	uint8 aCellDelin;
	uint8 aLinkRetrain;
	
	uint8 aInitErrors;
	uint8 aInitTimeouts;
	uint8 aLossOfFraming;
	uint8 aErroredSecs;
	
	uint8 aSeverelyErroredSecs;	
	uint8 aFecErrors;
	uint8 aATUCFECErrors;
	uint8 aHECErrors;

	uint8 aCRCErrors;	
	uint8 aATUCCRCErrors;
	
	uint8 reserved[2];	
	uint32  magic;
}lastShowtimeAttr;

//add by renyu for stats.CurrentDay
#define CURRENTDAY_MAGIC_ATTR 0x44331122 
typedef struct currentDayAttr
{
	uint8 aReceiveBlock;
	uint8 aTransmitBlock;
	uint8 aCellDelin;
	uint8 aLinkRetrain;
	
	uint8 aInitErrors;
	uint8 aInitTimeouts;
	uint8 aLossOfFraming;
	uint8 aErroredSecs;
	
	uint8 aSeverelyErroredSecs;	
	uint8 aFecErrors;
	uint8 aATUCFECErrors;
	uint8 aHECErrors;
	
	uint8 aCRCErrors;	
	uint8 aATUCCRCErrors;
	
	uint8 reserved[2];	
	uint32  magic;
}currentDayAttr;

//add by renyu for stats.QuarterHour
#define QUARTERHOUR_MAGIC_ATTR 0x44331122 
typedef struct quarterHourAttr
{
	uint8 aReceiveBlock;
	uint8 aTransmitBlock;
	uint8 aCellDelin;
	uint8 aLinkRetrain;
	
	uint8 aInitErrors;
	uint8 aInitTimeouts;
	uint8 aLossOfFraming;
	uint8 aErroredSecs;
	
	uint8 aSeverelyErroredSecs;	
	uint8 aFecErrors;
	uint8 aATUCFECErrors;
	uint8 aHECErrors;
	
	uint8 aCRCErrors;	
	uint8 aATUCCRCErrors;
	
	uint8 reserved[2];	
	uint32  magic;
}quarterHourAttr;
#define LANCONFIGSECURITY_MAGIC_ATTR 0x44332114
typedef struct lanconfigsecurityAttr
{
	uint8  aConfigPassword;	
	uint8  reserved[3];
	uint32 magic;
}lanconfigsecurityAttr;

#define LANDEVICE_MAGIC_ATTR	0x13121213
typedef struct lanDeviceAttr
{
	uint8  aLanEthIfNumOfEntries;
	uint8  aLanUSBIfNumOfEntries;
	uint8  aLanWLANConfigNumOfEntries;
#ifdef DHCP_PROFILE	
	uint8  aLanDHCPOptionNumOfEntries;
	uint8  aDHCPConditionalPoolNumOfEntries;
#endif	
	uint8  aDHCPServerCfg1;
	
	uint8  aDHCPServerEnable1;
	uint8  aDHCPRelay1;
	uint8  aMinAddress1;
	uint8  aMaxAddress1;
	
	uint8  aReservedAddr1;
	uint8  aSubnetMask1;
	uint8  aDNSServers1;
	uint8  aDomainName1;
	
	uint8  aIPRouters1;
	uint8  aDHCPLeaseTime1;	
	uint8  aIPInterfaceNum;	

	uint8  aUseAllocatedWAN1;
	uint8  aAssociatedConnection1;
	uint8  aPassthroughLease1;
	uint8  aPassthroughMACAddress1;
	uint8  aAllowedMACAddress1;
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	uint8 aHGWMinAddress;
	uint8 aHGWMaxAddress;
	uint8 aSTBMinAddress;
	uint8 aSTBMaxAddress;
	uint8 aPhoneMinAddress;
	uint8 aPhoneMaxAddress;
	uint8 aCameraMinAddress;
	uint8 aCameraMaxAddress;
	uint8 aComputerMinAddress;
	uint8 aComputerMaxAddress;
	uint8 aOPTION60Enable;
	uint8 aOPTION125Enable;
	uint8 aOPTION16Enable;
	uint8 aOPTION17Enable;
	uint8 aIPv6CfgDomainName;	
	uint8 aIPv6CfgDNSConfigType;	
	uint8 aIPv6CfgDNSWANConnection;	
	uint8 aIPv6CfgDNSServers;	
	uint8 aIPv6CfgRadvdPrefixMode;	
	uint8 aIPv6CfgRadvdDelegatedWanConn;		
	uint8 aIPv6CfgRadvdPrefix;	
	uint8 aIPv6CfgRadvdPltime;
	uint8 aIPv6CfgRadvdVltime;
	uint8 Reserved_0;	
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER) && defined(TCSUPPORT_CT_USB)
	uint8 aUSBPortStatus;
	uint8  Reserved_3[3];
#endif
#ifdef DHCP_PROFILE	
	uint8  Reserved[3];
#else
	uint8  Reserved[4];
#endif	
	uint32 magic;  
}lanDeviceAttr;

#define IPINTERFACE_MAGIC_ATTR	0x13121213
typedef struct ipInterfaceAttr
{
	uint8  aEnable;
	uint8  aIPInterfaceIPAddress;
	uint8  aIPInterfaceSubnetMask;	
	uint8  aIPInterfaceAddressingType;
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	uint8  aIPInterfaceIPv6Address[MAX_LAN_IPV6_DEVICE_NODE];
	uint8  aIPInterfaceIPv6LocalAddress;
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	uint8  aIPv6InterfaceAddress;
	uint8  Reserved_3[3];
#endif
	uint32 magic;  
}ipInterfaceAttr;

#define ETHERINTERCFG_MAGIC_ATTR	0x13121213
typedef struct etherInterCfgAttr
{
#if 1    //LAN_ETHERNET_INTERFACE
	uint8  aStatus[4];
	uint8  aMACAddress[4];
	
	uint8  aBytesSent[4];
	uint8  aBytesReceived[4];
	uint8  aPacketsSent[4];
	uint8  aPacketsReceived[4];
	uint8  aEnable[4];
	uint8  aduplexMode[4];
	uint8  amaxBitRate[4];
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	uint8  aX_CTCOM_Mode[4];
	uint8  aX_CTCOM_VLAN[4];
	uint8  aDetectionStatus[4];
#endif
#if defined(TCSUPPORT_CT_PMINFORM)
	uint8  aAccessEquipmentMac[4];
#endif
	uint8  aBSREnable[4];
#else
	uint8  aStatus;
	uint8  aMACAddress;
	
	uint8  aBytesSent;
	uint8  aBytesReceived;
	uint8  aPacketsSent;
	uint8  aPacketsReceived;
	
	uint8  Reserved[2];
#endif
	uint32 magic;  
}etherInterCfgAttr;

//add by renyu for Hosts.Host.{i}.
#define HOST_MAGIC_ATTR 0x44331122 
typedef struct hostAttr
{
	uint8 aHostNumber;
	uint8 aIPAddress;
	uint8 aAddressSource;
	uint8 aLeaseTimeRemaining;

	uint8 aMACAddress;	
	uint8 aHostName;
	uint8 aInterfaceType;
	uint8 aActive;

        uint8 aLayer2Interface;
        uint8 Reserved[3];
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	uint8 aHosttype;
	uint8  Reserved_3[3];
#endif
		
	uint32  magic;
}hostAttr;

#if defined(TCSUPPORT_CT_QOS)
#define QOS_MAGIC_ATTR			0x11223344
typedef struct qosAttr
{
	uint8 aMode;
	uint8 aEnable;
	uint8 aBandwidth;
	uint8 aPlan;
	uint8 aEnableForceWeight;
	uint8 aEnableDSCPMark;
	uint8 aEnable8021P;
	/* support 4 app at most */
	uint8 aAppName[4];
	uint8 aAppClassQueue[4];

	/* support 16 cls at most */
	uint8 aCSFClassQueue[16];
	uint8 aDSCPMarkValue[16];
	uint8 a8021PValue[16];
	uint8 aType[16][10];
	uint8 aMax[16][10];
	uint8 aMin[16][10]; 
	uint8 aProtocolList[16][10];

	/* support 6 queue at most */
	uint8 aPQEnable[10];
	uint8 aPriority[10];
	uint8 aWeight[10];

	uint8 Reserved[2];
	uint32 magic;
}qosAttr;
#else
//add by yzwang_20100125 for QoS
#define QOS_MAGIC_ATTR	0x12121214
typedef struct qosAttr
{
	uint8  aQoSQueueInterface[4];
	uint8  aQoSDropAlgorithm[4];
	uint8  aQoSSchedulerAlgorithm[4];
	uint8  aQoSQueueEnable[4];

	uint8  aQoSProtocol[PKT_QOS_RULE_MAX];
	uint8  aQoSDSCPMark[PKT_QOS_RULE_MAX];
	uint8  aQoSForwardingPolicy[PKT_QOS_RULE_MAX];
	uint8  aQoSClassQueue[PKT_QOS_RULE_MAX];
	uint8  aQoSClassPolicer[PKT_QOS_RULE_MAX];
	uint8  aQoSClassificationEnable[PKT_QOS_RULE_MAX];
	uint8  aQosDestIP[PKT_QOS_RULE_MAX];
	uint8  aQosDestMask[PKT_QOS_RULE_MAX];
	uint8  aQosSourceIP[PKT_QOS_RULE_MAX];
	uint8  aQosSourceMask[PKT_QOS_RULE_MAX];
	uint8  aQosDestPort[PKT_QOS_RULE_MAX];
	uint8  aQoSSourcePort[PKT_QOS_RULE_MAX];
	uint8  aQoSSourceMACAddress[PKT_QOS_RULE_MAX];
	uint8  aQoSDestMACAddress[PKT_QOS_RULE_MAX];
	uint8  aQoSPolicerEnable[PKT_QOS_RULE_MAX];

	uint8 Reserved[296];//reserved for 10+64 parameters
	uint32  magic;  
}qosAttr;
#endif

//add by yzwang_20100204 for UserInterface
#define USERITF_MAGIC_ATTR 	0x12121215
typedef struct userInterfaceAttr
{
	uint8  aWarrantyDate;

	uint8 Reserved[31];//reserved for 23 parameters
	uint32  magic;  
}userInterfaceAttr;

#ifdef TR143
#define TR143_MAGIC_ATTR	0x11223344
typedef struct tr143Attr
{
	uint8	aDiagDldTsports;
	uint8	aDiagUpTsports;	
	uint8 	aDldDiagnostics;
	uint8 	aDldInterface;
	
	uint8	aDldDownloadURL;
	uint8 	aDldDSCP;
	uint8 	aDldEthernetPriority;
	uint8 	aDldROMTime;
	
	uint8 	aDldBOMTime;
	uint8 	aDldEOMTime;
	uint8 	aDldTestBytesReceived;
	uint8 	aDldTotalBytesReceived;
	
	uint8 	aDldTCPOpenRequestTime;
	uint8 	aDldTCPOpenResponseTime;
	uint8 	aUldDiagnostics;
	uint8 	aUldInterface;
	
	uint8 	aUldUploadURL;
	uint8 	aUldDSCP;
	uint8 	aUldEthernetPriority;
	uint8 	aUldTestFileLength;
	
	uint8 	aUldROMTime;
	uint8 	aUldBOMTime;
	uint8 	aUldEOMTime;
	uint8 	aUldTotalBytesSent;
	
	uint8 	aUldTCPOpenRequestTime;
	uint8 	aUldTCPOpenResponseTime;
	uint8 	aUDPEchoEnable;
	uint8 	aUDPEchoInterface;
	
	uint8 	aEchoSourceIPAddress;
	uint8 	aUDPEchoUDPPort;
	uint8 	aUDPEchoPlusEnabled;
	uint8 	aUDPEchoPlusSupported;
	
	uint8 	aUDPEchoPacketsReceived;
	uint8 	aUDPEchoPacketsResponded;
	uint8 	aUDPEchoBytesReceived;
	uint8 	aUDPEchoBytesResponded;
	
	uint8 	aTimeFirstPacketReceived;
	uint8 	aTimeLastPacketReceived;
	
#if defined(TCSUPPORT_CT_PON_SC)
	uint8 	aDldSampledValues;
	uint8 	Reserved_1[3];
#endif

	uint8 	Reserved[2];
	uint32  magic;
}tr143Attr;
#endif

#ifdef DHCP_PROFILE
#define DHCPOPTION_MAGIC_ATTR 0x11223344
typedef struct dhcpOptionAttr
{
	uint8 aEnable;
	uint8 aTag;
	uint8 aValue;

	uint8 Reserved[1];	
	uint32  magic;  
}dhcpOptionAttr;

#define DHCPCONSER_MAGIC_ATTR 0x11223344
typedef struct dhcpConSerAttr
{
	uint8 aEnable;
	uint8 aPoolOrder;
	uint8 aSourceInterface;
	uint8 aVendorClassID;

	uint8 aVendorClassIDExclude;
	uint8 aVendorClassIDMode;
	uint8 aClientID;
	uint8 aClientIDExclude;

	uint8 aUserClassID;
	uint8 aUserClassIDExclude;
	uint8 aChaddr;
	uint8 aChaddrMask;

	uint8 aChaddrExclude;
	uint8 aLocallyServed;
	uint8 aMinAddress;
	uint8 aMaxAddress;

	uint8 aReservedAddresses;
	uint8 aSubnetMask;
	uint8 aDNSServers;
	uint8 aDomainName;

	uint8 aIPRouters;
	uint8 aDHCPLeaseTime;
	uint8 aUseAllocatedWAN;
	uint8 aAssociatedConnection;

	uint8 aDHCPServerIPAddress;
	uint8 aDHCPStaticAddressNum;
	uint8 aOptionNum;

	uint8 aconSerOption_Enable;
	uint8 aconSerOption_Tag;
	uint8 aconSerOption_Value;

	uint8 Reserved[2];	
	uint32  magic;  
}dhcpConSerAttr;
#endif

#if defined(TCSUPPORT_CT_ACCESSLIMIT)
#define ACCESSLIMIT_MAGIC_ATTR 0x91223344
typedef struct accessLimitAttr
{
	uint8 aMode;
	uint8 aTotalTerminalNum;
#if defined(TCSUPPORT_CMCC)
	uint8 aTerminalType;
	uint8 Reserved1[3];
#endif
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	uint8 aSTBEnable;
	uint8 aSTBNumber;
	uint8 aCameraEnable;
	uint8 aCameraNumber;
	uint8 aComputerEnable;
	uint8 aComputerNumber;
	uint8 aPhoneEnable;
	uint8 aPhoneNumber;
#endif
	uint8 Reserved[10];	
	uint32  magic;  
}accessLimitAttr;
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
#define CTCOM_IPPINGDIAGNOSTIC_MAGIC_ATTR 0x44333344
typedef struct ctcomIpPingDiagnosticAttr
{
	uint8 actcomIpPingDiagnosticsState[MAX_CTCOM_PING_NUM];
	uint8 actcomIpPingInterface[MAX_CTCOM_PING_NUM];
	uint8 actcomIpPingHost[MAX_CTCOM_PING_NUM];
	uint8 actcomIpPingNumberOfRepetitions[MAX_CTCOM_PING_NUM];
	uint8 actcomIpPingTimeout[MAX_CTCOM_PING_NUM];
	uint8 actcomIpPingDataBlockSize[MAX_CTCOM_PING_NUM];
	uint8 actcomIpPingDSCP[MAX_CTCOM_PING_NUM];
	uint8 actcomIpPingInterval[MAX_CTCOM_PING_NUM];
	uint8 actcomIpPingStop[MAX_CTCOM_PING_NUM];
	uint8 reserved[7];	
	uint32  magic;
}ctcomIpPingDiagnosticAttr;
#endif

#if defined(TCSUPPORT_CT_ALARMMONITOR)
#define MAX_INSTANCE_NUM	50
#define DEVICEALARM_MAGIC_ATTR 0x12341111
typedef struct deviceAlarmAttr
{
	uint8 aEnable;
	uint8 aMaxInstanceNum;
	uint8 aParaList[MAX_INSTANCE_NUM];
	uint8 aLimitMax[MAX_INSTANCE_NUM];
	uint8 aLimitMin[MAX_INSTANCE_NUM];
	uint8 aTimeList[MAX_INSTANCE_NUM];
	uint8 aMode[MAX_INSTANCE_NUM];
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	uint8 aAlarmNumber;
	uint8 Reserved_0[3];
#endif

	uint8 Reserved[4];	
	uint32  magic;  
}deviceAlarmAttr;

#define DEVICEMONITOR_MAGIC_ATTR 0x12341122
typedef struct deviceMonitorAttr
{
	uint8 aEnable;
	uint8 aMaxInstanceNum;
	uint8 aParaList[MAX_INSTANCE_NUM];
	uint8 aTimeList[MAX_INSTANCE_NUM];

	uint8 Reserved[2];	
	uint32  magic;  
}deviceMonitorAttr;
#endif

#ifdef TCSUPPORT_SYSLOG
#define SYSLOG_MAGIC_ATTR 0x11227788
typedef struct sysLogAttr
{
	uint8 aEnable;
	uint8 aLevel;

	uint8 Reserved[2];	
	uint32  magic;  
}sysLogAttr;
#endif

#ifdef TCSUPPORT_IPV6
#define SYS_MAGIC_ATTR 0x11227788
typedef struct sysAttr
{
	uint8 mode;

	uint8 Reserved[3];	
	uint32  magic;  
}sysAttr;
#endif

#if defined(TCSUPPORT_CT_ALGABILITY)
#define ALGSWITCH_MAGIC_ATTR 0x11227789
typedef struct algSwitchAttr
{
	uint8 aH323Enable;
	uint8 aSIPEnable;
	uint8 aRTSPEnable;
	uint8 aL2TPEnable;
	uint8 aIPSECEnable;
	uint8 aFTPEnable;
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	uint8 aPPTPEnable;
	uint8 Reserved_0[3];
#endif

	uint8 Reserved[2];	
	uint32  magic;  
}algSwitchAttr;
#endif

#if defined(TCSUPPORT_CT_USB_BACKUPRESTORE)
#define USBRESTORE_MAGIC_ATTR 0x11227790
typedef struct usbRestoreAttr
{
	uint8 aAutoRestore;
	
	uint8 Reserved[3];
	uint32  magic;  
}usbRestoreAttr;
#endif

#if defined(TCSUPPORT_C1_CUC) || defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#define  CUACCOUNT_MAGIC_ATTR 0x11227791
#define  CULOG_MAGIC_ATTR 0x11227792
typedef struct  X_CU_AccoutAttr
{

	uint8 aAdminName;
	uint8 aAdminPasswd;
	uint8 aAdminWebIP;
	uint8 aUserName;
	uint8 aUserPasswd;
	uint8 aUserWebIP;
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	uint8 aAdminaccountChange;
#endif

	uint8 Reserved[30];
	uint32  magic;  
}X_CU_AccoutAttr;
typedef struct  X_CU_LogAttr
{

	uint8 aLogEnable;
	uint8 aLogLevel;
	uint8 aLogUploadEnable;
	uint8 aLogUploadInterval;
	uint8 aAlarmEnable;
	uint8 aAlarmLevel;
	uint8 aUploadServer;
	uint8 aUsername;
	uint8 aPassword;
	uint8 aNumberOfAlarmInfo;
	// uint8 aAlarmID[MAX_LOG_ALARM_NUM];
	// uint8 aAlarmCode[MAX_LOG_ALARM_NUM];
	// uint8 aAlarmStatus[MAX_LOG_ALARM_NUM];
	// uint8 aPerceivedServerity[MAX_LOG_ALARM_NUM];
	// uint8 aAlarmRaisedTime[MAX_LOG_ALARM_NUM];
	// uint8 aAlarmClearedTime[MAX_LOG_ALARM_NUM];
	// uint8 aAlarmDetail[MAX_LOG_ALARM_NUM];
	uint8 Reserved[2];
	uint32  magic;  
}X_CU_LogAttr;
#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#define  CU_MAGIC_ATTR 0x11227794
typedef struct  cuAttr
{
	uint8 aPPPoEDiagState;
	uint8 aPPPoEDiagUsername;
	uint8 aPPPoEDiagPassword;
	uint8 aPPPoEDiagVLANID;
	
	uint8 aPPPoEDiagPort;
	uint8 aPPPoEDiagAuthProtocol;
	uint8 aPPPoEDiagRetryTimes;
	uint8 aPPPoEDiagResult;
	
	uint8 aPPPoEDiagSessionID;
	uint8 aPPPoEDiagExternalIPAddress;
	uint8 aPPPoEDiagDefaultGateway;	
	uint8 aPPPoEDiagWanInterface;
	uint8 aPPPoEDiagLanInterface;

	uint8 aPacketofAPPFilterEnable;
	uint8 aPortScanEnable;
	uint8 aInvalidPacketDenyEnable;
	uint8 aDoSEnable;

	uint8 aDdnsEnable;	
	uint8 aDdnsProvider;
	uint8 aDdnsUsername;
	uint8 aDdnsPassword;

	uint8 aDdnsServicePort;	
	uint8 aDdnsDomainName;
	uint8 aDdnsHostName;
	uint8 aDdnsWANPath;

	uint8 aDmzEnable;
	uint8 aDmzHostIP;
	
	
	uint8 aUserNumberOfEntries;
	
	uint8 aUserEnable[MAX_USER_NUM];
	uint8 aUserName[MAX_USER_NUM];
	uint8 aUserDescription[MAX_USER_NUM];	
	uint8 aNumberOfSubuser[MAX_USER_NUM];
	
	uint8 aMACFilterNumberOfEntries[MAX_USER_NUM];
	uint8 aIPFilterNumberOfEntries[MAX_USER_NUM];
	uint8 aWhiteListNumberOfEntries[MAX_USER_NUM];	
	uint8 aBlackListNumberOfEntries[MAX_USER_NUM];
	uint8 aURLListFilterMode[MAX_USER_NUM];
	
	uint8 aTimeEnable[MAX_USER_NUM];	
	uint8 aTimeRules[MAX_USER_NUM];
	
	uint8 aFilterMac[MAX_USER_NUM][MAC_FILTER_NUM_PER_USER];
	uint8 aFilterIP[MAX_USER_NUM][IP_FILTER_NUM_PER_USER];
	uint8 aWhiteURL[MAX_USER_NUM][WHITE_URL_NUM_PER_USER];
	uint8 aBlackURL[MAX_USER_NUM][BLACK_URL_NUM_PER_USER];

	uint8 aPortalEnable[MAX_USER_NUM];	
	uint8 aPortalComputer[MAX_USER_NUM];
	uint8 aPortalIPTV[MAX_USER_NUM];
	uint8 aPortalPhone[MAX_USER_NUM];	

	uint8 aFTPServerNumberOfEntries;
	uint8 aFTPClientNumberOfEntries;
	
	uint8 aFTPSvrUserRight;
	uint8 aFTPSvrPath;
	
	uint8 aFTPCltEnable[APPFTPCLIENT_NUM];
	uint8 aFTPCltFtpServerURL[APPFTPCLIENT_NUM];
	uint8 aFTPCltSvrPort[APPFTPCLIENT_NUM];	
	uint8 aFTPCltSvrUserName[APPFTPCLIENT_NUM];

	uint8 aFTPCltSvrPassword[APPFTPCLIENT_NUM];
	uint8 aFTPCltStatus[APPFTPCLIENT_NUM];

	uint8 aFactoryResetMode;
	uint8 Reserved[31];
	uint32  magic;  
}cuAttr;
#endif

#if defined(TCSUPPORT_CT_MIDWARE)
#define  CT_MIDWARE_MAGIC_ATTR 0x11227793
typedef struct  CT_MidwareAttr
{
	uint8 aCTUserIPAddress1;
	uint8 aCTUserIPAddress2;
	uint8 aCTUserIPAddress3;
	uint8 aCTUserIPAddress4;
	uint8 aCTUserIPAddress5;
	uint8 aCTUserIPAddress6;
	uint8 aCTUserIPAddress7;
	uint8 aCTUserIPAddress8;
	uint8 aTeleAccountChange;
	uint8 aCTMgtIPAddress;
	uint8 aMgtDNS;
	uint8 aMWSURL;
	uint8 aITMSURL;
	uint8 aInternetPvc;
	uint8 aCTEvent;
	uint8 aPushKeyEvent;
	
	uint8 Reserved[8];
	uint32  magic;  
}CT_MidwareAttr;
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#define CT_UPNP_MAGIC_ATTR 0x11227795
typedef struct CT_UPNPAttr
{
	uint8 aEnable;

	uint8 Reserved[3];
	uint32  magic;  
}CT_UPNPAttr;
#endif

#if defined(TCSUPPORT_CT_PON)
#define CT_PONINTERCFG_MAGIC_ATTR    0x11227797
typedef struct CT_PonInterCfgAttr
{
	uint8 aWANPonInterCfgStatus;
	uint8 aWANPonInterCfgTXPower;
	uint8 aWANPonInterCfgRXPower;
	uint8 aWANPonInterCfgTransTemperature;
	uint8 aWANPonInterCfgSupplyVottage;
	uint8 aWANPonInterCfgBiasCurrent;
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	uint8 aWANPonInterCfgEnable;
	uint8 aWANPonInterCfgStandardsSupported;
	uint8 aWANPonInterCfgStandardUsed;
	uint8 aWANPonInterCfgUpstreamMaxRate;
	uint8 aWANPonInterCfgDownstreamMaxRate;
	uint8 Reserved_3[3];
#endif
#if defined(TCSUPPORT_CT_PMINFORM)	
	uint8 aWANPonInterCfgUpData;
	uint8 aWANPonInterCfgDownData;
	uint8 aWANPonInterCfgTEMP;
	uint8 aWANPonInterCfgPacketLost;

	uint8 aWANPonInterCfgPLR;
	uint8 aWANPonInterCfgErrorCode;
	uint8 aWANPonInterCfgSER;
	uint8 aWANPonInterCfgMemRate;

	uint8 aWANPonInterCfgCPURate;
	uint8 aWANPonInterCfgPMInformEnable;
	uint8 aWANPonInterCfgPMInformLSURL;
	uint8 aWANPonInterCfgPMInformLUInterval;

	uint8 aWANPonInterCfgPMInformLCInterval;
	uint8 aWANPonInterCfgPMInformCREnable;
	uint8 aWANPonInterCfgPMInformMREnable;
	uint8 aWANPonInterCfgPMInformSEREnable;

	uint8 aWANPonInterCfgPMInformECEnable;
	uint8 aWANPonInterCfgPMInformPLREnable;
	uint8 aWANPonInterCfgPMInformPLEnable;
	uint8 aWANPonInterCfgPMInformTMPEnable;

	uint8 aWANPonInterCfgPMInformUpDEnable;
	uint8 aWANPonInterCfgPMInformDwDEnable;
	uint8 aWANPonInterCfgPMInformLanDevMACEnable;
	uint8 aWANPonInterCfgPMInformLanDevMAC;

	uint8 aWANPonInterCfgPMInformWlanDevMACEnable;
	uint8 aWANPonInterCfgPMInformWlanDevMAC;
	uint8 Reserved_2[2];
#endif
	uint8 Reserved[2];

	uint32  magic;  
}CT_PonInterCfgAttr;

#define CT_PONINTERCFGSTATS_MAGIC_ATTR    0x11227798
typedef struct CT_PonInterCfgStatsAttr
{
	uint8 aWANPonInterCfgStatsBytesSent;
	uint8 aWANPonInterCfgStatsBytesReceived;
	uint8 aWANPonInterCfgStatsPacketsSent;
	uint8 aWANPonInterCfgStatsPacketsReceived;
	uint8 aWANPonInterCfgStatsSUnicastPackets;
	uint8 aWANPonInterCfgStatsRUnicastPackets;
	uint8 aWANPonInterCfgStatsSMulticastPackets;
	uint8 aWANPonInterCfgStatsRMulticastPackets;
	uint8 aWANPonInterCfgStatsSBroadcastPackets;
	uint8 aWANPonInterCfgStatsRBroadcastPackets;
	uint8 aWANPonInterCfgStatsFECError;
	uint8 aWANPonInterCfgStatsHECError;
	uint8 aWANPonInterCfgStatsDropPackets;
	uint8 aWANPonInterCfgStatsSPausePackets;
	uint8 aWANPonInterCfgStatsRPausePackets;
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	uint8 aWANPonInterCfgStatsReceiveBlocks;
	uint8 aWANPonInterCfgStatsTransmitBlocks;
	uint8 aWANPonInterCfgStatsFCSError;
#endif
	uint8 Reserved;

	uint32  magic;  
}CT_PonInterCfgStatsAttr;
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#define  X_CT_COM_DHCPV6_MAGIC_ATTR 0x11227799
typedef struct  ctcomdhcpv6Attr
{
	uint8 aDhcpv6Enable;	
	uint8 aDhcpv6PoolEnable;
	uint8 aDhcpv6PoolMinAddr;
	uint8 aDhcpv6PoolMaxAddr;
	uint8 aDHCPv6PreferredLifeTime;
	uint8 aDHCPv6ValidLifeTime;
	uint8 Reserved[2];
	uint32  magic;  
}ctcomdhcpv6Attr;

#define  X_CT_COM_ROUTERADV_MAGIC_ATTR 0x1122779A
typedef struct  ctcomrouteradvAttr
{
	uint8 aRouterAdvEnable;	
	uint8 aRouterAdvMaxInterval;
	uint8 aRouterAdvMinInterval;
	uint8 aRouterAdvManageFlag;
	uint8 aRouterAdvOtherFlag;
	uint8 Reserved[3];
	uint32  magic;  
}ctcomrouteradvAttr;

#define  LOOPBACKDETECT_MAGIC_ATTR 0x1122779B
typedef struct LoopbackDetectAttr
{
	uint8 aLoopbackEnable;	
	uint8 aLoopExistPeriod;
	uint8 aLoopCancelPeriod;
	uint8 aVlanTag;
	uint8 aEthernetType;
	uint8 Reserved[3];
	uint32  magic;  
}LoopbackDetectAttr;

#define  DATASPEEDLIMIT_MAGIC_ATTR 0x1122779C
typedef struct DataSpeedLimitAttr
{
	uint8 aSpeedLimitModeUP;	
	uint8 aSpeedLimitModeDOWN;
	uint8 aInterfaceLimitUP;
	uint8 aInterfaceLimitDOWN;
	uint8 aVlanTagLimitUP;
	uint8 aVlanTagLimitDOWN;
	uint8 aIPLimitUP;
	uint8 aIPLimitDOWN;
	uint8 Reserved[4];
	uint32  magic;  
}DataSpeedLimitAttr;
#endif


#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
//#define MONITOR_COLLECTOR_MAXNUM	5

#define MONITOR_COLLECTOR_MAGIC_ATTR 0x1122779D
typedef struct MonitorCollectorAttr
{
	uint8 aEnable;
	uint8 aServerURL;
	uint8 aTftpPort;
	uint8 aMaxInstanceNum;
	uint8 aParaList[MONITOR_COLLECTOR_MAXNUM];
	uint8 aTimeList[MONITOR_COLLECTOR_MAXNUM];

	uint8 aQoeTestDownloadUrl;
	uint8 aQoeTestDownloadStatus;
	uint32  magic;  
}MonitorCollectorAttr;
#endif

#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
/*the attribute of TR069 Parameter: InternetGatewayDevice.X_CT-COM_PPPOE_EMULATOR..*/
#define PPPOEEMULATOR_MAGIC_ATTR 	0x14584286
typedef struct pppoeEmulatorAttr
{
	uint8  aDiagState;
	uint8  aUsername;	
	uint8  aPassword;	
	uint8  aWANInterface;
	
	uint8  aPPPAuthProto;	
	uint8  aRetryTimes;			
	uint8  aResult;
	uint8  aPPPSessionID;
	
	uint8  aExternalIP;
	uint8  aDefGw;
	uint8 Reserved[2];
	
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
	uint8 aErrorCode;
	uint8 aWanIPMode;
	uint8 aDsliteEnable;
	uint8 aAftrAddress;
	
	uint8 aWANIPv6IPAddress;
	uint8 aWANIPv6DNSServers;
	uint8 aWANIPv6Prefix;
	uint8 aWANDefaultIPv6GW;

	uint8 aLANIPv6Prefix;
	uint8 reserved_3[3];
#endif
	
	uint32  magic;  //This variable should be at the end of the structure. 
}pppoeEmulatorAttr;


/*the attribute of TR069 Parameter: InternetGatewayDevice.X_CT-COM_PPPOE_BridgeAutoEmulator..*/
#define PPPOEEMULATOR_BRIDGE_MAGIC_ATTR 	0x14584287
typedef struct pppoeEmulatorBridgeAttr
{
	uint8  aEnable;
	uint8  aUsername;	
	uint8  aPassword;	
	uint8  aTimeList;
	
	uint8  aFailRetryTimeList;	
	uint8 Reserved[3];
	
	uint32  magic;  //This variable should be at the end of the structure. 
}pppoeEmulatorBridgeAttr;

#endif
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
/*the attribute of TR069 Parameter: InternetGatewayDevice.X_CT-COM_IPoEDiagnostics.*/
#define IPOEEMULATOR_MAGIC_ATTR 	0x14584288
typedef struct ipoeEmulatorAttr
{
	uint8 aDiagnosticsState;
	uint8 aWANInterface;
	uint8 aUserMac;
	uint8 aVendorClassID;

	uint8 aResult;
	uint8 aLocalAddress;
	uint8 aDefaultGateway;
	uint8 aPingDestIPAddress;

	uint8 aPingNumberOfRepetitions;
	uint8 aTimeout;
	uint8 aSuccessCount;
	uint8 aFailureCount;
	
	uint8 aAverageResponseTime;
	uint8 aMinimumResponseTime;
	uint8 aMaximumResponseTime;
	uint8 reserved;

	uint32 magic;  //This variable should be at the end of the structure. 
}ipoeEmulatorAttr;
#endif
#if defined(TCSUPPORT_CT_JOYME)
/*the attribute of TR069 Parameter: InternetGatewayDevice.X_CT-COM_DNSSpeedLimit.*/
#define DNS_SPEED_LIMIT_MAGIC_ATTR 	0x14584289
typedef struct dnsSpeedLimitAttr
{
	uint8 aDnsSpeedLimitDomain;
	uint8 aDnsSpeedLimitAction;
	uint8 aDnsSpeedLimitHgwInfo;
	uint8 aDnsSpeedLimitDeviceInfo;

	uint32 magic;  /* This variable should be at the end of the structure. */
}dnsSpeedLimitAttr;
#endif


typedef enum AsynOpType
{
    ASYN_COMMIT = 1,
    ASYN_SET,
    ASYN_SAVE,
    ASYN_GET,
    ASYN_UNSET,
}AsynOpType;

typedef struct QueueNode
{
    AsynOpType opType;
    char node[MAXLEN_NODE_NAME];
    char attr[MAXLEN_ATTR_NAME];
    char value[MAXLEN_TCAPI_MSG];
    struct QueueNode *next;
}QueueNode, *QueueNodePtr;

/*attribute struct definition.*/

#if defined(TCSUPPORT_CMCC)
extern LANInterfacesAttr tcLANInterfacesAttr;
#endif

#if defined(TCSUPPORT_CT_PHONEAPP)
extern X_CTCOM_MgtAttr tcX_CTCOM_MgtAttr;
#endif

/*attribute struct definition.*/
extern 	deviceInfoAttr				tcDeviceInfoAttr;
extern 	cwmpAttr 
					tcCwmpAttr;
#ifdef CT_COM_DEVICEREG
extern	deviceRegistrationAttr		tcDeviceRegistrationAttr;
#endif

#if !defined(TCSUPPORT_CT_PON)
extern	dslLinkAttr 				tcDslLinkAttr;
#endif
extern 	wlanAttr					tcWlanAttr;
extern 	ipConAttr 					tcIpConAttr;
//add by brian for ip ping diagnostic
extern 	ipPingDiagnosticAttr 		tcIpPingDiagnosticAttrAttr;
#ifdef TCSUPPORT_TRACEROUTE
extern     traceRouteDiagnosticAttr 	tcTraceRouteDiagnosticAttrAttr;
#endif
#if !defined(TCSUPPORT_CT_PON)
//add by brian for dsl diagnostic
extern 	dslDiagnosticAttr 			tcdslDiagnosticAttr;
//add by brian for atm f5 loopback diagnostic
extern 	atmDiagnosticAttr 			tcatmDiagnosticAttr;
#endif
#ifdef TR111
extern 	tr111Attr 					tcTR111Attr;
#endif
#if VOIP
extern 	tr104Attr 					tcTR104Attr;
#endif
#if defined(TCSUPPORT_CT_IPTV_NODE)
extern 	iptvAttr 					tcIPTVAttr;
#endif
extern 	timeAttr 					tcTimeAttr;
//add by renyu
extern 	dslInterCfgAttr 			tcdslInterCfgAttr;
extern 	dslComInterCfgAttr			tcdslComInterCfgAttr;
#if !defined(TCSUPPORT_CT_PON)
extern 	dslConnMgmtAttr				tcdslConnMgmtAttr;
extern 	dslConnMgmtSvrAttr			tcdslConnMgmtSvrAttr;
#endif
extern 	devConfigAttr 				tcDevConfigAttr;
//add by brian for layer3forwarding
extern layer3ForwardingAttr 		tclayer3ForwardingAttr;
extern lanconfigsecurityAttr 		tclanconfigsecurityAttr;
extern 	showtimeAttr				tcshowtimeAttr;
extern 	totalAttr					tctotalAttr;
extern 	lastShowtimeAttr			tclastShowtimeAttr;
extern 	currentDayAttr				tccurrentDayAttr;
extern 	quarterHourAttr				tcquarterHourAttr;

extern  lanDeviceAttr          		tcLanDeviceAttr;
extern  ipInterfaceAttr          	tcipInterfaceAttr;
extern  etherInterCfgAttr          	tcetherInterCfgAttr;
extern	hostAttr					tchostAttr;

//extern	qosAttr			        	tcQoSAttr;
extern	userInterfaceAttr			tcUserInterfaceAttr;
#ifdef TR143
extern tr143Attr 					tcTr143Attr;
#endif

#ifdef DHCP_PROFILE
extern  dhcpOptionAttr          	tcdhcpOptionAttr;
extern  dhcpConSerAttr          	tcdhcpConSerAttr;
#endif
#if defined(TCSUPPORT_CT_ACCESSLIMIT)
extern  accessLimitAttr          	tcaccessLimitAttr;
#endif
//#ifdef TCSUPPORT_CT_QOS
extern  qosAttr					    tcQoSAttr;
//#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
extern ctcomIpPingDiagnosticAttr 	ctIpPingDiagnosticAttr;
#endif

#if defined(TCSUPPORT_CT_ALARMMONITOR)
extern	deviceAlarmAttr		tcdeviceAlarmAttr;
extern	deviceMonitorAttr	tcdeviceMonitorAttr;
#endif

#ifdef TCSUPPORT_SYSLOG
extern sysLogAttr tcsysLogAttr;
#endif

#if defined(TCSUPPORT_CT_ALGABILITY)
extern algSwitchAttr tcalgSwitchAttr;
#endif

#ifdef TCSUPPORT_IPV6
extern sysAttr tcSysAttr;
#endif

#if defined(TCSUPPORT_CT_USB_BACKUPRESTORE)
extern usbRestoreAttr tcusbRestoreAttr;
#endif
#if defined(TCSUPPORT_C1_CUC) || defined(TCSUPPORT_CUC_CWMP_PARAMETER)
extern X_CU_AccoutAttr cuAccountAttr;
extern X_CU_LogAttr cuLogAttr;
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
extern cuAttr tcCUAttr;
#endif

#if defined(TCSUPPORT_CT_MIDWARE)
extern CT_MidwareAttr ctmidwareAttr;
#endif
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
extern CT_UPNPAttr ctUPNPAttr;
extern ctcomdhcpv6Attr tcctcomdhcpv6Attr;
extern ctcomrouteradvAttr tcctcomrouteradvAttr;
extern LoopbackDetectAttr loopbackdetectAttr;
extern DataSpeedLimitAttr dataspeedlimitAttr;
#endif
#if defined(TCSUPPORT_CT_PON)
extern CT_PonLinkAttr ctPonLinkAttr;
extern CT_PonInterCfgAttr ctPonInterCfgAttr;
extern CT_PonInterCfgStatsAttr ctPonInterCfgStatsAttr;
#endif
#if defined(TCSUPPORT_CT_WAN_PTM)
extern CT_VdslLinkAttr ctVdslLinkAttr;
extern	ptmLinkAttr tcPtmLinkAttr;
#endif

#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
extern	MonitorCollectorAttr	tcMonitorCollectorAttr;
#endif

#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
extern pppoeEmulatorAttr tcPppoeEmulatorAttr;
extern pppoeEmulatorBridgeAttr tcPppoeEmulatorBridgeAttr;
#endif

#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
extern ipoeEmulatorAttr tcIpoeEmulatorAttr;
#endif
#if defined(TCSUPPORT_CT_JOYME)
extern dnsSpeedLimitAttr tcDnsSpeedLimitAttr;
#endif

/***************************************************************************************************/
						/*TR069 attribute struct definition*/
/***************************************************************************************************/

void SetCfgNodeFlag( char *cfgNode, int nodeIndex );
cfgNode_ptr SearchCfgNode( char *cfgNodeName );
void ClearCfgNodeFlag( cfgNode_ptr pCfgNode );
int delayCommitCfgNode( char *cfgNode );
int WanCommitCfgNode(cfgNode_ptr pCfgNode);
int VirserverCommitCfgNode(cfgNode_ptr pCfgNode);
int tr069CommitCfgNode(cfgNode_ptr pCfgNode);
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
int CtDiagnosticCommitNode(cfgNode_ptr pCfgNode);
#endif
extern int HandleParametersSetting( cfgNode_ptr pCfgNode, int type, int delayFlag );

void InitAttrs(void);
void ReadAttr(attr_ptr tcAttr_ptr, int *saveFlag);
void ResetAttr(attr_ptr tcAttr_ptr);
void SaveAttr(attr_ptr tcAttr_ptr);

#if defined(TCSUPPORT_CT_MIDWARE)
DeviceNodePtr AddTeleNode();
#endif

int cwmpInitParameterTree();
void cwmpFreeParameterTree(DeviceNodePtr rootPtr);
int8 getTheValue(char** value, ParameterType* type, DeviceNode*  deviceNode, char *valueString);

int8 GetTheAttributes(uint8 attribute,uint8* notification, AccessEntry** acl, DeviceNodePtr deviceNode);
int8 SetTheAttributes(uint8* attribute,
					  uint8 notificationChange, 
					  uint8 notification, 
					  uint8 accessListChange, 
					  AccessEntry* acl, 
					  DeviceNodePtr deviceNode);

int QuotationMarksExchange(char * SrcPtr, int SrcLength , char* DestPtr , int DestLength);
int RpHex2dec(char c);
uint8 Get_ACS_PVC_Number(void);
int8 cwmpGetVpiVciStr(char* pvcStr,  char* vpiStr, char* vciStr);
int CheckVPIVCI(int pvcIndex, char*vpistr, char* vcistr);
int8 cwmpGetVpiVciStrFromList(char* pvcStr,  char* vpiStr, char* vciStr, int *len);
uint8 GetVcPoolProbeAction(void);
int InternalClientIpCheck(char *st);
int SubnetLegalCheck(char *mask);
int getWANLinkIndex(DeviceNode*  deviceNode);
int getWANConnectionIndex(DeviceNode*  deviceNode);
int getWANNodeIndexForPortMapping(DeviceNode*  deviceNode);
int getPortMappingIndex(DeviceNode*  deviceNode);

int updateConfigFile( cfgNode_ptr pCfgNode );

int RouteCommitCfgNode( cfgNode_ptr pCfgNode );
#if defined(TCSUPPORT_CT_QOS)
int ctqosCommit(cfgNode_ptr pCfgNode);
int ctqos_common_commit(cfgNode_ptr pCfgNode);
#else
int queueManagementCommit(cfgNode_ptr pCfgNode);
#endif

#ifdef TR143
int tr143UDPEchoCommitFunc(cfgNode_ptr pCfgNode);
#endif

int  _checkIp(char *ipstr);
BOOLEAN ValidateChecksum(UINT PIN);
#ifdef TR143
int8 setDldUldDiagStateType(char * value);
char * getDldUldDiagStateType(void);
#endif

typedef struct RouteEntry_s
{
	char DST_IP[16];
	char GateWay[16];
	char Sub_Mask[16];
	char Device[16];
}RouteEntry_t;
int SearchRouteByDesIP(char *desIp, RouteEntry_t *re);
int checkCPEIP(char *ipaddr);
int checkPortValid( uint32 portIn );

#ifdef DHCP_PROFILE
int8 DHCPProfileCheckIP(cfgNode_ptr pCfgNode);
#endif

/* check qos type value function */
int checkMACFormat(char *mac);

#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
int8 pingCommitFunc(cfgNode_ptr pCfgNode);
#endif
#if defined(TCSUPPORT_CT_ADSLSETTING) && !defined(TCSUPPORT_CT_PON)
int8 StanddardUsedCommitFunc(cfgNode_ptr pCfgNode);
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#define NS_INT16SZ	 2
#define NS_INADDRSZ	 4
#define NS_IN6ADDRSZ	16

extern int inet_pton4(const char *src,u_char *dst);
extern int inet_pton6(const char *src,u_char *dst);
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 ADSLPARASETTING(cfgNode_ptr pCfgNode);
#endif

#endif

#if defined(TCSUPPORT_CT_MIDWARE)
int tr069EnableCommitFunc(cfgNode_ptr pCfgNode);
#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int cuappFTPCommit(cfgNode_ptr pCfgNode);
#endif

#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
int PppoeEmulatorCommit(cfgNode_ptr pCfgNode);
int PppoeEmulatorBridgeCommit(cfgNode_ptr pCfgNode);
#endif

char * achieveDeviceSummary();

int isCwmpUrlIP(char* name);

