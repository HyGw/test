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
    auth_rsp.c
 
    Abstract:
    Handle auth/de-auth requests from WSTA
 
    Revision History:
    Who         When          What
    --------    ----------    ----------------------------------------------
    John Chang  08-04-2003    created for 11g soft-AP
 */

#include "rt_config.h"

/*
    ==========================================================================
    Description:
        authentication state machine init procedure
    Parameters:
        Sm - the state machine
    Note:
        the state machine looks like the following 
        
                                        AUTH_RSP_IDLE                   
    MT2_AUTH_CHALLENGE_TIMEOUT      auth_rsp_challenge_timeout_action    
    MT2_PEER_AUTH_ODD               peer_auth_at_auth_rsp_idle_action 
    MT2_PEER_DEAUTH                 peer_deauth_action         
    ==========================================================================
 */
VOID AuthRspStateMachineInit(
    IN PRTMP_ADAPTER pAd, 
    IN PSTATE_MACHINE Sm, 
    IN STATE_MACHINE_FUNC Trans[]) 
{
    UMAXLONG        NOW;

    StateMachineInit(Sm, (STATE_MACHINE_FUNC*)Trans, MAX_AUTH_RSP_STATE, MAX_AUTH_RSP_MSG, (STATE_MACHINE_FUNC)Drop, AUTH_RSP_IDLE, AUTH_RSP_MACHINE_BASE);

    // column 1
    StateMachineSetAction(Sm, AUTH_RSP_IDLE, MT2_PEER_AUTH_ODD, (STATE_MACHINE_FUNC)PeerAuthAtAuthRspIdleAction);
    StateMachineSetAction(Sm, AUTH_RSP_IDLE, MT2_PEER_DEAUTH, (STATE_MACHINE_FUNC)PeerDeauthReqAction);

    // initialize the random number generator
    NOW = jiffies;
    LfsrInit(pAd, NOW);
}

