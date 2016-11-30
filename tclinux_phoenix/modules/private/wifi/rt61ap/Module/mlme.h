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
	mlme.h

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	John Chang	2003-08-28		Created
	John Chang  2004-09-06      modified for RT2600
	
*/
#ifndef __MLME_H__
#define __MLME_H__

#include "oid.h"

// maximum supported capability information - 
// ESS, IBSS, Privacy, Short Preamble, Short Slot
#define SUPPORTED_CAPABILITY_INFO   0x0433

#define END_OF_ARGS                 -1
#define LFSR_MASK                   0x80000057
#define MLME_TASK_EXEC_INTV         (1*HZ)       // 1 sec
#ifdef GEM_LEDMODE
#define LED_TASK_EXEC_INTV         (HZ/10)       // 0.1 sec
#endif

// The definition of Radar detection duration region
#define JAP		0
#define FCC 	1
#define CE		2

#ifdef APCLI_SUPPORT
#define AUTH_TIMEOUT                300         // unit: msec
#define ASSOC_TIMEOUT               300         // unit: msec
#define JOIN_TIMEOUT                2000        // unit: msec
#define PROBE_TIMEOUT               1000        // unit: msec
#define BEACON_LOST_TIME            4000       // 2048 msec = 2 sec
#endif

#define MIN_CHANNEL_TIME            110        // unit: msec, for dual band scan
#define MAX_CHANNEL_TIME            140       // unit: msec, for single band scan
#define CW_MIN_IN_BITS              4         // actual CwMin = 2^CW_MIN_IN_BITS - 1
#define CW_MAX_IN_BITS              6/*10*/        // actual CwMax = 2^CW_MAX_IN_BITS - 1

// Note: RSSI_TO_DBM_OFFSET has been changed to variable for new RF (2004-0720).
// SHould not refer to this constant anymore
//#define RSSI_TO_DBM_OFFSET          120 // for RT2530 RSSI-115 = dBm
#define RSSI_FOR_MID_TX_POWER       -55  // -55 db is considered mid-distance
#define RSSI_FOR_LOW_TX_POWER       -45  // -45 db is considered very short distance and 
                                        // eligible to use a lower TX power
#define RSSI_FOR_LOWEST_TX_POWER    -30
//#define MID_TX_POWER_DELTA          0   // 0 db from full TX power upon mid-distance to AP
#define LOW_TX_POWER_DELTA          6    // -3 db from full TX power upon very short distance. 1 grade is 0.5 db
#define LOWEST_TX_POWER_DELTA       16   // -8 db from full TX power upon shortest distance. 1 grade is 0.5 db

#define RSSI_TRIGGERED_UPON_BELOW_THRESHOLD     0
#define RSSI_TRIGGERED_UPON_EXCCEED_THRESHOLD   1
#define RSSI_THRESHOLD_FOR_ROAMING              25
#define RSSI_DELTA                              5

// weighting factor to calculate Channel quality, total should be 100%
#define RSSI_WEIGHTING                   50
#define TX_WEIGHTING                     30
#define RX_WEIGHTING                     20

#define MAX_LEN_OF_BSS_TABLE             64
#define BSS_NOT_FOUND                    0xFFFFFFFF

#define MAX_LEN_OF_MLME_QUEUE            20

// assoc state-machine states
#define ASSOC_IDLE                       0
#define MAX_ASSOC_STATE                  1

// authentication state machine
#define AUTH_REQ_IDLE                    0
#define MAX_AUTH_STATE                   1

// authentication response state machine
#define AUTH_RSP_IDLE                    0
#define MAX_AUTH_RSP_STATE               1

// SYNC state machine
#define SYNC_IDLE                        0  // merge NO_BSS,IBSS_IDLE,IBSS_ACTIVE and BSS in to 1 state
#define SCAN_LISTEN                      1
#define MAX_SYNC_STATE                   2

#define SCAN_PASSIVE                     0
#define SCAN_ACTIVE                      1

//WPA State machine
#define WPA_PTK                          0
#define MAX_WPA_PTK_STATE                1

