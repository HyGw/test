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
	rtmp_data.c

	Abstract:
	Data path subroutines

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	John        Aug/17/04       major modification for RT2561/2661
*/

#include "rt_config.h"

//iverson
#define GETOSPKTDATAPTR(_pkt) (RTPKT_TO_OSPKT(_pkt)->data)
#define RTPKT_TO_OSPKT(_p) ((struct sk_buff*)(_p))
//iverson end

#ifdef RTL865X_SOC
static inline BOOLEAN RTMPBridgeToWdsAndWirelessSta(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  PUCHAR          pHeader802_3,
    IN  UINT            HdrLen,
    IN  PUCHAR          pData,
    IN  UINT            DataLen,
    IN  UINT            FromWhichBSSID);

static inline BOOLEAN RTMPCheckDHCPFrame(
	IN	PRTMP_ADAPTER	pAd, 
	IN  struct sk_buff	*pSkb);

static inline NDIS_STATUS RTMPCloneNdisPacket(
    IN  PRTMP_ADAPTER   pAdapter,
    IN  struct sk_buff  *pInSkb,
    OUT struct sk_buff  **ppOutSkb);

static void rtl865x_rx(PRTMP_ADAPTER pAd, struct sk_buff *skb);

//#define netif_rx	rtl865x_rx
#endif /* RTL865X_SOC */

static inline NDIS_STATUS InsertQueuePkt(
	IN PRTMP_ADAPTER pAd,
	IN UCHAR QueIdx,
	IN struct sk_buff *pSkb)
{
	unsigned long IrqFlags;
#ifdef BLOCK_NET_IF
	PNET_DEV pNetDev;
#endif // BLOCK_NET_IF //

	if (pAd->TxSwQueue[QueIdx].Number >= pAd->PortCfg.TxQueueSize)
	{
#ifdef RTL865X_FAST_PATH
		rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
		RELEASE_NDIS_PACKET(pAd, pSkb);
#endif
		DBGPRINT(RT_DEBUG_TRACE, "too many frames (=%d) in TxSwQueue[%d], drop this one\n", pAd->TxSwQueue[QueIdx].Number, QueIdx);

#ifdef BLOCK_NET_IF

#ifdef APCLI_SUPPORT
		if (RTMP_GET_PACKET_NET_DEVICE(pSkb) >= MIN_NET_DEVICE_FOR_APCLI)
			pNetDev = pAd->ApCliTab.ApCliEntry[RTMP_GET_PACKET_NET_DEVICE(pSkb)- MIN_NET_DEVICE_FOR_APCLI].dev;
		else
#endif // APCLI_SUPPORT //
#ifdef WDS_SUPPORT
		// WDS link should never go into power-save mode; just send out the frame
		if (RTMP_GET_PACKET_NET_DEVICE(pSkb) >= MIN_NET_DEVICE_FOR_WDS)
			pNetDev = pAd->WdsTab.WdsEntry[RTMP_GET_PACKET_NET_DEVICE(pSkb)- MIN_NET_DEVICE_FOR_WDS].dev;
		else
#endif // WDS_SUPPORT //
		{
			UCHAR apidx;
			apidx = RTMP_GET_PACKET_NET_DEVICE(pSkb);
			pNetDev = (apidx == MAIN_MBSSID) ? 
				pAd->net_dev : pAd->PortCfg.MBSSID[apidx].MSSIDDev;
		}

		blockNetIf(&pAd->blockQueueTab[QueIdx], pNetDev); 
#endif // BLOCK_NET_IF //
		return NDIS_STATUS_FAILURE;
	}
	else 
	{
#ifndef THREAD_ISR
		RTMP_SEM_LOCK(&pAd->TxSwQueueLock, IrqFlags);
#else
		RTMP_IRQ_LOCK(IrqFlags);
#endif
		InsertTailQueue(&pAd->TxSwQueue[QueIdx], pSkb);
#ifndef THREAD_ISR
		RTMP_SEM_UNLOCK(&pAd->TxSwQueueLock, IrqFlags);
#else
		RTMP_IRQ_UNLOCK(IrqFlags);
#endif
	}
	return NDIS_STATUS_SUCCESS;
}


UCHAR	SNAP_802_1H[] = {0xaa, 0xaa, 0x03, 0x00, 0x00, 0x00};
UCHAR	SNAP_BRIDGE_TUNNEL[] = {0xaa, 0xaa, 0x03, 0x00, 0x00, 0xf8};
// Add Cisco Aironet SNAP heade for CCX2 support
UCHAR	SNAP_AIRONET[] = {0xaa, 0xaa, 0x03, 0x00, 0x40, 0x96, 0x00, 0x00};

UCHAR	EAPOL[]			= {0x88, 0x8E};
UCHAR	IPX[]			= {0x81, 0x37};
UCHAR	APPLE_TALK[]	= {0x80, 0xF3};

//static  UCHAR   TPID[]			= {0x81, 0x00};

static  UCHAR   RateIdToPlcpSignal[12] = { 
	 0, /* RATE_1 */    1, /* RATE_2 */     2, /* RATE_5_5 */   3, /* RATE_11 */    // see BBP spec
	11, /* RATE_6 */   15, /* RATE_9 */    10, /* RATE_12 */   14, /* RATE_18 */    // see IEEE802.11a-1999 p.14
	 9, /* RATE_24 */  13, /* RATE_36 */    8, /* RATE_48 */   12  /* RATE_54 */ }; // see IEEE802.11a-1999 p.14

// Macro for rx indication
VOID REPORT_ETHERNET_FRAME_TO_LLC(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			p8023hdr,
	IN	PUCHAR			pData,
	IN	ULONG 			DataSize,
	IN	struct net_device	*net_dev)
{
	struct sk_buff	*pSkb;

    if ((pSkb = __dev_alloc_skb(DataSize + LENGTH_802_3 + 2, MEM_ALLOC_FLAG)) != NULL)
    {
        	pSkb->dev = net_dev;
        	skb_reserve(pSkb, 2);	// 16 byte align the IP header
        	memcpy(skb_put(pSkb, LENGTH_802_3), p8023hdr, LENGTH_802_3);
        	memcpy(skb_put(pSkb, DataSize), pData, DataSize);
#ifdef APCLI_SUPPORT
			UWRMacConverterForRxPkt(pAd, pSkb, net_dev);
#endif
        	pSkb->protocol = eth_type_trans(pSkb, net_dev);
#ifdef RTL865X_SOC
	        skb_push(pSkb, net_dev->hard_header_len);
			rtl865x_rx(pAd, pSkb);
#else
        	netif_rx(pSkb);
#endif
        	pAd->net_dev->last_rx = jiffies;
        	pAd->stats.rx_packets++;
    }
}

#ifdef RTL865X_SOC
// Macro for rx indication
VOID REPORT_ETHERNET_FRAME_TO_LLC_RTL865X(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			p8023hdr,
	IN	PUCHAR			pData,
	IN	ULONG 			DataSize,
	IN	struct net_device	*net_dev)
{
	struct sk_buff	*pSkb;

    if ((pSkb = __dev_alloc_skb(DataSize + LENGTH_802_3 + 2, MEM_ALLOC_FLAG)) != NULL)
    {
        	pSkb->dev = net_dev;
        	skb_reserve(pSkb, 2);	// 16 byte align the IP header
        	memcpy(skb_put(pSkb, LENGTH_802_3), p8023hdr, LENGTH_802_3);
        	memcpy(skb_put(pSkb, DataSize), pData, DataSize);
			
#ifdef APCLI_SUPPORT
			UWRMacConverterForRxPkt(pAd, pSkb, net_dev);
#endif
        	pSkb->protocol = eth_type_trans(pSkb, net_dev);
        	netif_rx(pSkb);
        	pAd->net_dev->last_rx = jiffies;
        	pAd->stats.rx_packets++;
    }
}
#endif

// Macro for rx indication
VOID REPORT_ETHERNET_FRAME_TO_LLC_WITH_NON_COPY(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			p8023hdr,
	IN	PUCHAR			pData,
	IN	ULONG 			DataSize,
	IN	struct net_device	*net_dev)
{
#ifdef NONCOPY_RX 
//   	PRXD_STRUC		pRxD;
	struct sk_buff	*pRxSkb = pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.pSkb;
	struct sk_buff  *pSkb = NULL;
	
	// allocate a new one and update RxD
#ifdef TC_SOC
    if ((pSkb = skbmgr_dev_alloc_skb2k()) != NULL)
#else
    if ((pSkb = __dev_alloc_skb(RX_DMA_BUFFER_SIZE, MEM_ALLOC_FLAG)) != NULL)
#endif
    {    
	    pRxSkb->dev = net_dev;
	    pRxSkb->data = pData;
    	pRxSkb->len = DataSize;
	    pRxSkb->tail = pRxSkb->data + pRxSkb->len;
	    memcpy(skb_push(pRxSkb, LENGTH_802_3), p8023hdr, LENGTH_802_3);

#ifdef APCLI_SUPPORT
		UWRMacConverterForRxPkt(pAd, pRxSkb, net_dev);
#endif

	    pRxSkb->protocol = eth_type_trans(pRxSkb, net_dev);
#ifdef RTL865X_SOC
	    skb_push(pRxSkb, net_dev->hard_header_len);
		rtl865x_rx(pAd, pRxSkb);
#else
		netif_rx(pRxSkb);
#endif
		pAd->net_dev->last_rx = jiffies;
		pAd->stats.rx_packets++;
    		
	    RTMP_SET_PACKET_SOURCE(pSkb, PKTSRC_DRIVER);
	    pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocSize	= RX_DMA_BUFFER_SIZE;
		pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.pSkb		= pSkb;
		pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocVa		= pSkb->data;
		pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocPa		= pci_map_single(pAd->pPci_Dev, pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocVa, pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocSize, PCI_DMA_FROMDEVICE);

		// Write RxD buffer address & allocated buffer length
		//pRxD = (PRXD_STRUC) pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].AllocVa;
		//pRxD->BufPhyAddr = pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocPa;
    } else {
		RTMP_SET_PACKET_SOURCE(pRxSkb, PKTSRC_DRIVER);
	    pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocSize	= RX_DMA_BUFFER_SIZE;
		pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.pSkb		= pRxSkb;
		pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocVa		= pRxSkb->data;
		pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocPa		= pci_map_single(pAd->pPci_Dev, pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocVa, pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocSize, PCI_DMA_FROMDEVICE);

		// Write RxD buffer address & allocated buffer length
		//pRxD = (PRXD_STRUC) pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].AllocVa;
		//pRxD->BufPhyAddr = pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocPa;    	
    }
#else
	struct sk_buff	*pSkb;

    if ((pSkb = __dev_alloc_skb(DataSize + LENGTH_802_3 + 2, MEM_ALLOC_FLAG)) != NULL)
    {
        	pSkb->dev = net_dev;
        	skb_reserve(pSkb, 2);	// 16 byte align the IP header
        	memcpy(skb_put(pSkb, LENGTH_802_3), p8023hdr, LENGTH_802_3);
        	memcpy(skb_put(pSkb, DataSize), pData, DataSize);

#ifdef APCLI_SUPPORT
			UWRMacConverterForRxPkt(pAd, pSkb, net_dev);
#endif
        	pSkb->protocol = eth_type_trans(pSkb, net_dev);
#ifdef RTL865X_SOC
	        skb_push(pSkb, net_dev->hard_header_len);
			rtl865x_rx(pAd, pSkb);
#else
        	netif_rx(pSkb);
#endif
        	pAd->net_dev->last_rx = jiffies;
        	pAd->stats.rx_packets++;
    }
#endif
}

// Macro for rx indication
VOID REPORT_AGGREGATE_ETHERNET_FRAME_TO_LLC_WITH_NON_COPY(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			p8023hdr,
	IN	PUCHAR			pData,
	IN	ULONG 			DataSize1,
	IN	ULONG 			DataSize2,
	IN	struct net_device	*net_dev)
{
	struct sk_buff	*pRxSkb = pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.pSkb;
	struct sk_buff  *pSkb1 = NULL;
	struct sk_buff  *pSkb2 = NULL;
	PUCHAR			pData2;
	PUCHAR			phdr;

	pData2 = pData + DataSize1 + LENGTH_802_3;
	phdr = pData + DataSize1;

    if ((pSkb2 = __dev_alloc_skb(DataSize2 + LENGTH_802_3 + 2, MEM_ALLOC_FLAG)) != NULL)
    {
    	pSkb2->dev = net_dev;
    	skb_reserve(pSkb2, 2);	// 16 byte align the IP header
    	memcpy(skb_put(pSkb2, LENGTH_802_3), phdr, LENGTH_802_3);
    	memcpy(skb_put(pSkb2, DataSize2), pData2, DataSize2);
#ifdef APCLI_SUPPORT
		UWRMacConverterForRxPkt(pAd, pSkb2, net_dev);
#endif
    	pSkb2->protocol = eth_type_trans(pSkb2, net_dev);
    } else {
    	return;
    }
	
	// allocate a new one and update RxD
#ifdef TC_SOC
    if ((pSkb1 = skbmgr_dev_alloc_skb2k()) != NULL)
#else
    if ((pSkb1 = __dev_alloc_skb(RX_DMA_BUFFER_SIZE, MEM_ALLOC_FLAG)) != NULL)
#endif
    {    
	    pRxSkb->dev = net_dev;
	    pRxSkb->data = pData;
    	pRxSkb->len = DataSize1;
	    pRxSkb->tail = pRxSkb->data + pRxSkb->len;
	    memcpy(skb_push(pRxSkb, LENGTH_802_3), p8023hdr, LENGTH_802_3);

#ifdef APCLI_SUPPORT
		UWRMacConverterForRxPkt(pAd, pRxSkb, net_dev);
#endif
	    pRxSkb->protocol = eth_type_trans(pRxSkb, net_dev);
#ifdef RTL865X_SOC
	    skb_push(pRxSkb, net_dev->hard_header_len);
		rtl865x_rx(pAd, pRxSkb);

	    skb_push(pSkb2, net_dev->hard_header_len);
		rtl865x_rx(pAd, pSkb2);
#else
		netif_rx(pRxSkb);
		netif_rx(pSkb2);
#endif
		pAd->net_dev->last_rx = jiffies;
		pAd->stats.rx_packets += 2;
    		
	    RTMP_SET_PACKET_SOURCE(pSkb1, PKTSRC_DRIVER);
	    pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocSize	= RX_DMA_BUFFER_SIZE;
		pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.pSkb		= pSkb1;
		pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocVa		= pSkb1->data;
		pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocPa		= pci_map_single(pAd->pPci_Dev, pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocVa, pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocSize, PCI_DMA_FROMDEVICE);

		// Write RxD buffer address & allocated buffer length
		//pRxD = (PRXD_STRUC) pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].AllocVa;
		//pRxD->BufPhyAddr = pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocPa;
    } else {
		RTMP_SET_PACKET_SOURCE(pRxSkb, PKTSRC_DRIVER);
	    pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocSize	= RX_DMA_BUFFER_SIZE;
		pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.pSkb		= pRxSkb;
		pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocVa		= pRxSkb->data;
		pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocPa		= pci_map_single(pAd->pPci_Dev, pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocVa, pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocSize, PCI_DMA_FROMDEVICE);
		dev_kfree_skb_any(pSkb2);
		return;
		// Write RxD buffer address & allocated buffer length
		//pRxD = (PRXD_STRUC) pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].AllocVa;
		//pRxD->BufPhyAddr = pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocPa;    	
    }
}

// Enqueue this frame to MLME engine
// We need to enqueue the whole frame because MLME need to pass data type
// information from 802.11 header
#define REPORT_MGMT_FRAME_TO_MLME(_pAd, _pFrame, _FrameSize, _Rssi, _Offset)                     \
{                                                                                       \
    ULONG  High32TSF, Low32TSF;                                                          \
	RTMP_IO_READ32(_pAd, TXRX_CSR13, &High32TSF);                                       \
	RTMP_IO_READ32(_pAd, TXRX_CSR12, &Low32TSF);                                        \
	MlmeEnqueueForRecv(_pAd, High32TSF,	Low32TSF, (UCHAR)_Rssi, _FrameSize, _pFrame, _Offset);   \
}

/*
	========================================================================

	Routine	Description:
		Process	RxDone interrupt, running in DPC level

	Arguments:
		pAdapter	Pointer	to our adapter

	Return Value:
		None

	Note:
		This routine has to	maintain Rx	ring read pointer.
		Need to consider QOS DATA format when converting to 802.3
	========================================================================
*/
VOID	RTMPHandleRxDoneInterrupt(
	IN	PRTMP_ADAPTER	pAd)
{
    PRXD_STRUC					pRxD;
#ifdef BIG_ENDIAN
    PRXD_STRUC      			pDestRxD;
    RXD_STRUC       			RxD;
#endif
	PHEADER_802_11				pHeader;
	PUCHAR						pData;
	USHORT						DataSize, Msdu2Size;
	PUCHAR						pDA, pSA;
	UCHAR						Header802_3[14];
	INT							Count, apidx = MAIN_MBSSID, i;
	MAC_TABLE_ENTRY				*pEntry = NULL;
    NDIS_802_11_PRIVACY_FILTER	Privacy;
    WPA_STATE					WpaState;
    struct net_device			*net_dev = pAd->net_dev;
    BOOLEAN         			bWdsPacket = FALSE;
    UCHAR                       Offset = 0;
    UMAXLONG					IrqFlags;
	BOOLEAN						bQoS;
	UCHAR						OldPwrMgmt = PWR_ACTIVE;
#ifdef APCLI_SUPPORT
	INT 						ApCliIdx = MAIN_MBSSID;
	BOOLEAN         			bApCliPacket = FALSE;	
#endif

    // Make sure Rx ring resource won't be used by other threads
    RTMP_SEM_LOCK(&pAd->RxRingLock, IrqFlags);

    DBGPRINT(RT_DEBUG_INFO, "RTMPHandleRxDoneInterrupt --->\n");

    for (Count=0; Count<MAX_RX_PROCESS; Count++)
    {
#ifndef BIG_ENDIAN
        // Point to Rx indexed rx ring descriptor
        pRxD = (PRXD_STRUC) pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].AllocVa;
#else
        pDestRxD = (PRXD_STRUC) pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].AllocVa;
        RxD = *pDestRxD;
        pRxD = &RxD;
        RTMPDescriptorEndianChange((PUCHAR)pRxD, TYPE_RXD);
