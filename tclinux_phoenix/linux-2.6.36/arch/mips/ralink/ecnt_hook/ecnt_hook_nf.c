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
#include <linux/netfilter.h>
#include <linux/init.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/inet.h>
#include <linux/netfilter_bridge.h>
#include <linux/netfilter_arp.h>
#include <linux/netfilter_ipv6.h>
#include <linux/ipv6.h>
#include <ecnt_hook/ecnt_hook_nf.h>

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
unsigned int ipv4_pre_routing_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));
unsigned int ipv4_local_in_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));

unsigned int ipv4_forward_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));

unsigned int ipv4_local_out_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));

unsigned int ipv4_post_routing_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));



unsigned int ipv6_pre_routing_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));

unsigned int ipv6_local_in_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));

unsigned int ipv6_forward_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));

unsigned int ipv6_local_out_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));

unsigned int ipv6_post_routing_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));

unsigned int arp_in_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));
unsigned int arp_forward_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));

unsigned int arp_out_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));

unsigned int bridge_pre_routing_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));

unsigned int bridge_local_in_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));

unsigned int bridge_forward_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));

unsigned int bridge_local_out_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));
unsigned int bridge_post_routing_hook_fn(unsigned int hooknum,
				   struct sk_buff *skb,
				   const struct net_device *in,
				   const struct net_device *out,
				   int (*okfn)(struct sk_buff *));

/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/

/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/
struct nf_hook_ops ipv4_pre_routing_hook_op = {
	.hook = ipv4_pre_routing_hook_fn,
	.pf = NFPROTO_IPV4,
	.hooknum = NF_INET_PRE_ROUTING,
	.priority = NF_IP_PRI_FIRST,
	.owner = THIS_MODULE,
};

struct nf_hook_ops ipv4_local_in_hook_op = {
	.hook = ipv4_local_in_hook_fn,
	.pf = NFPROTO_IPV4,
	.hooknum = NF_INET_LOCAL_IN,
	.priority = NF_IP_PRI_FIRST,
	.owner = THIS_MODULE,
};

struct nf_hook_ops ipv4_forward_hook_op = {
	.hook = ipv4_forward_hook_fn,
	.pf = NFPROTO_IPV4,
	.hooknum = NF_INET_FORWARD,
	.priority = NF_IP_PRI_FIRST,
	.owner = THIS_MODULE,
};

struct nf_hook_ops ipv4_local_out_hook_op = {
	.hook = ipv4_local_out_hook_fn,
	.pf = NFPROTO_IPV4,
	.hooknum = NF_INET_LOCAL_OUT,
	.priority = NF_IP_PRI_FIRST,
	.owner = THIS_MODULE,
};

struct nf_hook_ops ipv4_post_routing_hook_op = {
	.hook = ipv4_post_routing_hook_fn,
	.pf = NFPROTO_IPV4,
	.hooknum = NF_INET_POST_ROUTING,
	.priority = NF_IP_PRI_FIRST,
	.owner = THIS_MODULE,
};

struct nf_hook_ops ipv6_pre_routing_hook_op = {
	.hook = ipv6_pre_routing_hook_fn,
	.pf = NFPROTO_IPV6,
	.hooknum = NF_INET_PRE_ROUTING,
	.priority = NF_IP6_PRI_FIRST,
	.owner = THIS_MODULE,
};

struct nf_hook_ops ipv6_local_in_hook_op = {
	.hook = ipv6_local_in_hook_fn,
	.pf = NFPROTO_IPV6,
	.hooknum = NF_INET_LOCAL_IN,
	.priority = NF_IP6_PRI_FIRST,
	.owner = THIS_MODULE,
};

struct nf_hook_ops ipv6_forward_hook_op = {
	.hook = ipv6_forward_hook_fn,
	.pf = NFPROTO_IPV6,
	.hooknum = NF_INET_FORWARD,
	.priority = NF_IP6_PRI_FIRST,
	.owner = THIS_MODULE,
};

struct nf_hook_ops ipv6_local_out_hook_op = {
	.hook = ipv6_local_out_hook_fn,
	.pf = NFPROTO_IPV6,
	.hooknum = NF_INET_LOCAL_OUT,
	.priority = NF_IP6_PRI_FIRST,
	.owner = THIS_MODULE,
};

