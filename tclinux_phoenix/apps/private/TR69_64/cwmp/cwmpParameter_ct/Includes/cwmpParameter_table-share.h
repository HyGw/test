
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

#ifndef _PARAMETER_SHARE_C_
#define _PARAMETER_SHARE_C_

#define MAX_WAN_NODE 			8
#define MAX_WAN_NODEL2		8
#define MAX_PM_RULE				10


/*the attribute of TR069 Parameter: InternetGatewayDevice.ManagementServer.*/
#define CWMP_MAGIC_ATTR 	0xAABBCCDD 
typedef struct cwmpAttr
{
	uint8  aURL;
	uint8  aUsername;	
	uint8  aPassword;	
	uint8  aPeriodicInformEnable;
	uint8  aPeriodicInformInterval;	
	uint8  aPeriodicInformTime;	
	uint8  aConnectionRequestURL;
	uint8  aConnectionRequestUsername;
	uint8  aConnectionRequestPassword;
	uint8  aUpgradesManaged;
	uint8  aParameterKey;
	uint8  aKickURL;   
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	uint8  aBindInterface;
	uint8  aRMSFlooding;
	uint8  Reserved_2[2];
#endif
	
	/*  there are 11 bytes used, 
	 ** if you add other attribute,please update the stats.
	  */
	  
	uint8 Reserved[8];
	
	uint32  magic;  //This variable should be at the end of the structure. 
}cwmpAttr;

/*InternetGatewayDevice.LanDevice.1.WLANConfiguration.{i}.*/
#define WLAN_MAGIC_ATTR 	0x22335566
typedef struct wlanAttr
{
	uint8  aWLanEnable[4];	
 	uint8  aWLanMaxBitRate[4];	
 	uint8  aWLanChannel[4];
 	uint8  aWLanSSID[4];

 	uint8  aWLanBeaconType[4];
 	uint8  aWLanMACAddressControlEnabled[4];
 	uint8  aWLanWEPKeyIndex[4];	
  	uint8  aWLanKeyPassphrase[4];
	
  	uint8  aWLanBasicEncryptionModes[4];
  	uint8  aWLanBasicAuthenticationMode[4];
  	uint8  aWLanWPAEncryptionModes[4];
  	uint8  aWLanWPAAuthenticationMode[4];
	
  	uint8  aWLanBasicDataTransmitRates[4];
  	uint8  aWLanOperationalDataTransmitRates[4];
  	uint8  aWLanRadioEnabled[4];
  	uint8  aWLanAutoRateFallBackEnabled[4];
		
	uint8  aWEPKey[4];	
	uint8  aWLanPreSharedKey[10];
	uint8  aWLanPreKeyPassphrase[10];
	
	uint8  aWLanStatus[4];
	uint8  aWLanBSSID[4];
	uint8  aWLanStandard[4];
	uint8  aWLanWEPEncryptionLevel[4];

	uint8  aWLanIEEE11iEncryptionModes[4];
	uint8  aWLanIEEE11iAuthenticationMode[4];
	
	uint8  aWLanPossibleChannels[4];
	uint8  aWLanPossibleDataTransmitRates[4];
	uint8  aWLanTotalBytesSent[4];
	uint8  aWLanTotalBytesReceived[4];
	
	uint8  aWLanTotalPacketsSent[4];
	uint8  aWLanTotalPacketsReceived[4];

	uint8  aWLanTotalAssociations[4];

	uint8  aWLanAssociatedDevMACAddr[4];
	uint8  aWLanAssociatedDevIPAddr[4];
	uint8  aWLanAssociatedDevAuthState[4];

	/*wps attribute start*/
	uint8  aWPSEnable;
	uint8  aWPSDeviceName;
	uint8  aWPSDevicePassword;
	uint8  aWPSUUID;
	
	uint8  aWPSVersion;
	uint8  aWPSConfigMethodsSupported;
	uint8  aWPSConfigMethodsEnabled;
	uint8  aWPSSetupLockedState;
	
	uint8  aWPSSetupLock;
	uint8  aWPSConfigurationState;
	uint8  aWPSLastConfigurationError;
	uint8  aWPSRegistrarNumberOfEntries;
	
	uint8  aWPSRegistrarEstablished;
	uint8  aWPSRegistrarEnable[1];
	uint8  aWPSRegistrarUUID[1];
	uint8  aWPSRegistrarDeviceName[1];
	/*wps attribute end*/
	
	/*	there are 132 bytes used,
	 **	if you add other attributes, please update the stats.
	  */
#if defined(TCSUPPORT_CT_WLAN_NODE)
	uint8  aSSIDEnable[4];
	uint8  aWLanSSIDHiden[4];
	uint8  aWPSKeyWord;
	uint8  aChannelInUse;
	uint8  aPowerlevel;
	uint8  aPowerValue;
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	uint8 aRFBand[4];
	uint8 aChannelWidth[4];
	uint8 aGuardInterval[4];
	uint8 aRetryTimeout[4];
	uint8  aX_CTCOM_Mode[4];
	uint8  aX_CTCOM_VLAN[4];
	uint8 aDetectionStatus[4];
#endif

#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
	uint8 maxUserNum[4];
#endif
#if defined(TCSUPPORT_CT_PMINFORM)
	uint8 aAccessEquipmentMac[4];
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	uint8 aWLanCfgName[4];
	uint8 aWlanCfgDualBandSupport;
	uint8 aWlanCfgPowerSupported;
	uint8 aWlanCfgWMMSupported;
	uint8 aWlanCfgUAPSDSupported;
	uint8 aWlanCfgWMMEnable[4];
	uint8 aWlanCfgUAPSDEnable[4];	
#endif
#if defined(TCSUPPORT_CT_FJ)
	uint8 aWlanCfgErrorsSent[4];
	uint8 aWlanCfgErrorsReceived[4];
	uint8 aWlanCfgDiscardPacketsSent[4];
#endif
	uint8 aBeaconAdvertisementEnabled[4];
	uint8 Reserved[52];
	
	uint32 magic;
}wlanAttr;

