#include "rt_config.h"

ULONG	RTDebugLevel = RT_DEBUG_ERROR;

#ifdef WIRELESS_EVENT_SUPPORT
// for wireless system event message
char const *pWirelessSysEventText[IW_SYS_EVENT_TYPE_NUM] = {    
	// system status event     
    "had associated successfully",							/* IW_ASSOC_EVENT_FLAG */
    "had disassociated",									/* IW_DISASSOC_EVENT_FLAG */
    "had deauthenticated",									/* IW_DEAUTH_EVENT_FLAG */
    "had been aged-out and disassociated",					/* IW_AGEOUT_EVENT_FLAG */
    "occurred CounterMeasures attack",						/* IW_COUNTER_MEASURES_EVENT_FLAG */	
    "occurred replay counter different in Key Handshaking",	/* IW_REPLAY_COUNTER_DIFF_EVENT_FLAG */
    "occurred RSNIE different in Key Handshaking",			/* IW_RSNIE_DIFF_EVENT_FLAG */
    "occurred MIC different in Key Handshaking",			/* IW_MIC_DIFF_EVENT_FLAG */
    "occurred ICV error in RX",								/* IW_ICV_ERROR_EVENT_FLAG */
    "occurred MIC error in RX",								/* IW_MIC_ERROR_EVENT_FLAG */
	"Group Key Handshaking timeout",						/* IW_GROUP_HS_TIMEOUT_EVENT_FLAG */ 
	"Pairwise Key Handshaking timeout",						/* IW_PAIRWISE_HS_TIMEOUT_EVENT_FLAG */ 
	"RSN IE sanity check failure",							/* IW_RSNIE_SANITY_FAIL_EVENT_FLAG */ 
	"set key done in WPA/WPAPSK",							/* IW_SET_KEY_DONE_WPA1_EVENT_FLAG */ 
	"set key done in WPA2/WPA2PSK",                         /* IW_SET_KEY_DONE_WPA2_EVENT_FLAG */ 
	"connects with our wireless client",                    /* IW_STA_LINKUP_EVENT_FLAG */ 
	"disconnects with our wireless client",                 /* IW_STA_LINKDOWN_EVENT_FLAG */
	"scan completed",										/* IW_SCAN_COMPLETED_EVENT_FLAG */
	"scan terminate!! Busy!! Enqueue fail!!",				/* IW_SCAN_ENQUEUE_FAIL_EVENT_FLAG */
	"channel switch to ",									/* IW_CHANNEL_CHANGE_EVENT_FLAG */
	"wireless mode is not support",							/* IW_STA_MODE_EVENT_FLAG */
	"blacklisted in MAC filter list",						/* IW_MAC_FILTER_LIST_EVENT_FLAG */
	"Authentication rejected because of challenge failure"	/* IW_AUTH_REJECT_CHALLENGE_FAILURE */
	};
#endif // WIRELESS_EVENT_SUPPORT //

#ifdef IDS_SUPPORT
// for wireless IDS_spoof_attack event message
char const *pWirelessSpoofEventText[IW_SPOOF_EVENT_TYPE_NUM] = {   	
    "detected conflict SSID",								/* IW_CONFLICT_SSID_EVENT_FLAG */
    "detected spoofed association response",				/* IW_SPOOF_ASSOC_RESP_EVENT_FLAG */
    "detected spoofed reassociation responses",				/* IW_SPOOF_REASSOC_RESP_EVENT_FLAG */
    "detected spoofed probe response",						/* IW_SPOOF_PROBE_RESP_EVENT_FLAG */
    "detected spoofed beacon",								/* IW_SPOOF_BEACON_EVENT_FLAG */
    "detected spoofed disassociation",						/* IW_SPOOF_DISASSOC_EVENT_FLAG */
    "detected spoofed authentication",						/* IW_SPOOF_AUTH_EVENT_FLAG */
    "detected spoofed deauthentication",					/* IW_SPOOF_DEAUTH_EVENT_FLAG */
    "detected spoofed unknown management frame",			/* IW_SPOOF_UNKNOWN_MGMT_EVENT_FLAG */
	"detected replay attack"								/* IW_REPLAY_ATTACK_EVENT_FLAG */	
	};

// for wireless IDS_flooding_attack event message
char const *pWirelessFloodEventText[IW_FLOOD_EVENT_TYPE_NUM] = {   	
	"detected authentication flooding",						/* IW_FLOOD_AUTH_EVENT_FLAG */
    "detected association request flooding",				/* IW_FLOOD_ASSOC_REQ_EVENT_FLAG */
    "detected reassociation request flooding",				/* IW_FLOOD_REASSOC_REQ_EVENT_FLAG */
    "detected probe request flooding",						/* IW_FLOOD_PROBE_REQ_EVENT_FLAG */
    "detected disassociation flooding",						/* IW_FLOOD_DISASSOC_EVENT_FLAG */
    "detected deauthentication flooding",					/* IW_FLOOD_DEAUTH_EVENT_FLAG */
    "detected 802.1x eap-request flooding"					/* IW_FLOOD_EAP_REQ_EVENT_FLAG */	
	};
#endif // IDS_SUPPORT //
#ifdef WSC_INCLUDED
// for WSC wireless event message
char const *pWirelessWscEventText[IW_WSC_EVENT_TYPE_NUM] = {   	
	"PBC Session Overlap",									/* IW_WSC_PBC_SESSION_OVERLAP */
	"This WPS Registrar supports PBC",						/* IW_WSC_REGISTRAR_SUPPORT_PBC */
	"This WPS Registrar supports PIN",						/* IW_WSC_REGISTRAR_SUPPORT_PIN */
	"WPS status success",									/* IW_WSC_STATUS_SUCCESS */
	"WPS status fail",										/* IW_WSC_STATUS_FAIL */
	"WPS 2 mins time out!",									/* IW_WSC_2MINS_TIMEOUT */
	};
#endif // WSC_INCLUDED //

/* timeout -- ms */
VOID RTMP_SetPeriodicTimer(
	IN	NDIS_MINIPORT_TIMER *pTimer, 
	IN	unsigned long timeout)
{
	timeout = ((timeout*OS_HZ) / 1000);
	pTimer->expires = jiffies + timeout;
	add_timer(pTimer);
}

/* convert NdisMInitializeTimer --> RTMP_OS_Init_Timer */
VOID RTMP_OS_Init_Timer(
	IN	PRTMP_ADAPTER pAd,
	IN	NDIS_MINIPORT_TIMER *pTimer, 
	IN	TIMER_FUNCTION function,
	IN	PVOID data)
{
	if (!timer_pending(pTimer))
	{
		init_timer(pTimer);
		pTimer->data = (unsigned long)data;
		pTimer->function = function;		
	}
}


VOID RTMP_OS_Add_Timer(
	IN	NDIS_MINIPORT_TIMER		*pTimer,
	IN	unsigned long timeout)
{
	if (timer_pending(pTimer))
		return;

	timeout = ((timeout*OS_HZ) / 1000);
	pTimer->expires = jiffies + timeout;
	add_timer(pTimer);
}

VOID RTMP_OS_Mod_Timer(
	IN	NDIS_MINIPORT_TIMER		*pTimer,
	IN	unsigned long timeout)
{
	timeout = ((timeout*OS_HZ) / 1000);
	mod_timer(pTimer, jiffies + timeout);
}

VOID RTMP_OS_Del_Timer(
	IN	NDIS_MINIPORT_TIMER		*pTimer,
	OUT	BOOLEAN					*pCancelled)
{
	if (timer_pending(pTimer))
	{	
		*pCancelled = del_timer_sync(pTimer);	
	}
	else
	{
		*pCancelled = TRUE;
	}
	
}

	
// Unify all delay routine by using udelay
VOID RTMPusecDelay(
	IN	ULONG	usec)
{
	ULONG	i;

	for (i = 0; i < (usec / 50); i++)
		udelay(50);

	if (usec % 50)
		udelay(usec % 50);
}

