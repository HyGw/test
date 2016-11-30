#ifdef RTL865X_FAST_PATH 

#include "rt_config.h"
#include "re865x/rtl865x/mbuf.h" 
#include "re865x/rtl865x/rtl8651_tblDrvFwd.h"
#include "re865x/rtl865x/rtl8651_tblAsicDrv.h"
#include "re865x/rtl865x/rtl8651_tblDrv.h"

/* ============================ Declaration ============================ */
static inline void rtl865x_extDev_set_skb_property(	u16 pktType,
													struct sk_buff *skb,
													struct net_device *dev,
													struct rtl_pktHdr *pktHdr,
													u16 rxIdx);
static inline void rtl865x_extDev_clr_skb_property(	struct sk_buff *skb);
static struct sk_buff *rtl865x_extDev_alloc_skb (	u32 length,
												u32 headroom,
												u16 rxIdx,
												struct net_device *dev);
static void rtl865x_extDev_actualFreeRxRingSkb(struct sk_buff *skb);

static int rtl865x_extDev_mbuf2Skb(struct net_device *dev, struct sk_buff *skb);
static void rtl865x_extDev_rx_skb(struct net_device *dev, struct sk_buff *skb);


int rtl865x_extDev_addHost (	u8 *hostMac,
								u16 vid,
								unsigned int linkid);
int rtl865x_extDev_removeHost (	u8 *hostMac,
								u16 vid);

int rtl865x_extDev_unicastFastRecv (	struct sk_buff *skb,
												u16 vid,
												struct net_device *dev);
static int rtl865x_extDev_rxRunoutTxPending(	struct sk_buff *skb,
												struct net_device *dev);
static int rtl865x_extDev_pktUcastTx(struct rtl_pktHdr *pkt);
static void rtl865x_extDev_pktFree(struct rtl_pktHdr *pkt);
/*
	design note:

	[chenyl] In order to simplify the flow, we only speed up single 8139CP.
*/
static struct net_device *rt61_txDev = NULL;
static int rt61_activeCnt = 0;
static int rt61_firstPkt = 0;

/* ============================ Implementation ============================ */

/* ======================================================
						Library
     ====================================================== */

static inline void rtl865x_extDev_set_skb_property(	u16 pktType,
													struct sk_buff *skb,
													struct net_device *dev,
													struct rtl_pktHdr *pktHdr,
													u16 rxIdx)
{
	struct rtl_mBuf *mbuf;

	/*
		1) Link socket buffer and mbuf/packet header.
		2) Set property of this socket buffer.
		3) Set rx-descriptor ring index for this packet
		4) Set cp pointer for this packet
		5) Set proprietary magic number for Rome Driver process.
	*/

	mbuf = pktHdr->ph_mbuf;

	RTL865X_SET_PKTHDR(skb, pktHdr);
	RTL865X_SET_DEVPTR(skb, dev);
	RTL865X_SET_PKTTYPE(skb, pktType);

	if (	(pktType == RTL865X_TYPE_RXRING) ||
		(pktType == RTL865X_TYPE_RUNOUTRX))
	{
		mbuf->m_unused1 = RTL865X_MBUF_MAGIC_EXTDEV;
		skb->pkt_type = PACKET_AIRGO;
		RTL865X_SET_RXDESC(skb, rxIdx);
	} else
	{
		RTL865X_SET_RXDESC(skb, RTL865X_MBUF_MAX_RXDESC);
	}
}

static inline void rtl865x_extDev_clr_skb_property(	struct sk_buff *skb)
{
	u16 type;

	type = RTL865X_GET_PKTTYPE(skb);

	if (	(type == RTL865X_TYPE_RXRING) ||
		(type == RTL865X_TYPE_RUNOUTRX))
	{
		struct rtl_pktHdr *pktHdr;
		struct rtl_mBuf *mbuf;

		pktHdr = RTL865X_GET_PKTHDR(skb);

		if (pktHdr)
		{
			mbuf = pktHdr->ph_mbuf;
			mbuf->m_unused1 = RTL865X_MBUF_MAGIC_NONE;
		}
	}

	RTL865X_SET_RXDESC(skb, 0);
	RTL865X_SET_DEVPTR(skb, NULL);
	RTL865X_SET_SKBOWNER(skb, 0);
	RTL865X_SET_PKTHDR(skb, NULL);
	RTL865X_SET_PKTTYPE(skb, RTL865X_TYPE_NONE);
	skb->pkt_type = PACKET_HOST;	/* default type */
}

