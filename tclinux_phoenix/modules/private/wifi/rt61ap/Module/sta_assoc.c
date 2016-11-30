/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2006, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attemp
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************


	Module Name:
	sta_assoc.c

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Fonchi		2006-6-23		modified for rt61-APClinent
*/
#include "rt_config.h"
#include <net/iw_handler.h>


/*  
    ==========================================================================
    Description: 
        association state machine init, including state transition and timer init
    Parameters: 
        S - pointer to the association state machine
    Note:
        The state machine looks like the following 
    ==========================================================================
 */
VOID StaAssocStateMachineInit(
	IN	PRTMP_ADAPTER	pAd, 
	IN  STA_STATE_MACHINE *S, 
	OUT STA_STATE_MACHINE_FUNC Trans[]) 
{
	UCHAR i;

	StaStateMachineInit(S, (STA_STATE_MACHINE_FUNC*)Trans, STA_MAX_ASSOC_STATE, STA_MAX_ASSOC_MSG, (STA_STATE_MACHINE_FUNC)StaDrop, STA_ASSOC_IDLE, STA_ASSOC_MACHINE_BASE);

	// first column
	StaStateMachineSetAction(S, STA_ASSOC_IDLE, STA_MT2_MLME_ASSOC_REQ, (STA_STATE_MACHINE_FUNC)StaMlmeAssocReqAction);
	StaStateMachineSetAction(S, STA_ASSOC_IDLE, STA_MT2_MLME_DISASSOC_REQ, (STA_STATE_MACHINE_FUNC)StaMlmeDisassocReqAction);
	StaStateMachineSetAction(S, STA_ASSOC_IDLE, STA_MT2_PEER_DISASSOC_REQ, (STA_STATE_MACHINE_FUNC)StaPeerDisassocAction);
   
	// second column
	StaStateMachineSetAction(S, STA_ASSOC_WAIT_RSP, STA_MT2_MLME_ASSOC_REQ, (STA_STATE_MACHINE_FUNC)StaInvalidStateWhenAssoc);
	StaStateMachineSetAction(S, STA_ASSOC_WAIT_RSP, STA_MT2_MLME_DISASSOC_REQ, (STA_STATE_MACHINE_FUNC)StaInvalidStateWhenDisassociate);
	StaStateMachineSetAction(S, STA_ASSOC_WAIT_RSP, STA_MT2_PEER_DISASSOC_REQ, (STA_STATE_MACHINE_FUNC)StaPeerDisassocAction);
	StaStateMachineSetAction(S, STA_ASSOC_WAIT_RSP, STA_MT2_PEER_ASSOC_RSP, (STA_STATE_MACHINE_FUNC)StaPeerAssocRspAction);
	StaStateMachineSetAction(S, STA_ASSOC_WAIT_RSP, STA_MT2_ASSOC_TIMEOUT, (STA_STATE_MACHINE_FUNC)StaAssocTimeoutAction);

	// timer init
	RTMPInitTimer(pAd, &pAd->MlmeAux.ApCliAssocTimer, (PVOID)&StaAssocTimeout);

	for (i=0; i < MAX_APCLI_ENTRY; i++)
		pAd->ApCliTab.ApCliEntry[i].AssocCurrState = STA_ASSOC_IDLE;

	return;
}

/*
    ==========================================================================
    Description:
        Association timeout procedure. After association timeout, this function 
        will be called and it will put a message into the MLME queue
    Parameters:
        Standard timer parameters
    ==========================================================================
 */
VOID StaAssocTimeout(
    IN	unsigned long data) 
{
    RTMP_ADAPTER *pAd = (RTMP_ADAPTER *)data;
	DBGPRINT(RT_DEBUG_TRACE,"STA_ASSOC - enqueue STA_MT2_ASSOC_TIMEOUT \n");

	StaMlmeEnqueue(pAd, STA_ASSOC_STATE_MACHINE, STA_MT2_ASSOC_TIMEOUT, 0, NULL, 0);
    MlmeHandler(pAd);

	return;
}

/*
    ==========================================================================
    Description:
        mlme assoc req handling procedure
    Parameters:
        Adapter - Adapter pointer
        Elem - MLME Queue Element
    Pre:
        the station has been authenticated and the following information is stored in the config
            -# SSID
            -# supported rates and their length
            -# listen interval (Adapter->PortCfg.default_listen_count)
            -# Transmit power  (Adapter->PortCfg.tx_power)
    Post  :
        -# An association request frame is generated and sent to the air
        -# Association timer starts
        -# Association state -> ASSOC_WAIT_RSP
        
    ==========================================================================
 */
