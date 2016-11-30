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
    rtmp_def.h

    Abstract:
    Miniport related definition header

    Revision History:
    Who         When          What
    --------    ----------    ----------------------------------------------
    Paul Lin    08-01-2002    created

*/
#ifndef __RTMP_DEF_H__
#define __RTMP_DEF_H__


/*  
 * If define THREAD_ISR, then we need to protect
 * McastPsQueue, TxRing, TxSwQueue 
 * Refer to UpdateBeaconFrame() in connect.c 
 */

#define THREAD_ISR
#define NONCOPY_RX


//
//  Debug information verbosity: lower values indicate higher urgency
//
#define RT_DEBUG_OFF        0
#define RT_DEBUG_ERROR      1
#define RT_DEBUG_WARN       2
#define RT_DEBUG_TRACE      3
#define RT_DEBUG_INFO       4
#define RT_DEBUG_LOUD       5

#define NIC_DBG_STRING      ("*RT61*")

#ifdef BIG_ENDIAN
#define DIR_READ                    0
#define DIR_WRITE                   1
#define TYPE_TXD                    0
#define TYPE_RXD                    1
#endif

//
// Entry number for each DMA descriptor ring
//
#ifdef THREAD_ISR
#define TX_RING_SIZE            96
#define RX_RING_SIZE            96
#else
#define TX_RING_SIZE            64
#define RX_RING_SIZE            64
#endif
#define MGMT_RING_SIZE          32
#define NUM_OF_LOCAL_TXBUF      64
#define LOCAL_TXBUF_SIZE        2048
#define TXD_SIZE                64
#define RXD_SIZE                64
#define TX_DMA_1ST_BUFFER_SIZE  64    // only the 1st physical buffer is pre-allocated
#define MGMT_DMA_BUFFER_SIZE    2048
#ifdef TC_SOC
#define RX_DMA_BUFFER_SIZE      (SKB_WITH_OVERHEAD(2048) - NET_SKB_PAD)
#else
#define RX_DMA_BUFFER_SIZE      4096
#endif
#define	MAX_FRAME_SIZE			2346					// Maximum 802.11 frame size
#define MAX_AGGREGATION_SIZE    4096
#define	MAX_TX_PROCESS			8 

#ifdef THREAD_ISR
#define	MAX_RX_PROCESS			(RX_RING_SIZE) 	//16
#define MAX_DMA_DONE_PROCESS    (TX_RING_SIZE)
#define MAX_TX_DONE_PROCESS     (TX_RING_SIZE)	//8
#else
#define	MAX_RX_PROCESS			16
#define MAX_DMA_DONE_PROCESS    TX_RING_SIZE
#define MAX_TX_DONE_PROCESS     8
#endif

#define	MAX_CLIENT				2

//
//  AP's client table operation status flags
//
#define fCLIENT_STATUS_WMM_CAPABLE          0x00000001  // CLIENT can parse QOS DATA frame
#define fCLIENT_STATUS_AGGREGATION_CAPABLE  0x00000002  // CLIENT can receive Ralink's proprietary TX aggregation frame
#define fCLIENT_STATUS_PIGGYBACK_CAPABLE    0x00000004  // CLIENT support piggy-back
#define fCLIENT_STATUS_APSD_CAPABLE         0x00000008  // CLIENT can parse QOS APSD info

//
//  RTMP_ADAPTER flags
//
#define fRTMP_ADAPTER_MAP_REGISTER          0x00000001
#define fRTMP_ADAPTER_INTERRUPT_IN_USE      0x00000002
#define fRTMP_ADAPTER_HARDWARE_ERROR        0x00000004
#define fRTMP_ADAPTER_SCATTER_GATHER        0x00000008
//#define fRTMP_ADAPTER_SEND_PACKET_ERROR     0x00000010
//#define fRTMP_ADAPTER_RECEIVE_PACKET_ERROR  0x00000020
#define fRTMP_ADAPTER_HALT_IN_PROGRESS      0x00000040
#define fRTMP_ADAPTER_RESET_IN_PROGRESS     0x00000080
#ifdef THREAD_ISR
#define fRTMP_ADAPTER_REMOVE_IN_PROGRESS  0x00000100
#endif 
//#define fRTMP_ADAPTER_TX_RING_ALLOCATED   0x00000200
//#define fRTMP_ADAPTER_ATIM_RING_ALLOCATED 0x00000400
//#define fRTMP_ADAPTER_PRIO_RING_ALLOCATED 0x00000800
//#define fRTMP_ADAPTER_RX_RING_ALLOCATED   0x00001000
#define fRTMP_ADAPTER_INTERRUPT_ACTIVE      0x00002000
#define fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS  0x00004000
#define	fRTMP_ADAPTER_MEDIA_STATE_CHANGE	0x00008000
//#define	fRTMP_ADAPTER_MEDIA_STATE_PENDING	0x00010000
#define	fRTMP_ADAPTER_RADIO_OFF				0x00020000
#define	fRTMP_ADAPTER_RADIO_MEASUREMENT		0x00040000
#define	fRTMP_ADAPTER_NIC_NOT_EXIST      	0x02000000


