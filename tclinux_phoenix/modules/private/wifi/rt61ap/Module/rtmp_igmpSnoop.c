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
    rtmp_igmpSnoop.c

    Abstract:

    Revision History:
    Who         When            What
    --------    ----------      ----------------------------------------------
    Name        Date            Modification logs
    WY Juang    2007-07-03      patch to support IGMPv3
*/

#include "rt_config.h"
#include "rtmp_igmpSnoop.h"


spinlock_t MulticastFilterTabLock;
static MEMBER_ENTRY freeMemberPool[FREE_MEMBER_POOL_SIZE];
static LIST_HEADER freeEntryList;

static MEMBER_ENTRY freeSourcePool[FREE_SOURCE_POOL_SIZE];
static LIST_HEADER freeSourceList;

static VOID IGMPTableDisplay(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable);

static BOOLEAN isIgmpMacAddr(
	IN PUCHAR pMacAddr);

static VOID InsertIgmpMember(
	IN PLIST_HEADER pList,
	IN PUCHAR pAddr,
	IN IgmpGroupType IGMPType);

static VOID DeleteIgmpMember(
	IN PLIST_HEADER pList,
	IN PUCHAR pAddr);

static VOID DeleteIgmpMemberList(
	IN PLIST_HEADER pList);

/*
    ==========================================================================
    Description:
        This routine init the entire IGMP table.
    ==========================================================================
 */
VOID MulticastFilterTableInit(
	IN PMULTICAST_FILTER_TABLE *ppMulticastFilterTable)
{
    int i;
	// Initialize MAC table and allocate spin lock
	*ppMulticastFilterTable = kmalloc(sizeof(MULTICAST_FILTER_TABLE), MEM_ALLOC_FLAG);
	if (*ppMulticastFilterTable == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, "%s unable to alloc memory for Multicase filter table, size=%d\n",
			__FUNCTION__, sizeof(MULTICAST_FILTER_TABLE));
		return;
	}

	NdisZeroMemory(*ppMulticastFilterTable, sizeof(MULTICAST_FILTER_TABLE));
	NdisAllocateSpinLock(&MulticastFilterTabLock);
	initList(&freeEntryList);
	for (i = 0; i < FREE_MEMBER_POOL_SIZE; i++)
	  insertTailList(&freeEntryList, (PLIST_ENTRY)&freeMemberPool[i]);

	initList(&freeSourceList);
	for (i = 0; i < FREE_SOURCE_POOL_SIZE; i++)
	  insertTailList(&freeSourceList, (PLIST_ENTRY)&freeSourcePool[i]);
	return;
}

/*
    ==========================================================================
    Description:
        This routine reset the entire IGMP table.
    ==========================================================================
 */
VOID MultiCastFilterTableReset(
	IN PMULTICAST_FILTER_TABLE *ppMulticastFilterTable)
{
	UMAXLONG IrqFlags;

	if(*ppMulticastFilterTable == NULL)
		return;

#ifndef THREAD_ISR
	RTMP_SEM_LOCK(&MulticastFilterTabLock, IrqFlags);
#endif
    
#ifdef THREAD_ISR
	RTMP_IRQ_LOCK(IrqFlags);
#endif
	kfree(*ppMulticastFilterTable);
	*ppMulticastFilterTable = NULL;
#ifdef THREAD_ISR
	RTMP_IRQ_UNLOCK(IrqFlags);
#endif	

#ifndef THREAD_ISR
	RTMP_SEM_UNLOCK(&MulticastFilterTabLock, IrqFlags);
#endif
}

void DeleteIgmpMember_SourceList(
	IN PLIST_HEADER pList)
{
	PMEMBER_ENTRY pCurEntry, pPrvEntry;

	if((pList == NULL) || (pList->pHead == NULL))
		return;

	pPrvEntry = pCurEntry = (PMEMBER_ENTRY)pList->pHead;
	while (pCurEntry)
	{
		delEntryList(pList, (PLIST_ENTRY)pCurEntry);
		pPrvEntry = pCurEntry;
		pCurEntry = pCurEntry->pNext;
		insertTailList(&freeSourceList, (PLIST_ENTRY)pPrvEntry);
	}

	initList(pList);
	return;
}


void InsertIgmpMember_Source(
	IN PLIST_HEADER pList,IN PUCHAR pSourceAddr)
{
 PSOURCE_ENTRY pSourceEntry=(PSOURCE_ENTRY)pList->pHead;
 DBGPRINT(RT_DEBUG_TRACE, "==>InsertIgmpMember_Source\n");

 while(pSourceEntry)
 {
    if(IPV4_ADDR_EQUAL(pSourceAddr,pSourceEntry->Addr))
	 return;
  pSourceEntry=pSourceEntry->pNext;
 }

 if((pSourceEntry = (PSOURCE_ENTRY)removeHeadList(&freeSourceList)) != NULL)
 {
			NdisZeroMemory(pSourceEntry, sizeof(SOURCE_ENTRY));
			COPY_IPV4_ADDR(pSourceEntry->Addr, pSourceAddr);
			insertTailList((PLIST_HEADER)pList,(PLIST_ENTRY)pSourceEntry);
        
	DBGPRINT(RT_DEBUG_TRACE, "IGMPv3 add_Source addIP=(%02x:%02x:%02x:%02x)\n",pSourceEntry->Addr[0], pSourceEntry->Addr[1], pSourceEntry->Addr[2],pSourceEntry->Addr[3]);
			return; 
 }

}

void DeleteIgmpMember_Source(
	IN PLIST_HEADER pList,IN PUCHAR pSourceAddr)
{
 	PSOURCE_ENTRY pSourceEntry=(PSOURCE_ENTRY)pList->pHead,pPrvEntry;
 	while(pSourceEntry)
 	{
    		if(IPV4_ADDR_EQUAL(pSourceAddr,pSourceEntry->Addr))
		{
	  		delEntryList(pList, (PLIST_ENTRY)pSourceEntry);
	 		 pPrvEntry = pSourceEntry;
	  		pSourceEntry = pSourceEntry->pNext;
	  		insertTailList(&freeSourceList, (PLIST_ENTRY)pPrvEntry);	 
	  		return;
    		}
		pSourceEntry=pSourceEntry->pNext;
	}
}


