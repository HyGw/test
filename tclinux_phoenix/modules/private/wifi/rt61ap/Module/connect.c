/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2005, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attempt
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	connect.c

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	John		2004-08-08		Major modification from RT2560
*/
#include "rt_config.h"

UCHAR PowerConstraintIE[3] = {IE_POWER_CONSTRAINT, 1, 3};

/*
    ==========================================================================
    Description:
        Pre-build All BEACON frame in the shared memory
    ==========================================================================
*/
VOID MakeAllBssBeacon(
    IN PRTMP_ADAPTER pAd)
{
	INT		i, j;
	ULONG 	csr5;

	// before MakeBssBeacon, clear all beacon TxD's valid bit
	for(i=0; i<4; i++)
	{
		for (j=0; j<TXINFO_SIZE; j++)  // 24-byte TXINFO field
	    {
	        RTMP_IO_WRITE8(pAd, HW_BEACON_BASE0 + (i * HW_BEACON_OFFSET) + j, 0);
	    }
	}

	for(i=0; i<pAd->PortCfg.BssidNum; i++)
	{
		MakeBssBeacon(pAd, i);
	}

	// set MAC_CSR5 to turn on Unicast_to_me report
	RTMP_IO_READ32(pAd, MAC_CSR5, &csr5);
	csr5 &= 0xFFFCFFFF;
	if (pAd->PortCfg.BssidNum == 1)
		csr5 |= 0x00030000;
#ifdef MBSS_SUPPORT
	else if (pAd->PortCfg.BssidNum == 2)
		csr5 |= 0x00020000;
	else
		csr5 |= 0x00000000;	// three or four BSSID
#endif /* MBSS_SUPPORT */

#ifdef APCLI_SUPPORT
	if (pAd->CurrentAddress[ETH_LENGTH_OF_ADDRESS-1] % 2 == 0)  // add by johnli, fix WPAPSK bug if the MAC address is odd number
	{
		csr5 &= 0xFFFCFFFF;
		if (pAd->PortCfg.BssidNum == 1)
			csr5 |= 0x00020000;
	}
#endif

	RTMP_IO_WRITE32(pAd, MAC_CSR5, csr5);
}

/*
    ==========================================================================
    Description:
        Pre-build All BEACON frame in the shared memory
    ==========================================================================
*/
VOID UpdateAllBeaconFrame(
    IN PRTMP_ADAPTER pAd)
{
	INT		i;

	// update DTIM Count
	if (pAd->Mlme.DtimCount == 0)
        pAd->Mlme.DtimCount = pAd->Mlme.DtimPeriod - 1;
	else
        pAd->Mlme.DtimCount -= 1;

	for(i=0; i<pAd->PortCfg.BssidNum; i++)
	{
		UpdateBeaconFrame(pAd, i);
	}
}

