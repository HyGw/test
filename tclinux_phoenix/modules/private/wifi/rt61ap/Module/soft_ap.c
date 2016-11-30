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
	soft_ap.c

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	John		2004-08-08		Major modification from RT2560
*/
#include "rt_config.h"

#define NULL_FRAME_SW_QUEUE_SUPPORT		1

char const *pEventText[EVENT_MAX_EVENT_TYPE] = {
    "restart access point",
    "successfully associated",
    "has disassociated",
    "has been aged-out and disassociated" ,    
    "active countermeasures",
    "has disassociated with invalid PSK password"};
//added by xyzhu_nj_20091208,to show if wifi mac table is changed.
int gMacTabChanged = 0;

VOID ApSendNullFrame(
	IN PRTMP_ADAPTER pAd,
	IN ULONG ifType,
	IN UCHAR ifIndex,
	IN PVOID pBuffer,
	IN ULONG Length,
	IN UCHAR TxRate,
	IN UCHAR PID);


/*
    ==========================================================================
    Description:
        Initialize AP specific data
    ==========================================================================
 */
NDIS_STATUS ApInitialize(
	IN	PRTMP_ADAPTER	pAd)
{
    UCHAR       apidx;

    DBGPRINT(RT_DEBUG_TRACE, "---> ApInitialize\n");

    // Initialize MAC table and allocate spin lock
    NdisZeroMemory(&pAd->MacTab, sizeof(MAC_TABLE));
    NdisAllocateSpinLock(&pAd->MacTabLock);

    for (apidx = 0; apidx < pAd->PortCfg.BssidNum; apidx++)
    {
        // Init TKIP Group-Key-related variables
        GenRandom(pAd,pAd->PortCfg.MBSSID[apidx].GMK, apidx);
        GenRandom(pAd,pAd->PortCfg.MBSSID[apidx].GNonce, apidx);

        pAd->PortCfg.MBSSID[apidx].PortSecured = WPA_802_1X_PORT_NOT_SECURED;
    }

#ifdef IGMP_SNOOP_SUPPORT
	MulticastFilterTableInit(&pAd->pMulticastFilterTable);
#endif // IGMP_SNOOP_SUPPORT //

#ifdef BLOCK_NET_IF
	initblockQueueTab();

	{
		int i;
		for (i=0; i < NUM_OF_TX_RING; i++)
			initList(&pAd->blockQueueTab[i].NetIfList);
	}
#endif // BLOCK_NET_IF //

    // Init Group key update timer, and countermeasures timer
	RTMPInitTimer(pAd, &pAd->PortCfg.REKEYTimer, (PVOID)&GREKEYPeriodicExec);
	RTMPInitTimer(pAd, &pAd->PortCfg.CounterMeasureTimer, (PVOID)&CMTimerExec);
	RTMPInitTimer(pAd, &pAd->QuickResponeForRateUpTimer, (PVOID)&QuickResponeForRateUp);

#ifdef TC_SOC		// Add polling mode support
	RTMPInitTimer(pAd, &pAd->PollingModeDetect, (PVOID)&PeriodicPollingModeDetect);
      pAd->PollingModeDetectRunning = FALSE;
#endif 

    DBGPRINT(RT_DEBUG_TRACE, "<--- ApInitialize\n");
    
	return NDIS_STATUS_SUCCESS;
}

/*
    ==========================================================================
    Description:
        Shutdown AP and free AP specific resources
    ==========================================================================
 */
VOID ApShutdown(
    IN PRTMP_ADAPTER pAd)
{
    DBGPRINT(RT_DEBUG_TRACE, "---> ApShutdown\n");

    ApStop(pAd, TRUE);

#ifdef IGMP_SNOOP_SUPPORT
	MultiCastFilterTableReset(&pAd->pMulticastFilterTable);
#endif // IGMP_SNOOP_SUPPORT //

    DBGPRINT(RT_DEBUG_TRACE, "<--- ApShutdown\n");
}

VOID ApStartUp(
    IN PRTMP_ADAPTER	pAd,
    IN BOOLEAN			bDFSRestart)
{
	UCHAR       apidx, i;
	ULONG        sec_csr4 = 0, MacCsr0 = 0;
	BOOLEAN		bWmmCapable = FALSE;
#ifdef APCLI_SUPPORT
	UCHAR		apcliidx;
#endif

    DBGPRINT(RT_DEBUG_TRACE, "---> ApStartUp\n");

    for (apidx = 0; apidx < pAd->PortCfg.BssidNum; apidx++)
    {
        if ((pAd->PortCfg.MBSSID[apidx].SsidLen <= 0) || (pAd->PortCfg.MBSSID[apidx].SsidLen > MAX_LEN_OF_SSID))
            return;
        
        // when B-only mode, beacon will not report using short slot time
        if (pAd->PortCfg.PhyMode == PHY_11B)
        {
        	pAd->PortCfg.UseShortSlotTime = FALSE;
        }
        
        // Decide the Capability information field
        // In IEEE Std 802.1h-2003, the spectrum management bit is enabled in the 5 GHz band 
        if (pAd->PortCfg.PhyMode == PHY_11A && pAd->PortCfg.RadarDetect.IEEE80211H == TRUE)
        	pAd->PortCfg.MBSSID[apidx].CapabilityInfo = CAP_GENERATE((pAd->PortCfg.MBSSID[apidx].WepStatus != Ndis802_11EncryptionDisabled), (pAd->PortCfg.TxPreamble == Rt802_11PreambleShort), pAd->PortCfg.UseShortSlotTime, TRUE);
        else
        	pAd->PortCfg.MBSSID[apidx].CapabilityInfo = CAP_GENERATE((pAd->PortCfg.MBSSID[apidx].WepStatus != Ndis802_11EncryptionDisabled), (pAd->PortCfg.TxPreamble == Rt802_11PreambleShort), pAd->PortCfg.UseShortSlotTime, FALSE);

		// if one bssid turn on the WMM, the parameter will set to ASIC once.
        if (pAd->PortCfg.MBSSID[apidx].bWmmCapable)
        	bWmmCapable = TRUE;

        DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) CapabilityInfo=%x, WepStatus=%d\n", apidx,
            pAd->PortCfg.MBSSID[apidx].CapabilityInfo,pAd->PortCfg.MBSSID[apidx].WepStatus);

        // if AuthMode >= WPA and IEEE802.1x is enabled, then pairwise key table must be used; 
	    // if legacy WEP inused, then only shared key table is used
	    if ((pAd->PortCfg.MBSSID[apidx].AuthMode >= Ndis802_11AuthModeWPA) ||(pAd->PortCfg.MBSSID[apidx].IEEE8021X == TRUE))
	    {
	    	sec_csr4 |= BIT32[apidx];
	        DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d)-AP AuthMode=%d, IEEE802.1x is %d, Pairwise Key Table in-used\n", apidx, 
	        																		pAd->PortCfg.MBSSID[apidx].AuthMode, 
	        																		pAd->PortCfg.MBSSID[apidx].IEEE8021X);
	    }
	    else
	    {
	        DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d)-AP AuthMode=%d, disable Pairwise Key Table\n", apidx, pAd->PortCfg.MBSSID[apidx].AuthMode);
	    }
	    RTMP_IO_WRITE32(pAd, SEC_CSR4, sec_csr4);

	    // remove group key table
    	for (i=0; i<SHARE_KEY_NUM; i++)
    	{
			AsicRemoveSharedKeyEntry(pAd, apidx, i);
    	}

    	// remove pairwise key table
    	for (i=0; i<PAIRWISE_KEY_NO; i++)
    	{
			AsicRemovePairwiseKeyEntry(pAd, i);
    	}

		// Set shared key to ASIC :
		// SHARED-WEP, Auto-WEP, OPEN-WEP(no matter 802.1x is enabled or disabled) need to set key
    	if ((pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeShared) ||
    		(((pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeAutoSwitch) || (pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeOpen)) && (pAd->PortCfg.MBSSID[apidx].WepStatus == Ndis802_11Encryption1Enabled)))
    	{
    		for (i=0; i<SHARE_KEY_NUM; i++)
	    	{
	    		if (pAd->PortCfg.MBSSID[apidx].SharedKey[i].KeyLen != 0)
	    		{
                    AsicAddSharedKeyEntry(pAd, apidx, i, pAd->PortCfg.MBSSID[apidx].SharedKey[i].CipherAlg, pAd->PortCfg.MBSSID[apidx].SharedKey[i].Key, NULL, NULL);
	    		}
	    	}
    	}

        // Add wds key to pairwise key table when TKIP/AES
    	if (((pAd->WdsTab.WepStatus == Ndis802_11Encryption2Enabled) || (pAd->WdsTab.WepStatus == Ndis802_11Encryption3Enabled)) && (pAd->WdsTab.Wpa_key.KeyLen > 0))
    	{
			AsicAddPairwiseKeyEntry(pAd, pAd->WdsTab.WdsEntry[0].WdsAddr, WDS_PAIRWISE_KEY_OFFSET, pAd->WdsTab.Wpa_key.CipherAlg, 
				pAd->WdsTab.Wpa_key.Key, pAd->WdsTab.Wpa_key.TxMic, pAd->WdsTab.Wpa_key.RxMic);
			AsicAddPairwiseKeyEntry(pAd, pAd->WdsTab.WdsEntry[1].WdsAddr, WDS_PAIRWISE_KEY_OFFSET + 1, pAd->WdsTab.Wpa_key.CipherAlg,	
				pAd->WdsTab.Wpa_key.Key, pAd->WdsTab.Wpa_key.TxMic, pAd->WdsTab.Wpa_key.RxMic);
			AsicAddPairwiseKeyEntry(pAd, pAd->WdsTab.WdsEntry[2].WdsAddr, WDS_PAIRWISE_KEY_OFFSET + 2, pAd->WdsTab.Wpa_key.CipherAlg,	
				pAd->WdsTab.Wpa_key.Key, pAd->WdsTab.Wpa_key.TxMic, pAd->WdsTab.Wpa_key.RxMic);
			AsicAddPairwiseKeyEntry(pAd, pAd->WdsTab.WdsEntry[3].WdsAddr, WDS_PAIRWISE_KEY_OFFSET + 3, pAd->WdsTab.Wpa_key.CipherAlg,	
				pAd->WdsTab.Wpa_key.Key, pAd->WdsTab.Wpa_key.TxMic, pAd->WdsTab.Wpa_key.RxMic);
    	}

		// Send singal to daemon to indicate driver had restarted
	    if ((pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPA) || (pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPA2)
        	|| (pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPA1WPA2) || (pAd->PortCfg.MBSSID[apidx].IEEE8021X == TRUE))
        {
        	SendSingalToDaemon(pAd, SIGUSR1, pAd->ApdPid);
    	}
    }

