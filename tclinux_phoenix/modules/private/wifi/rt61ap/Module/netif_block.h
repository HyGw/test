
#ifndef __NET_IF_BLOCK_H__
#define __NET_IF_BLOCK_H__

#include <linux/device.h>
#include "link_list.h"
#include "rtmp.h"

#define FREE_NETIF_POOL_SIZE 32

typedef struct _NETIF_ENTRY
{
	struct _NETIF_ENTRY *pNext;
	PNET_DEV pNetDev;
} NETIF_ENTRY, *PNETIF_ENTRY;

typedef struct _BLOCK_QUEUE_ENTRY
{
	BOOLEAN SwTxQueueBlockFlag;
	LIST_HEADER NetIfList;
} BLOCK_QUEUE_ENTRY, *PBLOCK_QUEUE_ENTRY;


void initblockQueueTab(
	IN VOID);

BOOLEAN blockNetIf(
	IN PBLOCK_QUEUE_ENTRY pBlockQueueEntry,
	IN PNET_DEV pNetDev);

VOID releaseNetIf(
	IN PBLOCK_QUEUE_ENTRY pBlockQueueEntry);

#endif // __NET_IF_BLOCK_H__

