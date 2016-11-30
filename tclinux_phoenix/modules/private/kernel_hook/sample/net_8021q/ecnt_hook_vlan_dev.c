#include <ecnt_hook/ecnt_hook.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <net/arp.h>

#include <linux/if_vlan.h>

#include <ecnt_hook/ecnt_hook_net.h>


#ifdef TCSUPPORT_VLAN_TAG
extern int (*remove_vtag_hook)(struct sk_buff *skb, struct net_device *dev);
extern int (*insert_vtag_hook)(struct sk_buff **pskb);
extern int (*check_vtag_hook)(void);
#endif

#if defined(TCSUPPORT_CT_VLAN_TAG)
extern int (*check_vtag_ct_hook)(void);
extern int (*store_vtag_ct_hook)(struct sk_buff *skb, struct net_device *dev);
extern u32 vtag_free_rx_cnt;
#endif

#ifdef TCSUPPORT_PON_VLAN
extern int (*pon_store_tag_hook)(struct sk_buff *skb, struct net_device *orig_dev);
#endif

int ecnt_set_vlan_skb_info(struct sk_buff *skb){	
#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_PORTBIND//CONFIG_PORT_BINDING
	if (skb->dev->name[0] == 'e') {
		skb->portbind_mark |= MASK_ORIGIN_DEV;
		skb->orig_dev = skb->dev;
	}
#endif
#else
#ifdef CONFIG_PORT_BINDING
	if (skb->dev->name[0] == 'e') {
	//	skb->mark |= MASK_ORIGIN_DEV;
		skb->portbind_mark |= MASK_ORIGIN_DEV;
		memcpy(skb->orig_dev_name, skb->dev->name, IFNAMSIZ);
		//printk("vlan_skb_recv: begin orig_dev name is [%s], orig_dev name is [%s]\n", skb->orig_dev_name, orig_dev->name);
	}
#endif
#endif
	return 0;
}

ecnt_ret_val ecnt_vlan_skb_recv_hook(struct ecnt_data *in_data)
{
	struct net_data_s *net_data = (struct net_data_s *)in_data;
	struct sk_buff **pskb = net_data->pskb;
	struct sk_buff *skb = *pskb;
	struct ecnt_vlan_skb_rcv_s *pri_data = &net_data->vlan_skb_rcv_data;
	struct net_device *vlan_dev = pri_data->vlan_dev;
	struct packet_type *ptype = pri_data->ptype;
	struct net_device *orig_dev = pri_data->orig_dev;

	int ret = 0;
	struct vlan_hdr *vhdr;
	vhdr = (struct vlan_hdr *)skb->data;
	
#ifdef TCSUPPORT_VLAN_TAG
	u16 *proto = NULL;
#endif
	
#ifdef TCSUPPORT_PON_VLAN
	if(ptype->type != ETH_P_8021Q)
	{
		goto Pon_Handle;
	}
#endif
	if(!vlan_dev){
#ifdef TCSUPPORT_PON_VLAN
	Pon_Handle:
		if(pon_store_tag_hook)
		{
			ret = pon_store_tag_hook(skb, orig_dev);
			if(ret == 0)
			{
				printk("hokk==> 81, skb = %08X\n",skb);
				netif_rx(skb);
				printk("hook==>82, skb = %08X, \n",skb);
				return ECNT_RETURN;
			}
			else if(ret == -1)
			{
				//kfree_skb(skb);
				//return -1;
				return ECNT_RETURN_DROP;
			}
			else
			{
				//HGU mode,do nothing
			}
		}
#endif
#if defined(TCSUPPORT_CT_VLAN_TAG)
		/*remove vlan tag from packet, store vlan tag to skb structure*/
		if (check_vtag_ct_hook && (check_vtag_ct_hook() == 1)
			&& store_vtag_ct_hook){
			if(store_vtag_ct_hook(skb, orig_dev) == 0){
				/* Take off the VLAN header (4 bytes currently) */
				skb_pull_rcsum(skb, VLAN_HLEN);
				skb->dev = orig_dev;
				printk("hokk==> 106, skb = %08X\n",skb);
				netif_rx(skb);
				printk("hokk==> 108, skb = %08X\n",skb);
				rcu_read_unlock();
				//return 0;
				return ECNT_RETURN;
			}else{
				/* must free skb !! */
				//kfree_skb(skb);
				vtag_free_rx_cnt++;
				rcu_read_unlock();
				//return -1;
				return ECNT_RETURN_DROP;
			}
		}
#endif
	#ifdef TCSUPPORT_VLAN_TAG
			if (check_vtag_hook && (check_vtag_hook() == 1)) {
				if (remove_vtag_hook) {
					 if (remove_vtag_hook(skb, orig_dev) == -1) {
						/* must free skb !! */
						//kfree_skb(skb);
						rcu_read_unlock();
						//return -1;
						return ECNT_RETURN_DROP;
					 }
					 else {
						netif_rx(skb);
						rcu_read_unlock();
						//return 0;
						return ECNT_RETURN;
					 }
				}
				else {
					goto Normal_Handle;
				}
			}
			else {
	Normal_Handle:
#if defined(TCSUPPORT_CT)
				if((orig_dev != NULL) && (orig_dev->name[0] == 'b'))
#else
				if((orig_dev != NULL) && ((orig_dev->name[0] == 'b') || (orig_dev->name[0] == 'n'))) 
#endif
				{
					proto = vhdr->h_vlan_encapsulated_proto;
					skb->protocol = proto;
					/* Take off the VLAN header (4 bytes currently) */
					skb_pull_rcsum(skb, VLAN_HLEN);
					skb->dev = orig_dev;
					printk("hokk==> 156, skb = %08X\n",skb);
					netif_rx(skb);
					printk("hokk==> 158, skb = %08X\n",skb);
					rcu_read_unlock();
					//return 0;
					return ECNT_RETURN;
				}
				else {
					/*pr_debug("%s: ERROR: No net_device for VID: %u on dev: %s\n",
						 __func__, vlan_id, dev->name);*/
					//kfree_skb(skb);
					rcu_read_unlock();
					//return -1;
					return ECNT_RETURN_DROP;
	
				}
			}
#endif
		}
	else{
		skb->dev = vlan_dev;
		ecnt_set_vlan_skb_info(skb);	
	}

	return ECNT_CONTINUE;
}

struct ecnt_hook_ops ecnt_vlan_skb_recv_op = {
	.name = "vlan_skb_recv_hook",
	.is_execute = 1,
	.hookfn = ecnt_vlan_skb_recv_hook,
	.maintype = ECNT_NET_VLAN_DEV,
	.subtype =  ECNT_VLAN_SKB_RECV,
	.priority = 1
};
