/*

*/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/skbuff.h>
#include <linux/proc_fs.h>
#include <ecnt_hook/ecnt_hook_fe.h>
#include <ecnt_hook/ecnt_hook_traffic_classify.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/ipv6.h>
#include <linux/ip.h>
#include "parse_traffic.h"
#include "traffic_classify_itf.h"

int parse_dhcpv6_hdr(struct sk_buff *skb, ecn_traffic_classify_info_t *entry, uint32 userdata) {
  dhcpv6hdr_t *dhcphdr = (dhcpv6hdr_t *)(((char *)eth_hdr(skb))+userdata);
  int flags = 5;

  entry->fld_element.dhcpv6_msg_type = dhcphdr->type;
  entry->fld_element.fld_bit_map    |= ECN_FLD_DHCPV6_MESSAGE_TYPE;
  return flags;
}

/* parse tcp header */
int parse_tcp_hdr(struct sk_buff *skb, ecn_traffic_classify_info_t *entry, int ipv4, uint32 userdata) {
	struct tcphdr *tcph = (struct tcphdr *)(((char *)eth_hdr(skb))+userdata);
	int flags = 4;

	if(ipv4) { //ipv4 tcp
		entry->fld_element.l4_ipv4_dst_port = tcph->dest;
		entry->fld_element.l4_ipv4_src_port = tcph->source;

		entry->fld_element.fld_bit_map |= ECN_FLD_L4_SA_PORT;
		entry->fld_element.fld_bit_map |= ECN_FLD_L4_DA_PORT;
	}else {    //ipv6 tcp
		entry->fld_element.l4_ipv6_dst_port = tcph->dest;
		entry->fld_element.l4_ipv6_src_port = tcph->source;

		entry->fld_element.fld_bit_map |= ECN_FLD_IPV6_L4_SOURCE_PORT;
		entry->fld_element.fld_bit_map |= ECN_FLD_IPV6_L4_DEST_PORT;
	}
	return flags;
}

/* parse udp header */
int parse_udp_hdr(struct sk_buff *skb, ecn_traffic_classify_info_t *entry, int ipv4, uint32 userdata) {
	struct udphdr *udph = (struct udphdr *)(((char *)eth_hdr(skb))+userdata);
	int flags = 4;

	if(ipv4) {
		entry->fld_element.l4_ipv4_dst_port = udph->dest;
		entry->fld_element.l4_ipv4_src_port = udph->source;

		entry->fld_element.fld_bit_map |= ECN_FLD_L4_SA_PORT;
		entry->fld_element.fld_bit_map |= ECN_FLD_L4_DA_PORT;
	}else {
		entry->fld_element.l4_ipv6_dst_port = udph->dest;
		entry->fld_element.l4_ipv6_src_port = udph->source;

		entry->fld_element.fld_bit_map |= ECN_FLD_IPV6_L4_SOURCE_PORT;
		entry->fld_element.fld_bit_map |= ECN_FLD_IPV6_L4_DEST_PORT;
		userdata += sizeof(struct udphdr);
		
		if(entry->fld_element.l4_ipv6_dst_port == DHCPV6_CLI_PORT ||
		   entry->fld_element.l4_ipv6_dst_port == DHCPV6_SER_PORT) {
			flags = parse_dhcpv6_hdr(skb, entry, userdata);
		}
	}

	return flags;
}

/*	parse ipv4 related infomation */
int parse_ipv4_hdr(struct sk_buff *skb, ecn_traffic_classify_info_t *entry, uint32 userdata) {
	int flags = 3;
	struct iphdr *iph = (struct iphdr *)(((char *)eth_hdr(skb))+userdata);
    
	entry->fld_element.ipv4_dst_ip.s_addr  = iph->daddr;
	entry->fld_element.ipv4_src_ip.s_addr  = iph->saddr;
	entry->fld_element.tos                 = iph->tos;
	entry->fld_element.ipv4_ttl            = iph->ttl;
	entry->fld_element.ip_version          = iph->version;
	entry->fld_element.ip_type             = iph->protocol;

	entry->fld_element.fld_bit_map |= ECN_FLD_SA_IP;
	entry->fld_element.fld_bit_map |= ECN_FLD_DA_IP;
	entry->fld_element.fld_bit_map |= ECN_FLD_IPV4_TRAFFIC_CLASS;
	entry->fld_element.fld_bit_map |= ECN_FLD_TTL;
	entry->fld_element.fld_bit_map |= ECN_FLD_IP_VER;
	entry->fld_element.fld_bit_map |= ECN_FLD_IP_PROTOCOL_TYPE;
	
	userdata += iph->ihl*4;
	switch(iph->protocol) {
		case TCP:
			flags = parse_tcp_hdr(skb, entry, 1, userdata);
			break;
		case UDP:
			flags = parse_udp_hdr(skb, entry, 1, userdata);
			break;
		default:
			flags = 3;		
	}
	return flags;
}

