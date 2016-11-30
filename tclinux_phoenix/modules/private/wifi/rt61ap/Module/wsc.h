/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2006, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attemp
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	aironet.h

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Name		Date			Modification logs
	Paul Lin	06-08-08		Initial
*/

#ifndef	__WSC_H__
#define	__WSC_H__

//Messages for the WSC state machine,
#define	WSC_MACHINE_BASE		34
#define	WSC_EAPOL_PACKET_MSG	34
#define	WSC_EAPOL_START_MSG		35
#define	WSC_EAPOL_UPNP_MSG		36

#define	MAX_WSC_MSG			    3

#define WSC_AP_STATE_MACHINE 12
//#define WSC_UNCONF_AP_STATE_MACHINE 12
//#define WSC_AP_PROXY_STATE_MACHINE  13
//#define WSC_AP_REG_STATE_MACHINE    14
#define WSC_UPNP_STATE_MACHINE		15

// WSC OUI SMI
#define WSC_OUI				0x0050f204
#define	WSC_SMI				0x00372A
#define	WSC_VENDOR_TYPE		0x00000001

// EAP code
#define	EAP_CODE_REQ		0x01
#define	EAP_CODE_RSP		0x02
#define	EAP_CODE_FAIL		0x04
#define EAP_TYPE_ID			0x01
#define EAP_TYPE_NOTIFY		0x02
#define	EAP_TYPE_WSC		0xfe

// WSC Opcode
#define	WSC_OPCODE_START		0x01
#define	WSC_OPCODE_ACK			0x02
#define	WSC_OPCODE_NACK			0x03
#define	WSC_OPCODE_MSG			0x04
#define	WSC_OPCODE_DONE			0x05
#define	WSC_OPCODE_FRAG_ACK		0x06

#define WSC_OPCODE_UPNP_MASK	0x10
#define WSC_OPCODE_UPNP_DATA	0x11
#define WSC_OPCODE_UPNP_MGMT	0x12
#define WSC_OPCODE_UPNP_CTRL	0x13

#define WSC_UPNP_MGMT_SUB_PROBE_REQ		0x01
#define WSC_UPNP_MGMT_SUB_CONFIG_REQ	0x02
#define WSC_UPNP_MGMT_SUB_REG_SELECT	0x03


#define WSC_UPNP_DATA_SUB_NORMAL		0x00
#define WSC_UPNP_DATA_SUB_TO_ALL		0x01
#define WSC_UPNP_DATA_SUB_TO_ONE		0x02
#define WSC_UPNP_DATA_SUB_ACK			0x03
#define WSC_UPNP_DATA_SUB_M1			0x04
#define WSC_UPNP_DATA_SUB_M2			0x05
#define WSC_UPNP_DATA_SUB_M2D			0x06
#define WSC_UPNP_DATA_SUB_M3			0x07
#define WSC_UPNP_DATA_SUB_M4			0x08
#define WSC_UPNP_DATA_SUB_M5			0x09
#define WSC_UPNP_DATA_SUB_M6			0x0A
#define WSC_UPNP_DATA_SUB_M7			0x0B
#define WSC_UPNP_DATA_SUB_M8			0x0C
#define WSC_UPNP_DATA_SUB_WSC_ACK		0x0D
#define WSC_UPNP_DATA_SUB_WSC_NACK		0x0E
#define WSC_UPNP_DATA_SUB_WSC_DONE		0x0F
#define WSC_UPNP_DATA_SUB_WSC_UNKNOWN	0xff


// Wsc EAP Messges type
#define	WSC_MSG_EAP_RSP_ID			0x21	
#define	WSC_MSG_EAP_REG_RSP_ID      0x22
#define	WSC_MSG_EAP_ENR_RSP_ID      0x23
#define	WSC_MSG_EAP_UPNP_RSP_ID     0x24
//  For AP Client support WPS Modification
#define	WSC_MSG_EAP_REQ_ID			0x25
#define	WSC_MSG_EAP_REQ_START		0x26
#define	WSC_MSG_EAP_FAIL			0x27
// --
#define	WSC_MSG_PROB_RSP			0x01
#define WSC_MSG_EAPOL_START         0x02
#define	WSC_MSG_M1					0x04
#define	WSC_MSG_M2					0x05
#define	WSC_MSG_M2D					0x06
#define	WSC_MSG_M3					0x07
#define	WSC_MSG_M4					0x08
#define	WSC_MSG_M5					0x09
#define	WSC_MSG_M6					0x0A
#define	WSC_MSG_M7					0x0B
#define	WSC_MSG_M8					0x0C
#define	WSC_MSG_WSC_ACK				0x0D
#define	WSC_MSG_WSC_NACK			0x0E
#define	WSC_MSG_WSC_DONE			0x0F
#define	WSC_MSG_UNKNOWN				0xff