VOID StaMlmeAssocReqAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex) 
	
{
	UCHAR            ApAddr[6];
	HEADER_802_11    AssocHdr;
	UCHAR            WmeIe[9] = {IE_VENDOR_SPECIFIC, 0x07, 0x00, 0x50, 0xf2, 0x02, 0x00, 0x01, STA_QOS_CAPABILITY};
	USHORT           ListenIntv;
	ULONG            Timeout;
	USHORT           CapabilityInfo;
	PUCHAR           pOutBuffer = NULL;
	ULONG            FrameLen = 0;
	ULONG            tmp;
	UCHAR            SsidIe    = IE_SSID;
	UCHAR            SupRateIe = IE_SUPP_RATES;
	UCHAR            ExtRateIe = IE_EXT_SUPP_RATES;
	STA_CTRL_MSG_STRUCT StaCtrlMsg;
#ifdef APCLI_SUPPORT
	RT_802_11_APCLI_ENTRY *pApCliEntry;
#endif

	pApCliEntry = &pAd->ApCliTab.ApCliEntry[ifIndex];	

	// Block all authentication request durning WPA block period
	if (pApCliEntry->bBlockAssoc == TRUE)
	{
		DBGPRINT(RT_DEBUG_TRACE, "STA ASSOC - Block Auth request durning WPA block period!\n");
		*pCurrState = STA_ASSOC_IDLE;
		StaCtrlMsg.Status = MLME_STATE_MACHINE_REJECT;
		StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_ASSOC_RSP,
			sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);
	}
	else if(MlmeAssocReqSanity(pAd, Elem->Msg, Elem->MsgLen, ApAddr, &CapabilityInfo, &Timeout, &ListenIntv))
	{
		RTMPCancelTimer(&pAd->MlmeAux.ApCliAssocTimer);

		// allocate and send out AssocRsp frame
		pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
		if(pOutBuffer == NULL)
		{
			DBGPRINT(RT_DEBUG_TRACE,"STA_ASSOC - StaMlmeAssocReqAction() allocate memory failed \n");
			*pCurrState = STA_ASSOC_IDLE;

			StaCtrlMsg.Status = MLME_FAIL_NO_RESOURCE;
			StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_ASSOC_RSP,
				sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);

			return;
		}

		DBGPRINT(RT_DEBUG_TRACE, "STA_ASSOC - Send ASSOC request...\n");
		ApCliMgtMacHeaderInit(pAd, &AssocHdr, SUBTYPE_ASSOC_REQ, 0, ApAddr, ApAddr, ifIndex);

		// Build basic frame first
		MakeOutgoingFrame(pOutBuffer,               &FrameLen,
			sizeof(HEADER_802_11),    &AssocHdr,
			2,                        &CapabilityInfo,
			2,                        &ListenIntv,
			1,                        &SsidIe,
			1,                        &pAd->MlmeAux.SsidLen, 
			pAd->MlmeAux.SsidLen,     pAd->MlmeAux.Ssid,
			1,                        &SupRateIe,
			1,                        &pAd->MlmeAux.SupRateLen,
			pAd->MlmeAux.SupRateLen,  pAd->MlmeAux.SupRate,
			END_OF_ARGS);

		if(pAd->MlmeAux.ExtRateLen != 0)
		{
			MakeOutgoingFrame(pOutBuffer + FrameLen,    &tmp,
				1,                        &ExtRateIe,
				1,                        &pAd->MlmeAux.ExtRateLen,
				pAd->MlmeAux.ExtRateLen,  pAd->MlmeAux.ExtRate,                           
				END_OF_ARGS);
			FrameLen += tmp;
		}

		if(pAd->MlmeAux.APEdcaParm.bValid)
		{
			WmeIe[8] |= (pAd->MlmeAux.APEdcaParm.EdcaUpdateCount & 0x0f);
			MakeOutgoingFrame(pOutBuffer + FrameLen,    &tmp,
				9,                        &WmeIe[0],
				END_OF_ARGS);
			FrameLen += tmp;
		}

