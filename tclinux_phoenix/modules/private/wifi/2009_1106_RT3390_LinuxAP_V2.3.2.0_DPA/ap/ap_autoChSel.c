/****************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ****************************************************************************
 
    Module Name:
 
    Abstract:
*/


#include "rt_config.h"
#include "ap_autoChSel.h"

#if defined(TCSUPPORT_C1_CUC) || defined(TCSUPPORT_CZ_OTE)
#define MAX_AUTO_CH	11
#endif

extern UCHAR ZeroSsid[32];

static inline INT GetABandChOffset(
	IN INT Channel)
{
#ifdef A_BAND_SUPPORT
	if ((Channel == 36) || (Channel == 44) || (Channel == 52) || (Channel == 60) || (Channel == 100) || (Channel == 108) ||
	    (Channel == 116) || (Channel == 124) || (Channel == 132) || (Channel == 149) || (Channel == 157))
	{
		return 1;
	}
	else if ((Channel == 40) || (Channel == 48) || (Channel == 56) || (Channel == 64) || (Channel == 104) || (Channel == 112) ||
			(Channel == 120) || (Channel == 128) || (Channel == 136) || (Channel == 153) || (Channel == 161))
	{
		return -1;
	}
#endif // A_BAND_SUPPORT //
	return 0;
}

ULONG AutoChBssSearchWithSSID(
	IN PRTMP_ADAPTER pAd,
	IN PUCHAR Bssid,
	IN PUCHAR pSsid,
	IN UCHAR SsidLen,
	IN UCHAR Channel)
{
	UCHAR i;
	PBSSINFO pBssInfoTab = pAd->pBssInfoTab;

	if(pBssInfoTab == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("pAd->pBssInfoTab equal NULL.\n"));
		return (ULONG)BSS_NOT_FOUND;
	}

	for (i = 0; i < pBssInfoTab->BssNr; i++) 
	{
		if ((((pBssInfoTab->BssEntry[i].Channel <= 14) && (Channel <= 14)) ||
			((pBssInfoTab->BssEntry[i].Channel > 14) && (Channel > 14))) &&
			MAC_ADDR_EQUAL(&(pBssInfoTab->BssEntry[i].Bssid), Bssid) &&
			(SSID_EQUAL(pSsid, SsidLen, pBssInfoTab->BssEntry[i].Ssid, pBssInfoTab->BssEntry[i].SsidLen) ||
			(NdisEqualMemory(pSsid, ZeroSsid, SsidLen)) || 
			(NdisEqualMemory(pBssInfoTab->BssEntry[i].Ssid, ZeroSsid, pBssInfoTab->BssEntry[i].SsidLen))))
		{ 
			return i;
		}
	}
	return (ULONG)BSS_NOT_FOUND;
}

static inline VOID AutoChBssEntrySet(
	OUT BSSENTRY *pBss, 
	IN PUCHAR pBssid, 
	IN CHAR Ssid[], 
	IN UCHAR SsidLen, 
	IN UCHAR Channel,
	IN UCHAR ExtChOffset,
	IN CHAR Rssi)
{
	COPY_MAC_ADDR(pBss->Bssid, pBssid);
	if (SsidLen > 0)
	{
		// For hidden SSID AP, it might send beacon with SSID len equal to 0
		// Or send beacon /probe response with SSID len matching real SSID length,
		// but SSID is all zero. such as "00-00-00-00" with length 4.
		// We have to prevent this case overwrite correct table
		if (NdisEqualMemory(Ssid, ZeroSsid, SsidLen) == 0)
		{
			NdisMoveMemory(pBss->Ssid, Ssid, SsidLen);
			pBss->SsidLen = SsidLen;
		}
	}

	pBss->Channel = Channel;
	pBss->ExtChOffset = ExtChOffset;
	pBss->Rssi = Rssi;

	return;
}

static inline VOID AutoChBssTableReset(
	IN PRTMP_ADAPTER pAd)
{
	if (pAd->pBssInfoTab)
		NdisZeroMemory(pAd->pBssInfoTab, sizeof(BSSINFO));
	else
		DBGPRINT(RT_DEBUG_ERROR, ("pAd->pBssInfoTab equal NULL.\n"));

	return;
}