/* ======================================================
						sock-buffer interface
     ====================================================== */
/*
	This function must be called when Rx Ring init. => Only allocate packet in Rx Ring.

	- This function do the following thing to allocate mbuf structure:

		1. Allocate socket buffer.
		2. Attach mbuf & packet header gotten from mbuf system of Rome Driver.
		3. Set related parameter to link mbuf, packet header and socket buffer.
*/
struct sk_buff *rtl865x_extDev_alloc_skb (	u32 length,
												u32 headroom,
												u16 rxIdx,
												struct net_device *dev)
{
	struct sk_buff *skb = NULL;
	struct rtl_mBuf *mbuf = NULL;
	struct rtl_pktHdr *pktHdr = NULL;

#ifdef DEBUG
	printk (	"%s -- (%s [%d]) Allocate Socket buffer for Rx Ring (len %d headroom %d rxIdx %d).\n",
			dev->name,
			__FUNCTION__,
			__LINE__,
			length,
			headroom,
			rxIdx);
#endif

	/* 1 & 2. allocate skbuff / mbuf / packet header and reserve headroom */
	skb = dev_alloc_skb(length + headroom);

	if (skb == NULL)
	{
		goto out;
	}

	skb_reserve(skb, headroom);

	mbuf = mBuf_attachHeader(	(void*)(UNCACHE((u32)(skb->head))),
								(u32)(skb),
								length,
								0,
								skb_headroom(skb));

	if (mbuf == NULL)
	{
		dev_kfree_skb(skb);
		skb = NULL;
		goto out;
	}

	pktHdr = mbuf->m_pkthdr;

#ifdef DEBUG
	printk (	"\t=>mbuf (%p) pktHdr (%p) m_data(%p) leadingSpace(%d) skb-head(%p) skb-data(%p).\n",
			mbuf,
			pktHdr,
			mbuf->m_data,
			mBuf_leadingSpace(mbuf),
			skb->head,
			skb->data);
#endif

	/*	3. Set relative fields */
	skb->dev = dev;
	rtl865x_extDev_set_skb_property(	RTL865X_TYPE_RXRING,
										skb,
										dev,
										pktHdr,
										rxIdx);

	RTL865X_SET_SKBOWNER(skb, RTL865X_RXRING_OWN);

out:
	return skb;
}

/*
	Actually free socket buffer of RxRing or Runout
*/
static void rtl865x_extDev_actualFreeRxRingSkb(struct sk_buff *skb)
{
	struct rtl_pktHdr *pktHdr;
	u16 headroom;

	pktHdr = RTL865X_GET_PKTHDR(skb);

	rtl865x_extDev_clr_skb_property(skb);

	headroom = mBuf_leadingSpace(pktHdr->ph_mbuf);
	mBuf_freeOneMbufPkthdr(	pktHdr->ph_mbuf,
								NULL,
								NULL,
								NULL);

#ifdef DEBUG
	printk (	"(%s [%d]) Free Rx Ring packet (pkt %p rxIdx %d).\n",
			__FUNCTION__,
			__LINE__,
			RTL865X_GET_PKTHDR(skb),
			RTL865X_GET_RXDESC(skb));
#endif

	/*
		clear this socket buffer.
	*/
	skb->data = skb->tail = skb->head;
	skb->len  = skb->data_len = 0;
	skb_reserve(skb, headroom);

	dev_kfree_skb(skb);
}