//DLS State machine
#define DLS_IDLE                         0
#define MAX_DLS_STATE                    1

#define ASSOC_FUNC_SIZE             (MAX_ASSOC_STATE * MAX_ASSOC_MSG)
#define AUTH_FUNC_SIZE              (MAX_AUTH_STATE * MAX_AUTH_MSG)
#define AUTH_RSP_FUNC_SIZE          (MAX_AUTH_RSP_STATE * MAX_AUTH_RSP_MSG)
#define SYNC_FUNC_SIZE              (MAX_SYNC_STATE * MAX_SYNC_MSG)
#define WPA_FUNC_SIZE               (MAX_WPA_PTK_STATE * MAX_WPA_MSG)
#define DLS_FUNC_SIZE               (MAX_DLS_STATE * MAX_DLS_MSG)

//#define BSS_TABLE_EMPTY(x)             ((x).BssNr == 0)
#define MAC_ADDR_IS_GROUP(Addr)       (((Addr[0]) & 0x01) != 0)
#define MAC_ADDR_HASH(Addr)            (Addr[0] ^ Addr[1] ^ Addr[2] ^ Addr[3] ^ Addr[4] ^ Addr[5])
#define MAC_ADDR_HASH_INDEX(Addr)      (MAC_ADDR_HASH(Addr) % HASH_TABLE_SIZE)

// LED Control
// assoiation ON. one LED ON. another blinking when TX, OFF when idle
// no association, both LED off
#define ASIC_LED_ACT_ON(pAdapter)        RTMP_IO_WRITE32(pAdapter, MAC_CSR14, 0x00031e46)
#define ASIC_LED_ACT_OFF(pAdapter)       RTMP_IO_WRITE32(pAdapter, MAC_CSR14, 0x00001e46)
#ifdef GEM_LEDMODE
#define ASIC_LED_SET_ON(pAdapter)        RTMP_IO_WRITE32(pAdapter, MAC_CSR14, 0x00041e46)
#define ASIC_LED_SET_OFF(pAdapter)       RTMP_IO_WRITE32(pAdapter, MAC_CSR14, 0x00001e46)
#define ASIC_LED_SET_BLINK(pAdapter)     RTMP_IO_WRITE32(pAdapter, MAC_CSR14, 0x00051e46)
#endif

// bit definition of the 2-byte pBEACON->Capability field
#define CAP_IS_ESS_ON(x)                 (((x) & 0x0001) != 0)
#define CAP_IS_IBSS_ON(x)                (((x) & 0x0002) != 0)
#define CAP_IS_CF_POLLABLE_ON(x)         (((x) & 0x0004) != 0)
#define CAP_IS_CF_POLL_REQ_ON(x)         (((x) & 0x0008) != 0)
#define CAP_IS_PRIVACY_ON(x)             (((x) & 0x0010) != 0)
#define CAP_IS_SHORT_PREAMBLE_ON(x)      (((x) & 0x0020) != 0)
#define CAP_IS_PBCC_ON(x)                (((x) & 0x0040) != 0)
#define CAP_IS_AGILITY_ON(x)             (((x) & 0x0080) != 0)
#define CAP_IS_SPECTRUM_MGMT(x)          (((x) & 0x0100) != 0)  // 802.11e d9
#define CAP_IS_QOS(x)                    (((x) & 0x0200) != 0)  // 802.11e d9
#define CAP_IS_SHORT_SLOT(x)             (((x) & 0x0400) != 0)
#define CAP_IS_APSD(x)                   (((x) & 0x0800) != 0)  // 802.11e d9
#define CAP_IS_IMMED_BA(x)               (((x) & 0x1000) != 0)  // 802.11e d9
#define CAP_IS_DSSS_OFDM(x)              (((x) & 0x2000) != 0)
#define CAP_IS_DELAY_BA(x)               (((x) & 0x4000) != 0)  // 802.11e d9

