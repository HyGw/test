#include <ecnt_hook/ecnt_hook.h>
#include <linux/netdevice.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <net/arp.h>
#include <linux/if_vlan.h>
#include <ecnt_hook/ecnt_hook_net.h>

#define MODE_HGU 0
#define MODE_SFU 1

#ifdef TCSUPPORT_PON_VLAN
extern int (*pon_vlan_get_mode_hook)(void);
extern int (*pon_insert_tag_hook)(struct sk_buff **pskb);
#endif
#if defined(TCSUPPORT_PON_MAC_FILTER)
extern int (*pon_check_mac_hook)(struct sk_buff *skb);
extern int (*pon_mac_filter_get_mode_hook)(void);
#endif
#if defined(TCSUPPORT_XPON_IGMP)
extern int (*xpon_igmp_acl_filter_hook)(struct sk_buff* skb);
#endif
#if defined(TCSUPPORT_BRIDGE_FASTPATH)
extern int (*hook_bridge_shortcut_process)(struct net_device *net_dev, struct sk_buff *skb);
#endif
#ifdef TCSUPPORT_UPSTREAM_VLAN_POLICER
extern int (*upstream_vlan_policer_hook)(struct sk_buff *skb);
#endif
#ifdef TCSUPPORT_UPSTREAM_VLAN_POLICER
extern int (*upstream_vlan_policer_hook)(struct sk_buff *skb);
#endif
#ifdef TCSUPPORT_VLAN_TAG
extern int (*check_vtag_hook)(void);
extern int (*get_vtag_hook)(struct net_device *dev, struct sk_buff *skb);
#endif

extern int (*portbind_sw_hook)(void);

int ecnt_netif_recv_skb_hook(struct ecnt_data *in_data)
{
	struct net_data_s *net_data = (struct net_data_s *)in_data;
	//struct sk_buff **pskb = in_data->net_data.pskb;
	struct sk_buff *skb = *(net_data->pskb);
	struct net_device *orig_dev;
	orig_dev = skb->dev;

#if defined(TCSUPPORT_PON_VLAN)
		int vlan_mode = MODE_HGU;
#endif
#if defined(TCSUPPORT_PON_MAC_FILTER)
		int mac_filter_mode = MODE_HGU;
#endif
		
#ifdef TCSUPPORT_PON_VLAN
		if(pon_vlan_get_mode_hook)
			vlan_mode = pon_vlan_get_mode_hook();
#endif
#if defined(TCSUPPORT_PON_MAC_FILTER)
		if(pon_mac_filter_get_mode_hook)
			mac_filter_mode = pon_mac_filter_get_mode_hook();
#endif
#if defined(TCSUPPORT_XPON_IGMP)
		if(strstr(skb->dev->name, "pon")){
			if(xpon_igmp_acl_filter_hook && xpon_igmp_acl_filter_hook(skb) == 0){
				//return NET_RX_DROP;
				printk("68\n");
				return ECNT_RETURN;
			}
		}
#endif
	

#if !defined(TCSUPPORT_CT)
#if defined(TCSUPPORT_BRIDGE_FASTPATH)
			if(hook_bridge_shortcut_process)
			{
				if( hook_bridge_shortcut_process(skb->dev, skb) ){
					//return NET_RX_DROP;
					printk("83\n");
					return ECNT_RETURN;
				}	
			}
#endif
#endif
	
#ifdef TCSUPPORT_PON_VLAN
		if((orig_dev->name[0] == 'r' || orig_dev->name[0] == 'u' || orig_dev->name[0] == 'e') && vlan_mode == MODE_SFU)
		{
			skb->pon_vlan_flag |= PON_PKT_FROM_LAN;
		}
		else if(strcmp(orig_dev->name,"pon") == 0 && vlan_mode == MODE_SFU)
		{
			skb->pon_vlan_flag |= PON_PKT_FROM_WAN;
		}
		skb->original_dev = skb->dev;
#endif
	
#ifdef TCSUPPORT_PON_VLAN
		if(pon_insert_tag_hook && vlan_mode == MODE_HGU)
		{
#if 0 //def TCSUPPORT_PON_IP_HOST
			if(strcmp(orig_dev->name,"n") == 0){			
				skb->pon_vlan_flag |= PON_VLAN_RX_CALL_HOOK;
				if(isVoipWan(orig_dev))
					skb->pon_vlan_flag |= PON_PKT_VOIP_RX;			
			}
#else
			if(strcmp(orig_dev->name,"pon") == 0)
				skb->pon_vlan_flag |= PON_VLAN_RX_CALL_HOOK;
#endif
			if(pon_insert_tag_hook(&skb) == -1)
			{
				//kfree_skb(skb);
				//return NET_RX_DROP;
				printk("119\n");
				return ECNT_RETURN_DROP;
			}
		}
#ifdef TCSUPPORT_PON_IP_HOST
		else if(pon_insert_tag_hook && vlan_mode == MODE_SFU){
			if(isVoipWan(orig_dev)){
				skb->pon_vlan_flag |= PON_PKT_VOIP_RX;
				if(pon_insert_tag_hook(&skb) == -1)
				{
					//kfree_skb(skb);
					//return NET_RX_DROP;
					printk("131\n");
					return ECNT_RETURN_DROP;
				}
			}
		}
#endif
#endif
#ifdef TCSUPPORT_PON_MAC_FILTER 
		if(pon_check_mac_hook && mac_filter_mode == MODE_HGU)
		{
			if(strcmp(orig_dev->name,"pon") == 0)
				skb->pon_mac_filter_flag |= PON_MAC_FILTER_RX_CALL_HOOK;
			if(pon_check_mac_hook(skb) == -1)
			{
				//kfree_skb(skb);
				//return NET_RX_DROP;
				printk("147\n");
				return ECNT_RETURN_DROP;
			}
		} 
#ifdef TCSUPPORT_CHS
		else if(pon_check_mac_hook && mac_filter_mode == MODE_SFU)
		{
			if (orig_dev->name[0] == 'e'|| orig_dev->name[0] == 'r'|| orig_dev->name[0] == 'u')
				skb->pon_mac_filter_flag |= PON_MAC_FILTER_RX_CALL_HOOK;
			if (pon_check_mac_hook(skb) == -1)
			{
				//kfree_skb(skb);
				//return NET_RX_DROP;
				printk("160\n");
				return ECNT_RETURN_DROP;
			}
		}
#endif
#endif
#ifdef TCSUPPORT_UPSTREAM_VLAN_POLICER
			if (upstream_vlan_policer_hook)
			{
					upstream_vlan_policer_hook(skb);
			}
#endif
#ifdef TCSUPPORT_PON_VLAN
		if(vlan_mode == MODE_HGU)
#endif
		{
	#ifdef TCSUPPORT_VLAN_TAG
		//#if !defined(TCSUPPORT_FTP_THROUGHPUT)
		if (check_vtag_hook && (check_vtag_hook() == 1))
		//#endif
		{
			if (get_vtag_hook)
				if (-1 == get_vtag_hook(orig_dev, skb)) {
					//kfree_skb(skb);
					//return NET_RX_DROP;
					printk("185\n");
					return ECNT_RETURN_DROP;
				}
		}
	#endif
		}
#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_PORTBIND//CONFIG_PORT_BINDING
#ifdef CONFIG_SMUX
			 /*we only check OSMUX interface and other interface*/
			if((orig_dev->name[0] != 'n') || (orig_dev->priv_flags & IFF_OSMUX))
			 {
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_PON)
				if( ((skb->portbind_mark & MASK_ORIGIN_DEV) == 0) || (orig_dev->priv_flags & IFF_OSMUX))
#else
				if( (skb->portbind_mark & MASK_ORIGIN_DEV) == 0)
#endif
				{
					skb->portbind_mark |= MASK_ORIGIN_DEV;
					skb->orig_dev = skb->dev;
				}
	
				if (orig_dev->name[0] == 'p') {
					skb->orig_dev = skb->dev;
				}
			 }
