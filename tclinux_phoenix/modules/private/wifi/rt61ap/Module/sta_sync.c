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
	sta_sync.c

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
        The sync state machine, 
    Parameters:
        Sm - pointer to the state machine
    Note:
        the state machine looks like the following
    ==========================================================================
 */
VOID StaSyncStateMachineInit(
	IN PRTMP_ADAPTER pAd,
	IN STA_STATE_MACHINE *Sm,
	OUT STA_STATE_MACHINE_FUNC Trans[])
{
	UCHAR i;

	StaStateMachineInit(Sm, (STA_STATE_MACHINE_FUNC*)Trans, STA_MAX_SYNC_STATE, STA_MAX_SYNC_MSG, (STA_STATE_MACHINE_FUNC)StaDrop, STA_SYNC_IDLE, STA_SYNC_MACHINE_BASE);

	// column 1
	StaStateMachineSetAction(Sm, STA_SYNC_IDLE, STA_MT2_MLME_PROBE_REQ, (STA_STATE_MACHINE_FUNC)StaMlmeProbeReqAction);

	//column 2
	StaStateMachineSetAction(Sm, STA_JOIN_WAIT_PROBE_RSP, STA_MT2_MLME_PROBE_REQ, (STA_STATE_MACHINE_FUNC)StaInvalidStateWhenJoin);
	StaStateMachineSetAction(Sm, STA_JOIN_WAIT_PROBE_RSP, STA_MT2_PEER_PROBE_RSP, (STA_STATE_MACHINE_FUNC)StaPeerProbeRspAtJoinAction);
	StaStateMachineSetAction(Sm, STA_JOIN_WAIT_PROBE_RSP, STA_MT2_PROBE_TIMEOUT, (STA_STATE_MACHINE_FUNC)StaProbeTimeoutAtJoinAction);

	// timer init
	RTMPInitTimer(pAd, &pAd->MlmeAux.ProbeTimer, (PVOID)&StaProbeTimeout);

	for (i = 0; i < MAX_APCLI_ENTRY; i++)
		pAd->ApCliTab.ApCliEntry[i].SyncCurrState = STA_SYNC_IDLE;

	return;
}

/* 
    ==========================================================================
    Description:
        Becaon timeout handler, executed in timer thread
    ==========================================================================
 */
VOID StaProbeTimeout(
	IN  unsigned long data) 
{
	RTMP_ADAPTER *pAd = (RTMP_ADAPTER *)data;

	DBGPRINT(RT_DEBUG_TRACE,"STA_SYNC - ProbeReqTimeout\n");

	StaMlmeEnqueue(pAd, STA_SYNC_STATE_MACHINE, STA_MT2_PROBE_TIMEOUT, 0, NULL, 0);
	MlmeHandler(pAd);

	return;
}

/* 
    ==========================================================================
    Description:
        MLME PROBE req state machine procedure
    ==========================================================================
 */
VOID StaMlmeProbeReqAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex) 
{
	MLME_JOIN_REQ_STRUCT *Info = (MLME_JOIN_REQ_STRUCT *)(Elem->Msg);

	DBGPRINT(RT_DEBUG_TRACE, "STA SYNC - StaMlmeProbeReqAction(Ssid %s)\n", Info->Ssid);

	// reset all the timers
	RTMPCancelTimer(&pAd->MlmeAux.ProbeTimer);

	pAd->MlmeAux.Channel = pAd->PortCfg.Channel;
	pAd->MlmeAux.SupRateLen = pAd->PortCfg.SupportedRatesLen;
	NdisMoveMemory(pAd->MlmeAux.SupRate, pAd->PortCfg.SupportedRates, pAd->MlmeAux.SupRateLen);
    
	RTMPAddTimer(&pAd->MlmeAux.ProbeTimer, PROBE_TIMEOUT);
    
	EnqueueProbeRequest(pAd, Info->SsidLen, Info->Ssid, ifIndex);

	DBGPRINT(RT_DEBUG_TRACE, "APCLI SYNC - Start Probe the SSID %s on channel =%d\n", pAd->MlmeAux.Ssid, pAd->MlmeAux.Channel);

	*pCurrState = STA_JOIN_WAIT_PROBE_RSP;

	return;
}

/* 
    ==========================================================================
    Description:
        When waiting joining the (I)BSS, beacon received from external
    ==========================================================================
 */
VOID StaPeerProbeRspAtJoinAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex) 
{
	UCHAR Bssid[MAC_ADDR_LEN], Addr2[MAC_ADDR_LEN];
	UCHAR Ssid[MAX_LEN_OF_SSID], SsidLen, BssType, Channel, MessageToMe, 
		DtimCount, DtimPeriod, BcastFlag; 
	LARGE_INTEGER TimeStamp;
	USHORT BeaconPeriod, AtimWin, CapabilityInfo;
//	UINT FrameLen = 0;
	CF_PARM Cf;
	UCHAR Erp;
	UCHAR SupRate[MAX_LEN_OF_SUPPORTED_RATES], ExtRate[MAX_LEN_OF_SUPPORTED_RATES];
	UCHAR SupRateLen, ExtRateLen;
	UCHAR CkipFlag;
	UCHAR LenVIE;
	UCHAR AironetCellPowerLimit;
	EDCA_PARM EdcaParm;
	QBSS_LOAD_PARM QbssLoad;
	QOS_CAPABILITY_PARM QosCapability;
	UCHAR VarIE[MAX_VIE_LEN];		// Total VIE length = MAX_VIE_LEN - -5
	NDIS_802_11_VARIABLE_IEs *pVIE = NULL;
	ULONG RalinkIe;
	STA_CTRL_MSG_STRUCT StaCtrlMsg;

	// Init Variable IE structure
	pVIE = (PNDIS_802_11_VARIABLE_IEs) VarIE;
	pVIE->Length = 0;
	if(PeerBeaconAndProbeRspSanity(
		pAd, 
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
		&Cf, 
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
		// BEACON from desired BSS/IBSS found. We should be able to decide most
		// BSS parameters here.
		// Q. But what happen if this JOIN doesn't conclude a successful ASSOCIATEION?
		//    Do we need to receover back all parameters belonging to previous BSS?
		// A. Should be not. There's no back-door recover to previous AP. It still need
		//    a new JOIN-AUTH-ASSOC sequence.
		INT ssidEqualFlag = FALSE;
		INT ssidEmptyFlag = FALSE;
		INT bssidEqualFlag = FALSE;
		INT bssidEmptyFlag = FALSE;
		INT matchFlag = FALSE;

		// Check the Probe-Rsp's Bssid.
		if(!MAC_ADDR_EQUAL(pAd->ApCliTab.ApCliEntry[0].CfgApCliBssid, ZERO_MAC_ADDR))
			bssidEqualFlag = MAC_ADDR_EQUAL(pAd->ApCliTab.ApCliEntry[0].CfgApCliBssid, Bssid);
		else
			bssidEmptyFlag = TRUE;

		// Check the Probe-Rsp's Ssid.
		if(pAd->ApCliTab.ApCliEntry[0].CfgSsidLen != 0)
			ssidEqualFlag = SSID_EQUAL(pAd->ApCliTab.ApCliEntry[0].CfgSsid, pAd->ApCliTab.ApCliEntry[0].CfgSsidLen, Ssid, SsidLen);
		else
			ssidEmptyFlag = TRUE;


		// bssid and ssid, Both match.
		if (bssidEqualFlag && ssidEqualFlag)
			matchFlag = TRUE;

		// ssid match but bssid doesn't be indicate.
		else if(ssidEqualFlag && bssidEmptyFlag)
			matchFlag = TRUE;

		// user doesn't indicate any bssid or ssid. AP-Clinet will auto pick a AP to join by most strong siganl strength.
		else if (bssidEmptyFlag && ssidEmptyFlag)
			matchFlag = TRUE;


		DBGPRINT(RT_DEBUG_TRACE, "SYNC - bssidEqualFlag=%d, ssidEqualFlag=%d, matchFlag=%d\n", bssidEqualFlag, ssidEqualFlag, matchFlag);
		if (matchFlag)
		{
			// Validate RSN IE if necessary, then copy store this information
			if (LenVIE > 0
// For AP Client support WPS Modification
#ifdef WSC_SUPPORT
			&& pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.WscConfMode == WSC_DISABLE
#endif // WSC_SUPPORT //
// --
			)
			{
				// Root AP may use mixed mode.  Beacon could contain both WPA and WPA2 RSNIE.
				// In this case, pVIE contains WPAPSK RSNIE.  And
				// (PEID_STRUCT)((PUCHAR)pVIE+2+pVIE->Length) contains WPA2PSK RSNIE
				if (ApCliValidateRSNIE(pAd, (PEID_STRUCT)pVIE))
				{	// the only RSNIE or 1st RSNIE
					// RSN_IE got from AP's Beacon or Probe rsp may contain 2 unicase ciphers TKIP and AES.
					// We can't copy it to ApCli RSNIE directly.
					//pAd->MlmeAux.VarIELen = pVIE->Length + 2;
					//NdisMoveMemory(pAd->MlmeAux.VarIEs, pVIE, pAd->MlmeAux.VarIELen);
					pAd->ApCliTab.ApCliEntry[0].RSN_IELen_from_AP = pVIE->Length + 2;
					NdisMoveMemory(pAd->ApCliTab.ApCliEntry[0].RSN_IE_from_AP, pVIE, pAd->ApCliTab.ApCliEntry[0].RSN_IELen_from_AP);
				}
				else if (((pVIE->Length+2) < LenVIE) && ApCliValidateRSNIE(pAd, (PEID_STRUCT)((PUCHAR)pVIE+2+pVIE->Length)))
				{	// 2nd RSNIE
					// RSN_IE got from AP's Beacon or Probe rsp may contain 2 unicase ciphers TKIP and AES.
					// We can't copy it to ApCli RSNIE directly.
					//pAd->MlmeAux.VarIELen = ((PEID_STRUCT)((PUCHAR)pVIE+2+pVIE->Length))->Len + 2;
					//NdisMoveMemory(pAd->MlmeAux.VarIEs, (PUCHAR)pVIE+2+pVIE->Length, pAd->MlmeAux.VarIELen);
					pAd->ApCliTab.ApCliEntry[0].RSN_IELen_from_AP = ((PEID_STRUCT)((PUCHAR)pVIE+2+pVIE->Length))->Len + 2;
					NdisMoveMemory(pAd->ApCliTab.ApCliEntry[0].RSN_IE_from_AP, (PUCHAR)pVIE+2+pVIE->Length, pAd->ApCliTab.ApCliEntry[0].RSN_IELen_from_AP);
				}
				else
				{
					// ignore this response
					pAd->MlmeAux.VarIELen = 0;
					printk("ERROR: The RSN IE of this received Probe-resp is dis-match !!!!!!!!!! \n");
					return;
				}
			}
			else
			{
				if (pAd->ApCliTab.ApCliEntry[0].AuthMode >= Ndis802_11AuthModeWPA
// For AP Client support WPS Modification
#ifdef WSC_SUPPORT
				&& pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.WscConfMode == WSC_DISABLE
#endif // WSC_SUPPORT //
// --
				)
				{
					// ignore this response
					printk("ERROR: The received Probe-resp has empty RSN IE !!!!!!!!!! \n");
					return;
				}	
				
				pAd->MlmeAux.VarIELen = 0;
			}
			
#ifdef RTL865X_SOC
			printk("SYNC - receive desired PROBE_RSP at JoinWaitProbeRsp... Channel = %d\n", Channel);
#else
			DBGPRINT(RT_DEBUG_TRACE, "SYNC - receive desired PROBE_RSP at JoinWaitProbeRsp... Channel = %d\n", Channel);
#endif			

			// if the Bssid doesn't be indicated then you need to decide which AP to connect by most strong Rssi signal strength.
			if (bssidEqualFlag == FALSE)
			{
				LONG RealRssi=0;

				// caculate real rssi value.
				RealRssi = (LONG)(ConvertToRssi(pAd, Elem->Rssi, RSSI_NO_1) + pAd->BbpRssiToDbmDelta);

				DBGPRINT(RT_DEBUG_TRACE, "SYNC - previous Rssi = %d current Rssi=%d\n", pAd->MlmeAux.Rssi, RealRssi);
				if (pAd->MlmeAux.Rssi > (LONG)RealRssi)
					return;
				else
					pAd->MlmeAux.Rssi = RealRssi;
			} else
			{
				RTMPCancelTimer(&pAd->MlmeAux.ProbeTimer);
			}

			NdisMoveMemory(pAd->MlmeAux.Ssid, Ssid, SsidLen);
			pAd->MlmeAux.SsidLen = SsidLen;

			NdisMoveMemory(pAd->MlmeAux.Bssid, Bssid, MAC_ADDR_LEN);
			pAd->MlmeAux.CapabilityInfo = CapabilityInfo & pAd->PortCfg.MBSSID[MAIN_MBSSID].CapabilityInfo;
			pAd->MlmeAux.BssType = BssType;
			pAd->MlmeAux.BeaconPeriod = BeaconPeriod;
			pAd->MlmeAux.Channel = Channel;
			pAd->MlmeAux.AtimWin = AtimWin;
			pAd->MlmeAux.CfpPeriod = Cf.CfpPeriod;
			pAd->MlmeAux.CfpMaxDuration = Cf.CfpMaxDuration;
			pAd->MlmeAux.APRalinkIe = RalinkIe;

			// Copy AP's supported rate to MlmeAux for creating assoication request
			// Also filter out not supported rate
			pAd->MlmeAux.SupRateLen = SupRateLen;
			NdisMoveMemory(pAd->MlmeAux.SupRate, SupRate, SupRateLen);
			RTMPCheckRates(pAd, pAd->MlmeAux.SupRate, &pAd->MlmeAux.SupRateLen);
			pAd->MlmeAux.ExtRateLen = ExtRateLen;
			NdisMoveMemory(pAd->MlmeAux.ExtRate, ExtRate, ExtRateLen);
			RTMPCheckRates(pAd, pAd->MlmeAux.ExtRate, &pAd->MlmeAux.ExtRateLen);


			// copy QOS related information
			if(pAd->PortCfg.MBSSID[MAIN_MBSSID].bWmmCapable)
			{
				NdisMoveMemory(&pAd->MlmeAux.APEdcaParm, &EdcaParm, sizeof(EDCA_PARM));
				NdisMoveMemory(&pAd->MlmeAux.APQbssLoad, &QbssLoad, sizeof(QBSS_LOAD_PARM));
				NdisMoveMemory(&pAd->MlmeAux.APQosCapability, &QosCapability, sizeof(QOS_CAPABILITY_PARM));
			} else
			{
				NdisZeroMemory(&pAd->MlmeAux.APEdcaParm, sizeof(EDCA_PARM));
				NdisZeroMemory(&pAd->MlmeAux.APQbssLoad, sizeof(QBSS_LOAD_PARM));
				NdisZeroMemory(&pAd->MlmeAux.APQosCapability, sizeof(QOS_CAPABILITY_PARM));
			}

			DBGPRINT(RT_DEBUG_TRACE, "APCLI SYNC - after JOIN, SupRateLen=%d, ExtRateLen=%d\n", 
				pAd->MlmeAux.SupRateLen, pAd->MlmeAux.ExtRateLen);

			if(bssidEqualFlag == TRUE)
			{
				*pCurrState = STA_SYNC_IDLE;

				StaCtrlMsg.Status = MLME_SUCCESS;
				StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_PROBE_RSP,
					sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);
			}
		}
		// not to me BEACON, ignored
	}
	// sanity check fail, ignore this frame

	return;
}

