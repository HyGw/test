/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2005, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    wpa.c

    Abstract:

    Revision History:
    Who         When            What
    --------    ----------      ----------------------------------------------
    Jan Lee     03-07-22        Initial
*/
#include "rt_config.h"

static UCHAR       OUI_WPA_WEP40[4]		= {0x00, 0x50, 0xF2, 0x01};
static UCHAR       OUI_WPA_TKIP[4]		= {0x00, 0x50, 0xF2, 0x02};
static UCHAR       OUI_WPA_CCMP[4]		= {0x00, 0x50, 0xF2, 0x04};
static UCHAR       OUI_WPA2_WEP40[4]		= {0x00, 0x0F, 0xAC, 0x01};
static UCHAR       OUI_WPA2_TKIP[4]		= {0x00, 0x0F, 0xAC, 0x02};
static UCHAR       OUI_WPA2_CCMP[4]		= {0x00, 0x0F, 0xAC, 0x04};
static UCHAR	   prf_input[1024];

#ifdef RTL865X_SOC
static inline VOID	ApCliWpaMacHeaderInit(
	IN		PRTMP_ADAPTER	pAd, 
	IN OUT	PHEADER_802_11	pHdr80211, 
	IN		UCHAR			wep, 
	IN		PUCHAR		    pAddr1,
	IN		PUCHAR		    pAddr2);

static inline VOID    Wpa2PairMsg3Action(
    IN  PRTMP_ADAPTER   pAd, 
    IN  MLME_QUEUE_ELEM *Elem,
	IN	INT				IfIndex);

static inline BOOLEAN ParseKeyData(
    IN  PRTMP_ADAPTER   pAd,
    IN  PUCHAR          pKeyData,
    IN  UCHAR           KeyDataLen,
	IN	INT				IfIndex,
	IN	UCHAR			bPairewise);	
#endif	/* endif ifdef RTL865X_SOC */

#define ROUND_UP(__x, __y) \
	(((UMAXLONG)((__x)+((__y)-1))) & ((UMAXLONG)~((__y)-1)))\

BOOLEAN WPAMsgTypeSubst(
    IN UCHAR    EAPType,
    OUT ULONG   *MsgType) 
{
	DBGPRINT(RT_DEBUG_TRACE, "WPAMsgTypeSubst (EAPType=%d)\n", EAPType);
	
    switch(EAPType)
    {
        case EAPPacket:
            *MsgType = MACHINE_TYPE_EAPPacket;
            break;

        case EAPOLStart:
            *MsgType = MACHINE_TYPE_EAPOLStart;
            break;

        case EAPOLLogoff:
            *MsgType = MACHINE_TYPE_EAPOLLogoff;
            break;

        case EAPOLKey:
            *MsgType = MACHINE_TYPE_EAPOLKey;
            break;

        case EAPOLASFAlert:
            *MsgType = MACHINE_TYPE_EAPOLASFAlert;
            break;

        default:
            DBGPRINT(RT_DEBUG_TRACE, "Unknown WPA MsgType\n");
            return FALSE;
    }
    
    return TRUE;
}

/*  
    ==========================================================================
    Description: 
        association state machine init, including state transition and timer init
    Parameters: 
        S - pointer to the association state machine
    ==========================================================================
 */
VOID WpaStateMachineInit(
    IN  PRTMP_ADAPTER       pAd,
    IN  STATE_MACHINE       *S,
    OUT STATE_MACHINE_FUNC  Trans[])
{
    StateMachineInit(S, (STATE_MACHINE_FUNC*)Trans, MAX_WPA_PTK_STATE, MAX_WPA_MSG, (STATE_MACHINE_FUNC)Drop, WPA_PTK, WPA_MACHINE_BASE);

    StateMachineSetAction(S, WPA_PTK, MACHINE_TYPE_EAPPacket, (STATE_MACHINE_FUNC)WpaEAPPacketAction);
    StateMachineSetAction(S, WPA_PTK, MACHINE_TYPE_EAPOLStart, (STATE_MACHINE_FUNC)WpaEAPOLStartAction);
    StateMachineSetAction(S, WPA_PTK, MACHINE_TYPE_EAPOLLogoff, (STATE_MACHINE_FUNC)WpaEAPOLLogoffAction);
    StateMachineSetAction(S, WPA_PTK, MACHINE_TYPE_EAPOLKey, (STATE_MACHINE_FUNC)WpaEAPOLKeyAction);
    StateMachineSetAction(S, WPA_PTK, MACHINE_TYPE_EAPOLASFAlert, (STATE_MACHINE_FUNC)WpaEAPOLASFAlertAction);
}

/*
    ==========================================================================
    Description:
        Port Access Control Inquiry function. Return entry's Privacy and Wpastate.
        Also check if AP needs to initilize 4-way handshake. 
    Return:
        pEntry 
    ==========================================================================
*/
MAC_TABLE_ENTRY *PACInquiry(
    IN PRTMP_ADAPTER                pAd,
    IN PUCHAR                       pAddr,
    OUT NDIS_802_11_PRIVACY_FILTER  *Privacy,
    OUT WPA_STATE                   *WpaState)
{
    MAC_TABLE_ENTRY *pEntry = (MAC_TABLE_ENTRY*)NULL;
    
    *Privacy = Ndis802_11PrivFilterAcceptAll;
    *WpaState = AS_NOTUSE;

    if (MAC_ADDR_IS_GROUP(pAddr))
    {// mcast & broadcast address
    } 
    else
    {// unicast address
        pEntry = MacTableLookup(pAd, pAddr);
        if (pEntry) 
        {
            *Privacy = pEntry->PrivacyFilter;
            *WpaState = pEntry->WpaState;
        } 
    }
    return pEntry;
}

/*
    ==========================================================================
    Description:
       
    Return:
         TRUE if this is EAP frame
         FALSE otherwise
    ==========================================================================
*/
BOOLEAN RTMPCheckWPAframe(
    IN PRTMP_ADAPTER    pAdapter,
    IN PUCHAR           pHeader,
    IN ULONG            DataByteCount,
    IN UCHAR            Offset)
{
    PUCHAR  pData;
    
    DBGPRINT(RT_DEBUG_INFO, "RTMPCheckWPAframe ===>\n");
    
    if (DataByteCount < (LENGTH_802_11 + LENGTH_802_1_H + LENGTH_EAPOL_H + Offset))
        return FALSE;

    pData = pHeader + LENGTH_802_11 + Offset;
    DataByteCount -= (LENGTH_802_11 + Offset);
    if (RTMPEqualMemory(SNAP_802_1H, pData, 6)) 
    {
        pData += 6;
    }
    if (RTMPEqualMemory(EAPOL, pData, 2)) 
    {
        pData += 2;
    }
    else
        return FALSE;

    switch (*(pData+1))
    {   
        case EAPPacket:
            DBGPRINT(RT_DEBUG_INFO, "Receive EAP-Packet frame, TYPE = 0 \n");
            break;

        case EAPOLStart:
            DBGPRINT(RT_DEBUG_INFO, "Receive EAPOL-Start frame, TYPE = 1 \n");
            break;

        case EAPOLLogoff:
            DBGPRINT(RT_DEBUG_INFO, "Receive EAPOLLogoff frame, TYPE = 2 \n");
            break;

        case EAPOLKey:
            DBGPRINT(RT_DEBUG_INFO, "Receive EAPOL-Key frame, TYPE = 3, Length =%x\n", *(pData+2));
            break;

        case EAPOLASFAlert:
            DBGPRINT(RT_DEBUG_INFO, "Receive EAPOLASFAlert frame, TYPE = 4 \n");
            break;

        default:
            return FALSE;
    }   
    return TRUE;
}

/*
    ==========================================================================
    Description:
       Check invalidity of multicast cipher selection in RSN IE.
    Return:
         TRUE if match
         FALSE otherwise
    ==========================================================================
*/
BOOLEAN RTMPCheckMcast(
    IN PRTMP_ADAPTER    pAdapter,
    IN PEID_STRUCT      eid_ptr,
    IN MAC_TABLE_ENTRY	*pEntry)
{
	pEntry->AuthMode = pAdapter->PortCfg.MBSSID[pEntry->ApIdx].AuthMode;

    if (eid_ptr->Len >= 6)
    {
    	// WPA and WPA2 format not the same in RSN_IE
    	if (eid_ptr->Eid == IE_WPA)
    	{
    		if (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].AuthMode == Ndis802_11AuthModeWPA1WPA2)
    			pEntry->AuthMode = Ndis802_11AuthModeWPA;
    		else if (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].AuthMode == Ndis802_11AuthModeWPAPSKWPA2PSK)
    			pEntry->AuthMode = Ndis802_11AuthModeWPAPSK;

	        if (RTMPEqualMemory(&eid_ptr->Octet[6], &pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSN_IE[0][6], 4))
	            return TRUE;
    	}
    	else if (eid_ptr->Eid == IE_WPA2)
    	{
    		UCHAR	IE_Idx = 0;

			// When WPA1/WPA2 mix mode, the RSN_IE is stored in different structure
			if ((pAdapter->PortCfg.MBSSID[pEntry->ApIdx].AuthMode == Ndis802_11AuthModeWPA1WPA2) || (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].AuthMode == Ndis802_11AuthModeWPAPSKWPA2PSK))
				IE_Idx = 1;
	
    		if (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].AuthMode == Ndis802_11AuthModeWPA1WPA2)
    			pEntry->AuthMode = Ndis802_11AuthModeWPA2;
    		else if (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].AuthMode == Ndis802_11AuthModeWPAPSKWPA2PSK)
    			pEntry->AuthMode = Ndis802_11AuthModeWPA2PSK;

    		if (RTMPEqualMemory(&eid_ptr->Octet[2], &pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSN_IE[IE_Idx][2], 4))
	            return TRUE;
    	}
    }

    DBGPRINT(RT_DEBUG_ERROR, "RTMPCheckMcast ==> WPAIE parsing error, WPAIE=%d\n", eid_ptr->Eid);

    return FALSE;
}

/*
    ==========================================================================
    Description:
       Check invalidity of unicast cipher selection in RSN IE.
    Return:
         TRUE if match
         FALSE otherwise
    ==========================================================================
*/
BOOLEAN RTMPCheckUcast(
    IN PRTMP_ADAPTER    pAdapter,
    IN PEID_STRUCT      eid_ptr,
    IN MAC_TABLE_ENTRY	*pEntry)
{
	PUCHAR 	pStaTmp;
	USHORT	Count;

	pEntry->WepStatus = pAdapter->PortCfg.MBSSID[pEntry->ApIdx].WepStatus;
    
	if (eid_ptr->Len < 16)
	{
		DBGPRINT(RT_DEBUG_ERROR, "RTMPCheckUcast ==> WPAIE len is too short(%d) \n", eid_ptr->Len);
	    return FALSE;
	}	

	// Store STA RSN_IE capability
	pStaTmp = &eid_ptr->Octet[0];
	if(eid_ptr->Eid == IE_WPA2)
	{
		// skip Version(2),Multicast cipter(4) 2+4==6
		// point to number of unicast
        pStaTmp +=6;
	}
	else if (eid_ptr->Eid == IE_WPA)	
	{
		// skip OUI(4),Vesrion(2),Multicast cipher(4) 4+2+4==10
		// point to number of unicast
        pStaTmp += 10;
	}
	else
	{
		DBGPRINT(RT_DEBUG_ERROR, "RTMPCheckUcast ==> Unknown WPAIE, WPAIE=%d\n", eid_ptr->Eid);
	    return FALSE;
	}

	// Store unicast cipher count
    NdisMoveMemory(&Count, pStaTmp, sizeof(USHORT));
#ifdef BIG_ENDIAN
    Count = SWAP16(Count);		
#endif

	// pointer to unicast cipher
    pStaTmp += sizeof(USHORT);	
			
    if (eid_ptr->Len >= 16)
    {
    	if (eid_ptr->Eid == IE_WPA)
    	{
    		if (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].WepStatus == Ndis802_11Encryption4Enabled)
			{// multiple cipher (TKIP/CCMP)

				while (Count > 0)
				{
					// TKIP
					if (RTMPEqualMemory(pStaTmp /*&eid_ptr->Octet[12]*/, &pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSN_IE[0][12], 4))
					{
						pEntry->WepStatus = Ndis802_11Encryption2Enabled;
						return TRUE;
					}
					// AES
					else if (RTMPEqualMemory(pStaTmp /*&eid_ptr->Octet[12]*/, &pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSN_IE[0][16], 4))
					{
						pEntry->WepStatus = Ndis802_11Encryption3Enabled;
						return TRUE;
					}
					pStaTmp += 4;
					Count--;
				}
    		}
    		else
    		{// single cipher
    			while (Count > 0)
    			{
    				if (RTMPEqualMemory(pStaTmp /*&eid_ptr->Octet[12]*/, &pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSN_IE[0][12], 4))
		            	return TRUE;

					pStaTmp += 4;
					Count--;
				}
    		}
    	}
    	else if (eid_ptr->Eid == IE_WPA2)
    	{
    		UCHAR	IE_Idx = 0;

			// When WPA1/WPA2 mix mode, the RSN_IE is stored in different structure
			if ((pAdapter->PortCfg.MBSSID[pEntry->ApIdx].AuthMode == Ndis802_11AuthModeWPA1WPA2) || (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].AuthMode == Ndis802_11AuthModeWPAPSKWPA2PSK))
				IE_Idx = 1;
	
			if (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].WepStatus == Ndis802_11Encryption4Enabled)
			{// multiple cipher (TKIP/CCMP)

				while (Count > 0)
    			{
    				// TKIP
					if (RTMPEqualMemory(pStaTmp /*&eid_ptr->Octet[8]*/, &pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSN_IE[IE_Idx][8], 4))
					{
						pEntry->WepStatus = Ndis802_11Encryption2Enabled;
						return TRUE;
					}
					// AES
					else if (RTMPEqualMemory(pStaTmp /*&eid_ptr->Octet[8]*/, &pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSN_IE[IE_Idx][12], 4))
					{
						pEntry->WepStatus = Ndis802_11Encryption3Enabled;
						return TRUE;
					}
					pStaTmp += 4;
					Count--;
				}
			}
			else
			{// single cipher
				while (Count > 0)
    			{
					if (RTMPEqualMemory(pStaTmp /*&eid_ptr->Octet[8]*/, &pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSN_IE[IE_Idx][8], 4))
						return TRUE;

					pStaTmp += 4;
					Count--;
				}
			}
    	}
    }

    DBGPRINT(RT_DEBUG_ERROR, "RTMPCheckUcast ==> WPAIE parsing error, WPAIE=%d\n", eid_ptr->Eid);

    return FALSE;
}

/*
    ==========================================================================
    Description:
       Check invalidity of authentication method selection in RSN IE.
    Return:
         TRUE if match
         FALSE otherwise
    ==========================================================================
*/
BOOLEAN RTMPCheckAKM(PUCHAR sta_akm, PUCHAR ap_rsn_ie, INT iswpa2)
{
	PUCHAR pTmp;
	USHORT Count;

	//pTmp = &pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSN_IE[0][0];
	//rtmp_hexdump(RT_DEBUG_TRACE,"MBSS WPA_IE  ",ap_rsn_ie,24);
	//rtmp_hexdump(RT_DEBUG_TRACE,"MBSS STA AKM ",sta_akm,4);

	pTmp = ap_rsn_ie;

	if(iswpa2)
    // skip Version(2),Multicast cipter(4) 2+4==6
        pTmp +=6;
    else	
    //skip OUI(4),Vesrion(2),Multicast cipher(4) 4+2+4==10
        pTmp += 10;//point to number of unicast
	    
    NdisMoveMemory(&Count, pTmp, sizeof(USHORT));	
#ifdef BIG_ENDIAN
    Count = SWAP16(Count);		
#endif
    pTmp   += sizeof(USHORT);//pointer to unicast cipher

    // Skip all unicast cipher suite
    while (Count > 0)
    	{
		// Skip OUI
		pTmp += 4;
		Count--;
	}

	NdisMoveMemory(&Count, pTmp, sizeof(USHORT));
#ifdef BIG_ENDIAN
    Count = SWAP16(Count);		
#endif
    pTmp   += sizeof(USHORT);//pointer to AKM cipher
    while (Count > 0)
    {
		//rtmp_hexdump(RT_DEBUG_TRACE,"MBSS WPA_IE AKM ",pTmp,4);
		if(RTMPEqualMemory(sta_akm,pTmp,4))
		   return TRUE;
    	else
		{
			pTmp += 4;
			Count--;
		}
    }
    return FALSE;// do not match the AKM   	

}

/*
    ==========================================================================
    Description:
       Check invalidity of authentication method selection in RSN IE.
    Return:
         TRUE if match
         FALSE otherwise
    ==========================================================================
*/
BOOLEAN RTMPCheckAUTH(
    IN PRTMP_ADAPTER    pAdapter,
    IN PEID_STRUCT      eid_ptr,
    IN MAC_TABLE_ENTRY	*pEntry)
{
	PUCHAR pStaTmp;
	USHORT Count;	

	if (eid_ptr->Len < 16)
	{
		DBGPRINT(RT_DEBUG_ERROR, "RTMPCheckAUTH ==> WPAIE len is too short(%d) \n", eid_ptr->Len);
	    return FALSE;
	}	

	// Store STA RSN_IE capability
	pStaTmp = &eid_ptr->Octet[0];
	if(eid_ptr->Eid == IE_WPA2)
	{
		// skip Version(2),Multicast cipter(4) 2+4==6
		// point to number of unicast
        pStaTmp +=6;
	}
	else if (eid_ptr->Eid == IE_WPA)	
	{
		// skip OUI(4),Vesrion(2),Multicast cipher(4) 4+2+4==10
		// point to number of unicast
        pStaTmp += 10;
	}
	else
	{
		DBGPRINT(RT_DEBUG_ERROR, "RTMPCheckAUTH ==> Unknown WPAIE, WPAIE=%d\n", eid_ptr->Eid);
	    return FALSE;
	}

	// Store unicast cipher count
    NdisMoveMemory(&Count, pStaTmp, sizeof(USHORT));
#ifdef BIG_ENDIAN
    Count = SWAP16(Count);		
#endif

	// pointer to unicast cipher
    pStaTmp += sizeof(USHORT);	

    // Skip all unicast cipher suite
    while (Count > 0)
    {
		// Skip OUI
		pStaTmp += 4;
		Count--;
	}

	// Store AKM count
	NdisMoveMemory(&Count, pStaTmp, sizeof(USHORT));
#ifdef BIG_ENDIAN
    Count = SWAP16(Count);		
#endif

	//pointer to AKM cipher
    pStaTmp += sizeof(USHORT);			

    if (eid_ptr->Len >= 16)
    {
    	if (eid_ptr->Eid == IE_WPA)
    	{
			while (Count > 0)
			{
				if (RTMPCheckAKM(pStaTmp /*&eid_ptr->Octet[18]*/,&pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSN_IE[0][0],0))
					return TRUE;
	
				pStaTmp += 4;
				Count--;
			}
    	}
    	else if (eid_ptr->Eid == IE_WPA2)
    	{
    		UCHAR	IE_Idx = 0;

			// When WPA1/WPA2 mix mode, the RSN_IE is stored in different structure
			if ((pAdapter->PortCfg.MBSSID[pEntry->ApIdx].AuthMode == Ndis802_11AuthModeWPA1WPA2) || (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].AuthMode == Ndis802_11AuthModeWPAPSKWPA2PSK))
				IE_Idx = 1;

			while (Count > 0)
			{			
				if (RTMPCheckAKM(pStaTmp /*&eid_ptr->Octet[14]*/,&pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSN_IE[IE_Idx][0],1))
					return TRUE;
			
				pStaTmp += 4;
				Count--;
			}
    	}
    }

    DBGPRINT(RT_DEBUG_ERROR, "RTMPCheckAUTH ==> WPAIE parsing error, WPAIE=%d\n", eid_ptr->Eid);

    return FALSE;
}

/*
    ==========================================================================
    Description:
        this is state machine function. 
        When receiving EAP packets which is  for 802.1x authentication use. 
        Not use in PSK case
    Return:
    ==========================================================================
*/
VOID WpaEAPPacketAction(
    IN PRTMP_ADAPTER    pAd, 
    IN MLME_QUEUE_ELEM  *Elem) 
{
}

VOID WpaEAPOLLogoffAction(
    IN PRTMP_ADAPTER	pAd, 
    IN MLME_QUEUE_ELEM	*Elem) 
{
}

VOID WpaEAPOLASFAlertAction(
    IN PRTMP_ADAPTER    pAd, 
    IN MLME_QUEUE_ELEM  *Elem) 
{   
}

VOID WpaEAPOLStartAction(
    IN PRTMP_ADAPTER    pAd, 
    IN MLME_QUEUE_ELEM  *Elem) 
{
    MAC_TABLE_ENTRY     *pEntry;
    PHEADER_802_11      pHeader;

    DBGPRINT(RT_DEBUG_TRACE, "WpaEAPOLStartAction ====>> \n");
    
    // Delay for avoiding the misorder of EAPOL-KEY and Assoc Rsp.
    //RTMPusecDelay(300000);
    
    pHeader = (PHEADER_802_11)Elem->Msg;
    // TODO: For normaol PSK, we enqueue an EAPOL-Start command to trigger the process.
    if (Elem->MsgLen == 6)
        pEntry = MacTableLookup(pAd, Elem->Msg);
    else
    {
	    pEntry = MacTableLookup(pAd, pHeader->Addr2);
#ifdef WSC_SUPPORT
        /* 
            a WSC enabled AP must ignore EAPOL-Start frames received from clients that associated to 
            the AP with an RSN IE or SSN IE indicating a WPA2-PSK/WPA-PSK authentication method in 
            the assication request.  <<from page52 in Wi-Fi Simple Config Specification version 1.0g>>
        */
        if (pEntry && 
            (pEntry->ApIdx == MAIN_MBSSID) &&
            (pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.WscConfMode != WSC_DISABLE) &&
            ((pEntry->AuthMode == Ndis802_11AuthModeWPAPSK) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2PSK)))
        {
            DBGPRINT(RT_DEBUG_TRACE, "WPS enabled AP: Ignore EAPOL-Start frames received from clients.\n");
            return;
        }
#endif // WSC_SUPPORT //
    }
    
    if (pEntry) 
    {
        // TODO: If WPA-PSK mode and port not secured, reset to the init PSK state
        if ((pEntry->PortSecured == WPA_802_1X_PORT_NOT_SECURED)
        	&& ((pEntry->AuthMode == Ndis802_11AuthModeWPAPSK) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2PSK) || ((pEntry->AuthMode == Ndis802_11AuthModeWPA2) && (pEntry->PMKID_CacheIdx != ENTRY_NOT_FOUND))))
        {
            pEntry->PrivacyFilter = Ndis802_11PrivFilter8021xWEP;
            pEntry->WpaState = AS_INITPSK;
            pEntry->PortSecured = WPA_802_1X_PORT_NOT_SECURED;
            NdisZeroMemory(pEntry->R_Counter, sizeof(pEntry->R_Counter));
            
            // add timer for retry
            pEntry->RetryTimer.expires = jiffies + WPA_RETRY_EXEC_INTV;
            pEntry->ReTryCounter = PEER_MSG1_RETRY_TIMER_CTR;
            WPAStart4WayHS(pAd, pEntry);
        }
    } 
}

/*
    ==========================================================================
    Description:
        Function to handel countermeasures active attack.  Init 60-sec timer if necessary.
    Return:
    ==========================================================================
*/
VOID HandleCounterMeasure(
    IN PRTMP_ADAPTER    pAd, 
    IN MAC_TABLE_ENTRY  *pEntry) 
{
    INT         i;

    do{
		if (!pEntry)
			break;

		// if entry not set key done, ignore this RX MIC ERROR
	    if ((pEntry->WpaState < AS_PTKINITDONE) || (pEntry->GTKState != REKEY_ESTABLISHED))
	        break;
			
	    DBGPRINT(RT_DEBUG_TRACE, "HandleCounterMeasure ===> \n");

		pAd->PortCfg.MICFailureCounter++;
		// record which entry causes this MIC error, if this entry sends disauth/disassoc, AP doesn't need to log the CM
		pEntry->CMTimerRunning = TRUE;
		if (pAd->PortCfg.CMTimerRunning == TRUE)
		{
			DBGPRINT(RT_DEBUG_TRACE, "Receive CM Attack Twice Within 60 seconds ====>>> \n");            

			ApLogEvent(pAd, pEntry->Addr, EVENT_COUNTER_M, pEntry->ApIdx);
			RTMPCancelTimer(&pAd->PortCfg.CounterMeasureTimer);
			pAd->PortCfg.CMTimerRunning = FALSE;
			// happened twice within 60 sec,  AP SENDS disaccociate all associated STAs.  All STA's transition to State 2
			// renew GTK
			GenRandom(pAd, pAd->PortCfg.MBSSID[pEntry->ApIdx].GNonce,pEntry->ApIdx);
			for (i = 0; i < MAX_LEN_OF_MAC_TABLE; i++)
			{
				if (pAd->MacTab.Content[i].Valid == TRUE)
				{
					DisAssocAction(pAd, &pAd->MacTab.Content[i], SUBTYPE_DISASSOC, REASON_MIC_FAILURE);
				}
			}

			// Further,  ban all Class 3 DATA transportation for  a period 0f 60 sec
			// disallow new association , too
			pAd->PortCfg.MBSSID[pEntry->ApIdx].BANClass3Data = TRUE;        
		}
	}while(FALSE);

	RTMPAddTimer(&pAd->PortCfg.CounterMeasureTimer, 60*MLME_TASK_EXEC_INTV);
    pAd->PortCfg.CMTimerRunning = TRUE;
    pAd->PortCfg.PrevaMICFailTime = pAd->PortCfg.aMICFailTime;
    pAd->PortCfg.aMICFailTime = jiffies;
}

/*
    ==========================================================================
    Description:
        This is state machine function. 
        When receiving EAPOL packets which is  for 802.1x key management. 
        Use both in WPA, and WPAPSK case. 
        In this function, further dispatch to different functions according to the received packet.  3 categories are : 
          1.  normal 4-way pairwisekey and 2-way groupkey handshake
          2.  MIC error (Countermeasures attack)  report packet from STA.
          3.  Request for pairwise/group key update from STA
    Return:
    ==========================================================================
*/
VOID WpaEAPOLKeyAction(
    IN PRTMP_ADAPTER    pAdapter, 
    IN MLME_QUEUE_ELEM  *Elem) 
{
    INT                 i;
    MAC_TABLE_ENTRY     *pEntry;
	KEY_DESCRIPTER		*pKeyDesc;
    PHEADER_802_11      pHeader;
    PEAPOL_PACKET       pMsg;
    UCHAR               Offset = 0;

    DBGPRINT(RT_DEBUG_TRACE, "WpaEAPOLKeyAction ===> \n");

    pHeader = (PHEADER_802_11)Elem->Msg;

    if (pHeader->FC.SubType & 0x08)
	    Offset += 2;
    
    pMsg = (PEAPOL_PACKET) &Elem->Msg[LENGTH_802_11 + LENGTH_802_1_H + Offset];
    pKeyDesc = (PKEY_DESCRIPTER)&Elem->Msg[LENGTH_802_11+LENGTH_802_1_H+LENGTH_EAPOL_H+Offset];

#ifdef BIG_ENDIAN
    // pMsg->KeyDesc.Keyinfo and pKeyDesc->Keyinfo both point to the same addr.
    // Thus, it only needs swap once.
{
		USHORT	tmpKeyinfo;

		NdisMoveMemory(&tmpKeyinfo, &pMsg->KeyDesc.Keyinfo, sizeof(USHORT)); 
		tmpKeyinfo = SWAP16(tmpKeyinfo);
		NdisMoveMemory(&pMsg->KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT)); 
}
//    *(USHORT *)&pMsg->KeyDesc.Keyinfo = SWAP16(*(USHORT *)&pMsg->KeyDesc.Keyinfo);
#endif

    pEntry = MacTableLookup(pAdapter, pHeader->Addr2);
	if (!pEntry)
		return;

    if ((pEntry->AuthMode != Ndis802_11AuthModeWPA) && (pEntry->AuthMode != Ndis802_11AuthModeWPAPSK)
    	&& (pEntry->AuthMode != Ndis802_11AuthModeWPA2) && (pEntry->AuthMode != Ndis802_11AuthModeWPA2PSK))
        return;

    if (((pMsg->ProVer!= EAPOL_VER) && (pMsg->ProVer!= EAPOL_VER2)) || ((pMsg->KeyDesc.Type != WPA1_KEY_DESC) && (pMsg->KeyDesc.Type != WPA2_KEY_DESC)))
    {
        DBGPRINT(RT_DEBUG_ERROR, "Key descripter does not match with WPA rule \n");
        return;
    }

    if ((pEntry->WepStatus == Ndis802_11Encryption3Enabled) && (pMsg->KeyDesc.Keyinfo.KeyDescVer != DESC_TYPE_AES))
    {
        DBGPRINT(RT_DEBUG_ERROR, "Key descripter version not match AES \n");
        return;
    }
    else if ((pEntry->WepStatus == Ndis802_11Encryption2Enabled) && (pMsg->KeyDesc.Keyinfo.KeyDescVer != DESC_TYPE_TKIP))
    {
        DBGPRINT(RT_DEBUG_ERROR, "Key descripter version not match TKIP \n");
        return;
    }

    if ((pEntry->Sst == SST_ASSOC) && (pEntry->WpaState >= AS_INITPSK))
    {
    	if ((pEntry->GTKState == REKEY_ESTABLISHED) && (pKeyDesc->Keyinfo.KeyMic) && (pKeyDesc->Keyinfo.Secure) && !(pKeyDesc->Keyinfo.Error))
        {
        	RTMPHandleSTAKey(pAdapter, pEntry, Elem);
    	}
        else if ((pKeyDesc->Keyinfo.KeyMic) && (pKeyDesc->Keyinfo.Error) && (pKeyDesc->Keyinfo.Request))
        {
            // Category 2. 
            DBGPRINT(RT_DEBUG_TRACE, "MIC, ERROR, REQUEST ALL 1'S, ACTIVE COUNTERMEASURE \n");
            HandleCounterMeasure(pAdapter, pEntry);
        }
        else if ((pKeyDesc->Keyinfo.Secure) && !(pKeyDesc->Keyinfo.Error) && !(pKeyDesc->Keyinfo.Request))
        {
#ifdef BIG_ENDIAN
            // recovery original byte order, before forward Elem to another routine	   
{
			USHORT	tmpKeyinfo;

			NdisMoveMemory(&tmpKeyinfo, &pMsg->KeyDesc.Keyinfo, sizeof(USHORT)); 
			tmpKeyinfo = SWAP16(tmpKeyinfo);
			NdisMoveMemory(&pMsg->KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT)); 
}            
//            *(USHORT *)&pMsg->KeyDesc.Keyinfo = SWAP16(*(USHORT *)&pMsg->KeyDesc.Keyinfo);
#endif
            // SECURE=1, REQUEST=0, ERROR=0
            if ((pEntry->AuthMode == Ndis802_11AuthModeWPA) || (pEntry->AuthMode == Ndis802_11AuthModeWPAPSK))
            PeerGroupMsg2Action(pAdapter, pEntry, &Elem->Msg[LENGTH_802_11+Offset], (Elem->MsgLen-LENGTH_802_11-Offset));
	    else if (((pEntry->AuthMode == Ndis802_11AuthModeWPA2) || 
		      (pEntry->AuthMode == Ndis802_11AuthModeWPA2PSK)) 
		     && (pEntry->WpaState >=AS_PTKINITDONE) && (pEntry->GTKState == REKEY_NEGOTIATING))
                PeerGroupMsg2Action(pAdapter, pEntry, &Elem->Msg[LENGTH_802_11+Offset], (Elem->MsgLen-LENGTH_802_11-Offset));
            else if ((pEntry->AuthMode == Ndis802_11AuthModeWPA2) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2PSK))
            	Wpa2PeerPairMsg4Action(pAdapter, pEntry, Elem);
        }
        else if (!(pKeyDesc->Keyinfo.Secure) && !(pKeyDesc->Keyinfo.Error) && !(pKeyDesc->Keyinfo.Request))
        {
#ifdef BIG_ENDIAN
            // recovery original byte order, before forward Elem to another routine
{
			USHORT	tmpKeyinfo;

			NdisMoveMemory(&tmpKeyinfo, &pMsg->KeyDesc.Keyinfo, sizeof(USHORT)); 
			tmpKeyinfo = SWAP16(tmpKeyinfo);
			NdisMoveMemory(&pMsg->KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT)); 
}

