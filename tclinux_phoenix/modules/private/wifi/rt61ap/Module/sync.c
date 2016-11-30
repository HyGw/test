/****************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 * (c) Copyright 2005, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ****************************************************************************
     
     Module Name:
     sync.c
     
     Abstract:
     Synchronization state machine related services
     
     Revision History:
     Who         When          What
     --------    ----------    ----------------------------------------------
     John Chang  08-04-2003    created for 11g soft-AP
     
 */

#include "rt_config.h"

// 2.4 Ghz channel plan index in the TxPower arrays.
#define	BG_BAND_REGION_0_START	0			// 1,2,3,4,5,6,7,8,9,10,11	
#define	BG_BAND_REGION_0_SIZE	11
#define	BG_BAND_REGION_1_START	0			// 1,2,3,4,5,6,7,8,9,10,11,12,13
#define	BG_BAND_REGION_1_SIZE	13
#define	BG_BAND_REGION_2_START	9			// 10,11
#define	BG_BAND_REGION_2_SIZE	2
#define	BG_BAND_REGION_3_START	9			// 10,11,12,13
#define	BG_BAND_REGION_3_SIZE	4
#define	BG_BAND_REGION_4_START	13			// 14
#define	BG_BAND_REGION_4_SIZE	1
#define	BG_BAND_REGION_5_START	0			// 1,2,3,4,5,6,7,8,9,10,11,12,13,14 
#define	BG_BAND_REGION_5_SIZE	14
#define	BG_BAND_REGION_6_START	2			// 3,4,5,6,7,8,9
#define	BG_BAND_REGION_6_SIZE	7
#define	BG_BAND_REGION_7_START	4			// 5,6,7,8,9,10,11,12,13
#define	BG_BAND_REGION_7_SIZE	9

// 5 Ghz channel plan index in the TxPower arrays.
UCHAR A_BAND_REGION_0_CHANNEL_LIST[]={36, 40, 44, 48, 52, 56, 60, 64, 149, 153, 157, 161, 165};
UCHAR A_BAND_REGION_1_CHANNEL_LIST[]={36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140};
UCHAR A_BAND_REGION_2_CHANNEL_LIST[]={36, 40, 44, 48, 52, 56, 60, 64};
UCHAR A_BAND_REGION_3_CHANNEL_LIST[]={52, 56, 60, 64, 149, 153, 157, 161};
UCHAR A_BAND_REGION_4_CHANNEL_LIST[]={149, 153, 157, 161, 165};
UCHAR A_BAND_REGION_5_CHANNEL_LIST[]={149, 153, 157, 161};
UCHAR A_BAND_REGION_6_CHANNEL_LIST[]={36, 40, 44, 48};
UCHAR A_BAND_REGION_7_CHANNEL_LIST[]={36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 149, 153, 157, 161, 165};
UCHAR A_BAND_REGION_8_CHANNEL_LIST[]={52, 56, 60, 64};
UCHAR A_BAND_REGION_9_CHANNEL_LIST[]={34, 38, 42, 46};
UCHAR A_BAND_REGION_10_CHANNEL_LIST[]={34, 36, 38, 40, 42, 44, 46, 48, 52, 56, 60, 64}; 
/*
    ==========================================================================
    Description:
        The sync state machine, 
    Parameters:
        Sm - pointer to the state machine
    Note:
        the state machine looks like the following
        
                        SYNC_IDLE             
    MT2_PEER_PROBE_REQ  peer_probe_req_action    
    ==========================================================================
 */
VOID SyncStateMachineInit(
	IN PRTMP_ADAPTER pAd, 
	IN STATE_MACHINE *Sm, 
	OUT STATE_MACHINE_FUNC Trans[])
{
	StateMachineInit(Sm, (STATE_MACHINE_FUNC*)Trans, MAX_SYNC_STATE, MAX_SYNC_MSG, (STATE_MACHINE_FUNC)Drop, SYNC_IDLE, SYNC_MACHINE_BASE);

	// idle state
	StateMachineSetAction(Sm, SYNC_IDLE, MT2_PEER_PROBE_REQ, (STATE_MACHINE_FUNC)PeerProbeReqAction);
	StateMachineSetAction(Sm, SYNC_IDLE, MT2_PEER_BEACON, (STATE_MACHINE_FUNC)PeerBeaconAction);
	StateMachineSetAction(Sm, SYNC_IDLE, MT2_MLME_SCAN_REQ, (STATE_MACHINE_FUNC)MlmeScanReqAction);

	// scan_listen state
	StateMachineSetAction(Sm, SCAN_LISTEN, MT2_MLME_SCAN_REQ, (STATE_MACHINE_FUNC)InvalidStateWhenScan);
	StateMachineSetAction(Sm, SCAN_LISTEN, MT2_PEER_BEACON, (STATE_MACHINE_FUNC)PeerBeaconAtScanAction);
	StateMachineSetAction(Sm, SCAN_LISTEN, MT2_PEER_PROBE_RSP, (STATE_MACHINE_FUNC)PeerBeaconAtScanAction);
	StateMachineSetAction(Sm, SCAN_LISTEN, MT2_SCAN_TIMEOUT, (STATE_MACHINE_FUNC)ScanTimeoutAction);

	RTMPInitTimer(pAd, &pAd->Mlme.SyncAux.ScanTimer, (PVOID)&ScanTimeout);
}

/* 
    ==========================================================================
    Description:
        Scan timeout handler, executed in timer thread
    ==========================================================================
 */
VOID ScanTimeout(
	IN  unsigned long data)
{
	RTMP_ADAPTER *pAd = (RTMP_ADAPTER *)data;

	DBGPRINT(RT_DEBUG_INFO,"SYNC - Scan Timeout \n");
	MlmeEnqueue(pAd, SYNC_STATE_MACHINE, MT2_SCAN_TIMEOUT, 0, NULL);
	MlmeHandler(pAd);
}

/* 
    ==========================================================================
    Description:
    ==========================================================================
 */
VOID InvalidStateWhenScan(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem)
{
	DBGPRINT(RT_DEBUG_TRACE, "AYNC - InvalidStateWhenScan(state=%d). Reset SYNC machine\n", pAd->Mlme.SyncMachine.CurrState);
}

/* 
    ==========================================================================
    Description:
        Scan timeout procedure. basically add channel index by 1 and rescan
    ==========================================================================
 */
VOID ScanTimeoutAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem)
{
	pAd->Mlme.SyncAux.Channel = NextChannel(pAd, pAd->Mlme.SyncAux.Channel);
	ScanNextChannel(pAd);
}

/* 
    ==========================================================================
    Description:
        peer sends beacon back when scanning
    ==========================================================================
 */
VOID PeerBeaconAtScanAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem)
{
    UCHAR           Bssid[MAC_ADDR_LEN], Addr2[MAC_ADDR_LEN];
    UCHAR           Ssid[MAX_LEN_OF_SSID], BssType, Channel,  
                    SsidLen, DtimCount, DtimPeriod, BcastFlag, MessageToMe;
    CF_PARM         CfParm;
    USHORT          BeaconPeriod, AtimWin, CapabilityInfo;
    PFRAME_802_11   pFrame;
    LARGE_INTEGER   TimeStamp;
    UCHAR           Erp;
	UCHAR         	SupRate[MAX_LEN_OF_SUPPORTED_RATES], ExtRate[MAX_LEN_OF_SUPPORTED_RATES];
	UCHAR		  	SupRateLen, ExtRateLen;
	UCHAR			CkipFlag;
	UCHAR			AironetCellPowerLimit;
	EDCA_PARM       EdcaParm;
	QBSS_LOAD_PARM  QbssLoad;
	QOS_CAPABILITY_PARM QosCapability;
    ULONG           RalinkIe;
	UCHAR						VarIE[MAX_VIE_LEN];		// Total VIE length = MAX_VIE_LEN - -5
    UCHAR           LenVIE;
	NDIS_802_11_VARIABLE_IEs	*pVIE = NULL;

    pFrame = (PFRAME_802_11) Elem->Msg;
	// Init Variable IE structure
	pVIE = (PNDIS_802_11_VARIABLE_IEs) VarIE;
	pVIE->Length = 0;
    if (PeerBeaconAndProbeRspSanity(pAd, 
                                Elem->Msg, 
                                Elem->MsgLen, 
                                Addr2, 
                                Bssid, 
                                Ssid, 
                                &SsidLen, 
                                &BssType, 
                                &BeaconPeriod, 
                                &Channel, 
                                &TimeStamp, 
                                &CfParm, 
                                &AtimWin, 
                                &CapabilityInfo, 
                                &Erp,
                                &DtimCount, 
                                &DtimPeriod, 
                                &BcastFlag, 
                                &MessageToMe, 
								SupRate,
								&SupRateLen,
								ExtRate,
								&ExtRateLen,
								&CkipFlag,
								&AironetCellPowerLimit,
								&EdcaParm,
								&QbssLoad,
								&QosCapability,
								&RalinkIe,
				                &LenVIE,
                                pVIE)) 
    {
		ULONG Idx;
		UCHAR Rssi = 0;
        CHAR  RealRssi;

		// This correct im-proper RSSI indication during SITE SURVEY issue.
		// Always report bigger RSSI during SCANNING when receiving multiple BEACONs from the same AP. 
		// This case happens because BEACONs come from adjacent channels, so RSSI become weaker as we 
		// switch to more far away channels.
        Idx = BssTableSearch(&pAd->ScanTab, Bssid);
		if (Idx != BSS_NOT_FOUND) 
            Rssi = pAd->ScanTab.BssEntry[Idx].Rssi;

        // TODO: 2005-03-04 dirty patch. we should change all RSSI related variables to SIGNED SHORT for easy/efficient reading and calaulation
        RealRssi = ConvertToRssi(pAd, Elem->Rssi, RSSI_NO_1);
        if ((RealRssi + pAd->BbpRssiToDbmDelta) > Rssi)
            Rssi = RealRssi + pAd->BbpRssiToDbmDelta;

    	Idx = BssTableSetEntry(pAd, &pAd->ScanTab, Bssid, Ssid, SsidLen, BssType, 
                     BeaconPeriod, &CfParm, AtimWin, CapabilityInfo, SupRate, 
                     SupRateLen, ExtRate, ExtRateLen, Channel, Rssi, TimeStamp, CkipFlag, 
                     &EdcaParm, &QosCapability, &QbssLoad, LenVIE, pVIE);
	}
	// sanity check fail, ignored
}