/* parse ipv6 related infomation */
int parse_ipv6_hdr(struct sk_buff *skb, ecn_traffic_classify_info_t *entry, uint32 userdata) {
	int flags = 3;
	ipv6hdr_t *ipv6h = (ipv6hdr_t *)(((char *)eth_hdr(skb))+userdata); 

	entry->fld_element.flow_lbl[2] 			= ipv6h->flowlbl&0xff;
	entry->fld_element.flow_lbl[1] 			= (ipv6h->flowlbl>>8)&0xff;
	entry->fld_element.flow_lbl[0] 			= (ipv6h->flowlbl>>16)&0xff;
	entry->fld_element.hop_limit   			= ipv6h->hoplimit;
	entry->fld_element.ipv6_traffic_class   = ipv6h->traffic;
	entry->fld_element.ipv6_dscp            = (ipv6h->traffic&0xfc)>>2;
	entry->fld_element.nexthdr              = ipv6h->nexthr;
	entry->fld_element.ip_version           = ipv6h->version;
	memcpy(entry->fld_element.ipv6_daddr.s6_addr, ipv6h->daddr, sizeof(struct in6_addr));
	memcpy(entry->fld_element.ipv6_saddr.s6_addr, ipv6h->saddr, sizeof(struct in6_addr));
	memcpy(entry->fld_element.ipv6_daddr_hdr.s6_addr, ipv6h->daddr, IPV6_HEAD);
	memcpy(entry->fld_element.ipv6_saddr_hdr.s6_addr, ipv6h->saddr, IPV6_HEAD);

	entry->fld_element.fld_bit_map |= ECN_FLD_IPV6_FLOW_LABEL;
	entry->fld_element.fld_bit_map |= ECN_FLD_IPV6_HOP_LIMIT;
	entry->fld_element.fld_bit_map |= ECN_FLD_IPV6_TRAFFIC_CLASS;
	entry->fld_element.fld_bit_map |= ECN_FLD_IPV6_DSCP;
	entry->fld_element.fld_bit_map |= ECN_FLD_IPV6_NEXT_HEADER;
	entry->fld_element.fld_bit_map |= ECN_FLD_IP_VER;
	entry->fld_element.fld_bit_map |= ECN_FLD_DA_IPV6;
	entry->fld_element.fld_bit_map |= ECN_FLD_SA_IPV6;
	entry->fld_element.fld_bit_map |= ECN_FLD_DA_IPV6_HEAD;
	entry->fld_element.fld_bit_map |= ECN_FLD_SA_IPV6_HEAD;

	userdata += sizeof(ipv6hdr_t);
	switch(entry->fld_element.nexthdr) {
		case TCP:
			flags = parse_tcp_hdr(skb, entry, 0, userdata);
			break;
		case UDP:
			flags = parse_udp_hdr(skb, entry, 0, userdata);
			break;
		default:
			flags = 3;	 
	}
	return flags;
}

/*	*/
int classify_flow_hook(struct sk_buff *skb, ecn_traffic_classify_info_t *entry) {

	/*parse mac*/
	mac_basic_t * mac_basic = (mac_basic_t *)eth_hdr(skb);
    int          vlan_layer = 0;
	ppp_hdr_t	       *ppp = NULL;
	uint16        etherType = 0;
	uint16         userdata = 0;
	uint32      phydestport = 0;
	int               flags = 2;	   //parse layer
	ecn_vlan_info_t	vlan_info[MAX_IDENIFY_VLAN_LAYER] = {0};
	
	entry->port = ((skb->mark >> PORT_MASK_OFFSET) & PORT_MASK);
	
	if( ((E_ETH_PORT_0 <= entry->port) && (E_ETH_PORT_3 >= entry->port)) || (E_WAN_PORT == entry->port) )
	{
		entry->fld_element.fld_bit_map |= ECN_FLD_SRC_PORT;
	}
	memcpy(entry->fld_element.src_mac, mac_basic->ethaddr.h_source, ETH_ALEN);
	memcpy(entry->fld_element.dst_mac,   mac_basic->ethaddr.h_dest, ETH_ALEN);
	entry->fld_element.fld_bit_map |= ECN_FLD_L2_ETH_TYPE;
	entry->fld_element.fld_bit_map |= ECN_FLD_SA_MAC | ECN_FLD_DA_MAC;
	etherType = mac_basic->ethaddr.h_proto;
	userdata = sizeof(mac_basic_t);

	/*parse vlan*/
	while(((ETH_P_8021Q == etherType) || (0x88a8 == etherType)) && (MAX_IDENIFY_VLAN_LAYER > vlan_layer))
	{
		struct vlan_hdr *vhdr            = (struct vlan_hdr *)(((char *)mac_basic)+userdata);
		vlan_info[vlan_layer].vlan_prio  = (vhdr->h_vlan_TCI & VLAN_PRIO_MASK) >> VLAN_PRIO_SHIFT;
		vlan_info[vlan_layer].vlan_id    = vhdr->h_vlan_TCI & VLAN_VID_MASK;
		vlan_info[vlan_layer].vlan_proto = vhdr->h_vlan_encapsulated_proto;
		etherType                        = vhdr->h_vlan_encapsulated_proto;
		userdata                         += sizeof(struct vlan_hdr);
		vlan_layer++;
	}
	if(0 != vlan_layer)
	{
		entry->fld_element.pbit  = vlan_info[0].vlan_prio;
		entry->fld_element.vid   = vlan_info[0].vlan_id;
		entry->fld_element.fld_bit_map |= ECN_FLD_VLAN_PRIO;
		entry->fld_element.fld_bit_map |= ECN_FLD_VLAN_ID;
	}
        entry->fld_element.ether_type   = etherType;
	/*parse pppoe*/
	if(etherType == ETH_P_PPP_SES) {
		ppp       = (ppp_hdr_t *)(((char *)mac_basic)+userdata);
		userdata += sizeof(ppp_hdr_t);
		etherType = ppp->proto;
	}

	switch(etherType) {
		case ETH_P_IP:	  //parse ipv4
		case PPP_IP_PROTOCOL:
			flags = parse_ipv4_hdr(skb, entry, userdata);
			break;
		case ETH_P_IPV6:	  //parse ipv6
		case PPP_IPV6_PROTOCOL:
			flags = parse_ipv6_hdr(skb, entry, userdata);
			break;
		default:
			flags = 2;
	}
	
	return flags;
}