//            *(USHORT *)&pMsg->KeyDesc.Keyinfo = SWAP16(*(USHORT *)&pMsg->KeyDesc.Keyinfo);
#endif
            // general 4-way handshake step // SECURE=0, REQUEST=0 ,  ERROR= 0
            // Category 1
            if (pEntry->WpaState == AS_PTKSTART)
                PeerPairMsg2Action(pAdapter, pEntry, Elem);
            else if (pEntry->WpaState == AS_PTKINIT_NEGOTIATING)
                Wpa1PeerPairMsg4Action(pAdapter, pEntry, Elem);
        }
        else if (!(pKeyDesc->Keyinfo.Error) && (pKeyDesc->Keyinfo.Request))
        {
            // Category 3
            if (pKeyDesc->Keyinfo.KeyType == GROUPKEY)
            {
                DBGPRINT(RT_DEBUG_TRACE, "REQUEST=1, ERROR=0, update group key\n");
                GenRandom(pAdapter, pAdapter->PortCfg.MBSSID[pEntry->ApIdx].GNonce, pEntry->ApIdx);
                for (i = 0; i < MAX_LEN_OF_MAC_TABLE; i++)
                {
                    if ((pAdapter->MacTab.Content[i].Valid == TRUE) && (pAdapter->MacTab.Content[i].WpaState == AS_PTKINITDONE)
                    	&& (pAdapter->MacTab.Content[i].ApIdx == pEntry->ApIdx))
                    {
                        pAdapter->MacTab.Content[i].GTKState = REKEY_NEGOTIATING;
                        WPAHardTransmit(pAdapter, &pAdapter->MacTab.Content[i]);
                    }
                }
            }
            else
            {
                DBGPRINT(RT_DEBUG_TRACE, "REQUEST=1, ERROR= 0, update pairwise key\n");
                pEntry->PairwiseKey.KeyLen = 0;
                pEntry->Sst = SST_ASSOC;
                if ((pEntry->AuthMode == Ndis802_11AuthModeWPA) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2))
                    pEntry->WpaState = AS_INITPMK;  
                else if ((pEntry->AuthMode == Ndis802_11AuthModeWPAPSK) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2PSK))
                    pEntry->WpaState = AS_INITPSK;  
                pEntry->GTKState = REKEY_NEGOTIATING;
                
				pEntry->RetryTimer.expires = jiffies + WPA_RETRY_EXEC_INTV;
				pEntry->ReTryCounter = PEER_MSG1_RETRY_TIMER_CTR;
				WPAStart4WayHS(pAdapter, pEntry);
            }
        }
        else
        {
                //
        }
    }
}

VOID RTMPHandleSTAKey(
    IN PRTMP_ADAPTER    pAd, 
    IN PMAC_TABLE_ENTRY	pEntry,
    IN MLME_QUEUE_ELEM  *Elem) 
{
	ULONG				FrameLen = 0;
	PUCHAR				pOutBuffer = NULL;
	UCHAR				Header802_3[14];
	EAPOL_PACKET		Packet, EAPOLPKT;
	PEAPOL_PACKET		pSTAKey;
	PHEADER_802_11		pHeader;
	UCHAR				Offset = 0;
	ULONG				MICMsgLen;
	UCHAR				DA[MAC_ADDR_LEN];
	UCHAR				Key_Data[512];
	UCHAR				key_length;
	UCHAR				mic[LEN_KEY_DESC_MIC];
	UCHAR				digest[80];
	UCHAR				temp[64];
	UCHAR				PTK[80];
	PMAC_TABLE_ENTRY	pDaEntry;

    DBGPRINT(RT_DEBUG_TRACE, "==> RTMPHandleSTAKey\n");

    if (!pEntry)
		return;
	
	if ((pEntry->WpaState != AS_PTKINITDONE))
    {
        DBGPRINT(RT_DEBUG_ERROR, "Not expect calling STAKey hand shaking here");
        return;
    }

    pHeader = (PHEADER_802_11) Elem->Msg;

    if (pHeader->FC.SubType & 0x08)
        Offset += 2;
    
    pSTAKey = (PEAPOL_PACKET)&Elem->Msg[LENGTH_802_11 + LENGTH_802_1_H + Offset];	

    // Check Replay Counter
    if (!RTMPEqualMemory(pSTAKey->KeyDesc.RCounter, pEntry->R_Counter, LEN_KEY_DESC_REPLAY))
    {
        DBGPRINT(RT_DEBUG_ERROR, "Replay Counter Different in STAKey handshake!! \n");
        DBGPRINT(RT_DEBUG_ERROR, "Receive : %d %d %d %d  \n", pSTAKey->KeyDesc.RCounter[0],pSTAKey->KeyDesc.RCounter[1],pSTAKey->KeyDesc.RCounter[2],pSTAKey->KeyDesc.RCounter[3]);
        DBGPRINT(RT_DEBUG_ERROR, "Current : %d %d %d %d  \n", pEntry->R_Counter[4],pEntry->R_Counter[5],pEntry->R_Counter[6],pEntry->R_Counter[7]);
        return;
    }

    // Check MIC, if not valid, discard silently
    NdisMoveMemory(DA, &pSTAKey->KeyDesc.Data[6], MAC_ADDR_LEN);
	if (pSTAKey->KeyDesc.Keyinfo.KeyMic && pSTAKey->KeyDesc.Keyinfo.Secure && pSTAKey->KeyDesc.Keyinfo.Request)
	{
		pEntry->bDlsInit = TRUE;
		DBGPRINT(RT_DEBUG_TRACE, "STAKey Initiator: %02x:%02x:%02x:%02x:%02x:%02x\n",
			pEntry->Addr[0], pEntry->Addr[1], pEntry->Addr[2], pEntry->Addr[3], pEntry->Addr[4], pEntry->Addr[5]);
	}
    MICMsgLen = pSTAKey->Body_Len[1] | ((pSTAKey->Body_Len[0]<<8) && 0xff00);
    MICMsgLen += LENGTH_EAPOL_H;
    if (MICMsgLen > (Elem->MsgLen - LENGTH_802_11 - LENGTH_802_1_H))
    {
        DBGPRINT(RT_DEBUG_ERROR, "Receive wrong format EAPOL packets \n");
        return;        
    }

	// This is proprietary DLS protocol, it will be adhered when spec. is finished.
	NdisZeroMemory(temp, 64);
	NdisMoveMemory(temp, "IEEE802.11 WIRELESS ACCESS POINT", 32);
	CountPTK(temp, temp, pAd->PortCfg.MBSSID[pEntry->ApIdx].Bssid, temp, pEntry->Addr, PTK, LEN_PTK);
	NdisMoveMemory(pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK, PTK, LEN_PTK);
	DBGPRINT(RT_DEBUG_INFO, "PTK-%x %x %x %x %x %x %x %x \n",pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK[0],pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK[1],pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK[2],pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK[3],pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK[4],pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK[5],pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK[6],pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK[7]);

    NdisMoveMemory((PUCHAR)&EAPOLPKT, pSTAKey, MICMsgLen);
    NdisZeroMemory(EAPOLPKT.KeyDesc.MIC, LEN_KEY_DESC_MIC);
    if (pEntry->WepStatus == Ndis802_11Encryption2Enabled)
    {
        hmac_md5(pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK, LEN_EAP_MICK, (PUCHAR)&EAPOLPKT, MICMsgLen, mic);
    }
    else
    {
        HMAC_SHA1((PUCHAR)&EAPOLPKT,  MICMsgLen, pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK, LEN_EAP_MICK, mic);
    }
    if (!RTMPEqualMemory(pSTAKey->KeyDesc.MIC, mic, LEN_KEY_DESC_MIC))
    {
        DBGPRINT(RT_DEBUG_ERROR, "MIC Different in STAKey handshake!! \n");
        return;
    }
    else
        DBGPRINT(RT_DEBUG_TRACE, "MIC VALID in STAKey handshake!! \n");

	// Receive init STA's STAKey Message-2, and terminate the handshake
	if (pEntry->bDlsInit && !pSTAKey->KeyDesc.Keyinfo.Request)
	{
		pEntry->bDlsInit = FALSE;
		DBGPRINT(RT_DEBUG_TRACE, "Receive init STA's STAKey Message-2, STAKey handshake finished \n");
		return;
	}

	// Receive init STA's STAKey Message-2, and terminate the handshake
	if (RTMPEqualMemory(&pSTAKey->KeyDesc.Data[2], OUI_WPA2_WEP40, 3))
	{
		DBGPRINT(RT_DEBUG_WARN, "Receive a STAKey message which not support currently, just drop it \n");
		return;
	}
	
    do
    {
    	pDaEntry = MacTableLookup(pAd, DA);
    	if (!pDaEntry)
    		break;

    	if ((pDaEntry->WpaState != AS_PTKINITDONE))
	    {
	        DBGPRINT(RT_DEBUG_ERROR, "Not expect calling STAKey hand shaking here \n");
	        break;
	    }
    	
        pOutBuffer = kmalloc(MAX_LEN_OF_EAP_HS, MEM_ALLOC_FLAG);
        if(pOutBuffer == NULL)
            break;

        WPAMake8023Hdr(pAd, pDaEntry, Header802_3);

        // Increment replay counter by 1  
        ADD_ONE_To_64BIT_VAR(pAd->PortCfg.R_Counter);
        
        // 0. init Packet and Fill header
        NdisZeroMemory(&Packet, sizeof(Packet));

        Packet.ProVer = EAPOL_VER;
        Packet.ProType = EAPOLKey;
        Packet.Body_Len[1] = 0x5f;
        
        // 1. Fill replay counter
        NdisMoveMemory(pDaEntry->R_Counter, pAd->PortCfg.R_Counter, sizeof(pDaEntry->R_Counter));
        NdisMoveMemory(Packet.KeyDesc.RCounter, pDaEntry->R_Counter, sizeof(pDaEntry->R_Counter));
        
        // 2. Fill key version, keyinfo, key len
        Packet.KeyDesc.Keyinfo.KeyDescVer= GROUP_KEY;
        Packet.KeyDesc.Keyinfo.KeyType	= 0;
        Packet.KeyDesc.Keyinfo.Install	= 1;
        Packet.KeyDesc.Keyinfo.KeyAck	= 1;
        Packet.KeyDesc.Keyinfo.KeyMic	= 1;
        Packet.KeyDesc.Keyinfo.Secure	= 1;
        Packet.KeyDesc.Keyinfo.EKD		= 1;
		DBGPRINT(RT_DEBUG_TRACE, "STAKey handshake for peer STA %02x:%02x:%02x:%02x:%02x:%02x\n",
			DA[0], DA[1], DA[2], DA[3], DA[4], DA[5]);
        
        if ((pDaEntry->AuthMode == Ndis802_11AuthModeWPA) || (pDaEntry->AuthMode == Ndis802_11AuthModeWPAPSK))
        {
        	Packet.KeyDesc.Type = WPA1_KEY_DESC;

        	DBGPRINT(RT_DEBUG_TRACE, "pDaEntry->AuthMode == Ndis802_11AuthModeWPA/WPAPSK\n");
        }
        else if ((pDaEntry->AuthMode == Ndis802_11AuthModeWPA2) || (pDaEntry->AuthMode == Ndis802_11AuthModeWPA2PSK))
        {
        	Packet.KeyDesc.Type = WPA2_KEY_DESC;
        	Packet.KeyDesc.DataLen[1] = 0;

        	DBGPRINT(RT_DEBUG_TRACE, "pDaEntry->AuthMode == Ndis802_11AuthModeWPA2/WPA2PSK\n");
        }

        Packet.KeyDesc.KeyLength[1] = LEN_TKIP_KEY;
        Packet.KeyDesc.DataLen[1] = LEN_TKIP_KEY;
        Packet.KeyDesc.Keyinfo.KeyDescVer = DESC_TYPE_TKIP;
        if (pDaEntry->WepStatus == Ndis802_11Encryption3Enabled)
        {
            Packet.KeyDesc.KeyLength[1] = LEN_AES_KEY;
            Packet.KeyDesc.DataLen[1] = LEN_AES_KEY;
            Packet.KeyDesc.Keyinfo.KeyDescVer = DESC_TYPE_AES;
        }

		// Key Data Encapsulation format, use Ralink OUI to distinguish proprietary and standard.
    	Key_Data[0] = 0xDD;
		Key_Data[1] = 0x00;		// Length (This field will be filled later)
    	Key_Data[2] = 0x00;		// OUI
    	Key_Data[3] = 0x0C;		// OUI
    	Key_Data[4] = 0x43;		// OUI
    	Key_Data[5] = 0x02;		// Data Type (STAKey Key Data Encryption)

		// STAKey Data Encapsulation format
    	Key_Data[6] = 0x00;		//Reserved
		Key_Data[7] = 0x00;		//Reserved

		// STAKey MAC address
		NdisMoveMemory(&Key_Data[8], pEntry->Addr, MAC_ADDR_LEN);		// initiator MAC address

		// STAKey (Handle the difference between TKIP and AES-CCMP)
		if (pDaEntry->WepStatus == Ndis802_11Encryption3Enabled)
        {
        	Key_Data[1] = 0x1E;	// 4+2+6+16(OUI+Reserved+STAKey_MAC_Addr+STAKey)
        	NdisMoveMemory(&Key_Data[14], pEntry->PairwiseKey.Key, LEN_AES_KEY);
		}
		else
		{
			Key_Data[1] = 0x2E;	// 4+2+6+32(OUI+Reserved+STAKey_MAC_Addr+STAKey)
			NdisMoveMemory(&Key_Data[14], pEntry->PairwiseKey.Key, LEN_TKIP_EK);
			NdisMoveMemory(&Key_Data[14+LEN_TKIP_EK], pEntry->PairwiseKey.TxMic, LEN_TKIP_TXMICK);
			NdisMoveMemory(&Key_Data[14+LEN_TKIP_EK+LEN_TKIP_TXMICK], pEntry->PairwiseKey.RxMic, LEN_TKIP_RXMICK);
		}

		key_length = Key_Data[1];
		Packet.Body_Len[1] = key_length + 0x5f;

		// This is proprietary DLS protocol, it will be adhered when spec. is finished.
		NdisZeroMemory(temp, 64);
		NdisMoveMemory(temp, "IEEE802.11 WIRELESS ACCESS POINT", 32);
		CountPTK(temp, temp, pAd->PortCfg.MBSSID[pEntry->ApIdx].Bssid, temp, DA, PTK, LEN_PTK);
		NdisMoveMemory(pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK, PTK, LEN_PTK);
		DBGPRINT(RT_DEBUG_TRACE, "PTK-0-%x %x %x %x %x %x %x %x \n",pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK[0],pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK[1],pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK[2],pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK[3],pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK[4],pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK[5],pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK[6],pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK[7]);

       	NdisMoveMemory(Packet.KeyDesc.Data, Key_Data, key_length);
		NdisZeroMemory(mic, sizeof(mic));

		MakeOutgoingFrame(pOutBuffer,			&FrameLen,
                        Packet.Body_Len[1] + 4,	&Packet,
                        END_OF_ARGS);
	    
		// Calculate MIC
        if (pDaEntry->WepStatus == Ndis802_11Encryption3Enabled)
        {
            HMAC_SHA1(pOutBuffer, FrameLen, pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK, LEN_EAP_MICK, digest);
            NdisMoveMemory(Packet.KeyDesc.MIC, digest, LEN_KEY_DESC_MIC);
	    }
        else
        {
            hmac_md5(pAd->PortCfg.MBSSID[pEntry->ApIdx].DlsPTK, LEN_EAP_MICK, pOutBuffer, FrameLen, mic);
            NdisMoveMemory(Packet.KeyDesc.MIC, mic, LEN_KEY_DESC_MIC);
        }

#ifdef BIG_ENDIAN
		{
			USHORT	tmpKeyinfo;

			NdisMoveMemory(&tmpKeyinfo, &Packet.KeyDesc.Keyinfo, sizeof(USHORT)); 
			tmpKeyinfo = SWAP16(tmpKeyinfo);
			NdisMoveMemory(&Packet.KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT)); 
		}
#endif

        // Fill frame
        MakeOutgoingFrame(pOutBuffer,				&FrameLen,
                            sizeof(Header802_3),	Header802_3,
                            Packet.Body_Len[1] + 4,	&Packet,
                            END_OF_ARGS);
//iversonadd
        RTMPToWirelessSta(pAd, pOutBuffer, FrameLen, FALSE);

        kfree(pOutBuffer);
    }while(FALSE);
    
    DBGPRINT(RT_DEBUG_TRACE, "<== RTMPHandleSTAKey: FrameLen=%d\n", FrameLen);
}

/*
    ==========================================================================
    Description:
        This is a function to initilize 4way handshake
    Return:
         
    ==========================================================================
*/
VOID WPAStart4WayHS(
    IN PRTMP_ADAPTER    pAdapter,
    IN MAC_TABLE_ENTRY  *pEntry)
{
    ULONG           FrameLen = 0;
    UCHAR           *OutBuffer = NULL;
    UCHAR           Header802_3[14];
    EAPOL_PACKET    Packet;

    DBGPRINT(RT_DEBUG_TRACE, "==>WPAStart4WayHS\n");

    do
    {
    	if (!pEntry)
    		break;
    	if ((pEntry->WpaState > AS_PTKSTART) || (pEntry->WpaState < AS_INITPMK))
	    {
	        DBGPRINT(RT_DEBUG_ERROR, "Not expect calling WPAStart4WayHS here ");
	        break;
	    }
    	
#ifdef WSC_SUPPORT
        if (MAC_ADDR_EQUAL(pEntry->Addr, pAdapter->PortCfg.MBSSID[pEntry->ApIdx].WscControl.EntryAddr) &&
            pAdapter->PortCfg.MBSSID[pEntry->ApIdx].WscControl.EapMsgRunning)
        {
            pEntry->WpaState = AS_NOTUSE;
	        DBGPRINT(RT_DEBUG_ERROR, "This is a WSC-Enrollee. Not expect calling WPAStart4WayHS here \n");
	        break;
        }
#endif // WSC_SUPPORT //
    	
        OutBuffer = kmalloc(MAX_LEN_OF_EAP_HS, MEM_ALLOC_FLAG);
        if(OutBuffer == NULL)
            break;

        WPAMake8023Hdr(pAdapter, pEntry, Header802_3);

        // Increment replay counter by 1  
        ADD_ONE_To_64BIT_VAR(pAdapter->PortCfg.R_Counter);
        
        // 0. init Packet and Fill header
        NdisZeroMemory(&Packet, sizeof(Packet));
        Packet.ProVer = EAPOL_VER;
        Packet.ProType = EAPOLKey;
        Packet.Body_Len[1] = 0x5f;
        
        // 1. Fill replay counter
        NdisMoveMemory(pEntry->R_Counter, pAdapter->PortCfg.R_Counter, sizeof(pEntry->R_Counter));
        NdisMoveMemory(Packet.KeyDesc.RCounter, pEntry->R_Counter, sizeof(pEntry->R_Counter));
        
        // 2. Fill key version, keyinfo, key len
        Packet.KeyDesc.Keyinfo.KeyType = PAIRWISEKEY;
        Packet.KeyDesc.Keyinfo.KeyAck = 1;
		DBGPRINT(RT_DEBUG_TRACE, "STA from %02x:%02x:%02x:%02x:%02x:%02x\n", 
				pEntry->Addr[0], pEntry->Addr[1], pEntry->Addr[2], pEntry->Addr[3],
				pEntry->Addr[4], pEntry->Addr[5]);
        DBGPRINT(RT_DEBUG_TRACE, "PMK = %02x:%02x:%02x:%02x-%02x:%02x:%02x:%02x\n",
        	pAdapter->PortCfg.MBSSID[pEntry->ApIdx].PMK[0],pAdapter->PortCfg.MBSSID[pEntry->ApIdx].PMK[1],pAdapter->PortCfg.MBSSID[pEntry->ApIdx].PMK[2],pAdapter->PortCfg.MBSSID[pEntry->ApIdx].PMK[3],
        	pAdapter->PortCfg.MBSSID[pEntry->ApIdx].PMK[4],pAdapter->PortCfg.MBSSID[pEntry->ApIdx].PMK[5],pAdapter->PortCfg.MBSSID[pEntry->ApIdx].PMK[6],pAdapter->PortCfg.MBSSID[pEntry->ApIdx].PMK[7]);
        
        if ((pEntry->AuthMode == Ndis802_11AuthModeWPA) || (pEntry->AuthMode == Ndis802_11AuthModeWPAPSK))
        {
        	Packet.KeyDesc.Type = WPA1_KEY_DESC;
        	Packet.KeyDesc.DataLen[1] = 0;

        	DBGPRINT(RT_DEBUG_TRACE, "pEntry->AuthMode == Ndis802_11AuthModeWPA/WPAPSK\n");
        }
        else if ((pEntry->AuthMode == Ndis802_11AuthModeWPA2) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2PSK))
        {
        	Packet.KeyDesc.Type = WPA2_KEY_DESC;
        	Packet.KeyDesc.DataLen[1] = 0;

			// PMKID match, then start 4-way HS directly
        	if (pEntry->PMKID_CacheIdx != ENTRY_NOT_FOUND)
        	{
        		Packet.KeyDesc.Data[0] = 0xDD;
	        	Packet.KeyDesc.Data[2] = 0x00;
	        	Packet.KeyDesc.Data[3] = 0x0F;
	        	Packet.KeyDesc.Data[4] = 0xAC;
	        	Packet.KeyDesc.Data[5] = 0x04;

        		NdisMoveMemory(&Packet.KeyDesc.Data[6], &pAdapter->PortCfg.MBSSID[pEntry->ApIdx].PMKIDCache.BSSIDInfo[pEntry->PMKID_CacheIdx].PMKID, LEN_PMKID);
        		NdisMoveMemory(&pAdapter->PortCfg.MBSSID[pEntry->ApIdx].PMK, &pAdapter->PortCfg.MBSSID[pEntry->ApIdx].PMKIDCache.BSSIDInfo[pEntry->PMKID_CacheIdx].PMK, PMK_LEN);
        		
        		Packet.KeyDesc.Data[1] = 0x14;// 4+2+LEN_PMKID
        		Packet.KeyDesc.DataLen[1] += 6 + LEN_PMKID;
        		Packet.Body_Len[1] += 6 + LEN_PMKID;
        	}
        	
        	DBGPRINT(RT_DEBUG_TRACE, "pEntry->AuthMode == Ndis802_11AuthModeWPA2/WPA2PSK\n");
        }

        if (pEntry->WepStatus == Ndis802_11Encryption2Enabled)
        {
            Packet.KeyDesc.KeyLength[1] = LEN_TKIP_KEY;
            Packet.KeyDesc.Keyinfo.KeyDescVer = DESC_TYPE_TKIP;
        }
        else if (pEntry->WepStatus == Ndis802_11Encryption3Enabled)
        {
            Packet.KeyDesc.KeyLength[1] = LEN_AES_KEY;
            Packet.KeyDesc.Keyinfo.KeyDescVer = DESC_TYPE_AES;
        }

#ifdef BIG_ENDIAN
{
		USHORT	tmpKeyinfo;

		NdisMoveMemory(&tmpKeyinfo, &Packet.KeyDesc.Keyinfo, sizeof(USHORT)); 
		tmpKeyinfo = SWAP16(tmpKeyinfo);
		NdisMoveMemory(&Packet.KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT)); 
}

//        *(USHORT *)&Packet.KeyDesc.Keyinfo = SWAP16(*(USHORT *)&Packet.KeyDesc.Keyinfo);
#endif
        
        // 3. Fill Anonce
        GenRandom(pAdapter, pEntry->ANonce, pEntry->ApIdx);
        NdisMoveMemory(Packet.KeyDesc.Nonce, pEntry->ANonce, sizeof(pEntry->ANonce));
        MakeOutgoingFrame(OutBuffer,            &FrameLen,
                            sizeof(Header802_3),    Header802_3,
                            Packet.Body_Len[1] + 4,  &Packet,
                            END_OF_ARGS);
//iversonadd
        RTMPToWirelessSta(pAdapter, OutBuffer, FrameLen, FALSE);

        mod_timer(&pEntry->RetryTimer, pEntry->RetryTimer.expires);
		// Should start from 1, init is 0
		pEntry->ReTryCounter++;
		pEntry->RetryTimerRunning = TRUE;

        pEntry->WpaState = AS_PTKSTART;

        kfree(OutBuffer);
    }while(FALSE);
    
    DBGPRINT(RT_DEBUG_TRACE, "<== WPAStart4WayHS:pEntry->WpaState=%d, FrameLen=%d\n",pEntry->WpaState,FrameLen);
}

/*
    ==========================================================================
    Description:
        When receiving the second packet of 4-way pairwisekey handshake.
    Return:
    ==========================================================================
*/
VOID PeerPairMsg2Action(
    IN PRTMP_ADAPTER    pAdapter, 
    IN MAC_TABLE_ENTRY  *pEntry,
    IN MLME_QUEUE_ELEM  *Elem) 
{
    UCHAR                   *mpool, *PTK, *digest, *Key_Data, *pRc4GTK; 
    UCHAR					mic[LEN_KEY_DESC_MIC], SNonce[LEN_KEY_DESC_NONCE];
    ULONG                   FrameLen = 0;
    UCHAR                   *OutBuffer = NULL;
    USHORT                  MICMsgLen, MICOffset;
    PUCHAR                  pSA;
    UCHAR                   Header802_3[LENGTH_802_3];
    EAPOL_PACKET            Packet;
    PEAPOL_PACKET           pMsg2;
    PHEADER_802_11          pHeader;

    DBGPRINT(RT_DEBUG_TRACE, "PeerPairMsg2Action ===>\n");

    if (!pEntry)
    	return;

    // check Entry in valid State
    if (pEntry->WpaState < AS_PTKSTART)
        return;

	mpool = kmalloc(4096, MEM_ALLOC_FLAG);  // allocate memory
    if (mpool == NULL)
            return;
            
	/* OutBuffer Len = MAX_LEN_OF_EAP_HS */
	OutBuffer = (UCHAR *) ROUND_UP(mpool, 4);
	/* PTK  Len = 80 */
	PTK = (UCHAR *) ROUND_UP(OutBuffer+MAX_LEN_OF_EAP_HS, 4);
	/* digest Len = 80 */
	digest =(UCHAR *) ROUND_UP(PTK+80, 4);
	/* pRc4GTK Len = 512 */
	pRc4GTK = (UCHAR *) ROUND_UP(digest+80, 4);
	/* Key_Data Len = 512 */
	Key_Data = (UCHAR *) ROUND_UP(pRc4GTK+512, 4);

    do
    {            
        // Save Data Length to pDesc for receiving packet,   then put in outgoing frame Data Len fields.
        pHeader =  (PHEADER_802_11) Elem->Msg;
        if (pHeader->FC.SubType & 0x08)
        {
            pMsg2 = (PEAPOL_PACKET) &Elem->Msg[LENGTH_802_11 + LENGTH_802_1_H + 2];
        }
        else
        {
            pMsg2 = (PEAPOL_PACKET) &Elem->Msg[LENGTH_802_11 + LENGTH_802_1_H];
	    }
            
        // 1. check Replay Counter coresponds to MSG1.,  otherwise discard
        if (!RTMPEqualMemory(pMsg2->KeyDesc.RCounter, pEntry->R_Counter, LEN_KEY_DESC_REPLAY))
        {
            DBGPRINT(RT_DEBUG_ERROR, "Replay Counter Different in Msg 2 of 4-way handshake !! \n");
            DBGPRINT(RT_DEBUG_ERROR, "Receive : %d  %d  %d  %d  %d  %d  %d  %d \n", pMsg2->KeyDesc.RCounter[0],pMsg2->KeyDesc.RCounter[1],\
                pMsg2->KeyDesc.RCounter[2],pMsg2->KeyDesc.RCounter[3],pMsg2->KeyDesc.RCounter[4],pMsg2->KeyDesc.RCounter[5],pMsg2->KeyDesc.RCounter[6],pMsg2->KeyDesc.RCounter[7]);
            DBGPRINT(RT_DEBUG_ERROR, "Current : %d  %d  %d  %d  %d  %d  %d  %d \n",pEntry->R_Counter[0],pEntry->R_Counter[1],pEntry->R_Counter[2],pEntry->R_Counter[3],\
                pEntry->R_Counter[4],pEntry->R_Counter[5],pEntry->R_Counter[6],pEntry->R_Counter[7]);
            break;
        }

        // 2. Derive necessary info for counting PTK and thus count PTK
        MICMsgLen = pMsg2->Body_Len[1] | ((pMsg2->Body_Len[0]<<8) && 0xff00);
        MICMsgLen += LENGTH_EAPOL_H;
        if (MICMsgLen > (Elem->MsgLen -LENGTH_802_11 - LENGTH_802_1_H))
        {
            DBGPRINT(RT_DEBUG_ERROR, "Receive wrong format EAPOL packets \n");
            break;        
        }
        NdisMoveMemory(SNonce, pMsg2->KeyDesc.Nonce, LEN_KEY_DESC_NONCE);
        pSA = pEntry->Addr;

        CountPTK(pAdapter->PortCfg.MBSSID[pEntry->ApIdx].PMK,  pEntry->ANonce, pAdapter->PortCfg.MBSSID[pEntry->ApIdx].Bssid, SNonce, pSA, PTK, LEN_PTK); 
        NdisMoveMemory(pEntry->PTK, PTK, LEN_PTK);

		DBGPRINT(RT_DEBUG_TRACE, "PTK-%x %x %x %x %x %x %x %x \n",PTK[0],PTK[1],PTK[2],PTK[3],PTK[4],PTK[5],PTK[6],PTK[7]);
	    DBGPRINT(RT_DEBUG_TRACE, "ANonce1-%x %x %x %x %x %x %x %x \n",pEntry->ANonce[0],pEntry->ANonce[1],pEntry->ANonce[2],pEntry->ANonce[3],pEntry->ANonce[4],pEntry->ANonce[5],pEntry->ANonce[6],pEntry->ANonce[7]);
	    DBGPRINT(RT_DEBUG_TRACE, "ANonce2-%x %x %x %x %x %x %x %x \n",pEntry->ANonce[8],pEntry->ANonce[9],pEntry->ANonce[10],pEntry->ANonce[11],pEntry->ANonce[12],pEntry->ANonce[13],pEntry->ANonce[14],pEntry->ANonce[15]);

        // 3. verify MSG2 MIC, If not valid, discard.
        MICOffset = sizeof(EAPOL_PACKET)-MAX_LEN_OF_RSNIE-2-LEN_KEY_DESC_MIC;
        NdisMoveMemory(OutBuffer, pMsg2, MICMsgLen);  
        NdisZeroMemory((OutBuffer+MICOffset), LEN_KEY_DESC_MIC);
        if (pEntry->WepStatus == Ndis802_11Encryption2Enabled)
        {
            hmac_md5(PTK, LEN_EAP_MICK, OutBuffer, MICMsgLen, mic);
        }
        else
        {
            HMAC_SHA1(OutBuffer,  MICMsgLen, PTK, LEN_EAP_MICK, digest);
            NdisMoveMemory(mic, digest, LEN_KEY_DESC_MIC);
        }
        if (!RTMPEqualMemory((PUCHAR)pMsg2+MICOffset, mic, LEN_KEY_DESC_MIC))
        {
            DBGPRINT(RT_DEBUG_ERROR, "MIC Different in Msg 2 of 4-way handshake!! \n");
            break;
        }
        else
            DBGPRINT(RT_DEBUG_TRACE, "MIC VALID in Msg 2 of 4-way handshake!! \n");
            
        // 4. check RSN IE, if not match, send MLME-DEAUTHENTICATE.
        if (!RTMPEqualMemory(&pMsg2->KeyDesc.Data[2], pEntry->RSN_IE, pEntry->RSNIE_Len))
        {       
            DBGPRINT(RT_DEBUG_ERROR, "RSN_IE Different in Msg 2 of 4-way handshake!! \n");
            DBGPRINT(RT_DEBUG_ERROR, "Receive : %x %x %x %x ...\n",pMsg2->KeyDesc.Data[2], pMsg2->KeyDesc.Data[3],pMsg2->KeyDesc.Data[4],pMsg2->KeyDesc.Data[5] );
            DBGPRINT(RT_DEBUG_ERROR, "Current : %x %x %x %x ...\n",pEntry->RSN_IE[0], pEntry->RSN_IE[1],pEntry->RSN_IE[2],pEntry->RSN_IE[3] );
            break;  
        }
        else
            DBGPRINT(RT_DEBUG_TRACE, "RSN_IE VALID in Msg 2 of 4-way handshake!! \n");
            
        pEntry->WpaState = AS_PTKINIT_NEGOTIATING;

        // Increment replay counter by 1       
        ADD_ONE_To_64BIT_VAR(pAdapter->PortCfg.R_Counter);
        NdisMoveMemory(pEntry->R_Counter, pAdapter->PortCfg.R_Counter, sizeof(pEntry->R_Counter));
            
        // 5. CONSTRUCT msg3
        // 5-0. Init Packet and Fill header
        NdisZeroMemory(&Packet, sizeof(Packet));
        Packet.ProVer = EAPOL_VER;
        Packet.ProType = EAPOLKey;
            
        // Fill replay counter
        NdisMoveMemory(Packet.KeyDesc.RCounter, pEntry->R_Counter, sizeof(pEntry->R_Counter));
          
        // Fill key version, keyinfo, key len
        Packet.KeyDesc.Keyinfo.KeyMic = 1;
        Packet.KeyDesc.Keyinfo.KeyType = PAIRWISEKEY;
        Packet.KeyDesc.Keyinfo.Install = 1;
        Packet.KeyDesc.Keyinfo.KeyAck = 1;
        if ((pEntry->AuthMode == Ndis802_11AuthModeWPA) || (pEntry->AuthMode == Ndis802_11AuthModeWPAPSK))
        {
        	Packet.KeyDesc.Type = WPA1_KEY_DESC;
        	Packet.KeyDesc.DataLen[1] = 0;
        }
        else if ((pEntry->AuthMode == Ndis802_11AuthModeWPA2) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2PSK))
        {
        	Packet.KeyDesc.Type = WPA2_KEY_DESC;
        	Packet.KeyDesc.DataLen[1] = 0;
        	Packet.KeyDesc.Keyinfo.EKD = 1;
        	Packet.KeyDesc.Keyinfo.Secure = 1;
        }

        if (pEntry->WepStatus == Ndis802_11Encryption2Enabled)
        {
            Packet.KeyDesc.KeyLength[1] = LEN_TKIP_KEY;
        }
        else if (pEntry->WepStatus == Ndis802_11Encryption3Enabled)
        {
            Packet.KeyDesc.KeyLength[1] = LEN_AES_KEY;
        }
        Packet.KeyDesc.Keyinfo.KeyDescVer =
        	(((pEntry->WepStatus == Ndis802_11Encryption3Enabled) || (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].GroupKeyWepStatus == Ndis802_11Encryption3Enabled)) ? (DESC_TYPE_AES) : (DESC_TYPE_TKIP));

