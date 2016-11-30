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

#ifdef CONFIG_QOS
extern u32 qos_queue_mask;
#endif

#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_PORTBIND//CONFIG_PORT_BINDING
extern int (*portbind_sw_hook)(void);
extern int (*portbind_check_hook)(struct net_device *in_dev, struct net_device *out_dev);
#endif
#else
#ifdef CONFIG_PORT_BINDING
extern int (*portbind_sw_hook)(void);
extern int (*portbind_check_hook)(char *inIf, char *outIf);
#endif
#endif

#ifdef TCSUPPORT_PON_MAC_FILTER
extern int (*pon_check_mac_hook)(struct sk_buff *skb);
extern int (*pon_mac_filter_get_mode_hook)(void);
#endif

#ifdef TCSUPPORT_PON_VLAN
extern int (*pon_insert_tag_hook)(struct sk_buff **pskb);
extern int (*pon_vlan_get_mode_hook)(void);
extern int (*pon_check_vlan_hook)(struct net_device *dev, struct sk_buff *skb);
extern int (*pon_check_user_group_hook)(struct sk_buff *skb);
extern int (*pon_PCP_decode_hook)(struct sk_buff **pskb);
#endif

#ifdef TCSUPPORT_VLAN_TAG
extern int (*insert_vtag_hook)(struct sk_buff **pskb);
extern int (*check_vtag_hook)(void);
#endif

#if defined(TCSUPPORT_GPON_DOWNSTREAM_MAPPING)
extern int (*gpon_downstream_mapping_hook)(struct sk_buff *skb);
#endif

#if defined(TCSUPPORT_CT_DS_LIMIT)
extern int remarkQueueIndexfromDSNodes( struct sk_buff *skb, int up_dw );
#endif


