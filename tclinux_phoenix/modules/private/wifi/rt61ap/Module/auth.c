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
    auth.c
 
    Abstract:
    Handle de-auth request from local MLME
 
    Revision History:
    Who         When          What
    --------    ----------    ----------------------------------------------
    John Chang  08-04-2003    created for 11g soft-AP
 */

#include "rt_config.h"

/*
    ==========================================================================
    Description:
        authenticate state machine init, including state transition and timer init
    Parameters:
        Sm - pointer to the auth state machine
    Note:
        The state machine looks like this
        
                                    AUTH_REQ_IDLE           
        MT2_MLME_DEAUTH_REQ     mlme_deauth_req_action  
    ==========================================================================
 */
void AuthStateMachineInit(
    IN PRTMP_ADAPTER pAd, 
    IN STATE_MACHINE *Sm, 
    OUT STATE_MACHINE_FUNC Trans[]) 
{
    StateMachineInit(Sm, (STATE_MACHINE_FUNC*)Trans, MAX_AUTH_STATE, MAX_AUTH_MSG, (STATE_MACHINE_FUNC)Drop, AUTH_REQ_IDLE, AUTH_MACHINE_BASE);
     
    // the first column
    StateMachineSetAction(Sm, AUTH_REQ_IDLE, MT2_MLME_DEAUTH_REQ, (STATE_MACHINE_FUNC)MlmeDeauthReqAction);
}

/*
    ==========================================================================
    Description:
        Upper Layer request to kick out a STA
    ==========================================================================
 */
VOID MlmeDeauthReqAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem) 
{
    MLME_DEAUTH_REQ_STRUCT  *pInfo;
    HEADER_802_11           Hdr;
    UCHAR                   *OutBuffer = NULL;
    ULONG                   FrameLen = 0;
    MAC_TABLE_ENTRY         *pEntry;

    pInfo = (MLME_DEAUTH_REQ_STRUCT *)Elem->Msg;

    pEntry = MacTableLookup(pAd, pInfo->Addr);

    if (pEntry)
    {
        // remove this STA from MAC table
        ApLogEvent(pAd, pInfo->Addr, EVENT_DISASSOCIATED, pEntry->ApIdx);
        MacTableDeleteEntry(pAd, pInfo->Addr);

        // send out DE-AUTH request frame
        OutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
        if(OutBuffer == NULL)
        	return;
        
        DBGPRINT(RT_DEBUG_TRACE, "AUTH - Send DE-AUTH req to %02x:%02x:%02x:%02x:%02x:%02x\n",
            pInfo->Addr[0],pInfo->Addr[1],pInfo->Addr[2],pInfo->Addr[3],pInfo->Addr[4],pInfo->Addr[5]);
        MgtMacHeaderInit(pAd, &Hdr, SUBTYPE_DEAUTH, 0, pInfo->Addr, pAd->PortCfg.MBSSID[pEntry->ApIdx].Bssid);
        MakeOutgoingFrame(OutBuffer,              &FrameLen, 
                          sizeof(HEADER_802_11),  &Hdr, 
                          2,                      &pInfo->Reason, 
                          END_OF_ARGS);
        MiniportMMRequest(pAd, OutBuffer, FrameLen);
        kfree(OutBuffer);
    }
}

/*
    ==========================================================================
    Description:
        When class 2 error occuring, call this function to deauth client.
    Note:
        This action should never trigger AUTH state transition, therefore we
        separate it from AUTH state machine, and make it as a standalone service
    ==========================================================================
 */
VOID Cls2errAction(
    IN PRTMP_ADAPTER	pAd, 
    IN PUCHAR			pAddr1,
    IN PUCHAR			pAddr2)
{
    HEADER_802_11   Hdr;
    UCHAR           *OutBuffer = NULL;
    ULONG           FrameLen = 0;
    USHORT          Reason = REASON_CLS2ERR;
    MAC_TABLE_ENTRY *pEntry;

    pEntry = MacTableLookup(pAd, pAddr2);
    if (pEntry)
    {
        //ApLogEvent(pAd, pAddr, EVENT_DISASSOCIATED, pEntry->ApIdx);
        MacTableDeleteEntry(pAd, pAddr2);
    }

    // send out DEAUTH request frame 
    OutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
    if (OutBuffer == NULL)
        return;

    DBGPRINT(RT_DEBUG_TRACE, "AUTH - Class 2 error, Send DEAUTH frame to %02x:%02x:%02x:%02x:%02x:%02x\n",
        pAddr2[0],pAddr2[1],pAddr2[2],pAddr2[3],pAddr2[4],pAddr2[5]);
    MgtMacHeaderInit(pAd, &Hdr, SUBTYPE_DEAUTH, 0, pAddr2, pAddr1);
    MakeOutgoingFrame(OutBuffer,				&FrameLen, 
                      sizeof(HEADER_802_11),	&Hdr, 
                      2,							&Reason, 
                      END_OF_ARGS);
    MiniportMMRequest(pAd, OutBuffer, FrameLen);
    kfree(OutBuffer);
}