/* 
    ==========================================================================
    Description:
        Scan next channel
    ==========================================================================
 */
VOID ScanNextChannel(
	IN PRTMP_ADAPTER pAd)
{
    HEADER_802_11   Hdr80211;
    PUCHAR          pOutBuffer = NULL;
    ULONG           FrameLen = 0;
    UCHAR           SsidLen = 0;
    USHORT          Status;
	UCHAR			SsidIe    = IE_SSID;
	UCHAR			SupRateIe = IE_SUPP_RATES;

    DBGPRINT(RT_DEBUG_INFO, "ScanNextChannel(ch=%d)\n",pAd->Mlme.SyncAux.Channel);

    if (pAd->Mlme.SyncAux.Channel == 0) 
    {
        DBGPRINT(RT_DEBUG_TRACE, "SYNC - End of SCAN, restore to channel %d\n",pAd->PortCfg.Channel);
        AsicSwitchChannel(pAd, pAd->PortCfg.Channel);
        AsicLockChannel(pAd, pAd->PortCfg.Channel);

        pAd->Mlme.SyncMachine.CurrState = SYNC_IDLE;

        RTMPResumeMsduTransmission(pAd);
        if (!((pAd->PortCfg.PhyMode == PHY_11A) && (pAd->PortCfg.RadarDetect.IEEE80211H == TRUE)))
        {
            AsicEnableBssSync(pAd);
        } 
    } 
    else 
    {
        AsicSwitchChannel(pAd, pAd->Mlme.SyncAux.Channel);
        AsicLockChannel(pAd, pAd->Mlme.SyncAux.Channel);
		
		// We need to shorten active scan time in order for WZC connect issue
		// Chnage the channel scan time for CISCO stuff based on its IAPP announcement
        if (pAd->Mlme.SyncAux.ScanType == SCAN_ACTIVE)
        {
			RTMPAddTimer(&pAd->Mlme.SyncAux.ScanTimer, (MIN_CHANNEL_TIME * HZ)/1000);
        }
        else // must be SCAN_PASSIVE
        {
        	if (pAd->PortCfg.PhyMode == PHY_11ABG_MIXED)
				RTMPAddTimer(&pAd->Mlme.SyncAux.ScanTimer, (MIN_CHANNEL_TIME * HZ)/1000);
			else
				RTMPAddTimer(&pAd->Mlme.SyncAux.ScanTimer, (MAX_CHANNEL_TIME * HZ)/1000);
        }

        if ((pAd->Mlme.SyncAux.ScanType == SCAN_ACTIVE) || 
            (pAd->Mlme.SyncAux.ScanType == SCAN_PASSIVE))
        {
        	pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
        	if (pOutBuffer == NULL)
	        {
    	        DBGPRINT(RT_DEBUG_TRACE, "SYNC - ScanNextChannel() allocate memory fail\n");
        	    pAd->Mlme.SyncMachine.CurrState = SYNC_IDLE;
                Status = MLME_FAIL_NO_RESOURCE;
	            return;
    	    }

    		// There is no need to send broadcast probe request if active scan is in effect.
            if (pAd->Mlme.SyncAux.ScanType == SCAN_ACTIVE)
                SsidLen = pAd->Mlme.SyncAux.SsidLen;
            else
                SsidLen = 0;

    		MgtMacHeaderInit(pAd, &Hdr80211, SUBTYPE_PROBE_REQ, 0, pAd->PortCfg.Broadcast, pAd->PortCfg.Broadcast);
            MakeOutgoingFrame(pOutBuffer,               &FrameLen,
                              sizeof(HEADER_802_11),    &Hdr80211,
                              1,                        &SsidIe,
                              1,                        &SsidLen,
                              SsidLen,			        pAd->Mlme.SyncAux.Ssid,
                              1,                        &SupRateIe,
                              1,                        &pAd->PortCfg.SupportedRatesLen,
	                          pAd->PortCfg.SupportedRatesLen, pAd->PortCfg.SupportedRates,
                              END_OF_ARGS);

            MiniportMMRequest(pAd, pOutBuffer, FrameLen);
        	kfree(pOutBuffer);
            DBGPRINT(RT_DEBUG_INFO, "SYNC - send ProbeReq @ channel=%d, Len=%d\n", pAd->Mlme.SyncAux.Channel, FrameLen);
        }

		// For SCAN_CISCO_PASSIVE, do nothing and silently wait for beacon or other probe reponse
		
        pAd->Mlme.SyncMachine.CurrState = SCAN_LISTEN;
    }
}

/* 
    ==========================================================================
    Description:
        This routine return the first channel number according to the country 
        code selection and RF IC selection (signal band or dual band). It is called
        whenever driver need to start a site survey of all supported channels.
    Return:
        ch - the first channel number of current country code setting
    ==========================================================================
 */
UCHAR FirstChannel(
    IN PRTMP_ADAPTER pAd)
{
	return pAd->ChannelList[0].Channel;
}

/* 
    ==========================================================================
    Description:
        This routine returns the next channel number. This routine is called
        during driver need to start a site survey of all supported channels.
    Return:
        next_channel - the next channel number valid in current country code setting.
    Note:
        return 0 if no more next channel
    ==========================================================================
 */
UCHAR NextChannel(
	IN PRTMP_ADAPTER pAd, 
	IN UCHAR channel)
{
	int i;
    UCHAR next_channel = 0;
			
    for (i = 0; i < (pAd->ChannelListNum - 1); i++)
        if (channel == pAd->ChannelList[i].Channel)
        {
            next_channel = pAd->ChannelList[i+1].Channel;
			break;
	}	
    return next_channel;
}

/* 
    ==========================================================================
    Description:
        Update PortCfg->ChannelList[] according to 1) Country Region 2) RF IC type,
        and 3) PHY-mode user selected.
        The outcome is used by driver when doing site survey.
    ==========================================================================
 */
