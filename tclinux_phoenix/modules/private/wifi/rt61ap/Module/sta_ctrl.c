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
	sta_ctrl.c

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Fonchi		2006-06-23      modified for rt61-APClinent
*/
#include "rt_config.h"

/*
    ==========================================================================
    Description:
        The apcli ctrl state machine, 
    Parameters:
        Sm - pointer to the state machine
    Note:
        the state machine looks like the following
    ==========================================================================
 */
VOID StaCtrlStateMachineInit(
	IN PRTMP_ADAPTER pAd,
	IN STA_STATE_MACHINE *Sm,
	OUT STA_STATE_MACHINE_FUNC Trans[])
{
	UCHAR i;

	StaStateMachineInit(Sm, (STA_STATE_MACHINE_FUNC*)Trans, STA_MAX_CTRL_STATE, STA_MAX_CTRL_MSG, (STA_STATE_MACHINE_FUNC)StaDrop, STA_CTRL_DISCONNECTED, STA_CTRL_MACHINE_BASE);

	// disconnected state
	StaStateMachineSetAction(Sm, STA_CTRL_DISCONNECTED, STA_CTRL_JOIN_REQ, (STA_STATE_MACHINE_FUNC)StaCtrlJoinReqAction);

	// probe state
	StaStateMachineSetAction(Sm, STA_CTRL_PROBE, STA_CTRL_PROBE_RSP, (STA_STATE_MACHINE_FUNC)StaCtrlProbeRspAction);
	StaStateMachineSetAction(Sm, STA_CTRL_PROBE, STA_CTRL_JOIN_REQ_TIMEOUT, (STA_STATE_MACHINE_FUNC)StaCtrlJoinReqTimeoutAction);
	StaStateMachineSetAction(Sm, STA_CTRL_PROBE, STA_CTRL_DISCONNECT_REQ, (STA_STATE_MACHINE_FUNC)StaCtrlDisconnectReqAction);

	// auth state
	StaStateMachineSetAction(Sm, STA_CTRL_AUTH, STA_CTRL_AUTH_RSP, (STA_STATE_MACHINE_FUNC)StaCtrlAuthRspAction);
	StaStateMachineSetAction(Sm, STA_CTRL_AUTH, STA_CTRL_AUTH_REQ_TIMEOUT, (STA_STATE_MACHINE_FUNC)StaCtrlAuthReqTimeoutAction);
	StaStateMachineSetAction(Sm, STA_CTRL_AUTH, STA_CTRL_DISCONNECT_REQ, (STA_STATE_MACHINE_FUNC)StaCtrlDisconnectReqAction);
 	StaStateMachineSetAction(Sm, STA_CTRL_AUTH, STA_CTRL_PEER_DISCONNECT_REQ, (STA_STATE_MACHINE_FUNC)StaCtrlPeerDeAssocReqAction);

	// auth2 state
	StaStateMachineSetAction(Sm, STA_CTRL_AUTH_2, STA_CTRL_AUTH_RSP, (STA_STATE_MACHINE_FUNC)StaCtrlAuth2RspAction);
	StaStateMachineSetAction(Sm, STA_CTRL_AUTH_2, STA_CTRL_AUTH_REQ_TIMEOUT, (STA_STATE_MACHINE_FUNC)StaCtrlAuth2ReqTimeoutAction);
	StaStateMachineSetAction(Sm, STA_CTRL_AUTH_2, STA_CTRL_DISCONNECT_REQ, (STA_STATE_MACHINE_FUNC)StaCtrlDisconnectReqAction);
 	StaStateMachineSetAction(Sm, STA_CTRL_AUTH_2, STA_CTRL_PEER_DISCONNECT_REQ, (STA_STATE_MACHINE_FUNC)StaCtrlPeerDeAssocReqAction);

	// assoc state
	StaStateMachineSetAction(Sm, STA_CTRL_ASSOC, STA_CTRL_ASSOC_RSP, (STA_STATE_MACHINE_FUNC)StaCtrlAssocRspAction);
	StaStateMachineSetAction(Sm, STA_CTRL_ASSOC, STA_CTRL_ASSOC_REQ_TIMEOUT, (STA_STATE_MACHINE_FUNC)StaCtrlAssocReqTimeoutAction);
	StaStateMachineSetAction(Sm, STA_CTRL_ASSOC, STA_CTRL_DISCONNECT_REQ, (STA_STATE_MACHINE_FUNC)StaCtrlDeAssocAction);
 	StaStateMachineSetAction(Sm, STA_CTRL_ASSOC, STA_CTRL_PEER_DISCONNECT_REQ, (STA_STATE_MACHINE_FUNC)StaCtrlPeerDeAssocReqAction);

	// deassoc state
	StaStateMachineSetAction(Sm, STA_CTRL_DEASSOC, STA_CTRL_DEASSOC_RSP, (STA_STATE_MACHINE_FUNC)StaCtrlDeAssocRspAction);

	// connected state
	StaStateMachineSetAction(Sm, STA_CTRL_CONNECTED, STA_CTRL_DISCONNECT_REQ, (STA_STATE_MACHINE_FUNC)StaCtrlDeAssocAction);
 	StaStateMachineSetAction(Sm, STA_CTRL_CONNECTED, STA_CTRL_PEER_DISCONNECT_REQ, (STA_STATE_MACHINE_FUNC)StaCtrlPeerDeAssocReqAction);

	for (i = 0; i < MAX_APCLI_ENTRY; i++)
		pAd->ApCliTab.ApCliEntry[i].CtrlCurrState = STA_CTRL_DISCONNECTED;

	return;
}


