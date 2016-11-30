/*********************************************************************************
 * igmp data flow forward contorl module for FHTWCX CMCC specific feature
 *
 * Copyright (C) 2015 Econet Technologies, Corp.
 * All Rights Reserved.
 *
 *********************************************************************************/
#include <linux/if_ether.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/if_vlan.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/proc_fs.h>
#include <linux/ppp_defs.h>
#include <linux/if_pppox.h>
#include <linux/version.h>
#include <linux/foe_hook.h>

#include "ecnt_igmp.h"
#include <ecnt_hook/ecnt_hook_net.h>
#include <ecnt_hook/ecnt_hook_multicast.h>
#include <ecnt_hook/ecnt_hook_traffic_classify.h> 


/*********************************************************************************
 * data type and global variable definition setction
 *
 *********************************************************************************/
#define SWIC_PORTNUM	7
#define PON_IF_NAME		"pon"

#define ECNT_IGMP_MAJOR		253
#define ECNT_IGMP_DEV_NAME	"ecnt_igmp"

#define MAX_DP_NUM		6
#define DP_QDMA1_SW		0
#define DP_GDMA1		1
#define DP_DROP			7

#define	ECNT_LAN_PORT_MASK	0xf

typedef struct
{
	int port_id;
	char *name;
}port_lan_name_map;


char lan_port_map[SWIC_PORTNUM];

static unsigned long debuglever = 0;

static MC_FwdTable mc_fdb;
static mc_ctrl_packet_rx mc_ctrl_packet_rx_cb_func = NULL;

static int g_igmp_snooping_on_off = E_IGMP_SNOOPING_ON;
static struct list_head mtk_hwnat_igmp;
static struct list_head mtk_hwnat_drop;
static spinlock_t mtk_hwnat_mcst_lock;
static spinlock_t mtk_hwnat_drop_mcst_lock;
static spinlock_t ecnt_fwd_tbl_lock;
static int g_ethport_switchport_map[MAX_PORT_NUM] = {0};

int (*mtk_multicast_data_handle_hook)(struct sk_buff* skb) = NULL;

int mtk_xpon_is_multicast_addr(unsigned char* addr);
int mtk_xpon_hwnat_update_all_flow(void);
static inline int igmp_get_downstream_vlan_action(uint8 port_id,struct sk_buff* skb,
	mc_fwd_tbl_entry *data,int vlanid,mc_fwd_tbl_entry *pEntry);
inline char* get_dev_name_by_port(uint8 port_id);

EXPORT_SYMBOL(lan_port_map);
EXPORT_SYMBOL(mtk_multicast_data_handle_hook);


/*********************************************************************************
 * function implementation setction
 *
 *********************************************************************************/
