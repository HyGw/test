/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    cmm_radar.c

    Abstract:
    CS/DFS common functions.

    Revision History:
    Who       When            What
    --------  ----------      ----------------------------------------------
*/

#if defined(DFS_SUPPORT) || defined(CARRIER_DETECTION_SUPPORT)

#include "rt_config.h"

INT	Set_RadarShow_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
#ifdef DFS_SUPPORT
	int i;
	PRADAR_DETECT_STRUCT pRadarDetect = &pAd->CommonCfg.RadarDetect;
	PDFS_PROGRAM_PARAM pDfsProgramParam = &pRadarDetect->DfsProgramParam;
	PDFS_SW_DETECT_PARAM pDfsSwParam = &pRadarDetect->DfsSwParam;

	if (pRadarDetect->dfs_func >= HARDWARE_DFS_V1)
	{
		UINT8 idx;
		printk("DFSUseTasklet = %d\n", pRadarDetect->use_tasklet);
		printk("McuRadarDebug = %x\n", (unsigned int)pRadarDetect->McuRadarDebug);
		printk("PollTime = %d\n", pRadarDetect->PollTime);
		printk("ChEnable = %d (0x%x)\n", pDfsProgramParam->ChEnable, pDfsProgramParam->ChEnable);
		printk("DeltaDelay = %d\n", pDfsProgramParam->DeltaDelay);
		printk("Fcc5Thrd = %d\n", pDfsSwParam->fcc_5_threshold);
		printk("PeriodErr = %d\n", pDfsSwParam->dfs_period_err);
		printk("MaxPeriod = %d\n", (unsigned int)pDfsSwParam->dfs_max_period);
		printk("Ch0LErr = %d\n", pDfsSwParam->dfs_width_ch0_err_L);
		printk("Ch0HErr = %d\n", pDfsSwParam->dfs_width_ch0_err_H);
		printk("Ch1Shift = %d\n", pDfsSwParam->dfs_width_diff_ch1_Shift);
		printk("Ch2Shift = %d\n", pDfsSwParam->dfs_width_diff_ch2_Shift);
		/*printk("CeSwCheck = %d\n", pAd->CommonCfg.ce_sw_check);*/
		/*printk("CEStagCheck = %d\n", pAd->CommonCfg.ce_staggered_check);*/
		/*printk("HWDFSDisabled = %d\n", pAd->CommonCfg.hw_dfs_disabled);*/
		/*printk("WidthDiffShift = %d\n", pDfsSwParam->dfs_width_diff_Shift);*/
		printk("DfsRssiHigh = %d\n", pRadarDetect->DfsRssiHigh);
		printk("DfsRssiLow = %d\n", pRadarDetect->DfsRssiLow);

		printk("CheckLoop = %d\n", pDfsSwParam->dfs_check_loop);
		printk("DeclareThres = %d\n", pDfsSwParam->dfs_declare_thres);
		for (i =0; i < pRadarDetect->fdf_num; i++)
		{
			printk("ChBusyThrd[%d] = %d\n", i, pRadarDetect->ch_busy_threshold[i]);
			printk("RssiThrd[%d] = %d\n", i, pRadarDetect->rssi_threshold[i]);
		}
		for (idx=0; idx < pAd->chipCap.DfsEngineNum; idx++)
			printk("sw_idx[%u] = %u\n", idx, pDfsSwParam->sw_idx[idx]);
		for (idx=0; idx < pAd->chipCap.DfsEngineNum; idx++)
			printk("hw_idx[%u] = %u\n", idx, pDfsSwParam->hw_idx[idx]);
#ifdef DFS_DEBUG
		printk("Total[0] = %lu\n", pDfsSwParam->TotalEntries[0]);
		printk("Total[1] = %lu\n", pDfsSwParam->TotalEntries[1]);
		printk("Total[2] = %lu\n", pDfsSwParam->TotalEntries[2]);
		printk("Total[3] = %lu\n", pDfsSwParam->TotalEntries[3]);

		pDfsSwParam->TotalEntries[0] = pDfsSwParam->TotalEntries[1] = pDfsSwParam->TotalEntries[2] = pDfsSwParam->TotalEntries[3] = 0;

		printk("T_Matched_2 = %lu\n", pDfsSwParam->T_Matched_2);
		printk("T_Matched_3 = %lu\n", pDfsSwParam->T_Matched_3);
		printk("T_Matched_4 = %lu\n", pDfsSwParam->T_Matched_4);
		printk("T_Matched_5 = %lu\n", pDfsSwParam->T_Matched_5);		
#endif /* DFS_DEBUG */
	}

	printk("pRadarDetect->ChMovingTime = %d\n", pRadarDetect->ChMovingTime);
	printk("pRadarDetect->RDMode = %d\n", pRadarDetect->RDMode);