#endif

		        
        // In case of false alarm or processed at last instance
        if (pRxD->Owner != DESC_OWN_HOST)
            break;

        // beak out this "do {...} while FALSE" loop whenever RX frame error without indicating 
        // to upper layer
        do
        {
			// ignore all incoming packet while Radar or Carrier signal existed.
			if ( (pAd->PortCfg.RadarDetect.RDMode != RD_NORMAL_MODE)
#ifdef CARRIER_DETECTION_SUPPORT
				||(isCarrierDetectExist(pAd) == TRUE)
#endif // CARRIER_DETECTION_SUPPORT //
				)
			{
				break;
			}

        	pci_unmap_single(pAd->pPci_Dev, pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocPa, pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocSize, PCI_DMA_FROMDEVICE);
            pData   = (PUCHAR) (pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocVa);
            pHeader = (PHEADER_802_11) pData;

#ifdef BIG_ENDIAN
	        RTMPFrameEndianChange(pAd, (PUCHAR)pHeader, DIR_READ, FALSE);
#endif
            // Increase Total receive byte counter after real data received no mater any error or not
            pAd->RalinkCounters.ReceivedByteCount +=  pRxD->DataByteCnt;
            pAd->RalinkCounters.RxCount ++;
            
            // Check for all RxD errors
            if (RTMPCheckRxError(pRxD) != NDIS_STATUS_SUCCESS)
            {
                if (pRxD->U2M && pRxD->CipherErr)
                {
                	if ((pRxD->CipherAlg == CIPHER_TKIP) && (pRxD->CipherErr == 2))	//MIC error
                	{
                		pEntry = PACInquiry(pAd, pHeader->Addr2, &Privacy, &WpaState);

                		if (pEntry)
                		{
                			HandleCounterMeasure(pAd, pEntry);
                		}
                	}
                    DBGPRINT(RT_DEBUG_ERROR, "Rx u2me DATA Cipher Err(len=%d,Alg=%d,keyidx=%d,ciphererr=%d)\n", 
                    	pRxD->DataByteCnt, pRxD->CipherAlg, pRxD->KeyIndex, pRxD->CipherErr);
                }

				// statistics - Rx Error & Rx Drop Frames
                if(pEntry)
                {
                	pAd->PortCfg.MBSSID[pEntry->ApIdx].RxErrors++;
                	pAd->PortCfg.MBSSID[pEntry->ApIdx].RxDrops++;
                }
#ifdef WDS_SUPPORT
		  		else if(pHeader->FC.FrDs==1 && pHeader->FC.ToDs==1 && pAd->WdsTab.Mode != WDS_DISABLE_MODE)
		  		{
 					for (i = 0; i < MAX_WDS_ENTRY; i++)
					{
						if ((pAd->WdsTab.WdsEntry[i].Valid == TRUE) && (MAC_ADDR_EQUAL(&pAd->WdsTab.WdsEntry[i].WdsAddr, &pHeader->Addr2)))
						{
							pAd->WdsTab.WdsEntry[i].RxErrors++;
							pAd->WdsTab.WdsEntry[i].RxDrops++;			  
 							break;
						}
					}
		  		}
		  
#endif 

#ifdef APCLI_SUPPORT
				for (i = 0; i < MAX_APCLI_ENTRY; i++)
				{
					//BSSID match the ApCliBssid ?(from a valid AP)
					if ((pAd->ApCliTab.ApCliEntry[i].Valid== TRUE)
						&& (MAC_ADDR_EQUAL(&pAd->ApCliTab.ApCliEntry[i].ApCliBssid, &pHeader->Addr2)))
					{
						pAd->ApCliTab.ApCliEntry[i].RxErrors++;
						pAd->ApCliTab.ApCliEntry[i].RxDrops++;
						break;
					}
				}
#endif
                pAd->Counters8023.RxErrors++;
                break;  // give up this frame
            }			  

            pAd->WlanCounters.ReceivedFragmentCount++;
            
#ifdef RALINK_ATE
            pAd->ate.RxCntPerSec++;
#endif

            // All frames to AP are directed except probe_req. IEEE 802.11/1999 - p.463
            // Do this before checking "duplicate frame".
            // 2003-08-20 accept BEACON to decide if OLBC (Overlapping Legacy BSS Condition) happens
            // TODO: consider move this code to be inside "APCheckRxError()"
            if (pRxD->U2M)
            {
                if (pHeader->FC.Type == BTYPE_DATA)
                {
                    CHAR RealRssi;
                    
                    RealRssi = ConvertToRssi(pAd, (UCHAR)pRxD->PlcpRssi, RSSI_NO_1);
                    pAd->PortCfg.LastRssi  = RealRssi + pAd->BbpRssiToDbmDelta;
                    pAd->PortCfg.AvgRssiX8  = (pAd->PortCfg.AvgRssiX8 - pAd->PortCfg.AvgRssi) + pAd->PortCfg.LastRssi;
                    pAd->PortCfg.AvgRssi = pAd->PortCfg.AvgRssiX8 >> 3;
                    pAd->PortCfg.NumOfAvgRssiSample ++;
                    
                    if ((pAd->RfIcType == RFIC_5325) || (pAd->RfIcType == RFIC_2529))
                    {
                    	pAd->PortCfg.LastRssi2  = ConvertToRssi(pAd, (UCHAR)pRxD->PlcpSignal, RSSI_NO_2) + pAd->BbpRssiToDbmDelta;
                    	pAd->PortCfg.AvgRssi2X8  = (pAd->PortCfg.AvgRssi2X8 - pAd->PortCfg.AvgRssi2) + pAd->PortCfg.LastRssi2;
	                    pAd->PortCfg.AvgRssi2 = pAd->PortCfg.AvgRssi2X8 >> 3;
                    }
                    
                    // Gather PowerSave information from all valid DATA frames. IEEE 802.11/1999 p.461
                    if (pHeader->FC.PwrMgmt)
                        OldPwrMgmt = PsIndicate(pAd, pHeader->Addr2, pAd->PortCfg.LastRssi, PWR_SAVE);
                    else
                        OldPwrMgmt = PsIndicate(pAd, pHeader->Addr2, pAd->PortCfg.LastRssi, PWR_ACTIVE);
            
#ifdef APCLI_SUPPORT               
                	// gather U2M data frame from ApCli interface to update AvgRSSI 
                	if (pHeader->FC.FrDs == 1 && pHeader->FC.ToDs == 0)
                	{
                		for (i = 0; i < MAX_APCLI_ENTRY; i++)
						{
							//BSSID match the ApCliBssid ?(from a valid AP)
							if ((pAd->ApCliTab.ApCliEntry[i].Valid== TRUE)
									&& (MAC_ADDR_EQUAL(&pAd->ApCliTab.ApCliEntry[i].ApCliBssid, &pHeader->Addr2)))
							{
								pAd->ApCliTab.ApCliEntry[i].NumOfAvgRssiSample ++;
								break;
							}
						}
                	}
#endif                		
                }
                // ignore all CNTL frames except PS-POLL
                else if ((pHeader->FC.Type == BTYPE_CNTL) && (pHeader->FC.SubType != SUBTYPE_PS_POLL))
                    break;
            }
            else // B/M-cast frame
            { 
#ifdef RALINK_ATE
				if(pAd->ate.Mode != ATE_APSTART)
				{
	            	CHAR RealRssi;
	                    
	                RealRssi = ConvertToRssi(pAd, (UCHAR)pRxD->PlcpRssi, RSSI_NO_1);
	                pAd->PortCfg.LastRssi  = RealRssi + pAd->BbpRssiToDbmDelta;
	                pAd->PortCfg.AvgRssiX8  = (pAd->PortCfg.AvgRssiX8 - pAd->PortCfg.AvgRssi) + pAd->PortCfg.LastRssi;
	                pAd->PortCfg.AvgRssi = pAd->PortCfg.AvgRssiX8 >> 3;

	                if ((pAd->RfIcType == RFIC_5325) || (pAd->RfIcType == RFIC_2529))
                    {
                    	pAd->PortCfg.LastRssi2  = ConvertToRssi(pAd, (UCHAR)pRxD->PlcpSignal, RSSI_NO_2) + pAd->BbpRssiToDbmDelta;
                    	pAd->PortCfg.AvgRssi2X8  = (pAd->PortCfg.AvgRssi2X8 - pAd->PortCfg.AvgRssi2) + pAd->PortCfg.LastRssi2;
	                	pAd->PortCfg.AvgRssi2 = pAd->PortCfg.AvgRssi2X8 >> 3;
                    }
				}
#endif	// RALINK_ATE
                    
                if ((pHeader->FC.Type == BTYPE_MGMT) && 
                    ((pHeader->FC.SubType == SUBTYPE_BEACON) || (pHeader->FC.SubType == SUBTYPE_PROBE_REQ)))
                {    
#ifdef APCLI_SUPPORT
					// gather beacon from ApCli interface to update AvgRSSI 
					if (pHeader->FC.SubType == SUBTYPE_BEACON)
					{
						for (i = 0; i < MAX_APCLI_ENTRY; i++)
						{
							//BSSID match the ApCliBssid ?(from a valid AP)
							if ((pAd->ApCliTab.ApCliEntry[i].Valid== TRUE)
								&& (MAC_ADDR_EQUAL(&pAd->ApCliTab.ApCliEntry[i].ApCliBssid, &pHeader->Addr2)))
							{
								pAd->ApCliTab.ApCliEntry[i].NumOfAvgRssiSample ++;
								break;
							}
						}	
					}	
#endif
                }
                else
				{
#ifndef APCLI_SUPPORT
                    break; // give up this frame
#else 
					// It is possible to receive the multicast packet when in AP Client mode
					// Such as a broadcast from AP2 to AP1, address1 is ffffff, address2 is AP2 bssid, addr3 is sta4 mac address
					PUCHAR pRemovedLLCSNAP;

					for(i = 0; i < MAX_APCLI_ENTRY; i++)
					{
						//BSSID match the ApCliBssid ?(from a valid AP)
						if((pAd->ApCliTab.ApCliEntry[i].Valid == TRUE)
							&& (pAd->ApCliTab.ApCliEntry[i].Enable == TRUE)
							&& (MAC_ADDR_EQUAL(&pAd->ApCliTab.ApCliEntry[i].ApCliBssid, &pHeader->Addr2)))
						{
							net_dev = pAd->ApCliTab.ApCliEntry[i].dev;
							DBGPRINT(RT_DEBUG_INFO, "APCLI Multicast packet from (apcli%d)!!!\n", i);
							break;
						}
					}   

					if (i == MAX_APCLI_ENTRY)
					{
						DBGPRINT(RT_DEBUG_INFO, "APCLI packet, but not our APCLI group, give it up!!!\n");
						break;// give up this frame
					}

 					// Filter out Bcast frame which AP relayed for us
					// Multicast packet send from AP1 , received by AP2 and send back to AP1, drop this frame   					
					if (MAC_ADDR_EQUAL(pHeader->Addr3, pAd->ApCliTab.ApCliEntry[i].CurrentAddress))
							break;// give up this frame
					
					// This frame must be from DS when in AP Client mode
					if (pHeader->FC.FrDs == 0)
						break;// give up this frame
					
					// Use software to decrypt the encrypted frame
					// Because this received frame isn't my BSS frame, Asic passed to driver without decrypting it.
					// Then its "CipherAlg" in RxD would be marked as "NONE".
            		if ((pRxD->MyBss == 0) && (pRxD->CipherAlg == CIPHER_NONE) && (pHeader->FC.Wep == 1)) 
            		{
            			// handle WEP decryption
            			if(pAd->ApCliTab.ApCliEntry[i].GroupCipher == Ndis802_11WEPEnabled)
            			{
            				UCHAR *pPayload = (UCHAR *)pData + LENGTH_802_11;
            	
            				if (RTMPDecryptData(pAd, pPayload, pRxD->DataByteCnt - LENGTH_802_11) == FALSE)
   							{
								printk("ERROR : Software decrypt WEP data fails.\n");	
								break;// give up this frame
							}
                			else 
								pRxD->DataByteCnt -= 8;  //Minus IV[4] & ICV[4]
#ifdef RTL865X_SOC
							//printk("RTMPDecryptData WEP data Complete \n");	
#else							
							DBGPRINT(RT_DEBUG_INFO, "RTMPDecryptData WEP data Complete \n");	
#endif							
						}
						// handle TKIP decryption
						else if(pAd->ApCliTab.ApCliEntry[i].GroupCipher == Ndis802_11Encryption2Enabled)
						{	
							if (RTMPSoftDecryptTKIP(pAd, pData, pRxD->DataByteCnt, 0, pAd->ApCliTab.ApCliEntry[i].SharedKey))
							{
#ifdef RTL865X_SOC
								//printk("RTMPSoftDecryptTKIP Complete \n");
#else								
								DBGPRINT(RT_DEBUG_INFO, "RTMPSoftDecryptTKIP Complete \n");
#endif								
								pRxD->DataByteCnt -= 20;  //Minus 8 bytes MIC, 8 bytes IV/EIV, 4 bytes ICV
							}
                			else
							{
								printk("ERROR : RTMPSoftDecryptTKIP Failed\n");
                    			break; // give up this frame
            				}
						}
						// handle AES decryption
						else if(pAd->ApCliTab.ApCliEntry[i].GroupCipher == Ndis802_11Encryption3Enabled)
						{
							if (RTMPSoftDecryptAES(pAd, pData, pRxD->DataByteCnt , pAd->ApCliTab.ApCliEntry[i].SharedKey))
							{
#ifdef RTL865X_SOC
								//printk("RTMPSoftDecryptAES Complete \n");
#else								
								DBGPRINT(RT_DEBUG_INFO, "RTMPSoftDecryptAES Complete \n");
#endif								
								pRxD->DataByteCnt -= 16;  //8 bytes MIC, 8 bytes IV/EIV (CCMP Header)
							}
							else
							{
								printk("ERROR : RTMPSoftDecryptAES Failed\n");
								break;  // give up this frame
							}
						}
						else
						{
							break;  // give up this frame
						}		
            		}						

					pDA = pHeader->Addr1;
					pSA = pHeader->Addr3;
					pData += LENGTH_802_11;
					DataSize = (USHORT)pRxD->DataByteCnt - LENGTH_802_11;

					// remove the 2 extra QOS CNTL bytes
					if(pHeader->FC.SubType & 0x08)
					{
						bQoS = TRUE;
						pData += 2;
						DataSize -= 2;
						Offset += 2;
					}

					// remove the 2 extra AGGREGATION bytes
					Msdu2Size = 0;
					if(pHeader->FC.Order)
					{
						Msdu2Size = *pData + (*(pData+1) << 8);
						if((Msdu2Size <= 1536) && (Msdu2Size < DataSize))
						{
							pData += 2;
							DataSize -= 2;
						} else
							Msdu2Size = 0;
					}

					CONVERT_TO_802_3(Header802_3, pDA, pSA, pData, DataSize, pRemovedLLCSNAP);                                                                                                          
					REPORT_ETHERNET_FRAME_TO_LLC_WITH_NON_COPY(pAd, Header802_3, pData, DataSize, net_dev);                     
					pRxD->BufPhyAddr = pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocPa;                     
					break;//end process this packet
#endif 		
				}//else			
            }	// end - Broadcast/Multicast frame

            // DUPLICATE FRAME CHECK:
            //   Check retry bit. If this bit is on, search the cache with SA & sequence
            //   as index, if matched, discard this frame, otherwise, update cache
            //   This check only apply to unicast data & management frames
            if ((pRxD->U2M) && 
                (pHeader->FC.Retry) &&
                (RTMPSearchTupleCache(pAd, pHeader) == TRUE))
            {
                DBGPRINT(RT_DEBUG_INFO,"RxDone- drop DUPLICATE frame(len=%d)\n",pRxD->DataByteCnt);
                pAd->WlanCounters.FrameDuplicateCount++;
                break; // give up this frame
            }
            else    // Update Tuple Cache
                RTMPUpdateTupleCache(pAd, pHeader);

            pAd->Counters8023.GoodReceives++;

#ifdef RALINK_ATE
			if(pAd->ate.Mode != ATE_APSTART)
			{
				break;
			}
#endif

            //
            // CASE I. receive a DATA frame
            //
            if (pHeader->FC.Type == BTYPE_DATA)
            {

#ifndef APCLI_SUPPORT			
            	if (pHeader->FC.ToDs == 0)
                    break; // give up this frame
#endif

				pAd->RalinkCounters.OneSecRxOkCnt++;

                pEntry = PACInquiry(pAd, pHeader->Addr2, &Privacy, &WpaState);
				if(pEntry)
				{
				    apidx = pEntry->ApIdx;
				    net_dev = pAd->PortCfg.MBSSID[apidx].MSSIDDev;
				    DBGPRINT(RT_DEBUG_INFO, "Rcv packet to IF(ra%d)\n", apidx);
				}
				
				if ((pEntry) && (CLIENT_STATUS_TEST_FLAG(pEntry, fCLIENT_STATUS_APSD_CAPABLE)))
            	{
            		UCHAR			OldUP;

            		OldUP = (*(pData+LENGTH_802_11) & 0x07);

                    if (pHeader->FC.PwrMgmt && (OldPwrMgmt == PWR_SAVE))
                    {
                    	RTMPHandleUAPSDTriggerFrame(pAd, pEntry, OldUP);
                    }
            	}

				// Drop NULL, CF-ACK(no data), CF-POLL(no data), and CF-ACK+CF-POLL(no data) data frame
                if (pHeader->FC.SubType & 0x04) // bit 2 : no DATA
                    break; // give up this frame
				
                // check if Class2 or 3 error
                if ((pHeader->FC.FrDs == 0) && (CheckClass2Class3Error(pAd, pHeader, pEntry)))
                    break; // give up this frame

                if(pAd->PortCfg.MBSSID[apidx].BANClass3Data == TRUE)
                    break; // give up this frame

				// statistics - Rx traffic count
#ifdef APCLI_SUPPORT
				for (i = 0; i < MAX_APCLI_ENTRY; i++)
				{
					//BSSID match the ApCliBssid ?(from a valid AP)
					if ((pAd->ApCliTab.ApCliEntry[i].Valid== TRUE)
						&& (MAC_ADDR_EQUAL(&pAd->ApCliTab.ApCliEntry[i].ApCliBssid, &pHeader->Addr2)))
					{
						pAd->ApCliTab.ApCliEntry[i].RxOkCount++;
						pAd->ApCliTab.ApCliEntry[i].ReceivedByteCount += pRxD->DataByteCnt;
						break;
					}
				}			
#endif

#ifdef WDS_SUPPORT
		  		if(pHeader->FC.FrDs==1 && pHeader->FC.ToDs==1 && pAd->WdsTab.Mode != WDS_DISABLE_MODE)
		  		{
 					for (i = 0; i < MAX_WDS_ENTRY; i++)
					{
						if ((pAd->WdsTab.WdsEntry[i].Valid == TRUE) && (MAC_ADDR_EQUAL(&pAd->WdsTab.WdsEntry[i].WdsAddr, &pHeader->Addr2)))
						{
		   					pAd->WdsTab.WdsEntry[i].RxOkCount++;
		   					pAd->WdsTab.WdsEntry[i].ReceivedByteCount += pRxD->DataByteCnt;		  
 							break;
						}
					}
		  		}
		  		else
#endif // WDS_SUPPORT //

				if(pEntry)
				{
                	pAd->PortCfg.MBSSID[apidx].RxOkCount++;
                	pAd->PortCfg.MBSSID[apidx].ReceivedByteCount += pRxD->DataByteCnt;
				}
				
                if ((pEntry) && (pAd->PortCfg.bEnableHSCounter))
                {
                    pEntry->HSCounter.TotalRxByteCount += pRxD->DataByteCnt;
                    pEntry->HSCounter.LastDataPacketTime = jiffies;
                }

                // pData : Pointer skip the first 24 bytes, 802.11 HEADER
                if ((pHeader->FC.FrDs == 1) && (pHeader->FC.ToDs == 1))
                {
                    pData += LENGTH_802_11_WITH_ADDR4;
                    DataSize = (USHORT)pRxD->DataByteCnt - LENGTH_802_11_WITH_ADDR4;
                }
                else
                {
                    pData += LENGTH_802_11;
                    DataSize = (USHORT)pRxD->DataByteCnt - LENGTH_802_11;
                }

				bQoS = FALSE;

                // remove the 2 extra QOS CNTL bytes
                if (pHeader->FC.SubType & 0x08)
                {
					bQoS = TRUE;
                    pData += 2;
                    DataSize -= 2;
                    Offset += 2;
                }
                    
                // remove the 2 extra AGGREGATION bytes
                Msdu2Size = 0;
                if (pHeader->FC.Order)
                {
                    Msdu2Size = *pData + (*(pData+1) << 8);
                    if ((Msdu2Size <= 1536) && (Msdu2Size < DataSize))
                    {
                        pData += 2;
                        DataSize -= 2;
                    }
                    else
                        Msdu2Size = 0;
                }
#ifdef APCLI_SUPPORT
				// handle APCLI
				bApCliPacket = FALSE;
				if ((pHeader->FC.FrDs == 1) && (pHeader->FC.ToDs == 0))
				{
					for (i = 0; i < MAX_APCLI_ENTRY; i++)
					{
						//BSSID match the ApCliBssid ?(from a valid AP)
						if ((pAd->ApCliTab.ApCliEntry[i].Valid == TRUE)
							&& (pAd->ApCliTab.ApCliEntry[i].Enable == TRUE)
							&& (MAC_ADDR_EQUAL(&pAd->ApCliTab.ApCliEntry[i].ApCliBssid, &pHeader->Addr2)))
						{
							net_dev = pAd->ApCliTab.ApCliEntry[i].dev;
							bApCliPacket = TRUE;
							ApCliIdx = i;
							DBGPRINT(RT_DEBUG_INFO, "APCLI packet from (apcli%d)!!!\n", ApCliIdx);
							break;
						}
					}	
                    
					if (i == MAX_APCLI_ENTRY)
					{
						DBGPRINT(RT_DEBUG_INFO, "APCLI packet, but not our APCLI group, give it up!!!\n");
						break;	// give up this frame
					}
				}																	
#endif																			                    
                //
                // handle WDS
                //
                bWdsPacket = FALSE;
#ifdef WDS_SUPPORT
                if ((pHeader->FC.FrDs == 1) && (pHeader->FC.ToDs == 1))
                {
					// Only Lazy mode will auto learning, match with FrDs=1 and ToDs=1
					if (pAd->WdsTab.Mode >= WDS_LAZY_MODE)
					{
						// Lookup entry, if exist, then update the Rcv beacon time and return
						for (i = 0; i < MAX_WDS_ENTRY; i++)
						{
							if ((pAd->WdsTab.WdsEntry[i].Valid == TRUE) && (MAC_ADDR_EQUAL(&pAd->WdsTab.WdsEntry[i].WdsAddr, &pHeader->Addr2)))
							{
								// Update wds received beacon time for age out usage
								pAd->WdsTab.WdsEntry[i].WdsRcvBeaconTime = jiffies;
								break;
							}
						}

						// Look for empty entry, and add to our WdsTable
						if (i == MAX_WDS_ENTRY)
						{
							for (i = 0; i < MAX_WDS_ENTRY; i++)
							{
								if (pAd->WdsTab.WdsEntry[i].Valid == FALSE)
								{
									COPY_MAC_ADDR(&pAd->WdsTab.WdsEntry[i].WdsAddr, &pHeader->Addr2);
									pAd->WdsTab.WdsEntry[i].WdsRcvBeaconTime = jiffies;

									pAd->WdsTab.WdsEntry[i].CurrTxRate = RATE_11;
									if(pAd->PortCfg.PhyMode == PHY_11B)
										pAd->WdsTab.WdsEntry[i].MaxSupportedRate = RATE_11;
									else
										pAd->WdsTab.WdsEntry[i].MaxSupportedRate = RATE_54;
									pAd->WdsTab.WdsEntry[i].Valid = TRUE;

									// Update Pair-wise key table for this WDS entry
									if (((pAd->WdsTab.WepStatus == Ndis802_11Encryption2Enabled) || (pAd->WdsTab.WepStatus == Ndis802_11Encryption3Enabled)) && (pAd->WdsTab.Wpa_key.KeyLen > 0))
									{
										AsicAddPairwiseKeyEntry(pAd, pAd->WdsTab.WdsEntry[i].WdsAddr, WDS_PAIRWISE_KEY_OFFSET + i, pAd->WdsTab.Wpa_key.CipherAlg, 
											pAd->WdsTab.Wpa_key.Key, pAd->WdsTab.Wpa_key.TxMic, pAd->WdsTab.Wpa_key.RxMic);
									}

									DBGPRINT(RT_DEBUG_TRACE, "Lazy WDS mode, AP(%02x:%02x:%02x:%02x:%02x:%02x) has 4-addr packet and add to entry(%d) in our WDSTable \n", 
										pHeader->Addr2[0], pHeader->Addr2[1], pHeader->Addr2[2],
										pHeader->Addr2[3], pHeader->Addr2[4], pHeader->Addr2[5], i);
									break;
								}
							}
							if (i == MAX_WDS_ENTRY)
							{
								DBGPRINT(RT_DEBUG_WARN, "WDS Table is full!\n");
							}
						}
					}

					if (pAd->WdsTab.Mode != WDS_DISABLE_MODE)
					{
						for (i = 0; i < MAX_WDS_ENTRY; i++)
						{
							if ((pAd->WdsTab.WdsEntry[i].Valid == TRUE) && (MAC_ADDR_EQUAL(&pAd->WdsTab.WdsEntry[i].WdsAddr, &pHeader->Addr2)))
							{
								net_dev = pAd->WdsTab.WdsEntry[i].dev;
								bWdsPacket = TRUE;
								DBGPRINT(RT_DEBUG_INFO, "WDS packet from (%d)!!!\n", i);
								break;
							}
						}
						if (i == MAX_WDS_ENTRY)
						{
							DBGPRINT(RT_DEBUG_WARN, "WDS packet, but not our WDS group, give it up!!!\n");
							break;
						}
					}
                }
#else 
	            if ((pHeader->FC.FrDs == 1) && (pHeader->FC.ToDs == 1))
                {
					break;
				}
#endif /* !WDS_SUPPORT */

#ifdef APCLI_SUPPORT
				if (bApCliPacket)
				{
				    Privacy 	= Ndis802_11PrivFilterAcceptAll; // no 802.1x PAC for APCLI frame
					WpaState 	= pAd->ApCliTab.ApCliEntry[ApCliIdx].WpaState;
					pDA = pHeader->Addr1;
					pSA = pHeader->Addr3;
				}
				else
#endif 			            
                // WDS packet's DA & SA is not the same
                if (bWdsPacket)
                {
                    Privacy = Ndis802_11PrivFilterAcceptAll; // no 802.1x PAC for WDS frame
                    pDA = pHeader->Addr3;
                    pSA = (PUCHAR)pHeader + sizeof(HEADER_802_11);
                }
                else
                {
                    pDA = pHeader->Addr3;
                    pSA = pHeader->Addr2;
                }

                // Determine the destination of 802.1x frame (WPA state machine or upper layer TCPIP)
                // drop all non-802.1x DATA frame before this client's Port-Access-Control is secured
                if (Privacy != Ndis802_11PrivFilterAcceptAll)
                {
                    //needs to handle fragmented EAP packets specially
					if (pHeader->Frag == 0)     // Frag. Number is 0 : First frag or only one pkt
                    {
						// One & The only fragment
                    	if (pHeader->FC.MoreFrag == FALSE)
		                {
		                	if (!RTMPCheckWPAframe(pAd, (PUCHAR)pHeader, pRxD->DataByteCnt, Offset))
		                		break;

		                	if ((pEntry) && (pEntry->WpaState < AS_INITPMK) && 
		                		((pEntry->AuthMode == Ndis802_11AuthModeWPA) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2) || pAd->PortCfg.MBSSID[apidx].IEEE8021X == TRUE))
		                	{
		                		UCHAR           LLC_Len[2];
		                		
#ifdef WSC_SUPPORT                                
                                if (pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.WscConfMode != WSC_DISABLE)
                                {
                                    REPORT_MGMT_FRAME_TO_MLME(pAd, pHeader, pRxD->DataByteCnt, pRxD->PlcpRssi, Offset);
                                }       
#endif // WSC_SUPPORT //                                                 
		                		
		                		NdisMoveMemory(LLC_Len, EAPOL, 2);
		                		MAKE_802_3_HEADER(Header802_3, pDA, pSA, ((PUCHAR) LLC_Len));
#ifdef RTL865X_SOC
		                		REPORT_ETHERNET_FRAME_TO_LLC_RTL865X(pAd, Header802_3, pData, DataSize, net_dev);						
#else
		                		REPORT_ETHERNET_FRAME_TO_LLC(pAd, Header802_3, pData, DataSize, net_dev);
#endif
		                		DBGPRINT(RT_DEBUG_INFO, "Apidx=%d,pDA %x %x %x %x %x %x,pSA %x %x %x %x %x %x,pData %x %x %x %x %x \n",apidx,*pDA,*(pDA+1),*(pDA+2),*(pDA+3),*(pDA+4),*(pDA+5),
		                			*pSA,*(pSA+1),*(pSA+2),*(pSA+3),*(pSA+4),*(pSA+5),*pData,*(pData+1),*(pData+2),*(pData+3),*(pData+4));
		                	}
		                	else
		                	{
		                		REPORT_MGMT_FRAME_TO_MLME(pAd, pHeader, pRxD->DataByteCnt, pRxD->PlcpRssi, Offset);
		                	}

		                }
                    	else	//the first pkt of fragment, record it.
                    	{
                    		UCHAR           LLC_Len[2];
                    		
                    		DBGPRINT(RT_DEBUG_OFF, "Apidx=%d,Frag.0 (len:%d):pData %x %x %x %x %x \n",apidx,DataSize,*pData,*(pData+1),*(pData+2),*(pData+3),*(pData+4));
                    		NdisZeroMemory(&pAd->FragFrame, sizeof(FRAGMENT_FRAME));
                            NdisZeroMemory(Header802_3, LENGTH_802_3);
                    		
                    		// create 802.3 MAC header
                            NdisMoveMemory(LLC_Len, EAPOL, 2);
                            MAKE_802_3_HEADER(Header802_3, pDA, pSA, ((PUCHAR) LLC_Len));

                            NdisMoveMemory(&pAd->FragFrame.Buffer[LENGTH_802_3], pData, DataSize);
                            NdisMoveMemory(pAd->FragFrame.Header802_3, Header802_3, LENGTH_802_3);
                            NdisZeroMemory(Header802_3, LENGTH_802_3);
                            pAd->FragFrame.RxSize   = DataSize;
                            pAd->FragFrame.Sequence = pHeader->Sequence;
                            pAd->FragFrame.LastFrag = pHeader->Frag;       // Should be 0
						}
					}
					else	//Middle & End of fragment
                    {
                    	// Fragment is not the same sequence or out of fragment number order 
                    	if ((pHeader->Sequence != pAd->FragFrame.Sequence) ||
                        	(pHeader->Frag != (pAd->FragFrame.LastFrag + 1)))
                    	{
                        	// Fragment is not the same sequence or out of fragment number order
                        	// Clear Fragment frame contents
                        	NdisZeroMemory(&pAd->FragFrame, sizeof(FRAGMENT_FRAME));
                        	DBGPRINT(RT_DEBUG_ERROR, "Fragment is not the same sequence or out of fragment number order.\n");
                        	break; // give up this frame
                    	}   
                    	else if ((pAd->FragFrame.RxSize + DataSize) > MAX_FRAME_SIZE)
                    	{
                        	// Fragment frame is too large, it exeeds the maximum frame size.
                        	// Clear Fragment frame contents
                        	NdisZeroMemory(&pAd->FragFrame, sizeof(FRAGMENT_FRAME));
                        	DBGPRINT(RT_DEBUG_ERROR, "Fragment frame is too large, it exeeds the maximum frame size.\n");
                        	break; // give up this frame
                    	}
                    	// concatenate this fragment into the re-assembly buffer
                    	DBGPRINT(RT_DEBUG_OFF, "Apidx=%d,Frag.%d (len:%d):pData %x %x %x %x %x \n",apidx,pHeader->Frag,DataSize,*pData,*(pData+1),*(pData+2),*(pData+3),*(pData+4));
                        NdisMoveMemory(&pAd->FragFrame.Buffer[LENGTH_802_3 + pAd->FragFrame.RxSize], pData, DataSize);
                    	pAd->FragFrame.RxSize  += DataSize;
                    	pAd->FragFrame.LastFrag = pHeader->Frag;       // Update fragment number
                                
						// Last fragment
                    	if (pHeader->FC.MoreFrag == FALSE)                    	
                        {
                        	BOOLEAN CheckPktSanity = TRUE;
                        	
                        	if(pAd->FragFrame.RxSize < (LENGTH_802_1_H + LENGTH_EAPOL_H))
                        	{
                        		CheckPktSanity = FALSE;
                        		DBGPRINT(RT_DEBUG_ERROR, "Total pkts size is too small.\n");
							}	
                        	else if (!RTMPEqualMemory(SNAP_802_1H, (PUCHAR)&pAd->FragFrame.Buffer[LENGTH_802_3], 6))
                        	{
                        		CheckPktSanity = FALSE;	
                        		DBGPRINT(RT_DEBUG_ERROR, "Can't find SNAP_802_1H parameter.\n");
                        	}	 
                        	else if (!RTMPEqualMemory(EAPOL, (PUCHAR)&pAd->FragFrame.Buffer[LENGTH_802_3 + 6], 2))
                        	{
                        		CheckPktSanity = FALSE;	
                        		DBGPRINT(RT_DEBUG_ERROR, "Can't find EAPOL parameter.\n");	
                        	}	
                        	else if(pAd->FragFrame.Buffer[LENGTH_802_3 + 9] > EAPOLASFAlert)
                        	{
                        		CheckPktSanity = FALSE;	
                        		DBGPRINT(RT_DEBUG_ERROR, "Unknown EAP type(%d).\n",pAd->FragFrame.Buffer[LENGTH_802_3+9]);	
                        	}
                        	
                        	if(CheckPktSanity == FALSE)
                        	{
                        		NdisZeroMemory(&pAd->FragFrame, sizeof(FRAGMENT_FRAME));
                        		break;
                        	}	
                        	
                        	if ((pEntry) && (pEntry->WpaState < AS_INITPMK) && 
                				((pEntry->AuthMode == Ndis802_11AuthModeWPA) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2) || pAd->PortCfg.MBSSID[apidx].IEEE8021X == TRUE))
                			{
                				UCHAR           LLC_Len[2];
                		
#ifdef WSC_SUPPORT
                                if (pAd->PortCfg.MBSSID[pEntry->ApIdx].WscControl.WscConfMode != WSC_DISABLE)
                                {
                                    REPORT_MGMT_FRAME_TO_MLME(pAd, pHeader, pRxD->DataByteCnt, pRxD->PlcpRssi, Offset);
                                }       
#endif // WSC_SUPPORT //
                        
                				NdisMoveMemory(LLC_Len, EAPOL, 2);
                				MAKE_802_3_HEADER(Header802_3, pDA, pSA, ((PUCHAR) LLC_Len));
#ifdef RTL865X_SOC
		                		REPORT_ETHERNET_FRAME_TO_LLC_RTL865X(pAd, Header802_3, &pAd->FragFrame.Buffer[LENGTH_802_3], pAd->FragFrame.RxSize, net_dev);						
#else
                				REPORT_ETHERNET_FRAME_TO_LLC(pAd, Header802_3, &pAd->FragFrame.Buffer[LENGTH_802_3], pAd->FragFrame.RxSize, net_dev);
#endif
                				DBGPRINT(RT_DEBUG_OFF, "Apidx=%d,sned whole pkt: pDA %x %x %x %x %x %x,pSA %x %x %x %x %x %x\n",apidx,*pDA,*(pDA+1),*(pDA+2),*(pDA+3),*(pDA+4),*(pDA+5),
                										*pSA,*(pSA+1),*(pSA+2),*(pSA+3),*(pSA+4),*(pSA+5));
                			}
                        
                        	// Clear Fragment frame contents
                            NdisZeroMemory(&pAd->FragFrame, sizeof(FRAGMENT_FRAME));
                            DBGPRINT(RT_DEBUG_INFO, "!!! Frame with Fragment Indicated !!!\n");	
                        }	                                    
                    }
                	break; // end of processing this frame
                }

                if (RTMPCheckWPAframe(pAd, (PUCHAR)pHeader, pRxD->DataByteCnt, Offset))
                {
#ifdef APCLI_SUPPORT						
					// Special DATA frame that has to pass to MLME
					// EAPOL handshaking frames when driver supplicant enabled, pass to MLME for special process
					if (bApCliPacket)
					{
// For AP Client support WPS Modification
//						if (pAd->ApCliTab.ApCliEntry[ApCliIdx].WpaState != SS_NOTUSE)
						if ((pAd->ApCliTab.ApCliEntry[ApCliIdx].WpaState != SS_NOTUSE)
#ifdef WSC_SUPPORT
						|| (pAd->ApCliTab.ApCliEntry[ApCliIdx].WscControl.WscStatus != STATUS_WSC_NOTUSED)
#endif // WSC_SUPPORT //
						)
// --
                			REPORT_MGMT_FRAME_TO_MLME(pAd, pHeader, pRxD->DataByteCnt, pRxD->PlcpRssi, Offset);
					}
					else
                	REPORT_MGMT_FRAME_TO_MLME(pAd, pHeader, pRxD->DataByteCnt, pRxD->PlcpRssi, Offset);
#else
                	REPORT_MGMT_FRAME_TO_MLME(pAd, pHeader, pRxD->DataByteCnt, pRxD->PlcpRssi, Offset);
#endif	
                	break;
                }
                
                // Security check, our ASIC will accept un-encrypt packets, although the security is enabled.
                // Driver needs to drop it.
                if (pRxD->CipherAlg == CIPHER_NONE)
                {
                	if ((pHeader->FC.FrDs == 1) && (pHeader->FC.ToDs == 1))
                	{
                		// WDS packets
                		if (pAd->WdsTab.WepStatus != Ndis802_11WEPDisabled)
                		{
                			DBGPRINT(RT_DEBUG_ERROR, "WDS has security enabled, but receive un-encrypt packet\n");
                			break;
                		}
                	}
                	else if (pEntry)
                	{
                		// BSS packets
                		if (pAd->PortCfg.MBSSID[pEntry->ApIdx].WepStatus != Ndis802_11WEPDisabled)
                		{
                			DBGPRINT(RT_DEBUG_ERROR, "AP has security enabled, but receive un-encrypt packet\n");
                			break;
                		}
                	}
                }

                // First or Only fragment
                if (pHeader->Frag == 0)     
                {
                    PUCHAR pRemovedLLCSNAP;
                           
                    CONVERT_TO_802_3(Header802_3, pDA, pSA, pData, DataSize, pRemovedLLCSNAP);
                                                                      
#ifdef IGMP_SNOOP_SUPPORT
					if ((pAd->PortCfg.MBSSID[apidx].IgmpSnoopEnable)
#ifdef APCLI_SUPPORT
						&& (bApCliPacket == FALSE)
#endif // APCLI_SUPPORT //
#ifdef WDS_SUPPORT
						&& (bWdsPacket == FALSE)
#endif // WDS_SUPPORT //
						&& IS_MULTICAST_MAC_ADDR(pDA))
					{
						IGMPSnooping(pAd->pMulticastFilterTable, pDA, pSA, pData - 2, net_dev);
					}
#endif // IGMP_SNOOP_SUPPORT //
                           
                    // One & The only fragment
                    if (pHeader->FC.MoreFrag == FALSE)
                    {
                        if ((pHeader->FC.Order == 1) && (Msdu2Size > 0)) // this is an aggregation
                        {
                            USHORT Payload1Size, Payload2Size;
                            PUCHAR pData2;
                           
                            pAd->RalinkCounters.OneSecRxAggregationCount ++;
                            Payload1Size = DataSize - Msdu2Size;
                            Payload2Size = Msdu2Size - LENGTH_802_3;

#ifdef GEM_LEDMODE
			    			pAd->Mlme.bLEDblinking = TRUE;
#endif
                            // check if DA is another associted WSTA reachable via wireless bridging,
                            // if it is, then no need to indicate to LLC
                            if (RTMPBridgeToWdsAndWirelessSta(pAd, Header802_3, LENGTH_802_3, pData, Payload1Size, apidx))
                            {
                                pData2 = pData + Payload1Size + LENGTH_802_3;
                                RTMPBridgeToWdsAndWirelessSta(pAd, Header802_3, LENGTH_802_3, pData2, Payload2Size, apidx);
                            }
                            else
                            {
#ifndef NONCOPY_RX
                                REPORT_ETHERNET_FRAME_TO_LLC(pAd, Header802_3, pData, Payload1Size, net_dev);
                                DBGPRINT(RT_DEBUG_INFO, "!!! report segregated MSDU1 to LLC (len=%d, proto=%02x:%02x) %02x:%02x:%02x:%02x-%02x:%02x:%02x:%02x\n",
                                    LENGTH_802_3+Payload1Size, Header802_3[12], Header802_3[13],
                                    *pData, *(pData+1),*(pData+2),*(pData+3),*(pData+4),*(pData+5),*(pData+6),*(pData+7));
                                    
                                pData2 = pData + Payload1Size + LENGTH_802_3;
                                REPORT_ETHERNET_FRAME_TO_LLC(pAd, pData + Payload1Size, pData2, Payload2Size, net_dev);
                                DBGPRINT(RT_DEBUG_INFO, "!!! report segregated MSDU2 to LLC (len=%d, proto=%02x:%02x) %02x:%02x:%02x:%02x-%02x:%02x:%02x:%02x\n",
                                    LENGTH_802_3+Payload2Size, *(pData2 -2), *(pData2 - 1),
                                    *pData2, *(pData2+1),*(pData2+2),*(pData2+3),*(pData2+4),*(pData2+5),*(pData2+6),*(pData2+7));
#else
#ifndef	RTL865X_SOC
								if (bQoS == TRUE) {
	                                REPORT_ETHERNET_FRAME_TO_LLC(pAd, Header802_3, pData, Payload1Size, net_dev);
	                                DBGPRINT(RT_DEBUG_INFO, "!!! report segregated MSDU1 to LLC (len=%d, proto=%02x:%02x) %02x:%02x:%02x:%02x-%02x:%02x:%02x:%02x\n",
	                                    LENGTH_802_3+Payload1Size, Header802_3[12], Header802_3[13],
    	                                *pData, *(pData+1),*(pData+2),*(pData+3),*(pData+4),*(pData+5),*(pData+6),*(pData+7));
                                    
        	                        pData2 = pData + Payload1Size + LENGTH_802_3;
            	                    REPORT_ETHERNET_FRAME_TO_LLC(pAd, pData + Payload1Size, pData2, Payload2Size, net_dev);
                	                DBGPRINT(RT_DEBUG_INFO, "!!! report segregated MSDU2 to LLC (len=%d, proto=%02x:%02x) %02x:%02x:%02x:%02x-%02x:%02x:%02x:%02x\n",
                    	                LENGTH_802_3+Payload2Size, *(pData2 -2), *(pData2 - 1),
                        	            *pData2, *(pData2+1),*(pData2+2),*(pData2+3),*(pData2+4),*(pData2+5),*(pData2+6),*(pData2+7));									
								} else 
#endif																		
								{
									REPORT_AGGREGATE_ETHERNET_FRAME_TO_LLC_WITH_NON_COPY(pAd, Header802_3, pData, Payload1Size, Payload2Size, net_dev);
		                            pRxD->BufPhyAddr = pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocPa;									
								}
#endif
                            }
                        }
                        else
                        {
#ifdef GEM_LEDMODE
			    			pAd->Mlme.bLEDblinking = TRUE;
#endif
                            // check if DA is another associted WSTA reachable via wireless bridging,
                            // if it is, then no need to indicate to LLC
                            if (RTMPBridgeToWdsAndWirelessSta(pAd, Header802_3, LENGTH_802_3, pData, DataSize, apidx))
                                break;
                                   
                            if (bQoS == TRUE) {
	                            REPORT_ETHERNET_FRAME_TO_LLC(pAd, Header802_3, pData, DataSize, net_dev);                            	
                            } else {
	                            REPORT_ETHERNET_FRAME_TO_LLC_WITH_NON_COPY(pAd, Header802_3, pData, DataSize, net_dev);
#ifdef NONCOPY_RX
	                            pRxD->BufPhyAddr = pAd->RxRing.Cell[pAd->RxRing.CurRxIndex].DmaBuf.AllocPa;
#endif
							}
                            DBGPRINT(RT_DEBUG_INFO, "!!! report DATA (no frag) to LLC (len=%d, proto=%02x:%02x) %02x:%02x:%02x:%02x-%02x:%02x:%02x:%02x\n",
                                DataSize, Header802_3[12], Header802_3[13],
                                *pData, *(pData+1),*(pData+2),*(pData+3),*(pData+4),*(pData+5),*(pData+6),*(pData+7));
                        }
                    }
                                
                    // First fragment - record the 802.3 header and frame body
                    else
                    {
                    	pAd->FragFrame.Flags &= 0xFFFFFFFE;

                    	// Firt Fragment & LLC/SNAP been removed. Keep the removed LLC/SNAP for later on
	                    // TKIP MIC verification.
	                    if (pHeader->FC.MoreFrag && pRemovedLLCSNAP)
	                    {
	                        NdisMoveMemory(pAd->FragFrame.Header_LLC, pRemovedLLCSNAP, LENGTH_802_1_H);
	                        pAd->FragFrame.Flags |= 0x01;
	                    }
	                    
                        NdisMoveMemory(&pAd->FragFrame.Buffer[LENGTH_802_3], pData, DataSize);
                        NdisMoveMemory(pAd->FragFrame.Header802_3, Header802_3, LENGTH_802_3);
                        pAd->FragFrame.RxSize   = DataSize;
                        pAd->FragFrame.Sequence = pHeader->Sequence;
                        pAd->FragFrame.LastFrag = pHeader->Frag;       // Should be 0
                    }
                }

                // Middle & End of fragment burst
                else
                {
                    // No LLC-SNAP header in except the first fragment frame
                    if ((pHeader->Sequence != pAd->FragFrame.Sequence) ||
                        (pHeader->Frag != (pAd->FragFrame.LastFrag + 1)))
                    {
                        // Fragment is not the same sequence or out of fragment number order
                        // Clear Fragment frame contents
                        NdisZeroMemory(&pAd->FragFrame, sizeof(FRAGMENT_FRAME));
                        break; // give up this frame
                    }   
                    else if ((pAd->FragFrame.RxSize + DataSize) > MAX_FRAME_SIZE)
                    {
                        // Fragment frame is too large, it exeeds the maximum frame size.
                        // Clear Fragment frame contents
                        NdisZeroMemory(&pAd->FragFrame, sizeof(FRAGMENT_FRAME));
                        break; // give up this frame
                    }
                                                    
                    // concatenate this fragment into the re-assembly buffer
                    NdisMoveMemory(&pAd->FragFrame.Buffer[LENGTH_802_3 + pAd->FragFrame.RxSize], pData, DataSize);
                    pAd->FragFrame.RxSize  += DataSize;
                    pAd->FragFrame.LastFrag = pHeader->Frag;       // Update fragment number
                                                            
                    // Last fragment
                    if (pHeader->FC.MoreFrag == FALSE)
                    {
                        // For TKIP frame, calculate the MIC value
                        if (pRxD->CipherAlg == CIPHER_TKIP)
                        {
                            PCIPHER_KEY pWpaKey;

                            if (!pEntry)
                        		break;
                       		pWpaKey = &pEntry->PairwiseKey;
                        
                            // Minus MIC length
                            pAd->FragFrame.RxSize -= 8;
                                                                    
                            if (pAd->FragFrame.Flags & 0x00000001)
                            {
                                // originally there's an LLC/SNAP field in the first fragment
                                // but been removed in re-assembly buffer. here we have to include
                                // this LLC/SNAP field upon calculating TKIP MIC
                                // pData = pAd->FragFrame.Header_LLC;
                                // Copy LLC data to the position in front of real data for MIC calculation
                                NdisMoveMemory(&pAd->FragFrame.Buffer[LENGTH_802_3 - LENGTH_802_1_H],
                                    pAd->FragFrame.Header_LLC, 
                                    LENGTH_802_1_H);
                                pData = (PUCHAR) &pAd->FragFrame.Buffer[LENGTH_802_3 - LENGTH_802_1_H];                                        
                                DataSize = (USHORT)pAd->FragFrame.RxSize + LENGTH_802_1_H;
                            }
                            else
                            {
                                pData = (PUCHAR) &pAd->FragFrame.Buffer[LENGTH_802_3];
                                DataSize = (USHORT)pAd->FragFrame.RxSize;
                            }
                        
                            if (RTMPTkipCompareMICValue(pAd,
                                                        pData,
                                                        pDA,
                                                        pSA,
                                                        pWpaKey->RxMic,
                                                        DataSize) == FALSE)
                            {
                                DBGPRINT(RT_DEBUG_ERROR,"Rx MIC Value error 2\n");                            
                                if (pEntry)
                                {
                                	HandleCounterMeasure(pAd, pEntry);
                                }
                                break;  // give up this frame
                            }
                                                    
                        }               

#ifdef GEM_LEDMODE
			    		pAd->Mlme.bLEDblinking = TRUE;
#endif
                        // check if DA is another associted WSTA reachable via wireless bridging,
                        // if it is, then no need to indicate to LLC
                        if (RTMPBridgeToWdsAndWirelessSta(pAd, pAd->FragFrame.Header802_3, LENGTH_802_3, &pAd->FragFrame.Buffer[LENGTH_802_3], pAd->FragFrame.RxSize, apidx) == FALSE)
                        {
                            pData = &pAd->FragFrame.Buffer[LENGTH_802_3];
                            REPORT_ETHERNET_FRAME_TO_LLC(pAd, pAd->FragFrame.Header802_3, pData, pAd->FragFrame.RxSize, net_dev);
							DBGPRINT(RT_DEBUG_INFO, "!!! report DATA (fragmented) to LLC (len=%d) !!!\n", pAd->FragFrame.RxSize);
                        }
                                
                        // Clear Fragment frame contents
                        NdisZeroMemory(&pAd->FragFrame, sizeof(FRAGMENT_FRAME));
                    }
                }
                break;
            }

            //
            // CASE II. receive a MGMT frame
            //
            else if (pHeader->FC.Type == BTYPE_MGMT)
            {
                if (pAd->PortCfg.MBSSID[apidx].BANClass3Data == TRUE)
                {
                    // disallow new association
                    if ((pHeader->FC.SubType == SUBTYPE_ASSOC_REQ) || (pHeader->FC.SubType == SUBTYPE_AUTH))
                    {
                        DBGPRINT(RT_DEBUG_TRACE, "Disallow new Association \n ");
                        break; // give up this frame
                    }
                }
                
                REPORT_MGMT_FRAME_TO_MLME(pAd, pHeader, pRxD->DataByteCnt, pRxD->PlcpRssi, Offset);
                break;  // end of processing this frame
            }

            //
            // CASE III. receive a CNTL frame
            //
            else if (pHeader->FC.Type == BTYPE_CNTL)
            {
                // handle PS-POLL here
                if ((pRxD->U2M) && (pHeader->FC.SubType == SUBTYPE_PS_POLL))
                {
                    PUCHAR pAddr = pHeader->Addr2;
                                        
                    pEntry = MacTableLookup(pAd, pAddr);
                	if (pEntry)
                	{
	                    if (pHeader->FC.PwrMgmt)
	                    	RTMPHandleRxPsPoll(pAd, pAddr, pEntry, FALSE);
						else
							RTMPHandleRxPsPoll(pAd, pAddr, pEntry, TRUE);
                	}
                }
                else
                {
                     DBGPRINT(RT_DEBUG_TRACE,"ignore CNTL (subtype=%d)\n", pHeader->FC.SubType);
                }
                break; // end of processing this frame
            }

            //
            // CASE IV. receive a frame of invalid type
            //
            else
                break; // give up this frame
        
        } while (FALSE);

        pRxD->Owner = DESC_OWN_NIC;
