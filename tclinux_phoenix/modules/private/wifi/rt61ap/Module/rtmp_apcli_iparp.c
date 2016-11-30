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
	rtmp_apcli_iparp.c

	Abstract:
	apcli convert engine subroutine for ip/arp protocol.

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Shiang      June/16/06      Init version
*/
#ifdef APCLI_SUPPORT

#include "rtmp_apcli.h"

NDIS_STATUS APCLI_IP_init(VOID);
NDIS_STATUS APCLI_IP_exit(VOID);
PUCHAR APCLI_IP_rx(PRTMP_ADAPTER pAd, struct sk_buff *pSkb, PUCHAR pLayerHdr);
PUCHAR APCLI_IP_tx(PRTMP_ADAPTER pAd, struct sk_buff *pSkb, PUCHAR pLayerHdr);

NDIS_STATUS APCLI_ARP_init(VOID);
NDIS_STATUS APCLI_ARP_exit(VOID);
PUCHAR APCLI_ARP_rx(PRTMP_ADAPTER pAd, struct sk_buff *pSkb, PUCHAR pLayerHdr);
PUCHAR APCLI_ARP_tx(PRTMP_ADAPTER pAd, struct sk_buff *pSkb,PUCHAR pLayerHdr);

#define IPV4_ADDR_LEN 4

#define NEED_UPDATE_IPMAC_TB(Mac, IP) (IS_UCAST_MAC(Mac) && IS_GOOD_IP(IP))


typedef struct _IPMacMappingEntry
{
	UINT  ipAddr;	// In network order
	UCHAR macAddr[MAC_ADDR_LEN];
	UMAXLONG  lastTime;
	struct _IPMacMappingEntry *pNext;
}IPMacMappingEntry, *PIPMacMappingEntry;


typedef struct _IPMacMappingTable
{
	BOOLEAN			valid;
	IPMacMappingEntry *hash[MAX_SUPPORT_APCLI_STA+1]; //0~63 for specific station, 64 for broadcast MacAddress
}IPMacMappingTable;

static IPMacMappingTable IPMacTable=
{
	.valid = FALSE,
}; // Used for IP, ARP protocol

struct _UWRProtoEntry apCliIPHandler =
{
	.init = APCLI_IP_init,
	.tx = APCLI_IP_tx,
	.rx = APCLI_IP_rx,
	.exit = APCLI_IP_exit,
};

struct _UWRProtoEntry apCliARPHandler =
{
	.init = APCLI_ARP_init,
	.tx = APCLI_ARP_tx,
	.rx = APCLI_ARP_rx,
	.exit =APCLI_ARP_exit,
};

VOID dumpIPMacTb(int index)
{
	IPMacMappingEntry *pHead;

	if (!IPMacTable.valid)
	{
		printk("%s():IPMacTable not init yet, so cannot do dump!\n", __FUNCTION__);
		return;
	}
	
	pHead = IPMacTable.hash[index];
	DBGPRINT(RT_DEBUG_TRACE,"%s(): Now dump IPMac Table with index=%d!\n", __FUNCTION__, index);
	while(pHead)
	{
		DBGPRINT(RT_DEBUG_TRACE,"---IP=0x%x,Mac=%02x:%02x:%02x:%02x:%02x:%02x, jiffies=0x%lx, next=%p\n", 
				pHead->ipAddr, pHead->macAddr[0],pHead->macAddr[1],pHead->macAddr[2],
				pHead->macAddr[3],pHead->macAddr[4],pHead->macAddr[5], pHead->lastTime,
				pHead->pNext);
		pHead = pHead->pNext;
	}
	DBGPRINT(RT_DEBUG_TRACE,"IPMac Table EndOfDump!\n");
	
}


static inline NDIS_STATUS getDstIPFromIpPkt(
	IN PUCHAR pIpHdr, 
	IN UINT *dstIP)
{
	
	if (!pIpHdr)
		return FALSE;
	
	memcpy(dstIP, (pIpHdr + 16), 4); //shift 16 for IP header len before DstIP.
//	DBGPRINT(RT_DEBUG_TRACE,"%s(): Get the dstIP=0x%x\n", __FUNCTION__, *dstIP);
	
	return TRUE;
}