struct nf_hook_ops ipv6_post_routing_hook_op = {
	.hook = ipv6_post_routing_hook_fn,
	.pf = NFPROTO_IPV6,
	.hooknum = NF_INET_POST_ROUTING,
	.priority = NF_IP6_PRI_FIRST,
	.owner = THIS_MODULE,
};

struct nf_hook_ops arp_in_hook_op = {
	.hook = arp_in_hook_fn,
	.pf = NFPROTO_ARP,
	.hooknum = NF_ARP_IN,
	.priority = NF_IP_PRI_FIRST,
	.owner = THIS_MODULE,
};

struct nf_hook_ops arp_forward_hook_op = {
	.hook = arp_forward_hook_fn,
	.pf = NFPROTO_ARP,
	.hooknum = NF_ARP_FORWARD,
	.priority = NF_IP_PRI_FIRST,
	.owner = THIS_MODULE,
};

struct nf_hook_ops arp_out_hook_op = {
	.hook = arp_out_hook_fn,
	.pf = NFPROTO_ARP,
	.hooknum = NF_ARP_OUT,
	.priority = NF_IP_PRI_FIRST,
	.owner = THIS_MODULE,
};

struct nf_hook_ops bridge_pre_routing_hook_op = {
	.hook = bridge_pre_routing_hook_fn,
	.pf = NFPROTO_BRIDGE,
	.hooknum = NF_BR_PRE_ROUTING,
	.priority = NF_BR_PRI_FIRST,
	.owner = THIS_MODULE,
};

struct nf_hook_ops bridge_local_in_hook_op = {
	.hook = bridge_local_in_hook_fn,
	.pf = NFPROTO_BRIDGE,
	.hooknum = NF_BR_LOCAL_IN,
	.priority = NF_BR_PRI_FIRST,
	.owner = THIS_MODULE,
};

struct nf_hook_ops bridge_forward_hook_op = {
	.hook = bridge_forward_hook_fn,
	.pf = NFPROTO_BRIDGE,
	.hooknum = NF_BR_FORWARD,
	.priority = NF_BR_PRI_FIRST,
	.owner = THIS_MODULE,
};

struct nf_hook_ops bridge_local_out_hook_op = {
	.hook = bridge_local_out_hook_fn,
	.pf = NFPROTO_BRIDGE,
	.hooknum = NF_BR_LOCAL_OUT,
	.priority = NF_BR_PRI_FIRST,
	.owner = THIS_MODULE,
};