VOID BuildChannelList(
	IN PRTMP_ADAPTER pAd)
{
    UCHAR i, j, index=0, num=0;
    PUCHAR	pChannelList=NULL;
	
    NdisZeroMemory(pAd->ChannelList, MAX_NUM_OF_CHANNELS * sizeof(CHANNEL_TX_POWER));
    
    // if not 11a-only mode, channel list starts from 2.4Ghz band
    if (pAd->PortCfg.PhyMode != PHY_11A)
    {
        switch (pAd->PortCfg.CountryRegion & 0x7f)
        {
            case REGION_0_BG_BAND:	// 1 -11
                NdisMoveMemory(&pAd->ChannelList[index], &pAd->TxPower[BG_BAND_REGION_0_START], sizeof(CHANNEL_TX_POWER) * BG_BAND_REGION_0_SIZE);
                index += BG_BAND_REGION_0_SIZE;
                break;
            case REGION_1_BG_BAND:	// 1 - 13
                NdisMoveMemory(&pAd->ChannelList[index], &pAd->TxPower[BG_BAND_REGION_1_START], sizeof(CHANNEL_TX_POWER) * BG_BAND_REGION_1_SIZE);
                index += BG_BAND_REGION_1_SIZE;
                break;
            case REGION_2_BG_BAND:	// 10 - 11
                NdisMoveMemory(&pAd->ChannelList[index], &pAd->TxPower[BG_BAND_REGION_2_START], sizeof(CHANNEL_TX_POWER) * BG_BAND_REGION_2_SIZE);
                index += BG_BAND_REGION_2_SIZE;
                break;
            case REGION_3_BG_BAND:	// 10 - 13
                NdisMoveMemory(&pAd->ChannelList[index], &pAd->TxPower[BG_BAND_REGION_3_START], sizeof(CHANNEL_TX_POWER) * BG_BAND_REGION_3_SIZE);
                index += BG_BAND_REGION_3_SIZE;
                break;
            case REGION_4_BG_BAND:	// 14
                NdisMoveMemory(&pAd->ChannelList[index], &pAd->TxPower[BG_BAND_REGION_4_START], sizeof(CHANNEL_TX_POWER) * BG_BAND_REGION_4_SIZE);
                index += BG_BAND_REGION_4_SIZE;
                break;
            case REGION_5_BG_BAND:	// 1 - 14
                NdisMoveMemory(&pAd->ChannelList[index], &pAd->TxPower[BG_BAND_REGION_5_START], sizeof(CHANNEL_TX_POWER) * BG_BAND_REGION_5_SIZE);
                index += BG_BAND_REGION_5_SIZE;
                break;
            case REGION_6_BG_BAND:	// 3 - 9
                NdisMoveMemory(&pAd->ChannelList[index], &pAd->TxPower[BG_BAND_REGION_6_START], sizeof(CHANNEL_TX_POWER) * BG_BAND_REGION_6_SIZE);
                index += BG_BAND_REGION_6_SIZE;
                break;
			case REGION_7_BG_BAND:	// 5 - 13
                NdisMoveMemory(&pAd->ChannelList[index], &pAd->TxPower[BG_BAND_REGION_7_START], sizeof(CHANNEL_TX_POWER) * BG_BAND_REGION_7_SIZE);
                index += BG_BAND_REGION_7_SIZE;
                break;
            default:            // Error. should never happen
                break;
        }   
    }
    
    if (pAd->PortCfg.PhyMode == PHY_11A)
    {
    	switch (pAd->PortCfg.CountryRegionForABand & 0x7f)
        {
            case REGION_0_A_BAND:
            	num = sizeof(A_BAND_REGION_0_CHANNEL_LIST)/sizeof(UCHAR);
	            pChannelList = A_BAND_REGION_0_CHANNEL_LIST;
                break;
            case REGION_1_A_BAND:
            	num = sizeof(A_BAND_REGION_1_CHANNEL_LIST)/sizeof(UCHAR);
            	pChannelList = A_BAND_REGION_1_CHANNEL_LIST;
                break;
            case REGION_2_A_BAND:
            	num = sizeof(A_BAND_REGION_2_CHANNEL_LIST)/sizeof(UCHAR);
            	pChannelList = A_BAND_REGION_2_CHANNEL_LIST;
                break;
            case REGION_3_A_BAND:
            	num = sizeof(A_BAND_REGION_3_CHANNEL_LIST)/sizeof(UCHAR);
            	pChannelList = A_BAND_REGION_3_CHANNEL_LIST;
                break;
            case REGION_4_A_BAND:
            	num = sizeof(A_BAND_REGION_4_CHANNEL_LIST)/sizeof(UCHAR);
            	pChannelList = A_BAND_REGION_4_CHANNEL_LIST;
                break;
            case REGION_5_A_BAND:
            	num = sizeof(A_BAND_REGION_5_CHANNEL_LIST)/sizeof(UCHAR);
            	pChannelList = A_BAND_REGION_5_CHANNEL_LIST;
                break;
            case REGION_6_A_BAND:
            	num = sizeof(A_BAND_REGION_6_CHANNEL_LIST)/sizeof(UCHAR);
            	pChannelList = A_BAND_REGION_6_CHANNEL_LIST;
                break;
            case REGION_7_A_BAND:
            	num = sizeof(A_BAND_REGION_7_CHANNEL_LIST)/sizeof(UCHAR);
            	pChannelList = A_BAND_REGION_7_CHANNEL_LIST;
                break;
	    case REGION_8_A_BAND:
            	num = sizeof(A_BAND_REGION_8_CHANNEL_LIST)/sizeof(UCHAR);
            	pChannelList = A_BAND_REGION_8_CHANNEL_LIST;
                break;

            case REGION_9_A_BAND:
                num = sizeof(A_BAND_REGION_9_CHANNEL_LIST)/sizeof(UCHAR);
                pChannelList = A_BAND_REGION_9_CHANNEL_LIST;
                break;
            case REGION_10_A_BAND:
                num = sizeof(A_BAND_REGION_10_CHANNEL_LIST)/sizeof(UCHAR);
                pChannelList = A_BAND_REGION_10_CHANNEL_LIST;
                break;  
	
            default:            // Error. should never happen
            	DBGPRINT(RT_DEBUG_WARN,"countryregion=%d not support", pAd->PortCfg.CountryRegionForABand);
                break;
        }   

		if (num != 0)
		{
	        for (i=0; i<num; i++)
	        {
	        	for (j=0; j<MAX_NUM_OF_CHANNELS; j++)
	        	{
	        		if (pChannelList[i] == pAd->TxPower[j].Channel)
	        		{
			        	NdisMoveMemory(&pAd->ChannelList[index+i], &pAd->TxPower[j], sizeof(CHANNEL_TX_POWER));
	        		}
	        	}
	        }
	        index += num;
		}
    }
    
    pAd->ChannelListNum = index;
    DBGPRINT(RT_DEBUG_TRACE,"country code=%d/%d, RFIC=%d, PHY mode=%d, support %d channels\n", 
        pAd->PortCfg.CountryRegion, pAd->PortCfg.CountryRegionForABand, pAd->RfIcType, pAd->PortCfg.PhyMode, pAd->ChannelListNum);
    for (i=0;i<index;i++)
    {
        DBGPRINT(RT_DEBUG_TRACE,"channel #%d\n", pAd->ChannelList[i].Channel);
    }
}

/* 
    ==========================================================================
    Description:
        MLME SCAN req state machine procedure
    ==========================================================================
 */
VOID MlmeScanReqAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem)
{
	UCHAR          Ssid[MAX_LEN_OF_SSID], SsidLen, ScanType, BssType;
	UMAXLONG       Now;

	// Suspend MSDU transmission here
	RTMPSuspendMsduTransmission(pAd);

	// first check the parameter sanity
	if (MlmeScanReqSanity(pAd, Elem->Msg, Elem->MsgLen, &BssType, Ssid, &SsidLen, &ScanType))
	{
		DBGPRINT(RT_DEBUG_TRACE, "SYNC - MlmeScanReqAction\n");
		Now = jiffies;
		pAd->LastScanTime = Now;

		RTMPCancelTimer(&pAd->Mlme.SyncAux.ScanTimer);

		// record desired BSS parameters
		pAd->Mlme.SyncAux.BssType = BssType;
		pAd->Mlme.SyncAux.ScanType = ScanType;
		pAd->Mlme.SyncAux.SsidLen = SsidLen;
		NdisMoveMemory(pAd->Mlme.SyncAux.Ssid, Ssid, SsidLen);

		// start from the first channel
		pAd->Mlme.SyncAux.Channel = FirstChannel(pAd);
		ScanNextChannel(pAd);
	}
	else
	{
		DBGPRINT(RT_DEBUG_ERROR, "SYNC - MlmeScanReqAction() sanity check fail. BUG!!!\n");
		pAd->Mlme.SyncMachine.CurrState = SYNC_IDLE;
	}
}

/*
    ==========================================================================
    Description:
        Process the received ProbeRequest from clients
    Parameters:
        Elem - msg containing the ProbeReq frame
    ==========================================================================
 */
