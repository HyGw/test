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
	rtmp_apcli.c

	Abstract:
	apcli convert engine subroutines, we should just take care data packet.

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Shiang      June/16/06  	init version for RT2561/2661
*/

#ifdef APCLI_SUPPORT

#include "rtmp_apcli.h"

typedef struct _UWRProtoTable
{
	UINT16 					protoCode;
	struct _UWRProtoEntry	*pHandle;	
}UWRProtoTable, PUWRProtoTable;

extern UWRProtoEntry apCliIPHandler;
extern UWRProtoEntry apCliARPHandler;
extern UWRProtoEntry apCliPPPoEDisHandler;
extern UWRProtoEntry apCliPPPoESesHandler;

extern UCHAR SNAP_802_1H[];
extern UCHAR SNAP_BRIDGE_TUNNEL[];

static UWRProtoTable UWRProtoTb[]=
{
	{ETH_P_IP, 		&apCliIPHandler},		// IP handler
	{ETH_P_ARP, 		&apCliARPHandler},		// ARP handler
	{ETH_P_PPP_DISC,	&apCliPPPoEDisHandler}, // PPPoE discovery stage handler
	{ETH_P_PPP_SES,		&apCliPPPoESesHandler},	// PPPoE session stage handler
//	{0x0,    			NULL},					
};

#define MAX_SUPPORT_PROTOCOL_NUM (sizeof(UWRProtoTb)/sizeof(UWRProtoTable))

#define APCLI_NODE_ENTRY_SIZE	28	// bytes
#define APCLI_NODE_ENTRY_NUM	127	// We support maxima 127 node entry for our system
typedef struct _UWRNodeEntry
{
	UCHAR data[APCLI_NODE_ENTRY_SIZE];
	struct _UWRNodeEntry *next;
}UWRNodeEntry, *PUWRNodeEntry;

static int UWREngineInited = 0;
static int UWREngineExited = 1;
spinlock_t apcli_db_lock;

#ifdef KMALLOC_BATCH
static UWRNodeEntry *UWRNodeEntryPoll = NULL;

NDIS_STATUS UWRNodeEntryFree(PUCHAR NodeEntry)
{
	UWRNodeEntry *pPtr;

	pPtr = (UWRNodeEntry *)NodeEntry;
	memset(pPtr, 0, sizeof(UWRNodeEntry));
	if (UWRNodeEntryPoll->next)
	{
		pPtr->next = UWRNodeEntryPoll->next;
		UWRNodeEntryPoll->next = pPtr;
	} else {
		UWRNodeEntryPoll->next = pPtr;
	}

	return TRUE;
}

PUCHAR UWRNodeEntryAlloc(VOID)
{
	struct _UWRNodeEntry *pPtr = NULL;
	
	if (UWRNodeEntryPoll->next)
	{
		pPtr = UWRNodeEntryPoll->next;
		UWRNodeEntryPoll->next = pPtr->next;
	}
	
	return (PUCHAR)pPtr;
}

#endif

VOID dumpPkt(PUCHAR pHeader, int len)
{
	int i;
	char *tmp;

	tmp = pHeader;

	printk("--StartDump\n");
	for(i=0;i<len; i++)
	{
		if ( (i%16==0) && (i!=0))
			printk("\n");
		printk("%02x ", tmp[i]& 0xff);
	}
	printk("\n--EndDump\n");

	return;
}

PUCHAR get80211UpperLayerProto(
	IN PUCHAR 			pHeader,
	IN PRXD_STRUC		pRxD)
{
	USHORT	DataSize, Msdu2Size;
	HEADER_802_11 		*p80211Hdr;
	PUCHAR				pProto=NULL;
	
	if (!pHeader)
		return NULL;

	pProto = (UCHAR *)pHeader;
	p80211Hdr = (PHEADER_802_11)pHeader;

	// If it's a WDS acket, shift the addr4
	if ((p80211Hdr->FC.FrDs == 1) && (p80211Hdr->FC.ToDs == 1))
    {
    	pProto += LENGTH_802_11_WITH_ADDR4;
		DataSize = (USHORT)pRxD->DataByteCnt - LENGTH_802_11_WITH_ADDR4;
	}
    else
    {
    	pProto += LENGTH_802_11;
		DataSize = (USHORT)pRxD->DataByteCnt - LENGTH_802_11;
    }

	// If it's a QoS packet, shift more 2 bytes
	if (p80211Hdr->FC.SubType & 0x08)
    {
        pProto += 2;
		DataSize -= 2;
    }

	//Check for Ralink specific aggregate packet
	if (p80211Hdr->FC.Order)
    {
		Msdu2Size = *pProto + (*(pProto+1) << 8);
        if ((Msdu2Size <= 1536) && (Msdu2Size < DataSize))
        {
			pProto += 2;
        }
	}

	// Handle 802.1h or bridge tunnel data
	if (NdisEqualMemory(SNAP_802_1H, pProto, 6) ||
	    NdisEqualMemory(SNAP_BRIDGE_TUNNEL, pProto, 6))
	{
		pProto += 6;
	}

	return pProto;

}