// WSC connection mode
#define	WSC_PIN_MODE		        1
#define	WSC_PBC_MODE		        2

// Value of WSC_IE_DEV_PASS_ID 0x1012
#define	DEV_PASS_ID_PIN				0x0000
#define	DEV_PASS_ID_USER			0x0001
#define	DEV_PASS_ID_MACHINE			0x0002
#define	DEV_PASS_ID_REKEY			0x0003
#define	DEV_PASS_ID_PBC				0x0004
#define	DEV_PASS_ID_REG				0x0005

// Common definition
#define	WSC_VERSION					0x10
#define WSC_CONFIG_METHODS			0x008A

// Wsc status code
#define	STATUS_WSC_NOTUSED						0
#define	STATUS_WSC_IDLE							1
#define STATUS_WSC_FAIL        			        2		// WSC Process Fail
#define	STATUS_WSC_LINK_UP						3		// Start WSC Process
#define	STATUS_WSC_EAPOL_START_RECEIVED			4		// Received EAPOL-Start
#define	STATUS_WSC_EAP_REQ_ID_SENT				5		// Sending EAP-Req(ID)
#define	STATUS_WSC_EAP_RSP_ID_RECEIVED			6		// Receive EAP-Rsp(ID)
#define	STATUS_WSC_EAP_RSP_WRONG_SMI			7		// Receive EAP-Req with wrong WSC SMI Vendor Id
#define	STATUS_WSC_EAP_RSP_WRONG_VENDOR_TYPE	8		// Receive EAPReq with wrong WSC Vendor Type
#define	STATUS_WSC_EAP_REQ_WSC_START			9		// Sending EAP-Req(WSC_START)
#define	STATUS_WSC_EAP_M1_SENT					10		// Send M1
#define	STATUS_WSC_EAP_M1_RECEIVED				11		// Received M1
#define	STATUS_WSC_EAP_M2_SENT					12		// Send M2
#define	STATUS_WSC_EAP_M2_RECEIVED				13		// Received M2
#define	STATUS_WSC_EAP_M2D_RECEIVED				14		// Received M2D
#define	STATUS_WSC_EAP_M3_SENT					15		// Send M3
#define	STATUS_WSC_EAP_M3_RECEIVED				16		// Received M3
#define	STATUS_WSC_EAP_M4_SENT					17		// Send M4
#define	STATUS_WSC_EAP_M4_RECEIVED				18		// Received M4
#define	STATUS_WSC_EAP_M5_SENT					19		// Send M5
#define	STATUS_WSC_EAP_M5_RECEIVED				20		// Received M5
#define	STATUS_WSC_EAP_M6_SENT					21		// Send M6
#define	STATUS_WSC_EAP_M6_RECEIVED				22		// Received M6
#define	STATUS_WSC_EAP_M7_SENT					23		// Send M7
#define	STATUS_WSC_EAP_M7_RECEIVED				24		// Received M7
#define	STATUS_WSC_EAP_M8_SENT					25		// Send M8
#define	STATUS_WSC_EAP_M8_RECEIVED				26		// Received M8
#define	STATUS_WSC_EAP_RAP_RSP_ACK				27		// Processing EAP Response (ACK)
#define	STATUS_WSC_EAP_RAP_REQ_DONE_SENT		28		// Processing EAP Request (Done)
#define	STATUS_WSC_EAP_RAP_RSP_DONE_SENT		29		// Processing EAP Response (Done)
#define STATUS_WSC_EAP_FAIL_SENT                30      // Sending EAP-Fail
#define STATUS_WSC_ERROR_HASH_FAIL              31      // WSC_ERROR_HASH_FAIL
#define STATUS_WSC_ERROR_HMAC_FAIL              32      // WSC_ERROR_HMAC_FAIL
#define STATUS_WSC_ERROR_DEV_PWD_AUTH_FAIL      33      // WSC_ERROR_DEV_PWD_AUTH_FAIL
#define STATUS_WSC_CONFIGURED					34
// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
#define STATUS_WSC_SCAN_AP						35		// Scanning AP
#define STATUS_WSC_EAPOL_START_SENT				36
#define STATUS_WSC_WAIT_PIN_CODE                37
#endif // APCLI_SUPPORT //
// --