BOOLEAN InsertIgmpMember_Sources(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable,
	IN PUCHAR pGrpId,
	IN PUCHAR pMemberAddr,
	IN PUCHAR pSourceAddr,
	IN UCHAR numOfSources,
	IN UCHAR GroupType,
	IN PNET_DEV dev)
{
	int i;
	MULTICAST_FILTER_TABLE_ENTRY *pEntry = NULL;
	PMEMBER_ENTRY pMemberEntry;
	unsigned long IrqFlags;

	if (pMulticastFilterTable == NULL || pMemberAddr==NULL || pSourceAddr==NULL || pGrpId==NULL)
		return FALSE;

    //check is the multicast address in table or not
    if ((pEntry = MulticastFilterTableLookup(pMulticastFilterTable, pGrpId, dev))==NULL)
	    return FALSE;

	//printk(KERN_EMERG "start finding member \n");
    
	RTMP_SEM_LOCK(&MulticastFilterTabLock, IrqFlags);
    pMemberEntry = (PMEMBER_ENTRY)pEntry->MemberList.pHead;

	while (pMemberEntry)
	{
		if (MAC_ADDR_EQUAL(pMemberAddr, pMemberEntry->Addr))
		{
			if(pMemberEntry->type==IGMPV1V2)
			{	RTMP_SEM_UNLOCK(&MulticastFilterTabLock, IrqFlags);
				return FALSE;
			}
			
         		if(GroupType==ALLOW_NEW_SOURCES)
        		 {
           			DBGPRINT(RT_DEBUG_TRACE, "Mode is %d ,ALLOW_NEW_SOURCES:%d\n",pMemberEntry->type,numOfSources);

	   		       if(pMemberEntry->type==MODE_IS_EXCLUDE)
	    	   		{
		    			for(i=0;i<numOfSources;i++)
		  			{
		     				DeleteIgmpMember_Source(&pMemberEntry->SourceList,pSourceAddr);
			 			pSourceAddr+=IPV4_ADDR_LEN;		
						if(pMemberEntry->SourceList.size==0)
						{	RTMP_SEM_UNLOCK(&MulticastFilterTabLock, IrqFlags);
							MulticastFilterTableDeleteEntry(pMulticastFilterTable,pGrpId,pMemberAddr,dev);
							return TRUE;
						}
		   			 }
		   		}
		 		 //run out of SourceList entry , change IPV3 -> IPV2
        	  		else if(freeSourceList.size<numOfSources)
  	         		{
 		           		pMemberEntry->type=IGMPV1V2;
			    		DeleteIgmpMember_SourceList(&pMemberEntry->SourceList);
					DBGPRINT(RT_DEBUG_TRACE, "run out of SourceList Entry \n");
      		   		}
		   		else if(pMemberEntry->type==MODE_IS_INCLUDE)
		  		{
		    			for(i=0;i<numOfSources;i++)
		    			{
		     				InsertIgmpMember_Source(&pMemberEntry->SourceList,pSourceAddr);
			 			pSourceAddr+=IPV4_ADDR_LEN;
		   			 }
		   		} 
	
		   	RTMP_SEM_UNLOCK(&MulticastFilterTabLock, IrqFlags);
		   	return TRUE;
        	 }	
		 else if(GroupType==BLOCK_OLD_SOURCES)
	        {
         		DBGPRINT(RT_DEBUG_TRACE, "Mode is %d ,BLOCK_OLD_SOURCES:%d\n",pMemberEntry->type,numOfSources);

	  		if(pMemberEntry->type==MODE_IS_INCLUDE)
	             		for(i=0;i<numOfSources;i++)
				{
		      			DeleteIgmpMember_Source(&pMemberEntry->SourceList,pSourceAddr);
		      			pSourceAddr+=IPV4_ADDR_LEN;
						if(pMemberEntry->SourceList.size==0)
						{	RTMP_SEM_UNLOCK(&MulticastFilterTabLock, IrqFlags);
							MulticastFilterTableDeleteEntry(pMulticastFilterTable,pGrpId,pMemberAddr,dev);
							return TRUE;
						}
		     		}
        	  	else if(freeSourceList.size<numOfSources)
  	         	{
 		           		pMemberEntry->type=IGMPV1V2;
			    		DeleteIgmpMember_SourceList(&pMemberEntry->SourceList);
					DBGPRINT(RT_DEBUG_TRACE, "run out of SourceList Entry \n");
      		   	}
			else if(pMemberEntry->type==MODE_IS_EXCLUDE)
             			for(i=0;i<numOfSources;i++)
		     		{
		      			InsertIgmpMember_Source(&pMemberEntry->SourceList,pSourceAddr);
		      			pSourceAddr+=IPV4_ADDR_LEN;
		     		}
			RTMP_SEM_UNLOCK(&MulticastFilterTabLock, IrqFlags);
			return TRUE;
		 }
		 else if(freeSourceList.size+pMemberEntry->SourceList.size<numOfSources)
		 {//run out of SourceList entry , change IPV3 -> IPV2
          		pMemberEntry->type=IGMPV1V2;
		  	DeleteIgmpMember_SourceList(&pMemberEntry->SourceList);
		  	DBGPRINT(RT_DEBUG_TRACE, "run out of SourceList Entry \n");
		  	RTMP_SEM_UNLOCK(&MulticastFilterTabLock, IrqFlags);
		  	return FALSE;
		 }
	        else if((GroupType==CHANGE_TO_EXCLUDE_MODE) || (GroupType==MODE_IS_EXCLUDE))
         	{
          		DBGPRINT(RT_DEBUG_TRACE, "MODE_IS_EXCLUDE:%d\n",numOfSources);
          		pMemberEntry->type=MODE_IS_EXCLUDE;
         	}
         	else if((GroupType==CHANGE_TO_INCLUDE_MODE) || (GroupType==MODE_IS_INCLUDE))
         	{
          		pMemberEntry->type=MODE_IS_INCLUDE;
		  	DBGPRINT(RT_DEBUG_TRACE, "Mode is include:%d\n",numOfSources);
         	}
         
        	 DeleteIgmpMember_SourceList(&pMemberEntry->SourceList);
		 for(i=0;i<numOfSources;i++)
		 {	
		     InsertIgmpMember_Source(&pMemberEntry->SourceList,pSourceAddr);
			 pSourceAddr+=IPV4_ADDR_LEN;
		 }	 
		}
	  pMemberEntry = pMemberEntry->pNext;
	}

 RTMP_SEM_UNLOCK(&MulticastFilterTabLock, IrqFlags);
 return TRUE;
}


