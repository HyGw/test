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
	sta_auth.c

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Fonchi		2006-6-23		modified for rt61-APClinent
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
	==========================================================================
 */

VOID StaAuthStateMachineInit(
	IN PRTMP_ADAPTER pAd,
	IN STA_STATE_MACHINE *Sm,
	OUT STA_STATE_MACHINE_FUNC Trans[])
{
	UCHAR i;

	StaStateMachineInit(Sm, (STA_STATE_MACHINE_FUNC*)Trans, STA_MAX_AUTH_STATE, STA_MAX_AUTH_MSG, (STA_STATE_MACHINE_FUNC)StaDrop, STA_AUTH_REQ_IDLE, STA_AUTH_MACHINE_BASE);

	// the first column
	StaStateMachineSetAction(Sm, STA_AUTH_REQ_IDLE, STA_MT2_MLME_AUTH_REQ, (STA_STATE_MACHINE_FUNC)StaMlmeAuthReqAction);
	StaStateMachineSetAction(Sm, STA_AUTH_REQ_IDLE, STA_MT2_PEER_DEAUTH, (STA_STATE_MACHINE_FUNC)StaPeerDeauthAction);

	// the second column
	StaStateMachineSetAction(Sm, STA_AUTH_WAIT_SEQ2, STA_MT2_MLME_AUTH_REQ, (STA_STATE_MACHINE_FUNC)StaInvalidStateWhenAuth);
	StaStateMachineSetAction(Sm, STA_AUTH_WAIT_SEQ2, STA_MT2_PEER_AUTH_EVEN, (STA_STATE_MACHINE_FUNC)StaPeerAuthRspAtSeq2Action);
	StaStateMachineSetAction(Sm, STA_AUTH_WAIT_SEQ2, STA_MT2_PEER_DEAUTH, (STA_STATE_MACHINE_FUNC)StaPeerDeauthAction);
	StaStateMachineSetAction(Sm, STA_AUTH_WAIT_SEQ2, STA_MT2_AUTH_TIMEOUT, (STA_STATE_MACHINE_FUNC)StaAuthTimeoutAction);

	// the third column
	StaStateMachineSetAction(Sm, STA_AUTH_WAIT_SEQ4, STA_MT2_MLME_AUTH_REQ, (STA_STATE_MACHINE_FUNC)StaInvalidStateWhenAuth);
	StaStateMachineSetAction(Sm, STA_AUTH_WAIT_SEQ4, STA_MT2_PEER_AUTH_EVEN, (STA_STATE_MACHINE_FUNC)StaPeerAuthRspAtSeq4Action);
	StaStateMachineSetAction(Sm, STA_AUTH_WAIT_SEQ2, STA_MT2_PEER_DEAUTH, (STA_STATE_MACHINE_FUNC)StaPeerDeauthAction);
	StaStateMachineSetAction(Sm, STA_AUTH_WAIT_SEQ4, STA_MT2_AUTH_TIMEOUT, (STA_STATE_MACHINE_FUNC)StaAuthTimeoutAction);

	// timer init
	RTMPInitTimer(pAd, &pAd->MlmeAux.ApCliAuthTimer, (PVOID)&StaAuthTimeout);

	for (i=0; i < MAX_APCLI_ENTRY; i++)
		pAd->ApCliTab.ApCliEntry[i].AuthCurrState = STA_AUTH_REQ_IDLE;

	return;
}

/*
	==========================================================================
	Description:
		function to be executed at timer thread when auth timer expires
	==========================================================================
 */
VOID StaAuthTimeout(
	IN  unsigned long data)
{
	RTMP_ADAPTER *pAd = (RTMP_ADAPTER *)data;

	DBGPRINT(RT_DEBUG_TRACE,"STA AUTH - AuthTimeout\n");

	StaMlmeEnqueue(pAd, STA_AUTH_STATE_MACHINE, STA_MT2_AUTH_TIMEOUT, 0, NULL, 0);
	MlmeHandler(pAd);

	return;
}

/*
	==========================================================================
	Description:
	==========================================================================
 */