#define MULTI_LOG(Level,fmt,args...)  		\
		{									\
			if (Level <= debuglever)		\
			{								\
				printk("\r\n[%s]"fmt,__func__,##args);\
			}								\
		}

void skb_dump(struct sk_buff* sk)
{  
  	unsigned int i;	
	//if (MULTI_LOG_DUMP_DATA == debuglever)
	{
		printk("\nskb_dump: from %s with len %d (%d) headroom=%d tailroom=%d\n",
			sk->dev?sk->dev->name:"ip stack",sk->len,sk->truesize,
			skb_headroom(sk),skb_tailroom(sk));
		for (i = (unsigned int)sk->head; i < (unsigned int)sk->tail; i++)
		{
			if (0 == (i % 16))
			{
				printk("\n");
			}
			
	        if (i == (unsigned int)sk->head) printk("@h");	
			if(i==(unsigned int)sk->network_header) printk("@n");
			if(i==(unsigned int)sk->mac_header) printk("@*");
			if(i==(unsigned int)sk->data) printk("@d");
			printk("%02X-",*((unsigned char*)i));
		}
		printk("\n");
	}
}

void mtk_init_hwnat_list(void)
{
	INIT_LIST_HEAD(&mtk_hwnat_igmp);
	INIT_LIST_HEAD(&mtk_hwnat_drop);
}

static void* mtk_xpon_alloc(int size)
{
	void* ptr = NULL;
	
	if (size > 0)
	{
		ptr = kmalloc(size, GFP_ATOMIC);
		if (ptr != NULL)
		{
			memset(ptr,0,size);
		}
	}
	
	return ptr;
} 

static void mtk_xpon_free(void* ptr)
{
	if (ptr)
    {
        kfree(ptr);
        ptr = NULL;
    } 
}

static short int mtk_calc_wifi_count(uint32 mask)
{
	int i;
	short int wifi_num = 0;
	uint32 wifi_mask = ((mask >> HWNAT_WLAN_IF_BASE) & 0x0ff);
	
    for (i = 0; i < HWNAT_WLAN_IF_MAXNUM; i++)
    {
        if (wifi_mask & (1 << i))
        {
            wifi_num++;
        }
    }
	
	return wifi_num;
}

static short int mtk_calc_lan_count(uint32 mask)
{
	int i;
	short int lan_num = 0;
	uint32 lan_mask = (mask & 0xff);
	
    for (i = 0; i < HWNAT_WLAN_IF_BASE; i++)
    {
        if (lan_mask & (1 << i))
        {
            lan_num++;
        }
    }
	
	return lan_num;
}

static unsigned long mtk_calc_port_mask(uint32 mask)
{
	int i = 0;
	uint32 port_mask = 0;
	int remainder = 0;
	int array[MAX_PORT_NUM] = {0};
	unsigned long switch_mask = 0;

	port_mask = (mask & 0xf);
	
	do
	{
	  remainder = (port_mask %2);
	  port_mask = (port_mask /2);
	  array[i] = remainder;
	  i++;
	}while (0 != port_mask);

	for (i = 0; i < MAX_PORT_NUM; i++)
	{
		if ((1 == array[i]) && (-1 != lan_port_map[i]))
		{
			switch_mask |= (1 << lan_port_map[i]);
		}	
	}

	return switch_mask;
}	

static void mtk_xpon_hwnat_delete_entry(MTK_IGMP_HWNATEntry_t* entry)
{
	if (NULL == entry)
	{
		return;
	}

	del_timer(&entry->age_timer);
	list_del(&entry->list);
	mtk_xpon_free(entry);
}


static int mtk_xpon_hwnat_flow_index(struct sk_buff* skb)
{
	int index = -1; 
	
	if (hwnat_skb_to_foe_hook && skb)
	{
		index = hwnat_skb_to_foe_hook(skb);
	}

	return index;
}

static int mtk_xpon_get_vlan_id(struct sk_buff* skb)
{
	unsigned int eth_type = eth_hdr(skb)->h_proto;
	unsigned char* buff = skb_mac_header(skb);
	
	if ((0x8100 == eth_type) || (0x88a8 == eth_type))
	{
		return ((*(short int*)(buff+14)) & 0xfff);
	}
	
	return -1;
}

static int ecnt_get_vlan_pbit(struct sk_buff* skb)
{
	unsigned int eth_type = eth_hdr(skb)->h_proto;
	unsigned char* buff = skb_mac_header(skb);
	
	if ((0x8100 == eth_type) || (0x88a8 == eth_type))
	{
		return (((*(short int*)(buff+14)) >> 13) & 0x7);
	}
	
	return -1;
}

static void mtk_xpon_hwnat_update_flow(MTK_IGMP_HWNATEntry_t* entry ,unsigned long mask)
{
	int type = 0;
	int ret = 0;
	
	if (XPON_MASK_MLDV1 == entry->proto)
	{
		type = 1;
	}

	if (hwnat_is_multicast_entry_hook)
	{
		ret = hwnat_is_multicast_entry_hook(entry->index,entry->grp_addr,entry->src_addr,type);
	}

	if (ret < 1)
	{
		MULTI_LOG(MULTI_LOG_ERROR,"not multicast entry,delete it");
		mtk_xpon_hwnat_delete_entry(entry);
		return;
	}
	
	if(hwnat_multicast_set_hwnat_info_hook)
	{
		hwnat_multicast_set_hwnat_info_hook(entry->index,mask);
	}

	entry->mask = mask;
}

static void mtk_xpon_hwnat_close_foe(MTK_IGMP_HWNATEntry_t* entry)
{
	int type = 0;
	int ret = 0;

	if (XPON_MASK_MLDV1 == entry->proto)
	{
		type = 1;
	}

	if (hwnat_is_multicast_entry_hook)
	{
		ret = hwnat_is_multicast_entry_hook(entry->index,entry->grp_addr,entry->src_addr,type);
	}
	
	
	if ((ret > 0) && (hwnat_delete_foe_entry_hook))
	{
		hwnat_delete_foe_entry_hook(entry->index);
	}
}

static void mtk_xpon_hwnat_delete_flow(MTK_IGMP_HWNATEntry_t* entry)
{
	if (NULL == entry)
	{
		return;
	}

	mtk_xpon_hwnat_close_foe(entry);
	mtk_xpon_hwnat_delete_entry(entry);
}

void mtk_xpon_hwnat_clear_flows(void)
{
	MTK_IGMP_HWNATEntry_t* entry = NULL;
	MTK_IGMP_HWNATEntry_t* tmp = NULL;

	spin_lock_bh(&mtk_hwnat_mcst_lock);

	list_for_each_entry_safe(entry,tmp,&mtk_hwnat_igmp,list)
	{
		mtk_xpon_hwnat_delete_flow(entry);
	}

	spin_unlock_bh(&mtk_hwnat_mcst_lock);
}

MTK_IGMP_HWNATEntry_t* mtk_xpon_hwnat_find_entry(struct list_head* plist,int index)
{
	MTK_IGMP_HWNATEntry_t* entry = NULL;
	int find_flag = 0;
	
	if ((NULL == plist) || (0 >= index))
	{
		return NULL;
	}
	
	spin_lock_bh(&mtk_hwnat_mcst_lock);

	list_for_each_entry_rcu(entry,plist,list)
	{
		if (entry->index == index)
		{
			find_flag = 1;
			break;
		}      
	}

	spin_unlock_bh(&mtk_hwnat_mcst_lock);

	if (find_flag)
	{
		return entry;
	}

	return NULL;
}

MTK_IGMP_HWNATEntry_t* mtk_xpon_hwnat_find_drop_entry(int index)
{
	MTK_IGMP_HWNATEntry_t* entry = NULL;
	int find_flag = 0;
	struct list_head *plist = &mtk_hwnat_drop;
	
	if ((NULL == plist) || (0 >= index))
	{
		return NULL;
	}
	
	spin_lock_bh(&mtk_hwnat_drop_mcst_lock);
	list_for_each_entry(entry,plist,list)
	{
		if (entry->index == index)
		{
			find_flag = 1;
			break;
		}      
	}
	spin_unlock_bh(&mtk_hwnat_drop_mcst_lock);

	if (find_flag)
	{
		return entry;
	}

	return NULL;
}


void mtk_xpon_hwnat_timer_timeout(unsigned long arg)
{
	int ret  = 0;
	int type = 0;
	MTK_IGMP_HWNATEntry_t* entry = NULL;
	MTK_IGMP_HWNATEntry_t*   tmp = NULL;
	MTK_IGMP_HWNATEntry_t* hwnat_entry = NULL;

	spin_lock(&mtk_hwnat_mcst_lock);

	entry = (MTK_IGMP_HWNATEntry_t* )arg;
	if (NULL == entry)
	{
		goto out_unlock;
	}

	if (XPON_MASK_MLDV1 == entry->proto)
	{
		type = 1;
	}

	if (hwnat_is_multicast_entry_hook)
	{
		ret = hwnat_is_multicast_entry_hook(entry->index,entry->grp_addr,entry->src_addr,type);
	}

	if (1 == ret)
	{
		mod_timer(&entry->age_timer,round_jiffies(jiffies) + XPON_HWNAT_AGE_TIME);
		goto out_unlock;
	}

	if (entry)
	{
		list_for_each_entry_safe(hwnat_entry,tmp,&mtk_hwnat_igmp,list)
		{
			if((entry->index   == hwnat_entry->index)
			&& (entry->hwnat_vid  == hwnat_entry->hwnat_vid) 
			&& (entry->proto == hwnat_entry->proto))
			{
				if((XPON_MASK_IGMPV2 == entry->proto) 
				&& !memcmp(entry->grp_addr, hwnat_entry->grp_addr,4)
				&& !memcmp(entry->src_addr, hwnat_entry->src_addr,4))
				{
					mtk_xpon_hwnat_delete_flow(entry);
				}
				else if((XPON_MASK_MLDV1 == entry->proto)
				&& !memcmp(entry->grp_addr, hwnat_entry->grp_addr,16)
				&& !memcmp(entry->src_addr, hwnat_entry->src_addr,16))
				{
					mtk_xpon_hwnat_delete_flow(entry);
				}
			}
		}
	}   

out_unlock:
	spin_unlock(&mtk_hwnat_mcst_lock);
}

static void mtk_xpon_hwnat_delete_drop(MTK_IGMP_HWNATEntry_t* entry)
{
	int type = 0;
	int ret = 0;
	
	if (XPON_MASK_MLDV1 == entry->proto)
	{
		type = 1;
	}

	if (hwnat_is_drop_entry_hook)
	{
		ret = hwnat_is_drop_entry_hook(entry->index,entry->grp_addr,entry->src_addr,type);
	}
	
	if (hwnat_delete_foe_entry_hook)
	{
		hwnat_delete_foe_entry_hook(entry->index);
	}
	
	mtk_xpon_hwnat_delete_entry(entry);
}

void mtk_xpon_hwnat_clear_all_drop(void)
{
	MTK_IGMP_HWNATEntry_t* entry = NULL;
	MTK_IGMP_HWNATEntry_t*  tmp  = NULL;

	spin_lock_bh(&mtk_hwnat_drop_mcst_lock);

	list_for_each_entry_safe(entry, tmp, &mtk_hwnat_drop, list)
	{
		mtk_xpon_hwnat_delete_drop(entry);
	}

	spin_unlock_bh(&mtk_hwnat_drop_mcst_lock);
}

void mtk_xpon_hwnat_drop_timeout(unsigned long arg)
{
	int ret = 0;
	int type = 0;
	MTK_IGMP_HWNATEntry_t*            entry = NULL;
	MTK_IGMP_HWNATEntry_t*              tmp = NULL;
	MTK_IGMP_HWNATEntry_t* hwnat_drop_entry = NULL;

	spin_lock(&mtk_hwnat_drop_mcst_lock);

	entry = (MTK_IGMP_HWNATEntry_t* )arg;
	if (NULL == entry)
	{
		goto out_unlock;
	}
	
	if (XPON_MASK_MLDV1 == entry->proto)
	{
		type = 1;
	}

	if (hwnat_is_drop_entry_hook)
	{
		ret = hwnat_is_drop_entry_hook(entry->index,entry->grp_addr,entry->src_addr,type);
	}

	if (ret > 0)
	{
		mod_timer(&entry->age_timer,round_jiffies(jiffies)+XPON_HWNAT_AGE_TIME);
		goto out_unlock;
	}

	if (entry)
	{
		list_for_each_entry_safe(hwnat_drop_entry,tmp,&mtk_hwnat_drop,list)
		{
			if((entry->index   == hwnat_drop_entry->index)
			&& (entry->hwnat_vid  == hwnat_drop_entry->hwnat_vid)
			&& (entry->proto == hwnat_drop_entry->proto))
			{
				if((XPON_MASK_IGMPV2 == entry->proto)
				&& !memcmp(entry->grp_addr, hwnat_drop_entry->grp_addr,4)
				&& !memcmp(entry->src_addr, hwnat_drop_entry->src_addr,4))
				{
					mtk_xpon_hwnat_delete_drop(entry);
				}
				else if((XPON_MASK_MLDV1 == entry->proto)
				&& !memcmp(entry->grp_addr, hwnat_drop_entry->grp_addr,16)
				&& !memcmp(entry->src_addr, hwnat_drop_entry->src_addr,16))
				{
					mtk_xpon_hwnat_delete_drop(entry);
				}
			}
		}
	}   

out_unlock:
	spin_unlock(&mtk_hwnat_drop_mcst_lock);
}

void mtk_xpon_hwnat_add_drop(struct sk_buff* skb,int index)
{
	MTK_IGMP_HWNATEntry_t* entry = NULL;
	unsigned char dest_addr[16],src_addr[16];
	short int vid = -1,proto = 0;
	uint16 eth_type = 0;
	struct iphdr*  ih;
	struct ipv6hdr* i6h;
	unsigned char* buff = skb_mac_header(skb);
	int vlan_tag_num = mtk_xpon_get_vlan_tag_num(skb);
	uint16 pppoe_proto = 0;

	if ((NULL == skb) || (0 > index))
	{
		return;
	}

	eth_type = *(uint16*)(buff+12+4*vlan_tag_num);

	if (0 != vlan_tag_num)
	{
		vid = (vlan_eth_hdr(skb)->h_vlan_TCI & VLAN_VID_MASK);
	}

	buff += (14 + 4*vlan_tag_num);

	memset(dest_addr,0,16);
	memset(src_addr,0,16);
#if !defined(TCSUPPORT_FWC_MCST)
	if (eth_type == ETH_P_PPP_SES)
	{
		buff+= sizeof(struct pppoe_hdr);
		pppoe_proto = *(uint16*)buff;

		buff += 2; /* skip ppp header*/

		if (pppoe_proto == PPP_IP)
		{
			eth_type = ETH_P_IP;
		}
		else if (pppoe_proto == PPP_IPV6)
		{
			eth_type = ETH_P_IPV6;
		}
	}
#endif
	if (eth_type == ETH_P_IP)
	{
		proto = XPON_MASK_IGMPV2;
		ih = (struct iphdr*)buff;
		memcpy(dest_addr,&ih->daddr,4);
		memcpy(src_addr,&ih->saddr,4);
	}
	else if (eth_type == ETH_P_IPV6)
	{
		proto = XPON_MASK_MLDV1;
		i6h = (struct ipv6hdr*)buff;
		memcpy(dest_addr, i6h->daddr.s6_addr,16);
		memcpy(src_addr, i6h->saddr.s6_addr,16);	
	}
	else
	{
		return;
	}

	entry = (MTK_IGMP_HWNATEntry_t* )mtk_xpon_alloc(sizeof(MTK_IGMP_HWNATEntry_t));
	if (NULL == entry)
	{
		return;
	}

	spin_lock_bh(&mtk_hwnat_drop_mcst_lock);

	if (NULL == &mtk_hwnat_drop)
	{
		mtk_xpon_free(entry);
		goto out_unlock;
	}
	entry->proto = proto;
	entry->hwnat_vid= vid;
	entry->index = index;
	entry->mask = 0;
	memcpy(entry->grp_addr,dest_addr,16);
	memcpy(entry->src_addr,src_addr,16);
	list_add_tail(&entry->list, &mtk_hwnat_drop);
	setup_timer(&entry->age_timer, mtk_xpon_hwnat_drop_timeout, (unsigned long)entry);	
	mod_timer(&entry->age_timer,round_jiffies(jiffies)+XPON_HWNAT_AGE_TIME);

out_unlock:
	spin_unlock_bh(&mtk_hwnat_drop_mcst_lock);
}

int mtk_xpon_hwnat_drop_multicast(struct sk_buff* skb)
{
	int index = mtk_xpon_hwnat_flow_index(skb);
	MTK_IGMP_HWNATEntry_t* entry = NULL;
	int ret = 0;

	if (index <= 0)
	{
		return -1;
	}

	if (ra_sw_nat_hook_drop_packet)
	{
		ret = ra_sw_nat_hook_drop_packet(skb);
	}

	if (ret > 0)
	{		

		entry = mtk_xpon_hwnat_find_drop_entry(index);
		if (NULL == entry)
		{
			mtk_xpon_hwnat_add_drop(skb,index);
		}
		
		return 0;
	}
	else
	{
		return -1;
	}
}

int mtk_add_or_update_iptv_table_entry(short int group_vid ,MTK_IP_ADDRESS group_ip)
{
	MTK_IGMP_HWNATEntry_t*   entry = NULL;
	MTK_IGMP_HWNATEntry_t*   tmp = NULL;
	int find_flag = 0;

	spin_lock_bh(&mtk_hwnat_mcst_lock);
	list_for_each_entry_safe(entry,tmp,&mtk_hwnat_igmp,list)
	{
		if (entry->hwnat_vid == group_vid)
		{
			#if 1
			if(group_ip.is_ipv6 == 1)
			{
				if (!memcmp(entry->grp_addr,group_ip.IP.ipv6_address.ip_address_array,16))
				{
					find_flag = 1;
					break;
				} 

			}
			else
			{
				if (!memcmp(entry->grp_addr,&(group_ip.IP.ipv4_address),4))
				{
					find_flag = 1;
					break;
				} 
			}
			#else
			if(memcmp(entry->grp_addr,&(group_ip.IP),sizeof(MTK_IPV6_ADDRESS)) == 0)
			{
				find_flag = 1;
				break;
			}
			#endif
		}
	}
	spin_unlock_bh(&mtk_hwnat_mcst_lock);	

	if (find_flag)
	{
		mtk_update_iptv_table_entry();
	}
	else
	{
		mtk_add_iptv_table_entry();
	}
}

int mtk_add_iptv_table_entry(void)
{
	mtk_xpon_hwnat_clear_all_drop();
	return 0;
}

int mtk_remove_iptv_table_entry(void)
{
	mtk_xpon_hwnat_clear_flows();
	return 0;
}

int mtk_update_iptv_table_entry(void)
{
	mtk_xpon_hwnat_clear_flows();
	return 0;
}

int mtk_configure_downstream_vlan_action(uint32 vlanEtyId, uint16 vlanMode,uint32 outerVlan,uint32 innerVlan, uint32 outerTci,uint32 innerTci)
{
	mtk_xpon_hwnat_clear_flows();
	mtk_xpon_hwnat_clear_all_drop();
	return 0;
}


/*********************************************************************************
 * forward multicast vlan flow handler routine setction
 *
 *********************************************************************************/
static int mtk_mcts_vlan_transparent(struct sk_buff* skb,uint32 outerVlan,uint32 innerVlan,uint32 outerTci,uint32 innerTci);
static int mtk_mcts_vlan_replace(struct sk_buff* skb,uint32 outerVlan,uint32 innerVlan,uint32 outerTci,uint32 innerTci);
static int mtk_mcts_vlan_replace_and_remark(struct sk_buff* skb,uint32 outerVlan,uint32 innerVlan,uint32 outerTci,uint32 innerTci);
static int mtk_mcts_vlan_strip(struct sk_buff* skb,uint32 outerVlan,uint32 innerVlan,uint32 outerTci,uint32 innerTci);
static int mtk_mcts_vlan_add(struct sk_buff* skb,uint32 outerVlan,uint32 innerVlan,uint32 outerTci,uint32 innerTci);
static int mtk_mcts_vlan_add_and_remark(struct sk_buff* skb,uint32 outerVlan,uint32 innerVlan,uint32 outerTci,uint32 innerTci);

MTK_VLAN_HANDLE_t mtk_vlan_handle_s[] =
{
	{MCST_VLAN_TRANSPARENT,mtk_mcts_vlan_transparent},
	{MCST_VLAN_REPLACE,mtk_mcts_vlan_replace},	
	{MCST_VLAN_REPLACE_AND_REMARK,mtk_mcts_vlan_replace_and_remark},
	{MCST_VLAN_STRIP,mtk_mcts_vlan_strip},
	{MCST_VLAN_ADD,mtk_mcts_vlan_add},
	{MCST_VLAN_ADD_AND_REMARK,mtk_mcts_vlan_add_and_remark},
};
#define MTK_VLAN_HANDLE_S_SIZE sizeof(mtk_vlan_handle_s)/sizeof(MTK_VLAN_HANDLE_t)

int mtk_xpon_is_multicast_addr(unsigned char* addr)
{
	char mac[3]  =  {0x01,0x00,0x5e};
	char mac2[2] = {0x33,0x33};
    
    if (NULL == addr)
    {
        return 0;
    }
    
	if (!memcmp(addr,mac,3))
	{
		return 1;
	}
	
	if (!memcmp(addr,mac2,2))
	{
		return 1;
	}
	
	return 0;
}

int mtk_xpon_get_packet_type(struct sk_buff* skb)
{
	int vlan_tag_num = mtk_xpon_get_vlan_tag_num(skb);
	unsigned char* buff = skb_mac_header(skb);
	uint32 eth_type = 0;
	uint16 pppoe_proto = 0;

	buff += (12 + 4*vlan_tag_num);
	eth_type = *(uint16*)buff;

	buff += 2;/*skip ether type*/
#if !defined(TCSUPPORT_FWC_MCST)	
	if (eth_type == ETH_P_PPP_SES)
	{
		buff+= sizeof(struct pppoe_hdr);
		pppoe_proto = *(uint16*)buff;

		if (pppoe_proto == PPP_IP)
		{
			eth_type = ETH_P_IP;
		}
		else if (pppoe_proto == PPP_IPV6)
		{
			eth_type = ETH_P_IPV6;
		}
	}
#endif
	return eth_type;
}

void* mtk_xpon_get_network_header(struct sk_buff* skb)
{
	unsigned char* buff = skb_mac_header(skb);
	int vlan_tag_num = mtk_xpon_get_vlan_tag_num(skb);
	uint32 eth_type = 0;

	buff += (12 + 4*vlan_tag_num);
	eth_type = *(uint16*)buff;

	buff += 2;/*skip ether type*/
#if !defined(TCSUPPORT_FWC_MCST)
	if (eth_type == ETH_P_PPP_SES)
	{
		buff += sizeof(struct pppoe_hdr);
		buff += 2;/*skip ppp header*/
	}	
#endif	
	return buff;
}

int mtk_xpon_get_ip_type(struct sk_buff* skb)
{
	unsigned int eth_type = mtk_xpon_get_packet_type(skb);
	unsigned char* buff = NULL;
	struct iphdr* ih = NULL;
	struct ipv6hdr* i6h = NULL;

	if (eth_type == PACKET_IPV4)
	{
		ih = (struct iphdr* )mtk_xpon_get_network_header(skb);
		return ih->protocol;
	}

	if (eth_type == PACKET_IPV6)
	{
		buff = mtk_xpon_get_network_header(skb);
		i6h = (struct ipv6hdr*)buff;
		return ((i6h->nexthdr == 0) ? buff[40]:i6h->nexthdr);
	}

	return 0;
}

int mtk_xpon_is_data_pkt(struct sk_buff* skb)
{
	int type = mtk_xpon_get_ip_type(skb);
	
	if ((type == PROTOCOL_UDP) || (type == PROTOCOL_UDP6))
	{
		return 1;
	}

	return 0;
}

/*ipv4:224.0.0.0~239.255.255.255*/
static int mtk_xpon_is_ipv4_multicast_addr(uint32 addr)
{
	uint8 ip_high = ((addr&0xFF000000) >> 24);
	
	if ((ip_high < 224) || (ip_high > 239))
	{
		return 0;
	}
	
	return 1;
}

/*ipv6:|11111111|0|R|P|T|....*/
static int mtk_xpon_is_ipv6_multicast_addr(MTK_IPV6_ADDRESS* ipv6_addr)
{
	if ((0xFF == ipv6_addr->ip_address_array[0]) 
		&& (0 == (ipv6_addr->ip_address_array[1] >> 7)))
	{
		return 1;
	}
		
	return 0;
}

void mtk_xpon_get_src_and_dst_addr(MTK_IP_ADDRESS* src,MTK_IP_ADDRESS* dst,struct sk_buff* skb)
{

	unsigned int eth_type = mtk_xpon_get_packet_type(skb);
	struct iphdr*  ih;
	struct ipv6hdr* i6h;

	if (eth_type == PACKET_IPV4)
	{
		ih = (struct iphdr*)mtk_xpon_get_network_header(skb);
		if (src != NULL)
		{
			memcpy(&(src->IP.ipv4_address),(unsigned char*)&ih->saddr,4);
			src->is_ipv6 = 0;
		}

		if (dst != NULL)
		{
			memcpy(&(dst->IP.ipv4_address),(unsigned char*)&ih->daddr,4);
			dst->is_ipv6 = 0;
		}
	}
	else if (eth_type == PACKET_IPV6)
	{
		i6h = (struct ipv6hdr*)mtk_xpon_get_network_header(skb);
		if (src != NULL)
		{
			memcpy(src->IP.ipv6_address.ip_address_array,i6h->saddr.s6_addr,16);
			src->is_ipv6 = 1;
		}

		if (dst != NULL)
		{
			memcpy(dst->IP.ipv6_address.ip_address_array, i6h->daddr.s6_addr,16);
			dst->is_ipv6 = 1;
		}
	}
}

int mtk_xpon_is_pppox_multicast_pkt(struct sk_buff* skb)
{
	unsigned char* buff = skb_mac_header(skb);
	int vlan_tag_num = mtk_xpon_get_vlan_tag_num(skb);
	uint32 eth_type = 0;
	MTK_IP_ADDRESS src,dst;
	
	buff += (12 + 4*vlan_tag_num);
	eth_type = *(uint16*)buff;
	
	if (eth_type != ETH_P_PPP_SES)
	{
		return 0;
	}

	mtk_xpon_get_src_and_dst_addr(&src,&dst,skb);
	
	if ((0 == dst.is_ipv6) && mtk_xpon_is_ipv4_multicast_addr(dst.IP.ipv4_address))
	{
		return 1;
	}
	
	if ((1 == dst.is_ipv6) && mtk_xpon_is_ipv6_multicast_addr(&(dst.IP.ipv6_address)))
	{
		return 1;
	}
	
	return 0;
}

static int mtk_mcts_is_valid_pbit(uint32 pbit)
{
	if ((pbit >= 0) && (pbit <= 7))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

static int mtk_mcts_is_valid_vid(uint32 vid)
{
	if ((vid >= 0) && (vid <= 4094))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

static int mtk_xpon_get_vlan_tag_num(struct sk_buff* skb)
{
	uint16* tmp = NULL;
	int vlan_tag_num = 0;

	tmp = (uint16 *)skb_mac_header(skb);
	tmp += 6;

	while ((*tmp == htons(ETH_P_8021Q)) 
		|| (*tmp == htons(ETH_P_QinQ_88a8)) 
		|| (*tmp == htons(ETH_P_QinQ_9100)))
	{
		vlan_tag_num++;
		tmp += 2;
	}

	return vlan_tag_num;
}

static int ecnt_get_vlan_info(struct sk_buff *skb,struct vlan_info *pvlan)
{
	uint16 *tmp = NULL;
	int vlan_tag_num = 0;

	tmp = (uint16 *)skb_mac_header(skb);
	tmp += 6;

	while ((*tmp == htons(ETH_P_8021Q)) 
		|| (*tmp == htons(ETH_P_QinQ_88a8)) 
		|| (*tmp == htons(ETH_P_QinQ_9100)))
	{
		/* get outer TPID */
		pvlan[vlan_tag_num].outer_tpid = *tmp;

		/* move to TCI */
		tmp++;
		
		/* get outer TCI */
		pvlan[vlan_tag_num++].outer_tci = *tmp;

		/* move to inner TPID if having */
		tmp++;
	}

	return vlan_tag_num;
}

static int mtk_mcts_vlan_transparent(struct sk_buff* skb,uint32 outerVlan,uint32 innerVlan,uint32 outerTci,uint32 innerTci)
{
	return 0;
}

static int mtk_mcts_vlan_replace(struct sk_buff* skb,uint32 outerVlan,uint32 innerVlan,uint32 outerTci,uint32 innerTci)
{
	uint16* pvid = NULL;
	uint8* buff = skb_mac_header(skb);
	int vlan_tag_num = 0;
	
	vlan_tag_num = mtk_xpon_get_vlan_tag_num(skb);

	if ((vlan_tag_num > 0) && mtk_mcts_is_valid_vid(outerVlan))
	{
		pvid = (short int*)(buff + 14);
		*pvid = (*pvid) & 0xf000;
		*pvid = (*pvid) | outerVlan;	
	}

	if ((vlan_tag_num > 1) && mtk_mcts_is_valid_vid(innerVlan))
	{
		pvid = (short int*)(buff + 18);
		*pvid = (*pvid) & 0xf000;
		*pvid = (*pvid) | innerVlan;		
	}
	
	return 0;
}

static int mtk_mcts_vlan_replace_and_remark(struct sk_buff* skb,uint32 outerVlan,uint32 innerVlan,uint32 outerTci,uint32 innerTci)
{
	uint16* pvid = NULL;
	uint8* buff = skb_mac_header(skb);
	uint16 vlan_tci = 0;
	int vlan_tag_num = 0;
	
	vlan_tag_num = mtk_xpon_get_vlan_tag_num(skb);

	if ((vlan_tag_num > 0) 
		&& mtk_mcts_is_valid_vid(outerVlan) 
		&& mtk_mcts_is_valid_pbit(outerTci))
	{
		vlan_tci = (outerTci << 13) + outerVlan;
		pvid = (short int*)(buff + 14);
		*pvid = vlan_tci;
	}

	if ((vlan_tag_num > 1) 
		&& mtk_mcts_is_valid_vid(innerVlan) 
		&& mtk_mcts_is_valid_pbit(innerTci))
	{
		vlan_tci = (innerTci << 13) + innerVlan;
		pvid = (short int*)(buff + 18);
		*pvid = vlan_tci;
	}
	
	return 0;
}

static int mtk_mcts_vlan_strip(struct sk_buff* skb,uint32 outerVlan,uint32 innerVlan,uint32 outerTci,uint32 innerTci)
{
	int skb_pbit = 0;
	int skb_vid = 0;
	uint8 * buff = skb_mac_header(skb);
	uint16 type = 0;

	skb_pbit = ecnt_get_vlan_pbit(skb);
	if (skb_pbit >= 0)
	{
		/* save the original pbit */
		skb->mark |= (1 << QUQUE_REMARK_EXIST_OFFSET);
		
		skb->mark &= (~(QUQUE_REMARK_MASK << QUEUE_REMARK_OFFSET));
		skb->mark |= (skb_pbit << QUEUE_REMARK_OFFSET);
	}
	
	skb_vid = mtk_xpon_get_vlan_id(skb);

	if (skb_vid >= 0)
	{
		type = *(short int*)(buff+16);
		memmove(buff+4,buff,14);
		skb_pull(skb,4);
		skb->mac_header += 4;
		eth_hdr(skb)->h_proto = type;
		skb->protocol = type;
	}
	
	return 0;
}

static int mtk_mcts_vlan_add(struct sk_buff* skb,uint32 outerVlan,uint32 innerVlan,uint32 outerTci,uint32 innerTci)
{
	uint16 vlan_tci = 0;

	if (mtk_mcts_is_valid_vid(innerVlan))
	{
		vlan_tci = innerVlan;
		__vlan_put_tag(skb, vlan_tci);
	}

	if (mtk_mcts_is_valid_vid(outerVlan))
	{
		vlan_tci = outerVlan;
		 __vlan_put_tag(skb, vlan_tci);
	}
	
	return 0;
}

static int mtk_mcts_vlan_add_and_remark(struct sk_buff* skb,uint32 outerVlan,uint32 innerVlan,uint32 outerTci,uint32 innerTci)
{
	uint16 vlan_tci = 0;

	if (mtk_mcts_is_valid_vid(innerVlan) && mtk_mcts_is_valid_pbit(innerTci))
	{
		vlan_tci = innerVlan+(innerTci<<13);
		__vlan_put_tag(skb, vlan_tci);
	}

	if (mtk_mcts_is_valid_vid(outerVlan) && mtk_mcts_is_valid_pbit(outerTci))
	{
		vlan_tci = outerVlan+(outerTci<<13);
		__vlan_put_tag(skb, vlan_tci);
	}
	
	return 0;
}

int mtk_xpon_data_vlan_handle(struct sk_buff* skb,uint32 port_id)
{
	int i = 0;
	mc_fwd_tbl_entry entry;
	
	uint16 vlanMode = 0;
	uint32 outerVlan = 4096,innerVlan = 4096,outerTci = 0,innerTci = 0;

	memset(&entry,0,sizeof(mc_fwd_tbl_entry));

	/* get dwonstream vlan action */
	MULTI_LOG(MULTI_LOG_DBG,"data_vlan_handle: port id = %d\n",port_id);
	if (igmp_get_downstream_vlan_action(port_id,skb,NULL,-1,&entry))
	{
		vlanMode = entry.vlan_action.vlan_mode;
		outerVlan = (entry.vlan_action.outer_tci & 0xfff);
		outerTci = (entry.vlan_action.outer_tci >> 13);
		innerVlan = (entry.vlan_action.inner_tci & 0xfff);
		innerTci = (entry.vlan_action.inner_tci >> 13);

		MULTI_LOG(MULTI_LOG_DBG,"mtk_xpon_data_vlan_handle: \n");
		MULTI_LOG(MULTI_LOG_DBG,"vlanMode = %d\n",vlanMode);
		MULTI_LOG(MULTI_LOG_DBG,"outerVlan = %d\n",outerVlan);
		MULTI_LOG(MULTI_LOG_DBG,"outerTci = %d\n",outerTci);
		MULTI_LOG(MULTI_LOG_DBG,"innerVlan = %d\n",innerVlan);
		MULTI_LOG(MULTI_LOG_DBG,"innerTci = %d\n",innerTci);
	}
	
	for (i = 0; i < MTK_VLAN_HANDLE_S_SIZE; i++)
	{
		if (vlanMode == mtk_vlan_handle_s[i].vlan_mode)
		{
			mtk_vlan_handle_s[i].mtk_vlan_handle(skb,outerVlan,innerVlan,outerTci,innerTci);
			break;
		}
	}

	return 0;
}

int mtk_xpon_fwd_data_to_port(struct sk_buff* skb,uint32 fwd_port_mask)
{
	struct net_device* dev;
	unsigned char dev_name[16] = {0};
	int i = 0;
	struct sk_buff* skb2 = NULL;
	int interfacecount = 0;

	interfacecount = mtk_calc_lan_count(fwd_port_mask)+mtk_calc_wifi_count(fwd_port_mask);
	
	for (i = 0;i < MAX_PORT_NUM; i++)
	{
		if (fwd_port_mask & (1<<i))
		{
			if (--interfacecount < 0)
			{
				break;
			}

			skb2 = skb_copy(skb, GFP_ATOMIC);
			if (!skb2) 
			{
				continue;
			}
			
			mtk_xpon_data_vlan_handle(skb2,i);
			
			memset(dev_name,0,16);
			if (i < HWNAT_WLAN_IF_BASE)
			{
				sprintf(dev_name,"eth%d",i);
			}
			else//wifi interface
			{
				i -= HWNAT_WLAN_IF_BASE;
				if (i >= HWNAT_WLAN_IF_NUM)
				{
					i -= HWNAT_WLAN_IF_NUM;
					sprintf(dev_name,"rai%d",i);
				}
				else
				{
					sprintf(dev_name,"ra%d",i);
				}	
			}
				
			dev = dev_get_by_name(&init_net,dev_name);
			if (dev)
			{
				skb2->dev = dev;
				skb2->dev->netdev_ops->ndo_start_xmit(skb2,skb2->dev);
				dev_put(dev);
			}
			else
			{
				kfree_skb(skb2);
			}
		}
	}
	return 0;
}

static int mtk_xpon_hwnat_add_flow(struct sk_buff* skb, uint32 fwd_port_mask)
{
	int                   index = -1;
	short int               vid = -1;
	short int             proto = 0;
	int 			   eth_type = 0;
	short int       wifi_num = 0;
	unsigned long       switch_mask = 0;
	
	unsigned char dest_addr[16] = {0};
	unsigned char src_addr[16]  = {0};
	struct iphdr*  			 ih = NULL;
	struct ipv6hdr* 		i6h = NULL;
	MTK_IGMP_HWNATEntry_t*   tmp = NULL;
	MTK_IGMP_HWNATEntry_t* entry = NULL;
	MTK_IGMP_HWNATEntry_t* hwnat_entry = NULL;
	struct vlan_info vlan_infor[2];
	int vlan_tag_num = 0;

	index = mtk_xpon_hwnat_flow_index(skb);
	if (0 > index)
	{
		MULTI_LOG(MULTI_LOG_WARNING,"index is not correct");
		return 0;
	}
	
	if (0 == fwd_port_mask)
	{
		return 0;
	}
	
	eth_type = mtk_xpon_get_packet_type(skb);
	vid      = mtk_xpon_get_vlan_id(skb);
	vlan_tag_num = ecnt_get_vlan_info(skb,vlan_infor);
	memset(dest_addr, 0, 16);
	memset(src_addr,  0, 16);

	if (PACKET_IPV4 == eth_type)
	{
		proto = XPON_MASK_IGMPV2;
		ih = (struct iphdr* ) mtk_xpon_get_network_header(skb);
		memcpy(dest_addr,&ih->daddr,4);
		memcpy(src_addr,&ih->saddr,4);
	}
	else if (PACKET_IPV6 == eth_type)
	{
		proto = XPON_MASK_MLDV1;
		i6h = (struct ipv6hdr*) mtk_xpon_get_network_header(skb);
		memcpy(dest_addr, i6h->daddr.s6_addr,16);
		memcpy(src_addr, i6h->saddr.s6_addr,16);

	}
	else
	{
		MULTI_LOG(MULTI_LOG_WARNING,"eth_type(%x) is not support!",eth_type);
		return 0;
	}
	switch_mask = mtk_calc_port_mask(fwd_port_mask);
	//MULTI_LOG(MULTI_LOG_DBG,"switch_mask = 0x%x\n",switch_mask);
	wifi_num = mtk_calc_wifi_count(fwd_port_mask);

	entry = (MTK_IGMP_HWNATEntry_t* )mtk_xpon_alloc(sizeof(MTK_IGMP_HWNATEntry_t));
	
	if (NULL == entry)
	{
		MULTI_LOG(MULTI_LOG_ERROR,"Not enough memory!");
		return 0;
	}
	
	spin_lock_bh(&mtk_hwnat_mcst_lock);
	
	/*before add entry, del same entry*/
	list_for_each_entry_safe(hwnat_entry, tmp, &mtk_hwnat_igmp, list)    
	{
	
		if (hwnat_entry->index == index)
		{
			mtk_xpon_hwnat_delete_entry(hwnat_entry);
		}      
	}
	
	entry->proto  = proto;
	entry->hwnat_vid   = vid;
	entry->index = index;
	entry->mask  = /*fwd_port_mask*/switch_mask | (fwd_port_mask &(~(uint32)0xff));
	//MULTI_LOG(MULTI_LOG_DBG,"entry->mask = 0x%x\n",entry->mask);
	entry->wifinum = wifi_num;

	/* save the skb original vlan */
	if (1 == vlan_tag_num)
	{
		entry->vlan_tag_num = 1;
		entry->outer_vlan = ((vlan_infor[0].outer_tpid << 16) | vlan_infor[0].outer_tci);
		entry->inner_vlan = 0;
	}
	else if (2 == vlan_tag_num)
	{
		entry->vlan_tag_num = 2;
		entry->outer_vlan = ((vlan_infor[0].outer_tpid << 16) | vlan_infor[0].outer_tci);
		entry->inner_vlan = ((vlan_infor[1].inner_tpid << 16) | vlan_infor[1].inner_tci);
	}
	else
	{
		entry->vlan_tag_num = 0;
		entry->outer_vlan = 0;
		entry->inner_vlan = 0;
	}
	
	memcpy(entry->grp_addr, dest_addr, 16);
	memcpy(entry->src_addr, src_addr,  16);
	MULTI_LOG(MULTI_LOG_DBG,"Add an entry");
	list_add_tail(&entry->list,&mtk_hwnat_igmp);
	setup_timer(&entry->age_timer, mtk_xpon_hwnat_timer_timeout, (unsigned long)entry);	
	mod_timer(&entry->age_timer, round_jiffies(jiffies) + XPON_HWNAT_AGE_TIME);
	spin_unlock_bh(&mtk_hwnat_mcst_lock);
	return 0;
}

MTK_IGMP_HWNATEntry_t* mtk_xpon_hwnat_find_speed_entry(int index)
{
	return mtk_xpon_hwnat_find_entry(&mtk_hwnat_igmp,index);
}

int mtk_xpon_hwnat_update_all_flow(void)
{
	MTK_IGMP_HWNATEntry_t* entry = NULL;
	MTK_IGMP_HWNATEntry_t* tmp   = NULL;
	
	spin_lock_bh(&mtk_hwnat_mcst_lock);
	list_for_each_entry_safe(entry, tmp, &mtk_hwnat_igmp, list)
	{
		if (entry->mask == 0)
		{
			mtk_xpon_hwnat_delete_flow(entry);  
		}
		else
		{
			mtk_xpon_hwnat_update_flow(entry, entry->mask);

		}
	}

	spin_unlock_bh(&mtk_hwnat_mcst_lock);

	return 0;
}


/*___________________________________________________________________________
**      function name: ecnt_hwnat_update_flows
**      descriptions:
**      	update all the hw_nat entry flows
**          and this function is only used by PpeTxHandle in ra_nat.c
**      input parameters:
**		N/A
**      output parameters:
**      	N/A
**
**      return:
**      	N/A
**___________________________________________________________________________
*/
void ecnt_hwnat_update_flows(void)
{
	unsigned long lock_flag;
	MTK_IGMP_HWNATEntry_t *tmp = NULL;
	MTK_IGMP_HWNATEntry_t *entry = NULL;

	spin_lock_irqsave(&mtk_hwnat_mcst_lock,lock_flag);
	list_for_each_entry_safe(entry,tmp,&mtk_hwnat_igmp,list)
	{
		if (0 == entry->mask)
		{
			mtk_xpon_hwnat_delete_flow(entry);  
		}
		else
		{
			mtk_xpon_hwnat_update_flow(entry,entry->mask);
		}
	}
	spin_unlock_irqrestore(&mtk_hwnat_mcst_lock,lock_flag);
}

int mtk_xpon_hwnat_learn_flow(struct sk_buff* skb)
{
	unsigned char* dst = eth_hdr(skb)->h_dest;

	if(!mtk_xpon_is_multicast_addr(dst)
#if !defined(TCSUPPORT_FWC_MCST)
	 && !mtk_xpon_is_pppox_multicast_pkt(skb)
#endif
	 )
	{
		return 0;
	}
	
	MULTI_LOG(MULTI_LOG_DBG,"tx bind enter!!!!");
	ecnt_hwnat_update_flows();
	return 0;
}


/*********************************************************************************
 * ecnt igmp multicast forward table management setction
 *
 *********************************************************************************/
void igmp_init_fdb(void)
{
	mc_fdb.fwd_mode = FWD_MODE_IP_AND_VID;
	INIT_LIST_HEAD(&mc_fdb.fwd_list);
}

struct list_head* igmp_get_fdb_list(void)
{
	return &mc_fdb.fwd_list;
}

void igmp_clear_entry(void)
{
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	struct list_head *fwd_list = igmp_get_fdb_list();
	
	if (list_empty(fwd_list))
	{
		return;
	}

	spin_lock_bh(&ecnt_fwd_tbl_lock);
	list_for_each_entry_safe(entry,tmp,fwd_list,list)
	{
		list_del(&entry->list);
		mtk_xpon_free(entry);
	}
	spin_unlock_bh(&ecnt_fwd_tbl_lock);

	mtk_remove_iptv_table_entry();
}

static int ecnt_igmp_set_filterMode(mc_fwd_mode mode)
{
	if (mc_fdb.fwd_mode != mode)
	{
		igmp_clear_entry();
	}
	
	mc_fdb.fwd_mode = mode;
	
	return 0;
}

static int ecnt_igmp_get_filterMode(void)
{

	return mc_fdb.fwd_mode;
}

static int switch_snooping_on2off(void)
{
	igmp_clear_entry();
	mtk_xpon_hwnat_clear_all_drop();
	mtk_xpon_hwnat_clear_flows();
	return 0;
}

static int switch_snooping_off2on(void)
{
	mtk_xpon_hwnat_clear_flows();
	return 0;
}

static int ecnt_igmp_get_snooping_on_off(unsigned char *enable)
{
	spin_lock_bh(&ecnt_fwd_tbl_lock);
	*enable = g_igmp_snooping_on_off;
	spin_unlock_bh(&ecnt_fwd_tbl_lock);
    
	return 0;
}

static int ecnt_igmp_set_snooping_on_off(unsigned char enable)
{
	if (g_igmp_snooping_on_off == enable)
	{
		return 0;
	}
	
	spin_lock_bh(&ecnt_fwd_tbl_lock);
	g_igmp_snooping_on_off = enable;
	spin_unlock_bh(&ecnt_fwd_tbl_lock);

	/* compare current state */
	if (E_IGMP_SNOOPING_OFF == enable)
	{
		switch_snooping_on2off();
	}
	else
	{
		switch_snooping_off2on();
	}

	return 0;
}

static inline void igmp_get_entry(MC_FwdEntry *entry,mc_fwd_tbl_entry *pdata)
{
	if (!entry->group_addr.is_ipv6)
	{
		memcpy(&pdata->group_addr.IP.ipv4_address,&entry->group_addr.IP.ipv4_address,4);
	}
	else
	{
		memcpy(pdata->group_addr.IP.ipv6_address.ip_address_array,
			entry->group_addr.IP.ipv6_address.ip_address_array,16);
	}
	
	memcpy(pdata->group_mac.mac_address_byte,entry->group_mac.mac_address_byte,6);
	
	pdata->port_id = entry->port_id;
	pdata->vlan_id = entry->vlan_id;
	pdata->vlan_action.vlan_mode = entry->vlan_action.vlan_mode;
	pdata->vlan_action.outer_tpid = entry->vlan_action.outer_tpid;
	pdata->vlan_action.outer_tci = entry->vlan_action.outer_tci;
	pdata->vlan_action.inner_tpid = entry->vlan_action.inner_tpid;
	pdata->vlan_action.inner_tci = entry->vlan_action.inner_tci;
}

static inline void igmp_set_entry(MC_FwdEntry *entry,mc_fwd_tbl_entry *pdata)
{
	if (!pdata->group_addr.is_ipv6)
	{
		memcpy(&entry->group_addr.IP.ipv4_address,&pdata->group_addr.IP.ipv4_address,4);
	}
	else
	{
		memcpy(entry->group_addr.IP.ipv6_address.ip_address_array,
			pdata->group_addr.IP.ipv6_address.ip_address_array,16);
	}
	
	memcpy(entry->group_mac.mac_address_byte,pdata->group_mac.mac_address_byte,6);
	
	entry->port_id = pdata->port_id;
	entry->vlan_id = pdata->vlan_id;
	entry->vlan_action.vlan_mode = pdata->vlan_action.vlan_mode;
	entry->vlan_action.outer_tpid = pdata->vlan_action.outer_tpid;
	entry->vlan_action.outer_tci = pdata->vlan_action.outer_tci;
	entry->vlan_action.inner_tpid = pdata->vlan_action.inner_tpid;
	entry->vlan_action.inner_tci = pdata->vlan_action.inner_tci;
}

/*___________________________________________________________________________
**      function name: igmp_find_entry
**      descriptions:
**      	find the entry according to the "key info" from multicast forward table
** 		the "key info" conforms to the current forward mode. Example for as following:
**		fwd mode = MAC, that "key info" include entry port_id and entry group mac
**		fwd mode = IP + VID, that "key info" include port_id,entry vid and entry group ip
** 
**      input parameters:
**      	pdata:	the entry key information
**      output parameters:
**      	N/A
**
**      return:
**      	success:	entry
**		failure:	NULL
**___________________________________________________________________________
*/
MC_FwdEntry* igmp_find_entry_by_ip_vid(mc_fwd_tbl_entry *pdata)
{
	bool isIPv6 = 0;
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	struct list_head *fwd_list = igmp_get_fdb_list();
	
	if ((list_empty(fwd_list)) || (NULL == pdata))
	{
		return NULL;
	}
	
	isIPv6 = pdata->group_addr.is_ipv6;
	if (!isIPv6)
	{
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->port_id == pdata->port_id) 
				&& (entry->vlan_id == pdata->vlan_id) 
				&& (!memcmp(&(entry->group_addr.IP.ipv4_address),&(pdata->group_addr.IP.ipv4_address),4)))
			{
				return entry;
			}
		}
	}
	else
	{
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->port_id == pdata->port_id) 
				&& (entry->vlan_id == pdata->vlan_id) 
				&& (!memcmp(entry->group_addr.IP.ipv6_address.ip_address_array,
				pdata->group_addr.IP.ipv6_address.ip_address_array,16)))
			{
				return entry;
			}
		}
	}

	return NULL;
}

