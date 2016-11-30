/*
 *	Handle incoming frames
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/netfilter_bridge.h>
#include "br_private.h"

#include <ecnt_hook/ecnt_hook_net.h>

#ifdef TCSUPPORT_INIC_HOST
#include <linux/mtd/fttdp_inic.h>
#endif
#if defined(TCSUPPORT_CT_DS_LIMIT)
#include <linux/qos_type.h>
#include <linux/if_pppox.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#if defined(TCSUPPORT_CPU_EN7521)
#include "../../../modules/private/ether/en7512/tcphy/mtkswitch_api.h"
#else
#include "../../../modules/private/tcphy/mtkswitch_api.h"
#endif
#endif

#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_PORTBIND//CONFIG_PORT_BINDING
extern int (*portbind_sw_hook)(void);
extern int (*portbind_check_hook)(char *inIf, char *outIf);
#endif
#if defined(TCSUPPORT_CT_VLAN_BIND)
extern int (*vlanbind_active_hook)(void);
extern int (*vlanbind_entry_active_hook)(int i);
#endif
#else
#ifdef CONFIG_PORT_BINDING
#if defined(TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND)
extern int (*portbind_sw_prior_hook)(struct sk_buff *skb);
#endif
extern int (*portbind_sw_hook)(void);
extern int (*portbind_check_hook)(char *inIf, char *outIf);
#endif
#endif
#if defined(TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND)
extern struct net_bridge_fdb_entry *vlan_fdb_get(struct sk_buff *skb, struct net_bridge *br,
					  const unsigned char *addr);
#endif
/* Bridge group multicast address 802.1d (pg 51). */
const u8 br_group_address[ETH_ALEN] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x00 };

#if defined(TCSUPPORT_VLAN_TAG)
extern int (*remove_vtag_hook)(struct sk_buff *skb, struct net_device *dev);
#endif

#if defined(TCSUPPORT_XPON_IGMP)

#if 0
int (*xpon_bridge_incoming_hook)(struct sk_buff *skb, int clone) = NULL;
EXPORT_SYMBOL(xpon_bridge_incoming_hook);
#endif


int (*xpon_sfu_up_send_multicast_frame_hook)(struct sk_buff *skb, int clone) = NULL;

int (*xpon_sfu_up_multicast_incoming_hook)(struct sk_buff *skb, int clone) = NULL;

int (*xpon_sfu_down_multicast_incoming_hook)(struct sk_buff *skb, int clone) = NULL;

int (*xpon_sfu_up_multicast_vlan_hook)(struct sk_buff *skb, int clone) = NULL;

int (*xpon_sfu_multicast_protocol_hook)(struct sk_buff *skb) = NULL;

int (*xpon_up_igmp_uni_vlan_filter_hook)(struct sk_buff *skb) = NULL;

int (*xpon_up_igmp_ani_vlan_filter_hook)(struct sk_buff *skb) = NULL;

int (*isVlanOperationInMulticastModule_hook)(struct sk_buff *skb) = NULL;


EXPORT_SYMBOL(xpon_sfu_up_send_multicast_frame_hook);

EXPORT_SYMBOL(xpon_sfu_up_multicast_incoming_hook);

EXPORT_SYMBOL(xpon_sfu_down_multicast_incoming_hook);

EXPORT_SYMBOL(xpon_sfu_up_multicast_vlan_hook);

EXPORT_SYMBOL(xpon_sfu_multicast_protocol_hook);

EXPORT_SYMBOL(xpon_up_igmp_uni_vlan_filter_hook);

EXPORT_SYMBOL(xpon_up_igmp_ani_vlan_filter_hook);

EXPORT_SYMBOL(isVlanOperationInMulticastModule_hook);


extern int (*wan_multicast_drop_hook)(struct sk_buff *skb);
#endif

__IMEM static int br_pass_frame_up(struct sk_buff *skb)
{
	struct net_device *indev, *brdev = BR_INPUT_SKB_CB(skb)->brdev;
	struct net_bridge *br = netdev_priv(brdev);
	struct br_cpu_netstats *brstats = this_cpu_ptr(br->stats);

	u64_stats_update_begin(&brstats->syncp);
	brstats->rx_packets++;
	brstats->rx_bytes += skb->len;
	u64_stats_update_end(&brstats->syncp);

	indev = skb->dev;
	skb->dev = brdev;

#if defined(TCSUPPORT_CT_STB_TEST)
	if (indev != NULL && skb->protocol == 0x980B && indev->name[0] != 'e' && indev->name[0] != 'r') {
		kfree_skb(skb);
		return 0;
	}
#endif
	
	return NF_HOOK(NFPROTO_BRIDGE, NF_BR_LOCAL_IN, skb, indev, NULL,
		       netif_receive_skb);
}

#ifdef TCSUPPORT_BRIDGE_MAC_LIMIT
extern void br_fdb_need_flush(struct net_bridge *br);
extern bool br_fdb_judge_mac_num(struct net_bridge *br, char* devName);

extern bool br_fdb_mac_exist(struct net_bridge *br,const unsigned char *addr,char* devName);
#endif

#if defined(TCSUPPORT_CT_DS_LIMIT)
#ifdef TCSUPPORT_MT7530_SWITCH_API
int (*dslimit_SetDsTrtcmEnable)(u8 enable);
int (*dslimit_SetDsTrtcm)(mt7530_switch_api_trtcm_t *trtcm_p);
EXPORT_SYMBOL(dslimit_SetDsTrtcmEnable);
EXPORT_SYMBOL(dslimit_SetDsTrtcm);
#endif
#define VTAG_VID_MASK 0xfff
#define VTAG_GET_VID(tci) ((tci) & VLAN_VID_MASK)

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

/* upstream 0: disabled; 1: base interface; 2: base vlan id; 3: base ip range; */
static int dsmode_up = 0;
/* upstream dataspeed nodes list, max support 10 nodes */
static struct dsnode gk_dataspeed_nodes_up[DSLIMIT_MAX_NODES] = {0};
/* downstream 0: disabled; 1: base interface; 2: base vlan id; 3: base ip range; */
static int dsmode_dw = 0;
/* downstream dataspeed nodes list, max support 10 nodes */
static struct dsnode gk_dataspeed_nodes_dw[DSLIMIT_MAX_NODES] = {0};