#define CAP_GENERATE(priv,s_pre,s_slot,spectrum)  (0x0001 | ((priv) ? 0x0010 : 0x0000) | ((s_pre) ? 0x0020 : 0x0000) | ((s_slot) ? 0x0400 : 0x0000) | ((spectrum) ? 0x0100 : 0x0000))

#define STA_QOS_CAPABILITY               0 // 1-byte. see 802.11e d9.0 for bit definition

#define ERP_IS_NON_ERP_PRESENT(x)        (((x) & 0x01) != 0)    // 802.11g
#define ERP_IS_USE_PROTECTION(x)         (((x) & 0x02) != 0)    // 802.11g
#define ERP_IS_USE_BARKER_PREAMBLE(x)    (((x) & 0x04) != 0)    // 802.11g

#define DRS_TX_QUALITY_WORST_BOUND       3
#define DRS_PENALTY                      8

//
// 802.11 frame formats
//
// Value domain of pMacEntry->Sst
typedef enum _Sst {
    SST_NOT_AUTH,   // 0: equivalent to IEEE 802.11/1999 state 1
    SST_AUTH,       // 1: equivalent to IEEE 802.11/1999 state 2
    SST_ASSOC       // 2: equivalent to IEEE 802.11/1999 state 3
} SST;

// for-wpa value domain of pMacEntry->WpaState  802.1i D3   p.114
typedef enum _GTKState {
	REKEY_NEGOTIATING,
	REKEY_ESTABLISHED,
	KEYERROR,
} GTK_STATE;

// 2-byte QOS CONTROL field
typedef struct PACKED {
#ifdef BIG_ENDIAN
    USHORT      Txop_QueueSize:8;
    USHORT      Rsv:1;
    USHORT      AckPolicy:2;
    USHORT      EOSP:1;
    USHORT      TID:4;
#else
    USHORT      TID:4;
    USHORT      EOSP:1;
    USHORT      AckPolicy:2;
    USHORT      Rsv:1;
    USHORT      Txop_QueueSize:8;
#endif
} QOS_CONTROL, *PQOS_CONTROL;

// 2-byte Frame control field
typedef	struct	PACKED {
#ifdef BIG_ENDIAN
    USHORT		Order:1;
    USHORT		Wep:1;
    USHORT		MoreData:1;
    USHORT		PwrMgmt:1;
    USHORT		Retry:1;
    USHORT		MoreFrag:1;
    USHORT		FrDs:1;
    USHORT		ToDs:1;
    USHORT		SubType:4;
    USHORT		Type:2;
    USHORT		Ver:2;
#else
	USHORT		Ver:2;				// Protocol version
	USHORT		Type:2;				// MSDU type
	USHORT		SubType:4;			// MSDU subtype
	USHORT		ToDs:1;				// To DS indication
	USHORT		FrDs:1;				// From DS indication
	USHORT		MoreFrag:1;			// More fragment bit
	USHORT		Retry:1;			// Retry status bit
	USHORT		PwrMgmt:1;			// Power management bit
	USHORT		MoreData:1;			// More data bit
	USHORT		Wep:1;				// Wep data
	USHORT		Order:1;			// Strict order expected
#endif
}	FRAME_CONTROL, *PFRAME_CONTROL;

typedef	struct	PACKED _HEADER_802_11	{
    FRAME_CONTROL   FC;
    USHORT          Duration;
    UCHAR           Addr1[MAC_ADDR_LEN];
    UCHAR           Addr2[MAC_ADDR_LEN];
	UCHAR			Addr3[MAC_ADDR_LEN];
#ifdef BIG_ENDIAN
    USHORT    		Sequence:12;
    USHORT    		Frag:4;
#else
	USHORT			Frag:4;
	USHORT			Sequence:12;
#endif
}	HEADER_802_11, *PHEADER_802_11;

typedef struct PACKED _FRAME_802_11 {
    HEADER_802_11   Hdr;
    CHAR            Octet[1];
}   FRAME_802_11, *PFRAME_802_11;

