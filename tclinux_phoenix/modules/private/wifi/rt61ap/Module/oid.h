/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2005, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attempt
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	oid.h

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Name		Date			Modification logs
*/

#ifndef _OID_H_
#define _OID_H_

#include <linux/wireless.h>

// Ralink defined OIDs
#if WIRELESS_EXT <= 11
#ifndef SIOCDEVPRIVATE
#define SIOCDEVPRIVATE                              0x8BE0
#endif
#define SIOCIWFIRSTPRIV								SIOCDEVPRIVATE
#endif

#define RT_PRIV_IOCTL								(SIOCIWFIRSTPRIV + 0x01)
#define RTPRIV_IOCTL_SET							(SIOCIWFIRSTPRIV + 0x02)
				
#ifdef DBG
#define RTPRIV_IOCTL_BBP                            (SIOCIWFIRSTPRIV + 0x03)
#define RTPRIV_IOCTL_MAC                            (SIOCIWFIRSTPRIV + 0x05)
#define RTPRIV_IOCTL_E2P                            (SIOCIWFIRSTPRIV + 0x07)
#define RTPRIV_IOCTL_CHK_IC_VERSION       			(SIOCIWFIRSTPRIV + 0x0B)
#endif
#define RTPRIV_IOCTL_STATISTICS                     (SIOCIWFIRSTPRIV + 0x09)
#define RTPRIV_IOCTL_ADD_PMKID_CACHE                (SIOCIWFIRSTPRIV + 0x0A)
#define RTPRIV_IOCTL_RADIUS_DATA                    (SIOCIWFIRSTPRIV + 0x0C)
#define RTPRIV_IOCTL_GSITESURVEY					(SIOCIWFIRSTPRIV + 0x0D)
#define RTPRIV_IOCTL_ADD_WPA_KEY                    (SIOCIWFIRSTPRIV + 0x0E)
#define RTPRIV_IOCTL_GET_MAC_TABLE					(SIOCIWFIRSTPRIV + 0x0F)
#define RTPRIV_IOCTL_STATIC_WEP_COPY                (SIOCIWFIRSTPRIV + 0x10)
// for consistency with RT2860
#define RTPRIV_IOCTL_WSC_PROFILE                    (SIOCIWFIRSTPRIV + 0x12) 

#define OID_GET_SET_TOGGLE							0x8000

#define RT_QUERY_ATE_TXDONE_COUNT					0x0401
#define RT_QUERY_SIGNAL_CONTEXT						0x0402
#define RT_SET_IAPP_PID                 			0x0404
#define RT_SET_APD_PID								0x0405
#define RT_SET_DEL_MAC_ENTRY						0x0406

//	OID for sofap copy from RT2560 Softap
#define RT_OID_802_11_EVENT_TABLE			0x0210
#define RT_OID_802_11_MAC_TABLE				0x0211
#define RT_OID_802_11_ACL					0x0216

#ifdef SNMP
//IEEE 802dot11
#define OID_802_11_NUMBER_OF_ANTENNAS               0x010B
#define OID_802_11_RX_ANTENNA_SELECTED              0x010C
#define OID_802_11_TX_ANTENNA_SELECTED              0x010D
#define OID_802_11_DESIRED_RATES                    0x050E
#define OID_802_11_RTS_THRESHOLD                    0x0514
#define OID_802_11_FRAGMENTATION_THRESHOLD          0x0515
#define OID_802_3_CURRENT_ADDRESS                   0x060A
#define OID_802_11_STATISTICS                       0x060E

//SNMP ieee 802dot11, kathy , 2006-0629
//dot11mac
#define RT_OID_802_11_MAC_ADDRESS					0x0713

// dot11res(3)
#define RT_OID_802_11_MANUFACTUREROUI				0x0700
#define RT_OID_802_11_MANUFACTURERNAME				0x0701
#define RT_OID_802_11_RESOURCETYPEIDNAME			0x0702


