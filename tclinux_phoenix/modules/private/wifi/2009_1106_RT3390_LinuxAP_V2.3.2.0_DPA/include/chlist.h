/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	chlist.c

	Abstract:

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
	Fonchi Wu   2007-12-19    created
*/

#ifndef __CHLIST_H__
#define __CHLIST_H__

#include "rtmp_type.h"
#include "rtmp_def.h"


#define ODOR			0
#define IDOR			1
#define BOTH			2

#define BAND_5G         0
#define BAND_24G        1
#define BAND_BOTH       2

#if defined(EXT_BUILD_CHANNEL_LIST) || defined(CONFIG_STA_SUPPORT)
//#ifdef EXT_BUILD_CHANNEL_LIST
typedef struct _CH_DESP {
	UCHAR FirstChannel;
	UCHAR NumOfCh;
	CHAR MaxTxPwr;			// dBm
	UCHAR Geography;			// 0:out door, 1:in door, 2:both
	BOOLEAN DfsReq;			// Dfs require, 0: No, 1: yes.
} CH_DESP, *PCH_DESP;

typedef struct _CH_REGION {
	UCHAR CountReg[3];
	UCHAR DfsType;			// 0: CE, 1: FCC, 2: JAP, 3:JAP_W53, JAP_W56
	CH_DESP ChDesp[10];
} CH_REGION, *PCH_REGION;

extern CH_REGION ChRegion[];

typedef struct _CH_FREQ_MAP_{
	UINT16		channel;
	UINT16		freqKHz;
}CH_FREQ_MAP;

extern CH_FREQ_MAP CH_HZ_ID_MAP[];
extern int CH_HZ_ID_MAP_NUM;
#endif // defined(EXT_BUILD_CHANNEL_LIST) || defined(CONFIG_STA_SUPPORT) //


#ifdef EXT_BUILD_CHANNEL_LIST
VOID BuildChannelListEx(
	IN PRTMP_ADAPTER pAd);

VOID BuildBeaconChList(
	IN PRTMP_ADAPTER pAd,
	OUT PUCHAR pBuf,
	OUT	PULONG pBufLen);
#endif // EXT_BUILD_CHANNEL_LIST //

#ifdef DOT11_N_SUPPORT
VOID N_ChannelCheck(
	IN PRTMP_ADAPTER pAd);

#endif // DOT11_N_SUPPORT //

UINT8 GetCuntryMaxTxPwr(
	IN PRTMP_ADAPTER pAd,
	IN UINT8 channel);
	
#endif // __CHLIST_H__

