
#include "rt_config.h"
#include "netif_block.h"

static NETIF_ENTRY freeNetIfEntryPool[FREE_NETIF_POOL_SIZE];
static LIST_HEADER freeNetIfEntryList;

void initblockQueueTab(
    IN  VOID)
{
	int i;

	initList(&freeNetIfEntryList);
	for (i = 0; i < FREE_NETIF_POOL_SIZE; i++)
		insertTailList(&freeNetIfEntryList, (PLIST_ENTRY)&freeNetIfEntryPool[i]);

	return;
}

BOOLEAN blockNetIf(
	IN PBLOCK_QUEUE_ENTRY pBlockQueueEntry,
	IN PNET_DEV pNetDev)
{
	PNETIF_ENTRY pNetIfEntry = NULL;
	
	if ((pNetIfEntry = (PNETIF_ENTRY)removeHeadList(&freeNetIfEntryList)) != NULL)
	{
		netif_stop_queue(pNetDev);
		pNetIfEntry->pNetDev = pNetDev;
		insertTailList(&pBlockQueueEntry->NetIfList, (PLIST_ENTRY)pNetIfEntry);

		pBlockQueueEntry->SwTxQueueBlockFlag = TRUE;
		DBGPRINT(RT_DEBUG_TRACE, "netif_stop_queue(%s)\n", pNetDev->name);
	}
	else
		return FALSE;

	return TRUE;
}

VOID releaseNetIf(
	IN PBLOCK_QUEUE_ENTRY pBlockQueueEntry)
{
	PNETIF_ENTRY pNetIfEntry = NULL;
	PLIST_HEADER pNetIfList = &pBlockQueueEntry->NetIfList;

	while((pNetIfEntry = (PNETIF_ENTRY)removeHeadList(pNetIfList)) !=  NULL)
	{
		PNET_DEV pNetDev = pNetIfEntry->pNetDev;
		netif_wake_queue(pNetDev);
		insertTailList(&freeNetIfEntryList, (PLIST_ENTRY)pNetIfEntry);

		DBGPRINT(RT_DEBUG_TRACE, "netif_wake_queue(%s)\n", pNetDev->name);
	}
	pBlockQueueEntry->SwTxQueueBlockFlag = FALSE;
	return;
}