typedef	struct	PACKED _RTS_FRAME	{
    FRAME_CONTROL   FC;
    USHORT          Duration;
    UCHAR           Addr1[MAC_ADDR_LEN];
    UCHAR           Addr2[MAC_ADDR_LEN];
}	RTS_FRAME, *PRTS_FRAME;


//
// Contention-free parameter (without ID and Length)
//
typedef struct PACKED {
    BOOLEAN     bValid;         // 1: variable contains valid value
    UCHAR       CfpCount;
    UCHAR       CfpPeriod;
    USHORT      CfpMaxDuration;
    USHORT      CfpDurRemaining;
} CF_PARM, *PCF_PARM;

typedef	struct PACKED _CIPHER_SUITE	{
	NDIS_802_11_ENCRYPTION_STATUS	PairCipher;		// Unicast cipher 1, this one has more secured cipher suite
	NDIS_802_11_ENCRYPTION_STATUS	PairCipherAux;	// Unicast cipher 2 if AP announce two unicast cipher suite
	NDIS_802_11_ENCRYPTION_STATUS	GroupCipher;	// Group cipher
	USHORT				RsnCapability;	// RSN capability from beacon
	BOOLEAN				bMixMode;		// Indicate Pair & Group cipher might be different
}	CIPHER_SUITE, *PCIPHER_SUITE;

// EDCA configuration from AP's BEACON/ProbeRsp
typedef struct PACKED {
    BOOLEAN     bValid;         // 1: variable contains valid value
    BOOLEAN     bQAck;
    BOOLEAN     bQueueRequest;
    BOOLEAN     bTxopRequest;
//  BOOLEAN     bMoreDataAck;
    UCHAR       EdcaUpdateCount;
    UCHAR       Aifsn[4];       // 0:AC_BK, 1:AC_BE, 2:AC_VI, 3:AC_VO
    UCHAR       Cwmin[4];
    UCHAR       Cwmax[4];
    USHORT      Txop[4];      // in unit of 32-us
    BOOLEAN     bACM[4];      // 1: Admission Control of AC_BK is mandattory
} EDCA_PARM, *PEDCA_PARM;

// QBSS LOAD information from QAP's BEACON/ProbeRsp
typedef struct PACKED {
    BOOLEAN     bValid;                     // 1: variable contains valid value
    USHORT      StaNum;
    UCHAR       ChannelUtilization;
    USHORT      RemainingAdmissionControl;  // in unit of 32-us
} QBSS_LOAD_PARM, *PQBSS_LOAD_PARM;

// QBSS Info field in QSTA's assoc req
typedef struct PACKED {
#ifndef BIG_ENDIAN
    UCHAR		UAPSD_AC_VO:1;
	UCHAR		UAPSD_AC_VI:1;
	UCHAR		UAPSD_AC_BK:1;
	UCHAR		UAPSD_AC_BE:1;
	UCHAR		Rsv1:1;
	UCHAR		MaxSPLength:2;
	UCHAR		Rsv2:1;
#else		
	UCHAR		Rsv2:1;
	UCHAR		MaxSPLength:2;		
	UCHAR		Rsv1:1;
	UCHAR		UAPSD_AC_BE:1;
	UCHAR		UAPSD_AC_BK:1;
	UCHAR		UAPSD_AC_VI:1;
	UCHAR		UAPSD_AC_VO:1;
#endif
} QBSS_STA_INFO_PARM, *PQBSS_STA_INFO_PARM;

// QBSS Info field in QAP's Beacon/ProbeRsp
typedef struct {
    UCHAR		ParamSetCount:4;
	UCHAR		Rsv:3;
	UCHAR		UAPSD:1;
} QBSS_AP_INFO_PARM, *PQBSS_AP_INFO_PARM;

// QOS Capability reported in QAP's BEACON/ProbeRsp
// QOS Capability sent out in QSTA's AssociateReq/ReAssociateReq
typedef struct PACKED {
    BOOLEAN     bValid;                     // 1: variable contains valid value
    BOOLEAN     bQAck;
    BOOLEAN     bQueueRequest;
    BOOLEAN     bTxopRequest;
//  BOOLEAN     bMoreDataAck;
    UCHAR       EdcaUpdateCount;
} QOS_CAPABILITY_PARM, *PQOS_CAPABILITY_PARM;

