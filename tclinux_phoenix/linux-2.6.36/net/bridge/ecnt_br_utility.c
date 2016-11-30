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
#include "br_private.h"
#include <linux/ip.h>
#ifdef TCSUPPORT_IPV6
#include <linux/ipv6.h>
#endif
#include <linux/udp.h>
#include <linux/uaccess.h>
#include <linux/if_bridge.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/if.h>
#include <linux/types.h>
#include <linux/rculist.h>
#include <linux/jiffies.h>
#include "br_private.h"



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


/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

/****************************************************************************
**function name
	 get_client_mac_from_dhcp_packet
**description:
	get clinet mac from broadcast dhcp packet
**return 
	0: success
	-1: fail
**parameter:
	skb: the packet information
	cliMacAddr: client MAC Address
****************************************************************************/
int get_client_mac_from_dhcp_packet(struct sk_buff *skb, unsigned char *cliMacAddr)
{
	int eth_type = 0;
	int dhcp_mode = 0;
	struct iphdr *ip_hdr = NULL;
	struct udphdr *udp_hdr = NULL;
	unsigned char *p = skb->data;
	
	if(skb == NULL)
		return -1;
	if(skb->dev == NULL)
		return -1;

	p = p + 12;//point to ethernet type
	eth_type = *(unsigned short *)p;
	if(eth_type == 0x8100){
		p = p + 4;
		eth_type = *(unsigned short *)p;
		if(eth_type == 0x8100){
			p = p + 4;
			eth_type = *(unsigned short *)p;
		}
	}

	if (eth_type != ETH_P_IP){
		return -1;
	}
	
	p = p + 2;//point to ip header
	ip_hdr = (struct iphdr *)p;
	if(ip_hdr->protocol != 17) {//not udp
		return -1;
	}
	
	p = p + 20; //point to UDP header
	udp_hdr = (struct udphdr *)p;
	if(udp_hdr->source == 68 || udp_hdr->dest == 67)
	{
		dhcp_mode = 1; //dhcp boot request
	}
	else if(udp_hdr->source == 67 || udp_hdr->dest == 68)
	{
		dhcp_mode = 2; //dhcp boot reply
	}
	
	if(dhcp_mode == 0){//not dhcp
		return -1;
	}
	
	p = p + 8; //point to DHCP 
	p += 28;//poinrt to Client Mac address
	memcpy(cliMacAddr, p, 6);
	
	return 0;
	
}

/****************************************************************************
**function name
	 get_fdb_by_skb
**description:
	get net bridge fdb entry by skb information
**return 
	dst: if find match entry
	NULL: if not find match entry
**parameter:
	skb: the packet information
****************************************************************************/
struct net_bridge_fdb_entry *get_fdb_by_skb(struct sk_buff *skb)
{
	unsigned char dest[6] = {0};
	struct net_bridge_fdb_entry *dst = NULL;	
	struct net_bridge_port *p = br_port_get_rcu(skb->dev);
	struct net_bridge *br;

	if (!p || p->state == BR_STATE_DISABLED)
		return NULL;

	memcpy(dest, skb->data, sizeof(dest));
	if(is_broadcast_ether_addr(dest))
	{
		if(get_client_mac_from_dhcp_packet(skb, dest) == -1)
		{
			return dst;
		}

	}

	br = p->br;
	dst = __br_fdb_get(br, dest);

	return dst;
}
EXPORT_SYMBOL(get_fdb_by_skb);

char *gc_ifname[] = {
#if defined(TCSUPPORT_FWC_ENV)
	"eth0",
	"eth1",
	"eth2",
	"eth3",
#else
	"eth0.1",
	"eth0.2",
	"eth0.3",
	"eth0.4",
#endif
	"ra0",
	"ra1",
	"ra2",
	"ra3"
};

char *gc_ifname_wan[] = {
	"pon",
	"nas"	
};

typedef enum _port_list_
{
	E_ETH_PORT_0  = 0,
	E_ETH_PORT_1  = 1,
	E_ETH_PORT_2  = 2,
	E_ETH_PORT_3  = 3,
	
	E_WIFI_PORT_0 = 4,
	E_WIFI_PORT_1 = 5,
	E_WIFI_PORT_2 = 6,
	E_WIFI_PORT_3 = 7,
	
	E_WAN_PORT    = 15,
	E_MAX_PORT    = 16,
}e_port_list;


int customer_dev_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
	struct net_bridge *br = netdev_priv(dev);
	unsigned long args[4];

	if (copy_from_user(args, rq->ifr_data, sizeof(args)))
		return -EFAULT;

	switch (args[0]) 
	{
	}

	return -EOPNOTSUPP; 
}