//
//  STA operation status flags
//
#define fOP_STATUS_INFRA_ON                 0x00000001
#define fOP_STATUS_ADHOC_ON                 0x00000002
#define fOP_STATUS_BG_PROTECTION_INUSED     0x00000004
#define fOP_STATUS_SHORT_SLOT_INUSED        0x00000008
#define fOP_STATUS_SHORT_PREAMBLE_INUSED    0x00000010
#define fOP_STATUS_RECEIVE_DTIM             0x00000020
#define fOP_STATUS_TX_RATE_SWITCH_ENABLED   0x00000040
#define fOP_STATUS_MEDIA_STATE_CONNECTED    0x00000080
#define fOP_STATUS_WMM_INUSED               0x00000100
#define fOP_STATUS_AGGREGATION_INUSED       0x00000200
#define fOP_STATUS_DOZE                     0x00000400  // debug purpose

//
// Error code section
//
// NDIS_ERROR_CODE_ADAPTER_NOT_FOUND
#define ERRLOG_READ_PCI_SLOT_FAILED     0x00000101L
#define ERRLOG_WRITE_PCI_SLOT_FAILED    0x00000102L
#define ERRLOG_VENDOR_DEVICE_NOMATCH    0x00000103L

// NDIS_ERROR_CODE_ADAPTER_DISABLED
#define ERRLOG_BUS_MASTER_DISABLED      0x00000201L

// NDIS_ERROR_CODE_UNSUPPORTED_CONFIGURATION
#define ERRLOG_INVALID_SPEED_DUPLEX     0x00000301L
#define ERRLOG_SET_SECONDARY_FAILED     0x00000302L

// NDIS_ERROR_CODE_OUT_OF_RESOURCES
#define ERRLOG_OUT_OF_MEMORY            0x00000401L
#define ERRLOG_OUT_OF_SHARED_MEMORY     0x00000402L
#define ERRLOG_OUT_OF_MAP_REGISTERS     0x00000403L
#define ERRLOG_OUT_OF_BUFFER_POOL       0x00000404L
#define ERRLOG_OUT_OF_NDIS_BUFFER       0x00000405L
#define ERRLOG_OUT_OF_PACKET_POOL       0x00000406L
#define ERRLOG_OUT_OF_NDIS_PACKET       0x00000407L
#define ERRLOG_OUT_OF_LOOKASIDE_MEMORY  0x00000408L

// NDIS_ERROR_CODE_HARDWARE_FAILURE
#define ERRLOG_SELFTEST_FAILED          0x00000501L
#define ERRLOG_INITIALIZE_ADAPTER       0x00000502L
#define ERRLOG_REMOVE_MINIPORT          0x00000503L

// NDIS_ERROR_CODE_RESOURCE_CONFLICT
#define ERRLOG_MAP_IO_SPACE             0x00000601L
#define ERRLOG_QUERY_ADAPTER_RESOURCES  0x00000602L
#define ERRLOG_NO_IO_RESOURCE           0x00000603L
#define ERRLOG_NO_INTERRUPT_RESOURCE    0x00000604L
#define ERRLOG_NO_MEMORY_RESOURCE       0x00000605L

//============================================================
// Length definitions
#define PEER_KEY_NO                       2
#define CRC_LEN                           4
#define MAC_ADDR_LEN                      6
#define TIMESTAMP_LEN                     8
#define MAX_LEN_OF_SUPPORTED_RATES        12    // 1, 2, 5.5, 11, 6, 9, 12, 18, 24, 36, 48, 54
//#define MAX_NUM_OF_POWER_LEVEL            8
//#define MAX_NUM_OF_DOMAIN                 8
#define MAX_LEN_OF_KEY                    32      // 32 octets == 256 bits, Redefine for WPA
#define MAX_NUM_OF_CHANNELS               43  //1-14, 36/40/44/48/52/56/60/64/100/104/108/112/116/120/124/ 
                                              //128/132/136/140/149/153/157/161/165/34/38/42/46 + 1 as NULL termination
#define MAX_LEN_OF_PEER_KEY               16
#define MAX_LEN_OF_MANUFACTURE_ID         32
#define MAX_LEN_OF_PRODUCT_ID             32
#define MAX_LEN_OF_SSID                   32
#define CIPHER_TEXT_LEN                   128
#define MAX_LEN_OF_MLME_BUFFER            2048
#define MAX_FRAME_LEN                     2338
#define	MAX_VIE_LEN                       128	// New for WPA cipher suite variable IE sizes.

#define MAX_TX_POWER_LEVEL                100   /* mW */
#define MAX_RSSI_TRIGGER                 -10    /* dBm */
#define MIN_RSSI_TRIGGER                 -200   /* dBm */
#define MAX_FRAG_THRESHOLD                2346  /* byte count */
#define MIN_FRAG_THRESHOLD                256   /* byte count */
#define MAX_RTS_THRESHOLD                 2347  /* byte count */

// key related definitions
#define SHARE_KEY_NUM                     4
#define MAX_LEN_OF_SHARE_KEY              16
#define PAIRWISE_KEY_NO                   64
#define GROUP_KEY_NO                      4