#ifdef APCLI_SUPPORT
		// Append RSN_IE when WPAPSK OR WPA2PSK,
		if (((pApCliEntry->AuthMode == Ndis802_11AuthModeWPAPSK) || 
            (pApCliEntry->AuthMode == Ndis802_11AuthModeWPA2PSK))
// For AP Client support WPS Modification
#ifdef WSC_SUPPORT
		&& (pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.WscConfMode == WSC_DISABLE)
#endif // WSC_SUPPORT //
// --
		)
		{
			UCHAR RSNIe = IE_WPA;

			// The group cipher suite is from the RSN IE in the beacon or probe response.
			// We use it to decide pApCliEntry->GroupCipher.
			if (pApCliEntry->AuthMode == Ndis802_11AuthModeWPA2PSK)
			{
				RSNIe = IE_WPA2;
				if (pApCliEntry->RSN_IE[2] == 0x00 && pApCliEntry->RSN_IE[3] == 0x0F &&
					pApCliEntry->RSN_IE[4] == 0xAC && pApCliEntry->RSN_IE[5] == 0x02)
					pApCliEntry->GroupCipher = Ndis802_11Encryption2Enabled;	// TKIP
				else if (pApCliEntry->RSN_IE[2] == 0x00 && pApCliEntry->RSN_IE[3] == 0x0F &&
					pApCliEntry->RSN_IE[4] == 0xAC && pApCliEntry->RSN_IE[5] == 0x04)
					pApCliEntry->GroupCipher = Ndis802_11Encryption3Enabled;	// AES
			}
			else
			{
				//RSNIe = IE_WPA;
				if (pApCliEntry->RSN_IE[6] == 0x00 && pApCliEntry->RSN_IE[7] == 0x50 &&
					pApCliEntry->RSN_IE[8] == 0xF2 && pApCliEntry->RSN_IE[9] == 0x02)
					pApCliEntry->GroupCipher = Ndis802_11Encryption2Enabled;	// TKIP
				else if (pApCliEntry->RSN_IE[6] == 0x00 && pApCliEntry->RSN_IE[7] == 0x50 &&
					pApCliEntry->RSN_IE[8] == 0xF2 && pApCliEntry->RSN_IE[9] == 0x04)
					pApCliEntry->GroupCipher = Ndis802_11Encryption3Enabled;	// AES
			}
			
			MakeOutgoingFrame(pOutBuffer + FrameLen,    			&tmp,
			              	1,                                      &RSNIe,
	                        1,                                      &pApCliEntry->RSN_IELen,
	                        pApCliEntry->RSN_IELen,					pApCliEntry->RSN_IE,
	                        END_OF_ARGS);
			
			FrameLen += tmp;	
		}	
#endif

#ifdef AGGREGATION_SUPPORT
		// add Ralink proprietary IE to inform AP this STA is going to use AGGREGATION or PIGGY-BACK+AGGREGATION
		// Case I: (Aggregation + Piggy-Back)
		// 1. user enable aggregation, AND
		// 2. Mac support piggy-back
		// 3. AP annouces it's PIGGY-BACK+AGGREGATION-capable in BEACON
		// Case II: (Aggregation)
		// 1. user enable aggregation, AND
		// 2. AP annouces it's AGGREGATION-capable in BEACON
		if(pAd->PortCfg.bAggregationCapable)
		{
			if(pAd->MlmeAux.APRalinkIe & 0x00000001)
			{
				ULONG TmpLen;
				UCHAR RalinkIe[9] = {IE_VENDOR_SPECIFIC, 7, 0x00, 0x0c, 0x43, 0x01, 0x00, 0x00, 0x00}; 
				MakeOutgoingFrame(pOutBuffer+FrameLen,           &TmpLen,
					9,                             RalinkIe,
					END_OF_ARGS);
				FrameLen += TmpLen;
			}
		}
#endif  /* AGGREGATION_SUPPORT */
// For AP Client support WPS Modification
#ifdef WSC_SUPPORT
		// Add WSC IE if we are connecting to WSC AP
		if (pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.WscState != WSC_STATE_OFF)
		{
			// WSC fixed IE for Version and Request type
			UCHAR	WscBuf[] = {0xdd, 0x0e, 0x00, 0x50, 0xf2, 0x04, 0x10, 0x4a, 0x00, 0x01, 0x10, 0x10, 0x3a, 0x00, 0x01, 0x01};

			MakeOutgoingFrame(pOutBuffer + FrameLen, &tmp,
						sizeof(WscBuf),				&WscBuf,
						END_OF_ARGS);
			FrameLen += tmp;
		}
