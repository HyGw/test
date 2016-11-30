/***************************************************************
Copyright Statement:

This software/firmware and related documentation (EcoNet Software) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (EcoNet) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (ECONET SOFTWARE) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN AS IS 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVERS SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVERS SPECIFICATION OR CONFORMING TO A PARTICULAR 
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
#include <linux/in.h>
#include <linux/if_vlan.h>
#include <linux/if_ether.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/skbuff.h>
#include <ecnt_hook/ecnt_hook.h>
#include <ecnt_hook/ecnt_hook_net.h>
#include <ecnt_hook/ecnt_hook_traffic_classify.h>
#include <linux/rcupdate.h>
#include <linux/rculist.h>
#include <linux/version.h>
#include "traffic_classify_itf.h"
#include "parse_traffic.h"

unsigned long g_classify_fun_bit_map = (E_ACL_ACTION | E_QUEUE_MAPPING_ACTION | E_PRIORITY_REMARK_ACTION | E_LIMIT_ACTION | E_TOS_REMARK_ACTION);
unsigned long g_CLASSIFY_DEBUG_LEVEL = 0;

struct list_head  classify_list[E_MAX_PORT] = {0};
struct list_head       acl_list[E_MAX_PORT] = {0};
static int         acl_rule_num[E_MAX_PORT] = {0};
static int    classify_rule_num[E_MAX_PORT] = {0};

typedef int (*linemark_ctrl_packet_rx)(void *data, unsigned int len, unsigned char src_port, unsigned int flag);
static spinlock_t classify_traffic_lock;
static linemark_ctrl_packet_rx glm_ctrl_packet_rx = NULL;


static int queue_mapping_action(struct sk_buff* skb, ecn_classify_action_info_t *pt_info);
static int limit_group_remark_action(struct sk_buff* skb, ecn_classify_action_info_t *pt_info);
static int priority_remark_action(struct sk_buff* skb, ecn_classify_action_info_t *pt_info);
ecnt_ret_val traffic_classify_hook(struct ecnt_data *in_data);
ecnt_ret_val ecnt_line_mark_hook(struct ecnt_data *in_data);
static int print_config_para(ecn_traffic_classify_info_t *entry);
static int print_parse_packet_para(ecn_traffic_classify_info_t *entry, int line);
static int clean_Entry(ecn_traffic_classify_info_t *info, int type);


classify_action_handle_t classify_action_handle_tbl[] =
{	{E_ACL_ACTION, 								NULL},
	{E_QUEUE_MAPPING_ACTION,                 	queue_mapping_action},	
	{E_PRIORITY_REMARK_ACTION,      			priority_remark_action},
	{E_LIMIT_ACTION,             				limit_group_remark_action},
	{E_TOS_REMARK_ACTION,						NULL},
};

#define CLASSIFY_ACTION_SIZE  (sizeof(classify_action_handle_tbl) / sizeof(classify_action_handle_t))

struct ecnt_hook_ops ecnt_flow_classify_op = 
{
	.name       = "ecnt_traffic_classify",
	.is_execute = 1,
	.hookfn     = traffic_classify_hook,
	.maintype   = ECNT_TRAFFIC_CLASSIFY,
	.subtype    = ECNT_TCCBF_SUBTYPE,
	.priority   = 1
};

struct ecnt_hook_ops ecnt_line_mark_ops = {
    .name = "ecnt_line_mark_handle",
    .is_execute = 1,
    .hookfn = ecnt_line_mark_hook,
    .maintype = ECNT_NET_CORE_DEV,
    .subtype = ECNT_DEV_QUE_XMIT,
    .priority = 1
};


static int limit_group_remark_action(struct sk_buff* skb, ecn_classify_action_info_t *pt_info)
{
	skb->mark &= (~(LIMIT_GROUP_REMARK_MASK << LIMIT_GROUP_REMARK_OFFSET));
	skb->mark |= ((pt_info->limit_group_id & LIMIT_GROUP_REMARK_MASK) << LIMIT_GROUP_REMARK_OFFSET);

	return 0;
}

static int judge_dhcp_protocol(ecn_traffic_classify_info_t *pt_info)
{
	if(pt_info->fld_element.fld_bit_map & ECN_FLD_IP_PROTOCOL_TYPE)
	{
		if(pt_info->fld_element.ip_type == UDP)
		{
			if(pt_info->fld_element.l4_ipv4_src_port == DHCP_CLI_PORT ||
			   pt_info->fld_element.l4_ipv4_src_port == DHCP_SER_PORT)
			   return 1;
		}
	}

	return 0;
}

static int judge_ppoe_protocol(ecn_traffic_classify_info_t *pt_info)
{
	if(pt_info->fld_element.fld_bit_map & ECN_FLD_L2_ETH_TYPE)
	{
		if(pt_info->fld_element.ether_type == ETH_P_PPP_DISC)
		{
			return 1;
		}
	}

	return 0;
}
/*
	0 ԭ
	1 ponӿ
*/
static int line_mark_action(struct sk_buff* skb, ecn_traffic_classify_info_t *pt_info)
{
	int port_num = (skb->mark>>PORT_MASK_OFFSET) & PORT_MASK;
	/*
		0 ûƥ䵽ppoedhcp
		1 dhcp
		2 ppoe
	*/
	int dhcp_ppoe_flag = 0;
	int res = 0;

	CLASSIFY_DEBUG_INFO("enter %s...\n", __FUNCTION__);

	/*
		жdhcp
	*/
	
	if(0 == dhcp_ppoe_flag)
	{
		if(judge_dhcp_protocol(pt_info))
		{
			dhcp_ppoe_flag |= (1<<DHCP_MARK_OFFSET);
		}
	}
	
	if(0 == dhcp_ppoe_flag)
	{
		if(judge_ppoe_protocol(pt_info))
		{
			dhcp_ppoe_flag |= (1<<PPOE_MARK_OFFSET);
		}
	}	
	if(0 != dhcp_ppoe_flag)
	{
		if(glm_ctrl_packet_rx)
		{
			CLASSIFY_DEBUG_INFO("skb->len = %d, src_port = %d\n", skb->len, port_num);
			res = glm_ctrl_packet_rx(eth_hdr(skb), skb->len, port_num, dhcp_ppoe_flag);
		}
	}
	return res;
}


static int priority_remark_action(struct sk_buff* skb, ecn_classify_action_info_t *pt_info)
{
	
	unsigned int eth_type = eth_hdr(skb)->h_proto;
	unsigned char* buff   = skb_mac_header(skb);
	unsigned short *vlan_tci = NULL;
	
	if (eth_type == ETH_P_8021Q || eth_type == 0x88a8)
	{
		vlan_tci = (unsigned short*)(buff + (2 * ETH_ALEN + sizeof(unsigned short)));
		*vlan_tci = ((*vlan_tci) & (~VLAN_PRIO_MASK));
		*vlan_tci |= ((pt_info->remark_pbit & PRIO_REMARK_MASK) << VLAN_PRIO_SHIFT);	
		skb->mark |= (1<<PRIO_REMARK_EXIST_OFFSET);
	}

	return 0;
}

