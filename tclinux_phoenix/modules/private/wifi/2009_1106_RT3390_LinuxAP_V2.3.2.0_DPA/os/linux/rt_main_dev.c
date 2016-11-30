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
    rt_main_dev.c

    Abstract:
    Create and register network interface.

    Revision History:
    Who         When            What
    --------    ----------      ----------------------------------------------
*/

#include "rt_config.h"


#ifdef CONFIG_APSTA_MIXED_SUPPORT
UINT32 CW_MAX_IN_BITS;
#endif // CONFIG_APSTA_MIXED_SUPPORT //

/*---------------------------------------------------------------------*/
/* Private Variables Used                                              */
/*---------------------------------------------------------------------*/

PSTRING mac = "";		   // default 00:00:00:00:00:00
PSTRING hostname = "";		   // default CMPC
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,12)
MODULE_PARM (mac, "s");
#else
module_param (mac, charp, 0);
#endif
MODULE_PARM_DESC (mac, "rt28xx: wireless mac addr");

#ifdef TC_SOC //Add polling mode
#define POLLING_MODE_DETECT_INTV  30
extern void os_TCIfQuery (unsigned short query_id, void* result, void* result2);
#endif

/*---------------------------------------------------------------------*/
/* Prototypes of Functions Used                                        */
/*---------------------------------------------------------------------*/

// public function prototype
int rt28xx_close(IN struct net_device *net_dev);
int rt28xx_open(struct net_device *net_dev);

// private function prototype
static INT rt28xx_send_packets(IN struct sk_buff *skb_p, IN struct net_device *net_dev);


static struct net_device_stats *RT28xx_get_ether_stats(
    IN  struct net_device *net_dev);

/*
========================================================================
Routine Description:
    Close raxx interface.

Arguments:
	*net_dev			the raxx interface pointer

Return Value:
    0					Open OK
	otherwise			Open Fail

Note:
	1. if open fail, kernel will not call the close function.
	2. Free memory for
		(1) Mlme Memory Handler:		MlmeHalt()
		(2) TX & RX:					RTMPFreeTxRxRingMemory()
		(3) BA Reordering: 				ba_reordering_resource_release()
========================================================================
*/
int MainVirtualIF_close(IN struct net_device *net_dev)
{
    RTMP_ADAPTER *pAd = NULL;

	GET_PAD_FROM_NET_DEV(pAd, net_dev);	

	// Sanity check for pAd
	if (pAd == NULL)
		return 0; // close ok

	netif_carrier_off(pAd->net_dev);
	netif_stop_queue(pAd->net_dev);

#ifdef CONFIG_AP_SUPPORT
	pAd->ApCfg.MBSSID[MAIN_MBSSID].bBcnSntReq = FALSE;

	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		// kick out all STAs behind the bss.
		MbssKickOutStas(pAd, MAIN_MBSSID, REASON_DISASSOC_INACTIVE);
	}

	APMakeAllBssBeacon(pAd);
	APUpdateAllBeaconFrame(pAd);
#endif // CONFIG_AP_SUPPORT //




	VIRTUAL_IF_DOWN(pAd);

	RT_MOD_DEC_USE_COUNT();

	return 0; // close ok
}

/*
========================================================================
Routine Description:
    Open raxx interface.

Arguments:
	*net_dev			the raxx interface pointer

Return Value:
    0					Open OK
	otherwise			Open Fail

Note:
	1. if open fail, kernel will not call the close function.
	2. Free memory for
		(1) Mlme Memory Handler:		MlmeHalt()
		(2) TX & RX:					RTMPFreeTxRxRingMemory()
		(3) BA Reordering: 				ba_reordering_resource_release()
========================================================================
*/
int MainVirtualIF_open(IN struct net_device *net_dev)
{
    RTMP_ADAPTER *pAd = NULL;

	GET_PAD_FROM_NET_DEV(pAd, net_dev);	

	// Sanity check for pAd
	if (pAd == NULL)
		return 0; // close ok

#ifdef CONFIG_AP_SUPPORT
	pAd->ApCfg.MBSSID[MAIN_MBSSID].bBcnSntReq = TRUE;
#endif // CONFIG_AP_SUPPORT //
	if (VIRTUAL_IF_UP(pAd) != 0)
		return -1;

	// increase MODULE use count
	RT_MOD_INC_USE_COUNT();

	netif_start_queue(net_dev);
	netif_carrier_on(net_dev);
	netif_wake_queue(net_dev);

	return 0;
}