#ifdef AUTO_CH_SELECT_ENHANCE
static VOID ChannelInfoReset(
	IN PRTMP_ADAPTER pAd)
{
	if (pAd->pChannelInfo)
		NdisZeroMemory(pAd->pChannelInfo, sizeof(CHANNELINFO));
	else
		DBGPRINT(RT_DEBUG_ERROR, ("pAd->pChannelInfo equal NULL.\n"));

	return;
}
#endif // AUTO_CH_SELECT_ENHANCE //

static inline VOID UpdateChannelInfo(
	IN PRTMP_ADAPTER pAd,
	IN int ch,
	IN UINT32 FalseCCA)
{
	if(pAd->pChannelInfo != NULL)
	{
		pAd->pChannelInfo->FalseCCA[ch] = FalseCCA;
		pAd->pChannelInfo->chanbusytime[ch] = QBSS_LoadBusyTimeGet(pAd); /* QLOAD ALARM */
	}
	else
		DBGPRINT(RT_DEBUG_ERROR, ("pAd->pChannelInfo equal NULL.\n"));

	return;
}

static inline INT GetChIdx(
	IN PRTMP_ADAPTER pAd,
	IN UCHAR Channel)
{
	INT Idx;

	Idx = -1;
	for (Idx = 0; Idx < MAX_NUM_OF_CHANNELS; Idx++)
	{
		if (Channel == pAd->ChannelList[Idx].Channel)
			break;
	}

	return Idx;
}

static inline BOOLEAN BW40_ChannelCheck(
	IN UCHAR ch)
{
	INT i;
	BOOLEAN result = FALSE;
	UCHAR BW40_CH[] = {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108,
						112, 116, 120, 124, 128, 132, 136, 149, 153, 157, 161};
	UCHAR BW40ChNum = sizeof(BW40_CH) / sizeof(UCHAR);

	for (i=0; i<BW40ChNum; i++)
	{
		if (ch == BW40_CH[i])
		{
			result = TRUE;
			break;
		}
	}

	return result;
}

