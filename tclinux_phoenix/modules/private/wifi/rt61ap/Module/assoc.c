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
    assoc.c
 
    Abstract:
    Handle association related requests either from WSTA or from local MLME
 
    Revision History:
    Who         When          What
    --------    ----------    ----------------------------------------------
    John Chang  08-04-2003    created for 11g soft-AP
    Rory Chen   02-04-2005    Support RT61
 */

#include "rt_config.h"

#ifdef WSC_SUPPORT
#ifdef DBG
BOOLEAN     g_b_test_with_intel_sdk = FALSE;
#endif
#endif // WSC_SUPPORT //
/*  
    ==========================================================================
    Description: 
        association state machine init, including state transition and timer init
    Parameters: 
        S - pointer to the association state machine
    Note:
        The state machine looks like the following 
        
                                    ASSOC_IDLE             
        MT2_MLME_DISASSOC_REQ    mlme_disassoc_req_action 
        MT2_PEER_DISASSOC_REQ    peer_disassoc_action     
        MT2_PEER_ASSOC_REQ       drop                     
        MT2_PEER_REASSOC_REQ     drop                     
        MT2_CLS3ERR              cls3err_action           
    ==========================================================================
 */
VOID AssocStateMachineInit(
    IN	PRTMP_ADAPTER	pAd, 
    IN  STATE_MACHINE *S, 
    OUT STATE_MACHINE_FUNC Trans[]) 
{
    StateMachineInit(S, (STATE_MACHINE_FUNC*)Trans, MAX_ASSOC_STATE, MAX_ASSOC_MSG, (STATE_MACHINE_FUNC)Drop, ASSOC_IDLE, ASSOC_MACHINE_BASE);

    StateMachineSetAction(S, ASSOC_IDLE, MT2_MLME_DISASSOC_REQ, (STATE_MACHINE_FUNC)MlmeDisassocReqAction);
    StateMachineSetAction(S, ASSOC_IDLE, MT2_PEER_DISASSOC_REQ, (STATE_MACHINE_FUNC)PeerDisassocReqAction);
    StateMachineSetAction(S, ASSOC_IDLE, MT2_PEER_ASSOC_REQ, (STATE_MACHINE_FUNC)PeerAssocReqAction);
    StateMachineSetAction(S, ASSOC_IDLE, MT2_PEER_REASSOC_REQ, (STATE_MACHINE_FUNC)PeerReassocReqAction);
//    StateMachineSetAction(S, ASSOC_IDLE, MT2_CLS3ERR, (STATE_MACHINE_FUNC)Cls3errAction);
}

/*
    ==========================================================================
    Description:
        peer assoc req handling procedure
    Parameters:
        Adapter - Adapter pointer
        Elem - MLME Queue Element
    Pre:
        the station has been authenticated and the following information is stored
    Post  :
        -# An association response frame is generated and sent to the air
    ==========================================================================
 */