/*
========================================================================
Routine Description:
    Close raxx interface.

Arguments:
	*net_dev			the raxx interface pointer

Return Value:
    0					Open OK
	otherwise			Open Fail

Note:
	1. if open fail, kernel will not call the close function.
	2. Free memory for
		(1) Mlme Memory Handler:		MlmeHalt()
		(2) TX & RX:					RTMPFreeTxRxRingMemory()
		(3) BA Reordering: 				ba_reordering_resource_release()
========================================================================
*/
int rt28xx_close(IN PNET_DEV dev)
{
	struct net_device * net_dev = (struct net_device *)dev;
    RTMP_ADAPTER	*pAd = NULL;
	BOOLEAN 		Cancelled;
	UINT32			i = 0;

	GET_PAD_FROM_NET_DEV(pAd, net_dev);	

	DBGPRINT(RT_DEBUG_TRACE, ("===> rt28xx_close\n"));
#ifdef TC_SOC // add polling mode
	pAd->PollingModeDetectRunning = FALSE;
	del_timer_sync(&pAd->PollingModeDetect);
#endif

#ifdef CONFIG_AP_SUPPORT
#ifdef BG_FT_SUPPORT
	BG_FTPH_Remove();
#endif // BG_FT_SUPPORT //
#endif // CONFIG_AP_SUPPORT //

	Cancelled = FALSE;
	// Sanity check for pAd
	if (pAd == NULL)
		return 0; // close ok

#ifdef WMM_ACM_SUPPORT
	/* must call first */
	ACMP_Release(pAd);
#endif // WMM_ACM_SUPPORT //


#ifdef WDS_SUPPORT
	WdsDown(pAd);
#endif // WDS_SUPPORT //


	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);

	for (i = 0 ; i < NUM_OF_TX_RING; i++)
	{
		while (pAd->DeQueueRunning[i] == TRUE)
		{
			DBGPRINT(RT_DEBUG_TRACE, ("Waiting for TxQueue[%d] done..........\n", i));
			RTMPusecDelay(1000);
		}
	}
	

#ifdef CONFIG_AP_SUPPORT

	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{

#ifdef DOT11N_DRAFT3
		if (pAd->CommonCfg.Bss2040CoexistFlag & BSS_2040_COEXIST_TIMER_FIRED)
		{
			RTMPCancelTimer(&pAd->CommonCfg.Bss2040CoexistTimer, &Cancelled);
			pAd->CommonCfg.Bss2040CoexistFlag  = 0;
		}
#endif // DOT11N_DRAFT3 //

		// PeriodicTimer already been canceled by MlmeHalt() API.
		//RTMPCancelTimer(&pAd->PeriodicTimer,	&Cancelled);
	}
#endif // CONFIG_AP_SUPPORT //

	// Stop Mlme state machine
	MlmeHalt(pAd);
	
	// Close net tasklets
	RtmpNetTaskExit(pAd);



#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
#ifdef MAT_SUPPORT
		MATEngineExit(pAd);
#endif // MAT_SUPPORT //

#ifdef CLIENT_WDS
		CliWds_ProxyTabDestory(pAd);
#endif // CLIENT_WDS //
		// Shutdown Access Point function, release all related resources 
		APShutdown(pAd);

#ifndef VENDOR_FEATURE3_SUPPORT
		// Free BssTab & ChannelInfo tabbles.
		AutoChBssTableDestroy(pAd);
		ChannelInfoDestroy(pAd);
#endif // VENDOR_FEATURE3_SUPPORT //
	}
#endif // CONFIG_AP_SUPPORT //

	MeasureReqTabExit(pAd);
	TpcReqTabExit(pAd);

#ifdef WSC_INCLUDED
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		WscStop(pAd, FALSE, &pAd->ApCfg.MBSSID[MAIN_MBSSID].WscControl);
#endif // CONFIG_AP_SUPPORT //

