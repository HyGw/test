/***************************************************************
Copyright Statement:

This software/firmware and related documentation (¡°EcoNet Software¡±) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (¡°EcoNet¡±) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (¡°ECONET SOFTWARE¡±) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN ¡°AS IS¡± 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVER¡¯S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVER¡¯S SPECIFICATION OR CONFORMING TO A PARTICULAR 
STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND 
ECONET'S ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE ECONET 
SOFTWARE RELEASED HEREUNDER SHALL BE, AT ECONET'S SOLE OPTION, TO 
REVISE OR REPLACE THE ECONET SOFTWARE AT ISSUE OR REFUND ANY SOFTWARE 
LICENSE FEES OR SERVICE CHARGES PAID BY RECEIVER TO ECONET FOR SUCH 
ECONET SOFTWARE.
***************************************************************/

/************************************************************************
*                  I N C L U D E S
*************************************************************************
*/
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/if_vlan.h>
#include <linux/if_pppox.h>
#include <linux/ip.h>
#ifdef TCSUPPORT_IPV6
#include <linux/ipv6.h>
#endif
#include <net/tcp.h>


/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/

/************************************************************************
*                  M A C R O S
*************************************************************************
*/

/************************************************************************
*                  D A T A   T Y P E S
*************************************************************************
*/

/************************************************************************
*                  E X T E R N A L   D A T A   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/


/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/
int (*match_multicast_vtag_check)
(struct sk_buff *skb, struct net_device *vdev);
EXPORT_SYMBOL(match_multicast_vtag_check);
#if !defined(TCSUPPORT_CT_VLAN_TAG)
int (*match_multicast_vtag)(struct sk_buff *skb, struct net_device *vdev);
EXPORT_SYMBOL(match_multicast_vtag);
#endif


/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

/****************************************************************************
**function name
	 __vlan_proto
**description:
	get protocol via skb
**return 
	eth_type
**parameter:
	skb: the packet information
****************************************************************************/
static inline __be16 __vlan_proto(const struct sk_buff *skb)
{
	return vlan_eth_hdr(skb)->h_vlan_encapsulated_proto;
}

/****************************************************************************
**function name
	 check_ppp_udp_multicast
**description:
	check multicast packet in downstream
**return 
	0:	check ok or ignore
	-1:	fail
**parameter:
	skb: the packet information
	vdev: virtual net device
****************************************************************************/
int check_ppp_udp_multicast
(struct sk_buff *skb, struct net_device *vdev)
{
#if defined(TCSUPPORT_CT_PON_SC)
	struct pppoe_hdr *ppph = NULL;
	struct iphdr *iph = NULL;
	__be16 ppp_proto = 0;
	unsigned char *pppoe_h = NULL, dest_addr[16] = {0};;
	struct ipv6hdr *ip6h = NULL;
	u8 nexthdr = 0;
	int offset = 0, skip_start = 0;
#define PPP_IP		0x21	/* Internet Protocol */
#define PPP_IPV6	0x57	/* Internet Protocol Version 6 */
#define VTAG_SUCCESS 0

	if ( htons(ETH_P_PPP_SES) == skb->protocol )
		ppph = pppoe_hdr(skb);
	else if( ( htons(ETH_P_8021Q) == skb->protocol )
			&& ( htons(ETH_P_PPP_SES) == __vlan_proto(skb) ) )
		ppph = (struct pppoe_hdr *)(skb_mac_header(skb) + VLAN_ETH_HLEN);
	else
		return 0;

	pppoe_h = (ppph + 1);
	ppp_proto = *(__be16*)(pppoe_h);
	pppoe_h += 2; /* skip ppp protocol */
	memset(dest_addr, 0, sizeof(dest_addr));
	/* IPv4 */
	if ( PPP_IP == ppp_proto )
	{
		iph = (struct iphdr* )(pppoe_h);
		if ( IPPROTO_UDP != iph->protocol )
			return 0;
		memcpy(dest_addr, (unsigned char*)&iph->daddr, 4);
		if ( 0xe0 != (dest_addr[0] & 0xf0) )
			return 0;

		/* drop it when multicast vlanid isn't cofigured correctly. */
		if ( match_multicast_vtag_check
			&& VTAG_SUCCESS != match_multicast_vtag_check(skb, vdev) )
			return -1;
	}
#ifdef TCSUPPORT_IPV6
	else if ( PPP_IPV6 == ppp_proto ) /* IPv6 */
	{
		ip6h = (struct ipv6hdr* )(pppoe_h);
		nexthdr = ip6h->nexthdr;
		offset = ipv6_skip_exthdr(skb, skip_start, &nexthdr);
		if ( offset >= 0 && IPPROTO_UDP == nexthdr )
		{
			memcpy(dest_addr, ip6h->daddr.s6_addr, 16);
	 		if ( 0xff != dest_addr[0] )
				return 0;

			/* drop it when multicast vlanid isn't cofigured correctly. */
			if ( match_multicast_vtag_check
				&& VTAG_SUCCESS != match_multicast_vtag_check(skb, vdev) )
				return -1;
		}
	}
#endif
#endif

	return 0; /* check ok or ignore. */
}
EXPORT_SYMBOL(check_ppp_udp_multicast);