#ifdef BIG_ENDIAN
        RTMPDescriptorEndianChange((PUCHAR)pRxD, TYPE_RXD);    
        WriteBackToDescriptor((PUCHAR)pDestRxD, (PUCHAR)pRxD, FALSE, TYPE_RXD);
#endif
        INC_RING_INDEX(pAd->RxRing.CurRxIndex, RX_RING_SIZE);
    }
        
    // Make sure to release Rx ring resource
    RTMP_SEM_UNLOCK(&pAd->RxRingLock, IrqFlags);
}

/*
	========================================================================

	Routine	Description:
		Process	TxDone interrupt, running in	DPC	level

	Arguments:
		Adapter		Pointer	to our adapter

	Return Value:
		None

	Note:

	========================================================================
*/
VOID	RTMPHandleTxDoneInterrupt(
	IN	PRTMP_ADAPTER	pAd)
{
	STA_CSR4_STRUC csr;
	int i;
	UCHAR UpRate;

//	DBGPRINT(RT_DEBUG_INFO, "====> RTMPHandleTxDoneInterrupt\n");

    for (i=0; i<MAX_TX_DONE_PROCESS; i++)
    {
        RTMP_IO_READ32(pAd, STA_CSR4, &csr.word);
        if (! csr.field.bValid)
            break;

        pAd->RalinkCounters.OneSecTxDoneCount++;
        switch (csr.field.TxResult)
        {
    		case TX_RESULT_SUCCESS:			// Success with or without retry
		    	// DBGPRINT(RT_DEBUG_INFO, "TX Success without retry<<<\n");
	    		pAd->WlanCounters.TransmittedFragmentCount++;
    			pAd->Counters8023.GoodTransmits++;

                if ((csr.field.PidType == (PTYPE_SPECIAL >> 6)) && (csr.field.PidSubtype == PSUBTYPE_RTS))
                    pAd->WlanCounters.RTSSuccessCount++;
                    
                // case 1. no retry
                if (csr.field.RetryCount == 0)
                {
                    // update DRS related counters
                    if (csr.field.PidType == (PTYPE_DATA_REQUIRE_ACK >> 6))
                    {
                        pAd->RalinkCounters.OneSecTxNoRetryOkCount ++;

#ifdef APCLI_SUPPORT
						if (csr.field.PidSubtype >= APCLI_PAIRWISE_KEY_OFFSET)						
                            pAd->ApCliTab.ApCliEntry[csr.field.PidSubtype - APCLI_PAIRWISE_KEY_OFFSET].OneSecTxOkCount++;
                        else
#endif					
						if (csr.field.PidSubtype >= WDS_PAIRWISE_KEY_OFFSET)
                            pAd->WdsTab.WdsEntry[csr.field.PidSubtype - WDS_PAIRWISE_KEY_OFFSET].OneSecTxOkCount++;
						else
						{ // AP Mode
							// reset NoDateIdleCount when successfully send packet to STA.
							pAd->MacTab.Content[csr.field.PidSubtype].NoDataIdleCount = 0;
                            pAd->MacTab.Content[csr.field.PidSubtype].OneSecTxOkCount++;
						}
                    }
                }
                else
                {
                    // case 2. one or multiple retry
    			    pAd->WlanCounters.RetryCount++;
        			pAd->WlanCounters.ACKFailureCount++;
    			    if (csr.field.RetryCount > 1)
    			    {
    				    pAd->WlanCounters.MultipleRetryCount++;
    				    pAd->Counters8023.MoreCollisions++;
        			}
    	    		else
        				pAd->Counters8023.OneCollision++;
			
                    // update DRS related counters
                    if (csr.field.PidType == (PTYPE_NULL_AT_HIGH_RATE >> 6))
                    {
                        // DRS - must be NULL frame retried @ UpRate; downgrade 
                        //       TxQuality[UpRate] so that not upgrade TX rate
#ifdef APCLI_SUPPORT
						if (csr.field.PidSubtype >= APCLI_PAIRWISE_KEY_OFFSET)
                        {
							UCHAR ApCliIdx = (UCHAR)(csr.field.PidSubtype - APCLI_PAIRWISE_KEY_OFFSET);

                        	if (pAd->PortCfg.Channel > 14)
                            	UpRate = Phy11ANextRateUpward[pAd->ApCliTab.ApCliEntry[ApCliIdx].CurrTxRate];
                        	else
                            	UpRate = Phy11BGNextRateUpward[pAd->ApCliTab.ApCliEntry[ApCliIdx].CurrTxRate];
                        	
                            pAd->ApCliTab.ApCliEntry[ApCliIdx].TxQuality[UpRate] += 2;
                            if (pAd->ApCliTab.ApCliEntry[ApCliIdx].TxQuality[UpRate] > DRS_TX_QUALITY_WORST_BOUND)
                                pAd->ApCliTab.ApCliEntry[ApCliIdx].TxQuality[UpRate] = DRS_TX_QUALITY_WORST_BOUND;																		
                        }
						else // operate in WDS mode					
#endif					
						if (csr.field.PidSubtype >= WDS_PAIRWISE_KEY_OFFSET) 		
                        {
                            UCHAR WdsIdx = (UCHAR)(csr.field.PidSubtype - WDS_PAIRWISE_KEY_OFFSET);

                            if (pAd->PortCfg.Channel > 14)
                            	UpRate = Phy11ANextRateUpward[pAd->WdsTab.WdsEntry[WdsIdx].CurrTxRate];
                            else
                            	UpRate = Phy11BGNextRateUpward[pAd->WdsTab.WdsEntry[WdsIdx].CurrTxRate];
                            
                            pAd->WdsTab.WdsEntry[WdsIdx].TxQuality[UpRate] += 2;
                            if (pAd->WdsTab.WdsEntry[WdsIdx].TxQuality[UpRate] > DRS_TX_QUALITY_WORST_BOUND)
                                pAd->WdsTab.WdsEntry[WdsIdx].TxQuality[UpRate] = DRS_TX_QUALITY_WORST_BOUND;
                        }
						else  // operate in AP mode
                        {
                        	if (pAd->PortCfg.Channel > 14)
                        		UpRate = Phy11ANextRateUpward[pAd->MacTab.Content[csr.field.PidSubtype].CurrTxRate];
                        	else
                        		UpRate = Phy11BGNextRateUpward[pAd->MacTab.Content[csr.field.PidSubtype].CurrTxRate];
                        	
							// reset NoDateIdleCount when successfully send packet to STA.
							pAd->MacTab.Content[csr.field.PidSubtype].NoDataIdleCount = 0;

                            pAd->MacTab.Content[csr.field.PidSubtype].TxQuality[UpRate] += 2;
                            if (pAd->MacTab.Content[csr.field.PidSubtype].TxQuality[UpRate] > DRS_TX_QUALITY_WORST_BOUND)
                                pAd->MacTab.Content[csr.field.PidSubtype].TxQuality[UpRate] = DRS_TX_QUALITY_WORST_BOUND;
                        }
                    }
                    else if (csr.field.PidType == (PTYPE_DATA_REQUIRE_ACK >> 6))
                    {
                        pAd->RalinkCounters.OneSecTxRetryOkCount ++;
#ifdef APCLI_SUPPORT
						if (csr.field.PidSubtype >= APCLI_PAIRWISE_KEY_OFFSET)
							pAd->ApCliTab.ApCliEntry[csr.field.PidSubtype - APCLI_PAIRWISE_KEY_OFFSET].OneSecTxRetryOkCount++;
						else //WDS
#endif                    
                        if (csr.field.PidSubtype >= WDS_PAIRWISE_KEY_OFFSET)
                            pAd->WdsTab.WdsEntry[csr.field.PidSubtype - WDS_PAIRWISE_KEY_OFFSET].OneSecTxRetryOkCount++;
						else
						{ //AP
							// reset NoDateIdleCount when successfully send packet to STA.
							pAd->MacTab.Content[csr.field.PidSubtype].NoDataIdleCount = 0;
                            pAd->MacTab.Content[csr.field.PidSubtype].OneSecTxRetryOkCount++;
                    }
                }
				}
			    break;
			  
		    case TX_RESULT_RETRY_FAIL:				// Fail on hitting retry count limit
			    DBGPRINT(RT_DEBUG_INFO, "TX Failed (RETRY LIMIT)<<<\n");
			    pAd->WlanCounters.FailedCount++;
			    pAd->WlanCounters.ACKFailureCount++;
			    pAd->Counters8023.TxErrors++;
                if ((csr.field.PidType == (PTYPE_SPECIAL >> 6)) && (csr.field.PidSubtype == PSUBTYPE_RTS))
                    pAd->WlanCounters.RTSFailureCount++;
			
                // update DRS related counters
                if (csr.field.PidType == (PTYPE_NULL_AT_HIGH_RATE >> 6))
                {
                    // DRS - must be NULL frame failed @ UpRate; downgrade 
                    //       TxQuality[UpRate] so that not upgrade TX rate
#ifdef APCLI_SUPPORT
                    if (csr.field.PidSubtype >= APCLI_PAIRWISE_KEY_OFFSET)
                    {
                        UCHAR ApCliIdx = (UCHAR)(csr.field.PidSubtype - APCLI_PAIRWISE_KEY_OFFSET);

                        UpRate = Phy11BGNextRateUpward[pAd->ApCliTab.ApCliEntry[ApCliIdx].CurrTxRate];
                        pAd->ApCliTab.ApCliEntry[ApCliIdx].TxQuality[UpRate] = DRS_TX_QUALITY_WORST_BOUND;										
                    }
                    else
#endif
                    if (csr.field.PidSubtype >= WDS_PAIRWISE_KEY_OFFSET) // operate in AP mode
                    {
                        UCHAR WdsIdx = (UCHAR)(csr.field.PidSubtype - WDS_PAIRWISE_KEY_OFFSET);

                        UpRate = Phy11BGNextRateUpward[pAd->WdsTab.WdsEntry[WdsIdx].CurrTxRate];
                        pAd->WdsTab.WdsEntry[WdsIdx].TxQuality[UpRate] = DRS_TX_QUALITY_WORST_BOUND;
                    }
                    else
                    {
                        UpRate = Phy11BGNextRateUpward[pAd->MacTab.Content[csr.field.PidSubtype].CurrTxRate];
                        pAd->MacTab.Content[csr.field.PidSubtype].TxQuality[UpRate] = DRS_TX_QUALITY_WORST_BOUND;
                    }
                }
                else if (csr.field.PidType == (PTYPE_DATA_REQUIRE_ACK >> 6))
                {
                    pAd->RalinkCounters.OneSecTxFailCount ++;

#ifdef APCLI_SUPPORT
					if (csr.field.PidSubtype >= APCLI_PAIRWISE_KEY_OFFSET)
					{
					 	UCHAR ApCliIdx = (UCHAR)(csr.field.PidSubtype - APCLI_PAIRWISE_KEY_OFFSET);
                        pAd->ApCliTab.ApCliEntry[ApCliIdx].OneSecTxFailCount ++;
						pAd->ApCliTab.ApCliEntry[ApCliIdx].TxErrors ++;
						pAd->ApCliTab.ApCliEntry[ApCliIdx].TxDrops ++;
					}
                    else
#endif					
                    // for WDS
                    if (csr.field.PidSubtype >= WDS_PAIRWISE_KEY_OFFSET)                
                    {
                        UCHAR WdsIdx = (UCHAR)(csr.field.PidSubtype - WDS_PAIRWISE_KEY_OFFSET);
                        pAd->WdsTab.WdsEntry[WdsIdx].OneSecTxFailCount ++;
						// statistics - tx error & drop frame
                        pAd->WdsTab.WdsEntry[WdsIdx].TxErrors++;
			   			pAd->WdsTab.WdsEntry[WdsIdx].TxDrops++;
                    }
					else
					{
                        UCHAR idx = pAd->MacTab.Content[csr.field.PidSubtype].ApIdx;
                        
						pAd->MacTab.Content[csr.field.PidSubtype].OneSecTxFailCount ++;
						// statistics - tx error & drop frame
						pAd->PortCfg.MBSSID[idx].TxErrors ++;
						pAd->PortCfg.MBSSID[idx].TxDrops ++;
                	}
                }

			    break;
			
		    default:
			    DBGPRINT(RT_DEBUG_ERROR, "TX Failed (%04x)<<<\n",csr.word);
			    pAd->WlanCounters.FailedCount++;
			    pAd->WlanCounters.ACKFailureCount++;
			    pAd->Counters8023.TxErrors++;
			    break;			
        }
    }
}

/*
	========================================================================

	Routine	Description:
		Process	TX Rings DMA Done interrupt, running in	DPC	level

	Arguments:
		Adapter		Pointer	to our adapter

	Return Value:
		None

	========================================================================
*/
VOID    RTMPHandleTxRingDmaDoneInterrupt(
	IN	PRTMP_ADAPTER	pAdapter,
	IN  INT_SOURCE_CSR_STRUC TxRingBitmap)
{
	UMAXLONG		IrqFlags;

	// Make sure Tx ring resource won't be used by other threads
#ifndef THREAD_ISR
	RTMP_SEM_LOCK(&pAdapter->TxRingLock, IrqFlags);
#else
	RTMP_IRQ_LOCK(IrqFlags);
#endif

    if (TxRingBitmap.field.Ac0DmaDone)
        RTMPFreeTXDUponTxDmaDone(pAdapter, QID_AC_BE, TX_RING_SIZE);
    
    if (TxRingBitmap.field.HccaDmaDone)
        RTMPFreeTXDUponTxDmaDone(pAdapter, QID_HCCA, TX_RING_SIZE);

    if (TxRingBitmap.field.Ac3DmaDone)
        RTMPFreeTXDUponTxDmaDone(pAdapter, QID_AC_VO, TX_RING_SIZE);

    if (TxRingBitmap.field.Ac2DmaDone)
        RTMPFreeTXDUponTxDmaDone(pAdapter, QID_AC_VI, TX_RING_SIZE);

    if (TxRingBitmap.field.Ac1DmaDone)
        RTMPFreeTXDUponTxDmaDone(pAdapter, QID_AC_BK, TX_RING_SIZE);
    
	// Make sure to release Tx ring resource
#ifndef THREAD_ISR
	RTMP_SEM_UNLOCK(&pAdapter->TxRingLock, IrqFlags);
#else
	RTMP_IRQ_UNLOCK(IrqFlags);
#endif
	
	// Dequeue one frame from TxSwQueue[] and process it
	if ((!RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS)) && 
		(!RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_RESET_IN_PROGRESS)) &&
		(!RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_HALT_IN_PROGRESS)))
	{
		// let the subroutine select the right priority Tx software queue
		RTMPDeQueuePacket(pAdapter, FALSE, FALSE);
	}
}

/*
	========================================================================

	Routine	Description:
		Process	MGMT ring DMA done interrupt, running in DPC level

	Arguments:
		pAdapter		Pointer	to our adapter

	Return Value:
		None

	Note:

	========================================================================
*/
VOID	RTMPHandleMgmtRingDmaDoneInterrupt(
	IN	PRTMP_ADAPTER	pAdapter)
{
    PTXD_STRUC		pTxD;
#ifdef BIG_ENDIAN
    PTXD_STRUC      pDestTxD;
    TXD_STRUC       TxD;
#endif
    struct sk_buff	*pSkb;
    int				i;
    UMAXLONG		IrqFlags;

    DBGPRINT(RT_DEBUG_INFO, "RTMPHandleMgmtRingDmaDoneInterrupt...\n");

	RTMP_SEM_LOCK(&pAdapter->MgmtRingLock, IrqFlags);
    for (i=0; i<MGMT_RING_SIZE; i++)
    {

#ifndef BIG_ENDIAN
	    pTxD = (PTXD_STRUC)	(pAdapter->MgmtRing.Cell[pAdapter->MgmtRing.NextTxDmaDoneIndex].AllocVa);
#else
        pDestTxD = (PTXD_STRUC) (pAdapter->MgmtRing.Cell[pAdapter->MgmtRing.NextTxDmaDoneIndex].AllocVa);
        TxD = *pDestTxD;
        pTxD = &TxD;
		RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif /* BIG_ENDIAN */

        if ((pTxD->Owner == DESC_OWN_NIC) || (pTxD->bWaitingDmaDoneInt == 0))
	        break;

		pci_unmap_single(pAdapter->pPci_Dev, pTxD->BufPhyAddr0, pTxD->BufLen0, PCI_DMA_TODEVICE);

#ifdef CONFIG_64BIT
	    pSkb = pAdapter->MgmtRing.Cell[pAdapter->MgmtRing.NextTxDmaDoneIndex].pSkb;
        if (pSkb)
        {
//iverson
       UAPSD_QoSNullTxDoneHandle(pAdapter, pSkb, GETOSPKTDATAPTR(pSkb));     
//iverson end
            pAdapter->MgmtRing.Cell[pAdapter->MgmtRing.NextTxDmaDoneIndex].pSkb = 0;
            RELEASE_NDIS_PACKET(pAdapter, pSkb);
        }
#else
	    pSkb = pTxD->pSkb;
        if (pSkb)
        {
//iverson
       UAPSD_QoSNullTxDoneHandle(pAdapter, pSkb, GETOSPKTDATAPTR(pSkb));     
//iverson end
            pTxD->pSkb = 0;
            RELEASE_NDIS_PACKET(pAdapter, pSkb);
        }
#endif // CONFIG_64BIT //

	    pTxD->bWaitingDmaDoneInt = 0;
	    INC_RING_INDEX(pAdapter->MgmtRing.NextTxDmaDoneIndex, MGMT_RING_SIZE);
//      pAdapter->RalinkCounters.TransmittedByteCount +=  pTxD->DataByteCnt;

#ifdef BIG_ENDIAN
        RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
        WriteBackToDescriptor((PUCHAR)pDestTxD, (PUCHAR)pTxD, TRUE, TYPE_TXD);
#endif			
		
    }
	RTMP_SEM_UNLOCK(&pAdapter->MgmtRingLock, IrqFlags);
}

/*
	========================================================================

	Routine	Description:
		API for MLME to transmit management frame to AP (BSS Mode)
	or station (IBSS Mode)
	
	Arguments:
		pAdapter	Pointer	to our adapter
		pData		Pointer to the outgoing 802.11 frame
		Length		Size of outgoing management frame
		
	Return Value:
		NDIS_STATUS_FAILURE
		NDIS_STATUS_PENDING
		NDIS_STATUS_SUCCESS

	Note:
	
	========================================================================
*/
NDIS_STATUS	MiniportMMRequest(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	PUCHAR			pData,
	IN	UINT			Length)
{
    struct sk_buff	*pSkb = NULL;
    NDIS_STATUS		Status = NDIS_STATUS_SUCCESS;
    UCHAR			FreeNum;

    ASSERT(Length <= MGMT_DMA_BUFFER_SIZE);

    do 
    {
    	// Check Free priority queue
    	Status = RTMPFreeTXDRequest(pAdapter, QID_MGMT, 1, &FreeNum);
	    if (Status == NDIS_STATUS_SUCCESS)
    	{
            Status = RTMPAllocateNdisPacket(pAdapter, &pSkb, pData, Length);
            if (Status != NDIS_STATUS_SUCCESS)
            {
    	    	DBGPRINT(RT_DEBUG_WARN, "MiniportMMRequest (error:: can't allocate skb buffer)\n");
	    	    break;
            }
            
			Status = MlmeHardTransmit(pAdapter, pSkb);
			if (Status != NDIS_STATUS_SUCCESS)
                RELEASE_NDIS_PACKET(pAdapter, pSkb);
		}
    	else
	    {
    		pAdapter->RalinkCounters.MgmtRingFullCount++;
    		
    		// Kick MGMT ring transmit
    		RTMP_IO_WRITE32(pAdapter, TX_CNTL_CSR, 0x00000010);
    		
		    DBGPRINT(RT_DEBUG_TRACE, "Not enough space in MgmtRing\n");
    	}
    } while (FALSE);

    return Status;
}