#endif // WSC_SUPPORT //
// --
		MiniportMMRequest(pAd, pOutBuffer, FrameLen);
		kfree( pOutBuffer);

		RTMPAddTimer(&pAd->MlmeAux.ApCliAssocTimer, Timeout);
		*pCurrState = STA_ASSOC_WAIT_RSP;
	} 
	else
	{
		DBGPRINT(RT_DEBUG_TRACE,"STA_ASSOC - StaMlmeAssocReqAction() sanity check failed. BUG!!!!!! \n");
		*pCurrState = STA_ASSOC_IDLE;

		StaCtrlMsg.Status = MLME_INVALID_FORMAT;
		StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_ASSOC_RSP,
			sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);
	}

	return;
}

/*
    ==========================================================================
    Description:
        Upper layer issues disassoc request
    Parameters:
        Elem -
    ==========================================================================
 */
VOID StaMlmeDisassocReqAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex) 
{
	PMLME_DISASSOC_REQ_STRUCT pDisassocReq;
	HEADER_802_11         DisassocHdr;
	PCHAR                 pOutBuffer = NULL;
	ULONG                 FrameLen = 0;
#if defined(WPA_SUPPLICANT_SUPPORT) && (WPA_SUPPLICANT_SUPPORT == 1)
	union iwreq_data      wrqu;
#endif
	STA_CTRL_MSG_STRUCT StaCtrlMsg;

	// skip sanity check
	pDisassocReq = (PMLME_DISASSOC_REQ_STRUCT)(Elem->Msg);

	// allocate and send out DeassocReq frame
	pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
	if (pOutBuffer == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, "STA_ASSOC - StaMlmeDisassocReqAction() allocate memory failed\n");
		*pCurrState = STA_ASSOC_IDLE;

		StaCtrlMsg.Status = MLME_FAIL_NO_RESOURCE;
		StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_DEASSOC_RSP,
			sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);
		return;
    }

    DBGPRINT(RT_DEBUG_TRACE, "STA_ASSOC - Send DISASSOC request [BSSID::%02x:%02x:%02x:%02x:%02x:%02x] \n", 
				pDisassocReq->Addr[0], pDisassocReq->Addr[1], pDisassocReq->Addr[2],
				pDisassocReq->Addr[3], pDisassocReq->Addr[4], pDisassocReq->Addr[5]);
    ApCliMgtMacHeaderInit(pAd, &DisassocHdr, SUBTYPE_DISASSOC, 0, pDisassocReq->Addr, pDisassocReq->Addr, ifIndex);
    MakeOutgoingFrame(pOutBuffer,           &FrameLen, 
                      sizeof(HEADER_802_11),&DisassocHdr, 
                      2,                    &pDisassocReq->Reason, 
                      END_OF_ARGS);
    MiniportMMRequest(pAd, pOutBuffer, FrameLen);
	kfree(pOutBuffer);

	// Set the control aux SSID to prevent it reconnect to old SSID
	// Since calling this indicate user don't want to connect to that SSID anymore.
	// 2004-11-10 can't reset this info, cause it may be the new SSID that user requests for
	// pAd->MlmeAux.SsidLen = MAX_LEN_OF_SSID;
	// NdisZeroMemory(pAd->MlmeAux.Ssid, MAX_LEN_OF_SSID);
    // NdisZeroMemory(pAd->MlmeAux.Bssid, MAC_ADDR_LEN);
    
#if defined(WPA_SUPPLICANT_SUPPORT) && (WPA_SUPPLICANT_SUPPORT == 1)
    if (pAd->PortCfg.WPA_Supplicant == TRUE)
	{
       //send disassociate event to wpa_supplicant
       memset(&wrqu, 0, sizeof(wrqu));
       wrqu.data.flags = RT_DISASSOC_EVENT_FLAG;
       wireless_send_event(pAd->net_dev, IWEVCUSTOM, &wrqu, NULL);
    } 
#endif
    
    //pAd->PortCfg.DisassocReason = REASON_DISASSOC_STA_LEAVING;
    //COPY_MAC_ADDR(pAd->PortCfg.DisassocSta, pDisassocReq->Addr);


    *pCurrState = STA_ASSOC_IDLE;

	StaCtrlMsg.Status = MLME_SUCCESS;
	StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_DEASSOC_RSP,
		sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);

	return;
}


