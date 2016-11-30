/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	sanity.c

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	John Chang  2004-09-01      add WMM support
*/
#include "rt_config.h"
#include <net/iw_handler.h>

typedef struct wsc_ie_probreq_data
{
	UCHAR	ssid[32];
	UCHAR	macAddr[6];
	UCHAR	data[2];
} WSC_IE_PROBREQ_DATA;

UCHAR	WPA_OUI[] = {0x00, 0x50, 0xf2, 0x01};
UCHAR   RSN_OUI[] = {0x00, 0x0f, 0xac};
UCHAR   WME_INFO_ELEM[]  = {0x00, 0x50, 0xf2, 0x02, 0x00, 0x01};
UCHAR   WME_PARM_ELEM[] = {0x00, 0x50, 0xf2, 0x02, 0x01, 0x01};
UCHAR	Ccx2QosInfo[] = {0x00, 0x40, 0x96, 0x04};
UCHAR   RALINK_OUI[]  = {0x00, 0x0c, 0x43};
UCHAR   RALINK_GET_CONFIG_OUI[] = {0x00, 0x0c, 0x43, 0x80};

#ifdef WSC_SUPPORT
UCHAR   WPS_OUI[] = {0x00, 0x50, 0xf2, 0x04};
#endif // WSC_SUPPORT //

/* 
    ==========================================================================
    Description:
        MLME message sanity check
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
 */
BOOLEAN MlmeScanReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT UCHAR *pBssType, 
    OUT CHAR Ssid[], 
    OUT UCHAR *pSsidLen, 
    OUT UCHAR *pScanType) 
{
    MLME_SCAN_REQ_STRUCT *Info;

    Info = (MLME_SCAN_REQ_STRUCT *)(Msg);
    *pBssType = Info->BssType;
    *pSsidLen = Info->SsidLen;
    NdisMoveMemory(Ssid, Info->Ssid, *pSsidLen);
    *pScanType = Info->ScanType;
    
    if ((*pBssType == BSS_INFRA || *pBssType == BSS_ADHOC || *pBssType == BSS_ANY) &&
       (*pScanType == SCAN_ACTIVE || *pScanType == SCAN_PASSIVE)) 
        return TRUE;
    else 
    {
        DBGPRINT(RT_DEBUG_TRACE, "MlmeScanReqSanity fail - wrong BssType or ScanType\n");
        return FALSE;
    }
}

/* 
    ==========================================================================
    Description:
        MLME message sanity check
    Return:
        TRUE if all parameters are OK, FALSE otherwise
        
    ==========================================================================
 */
BOOLEAN PeerAuthSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT PUCHAR pAddr1,
    OUT PUCHAR pAddr2, 
    OUT USHORT *pAlg, 
    OUT USHORT *pSeq, 
    OUT USHORT *pStatus, 
    CHAR *pChlgText) 
{
    PFRAME_802_11 pFrame = (PFRAME_802_11)Msg;

	COPY_MAC_ADDR(pAddr1,   pFrame->Hdr.Addr1);
    COPY_MAC_ADDR(pAddr2,   pFrame->Hdr.Addr2);
    NdisMoveMemory(pAlg,    &pFrame->Octet[0], 2);
    NdisMoveMemory(pSeq,    &pFrame->Octet[2], 2);
    NdisMoveMemory(pStatus, &pFrame->Octet[4], 2);

    if (*pAlg == Ndis802_11AuthModeOpen)
    {
        if (*pSeq == 1 || *pSeq == 2) 
        {
            return TRUE;
        } 
        else 
        {
            DBGPRINT(RT_DEBUG_TRACE, "PeerAuthSanity fail - wrong Seg#\n");
            return FALSE;
        }
    } 
    else if (*pAlg == Ndis802_11AuthModeShared) 
    {
        if (*pSeq == 1 || *pSeq == 4) 
        {
            return TRUE;
        } 
        else if (*pSeq == 2 || *pSeq == 3) 
        {
            NdisMoveMemory(pChlgText, &pFrame->Octet[8], CIPHER_TEXT_LEN);
            return TRUE;
        } 
        else 
        {
            DBGPRINT(RT_DEBUG_TRACE, "PeerAuthSanity fail - wrong Seg#\n");
            return FALSE;
        }
    } 
    else 
    {
        DBGPRINT(RT_DEBUG_TRACE, "PeerAuthSanity fail - wrong algorithm\n");
        return FALSE;
    }
}

/* 
    ==========================================================================
    Description:
        MLME message sanity check
    Return:
        TRUE if all parameters are OK, FALSE otherwise
        
    ==========================================================================
 */
BOOLEAN PeerProbeReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT PUCHAR pAddr2,
    OUT CHAR Ssid[], 
    OUT UCHAR *pSsidLen) 
{
	PFRAME_802_11 pFrame = (PFRAME_802_11)Msg;
    CHAR                *Ptr;
    PEID_STRUCT         eid_ptr;
#ifdef WSC_SUPPORT
    UCHAR         apidx = MAIN_MBSSID;
#endif // WSC_SUPPORT //

    // to prevent caller from using garbage output value
    *pSsidLen = 0;

    COPY_MAC_ADDR(pAddr2, pFrame->Hdr.Addr2);

    if (pFrame->Octet[0] != IE_SSID || pFrame->Octet[1] > MAX_LEN_OF_SSID) 
    {
        DBGPRINT(RT_DEBUG_TRACE, "PeerProbeReqSanity fail - wrong SSID IE\n");
        return FALSE;
    } 
    
    *pSsidLen = pFrame->Octet[1];
    NdisMoveMemory(Ssid, &pFrame->Octet[2], *pSsidLen);

   	Ptr = pFrame->Octet;
    eid_ptr = (PEID_STRUCT) Ptr;

    // get variable fields from payload and advance the pointer
	while(((UCHAR*)eid_ptr + eid_ptr->Len + 1) < ((UCHAR*)pFrame + MsgLen))
    {
        switch(eid_ptr->Eid)
        {
	         	case IE_VENDOR_SPECIFIC:
                if (NdisEqualMemory(eid_ptr->Octet, RALINK_GET_CONFIG_OUI, 4) && (eid_ptr->Len == 4)) 
                {
                    pAd->PortCfg.bGetAPConfig = TRUE;
                    break;
                }
#ifdef WSC_SUPPORT
                else if (be2cpu32(*(ULONG *)&eid_ptr->Octet) == WSC_OUI)
                {
					if (pAd->PortCfg.MBSSID[apidx].WscControl.WscConfMode == WSC_AP_PROXY)
					{	
						int bufLen = 0;
						PUCHAR pBuf = NULL;
						WSC_IE_PROBREQ_DATA	*pprobreq = NULL;

						bufLen = sizeof(WSC_IE_PROBREQ_DATA) + eid_ptr->Len;
						pBuf = kmalloc(bufLen, MEM_ALLOC_FLAG);
						if(pBuf == NULL)
							break;

                		//Send WSC probe req to UPnP
	                	memset(pBuf, 0 ,  bufLen);
    	                pprobreq = (WSC_IE_PROBREQ_DATA*)pBuf;
						if (32 >= *pSsidLen)	//Well, I think that it must be TRUE!
						{
							NdisMoveMemory(pprobreq->ssid, Ssid, *pSsidLen);			// SSID
							NdisMoveMemory(pprobreq->macAddr, pFrame->Hdr.Addr2, 6);	// Mac address
	                        pprobreq->data[0] = 221; 									// element ID
    	                    pprobreq->data[1] = eid_ptr->Len;							// element Length
							NdisMoveMemory((pBuf+sizeof(WSC_IE_PROBREQ_DATA)), eid_ptr->Octet, eid_ptr->Len);	// (WscProbeReqData)
							WscSendUPnPMessage(pAd, WSC_OPCODE_UPNP_MGMT, WSC_UPNP_MGMT_SUB_PROBE_REQ, pBuf, bufLen, 0, 0);
	            		}
	            		break;
					}
	                break;
                }
#endif // WSC_SUPPORT //
                
            default:
                break;
        }
        eid_ptr = (PEID_STRUCT)((UCHAR*)eid_ptr + 2 + eid_ptr->Len);        
	}

        return TRUE;
}