#ifdef WMM_SUPPORT
    if (bWmmCapable)
    {
        // EDCA parameters used for AP's own transmission
        if (pAd->PortCfg.APEdcaParm.bValid == FALSE)
        {
            pAd->PortCfg.APEdcaParm.bValid = TRUE;
            pAd->PortCfg.APEdcaParm.Aifsn[0] = 3;
            pAd->PortCfg.APEdcaParm.Aifsn[1] = 7;
            pAd->PortCfg.APEdcaParm.Aifsn[2] = 1;
            pAd->PortCfg.APEdcaParm.Aifsn[3] = 1;
                              
            pAd->PortCfg.APEdcaParm.Cwmin[0] = 4;
            pAd->PortCfg.APEdcaParm.Cwmin[1] = 4;
            pAd->PortCfg.APEdcaParm.Cwmin[2] = 3;
            pAd->PortCfg.APEdcaParm.Cwmin[3] = 2;
                                    
            pAd->PortCfg.APEdcaParm.Cwmax[0] = 10;
            pAd->PortCfg.APEdcaParm.Cwmax[1] = 6;
            pAd->PortCfg.APEdcaParm.Cwmax[2] = 4;
            pAd->PortCfg.APEdcaParm.Cwmax[3] = 3;
        
            pAd->PortCfg.APEdcaParm.Txop[0]  = 0;
            pAd->PortCfg.APEdcaParm.Txop[1]  = 0;
            pAd->PortCfg.APEdcaParm.Txop[2]  = 94;
            pAd->PortCfg.APEdcaParm.Txop[3]  = 47;
        }
        AsicSetEdcaParm(pAd, &pAd->PortCfg.APEdcaParm);

        // EDCA parameters to be annouced in outgoing BEACON, used by WMM STA
        if (pAd->PortCfg.BssEdcaParm.bValid == FALSE)
        {
            pAd->PortCfg.BssEdcaParm.bValid = TRUE;
            pAd->PortCfg.BssEdcaParm.Aifsn[0] = 3;
            pAd->PortCfg.BssEdcaParm.Aifsn[1] = 7;
            pAd->PortCfg.BssEdcaParm.Aifsn[2] = 2;
            pAd->PortCfg.BssEdcaParm.Aifsn[3] = 2;
                              
            pAd->PortCfg.BssEdcaParm.Cwmin[0] = 4;
            pAd->PortCfg.BssEdcaParm.Cwmin[1] = 4;
            pAd->PortCfg.BssEdcaParm.Cwmin[2] = 3;
            pAd->PortCfg.BssEdcaParm.Cwmin[3] = 2;
                                    
            pAd->PortCfg.BssEdcaParm.Cwmax[0] = 10;
            pAd->PortCfg.BssEdcaParm.Cwmax[1] = 10;
            pAd->PortCfg.BssEdcaParm.Cwmax[2] = 4;
            pAd->PortCfg.BssEdcaParm.Cwmax[3] = 3;
        
            pAd->PortCfg.BssEdcaParm.Txop[0]  = 0;
            pAd->PortCfg.BssEdcaParm.Txop[1]  = 0;
            pAd->PortCfg.BssEdcaParm.Txop[2]  = 94;
            pAd->PortCfg.BssEdcaParm.Txop[3]  = 47;
        }
        DBGPRINT(RT_DEBUG_TRACE,"STA        : AIFSN CWmin CWmax TXOP(us)  ACM\n");
        DBGPRINT(RT_DEBUG_TRACE,"    AC_BE     %d     %d   %d    %4d     %d\n",
            pAd->PortCfg.BssEdcaParm.Aifsn[0], pAd->PortCfg.BssEdcaParm.Cwmin[0], pAd->PortCfg.BssEdcaParm.Cwmax[0],
            pAd->PortCfg.BssEdcaParm.Txop[0]<<5, pAd->PortCfg.BssEdcaParm.bACM[0]);
        DBGPRINT(RT_DEBUG_TRACE,"    AC_BK     %d     %d   %d    %4d     %d\n",
            pAd->PortCfg.BssEdcaParm.Aifsn[1], pAd->PortCfg.BssEdcaParm.Cwmin[1], pAd->PortCfg.BssEdcaParm.Cwmax[1],
            pAd->PortCfg.BssEdcaParm.Txop[1]<<5, pAd->PortCfg.BssEdcaParm.bACM[1]);
        DBGPRINT(RT_DEBUG_TRACE,"    AC_VI     %d     %d     %d    %4d     %d\n",
            pAd->PortCfg.BssEdcaParm.Aifsn[2], pAd->PortCfg.BssEdcaParm.Cwmin[2], pAd->PortCfg.BssEdcaParm.Cwmax[2],
            pAd->PortCfg.BssEdcaParm.Txop[2]<<5, pAd->PortCfg.BssEdcaParm.bACM[2]);
        DBGPRINT(RT_DEBUG_TRACE,"    AC_VO     %d     %d     %d     %4d     %d\n",
            pAd->PortCfg.BssEdcaParm.Aifsn[3], pAd->PortCfg.BssEdcaParm.Cwmin[3], pAd->PortCfg.BssEdcaParm.Cwmax[3],
            pAd->PortCfg.BssEdcaParm.Txop[3]<<5, pAd->PortCfg.BssEdcaParm.bACM[3]);
    }
#endif /* WMM_SUPPORT */

	AsicSetBssid(pAd, pAd->CurrentAddress); 
    AsicSwitchChannel(pAd, pAd->PortCfg.Channel);
    AsicLockChannel(pAd, pAd->PortCfg.Channel);
    MlmeUpdateTxRates(pAd);
	AsicSetSlotTime(pAd, (BOOLEAN)pAd->PortCfg.UseShortSlotTime);
    
//	if (pAd->PortCfg.RadarDetect.IEEE80211H == TRUE)
	{
		RTMPPrepareRadarDetectParams(pAd);
	}
    
	RTMP_IO_READ32(pAd, MAC_CSR0, &MacCsr0);
	MakeAllBssBeacon(pAd);
	UpdateAllBeaconFrame(pAd);

#ifdef CARRIER_DETECTION_SUPPORT
	if (pAd->PortCfg.CarrierDetect.Enable)
		CarrierDetectionStart(pAd, 1);
	else
		CarrierDetectionStop(pAd);
#endif

	// This part needs to take care of following situation.
	// 1. NO DFS, 2. DFS in silence mode, 3. DFS in normal mode 4. DFS in switching mode
	if ((pAd->PortCfg.PhyMode == PHY_11A) && (pAd->PortCfg.RadarDetect.IEEE80211H == TRUE) && RadarChannelCheck(pAd, pAd->PortCfg.Channel) && (bDFSRestart == TRUE))
	{
		// Change RDMode to RD_SLIENCE_MODE state early.
		pAd->PortCfg.RadarDetect.RDMode = RD_SILENCE_MODE;

		AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x00, 0x00);   // send stop-RD command to MCU
		RTMPusecDelay(500000);

#ifdef CARRIER_DETECTION_SUPPORT
		if (pAd->PortCfg.CarrierDetect.Enable == TRUE)
		{
			// Keep quiet for Radar-Detection. Accroding to 80211h AP should keep quiet for 65s when AP switch to a new channel.
			// trun on silent Carrier-Detection. (Carrier-Detect without CTS protection).
			CarrierDetectionStart(pAd, 0);
		}
#endif // CARRIER_DETECTION_SUPPORT //

		pAd->PortCfg.RadarDetect.RDCount = 0;
		pAd->PortCfg.RadarDetect.InServiceMonitorCount = 0;
		RadarDetectionStart(pAd);
	}
	else if ((pAd->PortCfg.PhyMode == PHY_11A) && (pAd->PortCfg.RadarDetect.IEEE80211H == TRUE) && RadarChannelCheck(pAd, pAd->PortCfg.Channel))
	{
	}
	else
	{
		AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x00, 0x00);   // send stop-RD command to MCU
		RTMPusecDelay(500000);
		
		pAd->PortCfg.RadarDetect.RDMode = RD_NORMAL_MODE;
		RadarDetectionStop(pAd);
        AsicEnableBssSync(pAd);
	}

#ifndef GEM_LEDMODE
    // Set LED
	RTMPSetLED(pAd, LED_RADIO_ON);
	//delay to get H2M_MAILBOX ownership again before next RTMPSetLED().
	mdelay(1000);
	RTMPSetLED(pAd, LED_LINK_UP);
#else
	ASIC_LED_SET_OFF(pAd);
#endif

    // Group rekey related
    if ((pAd->PortCfg.WPAREKEY.ReKeyInterval != 0) && ((pAd->PortCfg.WPAREKEY.ReKeyMethod == TIME_REKEY) || (pAd->PortCfg.WPAREKEY.ReKeyMethod == PKT_REKEY))) 
    {
        // Regularly check the timer
        if (pAd->PortCfg.REKEYTimerRunning == FALSE)
        {
			RTMPAddTimer(&pAd->PortCfg.REKEYTimer, GUPDATE_EXEC_INTV);
            pAd->PortCfg.REKEYTimerRunning = TRUE;
            pAd->PortCfg.REKEYCOUNTER = 0;
        }
    }
    else
    	pAd->PortCfg.REKEYTimerRunning = FALSE;

	pAd->Mlme.PeriodicRound = 0;

    ApLogEvent(pAd, pAd->CurrentAddress, EVENT_RESET_ACCESS_POINT, MAIN_MBSSID);

#ifdef APCLI_SUPPORT
	for (apcliidx = 0; apcliidx < MAX_APCLI_ENTRY; apcliidx++)
	{
		// Pairwise or Shared key table lookup enable for ApCli interface
		if (pAd->ApCliTab.ApCliEntry[apcliidx].Enable)
		{
			// remove group key table for ApCli IF
    		for (i = 0; i < SHARE_KEY_NUM; i++)
    		{
				AsicRemoveSharedKeyEntry(pAd, pAd->PortCfg.BssidNum, i);
    		}
			
			// When OPEN-WEP, Shared-WEP, Auto-WEP, open shared key table security
			if (pAd->ApCliTab.ApCliEntry[apcliidx].PairCipher == Ndis802_11WEPEnabled)
    		{
    			for (i = 0; i < SHARE_KEY_NUM; i++)
	    		{
	    			if (pAd->ApCliTab.ApCliEntry[apcliidx].SharedKey[i].KeyLen != 0)
	    			{
                    	AsicAddSharedKeyEntry(pAd, pAd->PortCfg.BssidNum, i, pAd->ApCliTab.ApCliEntry[apcliidx].SharedKey[i].CipherAlg, pAd->ApCliTab.ApCliEntry[apcliidx].SharedKey[i].Key, NULL, NULL);
	    			}
	    		}
	    		
	    		// Stop STA supplicant WPA state machine
                pAd->ApCliTab.ApCliEntry[apcliidx].WpaState = SS_NOTUSE;
#ifdef RTL865X_SOC
				printk("IF(apcli%d) AP-Client AuthMode=%d, Shared Key Table in-used\n", apcliidx, pAd->ApCliTab.ApCliEntry[apcliidx].AuthMode);
#else                
                DBGPRINT(RT_DEBUG_TRACE, "IF(apcli%d) AP-Client AuthMode=%d, Shared Key Table in-used\n", apcliidx, pAd->ApCliTab.ApCliEntry[apcliidx].AuthMode);
#endif                
    		}
    		// When WPA, open pairwise key table security
			else if (pAd->ApCliTab.ApCliEntry[apcliidx].AuthMode >= Ndis802_11AuthModeWPA)
	    	{
	    		sec_csr4 |= BIT32[pAd->PortCfg.BssidNum];
	        	
	        	// Start STA supplicant WPA state machine
                pAd->ApCliTab.ApCliEntry[apcliidx].WpaState = SS_START;

#ifdef RTL865X_SOC
				printk("IF(apcli%d) AP-Client AuthMode=%d, Pairwise Key Table in-used\n", apcliidx, pAd->ApCliTab.ApCliEntry[apcliidx].AuthMode);
#else
	        	DBGPRINT(RT_DEBUG_TRACE, "IF(apcli%d) AP-Client AuthMode=%d, Pairwise Key Table in-used\n", apcliidx, pAd->ApCliTab.ApCliEntry[apcliidx].AuthMode);
#endif	        	

	    		RTMP_IO_WRITE32(pAd, SEC_CSR4, sec_csr4);	
	    	}
	    	else
	    	{
	        	// Stop STA supplicant WPA state machine
                pAd->ApCliTab.ApCliEntry[apcliidx].WpaState = SS_NOTUSE;
#ifdef RTL865X_SOC
				printk("IF(apcli%d) AP-Client AuthMode=%d, disable Pairwise or Shared Key Table\n", apcliidx, pAd->ApCliTab.ApCliEntry[apcliidx].AuthMode);
#else                
	        	DBGPRINT(RT_DEBUG_TRACE, "IF(apcli%d) AP-Client AuthMode=%d, disable Pairwise or Shared Key Table\n", apcliidx, pAd->ApCliTab.ApCliEntry[apcliidx].AuthMode);
#endif	        	
	    	}
		}
		
		// firstly, disassociate this connection,
		// Then re-connect that previous AP. 
		if (pAd->ApCliTab.ApCliEntry[apcliidx].Enable && pAd->ApCliTab.ApCliEntry[apcliidx].Valid)
		{
			DBGPRINT(RT_DEBUG_TRACE, "Restart ApCli link - IF(apcli%d).\n", apcliidx);
			StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_DISCONNECT_REQ, 0, NULL, apcliidx);
		}		
	}
#endif // APCLI_SUPPORT //