static inline INT SelectClearChannel(
	IN PRTMP_ADAPTER pAd,
	IN BOOLEAN IsABand)
{
	#define CCA_THRESHOLD (100)

	PBSSINFO pBssInfoTab = pAd->pBssInfoTab;
	PCHANNELINFO pChannelInfo = pAd->pChannelInfo;
	INT i;
	INT ch, channel_idx;
	BSSENTRY *pBss;
	UINT32 min_dirty, min_falsecca;
	int candidate_ch;

	if(pBssInfoTab == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("pAd->pBssInfoTab equal NULL.\n"));
		return (FirstChannel(pAd));
	}

	if(pChannelInfo == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("pAd->pChannelInfo equal NULL.\n"));
		return (FirstChannel(pAd));
	}

	for (i = 0; i < pBssInfoTab->BssNr; i++)
	{
		pBss = &(pBssInfoTab->BssEntry[i]);
		channel_idx = GetChIdx(pAd, pBss->Channel);
		if (channel_idx < 0 )
			continue;

		if (pChannelInfo->max_rssi[channel_idx] < pBss->Rssi)
			pChannelInfo->max_rssi[channel_idx] = pBss->Rssi;

		if (pBss->Rssi >= RSSI_TO_DBM_OFFSET-50)
		{
			/* high signal >= -50 dbm */
			pChannelInfo->dirtyness[channel_idx] += 50;
		}
		else if (pBss->Rssi <= RSSI_TO_DBM_OFFSET-80)
		{
			/* low signal <= -80 dbm */
			pChannelInfo->dirtyness[channel_idx] += 30;
		}
		else
		{
			/* mid signal -50 ~ -80 dbm */
			pChannelInfo->dirtyness[channel_idx] += 40;
		}

		pChannelInfo->dirtyness[channel_idx] += 40;

		{
			INT BelowBound;
			INT AboveBound;
			INT loop;

			switch(pBss->ExtChOffset)
			{
				case EXTCHA_ABOVE:
					BelowBound = IsABand ? 1 : 4;
					AboveBound = IsABand ? 2 : 8;
					break;

				case EXTCHA_BELOW:
					BelowBound = IsABand ? 2 : 8;
					AboveBound = IsABand ? 1 : 4;
					break;

				default:
					BelowBound = IsABand ? 1 : 4;
					AboveBound = IsABand ? 1 : 4;
					break;
			}

			for (loop = (channel_idx+1); loop <= (channel_idx+AboveBound); loop++)
			{
				if (loop >= MAX_NUM_OF_CHANNELS)
					continue;

				if (pAd->ChannelList[loop].Channel - pAd->ChannelList[loop-1].Channel > 4)
					continue;

				pChannelInfo->dirtyness[loop] +=
					((9 - (loop - channel_idx)) * 4);
			}

			for (loop=(channel_idx-1); loop >= (channel_idx-BelowBound); loop--)
			{
				if (loop < 0)
					continue;

				if (pAd->ChannelList[loop+1].Channel - pAd->ChannelList[loop].Channel > 4)
					continue;

				pChannelInfo->dirtyness[loop] +=
					((9 - (channel_idx - loop)) * 4);
			}
		}

		DBGPRINT(RT_DEBUG_TRACE,(" ch%d bssid=%02x:%02x:%02x:%02x:%02x:%02x\n",
			pBss->Channel, pBss->Bssid[0], pBss->Bssid[1], pBss->Bssid[2], pBss->Bssid[3], pBss->Bssid[4], pBss->Bssid[5]));
	}
			
	DBGPRINT(RT_DEBUG_TRACE, ("=====================================================\n"));
	for (i=0; i<pAd->ChannelListNum; i++)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("Channel %d : max RSSI = %ld, Dirty = %ld, False CCA = %u, Busy Time = %u\n",
					pAd->ChannelList[i].Channel,
					pChannelInfo->max_rssi[i], pChannelInfo->dirtyness[i],
					pChannelInfo->FalseCCA[i], pChannelInfo->chanbusytime[i]));
	}
	DBGPRINT(RT_DEBUG_TRACE, ("=====================================================\n"));

	min_dirty = min_falsecca = 0xFFFFFFFF;
	candidate_ch = 0;

	/* 
	 * Rule 1. Pick up a good channel that False_CCA =< CCA_THRESHOLD 
	 *		   by dirtyness
	 */
	candidate_ch = -1;
	for (i = 0; i < pAd->ChannelListNum; i++)
	{
		if (pChannelInfo->FalseCCA[i] <= CCA_THRESHOLD)
		{
			UINT32 dirtyness = pChannelInfo->dirtyness[i];
			ch = pAd->ChannelList[i].Channel;

			/* QLOAD ALARM */
			/* when busy time of a channel > threshold, skip it */
			/* TODO: Use weight for different references to do channel selection */
			if (QBSS_LoadIsBusyTimeAccepted(pAd,
									pChannelInfo->chanbusytime[i]) == FALSE)
			{
				/* check next one */
				continue;
			}

#ifdef DOT11_N_SUPPORT
			/*
				User require 40MHz Bandwidth.
				In the case, ignor all channel
				doesn't support 40MHz Bandwidth.
			*/
			if ((pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth == BW_40)
				&& (IsABand && (GetABandChOffset(ch) == 0)))
				continue;

			/*
				Need to Consider the dirtyness of extending channel
				in 40 MHz bandwidth channel.
			*/
			if (pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth == BW_40)
			{
				if (IsABand)
				{
					if (((i + GetABandChOffset(ch)) >=0)
						&& ((i + GetABandChOffset(ch)) < pAd->ChannelListNum))
					{
						INT ChOffsetIdx = i + GetABandChOffset(ch);
						dirtyness += pChannelInfo->dirtyness[ChOffsetIdx];
					}
				}
				else
				{
					if (pAd->CommonCfg.AddHTInfo.AddHtInfo.ExtChanOffset == EXTCHA_ABOVE)
					{
						if (((i + 4) >=0) && ((i + 4) < pAd->ChannelListNum))
						{
							dirtyness += pChannelInfo->dirtyness[i + 4];
						}
					}
					else if (pAd->CommonCfg.AddHTInfo.AddHtInfo.ExtChanOffset == EXTCHA_BELOW)
					{
						if (((i - 4) >=0) && ((i - 4) < pAd->ChannelListNum))
						{
							dirtyness += pChannelInfo->dirtyness[i - 4];
						}
					}
				}
			}
#endif // DOT11_N_SUPPORT //

			if ((min_dirty > dirtyness))
			{
				min_dirty = dirtyness;
				candidate_ch = i;
			}
		}
	}

	if (candidate_ch >= 0)
	{
		ch = pAd->ChannelList[candidate_ch].Channel;
		DBGPRINT(RT_DEBUG_TRACE, ("Rule 1 ==> Select Channel %d\n", ch));
		return ch;
	}

	/*
	 * Rule 2. Pick up a good channel that False_CCA > CCA_THRESHOLD 
	 *		   by FalseCCA
	 */
	candidate_ch = -1;
	for (i = 0; i < pAd->ChannelListNum; i++)
	{
		if (pChannelInfo->FalseCCA[i] > CCA_THRESHOLD)
		{
			UINT32 falsecca = pChannelInfo->FalseCCA[i] + pChannelInfo->dirtyness[i];
			ch = pAd->ChannelList[i].Channel;

#ifdef DOT11_N_SUPPORT
			if ((pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth == BW_40)
				&& (GetABandChOffset(ch) == 0))
				continue;
#endif // DOT11_N_SUPPORT //

			if ((GetABandChOffset(ch) != 0)
					&& ((i + GetABandChOffset(ch)) >=0)
					&& ((i + GetABandChOffset(ch)) < pAd->ChannelListNum))
			{
				INT ChOffsetIdx = i + GetABandChOffset(ch);
				falsecca += (pChannelInfo->FalseCCA[ChOffsetIdx] +
							pChannelInfo->dirtyness[ChOffsetIdx]);
			}

			/* QLOAD ALARM */
			/* when busy time of a channel > threshold, skip it */
			/* TODO: Use weight for different references to do channel selection */
			if (QBSS_LoadIsBusyTimeAccepted(pAd,
									pChannelInfo->chanbusytime[i]) == FALSE)
			{
				/* check next one */
				continue;
			}

			if ((min_falsecca > falsecca))
			{
				min_falsecca = falsecca;
				candidate_ch = i;
			}
		}
	}

	if (candidate_ch >= 0)
	{
		ch = pAd->ChannelList[candidate_ch].Channel;
		DBGPRINT(RT_DEBUG_TRACE, ("Rule 2 ==> Select Channel %d\n", ch));
		return	ch;
	}
	
	return (pAd->ChannelList[RandomByte(pAd)%pAd->ChannelListNum].Channel);
}