/*
    ==========================================================================
    Description:
        Process the received Authnetication frame from client
    ==========================================================================
*/
VOID PeerAuthAtAuthRspIdleAction(
    IN PRTMP_ADAPTER pAd, 
    IN PMLME_QUEUE_ELEM Elem) 
{
    USHORT          Seq, Alg, RspReason, i, Status;
    UCHAR           Addr1[MAC_ADDR_LEN], Addr2[MAC_ADDR_LEN];
    PHEADER_802_11  pRcvHdr;
    HEADER_802_11   AuthHdr;
    CHAR            Chtxt[CIPHER_TEXT_LEN];
    CHAR            *OutBuffer = NULL;
    ULONG            FrameLen = 0;
    MAC_TABLE_ENTRY *pEntry;
    UCHAR           ChTxtIe = 16, ChTxtLen = CIPHER_TEXT_LEN, apidx = MAIN_MBSSID;

    if (! PeerAuthSanity(pAd, Elem->Msg, Elem->MsgLen, Addr1, Addr2, &Alg, &Seq, &Status, Chtxt)) 
        return;
    
    // Search which MBSSID to be authenticate
    for (i=0; i<pAd->PortCfg.BssidNum; i++)
	{
		if (i == pAd->PortCfg.BssidNum)
	    {
	    	DBGPRINT(RT_DEBUG_TRACE, "AUTH_RSP - Bssid not found\n");
	    	return;
	    }

		if(RTMPEqualMemory(Addr1, pAd->PortCfg.MBSSID[i].Bssid, MAC_ADDR_LEN))
		{
			apidx = i;
			break;
		}
	}

    pEntry = MacTableLookup(pAd, Addr2);
    
    pRcvHdr = (PHEADER_802_11)(Elem->Msg);
    DBGPRINT(RT_DEBUG_TRACE, "AUTH_RSP-Rcv AUTH seq#%d,Alg=%d,Status=%d from %02x:%02x:%02x:%02x:%02x:%02x to IF(ra%d)\n",
        Seq,Alg,Status,Addr2[0],Addr2[1],Addr2[2],Addr2[3],Addr2[4],Addr2[5],apidx);
    if (Seq == 1) 
    {
        if ((Alg == AUTH_MODE_OPEN) && 
            (pAd->PortCfg.MBSSID[apidx].AuthMode != Ndis802_11AuthModeShared)) 
        {
            if (!pEntry)
                pEntry = MacTableInsertEntry(pAd, Addr2, apidx);

            if (pEntry)
            {
            	pEntry->ApIdx = apidx;
                pEntry->AuthState = AS_AUTH_OPEN;
                pEntry->Sst = SST_AUTH; // what if it already in SST_ASSOC ???????
                if (pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeAutoSwitch)
                	pEntry->AuthMode = Ndis802_11AuthModeShared;
                PeerAuthSimpleRspGenAndSend(pAd, pRcvHdr, Alg, Seq + 1, MLME_SUCCESS, apidx);
            }
            else
                ; // MAC table full, what should we respond ?????
        } 
        else if ((Alg == AUTH_MODE_KEY) && 
            ((pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeShared) || (pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeAutoSwitch))) 
        {
            if (!pEntry)
                pEntry = MacTableInsertEntry(pAd, Addr2, apidx);

            if (pEntry)
            {
            	pEntry->ApIdx = apidx;
                pEntry->AuthState = AS_AUTHENTICATING;
                pEntry->Sst = SST_NOT_AUTH; // what if it already in SST_ASSOC ???????

                if (pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeAutoSwitch)
                	pEntry->AuthMode = Ndis802_11AuthModeShared;

                // log this STA in AuthRspAux machine, only one STA is stored. If two STAs using
                // SHARED_KEY authentication mingled together, then the late comer will win.
                COPY_MAC_ADDR(pAd->Mlme.AuthRspAux.Addr, Addr2);
                for(i = 0; i < CIPHER_TEXT_LEN; i++) 
                    pAd->Mlme.AuthRspAux.Challenge[i] = RandomByte(pAd);

                RspReason = 0;
                Seq++;
  
                OutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
                if(OutBuffer == NULL)
                	return;

                DBGPRINT(RT_DEBUG_TRACE, "AUTH_RSP - Send AUTH seq#2 (Challenge)\n");
		        MgtMacHeaderInit(pAd, &AuthHdr, SUBTYPE_AUTH, 0, Addr2, pAd->PortCfg.MBSSID[apidx].Bssid);
		        MakeOutgoingFrame(OutBuffer,                &FrameLen, 
		                          sizeof(HEADER_802_11),    &AuthHdr, 
		                          2,                        &Alg, 
		                          2,                        &Seq, 
		                          2,                        &RspReason,
		                          1,                        &ChTxtIe,
                                  1,                        &ChTxtLen,
                                  ChTxtLen,                 pAd->Mlme.AuthRspAux.Challenge,
		                          END_OF_ARGS);
		        MiniportMMRequest(pAd, OutBuffer, FrameLen);
		        kfree(OutBuffer);
            }
            else
                ; // MAC table full, what should we respond ????
        } 
        else 
        {
            // wrong algorithm
            PeerAuthSimpleRspGenAndSend(pAd, pRcvHdr, Alg, Seq + 1, MLME_ALG_NOT_SUPPORT, apidx);
            DBGPRINT(RT_DEBUG_TRACE, "AUTH_RSP - IF(ra%d) Alg=%d, Seq=%d, AuthMode=%d\n",
                apidx, Alg, Seq, pAd->PortCfg.MBSSID[apidx].AuthMode);
        }
    } 
    else if (Seq == 3)
    {
        if (pEntry && MAC_ADDR_EQUAL(Addr2, &(pAd->Mlme.AuthRspAux.Addr))) 
        {
            if ((pRcvHdr->FC.Wep == 1) && RTMPEqualMemory(Chtxt, pAd->Mlme.AuthRspAux.Challenge, CIPHER_TEXT_LEN)) 
            {
                // Successful
                PeerAuthSimpleRspGenAndSend(pAd, pRcvHdr, Alg, Seq + 1, MLME_SUCCESS, apidx);
                pEntry->AuthState = AS_AUTH_KEY;
                pEntry->Sst = SST_AUTH;
            } 
            else 
            {
                // fail - wep bit is not set or challenge text is not equal
                PeerAuthSimpleRspGenAndSend(pAd, pRcvHdr, Alg, Seq + 1, MLME_REJ_CHALLENGE_FAILURE, apidx);
                MacTableDeleteEntry(pAd, pEntry->Addr);
                Chtxt[127]='\0';
                pAd->Mlme.AuthRspAux.Challenge[127]='\0';
                DBGPRINT(RT_DEBUG_TRACE, "pRcvHdr->Wep= %d\n",pRcvHdr->FC.Wep);
                DBGPRINT(RT_DEBUG_TRACE, "Sent Challenge = %s\n",&pAd->Mlme.AuthRspAux.Challenge[100]);
                DBGPRINT(RT_DEBUG_TRACE, "Rcv Challenge = %s\n",&Chtxt[100]);
            }
        } 
        else 
        {
            // fail for unknown reason. most likely is AuthRspAux machine be overwritten by another
            // STA also using SHARED_KEY authentication
            PeerAuthSimpleRspGenAndSend(pAd, pRcvHdr, Alg, Seq + 1, MLME_UNSPECIFY_FAIL, apidx);
        }
    }
    else 
    {
        // fail - wrong sequence number
        PeerAuthSimpleRspGenAndSend(pAd, pRcvHdr, Alg, Seq + 1, MLME_SEQ_NR_OUT_OF_SEQUENCE, apidx);
    }
}