VOID PeerProbeReqAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem) 
{
    UCHAR       Addr2[MAC_ADDR_LEN];
    CHAR          Ssid[MAX_LEN_OF_SSID];
    UCHAR         SsidLen, apidx = 0;
    HEADER_802_11        ProbeRspHdr;
    PUCHAR        pOutBuffer = NULL;
    ULONG         FrameLen = 0, TmpLen;
    LONGLONG      FakeTimestamp;
    UCHAR         SsidIe = IE_SSID, DsIe = IE_DS_PARM, SuppIe = IE_SUPP_RATES, 
                  DsLen = 1, RSNIe = IE_WPA, RSNIe2 = IE_WPA2;
#ifdef WSC_SUPPORT
    UCHAR		  Addr3[MAC_ADDR_LEN];
    PFRAME_802_11 pFrame = (PFRAME_802_11)Elem->Msg;
#endif // WSC_SUPPORT //

#ifdef WSC_SUPPORT
    COPY_MAC_ADDR(Addr3, pFrame->Hdr.Addr3);
#endif // WSC_SUPPORT //
    
    pAd->PortCfg.bGetAPConfig = FALSE;
    
    if (! PeerProbeReqSanity(pAd, Elem->Msg, Elem->MsgLen, Addr2, Ssid, &SsidLen))
        return;

    for (apidx = MAIN_MBSSID; apidx < pAd->PortCfg.BssidNum; apidx++)
    {
    	// if in bridge mode, no need to reply probe req.
	    if (pAd->WdsTab.Mode == WDS_BRIDGE_MODE)
	        break;

		if (pAd->PortCfg.MBSSID[apidx].MSSIDDev->flags & IFF_UP)

		if (((SsidLen == 0) && (pAd->PortCfg.MBSSID[apidx].bHideSsid == 0))
#ifdef WSC_SUPPORT
            || ((SsidLen == 32) && MAC_ADDR_EQUAL(Addr3, pAd->PortCfg.Bssid) && (pAd->PortCfg.MBSSID[apidx].bHideSsid == 0)) /* buffalo WPS testbed STA send ProbrRequest ssid length = 32 and ssid are not AP , but DA are AP. for WPS test send ProbeResponse */
#endif // WSC_SUPPORT //
            || ((SsidLen == pAd->PortCfg.MBSSID[apidx].SsidLen) && NdisEqualMemory(Ssid, pAd->PortCfg.MBSSID[apidx].Ssid, (ULONG)SsidLen)))
	    {
	        // allocate and send out ProbeRsp frame
	        pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
	        if(pOutBuffer == NULL)
		        break;

	        DBGPRINT(RT_DEBUG_INFO, "SYNC - IF(ra%d)Send PROBE_RSP to %02x:%02x:%02x:%02x:%02x:%02x...\n", apidx,
	            Addr2[0],Addr2[1],Addr2[2],Addr2[3],Addr2[4],Addr2[5] );

	        MgtMacHeaderInit(pAd, &ProbeRspHdr, SUBTYPE_PROBE_RSP, 0, Addr2, pAd->PortCfg.MBSSID[apidx].Bssid);

	        if ((pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPA) || (pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPAPSK))
		    	RSNIe = IE_WPA;
		    else if ((pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPA2) || (pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPA2PSK))
		    	RSNIe = IE_WPA2;

	        // Append RSN_IE when  WPA OR WPAPSK, 
	        if (((pAd->PortCfg.PhyMode == PHY_11BG_MIXED) || (pAd->PortCfg.PhyMode == PHY_11G)) 
	            && (pAd->PortCfg.MBSSID[apidx].AuthMode < Ndis802_11AuthModeWPA))
	        {
	            UCHAR SupportedRatesLen = 4, ErpLen = 1;
	            UCHAR ExtendedRatesIe = IE_EXT_SUPP_RATES, ErpIe = IE_ERP;
	            UCHAR ExtendedRatesLen = pAd->PortCfg.SupportedRatesLen - SupportedRatesLen;
	                
	            MakeOutgoingFrame(pOutBuffer,                       &FrameLen, 
	                          sizeof(HEADER_802_11),                &ProbeRspHdr, 
	                          TIMESTAMP_LEN,                        &FakeTimestamp,
	                          2,                                    &pAd->PortCfg.BeaconPeriod,
	                          2,                                    &pAd->PortCfg.MBSSID[apidx].CapabilityInfo,
	                          1,                                    &SsidIe, 
	                          1,                                    &pAd->PortCfg.MBSSID[apidx].SsidLen, 
	                          pAd->PortCfg.MBSSID[apidx].SsidLen,    pAd->PortCfg.MBSSID[apidx].Ssid,
	                          1,                                    &SuppIe, 
	                          1,                                    &SupportedRatesLen,
	                          SupportedRatesLen,                    pAd->PortCfg.SupportedRates, 
	                          1,                                    &DsIe, 
	                          1,                                    &DsLen, 
	                          1,                                    &pAd->PortCfg.Channel,
	                          1,                                    &ErpIe,
	                          1,                                    &ErpLen,
	                          1,                                    &pAd->Mlme.ErpIeContent,
	                          1,                                    &ExtendedRatesIe,
	                          1,                                    &ExtendedRatesLen,
	                          ExtendedRatesLen,                     &pAd->PortCfg.SupportedRates[SupportedRatesLen],
	                          END_OF_ARGS);
	        }
	        else if(  pAd->PortCfg.MBSSID[apidx].AuthMode < Ndis802_11AuthModeWPA)
	        {
	            MakeOutgoingFrame(pOutBuffer,                       &FrameLen, 
	                        sizeof(HEADER_802_11),                  &ProbeRspHdr, 
	                        TIMESTAMP_LEN,                          &FakeTimestamp,
	                        2,                                      &pAd->PortCfg.BeaconPeriod,
	                        2,                                      &pAd->PortCfg.MBSSID[apidx].CapabilityInfo,
	                        1,                                      &SsidIe, 
	                        1,                                      &pAd->PortCfg.MBSSID[apidx].SsidLen, 
	                        pAd->PortCfg.MBSSID[apidx].SsidLen,      pAd->PortCfg.MBSSID[apidx].Ssid,
	                        1,                                      &SuppIe, 
	                        1,                                      &pAd->PortCfg.SupportedRatesLen,
	                        pAd->PortCfg.SupportedRatesLen,         pAd->PortCfg.SupportedRates, 
	                        1,                                      &DsIe, 
	                        1,                                      &DsLen, 
	                        1,                                      &pAd->PortCfg.Channel,
	                        END_OF_ARGS);
	        }
	        else if (((pAd->PortCfg.PhyMode == PHY_11BG_MIXED) || (pAd->PortCfg.PhyMode == PHY_11G)) 
	            && ((pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPA1WPA2) ||(pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPAPSKWPA2PSK)))
	        {
	            UCHAR SupportedRatesLen = 4, ErpLen = 1;
	            UCHAR ExtendedRatesIe = IE_EXT_SUPP_RATES, ErpIe = IE_ERP;
	            UCHAR ExtendedRatesLen = pAd->PortCfg.SupportedRatesLen - SupportedRatesLen;
	                
	            MakeOutgoingFrame(pOutBuffer,                       &FrameLen, 
	                          sizeof(HEADER_802_11),                &ProbeRspHdr, 
	                          TIMESTAMP_LEN,                        &FakeTimestamp,
	                          2,                                    &pAd->PortCfg.BeaconPeriod,
	                          2,                                    &pAd->PortCfg.MBSSID[apidx].CapabilityInfo,
	                          1,                                    &SsidIe, 
	                          1,                                    &pAd->PortCfg.MBSSID[apidx].SsidLen, 
	                          pAd->PortCfg.MBSSID[apidx].SsidLen,    pAd->PortCfg.MBSSID[apidx].Ssid,
	                          1,                                    &SuppIe, 
	                          1,                                    &SupportedRatesLen,
	                          SupportedRatesLen,                    pAd->PortCfg.SupportedRates, 
	                          1,                                    &DsIe, 
	                          1,                                    &DsLen, 
	                          1,                                    &pAd->PortCfg.Channel,
	                          1,                                    &ErpIe,
	                          1,                                    &ErpLen,
	                          1,                                    &pAd->Mlme.ErpIeContent,
	                          1,                                    &ExtendedRatesIe,
	                          1,                                    &ExtendedRatesLen,
	                          ExtendedRatesLen,                     &pAd->PortCfg.SupportedRates[SupportedRatesLen],
	                          1,                                      &RSNIe,
							  1,                                      &pAd->PortCfg.MBSSID[apidx].RSNIE_Len[0],
	                          pAd->PortCfg.MBSSID[apidx].RSNIE_Len[0], pAd->PortCfg.MBSSID[apidx].RSN_IE[0],
	                          1,                                      &RSNIe2,
	                          1,                                      &pAd->PortCfg.MBSSID[apidx].RSNIE_Len[1],
	                          pAd->PortCfg.MBSSID[apidx].RSNIE_Len[1], pAd->PortCfg.MBSSID[apidx].RSN_IE[1],
	                          END_OF_ARGS);
	        }
	        else if ((pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPA1WPA2) ||(pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPAPSKWPA2PSK))
	        {
	            MakeOutgoingFrame(pOutBuffer,                       &FrameLen, 
	                        sizeof(HEADER_802_11),                  &ProbeRspHdr, 
	                        TIMESTAMP_LEN,                          &FakeTimestamp,
	                        2,                                      &pAd->PortCfg.BeaconPeriod,
	                        2,                                      &pAd->PortCfg.MBSSID[apidx].CapabilityInfo,
	                        1,                                      &SsidIe, 
	                        1,                                      &pAd->PortCfg.MBSSID[apidx].SsidLen, 
	                        pAd->PortCfg.MBSSID[apidx].SsidLen,      pAd->PortCfg.MBSSID[apidx].Ssid,
	                        1,                                      &SuppIe, 
	                        1,                                      &pAd->PortCfg.SupportedRatesLen,
	                        pAd->PortCfg.SupportedRatesLen,         pAd->PortCfg.SupportedRates, 
	                        1,                                      &DsIe, 
	                        1,                                      &DsLen, 
	                        1,                                      &pAd->PortCfg.Channel,
	                        1,                                      &RSNIe,
	                        1,                                      &pAd->PortCfg.MBSSID[apidx].RSNIE_Len[0],
	                        pAd->PortCfg.MBSSID[apidx].RSNIE_Len[0], pAd->PortCfg.MBSSID[apidx].RSN_IE[0],
	                        1,                                      &RSNIe2,
	                        1,                                      &pAd->PortCfg.MBSSID[apidx].RSNIE_Len[1],
	                        pAd->PortCfg.MBSSID[apidx].RSNIE_Len[1], pAd->PortCfg.MBSSID[apidx].RSN_IE[1],
	                        END_OF_ARGS);
	        }
	        else if ((pAd->PortCfg.PhyMode == PHY_11BG_MIXED) || (pAd->PortCfg.PhyMode == PHY_11G))
	        {
	            UCHAR SupportedRatesLen = 4, ErpLen = 1;
	            UCHAR ExtendedRatesIe = IE_EXT_SUPP_RATES, ErpIe = IE_ERP;
	            UCHAR ExtendedRatesLen = pAd->PortCfg.SupportedRatesLen - SupportedRatesLen;
	             
	            MakeOutgoingFrame(pOutBuffer,                       &FrameLen, 
	                        sizeof(HEADER_802_11),                  &ProbeRspHdr, 
	                        TIMESTAMP_LEN,                          &FakeTimestamp,
	                        2,                                      &pAd->PortCfg.BeaconPeriod,
	                        2,                                      &pAd->PortCfg.MBSSID[apidx].CapabilityInfo,
	                        1,                                      &SsidIe, 
	                        1,                                      &pAd->PortCfg.MBSSID[apidx].SsidLen, 
	                        pAd->PortCfg.MBSSID[apidx].SsidLen,      pAd->PortCfg.MBSSID[apidx].Ssid,
	                        1,                                      &SuppIe, 
	                        1,                                      &SupportedRatesLen,
	                        SupportedRatesLen,                      pAd->PortCfg.SupportedRates, 
	                        1,                                      &DsIe, 
	                        1,                                      &DsLen, 
	                        1,                                      &pAd->PortCfg.Channel,
	                        1,                                      &ErpIe,
	                        1,                                      &ErpLen,
	                        1,                                      &pAd->Mlme.ErpIeContent,
	                        1,                                      &ExtendedRatesIe,
	                        1,                                      &ExtendedRatesLen,
	                        ExtendedRatesLen,                       &pAd->PortCfg.SupportedRates[SupportedRatesLen],
	                        1,                                      &RSNIe,
	                        1,                                      &pAd->PortCfg.MBSSID[apidx].RSNIE_Len[0],
	                        pAd->PortCfg.MBSSID[apidx].RSNIE_Len[0],  pAd->PortCfg.MBSSID[apidx].RSN_IE[0],
	                        END_OF_ARGS);
	        }
	        else
	        {
	            MakeOutgoingFrame(pOutBuffer,                       &FrameLen, 
	                        sizeof(HEADER_802_11),                  &ProbeRspHdr, 
	                        TIMESTAMP_LEN,                          &FakeTimestamp,
	                        2,                                      &pAd->PortCfg.BeaconPeriod,
	                        2,                                      &pAd->PortCfg.MBSSID[apidx].CapabilityInfo,
	                        1,                                      &SsidIe, 
	                        1,                                      &pAd->PortCfg.MBSSID[apidx].SsidLen, 
	                        pAd->PortCfg.MBSSID[apidx].SsidLen,      pAd->PortCfg.MBSSID[apidx].Ssid,
	                        1,                                      &SuppIe, 
	                        1,                                      &pAd->PortCfg.SupportedRatesLen,
	                        pAd->PortCfg.SupportedRatesLen,         pAd->PortCfg.SupportedRates, 
	                        1,                                      &DsIe, 
	                        1,                                      &DsLen, 
	                        1,                                      &pAd->PortCfg.Channel,
	                        1,                                      &RSNIe,
	                        1,                                      &pAd->PortCfg.MBSSID[apidx].RSNIE_Len[0],
	                        pAd->PortCfg.MBSSID[apidx].RSNIE_Len[0], pAd->PortCfg.MBSSID[apidx].RSN_IE[0],
	                        END_OF_ARGS);
	        }

#ifdef WMM_SUPPORT
	        // add WMM IE here
	        if (pAd->PortCfg.MBSSID[apidx].bWmmCapable)
	        {
	            UCHAR i;
	            UCHAR WmeParmIe[26] = {IE_VENDOR_SPECIFIC, 24, 0x00, 0x50, 0xf2, 0x02, 0x01, 0x01, 0, 0}; 
	            WmeParmIe[8] = pAd->PortCfg.BssEdcaParm.EdcaUpdateCount & 0x0f;
	            WmeParmIe[8] |= (pAd->PortCfg.bAPSDCapable) ? 0x80 : 0x00;
	            for (i=QID_AC_BE; i<=QID_AC_VO; i++)
	            {
	                WmeParmIe[10+ (i*4)] = (i << 5)                                         +     // b5-6 is ACI
	                                       ((UCHAR)pAd->PortCfg.BssEdcaParm.bACM[i] << 4)     +     // b4 is ACM
	                                       (pAd->PortCfg.BssEdcaParm.Aifsn[i] & 0x0f);              // b0-3 is AIFSN
	                WmeParmIe[11+ (i*4)] = (pAd->PortCfg.BssEdcaParm.Cwmax[i] << 4)           +     // b5-8 is CWMAX
	                                       (pAd->PortCfg.BssEdcaParm.Cwmin[i] & 0x0f);              // b0-3 is CWMIN
	                WmeParmIe[12+ (i*4)] = (UCHAR)(pAd->PortCfg.BssEdcaParm.Txop[i] & 0xff);        // low byte of TXOP
	                WmeParmIe[13+ (i*4)] = (UCHAR)(pAd->PortCfg.BssEdcaParm.Txop[i] >> 8);          // high byte of TXOP
	            }

	            MakeOutgoingFrame(pOutBuffer+FrameLen,      &TmpLen,
	                              26,                       WmeParmIe,
	                              END_OF_ARGS);
	            FrameLen += TmpLen;
	        }
#endif /* WMM_SUPPORT */

#ifdef AGGREGATION_SUPPORT
	        // add Ralink-specific IE here - Byte0.b0=1 for aggregation, Byte0.b1=1 for piggy-back
		    if (pAd->PortCfg.bAggregationCapable)
		    {
		    	{
			        ULONG TmpLen;
			        UCHAR RalinkSpecificIe[9] = {IE_VENDOR_SPECIFIC, 7, 0x00, 0x0c, 0x43, 0x01, 0x00, 0x00, 0x00}; 
			        MakeOutgoingFrame(pOutBuffer+FrameLen,       &TmpLen,
			                          9,                         RalinkSpecificIe,
			                          END_OF_ARGS);
			        FrameLen += TmpLen;
		    	}
		    }
#endif /* AGGREGATION_SUPPORT */
	        
            // add Channel switch announcement IE
	        if ((pAd->PortCfg.PhyMode == PHY_11A) && (pAd->PortCfg.RadarDetect.IEEE80211H == TRUE) && (pAd->PortCfg.RadarDetect.RDMode == RD_SWITCHING_MODE))
	        {
	            UCHAR CSAIe=IE_CHANNEL_SWITCH_ANNOUNCEMENT;
	            UCHAR CSALen=3;
	            UCHAR CSAMode=1;
	            
	            MakeOutgoingFrame(pOutBuffer+FrameLen,      &TmpLen,
	                              1,                        &CSAIe,
	                              1,                        &CSALen,
	                              1,                        &CSAMode,
	                              1,                        &pAd->PortCfg.Channel,
	                              1,                        &pAd->PortCfg.RadarDetect.CSCount,
	                              END_OF_ARGS);
	            FrameLen += TmpLen;
	        }

	        // add country IE, power constraint IE
		    if (pAd->PortCfg.bCountryFlag)
		    {
		        ULONG TmpLen2=0;
		        UCHAR TmpFrame[256];
		        UCHAR CountryIe = IE_COUNTRY;
		        UCHAR MaxTxPower = pAd->PortCfg.MaxTxPowerLevel;

				// Only 802.11a APs that comply with 802.11h are required to include a Power Constrint Element(IE=32) 
				// in beacons and probe response frames
				if (pAd->PortCfg.PhyMode == PHY_11A && pAd->PortCfg.RadarDetect.IEEE80211H == TRUE)
				{
		        // prepare power constraint IE
		        MakeOutgoingFrame(pOutBuffer+FrameLen,    &TmpLen,
		                          3,                 	PowerConstraintIE,
		                          END_OF_ARGS);
		        FrameLen += TmpLen;
				}

		        NdisZeroMemory(TmpFrame, sizeof(TmpFrame));

				// prepare channel information
		        MakeOutgoingFrame(TmpFrame+TmpLen2,     &TmpLen,
		                          1,                 	&pAd->ChannelList[0].Channel,
		                          1,                 	&pAd->ChannelListNum,
		                          1,                 	&MaxTxPower,
		                          END_OF_ARGS);
		        TmpLen2 += TmpLen;

		        // need to do the padding bit check, and concatenate it
		        if ((TmpLen2%2) == 0)
		        {
		        	UCHAR	TmpLen3 = TmpLen2+4;
			        MakeOutgoingFrame(pOutBuffer+FrameLen,  &TmpLen,
			                          1,                 	&CountryIe,
			                          1,                 	&TmpLen3,
			                          3,                 	pAd->PortCfg.CountryCode,
			                          TmpLen2+1,				TmpFrame,
			                          END_OF_ARGS);
		        }
		        else
		        {
		        	UCHAR	TmpLen3 = TmpLen2+3;
			        MakeOutgoingFrame(pOutBuffer+FrameLen,  &TmpLen,
			                          1,                 	&CountryIe,
			                          1,                 	&TmpLen3,
			                          3,                 	pAd->PortCfg.CountryCode,
			                          TmpLen2,				TmpFrame,
			                          END_OF_ARGS);
		        }
		        FrameLen += TmpLen;
		    }

#ifdef WSC_SUPPORT
            // add Simple Config Information Element
            if ((pAd->PortCfg.MBSSID[apidx].WscControl.WscConfMode > WSC_DISABLE) && (pAd->PortCfg.MBSSID[apidx].WscIEProbeResp.ValueLen))
            {
        		ULONG WscTmpLen = 0;
        		MakeOutgoingFrame(pOutBuffer+FrameLen,                                  &WscTmpLen,
        						  pAd->PortCfg.MBSSID[apidx].WscIEProbeResp.ValueLen,   pAd->PortCfg.MBSSID[apidx].WscIEProbeResp.Value,
                                  END_OF_ARGS);
        		FrameLen += WscTmpLen;
            }
#endif // WSC_SUPPORT //

            if (pAd->PortCfg.bGetAPConfig &&
                ((SsidLen == pAd->PortCfg.MBSSID[apidx].SsidLen) && NdisEqualMemory(Ssid, pAd->PortCfg.MBSSID[apidx].Ssid, (ULONG)SsidLen)))
            {
                CHAR    cfgBuf[MAX_CFG_BUFFER_LEN+1] = {0};
                if ( getCfgDataForSTA(pAd, cfgBuf) ) 
                {
                    UCHAR RalinkSpecificOUIForGetCfg[4] = {0x00, 0x0c, 0x43, 0x80};
                    SHORT dataLen = 0;
                    UCHAR Vendor_IE = IE_VENDOR_SPECIFIC, pktLen = 0, count = 0;
            
                    dataLen = strlen(cfgBuf);
                    for ( ; ; ) 
                    {
                        UCHAR tempBuf[251] = {0}, tempBufLen = 251; 

                        if (dataLen <= 251) 
                        {
                            pktLen = (UCHAR)dataLen + 4;
                            RalinkSpecificOUIForGetCfg[3] = 0x80;
                            NdisMoveMemory(tempBuf, cfgBuf+(count*tempBufLen), dataLen);
                            MakeOutgoingFrame(pOutBuffer+FrameLen,      &TmpLen,
                                              1,                        &Vendor_IE,
                                              1,                        &pktLen,
            	                              4,                        RalinkSpecificOUIForGetCfg,
            	                              dataLen,                  tempBuf,
            	                              END_OF_ARGS);
            	            FrameLen += TmpLen;
                            break;
                        }
                        else 
                        {
                            pktLen = 4 + tempBufLen;
                            RalinkSpecificOUIForGetCfg[3] = 0x40;
                            NdisMoveMemory(tempBuf, cfgBuf+(count*tempBufLen), tempBufLen);
                            MakeOutgoingFrame(pOutBuffer+FrameLen,      &TmpLen,
                                              1,                        &Vendor_IE,
                                              1,                        &pktLen,
            	                              4,                        RalinkSpecificOUIForGetCfg,
            	                              tempBufLen,               tempBuf,
            	                              END_OF_ARGS);
            	            FrameLen += TmpLen;
                            dataLen -= tempBufLen;
                            count++;
                        }
                    }
                }
            }

	        MiniportMMRequest(pAd, pOutBuffer, FrameLen);
	        kfree(pOutBuffer);
	    }
   	}
}

