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
    dls.c
 
    Abstract:
    Handle WMM-DLS state machine
 
    Revision History:
    Who         When          What
    --------    ----------    ----------------------------------------------
    Rory Chen   02-14-2006
 */

#include "rt_config.h"

/*  
    ==========================================================================
    Description: 
        DLS state machine init, including state transition and timer init
    Parameters: 
        S - pointer to the DLS state machine
    Note:
        The state machine looks like the following 
        
                                    DLS_IDLE             
        MT2_PEER_DLS_REQ			PeerDlsReqAction 
        MT2_PEER_DLS_RSP			PeerDlsRspAction     
        MT2_PEER_DLS_TEAR_DOWN		PeerDlsTearDownAction
        
    ==========================================================================
 */
VOID DLSStateMachineInit(
    IN	PRTMP_ADAPTER	pAd, 
    IN  STATE_MACHINE *S, 
    OUT STATE_MACHINE_FUNC Trans[]) 
{
    StateMachineInit(S, (STATE_MACHINE_FUNC*)Trans, MAX_DLS_STATE, MAX_DLS_MSG, (STATE_MACHINE_FUNC)Drop, DLS_IDLE, DLS_MACHINE_BASE);

    StateMachineSetAction(S, DLS_IDLE, MT2_PEER_DLS_REQ, (STATE_MACHINE_FUNC)PeerDlsReqAction);
    StateMachineSetAction(S, DLS_IDLE, MT2_PEER_DLS_RSP, (STATE_MACHINE_FUNC)PeerDlsRspAction);
    StateMachineSetAction(S, DLS_IDLE, MT2_PEER_DLS_TEAR_DOWN, (STATE_MACHINE_FUNC)PeerDlsTearDownAction);
}

VOID PeerDlsReqAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem) 
{
	PMAC_TABLE_ENTRY	pDAEntry, pSAEntry;
	UCHAR				DA[MAC_ADDR_LEN], SA[MAC_ADDR_LEN];
	USHORT				CapabilityInfo;
	USHORT				DLSTimeout;
	PUCHAR				pOutBuffer = NULL;
	PFRAME_802_11		Fr;
	USHORT				Status;
	ULONG				FrameLen = 0;
	HEADER_802_11		DlsRspHdr;
	UCHAR				Category = CATEGORY_DLS;
	UCHAR				Action = ACTION_DLS_RESPONSE;

	// frame sanity check
    if (! PeerDlsReqSanity(pAd, Elem->Msg, Elem->MsgLen, DA, SA, &CapabilityInfo, &DLSTimeout)) 
        return;
	
	pSAEntry = MacTableLookup(pAd, SA);
	if (!pSAEntry)
		return;

	if (pSAEntry)
		pSAEntry->bDlsInit	= FALSE;

	pDAEntry = MacTableLookup(pAd, DA);
	if (!pDAEntry)
		Status = MLME_DEST_STA_NOT_IN_QBSS;
	else if (pDAEntry && (pDAEntry->ApIdx != pSAEntry->ApIdx))
		Status = MLME_DEST_STA_NOT_IN_QBSS;
	else if (pDAEntry && !CLIENT_STATUS_TEST_FLAG(pDAEntry, fCLIENT_STATUS_WMM_CAPABLE))
		Status = MLME_DEST_STA_IS_NOT_A_QSTA;
	else if (pDAEntry->WepStatus != pSAEntry->WepStatus)
		Status = MLME_QOS_UNSPECIFY;
	else if (!pAd->PortCfg.MBSSID[pSAEntry->ApIdx].bDLSCapable)
		Status = MLME_DLS_NOT_ALLOW_IN_QBSS;
	else
		Status = MLME_SUCCESS;

	if (pDAEntry)
		pDAEntry->bDlsInit	= FALSE;

    Fr = (PFRAME_802_11)Elem->Msg;

	// forward DLS-Request to real destination
	pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
	if(pOutBuffer == NULL)
		return;

	// if status is successful, forward DLS-Request frame to destination
	// otherwise send DLS-Response with reason code to originator
	if (Status == MLME_SUCCESS)
	{
		NdisMoveMemory(Fr->Hdr.Addr1, DA, MAC_ADDR_LEN);
		NdisMoveMemory(Fr->Hdr.Addr2, pAd->PortCfg.MBSSID[pSAEntry->ApIdx].Bssid, MAC_ADDR_LEN);
		NdisMoveMemory(Fr->Hdr.Addr3, SA, MAC_ADDR_LEN);
		NdisMoveMemory(pOutBuffer, Elem->Msg, Elem->MsgLen);
		FrameLen = Elem->MsgLen;
	}
	else
	{
		MgtMacHeaderInit(pAd, &DlsRspHdr, SUBTYPE_ACTION, 0, SA, pAd->PortCfg.MBSSID[pSAEntry->ApIdx].Bssid);

		// Capability information and supported rate field are present only when status code is zero	        
        MakeOutgoingFrame(pOutBuffer,                       &FrameLen,
                          sizeof(HEADER_802_11),            &DlsRspHdr,
                          1,                                &Category,
                          1,                                &Action,
                          2,                                &Status,
                          6,                                SA,
                          6,                                DA,
                          END_OF_ARGS);
	}

	MiniportMMRequest(pAd, pOutBuffer, FrameLen);
	kfree(pOutBuffer);

	DBGPRINT(RT_DEBUG_TRACE,"DLS - PeerDlsReqAction() from %02x:%02x:%02x:%02x:%02x:%02x with Status=%d\n", SA[0], SA[1], SA[2], SA[3], SA[4], SA[5], Status);
}