/*
    ==========================================================================
    Description:
        peer sends assoc rsp back
    Parameters:
        Elme - MLME message containing the received frame
    ==========================================================================
 */
VOID StaPeerAssocRspAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex) 
{
	USHORT        CapabilityInfo, Status, Aid;
	UCHAR         SupRate[MAX_LEN_OF_SUPPORTED_RATES], SupRateLen;
	UCHAR         ExtRate[MAX_LEN_OF_SUPPORTED_RATES], ExtRateLen;
	UCHAR         Addr2[MAC_ADDR_LEN];
	EDCA_PARM     EdcaParm;
#if defined(WPA_SUPPLICANT_SUPPORT) && (WPA_SUPPLICANT_SUPPORT == 1)
	union iwreq_data wrqu;
#endif
	STA_CTRL_MSG_STRUCT StaCtrlMsg;


	if (PeerAssocRspSanity(pAd, Elem->Msg, Elem->MsgLen, Addr2, &CapabilityInfo, &Status, &Aid, SupRate, &SupRateLen, ExtRate, &ExtRateLen, &EdcaParm))
	{
		// The frame is for me ?
		if(MAC_ADDR_EQUAL(Addr2, pAd->MlmeAux.Bssid))
		{
			DBGPRINT(RT_DEBUG_TRACE, "STA_ASSOC - receive ASSOC_RSP to me (status=%d)\n", Status);
			RTMPCancelTimer(&pAd->MlmeAux.ApCliAssocTimer);
			if(Status == MLME_SUCCESS) 
			{
				// go to procedure listed on page 376
				AssocPostProc(pAd, Addr2, CapabilityInfo, Aid, SupRate, SupRateLen, ExtRate, ExtRateLen, &EdcaParm);  	

#if defined(WPA_SUPPLICANT_SUPPORT) && (WPA_SUPPLICANT_SUPPORT == 1)
				if (pAd->PortCfg.WPA_Supplicant == TRUE)
				{
					// collect associate info 
					link_status_handler(pAd);
					//send associnfo event to wpa_supplicant
					memset(&wrqu, 0, sizeof(wrqu));
					wrqu.data.flags = RT_ASSOC_EVENT_FLAG;
					wireless_send_event(pAd->net_dev, IWEVCUSTOM, &wrqu, NULL);
				}
#endif
				StaCtrlMsg.Status = MLME_SUCCESS;
				StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_ASSOC_RSP,
					sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);
			}
			else
			{
				StaCtrlMsg.Status = Status;
				StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_ASSOC_RSP,
     				sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);
			}
			*pCurrState = STA_ASSOC_IDLE;
		}
	}
	else
	{
		DBGPRINT(RT_DEBUG_TRACE, "STA_ASSOC - StaPeerAssocRspAction() sanity check fail\n");
	}

	return;
}

/*
    ==========================================================================
    Description:
        left part of IEEE 802.11/1999 p.374 
    Parameters:
        Elem - MLME message containing the received frame
    ==========================================================================
 */
VOID StaPeerDisassocAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex) 
{
	UCHAR         Addr2[MAC_ADDR_LEN];
	USHORT        Reason;
#if defined(WPA_SUPPLICANT_SUPPORT) && (WPA_SUPPLICANT_SUPPORT == 1)
	union iwreq_data wrqu;
#endif

	if(PeerDisassocSanity(pAd, Elem->Msg, Elem->MsgLen, Addr2, &Reason))
	{
		if (MAC_ADDR_EQUAL(pAd->MlmeAux.Bssid, Addr2))
		{
			*pCurrState = STA_ASSOC_IDLE;

			StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_PEER_DISCONNECT_REQ, 0, NULL, ifIndex);

#if defined(WPA_SUPPLICANT_SUPPORT) && (WPA_SUPPLICANT_SUPPORT == 1)
			if (pAd->PortCfg.WPA_Supplicant == TRUE) {
               // send disassoc event to wpa_supplicant 
               memset(&wrqu, 0, sizeof(wrqu));
               wrqu.data.flags = RT_DISASSOC_EVENT_FLAG;
               wireless_send_event(pAd->net_dev, IWEVCUSTOM, &wrqu, NULL);
            }
#endif

#if 0
            // 2004-09-11 john: can't remember why AP will DISASSOCIATE us.
            //   But since it says for 2430 only, we temporaily remove the patch.
            // 2002/11/21 -
            //   patch RT2430/RT2420 hangup issue. We suspect this AP DIS-ASSOCIATE frame
            //   is caused by PHY hangup, so we reset PHY, then auto recover the connection.
            //   if this attempt fails, then remains in LinkDown and leaves the problem
            //   to MlmePeriodicExec()
            // NICPatchRT2430Bug(pAd);
            pAd->RalinkCounters.BeenDisassociatedCount ++;
			// Remove auto recover effort when disassociate by AP, re-enable for patch 2430 only
            DBGPRINT(RT_DEBUG_TRACE, "ASSOC - Disassociated by AP, Auto Recovery attempt #%ld\n", pAd->RalinkCounters.BeenDisassociatedCount);
            MlmeAutoReconnectLastSSID(pAd);
#endif
        }
    }
    else
    {
        DBGPRINT(RT_DEBUG_TRACE, "STA_ASSOC - StaPeerDisassocAction() sanity check fail\n");
    }
	
	return;
}