#ifdef BIG_ENDIAN
{
		USHORT	tmpKeyinfo;

		NdisMoveMemory(&tmpKeyinfo, &Packet.KeyDesc.Keyinfo, sizeof(USHORT)); 
		tmpKeyinfo = SWAP16(tmpKeyinfo);
		NdisMoveMemory(&Packet.KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT)); 
}

//        *(USHORT *)&Packet.KeyDesc.Keyinfo = SWAP16(*(USHORT *)&Packet.KeyDesc.Keyinfo);
#endif

        // 5-3, Fill in Data, Data Len
        NdisMoveMemory(Packet.KeyDesc.Nonce, pEntry->ANonce, sizeof(pEntry->ANonce));
		NdisZeroMemory(Packet.KeyDesc.MIC, sizeof(Packet.KeyDesc.MIC));
        
        if ((pEntry->AuthMode == Ndis802_11AuthModeWPA) || (pEntry->AuthMode == Ndis802_11AuthModeWPAPSK))
        {
			Packet.KeyDesc.Data[0] = IE_WPA;
			Packet.KeyDesc.Data[1] = pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSNIE_Len[0];
			Packet.KeyDesc.DataLen[1] = pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSNIE_Len[0] + 2;
			Packet.Body_Len[1] = 93 + 2 + 2 + pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSNIE_Len[0];

			NdisMoveMemory(&Packet.KeyDesc.Data[2], pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSN_IE[0], pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSNIE_Len[0]);

			// 5-4, First make a frame for Counting MIC.
			MakeOutgoingFrame(OutBuffer,            &FrameLen,
			    Packet.Body_Len[1] + 4,  &Packet,
			    END_OF_ARGS);
			NdisZeroMemory(mic, sizeof(mic));

			// 5-5, count MIC
			if (pEntry->WepStatus == Ndis802_11Encryption3Enabled)
			{
				UCHAR digest[80];
				HMAC_SHA1(OutBuffer,  FrameLen, PTK, LEN_EAP_MICK, digest);
				NdisMoveMemory(mic, digest, LEN_KEY_DESC_MIC);
			}
			else
			{
				hmac_md5(PTK,  LEN_EAP_MICK, OutBuffer, FrameLen, mic);
			}
			NdisMoveMemory(&Packet.KeyDesc.MIC, mic, LEN_KEY_DESC_MIC);
        }
        else if ((pEntry->AuthMode == Ndis802_11AuthModeWPA2) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2PSK))
        {
        	UCHAR       GTK[TKIP_GTK_LENGTH],/*pRc4GTK[512],*/ mic[LEN_KEY_DESC_MIC];
			UCHAR       ekey[(LEN_KEY_DESC_IV+LEN_EAP_EK)], /*Key_Data[512],*/ key_length, RSNIE_Len;

			NdisMoveMemory(Packet.KeyDesc.RSC, pAdapter->PortCfg.MBSSID[pEntry->ApIdx].SharedKey[pAdapter->PortCfg.MBSSID[pEntry->ApIdx].DefaultKeyId].TxTsc, 6);

			CountGTK(pAdapter->PortCfg.MBSSID[pEntry->ApIdx].GMK, (UCHAR*)pAdapter->PortCfg.MBSSID[pEntry->ApIdx].GNonce, pAdapter->PortCfg.MBSSID[pEntry->ApIdx].Bssid, GTK, TKIP_GTK_LENGTH);

	        NdisMoveMemory(pAdapter->PortCfg.MBSSID[pEntry->ApIdx].SharedKey[pAdapter->PortCfg.MBSSID[pEntry->ApIdx].DefaultKeyId].Key, GTK, LEN_TKIP_EK);
	        NdisMoveMemory(pAdapter->PortCfg.MBSSID[pEntry->ApIdx].SharedKey[pAdapter->PortCfg.MBSSID[pEntry->ApIdx].DefaultKeyId].TxMic, &GTK[16], LEN_TKIP_TXMICK);
	        NdisMoveMemory(pAdapter->PortCfg.MBSSID[pEntry->ApIdx].SharedKey[pAdapter->PortCfg.MBSSID[pEntry->ApIdx].DefaultKeyId].RxMic, &GTK[24], LEN_TKIP_RXMICK);
	        DBGPRINT(RT_DEBUG_INFO, "WPA2 Group Key ID  = %d\n", pAdapter->PortCfg.MBSSID[pEntry->ApIdx].DefaultKeyId);
	        DBGPRINT(RT_DEBUG_INFO, "G_Key :%x %x %x %x %x %x %x %x \n",GTK[0],GTK[1],GTK[2],GTK[3],GTK[4],GTK[5],GTK[6],GTK[7]);
	        DBGPRINT(RT_DEBUG_INFO, "       %x %x %x %x %x %x %x %x \n",GTK[8],GTK[9],GTK[10],GTK[11],GTK[12],GTK[13],GTK[14],GTK[15]);
	        DBGPRINT(RT_DEBUG_INFO, "TX Mic:%x %x %x %x %x %x %x %x \n",GTK[16],GTK[17],GTK[18],GTK[19],GTK[20],GTK[21],GTK[22],GTK[23]);
	        DBGPRINT(RT_DEBUG_INFO, "RX Mic:%x %x %x %x %x %x %x %x \n",GTK[24],GTK[25],GTK[26],GTK[27],GTK[28],GTK[29],GTK[30],GTK[31]);

	        if (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].GroupKeyWepStatus == Ndis802_11Encryption2Enabled)
                pAdapter->PortCfg.MBSSID[pEntry->ApIdx].SharedKey[pAdapter->PortCfg.MBSSID[pEntry->ApIdx].DefaultKeyId].CipherAlg = CIPHER_TKIP;
            else if (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].GroupKeyWepStatus == Ndis802_11Encryption3Enabled)
                pAdapter->PortCfg.MBSSID[pEntry->ApIdx].SharedKey[pAdapter->PortCfg.MBSSID[pEntry->ApIdx].DefaultKeyId].CipherAlg = CIPHER_AES;
            else
                pAdapter->PortCfg.MBSSID[pEntry->ApIdx].SharedKey[pAdapter->PortCfg.MBSSID[pEntry->ApIdx].DefaultKeyId].CipherAlg = CIPHER_NONE;
              
            // install Group Key to MAC ASIC
            AsicAddSharedKeyEntry(
                pAdapter, 
                pEntry->ApIdx, 
                pAdapter->PortCfg.MBSSID[pEntry->ApIdx].DefaultKeyId, 
                pAdapter->PortCfg.MBSSID[pEntry->ApIdx].SharedKey[pAdapter->PortCfg.MBSSID[pEntry->ApIdx].DefaultKeyId].CipherAlg, 
                pAdapter->PortCfg.MBSSID[pEntry->ApIdx].SharedKey[pAdapter->PortCfg.MBSSID[pEntry->ApIdx].DefaultKeyId].Key, 
                pAdapter->PortCfg.MBSSID[pEntry->ApIdx].SharedKey[pAdapter->PortCfg.MBSSID[pEntry->ApIdx].DefaultKeyId].TxMic, 
                pAdapter->PortCfg.MBSSID[pEntry->ApIdx].SharedKey[pAdapter->PortCfg.MBSSID[pEntry->ApIdx].DefaultKeyId].RxMic);

	        NdisZeroMemory(Key_Data, 512); //sizeof(Key_Data));

			if ((pAdapter->PortCfg.MBSSID[pEntry->ApIdx].AuthMode == Ndis802_11AuthModeWPAPSKWPA2PSK) ||
				(pAdapter->PortCfg.MBSSID[pEntry->ApIdx].AuthMode == Ndis802_11AuthModeWPA1WPA2))
			{
				RSNIE_Len = pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSNIE_Len[1];
	        	NdisMoveMemory(&Key_Data[2], pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSN_IE[1], RSNIE_Len);
			}
			else
			{
				RSNIE_Len = pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSNIE_Len[0];
		        NdisMoveMemory(&Key_Data[2], &pAdapter->PortCfg.MBSSID[pEntry->ApIdx].RSN_IE[0], RSNIE_Len);
			}

			Packet.Body_Len[1] = 2+RSNIE_Len;

			Key_Data[0] = IE_WPA2;
        	Key_Data[1] = RSNIE_Len;

			// Key Data Encapsulation format
        	Key_Data[2+RSNIE_Len+0] = 0xDD;
        	Key_Data[2+RSNIE_Len+2] = 0x00;
        	Key_Data[2+RSNIE_Len+3] = 0x0F;
        	Key_Data[2+RSNIE_Len+4] = 0xAC;
        	Key_Data[2+RSNIE_Len+5] = 0x01;

			// GTK Key Data Encapsulation format
        	Key_Data[2+RSNIE_Len+6] = (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].DefaultKeyId & 0x03);
			Key_Data[2+RSNIE_Len+7] = 0x00;

			// handle the difference between TKIP and AES-CCMP
			if (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].GroupKeyWepStatus == Ndis802_11Encryption3Enabled)
	        {
	        	Key_Data[2+RSNIE_Len+1] = 0x16;// 4+2+16(OUI+GTK+GTKKEY)
	        	NdisMoveMemory(&Key_Data[2+RSNIE_Len+8], GTK, LEN_AES_KEY);
	        	Packet.Body_Len[1] += 8+LEN_AES_KEY;
			}
			else
			{
				Key_Data[2+RSNIE_Len+1] = 0x26;// 4+2+32(OUI+GTK+GTKKEY)
				NdisMoveMemory(&Key_Data[2+RSNIE_Len+8], GTK, TKIP_GTK_LENGTH);
	        	Packet.Body_Len[1] += 8+TKIP_GTK_LENGTH;
			}

			// Still dont know why, but if not append will occur "GTK not include in MSG3"
			// Patch for compatibility between zero config and funk
			if (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].GroupKeyWepStatus == Ndis802_11Encryption3Enabled)
			{
				Key_Data[2+RSNIE_Len+8+TKIP_GTK_LENGTH] = 0xDD;
				Key_Data[2+RSNIE_Len+8+TKIP_GTK_LENGTH+1] = 0;
				Packet.Body_Len[1] += 2;
			}
			else
			{
				Key_Data[2+RSNIE_Len+8+TKIP_GTK_LENGTH] = 0xDD;
				Key_Data[2+RSNIE_Len+8+TKIP_GTK_LENGTH+1] = 0;
				Key_Data[2+RSNIE_Len+8+TKIP_GTK_LENGTH+2] = 0;
				Key_Data[2+RSNIE_Len+8+TKIP_GTK_LENGTH+3] = 0;
				Key_Data[2+RSNIE_Len+8+TKIP_GTK_LENGTH+4] = 0;
				Key_Data[2+RSNIE_Len+8+TKIP_GTK_LENGTH+5] = 0;
				Packet.Body_Len[1] += 6;
			}

        	key_length = Packet.Body_Len[1];
        	Packet.KeyDesc.DataLen[1] = key_length;
        	Packet.Body_Len[1] += 93 + 2; 

	        NdisZeroMemory(mic, sizeof(mic));

	        // count MIC
	        if (pEntry->WepStatus == Ndis802_11Encryption3Enabled)
	        {
	            AES_GTK_KEY_WRAP(&pEntry->PTK[16], Key_Data, key_length, pRc4GTK);
	            // AES wrap function will grow 8 bytes in length
	            NdisMoveMemory(Packet.KeyDesc.Data, pRc4GTK, (key_length + 8));
	            Packet.Body_Len[1] += 8;
	            Packet.KeyDesc.DataLen[1] += 8;
	            MakeOutgoingFrame(OutBuffer,            &FrameLen,
	                            Packet.Body_Len[1] + 4, &Packet,
	                            END_OF_ARGS);

	            HMAC_SHA1(OutBuffer, FrameLen, pEntry->PTK, LEN_EAP_MICK, digest);
	            NdisMoveMemory(Packet.KeyDesc.MIC, digest, LEN_KEY_DESC_MIC);
 	        }
	        else
	        {
	            // PREPARE Encrypted  "Key DATA" field.  (Encrypt GTK with RC4, usinf PTK[16]->[31] as Key, IV-field as IV)
	            // put TxTsc in Key RSC field
	            pAdapter->PrivateInfo.FCSCRC32 = PPPINITFCS32;   //Init crc32.

	            // ekey is the contanetion of IV-field, and PTK[16]->PTK[31]
	            NdisMoveMemory(ekey, Packet.KeyDesc.IV, LEN_KEY_DESC_IV);
	            NdisMoveMemory(&ekey[LEN_KEY_DESC_IV], &pEntry->PTK[16], LEN_EAP_EK);
	            ARCFOUR_INIT(&pAdapter->PrivateInfo.WEPCONTEXT, ekey, sizeof(ekey));  //INIT SBOX, KEYLEN+3(IV)
	            pAdapter->PrivateInfo.FCSCRC32 = RTMP_CALC_FCS32(pAdapter->PrivateInfo.FCSCRC32, Key_Data, key_length);
	            WPAARCFOUR_ENCRYPT(&pAdapter->PrivateInfo.WEPCONTEXT, pRc4GTK,Key_Data, key_length);
	            NdisMoveMemory(Packet.KeyDesc.Data, pRc4GTK, key_length);

	            // make a frame for Countint MIC,
	            MakeOutgoingFrame(OutBuffer,            &FrameLen,
	                            Packet.Body_Len[1] + 4, &Packet,
	                            END_OF_ARGS);

	            hmac_md5(pEntry->PTK, LEN_EAP_MICK, OutBuffer, FrameLen, mic);
	            NdisMoveMemory(Packet.KeyDesc.MIC, mic, LEN_KEY_DESC_MIC);
	        }
        }
            
        // 5-6, Make outgoing frame
        WPAMake8023Hdr(pAdapter, pEntry, Header802_3);
        MakeOutgoingFrame(OutBuffer,            &FrameLen,
                            LENGTH_802_3,            Header802_3,
                            Packet.Body_Len[1] + 4,  &Packet,
                            END_OF_ARGS);
            
        DBGPRINT(RT_DEBUG_TRACE, "RTMPToWirelessSta : ETHTYPE = %x %x FrameLen = %d! \n",Header802_3[12],Header802_3[13],FrameLen);
//iversonadd
        RTMPToWirelessSta(pAdapter, OutBuffer, FrameLen, FALSE);

        // Should start from 1, init is 0
		pEntry->ReTryCounter = PEER_MSG3_RETRY_TIMER_CTR;
		pEntry->RetryTimer.expires = jiffies + PEER_MSG3_RETRY_EXEC_INTV;
		pEntry->RetryTimerRunning = TRUE;
        mod_timer(&pEntry->RetryTimer, pEntry->RetryTimer.expires);
		DBGPRINT(RT_DEBUG_TRACE, "Send Msg3 and setup timeout timer\n");

        pEntry->WpaState = AS_PTKINIT_NEGOTIATING;
    }while(FALSE);

	kfree(mpool);
}

/*
    ==========================================================================
    Description:
        countermeasures active attack timer execution
    Return:
    ==========================================================================
*/
VOID CMTimerExec(
    IN  unsigned long data) 
{
    UINT            i, j=0, apidx;
    RTMP_ADAPTER    *pAdapter = (RTMP_ADAPTER *)data;
        
    for (apidx = 0; apidx < pAdapter->PortCfg.BssidNum; apidx++)
    {
        pAdapter->PortCfg.MBSSID[apidx].BANClass3Data = FALSE;
    }

    for (i = 0; i < MAX_LEN_OF_MAC_TABLE; i++)
    {
        if ((pAdapter->MacTab.Content[i].Valid == TRUE) && (pAdapter->MacTab.Content[i].CMTimerRunning == TRUE))
        {
            pAdapter->MacTab.Content[i].CMTimerRunning = FALSE;
            j++;
        }
    }

    if (j > 1)
        DBGPRINT(RT_DEBUG_ERROR, "Find more than one entry which generated MIC Fail ..  \n");

    pAdapter->PortCfg.CMTimerRunning = FALSE;
}
    
VOID EnqueueStartForPSKExec(
    IN  unsigned long data) 
{
	MAC_TABLE_ENTRY     *pEntry = (PMAC_TABLE_ENTRY) data;

	if ((pEntry) && (pEntry->EnqueueStartForPSKTimerRunning == TRUE))
	{
		RTMP_ADAPTER		*pAd = (PRTMP_ADAPTER)pEntry->pAd;

		pEntry->EnqueueStartForPSKTimerRunning = FALSE;	
		DBGPRINT(RT_DEBUG_TRACE, "Execute enqueue EAPoL-Start for %02x:%02x:%02x:%02x:%02x:%02x \n",
								pEntry->Addr[0], pEntry->Addr[1], pEntry->Addr[2],
    							pEntry->Addr[3], pEntry->Addr[4], pEntry->Addr[5]);
		
		MlmeEnqueue(pAd, WPA_STATE_MACHINE, MACHINE_TYPE_EAPOLStart, 6, &pEntry->Addr);		
	}				
}
	
VOID WPARetryExec(
    IN  unsigned long data) 
{
    MAC_TABLE_ENTRY     *pEntry = (PMAC_TABLE_ENTRY) data;
    
    if ((pEntry) && (pEntry->RetryTimerRunning == TRUE))
    {
    	RTMP_ADAPTER		*pAdapter = (PRTMP_ADAPTER)pEntry->pAd;
    	
        // Turn off the retry flag first
        pEntry->RetryTimerRunning = FALSE;  
        DBGPRINT(RT_DEBUG_TRACE, "WPARetryExec - ReTryCounter=%d, WpaState=%d \n", pEntry->ReTryCounter, pEntry->WpaState);

        switch (pEntry->AuthMode)
        {
            case Ndis802_11AuthModeWPA:
            case Ndis802_11AuthModeWPAPSK:
            case Ndis802_11AuthModeWPA2:
            case Ndis802_11AuthModeWPA2PSK:
                //  1.   GTK already retried, give up and disconnect client.
                if (pEntry->ReTryCounter > (GRP_HS_RETRY_TIMER_CTR + 1))
                {    
					// Deauthenticate and delete timer and entry
					DisAssocAction(pAdapter, pEntry, SUBTYPE_DISASSOC, REASON_GROUP_KEY_UPDATE_TIMEOUT);
                    DBGPRINT(RT_DEBUG_TRACE, "Delete client after GTK retry failed\n");
                }
				// 2. Retry GTK.
                else if (pEntry->ReTryCounter >= GRP_HS_RETRY_TIMER_CTR)
                {
                    DBGPRINT(RT_DEBUG_TRACE, "ReInitilize 2-way group-key Handshake ===> \n");
                    WPAHardTransmit( pAdapter,  pEntry);
	                pEntry->RetryTimer.expires = jiffies + WPA_RETRY_EXEC_INTV;
    				mod_timer(&pEntry->RetryTimer, pEntry->RetryTimer.expires);
					pEntry->ReTryCounter++;
                    pEntry->RetryTimerRunning = TRUE;  
                }
				// 3. 4-way message 1 retried more than three times. Disconnect client
                else if (pEntry->ReTryCounter > (PEER_MSG1_RETRY_TIMER_CTR + 3))                
                {   
					// Deauthenticate and delete timer and entry
					DisAssocAction(pAdapter, pEntry, SUBTYPE_DEAUTH, REASON_UNSPECIFY);
                    DBGPRINT(RT_DEBUG_TRACE, "Delete client after 3 retries of message_1 \n");
                }
				// 4. Retry 4 way message 1, the last try, the timeout is 5 sec for EAPOL-Start
                else if (pEntry->ReTryCounter == (PEER_MSG1_RETRY_TIMER_CTR + 3))                
                {
                    DBGPRINT(RT_DEBUG_TRACE, "Message 1 time out, the last retry\n");
	              pEntry->PrivacyFilter = Ndis802_11PrivFilter8021xWEP;
	                pEntry->WpaState = AS_INITPSK;
	                pEntry->PortSecured = WPA_802_1X_PORT_NOT_SECURED;
	                pEntry->RetryTimer.expires = jiffies + (5 * HZ);
                    WPAStart4WayHS(pAdapter, pEntry);
                }
				// 4. Retry 4 way message 1
                else if (pEntry->ReTryCounter < (PEER_MSG1_RETRY_TIMER_CTR + 3))                
                {
                    DBGPRINT(RT_DEBUG_TRACE, "Message 1 time out, retry it\n");
	                pEntry->PrivacyFilter = Ndis802_11PrivFilter8021xWEP;
	                pEntry->WpaState = AS_INITPSK;
	                pEntry->PortSecured = WPA_802_1X_PORT_NOT_SECURED;
	                pEntry->RetryTimer.expires = jiffies + WPA_RETRY_EXEC_INTV;
                    WPAStart4WayHS(pAdapter, pEntry);
                }
                break;

            default:
                break;
        }        
    }
}

/*
    ==========================================================================
    Description:
        When receiving the last packet of 4-way pairwisekey handshake.
        Initilize 2-way groupkey handshake following.
    Return:
    ==========================================================================
*/
VOID Wpa1PeerPairMsg4Action(
    IN PRTMP_ADAPTER    pAdapter, 
    IN MAC_TABLE_ENTRY  *pEntry,
    IN MLME_QUEUE_ELEM  *Elem) 
{
    UCHAR               mic[LEN_KEY_DESC_MIC], digest[80];
    ULONG               MICMsgLen;
    EAPOL_PACKET        EAPOLPKT;
    PEAPOL_PACKET       pMsg4;
    NDIS_STATUS         Status;
    PHEADER_802_11      pHeader;
    UCHAR               Offset = 0;

    DBGPRINT(RT_DEBUG_TRACE, "WpaEAPOL Peer Pair Msg4 Action ===>\n");

    if (!pEntry)
    	return;

    if (pEntry->WpaState < AS_PTKINIT_NEGOTIATING)
        return;

    pHeader = (PHEADER_802_11) Elem->Msg;

    if (pHeader->FC.SubType & 0x08)
        Offset += 2;
    
    pMsg4 = (PEAPOL_PACKET)&Elem->Msg[LENGTH_802_11 + LENGTH_802_1_H + Offset];

    // 1.check Replay Counter
    if (!RTMPEqualMemory(pMsg4->KeyDesc.RCounter, pEntry->R_Counter, LEN_KEY_DESC_REPLAY))
    {
        DBGPRINT(RT_DEBUG_ERROR, "Replay Counter Different in Msg 4 of 4-way handshake!! \n");
        DBGPRINT(RT_DEBUG_ERROR, "Receive : %d %d %d %d  \n", pMsg4->KeyDesc.RCounter[0],pMsg4->KeyDesc.RCounter[1],pMsg4->KeyDesc.RCounter[2],pMsg4->KeyDesc.RCounter[3]);
        DBGPRINT(RT_DEBUG_ERROR, "Current : %d %d %d %d  \n", pEntry->R_Counter[4],pEntry->R_Counter[5],pEntry->R_Counter[6],pEntry->R_Counter[7]);
        return;
    }

    // 2. check MIC, if not valid, discard silently
    MICMsgLen = pMsg4->Body_Len[1] | ((pMsg4->Body_Len[0]<<8) && 0xff00);
    MICMsgLen += LENGTH_EAPOL_H;
    if (MICMsgLen > (Elem->MsgLen -LENGTH_802_11 - LENGTH_802_1_H))
    {
        DBGPRINT(RT_DEBUG_ERROR, "Receive wrong format EAPOL packets \n");
        return;        
    }
    NdisZeroMemory((PUCHAR)&EAPOLPKT, sizeof(EAPOLPKT));
    NdisMoveMemory((PUCHAR)&EAPOLPKT, pMsg4, MICMsgLen);  
    NdisZeroMemory(EAPOLPKT.KeyDesc.MIC, sizeof(EAPOLPKT.KeyDesc.MIC));

    hmac_md5(pEntry->PTK, LEN_EAP_MICK, (PUCHAR)&EAPOLPKT, (MICMsgLen), mic);
    if (pEntry->WepStatus == Ndis802_11Encryption2Enabled)
    {
        hmac_md5(pEntry->PTK, LEN_EAP_MICK, (PUCHAR)&EAPOLPKT, (MICMsgLen), mic);
    }
    else
    {
        HMAC_SHA1((PUCHAR)&EAPOLPKT,  MICMsgLen, pEntry->PTK, LEN_EAP_MICK, digest);
        NdisMoveMemory(mic, digest, LEN_KEY_DESC_MIC);
    }
    if (!RTMPEqualMemory(pMsg4->KeyDesc.MIC, mic, LEN_KEY_DESC_MIC))
    {
        DBGPRINT(RT_DEBUG_ERROR, "MIC different in Msg 4 of 4-way handshake!! \n");
        return;
    }
    else
        DBGPRINT(RT_DEBUG_INFO, "MIC valid in Msg 4 of 4-way handshake!! \n");

    // 3. uses the MLME.SETKEYS.request to configure PTK into MAC
    NdisZeroMemory(&pEntry->PairwiseKey, sizeof(CIPHER_KEY));   
    NdisMoveMemory(pEntry->PairwiseKey.RxTsc, EAPOLPKT.KeyDesc.RSC, 6);

    pEntry->PairwiseKey.KeyLen = LEN_TKIP_EK;
    NdisMoveMemory(pEntry->PairwiseKey.Key, &pEntry->PTK[32], LEN_TKIP_EK);
    NdisMoveMemory(pEntry->PairwiseKey.RxMic, &pEntry->PTK[TKIP_AP_RXMICK_OFFSET], LEN_TKIP_RXMICK);
    NdisMoveMemory(pEntry->PairwiseKey.TxMic, &pEntry->PTK[TKIP_AP_TXMICK_OFFSET], LEN_TKIP_TXMICK);

    DBGPRINT(RT_DEBUG_INFO, "WPA1(PairwiseKey) = %02x:%02x:%02x:%02x-%02x:%02x:%02x:%02x\n",
    	pEntry->PairwiseKey.Key[0],pEntry->PairwiseKey.Key[1],pEntry->PairwiseKey.Key[2],pEntry->PairwiseKey.Key[3],
    	pEntry->PairwiseKey.Key[4],pEntry->PairwiseKey.Key[5],pEntry->PairwiseKey.Key[6],pEntry->PairwiseKey.Key[7]);
    DBGPRINT(RT_DEBUG_INFO, "WPA1(RxMic) = %02x:%02x:%02x:%02x-%02x:%02x:%02x:%02x\n",
    	pEntry->PairwiseKey.RxMic[0],pEntry->PairwiseKey.RxMic[1],pEntry->PairwiseKey.RxMic[2],pEntry->PairwiseKey.RxMic[3],
    	pEntry->PairwiseKey.RxMic[4],pEntry->PairwiseKey.RxMic[5],pEntry->PairwiseKey.RxMic[6],pEntry->PairwiseKey.RxMic[7]);
    DBGPRINT(RT_DEBUG_INFO, "WPA1(TxMic) = %02x:%02x:%02x:%02x-%02x:%02x:%02x:%02x\n",
    	pEntry->PairwiseKey.TxMic[0],pEntry->PairwiseKey.TxMic[1],pEntry->PairwiseKey.TxMic[2],pEntry->PairwiseKey.TxMic[3],
    	pEntry->PairwiseKey.TxMic[4],pEntry->PairwiseKey.TxMic[5],pEntry->PairwiseKey.TxMic[6],pEntry->PairwiseKey.TxMic[7]);

	// Set pairwise key to Asic
    {
        pEntry->PairwiseKey.CipherAlg = CIPHER_NONE;
        if (pEntry->WepStatus == Ndis802_11Encryption2Enabled)
            pEntry->PairwiseKey.CipherAlg = CIPHER_TKIP;
        else if (pEntry->WepStatus == Ndis802_11Encryption3Enabled)
            pEntry->PairwiseKey.CipherAlg = CIPHER_AES;
        AsicAddPairwiseKeyEntry(
            pAdapter, 
            pEntry->Addr, 
            (UCHAR)pEntry->Aid,
            pEntry->PairwiseKey.CipherAlg, 
            pEntry->PairwiseKey.Key,
            pEntry->PairwiseKey.TxMic, 
            pEntry->PairwiseKey.RxMic);
    }
    
    // 4. upgrade state
    pEntry->PrivacyFilter = Ndis802_11PrivFilterAcceptAll;
    pEntry->WpaState = AS_PTKINITDONE;
    pAdapter->PortCfg.MBSSID[pEntry->ApIdx].PortSecured = WPA_802_1X_PORT_SECURED;
    pEntry->PortSecured = WPA_802_1X_PORT_SECURED;

#ifdef WSC_SUPPORT
    if (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].WscControl.WscConfMode != WSC_DISABLE)
        WscInformFromWPA(pEntry);
#endif // WSC_SUPPORT //

    // 5. init Group 2-way handshake if necessary.
    if ((Status = WPAHardTransmit(pAdapter, pEntry)) != NDIS_STATUS_SUCCESS)
        return;
    
    pEntry->RetryTimerRunning = TRUE;
    pEntry->RetryTimer.expires = jiffies + WPA_RETRY_EXEC_INTV;
    pEntry->ReTryCounter = GRP_HS_RETRY_TIMER_CTR;
	mod_timer(&pEntry->RetryTimer, pEntry->RetryTimer.expires);

    DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) recv WpaEAPOL Peer PAIR Msg4 Action and send GROUP Msg1\n", pEntry->ApIdx);
}

/*
    ==========================================================================
    Description:
        When receiving the last packet of 4-way pairwisekey handshake.
    Return:
    ==========================================================================
*/
VOID Wpa2PeerPairMsg4Action(
    IN PRTMP_ADAPTER	pAdapter, 
    IN MAC_TABLE_ENTRY	*pEntry,
    IN MLME_QUEUE_ELEM	*Elem) 
{
	UINT                Len;
    UCHAR               mic[LEN_KEY_DESC_MIC], digest[80], PMK_key[20];
    PUCHAR              pData;
    PEAPOL_PACKET       pMsg2;
    EAPOL_PACKET        EAPOLPKT;
    VOID                *Msg;
    UINT                MsgLen;
    PHEADER_802_11      pHeader;

    if(!pEntry)
	    return;

    pHeader = (PHEADER_802_11)Elem->Msg;
    if (pHeader->FC.SubType & 0x08)
    {
        Msg = &Elem->Msg[LENGTH_802_11 + 2];
        MsgLen = Elem->MsgLen - LENGTH_802_11 - 2;
    }
    else
    {
        Msg = &Elem->Msg[LENGTH_802_11];
        MsgLen = Elem->MsgLen - LENGTH_802_11;
    }

    DBGPRINT(RT_DEBUG_TRACE, "Wpa2PeerPairMsg4Action ===> from MAC:%02x:%02x:%02x:%02x:%02x:%02x\n",
    	pEntry->Addr[0], pEntry->Addr[1], pEntry->Addr[2],pEntry->Addr[3],pEntry->Addr[4],pEntry->Addr[5]);

    Len = MsgLen - LENGTH_802_1_H;
    pData = (PUCHAR)Msg;
    NdisZeroMemory((PUCHAR)&EAPOLPKT, sizeof(EAPOLPKT));
    NdisZeroMemory(mic, sizeof(mic));

    pMsg2 = (PEAPOL_PACKET) (pData + LENGTH_802_1_H);

    // 1. verify the Reaply counter, if not valid,
    if (!RTMPEqualMemory(pMsg2->KeyDesc.RCounter, pEntry->R_Counter, LEN_KEY_DESC_REPLAY))
    {
        DBGPRINT(RT_DEBUG_ERROR, "Replay Counter Different in Msg 4 of 4-way handshake!!!\n");
        DBGPRINT(RT_DEBUG_ERROR, "Receive : %d %d %d %d \n", pMsg2->KeyDesc.RCounter[4], pMsg2->KeyDesc.RCounter[5], pMsg2->KeyDesc.RCounter[6], pMsg2->KeyDesc.RCounter[7]);
        DBGPRINT(RT_DEBUG_ERROR, "Current : %d %d %d %d \n",pEntry->R_Counter[4],pEntry->R_Counter[5],pEntry->R_Counter[6],pEntry->R_Counter[7]);
        return;
    }
    else
        DBGPRINT(RT_DEBUG_TRACE, "Replay Counter VALID in Msg 4 of 4-way handshake!\n");

    // 2. verify MIC, 
    NdisMoveMemory(&EAPOLPKT, pMsg2, Len);  
    NdisZeroMemory(&EAPOLPKT.KeyDesc.MIC, LEN_KEY_DESC_MIC);
    if (pEntry->WepStatus == Ndis802_11Encryption2Enabled)
    {
        hmac_md5(pEntry->PTK, LEN_EAP_MICK, (UCHAR *)&EAPOLPKT, Len, mic);
    }
    else
    {
        HMAC_SHA1((UCHAR *)&EAPOLPKT,   Len, pEntry->PTK, LEN_EAP_MICK, digest);
        NdisMoveMemory(mic, digest, LEN_KEY_DESC_MIC);
    }
    if (!RTMPEqualMemory(pMsg2->KeyDesc.MIC, mic, LEN_KEY_DESC_MIC))
    {
        DBGPRINT(RT_DEBUG_ERROR, "MIC Different in Msg 4 of 4-way handshake!! \n");
        return;
    }
    else
        DBGPRINT(RT_DEBUG_TRACE, "MIC Valid in Msg 4 of 4-way handshake!! \n");

    // 3.  upgrade state
    if (pEntry->RetryTimerRunning == TRUE)
    {
        RTMPCancelTimer(&pEntry->RetryTimer);
        pEntry->RetryTimerRunning = FALSE;              
    }

    NdisZeroMemory(&pEntry->PairwiseKey, sizeof(CIPHER_KEY));   
    NdisMoveMemory(pEntry->PairwiseKey.RxTsc, EAPOLPKT.KeyDesc.RSC, 6);

    pEntry->PairwiseKey.KeyLen = LEN_TKIP_EK;
    NdisMoveMemory(pEntry->PairwiseKey.Key, &pEntry->PTK[32], LEN_TKIP_EK);
    NdisMoveMemory(pEntry->PairwiseKey.RxMic, &pEntry->PTK[TKIP_AP_RXMICK_OFFSET], LEN_TKIP_RXMICK);
    NdisMoveMemory(pEntry->PairwiseKey.TxMic, &pEntry->PTK[TKIP_AP_TXMICK_OFFSET], LEN_TKIP_TXMICK);

	// Set pairwise key to Asic
    {
        pEntry->PairwiseKey.CipherAlg = CIPHER_NONE;
        if (pEntry->WepStatus == Ndis802_11Encryption2Enabled)
            pEntry->PairwiseKey.CipherAlg = CIPHER_TKIP;
        else if (pEntry->WepStatus == Ndis802_11Encryption3Enabled)
            pEntry->PairwiseKey.CipherAlg = CIPHER_AES;
        AsicAddPairwiseKeyEntry(
            pAdapter, 
            pEntry->Addr, 
            (UCHAR)pEntry->Aid,
            pEntry->PairwiseKey.CipherAlg, 
            pEntry->PairwiseKey.Key,
            pEntry->PairwiseKey.TxMic, 
            pEntry->PairwiseKey.RxMic);
    }
    
    pEntry->PrivacyFilter = Ndis802_11PrivFilterAcceptAll;
    pEntry->WpaState = AS_PTKINITDONE;
    pAdapter->PortCfg.MBSSID[pEntry->ApIdx].PortSecured = WPA_802_1X_PORT_SECURED;
    pEntry->PortSecured = WPA_802_1X_PORT_SECURED;
    pEntry->GTKState = REKEY_ESTABLISHED;

#ifdef WSC_SUPPORT
    if (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].WscControl.WscConfMode != WSC_DISABLE)
        WscInformFromWPA(pEntry);