ULONG AutoChBssInsertEntry(
	IN PRTMP_ADAPTER pAd,
	IN PUCHAR pBssid,
	IN CHAR Ssid[],
	IN UCHAR SsidLen, 
	IN UCHAR ChannelNo,
	IN UCHAR ExtChOffset,
	IN CHAR Rssi)
{
	ULONG	Idx;
	PBSSINFO pBssInfoTab = pAd->pBssInfoTab;

	if(pBssInfoTab == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("pAd->pBssInfoTab equal NULL.\n"));
		return BSS_NOT_FOUND;
	}

	Idx = AutoChBssSearchWithSSID(pAd, pBssid, (PUCHAR)Ssid, SsidLen, ChannelNo);
	if (Idx == BSS_NOT_FOUND) 
	{
		if (pBssInfoTab->BssNr >= MAX_LEN_OF_BSS_TABLE)
			return BSS_NOT_FOUND;
		Idx = pBssInfoTab->BssNr;
		AutoChBssEntrySet(&pBssInfoTab->BssEntry[Idx], pBssid, Ssid, SsidLen,
							ChannelNo, ExtChOffset, Rssi);
		pBssInfoTab->BssNr++;
	} 
	else
	{
		AutoChBssEntrySet(&pBssInfoTab->BssEntry[Idx], pBssid, Ssid, SsidLen,
							ChannelNo, ExtChOffset, Rssi);
	}

	return Idx;
}