static inline NDIS_STATUS getSrcIPFromIpPkt(
	IN PUCHAR pIpHdr,
	IN UINT   *pSrcIP)
{
	
	if (!pIpHdr)
		return FALSE;
	
	memcpy(pSrcIP, (pIpHdr + 12), 4); //shift 12 for IP header len before DstIP.
//	DBGPRINT(RT_DEBUG_TRACE,"%s(): Get the srcIP=0x%x\n", __FUNCTION__, *pSrcIP);
	
	return TRUE;
	
}

NDIS_STATUS IPMacTableUpdate(
	IN PRTMP_ADAPTER	pAd,
	IN PUCHAR			pMacAddr,
	IN UINT			ipAddr)
{
	UINT 				hashIdx;
	IPMacMappingEntry	*pEntry = NULL, *pPrev = NULL, *pNewEntry =NULL;

	if (!IPMacTable.valid)
		return 0;

#if 0
	//We didn't need to maintain the Mac address of ourself.
	if(IS_EQUAL_MAC(pMacAddr, pAd->CurrentAddress))
		return 0;
#endif
	
	hashIdx = APCLI_IP_ADDR_HASH_INDEX(ipAddr);

    pEntry = pPrev = IPMacTable.hash[hashIdx];
	while(pEntry)
	{
		// Find a existed IP-MAC Mapping entry
		if (ipAddr == pEntry->ipAddr)
    	{
/*			DBGPRINT(RT_DEBUG_TRACE,"%s(): Got the Mac(%02x:%02x:%02x:%02x:%02x:%02x) of mapped IP(%d.%d.%d.%d)\n",
					__FUNCTION__, pEntry->macAddr[0],pEntry->macAddr[1],pEntry->macAddr[2],
					pEntry->macAddr[3],pEntry->macAddr[4],pEntry->macAddr[5],
					(ipAddr>>24) & 0xff, (ipAddr>>16) & 0xff, (ipAddr>>8) & 0xff, ipAddr & 0xff); 
*/
			// compare is useless. So we directly copy it into the entry.
			memcpy(pEntry->macAddr, pMacAddr, 6);
			pEntry->lastTime = jiffies;
			
	        return TRUE;
		}
        else
        {	// handle the age-out situation
        	if ((jiffies - pEntry->lastTime) > IPMAC_TB_ENTRY_AGEOUT_TIME)
        	{
        		// Remove the aged entry
        		if (pEntry == IPMacTable.hash[hashIdx])
				{
					IPMacTable.hash[hashIdx]= pEntry->pNext;
					pPrev = IPMacTable.hash[hashIdx];
        		}
				else 
				{	
	        		pPrev->pNext = pEntry->pNext;
				}
#ifdef KMALLOC_BATCH
				UWRNodeEntryFree((PUCHAR)pEntry);
#else
				kfree(pEntry);
#endif
				pEntry = (pPrev == NULL ? NULL: pPrev->pNext);
        	} 
			else
			{
				pPrev = pEntry;
	            pEntry = pEntry->pNext;
        	}
        }
	}

	// Allocate a new IPMacMapping entry and insert into the hash
#ifdef KMALLOC_BATCH
	pNewEntry = (IPMacMappingEntry *)UWRNodeEntryAlloc();
#else
	pNewEntry = (IPMacMappingEntry *)kmalloc(sizeof(IPMacMappingEntry), MEM_ALLOC_FLAG);
#endif
	if (pNewEntry != NULL)
	{	
		pNewEntry->ipAddr = ipAddr;
		memcpy(pNewEntry->macAddr, pMacAddr, 6);
		pNewEntry->pNext = NULL;
		pNewEntry->lastTime = jiffies;

		if (IPMacTable.hash[hashIdx] == NULL)
		{	// Hash list is empty, directly assign it.
			IPMacTable.hash[hashIdx] = pNewEntry;
		} 
		else 
		{
			// Ok, we insert the new entry into the root of hash[hashIdx]
			pNewEntry->pNext = IPMacTable.hash[hashIdx];
			IPMacTable.hash[hashIdx] = pNewEntry;
		}
		//dumpIPMacTb(hashIdx); //for debug
			
		return TRUE;
	}
	
	return FALSE;
}