/*
    ==========================================================================
    Description:
        Display all entrys in IGMP table
    ==========================================================================
 */
static VOID IGMPTableDisplay(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable)
{
	int i;
	MULTICAST_FILTER_TABLE_ENTRY *pEntry = NULL;
	unsigned long IrqFlags;

	if (pMulticastFilterTable == NULL)
		return;

	// if FULL, return
	if (pMulticastFilterTable->Size == 0)
	{
		DBGPRINT(RT_DEBUG_ERROR, "Multicast Table empty. freeMember:%d freeSource:%d\n",freeEntryList.size,freeSourceList.size);
		return;
	}

	DBGPRINT(RT_DEBUG_ERROR, "Multicast Table Info FreeMember:%d FreeSource:%d\n",freeEntryList.size,freeSourceList.size);
    
	// allocate one MAC entry
	RTMP_SEM_LOCK(&MulticastFilterTabLock, IrqFlags);

	for (i = 0; i< MAX_LEN_OF_MULTICAST_FILTER_TABLE; i++)
	{
		// pick up the first available vacancy
		if (pMulticastFilterTable->Content[i].Valid == TRUE)
		{
			PMEMBER_ENTRY pMemberEntry = NULL;
			PSOURCE_ENTRY pSourceEntry = NULL;
			pEntry = &pMulticastFilterTable->Content[i];

			DBGPRINT(RT_DEBUG_ERROR, "IF(%s) entry #%d, type=%7s, GrpId=(%02x:%02x:%02x:%02x:%02x:%02x) memberCnt=%d\n",
				pEntry->net_dev->name, i, (pEntry->type==0 ? "static":"dynamic"),
				pEntry->Addr[0], pEntry->Addr[1], pEntry->Addr[2],
				pEntry->Addr[3], pEntry->Addr[4], pEntry->Addr[5],
				IgmpMemberCnt(&pEntry->MemberList));

			pMemberEntry = (PMEMBER_ENTRY)pEntry->MemberList.pHead;
			while (pMemberEntry)
			{
					DBGPRINT(RT_DEBUG_ERROR, "		member type=%d mac=(%02x:%02x:%02x:%02x:%02x:%02x) SourceList:%d\n",pMemberEntry->type,
					pMemberEntry->Addr[0], pMemberEntry->Addr[1], pMemberEntry->Addr[2],
					pMemberEntry->Addr[3], pMemberEntry->Addr[4], pMemberEntry->Addr[5],pMemberEntry->SourceList.size);

					pSourceEntry=(PSOURCE_ENTRY)pMemberEntry->SourceList.pHead;
                			while(pSourceEntry)
                			{
				   		DBGPRINT(RT_DEBUG_ERROR, "			Source IP=(%02x:%02x:%02x:%02x)\n",
						pSourceEntry->Addr[0], pSourceEntry->Addr[1], pSourceEntry->Addr[2],
						pSourceEntry->Addr[3]);       
						pSourceEntry=pSourceEntry->pNext;
               			 }

				pMemberEntry = pMemberEntry->pNext;
			}
		}
	}

	RTMP_SEM_UNLOCK(&MulticastFilterTabLock, IrqFlags);

	return;
}

/*
    ==========================================================================
    Description:
        Add and new entry into MAC table
    ==========================================================================
 */