/*
	3 processes for socket buffer free module:

		1. This packet is from RX ring.
			A. bridge between 8139cp devices.
			B. RX failed.
				- packet type == RTL865X_TYPE_RXRING
				- put it back to corresponding RX ring if "actualFree" == FALSE.
				- actually free scoket buffer / mbuf / packet header if "actualFree" == TRUE.

		2. This packet is from Rome Driver.
			A. Rx frome Rome Driver and Tx to 8139cp.
				- packet type != RTL865X_TYPE_RXRING && packet type != RTL865X_TYPE_PS
				- use Rome Driver's MBUF API to free this packet.

		3. This packet is from other position.
			A. Direct TX from protocol stack.
				- packet type == RTL865X_TYPE_PS
				- free socket buffer directly.
*/
__IRAM_AIRGO  void rtl865x_extDev_kfree_skb(struct sk_buff *skb, u32 actualFree)
{
	u16 pktType = RTL865X_GET_PKTTYPE(skb);

#ifdef DEBUG
	printk("free socket buffer : %p (type %d), actualFree (%d)\n", skb, pktType, actualFree);
#endif

	switch (pktType)
	{
		case RTL865X_TYPE_RT61:
	        RTMPFreeSkbBuffer(skb);
			break;
		case RTL865X_TYPE_ROMEDRV:
		/* packet from Rome Driver */
		{
			struct rtl_pktHdr *pktHdr;
			struct rtl_mBuf *mbuf;

			pktHdr = RTL865X_GET_PKTHDR(skb);
			mbuf = pktHdr->ph_mbuf;

			/*	we just modify some socket buffer's fields in rtl865x_extDev_pktUcastTx(),
				so we MUST restore them before free it back to Rome Driver				*/
			skb->data = RTL865X_GET_ROMEDRV_SKBINFO(skb, data);
			skb->tail = RTL865X_GET_ROMEDRV_SKBINFO(skb, tail);
			skb->len = RTL865X_GET_ROMEDRV_SKBINFO(skb, len);
			skb->data_len = 0;

			RTL865X_SET_ROMEDRV_SKBINFO(skb, 0, data);
			RTL865X_SET_ROMEDRV_SKBINFO(skb, 0, tail);
			RTL865X_SET_ROMEDRV_SKBINFO(skb, 0, len);
			RTL865X_SET_PKTHDR(skb, NULL);
			RTL865X_SET_PKTTYPE(skb, RTL865X_TYPE_NONE);

			mBuf_freeMbufChain(mbuf);
		}
		break;

		case RTL865X_TYPE_RXRING:
		/* packet from RX Ring */
		{
//			printk("RTL865X_TYPE_RXRING\n");
#if 0
			u16 rxIdx;
			struct net_device *dev;
			struct cp_private *cp;
			struct rtl_pktHdr *pktHdr;
			struct rtl_mBuf *mbuf;

			if (actualFree == FALSE)
			{
				rxIdx = RTL865X_GET_RXDESC(skb);
				dev = RTL865X_GET_DEVPTR(skb);
				pktHdr = RTL865X_GET_PKTHDR(skb);
				cp = dev->priv;
				mbuf = pktHdr->ph_mbuf;

				/*
					clear this socket buffer.
				*/
				skb->data = skb->tail = skb->head;
				skb->len  = skb->data_len = 0;
				skb_reserve(skb, mBuf_leadingSpace(mbuf));
				skb->dev = cp->dev;

#ifdef DEBUG
				printk("\t=> Reclaim socket buff (0x%p) into RxRing Idx (%d)\n\t\t(data(0x%p) tail(0x%p) head(0x%p) len(%d) trueSize(%d)) - dev (%s).\n\t\tmbuf(0x%p) pktHdr(0x%p) mbufLead(%d)\n",
						skb,
						rxIdx,
						skb->data,
						skb->tail,
						skb->head,
						skb->len,
						skb->truesize,
						skb->dev->name,
						RTL865X_GET_PKTHDR(skb),
						((struct rtl_pktHdr*)(RTL865X_GET_PKTHDR(skb)))->ph_mbuf,
						mBuf_leadingSpace(((struct rtl_pktHdr*)(RTL865X_GET_PKTHDR(skb)))->ph_mbuf));
#endif

				rtlglue_drvMutexLock();
				/* ================================================================== */
				/* put this socket buffer back to Rx Ring */
				cp->rx_skb[rxIdx].mapping = pci_map_single(	cp->pdev,
															skb->tail,
															cp->rx_buf_sz,
															PCI_DMA_FROMDEVICE);
				cp->rx_skb[rxIdx].skb = skb;
				cp->rx_skb[rxIdx].frag = 0;

				if (rxIdx == (CP_RX_RING_SIZE - 1))
				{
					cp->rx_ring[rxIdx].opts1 = cpu_to_le32( DescOwn | RingEnd | cp->rx_buf_sz );
				} else
				{
					cp->rx_ring[rxIdx].opts1 = cpu_to_le32( DescOwn | cp->rx_buf_sz );
				}

				cp->rx_ring[rxIdx].opts2 = 0;
				cp->rx_ring[rxIdx].addr_lo = cpu_to_le32(cp->rx_skb[rxIdx].mapping);
				cp->rx_ring[rxIdx].addr_hi = 0;

				RTL865X_SET_SKBOWNER(skb, RTL865X_RXRING_OWN);

				/* ================================================================== */
				rtlglue_drvMutexUnlock();

			}else
#endif
			{
				/* actualFree == TRUE: actually free Rx Ring Socket buffer */
				rtl865x_extDev_actualFreeRxRingSkb(skb);
			}
		}
		break;

		case RTL865X_TYPE_PS:
		/* packet from Protocol Stack */
		{
			rtl865x_extDev_clr_skb_property(skb);
			dev_kfree_skb(skb);
		}
		break;

		case RTL865X_TYPE_RUNOUTRX:
		{
			/* the position of this packet in RX Ring is replaced : actually free it */
			rtl865x_extDev_actualFreeRxRingSkb(skb);
		}
		break;

		default:
			printk("Packet type : %d -- free socket buffer only\n", pktType);
			dev_kfree_skb(skb);
	}
}