#endif /* DFS_SUPPORT */

#ifdef CARRIER_DETECTION_SUPPORT
	printk("pAd->CommonCfg.CarrierDetect.CD_State = %d\n", pAd->CommonCfg.CarrierDetect.CD_State);
	printk("pAd->CommonCfg.CarrierDetect.criteria = %d\n", pAd->CommonCfg.CarrierDetect.criteria);
	printk("pAd->CommonCfg.CarrierDetect.Delta = %d\n", pAd->CommonCfg.CarrierDetect.delta);
	printk("pAd->CommonCfg.CarrierDetect.DivFlag = %d\n", pAd->CommonCfg.CarrierDetect.div_flag);
	printk("pAd->CommonCfg.CarrierDetect.Threshold = %d(0x%x)\n", pAd->CommonCfg.CarrierDetect.threshold, pAd->CommonCfg.CarrierDetect.threshold);
#endif /* CARRIER_DETECTION_SUPPORT */

	return TRUE;
}

/*
	========================================================================

	Routine Description:
		Radar channel check routine

	Arguments:
		pAd 	Pointer to our adapter

	Return Value:
		TRUE	need to do radar detect
		FALSE	need not to do radar detect

	========================================================================
*/
BOOLEAN RadarChannelCheck(
	IN PRTMP_ADAPTER	pAd,
	IN UCHAR			Ch)
{
	INT 	i;
	BOOLEAN result = FALSE;

	for (i=0; i<pAd->ChannelListNum; i++)
	{
		if (Ch == pAd->ChannelList[i].Channel)
		{
			result = pAd->ChannelList[i].DfsReq;
			break;
		}
	}

	return result;
}

/*
       ========================================================================
       Routine Description:
               Control CCK_MRC Status
       Arguments:
               pAd     Pointer to our adapter
       Return Value:

       ========================================================================
*/
VOID CckMrcStatusCtrl(IN PRTMP_ADAPTER pAd)
{
#ifdef RT5592
       UCHAR bbp = 0, bCckMrc = 0;
       RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R95, &bbp);
       bCckMrc = (bbp >> 7);
       if (bCckMrc)
       {
			if (pAd->CommonCfg.Channel>14
#ifdef CARRIER_DETECTION_SUPPORT
				|| (pAd->CommonCfg.CarrierDetect.Enable == TRUE &&
					pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth == BW_20)
#endif /* CARRIER_DETECTION_SUPPORT */
			)
			{
				/* Disable CCK_MRC*/
				bbp &= ~(1 << 7);
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R95, bbp);
			}
       }
       else
       {
			if (pAd->CommonCfg.Channel<=14)
			{
#ifdef CARRIER_DETECTION_SUPPORT
				if (pAd->CommonCfg.CarrierDetect.Enable == FALSE ||
					pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth == BW_40)
#endif /* CARRIER_DETECTION_SUPPORT */
				{
					/* Enable CCK_MRC */
					bbp |= (1 << 7);
					RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R95, bbp);
				}
			}
       }
#endif /* RT5592 */
}


/*
       ========================================================================
       Routine Description:
               Enhance DFS/CS when using GLRT.
       Arguments:
               pAd     Pointer to our adapter
       Return Value:

       ========================================================================
*/
VOID RadarGLRTCompensate(IN PRTMP_ADAPTER pAd)
{
#ifdef RT5592


	if (pAd->CommonCfg.bIEEE80211H == 1 
#ifdef CARRIER_DETECTION_SUPPORT
		|| pAd->CommonCfg.CarrierDetect.Enable == TRUE
#endif /* CARRIER_DETECTION_SUPPORT */
		)
	{
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x91);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x24);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x95);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x2D);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x99);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x40);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x9A);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x3E);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x9B);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x42);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x9C);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x3D);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x9D);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x40);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0xA1);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x2F);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0xA5);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x2A);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0xB5);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x40);

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0xCE);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x43);
	}
#endif /* RT5592 */
}
#endif /*defined(DFS_SUPPORT) || defined(CARRIER_DETECTION_SUPPORT) */