void AutoChBssTableInit(
	IN PRTMP_ADAPTER pAd)
{
	pAd->pBssInfoTab = (PBSSINFO)kmalloc(sizeof(BSSINFO), GFP_ATOMIC);
	if (pAd->pBssInfoTab)
		NdisZeroMemory(pAd->pBssInfoTab, sizeof(BSSINFO));
	else
		DBGPRINT(RT_DEBUG_ERROR, ("%s Fail to alloc memory for pAd->pBssInfoTab", __FUNCTION__));

	return;
}

void ChannelInfoInit(
	IN PRTMP_ADAPTER pAd)
{
	pAd->pChannelInfo = (PCHANNELINFO)kmalloc(sizeof(CHANNELINFO), GFP_ATOMIC);
	if (pAd->pChannelInfo)
		NdisZeroMemory(pAd->pChannelInfo, sizeof(CHANNELINFO));
	else
		DBGPRINT(RT_DEBUG_ERROR, ("%s Fail to alloc memory for pAd->pChannelInfo", __FUNCTION__));


	return;
}

void AutoChBssTableDestroy(
	IN PRTMP_ADAPTER pAd)
{
	if (pAd->pBssInfoTab)
	{
		kfree(pAd->pBssInfoTab);
		pAd->pBssInfoTab = NULL;
	}

	return;
}

void ChannelInfoDestroy(
	IN PRTMP_ADAPTER pAd)
{
	if (pAd->pChannelInfo)
	{
		kfree(pAd->pChannelInfo);
		pAd->pChannelInfo = NULL;
	}

	return;
}

UCHAR RandomChannel(
	IN PRTMP_ADAPTER pAd)
{
	INT i;
	UCHAR cnt = 0;
	UCHAR ch;
	BOOLEAN bFindIt = FALSE;

	while(TRUE)
	{
		cnt++;
		ch = pAd->ChannelList[RandomByte(pAd)%pAd->ChannelListNum].Channel;

		if (ch == 0)
			ch = FirstChannel(pAd);

		// have no avaiable channel now. force pick first channel here.
		if (cnt == pAd->ChannelListNum)
		{
			ch = FirstChannel(pAd);
			break;
		}

#ifdef A_BAND_SUPPORT
		if (RadarChannelCheck(pAd, ch))
			continue;
#endif // A_BAND_SUPPORT //

		for (i=0; i<pAd->ChannelListNum; i++)
		{
			if (pAd->ChannelList[i].Channel == ch)
			{
				if (pAd->ChannelList[i].RemainingTimeForUse == 0)
					bFindIt = TRUE;
				
				break;
			}
		}
		
		if (bFindIt == TRUE)
			break;
	};

	return ch;
}

#ifdef AUTO_CH_SELECT_ENHANCE
UCHAR New_APAutoSelectChannel(
	IN PRTMP_ADAPTER pAd,
	IN BOOLEAN Optimal)
{
	INT i;
	UCHAR ch=1;
	BOOLEAN IsABand;

#ifdef VENDOR_FEATURE3_SUPPORT // Allocate dynamicaly
	//AutoChBssTableInit(pAd);
	pAd->pBssInfoTab = (PBSSINFO)kmalloc(sizeof(BSSINFO), GFP_ATOMIC);
	if (pAd->pBssInfoTab)
		NdisZeroMemory(pAd->pBssInfoTab, sizeof(BSSINFO));
	else
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s Fail to alloc memory for pAd->pBssInfoTab", __FUNCTION__));
		return ch;
	}
	
	//ChannelInfoInit(pAd);
	pAd->pChannelInfo = (PCHANNELINFO)kmalloc(sizeof(CHANNELINFO), GFP_ATOMIC);
	if (pAd->pChannelInfo)
		NdisZeroMemory(pAd->pChannelInfo, sizeof(CHANNELINFO));
	else
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s Fail to alloc memory for pAd->pChannelInfo", __FUNCTION__));
		kfree(pAd->pBssInfoTab);
		pAd->pBssInfoTab=NULL;
		return ch;
	}