void RTMP_GetCurrentSystemTime(LARGE_INTEGER *time)
{
	time->u.LowPart = jiffies;
}

// pAd MUST allow to be NULL
NDIS_STATUS os_alloc_mem(
	IN	RTMP_ADAPTER *pAd,
	OUT	UCHAR **mem,
	IN	ULONG  size)
{	
	*mem = (PUCHAR) kmalloc(size, GFP_ATOMIC);
	if (*mem)
		return (NDIS_STATUS_SUCCESS);
	else
		return (NDIS_STATUS_FAILURE);
}

// pAd MUST allow to be NULL
NDIS_STATUS os_free_mem(
	IN	PRTMP_ADAPTER pAd,
	IN	PVOID mem)
{
	
	ASSERT(mem);
	kfree(mem);
	return (NDIS_STATUS_SUCCESS);
}




PNDIS_PACKET RtmpOSNetPktAlloc(
	IN RTMP_ADAPTER *pAd, 
	IN int size)
{
	struct sk_buff *skb;
	/* Add 2 more bytes for ip header alignment*/
	
	skb = dev_alloc_skb(size+2);

	return ((PNDIS_PACKET)skb);
}


PNDIS_PACKET RTMP_AllocateFragPacketBuffer(
	IN	PRTMP_ADAPTER pAd,
	IN	ULONG	Length)
{
	struct sk_buff *pkt;
	
	pkt = dev_alloc_skb(Length);

	if (pkt == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("can't allocate frag rx %ld size packet\n",Length));
	}

	if (pkt)
	{
		RTMP_SET_PACKET_SOURCE(OSPKT_TO_RTPKT(pkt), PKTSRC_NDIS);
	}

	return (PNDIS_PACKET) pkt;
}


VOID	RTMPFreeAdapter(
	IN	PRTMP_ADAPTER	pAd)
{
	POS_COOKIE os_cookie;
	int index;	

	os_cookie=(POS_COOKIE)pAd->OS_Cookie;



	NdisFreeSpinLock(&pAd->MgmtRingLock);
	
#ifdef RTMP_MAC_PCI 
	NdisFreeSpinLock(&pAd->RxRingLock);
#if defined(RT3090) || defined(RT3390)
NdisFreeSpinLock(&pAd->McuCmdLock);
#endif // RT3090 //
#endif // RTMP_MAC_PCI //

	for (index =0 ; index < NUM_OF_TX_RING; index++)
	{
		NdisFreeSpinLock(&pAd->TxSwQueueLock[index]);
		NdisFreeSpinLock(&pAd->DeQueueLock[index]);
		pAd->DeQueueRunning[index] = FALSE;
	}
	
	NdisFreeSpinLock(&pAd->irq_lock);

#ifdef CONFIG_AP_SUPPORT
#ifdef UAPSD_AP_SUPPORT
	UAPSD_Release(pAd);
#endif // UAPSD_AP_SUPPORT //
#endif // CONFIG_AP_SUPPORT //

	vfree(pAd); // pci_free_consistent(os_cookie->pci_dev,sizeof(RTMP_ADAPTER),pAd,os_cookie->pAd_pa);
	if (os_cookie)
		kfree(os_cookie);
}


// the allocated NDIS PACKET must be freed via RTMPFreeNdisPacket()
NDIS_STATUS RTMPAllocateNdisPacket(
	IN	PRTMP_ADAPTER	pAd,
	OUT PNDIS_PACKET   *ppPacket,
	IN	PUCHAR			pHeader,
	IN	UINT			HeaderLen,
	IN	PUCHAR			pData,
	IN	UINT			DataLen)
{
	PNDIS_PACKET	pPacket;
	ASSERT(pData);
	ASSERT(DataLen);

	// 1. Allocate a packet 
	pPacket = (PNDIS_PACKET *) dev_alloc_skb(HeaderLen + DataLen + RTMP_PKT_TAIL_PADDING);
	if (pPacket == NULL)
 	{
		*ppPacket = NULL;
#ifdef DEBUG
		printk("RTMPAllocateNdisPacket Fail\n\n");
#endif
		return NDIS_STATUS_FAILURE;
	}

	// 2. clone the frame content
	if (HeaderLen > 0)
		NdisMoveMemory(GET_OS_PKT_DATAPTR(pPacket), pHeader, HeaderLen);
	if (DataLen > 0)
		NdisMoveMemory(GET_OS_PKT_DATAPTR(pPacket) + HeaderLen, pData, DataLen);

	// 3. update length of packet
 	skb_put(GET_OS_PKT_TYPE(pPacket), HeaderLen+DataLen);

	RTMP_SET_PACKET_SOURCE(pPacket, PKTSRC_NDIS);
//	printk("%s : pPacket = %p, len = %d\n", __FUNCTION__, pPacket, GET_OS_PKT_LEN(pPacket));
	*ppPacket = pPacket;
	return NDIS_STATUS_SUCCESS;
}

/*
  ========================================================================
  Description:
	This routine frees a miniport internally allocated NDIS_PACKET and its
	corresponding NDIS_BUFFER and allocated memory.
  ========================================================================
*/
VOID RTMPFreeNdisPacket(
	IN PRTMP_ADAPTER pAd,
	IN PNDIS_PACKET  pPacket)
{
	dev_kfree_skb_any(RTPKT_TO_OSPKT(pPacket));
}


// IRQL = DISPATCH_LEVEL
// NOTE: we do have an assumption here, that Byte0 and Byte1 always reasid at the same 
//			 scatter gather buffer
NDIS_STATUS Sniff2BytesFromNdisBuffer(
	IN	PNDIS_BUFFER	pFirstBuffer,
	IN	UCHAR			DesiredOffset,
	OUT PUCHAR			pByte0,
	OUT PUCHAR			pByte1)
{
    *pByte0 = *(PUCHAR)(pFirstBuffer + DesiredOffset);
    *pByte1 = *(PUCHAR)(pFirstBuffer + DesiredOffset + 1);

	return NDIS_STATUS_SUCCESS;
}


void RTMP_QueryPacketInfo(
	IN  PNDIS_PACKET pPacket,
	OUT PACKET_INFO  *pPacketInfo,
	OUT PUCHAR		 *pSrcBufVA,
	OUT	UINT		 *pSrcBufLen)
{
	pPacketInfo->BufferCount = 1;
	pPacketInfo->pFirstBuffer = (PNDIS_BUFFER)GET_OS_PKT_DATAPTR(pPacket);
	pPacketInfo->PhysicalBufferCount = 1;
	pPacketInfo->TotalPacketLength = GET_OS_PKT_LEN(pPacket);

	*pSrcBufVA = GET_OS_PKT_DATAPTR(pPacket);
	*pSrcBufLen = GET_OS_PKT_LEN(pPacket); 	
}
	
PNDIS_PACKET DuplicatePacket(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PNDIS_PACKET	pPacket,
	IN	UCHAR			FromWhichBSSID)
{
	struct sk_buff	*skb;
	PNDIS_PACKET	pRetPacket = NULL;
	USHORT			DataSize;
	UCHAR			*pData;

	DataSize = (USHORT) GET_OS_PKT_LEN(pPacket);
	pData = (PUCHAR) GET_OS_PKT_DATAPTR(pPacket);	


	skb = skb_clone(RTPKT_TO_OSPKT(pPacket), MEM_ALLOC_FLAG);
	if (skb)
	{
		skb->dev = get_netdev_from_bssid(pAd, FromWhichBSSID);
		pRetPacket = OSPKT_TO_RTPKT(skb);
	}


	return pRetPacket;

}