// dot11smt(1)
#define RT_OID_802_11_PRIVACYOPTIONIMPLEMENTED		0x0703
#define RT_OID_802_11_POWERMANAGEMENTMODE			0x0704
#define OID_802_11_WEPDEFAULTKEYVALUE				0x0705 // read , write
#define OID_802_11_WEPDEFAULTKEYID					0x0706
#define RT_OID_802_11_WEPKEYMAPPINGLENGTH			0x0707
#define OID_802_11_SHORTRETRYLIMIT					0x0708
#define OID_802_11_LONGRETRYLIMIT					0x0709
#define RT_OID_802_11_PRODUCTID						0x0710
#define RT_OID_802_11_MANUFACTUREID					0x0711

// //dot11Phy(4)
#define OID_802_11_CURRENTCHANNEL							0x0712
//end snmp

#endif  //snmp

#ifdef WSC_SUPPORT
// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
#define RT_OID_APCLI_WSC_PIN_CODE					0x0749
#endif // APCLI_SUPPORT //
#define RT_OID_802_11_WSC_QUERY_PROFILE				0x0750
// --
#define RT_OID_WSC_QUERY_STATUS						0x0751
#define RT_OID_WSC_PIN_CODE							0x0752
#define RT_OID_WSC_UUID								0x0753
#define RT_OID_WSC_SET_SELECTED_REGISTRAR			0x0754
#define RT_OID_WSC_EAPMSG							0x0755
#define RT_OID_WSC_MANUFACTURER						0x0756
#define RT_OID_WSC_MODEL_NAME						0x0757
#define RT_OID_WSC_MODEL_NO							0x0758
#define RT_OID_WSC_SERIAL_NO						0x0759
#define RT_OID_WSC_MAC_ADDRESS						0x0760
#endif // WSC_SUPPORT //

#ifdef LLTD_SUPPORT
#define RT_OID_GET_PHY_MODE                         0x0761
#define RT_OID_GET_LLTD_ASSO_TANLE                  0x0762
#endif /* LLTD_SUPPORT */

#ifdef APCLI_SUPPORT
#define RT_OID_GET_REPEATER_AP_LINEAGE              0x0763
#define RT_OID_GET_REPEATER_AP_TABLE                0x0764
#endif /* APCLI_SUPPORT */

#ifndef OID_802_11_TEST
#define OID_802_11_TEST                       0x0D010120
#endif
#define RT_OID_802_11_BSSID                   (OID_GET_SET_TOGGLE | OID_802_11_BSSID)
#define RT_OID_802_11_SSID                    (OID_GET_SET_TOGGLE | OID_802_11_SSID)
#define RT_OID_802_11_INFRASTRUCTURE_MODE     (OID_GET_SET_TOGGLE | OID_802_11_INFRASTRUCTURE_MODE)
#define RT_OID_802_11_ADD_WEP                 (OID_GET_SET_TOGGLE | OID_802_11_ADD_WEP)
#define RT_OID_802_11_ADD_KEY                 (OID_GET_SET_TOGGLE | OID_802_11_ADD_KEY)
#define RT_OID_802_11_REMOVE_WEP              (OID_GET_SET_TOGGLE | OID_802_11_REMOVE_WEP)
#define RT_OID_802_11_REMOVE_KEY              (OID_GET_SET_TOGGLE | OID_802_11_REMOVE_KEY)
#define RT_OID_802_11_DISASSOCIATE            (OID_GET_SET_TOGGLE | OID_802_11_DISASSOCIATE)
#define RT_OID_802_11_AUTHENTICATION_MODE     (OID_GET_SET_TOGGLE | OID_802_11_AUTHENTICATION_MODE)
#define RT_OID_802_11_PRIVACY_FILTER          (OID_GET_SET_TOGGLE | OID_802_11_PRIVACY_FILTER)
#define RT_OID_802_11_BSSID_LIST_SCAN         (OID_GET_SET_TOGGLE | OID_802_11_BSSID_LIST_SCAN)
#define RT_OID_802_11_WEP_STATUS              (OID_GET_SET_TOGGLE | OID_802_11_WEP_STATUS)
#define RT_OID_802_11_RELOAD_DEFAULTS         (OID_GET_SET_TOGGLE | OID_802_11_RELOAD_DEFAULTS)
#define RT_OID_802_11_NETWORK_TYPE_IN_USE     (OID_GET_SET_TOGGLE | OID_802_11_NETWORK_TYPE_IN_USE)
#define RT_OID_802_11_TX_POWER_LEVEL          (OID_GET_SET_TOGGLE | OID_802_11_TX_POWER_LEVEL)
#define RT_OID_802_11_RSSI_TRIGGER            (OID_GET_SET_TOGGLE | OID_802_11_RSSI_TRIGGER)
#define RT_OID_802_11_FRAGMENTATION_THRESHOLD (OID_GET_SET_TOGGLE | OID_802_11_FRAGMENTATION_THRESHOLD)
#define RT_OID_802_11_RTS_THRESHOLD           (OID_GET_SET_TOGGLE | OID_802_11_RTS_THRESHOLD)
#define RT_OID_802_11_RX_ANTENNA_SELECTED     (OID_GET_SET_TOGGLE | OID_802_11_RX_ANTENNA_SELECTED)
#define RT_OID_802_11_TX_ANTENNA_SELECTED     (OID_GET_SET_TOGGLE | OID_802_11_TX_ANTENNA_SELECTED)
#define RT_OID_802_11_SUPPORTED_RATES         (OID_GET_SET_TOGGLE | OID_802_11_SUPPORTED_RATES)
#define RT_OID_802_11_DESIRED_RATES           (OID_GET_SET_TOGGLE | OID_802_11_DESIRED_RATES)
#define RT_OID_802_11_CONFIGURATION           (OID_GET_SET_TOGGLE | OID_802_11_CONFIGURATION)
#define RT_OID_802_11_POWER_MODE              (OID_GET_SET_TOGGLE | OID_802_11_POWER_MODE)

