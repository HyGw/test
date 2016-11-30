/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 5F., No.36 Taiyuan St., Jhubei City,
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2008, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	dot11i_wpa.h
	
	Revision History:
	Who 			When			What
	--------		----------		----------------------------------------------
	
*/

#ifndef	__DOT11I_WPA_H__
#define	__DOT11I_WPA_H__

#include "rtmp_type.h"

/* The length is the EAPoL-Key frame except key data field. 
   Please refer to 802.11i-2004 ,Figure 43u in p.78 */
#define MIN_LEN_OF_EAPOL_KEY_MSG	95	

/* The related length of the EAPOL Key frame */
#define LEN_KEY_DESC_NONCE			32
#define LEN_KEY_DESC_IV				16
#define LEN_KEY_DESC_RSC			8
#define LEN_KEY_DESC_ID				8
#define LEN_KEY_DESC_REPLAY			8
#define LEN_KEY_DESC_MIC			16

/* EAP Code Type */
#define EAP_CODE_REQUEST	1
#define EAP_CODE_RESPONSE	2
#define EAP_CODE_SUCCESS    3
#define EAP_CODE_FAILURE    4

/* EAPOL frame Protocol Version */
#define	EAPOL_VER					1
#define	EAPOL_VER2					2

/* EAPOL-KEY Descriptor Type */
#define	WPA1_KEY_DESC				0xfe
#define WPA2_KEY_DESC               0x02

/* Key Descriptor Version of Key Information */
#define	KEY_DESC_TKIP			1
#define	KEY_DESC_AES			2
#define KEY_DESC_EXT			3

#define IE_WPA					221
#define IE_RSN					48

//EAP Packet Type
#define	EAPPacket		0
#define	EAPOLStart		1
#define	EAPOLLogoff		2
#define	EAPOLKey		3
#define	EAPOLASFAlert	4
#define	EAPTtypeMax		5

#define PAIRWISEKEY					1
#define GROUPKEY					0

/* RSN IE Length definition */
#define MAX_LEN_OF_RSNIE         	255
#define MIN_LEN_OF_RSNIE         	18
#define MAX_LEN_GTK					32
#define MIN_LEN_GTK					5

#define LEN_PMK						32
#define LEN_PMKID					16
#define LEN_PMK_NAME				16

#define LEN_PTK_KCK					16
#define LEN_PTK_KEK					16
#define LEN_TK						16	// The length Temporal key.
#define LEN_TKIP_MIC				8	// The length of TX/RX Mic of TKIP
#define LEN_TK2						(2 * LEN_TKIP_MIC)
#define LEN_PTK						(LEN_PTK_KCK + LEN_PTK_KEK + LEN_TK + LEN_TK2)

#define LEN_TKIP_PTK				LEN_PTK
#define LEN_AES_PTK					(LEN_PTK_KCK + LEN_PTK_KEK + LEN_TK)
#define LEN_TKIP_GTK				(LEN_TK + LEN_TK2)
#define LEN_AES_GTK					LEN_TK
#define LEN_TKIP_TK					(LEN_TK + LEN_TK2)
#define LEN_AES_TK					LEN_TK

#define OFFSET_OF_PTK_TK			(LEN_PTK_KCK + LEN_PTK_KEK)	// The offset of the PTK Temporal key in PTK
#define OFFSET_OF_AP_TKIP_TX_MIC	(OFFSET_OF_PTK_TK + LEN_TK)
#define OFFSET_OF_AP_TKIP_RX_MIC	(OFFSET_OF_AP_TKIP_TX_MIC + LEN_TKIP_MIC)
#define OFFSET_OF_STA_TKIP_RX_MIC	(OFFSET_OF_PTK_TK + LEN_TK)
#define OFFSET_OF_STA_TKIP_TX_MIC	(OFFSET_OF_AP_TKIP_TX_MIC + LEN_TKIP_MIC)

#define LEN_NONCE					32
#define LEN_PN						6
#define LEN_TKIP_IV_HDR				8
#define LEN_CCMP_HDR				8
#define LEN_CCMP_MIC				8
#define LEN_OUI_SUITE				4
#define LEN_WEP_TSC					3
#define LEN_WPA_TSC					6
#define LEN_WEP_IV_HDR				4
#define LEN_ICV						4