/* dataspeed limit node downstream queue  */
static int dslimit_dw_setqueue(int queue[])
{
#ifdef TCSUPPORT_MT7530_SWITCH_API
	int idx = 0, queue_idx = 0;
	mt7530_switch_api_trtcm_t trtcm = {0};

	if ( !dslimit_SetDsTrtcm )
	{
		printk("\nerror: dslimit_SetDsTrtcm NULL!\n");
		return -1;
	}

	for ( idx = 1; idx <= DSLIMIT_MAX_QUEUE_NUM; idx ++ )
	{
		queue_idx = idx - 1;
		memset(&trtcm, 0, sizeof(trtcm));

		if ( 0 == queue[queue_idx] )
			break;

		trtcm.trtcmId = idx;
		trtcm.CIR = ( queue[queue_idx] / 64 ); /* per 64k bps*/
		dslimit_SetDsTrtcm(&trtcm);
	}
#endif

	return 0;
}

/* dataspeed limit node info display  */
static int dataspeed_limit_node_disp(char *buf, int up_dw)
{
	int idx = 0;
	const char *mode_str[] = {"disabled", "base interface",
		"base vlanid", "base ip range", "unknow"};
	int mode_str_size = sizeof(mode_str)/sizeof(char *);
	int unknow_ptr = mode_str_size  - 1;
	int lan_idx = 0, vlan_id = 0;
	int dsmode = 0, len = 0;
	struct dsnode *p_dataspeed_nodes = NULL;

	if ( DSLIMIT_UP == up_dw )
	{
		dsmode = dsmode_up;
		p_dataspeed_nodes = gk_dataspeed_nodes_up;
	}
	else if ( DSLIMIT_DW == up_dw )
	{
		dsmode = dsmode_dw;
		p_dataspeed_nodes = gk_dataspeed_nodes_dw;
	}
	else
		return 0;

	len += sprintf(buf,
		"========%s stream nodes informations========\n",
		( DSLIMIT_UP == up_dw ) ? "up" : "down" );

	len += sprintf(buf + len, "mode=[%u:%s]\n", dsmode,
		( dsmode >= unknow_ptr ) ? mode_str[unknow_ptr] : mode_str[dsmode]);

	if ( DSLIMIT_MODE_DISABLE != dsmode )
	{
		for ( idx = 0; idx < DSLIMIT_MAX_NODES; idx ++ )
		{
			if ( 1 == p_dataspeed_nodes[idx].ds_used )
			{
				len += sprintf(buf + len, "\n--------node[%d]--------\n", idx);
				switch ( dsmode )
				{
					case DSLIMIT_MODE_IFACE :
						lan_idx = p_dataspeed_nodes[idx].ds_lanif[0];
						if ( lan_idx >= 1 && lan_idx <= 4 )
							len += sprintf(buf + len,
								"Interface=[LAN%d]\n", lan_idx);
						else if ( lan_idx >= 5 && lan_idx <= 8 )
							len += sprintf(buf + len,
								"Interface=[SSID%d]\n", lan_idx - 4);
						else
							continue;
						break;
					case DSLIMIT_MODE_VLANID :
						vlan_id = p_dataspeed_nodes[idx].ds_vlanid[0];
						if ( vlan_id >= 1 && vlan_id <= 4094 )
							len += sprintf(buf + len,
								"VLANID=[%d]\n", vlan_id);
						else if ( DS_VLANID_UNTAG == vlan_id )
							len += sprintf(buf + len,
								"VLANID=[%s]\n", "UNTAGGED");
						else
							continue;
						break;
					case DSLIMIT_MODE_IPRANGE :
						if ( DS_IPTYPE_IPV4 
							== p_dataspeed_nodes[idx].ds_iptype )
						{
							len += sprintf(buf + len,
								"IPv4 Range=[%d.%d.%d.%d-%d.%d.%d.%d]\n",
								  p_dataspeed_nodes[idx].ds_ip6addr[0]
								, p_dataspeed_nodes[idx].ds_ip6addr[1]
								, p_dataspeed_nodes[idx].ds_ip6addr[2]
								, p_dataspeed_nodes[idx].ds_ip6addr[3]
								, p_dataspeed_nodes[idx].ds_ip6addr[4]
								, p_dataspeed_nodes[idx].ds_ip6addr[5]
								, p_dataspeed_nodes[idx].ds_ip6addr[6]
								, p_dataspeed_nodes[idx].ds_ip6addr[7]);
						}
						else if ( DS_IPTYPE_IPV6 
							== p_dataspeed_nodes[idx].ds_iptype )
						{
							len += sprintf(buf + len,
								"IPv6=[%04X:%04X:%04X:%04X]\n",
								  p_dataspeed_nodes[idx].ds_ip4addr[0]
								, p_dataspeed_nodes[idx].ds_ip4addr[1]
								, p_dataspeed_nodes[idx].ds_ip4addr[2]
								, p_dataspeed_nodes[idx].ds_ip4addr[3]);
						}
						break;
					default:
						continue;
				}

				switch ( dsmode )
				{
					case DSLIMIT_MODE_IFACE :
					case DSLIMIT_MODE_VLANID :
					case DSLIMIT_MODE_IPRANGE :
						len += sprintf(buf + len,
							"Use queue[%d], limit speed=[%d]kbps\n",
							  p_dataspeed_nodes[idx].ds_queue
							, p_dataspeed_nodes[idx].ds_queue_speed);
						break;
					default :
						break;
				}
			}
		}
	}

	return len;
}

/* dataspeed limit read proc for upstream  */
static int up_dataspeed_limit_read_proc
(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int len = 0;

	len = dataspeed_limit_node_disp(buf, DSLIMIT_UP);

	*start = buf + off;
	if (len < off + count)
		*eof = 1;
	len -= off;
	if (len > count)
		len = count ;
	if (len <0)
		len = 0;

	return len;
}