/* ======================================================
						protocol stack interface
     ====================================================== */
static int rtl865x_extDev_mbuf2Skb(struct net_device *dev, struct sk_buff *skb)
{
	struct sk_buff *newSkb = NULL;
	struct rtl_pktHdr *pktHdr = NULL;
	struct rtl_mBuf *mbuf = NULL;
	unsigned int pktLen = 0;
	unsigned int pktTotalLen = 0;
	u16 rxIdx;
	u16 pktType;
	u16 headroom;

#ifdef DEBUG
	printk("%s -- for socket buffer (%p) -- head %p data %p headroom %d len %d mbuf_head %d\n",
		__FUNCTION__,
		skb,
		skb->head,
		skb->data,
		(u32)skb->data - (u32)skb->head,
		skb->len,
		mBuf_leadingSpace(((struct rtl_pktHdr*)RTL865X_GET_PKTHDR(skb))->ph_mbuf)
		);
#endif

	pktHdr = RTL865X_GET_PKTHDR(skb);
	mbuf = pktHdr->ph_mbuf;
	pktLen = pktHdr->ph_len;
	pktTotalLen = mbuf->m_extsize;
	rxIdx = RTL865X_GET_RXDESC(skb);
	pktType = RTL865X_GET_PKTTYPE(skb);
	headroom = mBuf_leadingSpace(mbuf);

	if (pktType != RTL865X_TYPE_RXRING)
	{
		printk("BUG ! socket buffer which put into %s is not from Rx Ring!!\n", __FUNCTION__);
		goto err_out;
	}

	/* Allocate socket buffer for this rx packet before trapping it to protocol stack */
	newSkb = dev_alloc_skb(pktTotalLen);

	if (newSkb == NULL)
	{
		/* This packet RX failed : ignore this packet and return directly */
		printk (	"%s -- (%s [%d]) socket buffer allocation failed.\n",
				dev->name,
				__FUNCTION__,
				__LINE__);
		goto err_out;
	}

	newSkb->data = newSkb->tail = newSkb->head;
	newSkb->len = 0;
	skb_reserve(newSkb, headroom);
	newSkb->dev = dev;

	/* We need to flush D-cache before TX to protocol stack */
	lx4180_writeCacheCtrl(0);
	lx4180_writeCacheCtrl(1);
	lx4180_writeCacheCtrl(0);

	/* attach mbuf/packet header to newly allocated socket buffer */
	mBuf_attachCluster(	mbuf,
						(void*)(UNCACHE((u32)(newSkb->head))),
						(unsigned int)newSkb,
						pktTotalLen,
						0,
						0);
	mBuf_reserve(mbuf, headroom);

	/* clear original Socket buffer */
	rtl865x_extDev_clr_skb_property(skb);

	/* re-link pointer of socket buffer to packet header */
	rtl865x_extDev_set_skb_property(	pktType,
										newSkb,
										dev,
										pktHdr,
										rxIdx);

	/* reclaim packet with new socket buffer */
	rtl865x_extDev_kfree_skb(newSkb, FALSE);

	return 0;

err_out:
	rtl865x_extDev_kfree_skb(skb, FALSE);
	return -1;
}
static void rtl865x_extDev_rx_skb(struct net_device *dev, struct sk_buff *skb)
{
	if (rtl865x_extDev_mbuf2Skb(dev, skb) != 0)
	{
		return;
	}

//	memDump(skb->data, skb->len>128?128:skb->len, "rx skb socket buffer");

	/* Trap original skb to protocol stack : we don't update statistics in this function because it's done in previous code. */
	skb->protocol = eth_type_trans(skb, dev);
	skb->dev = dev;							/* set RX device */
	netif_rx(skb);

}