UCHAR ChannelSanity(
    IN PRTMP_ADAPTER pAd, 
    IN UCHAR channel)
{
    INT i;

    for (i = 0; i < pAd->ChannelListNum; i ++)
    {
        if (channel == pAd->ChannelList[i].Channel)
            return 1;
    }
    return 0;
}

/* 
    ==========================================================================
    Description:
        MLME message sanity check
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
 */
BOOLEAN PeerAssocReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen,
    OUT PUCHAR pAddr1,
    OUT PUCHAR pAddr2,
    OUT USHORT *pCapabilityInfo, 
    OUT USHORT *pListenInterval, 
    OUT UCHAR *pSsidLen,
    OUT char *Ssid,
    OUT UCHAR *pRatesLen,
    OUT UCHAR Rates[],
    OUT UCHAR *RSN,
    OUT UCHAR *pRSNLen,
    OUT BOOLEAN *pbWmmCapable,
#ifdef WSC_SUPPORT
    OUT BOOLEAN *pWscCapable,
#endif // WSC_SUPPORT //
    OUT ULONG  *pRalinkIe) 
{
    CHAR            *Ptr;
    PFRAME_802_11        Fr = (PFRAME_802_11)Msg;
    PEID_STRUCT     eid_ptr;
    UCHAR           Sanity = 0;
    UCHAR           WPA[4] = {0x00,0x50,0xf2,0x01};
    MAC_TABLE_ENTRY *pEntry = (MAC_TABLE_ENTRY*)NULL;

    // to prevent caller from using garbage output value
    *pRatesLen	= 0;
    *pRSNLen	= 0;
    *pbWmmCapable	= FALSE;
    *pRalinkIe	= 0;

    COPY_MAC_ADDR(pAddr1, Fr->Hdr.Addr1);
    COPY_MAC_ADDR(pAddr2, Fr->Hdr.Addr2);
    Ptr = Fr->Octet;

    NdisMoveMemory(pCapabilityInfo, &Fr->Octet[0], 2);
    NdisMoveMemory(pListenInterval, &Fr->Octet[2], 2);

    eid_ptr = (PEID_STRUCT) &Fr->Octet[4];

    pEntry = MacTableLookup(pAd, pAddr2);

    // get variable fields from payload and advance the pointer
    while (((UCHAR*)eid_ptr + eid_ptr->Len + 1) < ((UCHAR*)Fr + MsgLen))
    {
        switch(eid_ptr->Eid)
        {
            case IE_SSID:
                if(eid_ptr->Len <= MAX_LEN_OF_SSID)
                {
                    Sanity |= 0x01;
                    NdisMoveMemory(Ssid, eid_ptr->Octet, eid_ptr->Len);
                    *pSsidLen = eid_ptr->Len;
                }
                else
                {
                    DBGPRINT(RT_DEBUG_TRACE, "PeerAssocReqSanity - wrong IE_SSID\n");
                    return FALSE;
                }
                break;
                
            case IE_SUPP_RATES:
                if ((eid_ptr->Len <= MAX_LEN_OF_SUPPORTED_RATES) && (eid_ptr->Len > 0))
                {
                    Sanity |= 0x02;
                    NdisMoveMemory(Rates, eid_ptr->Octet, eid_ptr->Len);
                    *pRatesLen = eid_ptr->Len;
                }
                else
                {
                    DBGPRINT(RT_DEBUG_TRACE, "PeerAssocReqSanity - wrong IE_SUPP_RATES\n");
                    return FALSE;
                }
                break;
                
            case IE_EXT_SUPP_RATES:
                if (eid_ptr->Len + *pRatesLen <= MAX_LEN_OF_SUPPORTED_RATES)
                {
                    NdisMoveMemory(&Rates[*pRatesLen], eid_ptr->Octet, eid_ptr->Len);
                    *pRatesLen = (*pRatesLen) + eid_ptr->Len;
                }
                else
                {
                    NdisMoveMemory(&Rates[*pRatesLen], eid_ptr->Octet, MAX_LEN_OF_SUPPORTED_RATES - (*pRatesLen));
                    *pRatesLen = MAX_LEN_OF_SUPPORTED_RATES;
                }
                break;

            case IE_WPA:
           	case IE_WPA2:
#ifdef WSC_SUPPORT                
                if (NdisEqualMemory(eid_ptr->Octet, WPS_OUI, 4))
                    *pWscCapable = TRUE;
#endif // WSC_SUPPORT //
           		if (NdisEqualMemory(eid_ptr->Octet, RALINK_OUI, 3) && (eid_ptr->Len == 7))
                {
                    *pRalinkIe = eid_ptr->Octet[3];
                    break;
                }
           		
           		// WMM_IE
                if (NdisEqualMemory(eid_ptr->Octet, WME_INFO_ELEM, 6) && (eid_ptr->Len == 7))
                {
                    *pbWmmCapable = TRUE;

                	if ((pEntry) && (pAd->PortCfg.bAPSDCapable))
                    {
                    	QBSS_STA_INFO_PARM	QosInfo;
                    	
	                	QosInfo = *(PQBSS_STA_INFO_PARM) &eid_ptr->Octet[6];
	                	pEntry->MaxSPLength = QosInfo.MaxSPLength;
	                	pEntry->bAPSDCapablePerAC[QID_AC_BE] = QosInfo.UAPSD_AC_BE;
	                	pEntry->bAPSDCapablePerAC[QID_AC_BK] = QosInfo.UAPSD_AC_BK;
	                	pEntry->bAPSDCapablePerAC[QID_AC_VI] = QosInfo.UAPSD_AC_VI;
	                	pEntry->bAPSDCapablePerAC[QID_AC_VO] = QosInfo.UAPSD_AC_VO;
	                	if ((pEntry->bAPSDCapablePerAC[QID_AC_BE] == 0) && (pEntry->bAPSDCapablePerAC[QID_AC_BK] == 0) && (pEntry->bAPSDCapablePerAC[QID_AC_VI] == 0) && (pEntry->bAPSDCapablePerAC[QID_AC_VO] == 0))
	                	{
	                		CLIENT_STATUS_CLEAR_FLAG(pEntry, fCLIENT_STATUS_APSD_CAPABLE);
	                	}
	                	else
	                	{
	                		CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_APSD_CAPABLE);
	                	}
                    }

                    break;
                }
           		
                if ((!pEntry) || (pAd->PortCfg.MBSSID[pEntry->ApIdx].AuthMode < Ndis802_11AuthModeWPA))
                    break;
                
                // If this IE did not begins with 00:0x50:0xf2:0x01,  it would be proprietary.  So we ignore
                if (eid_ptr->Eid == IE_WPA)
                {
	                if (!RTMPEqualMemory(eid_ptr->Octet, WPA, 4))
	                    break;
                }
                if (eid_ptr->Len <= MAX_LEN_OF_RSNIE && eid_ptr->Len > MIN_LEN_OF_RSNIE)
                {
                    *pRSNLen=eid_ptr->Len;
                    if (!RTMPCheckMcast(pAd, eid_ptr, pEntry))
                    {
                        DBGPRINT(RT_DEBUG_TRACE, " RTMPCheckMcast  FAILED !!!!  \n");

                        if (pEntry)
                            DisAssocAction( pAd, pEntry, SUBTYPE_DISASSOC, REASON_MCIPHER_NOT_VALID);
                        return FALSE;
                    }                        
                    if (!RTMPCheckUcast(pAd, eid_ptr, pEntry))
                    {
                        DBGPRINT(RT_DEBUG_TRACE, " RTMPCheckUcast  FAILED !!!!  \n");

                        if (pEntry)
                            DisAssocAction( pAd, pEntry, SUBTYPE_DISASSOC, REASON_UCIPHER_NOT_VALID);
                        return FALSE;
                    }                        
                    if (!RTMPCheckAUTH(pAd, eid_ptr, pEntry))
                    {
                        DBGPRINT(RT_DEBUG_TRACE, " RTMPCheckAUTH Method   FAILED !!!!  \n");

                        if (pEntry)
                            DisAssocAction( pAd, pEntry, SUBTYPE_DISASSOC, REASON_INVALID_IE);
                        return FALSE;
                    }

                    NdisMoveMemory(RSN, eid_ptr->Octet, eid_ptr->Len);
                    DBGPRINT(RT_DEBUG_INFO, "Receive IE_WPA : %x %x %x %x %x %x %x %x \n",\
                        eid_ptr->Octet[0],eid_ptr->Octet[1],eid_ptr->Octet[2],eid_ptr->Octet[3],eid_ptr->Octet[4],\
                        eid_ptr->Octet[5],eid_ptr->Octet[6],eid_ptr->Octet[7]);
                }
                else
                {
                    *pRSNLen=0;
                    DBGPRINT(RT_DEBUG_TRACE, "PeerAssocReqSanity - missing IE_WPA)\n");
                    return FALSE;
                }               
                break;

            default:
                break;
        }
        eid_ptr = (PEID_STRUCT)((UCHAR*)eid_ptr + 2 + eid_ptr->Len);        
    }

    if (Sanity != 0x03)    
    {
        DBGPRINT(RT_DEBUG_WARN, "PeerAssocReqSanity - missing mandatory field)\n");
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}