// EAPOL Key Information definition within Key descriptor format
typedef	struct PACKED _KEY_INFO
{
#ifdef RT_BIG_ENDIAN
	UCHAR	KeyAck:1;
    UCHAR	Install:1;
    UCHAR	KeyIndex:2;
    UCHAR	KeyType:1;
    UCHAR	KeyDescVer:3;
    UCHAR	Rsvd:3;
    UCHAR	EKD_DL:1;		// EKD for AP; DL for STA
    UCHAR	Request:1;
    UCHAR	Error:1;
    UCHAR	Secure:1;
    UCHAR	KeyMic:1;
#else
	UCHAR	KeyMic:1;
	UCHAR	Secure:1;
	UCHAR	Error:1;
	UCHAR	Request:1;
	UCHAR	EKD_DL:1;       // EKD for AP; DL for STA
	UCHAR	Rsvd:3;
	UCHAR	KeyDescVer:3;
	UCHAR	KeyType:1;
	UCHAR	KeyIndex:2;
	UCHAR	Install:1;
	UCHAR	KeyAck:1;
#endif	
}	KEY_INFO, *PKEY_INFO;

// EAPOL Key descriptor format
typedef	struct PACKED _KEY_DESCRIPTER
{
	UCHAR		Type;
	KEY_INFO	KeyInfo;
	UCHAR		KeyLength[2];
	UCHAR		ReplayCounter[LEN_KEY_DESC_REPLAY];
	UCHAR		KeyNonce[LEN_KEY_DESC_NONCE];
	UCHAR		KeyIv[LEN_KEY_DESC_IV];
	UCHAR		KeyRsc[LEN_KEY_DESC_RSC];
	UCHAR		KeyId[LEN_KEY_DESC_ID];
	UCHAR		KeyMic[LEN_KEY_DESC_MIC];
	UCHAR		KeyDataLen[2];	   
	UCHAR		KeyData[0];
}	KEY_DESCRIPTER, *PKEY_DESCRIPTER;

typedef	struct PACKED _EAPOL_PACKET
{
	UCHAR	 			ProVer;
	UCHAR	 			ProType;
	UCHAR	 			Body_Len[2];
	KEY_DESCRIPTER		KeyDesc;
}	EAPOL_PACKET, *PEAPOL_PACKET;

//802.11i D10 page 83
typedef struct PACKED _GTK_ENCAP
{
#ifndef RT_BIG_ENDIAN
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
    UCHAR               GTK[MAX_LEN_GTK];
}   GTK_ENCAP, *PGTK_ENCAP;

typedef struct PACKED _KDE_ENCAP
{
    UCHAR               Type;
    UCHAR               Len;
    UCHAR               OUI[3];
    UCHAR               DataType;
    GTK_ENCAP      		GTKEncap;
}   KDE_ENCAP, *PKDE_ENCAP;

// For WPA1
typedef struct PACKED _RSNIE {
    UCHAR   oui[4];
    USHORT  version;
    UCHAR   mcast[4];
    USHORT  ucount;
    struct PACKED {
        UCHAR oui[4];
    }ucast[1];
} RSNIE, *PRSNIE;

// For WPA2
typedef struct PACKED _RSNIE2 {
    USHORT  version;
    UCHAR   mcast[4];
    USHORT  ucount;
    struct PACKED {
        UCHAR oui[4];
    }ucast[1];
} RSNIE2, *PRSNIE2;

// AKM Suite
typedef struct PACKED _RSNIE_AUTH {
    USHORT acount;
    struct PACKED {
        UCHAR oui[4];
    }auth[1];
} RSNIE_AUTH,*PRSNIE_AUTH;

// PMKID List
typedef struct PACKED _RSNIE_PMKID {
    USHORT pcount;
    struct PACKED {
        UCHAR list[16];
    }pmkid[1];
} RSNIE_PMKID,*PRSNIE_PMKID;

typedef	union PACKED _RSN_CAPABILITIES	{
	struct	PACKED {
#ifdef RT_BIG_ENDIAN
        USHORT		Rsvd:8;		
		USHORT		MFPC:1;
		USHORT		MFPR:1;
        USHORT		GTKSA_R_Counter:2;
        USHORT		PTKSA_R_Counter:2;
        USHORT		No_Pairwise:1;
		USHORT		PreAuth:1;
#else
        USHORT		PreAuth:1;
		USHORT		No_Pairwise:1;
		USHORT		PTKSA_R_Counter:2;
		USHORT		GTKSA_R_Counter:2;
		USHORT		MFPR:1;
		USHORT		MFPC:1;
		USHORT		Rsvd:8;
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


#endif // __DOT11I_WPA_H__ //