// power status related definitions
#define PWR_ACTIVE                        0
#define PWR_SAVE                          1
#define PWR_UNKNOWN                       2

// BSS Type definitions
#define BSS_ADHOC                         0  // = Ndis802_11IBSS
#define BSS_INFRA                         1  // = Ndis802_11Infrastructure
#define BSS_ANY                           2  // = Ndis802_11AutoUnknown

// Reason code definitions
#define REASON_RESERVED                   0
#define REASON_UNSPECIFY                  1
#define REASON_NO_LONGER_VALID            2
#define REASON_DEAUTH_STA_LEAVING         3
#define REASON_DISASSOC_INACTIVE          4
#define REASON_DISASSPC_AP_UNABLE         5
#define REASON_CLS2ERR                    6
#define REASON_CLS3ERR                    7
#define REASON_DISASSOC_STA_LEAVING       8
#define REASON_STA_REQ_ASSOC_NOT_AUTH     9
// new for wpa
#define REASON_INVALID_IE                 13
#define REASON_MIC_FAILURE                14
#define REASON_4_WAY_TIMEOUT              15
#define REASON_GROUP_KEY_UPDATE_TIMEOUT   16
#define REASON_IE_DIFFERENT               17
#define REASON_MCIPHER_NOT_VALID          18
#define REASON_UCIPHER_NOT_VALID          19
#define REASON_AKMP_NOT_VALID             20
#define REASON_UNSUPPORT_RSNE_VER         21
#define REASON_INVALID_RSNE_CAP           22
#define REASON_8021X_AUTH_FAIL            23

#define REASON_QOS_UNSPECIFY              32	
#define REASON_QOS_LACK_BANDWIDTH         33	
#define REASON_POOR_CHANNEL_CONDITION     34
#define REASON_QOS_OUTSIDE_TXOP_LIMITION  35
#define REASON_QOS_QSTA_LEAVING_QBSS      36
#define REASON_QOS_UNWANTED_MECHANISM     37
#define REASON_QOS_MECH_SETUP_REQUIRED    38
#define REASON_QOS_REQUEST_TIMEOUT        39
#define REASON_QOS_CIPHER_NOT_SUPPORT     45

// Status code definitions
#define MLME_SUCCESS                      0
#define MLME_UNSPECIFY_FAIL               1
#define MLME_CANNOT_SUPPORT_CAP           10
#define MLME_REASSOC_DENY_NO_ASSOC_EXIST  11 
#define MLME_ASSOC_DENY_OUT_SCOPE         12
#define MLME_ALG_NOT_SUPPORT              13
#define MLME_SEQ_NR_OUT_OF_SEQUENCE       14
#define MLME_REJ_CHALLENGE_FAILURE        15
#define MLME_REJ_TIMEOUT                  16
#define MLME_ASSOC_REJ_UNABLE_HANDLE_STA  17
#define MLME_ASSOC_REJ_DATA_RATE          18

#define MLME_ASSOC_REJ_NO_EXT_RATE        22
#define MLME_ASSOC_REJ_NO_EXT_RATE_PBCC   23
#define MLME_ASSOC_REJ_NO_CCK_OFDM        24

#define MLME_QOS_UNSPECIFY                32
#define MLME_REQUEST_DECLINED             37
#define MLME_REQUEST_WITH_INVALID_PARAM   38
#define MLME_DLS_NOT_ALLOW_IN_QBSS        48
#define MLME_DEST_STA_NOT_IN_QBSS         49
#define MLME_DEST_STA_IS_NOT_A_QSTA       50

#define MLME_INVALID_FORMAT               0x51
#define MLME_FAIL_NO_RESOURCE             0x52
#define MLME_STATE_MACHINE_REJECT         0x53
#define MLME_MAC_TABLE_FAIL               0x54

//IE code
#define IE_SSID                           0
#define IE_SUPP_RATES                     1
#define IE_FH_PARM                        2
#define IE_DS_PARM                        3
#define IE_CF_PARM                        4
#define IE_TIM                            5
#define IE_IBSS_PARM                      6
#define IE_COUNTRY                        7     // 802.11d
#define IE_802_11D_REQUEST                10    // 802.11d
#define IE_QBSS_LOAD                      11    // 802.11e d9
#define IE_EDCA_PARAMETER                 12    // 802.11e d9
#define IE_TSPEC                          13    // 802.11e d9 
#define IE_TCLAS                          14    // 802.11e d9
#define IE_SCHEDULE                       15    // 802.11e d9
#define IE_CHALLENGE_TEXT                 16
#define IE_POWER_CONSTRAINT               32    // 802.11h d3.3
#define IE_POWER_CAPABILITY               33    // 802.11h d3.3
#define IE_TPC_REQUEST                    34    // 802.11h d3.3
#define IE_TPC_REPORT                     35    // 802.11h d3.3
#define IE_SUPP_CHANNELS                  36    // 802.11h d3.3
#define IE_CHANNEL_SWITCH_ANNOUNCEMENT    37    // 802.11h d3.3
#define IE_MEASUREMENT_REQUEST            38    // 802.11h d3.3
#define IE_MEASUREMENT_REPORT             39    // 802.11h d3.3
#define IE_QUIET                          40    // 802.11h d3.3
#define IE_IBSS_DFS                       41    // 802.11h d3.3
#define IE_ERP                            42    // 802.11g
#define IE_TS_DELAY                       43    // 802.11e d9
#define IE_TCLAS_PROCESSING               44    // 802.11e d9
#define IE_QOS_CAPABILITY                 45    // 802.11e d6
#define IE_EXT_SUPP_RATES                 50    // 802.11g
#define IE_WPA                            221   // WPA
#define IE_VENDOR_SPECIFIC                221   // Wifi,WMM (WME),EOU
#define IE_RSN                            48    // 802.11i d3.0
#define IE_WPA2								48