MC_FwdEntry* igmp_find_entry_by_mac(mc_fwd_tbl_entry *pdata)
{
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	struct list_head *fwd_list = igmp_get_fdb_list();
	
	if ((list_empty(fwd_list)) || (NULL == pdata))
	{
		return NULL;
	}
	
	list_for_each_entry_safe(entry,tmp,fwd_list,list)
	{
		if ((entry->port_id == pdata->port_id) 
			&& (!memcmp(entry->group_mac.mac_address_byte,pdata->group_mac.mac_address_byte,ETH_ALEN)))
		{
			return entry;
		}
	}
	
	return NULL;
}

MC_FwdEntry* igmp_find_entry_by_mac_ip(mc_fwd_tbl_entry *pdata)
{
	bool isIPv6 = 0;
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	struct list_head *fwd_list = igmp_get_fdb_list();
	
	if ((list_empty(fwd_list)) || (NULL == pdata))
	{
		return NULL;
	}
	
	isIPv6 = pdata->group_addr.is_ipv6;
	if (!isIPv6)
	{
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->port_id == pdata->port_id) 
				&& (!memcmp(entry->group_mac.mac_address_byte,pdata->group_mac.mac_address_byte,ETH_ALEN)) 
				&& (!memcmp(&(entry->group_addr.IP.ipv4_address),&(pdata->group_addr.IP.ipv4_address),4)))
			{
				return entry;
			}
		}
	}
	else
	{
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->port_id == pdata->port_id) 
				&& (!memcmp(entry->group_mac.mac_address_byte,pdata->group_mac.mac_address_byte,ETH_ALEN)) 
				&& (!memcmp(entry->group_addr.IP.ipv6_address.ip_address_array,
				pdata->group_addr.IP.ipv6_address.ip_address_array,16)))
			{
				return entry;
			}
		}
	}

	return NULL;
}

