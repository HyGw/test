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

	All related Dynamic Rate Switch (AP/STA) function body.

	History:

***************************************************************************/

#ifdef NEW_RATE_ADAPT_SUPPORT

#include "rt_config.h"




/*
	3x3 rate switch table for new rate adaption
	(default: for good siganl environment (RSSI > -65))
	Target: Good throughput
*/
UCHAR RateSwitchTable11N3S_Group[] = {
	/* item no.     mcs   highPERThrd  upMcs3   upMcs1     Mode- Bit0: STBC, Bit1: Short GI, Bit4,5: Mode(0:CCK, 1:OFDM, 2:HT Mix, 3:HT GF)*/
	/*        mode    lowPERThrd  downMcs   upMcs2*/
    0x19, 0x18,  0,  0,   0,   0,   0,   0,   0,   0,/* Initial used item after association: the number of rate indexes, the initial mcs*/
    0x00, 0x21,  0, 30, 101,   0,  16,   8,   1,   7,/*mcs0*/
    0x01, 0x21,  1, 20,  50,   0,  16,   9,   2,  13,/*mcs1*/
    0x02, 0x21,  2, 20,  50,   1,  17,   9,   3,  20,/*mcs2*/
    0x03, 0x21,  3, 15,  50,   2,  17,  10,   4,  26,/*mcs3*/
    0x04, 0x21,  4, 15,  30,   3,  18,  11,   5,  39,/*mcs4*/
    0x05, 0x21,  5, 10,  25,   4,  18,  12,   6,  52,/*mcs5*/
    0x06, 0x21,  6,  8,  14,   5,  19,  12,   7,  59,/*mcs6*/
    0x07, 0x21,  7,  8,  14,   6,  19,  12,   7,  65,/*mcs7*/
    0x08, 0x20,  8, 30,  50,   0,  16,   9,   2,  13,/*mcs8*/
    0x09, 0x20,  9, 20,  50,   8,  17,  10,   4,  26,/*mcs9*/
    0x0a, 0x20, 10, 20,  50,   9,  18,  11,   5,  39,/*mcs10*/
    0x0b, 0x20, 11, 15,  30,  10,  18,  12,   6,  52,/*mcs11*/
    0x0c, 0x20, 12, 15,  30,  11,  20,  13,  12,  78,/*mcs12*/
    0x0d, 0x20, 13,  8,  20,  12,  20,  14,  13, 104,/*mcs13*/
    0x0e, 0x20, 14,  8,  18,  13,  21,  15,  14, 117,/*mcs14*/
    0x0f, 0x20, 15,  8,  14,  14,  21,  15,  15, 130,/*mcs15*/
    0x10, 0x20, 16, 30,  50,   8,  17,   9,   3,  20,/*mcs16*/
    0x11, 0x20, 17, 20,  50,  16,  18,  11,   5,  39,/*mcs17*/
    0x12, 0x20, 18, 20,  50,  17,  19,  12,   7,  59,/*mcs18*/
    0x13, 0x20, 19, 15,  30,  18,  20,  13,  19,  78,/*mcs19*/
    0x14, 0x20, 20, 15,  30,  19,  21,  15,  20, 117,/*mcs20*/
    0x15, 0x20, 21,  8,  20,  20,  22,  21,  21, 156,/*mcs21*/
    0x16, 0x20, 22,  8,  20,  21,  23,  22,  22, 176,/*mcs22*/
    0x17, 0x20, 23,  6,  18,  22,  24,  23,  23, 196,/*mcs23*/
    0x18, 0x22, 23,  6,  14,  23,  24,  24,  24, 217,/*mcs23+shortGI*/
		};


/* 
	MlmeSetMcsGroup - set initial mcsGroup based on supported MCSs
		On exit pEntry->mcsGroup is set to the mcsGroup
*/
VOID MlmeSetMcsGroup(
	IN PRTMP_ADAPTER 		pAd,
	OUT PMAC_TABLE_ENTRY	pEntry)
{
#ifdef DOT11N_SS3_SUPPORT
	if ((pEntry->HTCapability.MCSSet[2] == 0xff) && (pAd->CommonCfg.TxStream == 3))
		pEntry->mcsGroup = 3;
	 else
#endif /* DOT11N_SS3_SUPPORT */
	 if ((pEntry->HTCapability.MCSSet[0] == 0xff) &&
		(pEntry->HTCapability.MCSSet[1] == 0xff) &&
		(pAd->CommonCfg.TxStream > 1) &&
		((pAd->CommonCfg.TxStream == 2) || (pEntry->HTCapability.MCSSet[2] == 0x0)))
		pEntry->mcsGroup = 2;
	else
		pEntry->mcsGroup = 1;
}


#ifdef CONFIG_AP_SUPPORT
/*
    ==========================================================================
    Description:
        This routine walks through the MAC table, see if TX rate change is 
        required for each associated client. 
    Output:
        pEntry->CurrTxRate - 
    NOTE:
        call this routine every second
    ==========================================================================
 */