// Message type for the MLME state machine
// Messages for Associate state machine
#define ASSOC_MACHINE_BASE          0
#define MT2_MLME_DISASSOC_REQ       0
#define MT2_PEER_DISASSOC_REQ       1
#define MT2_PEER_ASSOC_REQ          2
#define MT2_PEER_REASSOC_REQ        3
#define MT2_CLS3ERR                 4
#define MAX_ASSOC_MSG               5

// Messages for Authentication state machine
#define AUTH_MACHINE_BASE           11
#define MT2_MLME_DEAUTH_REQ         11
#define MT2_CLS2ERR                 12
#define MAX_AUTH_MSG                 2

// Messages for authentication response state machine
#define AUTH_RSP_MACHINE_BASE       16
#define MT2_AUTH_CHALLENGE_TIMEOUT  16
#define MT2_PEER_AUTH_ODD           17
#define MT2_PEER_DEAUTH             18
#define MAX_AUTH_RSP_MSG             3

// Messages for the sync state machine
#define SYNC_MACHINE_BASE           19
#define MT2_MLME_SCAN_REQ           19
#define MT2_PEER_BEACON             20
#define MT2_PEER_PROBE_RSP          21
#define MT2_SCAN_TIMEOUT            22
#define MT2_PEER_PROBE_REQ          23
#define MAX_SYNC_MSG                5

//Messages for the wpa state machine,
#define WPA_MACHINE_BASE            26
#define MACHINE_TYPE_EAPPacket      26
#define MACHINE_TYPE_EAPOLStart     27
#define MACHINE_TYPE_EAPOLLogoff    28
#define MACHINE_TYPE_EAPOLKey       29
#define MACHINE_TYPE_EAPOLASFAlert  30
#define MAX_WPA_MSG                 5

//Messages for the DLS state machine
#define DLS_MACHINE_BASE            31
#define MT2_PEER_DLS_REQ            31
#define MT2_PEER_DLS_RSP            32
#define MT2_PEER_DLS_TEAR_DOWN      33
#define MAX_DLS_MSG                 3

#ifdef WSC_SUPPORT
//Messages for the WSC State machine
#define	WSC_IDLE					0
#define	MAX_WSC_STATE				1
#define	WSC_FUNC_SIZE				MAX_WSC_STATE * MAX_WSC_MSG
#endif // WSC_SUPPORT //

// Confirm message
#define MT2_ASSOC_CONF              34
#define MT2_AUTH_CONF               35
#define MT2_DEAUTH_CONF             36
#define MT2_DISASSOC_CONF           37
#define MT2_REASSOC_CONF            38
#define MT2_PWR_MGMT_CONF           39
#define MT2_JOIN_CONF               40
#define MT2_SCAN_CONF               41
#define MT2_START_CONF              42
#define MT2_GET_CONF                43
#define MT2_SET_CONF                44
#define MT2_RESET_CONF              45

// Indication message
#define MT2_DEAUTH_IND              46
#define MT2_ASSOC_IND               47
#define MT2_DISASSOC_IND            48
#define MT2_REASSOC_IND             49
#define MT2_AUTH_IND                50

#define MT2_SCAN_END_CONF           51  // For scan end
#define MT2_MLME_ROAMING_REQ        52

#ifdef APCLI_SUPPORT
//ApCli authentication state machine
#define STA_AUTH_REQ_IDLE                0
#define STA_AUTH_WAIT_SEQ2               1
#define STA_AUTH_WAIT_SEQ4               2
#define STA_MAX_AUTH_STATE               3

#define STA_AUTH_MACHINE_BASE            0
#define STA_MT2_MLME_AUTH_REQ            0
#define STA_MT2_PEER_AUTH_EVEN           1
#define STA_MT2_PEER_DEAUTH              2
#define STA_MT2_AUTH_TIMEOUT             3
#define STA_MAX_AUTH_MSG                 4

#define STA_AUTH_FUNC_SIZE               (STA_MAX_AUTH_STATE * STA_MAX_AUTH_MSG)

//ApCli association state machine
#define STA_ASSOC_IDLE                   0
#define STA_ASSOC_WAIT_RSP               1
#define STA_MAX_ASSOC_STATE              2