/* 
    ==========================================================================
    Description:
        parse the received BEACON

    NOTE:
        The only thing AP cares about received BEACON frames is to decide 
        if there's any overlapped legacy BSS condition (OLBC).
        If OLBC happened, this AP should set the ERP->Use_Protection bit in its 
        outgoing BEACON. The result is to tell all its clients to use RTC/CTS
        or CTS-to-self protection to protect B/G mixed traffic
    ==========================================================================
 */
VOID PeerBeaconAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem) 
{
    UCHAR       Bssid[MAC_ADDR_LEN], Addr2[MAC_ADDR_LEN];
    CHAR          Ssid[MAX_LEN_OF_SSID];
    CF_PARM       CfParm;
    UCHAR         SsidLen, MessageToMe=0, BssType, Channel;
    UCHAR         DtimCount=0, DtimPeriod=0, BcastFlag=0;
    USHORT        CapabilityInfo, AtimWin, BeaconPeriod;
    LARGE_INTEGER TimeStamp;
    UCHAR         Erp;
	UCHAR         SupRate[MAX_LEN_OF_SUPPORTED_RATES], ExtRate[MAX_LEN_OF_SUPPORTED_RATES];
	UCHAR		  SupRateLen, ExtRateLen;
	UCHAR		  CkipFlag;
	UCHAR		  AironetCellPowerLimit;
	EDCA_PARM					EdcaParm;
	QBSS_LOAD_PARM				QbssLoad;
	QOS_CAPABILITY_PARM			QosCapability;
    ULONG						RalinkIe;
	// New for WPA security suites
	UCHAR						VarIE[MAX_VIE_LEN];		// Total VIE length = MAX_VIE_LEN - -5
	NDIS_802_11_VARIABLE_IEs	*pVIE = NULL;
    UCHAR         LenVIE;
    BOOLEAN       LegacyBssExist;

	// Init Variable IE structure
	pVIE = (PNDIS_802_11_VARIABLE_IEs) VarIE;
	pVIE->Length = 0;
    if (PeerBeaconAndProbeRspSanity(pAd, 
                                Elem->Msg, 
                                Elem->MsgLen, 
                                Addr2, 
                                Bssid, 
                                Ssid, 
                                &SsidLen, 
                                &BssType, 
                                &BeaconPeriod, 
                                &Channel, 
                                &TimeStamp, 
                                &CfParm, 
                                &AtimWin, 
                                &CapabilityInfo, 
                                &Erp,
                                &DtimCount, 
                                &DtimPeriod, 
                                &BcastFlag, 
                                &MessageToMe, 
								SupRate,
								&SupRateLen,
								ExtRate,
								&ExtRateLen,
								&CkipFlag,
								&AironetCellPowerLimit,
								&EdcaParm,
								&QbssLoad,
								&QosCapability,
								&RalinkIe,
				                &LenVIE,
                                pVIE)) 
    {
        if (Channel == pAd->PortCfg.AutoChannel_Channel)
        {
            CHAR RealRssi = ConvertToRssi(pAd, Elem->Rssi, RSSI_NO_1);
            // record the max RSSI of any received BEACON frames. APStartUp phase will
            // use this information to help select a less interference channel
            // TODO: 2005-03-04 this is a dirty patch. we should change all RSSI related variables to UNSIGNED SHORT for easy reading and calculation
            if ((RealRssi + pAd->BbpRssiToDbmDelta) > pAd->PortCfg.AutoChannel_MaxRssi)
                pAd->PortCfg.AutoChannel_MaxRssi = RealRssi + pAd->BbpRssiToDbmDelta;
        }

        // ignore BEACON not in this channel
        if (Channel != pAd->PortCfg.Channel)
            return;

        if ((Erp & 0x01) || ((SupRateLen + ExtRateLen) <= 4))
            LegacyBssExist = TRUE;
        else
            LegacyBssExist = FALSE;

        if (LegacyBssExist && pAd->PortCfg.DisableOLBCDetect==0)
        {
            pAd->Mlme.LastOLBCDetectTime = pAd->Mlme.Now32;
            DBGPRINT(RT_DEBUG_INFO, "%02x:%02x:%02x:%02x:%02x:%02x is a legacy BSS (rate# =%d, ERP=%d), set Use_Protection bit\n", 
                Bssid[0], Bssid[1], Bssid[2], Bssid[3], Bssid[4], Bssid[5], (SupRateLen + ExtRateLen), Erp);
        }

#ifdef APCLI_SUPPORT
		do
		{
			INT i;

			// check BEACON does in ApCli TABLE.
			for (i = 0; i < MAX_APCLI_ENTRY; i++)
			{
				//BSSID match the ApCliBssid ?(from a valid AP)
				if ((pAd->ApCliTab.ApCliEntry[i].Valid== TRUE)
					&& (MAC_ADDR_EQUAL(&pAd->ApCliTab.ApCliEntry[i].ApCliBssid, Addr2)))
				{
					pAd->ApCliTab.ApCliEntry[i].NumOfAvgRssiSample ++;
					break;
				}
			}	

			if (i < MAX_APCLI_ENTRY)
			{
				pAd->ApCliTab.ApCliEntry[i].ApCliRcvBeaconTime = jiffies;
				ChannelSwitchAnnouncementProc(pAd, pVIE, LenVIE);
			}
		} while(FALSE);
#endif // APCLI_SUPPORT //

    }
    // sanity check fail, ignore this frame
}

