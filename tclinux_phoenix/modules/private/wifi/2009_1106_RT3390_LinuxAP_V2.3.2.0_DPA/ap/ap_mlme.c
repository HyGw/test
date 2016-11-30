/****************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ****************************************************************************
 
    Module Name:
    mlme.c
 
    Abstract:
    Major MLME state machiones here
 
    Revision History:
    Who         When          What
    --------    ----------    ----------------------------------------------
    John Chang  08-04-2003    created for 11g soft-AP
 */

#include "rt_config.h"
#include <stdarg.h>


#ifdef DOT11_N_SUPPORT
//BOOLEAN isExtChannel;
int DetectOverlappingPeriodicRound;


#ifdef DOT11N_DRAFT3
VOID Bss2040CoexistTimeOut(
	IN PVOID SystemSpecific1, 
	IN PVOID FunctionContext, 
	IN PVOID SystemSpecific2, 
	IN PVOID SystemSpecific3)
{
	int apidx;
	PRTMP_ADAPTER	pAd = (RTMP_ADAPTER *)FunctionContext;

	DBGPRINT(RT_DEBUG_TRACE, ("Bss2040CoexistTimeOut(): Recovery to original setting!\n"));
	
	// Recovery to original setting when next DTIM Interval.
	pAd->CommonCfg.Bss2040CoexistFlag &= (~BSS_2040_COEXIST_TIMER_FIRED);
	NdisZeroMemory(&pAd->CommonCfg.LastBSSCoexist2040, sizeof(BSS_2040_COEXIST_IE));
	pAd->CommonCfg.Bss2040CoexistFlag |= BSS_2040_COEXIST_INFO_SYNC;
	
	if (pAd->CommonCfg.bBssCoexEnable == FALSE)
	{
		// TODO: Find a better way to handle this when the timer is fired and we disable the bBssCoexEable support!!
		DBGPRINT(RT_DEBUG_TRACE, ("Bss2040CoexistTimeOut(): bBssCoexEnable is FALSE, return directly!\n"));
		return;
	}
	
	for (apidx = 0; apidx < pAd->ApCfg.BssidNum; apidx++)
		SendBSS2040CoexistMgmtAction(pAd, MCAST_WCID, apidx, 0);
	
}
#endif // DOT11N_DRAFT3 //

#endif // DOT11_N_SUPPORT //


VOID APDetectOverlappingExec(
	IN PVOID SystemSpecific1, 
	IN PVOID FunctionContext, 
	IN PVOID SystemSpecific2, 
	IN PVOID SystemSpecific3) 
{
#ifdef DOT11_N_SUPPORT
	PRTMP_ADAPTER	pAd = (RTMP_ADAPTER *)FunctionContext;

	if (DetectOverlappingPeriodicRound == 0)
	{
		// switch back 20/40		
		if ((pAd->CommonCfg.Channel <=14) && (pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth == BW_40))
		{
            pAd->CommonCfg.AddHTInfo.AddHtInfo.RecomWidth = 1;	
			pAd->CommonCfg.AddHTInfo.AddHtInfo.ExtChanOffset = pAd->CommonCfg.RegTransmitSetting.field.EXTCHA;			
		}
	}
	else
	{
		if ((DetectOverlappingPeriodicRound == 25) || (DetectOverlappingPeriodicRound == 1))
		{   
   			if ((pAd->CommonCfg.Channel <=14) && (pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth==BW_40))
			{                                     
				SendBeaconRequest(pAd, 1);			
				SendBeaconRequest(pAd, 2);
                SendBeaconRequest(pAd, 3);
			}

		}
		DetectOverlappingPeriodicRound--;
	}


#endif // DOT11_N_SUPPORT //
}


/*
    ==========================================================================
    Description:
        This routine is executed every second -
        1. Decide the overall channel quality
        2. Check if need to upgrade the TX rate to any client
        3. perform MAC table maintenance, including ageout no-traffic clients, 
           and release packet buffer in PSQ is fail to TX in time.
    ==========================================================================
 */