VOID APMlmeDynamicTxRateSwitchingAdapt(
    IN PRTMP_ADAPTER pAd, IN ULONG i)
{
	PUCHAR					pTable;
	UCHAR					TableSize = 0;
	UCHAR					UpRateIdx, DownRateIdx, CurrRateIdx;
	ULONG					AccuTxTotalCnt, TxTotalCnt;
	ULONG					TxErrorRatio = 0;
#ifdef FIFO_EXT_SUPPORT
	ULONG 					HwTxCnt, HwErrRatio;
#endif /* FIFO_EXT_SUPPORT */
	MAC_TABLE_ENTRY			*pEntry;
	PRTMP_TX_RATE_SWITCH_3S	pCurrTxRate, pNextTxRateIdx;
	PRTMP_TX_RATE_SWITCH	pNextTxRate = NULL;
	BOOLEAN					bTxRateChanged = TRUE, bUpgradeQuality = FALSE;
	UCHAR					InitTxRateIdx, TrainUp, TrainDown;
	TX_STA_CNT1_STRUC		StaTx1;
	TX_STA_CNT0_STRUC		TxStaCnt0;
	CHAR					Rssi, RssiOffset = 0;
	ULONG					TxRetransmit = 0, TxSuccess = 0, TxFailCount = 0;
	ULONG					phyRateLimit20 = 0;
#ifdef DBG
	BOOLEAN					bDisplayEntryDebug;
#endif /* DBG */
	UCHAR					tmpTxRate = 0;
	PRTMP_TX_RATE_SWITCH_3S	pTempTxRate = NULL;

#ifdef RALINK_ATE
   	if (ATE_ON(pAd))
   	{
		return;
   	}
#endif /* RALINK_ATE */
    
	/*
		walk through MAC table, see if need to change AP's TX rate toward each entry
	*/
    
	/* point to information of the individual station */
	pEntry = &pAd->MacTab.Content[i];

	APMlmeSelectTxRateTable(pAd, pEntry, &pTable, &TableSize, &InitTxRateIdx);

	if (pAd->MacTab.Size == 1)
	{

		/* Update statistic counter
			because RESET_ONE_SEC_TX_CNT() was executed when RateSwitching() was 
			called last time, OneSecTx.... are all zero.
		*/
		NicGetTxRawCounters(pAd, &TxStaCnt0, &StaTx1);

		TxRetransmit = StaTx1.field.TxRetransmit;
		TxSuccess = StaTx1.field.TxSuccess;
		TxFailCount = TxStaCnt0.field.TxFailCount;
		TxTotalCnt = TxRetransmit + TxSuccess + TxFailCount;

		AccuTxTotalCnt = pAd->RalinkCounters.OneSecTxNoRetryOkCount + 
					 pAd->RalinkCounters.OneSecTxRetryOkCount + 
					 pAd->RalinkCounters.OneSecTxFailCount;

		if (TxTotalCnt)
			TxErrorRatio = ((TxRetransmit + TxFailCount) * 100) / TxTotalCnt;			
	}
	else
	{
		TxTotalCnt = pEntry->OneSecTxNoRetryOkCount + 
			 pEntry->OneSecTxRetryOkCount + 
			 pEntry->OneSecTxFailCount;

		if (TxTotalCnt)
			TxErrorRatio = ((pEntry->OneSecTxRetryOkCount + pEntry->OneSecTxFailCount) * 100) / TxTotalCnt;

#ifdef FIFO_EXT_SUPPORT
		if (pAd->chipCap.FlgHwFifoExtCap)
		{
			if ((pEntry->Aid >= 1) && (pEntry->Aid <= 8))
			{
				NicGetMacFifoTxCnt(pAd, pEntry);
				HwTxCnt = pEntry->fifoTxSucCnt + pEntry->fifoTxRtyCnt;
				if (HwTxCnt)
					HwErrRatio = (pEntry->fifoTxRtyCnt * 100) / HwTxCnt;
				else
					HwErrRatio = 0;
				DBGPRINT(RT_DEBUG_TRACE | DBG_FUNC_RA,("%s():Aid:%d, MCS:%d, TxErrRatio(Hw:0x%lx-0x%lx, Sw:0x%lx-%lx)\n", 
						__FUNCTION__, pEntry->Aid, pEntry->HTPhyMode.field.MCS, 
						HwTxCnt, HwErrRatio, TxTotalCnt, TxErrorRatio));

				TxTotalCnt = HwTxCnt;
				TxErrorRatio = HwErrRatio;
			}
		}
#endif /* FIFO_EXT_SUPPORT */
	}


#ifdef DBG
	bDisplayEntryDebug = ((pAd->MacTab.Size==1) || 
						(pEntry->OneSecTxFailCount!=0) || 
						(pEntry->OneSecTxNoRetryOkCount!=0));
#endif /* DBG */
		
	CurrRateIdx = UpRateIdx = DownRateIdx = pEntry->CurrTxRateIndex;

	/*different calculation in APQuickResponeForRateUpExec() */
	/*Rssi = RTMPMaxRssi(pAd, (CHAR)pEntry->RssiSample.AvgRssi0, (CHAR)pEntry->RssiSample.AvgRssi1, (CHAR)pEntry->RssiSample.AvgRssi2); */
	Rssi = RTMPAvgRssi(pAd, &pEntry->RssiSample);

	/* decide the next upgrade rate and downgrade rate, if any */
	pCurrTxRate = (PRTMP_TX_RATE_SWITCH_3S) &pTable[(CurrRateIdx+1)*10]; 

	do 
	{
		if ((pEntry->HTCapability.MCSSet[2] == 0xff) && (pAd->CommonCfg.TxStream == 3))
		{
				switch (pEntry->mcsGroup)
			{
				case 0:/*improvement: use round robin mcs when group == 0 */
					UpRateIdx = pCurrTxRate->upMcs3;
					if ((pEntry->TxQuality[UpRateIdx] > pEntry->TxQuality[pCurrTxRate->upMcs2]) &&
						(pCurrTxRate->upMcs2 != pCurrTxRate->ItemNo))
							UpRateIdx = pCurrTxRate->upMcs2;
					if ((pEntry->TxQuality[UpRateIdx] > pEntry->TxQuality[pCurrTxRate->upMcs1]) &&
						(pCurrTxRate->upMcs1 != pCurrTxRate->ItemNo))
							UpRateIdx = pCurrTxRate->upMcs1;
						break;
				case 3:
					UpRateIdx = pCurrTxRate->upMcs3;
					break;
				case 2:
					UpRateIdx = pCurrTxRate->upMcs2;
					break;
				case 1:
					UpRateIdx = pCurrTxRate->upMcs1;
					break;
				default:
						DBGPRINT_RAW(RT_DEBUG_ERROR, ("wrong mcsGroup value\n"));
					break;
			}

#ifdef DBG
			if (bDisplayEntryDebug)
			{
				DBGPRINT_RAW(RT_DEBUG_INFO | DBG_FUNC_RA,("DRS: Before- mcsGroup=%d, TxQuality2[%d]=%d,  TxQuality1[%d]=%d, TxQuality0[%d]=%d\n",
						pEntry->mcsGroup,
					pCurrTxRate->upMcs3,
					pEntry->TxQuality[pCurrTxRate->upMcs3],
					pCurrTxRate->upMcs2,
					pEntry->TxQuality[pCurrTxRate->upMcs2],
					pCurrTxRate->upMcs1,				
					pEntry->TxQuality[pCurrTxRate->upMcs1]));
			}
#endif /* DBG */
		} 
		else if ((pEntry->HTCapability.MCSSet[0] == 0xff) &&
					(pEntry->HTCapability.MCSSet[1] == 0xff) &&
					(pAd->CommonCfg.TxStream > 1) && 
					((pAd->CommonCfg.TxStream == 2) || (pEntry->HTCapability.MCSSet[2] == 0x0)))
		{
				switch (pEntry->mcsGroup)
			{
				case 0:
					UpRateIdx = pCurrTxRate->upMcs2;
						if ((pEntry->TxQuality[UpRateIdx] > pEntry->TxQuality[pCurrTxRate->upMcs1]) &&
							(pCurrTxRate->upMcs1 != pCurrTxRate->ItemNo))
							UpRateIdx = pCurrTxRate->upMcs1;
						break;
				case 2:
					UpRateIdx = pCurrTxRate->upMcs2;
					break;
				case 1:
					UpRateIdx = pCurrTxRate->upMcs1;
					break;
				default:
						DBGPRINT_RAW(RT_DEBUG_ERROR, ("wrong mcsGroup value %d\n", pEntry->mcsGroup));
					break;
			}	
#ifdef DBG
			if (bDisplayEntryDebug) 
			{
				DBGPRINT_RAW(RT_DEBUG_INFO | DBG_FUNC_RA,("DRS: Before- mcsGroup=%d,  TxQuality1[%d]=%d, TxQuality0[%d]=%d\n",
						pEntry->mcsGroup,
						pCurrTxRate->upMcs2,
						pEntry->TxQuality[pCurrTxRate->upMcs2],
						pCurrTxRate->upMcs1,				
						pEntry->TxQuality[pCurrTxRate->upMcs1]));
			}
#endif /* DBG */
		} 
		else 
		{
				switch (pEntry->mcsGroup)
			{
				case 1:
				case 0:
					UpRateIdx = pCurrTxRate->upMcs1;
					break;
				default:
						DBGPRINT_RAW(RT_DEBUG_ERROR, ("wrong mcsGroup value %d\n", pEntry->mcsGroup));
					break;
			}
		}

		if (UpRateIdx == pEntry->CurrTxRateIndex)
		{
				pEntry->mcsGroup = 0;
			break;
		}

		if (pEntry->TxQuality[UpRateIdx] > 0 && pEntry->mcsGroup > 0)
			pEntry->mcsGroup --;
		else
			break;/*break the while loop */
	} while (1);

	DownRateIdx = pCurrTxRate->downMcs;
/*UpRateIdx = DownRateIdx = 13; */

#ifdef DOT11_N_SUPPORT
	if ((Rssi > -65) && (pCurrTxRate->Mode >= MODE_HTMIX) && pEntry->perThrdAdj == 1)
	{
		TrainUp		= (pCurrTxRate->TrainUp + (pCurrTxRate->TrainUp >> 1));
		TrainDown	= (pCurrTxRate->TrainDown + (pCurrTxRate->TrainDown >> 1));
	}
	else
#endif /* DOT11_N_SUPPORT */
	{
		TrainUp		= pCurrTxRate->TrainUp;
		TrainDown	= pCurrTxRate->TrainDown;
	}

	/* keep record whether mcs action of last time, used in APQuickResponeForRateUpExec() */
	pEntry->LastTimeTxRateChangeAction = pEntry->LastSecTxRateChangeAction;

	if (pAd->MacTab.Size == 1)
	{
			DBGPRINT_RAW(RT_DEBUG_INFO | DBG_FUNC_RA,("DRS:Aid=%d, TxRetransmit=%ld, TxFailCount=%ld, TxSuccess=%ld, rssi0=%d, rssi1=%d, rssi2=%d \n",
			pEntry->Aid, TxRetransmit, TxFailCount,	TxSuccess, 
			(CHAR)pEntry->RssiSample.AvgRssi0, (CHAR)pEntry->RssiSample.AvgRssi1, (CHAR)pEntry->RssiSample.AvgRssi2));
	}
#ifdef DBG
	else
	{
			DBGPRINT_RAW(RT_DEBUG_INFO | DBG_FUNC_RA,("DRS:Aid=%d, OneSecTxRetryOkCount=%d, OneSecTxFailCount=%d, OneSecTxNoRetryOkCount=%d \n",
				pEntry->Aid,
				pEntry->OneSecTxRetryOkCount,
				pEntry->OneSecTxFailCount,
				pEntry->OneSecTxNoRetryOkCount));
	}
#endif /* DBG */

/*
	if (pAd->CommonCfg.bAutoTxRateSwitch == FALSE)
	{
		DBGPRINT_RAW(RT_DEBUG_INFO,("DRS: Fixed - CurrTxRateIdx=%d, MCS=%d, STBC=%d, ShortGI=%d, Mode=%d, BW=%d, PER=%ld%% \n\n",
			pEntry->CurrTxRateIndex, pEntry->HTPhyMode.field.MCS, pEntry->HTPhyMode.field.STBC,
			pEntry->HTPhyMode.field.ShortGI, pEntry->HTPhyMode.field.MODE, pEntry->HTPhyMode.field.BW, TxErrorRatio));
		return;
	}
	else
 */		
	{
			DBGPRINT_RAW(RT_DEBUG_INFO | DBG_FUNC_RA,("DRS: Before- CurrRateIdx=%d, MCS=%d, STBC=%d, SGI=%d, Mode=%d, TrainUp/Down=%d/%d%%, NextUp/Down=%d/%d, CurrMCS=%d, TxBF=%c, mcsGroup=%d, TxQuality=%d, PER=%ld%%\n",
				CurrRateIdx,
				pCurrTxRate->CurrMCS,
				pCurrTxRate->STBC,
				pCurrTxRate->ShortGI,
				pCurrTxRate->Mode,
				TrainUp, TrainDown,
				UpRateIdx, DownRateIdx,
				pEntry->HTPhyMode.field.MCS,
				pEntry->HTPhyMode.field.eTxBF? '2': (pEntry->HTPhyMode.field.iTxBF? '1': '0'), 
				pEntry->mcsGroup,
				pEntry->TxQuality[UpRateIdx],
				TxErrorRatio));
	}

#ifdef MFB_SUPPORT
	if (pEntry->fLastChangeAccordingMfb == TRUE)
	{
			/*with this method mfb result can be applied every 500msec, instead of immediately */
			NdisAcquireSpinLock(&pEntry->fLastChangeAccordingMfbLock);
		pEntry->fLastChangeAccordingMfb = FALSE;
		pEntry->LastSecTxRateChangeAction = RATE_NO_CHANGE;
			NdisReleaseSpinLock(&pEntry->fLastChangeAccordingMfbLock);
			APMlmeSetTxRate(pAd, pEntry, pEntry->LegalMfbRS);
		DBGPRINT_RAW(RT_DEBUG_TRACE,("DRS: MCS is according to MFB, and ignore tuning this sec \n"));

		/* Clear all history, same as train up, purpose??? */
		NdisZeroMemory(pEntry->TxQuality, sizeof(USHORT) * MAX_STEP_OF_TX_RATE_SWITCH);
		/* NdisZeroMemory(pEntry->PER, sizeof(UCHAR) * MAX_STEP_OF_TX_RATE_SWITCH);//not used */
		/* reset all OneSecTx counters */
		RESET_ONE_SEC_TX_CNT(pEntry);

			pEntry->CurrTxRateIndex = (pEntry->LegalMfbRS)->ItemNo;
			pNextTxRate = (PRTMP_TX_RATE_SWITCH) &pTable[(pEntry->CurrTxRateIndex+1)*10];/*actually = pEntry->LegalMfbRS */
			return;
	}
#endif	/* MFB_SUPPORT */


	if (TxTotalCnt <= 15)
	{
   		CHAR	idx = 0;
		UCHAR	TxRateIdx;
		UCHAR	MCS0 = 0, MCS1 = 0, MCS2 = 0, MCS3 = 0, MCS4 = 0,  MCS5 =0, MCS6 = 0, MCS7 = 0;
		UCHAR	MCS8 = 0, MCS9 = 0, MCS10 = 0, MCS11 = 0, MCS12 = 0,  MCS13 =0, MCS14 = 0, MCS15 = 0;
		UCHAR	MCS16 = 0, MCS17 = 0, MCS18 = 0, MCS19 = 0, MCS20 = 0,  MCS21 =0, MCS22 = 0, MCS23 = 0;

		/* check the existence and index of each needed MCS */
		while (idx < pTable[0])
		{
			pCurrTxRate = (PRTMP_TX_RATE_SWITCH_3S) &pTable[(idx+1)*10];

			if (pCurrTxRate->CurrMCS == MCS_0)
			{
				MCS0 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_1)
			{
				MCS1 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_2)
			{
				MCS2 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_3)
			{
				MCS3 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_4)
			{
				MCS4 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_5)
			{
				MCS5 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_6)
			{
				MCS6 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_7)
			{
				MCS7 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_8)
			{
				MCS8 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_9)
			{
				MCS9 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_10)
			{
				MCS10 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_11)
			{
				MCS11 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_12)
			{
				MCS12 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_13)
			{
				MCS13 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_14)
			{
				MCS14 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_15)
			{
				MCS15 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_16)
			{
				MCS16 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_17)
			{
				MCS17 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_18)
			{
				MCS18 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_19)
			{
				MCS19 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_20)
			{
				MCS20 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_21)
			{
				MCS21 = idx;
			}
			else if (pCurrTxRate->CurrMCS == MCS_22)
			{
				MCS22 = idx;
			}
			else if ((pCurrTxRate->CurrMCS == MCS_23) && (pCurrTxRate->ShortGI == GI_400))	/*we hope to use ShortGI as initial rate */
			{
				MCS23 = idx;
			}
	
			idx ++;
		}
		if ((pTable == RateSwitchTable11BGN2S) || (pTable == RateSwitchTable11BGN2SForABand) || (pTable == RateSwitchTable11N2S) || (pTable == RateSwitchTable11N2SForABand) || (pTable == RateSwitchTable))
		{
			RssiOffset = 2;
		}
			/*ask Gary and Rory why RssiOffset is needed!!! */
		else if (pTable == RateSwitchTable11N3S_Group)
		{
			RssiOffset = 0;
		}
		else
		{
			RssiOffset = 5;
		}


			/* Debug Option: If PHY limit disable all PHY > 85. Also check for 65 and 40 Mbps */
			if (phyRateLimit20 != 0)
			{
				MCS13 = MCS14 = MCS15 = MCS20 = MCS21 = MCS22 = MCS23 = 0;
				if (phyRateLimit20 <= 65)
					MCS7 = MCS12 = MCS19 = 0;

				if (phyRateLimit20 <= 40)
					MCS5 = MCS6 = MCS11 = MCS18 = 0;

				if (phyRateLimit20 <= 20)
					MCS3 = MCS4 = MCS9 = MCS10 = MCS17 = 0;
			}

		if ((pTable == RateSwitchTable11BGN2S) || (pTable == RateSwitchTable11N2S) || (pTable == RateSwitchTable))
		{/* N mode with 2 stream */
			if (MCS15 && (Rssi >= (-70+RssiOffset)))
				TxRateIdx = MCS15;
			else if (MCS14 && (Rssi >= (-72+RssiOffset)))
				TxRateIdx = MCS14;
			else if (MCS13 && (Rssi >= (-76+RssiOffset)))
				TxRateIdx = MCS13;
			else if (MCS12 && (Rssi >= (-78+RssiOffset)))
				TxRateIdx = MCS12;
			else if (MCS4 && (Rssi >= (-82+RssiOffset)))
				TxRateIdx = MCS4;
			else if (MCS3 && (Rssi >= (-84+RssiOffset)))
				TxRateIdx = MCS3;
			else if (MCS2 && (Rssi >= (-86+RssiOffset)))
				TxRateIdx = MCS2;
			else if (MCS1 && (Rssi >= (-88+RssiOffset)))
				TxRateIdx = MCS1;
			else
				TxRateIdx = MCS0;
		}
		else if ((pTable == RateSwitchTable11BGN1S) || (pTable == RateSwitchTable11N1S))
		{/* N mode with 1 stream */
			if (MCS7 && (Rssi > (-72+RssiOffset)))
				TxRateIdx = MCS7;
			else if (MCS6 && (Rssi > (-74+RssiOffset)))
				TxRateIdx = MCS6;
			else if (MCS5 && (Rssi > (-77+RssiOffset)))
				TxRateIdx = MCS5;
			else if (MCS4 && (Rssi > (-79+RssiOffset)))
				TxRateIdx = MCS4;
			else if (MCS3 && (Rssi > (-81+RssiOffset)))
				TxRateIdx = MCS3;
			else if (MCS2 && (Rssi > (-83+RssiOffset)))
				TxRateIdx = MCS2;
			else if (MCS1 && (Rssi > (-86+RssiOffset)))
				TxRateIdx = MCS1;
			else
				TxRateIdx = MCS0;
		}
		else if ((pTable == RateSwitchTable11BGN3S) || (pTable == RateSwitchTable11N3S_Group) || (pTable == RateSwitchTable11BGN3SForABand))
		{
			/* N mode with 3 stream */
			if ((pEntry->HTCapability.MCSSet[2] == 0xff) && (pAd->CommonCfg.TxStream == 3))
			{
				if (MCS23 && (Rssi > (-72+RssiOffset)))
					TxRateIdx = MCS23;
				else if (MCS22 && (Rssi > (-74+RssiOffset)))
					TxRateIdx = MCS22;
				else if (MCS21 && (Rssi > (-77+RssiOffset)))
					TxRateIdx = MCS21;
				else if (MCS20 && (Rssi > (-79+RssiOffset)))
					TxRateIdx = MCS20;
				else if (MCS19 && (Rssi > (-81+RssiOffset)))
					TxRateIdx = MCS19;
				else if (MCS18 && (Rssi > (-83+RssiOffset)))
					TxRateIdx = MCS18;
				else if (MCS17 && (Rssi > (-86+RssiOffset)))
					TxRateIdx = MCS17;
				else
					TxRateIdx = MCS16;
				pEntry->mcsGroup = 3;
			}
			else if ((pEntry->HTCapability.MCSSet[0] == 0xff) &&
						(pEntry->HTCapability.MCSSet[1] == 0xff) &&
						(pAd->CommonCfg.TxStream > 1) &&
						((pAd->CommonCfg.TxStream == 2) || (pEntry->HTCapability.MCSSet[2] == 0x0)))
			{
				if (MCS15 && (Rssi > (-72+RssiOffset)))
					TxRateIdx = MCS15;
				else if (MCS14 && (Rssi > (-74+RssiOffset)))
					TxRateIdx = MCS14;
				else if (MCS13 && (Rssi > (-77+RssiOffset)))
					TxRateIdx = MCS13;
				else if (MCS12 && (Rssi > (-79+RssiOffset)))
					TxRateIdx = MCS12;
				else if (MCS11 && (Rssi > (-81+RssiOffset)))
					TxRateIdx = MCS11;
				else if (MCS10 && (Rssi > (-83+RssiOffset)))
					TxRateIdx = MCS10;
				else if (MCS9 && (Rssi > (-86+RssiOffset)))
					TxRateIdx = MCS9;
				else
					TxRateIdx = MCS8;
					pEntry->mcsGroup = 2;
					
		 	}
			else 
			{
				if (MCS7 && (Rssi > (-72+RssiOffset)))
					TxRateIdx = MCS7;
				else if (MCS6 && (Rssi > (-74+RssiOffset)))
					TxRateIdx = MCS6;
				else if (MCS5 && (Rssi > (-77+RssiOffset)))
					TxRateIdx = MCS5;
				else if (MCS4 && (Rssi > (-79+RssiOffset)))
					TxRateIdx = MCS4;
				else if (MCS3 && (Rssi > (-81+RssiOffset)))
					TxRateIdx = MCS3;
				else if (MCS2 && (Rssi > (-83+RssiOffset)))
					TxRateIdx = MCS2;
				else if (MCS1 && (Rssi > (-86+RssiOffset)))
					TxRateIdx = MCS1;
				else
					TxRateIdx = MCS0;
		 	}
		}
		else
		{/* Legacy mode */
			if (MCS7 && (Rssi > -70))
				TxRateIdx = MCS7;
			else if (MCS6 && (Rssi > -74))
				TxRateIdx = MCS6;
			else if (MCS5 && (Rssi > -78))
				TxRateIdx = MCS5;
			else if (MCS4 && (Rssi > -82))
				TxRateIdx = MCS4;
			else if (MCS4 == 0)							/* for B-only mode */
				TxRateIdx = MCS3;
			else if (MCS3 && (Rssi > -85))
				TxRateIdx = MCS3;
			else if (MCS2 && (Rssi > -87))
				TxRateIdx = MCS2;
			else if (MCS1 && (Rssi > -90))
				TxRateIdx = MCS1;
			else
				TxRateIdx = MCS0;
				pEntry->mcsGroup = 2;
				
		}

			(pEntry->fewPktsCnt) ++;
		DBGPRINT(RT_DEBUG_INFO | DBG_FUNC_RA, ("f->%d\n", pEntry->fewPktsCnt));
			if (pEntry->fewPktsCnt == FEW_PKTS_CNT_THRD)
			{
				pEntry->fewPktsCnt = 0;
		
			/*if (TxRateIdx != pEntry->CurrTxRateIndex) */
				{
					pEntry->lastRateIdx = pEntry->CurrTxRateIndex;
					MlmeSetMcsGroup(pAd, pEntry);

					pEntry->CurrTxRateIndex = TxRateIdx;
					pNextTxRate = (PRTMP_TX_RATE_SWITCH) &pTable[(pEntry->CurrTxRateIndex+1)*10];

					APMlmeSetTxRate(pAd, pEntry, pNextTxRate);
					DBGPRINT_RAW(RT_DEBUG_INFO | DBG_FUNC_RA,
									("DRS: TxTotalCnt <= 15 for several times, switch to MCS%d according to RSSI (%d), RssiOffset=%d\n", 
									pNextTxRate->CurrMCS, Rssi, RssiOffset));
		}

		NdisZeroMemory(pEntry->TxQuality, sizeof(USHORT) * MAX_STEP_OF_TX_RATE_SWITCH);/*clear all history, same as train up, purpose??? */
		NdisZeroMemory(pEntry->PER, sizeof(UCHAR) * MAX_STEP_OF_TX_RATE_SWITCH);/*clear all history, same as train up, purpose??? */
		pEntry->fLastSecAccordingRSSI = TRUE;
			}

		/* reset all OneSecTx counters */
		RESET_ONE_SEC_TX_CNT(pEntry);

#ifdef TXBF_SUPPORT
		if (pAd->chipCap.FlgHwTxBfCap)
		{
			pNextTxRate = (PRTMP_TX_RATE_SWITCH) &pTable[(pEntry->CurrTxRateIndex+1)*10];
			eTxBFProbing(pAd, pEntry, pNextTxRate);
		}
#endif /* TXBF_SUPPORT */
		return;
	}

		pEntry->fewPktsCnt = 0;
	/*
		After pEntry->fLastSecAccordingRSSI = TRUE; the for loop continue. 
		this condition is true when RateSwitching() is run next time.
		So the next rate adaptation is skipped. This mechanism is deliberately 
		designed by rory.
	*/
	if (pEntry->fLastSecAccordingRSSI == TRUE)
	{
		pEntry->fLastSecAccordingRSSI = FALSE;
		pEntry->LastSecTxRateChangeAction = RATE_NO_CHANGE;
			DBGPRINT_RAW(RT_DEBUG_INFO | DBG_FUNC_RA, 
							("DRS: MCS is according to RSSI, and ignore tuning this sec \n"));

		/* reset all OneSecTx counters */
		RESET_ONE_SEC_TX_CNT(pEntry);

		return;
	}

	do
	{
		BOOLEAN	bTrainUpDown = FALSE;

		pEntry->CurrTxRateStableTime ++;/*redundant */

		/* downgrade TX quality if PER >= Rate-Down threshold */
		if ((TxErrorRatio >= TrainDown) ||
			((phyRateLimit20 != 0) && (pCurrTxRate->dataRate>=phyRateLimit20)))
		{
			bTrainUpDown = TRUE;
			pEntry->TxQuality[CurrRateIdx] = DRS_TX_QUALITY_WORST_BOUND;/*so that mcs decrease occurs right away */
		}
		/* upgrade TX quality if PER <= Rate-Up threshold */
		else if (TxErrorRatio <= TrainUp)
		{
			bTrainUpDown = TRUE;
			bUpgradeQuality = TRUE;/*redundant */
			if (pEntry->TxQuality[CurrRateIdx])
				pEntry->TxQuality[CurrRateIdx] --;  /* quality very good in CurrRate */

			if (pEntry->TxRateUpPenalty)/*always == 0, always go to else */
				pEntry->TxRateUpPenalty --;
			else
			{
				if (pEntry->TxQuality[pCurrTxRate->upMcs3] && pCurrTxRate->upMcs3 != CurrRateIdx)
					pEntry->TxQuality[pCurrTxRate->upMcs3] --;
				if (pEntry->TxQuality[pCurrTxRate->upMcs2] && pCurrTxRate->upMcs2 != CurrRateIdx)
					pEntry->TxQuality[pCurrTxRate->upMcs2] --;
				if (pEntry->TxQuality[pCurrTxRate->upMcs1] && pCurrTxRate->upMcs1 != CurrRateIdx)
					pEntry->TxQuality[pCurrTxRate->upMcs1] --;
			}
		}
		else if (pEntry->mcsGroup > 0) /* even if TxErrorRatio > TrainUp */
		{
			/* moderate per but some groups are not tried */
			if (UpRateIdx != 0)
			{
				bTrainUpDown = TRUE;

				/* TxQuality[CurrRateIdx] must be decremented so that mcs won't decrease wrongly */
				if (pEntry->TxQuality[CurrRateIdx])
					pEntry->TxQuality[CurrRateIdx]--;  /* quality very good in CurrRate */

	/*			if (pEntry->TxRateUpPenalty)//always == 0, always go to else */
	/*				pEntry->TxRateUpPenalty --; */
						/*else if (pEntry->TxQuality[UpRateIdx]) */
				if (pEntry->TxQuality[UpRateIdx])
					pEntry->TxQuality[UpRateIdx] --;    /* may improve next UP rate's quality */
			}
				
		}

		pEntry->PER[CurrRateIdx] = (UCHAR)TxErrorRatio;/*redundant */

		if (bTrainUpDown == TRUE)
		{
			PRTMP_TX_RATE_SWITCH_3S pUpRateIdx = (PRTMP_TX_RATE_SWITCH_3S) &pTable[(UpRateIdx+1)*10];

			/* perform DRS - consider TxRate Down first, then rate up. */
			/*TxQuality is checked */
			if ((CurrRateIdx != DownRateIdx) && (pEntry->TxQuality[CurrRateIdx] >= DRS_TX_QUALITY_WORST_BOUND))
			{
				pEntry->CurrTxRateIndex = DownRateIdx;
				pEntry->LastSecTxRateChangeAction = RATE_DOWN;
			}
			else if ((CurrRateIdx != UpRateIdx) && (pEntry->TxQuality[UpRateIdx] <= 0) && /*UpRateIdx, not currRateIdx */
					(phyRateLimit20==0 || pUpRateIdx->dataRate<phyRateLimit20) )
			{
				pEntry->CurrTxRateIndex = UpRateIdx;
				pEntry->LastSecTxRateChangeAction = RATE_UP;
			}
		}
	}while (FALSE);

	/* if rate-up happen, clear all bad history of all TX rates */
	if (pEntry->CurrTxRateIndex != CurrRateIdx && pEntry->LastSecTxRateChangeAction == RATE_UP)
	{
		DBGPRINT_RAW(RT_DEBUG_INFO | DBG_FUNC_RA,
						("DRS: ++TX rate from %d to %d \n", 
						CurrRateIdx, pEntry->CurrTxRateIndex));

		pEntry->CurrTxRateStableTime = 0;/*redundant */
		pEntry->TxRateUpPenalty = 0;/*redundant */
		pEntry->LastSecTxRateChangeAction = RATE_UP;
		/*clean the memory after the new mcs is verified */
		NdisZeroMemory(pEntry->TxQuality, sizeof(USHORT) * MAX_STEP_OF_TX_RATE_SWITCH);

		pNextTxRateIdx = (PRTMP_TX_RATE_SWITCH_3S) &pTable[(pEntry->CurrTxRateIndex+1)*10]; 
		NdisZeroMemory(pEntry->PER, sizeof(UCHAR) * MAX_STEP_OF_TX_RATE_SWITCH);/*redundant */
			pEntry->lastRateIdx = CurrRateIdx;

		/*
			For TxRate fast train up
		*/
		if (!pAd->ApCfg.ApQuickResponeForRateUpTimerRunning)
		{
			RTMPSetTimer(&pAd->ApCfg.ApQuickResponeForRateUpTimer, pAd->ra_fast_interval);
			pAd->ApCfg.ApQuickResponeForRateUpTimerRunning = TRUE;
		}
	}
	/* if rate-down happen, only clear DownRate's bad history */
	else if (pEntry->CurrTxRateIndex != CurrRateIdx && pEntry->LastSecTxRateChangeAction == RATE_DOWN)
	{
		DBGPRINT_RAW(RT_DEBUG_INFO | DBG_FUNC_RA, 
						("DRS: --TX rate from %d to %d \n", 
						CurrRateIdx, pEntry->CurrTxRateIndex));

		pEntry->CurrTxRateStableTime = 0;/*redundant */
		pEntry->TxRateUpPenalty = 0;           /* no penalty */
		pEntry->LastSecTxRateChangeAction = RATE_DOWN;
		pEntry->TxQuality[pEntry->CurrTxRateIndex] = 0;
		pEntry->PER[pEntry->CurrTxRateIndex] = 0;/*redundant */
		pEntry->lastRateIdx = CurrRateIdx;
	
		/*
			For TxRate fast train down
		*/
		if (!pAd->ApCfg.ApQuickResponeForRateUpTimerRunning)
		{
			RTMPSetTimer(&pAd->ApCfg.ApQuickResponeForRateUpTimer, pAd->ra_fast_interval);
			pAd->ApCfg.ApQuickResponeForRateUpTimerRunning = TRUE;
		}
	}
	else
	{
		pEntry->LastSecTxRateChangeAction = RATE_NO_CHANGE;
		bTxRateChanged = FALSE;
	}
		
	if (pAd->MacTab.Size == 1)
	{
		/*test by gary */
       	/*pEntry->LastTxOkCount = pAd->RalinkCounters.OneSecTxNoRetryOkCount; */
		pEntry->LastTxOkCount = TxSuccess;/*for APQuickResponeForRateUpExec() */
	}
	else
	{
		pEntry->LastTxOkCount = pEntry->OneSecTxNoRetryOkCount;/*for APQuickResponeForRateUpExec() */
#ifdef FIFO_EXT_SUPPORT
		if (pAd->chipCap.FlgHwFifoExtCap)
		{
			if (pEntry->Aid >= 1 && pEntry->Aid <= 8)
				pEntry->LastTxOkCount = pEntry->fifoTxSucCnt;
		}
#endif /* FIFO_EXT_SUPPORT */
	}

	tmpTxRate = pEntry->CurrTxRateIndex;

	/*turn off RDG when 3s and rx count > tx count*5 */
	if (((pTable == RateSwitchTable11BGN3S) || (pTable == RateSwitchTable11BGN3SForABand) || (pTable == RateSwitchTable11N3S_Group)) &&
		(pAd->RalinkCounters.OneSecReceivedByteCount > 50000) &&
		(pAd->RalinkCounters.OneSecTransmittedByteCount > 50000) &&
		CLIENT_STATUS_TEST_FLAG(pEntry, fCLIENT_STATUS_RDG_CAPABLE))
	{
		TX_LINK_CFG_STRUC	TxLinkCfg;
		ULONG				TxOpThres;

		pTempTxRate = (PRTMP_TX_RATE_SWITCH_3S)(&pTable[(tmpTxRate + 1)*10]);
		RTMP_IO_READ32(pAd, TX_LINK_CFG, &TxLinkCfg.word);

		if ((pAd->RalinkCounters.OneSecReceivedByteCount > (pAd->RalinkCounters.OneSecTransmittedByteCount * 5)) &&
			(pTempTxRate->CurrMCS != 23) &&
			(pTempTxRate->ShortGI != 1))
		{
			if (TxLinkCfg.field.TxRDGEn == 1)
			{
				TxLinkCfg.field.TxRDGEn = 0;
				RTMP_IO_WRITE32(pAd, TX_LINK_CFG, TxLinkCfg.word);
				RTMP_IO_READ32(pAd, TXOP_THRES_CFG, &TxOpThres);
				TxOpThres |= 0xff00;
				RTMP_IO_WRITE32(pAd, TXOP_THRES_CFG, TxOpThres);
				DBGPRINT_RAW(RT_DEBUG_WARN,("DRS: RDG off!\n"));
			}
		}
		else
		{
			if (TxLinkCfg.field.TxRDGEn == 0)
			{
				TxLinkCfg.field.TxRDGEn = 1;
				RTMP_IO_WRITE32(pAd, TX_LINK_CFG, TxLinkCfg.word);
				RTMP_IO_READ32(pAd, TXOP_THRES_CFG, &TxOpThres);
				TxOpThres &= 0xffff00ff;
				RTMP_IO_WRITE32(pAd, TXOP_THRES_CFG, TxOpThres);
				DBGPRINT_RAW(RT_DEBUG_WARN,("DRS: RDG on!\n"));
			}
		}
	}

	/* reset all OneSecTx counters */
	RESET_ONE_SEC_TX_CNT(pEntry);/*reasonable to reset this counter whenever RateSwitching() is over */

	pNextTxRate = (PRTMP_TX_RATE_SWITCH) &pTable[(tmpTxRate+1)*10];

	if (bTxRateChanged && pNextTxRate)
	{
			/*DBGPRINT(RT_DEBUG_WARN, ("-->%d\n", pNextTxRate->CurrMCS)); */
		APMlmeSetTxRate(pAd, pEntry, pNextTxRate);
	}

#ifdef TXBF_SUPPORT
	if (pAd->chipCap.FlgHwTxBfCap)
		eTxBFProbing(pAd, pEntry, pNextTxRate);
#endif /* TXBF_SUPPORT */
}