#ifdef WSC_SUPPORT
// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
    if (pAd->ApCliTab.ApCliEntry[MAIN_MBSSID].WscControl.WscConfMode != WSC_DISABLE)  // add by johnli, fix crash bug when enable WPS for ra0
        WscInitRegistrarPair(pAd, TRUE, &pAd->ApCliTab.ApCliEntry[apcliidx].WscControl);
#endif // APCLI_SUPPORT //
// --
//    if (pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscConfMode != WSC_DISABLE)  // remove by johnli, bug fix: the version of WPS IE is incorrect ("00") when user enable WPS at first time
        WscInitRegistrarPair(pAd, FALSE, &pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl);
#endif // WSC_SUPPORT //

    DBGPRINT(RT_DEBUG_TRACE, "<--- ApStartUp (sec_csr4=0x%x)\n", sec_csr4);
}

VOID SendSingalToDaemon(
	IN PRTMP_ADAPTER	pAd,
	IN INT				sig,
	ULONG				pid)
{
	DBGPRINT(RT_DEBUG_TRACE, "SendSingalToDaemon : Pid=%d\n", pid);

	if (pid != 0)
	{
		kill_proc(pid, sig, 0);
	}
}

VOID ApStop(
    IN PRTMP_ADAPTER	pAd,
    IN BOOLEAN			bDFSRestart) 
{
	DBGPRINT(RT_DEBUG_TRACE, "!!! ApStop !!!\n");

	// It's the only way to stop beacon sending.
	// Must do AsicDisableSync first to send precise number of Channel-Switch-Announcement.
    AsicDisableSync(pAd);

    if(pAd->PortCfg.REKEYTimerRunning==TRUE)
    {
        RTMPCancelTimer(&pAd->PortCfg.REKEYTimer);
        pAd->PortCfg.REKEYTimerRunning=FALSE;
    }

    if(pAd->PortCfg.CMTimerRunning==TRUE)
    {
        RTMPCancelTimer(&pAd->PortCfg.CounterMeasureTimer);
    	pAd->PortCfg.CMTimerRunning = FALSE;
    }

    MacTableReset(pAd, bDFSRestart);

	if (bDFSRestart == TRUE)
	{
		RadarDetectionStop(pAd);

		AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x00, 0x00);   // send stop-RD command to MCU
		//delay to get H2M_MAILBOX ownership again before next RTMPSetLED().
		mdelay(1000);
	}

#ifndef GEM_LEDMODE
	RTMPSetLED(pAd, LED_RADIO_OFF);
	//delay to get H2M_MAILBOX ownership again before next RTMPSetLED().
	mdelay(1000);
    RTMPSetLED(pAd, LED_LINK_DOWN);
#else
	ASIC_LED_SET_OFF(pAd);
#endif
}

/*
    ==========================================================================
    Description:
        This routine is used to clean up a specified power-saving queue. It's
        used whenever a wireless client is deleted.
    ==========================================================================
 */
VOID CleanupPsQueue(
    IN  PRTMP_ADAPTER   pAd,
    IN  PQUEUE_HEADER   pQueue)
{
    struct sk_buff	*pSkb;
    
    while (pQueue->Head)
    {
        pSkb = (struct sk_buff *)RemoveHeadQueue(pQueue);
        if(pSkb)		
#ifdef RTL865X_FAST_PATH
	  	 	rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
        	RELEASE_NDIS_PACKET(pAd, pSkb);
#endif
    }
}

/*
    ==========================================================================
    Description:
        This routine reset the entire MAC table. All packets pending in
        the power-saving queues are freed here.
    ==========================================================================
 */
VOID MacTableReset(
    IN  PRTMP_ADAPTER  pAd,
    IN	BOOLEAN		bDFSRestart)
{
    INT				i;
    UMAXLONG		IrqFlags;
    PCHAR        	pOutBuffer = NULL;
    ULONG       	FrameLen = 0;
    HEADER_802_11	DisassocHdr;
    USHORT      	Reason;
#ifdef WSC_SUPPORT
    UCHAR         	apidx = MAIN_MBSSID;
#endif // WSC_SUPPORT //

#ifndef THREAD_ISR
    RTMP_SEM_LOCK(&pAd->MacTabLock, IrqFlags);
#endif
    
    for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++)
    {
        if (pAd->MacTab.Content[i].Valid)
       {
			if (pAd->MacTab.Content[i].EnqueueStartForPSKTimerRunning == TRUE)
            {
                DBGPRINT(RT_DEBUG_TRACE, "pAd->MacTab.Content[%d]  Cancel EnqueueStartForPSK Timer !\n",i);
                RTMPCancelTimer(&pAd->MacTab.Content[i].EnqueueStartForPSKTimer);
                pAd->MacTab.Content[i].EnqueueStartForPSKTimerRunning = FALSE;             
            }

            if (pAd->MacTab.Content[i].RetryTimerRunning == TRUE)
            {
                DBGPRINT(RT_DEBUG_TRACE, "pAd->MacTab.Content[%d]  Cancel Retry Timer !\n",i);
                RTMPCancelTimer(&pAd->MacTab.Content[i].RetryTimer);
                pAd->MacTab.Content[i].RetryTimerRunning = FALSE;             
            }

#ifdef WSC_SUPPORT
            if (pAd->MacTab.Content[i].EnqueueEapolStartTimerForWscRunning == TRUE)
            {
                RTMPCancelTimer(&pAd->MacTab.Content[i].EnqueueEapolStartTimerForWsc);
                pAd->MacTab.Content[i].EnqueueEapolStartTimerForWscRunning = FALSE;
            }
            pAd->MacTab.Content[i].Receive_EapolStart_EapRspId = 0;
#endif // WSC_SUPPORT //

#ifdef THREAD_ISR
			RTMP_IRQ_LOCK(IrqFlags);
#endif    
            CleanupPsQueue(pAd, &pAd->MacTab.Content[i].PsQueue);
#ifdef THREAD_ISR
			RTMP_IRQ_UNLOCK(IrqFlags);
#endif    


            // Before reset MacTable, send disassociation packet to client.
            if (/*(!bDFSRestart) &&*/ (pAd->MacTab.Content[i].Sst == SST_ASSOC))
            {
                //  send out a DISASSOC request frame
                pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
                if(pOutBuffer == NULL) {
#ifndef THREAD_ISR
				    RTMP_SEM_UNLOCK(&pAd->MacTabLock, IrqFlags);
#endif
                	return;
                }
                	


                Reason = REASON_DISASSOC_INACTIVE;
                DBGPRINT(RT_DEBUG_ERROR, "ASSOC - Send DISASSOC  Reason = %d frame  TO %x %x %x %x %x %x \n",Reason,pAd->MacTab.Content[i].Addr[0],
                    pAd->MacTab.Content[i].Addr[1],pAd->MacTab.Content[i].Addr[2],pAd->MacTab.Content[i].Addr[3],pAd->MacTab.Content[i].Addr[4],pAd->MacTab.Content[i].Addr[5]);
                MgtMacHeaderInit(pAd, &DisassocHdr, SUBTYPE_DISASSOC, 0, pAd->MacTab.Content[i].Addr, pAd->PortCfg.MBSSID[pAd->MacTab.Content[i].ApIdx].Bssid);
                MakeOutgoingFrame(pOutBuffer, &FrameLen, sizeof(HEADER_802_11), (CHAR *)&DisassocHdr, 2, (char*)&Reason, END_OF_ARGS);
                MiniportMMRequest(pAd, pOutBuffer, FrameLen);
                kfree(pOutBuffer);

                RTMPusecDelay(2000);
            }
        }
    }
    
#ifdef WSC_SUPPORT
	for (apidx = MAIN_MBSSID; apidx < pAd->PortCfg.BssidNum; apidx++)
	{
    	RTMPCancelTimer(&pAd->PortCfg.MBSSID[apidx].WscControl.EapolTimer);
    	pAd->PortCfg.MBSSID[apidx].WscControl.EapolTimerRunning = FALSE;
    
    	pAd->PortCfg.MBSSID[apidx].WscControl.EntryApIdx = WSC_INIT_ENTRY_APIDX;
    	pAd->PortCfg.MBSSID[apidx].WscControl.EapMsgRunning = FALSE;
	}
#endif // WSC_SUPPORT //

    
#ifdef THREAD_ISR
	RTMP_IRQ_LOCK(IrqFlags);
#endif    
    CleanupPsQueue(pAd, &pAd->MacTab.McastPsQueue);
    NdisZeroMemory(&pAd->MacTab, sizeof(MAC_TABLE));
    InitializeQueueHeader(&pAd->MacTab.McastPsQueue);
#ifdef THREAD_ISR
	RTMP_IRQ_UNLOCK(IrqFlags);
#endif	

#ifndef THREAD_ISR
    RTMP_SEM_UNLOCK(&pAd->MacTabLock, IrqFlags);
#endif

}


/*
    ==========================================================================
    Description:
        Add and new entry into MAC table
    ==========================================================================
 */
MAC_TABLE_ENTRY *MacTableInsertEntry(
    IN	PRTMP_ADAPTER	pAd, 
    IN  PUCHAR        pAddr,
    IN  UCHAR           apidx) 
{
    UCHAR HashIdx;
    int i;
    MAC_TABLE_ENTRY *pEntry = NULL, *pCurrEntry;
    UMAXLONG	IrqFlags;

    // if FULL, return
    if (pAd->MacTab.Size >= MAX_LEN_OF_MAC_TABLE) 
        return NULL;

    // allocate one MAC entry
    RTMP_SEM_LOCK(&pAd->MacTabLock, IrqFlags);
    for (i = 1; i< MAX_LEN_OF_MAC_TABLE; i++)		// skip entry#0 so that "entry index == AID" for fast lookup
    {
        // pick up the first available vacancy
        if (pAd->MacTab.Content[i].Valid == FALSE)
        {
            pEntry = &pAd->MacTab.Content[i];
            NdisZeroMemory(pEntry, sizeof(MAC_TABLE_ENTRY));
            pEntry->Valid = TRUE;

            // Move init entry retry timer to where it first created
            init_timer(&pEntry->RetryTimer);
			RTMPInitTimer((PRTMP_ADAPTER) pEntry, &pEntry->EnqueueStartForPSKTimer, (PVOID)&EnqueueStartForPSKExec);
            
            pEntry->bDot1xDynamicWep = FALSE;
            pEntry->RetryTimerRunning = FALSE;
            pEntry->CMTimerRunning = FALSE;
			pEntry->EnqueueStartForPSKTimerRunning= FALSE;
            pEntry->RSNIE_Len = 0;
            NdisMoveMemory(pEntry->R_Counter, pAd->PortCfg.R_Counter, sizeof(pEntry->R_Counter));
            pEntry->ReTryCounter = PEER_MSG1_RETRY_TIMER_CTR;
            pEntry->AuthMode = pAd->PortCfg.MBSSID[apidx].AuthMode;
            pEntry->WepStatus = pAd->PortCfg.MBSSID[apidx].WepStatus;
            pEntry->pAd = (unsigned long)pAd;
            pEntry->PMKID_CacheIdx = ENTRY_NOT_FOUND;

            if (pEntry->AuthMode < Ndis802_11AuthModeWPA)
                pEntry->WpaState = AS_NOTUSE;
            else
                pEntry->WpaState = AS_INITIALIZE;
            pEntry->GTKState = REKEY_NEGOTIATING;
            pEntry->PrivacyFilter = Ndis802_11PrivFilterAcceptAll;        
            pEntry->PairwiseKey.KeyLen = 0;
            pEntry->PortSecured = WPA_802_1X_PORT_NOT_SECURED;
            AsicRemovePairwiseKeyEntry(pAd, (UCHAR)i);
            COPY_MAC_ADDR(pEntry->Addr, pAddr);
            pEntry->Sst = SST_NOT_AUTH;
            pEntry->AuthState = AS_NOT_AUTH;
            pEntry->Aid = 0;
            pEntry->CapabilityInfo = 0;
            pEntry->PsMode = PWR_ACTIVE;
            pEntry->MaxSupportedRate = RATE_11;
            pEntry->CurrTxRate = RATE_11;
            pEntry->LastTxRate = pEntry->CurrTxRate;//for initial value
            pEntry->PsQIdleCount = 0;
            pEntry->NoDataIdleCount = 0;
            InitializeQueueHeader(&pEntry->PsQueue);
//iverson
	    pEntry->bAPSDFlagEOSPOK = 0; 
            pEntry->bAPSDFlagSPStart = 0;    
//iverson end
            pEntry->MaxSPLength = 0;
            pEntry->bDlsInit = FALSE;

            pEntry->ApIdx = apidx;
            
            pAd->MacTab.Size ++;
#ifdef GEM_LEDMODE
	    	if (pAd->MacTab.Size == 1)
				ASIC_LED_SET_ON(pAd);
#endif

#ifdef IGMP_SNOOP_SUPPORT
			pAd->StaCount[apidx]++;
#endif

#ifdef WSC_SUPPORT
            pEntry->bWscCapable = FALSE;
            RTMPInitTimer((PRTMP_ADAPTER) pEntry, &pEntry->EnqueueEapolStartTimerForWsc, (PVOID)&WscEnqueueEapolStart);
            pEntry->EnqueueEapolStartTimerForWscRunning = FALSE;
            pEntry->Receive_EapolStart_EapRspId = 0;
#endif // WSC_SUPPORT //

            DBGPRINT(RT_DEBUG_TRACE, "MacTableInsertEntry -IF(ra%d) allocate entry #%d, Total= %d\n",apidx, i, pAd->MacTab.Size);
            break;
        }
    }

    // add this MAC entry into HASH table
    if (pEntry)
    {
        HashIdx = MAC_ADDR_HASH_INDEX(pAddr);
        if (pAd->MacTab.Hash[HashIdx] == NULL)
        {
            pAd->MacTab.Hash[HashIdx] = pEntry;
        }
        else
        {
            pCurrEntry = pAd->MacTab.Hash[HashIdx];
            while (pCurrEntry->pNext != NULL)
                pCurrEntry = pCurrEntry->pNext;
            pCurrEntry->pNext = pEntry;
        }
#ifdef WSC_SUPPORT
        if ((pEntry->ApIdx == MAIN_MBSSID) &&
             MAC_ADDR_EQUAL(pEntry->Addr, pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EntryAddr))
        {
            pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EntryApIdx = WSC_INIT_ENTRY_APIDX;
            memset(pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EntryAddr, 0, MAC_ADDR_LEN);
        }
#endif // WSC_SUPPORT //
    }
    
    RTMP_SEM_UNLOCK(&pAd->MacTabLock, IrqFlags);
	//added by xyzhu_nj_20091208
	gMacTabChanged = 1;
	
    return pEntry;
}