VOID PeerDlsRspAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem) 
{
	PMAC_TABLE_ENTRY	pEntry;
	UCHAR				DA[MAC_ADDR_LEN], SA[MAC_ADDR_LEN];
	USHORT				CapabilityInfo;
	USHORT				StatusCode;
	PUCHAR				pOutBuffer = NULL;
	PFRAME_802_11		Fr;
	ULONG				FrameLen = 0;

	// frame sanity check
    if (! PeerDlsRspSanity(pAd, Elem->Msg, Elem->MsgLen, DA, SA, &CapabilityInfo, &StatusCode)) 
        return;

    DBGPRINT(RT_DEBUG_TRACE,"DLS - PeerDlsRspAction() from %02x:%02x:%02x:%02x:%02x:%02x with StatusCode=%d\n",
    	SA[0], SA[1], SA[2], SA[3], SA[4], SA[5], StatusCode);
	
	pEntry = MacTableLookup(pAd, SA);
    if (!pEntry)
    	return;

	if (pEntry)
		pEntry->bDlsInit	= FALSE;

    Fr = (PFRAME_802_11)Elem->Msg;

    // forward DLS-Request to real destination
	pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
	if(pOutBuffer == NULL)
		return;

	NdisMoveMemory(Fr->Hdr.Addr1, DA, MAC_ADDR_LEN);
	NdisMoveMemory(Fr->Hdr.Addr2, pAd->PortCfg.MBSSID[pEntry->ApIdx].Bssid, MAC_ADDR_LEN);
	NdisMoveMemory(Fr->Hdr.Addr3, SA, MAC_ADDR_LEN);

	NdisMoveMemory(pOutBuffer, Elem->Msg, Elem->MsgLen);
	FrameLen = Elem->MsgLen;

	MiniportMMRequest(pAd, pOutBuffer, FrameLen);
	kfree(pOutBuffer);
}

VOID PeerDlsTearDownAction(
    IN PRTMP_ADAPTER pAd, 
    IN MLME_QUEUE_ELEM *Elem) 
{
	PMAC_TABLE_ENTRY	pEntry;
	UCHAR				DA[MAC_ADDR_LEN], SA[MAC_ADDR_LEN];
	USHORT				ReasonCode;
	PUCHAR				pOutBuffer = NULL;
	PFRAME_802_11		Fr;
	ULONG				FrameLen = 0;

	// frame sanity check
    if (! PeerDlsTearDownSanity(pAd, Elem->Msg, Elem->MsgLen, DA, SA, &ReasonCode)) 
        return;

    DBGPRINT(RT_DEBUG_TRACE,"DLS - PeerDlsTearDownAction() from %02x:%02x:%02x:%02x:%02x:%02x\n", SA[0], SA[1], SA[2], SA[3], SA[4], SA[5]);
	
	pEntry = MacTableLookup(pAd, SA);
    if (!pEntry)
    	return;

	pEntry->bDlsInit	= FALSE;

    Fr = (PFRAME_802_11)Elem->Msg;

    // forward DLS-Request to real destination
	pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
	if(pOutBuffer == NULL)
		return;

	NdisMoveMemory(Fr->Hdr.Addr1, DA, MAC_ADDR_LEN);
	NdisMoveMemory(Fr->Hdr.Addr2, pAd->PortCfg.MBSSID[pEntry->ApIdx].Bssid, MAC_ADDR_LEN);
	NdisMoveMemory(Fr->Hdr.Addr3, SA, MAC_ADDR_LEN);

	NdisMoveMemory(pOutBuffer, Elem->Msg, Elem->MsgLen);
	FrameLen = Elem->MsgLen;

	MiniportMMRequest(pAd, pOutBuffer, FrameLen);
	kfree(pOutBuffer);
}