/****************************************************************************
**function name
	 __is_ip_udp
**description:
	check whether packet is IP udp packets.
**return 
	0:	check ok or ignore
	-1:	fail
**parameter:
	skb: the packet information
	vdev: virtual net device
****************************************************************************/
int __is_ip_udp(struct sk_buff *skb)
{
#if defined(TCSUPPORT_CT_PON_SC)
	struct iphdr *iph = NULL;
#ifdef TCSUPPORT_IPV6
	struct ipv6hdr *ip6h = NULL;
#endif
	u8 nexthdr = 0;
	int offset = 0, skip_start = 0;

	if ( htons(ETH_P_IP) == skb->protocol )
		iph = ip_hdr(skb);
	else if( ( htons(ETH_P_8021Q) == skb->protocol )
			&& ( htons(ETH_P_IP) == __vlan_proto(skb) ) )
		iph = (struct iphdr *)(skb_mac_header(skb) + VLAN_ETH_HLEN);
#ifdef TCSUPPORT_IPV6	
	else if ( skb->protocol == htons(ETH_P_IPV6) )
		ip6h = ipv6_hdr(skb);	
	else if( (skb->protocol == htons(ETH_P_8021Q))
			&& (__vlan_proto(skb) == htons(ETH_P_IPV6)) )
		ip6h = (struct iphdr *)(skb_mac_header(skb) + VLAN_ETH_HLEN);
#endif	
	else
		return 0;

	if ( iph && IPPROTO_UDP == iph->protocol )
		return 1;
#ifdef TCSUPPORT_IPV6
	else if (ip6h )
	{
		nexthdr = ip6h->nexthdr;
		offset = ipv6_skip_exthdr(skb, skip_start, &nexthdr);
		if ( offset >= 0 && IPPROTO_UDP == nexthdr )
			return 1;
	}
#endif
	else
		return 0;
#else
	return 0;
#endif
}
EXPORT_SYMBOL(__is_ip_udp);

/****************************************************************************
**function name
	 tr143RxShortCut
**description:
	tr143 test shortcut data path
**return 
	0:	receive succeed.
	-1:	fail
**parameter:
	skb: the packet information
	vlanLayer: vlan layer counts
	ifaceidx: interface index
	iptype: ip type, 1: IP, 2:PPP, 3:dslite+IP, 4:dslite+PPP 
****************************************************************************/
int tr143RxShortCut(int enable
, struct sk_buff *skb
, int vlanLayer
, int ifaceidx
, int iptype
)
{
#if defined(TCSUPPORT_CT_PON_GDV20) || defined(TCSUPPORT_IS_FH_PON)
	struct iphdr *iph = NULL;
	struct net_device *dev = NULL;
	int isPPP = 0, isDslite = 0;
	char devName[IFNAMSIZ] = {0};
#define IP6_NETWORK_HLEN	40
#define MAX_SMUX_NUM 8

	switch ( iptype )
	{
		case 1: /* IP */
		case 3: /* dslite+IP */
			isDslite = (3 == iptype ? 1 : 0 );
			snprintf(devName, sizeof(devName) - 1, "%snas%d_%d"
				, ( 1 == isDslite ? "ds." : "")
				, ifaceidx / MAX_SMUX_NUM
				, ifaceidx % MAX_SMUX_NUM );
			break;
		case 2: /* PPP */
		case 4: /* dslite+PPP */
			isDslite = (4 == iptype ? 1 : 0 );
			snprintf(devName, sizeof(devName) - 1, "%sppp%d"
				, ( 1 == isDslite ? "ds." : "")
				, ifaceidx );
			isPPP = 1;
			break;
		default:
			return -2;
	}
	dev = dev_get_by_name(&init_net, devName);
	if ( !dev )
		return -3;

	skb->pkt_type = PACKET_HOST;
	skb->protocol = htons(ETH_P_IP);
	skb->ip_summed = CHECKSUM_UNNECESSARY ;

	/* remove vlan tag */
	if ( vlanLayer != 0 )
		skb_pull(skb, 4*vlanLayer);
	skb_reset_network_header(skb);
	skb_reset_transport_header(skb);
	skb->mac_len = skb->network_header - skb->mac_header;

	skb->dev = dev;
	if ( !skb->skb_iif )
		skb->skb_iif = skb->dev->ifindex;

	if ( 2 == enable ) /* debug on */
	{
		printk("\nTR143 shortcut dev=[%s], iptype=[%d] \n", devName, iptype);
	}

	/* remove ppp header */
	if ( isPPP )
	{
		skb_pull(skb, PPPOE_SES_HLEN);
		skb_reset_network_header(skb);
	}

	/* remove ipv6 header */
	if ( isDslite )
	{
		skb_pull(skb, IP6_NETWORK_HLEN);
		skb_reset_network_header(skb);

	}

	/* remove ip header */
	skb_pull(skb, ip_hdrlen(skb));
	skb_reset_transport_header(skb);
	iph = ip_hdr(skb);
	if ( NULL == skb_dst(skb) )
	{
		ip_route_input_noref(skb, iph->daddr, iph->saddr,
								   iph->tos, skb->dev);
	}

	/* Point into the IP datagram, just past the header. */
	tcp_v4_rcv(skb);
	dev_put(dev);
#endif

	return 0;
}
EXPORT_SYMBOL(tr143RxShortCut);