#define WSC_DISABLE		    0x0
#define WSC_AP_Enrollee     0x1
#define WSC_AP_PROXY        0x2
#define WSC_AP_REGISTRAR    0x4
#define WSC_AP_Enrollee_PROXY           (WSC_AP_Enrollee | WSC_AP_PROXY)
#define WSC_AP_Enrollee_REGISTRAR       (WSC_AP_Enrollee | WSC_AP_REGISTRAR)
#define WSC_AP_PROXY_REGISTRAR          (WSC_AP_PROXY | WSC_AP_REGISTRAR)
#define WSC_AP_Enrollee_PROXY_REGISTRAR (WSC_AP_Enrollee | WSC_AP_PROXY | WSC_AP_REGISTRAR)

// Device request/response type
#define WSC_MSGTYPE_ENROLLEE_INFO_ONLY		0x00
#define WSC_MSGTYPE_ENROLLEE_OPEN_8021X		0x01
#define WSC_MSGTYPE_REGISTRAR				0x02
#define WSC_MSGTYPE_AP_WLAN_MGR				0x03

// RF Band
#define WSC_RFBAND_24GHZ    0x01
#define WSC_RFBAND_50GHZ    0x02

// Simple Config state
#define WSC_SCSTATE_UNCONFIGURED	0x01
#define WSC_SCSTATE_CONFIGURED		0x02

// Common definition
#define	WSC_MANUFACTURE		"Ralink Technology, Corp."
#define	WSC_MODEL_NAME		"Ralink Wireless Access Point"
#define	WSC_MODEL_NUMBER	"RT2561"
#define	WSC_MODEL_SERIAL	"12345678"
#define	WSC_DEVICE_NAME		"RalinkAPS"

// Time-Out
#define WSC_EAPOL_START_TIME_OUT    (2 * HZ)
#define WSC_EAP_ID_TIME_OUT         (5 * HZ)
#define WSC_EAP_MSG_TIME_OUT        (5 * HZ)
#define WSC_EAP_MSG_ACK_TIME_OUT    (1 * HZ)
// For AP Client support WPS Modification
#define WSC_EAP_EAP_FAIL_TIME_OUT    (1 * HZ)
// --
#define WSC_TWO_MINS_TIME_OUT       (120 * HZ)
#define WSC_UPNP_M2D_TIME_OUT		(10 * HZ)
#define WSC_UPNP_MSG_TIME_OUT		(5 * HZ)

#define WSC_INIT_ENTRY_APIDX        255
#define WSC_MAX_DATA_LEN            1024

#define WSC_ENTRY_GET_EAPOL_START   0x1
#define WSC_ENTRY_GET_EAP_RSP_ID    0x2

// Pack struct to align at byte
//#pragma pack(1)

// General used field

// <<WCN vista logo>> ++, add by johnli
// UUID related definition
#define UUID_LEN_HEX 16  // 128 bits => 16 bytes
#define UUID_LEN_STR 37  // hex to string, plus 4 dash, plus 1 '\0'
#define UUID_VERSION 1   // We currently just support version 1

typedef struct _WSC_UUID_T{
	UINT32 timeLow;
	UINT16 timeMid;
	UINT16 timeHi_Version;
	UCHAR  clockSeqHi_Var;
	UCHAR  clockSeqLow;
	UCHAR  node[6];
}WSC_UUID_T;