BOOLEAN PeerReassocReqSanity(
    IN	PRTMP_ADAPTER	pAd, 
    IN  VOID *Msg, 
    IN  ULONG MsgLen, 
    OUT PUCHAR pAddr1, 
    OUT PUCHAR pAddr2, 
    OUT USHORT *pCapabilityInfo, 
    OUT USHORT *pListenInterval, 
    OUT PUCHAR pApAddr,
    OUT UCHAR *pSsidLen,
    OUT char *Ssid,
    OUT UCHAR *pRatesLen,
    OUT UCHAR Rates[],
    OUT UCHAR *RSN,
    OUT UCHAR *pRSNLen,
    OUT BOOLEAN *pbWmmCapable,
#ifdef WSC_SUPPORT
    OUT BOOLEAN *pWscCapable,
#endif // WSC_SUPPORT //
    OUT ULONG  *pRalinkIe) 
{
    CHAR         *Ptr;
    PFRAME_802_11     Fr = (PFRAME_802_11)Msg;
    PEID_STRUCT	 eid_ptr;
    UCHAR        Sanity=0;
    UCHAR       WPA[4]={0x00,0x50,0xf2,0x01};
    MAC_TABLE_ENTRY *pEntry = (MAC_TABLE_ENTRY*)NULL;
  
    // to prevent caller from using garbage output value
    *pSsidLen	= 0;
    *pRatesLen	= 0;
    *pRSNLen	= 0;
    *pbWmmCapable	= FALSE;
    *pRalinkIe	= 0;

    COPY_MAC_ADDR(pAddr1, &Fr->Hdr.Addr1);
    COPY_MAC_ADDR(pAddr2, &Fr->Hdr.Addr2);
    Ptr = Fr->Octet;

    NdisMoveMemory(pCapabilityInfo, &Fr->Octet[0], 2);
    NdisMoveMemory(pListenInterval, &Fr->Octet[2], 2);
    NdisMoveMemory(pApAddr, &Fr->Octet[4], 6);

    eid_ptr = (PEID_STRUCT) &Fr->Octet[10];

    pEntry = MacTableLookup(pAd, pAddr2);

    // get variable fields from payload and advance the pointer
    while (((UCHAR*)eid_ptr + eid_ptr->Len + 1) < ((UCHAR*)Fr + MsgLen))
    {
        switch(eid_ptr->Eid)
        {
            case IE_SSID:
                if(eid_ptr->Len <= MAX_LEN_OF_SSID)
                {
                    Sanity |= 0x01;
                    NdisMoveMemory(Ssid, eid_ptr->Octet, eid_ptr->Len);
                    *pSsidLen = eid_ptr->Len;
                }
                else
                {
                    DBGPRINT(RT_DEBUG_TRACE, "PeerAssocReqSanity - wrong IE_SSID (len=%d)\n", eid_ptr->Len);
                    return FALSE;
                }
                break;
                
            case IE_SUPP_RATES:
                if (eid_ptr->Len <= MAX_LEN_OF_SUPPORTED_RATES)
                {
                    Sanity |= 0x02;
                    NdisMoveMemory(Rates, eid_ptr->Octet, eid_ptr->Len);
                    *pRatesLen = eid_ptr->Len;
                }
                else
                {
                    DBGPRINT(RT_DEBUG_TRACE, "PeerAssocReqSanity - wrong IE_SUPP_RATES (len=%d)\n", eid_ptr->Len);
                    return FALSE;
                }
                break;
                
            case IE_EXT_SUPP_RATES:
                if (eid_ptr->Len + *pRatesLen <= MAX_LEN_OF_SUPPORTED_RATES)
                {
                    NdisMoveMemory(&Rates[*pRatesLen], eid_ptr->Octet, eid_ptr->Len);
                    *pRatesLen = (*pRatesLen) + eid_ptr->Len;
                }
                else
                {
                    NdisMoveMemory(&Rates[*pRatesLen], eid_ptr->Octet, MAX_LEN_OF_SUPPORTED_RATES - (*pRatesLen));
                    *pRatesLen = MAX_LEN_OF_SUPPORTED_RATES;
                }
			    break;

	        case IE_WPA:
	        case IE_WPA2:
#ifdef WSC_SUPPORT
                if (NdisEqualMemory(eid_ptr->Octet, WPS_OUI, 4))
                    *pWscCapable = TRUE;
#endif // WSC_SUPPORT //
	        	if (NdisEqualMemory(eid_ptr->Octet, RALINK_OUI, 3) && (eid_ptr->Len == 7))
                {
                    *pRalinkIe = eid_ptr->Octet[3];
                    break;
                }
        	
	        	// WMM_IE
                if (NdisEqualMemory(eid_ptr->Octet, WME_INFO_ELEM, 6) && (eid_ptr->Len == 7))
                {
                    *pbWmmCapable = TRUE;

                    if ((pEntry) && (pAd->PortCfg.bAPSDCapable))
                    {
                    	QBSS_STA_INFO_PARM	QosInfo;
                    	
	                	QosInfo = *(PQBSS_STA_INFO_PARM) &eid_ptr->Octet[6];
	                	pEntry->MaxSPLength = QosInfo.MaxSPLength;
	                	pEntry->bAPSDCapablePerAC[QID_AC_BE] = QosInfo.UAPSD_AC_BE;
	                	pEntry->bAPSDCapablePerAC[QID_AC_BK] = QosInfo.UAPSD_AC_BK;
	                	pEntry->bAPSDCapablePerAC[QID_AC_VI] = QosInfo.UAPSD_AC_VI;
	                	pEntry->bAPSDCapablePerAC[QID_AC_VO] = QosInfo.UAPSD_AC_VO;
	                	if ((pEntry->bAPSDCapablePerAC[QID_AC_BE] == 0) && (pEntry->bAPSDCapablePerAC[QID_AC_BK] == 0) && (pEntry->bAPSDCapablePerAC[QID_AC_VI] == 0) && (pEntry->bAPSDCapablePerAC[QID_AC_VO] == 0))
	                	{
	                		CLIENT_STATUS_CLEAR_FLAG(pEntry, fCLIENT_STATUS_APSD_CAPABLE);
	                	}
	                	else
	                	{
	                		CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_APSD_CAPABLE);
	                	}
                    }
                    
                    break;
                }
        	
                if ((!pEntry) || (pAd->PortCfg.MBSSID[pEntry->ApIdx].AuthMode < Ndis802_11AuthModeWPA))
                    break;
                
                // If this IE did not begins with 00:0x50:0xf2:0x01,  it would be proprietary.  So we ignore
                if (eid_ptr->Eid == IE_WPA)
                {
	                if (!RTMPEqualMemory(eid_ptr->Octet, WPA, 4))
	                    break;
                }
                if (eid_ptr->Len <= MAX_LEN_OF_RSNIE  && eid_ptr->Len >MIN_LEN_OF_RSNIE)
                {
                    *pRSNLen=eid_ptr->Len;
                    if (!RTMPCheckMcast(pAd, eid_ptr, pEntry))
                    {
                        DBGPRINT(RT_DEBUG_TRACE, " RTMPCheckMcast  FAILED !!!!  \n");

                        if (pEntry)
                            DisAssocAction( pAd, pEntry, SUBTYPE_DISASSOC, REASON_MCIPHER_NOT_VALID);
                        return FALSE;
                    }                        
                    if (!RTMPCheckUcast(pAd, eid_ptr, pEntry))
                    {
                        DBGPRINT(RT_DEBUG_TRACE, " RTMPCheckUcast  FAILED !!!!  \n");

                        if (pEntry)
                            DisAssocAction( pAd, pEntry, SUBTYPE_DISASSOC, REASON_UCIPHER_NOT_VALID);
                        return FALSE;
                    }          
                    if (!RTMPCheckAUTH(pAd, eid_ptr, pEntry))
                    {
                        DBGPRINT(RT_DEBUG_TRACE, " RTMPCheckAUTH Method   FAILED !!!!  \n");

                        if (pEntry)
                            DisAssocAction( pAd, pEntry, SUBTYPE_DISASSOC, REASON_INVALID_IE);
                        return FALSE;
                    }

                    NdisMoveMemory(RSN, eid_ptr->Octet, eid_ptr->Len);
                    DBGPRINT(RT_DEBUG_TRACE, "Receive IE_WPA : %x %x %x %x %x %x %x %x \n",\
                        eid_ptr->Octet[0],eid_ptr->Octet[1],eid_ptr->Octet[2],eid_ptr->Octet[3],eid_ptr->Octet[4],\
                        eid_ptr->Octet[5],eid_ptr->Octet[6],eid_ptr->Octet[7]);
                }
                else
                {
                    *pRSNLen=0;
                    DBGPRINT(RT_DEBUG_TRACE, "PeerAssocReqSanity - missing IE_WPA)\n");
                    return FALSE;
               }                
               break;

            default:
                break;
        }
        eid_ptr = (PEID_STRUCT)((UCHAR*)eid_ptr + 2 + eid_ptr->Len);        
    }

    if (Sanity != 0x03)    
    {
        DBGPRINT(RT_DEBUG_WARN, "PeerAssocReqSanity - missing mandatory field)\n");
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/* 
    ==========================================================================
    Description:
        MLME message sanity check
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
 */
BOOLEAN PeerDisassocReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT PUCHAR Addr2, 
    OUT USHORT *Reason) 
{
    PFRAME_802_11 Fr = (PFRAME_802_11)Msg;

    COPY_MAC_ADDR(Addr2, Fr->Hdr.Addr2);
    NdisMoveMemory(Reason, &Fr->Octet[0], 2);

    return TRUE;
}