struct nf_hook_ops bridge_post_routing_hook_op = {
	.hook = bridge_post_routing_hook_fn,
	.pf = NFPROTO_BRIDGE,
	.hooknum = NF_BR_POST_ROUTING,
	.priority = NF_BR_PRI_FIRST,
	.owner = THIS_MODULE,
};

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
/*****************************************************************************
//function :
//		ECNT_NF_HOOK
//description : 
//		called by nf_hook_ops hook functions
//input :	
//		unsigned int maintype, unsigned int subtype,struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
int ECNT_NF_HOOK(unsigned int maintype, unsigned int subtype, struct sk_buff *skb, const struct net_device *in, 
					const struct net_device *out)
{
	int ret;
	struct nf_data_s nf_data;
	
	nf_data.skb = skb;
	nf_data.in = in;
	nf_data.out = out;
	
	ret = __ECNT_HOOK(maintype, subtype, (struct ecnt_data *)&nf_data);
	switch(ret){
		case ECNT_RETURN_DROP:
			return NF_DROP;
		case ECNT_CONTINUE:
		case ECNT_HOOK_ERROR:
			return NF_ACCEPT;
		case ECNT_RETURN:
			return NF_STOP;
	}
	return ret;
}

/*****************************************************************************
//function :
//		ipv4_pre_routing_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int ipv4_pre_routing_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_IPV4,ECNT_NF_IP4_PRE_ROUTING,skb,in,out);
}

/*****************************************************************************
//function :
//		ipv4_local_in_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int ipv4_local_in_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_IPV4,ECNT_NF_IP4_LOCAL_IN,skb,in,out);
}

/*****************************************************************************
//function :
//		ipv4_forward_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int ipv4_forward_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_IPV4,ECNT_NF_IP4_FORWARD,skb,in,out);
}

/*****************************************************************************
//function :
//		ipv4_local_out_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int ipv4_local_out_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_IPV4,ECNT_NF_IP4_LOCAL_OUT,skb,in,out);
}

/*****************************************************************************
//function :
//		ipv4_post_routing_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int ipv4_post_routing_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_IPV4,ECNT_NF_IP4_POST_ROUTING,skb,in,out);
}

/*****************************************************************************
//function :
//		ipv6_pre_routing_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int ipv6_pre_routing_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_IPV6,ECNT_NF_IP6_PRE_ROUTING,skb,in,out);
}

/*****************************************************************************
//function :
//		ipv6_local_in_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int ipv6_local_in_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_IPV6,ECNT_NF_IP6_LOCAL_IN,skb,in,out);
}

/*****************************************************************************
//function :
//		ipv6_forward_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int ipv6_forward_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_IPV6,ECNT_NF_IP6_FORWARD,skb,in,out);
}

/*****************************************************************************
//function :
//		ipv6_local_out_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int ipv6_local_out_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_IPV6,ECNT_NF_IP6_LOCAL_OUT,skb,in,out);
}

/*****************************************************************************
//function :
//		ipv6_post_routing_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int ipv6_post_routing_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_IPV6,ECNT_NF_IP6_POST_ROUTING,skb,in,out);
}

/*****************************************************************************
//function :
//		arp_in_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int arp_in_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_ARP,ECNT_NF_ARP_IN,skb,in,out);
}

/*****************************************************************************
//function :
//		arp_forward_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int arp_forward_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_ARP,ECNT_NF_ARP_FORWARD,skb,in,out);
}

/*****************************************************************************
//function :
//		arp_out_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int arp_out_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_ARP,ECNT_NF_ARP_OUT,skb,in,out);
}

/*****************************************************************************
//function :
//		bridge_pre_routing_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int bridge_pre_routing_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_BR,ECNT_NF_BR_PRE_ROUTING,skb,in,out);
}

/*****************************************************************************
//function :
//		bridge_local_in_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int bridge_local_in_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_BR,ECNT_NF_BR_LOCAL_IN,skb,in,out);
}

/*****************************************************************************
//function :
//		bridge_forward_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int bridge_forward_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_BR,ECNT_NF_BR_FORWARD,skb,in,out);
}

/*****************************************************************************
//function :
//		bridge_local_out_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int bridge_local_out_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_BR,ECNT_NF_BR_LOCAL_OUT,skb,in,out);
}

/*****************************************************************************
//function :
//		bridge_post_routing_hook_fn
//description : 
//		nf_hook_ops hook function
//input :	
//		unsigned int hooknum, struct sk_buff *skb,
//		const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)
//return :
//		NF_DROP
//		NF_ACCEPT
//		NF_STOP
******************************************************************************/
unsigned int bridge_post_routing_hook_fn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{
	return ECNT_NF_HOOK(ECNT_NF_BR,ECNT_NF_BR_POST_ROUTING,skb,in,out);
}

/*****************************************************************************
//function :
//		nf_register_ipv4_hook
//description : 
//		register IPv4 nf_hook_ops
//input :	
//		void
//return :
//		-1:fail
//		0: success
******************************************************************************/
int nf_register_ipv4_hook(void)
{
	if(nf_register_hook(&ipv4_pre_routing_hook_op)){
		printk("nf_register_hook() ipv4_pre_routing failed\n");
		return -1;
	}
	if(nf_register_hook(&ipv4_local_in_hook_op)){
		printk("nf_register_hook() ipv4_local_in_hook_op failed\n");
		return -1;
	}
	if(nf_register_hook(&ipv4_forward_hook_op)){
		printk("nf_register_hook() ipv4_forward_hook_op failed\n");
		return -1;
	}
	if(nf_register_hook(&ipv4_local_out_hook_op)){
		printk("nf_register_hook() ipv4_local_out_hook_op failed\n");
		return -1;
	}
	if(nf_register_hook(&ipv4_post_routing_hook_op)){
		printk("nf_register_hook() ipv4_post_routing_hook_op failed\n");
		return -1;
	}
	return 0;
}

