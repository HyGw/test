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
    rtmp_igmpSnoop.h

    Abstract:

    Revision History:
    Who         When            What
    --------    ----------      ----------------------------------------------
    Name        Date            Modification logs
    WY Juang    2007-07-03      patch to support IGMPv3
*/

#ifndef __RTMP_IGMP_SNOOP_H__
#define __RTMP_IGMP_SNOOP_H__

#include "link_list.h"

#define IGMP_PROTOCOL_DESCRIPTOR	0x02
#define IGMP_V1_MEMBERSHIP_REPORT	0x12
#define IGMP_V2_MEMBERSHIP_REPORT	0x16
#define IGMP_LEAVE_GROUP			0x17
#define IGMP_V3_MEMBERSHIP_REPORT	0x22

#define IPV4_ADDR_LEN 4

#define IGMPMAC_TB_ENTRY_AGEOUT_TIME 220 * HZ

#define MAX_LEN_OF_MULTICAST_FILTER_TABLE 64
#define MAX_LEN_OF_MULTICAST_FILTER_HASH_TABLE ((MAX_LEN_OF_MULTICAST_FILTER_TABLE) * 2)
#define FREE_MEMBER_POOL_SIZE 64
#define FREE_SOURCE_POOL_SIZE 14

#define MULTICAST_ADDR_HASH_INDEX(Addr)      (MAC_ADDR_HASH(Addr) % (MAX_LEN_OF_MULTICAST_FILTER_HASH_TABLE))

#define IS_MULTICAST_MAC_ADDR(Addr)			((((Addr[0]) & 0x01) == 0x01) && ((Addr[0]) != 0xff))
#define IS_BROADCAST_MAC_ADDR(Addr)			((((Addr[0]) & 0xff) == 0xff))

#define IPV4_ADDR_EQUAL(pAddr1,pAddr2)          RTMPEqualMemory((PVOID)(pAddr1), (PVOID)(pAddr2), IPV4_ADDR_LEN)
#define COPY_IPV4_ADDR(Addr1, Addr2)             memcpy((Addr1), (Addr2), IPV4_ADDR_LEN)

typedef enum _IGMP_GROUP_TYPE
{
	MODE_IS_INCLUDE = 1,
	MODE_IS_EXCLUDE,
	CHANGE_TO_INCLUDE_MODE,
	CHANGE_TO_EXCLUDE_MODE,
	ALLOW_NEW_SOURCES,
	BLOCK_OLD_SOURCES,
	IGMPV1V2
} IgmpGroupType;

typedef enum _MULTICAST_FILTER_ENTRY_TYPE
{
	MCAT_FILTER_STATIC = 0,
	MCAT_FILTER_LLC,
	MCAT_FILTER_DYNAMIC,
} MulticastFilterEntryType;

typedef struct _MEMBER_ENTRY
{
	struct _MEMBER_ENTRY *pNext;
	UCHAR Addr[MAC_ADDR_LEN];
	UCHAR type;
	LIST_HEADER SourceList;
} MEMBER_ENTRY, *PMEMBER_ENTRY;

typedef struct _SOURCE_ENTRY
{
	struct _SOURCE_ENTRY *pNext;
	UCHAR Addr[IPV4_ADDR_LEN];
} SOURCE_ENTRY, *PSOURCE_ENTRY;

typedef struct _MULTICAST_FILTER_TABLE_ENTRY
{
	BOOLEAN Valid;
	MulticastFilterEntryType type; // 0: static, 1: dynamic.
	UMAXLONG  lastTime;
	PNET_DEV net_dev;
	UCHAR Addr[MAC_ADDR_LEN];
	LIST_HEADER MemberList;
	struct _MULTICAST_FILTER_TABLE_ENTRY *pNext;
} MULTICAST_FILTER_TABLE_ENTRY, *PMULTICAST_FILTER_TABLE_ENTRY;

typedef struct _MULTICAST_FILTER_TABLE
{
	UCHAR Size;
	PMULTICAST_FILTER_TABLE_ENTRY Hash[MAX_LEN_OF_MULTICAST_FILTER_HASH_TABLE];
	MULTICAST_FILTER_TABLE_ENTRY Content[MAX_LEN_OF_MULTICAST_FILTER_TABLE];
} MULTICAST_FILTER_TABLE, *PMULTICAST_FILTER_TABLE;

VOID MulticastFilterTableInit(
	IN PMULTICAST_FILTER_TABLE *ppMulticastFilterTable);

VOID MultiCastFilterTableReset(
	IN PMULTICAST_FILTER_TABLE *ppMulticastFilterTable);

BOOLEAN MulticastFilterTableInsertEntry(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable,
	IN PUCHAR pGrpId,
	IN PUCHAR pMemberAddr,
	IN PNET_DEV dev,
	IN MulticastFilterEntryType type,
	IN IgmpGroupType IGMPType);

BOOLEAN MulticastFilterTableDeleteEntry(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable,
	IN PUCHAR pGrpId,
	IN PUCHAR pMemberAddr,
	IN PNET_DEV dev);

PMULTICAST_FILTER_TABLE_ENTRY MulticastFilterTableLookup(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable,
	IN PUCHAR pAddr,
	IN PNET_DEV dev);

void MulticastFilterTableUpdate(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable,
	IN MulticastFilterEntryType type);

#if 0 // Doesn't be used.
BOOLEAN MulticastFilterApply(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable,
	IN PUCHAR pDstMacAddr,
	IN PNET_DEV pDev);
#endif

VOID ConvertMulticastIP2MAC(
	IN PUCHAR pIpAddr,
	IN PUCHAR *ppMacAddr);

BOOLEAN isIgmpPkt(
	IN PUCHAR pDstMacAddr,
	IN PUCHAR pIpHeader);

VOID IGMPSnooping(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable,
	IN PUCHAR pDstMacAddr,
	IN PUCHAR pSrcMacAddr,
	IN PUCHAR pIpHeader,
	IN PNET_DEV pDev);

UCHAR IgmpMemberCnt(
	IN PLIST_HEADER pList);

VOID IgmpGroupDelMembers(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable,
	IN PUCHAR pMemberAddr,
	IN PNET_DEV pDev);

void InsertIgmpMember_Source(
	IN PLIST_HEADER pList,IN PUCHAR pSourceAddr);
void DeleteIgmpMember_Source(
	IN PLIST_HEADER pList,IN PUCHAR pSourceAddr);
void DeleteIgmpMember_SourceList(
	IN PLIST_HEADER pList);
BOOLEAN InsertIgmpMember_Sources(
	IN PMULTICAST_FILTER_TABLE pMulticastFilterTable,
	IN PUCHAR pGrpId,
	IN PUCHAR pMemberAddr,
	IN PUCHAR pSourceAddr,
	IN UCHAR numOfSources,
	IN UCHAR GroupType,
	IN PNET_DEV dev);

#endif /* __RTMP_IGMP_SNOOP_H__ */