/*
	========================================================================

	Routine	Description:
		Copy frame from waiting queue into relative ring buffer and set 
	appropriate ASIC register to kick hardware transmit function
	
	Arguments:
		pAdapter	Pointer	to our adapter
		pBuffer		Pointer to  memory of outgoing frame
		Length		Size of outgoing management frame
		
	Return Value:
		NDIS_STATUS_FAILURE
		NDIS_STATUS_PENDING
		NDIS_STATUS_SUCCESS

	Note:
	
	========================================================================
*/
NDIS_STATUS	MlmeHardTransmit(
	IN	PRTMP_ADAPTER	pAdapter,
	IN  struct sk_buff	*pSkb)
{
	PUCHAR			pSrcBufVA;
	ULONG			SrcBufLen;
	PTXD_STRUC		pTxD;
#ifdef BIG_ENDIAN
    PTXD_STRUC      pDestTxD;
    TXD_STRUC       TxD;
#endif
	PHEADER_802_11	pHeader_802_11;
	BOOLEAN         bAckRequired, bInsertTimestamp;
	UCHAR			MlmeRate;
	UMAXLONG		IrqFlags;
//iverson
        UCHAR			RetryMode = SHORT_RETRY; 
//iverson end	
	DBGPRINT(RT_DEBUG_INFO, "MlmeHardTransmit\n");

	// drop all packet while radar or carrier signal existed.
	if ((pAdapter->PortCfg.RadarDetect.RDMode != RD_NORMAL_MODE)
#ifdef CARRIER_DETECTION_SUPPORT
		||(isCarrierDetectExist(pAdapter) == TRUE)
#endif // CARRIER_DETECTION_SUPPORT //
		)
	{
        return (NDIS_STATUS_FAILURE);
	}

    // only one buffer is used in MGMT packet
    pSrcBufVA = (PVOID)pSkb->data;
    SrcBufLen = pSkb->len;

	// Make sure MGMT ring resource won't be used by other threads
	RTMP_SEM_LOCK(&pAdapter->MgmtRingLock, IrqFlags);

#ifndef BIG_ENDIAN
	pTxD  = (PTXD_STRUC) pAdapter->MgmtRing.Cell[pAdapter->MgmtRing.CurTxIndex].AllocVa;
#else
    pDestTxD  = (PTXD_STRUC) pAdapter->MgmtRing.Cell[pAdapter->MgmtRing.CurTxIndex].AllocVa;
    TxD = *pDestTxD;
    pTxD = &TxD;
    RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif

	if ((pTxD->Owner == DESC_OWN_NIC) || (pTxD->bWaitingDmaDoneInt == 1))
	{
		// Descriptor owned by NIC. No descriptor avaliable
		// This should not happen since caller guaranteed.
		// Make sure to release MGMT ring resource
		DBGPRINT_ERR("MlmeHardTransmit: MGMT RING full\n");
		RTMP_SEM_UNLOCK(&pAdapter->MgmtRingLock, IrqFlags);
		return NDIS_STATUS_FAILURE;
	}
	if (pSrcBufVA == NULL)
	{
		// The buffer shouldn't be NULL
		DBGPRINT_ERR("MlmeHardTransmit: buffer NULL\n");
		RTMP_SEM_UNLOCK(&pAdapter->MgmtRingLock, IrqFlags);
		return NDIS_STATUS_FAILURE;
	}

	pHeader_802_11           = (PHEADER_802_11) pSrcBufVA;
	bInsertTimestamp = FALSE;
//iverson
	if (pHeader_802_11->FC.SubType == SUBTYPE_QOS_NULL)
		RetryMode = LONG_RETRY;
//iverson end
	if (pHeader_802_11->FC.Type == BTYPE_CNTL) // must be PS-POLL
	{
		bAckRequired = FALSE;
	}
	else // BTYPE_MGMT or BTYPE_DATA(must be NULL frame)
	{
        pAdapter->Sequence++;
		pHeader_802_11->Sequence = pAdapter->Sequence;

		if (pHeader_802_11->Addr1[0] & 0x01) // MULTICAST, BROADCAST
		{
			bAckRequired = FALSE;
			pHeader_802_11->Duration = 0;
		}
		else
		{
			bAckRequired = TRUE;
			pHeader_802_11->Duration = RTMPCalcDuration(pAdapter, pAdapter->PortCfg.MlmeRate, 14) + pAdapter->PortCfg.Dsifs;
			if (pHeader_802_11->FC.SubType == SUBTYPE_PROBE_RSP)
			{
				bInsertTimestamp = TRUE;
			}
		}
	}

#ifdef BIG_ENDIAN
    RTMPFrameEndianChange(pAdapter, (PUCHAR)pHeader_802_11, DIR_WRITE, FALSE);
#endif 

    //
    // fill scatter-and-gather buffer list into TXD
    //
    pTxD->BufCount = 1;
    pTxD->BufLen0  = SrcBufLen;
    pTxD->BufPhyAddr0 = pci_map_single(pAdapter->pPci_Dev, (PVOID)pSrcBufVA, SrcBufLen, PCI_DMA_TODEVICE);

	// Verify Mlme rate for a / g bands.
	MlmeRate = pAdapter->PortCfg.MlmeRate;
	if (pAdapter->LatchRfRegs.Channel > 14)
	{
		// A band
		if (MlmeRate < RATE_6)
			MlmeRate = RATE_6;
	}
	
#ifdef BIG_ENDIAN
    RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
    *pDestTxD = TxD;
    pTxD = pDestTxD;
#endif

	// Initialize TX Descriptor
	// For inter-frame gap, the number is for this frame and next frame
	// For MLME rate, we will fix as 2Mb to match other vendor's implement
#ifdef CONFIG_64BIT
	{
    RTMP_DMACB *dmacb_p = &pAdapter->MgmtRing.Cell[pAdapter->MgmtRing.CurTxIndex];

//iverson
	RTMPWriteTxDescriptor(pAdapter, pTxD, CIPHER_NONE, 0,0, bAckRequired, FALSE, bInsertTimestamp,
		RetryMode, IFS_BACKOFF, MlmeRate, SrcBufLen,
		QID_MGMT, PTYPE_SPECIAL|PSUBTYPE_MGMT, pSkb, NULL, FALSE, FALSE, FALSE, FALSE, dmacb_p);
	}
#else
//iverson
	RTMPWriteTxDescriptor(pAdapter, pTxD, CIPHER_NONE, 0,0, bAckRequired, FALSE, bInsertTimestamp,
		RetryMode, IFS_BACKOFF, MlmeRate, SrcBufLen, 
		QID_MGMT, PTYPE_SPECIAL|PSUBTYPE_MGMT, pSkb, NULL, FALSE, FALSE, FALSE, FALSE);
#endif // CONFIG_64BIT //

	DBGPRINT(RT_DEBUG_INFO,"MLMEHardTransmit use rate %d\n", MlmeRate);
	
    INC_RING_INDEX(pAdapter->MgmtRing.CurTxIndex, MGMT_RING_SIZE);
	pAdapter->RalinkCounters.KickTxCount++;
		
	// Kick MGMT ring transmit
	RTMP_IO_WRITE32(pAdapter, TX_CNTL_CSR, 0x00000010);

	// Make sure to release MGMT ring resource
	RTMP_SEM_UNLOCK(&pAdapter->MgmtRingLock, IrqFlags);

    return NDIS_STATUS_SUCCESS;
}   

// should be called only when -
// 1. MEADIA_CONNECTED
// 2. AGGREGATION_IN_USED
// 3. Fragmentation not in used
// 4. either no previous frame (pPrevAddr1=NULL) .OR. previoud frame is aggregatible
BOOLEAN TxFrameIsAggregatible(
    IN  PRTMP_ADAPTER   pAd,
    IN  PUCHAR          pPrevAddr1,
    IN  PUCHAR          p8023hdr)
{
    // can't aggregate EAPOL (802.1x) frame
    if ((p8023hdr[12] == 0x88) && (p8023hdr[13] == 0x8e))
        return FALSE;

    // can't aggregate multicast/broadcast frame
    if (p8023hdr[0] & 0x01)
        return FALSE;
    
    if ((pPrevAddr1 == NULL) || MAC_ADDR_EQUAL(pPrevAddr1, p8023hdr)) // unicast to same STA
        return TRUE;
    else
        return FALSE;
}

// NOTE: we do have an assumption here, that Byte0 and Byte1 always reasid at the same 
//       scatter gather buffer
NDIS_STATUS Sniff2BytesFromNdisBuffer(
    IN  struct sk_buff	*pFirstSkb,
    IN  UCHAR           DesiredOffset,
    OUT PUCHAR          pByte0,
    OUT PUCHAR          pByte1)
{
    PUCHAR pBufferVA;
    ULONG  BufferLen, AccumulateBufferLen, BufferBeginOffset;
    
    pBufferVA = (PVOID)pFirstSkb->data;
    BufferLen = pFirstSkb->len;
    BufferBeginOffset   = 0;
    AccumulateBufferLen = BufferLen;

    *pByte0 = *(PUCHAR)(pBufferVA + DesiredOffset - BufferBeginOffset);
    *pByte1 = *(PUCHAR)(pBufferVA + DesiredOffset - BufferBeginOffset + 1);
    return NDIS_STATUS_SUCCESS;
}


/*
	========================================================================

	Routine	Description:
		This routine classifies outgoing frames into several AC (Access
		Category) and enqueue them into corresponding s/w waiting queues.
		
	Arguments:
		pAdapter	Pointer	to our adapter
		pPacket		Pointer to send packet
		
	Return Value:
		None

	Note:
	
	========================================================================
*/
UCHAR MapUserPriorityToAccessCategory[8] = {QID_AC_BE, QID_AC_BK, QID_AC_BK, QID_AC_BE, QID_AC_VI, QID_AC_VI, QID_AC_VO, QID_AC_VO};
NDIS_STATUS	RTMPSendPacket(
	IN	PRTMP_ADAPTER	pAd,
	IN	struct sk_buff  *pSkb)
{
    PUCHAR          pSrcBufVA;
    UINT            AllowFragSize;
    UCHAR           NumberOfFrag;
    UCHAR           RTSRequired;
    UCHAR           QueIdx, UserPriority, apidx = MAIN_MBSSID;
    USHORT          Aid;
    SST             Sst = SST_ASSOC;
    UCHAR           PsMode = PWR_ACTIVE, Rate;
    MAC_TABLE_ENTRY *pMacEntry = NULL;
    UMAXLONG		IrqFlags;
#ifdef IGMP_SNOOP_SUPPORT
	BOOLEAN			InIgmpGroup = FALSE;
	PMULTICAST_FILTER_TABLE_ENTRY pGroupEntry = NULL;
#endif // IGMP_SNOOP_SUPPORT //

    
    DBGPRINT(RT_DEBUG_INFO, "====> RTMPSendPacket\n");

    // Prepare packet information
    pSrcBufVA = (PVOID)pSkb->data;

    if (pSrcBufVA == NULL)
    {
        // Resource is low, system did not allocate virtual address
        // release packet resource and return NDIS_STATUS_FAILURE to upper layer
#ifdef RTL865X_FAST_PATH
	 	rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
        RELEASE_NDIS_PACKET(pAd, pSkb);
#endif
        return NDIS_STATUS_FAILURE;
    }

#ifdef APCLI_SUPPORT
    if (RTMP_GET_PACKET_NET_DEVICE(pSkb) >= MIN_NET_DEVICE_FOR_APCLI)
	{ //To AP Client interface
		Rate = pAd->ApCliTab.ApCliEntry[RTMP_GET_PACKET_NET_DEVICE(pSkb)- MIN_NET_DEVICE_FOR_APCLI].CurrTxRate;
	}
	else
#endif	
    // Get the tx rate and BSSID index
    if (RTMP_GET_PACKET_NET_DEVICE(pSkb) >= MIN_NET_DEVICE_FOR_WDS)
    {// WDS packet
    	Rate = pAd->WdsTab.WdsEntry[RTMP_GET_PACKET_NET_DEVICE(pSkb)- MIN_NET_DEVICE_FOR_WDS].CurrTxRate;

    	// The security policy of WDS packets will follow with MAIN_MBSSID's policy, and we don't support encrypt algorithm(TKIPAES) under WDS
    	if ((pAd->WdsTab.WepStatus >= Ndis802_11Encryption4Enabled))
    	{
#ifdef RTL865X_FAST_PATH
	 		rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
    		RELEASE_NDIS_PACKET(pAd, pSkb);
#endif
    		return NDIS_STATUS_FAILURE;
    	}
    }
    else
    {// Normal packet or MBSSID packet
    	// Get the BSSID index
        apidx = RTMP_GET_PACKET_NET_DEVICE(pSkb);
	    
#ifdef IGMP_SNOOP_SUPPORT
		if (pAd->PortCfg.MBSSID[apidx].IgmpSnoopEnable)
		{
			if(IS_MULTICAST_MAC_ADDR(pSrcBufVA))
			{
				PUCHAR pIpHeader = pSrcBufVA + 12;
				if (isIgmpPkt(pSrcBufVA, pIpHeader))
				{
					pGroupEntry = NULL;
				}
				else if ((pGroupEntry = MulticastFilterTableLookup(pAd->pMulticastFilterTable, pSrcBufVA,
									pAd->PortCfg.MBSSID[apidx].MSSIDDev)) == NULL)
				{
					RELEASE_NDIS_PACKET(pAd, pSkb);
					return NDIS_STATUS_FAILURE;
				}
				InIgmpGroup = TRUE;
			}
			else if (IS_BROADCAST_MAC_ADDR(pSrcBufVA))
			{
				PUCHAR pDstIpAddr = pSrcBufVA + 30; // point to Destination of Ip address of IP header.
				UCHAR GroupMacAddr[6];
				PUCHAR pGroupMacAddr = (PUCHAR)&GroupMacAddr;

				ConvertMulticastIP2MAC(pDstIpAddr, (PUCHAR *)&pGroupMacAddr);
				if ((pGroupEntry = MulticastFilterTableLookup(pAd->pMulticastFilterTable, pGroupMacAddr,
									pAd->PortCfg.MBSSID[apidx].MSSIDDev)) != NULL)
				{
					InIgmpGroup = TRUE;
				}
			}
		}
#endif // IGMP_SNOOP_SUPPORT //

    	// check if target STA is in power-saving mode. care only those associated STAs
        pMacEntry = SsPsInquiry(pAd, pSrcBufVA, &Sst, &Aid, &PsMode, &Rate);
		
    	if (pMacEntry)
    	{
    		RTMP_SET_PACKET_AID(pSkb, Aid);
    	}
    	else if((*pSrcBufVA & 0x01) == 0) 
    	{	// drop this unicast frame because it lsn't one of those associated STAs  
    		RELEASE_NDIS_PACKET(pAd, pSkb);
    		return NDIS_STATUS_FAILURE;
    	}	
		else // multicast packet.
		{
			// Set a unique ID here to indicate that it's a mcast packet.
			RTMP_SET_PACKET_AID(pSkb, PKTSRC_MCAST); 
		}
    }

    // STEP 1. Decide number of fragments required to deliver this MSDU. 
    //     The estimation here is not very accurate because difficult to 
    //     take encryption overhead into consideration here. The result 
    //     "NumberOfFrag" is then just used to pre-check if enough free 
    //     TXD are available to hold this MSDU.

    if (*pSrcBufVA & 0x01)  // fragmentation not allowed on multicast & broadcast
        NumberOfFrag = 1;    
    else  if (pMacEntry && CLIENT_STATUS_TEST_FLAG(pMacEntry, fCLIENT_STATUS_AGGREGATION_CAPABLE))
        NumberOfFrag = 1;   // Aggregation overwhelms fragmentation
    else
    {
        // The calculated "NumberOfFrag" is a rough estimation because of various 
        // encryption/encapsulation overhead not taken into consideration. This number is just
        // used to make sure enough free TXD are available before fragmentation takes place.
        // In case the actual required number of fragments of an skb buffer 
        // excceeds "NumberOfFrag"caculated here and not enough free TXD available, the
        // last fragment (i.e. last MPDU) will be dropped in RTMPHardTransmit() due to out of 
        // resource, and the skb buffer will be indicated NDIS_STATUS_FAILURE. This should 
        // rarely happen and the penalty is just like a TX RETRY fail. Affordable.
        
        AllowFragSize = (pAd->PortCfg.FragmentThreshold) - LENGTH_802_11 - LENGTH_CRC;
        NumberOfFrag = ((pSkb->len - LENGTH_802_3 + LENGTH_802_1_H) / AllowFragSize) + 1;
    }
    // Save fragment number to Ndis packet reserved field
    RTMP_SET_PACKET_FRAGMENTS(pSkb, NumberOfFrag);  

    // STEP 2. Check the requirement of RTS; decide packet TX rate
    //     If multiple fragment required, RTS is required only for the first fragment
    //     if the fragment size large than RTS threshold
    if (NumberOfFrag > 1)
        RTSRequired = (pAd->PortCfg.FragmentThreshold > pAd->PortCfg.RtsThreshold) ? 1 : 0;
    else
    {
        //RTSRequired = (pSkb->len > pAd->PortCfg.RtsThreshold) ? 1 : 0;
		// EYL
		// Compare the min possilbe frame length to RtsThreshold.  If it is larger than
		// RtsThreshold, RTSRequired must be 1.
		if ((pSkb->len + LENGTH_802_11 + LENGTH_CRC - LENGTH_802_3) > pAd->PortCfg.RtsThreshold)
	    	RTSRequired = 1;
		else
		{
	    // Else, compare the max possilbe frame length to RtsThreshold.  This max
	    // possilbe frame length is (pSkb->len + LENGTH_802_11 + LENGTH_CRC - LENGTH_802_3) 
	    // plus all other possible overheads.  If it is less than RtsThreshold, then 
	    // RtsThreshold must be 0.
	    	if ((pSkb->len + LENGTH_802_11 + LENGTH_CRC - LENGTH_802_3
				+ 6 // for WDS
				+ 2 // for WMM QoS Control field
				+ 20 // max possilbe encryption overhead (TKIP)
				+ 8) // LLC/SNAP encap
				<= pAd->PortCfg.RtsThreshold)
				RTSRequired = 0;
	    	else
	    	{ // RTSRequired could be 1 or 0.  Let's assume it is 1 first, because it is only
	      	// used to check if TXD is enough or not in RTMPDequeuePacket().
				RTSRequired = 1;
	    	}
		}
    }

    // RTS/CTS may also be required in order to protect OFDM frame
    if ((Rate >= RATE_FIRST_OFDM_RATE) && 
        ERP_IS_USE_PROTECTION(pAd->Mlme.ErpIeContent))
        RTSRequired = 2;

	//statistics - Tx traffic count
#ifdef APCLI_SUPPORT
	if(RTMP_GET_PACKET_NET_DEVICE(pSkb) >= MIN_NET_DEVICE_FOR_APCLI)
	{
		pAd->ApCliTab.ApCliEntry[RTMP_GET_PACKET_NET_DEVICE(pSkb) - MIN_NET_DEVICE_FOR_APCLI].TxOkCount++;
		pAd->ApCliTab.ApCliEntry[RTMP_GET_PACKET_NET_DEVICE(pSkb) - MIN_NET_DEVICE_FOR_APCLI].TransmittedByteCount+=pSkb->len;
	}
	else
#endif

#ifdef WDS_SUPPORT
    if (RTMP_GET_PACKET_NET_DEVICE(pSkb) >= MIN_NET_DEVICE_FOR_WDS)
    {// WDS packet
       pAd->WdsTab.WdsEntry[RTMP_GET_PACKET_NET_DEVICE(pSkb) - MIN_NET_DEVICE_FOR_WDS].TxOkCount++;
       pAd->WdsTab.WdsEntry[RTMP_GET_PACKET_NET_DEVICE(pSkb) - MIN_NET_DEVICE_FOR_WDS].TransmittedByteCount += pSkb->len;
    }
     else
#endif
	{
		pAd->PortCfg.MBSSID[apidx].TxOkCount++;
		pAd->PortCfg.MBSSID[apidx].TransmittedByteCount += pSkb->len;
	}
	
    // HotSpot counter
    if (pMacEntry && pAd->PortCfg.bEnableHSCounter)
    {
        pMacEntry->HSCounter.TotalTxByteCount += pSkb->len;
        pMacEntry->HSCounter.LastDataPacketTime = jiffies;
    }

    // Save RTS requirement to Ndis packet reserved field
    RTMP_SET_PACKET_RTS(pSkb, RTSRequired);
    RTMP_SET_PACKET_TXRATE(pSkb, Rate);

    //
    // STEP 3. Traffic classification. outcome = <UserPriority, QueIdx>
    //
    UserPriority = 0;
    QueIdx       = QID_AC_BE;
    if (pAd->PortCfg.MBSSID[apidx].bWmmCapable)
    {
        USHORT Protocol;
        UCHAR  LlcSnapLen = 0, Byte0, Byte1;
        do
        {
            // get Ethernet protocol field
            Protocol = (USHORT)((pSrcBufVA[12] << 8) + pSrcBufVA[13]);
            if (Protocol <= 1500)
            {
                // get Ethernet protocol field from LLC/SNAP
                if (Sniff2BytesFromNdisBuffer(pSkb, LENGTH_802_3 + 6, &Byte0, &Byte1) != NDIS_STATUS_SUCCESS)
                    break;
        
                Protocol = (USHORT)((Byte0 << 8) + Byte1);
                LlcSnapLen = 8;
            }

            // Use AC_BE as default value
			if (Protocol == 0x8100)
			{
#ifdef RTL865X_SOC // 2006/01/20 From Rory to modify WMM reference
				if (pAd->PortCfg.bEthWithVLANTag == TRUE)
				{
					UserPriority = (pSrcBufVA[14] & 0xe0) >> 5;
					if (Sniff2BytesFromNdisBuffer(pSkb, LENGTH_802_3 + 4, &Byte0, &Byte1) != NDIS_STATUS_SUCCESS)
						break;
				}
				else
				{
					if (Sniff2BytesFromNdisBuffer(pSkb, LENGTH_802_3 + 4, &Byte0, &Byte1) == NDIS_STATUS_SUCCESS) // Get IP header
					{
						UserPriority = (Byte1 & 0xe0) >> 5;
					}
				}
#else
				UserPriority = (pSrcBufVA[14] & 0xe0) >> 5;

				if (Sniff2BytesFromNdisBuffer(pSkb, LENGTH_802_3 + 4, &Byte0, &Byte1) != NDIS_STATUS_SUCCESS)
					break;
#endif	
			}
			else if (Protocol != 0x0800)
			{
				break;
			}
			else if (Sniff2BytesFromNdisBuffer(pSkb, LENGTH_802_3 + LlcSnapLen, &Byte0, &Byte1) == NDIS_STATUS_SUCCESS) // Get IP header
			{
				UserPriority = (Byte1 & 0xe0) >> 5;
			}
			else
			{
#ifdef RTL865X_FAST_PATH
			 	rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
				RELEASE_NDIS_PACKET(pAd, pSkb);
#endif
				return NDIS_STATUS_FAILURE;
			}

            // return AC_BE if packet is not IPv4
            if ((Byte0 & 0xf0) != 0x40)
                break;

            QueIdx = MapUserPriorityToAccessCategory[UserPriority];

            // have to check ACM bit. downgrade UP & QueIdx before passing ACM
            // NOTE: AP doesn't have to negotiate TSPEC. ACM is controlled purely via user setup, not protocol handshaking
            if (pAd->PortCfg.APEdcaParm.bACM[QueIdx] || ((pMacEntry) && (!CLIENT_STATUS_TEST_FLAG(pMacEntry, fCLIENT_STATUS_WMM_CAPABLE))))
            {
                UserPriority = 0;
                QueIdx       = QID_AC_BE;
            }
        } while (FALSE);
    }
    
    RTMP_SET_PACKET_UP(pSkb, UserPriority);

    //
    // 4. put to corrsponding TxSwQueue or Power-saving queue
    //

#ifdef APCLI_SUPPORT
    if (RTMP_GET_PACKET_NET_DEVICE(pSkb) >= MIN_NET_DEVICE_FOR_APCLI)
	{
		//To AP Client interface
	  	// Ap Client link should never go into power-save mode; just send out the frame
		if (NDIS_STATUS_FAILURE == InsertQueuePkt(pAd, QueIdx, pSkb))
            return NDIS_STATUS_FAILURE;
	}
	else 
#endif	
    // WDS link should never go into power-save mode; just send out the frame
    if (RTMP_GET_PACKET_NET_DEVICE(pSkb) >= MIN_NET_DEVICE_FOR_WDS)
    {
		if (NDIS_STATUS_FAILURE == InsertQueuePkt(pAd, QueIdx, pSkb))
            return NDIS_STATUS_FAILURE;
	}
    // M/BCAST frames are put to PSQ as long as there's any associated STA in power-save mode
	// except the cast that a MCAST packet in IGMP-Group. It'll be cloned to UCAST late.
	else if ((*pSrcBufVA & 0x01) && pAd->MacTab.fAnyStationInPsm
#ifdef IGMP_SNOOP_SUPPORT
			&& !InIgmpGroup
#endif
			)
    {
        // we don't want too many MCAST/BCAST backlog frames to eat up all buffers. So in case number of backlog 
        // MCAST/BCAST frames exceeds a pre-defined watermark within a DTIM period, simply drop coming new 
        // MCAST/BCAST frames. This design is similiar to "BROADCAST throttling in most manageable Ethernet Switch chip.
        if (pAd->MacTab.McastPsQueue.Number >= MAX_PACKETS_IN_MCAST_PS_QUEUE)
        {
#ifdef RTL865X_FAST_PATH
		 	rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
            RELEASE_NDIS_PACKET(pAd, pSkb);
#endif
            DBGPRINT(RT_DEBUG_TRACE, "too many frames (=%d) in M/BCAST PSQ, drop this one\n", pAd->MacTab.McastPsQueue.Number);
            return NDIS_STATUS_FAILURE;
        }
        else
        {
#ifndef THREAD_ISR
            RTMP_SEM_LOCK(&pAd->MacTabLock, IrqFlags);
#else
			RTMP_IRQ_LOCK(IrqFlags);
#endif
            InsertTailQueue(&pAd->MacTab.McastPsQueue, pSkb);
#ifndef THREAD_ISR
            RTMP_SEM_UNLOCK(&pAd->MacTabLock, IrqFlags);
#else
			RTMP_IRQ_UNLOCK(IrqFlags);
#endif
                
            pAd->PortCfg.MBSSID[apidx].TimBitmap |= BIT8[0];  // mark MCAST/BCAST TIM bit
            DBGPRINT(RT_DEBUG_INFO, "at least 1 STA in psm, move M/BCAST to PSQ, TIM bitmap=%08x\n", pAd->PortCfg.MBSSID[apidx].TimBitmap);
        }
    }
    // else if the associted STA in power-save mode, frame also goes to PSQ
    else if (pMacEntry && (Sst == SST_ASSOC) && (PsMode == PWR_SAVE))
    {
        if (pMacEntry->PsQueue.Number >= MAX_PACKETS_IN_PS_QUEUE)
        {
#ifdef RTL865X_FAST_PATH
	 		rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
            RELEASE_NDIS_PACKET(pAd, pSkb);
#endif
            DBGPRINT(RT_DEBUG_TRACE, "too many frames (=%d) in AID(%d) PSQ, drop this one\n", pMacEntry->PsQueue.Number, Aid);
            return NDIS_STATUS_FAILURE;
        }
        else 
        {
#ifndef THREAD_ISR
	        RTMP_SEM_LOCK(&pAd->MacTabLock, IrqFlags);
#else
			RTMP_IRQ_LOCK(IrqFlags);
#endif
	        InsertTailQueue(&pMacEntry->PsQueue, pSkb);

#ifndef THREAD_ISR
	        RTMP_SEM_UNLOCK(&pAd->MacTabLock, IrqFlags);
#else
			RTMP_IRQ_UNLOCK(IrqFlags);
#endif
		}

        // mark corresponding TIM bit in outgoing BEACON frame
        // if STA is APSD-enable, partial AC is delivery-enabled and this packet belongs to delivery-enabled,
        // then do not mark the TimBitmap, otherwise need
        if (CLIENT_STATUS_TEST_FLAG(pMacEntry, fCLIENT_STATUS_APSD_CAPABLE) &&
        	(!pMacEntry->bAPSDCapablePerAC[QID_AC_VO] || !pMacEntry->bAPSDCapablePerAC[QID_AC_VI] || !pMacEntry->bAPSDCapablePerAC[QID_AC_BE] || !pMacEntry->bAPSDCapablePerAC[QID_AC_BK]) &&
        	pMacEntry->bAPSDCapablePerAC[QueIdx])
        {
        }
        else
        {
	        if (Aid >= 32)
	            pAd->PortCfg.MBSSID[apidx].TimBitmap2 |= BIT32[Aid-32];
	        else
	            pAd->PortCfg.MBSSID[apidx].TimBitmap |= BIT32[Aid];
	        DBGPRINT(RT_DEBUG_TRACE, "target (AID=%d) in psm, move to PSQ, TIM bitmap=%02x-%02x\n", Aid, pAd->PortCfg.MBSSID[apidx].TimBitmap, pAd->PortCfg.MBSSID[apidx].TimBitmap2);
        }
    }
    // 3. otherwise, transmit the frame
    else // (PsMode == PWR_ACTIVE) || (PsMode == PWR_UNKNOWN)
    {
#ifdef IGMP_SNOOP_SUPPORT
		// if it's a mcast packet in igmp gourp.
		// ucast clone it for all members in the gourp.
		if(InIgmpGroup && pGroupEntry && (IgmpMemberCnt(&pGroupEntry->MemberList) > 0))
        {
			struct sk_buff  *pSkbClone = NULL;
			PSOURCE_ENTRY pSourceEntry;
			PMEMBER_ENTRY pMemberEntry = (PMEMBER_ENTRY)pGroupEntry->MemberList.pHead;

			// check all members of the IGMP group.
			while(pMemberEntry != NULL)
			{
				if(pMemberEntry->type!=IGMPV1V2)
				{
                			BOOLEAN found=FALSE;
                			pSourceEntry=(PSOURCE_ENTRY)pMemberEntry->SourceList.pHead;
					while(pSourceEntry!=NULL)
					{		
						UINT16 IpProtocol=ntohs(*((UINT16 *)(pSrcBufVA+12)));

						 if(IpProtocol == ETH_P_IP)		
						 {	if(RTMPEqualMemory((PVOID)(pSourceEntry->Addr), (PVOID)(pSrcBufVA+LENGTH_802_3+12), IPV4_ADDR_LEN))
							  	found=TRUE;
						 }
						 pSourceEntry=pSourceEntry->pNext;
					}

					if( (pMemberEntry->type==MODE_IS_INCLUDE && found==FALSE) || (pMemberEntry->type==MODE_IS_EXCLUDE && found==TRUE))
                			{
						 pMemberEntry = pMemberEntry->pNext;
							 continue;                	 
                			}				
               		}
			
				pSkbClone = skb_clone(pSkb, GFP_ATOMIC);
				if(pSkbClone)
				{
					pMacEntry = SsPsInquiry(pAd, pMemberEntry->Addr, &Sst, &Aid, &PsMode, &Rate);
					if (pMacEntry && (Sst == SST_ASSOC) && (PsMode != PWR_SAVE))
					{
						RTMP_SET_PACKET_AID(pSkbClone, Aid);
						RTMP_SET_PACKET_TXRATE(pSkbClone, Rate);
						RTMP_SET_PACKET_SOURCE(pSkbClone, PKTSRC_DRIVER);
					}
					else {
						RELEASE_NDIS_PACKET(pAd, pSkbClone);
						pMemberEntry = pMemberEntry->pNext;
						continue;
					}

					// HotSpot counter
					if (pMacEntry && pAd->PortCfg.bEnableHSCounter)
        			{
						pMacEntry->HSCounter.TotalTxByteCount += pSkbClone->len;
						pMacEntry->HSCounter.LastDataPacketTime = jiffies;
					}

					// insert the pkt to TxSwQueue.
					if (NDIS_STATUS_FAILURE == InsertQueuePkt(pAd, QueIdx, pSkbClone))
					{
						RELEASE_NDIS_PACKET(pAd, pSkb);
            			return NDIS_STATUS_FAILURE;
					}

				}
				pMemberEntry = pMemberEntry->pNext;
			}
            RELEASE_NDIS_PACKET(pAd, pSkb);
        }
        else 
#endif // IGMP_SNOOP_SUPPORT //
        {
			// insert the pkt to TxSwQueue.
			if (NDIS_STATUS_FAILURE == InsertQueuePkt(pAd, QueIdx, pSkb))
				return NDIS_STATUS_FAILURE;
		}
        DBGPRINT(RT_DEBUG_INFO, "TxSwQueue[%d] size=%d\n", QueIdx, pAd->TxSwQueue[QueIdx].Number);
    }

    pAd->RalinkCounters.OneSecOsTxCount[QueIdx]++; // TODO: for debug only. to be removed
    return NDIS_STATUS_SUCCESS;
}