typedef struct _MLME_QUEUE_ELEM {
	UCHAR             Msg[MAX_LEN_OF_MLME_BUFFER];
    ULONG             Machine;
    ULONG             MsgType;
    ULONG             MsgLen;
    LARGE_INTEGER     TimeStamp;
    UCHAR             Rssi;
    UCHAR             Channel;
    BOOLEAN           Occupied;
#ifdef APCLI_SUPPORT
	USHORT            ifIndex;
#endif
} MLME_QUEUE_ELEM, *PMLME_QUEUE_ELEM;

typedef struct _MLME_QUEUE {
    ULONG             Num;
    ULONG             Head;
    ULONG             Tail;
    spinlock_t        Lock;
    MLME_QUEUE_ELEM   Entry[MAX_LEN_OF_MLME_QUEUE];
} MLME_QUEUE, *PMLME_QUEUE;

typedef VOID (*STATE_MACHINE_FUNC)(VOID *Adaptor, MLME_QUEUE_ELEM *Elem);

typedef struct _STATE_MACHINE {
    ULONG                           Base;
    ULONG                           NrState;
    ULONG                           NrMsg;
    ULONG                           CurrState;
    STATE_MACHINE_FUNC             *TransFunc;
} STATE_MACHINE, *PSTATE_MACHINE;

#ifdef APCLI_SUPPORT
// MLME AUX data structure that hold temporarliy settings during a connection attempt.
// Once this attemp succeeds, all settings will be copy to pAd->ActiveCfg.
// A connection attempt (user set OID, roaming, CCX fast roaming,..) consists of
// several steps (JOIN, AUTH, ASSOC or REASSOC) and may fail at any step. We purposely
// separate this under-trial settings away from pAd->ActiveCfg so that once
// this new attempt failed, driver can auto-recover back to the active settings.
typedef struct _MLME_AUX {
    UCHAR               BssType;
    UCHAR               Ssid[MAX_LEN_OF_SSID];
    UCHAR               SsidLen;
    UCHAR               Bssid[MAC_ADDR_LEN];
    UCHAR               AutoReconnectSsid[MAX_LEN_OF_SSID];
    UCHAR               AutoReconnectSsidLen;
    USHORT              Alg;
    UCHAR               ScanType;
    UCHAR               Channel;
    USHORT              Aid;
    USHORT              CapabilityInfo;
    USHORT              BeaconPeriod;
    USHORT              CfpMaxDuration;
    USHORT              CfpPeriod;
    USHORT              AtimWin;
    
    // Copy supported rate from desired AP's beacon. We are trying to match
    // AP's supported and extended rate settings.
    UCHAR               SupRate[MAX_LEN_OF_SUPPORTED_RATES];
    UCHAR               ExtRate[MAX_LEN_OF_SUPPORTED_RATES];
    UCHAR               SupRateLen;
    UCHAR               ExtRateLen;

    // new for QOS
    QOS_CAPABILITY_PARM APQosCapability;    // QOS capability of the current associated AP
    EDCA_PARM           APEdcaParm;         // EDCA parameters of the current associated AP
    QBSS_LOAD_PARM      APQbssLoad;         // QBSS load of the current associated AP

    // new to keep Ralink specific feature
    ULONG               APRalinkIe;
    
    ULONG               BssIdx;
    ULONG               RoamIdx;

    BOOLEAN             CurrReqIsFromNdis;  // TRUE - then we should call NdisMSetInformationComplete()
                                            // FALSE - req is from driver itself. 
                                            // no NdisMSetInformationComplete() is required

    UCHAR               VarIELen;           // Length of next VIE include EID & Length
    UCHAR               VarIEs[MAX_VIE_LEN];

    // Record the rssi value when receive Probe Rsp.
    UCHAR               Rssi;

    struct timer_list   ProbeTimer;
    struct timer_list   ApCliAuthTimer;
    struct timer_list   ApCliAssocTimer;
} MLME_AUX, *PMLME_AUX;
#endif /* APCLI_SUPPORT */