VOID APMlmePeriodicExec(
    PRTMP_ADAPTER pAd)
{
    // Reqeust by David 2005/05/12
    // It make sense to disable Adjust Tx Power on AP mode, since we can't take care all of the client's situation    
    // ToDo: need to verify compatibility issue with WiFi product.
    // 

	/*
		We return here in ATE mode, because the statistics 
		that ATE need are not collected via this routine.
	*/
#ifdef RALINK_ATE
	if (ATE_ON(pAd))
		return;
#endif // RALINK_ATE //


	// Disable Adjust Tx Power for WPA WiFi-test. 
	// Because high TX power results in the abnormal disconnection of Intel BG-STA.  
//#ifndef WIFI_TEST    
	if (pAd->CommonCfg.bWiFiTest == FALSE)	
		AsicAdjustTxPower(pAd);
//#endif // WIFI_TEST //

	/* BBP TUNING: dynamic tune BBP R66 to find a balance between sensibility
		and noise isolation */
//	AsicBbpTuning2(pAd);

    // walk through MAC table, see if switching TX rate is required

    // MAC table maintenance
	if (pAd->Mlme.PeriodicRound % MLME_TASK_EXEC_MULTIPLE == 0)
	{
		// one second timer
	    MacTableMaintenance(pAd);
		RTMPMaintainPMKIDCache(pAd);

#ifdef WDS_SUPPORT
		WdsTableMaintenance(pAd);
#endif // WDS_SUPPORT //


#ifdef CLIENT_WDS
	CliWds_ProxyTabMaintain(pAd);
#endif // CLIENT_WDS //
	}
	
	APUpdateCapabilityAndErpIe(pAd);

#ifdef APCLI_SUPPORT
	if (pAd->Mlme.OneSecPeriodicRound % 2 == 0)
		ApCliIfMonitor(pAd);

	if (pAd->Mlme.OneSecPeriodicRound % 2 == 1)
		ApCliIfUp(pAd);

	{
		INT loop;
		ULONG Now32;
		NdisGetSystemUpTime(&Now32);
		for (loop = 0; loop < MAX_APCLI_NUM; loop++)
		{
			PAPCLI_STRUCT pApCliEntry = &pAd->ApCfg.ApCliTab[loop];
			if ((pApCliEntry->Valid == TRUE)
				&& (pApCliEntry->MacTabWCID < MAX_LEN_OF_MAC_TABLE))
			{
				// update channel quality for Roaming and UI LinkQuality display
				MlmeCalculateChannelQuality(pAd,
					&pAd->MacTab.Content[pApCliEntry->MacTabWCID], Now32);
			}
		}
	}
#endif // APCLI_SUPPORT //

#ifdef DOT11_N_SUPPORT
    if (pAd->CommonCfg.bHTProtect)
    {
    	//APUpdateCapabilityAndErpIe(pAd);
    	APUpdateOperationMode(pAd);
		if (pAd->CommonCfg.IOTestParm.bRTSLongProtOn == FALSE)
		{
        	AsicUpdateProtect(pAd, (USHORT)pAd->CommonCfg.AddHTInfo.AddHtInfo2.OperaionMode, ALLN_SETPROTECT, FALSE, pAd->MacTab.fAnyStationNonGF);
    	}
    }
#endif // DOT11_N_SUPPORT //

#ifdef A_BAND_SUPPORT
	if ( (pAd->CommonCfg.Channel > 14)
		&& (pAd->CommonCfg.bIEEE80211H == 1)
		)
	{
#ifdef DFS_SUPPORT
		ApRadarDetectPeriodic(pAd);
#else
		pAd->CommonCfg.RadarDetect.InServiceMonitorCount++;
		if (pAd->CommonCfg.RadarDetect.RDMode == RD_SILENCE_MODE)
		{
			if (pAd->CommonCfg.RadarDetect.RDCount++ > pAd->CommonCfg.RadarDetect.ChMovingTime)
			{
#ifdef CARRIER_DETECTION_SUPPORT
				if (pAd->CommonCfg.CarrierDetect.Enable == TRUE)
				{
					// trun on Carrier-Detection. (Carrier-Detect with CTS protection).
					CarrierDetectionStart(pAd, 1);
				}
#endif // CARRIER_DETECTION_SUPPORT //
				AsicEnableBssSync(pAd);
				pAd->CommonCfg.RadarDetect.RDMode = RD_NORMAL_MODE;
			}
		}
#endif // DFS_SUPPORT //
	}

#ifdef DFS_SUPPORT
	if ( (pAd->CommonCfg.Channel > 14)
		&& (pAd->CommonCfg.bIEEE80211H == 1)
		&& RadarChannelCheck(pAd, pAd->CommonCfg.Channel)
		&& OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED)
		&& (pAd->CommonCfg.RadarDetect.RDMode == RD_NORMAL_MODE))
	{
		AdaptRadarDetection(pAd);
	}
#endif // DFS_SUPPORT //
#endif // A_BAND_SUPPORT //


}

VOID APMlmeSelectTxRateTable(
	IN PRTMP_ADAPTER		pAd,
	IN PMAC_TABLE_ENTRY		pEntry,
	IN PUCHAR				*ppTable,
	IN PUCHAR				pTableSize,
	IN PUCHAR				pInitTxRateIdx)
{
	MlmeSelectTxRateTable(pAd, pEntry, ppTable, pTableSize, pInitTxRateIdx);
}