#ifdef OLD_DH_KEY
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	    	WSC_VFREE_KEY_MEM(pAd->ApCfg.MBSSID[0].WscControl.pPubKeyMem, pAd->ApCfg.MBSSID[0].WscControl.pSecKeyMem);
#endif // CONFIG_AP_SUPPORT //
#endif // OLD_DH_KEY //

	/* WSC hardware push button function 0811 */
	WSC_HDR_BTN_Stop(pAd);
#endif // WSC_INCLUDED //

	// Close kernel threads
	RtmpMgmtTaskExit(pAd);

#ifdef RTMP_MAC_PCI
	{
			BOOLEAN brc;
			//	ULONG			Value;

			if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_ACTIVE))
			{
				RTMP_ASIC_INTERRUPT_DISABLE(pAd);
			}

			// Receive packets to clear DMA index after disable interrupt. 
			//RTMPHandleRxDoneInterrupt(pAd);
			// put to radio off to save power when driver unload.  After radiooff, can't write /read register.  So need to finish all 
			// register access before Radio off.


			brc=RT28xxPciAsicRadioOff(pAd, RTMP_HALT, 0);

//In  solution 3 of 3090F, the bPCIclkOff will be set to TRUE after calling RT28xxPciAsicRadioOff
#ifdef PCIE_PS_SUPPORT
			pAd->bPCIclkOff = FALSE;    
#endif // PCIE_PS_SUPPORT //

			if (brc==FALSE)
			{
				DBGPRINT(RT_DEBUG_ERROR,("%s call RT28xxPciAsicRadioOff fail !!\n", __FUNCTION__)); 
			}
	}
	

/*
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_ACTIVE))
	{
		RTMP_ASIC_INTERRUPT_DISABLE(pAd);
	}

	// Disable Rx, register value supposed will remain after reset
	NICIssueReset(pAd);
*/
#endif // RTMP_MAC_PCI //

	// Free IRQ
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE))
	{
#ifdef RTMP_MAC_PCI
		// Deregister interrupt function
		RtmpOSIRQRelease(net_dev);
#endif // RTMP_MAC_PCI //
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE);
	}

	// Free Ring or USB buffers
	RTMPFreeTxRxRingMemory(pAd);

	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);

#ifdef DOT11_N_SUPPORT
	// Free BA reorder resource
	ba_reordering_resource_release(pAd);
#endif // DOT11_N_SUPPORT //
	

	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_START_UP);

/*+++Modify by woody to solve the bulk fail+++*/

	DBGPRINT(RT_DEBUG_TRACE, ("<=== rt28xx_close\n"));
	return 0; // close ok
} /* End of rt28xx_close */

#ifdef TC_SOC
extern void tc3162_enable_irq(unsigned int irq);
extern void tc3162_disable_irq(unsigned int irq);
/*
    ========================================================================

    Routine Description:
       Trendchip DMT Trainning status detect

    Arguments:
        data                     Point to RTMP_ADAPTER

    Return Value:
        NONE

    Note: 

    ========================================================================
*/

VOID PeriodicPollingModeDetect(
	IN	unsigned long data)
{
  UCHAR modem_status=0; 
  RTMP_ADAPTER *pAd = (RTMP_ADAPTER *)data;
  POS_COOKIE _pObj = (POS_COOKIE)(pAd->OS_Cookie);

  os_TCIfQuery(0x0002, &modem_status, NULL);
  
  if ((modem_status==0x08)||(modem_status==0x10))
  {
	// disable enterrupt
	tc3162_disable_irq(_pObj->pci_dev->irq);
	schedule_work(&pAd->PollingDataBH);
	//slow down the POLLING MODE DETECT while the dmt in wait init state
  	pAd->PollingModeDetect.expires = jiffies + POLLING_MODE_DETECT_INTV;
  	mod_timer(&pAd->PollingModeDetect,pAd->PollingModeDetect.expires);
  	pAd->PollingModeDetectRunning = TRUE;
	return;
  }
  else{
 	tc3162_enable_irq(_pObj->pci_dev->irq);
  }
  pAd->PollingModeDetect.expires = jiffies + POLLING_MODE_DETECT_INTV;
  mod_timer(&pAd->PollingModeDetect,pAd->PollingModeDetect.expires);
  pAd->PollingModeDetectRunning = TRUE;
  return;
}