VOID PeerAssocReqAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem) 
{
    UCHAR            Addr1[MAC_ADDR_LEN], Addr2[MAC_ADDR_LEN];
    HEADER_802_11    AssocRspHdr;
    UCHAR            RateIe = IE_SUPP_RATES;
    USHORT           ListenInterval;
    USHORT           CapabilityInfo;
    USHORT           StatusCode = 0;
    USHORT           Aid = 1;
    PUCHAR           pOutBuffer = NULL;
    ULONG            FrameLen = 0;
    CHAR             Ssid[32];
    UCHAR            SsidLen;
    UCHAR            SupportedRatesLen;
    UCHAR            SupportedRates[MAX_LEN_OF_SUPPORTED_RATES];
    UCHAR            MaxSupportedRate = 0;
    INT              i;
    UCHAR            RSNIE_Len;
    UCHAR            RSN_IE[MAX_LEN_OF_RSNIE];
    MAC_TABLE_ENTRY  *pEntry;
    BOOLEAN          bWmmCapable;
    ULONG            RalinkIe;
	BOOLEAN			 bRejectSTA = FALSE;
	PFRAME_802_11    Fr = (PFRAME_802_11)Elem->Msg;
    USHORT           CapabilityInfoForAssocResp;
#ifdef WSC_SUPPORT
    BOOLEAN          bWscCapable = FALSE;
#endif // WSC_SUPPORT //

    // frame sanity check
    if (! PeerAssocReqSanity(pAd, Elem->Msg, Elem->MsgLen, Addr1, Addr2, &CapabilityInfo, &ListenInterval, &SsidLen, &Ssid[0], &SupportedRatesLen, &SupportedRates[0],RSN_IE, &RSNIE_Len, &bWmmCapable,
#ifdef WSC_SUPPORT
		&bWscCapable,
#endif // WSC_SUPPORT //
		&RalinkIe))
        return;

    pEntry = MacTableLookup(pAd, Addr2);
    if (!pEntry)
    	return;

    // clear the previous Pairwise key table
    if(pEntry->Aid != 0)
    {
    	NdisZeroMemory(&pEntry->PairwiseKey, sizeof(CIPHER_KEY)); 	
    	AsicRemovePairwiseKeyEntry(pAd, (UCHAR)pEntry->Aid);
    	
		//notify 802.1x-deamon to discard the sta
		RTMPHandleNotify8021xDiscardSta(pAd, pEntry);
    }		

#ifdef WSC_SUPPORT
        // since sta has been left, ap should receive EapolStart and EapRspId again.
        pEntry->Receive_EapolStart_EapRspId = 0;
        // only support WSC in ra0 now, 2006.11.10
        if (pEntry->ApIdx == MAIN_MBSSID)
        {
            if (MAC_ADDR_EQUAL(pEntry->Addr, pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EntryAddr))
            {
                pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EntryApIdx = WSC_INIT_ENTRY_APIDX;
                memset(pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EntryAddr, 0, MAC_ADDR_LEN);
                RTMPCancelTimer(&pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EapolTimer);
                pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EapolTimerRunning = FALSE;
                DBGPRINT(RT_DEBUG_TRACE, "Reset EntryApIdx to WSC_INIT_ENTRY_APIDX.\n");
            }
        }

    pEntry->bWscCapable = bWscCapable;
    if ((RSNIE_Len == 0) && 
        (pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscConfMode != WSC_DISABLE))
        pEntry->bWscCapable = TRUE;
#endif // WSC_SUPPORT //

	// for hidden SSID sake, SSID in AssociateRequest should be fully verified
    if ((SsidLen != pAd->PortCfg.MBSSID[pEntry->ApIdx].SsidLen) || (NdisEqualMemory(Ssid, pAd->PortCfg.MBSSID[pEntry->ApIdx].Ssid, SsidLen)==0))
        return;

    // ignore request from unwanted STA
    if (! ApCheckAccessControlList(pAd, Addr2, pEntry->ApIdx))
        return;
    
    DBGPRINT(RT_DEBUG_TRACE, "ASSOC - receive ASSOC request from %02x:%02x:%02x:%02x:%02x:%02x\n",
        Addr2[0],Addr2[1],Addr2[2],Addr2[3],Addr2[4],Addr2[5]);
    
	// sta supported rates must support all basic rates
	for (i=0; i<pAd->PortCfg.SupportedRatesLen; i++) {
		int j;
		// the basic rate ap supported 
		if (pAd->PortCfg.SupportedRates[i] & 0x80) {
			bRejectSTA = TRUE;
			for (j=0; j<SupportedRatesLen; j++) {
				if ((SupportedRates[j] & 0x7f) == (pAd->PortCfg.SupportedRates[i] & 0x7f)) {
					bRejectSTA = FALSE;
					break;
				}
			}
			if (bRejectSTA == TRUE)
				break;
		}
	}

    // supported rates array may not be sorted. sort it and find the maximum rate
    for (i=0; i<SupportedRatesLen; i++)
    {
        if (MaxSupportedRate < (SupportedRates[i] & 0x7f)) 
            MaxSupportedRate = SupportedRates[i] & 0x7f;
    }            
    
	if (bRejectSTA)
		StatusCode = MLME_ASSOC_REJ_DATA_RATE;
	else 
	    // qualify this STA's auth_asoc status in the MAC table, decide StatusCode
	    StatusCode = BuildAssociation(pAd, Addr2, CapabilityInfo, MaxSupportedRate, RSN_IE, &RSNIE_Len, bWmmCapable, RalinkIe, &Aid);

    CapabilityInfoForAssocResp = pAd->PortCfg.MBSSID[pEntry->ApIdx].CapabilityInfo; //use AP's cability 
#ifdef WSC_SUPPORT
	if ((pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.WscConfMode != WSC_DISABLE) && 
        (CapabilityInfo & 0x0010))
	{
		CapabilityInfoForAssocResp |= 0x0010;
	}
#endif // WSC_SUPPORT //

    // send Association Response
    pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
    if(pOutBuffer == NULL)
        return;
        
    DBGPRINT(RT_DEBUG_TRACE, "ASSOC - Send ASSOC response (Status=%d) from IF(ra%d)...\n\n", StatusCode, pEntry->ApIdx);
    Aid |= 0xc000; // 2 most significant bits should be ON
    MgtMacHeaderInit(pAd, &AssocRspHdr, SUBTYPE_ASSOC_RSP, 0, Addr2, pAd->PortCfg.MBSSID[pEntry->ApIdx].Bssid);

    if ((pAd->PortCfg.PhyMode == PHY_11BG_MIXED) || (pAd->PortCfg.PhyMode == PHY_11G))
    {
        UCHAR SupportedRatesLen = 4;
        UCHAR ExtendedRatesIe = IE_EXT_SUPP_RATES;
        UCHAR ExtendedRatesLen = pAd->PortCfg.SupportedRatesLen - SupportedRatesLen;
        
        MakeOutgoingFrame(pOutBuffer,            &FrameLen,
                          sizeof(HEADER_802_11), &AssocRspHdr,
                          2,                     &CapabilityInfoForAssocResp,
                          2,                     &StatusCode,
                          2,                     &Aid,
                          1,                     &RateIe,
                          1,                     &SupportedRatesLen,
                          SupportedRatesLen,     pAd->PortCfg.SupportedRates,
                          1,                     &ExtendedRatesIe,
                          1,                     &ExtendedRatesLen,
                          ExtendedRatesLen,      &pAd->PortCfg.SupportedRates[SupportedRatesLen],
                          END_OF_ARGS);
    }
    else	// include A band
    {
        MakeOutgoingFrame(pOutBuffer,                       &FrameLen,
                          sizeof(HEADER_802_11),            &AssocRspHdr,
                          2,                                &pAd->PortCfg.MBSSID[pEntry->ApIdx].CapabilityInfo ,//use AP's cability 
                          2,                                &StatusCode,
                          2,                                &Aid,
                          1,                                &RateIe,
                          1,                                &pAd->PortCfg.SupportedRatesLen,
                          pAd->PortCfg.SupportedRatesLen,   pAd->PortCfg.SupportedRates,
                          END_OF_ARGS);
    }

#ifdef WMM_SUPPORT
    // add WMM IE here
    if (pAd->PortCfg.MBSSID[pEntry->ApIdx].bWmmCapable && CLIENT_STATUS_TEST_FLAG(pEntry, fCLIENT_STATUS_WMM_CAPABLE))
    {
        ULONG TmpLen;
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
#endif
    
    MiniportMMRequest(pAd, pOutBuffer, FrameLen);
    kfree(pOutBuffer);

	// enqueue a EAPOL_START message to trigger WPA state machine doing the authentication
    if (StatusCode == MLME_SUCCESS)
	{
#ifdef RTL865X_FAST_PATH
		rtl865x_extDev_addHost(pEntry->Addr, CONFIG_8139CP_VID, pAd->PortCfg.MBSSID[pEntry->ApIdx].mylinkid);
#endif
		pAd->RTSignal.Sig = SIG_ASSOCIATION;
		pAd->RTSignal.Sequence = Fr->Hdr.Sequence;
		NdisMoveMemory(pAd->RTSignal.MacAddr, pEntry->Addr, MAC_ADDR_LEN);
		SendSingalToDaemon(pAd, SIGUSR2, pAd->IappPid);

	    if ((pEntry->AuthMode == Ndis802_11AuthModeWPAPSK) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2PSK))
	    {
#ifdef WSC_SUPPORT
            /*
                In WPA-PSK mode,
                If Association Request of station has RSN/SSN, WPS AP Must Not send EAP-Request/Identity to station 
                no matter WPS AP does receive EAPoL-Start from STA or not.
            */
            if (pEntry->bWscCapable && (RSNIE_Len == 0))
            {
                RTMPAddTimer(&pEntry->EnqueueEapolStartTimerForWsc, WSC_EAPOL_START_TIME_OUT);
				pEntry->EnqueueEapolStartTimerForWscRunning = TRUE;
                DBGPRINT(RT_DEBUG_TRACE, "ASSOC - IF(ra%d) This is a WPS Client.\n\n", pEntry->ApIdx);
                return;
            }
            else
            {
#if defined(DBG) && defined(WSC_SUPPORT)
                if (!g_b_test_with_intel_sdk)
#endif
                {
                    pEntry->bWscCapable = FALSE;
                    pEntry->Receive_EapolStart_EapRspId = (WSC_ENTRY_GET_EAPOL_START | WSC_ENTRY_GET_EAP_RSP_ID);
                }
            }

#if defined(DBG) && defined(WSC_SUPPORT)
            if (g_b_test_with_intel_sdk)
#endif
            {
                if ((pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscConfMode != WSC_DISABLE) &&
                    MAC_ADDR_EQUAL(pEntry->Addr, pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EntryAddr))
                {
                    RTMPAddTimer(&pEntry->EnqueueEapolStartTimerForWsc, WSC_EAPOL_START_TIME_OUT);
    				pEntry->EnqueueEapolStartTimerForWscRunning = TRUE;
                }
            }
#endif // WSC_SUPPORT //            
            // Enqueue a EAPOL-start message with the pEntry for WPAPSK State Machine
            if (!pEntry->EnqueueStartForPSKTimerRunning
#ifdef WSC_SUPPORT
                && !pEntry->bWscCapable
#endif // WSC_SUPPORT //
                )
			{				
				RTMPAddTimer(&pEntry->EnqueueStartForPSKTimer, (300 * HZ)/1000);
				pEntry->EnqueueStartForPSKTimerRunning = TRUE;	
			}	
	    }
	    else if ((pEntry->AuthMode == Ndis802_11AuthModeWPA2) && (RSNIE_Len == 38))
	    {// Key cache
	    	// WZC didn't support PMK cache in assoc phase.
#if 0
	    	INT	CacheIdx;
	    	
	    	if (((CacheIdx = RTMPSearchPMKIDCache(pAd, pEntry->ApIdx, pEntry->Addr)) != -1) && (RTMPEqualMemory((RSN_IE + 22), &pAd->PortCfg.MBSSID[pEntry->ApIdx].PMKIDCache.BSSIDInfo[CacheIdx].PMKID, LEN_PMKID)))
	    	{
		    	// Enqueue a EAPOL-start message with the pEntry
		        MlmeEnqueue(pAd, WPA_STATE_MACHINE, MACHINE_TYPE_EAPOLStart, 6, &Addr2);
		    	pEntry->PMKID_CacheIdx = CacheIdx;
		    	DBGPRINT(RT_DEBUG_ERROR, "ASSOC - 1.PMKID matched and start key cache algorithm\n");
	    	}
	    	else
	    	{
	    		pEntry->PMKID_CacheIdx = ENTRY_NOT_FOUND;
		    	DBGPRINT(RT_DEBUG_ERROR, "ASSOC - 1.Recv PMKID=%02x:%02x:%02x:%02x:%02x:%02x\n", *(RSN_IE+22),*(RSN_IE+23),*(RSN_IE+24),*(RSN_IE+25),*(RSN_IE+26),*(RSN_IE+27));
	    	}
#endif

#ifdef WSC_SUPPORT
            if ((pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscConfMode != WSC_DISABLE) &&
                (pEntry->ApIdx == MAIN_MBSSID) && 
                MAC_ADDR_EQUAL(pEntry->Addr, pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EntryAddr))
            {
                if (!pEntry->EnqueueEapolStartTimerForWscRunning)
                {
                    pEntry->EnqueueEapolStartTimerForWscRunning = TRUE;
                    RTMPAddTimer(&pEntry->EnqueueEapolStartTimerForWsc, WSC_EAPOL_START_TIME_OUT);
                }
            }
#endif // WSC_SUPPORT //
	    }
#ifdef WSC_SUPPORT
		else
		{
			/*
				In WEP mode (no 802.1X indicated in beacon),
				Preferably, EAP-Request/Identity should not be sent unless STA first sends EAPoL-Start.
				Therefore, when WPS AP is configured as static WEP mode, driver would not enqueue EAPoL-Start.
			*/
			if ((pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscConfMode != WSC_DISABLE) &&
				(pAd->PortCfg.MBSSID[MAIN_MBSSID].IEEE8021X || (pAd->PortCfg.MBSSID[MAIN_MBSSID].WepStatus != Ndis802_11WEPEnabled)) &&
				(pEntry->ApIdx == MAIN_MBSSID) && 
				MAC_ADDR_EQUAL(pEntry->Addr, pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EntryAddr))
			{
				if (!pEntry->EnqueueEapolStartTimerForWscRunning)
				{
					pEntry->EnqueueEapolStartTimerForWscRunning = TRUE;
					RTMPAddTimer(&pEntry->EnqueueEapolStartTimerForWsc, WSC_EAPOL_START_TIME_OUT);
				}
			}
		}
#endif // WSC_SUPPORT //
	}
}