/*
    ==========================================================================
    Description:
        Update BEACON frame in the shared memory per TBTT
    ==========================================================================
*/
VOID MakeBssBeacon(
    IN PRTMP_ADAPTER	pAd,
    IN INT				apidx) 
{
	UCHAR         SsidIe = IE_SSID, DsIe = IE_DS_PARM, SuppIe = IE_SUPP_RATES, RSNIe = IE_WPA, RSNIe2 = IE_WPA2,
					SupportedRatesLen;
    UCHAR         DsLen = 1, SsidLen;
    HEADER_802_11 BcnHdr;
    LARGE_INTEGER FakeTimestamp;
    ULONG         FrameLen;
    PTXD_STRUC    pTxD = &pAd->PortCfg.MBSSID[apidx].BeaconTxD;
    PUCHAR        pBeaconFrame = pAd->PortCfg.MBSSID[apidx].BeaconBuf;

    // Bridge mode doesn't send beacon.
    if (pAd->WdsTab.Mode == WDS_BRIDGE_MODE)
        return;

    if ((pAd->PortCfg.PhyMode == PHY_11BG_MIXED) || (pAd->PortCfg.PhyMode == PHY_11G))
        SupportedRatesLen = 4;
    else	// include A band
        SupportedRatesLen = pAd->PortCfg.SupportedRatesLen;

    if (pAd->PortCfg.MBSSID[apidx].bHideSsid)
        SsidLen = 0;
    else
        SsidLen = pAd->PortCfg.MBSSID[apidx].SsidLen;

    MgtMacHeaderInit(pAd, &BcnHdr, SUBTYPE_BEACON, 0, pAd->PortCfg.Broadcast, pAd->PortCfg.MBSSID[apidx].Bssid);

    if ((pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPA) || (pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPAPSK))
    	RSNIe = IE_WPA;
    else if ((pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPA2) || (pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPA2PSK))
    	RSNIe = IE_WPA2;
    
	if ((pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPA1WPA2) || (pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPAPSKWPA2PSK))
    {
        MakeOutgoingFrame(pBeaconFrame,                             &FrameLen,
                        sizeof(HEADER_802_11),                      &BcnHdr,
                        TIMESTAMP_LEN,                              &FakeTimestamp,
                        2,                                          &pAd->PortCfg.BeaconPeriod,
                        2,                                          &pAd->PortCfg.MBSSID[apidx].CapabilityInfo,
                        1,                                          &SsidIe,
                        1,                                          &SsidLen,
                        SsidLen,                                    pAd->PortCfg.MBSSID[apidx].Ssid,
                        1,                                          &SuppIe,
                        1,                                          &SupportedRatesLen,
                        SupportedRatesLen,                          pAd->PortCfg.SupportedRates,
                        1,                                          &DsIe,
                        1,                                          &DsLen,
                        1,                                          &pAd->PortCfg.Channel,
                        1,                                          &RSNIe,
                        1,                                          &pAd->PortCfg.MBSSID[apidx].RSNIE_Len[0],
                        pAd->PortCfg.MBSSID[apidx].RSNIE_Len[0],     pAd->PortCfg.MBSSID[apidx].RSN_IE[0],
                        1,                                          &RSNIe2,
                        1,                                          &pAd->PortCfg.MBSSID[apidx].RSNIE_Len[1],
                        pAd->PortCfg.MBSSID[apidx].RSNIE_Len[1],     pAd->PortCfg.MBSSID[apidx].RSN_IE[1],
                        END_OF_ARGS);
    }
	else if (pAd->PortCfg.MBSSID[apidx].AuthMode >= Ndis802_11AuthModeWPA)
    {
        MakeOutgoingFrame(pBeaconFrame,                             &FrameLen,
                        sizeof(HEADER_802_11),                      &BcnHdr,
                        TIMESTAMP_LEN,                              &FakeTimestamp,
                        2,                                          &pAd->PortCfg.BeaconPeriod,
                        2,                                          &pAd->PortCfg.MBSSID[apidx].CapabilityInfo,
                        1,                                          &SsidIe,
                        1,                                          &SsidLen,
                        SsidLen,                                    pAd->PortCfg.MBSSID[apidx].Ssid,
                        1,                                          &SuppIe,
                        1,                                          &SupportedRatesLen,
                        SupportedRatesLen,                          pAd->PortCfg.SupportedRates,
                        1,                                          &DsIe,
                        1,                                          &DsLen,
                        1,                                          &pAd->PortCfg.Channel,
                        1,                                          &RSNIe,
                        1,                                          &pAd->PortCfg.MBSSID[apidx].RSNIE_Len[0],
                        pAd->PortCfg.MBSSID[apidx].RSNIE_Len[0],   pAd->PortCfg.MBSSID[apidx].RSN_IE[0],
                        END_OF_ARGS);
    }
    else
    {
        MakeOutgoingFrame(pBeaconFrame,                  &FrameLen,
                        sizeof(HEADER_802_11),           &BcnHdr,
                        TIMESTAMP_LEN,                   &FakeTimestamp,
                        2,                               &pAd->PortCfg.BeaconPeriod,
                        2,                               &pAd->PortCfg.MBSSID[apidx].CapabilityInfo,
                        1,                               &SsidIe,
                        1,                               &SsidLen,
                        SsidLen,                         pAd->PortCfg.MBSSID[apidx].Ssid,
                        1,                               &SuppIe,
                        1,                               &SupportedRatesLen,
                        SupportedRatesLen,               pAd->PortCfg.SupportedRates,
                        1,                               &DsIe,
                        1,                               &DsLen,
                        1,                               &pAd->PortCfg.Channel,
                        END_OF_ARGS);
    }

//#ifdef WMM_SUPPORT
#if 0
    // add WMM IE here
    if (pAd->PortCfg.MBSSID[apidx].bWmmCapable)
    {
        ULONG TmpLen;
        UCHAR i;
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

        MakeOutgoingFrame(pBeaconFrame+FrameLen,         &TmpLen,
                          26,                            WmeParmIe,
                          END_OF_ARGS);
        FrameLen += TmpLen;
    }
#endif /* WMM_SUPPORT */

    // add country IE, power constraint IE
    if (pAd->PortCfg.bCountryFlag)
    {
        ULONG TmpLen, TmpLen2=0;
        UCHAR TmpFrame[256];
        UCHAR CountryIe = IE_COUNTRY;
        UCHAR MaxTxPower = pAd->PortCfg.MaxTxPowerLevel;

		// Only 802.11a APs that comply with 802.11h are required to include a Power Constrint Element(IE=32) 
		// in beacons and probe response frames
		if (pAd->PortCfg.PhyMode == PHY_11A && pAd->PortCfg.RadarDetect.IEEE80211H == TRUE)
		{
        // prepare power constraint IE
        MakeOutgoingFrame(pBeaconFrame+FrameLen,	&TmpLen,
                          3,							PowerConstraintIE,
                          END_OF_ARGS);
        FrameLen += TmpLen;
		}	

        NdisZeroMemory(TmpFrame, sizeof(TmpFrame));

		// prepare channel information
        MakeOutgoingFrame(TmpFrame+TmpLen2,     &TmpLen,
                          1,                 	&pAd->ChannelList[0].Channel,
                          1,                 	&pAd->ChannelListNum,
                          1,                 	&MaxTxPower,
                          END_OF_ARGS);
        TmpLen2 += TmpLen;

        // need to do the padding bit check, and concatenate it
        if ((TmpLen2%2) == 0)
        {
        	UCHAR	TmpLen3 = TmpLen2+4;
	        MakeOutgoingFrame(pBeaconFrame+FrameLen,&TmpLen,
	                          1,                 	&CountryIe,
	                          1,                 	&TmpLen3,
	                          3,                 	pAd->PortCfg.CountryCode,
	                          TmpLen2+1,				TmpFrame,
	                          END_OF_ARGS);
        }
        else
        {
        	UCHAR	TmpLen3 = TmpLen2+3;
	        MakeOutgoingFrame(pBeaconFrame+FrameLen,&TmpLen,
	                          1,                 	&CountryIe,
	                          1,                 	&TmpLen3,
	                          3,                 	pAd->PortCfg.CountryCode,
	                          TmpLen2,				TmpFrame,
	                          END_OF_ARGS);
        }
        FrameLen += TmpLen;
    }

#ifdef WSC_SUPPORT
    // add Simple Config Information Element
    if ((pAd->PortCfg.MBSSID[apidx].WscControl.WscConfMode >= 1) && (pAd->PortCfg.MBSSID[apidx].WscIEBeacon.ValueLen))
    {
		ULONG WscTmpLen = 0;
        
		MakeOutgoingFrame(pBeaconFrame+FrameLen,                            &WscTmpLen,
						  pAd->PortCfg.MBSSID[apidx].WscIEBeacon.ValueLen,  pAd->PortCfg.MBSSID[apidx].WscIEBeacon.Value,
                              END_OF_ARGS);
		FrameLen += WscTmpLen;		  
    }

    if ((pAd->PortCfg.MBSSID[apidx].WscControl.WscConfMode != WSC_DISABLE) &&
        (pAd->PortCfg.MBSSID[apidx].IEEE8021X == FALSE) && 
        (pAd->PortCfg.MBSSID[apidx].WepStatus == Ndis802_11WEPEnabled))
    {
        /*
            Non-WPS Windows XP and Vista PCs are unable to determine if a WEP enalbed network is static key based 
            or 802.1X based. If the legacy station gets an EAP-Rquest/Identity from the AP, it assume the WEP
            network is 802.1X enabled & will prompt the user for 802.1X credentials. If the legacy station doesn't
            receive anything after sending an EAPOL-Start, it will assume the WEP network is static key based and
            prompt user for the WEP key. <<from "WPS and Static Key WEP Networks">>
            A WPS enabled AP should include this IE in the beacon when the AP is hosting a static WEP key network.  
            The IE would be 7 bytes long with the Extended Capability field set to 0 (all bits zero)
            http://msdn.microsoft.com/library/default.asp?url=/library/en-us/randz/protocol/securing_public_wi-fi_hotspots.asp
        */
        ULONG TempLen = 0;
        UCHAR PROVISION_SERVICE_IE[7] = {0xDD, 0x05, 0x00, 0x50, 0xF2, 0x05, 0x00};
        MakeOutgoingFrame(pBeaconFrame+FrameLen,        &TempLen,
						  7,                            PROVISION_SERVICE_IE,
                          END_OF_ARGS);
        FrameLen += TempLen;
    }
#endif // WSC_SUPPORT //

#ifdef BIG_ENDIAN
    RTMPFrameEndianChange(pAd, pBeaconFrame, DIR_WRITE, FALSE);
#endif

#ifdef CONFIG_64BIT
    RTMPWriteTxDescriptor(pAd, pTxD, CIPHER_NONE, 0, 0, FALSE, FALSE, TRUE, SHORT_RETRY, IFS_BACKOFF,
        pAd->PortCfg.MlmeRate, FrameLen, QID_MGMT, PTYPE_SPECIAL|PSUBTYPE_MGMT, NULL, NULL, pAd->PortCfg.MBSSID[apidx].bWmmCapable, FALSE, FALSE, FALSE, NULL);
#else
    RTMPWriteTxDescriptor(pAd, pTxD, CIPHER_NONE, 0, 0, FALSE, FALSE, TRUE, SHORT_RETRY, IFS_BACKOFF, 
        pAd->PortCfg.MlmeRate, FrameLen, QID_MGMT, PTYPE_SPECIAL|PSUBTYPE_MGMT, NULL, NULL, pAd->PortCfg.MBSSID[apidx].bWmmCapable, FALSE, FALSE, FALSE);
#endif // CONFIG_64BIT //

    pAd->PortCfg.MBSSID[apidx].TimIELocationInBeacon = (UCHAR)FrameLen; 
    pAd->PortCfg.MBSSID[apidx].CapabilityInfoLocationInBeacon = sizeof(HEADER_802_11) + TIMESTAMP_LEN + 2;

    DBGPRINT(RT_DEBUG_TRACE,"MakeBssBeacon(ra%d)(FrameLen=%d,TimIELocateInBeacon=%d,CapInfoLocateInBeacon=%d)\n", apidx, FrameLen, pAd->PortCfg.MBSSID[apidx].TimIELocationInBeacon,pAd->PortCfg.MBSSID[apidx].CapabilityInfoLocationInBeacon);
}