/* 
    ==========================================================================
    Description:
        Always call this function to get real rssi

    NOTE:
    ==========================================================================
 */
CHAR	ConvertToRssi(
    IN PRTMP_ADAPTER pAd,
	IN	UCHAR	Rssi,
	IN  UCHAR   RssiNumber)
{
	typedef	union	_LNA_AGC
	{
		struct
		{
#ifdef BIG_ENDIAN
			UCHAR	Rsvd:1;
			UCHAR	Lna:2;
			UCHAR	Agc:5;
#else			
			UCHAR	Agc:5;
			UCHAR	Lna:2;
			UCHAR	Rsvd:1;
#endif				
		}	field;
		UCHAR		Byte;
	}	LNA_AGC;

	LNA_AGC	LnaAgc;
	CHAR	RssiOffset = 0;

	//
	// Get RSSI Offset.
	//
	if (pAd->PortCfg.Channel <= 14)
	{	
		if (RssiNumber == RSSI_NO_1)
			RssiOffset = pAd->BGRssiOffset1;
		else if (RssiNumber == RSSI_NO_2)
			RssiOffset = pAd->BGRssiOffset2;
	}
	else
	{
		if (RssiNumber == RSSI_NO_1)
			RssiOffset = pAd->ARssiOffset1;
		else if (RssiNumber == RSSI_NO_2)
			RssiOffset = pAd->ARssiOffset2;
	}

	LnaAgc.Byte = Rssi;

	if (pAd->PortCfg.Channel <= 14)
	{
		//
		// B/G mode
		//
		if (pAd->NicConfig2.field.ExternalLNAForG == 0)
		{
			if (LnaAgc.field.Lna == 0x03)
				return (LnaAgc.field.Agc * 2 - 90 + RssiOffset); //for B/G mode
			else if (LnaAgc.field.Lna == 0x02)
				return (LnaAgc.field.Agc * 2 - 74 + RssiOffset); //for B/G mode
			else if (LnaAgc.field.Lna == 0x01)
				return (LnaAgc.field.Agc * 2 - 64 + RssiOffset);
			else 
				return -1;
		}
		else
		{
			// RSSI needs to be offset when external LNA enable
			if (LnaAgc.field.Lna == 0x03)
				return (LnaAgc.field.Agc * 2 - (90 + 14) + RssiOffset); //for B/G mode
			else if (LnaAgc.field.Lna == 0x02)
				return (LnaAgc.field.Agc * 2 - (74 + 14) + RssiOffset); //for B/G mode
			else if (LnaAgc.field.Lna == 0x01)
				return (LnaAgc.field.Agc * 2 - (64 + 14) + RssiOffset);
			else
				return -1;
		}
	}
	else
	{
		//
		// A mode
		//
		if (pAd->NicConfig2.field.ExternalLNAForA == 0)
		{
			if (LnaAgc.field.Lna == 0x03)
				return (LnaAgc.field.Agc * 2 - 100 + RssiOffset); //for A mode
			else if (LnaAgc.field.Lna == 0x02)
				return (LnaAgc.field.Agc * 2 - 86 + RssiOffset); //for A mode
			else if (LnaAgc.field.Lna == 0x01)
				return (LnaAgc.field.Agc * 2 - 64 + RssiOffset);
			else
				return -1;
		}
		else
		{
			// RSSI needs to be offset when external LNA enable
			if (LnaAgc.field.Lna == 0x03)
				return (LnaAgc.field.Agc * 2 - (100 + pAd->AModeGainValue) + RssiOffset); //for A mode
			else if (LnaAgc.field.Lna == 0x02)
				return (LnaAgc.field.Agc * 2 - (86 + pAd->AModeGainValue) + RssiOffset); //for A mode
			else if (LnaAgc.field.Lna == 0x01)
				return (LnaAgc.field.Agc * 2 - (64 + pAd->AModeGainValue) + RssiOffset);
			else
				return -1;
		}
	}
}