/*
	========================================================================
	Routine	Description:
		For each out-going packet, check the upper layer protocol type if need
		to handled by our APCLI convert engine. If yes, call corresponding handler 
		to handle it.
		
	Arguments:
		pAd		=>Pointer to our adapter
		pHeader =>pointer to the 802.11 header of outgoing packet 

	Return Value:
		Success	=>
			TRUE
			Mapped mac address if found, else return specific default mac address 
			depends on the upper layer protocol type.
		Error	=>
			FALSE.

	Note:
		1.the pPktHdr must be a 802.3 packet.
		2.Maybe we need a TxD arguments?
		3.We check every packet here including group mac address becasue we need to
		  handle DHCP packet.
	========================================================================
 */
PUCHAR UWRMacUpdaterForTxPkt(
	IN PRTMP_ADAPTER	pAd,
	IN struct sk_buff	*pSkb,
	IN PTXD_STRUC		pTxD)
{
	PUCHAR 		pLayerHdr = NULL, pPktHdr = NULL;
	UINT16		protoType;
	INT			i;
	struct _UWRProtoEntry 	*pHandle = NULL;
	PUCHAR  retSkb = NULL;
#ifndef THREAD_ISR
	UMAXLONG	IrqFlags;
#endif

	pPktHdr = pSkb->data;
	if (!pPktHdr)
		return NULL;
	
	// Get the upper layer protocol type of this 802.3 pkt.
	protoType = ntohs(*((UINT16 *)(pPktHdr + 12)));
	//DBGPRINT(RT_DEBUG_TRACE,"%s(): protoType=0x%04x\n", __FUNCTION__, protoType);
	// For differnet protocol, dispatch to specific handler
	for (i=0; i<MAX_SUPPORT_PROTOCOL_NUM; i++)
	{
		if (protoType == UWRProtoTb[i].protoCode)
		{
			pHandle = UWRProtoTb[i].pHandle;	// the pHandle must not be null!
			pLayerHdr = (pPktHdr + APCLI_ETHER_HDR_LEN);
#ifndef THREAD_ISR
			RTMP_SEM_LOCK(&apcli_db_lock, IrqFlags);
#endif
			if (pHandle->tx!=NULL)
				retSkb = pHandle->tx(pAd, pSkb, pLayerHdr);
#ifndef THREAD_ISR
			RTMP_SEM_UNLOCK(&apcli_db_lock, IrqFlags);
#endif
			return retSkb;
		}
	}
	return retSkb;
}


/*
	========================================================================
	Routine	Description:
		Depends on the Received packet, check the upper layer protocol type
		and search for specific mapping table to find out the real destination 
		MAC address.
		
	Arguments:
		pAd		=>Pointer to our adapter
		pHeader =>pointer to the 802.11 header of receviced packet 
		pRxD	=>pointer to the RxDesc of the receviced packet.

	Return Value:
		Success	=>
			Mapped mac address if found, else return specific default mac address 
			depends on the upper layer protocol type.
		Error	=>
			NULL

	Note:
	========================================================================
 */