/*
	========================================================================

	Routine	Description:
		This subroutine will scan through releative ring descriptor to find
		out avaliable free ring descriptor and compare with request size.
		
	Arguments:
		pAdapter	Pointer	to our adapter
		QueIdx	    Selected TX Ring
		
	Return Value:
		NDIS_STATUS_FAILURE		Not enough free descriptor
		NDIS_STATUS_SUCCESS		Enough free descriptor

	Note:
	
	========================================================================
*/
NDIS_STATUS	RTMPFreeTXDRequest(
	IN		PRTMP_ADAPTER	pAdapter,
	IN		UCHAR			QueIdx,
	IN		UCHAR			NumberRequired,
	IN 		PUCHAR			FreeNumberIs)
{
	INT			    FreeNumber = 0;
	PTXD_STRUC		pTxD;
#ifdef BIG_ENDIAN
    PTXD_STRUC      pDestTxD;
    TXD_STRUC       TxD;
#endif
	PRTMP_TX_RING   pTxRing;
	PRTMP_MGMT_RING pMgmtRing;
	NDIS_STATUS		Status = NDIS_STATUS_FAILURE;

	switch (QueIdx)
	{
		case QID_AC_BK:
		case QID_AC_BE:
		case QID_AC_VI:
		case QID_AC_VO:
		case QID_HCCA:
// TODO: do we need to wrap the code by SpinLock?
	        pTxRing = &pAdapter->TxRing[QueIdx];
#ifndef BIG_ENDIAN
			pTxD  = (PTXD_STRUC) pTxRing->Cell[pTxRing->CurTxIndex].AllocVa;
#else
		    pDestTxD  = (PTXD_STRUC) pTxRing->Cell[pTxRing->CurTxIndex].AllocVa;
		    TxD = *pDestTxD;
		    pTxD = &TxD;
		    RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif

            // ring full waiting for DmaDoneInt() to release skb buffer
            if (pTxD->bWaitingDmaDoneInt == 1)
                FreeNumber = 0;
            else 
            {
                FreeNumber = TX_RING_SIZE + pTxRing->NextTxDmaDoneIndex - pTxRing->CurTxIndex;
                if (FreeNumber > TX_RING_SIZE)     
                    FreeNumber -= TX_RING_SIZE;
            }

			if (FreeNumber >= NumberRequired)
				Status = NDIS_STATUS_SUCCESS;
			break;
			
		case QID_MGMT:
		    pMgmtRing = &pAdapter->MgmtRing;
#ifndef BIG_ENDIAN
			pTxD  = (PTXD_STRUC) pMgmtRing->Cell[pMgmtRing->CurTxIndex].AllocVa;
#else
		    pDestTxD  = (PTXD_STRUC) pMgmtRing->Cell[pMgmtRing->CurTxIndex].AllocVa;
		    TxD = *pDestTxD;
		    pTxD = &TxD;
		    RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif
			

            if (pTxD->bWaitingDmaDoneInt == 1)
                FreeNumber = 0;
            else
            {
                FreeNumber = MGMT_RING_SIZE + pMgmtRing->NextTxDmaDoneIndex - pMgmtRing->CurTxIndex;
                if (FreeNumber > MGMT_RING_SIZE) 
                    FreeNumber -= MGMT_RING_SIZE;
            }

			if (FreeNumber >= NumberRequired)
				Status = NDIS_STATUS_SUCCESS;
			break;

		default:
			break;
	}
	
	*FreeNumberIs = (UCHAR)FreeNumber;
	return (Status);
}

#ifdef DBG
static unsigned int dbg_TotalTxPkts = 0;
static unsigned int dbg_AggregatedPkts = 0;

void dbg_AggregatedRatio(PRTMP_ADAPTER	pAd)
{
	if (pAd->PortCfg.bAggregationCapable && dbg_TotalTxPkts) {
		DBGPRINT(RT_DEBUG_TRACE,"Aggregation Ratio = %02d.%02d%%\n",  ((dbg_AggregatedPkts * 100)/dbg_TotalTxPkts),  (((dbg_AggregatedPkts*100)%dbg_TotalTxPkts)*100)/dbg_TotalTxPkts);		
	}

	dbg_TotalTxPkts = 0;
	dbg_AggregatedPkts = 0;
}
#endif