#ifdef CONFIG_PORT_BINDING	
int portbind_in_dev_que_xmit(struct sk_buff *skb)
{
	int rc = -ENOMEM;
	int portbind_ret = NULL;

#if defined(TCSUPPORT_FTP_THROUGHPUT)
	if ((portbind_sw_hook) && ((skb->portbind_mark & MASK_OUT_DEV) == 0)){
#else
	if ((portbind_sw_hook) && (portbind_sw_hook() == 1) && ((skb->portbind_mark & MASK_OUT_DEV) == 0)){
#endif
		if (portbind_check_hook){
			//only need check once. shnwind 20110407.
			portbind_ret = portbind_check_hook(skb->orig_dev_name, skb->dev->name);
			if(portbind_ret == 0){
				kfree_skb(skb);
				return rc;
			}else if(portbind_ret == 1){
				skb->portbind_mark |= MASK_OUT_DEV; 
			}
		}
	}

	return ECNT_CONTINUE;
}
#endif

#if defined(TCSUPPORT_PON_MAC_FILTER)
int pon_mac_filter_in_dev_que_xmit(struct sk_buff *skb)
{
	int mac_filter_mode = MODE_HGU;
	int rc = -ENOMEM;

	if(pon_mac_filter_get_mode_hook)
		mac_filter_mode = pon_mac_filter_get_mode_hook();

	if(pon_check_mac_hook)
	{
		if(mac_filter_mode == MODE_SFU && (skb->dev->name[0] == 'e' || skb->dev->name[0] == 'r' || skb->dev->name[0] == 'u'))
			skb->pon_mac_filter_flag |= PON_MAC_FILTER_TX_CALL_HOOK;
		if(pon_check_mac_hook(skb) == -1)
		{
			kfree_skb(skb);
			printk("\npon mac filter drop");
			return rc;
		}
	}
	
	return ECNT_CONTINUE;
}
#endif

#ifdef TCSUPPORT_PON_VLAN
int pon_vlan_in_dev_que_xmit(struct sk_buff **pskb, struct net_device *dev)
{
	int vlan_mode = MODE_HGU;
	struct sk_buff *skb = *pskb;
	int rc = -ENOMEM;
	
	if(pon_vlan_get_mode_hook)
		vlan_mode = pon_vlan_get_mode_hook();

	if(vlan_mode == MODE_SFU){
		/* packet from cpe */
		if (skb->original_dev == NULL) {
			skb->pon_vlan_flag |= PON_PKT_FROM_CPE;
		}
	}else{
		if(strcmp(skb->dev->name,"pon") == 0)
			skb->pon_vlan_flag |= PON_VLAN_TX_CALL_HOOK;
	}
	if(pon_insert_tag_hook){
		if(pon_insert_tag_hook(pskb) == -1){
			kfree_skb(skb);
			return rc;
		}
	}
	if(pon_check_vlan_hook && (pon_check_vlan_hook(dev,skb) != 1)){
		return rc;
	}
	return ECNT_CONTINUE;
}
#endif

#ifdef TCSUPPORT_VLAN_TAG
int vlan_tag_in_dev_que_xmit(struct sk_buff **pskb)
{
	int rc = -ENOMEM;
	struct sk_buff *skb = *pskb;
#ifdef TCSUPPORT_PON_VLAN
	int vlan_mode = MODE_HGU;
	if(pon_vlan_get_mode_hook)
		vlan_mode = pon_vlan_get_mode_hook();
	if(vlan_mode == MODE_HGU)
#endif
	{
		if (check_vtag_hook && (check_vtag_hook()) == 1){
			if (insert_vtag_hook && (-1 == insert_vtag_hook(pskb))){
				kfree_skb(skb);
				return rc;
			}
		}
	}
	return ECNT_CONTINUE;
}
#endif

#if 1
ecnt_ret_val ecnt_dev_que_xmit_hook(struct ecnt_data *in_data)
{
	struct net_data_s *net_data = (struct net_data_s *)in_data;
	struct sk_buff **pskb = net_data->pskb;
	int rc = -ENOMEM;
	int ret = 0;
	struct sk_buff *skb = *pskb;
	struct net_device *dev = skb->dev;

	
#if defined(TCSUPPORT_CT)
#else
#ifdef CONFIG_PORT_BINDING	
	ret = portbind_in_dev_que_xmit(skb);
	if(ret != ECNT_CONTINUE)
		return ret;
#endif
#endif

#if defined(TCSUPPORT_PON_VLAN) && defined(TCSUPPORT_PON_USER_ISOLATION)
	if(pon_check_user_group_hook)
	{
		if(pon_check_user_group_hook(skb) == -1)
			return rc;
	}
#endif
#ifdef TCSUPPORT_VLAN_TAG
/*this compile option will cause crash*/
	ret = vlan_tag_in_dev_que_xmit(pskb);
	if(ret != ECNT_CONTINUE)
		return ret;
#endif

#ifdef TCSUPPORT_PON_VLAN
	ret = pon_vlan_in_dev_que_xmit(pskb,dev);
	if(ret != ECNT_CONTINUE)
		return ret;
#endif

#ifdef TCSUPPORT_PON_MAC_FILTER 
	ret = pon_mac_filter_in_dev_que_xmit(skb);
	if(ret != ECNT_CONTINUE)
		return ret;
#endif
#if defined(TCSUPPORT_GPON_DOWNSTREAM_MAPPING)
	if(gpon_downstream_mapping_hook && (-1 == gpon_downstream_mapping_hook(skb)))
		return rc;
#endif
	
#ifdef TCSUPPORT_PON_VLAN
	/*decode after mapping*/
	if(pon_PCP_decode_hook){
		if(pon_PCP_decode_hook(pskb) == -1)
			return rc;
	}
#endif

	return ECNT_CONTINUE;
}
#endif

struct ecnt_hook_ops ecnt_dev_que_xmit_op = {
	.name = "dev_que_xmit_hook",
	.is_execute = 1,
	.hookfn = ecnt_dev_que_xmit_hook,
	.maintype = ECNT_NET_CORE_DEV,
	.subtype = ECNT_DEV_QUE_XMIT,
	.priority = 1
};