/* ======================================================
						RTL865X Rome Driver interface
     ====================================================== */
int rtl865x_extDev_addHost (	u8 *hostMac,
											u16 vid,
											unsigned int linkid)
//											struct net_device *dev)
{
	unsigned int i;

	printk("@ = Add Host : [%02x:%02x:%02x:%02x:%02x:%02x] VID %d LinkID %d PortNumber %d\n",
		hostMac[0],
		hostMac[1],
		hostMac[2],
		hostMac[3],
		hostMac[4],
		hostMac[5],
		vid,
		linkid,
		CONFIG_RTL865XB_WLAN1_PORT);

	return rtl8651_fwdEngineAddWlanSTA(	hostMac,
											vid,
											(1 << CONFIG_RTL865XB_WLAN1_PORT),
											linkid);
}

int rtl865x_extDev_removeHost (	u8 *hostMac,
												u16 vid)
{
	return rtl8651_fwdEngineDelWlanSTA(	hostMac,
										vid);
}

#if 1
__IRAM_AIRGO static int rtl865x_extDev_unicastFastRecv (	struct sk_buff *skb,
															u16 vid,
															struct net_device *dev)
{
	struct rtl_pktHdr *pktHdr;
	struct rtl_mBuf *mbuf;

	pktHdr = RTL865X_GET_PKTHDR(skb);
	mbuf = pktHdr->ph_mbuf;

	mbuf->m_data = skb->data;
	mbuf->m_len = skb->len;
	pktHdr->ph_len = skb->len;
	pktHdr->ph_iphdrOffset = 0;
	pktHdr->ph_pppeTagged = 0;
	pktHdr->ph_LLCTagged = 0;
	pktHdr->ph_vlanTagged = 0;
	pktHdr->ph_extPortList = 0;

#ifdef DEBUG
	printk (	"%s -- (%s [%d]) Unicast Fast Tx to Rome Driver\n\t=> Type(%d) Rx Idx(%d) - pkt(%p) mbuf(%p) mdata(%p) vid(%d) dev(%p).\n\t=> leadroom %d\n\t=> head (%p) data(%p)\n",
			dev->name,
			__FUNCTION__,
			__LINE__,
			RTL865X_GET_PKTTYPE(skb),
			RTL865X_GET_RXDESC(skb),
			pktHdr,
			mbuf,
			mbuf->m_data,
			vid,
			skb->dev,
			mBuf_leadingSpace(mbuf),
			skb->head,
			skb->data);

	printk("=============================================\n");
	memDump(pktHdr->ph_mbuf->m_data, pktHdr->ph_len, "Fast Tx pkt");
	printk("=============================================\n");
#endif

#if 0
	if ((rt61_firstPkt ++) < 3)
	{
		rtl865x_extDev_addHost(&(skb->data[6]), vid, rt61_txDev);
	}
#endif

	return rtl8651_fwdEngineExtPortUcastFastRecv(	pktHdr,
													vid,
													(1 << CONFIG_RTL865XB_WLAN1_PORT));
}