VOID UpdateBeaconFrame(
    IN PRTMP_ADAPTER	pAd,
    IN INT				apidx) 
{
	PTXD_STRUC		pTxD = &pAd->PortCfg.MBSSID[apidx].BeaconTxD;
    PHEADER_802_11	pBcnHdr = (PHEADER_802_11)pAd->PortCfg.MBSSID[apidx].BeaconBuf;
    UCHAR			*ptr;
    ULONG			FrameLen = pAd->PortCfg.MBSSID[apidx].TimIELocationInBeacon;
    UCHAR			byte0 = (UCHAR)(pAd->PortCfg.MBSSID[apidx].TimBitmap & 0x000000fe);  // skip AID#0
    UCHAR			byte1 = (UCHAR)((pAd->PortCfg.MBSSID[apidx].TimBitmap & 0x0000ff00) >> 8);
    UCHAR			byte2 = (UCHAR)((pAd->PortCfg.MBSSID[apidx].TimBitmap & 0x00ff0000) >> 16);
    UCHAR			byte3 = (UCHAR)((pAd->PortCfg.MBSSID[apidx].TimBitmap & 0xff000000) >> 24);
    UCHAR			byte4 = (UCHAR)(pAd->PortCfg.MBSSID[apidx].TimBitmap2 & 0x000000ff);
    UCHAR			byte5 = (UCHAR)((pAd->PortCfg.MBSSID[apidx].TimBitmap2 & 0x0000ff00) >> 8);
    UCHAR			byte6 = (UCHAR)((pAd->PortCfg.MBSSID[apidx].TimBitmap2 & 0x00ff0000) >> 16);
    UCHAR			byte7 = (UCHAR)((pAd->PortCfg.MBSSID[apidx].TimBitmap2 & 0xff000000) >> 24);
    UINT			i;
	UCHAR			PreTrafficInd = pAd->PortCfg.MBSSID[apidx].TrafficInd; // previous traffic indicator status

    // Bridge mode doesn't send beacon.
    if (pAd->WdsTab.Mode == WDS_BRIDGE_MODE)
        return;
    
#ifdef WIFI_TEST	// work-around for awaking Connexant STA
	// Fix WPA2 4.4.2 because beacon time not accurate.  Borrow Albert's patch for Connexant
	// here instead of resetting the beacon time stamp every 10 beacons.
	if (pAd->PortCfg.MBSSID[apidx].TimBitmap & 0x01) /* bc/mc exist */
	{
		/* set STATION TIM bit if the station is in PW mode and
		   do NOT care whether any queued packet is for it */
		for (i = 0; i < MAX_LEN_OF_MAC_TABLE; i++) 
	    {
        	MAC_TABLE_ENTRY *pEntry = &pAd->MacTab.Content[i];

	        if (pEntry->Valid == FALSE || pEntry->ApIdx != apidx)
            	continue;

			if (pEntry->PsMode == PWR_SAVE)
			{
				if (pEntry->Aid >= 32)
	            	pAd->PortCfg.MBSSID[apidx].TimBitmap2 |= BIT32[(pEntry->Aid)-32];
		        else
		            pAd->PortCfg.MBSSID[apidx].TimBitmap |= BIT32[pEntry->Aid];	    	    
			}
		}

		// Update TimBitMap to awake STA
		byte0 = (UCHAR)(pAd->PortCfg.MBSSID[apidx].TimBitmap & 0x000000fe);  // skip AID#0
    	byte1 = (UCHAR)((pAd->PortCfg.MBSSID[apidx].TimBitmap & 0x0000ff00) >> 8);
    	byte2 = (UCHAR)((pAd->PortCfg.MBSSID[apidx].TimBitmap & 0x00ff0000) >> 16);
    	byte3 = (UCHAR)((pAd->PortCfg.MBSSID[apidx].TimBitmap & 0xff000000) >> 24);
    	byte4 = (UCHAR)(pAd->PortCfg.MBSSID[apidx].TimBitmap2 & 0x000000ff);
    	byte5 = (UCHAR)((pAd->PortCfg.MBSSID[apidx].TimBitmap2 & 0x0000ff00) >> 8);
    	byte6 = (UCHAR)((pAd->PortCfg.MBSSID[apidx].TimBitmap2 & 0x00ff0000) >> 16);
    	byte7 = (UCHAR)((pAd->PortCfg.MBSSID[apidx].TimBitmap2 & 0xff000000) >> 24);
	}

#endif
	
    // update BEACON's Capability
    ptr = (UCHAR *)pBcnHdr + pAd->PortCfg.MBSSID[apidx].CapabilityInfoLocationInBeacon;
    *ptr = (UCHAR)(pAd->PortCfg.MBSSID[apidx].CapabilityInfo & 0x00ff);
    *(ptr+1) = (UCHAR)((pAd->PortCfg.MBSSID[apidx].CapabilityInfo & 0xff00) >> 8);

    //
    // fill up Channel Switch Announcement Element
    //
    if ((pAd->PortCfg.PhyMode == PHY_11A) && (pAd->PortCfg.RadarDetect.IEEE80211H == TRUE) && (pAd->PortCfg.RadarDetect.RDMode == RD_SWITCHING_MODE))
    {
    	ptr = (UCHAR *)pBcnHdr + FrameLen;
    	*ptr = IE_CHANNEL_SWITCH_ANNOUNCEMENT;
        *(ptr + 1) = 3;
        *(ptr + 2) = 1;
        *(ptr + 3) = pAd->PortCfg.Channel;
        *(ptr + 4) = pAd->PortCfg.RadarDetect.CSPeriod - pAd->PortCfg.RadarDetect.CSCount;
        ptr      += 5;
        FrameLen += 5;
    }

	// step 2 - update TIM IE
#if 0	// remove to connect.c::UpdateAllBeaconFrame
    if (pAd->Mlme.DtimCount == 0)
        pAd->Mlme.DtimCount = pAd->Mlme.DtimPeriod - 1;
    else
        pAd->Mlme.DtimCount -= 1;
#endif
    
    ptr = (UCHAR *)pBcnHdr + FrameLen;
    *ptr = IE_TIM;
    *(ptr + 2) = pAd->Mlme.DtimCount;
    *(ptr + 3) = pAd->Mlme.DtimPeriod;

    if (byte0 || byte1) // there's some backlog frame for AID 1-15
    {
        *(ptr + 4) = 0;      // Virtual TIM bitmap stars from AID #0
        *(ptr + 5) = byte0;
        *(ptr + 6) = byte1;
        *(ptr + 7) = byte2;
        *(ptr + 8) = byte3;
        *(ptr + 9) = byte4;
        *(ptr + 10) = byte5;
        *(ptr + 11) = byte6;
        *(ptr + 12) = byte7;
        if (byte7)      *(ptr + 1) = 11; // IE length
        else if (byte6) *(ptr + 1) = 10; // IE length
        else if (byte5) *(ptr + 1) = 9;  // IE length
        else if (byte4) *(ptr + 1) = 8;  // IE length
        else if (byte3) *(ptr + 1) = 7;  // IE length
        else if (byte2) *(ptr + 1) = 6;  // IE length
        else if (byte1) *(ptr + 1) = 5;  // IE length
        else            *(ptr + 1) = 4;  // IE length
    }
    else if (byte2 || byte3) // there's some backlogged frame for AID 16-31
    {
        *(ptr + 4) = 2;      // Virtual TIM bitmap starts from AID #16
        *(ptr + 5) = byte2;
        *(ptr + 6) = byte3;
        *(ptr + 7) = byte4;
        *(ptr + 8) = byte5;
        *(ptr + 9) = byte6;
        *(ptr + 10) = byte7;
        if (byte7)      *(ptr + 1) = 9; // IE length
        else if (byte6) *(ptr + 1) = 8; // IE length
        else if (byte5) *(ptr + 1) = 7; // IE length
        else if (byte4) *(ptr + 1) = 6; // IE length
        else if (byte3) *(ptr + 1) = 5; // IE length
        else            *(ptr + 1) = 4; // IE length
    }
    else if (byte4 || byte5) // there's some backlogged frame for AID 32-47
    {
        *(ptr + 4) = 4;      // Virtual TIM bitmap starts from AID #32
        *(ptr + 5) = byte4;
        *(ptr + 6) = byte5;
        *(ptr + 7) = byte6;
        *(ptr + 8) = byte7;
        if (byte7)      *(ptr + 1) = 7; // IE length
        else if (byte6) *(ptr + 1) = 6; // IE length
        else if (byte5) *(ptr + 1) = 5; // IE length
        else            *(ptr + 1) = 4; // IE length
    }
    else if (byte6 || byte7) // there's some backlogged frame for AID 48-63
    {
        *(ptr + 4) = 6;      // Virtual TIM bitmap starts from AID #48
        *(ptr + 5) = byte6;
        *(ptr + 6) = byte7;
        if (byte7)      *(ptr + 1) = 5; // IE length
        else            *(ptr + 1) = 4; // IE length
    }
    else // no backlogged frames
    {
        *(ptr + 1) = 4; // IE length
        *(ptr + 4) = 0;
        *(ptr + 5) = 0;
    }
    
    // bit0 means backlogged mcast/bcast
    // The Traffic Indicator bit is set to 1 in TIM elements 
    // with a value of 0 in the DTIM Count field when one or
    // more broadcast or multicast frames are buffered at the AP. (refer to IEEE802.11 1999)
#ifndef WIFI_TEST
	if (pAd->Mlme.DtimCount == 0)
	{
    	*(ptr + 4) |= (pAd->PortCfg.MBSSID[apidx].TimBitmap & 0x01); 

		if (pAd->PortCfg.MBSSID[apidx].TimBitmap & 0x01)
			pAd->PortCfg.MBSSID[apidx].TrafficInd = 1;
		else
			pAd->PortCfg.MBSSID[apidx].TrafficInd = 0;		
	}
	else
	{
		pAd->PortCfg.MBSSID[apidx].TrafficInd = 0; // clear this flag
	}
#else	// work-around for awaking Connexant STA
	// Fix WPA2 4.4.2 because beacon time not accurate.  Borrow Albert's patch for Connexant
	// here instead of resetting the beacon time stamp every 10 beacons.
	{
		/* dont care DTIM == 0, set TIM bit 0 for bc/mc packet,
		   but we still deliver bc/mc packets when DTIM == 0 */
    	*(ptr + 4) |= (pAd->PortCfg.MBSSID[apidx].TimBitmap & 0x01);

		if (pAd->PortCfg.MBSSID[apidx].TimBitmap & 0x01)
			pAd->PortCfg.MBSSID[apidx].TrafficInd = 1;
		else
			pAd->PortCfg.MBSSID[apidx].TrafficInd = 0;		
	}	
#endif
    
    // adjust BEACON length according to the new TIM
    FrameLen += (2 + *(ptr+1)); 

#ifdef WMM_SUPPORT
    // add WMM IE here
    if (pAd->PortCfg.MBSSID[apidx].bWmmCapable)
    {
        ULONG TmpLen;
        UCHAR i;
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

        MakeOutgoingFrame((UCHAR *)pBcnHdr+FrameLen,     &TmpLen,
                          26,                            WmeParmIe,
                          END_OF_ARGS);
        FrameLen += TmpLen;
    }
#endif /* WMM_SUPPORT */

	// Update ERP and extended tx rate
    if ((pAd->PortCfg.PhyMode == PHY_11BG_MIXED) || (pAd->PortCfg.PhyMode == PHY_11G))
    {
        UCHAR ExtendedRatesLen = pAd->PortCfg.SupportedRatesLen - 4;
        
        //
        // fill ERP IE
        // 
        ptr = (UCHAR *)pBcnHdr + FrameLen; // pTxD->DataByteCnt;
        *ptr = IE_ERP;
        *(ptr + 1) = 1;
        *(ptr + 2) = pAd->Mlme.ErpIeContent;

        //
        // fill up EXTENDED RATE IE.
        // 
        *(ptr + 3) = IE_EXT_SUPP_RATES;
        *(ptr + 4) = ExtendedRatesLen;
        NdisMoveMemory(ptr + 5, &pAd->PortCfg.SupportedRates[4], *(ptr + 4));
        ptr      += (3 + 2 + ExtendedRatesLen);
        FrameLen += (3 + 2 + ExtendedRatesLen);
    }

#ifdef AGGREGATION_SUPPORT
	// add Ralink-specific IE here - Byte0.b0=1 for aggregation, Byte0.b1=1 for piggy-back
    if (pAd->PortCfg.bAggregationCapable)
    {
    	{
	        ULONG TmpLen;
	        UCHAR RalinkSpecificIe[9] = {IE_VENDOR_SPECIFIC, 7, 0x00, 0x0c, 0x43, 0x01, 0x00, 0x00, 0x00}; 
	        MakeOutgoingFrame((UCHAR *)pBcnHdr+FrameLen, &TmpLen,
	                          9,                         RalinkSpecificIe,
	                          END_OF_ARGS);
	        FrameLen += TmpLen;
    	}
    }
#endif /* AGGREGATION_SUPPORT */

    // Since FrameLen may change, update TXD
#ifdef CONFIG_64BIT
    RTMPWriteTxDescriptor(pAd, pTxD, CIPHER_NONE, 0, 0, FALSE, FALSE, TRUE, SHORT_RETRY, IFS_BACKOFF,
        pAd->PortCfg.MlmeRate, FrameLen, QID_MGMT, PTYPE_SPECIAL|PSUBTYPE_MGMT, NULL, NULL, pAd->PortCfg.MBSSID[apidx].bWmmCapable, FALSE, FALSE, FALSE, NULL);
#else
    RTMPWriteTxDescriptor(pAd, pTxD, CIPHER_NONE, 0, 0, FALSE, FALSE, TRUE, SHORT_RETRY, IFS_BACKOFF, 
        pAd->PortCfg.MlmeRate, FrameLen, QID_MGMT, PTYPE_SPECIAL|PSUBTYPE_MGMT, NULL, NULL, pAd->PortCfg.MBSSID[apidx].bWmmCapable, FALSE, FALSE, FALSE);
#endif // CONFIG_64BIT //

    // move BEACON TXD and frame content to on-chip memory
	if (pAd->PortCfg.MBSSID[apidx].MSSIDDev->flags & IFF_UP)
	{
    ptr = (PUCHAR)&pAd->PortCfg.MBSSID[apidx].BeaconTxD;
    for (i=0; i<TXINFO_SIZE; i++)  // 24-byte TXINFO field
    {
        RTMP_IO_WRITE8(pAd, HW_BEACON_BASE0 + (apidx * HW_BEACON_OFFSET) + i, *ptr);
        ptr ++;
    }

    // start right after the 24-byte TXINFO field
    ptr = pAd->PortCfg.MBSSID[apidx].BeaconBuf;
//    for (i=0; i< pAd->PortCfg.MBSSID[apidx].BeaconTxD.DataByteCnt; i++)
	 for (i=0; i< FrameLen; i++)
    {
        RTMP_IO_WRITE8(pAd, HW_BEACON_BASE0 + (apidx * HW_BEACON_OFFSET) + TXINFO_SIZE + i, *ptr);
        ptr ++;
    }
	}
	else
	{
	    for (i=0; i<TXINFO_SIZE; i++)  // 24-byte TXINFO field
	        RTMP_IO_WRITE8(pAd, HW_BEACON_BASE0 + (apidx * HW_BEACON_OFFSET) + i, 0x00);
	
	    // start right after the 24-byte TXINFO field
		for (i=0; i< FrameLen; i++)
	        RTMP_IO_WRITE8(pAd, HW_BEACON_BASE0 + (apidx * HW_BEACON_OFFSET) + TXINFO_SIZE + i, 0x00);
	}

    //
    // if DTIM, then move backlogged bcast/mcast frames from PSQ to TXQ whenever DtimCount==0
#if 1    
    // NOTE: This updated BEACON frame will be sent at "next" TBTT instead of at cureent TBTT. The reason is
    //       because ASIC already fetch the BEACON content down to TX FIFO before driver can make any
    //       modification. To compenstate this effect, the actual time to deilver PSQ frames will be
    //       at the time that we wrapping around DtimCount from 0 to DtimPeriod-1
    if (((pAd->Mlme.DtimCount + 1) == pAd->Mlme.DtimPeriod) &&  PreTrafficInd == 1)
#else
    if (pAd->Mlme.DtimCount == 0)
#endif
    {
        PQUEUE_ENTRY    pEntry;
#ifndef THREAD_ISR
        UMAXLONG		IrqFlags;
        UMAXLONG		IrqFlags2;
#endif
		BOOLEAN			bPS=FALSE;

#ifndef THREAD_ISR
        RTMP_SEM_LOCK(&pAd->MacTabLock, IrqFlags);
        RTMP_SEM_LOCK(&pAd->TxSwQueueLock, IrqFlags2);
#else
		/* 
		 *  Protect TxSwQueue0 & McastPsQueue 
		 *  because use them in interrupt context 
		 */		
//		RTMP_IRQ_LOCK(IrqFlags);
#endif	/* THREAD_ISR */

        while (pAd->MacTab.McastPsQueue.Head)
        {
			bPS = TRUE;
			if (pAd->TxSwQueue[QID_AC_BE].Number <= (MAX_PACKETS_IN_QUEUE + (MAX_PACKETS_IN_MCAST_PS_QUEUE>>1))) {
	            pEntry = RemoveHeadQueue(&pAd->MacTab.McastPsQueue);
                          if(pAd->MacTab.McastPsQueue.Number){
            	                      RTMP_SET_PACKET_MOREDATA((struct sk_buff *) pEntry, TRUE);
                          }
    	        InsertHeadQueue(&pAd->TxSwQueue[QID_AC_BE], pEntry);				
			} else {
				break;
			}
        }

        DBGPRINT(RT_DEBUG_INFO, "DTIM=%d/%d, tx mcast/bcast out...\n",pAd->Mlme.DtimCount,pAd->Mlme.DtimPeriod);
        pAd->MacTab.PsQIdleCount = 0;

		if (pAd->MacTab.McastPsQueue.Number == 0 ) {
        	pAd->PortCfg.MBSSID[apidx].TimBitmap &= ~BIT32[0]; // clear MCAST/BCAST backlog bit			
		}

#ifndef THREAD_ISR
        RTMP_SEM_UNLOCK(&pAd->TxSwQueueLock, IrqFlags2);
        RTMP_SEM_UNLOCK(&pAd->MacTabLock, IrqFlags);
#else
//		RTMP_IRQ_UNLOCK(IrqFlags);
#endif /* THREAD_ISR */

        // Dequeue outgoing framea from TxSwQueue0..3 queue and process it
		if (bPS == TRUE) 
		{
	        RTMPDeQueuePacket(pAd, FALSE, TRUE);
		}
    }
}