VOID StaProbeTimeoutAtJoinAction(
	IN PRTMP_ADAPTER pAd,
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex) 
{
	STA_CTRL_MSG_STRUCT StaCtrlMsg;

	DBGPRINT(RT_DEBUG_TRACE, "STA_SYNC - ProbeTimeoutAtJoinAction\n");
	*pCurrState = SYNC_IDLE;

	DBGPRINT(RT_DEBUG_TRACE, "STA_SYNC - MlmeAux.Bssid=%02x:%02x:%02x:%02x:%02x:%02x\n",
		pAd->MlmeAux.Bssid[0], pAd->MlmeAux.Bssid[1], pAd->MlmeAux.Bssid[2], pAd->MlmeAux.Bssid[3], pAd->MlmeAux.Bssid[4], pAd->MlmeAux.Bssid[5]);

	if(!MAC_ADDR_EQUAL(pAd->MlmeAux.Bssid, ZERO_MAC_ADDR))
	{
		StaCtrlMsg.Status = MLME_SUCCESS;
		StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_PROBE_RSP,
			sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);
	} else
	{
		StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_JOIN_REQ_TIMEOUT, 0, NULL, ifIndex);
	}

	return;
}

/* 
    ==========================================================================
    Description:
    ==========================================================================
 */
VOID StaInvalidStateWhenJoin(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem,
	OUT PULONG pCurrState,
	IN USHORT ifIndex) 
{
	STA_CTRL_MSG_STRUCT StaCtrlMsg;

	StaCtrlMsg.Status = MLME_STATE_MACHINE_REJECT;
	StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_PROBE_RSP,
		sizeof(STA_CTRL_MSG_STRUCT), &StaCtrlMsg, ifIndex);

	DBGPRINT(RT_DEBUG_TRACE, "STA_AYNC - StaInvalidStateWhenJoin(state=%d). Reset SYNC machine\n", *pCurrState);

	return;
}