/*
    ==========================================================================
    Description:
        Delete a specified client from MAC table
    ==========================================================================
 */
BOOLEAN MacTableDeleteEntry(
    IN PRTMP_ADAPTER pAd, 
    IN PUCHAR pAddr) 
{
    USHORT HashIdx;
    MAC_TABLE_ENTRY *pEntry, *pPrevEntry;
    UMAXLONG	IrqFlags;

    RTMP_SEM_LOCK(&pAd->MacTabLock, IrqFlags);
    HashIdx = MAC_ADDR_HASH_INDEX(pAddr);
    pEntry = pAd->MacTab.Hash[HashIdx];
    if (pEntry)
    {
        if (MAC_ADDR_EQUAL(pEntry->Addr, pAddr))
        {
			if(pEntry->EnqueueStartForPSKTimerRunning == TRUE)
	        {
	            DBGPRINT(RT_DEBUG_TRACE, "MacTableDeleteEntry : Cancel EnqueueStartForPSK Timer !\n");
	            RTMPCancelTimer(&pEntry->EnqueueStartForPSKTimer);
	            pEntry->EnqueueStartForPSKTimerRunning = FALSE;                
	        }

        	if(pEntry->RetryTimerRunning==TRUE)
	        {
	            DBGPRINT(RT_DEBUG_TRACE, "MacTableDeleteEntry : Cancel Retry Timer !\n");
	            RTMPCancelTimer(&pEntry->RetryTimer);
	            pEntry->RetryTimerRunning=FALSE;                
	        }
            pAd->MacTab.Hash[HashIdx] = pEntry->pNext;
            CleanupPsQueue(pAd, &pEntry->PsQueue); // return all skb buffer in PSQ
            AsicRemovePairwiseKeyEntry(pAd, (UCHAR)(((UMAXLONG)pEntry - (UMAXLONG)&pAd->MacTab.Content[0])/sizeof(MAC_TABLE_ENTRY)));

			// Clear corresponding bit for PS-mode status 
        	if ((pEntry->Aid > 0) && (pEntry->Aid < MAX_LEN_OF_MAC_TABLE) && pEntry->PsMode == PWR_SAVE)
        	{			
				// clear corresponding bit
				if (pEntry->Aid >= 32)
	            	pAd->MacTab.PsmBitmap2 &= (~BIT32[(pEntry->Aid)-32]); 
		        else
		            pAd->MacTab.PsmBitmap &= (~BIT32[pEntry->Aid]);
	    	    DBGPRINT(RT_DEBUG_TRACE, "Clear (AID=%d) , PS-mode bitmap=%02x-%02x\n", pEntry->Aid, pAd->MacTab.PsmBitmap, pAd->MacTab.PsmBitmap2);

				// check if there's any associated STA in power-save mode. this affects outgoing
	        	// MCAST/BCAST frames should be stored in PSQ till DtimCount=0
    	    	if (pAd->MacTab.PsmBitmap == 0 && pAd->MacTab.PsmBitmap2 == 0)
        	    	pAd->MacTab.fAnyStationInPsm = FALSE;
				else 
					pAd->MacTab.fAnyStationInPsm = TRUE;	

				DBGPRINT(RT_DEBUG_TRACE, "fAnyStationInPsm = %d\n", pAd->MacTab.fAnyStationInPsm);
			}
	        		
#ifdef IGMP_SNOOP_SUPPORT
			pAd->StaCount[pEntry->ApIdx]--;
			IgmpGroupDelMembers(pAd->pMulticastFilterTable, (PUCHAR)pEntry->Addr, pAd->PortCfg.MBSSID[pEntry->ApIdx].MSSIDDev);
#endif

#ifdef WSC_SUPPORT
            if (pEntry->EnqueueEapolStartTimerForWscRunning)
            {
                RTMPCancelTimer(&pEntry->EnqueueEapolStartTimerForWsc);
                pEntry->EnqueueEapolStartTimerForWscRunning = FALSE;
            }
            
            if (MAC_ADDR_EQUAL(pEntry->Addr, pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.EntryAddr))
            {
                RTMPCancelTimer(&pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.EapolTimer);
                pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.EapolTimerRunning = FALSE;
                pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.EntryApIdx = WSC_INIT_ENTRY_APIDX;
                pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.EapMsgRunning = FALSE;
                NdisZeroMemory(&(pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.EntryAddr[0]), MAC_ADDR_LEN);
                if (pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.Wsc2MinsTimerRunning)
            	{
            		pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.Wsc2MinsTimerRunning = FALSE;
            		RTMPCancelTimer(&pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.Wsc2MinsTimer);
            	}
            }
            pEntry->Receive_EapolStart_EapRspId = 0;
#endif // WSC_SUPPORT //

            NdisZeroMemory(pEntry, sizeof(MAC_TABLE_ENTRY));
            pAd->MacTab.Size --;
            DBGPRINT(RT_DEBUG_TRACE, "MacTableDeleteEntry 1 - Total= %d\n", pAd->MacTab.Size);
#ifdef GEM_LEDMODE
	    	if (!pAd->MacTab.Size)
				ASIC_LED_SET_OFF(pAd);
#endif
        }
        else
        {
            while (pEntry->pNext)
            {
                pPrevEntry = pEntry;
                pEntry = pEntry->pNext;
                if (MAC_ADDR_EQUAL(pEntry->Addr, pAddr))
                {
					if(pEntry->EnqueueStartForPSKTimerRunning == TRUE)
			        {
			            DBGPRINT(RT_DEBUG_TRACE, "MacTableDeleteEntry : Cancel EnqueueStartForPSK Timer !\n");
			            RTMPCancelTimer(&pEntry->EnqueueStartForPSKTimer);
			            pEntry->EnqueueStartForPSKTimerRunning=FALSE;                
			        }

                	if(pEntry->RetryTimerRunning==TRUE)
			        {
			            DBGPRINT(RT_DEBUG_TRACE, "MacTableDeleteEntry : Cancel Retry Timer !\n");
			            RTMPCancelTimer(&pEntry->RetryTimer);
			            pEntry->RetryTimerRunning=FALSE;                
			        }

                    pPrevEntry->pNext = pEntry->pNext;
                    CleanupPsQueue(pAd, &pEntry->PsQueue); // return all skb buffer in PSQ
                    AsicRemovePairwiseKeyEntry(pAd, (UCHAR)(((UMAXLONG)pEntry - (UMAXLONG)&pAd->MacTab.Content[0])/sizeof(MAC_TABLE_ENTRY)));

					// Clear corresponding bit for PS-mode status 
        			if ((pEntry->Aid > 0) && (pEntry->Aid < MAX_LEN_OF_MAC_TABLE) && pEntry->PsMode == PWR_SAVE)
		        	{			
						// clear corresponding bit
						if (pEntry->Aid >= 32)
		            		pAd->MacTab.PsmBitmap2 &= (~BIT32[(pEntry->Aid)-32]); 
				        else
		    		        pAd->MacTab.PsmBitmap &= (~BIT32[pEntry->Aid]);
			    	    DBGPRINT(RT_DEBUG_TRACE, "Clear (AID=%d) , PS-mode bitmap=%02x-%02x\n", pEntry->Aid, pAd->MacTab.PsmBitmap, pAd->MacTab.PsmBitmap2);

						// check if there's any associated STA in power-save mode. this affects outgoing
	    	    		// MCAST/BCAST frames should be stored in PSQ till DtimCount=0
	    		    	if (pAd->MacTab.PsmBitmap == 0 && pAd->MacTab.PsmBitmap2 == 0)
        		    		pAd->MacTab.fAnyStationInPsm = FALSE;
						else 
							pAd->MacTab.fAnyStationInPsm = TRUE;		

						DBGPRINT(RT_DEBUG_TRACE, "fAnyStationInPsm = %d\n", pAd->MacTab.fAnyStationInPsm);
					}

#ifdef IGMP_SNOOP_SUPPORT
					pAd->StaCount[pEntry->ApIdx]--;
					IgmpGroupDelMembers(pAd->pMulticastFilterTable, (PUCHAR)pEntry->Addr, pAd->PortCfg.MBSSID[pEntry->ApIdx].MSSIDDev);
#endif

#ifdef WSC_SUPPORT
                    if (pEntry->EnqueueEapolStartTimerForWscRunning)
                    {
                        RTMPCancelTimer(&pEntry->EnqueueEapolStartTimerForWsc);
                        pEntry->EnqueueEapolStartTimerForWscRunning = FALSE;
                    }
                    if (MAC_ADDR_EQUAL(pEntry->Addr, pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.EntryAddr))
                    {
                        RTMPCancelTimer(&pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.EapolTimer);
                        pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.EapolTimerRunning = FALSE;
                        pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.EntryApIdx = WSC_INIT_ENTRY_APIDX;
                        pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.EapMsgRunning = FALSE;
                    }
                    pEntry->Receive_EapolStart_EapRspId = 0;
#endif // WSC_SUPPORT //

                    NdisZeroMemory(pEntry, sizeof(MAC_TABLE_ENTRY));
                    pAd->MacTab.Size --;
                    DBGPRINT(RT_DEBUG_TRACE, "MacTableDeleteEntry 2 - Total= %d\n", pAd->MacTab.Size);
#ifdef GEM_LEDMODE
		    		if (!pAd->MacTab.Size)
						ASIC_LED_SET_OFF(pAd);
#endif
                    break;
                }
            }
        }
    }

    RTMP_SEM_UNLOCK(&pAd->MacTabLock, IrqFlags);
    ApUpdateCapabilityAndErpIe(pAd);
	//added by xyzhu_nj_20091208
	gMacTabChanged = 1;
	
    return TRUE;
}