static int queue_mapping_action(struct sk_buff* skb, ecn_classify_action_info_t *pt_info)
{
	skb->mark &= (~(QUQUE_REMARK_MASK << QUEUE_REMARK_OFFSET));
	skb->mark |= ((pt_info->remark_queue & QUQUE_REMARK_MASK) << QUEUE_REMARK_OFFSET);
	skb->mark |= (1<<QUQUE_REMARK_EXIST_OFFSET);
	
	return 0;
}

static int traffic_classify_action(struct sk_buff* skb, ecn_classify_action_info_t *pt_info)
{
	int                               i = 0;
	int 						   mask = 0;
	int                       remainder = 0;
	int  							res = 0;
	int						action_bit  = 0;
	int				  max_valid_act_bit = 0;
	int array[MAX_ACTION_BIT_MAP_VALUE] = {0};	

	mask = pt_info->action_bit_map;
	do	
	{	  
		remainder = (mask % 2);	  
		mask      = (mask / 2);	 
		array[i]  = remainder; 
		i++;	
	}while((0 != mask) && (i<MAX_ACTION_BIT_MAP_VALUE));

	max_valid_act_bit = i;
	for(i=0; i<max_valid_act_bit; i++)	
	{	
		if((1 == array[i]) && (i < CLASSIFY_ACTION_SIZE)) 
		{
			action_bit = (1 << i);
			if((action_bit == classify_action_handle_tbl[i].classify_action) &&  (0 != (classify_action_handle_tbl[i].classify_action & g_classify_fun_bit_map)))
			{
				if(NULL != classify_action_handle_tbl[i].classify_action_handle)
				{
					classify_action_handle_tbl[i].classify_action_handle(skb, pt_info);
				}
			}
		}
	}

	return 0;
}

void del_list_mem(struct rcu_head *head) 
{
	ecn_traffic_classify_list_t *entry = NULL;

	entry = container_of(head, ecn_traffic_classify_list_t, head);
	if(entry != NULL) 
	{
		kfree(entry);
	}
}