PNDIS_PACKET duplicate_pkt(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			pHeader802_3,
    IN  UINT            HdrLen,
	IN	PUCHAR			pData,
	IN	ULONG			DataSize,
	IN	UCHAR			FromWhichBSSID)
{
	struct sk_buff	*skb;
	PNDIS_PACKET	pPacket = NULL;


	if ((skb = __dev_alloc_skb(HdrLen + DataSize + 2, MEM_ALLOC_FLAG)) != NULL)
	{
		skb_reserve(skb, 2);				
		NdisMoveMemory(skb->tail, pHeader802_3, HdrLen);
		skb_put(skb, HdrLen);
		NdisMoveMemory(skb->tail, pData, DataSize);
		skb_put(skb, DataSize);
		skb->dev = get_netdev_from_bssid(pAd, FromWhichBSSID);
		pPacket = OSPKT_TO_RTPKT(skb);
	}

	return pPacket;
}


#define TKIP_TX_MIC_SIZE		8
PNDIS_PACKET duplicate_pkt_with_TKIP_MIC(
	IN	PRTMP_ADAPTER	pAd,
	IN	PNDIS_PACKET	pPacket)
{
	struct sk_buff	*skb, *newskb;
	

	skb = RTPKT_TO_OSPKT(pPacket);
	if (skb_tailroom(skb) < TKIP_TX_MIC_SIZE)
	{
		// alloc a new skb and copy the packet
		newskb = skb_copy_expand(skb, skb_headroom(skb), TKIP_TX_MIC_SIZE, GFP_ATOMIC);
		dev_kfree_skb_any(skb);
		if (newskb == NULL)
		{
			DBGPRINT(RT_DEBUG_ERROR, ("Extend Tx.MIC for packet failed!, dropping packet!\n"));
			return NULL;
		}
		skb = newskb;
	}

	return OSPKT_TO_RTPKT(skb);


}


#ifdef CONFIG_AP_SUPPORT
PNDIS_PACKET duplicate_pkt_with_VLAN(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			pHeader802_3,
    IN  UINT            HdrLen,
	IN	PUCHAR			pData,
	IN	ULONG			DataSize,
	IN	UCHAR			FromWhichBSSID)
{
	struct sk_buff	*skb;
	PNDIS_PACKET	pPacket = NULL;
	UINT16			VLAN_Size;


	if ((skb = __dev_alloc_skb(HdrLen + DataSize + LENGTH_802_1Q + 2, \
								MEM_ALLOC_FLAG)) != NULL)
	{
		skb_reserve(skb, 2);

		/* copy header (maybe +VLAN tag) */
		VLAN_Size = VLAN_8023_Header_Copy(pAd, pHeader802_3, HdrLen,
											skb->tail, FromWhichBSSID);
		skb_put(skb, HdrLen + VLAN_Size);

		/* copy data body */
		NdisMoveMemory(skb->tail, pData, DataSize);
		skb_put(skb, DataSize);
		skb->dev = get_netdev_from_bssid(pAd, FromWhichBSSID);
		pPacket = OSPKT_TO_RTPKT(skb);
	} /* End of if */

	return pPacket;
} /* End of duplicate_pkt_with_VLAN */
#endif // CONFIG_AP_SUPPORT //

#ifdef WAPI_SUPPORT
PNDIS_PACKET duplicate_pkt_with_WPI(
	IN	PRTMP_ADAPTER	pAd,
	IN	PNDIS_PACKET	pPacket,
	IN	UINT32			ext_head_len,
	IN	UINT32			ext_tail_len)
{
	struct sk_buff	*skb, *newskb;
	

	skb = RTPKT_TO_OSPKT(pPacket);
	if (skb_cloned(skb) || (skb_headroom(skb) < ext_head_len) || (skb_tailroom(skb) < ext_tail_len))
	{
		UINT32 head_len = (skb_headroom(skb) < ext_head_len) ? ext_head_len : skb_headroom(skb);
		UINT32 tail_len = (skb_tailroom(skb) < ext_tail_len) ? ext_tail_len : skb_tailroom(skb);
	
		// alloc a new skb and copy the packet
		newskb = skb_copy_expand(skb, head_len, tail_len, GFP_ATOMIC);
		dev_kfree_skb_any(skb);
		if (newskb == NULL)
		{
			DBGPRINT(RT_DEBUG_ERROR, ("Extend Tx buffer for WPI failed!, dropping packet!\n"));
			return NULL;
		}
		skb = newskb;
	}

	return OSPKT_TO_RTPKT(skb);
	
}

/*
	========================================================================
	
	Routine Description:
		Send a L2 frame to wapi daemon to trigger WAPI state machine

	Arguments:		
		pAd			-	pointer to our pAdapter context	
  				
	Return Value:
		
	Note:
		
	========================================================================
*/
BOOLEAN SendL2WapiTriggerFrame(
		IN  PRTMP_ADAPTER		pAd,
		IN	UCHAR				AuthMode,
		IN	UCHAR				apidx,
		IN	PUCHAR				pAddr,
		IN	UCHAR				flag)
{												   
    if ((AuthMode == Ndis802_11AuthModeWAICERT) || 
		(AuthMode == Ndis802_11AuthModeWAIPSK))
	{				
		UCHAR 			Header802_3[14];
		UCHAR			WAPI_IE[] = {0x88, 0xb4};
		UCHAR 			wapi_hdr[12];		
		UCHAR			data_len = 0;
		INT total_size = sizeof(Header802_3) + sizeof(wapi_hdr);
		struct sk_buff *skb = dev_alloc_skb(total_size+2);

		if (!skb)
		{
			DBGPRINT(RT_DEBUG_ERROR, ("Error! Can't allocate a skb.\n"));
			return FALSE;
		}

#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
			MAKE_802_3_HEADER(Header802_3, pAd->ApCfg.MBSSID[apidx].Bssid, pAddr, WAPI_IE); 
		}
#endif // CONFIG_AP_SUPPORT //
		
	   	GET_OS_PKT_NETDEV(skb) = get_netdev_from_bssid(pAd, apidx);
					    	
		skb_reserve(skb, 2);	// 16 byte align the IP header
		NdisMoveMemory(GET_OS_PKT_DATAPTR(skb), Header802_3, LENGTH_802_3);	
		data_len = LENGTH_802_3;

		// Prepare the specific WAPI header
		NdisZeroMemory(wapi_hdr, sizeof(wapi_hdr));
		NdisMoveMemory(wapi_hdr, RALINK_OUI, 3);

		// Set the state of this command
		wapi_hdr[3] = flag;


		// append proprietary header
		NdisMoveMemory(GET_OS_PKT_DATAPTR(skb) + data_len, wapi_hdr, sizeof(wapi_hdr));	
		data_len += sizeof(wapi_hdr);


		// end this frame
		skb_put(GET_OS_PKT_TYPE(skb), data_len);		        

		DBGPRINT(RT_DEBUG_TRACE, ("Trigger WAPI for this sta(%02x:%02x:%02x:%02x:%02x:%02x)\n", PRINT_MAC(pAddr)));

	    // UCOS: update the built-in bridge, too (don't use gmac.xmit())
	    announce_802_3_packet(pAd, skb);

	}	

	return TRUE;
}	


#endif // WAPI_SUPPORT //