/*
    ========================================================================
    Routine Description:
        AP side, Auto TxRate faster train up timer call back function.
        
    Arguments:
        SystemSpecific1         - Not used.
        FunctionContext         - Pointer to our Adapter context.
        SystemSpecific2         - Not used.
        SystemSpecific3         - Not used.
        
    Return Value:
        None
        
    ========================================================================
*/
VOID APQuickResponeForRateUpExecAdapt(/*actually for both up and down */
    IN PRTMP_ADAPTER pAd,
    IN ULONG idx) 
{
	PUCHAR					pTable;
	UCHAR					TableSize = 0;
	UCHAR					UpRateIdx, DownRateIdx, CurrRateIdx;
	ULONG					AccuTxTotalCnt, TxTotalCnt, TxCnt;
	ULONG					TxErrorRatio = 0;
#ifdef FIFO_EXT_SUPPORT
	ULONG 					HwTxCnt, HwErrRatio;
#endif /* FIFO_EXT_SUPPORT */
	MAC_TABLE_ENTRY			*pEntry;
	PRTMP_TX_RATE_SWITCH_3S	pCurrTxRate;
	PRTMP_TX_RATE_SWITCH	pNextTxRate = NULL;
	BOOLEAN					bTxRateChanged = TRUE;
	UCHAR					InitTxRateIdx, TrainUp, TrainDown;
	TX_STA_CNT1_STRUC		StaTx1;
	TX_STA_CNT0_STRUC		TxStaCnt0;
	CHAR					Rssi, ratio;
	ULONG					TxRetransmit = 0, TxSuccess = 0, TxFailCount = 0;

	pAd->ApCfg.ApQuickResponeForRateUpTimerRunning = FALSE;
	
	 pEntry = &pAd->MacTab.Content[idx];


	APMlmeSelectTxRateTable(pAd, pEntry, &pTable, &TableSize, &InitTxRateIdx);

	/*Rssi = RTMPMaxRssi(pAd, (CHAR)pEntry->RssiSample.AvgRssi0, (CHAR)pEntry->RssiSample.AvgRssi1, (CHAR)pEntry->RssiSample.AvgRssi2); */
	Rssi = RTMPAvgRssi(pAd, &pEntry->RssiSample);

	CurrRateIdx = UpRateIdx = DownRateIdx = pEntry->CurrTxRateIndex;

	if (pAd->MacTab.Size == 1)
	{
		/* Update statistic counter */
		NicGetTxRawCounters(pAd, &TxStaCnt0, &StaTx1);

		TxRetransmit = StaTx1.field.TxRetransmit;
		TxSuccess = StaTx1.field.TxSuccess;
		TxFailCount = TxStaCnt0.field.TxFailCount;
		TxTotalCnt = TxRetransmit + TxSuccess + TxFailCount;

		AccuTxTotalCnt = pAd->RalinkCounters.OneSecTxNoRetryOkCount + 
							pAd->RalinkCounters.OneSecTxRetryOkCount + 
							pAd->RalinkCounters.OneSecTxFailCount;

		if (TxTotalCnt)
			TxErrorRatio = ((TxRetransmit + TxFailCount) * 100) / TxTotalCnt;

		/*Rssi is calculated again with new formula?In rory's code, the average instead of max is used. */
		if (pAd->Antenna.field.TxPath > 1)
			Rssi = (pEntry->RssiSample.AvgRssi0 + pEntry->RssiSample.AvgRssi1) >> 1;
		else
			Rssi = pEntry->RssiSample.AvgRssi0;

		TxCnt = AccuTxTotalCnt;
	}
	else
	{
		TxTotalCnt = pEntry->OneSecTxNoRetryOkCount + 
						pEntry->OneSecTxRetryOkCount + 
						pEntry->OneSecTxFailCount;

		if (TxTotalCnt)
			TxErrorRatio = ((pEntry->OneSecTxRetryOkCount + pEntry->OneSecTxFailCount) * 100) / TxTotalCnt;
	
		TxCnt = TxTotalCnt;

#ifdef FIFO_EXT_SUPPORT
		if (pAd->chipCap.FlgHwFifoExtCap)
		{
			if ((pEntry->Aid >= 1) && (pEntry->Aid <= 8))
			{
				NicGetMacFifoTxCnt(pAd, pEntry);
				HwTxCnt = pEntry->fifoTxSucCnt + pEntry->fifoTxRtyCnt;
				if (HwTxCnt)
					HwErrRatio = (pEntry->fifoTxRtyCnt * 100) / HwTxCnt;
				else
					HwErrRatio = 0;
				
				DBGPRINT(RT_DEBUG_TRACE | DBG_FUNC_RA,("%s():Aid:%d, MCS:%d, TxErrRation(Hw:0x%lx-0x%lx, Sw:0x%lx-%lx)\n", 
						__FUNCTION__, pEntry->Aid, pEntry->HTPhyMode.field.MCS, 
						HwTxCnt, HwErrRatio, TxTotalCnt, TxErrorRatio));

				TxErrorRatio = HwErrRatio;
				TxTotalCnt = HwTxCnt;
				TxCnt = HwTxCnt;
			}
		}
#endif /* FIFO_EXT_SUPPORT */

	}

#ifdef MFB_SUPPORT
	if (pEntry->fLastChangeAccordingMfb == TRUE)
	{
		pEntry->fLastChangeAccordingMfb = FALSE;
		pEntry->LastSecTxRateChangeAction = RATE_NO_CHANGE;
		DBGPRINT_RAW(RT_DEBUG_INFO | DBG_FUNC_RA,("DRS: MCS is according to MFB, and ignore tuning this sec \n"));
		/* reset all OneSecTx counters */
		RESET_ONE_SEC_TX_CNT(pEntry);		
			return;
	}
#endif	/* MFB_SUPPORT */

	/* decide the next upgrade rate and downgrade rate, if any */
	UpRateIdx = DownRateIdx = pEntry->lastRateIdx;

	pCurrTxRate = (PRTMP_TX_RATE_SWITCH_3S) &pTable[(CurrRateIdx+1)*10];

#ifdef DOT11_N_SUPPORT
		if ((Rssi > -65) && (pCurrTxRate->Mode >= MODE_HTMIX)  && pEntry->perThrdAdj == 1 )
	{
		TrainUp		= (pCurrTxRate->TrainUp + (pCurrTxRate->TrainUp >> 1));
		TrainDown	= (pCurrTxRate->TrainDown + (pCurrTxRate->TrainDown >> 1));
	}
	else
#endif /* DOT11_N_SUPPORT */
	{
		TrainUp		= pCurrTxRate->TrainUp;
		TrainDown	= pCurrTxRate->TrainDown;
	}

	if (pAd->MacTab.Size == 1)
	{
			DBGPRINT_RAW(RT_DEBUG_INFO | DBG_FUNC_RA,("   QuickDRS:Aid=%d, TxRetransmit=%ld, TxFailCount=%ld, TxSuccess=%ld \n",
					pEntry->Aid, TxRetransmit, TxFailCount,	TxSuccess));
	}
	else
	{
			DBGPRINT_RAW(RT_DEBUG_INFO | DBG_FUNC_RA,("   QuickDRS:Aid=%d, OneSecTxRetryOkCount=%d, OneSecTxFailCount=%d, OneSecTxNoRetryOkCount=%d \n",
					pEntry->Aid,
					pEntry->OneSecTxRetryOkCount,
					pEntry->OneSecTxFailCount,
					pEntry->OneSecTxNoRetryOkCount));
	}

	{
		DBGPRINT_RAW(RT_DEBUG_INFO | DBG_FUNC_RA,("   QuickDRS: Before- CurrRateIdx=%d, MCS=%d, STBC=%d, SGI=%d, Mode=%d, TrainUp/Down=%d/%d%%, NextUp/Down=%d/%d, CurrMCS=%d, PER=%ld%%\n",
					CurrRateIdx,
					pCurrTxRate->CurrMCS,
					pCurrTxRate->STBC,
					pCurrTxRate->ShortGI,
					pCurrTxRate->Mode,
					TrainUp,
					TrainDown,
					UpRateIdx,
					DownRateIdx,
					pEntry->HTPhyMode.field.MCS,		
					TxErrorRatio));
	}



	if (/*TxTotalCnt*/ TxCnt <= 15)
	{
		NdisZeroMemory(pEntry->TxQuality, sizeof(USHORT) * MAX_STEP_OF_TX_RATE_SWITCH);/*redundant */
		NdisZeroMemory(pEntry->PER, sizeof(UCHAR) * MAX_STEP_OF_TX_RATE_SWITCH);/*redundant */

		/*recover the original mcs */
		/* perform DRS - consider TxRate Down first, then rate up. */
		if ((pEntry->LastSecTxRateChangeAction == RATE_UP) && (CurrRateIdx != DownRateIdx))
		{
			pEntry->CurrTxRateIndex = DownRateIdx;
			pEntry->TxQuality[CurrRateIdx] = DRS_TX_QUALITY_WORST_BOUND;/*set current MCS at the worst quality */
		}
		else if ((pEntry->LastSecTxRateChangeAction == RATE_DOWN) && (CurrRateIdx != UpRateIdx))/*TxQuality and PER are not cleared??? */
		{
			pEntry->CurrTxRateIndex = UpRateIdx;
		}
	
		DBGPRINT_RAW(RT_DEBUG_INFO | DBG_FUNC_RA, 
						("   QuickDRS: TxTotalCnt <= 15, train back to original rate \n"));
		return;
	}

	do
	{
		ULONG		OneSecTxNoRetryOKRationCount;

		ratio = (pAd->ra_interval / pAd->ra_fast_interval);
		if (pEntry->LastTimeTxRateChangeAction != RATE_NO_CHANGE)
			ratio--;

		/* downgrade TX quality if PER >= Rate-Down threshold */
		if (TxErrorRatio >= TrainDown)
		{
			pEntry->TxQuality[CurrRateIdx] = DRS_TX_QUALITY_WORST_BOUND;/*the only situation when pEntry->TxQuality[CurrRateIdx] = DRS_TX_QUALITY_WORST_BOUND but no rate change */
		}

		pEntry->PER[CurrRateIdx] = (UCHAR)TxErrorRatio;

		if (pAd->MacTab.Size == 1)
		{
			/* test by gary */
			OneSecTxNoRetryOKRationCount = (TxSuccess * ratio);
		}
		else
		{
			OneSecTxNoRetryOKRationCount = pEntry->OneSecTxNoRetryOkCount * ratio + (pEntry->OneSecTxNoRetryOkCount >> 1);/*the second term does not appear in rory's code */
		}


		/* perform DRS - consider TxRate Down first, then rate up. */
		if ((pEntry->LastSecTxRateChangeAction == RATE_UP) && (CurrRateIdx != DownRateIdx))
		{
			if (TxErrorRatio >= TrainDown)
			{
				pEntry->CurrTxRateIndex = DownRateIdx;
				pEntry->TxQuality[CurrRateIdx] = DRS_TX_QUALITY_WORST_BOUND;
			}
			else
			{
				DBGPRINT_RAW(RT_DEBUG_TRACE | DBG_FUNC_RA,("   QuickDRS: (Up) keep rate-up (L:%ld, C:%ld)\n", pEntry->LastTxOkCount, OneSecTxNoRetryOKRationCount));
				NdisZeroMemory(pEntry->TxQuality, sizeof(USHORT) * MAX_STEP_OF_TX_RATE_SWITCH);
					if (pEntry->mcsGroup == 0)
				{
					MlmeSetMcsGroup(pAd, pEntry);
				}	
			}
		}
		else if ((pEntry->LastSecTxRateChangeAction == RATE_DOWN) && (CurrRateIdx != UpRateIdx))
		{
				/*if ((TxErrorRatio >= 50) || (TxErrorRatio >= TrainDown)) */
			if ((TxErrorRatio >= 50) && (TxErrorRatio >= TrainDown))/*there will be train down again */
			/*have to remember old PCR so that we can know mcs changes from 11 to 10!!! */
			{
				MlmeSetMcsGroup(pAd, pEntry);
				DBGPRINT_RAW(RT_DEBUG_TRACE,("   QuickDRS: (Down) direct train down (TxErrorRatio >= TrainDown)\n"));
			}
			else if ((pEntry->LastTxOkCount + 2) >= OneSecTxNoRetryOKRationCount)/*this is esentially throughput */
			{
				pEntry->CurrTxRateIndex = UpRateIdx;/*doesn't have to check TxQuality[UpRateIdx] */
				DBGPRINT_RAW(RT_DEBUG_TRACE | DBG_FUNC_RA,("   QuickDRS: (Down) bad tx ok count (L:%ld, C:%ld)\n", pEntry->LastTxOkCount, OneSecTxNoRetryOKRationCount));
			}
			else
			{
				MlmeSetMcsGroup(pAd, pEntry);
				DBGPRINT_RAW(RT_DEBUG_TRACE | DBG_FUNC_RA,("   QuickDRS: (Down) keep rate-down (L:%ld, C:%ld)\n", pEntry->LastTxOkCount, OneSecTxNoRetryOKRationCount));
			}
		}
	}while (FALSE);


	if (pEntry->LastSecTxRateChangeAction == RATE_UP)
	{
		/* Last action is up, looking for the next group with valid mcs */
			if (pEntry->CurrTxRateIndex != CurrRateIdx && pEntry->mcsGroup > 0)
			{/*move back */
				pEntry->mcsGroup --;				
			pCurrTxRate = (PRTMP_TX_RATE_SWITCH_3S) &pTable[(DownRateIdx+1)*10];
		}

		/* UpRateIdx is for temp use in this section */
		switch (pEntry->mcsGroup)
		{
			case 3:
				UpRateIdx = pCurrTxRate->upMcs3;
				break;
			case 2:
				UpRateIdx = pCurrTxRate->upMcs2;
				break;
			case 1:
				UpRateIdx = pCurrTxRate->upMcs1;
				break;
			case 0:
				UpRateIdx = CurrRateIdx;
				break;
			default:
				DBGPRINT_RAW(RT_DEBUG_ERROR, ("wrong mcsGroup value\n"));
				break;
		}

		if (UpRateIdx == pEntry->CurrTxRateIndex)
				pEntry->mcsGroup = 0;
			/*DBGPRINT_RAW(RT_DEBUG_TRACE,("              QuickDRS: next mcsGroup =%d \n", pEntry->mcsGroup)); */
	}
	/* if rate-up happen, clear all bad history of all TX rates */
	if (pEntry->CurrTxRateIndex != CurrRateIdx && pEntry->LastSecTxRateChangeAction == RATE_DOWN)
	{
		DBGPRINT_RAW(RT_DEBUG_TRACE | DBG_FUNC_RA,("   QuickDRS: ++TX rate from %d to %d \n", CurrRateIdx, pEntry->CurrTxRateIndex));
	
		pEntry->TxRateUpPenalty = 0;/*redundant */
		/*NdisZeroMemory(pEntry->TxQuality, sizeof(USHORT) * MAX_STEP_OF_TX_RATE_SWITCH); */
		pEntry->TxQuality[pEntry->CurrTxRateIndex] = 0;/*restore the TxQuality from max to 0 */
		NdisZeroMemory(pEntry->PER, sizeof(UCHAR) * MAX_STEP_OF_TX_RATE_SWITCH);
	}
	/* if rate-down happen, only clear DownRate's bad history */
	else if (pEntry->CurrTxRateIndex != CurrRateIdx && pEntry->LastSecTxRateChangeAction == RATE_UP)
	{
		DBGPRINT_RAW(RT_DEBUG_TRACE | DBG_FUNC_RA,("   QuickDRS: --TX rate from %d to %d \n", CurrRateIdx, pEntry->CurrTxRateIndex));

		pEntry->TxRateUpPenalty = 0;           /* no penalty//redundant */
		pEntry->TxQuality[pEntry->CurrTxRateIndex] = 0;
		pEntry->PER[pEntry->CurrTxRateIndex] = 0;/*redundant */
	}
	else
	{
		bTxRateChanged = FALSE;
	}

	pNextTxRate = (PRTMP_TX_RATE_SWITCH) &pTable[(pEntry->CurrTxRateIndex+1)*10];

	if (bTxRateChanged && pNextTxRate)
	{
			/*DBGPRINT(RT_DEBUG_WARN, ("==>%d\n", pNextTxRate->CurrMCS)); */
		APMlmeSetTxRate(pAd, pEntry, pNextTxRate);

#ifdef TXBF_SUPPORT
		if (pEntry->eTxBfEnCond == 2)
			txSndgSameMcs(pAd, pEntry, pNextTxRate->CurrMCS);
#endif /* TXBF_SUPPORT */
	}
}
#endif /* CONFIG_AP_SUPPORT */




INT	Set_UseNewRateAdapt_Proc(
		IN	PRTMP_ADAPTER	pAd, 
		IN	PSTRING arg)
{
	/* insert code here */
	UCHAR i;
	MAC_TABLE_ENTRY		*pEntry = NULL;	
	for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++){
		pEntry =&pAd->MacTab.Content[i];
		pEntry->useNewRateAdapt = simple_strtol(arg, 0, 10);
	}
	return TRUE;	
}


INT	Set_PerThrdAdj_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	UCHAR i;
	for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++){
		pAd->MacTab.Content[i].perThrdAdj = simple_strtol(arg, 0, 10);
	}
	return TRUE;	
}

#endif /* NEW_RATE_ADAPT_SUPPORT */

/* End of drs_grp.c */