/* dataspeed limit read proc for downstream  */
static int down_dataspeed_limit_read_proc
(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int len = 0;

	len = dataspeed_limit_node_disp(buf, DSLIMIT_DW);

	*start = buf + off;
	if (len < off + count)
		*eof = 1;
	len -= off;
	if (len > count)
		len = count ;
	if (len <0)
		len = 0;

	return len;
}

/* ioctl for /dev/qos_dslimit  */
static int dslimit_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int queue_speed[DSLIMIT_MAX_QUEUE_NUM] = {0};

	switch ( cmd )
	{
	case QOSDSLIMIT_IOC_MODECFG_UP:
		if ( copy_from_user(&dsmode_up, (int *)arg, sizeof(dsmode_up)) )
			return -EFAULT;
		break;
	case QOSDSLIMIT_IOC_MODECFG_DW:
		if ( copy_from_user(&dsmode_dw, (int *)arg, sizeof(dsmode_dw)) )
			return -EFAULT;
#ifdef TCSUPPORT_MT7530_SWITCH_API
		if ( dslimit_SetDsTrtcmEnable )
		{
			if ( DSLIMIT_MODE_DISABLE == dsmode_dw )
				dslimit_SetDsTrtcmEnable(0);
			else
				dslimit_SetDsTrtcmEnable(1);
		}
		else
		{
			printk("\nerror: dslimit_SetDsTrtcmEnable NULL!\n");
			return -EFAULT;
		}
#endif
		break;
	case QOSDSLIMIT_IOC_DATACFG_UP:
		if ( copy_from_user(gk_dataspeed_nodes_up,
					(struct dsnode*)arg, sizeof(gk_dataspeed_nodes_up)) )
			return -EFAULT;
		break;
	case QOSDSLIMIT_IOC_DATACFG_DW:
		if ( copy_from_user(gk_dataspeed_nodes_dw,
					(struct dsnode*)arg, sizeof(gk_dataspeed_nodes_dw)) )
			return -EFAULT;
		break;
	case QOSDSLIMIT_IOC_QUEUECFG_DW:
		if ( copy_from_user(queue_speed, (int *)arg, sizeof(queue_speed)) )
			return -EFAULT;
		dslimit_dw_setqueue(queue_speed);
		break;
	default:
		return -EFAULT;
	}

	return 0;
}

/* open for /dev/qos_dslimit  */
static int dslimit_open(struct inode *inode, struct file *filp)
{
	return 0;
}

/* operations for  /dev/qos_dslimit  */
static struct file_operations dslimit_fops =
{
	.owner          = THIS_MODULE,
	.write          = NULL,
	.read           = NULL,
	.unlocked_ioctl = dslimit_ioctl,
	.open           = dslimit_open,
	.release        = NULL,
};

/* init dataspeed limit */
int br_dataspeed_limit_init(void)
{
	int status = 0;
	struct proc_dir_entry *up_dslimit_proc = NULL;
	struct proc_dir_entry *dw_dslimit_proc = NULL;

	up_dslimit_proc = create_proc_entry("tc3162/upstream_dataspeed_limit", 0, NULL);
	if( NULL == up_dslimit_proc )
	{
		printk("ERROR!Create proc entry br_dataspeed_limit fail!\n");
		return -1;
	}
	up_dslimit_proc->read_proc = up_dataspeed_limit_read_proc;

	dw_dslimit_proc = create_proc_entry("tc3162/downstream_dataspeed_limit", 0, NULL);
	if( NULL == dw_dslimit_proc )
	{
		printk("ERROR!Create proc entry br_dataspeed_limit fail!\n");
		return -1;
	}
	dw_dslimit_proc->read_proc = down_dataspeed_limit_read_proc;

	status = register_chrdev(QOSDSLIMIT_MAJOR, QOSDSLIMIT_DEV_NAME, &dslimit_fops);
	if ( status < 0 )
		return status;

	dsmode_up = DSLIMIT_MODE_DISABLE;
	dsmode_dw = DSLIMIT_MODE_DISABLE;
	memset(gk_dataspeed_nodes_up, 0, sizeof(gk_dataspeed_nodes_up));
	memset(gk_dataspeed_nodes_dw, 0, sizeof(gk_dataspeed_nodes_dw));
	return 0;
}
/* remove dataspeed limit */
void br_dataspeed_limit_fini(void)
{
	remove_proc_entry("tc3162/upstream_dataspeed_limit", 0);
	remove_proc_entry("tc3162/downstream_dataspeed_limit", 0);
	unregister_chrdev(QOSDSLIMIT_MAJOR, QOSDSLIMIT_DEV_NAME);
	return;
}

/*
	get queue index by lan interface.
*/
int getQueueIndexfromIface(int lanidx, int up_dw)
{
	int idx = 0;
	struct dsnode *p_dataspeed_nodes = NULL;

	if ( DSLIMIT_UP == up_dw )
		p_dataspeed_nodes = gk_dataspeed_nodes_up;
	else if ( DSLIMIT_DW == up_dw )
		p_dataspeed_nodes = gk_dataspeed_nodes_dw;
	else
		return -1;

	for ( idx = 0; idx < DSLIMIT_MAX_NODES; idx ++ )
	{
		/* no need to check when found deactived rule. */
		if ( 0 == p_dataspeed_nodes[idx].ds_used )
			return -1;

		if ( lanidx == p_dataspeed_nodes[idx].ds_lanif[0] )
			return p_dataspeed_nodes[idx].ds_queue;
	}

	return -1;
}