/*
	========================================================================

	Routine	Description:
		Copy frame from waiting queue into relative ring buffer and set 
	appropriate ASIC register to kick hardware encryption before really
	sent out to air.
		
	Arguments:
		pAdapter		Pointer	to our adapter
		PNDIS_PACKET	Pointer to outgoing Ndis frame
		NumberOfFrag	Number of fragment required
		
	Return Value:
		None

	Note:
		When THREAD_ISR was defined, caller must disable IRQ before using this
		function.
	
	========================================================================
*/
static inline NDIS_STATUS	RTMPHardTransmit(
	IN	PRTMP_ADAPTER	pAd,
	IN	struct sk_buff  *pSkb,
	IN  UCHAR           QueIdx)
{
    PUCHAR          pSrcBufVA, pNextPacketBufVA;
    ULONG           SrcBufLen, NextPacketBufLen=0;
    UCHAR           SrcBufIdx;
    UMAXLONG        SrcBufPA = 0;
    UMAXLONG        HeaderPAOffset = 0;
    UMAXLONG        FragPAOffset = 0;
    USHORT          SrcRemainingBytes;
    UCHAR           FrameGap;
    HEADER_802_11   Header_802_11;
    PHEADER_802_11  pHeader80211;
    PUCHAR          pExtraLlcSnapEncap; // NULL: no extra LLC/SNAP is required
    UCHAR           CipherAlg = CIPHER_NONE;
    PTXD_STRUC      pTxD;
#ifdef BIG_ENDIAN
    PTXD_STRUC      pDestTxD;
    TXD_STRUC       TxD;
#endif
    BOOLEAN         bEAPOLFrame;
    BOOLEAN         bAckRequired;
    ULONG           Iv16, Iv32;
    UCHAR           RetryMode = SHORT_RETRY;
    USHORT          AckDuration = 0;
    UCHAR           KeyIdx = 0, KeyTable = SHARED_KEY_TABLE;
    PCIPHER_KEY     pKey;
    UCHAR           PID;
    PRTMP_TX_RING   pTxRing = &pAd->TxRing[QueIdx];
    PRTMP_SCATTER_GATHER_LIST   pSGList = NULL;
    RTMP_SCATTER_GATHER_LIST    LocalSGList;
    struct sk_buff  *pNextSkb;
    BOOLEAN         bClonePacket;
    UCHAR           MpduRequired, RtsRequired, UserPriority;
    UCHAR           TxRate;
    PMAC_TABLE_ENTRY pMacEntry = NULL;
    BOOLEAN         bWdsPacket = FALSE;
    NDIS_802_11_WEP_STATUS	EncryptType = Ndis802_11EncryptionDisabled;
    UCHAR           apidx;
    UCHAR           WdsIdx = 0;
#ifndef THREAD_ISR
    UMAXLONG		IrqFlags;
#endif
    USHORT          EncryptionOverhead = 0;
    UCHAR	    SrcAddr[6];
    BOOLEAN			bPiggyBack = FALSE;
    BOOLEAN			bRTS_CTSFrame = FALSE;
    BOOLEAN			bMoreData;
    BOOLEAN			bUseIndividualWepKeyIdx = FALSE;
#ifdef APCLI_SUPPORT
	struct sk_buff *apCliSkb= NULL;
	BOOLEAN         bApCliPacket = FALSE;
	UCHAR 			ApCliIdx = 0;
#endif
	BOOLEAN			bMcastPkt = FALSE; // used to indicate the packet is a mcast packet.
	UINT			SkbLen1;


    RetryMode = pAd->PortCfg.bRetryLimit; 

	// drop all packets while radar or carrier signal existed.
    if ( (pAd->PortCfg.RadarDetect.RDMode != RD_NORMAL_MODE)
#ifdef CARRIER_DETECTION_SUPPORT
		||(isCarrierDetectExist(pAd) == TRUE)
#endif // CARRIER_DETECTION_SUPPORT //
		)
	{
#ifdef RTL865X_FAST_PATH
	 	rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
		RELEASE_NDIS_PACKET(pAd, pSkb);
#endif
        return (NDIS_STATUS_FAILURE);
	}

    //
    // Prepare packet information structure which will be query for buffer descriptor
    //
    pSrcBufVA = (PVOID)pSkb->data;
    SrcBufLen = pSkb->len;

//iversonadd
int k;
    k = RTMP_GET_PACKET_ENCRYPT(pSkb);

    MpduRequired = RTMP_GET_PACKET_FRAGMENTS(pSkb);
    RtsRequired  = RTMP_GET_PACKET_RTS(pSkb);
    UserPriority = RTMP_GET_PACKET_UP(pSkb);
    TxRate       = RTMP_GET_PACKET_TXRATE(pSkb);
	bMoreData    = RTMP_GET_PACKET_MOREDATA(pSkb);
//iverson
	if (MapUserPriorityToAccessCategory[UserPriority] == QID_AC_VO)
		RetryMode = LONG_RETRY;
//iverson end


	// Find out the packet comes from which interface(MBSSID/WDS/MAIN_SSID/APCLI)
#ifdef APCLI_SUPPORT
    if (RTMP_GET_PACKET_NET_DEVICE(pSkb) >= MIN_NET_DEVICE_FOR_APCLI)
	{ //To AP Client interface
		bApCliPacket = TRUE;
		ApCliIdx = RTMP_GET_PACKET_NET_DEVICE(pSkb) - MIN_NET_DEVICE_FOR_APCLI;
		apidx = MAIN_MBSSID;

		// For each tx packet, update our apcli convert engine databases.
		apCliSkb = (struct sk_buff *)UWRMacUpdaterForTxPkt(pAd, pSkb, NULL);
		if(apCliSkb) {
			RELEASE_NDIS_PACKET(pAd, pSkb);
			pSkb = apCliSkb;
		}
		pSrcBufVA = (PVOID)pSkb->data;
		SrcBufLen = pSkb->len;
	}
	else
#endif	
    if (RTMP_GET_PACKET_NET_DEVICE(pSkb) >= MIN_NET_DEVICE_FOR_WDS)
    {// WDS packet
        bWdsPacket = TRUE;
        WdsIdx = RTMP_GET_PACKET_NET_DEVICE(pSkb) - MIN_NET_DEVICE_FOR_WDS;
        apidx = MAIN_MBSSID;
    }
    else
    {// Multiple BSSID packet
        bWdsPacket = FALSE;
        apidx = RTMP_GET_PACKET_NET_DEVICE(pSkb);

		// check the packet is a really MCAST packet by a unigue AID.
		// if the packet's DA be MCAST address but it doesn't be marked by MCAST AID.
		// then it must a IGMP-Group packet. we do UCAST clone for all group members late. 
		bMcastPkt = ((UCHAR)RTMP_GET_PACKET_AID(pSkb) == PKTSRC_MCAST) ? TRUE : FALSE;
        if ((*pSrcBufVA & 0x01) == 0 || !bMcastPkt)
        {
	        pMacEntry = &pAd->MacTab.Content[(UCHAR)RTMP_GET_PACKET_AID(pSkb)];
        }
    }

    DBGPRINT(RT_DEBUG_INFO,"HardTransmit(RTS=%d,Frag=%d,UP=%d,TxRate=%d,apidx=%d,bWds=%d)\n",RtsRequired,MpduRequired,UserPriority,TxRate,apidx,bWdsPacket);

    if (SrcBufLen < 14)
    {
        DBGPRINT(RT_DEBUG_ERROR,"RTMPHardTransmit --> Skb buffer error !!!\n");
#ifdef RTL865X_FAST_PATH
	 	rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
        RELEASE_NDIS_PACKET(pAd, pSkb);
#endif
        return (NDIS_STATUS_FAILURE);
    }

    //
	// If DHCP datagram or ARP datagram , we need to send it as Low rates.
	//
	if (pAd->PortCfg.Channel <= 14)
	{
		//
		// Case 802.11 b/g
		// basic channel means that we can use CCKM's low rate as RATE_1.
		//		
		if ((TxRate != RATE_1) && RTMPCheckDHCPFrame(pAd, pSkb))
			TxRate = RATE_1;
	}
	else
	{
		//
		// Case 802.11a
		// We must used OFDM's low rate as RATE_6, note RATE_1 is not allow
		// Only OFDM support on Channel > 14
		//
		if ((TxRate != RATE_6) && RTMPCheckDHCPFrame(pAd, pSkb))
			TxRate = RATE_6;
	}

    bClonePacket = FALSE;  // cloning is not necessary
    pNextSkb  = NULL;   // no aggregation is required

    if ((((*((PUCHAR) pSrcBufVA) & 0x01) == 0) || !bMcastPkt) && pMacEntry)
    {// Unicast packet
    	EncryptType = pMacEntry->WepStatus;
    }
#ifdef APCLI_SUPPORT
    else if (bApCliPacket == TRUE)
    {// to Ap Client Interface packet
	 // AP Client interface always do unicast encrypt when transmit
    	EncryptType = pAd->ApCliTab.ApCliEntry[ApCliIdx].PairCipher;
    }
#endif	
    else if (bWdsPacket == TRUE)
    {// WDS packet
    	EncryptType = pAd->WdsTab.WepStatus;
    }
    else
    {// multicast packet
    	EncryptType = pAd->PortCfg.MBSSID[apidx].GroupKeyWepStatus;
    }

#ifdef DBG
	dbg_TotalTxPkts++;
#endif

	SkbLen1 = pSkb->len;	// 1st pSkb->len before aggregation

#ifdef AGGREGATION_SUPPORT
    // check if aggregation applicable on this MSDU and the next one.
    // NOTE: aggregation not applicable when CKIP inused. because it's difficult for driver
    //       to calculate CMIC on the aggregated MSDU
    // If packet's MpduRequired > 1, then turn off aggregation due to receiver can not receive this kind of packets
	if (pAd->PortCfg.bAggregationCapable &&
		pMacEntry                                                         &&
        CLIENT_STATUS_TEST_FLAG(pMacEntry, fCLIENT_STATUS_AGGREGATION_CAPABLE) &&
        (pAd->TxSwQueue[QueIdx].Head != NULL)                             &&
        (TxRate >= RATE_6)                                                &&
        TxFrameIsAggregatible(pAd, NULL, pSrcBufVA)                       &&
        ((*pSrcBufVA & 0x01) == 0)) // MCAST packet should not be aggregated. Add a more condition for IGMP-Group packet.
    {
        DBGPRINT(RT_DEBUG_INFO,"AGGRE: 1st MSDU aggregatible(len=%d)\n", pSkb->len);

        pNextSkb = (struct sk_buff*)RemoveHeadQueue(&pAd->TxSwQueue[QueIdx]);

        pNextPacketBufVA = (PVOID)pNextSkb->data;
        NextPacketBufLen = pNextSkb->len;

        if (TxFrameIsAggregatible(pAd, pSrcBufVA, pNextPacketBufVA))
        {
            // need to clone pNextPacket to assure it uses only 1 scatter buffer
           	DBGPRINT(RT_DEBUG_INFO,"AGGRE: 2nd MSDU aggregatible(len=%d)\n", NextPacketBufLen);
#ifdef DBG
			dbg_TotalTxPkts++;
			dbg_AggregatedPkts += 2;
#endif
		    // If having VLAN tag, skip it when transmit
			if (((pNextPacketBufVA[12] << 8) + pNextPacketBufVA[13]) == 0x8100)
			{
				*((u16 *)&pNextSkb->data[14]) = *((u16 *)&pNextSkb->data[10]);
				*((u32 *)&pNextSkb->data[10]) = *((u32 *)&pNextSkb->data[6]);
				*((u32 *)&pNextSkb->data[6]) = *((u32 *)&pNextSkb->data[2]);
				*((u16 *)&pNextSkb->data[4]) = *((u16 *)&pNextSkb->data[0]);
			
				/* dest addr (6) + src addr (6) + vlan (4) */
				pNextSkb->data += 12+4;
				pNextSkb->len -= 12+4;
				//skb_pull(pNextSkb, 12+4); 				//printk("@: %d\n", pNextSkb->len);
				skb_push(pNextSkb, 12);
				pNextPacketBufVA = (PVOID)pNextSkb->data;
				NextPacketBufLen = pNextSkb->len;				
			}
        }
        else
        {
            // can't aggregate. put next packe back to TxSwQueue
            InsertHeadQueue(&pAd->TxSwQueue[QueIdx], pNextSkb);
            pNextSkb = NULL;  
        }
    }
#else /* !AGGREGATION_SUPPORT */
	pNextSkb = NULL;
#endif 

    // Decide if Packet Cloning is required -
    // 1. when fragmentation && TKIP is inused, we have to clone it into a single buffer
    //    NDIS_PACKET so that driver can pre-cluculate and append TKIP MIC at tail of the
    //    source packet before hardware fragmentation can be performed
    // 2. if too many physical buffers in scatter-gather list (>4), we have to clone it into
    //    a single buffer NDIS_PACKET before furthur processing
    if ((RTMP_GET_PACKET_SOURCE(pSkb) == PKTSRC_NDIS) && (bClonePacket == FALSE))
    {
        if ((MpduRequired > 1) && (EncryptType == Ndis802_11Encryption2Enabled))
        {
            bClonePacket = TRUE;
        }
        else
        {
#ifndef RTL865X_FAST_PATH
        	if (skb_shinfo(pSkb)->nr_frags != 0)
        	{
        		DBGPRINT(RT_DEBUG_WARN,"\nnr_frags=%d\n\n", skb_shinfo(pSkb)->nr_frags);
        	}
        	if (skb_shinfo(pSkb)->nr_frags == 0)
#endif
        	
        	{
	            LocalSGList.NumberOfElements = 1;
		        LocalSGList.Elements[0].Length = SrcBufLen;
		        LocalSGList.Elements[0].Address = pSkb->data;
		        LocalSGList.Elements[1].Length = 0;
		        LocalSGList.Elements[1].Address = NULL;
        	}
#ifndef RTL865X_FAST_PATH
        	else
        	{
        		DBGPRINT(RT_DEBUG_WARN,"Scatter-gather packet (nr_frags=%d)\n\n\n", skb_shinfo(pSkb)->nr_frags);
        		bClonePacket = TRUE;
        	}
#endif
        }
    }

    // Clone then release the original skb buffer
    if (bClonePacket)
    {
        struct sk_buff  *pOutSkb;
        NDIS_STATUS  Status;

        Status = RTMPCloneNdisPacket(pAd, pSkb, &pOutSkb);
#ifdef RTL865X_FAST_PATH
	 	rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
        RELEASE_NDIS_PACKET(pAd, pSkb);
#endif
        if (Status != NDIS_STATUS_SUCCESS)
        {
        	DBGPRINT(RT_DEBUG_ERROR,"RTMPHardTransmit --> RTMPCloneNdisPacket error !!!\n");
            return Status;
        }

        // Use the new cloned packet from now on
        pSkb = pOutSkb;
        RTMP_SET_PACKET_UP(pSkb, UserPriority);
        
        pSrcBufVA = (PVOID)pSkb->data;
        SrcBufLen = pSkb->len;
    }

    // use local scatter-gather structure for internally created skb buffer
    // Can't use NDIS_PER_PACKET_INFO() to get scatter gather list
    //if (RTMP_GET_PACKET_SOURCE(pSkb) != PKTSRC_NDIS)
    {
        LocalSGList.NumberOfElements = 1;
        LocalSGList.Elements[0].Length = SrcBufLen;
        LocalSGList.Elements[0].Address = pSkb->data;
        LocalSGList.Elements[1].Length = 0;
        LocalSGList.Elements[1].Address = NULL;

        // link next MSDU into scatter list, if any
        if (pNextSkb)
        {
            LocalSGList.NumberOfElements = 2;
            LocalSGList.Elements[0].Length = SrcBufLen;
	        LocalSGList.Elements[0].Address = pSkb->data;
            LocalSGList.Elements[1].Length = NextPacketBufLen;
            LocalSGList.Elements[1].Address = pNextSkb->data;
            pSkb->len += NextPacketBufLen;
        }
    }
    pSGList = &LocalSGList;

    // ----------------------------------------
    // STEP 0.1 Add 802.1x protocol check.
    // ----------------------------------------
    
    // For non-WPA network, 802.1x message should not encrypt even privacy is on.
    if (NdisEqualMemory(EAPOL, pSrcBufVA + 12, 2))
        bEAPOLFrame = TRUE;
    else
        bEAPOLFrame = FALSE;

    // WPA 802.1x secured port control
#ifdef APCLI_SUPPORT 
	// drop all non-802.1x frame before port secured
    if(bApCliPacket)
	{
	    if (((pAd->ApCliTab.ApCliEntry[ApCliIdx].AuthMode == Ndis802_11AuthModeWPAPSK) ||
         	 (pAd->ApCliTab.ApCliEntry[ApCliIdx].AuthMode == Ndis802_11AuthModeWPA2PSK)) &&
        	 ((pAd->ApCliTab.ApCliEntry[ApCliIdx].PortSecured == WPA_802_1X_PORT_NOT_SECURED) || 
        	 (pAd->ApCliTab.ApCliEntry[ApCliIdx].MicErrCnt >= 2)) && (bEAPOLFrame == FALSE))
		{
            DBGPRINT(RT_DEBUG_INFO,"I/F(apcli%d) RTMPHardTransmit --> Drop packet before AP-Client secured !!!\n", ApCliIdx);
#ifdef RTL865X_FAST_PATH
	 		rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
            RELEASE_NDIS_PACKET(pAd, pSkb);
#endif
            return (NDIS_STATUS_FAILURE);
        }
	}
	else
#endif				
    if(!bWdsPacket)
    {
        // in WPA mode, AP does not send packets before port secured.
        if (((pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPA) || (pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPA2)
        	|| (pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPAPSK) || (pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPA2PSK)
        	|| (pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPAPSKWPA2PSK) || (pAd->PortCfg.MBSSID[apidx].AuthMode == Ndis802_11AuthModeWPA1WPA2))
        	&& (pAd->PortCfg.MBSSID[apidx].PortSecured == WPA_802_1X_PORT_NOT_SECURED) && (bEAPOLFrame == FALSE))
        {
            DBGPRINT(RT_DEBUG_INFO,"I/F(ra%d) RTMPHardTransmit --> Drop packet before AP secured !!!\n", apidx);
#ifdef RTL865X_FAST_PATH
	 		rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
            RELEASE_NDIS_PACKET(pAd, pSkb);
#endif
            return (NDIS_STATUS_FAILURE);
        }
        // if 802.1x, AP does not send packets other than EAPOL before this session control port set to "accept all".
		if ((pAd->PortCfg.MBSSID[apidx].IEEE8021X == TRUE) && (pMacEntry) &&
			(pMacEntry->PrivacyFilter == Ndis802_11PrivFilter8021xWEP) && (bEAPOLFrame == FALSE))
		{
			DBGPRINT(RT_DEBUG_TRACE,"RTMPHardTransmit --> Drop packet before this control port accept all !!!\n");
#ifdef RTL865X_FAST_PATH
		 	rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
			RELEASE_NDIS_PACKET(pAd, pSkb);
#endif
			return (NDIS_STATUS_FAILURE);
		}
    }

#ifdef GEM_LEDMODE
    pAd->Mlme.bLEDblinking = TRUE;
#endif

    // -------------------------------------------
    // STEP 0.2. some early parsing
    // -------------------------------------------
    
    // 1. traditional TX burst
    if (pAd->PortCfg.bEnableTxBurst && pMacEntry && (pMacEntry->MaxSupportedRate >= RATE_FIRST_OFDM_RATE) && (pAd->Sequence & 0x7))
        FrameGap = IFS_SIFS;
    // 2. frame belonging to AC that has non-zero TXOP
    else if (pAd->PortCfg.MBSSID[apidx].bWmmCapable && pAd->PortCfg.APEdcaParm.bValid && pAd->PortCfg.APEdcaParm.Txop[QueIdx])
        FrameGap = IFS_SIFS;
    // 3. otherwise, always BACKOFF before transmission
    else
        FrameGap = IFS_BACKOFF;     // Default frame gap mode

    // if orginal Ethernet frame contains no LLC/SNAP, then an extra LLC/SNAP encap is required 
    if (((*(pSrcBufVA + 12) << 8) + *(pSrcBufVA + 13)) > 1500)
    {
        pExtraLlcSnapEncap = SNAP_802_1H;
        if (NdisEqualMemory(IPX, pSrcBufVA + 12, 2) || 
            NdisEqualMemory(APPLE_TALK, pSrcBufVA + 12, 2))
        {
            pExtraLlcSnapEncap = SNAP_BRIDGE_TUNNEL;
        }
    }
    else
        pExtraLlcSnapEncap = NULL;

    // -----------------------------------------------------------------
    // STEP 2. MAKE A COMMON 802.11 HEADER SHARED BY ENTIRE FRAGMENT BURST.
    // -----------------------------------------------------------------

    pAd->Sequence ++;

    NdisZeroMemory(&Header_802_11, sizeof(HEADER_802_11));
    Header_802_11.FC.FrDs = 1;
    Header_802_11.FC.Type = BTYPE_DATA;
    Header_802_11.FC.SubType = SUBTYPE_DATA;
    Header_802_11.Sequence   = pAd->Sequence;
    
    // Inform STA not fail in sleep state when STA in power saving mode
    if (bMoreData == TRUE)
    {
    	Header_802_11.FC.MoreData = 1;
    }
#ifdef APCLI_SUPPORT
    if (bApCliPacket)
	{
		Header_802_11.FC.ToDs = 1;
		Header_802_11.FC.FrDs = 0;
        COPY_MAC_ADDR(Header_802_11.Addr1, pAd->ApCliTab.ApCliEntry[(RTMP_GET_PACKET_NET_DEVICE(pSkb) - MIN_NET_DEVICE_FOR_APCLI)].ApCliBssid);// to AP2
        COPY_MAC_ADDR(Header_802_11.Addr2, pAd->ApCliTab.ApCliEntry[(RTMP_GET_PACKET_NET_DEVICE(pSkb) - MIN_NET_DEVICE_FOR_APCLI)].CurrentAddress); // from AP1
        COPY_MAC_ADDR(Header_802_11.Addr3, pSrcBufVA); //DA
	}
    else
#endif 		   
    if (bWdsPacket)
    {
        Header_802_11.FC.ToDs = 1;
        COPY_MAC_ADDR(Header_802_11.Addr1, pAd->WdsTab.WdsEntry[(RTMP_GET_PACKET_NET_DEVICE(pSkb) - MIN_NET_DEVICE_FOR_WDS)].WdsAddr);    // to AP2
        COPY_MAC_ADDR(Header_802_11.Addr2, pAd->PortCfg.MBSSID[MAIN_MBSSID].Bssid);                // from AP1
        COPY_MAC_ADDR(Header_802_11.Addr3, pSrcBufVA);                                      // DA
         
        // NOTE: ADDR4(=SA) will be appended later directly into pTxD->TxBuf0
    }
    else
    {
        // TODO: how about "MoreData" bit? AP need to set this bit especially for PS-POLL response
        if (bMcastPkt)
        	COPY_MAC_ADDR(Header_802_11.Addr1, pSrcBufVA);                // DA
        else
            COPY_MAC_ADDR(Header_802_11.Addr1, pMacEntry->Addr);
        COPY_MAC_ADDR(Header_802_11.Addr2, pAd->PortCfg.MBSSID[apidx].Bssid);  // BSSID
        COPY_MAC_ADDR(Header_802_11.Addr3, pSrcBufVA + MAC_ADDR_LEN); // SA
    }

    //  AP's WMM-inused or not should depends on MAC table. It's per-client not per-system
    if (pAd->PortCfg.MBSSID[apidx].bWmmCapable && pMacEntry && CLIENT_STATUS_TEST_FLAG(pMacEntry, fCLIENT_STATUS_WMM_CAPABLE))
        Header_802_11.FC.SubType = SUBTYPE_QDATA;

    if (Header_802_11.Addr1[0] & 0x01) // Multicast or Broadcast
    {
        bAckRequired = FALSE;
        PID = PTYPE_SPECIAL|PSUBTYPE_DATA_NO_ACK;
        pAd->WlanCounters.MulticastTransmittedFrameCount++;
    }
    else
    {
        bAckRequired = TRUE;
        // PID is used in TxDoneIsr to calculate per-client/WDS TX statistics 
        // b6-7 marks this frame requires an ACK; b0-5: 0-59 client AID, 60-63 WDS link 0-3
        PID = PTYPE_DATA_REQUIRE_ACK; 
#ifdef APCLI_SUPPORT
		if (bApCliPacket)
			PID += ApCliIdx + APCLI_PAIRWISE_KEY_OFFSET;
		else
#endif				 
        if (bWdsPacket)
            PID += WdsIdx + WDS_PAIRWISE_KEY_OFFSET;
        else if (pMacEntry)
            PID += pMacEntry->Aid;
    }

    // --------------------------------------------------------
    // STEP 3. FIND ENCRYPT KEY AND DECIDE CIPHER ALGORITHM
    //      Find the WPA key, either Group or Pairwise Key
    //      LEAP + TKIP also use WPA key.
    // --------------------------------------------------------
    // Decide WEP bit and cipher suite to be used. Same cipher suite should be used for whole fragment burst
    // In Cisco CCX 2.0 Leap Authentication
    //         WepStatus is Ndis802_11Encryption1Enabled but the key will use PairwiseKey
    //         Instead of the SharedKey, SharedKey Length may be Zero.
    // APFindEncryptKey(pAd, WdsOrAid, bEAPOLFrame, Header_802_11.Addr1[0] & 0x01, &CipherAlg, &KeyTable, &KeyIdx, &pKey);
    pKey = NULL;

    if ((bEAPOLFrame) ||
        (EncryptType == Ndis802_11Encryption2Enabled)  ||
        (EncryptType == Ndis802_11Encryption3Enabled)  ||
        (EncryptType == Ndis802_11Encryption4Enabled))
    {
#ifdef APCLI_SUPPORT
		if (bApCliPacket) 		// to AP Client link
		{	
			KeyIdx     = APCLI_PAIRWISE_KEY_OFFSET + ApCliIdx;
			KeyTable   = PAIRWISE_KEY_TABLE;
			CipherAlg  = pAd->ApCliTab.ApCliEntry[ApCliIdx].PairwiseKey.CipherAlg;
			if (CipherAlg)
				pKey = &pAd->ApCliTab.ApCliEntry[ApCliIdx].PairwiseKey;
		}
		else
#endif			
        if (bWdsPacket)        // to WDS link
        {
            KeyIdx     = WDS_PAIRWISE_KEY_OFFSET + WdsIdx;
            KeyTable   = PAIRWISE_KEY_TABLE;
            CipherAlg  = pAd->WdsTab.Wpa_key.CipherAlg;
            if (CipherAlg)
                pKey = &pAd->WdsTab.Wpa_key;
        }
        else if (pMacEntry)                      // unicast to local BSS
        {
 //iversonadd
                if(k==3){
                      CipherAlg = CIPHER_NONE;
                        KeyTable  = SHARED_KEY_TABLE;
                        KeyIdx    = 0;
                        pKey      = NULL;
                }
                else{
                    KeyIdx     = pMacEntry->Aid;
                    KeyTable   = PAIRWISE_KEY_TABLE;
                    CipherAlg  = pMacEntry->PairwiseKey.CipherAlg;
                    if (CipherAlg)
                        pKey = &pMacEntry->PairwiseKey;
 	       }
        }
        else if (Header_802_11.Addr1[0] & 0x01)        // M/BCAST to local BSS, use default key in shared key table
        {
            KeyTable   = SHARED_KEY_TABLE; 
            CipherAlg  = pAd->PortCfg.MBSSID[apidx].SharedKey[pAd->PortCfg.MBSSID[apidx].DefaultKeyId].CipherAlg;
            if (CipherAlg)
                pKey = &pAd->PortCfg.MBSSID[apidx].SharedKey[pAd->PortCfg.MBSSID[apidx].DefaultKeyId];
            KeyIdx     = pAd->PortCfg.MBSSID[apidx].DefaultKeyId + (SHARE_KEY_NUM * apidx);
        }
    }
    else if ((EncryptType == Ndis802_11Encryption1Enabled) && (pAd->PortCfg.MBSSID[apidx].IEEE8021X == TRUE)) 
    {//OPEN-WEP with 802.1x mode
             
        // unicast to local BSS
        if (pMacEntry)
		{
			KeyIdx     = pMacEntry->Aid;
			KeyTable   = PAIRWISE_KEY_TABLE;
            CipherAlg  = pMacEntry->PairwiseKey.CipherAlg;
            if (CipherAlg)
                pKey = &pMacEntry->PairwiseKey;
    		   
            // If it is 802.1x with dynamic WEP, 
            // use the key index provided by 802.1x for Tx WEP key index  
            if (pMacEntry->bDot1xDynamicWep == TRUE)
                bUseIndividualWepKeyIdx = TRUE;
        }
        else if (Header_802_11.Addr1[0] & 0x01)        // M/BCAST to local BSS, use default key in shared key table
        {
            KeyTable   = SHARED_KEY_TABLE; 
            CipherAlg  = pAd->PortCfg.MBSSID[apidx].SharedKey[pAd->PortCfg.MBSSID[apidx].DefaultKeyId].CipherAlg;
            if (CipherAlg)
                pKey = &pAd->PortCfg.MBSSID[apidx].SharedKey[pAd->PortCfg.MBSSID[apidx].DefaultKeyId];
            KeyIdx     = pAd->PortCfg.MBSSID[apidx].DefaultKeyId + (SHARE_KEY_NUM * apidx);
	     
        }
    }
    else if (EncryptType == Ndis802_11Encryption1Enabled) // WEP or CKIP, always use shared key table
    {
    	//KH: Isolate the WEP of AP Client form Main Bssid.
#ifdef APCLI_SUPPORT
		if (bApCliPacket) 		// to AP Client link
		{	
			KeyTable   = SHARED_KEY_TABLE;    // shared key table
			CipherAlg  = pAd->ApCliTab.ApCliEntry[ApCliIdx].SharedKey[pAd->ApCliTab.ApCliEntry[ApCliIdx].DefaultKeyId].CipherAlg;
			if (CipherAlg)
	     		pKey = &pAd->ApCliTab.ApCliEntry[ApCliIdx].SharedKey[pAd->ApCliTab.ApCliEntry[ApCliIdx].DefaultKeyId];
			KeyIdx     = pAd->ApCliTab.ApCliEntry[ApCliIdx].DefaultKeyId + (SHARE_KEY_NUM * pAd->PortCfg.BssidNum);
		}
		else
#endif
		{
        	KeyTable   = SHARED_KEY_TABLE;    // shared key table
        	CipherAlg  = pAd->PortCfg.MBSSID[apidx].SharedKey[pAd->PortCfg.MBSSID[apidx].DefaultKeyId].CipherAlg;
        	if (CipherAlg)
            	pKey = &pAd->PortCfg.MBSSID[apidx].SharedKey[pAd->PortCfg.MBSSID[apidx].DefaultKeyId];
        	KeyIdx     = pAd->PortCfg.MBSSID[apidx].DefaultKeyId + (SHARE_KEY_NUM * apidx);
    	}
    }
    else
    {
        CipherAlg = CIPHER_NONE;
        KeyTable  = SHARED_KEY_TABLE;
        KeyIdx    = 0;
        pKey      = NULL;
    }

	if ((Header_802_11.Addr1[0] & 0x01) == 0 || !bMcastPkt)
	{
	    DBGPRINT(RT_DEBUG_INFO,"HardTransmit(apidx=%d,EAPOL=%d, Mcast=%d), Alg=%s, Ktab=%d, key#=%d\n", 
	        apidx, bEAPOLFrame, Header_802_11.Addr1[0] & 0x01, CipherName[CipherAlg], KeyTable, KeyIdx);
	}

    if (CipherAlg != CIPHER_NONE){
        Header_802_11.FC.Wep = 1;
  
            if(k==3){
                Header_802_11.FC.Wep = 0;
		k = 0;
            }        
    }
    // STEP 3.1 if TKIP is used and fragmentation is required. Driver has to
    //          append TKIP MIC at tail of the scatter buffer (This must be the
    //          ONLY scatter buffer in the skb buffer). 
    //          MAC ASIC will only perform IV/EIV/ICV insertion but no TKIP MIC
    if ((MpduRequired > 1) && (CipherAlg == CIPHER_TKIP))
    {
        ASSERT(pSGList->NumberOfElements == 1);
        RTMPCalculateMICValue(pAd, pSkb, pExtraLlcSnapEncap, pKey);
        NdisMoveMemory(pSrcBufVA + SrcBufLen, pAd->PrivateInfo.Tx.MIC, 8);
        SrcBufLen += 8;
        pSkb->len += 8;
		SkbLen1 += 8;
        CipherAlg = CIPHER_TKIP_NO_MIC;
    }

    // ----------------------------------------------------------------
    // STEP 3. Make RTS frame or CTS-to-self frame if required
    // ----------------------------------------------------------------

    //
    // calcuate the overhead bytes that encryption algorithm may add. This
    // affects the calculate of "duration" field
    //
	if ((CipherAlg == CIPHER_WEP64) || (CipherAlg == CIPHER_WEP128)) 
		EncryptionOverhead = 8; //WEP: IV[4] + ICV[4];
	else if (CipherAlg == CIPHER_TKIP_NO_MIC)
		EncryptionOverhead = 12;//TKIP: IV[4] + EIV[4] + ICV[4], MIC will be added to TotalPacketLength
	else if (CipherAlg == CIPHER_TKIP)
		EncryptionOverhead = 20;//TKIP: IV[4] + EIV[4] + ICV[4] + MIC[8]
	else if (CipherAlg == CIPHER_AES)
		EncryptionOverhead = 16;    // AES: IV[4] + EIV[4] + MIC[8]
	else
		EncryptionOverhead = 0;

    // decide how much time an ACK/CTS frame will consume in the air
    AckDuration = RTMPCalcDuration(pAd, pAd->PortCfg.ExpectedACKRate[TxRate], 14);

    // EYL: Calculate the real "RtsRequired".
    if (RtsRequired == 2)
    {	// means use B/G protection (set in RTMPSendPacket)
    	RtsRequired = 1;
    }
	else
	{	// Use SkbLen1 instead of pSkb->len because pSkb->len could be after aggregation.
		// And it cause problem here.
    	if ((SkbLen1 + LENGTH_802_11 + LENGTH_CRC - LENGTH_802_3
			+ (bWdsPacket ? 6 : 0)
			+ EncryptionOverhead
			+ ((Header_802_11.FC.SubType == SUBTYPE_QDATA) ? 2 : 0)
			+ (pExtraLlcSnapEncap ? LENGTH_802_1_H : 0)) > pAd->PortCfg.RtsThreshold)
    	{
			RtsRequired = 1;
    	}
    	else
    	{
			RtsRequired = 0;
    	}
	}

    DBGPRINT(RT_DEBUG_INFO,"HardTransmit(Real RTS=%d)\n",RtsRequired);

    if (RtsRequired)
    {
        unsigned int NextMpduSize;

        // If fragment required, MPDU size is maximum fragment size
        // Else, MPDU size should be frame with 802.11 header & CRC
        if (MpduRequired > 1)
            NextMpduSize = pAd->PortCfg.FragmentThreshold;
        else
        {
            NextMpduSize = pSkb->len + LENGTH_802_11 + LENGTH_CRC - LENGTH_802_3;
            if (pExtraLlcSnapEncap)
                NextMpduSize += LENGTH_802_1_H;
			if (bWdsPacket)
				NextMpduSize += 6;
			if (Header_802_11.FC.SubType == SUBTYPE_QDATA) // WMM
				NextMpduSize += 2;
        }

        RTMPSendRTSFrame(pAd, Header_802_11.Addr1, NextMpduSize + EncryptionOverhead,TxRate,
        	pAd->PortCfg.RtsRate/*pAd->PortCfg.ExpectedACKRate[TxRate]*/, AckDuration, QueIdx, FrameGap, apidx);
        
        // RTS/CTS-protected frame should use LONG_RETRY (=4) and SIFS
        RetryMode = LONG_RETRY;
        FrameGap = IFS_SIFS;
        bRTS_CTSFrame = TRUE;
    }

    // decide is the need of piggy-back
    if (pMacEntry && CLIENT_STATUS_TEST_FLAG(pMacEntry, fCLIENT_STATUS_PIGGYBACK_CAPABLE))
    {
    	bPiggyBack = TRUE;
    }
    else
    {
    	bPiggyBack = FALSE;
    }

    // --------------------------------------------------------
    // STEP 5. START MAKING MPDU(s)
    //      Start Copy Ndis Packet into Ring buffer.
    //      For frame required more than one ring buffer (fragment), all ring buffers
    //      have to be filled before kicking start tx bit.
    //      Make sure TX ring resource won't be used by other threads
    // --------------------------------------------------------
    
    SrcBufIdx         = 0;
    HeaderPAOffset    += LENGTH_802_3;  // skip 802.3 header
    SrcBufLen        -= LENGTH_802_3;  // skip 802.3 header
    SrcRemainingBytes = pSkb->len - LENGTH_802_3;

	COPY_MAC_ADDR(&SrcAddr, pSrcBufVA + MAC_ADDR_LEN);
    // If having VLAN tag, skip it when transmit
	if (((pSrcBufVA[12] << 8) + pSrcBufVA[13]) == 0x8100)
	{
		HeaderPAOffset += 4;
		SrcBufLen -= 4;
		SrcRemainingBytes -= 4;
		pSrcBufVA += 4;
	}

#ifndef THREAD_ISR
    RTMP_SEM_LOCK(&pAd->TxRingLock, IrqFlags);
#else
	/*
	 * This function must be called in locked IRQ mode
	 */
	//RTMP_IRQ_LOCK(IrqFlags);
#endif

    do
    {
        PUCHAR pDestBufVA, pDest;
        UINT   SrcBytesCopied;
        UINT   FreeMpduSize, MpduSize = 0;
        
        //
        // STEP 5.1 ACQUIRE TXD
        //
        pDestBufVA = (PUCHAR) pTxRing->Cell[pTxRing->CurTxIndex].DmaBuf.AllocVa;              
#ifndef BIG_ENDIAN
        pTxD  = (PTXD_STRUC) pTxRing->Cell[pTxRing->CurTxIndex].AllocVa;
#else
        pDestTxD = (PTXD_STRUC) pTxRing->Cell[pTxRing->CurTxIndex].AllocVa;
        TxD = *pDestTxD;
        pTxD = &TxD;
        RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif
        
        if ((pTxD->Owner == DESC_OWN_NIC) || (pTxD->bWaitingDmaDoneInt == 1))
        {
            // Descriptor owned by NIC. No descriptor avaliable
            // This should not happen since caller guaranteed.
            DBGPRINT_ERR("RTMPHardTransmit: TX RING full\n");
            pAd->RalinkCounters.TxRingErrCount++;
#ifndef THREAD_ISR
            RTMP_SEM_UNLOCK(&pAd->TxRingLock, IrqFlags);
#else
			//RTMP_IRQ_UNLOCK(IrqFlags);
#endif
#ifdef RTL865X_FAST_PATH
	 		rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
            RELEASE_NDIS_PACKET(pAd, pSkb);
#endif
            if (pNextSkb)
            {
#ifdef RTL865X_FAST_PATH
			 	rtl865x_extDev_kfree_skb(pNextSkb, FALSE);
#else
                RELEASE_NDIS_PACKET(pAd, pNextSkb);
#endif
            }
            return (NDIS_STATUS_RESOURCES);
        }

        //
        // STEP 5.2 PUT IVOFFSET, IV, EIV INTO TXD
        //
        if (bWdsPacket)    // NOTE: no QOS DATA format on WDS link so far
            pTxD->IvOffset = LENGTH_802_11_WITH_ADDR4;
        else 
        {
            pTxD->IvOffset  = LENGTH_802_11;
        
            if (pAd->PortCfg.MBSSID[apidx].bWmmCapable && pMacEntry && CLIENT_STATUS_TEST_FLAG(pMacEntry, fCLIENT_STATUS_WMM_CAPABLE))
                pTxD->IvOffset += 2;  // add QOS CONTROL bytes
        }
        
        if ((CipherAlg == CIPHER_WEP64) || (CipherAlg == CIPHER_WEP128))
        {
			PUCHAR pTmp;
			
			pTmp = (PUCHAR) &pTxD->Iv;
			*pTmp       = RandomByte(pAd);
			*(pTmp + 1) = RandomByte(pAd);
			*(pTmp + 2) = RandomByte(pAd);
            
            // Set Tx WEP Key Index
            if(bUseIndividualWepKeyIdx == TRUE)
            {
				*(pTmp + 3) = (pAd->PortCfg.MBSSID[apidx].IndividualWepKeyIdx << 6);
            }
            else
            {
//KH: Fixed the WEP Default Key Bug
#ifdef APCLI_SUPPORT
				if (bApCliPacket)   // to AP Client link
					*(pTmp + 3)=(pAd->ApCliTab.ApCliEntry[ApCliIdx].DefaultKeyId << 6);
				else
#endif
				*(pTmp + 3) = (pAd->PortCfg.MBSSID[apidx].DefaultKeyId << 6);
            }
        }
        else if ((CipherAlg == CIPHER_TKIP) || (CipherAlg == CIPHER_TKIP_NO_MIC))
        {
        	UCHAR	kidx;

        	if (Header_802_11.Addr1[0] & 0x01)
        		kidx = pAd->PortCfg.MBSSID[apidx].DefaultKeyId;
        	else
        		kidx = 0;
        	
            RTMPInitTkipEngine(
                pAd,
                pKey->Key,
                kidx,
                Header_802_11.Addr2,
                pKey->TxMic,
                pKey->TxTsc,
                &Iv16,
                &Iv32);

            NdisMoveMemory(&pTxD->Iv, &Iv16, 4);   // Copy IV
            NdisMoveMemory(&pTxD->Eiv, &Iv32, 4);  // Copy EIV
            INC_TX_TSC(pKey->TxTsc);               // Increase TxTsc for next transmission
        }
        else if (CipherAlg == CIPHER_AES)
        {
            PUCHAR  pTmp;
            UCHAR	kidx;

        	if (Header_802_11.Addr1[0] & 0x01)
        		kidx = pAd->PortCfg.MBSSID[apidx].DefaultKeyId;
        	else
        		kidx = 0;
        	
            pTmp = (PUCHAR) &Iv16;
            *pTmp       = pKey->TxTsc[0];
            *(pTmp + 1) = pKey->TxTsc[1];
            *(pTmp + 2) = 0;
            *(pTmp + 3) = (kidx << 6) | 0x20;
            //Iv32 = *(PULONG)(&pKey->TxTsc[2]);
			NdisMoveMemory(&Iv32, &pKey->TxTsc[2], 4);
            
            NdisMoveMemory(&pTxD->Iv, &Iv16, 4);    // Copy IV
            NdisMoveMemory(&pTxD->Eiv, &Iv32, 4);   // Copy EIV
            INC_TX_TSC(pKey->TxTsc);                // Increase TxTsc for next transmission
        }

        //
        // STEP 5.3 COPY 802.11 HEADER INTO 1ST DMA BUFFER
        //
        pDest = pDestBufVA;
        NdisMoveMemory(pDest, &Header_802_11, sizeof(Header_802_11));
        pDest       += sizeof(Header_802_11);
        FreeMpduSize = pAd->PortCfg.FragmentThreshold - sizeof(Header_802_11) - LENGTH_CRC;

		// fragmentation not allowed on multicast & broadcast
		if (Header_802_11.Addr1[0] & 0x01)
			FreeMpduSize = MAX_FRAME_SIZE;

        // TODO: 2004-12-27 add ADDR4 field if WDS frame
        if (bWdsPacket)
        {
            COPY_MAC_ADDR(pDest, &SrcAddr); // ADDR4 = SA
            pDest        += MAC_ADDR_LEN;
            FreeMpduSize -= MAC_ADDR_LEN;
        }
        else
        {
            // TODO: 2005-02-10 also check if the STA is WMM capable
            // NOTE: no QOS DATA format on WDS link so far
            if (pAd->PortCfg.MBSSID[apidx].bWmmCapable && pMacEntry && CLIENT_STATUS_TEST_FLAG(pMacEntry, fCLIENT_STATUS_WMM_CAPABLE))
            {
                // copy QOS CONTROL bytes
               	if (CLIENT_STATUS_TEST_FLAG(pMacEntry, fCLIENT_STATUS_APSD_CAPABLE))
                {
               		*pDest    =  (UserPriority & 0x0f) | (pAd->PortCfg.AckPolicy[QueIdx] << 5) | (!bMoreData << 4);
                }
                else
                {
               		*pDest    =  (UserPriority & 0x0f) | (pAd->PortCfg.AckPolicy[QueIdx] << 5);
                }
                *(pDest+1)    =  0;
                pDest         += 2;
                FreeMpduSize  -= 2;
                if (pAd->PortCfg.AckPolicy[QueIdx] != NORMAL_ACK) // ?????
                {
                    bAckRequired = FALSE;
                    PID = PTYPE_SPECIAL|PSUBTYPE_DATA_NO_ACK;
                }
            }
        }

        // if aggregation, put the 2nd MSDU length(extra 2-byte field) after QOS_CONTROL. little endian
        if (pNextSkb)
        {
            *pDest        =  (UCHAR)NextPacketBufLen & 0xff;
            *(pDest+1)    =  (UCHAR)(NextPacketBufLen >> 8);
            pDest         += 2;
            FreeMpduSize  = MAX_AGGREGATION_SIZE;
            ((PHEADER_802_11)pDestBufVA)->FC.Order = 1; // steal "order" bit to mark "aggregation"
        }

        //
        // STEP 5.4 COPY LLC/SNAP, CKIP MIC INTO 1ST DMA BUFFER ONLY WHEN THIS 
        //          MPDU IS THE 1ST OR ONLY FRAGMENT 
        //
        if (Header_802_11.Frag == 0)
        {
            if (pExtraLlcSnapEncap)
            {
                // Insert LLC-SNAP encapsulation
                NdisMoveMemory(pDest, pExtraLlcSnapEncap, 6);
                pDest += 6;
                NdisMoveMemory(pDest, pSrcBufVA + 12, 2);
                pDest += 2;
                FreeMpduSize -= LENGTH_802_1_H;
            }
        }

        // TX buf0 size fixed here
        pTxD->BufLen0    = (ULONG)((UMAXLONG)pDest - (UMAXLONG)pDestBufVA);
        pTxD->BufCount   = 1;
            
        //
        // STEP 5.5 TRAVERSE scatter-gather list TO BUILD THE MPDU PAYLOAD
        //
        MpduSize         = pTxD->BufLen0;
        SrcBytesCopied   = 0;

        // Make sure the buffer maping address and length are same as unmaping ones, especially in IXP platform.
        if (SrcBufLen == 0)
        {
        }
        else if (SrcBufLen <= FreeMpduSize)
        {
            SrcBufPA = pci_map_single(pAd->pPci_Dev, (PUCHAR)pSGList->Elements[0].Address+HeaderPAOffset+FragPAOffset, SrcBufLen, PCI_DMA_TODEVICE);
        }
        else
        {
            SrcBufPA = pci_map_single(pAd->pPci_Dev, (PUCHAR)pSGList->Elements[0].Address+HeaderPAOffset+FragPAOffset, FreeMpduSize, PCI_DMA_TODEVICE);
        }

        do
        {
            if ((SrcBufLen == 0) || (SrcBufPA == 0))
            {
                // do nothing. skip to next scatter-gather buffer
            }
            else if (SrcBufLen <= FreeMpduSize)
            {
                // scatter-gather buffer still fit into current MPDU
                switch (pTxD->BufCount) {
                    case 1: pTxD->BufPhyAddr1 = SrcBufPA;
                            pTxD->BufLen1     = SrcBufLen;
                            pTxD->BufCount ++;
                            break;
                    case 2: pTxD->BufPhyAddr2 = SrcBufPA;
                            pTxD->BufLen2     = SrcBufLen;
                            pTxD->BufCount ++;
                            break;
                    case 3: pTxD->BufPhyAddr3 = SrcBufPA;
                            pTxD->BufLen3     = SrcBufLen;
                            pTxD->BufCount ++;
                            break;
                    case 4: pTxD->BufPhyAddr4 = SrcBufPA;
                            pTxD->BufLen4     = SrcBufLen;
                            pTxD->BufCount ++;
                            break;
                    default:    // should never happen
                            break;
                }
                SrcBytesCopied  += SrcBufLen;
                pDest           += SrcBufLen;
                FreeMpduSize    -= SrcBufLen;
                MpduSize        += SrcBufLen;
                FragPAOffset    += SrcBufLen;
                SrcBufLen        = 0;
            }
            else
            {
                // scatter-gather buffer exceed current MPDU. leave some of the buffer to next MPDU
                switch (pTxD->BufCount) {
                    case 1: pTxD->BufPhyAddr1 = SrcBufPA;
                            pTxD->BufLen1     = FreeMpduSize;
                            pTxD->BufCount ++;
                            break;
                    case 2: pTxD->BufPhyAddr2 = SrcBufPA;
                            pTxD->BufLen2     = FreeMpduSize;
                            pTxD->BufCount ++;
                            break;
                    case 3: pTxD->BufPhyAddr3 = SrcBufPA;
                            pTxD->BufLen3     = FreeMpduSize;
                            pTxD->BufCount ++;
                            break;
                    case 4: pTxD->BufPhyAddr4 = SrcBufPA;
                            pTxD->BufLen4     = FreeMpduSize;
                            pTxD->BufCount ++;
                            break;
                    default:    // should never happen
                            break;
                }
                SrcBytesCopied += FreeMpduSize;
                pDest          += FreeMpduSize;
                MpduSize       += FreeMpduSize;
                FragPAOffset   += FreeMpduSize;
                SrcBufLen      -= FreeMpduSize;

                // a complete MPDU is built. break out to write TXD of this MPDU
                break;
            }

            // advance to next scatter-gather BUFFER
            SrcBufIdx++;
            if (SrcBufIdx < pSGList->NumberOfElements)
            {
				SrcBufPA = pci_map_single(pAd->pPci_Dev, pSGList->Elements[SrcBufIdx].Address, pSGList->Elements[SrcBufIdx].Length, PCI_DMA_TODEVICE);
                SrcBufLen = pSGList->Elements[SrcBufIdx].Length;
            }
            else
                SrcBufLen = 0;

            if (SrcBufLen == 0)
                break;

        }   while (TRUE);       // End of copying payload

        // remaining size of the skb buffer payload
        SrcRemainingBytes -= SrcBytesCopied;

        //
        // STEP 5.6 MODIFY MORE_FRAGMENT BIT & DURATION FIELD. WRITE TXD
        //
        pHeader80211 = (PHEADER_802_11)pDestBufVA;
        if (SrcRemainingBytes > 0) // more fragment is required
        {
            UINT NextMpduSize;

            pHeader80211->FC.MoreFrag = 1;
            NextMpduSize = min(SrcRemainingBytes, pAd->PortCfg.FragmentThreshold);
            if (NextMpduSize == SrcRemainingBytes)
   			{
   				// In this case, the NextMpduSize doesn't include LENGTH_802_11 and LENGTH_CRC
				pHeader80211->Duration = (3 * pAd->PortCfg.Dsifs) + (2 * AckDuration) + 
					RTMPCalcDuration(pAd, TxRate, NextMpduSize + EncryptionOverhead + LENGTH_802_11 + LENGTH_CRC);
   			}
   			else
   			{
   				// In this case, the NextMpduSize doesn't include LENGTH_CRC
				pHeader80211->Duration = (3 * pAd->PortCfg.Dsifs) + (2 * AckDuration) + 
					RTMPCalcDuration(pAd, TxRate, NextMpduSize + EncryptionOverhead + LENGTH_CRC);
   			}

#ifdef BIG_ENDIAN
            RTMPFrameEndianChange(pAd, (PUCHAR)pHeader80211, DIR_WRITE, FALSE);
            RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
            *pDestTxD = TxD;
            pTxD = pDestTxD;
#endif
#ifdef CONFIG_64BIT
			{
            RTMP_DMACB *dmacb_p = &pTxRing->Cell[pTxRing->CurTxIndex];

            RTMPWriteTxDescriptor(pAd, pTxD, CipherAlg, KeyTable, KeyIdx, bAckRequired, TRUE, FALSE,
            	RetryMode, FrameGap, TxRate, MpduSize, QueIdx, PID, NULL, NULL, pAd->PortCfg.MBSSID[apidx].bWmmCapable, bPiggyBack, bRTS_CTSFrame, TRUE, dmacb_p);
			}
#else
            RTMPWriteTxDescriptor(pAd, pTxD, CipherAlg, KeyTable, KeyIdx, bAckRequired, TRUE, FALSE,
            	RetryMode, FrameGap, TxRate, MpduSize, QueIdx, PID, NULL, NULL, pAd->PortCfg.MBSSID[apidx].bWmmCapable, bPiggyBack, bRTS_CTSFrame, TRUE);
#endif // CONFIG_64BIT //

            FrameGap = IFS_SIFS;     // use SIFS for all subsequent fragments
            Header_802_11.Frag ++;   // increase Frag #
        }
        else // this is the last or only fragment
        {
            pHeader80211->FC.MoreFrag = 0;
            if (pHeader80211->Addr1[0] & 0x01) // multicast/broadcast
                pHeader80211->Duration = 0;
            else
                pHeader80211->Duration = pAd->PortCfg.Dsifs + AckDuration;

            if ((bEAPOLFrame) && (TxRate > RATE_6))
                TxRate = RATE_6;
#ifdef BIG_ENDIAN
            RTMPFrameEndianChange(pAd, (PUCHAR)pHeader80211, DIR_WRITE, FALSE);
            RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
            *pDestTxD = TxD;
            pTxD = pDestTxD;
#endif
#ifdef CONFIG_64BIT
			{
            RTMP_DMACB *dmacb_p = &pTxRing->Cell[pTxRing->CurTxIndex];

            RTMPWriteTxDescriptor(pAd, pTxD, CipherAlg, KeyTable, KeyIdx, bAckRequired, FALSE, FALSE,
            	RetryMode, FrameGap, TxRate, MpduSize, QueIdx, PID, pSkb, pNextSkb, pAd->PortCfg.MBSSID[apidx].bWmmCapable, bPiggyBack, bRTS_CTSFrame, FALSE, dmacb_p);
			}
#else
            RTMPWriteTxDescriptor(pAd, pTxD, CipherAlg, KeyTable, KeyIdx, bAckRequired, FALSE, FALSE, 
            	RetryMode, FrameGap, TxRate, MpduSize, QueIdx, PID, pSkb, pNextSkb, pAd->PortCfg.MBSSID[apidx].bWmmCapable, bPiggyBack, bRTS_CTSFrame, FALSE);
#endif // CONFIG_64BIT //
        }

        INC_RING_INDEX(pTxRing->CurTxIndex, TX_RING_SIZE);
        pAd->RalinkCounters.KickTxCount++;
        
    } while (SrcRemainingBytes > 0);

    // Make sure to release Tx ring resource
#ifndef THREAD_ISR
    RTMP_SEM_UNLOCK(&pAd->TxRingLock, IrqFlags);
#else
	//RTMP_IRQ_UNLOCK(IrqFlags);
#endif

    // -------------------------------------------------
    // STEP 6. KICK TX AND EXIT
    // -------------------------------------------------
    
    if (pAd->PortCfg.MBSSID[apidx].bWmmCapable)
	{
		//
		// Kick all TX rings, QID_AC_VO, QID_AC_VI, QID_AC_BE, QID_AC_BK, to pass WMM testing.
		//
		RTMP_IO_WRITE32(pAd, TX_CNTL_CSR, 0x0000000f); 
	}
	else
	{
		if (QueIdx <= QID_AC_VO)
		{
			RTMP_IO_WRITE32(pAd, TX_CNTL_CSR, BIT8[QueIdx]); 
		}
	}

    // the skb buffer will be released at DMA done interrupt service routine
    return (NDIS_STATUS_SUCCESS);
}