VOID PollingModeIsr(struct work_struct *work)
{
  	RTMP_ADAPTER *pAd = container_of(work, RTMP_ADAPTER, PollingDataBH);
	struct net_device *net_dev = pAd->net_dev;
	rt2860_interrupt(0, net_dev);
}
#endif

/*
========================================================================
Routine Description:
    Open raxx interface.

Arguments:
	*net_dev			the raxx interface pointer

Return Value:
    0					Open OK
	otherwise			Open Fail

Note:
========================================================================
*/
int rt28xx_open(IN PNET_DEV dev)
{				 
	struct net_device * net_dev = (struct net_device *)dev;
	PRTMP_ADAPTER pAd = NULL;
	int retval = 0;
 	//POS_COOKIE pObj;
	#ifdef TC_SOC
	pcie_init();
	#endif
	
	GET_PAD_FROM_NET_DEV(pAd, net_dev);	

	// Sanity check for pAd
	if (pAd == NULL)
	{
		/* if 1st open fail, pAd will be free;
		   So the net_dev->priv will be NULL in 2rd open */
		return -1;
	}

#ifdef CONFIG_APSTA_MIXED_SUPPORT
	if (pAd->OpMode == OPMODE_AP)
	{
		CW_MAX_IN_BITS = 6;
	}
	else if (pAd->OpMode == OPMODE_STA)
	{
		CW_MAX_IN_BITS = 10;
	}
#endif // CONFIG_APSTA_MIXED_SUPPORT //

#if WIRELESS_EXT >= 12
	if (net_dev->priv_flags == INT_MAIN) 
	{
#ifdef CONFIG_APSTA_MIXED_SUPPORT
		if (pAd->OpMode == OPMODE_AP)
			net_dev->wireless_handlers = (struct iw_handler_def *) &rt28xx_ap_iw_handler_def;
#endif // CONFIG_APSTA_MIXED_SUPPORT //
	}
#endif // WIRELESS_EXT >= 12 //

	// Request interrupt service routine for PCI device
	// register the interrupt routine with the os
	RtmpOSIRQRequest(net_dev);


	// Init IRQ parameters stored in pAd
	RTMP_IRQ_INIT(pAd);
	
	// Chip & other init
	if (rt28xx_init(pAd, mac, hostname) == FALSE)
		goto err;


	// Enable Interrupt
	RTMP_IRQ_ENABLE(pAd);

	// Now Enable RxTx
	RTMPEnableRxTx(pAd);
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_START_UP);

	{
	UINT32 reg = 0;
	RTMP_IO_READ32(pAd, 0x1300, &reg);  // clear garbage interrupts
	printk("0x1300 = %08x\n", reg);
	}

	{
//	u32 reg;
//	UINT8  byte;
//	u16 tmp;

//	RTMP_IO_READ32(pAd, XIFS_TIME_CFG, &reg);

//	tmp = 0x0805;
//	reg  = (reg & 0xffff0000) | tmp;
//	RTMP_IO_WRITE32(pAd, XIFS_TIME_CFG, reg);

	}



#ifdef CONFIG_AP_SUPPORT
#ifdef BG_FT_SUPPORT
	BG_FTPH_Init();
#endif // BG_FT_SUPPORT //
#endif // CONFIG_AP_SUPPORT //

#ifdef TC_SOC // add polling mode
	if (pAd->PollingModeDetectRunning == FALSE)
	{
	    printk("jiffies=%08lx, POLLING_MODE_DETECT_INTV=%d\r\n", jiffies, POLLING_MODE_DETECT_INTV);
	    pAd->PollingModeDetect.expires = jiffies + POLLING_MODE_DETECT_INTV;
	    add_timer(&pAd->PollingModeDetect);
	}
	// init a BH task here
	INIT_WORK(&pAd->PollingDataBH, PollingModeIsr);
#endif
	return (retval);

err:
//+++Add by shiang, move from rt28xx_init() to here.
	RtmpOSIRQRelease(net_dev);
//---Add by shiang, move from rt28xx_init() to here.
	return (-1);
} /* End of rt28xx_open */