/* 
    ==========================================================================
    Description:
        APCLI MLME JOIN req state machine procedure
    ==========================================================================
 */
VOID StaCtrlJoinReqAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex)
{
	MLME_JOIN_REQ_STRUCT JoinReq;

#ifdef RTL865X_SOC
	printk("(%s) Start Probe Req.\n", __FUNCTION__);
#else	
	DBGPRINT(RT_DEBUG_TRACE, "(%s) Start Probe Req.\n", __FUNCTION__);
#endif	

	NdisZeroMemory(&JoinReq, sizeof(MLME_JOIN_REQ_STRUCT));

	if (!MAC_ADDR_EQUAL(pAd->ApCliTab.ApCliEntry[ifIndex].CfgApCliBssid, ZERO_MAC_ADDR))
	{
		COPY_MAC_ADDR(JoinReq.Bssid, pAd->ApCliTab.ApCliEntry[ifIndex].CfgApCliBssid);
	}

// For AP Client support WPS Modification
#ifdef WSC_SUPPORT
	if (pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.WscConfMode != WSC_DISABLE)
	{
		NdisZeroMemory(JoinReq.Ssid, MAX_LEN_OF_SSID);
		JoinReq.SsidLen = pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.WscSsid.SsidLength;
		NdisMoveMemory(JoinReq.Ssid, pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.WscSsid.Ssid, JoinReq.SsidLen);
	}
	else
#endif // WSC_SUPPORT //
// --
	if (pAd->ApCliTab.ApCliEntry[ifIndex].CfgSsidLen != 0)
	{
		JoinReq.SsidLen = pAd->ApCliTab.ApCliEntry[ifIndex].CfgSsidLen;
		NdisMoveMemory(&(JoinReq.Ssid), pAd->ApCliTab.ApCliEntry[ifIndex].CfgSsid, JoinReq.SsidLen);
	}

	DBGPRINT(RT_DEBUG_TRACE, "(%s) Probe Ssid=%s, Bssid=%02x:%02x:%02x:%02x:%02x:%02x\n",
		__FUNCTION__, JoinReq.Ssid, JoinReq.Bssid[0], JoinReq.Bssid[1], JoinReq.Bssid[2],
		JoinReq.Bssid[3], JoinReq.Bssid[4], JoinReq.Bssid[5]);

	*pCurrState = STA_CTRL_PROBE;

	StaMlmeEnqueue(pAd, STA_SYNC_STATE_MACHINE, STA_MT2_MLME_PROBE_REQ,
		sizeof(MLME_JOIN_REQ_STRUCT), &JoinReq, ifIndex);

	return;
}

/* 
    ==========================================================================
    Description:
        APCLI MLME JOIN req timeout state machine procedure
    ==========================================================================
 */
VOID StaCtrlJoinReqTimeoutAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex)
{
	MLME_JOIN_REQ_STRUCT JoinReq;

	DBGPRINT(RT_DEBUG_TRACE, "(%s) Probe Req Timeout.\n", __FUNCTION__);

	// stay in same state.
	*pCurrState = STA_CTRL_PROBE;

	// retry Probe Req.
	DBGPRINT(RT_DEBUG_TRACE, "(%s) Retry Probe Req.\n", __FUNCTION__);

	NdisZeroMemory(&JoinReq, sizeof(MLME_JOIN_REQ_STRUCT));

	if (!MAC_ADDR_EQUAL(pAd->ApCliTab.ApCliEntry[ifIndex].CfgApCliBssid, ZERO_MAC_ADDR))
	{
		COPY_MAC_ADDR(JoinReq.Bssid, pAd->ApCliTab.ApCliEntry[ifIndex].CfgApCliBssid);
	}

// For AP Client support WPS Modification
#ifdef WSC_SUPPORT
	if (pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.WscConfMode != WSC_DISABLE)
	{
		NdisZeroMemory(JoinReq.Ssid, MAX_LEN_OF_SSID);
		JoinReq.SsidLen = pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.WscSsid.SsidLength;
		NdisMoveMemory(JoinReq.Ssid, pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.WscSsid.Ssid, JoinReq.SsidLen);
	}
	else
#endif // WSC_SUPPORT //
// --
	if (pAd->ApCliTab.ApCliEntry[ifIndex].CfgSsidLen != 0)
	{
		JoinReq.SsidLen = pAd->ApCliTab.ApCliEntry[ifIndex].CfgSsidLen;
		NdisMoveMemory(&(JoinReq.Ssid), pAd->ApCliTab.ApCliEntry[ifIndex].CfgSsid, JoinReq.SsidLen);
	}

	DBGPRINT(RT_DEBUG_TRACE, "(%s) Probe Ssid=%s, Bssid=%02x:%02x:%02x:%02x:%02x:%02x\n",
		__FUNCTION__, JoinReq.Ssid, JoinReq.Bssid[0], JoinReq.Bssid[1], JoinReq.Bssid[2],
		JoinReq.Bssid[3], JoinReq.Bssid[4], JoinReq.Bssid[5]);
	StaMlmeEnqueue(pAd, STA_SYNC_STATE_MACHINE, STA_MT2_MLME_PROBE_REQ,
		sizeof(MLME_JOIN_REQ_STRUCT), &JoinReq, ifIndex);

	return;
}

/* 
    ==========================================================================
    Description:
        APCLI MLME Probe Rsp state machine procedure
    ==========================================================================
 */