#if 0
static int rtl865x_extDev_rxRunoutTxPending(struct sk_buff *skb, struct net_device *dev)
{
	struct cp_private *cp = dev->priv;
	struct sk_buff *newSkb = NULL;
	struct rtl_pktHdr *pktHdr = NULL;
	u16 rxIdx;
	u16 orgOwner;

	orgOwner = RTL865X_GET_SKBOWNER(skb);

	rxIdx = RTL865X_GET_RXDESC(skb);
	pktHdr = RTL865X_GET_PKTHDR(skb);

	/* -------------- Allocate new Socket buffer ------------- */
	if ((newSkb = rtl865x_extDev_alloc_skb(	cp->rx_buf_sz,
											RX_OFFSET + 4,
											rxIdx,
											dev)) == NULL)
	{
		printk("FATAL!! Rx Run-out CAN NOT be solved.!\n");
		return FAILED;
	}

	rtlglue_drvMutexLock();

	/* -------------- Put this socket buffer into Rx Ring to replace original one -------------- */
	cp->rx_skb[rxIdx].mapping = pci_map_single(	cp->pdev,
												newSkb->tail,
												cp->rx_buf_sz,
												PCI_DMA_FROMDEVICE);

	cp->rx_skb[rxIdx].skb = newSkb;
	cp->rx_skb[rxIdx].frag = 0;

	if (rxIdx == (CP_RX_RING_SIZE - 1))
	{
		cp->rx_ring[rxIdx].opts1 = cpu_to_le32(DescOwn | RingEnd | cp->rx_buf_sz);
	} else
	{
		cp->rx_ring[rxIdx].opts1 = cpu_to_le32(DescOwn | cp->rx_buf_sz);
	}
	cp->rx_ring[rxIdx].opts2 = 0;
	cp->rx_ring[rxIdx].addr_lo = cpu_to_le32(cp->rx_skb[rxIdx].mapping);
	cp->rx_ring[rxIdx].addr_hi = 0;

	/* -------------- Original packet : change its property to freed by our driver -------------- */
	switch (orgOwner)
	{
		case RTL865X_RXRING_OWN:
			rtl865x_extDev_kfree_skb(skb, TRUE);
			break;
		case RTL865X_DRIVER_OWN:
			RTL865X_SET_PKTTYPE(skb, RTL865X_TYPE_RUNOUTRX);
			break;
		default:
			printk("BUG! Unknown owner type: %d\n", orgOwner);
			rtl865x_extDev_kfree_skb(skb, TRUE);
	}

	rtlglue_drvMutexUnlock();
	return SUCCESS;
}
#endif
#endif


/* declaration of fast Tx function */
static int RT61_start_FastUnicastXmit (struct sk_buff *skb, struct net_device *dev);

void rtl865x_extDev_registerUcastTxDev(struct net_device *txDev)
{
	printk (	"%s -- (%s [%d]) Register Unicast Tx Device [%p].\n",
			txDev->name,
			__FUNCTION__,
			__LINE__,
			txDev);

	if (rt61_txDev == NULL)
	{
		rt61_txDev = txDev;
		rt61_firstPkt = 0;
	} else
	{
		printk(	"%s -- (%s [%d])  The Fast Tx device is owned by [%s] (current active count = %d).\n",
				txDev->name,
				__FUNCTION__,
				__LINE__,
				rt61_txDev->name,
				rt61_activeCnt);
	}

	rt61_activeCnt ++;
}

void rtl865x_extDev_unregisterUcastTxDev(struct net_device *txDev)
{
	printk (	"%s -- (%s [%d]) Un-Register Unicast Tx Device [%p].\n",
			txDev->name,
			__FUNCTION__,
			__LINE__,
			txDev);

	if (rt61_activeCnt == 0)
	{
		printk (	"%s -- (%s [%d]) unregister device with current active count = %d.\n",
				txDev->name,
				__FUNCTION__,
				__LINE__,
				rt61_activeCnt);
		return;
}
	if (rt61_txDev == txDev)
	{
		rt61_txDev = NULL;
		rt61_firstPkt = 0;
	}

	rt61_activeCnt --;
}