/*
	get queue index by vlan id.
*/
int getQueueIndexfromvlanid(int vlanid, int up_dw)
{
	int idx = 0;
	struct dsnode *p_dataspeed_nodes = NULL;

	if ( DSLIMIT_UP == up_dw )
		p_dataspeed_nodes = gk_dataspeed_nodes_up;
	else if ( DSLIMIT_DW == up_dw )
		p_dataspeed_nodes = gk_dataspeed_nodes_dw;
	else
		return -1;

	for ( idx = 0; idx < DSLIMIT_MAX_NODES; idx ++ )
	{
		/* no need to check when found deactived rule. */
		if ( 0 == p_dataspeed_nodes[idx].ds_used )
			return -1;

		if ( vlanid == p_dataspeed_nodes[idx].ds_vlanid[0] )
			return p_dataspeed_nodes[idx].ds_queue;
	}

	return -1;
}

/*
	get queue index by ip range v4.
*/
int getQueueIndexfromiprangev4(__be32 ipaddr, int up_dw)
{
	int idx = 0;
	struct dsnode *p_dataspeed_nodes = NULL;

	if ( DSLIMIT_UP == up_dw )
		p_dataspeed_nodes = gk_dataspeed_nodes_up;
	else if ( DSLIMIT_DW == up_dw )
		p_dataspeed_nodes = gk_dataspeed_nodes_dw;
	else
		return -1;

	for ( idx = 0; idx < DSLIMIT_MAX_NODES; idx ++ )
	{
		/* no need to check when found deactived rule. */
		if ( 0 == p_dataspeed_nodes[idx].ds_used )
			return -1;

		if ( ipaddr >= p_dataspeed_nodes[idx].ds_ip4addr[0]
			&& ipaddr <= p_dataspeed_nodes[idx].ds_ip4addr[1] )
			return p_dataspeed_nodes[idx].ds_queue;
	}

	return -1;
}

/*
	get queue index by ip range v6.
*/
int getQueueIndexfromiprangev6(__u8 *ip6addr, int up_dw)
{
	int idx = 0;
	struct dsnode *p_dataspeed_nodes = NULL;

	if ( DSLIMIT_UP == up_dw )
		p_dataspeed_nodes = gk_dataspeed_nodes_up;
	else if ( DSLIMIT_DW == up_dw )
		p_dataspeed_nodes = gk_dataspeed_nodes_dw;
	else
		return -1;

	for ( idx = 0; idx < DSLIMIT_MAX_NODES; idx ++ )
	{
		/* no need to check when found deactived rule. */
		if ( 0 == p_dataspeed_nodes[idx].ds_used )
			return -1;

		if ( 0 == memcmp(ip6addr, p_dataspeed_nodes[idx].ds_ip6addr, 16) )
			return p_dataspeed_nodes[idx].ds_queue;
	}

	return -1;
}