/*
    ==========================================================================
    Description:
        Look up the MAC address in the MAC table. Return NULL if not found.
    Return:
        pEntry - pointer to the MAC entry; NULL is not found
    ==========================================================================
*/
MAC_TABLE_ENTRY *MacTableLookup(
    IN PRTMP_ADAPTER pAd, 
    IN PUCHAR pAddr) 
{
    ULONG HashIdx;
    MAC_TABLE_ENTRY *pEntry = NULL;

    HashIdx = MAC_ADDR_HASH_INDEX(pAddr);
    pEntry = pAd->MacTab.Hash[HashIdx];

    while (pEntry && pEntry->Valid)
    {
        if (MAC_ADDR_EQUAL(pEntry->Addr, pAddr)) 
        {
            break;
        }
        else
            pEntry = pEntry->pNext;
    }

    return pEntry;
}

/*
    ==========================================================================
    Description:
        This routine is called by MlmePeriodicExec() every second to check if
        1. any associated client in PSM. If yes, then TX MCAST/BCAST should be
           out in DTIM only
        2. any client being idle for too long and should be aged-out from MAC table
        3. garbage collect PSQ
    ==========================================================================
*/
VOID MacTableMaintenance(
    IN PRTMP_ADAPTER pAd)
{
    INT     i;
    UMAXLONG	IrqFlags;

    //pAd->MacTab.fAnyStationInPsm = FALSE;
    for (i = 0; i < MAX_LEN_OF_MAC_TABLE; i++) 
    {
        MAC_TABLE_ENTRY *pEntry = &pAd->MacTab.Content[i];

        if (pEntry->Valid == FALSE)
            continue;
        
        pEntry->NoDataIdleCount ++;

        DBGPRINT(RT_DEBUG_INFO, "(ra%d)%02x:%02x:%02x:%02x:%02x:%02x,TxByte=%ld,RxByte=%ld,LastDataTime=%ld\n", pEntry->ApIdx,
            pEntry->Addr[0],pEntry->Addr[1],pEntry->Addr[2],pEntry->Addr[3],
            pEntry->Addr[4],pEntry->Addr[5],pEntry->HSCounter.TotalTxByteCount,
            pEntry->HSCounter.TotalRxByteCount,pEntry->HSCounter.LastDataPacketTime);

        // 0. STA failed to complete association should be removed to save MAC table space.
        if ((pEntry->Sst != SST_ASSOC) && (pEntry->NoDataIdleCount >= MAC_TABLE_ASSOC_TIMEOUT))
        {
            DBGPRINT(RT_DEBUG_TRACE, "%02x:%02x:%02x:%02x:%02x:%02x fail to complete ASSOC in %d sec\n",
                    pEntry->Addr[0],pEntry->Addr[1],pEntry->Addr[2],pEntry->Addr[3],
                    pEntry->Addr[4],pEntry->Addr[5],MAC_TABLE_ASSOC_TIMEOUT);
            MacTableDeleteEntry(pAd, pEntry->Addr);
            continue;
        }
        
// remove to rtmp.h::PsIndicate
#if 0			
        // 1. check if there's any associated STA in power-save mode. this affects outgoing
        //    MCAST/BCAST frames should be stored in PSQ till DtimCount=0
        if (pEntry->PsMode == PWR_SAVE)
            pAd->MacTab.fAnyStationInPsm = TRUE;
#endif

        // 2. delete those MAC entry that has been idle for a long time
        if (pEntry->NoDataIdleCount >= MAC_TABLE_AGEOUT_TIME)
        {
            DBGPRINT(RT_DEBUG_TRACE, "ageout %02x:%02x:%02x:%02x:%02x:%02x after %d-sec silence\n",
                    pEntry->Addr[0],pEntry->Addr[1],pEntry->Addr[2],pEntry->Addr[3],
                    pEntry->Addr[4],pEntry->Addr[5],MAC_TABLE_AGEOUT_TIME);
            ApLogEvent(pAd, pEntry->Addr, EVENT_AGED_OUT, pEntry->ApIdx);
            
            //notify 802.1x-deamon to discard the sta
			RTMPHandleNotify8021xDiscardSta(pAd, pEntry);
            
            MacTableDeleteEntry(pAd, pEntry->Addr);
            continue;
        }
        
        RTMP_SEM_LOCK(&pAd->MacTabLock, IrqFlags);
        // 3. garbage collect the PsQueue if the STA has being idle for a while
        if (pEntry->PsQueue.Head)
        {
            pEntry->PsQIdleCount ++;  
            if (pEntry->PsQIdleCount > 12) // Patch for WMM-PS 4.8
            {
                CleanupPsQueue(pAd, &pEntry->PsQueue);                
                pEntry->PsQIdleCount = 0;
            }
        }
        else
            pEntry->PsQIdleCount = 0;
        RTMP_SEM_UNLOCK(&pAd->MacTabLock, IrqFlags);
    }
    
    // 4. garbage collect pAd->MacTab.McastPsQueue if backlogged MCAST/BCAST frames
    //    stale in queue. Since MCAST/BCAST frames always been sent out whenever 
    //    DtimCount==0, the only case to let them stale is surprise removal of the NIC,
    //    so that ASIC-based Tbcn interrupt stops and DtimCount dead.
#ifndef THREAD_ISR
	RTMP_SEM_LOCK(&pAd->MacTabLock, IrqFlags);
#else
	RTMP_IRQ_LOCK(IrqFlags);
#endif
    if (pAd->MacTab.McastPsQueue.Head)
    {
        pAd->MacTab.PsQIdleCount ++;
        if (pAd->MacTab.PsQIdleCount > 1)
        {
            CleanupPsQueue(pAd, &pAd->MacTab.McastPsQueue);
            pAd->MacTab.PsQIdleCount = 0;
        }
    }
    else
        pAd->MacTab.PsQIdleCount = 0;
#ifndef THREAD_ISR
    RTMP_SEM_UNLOCK(&pAd->MacTabLock, IrqFlags);
#else
	RTMP_IRQ_UNLOCK(IrqFlags);
#endif
}

/*
    ==========================================================================
    Description:
        Look up a STA MAC table. Return its Sst to decide if an incoming
        frame from this STA or an outgoing frame to this STA is permitted.
    Return:
    ==========================================================================
*/
MAC_TABLE_ENTRY *SsPsInquiry(
    IN	PRTMP_ADAPTER	pAd, 
    IN PUCHAR pAddr, 
    OUT SST   *Sst, 
    OUT USHORT *Aid,
    OUT UCHAR *PsMode,
    OUT UCHAR *Rate) 
{
    MAC_TABLE_ENTRY *pEntry = NULL;

    if (MAC_ADDR_IS_GROUP(pAddr)) // mcast & broadcast address
    {
        *Sst        = SST_ASSOC;
        *Aid        = 0;
        *PsMode     = PWR_ACTIVE;
        //*Rate       = pAd->PortCfg.MlmeRate; //pAd->PortCfg.MaxBasicRate;
        if (pAd->PortCfg.PhyMode == PHY_11A) 
     		*Rate   = RATE_6;	
		else 
			*Rate   = RATE_11;	
    } 
    else // unicast address
    {
        pEntry = MacTableLookup(pAd, pAddr);
        if (pEntry) 
        {
            *Sst        = pEntry->Sst;
            *Aid        = pEntry->Aid;
            *PsMode     = pEntry->PsMode;
             if((pAd->PortCfg.MBSSID[pEntry->ApIdx].AuthMode >= Ndis802_11AuthModeWPA) && (pEntry->GTKState != REKEY_ESTABLISHED)) 
             {
             	if (pAd->PortCfg.PhyMode == PHY_11A) 
             		*Rate   = RATE_6;	
				else 
					*Rate   = RATE_11;	
             }                
             else
                *Rate   = pEntry->CurrTxRate;
        } 
        else 
        {
            *Sst        = SST_NOT_AUTH;
            *Aid        = 0;
            *PsMode     = PWR_ACTIVE;
            //*Rate       = pAd->PortCfg.MlmeRate; //pAd->PortCfg.MaxBasicRate;
            if (pAd->PortCfg.PhyMode == PHY_11A) 
         		*Rate   = RATE_6;
			else 
				*Rate   = RATE_11;
        }
    }
    return pEntry;
}


#if BIG_ENDIAN == FALSE 
extern unsigned long get_cmos_time(void);
#endif
/*
    ==========================================================================
    Description:
        This routine is called to log a specific event into the event table.
        The table is a QUERY-n-CLEAR array that stop at full.
    ==========================================================================
 */
VOID ApLogEvent(
    IN PRTMP_ADAPTER pAd,
    IN PUCHAR   pAddr,
    IN USHORT   Event,
    IN UCHAR    apidx)
{
    if (pAd->EventTab.Num < MAX_NUM_OF_EVENT)
    {
        RT_802_11_EVENT_LOG *pLog = &pAd->EventTab.Log[pAd->EventTab.Num];

        pLog->SystemTime = jiffies;

        COPY_MAC_ADDR(&pLog->Addr, pAddr);
        pLog->Event = Event;
        pLog->ApIdx = apidx;
        DBGPRINT(RT_DEBUG_TRACE,"LOG#%d %02x:%02x:%02x:%02x:%02x:%02x %s\n",
            pAd->EventTab.Num, pAddr[0], pAddr[1], pAddr[2], 
            pAddr[3], pAddr[4], pAddr[5], pEventText[Event]);
        pAd->EventTab.Num += 1;
    }
}

/*
    ==========================================================================
    Description:
        Update ERP IE and CapabilityInfo based on STA association status.
        The result will be auto updated into the next outgoing BEACON in next
        TBTT interrupt service routine
    ==========================================================================
 */
VOID ApUpdateCapabilityAndErpIe(
    IN PRTMP_ADAPTER pAd)
{
    UCHAR  i, ErpIeContent = 0, apidx;
    BOOLEAN ShortSlotCapable = (BOOLEAN)pAd->PortCfg.UseShortSlotTime;

    if ((pAd->PortCfg.PhyMode == PHY_11A) || (pAd->PortCfg.PhyMode == PHY_11B))
        return;

    for (i=1; i<MAX_LEN_OF_MAC_TABLE; i++)
    {
        PMAC_TABLE_ENTRY pEntry = &pAd->MacTab.Content[i];
        if ((pEntry->Valid != TRUE) || (pEntry->Sst != SST_ASSOC))
            continue;
        
        // at least one 11b client associated, turn on ERP.NonERPPresent bit
        // almost all 11b client won't support "Short Slot" time, turn off for maximum compatibility
        if (pEntry->MaxSupportedRate < RATE_FIRST_OFDM_RATE)
        {
            ShortSlotCapable = FALSE;
            ErpIeContent |= 0x01;
        }

        // at least one client can't support short slot
        if ((pEntry->CapabilityInfo & 0x0400) == 0)
            ShortSlotCapable = FALSE;
    }

    // decide ErpIR.UseProtection bit, depending on pAd->PortCfg.UseBGProtection
    //    AUTO (0): UseProtection = 1 if any 11b STA associated
    //    ON (1): always USE protection
    //    OFF (2): always NOT USE protection
    if (pAd->PortCfg.UseBGProtection == 0)
    {
        ErpIeContent = (ErpIeContent)? 0x03 : 0x00;
        if ((pAd->Mlme.LastOLBCDetectTime + (5 * HZ)) > pAd->Mlme.Now32) // legacy BSS exist within 5 sec
        {
            DBGPRINT(RT_DEBUG_INFO, "ApUpdateCapabilityAndErpIe - Legacy 802.11b BSS overlaped\n");
            ErpIeContent |= 0x02;                                     // set Use_Protection bit
        }
    }
    else if (pAd->PortCfg.UseBGProtection == 1)   
        ErpIeContent |= 0x02;
    else
        ;

    if((pAd->PortCfg.TxPreamble == Rt802_11PreambleLong) || (ApCheckLongPreambleSTA(pAd) == TRUE))
    {
    	if (pAd->PortCfg.TxPreambleInUsed != Rt802_11PreambleLong)
        	MlmeSetTxPreamble(pAd, Rt802_11PreambleLong);
        pAd->Mlme.ErpIeContent = (ErpIeContent | 0x04);
    }
    else
    {
    	if (pAd->PortCfg.TxPreambleInUsed != Rt802_11PreambleShort)
        	MlmeSetTxPreamble(pAd, Rt802_11PreambleShort);
        pAd->Mlme.ErpIeContent = ErpIeContent;
    }

    //
    // deicide CapabilityInfo.ShortSlotTime bit and ShortPreamble bit
    //
    for (apidx = 0; apidx < pAd->PortCfg.BssidNum; apidx++)
    {
        if (ShortSlotCapable)
            pAd->PortCfg.MBSSID[apidx].CapabilityInfo |= 0x0400;
        else
            pAd->PortCfg.MBSSID[apidx].CapabilityInfo &= 0xfbff;

		if (pAd->PortCfg.TxPreamble == Rt802_11PreambleLong)
			pAd->PortCfg.MBSSID[apidx].CapabilityInfo &= (~0x020);
		else
			pAd->PortCfg.MBSSID[apidx].CapabilityInfo |= 0x020;
		
        DBGPRINT(RT_DEBUG_INFO, "ApUpdateCapabilityAndErpIe IF(ra%d)- Capability= 0x%04x, ERP is 0x%02x\n", 
            apidx, pAd->PortCfg.MBSSID[apidx].CapabilityInfo, pAd->Mlme.ErpIeContent);
    }

    AsicSetSlotTime(pAd, ShortSlotCapable);
}

