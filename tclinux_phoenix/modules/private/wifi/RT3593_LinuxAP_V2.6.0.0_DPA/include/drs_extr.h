/****************************************************************************
 * Ralink Tech Inc.
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2010, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************/

/****************************************************************************

	Abstract:

	All Dynamic Rate Switch Related Structure & Definition

***************************************************************************/

#ifndef __DRS_EXTR_H__
#define __DRS_EXTR_H__


/* group rate switch */
#ifdef NEW_RATE_ADAPT_SUPPORT

extern UCHAR RateSwitchTable11N3S_Group[];

/* MACRO */
#define RTMP_DRS_GROUP_ENABLE(__pAd, __pEntry)								\
{																			\
	MlmeSetMcsGroup((__pAd), (__pEntry));									\
	(__pEntry)->lastRateIdx = 1;											\
	(__pEntry)->fewPktsCnt = 0;												\
	(__pEntry)->perThrdAdj = PER_THRD_ADJ;									\
	(__pEntry)->useNewRateAdapt = 1;										\
}

#define RTMP_DRS_GROUP_IS_USED(__pAd, __pEntry)								\
	(((__pAd)->chipCap.DrsMethod == RTMP_CHIP_DRS_GROUP) &&					\
	((__pEntry)->useNewRateAdapt == 1))

#define RTMP_DRS_GROUP_TABLE_3S_SELECT(__ppTable, __pTableSize, __pInitTxRateIdx)	\
{																			\
	*(__ppTable) = RateSwitchTable11N3S_Group;								\
	*(__pTableSize) = RateSwitchTable11N3S_Group[0];						\
	*(__pInitTxRateIdx) = RateSwitchTable11N3S_Group[1];					\
}

#define RTMP_DRS_GROUP_TABLE_2S_SELECT(__ppTable, __pTableSize, __pInitTxRateIdx)	\
{																			\
	*(__ppTable) = RateSwitchTable11N3S_Group;								\
	*(__pTableSize) = RateSwitchTable11N3S_Group[0];						\
	*(__pInitTxRateIdx) = 0x0f;												\
}

#define RTMP_DRS_GROUP_TABLE_1S_SELECT(__ppTable, __pTableSize, __pInitTxRateIdx)	\
{																			\
	*(__ppTable) = RateSwitchTable11N3S_Group;								\
	*(__pTableSize) = RateSwitchTable11N3S_Group[0];						\
	*(__pInitTxRateIdx) = 0x07;												\
}

#define RTMP_DRS_GROUP_ALG_IS_SELECTED(__pTable)							\
	((__pTable) == RateSwitchTable11N3S_Group)

#define RTMP_DRS_GROUP_TX_RATE_INIT(__pEntry, __pTable, __pTxRate)			\
	(__pTxRate) = (PRTMP_TX_RATE_SWITCH) &(__pTable)[((__pEntry)->CurrTxRateIndex+1)*10];

#define PER_THRD_ADJ			1
#define FEW_PKTS_CNT_THRD		1

/* FUNCTION */
#ifdef CONFIG_AP_SUPPORT
VOID APMlmeDynamicTxRateSwitchingAdapt(
    IN PRTMP_ADAPTER			pAd,
    IN ULONG					idx);

VOID APQuickResponeForRateUpExecAdapt(
    IN PRTMP_ADAPTER			pAd,
    IN ULONG					idx);
#endif /* CONFIG_AP_SUPPORT */


VOID MlmeSetMcsGroup(
	IN PRTMP_ADAPTER 			pAd,
	OUT PMAC_TABLE_ENTRY		pEntry);

INT	Set_UseNewRateAdapt_Proc(
	IN	PRTMP_ADAPTER			pAd, 
	IN	PSTRING					arg);

INT	Set_PerThrdAdj_Proc(
	IN	PRTMP_ADAPTER			pAd,
	IN	PSTRING					arg);

VOID MlmeDynamicTxRateSwitchingAdapt(
	IN PRTMP_ADAPTER pAd,
	IN ULONG i,
	IN ULONG TxSuccess,
	IN ULONG TxRetransmit,
	IN ULONG TxFailCount);

#else

#define RTMP_DRS_GROUP_ENABLE(__pAd, __pEntry)

#endif /* NEW_RATE_ADAPT_SUPPORT */


/* normal rate switch */
#define RTMP_DRS_ALG_INIT(__pAd, __Alg)										\
	(__pAd)->chipCap.DrsMethod = __Alg;

#define RTMP_DRS_ENTRY_INIT(__pAd, __pEntry)								\
	if ((__pAd)->chipCap.DrsMethod == RTMP_CHIP_DRS_GROUP)					\
	{																		\
		RTMP_DRS_GROUP_ENABLE(__pAd, __pEntry);								\
	}																		\
	else																	\
	{																		\
		/* default rate switch parameters */								\
		(__pEntry)->useNewRateAdapt = 0;									\
	}

#endif /* __DRS_EXTR_H__ */

/* End of drs_extr.h */