/*
	remark qos queue index
*/
int remarkQueueIndexfromDSNodes( struct sk_buff *skb, int up_dw )
{
	char *pname = NULL;
	int lanidx = 0, queue_index = 0, vlanid = 0;
	struct vlan_hdr *vhdr = NULL;
	struct iphdr *iph = NULL;
	struct ipv6hdr *ip6h = NULL;
	struct pppoe_hdr *ppph = NULL;
	unsigned char *pppoe_h = NULL;
	int ds_iptype = 0, ds_mode = 0;
	__be32 check_addr = 0;
	__u8 check_u6_addr8[16] = {0};
	__be16 ppp_proto = 0;
#define PPP_IP		0x21	/* Internet Protocol */
#define PPP_IPV6	0x57	/* Internet Protocol Version 6 */

	if ( !skb )
		return -1;

	pname = skb->dev->name;

	/* disabled, no need to remark. */
	if ( DSLIMIT_UP == up_dw )
		ds_mode = dsmode_up;
	else if ( DSLIMIT_DW == up_dw )
		ds_mode = dsmode_dw;
	else
		return -1;
	
	if ( 0 == ds_mode )
		return 0;

	/* base interface mode. */
	if ( DSLIMIT_MODE_IFACE == ds_mode )
	{
		/* ex. eth0.1 */
		if ( 'e' == pname[0]
			&& '.' == pname[4] )
		{
			/* clear queue mark. */
			if ( DSLIMIT_UP == up_dw )
				skb->mark &= ~(0xf0);
			else if ( DSLIMIT_DW == up_dw )
				skb->vlan_tag_flag &= (~DATASPEED_LIMIT_ENABLE);

			lanidx = pname[5] - '0';
			queue_index = getQueueIndexfromIface(lanidx, up_dw);
			if ( queue_index <= 0 )
				return -1;
		}
		/* ex. ra0 */
		else if ( 'r' == pname[0]
			&& 'a' == pname[1] )
		{
			/* clear queue mark. */
			if ( DSLIMIT_UP == up_dw )
				skb->mark &= ~(0xf0);
			else if ( DSLIMIT_DW == up_dw )
				return -1; /* not support for wlan interface. */

			lanidx = pname[2] - '0' + 5;
			queue_index = getQueueIndexfromIface(lanidx, up_dw);
			if ( queue_index <= 0 )
				return -1;
		}
		else
			return 1; /* ignore */
	}
	/* base vlan id. */
	else if ( DSLIMIT_MODE_VLANID == ds_mode )
	{
		/* ex. eth0.1,  ra0  */
		if ( ('e' == pname[0] && '.' == pname[4])
			|| ('r' == pname[0] && 'a' == pname[1]) )
		{
			if ( DSLIMIT_UP == up_dw )
				skb->mark &= ~(0xf0); /* clear queue mark. */
			else if ( DSLIMIT_DW == up_dw )
			{
				if ('r' == pname[0] && 'a' == pname[1])
					return -1; /* not support for wlan interface. */

				skb->vlan_tag_flag &= (~DATASPEED_LIMIT_ENABLE);
			}

			/* tagged pkts. */
			if ( htons(ETH_P_8021Q) == skb->protocol )
			{
				vhdr = (struct vlan_hdr *)(skb_mac_header(skb) + ETH_HLEN);
				vlanid = VTAG_GET_VID(vhdr->h_vlan_TCI);
				queue_index = getQueueIndexfromvlanid(vlanid, up_dw);
				if ( queue_index <= 0 )
					return -1;
			}
			else /* untagged pkts. */
			{
				vlanid = DS_VLANID_UNTAG;
				queue_index = getQueueIndexfromvlanid(vlanid, up_dw);
				if ( queue_index <= 0 )
					return -1;
			}
		}
		else
			return 1; /* ignore */
	}
	/* base ip range. */
	else if ( DSLIMIT_MODE_IPRANGE == ds_mode )
	{
		/* ex. eth0.1,  ra0  */
		if ( ('e' == pname[0] && '.' == pname[4])
			|| ('r' == pname[0] && 'a' == pname[1]) )
		{
			if ( DSLIMIT_UP == up_dw )
				skb->mark &= ~(0xf0); /* clear queue mark. */
			else if ( DSLIMIT_DW == up_dw )
			{
				if ('r' == pname[0] && 'a' == pname[1])
					return -1; /* not support for wlan interface. */

				skb->vlan_tag_flag &= (~DATASPEED_LIMIT_ENABLE);
			}

			/* IPv4 */
			if ( htons(ETH_P_IP) == skb->protocol )
			{
				iph = ip_hdr(skb);
				if ( DSLIMIT_UP == up_dw )
					check_addr = iph->saddr;
				else if ( DSLIMIT_DW == up_dw )
					check_addr = iph->daddr;
				ds_iptype = DS_IPTYPE_IPV4;
			}
			/* IPv6 */
			else if ( htons(ETH_P_IPV6) == skb->protocol )
			{
				ip6h = ipv6_hdr(skb);
				if ( DSLIMIT_UP == up_dw )
					memcpy(check_u6_addr8, ip6h->saddr.s6_addr,
						sizeof(check_u6_addr8));
				else if ( DSLIMIT_DW == up_dw )
					memcpy(check_u6_addr8, ip6h->daddr.s6_addr,
						sizeof(check_u6_addr8));
				ds_iptype = DS_IPTYPE_IPV6;
			}
			/* PPP */
			else if ( htons(ETH_P_PPP_SES) == skb->protocol )
			{
				ppph = pppoe_hdr(skb);
				pppoe_h = (ppph + 1);
				ppp_proto = *(__be16*)(pppoe_h);
				pppoe_h += 2; /* skip ppp protocol */
				/* IPv4 */
				if ( PPP_IP == ppp_proto )
				{
					iph = (struct iphdr* )(pppoe_h);
					if ( DSLIMIT_UP == up_dw )
						check_addr = iph->saddr;
					else if ( DSLIMIT_DW == up_dw )
						check_addr = iph->daddr;
					ds_iptype = DS_IPTYPE_IPV4;
				}
				/* IPv6 */
				else if ( PPP_IPV6 == ppp_proto )
				{
					ip6h = (struct ipv6hdr* )(pppoe_h);
					if ( DSLIMIT_UP == up_dw )
						memcpy(check_u6_addr8, ip6h->saddr.s6_addr,
							sizeof(check_u6_addr8));
					else if ( DSLIMIT_DW == up_dw )
						memcpy(check_u6_addr8, ip6h->daddr.s6_addr,
							sizeof(check_u6_addr8));
					ds_iptype = DS_IPTYPE_IPV6;
				}
				else
					return -1; /* unmatch type */
			}
			else if( htons(ETH_P_8021Q) == skb->protocol ) /* tagged packets. */
			{
				/* IPv4 */
				if ( htons(ETH_P_IP) == __vlan_proto(skb) )
				{
					iph = (struct iphdr *)
						(skb_mac_header(skb) + VLAN_ETH_HLEN);
					if ( DSLIMIT_UP == up_dw )
						check_addr = iph->saddr;
					else if ( DSLIMIT_DW == up_dw )
						check_addr = iph->daddr;
					ds_iptype = DS_IPTYPE_IPV4;
				}
				/* IPv6 */
				else if ( htons(ETH_P_IPV6) == __vlan_proto(skb) )
				{
					ip6h = 
						(struct iphdr *)(skb_mac_header(skb) + VLAN_ETH_HLEN);
					if ( DSLIMIT_UP == up_dw )
						memcpy(check_u6_addr8, ip6h->saddr.s6_addr,
							sizeof(check_u6_addr8));
					else if ( DSLIMIT_DW == up_dw )
						memcpy(check_u6_addr8, ip6h->daddr.s6_addr,
							sizeof(check_u6_addr8));
					ds_iptype = DS_IPTYPE_IPV6;
				}
				/* PPP */
				else if ( htons(ETH_P_PPP_SES) == __vlan_proto(skb) )
				{
					ppph = (struct pppoe_hdr*)
						(skb_mac_header(skb) + VLAN_ETH_HLEN);
					pppoe_h = (ppph + 1);
					ppp_proto = *(__be16*)(pppoe_h);
					pppoe_h += 2; /* skip ppp protocol */
					/* IPv4 */
					if ( PPP_IP == ppp_proto )
					{
						iph = (struct iphdr* )(pppoe_h);
						if ( DSLIMIT_UP == up_dw )
							check_addr = iph->saddr;
						else if ( DSLIMIT_DW == up_dw )
							check_addr = iph->daddr;
						ds_iptype = DS_IPTYPE_IPV4;
					}
					/* IPv6 */
					else if ( PPP_IPV6 == ppp_proto )
					{
						ip6h = (struct ipv6hdr* )(pppoe_h);
						if ( DSLIMIT_UP == up_dw )
							memcpy(check_u6_addr8, ip6h->saddr.s6_addr,
								sizeof(check_u6_addr8));
						else if ( DSLIMIT_DW == up_dw )
							memcpy(check_u6_addr8, ip6h->daddr.s6_addr,
								sizeof(check_u6_addr8));
						ds_iptype = DS_IPTYPE_IPV6;
					}
					else
						return -1; /* unmatch type */
				}
			}
		}
		else
			return 1; /* ignore */

		if ( DS_IPTYPE_IPV4 == ds_iptype )
			queue_index = getQueueIndexfromiprangev4(check_addr, up_dw);
		else if ( DS_IPTYPE_IPV6 == ds_iptype )
			queue_index = getQueueIndexfromiprangev6(check_u6_addr8, up_dw);
		else
			return -1;

		if ( queue_index <= 0 )
			return -1;
	}
	else
		return -1; /* unmatch mode */

	/* remark the queue. */
	if ( DSLIMIT_UP == up_dw )
	{
		skb->mark |= ((queue_index & 0x0f) << 4);
		/*printk("\n [up]queue remark [%s]-[%x]\n", 
					skb->dev->name, skb->mark & 0xf0);*/
	}
	else if ( DSLIMIT_DW == up_dw )
	{
		skb->vlan_tag_flag |= (DATASPEED_LIMIT_ENABLE);
		setDownQueueID(skb->pon_mark, (queue_index & 0x0f));
		/*printk("\n [down]queue remark [%s]-[%x]\n", skb->dev->name,
								getDownQueueID(skb->pon_mark));*/
	}

	return 0;
}