#if defined(TCSUPPORT_L2_PPP_MCAST_SHORTCUT)
/****************************************************************************
**function name
	 isIpv4PppoeMulticast
**description:
	 judge whether IPV4 pppoe multicast data.
**return 
	0: no
	1: yes
**parameter:
	buff: skb data pointer
****************************************************************************/
int isIpv4PppoeMulticast(unsigned char* buff)
{
	buff += 16;
	if((*buff & 0xf0) != 0xe0){	/* Multicast IP */
		return 0;
	}

	buff -= 7;
	if((*buff & 0xff) == 0x11)	/* UDP */
		return 1;
	
	return 0;
}

/****************************************************************************
**function name
	 isIpv6PppoeMulticast
**description:
	 judge whether IPV6 pppoe multicast data.
**return 
	0: no
	1: yes
**parameter:
	buff: skb data pointer
****************************************************************************/
int isIpv6PppoeMulticast(unsigned char* buff)
{
	buff += 24;
	if((*buff & 0xff) != 0xff){	/* Multicast IPV6 */
		return 0;
	}

	buff -= 18;
	if((*buff & 0xff) == 0x11)	/* UDP */
		return 1;
		
	return 0;
}

/****************************************************************************
**function name
	 isPPPoEMulticastPacket
**description:
	 judge whether pppoe multicast data.
**return 
	0: no
	1: yes
**parameter:
	skb: packet skb
****************************************************************************/
int isPPPoEMulticastPacket(struct sk_buff *skb)
{
	unsigned char *cp = skb->data;	
	unsigned char tagNum = 0;
	unsigned char* buff = skb->data;
	unsigned short tpid = 0;
	unsigned short eth_type= 0;
	unsigned short pppoe_proto = 0;
	int ret=0;
	cp += 12;

	while((tpid =((*cp << 8) + *(cp + 1))) == 0x8100 ||(tpid == 0x88a8)||(tpid == 0x9100))
	{
		tagNum ++;
		cp += 4;
	}
	
	buff += (12+4*tagNum);
	eth_type = *(unsigned short*)buff;
	
	if (eth_type != ETH_P_PPP_SES)
	{
		return 0;
	}

	buff += 2;/*skip ether type*/
	buff += 6;/* pppoe header */
	pppoe_proto = *(unsigned short*)buff;

	if (pppoe_proto == 0x0021){
		buff += 2;
		ret = isIpv4PppoeMulticast(buff);
	}
	else if (pppoe_proto == 0x0057){
		buff += 2;
		ret = isIpv6PppoeMulticast(buff);
	}
	
	return ret;
}

/****************************************************************************
**function name
	 multicast_shortcut_handle
**description:
	 short cut handle
**return 
	0: fail
	1: sucess
**parameter:
	skb: packet skb
****************************************************************************/
int multicast_shortcut_handle(struct sk_buff* skb)
{
	unsigned char *cp = skb->data;	
	unsigned char tagNum = 0;
	unsigned short tpid = 0;
	struct net_bridge_fdb_entry *dst;
	struct net_device *ndev = NULL;
	struct net_bridge *br = NULL;
	struct net_device *dev = NULL;
	cp += 12;
	
	while((tpid =((*cp << 8) + *(cp + 1))) == 0x8100 ||(tpid == 0x88a8)||(tpid == 0x9100))
	{
		tagNum ++;
		cp += 4;
	}

	const unsigned char *dest = skb->data;
	ndev = dev_get_by_name(&init_net, "br0");
	if ( ndev )
		br = netdev_priv(ndev);
	
	if ((dst = __br_fdb_get(br, dest)) != NULL)
	{
		if(dst->is_local)
			return 0;
		
		dev = dst->dst->dev;
		if(dev)
		{
			/*do vlan operation*/
			memmove(skb->data+4*tagNum, skb->data, 12);
			skb_pull(skb, 4*tagNum);
			
			skb->dev = dst->dst->dev;
			skb->dev->netdev_ops->ndo_start_xmit(skb,skb->dev);
			dev_put(dev);

			return 1;
		}
	}

	return 0;
}

/****************************************************************************
**function name
	 multicast_shortcut_handle
**description:
	 pppoe bridge multicast short cut handle
**return 
	0: fail
	1: sucess
**parameter:
	skb: packet skb
****************************************************************************/
int pppoe_bridge_multicast_handle(struct sk_buff* skb)
{
	if(isPPPoEMulticastPacket(skb)){
		return multicast_shortcut_handle(skb);
	}

	return 0;
}

EXPORT_SYMBOL(pppoe_bridge_multicast_handle);
#endif