/*****************************************************************************
//function :
//		nf_register_ipv6_hook
//description : 
//		register IPv6 nf_hook_ops
//input :	
//		void
//return :
//		-1:fail
//		0: success
******************************************************************************/
int nf_register_ipv6_hook(void)
{
	if(nf_register_hook(&ipv6_pre_routing_hook_op)){
		printk("nf_register_hook() ipv6_pre_routing failed\n");
		return -1;
	}
	if(nf_register_hook(&ipv6_local_in_hook_op)){
		printk("nf_register_hook() ipv6_local_in_hook_op failed\n");
		return -1;
	}
	if(nf_register_hook(&ipv6_forward_hook_op)){
		printk("nf_register_hook() ipv6_forward_hook_op failed\n");
		return -1;
	}
	if(nf_register_hook(&ipv6_local_out_hook_op)){
		printk("nf_register_hook() ipv6_local_out_hook_op failed\n");
		return -1;
	}
	if(nf_register_hook(&ipv6_post_routing_hook_op)){
		printk("nf_register_hook() ipv6_post_routing_hook_op failed\n");
		return -1;
	}
	
	return 0;
}

/*****************************************************************************
//function :
//		nf_register_bridge_hook
//description : 
//		register Bridge nf_hook_ops
//input :	
//		void
//return :
//		-1:fail
//		0: success
******************************************************************************/
int nf_register_bridge_hook(void)
{
	if(nf_register_hook(&bridge_pre_routing_hook_op)){
		printk("nf_register_hook() bridge_pre_routing_hook_op failed\n");
		return -1;
	}
	if(nf_register_hook(&bridge_local_in_hook_op)){
		printk("nf_register_hook() bridge_local_in_hook_op failed\n");
		return -1;
	}
	if(nf_register_hook(&bridge_forward_hook_op)){
		printk("nf_register_hook() bridge_forward_hook_op failed\n");
		return -1;
	}
	if(nf_register_hook(&bridge_local_out_hook_op)){
		printk("nf_register_hook() bridge_local_out_hook_op failed\n");
		return -1;
	}
	if(nf_register_hook(&bridge_post_routing_hook_op)){
		printk("nf_register_hook() bridge_post_routing_hook_op failed\n");
		return -1;
	}
	
	return 0;
}

/*****************************************************************************
//function :
//		nf_register_arp_hook
//description : 
//		register ARP nf_hook_ops
//input :	
//		void
//return :
//		-1:fail
//		0: success
******************************************************************************/
int nf_register_arp_hook(void)
{
	if(nf_register_hook(&arp_in_hook_op)){
		printk("nf_register_hook() arp_in_hook_op failed\n");
		return -1;
	}
	if(nf_register_hook(&arp_forward_hook_op)){
		printk("nf_register_hook() arp_forward_hook_op failed\n");
		return -1;
	}
	if(nf_register_hook(&arp_out_hook_op)){
		printk("nf_register_hook() arp_out_hook_op failed\n");
		return -1;
	}
	return 0;
}

/*****************************************************************************
//function :
//		ecnt_net_filter_hook_init
//description : 
//		register nf_hook_ops, called by sock_init()
//input :	
//		void
//return :
//		-1:fail
//		0: success
******************************************************************************/
int ecnt_net_filter_hook_init(void)
{
	if(nf_register_ipv4_hook()){
		printk("nf_register_ipv4_hook() failed\n");
		return -1;
	}
	
	if(nf_register_ipv6_hook()){
		printk("nf_register_ipv6_hook() failed\n");
		return -1;
	}

	if(nf_register_bridge_hook()){
		printk("nf_register_bridge_hook() failed\n");
		return -1;
	}

	if(nf_register_arp_hook()){
		printk("nf_register_arp_hook() failed\n");
		return -1;
	}
	return 0;
}