/*
	remark downstrem queue index
*/
int dataspeed_limit_queue_hook(struct sk_buff *skb)
{
	uint16_t *stag = NULL;

	if ( !(skb->vlan_tag_flag & DATASPEED_LIMIT_ENABLE) )
		return 1; /* no need to change. */

	stag = (uint16_t*)(skb->data + 12);
	*stag = (*stag &= 0x0fff); /* clean FUP and UPRI */
	*stag = (*stag |= 0x8000); /* set FUP = 1,force PPE user priority */
	/* set queue index */
	*stag = (*stag |= (getDownQueueID(skb->pon_mark) << 12));

	/*printk("\n queue change [%s]-[%x]\n", skb->dev->name,
							getDownQueueID(skb->pon_mark));*/

	return 0;
}
EXPORT_SYMBOL(dataspeed_limit_queue_hook);
#endif

/* note: already called with rcu_read_lock */
__IMEM int br_handle_frame_finish(struct sk_buff *skb)
{
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	struct net_bridge_port *p = br_port_get_rcu(skb->dev);
	struct net_bridge *br;
	struct net_bridge_fdb_entry *dst;
#if defined (TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND)
	struct net_bridge_fdb_entry *dst2;
#endif
	struct net_bridge_mdb_entry *mdst;
	struct sk_buff *skb2;

	struct net_data_s net_data;
    int ret = 0;
	
#if defined(TCSUPPORT_XPON_IGMP)
#if 0
	typeof (xpon_bridge_incoming_hook)	xpon_bridge_incoming;
#endif
#endif
#if defined(TCSUPPORT_CT_VLAN_BIND)
	unsigned int mark = 0;
#endif
#if defined(TCSUPPORT_RA_HWNAT)
	skb->bridge_flag = 0;
#endif

	if (!p || p->state == BR_STATE_DISABLED)
		goto drop;

	net_data.pskb = &skb;
	ret = ECNT_BR_INPUT_HOOK(ECNT_BR_HANDLE_FRAME,&net_data);
	if (ECNT_RETURN == ret)
	{
		goto out;
	}
	else if (ECNT_RETURN_DROP == ret)
	{
		goto drop;
	}
	
	
	/* insert into forwarding database after filtering to avoid spoofing */
	br = p->br;

	/*for dasan feature,if mac numbers of mac table > limit,drop packet*/
	#ifdef TCSUPPORT_BRIDGE_MAC_LIMIT
	br_fdb_need_flush(br);
	if((!br_fdb_mac_exist(br,eth_hdr(skb)->h_source,skb->dev->name))&&
		(br_fdb_judge_mac_num(br, skb->dev->name)))	
		goto drop;
	#endif
	
	br_fdb_update(br, p, eth_hdr(skb)->h_source, skb);

#if defined(TCSUPPORT_XPON_IGMP)

#if 0
	int ret;
	xpon_bridge_incoming = rcu_dereference(xpon_bridge_incoming_hook);
	if(is_multicast_ether_addr(dest)&& xpon_bridge_incoming) 
	{
		ret = xpon_bridge_incoming(skb,1);
		if (ret>0)
		{
//			BR_INPUT_SKB_CB(skb)->brdev = br->dev;
//			br_pass_frame_up(skb);
			goto drop;	
		}
	}
#endif

    //downstream multicast operation 
    if(is_multicast_ether_addr(dest) && xpon_sfu_down_multicast_incoming_hook)
    {
        ret = xpon_sfu_down_multicast_incoming_hook(skb, 1);
        if (ret > 0 )
        {
            goto drop;  
        }
    }
    
    //send  upstream multicast to ANI, jump kernel multicast 
    if(is_multicast_ether_addr(dest) && xpon_sfu_up_send_multicast_frame_hook)
    {
        ret = xpon_sfu_up_send_multicast_frame_hook(skb, 1);
        if (ret > 0 )
        {
            goto drop;  
        }
    }
#endif
	if (is_multicast_ether_addr(dest) &&
	    br_multicast_rcv(br, p, skb))
		goto drop;

	if (p->state == BR_STATE_LEARNING)
		goto drop;

	BR_INPUT_SKB_CB(skb)->brdev = br->dev;

	/* The packet skb2 goes to the local host (NULL to skip). */
	skb2 = NULL;

	if (br->dev->flags & IFF_PROMISC)
		skb2 = skb;

	dst = NULL;

#if defined(TCSUPPORT_CT_DS_LIMIT)
	if ( BR_STATE_FORWARDING == p->state )
		remarkQueueIndexfromDSNodes(skb, DSLIMIT_UP);
#endif

	if (is_multicast_ether_addr(dest)) {
		mdst = br_mdb_get(br, skb);
		if (mdst || BR_INPUT_SKB_CB_MROUTERS_ONLY(skb)) {
			if ((mdst && !hlist_unhashed(&mdst->mglist)) ||
			    br_multicast_is_router(br))
				skb2 = skb;
#if defined(CONFIG_PORT_BINDING) || defined(TCSUPPORT_PORTBIND)
#if defined(TCSUPPORT_CT)
			if (portbind_sw_hook && (portbind_sw_hook() == 1)) 
			{
#if defined(TCSUPPORT_CT_VLAN_BIND)
				mark = (skb->mark & 0xF0000000) >> 28;
				/* From this LAN port and it is vlan binded.*/
				if (vlanbind_active_hook && (vlanbind_active_hook() != 0) 
					&& vlanbind_entry_active_hook &&(vlanbind_entry_active_hook(mark) != 0))
				{
					br_multicast_forward(mdst, skb, skb2);
				}
				else
#endif
				br_multicast_pb_forward(mdst, skb, skb2);
			}
#else
#if defined(TCSUPPORT_FTP_THROUGHPUT)
			if (portbind_sw_hook) 
#else
#if defined(TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND)
			if (portbind_sw_prior_hook && (portbind_sw_prior_hook(skb) == 1)) 
#else
			if (portbind_sw_hook && (portbind_sw_hook() == 1)) 
#endif
#endif
			{	
				br_multicast_pb_forward(mdst, p, skb, skb2);
			}
#endif
			else {
				br_multicast_forward(mdst, skb, skb2);
			}
#else			
			br_multicast_forward(mdst, skb, skb2);
#endif
			skb = NULL;
			if (!skb2)
				goto out;
		} else
#ifdef TCSUPPORT_IGMPSNOOPING_ENHANCE
		{
#endif
			skb2 = skb;
#ifdef TCSUPPORT_IGMPSNOOPING_ENHANCE
#if !defined(TCSUPPORT_FWC_MCST)
			/*drop the muticast packet if it is not in the muticast table*/
			if(br_multicast_should_drop(br, skb)){
#if defined(TCSUPPORT_XPON_IGMP)
				if (wan_multicast_drop_hook)  return wan_multicast_drop_hook(skb);
#endif
				br_multicast_dump_packet_info(skb, 1);
				goto up;
			}
#endif
		}	
#endif	
		br->dev->stats.multicast++;
	} else
	{
#if defined(CONFIG_PORT_BINDING) || defined(TCSUPPORT_PORTBIND)
#if defined(TCSUPPORT_CT)
		if (portbind_sw_hook && (portbind_sw_hook() == 1)) 
		{
#if defined(TCSUPPORT_CT_VLAN_BIND)
			mark = (skb->mark & 0xF0000000) >> 28;
			/* From this LAN port and it is vlan binded.*/
			if (vlanbind_active_hook && (vlanbind_active_hook() != 0) 
				&& vlanbind_entry_active_hook &&(vlanbind_entry_active_hook(mark) != 0))
			{		
				dst = __br_fdb_get(br, dest);
			}
			else
#endif
			dst = __br_fdb_pb_get(skb,br, dest);
		}
#else
#if defined(TCSUPPORT_FTP_THROUGHPUT)
		if (portbind_sw_hook) 
#else
#if defined(TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND)
		if (portbind_sw_prior_hook && (portbind_sw_prior_hook(skb) == 1)) 
#else
		if (portbind_sw_hook && (portbind_sw_hook() == 1)) 
#endif
#endif
		{
		//printk("In port is %s\n", p->dev->name);
			dst = __br_fdb_pb_get(br, p, dest);
		}
#endif
		else {
			dst = __br_fdb_get(br, dest);
		}
#else
		dst = __br_fdb_get(br, dest);
#endif
#if defined(TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND)
	dst2 = dst;
    if (dst->dst->dev->name[0] == 'n')
		dst = vlan_fdb_get(skb, br, dest); //get dest by portbind/routepolicy/vlan
	else
		dst = NULL;
	if(dst == NULL)
		dst = dst2; //not find, use orignal br_get result;
#endif
		if (dst && dst->is_local) {
		skb2 = skb;
		/* Do not forward the packet since it's local. */
		skb = NULL;
	}
	}

	if (skb) {
#if defined(TCSUPPORT_RA_HWNAT)	
		skb->bridge_flag = 1;
#endif
		if (dst)
		{
			br_forward(dst->dst, skb, skb2);
		}
		else
		{
#if defined(CONFIG_PORT_BINDING) || defined(TCSUPPORT_PORTBIND)
#if defined(TCSUPPORT_CT)
			if (portbind_sw_hook && (portbind_sw_hook() == 1)) 
			{					
#if defined(TCSUPPORT_CT_VLAN_BIND)
				mark = (skb->mark & 0xF0000000) >> 28;
				/* From this LAN port and it is vlan binded.*/
				if (vlanbind_active_hook && (vlanbind_active_hook() != 0) 
					&& vlanbind_entry_active_hook &&(vlanbind_entry_active_hook(mark) != 0))
				{
					br_flood_forward(br, skb, skb2);
				}
				else
#endif
				br_flood_pb_forward(br, skb, skb2);
			}
#else
		#if defined(TCSUPPORT_FTP_THROUGHPUT)
			if (portbind_sw_hook) 
		#else
#if defined(TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND)
			if (portbind_sw_prior_hook && (portbind_sw_prior_hook(skb) == 1)) 
#else
			if (portbind_sw_hook && (portbind_sw_hook() == 1)) 
		#endif
#endif
			{	
				br_flood_pb_forward(br, p, skb, skb2);
			}
#endif
			else {
				br_flood_forward(br, skb, skb2);
			}
#else
			br_flood_forward(br, skb, skb2);
#endif
		}
	}
#ifdef TCSUPPORT_IGMPSNOOPING_ENHANCE	
up:
#endif
	if (skb2)
		return br_pass_frame_up(skb2);

out:
	return 0;
drop:
	kfree_skb(skb);
	goto out;
}