PUCHAR IPMacTableLookUp(
	IN	PRTMP_ADAPTER	pAd,
	IN	UINT			ipAddr)
{
    UINT 				hashIdx;
    IPMacMappingEntry	*pEntry = NULL;

    if (!IPMacTable.valid)
        return NULL;
	
	// Use hash to find out the location of that entry and get the Mac address.
	hashIdx = APCLI_IP_ADDR_HASH_INDEX(ipAddr);

//	spin_lock_irqsave(&IPMacTabLock, irqFlag);
    pEntry = IPMacTable.hash[hashIdx];
	while(pEntry)
	{
		if (pEntry->ipAddr == ipAddr)
        {
/*			DBGPRINT(RT_DEBUG_TRACE,"%s(): dstMac=%02x:%02x:%02x:%02x:%02x:%02x for mapped dstIP(%d.%d.%d.%d)\n", 
					__FUNCTION__, pEntry->macAddr[0],pEntry->macAddr[1],pEntry->macAddr[2],
					pEntry->macAddr[3],pEntry->macAddr[4],pEntry->macAddr[5],
					(ipAddr>>24) & 0xff, (ipAddr>>16) & 0xff, (ipAddr>>8) & 0xff, ipAddr & 0xff); 
*/
			
			//Update the lastTime to prevent the aging before pDA processed!
			pEntry->lastTime = jiffies; 
			
			return pEntry->macAddr;
        }
        else
            pEntry = pEntry->pNext;
	}
	
	// We didn't find any matched Mac address, our policy is treat it as 
	// broadcast packet and send to all.
	return IPMacTable.hash[IPMAC_TB_HASH_INDEX_OF_BCAST]->macAddr;
	
}

NDIS_STATUS IPMacTable_RemoveAll(VOID)
{
	IPMacMappingEntry *pEntry;
	INT		i;

	if (!IPMacTable.valid)
		return TRUE;
	
	for (i=0; i<IPMAC_TB_HASH_ENTRY_NUM; i++)
	{
		while((pEntry = IPMacTable.hash[i]) != NULL)
		{
			IPMacTable.hash[i] = pEntry->pNext;
#ifdef KMALLOC_BATCH
			UWRNodeEntryFree((PUCHAR)pEntry);
#else
			kfree(pEntry);
#endif
		}
	}

	IPMacTable.valid = FALSE;

	return TRUE;
}

NDIS_STATUS IPMacTable_init(VOID)
{
	IPMacMappingEntry *pEntry = NULL;
	UCHAR BcastAddr[]= {0xff, 0xff, 0xff,0xff,0xff,0xff};
	
	memset(&IPMacTable, 0, sizeof(IPMacTable));
	
	//Set the last hash entry (hash[64]) as our default broadcast Mac address
#ifdef KMALLOC_BATCH
	pEntry = (IPMacMappingEntry *)UWRNodeEntryAlloc();
#else
	pEntry = kmalloc(sizeof(IPMacMappingEntry), MEM_ALLOC_FLAG);
#endif
	if (!pEntry)
		return FALSE;
	
	pEntry->ipAddr = 0;
	memcpy(pEntry->macAddr, BcastAddr, 6);
	pEntry->pNext = NULL;
	IPMacTable.hash[IPMAC_TB_HASH_INDEX_OF_BCAST] = pEntry;
	
	IPMacTable.valid = TRUE;

	DBGPRINT(RT_DEBUG_TRACE,"%s(): IPMacTable_init success!\n", __FUNCTION__);
	return TRUE;
}

NDIS_STATUS APCLI_ARP_exit(VOID)
{
	INT status;
		
	status = IPMacTable_RemoveAll();

	return status;
}