/* 
    ==========================================================================
    Description:
        MLME message sanity check
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
 */
BOOLEAN PeerBeaconAndProbeRspSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT PUCHAR pAddr2, 
    OUT PUCHAR pBssid, 
    OUT CHAR Ssid[], 
    OUT UCHAR *pSsidLen, 
    OUT UCHAR *pBssType, 
    OUT USHORT *pBeaconPeriod, 
    OUT UCHAR *pChannel, 
    OUT LARGE_INTEGER *pTimestamp, 
    OUT CF_PARM *pCfParm, 
    OUT USHORT *pAtimWin, 
    OUT USHORT *pCapabilityInfo, 
    OUT UCHAR *pErp,
    OUT UCHAR *pDtimCount, 
    OUT UCHAR *pDtimPeriod, 
    OUT UCHAR *pBcastFlag, 
    OUT UCHAR *pMessageToMe, 
    OUT UCHAR SupRate[],
	OUT UCHAR *pSupRateLen,
	OUT UCHAR ExtRate[],
	OUT UCHAR *pExtRateLen,
	OUT	UCHAR *pCkipFlag,
	OUT	UCHAR *pAironetCellPowerLimit,
	OUT PEDCA_PARM       pEdcaParm,
	OUT PQBSS_LOAD_PARM  pQbssLoad,
	OUT PQOS_CAPABILITY_PARM pQosCapability,
	OUT ULONG *pRalinkIe,
    OUT UCHAR *LengthVIE,
    OUT PNDIS_802_11_VARIABLE_IEs pVIE) 
{
    CHAR                *Ptr;
    PFRAME_802_11		pFrame;
    PEID_STRUCT         pEid;
    UCHAR               SubType;
    UCHAR               Sanity;

    // Add for 3 necessary EID field check
    Sanity = 0;

    *pAtimWin = 0;
    *pErp = 0;	
    *pDtimCount = 0;
    *pDtimPeriod = 0;
    *pBcastFlag = 0;
    *pMessageToMe = 0;
    *pExtRateLen = 0;
	*pCkipFlag = 0;			        // Default of CkipFlag is 0
	*pAironetCellPowerLimit = 0xFF;  // Default of AironetCellPowerLimit is 0xFF
	*pRalinkIe = 0;
    *LengthVIE = 0;
	pCfParm->bValid = FALSE;        // default: no IE_CF found
    pQbssLoad->bValid = FALSE;      // default: no IE_QBSS_LOAD found
    pEdcaParm->bValid = FALSE;      // default: no IE_EDCA_PARAMETER found
    pQosCapability->bValid = FALSE; // default: no IE_QOS_CAPABILITY found

    pFrame = (PFRAME_802_11)Msg;

    // get subtype from header
    SubType = (UCHAR)pFrame->Hdr.FC.SubType;

    // get Addr2 and BSSID from header
    COPY_MAC_ADDR(pAddr2, pFrame->Hdr.Addr2);
    COPY_MAC_ADDR(pBssid, pFrame->Hdr.Addr3);

    Ptr = pFrame->Octet;

    // get timestamp from payload and advance the pointer
    NdisMoveMemory(pTimestamp, Ptr, TIMESTAMP_LEN);
    Ptr += TIMESTAMP_LEN;

    // get beacon interval from payload and advance the pointer
    NdisMoveMemory(pBeaconPeriod, Ptr, 2);
    Ptr += 2;

    // get capability info from payload and advance the pointer
    NdisMoveMemory(pCapabilityInfo, Ptr, 2);
    Ptr += 2;
    if (CAP_IS_ESS_ON(*pCapabilityInfo)) 
        *pBssType = BSS_INFRA;
    else 
        *pBssType = BSS_ADHOC;

    pEid = (PEID_STRUCT) Ptr;

    // get variable fields from payload and advance the pointer
    while(((UCHAR*)pEid + pEid->Len + 1) < ((UCHAR*)pFrame + MsgLen))
    {
        switch(pEid->Eid)
        {
            case IE_SSID:
                // Already has one SSID EID in this beacon, ignore the second one
				if (Sanity & 0x1)
					break;
                if(pEid->Len <= MAX_LEN_OF_SSID)
                {
                    NdisMoveMemory(Ssid, pEid->Octet, pEid->Len);
                    *pSsidLen = pEid->Len;
                    Sanity |= 0x1;
                }
                else
                {
                    DBGPRINT(RT_DEBUG_TRACE, "PeerBeaconAndProbeRspSanity - wrong IE_SSID (len=%d)\n",pEid->Len);
                    return FALSE;
                }
                break;

            case IE_SUPP_RATES:
                if(pEid->Len <= MAX_LEN_OF_SUPPORTED_RATES)
                {
					Sanity |= 0x2;
					NdisMoveMemory(SupRate, pEid->Octet, pEid->Len);
					*pSupRateLen = pEid->Len;

					// TODO: 2004-09-14 not a good design here, cause it exclude extra rates 
					// from ScanTab. We should report as is. And filter out unsupported
					// rates in MlmeAux.
					// Check against the supported rates
					// RTMPCheckRates(pAd, SupRate, pSupRateLen);
                }
                else
                {
                    DBGPRINT(RT_DEBUG_TRACE, "PeerBeaconAndProbeRspSanity - wrong IE_SUPP_RATES (len=%d)\n",pEid->Len);
                    return FALSE;
                }
                break;

            case IE_FH_PARM:
                DBGPRINT(RT_DEBUG_TRACE, "PeerBeaconAndProbeRspSanity(IE_FH_PARM) \n");
                break;

            case IE_DS_PARM:
                if(pEid->Len == 1)
                {
                    *pChannel = *pEid->Octet;                    
                    if (ChannelSanity(pAd, *pChannel) == 0)
                    {
                        DBGPRINT(RT_DEBUG_INFO, "PeerBeaconAndProbeRspSanity - wrong IE_DS_PARM (ch=%d)\n",*pChannel);
                        return FALSE;
                    }
                    Sanity |= 0x4;
                }
                else
                {
                    DBGPRINT(RT_DEBUG_TRACE, "PeerBeaconAndProbeRspSanity - wrong IE_DS_PARM (len=%d)\n",pEid->Len);
                    return FALSE;
                }
                break;

            case IE_CF_PARM:
                if(pEid->Len == 6)
                {
                    pCfParm->bValid = TRUE;
                    pCfParm->CfpCount = pEid->Octet[0];
                    pCfParm->CfpPeriod = pEid->Octet[1];
                    pCfParm->CfpMaxDuration = pEid->Octet[2] + 256 * pEid->Octet[3];
                    pCfParm->CfpDurRemaining = pEid->Octet[4] + 256 * pEid->Octet[5];
                }
                else
                {
                    DBGPRINT(RT_DEBUG_TRACE, "PeerBeaconAndProbeRspSanity - wrong IE_CF_PARM\n");
                    return FALSE;
                }
                break;

            case IE_IBSS_PARM:
                if(pEid->Len == 2)
                {
                    NdisMoveMemory(pAtimWin, pEid->Octet, pEid->Len);
                }
                else
                {
                    DBGPRINT(RT_DEBUG_TRACE, "PeerBeaconAndProbeRspSanity - wrong IE_IBSS_PARM\n");
                    return FALSE;
                }
                break;

            case IE_TIM:
                break;

            case IE_VENDOR_SPECIFIC:
                // Check the OUI version, filter out non-standard usage
				if (NdisEqualMemory(pEid->Octet, RALINK_OUI, 3) && (pEid->Len == 7))
				{
                    *pRalinkIe = pEid->Octet[3];
				}
				else if (NdisEqualMemory(pEid->Octet, WPA_OUI, 4))
                {
                    // Copy to pVIE which will report to microsoft bssid list.
		            Ptr = (PUCHAR) pVIE;
                    NdisMoveMemory(Ptr + *LengthVIE, &pEid->Eid, pEid->Len + 2);
                    *LengthVIE += (pEid->Len + 2);
				}
				else if (NdisEqualMemory(pEid->Octet, WME_PARM_ELEM, 6) && (pEid->Len == 24))
				{
				}
				else if (NdisEqualMemory(pEid->Octet, WME_INFO_ELEM, 6) && (pEid->Len == 7))
				{
                }
                DBGPRINT(RT_DEBUG_INFO, "PeerBeaconAndProbeRspSanity - Receive IE_WPA\n");
                break;

            case IE_EXT_SUPP_RATES:
                if (pEid->Len <= MAX_LEN_OF_SUPPORTED_RATES)
                {
					NdisMoveMemory(ExtRate, pEid->Octet, pEid->Len);
					*pExtRateLen = pEid->Len;

					// TODO: 2004-09-14 not a good design here, cause it exclude extra rates 
					// from ScanTab. We should report as is. And filter out unsupported
					// rates in MlmeAux.
					// Check against the supported rates
					// RTMPCheckRates(pAd, ExtRate, pExtRateLen);
                }
                break;

            case IE_ERP:
                if (pEid->Len == 1)
                {
                    *pErp = (UCHAR)pEid->Octet[0];
                }
                break;
                
            case IE_RSN:
	            if (NdisEqualMemory(pEid->Octet + 2, RSN_OUI, 3)) {
			        Ptr = (PUCHAR) pVIE;
			        NdisMoveMemory(Ptr + *LengthVIE, &pEid->Eid, pEid->Len + 2);
			        *LengthVIE += (pEid->Len + 2);
		        }
		        DBGPRINT(RT_DEBUG_INFO, "PeerBeaconAndProbeRspSanity - Receive IE_RSN\n");
	            break;
                
            case IE_CHANNEL_SWITCH_ANNOUNCEMENT:
	            //if (NdisEqualMemory(pEid->Octet + 2, RSN_OUI, 3))
	            {
			        Ptr = (PUCHAR) pVIE;
			        NdisMoveMemory(Ptr + *LengthVIE, &pEid->Eid, pEid->Len + 2);
			        *LengthVIE += (pEid->Len + 2);
		        }
		        DBGPRINT(RT_DEBUG_INFO, "PeerBeaconAndProbeRspSanity - Receive IE_RSN\n");
	            break;

            default:
                DBGPRINT(RT_DEBUG_INFO, "PeerBeaconAndProbeRspSanity - unrecognized EID = %d\n", pEid->Eid);
                break;
        }
        
        pEid = (PEID_STRUCT)((UCHAR*)pEid + 2 + pEid->Len);        
    }

	// For some 11a AP. it did not have the channel EID, patch here
	if (pAd->LatchRfRegs.Channel > 14)
    {
		*pChannel = pAd->LatchRfRegs.Channel;					 
		Sanity |= 0x4;		
    }
    
    if (Sanity != 0x7)
    {
        DBGPRINT(RT_DEBUG_WARN, "PeerBeaconAndProbeRspSanity - missing field, Sanity=0x%02x\n", Sanity);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/* 
    ==========================================================================
    Description:
        MLME message sanity check
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
 */
BOOLEAN PeerDeauthReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT PUCHAR Addr2, 
    OUT USHORT *Reason) 
{
    PFRAME_802_11 Fr = (PFRAME_802_11)Msg;

    COPY_MAC_ADDR(Addr2, &Fr->Hdr.Addr2);
    NdisMoveMemory(Reason, &Fr->Octet[0], 2);

    return TRUE;
}

BOOLEAN PeerDlsReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen,
    OUT PUCHAR pDA,
    OUT PUCHAR pSA,
    OUT USHORT *pCapabilityInfo, 
    OUT USHORT *pDlsTimeout) 
{
    CHAR            *Ptr;
    PFRAME_802_11	Fr = (PFRAME_802_11)Msg;

    // to prevent caller from using garbage output value
    *pDlsTimeout	= 0;

    Ptr = Fr->Octet;

	// offset to destination MAC address
    Ptr += 2;

    // get DA from payload and advance the pointer
    NdisMoveMemory(pDA, Ptr, MAC_ADDR_LEN);
    Ptr += MAC_ADDR_LEN;

    // get SA from payload and advance the pointer
    NdisMoveMemory(pSA, Ptr, MAC_ADDR_LEN);
    Ptr += MAC_ADDR_LEN;

    // get capability info from payload and advance the pointer
    NdisMoveMemory(pCapabilityInfo, Ptr, 2);
    Ptr += 2;

    // get capability info from payload and advance the pointer
    NdisMoveMemory(pDlsTimeout, Ptr, 2);
    Ptr += 2;

    return TRUE;
}