VOID APMlmeSetTxRate(
	IN PRTMP_ADAPTER		pAd,
	IN PMAC_TABLE_ENTRY		pEntry,
	IN PRTMP_TX_RATE_SWITCH	pTxRate)
{
#ifdef DOT11_N_SUPPORT
	if ((pTxRate->STBC) && (pEntry->MaxHTPhyMode.field.STBC))
		pEntry->HTPhyMode.field.STBC = STBC_USE;
	else
		pEntry->HTPhyMode.field.STBC = STBC_NONE;

	if (((pTxRate->ShortGI) && (pEntry->MaxHTPhyMode.field.ShortGI))
         || (pAd->WIFItestbed.bShortGI && pEntry->MaxHTPhyMode.field.ShortGI) )
		pEntry->HTPhyMode.field.ShortGI = GI_400;
	else
		pEntry->HTPhyMode.field.ShortGI = GI_800;
#endif // DOT11_N_SUPPORT //

	if (pTxRate->CurrMCS < MCS_AUTO)
		pEntry->HTPhyMode.field.MCS = pTxRate->CurrMCS;

#ifdef DOT11_N_SUPPORT
	if (pTxRate->Mode <= MODE_HTGREENFIELD)
#endif // DOT11_N_SUPPORT //
		pEntry->HTPhyMode.field.MODE = pTxRate->Mode;

#ifdef DOT11_N_SUPPORT
	if ((pAd->WIFItestbed.bGreenField & pEntry->HTCapability.HtCapInfo.GF) && (pEntry->HTPhyMode.field.MODE == MODE_HTMIX))
	{
		// force Tx GreenField 
		pEntry->HTPhyMode.field.MODE = MODE_HTGREENFIELD;
	}

	if (pAd->CommonCfg.bRcvBSSWidthTriggerEvents)
	{
		pEntry->HTPhyMode.field.BW = BW_20;
	}
#endif // DOT11_N_SUPPORT //

	pAd->LastTxRate = (USHORT)(pEntry->HTPhyMode.word);

	
}


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
VOID APMlmeDynamicTxRateSwitching(
    IN PRTMP_ADAPTER pAd)
{
	ULONG					i;
	PUCHAR					pTable;
	UCHAR					TableSize = 0;
	UCHAR					UpRateIdx, DownRateIdx, CurrRateIdx;
	ULONG					AccuTxTotalCnt, TxTotalCnt;
	ULONG					TxErrorRatio = 0;
	MAC_TABLE_ENTRY		*pEntry;
	PRTMP_TX_RATE_SWITCH	pCurrTxRate, pNextTxRate = NULL;
	BOOLEAN					bTxRateChanged = TRUE, bUpgradeQuality = FALSE;
	UCHAR					InitTxRateIdx, TrainUp, TrainDown;
	TX_STA_CNT1_STRUC		StaTx1;
	TX_STA_CNT0_STRUC		TxStaCnt0;
	CHAR					Rssi, RssiOffset = 0;
	ULONG					TxRetransmit = 0, TxSuccess = 0, TxFailCount = 0;
	
#ifdef RALINK_ATE
   	if (ATE_ON(pAd))
   	{
		return;
   	}
#endif // RALINK_ATE //

    //
    // walk through MAC table, see if need to change AP's TX rate toward each entry
    //

	  

	for (i = 1; i < MAX_LEN_OF_MAC_TABLE; i++) 
	{
		pEntry = &pAd->MacTab.Content[i];


		if (IS_ENTRY_NONE(pEntry))
			continue;

		if (IS_ENTRY_CLIENT(pEntry) && (pEntry->Sst != SST_ASSOC))
			continue;

#ifdef APCLI_SUPPORT
		if (IS_ENTRY_APCLI(pEntry) && (pEntry->Sst != SST_ASSOC))
			continue;
#endif // APCLI_SUPPORT //

#ifdef WDS_SUPPORT
		if (IS_ENTRY_WDS(pEntry) && !WDS_IF_UP_CHECK(pAd, pEntry->MatchWDSTabIdx))
			continue;
#endif // WDS_SUPPORT //


		// check if this entry need to switch rate automatically
		if (RTMPCheckEntryEnableAutoRateSwitch(pAd, pEntry) == FALSE)
			continue;

		//NICUpdateFifoStaCounters(pAd);

		if (pAd->MacTab.Size == 1)
		{

			// Update statistic counter
			RTMP_IO_READ32(pAd, TX_STA_CNT0, &TxStaCnt0.word);
			RTMP_IO_READ32(pAd, TX_STA_CNT1, &StaTx1.word);
			pAd->bUpdateBcnCntDone = TRUE;
			TxRetransmit = StaTx1.field.TxRetransmit;
			TxSuccess = StaTx1.field.TxSuccess;
			TxFailCount = TxStaCnt0.field.TxFailCount;
			TxTotalCnt = TxRetransmit + TxSuccess + TxFailCount;

			pAd->RalinkCounters.OneSecBeaconSentCnt += TxStaCnt0.field.TxBeaconCount;
			pAd->RalinkCounters.OneSecTxRetryOkCount += StaTx1.field.TxRetransmit;
			pAd->RalinkCounters.OneSecTxNoRetryOkCount += StaTx1.field.TxSuccess;
			pAd->RalinkCounters.OneSecTxFailCount += TxStaCnt0.field.TxFailCount;

#ifdef STATS_COUNT_SUPPORT
			pAd->WlanCounters.TransmittedFragmentCount.u.LowPart += StaTx1.field.TxSuccess;
			pAd->WlanCounters.RetryCount.u.LowPart += StaTx1.field.TxRetransmit;
			pAd->WlanCounters.FailedCount.u.LowPart += TxStaCnt0.field.TxFailCount;
#endif // STATS_COUNT_SUPPORT //

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
		}

		CurrRateIdx = UpRateIdx = DownRateIdx = pEntry->CurrTxRateIndex;

		Rssi = RTMPMaxRssi(pAd, (CHAR)pEntry->RssiSample.AvgRssi0, (CHAR)pEntry->RssiSample.AvgRssi1, (CHAR)pEntry->RssiSample.AvgRssi2);

		APMlmeSelectTxRateTable(pAd, pEntry, &pTable, &TableSize, &InitTxRateIdx);

		// decide the next upgrade rate and downgrade rate, if any
		if ((CurrRateIdx > 0) && (CurrRateIdx < (TableSize - 1)))
		{
			UpRateIdx = CurrRateIdx + 1;
			DownRateIdx = CurrRateIdx -1;
		}
		else if (CurrRateIdx == 0)
		{
			UpRateIdx = CurrRateIdx + 1;
			DownRateIdx = CurrRateIdx;
		}
		else if (CurrRateIdx == (TableSize - 1))
		{
			UpRateIdx = CurrRateIdx;
			DownRateIdx = CurrRateIdx - 1;
		}

		pCurrTxRate = (PRTMP_TX_RATE_SWITCH) &pTable[(CurrRateIdx+1)*5];
#ifdef DOT11_N_SUPPORT
		if ((Rssi > -65)  && (pCurrTxRate->Mode >= MODE_HTMIX))
		{
			TrainUp		= (pCurrTxRate->TrainUp + (pCurrTxRate->TrainUp >> 1));
			TrainDown	= (pCurrTxRate->TrainDown + (pCurrTxRate->TrainDown >> 1));
		}
		else
#endif // DOT11_N_SUPPORT //
		if ((Rssi > -85) && (pCurrTxRate->Mode >= MODE_OFDM))  /* DynamicRateTuning Patch. */
		{
			TrainUp		= pCurrTxRate->TrainUp * 2;
			TrainDown	= pCurrTxRate->TrainDown * 2;
		}
		else
		{
			TrainUp		= pCurrTxRate->TrainUp;
			TrainDown	= pCurrTxRate->TrainDown;
		}

		
		pEntry->LastTimeTxRateChangeAction = pEntry->LastSecTxRateChangeAction;
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
 		//if (! OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_TX_RATE_SWITCH_ENABLED))
		//	continue;

		
        if (TxTotalCnt <= 15)
        {
   			CHAR	idx = 0;
			UCHAR	TxRateIdx;
			UCHAR	MCS0 = 0, MCS1 = 0, MCS2 = 0, MCS3 = 0, MCS4 = 0,  MCS5 =0, MCS6 = 0, MCS7 = 0;
#ifdef DOT11_N_SUPPORT
			UCHAR	MCS12 = 0, MCS13 = 0, MCS14 = 0, MCS15 = 0;
#endif // DOT11_N_SUPPORT //

			// check the existence and index of each needed MCS
			while (idx < pTable[0])
			{
				pCurrTxRate = (PRTMP_TX_RATE_SWITCH) &pTable[(idx+1)*5];

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
#ifdef DOT11_N_SUPPORT
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
				else if ((pCurrTxRate->CurrMCS == MCS_15) && (pCurrTxRate->ShortGI == GI_800))	//we hope to use ShortGI as initial rate
				{
					MCS15 = idx;
				}
#endif // DOT11_N_SUPPORT //
				
				idx ++;
			}
			if (pAd->LatchRfRegs.Channel <= 14)
			{
				if (pAd->NicConfig2.field.ExternalLNAForG)
				{
					RssiOffset = 2;
				}
				else
				{
					RssiOffset = 5;
				}
			}
#ifdef A_BAND_SUPPORT			
			else
			{
				if (pAd->NicConfig2.field.ExternalLNAForA)
				{
					RssiOffset = 5;
				}
				else
				{
					RssiOffset = 8;
				}
			}
#endif // A_BAND_SUPPORT //
#ifdef DOT11_N_SUPPORT
			if ((pTable == RateSwitchTable11BGN2S) || (pTable == RateSwitchTable11BGN2SForABand) || (pTable == RateSwitchTable11N2S) || (pTable == RateSwitchTable11N2SForABand) || (pTable == RateSwitchTable))
			{// N mode with 2 stream
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
			{// N mode with 1 stream
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
			else
#endif // DOT11_N_SUPPORT //
			{// Legacy mode
				if (MCS7 && (Rssi > -70))
				TxRateIdx = MCS7;
				else if (MCS6 && (Rssi > -74))
					TxRateIdx = MCS6;
				else if (MCS5 && (Rssi > -78))
					TxRateIdx = MCS5;
				else if (MCS4 && (Rssi > -82))
				TxRateIdx = MCS4;
				else if (MCS4 == 0)							// for B-only mode
					TxRateIdx = MCS3;
				else if (MCS3 && (Rssi > -85))
					TxRateIdx = MCS3;
				else if (MCS2 && (Rssi > -87))
					TxRateIdx = MCS2;
				else if (MCS1 && (Rssi > -90))
					TxRateIdx = MCS1;
				else
					TxRateIdx = MCS0;
			}

			if (TxRateIdx != pEntry->CurrTxRateIndex)
			{
				pEntry->CurrTxRateIndex = TxRateIdx;
				pNextTxRate = (PRTMP_TX_RATE_SWITCH) &pTable[(pEntry->CurrTxRateIndex+1)*5];
				APMlmeSetTxRate(pAd, pEntry, pNextTxRate);
			}

			NdisZeroMemory(pEntry->TxQuality, sizeof(USHORT) * MAX_STEP_OF_TX_RATE_SWITCH);
			NdisZeroMemory(pEntry->PER, sizeof(UCHAR) * MAX_STEP_OF_TX_RATE_SWITCH);
			pEntry->fLastSecAccordingRSSI = TRUE;


			// reset all OneSecTx counters
			RESET_ONE_SEC_TX_CNT(pEntry);

			continue;
        }

		if (pEntry->fLastSecAccordingRSSI == TRUE)
		{
			pEntry->fLastSecAccordingRSSI = FALSE;
			pEntry->LastSecTxRateChangeAction = 0;
			// reset all OneSecTx counters
			RESET_ONE_SEC_TX_CNT(pEntry);

			continue;
		}

		do
		{
			BOOLEAN	bTrainUpDown = FALSE;
			
			pEntry->CurrTxRateStableTime ++;

			// downgrade TX quality if PER >= Rate-Down threshold
			if (TxErrorRatio >= TrainDown)
			{
				bTrainUpDown = TRUE;
				pEntry->TxQuality[CurrRateIdx] = DRS_TX_QUALITY_WORST_BOUND;
			}
			// upgrade TX quality if PER <= Rate-Up threshold
			else if (TxErrorRatio <= TrainUp)
			{
				bTrainUpDown = TRUE;
				bUpgradeQuality = TRUE;
				if (pEntry->TxQuality[CurrRateIdx])
					pEntry->TxQuality[CurrRateIdx] --;  // quality very good in CurrRate

				if (pEntry->TxRateUpPenalty)
					pEntry->TxRateUpPenalty --;
				else if (pEntry->TxQuality[UpRateIdx])
					pEntry->TxQuality[UpRateIdx] --;    // may improve next UP rate's quality
			}

			pEntry->PER[CurrRateIdx] = (UCHAR)TxErrorRatio;

			if (bTrainUpDown == TRUE)
			{
				// perform DRS - consider TxRate Down first, then rate up.
				if ((CurrRateIdx != DownRateIdx) && (pEntry->TxQuality[CurrRateIdx] >= DRS_TX_QUALITY_WORST_BOUND))
				{
					pEntry->CurrTxRateIndex = DownRateIdx;
				}
				else if ((CurrRateIdx != UpRateIdx) && (pEntry->TxQuality[UpRateIdx] <= 0))
				{
					pEntry->CurrTxRateIndex = UpRateIdx;
				}
			}
		}while (FALSE);

		// if rate-up happen, clear all bad history of all TX rates
		if (pEntry->CurrTxRateIndex > CurrRateIdx)
		{
			pEntry->CurrTxRateStableTime = 0;
			pEntry->TxRateUpPenalty = 0;
			pEntry->LastSecTxRateChangeAction = 1; // rate UP
			NdisZeroMemory(pEntry->TxQuality, sizeof(USHORT) * MAX_STEP_OF_TX_RATE_SWITCH);
			NdisZeroMemory(pEntry->PER, sizeof(UCHAR) * MAX_STEP_OF_TX_RATE_SWITCH);

			//
			// For TxRate fast train up
			// 
#ifdef RTMP_MAC_PCI
			if (!pAd->ApCfg.ApQuickResponeForRateUpTimerRunning)
			{				
				RTMPSetTimer(&pAd->ApCfg.ApQuickResponeForRateUpTimer, DEF_QUICK_RA_TIME_INTERVAL); /* DynamicRateTuning Patch. */

				pAd->ApCfg.ApQuickResponeForRateUpTimerRunning = TRUE;
			}
#endif // RTMP_MAC_PCI //
		}
		// if rate-down happen, only clear DownRate's bad history
		else if (pEntry->CurrTxRateIndex < CurrRateIdx)
		{
			pEntry->CurrTxRateStableTime = 0;
			pEntry->TxRateUpPenalty = 0;           // no penalty
			pEntry->LastSecTxRateChangeAction = 2; // rate DOWN
			pEntry->TxQuality[pEntry->CurrTxRateIndex] = 0;
			pEntry->PER[pEntry->CurrTxRateIndex] = 0;

			//
			// For TxRate fast train down
			// 
#ifdef RTMP_MAC_PCI
			if (!pAd->ApCfg.ApQuickResponeForRateUpTimerRunning)
			{
				RTMPSetTimer(&pAd->ApCfg.ApQuickResponeForRateUpTimer, DEF_QUICK_RA_TIME_INTERVAL); /* DynamicRateTuning Patch. */

				pAd->ApCfg.ApQuickResponeForRateUpTimerRunning = TRUE;
			}
#endif // RTMP_MAC_PCI //
		}
		else
		{
			pEntry->LastSecTxRateChangeAction = 0; // rate no change
			bTxRateChanged = FALSE;
		}
			
		if (pAd->MacTab.Size == 1)
		{
			//test by gary 
       		//pEntry->LastTxOkCount = pAd->RalinkCounters.OneSecTxNoRetryOkCount;
			pEntry->LastTxOkCount = TxSuccess;
		}
		else
		{
			pEntry->LastTxOkCount = pEntry->OneSecTxNoRetryOkCount;
		}

		// reset all OneSecTx counters
		RESET_ONE_SEC_TX_CNT(pEntry);

		pNextTxRate = (PRTMP_TX_RATE_SWITCH) &pTable[(pEntry->CurrTxRateIndex+1)*5];
		if (bTxRateChanged && pNextTxRate)
		{
			APMlmeSetTxRate(pAd, pEntry, pNextTxRate);
		}
	}
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
VOID APQuickResponeForRateUpExec(
    IN PVOID SystemSpecific1, 
    IN PVOID FunctionContext, 
    IN PVOID SystemSpecific2, 
    IN PVOID SystemSpecific3) 
{
	PRTMP_ADAPTER			pAd = (PRTMP_ADAPTER)FunctionContext;
	ULONG					i;
	PUCHAR					pTable;
	UCHAR					TableSize = 0;
	UCHAR					UpRateIdx, DownRateIdx, CurrRateIdx;
	ULONG					AccuTxTotalCnt, TxTotalCnt, TxCnt;
	ULONG					TxErrorRatio = 0;
	MAC_TABLE_ENTRY			*pEntry;
	PRTMP_TX_RATE_SWITCH	pCurrTxRate, pNextTxRate = NULL;
	BOOLEAN					bTxRateChanged = TRUE;
	UCHAR					InitTxRateIdx, TrainUp, TrainDown;
	TX_STA_CNT1_STRUC		StaTx1;
	TX_STA_CNT0_STRUC		TxStaCnt0;
	CHAR					Rssi, ratio;
	ULONG					TxRetransmit = 0, TxSuccess = 0, TxFailCount = 0;

	pAd->ApCfg.ApQuickResponeForRateUpTimerRunning = FALSE;
	
    //
    // walk through MAC table, see if need to change AP's TX rate toward each entry
    //
   	for (i = 1; i < MAX_LEN_OF_MAC_TABLE; i++) 
	{
        pEntry = &pAd->MacTab.Content[i];


		if (IS_ENTRY_NONE(pEntry))
			continue;

		if (IS_ENTRY_CLIENT(pEntry) && (pEntry->Sst != SST_ASSOC))
			continue;

#ifdef APCLI_SUPPORT
		if (IS_ENTRY_APCLI(pEntry) && (pEntry->Sst != SST_ASSOC))
			continue;
#endif // APCLI_SUPPORT //

#ifdef WDS_SUPPORT
		if (IS_ENTRY_WDS(pEntry) && !WDS_IF_UP_CHECK(pAd, pEntry->MatchWDSTabIdx))
			continue;
#endif // WDS_SUPPORT //


    	Rssi = RTMPMaxRssi(pAd, (CHAR)pEntry->RssiSample.AvgRssi0, (CHAR)pEntry->RssiSample.AvgRssi1, (CHAR)pEntry->RssiSample.AvgRssi2);

		CurrRateIdx = UpRateIdx = DownRateIdx = pEntry->CurrTxRateIndex;


		if (pAd->MacTab.Size == 1)
		{
			// Update statistic counter
			RTMP_IO_READ32(pAd, TX_STA_CNT0, &TxStaCnt0.word);
			RTMP_IO_READ32(pAd, TX_STA_CNT1, &StaTx1.word);
			TxRetransmit = StaTx1.field.TxRetransmit;
			TxSuccess = StaTx1.field.TxSuccess;
			TxFailCount = TxStaCnt0.field.TxFailCount;
			TxTotalCnt = TxRetransmit + TxSuccess + TxFailCount;

			pAd->RalinkCounters.OneSecBeaconSentCnt += TxStaCnt0.field.TxBeaconCount;
			pAd->RalinkCounters.OneSecTxRetryOkCount += StaTx1.field.TxRetransmit;
			pAd->RalinkCounters.OneSecTxNoRetryOkCount += StaTx1.field.TxSuccess;
			pAd->RalinkCounters.OneSecTxFailCount += TxStaCnt0.field.TxFailCount;

#ifdef STATS_COUNT_SUPPORT
			pAd->WlanCounters.TransmittedFragmentCount.u.LowPart += StaTx1.field.TxSuccess;
			pAd->WlanCounters.RetryCount.u.LowPart += StaTx1.field.TxRetransmit;
			pAd->WlanCounters.FailedCount.u.LowPart += TxStaCnt0.field.TxFailCount;
#endif // STATS_COUNT_SUPPORT //

			AccuTxTotalCnt = pAd->RalinkCounters.OneSecTxNoRetryOkCount + 
					 pAd->RalinkCounters.OneSecTxRetryOkCount + 
					 pAd->RalinkCounters.OneSecTxFailCount;

			if (TxTotalCnt)
				TxErrorRatio = ((TxRetransmit + TxFailCount) * 100) / TxTotalCnt;

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
		}

		// decide the rate table for tuning
		APMlmeSelectTxRateTable(pAd, pEntry, &pTable, &TableSize, &InitTxRateIdx);

		// decide the next upgrade rate and downgrade rate, if any
		if ((CurrRateIdx > 0) && (CurrRateIdx < (TableSize - 1)))
		{
			UpRateIdx = CurrRateIdx + 1;
			DownRateIdx = CurrRateIdx -1;
		}
		else if (CurrRateIdx == 0)
		{
			UpRateIdx = CurrRateIdx + 1;
			DownRateIdx = CurrRateIdx;
		}
		else if (CurrRateIdx == (TableSize - 1))
		{
			UpRateIdx = CurrRateIdx;
			DownRateIdx = CurrRateIdx - 1;
		}

		pCurrTxRate = (PRTMP_TX_RATE_SWITCH) &pTable[(CurrRateIdx+1)*5];

#ifdef DOT11_N_SUPPORT
		if ((Rssi > -65) && (pCurrTxRate->Mode >= MODE_HTMIX))
		{
			TrainUp		= (pCurrTxRate->TrainUp + (pCurrTxRate->TrainUp >> 1));
			TrainDown	= (pCurrTxRate->TrainDown + (pCurrTxRate->TrainDown >> 1));
		}
		else
#endif // DOT11_N_SUPPORT //
		{
			TrainUp		= pCurrTxRate->TrainUp;
			TrainDown	= pCurrTxRate->TrainDown;
		}


//		if (! OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_TX_RATE_SWITCH_ENABLED))
//			continue;

        if (/*TxTotalCnt*/ TxCnt <= 15)
        {

			// perform DRS - consider TxRate Down first, then rate up.
			if ((pEntry->LastSecTxRateChangeAction == 1) && (CurrRateIdx != DownRateIdx))
			{
				pEntry->CurrTxRateIndex = DownRateIdx;
				pEntry->TxQuality[CurrRateIdx] = DRS_TX_QUALITY_WORST_BOUND;
			}
			else if ((pEntry->LastSecTxRateChangeAction == 2) && (CurrRateIdx != UpRateIdx))
			{
				pEntry->CurrTxRateIndex = UpRateIdx;
			}
			
			continue;
        }

		do
		{
			ULONG		OneSecTxNoRetryOKRationCount;

			// test by gary
			//if (pEntry->LastSecTxRateChangeAction == 0)
			if (pEntry->LastTimeTxRateChangeAction == 0)
				ratio = 5;
			else
				ratio = 4;

			if (DEF_QUICK_RA_TIME_INTERVAL == 50) /* DynamicRateTuning Patch. */
				ratio *=2;

			// downgrade TX quality if PER >= Rate-Down threshold
			if (TxErrorRatio >= TrainDown)
			{
				pEntry->TxQuality[CurrRateIdx] = DRS_TX_QUALITY_WORST_BOUND;
			}

			pEntry->PER[CurrRateIdx] = (UCHAR)TxErrorRatio;

			if (pAd->MacTab.Size == 1)
			{
   				//OneSecTxNoRetryOKRationCount = pAd->RalinkCounters.OneSecTxNoRetryOkCount * ratio + (pAd->RalinkCounters.OneSecTxNoRetryOkCount >> 1);
				// test by gary
				OneSecTxNoRetryOKRationCount = (TxSuccess * ratio);
			}
			else
			{
				OneSecTxNoRetryOKRationCount = pEntry->OneSecTxNoRetryOkCount * ratio + (pEntry->OneSecTxNoRetryOkCount >> 1);
			}

			// perform DRS - consider TxRate Down first, then rate up.
			if ((pEntry->LastSecTxRateChangeAction == 1) && (CurrRateIdx != DownRateIdx))
			{
				//if ((pEntry->LastTxOkCount + 2) >= (pEntry->OneSecTxNoRetryOkCount * ratio + (pEntry->OneSecTxNoRetryOkCount >> 1)))
#if 0 /* DynamicRateTuning Patch. */
				if ((pEntry->LastTxOkCount + 2) >= OneSecTxNoRetryOKRationCount)
#else
				if (TxErrorRatio >= TrainDown)
#endif
				{
					pEntry->CurrTxRateIndex = DownRateIdx;
					pEntry->TxQuality[CurrRateIdx] = DRS_TX_QUALITY_WORST_BOUND;
					pEntry->CurrTxRateIndex = DownRateIdx; /* DynamicRateTuning Patch. */
				}
				else
				{
					//DBGPRINT_RAW(RT_DEBUG_TRACE,("QuickDRS: (Up) keep rate-up (L:%ld, C:%d)\n", pEntry->LastTxOkCount, (pEntry->OneSecTxNoRetryOkCount * ratio + (pEntry->OneSecTxNoRetryOkCount >> 1))));
					DBGPRINT_RAW(RT_DEBUG_INFO,("QuickDRS: (Up) keep rate-up (L:%ld, C:%ld)\n", pEntry->LastTxOkCount, OneSecTxNoRetryOKRationCount));
				}
			}
			else if ((pEntry->LastSecTxRateChangeAction == 2) && (CurrRateIdx != UpRateIdx))
			{
				//if ((TxErrorRatio >= 50) || (TxErrorRatio >= TrainDown))
				if ((TxErrorRatio >= 50) && (TxErrorRatio >= TrainDown))
				{
					
				}
				//else if ((pEntry->LastTxOkCount + 2) >= (pEntry->OneSecTxNoRetryOkCount * ratio + (pEntry->OneSecTxNoRetryOkCount >> 1)))
				else if ((pEntry->LastTxOkCount + 2) >= OneSecTxNoRetryOKRationCount)
				{
					pEntry->CurrTxRateIndex = UpRateIdx;
					
				}
				
			}
		}while (FALSE);

		// if rate-up happen, clear all bad history of all TX rates
		if (pEntry->CurrTxRateIndex > CurrRateIdx)
		{
			
			pEntry->TxRateUpPenalty = 0;
			NdisZeroMemory(pEntry->TxQuality, sizeof(USHORT) * MAX_STEP_OF_TX_RATE_SWITCH);
			NdisZeroMemory(pEntry->PER, sizeof(UCHAR) * MAX_STEP_OF_TX_RATE_SWITCH);
		}
		// if rate-down happen, only clear DownRate's bad history
		else if (pEntry->CurrTxRateIndex < CurrRateIdx)
		{
			
			pEntry->TxRateUpPenalty = 0;           // no penalty
			pEntry->TxQuality[pEntry->CurrTxRateIndex] = 0;
			pEntry->PER[pEntry->CurrTxRateIndex] = 0;
		}
		else
		{
			bTxRateChanged = FALSE;
		}

		pNextTxRate = (PRTMP_TX_RATE_SWITCH) &pTable[(pEntry->CurrTxRateIndex+1)*5];
		if (bTxRateChanged && pNextTxRate)
		{
			APMlmeSetTxRate(pAd, pEntry, pNextTxRate);
		}
    }
}

/*! \brief   To substitute the message type if the message is coming from external
 *  \param  *Fr            The frame received
 *  \param  *Machine       The state machine
 *  \param  *MsgType       the message type for the state machine
 *  \return TRUE if the substitution is successful, FALSE otherwise
 *  \pre
 *  \post
 */
BOOLEAN APMsgTypeSubst(
    IN PRTMP_ADAPTER pAd,
    IN PFRAME_802_11 pFrame, 
    OUT INT *Machine, 
    OUT INT *MsgType) 
{
    USHORT Seq;
    UCHAR  EAPType;
    BOOLEAN     Return = FALSE;
#ifdef WSC_AP_SUPPORT
	UCHAR EAPCode;
    PMAC_TABLE_ENTRY pEntry;
#endif // WSC_AP_SUPPORT //

//TODO:
// only PROBE_REQ can be broadcast, all others must be unicast-to-me && is_mybssid; otherwise, 
// ignore this frame

    // wpa EAPOL PACKET
    if (pFrame->Hdr.FC.Type == BTYPE_DATA) 
    {    
#ifdef WSC_AP_SUPPORT    
        //WSC EAPOL PACKET        
        pEntry = MacTableLookup(pAd, pFrame->Hdr.Addr2);
        if ((!MAC_ADDR_EQUAL(pAd->ApCfg.MBSSID[pEntry->apidx].WscControl.EntryAddr, ZERO_MAC_ADDR)) &&
            pEntry && 
            IS_ENTRY_CLIENT(pEntry) && 
            (pAd->ApCfg.MBSSID[pEntry->apidx].WscControl.WscConfMode != WSC_DISABLE))
        {
            *Machine = WSC_STATE_MACHINE;
            EAPType = *((UCHAR*)pFrame + LENGTH_802_11 + LENGTH_802_1_H + 1);
            EAPCode = *((UCHAR*)pFrame + LENGTH_802_11 + LENGTH_802_1_H + 4);
            Return = WscMsgTypeSubst(EAPType, EAPCode, MsgType);
        }
        if (!Return)
        {
#endif // WSC_AP_SUPPORT //
	        *Machine = WPA_STATE_MACHINE;
        EAPType = *((UCHAR*)pFrame + LENGTH_802_11 + LENGTH_802_1_H + 1);
	        Return = WpaMsgTypeSubst(EAPType, (INT *) MsgType);
#ifdef WSC_AP_SUPPORT            
        }
#endif // WSC_AP_SUPPORT //
        return Return;
    }
    
    if (pFrame->Hdr.FC.Type != BTYPE_MGMT)
        return FALSE;
    
    switch (pFrame->Hdr.FC.SubType) 
    {
        case SUBTYPE_ASSOC_REQ:
            *Machine = AP_ASSOC_STATE_MACHINE;
            *MsgType = APMT2_PEER_ASSOC_REQ;
            
            break;
//      case SUBTYPE_ASSOC_RSP:
//          *Machine = AP_ASSOC_STATE_MACHINE;
//          *MsgType = APMT2_PEER_ASSOC_RSP;
//          break;
        case SUBTYPE_REASSOC_REQ:
            *Machine = AP_ASSOC_STATE_MACHINE;
            *MsgType = APMT2_PEER_REASSOC_REQ;
            break;
//      case SUBTYPE_REASSOC_RSP:
//          *Machine = AP_ASSOC_STATE_MACHINE;
//          *MsgType = APMT2_PEER_REASSOC_RSP;
//          break;
        case SUBTYPE_PROBE_REQ:
            *Machine = AP_SYNC_STATE_MACHINE;              
            *MsgType = APMT2_PEER_PROBE_REQ;
            break;
// test for 40Mhz intolerant
		/*
			For Active Scan
		*/
		case SUBTYPE_PROBE_RSP:
          *Machine = AP_SYNC_STATE_MACHINE;
          *MsgType = APMT2_PEER_PROBE_RSP;
          break;
        case SUBTYPE_BEACON:
            *Machine = AP_SYNC_STATE_MACHINE;
            *MsgType = APMT2_PEER_BEACON;
            break;
//      case SUBTYPE_ATIM:
//          *Machine = AP_SYNC_STATE_MACHINE;
//          *MsgType = APMT2_PEER_ATIM;
//          break;
        case SUBTYPE_DISASSOC:
            *Machine = AP_ASSOC_STATE_MACHINE;
            *MsgType = APMT2_PEER_DISASSOC_REQ;
            break;
        case SUBTYPE_AUTH:
            // get the sequence number from payload 24 Mac Header + 2 bytes algorithm
            NdisMoveMemory(&Seq, &pFrame->Octet[2], sizeof(USHORT));
            
			*Machine = AP_AUTH_STATE_MACHINE;
			if (Seq == 1)
				*MsgType = APMT2_PEER_AUTH_REQ;
			else if (Seq == 3)
				*MsgType = APMT2_PEER_AUTH_CONFIRM;
            else 
            {
                DBGPRINT(RT_DEBUG_TRACE,("wrong AUTH seq=%d Octet=%02x %02x %02x %02x %02x %02x %02x %02x\n", Seq,
                    pFrame->Octet[0], pFrame->Octet[1], pFrame->Octet[2], pFrame->Octet[3], 
                    pFrame->Octet[4], pFrame->Octet[5], pFrame->Octet[6], pFrame->Octet[7]));
                return FALSE;
            }
            break;
        case SUBTYPE_DEAUTH:
            *Machine = AP_AUTH_STATE_MACHINE; /*AP_AUTH_RSP_STATE_MACHINE;*/
            *MsgType = APMT2_PEER_DEAUTH;
            break;
	case SUBTYPE_ACTION:
		*Machine = ACTION_STATE_MACHINE;
		//  Sometimes Sta will return with category bytes with MSB = 1, if they receive catogory out of their support
		if ((pFrame->Octet[0]&0x7F) > MAX_PEER_CATE_MSG) 
		{
			*MsgType = MT2_ACT_INVALID;
		} 
		else
		{
			*MsgType = (pFrame->Octet[0]&0x7F);
		} 
		break;
        default:
            return FALSE;
            break;
    }

    return TRUE;
}



/*
    ========================================================================
    Routine Description:
        Periodic evaluate antenna link status
        
    Arguments:
        pAd         - Adapter pointer
        
    Return Value:
        None
        
    ========================================================================
*/
VOID APAsicEvaluateRxAnt(
	IN PRTMP_ADAPTER	pAd)
{
	UCHAR	BBPR3 = 0;
	ULONG	TxTotalCnt;

#ifdef RALINK_ATE
	if (ATE_ON(pAd))
		return;
#endif // RALINK_ATE //
#ifdef CARRIER_DETECTION_SUPPORT
	if(pAd->CommonCfg.CarrierDetect.CD_State == CD_SILENCE)
	return;
#endif // CARRIER_DETECTION_SUPPORT //
	
	
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BBPR3);
	BBPR3 &= (~0x18);
	if(pAd->Antenna.field.RxPath == 3 && pAd->CommonCfg.bBlockAntDivforGreenAP == FALSE)
	{
		BBPR3 |= (0x10);
	}
	else if(pAd->Antenna.field.RxPath == 2 && pAd->CommonCfg.bBlockAntDivforGreenAP == FALSE)
	{
		BBPR3 |= (0x8);
	}
	else if(pAd->Antenna.field.RxPath == 1)
	{
		BBPR3 |= (0x0);
	}
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BBPR3);

	TxTotalCnt = pAd->RalinkCounters.OneSecTxNoRetryOkCount + 
					pAd->RalinkCounters.OneSecTxRetryOkCount + 
					pAd->RalinkCounters.OneSecTxFailCount;

	if (TxTotalCnt > 50)
	{
		RTMPSetTimer(&pAd->Mlme.RxAntEvalTimer, 20);
		pAd->Mlme.bLowThroughput = FALSE;
	}
	else
	{
		RTMPSetTimer(&pAd->Mlme.RxAntEvalTimer, 300);
		pAd->Mlme.bLowThroughput = TRUE;
	}
}

/*
    ========================================================================
    Routine Description:
        After evaluation, check antenna link status
        
    Arguments:
        pAd         - Adapter pointer
        
    Return Value:
        None
        
    ========================================================================
*/
VOID APAsicRxAntEvalTimeout(
	PRTMP_ADAPTER	pAd) 
{
	UCHAR			BBPR3 = 0;
	CHAR			larger = -127, rssi0, rssi1, rssi2;

#ifdef RALINK_ATE
	if (ATE_ON(pAd))
		return;
#endif // RALINK_ATE //

	// if the traffic is low, use average rssi as the criteria
	if (pAd->Mlme.bLowThroughput == TRUE)
	{
		rssi0 = pAd->ApCfg.RssiSample.LastRssi0;
		rssi1 = pAd->ApCfg.RssiSample.LastRssi1;
		rssi2 = pAd->ApCfg.RssiSample.LastRssi2;
	}
	else
	{
		rssi0 = pAd->ApCfg.RssiSample.AvgRssi0;
		rssi1 = pAd->ApCfg.RssiSample.AvgRssi1;
		rssi2 = pAd->ApCfg.RssiSample.AvgRssi2;
	}

	if(pAd->Antenna.field.RxPath == 3)
	{
		larger = max(rssi0, rssi1);
		{
		if (larger > (rssi2 + 20))
			pAd->Mlme.RealRxPath = 2;
		else
			pAd->Mlme.RealRxPath = 3;
		}
	}
	// Disable the below to fix 1T/2R issue. It's suggested by Rory at 2007/7/11.

	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BBPR3);
	BBPR3 &= (~0x18);
	if(pAd->Mlme.RealRxPath == 3 && pAd->CommonCfg.bBlockAntDivforGreenAP == FALSE)
	{
		BBPR3 |= (0x10);
	}
	else if(pAd->Mlme.RealRxPath == 2 && pAd->CommonCfg.bBlockAntDivforGreenAP == FALSE)
	{
		BBPR3 |= (0x8);
	}
	else if(pAd->Mlme.RealRxPath == 1)
	{
		BBPR3 |= (0x0);
	}
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BBPR3);
	
}

