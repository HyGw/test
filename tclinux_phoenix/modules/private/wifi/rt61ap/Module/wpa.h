/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2005, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	mlme.h

	Abstract:

	Revision History:
	Who         When            What
	--------    ----------      ----------------------------------------------
	Name        Date            Modification logs
*/

#ifndef __WPA_H__
#define __WPA_H__

#define WPA_RETRY_EXEC_INTV         (1 * HZ)
#define PEER_MSG3_RETRY_EXEC_INTV	(3 * HZ)
#define GUPDATE_EXEC_INTV           (1 * HZ)

// Retry timer counter initial value
#define PEER_MSG1_RETRY_TIMER_CTR	0
#define PEER_MSG3_RETRY_TIMER_CTR	10
#define GRP_HS_RETRY_TIMER_CTR      20

//EAP Packet Type
#define EAPPacket       0
#define EAPOLStart      1
#define EAPOLLogoff     2
#define EAPOLKey        3
#define EAPOLASFAlert   4
#define EAPTtypeMax     5

#define EAPOL_MSG_INVALID   0
#define EAPOL_PAIR_MSG_1    1
#define EAPOL_PAIR_MSG_3    2
#define EAPOL_GROUP_MSG_1   3

// other EAP Code Request and Response are useful only in daemon.
#define EAP_CODE_SUCCESS    3
#define EAP_CODE_FAILURE    4

//EPA VERSION
#define EAPOL_VER           1
#define EAPOL_VER2          2

#define DESC_TYPE_TKIP      1
#define DESC_TYPE_AES       2

#define WPA1_KEY_DESC		0xFE
#define WPA2_KEY_DESC		0x2

#define LEN_MSG1_2WAY		0x7F

#define MAX_LEN_OF_EAP_HS   256

// group rekey interval
#define TIME_REKEY              0
#define PKT_REKEY               1
#define DISABLE_REKEY           2
#define MAX_REKEY               2

#define MAX_REKEY_INTER         0x3FFFFFF

#define PAIRWISEKEY			1
#define GROUPKEY			0

typedef struct PACKED _RSNIE {
    UCHAR   oui[4];
    USHORT  version;
    UCHAR   mcast[4];
    USHORT  ucount;
    struct PACKED {
        UCHAR oui[4];
    }ucast[1];
} RSNIE, *PRSNIE;

typedef struct PACKED _RSNIE2 {
    USHORT  version;
    UCHAR   mcast[4];
    USHORT  ucount;
    struct PACKED {
        UCHAR oui[4];
    }ucast[1];
} RSNIE2, *PRSNIE2;

typedef struct PACKED _RSNIE_AUTH {
    USHORT acount;
    struct PACKED {
        UCHAR oui[4];
    }auth[1];
} RSNIE_AUTH,*PRSNIE_AUTH;

typedef	union PACKED _RSN_CAPABILITIES	{
	struct	PACKED {
#ifdef BIG_ENDIAN
        USHORT		Rsvd:10;
        USHORT		GTKSA_R_Counter:2;
        USHORT		PTKSA_R_Counter:2;
        USHORT		No_Pairwise:1;
		USHORT		PreAuth:1;
#else
        USHORT		PreAuth:1;
		USHORT		No_Pairwise:1;
		USHORT		PTKSA_R_Counter:2;
		USHORT		GTKSA_R_Counter:2;
		USHORT		Rsvd:10;
#endif
	}	field;
	USHORT			word;
}	RSN_CAPABILITIES, *PRSN_CAPABILITIES;

typedef struct PACKED _EAP_HDR {
    UCHAR   ProVer;
    UCHAR   ProType;
    UCHAR   Body_Len[2];
    UCHAR   code;
    UCHAR   identifier;
    UCHAR   length[2]; // including code and identifier, followed by length-2 octets of data
} EAP_HDR, *PEAP_HDR;

typedef	struct PACKED _KEY_INFO
{
#ifdef BIG_ENDIAN
    UCHAR	KeyAck:1;
    UCHAR	Install:1;
    UCHAR	KeyIndex:2;
    UCHAR	KeyType:1;
    UCHAR	KeyDescVer:3;
    UCHAR	Rsvd:3;
    UCHAR	EKD:1;
    UCHAR	Request:1;
    UCHAR	Error:1;
    UCHAR	Secure:1;
    UCHAR	KeyMic:1;
#else
    UCHAR	KeyMic:1;
    UCHAR	Secure:1;
    UCHAR	Error:1;
    UCHAR	Request:1;
    UCHAR	EKD:1;
    UCHAR	Rsvd:3;
    UCHAR	KeyDescVer:3;
    UCHAR	KeyType:1;
    UCHAR	KeyIndex:2;
    UCHAR	Install:1;
    UCHAR	KeyAck:1;
#endif
} KEY_INFO, *PKEY_INFO;

