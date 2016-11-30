#ifndef __RTMP_APCLI_H__
#define __RTMP_APCLI_H__

#include "rt_config.h"

#if 1 //def __LINUX__

#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <linux/ip.h>
#else

//Currently support upper layer protocols
#define ETH_P_IP        0x0800          /* Internet Protocol packet     */
#define ETH_P_ARP       0x0806          /* Address Resolution packet    */
#define ETH_P_PPP_DISC  0x8863          /* PPPoE discovery messages     */
#define ETH_P_PPP_SES   0x8864          /* PPPoE session messages       */


/* ARP protocol HARDWARE identifiers. */
#define ARPHRD_ETHER    1               /* Ethernet 10Mbps              */

/* ARP protocol opcodes. */
#define ARPOP_REQUEST   1               /* ARP request                  */
#define ARPOP_REPLY     2               /* ARP reply                    */

struct arphdr{
	unsigned short  ar_hrd;         /* format of hardware address   */
	unsigned short  ar_pro;         /* format of protocol address   */
	unsigned char   ar_hln;         /* length of hardware address   */
	unsigned char   ar_pln;         /* length of protocol address   */
	unsigned short  ar_op;          /* ARP opcode (command)         */
};

struct iphdr{
#ifndef BIG_ENDIAN
	UCHAR   ihl:4,
			version:4;
#else
	UCHAR	version:4,
			ihl:4;
#endif
	UCHAR	tos;
	UINT16	tot_len;
	UINT16	id;
	UINT16	frag_off;
	UCHAR	ttl;
	UCHAR	protocol;
	UINT16	check;
	UINT16	saddr;
	UINT32	daddr;
}; 
#endif  //endif of __LINUX__


#define APCLI_ETHER_HDR_LEN		14		// dstMac(6) + srcMac(6) + protoType(2)
#define APCLI_IP_HDR_LEN		20

#define MAX_SUPPORT_APCLI_STA			64
#define IPMAC_TB_HASH_ENTRY_NUM			(MAX_SUPPORT_APCLI_STA+1)	// One entry for broadcast address
#define IPMAC_TB_HASH_INDEX_OF_BCAST 	MAX_SUPPORT_APCLI_STA		// cause hash index start from 0.
#define IPMAC_TB_ENTRY_AGEOUT_TIME		30000

#define APCLI_MAC_ADDR_IS_GROUP(Addr)			(((Addr[0]) & 0x01) != 0)
#define APCLI_MAC_ADDR_HASH(Addr)       (Addr[0] ^ Addr[1] ^ Addr[2] ^ Addr[3] ^ Addr[4] ^ Addr[5])
#define APCLI_MAC_ADDR_HASH_INDEX(Addr) (APCLI_MAC_ADDR_HASH(Addr) % MAX_SUPPORT_APCLI_STA)

#define APCLI_IP_ADDR_HASH(Addr)		(((Addr>>24)&0xff)^((Addr>>16) & 0xff) ^((Addr>>8) & 0xff) ^ (Addr & 0xff))
#define APCLI_IP_ADDR_HASH_INDEX(Addr)	(APCLI_IP_ADDR_HASH(Addr) % MAX_SUPPORT_APCLI_STA)

typedef struct _UWRProtoEntry
{
	NDIS_STATUS	(*init)(void);
	PUCHAR		(*tx)(PRTMP_ADAPTER pAd, struct sk_buff *pSkb, PUCHAR pLayerHdr);
	PUCHAR		(*rx)(PRTMP_ADAPTER pAd, struct sk_buff *pSkb, PUCHAR pLayerHdr);
	NDIS_STATUS	(*exit)(void);
	PVOID		pMgtTb;
}UWRProtoEntry, PUWRProtoEntry;


typedef struct _APCLIProtoTable
{
	UINT16					protoCode;
	struct _APCLIProtoEntry *pHandle;	
}APCLIProtoTable, PAPCLIProtoTable;

VOID dumpPkt(PUCHAR pHeader, int len);

//#define KMALLOC_BATCH

#ifdef KMALLOC_BATCH
PUCHAR UWRNodeEntryAlloc(VOID);
NDIS_STATUS UWRNodeEntryFree(PUCHAR NodeEntry);
#endif

#define isMcastEtherAddr(addr)	(0x01 & addr[0])
#define isBcastEtherAddr(addr)	((addr[0] & addr[1] & addr[2] & addr[3] & addr[4] & addr[5])== 0xff)
#define isZeroEtherAddr(addr)	(!(addr[0] | addr[1] | addr[2] | addr[3] | addr[4] | addr[5]))

//isUcastMac = !(00:00:00:00:00:00 || mcastMac);
#define IS_UCAST_MAC(addr)	(!(isMcastEtherAddr(addr) || isZeroEtherAddr(addr)))
#define IS_EQUAL_MAC(a, b)	(((a[0] ^ b[0]) | (a[1] ^ b[1]) | (a[2] ^ b[2]) | (a[3] ^ b[3]) | (a[4] ^ b[4]) | (a[5] ^ b[5])) == 0)

#define IS_GOOD_IP(IP)	(IP!= 0)

#endif  //endif of __RTMP_APCLI_H__