MC_FwdEntry* igmp_find_entry_by_all(mc_fwd_tbl_entry *pdata)
{
	bool isIPv6 = 0;
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	struct list_head *fwd_list = igmp_get_fdb_list();
	
	if ((list_empty(fwd_list)) || (NULL == pdata))
	{
		return NULL;
	}
	
	isIPv6 = pdata->group_addr.is_ipv6;
	if (!isIPv6)
	{
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->port_id == pdata->port_id) 
				&& (entry->vlan_id == pdata->vlan_id)
				&& (!memcmp(entry->group_mac.mac_address_byte,pdata->group_mac.mac_address_byte,ETH_ALEN)) 
				&& (!memcmp(&(entry->group_addr.IP.ipv4_address),&(pdata->group_addr.IP.ipv4_address),4)))
			{
				return entry;
			}
		}
	}
	else
	{
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->port_id == pdata->port_id) 
				&& (entry->vlan_id == pdata->vlan_id)
				&& (!memcmp(entry->group_mac.mac_address_byte,pdata->group_mac.mac_address_byte,ETH_ALEN)) 
				&& (!memcmp(entry->group_addr.IP.ipv6_address.ip_address_array,
				pdata->group_addr.IP.ipv6_address.ip_address_array,16)))
			{
				return entry;
			}
		}
	}

	return NULL;
}

MC_FwdEntry* igmp_find_entry_by_mac_vid(mc_fwd_tbl_entry *pdata)
{
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	struct list_head *fwd_list = igmp_get_fdb_list();
	
	if ((list_empty(fwd_list)) || (NULL == pdata))
	{
		return NULL;
	}
	
	list_for_each_entry_safe(entry,tmp,fwd_list,list)
	{
		if ((entry->port_id == pdata->port_id) 
			&& (entry->vlan_id == pdata->vlan_id)
			&& (!memcmp(entry->group_mac.mac_address_byte,pdata->group_mac.mac_address_byte,ETH_ALEN)))
		{
			return entry;
		}
	}
	
	return NULL;
}

MC_FwdEntry* igmp_find_entry(mc_fwd_tbl_entry *pdata)
{
	mc_fwd_mode fwd_mode;
	MC_FwdEntry *entry = NULL;
	
	fwd_mode = ecnt_igmp_get_filterMode();
	switch (fwd_mode)
	{
		case FWD_MODE_IP_AND_VID:
			entry = igmp_find_entry_by_ip_vid(pdata);
			break;
		case FWD_MODE_MAC:
			entry = igmp_find_entry_by_mac(pdata);
			break;
		case FWD_MODE_MAC_AND_IP:
			entry = igmp_find_entry_by_mac_ip(pdata);
			break;
		case FWD_MODE_MAC_AND_IP_AND_VID:
			entry = igmp_find_entry_by_all(pdata);
			break;
		case FWD_MODE_MAC_AND_VID:
			entry = igmp_find_entry_by_mac_vid(pdata);
			break;
		default:
			return NULL;
	}
	
	return entry;
}

int igmp_find_data_by_ip_vid(mc_fwd_tbl_entry *pdata,int *flow_idx,uint32 *port_mask,int opt)
{
	bool mcst_exist = 0;
	bool isIPv6 = 0;
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	struct list_head *fwd_list = igmp_get_fdb_list();
		
	if ((list_empty(fwd_list)) || (NULL == pdata))
	{
		return 0;
	}
		
	isIPv6 = pdata->group_addr.is_ipv6;
	if (!isIPv6)
	{
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->vlan_id == pdata->vlan_id) 
				&& (!memcmp(&entry->group_addr.IP.ipv4_address,&(pdata->group_addr.IP.ipv4_address),4)))
			{
				/* calc the port mask */
				*port_mask |= (1 << (entry->port_id));
				*flow_idx = entry->flow_idx;
				mcst_exist = 1;
			}
		}
	}
	else
	{
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->vlan_id == pdata->vlan_id) 
				&& (!memcmp(entry->group_addr.IP.ipv6_address.ip_address_array,
				pdata->group_addr.IP.ipv6_address.ip_address_array,16)))
			{
				/* calc the port mask */
				*port_mask |= (1 << (entry->port_id));
				*flow_idx = entry->flow_idx;
				mcst_exist = 1;
			}
		}
	}

	/* the port id is going to join or leave the same group */
	if (mcst_exist)
	{
		if (IGMP_ADD_ENTRY == opt)
		{
			*port_mask |= (1 << (pdata->port_id));
		}
		else if (IGMP_DEL_ENTRY == opt)
		{
			*port_mask &= (~(1 << (pdata->port_id)));
		}
	}
	
	return mcst_exist;
}

int igmp_find_data_by_mac(mc_fwd_tbl_entry *pdata,int *flow_idx,uint32 *port_mask,int opt)
{
	bool mcst_exist = 0;
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	struct list_head *fwd_list = igmp_get_fdb_list();
		
	if ((list_empty(fwd_list)) || (NULL == pdata))
	{
		return 0;
	}
	
	list_for_each_entry_safe(entry,tmp,fwd_list,list)
	{
		if (!memcmp(entry->group_mac.mac_address_byte,pdata->group_mac.mac_address_byte,ETH_ALEN))
		{
			/* calc the port mask */
			*port_mask |= (1 << (entry->port_id));
			*flow_idx = entry->flow_idx;
			mcst_exist = 1;
		}
	}
	
	/* the port id is going to join or leave the same group */
	if (mcst_exist)
	{
		if (IGMP_ADD_ENTRY == opt)
		{
			*port_mask |= (1 << (pdata->port_id));
		}
		else if (IGMP_DEL_ENTRY == opt)
		{
			*port_mask &= (~(1 << (pdata->port_id)));
		}
	}
	
	return mcst_exist;
}

int igmp_find_data_by_mac_ip(mc_fwd_tbl_entry *pdata,int *flow_idx,uint32 *port_mask,int opt)
{
	bool mcst_exist = 0;
	bool isIPv6 = 0;
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	struct list_head *fwd_list = igmp_get_fdb_list();
	
	if ((list_empty(fwd_list)) || (NULL == pdata))
	{
		return 0;
	}
	
	isIPv6 = pdata->group_addr.is_ipv6;
	if (!isIPv6)
	{
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((!memcmp(entry->group_mac.mac_address_byte,pdata->group_mac.mac_address_byte,ETH_ALEN)) 
				&& (!memcmp(&entry->group_addr.IP.ipv4_address,&(pdata->group_addr.IP.ipv4_address),4)))
			{
				/* calc the port mask */
				*port_mask |= (1 << (entry->port_id));
				*flow_idx = entry->flow_idx;
				mcst_exist = 1;
			}
		}
	}
	else
	{
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((!memcmp(entry->group_mac.mac_address_byte,pdata->group_mac.mac_address_byte,ETH_ALEN)) 
				&& (!memcmp(entry->group_addr.IP.ipv6_address.ip_address_array,
				pdata->group_addr.IP.ipv6_address.ip_address_array,16)))
			{
				/* calc the port mask */
				*port_mask |= (1 << (entry->port_id));
				*flow_idx = entry->flow_idx;
				mcst_exist = 1;
			}
		}
	}
	
	/* the port id is going to join or leave the same group */
	if (mcst_exist)
	{
		if (IGMP_ADD_ENTRY == opt)
		{
			*port_mask |= (1 << (pdata->port_id));
		}
		else if (IGMP_DEL_ENTRY == opt)
		{
			*port_mask &= (~(1 << (pdata->port_id)));
		}
	}
	
	return mcst_exist;
}

int igmp_find_data_by_all(mc_fwd_tbl_entry *pdata,int *flow_idx,uint32 *port_mask,int opt)
{
	bool mcst_exist = 0;
	bool isIPv6 = 0;
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	struct list_head *fwd_list = igmp_get_fdb_list();
	
	if ((list_empty(fwd_list)) || (NULL == pdata))
	{
		return 0;
	}
	
	isIPv6 = pdata->group_addr.is_ipv6;
	if (!isIPv6)
	{
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->vlan_id == pdata->vlan_id)
				&& (!memcmp(entry->group_mac.mac_address_byte,pdata->group_mac.mac_address_byte,ETH_ALEN)) 
				&& (!memcmp(&entry->group_addr.IP.ipv4_address,&(pdata->group_addr.IP.ipv4_address),4)))
			{
				/* calc the port mask */
				*port_mask |= (1 << (entry->port_id));
				*flow_idx = entry->flow_idx;
				mcst_exist = 1;
			}
		}
	}
	else
	{
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->vlan_id == pdata->vlan_id)
				&& (!memcmp(entry->group_mac.mac_address_byte,pdata->group_mac.mac_address_byte,ETH_ALEN)) 
				&& (!memcmp(entry->group_addr.IP.ipv6_address.ip_address_array,
				pdata->group_addr.IP.ipv6_address.ip_address_array,16)))
			{
				/* calc the port mask */
				*port_mask |= (1 << (entry->port_id));
				*flow_idx = entry->flow_idx;
				mcst_exist = 1;
			}
		}
	}
	
	/* the port id is going to join or leave the same group */
	if (mcst_exist)
	{
		if (IGMP_ADD_ENTRY == opt)
		{
			*port_mask |= (1 << (pdata->port_id));
		}
		else if (IGMP_DEL_ENTRY == opt)
		{
			*port_mask &= (~(1 << (pdata->port_id)));
		}
	}
	
	return mcst_exist;
}

int igmp_find_data_by_mac_vid(mc_fwd_tbl_entry *pdata,int *flow_idx,uint32 *port_mask,int opt)
{
	bool mcst_exist = 0;
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	struct list_head *fwd_list = igmp_get_fdb_list();
	
	if ((list_empty(fwd_list)) || (NULL == pdata))
	{
		return 0;
	}
	
	list_for_each_entry_safe(entry,tmp,fwd_list,list)
	{
		if ((entry->vlan_id == pdata->vlan_id)
			&& (!memcmp(entry->group_mac.mac_address_byte,pdata->group_mac.mac_address_byte,ETH_ALEN)))
		{
			/* calc the port mask */
			*port_mask |= (1 << (entry->port_id));
			*flow_idx = entry->flow_idx;
			mcst_exist = 1;
		}
	}
	
	/* the port id is going to join or leave the same group */
	if (mcst_exist)
	{
		if (IGMP_ADD_ENTRY == opt)
		{
			*port_mask |= (1 << (pdata->port_id));
		}
		else if (IGMP_DEL_ENTRY == opt)
		{
			*port_mask &= (~(1 << (pdata->port_id)));
		}
	}
	
	return mcst_exist;
}

int igmp_is_programme_exist(mc_fwd_tbl_entry *pdata,int *flow_idx,uint32 *port_mask,int opt)
{
	int ret;
	mc_fwd_mode fwd_mode;
		
	fwd_mode = ecnt_igmp_get_filterMode();
	switch (fwd_mode)
	{
		case FWD_MODE_IP_AND_VID:
			ret = igmp_find_data_by_ip_vid(pdata,flow_idx,port_mask,opt);
			break;
		case FWD_MODE_MAC:
			ret = igmp_find_data_by_mac(pdata,flow_idx,port_mask,opt);
			break;
		case FWD_MODE_MAC_AND_IP:
			ret = igmp_find_data_by_mac_ip(pdata,flow_idx,port_mask,opt);
			break;
		case FWD_MODE_MAC_AND_IP_AND_VID:
			ret = igmp_find_data_by_all(pdata,flow_idx,port_mask,opt);
			break;
		case FWD_MODE_MAC_AND_VID:
			ret = igmp_find_data_by_mac_vid(pdata,flow_idx,port_mask,opt);
			break;
		default:
			return 0;
	}
		
	return ret;
}

/*___________________________________________________________________________
**      function name: ecnt_get_entry_vlan_action
**      descriptions:
**      	only used at vlan 1:1 to get the entry vlan mode
**      input parameters:
**		port_mask: 
**      output parameters:
**      	vlan information
**
**      return:
**      	success:	1
**		failure:	0
**___________________________________________________________________________
*/
int ecnt_get_entry_vlan_action(uint32 port_mask,struct vlan_info *info)
{
	MC_FwdEntry *tmp = NULL;
	MC_FwdEntry *entry = NULL;
	
	struct list_head *fwd_list = igmp_get_fdb_list();
	
	if (list_empty(fwd_list))
	{
		return 0;
	}

	spin_lock_bh(&ecnt_fwd_tbl_lock);
	list_for_each_entry_safe(entry,tmp,fwd_list,list)
	{
		if (0 == (port_mask & (1 << (entry->port_id))))
		{
			continue;
		}
		
		info->vlan_mode = entry->vlan_action.vlan_mode;
		info->outer_tpid = entry->vlan_action.outer_tpid;
		info->outer_tci = entry->vlan_action.outer_tci;
		info->inner_tpid = entry->vlan_action.inner_tpid;
		info->inner_tci = entry->vlan_action.inner_tci;

		spin_unlock_bh(&ecnt_fwd_tbl_lock);
		return 1;
	}
	spin_unlock_bh(&ecnt_fwd_tbl_lock);
	
	return 0;
}

void ecnt_mcst_get_skb_vlan(int flow_idx,int *outer_vlan,int *inner_vlan,int *tag_num)
{
	MTK_IGMP_HWNATEntry_t *tmp = NULL;
	MTK_IGMP_HWNATEntry_t *entry = NULL;

	spin_lock_bh(&mtk_hwnat_mcst_lock);
	list_for_each_entry_safe(entry,tmp,&mtk_hwnat_igmp,list)
	{
		if (entry->index == flow_idx)
		{
			*outer_vlan = entry->outer_vlan;
			*inner_vlan = entry->inner_vlan;
			*tag_num = entry->vlan_tag_num;
		}
	}
	spin_unlock_bh(&mtk_hwnat_mcst_lock);
}

void ecnt_mcst_vlan_replace(int flow_idx,struct vlan_info v_info,int *o_vlan,int *i_vlan)
{
	int skb_outer_vlan = 0;
	int skb_inner_vlan = 0;
	int outer_vid = 0;
	int inner_vid = 0;
	int vlan_tag_num = 0;

	/* fwd table entry vlan */
	outer_vid = (v_info.outer_tci & 0xfff);
	inner_vid = (v_info.inner_tci & 0xfff);

	/* skb original vlan */
	ecnt_mcst_get_skb_vlan(flow_idx,&skb_outer_vlan,&skb_inner_vlan,&vlan_tag_num);

	/* vlan replace */
	if ((vlan_tag_num > 0) && (mtk_mcts_is_valid_vid(outer_vid)))
	{
		/* outer vlanid replace */
		skb_outer_vlan &= (~(0xfff));
		skb_outer_vlan |= outer_vid;
		
		*o_vlan = skb_outer_vlan;
	}
	
	if ((vlan_tag_num > 1) && (mtk_mcts_is_valid_vid(inner_vid)))
	{
		/* inner vlanid replace */
		skb_inner_vlan &= (~(0xfff));
		skb_inner_vlan |= inner_vid;

		*i_vlan = skb_inner_vlan;
	}
}

void ecnt_mcst_vlan_replace_and_remark(int flow_idx,struct vlan_info v_info,int *o_vlan,int *i_vlan)
{
	int skb_outer_vlan = 0;
	int skb_inner_vlan = 0;
	int outer_vid = 0;
	int inner_vid = 0;
	int outer_pbit = 0;
	int inner_pbit = 0;
	int vlan_tag_num = 0;

	/* fwd table entry vlan */
	outer_vid = (v_info.outer_tci & 0xfff);
	inner_vid = (v_info.inner_tci & 0xfff);
	outer_pbit = (v_info.outer_tci >> 13);
	inner_pbit = (v_info.inner_tci >> 13);

	/* skb original vlan */
	ecnt_mcst_get_skb_vlan(flow_idx,&skb_outer_vlan,&skb_inner_vlan,&vlan_tag_num);

	/* vlan replace and remark */
	if ((vlan_tag_num > 0)
		&& (mtk_mcts_is_valid_vid(outer_vid)) 
		&& (mtk_mcts_is_valid_pbit(outer_pbit)))
	{	
		/* vlanid replace */
		skb_outer_vlan &= (~(0xfff));
		skb_outer_vlan |= outer_vid;

		/* pbit remark */
		skb_outer_vlan &= (~(0x7 << 13));
		skb_outer_vlan |= (outer_pbit << 13);
		
		*o_vlan = skb_outer_vlan;
	}

	if ((vlan_tag_num > 1)
		&& (mtk_mcts_is_valid_vid(inner_vid)) 
		&& (mtk_mcts_is_valid_pbit(inner_pbit)))
	{
		/* vlanid replace */
		skb_inner_vlan &= (~(0xfff));
		skb_inner_vlan |= inner_vid;

		/* pbit remark */
		skb_inner_vlan &= (~(0x7 << 13));
		skb_inner_vlan |= (inner_pbit << 13);
		
		*i_vlan = skb_inner_vlan;
	}
}

void ecnt_mcst_vlan_add(int flow_idx,struct vlan_info v_info,int *o_vlan,int *i_vlan)
{
	int outer_vid = 0;
	int inner_vid = 0;
	int vlan_tag_num = 0;
	int skb_outer_vlan = 0;
	int skb_inner_vlan = 0;

	/* fwd table entry vlan */
	outer_vid = (v_info.outer_tci & 0xfff);
	inner_vid = (v_info.inner_tci & 0xfff);

	//only test
	v_info.outer_tpid = 0x8100;
	v_info.inner_tpid = 0x8100;
	
	/* skb original vlan */
	ecnt_mcst_get_skb_vlan(flow_idx,&skb_outer_vlan,&skb_inner_vlan,&vlan_tag_num);

	/* add two vlan */
	if ((mtk_mcts_is_valid_vid(inner_vid)) 
		&& (mtk_mcts_is_valid_vid(outer_vid)))
	{
		*o_vlan = ((v_info.outer_tpid << 16) | v_info.outer_tci);
		*i_vlan = ((v_info.inner_tpid << 16) | v_info.inner_tci);
	}
	else if (mtk_mcts_is_valid_vid(inner_vid))
	{
		*o_vlan = ((v_info.inner_tpid << 16) | v_info.inner_tci);
		*i_vlan = skb_outer_vlan;
	}
	else if (mtk_mcts_is_valid_vid(outer_vid))
	{
		*o_vlan = ((v_info.outer_tpid << 16) | v_info.outer_tci);
		*i_vlan = skb_outer_vlan;
	}
}