/*
    ==========================================================================
    Description:
        mlme reassoc req handling procedure
    Parameters:
        Elem - 
    Pre:
        -# SSID  (Adapter->PortCfg.ssid[])
        -# BSSID (AP address, Adapter->PortCfg.bssid)
        -# Supported rates (Adapter->PortCfg.supported_rates[])
        -# Supported rates length (Adapter->PortCfg.supported_rates_len)
        -# Tx power (Adapter->PortCfg.tx_power)
    ==========================================================================
 */
VOID PeerReassocReqAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem) 
{
    UCHAR            ApAddr[MAC_ADDR_LEN], Addr1[MAC_ADDR_LEN], Addr2[MAC_ADDR_LEN];
    HEADER_802_11    ReassocRspHdr;
    UCHAR            RateIe = IE_SUPP_RATES;
    USHORT           CapabilityInfo, ListenInterval;
    USHORT           StatusCode = MLME_SUCCESS;
    USHORT           Aid = 1;
    ULONG            FrameLen = 0;
    PUCHAR           pOutBuffer = NULL;
    CHAR             Ssid[32];
    UCHAR            SsidLen;
    UCHAR            SupportedRatesLen;
    UCHAR            SupportedRates[MAX_LEN_OF_SUPPORTED_RATES];
    UCHAR            MaxSupportedRate = 0;
    INT              i;
    UCHAR            RSNIE_Len;
    UCHAR            RSN_IE[MAX_LEN_OF_RSNIE];
    MAC_TABLE_ENTRY  *pEntry;
    BOOLEAN          bWmmCapable;
    ULONG            RalinkIe;
	BOOLEAN			 bRejectSTA = FALSE;
	PFRAME_802_11    Fr = (PFRAME_802_11)Elem->Msg;
#ifdef WSC_SUPPORT
    BOOLEAN          bWscCapable = FALSE;
#endif // WSC_SUPPORT //

    // frame sanity check
    if (! PeerReassocReqSanity(pAd, Elem->Msg, Elem->MsgLen, Addr1, Addr2, &CapabilityInfo, &ListenInterval, ApAddr, &SsidLen, &Ssid[0], &SupportedRatesLen, &SupportedRates[0],RSN_IE, &RSNIE_Len, &bWmmCapable,
#ifdef WSC_SUPPORT
		&bWscCapable,
#endif // WSC_SUPPORT //
		&RalinkIe))
        return;

    pEntry = MacTableLookup(pAd, Addr2);
    if (!pEntry)
    	return;

	// clear the previous Pairwise key table
    if(pEntry->Aid != 0)
    {
    	NdisZeroMemory(&pEntry->PairwiseKey, sizeof(CIPHER_KEY)); 	
    	AsicRemovePairwiseKeyEntry(pAd, (UCHAR)pEntry->Aid);

		//notify 802.1x-deamon to discard the sta
		RTMPHandleNotify8021xDiscardSta(pAd, pEntry);
    }	
    	
#ifdef WSC_SUPPORT
    // since sta has been left, ap should receive EapolStart and EapRspId again.
    pEntry->Receive_EapolStart_EapRspId = 0;
    // only support WSC in ra0 now, 2006.11.10
    if (pEntry->ApIdx == MAIN_MBSSID)
    {
        if (MAC_ADDR_EQUAL(pEntry->Addr, pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EntryAddr))
        {
            pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EntryApIdx = WSC_INIT_ENTRY_APIDX;
            memset(pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EntryAddr, 0, MAC_ADDR_LEN);
            RTMPCancelTimer(&pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EapolTimer);
            pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EapolTimerRunning = FALSE;
        }
    }
    
    pEntry->bWscCapable = bWscCapable;
    if ((RSNIE_Len == 0) && 
        (pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscConfMode != WSC_DISABLE))
        pEntry->bWscCapable = TRUE;
#endif // WSC_SUPPORT //

    // for hidden SSID sake, SSID in AssociateRequest should be fully verified
    if ((SsidLen != pAd->PortCfg.MBSSID[pEntry->ApIdx].SsidLen) || (NdisEqualMemory(Ssid, pAd->PortCfg.MBSSID[pEntry->ApIdx].Ssid, SsidLen)==0))
        return;
    
    // ignore request from unwanted STA
    if (! ApCheckAccessControlList(pAd, Addr2, pEntry->ApIdx))
        return;
    
    DBGPRINT(RT_DEBUG_TRACE, "ASSOC - receive RE-ASSOC request from %02x:%02x:%02x:%02x:%02x:%02x\n",
        Addr2[0],Addr2[1],Addr2[2],Addr2[3],Addr2[4],Addr2[5]);
    
	// sta supported rates must support all basic rates
	for (i=0; i<pAd->PortCfg.SupportedRatesLen; i++) {
		int j;
		// the basic rate ap supported 
		if (pAd->PortCfg.SupportedRates[i] & 0x80) {
			bRejectSTA = TRUE;
			for (j=0; j<SupportedRatesLen; j++) {
				if ((SupportedRates[j] & 0x7f) == (pAd->PortCfg.SupportedRates[i] & 0x7f)) {
					bRejectSTA = FALSE;
					break;
				}
			}
			if (bRejectSTA == TRUE)
				break;
		}
	}

    // supported rates array may not be sorted. sort it and find the maximum rate
    for (i=0; i<SupportedRatesLen; i++)
    {
        if (MaxSupportedRate < (SupportedRates[i] & 0x7f))
            MaxSupportedRate = SupportedRates[i] & 0x7f;
    }            
    
	if (bRejectSTA)
		StatusCode = MLME_ASSOC_REJ_DATA_RATE;
	else 
    	// qualify this STA's auth_asoc status in the MAC table, decide StatusCode
    	StatusCode = BuildAssociation(pAd, Addr2, CapabilityInfo, MaxSupportedRate, RSN_IE, &RSNIE_Len, bWmmCapable, RalinkIe, &Aid);

    // reply Re-association Response
    pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
    if(pOutBuffer == NULL)
        return;

    DBGPRINT(RT_DEBUG_TRACE, "ASSOC - Send RE-ASSOC response (Status = %d)...\n", StatusCode);
    Aid |= 0xc000; // 2 most significant bits should be ON
    MgtMacHeaderInit(pAd, &ReassocRspHdr, SUBTYPE_REASSOC_RSP, 0, Addr2, pAd->PortCfg.MBSSID[pEntry->ApIdx].Bssid);

    if ((pAd->PortCfg.PhyMode == PHY_11BG_MIXED) || (pAd->PortCfg.PhyMode == PHY_11G))
    {
        UCHAR SupportedRatesLen = 4;
        UCHAR ExtendedRatesIe = IE_EXT_SUPP_RATES;
        UCHAR ExtendedRatesLen = pAd->PortCfg.SupportedRatesLen - SupportedRatesLen;
        
        MakeOutgoingFrame(pOutBuffer,					&FrameLen,
                          sizeof(HEADER_802_11),		&ReassocRspHdr,
                          2,								&pAd->PortCfg.MBSSID[pEntry->ApIdx].CapabilityInfo ,//use AP's cability 
                          2,								&StatusCode,
                          2,								&Aid,
                          1,								&RateIe,
                          1,								&SupportedRatesLen,
                          SupportedRatesLen,			pAd->PortCfg.SupportedRates,
                          1,								&ExtendedRatesIe,
                          1,								&ExtendedRatesLen,
                          ExtendedRatesLen,				&pAd->PortCfg.SupportedRates[SupportedRatesLen],
                          END_OF_ARGS);
    }
    else	// include A band
    {
        MakeOutgoingFrame(pOutBuffer,                       &FrameLen,
                          sizeof(HEADER_802_11),            &ReassocRspHdr,
                          2,                                &pAd->PortCfg.MBSSID[pEntry->ApIdx].CapabilityInfo ,//use AP's cability 
                          2,                                &StatusCode,
                          2,                                &Aid,
                          1,                                &RateIe,
                          1,                                &pAd->PortCfg.SupportedRatesLen, 
                          pAd->PortCfg.SupportedRatesLen,   pAd->PortCfg.SupportedRates,
                          END_OF_ARGS);
    }
    
#ifdef WMM_SUPPORT
    // add WMM IE here
    if (pAd->PortCfg.MBSSID[pEntry->ApIdx].bWmmCapable && CLIENT_STATUS_TEST_FLAG(pEntry, fCLIENT_STATUS_WMM_CAPABLE))
    {
        ULONG TmpLen;
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
    
    MiniportMMRequest(pAd, pOutBuffer, FrameLen);
    kfree(pOutBuffer);

	// enqueue a EAPOL_START message to trigger WPA state machine doing the authentication
    if (StatusCode == MLME_SUCCESS)
	{
#ifdef RTL865X_FAST_PATH
		rtl865x_extDev_addHost(pEntry->Addr, CONFIG_8139CP_VID, pAd->PortCfg.MBSSID[pEntry->ApIdx].mylinkid);
#endif

		pAd->RTSignal.Sig = SIG_REASSOCIATION;
		pAd->RTSignal.Sequence = Fr->Hdr.Sequence;
		NdisMoveMemory(pAd->RTSignal.MacAddr, pEntry->Addr, MAC_ADDR_LEN);
		NdisMoveMemory(pAd->RTSignal.CurrAPAddr, ApAddr, MAC_ADDR_LEN);
		SendSingalToDaemon(pAd, SIGUSR2, pAd->IappPid);
		
	    if ((pEntry->AuthMode == Ndis802_11AuthModeWPAPSK) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2PSK))
	    {
#ifdef WSC_SUPPORT
            /*
                In WPA-PSK mode,
                If Association Request of station has RSN/SSN IE, WPS AP Must Not send EAP-Request/Identity to station 
                no matter WPS AP does receive EAPoL-Start from STA or not.
            */
            if (pEntry->bWscCapable && RSNIE_Len == 0)
            {
                RTMPAddTimer(&pEntry->EnqueueEapolStartTimerForWsc, WSC_EAPOL_START_TIME_OUT);
				pEntry->EnqueueEapolStartTimerForWscRunning = TRUE;
                DBGPRINT(RT_DEBUG_TRACE, "ASSOC - IF(ra%d) This is a WPS Client.\n\n", pEntry->ApIdx);
                return;
            }
            else
            {
#if defined(DBG) && defined(WSC_SUPPORT)           
                if (!g_b_test_with_intel_sdk)
#endif
                {
                    pEntry->bWscCapable = FALSE;
                    pEntry->Receive_EapolStart_EapRspId = (WSC_ENTRY_GET_EAPOL_START | WSC_ENTRY_GET_EAP_RSP_ID);
                }
            }

#if defined(DBG) && defined(WSC_SUPPORT)       
            if (g_b_test_with_intel_sdk)
#endif
            {
                if ((pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscConfMode != WSC_DISABLE) &&
                    MAC_ADDR_EQUAL(pEntry->Addr, pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EntryAddr))
                {
                    RTMPAddTimer(&pEntry->EnqueueEapolStartTimerForWsc, WSC_EAPOL_START_TIME_OUT);
    				pEntry->EnqueueEapolStartTimerForWscRunning = TRUE;
                }
            }
#endif // WSC_SUPPORT //            
            // Enqueue a EAPOL-start message with the pEntry
			if (!pEntry->EnqueueStartForPSKTimerRunning
#ifdef WSC_SUPPORT
				&& !pEntry->bWscCapable
#endif // WSC_SUPPORT //
				)
			{				
				RTMPAddTimer(&pEntry->EnqueueStartForPSKTimer, (300 * HZ)/1000);
				pEntry->EnqueueStartForPSKTimerRunning = TRUE;	
			}	
	    }
	    else if ((pEntry->AuthMode == Ndis802_11AuthModeWPA2) && (RSNIE_Len == 38))
	    {// Key cache
	    	INT	CacheIdx;
	    	
	    	if (((CacheIdx = RTMPSearchPMKIDCache(pAd, pEntry->ApIdx, pEntry->Addr)) != -1) && (RTMPEqualMemory((RSN_IE + 22), &pAd->PortCfg.MBSSID[pEntry->ApIdx].PMKIDCache.BSSIDInfo[CacheIdx].PMKID, LEN_PMKID)))
	    	{
                if (!pEntry->EnqueueStartForPSKTimerRunning
#ifdef WSC_SUPPORT
					&& !pEntry->bWscCapable
#endif // WSC_SUPPORT //
					)
		    	// Enqueue a EAPOL-start message with the pEntry
		        //MlmeEnqueue(pAd, WPA_STATE_MACHINE, MACHINE_TYPE_EAPOLStart, 6, &Addr2);
				if (!pEntry->EnqueueStartForPSKTimerRunning)
				{				
					RTMPAddTimer(&pEntry->EnqueueStartForPSKTimer, (300 * HZ)/1000);
					pEntry->EnqueueStartForPSKTimerRunning = TRUE;	
				}

		    	pEntry->PMKID_CacheIdx = CacheIdx;
		    	DBGPRINT(RT_DEBUG_ERROR, "ASSOC - 2.PMKID matched and start key cache algorithm\n");
	    	}
	    	else
	    	{
	    		pEntry->PMKID_CacheIdx = ENTRY_NOT_FOUND;
		    	DBGPRINT(RT_DEBUG_ERROR, "ASSOC - 2.Recv PMKID=%02x:%02x:%02x:%02x:%02x:%02x\n", *(RSN_IE+22),*(RSN_IE+23),*(RSN_IE+24),*(RSN_IE+25),*(RSN_IE+26),*(RSN_IE+27));
	    	}
            
#ifdef WSC_SUPPORT
            if ((pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscConfMode != WSC_DISABLE) &&
                (pEntry->ApIdx == MAIN_MBSSID) && 
                MAC_ADDR_EQUAL(pEntry->Addr, pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EntryAddr))
            {
                if (!pEntry->EnqueueEapolStartTimerForWscRunning)
                {
                    pEntry->EnqueueEapolStartTimerForWscRunning = TRUE;
                    RTMPAddTimer(&pEntry->EnqueueEapolStartTimerForWsc, WSC_EAPOL_START_TIME_OUT);
                }
            }
#endif // WSC_SUPPORT //

	    }
#ifdef WSC_SUPPORT        
        else
        {
            /*
                In WEP mode (no 802.1X indicated in beacon),
                Preferably, EAP-Request/Identity should not be sent unless STA first sends EAPoL-Start.
                Therefore, when WPS AP is configured as static WEP mode, driver would not enqueue EAPoL-Start.
            */
            if ((pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscConfMode != WSC_DISABLE) &&
                (pAd->PortCfg.MBSSID[MAIN_MBSSID].IEEE8021X || (pAd->PortCfg.MBSSID[MAIN_MBSSID].WepStatus != Ndis802_11WEPEnabled)) &&
                (pEntry->ApIdx == MAIN_MBSSID) && 
                MAC_ADDR_EQUAL(pEntry->Addr, pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.EntryAddr))
            {
                if (!pEntry->EnqueueEapolStartTimerForWscRunning)
                {
                    pEntry->EnqueueEapolStartTimerForWscRunning = TRUE;
                    RTMPAddTimer(&pEntry->EnqueueEapolStartTimerForWsc, WSC_EAPOL_START_TIME_OUT);
                }
            }
	    }
#endif // WSC_SUPPORT //
	}
}