#endif // WSC_SUPPORT //

	if (pEntry->AuthMode == Ndis802_11AuthModeWPA2)
	{
		// Calculate PMKID
		NdisMoveMemory(&PMK_key[0], "PMK Name", 8);
		NdisMoveMemory(&PMK_key[8], pAdapter->PortCfg.MBSSID[pEntry->ApIdx].Bssid, MAC_ADDR_LEN);
		NdisMoveMemory(&PMK_key[14], pEntry->Addr, MAC_ADDR_LEN);
		HMAC_SHA1(PMK_key, 20, pAdapter->PortCfg.MBSSID[pEntry->ApIdx].PMK, PMK_LEN, digest);
		RTMPAddPMKIDCache(pAdapter, pEntry->ApIdx, pEntry->Addr, digest, pAdapter->PortCfg.MBSSID[pEntry->ApIdx].PMK);
		DBGPRINT(RT_DEBUG_TRACE, "Calc PMKID=%02x:%02x:%02x:%02x:%02x:%02x\n", digest[0],digest[1],digest[2],digest[3],digest[4],digest[5]);
	}

    DBGPRINT(RT_DEBUG_TRACE, "===> AP SETKEYS DONE (ra%d) - WPA2, AuthMode=%d, WepStatus=%d \n\n", pEntry->ApIdx, pEntry->AuthMode, pEntry->WepStatus);
}

/*
    ==========================================================================
    Description:
        When receiving the last packet of 2-way groupkey handshake.
    Return:
    ==========================================================================
*/
VOID PeerGroupMsg2Action(
    IN PRTMP_ADAPTER    pAdapter, 
    IN MAC_TABLE_ENTRY  *pEntry,
    IN VOID             *Msg,
    IN UINT             MsgLen)
{
    UINT                Len;
    UCHAR               mic[LEN_KEY_DESC_MIC], digest[80];
    PUCHAR              pData;
    PEAPOL_PACKET       pMsg2;
    EAPOL_PACKET        EAPOLPKT;

    if (!pEntry)
	    return;
    
    DBGPRINT(RT_DEBUG_TRACE, "PeerGroupMsg2Action ===> from MAC(%02x:%02x:%02x:%02x:%02x:%02x)\n",
    	pEntry->Addr[0], pEntry->Addr[1], pEntry->Addr[2],pEntry->Addr[3],pEntry->Addr[4],pEntry->Addr[5]);

    Len = MsgLen - LENGTH_802_1_H;
    pData = (PUCHAR)Msg;
    NdisZeroMemory((PUCHAR)&EAPOLPKT, sizeof(EAPOLPKT));
    NdisZeroMemory(mic, sizeof(mic));

    if (MsgLen < (LENGTH_802_1_H + LENGTH_EAPOL_H + sizeof(KEY_DESCRIPTER) -MAX_LEN_OF_RSNIE -2 ) )
        return;
            
    if (pEntry->WpaState != AS_PTKINITDONE)
        return;
        
    pMsg2 = (PEAPOL_PACKET) (pData + LENGTH_802_1_H);

    // 1. verify the Reaply counter, if not valid,
    if (!RTMPEqualMemory(pMsg2->KeyDesc.RCounter, pEntry->R_Counter, LEN_KEY_DESC_REPLAY))
    {
        DBGPRINT(RT_DEBUG_ERROR, "Replay Counter Different in Msg 2 of GROUP 2-way handshake!!!\n");
        DBGPRINT(RT_DEBUG_ERROR, "Receive : %d %d %d %d \n", pMsg2->KeyDesc.RCounter[4], pMsg2->KeyDesc.RCounter[5], pMsg2->KeyDesc.RCounter[6], pMsg2->KeyDesc.RCounter[7]);
        DBGPRINT(RT_DEBUG_ERROR, "Current : %d %d %d %d \n",pEntry->R_Counter[4],pEntry->R_Counter[5],pEntry->R_Counter[6],pEntry->R_Counter[7]);
        return;
    }
    else
        DBGPRINT(RT_DEBUG_TRACE, "Replay Counter VALID in Msg 2 of GROUP 2-way handshake!!!\n");

    // 2. verify MIC, 
    NdisMoveMemory(&EAPOLPKT, pMsg2, Len);  
    NdisZeroMemory(&EAPOLPKT.KeyDesc.MIC, LEN_KEY_DESC_MIC);
    if (pEntry->WepStatus == Ndis802_11Encryption2Enabled)
    {
        hmac_md5(pEntry->PTK,   LEN_EAP_MICK, (UCHAR *)&EAPOLPKT, Len, mic);
    }
    else
    {
        HMAC_SHA1((UCHAR *)&EAPOLPKT,   Len, pEntry->PTK, LEN_EAP_MICK, digest);
        NdisMoveMemory(mic, digest, LEN_KEY_DESC_MIC);
    }
    if (!RTMPEqualMemory(pMsg2->KeyDesc.MIC, mic, LEN_KEY_DESC_MIC))
    {
        DBGPRINT(RT_DEBUG_ERROR, "MIC Different in Msg 2 of GROUP 2-way handshake! \n");
        return;
    }
    else
        DBGPRINT(RT_DEBUG_TRACE, "MIC Valid in Msg 2 of GROUP 2-way handshake. \n");

    // 3.  upgrade state
    if (pEntry->RetryTimerRunning == TRUE)
    {
        RTMPCancelTimer(&pEntry->RetryTimer);
        pEntry->RetryTimerRunning = FALSE;              
    }
    pEntry->GTKState = REKEY_ESTABLISHED;

    DBGPRINT(RT_DEBUG_TRACE, "===> AP SETKEYS DONE - (ra%d) WPA1, AuthMode=%d, WepStatus=%d \n\n", pEntry->ApIdx, pEntry->AuthMode, pEntry->WepStatus);
}

/*
    ==========================================================================
    Description:
        Only for sending the first packet of 2-way groupkey handshake
    Return:
    ==========================================================================
*/
NDIS_STATUS WPAHardTransmit(
    IN PRTMP_ADAPTER    pAdapter,
    IN MAC_TABLE_ENTRY  *pEntry)
{
    UCHAR               *GTK, *pRc4GTK, *mic, *ekey, *digest, *mpool ,*Key_Data;
    ULONG               FrameLen = 0, apidx;
    UCHAR               *OutBuffer = NULL;
    UCHAR               Header802_3[14];
    EAPOL_PACKET        Packet;

    if (!pEntry)
    	return NDIS_STATUS_FAILURE;

	mpool = kmalloc(4096, MEM_ALLOC_FLAG);  // allocate memory
    if (mpool == NULL)
		return (NDIS_STATUS_FAILURE);					

	/* OutBuffer Len = MAX_LEN_OF_EAP_HS */
	OutBuffer = (UCHAR *) ROUND_UP(mpool, 4);
	/* GTK Len = TKIP_GTK_LENGTH */
	GTK = (UCHAR *) ROUND_UP(OutBuffer+MAX_LEN_OF_EAP_HS, 4);
	/* digest Len = 80 */
	digest =(UCHAR *) ROUND_UP(GTK+TKIP_GTK_LENGTH, 4);
	/* pRc4GTK Len = TKIP_GTK_LENGTH */
	pRc4GTK = (UCHAR *) ROUND_UP(digest+80, 4);
	/* ekey Len = LEN_KEY_DESC_IV+LEN_EAP_EK */
	ekey = (UCHAR *) ROUND_UP(pRc4GTK+TKIP_GTK_LENGTH, 4);
	/* mic Len = LEN_KEY_DESC_MIC */
	mic = (UCHAR *) ROUND_UP(ekey+LEN_KEY_DESC_IV+LEN_EAP_EK, 4);
	/* Key Data Len = 512 */
	Key_Data = (UCHAR *) ROUND_UP(mic+512, 4);

    NdisZeroMemory(ekey, sizeof(char)*(LEN_KEY_DESC_IV+LEN_EAP_EK));
    NdisZeroMemory(pRc4GTK, sizeof(char)*(TKIP_GTK_LENGTH));

    apidx = pEntry->ApIdx;

    do
    {
        // Increment replay counter by 1  
        ADD_ONE_To_64BIT_VAR(pAdapter->PortCfg.R_Counter);
        NdisMoveMemory(pEntry->R_Counter, pAdapter->PortCfg.R_Counter, sizeof(pEntry->R_Counter));

        // 0. init Packet and Fill header
        NdisZeroMemory(&Packet, sizeof(Packet));
        Packet.ProVer = EAPOL_VER;
        Packet.ProType = EAPOLKey;
        Packet.Body_Len[1] = LEN_MSG1_2WAY;
        
        // 2, Fill key version, keyinfo, key len
	if ((pEntry->AuthMode == Ndis802_11AuthModeWPA2) || 
	    (pEntry->AuthMode == Ndis802_11AuthModeWPA2PSK))
	{	
            Packet.KeyDesc.Type = WPA2_KEY_DESC;
            Packet.KeyDesc.Keyinfo.EKD = 1;
	}    
	else
            Packet.KeyDesc.Type = WPA1_KEY_DESC;

        Packet.KeyDesc.Keyinfo.KeyMic = 1;
        Packet.KeyDesc.Keyinfo.Secure = 1;
        Packet.KeyDesc.Keyinfo.KeyDescVer = DESC_TYPE_TKIP;
        Packet.KeyDesc.Keyinfo.KeyType = GROUPKEY;
        Packet.KeyDesc.Keyinfo.KeyIndex = pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId;
        Packet.KeyDesc.Keyinfo.KeyAck = 1;
        Packet.KeyDesc.KeyLength[1] = TKIP_GTK_LENGTH;
        Packet.KeyDesc.DataLen[1] = TKIP_GTK_LENGTH;
        if (pEntry->WepStatus == Ndis802_11Encryption3Enabled)
        {
            Packet.Body_Len[1] -= 8;
            Packet.KeyDesc.KeyLength[1] = LEN_AES_KEY;
            Packet.KeyDesc.DataLen[1] = LEN_AES_KEY + 8;
            Packet.KeyDesc.Keyinfo.KeyDescVer = DESC_TYPE_AES;
        }

#ifdef BIG_ENDIAN
{
		USHORT	tmpKeyinfo;

		NdisMoveMemory(&tmpKeyinfo, &Packet.KeyDesc.Keyinfo, sizeof(USHORT)); 
		tmpKeyinfo = SWAP16(tmpKeyinfo);
		NdisMoveMemory(&Packet.KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT)); 
}

//        *(USHORT *)&Packet.KeyDesc.Keyinfo = SWAP16(*(USHORT *)&Packet.KeyDesc.Keyinfo);
#endif

        CountGTK(pAdapter->PortCfg.MBSSID[apidx].GMK, (UCHAR*)pAdapter->PortCfg.MBSSID[apidx].GNonce, pAdapter->PortCfg.MBSSID[pEntry->ApIdx].Bssid, GTK, TKIP_GTK_LENGTH);
        DBGPRINT(RT_DEBUG_TRACE, "IF(ra%d) WPA Group Key ID  = %d\n", apidx, pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId);
        DBGPRINT(RT_DEBUG_INFO, "%x %x %x %x %x %x %x %x \n",GTK[0],GTK[1],GTK[2],GTK[3],GTK[4],GTK[5],GTK[6],GTK[7]);
        DBGPRINT(RT_DEBUG_INFO, "%x %x %x %x %x %x %x %x \n",GTK[8],GTK[9],GTK[10],GTK[11],GTK[12],GTK[13],GTK[14],GTK[15]);
        DBGPRINT(RT_DEBUG_INFO, "%x %x %x %x %x %x %x %x \n",GTK[16],GTK[17],GTK[18],GTK[19],GTK[20],GTK[21],GTK[22],GTK[23]);
        DBGPRINT(RT_DEBUG_INFO, "%x %x %x %x %x %x %x %x \n",GTK[24],GTK[25],GTK[26],GTK[27],GTK[28],GTK[29],GTK[30],GTK[31]);
                
        NdisMoveMemory(pAdapter->PortCfg.MBSSID[apidx].SharedKey[pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId].Key, GTK, LEN_TKIP_EK);
        NdisMoveMemory(pAdapter->PortCfg.MBSSID[apidx].SharedKey[pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId].TxMic, &GTK[16], LEN_TKIP_TXMICK);
        NdisMoveMemory(pAdapter->PortCfg.MBSSID[apidx].SharedKey[pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId].RxMic, &GTK[24], LEN_TKIP_RXMICK);

        if ((pAdapter->PortCfg.MBSSID[pEntry->ApIdx].GroupKeyWepStatus == Ndis802_11Encryption2Enabled))
            pAdapter->PortCfg.MBSSID[apidx].SharedKey[pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId].CipherAlg = CIPHER_TKIP;
        else if ((pAdapter->PortCfg.MBSSID[pEntry->ApIdx].GroupKeyWepStatus == Ndis802_11Encryption3Enabled))
            pAdapter->PortCfg.MBSSID[apidx].SharedKey[pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId].CipherAlg = CIPHER_AES;
        else
            pAdapter->PortCfg.MBSSID[apidx].SharedKey[pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId].CipherAlg = CIPHER_NONE;
            
        // install Group Key to MAC ASIC
        AsicAddSharedKeyEntry(
            pAdapter, 
            apidx, 
            pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId, 
            pAdapter->PortCfg.MBSSID[apidx].SharedKey[pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId].CipherAlg, 
            pAdapter->PortCfg.MBSSID[apidx].SharedKey[pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId].Key, 
            pAdapter->PortCfg.MBSSID[apidx].SharedKey[pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId].TxMic, 
            pAdapter->PortCfg.MBSSID[apidx].SharedKey[pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId].RxMic);
        
     	if ((pEntry->AuthMode == Ndis802_11AuthModeWPA2) || 
	(pEntry->AuthMode == Ndis802_11AuthModeWPA2PSK))
        {
	    UCHAR  key_length ;
	
            NdisZeroMemory(Key_Data, 512); //sizeof(Key_Data));
	    NdisZeroMemory(Packet.KeyDesc.MIC, sizeof(Packet.KeyDesc.MIC));
	    
            Packet.Body_Len[1] = 0;
            // Key Data Encapsulation format
	    Key_Data[0] = 0xDD;
	    Key_Data[2] = 0x00;
	    Key_Data[3] = 0x0F;
            Key_Data[4] = 0xAC;
            Key_Data[5] = 0x01;

	    // GTK Key Data Encapsulation format
	    Key_Data[6] = (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].DefaultKeyId & 0x03);
	    Key_Data[7] = 0x00;

	    // handle the difference between TKIP and AES-CCMP
	    if (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].GroupKeyWepStatus == Ndis802_11Encryption3Enabled)
	    {
	       	Key_Data[1] = 0x16;// 4+2+16(OUI+GTK+GTKKEY)
	       	NdisMoveMemory(&Key_Data[8], GTK, LEN_AES_KEY);
	       	Packet.Body_Len[1] += 8+LEN_AES_KEY;
	    }
	    else
	    {
		Key_Data[1] = 0x26;// 4+2+32(OUI+GTK+GTKKEY)
		NdisMoveMemory(&Key_Data[8], GTK, TKIP_GTK_LENGTH);
        	Packet.Body_Len[1] += 8+TKIP_GTK_LENGTH;
	    }

			// Patch for compatibility between zero config and funk
			if (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].GroupKeyWepStatus == Ndis802_11Encryption3Enabled)
			{
				Key_Data[8+TKIP_GTK_LENGTH] = 0xDD;
				Key_Data[8+TKIP_GTK_LENGTH+1] = 0;
				Packet.Body_Len[1] += 2;
			}
			else
			{
				Key_Data[8+TKIP_GTK_LENGTH] = 0xDD;
				Key_Data[8+TKIP_GTK_LENGTH+1] = 0;
				Key_Data[8+TKIP_GTK_LENGTH+2] = 0;
				Key_Data[8+TKIP_GTK_LENGTH+3] = 0;
				Key_Data[8+TKIP_GTK_LENGTH+4] = 0;
				Key_Data[8+TKIP_GTK_LENGTH+5] = 0;
				Packet.Body_Len[1] += 6;
			}

            key_length = Packet.Body_Len[1];
       	    Packet.KeyDesc.DataLen[1] = key_length;
	    Packet.Body_Len[1] +=93 +2;
	    
	    NdisZeroMemory(mic,LEN_KEY_DESC_MIC );

            NdisMoveMemory(Packet.KeyDesc.RSC, pAdapter->PortCfg.MBSSID[apidx].SharedKey[pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId].TxTsc, 6);
	
            NdisMoveMemory(Packet.KeyDesc.RCounter, pEntry->R_Counter, LEN_KEY_DESC_REPLAY);
            NdisZeroMemory(Packet.KeyDesc.Nonce, LEN_KEY_DESC_NONCE);//WPA2 0;
            NdisZeroMemory(Packet.KeyDesc.IV,sizeof(LEN_KEY_DESC_IV));//WPA2 0; WPA1 random
	    // count MIC
	    if (pEntry->WepStatus == Ndis802_11Encryption3Enabled)
	    {
	        AES_GTK_KEY_WRAP(&pEntry->PTK[16], Key_Data, key_length, pRc4GTK);
	        // AES wrap function will grow 8 bytes in length
	        NdisMoveMemory(Packet.KeyDesc.Data, pRc4GTK, (key_length + 8));
	        Packet.Body_Len[1] += 8;
	        Packet.KeyDesc.DataLen[1] += 8;
	        MakeOutgoingFrame(OutBuffer,            &FrameLen,
	                        Packet.Body_Len[1] + 4, &Packet,
	                       END_OF_ARGS);

	        HMAC_SHA1(OutBuffer, FrameLen, pEntry->PTK, LEN_EAP_MICK, digest);
	        NdisMoveMemory(Packet.KeyDesc.MIC, digest, LEN_KEY_DESC_MIC);
 	    }
	    else
	    {
	        // PREPARE Encrypted  "Key DATA" field.  (Encrypt GTK with RC4, usinf PTK[16]->[31] as Key, IV-field as IV)
	        // put TxTsc in Key RSC field
	        pAdapter->PrivateInfo.FCSCRC32 = PPPINITFCS32;   //Init crc32.

	        // ekey is the contanetion of IV-field, and PTK[16]->PTK[31]
	        NdisMoveMemory(ekey, Packet.KeyDesc.IV, LEN_KEY_DESC_IV);
	        NdisMoveMemory(&ekey[LEN_KEY_DESC_IV], &pEntry->PTK[16], LEN_EAP_EK);
	        ARCFOUR_INIT(&pAdapter->PrivateInfo.WEPCONTEXT, ekey, (LEN_KEY_DESC_IV+LEN_EAP_EK));  //INIT SBOX, KEYLEN+3(IV)
	        pAdapter->PrivateInfo.FCSCRC32 = RTMP_CALC_FCS32(pAdapter->PrivateInfo.FCSCRC32, Key_Data, key_length);
	        WPAARCFOUR_ENCRYPT(&pAdapter->PrivateInfo.WEPCONTEXT, pRc4GTK,Key_Data, key_length);
	        NdisMoveMemory(Packet.KeyDesc.Data, pRc4GTK, key_length);

	        // make a frame for Countint MIC,
	        MakeOutgoingFrame(OutBuffer,            &FrameLen,
	                            Packet.Body_Len[1] + 4, &Packet,
	                            END_OF_ARGS);

	        hmac_md5(pEntry->PTK, LEN_EAP_MICK, OutBuffer, FrameLen, mic);
	        NdisMoveMemory(Packet.KeyDesc.MIC, mic, LEN_KEY_DESC_MIC);
	    }
            WPAMake8023Hdr(pAdapter, pEntry, Header802_3);
            MakeOutgoingFrame(OutBuffer,            &FrameLen,
                            LENGTH_802_3,            Header802_3,
                            Packet.Body_Len[1] + 4,  &Packet,
                            END_OF_ARGS);
            
            DBGPRINT(RT_DEBUG_TRACE, "RTMPToWirelessSta : ETHTYPE = %x %x FrameLen = %d! \n",Header802_3[12],Header802_3[13],FrameLen);
//iversonadd
            RTMPToWirelessSta(pAdapter, OutBuffer, FrameLen, TRUE);
	
	}  // WPA2 group key 
        else 
   	{// WPA1 group key
        WPAMake8023Hdr(pAdapter, pEntry, Header802_3);
        
        NdisMoveMemory(Packet.KeyDesc.RCounter, pEntry->R_Counter, LEN_KEY_DESC_REPLAY);
        NdisMoveMemory(Packet.KeyDesc.Nonce, pAdapter->PortCfg.MBSSID[apidx].GNonce, LEN_KEY_DESC_NONCE);
        NdisMoveMemory(Packet.KeyDesc.IV, &pAdapter->PortCfg.MBSSID[apidx].GNonce[16], sizeof(LEN_KEY_DESC_IV));
        
        // Suggest IV be random number plus some number,
        Packet.KeyDesc.IV[15] += 2;
        NdisMoveMemory(Packet.KeyDesc.RSC, pAdapter->PortCfg.MBSSID[apidx].SharedKey[pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId].TxTsc, 6);
        // Count EAPOL MIC , and encrypt DATA field before Send,   DATA fields includes the encrypted GTK
        if (pEntry->WepStatus == Ndis802_11Encryption3Enabled)
        {
//            UCHAR digest[80];  // 80 to prevent overflow
            
            if (pAdapter->PortCfg.MBSSID[apidx].GroupKeyWepStatus == Ndis802_11Encryption3Enabled)
            {
            	AES_GTK_KEY_WRAP(&pEntry->PTK[16], GTK, 16, pRc4GTK);
            	NdisMoveMemory(Packet.KeyDesc.Data, pRc4GTK, (LEN_AES_KEY +8));
            }
            else
            {
            	AES_GTK_KEY_WRAP(&pEntry->PTK[16], GTK, TKIP_GTK_LENGTH, pRc4GTK);
            	NdisMoveMemory(Packet.KeyDesc.Data, pRc4GTK, (TKIP_GTK_LENGTH +8));
            	Packet.KeyDesc.KeyLength[1] += 16;
		        Packet.KeyDesc.DataLen[1] += 16;
		        Packet.Body_Len[1] += 16;
            }
            
            // First make a frame  for Countint MIC,
     
            MakeOutgoingFrame(OutBuffer,            &FrameLen,
                            Packet.Body_Len[1] + 4, &Packet,
                            END_OF_ARGS);

            HMAC_SHA1(OutBuffer,  FrameLen, pEntry->PTK, LEN_EAP_MICK, digest);
            NdisMoveMemory(&Packet.KeyDesc.MIC, digest, LEN_KEY_DESC_MIC);
            
            // make  outgoing frame
            MakeOutgoingFrame(OutBuffer,            &FrameLen,
                            LENGTH_802_3,           &Header802_3,
                            Packet.Body_Len[1] + 4, &Packet,
                            END_OF_ARGS);
        }
        else
        {
            // PREPARE Encrypted  "Key DATA" field.  (Encrypt GTK with RC4, usinf PTK[16]->[31] as Key, IV-field as IV)
            // put TxTsc in Key RSC field
            pAdapter->PrivateInfo.FCSCRC32 = PPPINITFCS32;   //Init crc32.

            // ekey is the contanetion of IV-field, and PTK[16]->PTK[31]
            NdisMoveMemory(ekey, Packet.KeyDesc.IV, LEN_KEY_DESC_IV);
            NdisMoveMemory(&ekey[LEN_KEY_DESC_IV], &pEntry->PTK[16], LEN_EAP_EK);
            ARCFOUR_INIT(&pAdapter->PrivateInfo.WEPCONTEXT, ekey, (LEN_KEY_DESC_IV+LEN_EAP_EK));  //INIT SBOX, KEYLEN+3(IV)
            pAdapter->PrivateInfo.FCSCRC32 = RTMP_CALC_FCS32(pAdapter->PrivateInfo.FCSCRC32, GTK, TKIP_GTK_LENGTH);
            WPAARCFOUR_ENCRYPT(&pAdapter->PrivateInfo.WEPCONTEXT, pRc4GTK,GTK,  TKIP_GTK_LENGTH);
            NdisMoveMemory(Packet.KeyDesc.Data,  pRc4GTK, TKIP_GTK_LENGTH);

            // make a frame for Countint MIC,
            MakeOutgoingFrame(OutBuffer,            &FrameLen,
                            Packet.Body_Len[1] + 4, &Packet,
                            END_OF_ARGS);

            hmac_md5(pEntry->PTK, LEN_EAP_MICK, OutBuffer, FrameLen, mic);
            NdisMoveMemory(Packet.KeyDesc.MIC, mic, LEN_KEY_DESC_MIC);
            
            // make  outgoing frame
            MakeOutgoingFrame(OutBuffer,            &FrameLen,
                            LENGTH_802_3,           &Header802_3,
                            Packet.Body_Len[1] + 4, &Packet,
                            END_OF_ARGS);
        }
         
        DBGPRINT(RT_DEBUG_TRACE,"<== IF(ra%d) WPAHardTransmit - FrameLen = %d \n", apidx, FrameLen);
//iversonadd
        RTMPToWirelessSta(pAdapter, OutBuffer, FrameLen, TRUE);
	}//WPA1 group key
    }while (FALSE);

    kfree(mpool);

    return (NDIS_STATUS_SUCCESS);
}

VOID DisAssocAction(
    IN PRTMP_ADAPTER    pAd, 
    IN MAC_TABLE_ENTRY  *pEntry,
    IN USHORT           SubType,
    IN USHORT           Reason)
{
    CHAR            *OutBuffer = NULL;
    ULONG           FrameLen = 0;
    HEADER_802_11          DisassocHdr;
    UCHAR         addr[MAC_ADDR_LEN];

    if (pEntry)
    {
	    //  send out a DISASSOC request frame
	    OutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);
	    if(OutBuffer == NULL)
	        return;
	    
	    NdisMoveMemory(addr, pEntry->Addr, MAC_ADDR_LEN);
	    DBGPRINT(RT_DEBUG_ERROR, "Send DISASSOC Reason = %d frame  TO %x %x %x %x %x %x \n",Reason,pEntry->Addr[0],
	    pEntry->Addr[1],pEntry->Addr[2],pEntry->Addr[3],pEntry->Addr[4],pEntry->Addr[5]);
	    MgtMacHeaderInit(pAd, &DisassocHdr, SubType, 0, addr, pAd->PortCfg.MBSSID[pEntry->ApIdx].Bssid);
	    MakeOutgoingFrame(OutBuffer, &FrameLen, sizeof(HEADER_802_11), (CHAR *)&DisassocHdr, 2, (char*)&Reason, END_OF_ARGS);
	    MiniportMMRequest(pAd, OutBuffer, FrameLen);
	    kfree(OutBuffer);
    
        //ApLogEvent(pAd, pEntry->Addr, EVENT_DISASSOCIATED, apidx);
        MacTableDeleteEntry(pAd, pEntry->Addr);
    }
}

/*
    ==========================================================================
    Description:
        Timer execution function for periodically updating group key.
    Return:
    ==========================================================================
*/  
VOID GREKEYPeriodicExec(
    IN  unsigned long data) 
{
    UINT            i;
    ULONG           temp_counter = 0;
    RTMP_ADAPTER    *pAdapter = (RTMP_ADAPTER *)data;
    BOOLEAN         bTimer = TRUE;
    UCHAR           apidx;
        
    DBGPRINT(RT_DEBUG_INFO, "GROUP REKEY PeriodicExec ==>> \n");

    for (apidx = 0; apidx < pAdapter->PortCfg.BssidNum; apidx++)
    {
        if ((pAdapter->PortCfg.MBSSID[apidx].AuthMode != Ndis802_11AuthModeWPA) && (pAdapter->PortCfg.MBSSID[apidx].AuthMode != Ndis802_11AuthModeWPAPSK)
        	&& (pAdapter->PortCfg.MBSSID[apidx].AuthMode != Ndis802_11AuthModeWPA2) && (pAdapter->PortCfg.MBSSID[apidx].AuthMode != Ndis802_11AuthModeWPA2PSK)
        	&& (pAdapter->PortCfg.MBSSID[apidx].AuthMode != Ndis802_11AuthModeWPA1WPA2) && (pAdapter->PortCfg.MBSSID[apidx].AuthMode != Ndis802_11AuthModeWPAPSKWPA2PSK))
        {
            bTimer = FALSE;
            pAdapter->PortCfg.REKEYTimerRunning = FALSE;

            continue;
        }    
        else if (pAdapter->PortCfg.WPAREKEY.ReKeyInterval == 0)
        {
            bTimer = FALSE;
            pAdapter->PortCfg.REKEYTimerRunning = FALSE;

            continue;
        }
        
        if ((pAdapter->PortCfg.WPAREKEY.ReKeyMethod == TIME_REKEY) && (pAdapter->PortCfg.REKEYCOUNTER < 0xffffffff))
            temp_counter = (++pAdapter->PortCfg.REKEYCOUNTER);
        // REKEYCOUNTER is incremented every TX_RING_SIZE packets transmitted, 
        // But the unit of Rekeyinterval is 1K packets
        else if (pAdapter->PortCfg.WPAREKEY.ReKeyMethod == PKT_REKEY )
            temp_counter = pAdapter->PortCfg.REKEYCOUNTER/TX_RING_SIZE;
        else
        {
            bTimer = FALSE;
            pAdapter->PortCfg.REKEYTimerRunning = FALSE;

            continue;
        }
        
        if (temp_counter >= (pAdapter->PortCfg.WPAREKEY.ReKeyInterval))
        {
            pAdapter->PortCfg.REKEYCOUNTER = 0;
            DBGPRINT(RT_DEBUG_TRACE, "Rekey Interval Excess, GKeyDoneStations= %d\n", pAdapter->MacTab.Size);
            if (pAdapter->MacTab.Size > 0)
            {
                pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId++;
                if (pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId >= 4)
                    pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId = 1;
                GenRandom(pAdapter, pAdapter->PortCfg.MBSSID[apidx].GNonce, apidx);
                for (i = 0; i < MAX_LEN_OF_MAC_TABLE; i++)
                {
                    if ((pAdapter->MacTab.Content[i].Valid == TRUE) && (pAdapter->MacTab.Content[i].WpaState == AS_PTKINITDONE)
                    	&& (pAdapter->MacTab.Content[i].ApIdx == apidx))
                    {
                        pAdapter->MacTab.Content[i].GTKState = REKEY_NEGOTIATING;
                        NdisZeroMemory(pAdapter->PortCfg.MBSSID[apidx].SharedKey[pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId].TxTsc, sizeof(pAdapter->PortCfg.MBSSID[apidx].SharedKey[pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId].TxTsc));
                        WPAHardTransmit(pAdapter, &pAdapter->MacTab.Content[i]);
                        DBGPRINT(RT_DEBUG_TRACE, "Rekey interval excess, Update Group Key for %x %x %x %x %x %x , DefaultKeyId= %x \n",
                            pAdapter->MacTab.Content[i].Addr[0],pAdapter->MacTab.Content[i].Addr[1],
                            pAdapter->MacTab.Content[i].Addr[2],pAdapter->MacTab.Content[i].Addr[3],
                            pAdapter->MacTab.Content[i].Addr[4],pAdapter->MacTab.Content[i].Addr[5],
                            pAdapter->PortCfg.MBSSID[apidx].DefaultKeyId);
                    }
                }
            }
        }
    }
        
    if(bTimer)
    {
		RTMPAddTimer(&pAdapter->PortCfg.REKEYTimer, GUPDATE_EXEC_INTV);
        pAdapter->PortCfg.REKEYTimerRunning = TRUE;
    }
}