void ecnt_mcst_vlan_add_and_remark(int flow_idx,struct vlan_info v_info,int *o_vlan,int *i_vlan)
{
	int outer_vid = 0;
	int inner_vid = 0;
	int outer_pbit = 0;
	int inner_pbit = 0;
	
	int vlan_tag_num = 0;
	int skb_outer_vlan = 0;
	int skb_inner_vlan = 0;

	//only test
	v_info.outer_tpid = 0x8100;
	v_info.inner_tpid = 0x8100;
	
	/* fwd table entry vlan */
	outer_vid = (v_info.outer_tci & 0xfff);
	inner_vid = (v_info.inner_tci & 0xfff);
	outer_pbit = (v_info.outer_tci >> 13);
	inner_pbit = (v_info.inner_tci >> 13);

	/* skb original vlan */
	ecnt_mcst_get_skb_vlan(flow_idx,&skb_outer_vlan,&skb_inner_vlan,&vlan_tag_num);

	/* add two vlan */
	if ((mtk_mcts_is_valid_vid(inner_vid)) 
		&& (mtk_mcts_is_valid_vid(outer_vid)) 
		&& (mtk_mcts_is_valid_pbit(inner_pbit)) 
		&& (mtk_mcts_is_valid_pbit(outer_pbit)))
	{
		*o_vlan = ((v_info.outer_tpid << 16) | v_info.outer_tci);
		*i_vlan = ((v_info.inner_tpid << 16) | v_info.inner_tci);
	}
	else if ((mtk_mcts_is_valid_vid(inner_vid)) 
		&& (mtk_mcts_is_valid_pbit(inner_pbit)))
	{
		*o_vlan = ((v_info.inner_tpid << 16) | v_info.inner_tci);
		*i_vlan = skb_outer_vlan;
	}
	else if ((mtk_mcts_is_valid_vid(outer_vid)) 
		&& (mtk_mcts_is_valid_pbit(outer_pbit)))
	{
		*o_vlan = ((v_info.outer_tpid << 16) | v_info.outer_tci);
		*i_vlan = skb_outer_vlan;
	}
}

void ecnt_mcst_vlan_strip(int flow_idx,int *o_vlan,int *i_vlan)
{
	int vlan_tag_num = 0;
	int skb_outer_vlan = 0;
	int skb_inner_vlan = 0;

	/* skb original vlan */
	ecnt_mcst_get_skb_vlan(flow_idx,&skb_outer_vlan,&skb_inner_vlan,&vlan_tag_num);

	if (1 == vlan_tag_num)
	{
		*o_vlan = 0;
		*i_vlan = 0;
	}
	else if (2 == vlan_tag_num)
	{
		/* now the inner vlan becomes the outer */
		*o_vlan = skb_inner_vlan;
		*i_vlan = 0;
	}
}

void ecnt_mcst_do_vlan(int flow_idx,uint32 port_mask)
{
	int o_vlan = 0;
	int i_vlan = 0;
	int vlan_tag_num = 0;
	struct vlan_info v_info;
	
	if (!ecnt_get_entry_vlan_action(port_mask,&v_info))
	{
		return;
	}

	/* vlan handle */
	switch (v_info.vlan_mode)
	{
		case MCST_VLAN_TRANSPARENT:
			ecnt_mcst_get_skb_vlan(flow_idx,&o_vlan,&i_vlan,&vlan_tag_num);
			break;
		case MCST_VLAN_REPLACE:
			ecnt_mcst_vlan_replace(flow_idx,v_info,&o_vlan,&i_vlan);
			break;
		case MCST_VLAN_REPLACE_AND_REMARK:
			ecnt_mcst_vlan_replace_and_remark(flow_idx,v_info,&o_vlan,&i_vlan);
			break;
		case MCST_VLAN_ADD:
			ecnt_mcst_vlan_add(flow_idx,v_info,&o_vlan,&i_vlan);
			break;
		case MCST_VLAN_ADD_AND_REMARK:
			ecnt_mcst_vlan_add_and_remark(flow_idx,v_info,&o_vlan,&i_vlan);
			break;
		case MCST_VLAN_STRIP:
			ecnt_mcst_vlan_strip(flow_idx,&o_vlan,&i_vlan);
			break;
		default:
			return;
	}

	/* update hwnat entry vlan */
	if (ra_sw_nat_hook_update_vlan)
	{
		ra_sw_nat_hook_update_vlan(flow_idx,o_vlan,i_vlan);
	}
}

void ecnt_mcst_hwnat_update_dp(int index,int dp)
{
	if (ra_sw_nat_hook_update_dp)
	{
		ra_sw_nat_hook_update_dp(index,dp);
	}
}

void ecnt_mcst_hwnat_update_vlan(int flow_idx)
{
	MTK_IGMP_HWNATEntry_t *tmp = NULL;
	MTK_IGMP_HWNATEntry_t *entry = NULL;

	spin_lock_bh(&mtk_hwnat_mcst_lock);
	list_for_each_entry_safe(entry,tmp,&mtk_hwnat_igmp,list)
	{
		if (entry->index != flow_idx)
		{
			continue;
		}

		/* find the flow that update the vlan info */
		if (ra_sw_nat_hook_update_vlan)
		{
			ra_sw_nat_hook_update_vlan(flow_idx,entry->outer_vlan,entry->inner_vlan);
		}
	}
	spin_unlock_bh(&mtk_hwnat_mcst_lock);
}

void ecnt_mcst_hwnat_add_drop(MTK_IGMP_HWNATEntry_t *entry)
{
	MTK_IGMP_HWNATEntry_t *drop_entry = NULL;
	
	if (NULL == entry)
	{
		return;
	}

	drop_entry = (MTK_IGMP_HWNATEntry_t* )mtk_xpon_alloc(sizeof(MTK_IGMP_HWNATEntry_t));
	if (NULL == drop_entry)
	{
		return;
	}

	spin_lock_bh(&mtk_hwnat_drop_mcst_lock);

	/* init the drop entry */
	drop_entry->proto = entry->proto;
	drop_entry->hwnat_vid= entry->hwnat_vid;
	drop_entry->index = entry->index;
	drop_entry->mask = entry->mask;
	memcpy(drop_entry->grp_addr,entry->grp_addr,16);
	memcpy(drop_entry->src_addr,entry->src_addr,16);

	/* add into the drop list */
	list_add_tail(&drop_entry->list, &mtk_hwnat_drop);
	setup_timer(&drop_entry->age_timer,mtk_xpon_hwnat_drop_timeout,(unsigned long)drop_entry);	
	mod_timer(&drop_entry->age_timer,round_jiffies(jiffies)+XPON_HWNAT_AGE_TIME);
	
	spin_unlock_bh(&mtk_hwnat_drop_mcst_lock);
}

void ecnt_update_hwnat_entry(int flow_idx,uint32 port_mask)
{
	uint32 switch_mask = 0;
	MTK_IGMP_HWNATEntry_t *entry = NULL;
	MTK_IGMP_HWNATEntry_t *tmp   = NULL;

	switch_mask = mtk_calc_port_mask(port_mask);
	spin_lock_bh(&mtk_hwnat_mcst_lock);
	list_for_each_entry_safe(entry,tmp,&mtk_hwnat_igmp,list)
	{
		if (entry->index == flow_idx)
		{
			entry->mask = (switch_mask | (port_mask & (~(uint32)0xff)));
			MULTI_LOG(MULTI_LOG_ERROR,"switch_mask = 0x%x, entry->mask = 0x%x\n",switch_mask,entry->mask);
			
			if (0 == entry->mask)
			{
				/* sent to drop DP */
				ecnt_mcst_hwnat_update_dp(flow_idx,DP_DROP);

				/* firstly add the entry into drop list */
				ecnt_mcst_hwnat_add_drop(entry);
				
				/* then delete the entry from fwd list */
				mtk_xpon_hwnat_delete_entry(entry);
			}
			else
			{
				mtk_xpon_hwnat_update_flow(entry,entry->mask);
			}
		}
	}
	spin_unlock_bh(&mtk_hwnat_mcst_lock);
}

void igmp_entry_check_vlan_mode(mc_fwd_tbl_entry *pdata,uint32 port_mask,int flow_idx)
{
	/* check fwd table whether vlan 1:N */
	if (ecnt_vlan_IsOneToN(port_mask,NULL,pdata,-1))
	{
		/* restore original vlan info */
		ecnt_mcst_hwnat_update_vlan(flow_idx);
		
		ecnt_mcst_hwnat_update_dp(flow_idx,DP_QDMA1_SW);
	}
	else
	{
		if (0 == port_mask)
		{
			return;
		}
		
		/* restore expected vlan info */
		ecnt_mcst_do_vlan(flow_idx,port_mask);
		
		ecnt_mcst_hwnat_update_dp(flow_idx,DP_GDMA1);	
	}
}

void igmp_del_entry(MC_FwdEntry *entry)
{
	list_del(&entry->list);
	mtk_xpon_free(entry);
}

static inline void igmp_get_portmask_by_ip_vid(MTK_IP_ADDRESS grp_ip,short int vid,uint32* fwd_port_mask,int index)
{
	struct list_head *fwd_list = igmp_get_fdb_list();
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	bool isIPv6 = 0;
	
	if (list_empty(fwd_list))
	{
		return;
	}
	
	isIPv6 = grp_ip.is_ipv6;
	if (!isIPv6)
	{
		spin_lock_bh(&ecnt_fwd_tbl_lock);
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->vlan_id == vid) 
				&& (!memcmp(&(entry->group_addr.IP.ipv4_address),&(grp_ip.IP.ipv4_address),4)))
			{
				*fwd_port_mask |= (1 << (entry->port_id));
				entry->flow_idx = index;
			}
		}
		spin_unlock_bh(&ecnt_fwd_tbl_lock);
	}
	else
	{
		spin_lock_bh(&ecnt_fwd_tbl_lock);
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->vlan_id == vid) 
				&& (!memcmp(entry->group_addr.IP.ipv6_address.ip_address_array,
				grp_ip.IP.ipv6_address.ip_address_array,16)))
			{
				*fwd_port_mask |= (1 << (entry->port_id));
				entry->flow_idx = index;
			}
		}
		spin_unlock_bh(&ecnt_fwd_tbl_lock);
	}
}

static inline void igmp_get_portmask_by_mac(unsigned char *grp_mac,uint32* fwd_port_mask,int index)
{
	struct list_head *fwd_list = igmp_get_fdb_list();
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	
	if (list_empty(fwd_list))
	{
		return;
	}
	
	spin_lock_bh(&ecnt_fwd_tbl_lock);
	list_for_each_entry_safe(entry,tmp,fwd_list,list)
	{
		if (!memcmp(entry->group_mac.mac_address_byte,grp_mac,ETH_ALEN))
		{
			*fwd_port_mask |= (1 << (entry->port_id));
			entry->flow_idx = index;
		}
	}
	spin_unlock_bh(&ecnt_fwd_tbl_lock);
}

static inline void igmp_get_portmask_by_mac_vid(unsigned char *grp_mac,short int vid,uint32* fwd_port_mask,int index)
{
	struct list_head *fwd_list = igmp_get_fdb_list();
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	
	if (list_empty(fwd_list))
	{
		return;
	}
	
	spin_lock_bh(&ecnt_fwd_tbl_lock);
	list_for_each_entry_safe(entry,tmp,fwd_list,list)
	{
		if ((!memcmp(entry->group_mac.mac_address_byte,grp_mac,ETH_ALEN)) 
			&& (entry->vlan_id == vid))
		{
			*fwd_port_mask |= (1 << (entry->port_id));
			entry->flow_idx = index;
		}
	}
	spin_unlock_bh(&ecnt_fwd_tbl_lock);
}

static inline void igmp_get_portmask_by_mac_ip(unsigned char *grp_mac,MTK_IP_ADDRESS grp_ip,uint32* fwd_port_mask,int index)
{
	struct list_head *fwd_list = igmp_get_fdb_list();
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	bool isIPv6 = 0;
	
	if (list_empty(fwd_list))
	{
		return;
	}
	
	isIPv6 = grp_ip.is_ipv6;
	if (!isIPv6)
	{
		spin_lock_bh(&ecnt_fwd_tbl_lock);
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((!memcmp(entry->group_mac.mac_address_byte,grp_mac,ETH_ALEN)) 
				&& (!memcmp(&(entry->group_addr.IP.ipv4_address),&(grp_ip.IP.ipv4_address),4)))
			{
				*fwd_port_mask |= (1 << (entry->port_id));
				entry->flow_idx = index;
			}
		}
		spin_unlock_bh(&ecnt_fwd_tbl_lock);
	}
	else
	{
		spin_lock_bh(&ecnt_fwd_tbl_lock);
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((!memcmp(entry->group_mac.mac_address_byte,grp_mac,ETH_ALEN)) 
				&& (!memcmp(entry->group_addr.IP.ipv6_address.ip_address_array,
				grp_ip.IP.ipv6_address.ip_address_array,16)))
			{
				*fwd_port_mask |= (1 << (entry->port_id));
				entry->flow_idx = index;
			}
		}
		spin_unlock_bh(&ecnt_fwd_tbl_lock);
	}
}

static inline void igmp_get_portmask_by_all(unsigned char *grp_mac,MTK_IP_ADDRESS grp_ip,short int vid,uint32* fwd_port_mask,int index)
{
	struct list_head *fwd_list = igmp_get_fdb_list();
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	bool isIPv6 = 0;
	
	if (list_empty(fwd_list))
	{
		return;
	}
	
	isIPv6 = grp_ip.is_ipv6;
	if (!isIPv6)
	{
		spin_lock_bh(&ecnt_fwd_tbl_lock);
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->vlan_id == vid) 
				&& (!memcmp(entry->group_mac.mac_address_byte,grp_mac,ETH_ALEN)) 
				&& (!memcmp(&(entry->group_addr.IP.ipv4_address),&(grp_ip.IP.ipv4_address),4)))
			{
				*fwd_port_mask |= (1 << (entry->port_id));
				entry->flow_idx = index;
			}
		}
		spin_unlock_bh(&ecnt_fwd_tbl_lock);
	}
	else
	{
		spin_lock_bh(&ecnt_fwd_tbl_lock);
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->vlan_id == vid) 
				&& (!memcmp(entry->group_mac.mac_address_byte,grp_mac,ETH_ALEN)) 
				&& (!memcmp(entry->group_addr.IP.ipv6_address.ip_address_array,
				grp_ip.IP.ipv6_address.ip_address_array,16)))
			{
				*fwd_port_mask |= (1 << (entry->port_id));
				entry->flow_idx = index;
			}
		}
		spin_unlock_bh(&ecnt_fwd_tbl_lock);
	}
}

void igmp_get_fwd_portmask(struct sk_buff* skb,uint32* fwd_port_mask)
{
	unsigned char grp_mac[6] = {0};
	MTK_IP_ADDRESS grp_ip;
	short int vid = -1;
	int fwd_mode = -1;
	int i = 0;
	int index = -1;

	/* get group mac */
	memcpy(grp_mac,eth_hdr(skb)->h_dest,ETH_ALEN);

	/* get group ip */
	memset(&grp_ip,0,sizeof(MTK_IP_ADDRESS));
	mtk_xpon_get_src_and_dst_addr(NULL,&grp_ip,skb);

	/* get VLAN ID */
	vid = mtk_xpon_get_vlan_id(skb);

	/* get flow index */
	index = mtk_xpon_hwnat_flow_index(skb);
	
	/* debug log */
	MULTI_LOG(MULTI_LOG_DBG,"vid = %d\n",vid);
	MULTI_LOG(MULTI_LOG_DBG,"grp_mac = %2x:%2x:%2x:%2x:%2x:%2x\n",
	grp_mac[0],grp_mac[1],grp_mac[2],grp_mac[3],grp_mac[4],grp_mac[5]);
	MULTI_LOG(MULTI_LOG_DBG,"grp_ip.isipv6 = %d\n",grp_ip.is_ipv6);
	if (grp_ip.is_ipv6)
	{
		for(i = 0;i < 16;i++)
		{
			MULTI_LOG(MULTI_LOG_DBG,"%x ",grp_ip.IP.ipv6_address.ip_address_array[i]);
		}
	}
	else
	{
		MULTI_LOG(MULTI_LOG_DBG,"0x%x ",grp_ip.IP.ipv4_address);
	}
	MULTI_LOG(MULTI_LOG_DBG,"flow index = %d\n",index);
	MULTI_LOG(MULTI_LOG_DBG,"\n");

	/* find the port_mask according to the fwd mode */
	fwd_mode = ecnt_igmp_get_filterMode();
	switch (fwd_mode)
	{
		case FWD_MODE_IP_AND_VID:
			igmp_get_portmask_by_ip_vid(grp_ip,vid,fwd_port_mask,index);
			break;
		case FWD_MODE_MAC:
			igmp_get_portmask_by_mac(grp_mac,fwd_port_mask,index);
			break;
		case FWD_MODE_MAC_AND_IP:
			igmp_get_portmask_by_mac_ip(grp_mac,grp_ip,fwd_port_mask,index);
			break;
		case FWD_MODE_MAC_AND_IP_AND_VID:
			igmp_get_portmask_by_all(grp_mac,grp_ip,vid,fwd_port_mask,index);
			break;
		case FWD_MODE_MAC_AND_VID:
			igmp_get_portmask_by_mac_vid(grp_mac,vid,fwd_port_mask,index);
			break;
		default:
			break;
	}
}


/*___________________________________________________________________________
**      function name: ecnt_get_portmask_by_xx
**      descriptions:
**      	get current multicast data port mask, means which LAN ports joined this multicast
**          and these function are only used by ecnt_mcst_get_fwd_mask.
**      input parameters:
**		grp_ip: current multicast data group ip
**		vid:	    current multicast data vlan id
**          index:  current multicast data hw_nat index
**      output parameters:
**      	fwd_port_mask: port mask
**
**      return:
**      	void
**___________________________________________________________________________
*/
static inline void ecnt_get_portmask_by_ip_vid(MTK_IP_ADDRESS grp_ip,short int vid,uint32* fwd_port_mask,int index)
{
	struct list_head *fwd_list = igmp_get_fdb_list();
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	bool isIPv6 = 0;
	unsigned long lock_flag;
	
	if (list_empty(fwd_list))
	{
		return;
	}
	
	isIPv6 = grp_ip.is_ipv6;
	if (!isIPv6)
	{
		spin_lock_irqsave(&ecnt_fwd_tbl_lock,lock_flag);
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->vlan_id == vid) 
				&& (!memcmp(&(entry->group_addr.IP.ipv4_address),&(grp_ip.IP.ipv4_address),4)))
			{
				*fwd_port_mask |= (1 << (entry->port_id));
				entry->flow_idx = index;
			}
		}
		spin_unlock_irqrestore(&ecnt_fwd_tbl_lock,lock_flag);
	}
	else
	{
		spin_lock_irqsave(&ecnt_fwd_tbl_lock,lock_flag);
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->vlan_id == vid) 
				&& (!memcmp(entry->group_addr.IP.ipv6_address.ip_address_array,
				grp_ip.IP.ipv6_address.ip_address_array,16)))
			{
				*fwd_port_mask |= (1 << (entry->port_id));
				entry->flow_idx = index;
			}
		}
		spin_unlock_irqrestore(&ecnt_fwd_tbl_lock,lock_flag);
	}
}