extern UCHAR Wsc_Uuid_E[UUID_LEN_HEX];
extern UCHAR Wsc_Uuid_Str[UUID_LEN_STR];
// <<WCN vista logo>> --

// 802.1x authentication format
typedef	struct	_IEEE8021X_FRAME	{
	UCHAR	Version;					// 1.0
	UCHAR	Type;						// 0 = EAP Packet
	USHORT	Length;
}	IEEE8021X_FRAME, *PIEEE8021X_FRAME;

// EAP frame format
typedef	struct PACKED	_EAP_FRAME	{
	UCHAR	Code;						// 1 = Request, 2 = Response
	UCHAR	Id;
	USHORT	Length;
	UCHAR	Type;						// 1 = Identity, 0xfe = reserved, used by WSC
}	EAP_FRAME, *PEAP_FRAME;

// WSC fixed information within EAP
typedef	struct PACKED	_WSC_FRAME	{
	UCHAR	SMI[3];
	ULONG	VendorType;
	UCHAR	OpCode;
	UCHAR	Flags;
}	WSC_FRAME, *PWSC_FRAME;

// For WSC state machine states.
// We simplified it
typedef	enum	_WscState
{
	WSC_STATE_OFF,
// For AP Client support WPS Modification
	WSC_STATE_INIT,
// --
	WSC_STATE_FAIL,	
   	WSC_STATE_CONFIGURED,
// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
   	WSC_STATE_LINK_UP,
   	WSC_STATE_SEND_EAPOL_START,
#endif // APCLI_SUPPORT //
// --
	WSC_STATE_WAIT_EAPOL_START,
	WSC_STATE_WAIT_UPNP_START,
// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
	WSC_STATE_WAIT_REQ_ID,
#endif // APCLI_SUPPORT //
// --
	WSC_STATE_WAIT_RESP_ID,
// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
	WSC_STATE_WAIT_WSC_START,
#endif // APCLI_SUPPORT //
// --
	WSC_STATE_WAIT_M1,
	WSC_STATE_SENT_M1,
	WSC_STATE_WAIT_M2,
	WSC_STATE_RX_M2D,	
	WSC_STATE_SENT_M2D,
	WSC_STATE_WAIT_M3,
	WSC_STATE_WAIT_M4,
	WSC_STATE_WAIT_M5,
	WSC_STATE_WAIT_M6,
	WSC_STATE_WAIT_M7,
	WSC_STATE_WAIT_M8,
	WSC_STATE_WAIT_DONE,
	WSC_STATE_WAIT_ACK,
// For AP Client support WPS Modification
	WSC_STATE_WAIT_EAPFAIL
// --
}	WSC_STATE;

// WSC IE structure
typedef	struct PACKED	_WSC_IE
{
	USHORT	Type;
	USHORT	Length;
	UCHAR	Data[1];	// variable length data
}	WSC_IE, *PWSC_IE;

// WSC set mode
typedef	struct	_WSC_MODE
{
	ULONG				WscMode;		// predefined WSC mode, 1: PIN, 2: PBC
	UCHAR				PIN[8];			// 8 bytes PIN nunmber
	NDIS_802_11_SSID	WscSSID;
}	WSC_MODE, *PWSC_MODE;

// WSC saved message
typedef	struct	_WSC_MESSAGE
{
	ULONG	Length;			// Length of saved message
	UCHAR	Data[2048];		// Contents
}	WSC_MESSAGE, *PWSC_MESSAGE;

// Data structure to hold Enrollee and Registrar information
typedef struct	_WSC_DEV_INFO
{
	UCHAR	Version;
	UCHAR	Uuid[16];
	UCHAR	MacAddr[6];
	UCHAR	DeviceName[32];
	UCHAR	PriDeviceType[8];
	//  <<WCN vista logo>> ++, add by johnli
	USHORT	AuthType;
	USHORT	EncrType;
	//  <<WCN vista logo>> --
	USHORT	AuthTypeFlags;
	USHORT	EncrTypeFlags;
	UCHAR	ConnTypeFlags;
	USHORT	ConfigMethods;
	UCHAR	ScState;
	UCHAR	Manufacturer[64];
	UCHAR	ModelName[32];
	UCHAR	ModelNumber[32];
	UCHAR	SerialNumber[32];
	UCHAR	RfBand;
	ULONG	OsVersion;
	ULONG	FeatureId;
	USHORT	AssocState;
	USHORT	DevPwdId;
	USHORT	ConfigError;
	UCHAR	Ssid[32];
    UCHAR	NewKey[64 + 1]; // not sure sprintf would add '\0' or not, add one byte for \0'
    INT     NewKeyLen;
    UCHAR   NewKeyIndex;
}	WSC_DEV_INFO, *PWSC_DEV_INFO;