BOOLEAN PeerDlsRspSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen,
    OUT PUCHAR pDA,
    OUT PUCHAR pSA,
    OUT USHORT *pCapabilityInfo, 
    OUT USHORT *pStatus) 
{
    CHAR            *Ptr;
    PFRAME_802_11	Fr = (PFRAME_802_11)Msg;

    // to prevent caller from using garbage output value
    *pStatus	= 0;

    Ptr = Fr->Octet;

	// offset to destination MAC address
    Ptr += 2;

	// get status code from payload and advance the pointer
    NdisMoveMemory(pStatus, Ptr, 2);
    Ptr += 2;

    // get DA from payload and advance the pointer
    NdisMoveMemory(pDA, Ptr, MAC_ADDR_LEN);
    Ptr += MAC_ADDR_LEN;

    // get SA from payload and advance the pointer
    NdisMoveMemory(pSA, Ptr, MAC_ADDR_LEN);
    Ptr += MAC_ADDR_LEN;

	if (pStatus == 0)
	{
	    // get capability info from payload and advance the pointer
	    NdisMoveMemory(pCapabilityInfo, Ptr, 2);
	    Ptr += 2;
	}

    return TRUE;
}

BOOLEAN PeerDlsTearDownSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen,
    OUT PUCHAR pDA,
    OUT PUCHAR pSA,
    OUT USHORT *pReason) 
{
    CHAR            *Ptr;
    PFRAME_802_11	Fr = (PFRAME_802_11)Msg;

    // to prevent caller from using garbage output value
    *pReason	= 0;

    Ptr = Fr->Octet;

	// offset to destination MAC address
    Ptr += 2;

    // get DA from payload and advance the pointer
    NdisMoveMemory(pDA, Ptr, MAC_ADDR_LEN);
    Ptr += MAC_ADDR_LEN;

    // get SA from payload and advance the pointer
    NdisMoveMemory(pSA, Ptr, MAC_ADDR_LEN);
    Ptr += MAC_ADDR_LEN;

	// get reason code from payload and advance the pointer
    NdisMoveMemory(pReason, Ptr, 2);
    Ptr += 2;

    return TRUE;
}

