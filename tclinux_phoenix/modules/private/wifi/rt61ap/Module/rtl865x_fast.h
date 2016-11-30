#ifndef __RTL865X_FAST_H__
#define __RTL865X_FAST_H__

#include "re865x/rtl865x/mbuf.h"
#include "re865x/rtl865x/rtl_glue.h"

#define __IRAM_AIRGO		__attribute__  ((section(".iram-airgo")))
#define __DRAM_AIRGO		__attribute__  ((section(".dram-airgo")))

#define PACKET_AIRGO	7

/* ============================ Define ============================ */

/* indicate RTK pointer's offset of CB field in socket buffer structure : It record the pointer of Packet header */
typedef struct rtl865x_extDev_mbufInfo_s
{
	struct rtl_pktHdr *pktHdr;	/* This pointer MUST be "cb[4]", or the corresponding code must be changed in rtlglue_mbufFreeCluster() */
	u16 pktType;

	union
	{
		struct
		{
			struct net_device *dev;
			u16 rxdesc;
			u16 drvOwn;
		} type_rxRing;
		struct
		{
			void* skb_data;
			void* skb_tail;
			u32 skb_len;
		} type_romeDrv;
	} un;

} rtl865x_extDev_mbufInfo_t;

#define RTL865X_MBUFINFO_CBOFFSET			4	/* This value can NOT be arbitrarily changed, please refer to the comment of the declaration about pktHdr in struct "rtl865x_extDev_mbufInfo_t" */

#define RTL865X_GET_PKTHDR(skb) \
	(((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET)) ?\
		(((rtl865x_extDev_mbufInfo_t*)((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET))->pktHdr)\
		: NULL
#define RTL865X_SET_PKTHDR(skb, pkthdr) \
	do{ \
		if (((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET))\
			(((rtl865x_extDev_mbufInfo_t*)((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET))->pktHdr) = (struct rtl_pktHdr*)(pkthdr); \
	} while(0)

#define RTL865X_GET_PKTTYPE(skb) \
	(((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET)) ?\
		(((rtl865x_extDev_mbufInfo_t*)((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET))->pktType)\
		: RTL865X_TYPE_RT61
#define RTL865X_SET_PKTTYPE(skb, type) \
	do{ \
		if (((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET))\
			(((rtl865x_extDev_mbufInfo_t*)((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET))->pktType) = (u16)(type); \
	} while(0)
#define RTL865X_TYPE_NONE		0x00
#define RTL865X_TYPE_RXRING		0x10
#define RTL865X_TYPE_PS			0x11
#define RTL865X_TYPE_ROMEDRV	0x12
#define RTL865X_TYPE_RUNOUTRX	0x13
#define RTL865X_TYPE_RT61		0x14

#define RTL865X_GET_RXDESC(skb) \
	(((rtl865x_extDev_mbufInfo_t*)((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET))->un.type_rxRing.rxdesc)
#define RTL865X_SET_RXDESC(skb, rxIdx) \
	do{ \
		(((rtl865x_extDev_mbufInfo_t*)((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET))->un.type_rxRing.rxdesc) = (u16)(rxIdx); \
	} while(0)
#define RTL865X_MBUF_MAX_RXDESC		0xffff

#define RTL865X_GET_DEVPTR(skb) \
	(((rtl865x_extDev_mbufInfo_t*)((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET))->un.type_rxRing.dev)
#define RTL865X_SET_DEVPTR(skb, Dev) \
	do{ \
		if (((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET))\
			(((rtl865x_extDev_mbufInfo_t*)((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET))->un.type_rxRing.dev) = (struct net_device*)(Dev); \
	} while(0)

#define RTL865X_GET_SKBOWNER(skb) \
	(((rtl865x_extDev_mbufInfo_t*)((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET))->un.type_rxRing.drvOwn)
#define RTL865X_SET_SKBOWNER(skb, owner) \
	do{ \
		if (((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET))\
			(((rtl865x_extDev_mbufInfo_t*)((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET))->un.type_rxRing.drvOwn) = (owner); \
	} while(0)
#define RTL865X_RXRING_OWN	0xab
#define RTL865X_DRIVER_OWN	0xac

#define RTL865X_GET_ROMEDRV_SKBINFO(skb, infoType) \
	(((rtl865x_extDev_mbufInfo_t*)((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET))->un.type_romeDrv.skb_##infoType)
#define RTL865X_SET_ROMEDRV_SKBINFO(skb, skbInfo, infoType) \
	do{ \
		if (((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET))\
			(((rtl865x_extDev_mbufInfo_t*)((u8*)((skb)->cb) + RTL865X_MBUFINFO_CBOFFSET))->un.type_romeDrv.skb_##infoType) = (skbInfo); \
	} while(0)



#define RTL865X_MBUF_CLUSTER_SIZE			2048 /* Size of each temporary Rx buffer.*/

/* magic number */
#define RTL865X_MBUF_MAGIC_EXTDEV	0x66
#define RTL865X_MBUF_MAGIC_NONE		0

/* uncache/cache mask */
#define UNCACHE_MASK			0x20000000
#define UNCACHE(x)				((unsigned int)x|UNCACHE_MASK)
#define CACHED(x)				((unsigned int)x & ~UNCACHE_MASK)


void rtl865x_extDev_regCallBack(u16 linkid);
void rtl865x_extDev_kfree_skb(struct sk_buff *skb, u32 actualFree);
void rtl865x_extDev_unregisterUcastTxDev(struct net_device *txDev);
int rtl865x_extDev_addHost(u8 *hostMac, u16 vid, unsigned int linkid);
int rtl865x_extDev_removeHost(u8 *hostMac, u16 vid);
struct sk_buff *rtl865x_extDev_alloc_skb (	u32 length,
												u32 headroom,
												u16 rxIdx,
												struct net_device *dev);


#endif /* __RTL865X_FAST_H__*/