/*match fld element*/
static int match_fld_element(ecn_fld_element_t *entry_info, ecn_fld_element_t *packet_info, unsigned int fld_bit_map)
{	
	switch(fld_bit_map)
	{
		case ECN_FLD_SRC_PORT:	
		{
			return E_MATCH_FLD_BIT;
		}
		
		case ECN_FLD_SA_MAC:
		{
			return ((0 == memcmp(entry_info->src_mac, packet_info->src_mac, ETH_ALEN)) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}

		case ECN_FLD_DA_MAC: 
		{
			return ((0 == memcmp(entry_info->dst_mac, packet_info->dst_mac, ETH_ALEN)) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_SA_IP:
		{
			return ((entry_info->ipv4_src_ip.s_addr == packet_info->ipv4_src_ip.s_addr) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_DA_IP:
		{
			return ((entry_info->ipv4_dst_ip.s_addr == packet_info->ipv4_dst_ip.s_addr) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_VLAN_ID:
		{
			return ((entry_info->vid == packet_info->vid) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_L2_ETH_TYPE:
		{
			return ((entry_info->ether_type == packet_info->ether_type) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_IP_PROTOCOL_TYPE:
		{
			return ((entry_info->ip_type== packet_info->ip_type) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_VLAN_PRIO:
		{
			return ((entry_info->pbit == packet_info->pbit) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_IPV4_TRAFFIC_CLASS: 
		{
			return ((entry_info->tos== packet_info->tos) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_L4_SA_PORT:
		{
			return ((entry_info->l4_ipv4_src_port== packet_info->l4_ipv4_src_port) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_L4_DA_PORT:
		{
			return ((entry_info->l4_ipv4_dst_port== packet_info->l4_ipv4_dst_port) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_TTL:
		{
			return ((entry_info->ipv4_ttl== packet_info->ipv4_ttl) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_IPV6_DSCP: 
		{
			return ((entry_info->ipv6_dscp == packet_info->ipv6_dscp) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_IP_VER:
		{
			return ((entry_info->ip_version == packet_info->ip_version) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_IPV6_FLOW_LABEL:
		{
			return ((0 == memcmp(entry_info->flow_lbl, packet_info->flow_lbl, sizeof(entry_info->flow_lbl))) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_DA_IPV6:
		{
			return ((0 == memcmp(entry_info->ipv6_daddr.s6_addr, packet_info->ipv6_daddr.s6_addr, sizeof(entry_info->ipv6_daddr.s6_addr))) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_SA_IPV6:
		{
			return ((0 == memcmp(entry_info->ipv6_saddr.s6_addr, packet_info->ipv6_saddr.s6_addr, sizeof(entry_info->ipv6_saddr.s6_addr))) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_DA_IPV6_HEAD:
		{
			return ((0 == memcmp(entry_info->ipv6_daddr_hdr.s6_addr, packet_info->ipv6_daddr_hdr.s6_addr, IPV6_HEAD)) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_SA_IPV6_HEAD:
		{
			return ((0 == memcmp(entry_info->ipv6_saddr_hdr.s6_addr, packet_info->ipv6_saddr_hdr.s6_addr, IPV6_HEAD)) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_IPV6_NEXT_HEADER:	
		{
			return ((entry_info->nexthdr == packet_info->nexthdr) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_IPV6_TRAFFIC_CLASS:
		{
			return ((entry_info->ipv6_traffic_class == packet_info->ipv6_traffic_class) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_IPV6_HOP_LIMIT:
		{
			return ((entry_info->hop_limit == packet_info->hop_limit) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}	
		case ECN_FLD_IPV6_L4_SOURCE_PORT:
		{
			return ((entry_info->l4_ipv6_src_port == packet_info->l4_ipv6_src_port) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}				
		case ECN_FLD_IPV6_L4_DEST_PORT:
		{
			return ((entry_info->l4_ipv6_dst_port == packet_info->l4_ipv6_dst_port) ? E_MATCH_FLD_BIT : E_NOT_MATCH_FLD_BIT);
		}
		case ECN_FLD_PHY_DEST_PORT: 	 
		case ECN_FLD_DHCPV6_MESSAGE_TYPE: 
		defualt:
		{
			return E_NOT_MATCH_FLD_BIT;
		}
	}

	return E_NOT_MATCH_FLD_BIT;
}

/*match fld entry*/
static int match_fld_entry(ecn_fld_element_t *pt_entry_info, ecn_fld_element_t *pt_packet_info)
{
	int                               i = 0;
	int 						   mask = 0;
	int                       remainder = 0;
	int				  max_valid_fld_bit = 0;
	int 	   array[MAX_FLD_MAP_VALUE] = {0};	
	
	if((0 == pt_entry_info->fld_bit_map) || ((pt_packet_info->fld_bit_map & pt_entry_info->fld_bit_map) != pt_entry_info->fld_bit_map))
	{
		return E_NOT_MATCH_FLD_ENTRY;
	}
	
	mask = pt_entry_info->fld_bit_map;
	do	
	{	  
		remainder = (mask % 2);	  
		mask      = (mask / 2);	 
		array[i]  = remainder; 
		i++;	
	}while((0 != mask) && (i<MAX_FLD_MAP_VALUE)); 

	max_valid_fld_bit = i;
	for(i=0; i<max_valid_fld_bit; i++) 
	{
		if(1 == array[i]) 
		{
			if(E_NOT_MATCH_FLD_BIT == match_fld_element(pt_entry_info, pt_packet_info, (1<<i)))
			{
				CLASSIFY_CRITIC_INFO("not match bit = %d.\n", i);
				return E_NOT_MATCH_FLD_ENTRY;
			}
		}
	}

	return E_MATCH_FLD_ENTRY;
}

static int traffic_classify_fld_acl_action_handle(struct sk_buff *skb, ecn_traffic_classify_info_t *packet_info, struct list_head *head)
{
	int                            res = E_NOT_MATCH_FLD_ENTRY;
	unsigned int 				config = 0;
	ecn_traffic_classify_list_t	  *ptr = NULL;
	ecn_traffic_classify_list_t *entry = NULL;
	unsigned int 	flag = 0;
	
	
	rcu_read_lock();
	/*get port action bit map acl mode*/
	list_for_each_entry_rcu(entry, head, list) 
	{
		if((entry->entry.port == packet_info->port) && (entry->entry.act_element.action_bit_map & E_ACL_ACTION))
		{
			config = 1;
			break;
		}
	}
	
	if(1 == config)
	{
		flag = (1 << entry->entry.act_element.acl_mode);

		/*match fld entry*/
		entry = NULL;
		ptr   = NULL;
		list_for_each_entry_rcu(entry, head, list) 
		{
			res = match_fld_entry(&entry->entry.fld_element, &packet_info->fld_element);
			if(E_MATCH_FLD_ENTRY == res)
			{
				break;
			}
		}
		flag |= (1 << res);
	}
	else
	{
		flag = ((1 << E_FORWARD) | (1 << E_MATCH_FLD_ENTRY));
	}
	rcu_read_unlock();
	
	CLASSIFY_DEBUG_INFO("config = %d, res = %d, flag = %d.\n", config, res, flag);
	switch (flag)
	{	
		/*flag = 9*/
		case ((1 << E_FORWARD) | (1 << E_MATCH_FLD_ENTRY)):
		{
			res = E_FORWARD;
			break;
		}
		/*flag = 17*/
		case ((1 << E_FORWARD) | (1 << E_NOT_MATCH_FLD_ENTRY)):
		{
			res = E_DROP;
			break;
		}
		/*flag = 12*/
		case ((1 << E_DROP) | (1 << E_MATCH_FLD_ENTRY)):
		{
			res = E_DROP;
			break;
		}
		/*flag = 18*/
		case ((1 << E_DROP) | (1 << E_NOT_MATCH_FLD_ENTRY)):
		{
			res = E_FORWARD;
			break;
		}
		default:
		{
			res = E_FORWARD;
			break;
		}
	}
	
	
	return res;
}


/* traffic_classify_element_entry*/
static int traffic_classify_fld_action_handle(struct sk_buff *skb, ecn_traffic_classify_info_t *packet_info, struct list_head *head)
{
	int                            res = 0;
	unsigned char 			  fld_flag = false;
	ecn_traffic_classify_list_t	  *ptr = NULL;
	ecn_traffic_classify_list_t *entry = NULL;
	ecn_classify_action_info_t   action_info;
	
	if(NULL == packet_info || NULL == skb || NULL == head)
	{
		return E_FORWARD;
	}

	memset(&action_info, 0, sizeof(action_info));
	rcu_read_lock();
	/*match fld entry*/
	list_for_each_entry_rcu(entry, head, list) 
	{
		res = match_fld_entry(&entry->entry.fld_element, &packet_info->fld_element);
		if(E_MATCH_FLD_ENTRY == res)
		{
			fld_flag = true;
			action_info.action_bit_map = entry->entry.act_element.action_bit_map;
			action_info.remark_pbit    = entry->entry.act_element.remark_pbit;
			action_info.remark_queue   = entry->entry.act_element.remark_queue;
			action_info.limit_group_id = entry->entry.act_element.limit_group_id;
			action_info.remark_tos     = entry->entry.act_element.remark_tos;
			action_info.acl_mode	   = entry->entry.act_element.acl_mode;
			break;
		}
	}
	rcu_read_unlock();

	CLASSIFY_CRITIC_INFO("fld_flag = %d.\n", fld_flag);
	/*action deal*/
	if(true == fld_flag)
	{
		traffic_classify_action(skb, &action_info);
	}
		
	return 0;
}


static void  dump_skb(struct sk_buff *skb, int line)
{	
	unsigned char *p = skb->data;	
	int i=0;
	unsigned char prio = 0;
	unsigned char queue = 0;
	unsigned int  group = 0;
	struct ethhdr *eth_data = NULL;

	if(0 == (g_CLASSIFY_DEBUG_LEVEL & E_DEBUG_INFO_LEVLE))
	{
		return ;
	}
	
	eth_data =  eth_hdr(skb);
	
	printk("**********line = %d start**********.\n", line);
	
	queue = (skb->mark >> QUEUE_REMARK_OFFSET) & QUQUE_REMARK_MASK;
	group = (skb->mark >> LIMIT_GROUP_REMARK_OFFSET) & LIMIT_GROUP_REMARK_MASK;	
	printk("skb->mark = 0x%04X. queue = %d, group = %d.\n", skb->mark, queue, group);
	
	printk("dst_mac = %pM, src_mac = %pM, proto = 0x%04X.\n", eth_data->h_dest, eth_data->h_source, eth_data->h_proto);
	for(i=0; i< skb->len; i++)  
	{		
		printk("%02X ", *(p+i));		
		if(0 == ((i+1)% 8))		
		{
			printk("\t");		
		}		
		if(0 == ((i+1)% 16))		
		{			
			printk("\n");		
		}  
	}	
	printk("\n\n");	
	
	printk("**********line = %d end**********.\n", line);
	return;
}

static int print_config_para(ecn_traffic_classify_info_t *entry)
{
	if(NULL == entry)
	{
		return  -EINVAL;
	}
	
    printk("entry->port = %d.\n", 								entry->port);
	printk("entry->fld_element.fld_bit_map = 0x%04X.\n",   		entry->fld_element.fld_bit_map);
	printk("entry->fld_element.dst_mac = %pM.\n",   			entry->fld_element.dst_mac);
	printk("entry->fld_element.src_mac = %pM.\n",   			entry->fld_element.src_mac);
	printk("entry->fld_element.ether_type  = 0x%04X.\n", 		entry->fld_element.ether_type);
	printk("entry->fld_element.vid  = %d.\n", 					entry->fld_element.vid);
	printk("entry->fld_element.pbit = %d.\n", 					entry->fld_element.pbit);
	printk("entry->fld_element.ipv4_src_ip.s_addr = %pI4.\n", 	&entry->fld_element.ipv4_src_ip.s_addr);
	printk("entry->fld_element.ipv4_dst_ip.s_addr = %pI4.\n", 	&entry->fld_element.ipv4_dst_ip.s_addr);
	printk("entry->fld_element.tos = %d.\n", 		   			entry->fld_element.tos);
	printk("entry->fld_element.l4_ipv4_src_port = %d.\n", 		entry->fld_element.l4_ipv4_src_port);
	printk("entry->fld_element.l4_ipv4_dst_port = %d.\n", 		entry->fld_element.l4_ipv4_dst_port);
	printk("entry->fld_element.ipv4_ttl = %d.\n", 				entry->fld_element.ipv4_ttl);

	printk("entry->fld_element.ipv6_dscp  = %d.\n",   			entry->fld_element.ipv6_dscp);
	printk("entry->fld_element.ip_version = %d.\n",   		    entry->fld_element.ip_version);
	printk("entry->fld_element.flow_lbl   = 0x%02x%02x%02x.\n",entry->fld_element.flow_lbl[0], entry->fld_element.flow_lbl[1], entry->fld_element.flow_lbl[2]);
	printk("entry->fld_element.ipv6_saddr = %pI6.\n", 		    entry->fld_element.ipv6_saddr.s6_addr);
	printk("entry->fld_element.ipv6_daddr = %pI6.\n", 			entry->fld_element.ipv6_daddr.s6_addr);
	printk("entry->fld_element.nexthdr    = %d.\n", 			entry->fld_element.nexthdr);
	printk("entry->fld_element.ipv6_traffic_class = %d.\n", 	entry->fld_element.ipv6_traffic_class);
	printk("entry->fld_element.hop_limit = %d.\n", 	        entry->fld_element.hop_limit);
	printk("entry->fld_element.l4_ipv6_src_port = %d.\n", 		entry->fld_element.l4_ipv6_src_port);
	printk("entry->fld_element.l4_ipv6_dst_port = %d.\n", 		entry->fld_element.l4_ipv6_dst_port);
	printk("entry->fld_element.ipv6_saddr_hdr = %pI6.\n", 		entry->fld_element.ipv6_saddr_hdr.s6_addr);
	printk("entry->fld_element.ipv6_daddr_hdr = %pI6.\n", 		entry->fld_element.ipv6_daddr_hdr.s6_addr);
	printk("entry->fld_element.dhcpv6_msg_type = %d.\n",		entry->fld_element.dhcpv6_msg_type);
	printk("entry->fld_element.ip_type = %d.\n",				entry->fld_element.ip_type);

	printk("entry->act_element.action_bit_map = %d.\n",	    entry->act_element.action_bit_map);
	printk("entry->act_element.remark_pbit = %d.\n",	    	entry->act_element.remark_pbit);
	printk("entry->act_element.remark_queue = %d.\n",			entry->act_element.remark_queue);
	printk("entry->act_element.limit_group_id = %d.\n",		entry->act_element.limit_group_id);
	printk("entry->act_element.remark_tos = %d.\n",			entry->act_element.remark_tos);
	printk("entry->act_element.acl_mode = %d.\n",				entry->act_element.acl_mode);

	return 0;
}

static int print_parse_packet_para(ecn_traffic_classify_info_t *entry, int line)
{
	if(NULL == entry)
	{
		return  -EINVAL;
	}
	if(0 == (g_CLASSIFY_DEBUG_LEVEL & E_DEBUG_INFO_LEVLE))
	{
		return  -EINVAL;
	}
	
    CLASSIFY_DEBUG_INFO("*****************line = %d start****************.\n", line);
    CLASSIFY_DEBUG_INFO("entry->port = %d.\n", 								entry->port);
	CLASSIFY_DEBUG_INFO("entry->fld_element.fld_bit_map = 0x%04X.\n",   	entry->fld_element.fld_bit_map);
	CLASSIFY_DEBUG_INFO("entry->fld_element.dst_mac = %pM.\n",   			entry->fld_element.dst_mac);
	CLASSIFY_DEBUG_INFO("entry->fld_element.src_mac = %pM.\n",   			entry->fld_element.src_mac);
	CLASSIFY_DEBUG_INFO("entry->fld_element.ether_type  = 0x%04X.\n", 		entry->fld_element.ether_type);
	CLASSIFY_DEBUG_INFO("entry->fld_element.vid  = %d.\n", 					entry->fld_element.vid);
	CLASSIFY_DEBUG_INFO("entry->fld_element.pbit = %d.\n", 					entry->fld_element.pbit);
	CLASSIFY_DEBUG_INFO("entry->fld_element.ipv4_src_ip.s_addr = %pI4.\n", 	&entry->fld_element.ipv4_src_ip.s_addr);
	CLASSIFY_DEBUG_INFO("entry->fld_element.ipv4_dst_ip.s_addr = %pI4.\n", 	&entry->fld_element.ipv4_dst_ip.s_addr);
	CLASSIFY_DEBUG_INFO("entry->fld_element.tos = %d.\n", 		   			entry->fld_element.tos);
	CLASSIFY_DEBUG_INFO("entry->fld_element.l4_ipv4_src_port = %d.\n", 		entry->fld_element.l4_ipv4_src_port);
	CLASSIFY_DEBUG_INFO("entry->fld_element.l4_ipv4_dst_port = %d.\n", 		entry->fld_element.l4_ipv4_dst_port);
	CLASSIFY_DEBUG_INFO("entry->fld_element.ipv4_ttl = %d.\n", 				entry->fld_element.ipv4_ttl);

	CLASSIFY_DEBUG_INFO("entry->fld_element.ipv6_dscp  = %d.\n",   			entry->fld_element.ipv6_dscp);
	CLASSIFY_DEBUG_INFO("entry->fld_element.ip_version = %d.\n",   		    entry->fld_element.ip_version);
	CLASSIFY_DEBUG_INFO("entry->fld_element.flow_lbl   = 0x%02x%02x%02x.\n",entry->fld_element.flow_lbl[0], entry->fld_element.flow_lbl[1], entry->fld_element.flow_lbl[2]);
	CLASSIFY_DEBUG_INFO("entry->fld_element.ipv6_saddr = %pI6.\n", 		    entry->fld_element.ipv6_saddr.s6_addr);
	CLASSIFY_DEBUG_INFO("entry->fld_element.ipv6_daddr = %pI6.\n", 			entry->fld_element.ipv6_daddr.s6_addr);
	CLASSIFY_DEBUG_INFO("entry->fld_element.nexthdr    = %d.\n", 			entry->fld_element.nexthdr);
	CLASSIFY_DEBUG_INFO("entry->fld_element.ipv6_traffic_class = %d.\n", 	entry->fld_element.ipv6_traffic_class);
	CLASSIFY_DEBUG_INFO("entry->fld_element.hop_limit = %d.\n", 	        entry->fld_element.hop_limit);
	CLASSIFY_DEBUG_INFO("entry->fld_element.l4_ipv6_src_port = %d.\n", 		entry->fld_element.l4_ipv6_src_port);
	CLASSIFY_DEBUG_INFO("entry->fld_element.l4_ipv6_dst_port = %d.\n", 		entry->fld_element.l4_ipv6_dst_port);
	CLASSIFY_DEBUG_INFO("entry->fld_element.ipv6_saddr_hdr = %pI6.\n", 		entry->fld_element.ipv6_saddr_hdr.s6_addr);
	CLASSIFY_DEBUG_INFO("entry->fld_element.ipv6_daddr_hdr = %pI6.\n", 		entry->fld_element.ipv6_daddr_hdr.s6_addr);
	CLASSIFY_DEBUG_INFO("entry->fld_element.dhcpv6_msg_type = %d.\n",		entry->fld_element.dhcpv6_msg_type);
	CLASSIFY_DEBUG_INFO("entry->fld_element.ip_type = %d.\n",				entry->fld_element.ip_type);

    CLASSIFY_DEBUG_INFO("*****************line = %d end****************.\n", line);
	
	return 0;
}
int ecnt_lm_api_set_recv_callback(linemark_ctrl_packet_rx rx_cb)
{
	CLASSIFY_DEBUG_INFO("enter %s function = %p ====>........\n", __FUNCTION__, rx_cb);
	if(rx_cb)
	{
		glm_ctrl_packet_rx = (linemark_ctrl_packet_rx)rx_cb;
	}

	return 0;
}
EXPORT_SYMBOL(ecnt_lm_api_set_recv_callback);
int ecnt_lm_api_xmit_callback(void *data, unsigned int len, unsigned char port)
{
	CLASSIFY_DEBUG_INFO("enter %s====>........\n", __FUNCTION__);
	if(false != (g_classify_fun_bit_map & E_TOS_REMARK_ACTION))
	{
		char *dev_name = "pon";
		struct sk_buff *skb = NULL;
		struct net_device *dev = NULL;
		unsigned int eth_type = 0;
		unsigned char* buff   = NULL;
		unsigned short *vlan_tci = NULL;

		if(NULL == data)
		{
			printk("%s %d para errror!\n", __FUNCTION__, __LINE__);
			return -1;
		}
		CLASSIFY_DEBUG_INFO("recv data_len = %d line = %d\n", len, __LINE__);
		/* alloc skb */
		#if defined(TCSUPPORT_MAX_PACKET_2000)
		skb = skbmgr_alloc_skb4k();
		#else
		skb = skbmgr_alloc_skb2k();
		#endif
		if (unlikely(!skb))
		{
			return -1;
		}
		
		/* get dev */
		if (dev_name)
		{
			dev = dev_get_by_name(&init_net,dev_name);
		}
		//printk("%s name = %s\n", __FUNCTION__, dev->name)
		//data_dump(data, len);
		if (dev)
		{	
			/* init skb */
			skb->dev = dev;
			memmove(skb->data,(unsigned char *)data,len);
			skb_put(skb,len);
			skb->mark &= ~(PORT_MASK<<PORT_MASK_OFFSET);
			skb->mark |= (port<<PORT_MASK_OFFSET);

			/* xmit skb */
			#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29)
			skb->dev->netdev_ops->ndo_start_xmit(skb,skb->dev);
			#else
			skb->dev->hard_start_xmit(skb,skb->dev);
			#endif
			dev_put(dev);
		}
		else
		{
			dev_kfree_skb_any(skb);
		}
	}
	return 0;
}
EXPORT_SYMBOL(ecnt_lm_api_xmit_callback);
ecnt_ret_val ecnt_line_mark_hook(struct ecnt_data *in_data)
{
	if(false != (g_classify_fun_bit_map & E_TOS_REMARK_ACTION))
	{
		struct net_data_s *rx_info = NULL;
		struct sk_buff *skb = NULL;
		int res = 0;
	
		ecn_traffic_classify_info_t packet_info = {0};

		rx_info = (struct net_data_s *)in_data;
		if(NULL == rx_info || NULL == rx_info->pskb || 
		   NULL == *(rx_info->pskb))
		{
			return ECNT_HOOK_ERROR;
		}
		skb = (*(rx_info->pskb));
		if(((skb->mark >> SKB_FROM_LAN_OFFSET) & SKB_FROM_LAN_MASK) && (0 == strcasecmp(skb->dev->name, BRIDGE_WAN_NAME)))
		{
			skb_reset_mac_header(skb);
			classify_flow_hook(skb, &packet_info);
			res = line_mark_action(skb, &packet_info);
		}
		if(res)
		return ECNT_RETURN_DROP;
	}
	return ECNT_CONTINUE;

}

ecnt_ret_val traffic_classify_hook(struct ecnt_data *in_data)
{	
	int 				 res = 0;
	unsigned int 		port = 0;
	unsigned long      flags = 0;
	unsigned char is_forward = E_ECNT_FORWARD;
	struct sk_buff      *skb = NULL;
	ecn_traffic_classify_info_t packet_info = {0};
	traffic_classify_data_t  *net_data = NULL; 
	net_data = (traffic_classify_data_t  *) in_data;
	
	if(false == g_classify_fun_bit_map)
	{
		CLASSIFY_NOTICE_INFO("g_classify_fun_bit_map = %d.\n", g_classify_fun_bit_map);
		is_forward = E_ECNT_FORWARD;
		goto exit;	
	}
	if(NULL == net_data->skb)
	{
		is_forward = E_ECNT_FORWARD;
		goto exit;	
	}
	
	skb = net_data->skb;
	memset(&packet_info, 0, sizeof(packet_info));
	/*parse packet frow port*/
	port = ((skb->mark >> PORT_MASK_OFFSET) & PORT_MASK);
	if( ((E_ETH_PORT_0 <= port) && (E_ETH_PORT_3 >= port)) || (E_WAN_PORT == port) )
	{
		CLASSIFY_NOTICE_INFO("port is include in port list.\n");
	}
	else
	{
		CLASSIFY_NOTICE_INFO("port is exclude in port list.\n");
		is_forward = E_ECNT_FORWARD;
		goto exit;	
	}
	
	CLASSIFY_CRITIC_INFO("port = %d.\n", port);
	/*parse packet*/
	classify_flow_hook(skb, &packet_info);
	print_parse_packet_para(&packet_info, __LINE__);
	dump_skb(skb, __LINE__);

	//spin_lock_irqsave(&classify_traffic_lock, flags);
	/*check whether  there are any elements in ACL/CLASSIFY list */
	if((list_empty(&acl_list[port])) && (list_empty(&classify_list[port])))
	{
		CLASSIFY_NOTICE_INFO("acl && classify list is NULL.\n");
		is_forward = E_ECNT_FORWARD;
		goto exit;	
	}
	
	/*check whether  there are any elements in ACL list */
	if (false != (g_classify_fun_bit_map & E_ACL_ACTION))
	{
		if(!list_empty(&acl_list[packet_info.port]))
		{
			res = traffic_classify_fld_acl_action_handle(skb, &packet_info ,&acl_list[packet_info.port]);
			if(E_DROP == res)
			{
				CLASSIFY_CRITIC_INFO("packet should drop.\n");
				is_forward = E_ECNT_DROP;
				goto exit;	
			}
		}
	}
	CLASSIFY_CRITIC_INFO("packet forward.\n");
	
	/*check whether  there are any elements in classify_list list */
	if(!list_empty(&classify_list[packet_info.port]))
	{
		traffic_classify_fld_action_handle(skb, &packet_info ,&classify_list[packet_info.port]);
	}
	
	dump_skb(skb, __LINE__);
exit:
	//spin_unlock_irqrestore(&classify_traffic_lock, flags);
	*(net_data->forward) = is_forward;

	return res;
}

static int add_Entry(ecn_traffic_classify_info_t *info, int type)
{
	ecn_traffic_classify_list_t* entry = NULL;
    ecn_traffic_classify_list_t*   cur = NULL;
	ecn_traffic_classify_list_t*   nxt = NULL;
	int                       insert_flag = E_NOT_MATCH_FLD_ENTRY;
	int                               res = 0;
	int                          chg_flag = 0;
	unsigned long                       flags;

	//spin_lock_irqsave(&classify_traffic_lock, flags);
	rcu_read_lock();
	if(type == E_ACL_ACTION) {
		list_for_each_entry_rcu(cur, &acl_list[info->port], list) {
			if(info->act_element.acl_mode != cur->entry.act_element.acl_mode) {
				chg_flag = 1;
			}
			break;
		}
	}

	if(chg_flag) {
		spin_lock(&classify_traffic_lock);
		list_for_each_entry_rcu(cur, &acl_list[info->port], list) {
			if(cur->list.prev != LIST_POISON2) {
				list_del_rcu(cur);
				//kfree(cur);
				call_rcu(&cur->head, del_list_mem);
				//acl_rule_num[info->port]--;
			}
		}
		spin_unlock(&classify_traffic_lock);

	}
	
	/*
		exclude same rule
	*/
	if(type == E_ACL_ACTION) {
		list_for_each_entry_rcu(cur, &acl_list[info->port], list) {
			if(info->fld_element.fld_bit_map == cur->entry.fld_element.fld_bit_map) {
				insert_flag = match_fld_entry(&info->fld_element, &cur->entry.fld_element);
				if(insert_flag == E_MATCH_FLD_ENTRY) {//same rule
					break;          
				}
			}
		}
	}else if (0 != (type & CLASSIFY_QOS_ACTION_MASK)) {
		list_for_each_entry_rcu(cur, &classify_list[info->port], list) {
			if(info->fld_element.fld_bit_map == cur->entry.fld_element.fld_bit_map) {
				insert_flag = match_fld_entry(&info->fld_element, &cur->entry.fld_element);
				if(insert_flag == E_MATCH_FLD_ENTRY) {//same rule
					break;          
				}
			}
		}
	}

	/*
		limit rule num 
	*/

	/*
		add rule
	*/

	if(insert_flag == E_NOT_MATCH_FLD_ENTRY) {
		entry = kzalloc(sizeof(ecn_traffic_classify_list_t), GFP_ATOMIC);
		if(NULL == entry)
		{
			res = -EINVAL;
			goto add_entry_exit;
		}
		
		/*
			not contain pointer
		*/
		memcpy(&entry->entry,  info,  sizeof(*info));
		/*
		entry->entry.fld_element.fld_bit_map               = info->fld_element.fld_bit_map;
		entry->entry.fld_element.ipv4_src_ip.s_addr = info->fld_element.ipv4_src_ip.s_addr;
		entry->entry.fld_element.ipv4_dst_ip.s_addr = info->fld_element.ipv4_dst_ip.s_addr;
		entry->entry.fld_element.vid                               = info->fld_element.vid;
		entry->entry.fld_element.ether_type                 = info->fld_element.ether_type;
		entry->entry.fld_element.pbit                             = info->fld_element.pbit;
		entry->entry.fld_element.tos                               = info->fld_element.tos;
		entry->entry.fld_element.l4_ipv4_src_port     = info->fld_element.l4_ipv4_src_port;
		entry->entry.fld_element.l4_ipv4_dst_port     = info->fld_element.l4_ipv4_dst_port;
		entry->entry.fld_element.ipv4_ttl                     = info->fld_element.ipv4_ttl;
		entry->entry.fld_element.ipv6_dscp                   = info->fld_element.ipv6_dscp;
		entry->entry.fld_element.ip_version                 = info->fld_element.ip_version;
		entry->entry.fld_element.nexthdr                       = info->fld_element.nexthdr;
		entry->entry.fld_element.ipv6_traffic_class = info->fld_element.ipv6_traffic_class;
		entry->entry.fld_element.hop_limit                   = info->fld_element.hop_limit;
		entry->entry.fld_element.l4_ipv6_dst_port     = info->fld_element.l4_ipv6_dst_port;
		entry->entry.fld_element.l4_ipv6_src_port     = info->fld_element.l4_ipv6_src_port;
		entry->entry.fld_element.dhcpv6_msg_type       = info->fld_element.dhcpv6_msg_type;
		entry->entry.fld_element.ip_type                       = info->fld_element.ip_type;
		entry->entry.act_element                                       = info->act_element;
		entry->entry.port                                                     = info->port;
		memcpy(entry->entry.fld_element.dst_mac, info->fld_element.dst_mac, sizeof(info->fld_element.dst_mac));
		memcpy(entry->entry.fld_element.src_mac, info->fld_element.src_mac, sizeof(info->fld_element.src_mac));
		memcpy(entry->entry.fld_element.flow_lbl, info->fld_element.flow_lbl, sizeof(info->fld_element.flow_lbl));
		memcpy(entry->entry.fld_element.ipv6_daddr.s6_addr, info->fld_element.ipv6_daddr.s6_addr, 
			   sizeof(info->fld_element.ipv6_daddr.s6_addr));
	       memcpy(entry->entry.fld_element.ipv6_saddr.s6_addr, info->fld_element.ipv6_saddr.s6_addr,
	           sizeof(info->fld_element.ipv6_saddr.s6_addr));
		memcpy(entry->entry.fld_element.ipv6_daddr_hdr.s6_addr, info->fld_element.ipv6_daddr_hdr.s6_addr,
			   sizeof(info->fld_element.ipv6_daddr_hdr.s6_addr));
		memcpy(entry->entry.fld_element.ipv6_saddr_hdr.s6_addr, info->fld_element.ipv6_saddr_hdr.s6_addr,
			   sizeof(info->fld_element.ipv6_saddr_hdr.s6_addr));
		*/
		spin_lock(&classify_traffic_lock);
		if(entry->list.prev == NULL) {
			if(type == E_ACL_ACTION) {
				list_add_rcu(&entry->list, &acl_list[info->port]);
				//acl_rule_num[info->port]++;
			}else if (0 != (type & CLASSIFY_QOS_ACTION_MASK)) {
				list_add_rcu(&entry->list, &classify_list[info->port]);
				//classify_rule_num[info->port]++;
			}
		}
		spin_unlock(&classify_traffic_lock);
		res = 0;
	}else {
		res = -1;
	}

add_entry_exit:	
	//spin_unlock_irqrestore(&classify_traffic_lock, flags);
	rcu_read_unlock();
	return res;
}

static int del_Entry(ecn_traffic_classify_info_t *info, int type) 
{
	ecn_traffic_classify_list_t*   cur = NULL;
	ecn_traffic_classify_list_t*   nxt = NULL;
	unsigned long                       flags;
	int                          del_flag = E_NOT_MATCH_FLD_ENTRY;
	int                               res = 0;

	//spin_lock_irqsave(&classify_traffic_lock, flags);
	rcu_read_lock();
	if(type == E_ACL_ACTION) {
		list_for_each_entry_rcu(cur, &acl_list[info->port], list) {
			if(info->fld_element.fld_bit_map == cur->entry.fld_element.fld_bit_map) {
				del_flag = match_fld_entry(&info->fld_element, &cur->entry.fld_element);
				if(del_flag == E_MATCH_FLD_ENTRY) {//same rule
					break;          
				}
			}
		}
	}else if (0 != (type & CLASSIFY_QOS_ACTION_MASK)) {
		list_for_each_entry_rcu(cur, &classify_list[info->port], list) {
			if(info->fld_element.fld_bit_map == cur->entry.fld_element.fld_bit_map) {
				del_flag = match_fld_entry(&info->fld_element, &cur->entry.fld_element);
				if(del_flag == E_MATCH_FLD_ENTRY) {//same rule
					break;          
				}
			}
		}
	}
	
	if(del_flag == E_MATCH_FLD_ENTRY) { //match
	    spin_lock(&classify_traffic_lock);
		if(cur->list.prev != LIST_POISON2) {
			list_del_rcu(&cur->list);
			if(type == E_ACL_ACTION) {
				//acl_rule_num[cur->entry.port]--;
			}else if (0 != (type & CLASSIFY_QOS_ACTION_MASK)) {
				//classify_rule_num[cur->entry.port]--;
			}
			//kfree(cur);
			call_rcu(&cur->head, del_list_mem);
		}
		spin_unlock(&classify_traffic_lock);
		res = 0;
	}else {
		res = -1;
	}

	//spin_unlock_irqrestore(&classify_traffic_lock, flags);
	rcu_read_unlock();
	return res;
	
}



static int op_list_entry(ecn_traffic_classify_info_t *entry , int op, int type)
{
	int res = 0;
	
	if((NULL == entry) || (E_ETH_PORT_0 > entry->port) || (E_MAX_PORT <= entry->port) )
	{
		return -EINVAL;
	}

	switch (op)
	{
		case TRAFFIC_CLASSIFY_ADD_ENTRY:
		{
			res = add_Entry(entry, type);
			break;
		}
		case TRAFFIC_CLASSIFY_DEL_ENTRY:
		{
			res = del_Entry(entry, type);
			break;
		}
		case TRAFFIC_CLASSIFY_CLEAN:
		{
			res = clean_Entry(entry, type);
			break;
		}
		default:
		{
			res = -EINVAL;
			break;
		}
	}

	return res;
}


static int classify_entry_op(ecn_traffic_classify_info_t *entry , int op)
{
	int res = 0;
	
	if(NULL == entry)
	{
		return -EINVAL;
	}
	
	switch (entry->act_element.action_bit_map & CLASSIFY_ACTION_MASK)
	{
		case E_ACL_ACTION:
		{
           	res = op_list_entry(entry , op, E_ACL_ACTION);
			break;
		}
		case (E_QUEUE_MAPPING_ACTION)...(E_PRIORITY_REMARK_ACTION | E_QUEUE_MAPPING_ACTION | E_LIMIT_ACTION | E_TOS_REMARK_ACTION):
		{
			res = op_list_entry(entry , op, (entry->act_element.action_bit_map & CLASSIFY_QOS_ACTION_MASK));
			break;
		}
		default:
		{
			res = -EINVAL;
			break ;
		}
	}
	
	return res;
}

static int classify_show_list(e_rule_type rule_type) 
{
	int                              i = 0;
	int                            res = 0;
	unsigned long                    flags;
	ecn_traffic_classify_list_t *cur, *nxt;
	

	//spin_lock_irqsave(&classify_traffic_lock,flags);
	rcu_read_lock();
	for(i=0; i<E_MAX_PORT; i++)
	{
		if(rule_type == ACL_RULE_LIST) 
		{
		    printk("*****************ACL RULE*****************\n");
			list_for_each_entry_rcu(cur, &acl_list[i], list) {
				print_config_para(&cur->entry);
				printk("\n");
			}
	
		}
		else 
		{
			printk("*****************QOS RULE*****************\n");
			list_for_each_entry_rcu(cur, &classify_list[i], list) {
				print_config_para(&cur->entry);
				printk("\n");
			}
		}
		
	}
	//spin_unlock_irqrestore(&classify_traffic_lock,flags);
	rcu_read_unlock();
	return res;
	
}


static int classify_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static long classify_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = -EINVAL;
	void __user *argp = (void __user *)arg;
	ecn_traffic_classify_info_t info;
	memset(&info, 0, sizeof(info));
	
	if(_IOC_WRITE == _IOC_DIR(cmd))
	{
		if (copy_from_user(&info, argp, sizeof(info)))
		{
			return -EFAULT;
		}
		switch(cmd)
		{
			case TRAFFIC_CLASSIFY_ADD_ENTRY:
			{
				ret = classify_entry_op(&info, TRAFFIC_CLASSIFY_ADD_ENTRY);
				break;
			}
			case TRAFFIC_CLASSIFY_DEL_ENTRY:
			{
            	ret = classify_entry_op(&info, TRAFFIC_CLASSIFY_DEL_ENTRY);
				break;
			}
			case TRAFFIC_CLASSIFY_CLEAN:
			{
            	ret = classify_entry_op(&info, TRAFFIC_CLASSIFY_CLEAN);
				break;
			}
			default:
			{
				break;
			}
		}
	}
	else if(_IOC_READ == _IOC_DIR(cmd))
	{
		
	}
	else
	{
		
	}
	
	return ret;
}

const struct file_operations classify_fops = {
	.owner		    = THIS_MODULE,
	.open		    = classify_open,
	.unlocked_ioctl = classify_ioctl
};

static int init_list_head(int list_head_num)
{
	int i = 0;
	
	for(i = 0; i < list_head_num; i++)
	{
		INIT_LIST_HEAD(&classify_list[i]);
		INIT_LIST_HEAD(&acl_list[i]);
	}
	
	return 0;
}

static int traffic_classify_proc_write(struct file *file, const char *buffer,
			     unsigned long count, void *data)
{
	char buf[64] = {0};
	char cmd[32] = {0};
	char subcmd[32] = {0};
	unsigned int val = 0;

	if (count > sizeof(buf) - 1)
	{
		return -EINVAL ;
	}
	if (copy_from_user(buf, buffer, count))
	{
		return -EFAULT;
	}
	
	sscanf(buf, "%s %s %d", cmd, subcmd, &val) ;
	if(0 == strcmp(cmd, "set"))
	{
		if(0 == strcmp(subcmd, "debug_level"))
		{
			if(0 == val)
			{
				g_CLASSIFY_DEBUG_LEVEL = 0;
			}
			else
			{
				g_CLASSIFY_DEBUG_LEVEL |= (val & (E_ERROR_INFO_LEVLE | E_CRITIC_INFO_LEVLE | E_NOTICE_INFO_LEVLE | E_WARN_INFO_LEVLE | E_DEBUG_INFO_LEVLE | E_TRACE_INFO_LEVLE));
			}
		}
		else if(0 == strcmp(subcmd, "ctrl_bit_map"))
		{
			if(0 == val)
			{
				g_classify_fun_bit_map = 0;
			}
			else
			{
				g_classify_fun_bit_map |= (val & (E_ACL_ACTION | E_QUEUE_MAPPING_ACTION | E_PRIORITY_REMARK_ACTION | E_LIMIT_ACTION | E_TOS_REMARK_ACTION));
			}
		}
	}
	else if(0 == strcmp(cmd, "get"))
	{
		if(0 == strcmp(subcmd, "debug_level"))
		{
			printk("g_CLASSIFY_DEBUG_LEVEL  = 0x%04X.\n", g_CLASSIFY_DEBUG_LEVEL);
		}
		else if(0 == strcmp(subcmd, "ctrl_bit_map"))
		{
			printk("classify_fun_bit_map  = 0x%04X.\n", g_classify_fun_bit_map);
		}
	}
	else if(0 == strcmp(cmd, "show"))
	{
		if(0 == strcmp(subcmd, "acl_list"))
		{
			classify_show_list(ACL_RULE_LIST);
		}
		else if(0 == strcmp(subcmd, "classify_list"))
		{
			classify_show_list(QOS_RUEL_LIST);
		}
	}

	return count;
}


static int __init traffic_classify_init(void)
{
	int err = 0;
	struct proc_dir_entry     *dir = NULL;
	struct proc_dir_entry     *traffic_classify_proc = NULL;
	
	init_list_head(E_MAX_PORT);
	spin_lock_init(&classify_traffic_lock);

	err = register_chrdev(TRAFFIC_CLASSIFY_MAJOR, CLASSIFY_MODULE_NAME, &classify_fops);
	if (err < 0) 
	{
		printk("TRAFFIC_CLASSIFY_MAJOR: could not get major number.\n");
		return err;
	}
	
	dir = proc_mkdir(TRAFFIC_CLASSIFY_PROC_DIR, NULL);
	if(NULL == dir) 
	{
		printk("create proc %s directory error!\n", TRAFFIC_CLASSIFY_PROC_DIR);
		goto error_create_dir;
	}
	
	traffic_classify_proc = create_proc_entry(TRAFFIC_CLASSIFY_PROC_DEBUG_FILE, 0, dir);
	if(NULL == traffic_classify_proc) 
	{
		printk(KERN_ERR "create proc %s/%s file!\n", TRAFFIC_CLASSIFY_PROC_DIR, TRAFFIC_CLASSIFY_PROC_DEBUG_FILE);
		goto error_create_file;
	}

	traffic_classify_proc->write_proc = traffic_classify_proc_write;
	
	ecnt_register_hook(&ecnt_flow_classify_op);
	ecnt_register_hook(&ecnt_line_mark_ops);
	printk("init traffic_classify.\n");
	return 0;

error_create_file:
	remove_proc_entry(TRAFFIC_CLASSIFY_PROC_DIR, NULL);
error_create_dir:
	
	unregister_chrdev(TRAFFIC_CLASSIFY_MAJOR, CLASSIFY_MODULE_NAME);

	return -ENOENT;
}

static int clean_Entry(ecn_traffic_classify_info_t *info, int type) 
{
	int                                 i = 0;
	int 							 port = 0 ;
	ecn_traffic_classify_list_t*   cur = NULL;
	ecn_traffic_classify_list_t*   nxt = NULL;
	unsigned long                       flags;
	
	port = info->port;
	//spin_lock_irqsave(&classify_traffic_lock, flags);
	rcu_read_lock();
	spin_lock(&classify_traffic_lock);
	if(E_ACL_ACTION == (type & E_ACL_ACTION))
	{
		list_for_each_entry_rcu(cur, &acl_list[port], list) 
		{
	 		  if(cur->list.prev != LIST_POISON2) {
		 			list_del_rcu(cur);
		 			//acl_rule_num[port]--;
		 			//kfree(cur);
		 			call_rcu(&cur->head, del_list_mem);
	 		  }
		}
	}
	
	if(0 != (type & CLASSIFY_QOS_ACTION_MASK))
	{
		list_for_each_entry_rcu(cur, &classify_list[port], list) 
		{
		    if(cur->list.prev != LIST_POISON2) {
				list_del_rcu(cur);
				//classify_rule_num[port]--;
				//kfree(cur);
				call_rcu(&cur->head, del_list_mem);
		    }
		}
	}
	spin_unlock(&classify_traffic_lock);
	//spin_unlock_irqrestore(&classify_traffic_lock, flags);
	rcu_read_unlock();

	return 0;
}

static void remove_traffic_classify_list(void) 
{
	ecn_traffic_classify_info_t info;
	int i = 0;
	
	for(i=0; i<E_MAX_PORT; i++)
	{
		memset(&info, 0, sizeof(info));
		info.port = i;
		clean_Entry(&info, CLASSIFY_ACTION_MASK);
	}

	return ;
}

static void __exit traffic_classify_exit(void)
{
	remove_traffic_classify_list();
	remove_proc_entry(TRAFFIC_CLASSIFY_PROC_DEBUG_FILE, TRAFFIC_CLASSIFY_PROC_DIR);
	remove_proc_entry(TRAFFIC_CLASSIFY_PROC_DIR,   NULL);
	ecnt_unregister_hook(&ecnt_flow_classify_op);
	ecnt_unregister_hook(&ecnt_line_mark_ops);
	unregister_chrdev(TRAFFIC_CLASSIFY_MAJOR, CLASSIFY_MODULE_NAME);
	printk("exit traffic_classify.\n");
}

module_init(traffic_classify_init);
module_exit(traffic_classify_exit);

//MODULE_LICENSE("Proprietary");
MODULE_LICENSE("GPL");