BOOLEAN MulticastFilterTableInsertEntry(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable,
	IN PUCHAR pGrpId,
	IN PUCHAR pMemberAddr,
	IN PNET_DEV dev,
	IN MulticastFilterEntryType type,
	IN IgmpGroupType IGMPType)
{
	UCHAR HashIdx;
	int i;
	MULTICAST_FILTER_TABLE_ENTRY *pEntry = NULL, *pCurrEntry, *pPrevEntry;
	PMEMBER_ENTRY pMemberEntry;
	unsigned long IrqFlags;

	if (pMulticastFilterTable == NULL)
		return FALSE;

	// if FULL, return
	if (pMulticastFilterTable->Size >= MAX_LEN_OF_MULTICAST_FILTER_TABLE)
		return FALSE;

	// check the rule is in table already or not.
	if ((pEntry = MulticastFilterTableLookup(pMulticastFilterTable, pGrpId, dev)))
	{
		// doesn't indicate member mac address.
		if(pMemberAddr == NULL)
			return FALSE;

		pMemberEntry = (PMEMBER_ENTRY)pEntry->MemberList.pHead;
		while (pMemberEntry)
		{
			if (MAC_ADDR_EQUAL(pMemberAddr, pMemberEntry->Addr))
				return FALSE;
			pMemberEntry = pMemberEntry->pNext;
		}
	}

	RTMP_SEM_LOCK(&MulticastFilterTabLock, IrqFlags);

	do
	{
		// the multicast entry already exist but doesn't include the member yet.
		if (pEntry != NULL)
		{
			InsertIgmpMember(&pEntry->MemberList, pMemberAddr,IGMPType);
			break;
		}

		// allocate one MAC entry
		for (i = 0; i < MAX_LEN_OF_MULTICAST_FILTER_TABLE; i++)
		{
			// pick up the first available vacancy
			pEntry = &pMulticastFilterTable->Content[i];
			if ((pEntry->Valid == TRUE) && (pEntry->type == MCAT_FILTER_DYNAMIC)
				&& ((jiffies - pEntry->lastTime) > IGMPMAC_TB_ENTRY_AGEOUT_TIME))
			{
				PMULTICAST_FILTER_TABLE_ENTRY pHashEntry;

				HashIdx = MULTICAST_ADDR_HASH_INDEX(pEntry->Addr);
				pHashEntry = pMulticastFilterTable->Hash[HashIdx];

				if ((pEntry->net_dev == pHashEntry->net_dev)
					&& MAC_ADDR_EQUAL(pEntry->Addr, pHashEntry->Addr))
				{
					pMulticastFilterTable->Hash[HashIdx] = pHashEntry->pNext;
					pMulticastFilterTable->Size --;
					DBGPRINT(RT_DEBUG_TRACE, "MCastFilterTableDeleteEntry 1 - Total= %d - AGEOUT\n ", pMulticastFilterTable->Size);
				} else
				{
					while (pHashEntry->pNext)
					{
						pPrevEntry = pHashEntry;
						pHashEntry = pHashEntry->pNext;
						if ((pEntry->net_dev == pHashEntry->net_dev)
							&& MAC_ADDR_EQUAL(pEntry->Addr, pHashEntry->Addr))
						{
							pPrevEntry->pNext = pHashEntry->pNext;
							pMulticastFilterTable->Size --;
							DBGPRINT(RT_DEBUG_TRACE, "MCastFilterTableDeleteEntry 2 - Total= %d - AGEOUT\n ", pMulticastFilterTable->Size);
							break;
						}
					}
				}
				pEntry->Valid = FALSE;
				DeleteIgmpMemberList(&pEntry->MemberList);
			}

			if (pEntry->Valid == FALSE)
			{
				NdisZeroMemory(pEntry, sizeof(MULTICAST_FILTER_TABLE_ENTRY));
				pEntry->Valid = TRUE;

				COPY_MAC_ADDR(pEntry->Addr, pGrpId);
				pEntry->net_dev = dev;
				pEntry->lastTime = jiffies;
				pEntry->type = type;
				initList(&pEntry->MemberList);
				InsertIgmpMember(&pEntry->MemberList, pMemberAddr,IGMPType);
		
				pMulticastFilterTable->Size ++;

				DBGPRINT(RT_DEBUG_TRACE, "MulticastFilterTableInsertEntry -IF(%s) allocate entry #%d, Total= %d\n", dev->name, i, pMulticastFilterTable->Size);
				break;
			}
		}

		// add this MAC entry into HASH table
		if (pEntry)
		{
			HashIdx = MULTICAST_ADDR_HASH_INDEX(pGrpId);
			if (pMulticastFilterTable->Hash[HashIdx] == NULL)
			{
				pMulticastFilterTable->Hash[HashIdx] = pEntry;
			} else
			{
				pCurrEntry = pMulticastFilterTable->Hash[HashIdx];
				while (pCurrEntry->pNext != NULL)
					pCurrEntry = pCurrEntry->pNext;
				pCurrEntry->pNext = pEntry;
			}
		}
	}while(FALSE);

	RTMP_SEM_UNLOCK(&MulticastFilterTabLock, IrqFlags);

	return TRUE;
}

/*
    ==========================================================================
    Description:
        Delete a specified client from MAC table
    ==========================================================================
 */
BOOLEAN MulticastFilterTableDeleteEntry(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable,
	IN PUCHAR pGrpId,
	IN PUCHAR pMemberAddr,
	IN PNET_DEV dev)
{
	USHORT HashIdx;
	MULTICAST_FILTER_TABLE_ENTRY *pEntry, *pPrevEntry;
	unsigned long IrqFlags;

	if (pMulticastFilterTable == NULL)
		return FALSE;

	RTMP_SEM_LOCK(&MulticastFilterTabLock, IrqFlags);

	do
	{
		HashIdx = MULTICAST_ADDR_HASH_INDEX(pGrpId);
		pEntry = pMulticastFilterTable->Hash[HashIdx];

		// check the rule is in table already or not.
		if (pEntry && (pMemberAddr != NULL))
		{
			DeleteIgmpMember(&pEntry->MemberList, pMemberAddr);
			if (IgmpMemberCnt(&pEntry->MemberList) > 0)
				break;
		}

		if (pEntry)
		{
			if ((dev == pEntry->net_dev)
				&& MAC_ADDR_EQUAL(pEntry->Addr, pGrpId))
			{
				pMulticastFilterTable->Hash[HashIdx] = pEntry->pNext;
				DeleteIgmpMemberList(&pEntry->MemberList);
				NdisZeroMemory(pEntry, sizeof(MULTICAST_FILTER_TABLE_ENTRY));
				pMulticastFilterTable->Size --;
				DBGPRINT(RT_DEBUG_TRACE, "MCastFilterTableDeleteEntry 1 - Total= %d\n", pMulticastFilterTable->Size);
			} else
			{
				while (pEntry->pNext)
				{
					pPrevEntry = pEntry;
					pEntry = pEntry->pNext;
					if ((dev == pEntry->net_dev)
						&& MAC_ADDR_EQUAL(pEntry->Addr, pGrpId))
					{
						pPrevEntry->pNext = pEntry->pNext;
						DeleteIgmpMemberList(&pEntry->MemberList);
						NdisZeroMemory(pEntry, sizeof(MULTICAST_FILTER_TABLE_ENTRY));
						pMulticastFilterTable->Size --;
						DBGPRINT(RT_DEBUG_TRACE, "MCastFilterTableDeleteEntry 2 - Total= %d\n", pMulticastFilterTable->Size);
						break;
					}
				}
			}
		}
	} while(FALSE);

	RTMP_SEM_UNLOCK(&MulticastFilterTabLock, IrqFlags);
    
	return TRUE;
}