VOID StaCtrlProbeRspAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex)
{
	STA_CTRL_MSG_STRUCT *Info = (STA_CTRL_MSG_STRUCT *)(Elem->Msg);
	USHORT Status = Info->Status;

	MLME_AUTH_REQ_STRUCT AuthReq;

	if (Status == MLME_SUCCESS)
	{
		DBGPRINT(RT_DEBUG_TRACE, "(%s) Probe respond success.\n", __FUNCTION__);
			COPY_MAC_ADDR(pAd->ApCliTab.ApCliEntry[ifIndex].ApCliBssid, pAd->MlmeAux.Bssid);

			pAd->ApCliTab.ApCliEntry[ifIndex].SsidLen = pAd->MlmeAux.SsidLen;
			NdisMoveMemory(pAd->ApCliTab.ApCliEntry[ifIndex].Ssid, pAd->MlmeAux.Ssid, pAd->ApCliTab.ApCliEntry[ifIndex].SsidLen);

		// Store RSN_IE got from AP's Beacon or Probe rsp
		//if (pAd->MlmeAux.VarIELen != 0)
		//{
		//	DBGPRINT(RT_DEBUG_TRACE, "(%s) Store RSN_IE got from AP's Beacon or Probe rsp \n", __FUNCTION__);
			// RSN_IE got from AP's Beacon or Probe rsp may contain 2 unicase ciphers TKIP and AES.
			// We can't copy it to ApCli RSNIE directly.
		//	NdisMoveMemory(pAd->ApCliTab.ApCliEntry[ifIndex].RSN_IE, pAd->MlmeAux.VarIEs, pAd->MlmeAux.VarIELen);
		//	pAd->ApCliTab.ApCliEntry[ifIndex].RSN_IELen = pAd->MlmeAux.VarIELen;
		//}
		//else
		//	pAd->ApCliTab.ApCliEntry[ifIndex].RSN_IELen = 0;

		DBGPRINT(RT_DEBUG_TRACE, "(%s) Apcli-Interface Ssid=%s.\n", __FUNCTION__, pAd->ApCliTab.ApCliEntry[ifIndex].Ssid);
		DBGPRINT(RT_DEBUG_TRACE, "(%s) Apcli-Interface Bssid=%02x:%02x:%02x:%02x:%02x:%02x.\n",
			__FUNCTION__,
			pAd->ApCliTab.ApCliEntry[ifIndex].ApCliBssid[0],
			pAd->ApCliTab.ApCliEntry[ifIndex].ApCliBssid[1],
			pAd->ApCliTab.ApCliEntry[ifIndex].ApCliBssid[2],
			pAd->ApCliTab.ApCliEntry[ifIndex].ApCliBssid[3],
			pAd->ApCliTab.ApCliEntry[ifIndex].ApCliBssid[4],
			pAd->ApCliTab.ApCliEntry[ifIndex].ApCliBssid[5]);

		*pCurrState = STA_CTRL_AUTH;

		pAd->ApCliTab.ApCliEntry[ifIndex].AuthReqCnt = 0;

		COPY_MAC_ADDR(AuthReq.Addr, pAd->ApCliTab.ApCliEntry[ifIndex].ApCliBssid);

		// start Authentication Req.		
		// If AuthMode is Auto, try shared key first
		if ((pAd->ApCliTab.ApCliEntry[0].AuthMode == Ndis802_11AuthModeShared) ||
			(pAd->ApCliTab.ApCliEntry[0].AuthMode == Ndis802_11AuthModeAutoSwitch))
		{		
			AuthReq.Alg = Ndis802_11AuthModeShared;
		}
		else
		{
			AuthReq.Alg = Ndis802_11AuthModeOpen;
		}

		AuthReq.Timeout = AUTH_TIMEOUT;
		StaMlmeEnqueue(pAd, STA_AUTH_STATE_MACHINE, STA_MT2_MLME_AUTH_REQ,
			sizeof(MLME_AUTH_REQ_STRUCT), &AuthReq, ifIndex);
	} else
	{
		DBGPRINT(RT_DEBUG_TRACE, "(%s) Probe respond fail.\n", __FUNCTION__);
		*pCurrState = STA_CTRL_DISCONNECTED;
	}

	return;
}

/* 
    ==========================================================================
    Description:
        APCLI MLME AUTH Rsp state machine procedure
    ==========================================================================
 */
VOID StaCtrlAuthRspAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex)
{
	STA_CTRL_MSG_STRUCT *Info = (STA_CTRL_MSG_STRUCT *)(Elem->Msg);
	USHORT Status = Info->Status;
	MLME_ASSOC_REQ_STRUCT  AssocReq;
	MLME_AUTH_REQ_STRUCT AuthReq;

	if(Status == MLME_SUCCESS)
	{
		DBGPRINT(RT_DEBUG_TRACE, "(%s) Auth Rsp Success.\n", __FUNCTION__);
		*pCurrState = STA_CTRL_ASSOC;

		pAd->ApCliTab.ApCliEntry[ifIndex].AssocReqCnt = 0;

		AssocParmFill(pAd, &AssocReq, pAd->MlmeAux.Bssid, pAd->MlmeAux.CapabilityInfo,
			ASSOC_TIMEOUT, /*pAd->PortCfg.DefaultListenCount*/5);
		StaMlmeEnqueue(pAd, STA_ASSOC_STATE_MACHINE, STA_MT2_MLME_ASSOC_REQ, 
			sizeof(MLME_ASSOC_REQ_STRUCT), &AssocReq, ifIndex);
	} else
	{
		if (pAd->ApCliTab.ApCliEntry[0].AuthMode == Ndis802_11AuthModeAutoSwitch)
		{
			DBGPRINT(RT_DEBUG_TRACE, "(%s) Auth Rsp Failure.\n", __FUNCTION__);

			*pCurrState = STA_CTRL_AUTH_2;

			// start Second Authentication Req.
			DBGPRINT(RT_DEBUG_TRACE, "(%s) Start Second Auth Rep.\n", __FUNCTION__);
			COPY_MAC_ADDR(AuthReq.Addr, pAd->ApCliTab.ApCliEntry[ifIndex].ApCliBssid);
			AuthReq.Alg = Ndis802_11AuthModeOpen;
			AuthReq.Timeout = AUTH_TIMEOUT;
			StaMlmeEnqueue(pAd, STA_AUTH_STATE_MACHINE, STA_MT2_MLME_AUTH_REQ,
			sizeof(MLME_AUTH_REQ_STRUCT), &AuthReq, ifIndex);
		} else
		{
			NdisZeroMemory(pAd->MlmeAux.Bssid, MAC_ADDR_LEN);
			NdisZeroMemory(pAd->MlmeAux.Ssid, MAX_LEN_OF_SSID);
			pAd->ApCliTab.ApCliEntry[ifIndex].AuthReqCnt = 0;
			*pCurrState = STA_CTRL_DISCONNECTED;
		}
	}

	return;
}