#endif // VENDOR_FEATURE3_SUPPORT //

	IsABand = ((pAd->CommonCfg.PhyMode == PHY_11A)
		|| (pAd->CommonCfg.PhyMode == PHY_11ABG_MIXED)
#ifdef DOT11_N_SUPPORT
		|| (pAd->CommonCfg.PhyMode == PHY_11ABGN_MIXED)
		|| (pAd->CommonCfg.PhyMode == PHY_11AN_MIXED)
		|| (pAd->CommonCfg.PhyMode == PHY_11AGN_MIXED)
#endif // DOT11_N_SUPPORT //
		) ? TRUE : FALSE;
#ifdef A_BAND_SUPPORT
	if ((Optimal != TRUE) && (IsABand == TRUE))
	{
		if (pAd->CommonCfg.bIEEE80211H)
		{
			ch = RandomChannel(pAd);
		}
		else
		{
			ch = pAd->ChannelList[RandomByte(pAd)%pAd->ChannelListNum].Channel;
			if (ch == 0)
				ch = FirstChannel(pAd);
		}
		DBGPRINT(RT_DEBUG_TRACE,("1.APAutoSelectChannel pick up ch#%d\n",ch));
	}
	else
#endif // A_BAND_SUPPORT //
	{
		/* reset bss table */
		AutoChBssTableReset(pAd);

		/* clear Channel Info */
		ChannelInfoReset(pAd);

		for (i=0; i<pAd->ChannelListNum; i++)
		{
			RX_STA_CNT1_STRUC RxStaCnt1;

			ch = pAd->ChannelList[i].Channel;
			AsicSwitchChannel(pAd, ch, TRUE);
			AsicLockChannel(pAd, ch);
			pAd->ApCfg.AutoChannel_Channel = ch;

			if (QLOAD_DOES_ALARM_OCCUR(pAd)) /* QLOAD ALARM */
			{
				/* ever alarm from QLOAD module */
				OS_WAIT(400); // wait for 400 ms at each channel.
			}
			else
			{
				OS_WAIT(200); // wait for 200 ms at each channel.
			}

			RTMP_IO_READ32(pAd, RX_STA_CNT1, &RxStaCnt1.word);
			pAd->RalinkCounters.OneSecFalseCCACnt += RxStaCnt1.field.FalseCca;
			UpdateChannelInfo(pAd, i, RxStaCnt1.field.FalseCca);
		}
		ch = SelectClearChannel(pAd, IsABand);

#if defined(TCSUPPORT_C1_CUC) || defined(TCSUPPORT_CZ_OTE)
		/*Unicom test plan is request,auto must be belong 1~11*/	
		if(ch > MAX_AUTO_CH){
 			ch = (pAd->ChannelList[RandomByte(pAd)%pAd->ChannelListNum].Channel) % MAX_AUTO_CH;
			if(ch == 0)
				ch = MAX_AUTO_CH;
			/*printk("Random again get ch:%d\r\n", ch);*/
		}
		/*printk("Auto channel:%d\r\n",ch);*/	
#endif
		DBGPRINT(RT_DEBUG_TRACE, ("ApAutoSelectChannel pick up ch#%d\n",ch));
	}

#ifdef VENDOR_FEATURE3_SUPPORT
		// Free BssTab & ChannelInfo tables.
		AutoChBssTableDestroy(pAd);
		ChannelInfoDestroy(pAd);
#endif // VENDOR_FEATURE3_SUPPORT //

	return ch;
}
#else
/* 
	==========================================================================
	Description:
		This routine is called at initialization. It returns a channel number
		that complies to regulation domain and less interference with current
		enviornment.
	Return:
		ch -  channel number that
	NOTE:
		the retrun channel number is guaranteed to comply to current regulation
		domain that recorded in pAd->CommonCfg.CountryRegion
	==========================================================================
 */
