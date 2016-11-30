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
	dfs_mcu.c
 
    Abstract:
 
    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------
 */
 
#include "rt_config.h"

#ifdef CONFIG_AP_SUPPORT	
/* Radar detection and carrier detection for 2880-SW-MCU*/

#if defined(DFS_SUPPORT) || defined(CARRIER_DETECTION_SUPPORT)
#ifdef DFS_SOFTWARE_SUPPORT
static void TimerCB_Radar(PRTMP_ADAPTER pAd);
#endif /* DFS_SOFTWARE_SUPPORT */
#ifdef CARRIER_DETECTION_SUPPORT
static void TimerCB_Carrier(PRTMP_ADAPTER pAd);
#endif /* CARRIER_DETECTION_SUPPORT */
#ifdef DFS_HARDWARE_SUPPORT
void NewTimerCB_Radar(IN PRTMP_ADAPTER pAd);
#endif /* DFS_HARDWARE_SUPPORT */

void TimerCB(unsigned long data)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)data;
	if (pAd->CommonCfg.McuRadarProtection == 1)
	{
		return;
	}


#ifdef CARRIER_DETECTION_SUPPORT
	if (pAd->CommonCfg.McuRadarCmd & CARRIER_DETECTION)
		TimerCB_Carrier(pAd);
#endif /* CARRIER_DETECTION_SUPPORT */

}

#ifdef CARRIER_DETECTION_SUPPORT

static void TimerCB_Carrier(PRTMP_ADAPTER pAd)
{
	ULONG Value;

	if (pAd->CommonCfg.McuRadarEvent & RADAR_EVENT_RADAR_DETECTING)
	{
		pAd->CommonCfg.McuCarrierTick++;
		return;
	}
	else
		pAd->CommonCfg.McuCarrierTick++;


	if (pAd->CommonCfg.McuCarrierTick >= pAd->CommonCfg.McuCarrierPeriod)
	{
		/* Start of period*/
		pAd->CommonCfg.McuCarrierTick = 0;
		pAd->CommonCfg.McuCarrierDetectCount = 0;
		pAd->CommonCfg.McuCarrierState = WAIT_CTS_BEING_SENT;
		pAd->CommonCfg.McuRadarEvent |= RADAR_EVENT_CARRIER_DETECTING;

		if ((pAd->CommonCfg.McuCarrierCtsProtect == 1)
			|| (isCarrierDetectExist(pAd))
			)
		{
			/* no CTS protect*/
			pAd->CommonCfg.McuCarrierState = DO_DETECTION;
			MCURadarDetect(pAd);
		}
		else if (pAd->CommonCfg.McuCarrierCtsProtect == 2)
		{
			/* kick CTS*/
			RTMP_IO_WRITE32(pAd, PBF_CTRL, 0x80);
		}
		return;
	}

	if (pAd->CommonCfg.McuCarrierState == WAIT_CTS_BEING_SENT)
	{
		/* check event or timeout*/
		if (pAd->CommonCfg.McuRadarEvent & RADAR_EVENT_CTS_CARRIER_SENT)
		{
			pAd->CommonCfg.McuRadarEvent &= ~RADAR_EVENT_CTS_CARRIER_SENT;
			pAd->CommonCfg.McuCarrierDetectCount = 1;
			pAd->CommonCfg.McuCarrierState = DO_DETECTION;

			return;
		}
		
		if (pAd->CommonCfg.McuCarrierDetectCount >= (5-1)) /* timeout 5ms, start from 0*/
		{
			pAd->CommonCfg.McuCarrierState = FREE_FOR_TX; /* timeout 5ms, give up*/
			pAd->CommonCfg.McuRadarEvent &= ~(RADAR_EVENT_CARRIER_DETECTING);

		}
		
		pAd->CommonCfg.McuCarrierDetectCount++;
	}
	else if (pAd->CommonCfg.McuCarrierState == DO_DETECTION)
	{
		if (pAd->CommonCfg.McuCarrierDetectCount >= (pAd->CommonCfg.McuCarrierDetectPeriod - 1))
		{
			UCHAR BBPR115;

			BBP_IO_READ8_BY_REG_ID(pAd, BBP_R115, &BBPR115);

			CarrierDetectionCheck(pAd);

			pAd->CommonCfg.McuCarrierState = FREE_FOR_TX;
			pAd->CommonCfg.McuRadarEvent &= ~(RADAR_EVENT_CARRIER_DETECTING);

			RTMP_IO_WRITE32(pAd, TX_RTS_CFG, pAd->CommonCfg.OldRtsRetryLimit);
			
			/* Restore BBP*/
			AsicBBPWriteWithRxChain(pAd, BBP_R66, pAd->CommonCfg._R66, RX_CHAIN_ALL);
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R69, pAd->CommonCfg._R69);
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R70, pAd->CommonCfg._R70);
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R73, pAd->CommonCfg._R73);