#ifdef APCLI_SUPPORT
/*
    ==========================================================================

	Routine	Description:
		Disconnect current BSSID

	Arguments:
		pAd				- Pointer to our adapter
		ApCliIdx		- Which ApCli interface		
	Return Value:		
		None

	Note:

	==========================================================================
*/
VOID ApCliLinkDown(
    IN 	PRTMP_ADAPTER 	pAd,
    IN	UCHAR			ifIndex)
{
    if (ifIndex < MAX_APCLI_ENTRY)
    {
    	DBGPRINT(RT_DEBUG_TRACE, "!!! APCLI LINK DOWN - IF(apcli%d)!!!\n", ifIndex);
    }
    else
    {
    	DBGPRINT(RT_DEBUG_TRACE, "!!! ERROR : APCLI LINK DOWN - IF(apcli%d)!!!\n", ifIndex);	
    	return;
    }	
    	
    if (pAd->ApCliTab.ApCliEntry[ifIndex].Valid == FALSE)	
		return;

	// Reset WPA-PSK state. Only reset when supplicant enabled
	if (pAd->ApCliTab.ApCliEntry[ifIndex].WpaState != SS_NOTUSE)
	{
		pAd->ApCliTab.ApCliEntry[ifIndex].WpaState = SS_START;
		// Clear Replay counter
		NdisZeroMemory(pAd->ApCliTab.ApCliEntry[ifIndex].ReplayCounter, 8);
	}
	
	// If link down, need to remove pairwise key for this apcli link
	if (pAd->ApCliTab.ApCliEntry[ifIndex].AuthMode >= Ndis802_11AuthModeWPA)
	{
		NdisZeroMemory(&pAd->ApCliTab.ApCliEntry[ifIndex].PairwiseKey, sizeof(CIPHER_KEY)); 	
		AsicRemovePairwiseKeyEntry(pAd, APCLI_PAIRWISE_KEY_OFFSET);
	}
	
	if (pAd->PortCfg.RadarDetect.IEEE80211H == TRUE)
	{
		AsicSendCommandToMcu(pAd, 0x60, 0xff, 0x01, 0x01);   // To Make sure command is sucessful
	}

	// 802.1x port control
	pAd->ApCliTab.ApCliEntry[ifIndex].PortSecured = WPA_802_1X_PORT_NOT_SECURED;
	pAd->ApCliTab.ApCliEntry[ifIndex].MicErrCnt = 0;

}