// data structure to store info of the instance of Registration protocol
typedef	struct	_WSC_REG_DATA
{
	// enrollee endpoint - filled in by the Registrar, NULL for Enrollee
	WSC_DEV_INFO	EnrolleeInfo;        
	// Registrar endpoint - filled in by the Enrollee, NULL for Registrar
	WSC_DEV_INFO	RegistrarInfo;    

	//Diffie Hellman parameters
//	BIGNUM		*DH_PubKey_Peer; //peer's pub key stored in bignum format
//	DH			*DHSecret;       //local key pair in bignum format
	UCHAR		EnrolleeRandom[192];	// Saved random byte for public key generation
	
// For AP Client support WPS Modification
	UCHAR		ReComputePke;
// --
	UCHAR		Pke[192];		//enrollee's raw pub key
	UCHAR		Pkr[192];		//registrar's raw pub key

	UCHAR		SecretKey[192];	// Secret key calculated by enrollee
	
	UCHAR		StaEncrSettings[128];		// to be sent in M2/M8 by reg & M7 by enrollee
	UCHAR		ApEncrSettings[1024];

	// Saved Message content for authenticator calculation
	WSC_MESSAGE	LastTx;
	WSC_MESSAGE	LastRx;

	// Device password
	UCHAR		PIN[8];
	
	// From KDF Key
	UCHAR		AuthKey[32];
	UCHAR		KeyWrapKey[16];
	UCHAR		Emsk[32];
	
	USHORT		EnrolleePwdId;
	UCHAR		EnrolleeNonce[16];		//N1, from enrollee
	UCHAR		RegistrarNonce[16];		//N2, from registrar
	UCHAR		SelfNonce[16];

	UCHAR		Psk1[16];
	UCHAR		Psk2[16];

	UCHAR		EHash1[32];
	UCHAR		EHash2[32];
	UCHAR		Es1[16];
	UCHAR		Es2[16];

	UCHAR		RHash1[32];
	UCHAR		RHash2[32];
	UCHAR		Rs1[16];
	UCHAR		Rs2[16];
}	WSC_REG_DATA, *PWSC_REG_DATA;

// WSC configured credential
typedef	struct	_WSC_CREDENTIAL
{
	NDIS_802_11_SSID	SSID;				// mandatory
	USHORT				AuthType;			// mandatory, 1: open, 2: wpa-psk, 4: shared, 8:wpa, 0x10: wpa2, 0x20: wpa2-psk
	USHORT				EncrType;			// mandatory, 1: none, 2: wep, 4: tkip, 8: aes
	UCHAR				Key[64];			// mandatory, Maximum 64 byte
	USHORT				KeyLength;
	UCHAR				MacAddr[6];			// mandatory, AP MAC address
	UCHAR				KeyIndex;			// optional, default is 1
	UCHAR				Rsvd[3];			// Make alignment
//	UCHAR				EapType;			// optional, 8 bytes maximum
//	UCHAR				EapIdLength;		
//	UCHAR				EapId[64];			// optional, Maximum 64 byte
//	ULONG				KeyAuto;			// optional
//	ULONG				Enable1x;			// optional
}	WSC_CREDENTIAL, *PWSC_CREDENTIAL;

// WSC configured profiles
typedef	struct	_WSC_PROFILE
{
	ULONG			ProfileCnt;
	WSC_CREDENTIAL	Profile[8];				// Support up to 8 profiles
}	WSC_PROFILE, *PWSC_PROFILE;

#if 0
typedef struct _WSC_UPNP_TIMEOUT_DATAINFO{
	PVOID	pAd;
	ULONG	idx;
}WSC_UPNP_TIMEOUT_DATAINFO;