PUCHAR APCLI_ARP_rx(
	IN PRTMP_ADAPTER	pAd, 
	IN struct sk_buff	*pSkb,
	IN PUCHAR			pLayerHdr)
{
	PUCHAR pArpHdr = NULL, pMacAddr = NULL;
	PUCHAR	tgtMac, tgtIP;
	BOOLEAN isUcastMac, isGoodIP;

	
	pArpHdr = pLayerHdr;

//dumpPkt(pSkb->data, pSkb->len);		
	// We just take care about the target(Mac/IP address) fields.
	tgtMac = pArpHdr + 18;
	tgtIP = tgtMac + 6;
		
	// isUcastMac = !(00:00:00:00:00:00|| mcastMac);
	isUcastMac = ((tgtMac[0]|tgtMac[1]|tgtMac[2]|tgtMac[3]|tgtMac[4]|tgtMac[5])!=0);
	isUcastMac &= ((tgtMac[0] && 0x1)==0);

	// isGoodIP = ip address is not 0.0.0.0
	isGoodIP = (*(UINT *)tgtIP != 0);

	DBGPRINT(RT_DEBUG_INFO,"%s(): ARP Pkt=>tgtIP=%d.%d.%d.%d, tgtMac=%02x:%02x:%02x:%02x:%02x:%02x\n",
			__FUNCTION__, tgtIP[0], tgtIP[1], tgtIP[2], tgtIP[3],
			tgtMac[0],tgtMac[1],tgtMac[2],tgtMac[3],tgtMac[4],tgtMac[5]);		
		
	if (isUcastMac && isGoodIP)
		pMacAddr= IPMacTableLookUp(pAd, *(UINT *)tgtIP);
		
	// For need replaced mac, we need to replace the targetMAC as correct one to make 
	// the real receiver can receive that.
	if (isUcastMac && pMacAddr)
		memcpy(tgtMac, pMacAddr, MAC_ADDR_LEN);

	if (pMacAddr == NULL)
		pMacAddr = IPMacTable.hash[IPMAC_TB_HASH_INDEX_OF_BCAST]->macAddr;
	
	return pMacAddr;
}

PUCHAR APCLI_ARP_tx(
	IN PRTMP_ADAPTER pAd,
	IN struct sk_buff *pSkb,
	IN PUCHAR pLayerHdr)
{
	PUCHAR	pSMac, pSIP;
	BOOLEAN isUcastMac, isGoodIP;
	struct arphdr *arpHdr;
	PUCHAR pPktHdr;
	struct sk_buff *newSkb=NULL;

	pPktHdr = pSkb->data;
	
	arpHdr = (struct arphdr *)pLayerHdr;

	// Check the arp header.
	// We just handle ether type hardware address and IPv4 internet 
	// address type and opcode is  ARP reuqest/response.
	if ((arpHdr->ar_hrd != htons(ARPHRD_ETHER)) || (arpHdr->ar_pro != htons(ETH_P_IP)) ||
		(arpHdr->ar_op != htons(ARPOP_REPLY) && arpHdr->ar_op != htons(ARPOP_REQUEST)))
		return NULL;

	// We just take care about the sender(Mac/IP address) fields.
	pSMac =(PUCHAR)(pLayerHdr + 8);
	pSIP = (PUCHAR)(pSMac + MAC_ADDR_LEN);
	
	isUcastMac = IS_UCAST_MAC(pSMac);
	isGoodIP = IS_GOOD_IP(*(UINT *)pSIP);
	
/*	
	printk("%s(): ARP Pkt=>senderIP=%d.%d.%d.%d, senderMac=%02x:%02x:%02x:%02x:%02x:%02x\n",
			__FUNCTION__, pSIP[0], pSIP[1], pSIP[2], pSIP[3],
			pSMac[0],pSMac[1],pSMac[2],pSMac[3],pSMac[4],pSMac[5]);	
*/
	if (isUcastMac && isGoodIP)
		IPMacTableUpdate(pAd, pSMac, *(UINT *)pSIP);

	// For outgoing unicast mac, we need to replace the senderMAC as ourself to make 
	// the receiver can send to us.
	if (isUcastMac)
	{
		if(skb_cloned(pSkb)) 
		{
			newSkb = skb_copy(pSkb, GFP_ATOMIC);
			if(newSkb)
			{
				pSMac = (PUCHAR)(newSkb->data + APCLI_ETHER_HDR_LEN + 8);
				memcpy(pSMac, pAd->ApCliTab.ApCliEntry[0].CurrentAddress, MAC_ADDR_LEN);
			}
		}
		else
		{
			memcpy(pSMac, pAd->ApCliTab.ApCliEntry[0].CurrentAddress, MAC_ADDR_LEN);
		}
	}

	return (PUCHAR)newSkb;
}