/*
    ==========================================================================
    Description:
        Update MulticastFilter Table
        if type == MCAT_FILTER_STATIC , del all MCAT_FILTER_STATIC entry
           type == MCAT_FILTER_LLC , del all MCAT_FILTER_STATIC entry
           type == MCAT_FILTER_DYNAMIC , del all (MCAT_FILTER_DYNAMIC && aged) entry
    ==========================================================================
 */
void MulticastFilterTableUpdate(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable,
	IN MulticastFilterEntryType type)
{
	UCHAR HashIdx;
	int i;
	MULTICAST_FILTER_TABLE_ENTRY *pEntry = NULL,  *pPrevEntry;
	unsigned long IrqFlags;

	if (pMulticastFilterTable == NULL)
		return ;

       RTMP_SEM_LOCK(&MulticastFilterTabLock, IrqFlags);
	   
	for (i = 0; i < MAX_LEN_OF_MULTICAST_FILTER_TABLE; i++)
	{
        	pEntry = &pMulticastFilterTable->Content[i];
              if((pEntry->Valid == TRUE) && (pEntry->type == type) &&
		    ((type==MCAT_FILTER_STATIC) || (type==MCAT_FILTER_LLC) ||(type==MCAT_FILTER_DYNAMIC && ((jiffies - pEntry->lastTime) > IGMPMAC_TB_ENTRY_AGEOUT_TIME))))
		{
				PMULTICAST_FILTER_TABLE_ENTRY pHashEntry;

				HashIdx = MULTICAST_ADDR_HASH_INDEX(pEntry->Addr);
				pHashEntry = pMulticastFilterTable->Hash[HashIdx];

				if ((pEntry->net_dev == pHashEntry->net_dev)
					&& MAC_ADDR_EQUAL(pEntry->Addr, pHashEntry->Addr))
				{
					pMulticastFilterTable->Hash[HashIdx] = pHashEntry->pNext;
					pMulticastFilterTable->Size --;
					DBGPRINT(RT_DEBUG_TRACE, "MCastFilterTableDeleteEntry 1 - Total= %d\n", pMulticastFilterTable->Size);
				} else
				{
					while (pHashEntry->pNext)
					{
						pPrevEntry = pHashEntry;
						pHashEntry = pHashEntry->pNext;
						if ((pEntry->net_dev == pHashEntry->net_dev)
							&& MAC_ADDR_EQUAL(pEntry->Addr, pHashEntry->Addr))
						{
							pPrevEntry->pNext = pHashEntry->pNext;
							pMulticastFilterTable->Size --;
							DBGPRINT(RT_DEBUG_TRACE, "MCastFilterTableDeleteEntry 2 - Total= %d\n", pMulticastFilterTable->Size);
							break;
						}
					}
				}
				pEntry->Valid = FALSE;
				DeleteIgmpMemberList(&pEntry->MemberList);
		}
	}

	RTMP_SEM_UNLOCK(&MulticastFilterTabLock, IrqFlags);
}

/*
    ==========================================================================
    Description:
        Look up the MAC address in the IGMP table. Return NULL if not found.
    Return:
        pEntry - pointer to the MAC entry; NULL is not found
    ==========================================================================
*/
PMULTICAST_FILTER_TABLE_ENTRY MulticastFilterTableLookup(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable,
	IN PUCHAR pAddr,
	IN PNET_DEV dev)
{
	ULONG HashIdx;
	PMULTICAST_FILTER_TABLE_ENTRY pEntry = NULL, pPrev = NULL;


	if (pMulticastFilterTable == NULL)
		return NULL;

	HashIdx = MULTICAST_ADDR_HASH_INDEX(pAddr);
	pEntry = pPrev = pMulticastFilterTable->Hash[HashIdx];

	while (pEntry && pEntry->Valid)
	{

		if ((pEntry->net_dev ==  dev)
			&& MAC_ADDR_EQUAL(pEntry->Addr, pAddr))
		{
			pEntry->lastTime = jiffies;
			break;
		}
		else
		{
			if ((pEntry->Valid == TRUE) && (pEntry->type == MCAT_FILTER_DYNAMIC)
				&& ((jiffies - pEntry->lastTime) > IGMPMAC_TB_ENTRY_AGEOUT_TIME))
			{
				// Remove the aged entry
				if (pEntry == pMulticastFilterTable->Hash[HashIdx])
				{
					pMulticastFilterTable->Hash[HashIdx] = pEntry->pNext;
					pPrev = pMulticastFilterTable->Hash[HashIdx];
				}
				else 
				{	
					pPrev->pNext = pEntry->pNext;
				}

				NdisZeroMemory(pEntry, sizeof(MULTICAST_FILTER_TABLE_ENTRY));
				pMulticastFilterTable->Size --;
				pEntry = (pPrev == NULL ? NULL: pPrev->pNext);
				DBGPRINT(RT_DEBUG_TRACE, "MCastFilterTableDeleteEntry 2 - Total= %d - AGEOUT\n", pMulticastFilterTable->Size);
			}
			else
			{
				pPrev = pEntry;
				pEntry = pEntry->pNext;
			}
		}
	}

	return pEntry;
}