/* 
    ==========================================================================
    Description:
        Site survey entry point

    NOTE:
    ==========================================================================
*/
VOID ApSiteSurvey(
	IN   PRTMP_ADAPTER   pAd)
{
    MLME_SCAN_REQ_STRUCT    ScanReq;
    CHAR                    BroadSsid[MAX_LEN_OF_SSID];

    AsicDisableSync(pAd);

    BssTableInit(&pAd->ScanTab);
    BroadSsid[0] = '\0';
    pAd->Mlme.SyncMachine.CurrState = SYNC_IDLE;

    ScanReq.SsidLen = 0;
    NdisMoveMemory(ScanReq.Ssid, BroadSsid, ScanReq.SsidLen);
    ScanReq.BssType = BSS_ANY;
    ScanReq.ScanType = SCAN_PASSIVE;
    
    MlmeEnqueue(pAd, SYNC_STATE_MACHINE, MT2_MLME_SCAN_REQ, sizeof(MLME_SCAN_REQ_STRUCT), &ScanReq);
    MlmeHandler(pAd);
}

#define fn(x)    ((VOID(*)(PRTMP_ADAPTER pAd, PCHAR pOutput))(x))

static struct AP_CFG_ITEM_INFO {
	PCHAR   name;
	VOID    (*GetCfgData)(PRTMP_ADAPTER pAd, PCHAR pOutput); // functio pointer to get config data
} AP_CONFIG_ITEM_TABLE[] = {
    {"CountryRegion",       fn(getCfgCountryRegion)},
    {"CountryRegionABand",  fn(getCfgCountryRegionABand)},
    {"CountryCode",         fn(getCfgCountryCode)},
    {"SSID",                fn(getCfgSSID)},
    {"AP1WirelessMode",     fn(getCfgAP1WirelessMode)},
    {"TxRate",              fn(getCfgTxRate)},
    {"Channel",             fn(getCfgChannel)},
    {"BasicRate",           fn(getCfgBasicRate)},
    {"BeaconPeriod",        fn(getCfgBeaconPeriod)},
    {"DtimPeriod",          fn(getCfgDtimPeriod)},
    {"TxPower",             fn(getCfgTxPower)},
    {"DisableOLBC",         fn(getCfgDisableOLBC)},
    {"BGProtection",        fn(getCfgBGProtection)},
    {"TxAntenna",           fn(getCfgTxAntenna)},
    {"TxPreamble",          fn(getCfgTxPreamble)},
    {"RTSThreshold",        fn(getCfgRTSThreshold)},
    {"FragThreshold",       fn(getCfgFragThreshold)},
	{"TxBurst",             fn(getCfgTxBurst)},
	{"PktAggregate",        fn(getCfgPktAggregate)},
	{"TurboRate",           fn(getCfgTurboRate)},
	{"WmmCapable",          fn(getCfgWmmCapable)},
	{"APAifsn",             fn(getCfgAPAifsn)},
	{"APCwmin",             fn(getCfgAPCwmin)},
	{"APCwmax",             fn(getCfgAPCwmax)},
	{"APTxop",              fn(getCfgAPTxop)},
	{"APACM",               fn(getCfgAPACM)},
	{"BSSAifsn",            fn(getCfgBSSAifsn)},
	{"BSSCwmin",            fn(getCfgBSSCwmin)},
	{"BSSCwmax",            fn(getCfgBSSCwmax)},
	{"BSSTxop",             fn(getCfgBSSTxop)},
	{"BSSACM",              fn(getCfgBSSACM)},
	{"AckPolicy",           fn(getCfgAckPolicy)},
	{"NoForwarding",        fn(getCfgNoForwarding)},
	{"NoForwardingBTNBSSID",fn(getCfgNoForwardingBTNBSSID)},
	{"HideSSID",            fn(getCfgHideSSID)},
	{"ShortSlot",           fn(getCfgShortSlot)},
	{"AutoChannelSelect",   fn(getCfgAutoChannelSelect)},
	{"IEEE8021X",           fn(getCfgIEEE8021X)},
	{"IEEE80211H",          fn(getCfgIEEE80211H)},
	{"CSPeriod",            fn(getCfgCSPeriod)},
	{"PreAuth",             fn(getCfgPreAuth)},
	{"AuthMode",            fn(getCfgAuthMode)},
	{"EncrypType",          fn(getCfgEncrypType)},
	{"RekeyInterval",       fn(getCfgRekeyInterval)},
	{"RekeyMethod",         fn(getCfgRekeyMethod)},
	{"PMKCachePeriod",      fn(getCfgPMKCachePeriod)},
	{"WPAPSK",              fn(getCfgWPAPSK)},
	{"DefaultKeyID",        fn(getCfgDefaultKeyID)},
	{"Key1Type",            fn(getCfgKey1Type)},
	{"Key1Str",             fn(getCfgKey1Str)},
	{"Key2Type",            fn(getCfgKey2Type)},
	{"Key2Str",             fn(getCfgKey2Str)},
	{"Key3Type",            fn(getCfgKey3Type)},
	{"Key3Str",             fn(getCfgKey3Str)},
	{"Key4Type",            fn(getCfgKey4Type)},
	{"Key4Str",             fn(getCfgKey4Str)},
	{"HSCounter",           fn(getCfgHSCounter)},
    {NULL, NULL}
};

VOID getCfgCountryRegion(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.CountryRegion);
}

VOID getCfgCountryRegionABand(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.CountryRegionForABand);
}

VOID getCfgCountryCode(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        NdisMoveMemory(pOutput, pAd->PortCfg.CountryCode, 3);
}

VOID getCfgSSID(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
    {
        int i;
        for (i = 0; i < pAd->PortCfg.BssidNum; i++)
        {
            if (i==0)
                sprintf(pOutput, "%s", pAd->PortCfg.MBSSID[i].Ssid);
            else
                sprintf(pOutput, "%s;%s", pOutput, pAd->PortCfg.MBSSID[i].Ssid);
        }
    }
}

VOID getCfgAP1WirelessMode(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.PhyMode);
}

VOID getCfgTxRate(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.MBSSID[pAd->IoctlIF].DesiredRatesIndex);
}

VOID getCfgChannel(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.Channel);
}

VOID getCfgBasicRate(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.BasicRateBitMap);
}

VOID getCfgBeaconPeriod(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.BeaconPeriod);
}

VOID getCfgDtimPeriod(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->Mlme.DtimPeriod);
}

VOID getCfgTxPower(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.TxPowerPercentage);
}

VOID getCfgDisableOLBC(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.DisableOLBCDetect);
}

VOID getCfgBGProtection(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.UseBGProtection);
}

VOID getCfgTxAntenna(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->DefaultTxAntenna);
}

VOID getCfgTxPreamble(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.TxPreamble);
}

VOID getCfgRTSThreshold(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.RtsThreshold);
}

VOID getCfgFragThreshold(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.FragmentThreshold);
}

VOID getCfgTxBurst(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.bEnableTxBurst);
}

VOID getCfgPktAggregate(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.bAggregationCapable);
}

VOID getCfgTurboRate(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.EnableTurboRate);
}

VOID getCfgWmmCapable(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.MBSSID[MAIN_MBSSID].bWmmCapable);
}

VOID getCfgAPAifsn(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d;%d;%d;%d", pAd->PortCfg.APEdcaParm.Aifsn[0],
                                        pAd->PortCfg.APEdcaParm.Aifsn[1],
                                        pAd->PortCfg.APEdcaParm.Aifsn[2],
                                        pAd->PortCfg.APEdcaParm.Aifsn[3]);
}