/*
    ==========================================================================
    Description:
        Check to see the exist of long preamble STA in associated list
    ==========================================================================
 */
BOOLEAN ApCheckLongPreambleSTA(
    IN PRTMP_ADAPTER pAd)
{
    UCHAR   i;
    
    for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++)
    {
        PMAC_TABLE_ENTRY pEntry = &pAd->MacTab.Content[i];
        if ((pEntry->Valid != TRUE) || (pEntry->Sst != SST_ASSOC))
            continue;
    
        if (!CAP_IS_SHORT_PREAMBLE_ON(pEntry->CapabilityInfo))
        {
            DBGPRINT(RT_DEBUG_TRACE, "Long preamble capable STA exist\n");
            return TRUE;
        }
    }

    return FALSE;
}

BOOLEAN ApCheckAccessControlList(
    IN PRTMP_ADAPTER pAd,
    IN PUCHAR      pAddr,
    IN UCHAR         Apidx) 
{
	BOOLEAN Result = TRUE;

    if (pAd->PortCfg.MBSSID[Apidx].AccessControlList.Policy == 0)       // ACL is disabled
        Result = TRUE;
    else
    {
        ULONG i;
        if (pAd->PortCfg.MBSSID[Apidx].AccessControlList.Policy == 1)   // ACL is a positive list
            Result = FALSE;
        else                                              // ACL is a negative list
            Result = TRUE;
        for (i=0; i<pAd->PortCfg.MBSSID[Apidx].AccessControlList.Num; i++)
        {
            if (MAC_ADDR_EQUAL(pAddr, pAd->PortCfg.MBSSID[Apidx].AccessControlList.Entry[i].Addr))
            {
                Result = !Result;
                break;
            }
        }
    }

    if (Result == FALSE)
    {
        DBGPRINT(RT_DEBUG_TRACE, "%02x:%02x:%02x:%02x:%02x:%02x failed ACL checking\n",
        pAddr[0],pAddr[1],pAddr[2],pAddr[3],pAddr[4],pAddr[5]);
    }

    return Result;
}

VOID ApUpdateAccessControlList(
    IN PRTMP_ADAPTER pAd,
    IN UCHAR         Apidx)
{
    USHORT      AclIdx, MacIdx;
    BOOLEAN     Matched;

    // ACL is disabled. do nothing about the MAC table
    if (pAd->PortCfg.MBSSID[Apidx].AccessControlList.Policy == 0)
        return;

    for (MacIdx=0; MacIdx < MAX_LEN_OF_MAC_TABLE; MacIdx++)
    {
        if (! pAd->MacTab.Content[MacIdx].Valid) 
            continue;

        Matched = FALSE;
        for (AclIdx = 0; AclIdx < pAd->PortCfg.MBSSID[Apidx].AccessControlList.Num; AclIdx++)
        {
            if (MAC_ADDR_EQUAL(&pAd->MacTab.Content[MacIdx].Addr, pAd->PortCfg.MBSSID[Apidx].AccessControlList.Entry[AclIdx].Addr))
            {
                Matched = TRUE;
                break;
            }
        }
            
        if ((Matched == FALSE) && (pAd->PortCfg.MBSSID[Apidx].AccessControlList.Policy == 1))
        {
            DBGPRINT(RT_DEBUG_TRACE, "STA not on positive ACL. remove it...\n");
            MacTableDeleteEntry(pAd, pAd->MacTab.Content[MacIdx].Addr);
        }
        else if ((Matched == TRUE) && (pAd->PortCfg.MBSSID[Apidx].AccessControlList.Policy == 2))
        {
            DBGPRINT(RT_DEBUG_TRACE, "STA on negative ACL. remove it...\n");
            MacTableDeleteEntry(pAd, pAd->MacTab.Content[MacIdx].Addr);
        }
    }
}

UCHAR APAutoSelectChannel(
    IN PRTMP_ADAPTER	pAd)
{
	UCHAR ch, i;
    UCHAR dirtyness[MAX_NUM_OF_CHANNELS+1], dirty;
    UCHAR max_rssi[MAX_NUM_OF_CHANNELS+1];
    ULONG  FalseCca, FcsError;
    BOOLEAN bFindIt = FALSE;

    // passive scan. collect statistics
    NdisZeroMemory(dirtyness, MAX_NUM_OF_CHANNELS+1);

    if (pAd->PortCfg.PhyMode == PHY_11A)
    {
    	if (pAd->PortCfg.RadarDetect.IEEE80211H)
    	{
    		while(TRUE)
    		{
    			struct timeval tv;
    			
    			do_gettimeofday(&tv);
    			
    			ch = pAd->ChannelList[tv.tv_usec%pAd->ChannelListNum].Channel;

    			if (ch == 0)
    				ch = FirstChannel(pAd);
//    			if (!RadarChannelCheck(pAd, ch))
//    				continue;

    			for (i=0; i<pAd->ChannelListNum; i++)
    			{
    				if (pAd->ChannelList[i].Channel == ch)
    				{
    					if (pAd->ChannelList[i].RemainingTimeForUse == 0)
    						bFindIt = TRUE;
    					
   						break;
    				}
    			}
    			
    			if (bFindIt == TRUE)
    				break;
    		};
    	}
    	else
    	{
		    for (ch=0; ch<pAd->ChannelListNum; ch++)
		    {
		        AsicSwitchChannel(pAd, pAd->ChannelList[ch].Channel);
		        AsicLockChannel(pAd, pAd->ChannelList[ch].Channel);
		        pAd->Counters8023.GoodReceives = 0;
		        pAd->Counters8023.RxErrors = 0;
		        pAd->PortCfg.AutoChannel_MaxRssi = 0;
		        pAd->PortCfg.AutoChannel_Channel = pAd->ChannelList[ch].Channel;
		        max_rssi[ch]=0;
		        RTMPusecDelay(200000); // 0.2 sec at each channel
		        max_rssi[ch] = pAd->PortCfg.AutoChannel_MaxRssi;

		        RTMP_IO_READ32(pAd, STA_CSR1, &FalseCca);
		        FalseCca = FalseCca >> 16;
		        RTMP_IO_READ32(pAd, STA_CSR0, &FcsError);
		        FcsError &= 0x0000ffff;
		        
		        if (max_rssi[ch])
		        {
		            dirtyness[ch] += 10;
		        }
		        DBGPRINT(RT_DEBUG_TRACE,"Sleep at ch#%d to collect RX=%ld, RSSI=%d, CRC error=%d, False CCA=%d\n", 
		            pAd->ChannelList[ch].Channel, pAd->Counters8023.GoodReceives, max_rssi[ch] - pAd->BbpRssiToDbmDelta, FcsError, FalseCca);
		    }
	    
		    pAd->PortCfg.AutoChannel_Channel = 0;

		    DBGPRINT(RT_DEBUG_TRACE,"Dirtyness = %d.%d.%d.%d - %d.%d.%d.%d - %d.%d.%d.%d - %d.%d\n", 
		        dirtyness[0], dirtyness[1], dirtyness[2], dirtyness[3], dirtyness[4], dirtyness[5], dirtyness[6],
		        dirtyness[7], dirtyness[8], dirtyness[9], dirtyness[10], dirtyness[11], dirtyness[12], dirtyness[13]);

		    // RULE 1. pick up a good channel that no one used
		    for (ch=0; ch<pAd->ChannelListNum; ch++)
		    {
		        if (dirtyness[ch] == 0)		break;
		    }
		    if (ch < pAd->ChannelListNum)
		    {
		        DBGPRINT(RT_DEBUG_TRACE,"1.APAutoSelectChannel pick up ch#%d\n",pAd->ChannelList[ch].Channel);
		        return pAd->ChannelList[ch].Channel;
		    }

		    // RULE 2. if not available, pick up the channel with minimum RSSI
		    {
		        UCHAR min_rssi = 255, final_channel = 0;
		        
	            for (ch = 0; ch < pAd->ChannelListNum; ch++)
	            {
	                if (max_rssi[ch] < min_rssi)
	                {
	                    final_channel = ch;
	                    min_rssi = max_rssi[ch];
	                }
	            }
	            if (final_channel < pAd->ChannelListNum)
	            {
	                DBGPRINT(RT_DEBUG_TRACE,"2.APAutoSelectChannel pick up ch#%d\n",pAd->ChannelList[final_channel].Channel);
	                return pAd->ChannelList[final_channel].Channel;
	            }
		    }

		    // RULE 3. still not available, pick up the first channel
		    ch = FirstChannel(pAd);
		    DBGPRINT(RT_DEBUG_TRACE,"3.APAutoSelectChannel pick up ch#%d\n",ch);
    	}
		
    	DBGPRINT(RT_DEBUG_TRACE,"APAutoSelectChannel pick up ch#%d\n",ch);
    	return ch;
    }
    else
    {
	    for (ch=pAd->ChannelList[pAd->ChannelListNum-1].Channel; ch>=pAd->ChannelList[0].Channel; ch--)
	    {
	        AsicSwitchChannel(pAd, ch);
	        AsicLockChannel(pAd, ch);
	        pAd->Counters8023.GoodReceives = 0;
	        pAd->Counters8023.RxErrors = 0;
	        pAd->PortCfg.AutoChannel_MaxRssi = 0;
	        pAd->PortCfg.AutoChannel_Channel = ch;
	        max_rssi[ch]=0;
	        RTMPusecDelay(500000); // 0.5 sec at each channel
	        max_rssi[ch] = pAd->PortCfg.AutoChannel_MaxRssi;

	        RTMP_IO_READ32(pAd, STA_CSR1, &FalseCca);
	        FalseCca = FalseCca >> 16;
	        RTMP_IO_READ32(pAd, STA_CSR0, &FcsError);
	        FcsError &= 0x0000ffff;
	        
	        if (max_rssi[ch])
	        {
	            dirtyness[ch] += 10;
	            if (ch > 1) dirtyness[ch-1] += 1;
	            if (ch > 2) dirtyness[ch-2] += 1;
	            if (ch > 3) dirtyness[ch-3] += 1;
	            if (ch > 4) dirtyness[ch-4] += 1;
	            if (ch < 14) dirtyness[ch+1] += 1;
	            if (ch < 13) dirtyness[ch+2] += 1;
	            if (ch < 12) dirtyness[ch+3] += 1;
	            if (ch < 11) dirtyness[ch+4] += 1;
	        }
	        DBGPRINT(RT_DEBUG_TRACE,"Sleep at ch#%d to collect RX=%ld, RSSI=%d, CRC error=%d, False CCA=%d\n", 
	            ch, pAd->Counters8023.GoodReceives, max_rssi[ch] - pAd->BbpRssiToDbmDelta, FcsError, FalseCca);
	    }
    
	    pAd->PortCfg.AutoChannel_Channel = 0;

	    DBGPRINT(RT_DEBUG_TRACE,"Dirtyness = %d.%d.%d.%d - %d.%d.%d.%d - %d.%d.%d.%d - %d.%d\n", 
	        dirtyness[1], dirtyness[2], dirtyness[3], dirtyness[4], dirtyness[5], dirtyness[6], dirtyness[7],
	        dirtyness[8], dirtyness[9], dirtyness[10], dirtyness[11], dirtyness[12], dirtyness[13], dirtyness[14]);

	    // RULE 1. pick up a good channel that no one used
	    for (ch = FirstChannel(pAd); ch !=0; ch = NextChannel(pAd, ch))
	    {
	        if (dirtyness[ch] == 0) break;
	    }
	    if (ch != 0)
	    {
	        DBGPRINT(RT_DEBUG_TRACE,"2.APAutoSelectChannel pick up ch#%d\n",ch);
	        return ch;
	    }

	    // RULE 2. if not available, then co-use a channel that's no interference (dirtyness=10)
	    // RULE 3. if not available, then co-use a channel that has minimum interference (dirtyness=11,12)
	    for (dirty = 10; dirty <= 12; dirty++)
	    {
	        UCHAR candidate[MAX_NUM_OF_CHANNELS+1], candidate_num;
	        UCHAR min_rssi = 255, final_channel = 0;
	        
	        candidate_num = 0;
	        NdisZeroMemory(candidate, MAX_NUM_OF_CHANNELS+1);
	        for (ch = FirstChannel(pAd); ch != 0; ch = NextChannel(pAd, ch))
	        {
	            if (dirtyness[ch] == dirty) 
	            { 
	                candidate[ch]=1; 
	                candidate_num++; 
	            }
	        }
	        // if there's more than 1 candidate, pick up the channel with minimum RSSI
	        if (candidate_num)
	        {
	            for (ch = 1; ch <= 14; ch ++)
	            {
	                if (candidate[ch] && (max_rssi[ch] < min_rssi))
	                {
	                    final_channel = ch;
	                    min_rssi = max_rssi[ch];
	                }
	            }
	            if (final_channel != 0)
	            {
	                DBGPRINT(RT_DEBUG_TRACE,"3.APAutoSelectChannel pick up ch#%d\n",final_channel);
	                return final_channel;
	            }
	        }
	    }

	    // RULE 4. still not available, pick up the first channel
	    ch = FirstChannel(pAd);
	    DBGPRINT(RT_DEBUG_TRACE,"4.APAutoSelectChannel pick up ch#%d\n",ch);
	    
	    return ch;
    }
}