VOID IGMPSnooping(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable,
	IN PUCHAR pDstMacAddr,
	IN PUCHAR pSrcMacAddr,
	IN PUCHAR pIpHeader,
	IN PNET_DEV pDev)
{
	INT i;
	INT IpHeaderLen;
	UCHAR GroupType;
	UINT16 numOfGroup;
	UCHAR IgmpVerType;
	PUCHAR pIgmpHeader;
	PUCHAR pGroup;
	UCHAR AuxDataLen;
	UINT16 numOfSources;
	PUCHAR pGroupIpAddr;
	UCHAR GroupMacAddr[6];
	PUCHAR pGroupMacAddr = (PUCHAR)&GroupMacAddr;

	if(isIgmpPkt(pDstMacAddr, pIpHeader))
	{
		IpHeaderLen = (*(pIpHeader + 2) & 0x0f) * 4;
		pIgmpHeader = pIpHeader + 2 + IpHeaderLen;
		IgmpVerType = (UCHAR)(*(pIgmpHeader));

		DBGPRINT(RT_DEBUG_TRACE, "IGMP type=%0x\n", IgmpVerType);

		switch(IgmpVerType)
		{
		case IGMP_V1_MEMBERSHIP_REPORT: // IGMP version 1 membership report.
		case IGMP_V2_MEMBERSHIP_REPORT: // IGMP version 2 membership report.
			pGroupIpAddr = (PUCHAR)(pIgmpHeader + 4);
			ConvertMulticastIP2MAC(pGroupIpAddr, (PUCHAR *)&pGroupMacAddr);
			DBGPRINT(RT_DEBUG_TRACE, "IGMP Group=%02x:%02x:%02x:%02x:%02x:%02x\n",
				GroupMacAddr[0], GroupMacAddr[1], GroupMacAddr[2], GroupMacAddr[3], GroupMacAddr[4], GroupMacAddr[5]);
			MulticastFilterTableInsertEntry(pMulticastFilterTable, GroupMacAddr, pSrcMacAddr, pDev, MCAT_FILTER_DYNAMIC,IGMPV1V2);
			break;

		case IGMP_LEAVE_GROUP: // IGMP version 1 and version 2 leave group.
			pGroupIpAddr = (PUCHAR)(pIgmpHeader + 4);
			ConvertMulticastIP2MAC(pGroupIpAddr, (PUCHAR *)&pGroupMacAddr);
			DBGPRINT(RT_DEBUG_TRACE, "IGMP Group=%02x:%02x:%02x:%02x:%02x:%02x\n",
				GroupMacAddr[0], GroupMacAddr[1], GroupMacAddr[2], GroupMacAddr[3], GroupMacAddr[4], GroupMacAddr[5]);
			MulticastFilterTableDeleteEntry(pMulticastFilterTable, GroupMacAddr, pSrcMacAddr, pDev);
			break;

		case IGMP_V3_MEMBERSHIP_REPORT: // IGMP version 3 membership report.
			numOfGroup = ntohs(*((UINT16 *)(pIgmpHeader + 6)));
			pGroup = (PUCHAR)(pIgmpHeader + 8);
			DBGPRINT(RT_DEBUG_TRACE, "Rev IGMPv3 Packet NumOfGroup:%d \n",numOfGroup);
			for (i=0; i < numOfGroup; i++)
			{
				GroupType = (UCHAR)(*pGroup);
				AuxDataLen = (UCHAR)(*(pGroup + 1));
				numOfSources = ntohs(*((UINT16 *)(pGroup + 2)));
				pGroupIpAddr = (PUCHAR)(pGroup + 4);
				ConvertMulticastIP2MAC(pGroupIpAddr, (PUCHAR *)&pGroupMacAddr);
				DBGPRINT(RT_DEBUG_TRACE, "IGMPv3 Type=%d, ADL=%d, numOfSource=%d Group=%02x:%02x:%02x:%02x:%02x:%02x \n", GroupType, AuxDataLen, numOfSources,GroupMacAddr[0], GroupMacAddr[1], GroupMacAddr[2], GroupMacAddr[3], GroupMacAddr[4], GroupMacAddr[5]);
          		      if( (numOfSources == 0) &&  ((GroupType == CHANGE_TO_INCLUDE_MODE) || (GroupType == MODE_IS_INCLUDE)) )
          		      {
          		        	MulticastFilterTableDeleteEntry(pMulticastFilterTable, GroupMacAddr, pSrcMacAddr, pDev);
						    break;
         		       }
				else   	
				{
					MulticastFilterTableInsertEntry(pMulticastFilterTable, GroupMacAddr, pSrcMacAddr, pDev, MCAT_FILTER_DYNAMIC,MODE_IS_INCLUDE);
					if (numOfSources>0)
					 	InsertIgmpMember_Sources(pMulticastFilterTable, GroupMacAddr, pSrcMacAddr,pGroup+8,numOfSources,GroupType,pDev);
					break;
				}
				
				pGroup += (8 + (numOfSources * 4) + AuxDataLen);
			}
			IGMPTableDisplay(pMulticastFilterTable);
			break;

		default:
			DBGPRINT(RT_DEBUG_TRACE, "unknow IGMP Type=%d\n", IgmpVerType);
			break;
		}
	}

	return;
}

#if 0 // Doesn't be used.
BOOLEAN MulticastFilterApply(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable,
	IN PUCHAR pDstMacAddr,
	IN PNET_DEV pDev)
{
	if(isIgmpMacAddr(pDstMacAddr))
		return FALSE;

	return MulticastFilterTableLookup(pMulticastFilterTable, pDstMacAddr, pDev) ? FALSE : TRUE;
}
#endif

INT Set_IgmpSn_Enable_Proc(
	IN PRTMP_ADAPTER pAd,
	IN PUCHAR arg)
{
	UINT Enable;
	PNET_DEV pDev;

	pDev = (pAd->IoctlIF == MAIN_MBSSID) ? (pAd->net_dev) : (pAd->PortCfg.MBSSID[pAd->IoctlIF].MSSIDDev);
	Enable = simple_strtol(arg, 0, 10);

	pAd->PortCfg.MBSSID[pAd->IoctlIF].IgmpSnoopEnable = (BOOLEAN)(Enable == 0 ? 0 : 1);
	DBGPRINT(RT_DEBUG_TRACE, "%s::(%s) %s\n", __FUNCTION__, pDev->name, Enable == TRUE ? "Enable IGMP Snooping":"Disable IGMP Snooping");

	return TRUE;
}