/* 
    ==========================================================================
    Description:
        APCLI MLME AUTH2 Rsp state machine procedure
    ==========================================================================
 */
VOID StaCtrlAuth2RspAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex)
{
	STA_CTRL_MSG_STRUCT *Info = (STA_CTRL_MSG_STRUCT *)(Elem->Msg);
	USHORT Status = Info->Status;
	MLME_ASSOC_REQ_STRUCT  AssocReq;

	if(Status == MLME_SUCCESS)
	{
		DBGPRINT(RT_DEBUG_TRACE, "(%s) Auth2 Rsp Success.\n", __FUNCTION__);
		*pCurrState = STA_CTRL_ASSOC;

		pAd->ApCliTab.ApCliEntry[ifIndex].AssocReqCnt = 0;

		AssocParmFill(pAd, &AssocReq, pAd->MlmeAux.Bssid, pAd->MlmeAux.CapabilityInfo,
			ASSOC_TIMEOUT, /*pAd->PortCfg.DefaultListenCount*/5);
		StaMlmeEnqueue(pAd, STA_ASSOC_STATE_MACHINE, STA_MT2_MLME_ASSOC_REQ, 
			sizeof(MLME_ASSOC_REQ_STRUCT), &AssocReq, ifIndex);
	} else
	{
		DBGPRINT(RT_DEBUG_TRACE, "(%s) Sta Auth Rsp Failure.\n", __FUNCTION__);

		*pCurrState = STA_CTRL_DISCONNECTED;
	}

	return;
}

/* 
    ==========================================================================
    Description:
        APCLI MLME Auth Req timeout state machine procedure
    ==========================================================================
 */
VOID StaCtrlAuthReqTimeoutAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex)
{
	MLME_AUTH_REQ_STRUCT AuthReq;

	DBGPRINT(RT_DEBUG_TRACE, "(%s) Auth Req Timeout.\n", __FUNCTION__);

	pAd->ApCliTab.ApCliEntry[ifIndex].AuthReqCnt++;

	if (pAd->ApCliTab.ApCliEntry[ifIndex].AuthReqCnt > 5)
	{
		*pCurrState = STA_CTRL_DISCONNECTED;
		NdisZeroMemory(pAd->MlmeAux.Bssid, MAC_ADDR_LEN);
		NdisZeroMemory(pAd->MlmeAux.Ssid, MAX_LEN_OF_SSID);
		pAd->ApCliTab.ApCliEntry[ifIndex].AuthReqCnt = 0;
		return;
	}

	// stay in same state.
	*pCurrState = STA_CTRL_AUTH;

	// retry Authentication.
	DBGPRINT(RT_DEBUG_TRACE, "(%s) Retry Auth Req.\n", __FUNCTION__);
	COPY_MAC_ADDR(AuthReq.Addr, pAd->ApCliTab.ApCliEntry[ifIndex].ApCliBssid);
	AuthReq.Alg = Ndis802_11AuthModeOpen;
	AuthReq.Timeout = AUTH_TIMEOUT;
	StaMlmeEnqueue(pAd, STA_AUTH_STATE_MACHINE, STA_MT2_MLME_AUTH_REQ,
		sizeof(MLME_AUTH_REQ_STRUCT), &AuthReq, ifIndex);

	return;
}

/* 
    ==========================================================================
    Description:
        APCLI MLME Auth2 Req timeout state machine procedure
    ==========================================================================
 */
VOID StaCtrlAuth2ReqTimeoutAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex)
{
	return;
}

/* 
    ==========================================================================
    Description:
        APCLI MLME ASSOC RSP state machine procedure
    ==========================================================================
 */