#else
			if( (skb->portbind_mark & MASK_ORIGIN_DEV) == 0)
			{
				skb->portbind_mark |= MASK_ORIGIN_DEV;
				skb->orig_dev = skb->dev;
			}
			if (orig_dev->name[0] == 'p') {
				skb->orig_dev = skb->dev;
			}
#endif
#endif
#else
#ifdef CONFIG_PORT_BINDING
#if defined(TCSUPPORT_FTP_THROUGHPUT)
		if (portbind_sw_hook) {
#else
		if (portbind_sw_hook && (portbind_sw_hook() == 1)) {
#endif
#ifdef CONFIG_SMUX
		 /*we only check OSMUX interface and other interface*/
		 //if((orig_dev->priv_flags & IFF_RSMUX) == 0)
		//if((orig_dev->name[0] != 'n') || (orig_dev->priv_flags & IFF_OSMUX))
		 //{
			if( (orig_dev->priv_flags & IFF_OSMUX) || (skb->portbind_mark & MASK_ORIGIN_DEV) == 0)
			{
				skb->portbind_mark |= MASK_ORIGIN_DEV;
				memcpy(skb->orig_dev_name, orig_dev->name, IFNAMSIZ);
				//printk("netif_receive_skb: CONFIG_SMUX origin name is [%s], skb device name is [%s]\n", skb->orig_dev_name, skb->dev->name);
			}
	
		 //}
#else
		if( (skb->portbind_mark & MASK_ORIGIN_DEV) == 0)
		{
			skb->portbind_mark |= MASK_ORIGIN_DEV;
			memcpy(skb->orig_dev_name, orig_dev->name, IFNAMSIZ);
			//printk("netif_receive_skb: begin orig_dev name is [%s], skb device name is [%s]\n", skb->orig_dev_name, skb->dev->name);
		}
#endif
		}
#endif
#endif

	//printk("return continue\n");
	return ECNT_CONTINUE;
}

struct ecnt_hook_ops ecnt_netif_recv_skb_op = {
	.name = "netif_recv_skb_hook",
	.is_execute = 1,
	.hookfn = ecnt_netif_recv_skb_hook,
	.maintype = ECNT_NET_CORE_DEV,
	.subtype	= ECNT_NETIF_RCV_SKB,
	.priority = 1
};