//snmp
/*#define RT_OID_802_11_SHORTRETRYLIMIT				(OID_GET_SET_TOGGLE | OID_802_11_SHORTRETRYLIMIT)
#define RT_OID_802_11_LONGRETRYLIMIT				(OID_GET_SET_TOGGLE | OID_802_11_LONGRETRYLIMIT)
#define RT_OID_802_11_WEPDEFAULTKEYVALUE			(OID_GET_SET_TOGGLE | OID_802_11_WEPDEFAULTKEYVALUE)
#define RT_OID_802_11_WEPDEFAULTKEYID				(OID_GET_SET_TOGGLE | OID_802_11_WEPDEFAULTKEYID)
*/



//
// IEEE 802.11 Structures and definitions
//
// new types for Media Specific Indications

#define NDIS_802_11_LENGTH_SSID         32
#define NDIS_802_11_LENGTH_RATES        8
#define NDIS_802_11_LENGTH_RATES_EX     16


//
// IEEE 802.11 Structures and definitions
//

typedef UCHAR   NDIS_802_11_MAC_ADDRESS[6];

// Add new authentication modes
typedef enum _NDIS_802_11_AUTHENTICATION_MODE
{
	Ndis802_11AuthModeOpen,
	Ndis802_11AuthModeShared,
	Ndis802_11AuthModeAutoSwitch,
	Ndis802_11AuthModeWPA,
	Ndis802_11AuthModeWPAPSK,
	Ndis802_11AuthModeWPANone,
	Ndis802_11AuthModeWPA2,
	Ndis802_11AuthModeWPA2PSK,
	Ndis802_11AuthModeWPA1WPA2,
	Ndis802_11AuthModeWPAPSKWPA2PSK,
	Ndis802_11AuthModeMax           // Not a real mode, defined as upper bound
} NDIS_802_11_AUTHENTICATION_MODE, *PNDIS_802_11_AUTHENTICATION_MODE;
typedef UCHAR NDIS_802_11_RATES[NDIS_802_11_LENGTH_RATES];        // Set of 8 data rates
typedef UCHAR NDIS_802_11_RATES_EX[NDIS_802_11_LENGTH_RATES_EX];  // Set of 16 data rates

typedef struct _NDIS_802_11_FIXED_IEs 
{
    UCHAR Timestamp[8];
    USHORT BeaconInterval;
    USHORT Capabilities;
} NDIS_802_11_FIXED_IEs, *PNDIS_802_11_FIXED_IEs;