/*
    ==========================================================================
    Description:
        left part of IEEE 802.11/1999 p.374 
    Parameters:
        Elem - MLME message containing the received frame
    ==========================================================================
 */
VOID PeerDisassocReqAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem) 
{
    UCHAR           Addr2[MAC_ADDR_LEN];
    USHORT          Reason;
    MAC_TABLE_ENTRY *pEntry;

    if (! PeerDisassocReqSanity(pAd, Elem->Msg, Elem->MsgLen, Addr2, &Reason)) 
        return;
    
    pEntry = MacTableLookup(pAd, Addr2);
    if (pEntry)
    {
		//notify 802.1x-deamon to discard the sta
		RTMPHandleNotify8021xDiscardSta(pAd, pEntry);
		
        ApLogEvent(pAd, Addr2, EVENT_DISASSOCIATED, pEntry->ApIdx);
        MacTableDeleteEntry(pAd, Addr2);
#ifdef RTL865X_FAST_PATH
		rtl865x_extDev_removeHost(pEntry->Addr, CONFIG_8139CP_VID); 
#endif
    }

    DBGPRINT(RT_DEBUG_TRACE, "ASSOC - receive DIS-ASSOC request from %02x:%02x:%02x:%02x:%02x:%02x\n",
        Addr2[0],Addr2[1],Addr2[2],Addr2[3],Addr2[4],Addr2[5]);
}