typedef struct _MLME_DISASSOC_REQ_STRUCT{
    UCHAR     Addr[MAC_ADDR_LEN];
    USHORT    Reason;
} MLME_DISASSOC_REQ_STRUCT, *PMLME_DISASSOC_REQ_STRUCT;

typedef struct _MLME_DEAUTH_REQ_STRUCT {
    UCHAR        Addr[MAC_ADDR_LEN];
    USHORT       Reason;
} MLME_DEAUTH_REQ_STRUCT, *PMLME_DEAUTH_REQ_STRUCT;

typedef struct _MLME_SCAN_REQ_STRUCT {
    UCHAR      Bssid[MAC_ADDR_LEN];
    UCHAR      BssType;
    UCHAR      ScanType;
    UCHAR      SsidLen;
    CHAR       Ssid[MAX_LEN_OF_SSID];
} MLME_SCAN_REQ_STRUCT, *PMLME_SCAN_REQ_STRUCT;

#ifdef APCLI_SUPPORT
// assoc struct is equal to reassoc
typedef struct _MLME_ASSOC_REQ_STRUCT {
    UCHAR     Addr[MAC_ADDR_LEN];
    USHORT    CapabilityInfo;
    USHORT    ListenIntv;
    ULONG     Timeout;
} MLME_ASSOC_REQ_STRUCT, *PMLME_ASSOC_REQ_STRUCT, MLME_REASSOC_REQ_STRUCT, *PMLME_REASSOC_REQ_STRUCT;

typedef struct _MLME_AUTH_REQ_STRUCT {
    UCHAR     Addr[MAC_ADDR_LEN];
    USHORT    Alg;
    ULONG     Timeout;
} MLME_AUTH_REQ_STRUCT, *PMLME_AUTH_REQ_STRUCT;

typedef struct _MLME_JOIN_REQ_STRUCT {
    UCHAR     Bssid[MAC_ADDR_LEN];
    UCHAR     SsidLen;
    CHAR      Ssid[MAX_LEN_OF_SSID];
} MLME_JOIN_REQ_STRUCT;

typedef struct _APCLI_CTRL_JOIN_REQ_STRUCT {
    USHORT    Status;
} STA_CTRL_MSG_STRUCT, *PSTA_CTRL_MSG_STRUCT;

#endif /* APCLI_SUPPORT */

typedef struct {
    UCHAR   Eid;
    UCHAR   Len;
    UCHAR   Octet[1];
} EID_STRUCT,*PEID_STRUCT;

// New for WPA cipher suite 
typedef	struct	_RSN_EID_STRUCT	{
	UCHAR	Eid;
	UCHAR	Length;
	UCHAR	Oui[4];
	USHORT	Version;
	UCHAR	Multicast[4];
	USHORT	Count;
	struct	{
		UCHAR	Oui[4];
	}	Unicast[1];
}	RSN_EID_STRUCT, *PRSN_EID_STRUCT;

typedef struct _MLME_DLS_REQ_STRUCT {
    UCHAR		Addr[MAC_ADDR_LEN];
    USHORT		Reason;
} MLME_DLS_REQ_STRUCT, *PMLME_DLS_REQ_STRUCT;

// value domain of pMacEntry->AuthState
typedef enum _AuthState {
    AS_NOT_AUTH,
    AS_AUTH_OPEN,       // STA has been authenticated using OPEN SYSTEM
    AS_AUTH_KEY,        // STA has been authenticated using SHARED KEY
    AS_AUTHENTICATING   // STA is waiting for AUTH seq#3 using SHARED KEY
} AUTH_STATE;