VOID WPAMake8023Hdr(
    IN PRTMP_ADAPTER    pAd, 
    IN MAC_TABLE_ENTRY  *pEntry, 
    IN OUT PCHAR        pHdr)
{    
     // Addr1: DA, Addr2: BSSID, Addr3: SA
    NdisMoveMemory(pHdr, pEntry->Addr, MAC_ADDR_LEN);
    NdisMoveMemory(&pHdr[MAC_ADDR_LEN], pAd->PortCfg.MBSSID[pEntry->ApIdx].Bssid, MAC_ADDR_LEN);
    NdisMoveMemory(&pHdr[2*MAC_ADDR_LEN], EAPOL, LENGTH_802_3_TYPE);
}

/*
    ==========================================================================
    Description:
        Length of output is in octets rather than bits.  Since length is always a multiple of 8, output array is organized
        so first N octets starting from 0 contains PRF output.
        INPUT : supported input are 16, 32, 48, 64
        OUTPUT : output array should be 80 octets to allow for sha1 overflow
    Return:
    ==========================================================================
*/
VOID PRF(
    IN UCHAR    *Key,
    IN INT      key_len,
    IN UCHAR    *Prefix,
    IN INT      Prefix_len,
    IN UCHAR    *Data,
    IN INT      Data_len,
    OUT UCHAR   *Output,
    IN INT      Output_len)
{
    INT     i;
    UCHAR   *input;
    INT     currentindex=0;
    INT     total_len;
	BOOLEAN	bfree = TRUE;
    
	input = kmalloc(1024, MEM_ALLOC_FLAG);  // allocate memory
    if (input == NULL) {
    	input = prf_input;
		bfree = FALSE;
    }
		
    
    NdisMoveMemory(input, Prefix, Prefix_len);
    input[Prefix_len] = 0;
    NdisMoveMemory(&input[Prefix_len+1], Data, Data_len);
    total_len = Prefix_len+1+Data_len;
    input[total_len] = 0;
    total_len++;
    for (i = 0; i < (Output_len+19)/20; i++)
    {
        HMAC_SHA1(input, total_len, Key, key_len, &Output[currentindex]);
        currentindex += 20;
        input[total_len-1]++;
    }
	
	if (bfree==TRUE) kfree(input);
}

/*
	========================================================================
	
	Routine Description:
		Count TPTK from PMK

	Arguments:
		
	Return Value:
		Output		Store the PTK

	Note:
		
	========================================================================
*/
VOID CountPTK(
    IN UCHAR    *PMK,
    IN UCHAR    *ANonce,
    IN UCHAR    *AA,
    IN UCHAR    *SNonce,
    IN UCHAR    *SA,
    OUT UCHAR   *output,
    IN UINT     len)
{   
    UCHAR   concatenation[76];
    UINT    CurrPos=0;
    UCHAR   temp[32];
    UCHAR   Prefix[] = {'P', 'a', 'i', 'r', 'w', 'i', 's', 'e', ' ', 'k', 'e', 'y', ' ', 
                        'e', 'x', 'p', 'a', 'n', 's', 'i', 'o', 'n'};

    NdisZeroMemory(temp, sizeof(temp));

    GetSmall(SA, AA, temp, 6);
    NdisMoveMemory(concatenation, temp, 6);
    CurrPos += 6;

    GetLarge(SA, AA, temp, 6);
    NdisMoveMemory(&concatenation[CurrPos], temp, 6);
    CurrPos += 6;

    GetSmall(ANonce, SNonce, temp, 32);
    NdisMoveMemory(&concatenation[CurrPos], temp, 32);
    CurrPos += 32;

    GetLarge(ANonce, SNonce, temp, 32);
    NdisMoveMemory(&concatenation[CurrPos], temp, 32);
    CurrPos += 32;
    
    PRF(PMK, PMK_LEN, Prefix, 22, concatenation, 76 , output, len);
}

VOID CountGTK(
    IN UCHAR    *GMK,
    IN UCHAR    *GNonce,
    IN UCHAR    *AA,
    OUT UCHAR   *output,
    IN UINT     len)
{
    UCHAR   concatenation[76];
    UINT    CurrPos=0;
    UCHAR   Prefix[] = {'G', 'r', 'o', 'u', 'p', ' ',  'k', 'e', 'y', ' ', 
                        'e', 'x', 'p', 'a', 'n', 's', 'i', 'o', 'n'};

    NdisMoveMemory(&concatenation[CurrPos], AA, 6);
    CurrPos += 6;

    NdisMoveMemory(&concatenation[CurrPos], GNonce , 32);
    CurrPos += 32;

    PRF(GMK, PMK_LEN, Prefix, 19, concatenation, 38 , output, len);

}

VOID GetSmall(
    IN  PVOID   pSrc1,
    IN  PVOID   pSrc2,
    OUT PUCHAR  pOut,
    IN  ULONG   Length)
{
    PUCHAR  pMem1;
    PUCHAR  pMem2;
    ULONG   Index = 0;
    pMem1 = (PUCHAR) pSrc1;
    pMem2 = (PUCHAR) pSrc2;

    for (Index = 0; Index < Length; Index++)
    {
        if (pMem1[Index] != pMem2[Index])
        {
            if (pMem1[Index] > pMem2[Index])        
                NdisMoveMemory(pOut, pSrc2, Length);
            else
                NdisMoveMemory(pOut, pSrc1, Length);             

            break;
        }
    }
}

VOID GetLarge(
    IN  PVOID   pSrc1,
    IN  PVOID   pSrc2,
    OUT PUCHAR  pOut,
    IN  ULONG   Length)
{
    PUCHAR  pMem1;
    PUCHAR  pMem2;
    ULONG   Index = 0;
    pMem1 = (PUCHAR) pSrc1;
    pMem2 = (PUCHAR) pSrc2;

    for (Index = 0; Index < Length; Index++)
    {
        if (pMem1[Index] != pMem2[Index])
        {
            if (pMem1[Index] > pMem2[Index])        
                NdisMoveMemory(pOut, pSrc1, Length);
            else
                NdisMoveMemory(pOut, pSrc2, Length);             

            break;
        }
    }
}

// 802.1i  Annex F.9, Random number generation 	
VOID GenRandom(
    IN PRTMP_ADAPTER    pAd, 
    OUT UCHAR           *random,
    IN	UCHAR			apidx)
{   
    INT             i, curr;
    UCHAR           local[80];
    UCHAR           result[80];
    UMAXLONG        CurrentTime;
    UCHAR           prefix[] = {'I', 'n', 'i', 't', ' ', 'C', 'o', 'u', 'n', 't', 'e', 'r'};

    NdisZeroMemory(result, 80);
    NdisZeroMemory(local, 80);
    NdisMoveMemory(local, pAd->PortCfg.MBSSID[apidx].Bssid, ETH_LENGTH_OF_ADDRESS);
    
    for (i = 0; i < 32; i++)
    {       
        curr =  ETH_LENGTH_OF_ADDRESS;
        CurrentTime = jiffies;
        NdisMoveMemory(local,  pAd->PortCfg.MBSSID[apidx].Bssid, ETH_LENGTH_OF_ADDRESS);
        curr += ETH_LENGTH_OF_ADDRESS;
        NdisMoveMemory(&local[curr],  &CurrentTime, sizeof(CurrentTime));
        curr += sizeof(CurrentTime);
        NdisMoveMemory(&local[curr],  result, 32);
        curr += 32;
        NdisMoveMemory(&local[curr],  &i,  2);      
        curr += 2;
        PRF(pAd->PortCfg.Key_Counter, 32, prefix,12, local, curr, result, 32); 
    }

    for (i = 32; i > 0; i--)
    {   
        if (pAd->PortCfg.Key_Counter[i-1] == 0xff)
        {
            pAd->PortCfg.Key_Counter[i-1] = 0;
        }
        else
        {
            pAd->PortCfg.Key_Counter[i-1]++;
            break;
        }
    }
    NdisMoveMemory(random, result,  32);
}

/*
    ==========================================================================
    Description:
        ENCRYPT AES GTK before sending in EAPOL frame.
        AES GTK length = 128 bit,  so fix blocks for aes-key-wrap as 2 in this function.
        This function references to RFC 3394 for aes key wrap algorithm.
    Return:
    ==========================================================================
*/  
VOID AES_GTK_KEY_WRAP( 
    IN UCHAR    *key,
    IN UCHAR    *plaintext,
    IN UCHAR	p_len,
    OUT UCHAR   *ciphertext)
{
    UCHAR		A[8], BIN[16], BOUT[16];
    UCHAR		R[512];
    INT			num_blocks = p_len/8;	// unit:64bits
    INT			i, j;
    aes_context	aesctx;
    UCHAR		xor;

    aes_set_key(&aesctx, key, 128);

	// Init IA
	for (i = 0; i < 8; i++)
        A[i] = 0xa6;

	//Input plaintext
	for (i = 0; i < num_blocks; i++)
	{
		for (j = 0 ; j < 8; j++)
			R[8 * (i + 1) + j] = plaintext[8 * i + j];
	}

	// Key Mix
	for (j = 0; j < 6; j++)
	{
		for(i = 1; i <= num_blocks; i++)
		{
			//phase 1
			memcpy(BIN, A, 8);
			memcpy(&BIN[8], &R[8 * i], 8);
			aes_encrypt(&aesctx, BIN, BOUT);

			memcpy(A, &BOUT[0], 8);
			xor = num_blocks * j + i;
			A[7] = BOUT[7] ^ xor;
			memcpy(&R[8 * i], &BOUT[8], 8);
		}
	}

	// Output ciphertext
	memcpy(ciphertext, A, 8);

	for (i = 1; i <= num_blocks; i++)
	{
		for (j = 0 ; j < 8; j++)
			ciphertext[8 * i + j] = R[8 * i + j];
	}
}

/*
    ========================================================================
    
    Routine Description:
        Misc function to decrypt AES body
    
    Arguments:
            
    Return Value:
    
    Note:
        This function references to RFC 3394 for aes key unwrap algorithm.  
    ========================================================================
*/
VOID    AES_GTK_KEY_UNWRAP( 
    IN  UCHAR   *key,
    OUT UCHAR   *plaintext,
    IN UCHAR    c_len,
    IN  UCHAR   *ciphertext)
{
    UCHAR       A[8],   BIN[16], BOUT[16];
    UCHAR       xor;
    INT         i, j;
    aes_context aesctx;
    UCHAR       R[512];
    INT         num_blocks = c_len/8;   // unit:64bits

    // Initialize
    NdisMoveMemory(A, ciphertext, 8);
    //Input plaintext
    for (i = 0; i < (c_len-8); i++)
    {
        R[ i] = ciphertext[i + 8];
    }

    aes_set_key(&aesctx, key, 128);

    for (j = 5; j >= 0; j--)
    {
        for(i = (num_blocks-1); i > 0; i--)
        {
            xor = (num_blocks -1 )* j + i;
            NdisMoveMemory(BIN, A, 8);
            BIN[7] = A[7] ^ xor;
            NdisMoveMemory(&BIN[8], &R[(i-1)*8], 8);
            aes_decrypt(&aesctx, BIN, BOUT);
            NdisMoveMemory(A, &BOUT[0], 8);
            NdisMoveMemory(&R[(i-1)*8], &BOUT[8], 8);
        }
    }

    // OUTPUT
    for (i = 0; i < c_len; i++)
    {
        plaintext[i] = R[i];
    }
}

/*
    ========================================================================
    Routine Description:
       Send all EAP frames to wireless station.
       These frames don't come from normal SendPackets routine, but are EAPPacket, EAPOL, 
        
    Arguments:
        pRxD        Pointer to the Rx descriptor
        
    Return Value:
None
========================================================================
*/
//iversonadd
VOID RTMPToWirelessSta(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  PUCHAR          pFrame,
    IN  UINT            FrameLen,
    IN  BOOLEAN         DoEncrypt)
{
    SST             Sst;
    USHORT          Aid;
    UCHAR           PsMode, Rate;
    MAC_TABLE_ENTRY *pEntry;
    struct sk_buff  *skb;
#ifdef APCLI_SUPPORT
	UCHAR			ApCliIdx = 0;
	RT_802_11_APCLI_ENTRY *pApCliEntry = NULL;
#endif

    // decide the return value. TRUE if no need to indicate to LLC, FALSE otherwise
    pEntry = SsPsInquiry(pAdapter, pFrame, &Sst, &Aid, &PsMode, &Rate);
    
#ifdef APCLI_SUPPORT
	if (!pEntry)
	{
		int i;
		for (i = 0; i < MAX_APCLI_ENTRY; i++)
		{
			if ((pAdapter->ApCliTab.ApCliEntry[i].Valid == TRUE) && (pAdapter->ApCliTab.ApCliEntry[i].Enable == TRUE)
					&& (MAC_ADDR_EQUAL(&pAdapter->ApCliTab.ApCliEntry[i].ApCliBssid, pFrame)))
			{
				ApCliIdx = i;
				pApCliEntry = &pAdapter->ApCliTab.ApCliEntry[i];
				break;	
			}
		}	
	}
#endif
    
    if (pEntry && (Sst == SST_ASSOC))
    {
        DBGPRINT(RT_DEBUG_TRACE, "WPA - RTMPToWirelessSta ====>> to IF(ra%d)\n", pEntry->ApIdx);

        do {
            // 1. build a packet and call RTMPSendPacket();
            //    be careful about how/when to release this internal allocated PACKET buffer
            if ((skb = __dev_alloc_skb(FrameLen + 2, MEM_ALLOC_FLAG)) != NULL)
            {
                skb->len = FrameLen;
                memcpy((skb->data), pFrame, FrameLen);
            }
            else
            {
                break;
            }

            // 1.1 apidx != 0, then we need set packet mbssid attribute.
			RTMP_SET_PACKET_NET_DEVICE_MBSSID(skb, MAIN_MBSSID);	// set a default value
            if(pEntry->ApIdx != 0)
                RTMP_SET_PACKET_NET_DEVICE_MBSSID(skb, pEntry->ApIdx);
            RTMP_SET_PACKET_SOURCE(skb, PKTSRC_DRIVER);
            RTMP_SET_PACKET_MOREDATA(skb, FALSE);

#ifdef RTL865X_FAST_PATH
			/* indicate this packet is from RT61 for buffer free usage */
		    RTL865X_SET_PKTTYPE(skb, RTL865X_TYPE_RT61);
#endif
//iversonadd
if(!DoEncrypt)
        RTMP_SET_PACKET_ENCRYPT(skb,3);

            // 2. send out the packet
            RTMPSendPacket(pAdapter, skb);
        } while (FALSE);
    }
#ifdef APCLI_SUPPORT    
    else if (pApCliEntry)
    {
    	DBGPRINT(RT_DEBUG_TRACE, "WPA - RTMPToWirelessSta ====>> to IF(apcli%d - BSSID %02X:%02X:%02X:%02X:%02X:%02X)\n", ApCliIdx,
    							pApCliEntry->ApCliBssid[0], pApCliEntry->ApCliBssid[1], pApCliEntry->ApCliBssid[2],
    							pApCliEntry->ApCliBssid[3], pApCliEntry->ApCliBssid[4], pApCliEntry->ApCliBssid[5]);
    	
    	do {
            // 1. build a packet and call RTMPSendPacket();
            //    be careful about how/when to release this internal allocated PACKET buffer
            if ((skb = __dev_alloc_skb(FrameLen + 2, MEM_ALLOC_FLAG)) != NULL)
            {
                skb->len = FrameLen;
                memcpy((skb->data), pFrame, FrameLen);
            }
            else
            {
                break;
            }

            RTMP_SET_PACKET_NET_DEVICE_APCLI(skb, ApCliIdx);
            RTMP_SET_PACKET_SOURCE(skb, PKTSRC_DRIVER);
            RTMP_SET_PACKET_MOREDATA(skb, FALSE);

#ifdef RTL865X_FAST_PATH
			/* indicate this packet is from RT61 for buffer free usage */
		    RTL865X_SET_PKTTYPE(skb, RTL865X_TYPE_RT61);
#endif


            // 2. send out the packet
            RTMPSendPacket(pAdapter, skb);
        } while (FALSE);
    }	
#endif   
    
    RTMPDeQueuePacket(pAdapter, FALSE, FALSE);
}

/*
    ========================================================================

    Routine Description:
        Sending EAP Req. frame to station in authenticating state.
        These frames come from Authenticator deamon.

    Arguments:
        pAdapter        Pointer to our adapter
        pPacket     Pointer to outgoing EAP frame body + 8023 Header
        Len             length of pPacket
        
    Return Value:
        None
    ========================================================================
*/
VOID WpaSend(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  PUCHAR          pPacket,
    IN  ULONG           Len)
{
    PEAP_HDR        pEapHdr;
    UCHAR         Addr[MAC_ADDR_LEN];
    MAC_TABLE_ENTRY *pEntry;
    
    DBGPRINT(RT_DEBUG_INFO, "WpaSend ==> Len=%d\n",Len);

    NdisMoveMemory(Addr, pPacket, 6);
    pEapHdr = (EAP_HDR*)(pPacket + LENGTH_802_3);
    if ((pEntry = MacTableLookup(pAdapter, Addr)) == NULL)
        return;
//iversonadd
    if (((pEntry->AuthMode >= Ndis802_11AuthModeWPA) && (pEapHdr->ProType != EAPOLKey)) ||
        (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].IEEE8021X == TRUE))
        RTMPToWirelessSta(pAdapter, pPacket, Len, FALSE);

    if (RTMPEqualMemory((pPacket+12), EAPOL, 2) )
    {
        switch (pEapHdr->code)
        {
            case EAP_CODE_SUCCESS:
                if ((pEntry->AuthMode >= Ndis802_11AuthModeWPA) && (pEapHdr->ProType != EAPOLKey))
                {
                    DBGPRINT(RT_DEBUG_TRACE,"Send EAP_CODE_SUCCESS\n\n");
                    if (pEntry->Sst == SST_ASSOC)
                    {
                        pEntry->WpaState = AS_INITPMK;
						// Only set the expire and counters
	                    pEntry->RetryTimer.expires = jiffies + WPA_RETRY_EXEC_INTV;
	                    pEntry->ReTryCounter = PEER_MSG1_RETRY_TIMER_CTR;
	                    WPAStart4WayHS(pAdapter, pEntry);
                    }
                }
                else
                {
                    pEntry->PrivacyFilter = Ndis802_11PrivFilterAcceptAll;
                    pEntry->WpaState = AS_PTKINITDONE;
                    pAdapter->PortCfg.MBSSID[pEntry->ApIdx].PortSecured = WPA_802_1X_PORT_SECURED;
                    pEntry->PortSecured = WPA_802_1X_PORT_SECURED;

#ifdef WSC_SUPPORT
                    if (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].WscControl.WscConfMode != WSC_DISABLE)
                        WscInformFromWPA(pEntry);
#endif // WSC_SUPPORT //
                    
                    DBGPRINT(RT_DEBUG_TRACE,"IEEE8021X-WEP : Send EAP_CODE_SUCCESS\n\n");
                }
                break;

            case EAP_CODE_FAILURE:
                break;

            default:
                break;    
        }
    }
    else     
    {
        DBGPRINT(RT_DEBUG_TRACE, "Send Deauth, Reason : REASON_NO_LONGER_VALID\n");
        DisAssocAction(pAdapter, pEntry, SUBTYPE_DEAUTH, REASON_NO_LONGER_VALID);
    }
}

VOID RTMPMakeRSNIE(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  UINT            AuthMode,
    IN  UINT            WepStatus,
    IN  UCHAR           apidx)
{
	RSN_CAPABILITIES	*pRSN_Cap;
	
	if (WepStatus == Ndis802_11Encryption4Enabled)
		pAdapter->PortCfg.MBSSID[apidx].GroupKeyWepStatus = Ndis802_11Encryption2Enabled;
	else
		pAdapter->PortCfg.MBSSID[apidx].GroupKeyWepStatus = WepStatus;

    if ((AuthMode != Ndis802_11AuthModeWPA) && (AuthMode != Ndis802_11AuthModeWPAPSK)
    	&& (AuthMode != Ndis802_11AuthModeWPA2) && (AuthMode != Ndis802_11AuthModeWPA2PSK)
		&& (AuthMode != Ndis802_11AuthModeWPA1WPA2) && (AuthMode != Ndis802_11AuthModeWPAPSKWPA2PSK))
		return;

	pAdapter->PortCfg.MBSSID[apidx].RSNIE_Len[0] = 0;
	pAdapter->PortCfg.MBSSID[apidx].RSNIE_Len[1] = 0;
    NdisZeroMemory(pAdapter->PortCfg.MBSSID[apidx].RSN_IE[0], MAX_LEN_OF_RSNIE);
    NdisZeroMemory(pAdapter->PortCfg.MBSSID[apidx].RSN_IE[1], MAX_LEN_OF_RSNIE);

	// For WPA1, RSN_IE=221
	if ((AuthMode == Ndis802_11AuthModeWPA) || (AuthMode == Ndis802_11AuthModeWPAPSK)
		|| (AuthMode == Ndis802_11AuthModeWPA1WPA2) || (AuthMode == Ndis802_11AuthModeWPAPSKWPA2PSK))
	{
		RSNIE				*pRsnie;
		RSNIE_AUTH			*pRsnie_auth;
		UCHAR				Rsnie_size = 0;

		pRsnie = (RSNIE*)pAdapter->PortCfg.MBSSID[apidx].RSN_IE[0];
	    NdisMoveMemory(pRsnie->oui, OUI_WPA_WEP40, 4);
	    pRsnie->version = 1;

	    switch (WepStatus)
	    {
	        case Ndis802_11Encryption2Enabled:
	            NdisMoveMemory(pRsnie->mcast, OUI_WPA_TKIP, 4);
	            pRsnie->ucount = 1;
	            NdisMoveMemory(pRsnie->ucast[0].oui, OUI_WPA_TKIP, 4);
	            Rsnie_size = sizeof(RSNIE);
	            break;

	        case Ndis802_11Encryption3Enabled:
	            NdisMoveMemory(pRsnie->mcast, OUI_WPA_CCMP, 4);
	            pRsnie->ucount = 1;
	            NdisMoveMemory(pRsnie->ucast[0].oui, OUI_WPA_CCMP, 4);
	            Rsnie_size = sizeof(RSNIE);
	            break;
	            
	        case Ndis802_11Encryption4Enabled:
	            NdisMoveMemory(pRsnie->mcast, OUI_WPA_TKIP, 4);
	            pRsnie->ucount = 2;
	            NdisMoveMemory(pRsnie->ucast[0].oui, OUI_WPA_TKIP, 4);
	            NdisMoveMemory(pRsnie->ucast[0].oui + 4, OUI_WPA_CCMP, 4);
	            Rsnie_size = sizeof(RSNIE) + 4;
	            break;
	    }

	    pRsnie_auth = (RSNIE_AUTH*)((PUCHAR)pRsnie + Rsnie_size);

	    switch (AuthMode)
	    {
	        case Ndis802_11AuthModeWPA:
	        case Ndis802_11AuthModeWPA1WPA2:
	            pRsnie_auth->acount = 1;
	            NdisMoveMemory(pRsnie_auth->auth[0].oui, OUI_WPA_WEP40, 4);
	            break;

	        case Ndis802_11AuthModeWPAPSK:
	        case Ndis802_11AuthModeWPAPSKWPA2PSK:
	            pRsnie_auth->acount = 1;
	            NdisMoveMemory(pRsnie_auth->auth[0].oui, OUI_WPA_TKIP, 4);
	            break;
	    }

//	    pRSN_Cap = (RSN_CAPABILITIES*)((PUCHAR)pRsnie_auth + sizeof(RSNIE_AUTH));	// option in WPA1
	    
#ifdef BIG_ENDIAN
	    pRsnie->version = SWAP16(pRsnie->version);
	    pRsnie->ucount = SWAP16(pRsnie->ucount);
	    pRsnie_auth->acount = SWAP16(pRsnie_auth->acount);
#endif
	    
	    pAdapter->PortCfg.MBSSID[apidx].RSNIE_Len[0] = Rsnie_size + sizeof(RSNIE_AUTH)/* + sizeof(RSN_CAPABILITIES)*/;
	}

	// For WPA2, RSN_IE=48, if WPA1WPA2/WPAPSKWPA2PSK mix mode, we store RSN_IE in RSN_IE[1] else RSNIE[0]
	if ((AuthMode == Ndis802_11AuthModeWPA2) || (AuthMode == Ndis802_11AuthModeWPA2PSK)
		|| (AuthMode == Ndis802_11AuthModeWPA1WPA2) || (AuthMode == Ndis802_11AuthModeWPAPSKWPA2PSK))
	{
		RSNIE2				*pRsnie2;
		RSNIE_AUTH			*pRsnie_auth2;
		UCHAR				Rsnie_size = 0;

		if ((AuthMode == Ndis802_11AuthModeWPA1WPA2) || (AuthMode == Ndis802_11AuthModeWPAPSKWPA2PSK))
			pRsnie2 = (RSNIE2*)pAdapter->PortCfg.MBSSID[apidx].RSN_IE[1];
		else
			pRsnie2 = (RSNIE2*)pAdapter->PortCfg.MBSSID[apidx].RSN_IE[0];

	    pRsnie2->version = 1;

	    switch (WepStatus)
	    {
	        case Ndis802_11Encryption2Enabled:
	            NdisMoveMemory(pRsnie2->mcast, OUI_WPA2_TKIP, 4);
	            pRsnie2->ucount = 1;
	            NdisMoveMemory(pRsnie2->ucast[0].oui, OUI_WPA2_TKIP, 4);
	            Rsnie_size = sizeof(RSNIE2);
	            break;

	        case Ndis802_11Encryption3Enabled:
	            NdisMoveMemory(pRsnie2->mcast, OUI_WPA2_CCMP, 4);
	            pRsnie2->ucount = 1;
	            NdisMoveMemory(pRsnie2->ucast[0].oui, OUI_WPA2_CCMP, 4);
	            Rsnie_size = sizeof(RSNIE2);
	            break;

	        case Ndis802_11Encryption4Enabled:
	            NdisMoveMemory(pRsnie2->mcast, OUI_WPA2_TKIP, 4);
	            pRsnie2->ucount = 2;
	            NdisMoveMemory(pRsnie2->ucast[0].oui, OUI_WPA2_TKIP, 4);
	            NdisMoveMemory(pRsnie2->ucast[0].oui + 4, OUI_WPA2_CCMP, 4);
	            Rsnie_size = sizeof(RSNIE2) + 4;
	            break;
	    }

	    pRsnie_auth2 = (RSNIE_AUTH*)((PUCHAR)pRsnie2 + Rsnie_size);

	    switch (AuthMode)
	    {
	        case Ndis802_11AuthModeWPA2:
	        case Ndis802_11AuthModeWPA1WPA2:
	            pRsnie_auth2->acount = 1;
	            NdisMoveMemory(pRsnie_auth2->auth[0].oui, OUI_WPA2_WEP40, 4);
	            break;

	        case Ndis802_11AuthModeWPA2PSK:
	        case Ndis802_11AuthModeWPAPSKWPA2PSK:
	            pRsnie_auth2->acount = 1;
	            NdisMoveMemory(pRsnie_auth2->auth[0].oui, OUI_WPA2_TKIP, 4);
	            break;
	    }

	    pRSN_Cap = (RSN_CAPABILITIES*)((PUCHAR)pRsnie_auth2 + sizeof(RSNIE_AUTH));
	    pRSN_Cap->field.PreAuth = (pAdapter->PortCfg.MBSSID[apidx].PreAuth == TRUE) ? 1 : 0;

#ifdef BIG_ENDIAN
	    pRsnie2->version = SWAP16(pRsnie2->version);
	    pRsnie2->ucount = SWAP16(pRsnie2->ucount);
	    pRsnie_auth2->acount = SWAP16(pRsnie_auth2->acount);
#endif

		if ((AuthMode == Ndis802_11AuthModeWPA1WPA2) || (AuthMode == Ndis802_11AuthModeWPAPSKWPA2PSK))
			pAdapter->PortCfg.MBSSID[apidx].RSNIE_Len[1] = Rsnie_size + sizeof(RSNIE_AUTH) + sizeof(RSN_CAPABILITIES);
		else
			pAdapter->PortCfg.MBSSID[apidx].RSNIE_Len[0] = Rsnie_size + sizeof(RSNIE_AUTH) + sizeof(RSN_CAPABILITIES);
	}

    DBGPRINT(RT_DEBUG_TRACE,"RTMPMakeRSNIE IF(ra%d): RSNIE_Len[0]=%d, RSNIE_Len[1]=%d\n", apidx,
    	pAdapter->PortCfg.MBSSID[apidx].RSNIE_Len[0], pAdapter->PortCfg.MBSSID[apidx].RSNIE_Len[1]);
}


#ifdef APCLI_SUPPORT
/*KH:Add the following function to isolate Ap client security from main BSSID to make its specific RSNIE Frame*/
VOID RTMPApCliMakeRSNIE(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  UINT            AuthMode,
    IN  UINT            WepStatus,
    IN  UCHAR           apidx)
{
	RSN_CAPABILITIES	*pRSN_Cap;
	
	if (WepStatus == Ndis802_11Encryption4Enabled)
		pAdapter->ApCliTab.ApCliEntry[0].GroupCipher = Ndis802_11Encryption2Enabled;
	else
		pAdapter->ApCliTab.ApCliEntry[0].GroupCipher= WepStatus;
	

    if ( 
		(AuthMode != Ndis802_11AuthModeWPAPSK)
    		&&  (AuthMode != Ndis802_11AuthModeWPA2PSK)
	   )
		return;
	
    DBGPRINT(RT_DEBUG_TRACE,"In RTMPApCliMakeRSNIE\n");
	pAdapter->ApCliTab.ApCliEntry[0].RSN_IELen = 0;
	
    NdisZeroMemory(pAdapter->ApCliTab.ApCliEntry[0].RSN_IE, MAX_LEN_OF_RSNIE);
    

	// For WPA1, RSN_IE=221
	if (AuthMode == Ndis802_11AuthModeWPAPSK)
	{
		RSNIE				*pRsnie;
		RSNIE_AUTH			*pRsnie_auth;
		UCHAR				Rsnie_size = 0;

	    pRsnie = (RSNIE*)pAdapter->ApCliTab.ApCliEntry[0].RSN_IE;
	    NdisMoveMemory(pRsnie->oui, OUI_WPA_WEP40, 4);
	    pRsnie->version = 1;

	    switch (WepStatus)
	    {
	        case Ndis802_11Encryption2Enabled:
  		      printk("KH: APCLI:Ndis802_11AuthModeWPAPSK,Ndis802_11Encryption2Enabled\n");
	            NdisMoveMemory(pRsnie->mcast, OUI_WPA_TKIP, 4);
	            pRsnie->ucount = 1;
	            NdisMoveMemory(pRsnie->ucast[0].oui, OUI_WPA_TKIP, 4);
	            Rsnie_size = sizeof(RSNIE);
	            break;

	        case Ndis802_11Encryption3Enabled:
			printk("KH: APCLI:Ndis802_11AuthModeWPAPSK,Ndis802_11Encryption3Enabled\n");
	            NdisMoveMemory(pRsnie->mcast, OUI_WPA_CCMP, 4);
	            pRsnie->ucount = 1;
	            NdisMoveMemory(pRsnie->ucast[0].oui, OUI_WPA_CCMP, 4);
	            Rsnie_size = sizeof(RSNIE);
	            break;
	            
	        case Ndis802_11Encryption4Enabled:
			printk("KH: APCLI:Ndis802_11AuthModeWPAPSK,Ndis802_11Encryption4Enabled\n");
	            NdisMoveMemory(pRsnie->mcast, OUI_WPA_TKIP, 4);
	            pRsnie->ucount = 2;
	            NdisMoveMemory(pRsnie->ucast[0].oui, OUI_WPA_TKIP, 4);
	            NdisMoveMemory(pRsnie->ucast[0].oui + 4, OUI_WPA_CCMP, 4);
	            Rsnie_size = sizeof(RSNIE) + 4;
	            break;
	    }

	    pRsnie_auth = (RSNIE_AUTH*)((PUCHAR)pRsnie + Rsnie_size);
	    pRsnie_auth->acount = 1;
	    NdisMoveMemory(pRsnie_auth->auth[0].oui, OUI_WPA_TKIP, 4);
	       

//	    pRSN_Cap = (RSN_CAPABILITIES*)((PUCHAR)pRsnie_auth + sizeof(RSNIE_AUTH));	
// option in WPA1
	    
#ifdef BIG_ENDIAN
	    pRsnie->version = SWAP16(pRsnie->version);
	    pRsnie->ucount = SWAP16(pRsnie->ucount);
	    pRsnie_auth->acount = SWAP16(pRsnie_auth->acount);
#endif
	    
	   pAdapter->ApCliTab.ApCliEntry[0].RSN_IELen = Rsnie_size + sizeof(RSNIE_AUTH)/* + sizeof(RSN_CAPABILITIES)*/;
	}

	// For WPA2, RSN_IE=48, if WPA1WPA2/WPAPSKWPA2PSK mix mode, we store RSN_IE in RSN_IE[1] else RSNIE[0]
	if ((AuthMode == Ndis802_11AuthModeWPA2PSK))
	{
		RSNIE2				*pRsnie2;
		RSNIE_AUTH			*pRsnie_auth2;
		UCHAR				Rsnie_size = 0;

	    pRsnie2 = (RSNIE2*)pAdapter->ApCliTab.ApCliEntry[0].RSN_IE;

	    pRsnie2->version = 1;

	    switch (WepStatus)
	    {
	        case Ndis802_11Encryption2Enabled:
	            NdisMoveMemory(pRsnie2->mcast, OUI_WPA2_TKIP, 4);
	            pRsnie2->ucount = 1;
	            NdisMoveMemory(pRsnie2->ucast[0].oui, OUI_WPA2_TKIP, 4);
	            Rsnie_size = sizeof(RSNIE2);
	            break;

	        case Ndis802_11Encryption3Enabled:
	            NdisMoveMemory(pRsnie2->mcast, OUI_WPA2_CCMP, 4);
	            pRsnie2->ucount = 1;
	            NdisMoveMemory(pRsnie2->ucast[0].oui, OUI_WPA2_CCMP, 4);
	            Rsnie_size = sizeof(RSNIE2);
	            break;

	        case Ndis802_11Encryption4Enabled:
	            NdisMoveMemory(pRsnie2->mcast, OUI_WPA2_TKIP, 4);
	            pRsnie2->ucount = 2;
	            NdisMoveMemory(pRsnie2->ucast[0].oui, OUI_WPA2_TKIP, 4);
	            NdisMoveMemory(pRsnie2->ucast[0].oui + 4, OUI_WPA2_CCMP, 4);
	            Rsnie_size = sizeof(RSNIE2) + 4;
	            break;
	    }
	    pRsnie_auth2 = (RSNIE_AUTH*)((PUCHAR)pRsnie2 + Rsnie_size);

	    pRsnie_auth2->acount = 1;
	    NdisMoveMemory(pRsnie_auth2->auth[0].oui, OUI_WPA2_TKIP, 4);

	    pRSN_Cap = (RSN_CAPABILITIES*)((PUCHAR)pRsnie_auth2 + sizeof(RSNIE_AUTH));
	    pRSN_Cap->field.PreAuth = 0;
#ifdef BIG_ENDIAN
	    pRsnie2->version = SWAP16(pRsnie2->version);
	    pRsnie2->ucount = SWAP16(pRsnie2->ucount);
	    pRsnie_auth2->acount = SWAP16(pRsnie_auth2->acount);
#endif
	    pAdapter->ApCliTab.ApCliEntry[0].RSN_IELen = Rsnie_size + sizeof(RSNIE_AUTH) + sizeof(RSN_CAPABILITIES);
		
	}

    DBGPRINT(RT_DEBUG_TRACE,"RTMPMakeRSNIE IF(ra%d): RSN_IELen=%d", apidx,
					pAdapter->ApCliTab.ApCliEntry[0].RSN_IELen);
}
#endif