/*
		Fast Unicast Tx function
*/
static __IRAM_AIRGO int rtl865x_extDev_pktUcastTx(struct rtl_pktHdr *pkt)
{
	struct rtl_mBuf *mbuf;
	struct sk_buff *skb;

    PRTMP_ADAPTER   pAdapter;
	int i;



	/*
		[chenyl] In order to simplify the flow, we only speed up single 8139CP.
	*/
	if ((rt61_txDev == NULL) ||
		(netif_queue_stopped(rt61_txDev)) ||
		(rt61_activeCnt > 1))
	{
		mBuf_freeMbufChain(pkt->ph_mbuf);
		return SUCCESS;
	}


	pAdapter = rt61_txDev->priv;

	if (pkt->ph_extDev_linkID == 0) {
		printk("fast tx link id == 0\n");
		mBuf_freeMbufChain(pkt->ph_mbuf);
		return SUCCESS;		
	}


#ifdef DEBUG
	printk (	"(%s [%d]) Unicast TX by Rome Driver -- Pkt (%p) mbuf (%p) skb (%p) type(%d).\n",
			__FUNCTION__,
			__LINE__,
			pkt,
			pkt->ph_mbuf,
			(struct sk_buff *)(pkt->ph_mbuf->m_extClusterId),
			RTL865X_GET_PKTTYPE((struct sk_buff *)(pkt->ph_mbuf->m_extClusterId)));
#endif

	mbuf = pkt->ph_mbuf;
	skb = (struct sk_buff*)(mbuf->m_extClusterId);
	skb->ip_summed = CHECKSUM_NONE;

	skb->dev = rt61_txDev;
	RTMP_SET_PACKET_NET_DEVICE_MBSSID(skb, 0);

#if 0
{
	int j;
	for (j=0; j<48; j++) {
		if (skb->cb[j] != 0)
			printk("cb[%d] = %02x\n", j, skb->cb[j]);
	} 	
}
#endif

	/* record information */
	RTL865X_SET_PKTTYPE(skb, RTL865X_TYPE_ROMEDRV);
	RTL865X_SET_PKTHDR(skb, pkt);
	RTL865X_SET_ROMEDRV_SKBINFO(skb, skb->data, data);
	RTL865X_SET_ROMEDRV_SKBINFO(skb, skb->tail, tail);
	RTL865X_SET_ROMEDRV_SKBINFO(skb, skb->len, len);


#ifdef MBSS_SUPPORT
    for (i = FIRST_MBSSID; i < pAdapter->PortCfg.BssidNum; i++)
    {
        if (pAdapter->PortCfg.MBSSID[i].mylinkid == pkt->ph_extDev_linkID)
        {
        	RTMP_SET_PACKET_SOURCE(skb, PKTSRC_NDIS);
            RTMP_SET_PACKET_NET_DEVICE_MBSSID(skb, i);
            //skb->dev = pAdapter->PortCfg.MBSSID[i].MSSIDDev;
			break;
        }
    }
#endif /* MBSS_SUPPORT */


#ifdef WDS_SUPPORT
    for (i = 0; i < MAX_WDS_ENTRY; i++)
    {
        if ((pAdapter->WdsTab.WdsEntry[i].Valid == TRUE) && (pAdapter->WdsTab.WdsEntry[i].mylinkid == pkt->ph_extDev_linkID))
        {
        	RTMP_SET_PACKET_SOURCE(skb, PKTSRC_NDIS);
            RTMP_SET_PACKET_NET_DEVICE_WDS(skb, i);
			//skb->dev = pAdapter->WdsTab.WdsEntry[i].dev;
			break;
		}
    }
#endif 


	/* set socket buffer's field for TX */
	skb->data = skb->tail = skb->head;
	skb->len = 0;
	skb->data_len = 0;
	skb_reserve(skb, 16 /* please refer to __dev_alloc_skb() in the skbuff.h */ + mBuf_leadingSpace(mbuf));
	skb_put(skb, pkt->ph_len);

#ifdef DEBUG
	memDump(skb->data, 64, "Fast TX to RT61");
#endif

//	printk("skb: data = %lx, tail = %lx, len = %d(%d)\n", skb->data, skb->tail, skb->len, skb->tail-skb->data);

	if (RTMPSendFastPackets(skb, rt61_txDev) != 0)
	{
		rtl865x_extDev_kfree_skb(skb, FALSE);
	}

	return SUCCESS;
}

static __IRAM_AIRGO void rtl865x_extDev_pktFree(struct rtl_pktHdr *pkt)
{
#ifdef DEBUG
	printk("-------------------------------------------------------\n");
	printk (	"(%s [%d]) Free Packet by Rome Driver -- Pkt (%p) mbuf (%p) skb (%p).\n",
			__FUNCTION__,
			__LINE__,
			pkt,
			pkt->ph_mbuf,
			(struct sk_buff *)(pkt->ph_mbuf->m_extClusterId));
	printk("--------------------------------------------------------Printk done\n");
#endif

	rtl865x_extDev_kfree_skb(	(struct sk_buff*)(pkt->ph_mbuf->m_extClusterId),
								FALSE);
}

void rtl865x_extDev_regCallBack(u16 linkid)
{
	printk (	"(%s [%d]) Register CallBack function -- Ucast Tx (%p) Free (%p).\n",
			__FUNCTION__,
			__LINE__,
			rtl865x_extDev_pktUcastTx,
			rtl865x_extDev_pktFree);

	rtlairgo_fast_tx_register(	rtl865x_extDev_pktUcastTx,
								rtl865x_extDev_pktFree);
}
#endif /* RTL865X_FAST_PATH */