/*
	========================================================================

	Routine	Description:
		To do the enqueue operation and extract the first item of waiting 
		list. If a number of available shared memory segments could meet 
		the request of extracted item, the extracted item will be fragmented
		into shared memory segments.
		
	Arguments:
		pAdapter	Pointer	to our adapter
		pQueue		Pointer to Waiting Queue
		
	Return Value:
		None

	Note:
	
	========================================================================
*/
VOID	RTMPDeQueuePacket(
	IN	PRTMP_ADAPTER	pAdapter,
	IN  BOOLEAN			bMoreData,
	IN	BOOLEAN			bIntContext)
{
	struct sk_buff  *pSkb;
	UCHAR			MpduRequired;
	NDIS_STATUS		Status;
	UCHAR			Count = 0;
	PQUEUE_HEADER	pQueue;
	ULONG			Number;
	UCHAR           QueIdx, FreeNumber;
	UMAXLONG		IrqFlags = 0;
	UCHAR           Order = 1;
	UCHAR           RtsRequired;
	
#ifndef THREAD_ISR
	bIntContext = TRUE;
#endif

	while (1)
	{
		// Reset is in progress, stop immediately
		if ( RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_RESET_IN_PROGRESS) ||
			RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_RADIO_OFF)          ||
			RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_HALT_IN_PROGRESS))
		{
			break;
		}
			
		if (bIntContext==TRUE && (Count >= MAX_TX_PROCESS))
			break;

		// Make sure SendTxWait queue resource won't be used by other threads
#ifndef THREAD_ISR
		RTMP_SEM_LOCK(&pAdapter->TxSwQueueLock, IrqFlags);
#else
		if (bIntContext==FALSE) RTMP_IRQ_LOCK(IrqFlags);
#endif	/* THREAD_ISR */

		if (Order == 0)
			Order = 1;
		else
			Order = 0;

		if ((pQueue = RTMPCheckTxSwQueue(pAdapter, &Number, &QueIdx, Order)) != NULL)  {
        	// Dequeue the first entry from head of queue list
	       	pSkb = (struct sk_buff*)RemoveHeadQueue(pQueue);		

#ifdef BLOCK_NET_IF
			if ((pAdapter->blockQueueTab[QueIdx].SwTxQueueBlockFlag == TRUE)
					&& (pAdapter->TxSwQueue[QueIdx].Number < 1))
				releaseNetIf(&pAdapter->blockQueueTab[QueIdx]);
#endif // BLOCK_NET_IF //

		} else  {
		    // Release TxSwQueue0..3 resources
#ifndef THREAD_ISR
			RTMP_SEM_UNLOCK(&pAdapter->TxSwQueueLock, IrqFlags);
#else
			if (bIntContext==FALSE) RTMP_IRQ_UNLOCK(IrqFlags);
#endif	/* THREAD_ISR */	
			break;
		}

		// RTS or CTS-to-self for B/G protection mode has been set already.
		// There is no need to re-do it here. 
		// Total fragment required = number of fragment + RST if required
		RtsRequired = RTMP_GET_PACKET_RTS(pSkb);
		if (RtsRequired == 2)
    	{	// means use B/G protection (set in RTMPSendPacket)
    		RtsRequired = 1;
    	}
		MpduRequired = RTMP_GET_PACKET_FRAGMENTS(pSkb) + RtsRequired;
		
		if (RTMPFreeTXDRequest(pAdapter, QueIdx, MpduRequired, &FreeNumber) == NDIS_STATUS_SUCCESS)
		{
			if(pQueue == &pAdapter->NullFrameTxSwQueue)
			{
				UCHAR NullFramPID = GetNullFramePID(pSkb);
				Status = NullFrameHardTransmit(
							pAdapter,
							pSkb,
							RTMP_GET_PACKET_TXRATE(pSkb),
							NullFramPID);
				if(Status == NDIS_STATUS_RESOURCES)
					InsertHeadQueue(pQueue, pSkb);
			}
			else
			{
				if (FreeNumber != TX_RING_SIZE && pAdapter->TxSwQueue[QueIdx].Number == 0)
				{
					InsertHeadQueue(pQueue, pSkb);
#ifndef THREAD_ISR
					RTMP_SEM_UNLOCK(&pAdapter->TxSwQueueLock, IrqFlags);
#else
					if (bIntContext==FALSE) RTMP_IRQ_UNLOCK(IrqFlags);
#endif
					break;
				}
				// 2004-10-12 if HardTransmit returns SUCCESS, then the skb buffer should be 
				//            released in later on DMADoneIsr. If HardTransmit returns FAIL, the
				//            the skb buffer should already be released inside RTMPHardTransmit
				Status = RTMPHardTransmit(pAdapter, pSkb, QueIdx);
			}
            if (Status != NDIS_STATUS_SUCCESS)
            {
#ifndef THREAD_ISR
				RTMP_SEM_UNLOCK(&pAdapter->TxSwQueueLock, IrqFlags);
#else
				if (bIntContext==FALSE) RTMP_IRQ_UNLOCK(IrqFlags);
#endif
				break;
            }				
		}	
		else
		{
			InsertHeadQueue(pQueue, pSkb);
			pAdapter->PrivateInfo.TxRingFullCnt++;
			DBGPRINT(RT_DEBUG_INFO,"RTMPDequeuePacket --> Not enough free TxD (CurTxIndex=%d, NextTxDmaDoneIndex=%d)!!!\n",
			    pAdapter->TxRing[QueIdx].CurTxIndex, pAdapter->TxRing[QueIdx].NextTxDmaDoneIndex);			
#ifndef THREAD_ISR
			RTMP_SEM_UNLOCK(&pAdapter->TxSwQueueLock, IrqFlags);
#else
			if (bIntContext==FALSE) RTMP_IRQ_UNLOCK(IrqFlags);
#endif
			break;
		}
		Count++;
		// Release TxSwQueue resources
#ifndef THREAD_ISR
		RTMP_SEM_UNLOCK(&pAdapter->TxSwQueueLock, IrqFlags);
#else
	   if (bIntContext==FALSE) RTMP_IRQ_UNLOCK(IrqFlags);
#endif
	}

}

/*
	========================================================================

	Routine	Description:
		Calculates the duration which is required to transmit out frames 
	with given size and specified rate.
		
	Arguments:
		pAdapter		Pointer	to our adapter
		Rate			Transmit rate
		Size			Frame size in units of byte
		
	Return Value:
		Duration number in units of usec

	Note:
	
	========================================================================
*/
USHORT	RTMPCalcDuration(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	UCHAR			Rate,
	IN	ULONG			Size)
{
	ULONG	Duration = 0;

	if (Rate < RATE_FIRST_OFDM_RATE) // CCK
	{
	    if ((Rate > RATE_1) && (pAdapter->PortCfg.TxPreambleInUsed == Rt802_11PreambleShort))
    		Duration = 96;  // 72+24 preamble+plcp
  		else
            Duration = 192; // 144+48 preamble+plcp
    		
		Duration += (USHORT)((Size << 4) / RateIdTo500Kbps[Rate]);
		if ((Size << 4) % RateIdTo500Kbps[Rate])
			Duration ++;
	}
	else // OFDM rates
	{
		Duration = 20 + 6;      // 16+4 preamble+plcp + Signal Extension
		Duration += 4 * (USHORT)((11 + Size * 4) / RateIdTo500Kbps[Rate]);
		if ((11 + Size * 4) % RateIdTo500Kbps[Rate])
			Duration += 4;
	}
	
	return (USHORT)Duration;
	
}

#ifdef APCLI_SUPPORT
/*
    ========================================================================

    Routine Description:
        Verify the support rate for different PHY type

    Arguments:
        pAd                 Pointer to our adapter

    Return Value:
        None
        
    ========================================================================
*/
VOID	RTMPCheckRates(
	IN		PRTMP_ADAPTER	pAd,
	IN OUT	UCHAR			SupRate[],
	IN OUT	UCHAR			*SupRateLen)
{
	UCHAR	RateIdx, i, j;
	UCHAR	NewRate[12], NewRateLen;
	
	NewRateLen = 0;
	
	if (pAd->PortCfg.PhyMode == PHY_11B)
		RateIdx = 4;
//  else if ((pAd->PortCfg.PhyMode == PHY_11BG_MIXED) && 
//      (pAd->PortCfg.BssType == BSS_ADHOC)           &&
//      (pAd->PortCfg.AdhocMode == 0))
//		RateIdx = 4;
	else
		RateIdx = 12;

	// Check for support rates exclude basic rate bit	
	for (i = 0; i < *SupRateLen; i++)
		for (j = 0; j < RateIdx; j++)
			if ((SupRate[i] & 0x7f) == RateIdTo500Kbps[j])
				NewRate[NewRateLen++] = SupRate[i];
			
	*SupRateLen = NewRateLen;
	NdisMoveMemory(SupRate, NewRate, NewRateLen);
}

#endif

/*
	========================================================================
	
	Routine	Description:
		Calculates the duration which is required to transmit out frames 
	with given size and specified rate.
		
	Arguments:
		pTxD		Pointer to transmit descriptor
		Ack			Setting for Ack requirement bit
		Fragment	Setting for Fragment bit
		RetryMode	Setting for retry mode
		Ifs			Setting for IFS gap
		Rate		Setting for transmit rate
		Service		Setting for service
		Length		Frame length
		TxPreamble  Short or Long preamble when using CCK rates
		QueIdx - 0-3, according to 802.11e/d4.4 June/2003
		
	Return Value:
		None
		
	========================================================================
*/
#ifdef CONFIG_64BIT
VOID	RTMPWriteTxDescriptor(
    IN  PRTMP_ADAPTER pAd,
 	IN	PTXD_STRUC	pSourceTxD,
	IN	UCHAR		CipherAlg,
	IN  UCHAR       KeyTable,
	IN  UCHAR       KeyIdx,
	IN	BOOLEAN		Ack,
	IN	BOOLEAN		Fragment,
	IN  BOOLEAN     InsTimestamp,
	IN	UCHAR		RetryMode,
	IN	UCHAR		Ifs,
	IN	UINT		Rate,
	IN	ULONG		Length,
	IN  UCHAR       QueIdx,
	IN  UCHAR       PID,
	IN  struct sk_buff *pSkb,
	IN  struct sk_buff *pNextSkb,
	IN  BOOLEAN     bWmmCapable,
	IN  BOOLEAN     bPiggyBack,
	IN	BOOLEAN		bAfterRTSCTS,
	IN	BOOLEAN		bBurstMode,
    IN  RTMP_DMACB *pDmacb)
#else
VOID	RTMPWriteTxDescriptor(
    IN  PRTMP_ADAPTER pAd,
 	IN	PTXD_STRUC	pSourceTxD,
	IN	UCHAR		CipherAlg,
	IN  UCHAR       KeyTable,
	IN  UCHAR       KeyIdx,
	IN	BOOLEAN		Ack,
	IN	BOOLEAN		Fragment,
	IN  BOOLEAN     InsTimestamp,
	IN	UCHAR		RetryMode,
	IN	UCHAR		Ifs,
	IN	UINT		Rate,
	IN	ULONG		Length,
	IN  UCHAR       QueIdx,
	IN  UCHAR       PID,
	IN  struct sk_buff *pSkb,
	IN  struct sk_buff *pNextSkb,
	IN  BOOLEAN     bWmmCapable,
	IN  BOOLEAN     bPiggyBack,
	IN	BOOLEAN		bAfterRTSCTS,
	IN	BOOLEAN		bBurstMode)
#endif // CONFIG_64BIT //
{
	UINT	Residual;
	UCHAR	bbpR94;

    PTXD_STRUC      pTxD;
    
#ifndef BIG_ENDIAN
    pTxD = pSourceTxD;
#else
    TXD_STRUC       TxD;
    
    TxD = *pSourceTxD;
    pTxD = &TxD;
    RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif

    DBGPRINT(RT_DEBUG_INFO,"WriteTxD: scatter #%d = %d + %d + %d ... = %d\n", 
        pTxD->BufCount, pTxD->BufLen0, pTxD->BufLen1, pTxD->BufLen2, Length);
    
    pTxD->PiggyBack   = (bPiggyBack) ? 1: 0;
	pTxD->Burst       = (bBurstMode) ? 1: 0;
    pTxD->HostQId     = QueIdx;
	pTxD->MoreFrag    = Fragment;
	pTxD->ACK         = Ack;
	pTxD->Timestamp   = InsTimestamp;
	pTxD->RetryMd     = RetryMode;
    pTxD->Ofdm        = (Rate < RATE_FIRST_OFDM_RATE)? 0:1;
	pTxD->IFS         = Ifs;
    pTxD->PktId       = PID;
    pTxD->Drop        = 1;   // 1:valid, 0:drop
    pTxD->HwSeq       = (QueIdx == QID_MGMT)? 1:0; 
	pTxD->DataByteCnt = Length;
    
	if (pAd->TxPowerDelta.field.Enable == 1)
	{
	    if (pAd->TxPowerDelta.field.Sign == 1)
	    {
	    	if ((pAd->LatchBBPR94 + pAd->TxPowerDelta.field.Delta) < 12)
	    		bbpR94 = pAd->LatchBBPR94 + pAd->TxPowerDelta.field.Delta;
	    	else
	    		bbpR94 = 12;
	    }
	    else
	    {
	    	if ((pAd->LatchBBPR94 - pAd->TxPowerDelta.field.Delta) > 0)
		    	bbpR94 = pAd->LatchBBPR94 - pAd->TxPowerDelta.field.Delta;
	    	else
	    		bbpR94 = 0;
	    }
	    pTxD->BbpTxPower  = (Rate < RATE_FIRST_OFDM_RATE) ? bbpR94 : pAd->LatchBBPR94;
	    RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R94, pTxD->BbpTxPower);
	}
	else
		pTxD->BbpTxPower  = DEFAULT_BBP_TX_POWER;
		
    // fill encryption related information, if required
	pTxD->CipherAlg   = CipherAlg;
    if (CipherAlg != CIPHER_NONE)
    {
        pTxD->KeyTable    = KeyTable;
        pTxD->KeyIndex    = KeyIdx;
    	pTxD->TkipMic     = 1;
    }

    // In TKIP+fragmentation. TKIP MIC is already appended by driver. MAC needs not generate MIC
    if (CipherAlg == CIPHER_TKIP_NO_MIC)
    {
        pTxD->CipherAlg   = CIPHER_TKIP;
    	pTxD->TkipMic     = 0;   // tell MAC need not insert TKIP MIC
    }
        
    if ((bWmmCapable) && (pAd->PortCfg.APEdcaParm.bValid) && (QueIdx <= QID_AC_VO))
    {
        pTxD->Cwmin = pAd->PortCfg.APEdcaParm.Cwmin[QueIdx];
        pTxD->Cwmax = pAd->PortCfg.APEdcaParm.Cwmax[QueIdx];
        pTxD->Aifsn = pAd->PortCfg.APEdcaParm.Aifsn[QueIdx];
        //
		// Modify Cwmin/Cwmax on queue[QID_AC_VI], Recommend by Jerry 2005/07/27
		// To degrade our VIDO Queue's throughput for WiFi WMM S3T07 Issue.
		//		
		if (QueIdx == QID_AC_VI)
		{
			pTxD->Cwmin += 1;
			pTxD->Cwmax += 1;
		}
    }
    else
    {
    	if (bAfterRTSCTS)		
		{
			// After RTS/CTS frame, data frame should use SIFS time.
			// To patch this code, add the following code.
			// Recommended by Jerry 2005/07/25 for WiFi testing with Proxim AP
			pTxD->Cwmin = 0;
			pTxD->Cwmax = 0;
			pTxD->Aifsn = 1;
			pTxD->IFS = IFS_BACKOFF;
		}
    	else
    	{
	        pTxD->Cwmin = CW_MIN_IN_BITS;
	        pTxD->Cwmax = CW_MAX_IN_BITS;
	        pTxD->Aifsn = 2;
    	}
    }

	// fill up PLCP SIGNAL field
	pTxD->PlcpSignal = RateIdToPlcpSignal[Rate];
	if (((Rate == RATE_2) || (Rate == RATE_5_5) || (Rate == RATE_11)) && 
	    (pAd->PortCfg.TxPreambleInUsed == Rt802_11PreambleShort))
	{
		pTxD->PlcpSignal |= 0x0008;
	}

	// fill up PLCP SERVICE field, not used for OFDM rates
	pTxD->PlcpService = 4; // Service;

	// file up PLCP LENGTH_LOW and LENGTH_HIGH fields
	Length += LENGTH_CRC;   // CRC length
	switch (CipherAlg) 
    {
        case CIPHER_WEP64:       Length += 8;    break;  // IV + ICV
        case CIPHER_WEP128:      Length += 8;    break;  // IV + ICV
        case CIPHER_TKIP:        Length += 20;   break;  // IV + EIV + MIC + ICV
        case CIPHER_AES:         Length += 16;   break;  // IV + EIV + MIC
        case CIPHER_CKIP64:      Length += 8;    break;  // IV + CMIC + ICV, but CMIC already inserted by driver
        case CIPHER_CKIP128:     Length += 8;    break;  // IV + CMIC + ICV, but CMIC already inserted by driver
        case CIPHER_TKIP_NO_MIC: Length += 12;   break;  // IV + EIV + ICV
        default:                                 break;
    }

    // TODO: to be removed
    if (pNextSkb)
    {
        DBGPRINT(RT_DEBUG_INFO,"WriteTxD (%s, Len=%d PLCPlen=%d\n)",CipherName[CipherAlg], pTxD->DataByteCnt, Length);
        pAd->RalinkCounters.OneSecTxAggregationCount ++;
    }
    
	if (Rate < RATE_FIRST_OFDM_RATE)    // 11b - RATE_1, RATE_2, RATE_5_5, RATE_11
	{
		if ((Rate == RATE_1) || ( Rate == RATE_2))
		{
			Length = Length * 8 / (Rate + 1);
		}
		else
		{
			Residual = ((Length * 16) % (11 * (1 + Rate - RATE_5_5)));
			Length = Length * 16 / (11 * (1 + Rate - RATE_5_5));
			if (Residual != 0)
			{
				Length++;
			}
			if ((Residual <= (3 * (1 + Rate - RATE_5_5))) && (Residual != 0))
			{
				if (Rate == RATE_11)			// Only 11Mbps require length extension bit
					pTxD->PlcpService |= 0x80; // 11b's PLCP Length extension bit
			}
		}

		pTxD->PlcpLengthHigh = Length >> 8; // 256;
		pTxD->PlcpLengthLow = Length % 256;
	}
	else    // OFDM - RATE_6, RATE_9, RATE_12, RATE_18, RATE_24, RATE_36, RATE_48, RATE_54
	{
		pTxD->PlcpLengthHigh = Length >> 6; // 64;  // high 6-bit of total byte count
		pTxD->PlcpLengthLow = Length % 64;   // low 6-bit of total byte count
	}

#ifdef CONFIG_64BIT
    pTxD->Rsv10[0] = 0;
    pTxD->Rsv10[1] = 0;

    if (pDmacb != NULL)
    {
        pDmacb->pSkb = pSkb;
        pDmacb->pNextSkb = pNextSkb;
    } /* End of if */
#else
    pTxD->pSkb					= pSkb;
    pTxD->pNextSkb				= pNextSkb;
#endif // CONFIG_64BIT
    pTxD->bWaitingDmaDoneInt    = 1;
	pTxD->Owner                 = DESC_OWN_NIC;     // change OWNER bit at the last	

#ifdef BIG_ENDIAN
    RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
    WriteBackToDescriptor((PUCHAR)pSourceTxD, (PUCHAR)pTxD, FALSE, TYPE_TXD);
#endif
}

/*
	========================================================================

	Routine	Description:
		Search tuple cache for receive duplicate frame from unicast frames.
		
	Arguments:
		pAdapter		Pointer	to our adapter
		pHeader			802.11 header of receiving frame
		
	Return Value:
		TRUE			found matched tuple cache
		FALSE			no matched found

	Note:
	
	========================================================================
*/
BOOLEAN	RTMPSearchTupleCache(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	PHEADER_802_11	pHeader)
{
	INT	Index;

	for (Index = 0; Index < MAX_CLIENT; Index++)
	{
		if (pAdapter->TupleCache[Index].Valid == FALSE)
		    continue;
		
		if (MAC_ADDR_EQUAL(pAdapter->TupleCache[Index].MacAddress, pHeader->Addr2) &&
			(pAdapter->TupleCache[Index].Sequence == pHeader->Sequence) &&
			(pAdapter->TupleCache[Index].Frag == pHeader->Frag))
		{
//			DBGPRINT(RT_DEBUG_TRACE,("DUPCHECK - duplicate frame hit entry %d\n", Index)); 
			return (TRUE);
		}
	}
	return (FALSE);
}

/*
	========================================================================

	Routine	Description:
		Update tuple cache for new received unicast frames.
		
	Arguments:
		pAdapter		Pointer	to our adapter
		pHeader			802.11 header of receiving frame
		
	Return Value:
		None
		
	Note:
	
	========================================================================
*/
VOID	RTMPUpdateTupleCache(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	PHEADER_802_11	pHeader)
{
	UCHAR	Index;

	for (Index = 0; Index < MAX_CLIENT; Index++)
	{
		if (pAdapter->TupleCache[Index].Valid == FALSE)
		{
			// Add new entry
			COPY_MAC_ADDR(pAdapter->TupleCache[Index].MacAddress, pHeader->Addr2);
			pAdapter->TupleCache[Index].Sequence = pHeader->Sequence;
			pAdapter->TupleCache[Index].Frag     = pHeader->Frag;
			pAdapter->TupleCache[Index].Valid    = TRUE;
			pAdapter->TupleCacheLastUpdateIndex  = Index;
			DBGPRINT(RT_DEBUG_INFO,"DUPCHECK - Add Entry %d, MAC=%02x:%02x:%02x:%02x:%02x:%02x\n", Index,
			    pAdapter->TupleCache[Index].MacAddress[0], pAdapter->TupleCache[Index].MacAddress[1],
			    pAdapter->TupleCache[Index].MacAddress[2], pAdapter->TupleCache[Index].MacAddress[3],
			    pAdapter->TupleCache[Index].MacAddress[4], pAdapter->TupleCache[Index].MacAddress[5]);
			return;
		}
		else if (MAC_ADDR_EQUAL(pAdapter->TupleCache[Index].MacAddress, pHeader->Addr2))
		{
			// Update old entry
			pAdapter->TupleCache[Index].Sequence = pHeader->Sequence;
			pAdapter->TupleCache[Index].Frag     = pHeader->Frag;
			return;
		}
	}

    // tuple cache full, replace the first inserted one (even though it may not be
    // least referenced one)
	if (Index == MAX_CLIENT)
	{
	    pAdapter->TupleCacheLastUpdateIndex ++;
	    if (pAdapter->TupleCacheLastUpdateIndex >= MAX_CLIENT)
	        pAdapter->TupleCacheLastUpdateIndex = 0;
	    Index = pAdapter->TupleCacheLastUpdateIndex;

		// replace with new entry
		COPY_MAC_ADDR(pAdapter->TupleCache[Index].MacAddress, pHeader->Addr2);
		pAdapter->TupleCache[Index].Sequence = pHeader->Sequence;
		pAdapter->TupleCache[Index].Frag     = pHeader->Frag;
		pAdapter->TupleCache[Index].Valid    = TRUE;
		DBGPRINT(RT_DEBUG_INFO,"DUPCHECK - replace Entry %d, MAC=%02x:%02x:%02x:%02x:%02x:%02x\n", Index,
		    pAdapter->TupleCache[Index].MacAddress[0], pAdapter->TupleCache[Index].MacAddress[1],
		    pAdapter->TupleCache[Index].MacAddress[2], pAdapter->TupleCache[Index].MacAddress[3],
		    pAdapter->TupleCache[Index].MacAddress[4], pAdapter->TupleCache[Index].MacAddress[5]);
	}
}

/*
	========================================================================

	Routine	Description:
		Suspend MSDU transmission
		
	Arguments:
		pAdapter		Pointer	to our adapter
		
	Return Value:
		None
		
	Note:
	
	========================================================================
*/
VOID    RTMPSuspendMsduTransmission(
	IN	PRTMP_ADAPTER	pAd)
{
	DBGPRINT(RT_DEBUG_TRACE,"SCANNING, suspend MSDU transmission ...\n");

	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS);
	RTMP_IO_WRITE32(pAd, TX_CNTL_CSR, 0x000f0000);      // abort all TX rings
}

/*
	========================================================================

	Routine	Description:
		Resume MSDU transmission
		
	Arguments:
		pAdapter		Pointer	to our adapter
		
	Return Value:
		None
		
	Note:
	
	========================================================================
*/
VOID    RTMPResumeMsduTransmission(
	IN	PRTMP_ADAPTER	pAd)
{
	DBGPRINT(RT_DEBUG_TRACE,"SCAN done, resume MSDU transmission ...\n");
	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS);
    RTMPDeQueuePacket(pAd, FALSE, TRUE);
  	RTMP_IO_WRITE32(pAd, TX_CNTL_CSR, 0x0000000f);      // kick all TX rings
}

/*
	========================================================================

	Routine	Description:
		Check Rx descriptor, return NDIS_STATUS_FAILURE if any error dound
		
	Arguments:
		pRxD		Pointer	to the Rx descriptor
		
	Return Value:
		NDIS_STATUS_SUCCESS		No err
		NDIS_STATUS_FAILURE		Error
		
	Note:
	
	========================================================================
*/
NDIS_STATUS	RTMPCheckRxError(
	IN	PRXD_STRUC		pRxD)
{
	if (pRxD->Crc || pRxD->CipherErr)
        return NDIS_STATUS_FAILURE;
    else
        return NDIS_STATUS_SUCCESS;
}

/*
	========================================================================

	Routine	Description:
		Check and fine the packet waiting in SW queue with highest priority
		
	Arguments:
		pAdapter	Pointer	to our adapter
		
	Return Value:
		pQueue		Pointer to Waiting Queue

	Note:
	
	========================================================================
*/
PQUEUE_HEADER	RTMPCheckTxSwQueue(
	IN	PRTMP_ADAPTER	pAdapter,
	OUT	PULONG			pNumber,
	OUT PUCHAR           pQueIdx,
	IN UCHAR           Order)
{
	// Calculate total number of packets waiting in queues for Nitro mode 
	*pNumber = pAdapter->TxSwQueue[QID_AC_BK].Number 
	         + pAdapter->TxSwQueue[QID_AC_BE].Number 
	         + pAdapter->TxSwQueue[QID_AC_VI].Number 
	         + pAdapter->TxSwQueue[QID_AC_VO].Number
	         + pAdapter->TxSwQueue[QID_HCCA].Number;

	/* NULL fram queue have first priority */
	if(pAdapter->NullFrameTxSwQueue.Head != NULL)
	{
		*pQueIdx = 0;
		return (&pAdapter->NullFrameTxSwQueue);
	}

	if (Order == 1)
	{
        if (pAdapter->TxSwQueue[QID_HCCA].Head != NULL)
		{
		             *pQueIdx = QID_HCCA;
		             return (&pAdapter->TxSwQueue[QID_HCCA]);
		}
		else if (pAdapter->TxSwQueue[QID_AC_BK].Head != NULL)
		{
		             *pQueIdx = QID_AC_BK;
		             return (&pAdapter->TxSwQueue[QID_AC_BK]);
		}
		else if (pAdapter->TxSwQueue[QID_AC_BE].Head != NULL)
        {
                     *pQueIdx = QID_AC_BE;
                     return (&pAdapter->TxSwQueue[QID_AC_BE]);
        }
		else if (pAdapter->TxSwQueue[QID_AC_VI].Head != NULL)
        {
                     *pQueIdx = QID_AC_VI;
                     return (&pAdapter->TxSwQueue[QID_AC_VI]);
        }
		else if (pAdapter->TxSwQueue[QID_AC_VO].Head != NULL)
		{
		            *pQueIdx = QID_AC_VO;
		            return (&pAdapter->TxSwQueue[QID_AC_VO]);
        }
	}

	if (pAdapter->TxSwQueue[QID_AC_VO].Head != NULL)
	{
	    *pQueIdx = QID_AC_VO;
		return (&pAdapter->TxSwQueue[QID_AC_VO]);
	}
	else if (pAdapter->TxSwQueue[QID_AC_VI].Head != NULL)
	{
	    *pQueIdx = QID_AC_VI;
		return (&pAdapter->TxSwQueue[QID_AC_VI]);
	}
	else if (pAdapter->TxSwQueue[QID_AC_BE].Head != NULL)
	{
	    *pQueIdx = QID_AC_BE;
		return (&pAdapter->TxSwQueue[QID_AC_BE]);
	}
	else if (pAdapter->TxSwQueue[QID_AC_BK].Head != NULL)
	{
	    *pQueIdx = QID_AC_BK;
		return (&pAdapter->TxSwQueue[QID_AC_BK]);
	}
	else if (pAdapter->TxSwQueue[QID_HCCA].Head != NULL)
	{
	    *pQueIdx = QID_HCCA;
		return (&pAdapter->TxSwQueue[QID_HCCA]);
	}

	// No packet pending in Tx Sw queue
    *pQueIdx = QID_AC_BK;
	return (NULL);
}