#ifdef APCLI_SUPPORT
/* 
    ==========================================================================
    Description:
        MLME message sanity check
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
 */
BOOLEAN PeerDeauthSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT PUCHAR pAddr2, 
    OUT USHORT *pReason) 
{
    PFRAME_802_11 pFrame = (PFRAME_802_11)Msg;

    COPY_MAC_ADDR(pAddr2, pFrame->Hdr.Addr2);
    NdisMoveMemory(pReason, &pFrame->Octet[0], 2);

    return TRUE;
}

/* 
    ==========================================================================
    Description:
        MLME message sanity check
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
 */
BOOLEAN MlmeAuthReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT PUCHAR pAddr, 
    OUT ULONG *pTimeout, 
    OUT USHORT *pAlg) 
{
    MLME_AUTH_REQ_STRUCT *pInfo;

    pInfo  = (MLME_AUTH_REQ_STRUCT *)Msg;
    COPY_MAC_ADDR(pAddr, pInfo->Addr);
    *pTimeout = pInfo->Timeout;
    *pAlg = pInfo->Alg;
    
    if (((*pAlg == Ndis802_11AuthModeShared) ||(*pAlg == Ndis802_11AuthModeOpen)) && 
        ((*pAddr & 0x01) == 0)) 
    {
        return TRUE;
    } 
    else 
    {
        DBGPRINT(RT_DEBUG_TRACE, "MlmeAuthReqSanity fail - wrong algorithm\n");
        return FALSE;
    }
}