VOID StaMlmeAuthReqAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex)
{
	UCHAR               Addr[MAC_ADDR_LEN];
	USHORT              Alg, Seq, Status;
	ULONG               Timeout;
	HEADER_802_11       AuthHdr; 
	PUCHAR              pOutBuffer = NULL;
	ULONG               FrameLen = 0;
	STA_CTRL_MSG_STRUCT StaCtrlMsg;

	// Block all authentication request durning WPA block period
	if (pAd->ApCliTab.ApCliEntry[ifIndex].bBlockAssoc == TRUE)
	{
		DBGPRINT(RT_DEBUG_TRACE, "STA AUTH - Block Auth request durning WPA block period!\n");
		*pCurrState = STA_AUTH_REQ_IDLE;
		StaCtrlMsg.Status = MLME_STATE_MACHINE_REJECT;
		StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_AUTH_RSP,
			sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);
	}
	else if(MlmeAuthReqSanity(pAd, Elem->Msg, Elem->MsgLen, Addr, &Timeout, &Alg))
	{
		// reset timer
		RTMPCancelTimer(&pAd->MlmeAux.ApCliAuthTimer);

		pAd->MlmeAux.Alg  = Alg;

		Seq = 1;
		Status = MLME_SUCCESS;

		// allocate and send out AuthReq frame
		pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
		if(pOutBuffer == NULL)
		{
			DBGPRINT(RT_DEBUG_TRACE, "AUTH - MlmeAuthReqAction() allocate memory failed\n");
			*pCurrState = STA_AUTH_REQ_IDLE;

			StaCtrlMsg.Status = MLME_FAIL_NO_RESOURCE;
			StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_AUTH_RSP,
				sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);
			return;
		}

#ifdef RTL865X_SOC
		printk("AUTH - Send AUTH request seq#1 (Alg=%d)...\n", Alg);
#else
		DBGPRINT(RT_DEBUG_TRACE, "AUTH - Send AUTH request seq#1 (Alg=%d)...\n", Alg);
#endif		
		ApCliMgtMacHeaderInit(pAd, &AuthHdr, SUBTYPE_AUTH, 0, Addr, pAd->MlmeAux.Bssid, ifIndex);

		MakeOutgoingFrame(pOutBuffer,           &FrameLen, 
						  sizeof(HEADER_802_11),&AuthHdr, 
						  2,                    &Alg, 
						  2,                    &Seq, 
						  2,                    &Status, 
						  END_OF_ARGS);

		MiniportMMRequest(pAd, pOutBuffer, FrameLen);
		kfree(pOutBuffer);

		RTMPAddTimer(&pAd->MlmeAux.ApCliAuthTimer, AUTH_TIMEOUT);

		*pCurrState = STA_AUTH_WAIT_SEQ2;
	} else
	{
		DBGPRINT(RT_DEBUG_ERROR, "AUTH - MlmeAuthReqAction() sanity check failed. BUG!!!!!\n");
		*pCurrState = STA_AUTH_REQ_IDLE;
	}

	return;
}

/*
	==========================================================================
	Description:
	==========================================================================
 */