/*
    ==========================================================================
    Description:
        Upper layer orders to disassoc s STA
    Parameters:
        Elem -
    ==========================================================================
 */
VOID MlmeDisassocReqAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem) 
{
    MLME_DISASSOC_REQ_STRUCT    *DisassocReq;
    HEADER_802_11               DisassocHdr;
    CHAR                        *OutBuffer = NULL;
    ULONG                       FrameLen = 0;
    MAC_TABLE_ENTRY             *pEntry;

    DisassocReq = (MLME_DISASSOC_REQ_STRUCT *)(Elem->Msg);

    pEntry = MacTableLookup(pAd, DisassocReq->Addr);
    if (pEntry)
    {
#ifdef RTL865X_FAST_PATH
		rtl865x_extDev_removeHost(pEntry->Addr, CONFIG_8139CP_VID); 
#endif
        ApLogEvent(pAd, DisassocReq->Addr, EVENT_DISASSOCIATED, pEntry->ApIdx);
        MacTableDeleteEntry(pAd, DisassocReq->Addr);

	    //send out a DISASSOC request frame
	    OutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
	    if(OutBuffer == NULL)
	        return;
	    
	    DBGPRINT(RT_DEBUG_TRACE, "ASSOC - MLME disassociates %02x:%02x:%02x:%02x:%02x:%02x; Send DISASSOC request\n",
	        DisassocReq->Addr[0],DisassocReq->Addr[1],DisassocReq->Addr[2],
	        DisassocReq->Addr[3],DisassocReq->Addr[4],DisassocReq->Addr[5]);
	    MgtMacHeaderInit(pAd, &DisassocHdr, SUBTYPE_DISASSOC, 0, DisassocReq->Addr, pAd->PortCfg.MBSSID[pEntry->ApIdx].Bssid);
	    MakeOutgoingFrame(OutBuffer,				&FrameLen, 
	                        sizeof(HEADER_802_11),	&DisassocHdr, 
	                        2,						&DisassocReq->Reason, 
	                        END_OF_ARGS);
	    MiniportMMRequest(pAd, OutBuffer, FrameLen);
	    kfree(OutBuffer);
    }
}