typedef struct _NDIS_802_11_VARIABLE_IEs 
{
    UCHAR ElementID;
    UCHAR Length;    // Number of bytes in data field
    UCHAR data[1];
} NDIS_802_11_VARIABLE_IEs, *PNDIS_802_11_VARIABLE_IEs;

typedef enum _NDIS_802_11_PRIVACY_FILTER
{
   Ndis802_11PrivFilterAcceptAll,
   Ndis802_11PrivFilter8021xWEP
} NDIS_802_11_PRIVACY_FILTER, *PNDIS_802_11_PRIVACY_FILTER;

// Added new encryption types
// Also aliased typedef to new name
typedef enum _NDIS_802_11_WEP_STATUS
{
    Ndis802_11WEPEnabled,
    Ndis802_11Encryption1Enabled = Ndis802_11WEPEnabled,
    Ndis802_11WEPDisabled,
    Ndis802_11EncryptionDisabled = Ndis802_11WEPDisabled,
    Ndis802_11WEPKeyAbsent,
    Ndis802_11Encryption1KeyAbsent = Ndis802_11WEPKeyAbsent,
    Ndis802_11WEPNotSupported,
    Ndis802_11EncryptionNotSupported = Ndis802_11WEPNotSupported,
    Ndis802_11Encryption2Enabled,
    Ndis802_11Encryption2KeyAbsent,
    Ndis802_11Encryption3Enabled,
    Ndis802_11Encryption3KeyAbsent,
    Ndis802_11Encryption4Enabled,	// TKIP or AES mix
    Ndis802_11Encryption4KeyAbsent,
} NDIS_802_11_WEP_STATUS, *PNDIS_802_11_WEP_STATUS,
  NDIS_802_11_ENCRYPTION_STATUS, *PNDIS_802_11_ENCRYPTION_STATUS;

typedef enum _RT_802_11_PREAMBLE {
    Rt802_11PreambleLong,
    Rt802_11PreambleShort,
    Rt802_11PreambleAuto
} RT_802_11_PREAMBLE, *PRT_802_11_PREAMBLE;

typedef enum _RT_802_11_PHY_MODE {
    PHY_11BG_MIXED,
    PHY_11B,
    PHY_11G,
    PHY_11A,
    PHY_11ABG_MIXED,
} RT_802_11_PHY_MODE;

// Key mapping keys require a BSSID
typedef struct _NDIS_802_11_KEY
{
    ULONG           Length;             // Length of this structure
    UCHAR			addr[6];
    ULONG           KeyIndex;           
    ULONG           KeyLength;          // length of key in bytes
    UCHAR           KeyMaterial[1];     // variable length depending on above field
} NDIS_802_11_KEY, *PNDIS_802_11_KEY;

typedef struct _RT_802_11_EVENT_LOG {
    UMAXLONG        SystemTime;  // timestammp via NdisGetCurrentSystemTime()
    UCHAR           Addr[ETH_LENGTH_OF_ADDRESS];
    UCHAR           ApIdx;
    USHORT          Event;       // EVENT_xxx
} RT_802_11_EVENT_LOG, *PRT_802_11_EVENT_LOG;

typedef struct _RT_802_11_EVENT_TABLE {
    ULONG       Num;
    ULONG       Rsv;     // to align Log[] at LARGE_INEGER boundary
    RT_802_11_EVENT_LOG   Log[MAX_NUM_OF_EVENT];
} RT_802_11_EVENT_TABLE, PRT_802_11_EVENT_TABLE;

typedef struct _COUNTER_HOTSPOT {
    // UMAXLONG           LinkUpTime;
    UMAXLONG           LastDataPacketTime;
    UMAXLONG           TotalTxByteCount;
    UMAXLONG           TotalRxByteCount;
} COUNTER_HOTSPOT, *PCOUNTER_HOTSPOT;

typedef struct _RT_802_11_MAC_ENTRY {
    UCHAR       Addr[ETH_LENGTH_OF_ADDRESS];
    UCHAR       Aid;
    UCHAR       Psm;     // 0:PWR_ACTIVE, 1:PWR_SAVE
    COUNTER_HOTSPOT HSCounter;
} RT_802_11_MAC_ENTRY, *PRT_802_11_MAC_ENTRY;