VOID RTMPAddPMKIDCache(
	IN  PRTMP_ADAPTER   		pAd,
	IN	INT						apidx,
	IN	PUCHAR				pAddr,
	IN	UCHAR					*PMKID,
	IN	UCHAR					*PMK)
{
	INT	i, chcheidx;

	if ((chcheidx = RTMPSearchPMKIDCache(pAd, apidx, pAddr)) != -1)
	{
		pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[chcheidx].RefreshTime = jiffies;
		NdisMoveMemory(&pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[chcheidx].PMKID, PMKID, LEN_PMKID);
		NdisMoveMemory(&pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[chcheidx].PMK, PMK, PMK_LEN);
		DBGPRINT(RT_DEBUG_TRACE,"RTMPAddPMKIDCache update %02x:%02x:%02x:%02x:%02x:%02x cache(%d) from IF(ra%d)\n", 
           	pAddr[0], pAddr[1], pAddr[2], pAddr[3], pAddr[4], pAddr[5], chcheidx, apidx);
		
		return;
	}
	
	for (i = 0; i < MAX_PMKID_COUNT; i++)
	{
		if (!pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[i].Valid)
		{
			pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[i].Valid = TRUE;
			pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[i].RefreshTime = jiffies;
			COPY_MAC_ADDR(&pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[i].MAC, pAddr);
			NdisMoveMemory(&pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[i].PMKID, PMKID, LEN_PMKID);
			NdisMoveMemory(&pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[i].PMK, PMK, PMK_LEN);
			DBGPRINT(RT_DEBUG_TRACE,"RTMPAddPMKIDCache add %02x:%02x:%02x:%02x:%02x:%02x cache(%d) from IF(ra%d)\n", 
            	pAddr[0], pAddr[1], pAddr[2], pAddr[3], pAddr[4], pAddr[5], i, apidx);
			break;
		}
	}

	if (i == MAX_PMKID_COUNT)
	{
		ULONG	timestamp = 0, idx = 0;

		DBGPRINT(RT_DEBUG_TRACE, "RTMPAddPMKIDCache(IF(%d) Cache full\n", apidx);
		for (i = 0; i < MAX_PMKID_COUNT; i++)
		{
			if (pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[i].Valid)
			{
				if (((timestamp == 0) && (idx == 0)) || ((timestamp != 0) && timestamp < pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[i].RefreshTime))
				{
					timestamp = pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[i].RefreshTime;
					idx = i;
				}
			}
		}
		pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[idx].Valid = TRUE;
		pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[idx].RefreshTime = jiffies;
		COPY_MAC_ADDR(&pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[idx].MAC, pAddr);
		NdisMoveMemory(&pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[idx].PMKID, PMKID, LEN_PMKID);
		NdisMoveMemory(&pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[idx].PMK, PMK, PMK_LEN);
		DBGPRINT(RT_DEBUG_TRACE,"RTMPAddPMKIDCache add %02x:%02x:%02x:%02x:%02x:%02x cache(%d) from IF(ra%d)\n", 
           	pAddr[0], pAddr[1], pAddr[2], pAddr[3], pAddr[4], pAddr[5], idx, apidx);
	}
}

INT RTMPSearchPMKIDCache(
	IN  PRTMP_ADAPTER   pAd,
	IN	INT				apidx,
	IN	PUCHAR		pAddr)
{
	INT	i = 0;
	
	for (i = 0; i < MAX_PMKID_COUNT; i++)
	{
		if ((pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[i].Valid)
			&& MAC_ADDR_EQUAL(&pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[i].MAC, pAddr))
		{
			DBGPRINT(RT_DEBUG_TRACE,"RTMPSearchPMKIDCache %02x:%02x:%02x:%02x:%02x:%02x cache(%d) from IF(ra%d)\n", 
            	pAddr[0], pAddr[1], pAddr[2], pAddr[3], pAddr[4], pAddr[5], i, apidx);
			break;
		}
	}

	if (i == MAX_PMKID_COUNT)
	{
		DBGPRINT(RT_DEBUG_TRACE, "RTMPSearchPMKIDCache - IF(%d) not found\n", apidx);
		return -1;
	}

	return i;
}

VOID RTMPDeletePMKIDCache(
	IN  PRTMP_ADAPTER   pAd,
	IN	INT				apidx,
	IN  INT				idx)
{
	if (pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[idx].Valid)
	{
		pAd->PortCfg.MBSSID[apidx].PMKIDCache.BSSIDInfo[idx].Valid = FALSE;
		DBGPRINT(RT_DEBUG_TRACE, "RTMPDeletePMKIDCache(IF(%d), del PMKID CacheIdx=%d\n", apidx, idx);
	}
}

VOID RTMPMaintainPMKIDCache(
	IN  PRTMP_ADAPTER   pAd)
{
	INT	i, j;
	
	for (i = 0; i < MAX_MBSSID_NUM; i++)
	{
		for (j = 0; j < MAX_PMKID_COUNT; j++)
		{
			if ((pAd->PortCfg.MBSSID[i].PMKIDCache.BSSIDInfo[j].Valid)
				&& ((jiffies - pAd->PortCfg.MBSSID[i].PMKIDCache.BSSIDInfo[j].RefreshTime) >= pAd->PortCfg.PMKCachePeriod))
			{
				RTMPDeletePMKIDCache(pAd, i, j);
			}
		}
	}
}

#ifdef APCLI_SUPPORT
/*	
	==========================================================================
	Description: 
		WPAPSK state machine init for ApCli link
	Parameters:	
		Sm -	pointer	to the WPAPSK state machine
	==========================================================================
 */
VOID StaWpaStateMachineInit(
	IN	PRTMP_ADAPTER	pAd, 
	IN	STA_STATE_MACHINE *Sm, 
	OUT	STA_STATE_MACHINE_FUNC Trans[])	
{
	StaStateMachineInit(Sm, (STA_STATE_MACHINE_FUNC*)Trans, STA_MAX_WPA_PSK_STATE, STA_MAX_WPA_PSK_MSG, (STA_STATE_MACHINE_FUNC)StaDrop, STA_WPA_PSK_IDLE, STA_WPA_MACHINE_BASE);
	StaStateMachineSetAction(Sm, STA_WPA_PSK_IDLE, STA_MT2_EAPOLKey, (STA_STATE_MACHINE_FUNC)StaWpaEAPOLKeyAction);
}

/*
    ==========================================================================
    Description:
        This is state machine function. 
        When receiving EAPOL packets which is for 802.1x key management. 
        Use in WPAPSK case. 
        In this function, further dispatch to different functions according to the received packet.  3 categories are : 
          1.  normal 4-way pairwisekey and 2-way groupkey handshake
          2.  MIC error (Countermeasures attack)  report packet from STA.
          3.  Request for pairwise/group key update from STA
    Return:
    ==========================================================================
*/
VOID StaWpaEAPOLKeyAction(
    IN PRTMP_ADAPTER    pAdapter, 
    IN MLME_QUEUE_ELEM  *Elem) 
{
	INT					MsgType = EAPOL_MSG_INVALID;
    PKEY_DESCRIPTER		pKeyDesc;
    PHEADER_802_11  	pHeader; 
	UCHAR				ZeroReplay[LEN_KEY_DESC_REPLAY];
	PEAPOL_PACKET       pMsg;
	INT					ApCliIdx = 0;	
	
	
	DBGPRINT(RT_DEBUG_TRACE, "-----> WpaEAPOLKeyAction\n");

	// Get 802.11 header first
    pHeader = (PHEADER_802_11) Elem->Msg;
	
	pMsg = (PEAPOL_PACKET) &Elem->Msg[LENGTH_802_11 + LENGTH_802_1_H];
	pKeyDesc = (PKEY_DESCRIPTER) &Elem->Msg[(LENGTH_802_11 + LENGTH_802_1_H + LENGTH_EAPOL_H)];
#ifdef BIG_ENDIAN
    // pMsg->KeyDesc.Keyinfo and pKeyDesc->Keyinfo both point to the same addr.
    // Thus, it only needs swap once.
{
		USHORT	tmpKeyinfo;

		NdisMoveMemory(&tmpKeyinfo, &pKeyDesc->Keyinfo, sizeof(USHORT));
		tmpKeyinfo = SWAP16(tmpKeyinfo);
		NdisMoveMemory(&pKeyDesc->Keyinfo, &tmpKeyinfo, sizeof(USHORT));
}
	//	*(USHORT *)((UCHAR *)pKeyDesc+1) = SWAP16(*(USHORT *)((UCHAR *)pKeyDesc+1));
#endif

	// Sanity check, this should only happen in WPA(2)-PSK mode
	// 0. Debug print all bit information
	DBGPRINT(RT_DEBUG_INFO, "KeyInfo Key Description Version %d\n", pKeyDesc->Keyinfo.KeyDescVer);
	DBGPRINT(RT_DEBUG_INFO, "KeyInfo Key Type %d\n", pKeyDesc->Keyinfo.KeyType);
	DBGPRINT(RT_DEBUG_INFO, "KeyInfo Key Index %d\n", pKeyDesc->Keyinfo.KeyIndex);
	DBGPRINT(RT_DEBUG_INFO, "KeyInfo Install %d\n", pKeyDesc->Keyinfo.Install);
	DBGPRINT(RT_DEBUG_INFO, "KeyInfo Key Ack %d\n", pKeyDesc->Keyinfo.KeyAck);
	DBGPRINT(RT_DEBUG_INFO, "KeyInfo Key MIC %d\n", pKeyDesc->Keyinfo.KeyMic);
	DBGPRINT(RT_DEBUG_INFO, "KeyInfo Secure %d\n", pKeyDesc->Keyinfo.Secure);
	DBGPRINT(RT_DEBUG_INFO, "KeyInfo Error %d\n", pKeyDesc->Keyinfo.Error);
	DBGPRINT(RT_DEBUG_INFO, "KeyInfo Request %d\n", pKeyDesc->Keyinfo.Request);
	DBGPRINT(RT_DEBUG_INFO, "KeyInfo EKD_DL %d\n", pKeyDesc->Keyinfo.EKD);
	
	// 1. Check EAPOL frame version and type
	if ((pAdapter->ApCliTab.ApCliEntry[0].AuthMode != Ndis802_11AuthModeWPAPSK) 
		 && (pAdapter->ApCliTab.ApCliEntry[0].AuthMode != Ndis802_11AuthModeWPA2PSK))
    {
     	printk("The AP Client only supports WPAPSK or WPA2PSK.\n");
        return;
	}
	
    if (((pMsg->ProVer!= EAPOL_VER) && (pMsg->ProVer!= EAPOL_VER2)) || ((pMsg->KeyDesc.Type != WPA1_KEY_DESC) && (pMsg->KeyDesc.Type != WPA2_KEY_DESC)))
    {
        printk("Key descripter does not match with WPA rule \n");
        return;
    }

	// First validate replay counter, only accept message with larger replay counter
	// Let equal pass, some AP start with all zero replay counter
	NdisZeroMemory(ZeroReplay, LEN_KEY_DESC_REPLAY);
	if ((RTMPCompareMemory(pKeyDesc->RCounter, pAdapter->ApCliTab.ApCliEntry[ApCliIdx].ReplayCounter, LEN_KEY_DESC_REPLAY) != 1) &&
		(RTMPCompareMemory(pKeyDesc->RCounter, ZeroReplay, LEN_KEY_DESC_REPLAY) != 0))
    {
        printk("   ReplayCounter not match   \n");
		return;
    }

	// Classify message Type, either pairwise message 1, 3, or group message 1 for supplicant    
    // Process for WPA2PSK
    if (pAdapter->ApCliTab.ApCliEntry[0].AuthMode == Ndis802_11AuthModeWPA2PSK)
    {
        if ((pKeyDesc->Keyinfo.KeyType == 1) &&
            (pKeyDesc->Keyinfo.EKD == 0) &&
            (pKeyDesc->Keyinfo.KeyAck == 1) &&
            (pKeyDesc->Keyinfo.KeyMic == 0) &&
            (pKeyDesc->Keyinfo.Secure == 0) &&
            (pKeyDesc->Keyinfo.Error == 0) &&
            (pKeyDesc->Keyinfo.Request == 0))
        {
            MsgType = EAPOL_PAIR_MSG_1;
#ifdef RTL865X_SOC
			printk("[WPA2PSK] Receive EAPOL Key Pairwise Message 1\n");
#else            
            DBGPRINT(RT_DEBUG_TRACE, "[WPA2PSK] Receive EAPOL Key Pairwise Message 1\n");
#endif            
        }
        else if ((pKeyDesc->Keyinfo.KeyType == 1) &&
                (pKeyDesc->Keyinfo.EKD  == 1) &&
                (pKeyDesc->Keyinfo.KeyAck == 1) &&
                (pKeyDesc->Keyinfo.KeyMic == 1) &&
                (pKeyDesc->Keyinfo.Secure == 1) &&
                (pKeyDesc->Keyinfo.Error == 0) &&
                (pKeyDesc->Keyinfo.Request == 0))
        {
            MsgType = EAPOL_PAIR_MSG_3;
#ifdef RTL865X_SOC
			printk("[WPA2PSK] Receive EAPOL Key Pairwise Message 3\n");
#else            
            DBGPRINT(RT_DEBUG_TRACE, "[WPA2PSK] Receive EAPOL Key Pairwise Message 3\n");
#endif            
        }
        else if ((pKeyDesc->Keyinfo.KeyType == 0) &&
                (pKeyDesc->Keyinfo.EKD == 1) &&
                (pKeyDesc->Keyinfo.KeyAck == 1) &&
                (pKeyDesc->Keyinfo.KeyMic == 1) &&
                (pKeyDesc->Keyinfo.Secure == 1) &&
                (pKeyDesc->Keyinfo.Error == 0) &&
                (pKeyDesc->Keyinfo.Request == 0))
        {
            MsgType = EAPOL_GROUP_MSG_1;
#ifdef RTL865X_SOC
			printk("[WPA2PSK] Receive EAPOL Key Group Message 1\n");
#else
            DBGPRINT(RT_DEBUG_TRACE, "[WPA2PSK] Receive EAPOL Key Group Message 1\n");
#endif            
        }
        else
        {
        	MsgType = EAPOL_MSG_INVALID;
        	printk("[WPA2PSK] Receive INVALID EAPOL Message \n");
        	return;
        }	
    
#ifdef BIG_ENDIAN
        // recovery original byte order, before forward Elem to another routine	   
{
        USHORT	tmpKeyinfo;

        NdisMoveMemory(&tmpKeyinfo, &pKeyDesc->Keyinfo, sizeof(USHORT)); 
        tmpKeyinfo = SWAP16(tmpKeyinfo);
	    NdisMoveMemory(&pKeyDesc->Keyinfo, &tmpKeyinfo, sizeof(USHORT));
}
#endif

        // We will assume link is up (assoc suceess and port not secured).
        // All state has to be able to process message from previous state
        switch (pAdapter->ApCliTab.ApCliEntry[ApCliIdx].WpaState)
        {
            case SS_START:
                if (MsgType == EAPOL_PAIR_MSG_1)
                {
                    Wpa2PairMsg1Action(pAdapter, Elem, ApCliIdx);
                    pAdapter->ApCliTab.ApCliEntry[ApCliIdx].WpaState = SS_WAIT_MSG_3;
                }
                break;
                        
			case SS_WAIT_MSG_3:
			    if (MsgType == EAPOL_PAIR_MSG_1)
			    {
			        Wpa2PairMsg1Action(pAdapter, Elem, ApCliIdx);
			        pAdapter->ApCliTab.ApCliEntry[ApCliIdx].WpaState = SS_WAIT_MSG_3;
			    }
			    else if (MsgType == EAPOL_PAIR_MSG_3)
			    {
			        Wpa2PairMsg3Action(pAdapter, Elem, ApCliIdx);
			        pAdapter->ApCliTab.ApCliEntry[ApCliIdx].WpaState = SS_WAIT_GROUP;
			    }
			    break;
			        
			case SS_WAIT_GROUP:     // When doing group key exchange
			case SS_FINISH:         // This happened when update group key
			    if (MsgType == EAPOL_PAIR_MSG_1)
			    {
			        Wpa2PairMsg1Action(pAdapter, Elem, ApCliIdx);
			        pAdapter->ApCliTab.ApCliEntry[ApCliIdx].WpaState = SS_WAIT_MSG_3;
			        // Reset port secured variable
			        pAdapter->ApCliTab.ApCliEntry[ApCliIdx].PortSecured = WPA_802_1X_PORT_NOT_SECURED;
			    }
			    else if (MsgType == EAPOL_PAIR_MSG_3)
			    {
			        Wpa2PairMsg3Action(pAdapter, Elem, ApCliIdx);
			        pAdapter->ApCliTab.ApCliEntry[ApCliIdx].WpaState = SS_WAIT_GROUP;
			        // Reset port secured variable
			        pAdapter->ApCliTab.ApCliEntry[ApCliIdx].PortSecured = WPA_802_1X_PORT_NOT_SECURED;
			    }
			    else if (MsgType == EAPOL_GROUP_MSG_1)
			    {
			        WpaGroupMsg1Action(pAdapter, Elem, ApCliIdx);
			        pAdapter->ApCliTab.ApCliEntry[ApCliIdx].WpaState = SS_FINISH;
			    }
			    break;
			        
			default:
			    break;              
        } 
    }
    // Process for WPAPSK
    else if (pAdapter->ApCliTab.ApCliEntry[0].AuthMode == Ndis802_11AuthModeWPAPSK)
    {
		if ((pKeyDesc->Keyinfo.KeyType == 1) &&
		    (pKeyDesc->Keyinfo.KeyIndex == 0) &&
		    (pKeyDesc->Keyinfo.KeyAck == 1) &&
		    (pKeyDesc->Keyinfo.KeyMic == 0) &&
		    (pKeyDesc->Keyinfo.Secure == 0) &&
		    (pKeyDesc->Keyinfo.Error == 0) &&
		    (pKeyDesc->Keyinfo.Request == 0))
	    {
		    MsgType = EAPOL_PAIR_MSG_1;
#ifdef RTL865X_SOC
			printk("[WPAPSK] Receive EAPOL Key Pairwise Message 1\n");
#else		    
		    DBGPRINT(RT_DEBUG_TRACE, "[WPAPSK] Receive EAPOL Key Pairwise Message 1\n");
#endif		    
	    }
	    else if ((pKeyDesc->Keyinfo.KeyType == 1) &&
		    (pKeyDesc->Keyinfo.KeyIndex == 0) &&
		    (pKeyDesc->Keyinfo.KeyAck == 1) &&
		    (pKeyDesc->Keyinfo.KeyMic == 1) &&
		    (pKeyDesc->Keyinfo.Secure == 0) &&
		    (pKeyDesc->Keyinfo.Error == 0) &&
		    (pKeyDesc->Keyinfo.Request == 0))
	    {
		    MsgType = EAPOL_PAIR_MSG_3;
#ifdef RTL865X_SOC
			printk("[WPAPSK] Receive EAPOL Key Pairwise Message 3\n");
#else		    
		    DBGPRINT(RT_DEBUG_TRACE, "[WPAPSK] Receive EAPOL Key Pairwise Message 3\n");
#endif		    
	    }
	    else if ((pKeyDesc->Keyinfo.KeyType == 0) &&
		    (pKeyDesc->Keyinfo.KeyIndex != 0) &&
		    (pKeyDesc->Keyinfo.KeyAck == 1) &&
		    (pKeyDesc->Keyinfo.KeyMic == 1) &&
		    (pKeyDesc->Keyinfo.Secure == 1) &&
		    (pKeyDesc->Keyinfo.Error == 0) &&
		    (pKeyDesc->Keyinfo.Request == 0))
	    {
		    MsgType = EAPOL_GROUP_MSG_1;
#ifdef RTL865X_SOC
			printk("[WPAPSK] Receive EAPOL Key Group Message 1\n");
#else		    
		    DBGPRINT(RT_DEBUG_TRACE, "[WPAPSK] Receive EAPOL Key Group Message 1\n");
#endif		    
	    }
	    else
        {
        	MsgType = EAPOL_MSG_INVALID;
        	printk("[WPAPSK] Receive INVALID EAPOL Message \n");
        	return;
        }
	    
#ifdef BIG_ENDIAN
        // recovery original byte order, before forward Elem to another routine	   
		{
	    	USHORT	tmpKeyinfo;

	    	NdisMoveMemory(&tmpKeyinfo, &pKeyDesc->Keyinfo, sizeof(USHORT)); 
        	tmpKeyinfo = SWAP16(tmpKeyinfo);
	    	NdisMoveMemory(&pKeyDesc->Keyinfo, &tmpKeyinfo, sizeof(USHORT));
		}
#endif
		
		// We will assume link is up (assoc suceess and port not secured).
		// All state has to be able to process message from previous state
		switch (pAdapter->ApCliTab.ApCliEntry[ApCliIdx].WpaState)
		{
		    case SS_START:
			    if (MsgType == EAPOL_PAIR_MSG_1)
			    {
				    WpaPairMsg1Action(pAdapter, Elem, ApCliIdx);
				    pAdapter->ApCliTab.ApCliEntry[ApCliIdx].WpaState = SS_WAIT_MSG_3;
			    }
			    break;
				
		    case SS_WAIT_MSG_3:
			    if (MsgType == EAPOL_PAIR_MSG_1)
			    {
				    WpaPairMsg1Action(pAdapter, Elem, ApCliIdx);
				    pAdapter->ApCliTab.ApCliEntry[ApCliIdx].WpaState = SS_WAIT_MSG_3;
			    }
			    else if (MsgType == EAPOL_PAIR_MSG_3)
			    {
				    WpaPairMsg3Action(pAdapter, Elem, ApCliIdx);
				    pAdapter->ApCliTab.ApCliEntry[ApCliIdx].WpaState = SS_WAIT_GROUP;
			    }
			    break;
				
		    case SS_WAIT_GROUP:		// When doing group key exchange
		    case SS_FINISH:			// This happened when update group key
			    if (MsgType == EAPOL_PAIR_MSG_1)
			    {
				    WpaPairMsg1Action(pAdapter, Elem, ApCliIdx);
				    pAdapter->ApCliTab.ApCliEntry[ApCliIdx].WpaState = SS_WAIT_MSG_3;
				    // Reset port secured variable
				    pAdapter->ApCliTab.ApCliEntry[ApCliIdx].PortSecured = WPA_802_1X_PORT_NOT_SECURED;
			    }
			    else if (MsgType == EAPOL_PAIR_MSG_3)
			    {
				    WpaPairMsg3Action(pAdapter, Elem, ApCliIdx);
				    pAdapter->ApCliTab.ApCliEntry[ApCliIdx].WpaState = SS_WAIT_GROUP;
				    // Reset port secured variable
				    pAdapter->ApCliTab.ApCliEntry[ApCliIdx].PortSecured = WPA_802_1X_PORT_NOT_SECURED;
			    }
			    else if (MsgType == EAPOL_GROUP_MSG_1)
			    {
				    WpaGroupMsg1Action(pAdapter, Elem, ApCliIdx);
				    pAdapter->ApCliTab.ApCliEntry[ApCliIdx].WpaState = SS_FINISH;
			    }
			    break;
				
		    default:
			    break;				
	    }
    }	
}

/*
	==========================================================
	
	Routine Description:
		Choose MsgType for WPAPSK state machine
	
	========================================================== 
*/
BOOLEAN StaWPAMsgTypeSubst(
    IN UCHAR    EAPType,
    OUT INT    *MsgType) 
{
	DBGPRINT(RT_DEBUG_TRACE, "StaWPAMsgTypeSubst (EAPType=%d)\n", EAPType);
	
    switch(EAPType)
    {
        case EAPPacket:
            *MsgType = STA_MT2_EAPPacket;
            break;

        case EAPOLStart:
            *MsgType = STA_MT2_EAPOLStart;
            break;

        case EAPOLLogoff:
            *MsgType = STA_MT2_EAPOLLogoff;
            break;

        case EAPOLKey:
            *MsgType = STA_MT2_EAPOLKey;
            break;

        case EAPOLASFAlert:
            *MsgType = STA_MT2_EAPOLASFAlert;
            break;

        default:
            DBGPRINT(RT_DEBUG_TRACE, "Unknown WPA MsgType\n");
            return FALSE;
    }
    
    return TRUE;
}

/*
	========================================================================
	
	Routine Description:
		Process Pairwise key Msg-1 of 4-way handshaking and send Msg-2 

	Arguments:
		pAd			Pointer	to our adapter
		Elem		Message body
		
	Return Value:
		None
		
	Note:
		
	========================================================================
*/
VOID	WpaPairMsg1Action(
	IN	PRTMP_ADAPTER	pAd, 
	IN	MLME_QUEUE_ELEM	*Elem,
	IN	INT				IfIndex) 
{
	PHEADER_802_11		pHeader;
	UCHAR				PTK[80];
	PUCHAR				pOutBuffer = NULL;
	HEADER_802_11		Header_802_11;
	UCHAR				AckRate = RATE_2;
	USHORT				AckDuration = 0;
	ULONG				FrameLen = 0;
	UCHAR				EAPHEAD[8] = {0xaa,	0xaa, 0x03,	0x00, 0x00,	0x00, 0x88, 0x8e};
	PEAPOL_PACKET		pMsg1;
	EAPOL_PACKET		Packet;
	UCHAR				Mic[16];
	   
	DBGPRINT(RT_DEBUG_TRACE, "WpaPairMsg1Action ----->\n");
	
	// Store the received frame
	pHeader	= (PHEADER_802_11) Elem->Msg;
	pMsg1 = (PEAPOL_PACKET) &Elem->Msg[LENGTH_802_11 + LENGTH_802_1_H];
	
	// Process message 1 from authenticator
	// Key must be Pairwise key, already verified at caller.
	// 1. Save Replay counter, it will use to verify message 3 and construct message 2
	NdisMoveMemory(pAd->ApCliTab.ApCliEntry[IfIndex].ReplayCounter, pMsg1->KeyDesc.RCounter, LEN_KEY_DESC_REPLAY);		

	// 2. Save ANonce
	NdisMoveMemory(pAd->ApCliTab.ApCliEntry[IfIndex].ANonce, pMsg1->KeyDesc.Nonce, LEN_KEY_DESC_NONCE);
		
	// TSNonce <--- SNonce
	// Generate random SNonce
	GenRandom(pAd, pAd->ApCliTab.ApCliEntry[IfIndex].SNonce, IfIndex);  

    // TPTK <--- Calc PTK(ANonce, TSNonce)
    CountPTK(pAd->ApCliTab.ApCliEntry[IfIndex].PMK,
		        pAd->ApCliTab.ApCliEntry[IfIndex].ANonce,
			    pAd->ApCliTab.ApCliEntry[IfIndex].ApCliBssid, 
			    pAd->ApCliTab.ApCliEntry[IfIndex].SNonce, 
			    pAd->ApCliTab.ApCliEntry[IfIndex].CurrentAddress, 
			    PTK, 
			    LEN_PTK);
			    
	// Save key to PTK entry
	NdisMoveMemory(pAd->ApCliTab.ApCliEntry[IfIndex].PTK, PTK, LEN_PTK);
	
	// =====================================
	// Use Priority Ring & MiniportMMRequest
	// =====================================
	pAd->Sequence = ((pAd->Sequence) + 1) & (MAX_SEQ_NUMBER);
	ApCliWpaMacHeaderInit(	pAd, 
						  	&Header_802_11, 
						   	0, 
							pAd->ApCliTab.ApCliEntry[IfIndex].ApCliBssid,
							pAd->ApCliTab.ApCliEntry[IfIndex].CurrentAddress);

	// ACK size	is 14 include CRC, and its rate	is based on real time information
	AckRate = pAd->PortCfg.ExpectedACKRate[pAd->ApCliTab.ApCliEntry[IfIndex].CurrTxRate];
	AckDuration = RTMPCalcDuration(pAd, AckRate, 14);
	Header_802_11.Duration = pAd->PortCfg.Dsifs + AckDuration;
	
	// Zero message 2 body
	NdisZeroMemory(&Packet, sizeof(Packet));
	Packet.ProVer 	= EAPOL_VER;
	Packet.ProType	= EAPOLKey;
	//
	// Message 2 as  EAPOL-Key(0,1,0,0,0,P,0,SNonce,MIC,RSN IE)
	//
	Packet.KeyDesc.Type = WPA1_KEY_DESC;
	// 1. Key descriptor version and appropriate RSN IE
	if (pAd->ApCliTab.ApCliEntry[0].PairCipher == Ndis802_11Encryption3Enabled)
	{
		Packet.KeyDesc.Keyinfo.KeyDescVer = 2;
	}
	else	// TKIP
	{
        Packet.KeyDesc.Keyinfo.KeyDescVer = 1;
	}
	
	// fill in Data Material and its length
	Packet.KeyDesc.Data[0] = IE_WPA;
	Packet.KeyDesc.Data[1] = pAd->ApCliTab.ApCliEntry[0].RSN_IELen;
	Packet.KeyDesc.DataLen[1] = pAd->ApCliTab.ApCliEntry[0].RSN_IELen + 2;
	NdisMoveMemory(&Packet.KeyDesc.Data[2], pAd->ApCliTab.ApCliEntry[0].RSN_IE, pAd->ApCliTab.ApCliEntry[0].RSN_IELen);

	// Update packet length after decide Key data payload
	Packet.Body_Len[1]  = sizeof(KEY_DESCRIPTER) - MAX_LEN_OF_RSNIE + Packet.KeyDesc.DataLen[1];

	// Update Key length - same as message 1
	Packet.KeyDesc.KeyLength[0] = pMsg1->KeyDesc.KeyLength[0];
	Packet.KeyDesc.KeyLength[1] = pMsg1->KeyDesc.KeyLength[1];	
	
	// 2. Key Type is Pairwise Key
	Packet.KeyDesc.Keyinfo.KeyType = 1;

	// 3. KeyMic field presented
	Packet.KeyDesc.Keyinfo.KeyMic  = 1;

	// 4. Fill SNonce
	NdisMoveMemory(Packet.KeyDesc.Nonce, pAd->ApCliTab.ApCliEntry[IfIndex].SNonce, LEN_KEY_DESC_NONCE);

	// 5. Key Replay Count
	NdisMoveMemory(Packet.KeyDesc.RCounter, pAd->ApCliTab.ApCliEntry[IfIndex].ReplayCounter, LEN_KEY_DESC_REPLAY);		

#ifdef BIG_ENDIAN
    // recovery original byte order, before forward Elem to another routine	   
{
    USHORT	tmpKeyinfo;

    NdisMoveMemory(&tmpKeyinfo, &Packet.KeyDesc.Keyinfo, sizeof(USHORT));
    tmpKeyinfo = SWAP16(tmpKeyinfo);
    NdisMoveMemory(&Packet.KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT));
}      
#endif
	
	// Send EAPOL(0, 1, 0, 0, 0, P, 0, SNonce, MIC, RSN_IE)
	// Out buffer for transmitting message 2
    pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);  // allocate memory
    if (pOutBuffer == NULL)
		return;					

	// Prepare EAPOL frame for MIC calculation
	// Be careful, only EAPOL frame is counted for MIC calculation
	MakeOutgoingFrame(pOutBuffer,           &FrameLen,
		              Packet.Body_Len[1] + 4,    &Packet,
		              END_OF_ARGS);

	// 6. Prepare and Fill MIC value
	NdisZeroMemory(Mic,	sizeof(Mic));
	if (pAd->ApCliTab.ApCliEntry[0].PairCipher == Ndis802_11Encryption3Enabled)
	{
		// AES
		UCHAR digest[80];
			
		HMAC_SHA1(pOutBuffer, FrameLen, PTK, LEN_EAP_MICK, digest);
		NdisMoveMemory(Mic,	digest,	LEN_KEY_DESC_MIC);
	}
	else
	{
#if 1		
		INT i;
		printk(" PMK = ");
		for (i = 0; i < 16; i++)
			printk("%2x-", pAd->ApCliTab.ApCliEntry[IfIndex].PMK[i]);
		
		printk("\n PTK = ");
		for (i = 0; i < 64; i++)
			printk("%2x-", pAd->ApCliTab.ApCliEntry[IfIndex].PTK[i]);
		printk("\n FrameLen = %d\n", FrameLen);
#endif		
	    hmac_md5(PTK,  LEN_EAP_MICK, pOutBuffer, FrameLen, Mic);
	}
	NdisMoveMemory(Packet.KeyDesc.MIC, Mic, LEN_KEY_DESC_MIC);

    FrameLen = 0;
	MakeOutgoingFrame(pOutBuffer,           &FrameLen,	
	                  sizeof(HEADER_802_11),&Header_802_11,
		              sizeof(EAPHEAD),      EAPHEAD, 
		              Packet.Body_Len[1] + 4,    &Packet,
		              END_OF_ARGS);

	// Send using priority queue
	MiniportMMRequest(pAd, pOutBuffer, FrameLen);
   	kfree(pOutBuffer);
		
	DBGPRINT(RT_DEBUG_TRACE, "WpaPairMsg1Action <-----\n");
}	