PNDIS_PACKET ExpandPacket(
	IN	PRTMP_ADAPTER	pAd,
	IN	PNDIS_PACKET	pPacket,
	IN	UINT32			ext_head_len,
	IN	UINT32			ext_tail_len)
{
	struct sk_buff	*skb, *newskb;
	

	skb = RTPKT_TO_OSPKT(pPacket);
	//printk("original skb_headroom(%d)/skb_tailroom(%d)\n", skb_headroom(skb), skb_tailroom(skb));
	if (skb_cloned(skb) || (skb_headroom(skb) < ext_head_len) || (skb_tailroom(skb) < ext_tail_len))
	{
		UINT32 head_len = (skb_headroom(skb) < ext_head_len) ? ext_head_len : skb_headroom(skb);
		UINT32 tail_len = (skb_tailroom(skb) < ext_tail_len) ? ext_tail_len : skb_tailroom(skb);
	
		// alloc a new skb and copy the packet
		newskb = skb_copy_expand(skb, head_len, tail_len, GFP_ATOMIC);
		dev_kfree_skb_any(skb);
		if (newskb == NULL)
		{
			DBGPRINT(RT_DEBUG_ERROR, ("Extend Tx buffer for WPI failed!, dropping packet!\n"));
			return NULL;
		}
		skb = newskb;
		//printk("new skb_headroom(%d)/skb_tailroom(%d)\n", skb_headroom(skb), skb_tailroom(skb));
	}

	return OSPKT_TO_RTPKT(skb);
	
}

PNDIS_PACKET ClonePacket(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PNDIS_PACKET	pPacket,	
	IN	PUCHAR			pData,
	IN	ULONG			DataSize)
{
	struct sk_buff	*pRxPkt;
	struct sk_buff	*pClonedPkt;

	ASSERT(pPacket);
	pRxPkt = RTPKT_TO_OSPKT(pPacket);

	// clone the packet 
	pClonedPkt = skb_clone(pRxPkt, MEM_ALLOC_FLAG);

	if (pClonedPkt)
	{
    	// set the correct dataptr and data len
    	pClonedPkt->dev = pRxPkt->dev;
    	pClonedPkt->data = pData;
    	pClonedPkt->len = DataSize;
    	pClonedPkt->tail = pClonedPkt->data + pClonedPkt->len;
		ASSERT(DataSize < 1530);
	}
	return pClonedPkt;
}

// 
// change OS packet DataPtr and DataLen
// 
void  update_os_packet_info(
	IN	PRTMP_ADAPTER	pAd, 
	IN	RX_BLK			*pRxBlk,
	IN  UCHAR			FromWhichBSSID)
{
	struct sk_buff	*pOSPkt;

	ASSERT(pRxBlk->pRxPacket);
	pOSPkt = RTPKT_TO_OSPKT(pRxBlk->pRxPacket);

	pOSPkt->dev = get_netdev_from_bssid(pAd, FromWhichBSSID); 
	pOSPkt->data = pRxBlk->pData;
	pOSPkt->len = pRxBlk->DataSize;
	pOSPkt->tail = pOSPkt->data + pOSPkt->len;
}


void wlan_802_11_to_802_3_packet(
	IN	PRTMP_ADAPTER	pAd, 
	IN	RX_BLK			*pRxBlk,
	IN	PUCHAR			pHeader802_3,
	IN  UCHAR			FromWhichBSSID)
{
	struct sk_buff	*pOSPkt;

	ASSERT(pRxBlk->pRxPacket);
	ASSERT(pHeader802_3);

	pOSPkt = RTPKT_TO_OSPKT(pRxBlk->pRxPacket);

	pOSPkt->dev = get_netdev_from_bssid(pAd, FromWhichBSSID); 
	pOSPkt->data = pRxBlk->pData;
	pOSPkt->len = pRxBlk->DataSize;
	pOSPkt->tail = pOSPkt->data + pOSPkt->len;

	//
	// copy 802.3 header
	// 
	// 
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		/* maybe insert VLAN tag to the received packet */
		UCHAR VLAN_Size = 0;
		UCHAR *data_p;

		/* VLAN related */
		if ((FromWhichBSSID < pAd->ApCfg.BssidNum) &&
			(pAd->ApCfg.MBSSID[FromWhichBSSID].VLAN_VID != 0))
		{
			VLAN_Size = LENGTH_802_1Q;
		} /* End of if */

		data_p = skb_push(pOSPkt, LENGTH_802_3+VLAN_Size);

		VLAN_8023_Header_Copy(pAd, pHeader802_3, LENGTH_802_3,
								data_p, FromWhichBSSID);
	}
#endif // CONFIG_AP_SUPPORT //

	}



__IMEM void announce_802_3_packet(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PNDIS_PACKET	pPacket)
{

	struct sk_buff	*pRxPkt;
#ifdef INF_AMAZON_PPA
        int             ret = 0;
        unsigned int ppa_flags = 0; /* reserved for now */
#endif // INF_AMAZON_PPA //

	ASSERT(pPacket);

	pRxPkt = RTPKT_TO_OSPKT(pPacket);
#ifdef CONFIG_AP_SUPPORT
#ifdef APCLI_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		if (MATPktRxNeedConvert(pAd, pRxPkt->dev))
			MATEngineRxHandle(pAd, pPacket, 0);
	}
#endif // APCLI_SUPPORT //
#endif // CONFIG_AP_SUPPORT //


    /* Push up the protocol stack */
#ifdef IKANOS_VX_1X0
	IKANOS_DataFrameRx(pAd, pRxPkt->dev, pRxPkt, pRxPkt->len);
#else

// mark for bridge fast path, 2009/06/22
//	pRxPkt->protocol = eth_type_trans(pRxPkt, pRxPkt->dev);

#ifdef INF_AMAZON_PPA
	if (ppa_hook_directpath_send_fn && pAd->PPAEnable==TRUE ) 
	{
		pRxPkt->protocol = eth_type_trans(pRxPkt, pRxPkt->dev);

		memset(pRxPkt->head,0,pRxPkt->data-pRxPkt->head-14);
		DBGPRINT(RT_DEBUG_TRACE, ("ppa_hook_directpath_send_fn rx :ret:%d headroom:%d dev:%s pktlen:%d<===\n",ret,skb_headroom(pRxPkt)
			,pRxPkt->dev->name,pRxPkt->len));
		hex_dump("rx packet", pRxPkt->data, 32);
		ret = ppa_hook_directpath_send_fn(pAd->g_if_id, pRxPkt, pRxPkt->len, ppa_flags);
		pRxPkt=NULL;
		return;

	}	  	
#endif // INF_AMAZON_PPA //

//#ifdef CONFIG_5VT_ENHANCE
//	*(int*)(pRxPkt->cb) = BRIDGE_TAG; 
//#endif

	{
#ifdef CONFIG_AP_SUPPORT
#ifdef BG_FT_SUPPORT
		if (BG_FTPH_PacketFromApHandle(pRxPkt) == 0)
			return;
#endif // BG_FT_SUPPORT //
#endif // CONFIG_AP_SUPPORT //

		pRxPkt->protocol = eth_type_trans(pRxPkt, pRxPkt->dev);
		netif_rx(pRxPkt);
	}

#endif // IKANOS_VX_1X0 //
}


PRTMP_SCATTER_GATHER_LIST
rt_get_sg_list_from_packet(PNDIS_PACKET pPacket, RTMP_SCATTER_GATHER_LIST *sg)
{
	sg->NumberOfElements = 1;
	sg->Elements[0].Address =  GET_OS_PKT_DATAPTR(pPacket);	
	sg->Elements[0].Length = GET_OS_PKT_LEN(pPacket);	
	return (sg);
}

void hex_dump(char *str, unsigned char *pSrcBufVA, unsigned int SrcBufLen)
{
	unsigned char *pt;
	int x;

	if (RTDebugLevel < RT_DEBUG_TRACE)
		return;
	
	pt = pSrcBufVA;
	printk("%s: %p, len = %d\n",str,  pSrcBufVA, SrcBufLen);
	for (x=0; x<SrcBufLen; x++)
	{
		if (x % 16 == 0) 
			printk("0x%04x : ", x);
		printk("%02x ", ((unsigned char)pt[x]));
		if (x%16 == 15) printk("\n");
	}
	printk("\n");
}