// clone an input skb buffer to another one.
// NOTE: internally created NDIS_PACKET should be destroyed by RTMPFreeNdisPacket
#ifndef RTL865X_SOC 
NDIS_STATUS RTMPCloneNdisPacket(
#else
static inline NDIS_STATUS RTMPCloneNdisPacket(
#endif
    IN  PRTMP_ADAPTER   pAdapter,
    IN  struct sk_buff  *pInSkb,
    OUT struct sk_buff  **ppOutSkb)
{
    if ((*ppOutSkb = skb_clone(pInSkb, MEM_ALLOC_FLAG)) == NULL)
    	return NDIS_STATUS_FAILURE;

    RTMP_SET_PACKET_SOURCE(*ppOutSkb, PKTSRC_DRIVER);
    RTMP_SET_PACKET_MOREDATA(*ppOutSkb, FALSE);

    return NDIS_STATUS_SUCCESS;
}

// the allocated Skb buffer must be freed via RTMPFreeNdisPacket()
NDIS_STATUS RTMPAllocateNdisPacket(
    IN  PRTMP_ADAPTER   pAdapter,
    OUT struct sk_buff  **ppSkb,
    IN  PUCHAR          pData,
    IN  UINT            DataLen)
{
    // 1. Allocate Skb buffer
    *ppSkb = __dev_alloc_skb(DataLen + 2, MEM_ALLOC_FLAG);
    if (*ppSkb == NULL)
        return NDIS_STATUS_RESOURCES;

    // 2. Set packet flag
	skb_reserve(*ppSkb, 2);	// 16 byte align the IP header
	RTMP_SET_PACKET_SOURCE(*ppSkb, PKTSRC_DRIVER);
	RTMP_SET_PACKET_MOREDATA(*ppSkb, FALSE);

    // 3. clone the frame content
    NdisMoveMemory(skb_put(*ppSkb, DataLen), pData, DataLen);
    
    return NDIS_STATUS_SUCCESS;
}

VOID RTMPFreeTXDUponTxDmaDone(
    IN PRTMP_ADAPTER    pAd, 
    IN UCHAR            QueIdx,
    IN UINT             RingSize)
{
    PRTMP_TX_RING	pTxRing;
    PTXD_STRUC		pTxD;
#ifdef	BIG_ENDIAN
    PTXD_STRUC      pDestTxD;
    TXD_STRUC       TxD;
#endif
    struct sk_buff	*pSkb;
    int				i;

    ASSERT(QueIdx < NUM_OF_TX_RING);
    pTxRing = &pAd->TxRing[QueIdx];
	
    for (i=0; i<MAX_DMA_DONE_PROCESS; i++)
    {
#ifndef BIG_ENDIAN
	    pTxD = (PTXD_STRUC)	(pTxRing->Cell[pTxRing->NextTxDmaDoneIndex].AllocVa);
#else
        pDestTxD = (PTXD_STRUC) (pTxRing->Cell[pTxRing->NextTxDmaDoneIndex].AllocVa);
        TxD = *pDestTxD;
        pTxD = &TxD;
        RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif

        if (pTxD->Owner == DESC_OWN_NIC) 
	        break;
        if (pTxD->bWaitingDmaDoneInt == 0)
        {
            if (pTxRing->NextTxDmaDoneIndex != pTxRing->CurTxIndex)
            {
                // should never reach here!!!!
                DBGPRINT_TX_RING(pAd, QueIdx);
                // dump TXD at NextTxDmaDoneIndex, check sequence#, check Airopeek if
                // the frame been transmitted out or not.
#ifndef CONFIG_64BIT
                DBGPRINT_ERR("pNdisPacket[%d]=0x%p\n",pTxRing->NextTxDmaDoneIndex, pTxD->pSkb);
#else
				DBGPRINT_ERR("pNdisPacket[%d]=0x%p\n",pTxRing->NextTxDmaDoneIndex, pTxRing->Cell[pTxRing->NextTxDmaDoneIndex].pSkb);
#endif // CONFIG_64BIT //
                INC_RING_INDEX(pTxRing->NextTxDmaDoneIndex, RingSize);
                continue;
            }
            else
                break;
        }

        pTxD->bWaitingDmaDoneInt = 0;
	    INC_RING_INDEX(pTxRing->NextTxDmaDoneIndex, RingSize);
        pAd->RalinkCounters.TransmittedByteCount +=  pTxD->DataByteCnt;
        pAd->RalinkCounters.OneSecDmaDoneCount[QueIdx] ++;


#ifdef RALINK_ATE
		if(pAd->ate.Mode == ATE_TXFRAME && QueIdx == QID_AC_BE)
		{
			pAd->ate.TxDoneCount++;
			if ((pAd->ate.TxCount - pAd->ate.TxDoneCount) >= TX_RING_SIZE)
			{
#ifndef CONFIG_64BIT
				pSkb = pTxD->pSkb;
#else
				pSkb = pTxRing->Cell[pTxRing->NextTxDmaDoneIndex].pSkb
#endif // CONFIG_64BIT //
				ASSERT(pSkb);
#ifdef BIG_ENDIAN
        	    RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
    	        *pDestTxD = TxD;
	            pTxD = pDestTxD;
#endif
#ifndef CONFIG_64BIT
				RTMPWriteTxDescriptor(pAd, pTxD, CIPHER_NONE, 0, 0, FALSE, FALSE, FALSE, SHORT_RETRY,
					IFS_BACKOFF, pAd->ate.TxRate, pAd->ate.TxLength, QID_AC_BE, 0, pSkb, NULL, FALSE, FALSE, FALSE, FALSE);
#else
				RTMPWriteTxDescriptor(pAd, pTxD, CIPHER_NONE, 0, 0, FALSE, FALSE, FALSE, SHORT_RETRY,
					IFS_BACKOFF, pAd->ate.TxRate, pAd->ate.TxLength, QID_AC_BE, 0, pSkb, NULL, FALSE, FALSE, FALSE, FALSE, &pTxRing->Cell[pTxRing->NextTxDmaDoneIndex]);
#endif // CONFIG_64BIT //

				INC_RING_INDEX(pTxRing->CurTxIndex, TX_RING_SIZE);

				RTMP_IO_WRITE32(pAd, TX_CNTL_CSR, BIT8[QID_AC_BE]);
				continue;
			}
		}
#endif	//#ifdef RALINK_ATE

        // unmap skb buffer before release skb buffer
        if (pTxD->BufLen1 > 0)
        {
            pci_unmap_single(pAd->pPci_Dev, pTxD->BufPhyAddr1, pTxD->BufLen1, PCI_DMA_TODEVICE);
            pTxD->BufLen1 = 0;
        }
        if (pTxD->BufLen2 > 0)
        {
            pci_unmap_single(pAd->pPci_Dev, pTxD->BufPhyAddr2, pTxD->BufLen2, PCI_DMA_TODEVICE);
            pTxD->BufLen2 = 0;
        }

#ifndef CONFIG_64BIT
	    pSkb = pTxD->pSkb;
        if (pSkb)
        {
            pTxD->pSkb = 0;
#ifdef RTL865X_FAST_PATH
			rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
            RELEASE_NDIS_PACKET(pAd, pSkb);
#endif			
        }
        pSkb = pTxD->pNextSkb;
        if (pSkb)
        {
            pTxD->pNextSkb = 0;
#ifdef RTL865X_FAST_PATH
			rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
            RELEASE_NDIS_PACKET(pAd, pSkb);
#endif
        }
#else // CONFIG_64BIT //
		pSkb = pTxRing->Cell[pTxRing->NextTxDmaDoneIndex].pSkb;
        if (pSkb)
        {
            pTxRing->Cell[pTxRing->NextTxDmaDoneIndex].pSkb = 0;
#ifdef RTL865X_FAST_PATH
			rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
            RELEASE_NDIS_PACKET(pAd, pSkb);
#endif			
        }
        pSkb = pTxRing->Cell[pTxRing->NextTxDmaDoneIndex].pNextSkb;
        if (pSkb)
        {
            pTxRing->Cell[pTxRing->NextTxDmaDoneIndex].pNextSkb = 0;
#ifdef RTL865X_FAST_PATH
			rtl865x_extDev_kfree_skb(pSkb, FALSE);
#else
            RELEASE_NDIS_PACKET(pAd, pSkb);
#endif
        }
#endif // CONFIG_64BIT //
		
#ifdef BIG_ENDIAN
        RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
        *pDestTxD = TxD;
#endif
    }
}   

VOID DBGPRINT_TX_RING(
    IN PRTMP_ADAPTER  pAdapter,
    IN UCHAR          QueIdx)
{
    ULONG       PbfCsr, Ac0Base, Ac0CurAddr;
    ULONG       Ac0HwIdx, Ac0SwIdx, AC0DmaDoneIdx;
    PTXD_STRUC pTxD;
#ifdef	BIG_ENDIAN
    PTXD_STRUC      pDestTxD;
    TXD_STRUC       TxD;
#endif
    int        i;
        
    RTMP_IO_READ32(pAdapter, PBF_QUEUE_CSR, &PbfCsr);
    switch (QueIdx)
    {
        case QID_AC_BE:
            RTMP_IO_READ32(pAdapter, AC0_BASE_CSR, &Ac0Base);
            RTMP_IO_READ32(pAdapter, AC0_TXPTR_CSR, &Ac0CurAddr);
            break;
        case QID_AC_BK:
            RTMP_IO_READ32(pAdapter, AC1_BASE_CSR, &Ac0Base);
            RTMP_IO_READ32(pAdapter, AC1_TXPTR_CSR, &Ac0CurAddr);
            break;
        case QID_AC_VI:
            RTMP_IO_READ32(pAdapter, AC2_BASE_CSR, &Ac0Base);
            RTMP_IO_READ32(pAdapter, AC2_TXPTR_CSR, &Ac0CurAddr);
            break;
        case QID_AC_VO:
            RTMP_IO_READ32(pAdapter, AC3_BASE_CSR, &Ac0Base);
            RTMP_IO_READ32(pAdapter, AC3_TXPTR_CSR, &Ac0CurAddr);
            break;
        default:
            DBGPRINT_ERR("DBGPRINT_TX_RING(Ring %d) not supported\n", QueIdx);
            return;
    }
    Ac0HwIdx = (Ac0CurAddr - Ac0Base) / TXD_SIZE;
    Ac0SwIdx = pAdapter->TxRing[QueIdx].CurTxIndex;
    AC0DmaDoneIdx = pAdapter->TxRing[QueIdx].NextTxDmaDoneIndex;
    
    printk("TxRing%d PBF=%08x, HwIdx=%d, SwIdx[%d]=", QueIdx, PbfCsr, Ac0HwIdx, Ac0SwIdx);
    for (i=0;i<4;i++)
    {
#ifndef BIG_ENDIAN
        pTxD = (PTXD_STRUC)pAdapter->TxRing[QueIdx].Cell[Ac0SwIdx].AllocVa;
#else
        pDestTxD = (PTXD_STRUC)pAdapter->TxRing[QueIdx].Cell[Ac0SwIdx].AllocVa;
        TxD = *pDestTxD;
        pTxD = &TxD;
        RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif

        printk("%d%d.", pTxD->Owner, pTxD->bWaitingDmaDoneInt);
        INC_RING_INDEX(Ac0SwIdx, TX_RING_SIZE);
    }
    printk("DmaIdx[%d]=", AC0DmaDoneIdx);
    for (i=0;i<4;i++)
    {
#ifndef BIG_ENDIAN
        pTxD = (PTXD_STRUC)pAdapter->TxRing[QueIdx].Cell[AC0DmaDoneIdx].AllocVa;
#else
        pDestTxD = (PTXD_STRUC)pAdapter->TxRing[QueIdx].Cell[AC0DmaDoneIdx].AllocVa;
        TxD = *pDestTxD;
        pTxD = &TxD;
        RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif

        printk("%d%d.", pTxD->Owner, pTxD->bWaitingDmaDoneInt);
        INC_RING_INDEX(AC0DmaDoneIdx, TX_RING_SIZE);
    }
    DBGPRINT(RT_DEBUG_TRACE,"p-NDIS=%ld\n", pAdapter->RalinkCounters.PendingNdisPacketCount);
}

VOID	RTMPPrepareRDCTSFrame(
	IN	PRTMP_ADAPTER	pAd,
	IN	PUCHAR			pDA,
	IN	ULONG			Duration,
	IN  UCHAR           RTSRate,
	IN  UCHAR           CTSIdx,
	IN  UCHAR			FrameGap)
{
	RTS_FRAME		    RtsFrame;
	PRTS_FRAME		    pRtsFrame;
	TXD_STRUC		    TxD;
	PTXD_STRUC		    pTxD;
	UCHAR				*ptr;
	UINT				i;
		
   	pRtsFrame = &RtsFrame;
    pTxD = &TxD;
				
	NdisZeroMemory(pRtsFrame, sizeof(RTS_FRAME));
	pRtsFrame->FC.Type    = BTYPE_CNTL;
	pRtsFrame->Duration = (USHORT)Duration;

	// Write Tx descriptor
	pRtsFrame->FC.SubType = SUBTYPE_CTS;
	COPY_MAC_ADDR(pRtsFrame->Addr1, pAd->CurrentAddress);
#ifdef BIG_ENDIAN
	RTMPFrameEndianChange(pAd, (PUCHAR)pRtsFrame, DIR_WRITE, FALSE);
#endif
#ifdef CONFIG_64BIT
	RTMPWriteTxDescriptor(pAd, pTxD, CIPHER_NONE, 0,0, FALSE, TRUE, FALSE, SHORT_RETRY,
		FrameGap, RTSRate, 10, QID_OTHER, PTYPE_SPECIAL|PSUBTYPE_OTHER_CNTL, NULL, NULL, pAd->PortCfg.MBSSID[MAIN_MBSSID].bWmmCapable, FALSE, FALSE, FALSE, NULL);
#else
	RTMPWriteTxDescriptor(pAd, pTxD, CIPHER_NONE, 0,0, FALSE, TRUE, FALSE, SHORT_RETRY,
		FrameGap, RTSRate, 10, QID_OTHER, PTYPE_SPECIAL|PSUBTYPE_OTHER_CNTL, NULL, NULL, pAd->PortCfg.MBSSID[MAIN_MBSSID].bWmmCapable, FALSE, FALSE, FALSE);
#endif // CONFIG_64BIT //

	ptr = (PUCHAR)pTxD;
    for (i=0; i<TXINFO_SIZE; i++)  // 24-byte TXINFO field
    {
        RTMP_IO_WRITE8(pAd, HW_CTS_FRAME_BASE + CTSIdx * HW_CTS_FRAME_SIZE + i, *ptr);
        ptr ++;
    }

    // update CTS frame content. start right after the 24-byte TXINFO field
    ptr = (PUCHAR)pRtsFrame;
    for (i=0; i<10; i++)
    {
        RTMP_IO_WRITE8(pAd, HW_CTS_FRAME_BASE + CTSIdx * HW_CTS_FRAME_SIZE + TXINFO_SIZE + i, *ptr); 
        ptr ++;
    }
}

VOID RTMPHandleUAPSDTriggerFrame(
	IN  PRTMP_ADAPTER   pAd,
	IN  MAC_TABLE_ENTRY *pEntry,
    IN	UCHAR			OldUP)
{
	USHORT			Aid;
	UCHAR			UserPriority;
	QUEUE_HEADER    tmpQueue, txQueue;
	struct sk_buff	*pSkb;
	UCHAR           QueIdx;
	BOOLEAN			bRemoveTIMBit = FALSE;
	BOOLEAN			bSendNullFrame = FALSE;
	UMAXLONG		IrqFlags;
#ifndef THREAD_ISR
	UMAXLONG		IrqFlags2;
#endif
    BOOLEAN         bKeepTIMBit = FALSE;
    UINT32          apsd_count = 0;
    
    DBGPRINT(RT_DEBUG_TRACE,"Trigger frame(%d) from %02x:%02x:%02x:%02x:%02x:%02x\n",
    	pEntry->PsQueue.Number, pEntry->Addr[0], pEntry->Addr[1], pEntry->Addr[2],
    	pEntry->Addr[3], pEntry->Addr[4], pEntry->Addr[5]);

	Aid = pEntry->Aid;
	
#ifndef THREAD_ISR
    RTMP_SEM_LOCK(&pAd->MacTabLock, IrqFlags);
#else
	// Protect TxSwQueue0 & McastPsQueue because use them in interrupt context 
	RTMP_IRQ_LOCK(IrqFlags);
#endif	/* THREAD_ISR */

    // cleanup all backlogged frames in PSQ
    // PSM change will not be treated as trigger frame

	InitializeQueueHeader(&tmpQueue);
	InitializeQueueHeader(&txQueue);

	// split up into two catagory (delivery-enabled and non-delivery-enabled)
	while (pEntry->PsQueue.Head)
	{
        if ((apsd_count != 0) &&
            (pEntry->MaxSPLength != 0) &&
            (apsd_count >= (pEntry->MaxSPLength << 1)))
        {
            bKeepTIMBit = TRUE;
            break;
        } /* End of if */

		pSkb = (struct sk_buff *) RemoveHeadQueue(&pEntry->PsQueue);

		UserPriority = RTMP_GET_PACKET_UP(pSkb);
		QueIdx = MapUserPriorityToAccessCategory[UserPriority];

		if ((pEntry->bAPSDCapablePerAC[QueIdx]) || (QueIdx == OldUP))
		{
			bRemoveTIMBit = TRUE;
			InsertTailQueue(&txQueue, pSkb);
		}
		else
		{
			InsertTailQueue(&tmpQueue, pSkb);
		}
	}

#ifndef THREAD_ISR
	RTMP_SEM_LOCK(&pAd->TxSwQueueLock, IrqFlags2);
#endif

	if (txQueue.Head == NULL)
	{
		// if no data needs to tx, respond with QosNull for trigger frame
		DBGPRINT(RT_DEBUG_TRACE,"No data, send a Qos-Null frame with ESOP bit on and UP=%d to end USP\n", OldUP);
		bSendNullFrame = TRUE;
	}
	else
	{
		// transmit delivery-enabled packets out 
		while (txQueue.Head)
		{
			pSkb = (struct sk_buff *) RemoveHeadQueue(&txQueue);

			UserPriority = RTMP_GET_PACKET_UP(pSkb);
			QueIdx = MapUserPriorityToAccessCategory[UserPriority];

			if (txQueue.Head)
				RTMP_SET_PACKET_MOREDATA(pSkb, TRUE);
			else
				RTMP_SET_PACKET_MOREDATA(pSkb, FALSE);
			InsertTailQueue(&pAd->TxSwQueue[QueIdx], pSkb);
		}
	}

#ifndef THREAD_ISR
	RTMP_SEM_UNLOCK(&pAd->TxSwQueueLock, IrqFlags2);
#endif

	// re-queue non-delivery-enabled packets
	while (tmpQueue.Head)
	{
		pSkb = (struct sk_buff *) RemoveHeadQueue(&tmpQueue);

		InsertTailQueue(&pEntry->PsQueue, pSkb);
	}
        
	// clear corresponding TIM bit
    if (bKeepTIMBit == FALSE)
    {
	if ((pEntry->bAPSDCapablePerAC[QID_AC_VO] && pEntry->bAPSDCapablePerAC[QID_AC_VI] &&
		pEntry->bAPSDCapablePerAC[QID_AC_BE] && pEntry->bAPSDCapablePerAC[QID_AC_BK]) || (bRemoveTIMBit == TRUE))
	{
		if ((Aid > 0) && (Aid < MAX_LEN_OF_MAC_TABLE) && (pEntry->PsQueue.Number == 0))
	    {
	        if (Aid >= 32)
	            pAd->PortCfg.MBSSID[pEntry->ApIdx].TimBitmap2 &= (~BIT32[Aid-32]);  
	        else
	            pAd->PortCfg.MBSSID[pEntry->ApIdx].TimBitmap &= (~BIT32[Aid]);  
	        pEntry->PsQIdleCount = 0;
	    }
	}
    }

#ifndef THREAD_ISR
    RTMP_SEM_UNLOCK(&pAd->MacTabLock, IrqFlags);
#else
	RTMP_IRQ_UNLOCK(IrqFlags);
#endif	/* THREAD_ISR */

	// it will be crashed, when spin locked in kernel 2.6
	if (bSendNullFrame)
	{
//iverson     
                  if(pEntry->bAPSDFlagEOSPOK != 0){
                  return;
                  }
                  pEntry->bAPSDFlagEOSPOK = 1 ;
//iverson end
		ApEnqueueNullFrame(pAd, pEntry->Addr, pEntry->CurrTxRate, Aid, pEntry->ApIdx, TRUE, TRUE, OldUP);
	}

    // Dequeue outgoing frames from TxSwQueue0..3 queue and process it
    // TODO: 2004-12-27 it's not a good idea to handle "More Data" bit here. because the
    // RTMPDeQueue process doesn't guarantee to de-queue the desired MSDU from the corresponding
    // TxSwQueue/PsQueue when QOS in-used. We should consider "HardTransmt" this MPDU
    // using MGMT queue or things like that.
    RTMPDeQueuePacket(pAd, FALSE, FALSE);
}

/*
    ========================================================================
    Routine Description:
        This function checks if the received frames should be sent out to the 
        wireless media based on DA look-up in the wirelss MAC table.
        
    Arguments:
        pRxD        Pointer to the Rx descriptor
        
    Return Value:
        TRUE -  the input frame is destined to a known wireless STA 
                and had being sent out to the wireless media. It's not 
                necessary to make another copy up to LLC nor Portal function.
        FALSE - the input frame still need to deliver to the upper LLC
    ========================================================================
*/
#ifndef RTL865X_SOC
BOOLEAN RTMPBridgeToWdsAndWirelessSta(
#else
static inline BOOLEAN RTMPBridgeToWdsAndWirelessSta(
#endif
    IN  PRTMP_ADAPTER   pAdapter,
    IN  PUCHAR          pHeader802_3,
    IN  UINT            HdrLen,
    IN  PUCHAR          pData,
    IN  UINT            DataLen,
    IN  UINT            FromWhichBSSID)
{
    SST				Sst;
    USHORT			Aid;
    UCHAR			PsMode, Rate;
    MAC_TABLE_ENTRY	*pEntry, *pSrcEntry;
    BOOLEAN			result = FALSE;
    struct sk_buff *skb;
    UCHAR           SrcAddr[MAC_ADDR_LEN];

    // decide the return value. TRUE if no need to indicate to LLC, FALSE otherwise
    pEntry = SsPsInquiry(pAdapter, pHeader802_3, &Sst, &Aid, &PsMode, &Rate);
    if (pHeader802_3[0] & 0x01)
        result = FALSE;
    else if (pEntry && (Sst == SST_ASSOC)) 
        result = TRUE;
    else
        result = FALSE;
    
    // extract source client's mac address and check whether it has associated with AP
    NdisMoveMemory(SrcAddr, pHeader802_3 + 6, MAC_ADDR_LEN); 
    pSrcEntry = MacTableLookup(pAdapter, SrcAddr);
    
	if(!pSrcEntry)
	{ // WDS packet
		return FALSE;
	}

	// unicast packet
    if ((pHeader802_3[0] & 0x01) != 0x01)
    {
	    if(!pEntry)
	    {// STAs not in wireless
	        return FALSE;
	    }
	    else if (FromWhichBSSID == pEntry->ApIdx)
	    {// STAs in same SSID
	        if (pAdapter->PortCfg.MBSSID[pEntry->ApIdx].IsolateInterStaTraffic == 1)
	        {
                      DBGPRINT(RT_DEBUG_INFO, "IsolateInterStaTraffic: from %02X:%02X:%02X:%02X:%02X:%02X.\n",
                                                        SrcAddr[0],SrcAddr[1],SrcAddr[2],SrcAddr[3],SrcAddr[4],SrcAddr[5]);  
	            return result;
	        }
	        else
	        {   
	        }
	    }
	    else
	    {// STAs in different SSID
	        if (pAdapter->PortCfg.IsolateInterStaTrafficBTNBSSID == 1)
	        {
	            return TRUE;
	        }
	        else
	        {
	        }
	    }
	}

    if ((pHeader802_3[0] & 0x01) || (pEntry && (Sst == SST_ASSOC)))
    {

#ifdef IGMP_SNOOP_SUPPORT
		if ((pHeader802_3[0] & 0x01)
			&& (pAdapter->StaCount[pSrcEntry->ApIdx] <= 1))
			return FALSE;
#endif

        do {
            // 1. build a skb buffer and call RTMPSendPacket();
            //    be careful about how/when to release this internal allocated skb buffer buffer
            if ((skb = __dev_alloc_skb(HdrLen + DataLen + 2, MEM_ALLOC_FLAG)) != NULL)
            {
                skb->len = HdrLen + DataLen;
                memcpy((skb->data), pHeader802_3, HdrLen);
                memcpy((skb->data + HdrLen), pData, DataLen);
            }
            else
            {
                break;
            }
            	
            // 1.1 apidx != 0, then we need set packet mbssid attribute.
			RTMP_SET_PACKET_NET_DEVICE_MBSSID(skb, MAIN_MBSSID);	// set a default value
            if(pEntry && (pEntry->ApIdx != 0))
                RTMP_SET_PACKET_NET_DEVICE_MBSSID(skb, pEntry->ApIdx);
			/* send bc/mc frame back to the same bss */
			if (!pEntry)
				RTMP_SET_PACKET_NET_DEVICE_MBSSID(skb, FromWhichBSSID);
            RTMP_SET_PACKET_SOURCE(skb, PKTSRC_DRIVER);
            RTMP_SET_PACKET_MOREDATA(skb, FALSE);
            	
            // 2. send out the packet
            DBGPRINT(RT_DEBUG_INFO,"WBRG - len=%d, DA=%02x:%02x:%02x:%02x:%02x:%02x, SA=%02x:%02x:%02x:%02x:%02x:%02x\n", 
                HdrLen+DataLen,pHeader802_3[0],pHeader802_3[1],pHeader802_3[2],pHeader802_3[3],pHeader802_3[4],pHeader802_3[5],
                pHeader802_3[6],pHeader802_3[7],pHeader802_3[8],pHeader802_3[9],pHeader802_3[10],pHeader802_3[11]);

#ifdef RTL865X_FAST_PATH
			/* indicate this packet is from RT61 for buffer free usage */
		    RTL865X_SET_PKTTYPE(skb, RTL865X_TYPE_RT61);
#endif
            RTMPSendPacket(pAdapter, skb);
        } while (FALSE);
    }
    RTMPDeQueuePacket(pAdapter, FALSE, FALSE);

    return result;
}

/*
	========================================================================

	Routine	Description:
		Check the out going frame, if this is an DHCP or ARP datagram
	will be duplicate another frame at low data rate transmit.
		
	Arguments:
		pAd			Pointer	to our adapter
		pSkb		Pointer to outgoing skb buffer
		
	Return Value:		
		TRUE		To be transmitted at Low data rate transmit. (1Mbps/6Mbps)
		FALSE		Do nothing.

	Note:

		MAC header + IP Header + UDP Header
		  14 Bytes    20 Bytes
		  
		UDP Header
		00|01|02|03|04|05|06|07|08|09|10|11|12|13|14|15|
						Source Port
		16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|
					Destination Port

		port 0x43 means Bootstrap Protocol, server. 
		Port 0x44 means Bootstrap Protocol, client. 

	========================================================================
*/
#ifdef RTL865X_SOC
static inline BOOLEAN RTMPCheckDHCPFrame(
#else
BOOLEAN RTMPCheckDHCPFrame(
#endif
	IN	PRTMP_ADAPTER	pAd, 
	IN  struct sk_buff	*pSkb)
{
	PUCHAR			pSrc;
	ULONG			SrcLen = 0;

	pSrc = (PVOID)pSkb->data;
    SrcLen = pSkb->len;

	// Check ARP packet
    if (SrcLen >= 13)
    {
    	if ((pSrc[12] == 0x08) && (pSrc[13] == 0x06))
    	{
    		DBGPRINT(RT_DEBUG_INFO,"RTMPCheckDHCPFrame - ARP packet\n");
    		return TRUE;
    	}
    }

	// Check foe DHCP & BOOTP protocol
	if (SrcLen >= 37)
	{
		if ((pSrc[35] == 0x43) && (pSrc[37] == 0x44))
		{
			DBGPRINT(RT_DEBUG_INFO,"RTMPCheckDHCPFrame - DHCP packet\n");
			return TRUE;
		}
	}

	return FALSE;
}

#ifdef BIG_ENDIAN
#ifdef RTL865X_SOC
//extern unsigned int mylinkid;	/* Remember your link id */
__IRAM static void rtl865x_rx(PRTMP_ADAPTER pAd, struct sk_buff *skb)
{
	UINT32 	retval;
	INT		index;
	unsigned int mylinkid;

#if 0	
{
	int i;
	for (i=0; i<32; i++) {
		printk("%02x ", skb->data[i]);
		if (i%16 == 15) printk("\n");
	}
	printk("\n");
}
#endif
	for(index = 0; index < pAd->PortCfg.BssidNum; index++)
	{
		if (pAd->PortCfg.MBSSID[index].MSSIDDev == skb->dev) 
		{
			mylinkid = pAd->PortCfg.MBSSID[index].mylinkid;
			retval = rtl8651_fwdEngineExtPortRecv(skb, skb->data, skb->len,
					CONFIG_8139CP_VID, 1<<CONFIG_RTL865XB_WLAN1_PORT, mylinkid);
			if (retval != SUCCESS) 
			{
				/* exception. Drop it */
				dev_kfree_skb_irq(skb);
			}
			return;
		}							
	}

#ifdef APCLI_SUPPORT
	for (index = 0; index < MAX_APCLI_ENTRY; index++)
	{
		if ((pAd->ApCliTab.ApCliEntry[index].Valid == TRUE)
			&&(pAd->ApCliTab.ApCliEntry[index].Enable == TRUE)
			&& (pAd->ApCliTab.ApCliEntry[index].dev == skb->dev))
		{
			mylinkid = pAd->ApCliTab.ApCliEntry[index].mylinkid;
			retval = rtl8651_fwdEngineExtPortRecv(skb, skb->data, skb->len,
					CONFIG_8139CP_VID, 1<<CONFIG_RTL865XB_WLAN1_PORT, mylinkid);
			if (retval != SUCCESS) 
			{
				/* exception. Drop it */
				dev_kfree_skb_irq(skb);
			}
			return;
		}
	}							
#endif

	if (pAd->WdsTab.Mode != WDS_DISABLE_MODE) {
	    for (index = 0; index < MAX_WDS_ENTRY; index++)
	    {
	        if ((pAd->WdsTab.WdsEntry[index].Valid == TRUE) && (pAd->WdsTab.WdsEntry[index].dev == skb->dev))
	        {
				mylinkid = pAd->WdsTab.WdsEntry[index].mylinkid;
				retval = rtl8651_fwdEngineExtPortRecv(skb, skb->data, skb->len,
						CONFIG_8139CP_VID, 1<<CONFIG_RTL865XB_WLAN1_PORT, mylinkid);
				if (retval != SUCCESS) 
				{
					/* exception. Drop it */
					dev_kfree_skb_irq(skb);
				}
				return;
	        }
		}							
	}
}
#endif /* RTL865X_SOC */
#endif	//#ifdef BIG_ENDIAN

//iverson
//================================================================================
//
//
//
//================================================================================
VOID UAPSD_QoSNullTxDoneHandle(
        IN PRTMP_ADAPTER pAd,
        IN struct sk_buff *pSkb,
        IN UCHAR *dst_mac_p)
{
    HEADER_802_11 *header_p;
    MAC_TABLE_ENTRY *entry_p;

// sanity check
    if(pSkb == NULL)
             return;

//check if the packet sub type is QOS Null    
    if(dst_mac_p == NULL)
             return;

    header_p = (HEADER_802_11 *)dst_mac_p;
		/* carella modify: UAPSD reply packet use MGMT Queue to transmit Data(QoS Null) Frame.
		 * If don't check Frame Type of packet, will be confused with Mgmt packet(Deauthentication).
		 */
		if(header_p->FC.Type != BTYPE_DATA) //<--carella 2007/07/16
				return; //-->carella

    entry_p = MacTableLookup(pAd, header_p->Addr1);

    if((entry_p != NULL) && (header_p->FC.SubType == SUBTYPE_QOS_NULL))
    {
        entry_p->bAPSDFlagEOSPOK = 0 ;
//        printk("iverson success \n");
    }


}
//iverson end