VOID getCfgAPCwmin(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d;%d;%d;%d", pAd->PortCfg.APEdcaParm.Cwmin[0],
                                        pAd->PortCfg.APEdcaParm.Cwmin[1],
                                        pAd->PortCfg.APEdcaParm.Cwmin[2],
                                        pAd->PortCfg.APEdcaParm.Cwmin[3]);
}

VOID getCfgAPCwmax(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d;%d;%d;%d", pAd->PortCfg.APEdcaParm.Cwmax[0],
                                        pAd->PortCfg.APEdcaParm.Cwmax[1],
                                        pAd->PortCfg.APEdcaParm.Cwmax[2],
                                        pAd->PortCfg.APEdcaParm.Cwmax[3]);
}

VOID getCfgAPTxop(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d;%d;%d;%d", pAd->PortCfg.APEdcaParm.Txop[0],
                                        pAd->PortCfg.APEdcaParm.Txop[1],
                                        pAd->PortCfg.APEdcaParm.Txop[2],
                                        pAd->PortCfg.APEdcaParm.Txop[3]);
}

VOID getCfgAPACM(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d;%d;%d;%d", pAd->PortCfg.APEdcaParm.bACM[0],
                                        pAd->PortCfg.APEdcaParm.bACM[1],
                                        pAd->PortCfg.APEdcaParm.bACM[2],
                                        pAd->PortCfg.APEdcaParm.bACM[3]);
}

VOID getCfgBSSAifsn(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d;%d;%d;%d", pAd->PortCfg.BssEdcaParm.Aifsn[0],
                                        pAd->PortCfg.BssEdcaParm.Aifsn[1],
                                        pAd->PortCfg.BssEdcaParm.Aifsn[2],
                                        pAd->PortCfg.BssEdcaParm.Aifsn[3]);
}

VOID getCfgBSSCwmin(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d;%d;%d;%d", pAd->PortCfg.BssEdcaParm.Cwmin[0],
                                        pAd->PortCfg.BssEdcaParm.Cwmin[1],
                                        pAd->PortCfg.BssEdcaParm.Cwmin[2],
                                        pAd->PortCfg.BssEdcaParm.Cwmin[3]);
}

VOID getCfgBSSCwmax(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d;%d;%d;%d", pAd->PortCfg.BssEdcaParm.Cwmax[0],
                                        pAd->PortCfg.BssEdcaParm.Cwmax[1],
                                        pAd->PortCfg.BssEdcaParm.Cwmax[2],
                                        pAd->PortCfg.BssEdcaParm.Cwmax[3]);
}

VOID getCfgBSSTxop(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d;%d;%d;%d", pAd->PortCfg.BssEdcaParm.Txop[0],
                                        pAd->PortCfg.BssEdcaParm.Txop[1],
                                        pAd->PortCfg.BssEdcaParm.Txop[2],
                                        pAd->PortCfg.BssEdcaParm.Txop[3]);
}

VOID getCfgBSSACM(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d;%d;%d;%d", pAd->PortCfg.BssEdcaParm.bACM[0],
                                        pAd->PortCfg.BssEdcaParm.bACM[1],
                                        pAd->PortCfg.BssEdcaParm.bACM[2],
                                        pAd->PortCfg.BssEdcaParm.bACM[3]);
}

VOID getCfgAckPolicy(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d;%d;%d;%d", pAd->PortCfg.AckPolicy[0],
                                        pAd->PortCfg.AckPolicy[1],
                                        pAd->PortCfg.AckPolicy[2],
                                        pAd->PortCfg.AckPolicy[3]);
}

VOID getCfgNoForwarding(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.MBSSID[MAIN_MBSSID].IsolateInterStaTraffic);
}

VOID getCfgNoForwardingBTNBSSID(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.IsolateInterStaTrafficBTNBSSID);
}

VOID getCfgHideSSID(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.MBSSID[MAIN_MBSSID].bHideSsid);
}

VOID getCfgShortSlot(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.UseShortSlotTime);
}

VOID getCfgAutoChannelSelect(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.bAutoChannelAtBootup);
}

VOID getCfgIEEE8021X(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.MBSSID[MAIN_MBSSID].IEEE8021X);
}

VOID getCfgIEEE80211H(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.RadarDetect.IEEE80211H);
}

VOID getCfgCSPeriod(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.RadarDetect.CSPeriod);
}

VOID getCfgPreAuth(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.MBSSID[MAIN_MBSSID].PreAuth);
}

VOID getCfgAuthMode(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.MBSSID[MAIN_MBSSID].AuthMode);
}

VOID getCfgEncrypType(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.MBSSID[MAIN_MBSSID].WepStatus);
}

VOID getCfgRekeyInterval(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.WPAREKEY.ReKeyInterval);
}

VOID getCfgRekeyMethod(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.WPAREKEY.ReKeyMethod);
}

VOID getCfgPMKCachePeriod(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.PMKCachePeriod);
}

VOID getCfgWPAPSK(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput) {
        SHORT j;
        for (j = 0; j < 32; j++)
        {
            sprintf(pOutput, "%s%02x", pOutput, pAd->PortCfg.MBSSID[MAIN_MBSSID].PMK[j]);
        }
    }
}

VOID getCfgDefaultKeyID(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.MBSSID[MAIN_MBSSID].DefaultKeyId);
}

VOID getCfgKeyType(
    IN  PRTMP_ADAPTER   pAd,
    IN  SHORT           index,
    OUT PCHAR           pOutput)
{
    if (pOutput) {
        BOOLEAN HexKey = FALSE;
        switch(pAd->PortCfg.MBSSID[MAIN_MBSSID].SharedKey[index].KeyLen) {
            case 5:
            case 13:
            case 16:
                HexKey = FALSE;
                break;
            case 10:
            case 26:
            case 32:
                HexKey = TRUE;
                break;
        }
        sprintf(pOutput, "%s", HexKey ? "Hex" : "Ascii");
    }
}

VOID getCfgKey1Type(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    getCfgKeyType(pAd, 0, pOutput);
}

VOID getCfgKey2Type(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    getCfgKeyType(pAd, 1, pOutput);
}

VOID getCfgKey3Type(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    getCfgKeyType(pAd, 2, pOutput);
}

VOID getCfgKey4Type(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    getCfgKeyType(pAd, 3, pOutput);
}

VOID getCfgKeyStr(
    IN  PRTMP_ADAPTER   pAd,
    IN  SHORT           index,
    OUT PCHAR           pOutput)
{
    if (pOutput) {
        SHORT i;
        switch(pAd->PortCfg.MBSSID[MAIN_MBSSID].SharedKey[index].KeyLen) {
            case 5:
            case 13:
            case 16:
                sprintf(pOutput, "%s", pAd->PortCfg.MBSSID[MAIN_MBSSID].SharedKey[index].Key);
                break;
            case 10:
                for (i=0; i<10; i++)
                    sprintf(pOutput, "%s%02x", pOutput, pAd->PortCfg.MBSSID[MAIN_MBSSID].SharedKey[index].Key[i]);
                break;
            case 26:
                for (i=0; i<26; i++)
                    sprintf(pOutput, "%s%02x", pOutput, pAd->PortCfg.MBSSID[MAIN_MBSSID].SharedKey[index].Key[i]);
                break;
            case 32:
                for (i=0; i<32; i++)
                    sprintf(pOutput, "%s%02x", pOutput, pAd->PortCfg.MBSSID[MAIN_MBSSID].SharedKey[index].Key[i]);
                break;
        }
    }
}

VOID getCfgKey1Str(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    getCfgKeyStr(pAd, 0, pOutput);
}

VOID getCfgKey2Str(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    getCfgKeyStr(pAd, 1, pOutput);
}

VOID getCfgKey3Str(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    getCfgKeyStr(pAd, 2, pOutput);
}

VOID getCfgKey4Str(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    getCfgKeyStr(pAd, 3, pOutput);
}

VOID getCfgHSCounter(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pOutput)
{
    if (pOutput)
        sprintf(pOutput, "%d", pAd->PortCfg.bEnableHSCounter);
}

BOOLEAN getCfgDataForSTA(
    IN  PRTMP_ADAPTER   pAd,
    OUT PCHAR           pDataBuf)
{
    struct AP_CFG_ITEM_INFO *pApCfgItemTable;
    //SHORT                   dataLen = 0;
    CHAR                    temp1[152] = {0}, temp2[216] = {0};

    if (pDataBuf == NULL)
        return FALSE;
    for (pApCfgItemTable = AP_CONFIG_ITEM_TABLE; pApCfgItemTable->name != NULL; pApCfgItemTable++ ) 
    {
        pApCfgItemTable->GetCfgData(pAd, temp1);
        sprintf(temp2, "%s=%s", pApCfgItemTable->name, temp1);
        if ( (strlen(temp2) + strlen(pDataBuf)) > MAX_CFG_BUFFER_LEN ) 
        {
            DBGPRINT(RT_DEBUG_TRACE, "getCfgDataForSTA(Sync.c): Config Data is too big.\n");
            return FALSE;
        }
        sprintf(pDataBuf,"%s%s\n", pDataBuf, temp2);
        memset(temp1, 0, 33);
        memset(temp2, 0, 80);
    }
    DBGPRINT(RT_DEBUG_TRACE, "getCfgDataForSTA(Sync.c): Cfg Data ->\n%s\n", pDataBuf);
    return TRUE;
}