/*InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}. WANIPConnection.{i}.*/
/*InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}. WANPPPConnection.{i}.*/
#define IPCON_MAGIC_ATTR		0xAACCDDBB
typedef struct ipConAttr
{
	uint8  aWANIPCnt1_1Enable[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANIPCnt1_1Type[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANIPCnt1_1Name[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANIPCnt1_1DefGateway[MAX_WAN_NODE*MAX_WAN_NODEL2];

	uint8  aWANIPCnt1_1SubMask[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANIPCnt1_1ExtIPAddr[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANIPCnt1_1AddrType[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANIPCnt1_1CntTrigger[MAX_WAN_NODE*MAX_WAN_NODEL2];

	uint8  aWANIPCnt1_1RProRx[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANIPCnt1_1NATEnable[MAX_WAN_NODE*MAX_WAN_NODEL2];

	uint8  aWANIPCnt1_1CntStatus[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANIPCnt1_1PsbCntTypes[MAX_WAN_NODE*MAX_WAN_NODEL2];
	
	uint8  aWANIPCnt1_1Uptime[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANIPCnt1_1LastCntError[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANIPCnt1_1RSIPAvailable[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANIPCnt1_1DNSEnabled[MAX_WAN_NODE*MAX_WAN_NODEL2];	
	
	uint8  aWANIPCnt1_1DNSOverAllow[MAX_WAN_NODE*MAX_WAN_NODEL2];	
	uint8  aWANIPCnt1_1DNSServers[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANIPCnt1_1MACAddress[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANIPCnt1_1TransportType[MAX_WAN_NODE*MAX_WAN_NODEL2];
	
	uint8  aWANIPConnectionNumOfEntries[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANPPPConnectionNumOfEntries[MAX_WAN_NODE*MAX_WAN_NODEL2];
	
	uint8  aWANIPCnt1_1EthBytesSent[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANIPCnt1_1EthBytesRcv[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANIPCnt1_1EthPktsSent[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANIPCnt1_1EthPktRcv[MAX_WAN_NODE*MAX_WAN_NODEL2];
#if defined(TCSUPPORT_CT_FJ)
	uint8  aWANIPCnt1_1EthErrRcv[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANIPCnt1_1EthDisPktRcv[MAX_WAN_NODE*MAX_WAN_NODEL2];
#endif
	uint8  aPortMappingEntryNum[MAX_WAN_NODE*MAX_WAN_NODEL2];  

	uint8  aIPPMapping1Enable[MAX_WAN_NODE*MAX_WAN_NODEL2][MAX_PM_RULE];
	uint8  aIPPMapping1Descrip[MAX_WAN_NODE*MAX_WAN_NODEL2][MAX_PM_RULE];
	uint8  aIPPMapping1LeaseDur[MAX_WAN_NODE*MAX_WAN_NODEL2][MAX_PM_RULE];
	uint8  aIP1RemoteHost[MAX_WAN_NODE*MAX_WAN_NODEL2][MAX_PM_RULE];

	uint8  aIP1ExternalPort[MAX_WAN_NODE*MAX_WAN_NODEL2][MAX_PM_RULE];
	uint8  aIP1InternalPort[MAX_WAN_NODE*MAX_WAN_NODEL2][MAX_PM_RULE];
	uint8  aIPPMapping1Protocol[MAX_WAN_NODE*MAX_WAN_NODEL2][MAX_PM_RULE];
	uint8  aIP1InternalClient[MAX_WAN_NODE*MAX_WAN_NODEL2][MAX_PM_RULE];
	uint8  aExternalPortEndRange[MAX_WAN_NODE*MAX_WAN_NODEL2][MAX_PM_RULE]; 

	uint8  aWANPPPCnt1_1PPPoESvcName[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANPPPCnt1_1Username[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANPPPCnt1_1Password[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  AuthenType[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANPPPCnt1_1PPPoEACName[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComServiceList[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComDHCPEnable[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComInterfaceBond[MAX_WAN_NODE*MAX_WAN_NODEL2];
#if defined(TCSUPPORT_CT_E8DDNS)
	uint8  aWANCnt1_CTComDDNSCfgEnable[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComDDNSProvider[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComDDNSUsername[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComDDNSPassword[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComServicePort[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComDDNSDomainName[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComDDNSHostName[MAX_WAN_NODE*MAX_WAN_NODEL2];
#endif
	/*	there are 320 bytes used.
	 **	if add other attributes,please update the stats.
	  */
	  
#if defined(TCSUPPORT_CT_PPPOEPROXY)
	uint8  aWANIPCnt1_1proxyenable[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1maxuser[MAX_WAN_NODE];
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	uint8  aWANCnt1_CTComIPMode[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTIPv6ConStatus[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTMulticastVlan[MAX_WAN_NODE*MAX_WAN_NODEL2];	
	uint8  aWANCnt1_CTComDhcpOption60Enable[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_CTComDhcpOption60Type[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_CTComDhcpOption60ValueMode[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_CTComDhcpOption60Value[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_CTComDhcpOption125Enable[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_CTComDhcpOption125Value[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_CTComDhcpOption125SubCode[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_CTComDhcpOption125SubData[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_CTComDhcpv6Option17Enable[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_CTComDhcpv6Option17Type[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_CTComDhcpv6Option17SubCode[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_CTComDhcpv6Option17Value[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_CTComDhcpv6Option16Enable[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_CTComDhcpv6Option16Value[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_CTComIPv6Alias[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComIPv6IPAddress[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComIPv6DNSIPAddress[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComIPv6Origin[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComIPv6ChildPrefixBits[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComIPv6PrefixAlias[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComIPv6Prefix[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComIPv6PrefixOrigin[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComIPv6PrefixChildPrefixBits[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComDsliteEnable[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComAftrMode[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_CTComAftr[MAX_WAN_NODE*MAX_WAN_NODEL2];	
	uint8  aX_CTCOMWANIndex;
	uint8  Reserved1[3];	
	uint8  aWANCnt1_IPForwardList[MAX_WAN_NODE*MAX_WAN_NODEL2];	
	uint8  aWANCnt1_DhcpOption125Type[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_Dhcpv6Option16Type[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_Dhcpv6Option16ValueMode[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_Dhcpv6Option17SubData[MAX_WAN_NODE*MAX_WAN_NODEL2][4];
	uint8  aWANCnt1_IPv6IPAddress[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_IPv6IPAddressAlias[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_IPAddressOrigin[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_IPv6DNSServers[MAX_WAN_NODE*MAX_WAN_NODEL2];	
	uint8  aWANCnt1_IPv6PrefixDelegation[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_IPv6PrefixAlias[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_IPv6PrefixOrigin[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_IPv6Prefix[MAX_WAN_NODE*MAX_WAN_NODEL2];	
	uint8  aWANCnt1_IPv6PrefixPltime[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_IPv6PrefixVltime[MAX_WAN_NODE*MAX_WAN_NODEL2];	
	uint8  aWANCnt1_IPv6DefaultGateway[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_IPv6DomainName[MAX_WAN_NODE*MAX_WAN_NODEL2];
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	uint8  aWANCnt1_Reset[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_VlanEnable[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_VlanID[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_Priority[MAX_WAN_NODE*MAX_WAN_NODEL2];
	uint8  aWANCnt1_IgmpProxy[MAX_WAN_NODE*MAX_WAN_NODEL2];
#endif
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
	uint8  aWANCnt1_IdleDisconnectTime[MAX_WAN_NODE*MAX_WAN_NODEL2];
#endif
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER) && !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	uint8  aWANCnt1_PPPBIEnable[MAX_WAN_NODE*MAX_WAN_NODEL2];
#endif
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
	uint8  aWANPPPCnt1_1LastCntErrorCode[MAX_WAN_NODE*MAX_WAN_NODEL2];
#endif

	uint8 Reserved[64];
		
	uint32 magic;
}ipConAttr;

#if defined(TCSUPPORT_CT_PON)
#define CT_PONLLINK_MAGIC_ATTR    0x11227796
typedef struct CT_PonLinkAttr
{
	uint8 aWANPonLinkCfg1EPonEnable[MAX_WAN_NODE];
	uint8 aWANPonLinkCfg1GPonEnable[MAX_WAN_NODE];
	uint8 aWANPonLinkCfg1Mode[MAX_WAN_NODE];
	uint8 aWANPonLinkCfg1VLANIDMark[MAX_WAN_NODE];
	uint8 aWANPonLinkCfg18021PMark[MAX_WAN_NODE];

	uint8  aWANVLANEnable[MAX_WAN_NODE];
	uint8  aWANVLAN[MAX_WAN_NODE];
	uint8  aWANVLAN8021p[MAX_WAN_NODE];

	uint32  magic;  
}CT_PonLinkAttr;
#endif

#if defined(TCSUPPORT_CT_WAN_PTM)
#define CT_VDSLLINK_MAGIC_ATTR    0x11227797
typedef struct CT_VdslLinkAttr
{
	uint8 aWANVdslLinkCfg1Enable[MAX_WAN_NODE];
	uint8 aWANVdslLinkCfg1Mode[MAX_WAN_NODE];
	uint8 aWANVdslLinkCfg1VLANIDMark[MAX_WAN_NODE];
	uint8 aWANVdslLinkCfg18021PMark[MAX_WAN_NODE];

	uint32  magic;  
}CT_VdslLinkAttr;

#define PTMLink_MAGIC_ATTR    0x11227798
typedef struct ptmLinkAttr
{
	uint8  aWANPTMLinkCfgEnable[MAX_WAN_NODE];
	uint8  aWANPTMLinkCfgLinkStatus[MAX_WAN_NODE];
	uint8  aWANPTMLinkCfgMACAddress[MAX_WAN_NODE];
	uint8  aWANPTMLinkCfgBytesSent[MAX_WAN_NODE];

	uint8  aWANPTMLinkCfgBytesReceived[MAX_WAN_NODE];
	uint8  aWANPTMLinkCfgFramesSent[MAX_WAN_NODE];
	uint8  aWANPTMLinkCfgFramesReceived[MAX_WAN_NODE];
	uint8  aWANPTMLinkCfgOOSNearEnd[MAX_WAN_NODE];

	uint8  aWANPTMLinkCfgOOSFarEnd[MAX_WAN_NODE];
	uint8  aWANPTMLinkCfgErrorsSend[MAX_WAN_NODE];
	uint8  aWANPTMLinkCfgErrorsReceived[MAX_WAN_NODE];
	uint8  aWANPTMLinkCfgUnicastPacketsSent[MAX_WAN_NODE];

	uint8  aWANPTMLinkCfgUnicastPacketsReceived[MAX_WAN_NODE];
	uint8  aWANPTMLinkCfgDiscardPacketsSent[MAX_WAN_NODE];
	uint8  aWANPTMLinkCfgDiscardPacketsReceived[MAX_WAN_NODE];
	uint8  aWANPTMLinkCfgMulticastPacketsSent[MAX_WAN_NODE];

	uint8  aWANPTMLinkCfgMulticastPacketsReceived[MAX_WAN_NODE];
	uint8  aWANPTMLinkCfgBroadcastPacketsSent[MAX_WAN_NODE];
	uint8  aWANPTMLinkCfgBroadcastPacketsReceived[MAX_WAN_NODE];
	uint8  aWANPTMLinkCfgUnknownProtoPacketsReceived[MAX_WAN_NODE];
	/*	there are 40 bytes used,
	 **  if you add other attribute,please update the stats.
	  */	  
	uint8 Reserved[20];
	
	uint32  magic;  //This variable should be at the end of the structure. 
}ptmLinkAttr;

#endif


#endif