/*
    ==========================================================================
    Description:
        right part of IEEE 802.11/1999 page 374
    Note: 
        This event should never cause ASSOC state machine perform state
        transition, and has no relationship with CNTL machine. So we separate
        this routine as a service outside of ASSOC state transition table.
    ==========================================================================
 */
VOID Cls3errAction(
    IN PRTMP_ADAPTER pAd, 
    IN PUCHAR        pAddr1,
    IN PUCHAR        pAddr2)
{
    HEADER_802_11		DisassocHdr;
    CHAR				*OutBuffer = NULL;
    ULONG				FrameLen = 0;
    USHORT				Reason = REASON_CLS3ERR;
    MAC_TABLE_ENTRY		*pEntry;

    pEntry = MacTableLookup(pAd, pAddr2);
    if (pEntry)
    {
        //ApLogEvent(pAd, &pAddr, EVENT_DISASSOCIATED, pEntry->ApIdx);
        MacTableDeleteEntry(pAd, pAddr2);
    }
    
    // send out a DISASSOC request frame
    OutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
    if(OutBuffer == NULL)
        return;
    
    DBGPRINT(RT_DEBUG_TRACE, "ASSOC - Class 3 Error, Send DISASSOC frame to %02x:%02x:%02x:%02x:%02x:%02x\n",
        pAddr2[0],pAddr2[1],pAddr2[2],pAddr2[3],pAddr2[4],pAddr2[5]);
    MgtMacHeaderInit(pAd, &DisassocHdr, SUBTYPE_DISASSOC, 0, pAddr2, pAddr1);
    MakeOutgoingFrame(OutBuffer,				&FrameLen, 
                      sizeof(HEADER_802_11),	&DisassocHdr, 
                      2,							&Reason, 
                      END_OF_ARGS);
    MiniportMMRequest(pAd, OutBuffer, FrameLen);
    kfree(OutBuffer);
}
 
 /*
     ==========================================================================
     Description:
        search for entire MAC table to find next available AID to be used
        if none is available, return 0
     ==========================================================================
  */