/*
	========================================================================
	
	Routine Description:
		Send log message through wireless event

		Support standard iw_event with IWEVCUSTOM. It is used below.

		iwreq_data.data.flags is used to store event_flag that is defined by user. 
		iwreq_data.data.length is the length of the event log.

		The format of the event log is composed of the entry's MAC address and
		the desired log message (refer to pWirelessEventText).

			ex: 11:22:33:44:55:66 has associated successfully

		p.s. The requirement of Wireless Extension is v15 or newer. 

	========================================================================
*/
VOID RTMPSendWirelessEvent(
	IN	PRTMP_ADAPTER	pAd,
	IN	USHORT			Event_flag,
	IN	PUCHAR 			pAddr,
	IN	UCHAR			BssIdx,
	IN	CHAR			Rssi)
{
#ifndef VENDOR_FEATURE3_SUPPORT
#if WIRELESS_EXT >= 15

	//union 	iwreq_data      wrqu;
	PSTRING	pBuf = NULL, pBufPtr = NULL;
	USHORT	event, type, BufLen;	
	UCHAR	event_table_len = 0;

	type = Event_flag & 0xFF00;	
	event = Event_flag & 0x00FF;

	switch (type)
	{
		case IW_SYS_EVENT_FLAG_START:
			event_table_len = IW_SYS_EVENT_TYPE_NUM;
			break;

		case IW_SPOOF_EVENT_FLAG_START:
			event_table_len = IW_SPOOF_EVENT_TYPE_NUM;
			break;

		case IW_FLOOD_EVENT_FLAG_START:
			event_table_len = IW_FLOOD_EVENT_TYPE_NUM;
			break;
#ifdef WSC_INCLUDED
		case IW_WSC_EVENT_FLAG_START:
			event_table_len = IW_WSC_EVENT_TYPE_NUM;
			break;
#endif // WSC_INCLUDED //
	}
	
	if (event_table_len == 0)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s : The type(%0x02x) is not valid.\n", __FUNCTION__, type));			       		       		
		return;
	}
	
	if (event >= event_table_len)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s : The event(%0x02x) is not valid.\n", __FUNCTION__, event));			       		       		
		return;
	}	
 
	//Allocate memory and copy the msg.
	if((pBuf = kmalloc(IW_CUSTOM_MAX_LEN, GFP_ATOMIC)) != NULL)
	{
		//Prepare the payload 
		memset(pBuf, 0, IW_CUSTOM_MAX_LEN);

		pBufPtr = pBuf;		

		if (pAddr)
			pBufPtr += sprintf(pBufPtr, "(RT2860) STA(%02x:%02x:%02x:%02x:%02x:%02x) ", PRINT_MAC(pAddr));				
		else if (BssIdx < MAX_MBSSID_NUM)
			pBufPtr += sprintf(pBufPtr, "(RT2860) BSS(ra%d) ", BssIdx);
		else
			pBufPtr += sprintf(pBufPtr, "(RT2860) ");

		if (type == IW_SYS_EVENT_FLAG_START)
        {
			pBufPtr += sprintf(pBufPtr, "%s", pWirelessSysEventText[event]);
		    
            if (Event_flag == IW_CHANNEL_CHANGE_EVENT_FLAG)
		  	{
			 	pBufPtr += sprintf(pBufPtr, "%3d", Rssi);
			}			
		}
#ifdef IDS_SUPPORT		
		else if (type == IW_SPOOF_EVENT_FLAG_START)
			pBufPtr += sprintf(pBufPtr, "%s (RSSI=%d)", pWirelessSpoofEventText[event], Rssi);
		else if (type == IW_FLOOD_EVENT_FLAG_START)
			pBufPtr += sprintf(pBufPtr, "%s", pWirelessFloodEventText[event]);
#endif // IDS_SUPPORT //	
#ifdef WSC_INCLUDED
		else if (type == IW_WSC_EVENT_FLAG_START)
			pBufPtr += sprintf(pBufPtr, "%s", pWirelessWscEventText[event]);
#endif // WSC_INCLUDED //
		else
			pBufPtr += sprintf(pBufPtr, "%s", "unknown event");
		
		pBufPtr[pBufPtr - pBuf] = '\0';
		BufLen = pBufPtr - pBuf;
		
		RtmpOSWrielessEventSend(pAd, IWEVCUSTOM, Event_flag, NULL, (PUCHAR)pBuf, BufLen);
		//DBGPRINT(RT_DEBUG_TRACE, ("%s : %s\n", __FUNCTION__, pBuf));	
	
		kfree(pBuf);
	}
	else
		DBGPRINT(RT_DEBUG_ERROR, ("%s : Can't allocate memory for wireless event.\n", __FUNCTION__));			       		       				
#else
	DBGPRINT(RT_DEBUG_ERROR, ("%s : The Wireless Extension MUST be v15 or newer.\n", __FUNCTION__));	
#endif  /* WIRELESS_EXT >= 15 */  
#endif // VENDOR_FEATURE3_SUPPORT //
}


#ifdef CONFIG_AP_SUPPORT
#ifdef DOT1X_SUPPORT
VOID SendSingalToDaemon(
					   IN INT              sig,
					   ULONG               pid)
{
}


/*
 ========================================================================
 Routine Description:
    Send Leyer 2 Frame to notify 802.1x daemon to disconnect
    a specific client.

 Arguments:

 Return Value:
    TRUE - send successfully
    FAIL - send fail

 Note:
 ========================================================================
*/
BOOLEAN Dot1xDisconnectNotifyAction(
    IN  PRTMP_ADAPTER	pAd,
    IN  MAC_TABLE_ENTRY *pEntry)
{
	INT				apidx = MAIN_MBSSID;
	UCHAR 			Header802_3[14];
	UCHAR 			RalinkIe[9] = {221, 7, 0x00, 0x0c, 0x43, 0x00, 0x00, 0x00, 0x00};

    if((pEntry->AuthMode == Ndis802_11AuthModeWPA) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2) || (pAd->ApCfg.MBSSID[apidx].IEEE8021X == TRUE))
	{
		INT size = sizeof(Header802_3) + sizeof(RalinkIe);
		struct sk_buff *skb = dev_alloc_skb(size+2);

		apidx = pEntry->apidx;
		MAKE_802_3_HEADER(Header802_3, pAd->ApCfg.MBSSID[apidx].Bssid, pEntry->Addr, EAPOL);

		if (!skb)
		{
			DBGPRINT(RT_DEBUG_ERROR, ("Error! Can't allocate a skb.\n"));
			return FALSE;
		}
   		GET_OS_PKT_NETDEV(skb) = get_netdev_from_bssid(pAd, apidx);

		skb_reserve(skb, 2);	// 16 byte align the IP header
		NdisMoveMemory(GET_OS_PKT_DATAPTR(skb), Header802_3, LENGTH_802_3);
		NdisMoveMemory(GET_OS_PKT_DATAPTR(skb) + LENGTH_802_3, RalinkIe, sizeof(RalinkIe));

 		skb_put(GET_OS_PKT_TYPE(skb), size);

		DBGPRINT(RT_DEBUG_TRACE, ("Notify 8021.x daemon to remove this sta(%02x:%02x:%02x:%02x:%02x:%02x)\n", PRINT_MAC(pEntry->Addr)));

		announce_802_3_packet(pAd, skb);

	}

	return TRUE;
}