PUCHAR UWRMacConverterForRxPkt(
	IN PRTMP_ADAPTER	 pAd,
	IN struct sk_buff	 *pSkb,
	IN struct net_device *net_dev)
{
	PUCHAR				pMacAddr = NULL;
	PUCHAR 		pLayerHdr = NULL, pPktHdr = NULL;
	UINT16		protoType;
	INT			i =0;
	struct _UWRProtoEntry 	*pHandle = NULL;
	UMAXLONG  IrqFlags;

	pPktHdr = pSkb->data;
	if (!pPktHdr)
		return NULL;

	// If it's a multicast/broadcast packet, we do nothing.
	if (APCLI_MAC_ADDR_IS_GROUP(pPktHdr))
		return NULL;

	// Check if the packet will be send to apcli interface.
	while(i<MAX_APCLI_ENTRY)
	{
		//BSSID match the ApCliBssid ?(from a valid AP)
		if ((pAd->ApCliTab.ApCliEntry[i].Valid == TRUE) && 
			(net_dev == pAd->ApCliTab.ApCliEntry[i].dev))
			break;
		i++;
	}

	if(i == MAX_APCLI_ENTRY)
		return NULL;

	// Get the upper layer protocol type of this 802.3 pkt and dispatch to specific handler
	protoType = ntohs(*((UINT16 *)(pPktHdr + 12)));
	DBGPRINT(RT_DEBUG_INFO, "%s(): protoType=0x%04x\n", __FUNCTION__, protoType);
	for (i=0; i<MAX_SUPPORT_PROTOCOL_NUM; i++)
	{
		if (protoType == UWRProtoTb[i].protoCode)
		{
			pHandle = UWRProtoTb[i].pHandle;	// the pHandle must not be null!
			pLayerHdr = (pPktHdr + APCLI_ETHER_HDR_LEN);
			RTMP_SEM_LOCK(&apcli_db_lock, IrqFlags);
			if(pHandle->rx!=NULL)
				pMacAddr = pHandle->rx(pAd, pSkb, pLayerHdr);
			RTMP_SEM_UNLOCK(&apcli_db_lock, IrqFlags);
			break;
		}
	}

	if (pMacAddr)
		memcpy(pPktHdr, pMacAddr, MAC_ADDR_LEN);

	return NULL;

}


NDIS_STATUS APCLIConvertEngineExit(VOID)
{
	struct _UWRProtoEntry 	*pHandle = NULL;
	int i;

	if(UWREngineExited)
		return TRUE;
	
	// For each registered protocol, we call it's exit handler.
	for (i=0; i<MAX_SUPPORT_PROTOCOL_NUM; i++)
	{
		if (UWRProtoTb[i].pHandle != NULL)
		{
			pHandle = UWRProtoTb[i].pHandle;
			if (pHandle->exit!=NULL)
				pHandle->exit();
		}
	}

#ifdef KMALLOC_BATCH
	// Free the memory used to store node entries.
	if (UWRNodeEntryPoll) 
		kfree(UWRNodeEntryPoll);
#endif

	UWREngineExited = 1;
	
	return TRUE;
	
}


NDIS_STATUS APCLIConvertEngineInit(VOID)
{
	struct _UWRProtoEntry 	*pHandle = NULL;
	int i;

	if(UWREngineInited)
		return TRUE;
	
#ifdef KMALLOC_BATCH
	// Allocate memory for node entry, we totally allocate 127 entries and link list them together.
	UWRNodeEntryPoll = (UWRNodeEntry *)kmalloc(sizeof(UWRNodeEntry) * APCLI_NODE_ENTRY_NUM, MEM_ALLOC_FLAG);
	if (UWRNodeEntryPoll)
	{
		struct _UWRNodeEntry    *pPtr=NULL;

		memset(UWRNodeEntryPoll, 0, sizeof(UWRNodeEntry) * APCLI_NODE_ENTRY_NUM);
		pPtr = UWRNodeEntryPoll;
		for (i=0; i<(APCLI_NODE_ENTRY_NUM -1); i++)
		{
			pPtr->next = (struct _UWRNodeEntry *)(pPtr+1);
			pPtr = pPtr->next;
		}
		pPtr->next = NULL;
	} else {
		return FALSE;
	}
#endif

	// For each specific protocol, call it's init function.
	for (i=0; i<MAX_SUPPORT_PROTOCOL_NUM; i++)
	{
		if (UWRProtoTb[i].pHandle != NULL)
		{
			pHandle = UWRProtoTb[i].pHandle;
			if (pHandle->init!=NULL)
				pHandle->init();
		}
	}

	RTMP_SEM_INIT(&apcli_db_lock);

	UWREngineInited = 1;
	return TRUE;
}

#endif  //Endif of "APCLI_SUPPORT"