/*
    ==========================================================================
    Description:
    ==========================================================================
*/
VOID AssocParmFill(
    IN PRTMP_ADAPTER pAd, 
    IN OUT MLME_ASSOC_REQ_STRUCT *AssocReq, 
    IN PUCHAR                     pAddr, 
    IN USHORT                     CapabilityInfo, 
    IN ULONG                      Timeout, 
    IN USHORT                     ListenIntv) 
{
    COPY_MAC_ADDR(AssocReq->Addr, pAddr);
    // Add mask to support 802.11b mode only
    AssocReq->CapabilityInfo = CapabilityInfo & SUPPORTED_CAPABILITY_INFO; // not cf-pollable, not cf-poll-request
    AssocReq->Timeout = Timeout;
    AssocReq->ListenIntv = ListenIntv;
}

/*
    ==========================================================================
    Description:
    ==========================================================================
*/
VOID DisassocParmFill(
    IN PRTMP_ADAPTER pAd, 
    IN OUT MLME_DISASSOC_REQ_STRUCT *DisassocReq, 
    IN PUCHAR pAddr, 
    IN USHORT Reason) 
{
    COPY_MAC_ADDR(DisassocReq->Addr, pAddr);
    DisassocReq->Reason = Reason;
}

/*
    ==========================================================================
    Description:
    ==========================================================================
*/
VOID JoinParmFill(
    IN PRTMP_ADAPTER pAd, 
    IN OUT MLME_JOIN_REQ_STRUCT *JoinReq, 
	PUCHAR Bssid,
	UCHAR SsidLen,
	PCHAR Ssid)
{
	if(JoinReq == NULL)
		return;

	if(Bssid != NULL)
		COPY_MAC_ADDR(JoinReq->Bssid, Bssid);

	if(Ssid != NULL && SsidLen != 0)
		NdisMoveMemory(JoinReq->Ssid, Ssid, SsidLen);

	return;
}
#endif /* APCLI_SUPPORT */