/* 
    ==========================================================================
    Description:
        MLME message sanity check
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
 */
BOOLEAN MlmeAssocReqSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT PUCHAR pApAddr, 
    OUT USHORT *pCapabilityInfo, 
    OUT ULONG *pTimeout, 
    OUT USHORT *pListenIntv) 
{
    MLME_ASSOC_REQ_STRUCT *pInfo;

    pInfo = (MLME_ASSOC_REQ_STRUCT *)Msg;
    *pTimeout = pInfo->Timeout;                             // timeout
    COPY_MAC_ADDR(pApAddr, pInfo->Addr);                   // AP address
    *pCapabilityInfo = pInfo->CapabilityInfo;               // capability info
    *pListenIntv = pInfo->ListenIntv;
    
    return TRUE;
}

/* 
    ==========================================================================
    Description:
        MLME message sanity check
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
 */
BOOLEAN PeerAssocRspSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *pMsg, 
    IN ULONG MsgLen, 
    OUT PUCHAR pAddr2, 
    OUT USHORT *pCapabilityInfo, 
    OUT USHORT *pStatus, 
    OUT USHORT *pAid, 
    OUT UCHAR SupRate[], 
    OUT UCHAR *pSupRateLen,
    OUT UCHAR ExtRate[], 
    OUT UCHAR *pExtRateLen,
    OUT PEDCA_PARM pEdcaParm)
{
    CHAR          IeType, *Ptr;
    PFRAME_802_11 pFrame = (PFRAME_802_11)pMsg;
    PEID_STRUCT   pEid;
    
    COPY_MAC_ADDR(pAddr2, pFrame->Hdr.Addr2);
    Ptr = pFrame->Octet;
        
    NdisMoveMemory(pCapabilityInfo, &pFrame->Octet[0], 2);
    NdisMoveMemory(pStatus,         &pFrame->Octet[2], 2);
    *pExtRateLen = 0;
    pEdcaParm->bValid = FALSE;
    
    if (*pStatus != MLME_SUCCESS) 
        return TRUE;
    
    NdisMoveMemory(pAid, &pFrame->Octet[4], 2);

    // 	change Endian in RTMPFrameEndianChange() on big endian platform
    //*pAid = le2cpu16(*pAid);
    
    // TODO: check big endian issue &0x3fff
    *pAid = (*pAid) & 0x3fff; // AID is low 14-bit
        
    // -- get supported rates from payload and advance the pointer
    IeType = pFrame->Octet[6];
    *pSupRateLen = pFrame->Octet[7];
    if ((IeType != IE_SUPP_RATES) || (*pSupRateLen > MAX_LEN_OF_SUPPORTED_RATES))
    {
        DBGPRINT(RT_DEBUG_TRACE, "PeerAssocRspSanity fail - wrong SupportedRates IE\n");
        return FALSE;
    } 
    else 
   		NdisMoveMemory(SupRate, &pFrame->Octet[8], *pSupRateLen);

    // many AP implement proprietary IEs in non-standard order, we'd better
    // tolerate mis-ordered IEs to get best compatibility
    pEid = (PEID_STRUCT) &pFrame->Octet[8 + (*pSupRateLen)];
            
    // get variable fields from payload and advance the pointer
    while (((UCHAR*)pEid + pEid->Len + 1) < ((UCHAR*)pFrame + MsgLen))
    {
        switch (pEid->Eid)
        {
            case IE_EXT_SUPP_RATES:
                if (pEid->Len <= MAX_LEN_OF_SUPPORTED_RATES)
                {
                    NdisMoveMemory(ExtRate, pEid->Octet, pEid->Len);
                    *pExtRateLen = pEid->Len;
                }
                break;
				
			case IE_VENDOR_SPECIFIC:
                // handle WME PARAMTER ELEMENT
                if (NdisEqualMemory(pEid->Octet, WME_PARM_ELEM, 6) && (pEid->Len == 24))
                {
                    PUCHAR ptr;
                    int i;

                    // parsing EDCA parameters
                    pEdcaParm->bValid          = TRUE;
                    pEdcaParm->bQAck           = FALSE; // pEid->Octet[0] & 0x10;
                    pEdcaParm->bQueueRequest   = FALSE; // pEid->Octet[0] & 0x20;
                    pEdcaParm->bTxopRequest    = FALSE; // pEid->Octet[0] & 0x40;
                    //pEdcaParm->bMoreDataAck    = FALSE; // pEid->Octet[0] & 0x80;
                    pEdcaParm->EdcaUpdateCount = pEid->Octet[6] & 0x0f;
                    ptr = &pEid->Octet[8];
                    for (i=0; i<4; i++)
                    {
                        UCHAR aci = (*ptr & 0x60) >> 5; // b5~6 is AC INDEX
                        pEdcaParm->bACM[aci]  = (((*ptr) & 0x10) == 0x10);   // b5 is ACM
                        pEdcaParm->Aifsn[aci] = (*ptr) & 0x0f;               // b0~3 is AIFSN
                        pEdcaParm->Cwmin[aci] = *(ptr+1) & 0x0f;             // b0~4 is Cwmin
                        pEdcaParm->Cwmax[aci] = *(ptr+1) >> 4;               // b5~8 is Cwmax
                        pEdcaParm->Txop[aci]  = *(ptr+2) + 256 * (*(ptr+3)); // in unit of 32-us
                        ptr += 4; // point to next AC
                    }
                }
                break;

#if 0				
            case IE_EDCA_PARAMETER:
                if (pEid->Len == 18)
                {
                    PUCHAR ptr;
                    int i;
                    pEdcaParm->bValid          = TRUE;
                    pEdcaParm->bQAck           = pEid->Octet[0] & 0x10;
                    pEdcaParm->bQueueRequest   = pEid->Octet[0] & 0x20;
                    pEdcaParm->bTxopRequest    = pEid->Octet[0] & 0x40;
//                  pEdcaParm->bMoreDataAck    = pEid->Octet[0] & 0x80;
                    pEdcaParm->EdcaUpdateCount = pEid->Octet[0] & 0x0f;
                    ptr = &pEid->Octet[2];
                    for (i=0; i<4; i++)
                    {
                        UCHAR aci = (*ptr & 0x60) >> 5; // b5~6 is AC INDEX
                        pEdcaParm->bACM[aci]  = (((*ptr) & 0x10) == 0x10);   // b5 is ACM
                        pEdcaParm->Aifsn[aci] = (*ptr) & 0x0f;               // b0~3 is AIFSN
                        pEdcaParm->Cwmin[aci] = *(ptr+1) & 0x0f;             // b0~4 is Cwmin
                        pEdcaParm->Cwmax[aci] = *(ptr+1) >> 4;               // b5~8 is Cwmax
                        pEdcaParm->Txop[aci]  = *(ptr+2) + 256 * (*(ptr+3)); // in unit of 32-us
                        ptr += 4; // point to next AC
                    }
                }
                break;
#endif
            default:
                DBGPRINT(RT_DEBUG_TRACE, "PeerAssocRspSanity - ignore unrecognized EID = %d\n", pEid->Eid);
                break;
        }
                   
        pEid = (PEID_STRUCT)((UCHAR*)pEid + 2 + pEid->Len);        
    }

    return TRUE;
}

/* 
    ==========================================================================
    Description:
        MLME message sanity check
    Return:
        TRUE if all parameters are OK, FALSE otherwise       
    ==========================================================================
 */
BOOLEAN PeerDisassocSanity(
    IN PRTMP_ADAPTER pAd, 
    IN VOID *Msg, 
    IN ULONG MsgLen, 
    OUT PUCHAR pAddr2, 
    OUT USHORT *pReason) 
{
    PFRAME_802_11 pFrame = (PFRAME_802_11)Msg;

    COPY_MAC_ADDR(pAddr2, pFrame->Hdr.Addr2);
    NdisMoveMemory(pReason, &pFrame->Octet[0], 2);

    return TRUE;
}
#endif /* APCLI_SUPPORT */