/*
 ========================================================================
 Routine Description:
    Send Leyer 2 Frame to trigger 802.1x EAP state machine.

 Arguments:

 Return Value:
    TRUE - send successfully
    FAIL - send fail

 Note:
 ========================================================================
*/
BOOLEAN Dot1xEapTriggerAction(
    IN  PRTMP_ADAPTER	pAd,
    IN  MAC_TABLE_ENTRY *pEntry)
{
	INT				apidx = MAIN_MBSSID;
	UCHAR 			Header802_3[14];
	UCHAR 			eapol_start_1x_hdr[4] = {0x01, 0x01, 0x00, 0x00};

    if((pEntry->AuthMode == Ndis802_11AuthModeWPA) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2) || (pAd->ApCfg.MBSSID[apidx].IEEE8021X == TRUE))
	{
		INT size = sizeof(Header802_3) + sizeof(eapol_start_1x_hdr);
		struct sk_buff *skb = dev_alloc_skb(size+2);

		apidx = pEntry->apidx;
		MAKE_802_3_HEADER(Header802_3, pAd->ApCfg.MBSSID[apidx].Bssid, pEntry->Addr, EAPOL);

		if (!skb)
		{
				DBGPRINT(RT_DEBUG_ERROR, ("Error! Can't allocate a skb.\n"));
				return FALSE;
		}
	   	GET_OS_PKT_NETDEV(skb) = get_netdev_from_bssid(pAd, apidx);

		skb_reserve(skb, 2);	// 16 byte align the IP header
		NdisMoveMemory(GET_OS_PKT_DATAPTR(skb), Header802_3, LENGTH_802_3);
		NdisMoveMemory(GET_OS_PKT_DATAPTR(skb) + LENGTH_802_3, eapol_start_1x_hdr, sizeof(eapol_start_1x_hdr));

 		skb_put(GET_OS_PKT_TYPE(skb), size);

		DBGPRINT(RT_DEBUG_TRACE, ("Notify 8021.x daemon to trigger EAP-SM for this sta(%02x:%02x:%02x:%02x:%02x:%02x)\n", PRINT_MAC(pEntry->Addr)));

	    announce_802_3_packet(pAd, skb);

	}

	return TRUE;
}
#endif // DOT1X_SUPPORT //
#endif // CONFIG_AP_SUPPORT //




/*******************************************************************************

	Device IRQ related functions.
	
 *******************************************************************************/
int RtmpOSIRQRequest(IN PNET_DEV pNetDev)
{
	struct net_device *net_dev = pNetDev;
	PRTMP_ADAPTER pAd = NULL;
	int retval = 0;
	
	GET_PAD_FROM_NET_DEV(pAd, pNetDev);	
	
	ASSERT(pAd);
	
#ifdef RTMP_PCI_SUPPORT
	if (pAd->infType == RTMP_DEV_INF_PCI||pAd->infType == RTMP_DEV_INF_PCIE)
	{
		POS_COOKIE _pObj = (POS_COOKIE)(pAd->OS_Cookie);
		RTMP_MSI_ENABLE(pAd);	
		retval = request_irq(_pObj->pci_dev->irq,  rt2860_interrupt, SA_SHIRQ, (net_dev)->name, (net_dev));
		if (retval != 0) 
			printk("RT2860: request_irq  ERROR(%d)\n", retval);
	}
#endif // RTMP_PCI_SUPPORT //


	return retval; 
	
}


int RtmpOSIRQRelease(IN PNET_DEV pNetDev)
{
	struct net_device *net_dev = pNetDev;
	PRTMP_ADAPTER pAd = NULL;

	GET_PAD_FROM_NET_DEV(pAd, net_dev);	
	
	ASSERT(pAd);
	
#ifdef RTMP_PCI_SUPPORT
	if (pAd->infType == RTMP_DEV_INF_PCI||pAd->infType == RTMP_DEV_INF_PCIE)
	{ 
		POS_COOKIE pObj = (POS_COOKIE)(pAd->OS_Cookie);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
		synchronize_irq(pObj->pci_dev->irq);
#endif
		free_irq(pObj->pci_dev->irq, (net_dev));
		RTMP_MSI_DISABLE(pAd);
	}
#endif // RTMP_PCI_SUPPORT //


	return 0;
}


/*******************************************************************************

	File open/close related functions.
	
 *******************************************************************************/
RTMP_OS_FD RtmpOSFileOpen(char *pPath,  int flag, int mode)
{
	struct file	*filePtr;
		
	filePtr = filp_open(pPath, flag, 0);
	if (IS_ERR(filePtr))
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s(): Error %ld opening %s\n", __FUNCTION__, -PTR_ERR(filePtr), pPath));
	}

	return (RTMP_OS_FD)filePtr;
}

int RtmpOSFileClose(RTMP_OS_FD osfd)
{
	filp_close(osfd, NULL);
	return 0;
}


void RtmpOSFileSeek(RTMP_OS_FD osfd, int offset)
{
	osfd->f_pos = offset;
}


int RtmpOSFileRead(RTMP_OS_FD osfd, char *pDataPtr, int readLen)
{
	// The object must have a read method
	if (osfd->f_op && osfd->f_op->read)
	{
		return osfd->f_op->read(osfd,  pDataPtr, readLen, &osfd->f_pos);
	}
	else
	{
		DBGPRINT(RT_DEBUG_ERROR, ("no file read method\n"));
		return -1;
	}
}


int RtmpOSFileWrite(RTMP_OS_FD osfd, char *pDataPtr, int writeLen)
{
	return osfd->f_op->write(osfd, pDataPtr, (size_t)writeLen, &osfd->f_pos);
}


void RtmpOSFSInfoChange(RTMP_OS_FS_INFO *pOSFSInfo, BOOLEAN bSet)
{
	if (bSet)
	{
		// Save uid and gid used for filesystem access.
		// Set user and group to 0 (root)	
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29)
		pOSFSInfo->fsuid= current->fsuid;
		pOSFSInfo->fsgid = current->fsgid;
		current->fsuid = current->fsgid = 0;
#else
		pOSFSInfo->fsuid = current_fsuid();
		pOSFSInfo->fsgid = current_fsgid();		
#endif
		pOSFSInfo->fs = get_fs();
		set_fs(KERNEL_DS);
	}
	else
	{
		set_fs(pOSFSInfo->fs);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29)
		current->fsuid = pOSFSInfo->fsuid;
		current->fsgid = pOSFSInfo->fsgid;
#endif
	}
}



/*******************************************************************************

	Task create/management/kill related functions.
	
 *******************************************************************************/
NDIS_STATUS RtmpOSTaskKill(
	IN RTMP_OS_TASK *pTask)
{
	RTMP_ADAPTER *pAd;
	int ret = NDIS_STATUS_FAILURE;

	pAd = (RTMP_ADAPTER *)pTask->priv;

#ifdef KTHREAD_SUPPORT
	if (pTask->kthread_task)
	{
		kthread_stop(pTask->kthread_task);
		ret = NDIS_STATUS_SUCCESS;
	}
#else
	CHECK_PID_LEGALITY(pTask->taskPID)
	{
		printk("Terminate the task(%s) with pid(%d)!\n", pTask->taskName, GET_PID_NUMBER(pTask->taskPID));
		mb();
		pTask->task_killed = 1;
		mb();
		ret = KILL_THREAD_PID(pTask->taskPID, SIGTERM, 1);
		if (ret)
		{
			printk(KERN_WARNING "kill task(%s) with pid(%d) failed(retVal=%d)!\n", 
				pTask->taskName, GET_PID_NUMBER(pTask->taskPID), ret);
		}
		else 
		{
			wait_for_completion(&pTask->taskComplete);
			pTask->taskPID = THREAD_PID_INIT_VALUE;
			pTask->task_killed = 0;
			ret = NDIS_STATUS_SUCCESS;
		}
	}
#endif

	return ret;
	
}