static inline void ecnt_get_portmask_by_mac(unsigned char *grp_mac,uint32* fwd_port_mask,int index)
{
	struct list_head *fwd_list = igmp_get_fdb_list();
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	unsigned long lock_flag;
	
	if (list_empty(fwd_list))
	{
		return;
	}

	spin_lock_irqsave(&ecnt_fwd_tbl_lock,lock_flag);
	list_for_each_entry_safe(entry,tmp,fwd_list,list)
	{
		if (!memcmp(entry->group_mac.mac_address_byte,grp_mac,ETH_ALEN))
		{
			*fwd_port_mask |= (1 << (entry->port_id));
			entry->flow_idx = index;
		}
	}
	spin_unlock_irqrestore(&ecnt_fwd_tbl_lock,lock_flag);
}

static inline void ecnt_get_portmask_by_mac_vid(unsigned char *grp_mac,short int vid,uint32* fwd_port_mask,int index)
{
	struct list_head *fwd_list = igmp_get_fdb_list();
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	unsigned long lock_flag;
	
	if (list_empty(fwd_list))
	{
		return;
	}

	spin_lock_irqsave(&ecnt_fwd_tbl_lock,lock_flag);
	list_for_each_entry_safe(entry,tmp,fwd_list,list)
	{
		if ((!memcmp(entry->group_mac.mac_address_byte,grp_mac,ETH_ALEN)) 
			&& (entry->vlan_id == vid))
		{
			*fwd_port_mask |= (1 << (entry->port_id));
			entry->flow_idx = index;
		}
	}
	spin_unlock_irqrestore(&ecnt_fwd_tbl_lock,lock_flag);
}

static inline void ecnt_get_portmask_by_mac_ip(unsigned char *grp_mac,MTK_IP_ADDRESS grp_ip,uint32* fwd_port_mask,int index)
{
	struct list_head *fwd_list = igmp_get_fdb_list();
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	bool isIPv6 = 0;
	unsigned long lock_flag;
	
	if (list_empty(fwd_list))
	{
		return;
	}
	
	isIPv6 = grp_ip.is_ipv6;
	if (!isIPv6)
	{
		spin_lock_irqsave(&ecnt_fwd_tbl_lock,lock_flag);
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((!memcmp(entry->group_mac.mac_address_byte,grp_mac,ETH_ALEN)) 
				&& (!memcmp(&(entry->group_addr.IP.ipv4_address),&(grp_ip.IP.ipv4_address),4)))
			{
				*fwd_port_mask |= (1 << (entry->port_id));
				entry->flow_idx = index;
			}
		}
		spin_unlock_irqrestore(&ecnt_fwd_tbl_lock,lock_flag);
	}
	else
	{
		spin_lock_irqsave(&ecnt_fwd_tbl_lock,lock_flag);
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((!memcmp(entry->group_mac.mac_address_byte,grp_mac,ETH_ALEN)) 
				&& (!memcmp(entry->group_addr.IP.ipv6_address.ip_address_array,
				grp_ip.IP.ipv6_address.ip_address_array,16)))
			{
				*fwd_port_mask |= (1 << (entry->port_id));
				entry->flow_idx = index;
			}
		}
		spin_unlock_irqrestore(&ecnt_fwd_tbl_lock,lock_flag);
	}
}

static inline void ecnt_get_portmask_by_all(unsigned char *grp_mac,MTK_IP_ADDRESS grp_ip,short int vid,uint32* fwd_port_mask,int index)
{
	struct list_head *fwd_list = igmp_get_fdb_list();
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	bool isIPv6 = 0;
	unsigned long lock_flag;
	
	if (list_empty(fwd_list))
	{
		return;
	}
	
	isIPv6 = grp_ip.is_ipv6;
	if (!isIPv6)
	{
		spin_lock_irqsave(&ecnt_fwd_tbl_lock,lock_flag);
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->vlan_id == vid) 
				&& (!memcmp(entry->group_mac.mac_address_byte,grp_mac,ETH_ALEN)) 
				&& (!memcmp(&(entry->group_addr.IP.ipv4_address),&(grp_ip.IP.ipv4_address),4)))
			{
				*fwd_port_mask |= (1 << (entry->port_id));
				entry->flow_idx = index;
			}
		}
		spin_unlock_irqrestore(&ecnt_fwd_tbl_lock,lock_flag);
	}
	else
	{
		spin_lock_irqsave(&ecnt_fwd_tbl_lock,lock_flag);
		list_for_each_entry_safe(entry,tmp,fwd_list,list)
		{
			if ((entry->vlan_id == vid) 
				&& (!memcmp(entry->group_mac.mac_address_byte,grp_mac,ETH_ALEN)) 
				&& (!memcmp(entry->group_addr.IP.ipv6_address.ip_address_array,
				grp_ip.IP.ipv6_address.ip_address_array,16)))
			{
				*fwd_port_mask |= (1 << (entry->port_id));
				entry->flow_idx = index;
			}
		}
		spin_unlock_irqrestore(&ecnt_fwd_tbl_lock,lock_flag);
	}
}

void ecnt_mcst_get_fwd_mask(struct sk_buff *skb,int vid,uint32 *fwd_port_mask)
{
	unsigned char grp_mac[6] = {0};
	MTK_IP_ADDRESS grp_ip;
	int fwd_mode = -1;
	int i = 0;
	int index = -1;
	int outer_vlan = 0;
	int inner_vlan = 0;
	int vlan_tag_num = 0;

	/* get group mac */
	memcpy(grp_mac,eth_hdr(skb)->h_dest,ETH_ALEN);

	/* get group ip */
	memset(&grp_ip,0,sizeof(MTK_IP_ADDRESS));
	mtk_xpon_get_src_and_dst_addr(NULL,&grp_ip,skb);

	/* get flow index */
	index = mtk_xpon_hwnat_flow_index(skb);
	
	/* debug log */
	MULTI_LOG(MULTI_LOG_DBG,"vid = %d\n",vid);
	MULTI_LOG(MULTI_LOG_DBG,"grp_mac = %2x:%2x:%2x:%2x:%2x:%2x\n",
	grp_mac[0],grp_mac[1],grp_mac[2],grp_mac[3],grp_mac[4],grp_mac[5]);
	MULTI_LOG(MULTI_LOG_DBG,"grp_ip.isipv6 = %d\n",grp_ip.is_ipv6);
	if (grp_ip.is_ipv6)
	{
		for(i = 0;i < 16;i++)
		{
			MULTI_LOG(MULTI_LOG_DBG,"%x ",grp_ip.IP.ipv6_address.ip_address_array[i]);
		}
	}
	else
	{
		MULTI_LOG(MULTI_LOG_DBG,"0x%x ",grp_ip.IP.ipv4_address);
	}
	MULTI_LOG(MULTI_LOG_DBG,"flow index = %d\n",index);
	MULTI_LOG(MULTI_LOG_DBG,"\n");

	/* find the port_mask according to the fwd mode */
	fwd_mode = ecnt_igmp_get_filterMode();
	switch (fwd_mode)
	{
		case FWD_MODE_IP_AND_VID:
			ecnt_get_portmask_by_ip_vid(grp_ip,vid,fwd_port_mask,index);
			break;
		case FWD_MODE_MAC:
			ecnt_get_portmask_by_mac(grp_mac,fwd_port_mask,index);
			break;
		case FWD_MODE_MAC_AND_IP:
			ecnt_get_portmask_by_mac_ip(grp_mac,grp_ip,fwd_port_mask,index);
			break;
		case FWD_MODE_MAC_AND_IP_AND_VID:
			ecnt_get_portmask_by_all(grp_mac,grp_ip,vid,fwd_port_mask,index);
			break;
		case FWD_MODE_MAC_AND_VID:
			ecnt_get_portmask_by_mac_vid(grp_mac,vid,fwd_port_mask,index);
			break;
		default:
			break;
	}
}

static inline int igmp_get_downstream_vlan_action(uint8 port_id,struct sk_buff* skb,
	mc_fwd_tbl_entry *data,int vlanid,mc_fwd_tbl_entry *pEntry)
{
	unsigned char grp_mac[6] = {0};
	MTK_IP_ADDRESS grp_ip;
	short int vid = -1;
	
	MC_FwdEntry *entry = NULL;
	mc_fwd_tbl_entry tmp_entry;
	
	struct list_head *fwd_list = igmp_get_fdb_list();
	
	if ((list_empty(fwd_list)) || (NULL == pEntry))
	{
		return -1;
	}

	if (skb != NULL)
	{
		/* get current data packet group mac */
		memcpy(grp_mac,eth_hdr(skb)->h_dest,ETH_ALEN);

		/* get current data packet group ip */
		memset(&grp_ip,0,sizeof(MTK_IP_ADDRESS));
		mtk_xpon_get_src_and_dst_addr(NULL,&grp_ip,skb);

		/* get current data packet VLAN ID */
		if (vlanid > 0)
		{
			vid = vlanid;
		}
		else
		{
			vid = mtk_xpon_get_vlan_id(skb);
		}
	}
	else if (data != NULL)
	{
		vid = data->vlan_id;
		memcpy(grp_mac,data->group_mac.mac_address_byte,ETH_ALEN);
		if (!data->group_addr.is_ipv6)
		{
			memcpy(&grp_ip.IP.ipv4_address,&data->group_addr.IP.ipv4_address,4);
		}
		else
		{
			memcpy(grp_ip.IP.ipv6_address.ip_address_array,
				data->group_addr.IP.ipv6_address.ip_address_array,16);
		}
	}

	/* set the entry info */
	memset(&tmp_entry,0,sizeof(mc_fwd_tbl_entry));
	tmp_entry.port_id = port_id;
	tmp_entry.vlan_id = vid;
	memcpy(tmp_entry.group_mac.mac_address_byte,grp_mac,6);
	if (!grp_ip.is_ipv6)
	{
		memcpy(&tmp_entry.group_addr.IP.ipv4_address,&grp_ip.IP.ipv4_address,4);
	}
	else
	{
		memcpy(tmp_entry.group_addr.IP.ipv6_address.ip_address_array,
			grp_ip.IP.ipv6_address.ip_address_array,16);
	}
	
	/* search the entry */
	spin_lock_bh(&ecnt_fwd_tbl_lock);
	entry = igmp_find_entry(&tmp_entry);
	if (entry != NULL)
	{
		/* get the vlan information */;
		pEntry->vlan_action.vlan_mode = entry->vlan_action.vlan_mode;
		pEntry->vlan_action.outer_tpid = entry->vlan_action.outer_tpid;
		pEntry->vlan_action.outer_tci = entry->vlan_action.outer_tci;
		pEntry->vlan_action.inner_tpid = entry->vlan_action.inner_tpid;
		pEntry->vlan_action.inner_tci = entry->vlan_action.inner_tci;

		spin_unlock_bh(&ecnt_fwd_tbl_lock);
		return 1;
	}
	spin_unlock_bh(&ecnt_fwd_tbl_lock);
	
	return -1;
}

static int ecnt_igmp_add_entry(mc_fwd_tbl_entry *pdata)
{
	int exist_flag = 0;
	int flow_idx = -1;
	uint32 port_mask = 0;
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	struct list_head *fwd_list = igmp_get_fdb_list();
	
	if ((NULL == pdata) || (NULL == fwd_list))
	{
		return -1;
	}
	
	/* alloc new entry */
	entry = (MC_FwdEntry*)mtk_xpon_alloc(sizeof(MC_FwdEntry));
	if (NULL == entry)
	{
		return -1;
	}

	/* init new entry */
	igmp_set_entry(entry,pdata);
	
	spin_lock_bh(&ecnt_fwd_tbl_lock);
	/* join into the same program */
	exist_flag = igmp_is_programme_exist(pdata,&flow_idx,&port_mask,IGMP_ADD_ENTRY);

	/* find the entry */
	tmp = igmp_find_entry(pdata);
	if (NULL == tmp)
	{
		entry->flow_idx = flow_idx;
		
		/* add into the fwd table */
		list_add_tail(&entry->list,fwd_list);
	}
	else
	{
		/* update it */
		igmp_set_entry(tmp,pdata);

		/* free the new entry */
		mtk_xpon_free(entry);
	}
	spin_unlock_bh(&ecnt_fwd_tbl_lock);

	/* join to the same program */
	if (exist_flag)
	{
		MULTI_LOG(MULTI_LOG_ERROR,"flow_idx = %d, port_mask = 0x%x\n",flow_idx,port_mask);
		/* first check if vlan 1:N mode */
		igmp_entry_check_vlan_mode(pdata,port_mask,flow_idx);
		
		/* then update hw_nat mask */
		ecnt_update_hwnat_entry(flow_idx,port_mask);
	}
	else
	{
		mtk_add_iptv_table_entry();
	}
	
	return 0;
}

static int ecnt_igmp_del_entry(mc_fwd_tbl_entry *pdata)
{
	int exist_flag = 0;
	int flow_idx = -1;
	uint32 port_mask = 0;
	MC_FwdEntry *entry = NULL;

	if (NULL == pdata)
	{
		return -1;
	}
	
	spin_lock_bh(&ecnt_fwd_tbl_lock);
	entry = igmp_find_entry(pdata);
	if (NULL == entry)
	{
		spin_unlock_bh(&ecnt_fwd_tbl_lock);
		return 0;
	}

	/* get flow_idx and latest port mask */
	exist_flag = igmp_is_programme_exist(pdata,&flow_idx,&port_mask,IGMP_DEL_ENTRY);
	
	igmp_del_entry(entry);
	spin_unlock_bh(&ecnt_fwd_tbl_lock);

	if (!exist_flag)
	{
		return 0;
	}
	
	MULTI_LOG(MULTI_LOG_ERROR,"flow_idx = %d, port_mask = 0x%x\n",flow_idx,port_mask);
	
	/* first check if vlan 1:N mode */
	igmp_entry_check_vlan_mode(pdata,port_mask,flow_idx);

	/* update the hw_nat mask */
	ecnt_update_hwnat_entry(flow_idx,port_mask);
	
	return 0;
}

/* get all the entry info by port_id */
static int ecnt_igmp_get_entry(mc_fwd_entrys *pdata)
{
	int num = 0;
	MC_FwdEntry *entry = NULL;
	MC_FwdEntry *tmp = NULL;
	struct list_head *fwd_list = igmp_get_fdb_list();
		
	if ((list_empty(fwd_list)) || (NULL == pdata))
	{
		return -1;
	}
	
	/* find all the entrys according to port_id */
	spin_lock_bh(&ecnt_fwd_tbl_lock);
	list_for_each_entry_safe(entry,tmp,fwd_list,list)
	{
		if ((entry->port_id == pdata->port_id) 
			&& (num < (pdata->entry_num)))
		{
			igmp_get_entry(entry,&pdata->pEntry[num++]);
		}
	}
	spin_unlock_bh(&ecnt_fwd_tbl_lock);

	/* fill in the real number */
	pdata->entry_num = num;
	
	return num;
}

static int ecnt_igmp_update_entry(mc_fwd_tbl_entry *pdata)
{
	MC_FwdEntry *entry = NULL;
	
	if (NULL == pdata)
	{
		return -1;
	}

	/* firstly find the entry */
	spin_lock_bh(&ecnt_fwd_tbl_lock);
	entry = igmp_find_entry(pdata);
	if (NULL == entry)
	{
		spin_unlock_bh(&ecnt_fwd_tbl_lock);
		return -1;
	}

	/* then update the entry info */
	igmp_set_entry(entry,pdata);
	spin_unlock_bh(&ecnt_fwd_tbl_lock);
	
	mtk_update_iptv_table_entry();

	return 0;
}

static int ecnt_igmp_clear_entry(void)
{
	igmp_clear_entry();
	
	return 0;
}


/*********************************************************************************
 * ecnt igmp cdev implementation setction
 *
 *********************************************************************************/
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
long ecnt_igmp_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
#else
int ecnt_igmp_ioctl(struct inode *inode,struct file *filp,unsigned int cmd,unsigned long arg)
#endif
{
	int ret = 0;
	uint8 port_id = 0;
	int fwd_mode = -1;
	mc_fwd_tbl_entry temp;
	mc_fwd_entrys entrys;
	unsigned char snooping_enable = E_IGMP_SNOOPING_ON;
	
	memset(&temp,0,sizeof(temp));
	memset(&entrys,0,sizeof(entrys));
	
	switch (cmd)
	{
		case IGMP_ADD_ENTRY:
			copy_from_user(&temp,(mc_fwd_tbl_entry*)arg,sizeof(mc_fwd_tbl_entry));
			ret = ecnt_igmp_add_entry(&temp);
			break;
		case IGMP_DEL_ENTRY:
			copy_from_user(&temp,(mc_fwd_tbl_entry*)arg,sizeof(mc_fwd_tbl_entry));
			ret = ecnt_igmp_del_entry(&temp);
			break;
		case IGMP_GET_ENTRY:
			copy_from_user(&entrys,(mc_fwd_entrys*)arg,sizeof(mc_fwd_entrys));
			ret = ecnt_igmp_get_entry(&entrys);
			if (ret > 0)
			{
				copy_to_user((void* __user)arg,&entrys,sizeof(mc_fwd_entrys));
			}
			break;
		case IGMP_CLEAR_ENTRY:
			ret = ecnt_igmp_clear_entry();
			break;
		case IGMP_UPDATE_ENTRY:
			copy_from_user(&temp,(mc_fwd_tbl_entry*)arg,sizeof(mc_fwd_tbl_entry));
			ret = ecnt_igmp_update_entry(&temp);
			break;
		case IGMP_SET_FILTER_MODE:
			copy_from_user(&fwd_mode,(int*)arg,sizeof(int));
			ret = ecnt_igmp_set_filterMode((mc_fwd_mode)fwd_mode);
			break;
		case IGMP_GET_FILTER_MODE:
			ret = ecnt_igmp_get_filterMode();
			copy_to_user((void* __user)arg,&ret,sizeof(int));
			break;
		case IGMP_SET_IGMP_SNOOPING_ON_OFF:
			copy_from_user(&snooping_enable, (unsigned char*)arg, sizeof(unsigned char));
			ret = ecnt_igmp_set_snooping_on_off(snooping_enable);
            break;
		case IGMP_GET_IGMP_SNOOPING_ON_OFF:
			ret = ecnt_igmp_get_snooping_on_off(&snooping_enable);
			copy_to_user((void* __user)arg, &snooping_enable, sizeof(unsigned char));
			break;
		default:
			return -1;
	}
	
	return ret;
}