PNET_DEV RtmpPhyNetDevInit(
	IN RTMP_ADAPTER *pAd,
	IN RTMP_OS_NETDEV_OP_HOOK *pNetDevHook)
{
	struct net_device	*net_dev = NULL;
//	NDIS_STATUS		Status;
	
	net_dev = RtmpOSNetDevCreate(pAd, INT_MAIN, 0, sizeof(PRTMP_ADAPTER), INF_MAIN_DEV_NAME);
	if (net_dev == NULL)
	{
		printk("RtmpPhyNetDevInit(): creation failed for main physical net device!\n");
		return NULL;
	}

	NdisZeroMemory((unsigned char *)pNetDevHook, sizeof(RTMP_OS_NETDEV_OP_HOOK));
	pNetDevHook->open = MainVirtualIF_open;
	pNetDevHook->stop = MainVirtualIF_close;
	pNetDevHook->xmit = rt28xx_send_packets;
#ifdef IKANOS_VX_1X0
	pNetDevHook->xmit = IKANOS_DataFramesTx;
#endif // IKANOS_VX_1X0 //
	pNetDevHook->ioctl = rt28xx_ioctl;
	pNetDevHook->priv_flags = INT_MAIN;
	pNetDevHook->get_stats = RT28xx_get_ether_stats;

	pNetDevHook->needProtcted = FALSE;

	RTMP_OS_NETDEV_SET_PRIV(net_dev, pAd);
	pAd->net_dev = net_dev;
	
#ifdef CONFIG_AP_SUPPORT
	pAd->ApCfg.MBSSID[MAIN_MBSSID].MSSIDDev = net_dev;
#endif // CONFIG_AP_SUPPORT //


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24)
	SET_MODULE_OWNER(net_dev);
#endif 

	netif_stop_queue(net_dev);

	return net_dev;
	
}


/*
========================================================================
Routine Description:
    The entry point for Linux kernel sent packet to our driver.

Arguments:
    sk_buff *skb		the pointer refer to a sk_buffer.

Return Value:
    0					

Note:
	This function is the entry point of Tx Path for Os delivery packet to 
	our driver. You only can put OS-depened & STA/AP common handle procedures 
	in here.
========================================================================
*/
__IMEM int rt28xx_packet_xmit(struct sk_buff *skb)
{
	struct net_device *net_dev = skb->dev;
	PRTMP_ADAPTER pAd = NULL;
	int status = 0;
	PNDIS_PACKET pPacket = (PNDIS_PACKET) skb;

	GET_PAD_FROM_NET_DEV(pAd, net_dev);	

	/* RT2870STA does this in RTMPSendPackets() */
#ifdef RALINK_ATE
	if (ATE_ON(pAd))
	{
		RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_RESOURCES);
		return 0;
	}
#endif // RALINK_ATE //


        // EapolStart size is 18
	if (skb->len < 14)
	{
		//printk("bad packet size: %d\n", pkt->len);
		hex_dump("bad packet", skb->data, skb->len);
		RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_FAILURE);
		goto done;
	}



	RTMP_SET_PACKET_5VT(pPacket, 0);
//	MiniportMMRequest(pAd, pkt->data, pkt->len);
#ifdef CONFIG_5VT_ENHANCE
    if (*(int*)(skb->cb) == BRIDGE_TAG) {
		RTMP_SET_PACKET_5VT(pPacket, 1);
    }
#endif


#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		APSendPackets((NDIS_HANDLE)pAd, (PPNDIS_PACKET) &pPacket, 1);
#endif // CONFIG_AP_SUPPORT //


	status = 0;
done:
			   
	return status;
}


/*
========================================================================
Routine Description:
    Send a packet to WLAN.

Arguments:
    skb_p           points to our adapter
    dev_p           which WLAN network interface

Return Value:
    0: transmit successfully
    otherwise: transmit fail

Note:
========================================================================
*/
static int rt28xx_send_packets(
	IN struct sk_buff 		*skb_p, 
	IN struct net_device 	*net_dev)
{
	RTMP_ADAPTER *pAd = NULL;

	GET_PAD_FROM_NET_DEV(pAd, net_dev);	

	if (!(net_dev->flags & IFF_UP))
	{
		RELEASE_NDIS_PACKET(pAd, (PNDIS_PACKET)skb_p, NDIS_STATUS_FAILURE);
		return 0;
	}

	NdisZeroMemory((PUCHAR)&skb_p->cb[CB_OFF], 15);
	RTMP_SET_PACKET_NET_DEVICE_MBSSID(skb_p, MAIN_MBSSID);

	return rt28xx_packet_xmit(skb_p);
}