INT RtmpOSTaskNotifyToExit(
	IN RTMP_OS_TASK *pTask)
{

#ifndef KTHREAD_SUPPORT
	complete_and_exit(&pTask->taskComplete, 0);
#endif
	
	return 0;
}


void RtmpOSTaskCustomize(
	IN RTMP_OS_TASK *pTask)
{

#ifndef KTHREAD_SUPPORT

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
	daemonize((PSTRING)&pTask->taskName[0]/*"%s",pAd->net_dev->name*/);

	allow_signal(SIGTERM);
	allow_signal(SIGKILL);
	current->flags |= PF_NOFREEZE;
#else
	unsigned long flags;

	daemonize();
	reparent_to_init();
	strcpy(current->comm, &pTask->taskName[0]);

	siginitsetinv(&current->blocked, sigmask(SIGTERM) | sigmask(SIGKILL));	
	
	/* Allow interception of SIGKILL only
	 * Don't allow other signals to interrupt the transmission */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,4,22)
	spin_lock_irqsave(&current->sigmask_lock, flags);
	flush_signals(current);
	recalc_sigpending(current);
	spin_unlock_irqrestore(&current->sigmask_lock, flags);
#endif
#endif
	RTMP_GET_OS_PID(pTask->taskPID, current->pid);
    /* signal that we've started the thread */
	complete(&pTask->taskComplete);

#endif
}


NDIS_STATUS RtmpOSTaskAttach(
	IN RTMP_OS_TASK *pTask,
	IN int (*fn)(void *), 
	IN void *arg)
{	
	NDIS_STATUS status = NDIS_STATUS_SUCCESS;
	pid_t pid_number = -1;
	
#ifdef KTHREAD_SUPPORT
	pTask->task_killed = 0;
	pTask->kthread_task = NULL;
	pTask->kthread_task = kthread_run(fn, arg, pTask->taskName);
	if (IS_ERR(pTask->kthread_task))
		status = NDIS_STATUS_FAILURE;
#else
	pid_number = kernel_thread(fn, arg, RTMP_OS_MGMT_TASK_FLAGS);
	if (pid_number < 0) 
	{
		DBGPRINT (RT_DEBUG_ERROR, ("Attach task(%s) failed!\n", pTask->taskName));
		status = NDIS_STATUS_FAILURE;
	}
	else
	{
		//pTask->taskPID = GET_PID(pid_number);
		
		// Wait for the thread to start
		wait_for_completion(&pTask->taskComplete);
		status = NDIS_STATUS_SUCCESS;
	}
#endif
	return status;
}


NDIS_STATUS RtmpOSTaskInit(
	IN RTMP_OS_TASK *pTask,
	IN PSTRING		pTaskName,
	IN VOID			*pPriv)
{
	int len;

	ASSERT(pTask);

#ifndef KTHREAD_SUPPORT
	NdisZeroMemory((PUCHAR)(pTask), sizeof(RTMP_OS_TASK));
#endif

	len = strlen(pTaskName);
	len = len > (RTMP_OS_TASK_NAME_LEN -1) ? (RTMP_OS_TASK_NAME_LEN-1) : len;
	NdisMoveMemory(&pTask->taskName[0], pTaskName, len);
	pTask->priv = pPriv;

#ifndef KTHREAD_SUPPORT
	RTMP_SEM_EVENT_INIT_LOCKED(&(pTask->taskSema));
	pTask->taskPID = THREAD_PID_INIT_VALUE;
	
	init_completion (&pTask->taskComplete);
#endif

	return NDIS_STATUS_SUCCESS;
}


void RTMP_IndicateMediaState(
	IN	PRTMP_ADAPTER	pAd)
{	
	if (pAd->CommonCfg.bWirelessEvent)	
	{
		if (pAd->IndicateMediaState == NdisMediaStateConnected)
		{
			RTMPSendWirelessEvent(pAd, IW_STA_LINKUP_EVENT_FLAG, pAd->MacTab.Content[BSSID_WCID].Addr, BSS0, 0);
		}
		else
		{							
			RTMPSendWirelessEvent(pAd, IW_STA_LINKDOWN_EVENT_FLAG, pAd->MacTab.Content[BSSID_WCID].Addr, BSS0, 0); 		
		}	
	}
}


#if LINUX_VERSION_CODE <= 0x20402	// Red Hat 7.1
//static struct net_device *alloc_netdev(int sizeof_priv, const char *mask, void (*setup)(struct net_device *)) //sample
struct net_device *alloc_netdev(
	int sizeof_priv,
	const char *mask,
	void (*setup)(struct net_device *))
{
    struct net_device	*dev;
    INT					alloc_size;


    /* ensure 32-byte alignment of the private area */
    alloc_size = sizeof (*dev) + sizeof_priv + 31;

    dev = (struct net_device *) kmalloc(alloc_size, GFP_KERNEL);
    if (dev == NULL)
    {
        DBGPRINT(RT_DEBUG_ERROR,
				("alloc_netdev: Unable to allocate device memory.\n"));
        return NULL;
    }

    memset(dev, 0, alloc_size);

    if (sizeof_priv)
        dev->priv = (void *) (((long)(dev + 1) + 31) & ~31);

    setup(dev);
    strcpy(dev->name, mask);

    return dev;
}
#endif // LINUX_VERSION_CODE //


int RtmpOSWrielessEventSend(
	IN RTMP_ADAPTER *pAd,
	IN UINT32		eventType,
	IN INT			flags,
	IN PUCHAR		pSrcMac,
	IN PUCHAR		pData,
	IN UINT32		dataLen)
{
	union iwreq_data    wrqu;
	
       memset(&wrqu, 0, sizeof(wrqu));
	   
	if (flags > -1)
	       wrqu.data.flags = flags;

	if (pSrcMac)
		memcpy(wrqu.ap_addr.sa_data, pSrcMac, MAC_ADDR_LEN);
	
	if ((pData!= NULL) && (dataLen > 0))
		wrqu.data.length = dataLen;
	
       wireless_send_event(pAd->net_dev, eventType, &wrqu, (char *)pData);
	return 0;
}


int RtmpOSNetDevAddrSet(
	IN PNET_DEV pNetDev,
	IN PUCHAR	pMacAddr)
{
	struct net_device *net_dev;
	RTMP_ADAPTER *pAd;
		
	net_dev = pNetDev;
	GET_PAD_FROM_NET_DEV(pAd, net_dev);	
	

	NdisMoveMemory(net_dev->dev_addr, pMacAddr, 6);

	return 0;
}



/*
  *	Assign the network dev name for created Ralink WiFi interface.
  */
static int RtmpOSNetDevRequestName(
	IN RTMP_ADAPTER *pAd, 
	IN PNET_DEV dev, 
	IN PSTRING pPrefixStr, 
	IN INT	devIdx)
{
	PNET_DEV		existNetDev;
	STRING		suffixName[IFNAMSIZ];
	STRING		desiredName[IFNAMSIZ];
	int	ifNameIdx, prefixLen, slotNameLen;
	int Status;
	

	prefixLen = strlen(pPrefixStr);
	ASSERT((prefixLen < IFNAMSIZ));
	
	for (ifNameIdx = devIdx; ifNameIdx < 32; ifNameIdx++)
	{
		memset(suffixName, 0, IFNAMSIZ);
		memset(desiredName, 0, IFNAMSIZ);
		strncpy(&desiredName[0], pPrefixStr, prefixLen);
		
#ifdef MULTIPLE_CARD_SUPPORT
		if (pAd->MC_RowID >= 0)
			sprintf(suffixName, "%02d_%d", pAd->MC_RowID, ifNameIdx);
		else
#endif // MULTIPLE_CARD_SUPPORT //
		sprintf(suffixName, "%d", ifNameIdx);

		slotNameLen = strlen(suffixName);
		ASSERT(((slotNameLen + prefixLen) < IFNAMSIZ));
		strcat(desiredName, suffixName);
		
		existNetDev = RtmpOSNetDevGetByName(dev, &desiredName[0]);
		if (existNetDev == NULL) 
			break;
		else
			RtmpOSNetDeviceRefPut(existNetDev);
	}
	
	if(ifNameIdx < 32)
	{
#ifdef HOSTAPD_SUPPORT
		pAd->IoctlIF=ifNameIdx;
#endif//HOSTAPD_SUPPORT//
		strcpy(&dev->name[0], &desiredName[0]);
		Status = NDIS_STATUS_SUCCESS;
	} 
	else 
	{
		DBGPRINT(RT_DEBUG_ERROR, 
					("Cannot request DevName with preifx(%s) and in range(0~32) as suffix from OS!\n", pPrefixStr));
		Status = NDIS_STATUS_FAILURE;
	}

	return Status;
}