USHORT AssignAid(
    IN PRTMP_ADAPTER	pAd,
    IN PMAC_TABLE_ENTRY	pEntry)
{
    USHORT  Aid = 0;

    if (pEntry)
        Aid = ((UMAXLONG)pEntry - (UMAXLONG)&pAd->MacTab.Content[0])/sizeof(MAC_TABLE_ENTRY);

    DBGPRINT(RT_DEBUG_TRACE, "AssignAid (AID=%d)\n", Aid);

    return Aid;
}

/*
    ==========================================================================
    Description:
       assign a new AID to the newly associated/re-associated STA and
       decide its MaxSupportedRate and CurrTxRate. Both rates should not
       exceed AP's capapbility
    Return:
       MLME_SUCCESS - association successfully built
       others - association failed due to resource issue
    ==========================================================================
 */
USHORT BuildAssociation(
    IN PRTMP_ADAPTER pAd,
    IN PUCHAR        pAddr,
    IN USHORT        CapabilityInfo,
    IN UCHAR         MaxSupportedRateIn500Kbps,
    IN UCHAR         *RSN,
    IN UCHAR         *pRSNLen,
    IN BOOLEAN       bWmmCapable,
    IN ULONG         ClientRalinkIe,
    OUT USHORT       *pAid)
{
    USHORT           StatusCode = MLME_SUCCESS;
    PMAC_TABLE_ENTRY pEntry;
    UCHAR            MaxSupportedRate = RATE_11;

    switch (MaxSupportedRateIn500Kbps)
    {
        case 108: MaxSupportedRate = RATE_54;   break;
        case 96:  MaxSupportedRate = RATE_48;   break;
        case 72:  MaxSupportedRate = RATE_36;   break;
        case 48:  MaxSupportedRate = RATE_24;   break;
        case 36:  MaxSupportedRate = RATE_18;   break;
        case 24:  MaxSupportedRate = RATE_12;   break;
        case 18:  MaxSupportedRate = RATE_9;    break;
        case 12:  MaxSupportedRate = RATE_6;    break;
        case 22:  MaxSupportedRate = RATE_11;   break;
        case 11:  MaxSupportedRate = RATE_5_5;  break;
        case 4:   MaxSupportedRate = RATE_2;    break;
        case 2:   MaxSupportedRate = RATE_1;    break;
        default:  MaxSupportedRate = RATE_11;   break;
    }

    pEntry = MacTableLookup(pAd, pAddr);
    if (pEntry && ((pEntry->Sst == SST_AUTH) || (pEntry->Sst == SST_ASSOC)))
    {
        // TODO:
        // should qualify other parameters, for example - capablity, supported rates, listen interval, ... etc
        // to decide the Status Code
        *pAid = AssignAid(pAd, pEntry);
        pEntry->Aid = *pAid;
        
        NdisMoveMemory(pEntry->RSN_IE, RSN, *pRSNLen);
        pEntry->RSNIE_Len = *pRSNLen;

        if (*pAid == 0)
            StatusCode = MLME_ASSOC_REJ_UNABLE_HANDLE_STA;
		else if ((pEntry->RSNIE_Len == 0) && (pAd->PortCfg.MBSSID[pEntry->ApIdx].AuthMode >= Ndis802_11AuthModeWPA))
        {
			StatusCode = MLME_ASSOC_DENY_OUT_SCOPE;
#ifdef WSC_SUPPORT
            if (pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.WscConfMode != WSC_DISABLE && 
                pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.EntryApIdx == WSC_INIT_ENTRY_APIDX)
            {
                DBGPRINT(RT_DEBUG_TRACE, "Add timer for WSC\n");                
                WscInitEntryFunc(pEntry);
                pEntry->Sst = SST_ASSOC;
                StatusCode = MLME_SUCCESS;
            }
            else
            {
                DBGPRINT(RT_DEBUG_TRACE, "##) WSC_STATE_MACHINE for this STA is OFF.<WscConfMode = %d, EntryApIdx =%d>\n", 
                                         pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.WscConfMode,
                                         pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.EntryApIdx);
                StatusCode = MLME_ASSOC_DENY_OUT_SCOPE;
            }
#endif // WSC_SUPPORT //
        }
        else
        {
            pEntry->Sst = SST_ASSOC;
            pEntry->GTKState = REKEY_NEGOTIATING;
            
            // patch for Nintendo DS support rate bug - it only support tx rate 1 and 2 
            if ((((pAddr[0]==0x00) && (pAddr[1]==0x09) && (pAddr[2]==0xBF)) || 
            	 ((pAddr[0]==0x00) && (pAddr[1]==0x16) && (pAddr[2]==0x56))) 
            		&& MaxSupportedRate == RATE_11)
            { 
            	DBGPRINT(RT_DEBUG_TRACE, "==>Assoc-Req from Nintendo DS client.\n");
            	MaxSupportedRate = RATE_2;
            }
            
            pEntry->MaxSupportedRate = min(pAd->PortCfg.MBSSID[pEntry->ApIdx].MaxTxRate, MaxSupportedRate);
            pEntry->CapabilityInfo = CapabilityInfo;
            if ((pEntry->AuthMode == Ndis802_11AuthModeWPAPSK) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2PSK))
            {
                pEntry->PrivacyFilter = Ndis802_11PrivFilter8021xWEP;
                pEntry->WpaState = AS_INITPSK;
            }
            else if ((pEntry->AuthMode == Ndis802_11AuthModeWPA) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2)
            	|| (pAd->PortCfg.MBSSID[pEntry->ApIdx].IEEE8021X == TRUE))
            {
                pEntry->PrivacyFilter = Ndis802_11PrivFilter8021xWEP;
                pEntry->WpaState = AS_AUTHENTICATION;
            }

            if (bWmmCapable)
            {
                CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_WMM_CAPABLE);
            }
            else
            {
                CLIENT_STATUS_CLEAR_FLAG(pEntry, fCLIENT_STATUS_WMM_CAPABLE);
            }

            if (pAd->PortCfg.bAggregationCapable)
            {
		if ((ClientRalinkIe & 0x00000001) == 1)
            	{
                	CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_AGGREGATION_CAPABLE);
                	CLIENT_STATUS_CLEAR_FLAG(pEntry, fCLIENT_STATUS_PIGGYBACK_CAPABLE);
            	}
            	else
            	{
	                CLIENT_STATUS_CLEAR_FLAG(pEntry, fCLIENT_STATUS_AGGREGATION_CAPABLE);
	                CLIENT_STATUS_CLEAR_FLAG(pEntry, fCLIENT_STATUS_PIGGYBACK_CAPABLE);
	            }
            }
            else
            {
                CLIENT_STATUS_CLEAR_FLAG(pEntry, fCLIENT_STATUS_AGGREGATION_CAPABLE);
                CLIENT_STATUS_CLEAR_FLAG(pEntry, fCLIENT_STATUS_PIGGYBACK_CAPABLE);
            }
            
            // if (pAd->PortCfg.MBSSID[pEntry->ApIdx].IEEE8021X)
                 pEntry->PortSecured = WPA_802_1X_PORT_NOT_SECURED;
            // else		
            //     pEntry->PortSecured = WPA_802_1X_PORT_SECURED;

            DBGPRINT(RT_DEBUG_TRACE, "BuildAssoc-IF(%d):AuthMode=%d,WepStatus=%d,GroupWepStatus=%d,WpaState=%d,AGGRE=%d,PiggyBack=%d,APSD=%d\n",
            	pEntry->ApIdx, pEntry->AuthMode, pEntry->WepStatus, pAd->PortCfg.MBSSID[pEntry->ApIdx].GroupKeyWepStatus, pEntry->WpaState,
            	CLIENT_STATUS_TEST_FLAG(pEntry, fCLIENT_STATUS_AGGREGATION_CAPABLE), CLIENT_STATUS_TEST_FLAG(pEntry, fCLIENT_STATUS_PIGGYBACK_CAPABLE),
            	CLIENT_STATUS_TEST_FLAG(pEntry, fCLIENT_STATUS_APSD_CAPABLE));

            pEntry->LastTxRate = pEntry->CurrTxRate;// last time tx rate;
            pEntry->CurrTxRate = pEntry->MaxSupportedRate;// RATE_24;

            ApLogEvent(pAd, pEntry->Addr, EVENT_ASSOCIATED, pEntry->ApIdx);
            ApUpdateCapabilityAndErpIe(pAd);
            
            DBGPRINT(RT_DEBUG_TRACE, "Init entry init retry timer\n");
            pEntry->RetryTimerRunning = FALSE;
            pEntry->RetryTimer.expires = jiffies + WPA_RETRY_EXEC_INTV;
            pEntry->RetryTimer.data = (unsigned long) pEntry;
            pEntry->RetryTimer.function = &WPARetryExec;		// This might change for different call routine
            pEntry->ReTryCounter = PEER_MSG1_RETRY_TIMER_CTR;
            