/*
    ==========================================================================
    Description:
        what the state machine will do after assoc timeout
    ==========================================================================
 */
VOID StaAssocTimeoutAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex) 
{
	DBGPRINT(RT_DEBUG_TRACE, "STA_ASSOC - StaAssocTimeoutAction\n");
	*pCurrState = STA_ASSOC_IDLE;

	StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_ASSOC_REQ_TIMEOUT, 0, NULL, ifIndex);

	return;
}

VOID StaInvalidStateWhenAssoc(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex) 
{
	STA_CTRL_MSG_STRUCT StaCtrlMsg;

	DBGPRINT(RT_DEBUG_TRACE, "STA_ASSOC - StaInvalidStateWhenAssoc(state=%d), reset STA_ASSOC state machine\n", 
			*pCurrState);
	*pCurrState = STA_ASSOC_IDLE;

	StaCtrlMsg.Status = MLME_STATE_MACHINE_REJECT;
	StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_ASSOC_RSP,
		sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);

	return;
}

VOID StaInvalidStateWhenDisassociate(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex) 
{
	STA_CTRL_MSG_STRUCT StaCtrlMsg;

	DBGPRINT(RT_DEBUG_TRACE, "STA_ASSOC - InvalidStateWhenApCliDisassoc(state=%d), reset STA_ASSOC state machine\n", 
		*pCurrState);
	*pCurrState = STA_ASSOC_IDLE;

	StaCtrlMsg.Status = MLME_STATE_MACHINE_REJECT;
	StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_DEASSOC_RSP,
		sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);

	return;
}

/*
    ==========================================================================
    Description:
        procedures on IEEE 802.11/1999 p.376 
    Parametrs:
    ==========================================================================
 */
VOID AssocPostProc(
	IN PRTMP_ADAPTER pAd, 
	IN PUCHAR pAddr2, 
	IN USHORT CapabilityInfo, 
	IN USHORT Aid, 
	IN UCHAR SupRate[], 
	IN UCHAR SupRateLen,
	IN UCHAR ExtRate[],
	IN UCHAR ExtRateLen,
	IN PEDCA_PARM pEdcaParm)
{

	pAd->MlmeAux.BssType = BSS_INFRA;
	pAd->MlmeAux.Aid = Aid;
	pAd->MlmeAux.CapabilityInfo = CapabilityInfo & SUPPORTED_CAPABILITY_INFO;
	NdisMoveMemory(&pAd->MlmeAux.APEdcaParm, pEdcaParm, sizeof(EDCA_PARM));

	// filter out un-supported rates
	pAd->MlmeAux.SupRateLen = SupRateLen;
	NdisMoveMemory(pAd->MlmeAux.SupRate, SupRate, SupRateLen);
    RTMPCheckRates(pAd, pAd->MlmeAux.SupRate, &pAd->MlmeAux.SupRateLen);

	// filter out un-supported rates
	pAd->MlmeAux.ExtRateLen = ExtRateLen;
	NdisMoveMemory(pAd->MlmeAux.ExtRate, ExtRate, ExtRateLen);
    RTMPCheckRates(pAd, pAd->MlmeAux.ExtRate, &pAd->MlmeAux.ExtRateLen);

}