/* note: already called with rcu_read_lock */
static int br_handle_local_finish(struct sk_buff *skb)
{
	struct net_bridge_port *p = br_port_get_rcu(skb->dev);

	if(p) {
		br_fdb_update(p->br, p, eth_hdr(skb)->h_source, skb);
	}

	return 0;	 /* process further */
}

/* Does address match the link local multicast address.
 * 01:80:c2:00:00:0X
 */
static inline int is_link_local(const unsigned char *dest)
{
	__be16 *a = (__be16 *)dest;
	static const __be16 *b = (const __be16 *)br_group_address;
	static const __be16 m = cpu_to_be16(0xfff0);

	return ((a[0] ^ b[0]) | (a[1] ^ b[1]) | ((a[2] ^ b[2]) & m)) == 0;
}

/*
 * Return NULL if skb is handled
 * note: already called with rcu_read_lock
 */
__IMEM struct sk_buff *br_handle_frame(struct sk_buff *skb)
{
	struct net_bridge_port *p;
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	int (*rhook)(struct sk_buff *skb);

	#if defined(TCSUPPORT_VLAN_TAG) //Byron
	struct vlan_hdr *vhdr;
	u16 vlan_id; // vlan_id
	u16 vlan_tci; // Vlan field
	#endif
	
	if (skb->pkt_type == PACKET_LOOPBACK)
		return skb;

	if (!is_valid_ether_addr(eth_hdr(skb)->h_source))
		goto drop;
#ifdef TCSUPPORT_INIC_HOST
	if(skb->protocol == ETH_P_ROM) {
		return skb;	/* continue processing */
	}
#endif

	skb = skb_share_check(skb, GFP_ATOMIC);
	if (!skb)
		return NULL;

	p = br_port_get_rcu(skb->dev);
	if(!p) {
		goto drop;
	}
#ifndef TCSUPPORT_FWC_ENV
	if (unlikely(is_link_local(dest))) {
		/* Pause frames shouldn't be passed up by driver anyway */
		if (skb->protocol == htons(ETH_P_PAUSE))
			goto drop;

		/* If STP is turned off, then forward */
		if (p->br->stp_enabled == BR_NO_STP && dest[5] == 0)
			goto forward;

		if (NF_HOOK(NFPROTO_BRIDGE, NF_BR_LOCAL_IN, skb, skb->dev,
			    NULL, br_handle_local_finish))
			return NULL;	/* frame consumed by filter */
		else
			return skb;	/* continue processing */
	}
#endif

#if defined(TCSUPPORT_VLAN_TAG)
	if((skb->protocol==htons(ETH_P_8021Q)) && (skb->dev->name[0]=='n')){

		vhdr = (struct vlan_hdr *)skb->data;
		vlan_tci = ntohs(vhdr->h_vlan_TCI);
		vlan_id = vlan_tci & VLAN_VID_MASK;


		// if 8021q active & vlan_id match to do remove
		// when 8021q active only vlan_id match from WAN to LAN
		// this procedure will handle 8021q de-active. VLAN pkt can transparent from WAN to LAN, Byron
		if(skb->vlan_tags[0]==vlan_id){
			if (remove_vtag_hook) {
				if (remove_vtag_hook(skb, skb->dev) == -1) {
					/* must free skb !! */					
					kfree_skb(skb);					
					return NULL;
				}
			}
		}
	}
#endif
forward:
	switch (p->state) {
	case BR_STATE_FORWARDING:
#if !defined(TCSUPPORT_FWC_ENV)
		/*_____________________________________________
		** remark packet from different lan interfac,  
		** use the highest 4 bits.
		**
		** eth0	  	0x10000000
		** eth0.1 	0x10000000
		** eth0.2 	0x20000000
		** eth0.3 	0x30000000
		** eth0.4 	0x40000000
		** ra0	  	0x50000000
		** ra1    	0x60000000
		** ra2    	0x70000000
		** ra3    	0x80000000
		** usb0   	0x90000000
		** wds0~3  	0xA0000000
		** rai0   	0xB0000000
		** rai1   	0xC0000000
		** rai2   	0xD0000000
		** rai3   	0xE0000000
		**_________________________________________
		*/
#define		WLAN_DEV_OFFSET 		5
#ifdef TCSUPPORT_WLAN_AC
#define		WLAN_AC_DEV_OFFSET 		11
#endif
#define		USB_DEV_OFFSET		9
#define		WDS_DEV_OFFSET		10
#define		DEV_OFFSET			28
		switch (skb->dev->name[0]) {
			case 'e':
		#ifdef TCSUPPORT_TC2031
				/* device name format must be eth0 */
				skb->mark |= 0x10000000;
		#else
                //single lan port
                if(!strcmp(skb->dev->name, "eth0"))
                {
                    skb->mark |= 0x10000000;
                }

				/* device name format must be eth0.x */
				if (skb->dev->name[4] == '.')
					skb->mark |= (skb->dev->name[5] - '0') << DEV_OFFSET;
		#endif
				break;
			case 'r':
		#ifdef TCSUPPORT_WLAN_AC
				if (skb->dev->name[2] == 'i')
					/* device name must be raix */
					skb->mark |= ((skb->dev->name[3] - '0') + WLAN_AC_DEV_OFFSET) << DEV_OFFSET;
				else
		#endif
				/* device name must be rax */
				skb->mark |= ((skb->dev->name[2] - '0') + WLAN_DEV_OFFSET) << DEV_OFFSET;
				break;
			case 'u':
				/* device name must be usbx */
				skb->mark |= ((skb->dev->name[3] - '0') + USB_DEV_OFFSET) << DEV_OFFSET;
				break;
			case 'w':
				/* device name must be wdsx */
				skb->mark |= (WDS_DEV_OFFSET) << DEV_OFFSET;
				break;
			default:
				break;
		}
#endif
		rhook = rcu_dereference(br_should_route_hook);
		if (rhook != NULL) {
			if (rhook(skb))
				return skb;
			dest = eth_hdr(skb)->h_dest;
		}
		/* fall through */
	case BR_STATE_LEARNING:
		if (!compare_ether_addr(p->br->dev->dev_addr, dest))
			skb->pkt_type = PACKET_HOST;

		NF_HOOK(NFPROTO_BRIDGE, NF_BR_PRE_ROUTING, skb, skb->dev, NULL,
			br_handle_frame_finish);
		break;
	default:
drop:
		kfree_skb(skb);
	}
	return NULL;
}