struct file_operations ecnt_igmp_fops = {
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
	.unlocked_ioctl	= ecnt_igmp_ioctl,
#else
	.ioctl = ecnt_igmp_ioctl,
#endif
};

void ecnt_igmp_cdev_init(void)
{
	int result = 0;;

	result = register_chrdev(ECNT_IGMP_MAJOR,ECNT_IGMP_DEV_NAME,&ecnt_igmp_fops);
	if (result < 0)
	{
		printk(KERN_ERR "%s: ecnt_igmp_cdev_init failed\n", __FILE__);
    }
}

inline char* get_dev_name_by_port(uint8 port_id)
{
	int i = 0;
	port_lan_name_map map_data[] = {
		{0, "eth0"},
		{1, "eth1"},
		{2, "eth2"},
		{3, "eth3"},
		{4, "ra0"},
		{5, "ra1"},
		{6, "ra2"},
		{7, "ra3"},
		{15, "pon"},};

	int array_size = (sizeof(map_data) / sizeof(map_data[0]));
	
	for (i = 0; i < array_size; i++)
	{
		if (port_id == map_data[i].port_id)
		{
			return map_data[i].name;
		}
	}

	return NULL;
}

inline int get_port_id_by_name(const char *name)
{
	int i = 0;
	port_lan_name_map map_data[] = {
		{0, "eth0"},
		{1, "eth1"},
		{2, "eth2"},
		{3, "eth3"},
		{4, "ra0"},
		{5, "ra1"},
		{6, "ra2"},
		{7, "ra3"},
		{15, "pon"},};

	int array_size = (sizeof(map_data) / sizeof(map_data[0]));

	if (NULL == name)
	{
		return -1;
	}

	for (i = 0; i < array_size; i++)
	{
		if (!strcmp(name, map_data[i].name))
		{
			return map_data[i].port_id;
		}
	}

	return -1;
}

static void* ecnt_get_transport_header(struct sk_buff *skb)
{
	struct iphdr *ih = NULL;
	struct ipv6hdr *i6h = NULL;
	unsigned int eth_type = mtk_xpon_get_packet_type(skb);
	unsigned char *buff = (unsigned char *)mtk_xpon_get_network_header(skb);
	
	if (PACKET_IPV4 == eth_type)
	{
		ih = (struct iphdr*)buff;
		return (buff + (ih->ihl*4));
	}
	else if (PACKET_IPV6 == eth_type)
	{
		i6h = (struct ipv6hdr*)buff;
		if (0 == i6h->nexthdr)
		{
			return buff + 48;
		}
		
		return buff + 40;
	}
	
	return NULL;
}

static int ecnt_get_igmp_type(struct sk_buff *skb)
{
	int type = mtk_xpon_get_ip_type(skb);
	unsigned char *buff = ecnt_get_transport_header(skb);

	if ((type == PROTOCOL_IGMP) || (type == PROTOCOL_ICMPV6))
	{
		return buff[0];
	}
	
	return 0;
}

static int ecnt_get_dest_addr(unsigned char *src,struct sk_buff *skb)
{
	struct iphdr *ih = NULL;
	struct ipv6hdr *i6h = NULL;
	unsigned char *pppoe_h = NULL;
	unsigned int eth_type = mtk_xpon_get_packet_type(skb);
	
	if (eth_type == PACKET_IPV4)
	{
		ih = (struct iphdr*)mtk_xpon_get_network_header(skb);
		memcpy(src,(unsigned char*)&ih->daddr,4);
		return 1;
	}
	else if (eth_type == PACKET_IPV6)
	{
		i6h =  (struct ipv6hdr*)mtk_xpon_get_network_header(skb);
		memcpy(src, i6h->daddr.s6_addr,16);
		return 1;
	}
#if !defined(TCSUPPORT_FWC_MCST)
	else if (eth_type == ETH_P_PPP_SES)
	{
		__be16 pppoe_proto;
	
		pppoe_h = (unsigned char *)mtk_xpon_get_network_header(skb);
		/* skip pppoe header */
		pppoe_h += sizeof(struct pppoe_hdr);
		pppoe_proto = *(__be16*)pppoe_h;

		/* skip ppp header */
		pppoe_h += 2;
		if (pppoe_proto == PPP_IP)
		{
			ih = (struct iphdr* )(pppoe_h);
			memcpy(src,(unsigned char*)&ih->daddr,4);
			return 1;
		}
		else if (pppoe_proto == PPP_IPV6)
		{
			i6h = (struct ipv6hdr*)(pppoe_h);
			memcpy(src, i6h->daddr.s6_addr,16);
			return 1;
		}
	}
#endif	
	return 0;
}

static int ecnt_is_igmp_pkt(struct sk_buff *skb)
{
	int type = mtk_xpon_get_ip_type(skb);
	int igmp_type = ecnt_get_igmp_type(skb);
	
	if (type == PROTOCOL_IGMP)
	{
		return 1;
	}
		
	if (type == PROTOCOL_ICMPV6)
	{
		if (igmp_type == ICMPV6_MGM_REPORT)
		{
			return 1;
		}
		else if (igmp_type == ICMPV6_MLD2_REPORT)
		{
			return 1;
		}
		else if (igmp_type == ICMPV6_MGM_QUERY)
		{
			return 1;
		}
		else if (igmp_type == ICMPV6_MGM_REDUCTION)
		{
			return 1;
		}
	}

	return 0;
}

static int ecnt_is_igmp_protocol_pkt(struct sk_buff *skb)
{
	unsigned char *dst = eth_hdr(skb)->h_dest;
	
    if (!mtk_xpon_is_multicast_addr(dst))
    {
        return 0;
    }

	if (!ecnt_is_igmp_pkt(skb))
	{
    	return 0;
	}

    return 1;
}

static int ecnt_is_query_pkt(struct sk_buff *skb)
{
	int igmp_type = -1;
	unsigned char dst_addr[16] = {0};

	igmp_type = ecnt_get_igmp_type(skb);

	if (IGMP_HOST_MEMBER_QUERY != igmp_type)
	{
		return ECNT_IGMP_NOT_QUERY;
	}
	
	if (ecnt_get_dest_addr(dst_addr,skb))
	{
		/* general query dst_addr=224.0.0.1 */
		if ((224 == dst_addr[0]) && (0 == dst_addr[1]) 
			&& (0 == dst_addr[2]) && (1 == dst_addr[3]))
		{
			return ECNT_IGMP_GENERAL_QUERY;
		}
		else
		{
			return ECNT_IGMP_SPECIFIC_QUERY;
		}
	}

	return ECNT_IGMP_NOT_QUERY;
}

inline int ecnt_is_mcst_data_pkt(struct sk_buff *skb)
{
	unsigned char *dst = eth_hdr(skb)->h_dest;

	if (!mtk_xpon_is_multicast_addr(dst))
	{
		return 0;
	}

	if (!mtk_xpon_is_data_pkt(skb))
	{
		return 0;
	}
	
	return 1;
}

inline void ecnt_remove_stag(struct sk_buff *skb)
{
	uint8 vpm = 0;
	uint16 stag = 0;
	uint8 *cp = NULL;

	/* get stag */
	cp = skb->data;
	stag = *(uint16 *)(cp + 12);

	/* get vpm */
	vpm = ((stag & 0x300) >> 8);
	switch (vpm)
	{
		case 0:/*untag*/
			memmove(cp + 4,cp,12);
			skb_pull(skb,4);
			skb->mac_header += 4;
			break;
		case 1:/*8100*/
			*(uint16 *)(cp + 12) = 0x8100;
			break;
		case 2:/*88a8*/
			*(uint16 *)(cp + 12) = 0x88a8;
			break;
		case 3:/*defined by register*/
			break;
		default:
			return;	
	}
}

/*___________________________________________________________________________
**      function name: ecnt_get_skb_original_vid
**      descriptions:
**      	get skb data original vlan id
**          and this function is only used by ecnt_mc_hook_vlan_isOneToN
**      input parameters:
**		skb: multicast data
**      output parameters:
**      	N/A
**
**      return:
**      	vlan id
**___________________________________________________________________________
*/
int ecnt_get_skb_original_vid(struct sk_buff *skb)
{
	int index = -1;
	int outer_vlan = 0;
	unsigned long lock_flag;
	
	MTK_IGMP_HWNATEntry_t *tmp = NULL;
	MTK_IGMP_HWNATEntry_t *entry = NULL;
	
	/* get flow index */
	index = mtk_xpon_hwnat_flow_index(skb);

	if (index < 0)
	{
		MULTI_LOG(MULTI_LOG_ERROR,"index is not correct\n");
		return 0;
	}
	
	/* get original vid */
	spin_lock_irqsave(&mtk_hwnat_mcst_lock,lock_flag);
	list_for_each_entry_safe(entry,tmp,&mtk_hwnat_igmp,list)
	{
		if (entry->index == index)
		{
			outer_vlan = entry->outer_vlan;
		}
	}
	spin_unlock_irqrestore(&mtk_hwnat_mcst_lock,lock_flag);


	return (outer_vlan & 0xfff);
}


/*___________________________________________________________________________
**      function name: ecnt_mcst_update_vlan
**      descriptions:
**      	update the skb vlan information
**          and this function is only used by ecnt_mc_hook_vlan_isOneToN
**      input parameters:
**		skb: multicast data
**      output parameters:
**      	N/A
**
**      return:
**      	N/A
**___________________________________________________________________________
*/
void ecnt_mcst_update_vlan(struct sk_buff *skb)
{
	int index = -1;
	unsigned long lock_flag;
	MTK_IGMP_HWNATEntry_t *tmp = NULL;
	MTK_IGMP_HWNATEntry_t *entry = NULL;
	
	/* get flow index */
	index = mtk_xpon_hwnat_flow_index(skb);
	
	if (index < 0)
	{
		MULTI_LOG(MULTI_LOG_ERROR,"index is not correct\n");
		return;
	}

	spin_lock_irqsave(&mtk_hwnat_mcst_lock,lock_flag);
	list_for_each_entry_safe(entry,tmp,&mtk_hwnat_igmp,list)
	{
		if (entry->index != index)
		{
			continue;
		}

		/* find the flow that update the vlan info */
		if (ra_sw_nat_hook_update_vlan)
		{
			ra_sw_nat_hook_update_vlan(index,entry->outer_vlan,entry->inner_vlan);
		}
	}
	spin_unlock_irqrestore(&mtk_hwnat_mcst_lock,lock_flag);
}


/*___________________________________________________________________________
**      function name: ecnt_get_downstream_vlan_action
**      descriptions:
**      	this function is only used at vlan 1:N to get all the entry vlan action
**      input parameters:
**		port_id: port number example 0/1/2/3
**		skb: skb data
**          data: entry information
**		vlanid: vlan id
**      output parameters:
**      	pEntry: all the entry vlan information
**
**      return:
**      success:	1
**		failure:	-1
**___________________________________________________________________________
*/
static int ecnt_get_downstream_vlan_action(uint8 port_id,struct sk_buff* skb,
	mc_fwd_tbl_entry *data,int vlanid,mc_fwd_tbl_entry *pEntry)
{
	unsigned char grp_mac[6] = {0};
	MTK_IP_ADDRESS grp_ip;
	short int vid = -1;
	
	MC_FwdEntry *entry = NULL;
	mc_fwd_tbl_entry tmp_entry;
	
	struct list_head *fwd_list = igmp_get_fdb_list();
	
	if ((list_empty(fwd_list)) || (NULL == pEntry))
	{
		return -1;
	}

	if (skb != NULL)
	{
		/* get current data packet group mac */
		memcpy(grp_mac,eth_hdr(skb)->h_dest,ETH_ALEN);

		/* get current data packet group ip */
		memset(&grp_ip,0,sizeof(MTK_IP_ADDRESS));
		mtk_xpon_get_src_and_dst_addr(NULL,&grp_ip,skb);

		/* get current data packet VLAN ID */
		if (vlanid > 0)
		{
			vid = vlanid;
		}
		else
		{
			vid = mtk_xpon_get_vlan_id(skb);
		}
	}
	else if (data != NULL)
	{
		vid = data->vlan_id;
		memcpy(grp_mac,data->group_mac.mac_address_byte,ETH_ALEN);
		if (!data->group_addr.is_ipv6)
		{
			memcpy(&grp_ip.IP.ipv4_address,&data->group_addr.IP.ipv4_address,4);
		}
		else
		{
			memcpy(grp_ip.IP.ipv6_address.ip_address_array,
				data->group_addr.IP.ipv6_address.ip_address_array,16);
		}
	}

	/* set the entry info */
	memset(&tmp_entry,0,sizeof(mc_fwd_tbl_entry));
	tmp_entry.port_id = port_id;
	tmp_entry.vlan_id = vid;
	memcpy(tmp_entry.group_mac.mac_address_byte,grp_mac,6);
	if (!grp_ip.is_ipv6)
	{
		memcpy(&tmp_entry.group_addr.IP.ipv4_address,&grp_ip.IP.ipv4_address,4);
	}
	else
	{
		memcpy(tmp_entry.group_addr.IP.ipv6_address.ip_address_array,
			grp_ip.IP.ipv6_address.ip_address_array,16);
	}
	
	/* search the entry */
	spin_lock(&ecnt_fwd_tbl_lock);
	entry = igmp_find_entry(&tmp_entry);
	if (entry != NULL)
	{
		/* get the vlan information */;
		pEntry->vlan_action.vlan_mode = entry->vlan_action.vlan_mode;
		pEntry->vlan_action.outer_tpid = entry->vlan_action.outer_tpid;
		pEntry->vlan_action.outer_tci = entry->vlan_action.outer_tci;
		pEntry->vlan_action.inner_tpid = entry->vlan_action.inner_tpid;
		pEntry->vlan_action.inner_tci = entry->vlan_action.inner_tci;

		spin_unlock(&ecnt_fwd_tbl_lock);
		return 1;
	}
	spin_unlock(&ecnt_fwd_tbl_lock);
	
	return -1;
}

inline int ecnt_vlan_IsOneToN(uint32 port_mask,struct sk_buff *skb,mc_fwd_tbl_entry *data,int vid)
{
	uint8 idx = 0;
	uint8 i,j = 0;
	uint8 if_num = 0;
	mc_fwd_tbl_entry vlan_info[MAX_PORT_NUM];
	
	if (0 == port_mask)
	{
		MULTI_LOG(MULTI_LOG_ERROR,"port mask is 0...\n");
		return 0;
	}
	
	/* calculate the count */
	if_num = mtk_calc_lan_count(port_mask) + mtk_calc_wifi_count(port_mask);
	if (if_num < 2)
	{
		MULTI_LOG(MULTI_LOG_ERROR,"interface counter < 2...\n");
		return 0;
	}
	
	/* get the port vlan info */
	for (i = 0; i < MAX_PORT_NUM; i++)
	{
		if (0 == (port_mask & (1<<i)))
		{
			continue;
		}
		
		if (skb != NULL)
		{
			ecnt_get_downstream_vlan_action(i,skb,NULL,vid,&vlan_info[idx++]);
		}
		else if (data != NULL)
		{
			ecnt_get_downstream_vlan_action(i,NULL,data,vid,&vlan_info[idx++]);
		}
	}
	
	/* compare the vlan info */
	for (j = 0; j < (idx-1); j++)
	{
		if ((vlan_info[j].vlan_action.vlan_mode != vlan_info[j+1].vlan_action.vlan_mode) 
			|| (vlan_info[j].vlan_action.outer_tci != vlan_info[j+1].vlan_action.outer_tci) 
			|| (vlan_info[j].vlan_action.inner_tci != vlan_info[j+1].vlan_action.inner_tci))
		{
			return 1;
		}
	}

	return 0;
}

int ecnt_mcst_vlan_handle(struct sk_buff *skb,uint32 port_mask)
{
	int i = 0;
	int if_count = 0;
	char *dev_name = NULL;
	struct sk_buff *skb2 = NULL;
	struct net_device *dev = NULL;
	
	if_count = mtk_calc_lan_count(port_mask) + mtk_calc_wifi_count(port_mask);
		
	for (i = 0; i < MAX_PORT_NUM; i++)
	{	
		if (if_count <= 0)
		{
			return 0;
		}
		
		/* check port */
		if (0 == (port_mask & (1<<i)))
		{
			continue;
		}
		
		/* get dev name from port id */
		dev_name = get_dev_name_by_port(i);
		if (NULL == dev_name)
		{
			continue;
		}
		
		/* get dev from dev_name */
		dev = dev_get_by_name(&init_net,dev_name);
		if (NULL == dev)
		{
			continue;
		}

		/* get skb2 */
		if (if_count > 1)
		{
			skb2 = skb_copy(skb,GFP_ATOMIC);
		}
		else if (1 == if_count)
		{
			skb2 = skb;
		}
		
		if_count--;

		/* check skb2 */
		if (NULL == skb2)
		{
			continue;
		}

		/* data vlan handle */
		mtk_xpon_data_vlan_handle(skb2,i);

		/* start xmit */
		skb2->dev = dev;
		skb2->dev->netdev_ops->ndo_start_xmit(skb2,skb2->dev);
		dev_put(dev);
	}	
}

void ecnt_mcst_send_specific_query(struct sk_buff *skb)
{
	uint32 fwd_port_mask = 0;
	uint32 lan_port_mask = 0;
	uint32 wifi_port_mask = 0;
	
	igmp_get_fwd_portmask(skb,&fwd_port_mask);
	MULTI_LOG(MULTI_LOG_DBG,"before query : fwd_port_mask = 0x%x\n",fwd_port_mask);
	
	lan_port_mask = (fwd_port_mask & 0xf);
	wifi_port_mask = ((fwd_port_mask & (~(uint32)0xf)) << 4);
	fwd_port_mask = lan_port_mask | wifi_port_mask;
	MULTI_LOG(MULTI_LOG_DBG,"after query : fwd_port_mask = 0x%x\n",fwd_port_mask);
	
	if (fwd_port_mask != 0)
	{
		/* send to LAN interface directly */
		mtk_xpon_fwd_data_to_port(skb,fwd_port_mask);
	}
}