/*
	========================================================================
	
	Routine Description:
		Process Pairwise key Msg 3 of 4-way handshaking and send Msg 4 

	Arguments:
		pAd	Pointer	to our adapter
		Elem		Message body
		
	Return Value:
		None
		
	Note:
		
	========================================================================
*/
VOID	WpaPairMsg3Action(
	IN	PRTMP_ADAPTER	pAd, 
	IN	MLME_QUEUE_ELEM	*Elem,
	IN	INT				IfIndex) 
{
	PHEADER_802_11		pHeader;
	PUCHAR				pOutBuffer = NULL;
	HEADER_802_11		Header_802_11;
	UCHAR				AckRate = RATE_2;
	USHORT				AckDuration = 0;
	ULONG				FrameLen = 0;
	UCHAR				EAPHEAD[8] = {0xaa,	0xaa, 0x03,	0x00, 0x00,	0x00,0x88,0x8e};
	EAPOL_PACKET		Packet;
	PEAPOL_PACKET		pMsg3;
	UCHAR				Mic[16], OldMic[16];	
	RT_802_11_APCLI_ENTRY *pApCliEntry = &pAd->ApCliTab.ApCliEntry[IfIndex];
	   
	DBGPRINT(RT_DEBUG_TRACE, "WpaPairMsg3Action ----->\n");
	
	// Store the received frame
	pHeader	= (PHEADER_802_11) Elem->Msg;
	pMsg3 = (PEAPOL_PACKET) &Elem->Msg[LENGTH_802_11 + LENGTH_802_1_H];


#ifdef BIG_ENDIAN
   // recovery original byte order, before forward Elem to another routine	   
{
    USHORT	tmpKeyinfo;

    NdisMoveMemory(&tmpKeyinfo, &pMsg3->KeyDesc.Keyinfo, sizeof(USHORT));
    tmpKeyinfo = SWAP16(tmpKeyinfo);
    NdisMoveMemory(&pMsg3->KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT));
}
#endif

	// 1. Verify RSN IE & cipher type match
	if (pAd->ApCliTab.ApCliEntry[0].PairCipher == Ndis802_11Encryption3Enabled && (pMsg3->KeyDesc.Keyinfo.KeyDescVer != 2))
	{
        return;
	}
	else if (pAd->ApCliTab.ApCliEntry[0].PairCipher == Ndis802_11Encryption2Enabled && (pMsg3->KeyDesc.Keyinfo.KeyDescVer != 1))
	{
	    return;
	}

#ifdef BIG_ENDIAN
   // recovery original byte order, before forward Elem to another routine	   
{
    USHORT	tmpKeyinfo;

    NdisMoveMemory(&tmpKeyinfo, &pMsg3->KeyDesc.Keyinfo, sizeof(USHORT));
    tmpKeyinfo = SWAP16(tmpKeyinfo);
    NdisMoveMemory(&pMsg3->KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT));
}
#endif
	
	// 2. Check MIC value
	// Save the MIC and replace with zero
	NdisMoveMemory(OldMic, pMsg3->KeyDesc.MIC, LEN_KEY_DESC_MIC);
	NdisZeroMemory(pMsg3->KeyDesc.MIC, LEN_KEY_DESC_MIC);
	if (pAd->ApCliTab.ApCliEntry[0].PairCipher == Ndis802_11Encryption3Enabled)
	{
		// AES
		UCHAR digest[80];
			
		HMAC_SHA1((PUCHAR) pMsg3, pMsg3->Body_Len[1] + 4, pApCliEntry->PTK, LEN_EAP_MICK, digest);
		NdisMoveMemory(Mic,	digest,	LEN_KEY_DESC_MIC);
	}
	else
	{
		hmac_md5(pApCliEntry->PTK, LEN_EAP_MICK, (PUCHAR) pMsg3, pMsg3->Body_Len[1] + 4, Mic);
	}
	
	if (!NdisEqualMemory(OldMic, Mic, LEN_KEY_DESC_MIC))
	{
		DBGPRINT(RT_DEBUG_ERROR, " MIC Different in msg 3 of 4-way handshake!!!!!!!!!! \n");
		return;
	}
	else
		DBGPRINT(RT_DEBUG_TRACE, " MIC VALID in msg 3 of 4-way handshake!!!!!!!!!! \n");

	// 3. Check Replay Counter, it has to be larger than last one. No need to be exact one larger
	if (RTMPCompareMemory(pMsg3->KeyDesc.RCounter, pApCliEntry->ReplayCounter, LEN_KEY_DESC_REPLAY) != 1)
	{
		DBGPRINT(RT_DEBUG_ERROR, " Check Replay Counter Fail \n");
		return;
	}

	// check RSN IE
	if (!RTMPEqualMemory(pMsg3->KeyDesc.Data, pApCliEntry->RSN_IE_from_AP, pApCliEntry->RSN_IELen_from_AP))
	{       
		DBGPRINT(RT_DEBUG_ERROR, "RSN_IE Different in Msg 3 of WPA1 4-way handshake!! \n");
		return;
	}
	else
		DBGPRINT(RT_DEBUG_TRACE, "RSN_IE VALID in Msg 3 of WPA1 4-way handshake!! \n");
	/*
	if (pMsg3->KeyDesc.Data[0] == pApCliEntry->RSN_IE[0])
	{
		if (!RTMPEqualMemory(pMsg3->KeyDesc.Data, pApCliEntry->RSN_IE, pMsg3->KeyDesc.DataLen[1]))
		{       
			DBGPRINT(RT_DEBUG_ERROR, "RSN_IE Different in Msg 3 of WPA1 4-way handshake!! \n");
			return;
		}
		else
			DBGPRINT(RT_DEBUG_TRACE, "RSN_IE VALID in Msg 3 of WPA1 4-way handshake!! \n");
	}
	else
	{
		if (!RTMPEqualMemory(pMsg3->KeyDesc.Data, (UCHAR)(pApCliEntry->RSN_IE+pApCliEntry->RSN_IE[1]+2), pMsg3->KeyDesc.DataLen[1]))
		{       
			DBGPRINT(RT_DEBUG_ERROR, "RSN_IE Different in Msg 3 of WPA1 4-way handshake!! \n");
			return;
		}
		else
			DBGPRINT(RT_DEBUG_TRACE, "RSN_IE VALID in Msg 3 of WPA1 4-way handshake!! \n");
	}*/

	// Update new replay counter
	NdisMoveMemory(pApCliEntry->ReplayCounter, pMsg3->KeyDesc.RCounter, LEN_KEY_DESC_REPLAY);		

	// 4. Double check ANonce
	if (!NdisEqualMemory(pApCliEntry->ANonce, pMsg3->KeyDesc.Nonce, LEN_KEY_DESC_NONCE))
		return;
	
	// 5. Construct Message 4
	// =====================================
	// Use Priority Ring & MiniportMMRequest
	// =====================================
    pAd->Sequence = ((pAd->Sequence) + 1) & (MAX_SEQ_NUMBER);
	ApCliWpaMacHeaderInit(	pAd, 
						  	&Header_802_11, 
						   	0, 
							pApCliEntry->ApCliBssid,
							pApCliEntry->CurrentAddress);

	// ACK size	is 14 include CRC, and its rate	is based on real time information
	AckRate = pAd->PortCfg.ExpectedACKRate[pApCliEntry->CurrTxRate];
	AckDuration = RTMPCalcDuration(pAd, AckRate, 14);
	Header_802_11.Duration = pAd->PortCfg.Dsifs + AckDuration;
	
	// Zero message 4 body
	NdisZeroMemory(&Packet, sizeof(Packet));
	Packet.ProVer = EAPOL_VER;
	Packet.ProType    = EAPOLKey;
	Packet.Body_Len[1]  = sizeof(KEY_DESCRIPTER) - MAX_LEN_OF_RSNIE;		// No data field
	
	//
	// Message 4 as  EAPOL-Key(0,1,0,0,0,P,0,0,MIC,0)
	//
	Packet.KeyDesc.Type = WPA1_KEY_DESC;
	
#ifdef BIG_ENDIAN
   // recovery original byte order, before forward Elem to another routine	   
{
    USHORT	tmpKeyinfo;

    NdisMoveMemory(&tmpKeyinfo, &pMsg3->KeyDesc.Keyinfo, sizeof(USHORT));
    tmpKeyinfo = SWAP16(tmpKeyinfo);
    NdisMoveMemory(&pMsg3->KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT));
}
#endif
	
	// Key descriptor version and appropriate RSN IE
	Packet.KeyDesc.Keyinfo.KeyDescVer = pMsg3->KeyDesc.Keyinfo.KeyDescVer;

	// Update Key Length
	Packet.KeyDesc.KeyLength[0] = pMsg3->KeyDesc.KeyLength[0];
	Packet.KeyDesc.KeyLength[1] = pMsg3->KeyDesc.KeyLength[1];
	
	// Key Type PeerKey
	Packet.KeyDesc.Keyinfo.KeyType = 1;

	// KeyMic field presented
	Packet.KeyDesc.Keyinfo.KeyMic  = 1;

    // In Msg3,  KeyInfo.secure =0 if Group Key HS to come. 1 if no group key HS
    // Station sends Msg4  KeyInfo.secure should be the same as that in Msg.3
    Packet.KeyDesc.Keyinfo.Secure= pMsg3->KeyDesc.Keyinfo.Secure;

    
	// Key Replay count	
	NdisMoveMemory(Packet.KeyDesc.RCounter, pMsg3->KeyDesc.RCounter, LEN_KEY_DESC_REPLAY);		

#ifdef BIG_ENDIAN
   // recovery original byte order, before forward Elem to another routine	   
{
    USHORT	tmpKeyinfo;

    NdisMoveMemory(&tmpKeyinfo, &Packet.KeyDesc.Keyinfo, sizeof(USHORT));
    tmpKeyinfo = SWAP16(tmpKeyinfo);
    NdisMoveMemory(&Packet.KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT));
}
#endif

	// Out buffer for transmitting message 4
	pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);  // allocate memory
    if (pOutBuffer == NULL)
		return;					

	// Prepare EAPOL frame for MIC calculation
	// Be careful, only EAPOL frame is counted for MIC calculation
	MakeOutgoingFrame(pOutBuffer,           &FrameLen,
		              Packet.Body_Len[1] + 4,    &Packet,
		              END_OF_ARGS);

	// Prepare and Fill MIC value
	NdisZeroMemory(Mic,	sizeof(Mic));
	if (pAd->ApCliTab.ApCliEntry[0].PairCipher == Ndis802_11Encryption3Enabled)
	{
		// AES
		UCHAR digest[80];
			
		HMAC_SHA1(pOutBuffer, FrameLen, pApCliEntry->PTK, LEN_EAP_MICK, digest);
		NdisMoveMemory(Mic,	digest,	LEN_KEY_DESC_MIC);
	}
	else
	{
		hmac_md5(pApCliEntry->PTK, LEN_EAP_MICK, pOutBuffer, FrameLen, Mic);
	}
	NdisMoveMemory(Packet.KeyDesc.MIC, Mic, LEN_KEY_DESC_MIC);

    FrameLen = 0;  
	// Make	transmitting frame
	MakeOutgoingFrame(pOutBuffer,           	&FrameLen,
	                  sizeof(HEADER_802_11),	&Header_802_11,
		              sizeof(EAPHEAD),      	EAPHEAD, 
		              Packet.Body_Len[1] + 4,	&Packet,
		              END_OF_ARGS);

	// Prepare Pairwise key - Key material
	NdisZeroMemory(&pApCliEntry->PairwiseKey, sizeof(CIPHER_KEY));   
	pApCliEntry->PairwiseKey.KeyLen = LEN_TKIP_EK;
    NdisMoveMemory(pApCliEntry->PairwiseKey.Key, &pApCliEntry->PTK[32], LEN_TKIP_EK);
    NdisMoveMemory(pApCliEntry->PairwiseKey.RxTsc, pMsg3->KeyDesc.RSC, 6);
    
    // Prepare Pairwise key - Select RxMic and TxMic based on supplicant
    NdisMoveMemory(pApCliEntry->PairwiseKey.RxMic, &pApCliEntry->PTK[48], LEN_TKIP_RXMICK);
    NdisMoveMemory(pApCliEntry->PairwiseKey.TxMic, &pApCliEntry->PTK[48+LEN_TKIP_RXMICK], LEN_TKIP_TXMICK);
	
	// Prepare Pairwise key - CipherAlg
	pApCliEntry->PairwiseKey.CipherAlg = CIPHER_NONE;
	if (pAd->ApCliTab.ApCliEntry[0].PairCipher == Ndis802_11Encryption2Enabled)
		pApCliEntry->PairwiseKey.CipherAlg = CIPHER_TKIP;
	else if (pAd->ApCliTab.ApCliEntry[0].PairCipher == Ndis802_11Encryption3Enabled)
		pApCliEntry->PairwiseKey.CipherAlg = CIPHER_AES;
		
	// Set pairwise key to Asic
    AsicAddPairwiseKeyEntry(
            pAd, 
            pApCliEntry->ApCliBssid, 
            (APCLI_PAIRWISE_KEY_OFFSET + IfIndex),
            pApCliEntry->PairwiseKey.CipherAlg, 
            pApCliEntry->PairwiseKey.Key,
            pApCliEntry->PairwiseKey.TxMic, 
            pApCliEntry->PairwiseKey.RxMic);


    // 6. Send Message 4 to authenticator
	// Send using priority queue
	MiniportMMRequest(pAd, pOutBuffer, FrameLen);
   	kfree(pOutBuffer);

	DBGPRINT(RT_DEBUG_TRACE, "WpaPairMsg3Action <-----\n");
}

/*
	========================================================================
	
	Routine Description:
		Process Group key 2-way handshaking

	Arguments:
		pAd	Pointer	to our adapter
		Elem		Message body
		
	Return Value:
		None
		
	Note:
		
	========================================================================
*/
VOID	WpaGroupMsg1Action(
	IN	PRTMP_ADAPTER	pAd, 
	IN	MLME_QUEUE_ELEM	*Elem,
	IN	INT				IfIndex) 
{
	PUCHAR				pOutBuffer = NULL;
    UCHAR               Header802_3[14];
	ULONG				FrameLen = 0;
	UCHAR				EAPHEAD[8] = {0xaa,	0xaa, 0x03,	0x00, 0x00,	0x00,0x88,0x8e};
	EAPOL_PACKET		Packet;
	PEAPOL_PACKET		pGroup;
//    UCHAR               MSG[MAX_LEN_OF_MLME_BUFFER];
//    UCHAR               KEYDATA[512];	   
	UCHAR				*mpool, *MSG;
	UCHAR				Mic[16], OldMic[16];
	UCHAR				GTK[32], Key[32], KEYDATA[512];
	RT_802_11_APCLI_ENTRY *pApCliEntry = &pAd->ApCliTab.ApCliEntry[IfIndex];
	   
    mpool = kmalloc(2*MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);  // allocate memory
    if (mpool == NULL)
        return;
		    
	MSG = (UCHAR *) (((UMAXLONG)(mpool+3)) & ((UMAXLONG)~(0x03)));
	//KEYDATA =(UCHAR *) (((ULONG)(MSG+MAX_LEN_OF_MLME_BUFFER+3)) & ((ULONG)~(0x03)));


    if(NdisEqualMemory(&Elem->Msg[LENGTH_802_11 + 8], EAPHEAD, LENGTH_802_1_H))
    {
        DBGPRINT(RT_DEBUG_TRACE, "WpaGroupMsg1Action ----->MsgLen=%d\n",Elem->MsgLen);
        NdisMoveMemory(MSG, Elem->Msg, LENGTH_802_11);
        NdisMoveMemory(&MSG[LENGTH_802_11], &Elem->Msg[LENGTH_802_11+8], (Elem->MsgLen ));
    }
    else
    {
	    DBGPRINT(RT_DEBUG_TRACE, "WpaGroupMsg1Action ----->\n");
        NdisMoveMemory(MSG, Elem->Msg, Elem->MsgLen);
    }

	// Process Group message 1 frame.
	pGroup = (PEAPOL_PACKET) &MSG[LENGTH_802_11 + LENGTH_802_1_H];

#ifdef BIG_ENDIAN
   // recovery original byte order, before forward Elem to another routine	   
{
    USHORT	tmpKeyinfo;

    NdisMoveMemory(&tmpKeyinfo, &pGroup->KeyDesc.Keyinfo, sizeof(USHORT));
    tmpKeyinfo = SWAP16(tmpKeyinfo);
    NdisMoveMemory(&pGroup->KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT));
}
#endif

    // 0. Verify RSN IE & cipher type match
    if (pApCliEntry->PairCipher == Ndis802_11Encryption3Enabled && (pGroup->KeyDesc.Keyinfo.KeyDescVer != 2))
    {
		DBGPRINT(RT_DEBUG_ERROR, "Key Descriptor Version is not Match when AES mode \n");
		kfree(mpool);
        return;
    }
    else if (pApCliEntry->PairCipher == Ndis802_11Encryption2Enabled && (pGroup->KeyDesc.Keyinfo.KeyDescVer != 1))
    {
		DBGPRINT(RT_DEBUG_ERROR, "Key Descriptor Version is not Match when TKIP mode \n");
		kfree(mpool);
        return;
    }

	// 1. Verify Replay counter
	//    Check Replay Counter, it has to be larger than last one. No need to be exact one larger
	if (RTMPCompareMemory(pGroup->KeyDesc.RCounter, pApCliEntry->ReplayCounter, LEN_KEY_DESC_REPLAY) != 1)
	{
        DBGPRINT(RT_DEBUG_ERROR, "Verify Replay Counter Fail for Group Msg 1 \n");
        kfree(mpool);
		return;
	}
	
#ifdef BIG_ENDIAN
   // recovery original byte order, before forward Elem to another routine	   
{
    USHORT	tmpKeyinfo;

    NdisMoveMemory(&tmpKeyinfo, &pGroup->KeyDesc.Keyinfo, sizeof(USHORT));
    tmpKeyinfo = SWAP16(tmpKeyinfo);
    NdisMoveMemory(&pGroup->KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT));
}
#endif	

	// Update new replay counter
	NdisMoveMemory(pApCliEntry->ReplayCounter, pGroup->KeyDesc.RCounter, LEN_KEY_DESC_REPLAY);		

	// 2. Verify MIC is valid
	// Save the MIC and replace with zero
	NdisMoveMemory(OldMic, pGroup->KeyDesc.MIC, LEN_KEY_DESC_MIC);
	NdisZeroMemory(pGroup->KeyDesc.MIC, LEN_KEY_DESC_MIC);
	
	if (pApCliEntry->PairCipher == Ndis802_11Encryption3Enabled)
	{
		// AES
		UCHAR digest[80];
			
        HMAC_SHA1((PUCHAR) pGroup, pGroup->Body_Len[1] + 4, pApCliEntry->PTK, LEN_EAP_MICK, digest);
		NdisMoveMemory(Mic,	digest,	LEN_KEY_DESC_MIC);
	}
	else
	{
		hmac_md5(pApCliEntry->PTK, LEN_EAP_MICK, (PUCHAR) pGroup, pGroup->Body_Len[1] + 4, Mic);
	}
	
	if (!NdisEqualMemory(OldMic, Mic, LEN_KEY_DESC_MIC))
	{
		DBGPRINT(RT_DEBUG_ERROR, " MIC Different in group msg 1 of 2-way handshake!!!!!!!!!! \n");
		kfree(mpool);
		return;
	}
	else
		DBGPRINT(RT_DEBUG_TRACE, " MIC VALID in group msg 1 of 2-way handshake!!!!!!!!!! \n");

#ifdef BIG_ENDIAN
   // recovery original byte order, before forward Elem to another routine	   
{
    USHORT	tmpKeyinfo;

    NdisMoveMemory(&tmpKeyinfo, &pGroup->KeyDesc.Keyinfo, sizeof(USHORT));
    tmpKeyinfo = SWAP16(tmpKeyinfo);
    NdisMoveMemory(&pGroup->KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT));
}
#endif

	// 3. Decrypt GTK from Key Data
    DBGPRINT(RT_DEBUG_TRACE, " Install = %d!!!!EKD = %d!!!!!KeyIndex = %d! \n", pGroup->KeyDesc.Keyinfo.Install, pGroup->KeyDesc.Keyinfo.EKD,
        pGroup->KeyDesc.Keyinfo.KeyIndex);
    DBGPRINT(RT_DEBUG_TRACE, "Key Data Length = %d \n", pGroup->KeyDesc.DataLen[1]);    

	NdisZeroMemory(pApCliEntry->SharedKey, SHARE_KEY_NUM * sizeof(CIPHER_KEY));     

	// Use EK to decrypt KEYDATA 
	if (pAd->ApCliTab.ApCliEntry[0].PairCipher	== Ndis802_11Encryption3Enabled)
	{   
		// Decrypt AES GTK
        //NdisMoveMemory(KEYDATA, pGroup->KeyDesc.Data, 32);
        AES_GTK_KEY_UNWRAP(&pApCliEntry->PTK[16], KEYDATA,  pGroup->KeyDesc.DataLen[1], pGroup->KeyDesc.Data);       
		//NdisMoveMemory(GTK, KEYDATA, 32);
	}
	else	// TKIP
	{
		INT	i;

		// Decrypt TKIP GTK
		// Construct 32 bytes RC4 Key
		NdisMoveMemory(Key, pGroup->KeyDesc.IV, 16);
		NdisMoveMemory(&Key[16], &pApCliEntry->PTK[16], 16);
		ARCFOUR_INIT(&pAd->PrivateInfo.WEPCONTEXT, Key, 32);
        //discard first 256 bytes
		for (i = 0; i < 256; i++)
            ARCFOUR_BYTE(&pAd->PrivateInfo.WEPCONTEXT);
		// Decrypt GTK. Becareful, there is no ICV to check the result is correct or not
  		ARCFOUR_DECRYPT(&pAd->PrivateInfo.WEPCONTEXT, KEYDATA, pGroup->KeyDesc.Data, pGroup->KeyDesc.DataLen[1]);		
	}
	
	// Process decrypted key data material
	// Parse keyData to handle KDE format for WPA2PSK
	if (pGroup->KeyDesc.Keyinfo.EKD)
	{
		if (!ParseKeyData(pAd, KEYDATA, pGroup->KeyDesc.DataLen[1], IfIndex, 0))
        	return;
	}	
	else
	{
		// set key material, TxMic and RxMic for WPAPSK
		NdisMoveMemory(GTK, KEYDATA, 32);
		NdisMoveMemory(pApCliEntry->GTK, GTK, 32);
		pApCliEntry->DefaultKeyId = pGroup->KeyDesc.Keyinfo.KeyIndex; 
		NdisMoveMemory(pApCliEntry->SharedKey[pApCliEntry->DefaultKeyId].Key, GTK, LEN_TKIP_EK);
		NdisMoveMemory(pApCliEntry->SharedKey[pApCliEntry->DefaultKeyId].RxMic, &GTK[16], LEN_TKIP_RXMICK);
		NdisMoveMemory(pApCliEntry->SharedKey[pApCliEntry->DefaultKeyId].TxMic, &GTK[24], LEN_TKIP_TXMICK);
    
    	pApCliEntry->SharedKey[pApCliEntry->DefaultKeyId].KeyLen = LEN_TKIP_EK;

#if 1	//KeyMaterialDump
		{
			INT	i;
			
			printk("Set Group Key, KeyIndex(%d) for IF(apcli%d) ", pApCliEntry->DefaultKeyId, IfIndex);
			for (i = 0; i < LEN_TKIP_EK; i++)
				printk("%02x ", pApCliEntry->SharedKey[pApCliEntry->DefaultKeyId].Key[i]);
			printk("\n");	
		}
#endif
    	
		// Update CipherAlg
		pApCliEntry->SharedKey[pApCliEntry->DefaultKeyId].CipherAlg = CIPHER_NONE;
		if (pApCliEntry->GroupCipher == Ndis802_11Encryption2Enabled)
			pApCliEntry->SharedKey[pApCliEntry->DefaultKeyId].CipherAlg = CIPHER_TKIP;
		else if (pApCliEntry->GroupCipher == Ndis802_11Encryption3Enabled)
			pApCliEntry->SharedKey[pApCliEntry->DefaultKeyId].CipherAlg = CIPHER_AES;
	}

    // 802.1x port control
	pApCliEntry->PortSecured = WPA_802_1X_PORT_SECURED;
		
	// 4. Construct Group Message 2
    pAd->Sequence = ((pAd->Sequence) + 1) & (MAX_SEQ_NUMBER);

    // Make 802.3 header
	NdisZeroMemory(Header802_3, sizeof(Header802_3));
	NdisMoveMemory(Header802_3, pApCliEntry->ApCliBssid, MAC_ADDR_LEN);
    NdisMoveMemory(&Header802_3[MAC_ADDR_LEN], pApCliEntry->CurrentAddress, MAC_ADDR_LEN);
    NdisMoveMemory(&Header802_3[2*MAC_ADDR_LEN], EAPOL, LENGTH_802_3_TYPE);
	
	// Zero Group message 1 body
	NdisZeroMemory(&Packet, sizeof(Packet));
	Packet.ProVer 	= EAPOL_VER;
	Packet.ProType  = EAPOLKey;
	Packet.Body_Len[1]  = sizeof(KEY_DESCRIPTER) - MAX_LEN_OF_RSNIE;		// No data field
	
	//
	// Group Message 2 as  EAPOL-Key(1,0,0,0,G,0,0,MIC,0)
	//
	Packet.KeyDesc.Type = WPA1_KEY_DESC;
	
	// Key descriptor version and appropriate RSN IE
	Packet.KeyDesc.Keyinfo.KeyDescVer = pGroup->KeyDesc.Keyinfo.KeyDescVer;

	// Update Key Length and Key Index
	Packet.KeyDesc.Keyinfo.KeyIndex = pGroup->KeyDesc.Keyinfo.KeyIndex;
	Packet.KeyDesc.KeyLength[0] = pGroup->KeyDesc.KeyLength[0];
	Packet.KeyDesc.KeyLength[1] = pGroup->KeyDesc.KeyLength[1];	

	// Key Type Group key
	Packet.KeyDesc.Keyinfo.KeyType = 0;

	// KeyMic field presented
	Packet.KeyDesc.Keyinfo.KeyMic  = 1;

	// Secure bit
	Packet.KeyDesc.Keyinfo.Secure  = 1;
	
	// Key Replay count	
	NdisMoveMemory(Packet.KeyDesc.RCounter, pGroup->KeyDesc.RCounter, LEN_KEY_DESC_REPLAY);		

#ifdef BIG_ENDIAN
   // recovery original byte order, before forward Elem to another routine	   
{
    USHORT	tmpKeyinfo;

    NdisMoveMemory(&tmpKeyinfo, &Packet.KeyDesc.Keyinfo, sizeof(USHORT));
    tmpKeyinfo = SWAP16(tmpKeyinfo);
    NdisMoveMemory(&Packet.KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT));
}
#endif

	// Out buffer for transmitting group message 2
    pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);  // allocate memory
    if (pOutBuffer == NULL)
    {   
        kfree(mpool);
		return;			
    }
    
	// Prepare EAPOL frame for MIC calculation
	// Be careful, only EAPOL frame is counted for MIC calculation
	MakeOutgoingFrame(pOutBuffer,           &FrameLen,
		              Packet.Body_Len[1] + 4,    &Packet,
		              END_OF_ARGS);

	// Prepare and Fill MIC value
	NdisZeroMemory(Mic,	sizeof(Mic));
	if (pAd->ApCliTab.ApCliEntry[0].PairCipher	== Ndis802_11Encryption3Enabled)
	{
		// AES
		UCHAR digest[80];
			
		HMAC_SHA1(pOutBuffer, FrameLen, pApCliEntry->PTK, LEN_EAP_MICK, digest);
		NdisMoveMemory(Mic,	digest,	LEN_KEY_DESC_MIC);
	}
	else
	{
		hmac_md5(pApCliEntry->PTK, LEN_EAP_MICK, pOutBuffer, FrameLen, Mic);
	}
	NdisMoveMemory(Packet.KeyDesc.MIC, Mic, LEN_KEY_DESC_MIC);


    MakeOutgoingFrame(pOutBuffer,       &FrameLen, 
                      LENGTH_802_3,     &Header802_3,
		              Packet.Body_Len[1] + 4, &Packet,
		              END_OF_ARGS);


	// 5. Copy frame to Tx ring and prepare for encryption
//iversonadd
    RTMPToWirelessSta(pAd, pOutBuffer, FrameLen, TRUE);
	
	// 6 Free allocated memory
	kfree(pOutBuffer);
	kfree(mpool);

	DBGPRINT(RT_DEBUG_TRACE, "WpaGroupMsg1Action <-----\n");
}	