typedef struct _RT_802_11_MAC_TABLE {
    ULONG       Num;
    RT_802_11_MAC_ENTRY Entry[MAX_LEN_OF_MAC_TABLE];
} RT_802_11_MAC_TABLE, *PRT_802_11_MAC_TABLE;

typedef struct _RT_802_11_ACL_ENTRY {
    UCHAR   Addr[ETH_LENGTH_OF_ADDRESS];
    USHORT  Rsv;
} RT_802_11_ACL_ENTRY, *PRT_802_11_ACL_ENTRY;

typedef struct _RT_802_11_ACL {
    ULONG    Policy;             // 0-disable, 1-positive list, 2-negative list
    ULONG    Num;
    RT_802_11_ACL_ENTRY Entry[MAX_NUM_OF_ACL_LIST];
} RT_802_11_ACL, *PRT_802_11_ACL;

typedef struct _RT_802_11_AP_CONFIG {
    ULONG   EnableTxBurst;      // 0-disable, 1-enable
    ULONG   EnableTurboRate;    // 0-disable, 1-enable 72/100mbps turbo rate
    ULONG   IsolateInterStaTraffic;     // 0-disable, 1-enable isolation
    ULONG   HideSsid;           // 0-disable, 1-enable hiding
    ULONG   UseBGProtection;    // 0-AUTO, 1-always ON, 2-always OFF
    ULONG   UseShortSlotTime;   // 0-no use, 1-use 9-us short slot time
    ULONG   Rsv1;               // must be 0
    ULONG   SystemErrorBitmap;  // ignore upon SET, return system error upon QUERY
} RT_802_11_AP_CONFIG, *PRT_802_11_AP_CONFIG;
typedef struct _RT_802_11_WDS {
    ULONG						Num;
    NDIS_802_11_MAC_ADDRESS		Entry[MAX_WDS_ENTRY];
	ULONG						KeyLength;
	UCHAR						KeyMaterial[32];
} RT_802_11_WDS, *PRT_802_11_WDS;

//
// Defines the state of the LAN media
//
typedef enum _NDIS_MEDIA_STATE
{
    NdisMediaStateConnected,
    NdisMediaStateDisconnected
} NDIS_MEDIA_STATE, *PNDIS_MEDIA_STATE;

// structure for PMKID 
typedef	UCHAR	NDIS_802_11_PMKID_VALUE[16];

typedef	struct	_BSSID_INFO
{
	NDIS_802_11_MAC_ADDRESS		MAC;
	NDIS_802_11_PMKID_VALUE		PMKID;
	UCHAR                       PMK[32];
	UMAXLONG					RefreshTime;
	BOOLEAN						Valid;
}	BSSID_INFO, *PBSSID_INFO;

#define MAX_PMKID_COUNT		8
typedef	struct	_NDIS_802_11_PMKID
{
	BSSID_INFO	BSSIDInfo[MAX_PMKID_COUNT];
}	NDIS_802_11_PMKID, *PNDIS_802_11_PMKID;

typedef struct _NDIS_802_11_SITE_SURVEY_TABLE
{
   LONG         Channel;
   LONG         Rssi;
   UCHAR        Ssid[33];
   UCHAR        Bssid[MAC_ADDR_LEN];
   UCHAR        EncrypT[8];
} NDIS_802_11_SITE_SURVEY_TABLE, *PNDIS_802_11_SITE_SURVEY_TABLE;

typedef struct _NDIS_802_11_STATION_TABLE
{
   UCHAR        MacAddr[MAC_ADDR_LEN];
   ULONG        Aid;
   ULONG        PsMode;
   UMAXLONG     LastDataPacketTime;
   UMAXLONG     RxByteCount;
   UMAXLONG     TxByteCount;
   ULONG        CurrTxRate;
   ULONG        LastTxRate;
} NDIS_802_11_STATION_TABLE, *PNDIS_802_11_STATION_TABLE;
// add for UI
typedef struct PACKED _RT_VERSION_INFO{
    UCHAR       DriverVersionW;
    UCHAR       DriverVersionX;
    UCHAR       DriverVersionY;
    UCHAR       DriverVersionZ;
    UINT        DriverBuildYear;
    UINT        DriverBuildMonth;
    UINT        DriverBuildDay;
} RT_VERSION_INFO, *PRT_VERSION_INFO;