VOID StaPeerAuthRspAtSeq2Action(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex) 
{
	UCHAR           Addr1[MAC_ADDR_LEN];
	UCHAR           Addr2[MAC_ADDR_LEN];
	USHORT          Seq, Status, RemoteStatus, Alg;
	UCHAR           ChlgText[CIPHER_TEXT_LEN];
	UCHAR           CyperChlgText[CIPHER_TEXT_LEN + 8 + 8];
	UCHAR           Element[2];
	HEADER_802_11   AuthHdr;
	PUCHAR          pOutBuffer = NULL;
	ULONG           FrameLen = 0;
	STA_CTRL_MSG_STRUCT StaCtrlMsg;

	if(PeerAuthSanity(pAd, Elem->Msg, Elem->MsgLen, Addr1, Addr2, &Alg, &Seq, &Status, ChlgText))
	{
		if(MAC_ADDR_EQUAL(pAd->MlmeAux.Bssid, Addr2) && Seq == 2)
		{
#ifdef RTL865X_SOC
			printk("APCLI AUTH - Receive AUTH_RSP seq#2 to me (Alg=%d, Status=%d)\n", Alg, Status);
#else			
			DBGPRINT(RT_DEBUG_TRACE, "APCLI AUTH - Receive AUTH_RSP seq#2 to me (Alg=%d, Status=%d)\n", Alg, Status);
#endif
			RTMPCancelTimer(&pAd->MlmeAux.ApCliAuthTimer);

			if(Status == MLME_SUCCESS)
			{
				if(pAd->MlmeAux.Alg == Ndis802_11AuthModeOpen)
				{
					*pCurrState = STA_AUTH_REQ_IDLE;

					StaCtrlMsg.Status= MLME_SUCCESS;
					StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_AUTH_RSP,
						sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);
				}
				else
				{
					// 2. shared key, need to be challenged
					Seq++;
					RemoteStatus = MLME_SUCCESS;
					// allocate and send out AuthRsp frame
					pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
					if(pOutBuffer == NULL)
					{
						DBGPRINT(RT_DEBUG_TRACE, "AUTH - PeerAuthRspAtSeq2Action() allocate memory fail\n");
						*pCurrState = STA_AUTH_REQ_IDLE;

						StaCtrlMsg.Status= MLME_FAIL_NO_RESOURCE;
						StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_AUTH_RSP,
							sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);
						return;
					}

#ifdef RTL865X_SOC
					printk("AUTH - Send AUTH request seq#3...\n");
#else
					DBGPRINT(RT_DEBUG_TRACE, "AUTH - Send AUTH request seq#3...\n");
#endif	
					ApCliMgtMacHeaderInit(pAd, &AuthHdr, SUBTYPE_AUTH, 0, Addr2, pAd->MlmeAux.Bssid, ifIndex);
					AuthHdr.FC.Wep = 1;
					// Encrypt challenge text & auth information
					RTMPInitWepEngine(
									 pAd,
									 pAd->ApCliTab.ApCliEntry[ifIndex].SharedKey[pAd->ApCliTab.ApCliEntry[ifIndex].DefaultKeyId].Key,
									 pAd->ApCliTab.ApCliEntry[ifIndex].DefaultKeyId,
									 pAd->ApCliTab.ApCliEntry[ifIndex].SharedKey[pAd->ApCliTab.ApCliEntry[ifIndex].DefaultKeyId].KeyLen,
									 CyperChlgText);
									 
#ifdef BIG_ENDIAN
					Alg = SWAP16(*(USHORT *)&Alg);
					Seq = SWAP16(*(USHORT *)&Seq);
					RemoteStatus= SWAP16(*(USHORT *)&RemoteStatus);
                    pAd->NeedSwapToLittleEndian = FALSE;					
#endif
					RTMPEncryptData(pAd, (PUCHAR) &Alg, CyperChlgText + 4, 2);
					RTMPEncryptData(pAd, (PUCHAR) &Seq, CyperChlgText + 6, 2);
					RTMPEncryptData(pAd, (PUCHAR) &RemoteStatus, CyperChlgText + 8, 2);

					Element[0] = 16;
					Element[1] = 128;
					RTMPEncryptData(pAd, Element, CyperChlgText + 10, 2);
					RTMPEncryptData(pAd, ChlgText, CyperChlgText + 12, 128);
					RTMPSetICV(pAd, CyperChlgText + 140);

					MakeOutgoingFrame(pOutBuffer,               &FrameLen, 
									  sizeof(HEADER_802_11),    &AuthHdr,  
									  CIPHER_TEXT_LEN + 16,     CyperChlgText, 
									  END_OF_ARGS);

					MiniportMMRequest(pAd, pOutBuffer, FrameLen);
#ifdef BIG_ENDIAN
                    pAd->NeedSwapToLittleEndian = TRUE;
#endif					
					kfree(pOutBuffer);

					pAd->MlmeAux.ApCliAuthTimer.expires = jiffies + AUTH_TIMEOUT;
					add_timer(&pAd->MlmeAux.ApCliAuthTimer);
					*pCurrState = STA_AUTH_WAIT_SEQ4;
				}
			} 
			else
			{
				*pCurrState = STA_AUTH_REQ_IDLE;

				StaCtrlMsg.Status= Status;
				StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_AUTH_RSP,
					sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);
			}
		}
	} else
	{
		DBGPRINT(RT_DEBUG_TRACE, "AUTH - PeerAuthSanity() sanity check fail\n");
	}

	return;
}