void RtmpOSNetDevClose(
	IN PNET_DEV pNetDev)
{
	dev_close(pNetDev);
}


void RtmpOSNetDevFree(PNET_DEV pNetDev)
{
	ASSERT(pNetDev);
	
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
	free_netdev(pNetDev);
#else
	kfree(pNetDev);
#endif
}


INT RtmpOSNetDevAlloc(
	IN PNET_DEV *new_dev_p,
	IN UINT32	privDataSize)
{
	// assign it as null first.
	*new_dev_p = NULL;

	DBGPRINT(RT_DEBUG_TRACE, ("Allocate a net device with private data size=%d!\n", privDataSize));
#if LINUX_VERSION_CODE <= 0x20402 // Red Hat 7.1
	*new_dev_p = alloc_netdev(privDataSize, "eth%d", ether_setup);
#else
	*new_dev_p = alloc_etherdev(privDataSize);
#endif // LINUX_VERSION_CODE //

	if (*new_dev_p)
		return NDIS_STATUS_SUCCESS;
	else
		return NDIS_STATUS_FAILURE;
}


PNET_DEV RtmpOSNetDevGetByName(PNET_DEV pNetDev, PSTRING pDevName)
{
	PNET_DEV	pTargetNetDev = NULL;


#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)
	pTargetNetDev = dev_get_by_name(dev_net(pNetDev), pDevName);
#else
	ASSERT(pNetDev);
	pTargetNetDev = dev_get_by_name(pNetDev->nd_net, pDevName);
#endif
#else
	pTargetNetDev = dev_get_by_name(pDevName);
#endif // KERNEL_VERSION(2,6,24) //

#else
	int	devNameLen;

	devNameLen = strlen(pDevName);
	ASSERT((devNameLen <= IFNAMSIZ));
	
	for(pTargetNetDev=dev_base; pTargetNetDev!=NULL; pTargetNetDev=pTargetNetDev->next)
	{
		if (strncmp(pTargetNetDev->name, pDevName, devNameLen) == 0)
			break;
	}
#endif // KERNEL_VERSION(2,5,0) //

	return pTargetNetDev;
}


void RtmpOSNetDeviceRefPut(PNET_DEV pNetDev)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
	/* 
		every time dev_get_by_name is called, and it has returned a valid struct 
		net_device*, dev_put should be called afterwards, because otherwise the 
		machine hangs when the device is unregistered (since dev->refcnt > 1).
	*/
	if(pNetDev)
		dev_put(pNetDev);
#endif // LINUX_VERSION_CODE //
}


INT RtmpOSNetDevDestory(
	IN RTMP_ADAPTER *pAd,
	IN PNET_DEV		pNetDev)
{

	// TODO: Need to fix this
	printk("WARNING: This function(%s) not implement yet!!!\n", __FUNCTION__);
	return 0;
}


void RtmpOSNetDevDetach(PNET_DEV pNetDev)
{
	unregister_netdev(pNetDev);
}


int RtmpOSNetDevAttach(
	IN PNET_DEV pNetDev, 
	IN RTMP_OS_NETDEV_OP_HOOK *pDevOpHook)
{	
	int ret, rtnl_locked = FALSE;

	DBGPRINT(RT_DEBUG_TRACE, ("RtmpOSNetDevAttach()--->\n"));
	// If we need hook some callback function to the net device structrue, now do it.
	if (pDevOpHook)
	{
		PRTMP_ADAPTER pAd = NULL;
	
		GET_PAD_FROM_NET_DEV(pAd, pNetDev);	
		pNetDev->open			= pDevOpHook->open;
		pNetDev->stop			= pDevOpHook->stop;
		pNetDev->hard_start_xmit	= (HARD_START_XMIT_FUNC)(pDevOpHook->xmit);
		pNetDev->do_ioctl		= pDevOpHook->ioctl;
		
		/* if you don't implement get_stats, just leave the callback function as NULL, a dummy 
		     function will make kernel panic.
		*/
		if (pDevOpHook->get_stats)
			pNetDev->get_stats = pDevOpHook->get_stats;

		/* OS specific flags, here we used to indicate if we are virtual interface */
		pNetDev->priv_flags = pDevOpHook->priv_flags; 

#if (WIRELESS_EXT < 21) && (WIRELESS_EXT >= 12)
		pNetDev->get_wireless_stats = rt28xx_get_wireless_stats;
#endif


#ifdef CONFIG_APSTA_MIXED_SUPPORT
#if WIRELESS_EXT >= 12
		if (pAd->OpMode == OPMODE_AP)
		{
			pNetDev->wireless_handlers = &rt28xx_ap_iw_handler_def;
		}
#endif //WIRELESS_EXT >= 12
#endif // CONFIG_APSTA_MIXED_SUPPORT //

		// copy the net device mac address to the net_device structure.
		NdisMoveMemory(pNetDev->dev_addr, &pDevOpHook->devAddr[0], MAC_ADDR_LEN);

		rtnl_locked = pDevOpHook->needProtcted;
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24)
	pNetDev->validate_addr = NULL;
#endif

	if (rtnl_locked)
		ret = register_netdevice(pNetDev);
	else
		ret = register_netdev(pNetDev);

	DBGPRINT(RT_DEBUG_TRACE, ("<---RtmpOSNetDevAttach(), ret=%d\n", ret));
	if (ret == 0)
		return NDIS_STATUS_SUCCESS;
	else
		return NDIS_STATUS_FAILURE;
}


PNET_DEV RtmpOSNetDevCreate(
	IN RTMP_ADAPTER *pAd,
	IN INT 			devType, 
	IN INT			devNum,
	IN INT			privMemSize,
	IN PSTRING		pNamePrefix)
{
	struct net_device *pNetDev = NULL;
	int status;


	/* allocate a new network device */
	status = RtmpOSNetDevAlloc(&pNetDev, 0 /*privMemSize*/);
	if (status != NDIS_STATUS_SUCCESS)
	{
		/* allocation fail, exit */
		DBGPRINT(RT_DEBUG_ERROR, ("Allocate network device fail (%s)...\n", pNamePrefix));
		return NULL;
	}

	
	/* find a available interface name, max 32 interfaces */
	status = RtmpOSNetDevRequestName(pAd, pNetDev, pNamePrefix, devNum);
	if (status != NDIS_STATUS_SUCCESS)
	{
		/* error! no any available ra name can be used! */
		DBGPRINT(RT_DEBUG_ERROR, ("Assign interface name (%s with suffix 0~32) failed...\n", pNamePrefix));
		RtmpOSNetDevFree(pNetDev);
		
		return NULL;
	}
	else
	{
		DBGPRINT(RT_DEBUG_TRACE, ("The name of the new %s interface is %s...\n", pNamePrefix, pNetDev->name));
	}

	return pNetDev;
}