#define WSC_UPNP_MSGQ_MAX_NUM	32
typedef struct PACKED _WSC_UPNP_MSG_ENTRY{
	ULONG 				eventID;
	ULONG 				ipAddr;
	ULONG				waitingState;
	struct timer_list	UPnPMsgTimer;
	WSC_UPNP_TIMEOUT_DATAINFO *dataInfo;
}WSC_UPNP_MSG_ENTRY, *PWSC_UPNP_MSG_ENTRY;
#endif

// WSC UPnP node info.
typedef struct PACKED _WSC_UPNP_NODE_INFO{
	BOOLEAN				bUPnPInProgress;
	BOOLEAN				bUPnPM2DTimerRunning;
	BOOLEAN				bUPnPMsgTimerRunning;
	BOOLEAN				bUPnPMsgTimerPending;
	LONG				registrarID;
	INT					M2DACKBalance;
	struct timer_list   UPnPMsgTimer;
	struct timer_list   UPnPM2DTimer;
	
}WSC_UPNP_NODE_INFO, *PWSC_UPNP_NODE_INFO;

// WSC control block
typedef	struct	_WSC_CTRL
{
	INT             WscConfMode;    // 0 Wsc not enable; 1 un-configure AP ; 3 un-configure AP with Proxy ; 
	                                // 5 un-configure AP with Registrar ; 7 un-configure AP with proxy and Registrar
	INT             WscMode;        // 1 PIN ;2 PBC set from UI dynamically
	UCHAR           WscConfStatus;  // 1 un-configured; 2 configured; need to update to .dat
	INT             WscStatus;      // for user to monitor the status
	INT             WscState;    	// WSC Protocl State: M1 to M8
	UINT            WscPinCode;     // record the UI's PIN code input when we are registrar
	// <<Reject Same PinCode>> ++, add by johnli
	UINT            WscLastPinCode;     // record last UI's PIN code input when we are registrar
	INT            WscLastStatus;     // record last EAP-Fail status when we are registrar
	// <<Reject Same PinCode>> --
	// For Re-Generate Pin-Code Modification
	UINT            WscEnrolleePinCode; // recored Device own PIN code.
	// --
	INT             WscSelReg;     // record the UI's PIN code input when we are registrar
// For AP Client support WPS Modification
	NDIS_802_11_SSID	    WscSsid;		        // select a desired ssid to connect for PIN mode 
// --
	WSC_REG_DATA	RegData;		// Registrar pair data	
	UCHAR           lastId;
	UCHAR           WscUseUPnP;
	BOOLEAN         EapMsgRunning;  // already recived Eap-Rsp(Identity) and sent M1 or Eap-Req(Start)
	UCHAR           WscRetryCount;
	UCHAR           EntryApIdx;
	UCHAR           EntryAddr[MAC_ADDR_LEN];
	BOOLEAN         Wsc2MinsTimerRunning;
	struct timer_list   Wsc2MinsTimer;
	WSC_PROFILE			WscProfile;		// Saved WSC profile after M8
	BSS_TABLE			WscBssTab;		// AP list for the PBC activated
	WSC_UPNP_NODE_INFO	WscUPnPNodeInfo;	//Use to save UPnP node related info.

    BOOLEAN             EapolTimerRunning; 
    BOOLEAN             EapolTimerPending;
    struct timer_list   EapolTimer;

    UCHAR               WpaPsk[64];
    INT                 WpaPskLen;
    BOOLEAN             bWscTrigger;        // TRUE: AP-Enrollee & AP-Registrar work, FALSE: AP-Enrollee & AP-Registrar stop working
    void 			*pPubKeyMem;
	void			*pSecKeyMem;
// For AP Client support WPS Modification
    PVOID                   pAd;
// --
}	WSC_CTRL, *PWSC_CTRL;

// structure to store Simple Config Attributes Info
typedef struct PACKED _WSC_LV_INFO {
    USHORT  ValueLen;
    UCHAR   Value[512];
} WSC_LV_INFO;

typedef struct PACKED _WSC_IE_HEADER {
	UCHAR elemId;
	UCHAR length;
	UCHAR oui[4];
} WSC_IE_HEADER;