//for-wpa value domain of pMacEntry->WpaState  802.1i D3   p.114
typedef enum _WpaState {
	AS_NOTUSE,		//0
	AS_DISCONNECT,		//   1
	AS_DISCONNECTED,		// 2
	AS_INITIALIZE,	// 3
	AS_AUTHENTICATION,
	AS_AUTHENTICATION2,  // 5
	AS_INITPMK,	// 6
	AS_INITPSK,		// 7
	AS_PTKSTART,	// 8
	AS_PTKINIT_NEGOTIATING,	// 9
	AS_PTKINITDONE,	// 10
	AS_UPDATEKEYS,
	AS_INTEGRITY_FAILURE,
	AS_KEYUPDATE,
} WPA_STATE;

// For supplicant state machine states. 802.11i Draft 4.1, p. 97
// We simplified it
typedef enum    _SsWpaState
{
    SS_NOTUSE,              // 0
    SS_START,               // 1
    SS_WAIT_MSG_3,          // 2
    SS_WAIT_GROUP,          // 3
    SS_FINISH,              // 4
    SS_KEYUPDATE,           // 5
}   SS_WPA_STATE;

typedef struct {
    UCHAR  Bssid[MAC_ADDR_LEN];
    UCHAR   Channel;
    UCHAR   BssType;
    USHORT  AtimWin;
    USHORT  BeaconPeriod;

    UCHAR   SupRate[MAX_LEN_OF_SUPPORTED_RATES];
    UCHAR   SupRateLen;
    UCHAR   ExtRate[MAX_LEN_OF_SUPPORTED_RATES];
    UCHAR   ExtRateLen;
    UCHAR   Rssi;
    UCHAR   Privacy;            // Indicate security function ON/OFF. Don't mess up with auth mode.
    UCHAR   Hidden;

    USHORT  DtimPeriod;
    USHORT  CapabilityInfo;

    USHORT  CfpCount;
    USHORT  CfpPeriod;
    USHORT  CfpMaxDuration;
    USHORT  CfpDurRemaining;
    UCHAR   SsidLen;
    CHAR    Ssid[MAX_LEN_OF_SSID];
    
    UMAXLONG  LastBeaconRxTime; // OS's timestamp

    // New for WPA2
    CIPHER_SUITE         WPA;
    CIPHER_SUITE         WPA2;

    // New for microsoft WPA support
    NDIS_802_11_FIXED_IEs   FixIEs;
    NDIS_802_11_AUTHENTICATION_MODE	AuthModeAux;	// Addition mode for WPA2 / WPA capable AP
    NDIS_802_11_AUTHENTICATION_MODE	AuthMode;
	NDIS_802_11_WEP_STATUS	WepStatus;				// Unicast Encryption Algorithm extract from VAR_IE
    UCHAR                   VarIELen;               // Length of next VIE include EID & Length
    UCHAR                   VarIEs[MAX_VIE_LEN];

	// CCX Ckip information
    UCHAR   CkipFlag;

	// CCX 2 TSF
	UCHAR	PTSF[4];		// Parent TSF
	UCHAR	TTSF[8];		// Target TSF

    // 802.11e d9, and WMM
	EDCA_PARM           EdcaParm;
	QOS_CAPABILITY_PARM QosCapability;
	QBSS_LOAD_PARM      QbssLoad;
} BSS_ENTRY, *PBSS_ENTRY;

typedef struct {
    UCHAR           BssNr;
    BSS_ENTRY       *BssEntry; //[MAX_LEN_OF_BSS_TABLE];
} BSS_TABLE, *PBSS_TABLE;

#define DEFAULT_DTIM_PERIOD     1

#define MAC_TABLE_AGEOUT_TIME            300		// unit: sec
#define MAC_TABLE_ASSOC_TIMEOUT          5			// unit: sec

#ifdef CARRIER_DETECTION_SUPPORT
typedef enum CD_STATE_n
{
	CD_NORMAL,
	CD_SILENCE,
	CD_MAX_STATE
} CD_STATE;

typedef struct CARRIER_DETECTION_s
{
	BOOLEAN					Enable;
	CD_STATE				CD_State;
} CARRIER_DETECTION, *PCARRIER_DETECTION;

#endif // CARRIER_DETECTION_SUPPORT //

#endif	// MLME_H__