VOID StaCtrlAssocRspAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex)
{
	STA_CTRL_MSG_STRUCT *Info = (STA_CTRL_MSG_STRUCT *)(Elem->Msg);
	USHORT Status = Info->Status;

	if(Status == MLME_SUCCESS)
	{
#ifdef RTL865X_SOC
		printk("(%s) apCliIf = %d, Receive Assoc Rsp Success.\n", __FUNCTION__, ifIndex);
#else
		DBGPRINT(RT_DEBUG_TRACE, "(%s) apCliIf = %d, Receive Assoc Rsp Success.\n", __FUNCTION__, ifIndex);
#endif
		*pCurrState = STA_CTRL_CONNECTED;

		// set the apcli interface be valid.
		pAd->ApCliTab.ApCliEntry[ifIndex].Valid = TRUE;

		// ApCli will dynamic switch to new channel specifed in SWITCH-ANNOUNCEMNET IE.
		// Stop DFS function while ApCli connected to a root-ap.
		if (pAd->PortCfg.RadarDetect.IEEE80211H == TRUE)
		{
			AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x00, 0x00);   // send stop-RD command to MCU
			RTMPusecDelay(500000);
			AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x00, 0x00);   // send stop-RD command to MCU
		}

// For AP Client support WPS Modification
#ifdef WSC_SUPPORT
		// WSC initial connect to AP, jump to Wsc start action and set the correct parameters    
		if ((pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.WscState != WSC_STATE_OFF) && 
			(pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.WscConfMode == WSC_AP_Enrollee))
		{
			pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.WscState = WSC_STATE_LINK_UP;
			pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.WscStatus = WSC_STATE_LINK_UP;
			pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.EntryApIdx = MIN_NET_DEVICE_FOR_APCLI;
			pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.WscConfStatus = WSC_SCSTATE_UNCONFIGURED;
			NdisZeroMemory(pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.EntryAddr, MAC_ADDR_LEN);        
			NdisMoveMemory(pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.EntryAddr, pAd->MlmeAux.Bssid, MAC_ADDR_LEN);
			WscSendEapolStart(pAd, pAd->MlmeAux.Bssid, TRUE);
		}
		else
		{
			WscStop(pAd, &pAd->ApCliTab.ApCliEntry[ifIndex].WscControl, TRUE);
		}
#endif // WSC_SUPPORT //
// --
	} else
	{
		DBGPRINT(RT_DEBUG_TRACE, "(%s) apCliIf = %d, Receive Assoc Rsp Failure.\n", __FUNCTION__,  ifIndex);

		*pCurrState = STA_CTRL_DISCONNECTED;

		// set the apcli interface be valid.
		pAd->ApCliTab.ApCliEntry[ifIndex].Valid = FALSE;
	}

	return;
}

/* 
    ==========================================================================
    Description:
        APCLI MLME DeASSOC RSP state machine procedure
    ==========================================================================
 */
VOID StaCtrlDeAssocRspAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex)
{
	STA_CTRL_MSG_STRUCT *Info = (STA_CTRL_MSG_STRUCT *)(Elem->Msg);
	USHORT Status = Info->Status;

	if (Status == MLME_SUCCESS)
	{
		DBGPRINT(RT_DEBUG_TRACE, "(%s) Receive DeAssoc Rsp Success.\n", __FUNCTION__);
	} else
	{
		DBGPRINT(RT_DEBUG_TRACE, "(%s) Receive DeAssoc Rsp Failure.\n", __FUNCTION__);
	}

	if (pAd->ApCliTab.ApCliEntry[ifIndex].Valid)
		ApCliLinkDown(pAd, ifIndex);
	
	*pCurrState = STA_CTRL_DISCONNECTED;

	return;
}

/* 
    ==========================================================================
    Description:
        APCLI MLME Assoc Req timeout state machine procedure
    ==========================================================================
 */
VOID StaCtrlAssocReqTimeoutAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex)
{
	MLME_ASSOC_REQ_STRUCT  AssocReq;

	DBGPRINT(RT_DEBUG_TRACE, "(%s) Assoc Req Timeout.\n", __FUNCTION__);

	// give up to retry authentication req after retry it 5 times.
	pAd->ApCliTab.ApCliEntry[ifIndex].AssocReqCnt++;
	if (pAd->ApCliTab.ApCliEntry[ifIndex].AssocReqCnt > 5)
	{
		*pCurrState = STA_CTRL_DISCONNECTED;
		NdisZeroMemory(pAd->MlmeAux.Bssid, MAC_ADDR_LEN);
		NdisZeroMemory(pAd->MlmeAux.Ssid, MAX_LEN_OF_SSID);
		pAd->ApCliTab.ApCliEntry[ifIndex].AuthReqCnt = 0;
		return;
		
	}

	// stay in same state.
	*pCurrState = STA_CTRL_ASSOC;

	// retry Association Req.
	DBGPRINT(RT_DEBUG_TRACE, "(%s) Retry Association Req.\n", __FUNCTION__);
	AssocParmFill(pAd, &AssocReq, pAd->MlmeAux.Bssid, pAd->MlmeAux.CapabilityInfo,
		ASSOC_TIMEOUT, /*pAd->PortCfg.DefaultListenCount*/5);
	StaMlmeEnqueue(pAd, STA_ASSOC_STATE_MACHINE, STA_MT2_MLME_ASSOC_REQ, 
		sizeof(MLME_ASSOC_REQ_STRUCT), &AssocReq, ifIndex);

	return;
}

/* 
    ==========================================================================
    Description:
        APCLI MLME Disconnect Rsp state machine procedure
    ==========================================================================
 */
VOID StaCtrlDisconnectReqAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex)
{
	DBGPRINT(RT_DEBUG_TRACE, "(%s) MLME Request disconnect.\n", __FUNCTION__);

	if (pAd->ApCliTab.ApCliEntry[ifIndex].Valid)
		ApCliLinkDown(pAd, ifIndex);

	// set the apcli interface be invalid.
	pAd->ApCliTab.ApCliEntry[ifIndex].Valid = FALSE;

	// clear MlmeAux.Ssid and Bssid.
	NdisZeroMemory(pAd->MlmeAux.Bssid, MAC_ADDR_LEN);
	pAd->MlmeAux.SsidLen = 0;
	NdisZeroMemory(pAd->MlmeAux.Ssid, MAX_LEN_OF_SSID);
	pAd->MlmeAux.Rssi = 0;

	*pCurrState = STA_CTRL_DISCONNECTED;

	return;
}

/* 
    ==========================================================================
    Description:
        APCLI MLME Peer DeAssoc Req state machine procedure
    ==========================================================================
 */
VOID StaCtrlPeerDeAssocReqAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex)
{
	DBGPRINT(RT_DEBUG_TRACE, "(%s) Peer DeAssoc Req.\n", __FUNCTION__);

	if (pAd->ApCliTab.ApCliEntry[ifIndex].Valid)
		ApCliLinkDown(pAd, ifIndex);

	// set the apcli interface be invalid.
	pAd->ApCliTab.ApCliEntry[ifIndex].Valid = FALSE;

	// clear MlmeAux.Ssid and Bssid.
	NdisZeroMemory(pAd->MlmeAux.Bssid, MAC_ADDR_LEN);
	pAd->MlmeAux.SsidLen = 0;
	NdisZeroMemory(pAd->MlmeAux.Ssid, MAX_LEN_OF_SSID);
	pAd->MlmeAux.Rssi = 0;

	*pCurrState = STA_CTRL_DISCONNECTED;

	return;
}

/* 
    ==========================================================================
    Description:
        APCLI MLME Disconnect Req state machine procedure
    ==========================================================================
 */
VOID StaCtrlDeAssocAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex)
{
	MLME_DISASSOC_REQ_STRUCT DisassocReq;

	DBGPRINT(RT_DEBUG_TRACE, "(%s) MLME Request Disconnect.\n", __FUNCTION__);

	DisassocParmFill(pAd, &DisassocReq, pAd->MlmeAux.Bssid, REASON_DISASSOC_STA_LEAVING);
	StaMlmeEnqueue(pAd, STA_ASSOC_STATE_MACHINE, STA_MT2_MLME_DISASSOC_REQ,
		sizeof(MLME_DISASSOC_REQ_STRUCT), &DisassocReq, ifIndex);

	if (pAd->ApCliTab.ApCliEntry[ifIndex].Valid)
		ApCliLinkDown(pAd, ifIndex);

	// set the apcli interface be invalid.
	pAd->ApCliTab.ApCliEntry[ifIndex].Valid = FALSE;

	// clear MlmeAux.Ssid and Bssid.
	NdisZeroMemory(pAd->MlmeAux.Bssid, MAC_ADDR_LEN);
	pAd->MlmeAux.SsidLen = 0;
	NdisZeroMemory(pAd->MlmeAux.Ssid, MAX_LEN_OF_SSID);
	pAd->MlmeAux.Rssi = 0;

	*pCurrState = STA_CTRL_DEASSOC;

	return;
}