UCHAR APAutoSelectChannel(
	IN PRTMP_ADAPTER pAd,
	IN BOOLEAN Optimal)
{
	UCHAR ch = 0, i;
	UCHAR dirtyness[MAX_NUM_OF_CHANNELS+1], dirty;
	ULONG ApCnt[MAX_NUM_OF_CHANNELS+1];
	UINT32 chan_busy_time[MAX_NUM_OF_CHANNELS+1];
	//UINT32 FalseCca = 0, FcsError = 0; // remove sine them never be used to determine channel.
	BOOLEAN IsABand;

	// passive scan channel 1-14. collect statistics
	NdisZeroMemory(dirtyness, MAX_NUM_OF_CHANNELS+1);
	// In the autochannel select case. AP didn't get channel yet.
	// So have no way to determine which Band AP used by channel number.

	IsABand = ((pAd->CommonCfg.PhyMode == PHY_11A)
		|| (pAd->CommonCfg.PhyMode == PHY_11ABG_MIXED)
#ifdef DOT11_N_SUPPORT
		|| (pAd->CommonCfg.PhyMode == PHY_11ABGN_MIXED)
		|| (pAd->CommonCfg.PhyMode == PHY_11AN_MIXED)
		|| (pAd->CommonCfg.PhyMode == PHY_11AGN_MIXED)
#endif // DOT11_N_SUPPORT //
		) ? TRUE : FALSE;
#ifdef A_BAND_SUPPORT
	if ((Optimal != TRUE) && (IsABand == TRUE))
	{
		if (pAd->CommonCfg.bIEEE80211H)
		{
			ch = RandomChannel(pAd);
		}
		else
		{
			ch = pAd->ChannelList[RandomByte(pAd)%pAd->ChannelListNum].Channel;
			if (ch == 0)
				ch = FirstChannel(pAd);
		}
		DBGPRINT(RT_DEBUG_TRACE,("1.APAutoSelectChannel pick up ch#%d\n",ch));
		return ch;
	}
	else
#endif // A_BAND_SUPPORT //		
	{
		for (i=0; i<pAd->ChannelListNum; i++)
		{
			ch = pAd->ChannelList[i].Channel;
			//APSwitchChannel(pAd, ch);
			AsicSwitchChannel(pAd, ch, TRUE);
			AsicLockChannel(pAd, ch);
			pAd->Counters8023.GoodReceives = 0;
			pAd->Counters8023.RxErrors = 0;
			pAd->ApCfg.ApCnt = 0;
			pAd->ApCfg.AutoChannel_Channel = ch;
			ApCnt[i]=0;

			if (QLOAD_DOES_ALARM_OCCUR(pAd)) /* QLOAD ALARM */
			{
				/* ever alarm from QLOAD module */
				OS_WAIT(1000); // wait for 400 ms at each channel.
			}
			else
			{
				OS_WAIT(200); // wait for 200 ms at each channel.
			}

			ApCnt[i] = pAd->ApCfg.ApCnt;
			chan_busy_time[i] = QBSS_LoadBusyTimeGet(pAd); /* QLOAD ALARM */

			// remove sine them never be used to determine channel.
			//RTMP_IO_READ32(pAd, RX_STA_CNT1, &FalseCca);
			//FalseCca &= 0x0000ffff;
			//RTMP_IO_READ32(pAd, RX_STA_CNT0, &FcsError);
			//FcsError &= 0x0000ffff;
		}

		for (i=0; i<pAd->ChannelListNum; i++)
		{
			//if (pAd->Counters8023.GoodReceives)
			if (ApCnt[i] > 0)
			{
				INT ll;
				dirtyness[i] += 30;
				if (!IsABand)
				{
					for (ll=i; ll<=(i+4); ll++)
					{
						if (ll < MAX_NUM_OF_CHANNELS)
							dirtyness[ll]++;
					}

					for (ll=i; ll>=(i-4); ll--)
					{
						if (ll >= 0)
							dirtyness[ll]++;
					}
				}
			}
			//DBGPRINT(RT_DEBUG_TRACE,("Msleep at ch#%d to collect RX=%lu, RSSI=%d, CRC error =%d, False CCA =%d\n", 
			//	ch, pAd->Counters8023.GoodReceives, max_rssi[i] - pAd->BbpRssiToDbmDelta, FcsError, FalseCca));
			DBGPRINT(RT_DEBUG_TRACE,("Msleep at ch#%d to collect RX=%lu, ApCnt=%ld\n", 
				pAd->ChannelList[i].Channel, pAd->Counters8023.GoodReceives, ApCnt[i]));
		}
		pAd->ApCfg.AutoChannel_Channel = 0;

		DBGPRINT(RT_DEBUG_TRACE,("Dirtyness = "));
		for (i = 0; i < pAd->ChannelListNum; i++)
		{
			if (i!=0 && i%4 == 0) DBGPRINT(RT_DEBUG_TRACE, ("-"));
			DBGPRINT(RT_DEBUG_TRACE,("%d.", dirtyness[i]));
		}
		DBGPRINT(RT_DEBUG_TRACE, ("\n"));

		// RULE 1. pick up a good channel that no one used
		for (i = 0; i < pAd->ChannelListNum; i++)
		{
			/* QLOAD ALARM */
			if (QBSS_LoadIsBusyTimeAccepted(pAd,
									chan_busy_time[i]) == FALSE)
			{
				continue;
			}

			if (dirtyness[i] == 0) break;
		}
		if (i < pAd->ChannelListNum)
		{
			DBGPRINT(RT_DEBUG_TRACE,("APAutoSelectChannel pick up ch#%d\n", pAd->ChannelList[i].Channel));
			return pAd->ChannelList[i].Channel;
		}

		// RULE 2. if not available, then co-use a channel that's no interference (dirtyness=10)
		// RULE 3. if not available, then co-use a channel that has minimum interference (dirtyness=11,12)
		for (dirty = 30; dirty <= 32; dirty++)
		{
			UCHAR candidate[MAX_NUM_OF_CHANNELS+1], candidate_num;
			UCHAR final_channel = 0;
			ULONG min_ApCnt = 0;
			
			candidate_num = 0;
			NdisZeroMemory(candidate, MAX_NUM_OF_CHANNELS+1);
			for (i = 0; i<pAd->ChannelListNum; i++)
			{
				if (dirtyness[i] == dirty) 
				{ 
					candidate[i]=1; 
					candidate_num++; 
				}
			}
			// if there's more than 1 candidate, pick up the channel with minimum RSSI
			if (candidate_num)
			{
				for (i = 0; i < pAd->ChannelListNum; i++)
				{
					/* QLOAD ALARM */
					/* when busy time of a channel > threshold, skip it */
					/* TODO: Use weight for different references to do channel selection */
					if (QBSS_LoadIsBusyTimeAccepted(pAd,
											chan_busy_time[i]) == FALSE)
					{
						/* check next one */
						continue;
					}

					if (candidate[i] && (ApCnt[i] < min_ApCnt))
					{
						if((pAd->CommonCfg.RegTransmitSetting.field.BW == BW_40)
							&& (BW40_ChannelCheck(pAd->ChannelList[i].Channel) == FALSE))
							continue;
						final_channel = pAd->ChannelList[i].Channel;
						min_ApCnt = ApCnt[i];
					}
				}
				if (final_channel != 0)
				{
					DBGPRINT(RT_DEBUG_TRACE,("APAutoSelectChannel pick up ch#%d\n",final_channel));
					return final_channel;
				}
			}
		}

		// RULE 4. still not available, pick up the random channel
		ch = (pAd->ChannelList[RandomByte(pAd)%pAd->ChannelListNum].Channel);
		DBGPRINT(RT_DEBUG_TRACE,("APAutoSelectChannel pick up ch#%d\n",ch));
		return ch;
	}
}
#endif // AUTO_CH_SELECT_ENHANCE //