#define STA_ASSOC_MACHINE_BASE           0
#define STA_MT2_MLME_ASSOC_REQ           0
#define STA_MT2_MLME_DISASSOC_REQ        1  
#define STA_MT2_PEER_DISASSOC_REQ        2
#define STA_MT2_PEER_ASSOC_RSP           3
#define STA_MT2_ASSOC_TIMEOUT            4
#define STA_MAX_ASSOC_MSG                5

#define STA_ASSOC_FUNC_SIZE              (STA_MAX_ASSOC_STATE * STA_MAX_ASSOC_MSG)

//ApCli sync state machine
#define STA_SYNC_IDLE                   0  // merge NO_BSS,IBSS_IDLE,IBSS_ACTIVE and BSS in to 1 state
#define STA_JOIN_WAIT_PROBE_RSP         1
#define STA_MAX_SYNC_STATE              2

#define STA_SYNC_MACHINE_BASE           0
#define STA_MT2_MLME_PROBE_REQ          0
#define STA_MT2_PEER_PROBE_RSP          1
#define STA_MT2_PROBE_TIMEOUT           2
#define STA_MAX_SYNC_MSG                3

#define STA_SYNC_FUNC_SIZE              (STA_MAX_SYNC_STATE * STA_MAX_SYNC_MSG)

//ApCli ctrl state machine
#define STA_CTRL_DISCONNECTED           0  // merge NO_BSS,IBSS_IDLE,IBSS_ACTIVE and BSS in to 1 state
#define STA_CTRL_PROBE                  1
#define STA_CTRL_AUTH                   2
#define STA_CTRL_AUTH_2                 3
#define STA_CTRL_ASSOC                  4
#define STA_CTRL_DEASSOC                5
#define STA_CTRL_CONNECTED              6
#define STA_MAX_CTRL_STATE              7

#define STA_CTRL_MACHINE_BASE           0
#define STA_CTRL_JOIN_REQ               0
#define STA_CTRL_PROBE_RSP              1
#define STA_CTRL_AUTH_RSP               2
#define STA_CTRL_DISCONNECT_REQ         3
#define STA_CTRL_PEER_DISCONNECT_REQ    4
#define STA_CTRL_ASSOC_RSP              5
#define STA_CTRL_DEASSOC_RSP            6
#define STA_CTRL_JOIN_REQ_TIMEOUT       7
#define STA_CTRL_AUTH_REQ_TIMEOUT       8
#define STA_CTRL_ASSOC_REQ_TIMEOUT      9
#define STA_MAX_CTRL_MSG                10

#define STA_CTRL_FUNC_SIZE              (STA_MAX_CTRL_STATE * STA_MAX_CTRL_MSG)

// ApCli WPA state machine
#define STA_WPA_PSK_IDLE				0
#define STA_MAX_WPA_PSK_STATE			1

// ApCli WPA MSG Type
#define STA_WPA_MACHINE_BASE			0
#define STA_MT2_EAPPacket				0
#define STA_MT2_EAPOLStart				1
#define STA_MT2_EAPOLLogoff				2
#define STA_MT2_EAPOLKey				3
#define STA_MT2_EAPOLASFAlert			4
#define STA_MAX_WPA_PSK_MSG				5

#define	STA_WPA_PSK_FUNC_SIZE			(STA_MAX_WPA_PSK_STATE * STA_MAX_WPA_PSK_MSG)

#endif	//#ifdef APCLI_SUPPORT

// value domain of MacHdr.tyte, which is b3..b2 of the 1st-byte of MAC header
#define BTYPE_MGMT                  0
#define BTYPE_CNTL                  1
#define BTYPE_DATA                  2

// value domain of MacHdr.subtype, which is b7..4 of the 1st-byte of MAC header
// Management frame
#define SUBTYPE_ASSOC_REQ           0
#define SUBTYPE_ASSOC_RSP           1
#define SUBTYPE_REASSOC_REQ         2
#define SUBTYPE_REASSOC_RSP         3
#define SUBTYPE_PROBE_REQ           4
#define SUBTYPE_PROBE_RSP           5
#define SUBTYPE_BEACON              8
#define SUBTYPE_ATIM                9
#define SUBTYPE_DISASSOC            10
#define SUBTYPE_AUTH                11
#define SUBTYPE_DEAUTH              12
#define SUBTYPE_ACTION              13

// Control Frame
#define SUBTYPE_BLOCK_ACK_REQ       8
#define SUBTYPE_BLOCK_ACK           9
#define SUBTYPE_PS_POLL             10
#define SUBTYPE_RTS                 11  // 1011
#define SUBTYPE_CTS                 12  // 1100
#define SUBTYPE_ACK                 13  // 1101
#define SUBTYPE_CFEND               14
#define SUBTYPE_CFEND_CFACK         15