/*
	========================================================================
	
	Routine Description:
		Process pairwise message 1 of WPA2 4-way handshaking
		Then send out pairwise message 2

	Arguments:
		pAd	Pointer	to our adapter
		Elem		Message body
		
	Return Value:
		None
		
	Note:
		
	========================================================================
*/
VOID    Wpa2PairMsg1Action(
    IN  PRTMP_ADAPTER   pAd, 
    IN  MLME_QUEUE_ELEM *Elem,
	IN	INT				IfIndex) 
{
    PHEADER_802_11      pHeader;
    UCHAR               PTK[80];
    PUCHAR              pOutBuffer = NULL;
    HEADER_802_11       Header_802_11;
    UCHAR               AckRate = RATE_2;
    USHORT              AckDuration = 0;
    ULONG               FrameLen = 0;
    UCHAR               EAPHEAD[8] = {0xaa, 0xaa, 0x03, 0x00, 0x00, 0x00,0x88,0x8e};
    PEAPOL_PACKET       pMsg1;
    EAPOL_PACKET        Packet;
    UCHAR               Mic[16];
    RT_802_11_APCLI_ENTRY *pApCliEntry = &pAd->ApCliTab.ApCliEntry[IfIndex];
    
    DBGPRINT(RT_DEBUG_TRACE, "Wpa2PairMsg1Action ----->\n");
    
    pHeader = (PHEADER_802_11) Elem->Msg;
    
    // Save Data Length to pDesc for receiving packet, then put in outgoing frame   Data Len fields.
    pMsg1 = (PEAPOL_PACKET) &Elem->Msg[LENGTH_802_11 + LENGTH_802_1_H];
    
    // Process message 1 from authenticator
    // Key must be Pairwise key, already verified at callee.
    // 1. Save Replay counter, it will use to verify message 3 and construct message 2
    NdisMoveMemory(pApCliEntry->ReplayCounter, pMsg1->KeyDesc.RCounter, LEN_KEY_DESC_REPLAY);

    // 2. Save ANonce
    NdisMoveMemory(pApCliEntry->ANonce, pMsg1->KeyDesc.Nonce, LEN_KEY_DESC_NONCE);
        
    // TSNonce <--- SNonce
    // Generate random SNonce
    GenRandom(pAd, pApCliEntry->SNonce, IfIndex);  

    if (pMsg1->KeyDesc.DataLen[1] > 0 )
    {
        // cached PMKID
    }

    // TPTK <--- Calc PTK(ANonce, TSNonce)
    CountPTK(pApCliEntry->PMK,
		        pApCliEntry->ANonce,
			    pApCliEntry->ApCliBssid, 
			    pApCliEntry->SNonce, 
			    pApCliEntry->CurrentAddress, 
			    PTK, 
			    LEN_PTK);

    // Save key to PTK entry
    NdisMoveMemory(pApCliEntry->PTK, PTK, LEN_PTK);
    
    // =====================================
    // Use Priority Ring & MiniportMMRequest
    // =====================================
    pAd->Sequence = ((pAd->Sequence) + 1) & (MAX_SEQ_NUMBER);
	ApCliWpaMacHeaderInit(	pAd, 
						  	&Header_802_11, 
						   	0, 
							pApCliEntry->ApCliBssid,
							pApCliEntry->CurrentAddress);

    // ACK size is 14 include CRC, and its rate is based on real time information
    AckRate = pAd->PortCfg.ExpectedACKRate[pApCliEntry->CurrTxRate];
    AckDuration = RTMPCalcDuration(pAd, AckRate, 14);
    Header_802_11.Duration = pAd->PortCfg.Dsifs + AckDuration;
    
    // Zero message 2 body
    NdisZeroMemory(&Packet, sizeof(Packet));
    Packet.ProVer 	= EAPOL_VER;
    Packet.ProType  = EAPOLKey;
    //
    // Message 2 as  EAPOL-Key(0,1,0,0,0,P,0,SNonce,MIC,RSN IE)
    //
    Packet.KeyDesc.Type = WPA2_KEY_DESC;
    // 1. Key descriptor version and appropriate RSN IE
	Packet.KeyDesc.Data[0] = IE_WPA2;
	Packet.KeyDesc.Data[1] = pAd->ApCliTab.ApCliEntry[0].RSN_IELen;
	Packet.KeyDesc.DataLen[1] = pAd->ApCliTab.ApCliEntry[0].RSN_IELen + 2;
	NdisMoveMemory(&Packet.KeyDesc.Data[2], pAd->ApCliTab.ApCliEntry[0].RSN_IE, pAd->ApCliTab.ApCliEntry[0].RSN_IELen);


    if (pAd->ApCliTab.ApCliEntry[0].PairCipher== Ndis802_11Encryption3Enabled)
    {
        Packet.KeyDesc.Keyinfo.KeyDescVer = 2;
    }
    else    // TKIP
    {
        Packet.KeyDesc.Keyinfo.KeyDescVer = 1;
    }
    // Update packet length after decide Key data payload
    Packet.Body_Len[1]  = sizeof(KEY_DESCRIPTER) - MAX_LEN_OF_RSNIE + Packet.KeyDesc.DataLen[1];

    // 2. Key Type PeerKey
    Packet.KeyDesc.Keyinfo.KeyType = 1;

    // 3. KeyMic field presented
    Packet.KeyDesc.Keyinfo.KeyMic  = 1;
 
    // Update Key Length
    Packet.KeyDesc.KeyLength[0] = 0;
    Packet.KeyDesc.KeyLength[1] = pMsg1->KeyDesc.KeyLength[1];

    // 4. Fill SNonce
    NdisMoveMemory(Packet.KeyDesc.Nonce, pApCliEntry->SNonce, LEN_KEY_DESC_NONCE);

    // 5. Key Replay Count
    NdisMoveMemory(Packet.KeyDesc.RCounter, pApCliEntry->ReplayCounter, LEN_KEY_DESC_REPLAY);     

#ifdef BIG_ENDIAN
    // recovery original byte order, before forward Elem to another routine	   
{
    USHORT	tmpKeyinfo;

    NdisMoveMemory(&tmpKeyinfo, &Packet.KeyDesc.Keyinfo, sizeof(USHORT));
    tmpKeyinfo = SWAP16(tmpKeyinfo);
    NdisMoveMemory(&Packet.KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT));
}      
#endif

    // Out buffer for transmitting message 2
    pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);  // allocate memory
    if (pOutBuffer == NULL)
		return;	              

    // Prepare EAPOL frame for MIC calculation
    // Be careful, only EAPOL frame is counted for MIC calculation

    MakeOutgoingFrame(pOutBuffer,        &FrameLen,
                      Packet.Body_Len[1] + 4, &Packet,
                      END_OF_ARGS);

    
    // 6. Prepare and Fill MIC value
    NdisZeroMemory(Mic, sizeof(Mic));
    if (pAd->ApCliTab.ApCliEntry[0].PairCipher == Ndis802_11Encryption3Enabled)
    {
        // AES
        UCHAR digest[80];
            
        HMAC_SHA1(pOutBuffer, FrameLen, PTK, LEN_EAP_MICK, digest);
        NdisMoveMemory(Mic, digest, LEN_KEY_DESC_MIC);
    }
    else
    {
        hmac_md5(PTK,  LEN_EAP_MICK, pOutBuffer, FrameLen, Mic);
    }
    NdisMoveMemory(Packet.KeyDesc.MIC, Mic, LEN_KEY_DESC_MIC);


    FrameLen = 0;   
    // Make  Transmitting frame
    MakeOutgoingFrame(pOutBuffer,             	&FrameLen, 
                      sizeof(HEADER_802_11),  	&Header_802_11,
                      sizeof(EAPHEAD),        	EAPHEAD, 
                      Packet.Body_Len[1] + 4,   &Packet,
                      END_OF_ARGS);

    
    // Send using priority queue
    MiniportMMRequest(pAd, pOutBuffer, FrameLen);
    kfree(pOutBuffer);

    DBGPRINT(RT_DEBUG_TRACE, "Wpa2PairMsg1Action <-----\n");

}    

/*
	========================================================================
	
	Routine Description:
		Process pairwise message 3 of WPA2 4-way handshaking
		Then send out pairwise message 4

	Arguments:
		pAd	Pointer	to our adapter
		Elem		Message body
		
	Return Value:
		None
		
	Note:
		
	========================================================================
*/
#ifdef RTL865X_SOC
static inline VOID    Wpa2PairMsg3Action(
#else
VOID    Wpa2PairMsg3Action(
#endif
    IN  PRTMP_ADAPTER   pAd, 
    IN  MLME_QUEUE_ELEM *Elem,
	IN	INT				IfIndex) 
{
    PHEADER_802_11      pHeader;
    PUCHAR              pOutBuffer = NULL;
    HEADER_802_11       Header_802_11;
    UCHAR               AckRate = RATE_2;
    USHORT              AckDuration = 0;
    ULONG               FrameLen = 0;
    UCHAR               EAPHEAD[8] = {0xaa, 0xaa, 0x03, 0x00, 0x00, 0x00,0x88,0x8e};
    EAPOL_PACKET        Packet;
    PEAPOL_PACKET       pMsg3;
    UCHAR               Mic[16], OldMic[16];    
    UCHAR               KEYDATA[512], Key[32];
	RT_802_11_APCLI_ENTRY *pApCliEntry = &pAd->ApCliTab.ApCliEntry[IfIndex];

    DBGPRINT(RT_DEBUG_TRACE, "Wpa2PairMsg3Action ----->\n");
    
    pHeader = (PHEADER_802_11) Elem->Msg;
    
    // Process message 3 frame.
    pMsg3 = (PEAPOL_PACKET) &Elem->Msg[LENGTH_802_11 + LENGTH_802_1_H];

#ifdef BIG_ENDIAN
   // recovery original byte order, before forward Elem to another routine	   
{
    USHORT	tmpKeyinfo;

    NdisMoveMemory(&tmpKeyinfo, &pMsg3->KeyDesc.Keyinfo, sizeof(USHORT));
    tmpKeyinfo = SWAP16(tmpKeyinfo);
    NdisMoveMemory(&pMsg3->KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT));
}
#endif    

    // 1. Verify RSN IE & cipher type match
    if (pAd->ApCliTab.ApCliEntry[0].PairCipher == Ndis802_11Encryption3Enabled && (pMsg3->KeyDesc.Keyinfo.KeyDescVer != 2))
    {
        return;
    }
    else if (pAd->ApCliTab.ApCliEntry[0].PairCipher == Ndis802_11Encryption2Enabled && (pMsg3->KeyDesc.Keyinfo.KeyDescVer != 1))
    {
        return;
    }

#ifdef BIG_ENDIAN
   // recovery original byte order, before forward Elem to another routine	   
{
    USHORT	tmpKeyinfo;

    NdisMoveMemory(&tmpKeyinfo, &pMsg3->KeyDesc.Keyinfo, sizeof(USHORT));
    tmpKeyinfo = SWAP16(tmpKeyinfo);
    NdisMoveMemory(&pMsg3->KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT));
}
#endif

    // 2. Check MIC value
    // Save the MIC and replace with zero
    NdisMoveMemory(OldMic, pMsg3->KeyDesc.MIC, LEN_KEY_DESC_MIC);
    NdisZeroMemory(pMsg3->KeyDesc.MIC, LEN_KEY_DESC_MIC);
    if (pAd->ApCliTab.ApCliEntry[0].PairCipher == Ndis802_11Encryption3Enabled)
    {
        // AES
        UCHAR digest[80];
            
        HMAC_SHA1((PUCHAR) pMsg3, pMsg3->Body_Len[1] + 4, pApCliEntry->PTK, LEN_EAP_MICK, digest);
        NdisMoveMemory(Mic, digest, LEN_KEY_DESC_MIC);
    }
    else
    {
        hmac_md5(pApCliEntry->PTK, LEN_EAP_MICK, (PUCHAR) pMsg3, pMsg3->Body_Len[1] + 4, Mic);
    }
    
    if (!NdisEqualMemory(OldMic, Mic, LEN_KEY_DESC_MIC))
    {
        DBGPRINT(RT_DEBUG_ERROR, " MIC Different in msg 3 of 4-way handshake!!!!!!!!!! \n");
        return;
    }
    else
        DBGPRINT(RT_DEBUG_TRACE, " MIC VALID in msg 3 of 4-way handshake!!!!!!!!!! \n");

    // 3. Check Replay Counter, it has to be larger than last one. No need to be exact one larger
    if (RTMPCompareMemory(pMsg3->KeyDesc.RCounter, pApCliEntry->ReplayCounter, LEN_KEY_DESC_REPLAY) != 1)
        return;
   

    // Update new replay counter
    NdisMoveMemory(pApCliEntry->ReplayCounter, pMsg3->KeyDesc.RCounter, LEN_KEY_DESC_REPLAY);     

    // 4. Double check ANonce
    if (!NdisEqualMemory(pApCliEntry->ANonce, pMsg3->KeyDesc.Nonce, LEN_KEY_DESC_NONCE))
    {
        DBGPRINT(RT_DEBUG_ERROR, "ERROR: Double Check ANonce fails \n");
        return;
    }

    // Obtain GTK
    // 5. Decrypt GTK from Key Data
    if (pAd->ApCliTab.ApCliEntry[0].PairCipher == Ndis802_11Encryption3Enabled)
    {    
        // Decrypt AES GTK  
        AES_GTK_KEY_UNWRAP(&pApCliEntry->PTK[16], KEYDATA, pMsg3->KeyDesc.DataLen[1],pMsg3->KeyDesc.Data);       

        if (!ParseKeyData(pAd, KEYDATA, pMsg3->KeyDesc.DataLen[1], IfIndex, 1))
        	return; 
    }
    else    // TKIP
    {
        INT i;
        // Decrypt TKIP GTK
        // Construct 32 bytes RC4 Key
        NdisMoveMemory(Key, pMsg3->KeyDesc.IV, 16);
        NdisMoveMemory(&Key[16], &pApCliEntry->PTK[16], 16);
        ARCFOUR_INIT(&pAd->PrivateInfo.WEPCONTEXT, Key, 32);
        //discard first 256 bytes
        for (i = 0; i < 256; i++)
            ARCFOUR_BYTE(&pAd->PrivateInfo.WEPCONTEXT);
        // Decrypt GTK. Becareful, there is no ICV to check the result is correct or not
        ARCFOUR_DECRYPT(&pAd->PrivateInfo.WEPCONTEXT, KEYDATA, pMsg3->KeyDesc.Data, pMsg3->KeyDesc.DataLen[1]);       

        if (!ParseKeyData(pAd, KEYDATA, pMsg3->KeyDesc.DataLen[1], IfIndex, 1))
        	return;
    }
    
    // 6. Construct Message 4
    // =====================================
    // Use Priority Ring & MiniportMMRequest
    // =====================================
    pAd->Sequence = ((pAd->Sequence) + 1) & (MAX_SEQ_NUMBER);
	ApCliWpaMacHeaderInit(	pAd, 
						  	&Header_802_11, 
						   	0, 
							pApCliEntry->ApCliBssid,
							pApCliEntry->CurrentAddress);


    // ACK size is 14 include CRC, and its rate is based on real time information
    AckRate = pAd->PortCfg.ExpectedACKRate[pApCliEntry->CurrTxRate];
    AckDuration = RTMPCalcDuration(pAd, AckRate, 14);
    Header_802_11.Duration = pAd->PortCfg.Dsifs + AckDuration;
    
    // Zero message 4 body
    NdisZeroMemory(&Packet, sizeof(Packet));
    Packet.ProVer 	= EAPOL_VER;
    Packet.ProType  = EAPOLKey;
    Packet.Body_Len[1]  = sizeof(KEY_DESCRIPTER) - MAX_LEN_OF_RSNIE;     // No data field
    
    //
    // Message 4 as  EAPOL-Key(0,1,0,0,0,P,0,0,MIC,0)
    //
    Packet.KeyDesc.Type = WPA1_KEY_DESC;	// ?? // marked by AlbertY @ 20060701 
    
#ifdef BIG_ENDIAN
   // recovery original byte order, before forward Elem to another routine	   
{
    USHORT	tmpKeyinfo;

    NdisMoveMemory(&tmpKeyinfo, &pMsg3->KeyDesc.Keyinfo, sizeof(USHORT));
    tmpKeyinfo = SWAP16(tmpKeyinfo);
    NdisMoveMemory(&pMsg3->KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT));
}
#endif

    // Key descriptor version and appropriate RSN IE
    Packet.KeyDesc.Keyinfo.KeyDescVer = pMsg3->KeyDesc.Keyinfo.KeyDescVer;

    // Update Key Length
	Packet.KeyDesc.KeyLength[0] = pMsg3->KeyDesc.KeyLength[0];
	Packet.KeyDesc.KeyLength[1] = pMsg3->KeyDesc.KeyLength[1];
	
    // Key Type PeerKey
    Packet.KeyDesc.Keyinfo.KeyType = 1;

    // KeyMic field presented
    Packet.KeyDesc.Keyinfo.KeyMic  = 1;
    Packet.KeyDesc.Keyinfo.Secure = 1;

    // Key Replay count 
    NdisMoveMemory(Packet.KeyDesc.RCounter, pMsg3->KeyDesc.RCounter, LEN_KEY_DESC_REPLAY);        

#ifdef BIG_ENDIAN
   // recovery original byte order, before forward Elem to another routine	   
{
    USHORT	tmpKeyinfo;

    NdisMoveMemory(&tmpKeyinfo, &Packet.KeyDesc.Keyinfo, sizeof(USHORT));
    tmpKeyinfo = SWAP16(tmpKeyinfo);
    NdisMoveMemory(&Packet.KeyDesc.Keyinfo, &tmpKeyinfo, sizeof(USHORT));
}
#endif

    // Out buffer for transmitting message 4
    pOutBuffer = kmalloc(MAX_LEN_OF_MLME_BUFFER, MEM_ALLOC_FLAG);  // allocate memory
    if (pOutBuffer == NULL)
		return;	
               
    // Prepare EAPOL frame for MIC calculation
    // Be careful, only EAPOL frame is counted for MIC calculation
    MakeOutgoingFrame(pOutBuffer,           &FrameLen,
                      Packet.Body_Len[1] + 4,    &Packet,
                      END_OF_ARGS);

    // Prepare and Fill MIC value
    NdisZeroMemory(Mic, sizeof(Mic));
    if (pAd->ApCliTab.ApCliEntry[0].PairCipher == Ndis802_11Encryption3Enabled)
    {
        // AES
        UCHAR digest[80];
            
        HMAC_SHA1(pOutBuffer, FrameLen, pApCliEntry->PTK, LEN_EAP_MICK, digest);
        NdisMoveMemory(Mic, digest, LEN_KEY_DESC_MIC);
    }
    else
    {
        hmac_md5(pApCliEntry->PTK, LEN_EAP_MICK, pOutBuffer, FrameLen, Mic);
    }
    NdisMoveMemory(Packet.KeyDesc.MIC, Mic, LEN_KEY_DESC_MIC);

    FrameLen = 0;
    
    // Make  Transmitting frame
    MakeOutgoingFrame(pOutBuffer,               &FrameLen, 
                      sizeof(HEADER_802_11),    &Header_802_11,
                      sizeof(EAPHEAD),          EAPHEAD, 
                      Packet.Body_Len[1] + 4,   &Packet,
                      END_OF_ARGS);


    // 7. Update PTK
	// Prepare Pairwise key - Key material
	NdisZeroMemory(&pApCliEntry->PairwiseKey, sizeof(CIPHER_KEY));   
	pApCliEntry->PairwiseKey.KeyLen = LEN_TKIP_EK;
    NdisMoveMemory(pApCliEntry->PairwiseKey.Key, &pApCliEntry->PTK[32], LEN_TKIP_EK);
    NdisMoveMemory(pApCliEntry->PairwiseKey.RxTsc, pMsg3->KeyDesc.RSC, 6);
    
    // Prepare Pairwise key - Select RxMic and TxMic based on supplicant
    NdisMoveMemory(pApCliEntry->PairwiseKey.RxMic, &pApCliEntry->PTK[48], LEN_TKIP_RXMICK);
    NdisMoveMemory(pApCliEntry->PairwiseKey.TxMic, &pApCliEntry->PTK[48+LEN_TKIP_RXMICK], LEN_TKIP_TXMICK);
	
	// Prepare Pairwise key - CipherAlg
	pApCliEntry->PairwiseKey.CipherAlg = CIPHER_NONE;
	if (pAd->ApCliTab.ApCliEntry[0].PairCipher == Ndis802_11Encryption2Enabled)
		pApCliEntry->PairwiseKey.CipherAlg = CIPHER_TKIP;
	else if (pAd->ApCliTab.ApCliEntry[0].PairCipher == Ndis802_11Encryption3Enabled)
		pApCliEntry->PairwiseKey.CipherAlg = CIPHER_AES;
		
	// 802.1x port control
	pApCliEntry->PortSecured = WPA_802_1X_PORT_SECURED;	
		
	// Set pairwise key to Asic
    AsicAddPairwiseKeyEntry(
            pAd, 
            pApCliEntry->ApCliBssid, 
            (APCLI_PAIRWISE_KEY_OFFSET + IfIndex),
            pApCliEntry->PairwiseKey.CipherAlg, 
            pApCliEntry->PairwiseKey.Key,
            pApCliEntry->PairwiseKey.TxMic, 
            pApCliEntry->PairwiseKey.RxMic);
    
    // 6. Send Message 4 to authenticator
    // Send using priority queue
    MiniportMMRequest(pAd, pOutBuffer, FrameLen);
    kfree(pOutBuffer);

    DBGPRINT(RT_DEBUG_ERROR, "Wpa2PairMsg3Action <-----\n");

}   

/*
    ========================================================================
    
    Routine Description:
    Parse KEYDATA field.  KEYDATA[] May contain 2 RSN IE and optionally GTK.  
    GTK is encaptulated in KDE format at p.83 802.11i D10
	
    Arguments:
        
    Return Value:

    Note:
        802.11i D10  
        
    ========================================================================
*/
#ifdef RTL865X_SOC
static inline BOOLEAN ParseKeyData(
#else
BOOLEAN ParseKeyData(
#endif
    IN  PRTMP_ADAPTER   pAd,
    IN  PUCHAR          pKeyData,
    IN  UCHAR           KeyDataLen,
	IN	INT				IfIndex,
	IN	UCHAR			bPairewise)
{
    PKDE_ENCAP          pKDE = NULL;
    PUCHAR              pMyKeyData = pKeyData;
    UCHAR               KeyDataLength = KeyDataLen;
    UCHAR               GTKLEN;
    INT                 i, RSNIE_LEN;
    RT_802_11_APCLI_ENTRY *pApCliEntry = &pAd->ApCliTab.ApCliEntry[IfIndex];

	DBGPRINT(RT_DEBUG_TRACE, "Before ParseKeyData, the Total KeyDataLen = %d  \n", KeyDataLen);
	// Verify The RSN IE contained in Pairewise-Msg 3 and skip it
	if (bPairewise)
	{
		// Check RSN IE whether it is WPA2/WPA2PSK   
    	if (!NdisEqualMemory(pKeyData, pApCliEntry->RSN_IE_from_AP, pApCliEntry->RSN_IELen_from_AP))
    	{	
        	DBGPRINT(RT_DEBUG_ERROR, " WPA2/WPA2PSK RSN IE mismatched - Element ID(%d), Length(%d)!!!!!!!!!! \n", *pKeyData, *(pKeyData + 1));
 #if 1	//KeyMaterialDump
		{
			printk("Get RSN IE :\n");
			for (i = 0; i < *(pKeyData + 1); i++)
				printk("%02x ", *(pKeyData + 2 + i));
			printk("\n");	
		}
#endif       	
        	
        	return FALSE;
    	}
    	else
    	{
			// skip RSN IE
			RSNIE_LEN = *(pKeyData + 1);
			pMyKeyData += (2 + RSNIE_LEN);
			KeyDataLength -= (2 + RSNIE_LEN);

			DBGPRINT(RT_DEBUG_TRACE, " WPA2/WPA2PSK RSN IE matched in Msg 3, Length(%d) \n", (RSNIE_LEN + 2));
		}
	}    
	
    DBGPRINT(RT_DEBUG_TRACE,"KeyDataLength %d without RSN_IE \n", KeyDataLength);
    
    // Parse EKD format
    if (KeyDataLength >= 8)
    {
        pKDE = (PKDE_ENCAP) pMyKeyData;
        DBGPRINT(RT_DEBUG_INFO,"pKDE->Type %x \n", pKDE->Type);
        DBGPRINT(RT_DEBUG_INFO,"pKDE->Len 0x%x \n", pKDE->Len);
        DBGPRINT(RT_DEBUG_INFO,"pKDE->OUI %x %x %x \n", pKDE->OUI[0],pKDE->OUI[1],pKDE->OUI[2] );
    	DBGPRINT(RT_DEBUG_INFO,"pKDE->DataType %x \n", pKDE->DataType);
    }   

    if (pKDE->GTKEncap.Kid == 0)
    {
        DBGPRINT(RT_DEBUG_ERROR,"ERROR: GTK Key index zero \n");
        return FALSE;
    }

	if (KeyDataLength < (pKDE->Len + 2))
	{
        DBGPRINT(RT_DEBUG_ERROR,"ERROR: The len from KDE is too short \n");
        return FALSE;			
	}	
	
	GTKLEN = pKDE->Len - 6;

	DBGPRINT(RT_DEBUG_TRACE, "GTK Key with KDE formet got index=%d, len=%d \n", pKDE->GTKEncap.Kid, GTKLEN);

	if (GTKLEN < LEN_AES_KEY)
	{
		DBGPRINT(RT_DEBUG_ERROR, "ERROR: GTK Key length is too short (%d) \n", GTKLEN);
        return FALSE;
	}	
	
	// set key material, TxMic and RxMic if necessary
	NdisZeroMemory(pApCliEntry->SharedKey, SHARE_KEY_NUM * sizeof(CIPHER_KEY));    
	NdisMoveMemory(pApCliEntry->GTK, pKDE->GTKEncap.GTK, 32);
	pApCliEntry->DefaultKeyId = pKDE->GTKEncap.Kid; 
	NdisMoveMemory(pApCliEntry->SharedKey[pApCliEntry->DefaultKeyId].Key, pKDE->GTKEncap.GTK, LEN_TKIP_EK);
	NdisMoveMemory(pApCliEntry->SharedKey[pApCliEntry->DefaultKeyId].RxMic, &pKDE->GTKEncap.GTK[16], LEN_TKIP_RXMICK);
	NdisMoveMemory(pApCliEntry->SharedKey[pApCliEntry->DefaultKeyId].TxMic, &pKDE->GTKEncap.GTK[24], LEN_TKIP_TXMICK);
	pApCliEntry->SharedKey[pApCliEntry->DefaultKeyId].KeyLen = LEN_TKIP_EK;

#if 1	//KeyMaterialDump
	{
		printk("Set Group Key, KeyIndex(%d) for IF(apcli%d) ", pApCliEntry->DefaultKeyId, IfIndex);
		for (i = 0; i < LEN_TKIP_EK; i++)
			printk("%02x ", pApCliEntry->SharedKey[pApCliEntry->DefaultKeyId].Key[i]);
		printk("\n");	
	}
#endif

	// Update CipherAlg
	pApCliEntry->SharedKey[pApCliEntry->DefaultKeyId].CipherAlg = CIPHER_NONE;
	if (pApCliEntry->GroupCipher == Ndis802_11Encryption2Enabled)
		pApCliEntry->SharedKey[pApCliEntry->DefaultKeyId].CipherAlg = CIPHER_TKIP;
	else if (pApCliEntry->GroupCipher == Ndis802_11Encryption3Enabled)
		pApCliEntry->SharedKey[pApCliEntry->DefaultKeyId].CipherAlg = CIPHER_AES;

 	return TRUE;
}  

/*
	========================================================================
	
	Routine Description:
		Init WPA MAC header

	Arguments:
		pAd	Pointer	to our adapter
		
	Return Value:
		None
		
	Note:
		
	========================================================================
*/
#ifdef RTL865X_SOC
static inline VOID	ApCliWpaMacHeaderInit(
#else
VOID	ApCliWpaMacHeaderInit(
#endif
	IN		PRTMP_ADAPTER	pAd, 
	IN OUT	PHEADER_802_11	pHdr80211, 
	IN		UCHAR			wep, 
	IN		PUCHAR		    pAddr1,
	IN		PUCHAR		    pAddr2)	
{
	NdisZeroMemory(pHdr80211, sizeof(HEADER_802_11));
	pHdr80211->FC.Type	= BTYPE_DATA;	
	pHdr80211->FC.ToDs	= 1;
	if (wep	== 1)
		pHdr80211->FC.Wep = 1;
	
	 //	Addr1: BSSID, Addr2: SA, Addr3:	DA
	COPY_MAC_ADDR(pHdr80211->Addr1, pAddr1);
	COPY_MAC_ADDR(pHdr80211->Addr2, pAddr2);
	COPY_MAC_ADDR(pHdr80211->Addr3, pAddr1);
	pHdr80211->Sequence =	pAd->Sequence;
}

/*
	========================================================================
	
	Routine Description:
		Validate the security configuration against the RSN information 
		element

	Arguments:
		pAdapter	Pointer	to our adapter
		eid_ptr 	Pointer to VIE
		
	Return Value:
		TRUE 	for configuration match 
		FALSE	for otherwise
		
	Note:
		
	========================================================================
*/
BOOLEAN 	ApCliValidateRSNIE(
	IN		PRTMP_ADAPTER	pAdapter, 
	IN 		PEID_STRUCT     eid_ptr)
{
	PUCHAR 	pTmp;
	USHORT	Count;
	PRT_802_11_APCLI_ENTRY pApCliEntry;

	pApCliEntry = &pAdapter->ApCliTab.ApCliEntry[0];

	if (eid_ptr->Len <= MAX_LEN_OF_RSNIE && eid_ptr->Len > MIN_LEN_OF_RSNIE)
	{
		if (pApCliEntry->RSN_IE[0] == 0x00 && pApCliEntry->RSN_IE[1] == 0x50 &&
			pApCliEntry->RSN_IE[2] == 0xF2 && pApCliEntry->RSN_IE[3] == 0x01)
		{	// If ApCli RSNIE begins with 00 50 F2 01, ApCli RSNIE uses WPA1
			if (eid_ptr->Eid == IE_WPA2)
			{
				DBGPRINT(RT_DEBUG_ERROR, "ApCliValidateRSNIE - beacon is WPA2 while ApCli is WPA1\n");
	            return FALSE;
			}
		}
		else
		{	// else, ApCli RSNIE uses WPA2
			if (eid_ptr->Eid == IE_WPA)
			{
				DBGPRINT(RT_DEBUG_ERROR, "ApCliValidateRSNIE - beacon is WPA1 while ApCli is WPA2\n");
	            return FALSE;
			}
		}
		
		// We don't have to check invalidity of multicast cipher selection in beacon RSN IE.
		// We just use the multicast cipher selection in beacon RSN IE.
		if (eid_ptr->Len >= 6)
    	{
    		// WPA and WPA2 format not the same in RSN_IE
    		if (eid_ptr->Eid == IE_WPA)
	        	NdisMoveMemory(&pApCliEntry->RSN_IE[6], &eid_ptr->Octet[6], 4);
    		else if (eid_ptr->Eid == IE_WPA2)
    			NdisMoveMemory(&pApCliEntry->RSN_IE[2], &eid_ptr->Octet[2], 4);
    	}

		// Check invalidity of unicast cipher selection in RSN IE
		if (eid_ptr->Len >= 16)
		{
			pTmp = &eid_ptr->Octet[0];
			if(eid_ptr->Eid == IE_WPA2)
			{
				// skip Version(2),Multicast cipter(4) 2+4==6
				// point to number of unicast
        		pTmp +=6;
			}
			else if (eid_ptr->Eid == IE_WPA)	
			{
				// skip OUI(4),Vesrion(2),Multicast cipher(4) 4+2+4==10
				// point to number of unicast
        		pTmp += 10;
			}
			else
			{
				DBGPRINT(RT_DEBUG_ERROR, "ApCliValidateRSNIE ==> Unknown WPAIE, WPAIE=%d\n", eid_ptr->Eid);
	    		return FALSE;
			}

			// Store unicast cipher count
    		NdisMoveMemory(&Count, pTmp, sizeof(USHORT));
#ifdef BIG_ENDIAN
    		Count = SWAP16(Count);		
#endif
			// pointer to unicast cipher
    		pTmp += sizeof(USHORT);

    		if (eid_ptr->Eid == IE_WPA)
    		{
    			while (Count > 0)
    			{
    				if (RTMPEqualMemory(pTmp /*&eid_ptr->Octet[12]*/, &pApCliEntry->RSN_IE[12], 4))
		           		break;
						
					pTmp += 4;
					if ((--Count) == 0)
					{
	            		DBGPRINT(RT_DEBUG_ERROR, "ApCliValidateRSNIE - WPA1 unicast cipher is not match\n");
	            		return FALSE;
    				}
				}
    				
    		}
    		else if (eid_ptr->Eid == IE_WPA2)
    		{	
				while (Count > 0)
    			{
					if (RTMPEqualMemory(pTmp /*&eid_ptr->Octet[8]*/, &pApCliEntry->RSN_IE[8], 4))
						break;
					
					pTmp += 4;
					if ((--Count) == 0)
					{
	            		DBGPRINT(RT_DEBUG_ERROR, "ApCliValidateRSNIE - WPA2 unicast cipher is not match\n");
	            		return FALSE;
    				}
				}
    		}
    	}
		
		// Check invalidity of authentication method selection in RSN IE
		// Store AKM count
		NdisMoveMemory(&Count, pTmp, sizeof(USHORT));
#ifdef BIG_ENDIAN
    	Count = SWAP16(Count);		
#endif

		//pointer to AKM cipher
    	pTmp += sizeof(USHORT);

		if (eid_ptr->Len >= 16)
    	{
    		if (eid_ptr->Eid == IE_WPA)
    		{
    			while (Count > 0)
				{
					if (RTMPCheckAKM(pTmp /*&eid_ptr->Octet[18]*/,&pApCliEntry->RSN_IE[0],0))
    					break;
					
					pTmp += 4;
					if ((--Count) == 0)
					{
	            		DBGPRINT(RT_DEBUG_ERROR, "ApCliValidateRSNIE - WPA1 authentication method is not match \n");
	            		return FALSE;
    				}
				}
    		}
    		else if (eid_ptr->Eid == IE_WPA2)
    		{
				while (Count > 0)
				{			
					if (RTMPCheckAKM(pTmp /*&eid_ptr->Octet[14]*/,&pApCliEntry->RSN_IE[0],1))
						break;
					
					pTmp += 4;
					if ((--Count) == 0)
					{
	            		DBGPRINT(RT_DEBUG_ERROR, "ApCliValidateRSNIE - WPA2 authentication method is not match \n");
	            		return FALSE;
    				}
				}
    		}
    	}
	}
	else
	{
		DBGPRINT(RT_DEBUG_ERROR, "ApCliValidateRSNIE - length is wrong \n");
		return FALSE;
	}	
	
	return TRUE;
}	

#endif