NDIS_STATUS APCLI_ARP_init(VOID)
{
	BOOLEAN status = FALSE;

	status = (IPMacTable.valid==TRUE) ? TRUE : IPMacTable_init();

	if (status)
		apCliARPHandler.pMgtTb =&IPMacTable;
	else 
		apCliARPHandler.pMgtTb = NULL;

	return status;
}


NDIS_STATUS APCLI_IP_exit(VOID)
{
	INT status;
		
	status = IPMacTable_RemoveAll();

	return status;
}

PUCHAR APCLI_IP_rx(
	IN PRTMP_ADAPTER	pAd, 
	IN struct sk_buff	*pSkb,
	IN PUCHAR 			pLayerHdr)
{
	PUCHAR pMacAddr;
	UINT   dstIP;
	
	// Fetch the IP addres from the packet header.
	getDstIPFromIpPkt(pLayerHdr, &dstIP);

	DBGPRINT(RT_DEBUG_INFO,"%s():Receive a IP pkt=>dstIP=0x%x!\n", __FUNCTION__, dstIP);
	pMacAddr = IPMacTableLookUp(pAd, dstIP); 
	
	return pMacAddr;
}

static UCHAR  DHCP_MAGIC[]= {0x63, 0x82, 0x53, 0x63};
PUCHAR APCLI_IP_tx(
	IN PRTMP_ADAPTER pAd,
	IN struct sk_buff *pSkb,
	IN PUCHAR pLayerHdr)
{
	PUCHAR pSrcMac;
	PUCHAR pSrcIP;
	BOOLEAN needUpdate;
	PUCHAR pPktHdr;

	pPktHdr = pSkb->data;
	
	pSrcMac = pPktHdr + 6;
	pSrcIP = pLayerHdr + 12;

	DBGPRINT(RT_DEBUG_INFO,"%s(): Transmit a IP Pkt=>srcIP=0x%x\n", __FUNCTION__, *(UINT *)pSrcIP);

	needUpdate = NEED_UPDATE_IPMAC_TB(pSrcMac, (*(UINT *)pSrcIP));
	if (needUpdate)
		IPMacTableUpdate(pAd, pSrcMac, *(UINT *)pSrcIP);

	//For UDP packet, we need to check about the DHCP packet, to modify the flag of DHCP discovey/request as broadcast.
	if (*(pLayerHdr + 9) == 0x11)
	{
		PUCHAR udpHdr;
		UINT16 srcPort, dstPort;
		
		udpHdr = pLayerHdr +20;
		srcPort = ntohs(*((UINT16 *)udpHdr));
		dstPort = ntohs(*((UINT16 *)(udpHdr+2)));
		
		if (srcPort==68 && dstPort==67) //It's a DHCP packet
		{
			PUCHAR bootpHdr;
			UINT16 bootpFlag;	
			
			bootpHdr = udpHdr + 8;
			bootpFlag = ntohs(*((UINT16 *)(bootpHdr+10)));
			DBGPRINT(RT_DEBUG_TRACE,"is bootp packet! bootpFlag=0x%x\n", bootpFlag);	
			if (bootpFlag != 0x8000) //check if it's a broadcast request.
			{
				PUCHAR dhcpHdr;
				
				dhcpHdr = bootpHdr + 236;
				
				DBGPRINT(RT_DEBUG_TRACE,"the DHCP flag is a unicast, dhcp_magic=%02x:%02x:%02x:%02x\n", dhcpHdr[0], dhcpHdr[1], dhcpHdr[2], dhcpHdr[3]);
				if (NdisEqualMemory(dhcpHdr, DHCP_MAGIC, 4))
				{	
					DBGPRINT(RT_DEBUG_TRACE,"dhcp magic macthed!\n");	
					bootpFlag = htons(0x8000);
					memcpy((bootpHdr+10), &bootpFlag, 2);	//Set the bootp flag as broadcast
					memset((udpHdr+6), 0, 2); //modify the UDP chksum as zero
				}		 
			}	
		}
	}

	return NULL;
}

NDIS_STATUS APCLI_IP_init(VOID)
{
	BOOLEAN status = FALSE;

	status = (IPMacTable.valid==TRUE) ? TRUE : IPMacTable_init();

	if (status)
		apCliIPHandler.pMgtTb =&IPMacTable;
	else 
		apCliIPHandler.pMgtTb = NULL;
	
	return status;
}

#endif   // endif of "APCLI_SUPPORT"