typedef struct PACKED _WSC_CONFIGURED_VALUE {
	USHORT WscConfigured; // 1 un-configured; 2 configured
	UCHAR	WscSsid[32 + 1];
	USHORT WscAuthMode;	// mandatory, 0x01: open, 0x02: wpa-psk, 0x04: shared, 0x08:wpa, 0x10: wpa2, 0x20: wpa2-psk
	USHORT	WscEncrypType;	// 0x01: none, 0x02: wep, 0x04: tkip, 0x08: aes
	UCHAR	DefaultKeyIdx;
	UCHAR	WscWPAKey[64 + 1];
} WSC_CONFIGURED_VALUE;

/* 
	Following definitions are used for UPnP module to communicate msg.
*/

/* Ralink specific message header for Linux specific NETLINK socket. */
#define RTMP_WSC_NLMSG_HDR_LEN		24		//signature(8) + envID(4) + ackID(4) + msgLen(4) + Flag(2) + segLen(2)
typedef struct PACKED _RTMP_WSC_NLMSG_HDR{
	UCHAR	signature[8];	/* Signature used to identify that this's a Ralink specific NETLINK message. 
								MUST be "RAWSCMSG" currently.
							*/
	UMAXLONG	envID;			// Unique event Identification assigned by sender.
	UMAXLONG	ackID;			// Notify that this message is a repsone for the message whose event identifier is "ackID".
	ULONG	msgLen;			// Totally length for this message. This message may seperate in serveral packets.
	USHORT	flags;			
	USHORT	segLen;			/* The "segLen" means the actual data length in this one msg packet.
								Because the NETLINK socket just support 256bytes for "IWCUSTOM" typed message, so we may 
								need to do fragement for our msg. If one message was fragemented as serveral pieces, the 
								user space receiver need to re-assemble it.
							 */
}RTMP_WSC_NLMSG_HDR;

/* Ralink specific WSC Mesage Header definition. 
*/
#define RTMP_WSC_MSG_HDR_LEN		12	//msgType(2) + msgSubType(2) + ipAddr(4) + len(4)
typedef struct PACKED _RTMP_WSC_MSG_HDR{
	USHORT	msgType;
	USHORT	msgSubType;
	ULONG   ipAddr;
	ULONG   msgLen;		//Not include this header.
}RTMP_WSC_MSG_HDR;

#define WSC_MSG_TYPE_ENROLLEE    0x1
#define WSC_MSG_TYPE_PROXY	 0x2
#define WSC_MSG_TYPE_REGISTRAR   0x3
#define WSC_MSG_TYPE_CTRL	 0x4
#define WSC_MSG_TYPE_MGMT	 0x5

PCHAR   WscGetAuthTypeStr(
    IN  USHORT authFlag);

PCHAR   WscGetEncryTypeStr(
    IN  USHORT encryFlag);

#define IWEVCUSTOM_MSG_MAX_LEN  255 //refer to kernel definition. <linux/wireless.h>
#define IWEVCUSTOM_PAYLOD_MAX_LEN  (IWEVCUSTOM_MSG_MAX_LEN - RTMP_WSC_NLMSG_HDR_LEN)


#define WSC_U2KMSG_HDR_LEN	41
typedef	struct PACKED _RTMP_WSC_U2KMSG_HDR{
	ULONG			envID;					//Event ID.
	UCHAR			Addr1[MAC_ADDR_LEN];	//Must set as all zero.	
	UCHAR			Addr2[MAC_ADDR_LEN];	//Sender MAC address. Not used now.
	UCHAR			Addr3[MAC_ADDR_LEN];	//Receiver MAC address. Not used now.
	UCHAR			rsvWLHdr[2];			//Reserved space for remained 802.11 hdr content.
	UCHAR			rsv1HHdr[LENGTH_802_1_H];//Reserved space for 802.1h header
	IEEE8021X_FRAME	IEEE8021XHdr;			//802.1X header
	EAP_FRAME		EAPHdr;					//EAP frame header.
}RTMP_WSC_U2KMSG_HDR;
//--- Used for UPnP module to communicate msg.

#endif	// __WSC_H__