typedef struct PACKED _KEY_DESCRIPTER {
    UCHAR       Type;
    KEY_INFO    Keyinfo;
    UCHAR       KeyLength[2];
    UCHAR       RCounter[LEN_KEY_DESC_REPLAY];
    UCHAR       Nonce[LEN_KEY_DESC_NONCE];
    UCHAR       IV[LEN_KEY_DESC_IV];
    UCHAR       RSC[LEN_KEY_DESC_RSC];
    UCHAR       ID[LEN_KEY_DESC_ID];
    UCHAR       MIC[LEN_KEY_DESC_MIC];
    UCHAR       DataLen[2];
    UCHAR       Data[MAX_LEN_OF_RSNIE];
} KEY_DESCRIPTER, *PKEY_DESCRIPTER;

typedef struct PACKED _EAPOL_PACKET {
    UCHAR           ProVer;
    UCHAR           ProType;
    UCHAR           Body_Len[2];
    KEY_DESCRIPTER  KeyDesc;
} EAPOL_PACKET, *PEAPOL_PACKET;

#define	ADD_ONE_To_64BIT_VAR(_V)		\
{										\
	UCHAR	cnt = LEN_KEY_DESC_REPLAY;	\
	do									\
	{									\
		cnt--;							\
		_V[cnt]++;						\
		if (cnt == 0)					\
			break;						\
	}while (_V[cnt] == 0);				\
}

//802.11i D10 page 83
typedef struct PACKED _GTK_ENCAP
{
#ifndef BIG_ENDIAN
    UCHAR               Kid:2;
    UCHAR               tx:1;
    UCHAR               rsv:5;
    UCHAR               rsv1;
#else
    UCHAR               rsv:5;
    UCHAR               tx:1;
    UCHAR               Kid:2;
    UCHAR               rsv1;    	
#endif
    UCHAR               GTK[32];
}   GTK_ENCAP, *PGTK_ENCAP;

typedef struct PACKED _KDE_ENCAP
{
    UCHAR               Type;
    UCHAR               Len;
    UCHAR               OUI[3];
    UCHAR               DataType;
    GTK_ENCAP      		GTKEncap;
}   KDE_ENCAP, *PKDE_ENCAP;

#ifdef APCLI_SUPPORT

VOID StaWpaStateMachineInit(
	IN PRTMP_ADAPTER pAd,
	IN STA_STATE_MACHINE *Sm,
	OUT STA_STATE_MACHINE_FUNC Trans[]);

VOID StaWpaEAPOLKeyAction(
    IN PRTMP_ADAPTER    pAd, 
    IN MLME_QUEUE_ELEM  *Elem);

BOOLEAN StaWPAMsgTypeSubst(
    IN  UCHAR   EAPType,
    OUT INT    *MsgType);

VOID    WpaPairMsg1Action(
    IN  PRTMP_ADAPTER   pAd, 
    IN  MLME_QUEUE_ELEM *Elem,
	IN	INT				IfIndex);

VOID    WpaPairMsg3Action(
    IN  PRTMP_ADAPTER   pAd, 
    IN  MLME_QUEUE_ELEM *Elem,
	IN	INT				IfIndex); 

VOID    Wpa2PairMsg1Action(
    IN  PRTMP_ADAPTER   pAd, 
    IN  MLME_QUEUE_ELEM *Elem,
	IN	INT				IfIndex);

VOID    WpaGroupMsg1Action(
    IN  PRTMP_ADAPTER   pAd, 
    IN  MLME_QUEUE_ELEM *Elem,
	IN	INT				IfIndex); 

BOOLEAN 	ApCliValidateRSNIE(
	IN		PRTMP_ADAPTER	pAdapter, 
	IN 		PEID_STRUCT     eid_ptr);

#ifndef RTL865X_SOC
VOID    ApCliWpaMacHeaderInit(
    IN      PRTMP_ADAPTER   pAd, 
    IN OUT  PHEADER_802_11  pHdr80211, 
    IN      UCHAR           wep, 
    IN      PUCHAR          pAddr1,
    IN      PUCHAR          pAddr2); 
    