/* 
	==========================================================================
	Description:
	==========================================================================
 */
VOID EnqueueProbeRequest(
	IN PRTMP_ADAPTER pAd,
	IN UCHAR SsidLen,
	OUT PCHAR Ssid,
	IN USHORT ifIndex)
{
	PUCHAR          pOutBuffer;
	ULONG           FrameLen = 0;
	HEADER_802_11   Hdr80211;
	UCHAR           SsidIe    = IE_SSID;
	UCHAR           SupRateIe = IE_SUPP_RATES;
	UCHAR ssidLen;
	CHAR ssid[MAX_LEN_OF_SSID];

	DBGPRINT(RT_DEBUG_TRACE, "force out a ProbeRequest ...\n");

	pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);

	if(pOutBuffer == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, "EnqueueProbeRequest() allocate memory fail\n");
		return;
	} else
	{
		if(MAC_ADDR_EQUAL(pAd->ApCliTab.ApCliEntry[ifIndex].CfgApCliBssid, ZERO_MAC_ADDR))
			ApCliMgtMacHeaderInit(pAd, &Hdr80211, SUBTYPE_PROBE_REQ, 0,
				BROADCAST_ADDR, BROADCAST_ADDR, ifIndex);
		else
			ApCliMgtMacHeaderInit(pAd, &Hdr80211, SUBTYPE_PROBE_REQ, 0,
				pAd->ApCliTab.ApCliEntry[ifIndex].CfgApCliBssid, pAd->ApCliTab.ApCliEntry[ifIndex].CfgApCliBssid, ifIndex);

		ssidLen = SsidLen;
		NdisZeroMemory(ssid, MAX_LEN_OF_SSID);
		NdisMoveMemory(ssid, Ssid, ssidLen);

		// this ProbeRequest explicitly specify SSID to reduce unwanted ProbeResponse
		MakeOutgoingFrame(pOutBuffer,                     &FrameLen,
			sizeof(HEADER_802_11),          &Hdr80211,
			1,                              &SsidIe,
			1,                              &ssidLen,
			ssidLen,                        ssid,
			1,                              &SupRateIe,
			1,                              &pAd->MlmeAux.SupRateLen,
			pAd->MlmeAux.SupRateLen,        pAd->MlmeAux.SupRate, 
			END_OF_ARGS);

#if 0
		// add Simple Config Information Element
		if(pAd->PortCfg.bWscCapable && pAd->PortCfg.WscIEProbeReq.ValueLen)
		{
			ULONG WscTmpLen = 0;

			MakeOutgoingFrame(pOutBuffer+FrameLen,        &WscTmpLen,
				pAd->PortCfg.WscIEProbeReq.ValueLen,   pAd->PortCfg.WscIEProbeReq.Value,
				END_OF_ARGS);
			FrameLen += WscTmpLen;
		}
#endif
		if(pAd->PortCfg.bGetAPConfig)
		{
			UCHAR RalinkSpecificIEForGetCfg[6] = {IE_VENDOR_SPECIFIC, 4, 0x00, 0x0c, 0x43, 0x80};
			ULONG Tmp = 0;
			MakeOutgoingFrame(pOutBuffer + FrameLen,        &Tmp,
				6,                            RalinkSpecificIEForGetCfg,
				END_OF_ARGS);
			FrameLen += Tmp;
		}
// For AP Client support WPS Modification
#ifdef WSC_SUPPORT
		// Append WSC information in probe request if WSC state is running
		if (pAd->ApCliTab.ApCliEntry[ifIndex].WscControl.WscState != WSC_STATE_OFF)
		{
			UCHAR		WscBuf[256], WscIeLen;
			ULONG		Tmp2;
    
			WscMakeProbeReqIE(pAd, &WscBuf[0], &WscIeLen);
			MakeOutgoingFrame(pOutBuffer + FrameLen, &Tmp2, WscIeLen, WscBuf, END_OF_ARGS);
    
			FrameLen += Tmp2;
		}
#endif // WSC_SUPPORT //
// --
		MiniportMMRequest(pAd, pOutBuffer, FrameLen);
		kfree(pOutBuffer);
	}

	return;
}