/* 
    ==========================================================================
    Description:
        Send out a NULL frame to a specified STA at a higher TX rate. The 
        purpose is to ensure the designated client is okay to received at this
        rate.
    ==========================================================================
 */
VOID ApEnqueueNullFrame(
    IN PRTMP_ADAPTER pAd,
    IN PUCHAR        pAddr,
    IN UCHAR         TxRate,
    IN UCHAR         PID,
    IN UCHAR         Apidx,
    IN BOOLEAN       bQosNull,
    IN BOOLEAN       bEOSP,
    IN UCHAR         OldUP) 
{
	UCHAR			NullFrame[48];
    PHEADER_802_11 pNullFr;
    ULONG			Length;

    NdisZeroMemory(NullFrame, 48);
	Length = sizeof(HEADER_802_11);

    pNullFr = (PHEADER_802_11)NullFrame;

    // since TxRate may change, we have to change Duration each time
    if (pNullFr)
    {
#ifdef APCLI_SUPPORT    	
    	if ((PID & 0x3f) >= APCLI_PAIRWISE_KEY_OFFSET)
        {
    		// Send to Ap Client
            UCHAR ToWhichApCli = (PID & 0x3f) - APCLI_PAIRWISE_KEY_OFFSET;
            	
            NdisZeroMemory(pNullFr, sizeof(HEADER_802_11));
            pNullFr->FC.Type = BTYPE_DATA;
    		pNullFr->FC.SubType = SUBTYPE_NULL_FUNC;
    		pNullFr->FC.ToDs = 1;
    		COPY_MAC_ADDR(pNullFr->Addr1, pAd->ApCliTab.ApCliEntry[ToWhichApCli].ApCliBssid);
    		COPY_MAC_ADDR(pNullFr->Addr2, pAd->ApCliTab.ApCliEntry[ToWhichApCli].CurrentAddress);
    		COPY_MAC_ADDR(pNullFr->Addr3, pAd->ApCliTab.ApCliEntry[ToWhichApCli].ApCliBssid);
            pNullFr->Duration = RTMPCalcDuration(pAd, TxRate, 14);
#if defined(NULL_FRAME_SW_QUEUE_SUPPORT) && (NULL_FRAME_SW_QUEUE_SUPPORT == 1)
            ApSendNullFrame(pAd, INT_APCLI, ToWhichApCli, pNullFr, Length, TxRate, PID);
#else
            ApSendFrame(pAd, pNullFr, Length, TxRate, PID);
#endif
        }
        else
#endif    	
    	if ((PID & 0x3f) >= WDS_PAIRWISE_KEY_OFFSET)
    	{	// Send to WDS
        	UCHAR ToWhichWds = (PID & 0x3f) - WDS_PAIRWISE_KEY_OFFSET;
        	
            NdisZeroMemory(pNullFr, LENGTH_802_11_WITH_ADDR4);
            pNullFr->FC.Type = BTYPE_DATA;
            pNullFr->FC.SubType = SUBTYPE_NULL_FUNC;
            pNullFr->FC.FrDs = 1;
            pNullFr->FC.ToDs = 1;
            COPY_MAC_ADDR(pNullFr->Addr1, pAd->WdsTab.WdsEntry[ToWhichWds].WdsAddr);
            COPY_MAC_ADDR(pNullFr->Addr2, pAd->PortCfg.MBSSID[MAIN_MBSSID].Bssid);
            COPY_MAC_ADDR(pNullFr->Addr3, pAd->WdsTab.WdsEntry[ToWhichWds].WdsAddr);
            COPY_MAC_ADDR(pNullFr->Addr3 + MAC_ADDR_LEN, pAd->PortCfg.MBSSID[MAIN_MBSSID].Bssid);
            pNullFr->Duration = RTMPCalcDuration(pAd, TxRate, 14);
            Length = LENGTH_802_11_WITH_ADDR4;
#if defined(NULL_FRAME_SW_QUEUE_SUPPORT) && (NULL_FRAME_SW_QUEUE_SUPPORT == 1)
            ApSendNullFrame(pAd, INT_WDS, ToWhichWds, pNullFr, Length, TxRate, PID);
#else
            ApSendFrame(pAd, pNullFr, LENGTH_802_11_WITH_ADDR4, TxRate, PID);
#endif
        }
    	else
    	{	// Send to BSS
    		MgtMacHeaderInit(pAd, pNullFr, SUBTYPE_NULL_FUNC, 0, pAddr, pAd->PortCfg.MBSSID[Apidx].Bssid);
            pNullFr->FC.Type = BTYPE_DATA;
            pNullFr->FC.FrDs = 1;
            pNullFr->Duration = RTMPCalcDuration(pAd, TxRate, 14);
            if (bQosNull)
			{
				pNullFr->FC.SubType = SUBTYPE_QOS_NULL;

				// copy QOS control bytes
				NullFrame[Length]	= ((bEOSP) ? (1 << 4) : 0) | OldUP;
				NullFrame[Length+1]	= 0;
				Length += 2;
			}
#if defined(NULL_FRAME_SW_QUEUE_SUPPORT) && (NULL_FRAME_SW_QUEUE_SUPPORT == 1)
//iverson
//            ApSendNullFrame(pAd, INT_MBSSID, 0, pNullFr, Length, TxRate, PID);
              MiniportMMRequest(pAd, pNullFr, Length);
//iverson end

#else
            ApSendFrame(pAd, pNullFr, Length, TxRate, PID);
#endif
    	}	
#if defined(NULL_FRAME_SW_QUEUE_SUPPORT) && (NULL_FRAME_SW_QUEUE_SUPPORT == 1)
		RTMPDeQueuePacket(pAd, FALSE, FALSE);
#endif
    }
}

VOID    ApSendFrame(
    IN  PRTMP_ADAPTER   pAd,
    IN  PVOID           pBuffer,
    IN  ULONG           Length,
    IN  UCHAR           TxRate,
    IN  UCHAR           PID)
{
    PUCHAR          pDest;
    PTXD_STRUC      pTxD;
    PRTMP_TX_RING   pTxRing = &pAd->TxRing[QID_AC_BE];
    UMAXLONG		IrqFlags;
#ifdef BIG_ENDIAN
    PTXD_STRUC      pDestTxD;
    TXD_STRUC       TxD;
#endif

    if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RESET_IN_PROGRESS)	||
    	RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF)			||
    	RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS))
    {
        return;
    }

    // Make sure Tx ring resource won't be used by other threads
#ifndef THREAD_ISR
    RTMP_SEM_LOCK(&pAd->TxRingLock, IrqFlags);
#else
	RTMP_IRQ_LOCK(IrqFlags);
#endif

    // Get the Tx Ring descriptor & Dma Buffer address
    pDest = (PUCHAR) pTxRing->Cell[pTxRing->CurTxIndex].DmaBuf.AllocVa;
#ifndef BIG_ENDIAN
    pTxD  = (PTXD_STRUC) pTxRing->Cell[pTxRing->CurTxIndex].AllocVa;
#else
    pDestTxD  = (PTXD_STRUC) pTxRing->Cell[pTxRing->CurTxIndex].AllocVa;
    TxD = *pDestTxD;
    pTxD = &TxD;
    RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif

    if ((pTxD->Owner == DESC_OWN_NIC) || (pTxD->bWaitingDmaDoneInt == 1))
    {
        // Descriptor owned by NIC. No descriptor avaliable
        // This should not happen since caller guaranteed.
        DBGPRINT_ERR("APSendNullFrame: TX RING full\n");
        pAd->RalinkCounters.TxRingErrCount++;
#ifndef THREAD_ISR      
        RTMP_SEM_UNLOCK(&pAd->TxRingLock, IrqFlags);
#else
		RTMP_IRQ_UNLOCK(IrqFlags);
#endif
        return;
    }
        
    DBGPRINT(RT_DEBUG_TRACE, "send NULL Frame @%d Mbps...\n", RateIdToMbps[TxRate]);
#ifdef BIG_ENDIAN
   RTMPFrameEndianChange(pAd, (PUCHAR)pBuffer, DIR_WRITE, FALSE);
#endif
    NdisMoveMemory(pDest, pBuffer, Length);
//  pTxRing->Cell[pTxRing->CurTxIndex].FrameType = BTYPE_DATA;

	pTxD->BufCount = 1;
	pTxD->BufLen0  = Length;

#ifdef BIG_ENDIAN
    RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
    *pDestTxD = TxD;
    pTxD = pDestTxD;
#endif

#ifdef CONFIG_64BIT
	RTMP_DMACB *dmacb_p = &pTxRing->Cell[pTxRing->CurTxIndex];

    RTMPWriteTxDescriptor(pAd, 
                          pTxD, 
                          CIPHER_NONE, 0, 0, 
                          TRUE, 
                          FALSE, 
                          FALSE, 
                          SHORT_RETRY, 
                          IFS_BACKOFF, 
                          TxRate, 
                          Length, 
                          QID_AC_BE,
                          PID,
                          NULL,
                          NULL,
                          FALSE,
                          FALSE,
                          FALSE,
                          FALSE,
                          dmacb_p);
#else
	RTMPWriteTxDescriptor(pAd, 
                          pTxD, 
                          CIPHER_NONE, 0, 0, 
                          TRUE, 
                          FALSE, 
                          FALSE, 
                          SHORT_RETRY, 
                          IFS_BACKOFF, 
                          TxRate, 
                          Length, 
                          QID_AC_BE,
                          PID,
                          NULL,
                          NULL,
                          FALSE,
                          FALSE,
                          FALSE,
                          FALSE);
#endif // CONFIG_64BIT

    INC_RING_INDEX(pTxRing->CurTxIndex, TX_RING_SIZE);
    pAd->RalinkCounters.KickTxCount++;