VOID    Wpa2PairMsg3Action(
    IN  PRTMP_ADAPTER   pAd, 
    IN  MLME_QUEUE_ELEM *Elem,
	IN	INT				IfIndex);

BOOLEAN ParseKeyData(
    IN  PRTMP_ADAPTER   pAd,
    IN  PUCHAR          pKeyData,
    IN  UCHAR           KeyDataLen,
	IN	INT				IfIndex,
	IN	UCHAR			bPairewise);
#endif  /* endif - ifndef RTL865X_SOC */

#endif	/* endif - ifdef APCLI_SUPPORT */ 

BOOLEAN WPAMsgTypeSubst(
    IN  UCHAR   EAPType,
    OUT ULONG   *MsgType);

VOID WpaStateMachineInit(
    IN	PRTMP_ADAPTER       pAd, 
    IN  STATE_MACHINE       *S, 
    OUT STATE_MACHINE_FUNC  Trans[]);

MAC_TABLE_ENTRY *PACInquiry(
    IN	PRTMP_ADAPTER               pAd, 
    IN PUCHAR                      pAddr, 
    OUT NDIS_802_11_PRIVACY_FILTER  *Privacy,
    OUT WPA_STATE                   *WpaState);

BOOLEAN RTMPCheckWPAframe(
    IN PRTMP_ADAPTER    pAdapter,
    IN PUCHAR           pHeader,
    IN ULONG            DataByteCount,
    IN UCHAR            Offset);

BOOLEAN RTMPCheckMcast(
    IN PRTMP_ADAPTER    pAdapter,
    IN PEID_STRUCT      eid_ptr,
    IN MAC_TABLE_ENTRY	*pEntry);

BOOLEAN RTMPCheckUcast(
    IN PRTMP_ADAPTER    pAdapter,
    IN PEID_STRUCT      eid_ptr,
    IN MAC_TABLE_ENTRY	*pEntry);

BOOLEAN RTMPCheckAUTH(
    IN PRTMP_ADAPTER    pAdapter,
    IN PEID_STRUCT      eid_ptr,
    IN MAC_TABLE_ENTRY	*pEntry);

BOOLEAN RTMPCheckAKM(
    IN PUCHAR sta_akm,
    IN PUCHAR ap_rsn_ie,
    IN INT iswpa2
);

VOID RTMPHandleSTAKey(
    IN PRTMP_ADAPTER    pAdapter, 
    IN MAC_TABLE_ENTRY  *pEntry,
    IN MLME_QUEUE_ELEM  *Elem);

VOID WPAStart4WayHS(
    IN	PRTMP_ADAPTER   pAdapter, 
    IN 	MAC_TABLE_ENTRY *pEntry);

VOID WpaEAPPacketAction(
    IN PRTMP_ADAPTER    pAd, 
    IN MLME_QUEUE_ELEM  *Elem);

VOID WpaEAPOLStartAction(
    IN PRTMP_ADAPTER    pAd, 
    IN MLME_QUEUE_ELEM  *Elem);

VOID WpaEAPOLLogoffAction(
    IN PRTMP_ADAPTER    pAd, 
    IN MLME_QUEUE_ELEM  *Elem);

VOID WpaEAPOLKeyAction(
    IN PRTMP_ADAPTER    pAd, 
    IN MLME_QUEUE_ELEM  *Elem);

VOID HandleCounterMeasure(
    IN PRTMP_ADAPTER    pAd, 
    IN MAC_TABLE_ENTRY  *pEntry);

VOID PeerPairMsg2Action(
    IN PRTMP_ADAPTER    pAdapter, 
    IN MAC_TABLE_ENTRY  *pEntry,
    IN MLME_QUEUE_ELEM  *Elem);

VOID CMTimerExec(
    IN	unsigned long   data);

VOID EnqueueStartForPSKExec(
    IN	unsigned long   data);

VOID WPARetryExec(
    IN	unsigned long   data);

VOID Wpa1PeerPairMsg4Action(
    IN PRTMP_ADAPTER    pAdapter, 
    IN MAC_TABLE_ENTRY  *pEntry,
    IN MLME_QUEUE_ELEM  *Elem);