// Data Frame
#define SUBTYPE_DATA                0
#define SUBTYPE_DATA_CFACK          1
#define SUBTYPE_DATA_CFPOLL         2
#define SUBTYPE_DATA_CFACK_CFPOLL   3
#define SUBTYPE_NULL_FUNC           4
#define SUBTYPE_CFACK               5  // 0101
#define SUBTYPE_CFPOLL              6
#define SUBTYPE_CFACK_CFPOLL        7
#define SUBTYPE_QDATA               8
#define SUBTYPE_QDATA_CFACK         9
#define SUBTYPE_QDATA_CFPOLL        10
#define SUBTYPE_QDATA_CFACK_CFPOLL  11
#define SUBTYPE_QOS_NULL            12
#define SUBTYPE_QOS_CFACK           13
#define SUBTYPE_QOS_CFPOLL          14
#define SUBTYPE_QOS_CFACK_CFPOLL    15

// Management Action frame definition
#define CATEGORY_QOS				1
#define CATEGORY_DLS				2
#define CATEGORY_BLOCK_ACK			3

// DLS Action frame definition
#define ACTION_DLS_REQUEST			0
#define ACTION_DLS_RESPONSE			1
#define ACTION_DLS_TEARDOWN			2

// ACK policy of QOS Control field bit 6:5
#define NORMAL_ACK                  0x00  // b6:5 = 00
#define NO_ACK                      0x20  // b6:5 = 01
#define NO_EXPLICIT_ACK             0x40  // b6:5 = 10
#define BLOCK_ACK                   0x60  // b6:5 = 11

#define ASSOC_STATE_MACHINE         1
#define AUTH_STATE_MACHINE          2
#define AUTH_RSP_STATE_MACHINE      3
#define SYNC_STATE_MACHINE          4
#define WPA_STATE_MACHINE           5
#define DLS_STATE_MACHINE           6

#ifdef APCLI_SUPPORT
#define STA_AUTH_STATE_MACHINE      7
#define STA_ASSOC_STATE_MACHINE     8
#define STA_SYNC_STATE_MACHINE      9
#define STA_CTRL_STATE_MACHINE      10
#define STA_WPA_STATE_MACHINE    	11
#endif

//
// rtmp_data.c use these definition
//
#define	LENGTH_802_11				24
#define LENGTH_802_11_WITH_ADDR4    30
#define	LENGTH_802_11_AND_H			30
#define	LENGTH_802_11_CRC_H			34
#define	LENGTH_802_11_CRC			28
#define	LENGTH_802_3				14
#define LENGTH_802_3_TYPE			2
#define LENGTH_802_1_H				8
#define LENGTH_EAPOL_H				4
#define	LENGTH_CRC					4
#define	MAX_SEQ_NUMBER				0x0fff

#define	TX_RESULT_SUCCESS       0
#define TX_RESULT_ZERO_LENGTH   1
#define TX_RESULT_UNDER_RUN     2
#define TX_RESULT_PHY_ERROR     4
#define	TX_RESULT_RETRY_FAIL	6

#define	RATE_1					0
#define	RATE_2					1
#define	RATE_5_5				2
#define	RATE_11					3
#define RATE_6                  4   // OFDM
#define RATE_9                  5   // OFDM
#define RATE_12                 6   // OFDM
#define RATE_18                 7   // OFDM
#define RATE_24                 8   // OFDM
#define RATE_36                 9   // OFDM
#define RATE_48                 10  // OFDM
#define RATE_54                 11  // OFDM
#define RATE_FIRST_OFDM_RATE    RATE_6
#define RATE_AUTO_SWITCH        255 // for UserCfg.FixedTxRate only

// pTxD->Ifs
#define	IFS_BACKOFF				0
#define	IFS_SIFS				1

// pTxD->RetryMode
#define	LONG_RETRY				1
#define	SHORT_RETRY				0

// Country Region definition
#define REGION_0_BG_BAND                  0       // 1-11
#define REGION_1_BG_BAND                  1       // 1-13
#define REGION_2_BG_BAND                  2       // 10-11
#define REGION_3_BG_BAND                  3       // 10-13
#define REGION_4_BG_BAND                  4       // 14
#define REGION_5_BG_BAND                  5       // 1-14
#define REGION_6_BG_BAND                  6       // 3-9
#define REGION_7_BG_BAND                  7       // 5-13

#define REGION_0_A_BAND                   0       // 36, 40, 44, 48, 52, 56, 60, 64, 149, 153, 157, 161, 165
#define REGION_1_A_BAND                   1       // 36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140
#define REGION_2_A_BAND                   2       // 36, 40, 44, 48, 52, 56, 60, 64
#define REGION_3_A_BAND                   3       // 52, 56, 60, 64, 149, 153, 157, 161
#define REGION_4_A_BAND                   4       // 149, 153, 157, 161, 165
#define REGION_5_A_BAND                   5       // 149, 153, 157, 161
#define REGION_6_A_BAND                   6       // 36, 40, 44, 48
#define REGION_7_A_BAND                   7       // 36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 149, 153, 157, 161, 165
#define REGION_8_A_BAND                   8	  // 52, 56, 60, 64
#define REGION_9_A_BAND                   9	  // 34, 38, 42, 46 already not used
#define REGION_10_A_BAND                  10	  // 36, 40, 44, 48, 52, 56, 60, 64