#if WIRELESS_EXT >= 12
// This function will be called when query /proc
struct iw_statistics *rt28xx_get_wireless_stats(
    IN struct net_device *net_dev)
{
	PRTMP_ADAPTER pAd = NULL;
#ifdef CONFIG_AP_SUPPORT 
	PMAC_TABLE_ENTRY pMacEntry = NULL;
#endif // CONFIG_AP_SUPPORT //

	GET_PAD_FROM_NET_DEV(pAd, net_dev);	

#ifdef CONFIG_AP_SUPPORT
	if (pAd->OpMode == OPMODE_AP)
	{
#ifdef APCLI_SUPPORT
		if (net_dev->priv_flags == INT_APCLI)
		{
			INT ApCliIdx = ApCliIfLookUp(pAd, (PUCHAR)net_dev->dev_addr);
			if ((ApCliIdx >= 0) && VALID_WCID(pAd->ApCfg.ApCliTab[ApCliIdx].MacTabWCID))
				pMacEntry = &pAd->MacTab.Content[pAd->ApCfg.ApCliTab[ApCliIdx].MacTabWCID];
		}
		else
#endif // APCLI_SUPPORT //
		{
			/*
				only AP client support wireless stats function.
				return NULL pointer for all other cases.
			*/
			pMacEntry = &pAd->MacTab.Content[0];
		}
	}
#endif // CONFIG_AP_SUPPORT //

	DBGPRINT(RT_DEBUG_TRACE, ("rt28xx_get_wireless_stats --->\n"));

	//check if the interface is down
	if(!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE))
		return NULL;	
	
	pAd->iw_stats.status = 0; // Status - device dependent for now

	// link quality
#ifdef CONFIG_AP_SUPPORT
	if (pAd->OpMode == OPMODE_AP)
	{
		if (pMacEntry != NULL)
			pAd->iw_stats.qual.qual = ((pMacEntry->ChannelQuality * 12)/10 + 10);
	}
#endif // CONFIG_AP_SUPPORT //

	if(pAd->iw_stats.qual.qual > 100)
		pAd->iw_stats.qual.qual = 100;

#ifdef CONFIG_AP_SUPPORT
	if (pAd->OpMode == OPMODE_AP)
	{
		if (pMacEntry != NULL)
			pAd->iw_stats.qual.level =
				RTMPMaxRssi(pAd, pMacEntry->RssiSample.LastRssi0,
								pMacEntry->RssiSample.LastRssi1,
								pMacEntry->RssiSample.LastRssi2);
	}
#endif // CONFIG_AP_SUPPORT //

	pAd->iw_stats.qual.noise = pAd->BbpWriteLatch[66]; // noise level (dBm)
	
	pAd->iw_stats.qual.noise += 256 - 143;
	pAd->iw_stats.qual.updated = 1;     // Flags to know if updated
#ifdef IW_QUAL_DBM
	pAd->iw_stats.qual.updated |= IW_QUAL_DBM;	// Level + Noise are dBm
#endif // IW_QUAL_DBM //

	pAd->iw_stats.discard.nwid = 0;     // Rx : Wrong nwid/essid
	pAd->iw_stats.miss.beacon = 0;      // Missed beacons/superframe
	
	DBGPRINT(RT_DEBUG_TRACE, ("<--- rt28xx_get_wireless_stats\n"));
	return &pAd->iw_stats;
}
#endif // WIRELESS_EXT //


void tbtt_tasklet(unsigned long data)
{
//#define MAX_TX_IN_TBTT		(16)

#ifdef CONFIG_AP_SUPPORT
		PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) data;

#ifdef RTMP_MAC_PCI
	if (pAd->OpMode == OPMODE_AP)
	{
		/* update channel utilization */
		QBSS_LoadUpdate(pAd, 0);

	}