INT Set_IgmpSn_AddEntry_Proc(
	IN PRTMP_ADAPTER pAd, 
	IN PUCHAR arg)
{
	INT i;
	BOOLEAN bGroupId = 1;
	PUCHAR value;
	PUCHAR thisChar;
	UCHAR IpAddr[4];
	UCHAR Addr[ETH_LENGTH_OF_ADDRESS];
	UCHAR GroupId[ETH_LENGTH_OF_ADDRESS];
	PUCHAR *pAddr = (PUCHAR *)&Addr;
	PNET_DEV pDev;

	pDev = (pAd->IoctlIF == MAIN_MBSSID) ? (pAd->net_dev) : (pAd->PortCfg.MBSSID[pAd->IoctlIF].MSSIDDev);

	while ((thisChar = strsep((char **)&arg, "-")) != NULL)
	{
		// refuse the Member if it's not a MAC address.
		if((bGroupId == 0) && (strlen(thisChar) != 17))
			continue;

		if(strlen(thisChar) == 17)  //Mac address acceptable format 01:02:03:04:05:06 length 17
		{
			for (i=0, value = rstrtok(thisChar,":"); value; value = rstrtok(NULL,":"))
			{
				if((strlen(value) != 2) || (!isxdigit(*value)) || (!isxdigit(*(value+1))) ) 
					return FALSE;  //Invalid

				AtoH(value, &Addr[i++], 2);
			}

			if(i != 6)
				return FALSE;  //Invalid
		}
		else
		{
			for (i=0, value = rstrtok(thisChar,"."); value; value = rstrtok(NULL,".")) 
			{
				if((strlen(value) > 0) && (strlen(value) <= 3)) 
				{
					int ii;
					for(ii=0; ii<strlen(value); ii++)
						if (!isxdigit(*(value + ii)))
							return FALSE;
				}
				else
					return FALSE;  //Invalid

				IpAddr[i] = (UCHAR)simple_strtol(value, NULL, 10);
				i++;
			}

			if(i != 4)
				return FALSE;  //Invalid

			ConvertMulticastIP2MAC(IpAddr, (PUCHAR *)&pAddr);
		}

		if(bGroupId == 1)
			COPY_MAC_ADDR(GroupId, Addr);

		// Group-Id must be a MCAST address.
		if((bGroupId == 1) && IS_MULTICAST_MAC_ADDR(Addr))
			MulticastFilterTableInsertEntry(pAd->pMulticastFilterTable, GroupId, NULL, pDev, MCAT_FILTER_STATIC,IGMPV1V2);
		// Group-Member must be a UCAST address.
		else if ((bGroupId == 0) && !IS_MULTICAST_MAC_ADDR(Addr))
			MulticastFilterTableInsertEntry(pAd->pMulticastFilterTable, GroupId, Addr, pDev, MCAT_FILTER_STATIC,IGMPV1V2);
		else
		{
			DBGPRINT(RT_DEBUG_TRACE, "%s (%2X:%2X:%2X:%2X:%2X:%2X) is not a acceptable address.\n",
				__FUNCTION__, Addr[0], Addr[1], Addr[2], Addr[3], Addr[4], Addr[5]);
			return FALSE;
		}

		bGroupId = 0;
		DBGPRINT(RT_DEBUG_TRACE, "%s (%2X:%2X:%2X:%2X:%2X:%2X)\n",
			__FUNCTION__, Addr[0], Addr[1], Addr[2], Addr[3], Addr[4], Addr[5]);

	}

	return TRUE;
}

INT Set_IgmpSn_DelEntry_Proc(
	IN PRTMP_ADAPTER pAd, 
	IN PUCHAR arg)
{
	INT i, memberCnt = 0;
	BOOLEAN bGroupId = 1;
	PUCHAR value;
	PUCHAR thisChar;
	UCHAR IpAddr[4];
	UCHAR Addr[ETH_LENGTH_OF_ADDRESS];
	UCHAR GroupId[ETH_LENGTH_OF_ADDRESS];
	PUCHAR *pAddr = (PUCHAR *)&Addr;
	PNET_DEV pDev;

	pDev = (pAd->IoctlIF == MAIN_MBSSID) ? (pAd->net_dev) : (pAd->PortCfg.MBSSID[pAd->IoctlIF].MSSIDDev);

	while ((thisChar = strsep((char **)&arg, "-")) != NULL)
	{
		// refuse the Member if it's not a MAC address.
		if((bGroupId == 0) && (strlen(thisChar) != 17))
			continue;

		if(strlen(thisChar) == 17)  //Mac address acceptable format 01:02:03:04:05:06 length 17
		{
			for (i=0, value = rstrtok(thisChar,":"); value; value = rstrtok(NULL,":")) 
			{
				if((strlen(value) != 2) || (!isxdigit(*value)) || (!isxdigit(*(value+1))) ) 
					return FALSE;  //Invalid

				AtoH(value, &Addr[i++], 2);
			}

			if(i != 6)
				return FALSE;  //Invalid
		}
		else
		{
			for (i=0, value = rstrtok(thisChar,"."); value; value = rstrtok(NULL,".")) 
			{
				if((strlen(value) > 0) && (strlen(value) <= 3)) 
				{
					int ii;
					for(ii=0; ii<strlen(value); ii++)
						if (!isxdigit(*(value + ii)))
							return FALSE;
				}
				else
					return FALSE;  //Invalid

				IpAddr[i] = (UCHAR)simple_strtol(value, NULL, 10);
				i++;
			}

			if(i != 4)
				return FALSE;  //Invalid

			ConvertMulticastIP2MAC(IpAddr, (PUCHAR *)&pAddr);
		}

		if(bGroupId == 1)
			COPY_MAC_ADDR(GroupId, Addr);
		else
			memberCnt++;

		if (memberCnt > 0 )
			MulticastFilterTableDeleteEntry(pAd->pMulticastFilterTable, (PUCHAR)GroupId, Addr, pDev);

		bGroupId = 0;
	}

	if(memberCnt == 0)
		MulticastFilterTableDeleteEntry(pAd->pMulticastFilterTable, (PUCHAR)GroupId, NULL, pDev);

	DBGPRINT(RT_DEBUG_TRACE, "%s (%2X:%2X:%2X:%2X:%2X:%2X)\n",
		__FUNCTION__, Addr[0], Addr[1], Addr[2], Addr[3], Addr[4], Addr[5]);

	return TRUE;
}