VOID Wpa2PeerPairMsg4Action(
    IN PRTMP_ADAPTER    pAdapter, 
    IN MAC_TABLE_ENTRY  *pEntry,
    IN MLME_QUEUE_ELEM  *Elem);

VOID PeerGroupMsg2Action(
    IN PRTMP_ADAPTER    pAdapter, 
    IN MAC_TABLE_ENTRY  *pEntry,
    IN VOID             *Msg,
    IN UINT             MsgLen);

NDIS_STATUS WPAHardTransmit(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  MAC_TABLE_ENTRY *pEntry);

VOID WpaEAPOLASFAlertAction(
    IN PRTMP_ADAPTER    pAd, 
    IN MLME_QUEUE_ELEM  *Elem);

VOID PairDisAssocAction(
    IN PRTMP_ADAPTER    pAd, 
    IN MAC_TABLE_ENTRY  *pEntry,
    IN USHORT           Reason);

VOID DisAssocAction(
    IN PRTMP_ADAPTER    pAd, 
    IN MAC_TABLE_ENTRY  *pEntry,
    IN USHORT           SubType,
    IN USHORT           Reason);

VOID GREKEYPeriodicExec(
    IN	unsigned long   data);

VOID WPAMake8023Hdr(
    IN PRTMP_ADAPTER    pAd,
    IN MAC_TABLE_ENTRY  *pEntry,
    IN OUT PCHAR        pHdr);

VOID PRF(
	IN UCHAR	*Key,
	IN INT		key_len,
	IN UCHAR	*Prefix,
	IN INT		Prefix_len,
	IN UCHAR	*Data,
	IN INT		Data_len,
	OUT UCHAR	*Output,
	IN INT      Output_len);

VOID CountPTK(
	IN UCHAR    *PMK,
	IN UCHAR    *ANonce,
	IN UCHAR    *AA,
	IN UCHAR    *SNonce,
	IN UCHAR    *SA,
	OUT UCHAR   *output,
	IN UINT     len);

VOID CountGTK(
	IN UCHAR    *PMK,
	IN UCHAR    *GNonce,
	IN UCHAR    *AA,
	OUT UCHAR   *output,
	IN UINT     len);

VOID GetSmall(
	IN PVOID    pSrc1,
	IN PVOID    pSrc2,
	OUT PUCHAR  out,
	IN ULONG    Length);

VOID GetLarge(
	IN PVOID    pSrc1,
	IN PVOID    pSrc2,
	OUT PUCHAR  out,
	IN ULONG    Length);

VOID GenRandom(
    IN PRTMP_ADAPTER    pAd,
    OUT UCHAR           *random,
    IN  UCHAR			apidx);

VOID AES_GTK_KEY_WRAP( 
    IN UCHAR    *key,
    IN UCHAR    *plaintext,
    IN UCHAR	p_len,
    OUT UCHAR   *ciphertext);

VOID AES_GTK_KEY_UNWRAP( 
    IN  UCHAR   *key,
    OUT UCHAR   *plaintext,
    IN UCHAR    c_len,
    IN  UCHAR   *ciphertext);	

//iversonadd
VOID RTMPToWirelessSta(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  PUCHAR          pFrame,
    IN  UINT            FrameLen,
    IN  BOOLEAN         DoEncrypt);

VOID    WpaSend(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  PUCHAR          pPacket,
    IN  ULONG           Len);

VOID	RTMPMakeRSNIE(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  UINT            AuthMode,
    IN  UINT            WepStatus,
    IN  UCHAR           apidx);

#ifdef APCLI_SUPPORT
VOID	RTMPApCliMakeRSNIE(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  UINT            AuthMode,
    IN  UINT            WepStatus,
    IN  UCHAR           apidx);
#endif

VOID RTMPAddPMKIDCache(
	IN  PRTMP_ADAPTER   		pAd,
	IN	INT						apidx,
	IN	PUCHAR				pAddr,
	IN	UCHAR					*PMKID,
	IN	UCHAR					*PMK);

INT RTMPSearchPMKIDCache(
	IN  PRTMP_ADAPTER   pAd,
	IN	INT				apidx,
	IN	PUCHAR		pAddr);

VOID RTMPDeletePMKIDCache(
	IN  PRTMP_ADAPTER   pAd,
	IN	INT				apidx,
	IN  INT				idx);

VOID RTMPMaintainPMKIDCache(
	IN  PRTMP_ADAPTER   pAd);

#endif