#ifndef THREAD_ISR    
    RTMP_SEM_UNLOCK(&pAd->TxRingLock, IrqFlags);
#else
	RTMP_IRQ_UNLOCK(IrqFlags);
#endif

    RTMP_IO_WRITE32(pAd, TX_CNTL_CSR, BIT8[QID_AC_BE]); // use AC_BE
}

VOID RTMPHandleNotify8021xDiscardSta(
    IN  PRTMP_ADAPTER	pAd,
    IN  MAC_TABLE_ENTRY *pEntry)
{
	INT				apidx = MAIN_MBSSID;
	UCHAR			SrcAddr[MAC_ADDR_LEN];
    UCHAR			DestAddr[MAC_ADDR_LEN];	
				
	apidx = pEntry->ApIdx;        
    COPY_MAC_ADDR(SrcAddr, pEntry->Addr);
    COPY_MAC_ADDR(DestAddr, pAd->PortCfg.MBSSID[apidx].Bssid);
    
    if((pEntry->AuthMode == Ndis802_11AuthModeWPA) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2) || (pAd->PortCfg.MBSSID[apidx].IEEE8021X == TRUE))
	{
			UCHAR           	LLC_Len[2];
			UCHAR				Header802_3[14];
			UCHAR				RalinkDiscardIe[] = {0x01, 0xFE, 0x00, 0x06, 0x52, 0x61, 0x6c, 0x69, 0x6e, 0x6b};
			struct net_device	*net_dev = pAd->net_dev;
		                		
			net_dev = pAd->PortCfg.MBSSID[apidx].MSSIDDev;
			DBGPRINT(RT_DEBUG_TRACE, "Notify 8021.x daemon to remove this sta(%x %x %x %x %x %x)\n",SrcAddr[0],SrcAddr[1],SrcAddr[2],SrcAddr[3],SrcAddr[4],SrcAddr[5]);
		    
		    NdisMoveMemory(LLC_Len, EAPOL, 2);	
           	MAKE_802_3_HEADER(Header802_3, DestAddr, SrcAddr, ((PUCHAR) LLC_Len));
			
#ifdef RTL865X_SOC
		    REPORT_ETHERNET_FRAME_TO_LLC_RTL865X(pAd, Header802_3, RalinkDiscardIe, 10, net_dev);						
#else
		    REPORT_ETHERNET_FRAME_TO_LLC(pAd, Header802_3, RalinkDiscardIe, 10, net_dev);
#endif
	}	
}	    

VOID ApSendNullFrame(
	IN PRTMP_ADAPTER pAd,
	IN ULONG ifType,
	IN UCHAR ifIndex,
	IN PVOID pBuffer,
	IN ULONG Length,
	IN UCHAR TxRate,
	IN UCHAR PID)
{
	UMAXLONG IrqFlags;
	struct sk_buff	*pSkb;

	if (pAd->NullFrameTxSwQueue.Number >= MAX_NULL_FRAME_SIZE)
	{
		static ULONG reachedCnt = 0;
		if((reachedCnt++) % 100 == 0)
			DBGPRINT(RT_DEBUG_TRACE, "NullFramSwQueue Full !!!\n");
		return;
	}

	if ((pSkb = __dev_alloc_skb(Length + 2, MEM_ALLOC_FLAG)) != NULL)
	{
		memcpy(skb_put(pSkb, Length), pBuffer, Length);

		switch(ifType)
		{
		case INT_MBSSID:
			RTMP_SET_PACKET_NET_DEVICE_MBSSID(pSkb, ifIndex);
			break;

		case INT_APCLI:
			RTMP_SET_PACKET_NET_DEVICE_APCLI(pSkb, ifIndex);
			break;

		case INT_WDS:
			RTMP_SET_PACKET_NET_DEVICE_WDS(pSkb, ifIndex);
			break;

		default:
			return;
		}

		SetNullFrameAID(pSkb, PID);
		RTMP_SET_PACKET_TXRATE(pSkb, TxRate);

#ifndef THREAD_ISR
		RTMP_SEM_LOCK(&pAd->TxSwQueueLock, IrqFlags);
#else
		RTMP_IRQ_LOCK(IrqFlags);
#endif
		InsertTailQueue(&pAd->NullFrameTxSwQueue, pSkb);
#ifndef THREAD_ISR
		RTMP_SEM_UNLOCK(&pAd->TxSwQueueLock, IrqFlags);
#else
		RTMP_IRQ_UNLOCK(IrqFlags);
#endif

		DBGPRINT(RT_DEBUG_INFO, "%s TxRate=%d, PID=%d\n", __FUNCTION__, TxRate, PID);
	}
	return;
}

NDIS_STATUS NullFrameHardTransmit(
	IN PRTMP_ADAPTER pAd,
	IN struct sk_buff *pSkb,
	IN UCHAR TxRate,
	IN UCHAR PID)
{
	PUCHAR pDest;
	PTXD_STRUC pTxD;
	PRTMP_TX_RING pTxRing = &pAd->TxRing[QID_AC_BE];
#ifndef THREAD_ISR
	UMAXLONG IrqFlags;
#endif // THREAD_ISR //
#ifdef BIG_ENDIAN
	PTXD_STRUC pDestTxD;
	TXD_STRUC TxD;
#endif
	PVOID pBuffer = pSkb->data;
	ULONG Length = pSkb->len;

	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RESET_IN_PROGRESS)	||
		RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF)			||
		RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS))
	{
#ifdef RTL865X_FAST_PATH
		rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
		RELEASE_NDIS_PACKET(pAd, pSkb);
#endif
		return (NDIS_STATUS_FAILURE);
	}

	// Make sure Tx ring resource won't be used by other threads
#ifndef THREAD_ISR
	RTMP_SEM_LOCK(&pAd->TxRingLock, IrqFlags);
#else
	/*
	 * This function must be called in locked IRQ mode
	 */
	//RTMP_IRQ_LOCK(IrqFlags);
#endif

	// Get the Tx Ring descriptor & Dma Buffer address
	pDest = (PUCHAR) pTxRing->Cell[pTxRing->CurTxIndex].DmaBuf.AllocVa;
#ifndef BIG_ENDIAN
	pTxD  = (PTXD_STRUC) pTxRing->Cell[pTxRing->CurTxIndex].AllocVa;
#else
	pDestTxD  = (PTXD_STRUC) pTxRing->Cell[pTxRing->CurTxIndex].AllocVa;
	TxD = *pDestTxD;
	pTxD = &TxD;
	RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif

	if ((pTxD->Owner == DESC_OWN_NIC) || (pTxD->bWaitingDmaDoneInt == 1))
	{
		// Descriptor owned by NIC. No descriptor avaliable
		// This should not happen since caller guaranteed.
		DBGPRINT_ERR("APSendNullFrame: TX RING full\n");
		pAd->RalinkCounters.TxRingErrCount++;

#ifndef THREAD_ISR
		RTMP_SEM_UNLOCK(&pAd->TxRingLock, IrqFlags);
#else
		/*
		 * This function must be called in locked IRQ mode
		 */
		//RTMP_IRQ_UNLOCK(IrqFlags);
#endif
		return (NDIS_STATUS_RESOURCES);
	}
        
	DBGPRINT(RT_DEBUG_TRACE, "send NULL Frame @%d Mbps...\n", RateIdToMbps[TxRate]);
#ifdef BIG_ENDIAN
	RTMPFrameEndianChange(pAd, (PUCHAR)pBuffer, DIR_WRITE, FALSE);
#endif
	NdisMoveMemory(pDest, pBuffer, Length);
//  pTxRing->Cell[pTxRing->CurTxIndex].FrameType = BTYPE_DATA;

	pTxD->BufCount = 1;
	pTxD->BufLen0  = Length;

#ifdef RTL865X_FAST_PATH
	rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
	RELEASE_NDIS_PACKET(pAd, pSkb);
#endif

#ifdef BIG_ENDIAN
	RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
	*pDestTxD = TxD;
	pTxD = pDestTxD;
#endif

#ifdef CONFIG_64BIT
	{
    RTMP_DMACB *dmacb_p = &pTxRing->Cell[pTxRing->CurTxIndex];

	RTMPWriteTxDescriptor(pAd, 
						pTxD, 
						CIPHER_NONE, 0, 0, 
						TRUE, 
						FALSE, 
						FALSE, 
						SHORT_RETRY, 
						IFS_BACKOFF, 
						TxRate, 
						Length, 
						QID_AC_BE,
						PID,
						NULL,
						NULL,
						FALSE,
						FALSE,
						FALSE,
						FALSE,
						dmacb_p);
	}
#else
	RTMPWriteTxDescriptor(pAd, 
						pTxD, 
						CIPHER_NONE, 0, 0, 
						TRUE, 
						FALSE, 
						FALSE, 
						SHORT_RETRY, 
						IFS_BACKOFF, 
						TxRate, 
						Length, 
						QID_AC_BE,
						PID,
						NULL,
						NULL,
						FALSE,
						FALSE,
						FALSE,
						FALSE);
#endif // CONFIG_64BIT //

	INC_RING_INDEX(pTxRing->CurTxIndex, TX_RING_SIZE);
	pAd->RalinkCounters.KickTxCount++;
#ifndef THREAD_ISR    
	RTMP_SEM_UNLOCK(&pAd->TxRingLock, IrqFlags);
#else
	/*
	 * This function must be called in locked IRQ mode
	 */
	//RTMP_IRQ_UNLOCK(IrqFlags);
#endif

	RTMP_IO_WRITE32(pAd, TX_CNTL_CSR, BIT8[QID_AC_BE]); // use AC_BE

	return (NDIS_STATUS_SUCCESS);
}

VOID SetNullFrameAID(
	IN struct sk_buff *pSkb,
	IN UCHAR PID)
{
	UCHAR AID;

#ifdef APCLI_SUPPORT
	if(PID >= (PTYPE_NULL_AT_HIGH_RATE + APCLI_PAIRWISE_KEY_OFFSET))
	{ // To AP Client interface
		AID = PID - PTYPE_NULL_AT_HIGH_RATE - APCLI_PAIRWISE_KEY_OFFSET;
	}
	else
#endif // APCLI_SUPPORT //

#ifdef WDS_SUPPORT
	if(PID >= (PTYPE_NULL_AT_HIGH_RATE + WDS_PAIRWISE_KEY_OFFSET))
	{  // To WDS interface
		AID = PID - PTYPE_NULL_AT_HIGH_RATE - WDS_PAIRWISE_KEY_OFFSET;
	}
	else
#endif // WDS_SUPPORT //

	{  // Normal packet or MBSSID packet
		AID = PID - PTYPE_NULL_AT_HIGH_RATE;
	}

	RTMP_SET_PACKET_AID(pSkb, AID);
	DBGPRINT(RT_DEBUG_INFO, "%s, PID=%d, AID=%d\n", __FUNCTION__, PID, AID);
	return;
}

UCHAR GetNullFramePID(
	IN struct sk_buff *pSkb)
{
	UCHAR PID;
	
#ifdef APCLI_SUPPORT
	if (RTMP_GET_PACKET_NET_DEVICE(pSkb) >= MIN_NET_DEVICE_FOR_APCLI)
	{ // To AP Client interface
		PID = PTYPE_NULL_AT_HIGH_RATE + APCLI_PAIRWISE_KEY_OFFSET + RTMP_GET_PACKET_AID(pSkb);
	}
	else
#endif // APCLI_SUPPORT //

#ifdef WDS_SUPPORT
	if (RTMP_GET_PACKET_NET_DEVICE(pSkb) >= MIN_NET_DEVICE_FOR_WDS)
	{  // To WDS Client interface
		PID = PTYPE_NULL_AT_HIGH_RATE + WDS_PAIRWISE_KEY_OFFSET + RTMP_GET_PACKET_AID(pSkb);
	}
	else 	// Get the tx rate and BSSID index
#endif // WDS_SUPPORT //

	{ // Normal packet or MBSSID packet
		PID =  PTYPE_NULL_AT_HIGH_RATE + RTMP_GET_PACKET_AID(pSkb);
	}

	DBGPRINT(RT_DEBUG_INFO,"%s, PID=%d\n", __FUNCTION__, PID);
	return PID;
}