INT Set_IgmpSn_TabDisplay_Proc(
	IN PRTMP_ADAPTER pAd, 
	IN PUCHAR arg)
{
	IGMPTableDisplay(pAd->pMulticastFilterTable);
	return TRUE;
}

VOID ConvertMulticastIP2MAC(
	IN PUCHAR pIpAddr,
	IN PUCHAR *ppMacAddr)
{
	if(pIpAddr == NULL)
		return;

	if(ppMacAddr == NULL || *ppMacAddr == NULL)
		return;

	memset(*ppMacAddr, 0, ETH_LENGTH_OF_ADDRESS);
	*(*ppMacAddr) = 0x01;
	*(*ppMacAddr + 1) = 0x00;
	*(*ppMacAddr + 2) = 0x5e;
	*(*ppMacAddr + 3) = pIpAddr[1] & 0x7f;
	*(*ppMacAddr + 4) = pIpAddr[2];
	*(*ppMacAddr + 5) = pIpAddr[3];
	return;
}

static BOOLEAN isIgmpMacAddr(
	IN PUCHAR pMacAddr)
{
	if((pMacAddr[0] == 0x01)
		&& (pMacAddr[1] == 0x00)
		&& (pMacAddr[2] == 0x5e))
		return TRUE;
	return FALSE;
}

BOOLEAN isIgmpPkt(
	IN PUCHAR pDstMacAddr,
	IN PUCHAR pIpHeader)
{
	UINT16 IpProtocol = ntohs(*((UINT16 *)(pIpHeader)));
	UCHAR IgmpProtocol;

	if(!isIgmpMacAddr(pDstMacAddr))
		return FALSE;

	if(IpProtocol == ETH_P_IP)
	{
		IgmpProtocol = (UCHAR)*(pIpHeader + 11);
		if(IgmpProtocol == IGMP_PROTOCOL_DESCRIPTOR)
				return TRUE;
	}

	return FALSE;
}

static VOID InsertIgmpMember(
	IN PLIST_HEADER pList,
	IN PUCHAR pAddr,
	IN IgmpGroupType IGMPType)
{
	PMEMBER_ENTRY pMemberEntry;

	if((pAddr == NULL) || (pList == NULL))
		return;

	if((pMemberEntry = (PMEMBER_ENTRY)removeHeadList(&freeEntryList)) != NULL)
	{
		NdisZeroMemory(pMemberEntry, sizeof(MEMBER_ENTRY));
		COPY_MAC_ADDR(pMemberEntry->Addr, pAddr);
		pMemberEntry->type=IGMPType;
		initList(&pMemberEntry->SourceList);
		insertTailList(pList, (PLIST_ENTRY)pMemberEntry);

		DBGPRINT(RT_DEBUG_TRACE, "%s mac=(%02x:%02x:%02x:%02x:%02x:%02x) SList:%d\n",
			__FUNCTION__, pMemberEntry->Addr[0], pMemberEntry->Addr[1], pMemberEntry->Addr[2],
			pMemberEntry->Addr[3], pMemberEntry->Addr[4], pMemberEntry->Addr[5],pMemberEntry->SourceList.size);
	}
	return;
}

static VOID DeleteIgmpMember(
	IN PLIST_HEADER pList,
	IN PUCHAR pAddr)
{
	PMEMBER_ENTRY pCurEntry;

	if((pAddr == NULL) || (pList == NULL) || (pList->pHead == NULL))
		return;

	pCurEntry = (PMEMBER_ENTRY)pList->pHead;
	while (pCurEntry)
	{
		if(MAC_ADDR_EQUAL(pAddr, pCurEntry->Addr))
		{
		    DeleteIgmpMember_SourceList(&pCurEntry->SourceList);
			delEntryList(pList, (PLIST_ENTRY)pCurEntry);
			insertTailList(&freeEntryList, (PLIST_ENTRY)pCurEntry);
			break;
		}
		pCurEntry = pCurEntry->pNext;
	}

	return;
}

static VOID DeleteIgmpMemberList(
	IN PLIST_HEADER pList)
{
	PMEMBER_ENTRY pCurEntry, pPrvEntry;

	if((pList == NULL) || (pList->pHead == NULL))
		return;

	pPrvEntry = pCurEntry = (PMEMBER_ENTRY)pList->pHead;
	while (pCurEntry)
	{
	    DeleteIgmpMember_SourceList(&pCurEntry->SourceList);
		delEntryList(pList, (PLIST_ENTRY)pCurEntry);
		pPrvEntry = pCurEntry;
		pCurEntry = pCurEntry->pNext;
		insertTailList(&freeEntryList, (PLIST_ENTRY)pPrvEntry);
	}

	initList(pList);
	return;
}


UCHAR IgmpMemberCnt(
	IN PLIST_HEADER pList)
{
	if(pList == NULL)
		return 0;

	return getListSize(pList);
}

VOID IgmpGroupDelMembers(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable,
	IN PUCHAR pMemberAddr,
	IN PNET_DEV pDev)
{
	INT i;
	MULTICAST_FILTER_TABLE_ENTRY *pEntry = NULL;
	
	for (i = 0; i < MAX_LEN_OF_MULTICAST_FILTER_TABLE; i++)
	{
		// pick up the first available vacancy
		pEntry = &pMulticastFilterTable->Content[i];
		if (pEntry->Valid == TRUE)
			MulticastFilterTableDeleteEntry(pMulticastFilterTable, pEntry->Addr, pMemberAddr, pDev);
	}
}