/* 
    ==========================================================================
    Description:
        APCLI Interface Up.
    ==========================================================================
 */
VOID ApCliIfUp(
	IN PRTMP_ADAPTER pAd)
{
	UCHAR ifIndex;

	for(ifIndex = 0; ifIndex < MAX_APCLI_ENTRY; ifIndex++)
	{
		if ((pAd->ApCliTab.ApCliEntry[ifIndex].InterfaceReady == TRUE) 
			&& (pAd->ApCliTab.ApCliEntry[ifIndex].Enable == TRUE)
			&& (pAd->ApCliTab.ApCliEntry[ifIndex].Valid == FALSE))
		{
			DBGPRINT(RT_DEBUG_TRACE, "(%s) ApCli interface[%d] startup.\n", __FUNCTION__, ifIndex);
			StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_JOIN_REQ, 0, NULL, ifIndex);
		}
	}

	return;
}


/* 
    ==========================================================================
    Description:
        APCLI Interface Down.
    ==========================================================================
 */
VOID ApCliIfDown(
	IN PRTMP_ADAPTER pAd)
{
	UCHAR ifIndex;

	for(ifIndex = 0; ifIndex < MAX_APCLI_ENTRY; ifIndex++)
	{
		if (!(pAd->ApCliTab.ApCliEntry[ifIndex].Enable))
		{
			DBGPRINT(RT_DEBUG_TRACE, "(%s) ApCli interface[%d] startdown.\n", __FUNCTION__, ifIndex);
			StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_DISCONNECT_REQ, 0, NULL, ifIndex);
		}
	}

	return;
}



/* 
    ==========================================================================
    Description:
        APCLI Interface Monitor.
    ==========================================================================
 */
VOID ApCliIfMonitor(
	IN PRTMP_ADAPTER pAd)
{
	UCHAR index;
	UMAXLONG Now32;

	Now32 = jiffies;
	for(index = 0; index < MAX_APCLI_ENTRY; index++)
	{
		if ((pAd->ApCliTab.ApCliEntry[index].Valid == TRUE)
			&& (pAd->ApCliTab.ApCliEntry[index].ApCliRcvBeaconTime + BEACON_LOST_TIME < Now32))
		{
			DBGPRINT(RT_DEBUG_TRACE,"MlmePeriodicExec: no traffic and Beancon on ApCli%d, Probe-Response from root-AP.\n", index);
			DBGPRINT(RT_DEBUG_TRACE,"MlmePeriodicExec: Reconnect the Root-Ap again.\n");
			StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_DISCONNECT_REQ, 0, NULL, index);
			MlmeHandler(pAd);
		}
		pAd->ApCliTab.ApCliEntry[index].NumOfAvgRssiSample = 0;
	}

	return;
}

VOID ChannelSwitchAnnouncementProc(
	IN PRTMP_ADAPTER pAd,
	IN NDIS_802_11_VARIABLE_IEs	*pVIE,
	UCHAR LenVIE)
{
	UCHAR Length;
    PEID_STRUCT pEid;
    PUCHAR pTmp;
	Length = LenVIE;

	while (Length > 0)
	{
		// Parse cipher suite base on WPA1 & WPA2, they should be parsed differently
		pTmp = ((PUCHAR)pVIE) + LenVIE - Length;
		pEid = (PEID_STRUCT) pTmp;
		
		do
		{
			// is CHANNEL-SWITCH_ANNOUNCEMENT IE.
			if (pEid->Eid != IE_CHANNEL_SWITCH_ANNOUNCEMENT)
				break;

			// invaild IE length.
			if (pEid->Len < 3)
				break;

			// chech the CSCount
			if (pEid->Octet[2] != 0)
				break;

			pAd->PortCfg.Channel = pEid->Octet[1];

			// switch to the channel specified in CHANNEL-SWITCH_ANNOUNCEMENT IE.
			DBGPRINT(RT_DEBUG_TRACE, "APClI received Channel-Switch-announcement. switch ch to %d\n", pAd->PortCfg.Channel);

			ApStop(pAd, TRUE);
			ApStartUp(pAd, TRUE);

		} while (FALSE);
		Length -= (pEid->Len + 2);
	}
}

