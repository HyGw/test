/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2005, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attempt
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    rtmp_info.c

    Abstract:
    IOCTL related subroutines

    Revision History:
    Who         When          What
    --------    ----------    ----------------------------------------------
    Rory Chen   01-03-2003    created
	Rory Chen   02-14-2005    modify to support RT61
*/

#include	"rt_config.h"


/*
** Functions prototype
*/
#ifdef APCLI_SUPPORT
INT Set_ApCli_Enable_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
INT Set_ApCli_Ssid_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
INT Set_ApCli_Bssid_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
INT Set_ApCli_WPAPSK_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
/*KH:Adding for isolating the security of AP Client from Main Bssid*/
INT Set_ApCli_AuthMode_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
INT Set_ApCli_EncrypType_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
INT Set_ApCli_DefaultKeyID_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
INT Set_ApCli_Key1_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
INT Set_ApCli_Key2_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
INT Set_ApCli_Key3_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
INT Set_ApCli_Key4_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
// For AP Client support WPS Modification
#ifdef WSC_SUPPORT
INT Set_WscSsid_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
#endif // WSC_SUPPORT //
// --
#endif // APCLI_SUPPORT //

#ifdef CARRIER_DETECTION_SUPPORT
static INT Set_CarrierDetect_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
#endif // CARRIER_DETECTION_SUPPORT //