// pTxD->CipherAlg
#define	CIPHER_NONE				0
#define	CIPHER_WEP64			1
#define	CIPHER_WEP128			2
#define	CIPHER_TKIP				3
#define	CIPHER_AES				4
#define CIPHER_CKIP64           5
#define CIPHER_CKIP128          6
#define CIPHER_TKIP_NO_MIC      7    // MIC has been appended by driver; 
                                     // not a valid value in hardware key table

// WEP Key TYPE
#define WEP_HEXADECIMAL_TYPE    0
#define WEP_ASCII_TYPE          1

// Stall execution time for ndisdpracquires[inlock in miniportReset function
#define	WAIT_TIME_FOR_SPINLOCK	10		// usec

// value domain of pAdapter->RfIcType
#define RFIC_5225                   1       // Support a/b/g mode
#define RFIC_5325                   2       // Support a/b/g mode
#define RFIC_2527					3       // Support b/g mode
#define RFIC_2529					4       // Support b/g mode

// LED Status.
#define LED_LINK_DOWN               0
#define LED_LINK_UP                 1
#define LED_RADIO_OFF               2
#define LED_RADIO_ON                3
#define LED_HALT                    4

// value domain of pAdapter->LedCntl.LedMode and E2PROM
#define LED_MODE_DEFAULT        0

// RC4 init value, used fro WEP & TKIP
#define PPPINITFCS32            0xffffffff   /* Initial FCS value */

#define PAIRWISE_KEY_TABLE			1
#define SHARED_KEY_TABLE			0

#define DEFAULT_BBP_TX_POWER    0
#define DEFAULT_RF_TX_POWER         5

#ifdef MBSS_SUPPORT
#define MAX_INI_BUFFER_SIZE			(4*2048)
#else
#define MAX_INI_BUFFER_SIZE			4096
#endif	/* MBSS_SUPPORT */

#define MAX_PARAM_BUFFER_SIZE		(2048) // enough for ACL (18*64)

#define MAX_NUM_OF_ACL_LIST				64
/*
*If you modify MAX_LEN_OF_MAC_TABLE into 8 value.
*When the system is booting up, sometime cpe will be crash.
*/
#define MAX_LEN_OF_MAC_TABLE            64
#define HASH_TABLE_SIZE                   256

// Event definition
#define MAX_NUM_OF_EVENT            10  // entry # in EVENT table
#define EVENT_MAX_EVENT_TYPE        6

#define EVENT_RESET_ACCESS_POINT    0 // Log = "hh:mm:ss   Restart Access Point"
#define EVENT_ASSOCIATED            1 // Log = "hh:mm:ss   STA 00:01:02:03:04:05 associated"
#define EVENT_DISASSOCIATED         2 // Log = "hh:mm:ss   STA 00:01:02:03:04:05 left this BSS"
#define EVENT_AGED_OUT              3 // Log = "hh:mm:ss   STA 00:01:02:03:04:05 was aged-out and removed from this BSS"
#define EVENT_COUNTER_M             4
#define EVENT_INVALID_PSK           5
#define EVENT_MAX_EVENT_TYPE        6

// MBSSID definition
#define ENTRY_NOT_FOUND             0xFF
#ifdef MBSS_SUPPORT
#ifdef APCLI_SUPPORT
#define MAX_MBSSID_NUM              3
#else
#define MAX_MBSSID_NUM              4
#endif /* APCLI_SUPPORT */
#else
#define MAX_MBSSID_NUM              1
#endif /* !MBSS_SUPPORT */
#define MAIN_MBSSID                 0
#define FIRST_MBSSID                1


/* After Linux 2.6.9, 
 * VLAN module use Private (from user) interface flags (netdevice->priv_flags). 
 * #define IFF_802_1Q_VLAN 0x1         --    802.1Q VLAN device.  in if.h     
 * ref to ip_sabotage_out() [ out->priv_flags & IFF_802_1Q_VLAN ] in br_netfilter.c
 * 
 * For this reason, we MUST use EVEN value in priv_flags
 */
#define INT_MAIN                    0x0100
#define INT_MBSSID                  0x0200
#define INT_WDS                     0x0300
#define INT_APCLI					0x0400

// APCLI definition
#ifdef APCLI_SUPPORT
#define	MAX_APCLI_ENTRY             1
#define APCLI_PAIRWISE_KEY_OFFSET   (PAIRWISE_KEY_NO - MAX_APCLI_ENTRY)    				// Ap Client links uses pairwise key#63 in ASIC pairwise key table when MAX_APCLI_ENTRY is 1
#else
#define	MAX_APCLI_ENTRY             0
#endif

// WDS definition
#define	MAX_WDS_ENTRY               4
#define WDS_PAIRWISE_KEY_OFFSET     (PAIRWISE_KEY_NO - MAX_APCLI_ENTRY - MAX_WDS_ENTRY)	// WDS links uses pairwise key#59 ~ 62 in ASIC pairwise key table when MAX_APCLI_ENTRY is 1

#define	WDS_DISABLE_MODE            0
#define	WDS_RESTRICT_MODE           1
#define	WDS_BRIDGE_MODE             2
#define	WDS_REPEATER_MODE           3
#define	WDS_LAZY_MODE               4