/*
	==========================================================================
	Description:
	==========================================================================
 */
VOID StaPeerAuthRspAtSeq4Action(
	IN PRTMP_ADAPTER pAd,
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex)
{
	UCHAR       Addr1[MAC_ADDR_LEN];
	UCHAR       Addr2[MAC_ADDR_LEN];
	USHORT      Alg, Seq, Status;
	CHAR        ChlgText[CIPHER_TEXT_LEN];
	STA_CTRL_MSG_STRUCT StaCtrlMsg;

	if(PeerAuthSanity(pAd, Elem->Msg, Elem->MsgLen, Addr1, Addr2, &Alg, &Seq, &Status, ChlgText))
	{
		if(MAC_ADDR_EQUAL(pAd->MlmeAux.Bssid, Addr2) && Seq == 4)
		{
#ifdef RTL865X_SOC
			printk("AUTH - Receive AUTH_RSP seq#4 to me\n");
#else
			DBGPRINT(RT_DEBUG_TRACE, "AUTH - Receive AUTH_RSP seq#4 to me\n");
#endif	
			RTMPCancelTimer(&pAd->MlmeAux.ApCliAuthTimer);

			StaCtrlMsg.Status = MLME_SUCCESS;

			if(Status != MLME_SUCCESS)
			{
				StaCtrlMsg.Status = Status;
			}

			*pCurrState = STA_AUTH_REQ_IDLE;
			StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_AUTH_RSP,
			sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);
		}
	} else
	{
		DBGPRINT(RT_DEBUG_TRACE, "AUTH - PeerAuthRspAtSeq4Action() sanity check fail\n");
	}

	return;
}

/*
    ==========================================================================
    Description:
    ==========================================================================
*/
VOID StaPeerDeauthAction(
	IN PRTMP_ADAPTER pAd,
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex)
{
	UCHAR       Addr2[MAC_ADDR_LEN];
	USHORT      Reason;

	if (PeerDeauthSanity(pAd, Elem->Msg, Elem->MsgLen, Addr2, &Reason))
	{
		DBGPRINT(RT_DEBUG_TRACE,"STA AUTH_RSP - receive DE-AUTH from our AP\n");
		*pCurrState = STA_AUTH_REQ_IDLE;

		StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_PEER_DISCONNECT_REQ, 0, NULL, ifIndex);
	}
	else
	{
		DBGPRINT(RT_DEBUG_TRACE,"STA AUTH_RSP - StaPeerDeauthAction() sanity check fail\n");
	}

	return;
}

/*
	==========================================================================
	Description:
	==========================================================================
 */
VOID StaAuthTimeoutAction(
	IN PRTMP_ADAPTER pAd,
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex)
{
	DBGPRINT(RT_DEBUG_TRACE, "APCLI AUTH - AuthTimeoutAction\n");

	*pCurrState = STA_AUTH_REQ_IDLE;

	StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_AUTH_REQ_TIMEOUT, 0, NULL, ifIndex);

	return;
}

/*
	==========================================================================
	Description:
	==========================================================================
 */
VOID StaInvalidStateWhenAuth(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex) 
{
	STA_CTRL_MSG_STRUCT StaCtrlMsg;

	DBGPRINT(RT_DEBUG_TRACE, "AUTH - InvalidStateWhenAuth (state=%d), reset AUTH state machine\n",
		pAd->Mlme.ApCliAuthMachine.CurrState);

	*pCurrState= STA_AUTH_REQ_IDLE;

	StaCtrlMsg.Status = MLME_STATE_MACHINE_REJECT;
	StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_AUTH_RSP,
		sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);

	return;
}