#endif // RTMP_MAC_PCI //

	if (pAd->OpMode == OPMODE_AP)
	{
		//
		// step 7 - if DTIM, then move backlogged bcast/mcast frames from PSQ to TXQ whenever DtimCount==0
		if (pAd->ApCfg.DtimCount == 0)
		{
			PQUEUE_ENTRY    pEntry;
			BOOLEAN			bPS = FALSE;
			UINT 			count = 0;
			unsigned long 		IrqFlags;

//			NdisAcquireSpinLock(&pAd->MacTabLock);
//			NdisAcquireSpinLock(&pAd->TxSwQueueLock);
			
			RTMP_IRQ_LOCK(&pAd->irq_lock, IrqFlags);
			while (pAd->MacTab.McastPsQueue.Head)
			{
				bPS = TRUE;
				if (pAd->TxSwQueue[QID_AC_BE].Number <= (MAX_PACKETS_IN_QUEUE + MAX_PACKETS_IN_MCAST_PS_QUEUE))
				{
					pEntry = RemoveHeadQueue(&pAd->MacTab.McastPsQueue);
					//if(pAd->MacTab.McastPsQueue.Number)
					if (count)
					{
						RTMP_SET_PACKET_MOREDATA(pEntry, TRUE);
					}
					InsertHeadQueue(&pAd->TxSwQueue[QID_AC_BE], pEntry);
					count++;
				}
				else
				{
					break;
				}
			}
			RTMP_IRQ_UNLOCK(&pAd->irq_lock, IrqFlags);
			
			
//			NdisReleaseSpinLock(&pAd->TxSwQueueLock);
//			NdisReleaseSpinLock(&pAd->MacTabLock);
			if (pAd->MacTab.McastPsQueue.Number == 0)
			{			
                UINT bss_index;

                /* clear MCAST/BCAST backlog bit for all BSS */
                for(bss_index=BSS0; bss_index<pAd->ApCfg.BssidNum; bss_index++)
					WLAN_MR_TIM_BCMC_CLEAR(bss_index);
                /* End of for */
			}
			pAd->MacTab.PsQIdleCount = 0;

			// Dequeue outgoing framea from TxSwQueue0..3 queue and process it
            if (bPS == TRUE) 
			{
				RTMPDeQueuePacket(pAd, FALSE, NUM_OF_TX_RING, /*MAX_TX_IN_TBTT*/MAX_PACKETS_IN_MCAST_PS_QUEUE);
			}
		}
	}
#endif // CONFIG_AP_SUPPORT //
}

INT rt28xx_ioctl(
	IN	PNET_DEV	net_dev, 
	IN	OUT	struct ifreq	*rq, 
	IN	INT					cmd)
{
	RTMP_ADAPTER	*pAd = NULL;
	INT				ret = 0;

	GET_PAD_FROM_NET_DEV(pAd, net_dev);	

	if (pAd == NULL)
	{
		/* if 1st open fail, pAd will be free;
		   So the net_dev->priv will be NULL in 2rd open */
		return -ENETDOWN;
	}

#ifdef CONFIG_AP_SUPPORT
#ifdef AP_IWPRIV_COMMAND_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		ret = rt28xx_ap_ioctl(net_dev, rq, cmd);
	}
#endif // AP_IWPRIV_COMMAND_SUPPORT //
#endif // CONFIG_AP_SUPPORT //


	return ret;
}