#ifdef WSC_SUPPORT
            if ((pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.WscConfMode != WSC_DISABLE) && 
                (pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.EntryApIdx == WSC_INIT_ENTRY_APIDX))
            {
                DBGPRINT(RT_DEBUG_TRACE, "Add timer for WSC\n");               
                WscInitEntryFunc(pEntry);
            }
            else
                DBGPRINT(RT_DEBUG_TRACE, "**) WSC_STATE_MACHINE for this STA is OFF.<WscConfMode = %d, EntryApIdx =%d>\n", 
                                         pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.WscConfMode,
                                         pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.EntryApIdx);
#endif // WSC_SUPPORT //
            
            DBGPRINT(RT_DEBUG_TRACE,"assign AID=%d to %02x:%02x:%02x:%02x:%02x:%02x,MaxSupportedRate=%dMbps,CurrTxRate=%dMbps\n", 
                *pAid, pAddr[0], pAddr[1], pAddr[2], pAddr[3], pAddr[4], pAddr[5], RateIdToMbps[pEntry->MaxSupportedRate], RateIdToMbps[pEntry->CurrTxRate]);
            DBGPRINT(RT_DEBUG_TRACE, "RSNIE_Len=0x%x,pEntry->RSNIE_Len=%d,pEntry->PrivacyFilter=%x\n",*pRSNLen,pEntry->RSNIE_Len,pEntry->PrivacyFilter);
            StatusCode = MLME_SUCCESS;
        }
    }
    else // CLASS 3 error should have been handled beforehand; here should be MAC table full
        StatusCode = MLME_ASSOC_REJ_UNABLE_HANDLE_STA;

    return StatusCode;
}