/*
    ========================================================================
    Routine Description:
        After evaluation, check antenna link status
        
    Arguments:
        pAd         - Adapter pointer
        
    Return Value:
        None
        
    ========================================================================
*/
VOID	APAsicAntennaAvg(
	IN	PRTMP_ADAPTER	pAd,
	IN	UCHAR	              AntSelect,
	IN	SHORT*	              RssiAvg)  
{
		    SHORT	realavgrssi;
		    LONG         realavgrssi1, realavgrssi2;
		    ULONG	recvPktNum = pAd->RxAnt.RcvPktNum[AntSelect];

		    realavgrssi1 = pAd->RxAnt.Pair1AvgRssiGroup1[AntSelect];

		    if(realavgrssi1 == 0)
		    {      
		        *RssiAvg = 0;
		        return;
		    }

		    realavgrssi = (SHORT) (realavgrssi1 / recvPktNum);

		    pAd->RxAnt.Pair1AvgRssiGroup1[0] = 0;
		    pAd->RxAnt.Pair1AvgRssiGroup1[1] = 0;
		    pAd->RxAnt.Pair1AvgRssiGroup2[0] = 0;
		    pAd->RxAnt.Pair1AvgRssiGroup2[1] = 0;
		    pAd->RxAnt.RcvPktNum[0] = 0;
		    pAd->RxAnt.RcvPktNum[1] = 0;
		    *RssiAvg = realavgrssi - 256;
}
/* End of ap_mlme.c */