// 802.1X controlled port definition
#define	WPA_802_1X_PORT_SECURED			1
#define	WPA_802_1X_PORT_NOT_SECURED		2

#define	PAIRWISE_KEY			1
#define	GROUP_KEY				2

#define AUTH_MODE_OPEN                    0x00
#define AUTH_MODE_KEY                     0x01
#define AUTH_MODE_AUTO_SWITCH             0x03

// wpapsk EAPOL Key descripter frame format related length
#define LEN_KEY_DESC_NONCE          32
#define LEN_KEY_DESC_IV             16
#define LEN_KEY_DESC_RSC            8
#define LEN_KEY_DESC_ID             8
#define LEN_KEY_DESC_REPLAY         8
#define LEN_KEY_DESC_MIC            16

#define LEN_MASTER_KEY              32

// EAPOL EK, MK
#define LEN_EAP_EK                  16
#define LEN_EAP_MICK                16
#define LEN_EAP_KEY                 ((LEN_EAP_EK)+(LEN_EAP_MICK))
#define PMK_LEN                     32

// TKIP key related
#define LEN_PMKID					16
#define LEN_TKIP_EK                 16
#define LEN_TKIP_RXMICK             8
#define LEN_TKIP_TXMICK             8
#define LEN_AES_EK                  16
#define LEN_AES_KEY                 LEN_AES_EK
#define LEN_TKIP_KEY                ((LEN_TKIP_EK)+(LEN_TKIP_RXMICK)+(LEN_TKIP_TXMICK))
#define TKIP_AP_TXMICK_OFFSET       ((LEN_EAP_KEY)+(LEN_TKIP_EK))
#define TKIP_AP_RXMICK_OFFSET       (TKIP_AP_TXMICK_OFFSET+LEN_TKIP_TXMICK)
#define	TKIP_GTK_LENGTH             ((LEN_TKIP_EK)+(LEN_TKIP_RXMICK)+(LEN_TKIP_TXMICK))

#define LEN_PTK                     ((LEN_EAP_KEY)+(LEN_TKIP_KEY))
#define MAX_LEN_OF_RSNIE            80
#define MIN_LEN_OF_RSNIE            8

#ifdef TC_SOC
#define MAX_PACKETS_IN_QUEUE				400
#else
#define MAX_PACKETS_IN_QUEUE				1600    // to pass WMM A5-WPAPSK
#endif
#define MAX_PACKETS_IN_MCAST_PS_QUEUE		32	 
#define MAX_PACKETS_IN_PS_QUEUE				32

// definition RSSI Number
#define RSSI_NO_1					1
#define RSSI_NO_2					2

// definition of signal
#define	SIG_NONE					0
#define SIG_ASSOCIATION				1
#define SIG_REASSOCIATION			2

// definition of radar detection
#define RD_NORMAL_MODE				0	// Not found radar signal
#define RD_SWITCHING_MODE			1	// Found radar signal, and doing channel switch
#define RD_SILENCE_MODE				2	// After channel switch, need to be silence a while to ensure radar not found

#ifdef RALINK_ATE
#define	ATE_APSTOP                  0   // Stop AP
#define	ATE_APSTART                 1   // Start AP
#define	ATE_TXCONT                  2   // Continuous Transmit
#define	ATE_TXCARR                  3   // Transmit Carrier
#define	ATE_TXFRAME                 4   // Transmit Frames
#define	ATE_RXFRAME                 5   // Receive Frames
#endif	// RALINK_ATE

#define MAX_CFG_BUFFER_LEN  1536
#define MAX_NULL_FRAME_SIZE 10

#ifdef SNMP
// for snmp 
// to get manufacturer OUI, kathy
#define ManufacturerOUI_LEN			3
#define ManufacturerNAME			("Ralink Technology Company.")
#define	ResourceTypeIdName			("Ralink_ID")
#endif 

#define A_BAND_REGION_0				0
#define A_BAND_REGION_1				1
#define A_BAND_REGION_2				2
#define A_BAND_REGION_3				3
#define A_BAND_REGION_4				4
#define A_BAND_REGION_5				5
#define A_BAND_REGION_6				6
#define A_BAND_REGION_7				7
#define A_BAND_REGION_8				8
#define A_BAND_REGION_9				9
#define A_BAND_REGION_10			10

#define G_BAND_REGION_0				0
#define G_BAND_REGION_1				1
#define G_BAND_REGION_2				2
#define G_BAND_REGION_3				3
#define G_BAND_REGION_4				4
#define G_BAND_REGION_5				5
#define G_BAND_REGION_6				6
#define G_BAND_REGION_7				7

#ifdef CARRIER_DETECTION_SUPPORT
#define CARRIER_DETECT_HIGH_BOUNDARY_1	80
#define CARRIER_DETECT_HIGH_BOUNDARY_2	140
#define CARRIER_DETECT_LOW_BOUNDARY		40
#endif // CARRIER_DETECTION_SUPPORT //

#endif  // __RTMP_DEF_H__