static COUNTRY_CODE_TO_COUNTRY_REGION allCountry[] = {
	/* {Country Number, ISO Name, Country Name, Support 11A, 11A Country Region, Support 11G, 11G Country Region} */
	{0,	  "DB",	"Debug",	 			TRUE,  A_BAND_REGION_7,	TRUE,	G_BAND_REGION_5},
	{8,   "AL", "ALBANIA",              FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{12,  "DZ", "ALGERIA",              FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{32,  "AR", "ARGENTINA",            TRUE,  A_BAND_REGION_3, TRUE,	G_BAND_REGION_1},
	{51,  "AM", "ARMENIA",              TRUE,  A_BAND_REGION_2, TRUE,	G_BAND_REGION_1},
	{36,  "AU", "AUSTRALIA",            TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{40,  "AT", "AUSTRIA",              TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{31,  "AZ", "AZERBAIJAN",           TRUE,  A_BAND_REGION_2, TRUE,	G_BAND_REGION_1},
	{48,  "BH", "BAHRAIN",              TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{112, "BY", "BELARUS",              FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{56,  "BE", "BELGIUM",              TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{84,  "BZ", "BELIZE",               TRUE,  A_BAND_REGION_4, TRUE,	G_BAND_REGION_1},
	{68,  "BO", "BOLIVIA",              TRUE,  A_BAND_REGION_4, TRUE,	G_BAND_REGION_1},
	{76,  "BR", "BRAZIL",               TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{96,  "BN", "BRUNEI DARUSSALAM",    TRUE,  A_BAND_REGION_4, TRUE,	G_BAND_REGION_1},
	{100, "BG", "BULGARIA",             TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{124, "CA", "CANADA",               TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_0},
	{152, "CL", "CHILE",                TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{156, "CN", "CHINA",                TRUE,  A_BAND_REGION_4, TRUE,	G_BAND_REGION_1},
	{170, "CO", "COLOMBIA",             TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_0},
	{188, "CR", "COSTA RICA",           FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{191, "HR", "CROATIA",              TRUE,  A_BAND_REGION_2, TRUE,	G_BAND_REGION_1},
	{196, "CY", "CYPRUS",               TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{203, "CZ", "CZECH REPUBLIC",       TRUE,  A_BAND_REGION_2, TRUE,	G_BAND_REGION_1},
	{208, "DK", "DENMARK",              TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{214, "DO", "DOMINICAN REPUBLIC",   TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_0},
	{218, "EC", "ECUADOR",              FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{818, "EG", "EGYPT",                TRUE,  A_BAND_REGION_2, TRUE,	G_BAND_REGION_1},
	{222, "SV", "EL SALVADOR",          FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{233, "EE", "ESTONIA",              TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{246, "FI", "FINLAND",              TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{250, "FR", "FRANCE",               TRUE,  A_BAND_REGION_2, TRUE,	G_BAND_REGION_1},
	{268, "GE", "GEORGIA",              TRUE,  A_BAND_REGION_2, TRUE,	G_BAND_REGION_1},
	{276, "DE", "GERMANY",              TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{300, "GR", "GREECE",               TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{320, "GT", "GUATEMALA",            TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_0},
	{340, "HN", "HONDURAS",             FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{344, "HK", "HONG KONG",            TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{348, "HU", "HUNGARY",              TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{352, "IS", "ICELAND",              TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{356, "IN", "INDIA",                TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{360, "ID", "INDONESIA",            TRUE,  A_BAND_REGION_4, TRUE,	G_BAND_REGION_1},
	{364, "IR", "IRAN",                 TRUE,  A_BAND_REGION_4, TRUE,	G_BAND_REGION_1},
	{372, "IE", "IRELAND",              TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{376, "IL", "ISRAEL",               FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{380, "IT", "ITALY",                TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{392, "JP", "JAPAN",                TRUE,  A_BAND_REGION_6, TRUE,	G_BAND_REGION_1},
	{400, "JO", "JORDAN",               TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{398, "KZ", "KAZAKHSTAN",           FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{408, "KP", "KOREA",                TRUE,  A_BAND_REGION_5, TRUE,	G_BAND_REGION_1},
	{410, "KR", "KOREA REPUBLIC OF",    TRUE,  A_BAND_REGION_5, TRUE,	G_BAND_REGION_1},
	{414, "KW", "KUWAIT",               FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{428, "LV", "LATVIA",               TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{422, "LB", "LEBANON",              FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{438, "LI", "LIECHTENSTEIN",        TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{440, "LT", "LITHUANIA",            TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{442, "LU", "LUXEMBOURG",           TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{446, "MO", "MACAU",                TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{807, "MK", "MACEDONIA",            FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{458, "MY", "MALAYSIA",             TRUE,  A_BAND_REGION_3, TRUE,	G_BAND_REGION_1},
	{484, "MX", "MEXICO",               TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_0},
	{492, "MC", "MONACO",               TRUE,  A_BAND_REGION_2, TRUE,	G_BAND_REGION_1},
	{504, "MA", "MOROCCO",              FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{528, "NL", "NETHERLANDS",          TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{554, "NZ", "NEW ZEALAND",          TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{578, "NO", "NORWAY",               TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_0},
	{512, "OM", "OMAN",                 TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{586, "PK", "PAKISTAN",             FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{591, "PA", "PANAMA",               TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_0},
	{604, "PE", "PERU",                 TRUE,  A_BAND_REGION_4, TRUE,	G_BAND_REGION_1},
	{608, "PH", "PHILIPPINES",          TRUE,  A_BAND_REGION_4, TRUE,	G_BAND_REGION_1},
	{616, "PL", "POLAND",               TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{620, "PT", "PORTUGAL",             TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{630, "PR", "PUERTO RICO",          TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_0},
	{634, "QA", "QATAR",                FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{642, "RO", "ROMANIA",              FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{643, "RU", "RUSSIAN FEDERATION",   FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{682, "SA", "SAUDI ARABIA",         FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{702, "SG", "SINGAPORE",            TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{703, "SK", "SLOVAKIA",             TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{705, "SI", "SLOVENIA",             TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{710, "ZA", "SOUTH AFRICA",         TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{724, "ES", "SPAIN",                TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{752, "SE", "SWEDEN",               TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{756, "CH", "SWITZERLAND",          TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{760, "SY", "SYRIAN ARAB REPUBLIC", FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{158, "TW", "TAIWAN",               TRUE,  A_BAND_REGION_3, TRUE,	G_BAND_REGION_0},
	{764, "TH", "THAILAND",             FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{780, "TT", "TRINIDAD AND TOBAGO",  TRUE,  A_BAND_REGION_2, TRUE,	G_BAND_REGION_1},
	{788, "TN", "TUNISIA",              TRUE,  A_BAND_REGION_2, TRUE,	G_BAND_REGION_1},
	{792, "TR", "TURKEY",               TRUE,  A_BAND_REGION_2, TRUE,	G_BAND_REGION_1},
	{804, "UA", "UKRAINE",              FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{784, "AE", "UNITED ARAB EMIRATES", FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{826, "GB", "UNITED KINGDOM",       TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_1},
	{840, "US", "UNITED STATES",        TRUE,  A_BAND_REGION_0, TRUE,	G_BAND_REGION_0},
	{858, "UY", "URUGUAY",              TRUE,  A_BAND_REGION_5, TRUE,	G_BAND_REGION_1},
	{860, "UZ", "UZBEKISTAN",           TRUE,  A_BAND_REGION_1, TRUE,	G_BAND_REGION_0},
	{862, "VE", "VENEZUELA",            TRUE,  A_BAND_REGION_5, TRUE,	G_BAND_REGION_1},
	{704, "VN", "VIET NAM",             FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{887, "YE", "YEMEN",                FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{716, "ZW", "ZIMBABWE",             FALSE, A_BAND_REGION_0, TRUE,	G_BAND_REGION_1},
	{999,	"",	"",	0,	0,	0,	0}
};
//added by xyzhu_nj_20091208
extern int gMacTabChanged;

#define NUM_OF_COUNTRIES	(sizeof(allCountry)/sizeof(COUNTRY_CODE_TO_COUNTRY_REGION))

#ifdef IGMP_SNOOP_SUPPORT
extern INT Set_IgmpSn_Enable_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
extern INT Set_IgmpSn_AddEntry_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
extern INT Set_IgmpSn_DelEntry_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
extern INT Set_IgmpSn_TabDisplay_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
#endif // IGMP_SNOOP_SUPPORT //

struct iw_priv_args privtab[] = {
{ RTPRIV_IOCTL_SET, 
  IW_PRIV_TYPE_CHAR | 1024, 0,
  "set"},  
{ RTPRIV_IOCTL_GSITESURVEY,
  IW_PRIV_TYPE_CHAR | 1024, IW_PRIV_TYPE_CHAR | 1024 ,
  "get_site_survey"}, 
{ RTPRIV_IOCTL_GET_MAC_TABLE,
  IW_PRIV_TYPE_CHAR | 1024, IW_PRIV_TYPE_CHAR | 1024 ,
  "get_mac_table"}, 
#ifdef DBG
{ RTPRIV_IOCTL_BBP,
  IW_PRIV_TYPE_CHAR | 1024, IW_PRIV_TYPE_CHAR | 1024,
  "bbp"},
{ RTPRIV_IOCTL_MAC,
  IW_PRIV_TYPE_CHAR | 1024, IW_PRIV_TYPE_CHAR | 1024,
  "mac"},
{ RTPRIV_IOCTL_E2P,
  IW_PRIV_TYPE_CHAR | 1024, IW_PRIV_TYPE_CHAR | 1024,
  "e2p"},
{ RTPRIV_IOCTL_CHK_IC_VERSION,
  IW_PRIV_TYPE_CHAR | 1024, IW_PRIV_TYPE_CHAR | 1024,
  "chk_asic_ver"},  
#endif
#ifdef WSC_SUPPORT
{ RTPRIV_IOCTL_WSC_PROFILE,
  IW_PRIV_TYPE_CHAR | 1024, IW_PRIV_TYPE_CHAR | 1024 ,
  "get_wsc_profile"},
#endif // WSC_SUPPORT //
{ RTPRIV_IOCTL_STATISTICS,
  IW_PRIV_TYPE_CHAR | 1024, IW_PRIV_TYPE_CHAR | 1024,
  "stat"}
};

static struct {
	CHAR *name;
	INT (*set_proc)(PRTMP_ADAPTER pAdapter, PUCHAR arg);
} *PRTMP_PRIVATE_SET_PROC, RTMP_PRIVATE_SUPPORT_PROC[] = {
	{"DriverVersion",				Set_DriverVersion_Proc},
	{"CountryRegion",				Set_CountryRegion_Proc},
	{"CountryRegionABand",			Set_CountryRegionABand_Proc},
	{"CountryString",				Set_CountryString_Proc},
	{"CountryCode",					Set_CountryCode_Proc},
	{"SSID",						Set_SSID_Proc},
	{"WirelessMode",				Set_WirelessMode_Proc},
	{"TxRate",						Set_TxRate_Proc},
	{"BasicRate",					Set_BasicRate_Proc},
	{"Channel",						Set_Channel_Proc},
	{"BeaconPeriod",				Set_BeaconPeriod_Proc},
	{"DtimPeriod",					Set_DtimPeriod_Proc},
	{"TxPower",						Set_TxPower_Proc},
	{"BGProtection",				Set_BGProtection_Proc},
	{"DisableOLBC", 				Set_OLBCDetection_Proc},
	{"TxAntenna",					Set_TxAntenna_Proc},
	{"RxAntenna",					Set_RxAntenna_Proc},
	{"TxPreamble",					Set_TxPreamble_Proc},
	{"RTSThreshold",				Set_RTSThreshold_Proc},
	{"FragThreshold",				Set_FragThreshold_Proc},
	{"TxBurst",						Set_TxBurst_Proc},
#ifdef AGGREGATION_SUPPORT
	{"PktAggregate",				Set_PktAggregate_Proc},
#endif
	{"TurboRate",					Set_TurboRate_Proc},
	{"NoForwardingBTNBSSID",		Set_NoForwardingBTNSSID_Proc},
#ifdef WMM_SUPPORT
	{"WmmCapable",					Set_WmmCapable_Proc},
#endif
	{"NoForwarding",				Set_NoForwarding_Proc},
	{"HideSSID",					Set_HideSSID_Proc},
	{"ShortSlot",					Set_ShortSlot_Proc},
	{"IEEE8021X",					Set_IEEE8021X_Proc},
	{"IEEE80211H",					Set_IEEE80211H_Proc},
	{"RDDurRegion",					Set_RDDurRegion_Proc},	
	{"CSPeriod",					Set_CSPeriod_Proc},
	{"PreAuth",						Set_PreAuth_Proc},
	{"VLANID",						Set_VLANID_Proc},
	{"VLANPriority",				Set_VLANPriority_Proc},
	{"AuthMode",					Set_AuthMode_Proc},
	{"EncrypType",					Set_EncrypType_Proc},
	{"DefaultKeyID",				Set_DefaultKeyID_Proc},
	{"Key1",						Set_Key1_Proc},
	{"Key2",						Set_Key2_Proc},
	{"Key3",						Set_Key3_Proc},
	{"Key4",						Set_Key4_Proc},
	{"HSCounter",					Set_HSCounter_Proc},
	{"AccessPolicy",				Set_AccessPolicy_Proc},
	{"AccessControlList",			Set_AccessControlList_Proc},
	{"WPAPSK",						Set_WPAPSK_Proc},
	{"ResetCounter",				Set_ResetStatCounter_Proc},
	{"RadioOn",						Set_RadioOn_Proc},
	{"dfstest",						Set_dfstest_Proc},
	{"RekeyMethod",					Set_RekeyMethod_Proc},
	{"RekeyInterval",				Set_RekeyInterval_Proc},
	{"PMKCachePeriod",				Set_PMKCachePeriod_Proc},
	{"SiteSurvey",					Set_SiteSurvey_Proc},
	{"RetryLimit",					Set_RetryLimit_Proc},
	{"TxQueueSize",					Set_TxQueueSize_Proc},
#ifdef DBG	
	{"Debug",						Set_Debug_Proc},
#endif
#ifdef RALINK_ATE
	{"ATE",							Set_ATE_Proc},
	{"ATEDA",						Set_ATE_DA_Proc},
	{"ATESA",						Set_ATE_SA_Proc},
	{"ATEBSSID",					Set_ATE_BSSID_Proc},
	{"ATECHANNEL",					Set_ATE_CHANNEL_Proc},
	{"ATETXPOW",					Set_ATE_TX_POWER_Proc},
	{"ATETXFREQOFFSET",				Set_ATE_TX_FREQOFFSET_Proc},
	{"ATETXLEN",					Set_ATE_TX_LENGTH_Proc},
	{"ATETXCNT",					Set_ATE_TX_COUNT_Proc},
	{"ATETXRATE",					Set_ATE_TX_RATE_Proc},
	{"ATERXFER",					Set_ATE_RX_FER_Proc},
#endif	// RALINK_ATE
#ifdef APCLI_SUPPORT
	{"ApCliEnable",					Set_ApCli_Enable_Proc},
	{"ApCliSsid",					Set_ApCli_Ssid_Proc},
	{"ApCliBssid",					Set_ApCli_Bssid_Proc},
	{"ApCliWPAPSK",					Set_ApCli_WPAPSK_Proc},
//KH:Adding for isolating the security of AP Client from Main Bssid
	{"ApCliAuthMode",				Set_ApCli_AuthMode_Proc},
	{"ApCliEncrypType",				Set_ApCli_EncrypType_Proc},
	{"ApCliDefaultKeyID",			Set_ApCli_DefaultKeyID_Proc},
	{"ApCliKey1",					Set_ApCli_Key1_Proc},
	{"ApCliKey2",					Set_ApCli_Key2_Proc},
	{"ApCliKey3",					Set_ApCli_Key3_Proc},
	{"ApCliKey4",					Set_ApCli_Key4_Proc},
// For AP Client support WPS Modification
#ifdef WSC_SUPPORT
	{"ApCliWscSsid",				Set_WscSsid_Proc},
#endif // WSC_SUPPORT //
// --
#endif
#ifdef IGMP_SNOOP_SUPPORT
	{"IgmpSnEnable",				Set_IgmpSn_Enable_Proc},
	{"IgmpAdd",						Set_IgmpSn_AddEntry_Proc},
	{"IgmpDel",						Set_IgmpSn_DelEntry_Proc},
	{"IgmpTabShow",					Set_IgmpSn_TabDisplay_Proc},
#endif // IGMP_SNOOP_SUPPORT //
#ifdef CARRIER_DETECTION_SUPPORT
	{"CarrierDetect",				Set_CarrierDetect_Proc},
#endif // CARRIER_DETECTION_SUPPORT //
#ifdef WSC_SUPPORT
	{"WscConfMode",					Set_WscConfMode_Proc},
	{"WscConfStatus",				Set_WscConfStatus_Proc},
	{"WscMode",						Set_WscMode_Proc},
	{"WscStatus",					Set_WscStatus_Proc},
	{"WscGetConf",					Set_WscGetConf_Proc},
	{"WscPinCode",					Set_WscPinCode_Proc},
    {"WscOOB",                      Set_WscOOB_Proc},
    {"WscOpenOOB",                  Set_WscOpenOOB_Proc},
// For AP Client support WPS Modification
    {"WscStop",                     Set_WscStop_Proc},
// --
#ifdef DBG
    {"WscTestWithIntelSDK",         Set_Test_With_Intel_SDK_Proc},
#endif
#endif // WSC_SUPPORT //
	{NULL,}
};

INT RTMPSetInformation(
	IN	PRTMP_ADAPTER pAd,
	IN	OUT	struct ifreq	*rq,
	IN	INT					cmd)
{
	struct iwreq						*wrq = (struct iwreq *) rq;
	UCHAR						        Addr[MAC_ADDR_LEN];
	INT									Status = NDIS_STATUS_SUCCESS;
#ifdef SNMP	
	//snmp
	TXRX_CSR4_STRUC				csr4;
	ULONG						ShortRetryLimit, LongRetryLimit;
	USHORT						stmp;
	UCHAR						ctmp;
	NDIS_802_11_KEY				*pKey=NULL;
	ULONG						KeyIdx;
#endif //snmp	
	switch(cmd & 0x7FFF)
    {
		case RT_SET_IAPP_PID:
    		if (copy_from_user(&pAd->IappPid, wrq->u.data.pointer, wrq->u.data.length))
			{
				Status = -EFAULT; 	
			}
    		else
    		{
    			DBGPRINT(RT_DEBUG_TRACE, "RT_SET_IAPP_PID::(IappPid=%ld)\n", pAd->IappPid);
    		}
			break;

    	case RT_SET_APD_PID:
    		if (copy_from_user(&pAd->ApdPid, wrq->u.data.pointer, wrq->u.data.length))
			{
				Status = -EFAULT; 	
			}
    		else
    		{
    			DBGPRINT(RT_DEBUG_TRACE, "RT_SET_APD_PID::(ApdPid=%ld)\n", pAd->ApdPid);
    		}
			break;

		case RT_SET_DEL_MAC_ENTRY:
    		if (copy_from_user(Addr, wrq->u.data.pointer, wrq->u.data.length))
			{
				Status = -EFAULT; 	
			}
    		else
    		{
    			DBGPRINT(RT_DEBUG_TRACE, "RT_SET_DEL_MAC_ENTRY::(%02x:%02x:%02x:%02x:%02x:%02x)\n", Addr[0],Addr[1],Addr[2],Addr[3],Addr[4],Addr[5]);
    			MacTableDeleteEntry(pAd, Addr);
    		}
			break;
#ifdef SNMP
		case OID_802_11_SHORTRETRYLIMIT:
			if (wrq->u.data.length != sizeof(ULONG))
				Status = -EINVAL;
			else
			{
				copy_from_user(&ShortRetryLimit, wrq->u.data.pointer, wrq->u.data.length);
				RTMP_IO_READ32(pAd, TXRX_CSR4, &csr4.word);
				csr4.field.ShortRetryLimit = ShortRetryLimit;
				RTMP_IO_WRITE32(pAd, TXRX_CSR4, csr4.word);
				DBGPRINT(RT_DEBUG_TRACE, "Set::OID_802_11_SHORTRETRYLIMIT (csr4.field.ShortRetryLimit=%ld, ShortRetryLimit=%ld)\n", csr4.field.ShortRetryLimit, ShortRetryLimit);
			}
			break;

		case OID_802_11_LONGRETRYLIMIT:
			DBGPRINT(RT_DEBUG_TRACE, "Set::OID_802_11_LONGRETRYLIMIT \n");
			if (wrq->u.data.length != sizeof(ULONG))
				Status = -EINVAL;
			else
			{
				copy_from_user(&LongRetryLimit, wrq->u.data.pointer, wrq->u.data.length);
				RTMP_IO_READ32(pAd, TXRX_CSR4, &csr4.word);
				csr4.field.LongRetryLimit = LongRetryLimit;
				RTMP_IO_WRITE32(pAd, TXRX_CSR4, csr4.word);
				DBGPRINT(RT_DEBUG_TRACE, "Set::OID_802_11_LONGRETRYLIMIT (csr4.field.LongRetryLimit= %ld,LongRetryLimit=%ld)\n", csr4.field.LongRetryLimit, LongRetryLimit);
			}
			break;

		case OID_802_11_WEPDEFAULTKEYVALUE:
			DBGPRINT(RT_DEBUG_TRACE, "Set::OID_802_11_WEPDEFAULTKEYVALUE\n");
			pKey = kmalloc(wrq->u.data.length, GFP_KERNEL);
			copy_from_user(pKey, wrq->u.data.pointer, wrq->u.data.length);
			//pKey = &WepKey;
			
			if ( pKey->Length != wrq->u.data.length)
			{
				Status = -EINVAL;
				DBGPRINT(RT_DEBUG_TRACE, "Set::OID_802_11_WEPDEFAULTKEYVALUE, Failed!!\n");
			}
			KeyIdx = pKey->KeyIndex & 0x0fffffff;
			DBGPRINT(RT_DEBUG_TRACE,"pKey->KeyIndex =%d, pKey->KeyLength=%d\n", pKey->KeyIndex, pKey->KeyLength);

			// it is a shared key
			if (KeyIdx > 4)
				Status = -EINVAL;
			else
			{
				pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[KeyIdx].KeyLen = (UCHAR) pKey->KeyLength;
				NdisMoveMemory(pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[KeyIdx].Key, &pKey->KeyMaterial, pKey->KeyLength);
				if (pKey->KeyIndex & 0x80000000)
				{
					// Default key for tx (shared key)
					pAd->PortCfg.MBSSID[pAd->IoctlIF].DefaultKeyId = (UCHAR) KeyIdx;
				}
				//RestartAPIsRequired = TRUE;
			}
			break;

		case OID_802_11_WEPDEFAULTKEYID:
			DBGPRINT(RT_DEBUG_TRACE, "Set::OID_802_11_WEPDEFAULTKEYID \n");

			if (wrq->u.data.length != sizeof(UCHAR))
				Status = -EINVAL;
			else
				copy_from_user(&pAd->PortCfg.MBSSID[pAd->IoctlIF].DefaultKeyId, wrq->u.data.pointer, wrq->u.data.length);

			break;

		case OID_802_11_RTS_THRESHOLD:
			DBGPRINT(RT_DEBUG_TRACE, "Set:OID_802_11_RTS_THRESHOLD \n");

			if (wrq->u.data.length != sizeof(USHORT))
				Status = -EINVAL;
			else
				copy_from_user(&pAd->PortCfg.RtsThreshold, wrq->u.data.pointer, wrq->u.data.length);
			
			break;

		case OID_802_11_FRAGMENTATION_THRESHOLD:
			DBGPRINT(RT_DEBUG_TRACE, "Set::OID_802_11_FRAGMENTATION_THRESHOLD \n");
			if (wrq->u.data.length != sizeof(USHORT))
				Status = -EINVAL;
			else
				copy_from_user(&pAd->PortCfg.FragmentThreshold, wrq->u.data.pointer, wrq->u.data.length);

			break;

		case OID_802_11_TX_ANTENNA_SELECTED:
			DBGPRINT(RT_DEBUG_TRACE, "Set::OID_802_11_TX_ANTENNA_SELECTED \n");
			if (wrq->u.data.length != sizeof(USHORT))
				Status = -EINVAL;
			else
			{
				copy_from_user(&stmp, wrq->u.data.pointer, wrq->u.data.length);
				sprintf(&ctmp,"%d", stmp);
				Set_TxAntenna_Proc(pAd, &ctmp);
				//pAd->Antenna.field.TxDefaultAntenna = stmp;
			}
		
			break;

		case OID_802_11_RX_ANTENNA_SELECTED:
			DBGPRINT(RT_DEBUG_TRACE, "Set::OID_802_11_RX_ANTENNA_SELECTED \n");
			if (wrq->u.data.length != sizeof(USHORT))
				Status = -EINVAL;
			else
			{
				copy_from_user(&stmp, wrq->u.data.pointer, wrq->u.data.length);
				sprintf(&ctmp,"%d", stmp);
				Set_RxAntenna_Proc(pAd, &ctmp);
				//pAd->Antenna.field.RxDefaultAntenna = stmp;
			}

			break;
		
		case OID_802_11_CURRENTCHANNEL:
			DBGPRINT(RT_DEBUG_TRACE, "Set::OID_802_11_CURRENTCHANNEL \n");
			if (wrq->u.data.length != sizeof(UCHAR))
				Status = -EINVAL;
			else
			{
				copy_from_user(&ctmp, wrq->u.data.pointer, wrq->u.data.length);
				sprintf(&ctmp,"%d", ctmp);
				Set_Channel_Proc(pAd, &ctmp);
			}
			break;
#endif
#ifdef WSC_SUPPORT
		case RT_OID_WSC_SET_SELECTED_REGISTRAR:
			{	
				PUCHAR upnpInfo;
				
				DBGPRINT(RT_DEBUG_TRACE, "WSC::RT_OID_WSC_SET_SELECTED_REGISTRAR, wrq->u.data.length=%d!\n", wrq->u.data.length);
				upnpInfo = kmalloc(wrq->u.data.length, GFP_KERNEL);
				if(upnpInfo)
				{
					int len;
					
					copy_from_user(upnpInfo, wrq->u.data.pointer, wrq->u.data.length);
					len = wrq->u.data.length;
					
//					if(pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.WscConfMode == WSC_AP_PROXY)
					if((pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.WscConfMode & WSC_AP_PROXY) != 0)  // edit by johnli
					{
						WscSelectedRegistrar(pAd, upnpInfo, len);
						kfree(upnpInfo);

					if (pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.Wsc2MinsTimerRunning == TRUE)
					{
						RTMPCancelTimer(&pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.Wsc2MinsTimer);
					}
	    			// 2mins time-out timer
    				RTMPAddTimer(&pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.Wsc2MinsTimer, WSC_TWO_MINS_TIME_OUT);
    				pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.Wsc2MinsTimerRunning = TRUE;
					}
				} else {
					Status = -EINVAL;
				}
			}
			break;
		case RT_OID_WSC_EAPMSG:
			{
				RTMP_WSC_U2KMSG_HDR *msgHdr = NULL;
				PUCHAR pUPnPMsg = NULL;
				//PWSC_CTRL pWscCtrl = NULL;
				ULONG msgLen = 0, Machine = 0, msgType = 0;
				int retVal, senderID = 0;

				DBGPRINT(RT_DEBUG_TRACE, "WSC::RT_OID_WSC_EAPMSG, wrq->u.data.length=%d!\n", wrq->u.data.length);
			
				msgLen = wrq->u.data.length;				
				if((pUPnPMsg = kmalloc(msgLen, GFP_KERNEL)) == NULL)
					Status = -EINVAL;
				else
				{
					memset(pUPnPMsg, 0, msgLen);
					copy_from_user(pUPnPMsg, wrq->u.data.pointer, msgLen);
					
					msgHdr = (RTMP_WSC_U2KMSG_HDR *)pUPnPMsg;
					//pWscCtrl = &pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl;
					senderID = *((int *)&msgHdr->Addr2);
					//DBGPRINT(RT_DEBUG_TRACE, "WscConfMode=%d!, WscState=%d, senderID=0x%x!\n", pWscCtrl->WscConfMode, pWscCtrl->WscState, senderID);
					//assign the STATE_MACHINE type
/*
					if (((pWscCtrl->WscConfMode == WSC_AP_Enrollee) && (pWscCtrl->WscUseUPnP)) ||
						((pWscCtrl->WscConfMode == WSC_AP_PROXY) && (pWscCtrl->WscState >= WSC_STATE_WAIT_EAPOL_START)) ||
						((pWscCtrl->WscConfMode == WSC_AP_REGISTRAR) && (pWscCtrl->WscUseUPnP)))
*/
					{
#if 0					
						switch (pWscCtrl->WscConfMode)
						{
							case WSC_AP_Enrollee:
								Machine = WSC_UNCONF_AP_STATE_MACHINE; break;
							case WSC_AP_PROXY:
								Machine = WSC_AP_PROXY_STATE_MACHINE; break;
							case WSC_AP_REGISTRAR:
								Machine = WSC_AP_REG_STATE_MACHINE; break;
						}
#else					
                        Machine = WSC_AP_STATE_MACHINE;
#endif
						msgType = WSC_EAPOL_UPNP_MSG;
						retVal = MlmeEnqueueForWsc(pAd, msgHdr->envID, senderID, Machine, msgType, msgLen, pUPnPMsg);
						if((retVal == FALSE) && (msgHdr->envID != 0))
						{
							DBGPRINT(RT_DEBUG_TRACE, "MlmeEnqueuForWsc return False and envID=0x%x!\n", msgHdr->envID);
							Status = -EINVAL;
						}
					}

					kfree(pUPnPMsg);
				}
				DBGPRINT(RT_DEBUG_TRACE, "RT_OID_WSC_EAPMSG finished!\n");
			}
			break;
#endif // WSC_SUPPORT //
   		default:
			DBGPRINT(RT_DEBUG_TRACE, "Set::unknown IOCTL's subcmd = 0x%08x\n", cmd);
			Status = -EOPNOTSUPP;
			break;
    }
	
	return Status;
}

INT RTMPQueryInformation(
	IN	PRTMP_ADAPTER       pAd,
	IN	OUT	struct ifreq    *rq,
	IN	INT                 cmd)
{
	struct iwreq						*wrq = (struct iwreq *) rq;
    INT	Status = NDIS_STATUS_SUCCESS;
#ifdef SNMP	
	//for snmp, kathy
	UCHAR						DefaultKeyId;
	UCHAR						KeyValue[16];
	DefaultKeyIdxValue			KeyIdxValue, *pKeyIdxValue;
	INT							valueLen;
	TXRX_CSR4_STRUC				csr4;
	ULONG						ShortRetryLimit, LongRetryLimit, ulInfo = 0;
	UCHAR						tmp[64];
	NDIS_802_11_STATISTICS		Statistics;
#endif //SNMP
#ifdef WSC_SUPPORT
	ULONG						WscPinCode = 0;
//	UCHAR						uuidStr[17];
#endif // WSC_SUPPORT //

    switch(cmd)
    {
#ifdef RALINK_ATE
		case RT_QUERY_ATE_TXDONE_COUNT:
			DBGPRINT(RT_DEBUG_TRACE, "Query::RT_QUERY_ATE_TXDONE_COUNT \n");
			wrq->u.data.length = sizeof(ULONG);
			if (copy_to_user(wrq->u.data.pointer, &pAd->ate.TxDoneCount, wrq->u.data.length))
			{
				Status = -EFAULT; 	
			}
			break;
#endif

		case RT_QUERY_SIGNAL_CONTEXT:
			DBGPRINT(RT_DEBUG_TRACE, "Query::RT_QUERY_SIGNAL_CONTEXT \n");
			wrq->u.data.length = sizeof(RT_SIGNAL_STRUC);
			if (copy_to_user(wrq->u.data.pointer, &pAd->RTSignal, wrq->u.data.length))
			{
				Status = -EFAULT;
			}
			break;
#ifdef SNMP
		case OID_802_3_CURRENT_ADDRESS:
			DBGPRINT(RT_DEBUG_TRACE, "Query::OID_802_3_CURRENT_ADDRESS\n");
			wrq->u.data.length = MAC_ADDR_LEN;
			Status = copy_to_user(wrq->u.data.pointer, &pAd->CurrentAddress, wrq->u.data.length);
			DBGPRINT(RT_DEBUG_TRACE, "Status = %d, Mac len= %d, data=%s \n", Status, wrq->u.data.length, &pAd->CurrentAddress);
			break;

		case RT_OID_802_11_MAC_ADDRESS:
			DBGPRINT(RT_DEBUG_TRACE, "Query::RT_OID_802_11_MACADDRESS \n");
			wrq->u.data.length = MAC_ADDR_LEN;
			Status = copy_to_user(wrq->u.data.pointer, pAd->PortCfg.MBSSID[pAd->IoctlIF].Bssid, wrq->u.data.length);
			DBGPRINT(RT_DEBUG_TRACE, "Status = %d, Mac len= %d, data=%s \n", Status, wrq->u.data.length, &pAd->PortCfg.MBSSID[pAd->IoctlIF].Bssid);
			break;

		case OID_802_11_STATISTICS:
			DBGPRINT(RT_DEBUG_TRACE, "Query::OID_802_11_STATISTICS \n");
			
			// add the most up-to-date h/w raw counters into software counters
			NICUpdateRawCounters(pAd);

			// Sanity check for calculation of sucessful count
			if (pAd->WlanCounters.TransmittedFragmentCount < pAd->WlanCounters.RetryCount)
				pAd->WlanCounters.TransmittedFragmentCount = pAd->WlanCounters.RetryCount;
		
			//DBGPRINT(RT_DEBUG_TRACE,"-------------Statistics.TransmittedFragmentCount=%d\n",Statistics.TransmittedFragmentCount);
			Statistics.TransmittedFragmentCount = pAd->WlanCounters.TransmittedFragmentCount;
			Statistics.MulticastTransmittedFrameCount = pAd->WlanCounters.MulticastTransmittedFrameCount;
			Statistics.FailedCount = pAd->WlanCounters.FailedCount;
			Statistics.RetryCount = pAd->WlanCounters.RetryCount;
			Statistics.MultipleRetryCount = pAd->WlanCounters.MultipleRetryCount;
			Statistics.RTSSuccessCount = pAd->WlanCounters.RTSSuccessCount;
			Statistics.RTSFailureCount = pAd->WlanCounters.RTSFailureCount;
			Statistics.ACKFailureCount = pAd->WlanCounters.ACKFailureCount;
			Statistics.FrameDuplicateCount = pAd->WlanCounters.FrameDuplicateCount;
			Statistics.ReceivedFragmentCount = pAd->WlanCounters.ReceivedFragmentCount;
			Statistics.MulticastReceivedFrameCount = pAd->WlanCounters.MulticastReceivedFrameCount;
		#ifdef DBG	
			Statistics.FCSErrorCount = pAd->RalinkCounters.RealFcsErrCount;
		#else
			Statistics.FCSErrorCount = pAd->WlanCounters.FCSErrorCount;
			//Statistics.FrameDuplicateCount.vv.LowPart = pAd->WlanCounters.FrameDuplicateCount.vv.LowPart / 100;
		#endif
			wrq->u.data.length = sizeof(NDIS_802_11_STATISTICS);
			Status = copy_to_user(wrq->u.data.pointer, &Statistics, wrq->u.data.length);
			break;

		case OID_802_11_RTS_THRESHOLD:
			DBGPRINT(RT_DEBUG_TRACE, "Query::OID_802_11_RTS_THRESHOLD \n");
			wrq->u.data.length = sizeof(ulInfo);
			ulInfo = pAd->PortCfg.RtsThreshold; //MAX_RTS_THRESHOLD;
			Status = copy_to_user(wrq->u.data.pointer, &ulInfo, wrq->u.data.length);
			break;

		case OID_802_11_FRAGMENTATION_THRESHOLD:
			DBGPRINT(RT_DEBUG_TRACE, "Query::OID_802_11_FRAGMENTATION_THRESHOLD \n");
			wrq->u.data.length = sizeof(ulInfo);
			ulInfo = pAd->PortCfg.FragmentThreshold;//MAX_FRAG_THRESHOLD;
			Status = copy_to_user(wrq->u.data.pointer, &ulInfo, wrq->u.data.length);
			break;

		// SNMP ieee802dot11
		case RT_OID_802_11_MANUFACTUREROUI:
			DBGPRINT(RT_DEBUG_TRACE, "Query::RT_OID_802_11_MANUFACTUREROUI \n");
			wrq->u.data.length = ManufacturerOUI_LEN;
			Status = copy_to_user(wrq->u.data.pointer, &pAd->CurrentAddress, wrq->u.data.length);
			break;

		case RT_OID_802_11_MANUFACTURERNAME:
			DBGPRINT(RT_DEBUG_TRACE, "Query::RT_OID_802_11_MANUFACTURERNAME \n");
			wrq->u.data.length = strlen(ManufacturerNAME);
			Status = copy_to_user(wrq->u.data.pointer, ManufacturerNAME, wrq->u.data.length);
			break;

		case RT_OID_802_11_RESOURCETYPEIDNAME:
			DBGPRINT(RT_DEBUG_TRACE, "Query::RT_OID_802_11_RESOURCETYPEIDNAME \n");
			wrq->u.data.length = strlen(ResourceTypeIdName);
			Status = copy_to_user(wrq->u.data.pointer, ResourceTypeIdName, wrq->u.data.length);
			break;

		case RT_OID_802_11_PRIVACYOPTIONIMPLEMENTED:
			DBGPRINT(RT_DEBUG_TRACE, "Query::RT_OID_802_11_PRIVACYOPTIONIMPLEMENTED \n");
			ulInfo = 1; // 1 is support wep else 2 is not support.
			wrq->u.data.length = sizeof(ulInfo);
			Status = copy_to_user(wrq->u.data.pointer, &ulInfo, wrq->u.data.length);
			break;

		case RT_OID_802_11_POWERMANAGEMENTMODE:
			DBGPRINT(RT_DEBUG_TRACE, "Query::RT_OID_802_11_POWERMANAGEMENTMODE \n");
			ulInfo = 1; // 1 is power active else 2 is power save.
			wrq->u.data.length = sizeof(ulInfo);
			Status = copy_to_user(wrq->u.data.pointer, &ulInfo, wrq->u.data.length);
			break;

		case OID_802_11_WEPDEFAULTKEYVALUE:
			DBGPRINT(RT_DEBUG_TRACE, "Query::OID_802_11_WEPDEFAULTKEYVALUE \n");
			//KeyIdxValue.KeyIdx = pAd->PortCfg.MBSSID[pAd->IoctlIF].DefaultKeyId;
			pKeyIdxValue = wrq->u.data.pointer;
			DBGPRINT(RT_DEBUG_TRACE,"KeyIdxValue.KeyIdx = %d, \n",pKeyIdxValue->KeyIdx);
			valueLen = strlen(pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[pKeyIdxValue->KeyIdx].Key);
			NdisMoveMemory(pKeyIdxValue->Value,
						   pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[pKeyIdxValue->KeyIdx].Key,
						   valueLen);
			pKeyIdxValue->Value[valueLen]='\0';

			wrq->u.data.length = sizeof(DefaultKeyIdxValue);

			Status = copy_to_user(wrq->u.data.pointer, pKeyIdxValue, wrq->u.data.length);
			DBGPRINT(RT_DEBUG_TRACE,"DefaultKeyId = %d, total len = %d, str len=%d, KeyValue= %02x %02x %02x %02x %02x \n", pAd->PortCfg.MBSSID[pAd->IoctlIF].DefaultKeyId, wrq->u.data.length, strlen(pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[pKeyIdxValue->KeyIdx].Key),
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[pKeyIdxValue->KeyIdx].Key[0],
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[pKeyIdxValue->KeyIdx].Key[1],
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[pKeyIdxValue->KeyIdx].Key[2],
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[pKeyIdxValue->KeyIdx].Key[3],
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[pKeyIdxValue->KeyIdx].Key[4]);

			/*DBGPRINT(RT_DEBUG_TRACE,"DefaultKeyId = %d, len = %d, KeyValue= %02x %02x %02x %02x %02x \n", pAd->PortCfg.MBSSID[pAd->IoctlIF].DefaultKeyId, strlen(pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[KeyIdxValue.KeyIdx].Key),
			KeyIdxValue.Value[0],
			KeyIdxValue.Value[1],
			KeyIdxValue.Value[2],
			KeyIdxValue.Value[3],
			KeyIdxValue.Value[4]);*/
			
			break;
		
		case OID_802_11_WEPDEFAULTKEYID:
			DBGPRINT(RT_DEBUG_TRACE, "Query::RT_OID_802_11_WEPDEFAULTKEYID \n");
			wrq->u.data.length = sizeof(UCHAR);
			Status = copy_to_user(wrq->u.data.pointer, &pAd->PortCfg.MBSSID[pAd->IoctlIF].DefaultKeyId, wrq->u.data.length);
			DBGPRINT(RT_DEBUG_TRACE, "DefaultKeyId =%d \n",pAd->PortCfg.MBSSID[pAd->IoctlIF].DefaultKeyId);
			break;

		case RT_OID_802_11_WEPKEYMAPPINGLENGTH:
			DBGPRINT(RT_DEBUG_TRACE, "Query::RT_OID_802_11_WEPKEYMAPPINGLENGTH \n");
			wrq->u.data.length = sizeof(UCHAR);
			Status = copy_to_user(wrq->u.data.pointer,
									&pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[pAd->PortCfg.MBSSID[pAd->IoctlIF].DefaultKeyId].KeyLen,
									wrq->u.data.length);
			break;

		case OID_802_11_SHORTRETRYLIMIT:
			DBGPRINT(RT_DEBUG_TRACE, "Query::OID_802_11_SHORTRETRYLIMIT \n");
			wrq->u.data.length = sizeof(ULONG);
			RTMP_IO_READ32(pAd, TXRX_CSR4, &csr4.word);
			ShortRetryLimit = csr4.field.ShortRetryLimit;
			DBGPRINT(RT_DEBUG_TRACE, "ShortRetryLimit =%ld,  csr4.field.ShortRetryLimit=%ld\n", ShortRetryLimit, csr4.field.ShortRetryLimit);
			Status = copy_to_user(wrq->u.data.pointer, &ShortRetryLimit, wrq->u.data.length);
			break;

		case OID_802_11_LONGRETRYLIMIT:
			DBGPRINT(RT_DEBUG_TRACE, "Query::OID_802_11_LONGRETRYLIMIT \n");
			wrq->u.data.length = sizeof(ULONG);
			RTMP_IO_READ32(pAd, TXRX_CSR4, &csr4.word);
			LongRetryLimit = csr4.field.LongRetryLimit;
			DBGPRINT(RT_DEBUG_TRACE, "LongRetryLimit =%ld,  csr4.field.LongRetryLimit=%ld\n", LongRetryLimit, csr4.field.LongRetryLimit);
			Status = copy_to_user(wrq->u.data.pointer, &LongRetryLimit, wrq->u.data.length);
			break;
	
		case RT_OID_802_11_PRODUCTID:
			DBGPRINT(RT_DEBUG_TRACE, "Query::RT_OID_802_11_PRODUCTID \n");
			sprintf(tmp, "%04x, %04x, %04x \n", NIC2661_PCI_DEVICE_ID, NIC2561S_PCI_DEVICE_ID, NIC2561_PCI_DEVICE_ID);
			wrq->u.data.length = strlen(tmp);
			Status = copy_to_user(wrq->u.data.pointer, tmp, wrq->u.data.length);
			break;

		case RT_OID_802_11_MANUFACTUREID:
			DBGPRINT(RT_DEBUG_TRACE, "Query::RT_OID_802_11_MANUFACTUREID \n");
			wrq->u.data.length = strlen(ManufacturerNAME);
			Status = copy_to_user(wrq->u.data.pointer, ManufacturerNAME, wrq->u.data.length);
			break;

		case OID_802_11_TX_ANTENNA_SELECTED:
			DBGPRINT(RT_DEBUG_TRACE, "Query::OID_802_11_TX_ANTENNA_SELECTED \n");
			wrq->u.data.length = sizeof(USHORT);
			ulInfo = pAd->Antenna.field.TxDefaultAntenna;
			Status = copy_to_user(wrq->u.data.pointer, &ulInfo, wrq->u.data.length);
			DBGPRINT(RT_DEBUG_TRACE, "Antenna tx=%d \n", pAd->Antenna.field.TxDefaultAntenna);
			break;

		case OID_802_11_RX_ANTENNA_SELECTED:
			DBGPRINT(RT_DEBUG_TRACE, "Query::OID_802_11_TX_ANTENNA_SELECTED \n");
			wrq->u.data.length = sizeof(USHORT);
			ulInfo = pAd->Antenna.field.RxDefaultAntenna;
			Status = copy_to_user(wrq->u.data.pointer, &ulInfo, wrq->u.data.length);
			DBGPRINT(RT_DEBUG_TRACE, "Antenna rx=%d \n", pAd->Antenna.field.RxDefaultAntenna);
			break;

		case OID_802_11_NUMBER_OF_ANTENNAS:
			DBGPRINT(RT_DEBUG_TRACE, "Query::OID_802_11_NUMBER_OF_ANTENNAS \n");
			wrq->u.data.length = sizeof(USHORT);
			ulInfo = pAd->Antenna.field.NumOfAntenna;
			Status = copy_to_user(wrq->u.data.pointer, &ulInfo, wrq->u.data.length);
			DBGPRINT(RT_DEBUG_TRACE, "Antenna num=%d \n", pAd->Antenna.field.NumOfAntenna);
			break;

		case OID_802_11_DESIRED_RATES:
			DBGPRINT(RT_DEBUG_TRACE, "Query::OID_802_11_DESIRED_RATES \n");
			wrq->u.data.length = sizeof(NDIS_802_11_RATES);
			Status = copy_to_user(wrq->u.data.pointer, pAd->PortCfg.MBSSID[pAd->IoctlIF].DesiredRates, wrq->u.data.length);
			
			DBGPRINT(RT_DEBUG_TRACE, "Query::OID_802_11_DESIRED_RATES (%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x)\n", 
				pAd->PortCfg.MBSSID[pAd->IoctlIF].DesiredRates[0],pAd->PortCfg.MBSSID[pAd->IoctlIF].DesiredRates[1],
				pAd->PortCfg.MBSSID[pAd->IoctlIF].DesiredRates[2],pAd->PortCfg.MBSSID[pAd->IoctlIF].DesiredRates[3],
				pAd->PortCfg.MBSSID[pAd->IoctlIF].DesiredRates[4],pAd->PortCfg.MBSSID[pAd->IoctlIF].DesiredRates[5],
				pAd->PortCfg.MBSSID[pAd->IoctlIF].DesiredRates[6],pAd->PortCfg.MBSSID[pAd->IoctlIF].DesiredRates[7]);
				
			break;
			
		case OID_802_11_CURRENTCHANNEL:
			DBGPRINT(RT_DEBUG_TRACE, "Query::OID_802_11_CURRENTCHANNEL \n");
			wrq->u.data.length = sizeof(UCHAR);
			DBGPRINT(RT_DEBUG_TRACE, "sizeof UCHAR=%d, channel=%d \n", sizeof(UCHAR), pAd->PortCfg.Channel);
			Status = copy_to_user(wrq->u.data.pointer, &pAd->PortCfg.Channel, wrq->u.data.length);
			DBGPRINT(RT_DEBUG_TRACE, "Status=%d\n", Status);
			break;
#endif //snmp
#ifdef WSC_SUPPORT
		case RT_OID_WSC_QUERY_STATUS:
			DBGPRINT(RT_DEBUG_TRACE, "Query::RT_OID_WSC_QUERY_STATUS \n");
			wrq->u.data.length = sizeof(INT);
			if (copy_to_user(wrq->u.data.pointer, &pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.WscStatus, wrq->u.data.length))
			{
				Status = -EFAULT;
			}
			break;

		case RT_OID_WSC_PIN_CODE:
			DBGPRINT(RT_DEBUG_TRACE, "Query::RT_OID_WSC_PIN_CODE \n");
			wrq->u.data.length = sizeof(ULONG);
			// For Re-Generate Pin-Code Modification
			//WscPinCode = WscGeneratePinCode(pAd, pAd->IoctlIF);
			WscPinCode = pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscEnrolleePinCode;
			// --
			
			if (copy_to_user(wrq->u.data.pointer, &WscPinCode, wrq->u.data.length))
			{
				Status = -EFAULT;
			}
			break;
// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
		case RT_OID_APCLI_WSC_PIN_CODE:
			DBGPRINT(RT_DEBUG_TRACE, "Query::RT_OID_APCLI_WSC_PIN_CODE \n");
			wrq->u.data.length = sizeof(UINT);
			//WscPinCode = WscGeneratePinCode(pAd, TRUE, apidx);
			WscPinCode = pAd->ApCliTab.ApCliEntry[MAIN_MBSSID].WscControl.WscEnrolleePinCode;
			
			if (copy_to_user(wrq->u.data.pointer, &WscPinCode, wrq->u.data.length))
			{
				Status = -EFAULT;
			}
			break;
#endif // APCLI_SUPPORT //
// --
		case RT_OID_WSC_UUID:
			DBGPRINT(RT_DEBUG_TRACE, "Query::RT_OID_WSC_QUERY_UUID \n");
			// <<WCN vista logo>> ++, edit by johnli
/*
			wrq->u.data.length = 16*sizeof(UCHAR);
			NdisMoveMemory(&uuidStr[0], &Wsc_Uuid_E[0], 4);
			snprintf(&uuidStr[4], 13, "%02x%02x%02x%02x%02x%02x", 
				pAd->CurrentAddress[0], pAd->CurrentAddress[1], pAd->CurrentAddress[2], 
				pAd->CurrentAddress[3], pAd->CurrentAddress[4], pAd->CurrentAddress[5]);
			if (copy_to_user(wrq->u.data.pointer, &uuidStr, wrq->u.data.length))
*/
			wrq->u.data.length = UUID_LEN_STR;
			if (copy_to_user(wrq->u.data.pointer, &Wsc_Uuid_Str[0], UUID_LEN_STR))
			// <<WCN vista logo>> --
			{
				Status = -EFAULT;
			}
			break;
		case RT_OID_WSC_MAC_ADDRESS:
			DBGPRINT(RT_DEBUG_TRACE, "Query::RT_OID_WSC_MAC_ADDRESS \n");
			wrq->u.data.length = MAC_ADDR_LEN;
			if (copy_to_user(wrq->u.data.pointer, pAd->PortCfg.MBSSID[pAd->IoctlIF].Bssid, wrq->u.data.length))
			{
				Status = -EFAULT;
			}
			break;
#endif // WSC_SUPPORT //

//For LLTD, 2006.12.05 ++
#ifdef LLTD_SUPPORT
        case RT_OID_GET_PHY_MODE:
            DBGPRINT(RT_DEBUG_TRACE, "Query::Get phy mode (%02X) \n", pAd->PortCfg.PhyMode);
            wrq->u.mode = (u32)pAd->PortCfg.PhyMode;
            break;

        case RT_OID_GET_LLTD_ASSO_TANLE:
            DBGPRINT(RT_DEBUG_TRACE, "Query::Get LLTD assoication table\n");
            if ((wrq->u.data.pointer == NULL) || (pAd->IoctlIF != MAIN_MBSSID))
            {
                Status = -EFAULT;
            }
            else
            {
                INT						    i;
                RT_LLTD_ASSOICATION_TABLE	AssocTab;

            	AssocTab.Num = 0;
            	for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++)
            	{
            		if (pAd->MacTab.Content[i].Valid && (pAd->MacTab.Content[i].Sst == SST_ASSOC))
            		{
            			COPY_MAC_ADDR(AssocTab.Entry[AssocTab.Num].Addr, &pAd->MacTab.Content[i].Addr);
                        AssocTab.Entry[AssocTab.Num].phyMode = pAd->PortCfg.PhyMode;
                        AssocTab.Entry[AssocTab.Num].MOR = RateIdToMbps[pAd->PortCfg.MBSSID[pAd->IoctlIF].MaxTxRate] * 2;
            			AssocTab.Num += 1;
            		}
            	}            
                wrq->u.data.length = sizeof(RT_LLTD_ASSOICATION_TABLE);
            	if (copy_to_user(wrq->u.data.pointer, &AssocTab, wrq->u.data.length))
            	{
            		DBGPRINT(RT_DEBUG_TRACE, "%s: copy_to_user() fail\n", __FUNCTION__);
                    Status = -EFAULT;
            	}
                DBGPRINT(RT_DEBUG_TRACE, "AssocTab.Num = %d \n", AssocTab.Num);
            }
            break;

#ifdef APCLI_SUPPORT
        case RT_OID_GET_REPEATER_AP_LINEAGE:
            DBGPRINT(RT_DEBUG_TRACE, "Query::Get repeater AP lineage.\n");
            if ((wrq->u.data.pointer == NULL) || (pAd->IoctlIF != MAIN_MBSSID))
            {
                Status = -EFAULT;
            }
            else
            {
                if (pAd->ApCliTab.ApCliEntry[pAd->IoctlIF].Valid)
                {
                    wrq->u.data.length = 6;
                	if (copy_to_user(wrq->u.data.pointer, pAd->ApCliTab.ApCliEntry[pAd->IoctlIF].ApCliBssid, wrq->u.data.length))
                	{
                		DBGPRINT(RT_DEBUG_TRACE, "%s: copy_to_user() fail\n", __FUNCTION__);
                        Status = -EFAULT;
                	}
                	DBGPRINT(RT_DEBUG_TRACE, "%s: Root AP BSSID: \n", __FUNCTION__);
                }
                else
                    wrq->u.data.length = 0;
            }
            break;
#endif /* APCLI_SUPPORT */
#endif /* LLTD_SUPPORT */
//For LLTD, 2006.12.05 --

   		default:
			DBGPRINT(RT_DEBUG_TRACE, "Query::unknown IOCTL's subcmd = 0x%08x\n", cmd);
			Status = -EOPNOTSUPP;
			break;
    }

	return Status;
}

INT RT61_ioctl(
	IN	struct net_device	*net_dev, 
	IN	OUT	struct ifreq	*rq, 
	IN	INT					cmd)
{
    VIRTUAL_ADAPTER                     *pVirtualAd;
	RTMP_ADAPTER                        *pAd = net_dev->priv;
    struct iwreq                        *wrq = (struct iwreq *) rq;
    INT                                 Status = NDIS_STATUS_SUCCESS;
    USHORT                              subcmd, index;

    //+ patch for SnapGear Request even the interface is down
    if(cmd== SIOCGIWNAME){
	    DBGPRINT(RT_DEBUG_TRACE, "IOCTL::SIOCGIWNAME\n");
	    strcpy(wrq->u.name, "RT61 SoftAP");
	    return Status;
    }//- patch for SnapGear
	
    if((net_dev->priv_flags == INT_MAIN) && !RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE))
    {
		DBGPRINT(RT_DEBUG_TRACE, "INFO::Network is down!\n");
		return -ENETDOWN;
    }

    // determine this ioctl command is comming from which interface.
    if (net_dev->priv_flags == INT_MAIN)
    {
    	pAd= net_dev->priv;
        pAd->IoctlIF = MAIN_MBSSID;
        DBGPRINT(RT_DEBUG_INFO, "RT61_ioctl I/F(ra%d)(flags=%d): cmd = 0x%08x\n", pAd->IoctlIF, net_dev->priv_flags, cmd);
    }
    else if (net_dev->priv_flags == INT_MBSSID)
    {
    	pVirtualAd = net_dev->priv;
        pAd = pVirtualAd->RtmpDev->priv;

    	if (!RTMPEqualMemory(net_dev->name, pAd->net_dev->name, 3))  // for multi-physical card, no MBSSID
    	{
	        for (index = 1; index < pAd->PortCfg.BssidNum; index++)
	    	{
	    	    if (pAd->PortCfg.MBSSID[index].MSSIDDev == net_dev)
	    	    {
	    	        pAd->IoctlIF = index;
	    	        
	    	        DBGPRINT(RT_DEBUG_INFO, "RT61_ioctl I/F(ra%d)(flags=%d): cmd = 0x%08x\n", index, net_dev->priv_flags, cmd);
	    	        break;
	    	    }
	    	}
	        // Interface not found!
	        if(index == pAd->PortCfg.BssidNum)
	        {
	        	DBGPRINT(RT_DEBUG_ERROR, "RT61_ioctl can not find I/F\n");
	            return -ENETDOWN;
	        }
	    }
	    else    // ioctl command from I/F(ra0)
	    {
	    	pAd= net_dev->priv;
    	    pAd->IoctlIF = MAIN_MBSSID;
	        DBGPRINT(RT_DEBUG_ERROR, "RT61_ioctl can not find I/F and use default: cmd = 0x%08x\n", cmd);
	    }
    }
#ifdef APCLI_SUPPORT
    else if (net_dev->priv_flags == INT_APCLI)
    {
        pVirtualAd = net_dev->priv;
        pAd = pVirtualAd->RtmpDev->priv;

        for (index = 0; index < MAX_APCLI_ENTRY; index++)
        {
            if (pAd->ApCliTab.ApCliEntry[index].dev == net_dev)
            {
                pAd->ApcliIF = index;

                DBGPRINT(RT_DEBUG_INFO, "RT61_ioctl I/F(apcli%d)(flags=%d): cmd = 0x%08x\n", index, net_dev->priv_flags, cmd);
                break;
            }
        }
    }
#endif
    else
    {
    	DBGPRINT(RT_DEBUG_WARN, "IOCTL is not supported in WDS interface\n");
    	return -EOPNOTSUPP;
    }

	switch(cmd)
	{
        case SIOCGIFHWADDR:
			DBGPRINT(RT_DEBUG_TRACE, "IOCTLIOCTLIOCTL::SIOCGIFHWADDR\n");
			strcpy(wrq->u.name, pAd->PortCfg.MBSSID[pAd->IoctlIF].Bssid);
			break;
		case SIOCGIWNAME:
			DBGPRINT(RT_DEBUG_TRACE, "IOCTL::SIOCGIWNAME\n");
			strcpy(wrq->u.name, "RT61 SoftAP");
			break;
		case SIOCSIWESSID:  //Set ESSID
			Status = -EOPNOTSUPP;
			break;
		case SIOCGIWESSID:  //Get ESSID
			{
				struct iw_point *erq = &wrq->u.essid;
				PCHAR pSsidStr = NULL;

				erq->flags=1;

#ifdef APCLI_SUPPORT
				if (net_dev->priv_flags == INT_APCLI)
				{
					if (pAd->ApCliTab.ApCliEntry[pAd->ApcliIF].Valid == TRUE)
					{
						erq->length = pAd->ApCliTab.ApCliEntry[pAd->ApcliIF].SsidLen;
						pSsidStr = (PCHAR)&pAd->ApCliTab.ApCliEntry[pAd->ApcliIF].Ssid;
					}
					else {
						erq->length = 0;
						pSsidStr = NULL;
					}
				}
				else
#endif
				{
					erq->length = pAd->PortCfg.MBSSID[pAd->IoctlIF].SsidLen;
					pSsidStr = (PCHAR)&pAd->PortCfg.MBSSID[pAd->IoctlIF].Ssid;
				}

				if((erq->pointer) && (pSsidStr != NULL))
				{
					if(copy_to_user(erq->pointer, pSsidStr, erq->length))
					{
						Status = -EFAULT;
						break;
					}
				}
				DBGPRINT(RT_DEBUG_TRACE, "IOCTL::SIOCGIWESSID (Len=%d, ssid=%s...)\n", erq->length, (char *)erq->pointer);
			}
			break;
		case SIOCGIWNWID: // get network id 
		case SIOCSIWNWID: // set network id (the cell)
			Status = -EOPNOTSUPP;
			break;
		case SIOCGIWFREQ: // get channel/frequency (Hz)
			wrq->u.freq.m = pAd->PortCfg.Channel;
			wrq->u.freq.e = 0;
			wrq->u.freq.i = 0;
			break; 
		case SIOCSIWFREQ: //set channel/frequency (Hz)
		case SIOCGIWNICKN:
		case SIOCSIWNICKN: //set node name/nickname
		case SIOCGIWRATE:  //get default bit rate (bps)
			{
				UCHAR TxRate;

#ifdef APCLI_SUPPORT
				if (net_dev->priv_flags == INT_APCLI)
					TxRate = pAd->ApCliTab.ApCliEntry[pAd->ApcliIF].CurrTxRate;
				else
#endif
					TxRate = pAd->PortCfg.MBSSID[pAd->IoctlIF].TxRate;	

				wrq->u.bitrate.value = RateIdTo500Kbps[TxRate] * 500000;
				wrq->u.bitrate.disabled = 0;
			}
			break;
		case SIOCSIWRATE:  //set default bit rate (bps)
		case SIOCGIWRTS:  // get RTS/CTS threshold (bytes)
		case SIOCSIWRTS:  //set RTS/CTS threshold (bytes)
		case SIOCGIWFRAG:  //get fragmentation thr (bytes)
		case SIOCSIWFRAG:  //set fragmentation thr (bytes)
		case SIOCGIWENCODE:  //get encoding token & mode
		case SIOCSIWENCODE:  //set encoding token & mode
			Status = -EOPNOTSUPP;
			break;
		case SIOCGIWAP:  //get access point MAC addresses
			{
				PUCHAR pBssid = NULL;

#ifdef APCLI_SUPPORT
				if (net_dev->priv_flags == INT_APCLI)
				{
					if (pAd->ApCliTab.ApCliEntry[pAd->ApcliIF].Valid == TRUE)
						pBssid = (PCHAR)&pAd->ApCliTab.ApCliEntry[pAd->ApcliIF].ApCliBssid;
					else
						pBssid = NULL;
				}
				else
#endif
				{
					pBssid = (PCHAR)&pAd->PortCfg.MBSSID[pAd->IoctlIF].Bssid;
				}


				if (pBssid != NULL)
				{
					wrq->u.ap_addr.sa_family = ARPHRD_ETHER;
					memcpy(wrq->u.ap_addr.sa_data, pBssid, ETH_ALEN);
					DBGPRINT(RT_DEBUG_TRACE, "IOCTL::SIOCGIWAP(=%02x:%02x:%02x:%02x:%02x:%02x)\n",
						pBssid[0], pBssid[1], pBssid[2], pBssid[3], pBssid[4], pBssid[5]);
				}
				else
					Status = -ENOTCONN;
			}
			break;
		case SIOCGIWMODE:  //get operation mode
			wrq->u.mode = IW_MODE_INFRA;   //SoftAP always on INFRA mode.
			break;
		case SIOCSIWAP:  //set access point MAC addresses
		case SIOCSIWMODE:  //set operation mode
		case SIOCGIWSENS:   //get sensitivity (dBm)
		case SIOCSIWSENS:	//set sensitivity (dBm)
		case SIOCGIWPOWER:  //get Power Management settings
		case SIOCSIWPOWER:  //set Power Management settings
		case SIOCGIWTXPOW:  //get transmit power (dBm)
		case SIOCSIWTXPOW:  //set transmit power (dBm)
		//case SIOCGIWRANGE:	//Get range of parameters
		case SIOCGIWRETRY:	//get retry limits and lifetime
		case SIOCSIWRETRY:	//set retry limits and lifetime
			Status = -EOPNOTSUPP;
			break;
		case SIOCGIWRANGE:	//Get range of parameters
		    {
		        struct iw_range range;

                        memset(&range, 0, sizeof(range));
	                range.we_version_compiled = WIRELESS_EXT;
	                range.we_version_source = 14;
                        copy_to_user(wrq->u.data.pointer, &range, sizeof(range));
		    }
		    break;
		    
#if WIRELESS_EXT >= 14
		case SIOCGIWSCAN:   //get scanning results
			RTMP_SIOCGIWSCAN(pAd,wrq);
			break;
		case SIOCSIWSCAN:   //trigger scanning (list cells)
			ApSiteSurvey(pAd);
			break;
#endif
		case SIOCGIWAPLIST: //client association list
			RTMP_SIOCGIWAPLIST(pAd,wrq);
			break;
		case RT_PRIV_IOCTL:
			subcmd = wrq->u.data.flags;
			if( subcmd & OID_GET_SET_TOGGLE)
				Status = RTMPSetInformation(pAd, rq, subcmd);
			else
				Status = RTMPQueryInformation(pAd, rq, subcmd);
			break;
		case SIOCGIWPRIV:
			if (wrq->u.data.pointer) {
				if ( access_ok(VERIFY_WRITE, wrq->u.data.pointer, sizeof(privtab)) != TRUE)
					break;
				wrq->u.data.length = sizeof(privtab) / sizeof(privtab[0]);
				if (copy_to_user(wrq->u.data.pointer, privtab, sizeof(privtab)))
					Status = -EFAULT;
			}
			break;
		case RTPRIV_IOCTL_SET:
			{				
				CHAR *this_char;
				CHAR *value;

				if( access_ok(VERIFY_READ, wrq->u.data.pointer, wrq->u.data.length) != TRUE)
					break;

				while ((this_char = strsep((char **)&wrq->u.data.pointer, ",")) != NULL) 
				{
					if (!*this_char)
						 continue;

					if ((value = strchr(this_char, '=')) != NULL)
						*value++ = 0;

					if (!value)
						continue;  							

// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
					if (net_dev->priv_flags == INT_APCLI)
						pAd->bFromApcli = TRUE;
					else
						pAd->bFromApcli = FALSE;
#endif
// --

					for (PRTMP_PRIVATE_SET_PROC = RTMP_PRIVATE_SUPPORT_PROC; PRTMP_PRIVATE_SET_PROC->name; PRTMP_PRIVATE_SET_PROC++)
					{
						if (!strcmp(this_char, PRTMP_PRIVATE_SET_PROC->name)) 
						{						
							if(!PRTMP_PRIVATE_SET_PROC->set_proc(pAd, value))
							{   //FALSE:Set private failed then return Invalid argument 								
								Status = -EINVAL;							
							}
							break;  //Exit for loop.
						}
					}

					if(PRTMP_PRIVATE_SET_PROC->name == NULL)
					{  //Not found argument
						Status = -EINVAL;
						DBGPRINT(RT_DEBUG_TRACE, "IOCTL::(iwpriv) Command not Support [%s=%s]\n", this_char, value);
						break;
					}	
				}
			}
			break;
		    
		case RTPRIV_IOCTL_GET_MAC_TABLE:
			RTMPIoctlGetMacTable(pAd,wrq);
		    break;
		    
		case RTPRIV_IOCTL_GSITESURVEY:
			RTMPIoctlGetSiteSurvey(pAd,wrq);
		    break;
		
		case RTPRIV_IOCTL_STATISTICS:
		    RTMPIoctlStatistics(pAd, wrq);
		    break;

		case RTPRIV_IOCTL_RADIUS_DATA:
		    RTMPIoctlRadiusData(pAd, wrq);
		    break;

		case RTPRIV_IOCTL_ADD_WPA_KEY:
		    RTMPIoctlAddWPAKey(pAd, wrq);
		    break;

		case RTPRIV_IOCTL_ADD_PMKID_CACHE:
		    RTMPIoctlAddPMKIDCache(pAd, wrq);
		    break;

		case RTPRIV_IOCTL_STATIC_WEP_COPY:
		    RTMPIoctlStaticWepCopy(pAd, wrq);
		    break;
#ifdef WSC_SUPPORT
		case RTPRIV_IOCTL_WSC_PROFILE:
		    RTMPIoctlWscProfile(pAd, wrq);
		    break;
#endif // WSC_SUPPORT //
#ifdef DBG
		case RTPRIV_IOCTL_BBP:
			RTMPIoctlBBP(pAd, wrq);
			break;
			
		case RTPRIV_IOCTL_MAC:
			RTMPIoctlMAC(pAd, wrq);
			break;

		case RTPRIV_IOCTL_E2P:
			RTMPIoctlE2PROM(pAd, wrq);
			break;
			
		case RTPRIV_IOCTL_CHK_IC_VERSION:
			RTMPIoctlCHKICVER(pAd,wrq);
			break;			
#endif

		default:
			DBGPRINT(RT_DEBUG_ERROR, "IOCTL::unknown IOCTL's cmd = 0x%08x\n", cmd);
			Status = -EOPNOTSUPP;
			break;
	}

	return Status;
}

/*
	========================================================================
	
	Routine Description:
		Change NIC PHY mode. Re-association may be necessary.

	Arguments:
		pAdapter						Pointer to our adapter
        phmode
		
	========================================================================
*/
VOID	RTMPSetPhyMode(
	IN	PRTMP_ADAPTER	pAd,
	IN  ULONG  phymode,
	IN  UCHAR apidx)
{
	DBGPRINT(RT_DEBUG_TRACE,"IF(ra%d) RTMPSetPhyMode(=%d)\n", apidx, phymode);
    
	NdisZeroMemory(pAd->PortCfg.SupportedRates, MAX_LEN_OF_SUPPORTED_RATES);
	NdisZeroMemory(pAd->PortCfg.MBSSID[apidx].DesiredRates, MAX_LEN_OF_SUPPORTED_RATES);
	
    switch (phymode) {
    	case PHY_11A:
            pAd->PortCfg.SupportedRates[0]  = 0x8C;    // 6 mbps, in units of 0.5 Mbps, basic rate
            pAd->PortCfg.SupportedRates[1]  = 0x12;    // 9 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRates[2]  = 0x98;    // 12 mbps, in units of 0.5 Mbps, basic rate
            pAd->PortCfg.SupportedRates[3]  = 0x24;    // 18 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRates[4]  = 0xb0;    // 24 mbps, in units of 0.5 Mbps, basic rate
            pAd->PortCfg.SupportedRates[5]  = 0x48;    // 36 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRates[6]  = 0x60;    // 48 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRates[7]  = 0x6c;    // 54 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRatesLen  = 8;
            pAd->PortCfg.MBSSID[apidx].DesiredRates[0]  = 12;    // 6 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[1]  = 18;    // 9 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[2]  = 24;    // 12 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[3]  = 36;    // 18 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[4]  = 48;    // 24 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[5]  = 72;    // 36 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[6]  = 96;    // 48 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[7]  = 108;   // 54 mbps, in units of 0.5 Mbps
            break;
        case PHY_11B:
            pAd->PortCfg.SupportedRates[0]  = 0x82;    // 1 mbps, in units of 0.5 Mbps, basic rate
            pAd->PortCfg.SupportedRates[1]  = 0x84;    // 2 mbps, in units of 0.5 Mbps, basic rate
            pAd->PortCfg.SupportedRates[2]  = 0x0B;    // 5.5 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRates[3]  = 0x16;    // 11 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRatesLen  = 4;
            pAd->PortCfg.MBSSID[apidx].DesiredRates[0]  = 2;     // 1 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[1]  = 4;     // 2 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[2]  = 11;    // 5.5 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[3]  = 22;    // 11 mbps, in units of 0.5 Mbps
            break;
        case PHY_11BG_MIXED:
            // 2004-1-12 to keep maximum compatibility with 802.11b-only clients, we should not turn on
            // BASIC RATE bit of any OFDM rates in outgoing BEACON and ProbrResponse frames
            pAd->PortCfg.SupportedRates[0]  = 0x82;    // 1 mbps, in units of 0.5 Mbps, basic rate
            pAd->PortCfg.SupportedRates[1]  = 0x84;    // 2 mbps, in units of 0.5 Mbps, basic rate
            pAd->PortCfg.SupportedRates[2]  = 0x8B;    // 5.5 mbps, in units of 0.5 Mbps, basic rate
            pAd->PortCfg.SupportedRates[3]  = 0x96;    // 11 mbps, in units of 0.5 Mbps, basic rate
            pAd->PortCfg.SupportedRates[4]  = 0x0C;    // 6 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRates[5]  = 0x12;    // 9 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRates[6]  = 0x18;    // 12 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRates[7]  = 0x24;    // 18 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRates[8]  = 0x30;    // 24 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRates[9]  = 0x48;    // 36 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRates[10] = 0x60;    // 48 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRates[11] = 0x6C;    // 54 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRatesLen  = 12;
            pAd->PortCfg.MBSSID[apidx].DesiredRates[0]  = 2;     // 1 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[1]  = 4;     // 2 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[2]  = 11;    // 5.5 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[3]  = 22;    // 11 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[4]  = 12;    // 6 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[5]  = 18;    // 9 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[6]  = 24;    // 12 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[7]  = 36;    // 18 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[8]  = 48;    // 24 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[9]  = 72;    // 36 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[10] = 96;    // 48 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[11] = 108;   // 54 mbps, in units of 0.5 Mbps
            break;
        case PHY_11G:
            // 2004-1-12 to keep maximum compatibility with 802.11b-only clients, we should not turn on
            // BASIC RATE bit of any OFDM rates in outgoing BEACON and ProbrResponse frames
            pAd->PortCfg.SupportedRates[0]  = 0x82;    // 1 mbps, in units of 0.5 Mbps, basic rate
            pAd->PortCfg.SupportedRates[1]  = 0x84;    // 2 mbps, in units of 0.5 Mbps, basic rate
            pAd->PortCfg.SupportedRates[2]  = 0x8B;    // 5.5 mbps, in units of 0.5 Mbps, basic rate
            pAd->PortCfg.SupportedRates[3]  = 0x96;    // 11 mbps, in units of 0.5 Mbps, basic rate
            pAd->PortCfg.SupportedRates[4]  = 0x8C;    // 6 mbps, in units of 0.5 Mbps, basic rate
            pAd->PortCfg.SupportedRates[5]  = 0x12;    // 9 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRates[6]  = 0x98;    // 12 mbps, in units of 0.5 Mbps, basic rate
            pAd->PortCfg.SupportedRates[7]  = 0x24;    // 18 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRates[8]  = 0xB0;    // 24 mbps, in units of 0.5 Mbps, basic rate
            pAd->PortCfg.SupportedRates[9]  = 0x48;    // 36 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRates[10] = 0x60;    // 48 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRates[11] = 0x6C;    // 54 mbps, in units of 0.5 Mbps
            pAd->PortCfg.SupportedRatesLen  = 12;
            pAd->PortCfg.MBSSID[apidx].DesiredRates[0]  = 2;     // 1 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[1]  = 4;     // 2 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[2]  = 11;    // 5.5 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[3]  = 22;    // 11 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[4]  = 12;    // 6 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[5]  = 18;    // 9 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[6]  = 24;    // 12 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[7]  = 36;    // 18 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[8]  = 48;    // 24 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[9]  = 72;    // 36 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[10] = 96;    // 48 mbps, in units of 0.5 Mbps
            pAd->PortCfg.MBSSID[apidx].DesiredRates[11] = 108;   // 54 mbps, in units of 0.5 Mbps
            break;
        default:
            break;
    }
}

INT Set_DriverVersion_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
    DBGPRINT(RT_DEBUG_TRACE, "Driver version-%s\n", DRIVER_VERSION);
    
    return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Country Region.
        This command will not work, if the field of CountryRegion in eeprom is programmed.
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_CountryRegion_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG region;
		
	region = simple_strtol(arg, 0, 10);

	// Country can be set only when EEPROM not programmed
	if (pAd->PortCfg.CountryRegion & 0x80)
	{
		DBGPRINT(RT_DEBUG_ERROR, "Set_CountryRegion_Proc::parameter of CountryRegion in eeprom is programmed \n");
		return FALSE;
	}
	
	if(region >= 0 && region <= 7)
	{
		pAd->PortCfg.CountryRegion = (UCHAR) region;
	}
	else
	{
		DBGPRINT(RT_DEBUG_ERROR, "Set_CountryRegion_Proc::parameters out of range\n");
		return FALSE;
	}

	// if set country region, driver needs to be reset
	BuildChannelList(pAd);

	DBGPRINT(RT_DEBUG_TRACE, "Set_CountryRegion_Proc::(CountryRegion=%d)\n", pAd->PortCfg.CountryRegion);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Country Region for A band.
        This command will not work, if the field of CountryRegion in eeprom is programmed.
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_CountryRegionABand_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG region;
		
	region = simple_strtol(arg, 0, 10);

	// Country can be set only when EEPROM not programmed
	if (pAd->PortCfg.CountryRegionForABand & 0x80)
	{
		DBGPRINT(RT_DEBUG_ERROR, "Set_CountryRegionABand_Proc::parameter of CountryRegion in eeprom is programmed \n");
		return FALSE;
	}
	
	if((region >= 0) && (region <= 10) && (region != 9)) //region 9 already not support old band
	{
		pAd->PortCfg.CountryRegionForABand = (UCHAR) region;
	}
	else
	{
		DBGPRINT(RT_DEBUG_ERROR, "Set_CountryRegionABand_Proc::parameters out of range\n");
		return FALSE;
	}

	// if set country region, driver needs to be reset
	BuildChannelList(pAd);

	DBGPRINT(RT_DEBUG_TRACE, "Set_CountryRegionABand_Proc::(CountryRegion=%d)\n", pAd->PortCfg.CountryRegionForABand);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Country Code.
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_CountryCode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{

	if(strlen(arg) == 2)
	{
		NdisMoveMemory(pAd->PortCfg.CountryCode, arg, 2);
		pAd->PortCfg.CountryCode[2] = ' ';
		pAd->PortCfg.bCountryFlag = TRUE;
	}
	else
	{
		NdisZeroMemory(pAd->PortCfg.CountryCode, 3);
		pAd->PortCfg.bCountryFlag = FALSE;
	}	
		
	DBGPRINT(RT_DEBUG_TRACE, "Set_CountryCode_Proc::(bCountryFlag=%d, CountryCode=%s)\n", pAd->PortCfg.bCountryFlag, pAd->PortCfg.CountryCode);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Country String.
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_CountryString_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{

	INT   index;
	INT   success = FALSE;
	UCHAR  name_buffer[34] = {0};


	// Country can be set only when EEPROM not programmed
	if (pAd->PortCfg.PhyMode == PHY_11A)
	{
		if (pAd->PortCfg.CountryRegionForABand & 0x80)
		{
			DBGPRINT(RT_DEBUG_ERROR, "Set_CountryString_Proc::parameter of CountryRegion in eeprom is programmed \n");
			return FALSE;
		}
	}
	else
	{
		if (pAd->PortCfg.CountryRegion & 0x80)
		{
			DBGPRINT(RT_DEBUG_ERROR, "Set_CountryString_Proc::parameter of CountryRegion in eeprom is programmed \n");
			return FALSE;
		}
	}

	if(strlen(arg) <= 32)
	{
		for (index = 0; index < strlen(arg); index++)
		{
			if ((arg[index] >= 'a') && (arg[index] <= 'z'))
			{
				arg[index] = toupper(arg[index]);
			}
		}

		for (index = 0; index < NUM_OF_COUNTRIES; index++)
		{
			memset(name_buffer, 0, 34);
			sprintf(name_buffer, "\"%s\"", allCountry[index].CountryName);

			if (strncmp(allCountry[index].CountryName, arg, strlen(arg)) == 0)
			{
				break;
			}
			else if (strncmp(name_buffer, arg, strlen(arg)) == 0)
			{
				break;
			}
		}

		if (index == NUM_OF_COUNTRIES)
		{
			DBGPRINT(RT_DEBUG_TRACE, "Currently, Not Support the Country String\n");
		}
		else
		{
			NdisZeroMemory(pAd->PortCfg.CountryCode, 3);
			NdisMoveMemory(pAd->PortCfg.CountryCode, allCountry[index].isoName, 2);
			pAd->PortCfg.CountryCode[2] = ' ';

			if (pAd->PortCfg.PhyMode == PHY_11A)
			{
				if (allCountry[index].support11a == TRUE)
				{
					pAd->PortCfg.bCountryFlag = TRUE;

					// Country can be set only when EEPROM not programmed
					if (pAd->PortCfg.CountryRegionForABand & 0x80)
					{
						DBGPRINT(RT_DEBUG_ERROR, "Set_CountryString_Proc::parameter of CountryRegion in eeprom is programmed \n");
					}
					else
					{
						pAd->PortCfg.CountryRegionForABand = (UCHAR) allCountry[index].regDomainNum11a;
						BuildChannelList(pAd);
					}					
					success = TRUE;
					
					DBGPRINT(RT_DEBUG_TRACE, "Set_CountryString_Proc::(CountryString=%s CountryRegin=%d CountryCode=%s)\n", 
												allCountry[index].CountryName, pAd->PortCfg.CountryRegionForABand, pAd->PortCfg.CountryCode);
				}
				else
				{
					DBGPRINT(RT_DEBUG_TRACE, "The Country are Not Support 11A Channel\n");
				}
			}
			else
			{
				pAd->PortCfg.bCountryFlag = TRUE;

				// Country can be set only when EEPROM not programmed
				if (pAd->PortCfg.CountryRegion & 0x80)
				{
					DBGPRINT(RT_DEBUG_ERROR, "Set_CountryString_Proc::parameter of CountryRegion in eeprom is programmed \n");
				}
				else
				{
					pAd->PortCfg.CountryRegion = (UCHAR) allCountry[index].regDomainNum11g;
					BuildChannelList(pAd);
				}
				success = TRUE;
				
				DBGPRINT(RT_DEBUG_TRACE, "Set_CountryString_Proc::(CountryString=%s CountryRegin=%d CountryCode=%s)\n", 
											allCountry[index].CountryName, pAd->PortCfg.CountryRegion, pAd->PortCfg.CountryCode);
			}
		}
	}
	else
	{
		success = FALSE;
	}

	return success;
}

/* 
    ==========================================================================
    Description:
        Set SSID
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_SSID_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg)
{
	INT   success = FALSE;

	if(strlen(arg) <= MAX_LEN_OF_SSID)
	{
		NdisZeroMemory(pAdapter->PortCfg.MBSSID[pAdapter->IoctlIF].Ssid, MAX_LEN_OF_SSID);
		NdisMoveMemory(pAdapter->PortCfg.MBSSID[pAdapter->IoctlIF].Ssid, arg, strlen(arg));
		pAdapter->PortCfg.MBSSID[pAdapter->IoctlIF].SsidLen = (UCHAR)strlen(arg);
		success = TRUE;

		// If in detection mode, need to stop detect first.
		if (pAdapter->PortCfg.RadarDetect.IEEE80211H == FALSE)
		{
			ApStop(pAdapter, TRUE);
			ApStartUp(pAdapter, TRUE);
		}
		else
		{
			// each mode has different restart method
			if (pAdapter->PortCfg.RadarDetect.RDMode == RD_SILENCE_MODE)
			{
				/* bug fixed:
				** 80211H specific that AP should keep silent for 60s and detect Radar signal when it jump to new channel.
				** set bDFSRestart to FALSE AP won't keep silent for 60s if user using SSID command to restart AP.
				*/
				ApStop(pAdapter, TRUE/*FALSE*/);
				ApStartUp(pAdapter, TRUE/*FALSE*/);
			}
			else if (pAdapter->PortCfg.RadarDetect.RDMode == RD_SWITCHING_MODE)
			{
			}
			else if (pAdapter->PortCfg.RadarDetect.RDMode == RD_NORMAL_MODE)
			{
				/* bug fixed:
				** 80211H specific that AP should keep silent for 60s and detect Radar signal when it jump to new channel.
				** set bDFSRestart to FALSE AP won't keep silent for 60s if user using SSID command to restart AP.
				*/
				ApStop(pAdapter, TRUE/*FALSE*/);
				ApStartUp(pAdapter, TRUE/*FALSE*/);
				AsicEnableBssSync(pAdapter);
			}
		}

		DBGPRINT(RT_DEBUG_TRACE, "I/F(ra%d) Set_SSID_Proc::(Len=%d,Ssid=%s)\n", pAdapter->IoctlIF,
			pAdapter->PortCfg.MBSSID[pAdapter->IoctlIF].SsidLen, pAdapter->PortCfg.MBSSID[pAdapter->IoctlIF].Ssid);
	}
	else
		success = FALSE;

	return success;
}

/* 
    ==========================================================================
    Description:
        Set Wireless Mode
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_WirelessMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG	WirelessMode;
	INT		i, success = TRUE;
	static BOOLEAN bChangeSlot = FALSE;  // add by johnli, bug fix: change phy mode will cause performance dropping

	/* In ApStartUp(), it forces to use Long SlotTime for B mode. Therefore, when it switches mode
	   to non-B mode, we have to change SlotTime back if it's changed.
	   We don't have to do the same thing on Preamble because the preamble is not set to long for B
	   mode.  It's legal to set the preamble to short on B mode, but you won't pass the Wifi test
	   if you do so. */
	 
	WirelessMode = simple_strtol(arg, 0, 10);
	
	// add by johnli
	if ((WirelessMode != PHY_11B) && (bChangeSlot == TRUE))
	{
		pAd->PortCfg.UseShortSlotTime = TRUE;
		ApUpdateCapabilityAndErpIe(pAd);
		bChangeSlot = FALSE;
	}	// end johnli

	if(WirelessMode == PHY_11A)
		pAd->PortCfg.PhyMode = PHY_11A;
	else if(WirelessMode == PHY_11B)
	{
		pAd->PortCfg.PhyMode = PHY_11B;
		// add by johnli
		if (pAd->PortCfg.UseShortSlotTime != FALSE)
		{
			pAd->PortCfg.UseShortSlotTime = FALSE;
			//ApUpdateCapabilityAndErpIe(pAd);  // mark it because SlotTime is set in ApStartup for B mode
			bChangeSlot = TRUE;
		}		// end johnli
	}
	else if(WirelessMode == PHY_11BG_MIXED)
		pAd->PortCfg.PhyMode = PHY_11BG_MIXED;
	else if(WirelessMode == PHY_11G)
		pAd->PortCfg.PhyMode = PHY_11G;
	else
		success = FALSE;

	BuildChannelList(pAd);

	for(i=0; i<pAd->PortCfg.BssidNum;i++)
	{
		RTMPSetPhyMode(pAd, pAd->PortCfg.PhyMode, i);
		RTMPBuildDesireRate(pAd, i,	pAd->PortCfg.MBSSID[i].DesiredRatesIndex);
	}

	// it is needed to set SSID to take effect
	DBGPRINT(RT_DEBUG_TRACE, "Set_WirelessMode_Proc::(=%d)\n", pAd->PortCfg.PhyMode);
	
	return success;
}

/* 
    ==========================================================================
    Description:
        Set TxRate
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_TxRate_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	NdisZeroMemory(pAd->PortCfg.MBSSID[pAd->IoctlIF].DesiredRates, MAX_LEN_OF_SUPPORTED_RATES);

	pAd->PortCfg.MBSSID[pAd->IoctlIF].DesiredRatesIndex = simple_strtol(arg, 0, 10);
	RTMPBuildDesireRate(pAd, pAd->IoctlIF, pAd->PortCfg.MBSSID[pAd->IoctlIF].DesiredRatesIndex);
	
	MlmeUpdateTxRates(pAd);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set BasicRate
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_BasicRate_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
    pAd->PortCfg.BasicRateBitMap = simple_strtol(arg, 0, 10);

    if (pAd->PortCfg.BasicRateBitMap > 4095) // (2 ^ MAX_LEN_OF_SUPPORTED_RATES) -1
        return FALSE;

    MlmeUpdateTxRates(pAd);

    DBGPRINT(RT_DEBUG_TRACE, "Set_BasicRate_Proc::(BasicRateBitMap=0x%08x)\n", pAd->PortCfg.BasicRateBitMap);
    
    return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Channel
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_Channel_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	UCHAR channel;	

	channel = (UCHAR) simple_strtol(arg, 0, 10);

	if (channel == 0)
	{	
		DBGPRINT(RT_DEBUG_ERROR, "ERROR: Can't be zero \n");
		return FALSE;
	}

	// channel which can not be considered for a certain period of time (Non-Occupancy Period)
	// after a radar signal was detected on that channel
	if ((pAd->PortCfg.PhyMode == PHY_11A) && (pAd->PortCfg.RadarDetect.IEEE80211H == TRUE))
	{
		int i;
		
		for (i = 0; i < pAd->ChannelListNum; i++)
    	{
    		if (pAd->ChannelList[i].Channel == channel)
    		{
    			if (pAd->ChannelList[i].RemainingTimeForUse > 0)
    			{
					DBGPRINT(RT_DEBUG_ERROR, "ERROR: previous detection of a radar on this channel(Channel=%d)\n", channel);
					return FALSE;
    			}
				else
				{
					DBGPRINT(RT_DEBUG_TRACE, "RemainingTimeForUse %d ,Channel %d\n", pAd->ChannelList[i].RemainingTimeForUse, channel);
				}
    		}
    	}				
	}
	
	pAd->PortCfg.Channel = channel;
	if (pAd->PortCfg.RadarDetect.IEEE80211H == FALSE)
	{
		ApStop(pAd, TRUE);
		ApStartUp(pAd, TRUE);
	}
	else
	{
		if (pAd->PortCfg.RadarDetect.RDMode == RD_SILENCE_MODE)
		{
			ApStop(pAd, TRUE);
			ApStartUp(pAd, TRUE);
		}
		else
		{
			pAd->PortCfg.RadarDetect.RDMode = RD_SWITCHING_MODE;
			pAd->PortCfg.RadarDetect.CSCount = 0;
		}
	}
	
	DBGPRINT(RT_DEBUG_TRACE, "Set_Channel_Proc::(Channel=%d)\n", pAd->PortCfg.Channel);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Beacon Period
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_BeaconPeriod_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	USHORT BeaconPeriod;
	INT   success = FALSE;

	BeaconPeriod = (USHORT) simple_strtol(arg, 0, 10);
	if((BeaconPeriod >= 20) && (BeaconPeriod < 1024))
	{
		pAd->PortCfg.BeaconPeriod = BeaconPeriod;
		success = TRUE;		
	}
	else
		success = FALSE;

	DBGPRINT(RT_DEBUG_TRACE, "Set_BeaconPeriod_Proc::(BeaconPeriod=%d)\n", pAd->PortCfg.BeaconPeriod);

	return success;
}

/* 
    ==========================================================================
    Description:
        Set Dtim Period
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_DtimPeriod_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	USHORT DtimPeriod;
	INT   success = FALSE;

	DtimPeriod = (USHORT) simple_strtol(arg, 0, 10);
	if((DtimPeriod >= 1) && (DtimPeriod <= 255))
	{
		pAd->Mlme.DtimPeriod = DtimPeriod;
		success = TRUE;
	}
	else
		success = FALSE;

	DBGPRINT(RT_DEBUG_TRACE, "Set_DtimPeriod_Proc::(DtimPeriod=%d)\n", pAd->Mlme.DtimPeriod);

	return success;
}

/* 
    ==========================================================================
    Description:
        Set Tx power
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_TxPower_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG  TxPower;
	INT   success = FALSE;

	TxPower = simple_strtol(arg, 0, 10);
	if (TxPower <= 100)
    {
		pAd->PortCfg.TxPowerPercentage = TxPower;
		success = TRUE;
	}
	else
		success = FALSE;

	DBGPRINT(RT_DEBUG_TRACE, "Set_TxPower_Proc::(TxPowerPercentage=%d)\n", pAd->PortCfg.TxPowerPercentage);

	return success;
}

#ifdef DBG
/* 
    ==========================================================================
    Description:
        For Debug information
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_Debug_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	DBGPRINT(RT_DEBUG_TRACE, "==> Set_Debug_Proc *******************\n");

    if(simple_strtol(arg, 0, 10) <= RT_DEBUG_LOUD)
        RTDebugLevel = simple_strtol(arg, 0, 10);

	DBGPRINT(RT_DEBUG_TRACE, "<== Set_Debug_Proc(RTDebugLevel = %d)\n", RTDebugLevel);

	return TRUE;
}
#endif

/* 
    ==========================================================================
    Description:
        Disable/enable OLBC detection manually
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_OLBCDetection_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	switch (simple_strtol(arg, 0, 10))
	{
		case 0: //enable OLBC detect
			pAd->PortCfg.DisableOLBCDetect = 0;
			break;
		case 1: //disable OLBC detect
			pAd->PortCfg.DisableOLBCDetect = 1;
			break;
		default:  //Invalid argument 
			return FALSE;
	}

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set 11B/11G Protection
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_BGProtection_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	switch (simple_strtol(arg, 0, 10))
	{
		case 0: //AUTO
			pAd->PortCfg.UseBGProtection = 0;
			break;
		case 1: //Always On
			pAd->PortCfg.UseBGProtection = 1;
			break;
		case 2: //Always OFF
			pAd->PortCfg.UseBGProtection = 2;
			break;		
		default:  //Invalid argument 
			return FALSE;
	}
	ApUpdateCapabilityAndErpIe(pAd);
	DBGPRINT(RT_DEBUG_TRACE, "Set_BGProtection_Proc::(BGProtection=%d)\n", pAd->PortCfg.UseBGProtection);	

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set TxAntenna
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_TxAntenna_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg)
{
	DBGPRINT(RT_DEBUG_TRACE, "Set_TxAntenna_Proc\n");	

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set RxAntenna
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_RxAntenna_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg)
{
	DBGPRINT(RT_DEBUG_TRACE, "Set_RxAntenna_Proc\n");

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set TxPreamble
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_TxPreamble_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	switch (simple_strtol(arg, 0, 10))
	{
		case Rt802_11PreambleLong:
			pAd->PortCfg.TxPreamble = Rt802_11PreambleLong;
			break;
		case Rt802_11PreambleShort:
			pAd->PortCfg.TxPreamble = Rt802_11PreambleShort;
			break;
		default: //Invalid argument 
			return FALSE;
	}

	DBGPRINT(RT_DEBUG_TRACE, "Set_TxPreamble_Proc::(TxPreamble=%d)\n", pAd->PortCfg.TxPreamble);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set RTS Threshold
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_RTSThreshold_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG	RtsThresh;

	RtsThresh = simple_strtol(arg, 0, 10);

	if((RtsThresh >= 0) && (RtsThresh <= MAX_RTS_THRESHOLD))
		pAd->PortCfg.RtsThreshold  = (USHORT)RtsThresh;
	else
		return FALSE; //Invalid argument 

	DBGPRINT(RT_DEBUG_TRACE, "Set_RTSThreshold_Proc::(RTSThreshold=%d)\n", pAd->PortCfg.RtsThreshold);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Fragment Threshold
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_FragThreshold_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG	FragThresh;

	FragThresh = simple_strtol(arg, 0, 10);

	if ((FragThresh >= MIN_FRAG_THRESHOLD) || (FragThresh <= MAX_FRAG_THRESHOLD))
	{
		pAd->PortCfg.FragmentThreshold = FragThresh;
		pAd->PortCfg.AllowFragSize = (pAd->PortCfg.FragmentThreshold) - LENGTH_802_11 - LENGTH_CRC;
		pAd->PortCfg.bFragmentZeroDisable = TRUE;
	}
	else
		return FALSE; //Invalid argument 

	DBGPRINT(RT_DEBUG_TRACE, "Set_FragThreshold_Proc::(FragThreshold=%d)\n", pAd->PortCfg.FragmentThreshold);

	return TRUE;
}


/* 
    ==========================================================================
    Description:
        Set TxQueue Size
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_TxQueueSize_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG 	QueueSize;

	QueueSize = simple_strtol(arg, 0, 10);

	if (QueueSize > 0)
	{
		pAd->PortCfg.TxQueueSize = QueueSize;
	}
	else
		return FALSE; //Invalid argument 

	DBGPRINT(RT_DEBUG_TRACE, "Set_TxQueueSize_Proc::(QueueSize=%d)\n", pAd->PortCfg.TxQueueSize);

	return TRUE;
}



/* 
    ==========================================================================
    Description:
        Set TxBurst
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_TxBurst_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG TxBurst;

	TxBurst = simple_strtol(arg, 0, 10);

	if (TxBurst == 1)
		pAd->PortCfg.bEnableTxBurst = TRUE;
	else if (TxBurst == 0)
		pAd->PortCfg.bEnableTxBurst = FALSE;
	else
		return FALSE;  //Invalid argument 
	
	DBGPRINT(RT_DEBUG_TRACE, "Set_TxBurst_Proc::(TxBurst=%d)\n", pAd->PortCfg.bEnableTxBurst);

	return TRUE;
}

#ifdef AGGREGATION_SUPPORT
/* 
    ==========================================================================
    Description:
        Set TxBurst
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_PktAggregate_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG aggre;

	aggre = simple_strtol(arg, 0, 10);

	if (aggre == 1)
		pAd->PortCfg.bAggregationCapable = TRUE;
	else if (aggre == 0)
		pAd->PortCfg.bAggregationCapable = FALSE;
	else
		return FALSE;  //Invalid argument 
	
	DBGPRINT(RT_DEBUG_TRACE, "Set_PktAggregate_Proc::(AGGRE=%d)\n", pAd->PortCfg.bAggregationCapable);

	return TRUE;
}
#endif

/* 
    ==========================================================================
    Description:
        Set TurboRate Enable or Disable
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_TurboRate_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG TurboRate;

	TurboRate = simple_strtol(arg, 0, 10);

	if (TurboRate == 1)
		pAd->PortCfg.EnableTurboRate = TRUE;
	else if (TurboRate == 0)
		pAd->PortCfg.EnableTurboRate = FALSE;
	else
		return FALSE;  //Invalid argument 
	
	DBGPRINT(RT_DEBUG_TRACE, "Set_TurboRate_Proc::(TurboRate=%d)\n", pAd->PortCfg.EnableTurboRate);

	return TRUE;
}

#ifdef WMM_SUPPORT
/* 
    ==========================================================================
    Description:
        Set WmmCapable Enable or Disable
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_WmmCapable_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	BOOLEAN	bWmmCapable;

	bWmmCapable = simple_strtol(arg, 0, 10);

	if (bWmmCapable == 1)
		pAd->PortCfg.MBSSID[pAd->IoctlIF].bWmmCapable = TRUE;
	else if (bWmmCapable == 0)
		pAd->PortCfg.MBSSID[pAd->IoctlIF].bWmmCapable = FALSE;
	else
		return FALSE;  //Invalid argument 
	
#ifdef RTL865X_FAST_PATH	
	if (!isFastPathCapable(pAd)) {
		rtlairgo_fast_tx_unregister();
		rtl865x_extDev_unregisterUcastTxDev(pAd->net_dev);		
	}
#endif

	DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_WmmCapable_Proc::(bWmmCapable=%d)\n", 
		pAd->IoctlIF, pAd->PortCfg.MBSSID[pAd->IoctlIF].bWmmCapable);

	return TRUE;
}
#endif /* WMM_SUPPORT */

/* 
    ==========================================================================
    Description:
        Set No Forwarding Enable or Disable
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_NoForwarding_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG NoForwarding;

	NoForwarding = simple_strtol(arg, 0, 10);

	if (NoForwarding == 1)
		pAd->PortCfg.MBSSID[pAd->IoctlIF].IsolateInterStaTraffic = TRUE;
	else if (NoForwarding == 0)
		pAd->PortCfg.MBSSID[pAd->IoctlIF].IsolateInterStaTraffic = FALSE;
	else
		return FALSE;  //Invalid argument 
	
	DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_NoForwarding_Proc::(NoForwarding=%d)\n", 
		pAd->IoctlIF, pAd->PortCfg.MBSSID[pAd->IoctlIF].IsolateInterStaTraffic);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set No Forwarding between each SSID
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_NoForwardingBTNSSID_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG NoForwarding;

	NoForwarding = simple_strtol(arg, 0, 10);

	if (NoForwarding == 1)
		pAd->PortCfg.IsolateInterStaTrafficBTNBSSID = TRUE;
	else if (NoForwarding == 0)
		pAd->PortCfg.IsolateInterStaTrafficBTNBSSID = FALSE;
	else
		return FALSE;  //Invalid argument 
	
	DBGPRINT(RT_DEBUG_TRACE, "Set_NoForwardingBTNSSID_Proc::(NoForwarding=%d)\n", pAd->PortCfg.IsolateInterStaTrafficBTNBSSID);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Hide SSID Enable or Disable
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_HideSSID_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	BOOLEAN bHideSsid;

	bHideSsid = simple_strtol(arg, 0, 10);

	if (bHideSsid == 1)
		bHideSsid = TRUE;
	else if (bHideSsid == 0)
		bHideSsid = FALSE;
	else
		return FALSE;  //Invalid argument 
	
	if (pAd->PortCfg.MBSSID[pAd->IoctlIF].bHideSsid != bHideSsid)
	{
		pAd->PortCfg.MBSSID[pAd->IoctlIF].bHideSsid = bHideSsid;
	}

	DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_HideSSID_Proc::(HideSSID=%d)\n", pAd->IoctlIF, pAd->PortCfg.MBSSID[pAd->IoctlIF].bHideSsid);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Short Slot Time Enable or Disable
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ShortSlot_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG ShortSlot;

	ShortSlot = simple_strtol(arg, 0, 10);

	if (ShortSlot == 1)
		pAd->PortCfg.UseShortSlotTime = TRUE;
	else if (ShortSlot == 0)
		pAd->PortCfg.UseShortSlotTime = FALSE;
	else
		return FALSE;  //Invalid argument 
	
	DBGPRINT(RT_DEBUG_TRACE, "Set_ShortSlot_Proc::(ShortSlot=%d)\n", pAd->PortCfg.UseShortSlotTime);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set IEEE8021X.
        This parameter is 1 when 802.1x-wep turn on, otherwise 0
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_IEEE8021X_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
    ULONG ieee8021x;

	ieee8021x = simple_strtol(arg, 0, 10);

	if (ieee8021x == 1)
        pAd->PortCfg.MBSSID[pAd->IoctlIF].IEEE8021X = TRUE;
	else if (ieee8021x == 0)
		pAd->PortCfg.MBSSID[pAd->IoctlIF].IEEE8021X = FALSE;
	else
		return FALSE;  //Invalid argument 
	
	DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_IEEE8021X_Proc::(IEEE8021X=%d)\n", pAd->IoctlIF, pAd->PortCfg.MBSSID[pAd->IoctlIF].IEEE8021X);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set IEEE80211H.
        This parameter is 1 when needs radar detection, otherwise 0
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_IEEE80211H_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
    ULONG ieee80211h;

	ieee80211h = simple_strtol(arg, 0, 10);

	if (ieee80211h == 1)
		pAd->PortCfg.RadarDetect.IEEE80211H = TRUE;
	else if (ieee80211h == 0)
		pAd->PortCfg.RadarDetect.IEEE80211H = FALSE;
	else
		return FALSE;  //Invalid argument 
	
	DBGPRINT(RT_DEBUG_TRACE, "Set_IEEE80211H_Proc::(IEEE80211H=%d)\n", pAd->PortCfg.RadarDetect.IEEE80211H);

	return TRUE;
}

INT	Set_RDDurRegion_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
    ULONG rdregion;

	rdregion = simple_strtol(arg, 0, 10);

	if (rdregion == 0)
		pAd->PortCfg.RadarDetect.RDDurRegion = JAP;  
	else if (rdregion == 1)
		pAd->PortCfg.RadarDetect.RDDurRegion = FCC;
	else if (rdregion == 2)
		pAd->PortCfg.RadarDetect.RDDurRegion = CE;
	else	
		return FALSE;  //Invalid argument 
	
	DBGPRINT(RT_DEBUG_TRACE, "Set_RDDurRegion_Proc::(rd region=%d)\n", pAd->PortCfg.RadarDetect.RDDurRegion);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set channel switch Period
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_CSPeriod_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	pAd->PortCfg.RadarDetect.CSPeriod = (USHORT) simple_strtol(arg, 0, 10);

	DBGPRINT(RT_DEBUG_TRACE, "Set_CSPeriod_Proc::(CSPeriod=%d)\n", pAd->PortCfg.RadarDetect.CSPeriod);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set pre-authentication enable or disable when WPA/WPA2 turn on
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_PreAuth_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
    ULONG PreAuth;

	PreAuth = simple_strtol(arg, 0, 10);

	if (PreAuth == 1)
		pAd->PortCfg.MBSSID[pAd->IoctlIF].PreAuth = TRUE;
	else if (PreAuth == 0)
		pAd->PortCfg.MBSSID[pAd->IoctlIF].PreAuth = FALSE;
	else
		return FALSE;  //Invalid argument 
	
	DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_PreAuth_Proc::(PreAuth=%d)\n", pAd->IoctlIF, pAd->PortCfg.MBSSID[pAd->IoctlIF].PreAuth);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set VLAN's ID field
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_VLANID_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	pAd->PortCfg.MBSSID[pAd->IoctlIF].VLAN_VID = simple_strtol(arg, 0, 10);
	
	DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_VLANID_Proc::(VLAN_VID=%d)\n", pAd->IoctlIF, pAd->PortCfg.MBSSID[pAd->IoctlIF].VLAN_VID);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set VLAN's priority field
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_VLANPriority_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	pAd->PortCfg.MBSSID[pAd->IoctlIF].VLAN_Priority = simple_strtol(arg, 0, 10);

	DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_VLANPriority_Proc::(VLAN_Priority=%d)\n", pAd->IoctlIF, pAd->PortCfg.MBSSID[pAd->IoctlIF].VLAN_Priority);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Authentication mode
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_AuthMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG        sec_csr4 = 0;

	if ((strcmp(arg, "WEPAUTO") == 0) || (strcmp(arg, "wepauto") == 0))
		pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode = Ndis802_11AuthModeAutoSwitch;
	else if ((strcmp(arg, "OPEN") == 0) || (strcmp(arg, "open") == 0))
		pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode = Ndis802_11AuthModeOpen;
	else if ((strcmp(arg, "SHARED") == 0) || (strcmp(arg, "shared") == 0))
		pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode = Ndis802_11AuthModeShared;
	else if ((strcmp(arg, "WPAPSK") == 0) || (strcmp(arg, "wpapsk") == 0))
		pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode = Ndis802_11AuthModeWPAPSK;
	else if ((strcmp(arg, "WPA2PSK") == 0) || (strcmp(arg, "wpa2psk") == 0))
		pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode = Ndis802_11AuthModeWPA2PSK;
	else if ((strcmp(arg, "WPA") == 0) || (strcmp(arg, "wpa") == 0))
		pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode = Ndis802_11AuthModeWPA;
	else if ((strcmp(arg, "WPA2") == 0) || (strcmp(arg, "wpa2") == 0))
		pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode = Ndis802_11AuthModeWPA2;
	else if ((strcmp(arg, "WPA1WPA2") == 0) || (strcmp(arg, "wpa1wpa2") == 0))
		pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode = Ndis802_11AuthModeWPA1WPA2;
	else if ((strcmp(arg, "WPAPSKWPA2PSK") == 0) || (strcmp(arg, "wpapskwpa2psk") == 0))
		pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode = Ndis802_11AuthModeWPAPSKWPA2PSK;
	else
		return FALSE;  

	pAd->PortCfg.MBSSID[pAd->IoctlIF].PortSecured = WPA_802_1X_PORT_NOT_SECURED;
    RTMPMakeRSNIE(pAd, pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode, pAd->PortCfg.MBSSID[pAd->IoctlIF].WepStatus, pAd->IoctlIF);

	// set key table enable bit
	RTMP_IO_READ32(pAd, SEC_CSR4, &sec_csr4);
    if (pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode >= Ndis802_11AuthModeWPA)
    {
    	sec_csr4 |= BIT32[pAd->IoctlIF];
        DBGPRINT(RT_DEBUG_TRACE, "AP AuthMode=%d, Pairwise Key Table in-used\n", pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode);
    }
    else
    {
    	sec_csr4 &= ~BIT32[pAd->IoctlIF];
        DBGPRINT(RT_DEBUG_TRACE, "AP AuthMode=%d, disable Pairwise Key Table\n", pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode);
    }
    RTMP_IO_WRITE32(pAd, SEC_CSR4, sec_csr4);

	DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_AuthMode_Proc::(AuthMode=%d, sec_csr4=0x%x)\n", pAd->IoctlIF, pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode, sec_csr4);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Encryption Type
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_EncrypType_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	if ((strcmp(arg, "NONE") == 0) || (strcmp(arg, "none") == 0))
		pAd->PortCfg.MBSSID[pAd->IoctlIF].WepStatus = Ndis802_11WEPDisabled;
	else if ((strcmp(arg, "WEP") == 0) || (strcmp(arg, "wep") == 0))
		pAd->PortCfg.MBSSID[pAd->IoctlIF].WepStatus = Ndis802_11WEPEnabled;
	else if ((strcmp(arg, "TKIP") == 0) || (strcmp(arg, "tkip") == 0))
		pAd->PortCfg.MBSSID[pAd->IoctlIF].WepStatus = Ndis802_11Encryption2Enabled;
	else if ((strcmp(arg, "AES") == 0) || (strcmp(arg, "aes") == 0))
		pAd->PortCfg.MBSSID[pAd->IoctlIF].WepStatus = Ndis802_11Encryption3Enabled;
	else if ((strcmp(arg, "TKIPAES") == 0) || (strcmp(arg, "tkipaes") == 0))
		pAd->PortCfg.MBSSID[pAd->IoctlIF].WepStatus = Ndis802_11Encryption4Enabled;
	else
		return FALSE;

    RTMPMakeRSNIE(pAd, pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode, pAd->PortCfg.MBSSID[pAd->IoctlIF].WepStatus, pAd->IoctlIF);
	DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_EncrypType_Proc::(EncrypType=%d)\n", pAd->IoctlIF, pAd->PortCfg.MBSSID[pAd->IoctlIF].WepStatus);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Default Key ID
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_DefaultKeyID_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG KeyIdx;

	KeyIdx = simple_strtol(arg, 0, 10);
	if((KeyIdx >= 1 ) && (KeyIdx <= 4))
		pAd->PortCfg.MBSSID[pAd->IoctlIF].DefaultKeyId = (UCHAR) (KeyIdx - 1 );
	else
		return FALSE;  //Invalid argument 
	
	DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_DefaultKeyID_Proc::(DefaultKeyID(0~3)=%d)\n", pAd->IoctlIF, pAd->PortCfg.MBSSID[pAd->IoctlIF].DefaultKeyId);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set WEP KEY1
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_Key1_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	INT		KeyLen;
	INT		i;
	UCHAR	CipherAlg = CIPHER_WEP64;
    UCHAR   WepKeyType = WEP_ASCII_TYPE;

	KeyLen = strlen(arg);

	switch (KeyLen)
	{
		case 5: //wep 40 Ascii type
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[0].KeyLen = KeyLen;
			memcpy(pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[0].Key, arg, KeyLen);
			CipherAlg = CIPHER_WEP64;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key1_Proc::(Key1=%s and type=%s)\n", pAd->IoctlIF, arg, "Ascii");		
			break;
		case 10: //wep 40 Hex type
			for(i=0; i < KeyLen; i++)
			{
				if( !isxdigit(*(arg+i)) )
					return FALSE;  //Not Hex value;
			}
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[0].KeyLen = KeyLen/2 ;
			AtoH(arg, pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[0].Key, KeyLen/2);
            WepKeyType = WEP_HEXADECIMAL_TYPE;
			CipherAlg = CIPHER_WEP64;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key1_Proc::(Key1=%s and type=%s)\n", pAd->IoctlIF, arg, "Hex");		
			break;
		case 13: //wep 104 Ascii type
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[0].KeyLen = KeyLen;
			memcpy(pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[0].Key, arg, KeyLen);
			CipherAlg = CIPHER_WEP128;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key1_Proc::(Key1=%s and type=%s)\n", pAd->IoctlIF, arg, "Ascii");		
			break;
		case 26: //wep 104 Hex type
			for(i=0; i < KeyLen; i++)
			{
				if( !isxdigit(*(arg+i)) )
					return FALSE;  //Not Hex value;
			}
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[0].KeyLen = KeyLen/2 ;
			AtoH(arg, pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[0].Key, KeyLen/2);
            WepKeyType = WEP_HEXADECIMAL_TYPE;
			CipherAlg = CIPHER_WEP128;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key1_Proc::(Key1=%s and type=%s)\n", pAd->IoctlIF, arg, "Hex");		
			break;
		default: //Invalid argument 
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[0].KeyLen = 0;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key1_Proc::Invalid argument (=%s)\n", pAd->IoctlIF, arg);		
			return FALSE;
	}

	pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[0].CipherAlg = CipherAlg;
    pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[0].WepKeyType = WepKeyType;

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set WEP KEY2
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_Key2_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	INT		KeyLen;
	INT		i;
	UCHAR	CipherAlg = CIPHER_WEP64;
    UCHAR   WepKeyType = WEP_ASCII_TYPE;

	KeyLen = strlen(arg);

	switch (KeyLen)
	{
		case 5: //wep 40 Ascii type
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[1].KeyLen = KeyLen;
			memcpy(pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[1].Key, arg, KeyLen);
			CipherAlg = CIPHER_WEP64;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key2_Proc::(Key2=%s and type=%s)\n", pAd->IoctlIF, arg, "Ascii");		
			break;
		case 10: //wep 40 Hex type
			for(i=0; i < KeyLen; i++)
			{
				if( !isxdigit(*(arg+i)) )
					return FALSE;  //Not Hex value;
			}
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[1].KeyLen = KeyLen/2 ;
			AtoH(arg, pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[1].Key, KeyLen/2);
            WepKeyType = WEP_HEXADECIMAL_TYPE;
			CipherAlg = CIPHER_WEP64;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key2_Proc::(Key2=%s and type=%s)\n", pAd->IoctlIF, arg, "Hex");		
			break;
		case 13: //wep 104 Ascii type
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[1].KeyLen = KeyLen;
			memcpy(pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[1].Key, arg, KeyLen);
			CipherAlg = CIPHER_WEP128;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key2_Proc::(Key2=%s and type=%s)\n", pAd->IoctlIF, arg, "Ascii");		
			break;
		case 26: //wep 104 Hex type
			for(i=0; i < KeyLen; i++)
			{
				if( !isxdigit(*(arg+i)) )
					return FALSE;  //Not Hex value;
			}
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[1].KeyLen = KeyLen/2 ;
			AtoH(arg, pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[1].Key, KeyLen/2);
            WepKeyType = WEP_HEXADECIMAL_TYPE;
			CipherAlg = CIPHER_WEP128;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key2_Proc::(Key2=%s and type=%s)\n", pAd->IoctlIF, arg, "Hex");		
			break;
		default: //Invalid argument 
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[1].KeyLen = 0;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key2_Proc::Invalid argument (=%s)\n", pAd->IoctlIF, arg);		
			return FALSE;
	}

	pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[1].CipherAlg = CipherAlg;
    pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[1].WepKeyType = WepKeyType;

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set WEP KEY3
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_Key3_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	INT		KeyLen;
	INT		i;
	UCHAR	CipherAlg = CIPHER_WEP64;
    UCHAR   WepKeyType = WEP_ASCII_TYPE;

	KeyLen = strlen(arg);

	switch (KeyLen)
	{
		case 5: //wep 40 Ascii type
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[2].KeyLen = KeyLen;
			memcpy(pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[2].Key, arg, KeyLen);
			CipherAlg = CIPHER_WEP64;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key3_Proc::(Key3=%s and type=%s)\n", pAd->IoctlIF, arg, "Ascii");		
			break;
		case 10: //wep 40 Hex type
			for(i=0; i < KeyLen; i++)
			{
				if( !isxdigit(*(arg+i)) )
					return FALSE;  //Not Hex value;
			}
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[2].KeyLen = KeyLen/2 ;
			AtoH(arg, pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[2].Key, KeyLen/2);
            WepKeyType = WEP_HEXADECIMAL_TYPE;
			CipherAlg = CIPHER_WEP64;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key3_Proc::(Key3=%s and type=%s)\n", pAd->IoctlIF, arg, "Hex");		
			break;
		case 13: //wep 104 Ascii type
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[2].KeyLen = KeyLen;
			memcpy(pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[2].Key, arg, KeyLen);
			CipherAlg = CIPHER_WEP128;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key3_Proc::(Key3=%s and type=%s)\n", pAd->IoctlIF, arg, "Ascii");		
			break;
		case 26: //wep 104 Hex type
			for(i=0; i < KeyLen; i++)
			{
				if( !isxdigit(*(arg+i)) )
					return FALSE;  //Not Hex value;
			}
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[2].KeyLen = KeyLen/2 ;
			AtoH(arg, pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[2].Key, KeyLen/2);
            WepKeyType = WEP_HEXADECIMAL_TYPE;
			CipherAlg = CIPHER_WEP128;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key3_Proc::(Key3=%s and type=%s)\n", pAd->IoctlIF, arg, "Hex");		
			break;
		default: //Invalid argument 
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[2].KeyLen = 0;
			DBGPRINT(RT_DEBUG_ERROR, "IF(ra%d) Set_Key3_Proc::Invalid argument (=%s)\n", pAd->IoctlIF, arg);		
			return FALSE;
	}

	pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[2].CipherAlg = CipherAlg;
    pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[2].WepKeyType = WepKeyType;

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set WEP KEY4
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_Key4_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	INT		KeyLen;
	INT		i;
	UCHAR	CipherAlg = CIPHER_WEP64;
    UCHAR   WepKeyType = WEP_ASCII_TYPE;

	KeyLen = strlen(arg);

	switch (KeyLen)
	{
		case 5: //wep 40 Ascii type
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[3].KeyLen = KeyLen;
			memcpy(pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[3].Key, arg, KeyLen);
			CipherAlg = CIPHER_WEP64;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key4_Proc::(Key4=%s and type=%s)\n", pAd->IoctlIF, arg, "Ascii");		
			break;
		case 10: //wep 40 Hex type
			for(i=0; i < KeyLen; i++)
			{
				if( !isxdigit(*(arg+i)) )
					return FALSE;  //Not Hex value;
			}
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[3].KeyLen = KeyLen/2 ;
			AtoH(arg, pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[3].Key, KeyLen/2);
            WepKeyType = WEP_HEXADECIMAL_TYPE;
			CipherAlg = CIPHER_WEP64;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key4_Proc::(Key4=%s and type=%s)\n", pAd->IoctlIF, arg, "Hex");		
			break;
		case 13: //wep 104 Ascii type
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[3].KeyLen = KeyLen;
			memcpy(pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[3].Key, arg, KeyLen);
			CipherAlg = CIPHER_WEP128;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key4_Proc::(Key4=%s and type=%s)\n", pAd->IoctlIF, arg, "Ascii");		
			break;
		case 26: //wep 104 Hex type
			for(i=0; i < KeyLen; i++)
			{
				if( !isxdigit(*(arg+i)) )
					return FALSE;  //Not Hex value;
			}
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[3].KeyLen = KeyLen/2 ;
			AtoH(arg, pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[3].Key, KeyLen/2);
            WepKeyType = WEP_HEXADECIMAL_TYPE;
			CipherAlg = CIPHER_WEP128;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key4_Proc::(Key4=%s and type=%s)\n", pAd->IoctlIF, arg, "Hex");		
			break;
		default: //Invalid argument 
			pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[3].KeyLen = 0;
			DBGPRINT(RT_DEBUG_ERROR, "IF(ra%d) Set_Key4_Proc::Invalid argument (=%s)\n", pAd->IoctlIF, arg);		
			return FALSE;
	}

	pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[3].CipherAlg = CipherAlg;
    pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[3].WepKeyType = WepKeyType;

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set HotSpot counter policy
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_HSCounter_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg)
{
	switch (simple_strtol(arg, 0, 10))
	{
		case 0: //Disable
			pAdapter->PortCfg.bEnableHSCounter = FALSE;
			break;
		case 1: //Enable
			pAdapter->PortCfg.bEnableHSCounter = TRUE;
			break;
		default: //Invalid argument 
			DBGPRINT(RT_DEBUG_ERROR, "Set_HSCounter_Proc::Invalid argument (=%s)\n", arg);		
			return FALSE;
	}
	
	DBGPRINT(RT_DEBUG_TRACE, "Set_HSCounter_Proc::(bEnableHSCounter=%d)\n", pAdapter->PortCfg.bEnableHSCounter);

	return TRUE;	
}

/* 
    ==========================================================================
    Description:
        Set Data Frames RetryLimit
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_RetryLimit_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg)
{
	switch (simple_strtol(arg, 0, 10))
	{
		case 0: //Short
			pAdapter->PortCfg.bRetryLimit = SHORT_RETRY;
			break;
		case 1: //Long
			pAdapter->PortCfg.bRetryLimit = LONG_RETRY;
			break;
		default: //Invalid argument 
			DBGPRINT(RT_DEBUG_ERROR, "Set_RetryLimit_Proc::Invalid argument (=%s)\n", arg);		
			return FALSE;
	}
	
	DBGPRINT(RT_DEBUG_TRACE, "Set_RetryLimit_Proc::(bRetryLimit=%d)\n", pAdapter->PortCfg.bRetryLimit);

	return TRUE;	
}


/* 
    ==========================================================================
    Description:
        Set Access ctrol policy
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_AccessPolicy_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	switch (simple_strtol(arg, 0, 10))
	{
		case 0: //Disable
			pAd->PortCfg.MBSSID[pAd->IoctlIF].AccessControlList.Policy = 0;
			break;
		case 1: //Allow All
			pAd->PortCfg.MBSSID[pAd->IoctlIF].AccessControlList.Policy = 1;
			break;
		case 2: //Reject All
			pAd->PortCfg.MBSSID[pAd->IoctlIF].AccessControlList.Policy = 2;
			break;
		default: //Invalid argument 
			DBGPRINT(RT_DEBUG_ERROR, "Set_AccessPolicy_Proc::Invalid argument (=%s)\n", arg);		
			return FALSE;
	}

	// check if ACL change affects any existent associtions
	ApUpdateAccessControlList(pAd, pAd->IoctlIF);
	
	DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_AccessPolicy_Proc::(AccessPolicy=%d)\n", pAd->IoctlIF, pAd->PortCfg.MBSSID[pAd->IoctlIF].AccessControlList.Policy);

	return TRUE;	
}

/* 
    ==========================================================================
    Description:
        Set Access ctrol mac table list
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_AccessControlList_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	UCHAR					macAddr[MAC_ADDR_LEN];
	RT_802_11_ACL			acl;
	CHAR					*this_char;
	CHAR					*value;
	INT						i, j;
	BOOLEAN					isDuplicat=FALSE;

	memset(&acl, 0x00, sizeof(RT_802_11_ACL));

	while ((this_char = strsep((char **)&arg, ";")) != NULL) 
	{
		if(strlen(this_char) != 17)  //Mac address acceptable format 01:02:03:04:05:06 length 17
			return FALSE;

         for (i=0, value = rstrtok(this_char,":"); value; value = rstrtok(NULL,":")) 
		 {
			if((strlen(value) != 2) || (!isxdigit(*value)) || (!isxdigit(*(value+1))) ) 
				return FALSE;  //Invalid

			AtoH(value, &macAddr[i++], 2);
		 }

		if(i != 6)
			return FALSE;  //Invalid

		//Check is duplicate
		isDuplicat = FALSE;
		for(j=0; j<acl.Num; j++)
			if(memcmp(acl.Entry[j].Addr, &macAddr, 6) == 0)
				isDuplicat = TRUE;

		if(!isDuplicat)
			memcpy(acl.Entry[acl.Num++].Addr, &macAddr, 6);

		if(acl.Num >= MAX_NUM_OF_ACL_LIST)
			break;
	}

	acl.Policy = pAd->PortCfg.MBSSID[pAd->IoctlIF].AccessControlList.Policy;
	memcpy(&pAd->PortCfg.MBSSID[pAd->IoctlIF].AccessControlList, &acl, sizeof(RT_802_11_ACL));
	if (pAd->PortCfg.MBSSID[pAd->IoctlIF].AccessControlList.Num > MAX_NUM_OF_ACL_LIST)
		pAd->PortCfg.MBSSID[pAd->IoctlIF].AccessControlList.Num = MAX_NUM_OF_ACL_LIST;
	else
		pAd->PortCfg.MBSSID[pAd->IoctlIF].AccessControlList.Num = acl.Num;

	// check if ACL change affects any existent associtions
	ApUpdateAccessControlList(pAd, pAd->IoctlIF);

	DBGPRINT(RT_DEBUG_TRACE, "Set::Set_AccessControlList_Proc(Policy=%d, Entry#=%d\n",
        pAd->PortCfg.MBSSID[pAd->IoctlIF].AccessControlList.Policy, pAd->PortCfg.MBSSID[pAd->IoctlIF].AccessControlList.Num);

#ifdef DBG
	DBGPRINT(RT_DEBUG_TRACE, "=============== Entry ===============\n");
	for(i=0; i<pAd->PortCfg.MBSSID[pAd->IoctlIF].AccessControlList.Num; i++)
	{
		printk("Entry #%02d: ", i+1);
		for(j=0; j<6; j++)
		   printk("%02X ", pAd->PortCfg.MBSSID[pAd->IoctlIF].AccessControlList.Entry[i].Addr[j]);
		printk("\n");
	}
#endif
	return TRUE;
}

#ifdef DBG
static void _rtmp_hexdump(int level, const char *title, const u8 *buf,
			 size_t len, int show)
{
	size_t i;
	if (level < RTDebugLevel)
		return;
	printk("%s - hexdump(len=%lu):", title, (unsigned long) len);
	if (show) {
		for (i = 0; i < len; i++)
			printk(" %02x", buf[i]);
	} else {
		printk(" [REMOVED]");
	}
	printk("\n");
}

void rtmp_hexdump(int level, const char *title, const u8 *buf, size_t len)
{
	_rtmp_hexdump(level, title, buf, len, 1);
}
#endif

/* 
    ==========================================================================
    Description:
        Set WPA PSK key

    Arguments:
        pAdapter            Pointer to our adapter
        arg                 WPA pre-shared key string

    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_WPAPSK_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PUCHAR			arg)
{
	UCHAR       keyMaterial[40];

	DBGPRINT(RT_DEBUG_TRACE, "Set_WPAPSK_Proc::(WPAPSK=%s)\n", arg);

	if ((strlen(arg) < 8) || (strlen(arg) > 64))
	{
		DBGPRINT(RT_DEBUG_TRACE, "Set failed!!(WPAPSK=%s), WPAPSK key-string required 8 ~ 64 characters \n", arg);
		return FALSE;
	}

	if (strlen(arg) == 64)
	{
	    AtoH(arg, pAdapter->PortCfg.MBSSID[pAdapter->IoctlIF].PMK, 32);
	}
	else
	{
	    PasswordHash((CHAR *)arg, pAdapter->PortCfg.MBSSID[pAdapter->IoctlIF].Ssid, pAdapter->PortCfg.MBSSID[pAdapter->IoctlIF].SsidLen, keyMaterial);
	    NdisMoveMemory(pAdapter->PortCfg.MBSSID[pAdapter->IoctlIF].PMK, keyMaterial, 32);
	}

#ifdef WSC_SUPPORT
    NdisZeroMemory(pAdapter->PortCfg.MBSSID[pAdapter->IoctlIF].WscControl.WpaPsk, 64);
    pAdapter->PortCfg.MBSSID[pAdapter->IoctlIF].WscControl.WpaPskLen = 0;
    if ((strlen(arg) >= 8) && (strlen(arg) <= 64))
    {                                    
        NdisMoveMemory(pAdapter->PortCfg.MBSSID[pAdapter->IoctlIF].WscControl.WpaPsk, arg, strlen(arg));
        pAdapter->PortCfg.MBSSID[pAdapter->IoctlIF].WscControl.WpaPskLen = strlen(arg);
    }
#endif // WSC_SUPPORT //

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Reset statistics counter

    Arguments:
        pAdapter            Pointer to our adapter
        arg                 

    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ResetStatCounter_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
    UCHAR           i;
    MAC_TABLE_ENTRY *pEntry;
    
    DBGPRINT(RT_DEBUG_TRACE, "==>Set_ResetStatCounter_Proc\n");

    // add the most up-to-date h/w raw counters into software counters
    NICUpdateRawCounters(pAd);
    
    NdisZeroMemory(&pAd->WlanCounters, sizeof(COUNTER_802_11));
    NdisZeroMemory(&pAd->Counters8023, sizeof(COUNTER_802_3));
    NdisZeroMemory(&pAd->RalinkCounters, sizeof(COUNTER_RALINK));

    // Reset HotSpot counter
    for (i = 0; i < MAX_LEN_OF_MAC_TABLE; i++)
    {
        pEntry = &pAd->MacTab.Content[i];

        if ((pEntry->Valid == FALSE) || (pEntry->Sst != SST_ASSOC))
            continue;

        pEntry->HSCounter.LastDataPacketTime = 0;
        pEntry->HSCounter.TotalRxByteCount= 0;
        pEntry->HSCounter.TotalTxByteCount= 0;
    }
    
    return TRUE;
}

INT	Set_RadioOn_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	UCHAR radio;

	radio = simple_strtol(arg, 0, 10);

	if (radio)
	{
		MlmeRadioOn(pAd);
		DBGPRINT(RT_DEBUG_TRACE, "==>Set_RadioOn_Proc (ON)\n");
	}
	else
	{
		MlmeRadioOff(pAd);
		DBGPRINT(RT_DEBUG_TRACE, "==>Set_RadioOn_Proc (OFF)\n");
	}
	
	return TRUE;
}

INT	Set_dfstest_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	pAd->dfstest = simple_strtol(arg, 0, 10);

	DBGPRINT(RT_DEBUG_TRACE, "==>Set_dlstest_Proc (dlstest=%d)\n", pAd->dfstest);
	
	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Group Rekey interval type 

    Arguments:
        pAdapter            Pointer to our adapter
        arg                 

    Return:
          
    ==========================================================================
*/
INT	Set_RekeyMethod_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
    if ((strcmp(arg, "TIME") == 0) || (strcmp(arg, "time") == 0))
        pAd->PortCfg.WPAREKEY.ReKeyMethod = TIME_REKEY;
    else if ((strcmp(arg, "PKT") == 0) || (strcmp(arg, "pkt") == 0))
        pAd->PortCfg.WPAREKEY.ReKeyMethod = PKT_REKEY;
    else if ((strcmp(arg, "NONE") == 0) || (strcmp(arg, "none") == 0))
    {
        pAd->PortCfg.WPAREKEY.ReKeyMethod = DISABLE_REKEY;
        pAd->PortCfg.WPAREKEY.ReKeyInterval = 0;
    }
    else
        return FALSE;
    
    if ((pAd->PortCfg.REKEYTimerRunning == FALSE) && (pAd->PortCfg.WPAREKEY.ReKeyInterval != 0)
        && (pAd->PortCfg.WPAREKEY.ReKeyMethod != DISABLE_REKEY))
    {
		RTMPAddTimer(&pAd->PortCfg.REKEYTimer, GUPDATE_EXEC_INTV);
        pAd->PortCfg.REKEYTimerRunning = TRUE;
        pAd->PortCfg.REKEYCOUNTER = 0;
    }
    DBGPRINT(RT_DEBUG_TRACE, "Set_RekeyMethod_Proc::(RekeyMethod=%d)\n", pAd->PortCfg.WPAREKEY.ReKeyMethod);
    
    return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Group Rekey interval

    Arguments:
        pAdapter            Pointer to our adapter
        arg                 

    Return:
          
    ==========================================================================
*/
INT	Set_RekeyInterval_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
    ULONG    period;
    
    period = simple_strtol(arg, 0, 10);
    if ((pAd->PortCfg.REKEYTimerRunning == TRUE) && (period == 0))
    {
        RTMPCancelTimer(&pAd->PortCfg.REKEYTimer);
        pAd->PortCfg.WPAREKEY.ReKeyInterval = 0;
        pAd->PortCfg.REKEYTimerRunning = FALSE;
    }
    else if (period <= MAX_REKEY_INTER)
    {
        pAd->PortCfg.WPAREKEY.ReKeyInterval = period;
        if (pAd->PortCfg.REKEYTimerRunning == FALSE)
        {
			RTMPAddTimer(&pAd->PortCfg.REKEYTimer, GUPDATE_EXEC_INTV);
            pAd->PortCfg.REKEYTimerRunning = TRUE;
            pAd->PortCfg.REKEYCOUNTER = 0;
        }
    }
    else
        return FALSE;

    DBGPRINT(RT_DEBUG_TRACE, "Set_RekeyInterval_Proc::(RekeyInterval=%d)\n", period);
    return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set time period for WPA-PMK

    Arguments:
        pAdapter            Pointer to our adapter
        arg                 

    Return:
          
    ==========================================================================
*/
INT	Set_PMKCachePeriod_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
    pAd->PortCfg.PMKCachePeriod = simple_strtol(arg, 0, 10) * 60 * HZ;
    
    DBGPRINT(RT_DEBUG_TRACE, "Set_PMKCachePeriod_Proc::(PMKCachePeriod=%d)\n", pAd->PortCfg.PMKCachePeriod);
    return TRUE;
}

/* 
    ==========================================================================
    Description:
        Issue a site survey command to driver
	Arguments:
	    pAdapter                    Pointer to our adapter
	    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage: 
               1.) iwpriv ra0 set site_survey
    ==========================================================================
*/
INT Set_SiteSurvey_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
    ApSiteSurvey(pAd);
    
    DBGPRINT(RT_DEBUG_TRACE, "Set_SiteSurvey_Proc\n");

    return TRUE;
}

#ifdef CARRIER_DETECTION_SUPPORT
/* 
    ==========================================================================
    Description:
        Enable or Disable Carrier Detection feature.
	Arguments:
	    pAdapter                    Pointer to our adapter
	    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage: 
               1.) iwpriv ra0 set CarrierDetect=[1/0]
    ==========================================================================
*/
static INT Set_CarrierDetect_Proc(
	IN PRTMP_ADAPTER pAd, 
	IN PUCHAR arg)
{
	BOOLEAN CarrierSenseCts;
	UINT Enable;

	if (pAd->IoctlIF != MAIN_MBSSID)
		return FALSE;

	Enable = simple_strtol(arg, 0, 10);

	pAd->PortCfg.CarrierDetect.Enable = (BOOLEAN)(Enable == 0 ? FALSE : TRUE);

	if (pAd->PortCfg.RadarDetect.RDMode != RD_NORMAL_MODE)
		CarrierSenseCts = 0;
	else
		CarrierSenseCts = 1;

	if (pAd->PortCfg.CarrierDetect.Enable == TRUE)
		CarrierDetectionStart(pAd, CarrierSenseCts);
	else
		CarrierDetectionStop(pAd);

	DBGPRINT(RT_DEBUG_TRACE, "%s:: %s\n", __FUNCTION__,
		pAd->PortCfg.CarrierDetect.Enable == TRUE ? "Enable Carrier Detection":"Disable Carrier Detection");

	return TRUE;
}
#endif // CARRIER_DETECTION_SUPPORT //

/* 
    ==========================================================================
    Description:
        UI should not call this function, it only used by 802.1x daemon
	Arguments:
	    pAd		Pointer to our adapter
	    wrq		Pointer to the ioctl argument
    ==========================================================================
*/
VOID RTMPIoctlAddPMKIDCache(
	IN	PRTMP_ADAPTER	pAd, 
	IN	struct iwreq	*wrq)
{
	NDIS_802_11_KEY *pKey = (PNDIS_802_11_KEY) wrq->u.data.pointer;
    
    if (pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode >= Ndis802_11AuthModeWPA2)
	{
		if(pKey->KeyLength == 32)
		{
			UCHAR	digest[80], PMK_key[20], macaddr[MAC_ADDR_LEN];
			
			// Calculate PMKID
			NdisMoveMemory(&PMK_key[0], "PMK Name", 8);
			NdisMoveMemory(&PMK_key[8], pAd->PortCfg.MBSSID[pAd->IoctlIF].Bssid, MAC_ADDR_LEN);
			NdisMoveMemory(&PMK_key[14], pKey->addr, MAC_ADDR_LEN);
			HMAC_SHA1(PMK_key, 20, pKey->KeyMaterial, PMK_LEN, digest);

			NdisMoveMemory(macaddr, pKey->addr, MAC_ADDR_LEN);
			RTMPAddPMKIDCache(pAd, pAd->IoctlIF, macaddr, digest, pKey->KeyMaterial);
			
			DBGPRINT(RT_DEBUG_TRACE, "WPA2(pre-auth):(%02x:%02x:%02x:%02x:%02x:%02x)Calc PMKID=%02x:%02x:%02x:%02x:%02x:%02x\n", 
				pKey->addr[0],pKey->addr[1],pKey->addr[2],pKey->addr[3],pKey->addr[4],pKey->addr[5],digest[0],digest[1],digest[2],digest[3],digest[4],digest[5]);
			DBGPRINT(RT_DEBUG_TRACE, "PMK =%02x:%02x:%02x:%02x-%02x:%02x:%02x:%02x\n",pKey->KeyMaterial[0],pKey->KeyMaterial[1],
				pKey->KeyMaterial[2],pKey->KeyMaterial[3],pKey->KeyMaterial[4],pKey->KeyMaterial[5],pKey->KeyMaterial[6],pKey->KeyMaterial[7]);
		}
		else
            DBGPRINT(RT_DEBUG_ERROR, "Set::RT_OID_802_11_WPA2_ADD_PMKID_CACHE ERROR or is wep key \n");
	}
    
    DBGPRINT(RT_DEBUG_TRACE, "<== RTMPIoctlAddPMKIDCache\n");
}

/* 
    ==========================================================================
    Description:
        UI should not call this function, it only used by 802.1x daemon
	Arguments:
	    pAd		Pointer to our adapter
	    wrq		Pointer to the ioctl argument
    ==========================================================================
*/
VOID RTMPIoctlRadiusData(
	IN	PRTMP_ADAPTER	pAd, 
	IN	struct iwreq	*wrq)
{
	DBGPRINT(RT_DEBUG_INFO, "RTMPIoctlRadiusData, IF(ra%d)\n", pAd->IoctlIF);
	
	if ((pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode == Ndis802_11AuthModeWPA) 
    	|| (pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode == Ndis802_11AuthModeWPA2)
    	|| (pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode == Ndis802_11AuthModeWPA1WPA2) 
    	|| (pAd->PortCfg.MBSSID[pAd->IoctlIF].IEEE8021X == TRUE))
    	WpaSend(pAd, wrq->u.data.pointer, wrq->u.data.length);
}

/* 
    ==========================================================================
    Description:
        UI should not call this function, it only used by 802.1x daemon
	Arguments:
	    pAd		Pointer to our adapter
	    wrq		Pointer to the ioctl argument
    ==========================================================================
*/
VOID RTMPIoctlAddWPAKey(
	IN	PRTMP_ADAPTER	pAd, 
	IN	struct iwreq	*wrq)
{
	NDIS_802_11_KEY *pKey;
	ULONG			KeyIdx;
	MAC_TABLE_ENTRY  *pEntry;
		
    DBGPRINT(RT_DEBUG_TRACE, "RTMPIoctlAddWPAKey-IF(ra%d)\n", pAd->IoctlIF);

	pKey = (PNDIS_802_11_KEY) wrq->u.data.pointer;

	if (pAd->PortCfg.MBSSID[pAd->IoctlIF].AuthMode >= Ndis802_11AuthModeWPA)
	{
		if(pKey->KeyLength == 32)
		{
			NdisMoveMemory(pAd->PortCfg.MBSSID[pAd->IoctlIF].PMK, pKey->KeyMaterial, 32);
            DBGPRINT(RT_DEBUG_TRACE, "After ADD_KEY : PMK=%02x:%02x:%02x:%02x-%02x:%02x:%02x:%02x\n",
            	pAd->PortCfg.MBSSID[pAd->IoctlIF].PMK[0],pAd->PortCfg.MBSSID[pAd->IoctlIF].PMK[1],pAd->PortCfg.MBSSID[pAd->IoctlIF].PMK[2],pAd->PortCfg.MBSSID[pAd->IoctlIF].PMK[3],
            	pAd->PortCfg.MBSSID[pAd->IoctlIF].PMK[4],pAd->PortCfg.MBSSID[pAd->IoctlIF].PMK[5],pAd->PortCfg.MBSSID[pAd->IoctlIF].PMK[6],pAd->PortCfg.MBSSID[pAd->IoctlIF].PMK[7]);
		}
		else
            DBGPRINT(RT_DEBUG_TRACE, "RTMPIoctlAddWPAKey ERROR \n");
	}
	else	// Old WEP stuff
	{
		if(pKey->KeyLength == 32)
			return;
		
		KeyIdx = pKey->KeyIndex & 0x0fffffff;

		if (KeyIdx < 4)
		{
			// it is a shared key
			if (pKey->KeyIndex & 0x80000000)
			{
				// Default key for tx (shared key)
				pAd->PortCfg.MBSSID[pAd->IoctlIF].DefaultKeyId = (UCHAR) KeyIdx;								
                     
				// set key material and key length
				pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[KeyIdx].KeyLen = (UCHAR) pKey->KeyLength;
				NdisMoveMemory(pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[KeyIdx].Key, &pKey->KeyMaterial, pKey->KeyLength);
				
				// Set Ciper type
				if (pKey->KeyLength == 5)
					pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[KeyIdx].CipherAlg = CIPHER_WEP64;
				else
					pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[KeyIdx].CipherAlg = CIPHER_WEP128;
			
				// Set shared key to Asic
				AsicAddSharedKeyEntry(
						pAd,
						pAd->IoctlIF,
						KeyIdx,
						pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[KeyIdx].CipherAlg,
						pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[KeyIdx].Key,
						NULL,
						NULL);
												
			}
			else	// For Pairwise key setting
			{
				pEntry = MacTableLookup(pAd, pKey->addr);

				if (pEntry)
				{
					// set key material and key length
 					pEntry->PairwiseKey.KeyLen = (UCHAR)pKey->KeyLength;
					NdisMoveMemory(pEntry->PairwiseKey.Key, &pKey->KeyMaterial, pKey->KeyLength);
					
					// set Cipher type
					if (pKey->KeyLength == 5)
						pEntry->PairwiseKey.CipherAlg = CIPHER_WEP64;
					else
						pEntry->PairwiseKey.CipherAlg = CIPHER_WEP128;
						
					AsicAddPairwiseKeyEntry(
											pAd, 
											pEntry->Addr, 
											(UCHAR)pEntry->Aid,
											pEntry->PairwiseKey.CipherAlg, 
											pEntry->PairwiseKey.Key,
											NULL, 
											NULL);

					pAd->PortCfg.MBSSID[pAd->IoctlIF].IndividualWepKeyIdx = (UCHAR) KeyIdx;
					pEntry->bDot1xDynamicWep = TRUE;
				}	
			}
		}
	}
}

/* 
    ==========================================================================
    Description:
        UI should not call this function, it only used by 802.1x daemon
	Arguments:
	    pAd		Pointer to our adapter
	    wrq		Pointer to the ioctl argument
    ==========================================================================
*/
VOID RTMPIoctlStaticWepCopy(
	IN	PRTMP_ADAPTER	pAd, 
	IN	struct iwreq	*wrq)
{
	MAC_TABLE_ENTRY  *pEntry;
	UCHAR	MacAddr[MAC_ADDR_LEN];
		
    DBGPRINT(RT_DEBUG_TRACE, "RTMPIoctlStaticWepCopy-IF(ra%d)\n", pAd->IoctlIF);

    if (wrq->u.data.length != sizeof(MacAddr))
    {
        DBGPRINT(RT_DEBUG_ERROR, "RTMPIoctlStaticWepCopy: the length isn't match (%d)\n", wrq->u.data.length);
        return;
    }
    else
    {
        copy_from_user(&MacAddr, wrq->u.data.pointer, wrq->u.data.length);    
        pEntry = MacTableLookup(pAd, MacAddr);
        if (!pEntry)
        {
            DBGPRINT(RT_DEBUG_ERROR, "RTMPIoctlStaticWepCopy: the mac address isn't match\n");
            return;
        }
        else
        {
            UCHAR	KeyIdx;
            
            KeyIdx = pAd->PortCfg.MBSSID[pAd->IoctlIF].DefaultKeyId;
            
            //need to copy the default shared-key to pairwise key table for this entry in 802.1x mode
			if (pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[KeyIdx].KeyLen == 0)
			{
				DBGPRINT(RT_DEBUG_ERROR, "ERROR: Can not get Default shared-key (index-%d)\n", KeyIdx);
				return;
			}
			else
        	{
            	pEntry->PairwiseKey.KeyLen = pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[KeyIdx].KeyLen;
            	NdisMoveMemory(pEntry->PairwiseKey.Key, pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[KeyIdx].Key, pEntry->PairwiseKey.KeyLen);
            	pEntry->PairwiseKey.CipherAlg = pAd->PortCfg.MBSSID[pAd->IoctlIF].SharedKey[KeyIdx].CipherAlg;
			
            	AsicAddPairwiseKeyEntry(
                	pAd, 
                	pEntry->Addr, 
                	(UCHAR)pEntry->Aid,
                	pEntry->PairwiseKey.CipherAlg, 
                	pEntry->PairwiseKey.Key,
                	NULL, 
                	NULL);
        	}
			
        }
	}
    return;
}

#ifdef DBG
/* 
    ==========================================================================
    Description:
        Read / Write BBP
Arguments:
    pAdapter                    Pointer to our adapter
    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage: 
               1.) iwpriv ra0 bbp               ==> read all BBP
               2.) iwpriv ra0 bbp 1             ==> read BBP where RegID=1
               3.) iwpriv ra0 bbp 1=10		    ==> write BBP R1=0x10
    ==========================================================================
*/
VOID RTMPIoctlBBP(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq)
{
	CHAR				*this_char;
	CHAR				*value;
	UCHAR				regBBP;
	CHAR				*mpool, *msg; //msg[2048];
	CHAR				*arg; //arg[255];
	CHAR				*ptr;
	INT					bbpId;
	INT					bbpValue;
	BOOLEAN				bIsPrintAllBBP = FALSE;

	DBGPRINT(RT_DEBUG_INFO, "==>RTMPIoctlBBP\n");

	mpool = (CHAR *) kmalloc(sizeof(CHAR)*(2048+256+12), MEM_ALLOC_FLAG);

	if (mpool == NULL) {
		return;
	}

	msg = (CHAR *)((UMAXLONG)(mpool+3) & (UMAXLONG)~0x03);
	arg = (CHAR *)((UMAXLONG)(msg+2048+3) & (UMAXLONG)~0x03);

	memset(msg, 0x00, 2048);
	if (wrq->u.data.length > 1) //No parameters.
	{
		memcpy(arg, wrq->u.data.pointer, (wrq->u.data.length > 255) ? 255 : wrq->u.data.length);
		ptr = arg;
		sprintf(msg, "\n");
		//Parsing Read or Write
		while ((this_char = strsep(&ptr, ",")) != NULL)
		{
			DBGPRINT(RT_DEBUG_INFO, "this_char=%s\n", this_char);
			if (!*this_char)
				continue;

			if ((value = strchr(this_char, '=')) != NULL)
				*value++ = 0;

			if (!value || !*value)
			{ //Read
				DBGPRINT(RT_DEBUG_INFO, "this_char=%s, value=%s\n", this_char, value);
				if (sscanf(this_char, "%d", &(bbpId)) == 1)
				{
					if (bbpId <= 108)
					{
						RTMP_BBP_IO_READ8_BY_REG_ID(pAdapter, bbpId, &regBBP);
						sprintf(msg+strlen(msg), "R%02d[0x%02X]:%02X  ", bbpId, bbpId*2, regBBP);
						DBGPRINT(RT_DEBUG_INFO, "msg=%s\n", msg);
					}
					else
					{//Invalid parametes, so default printk all bbp
						bIsPrintAllBBP = TRUE;
						break;
					}
				}
				else
				{ //Invalid parametes, so default printk all bbp
					bIsPrintAllBBP = TRUE;
					break;
				}
			}
			else
			{ //Write
				DBGPRINT(RT_DEBUG_INFO, "this_char=%s, value=%s\n", this_char, value);
				if ((sscanf(this_char, "%d", &(bbpId)) == 1) && (sscanf(value, "%x", &(bbpValue)) == 1))
				{
					DBGPRINT(RT_DEBUG_INFO, "bbpID=%02d, value=0x%x\n", bbpId, bbpValue);
					if (bbpId <= 108)
					{
						RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, (UCHAR)bbpId,(UCHAR) bbpValue);
						//Read it back for showing
						RTMP_BBP_IO_READ8_BY_REG_ID(pAdapter, bbpId, &regBBP);
						sprintf(msg+strlen(msg), "R%02d[0x%02X]:%02X\n", bbpId, bbpId*2, regBBP);
						DBGPRINT(RT_DEBUG_INFO, "msg=%s\n", msg);
					}
					else
					{//Invalid parametes, so default printk all bbp
						bIsPrintAllBBP = TRUE;
						break;
					}
				}
				else
				{ //Invalid parametes, so default printk all bbp
					bIsPrintAllBBP = TRUE;
					break;
				}
			}
		}
	}
	else
		bIsPrintAllBBP = TRUE;

	if (bIsPrintAllBBP)
	{
		memset(msg, 0x00, 2048);
		sprintf(msg, "\n");
		for (bbpId = 0; bbpId <= 108; bbpId++)
		{
			RTMP_BBP_IO_READ8_BY_REG_ID(pAdapter, bbpId, &regBBP);
			sprintf(msg+strlen(msg), "R%02d[0x%02X]:%02X    ", bbpId, bbpId*2, regBBP);
			if (bbpId%5 == 4)
				sprintf(msg+strlen(msg), "\n");
		}
		// Copy the information into the user buffer
		DBGPRINT(RT_DEBUG_TRACE, "strlen(msg)=%d\n", strlen(msg));
		wrq->u.data.length = strlen(msg);
		if (copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length)) 
		{
			DBGPRINT(RT_DEBUG_TRACE, "%s: copy_to_user() fail\n", __FUNCTION__);			
		}
	}
	else
	{
		DBGPRINT(RT_DEBUG_INFO, "copy to user [msg=%s]\n", msg);
		// Copy the information into the user buffer
		DBGPRINT(RT_DEBUG_INFO, "strlen(msg) =%d\n", strlen(msg));
		wrq->u.data.length = strlen(msg);
		if (copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length))
		{
			DBGPRINT(RT_DEBUG_TRACE, "%s: copy_to_user() fail\n", __FUNCTION__);			
		}

	}

	kfree(mpool);
	DBGPRINT(RT_DEBUG_TRACE, "<==RTMPIoctlBBP\n\n");
}

/* 
    ==========================================================================
    Description:
        Read / Write MAC
Arguments:
    pAdapter                    Pointer to our adapter
    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage: 
               1.) iwpriv ra0 mac 0        ==> read MAC where Addr=0x0
               2.) iwpriv ra0 mac 0=12     ==> write MAC where Addr=0x0, value=12
    ==========================================================================
*/
VOID RTMPIoctlMAC(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	struct iwreq	*wrq)
{
	CHAR				*this_char;
	CHAR				*value;
	INT					j = 0, k = 0;
	CHAR				*mpool, *msg; //msg[1024];
	CHAR				*arg; //arg[255];
	CHAR				*ptr;
	ULONG 				macAddr = 0;
	UCHAR				temp[16], temp2[16];
	ULONG 				macValue;

	DBGPRINT(RT_DEBUG_INFO, "==>RTMPIoctlMAC\n");

	mpool = (CHAR *) kmalloc(sizeof(CHAR)*(2048+256+12), MEM_ALLOC_FLAG);

	if (mpool == NULL) {
		return;
	}

	msg = (CHAR *)((UMAXLONG)(mpool+3) & (UMAXLONG)~0x03);
	arg = (CHAR *)((UMAXLONG)(msg+1024+3) & (UMAXLONG)~0x03);

	memset(msg, 0x00, 1024);
	if (wrq->u.data.length > 1) //No parameters.
	{
		memcpy(arg, wrq->u.data.pointer, (wrq->u.data.length > 255) ? 255 : wrq->u.data.length);
		ptr = arg;
		sprintf(msg, "\n");
		//Parsing Read or Write
		while ((this_char = strsep(&ptr, ",")) != NULL)
		{
			DBGPRINT(RT_DEBUG_INFO, "this_char=%s\n", this_char);
			if (!*this_char)
				continue;

			if ((value = strchr(this_char, '=')) != NULL)
				*value++ = 0;

			if (!value || !*value)
			{ //Read
				DBGPRINT(RT_DEBUG_INFO, "Read: this_char=%s, strlen=%d\n", this_char, strlen(this_char));

				// Sanity check
				if(strlen(this_char) > 4)
					break;

				j = strlen(this_char);
				while(j-- > 0)
				{
					if(this_char[j] > 'f' || this_char[j] < '0')
						goto done; //return;
				}

				// Mac Addr
				k = j = strlen(this_char);
				while(j-- > 0)
				{
					this_char[4-k+j] = this_char[j];
				}
				
				while(k < 4)
					this_char[3-k++]='0';
				this_char[4]='\0';

				if(strlen(this_char) == 4)
				{
					AtoH(this_char, temp, 4);
					macAddr = *temp*256 + temp[1];					
					if (macAddr < 0xFFFF)
					{
						RTMP_IO_READ32(pAdapter, macAddr, &macValue);
						DBGPRINT(RT_DEBUG_TRACE, "MacAddr=%x, MacValue=%x\n", macAddr, macValue);
						sprintf(msg+strlen(msg), "[0x%08X]:%08X  ", macAddr , macValue);
						DBGPRINT(RT_DEBUG_INFO, "msg=%s\n", msg);
					}
					else
					{//Invalid parametes, so default printk all bbp
						break;
					}
				}
			}
			else
			{ //Write
				DBGPRINT(RT_DEBUG_INFO, "Write: this_char=%s, strlen(value)=%d, value=%s\n", this_char, strlen(value), value);
				memcpy(&temp2, value, strlen(value));
				temp2[strlen(value)] = '\0';

				// Sanity check
				if((strlen(this_char) > 4) || strlen(temp2) > 8)
					break;

				j = strlen(this_char);
				while(j-- > 0)
				{
					if(this_char[j] > 'f' || this_char[j] < '0')
						goto done; //return;
				}

				j = strlen(temp2);
				while(j-- > 0)
				{
					if(temp2[j] > 'f' || temp2[j] < '0')
						goto done; //return;
				}

				//MAC Addr
				k = j = strlen(this_char);
				while(j-- > 0)
				{
					this_char[4-k+j] = this_char[j];
				}

				while(k < 4)
					this_char[3-k++]='0';
				this_char[4]='\0';

				//MAC value
				k = j = strlen(temp2);
				while(j-- > 0)
				{
					temp2[8-k+j] = temp2[j];
				}
				
				while(k < 8)
					temp2[7-k++]='0';
				temp2[8]='\0';

				{
					AtoH(this_char, temp, 4);
					macAddr = *temp*256 + temp[1];

					AtoH(temp2, temp, 8);
					macValue = *temp*256*256*256 + temp[1]*256*256 + temp[2]*256 + temp[3];

					// debug mode
					if (macAddr == (HW_DEBUG_SETTING_BASE + 4))
					{
						// 0x2bf4: byte0 non-zero: enable R17 tuning, 0: disable R17 tuning
                        if (macValue & 0x000000ff) 
                        {
                            pAdapter->BbpTuning.bEnable = TRUE;
                            DBGPRINT(RT_DEBUG_TRACE,"turn on R17 tuning\n");
                        }
                        else
                        {
                            UCHAR R17;
                            pAdapter->BbpTuning.bEnable = FALSE;
                            if (pAdapter->PortCfg.Channel > 14)
                                R17 = pAdapter->BbpTuning.R17LowerBoundA;
                            else
                                R17 = pAdapter->BbpTuning.R17LowerBoundG;
                            RTMP_WRITE_BBP_R17(pAdapter, R17);
                            DBGPRINT(RT_DEBUG_TRACE,"turn off R17 tuning, restore to 0x%02x\n", R17);
                        }
						return;
					}

					DBGPRINT(RT_DEBUG_TRACE, "MacAddr=%02x, MacValue=0x%x\n", macAddr, macValue);
					
					RTMP_IO_WRITE32(pAdapter, macAddr, macValue);
					sprintf(msg+strlen(msg), "[0x%08X]:%08X  ", macAddr, macValue);
					DBGPRINT(RT_DEBUG_INFO, "msg=%s\n", msg);
				}
			}
		}
	}

	if(strlen(msg) == 1)
		sprintf(msg+strlen(msg), "===>Error command format!");
	DBGPRINT(RT_DEBUG_INFO, "copy to user [msg=%s]\n", msg);
	// Copy the information into the user buffer
	DBGPRINT(RT_DEBUG_INFO, "strlen(msg) =%d\n", strlen(msg));
	wrq->u.data.length = strlen(msg);
	if (copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length))
	{
		DBGPRINT(RT_DEBUG_TRACE, "%s: copy_to_user() fail\n", __FUNCTION__);			
	}

	
done:
	kfree(mpool);
	DBGPRINT(RT_DEBUG_TRACE, "<==RTMPIoctlMAC\n\n");
}

/* 
    ==========================================================================
    Description:
        Read / Write E2PROM
Arguments:
    pAdapter                    Pointer to our adapter
    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage: 
               1.) iwpriv ra0 e2p 0     	==> read E2PROM where Addr=0x0
               2.) iwpriv ra0 e2p 0=1234    ==> write E2PROM where Addr=0x0, value=1234
    ==========================================================================
*/
VOID RTMPIoctlE2PROM(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	struct iwreq	*wrq)
{
	CHAR				*this_char;
	CHAR				*value;
	INT					j = 0, k = 0;
	CHAR				*mpool, *msg;//msg[1024];
	CHAR				*arg; //arg[255];
	CHAR				*ptr;
	USHORT				eepAddr = 0;
	UCHAR				temp[16], temp2[16];
	USHORT				eepValue;

	DBGPRINT(RT_DEBUG_INFO, "==>RTMPIoctlE2PROM\n");

	mpool = (CHAR *) kmalloc(sizeof(CHAR)*(2048+256+12), MEM_ALLOC_FLAG);

	if (mpool == NULL) {
		return;
	}

	msg = (CHAR *)((UMAXLONG)(mpool+3) & (UMAXLONG)~0x03);
	arg = (CHAR *)((UMAXLONG)(msg+1024+3) & (UMAXLONG)~0x03);


	memset(msg, 0x00, 1024);
	if (wrq->u.data.length > 1) //No parameters.
	{
		memcpy(arg, wrq->u.data.pointer, (wrq->u.data.length > 255) ? 255 : wrq->u.data.length);
		ptr = arg;
		sprintf(msg, "\n");
		//Parsing Read or Write
		while ((this_char = strsep(&ptr, ",")) != NULL)
		{
			DBGPRINT(RT_DEBUG_INFO, "this_char=%s\n", this_char);
			if (!*this_char)
				continue;

			if ((value = strchr(this_char, '=')) != NULL)
				*value++ = 0;

			if (!value || !*value)
			{ //Read
				DBGPRINT(RT_DEBUG_INFO, "Read: this_char=%s, strlen=%d\n", this_char, strlen(this_char));

				// Sanity check
				if(strlen(this_char) > 4)
					break;

				j = strlen(this_char);
				while(j-- > 0)
				{
					if(this_char[j] > 'f' || this_char[j] < '0')
						goto done; //return;
				}

				// E2PROM addr
				k = j = strlen(this_char);
				while(j-- > 0)
				{
					this_char[4-k+j] = this_char[j];
				}
				
				while(k < 4)
					this_char[3-k++]='0';
				this_char[4]='\0';

				if(strlen(this_char) == 4)
				{
					AtoH(this_char, temp, 4);
					eepAddr = *temp*256 + temp[1];					
					if (eepAddr < 0xFFFF)
					{
						eepValue = RTMP_EEPROM_READ16(pAdapter, eepAddr);
						DBGPRINT(RT_DEBUG_INFO, "eepAddr=%x, eepValue=0x%x\n", eepAddr, eepValue);
						sprintf(msg+strlen(msg), "[0x%04X]:0x%X  ", eepAddr , eepValue);
						DBGPRINT(RT_DEBUG_INFO, "msg=%s\n", msg);
					}
					else
					{//Invalid parametes, so default printk all bbp
						break;
					}
				}
			}
			else
			{ //Write
				DBGPRINT(RT_DEBUG_INFO, "Write: this_char=%s, strlen(value)=%d, value=%s\n", this_char, strlen(value), value);
				memcpy(&temp2, value, strlen(value));
				temp2[strlen(value)] = '\0';

				// Sanity check
				if((strlen(this_char) > 4) || strlen(temp2) > 8)
					break;

				j = strlen(this_char);
				while(j-- > 0)
				{
					if(this_char[j] > 'f' || this_char[j] < '0')
						goto done; //return;
				}
				j = strlen(temp2);
				while(j-- > 0)
				{
					if(temp2[j] > 'f' || temp2[j] < '0')
						goto done; //return;
				}

				//MAC Addr
				k = j = strlen(this_char);
				while(j-- > 0)
				{
					this_char[4-k+j] = this_char[j];
				}

				while(k < 4)
					this_char[3-k++]='0';
				this_char[4]='\0';

				//MAC value
				k = j = strlen(temp2);
				while(j-- > 0)
				{
					temp2[4-k+j] = temp2[j];
				}
				
				while(k < 4)
					temp2[3-k++]='0';
				temp2[4]='\0';

				AtoH(this_char, temp, 4);
				eepAddr = *temp*256 + temp[1];

				AtoH(temp2, temp, 4);
				eepValue = *temp*256 + temp[1];

				DBGPRINT(RT_DEBUG_INFO, "eepAddr=%02x, eepValue=0x%x\n", eepAddr, eepValue);
				
				RTMP_EEPROM_WRITE16(pAdapter, eepAddr, eepValue);
				sprintf(msg+strlen(msg), "[0x%02X]:%02X  ", eepAddr, eepValue);
				DBGPRINT(RT_DEBUG_INFO, "msg=%s\n", msg);
			}
		}
	}

	if(strlen(msg) == 1)
		sprintf(msg+strlen(msg), "===>Error command format!");

	// Copy the information into the user buffer
	DBGPRINT(RT_DEBUG_INFO, "copy to user [msg=%s]\n", msg);
	wrq->u.data.length = strlen(msg);
	if (copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length))
	{
		DBGPRINT(RT_DEBUG_TRACE, "%s: copy_to_user() fail\n", __FUNCTION__);			
	}

done:	
	kfree(mpool);
	DBGPRINT(RT_DEBUG_TRACE, "<==RTMPIoctlE2PROM\n");
}

/* 
    ==========================================================================
    Description:
        Check IC Hard and EEPROM Match or no
    Arguments:
        pAdapter                 Pointer to our adapter
        wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage: 
               1.) iwpriv ra0 chk_asic_ver     	==> Read the status of version check
    ==========================================================================
*/
VOID RTMPIoctlCHKICVER(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	struct iwreq	*wrq)
{
	USHORT				eepValue, tmpValue;
	ULONG 				macValue, GetICVersion=FALSE;
	char					*msg;	

	DBGPRINT(RT_DEBUG_TRACE, "==>RTMPIoctlCHKICVER\n");
	
	msg = (CHAR *) kmalloc(sizeof(CHAR)*(2048), MEM_ALLOC_FLAG);
	if (msg == NULL) 
	{
		return;
	}
	
	memset(msg, 0x00, 1024);
	sprintf(msg, "\n");
	
	eepValue = RTMP_EEPROM_READ16(pAdapter, 0);
	RTMP_IO_READ32(pAdapter, MAC_CSR0, &macValue);
	tmpValue = (USHORT)(macValue >> 4);
	if(tmpValue == eepValue)
	{
		GetICVersion = TRUE;
	}

	sprintf(msg+strlen(msg), "ASIC model number \t\t= [%04x]\n", tmpValue);	
	sprintf(msg+strlen(msg), "ASIC model check status \t= %d\n", GetICVersion);
	if(GetICVersion)
	{
		sprintf(msg+strlen(msg), "After compare EEPROM and ASIC hard code, the result is match!\n");
	}
	else
	{
		sprintf(msg+strlen(msg), "After compare EEPROM and ASIC hard code, the result is miss match!\n");
	}

	DBGPRINT(RT_DEBUG_INFO, "copy to user [msg=%s]\n", msg);
	wrq->u.data.length = strlen(msg);
	
    	if (copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length))
	{
		DBGPRINT(RT_DEBUG_TRACE, "%s: copy_to_user() fail\n", __FUNCTION__);			
	}
		
	kfree(msg);
	DBGPRINT(RT_DEBUG_TRACE, "<==RTMPIoctlCHKICVER\n");		
}

#endif //#ifdef DBG

/* 
    ==========================================================================
    Description:
        Read statistics counter
Arguments:
    pAdapter                    Pointer to our adapter
    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage: 
               1.) iwpriv ra0 stat 0     	==> Read statistics counter
    ==========================================================================
*/
VOID RTMPIoctlStatistics(
	IN	PRTMP_ADAPTER	pAd, 
	IN	struct iwreq	*wrq)
{
	char                *msg;
#ifdef WSC_SUPPORT
#ifdef APCLI_SUPPORT
	UCHAR               idx = 0;
#endif // APCLI_SUPPORT //
#endif // WSC_SUPPORT //

	msg = (CHAR *) kmalloc(sizeof(CHAR)*(2048), MEM_ALLOC_FLAG);
	if (msg == NULL) {
		return;
	}

    memset(msg, 0x00, 1600);
    sprintf(msg, "\n");

    sprintf(msg+strlen(msg), "Tx success                      = %ld\n", (UMAXLONG)pAd->WlanCounters.TransmittedFragmentCount);
    sprintf(msg+strlen(msg), "Tx success without retry        = %ld\n", (UMAXLONG)pAd->WlanCounters.TransmittedFragmentCount - (ULONG)pAd->WlanCounters.RetryCount);
    sprintf(msg+strlen(msg), "Tx success after retry          = %ld\n", (UMAXLONG)pAd->WlanCounters.RetryCount);
    sprintf(msg+strlen(msg), "Tx fail to Rcv ACK after retry  = %ld\n", (UMAXLONG)pAd->WlanCounters.FailedCount);
    sprintf(msg+strlen(msg), "RTS Success Rcv CTS             = %ld\n", (UMAXLONG)pAd->WlanCounters.RTSSuccessCount);
    sprintf(msg+strlen(msg), "RTS Fail Rcv CTS                = %ld\n", (UMAXLONG)pAd->WlanCounters.RTSFailureCount);

    sprintf(msg+strlen(msg), "Rx success                      = %ld\n", (UMAXLONG)pAd->WlanCounters.ReceivedFragmentCount);
    sprintf(msg+strlen(msg), "Rx with CRC                     = %ld\n", (UMAXLONG)pAd->WlanCounters.FCSErrorCount);
    sprintf(msg+strlen(msg), "Rx drop due to out of resource  = %ld\n", (UMAXLONG)pAd->Counters8023.RxNoBuffer);
    sprintf(msg+strlen(msg), "Rx duplicate frame              = %ld\n", (UMAXLONG)pAd->WlanCounters.FrameDuplicateCount);

    sprintf(msg+strlen(msg), "False CCA (one second)          = %ld\n", (UMAXLONG)pAd->RalinkCounters.OneSecFalseCCACnt);
    sprintf(msg+strlen(msg), "RSSI-A                          = %d\n", (LONG)(pAd->PortCfg.LastRssi - pAd->BbpRssiToDbmDelta));
    sprintf(msg+strlen(msg), "RSSI-B (if available)           = %d\n\n", (LONG)(pAd->PortCfg.LastRssi2 - pAd->BbpRssiToDbmDelta));
    sprintf(msg+strlen(msg), "Tx Queue Size (BK/BE/VI/VO/HCCA)= %d/%d/%d/%d/%d (%d)\n\n", 
		(LONG)(pAd->TxSwQueue[QID_AC_BK].Number),
		(LONG)(pAd->TxSwQueue[QID_AC_BE].Number),
		(LONG)(pAd->TxSwQueue[QID_AC_VI].Number),
		(LONG)(pAd->TxSwQueue[QID_AC_VO].Number),
		(LONG)(pAd->TxSwQueue[QID_HCCA].Number),
		(LONG)(pAd->TxSwQueue[QID_AC_BK].Number 
	         + pAd->TxSwQueue[QID_AC_BE].Number 
	         + pAd->TxSwQueue[QID_AC_VI].Number 
	         + pAd->TxSwQueue[QID_AC_VO].Number
	         + pAd->TxSwQueue[QID_HCCA].Number));

#ifdef WSC_SUPPORT
    // For Re-Generate Pin-Code Modification
    sprintf(msg+strlen(msg), "WPS Information:\n");
    // display pin code
    sprintf(msg+strlen(msg), "Enrollee PinCode(ra0)           %08u\n", pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscEnrolleePinCode);
    // --
// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
    sprintf(msg+strlen(msg), "\n");
    sprintf(msg+strlen(msg), "Enrollee PinCode(ApCli0)        %08u\n", pAd->ApCliTab.ApCliEntry[0].WscControl.WscEnrolleePinCode);
    sprintf(msg+strlen(msg), "Ap Client WPS Profile Count     = %d\n", pAd->ApCliTab.ApCliEntry[0].WscControl.WscProfile.ProfileCnt);
    for (idx = 0; idx < pAd->ApCliTab.ApCliEntry[0].WscControl.WscProfile.ProfileCnt ; idx++)
    {
        PWSC_CREDENTIAL pCredential = &pAd->ApCliTab.ApCliEntry[0].WscControl.WscProfile.Profile[idx];
        sprintf(msg+strlen(msg), "Profile[%d]:\n", idx);        
        sprintf(msg+strlen(msg), "SSID                            = %s\n", pCredential->SSID.Ssid);
        sprintf(msg+strlen(msg), "AuthType                        = %s\n", WscGetAuthTypeStr(pCredential->AuthType));
        sprintf(msg+strlen(msg), "EncrypType                      = %s\n", WscGetEncryTypeStr(pCredential->EncrType)); 
        sprintf(msg+strlen(msg), "KeyIndex                        = %d\n", pCredential->KeyIndex);
        if (pCredential->KeyLength != 0)
        {
            sprintf(msg+strlen(msg), "Key                             = %s\n", pCredential->Key);
        }
    }
    sprintf(msg+strlen(msg), "\n");
#endif // APCLI_SUPPORT //
// --
#endif // WSC_SUPPORT //

    // Copy the information into the user buffer
    DBGPRINT(RT_DEBUG_INFO, "copy to user [msg=%s]\n", msg);
    wrq->u.data.length = strlen(msg);
    if (copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length))
	{
		DBGPRINT(RT_DEBUG_TRACE, "%s: copy_to_user() fail\n", __FUNCTION__);			
	}
	
	kfree(msg);
    DBGPRINT(RT_DEBUG_TRACE, "<==RTMPIoctlStatistics\n");
}

/* 
    ==========================================================================
    Description:
        Get WSC Profile
	Arguments:
	    pAdapter                    Pointer to our adapter
	    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage:
        		1.) iwpriv ra0 get_wsc_profile
        		3.) UI needs to prepare at least 4096bytes to get the results
    ==========================================================================
*/
#ifdef WSC_SUPPORT
VOID RTMPIoctlWscProfile(
	IN	PRTMP_ADAPTER	pAd, 
	IN	struct iwreq	*wrq)
{
	WSC_CONFIGURED_VALUE Profile;
	char *msg;

	memset(&Profile, 0x00, sizeof(WSC_CONFIGURED_VALUE));
	Profile.WscConfigured = pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscConfStatus;
	NdisZeroMemory(Profile.WscSsid, 32 + 1);
	NdisMoveMemory(Profile.WscSsid, pAd->PortCfg.MBSSID[MAIN_MBSSID].Ssid, 
										pAd->PortCfg.MBSSID[MAIN_MBSSID].SsidLen);
	Profile.WscSsid[pAd->PortCfg.MBSSID[MAIN_MBSSID].SsidLen] = '\0';
	Profile.WscAuthMode = WscGetAuthType(pAd->PortCfg.MBSSID[MAIN_MBSSID].AuthMode);
	Profile.WscEncrypType = WscGetEncryType(pAd->PortCfg.MBSSID[MAIN_MBSSID].WepStatus);
	NdisZeroMemory(Profile.WscWPAKey, 64 + 1);

	if (Profile.WscEncrypType == 2)
	{
		Profile.DefaultKeyIdx = pAd->PortCfg.MBSSID[MAIN_MBSSID].DefaultKeyId + 1;

		if (pAd->PortCfg.MBSSID[MAIN_MBSSID].SharedKey[pAd->PortCfg.MBSSID[MAIN_MBSSID].DefaultKeyId].WepKeyType == WEP_ASCII_TYPE)
		{
			int i;
			for (i=0; i<pAd->PortCfg.MBSSID[MAIN_MBSSID].SharedKey[pAd->PortCfg.MBSSID[MAIN_MBSSID].DefaultKeyId].KeyLen; i++)
			{
				sprintf(Profile.WscWPAKey,
						"%s%02x", Profile.WscWPAKey,
									pAd->PortCfg.MBSSID[MAIN_MBSSID].SharedKey[pAd->PortCfg.MBSSID[MAIN_MBSSID].DefaultKeyId].Key[i]);
			}
			Profile.WscWPAKey[(pAd->PortCfg.MBSSID[MAIN_MBSSID].SharedKey[pAd->PortCfg.MBSSID[MAIN_MBSSID].DefaultKeyId].KeyLen)*2] = '\0';
		}
		else // Hex WEP Key
		{
			int i;
			for (i=0; i<pAd->PortCfg.MBSSID[MAIN_MBSSID].SharedKey[pAd->PortCfg.MBSSID[MAIN_MBSSID].DefaultKeyId].KeyLen; i++)
			{
				sprintf(Profile.WscWPAKey,
						"%s%02x", Profile.WscWPAKey,
									pAd->PortCfg.MBSSID[MAIN_MBSSID].SharedKey[pAd->PortCfg.MBSSID[MAIN_MBSSID].DefaultKeyId].Key[i]);
			}
			Profile.WscWPAKey[(pAd->PortCfg.MBSSID[MAIN_MBSSID].SharedKey[pAd->PortCfg.MBSSID[MAIN_MBSSID].DefaultKeyId].KeyLen)*2] = '\0';
		}
	}
	else if (Profile.WscEncrypType >= 4)
	{
		Profile.DefaultKeyIdx = 2;
		NdisMoveMemory(Profile.WscWPAKey, pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WpaPsk, 
										pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WpaPskLen);
		Profile.WscWPAKey[pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WpaPskLen] = '\0';
	}
	else
	{
		Profile.DefaultKeyIdx = 1;
	}

	wrq->u.data.length = sizeof(Profile);

	if (copy_to_user(wrq->u.data.pointer, &Profile, wrq->u.data.length))
	{
		DBGPRINT(RT_DEBUG_TRACE, "%s: copy_to_user() fail\n", __FUNCTION__);			
	}

	msg = (CHAR *) kmalloc(sizeof(CHAR)*(2048), MEM_ALLOC_FLAG);
	if (msg == NULL) {
		return;
	}

	memset(msg, 0x00, 2048);
	sprintf(msg,"%s","\n");

	if (Profile.WscEncrypType == 1)
	{
		sprintf(msg+strlen(msg),"%-12s%-33s%-12s%-12s\n", "Configured", "SSID", "AuthMode", "EncrypType");
	}
	else if (Profile.WscEncrypType == 2)
	{
		sprintf(msg+strlen(msg),"%-12s%-33s%-12s%-12s%-13s%-26s\n", "Configured", "SSID", "AuthMode", "EncrypType", "DefaultKeyID", "Key");
	}
	else
	{
		sprintf(msg+strlen(msg),"%-12s%-33s%-12s%-12s%-64s\n", "Configured", "SSID", "AuthMode", "EncrypType", "Key");
	}

	if (Profile.WscConfigured == 1)
		sprintf(msg+strlen(msg),"%-12s", "No");
	else
		sprintf(msg+strlen(msg),"%-12s", "Yes");
	sprintf(msg+strlen(msg), "%-33s", Profile.WscSsid);
	sprintf(msg+strlen(msg), "%-12s", WscGetAuthTypeStr(Profile.WscAuthMode));
	sprintf(msg+strlen(msg), "%-12s", WscGetEncryTypeStr(Profile.WscEncrypType));

	if (Profile.WscEncrypType == 1)
	{
		sprintf(msg+strlen(msg), "%s\n", "");
	}
	else if (Profile.WscEncrypType == 2)
	{
		sprintf(msg+strlen(msg), "%-13d",Profile.DefaultKeyIdx);
		sprintf(msg+strlen(msg), "%-26s\n",Profile.WscWPAKey);
	}
	else if (Profile.WscEncrypType >= 4)
	{
	    sprintf(msg+strlen(msg), "%-64s\n",Profile.WscWPAKey);
	}

	DBGPRINT(RT_DEBUG_TRACE, "%s", msg);

	kfree(msg);
}
#endif // WSC_SUPPORT //

/* 
    ==========================================================================
    Description:
        Parse encryption type
Arguments:
    pAdapter                    Pointer to our adapter
    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
    ==========================================================================
*/
CHAR *GetEncryptType(CHAR enc)
{
    if(enc == Ndis802_11WEPDisabled)
        return "NONE";
    if(enc == Ndis802_11WEPEnabled)
    	return "WEP";
    if(enc == Ndis802_11Encryption2Enabled)
    	return "TKIP";
    if(enc == Ndis802_11Encryption3Enabled)
    	return "AES";
    if(enc == Ndis802_11Encryption4Enabled)
    	return "TKIPAES";
    else
    	return "UNKNOW";
}		
    	
/* 
    ==========================================================================
    Description:
        Get site survey results
	Arguments:
	    pAdapter                    Pointer to our adapter
	    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage:
        		1.) UI needs to wait 4 seconds after issue a site survey command
        		2.) iwpriv ra0 get_site_survey
        		3.) UI needs to prepare at least 4096bytes to get the results
    ==========================================================================
*/
#define	LINE_LEN	(8+8+35+20+8)	// Channel+RSSI+SSID+Bssid+WepStatus
VOID RTMPIoctlGetSiteSurvey(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	struct iwreq	*wrq)
{
	CHAR		*msg;
    INT 		i=0;     
	CHAR		strSsid[MAX_LEN_OF_SSID+1]; // ssid with null terminator
    
	msg = (CHAR *) kmalloc(sizeof(CHAR)*(MAX_LEN_OF_BSS_TABLE*LINE_LEN), MEM_ALLOC_FLAG);

    memset(msg, 0 ,MAX_LEN_OF_BSS_TABLE*LINE_LEN );
    sprintf(msg,"%s","\n");
	sprintf(msg+strlen(msg),"%-8s%-8s%-35s%-20s%-8s\n", "Channel", "RSSI", "SSID", "BSSID", "Security");
    for(i=0;i<MAX_LEN_OF_BSS_TABLE;i++)
    {
		if( pAdapter->ScanTab.BssEntry[i].Channel==0)
	    break;
	if((strlen(msg)+LINE_LEN ) >= (MAX_LEN_OF_BSS_TABLE*LINE_LEN) )
	    break;	
			
		sprintf(msg+strlen(msg),"%-8d", pAdapter->ScanTab.BssEntry[i].Channel);
		sprintf(msg+strlen(msg),"%-8d", pAdapter->ScanTab.BssEntry[i].Rssi - pAdapter->BbpRssiToDbmDelta);
		memcpy(strSsid, pAdapter->ScanTab.BssEntry[i].Ssid, pAdapter->ScanTab.BssEntry[i].SsidLen); 
		strSsid[pAdapter->ScanTab.BssEntry[i].SsidLen]='\0';
		sprintf(msg+strlen(msg),"%-35s", strSsid);
		sprintf(msg+strlen(msg),"%02x:%02x:%02x:%02x:%02x:%02x  ", 
			pAdapter->ScanTab.BssEntry[i].Bssid[0], 
			pAdapter->ScanTab.BssEntry[i].Bssid[1],
			pAdapter->ScanTab.BssEntry[i].Bssid[2], 
			pAdapter->ScanTab.BssEntry[i].Bssid[3], 
			pAdapter->ScanTab.BssEntry[i].Bssid[4], 
			pAdapter->ScanTab.BssEntry[i].Bssid[5]);
		sprintf(msg+strlen(msg),"%-8s\n",GetEncryptType(pAdapter->ScanTab.BssEntry[i].WepStatus));
    }

    wrq->u.data.length = strlen(msg);
    if (copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length))
	{
		DBGPRINT(RT_DEBUG_TRACE, "%s: copy_to_user() fail\n", __FUNCTION__);			
	}

	kfree(msg);
}

#define	MAC_LINE_LEN	(14+4+4+10+10+10+6+6)	// Addr+aid+psm+datatime+rxbyte+txbyte+current tx rate+last tx rate
VOID RTMPIoctlGetMacTable(
	IN PRTMP_ADAPTER pAd, 
	IN struct iwreq *wrq)
{
	INT						i;
	RT_802_11_MAC_TABLE		MacTab;
        char *msg;

	//added by xyzhu_nj_20091208
	int lMacTabChanged = 0;
	lMacTabChanged = gMacTabChanged;
	gMacTabChanged = 0;
	
	MacTab.Num = 0;
	for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++)
	{
		if (pAd->MacTab.Content[i].Valid && (pAd->MacTab.Content[i].Sst == SST_ASSOC))
		{
			COPY_MAC_ADDR(MacTab.Entry[MacTab.Num].Addr, &pAd->MacTab.Content[i].Addr);
			MacTab.Entry[MacTab.Num].Aid = (UCHAR)pAd->MacTab.Content[i].Aid;
			MacTab.Entry[MacTab.Num].Psm = pAd->MacTab.Content[i].PsMode;
			MacTab.Entry[MacTab.Num].HSCounter.LastDataPacketTime = pAd->MacTab.Content[i].HSCounter.LastDataPacketTime;
			MacTab.Entry[MacTab.Num].HSCounter.TotalRxByteCount = pAd->MacTab.Content[i].HSCounter.TotalRxByteCount;
			MacTab.Entry[MacTab.Num].HSCounter.TotalTxByteCount = pAd->MacTab.Content[i].HSCounter.TotalTxByteCount;
			MacTab.Num += 1;
		}
	}    
	wrq->u.data.length = sizeof(RT_802_11_MAC_TABLE);
	if (copy_to_user(wrq->u.data.pointer, &MacTab, wrq->u.data.length))
	{
		DBGPRINT(RT_DEBUG_TRACE, "%s: copy_to_user() fail\n", __FUNCTION__);			
	}
	
    msg = (CHAR *) kmalloc(sizeof(CHAR)*(MAX_LEN_OF_MAC_TABLE*MAC_LINE_LEN), MEM_ALLOC_FLAG);
    memset(msg, 0 ,MAX_LEN_OF_MAC_TABLE*MAC_LINE_LEN );

    sprintf(msg,"%s","\n");
	
	//modified by xyzhu_nj_20091208,it should save lMacTabChanged to msg.
	sprintf(msg+strlen(msg), "MacTabChanged:%d\n", lMacTabChanged);
#if 0	
	sprintf(msg+strlen(msg),"%-14s%-4s%-4s%-10s%-10s%-10s%-6s%-6s\n", "MAC", "AID", "PSM", "LDT", "RxB", "TxB","CTxR", "LTxR");
#else
	sprintf(msg+strlen(msg),"%-14s%-4s%-4s%-9s%-9s%-9s%-6s%-6s\n", "MAC", "AID", "PSM", "LDT", "RxB", "TxB","CTxR", "LTxR");
#endif
	
	for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++)
	{
		if (pAd->MacTab.Content[i].Valid && (pAd->MacTab.Content[i].Sst == SST_ASSOC))
		{
	//modifeid by xyzhu_nj_20091208
	#if 1
			if((strlen(msg)+MAC_LINE_LEN ) >= (MAX_LEN_OF_MAC_TABLE*MAC_LINE_LEN) )
	#else
			if((strlen(msg)+MAC_LINE_LEN ) >= (MAX_LEN_OF_MAC_TABLE*MAC_LINE_LEN - 19) ) //19 = sizeof("MacTabChanged:%d\n") + sizeof('\0')
	#endif
	    			break;	
			sprintf(msg+strlen(msg),"%02x%02x%02x%02x%02x%02x  ",
			pAd->MacTab.Content[i].Addr[0],
			pAd->MacTab.Content[i].Addr[1],
			pAd->MacTab.Content[i].Addr[2],
			pAd->MacTab.Content[i].Addr[3],
			pAd->MacTab.Content[i].Addr[4],
			pAd->MacTab.Content[i].Addr[5]);
			sprintf(msg+strlen(msg),"%-4d",(int)pAd->MacTab.Content[i].Aid);
			sprintf(msg+strlen(msg),"%-4d",(int)pAd->MacTab.Content[i].PsMode);
	#if 0
			sprintf(msg+strlen(msg),"%-10ld",pAd->MacTab.Content[i].HSCounter.LastDataPacketTime);
			sprintf(msg+strlen(msg),"%-10ld",pAd->MacTab.Content[i].HSCounter.TotalRxByteCount);
			sprintf(msg+strlen(msg),"%-10ld",pAd->MacTab.Content[i].HSCounter.TotalTxByteCount);
	#else
			sprintf(msg+strlen(msg),"%-9ld",pAd->MacTab.Content[i].HSCounter.LastDataPacketTime);
			sprintf(msg+strlen(msg),"%-9ld",pAd->MacTab.Content[i].HSCounter.TotalRxByteCount);
			sprintf(msg+strlen(msg),"%-9ld",pAd->MacTab.Content[i].HSCounter.TotalTxByteCount);
	#endif
			sprintf(msg+strlen(msg),"%-6d",RateIdToMbps[pAd->MacTab.Content[i].CurrTxRate]);
			sprintf(msg+strlen(msg),"%-6d\n",RateIdToMbps[pAd->MacTab.Content[i].LastTxRate]);
		}
	} 
        // for compatible with old API just do the printk to console
        //wrq->u.data.length = strlen(msg);
        //if (copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length))
	{
		DBGPRINT(RT_DEBUG_TRACE, "%s", msg);			
	}

	//added by xyzhu_nj for copy msg to user.
	wrq->u.data.length = strlen(msg);
	if (copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length))
	{
		DBGPRINT(RT_DEBUG_TRACE, "%s: copy_to_user() fail\n", __FUNCTION__);			
	}
	
	kfree(msg);
}

VOID RTMP_SIOCGIWSCAN(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	struct iwreq	*wrq)
{
    INT 		    i=0;
	CHAR		    strSsid[MAX_LEN_OF_SSID+1]; // ssid with null terminator
    NDIS_802_11_SITE_SURVEY_TABLE site_survey_table[MAX_LEN_OF_BSS_TABLE];

    memset(site_survey_table, 0x00, sizeof(NDIS_802_11_SITE_SURVEY_TABLE)*MAX_LEN_OF_BSS_TABLE);

    for(i=0;i<MAX_LEN_OF_BSS_TABLE;i++)
    {
        if( pAdapter->ScanTab.BssEntry[i].Channel==0)
            break;

		site_survey_table[i].Channel = pAdapter->ScanTab.BssEntry[i].Channel;
		site_survey_table[i].Rssi = pAdapter->ScanTab.BssEntry[i].Rssi - pAdapter->BbpRssiToDbmDelta;
		memcpy(strSsid, pAdapter->ScanTab.BssEntry[i].Ssid, pAdapter->ScanTab.BssEntry[i].SsidLen); 
		strSsid[pAdapter->ScanTab.BssEntry[i].SsidLen]='\0';
		sprintf(site_survey_table[i].Ssid, "%s", strSsid);

	    COPY_MAC_ADDR(site_survey_table[i].Bssid, &pAdapter->ScanTab.BssEntry[i].Bssid);
	    sprintf(site_survey_table[i].EncrypT, "%s", GetEncryptType(pAdapter->ScanTab.BssEntry[i].WepStatus));

	}

    wrq->u.data.length = i* sizeof(NDIS_802_11_SITE_SURVEY_TABLE);
    copy_to_user(wrq->u.data.pointer, site_survey_table, wrq->u.data.length);
}

VOID RTMP_SIOCGIWAPLIST(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	struct iwreq	*wrq)
{
    int i=0;
    NDIS_802_11_STATION_TABLE sta_list_table[MAX_LEN_OF_MAC_TABLE];

    memset(sta_list_table, 0x0, sizeof(NDIS_802_11_STATION_TABLE)*MAX_LEN_OF_MAC_TABLE);

	for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++)
	{
		if (pAdapter->MacTab.Content[i].Valid && (pAdapter->MacTab.Content[i].Sst == SST_ASSOC))
		{
			COPY_MAC_ADDR(sta_list_table[i].MacAddr, &pAdapter->MacTab.Content[i].Addr);
            sta_list_table[i].Aid = (int)pAdapter->MacTab.Content[i].Aid;
            sta_list_table[i].PsMode = (int)pAdapter->MacTab.Content[i].PsMode;
            sta_list_table[i].LastDataPacketTime = pAdapter->MacTab.Content[i].HSCounter.LastDataPacketTime;
            sta_list_table[i].RxByteCount = pAdapter->MacTab.Content[i].HSCounter.TotalRxByteCount;
            sta_list_table[i].TxByteCount = pAdapter->MacTab.Content[i].HSCounter.TotalTxByteCount;
            sta_list_table[i].CurrTxRate = RateIdToMbps[pAdapter->MacTab.Content[i].CurrTxRate];
            sta_list_table[i].LastTxRate = RateIdToMbps[pAdapter->MacTab.Content[i].LastTxRate];
		}
	}

    wrq->u.data.length = i * sizeof(NDIS_802_11_STATION_TABLE);
    copy_to_user(wrq->u.data.pointer, sta_list_table, wrq->u.data.length);
}

#ifdef RALINK_ATE
UCHAR TemplateFrame[24] = {0x08,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0xAA,0xBB,0x12,0x34,0x56,0x00,0x11,0x22,0xAA,0xBB,0xCC,0x00,0x00};	// 802.11 MAC Header, Type:Data, Length:24bytes 

/*
    ==========================================================================
    Description:
        Set ATE operation mode to
        0. APSTOP  = Stop AP Mode
        1. APSTART = Start AP Mode
        2. TXCONT  = Continuous Transmit
        3. TXCARR  = Transmit Carrier
        4. TXFRAME = Transmit Frames
        5. RXFRAME = Receive Frames
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/

static inline INT	set_ate_proc_inline(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	UCHAR			BbpData;
	ULONG 			MacData;
	PTXD_STRUC		pTxD;
	PUCHAR			pDest;
	UINT			i, j, atemode;
	UCHAR			r70;
	UCHAR			bbpR94, BbpTxPower;

#ifdef	BIG_ENDIAN
    PTXD_STRUC      pDestTxD;
    TXD_STRUC       TxD;
#endif

	ATEAsicSwitchChannel(pAd, pAd->ate.Channel);
	AsicLockChannel(pAd, pAd->ate.Channel);
	RTMPusecDelay(5000);

	if (pAd->TxPowerDelta.field.Enable == 1)
	{
	    if (pAd->TxPowerDelta.field.Sign == 1)
	    {
	    	if ((pAd->LatchBBPR94 + pAd->TxPowerDelta.field.Delta) < 12)
	    		bbpR94 = pAd->LatchBBPR94 + pAd->TxPowerDelta.field.Delta;
	    	else
	    		bbpR94 = 12;
	    }
	    else
	    {
	    	if ((pAd->LatchBBPR94 - pAd->TxPowerDelta.field.Delta) > 0)
		    	bbpR94 = pAd->LatchBBPR94 - pAd->TxPowerDelta.field.Delta;
	    	else
	    		bbpR94 = 0;
	    }
	    BbpTxPower  = (pAd->ate.TxRate < RATE_FIRST_OFDM_RATE) ? bbpR94 : pAd->LatchBBPR94;
	    RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R94, BbpTxPower);
	}

	// BBP R70 Japan filter bit on ch14, when (TXFRAME or TXCONT) and (pAd->ate.Channel == 14)
	if (!strcmp(arg, "TXFRAME") || !strcmp(arg, "TXCONT"))
	{
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R70, &r70);
	    if ((pAd->ate.TxRate <= RATE_11) && (pAd->ate.Channel == 14))
	        r70 |= 0x08;    // turn on 
	    else
	        r70 &= 0xf7;    // turn off
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R70, r70);
	}
	else
	{
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R70, &r70);
        r70 &= 0xf7;    // turn off
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R70, r70);
	}

	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, 63, &BbpData);
	RTMP_IO_READ32(pAd, PHY_CSR1, &MacData);

	BbpData = 0;
	MacData &= 0xFFFFFFFE;

	if (!strcmp(arg, "APSTOP")) 
	{						
		DBGPRINT(RT_DEBUG_TRACE, "ATE: APSTOP\n");
		
		atemode = pAd->ate.Mode;
		pAd->ate.Mode = ATE_APSTOP;
		pAd->ate.TxDoneCount = pAd->ate.TxCount;
		
		// we should free some resource which allocate when ATE_TXFRAME
		if (atemode == ATE_TXFRAME)
		{
			RTMP_IO_WRITE32(pAd, TX_CNTL_CSR, 0x000f0000);      // abort all TX rings
			RTMPusecDelay(5000);

			for (i=0; i<TX_RING_SIZE; i++)
			{
				struct sk_buff	*pSkb;
				pTxD = (PTXD_STRUC)pAd->TxRing[QID_AC_BE].Cell[i].AllocVa;
#ifndef BIG_ENDIAN
			    pTxD = (PTXD_STRUC)pAd->TxRing[QID_AC_BE].Cell[i].AllocVa;
#else
        		pDestTxD = (PTXD_STRUC)pAd->TxRing[QID_AC_BE].Cell[i].AllocVa;
        		TxD = *pDestTxD;
        		pTxD = &TxD;
        		RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif
				pTxD->Owner = DESC_OWN_NIC;
				pTxD->bWaitingDmaDoneInt = 0;
#ifdef CONFIG_64BIT
			    pSkb = pAd->TxRing[QID_AC_BE].Cell[i].pSkb;
		        if (pSkb)
		        {
					pci_unmap_single(pAd->pPci_Dev, pTxD->BufPhyAddr1, pTxD->BufLen1, PCI_DMA_TODEVICE);
		            pAd->TxRing[QID_AC_BE].Cell[i].pSkb = 0;
		            RELEASE_NDIS_PACKET(pAd, pSkb);
		        }

		        pSkb = pAd->TxRing[QID_AC_BE].Cell[i].pNextSkb;
		        if (pSkb)
		        {
		        	pci_unmap_single(pAd->pPci_Dev, pTxD->BufPhyAddr2, pTxD->BufLen2, PCI_DMA_TODEVICE);
		            pAd->TxRing[QID_AC_BE].Cell[i].pNextSkb = 0;
		            RELEASE_NDIS_PACKET(pAd, pSkb);
		        }
#else
			    pSkb = pTxD->pSkb;
		        if (pSkb)
		        {
					pci_unmap_single(pAd->pPci_Dev, pTxD->BufPhyAddr1, pTxD->BufLen1, PCI_DMA_TODEVICE);
		            pTxD->pSkb = 0;
		            RELEASE_NDIS_PACKET(pAd, pSkb);
		        }

		        pSkb = pTxD->pNextSkb;
		        if (pSkb)
		        {
		        	pci_unmap_single(pAd->pPci_Dev, pTxD->BufPhyAddr2, pTxD->BufLen2, PCI_DMA_TODEVICE);
		            pTxD->pNextSkb = 0;
		            RELEASE_NDIS_PACKET(pAd, pSkb);
		        }
#endif // CONFIG_64BIT //

#ifdef BIG_ENDIAN
				RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
				WriteBackToDescriptor((PUCHAR)pDestTxD, (PUCHAR)pTxD, FALSE, TYPE_TXD);
#endif

			}
	        pAd->TxRing[QID_AC_BE].CurTxIndex         = 0;
            pAd->TxRing[QID_AC_BE].NextTxDmaDoneIndex = 0;
		}
		
    	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 63, BbpData);
    	RTMP_IO_WRITE32(pAd, PHY_CSR1, MacData);

		netif_stop_queue(pAd->net_dev);
		ApStop(pAd, TRUE);
		
		// Reset R17 before ATE testing
		{
			UCHAR R17, R62, R17UpperBound, R17LowerBound;

			if (pAd->LatchRfRegs.Channel <= 14)
			{
				R17UpperBound = pAd->BbpTuning.R17UpperBoundG;
				R17LowerBound = pAd->BbpTuning.R17LowerBoundG;
			}
			else
			{
				R17UpperBound = pAd->BbpTuning.R17UpperBoundA;
				R17LowerBound = pAd->BbpTuning.R17LowerBoundA;
			}

			R17 = R17LowerBound;
			RTMP_WRITE_BBP_R17(pAd, R17);

			R62 = 0x04;
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R62, R62);
		}
		
		RTMP_IO_WRITE32(pAd, TXRX_CSR0, 0x0257b032);        // Disable Rx
	}
	else if (!strcmp(arg, "APSTART")) 
	{						
		DBGPRINT(RT_DEBUG_TRACE, "ATE: APSTART\n");
		
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 63, BbpData);
		RTMP_IO_WRITE32(pAd, PHY_CSR1, MacData);
		
		pAd->ate.Mode = ATE_APSTART;

		RTMP_IO_WRITE32(pAd, TXRX_CSR0, 0x0256b032);       // enable RX of MAC block
		netif_start_queue(pAd->net_dev);

		ApStartUp(pAd, TRUE);
	}
	else if (!strcmp(arg, "TXCONT")) 		// Continuous Tx
	{						
		UCHAR			R2;

		DBGPRINT(RT_DEBUG_TRACE, "ATE: TXCONT\n");

		pAd->ate.Mode = ATE_TXCONT;

		RTMP_IO_WRITE32(pAd, TX_CNTL_CSR, 0x000f0000);      // abort all TX rings

        if (pAd->ate.TxRate <=3)  // 11b mode
		{
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, 2, &R2);
			R2 &=0xF0;
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 2,R2 );
 			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 10,0x03 ); //CCK
		}
		else
		{
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, 2, &R2);
			R2 |=0x8;
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 2,R2 );
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 5,0x0c ); //OFDM
		}

		BbpData |= 0x80;
		MacData |= 0x00000001;
		
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 63, BbpData);
		RTMP_IO_WRITE32(pAd, PHY_CSR1, MacData);

		RTMP_IO_WRITE32(pAd, TX_CNTL_CSR, BIT8[QID_AC_BE]);
		RTMP_IO_WRITE32(pAd, TXRX_CSR0, 0x0257b032);        // Disable Rx
	}
	else if (!strcmp(arg, "TXCARR"))			// Tx Carrier
	{
		DBGPRINT(RT_DEBUG_TRACE, "ATE: TXCARR\n");
		pAd->ate.Mode = ATE_TXCARR;

		BbpData |= 0x40;
		MacData |= 0x00000001;

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 63, BbpData);
		RTMP_IO_WRITE32(pAd, PHY_CSR1, MacData);

		RTMP_IO_WRITE32(pAd, TXRX_CSR0, 0x0257b032);        // Disable Rx
	}
	else if (!strcmp(arg, "TXFRAME"))			// Tx Frames
	{						
		DBGPRINT(RT_DEBUG_TRACE, "ATE: TXFRAME(Count=%d)\n", pAd->ate.TxCount);
		pAd->ate.Mode = ATE_TXFRAME;

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 63, BbpData);
		RTMP_IO_WRITE32(pAd, PHY_CSR1, MacData);

		pAd->ate.TxDoneCount = 0;
		
		for (i = 0; (i < TX_RING_SIZE) && (i < pAd->ate.TxCount); i++)
		{
			struct sk_buff	*pSkb;
			PRTMP_TX_RING   pTxRing = &pAd->TxRing[QID_AC_BE];
			PUCHAR			pDestBufVA = (PUCHAR) pTxRing->Cell[pTxRing->CurTxIndex].DmaBuf.AllocVa;

#ifndef BIG_ENDIAN
			pTxD = (PTXD_STRUC)pAd->TxRing[QID_AC_BE].Cell[pTxRing->CurTxIndex].AllocVa;
#else
        	pDestTxD = (PTXD_STRUC)pAd->TxRing[QID_AC_BE].Cell[pTxRing->CurTxIndex].AllocVa;
        	TxD = *pDestTxD;
        	pTxD = &TxD;
        	RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif
#ifdef CONFIG_64BIT
		    pSkb = pAd->TxRing[QID_AC_BE].Cell[pTxRing->CurTxIndex].pSkb;
	        if (pSkb)
	        {
				pci_unmap_single(pAd->pPci_Dev, pTxD->BufPhyAddr1, pTxD->BufLen1, PCI_DMA_TODEVICE);
	            pAd->TxRing[QID_AC_BE].Cell[pTxRing->CurTxIndex].pSkb = 0;
	            RELEASE_NDIS_PACKET(pAd, pSkb);
	        }
	        pSkb = pAd->TxRing[QID_AC_BE].Cell[pTxRing->CurTxIndex].pNextSkb;
	        if (pSkb)
	        {
	        	pci_unmap_single(pAd->pPci_Dev, pTxD->BufPhyAddr2, pTxD->BufLen2, PCI_DMA_TODEVICE);
	            pAd->TxRing[QID_AC_BE].Cell[pTxRing->CurTxIndex].pNextSkb = 0;
	            RELEASE_NDIS_PACKET(pAd, pSkb);
	        }
#else
		    pSkb = pTxD->pSkb;
	        if (pSkb)
	        {
				pci_unmap_single(pAd->pPci_Dev, pTxD->BufPhyAddr1, pTxD->BufLen1, PCI_DMA_TODEVICE);
	            pTxD->pSkb = 0;
	            RELEASE_NDIS_PACKET(pAd, pSkb);
	        }
	        pSkb = pTxD->pNextSkb;
	        if (pSkb)
	        {
	        	pci_unmap_single(pAd->pPci_Dev, pTxD->BufPhyAddr2, pTxD->BufLen2, PCI_DMA_TODEVICE);
	            pTxD->pNextSkb = 0;
	            RELEASE_NDIS_PACKET(pAd, pSkb);
	        }
#endif // CONFIG_64BIT //

			pSkb = __dev_alloc_skb(LOCAL_TXBUF_SIZE, MEM_ALLOC_FLAG);
			RTMP_SET_PACKET_SOURCE(pSkb, PKTSRC_DRIVER);
			RTMP_SET_PACKET_MOREDATA(pSkb, FALSE);

			if (pSkb == NULL) {
				pAd->ate.TxCount = i+1;
				break;
			}

			pDest = (PUCHAR) pSkb->data;
			pSkb->len = pAd->ate.TxLength - LENGTH_802_11;
		
			// Prepare frame payload
			memcpy(pDestBufVA, TemplateFrame, LENGTH_802_11);
			memcpy(&pDestBufVA[4], pAd->ate.Addr1, ETH_LENGTH_OF_ADDRESS);
			memcpy(&pDestBufVA[10], pAd->ate.Addr2, ETH_LENGTH_OF_ADDRESS);
			memcpy(&pDestBufVA[16], pAd->ate.Addr3, ETH_LENGTH_OF_ADDRESS);
			
			pTxD->BufLen0		= LENGTH_802_11;
			pTxD->BufLen1		= pAd->ate.TxLength - LENGTH_802_11;
			pTxD->BufPhyAddr1	= pci_map_single(pAd->pPci_Dev, pSkb->data, pSkb->len, PCI_DMA_TODEVICE);
			pTxD->BufCount		= 2;
			
			for(j = 0; j < (pAd->ate.TxLength - LENGTH_802_11); j++)
				pDest[j] = 0xA5;

#ifdef BIG_ENDIAN
			RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
		    *pDestTxD = TxD;
		    pTxD = pDestTxD;
#endif
#ifdef CONFIG_64BIT
			{
            RTMP_DMACB *dmacb_p = &pAd->TxRing[QID_AC_BE].Cell[pTxRing->CurTxIndex];

			RTMPWriteTxDescriptor(pAd, pTxD, CIPHER_NONE, 0, 0, FALSE, FALSE, FALSE, SHORT_RETRY,
				IFS_BACKOFF, pAd->ate.TxRate, pAd->ate.TxLength, QID_AC_BE, 0, pSkb, NULL, FALSE, FALSE, FALSE, FALSE, dmacb_p);
			}
#else
			RTMPWriteTxDescriptor(pAd, pTxD, CIPHER_NONE, 0, 0, FALSE, FALSE, FALSE, SHORT_RETRY, 
				IFS_BACKOFF, pAd->ate.TxRate, pAd->ate.TxLength, QID_AC_BE, 0, pSkb, NULL, FALSE, FALSE, FALSE, FALSE);
#endif // CONFIG_64BIT //

			INC_RING_INDEX(pTxRing->CurTxIndex, TX_RING_SIZE);
		}

		RTMP_IO_WRITE32(pAd, TX_CNTL_CSR, BIT8[QID_AC_BE]);
		
		RTMP_IO_WRITE32(pAd, TXRX_CSR0, 0x0257b032);        // Disable Rx
	}
	else if (!strcmp(arg, "RXFRAME")) 			// Rx Frames
	{						
		DBGPRINT(RT_DEBUG_TRACE, "ATE: RXFRAME\n");

    	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 63, BbpData);
    	RTMP_IO_WRITE32(pAd, PHY_CSR1, MacData);
    	
		pAd->ate.Mode = ATE_RXFRAME;
		
		RTMP_IO_WRITE32(pAd, TX_CNTL_CSR, 0x000f0000);      // abort all TX rings
		RTMP_IO_WRITE32(pAd, TXRX_CSR0, 0x0256b032);       // enable RX of MAC block
	}
	else
	{	
		DBGPRINT(RT_DEBUG_TRACE, "ATE: Invalid arg!\n");
		return FALSE;
	}
	RTMPusecDelay(5000);

	return TRUE;
}

INT	Set_ATE_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	
	return (set_ate_proc_inline(pAd, arg));
}


/* 
    ==========================================================================
    Description:
        Set ATE ADDR1=DA for TxFrame
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ATE_DA_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	CHAR				*value;
	INT					i;
	
	if(strlen(arg) != 17)  //Mac address acceptable format 01:02:03:04:05:06 length 17
		return FALSE;

    for (i=0, value = rstrtok(arg,":"); value; value = rstrtok(NULL,":")) 
	{
		if((strlen(value) != 2) || (!isxdigit(*value)) || (!isxdigit(*(value+1))) ) 
			return FALSE;  //Invalid

		AtoH(value, &pAd->ate.Addr1[i++], 2);
	}

	if(i != 6)
		return FALSE;  //Invalid
		
	DBGPRINT(RT_DEBUG_TRACE, "Set_ATE_DA_Proc (DA = %2X:%2X:%2X:%2X:%2X:%2X)\n", pAd->ate.Addr1[0],
		pAd->ate.Addr1[1], pAd->ate.Addr1[2], pAd->ate.Addr1[3], pAd->ate.Addr1[4], pAd->ate.Addr1[5]);
	
	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set ATE ADDR2=SA for TxFrame
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ATE_SA_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	CHAR				*value;
	INT					i;
	
	if(strlen(arg) != 17)  //Mac address acceptable format 01:02:03:04:05:06 length 17
		return FALSE;

    for (i=0, value = rstrtok(arg,":"); value; value = rstrtok(NULL,":")) 
	{
		if((strlen(value) != 2) || (!isxdigit(*value)) || (!isxdigit(*(value+1))) ) 
			return FALSE;  //Invalid

		AtoH(value, &pAd->ate.Addr2[i++], 2);
	}

	if(i != 6)
		return FALSE;  //Invalid

	DBGPRINT(RT_DEBUG_TRACE, "Set_ATE_SA_Proc (DA = %2X:%2X:%2X:%2X:%2X:%2X)\n", pAd->ate.Addr2[0],
		pAd->ate.Addr2[1], pAd->ate.Addr2[2], pAd->ate.Addr2[3], pAd->ate.Addr2[4], pAd->ate.Addr2[5]);
	
	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set ATE ADDR3=BSSID for TxFrame

    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ATE_BSSID_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	CHAR				*value;
	INT					i;
	
	if(strlen(arg) != 17)  //Mac address acceptable format 01:02:03:04:05:06 length 17
		return FALSE;

    for (i=0, value = rstrtok(arg,":"); value; value = rstrtok(NULL,":")) 
	{
		if((strlen(value) != 2) || (!isxdigit(*value)) || (!isxdigit(*(value+1))) ) 
			return FALSE;  //Invalid

		AtoH(value, &pAd->ate.Addr3[i++], 2);
	}

	if(i != 6)
		return FALSE;  //Invalid

	DBGPRINT(RT_DEBUG_TRACE, "Set_ATE_BSSID_Proc (DA = %2X:%2X:%2X:%2X:%2X:%2X)\n",	pAd->ate.Addr3[0],
		pAd->ate.Addr3[1], pAd->ate.Addr3[2], pAd->ate.Addr3[3], pAd->ate.Addr3[4], pAd->ate.Addr3[5]);
	
	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set ATE Tx Length

    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ATE_CHANNEL_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	pAd->ate.Channel = simple_strtol(arg, 0, 10);

	if((pAd->ate.Channel < 1) || (pAd->ate.Channel > 200))// to allow A band channel
	{
		pAd->ate.Channel = 1;
		DBGPRINT(RT_DEBUG_ERROR, "Set_ATE_CHANNEL_Proc::Out of range\n");
		return FALSE;
	}

	DBGPRINT(RT_DEBUG_TRACE, "Set_ATE_CHANNEL_Proc (ATE Channel = %d)\n", pAd->ate.Channel);
	
	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set ATE Tx Power
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ATE_TX_POWER_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG R3;
	UCHAR Bbp94 = 0, BbpTxPower;
	
	pAd->ate.TxPower = simple_strtol(arg, 0, 10);

	if ((pAd->ate.TxPower > 36) || (pAd->ate.TxPower < -6))
	{
		DBGPRINT(RT_DEBUG_ERROR, "Set_ATE_TX_POWER_Proc::Out of range (Value=%d)\n", pAd->ate.TxPower);
		return FALSE;
	}

	if (pAd->ate.TxPower > 31)  
	{
		//
		// R3 can't large than 36 (0x24), 31 ~ 36 used by BBP 94
		//
		R3 = 31;
		if (pAd->ate.TxPower <= 36)
			Bbp94 = BBPR94_DEFAULT + (UCHAR) (pAd->ate.TxPower - 31);		
		pAd->ate.TxPower = (CHAR)R3;
	}
	else if (pAd->ate.TxPower < 0)
	{
		//
		// R3 can't less than 0, -1 ~ -6 used by BBP 94
		//	
		R3 = 0;
		if (pAd->ate.TxPower >= -6)
			Bbp94 = BBPR94_DEFAULT + pAd->ate.TxPower;
		pAd->ate.TxPower = (CHAR)R3;
	}
	else
	{  
		// 0 ~ 31
		R3 = (ULONG) pAd->ate.TxPower;
		Bbp94 = BBPR94_DEFAULT;
	}

	DBGPRINT(RT_DEBUG_TRACE, "Set_ATE_TX_POWER_Proc (TxPower=%d, R3=%d, BBP_R94=%d)\n", pAd->ate.TxPower, R3, Bbp94);

	R3 = R3 << 9;		// shift TX power control to correct RF register bit position
	R3 |= (pAd->LatchRfRegs.R3 & 0xffffc1ff);
	pAd->LatchRfRegs.R3 = R3;

	RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R1);
	RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R2);
	RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R3);
	RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R4);

	if (Bbp94 > 0)
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R94, Bbp94);
	
	pAd->LatchBBPR94 = Bbp94;

	if (pAd->TxPowerDelta.field.Enable == 1)
	{
	    if (pAd->TxPowerDelta.field.Sign == 1)
	    {
	    	if ((pAd->LatchBBPR94 + pAd->TxPowerDelta.field.Delta) < 12)
	    		Bbp94 = pAd->LatchBBPR94 + pAd->TxPowerDelta.field.Delta;
	    	else
	    		Bbp94 = 12;
	    }
	    else
	    {
	    	if ((pAd->LatchBBPR94 - pAd->TxPowerDelta.field.Delta) > 0)
		    	Bbp94 = pAd->LatchBBPR94 - pAd->TxPowerDelta.field.Delta;
	    	else
	    		Bbp94 = 0;
	    }
	    BbpTxPower  = (pAd->ate.TxRate < RATE_FIRST_OFDM_RATE) ? Bbp94 : pAd->LatchBBPR94;
	    RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R94, BbpTxPower);
	}
	
	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set ATE RF frequence offset
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ATE_TX_FREQOFFSET_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG R4;
	
	pAd->ate.RFFreqOffset = simple_strtol(arg, 0, 10);

	if(pAd->ate.RFFreqOffset >= 64)
	{
		DBGPRINT(RT_DEBUG_ERROR, "Set_ATE_TX_FREQOFFSET_Proc::Out of range\n");
		return FALSE;
	}

	R4 = pAd->ate.RFFreqOffset << 12;		// shift TX power control to correct RF register bit position
	R4 |= (pAd->LatchRfRegs.R4 & (~0x0003f000));
	pAd->LatchRfRegs.R4 = R4;
	
	RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R1);
	RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R2);
	RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R3);
	RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R4);

	DBGPRINT(RT_DEBUG_TRACE, "Set_ATE_TX_FREQOFFSET_Proc (RFFreqOffset = %d)\n", pAd->ate.RFFreqOffset);
	
	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set ATE Tx frame length
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ATE_TX_LENGTH_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	pAd->ate.TxLength = simple_strtol(arg, 0, 10);

	if((pAd->ate.TxLength < 24) || (pAd->ate.TxLength > 1500))
	{
		pAd->ate.TxLength = 1500;
		DBGPRINT(RT_DEBUG_ERROR, "Set_ATE_TX_LENGTH_Proc::Out of range\n");
		return FALSE;
	}

	DBGPRINT(RT_DEBUG_TRACE, "Set_ATE_TX_LENGTH_Proc (TxLength = %d)\n", pAd->ate.TxLength);
	
	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set ATE Tx frame count
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ATE_TX_COUNT_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	pAd->ate.TxCount = simple_strtol(arg, 0, 10);

	DBGPRINT(RT_DEBUG_TRACE, "Set_ATE_TX_COUNT_Proc (TxCount = %d)\n", pAd->ate.TxCount);
	
	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set ATE Tx frame rate
        
        Return:
        	TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ATE_TX_RATE_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	pAd->ate.TxRate = simple_strtol(arg, 0, 10);

	if(pAd->ate.TxRate > RATE_54)
	{
		pAd->ate.TxRate = RATE_11;
		DBGPRINT(RT_DEBUG_ERROR, "Set_ATE_TX_RATE_Proc::Out of range\n");
		return FALSE;
	}

	DBGPRINT(RT_DEBUG_TRACE, "Set_ATE_TX_RATE_Proc (TxRate = %d)\n", pAd->ate.TxRate);
	
	return TRUE;
}

/*
    ==========================================================================
    Description:
    ==========================================================================
 */
INT	Set_ATE_RX_FER_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	pAd->ate.bRxFer = simple_strtol(arg, 0, 10);

	if (pAd->ate.bRxFer == 1)
	{
		pAd->ate.RxCntPerSec = 0;
		pAd->ate.RxTotalCnt = 0;
	}

	DBGPRINT(RT_DEBUG_TRACE, "Set_ATE_RX_FER_Proc (bRxFer = %d)\n", pAd->ate.bRxFer);
	
	return TRUE;
}

/*
    ==========================================================================
    Description:
    ==========================================================================
 */
VOID ATEAsicSwitchChannel(
    IN PRTMP_ADAPTER pAd, 
    IN UCHAR Channel) 
{
	ULONG R3, R4;
    UCHAR index, BbpReg;
    RTMP_RF_REGS *RFRegTable;

    // Select antenna
    AsicAntennaSelect(pAd, Channel);
        
    R3 = pAd->ate.TxPower << 9; // shift TX power control to correct RF R3 bit position
    
	if (pAd->RFProgSeq == 0)
		RFRegTable = RF5225RegTable;
	else
		RFRegTable = RF5225RegTable_1;
    
    switch (pAd->RfIcType)
    {
        case RFIC_5225:
        case RFIC_5325:
        case RFIC_2527:
		case RFIC_2529:
            for (index = 0; index < NUM_OF_5225_CHNL; index++)
            {
                if (Channel == RFRegTable[index].Channel)
                {
                    R3 = R3 | (RFRegTable[index].R3 & 0xffffc1ff); // set TX power
                    R4 = (RFRegTable[index].R4 & (~0x0003f000)) | (pAd->RfFreqOffset << 12);

                    // Update variables
                    pAd->LatchRfRegs.Channel = Channel;
                    pAd->LatchRfRegs.R1 = RFRegTable[index].R1;
                    pAd->LatchRfRegs.R2 = RFRegTable[index].R2;
                    pAd->LatchRfRegs.R3 = R3;
                    pAd->LatchRfRegs.R4 = R4;
                    
                    // Set RF value
                    RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R1);
                    RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R2);
                    RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R3);
                    RTMP_RF_IO_WRITE32(pAd, pAd->LatchRfRegs.R4);
                    break;
                }
            }
            RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpReg);
            if ((pAd->RfIcType == RFIC_5225) || (pAd->RfIcType == RFIC_2527))
                BbpReg &= 0xFE;    // b0=0 for none smart mode
            else
                BbpReg |= 0x01;    // b0=1 for smart mode
            RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpReg);
            break;

        default:
            break;
    }

	//
	// On 11A, We should delay and wait RF/BBP to be stable
	// and the appropriate time should be 1000 micro seconds 
	//
    if (Channel > 14)
    	RTMPusecDelay(1000);
    
    DBGPRINT(RT_DEBUG_TRACE, "ATEAsicSwitchChannel(RF=%d, Pwr=%d) to #%d, R1=0x%08x, R2=0x%08x, R3=0x%08x, R4=0x%08x\n",
        pAd->RfIcType, 
        (R3 & 0x00003e00) >> 9,
        Channel, 
        pAd->LatchRfRegs.R1, 
        pAd->LatchRfRegs.R2, 
        pAd->LatchRfRegs.R3, 
        pAd->LatchRfRegs.R4);
}

#endif	// RALINK_ATE

#ifdef APCLI_SUPPORT
INT Set_ApCli_Enable_Proc(
	IN  PRTMP_ADAPTER pAd, 
	IN  PUCHAR arg)
{
	UINT Enable;
	UCHAR ifIndex = pAd->ApcliIF;

	Enable = simple_strtol(arg, 0, 16);

	pAd->ApCliTab.ApCliEntry[ifIndex].Enable = (Enable > 0) ? TRUE : FALSE;

	DBGPRINT(RT_DEBUG_TRACE, "Set_ApCli_Enable_Proc::(ApcliEntry[%d].enable = %d)\n", ifIndex, pAd->ApCliTab.ApCliEntry[ifIndex].Enable);

	if (pAd->ApCliTab.ApCliEntry[ifIndex].Enable)
	{
		pAd->ApCliTab.ApCliEntry[ifIndex].WpaState = SS_NOTUSE;
		pAd->ApCliTab.ApCliEntry[ifIndex].PortSecured = WPA_802_1X_PORT_NOT_SECURED;
		NdisZeroMemory(pAd->ApCliTab.ApCliEntry[ifIndex].ReplayCounter, LEN_KEY_DESC_REPLAY); 
	}

	ApCliIfDown(pAd);

	return TRUE;
}

INT Set_ApCli_Ssid_Proc(
	IN  PRTMP_ADAPTER pAd, 
	IN  PUCHAR arg)
{
	UCHAR ifIndex = pAd->ApcliIF;
	BOOLEAN apcliEn;
	INT success = FALSE;
	UCHAR keyMaterial[40];
	UCHAR PskKey[100];

	if(strlen(arg) <= MAX_LEN_OF_SSID)
	{
		apcliEn = pAd->ApCliTab.ApCliEntry[ifIndex].Enable;

		// bring apcli interface down first
		if(apcliEn == TRUE )
		{
			pAd->ApCliTab.ApCliEntry[ifIndex].Enable = FALSE;
			ApCliIfDown(pAd);
		}

		NdisZeroMemory(pAd->ApCliTab.ApCliEntry[ifIndex].CfgSsid, MAX_LEN_OF_SSID);
		NdisMoveMemory(pAd->ApCliTab.ApCliEntry[ifIndex].CfgSsid, arg, strlen(arg));
		pAd->ApCliTab.ApCliEntry[ifIndex].CfgSsidLen = (UCHAR)strlen(arg);
		success = TRUE;

		// Upadte PMK and restart WPAPSK state machine for ApCli link
		if (((pAd->ApCliTab.ApCliEntry[ifIndex].AuthMode == Ndis802_11AuthModeWPAPSK) ||
				(pAd->ApCliTab.ApCliEntry[ifIndex].AuthMode == Ndis802_11AuthModeWPA2PSK)) && 
					pAd->ApCliTab.ApCliEntry[ifIndex].PSKLen > 0)
		{
			NdisMoveMemory(PskKey, pAd->ApCliTab.ApCliEntry[ifIndex].PSK, pAd->ApCliTab.ApCliEntry[ifIndex].PSKLen);
			
			if ((strlen(PskKey) >= 8) && (strlen(PskKey) < 64))
			{
	    		PasswordHash((char *)PskKey, pAd->ApCliTab.ApCliEntry[ifIndex].CfgSsid, pAd->ApCliTab.ApCliEntry[ifIndex].CfgSsidLen, keyMaterial);
	    		NdisMoveMemory(pAd->ApCliTab.ApCliEntry[ifIndex].PMK, keyMaterial, 32);
			}
			else if (strlen(PskKey) == 64)
			{
				AtoH(PskKey, pAd->ApCliTab.ApCliEntry[ifIndex].PMK, 32);
			}	
		
			// Start STA supplicant WPA state machine
    		pAd->ApCliTab.ApCliEntry[ifIndex].WpaState = SS_START;
		}

		DBGPRINT(RT_DEBUG_TRACE, "I/F(apcli%d) Set_ApCli_Ssid_Proc::(Len=%d,Ssid=%s)\n", ifIndex,
			pAd->ApCliTab.ApCliEntry[ifIndex].CfgSsidLen, pAd->ApCliTab.ApCliEntry[ifIndex].CfgSsid);

		pAd->ApCliTab.ApCliEntry[ifIndex].Enable = apcliEn;
	}
	else
		success = FALSE;

	return success;
}


INT Set_ApCli_Bssid_Proc(
	IN  PRTMP_ADAPTER pAd, 
	IN  PUCHAR arg)
{
	INT i;
	CHAR *value;
	UCHAR ifIndex = pAd->ApcliIF;
	BOOLEAN apcliEn;
	
	apcliEn = pAd->ApCliTab.ApCliEntry[ifIndex].Enable;

	// bring apcli interface down first
	if(apcliEn == TRUE )
	{
		pAd->ApCliTab.ApCliEntry[ifIndex].Enable = FALSE;
		ApCliIfDown(pAd);
	}

	NdisZeroMemory(pAd->ApCliTab.ApCliEntry[ifIndex].CfgApCliBssid, MAC_ADDR_LEN);

	if(strlen(arg) == 17)  //Mac address acceptable format 01:02:03:04:05:06 length 17
	{
		for (i=0, value = rstrtok(arg,":"); value; value = rstrtok(NULL,":")) 
		{
			if((strlen(value) != 2) || (!isxdigit(*value)) || (!isxdigit(*(value+1))) ) 
				return FALSE;  //Invalid

			AtoH(value, &pAd->ApCliTab.ApCliEntry[ifIndex].CfgApCliBssid[i++], 2);
		}

		if(i != 6)
			return FALSE;  //Invalid
	}

	DBGPRINT(RT_DEBUG_TRACE, "Set_ApCli_Bssid_Proc (%2X:%2X:%2X:%2X:%2X:%2X)\n",
		pAd->ApCliTab.ApCliEntry[ifIndex].CfgApCliBssid[0],
		pAd->ApCliTab.ApCliEntry[ifIndex].CfgApCliBssid[1],
		pAd->ApCliTab.ApCliEntry[ifIndex].CfgApCliBssid[2],
		pAd->ApCliTab.ApCliEntry[ifIndex].CfgApCliBssid[3],
		pAd->ApCliTab.ApCliEntry[ifIndex].CfgApCliBssid[4],
		pAd->ApCliTab.ApCliEntry[ifIndex].CfgApCliBssid[5]);

	pAd->ApCliTab.ApCliEntry[ifIndex].Enable = apcliEn;

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set WPA PSK key for ApCli link

    Arguments:
        pAdapter            Pointer to our adapter
        arg                 WPA pre-shared key string

    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ApCli_WPAPSK_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	UCHAR keyMaterial[40];
	UCHAR ifIndex = pAd->ApcliIF;

	DBGPRINT(RT_DEBUG_TRACE, "Set_ApCli_WPAPSK_Proc::(WPAPSK=%s)\n", arg);

	if ((strlen(arg) < 8) || (strlen(arg) > 64))
	{
		DBGPRINT(RT_DEBUG_TRACE, "Set failed!!(WPAPSK=%s), WPAPSK key-string required 8 ~ 64 characters \n", arg);
		return FALSE;
	}

	NdisMoveMemory(pAd->ApCliTab.ApCliEntry[ifIndex].PSK, arg, strlen(arg));
	pAd->ApCliTab.ApCliEntry[ifIndex].PSKLen = strlen(arg);

	if (strlen(arg) == 64)
	{
	    AtoH(arg, pAd->ApCliTab.ApCliEntry[ifIndex].PMK, 32);
	}
	else
	{
	    PasswordHash((char *)arg, pAd->ApCliTab.ApCliEntry[ifIndex].CfgSsid, pAd->ApCliTab.ApCliEntry[ifIndex].CfgSsidLen, keyMaterial);
	    NdisMoveMemory(pAd->ApCliTab.ApCliEntry[ifIndex].PMK, keyMaterial, 32);
	}
	
	if ((pAd->ApCliTab.ApCliEntry[ifIndex].AuthMode == Ndis802_11AuthModeWPAPSK) ||
		(pAd->ApCliTab.ApCliEntry[ifIndex].AuthMode == Ndis802_11AuthModeWPA2PSK))
	{			
		// Start STA supplicant WPA state machine
    	pAd->ApCliTab.ApCliEntry[ifIndex].WpaState = SS_START;
    	DBGPRINT(RT_DEBUG_TRACE, "Start STA supplicant WPA state machine.\n");
	}	
    
	return TRUE;
}


/*KH:Adding for isolating the security of AP Client from Main Bssid*/
/*   
 ==========================================================================
    Description:
        Set Authentication mode
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ApCli_AuthMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG        sec_csr4 = 0;
      
	if ((strcmp(arg, "WEPAUTO") == 0) || (strcmp(arg, "wepauto") == 0))
		pAd->ApCliTab.ApCliEntry[0].AuthMode = Ndis802_11AuthModeAutoSwitch;
	else if ((strcmp(arg, "OPEN") == 0) || (strcmp(arg, "open") == 0))
		pAd->ApCliTab.ApCliEntry[0].AuthMode= Ndis802_11AuthModeOpen;
	else if ((strcmp(arg, "SHARED") == 0) || (strcmp(arg, "shared") == 0))
		pAd->ApCliTab.ApCliEntry[0].AuthMode = Ndis802_11AuthModeShared;
	else if ((strcmp(arg, "WPAPSK") == 0) || (strcmp(arg, "wpapsk") == 0))
		pAd->ApCliTab.ApCliEntry[0].AuthMode = Ndis802_11AuthModeWPAPSK;
	else if ((strcmp(arg, "WPA2PSK") == 0) || (strcmp(arg, "wpa2psk") == 0))
		pAd->ApCliTab.ApCliEntry[0].AuthMode = Ndis802_11AuthModeWPA2PSK;
	else
		return FALSE;  

	pAd->ApCliTab.ApCliEntry[0].PortSecured = WPA_802_1X_PORT_NOT_SECURED;
    RTMPApCliMakeRSNIE(pAd, pAd->ApCliTab.ApCliEntry[0].AuthMode, pAd->ApCliTab.ApCliEntry[0].PairCipher, 0);
	// set key table enable bit
	RTMP_IO_READ32(pAd, SEC_CSR4, &sec_csr4);
    if (pAd->ApCliTab.ApCliEntry[0].AuthMode >= Ndis802_11AuthModeWPA)
    {
    	sec_csr4 |= BIT32[pAd->PortCfg.BssidNum];
        DBGPRINT(RT_DEBUG_TRACE, "ApCli AuthMode=%d, Pairwise Key Table in-used\n", pAd->ApCliTab.ApCliEntry[0].AuthMode);
    }
    else
    {
    	sec_csr4 &= ~BIT32[pAd->PortCfg.BssidNum];
        DBGPRINT(RT_DEBUG_TRACE, "ApCli AuthMode=%d, disable Pairwise Key Table\n", pAd->ApCliTab.ApCliEntry[0].AuthMode);
    }
    RTMP_IO_WRITE32(pAd, SEC_CSR4, sec_csr4);

	DBGPRINT(RT_DEBUG_TRACE, "Set_AuthMode_Proc::(AuthMode=%d, sec_csr4=0x%x)\n", pAd->ApCliTab.ApCliEntry[0].AuthMode, sec_csr4);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Encryption Type
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ApCli_EncrypType_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	
	if ((strcmp(arg, "NONE") == 0) || (strcmp(arg, "none") == 0))
		pAd->ApCliTab.ApCliEntry[0].PairCipher = Ndis802_11WEPDisabled;
	else if ((strcmp(arg, "WEP") == 0) || (strcmp(arg, "wep") == 0))
		pAd->ApCliTab.ApCliEntry[0].PairCipher = Ndis802_11WEPEnabled;
	else if ((strcmp(arg, "TKIP") == 0) || (strcmp(arg, "tkip") == 0))
		pAd->ApCliTab.ApCliEntry[0].PairCipher= Ndis802_11Encryption2Enabled;
	else if ((strcmp(arg, "AES") == 0) || (strcmp(arg, "aes") == 0))
		pAd->ApCliTab.ApCliEntry[0].PairCipher = Ndis802_11Encryption3Enabled;
	else
		return FALSE;

    RTMPApCliMakeRSNIE(pAd, pAd->ApCliTab.ApCliEntry[0].AuthMode, pAd->ApCliTab.ApCliEntry[0].PairCipher, 0);					        						  
	DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_EncrypType_Proc::(EncrypType=%d)\n", pAd->IoctlIF, pAd->ApCliTab.ApCliEntry[0].PairCipher);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Default Key ID
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ApCli_DefaultKeyID_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG KeyIdx;
	KeyIdx = simple_strtol(arg, 0, 10);
	if((KeyIdx >= 1 ) && (KeyIdx <= 4))
		pAd->ApCliTab.ApCliEntry[0].DefaultKeyId = (UCHAR) (KeyIdx - 1 );
	else
		return FALSE;  //Invalid argument 
	
	DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_DefaultKeyID_Proc::(DefaultKeyID(0~3)=%d)\n", pAd->IoctlIF, pAd->ApCliTab.ApCliEntry[0].DefaultKeyId);

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set WEP KEY1
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ApCli_Key1_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	INT		KeyLen;
	INT		i;
	UCHAR	CipherAlg = CIPHER_WEP64;
	UCHAR   WepKeyType = WEP_ASCII_TYPE;
	KeyLen = strlen(arg);
	switch (KeyLen)
	{
		case 5: //wep 40 Ascii type
			pAd->ApCliTab.ApCliEntry[0].SharedKey[0].KeyLen = KeyLen;
			memcpy(pAd->ApCliTab.ApCliEntry[0].SharedKey[0].Key, arg, KeyLen);
			CipherAlg = CIPHER_WEP64;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key1_Proc::(Key1=%s and type=%s)\n", pAd->IoctlIF, arg, "Ascii");		
			break;
		case 10: //wep 40 Hex type
			for(i=0; i < KeyLen; i++)
			{
				if( !isxdigit(*(arg+i)) )
					return FALSE;  //Not Hex value;
			}
			pAd->ApCliTab.ApCliEntry[0].SharedKey[0].KeyLen = KeyLen/2 ;
			AtoH(arg, pAd->ApCliTab.ApCliEntry[0].SharedKey[0].Key, KeyLen/2);
			CipherAlg = CIPHER_WEP64;
			WepKeyType = WEP_HEXADECIMAL_TYPE;
			DBGPRINT(RT_DEBUG_TRACE, "Set_Key1_Proc::(Key1=%s and type=%s)\n", arg, "Hex");		
			break;
		case 13: //wep 104 Ascii type
			pAd->ApCliTab.ApCliEntry[0].SharedKey[0].KeyLen = KeyLen;
			memcpy(pAd->ApCliTab.ApCliEntry[0].SharedKey[0].Key, arg, KeyLen);
			CipherAlg = CIPHER_WEP128;
			DBGPRINT(RT_DEBUG_TRACE, "Set_Key1_Proc::(Key1=%s and type=%s)\n", arg, "Ascii");		
			break;
		case 26: //wep 104 Hex type
			for(i=0; i < KeyLen; i++)
			{
				if( !isxdigit(*(arg+i)) )
					return FALSE;  //Not Hex value;
			}
			pAd->ApCliTab.ApCliEntry[0].SharedKey[0].KeyLen = KeyLen/2 ;
			AtoH(arg, pAd->ApCliTab.ApCliEntry[0].SharedKey[0].Key, KeyLen/2);
			CipherAlg = CIPHER_WEP128;
			WepKeyType = WEP_HEXADECIMAL_TYPE;
			DBGPRINT(RT_DEBUG_TRACE, "Set_Key1_Proc::(Key1=%s and type=%s)\n",  arg, "Hex");		
			break;
		default: //Invalid argument 
			pAd->ApCliTab.ApCliEntry[0].SharedKey[0].KeyLen = 0;
			DBGPRINT(RT_DEBUG_TRACE, "Set_Key1_Proc::Invalid argument (=%s)\n", arg);		
			return FALSE;
	}

	pAd->ApCliTab.ApCliEntry[0].SharedKey[0].CipherAlg = CipherAlg;
	pAd->ApCliTab.ApCliEntry[0].SharedKey[0].WepKeyType =WepKeyType;
	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set WEP KEY2
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ApCli_Key2_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	INT		KeyLen;
	INT		i;
	UCHAR	CipherAlg = CIPHER_WEP64;
	UCHAR   WepKeyType = WEP_ASCII_TYPE;
	KeyLen = strlen(arg);

	switch (KeyLen)
	{
		case 5: //wep 40 Ascii type
			pAd->ApCliTab.ApCliEntry[0].SharedKey[1].KeyLen = KeyLen;
			memcpy(pAd->ApCliTab.ApCliEntry[0].SharedKey[1].Key, arg, KeyLen);
			CipherAlg = CIPHER_WEP64;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key2_Proc::(Key2=%s and type=%s)\n", pAd->IoctlIF, arg, "Ascii");		
			break;
		case 10: //wep 40 Hex type
			for(i=0; i < KeyLen; i++)
			{
				if( !isxdigit(*(arg+i)) )
					return FALSE;  //Not Hex value;
			}
			pAd->ApCliTab.ApCliEntry[0].SharedKey[1].KeyLen = KeyLen/2 ;
			AtoH(arg, pAd->ApCliTab.ApCliEntry[0].SharedKey[1].Key, KeyLen/2);
			WepKeyType = WEP_HEXADECIMAL_TYPE;
			CipherAlg = CIPHER_WEP64;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key2_Proc::(Key2=%s and type=%s)\n", pAd->IoctlIF, arg, "Hex");		
			break;
		case 13: //wep 104 Ascii type
			pAd->ApCliTab.ApCliEntry[0].SharedKey[1].KeyLen = KeyLen;
			memcpy(pAd->ApCliTab.ApCliEntry[0].SharedKey[1].Key, arg, KeyLen);
			CipherAlg = CIPHER_WEP128;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key2_Proc::(Key2=%s and type=%s)\n", pAd->IoctlIF, arg, "Ascii");		
			break;
		case 26: //wep 104 Hex type
			for(i=0; i < KeyLen; i++)
			{
				if( !isxdigit(*(arg+i)) )
					return FALSE;  //Not Hex value;
			}
			pAd->ApCliTab.ApCliEntry[0].SharedKey[1].KeyLen = KeyLen/2 ;
			WepKeyType = WEP_HEXADECIMAL_TYPE;
			AtoH(arg, pAd->ApCliTab.ApCliEntry[0].SharedKey[1].Key, KeyLen/2);
			CipherAlg = CIPHER_WEP128;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key2_Proc::(Key2=%s and type=%s)\n", pAd->IoctlIF, arg, "Hex");		
			break;
		default: //Invalid argument 
			pAd->ApCliTab.ApCliEntry[0].SharedKey[1].KeyLen = 0;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key2_Proc::Invalid argument (=%s)\n", pAd->IoctlIF, arg);		
			return FALSE;
	}

	pAd->ApCliTab.ApCliEntry[0].SharedKey[1].CipherAlg = CipherAlg;
	pAd->ApCliTab.ApCliEntry[0].SharedKey[1].WepKeyType =WepKeyType;
	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set WEP KEY3
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ApCli_Key3_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	INT		KeyLen;
	INT		i;
	UCHAR	CipherAlg = CIPHER_WEP64;
	UCHAR   WepKeyType = WEP_ASCII_TYPE;
	KeyLen = strlen(arg);

	switch (KeyLen)
	{
		case 5: //wep 40 Ascii type
			pAd->ApCliTab.ApCliEntry[0].SharedKey[2].KeyLen = KeyLen;
			memcpy(pAd->ApCliTab.ApCliEntry[0].SharedKey[2].Key, arg, KeyLen);
			CipherAlg = CIPHER_WEP64;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key3_Proc::(Key3=%s and type=%s)\n", pAd->IoctlIF, arg, "Ascii");		
			break;
		case 10: //wep 40 Hex type
			for(i=0; i < KeyLen; i++)
			{
				if( !isxdigit(*(arg+i)) )
					return FALSE;  //Not Hex value;
			}
			pAd->ApCliTab.ApCliEntry[0].SharedKey[2].KeyLen = KeyLen/2 ;
			AtoH(arg, pAd->ApCliTab.ApCliEntry[0].SharedKey[2].Key, KeyLen/2);
			WepKeyType = WEP_HEXADECIMAL_TYPE;
			CipherAlg = CIPHER_WEP64;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key3_Proc::(Key3=%s and type=%s)\n", pAd->IoctlIF, arg, "Hex");		
			break;
		case 13: //wep 104 Ascii type
			pAd->ApCliTab.ApCliEntry[0].SharedKey[2].KeyLen = KeyLen;
			memcpy(pAd->ApCliTab.ApCliEntry[0].SharedKey[2].Key, arg, KeyLen);
			CipherAlg = CIPHER_WEP128;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key3_Proc::(Key3=%s and type=%s)\n", pAd->IoctlIF, arg, "Ascii");		
			break;
		case 26: //wep 104 Hex type
			for(i=0; i < KeyLen; i++)
			{
				if( !isxdigit(*(arg+i)) )
					return FALSE;  //Not Hex value;
			}
			pAd->ApCliTab.ApCliEntry[0].SharedKey[2].KeyLen = KeyLen/2 ;
			AtoH(arg, pAd->ApCliTab.ApCliEntry[0].SharedKey[2].Key, KeyLen/2);
			WepKeyType = WEP_HEXADECIMAL_TYPE;
			CipherAlg = CIPHER_WEP128;
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Key3_Proc::(Key3=%s and type=%s)\n", pAd->IoctlIF, arg, "Hex");		
			break;
		default: //Invalid argument 
			pAd->ApCliTab.ApCliEntry[0].SharedKey[2].KeyLen = 0;
			DBGPRINT(RT_DEBUG_ERROR, "IF(ra%d) Set_Key3_Proc::Invalid argument (=%s)\n", pAd->IoctlIF, arg);		
			return FALSE;
	}

	pAd->ApCliTab.ApCliEntry[0].SharedKey[2].CipherAlg = CipherAlg;
	pAd->ApCliTab.ApCliEntry[0].SharedKey[2].WepKeyType =WepKeyType;
	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set WEP KEY4
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ApCli_Key4_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	INT		KeyLen;
	INT		i;
	UCHAR	CipherAlg = CIPHER_WEP64;
	UCHAR   WepKeyType = WEP_ASCII_TYPE;
	KeyLen = strlen(arg);

	switch (KeyLen)
	{
		case 5: //wep 40 Ascii type
			pAd->ApCliTab.ApCliEntry[0].SharedKey[3].KeyLen = KeyLen;
			memcpy(pAd->ApCliTab.ApCliEntry[0].SharedKey[3].Key, arg, KeyLen);
			CipherAlg = CIPHER_WEP64;
			DBGPRINT(RT_DEBUG_TRACE, "Set_Key4_Proc::(Key4=%s and type=%s)\n",  arg, "Ascii");		
			break;
		case 10: //wep 40 Hex type
			for(i=0; i < KeyLen; i++)
			{
				if( !isxdigit(*(arg+i)) )
					return FALSE;  //Not Hex value;
			}
			pAd->ApCliTab.ApCliEntry[0].SharedKey[3].KeyLen = KeyLen/2 ;
			AtoH(arg, pAd->ApCliTab.ApCliEntry[0].SharedKey[3].Key, KeyLen/2);
			WepKeyType = WEP_HEXADECIMAL_TYPE;
			CipherAlg = CIPHER_WEP64;
			DBGPRINT(RT_DEBUG_TRACE, "Set_Key4_Proc::(Key4=%s and type=%s)\n",  arg, "Hex");		
			break;
		case 13: //wep 104 Ascii type
			pAd->ApCliTab.ApCliEntry[0].SharedKey[3].KeyLen = KeyLen;
			memcpy(pAd->ApCliTab.ApCliEntry[0].SharedKey[3].Key, arg, KeyLen);
			CipherAlg = CIPHER_WEP128;
			DBGPRINT(RT_DEBUG_TRACE, "Set_Key4_Proc::(Key4=%s and type=%s)\n", arg, "Ascii");		
			break;
		case 26: //wep 104 Hex type
			for(i=0; i < KeyLen; i++)
			{
				if( !isxdigit(*(arg+i)) )
					return FALSE;  //Not Hex value;
			}
			pAd->ApCliTab.ApCliEntry[0].SharedKey[3].KeyLen = KeyLen/2 ;
			AtoH(arg, pAd->ApCliTab.ApCliEntry[0].SharedKey[3].Key, KeyLen/2);
			WepKeyType = WEP_HEXADECIMAL_TYPE;
			CipherAlg = CIPHER_WEP128;
			DBGPRINT(RT_DEBUG_TRACE, "Set_Key4_Proc::(Key4=%s and type=%s)\n", arg, "Hex");		
			break;
		default: //Invalid argument 
			pAd->ApCliTab.ApCliEntry[0].SharedKey[3].KeyLen = 0;
			DBGPRINT(RT_DEBUG_ERROR, "Set_Key4_Proc::Invalid argument (=%s)\n", arg);		
			return FALSE;
	}

	pAd->ApCliTab.ApCliEntry[0].SharedKey[3].CipherAlg = CipherAlg;
	pAd->ApCliTab.ApCliEntry[0].SharedKey[3].WepKeyType =WepKeyType;
	return TRUE;
}

// For AP Client support WPS Modification
#ifdef WSC_SUPPORT
INT Set_WscSsid_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	UCHAR ifIndex = pAd->ApcliIF;
	PWSC_CTRL	    pWscControl = &pAd->ApCliTab.ApCliEntry[ifIndex].WscControl;

	NdisZeroMemory(&pWscControl->WscSsid, sizeof(NDIS_802_11_SSID));

	if( (strlen(arg) > 0) && (strlen(arg) <= MAX_LEN_OF_SSID))
	{
		NdisMoveMemory(pWscControl->WscSsid.Ssid, arg, strlen(arg));
		pWscControl->WscSsid.SsidLength = strlen(arg);

		DBGPRINT(RT_DEBUG_TRACE, "Set_WscSsid_Proc:: (Select SsidLen=%d,Ssid=%s)\n", 
				pWscControl->WscSsid.SsidLength, pWscControl->WscSsid.Ssid);
	}
	else
		return FALSE;	//Invalid argument 

	return TRUE;	

}
#endif // WSC_SUPPORT //
// --
#endif // APCLI_SUPPORT


#ifdef WSC_SUPPORT
INT	Set_WscConfMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	INT ConfModeIdx;
// For AP Client support WPS Modification
	UCHAR	    apidx;
	BOOLEAN     bFromApCli = FALSE;
	PWSC_CTRL   pWscControl;
// --

	ConfModeIdx = simple_strtol(arg, 0, 10);

// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
	if (pAd->bFromApcli)
	{
		if (pAd->ApcliIF != MAIN_MBSSID)
		{
			DBGPRINT(RT_DEBUG_TRACE, "IF(apcli%d) Set_WscConfMode_Proc:: Only support WPS in apcli0 now.\n", pAd->ApcliIF);
			return FALSE;
		}
		apidx = pAd->ApcliIF;
		bFromApCli = TRUE;
		pWscControl = &pAd->ApCliTab.ApCliEntry[apidx].WscControl;
	}
	else
#endif // APCLI_SUPPORT //
// --
    // Only support WPS in ra0 now, 2006.11.10
	{
		if (pAd->IoctlIF != MAIN_MBSSID)
		{
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_WscConfMode_Proc:: Only support WPS in ra0 now.\n", pAd->IoctlIF);
			return FALSE;
		}
// For AP Client support WPS Modification
		apidx = pAd->IoctlIF;
		bFromApCli = FALSE;
		pWscControl = &pAd->PortCfg.MBSSID[apidx].WscControl;
// --
	}

    pWscControl->bWscTrigger = FALSE;
    if ((ConfModeIdx & WSC_AP_Enrollee_PROXY_REGISTRAR) == WSC_DISABLE)
    {
		pWscControl->WscConfMode = WSC_DISABLE;
// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
		if (bFromApCli)
		{
			DBGPRINT(RT_DEBUG_TRACE, "IF(apcli%d) Set_WscConfMode_Proc:: WPS is disabled.\n", apidx);
		}
		else
#endif // APCLI_SUPPORT //
// --
		{
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_WscConfMode_Proc:: WPS is disabled.\n", apidx);

			// Clear WPS IE in Beacon and ProbeResp
			pAd->PortCfg.MBSSID[apidx].WscIEBeacon.ValueLen = 0;
			pAd->PortCfg.MBSSID[apidx].WscIEProbeResp.ValueLen = 0;
			MakeAllBssBeacon(pAd);
			UpdateAllBeaconFrame(pAd);
		}
    }
    else
    {
// For AP Client support WPS Modification
/*
    	pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.WscConfMode = (ConfModeIdx & WSC_AP_Enrollee_PROXY_REGISTRAR);
    	pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.WscStatus = STATUS_WSC_IDLE;
*/
#ifdef APCLI_SUPPORT
		if (bFromApCli)
		{
			if (ConfModeIdx == WSC_AP_Enrollee)
			{
			    pWscControl->WscConfMode = WSC_AP_Enrollee;
			    WscInit(pAd, TRUE, pWscControl);
			}
			else
			{
			    DBGPRINT(RT_DEBUG_TRACE, "IF(apcli%d) Set_WscConfMode_Proc:: Ap Client only supports Enrollee mode.(ConfModeIdx=%d)\n", apidx, ConfModeIdx);
			    return FALSE;
			}
		}
		else
#endif // APCLI_SUPPORT //
// --
		{
			pWscControl->WscConfMode = (ConfModeIdx & WSC_AP_Enrollee_PROXY_REGISTRAR);
			WscInit(pAd, FALSE, pWscControl);
		}
    }


	DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_WscConfMode_Proc::(WscConfMode(0~7)=%d)\n", pAd->IoctlIF, pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.WscConfMode);
	return TRUE;
}

INT	Set_WscConfStatus_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	UCHAR IsAPConfigured;
	INT IsSelectedRegistrar;
	USHORT WscMode;

// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
	if (pAd->bFromApcli)
	{
		DBGPRINT(RT_DEBUG_TRACE, "IF(apcli%d) Set_WscConfStatus_Proc:: Ap Client doesn't need this command.\n", pAd->ApcliIF);
		return FALSE;
	}
	else
#endif // APCLI_SUPPORT //
// --
    // Only support WPS in ra0 now, 2006.11.10
    if ((pAd->IoctlIF != MAIN_MBSSID) || 
        (pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.WscConfMode == WSC_DISABLE))
    {
        DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_WscConfStatus_Proc:: Only support WPS in ra0 now.\n", pAd->IoctlIF);
        DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_WscConfStatus_Proc:: WPS is %s.\n", 
                 pAd->IoctlIF,
                (pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.WscConfMode == WSC_DISABLE) ? "Disabled":"Enabled");
        return FALSE;
    }

	IsAPConfigured = (UCHAR)simple_strtol(arg, 0, 10);
	IsSelectedRegistrar = pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.WscSelReg;
    if (pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.WscMode == 1)
		WscMode = DEV_PASS_ID_PIN;
	else
		WscMode = DEV_PASS_ID_PBC;

	if ((IsAPConfigured  > 0) && (IsAPConfigured  <= 2))
    {   
        pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.WscConfStatus = IsAPConfigured;
        // Change SC State of WPS IE in Beacon and ProbeResp
        WscBuildBeaconIE(pAd, IsAPConfigured, IsSelectedRegistrar, WscMode, 0);
    	WscBuildProbeRespIE(pAd, WSC_MSGTYPE_AP_WLAN_MGR, IsAPConfigured, IsSelectedRegistrar, WscMode, 0);

    	MakeAllBssBeacon(pAd);
    	UpdateAllBeaconFrame(pAd);
    }
	else
	{
		DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_WscConfStatus_Proc:: Set failed!!(WscConfStatus=%s), WscConfStatus is 1 or 2 \n", pAd->IoctlIF, arg);
        DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_WscConfStatus_Proc:: WscConfStatus is not changed (%d) \n", pAd->IoctlIF, pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.WscConfStatus);
		return FALSE;  //Invalid argument	
	}

	DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_WscConfStatus_Proc::(WscConfStatus=%d)\n", pAd->IoctlIF, pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.WscConfStatus);

	return TRUE;
}

INT	Set_WscMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	INT WscMode;
// For AP Client support WPS Modification
	UCHAR	    apidx;
	PWSC_CTRL   pWscControl;
// --
    
// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
	if (pAd->bFromApcli)
	{
		if (pAd->ApcliIF != MAIN_MBSSID)
		{
			DBGPRINT(RT_DEBUG_TRACE, "IF(apcli%d) Set_WscMode_Proc:: Only support WPS in apcli0 now.\n", pAd->ApcliIF);
			return FALSE;
		}
		apidx = pAd->ApcliIF;
		pWscControl = &pAd->ApCliTab.ApCliEntry[apidx].WscControl;
	}
	else
#endif // APCLI_SUPPORT //
// --
    // Only support WPS in ra0 now, 2006.11.10
	{
		if (pAd->IoctlIF != MAIN_MBSSID)
		{
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_WscMode_Proc:: Only support WPS in ra0 now.\n", pAd->IoctlIF);
			return FALSE;
		}
// For AP Client support WPS Modification
		apidx = pAd->IoctlIF;
		pWscControl = &pAd->PortCfg.MBSSID[apidx].WscControl;
// --
	}

	WscMode = simple_strtol(arg, 0, 10);
    
    if ((WscMode  > 0) && (WscMode  <= 2))
    {
        pWscControl->WscMode = WscMode;
        if (WscMode == 2)
	        WscGetRegDataPIN(pAd, pWscControl->WscPinCode, pWscControl);
    }
	else
	{
		DBGPRINT(RT_DEBUG_TRACE, "Set_WscMode_Proc:: Set failed!!(Set_WscMode_Proc=%s), WscConfStatus is 1 or 2 \n", arg);
        	DBGPRINT(RT_DEBUG_TRACE, "Set_WscMode_Proc:: WscMode is not changed (%d) \n", pWscControl->WscMode);
		return FALSE;  //Invalid argument
	}

	DBGPRINT(RT_DEBUG_TRACE, "Set_WscMode_Proc::(WscMode=%d)\n", pWscControl->WscMode);

	return TRUE;
}

INT	Set_WscStatus_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
// For AP Client support WPS Modification
	PWSC_CTRL   pWscControl;

#ifdef APCLI_SUPPORT
	if (pAd->bFromApcli)
	{
		pWscControl = &pAd->ApCliTab.ApCliEntry[pAd->ApcliIF].WscControl;
	}
	else
#endif // APCLI_SUPPORT //
	{
		pWscControl = &pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl;
	}
// --
	DBGPRINT(RT_DEBUG_TRACE, "Set_WscStatus_Proc::(WscMode=%d)\n", pWscControl->WscStatus);
	return TRUE;
}

#define WSC_GET_CONF_MODE_EAP	1
#define WSC_GET_CONF_MODE_UPNP	2
INT	Set_WscGetConf_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	INT WscMode, wscGetConfMode = 0;
	INT IsAPConfigured;
	PWSC_CTRL pWscControl;
//	PWSC_UPNP_NODE_INFO pWscUPnPNodeInfo;
//    INT	idx, DH_Len;
    BOOLEAN             bFromApCli = FALSE;  // For AP Client support WPS Modification


    // Only support WPS in ra0 now, 2006.11.10
// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
	if (pAd->bFromApcli)
	{
		if (pAd->ApcliIF != MAIN_MBSSID)
		{
			DBGPRINT(RT_DEBUG_TRACE, "IF(apcli%d) Set_WscGetConf_Proc:: Only support WPS in ra0 or apcli0 now.\n", pAd->ApcliIF);
			return FALSE;
		}
		bFromApCli = TRUE;
	}
	else
#endif // APCLI_SUPPORT //
// --
    if (pAd->IoctlIF != MAIN_MBSSID)
    {
        DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_WscGetConf_Proc:: Only support WPS in ra0 or apcli0 now.\n", pAd->IoctlIF);
        return FALSE;
    }

// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
	if (bFromApCli)
	{
		pWscControl = &pAd->ApCliTab.ApCliEntry[pAd->ApcliIF].WscControl;
		DBGPRINT(RT_DEBUG_TRACE, "IF(apcli%d) Set_WscGetConf_Proc:: This command is from apcli interface now.\n", pAd->ApcliIF);
	}
	else
#endif // APCLI_SUPPORT //
// --
	{
		pWscControl = &pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl;
		IsAPConfigured = pWscControl->WscConfStatus;
	}

	wscGetConfMode = simple_strtol(arg, 0, 10);

//    pWscUPnPNodeInfo = &pWscControl->WscUPnPNodeInfo;

    if (pWscControl->WscConfMode == WSC_DISABLE)
    {
        pWscControl->bWscTrigger = FALSE;
        DBGPRINT(RT_DEBUG_TRACE, "Set_WscGetConf_Proc: WPS is disabled.\n");
		return FALSE;
    }

// For AP Client support WPS Modification
//	WscStop(pAd);
	WscStop(pAd, pWscControl, bFromApCli);

	// trigger wsc re-generate public key
	pWscControl->RegData.ReComputePke = 1;
// --
    
	if (pWscControl->WscMode == 1)
		WscMode = DEV_PASS_ID_PIN;
	else
		WscMode = DEV_PASS_ID_PBC;
    
	WscInitRegistrarPair(pAd, bFromApCli, pWscControl);
    // Enrollee 192 random bytes for DH key generation
/* remove by johnli, not use EnrolleeRandom
	for (idx = 0; idx < 192; idx++)
		pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.RegData.EnrolleeRandom[idx] = RandomByte(pAd);
*/
	// <<Low Power CPU>> ++, add by johnli
/*
	GenerateDHPublicKey(NULL, NULL, 0, pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.RegData.Pkr, &DH_Len);
	if(DH_Len)
		memcpy(pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.RegData.Pke, pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl.RegData.Pkr, DH_Len);
*/
 	// <<Low Power CPU>> --
 
// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT    
	if (bFromApCli)
	{
		BOOLEAN apcliEn = pAd->ApCliTab.ApCliEntry[pAd->ApcliIF].Enable;

		NdisMoveMemory(pWscControl->RegData.EnrolleeInfo.MacAddr,
		               pAd->ApCliTab.ApCliEntry[pAd->ApcliIF].CurrentAddress, 
		               6);

		// bring apcli interface down first

		if(apcliEn == TRUE )
		{
			pAd->ApCliTab.ApCliEntry[pAd->ApcliIF].Enable = FALSE;
			ApCliIfDown(pAd);
		}
		pAd->ApCliTab.ApCliEntry[pAd->ApcliIF].Enable = apcliEn;

	}
	else
#endif // APCLI_SUPPORT //
// --
	{
		WscBuildBeaconIE(pAd, IsAPConfigured, TRUE, WscMode, WSC_CONFIG_METHODS);
		WscBuildProbeRespIE(pAd, WSC_MSGTYPE_AP_WLAN_MGR, IsAPConfigured, TRUE, WscMode, WSC_CONFIG_METHODS);

		MakeAllBssBeacon(pAd);
		UpdateAllBeaconFrame(pAd);
	}

    // 2mins time-out timer
    RTMPAddTimer(&pWscControl->Wsc2MinsTimer, WSC_TWO_MINS_TIME_OUT);
    pWscControl->Wsc2MinsTimerRunning = TRUE;
// For AP Client support WPS Modification
    pWscControl->WscState = WSC_STATE_INIT;
// --
    pWscControl->WscStatus = STATUS_WSC_LINK_UP;

// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT    
    if (!bFromApCli)
#endif // APCLI_SUPPORT //
// --
	WscSendUPnPConfReqMsg(pAd, pAd->PortCfg.MBSSID[pAd->IoctlIF].Ssid, pAd->PortCfg.MBSSID[pAd->IoctlIF].Bssid, 3, 0);

    pWscControl->bWscTrigger = TRUE;
	DBGPRINT(RT_DEBUG_TRACE, "Set_WscGetConf_Proc trigger WSC state machine, wscGetConfMode=%d\n", wscGetConfMode);

	return TRUE;
}

INT	Set_WscPinCode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	UINT PinCode = 0;
	BOOLEAN validatePin;
// For AP Client support WPS Modification
	PWSC_CTRL   pWscControl;
// --
#define IsZero(c) ('0' == (c) ? TRUE:FALSE)
	PinCode = simple_strtol(arg, 0, 10); // When PinCode is 03571361, return value is 3571361.
	
// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
	if (pAd->bFromApcli)
	{
		if (pAd->ApcliIF != MAIN_MBSSID)
		{
			DBGPRINT(RT_DEBUG_TRACE, "IF(apcli%d) Set_WscPinCode_Proc:: Only support WPS in apcli0 now.\n", pAd->ApcliIF);
			return FALSE;
		}
		pWscControl = &pAd->ApCliTab.ApCliEntry[pAd->ApcliIF].WscControl;
	}
	else
#endif // APCLI_SUPPORT //
 	{
		if (pAd->IoctlIF != MAIN_MBSSID)
		{
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_WscPinCode_Proc:: Only support WPS in ra0 now.\n", pAd->IoctlIF);
			return FALSE;
		}
		pWscControl = &pAd->PortCfg.MBSSID[pAd->IoctlIF].WscControl;
	}
// --

	// <<Reject Same PinCode>> ++, edit by johnli
	if ((pWscControl->WscLastStatus != WSC_ERROR_NO_ERROR) &&
		(pWscControl->WscLastPinCode == (INT)PinCode))
	{
		DBGPRINT(RT_DEBUG_ERROR, "Set failed!!(Set_WscPinCode_Proc=%s), Reject the same PinCode \n", arg);
		return FALSE;
	}
	// <<Reject Same PinCode>> --
	
	validatePin = ValidateChecksum(PinCode);

	if ( validatePin )
	{
		pWscControl->WscPinCode = (INT)PinCode;
        // PIN Code
        if (IsZero(*arg))
            memcpy(pWscControl->RegData.PIN, arg, 8);
        else
	        WscGetRegDataPIN(pAd, pWscControl->WscPinCode, pWscControl);
	// <<Reject Same PinCode>> ++, edit by johnli
	pWscControl->WscLastPinCode = 0;
	pWscControl->WscLastStatus = WSC_ERROR_NO_ERROR;
	// <<Reject Same PinCode>> --
	}
	else
	{
		DBGPRINT(RT_DEBUG_TRACE, "Set failed!!(Set_WscPinCode_Proc=%s), PinCode Checksum invalid \n", arg);
		return FALSE;  //Invalid argument
	}

	DBGPRINT(RT_DEBUG_TRACE, "Set_WscPinCode_Proc::(PinCode=%d)\n", pWscControl->WscPinCode);

	return TRUE;
}

INT	Set_WscOOB_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
    char tempSsid[32] = {0};
	
// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
	if (pAd->bFromApcli)
	{
		DBGPRINT(RT_DEBUG_TRACE, "IF(apcli%d) Set_WscOOB_Proc:: Ap Client doesn't need this command.\n", pAd->ApcliIF);
		return FALSE;
	}
	else
#endif // APCLI_SUPPORT //
// --
    // Only support WPS in ra0 now, 2006.11.10
    if (pAd->IoctlIF != MAIN_MBSSID)
    {
        DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_WscOOB_Proc:: Only support WPS in ra0 now.\n", pAd->IoctlIF);
        return FALSE;
    }

    Set_WscConfStatus_Proc(pAd, "1");
    Set_AuthMode_Proc(pAd, "WPAPSK");
    Set_EncrypType_Proc(pAd, "TKIP");
	Set_DefaultKeyID_Proc(pAd, "2");	
    sprintf(tempSsid, "RalinkInitialAP%02X%02X%02X", pAd->PortCfg.MBSSID[pAd->IoctlIF].Bssid[3],
                                                     pAd->PortCfg.MBSSID[pAd->IoctlIF].Bssid[4],
                                                     pAd->PortCfg.MBSSID[pAd->IoctlIF].Bssid[5]);
    Set_SSID_Proc(pAd, tempSsid);
	Set_WPAPSK_Proc(pAd, "RalinkInitialAPxx1234");
    
	DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_WscOOB_Proc\n", pAd->IoctlIF);
	return TRUE;
}

INT	Set_WscOpenOOB_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
    char tempSsid[32] = {0};

// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
	if (pAd->bFromApcli)
	{
		DBGPRINT(RT_DEBUG_TRACE, "IF(apcli%d) Set_WscOpenOOB_Proc:: Ap Client doesn't need this command.\n", pAd->ApcliIF);
		return FALSE;
	}
	else
#endif // APCLI_SUPPORT //
// --
    // Only support WPS in ra0 now, 2006.11.10
    if (pAd->IoctlIF != MAIN_MBSSID)
    {
        DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_WscOpenOOB_Proc:: Only support WPS in ra0 now.\n", pAd->IoctlIF);
        return FALSE;
    }

    Set_WscConfStatus_Proc(pAd, "1");
    Set_AuthMode_Proc(pAd, "OPEN");
    Set_EncrypType_Proc(pAd, "NONE");
    sprintf(tempSsid, "RalinkInitialAP%02X%02X%02X", pAd->PortCfg.MBSSID[pAd->IoctlIF].Bssid[3],
                                                     pAd->PortCfg.MBSSID[pAd->IoctlIF].Bssid[4],
                                                     pAd->PortCfg.MBSSID[pAd->IoctlIF].Bssid[5]);
    Set_SSID_Proc(pAd, tempSsid);
    
	DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_WscOpenOOB_Proc\n", pAd->IoctlIF);
	return TRUE;
}

// For AP Client support WPS Modification
INT	Set_WscStop_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
    UCHAR	    apidx;
    PWSC_CTRL   pWscControl;
    BOOLEAN     bFromApCli = FALSE;
    
#ifdef APCLI_SUPPORT
	if (pAd->bFromApcli)
	{
		if (pAd->ApcliIF != MAIN_MBSSID)
		{
			DBGPRINT(RT_DEBUG_TRACE, "IF(apcli%d) Set_WscStop_Proc:: Only support WPS in apcli0 now.\n", pAd->ApcliIF);
			return FALSE;
		}
		apidx = pAd->ApcliIF;
		bFromApCli = TRUE;
		pWscControl = &pAd->ApCliTab.ApCliEntry[apidx].WscControl;
	}
	else
#endif // APCLI_SUPPORT //
    // Only support WPS in ra0 now, 2006.11.10
	{
		if (pAd->IoctlIF != MAIN_MBSSID)
		{
			DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_WscStop_Proc:: Only support WPS in ra0 now.\n", pAd->IoctlIF);
			return FALSE;
		}
		apidx = pAd->IoctlIF;
		bFromApCli = FALSE;
		pWscControl = &pAd->PortCfg.MBSSID[apidx].WscControl;
	}

    if (bFromApCli)
		WscStop(pAd, pWscControl, TRUE);
    else
    {
		INT	 IsAPConfigured = pWscControl->WscConfStatus;
		WscBuildBeaconIE(pAd, IsAPConfigured, FALSE, 0, 0);
		WscBuildProbeRespIE(pAd, WSC_MSGTYPE_AP_WLAN_MGR, IsAPConfigured, FALSE, 0, 0);
		MakeAllBssBeacon(pAd);
		UpdateAllBeaconFrame(pAd);
		WscStop(pAd, pWscControl, FALSE);
    }

    pWscControl->bWscTrigger = FALSE;
    DBGPRINT(RT_DEBUG_TRACE, "<===== Set_WscStop_Proc");
    return TRUE;
}
// --

#ifdef DBG
INT	Set_Test_With_Intel_SDK_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
    INT temp = simple_strtol(arg, 0, 10);
    g_b_test_with_intel_sdk = (temp == 0) ? FALSE:TRUE;

    DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) Set_Test_With_Intel_SDK_Proc: g_b_test_with_intel_sdk is %s\n",
                             pAd->IoctlIF,
                             g_b_test_with_intel_sdk ? "TRUE":"FALSE");
    return TRUE;
}
#endif

#endif // WSC_SUPPORT //