typedef struct _NDIS_802_11_STATISTICS
{
   ULONG           Length;             // Length of structure
   UMAXLONG   TransmittedFragmentCount;
   UMAXLONG   MulticastTransmittedFrameCount;
   UMAXLONG   FailedCount;
   UMAXLONG   RetryCount;
   UMAXLONG   MultipleRetryCount;
   UMAXLONG   RTSSuccessCount;
   UMAXLONG   RTSFailureCount;
   UMAXLONG   ACKFailureCount;
   UMAXLONG   FrameDuplicateCount;
   UMAXLONG   ReceivedFragmentCount;
   UMAXLONG   MulticastReceivedFrameCount;
   UMAXLONG   FCSErrorCount;
} NDIS_802_11_STATISTICS, *PNDIS_802_11_STATISTICS;

// Added new types for OFDM 5G and 2.4G
typedef enum _NDIS_802_11_NETWORK_TYPE
{
    Ndis802_11FH, 
    Ndis802_11DS, 
    Ndis802_11OFDM5,
    Ndis802_11OFDM24,
    Ndis802_11Automode,
    Ndis802_11NetworkTypeMax    // not a real type, defined as an upper bound
} NDIS_802_11_NETWORK_TYPE, *PNDIS_802_11_NETWORK_TYPE;

typedef struct _NDIS_802_11_CONFIGURATION_FH
{
   ULONG           Length;            // Length of structure
   ULONG           HopPattern;        // As defined by 802.11, MSB set 
   ULONG           HopSet;            // to one if non-802.11
   ULONG           DwellTime;         // units are Kusec
} NDIS_802_11_CONFIGURATION_FH, *PNDIS_802_11_CONFIGURATION_FH;

typedef struct _NDIS_802_11_CONFIGURATION
{
   ULONG                           Length;             // Length of structure
   ULONG                           BeaconPeriod;       // units are Kusec
   ULONG                           ATIMWindow;         // units are Kusec
   ULONG                           DSConfig;           // Frequency, units are kHz
   NDIS_802_11_CONFIGURATION_FH    FHConfig;
} NDIS_802_11_CONFIGURATION, *PNDIS_802_11_CONFIGURATION;

typedef struct _NDIS_802_11_SSID 
{
    ULONG   SsidLength;         // length of SSID field below, in bytes;
                                // this can be zero.
    UCHAR   Ssid[NDIS_802_11_LENGTH_SSID];           // SSID information field
} NDIS_802_11_SSID, *PNDIS_802_11_SSID;

#ifdef SNMP
//for snmp , kathy
typedef struct _DefaultKeyIdxValue
{
	UCHAR	KeyIdx;
	UCHAR	Value[16];
} DefaultKeyIdxValue, *PDefaultKeyIdxValue;
#endif

// structure for query/set hardware register - MAC, BBP, RF register
typedef struct _RT_802_11_HARDWARE_REGISTER {
    ULONG   HardwareType;       // 0:MAC, 1:BBP, 2:RF register
    ULONG   Offset;             // Q/S register offset addr
    ULONG   Data;               // R/W data buffer
} RT_802_11_HARDWARE_REGISTER, *PRT_802_11_HARDWARE_REGISTER;

typedef struct _RT_LLTD_ASSOICATION_ENTRY {
    UCHAR           Addr[ETH_LENGTH_OF_ADDRESS];
    unsigned short  MOR;        // maximum operational rate
    UCHAR           phyMode;
} RT_LLTD_ASSOICATION_ENTRY, *PRT_LLTD_ASSOICATION_ENTRY;

typedef struct _RT_LLTD_ASSOICATION_TABLE {
    unsigned int                Num;
    RT_LLTD_ASSOICATION_ENTRY   Entry[MAX_LEN_OF_MAC_TABLE];
} RT_LLTD_ASSOICATION_TABLE, *PRT_LLTD_ASSOICATION_TABLE;
#endif