/*
    ==========================================================================
    Description:
        Process De-authentication request frame received from client
    ==========================================================================
*/
VOID PeerDeauthReqAction(
    IN PRTMP_ADAPTER pAd, 
    IN PMLME_QUEUE_ELEM Elem) 
{
    UCHAR			Addr2[MAC_ADDR_LEN];
    USHORT          Reason;
    MAC_TABLE_ENTRY *pEntry;

    if (! PeerDeauthReqSanity(pAd, Elem->Msg, Elem->MsgLen, Addr2, &Reason)) 
        return;
    
    pEntry = MacTableLookup(pAd, Addr2);
    if (pEntry)
    {
        if (pEntry->CMTimerRunning == TRUE)
        {
            // if one who initilized Counter Measure deauth itself,  AP doesn't log the MICFailTime
            pAd->PortCfg.aMICFailTime = pAd->PortCfg.PrevaMICFailTime;
        }
		//notify 802.1x-deamon to discard the sta
		RTMPHandleNotify8021xDiscardSta(pAd, pEntry);

        ApLogEvent(pAd, Addr2, EVENT_DISASSOCIATED, pEntry->ApIdx);
        MacTableDeleteEntry(pAd, Addr2);
        DBGPRINT(RT_DEBUG_TRACE,"AUTH_RSP-IF(ra%d) rcv DE-AUTH. delete MAC entry %02x:%02x:%02x:%02x:%02x:%02x\n",
            pEntry->ApIdx,Addr2[0],Addr2[1],Addr2[2],Addr2[3],Addr2[4],Addr2[5] );
    }
}

/*
    ==========================================================================
    Description:
        Send out a Authentication (response) frame
    ==========================================================================
*/
VOID PeerAuthSimpleRspGenAndSend(
    IN PRTMP_ADAPTER pAd, 
    IN PHEADER_802_11 Hdr, 
    IN USHORT Alg, 
    IN USHORT Seq, 
    IN USHORT StatusCode,
    IN UCHAR apidx) 
{
    HEADER_802_11     AuthHdr;
    ULONG              FrameLen = 0;
    UCHAR             *OutBuffer = NULL;

    OutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
    if(OutBuffer == NULL)
        return;

    if (StatusCode == MLME_SUCCESS)
    {
        DBGPRINT(RT_DEBUG_TRACE, "AUTH_RSP - IF(%d) Send AUTH response (SUCCESS)...\n", apidx);
        MgtMacHeaderInit(pAd, &AuthHdr, SUBTYPE_AUTH, 0, Hdr->Addr2, pAd->PortCfg.MBSSID[apidx].Bssid);
        MakeOutgoingFrame(OutBuffer,                &FrameLen, 
                          sizeof(HEADER_802_11),    &AuthHdr, 
                          2,                        &Alg, 
                          2,                        &Seq, 
                          2,                        &StatusCode, 
                          END_OF_ARGS);
        MiniportMMRequest(pAd, OutBuffer, FrameLen);
    }
    else
    {
        // For MAC wireless client(Macintosh), need to send AUTH_RSP with Status Code (fail reason code) to reject it.
        MgtMacHeaderInit(pAd, &AuthHdr, SUBTYPE_AUTH, 0, Hdr->Addr2, pAd->PortCfg.MBSSID[apidx].Bssid);
        MakeOutgoingFrame(OutBuffer,                &FrameLen, 
                          sizeof(HEADER_802_11),    &AuthHdr, 
                          2,                        &Alg, 
                          2,                        &Seq, 
                          2,                        &StatusCode, 
                          END_OF_ARGS);
        MiniportMMRequest(pAd, OutBuffer, FrameLen);
        DBGPRINT(RT_DEBUG_TRACE, "AUTH_RSP - Peer AUTH fail (Status = %d)...\n", StatusCode);
    }

    kfree(OutBuffer);
}