/*
    ========================================================================

    Routine Description:
        return ethernet statistics counter

    Arguments:
        net_dev                     Pointer to net_device

    Return Value:
        net_device_stats*

    Note:

    ========================================================================
*/
static struct net_device_stats *RT28xx_get_ether_stats(
    IN  struct net_device *net_dev)
{
    RTMP_ADAPTER *pAd = NULL;

	if (net_dev)
		GET_PAD_FROM_NET_DEV(pAd, net_dev);	

	if (pAd)
	{

		pAd->stats.rx_packets = pAd->WlanCounters.ReceivedFragmentCount.QuadPart;
		pAd->stats.tx_packets = pAd->WlanCounters.TransmittedFragmentCount.QuadPart;

		pAd->stats.rx_bytes = pAd->RalinkCounters.ReceivedByteCount;
		pAd->stats.tx_bytes = pAd->RalinkCounters.TransmittedByteCount;

		pAd->stats.rx_errors = pAd->Counters8023.RxErrors;
		pAd->stats.tx_errors = pAd->Counters8023.TxErrors;

		pAd->stats.rx_dropped = 0;
		pAd->stats.tx_dropped = 0;

	    pAd->stats.multicast = pAd->WlanCounters.MulticastReceivedFrameCount.QuadPart;   // multicast packets received
	    pAd->stats.collisions = pAd->Counters8023.OneCollision + pAd->Counters8023.MoreCollisions;  // Collision packets

	    pAd->stats.rx_length_errors = 0;
	    pAd->stats.rx_over_errors = pAd->Counters8023.RxNoBuffer;                   // receiver ring buff overflow
	    pAd->stats.rx_crc_errors = 0;//pAd->WlanCounters.FCSErrorCount;     // recved pkt with crc error
	    pAd->stats.rx_frame_errors = pAd->Counters8023.RcvAlignmentErrors;          // recv'd frame alignment error
	    pAd->stats.rx_fifo_errors = pAd->Counters8023.RxNoBuffer;                   // recv'r fifo overrun
	    pAd->stats.rx_missed_errors = 0;                                            // receiver missed packet

	    // detailed tx_errors
	    pAd->stats.tx_aborted_errors = 0;
	    pAd->stats.tx_carrier_errors = 0;
	    pAd->stats.tx_fifo_errors = 0;
	    pAd->stats.tx_heartbeat_errors = 0;
	    pAd->stats.tx_window_errors = 0;

	    // for cslip etc
	    pAd->stats.rx_compressed = 0;
	    pAd->stats.tx_compressed = 0;
		
		return &pAd->stats;
	}
	else
    	return NULL;
}


BOOLEAN RtmpPhyNetDevExit(
	IN RTMP_ADAPTER *pAd, 
	IN PNET_DEV net_dev)
{


#ifdef CONFIG_AP_SUPPORT
#ifdef APCLI_SUPPORT
	// remove all AP-client virtual interfaces.
	RT28xx_ApCli_Remove(pAd);
#endif // APCLI_SUPPORT //

#ifdef WDS_SUPPORT
	// remove all WDS virtual interfaces.
	RT28xx_WDS_Remove(pAd);
#endif // WDS_SUPPORT //

#ifdef MBSS_SUPPORT
	RT28xx_MBSS_Remove(pAd);
#endif // MBSS_SUPPORT //
#endif // CONFIG_AP_SUPPORT //

#ifdef INF_AMAZON_PPA
	if (ppa_hook_directpath_register_dev_fn && pAd->PPAEnable==TRUE) 
	{
		UINT status;
		status=ppa_hook_directpath_register_dev_fn(&pAd->g_if_id, pAd->net_dev, NULL, PPA_F_DIRECTPATH_DEREGISTER);
		printk("unregister PPA:g_if_id=%d status=%d\n",pAd->g_if_id,status);
	}
	kfree(pAd->pDirectpathCb); 
#endif // INF_AMAZON_PPA //

	// Unregister network device
	if (net_dev != NULL)
	{
		printk("RtmpOSNetDevDetach(): RtmpOSNetDeviceDetach(), dev->name=%s!\n", net_dev->name);
		RtmpOSNetDevDetach(net_dev);
	}

	return TRUE;
	
}


/*
========================================================================
Routine Description:
    Allocate memory for adapter control block.

Arguments:
    pAd					Pointer to our adapter

Return Value:
	NDIS_STATUS_SUCCESS
	NDIS_STATUS_FAILURE
	NDIS_STATUS_RESOURCES

Note:
========================================================================
*/
NDIS_STATUS AdapterBlockAllocateMemory(
	IN PVOID	handle,
	OUT	PVOID	*ppAd)
{

	*ppAd = (PVOID)vmalloc(sizeof(RTMP_ADAPTER)); //pci_alloc_consistent(pci_dev, sizeof(RTMP_ADAPTER), phy_addr);
    
	if (*ppAd) 
	{
		NdisZeroMemory(*ppAd, sizeof(RTMP_ADAPTER));
		((PRTMP_ADAPTER)*ppAd)->OS_Cookie = handle;
		return (NDIS_STATUS_SUCCESS);
	} else {
		return (NDIS_STATUS_FAILURE);
	}
}