#ifdef DFS_DEBUG
#endif /* DFS_DEBUG */
			/* EnableNormalTx*/
			RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &Value);
			Value |= 0x04;
			RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, Value);

			

		}
		pAd->CommonCfg.McuCarrierDetectCount++;
	}
	else /* FREE_FOR_TX*/
	{
		if (pAd->CommonCfg.McuRadarEvent & RADAR_EVENT_CTS_CARRIER_SENT)
		{
			pAd->CommonCfg.McuRadarEvent &= ~RADAR_EVENT_CTS_CARRIER_SENT;
			pAd->CommonCfg.McuCarrierDetectCount = 1;

			RTMP_IO_WRITE32(pAd, TX_RTS_CFG, pAd->CommonCfg.OldRtsRetryLimit);
			
			/* Restore BBP*/
			AsicBBPWriteWithRxChain(pAd, BBP_R66, pAd->CommonCfg._R66, RX_CHAIN_ALL);
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R69, pAd->CommonCfg._R69);
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R70, pAd->CommonCfg._R70);
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R73, pAd->CommonCfg._R73);


#ifdef DFS_DEBUG
#endif /* DFS_DEBUG */
			/* EnableNormalTx*/
			RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &Value);
			Value |= 0x04;
			RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, Value);
		}
	}	
}
#endif /* CARRIER_DETECTION_SUPPORT */

void MCURadarDetect(PRTMP_ADAPTER pAd)
{
	ULONG Value;
	UCHAR bbp = 0;
	
	BBP_IO_READ8_BY_REG_ID(pAd, BBP_R113, &bbp);
	if (bbp & 0x20)
		return;

	/*DisableNormalTx*/
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &Value);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0);
	{
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0xc);
	}
	Value &= (ULONG)~(0x04);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, Value);
				
#ifdef DFS_DEBUG
#endif /* DFS_DEBUG */


	RTMP_IO_READ32(pAd, TX_RTS_CFG, &pAd->CommonCfg.OldRtsRetryLimit);
	RTMP_IO_WRITE32(pAd, TX_RTS_CFG, 0);
	
	/*change BBP now*/
	if (pAd->CommonCfg.McuRadarEvent & RADAR_EVENT_CARRIER_DETECTING)
	{
		UCHAR BBPR4 = 0;
		UCHAR r66_val;
		
		/* Clear False CCA*/
		RTMP_IO_READ32(pAd, RX_STA_CNT1, &Value);
		
		/* save old value*/
		AsicBBPReadWithRxChain(pAd, BBP_R66, &pAd->CommonCfg._R66, RX_CHAIN_0);
		BBP_IO_READ8_BY_REG_ID(pAd, BBP_R69, &pAd->CommonCfg._R69);
		BBP_IO_READ8_BY_REG_ID(pAd, BBP_R70, &pAd->CommonCfg._R70);
		BBP_IO_READ8_BY_REG_ID(pAd, BBP_R73, &pAd->CommonCfg._R73);

		BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &BBPR4);
		if (BBPR4 & 0x18) /* BW 40*/
		{
			if (pAd->CommonCfg.Channel > 14)
				r66_val = 0x50;	/* BW 40, A band*/
			else
				r66_val = 0x40;	/* BW 40, G band*/
			AsicBBPWriteWithRxChain(pAd, BBP_R66, r66_val, RX_CHAIN_ALL);
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R69, 0x30);
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R70, 0x30);
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R73, 0x08);
		}
		else /* BW 20*/
		{
			if (pAd->CommonCfg.Channel > 14)
				r66_val = 0x60;	/* BW 20, A band*/
			else
				r66_val = 0x30;	/* BW 20, G band*/
			AsicBBPWriteWithRxChain(pAd, BBP_R66, r66_val, RX_CHAIN_ALL);
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R69, 0x28);
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R70, 0x28);
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R73, 0x08);
		}
		
	}
	else /* RADAR_DETECTION*/
	{
		/* save old value */
		BBP_IO_READ8_BY_REG_ID(pAd, BBP_R65, &pAd->CommonCfg._R65);
		AsicBBPReadWithRxChain(pAd, BBP_R66, &pAd->CommonCfg._R66, RX_CHAIN_0);

		BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R65, pAd->CommonCfg.R65);
		AsicBBPWriteWithRxChain(pAd, BBP_R66, pAd->CommonCfg.R66, RX_CHAIN_ALL);
		BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R113, 0x21);
	}		
	return;
}

#endif /* #if defined(DFS_SUPPORT) || defined(CARRIER_DETECTION_SUPPORT) */
#endif /* CONFIG_AP_SUPPORT */