void ecnt_mcst_recv_ctrl_pkt(struct sk_buff *skb,uint8 query_type)
{
	uint8 i = 0;
	int port_number = -1;
	unsigned char snooping_enable = E_IGMP_SNOOPING_ON;
	
	port_number = get_port_id_by_name(skb->dev->name);
	if (port_number < 0)
	{
		MULTI_LOG(MULTI_LOG_DBG,"port id is error\n");
		return;
	}

	if (NULL == mc_ctrl_packet_rx_cb_func)
	{
		MULTI_LOG(MULTI_LOG_DBG,"mc_ctrl_packet_rx_cb_func is NULL\n");
		return;
	}
	
	/* recovery header */
	skb_push(skb,ETH_HLEN);
	
	ecnt_igmp_get_snooping_on_off(&snooping_enable);

	/* snooping off: retun after xmit */
	if (E_IGMP_SNOOPING_OFF == snooping_enable)
	{
		for (i = 0;i < LAN_PORT_NUM;i++)
        {
			ecnt_mc_api_xmit(i, WAN_MAP_PORT_ID,skb->data,skb->len);
		}

		return;
	}
	
	/* if callback failed, go on sending query packet to LAN */
	if (mc_ctrl_packet_rx_cb_func(port_number,skb->data,skb->len) != 0)
	{
		/* general query, send to all port */
		if (ECNT_IGMP_GENERAL_QUERY == query_type)
		{
			for (i = 0;i < LAN_PORT_NUM;i++)
			{
				ecnt_mc_api_xmit(i, WAN_MAP_PORT_ID,skb->data,skb->len);
			}
		}
		/* specific query, send to the group member */
		else if (ECNT_IGMP_SPECIFIC_QUERY == query_type)
		{
			ecnt_mcst_send_specific_query(skb);
		}
	}
}

int mtk_multicast_data_handle(struct sk_buff* skb)
{
	unsigned char* dst = eth_hdr(skb)->h_dest;
	uint32 fwd_port_mask = 0;
	int ret = -1;
	int mcst_flag = 0;
	int data_flag = 0;
	uint32 lan_port_mask = 0;
	uint32 wifi_port_mask = 0;
	uint8 query_pkt_type = 0;
	unsigned char snooping_enable = E_IGMP_SNOOPING_ON;
	
	if (0 != strcmp(skb->dev->name,PON_IF_NAME))
	{
		return -1;
	}

	/* whether multicast packet */
	mcst_flag = mtk_xpon_is_multicast_addr(dst);
    /*FWC unicast mac should forward to kernel protocol stack*/
    if(0 == mcst_flag)
    {
        return -1;
    }
	data_flag = mtk_xpon_is_data_pkt(skb);
    
    /*unicast gemport mulitcast packet should forward to kernel protocol stack*/
    if((1 == data_flag) && (IS_PKT_FROM_DS_UNICAST_GEM(skb)))
    {
        return -1;
    }
	if ((0 == mcst_flag) || (0 == data_flag))
	{
		query_pkt_type = ecnt_is_query_pkt(skb);
		if (query_pkt_type != ECNT_IGMP_NOT_QUERY)
		{
			ecnt_mcst_recv_ctrl_pkt(skb,query_pkt_type);
			return 0;
		}
		else
		{
			return -1;
		}
	}
	
	/*recovery mac header*/
	skb_push(skb, ETH_HLEN);
	
	ecnt_igmp_get_snooping_on_off(&snooping_enable);
	if(E_IGMP_SNOOPING_ON == snooping_enable)
	{
		igmp_get_fwd_portmask(skb,&fwd_port_mask);
	}
	else
	{
		fwd_port_mask = ECNT_LAN_PORT_MASK;
	}

	MULTI_LOG(MULTI_LOG_DBG,"before : fwd_port_mask = 0x%x\n",fwd_port_mask);
	
	lan_port_mask = (fwd_port_mask & 0xf);
	wifi_port_mask = ((fwd_port_mask & (~(uint32)0xf)) << 4);
	fwd_port_mask = lan_port_mask | wifi_port_mask;
	MULTI_LOG(MULTI_LOG_DBG,"after : fwd_port_mask = 0x%x\n",fwd_port_mask);
	
	mtk_xpon_hwnat_add_flow(skb, fwd_port_mask);
	if(0 == fwd_port_mask)
	{
		ret = mtk_xpon_hwnat_drop_multicast(skb);
		return 0;
	}
	else
	{
		/* xmit to lan interface directly */
		ret = mtk_xpon_fwd_data_to_port(skb,fwd_port_mask);
	}
	
	/* match for release */
	skb_pull(skb, ETH_HLEN);
	
	return ret;
}

int mtk_xpon_hwnat_flow_read(char *buf, char**start, off_t off, int count, int *eof, void *data)
{
	int len = 0;
	MTK_IGMP_HWNATEntry_t *entry = NULL;
	MTK_IGMP_HWNATEntry_t *tmp = NULL;
	len += sprintf(buf, "index type mask vid grp_addr  src_addr \n");
	list_for_each_entry_safe(entry, tmp, &mtk_hwnat_igmp, list)
	{
		len += sprintf(buf+len, "%d	%d	%d	%d	%d	[%02x-%02x-%02x-%02x] [%02x-%02x-%02x-%02x] \n", \
					entry->index, entry->proto, entry->wifinum, entry->mask,\
			entry->hwnat_vid, entry->grp_addr[0], entry->grp_addr[1], entry->grp_addr[2],entry->grp_addr[3],\
			entry->src_addr[0], entry->src_addr[1], entry->src_addr[2],entry->src_addr[3]);
	}

	*start = buf + off;
	if (len < off+ count)
	{
		*eof = 1;
	}
	
	len -= off;
	if (len > count)
	{
		len = count;
	}
	
	if (len < 0)
	{
		len = 0;
	}

  return len;
}

static int multicast_debug_level_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len = 0;
	len = sprintf(page, "%d\n", debuglever);
	
	len -= off;
	*start = page + off;

	if (len > count)
	{
		len = count;
	}
	else
	{
		*eof = 1;
	}

	if (len < 0)
	{
		len = 0;
	}

	return len;
}

static int multicast_debug_level_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32];

	if (count > sizeof(val_string) - 1)
	{
		return -EINVAL;
	}

	if (copy_from_user(val_string, buffer, count))
	{
		return -EFAULT;
	}

	val_string[count] = '\0';

	if (sscanf(val_string, "%lu", &debuglever) != 1) 
	{
		printk("usage: <debuglevel>\n");
	}
	
	return count;
}

static int xpon_mc_show_entry(char *buf, char**start, off_t off, int count, int *eof, void *data)
{
	int len = 0;
	MC_FwdEntry *entry = NULL;
	struct list_head *fwd_list = igmp_get_fdb_list();
	
	len += sprintf(buf, "flow_idx port	vlan	grp_ip		vmode	otci	itci\n");
	list_for_each_entry_rcu(entry,fwd_list,list)
	{
		len += sprintf(buf+len, "%d        %d	%d	%d.%d.%d.%d	%d	%d	%d\n",
			entry->flow_idx,entry->port_id,entry->vlan_id,
			NIPQUAD(entry->group_addr.IP.ipv4_address),
			entry->vlan_action.vlan_mode,
			entry->vlan_action.outer_tci,
			entry->vlan_action.inner_tci);
	}

	*start = buf + off;
	if (len < off+ count)
	{
		*eof = 1;
	}
	
	len -= off;
	if (len > count)
	{
		len = count;
	}
	
	if (len < 0)
	{
		len = 0;
	}

  return len;
}


/*********************************************************************************
 * ecnt multicast hook function implementation setction
 *
 *********************************************************************************/
ecnt_ret_val ecnt_mc_hook_set_recv_cb(struct ecnt_data *in_data)
{
	mc_ctrl_packet_rx *rx_callback = NULL;
	
	rx_callback = (mc_ctrl_packet_rx *)in_data;

	mc_ctrl_packet_rx_cb_func = *rx_callback;
	
	return ECNT_RETURN;
}

ecnt_ret_val ecnt_br_hook_handle(struct ecnt_data *in_data)
{
	int port_number = -1;
	struct net_data_s *rx_info = NULL;
	struct sk_buff *skb = NULL;
	unsigned char snooping_enable = E_IGMP_SNOOPING_ON;
	
	rx_info = (struct net_data_s *)in_data;
	skb = (*(rx_info->pskb));

	/* whether igmp protocol pkt */
	if (!ecnt_is_igmp_protocol_pkt(skb))
	{
		return ECNT_HOOK_ERROR;
	}
	
	/* get port number */
	port_number = get_port_id_by_name(skb->dev->name);
	if (port_number >= 0)
	{
		ecnt_igmp_get_snooping_on_off(&snooping_enable);
		
		/* recovery the eth Header, then send */
		skb_push(skb,ETH_HLEN);

		/* snooping off, return after xmit */
		if (E_IGMP_SNOOPING_OFF == snooping_enable)
		{
			ecnt_mc_api_xmit(WAN_MAP_PORT_ID, port_number, skb->data,skb->len);
			return ECNT_RETURN_DROP;
		}

		/* snooping on */
		if (mc_ctrl_packet_rx_cb_func)
		{
			/* if callback failed, then go on sending the join/leave packet to wan */
			if (mc_ctrl_packet_rx_cb_func(port_number,skb->data,skb->len) != 0)
			{
				ecnt_mc_api_xmit(WAN_MAP_PORT_ID, port_number, skb->data,skb->len);
			}
		}
	}
	
	return ECNT_RETURN_DROP;
}

ecnt_ret_val ecnt_mc_hook_xmit(struct ecnt_data *in_data)
{
	ecnt_mc_tx_info *tx_info= NULL;

	char *dev_name = NULL;
	struct sk_buff *skb = NULL;
	struct net_device *dev = NULL;

	tx_info = (ecnt_mc_tx_info *)in_data;
	
	/* alloc skb */
	#if defined(TCSUPPORT_MAX_PACKET_2000)
	skb = skbmgr_dev_alloc_skb4k();
	#else
	skb = skbmgr_dev_alloc_skb2k();
	#endif
	if (unlikely(!skb))
	{
		return ECNT_HOOK_ERROR;
	}
	
	/* get dev name */
	dev_name = get_dev_name_by_port(tx_info->dest_port);

	/* get dev */
	if (dev_name)
	{
		dev = dev_get_by_name(&init_net,dev_name);
	}
	
	if (dev)
	{	
		/* init skb */
        skb->mark |= (tx_info->src_port << 1);
		skb->dev = dev;
		memmove(skb->data,(unsigned char *)tx_info->data,tx_info->data_len);
		skb_put(skb,tx_info->data_len);

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
	
	return ECNT_RETURN;
}

ecnt_ret_val ecnt_mc_hook_entry_add(struct ecnt_data *in_data)
{
	mc_fwd_tbl_entry *entry = (mc_fwd_tbl_entry *)in_data;

	if (ecnt_igmp_add_entry(entry) < 0)
	{
		return ECNT_HOOK_ERROR;
	}
	
	return ECNT_RETURN;
}

ecnt_ret_val ecnt_mc_hook_entry_del(struct ecnt_data *in_data)
{
	mc_fwd_tbl_entry *entry = (mc_fwd_tbl_entry *)in_data;

	if (ecnt_igmp_del_entry(entry) < 0)
	{
		return ECNT_HOOK_ERROR;
	}

	return ECNT_RETURN;
}

ecnt_ret_val ecnt_mc_hook_vlan_isOneToN(struct ecnt_data *in_data)
{
	int vid = 0;
	uint32 port_mask = 0;
	ecnt_mc_data_s *pdata = NULL;
	
	pdata = (ecnt_mc_data_s *)in_data;
	
	skb_reset_mac_header(pdata->skb);
	
	ecnt_remove_stag(pdata->skb);
	
	/* whether mcst data pkt */
	if (!ecnt_is_mcst_data_pkt(pdata->skb))
	{
		return ECNT_HOOK_ERROR;
	}

	/* get skb original vid */
	vid = ecnt_get_skb_original_vid(pdata->skb);
	
	ecnt_mcst_get_fwd_mask(pdata->skb,vid,&port_mask);
	
	/* whether vlan 1:N */
	if (!ecnt_vlan_IsOneToN(port_mask,pdata->skb,NULL,vid))
	{
		return ECNT_HOOK_ERROR;
	}

	/* vlan 1:N then update skb to original vlan info */
	ecnt_mcst_update_vlan(pdata->skb);
	
	return ECNT_RETURN;
}

ecnt_ret_val ecnt_mc_hook_vlan_handle(struct ecnt_data *in_data)
{
	uint8 i = 0;
	uint32 fwd_mask = 0;
	ecnt_mc_data_s *pdata = NULL;
	
	pdata = (ecnt_mc_data_s *)in_data;

	skb_reset_mac_header(pdata->skb);
	
	/* remove stag */
	ecnt_remove_stag(pdata->skb);
	
	/* get fwd mask */
	igmp_get_fwd_portmask(pdata->skb,&fwd_mask);
	
	/* vlan handle */
	ecnt_mcst_vlan_handle(pdata->skb,fwd_mask);
	
	return ECNT_RETURN;
}

struct ecnt_hook_ops ecnt_mc_set_cb_ops = {
    .name = "ecnt_mc_hook_set_recv_cb",
    .hookfn = ecnt_mc_hook_set_recv_cb,
    .is_execute = 1,
    .maintype = ECNT_MULTICAST,
    .subtype = ECNT_MC_SET_RX_CB_HOOK,
};

struct ecnt_hook_ops ecnt_mc_tx_ops = {
    .name = "ecnt_mc_hook_xmit",
    .hookfn = ecnt_mc_hook_xmit,
    .is_execute = 1,
    .maintype = ECNT_MULTICAST,
    .subtype = ECNT_MC_XMIT_HOOK,
};

struct ecnt_hook_ops ecnt_br_handle_ops = {
    .name = "ecnt_br_hook_handle",
    .hookfn = ecnt_br_hook_handle,
    .is_execute = 1,
    .maintype = ECNT_NET_BR_INPUT,
    .subtype = ECNT_BR_HANDLE_FRAME,
};

struct ecnt_hook_ops ecnt_mc_add_ops = {
    .name = "ecnt_mc_hook_entry_add",
    .hookfn = ecnt_mc_hook_entry_add,
    .is_execute = 1,
    .maintype = ECNT_MULTICAST,
    .subtype = ECNT_MC_ADD_ENTRY_HOOK,
};

struct ecnt_hook_ops ecnt_mc_del_ops = {
    .name = "ecnt_mc_hook_entry_del",
    .hookfn = ecnt_mc_hook_entry_del,
    .is_execute = 1,
    .maintype = ECNT_MULTICAST,
    .subtype = ECNT_MC_DEL_ENTRY_HOOK,
};

struct ecnt_hook_ops ecnt_mc_vlan_ops = {
    .name = "ecnt_mc_hook_vlan_isOneToN",
    .hookfn = ecnt_mc_hook_vlan_isOneToN,
    .is_execute = 1,
    .maintype = ECNT_MULTICAST,
    .subtype = ECNT_MC_VLAN_ONETON_HOOK,
};

struct ecnt_hook_ops ecnt_mc_data_vlan_ops = {
    .name = "ecnt_mc_hook_vlan_handle",
    .hookfn = ecnt_mc_hook_vlan_handle,
    .is_execute = 1,
    .maintype = ECNT_MULTICAST,
    .subtype = ECNT_MC_VLAN_HANDLE_HOOK,
};

static void ecnt_igmp_hook_register(void)
{
	ecnt_register_hook(&ecnt_mc_set_cb_ops);
	ecnt_register_hook(&ecnt_mc_tx_ops);
	ecnt_register_hook(&ecnt_br_handle_ops);
	ecnt_register_hook(&ecnt_mc_add_ops);
	ecnt_register_hook(&ecnt_mc_del_ops);
	ecnt_register_hook(&ecnt_mc_vlan_ops);
	ecnt_register_hook(&ecnt_mc_data_vlan_ops);
}

static void ecnt_igmp_hook_unregister(void)
{
	ecnt_unregister_hook(&ecnt_mc_set_cb_ops);
	ecnt_unregister_hook(&ecnt_mc_tx_ops);
	ecnt_unregister_hook(&ecnt_br_handle_ops);
	ecnt_unregister_hook(&ecnt_mc_add_ops);
	ecnt_unregister_hook(&ecnt_mc_del_ops);
	ecnt_unregister_hook(&ecnt_mc_vlan_ops);
	ecnt_unregister_hook(&ecnt_mc_data_vlan_ops);
}

void ecnt_igmp_remove_list(void)
{
	igmp_clear_entry();
}


/*********************************************************************************
 * ecnt igmp module insmod and unmod setction
 *
 *********************************************************************************/
static int __init ecnt_igmp_init(void)
{
	int i = 0;
	struct proc_dir_entry *multicast_proc;
	
	printk("ecnt igmp module init\n");
	
	xpon_igmp_learn_flow_hook = mtk_xpon_hwnat_learn_flow;
	rcu_assign_pointer(mtk_multicast_data_handle_hook,mtk_multicast_data_handle);
	mtk_init_hwnat_list();
	multicast_speed_find_entry_hook	= mtk_xpon_hwnat_find_speed_entry;

	igmp_init_fdb();
	ecnt_igmp_cdev_init();
	ecnt_igmp_hook_register();
	
	/*for debug multicast data speed entry*/
	create_proc_read_entry("tc3162/mtk_igmp_hwnat", 0, NULL, mtk_xpon_hwnat_flow_read, NULL);
	
	/*for show mc forward table entry */
	create_proc_read_entry("tc3162/show_mc_fdb", 0, NULL, xpon_mc_show_entry, NULL);

	multicast_proc = create_proc_entry("tc3162/multi_debug", 0, NULL);
	multicast_proc->read_proc = multicast_debug_level_read_proc;
	multicast_proc->write_proc = multicast_debug_level_write_proc;
	
	return 0;
}

static void __exit ecnt_igmp_exit(void)
{
	printk("ecnt igmp module exit\n");
	
	xpon_igmp_learn_flow_hook = NULL;
	mtk_multicast_data_handle_hook = NULL;
	multicast_speed_find_entry_hook = NULL;
		
	remove_proc_entry("tc3162/mtk_igmp_hwnat", 0);
	remove_proc_entry("tc3162/multi_debug", 0);

	ecnt_igmp_remove_list();

	ecnt_igmp_hook_unregister();
}

module_init(ecnt_igmp_init);
module_exit(ecnt_igmp_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("IGMP Snooping data flow forward control driver");

