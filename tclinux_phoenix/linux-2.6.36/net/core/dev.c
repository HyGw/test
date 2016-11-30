/*
 * 	NET3	Protocol independent device support routines.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 *	Derived from the non IP parts of dev.c 1.0.19
 * 		Authors:	Ross Biro
 *				Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *				Mark Evans, <evansmp@uhura.aston.ac.uk>
 *
 *	Additional Authors:
 *		Florian la Roche <rzsfl@rz.uni-sb.de>
 *		Alan Cox <gw4pts@gw4pts.ampr.org>
 *		David Hinds <dahinds@users.sourceforge.net>
 *		Alexey Kuznetsov <kuznet@ms2.inr.ac.ru>
 *		Adam Sulmicki <adam@cfar.umd.edu>
 *              Pekka Riikonen <priikone@poesidon.pspt.fi>
 *
 *	Changes:
 *              D.J. Barrow     :       Fixed bug where dev->refcnt gets set
 *              			to 2 if register_netdev gets called
 *              			before net_dev_init & also removed a
 *              			few lines of code in the process.
 *		Alan Cox	:	device private ioctl copies fields back.
 *		Alan Cox	:	Transmit queue code does relevant
 *					stunts to keep the queue safe.
 *		Alan Cox	:	Fixed double lock.
 *		Alan Cox	:	Fixed promisc NULL pointer trap
 *		????????	:	Support the full private ioctl range
 *		Alan Cox	:	Moved ioctl permission check into
 *					drivers
 *		Tim Kordas	:	SIOCADDMULTI/SIOCDELMULTI
 *		Alan Cox	:	100 backlog just doesn't cut it when
 *					you start doing multicast video 8)
 *		Alan Cox	:	Rewrote net_bh and list manager.
 *		Alan Cox	: 	Fix ETH_P_ALL echoback lengths.
 *		Alan Cox	:	Took out transmit every packet pass
 *					Saved a few bytes in the ioctl handler
 *		Alan Cox	:	Network driver sets packet type before
 *					calling netif_rx. Saves a function
 *					call a packet.
 *		Alan Cox	:	Hashed net_bh()
 *		Richard Kooijman:	Timestamp fixes.
 *		Alan Cox	:	Wrong field in SIOCGIFDSTADDR
 *		Alan Cox	:	Device lock protection.
 *		Alan Cox	: 	Fixed nasty side effect of device close
 *					changes.
 *		Rudi Cilibrasi	:	Pass the right thing to
 *					set_mac_address()
 *		Dave Miller	:	32bit quantity for the device lock to
 *					make it work out on a Sparc.
 *		Bjorn Ekwall	:	Added KERNELD hack.
 *		Alan Cox	:	Cleaned up the backlog initialise.
 *		Craig Metz	:	SIOCGIFCONF fix if space for under
 *					1 device.
 *	    Thomas Bogendoerfer :	Return ENODEV for dev_open, if there
 *					is no device open function.
 *		Andi Kleen	:	Fix error reporting for SIOCGIFCONF
 *	    Michael Chastain	:	Fix signed/unsigned for SIOCGIFCONF
 *		Cyrus Durgin	:	Cleaned for KMOD
 *		Adam Sulmicki   :	Bug Fix : Network Device Unload
 *					A network device unload needs to purge
 *					the backlog queue.
 *	Paul Rusty Russell	:	SIOCSIFNAME
 *              Pekka Riikonen  :	Netdev boot-time settings code
 *              Andrew Morton   :       Make unregister_netdevice wait
 *              			indefinitely on dev->refcnt
 * 		J Hadi Salim	:	- Backlog queue sampling
 *				        - netif_rx() feedback
 */

#include <asm/uaccess.h>
#include <asm/system.h>
#include <linux/bitops.h>
#include <linux/capability.h>
#include <linux/cpu.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/hash.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/mutex.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/socket.h>
#include <linux/sockios.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/if_ether.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/notifier.h>
#include <linux/skbuff.h>
#include <net/net_namespace.h>
#include <net/sock.h>
#include <linux/rtnetlink.h>
#if defined(CONFIG_IMQ) || defined(CONFIG_IMQ_MODULE)
#include <linux/imq.h>
#endif
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <net/dst.h>
#include <net/pkt_sched.h>
#include <net/checksum.h>
#include <net/xfrm.h>
#include <linux/highmem.h>
#include <linux/init.h>
#include <linux/kmod.h>
#include <linux/module.h>
#include <linux/netpoll.h>
#include <linux/rcupdate.h>
#include <linux/delay.h>
#include <net/wext.h>
#include <net/iw_handler.h>
#include <asm/current.h>
#include <linux/audit.h>
#include <linux/dmaengine.h>
#include <linux/err.h>
#include <linux/ctype.h>
#include <linux/if_arp.h>
#include <linux/if_vlan.h>
#include <linux/ip.h>
#include <net/ip.h>
#include <linux/ipv6.h>
#include <linux/in.h>
#include <linux/jhash.h>
#include <linux/random.h>
#include <trace/events/napi.h>
#include <linux/pci.h>

#include "net-sysfs.h"
#ifdef CONFIG_QOS
#include <linux/ip.h>
#include <linux/if_pppox.h>
#ifdef CONFIG_IPV6
#include <linux/ipv6.h>
#include <net/dsfield.h>
#endif
#include <linux/netfilter.h>
#include <linux/if_vlan.h>
#include <linux/qos_type.h>
#endif

#ifdef TCSUPPORT_XPON_HAL_API_EXT
#include "../../../modules/private/xpon/inc/xmcs/pon-defined.h"
#endif
#include <ecnt_hook/ecnt_hook_net.h>


/* Instead of increasing this, you should create a hash table. */
#define MAX_GRO_SKBS 8

/* This should be increased if a protocol with a bigger head is added. */
#define GRO_MAX_HEAD (MAX_HEADER + 128)

/*
 *	The list of packet types we will receive (as opposed to discard)
 *	and the routines to invoke.
 *
 *	Why 16. Because with 16 the only overlap we get on a hash of the
 *	low nibble of the protocol value is RARP/SNAP/X.25.
 *
 *      NOTE:  That is no longer true with the addition of VLAN tags.  Not
 *             sure which should go first, but I bet it won't make much
 *             difference if we are running VLANs.  The good news is that
 *             this protocol won't be in the list unless compiled in, so
 *             the average user (w/out VLANs) will not be adversely affected.
 *             --BLG
 *
 *		0800	IP
 *		8100    802.1Q VLAN
 *		0001	802.3
 *		0002	AX.25
 *		0004	802.2
 *		8035	RARP
 *		0005	SNAP
 *		0805	X.25
 *		0806	ARP
 *		8137	IPX
 *		0009	Localtalk
 *		86DD	IPv6
 */

#define PTYPE_HASH_SIZE	(16)
#define PTYPE_HASH_MASK	(PTYPE_HASH_SIZE - 1)
#if !defined(TCSUPPORT_CT)	
#if defined(TCSUPPORT_BRIDGE_FASTPATH)
int (*hook_bridge_shortcut_process)(struct net_device *net_dev, struct sk_buff *skb);
EXPORT_SYMBOL(hook_bridge_shortcut_process);
void (*hook_dev_shortcut_learn)(struct sk_buff *skb, struct net_device *dev);
EXPORT_SYMBOL(hook_dev_shortcut_learn);
#endif
#endif

#if defined(TCSUPPORT_CT_VOIP_QOS)
#define RTP_BIND_WAN_INDEX		"tc3162/RtpBindWanIdx"
#define RTP_PRIORITY		"tc3162/RtpPriority"
#endif

#if defined(TCSUPPORT_CMCC)
unsigned int etherTypeFilterArray[8];
#define ETHERTYPEFILTER_ARRAY_PATH 	"tc3162/ethertypefilter_array"
#endif

#if defined(TC_SUPPORT_3G) && defined(TR068_LED)
void (*Dongle_InternetLed_hook)(void);
EXPORT_SYMBOL(Dongle_InternetLed_hook);
#endif
#ifdef TCSUPPORT_USB_HOST_LED
#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520) || defined(TCSUPPORT_CPU_MT7505)
void (*Usb_Led_Flash_Op_hook)(unsigned int opmode ,unsigned int phyport);
EXPORT_SYMBOL(Usb_Led_Flash_Op_hook);
#endif
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
void (*Simcard_Led_hook)(unsigned int type ,unsigned int status) = NULL;
EXPORT_SYMBOL(Simcard_Led_hook);
#endif

int (*I2CWriterPtr)(u8 DevAddr, u8 WordAddr, u8* data_value, u8 data_len);
int (*I2CReaderPtr)(u8 DevAddr, u8 WordAddr, u8* data_value, u8 data_len);
EXPORT_SYMBOL(I2CWriterPtr);
EXPORT_SYMBOL(I2CReaderPtr);

#ifdef CONFIG_SMUX
int (*smux_pkt_recv_hook)(struct sk_buff *skb, 
                  struct net_device *dev,
                  struct net_device *rdev);
EXPORT_SYMBOL(smux_pkt_recv_hook);
#endif

#ifdef TCSUPPORT_DOWNSTREAM_QOS
/*use for set voip rx port in application, shnwind add 20110215.*/
unsigned short int voip_rx_port[VOIP_RX_PORT_NUM] = {0};
EXPORT_SYMBOL(voip_rx_port);
char downstream_qos_enable = 0;
EXPORT_SYMBOL(downstream_qos_enable);
#endif

static DEFINE_SPINLOCK(ptype_lock);
static struct list_head ptype_base[PTYPE_HASH_SIZE] __read_mostly;
static struct list_head ptype_all __read_mostly;	/* Taps */
#ifdef TCSUPPORT_VLAN_TAG
int (*remove_vtag_hook)(struct sk_buff *skb, struct net_device *dev);
int (*insert_vtag_hook)(struct sk_buff **pskb);
//#if !defined(TCSUPPORT_FTP_THROUGHPUT)
int (*check_vtag_hook)(void);
//#endif
int (*get_vtag_hook)(struct net_device *dev, struct sk_buff *skb);
/*TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND*/
struct net_device* (*tcvlan_get_outdev_hook)(struct sk_buff* skb);
/*END TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND*/
/*TCSUPPORT_CPU_PERFORMANCE_TEST---start---*/
int (*get_pvid_by_dev_name_hook)(char *devName);
int (*get_wan_index_by_vlan_hook)(unsigned char vlanLayer, unsigned short vlanId);
/*TCSUPPORT_CPU_PERFORMANCE_TEST---end---*/
EXPORT_SYMBOL(remove_vtag_hook);
EXPORT_SYMBOL(insert_vtag_hook);
//#if !defined(TCSUPPORT_FTP_THROUGHPUT)
EXPORT_SYMBOL(check_vtag_hook);
//#endif
EXPORT_SYMBOL(get_vtag_hook);
/*TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND*/
EXPORT_SYMBOL(tcvlan_get_outdev_hook);
/*END TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND*/
/*TCSUPPORT_CPU_PERFORMANCE_TEST---start---*/
EXPORT_SYMBOL(get_pvid_by_dev_name_hook);
EXPORT_SYMBOL(get_wan_index_by_vlan_hook);
/*TCSUPPORT_CPU_PERFORMANCE_TEST---end---*/
#endif

#if defined(TCSUPPORT_CPU_PERFORMANCE_TEST)
#define FTP_PORT_NUM 10
unsigned int test_ftp_port[FTP_PORT_NUM]={0};
EXPORT_SYMBOL(test_ftp_port);

int delTestFtpPort(unsigned short port)
{
	int i=0;
	for(i=0; i<FTP_PORT_NUM; i++)
	{
		if(test_ftp_port[i] == port)
		{
			test_ftp_port[i] = 0;
			return 0;
		}
	}
	
	return 0;
}
EXPORT_SYMBOL(delTestFtpPort);

int addTestFtpPort(unsigned short port)
{
	int i=0;
	
	for(i=0; i<FTP_PORT_NUM; i++)
	{
		if(test_ftp_port[i] == 0)
		{
			test_ftp_port[i] = port;
		
		//	printk("open ftp port: %d\n", test_ftp_port[i]);		
			break;
		}
	}
	return 0;
}
EXPORT_SYMBOL(addTestFtpPort);


#endif


int (*portbind_check_bind_lan2)(int bind_index);
EXPORT_SYMBOL(portbind_check_bind_lan2);
#if defined(TCSUPPORT_CT_DS_LIMIT)
extern 
int remarkQueueIndexfromDSNodes( struct sk_buff *skb, int up_dw );
#endif


/*TCSUPPORT_VLAN_PASSTHROUGH*/
int (*check_vlan_range_hook)(struct net_device *dev, struct sk_buff *skb);
EXPORT_SYMBOL(check_vlan_range_hook);
/*END TCSUPPORT_VLAN_PASSTHROUGH*/


#if defined(TCSUPPORT_CT_VLAN_TAG)
u32 	vtag_free_rx_cnt = 0;
u32 	vtag_free_tx_cnt = 0;

int (*check_vtag_ct_hook)(void);
int (*check_vtag_restore_ct_hook)(void);
int (*store_vtag_ct_hook)(struct sk_buff *skb, struct net_device *dev);
int (*restore_vtag_ct_hook)(struct sk_buff **pskb);
int (*handle_vtag_tx_ct_hook)(struct sk_buff **pskb);
int (*handle_vtag_rx_ct_hook)(struct sk_buff **pskb);

EXPORT_SYMBOL(check_vtag_ct_hook);
EXPORT_SYMBOL(check_vtag_restore_ct_hook);
EXPORT_SYMBOL(store_vtag_ct_hook);
EXPORT_SYMBOL(restore_vtag_ct_hook);
EXPORT_SYMBOL(handle_vtag_tx_ct_hook);
EXPORT_SYMBOL(handle_vtag_rx_ct_hook);
EXPORT_SYMBOL(vtag_free_rx_cnt);
EXPORT_SYMBOL(vtag_free_tx_cnt);
#endif

#ifdef CONFIG_QOS
#ifdef TCSUPPORT_SBTHROUGHPUT_ENHANCE
int tc_qos_switch = 0;
#endif
#endif

#if defined(TCSUPPORT_WAN2LAN_EXT)
u32 (*gswPbusRead_hook)(u32 pbus_addr);
int (*gswPbusWrite_hook)(u32 pbus_addr, u32 pbus_data);
int (*lanPortmap_hook)(int lanPort);
int (*lanPortmap_rev_hook)(int switchPort);
EXPORT_SYMBOL(gswPbusRead_hook);
EXPORT_SYMBOL(gswPbusWrite_hook);
EXPORT_SYMBOL(lanPortmap_hook);
EXPORT_SYMBOL(lanPortmap_rev_hook);
#endif
/* add for fix compile error */
int g_port_reverse_kernel = 0;
EXPORT_SYMBOL(g_port_reverse_kernel);


int (*xpon_store_upstream_igmp_vlan_tci_hook)(struct sk_buff *skb) = NULL;
int (*xpon_down_multicast_vlan_handle_hook)(struct sk_buff *skb) = NULL;
int (*xpon_upstream_vlan_recovery_by_dynlist_hook)(struct sk_buff* skb) = NULL;
int (*xpon_hgu_down_multicast_access_control_hook)(struct sk_buff* skb) = NULL;


EXPORT_SYMBOL(xpon_store_upstream_igmp_vlan_tci_hook);
EXPORT_SYMBOL(xpon_down_multicast_vlan_handle_hook);
EXPORT_SYMBOL(xpon_upstream_vlan_recovery_by_dynlist_hook);
EXPORT_SYMBOL(xpon_hgu_down_multicast_access_control_hook);


extern void tc3162wdog_kick(void);
/*
 * The @dev_base_head list is protected by @dev_base_lock and the rtnl
 * semaphore.
 *
 * Pure readers hold dev_base_lock for reading, or rcu_read_lock()
 *
 * Writers must hold the rtnl semaphore while they loop through the
 * dev_base_head list, and hold dev_base_lock for writing when they do the
 * actual updates.  This allows pure readers to access the list even
 * while a writer is preparing to update it.
 *
 * To put it another way, dev_base_lock is held for writing only to
 * protect against pure readers; the rtnl semaphore provides the
 * protection against other writers.
 *
 * See, for example usages, register_netdevice() and
 * unregister_netdevice(), which must be called with the rtnl
 * semaphore held.
 */
DEFINE_RWLOCK(dev_base_lock);
EXPORT_SYMBOL(dev_base_lock);

static inline struct hlist_head *dev_name_hash(struct net *net, const char *name)
{
	unsigned hash = full_name_hash(name, strnlen(name, IFNAMSIZ));
	return &net->dev_name_head[hash_32(hash, NETDEV_HASHBITS)];
}

static inline struct hlist_head *dev_index_hash(struct net *net, int ifindex)
{
	return &net->dev_index_head[ifindex & (NETDEV_HASHENTRIES - 1)];
}

static inline void rps_lock(struct softnet_data *sd)
{
#ifdef CONFIG_RPS
	spin_lock(&sd->input_pkt_queue.lock);
#endif
}

static inline void rps_unlock(struct softnet_data *sd)
{
#ifdef CONFIG_RPS
	spin_unlock(&sd->input_pkt_queue.lock);
#endif
}

/* Device list insertion */
static int list_netdevice(struct net_device *dev)
{
	struct net *net = dev_net(dev);

	ASSERT_RTNL();

	write_lock_bh(&dev_base_lock);
	list_add_tail_rcu(&dev->dev_list, &net->dev_base_head);
	hlist_add_head_rcu(&dev->name_hlist, dev_name_hash(net, dev->name));
	hlist_add_head_rcu(&dev->index_hlist,
			   dev_index_hash(net, dev->ifindex));
	write_unlock_bh(&dev_base_lock);
	return 0;
}

/* Device list removal
 * caller must respect a RCU grace period before freeing/reusing dev
 */
static void unlist_netdevice(struct net_device *dev)
{
	ASSERT_RTNL();

	/* Unlink dev from the device chain */
	write_lock_bh(&dev_base_lock);
	list_del_rcu(&dev->dev_list);
	hlist_del_rcu(&dev->name_hlist);
	hlist_del_rcu(&dev->index_hlist);
	write_unlock_bh(&dev_base_lock);
}

/*
 *	Our notifier list
 */

static RAW_NOTIFIER_HEAD(netdev_chain);

/*
 *	Device drivers call our routines to queue packets here. We empty the
 *	queue in the local softnet handler.
 */

DEFINE_PER_CPU_ALIGNED(struct softnet_data, softnet_data);
EXPORT_PER_CPU_SYMBOL(softnet_data);

#ifdef CONFIG_LOCKDEP
/*
 * register_netdevice() inits txq->_xmit_lock and sets lockdep class
 * according to dev->type
 */
static const unsigned short netdev_lock_type[] =
	{ARPHRD_NETROM, ARPHRD_ETHER, ARPHRD_EETHER, ARPHRD_AX25,
	 ARPHRD_PRONET, ARPHRD_CHAOS, ARPHRD_IEEE802, ARPHRD_ARCNET,
	 ARPHRD_APPLETLK, ARPHRD_DLCI, ARPHRD_ATM, ARPHRD_METRICOM,
	 ARPHRD_IEEE1394, ARPHRD_EUI64, ARPHRD_INFINIBAND, ARPHRD_SLIP,
	 ARPHRD_CSLIP, ARPHRD_SLIP6, ARPHRD_CSLIP6, ARPHRD_RSRVD,
	 ARPHRD_ADAPT, ARPHRD_ROSE, ARPHRD_X25, ARPHRD_HWX25,
	 ARPHRD_PPP, ARPHRD_CISCO, ARPHRD_LAPB, ARPHRD_DDCMP,
	 ARPHRD_RAWHDLC, ARPHRD_TUNNEL, ARPHRD_TUNNEL6, ARPHRD_FRAD,
	 ARPHRD_SKIP, ARPHRD_LOOPBACK, ARPHRD_LOCALTLK, ARPHRD_FDDI,
	 ARPHRD_BIF, ARPHRD_SIT, ARPHRD_IPDDP, ARPHRD_IPGRE,
	 ARPHRD_PIMREG, ARPHRD_HIPPI, ARPHRD_ASH, ARPHRD_ECONET,
	 ARPHRD_IRDA, ARPHRD_FCPP, ARPHRD_FCAL, ARPHRD_FCPL,
	 ARPHRD_FCFABRIC, ARPHRD_IEEE802_TR, ARPHRD_IEEE80211,
	 ARPHRD_IEEE80211_PRISM, ARPHRD_IEEE80211_RADIOTAP, ARPHRD_PHONET,
	 ARPHRD_PHONET_PIPE, ARPHRD_IEEE802154,
	 ARPHRD_VOID, ARPHRD_NONE};

static const char *const netdev_lock_name[] =
	{"_xmit_NETROM", "_xmit_ETHER", "_xmit_EETHER", "_xmit_AX25",
	 "_xmit_PRONET", "_xmit_CHAOS", "_xmit_IEEE802", "_xmit_ARCNET",
	 "_xmit_APPLETLK", "_xmit_DLCI", "_xmit_ATM", "_xmit_METRICOM",
	 "_xmit_IEEE1394", "_xmit_EUI64", "_xmit_INFINIBAND", "_xmit_SLIP",
	 "_xmit_CSLIP", "_xmit_SLIP6", "_xmit_CSLIP6", "_xmit_RSRVD",
	 "_xmit_ADAPT", "_xmit_ROSE", "_xmit_X25", "_xmit_HWX25",
	 "_xmit_PPP", "_xmit_CISCO", "_xmit_LAPB", "_xmit_DDCMP",
	 "_xmit_RAWHDLC", "_xmit_TUNNEL", "_xmit_TUNNEL6", "_xmit_FRAD",
	 "_xmit_SKIP", "_xmit_LOOPBACK", "_xmit_LOCALTLK", "_xmit_FDDI",
	 "_xmit_BIF", "_xmit_SIT", "_xmit_IPDDP", "_xmit_IPGRE",
	 "_xmit_PIMREG", "_xmit_HIPPI", "_xmit_ASH", "_xmit_ECONET",
	 "_xmit_IRDA", "_xmit_FCPP", "_xmit_FCAL", "_xmit_FCPL",
	 "_xmit_FCFABRIC", "_xmit_IEEE802_TR", "_xmit_IEEE80211",
	 "_xmit_IEEE80211_PRISM", "_xmit_IEEE80211_RADIOTAP", "_xmit_PHONET",
	 "_xmit_PHONET_PIPE", "_xmit_IEEE802154",
	 "_xmit_VOID", "_xmit_NONE"};

static struct lock_class_key netdev_xmit_lock_key[ARRAY_SIZE(netdev_lock_type)];
static struct lock_class_key netdev_addr_lock_key[ARRAY_SIZE(netdev_lock_type)];

static inline unsigned short netdev_lock_pos(unsigned short dev_type)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(netdev_lock_type); i++)
		if (netdev_lock_type[i] == dev_type)
			return i;
	/* the last key is used by default */
	return ARRAY_SIZE(netdev_lock_type) - 1;
}

static inline void netdev_set_xmit_lockdep_class(spinlock_t *lock,
						 unsigned short dev_type)
{
	int i;

	i = netdev_lock_pos(dev_type);
	lockdep_set_class_and_name(lock, &netdev_xmit_lock_key[i],
				   netdev_lock_name[i]);
}

static inline void netdev_set_addr_lockdep_class(struct net_device *dev)
{
	int i;

	i = netdev_lock_pos(dev->type);
	lockdep_set_class_and_name(&dev->addr_list_lock,
				   &netdev_addr_lock_key[i],
				   netdev_lock_name[i]);
}
#else
static inline void netdev_set_xmit_lockdep_class(spinlock_t *lock,
						 unsigned short dev_type)
{
}
static inline void netdev_set_addr_lockdep_class(struct net_device *dev)
{
}
#endif

/*******************************************************************************

		Protocol management and registration routines

*******************************************************************************/

/*
 *	Add a protocol ID to the list. Now that the input handler is
 *	smarter we can dispense with all the messy stuff that used to be
 *	here.
 *
 *	BEWARE!!! Protocol handlers, mangling input packets,
 *	MUST BE last in hash buckets and checking protocol handlers
 *	MUST start from promiscuous ptype_all chain in net_bh.
 *	It is true now, do not change it.
 *	Explanation follows: if protocol handler, mangling packet, will
 *	be the first on list, it is not able to sense, that packet
 *	is cloned and should be copied-on-write, so that it will
 *	change it and subsequent readers will get broken packet.
 *							--ANK (980803)
 */

/**
 *	dev_add_pack - add packet handler
 *	@pt: packet type declaration
 *
 *	Add a protocol handler to the networking stack. The passed &packet_type
 *	is linked into kernel lists and may not be freed until it has been
 *	removed from the kernel lists.
 *
 *	This call does not sleep therefore it can not
 *	guarantee all CPU's that are in middle of receiving packets
 *	will see the new packet type (until the next received packet).
 */

void dev_add_pack(struct packet_type *pt)
{
	int hash;

	spin_lock_bh(&ptype_lock);
	if (pt->type == htons(ETH_P_ALL))
		list_add_rcu(&pt->list, &ptype_all);
	else {
		hash = ntohs(pt->type) & PTYPE_HASH_MASK;
		list_add_rcu(&pt->list, &ptype_base[hash]);
	}
	spin_unlock_bh(&ptype_lock);
}
EXPORT_SYMBOL(dev_add_pack);

#ifdef TCSUPPORT_PON_VLAN
//check if type already in the list.
//return: 0:yes 1:no
int pon_check_pack(__u16 type)
{
	struct list_head *head;
	struct packet_type *pt1;

	spin_lock_bh(&ptype_lock);
	if (type == htons(ETH_P_ALL))
		head = &ptype_all;
	else
		head = &ptype_base[ntohs(type) & 15];

	list_for_each_entry(pt1, head, list) 
	{
		if (type == pt1->type)
		{
			spin_unlock_bh(&ptype_lock);
			return 0;
		}
	}
	spin_unlock_bh(&ptype_lock);
	return 1;
}
EXPORT_SYMBOL(pon_check_pack);
#endif

#if defined(TCSUPPORT_CT_SWQOS) || defined(TCSUPPORT_XPON_HAL_API_QOS) || defined(TCSUPPORT_XPON_HAL_API_EXT)
int (*sw_pktqosEnqueue) (struct sk_buff * bp) = NULL;
void (*sw_PKTQOS_SET_STOP) (void) = NULL;
void (*sw_PKTQOS_CLEAR_STOP) (void) = NULL;
int (*sw_isSWQosActive) (void) = NULL;
int (*sw_isHwnatOffloadEnable) (void) = NULL;
int (*sw_qosCreatPolicer) (int id, int bandwidth) = NULL;
int (*sw_qosDeletePolicer) (int id) = NULL;
int (*sw_qosOverallRatelimit) (int bandwidth) = NULL;
EXPORT_SYMBOL(sw_pktqosEnqueue);
EXPORT_SYMBOL(sw_PKTQOS_SET_STOP);
EXPORT_SYMBOL(sw_PKTQOS_CLEAR_STOP);
EXPORT_SYMBOL(sw_isSWQosActive);
EXPORT_SYMBOL(sw_isHwnatOffloadEnable);
EXPORT_SYMBOL(sw_qosCreatPolicer);
EXPORT_SYMBOL(sw_qosDeletePolicer);
EXPORT_SYMBOL(sw_qosOverallRatelimit);
#endif

/**
 *	__dev_remove_pack	 - remove packet handler
 *	@pt: packet type declaration
 *
 *	Remove a protocol handler that was previously added to the kernel
 *	protocol handlers by dev_add_pack(). The passed &packet_type is removed
 *	from the kernel lists and can be freed or reused once this function
 *	returns.
 *
 *      The packet type might still be in use by receivers
 *	and must not be freed until after all the CPU's have gone
 *	through a quiescent state.
 */
void __dev_remove_pack(struct packet_type *pt)
{
	struct list_head *head;
	struct packet_type *pt1;

	spin_lock_bh(&ptype_lock);

	if (pt->type == htons(ETH_P_ALL))
		head = &ptype_all;
	else
		head = &ptype_base[ntohs(pt->type) & PTYPE_HASH_MASK];

	list_for_each_entry(pt1, head, list) {
		if (pt == pt1) {
			list_del_rcu(&pt->list);
			goto out;
		}
	}

	printk(KERN_WARNING "dev_remove_pack: %p not found.\n", pt);
out:
	spin_unlock_bh(&ptype_lock);
}
EXPORT_SYMBOL(__dev_remove_pack);

/**
 *	dev_remove_pack	 - remove packet handler
 *	@pt: packet type declaration
 *
 *	Remove a protocol handler that was previously added to the kernel
 *	protocol handlers by dev_add_pack(). The passed &packet_type is removed
 *	from the kernel lists and can be freed or reused once this function
 *	returns.
 *
 *	This call sleeps to guarantee that no CPU is looking at the packet
 *	type after return.
 */
void dev_remove_pack(struct packet_type *pt)
{
	__dev_remove_pack(pt);

	synchronize_net();
}
EXPORT_SYMBOL(dev_remove_pack);

/******************************************************************************

		      Device Boot-time Settings Routines

*******************************************************************************/

/* Boot time configuration table */
static struct netdev_boot_setup dev_boot_setup[NETDEV_BOOT_SETUP_MAX];

/**
 *	netdev_boot_setup_add	- add new setup entry
 *	@name: name of the device
 *	@map: configured settings for the device
 *
 *	Adds new setup entry to the dev_boot_setup list.  The function
 *	returns 0 on error and 1 on success.  This is a generic routine to
 *	all netdevices.
 */
static int netdev_boot_setup_add(char *name, struct ifmap *map)
{
	struct netdev_boot_setup *s;
	int i;

	s = dev_boot_setup;
	for (i = 0; i < NETDEV_BOOT_SETUP_MAX; i++) {
		if (s[i].name[0] == '\0' || s[i].name[0] == ' ') {
			memset(s[i].name, 0, sizeof(s[i].name));
			strlcpy(s[i].name, name, IFNAMSIZ);
			memcpy(&s[i].map, map, sizeof(s[i].map));
			break;
		}
	}

	return i >= NETDEV_BOOT_SETUP_MAX ? 0 : 1;
}

/**
 *	netdev_boot_setup_check	- check boot time settings
 *	@dev: the netdevice
 *
 * 	Check boot time settings for the device.
 *	The found settings are set for the device to be used
 *	later in the device probing.
 *	Returns 0 if no settings found, 1 if they are.
 */
int netdev_boot_setup_check(struct net_device *dev)
{
	struct netdev_boot_setup *s = dev_boot_setup;
	int i;

	for (i = 0; i < NETDEV_BOOT_SETUP_MAX; i++) {
		if (s[i].name[0] != '\0' && s[i].name[0] != ' ' &&
		    !strcmp(dev->name, s[i].name)) {
			dev->irq 	= s[i].map.irq;
			dev->base_addr 	= s[i].map.base_addr;
			dev->mem_start 	= s[i].map.mem_start;
			dev->mem_end 	= s[i].map.mem_end;
			return 1;
		}
	}
	return 0;
}
EXPORT_SYMBOL(netdev_boot_setup_check);


/**
 *	netdev_boot_base	- get address from boot time settings
 *	@prefix: prefix for network device
 *	@unit: id for network device
 *
 * 	Check boot time settings for the base address of device.
 *	The found settings are set for the device to be used
 *	later in the device probing.
 *	Returns 0 if no settings found.
 */
unsigned long netdev_boot_base(const char *prefix, int unit)
{
	const struct netdev_boot_setup *s = dev_boot_setup;
	char name[IFNAMSIZ];
	int i;

	sprintf(name, "%s%d", prefix, unit);

	/*
	 * If device already registered then return base of 1
	 * to indicate not to probe for this interface
	 */
	if (__dev_get_by_name(&init_net, name))
		return 1;

	for (i = 0; i < NETDEV_BOOT_SETUP_MAX; i++)
		if (!strcmp(name, s[i].name))
			return s[i].map.base_addr;
	return 0;
}

/*
 * Saves at boot time configured settings for any netdevice.
 */
int __init netdev_boot_setup(char *str)
{
	int ints[5];
	struct ifmap map;

	str = get_options(str, ARRAY_SIZE(ints), ints);
	if (!str || !*str)
		return 0;

	/* Save settings */
	memset(&map, 0, sizeof(map));
	if (ints[0] > 0)
		map.irq = ints[1];
	if (ints[0] > 1)
		map.base_addr = ints[2];
	if (ints[0] > 2)
		map.mem_start = ints[3];
	if (ints[0] > 3)
		map.mem_end = ints[4];

	/* Add new entry to the list */
	return netdev_boot_setup_add(str, &map);
}

__setup("netdev=", netdev_boot_setup);

/*******************************************************************************

			    Device Interface Subroutines

*******************************************************************************/

/**
 *	__dev_get_by_name	- find a device by its name
 *	@net: the applicable net namespace
 *	@name: name to find
 *
 *	Find an interface by name. Must be called under RTNL semaphore
 *	or @dev_base_lock. If the name is found a pointer to the device
 *	is returned. If the name is not found then %NULL is returned. The
 *	reference counters are not incremented so the caller must be
 *	careful with locks.
 */

struct net_device *__dev_get_by_name(struct net *net, const char *name)
{
	struct hlist_node *p;
	struct net_device *dev;
	struct hlist_head *head = dev_name_hash(net, name);

	hlist_for_each_entry(dev, p, head, name_hlist)
		if (!strncmp(dev->name, name, IFNAMSIZ))
			return dev;

	return NULL;
}
EXPORT_SYMBOL(__dev_get_by_name);

/**
 *	dev_get_by_name_rcu	- find a device by its name
 *	@net: the applicable net namespace
 *	@name: name to find
 *
 *	Find an interface by name.
 *	If the name is found a pointer to the device is returned.
 * 	If the name is not found then %NULL is returned.
 *	The reference counters are not incremented so the caller must be
 *	careful with locks. The caller must hold RCU lock.
 */

struct net_device *dev_get_by_name_rcu(struct net *net, const char *name)
{
	struct hlist_node *p;
	struct net_device *dev;
	struct hlist_head *head = dev_name_hash(net, name);

	hlist_for_each_entry_rcu(dev, p, head, name_hlist)
		if (!strncmp(dev->name, name, IFNAMSIZ))
			return dev;

	return NULL;
}
EXPORT_SYMBOL(dev_get_by_name_rcu);

/**
 *	dev_get_by_name		- find a device by its name
 *	@net: the applicable net namespace
 *	@name: name to find
 *
 *	Find an interface by name. This can be called from any
 *	context and does its own locking. The returned handle has
 *	the usage count incremented and the caller must use dev_put() to
 *	release it when it is no longer needed. %NULL is returned if no
 *	matching device is found.
 */

struct net_device *dev_get_by_name(struct net *net, const char *name)
{
	struct net_device *dev;

	rcu_read_lock();
	dev = dev_get_by_name_rcu(net, name);
	if (dev)
		dev_hold(dev);
	rcu_read_unlock();
	return dev;
}
EXPORT_SYMBOL(dev_get_by_name);

/**
 *	__dev_get_by_index - find a device by its ifindex
 *	@net: the applicable net namespace
 *	@ifindex: index of device
 *
 *	Search for an interface by index. Returns %NULL if the device
 *	is not found or a pointer to the device. The device has not
 *	had its reference counter increased so the caller must be careful
 *	about locking. The caller must hold either the RTNL semaphore
 *	or @dev_base_lock.
 */

struct net_device *__dev_get_by_index(struct net *net, int ifindex)
{
	struct hlist_node *p;
	struct net_device *dev;
	struct hlist_head *head = dev_index_hash(net, ifindex);

	hlist_for_each_entry(dev, p, head, index_hlist)
		if (dev->ifindex == ifindex)
			return dev;

	return NULL;
}
EXPORT_SYMBOL(__dev_get_by_index);

/**
 *	dev_get_by_index_rcu - find a device by its ifindex
 *	@net: the applicable net namespace
 *	@ifindex: index of device
 *
 *	Search for an interface by index. Returns %NULL if the device
 *	is not found or a pointer to the device. The device has not
 *	had its reference counter increased so the caller must be careful
 *	about locking. The caller must hold RCU lock.
 */

struct net_device *dev_get_by_index_rcu(struct net *net, int ifindex)
{
	struct hlist_node *p;
	struct net_device *dev;
	struct hlist_head *head = dev_index_hash(net, ifindex);

	hlist_for_each_entry_rcu(dev, p, head, index_hlist)
		if (dev->ifindex == ifindex)
			return dev;

	return NULL;
}
EXPORT_SYMBOL(dev_get_by_index_rcu);


/**
 *	dev_get_by_index - find a device by its ifindex
 *	@net: the applicable net namespace
 *	@ifindex: index of device
 *
 *	Search for an interface by index. Returns NULL if the device
 *	is not found or a pointer to the device. The device returned has
 *	had a reference added and the pointer is safe until the user calls
 *	dev_put to indicate they have finished with it.
 */

struct net_device *dev_get_by_index(struct net *net, int ifindex)
{
	struct net_device *dev;

	rcu_read_lock();
	dev = dev_get_by_index_rcu(net, ifindex);
	if (dev)
		dev_hold(dev);
	rcu_read_unlock();
	return dev;
}
EXPORT_SYMBOL(dev_get_by_index);

/**
 *	dev_getbyhwaddr - find a device by its hardware address
 *	@net: the applicable net namespace
 *	@type: media type of device
 *	@ha: hardware address
 *
 *	Search for an interface by MAC address. Returns NULL if the device
 *	is not found or a pointer to the device. The caller must hold the
 *	rtnl semaphore. The returned device has not had its ref count increased
 *	and the caller must therefore be careful about locking
 *
 *	BUGS:
 *	If the API was consistent this would be __dev_get_by_hwaddr
 */

struct net_device *dev_getbyhwaddr(struct net *net, unsigned short type, char *ha)
{
	struct net_device *dev;

	ASSERT_RTNL();

	for_each_netdev(net, dev)
		if (dev->type == type &&
		    !memcmp(dev->dev_addr, ha, dev->addr_len))
			return dev;

	return NULL;
}
EXPORT_SYMBOL(dev_getbyhwaddr);

struct net_device *__dev_getfirstbyhwtype(struct net *net, unsigned short type)
{
	struct net_device *dev;

	ASSERT_RTNL();
	for_each_netdev(net, dev)
		if (dev->type == type)
			return dev;

	return NULL;
}
EXPORT_SYMBOL(__dev_getfirstbyhwtype);

struct net_device *dev_getfirstbyhwtype(struct net *net, unsigned short type)
{
	struct net_device *dev, *ret = NULL;

	rcu_read_lock();
	for_each_netdev_rcu(net, dev)
		if (dev->type == type) {
			dev_hold(dev);
			ret = dev;
			break;
		}
	rcu_read_unlock();
	return ret;
}
EXPORT_SYMBOL(dev_getfirstbyhwtype);

/**
 *	dev_get_by_flags_rcu - find any device with given flags
 *	@net: the applicable net namespace
 *	@if_flags: IFF_* values
 *	@mask: bitmask of bits in if_flags to check
 *
 *	Search for any interface with the given flags. Returns NULL if a device
 *	is not found or a pointer to the device. Must be called inside
 *	rcu_read_lock(), and result refcount is unchanged.
 */

struct net_device *dev_get_by_flags_rcu(struct net *net, unsigned short if_flags,
				    unsigned short mask)
{
	struct net_device *dev, *ret;

	ret = NULL;
	for_each_netdev_rcu(net, dev) {
		if (((dev->flags ^ if_flags) & mask) == 0) {
			ret = dev;
			break;
		}
	}
	return ret;
}
EXPORT_SYMBOL(dev_get_by_flags_rcu);

/**
 *	dev_valid_name - check if name is okay for network device
 *	@name: name string
 *
 *	Network device names need to be valid file names to
 *	to allow sysfs to work.  We also disallow any kind of
 *	whitespace.
 */
int dev_valid_name(const char *name)
{
	if (*name == '\0')
		return 0;
	if (strlen(name) >= IFNAMSIZ)
		return 0;
	if (!strcmp(name, ".") || !strcmp(name, ".."))
		return 0;

	while (*name) {
		if (*name == '/' || isspace(*name))
			return 0;
		name++;
	}
	return 1;
}
EXPORT_SYMBOL(dev_valid_name);

/**
 *	__dev_alloc_name - allocate a name for a device
 *	@net: network namespace to allocate the device name in
 *	@name: name format string
 *	@buf:  scratch buffer and result name string
 *
 *	Passed a format string - eg "lt%d" it will try and find a suitable
 *	id. It scans list of devices to build up a free map, then chooses
 *	the first empty slot. The caller must hold the dev_base or rtnl lock
 *	while allocating the name and adding the device in order to avoid
 *	duplicates.
 *	Limited to bits_per_byte * page size devices (ie 32K on most platforms).
 *	Returns the number of the unit assigned or a negative errno code.
 */

static int __dev_alloc_name(struct net *net, const char *name, char *buf)
{
	int i = 0;
	const char *p;
	const int max_netdevices = 8*PAGE_SIZE;
	unsigned long *inuse;
	struct net_device *d;

	p = strnchr(name, IFNAMSIZ-1, '%');
	if (p) {
		/*
		 * Verify the string as this thing may have come from
		 * the user.  There must be either one "%d" and no other "%"
		 * characters.
		 */
		if (p[1] != 'd' || strchr(p + 2, '%'))
			return -EINVAL;

		/* Use one page as a bit array of possible slots */
		inuse = (unsigned long *) get_zeroed_page(GFP_ATOMIC);
		if (!inuse)
			return -ENOMEM;

		for_each_netdev(net, d) {
			if (!sscanf(d->name, name, &i))
				continue;
			if (i < 0 || i >= max_netdevices)
				continue;

			/*  avoid cases where sscanf is not exact inverse of printf */
			snprintf(buf, IFNAMSIZ, name, i);
			if (!strncmp(buf, d->name, IFNAMSIZ))
				set_bit(i, inuse);
		}

		i = find_first_zero_bit(inuse, max_netdevices);
		free_page((unsigned long) inuse);
	}

	if (buf != name)
		snprintf(buf, IFNAMSIZ, name, i);
	if (!__dev_get_by_name(net, buf))
		return i;

	/* It is possible to run out of possible slots
	 * when the name is long and there isn't enough space left
	 * for the digits, or if all bits are used.
	 */
	return -ENFILE;
}

/**
 *	dev_alloc_name - allocate a name for a device
 *	@dev: device
 *	@name: name format string
 *
 *	Passed a format string - eg "lt%d" it will try and find a suitable
 *	id. It scans list of devices to build up a free map, then chooses
 *	the first empty slot. The caller must hold the dev_base or rtnl lock
 *	while allocating the name and adding the device in order to avoid
 *	duplicates.
 *	Limited to bits_per_byte * page size devices (ie 32K on most platforms).
 *	Returns the number of the unit assigned or a negative errno code.
 */

int dev_alloc_name(struct net_device *dev, const char *name)
{
	char buf[IFNAMSIZ];
	struct net *net;
	int ret;

	BUG_ON(!dev_net(dev));
	net = dev_net(dev);
	ret = __dev_alloc_name(net, name, buf);
	if (ret >= 0)
		strlcpy(dev->name, buf, IFNAMSIZ);
	return ret;
}
EXPORT_SYMBOL(dev_alloc_name);

static int dev_get_valid_name(struct net_device *dev, const char *name, bool fmt)
{
	struct net *net;

	BUG_ON(!dev_net(dev));
	net = dev_net(dev);

	if (!dev_valid_name(name))
		return -EINVAL;

	if (fmt && strchr(name, '%'))
		return dev_alloc_name(dev, name);
	else if (__dev_get_by_name(net, name))
		return -EEXIST;
	else if (dev->name != name)
		strlcpy(dev->name, name, IFNAMSIZ);

	return 0;
}

/**
 *	dev_change_name - change name of a device
 *	@dev: device
 *	@newname: name (or format string) must be at least IFNAMSIZ
 *
 *	Change name of a device, can pass format strings "eth%d".
 *	for wildcarding.
 */
int dev_change_name(struct net_device *dev, const char *newname)
{
	char oldname[IFNAMSIZ];
	int err = 0;
	int ret;
	struct net *net;

	ASSERT_RTNL();
	BUG_ON(!dev_net(dev));

	net = dev_net(dev);
	if (dev->flags & IFF_UP)
		return -EBUSY;

	if (strncmp(newname, dev->name, IFNAMSIZ) == 0)
		return 0;

	memcpy(oldname, dev->name, IFNAMSIZ);

	err = dev_get_valid_name(dev, newname, 1);
	if (err < 0)
		return err;

rollback:
	ret = device_rename(&dev->dev, dev->name);
	if (ret) {
		memcpy(dev->name, oldname, IFNAMSIZ);
		return ret;
	}

	write_lock_bh(&dev_base_lock);
	hlist_del(&dev->name_hlist);
	write_unlock_bh(&dev_base_lock);

	synchronize_rcu();

	write_lock_bh(&dev_base_lock);
	hlist_add_head_rcu(&dev->name_hlist, dev_name_hash(net, dev->name));
	write_unlock_bh(&dev_base_lock);

	ret = call_netdevice_notifiers(NETDEV_CHANGENAME, dev);
	ret = notifier_to_errno(ret);

	if (ret) {
		/* err >= 0 after dev_alloc_name() or stores the first errno */
		if (err >= 0) {
			err = ret;
			memcpy(dev->name, oldname, IFNAMSIZ);
			goto rollback;
		} else {
			printk(KERN_ERR
			       "%s: name change rollback failed: %d.\n",
			       dev->name, ret);
		}
	}

	return err;
}

/**
 *	dev_set_alias - change ifalias of a device
 *	@dev: device
 *	@alias: name up to IFALIASZ
 *	@len: limit of bytes to copy from info
 *
 *	Set ifalias for a device,
 */
int dev_set_alias(struct net_device *dev, const char *alias, size_t len)
{
	ASSERT_RTNL();

	if (len >= IFALIASZ)
		return -EINVAL;

	if (!len) {
		if (dev->ifalias) {
			kfree(dev->ifalias);
			dev->ifalias = NULL;
		}
		return 0;
	}

	dev->ifalias = krealloc(dev->ifalias, len + 1, GFP_KERNEL);
	if (!dev->ifalias)
		return -ENOMEM;

	strlcpy(dev->ifalias, alias, len+1);
	return len;
}


/**
 *	netdev_features_change - device changes features
 *	@dev: device to cause notification
 *
 *	Called to indicate a device has changed features.
 */
void netdev_features_change(struct net_device *dev)
{
	call_netdevice_notifiers(NETDEV_FEAT_CHANGE, dev);
}
EXPORT_SYMBOL(netdev_features_change);

/**
 *	netdev_state_change - device changes state
 *	@dev: device to cause notification
 *
 *	Called to indicate a device has changed state. This function calls
 *	the notifier chains for netdev_chain and sends a NEWLINK message
 *	to the routing socket.
 */
void netdev_state_change(struct net_device *dev)
{
	if (dev->flags & IFF_UP) {
		call_netdevice_notifiers(NETDEV_CHANGE, dev);
		rtmsg_ifinfo(RTM_NEWLINK, dev, 0);
	}
}
EXPORT_SYMBOL(netdev_state_change);

int netdev_bonding_change(struct net_device *dev, unsigned long event)
{
	return call_netdevice_notifiers(event, dev);
}
EXPORT_SYMBOL(netdev_bonding_change);

/**
 *	dev_load 	- load a network module
 *	@net: the applicable net namespace
 *	@name: name of interface
 *
 *	If a network interface is not present and the process has suitable
 *	privileges this function loads the module. If module loading is not
 *	available in this kernel then it becomes a nop.
 */

void dev_load(struct net *net, const char *name)
{
	struct net_device *dev;

	rcu_read_lock();
	dev = dev_get_by_name_rcu(net, name);
	rcu_read_unlock();

	if (!dev && capable(CAP_NET_ADMIN))
		request_module("%s", name);
}
EXPORT_SYMBOL(dev_load);

static int __dev_open(struct net_device *dev)
{
	const struct net_device_ops *ops = dev->netdev_ops;
	int ret;

	ASSERT_RTNL();

	/*
	 *	Is it even present?
	 */
	if (!netif_device_present(dev))
		return -ENODEV;

	ret = call_netdevice_notifiers(NETDEV_PRE_UP, dev);
	ret = notifier_to_errno(ret);
	if (ret)
		return ret;

	/*
	 *	Call device private open method
	 */
	set_bit(__LINK_STATE_START, &dev->state);

	if (ops->ndo_validate_addr)
		ret = ops->ndo_validate_addr(dev);

	if (!ret && ops->ndo_open)
		ret = ops->ndo_open(dev);

	/*
	 *	If it went open OK then:
	 */

	if (ret)
		clear_bit(__LINK_STATE_START, &dev->state);
	else {
		/*
		 *	Set the flags.
		 */
		dev->flags |= IFF_UP;

		/*
		 *	Enable NET_DMA
		 */
		net_dmaengine_get();

		/*
		 *	Initialize multicasting status
		 */
		dev_set_rx_mode(dev);

		/*
		 *	Wakeup transmit queue engine
		 */
		dev_activate(dev);
	}

	return ret;
}

/**
 *	dev_open	- prepare an interface for use.
 *	@dev:	device to open
 *
 *	Takes a device from down to up state. The device's private open
 *	function is invoked and then the multicast lists are loaded. Finally
 *	the device is moved into the up state and a %NETDEV_UP message is
 *	sent to the netdev notifier chain.
 *
 *	Calling this function on an active interface is a nop. On a failure
 *	a negative errno code is returned.
 */
int dev_open(struct net_device *dev)
{
	int ret;

	/*
	 *	Is it already up?
	 */
	if (dev->flags & IFF_UP)
		return 0;

	/*
	 *	Open device
	 */
	ret = __dev_open(dev);
	if (ret < 0)
		return ret;

	/*
	 *	... and announce new interface.
	 */
	rtmsg_ifinfo(RTM_NEWLINK, dev, IFF_UP|IFF_RUNNING);
	call_netdevice_notifiers(NETDEV_UP, dev);

	return ret;
}
EXPORT_SYMBOL(dev_open);

static int __dev_close(struct net_device *dev)
{
	const struct net_device_ops *ops = dev->netdev_ops;

	ASSERT_RTNL();
	might_sleep();

	/*
	 *	Tell people we are going down, so that they can
	 *	prepare to death, when device is still operating.
	 */
	call_netdevice_notifiers(NETDEV_GOING_DOWN, dev);

	clear_bit(__LINK_STATE_START, &dev->state);

	/* Synchronize to scheduled poll. We cannot touch poll list,
	 * it can be even on different cpu. So just clear netif_running().
	 *
	 * dev->stop() will invoke napi_disable() on all of it's
	 * napi_struct instances on this device.
	 */
	smp_mb__after_clear_bit(); /* Commit netif_running(). */

	dev_deactivate(dev);

	/*
	 *	Call the device specific close. This cannot fail.
	 *	Only if device is UP
	 *
	 *	We allow it to be called even after a DETACH hot-plug
	 *	event.
	 */
	if (ops->ndo_stop)
		ops->ndo_stop(dev);

	/*
	 *	Device is now down.
	 */

	dev->flags &= ~IFF_UP;

	/*
	 *	Shutdown NET_DMA
	 */
	net_dmaengine_put();

	return 0;
}

/**
 *	dev_close - shutdown an interface.
 *	@dev: device to shutdown
 *
 *	This function moves an active device into down state. A
 *	%NETDEV_GOING_DOWN is sent to the netdev notifier chain. The device
 *	is then deactivated and finally a %NETDEV_DOWN is sent to the notifier
 *	chain.
 */
int dev_close(struct net_device *dev)
{
	if (!(dev->flags & IFF_UP))
		return 0;

	__dev_close(dev);

	/*
	 * Tell people we are down
	 */
	rtmsg_ifinfo(RTM_NEWLINK, dev, IFF_UP|IFF_RUNNING);
	call_netdevice_notifiers(NETDEV_DOWN, dev);

	return 0;
}
EXPORT_SYMBOL(dev_close);


/**
 *	dev_disable_lro - disable Large Receive Offload on a device
 *	@dev: device
 *
 *	Disable Large Receive Offload (LRO) on a net device.  Must be
 *	called under RTNL.  This is needed if received packets may be
 *	forwarded to another interface.
 */
void dev_disable_lro(struct net_device *dev)
{
	if (dev->ethtool_ops && dev->ethtool_ops->get_flags &&
	    dev->ethtool_ops->set_flags) {
		u32 flags = dev->ethtool_ops->get_flags(dev);
		if (flags & ETH_FLAG_LRO) {
			flags &= ~ETH_FLAG_LRO;
			dev->ethtool_ops->set_flags(dev, flags);
		}
	}
	WARN_ON(dev->features & NETIF_F_LRO);
}
EXPORT_SYMBOL(dev_disable_lro);


static int dev_boot_phase = 1;

/*
 *	Device change register/unregister. These are not inline or static
 *	as we export them to the world.
 */

/**
 *	register_netdevice_notifier - register a network notifier block
 *	@nb: notifier
 *
 *	Register a notifier to be called when network device events occur.
 *	The notifier passed is linked into the kernel structures and must
 *	not be reused until it has been unregistered. A negative errno code
 *	is returned on a failure.
 *
 * 	When registered all registration and up events are replayed
 *	to the new notifier to allow device to have a race free
 *	view of the network device list.
 */

int register_netdevice_notifier(struct notifier_block *nb)
{
	struct net_device *dev;
	struct net_device *last;
	struct net *net;
	int err;

	rtnl_lock();
	err = raw_notifier_chain_register(&netdev_chain, nb);
	if (err)
		goto unlock;
	if (dev_boot_phase)
		goto unlock;
	for_each_net(net) {
		for_each_netdev(net, dev) {
			err = nb->notifier_call(nb, NETDEV_REGISTER, dev);
			err = notifier_to_errno(err);
			if (err)
				goto rollback;

			if (!(dev->flags & IFF_UP))
				continue;

			nb->notifier_call(nb, NETDEV_UP, dev);
		}
	}

unlock:
	rtnl_unlock();
	return err;

rollback:
	last = dev;
	for_each_net(net) {
		for_each_netdev(net, dev) {
			if (dev == last)
				break;

			if (dev->flags & IFF_UP) {
				nb->notifier_call(nb, NETDEV_GOING_DOWN, dev);
				nb->notifier_call(nb, NETDEV_DOWN, dev);
			}
			nb->notifier_call(nb, NETDEV_UNREGISTER, dev);
			nb->notifier_call(nb, NETDEV_UNREGISTER_BATCH, dev);
		}
	}

	raw_notifier_chain_unregister(&netdev_chain, nb);
	goto unlock;
}
EXPORT_SYMBOL(register_netdevice_notifier);

/**
 *	unregister_netdevice_notifier - unregister a network notifier block
 *	@nb: notifier
 *
 *	Unregister a notifier previously registered by
 *	register_netdevice_notifier(). The notifier is unlinked into the
 *	kernel structures and may then be reused. A negative errno code
 *	is returned on a failure.
 */

int unregister_netdevice_notifier(struct notifier_block *nb)
{
	int err;

	rtnl_lock();
	err = raw_notifier_chain_unregister(&netdev_chain, nb);
	rtnl_unlock();
	return err;
}
EXPORT_SYMBOL(unregister_netdevice_notifier);

/**
 *	call_netdevice_notifiers - call all network notifier blocks
 *      @val: value passed unmodified to notifier function
 *      @dev: net_device pointer passed unmodified to notifier function
 *
 *	Call all network notifier blocks.  Parameters and return value
 *	are as for raw_notifier_call_chain().
 */

int call_netdevice_notifiers(unsigned long val, struct net_device *dev)
{
	ASSERT_RTNL();
	return raw_notifier_call_chain(&netdev_chain, val, dev);
}

/* When > 0 there are consumers of rx skb time stamps */
static atomic_t netstamp_needed = ATOMIC_INIT(0);

void net_enable_timestamp(void)
{
	atomic_inc(&netstamp_needed);
}
EXPORT_SYMBOL(net_enable_timestamp);

void net_disable_timestamp(void)
{
	atomic_dec(&netstamp_needed);
}
EXPORT_SYMBOL(net_disable_timestamp);

static inline void net_timestamp_set(struct sk_buff *skb)
{
	if (atomic_read(&netstamp_needed))
		__net_timestamp(skb);
	else
		skb->tstamp.tv64 = 0;
}

static inline void net_timestamp_check(struct sk_buff *skb)
{
	if (!skb->tstamp.tv64 && atomic_read(&netstamp_needed))
		__net_timestamp(skb);
}

/**
 * dev_forward_skb - loopback an skb to another netif
 *
 * @dev: destination network device
 * @skb: buffer to forward
 *
 * return values:
 *	NET_RX_SUCCESS	(no congestion)
 *	NET_RX_DROP     (packet was dropped, but freed)
 *
 * dev_forward_skb can be used for injecting an skb from the
 * start_xmit function of one device into the receive queue
 * of another device.
 *
 * The receiving device may be in another namespace, so
 * we have to clear all information in the skb that could
 * impact namespace isolation.
 */
int dev_forward_skb(struct net_device *dev, struct sk_buff *skb)
{
	skb_orphan(skb);
	nf_reset(skb);

	if (!(dev->flags & IFF_UP) ||
	    (skb->len > (dev->mtu + dev->hard_header_len))) {
		kfree_skb(skb);
		return NET_RX_DROP;
	}
	skb_set_dev(skb, dev);
	skb->tstamp.tv64 = 0;
	skb->pkt_type = PACKET_HOST;
	skb->protocol = eth_type_trans(skb, dev);
	return netif_rx(skb);
}
EXPORT_SYMBOL_GPL(dev_forward_skb);

/*
 *	Support routine. Sends outgoing frames to any network
 *	taps currently in use.
 */

static void dev_queue_xmit_nit(struct sk_buff *skb, struct net_device *dev)
{
	struct packet_type *ptype;

#ifdef CONFIG_NET_CLS_ACT
	if (!(skb->tstamp.tv64 && (G_TC_FROM(skb->tc_verd) & AT_INGRESS)))
		net_timestamp_set(skb);
#else
	net_timestamp_set(skb);
#endif

	rcu_read_lock();
	list_for_each_entry_rcu(ptype, &ptype_all, list) {
		/* Never send packets back to the socket
		 * they originated from - MvS (miquels@drinkel.ow.org)
		 */
		if ((ptype->dev == dev || !ptype->dev) &&
		    (ptype->af_packet_priv == NULL ||
		     (struct sock *)ptype->af_packet_priv != skb->sk)) {
#if defined(TCSUPPORT_HWNAT)		
			struct sk_buff *skb2= skb_clone(skb, GFP_ATOMIC|GFP_SKIP_PKTFLOW);
#else
			struct sk_buff *skb2 = skb_clone(skb, GFP_ATOMIC);
#endif
			if (!skb2)
				break;

			/* skb->nh should be correctly
			   set by sender, so that the second statement is
			   just protection against buggy protocols.
			 */
			skb_reset_mac_header(skb2);

			if (skb_network_header(skb2) < skb2->data ||
			    skb2->network_header > skb2->tail) {
				if (net_ratelimit())
					printk(KERN_CRIT "protocol %04x is "
					       "buggy, dev %s\n",
					       ntohs(skb2->protocol),
					       dev->name);
				skb_reset_network_header(skb2);
			}

			skb2->transport_header = skb2->network_header;
			skb2->pkt_type = PACKET_OUTGOING;
			ptype->func(skb2, skb->dev, ptype, skb->dev);
		}
	}
	rcu_read_unlock();
}

/*
 * Routine to help set real_num_tx_queues. To avoid skbs mapped to queues
 * greater then real_num_tx_queues stale skbs on the qdisc must be flushed.
 */
void netif_set_real_num_tx_queues(struct net_device *dev, unsigned int txq)
{
	unsigned int real_num = dev->real_num_tx_queues;

	if (unlikely(txq > dev->num_tx_queues))
		;
	else if (txq > real_num)
		dev->real_num_tx_queues = txq;
	else if (txq < real_num) {
		dev->real_num_tx_queues = txq;
		qdisc_reset_all_tx_gt(dev, txq);
	}
}
EXPORT_SYMBOL(netif_set_real_num_tx_queues);

static inline void __netif_reschedule(struct Qdisc *q)
{
	struct softnet_data *sd;
	unsigned long flags;

	local_irq_save(flags);
	sd = &__get_cpu_var(softnet_data);
	q->next_sched = NULL;
	*sd->output_queue_tailp = q;
	sd->output_queue_tailp = &q->next_sched;
	raise_softirq_irqoff(NET_TX_SOFTIRQ);
	local_irq_restore(flags);
}

void __netif_schedule(struct Qdisc *q)
{
	if (!test_and_set_bit(__QDISC_STATE_SCHED, &q->state))
		__netif_reschedule(q);
}
EXPORT_SYMBOL(__netif_schedule);

void dev_kfree_skb_irq(struct sk_buff *skb)
{
	if (atomic_dec_and_test(&skb->users)) {
		struct softnet_data *sd;
		unsigned long flags;

		local_irq_save(flags);
		sd = &__get_cpu_var(softnet_data);
		skb->next = sd->completion_queue;
		sd->completion_queue = skb;
		raise_softirq_irqoff(NET_TX_SOFTIRQ);
		local_irq_restore(flags);
	}
}
EXPORT_SYMBOL(dev_kfree_skb_irq);

void dev_kfree_skb_any(struct sk_buff *skb)
{
	if (in_irq() || irqs_disabled())
		dev_kfree_skb_irq(skb);
	else
		dev_kfree_skb(skb);
}
EXPORT_SYMBOL(dev_kfree_skb_any);


/**
 * netif_device_detach - mark device as removed
 * @dev: network device
 *
 * Mark device as removed from system and therefore no longer available.
 */
void netif_device_detach(struct net_device *dev)
{
	if (test_and_clear_bit(__LINK_STATE_PRESENT, &dev->state) &&
	    netif_running(dev)) {
		netif_tx_stop_all_queues(dev);
	}
}
EXPORT_SYMBOL(netif_device_detach);

/**
 * netif_device_attach - mark device as attached
 * @dev: network device
 *
 * Mark device as attached from system and restart if needed.
 */
void netif_device_attach(struct net_device *dev)
{
	if (!test_and_set_bit(__LINK_STATE_PRESENT, &dev->state) &&
	    netif_running(dev)) {
		netif_tx_wake_all_queues(dev);
		__netdev_watchdog_up(dev);
	}
}
EXPORT_SYMBOL(netif_device_attach);

static bool can_checksum_protocol(unsigned long features, __be16 protocol)
{
	return ((features & NETIF_F_GEN_CSUM) ||
		((features & NETIF_F_IP_CSUM) &&
		 protocol == htons(ETH_P_IP)) ||
		((features & NETIF_F_IPV6_CSUM) &&
		 protocol == htons(ETH_P_IPV6)) ||
		((features & NETIF_F_FCOE_CRC) &&
		 protocol == htons(ETH_P_FCOE)));
}

static bool dev_can_checksum(struct net_device *dev, struct sk_buff *skb)
{
	if (can_checksum_protocol(dev->features, skb->protocol))
		return true;

	if (skb->protocol == htons(ETH_P_8021Q)) {
		struct vlan_ethhdr *veh = (struct vlan_ethhdr *)skb->data;
		if (can_checksum_protocol(dev->features & dev->vlan_features,
					  veh->h_vlan_encapsulated_proto))
			return true;
	}

	return false;
}

/**
 * skb_dev_set -- assign a new device to a buffer
 * @skb: buffer for the new device
 * @dev: network device
 *
 * If an skb is owned by a device already, we have to reset
 * all data private to the namespace a device belongs to
 * before assigning it a new device.
 */
#ifdef CONFIG_NET_NS
void skb_set_dev(struct sk_buff *skb, struct net_device *dev)
{
	skb_dst_drop(skb);
	if (skb->dev && !net_eq(dev_net(skb->dev), dev_net(dev))) {
		secpath_reset(skb);
		nf_reset(skb);
		skb_init_secmark(skb);
		skb->mark = 0;
		skb->priority = 0;
		skb->nf_trace = 0;
		skb->ipvs_property = 0;
#ifdef CONFIG_NET_SCHED
		skb->tc_index = 0;
#endif
	}
	skb->dev = dev;
}
EXPORT_SYMBOL(skb_set_dev);
#endif /* CONFIG_NET_NS */

/*
 * Invalidate hardware checksum when packet is to be mangled, and
 * complete checksum manually on outgoing path.
 */
int skb_checksum_help(struct sk_buff *skb)
{
	__wsum csum;
	int ret = 0, offset;

	if (skb->ip_summed == CHECKSUM_COMPLETE)
		goto out_set_summed;

	if (unlikely(skb_shinfo(skb)->gso_size)) {
		/* Let GSO fix up the checksum. */
		goto out_set_summed;
	}

	offset = skb->csum_start - skb_headroom(skb);
	BUG_ON(offset >= skb_headlen(skb));
	csum = skb_checksum(skb, offset, skb->len - offset, 0);

	offset += skb->csum_offset;
	BUG_ON(offset + sizeof(__sum16) > skb_headlen(skb));

	if (skb_cloned(skb) &&
	    !skb_clone_writable(skb, offset + sizeof(__sum16))) {
		ret = pskb_expand_head(skb, 0, 0, GFP_ATOMIC);
		if (ret)
			goto out;
	}

	*(__sum16 *)(skb->data + offset) = csum_fold(csum);
out_set_summed:
	skb->ip_summed = CHECKSUM_NONE;
out:
	return ret;
}
EXPORT_SYMBOL(skb_checksum_help);

/**
 *	skb_gso_segment - Perform segmentation on skb.
 *	@skb: buffer to segment
 *	@features: features for the output path (see dev->features)
 *
 *	This function segments the given skb and returns a list of segments.
 *
 *	It may return NULL if the skb requires no segmentation.  This is
 *	only possible when GSO is used for verifying header integrity.
 */
struct sk_buff *skb_gso_segment(struct sk_buff *skb, int features)
{
	struct sk_buff *segs = ERR_PTR(-EPROTONOSUPPORT);
	struct packet_type *ptype;
	__be16 type = skb->protocol;
	int err;

	skb_reset_mac_header(skb);
	skb->mac_len = skb->network_header - skb->mac_header;
	__skb_pull(skb, skb->mac_len);

	if (unlikely(skb->ip_summed != CHECKSUM_PARTIAL)) {
		struct net_device *dev = skb->dev;
		struct ethtool_drvinfo info = {};

		if (dev && dev->ethtool_ops && dev->ethtool_ops->get_drvinfo)
			dev->ethtool_ops->get_drvinfo(dev, &info);

		WARN(1, "%s: caps=(0x%lx, 0x%lx) len=%d data_len=%d "
			"ip_summed=%d",
		     info.driver, dev ? dev->features : 0L,
		     skb->sk ? skb->sk->sk_route_caps : 0L,
		     skb->len, skb->data_len, skb->ip_summed);

		if (skb_header_cloned(skb) &&
		    (err = pskb_expand_head(skb, 0, 0, GFP_ATOMIC)))
			return ERR_PTR(err);
	}

	rcu_read_lock();
	list_for_each_entry_rcu(ptype,
			&ptype_base[ntohs(type) & PTYPE_HASH_MASK], list) {
		if (ptype->type == type && !ptype->dev && ptype->gso_segment) {
			if (unlikely(skb->ip_summed != CHECKSUM_PARTIAL)) {
				err = ptype->gso_send_check(skb);
				segs = ERR_PTR(err);
				if (err || skb_gso_ok(skb, features))
					break;
				__skb_push(skb, (skb->data -
						 skb_network_header(skb)));
			}
			segs = ptype->gso_segment(skb, features);
			break;
		}
	}
	rcu_read_unlock();

	__skb_push(skb, skb->data - skb_mac_header(skb));

	return segs;
}
EXPORT_SYMBOL(skb_gso_segment);

/* Take action when hardware reception checksum errors are detected. */
#ifdef CONFIG_BUG
void netdev_rx_csum_fault(struct net_device *dev)
{
	if (net_ratelimit()) {
		printk(KERN_ERR "%s: hw csum failure.\n",
			dev ? dev->name : "<unknown>");
		dump_stack();
	}
}
EXPORT_SYMBOL(netdev_rx_csum_fault);
#endif

/* Actually, we should eliminate this check as soon as we know, that:
 * 1. IOMMU is present and allows to map all the memory.
 * 2. No high memory really exists on this machine.
 */

static int illegal_highdma(struct net_device *dev, struct sk_buff *skb)
{
#ifdef CONFIG_HIGHMEM
	int i;
	if (!(dev->features & NETIF_F_HIGHDMA)) {
		for (i = 0; i < skb_shinfo(skb)->nr_frags; i++)
			if (PageHighMem(skb_shinfo(skb)->frags[i].page))
				return 1;
	}

	if (PCI_DMA_BUS_IS_PHYS) {
		struct device *pdev = dev->dev.parent;

		if (!pdev)
			return 0;
		for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
			dma_addr_t addr = page_to_phys(skb_shinfo(skb)->frags[i].page);
			if (!pdev->dma_mask || addr + PAGE_SIZE - 1 > *pdev->dma_mask)
				return 1;
		}
	}
#endif
	return 0;
}

struct dev_gso_cb {
	void (*destructor)(struct sk_buff *skb);
};

#define DEV_GSO_CB(skb) ((struct dev_gso_cb *)(skb)->cb)

static void dev_gso_skb_destructor(struct sk_buff *skb)
{
	struct dev_gso_cb *cb;

	do {
		struct sk_buff *nskb = skb->next;

		skb->next = nskb->next;
		nskb->next = NULL;
		kfree_skb(nskb);
	} while (skb->next);

	cb = DEV_GSO_CB(skb);
	if (cb->destructor)
		cb->destructor(skb);
}

/**
 *	dev_gso_segment - Perform emulated hardware segmentation on skb.
 *	@skb: buffer to segment
 *
 *	This function segments the given skb and stores the list of segments
 *	in skb->next.
 */
static int dev_gso_segment(struct sk_buff *skb)
{
	struct net_device *dev = skb->dev;
	struct sk_buff *segs;
	int features = dev->features & ~(illegal_highdma(dev, skb) ?
					 NETIF_F_SG : 0);

	segs = skb_gso_segment(skb, features);

	/* Verifying header integrity only. */
	if (!segs)
		return 0;

	if (IS_ERR(segs))
		return PTR_ERR(segs);

	skb->next = segs;
	DEV_GSO_CB(skb)->destructor = skb->destructor;
	skb->destructor = dev_gso_skb_destructor;

	return 0;
}

/*
 * Try to orphan skb early, right before transmission by the device.
 * We cannot orphan skb if tx timestamp is requested, since
 * drivers need to call skb_tstamp_tx() to send the timestamp.
 */
static inline void skb_orphan_try(struct sk_buff *skb)
{
	struct sock *sk = skb->sk;

	if (sk && !skb_tx(skb)->flags) {
		/* skb_tx_hash() wont be able to get sk.
		 * We copy sk_hash into skb->rxhash
		 */
		if (!skb->rxhash)
			skb->rxhash = sk->sk_hash;
		skb_orphan(skb);
	}
}

/*
 * Returns true if either:
 *	1. skb has frag_list and the device doesn't support FRAGLIST, or
 *	2. skb is fragmented and the device does not support SG, or if
 *	   at least one of fragments is in highmem and device does not
 *	   support DMA from it.
 */
static inline int skb_needs_linearize(struct sk_buff *skb,
				      struct net_device *dev)
{
	return skb_is_nonlinear(skb) &&
	       ((skb_has_frags(skb) && !(dev->features & NETIF_F_FRAGLIST)) ||
	        (skb_shinfo(skb)->nr_frags && (!(dev->features & NETIF_F_SG) ||
					      illegal_highdma(dev, skb))));
}

__IMEM int dev_hard_start_xmit(struct sk_buff *skb, struct net_device *dev,
			struct netdev_queue *txq)
{
	const struct net_device_ops *ops = dev->netdev_ops;
	int rc = NETDEV_TX_OK;
	if (likely(!skb->next)) {
		if (!list_empty(&ptype_all)
#if defined(CONFIG_IMQ) || defined(CONFIG_IMQ_MODULE)
			&& !(skb->imq_flags & IMQ_F_ENQUEUE)
#endif
		   )
			dev_queue_xmit_nit(skb, dev);

		/*
		 * If device doesnt need skb->dst, release it right now while
		 * its hot in this cpu cache
		 */
		if (dev->priv_flags & IFF_XMIT_DST_RELEASE)
			skb_dst_drop(skb);

		skb_orphan_try(skb);

		if (netif_needs_gso(dev, skb)) {
			if (unlikely(dev_gso_segment(skb)))
				goto out_kfree_skb;
			if (skb->next)
				goto gso;
		} else {
			if (skb_needs_linearize(skb, dev) &&
			    __skb_linearize(skb))
				goto out_kfree_skb;

			/* If packet is not checksummed and device does not
			 * support checksumming for this protocol, complete
			 * checksumming here.
			 */
			if (skb->ip_summed == CHECKSUM_PARTIAL) {
				skb_set_transport_header(skb, skb->csum_start -
					      skb_headroom(skb));
				if (!dev_can_checksum(dev, skb) &&
				     skb_checksum_help(skb))
					goto out_kfree_skb;
			}
		}
#if !defined(TCSUPPORT_CT)
#if defined(TCSUPPORT_BRIDGE_FASTPATH)
		if(hook_dev_shortcut_learn){
			hook_dev_shortcut_learn(skb, dev);
		}
#endif	
#endif

		rc = ops->ndo_start_xmit(skb, dev);
		if (rc == NETDEV_TX_OK)
			txq_trans_update(txq);
		return rc;
	}

gso:
	do {
		struct sk_buff *nskb = skb->next;

		skb->next = nskb->next;
		nskb->next = NULL;

		/*
		 * If device doesnt need nskb->dst, release it right now while
		 * its hot in this cpu cache
		 */
		if (dev->priv_flags & IFF_XMIT_DST_RELEASE)
			skb_dst_drop(nskb);

		rc = ops->ndo_start_xmit(nskb, dev);
		if (unlikely(rc != NETDEV_TX_OK)) {
			if (rc & ~NETDEV_TX_MASK)
				goto out_kfree_gso_skb;
			nskb->next = skb->next;
			skb->next = nskb;
			return rc;
		}
		txq_trans_update(txq);
		if (unlikely(netif_tx_queue_stopped(txq) && skb->next))
			return NETDEV_TX_BUSY;
	} while (skb->next);

out_kfree_gso_skb:
	if (likely(skb->next == NULL))
		skb->destructor = DEV_GSO_CB(skb)->destructor;
out_kfree_skb:
	kfree_skb(skb);
	return rc;
}

#ifdef CONFIG_QOS
static inline __be16 vlan_proto(const struct sk_buff *skb)
{
	return vlan_eth_hdr(skb)->h_vlan_encapsulated_proto;
}
static inline __be16 pppoe_proto(const struct sk_buff *skb)
{
	return *((__be16 *)(skb_mac_header(skb) + ETH_HLEN + sizeof(struct pppoe_hdr)));
}

u32 qos_queue_mask = 0;
#endif


#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_PORTBIND//CONFIG_PORT_BINDING
#define ADD_GROUP		1
#define DEL_GROUP		2
int (*portbind_sw_hook)(void);
void (*portbind_update_hook)(struct net_device *dev, int type);
int (*portbind_check_hook)(struct net_device *in_dev, struct net_device *out_dev);
EXPORT_SYMBOL(portbind_sw_hook);
EXPORT_SYMBOL(portbind_check_hook);
EXPORT_SYMBOL(portbind_update_hook);
#endif
#if defined(TCSUPPORT_CT_VLAN_BIND)
/* interface type */
#define 	IF_TYPE_WAN_ROUTE		(1<<0)
#define		IF_TYPE_WAN_BRIDE		(1<<1)
#define		IF_TYPE_LAN				(1<<2)
#define		IF_TYPE_INTERNET		(1<<3)
#define		IF_TYPE_OTHER			(1<<4)
//#define		IF_TYPE_HAS_INTERNET	(1<<5)
#define 	IF_TYPE_HAS_REGISTER	(1<<5)
#define 	MAX_PVC_NUM 			8
#define		MAX_SMUX_NUM			8

int (*vlanbind_active_hook)(void);
int (*vlanbind_entry_active_hook)(int i);
EXPORT_SYMBOL(vlanbind_active_hook);
EXPORT_SYMBOL(vlanbind_entry_active_hook);
#if defined(TCSUPPORT_WLAN_AC)
#define MAX_LAN_PORT_NUM     14
#else
#define MAX_LAN_PORT_NUM    8
#endif
#define MAX_VLAN_GROUP 		20
typedef struct vlanBind {
	u16 lVlanId;
	u16 wVlanId;
} vlanBind_t;
vlanBind_t vBindArray[MAX_LAN_PORT_NUM][MAX_VLAN_GROUP];
#define VBIND_ENTRY_ARRAY_PATH "tc3162/vbind_entry_array"
#define 	VTAG_VID_MASK   		0xfff
#define 	VTAG_GET_VID(tci)   	((tci) & VLAN_VID_MASK)
#endif

#else

#ifdef CONFIG_PORT_BINDING
int (*portbind_sw_hook)(void);
/*TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND*/
int (*portbind_sw_prior_hook)(struct sk_buff *skb);
struct net_device* (*portbind_get_outdev_by_indev_hook)(unsigned char* indev_name);
/*END TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND*/
int (*portbind_check_hook)(char *inIf, char *outIf);
EXPORT_SYMBOL(portbind_sw_hook);
EXPORT_SYMBOL(portbind_check_hook);
/*TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND*/
EXPORT_SYMBOL(portbind_sw_prior_hook);
EXPORT_SYMBOL(portbind_get_outdev_by_indev_hook);
/*END TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND*/
#endif
#endif

#ifdef TCSUPPORT_EPON_MAPPING
int (*epon_sfu_clsfy_hook)(struct sk_buff *skb, int port);
int (*epon_mapping_hook)(struct sk_buff *skb);
EXPORT_SYMBOL(epon_sfu_clsfy_hook);
EXPORT_SYMBOL(epon_mapping_hook);

#endif

#ifdef TCSUPPORT_GPON_MAPPING
int (*gpon_mapping_hook)(struct sk_buff *pskb);
int (*xpon_mode_get_hook)(void);

EXPORT_SYMBOL(gpon_mapping_hook);
EXPORT_SYMBOL(xpon_mode_get_hook);

#if defined(TCSUPPORT_GPON_DOWNSTREAM_MAPPING)
int (*gpon_downstream_mapping_hook)(struct sk_buff *skb);
int (*gpon_downstream_mapping_stag_hook)(struct sk_buff *skb);
EXPORT_SYMBOL(gpon_downstream_mapping_hook);
EXPORT_SYMBOL(gpon_downstream_mapping_stag_hook);
#endif
#endif

#ifdef TCSUPPORT_CPU_PERFORMANCE_TEST
int (*pwan_tls_pon_passthrough_hook)(struct sk_buff *skb);
EXPORT_SYMBOL(pwan_tls_pon_passthrough_hook);
#endif

#define MODE_HGU 0
#define MODE_SFU 1
#ifdef TCSUPPORT_PON_VLAN
int (*pon_insert_tag_hook)(struct sk_buff **pskb);
int (*pon_vlan_get_mode_hook)(void);
int (*pon_store_tag_hook)(struct sk_buff *skb, struct net_device *dev);
int (*pon_check_vlan_hook)(struct net_device *dev, struct sk_buff *skb);
int (*pon_check_tpid_hook)(__u16 * buf);
int (*pon_check_user_group_hook)(struct sk_buff *skb);
int (*pon_PCP_decode_hook)(struct sk_buff **pskb);

EXPORT_SYMBOL(pon_insert_tag_hook);
EXPORT_SYMBOL(pon_vlan_get_mode_hook);
EXPORT_SYMBOL(pon_store_tag_hook);
EXPORT_SYMBOL(pon_check_vlan_hook);
EXPORT_SYMBOL(pon_check_tpid_hook);
EXPORT_SYMBOL(pon_check_user_group_hook);
EXPORT_SYMBOL(pon_PCP_decode_hook);
#endif
#ifdef TCSUPPORT_PON_MAC_FILTER
int (*pon_check_mac_hook)(struct sk_buff *skb);
int (*pon_mac_filter_get_mode_hook)(void);

EXPORT_SYMBOL(pon_check_mac_hook);
EXPORT_SYMBOL(pon_mac_filter_get_mode_hook);
#endif

#ifdef TCSUPPORT_UPSTREAM_VLAN_POLICER
int (*upstream_vlan_policer_hook)(struct sk_buff *skb);
EXPORT_SYMBOL(upstream_vlan_policer_hook);
#endif

static u32 hashrnd __read_mostly;

u16 skb_tx_hash(const struct net_device *dev, const struct sk_buff *skb)
{
	u32 hash;

	if (skb_rx_queue_recorded(skb)) {
		hash = skb_get_rx_queue(skb);
		while (unlikely(hash >= dev->real_num_tx_queues))
			hash -= dev->real_num_tx_queues;
		return hash;
	}

	if (skb->sk && skb->sk->sk_hash)
		hash = skb->sk->sk_hash;
	else
		hash = (__force u16) skb->protocol ^ skb->rxhash;
	hash = jhash_1word(hash, hashrnd);

	return (u16) (((u64) hash * dev->real_num_tx_queues) >> 32);
}
EXPORT_SYMBOL(skb_tx_hash);

static inline u16 dev_cap_txqueue(struct net_device *dev, u16 queue_index)
{
	if (unlikely(queue_index >= dev->real_num_tx_queues)) {
		if (net_ratelimit()) {
			pr_warning("%s selects TX queue %d, but "
				"real number of TX queues is %d\n",
				dev->name, queue_index, dev->real_num_tx_queues);
		}
		return 0;
	}
	return queue_index;
}

struct netdev_queue *dev_pick_tx(struct net_device *dev, struct sk_buff *skb)
{
	int queue_index;
	const struct net_device_ops *ops = dev->netdev_ops;

	if (ops->ndo_select_queue) {
		queue_index = ops->ndo_select_queue(dev, skb);
		queue_index = dev_cap_txqueue(dev, queue_index);
	} else {
		struct sock *sk = skb->sk;
		queue_index = sk_tx_queue_get(sk);
		if (queue_index < 0) {

			queue_index = 0;
			if (dev->real_num_tx_queues > 1)
				queue_index = skb_tx_hash(dev, skb);

			if (sk) {
				struct dst_entry *dst = rcu_dereference_check(sk->sk_dst_cache, 1);

				if (dst && skb_dst(skb) == dst)
					sk_tx_queue_set(sk, queue_index);
			}
		}
	}

	skb_set_queue_mapping(skb, queue_index);
	return netdev_get_tx_queue(dev, queue_index);
}
EXPORT_SYMBOL(dev_pick_tx);

static inline int __dev_xmit_skb(struct sk_buff *skb, struct Qdisc *q,
				 struct net_device *dev,
				 struct netdev_queue *txq)
{
	spinlock_t *root_lock = qdisc_lock(q);
	bool contended = qdisc_is_running(q);
	int rc;

	/*
	 * Heuristic to force contended enqueues to serialize on a
	 * separate lock before trying to get qdisc main lock.
	 * This permits __QDISC_STATE_RUNNING owner to get the lock more often
	 * and dequeue packets faster.
	 */
	if (unlikely(contended))
		spin_lock(&q->busylock);

	spin_lock(root_lock);
	if (unlikely(test_bit(__QDISC_STATE_DEACTIVATED, &q->state))) {
		kfree_skb(skb);
		rc = NET_XMIT_DROP;
	} else if ((q->flags & TCQ_F_CAN_BYPASS) && !qdisc_qlen(q) &&
		   qdisc_run_begin(q)) {
		/*
		 * This is a work-conserving queue; there are no old skbs
		 * waiting to be sent out; and the qdisc is not running -
		 * xmit the skb directly.
		 */
		if (!(dev->priv_flags & IFF_XMIT_DST_RELEASE))
			skb_dst_force(skb);
		__qdisc_update_bstats(q, skb->len);
		if (sch_direct_xmit(skb, q, dev, txq, root_lock)) {
			if (unlikely(contended)) {
				spin_unlock(&q->busylock);
				contended = false;
			}
			__qdisc_run(q);
		} else
			qdisc_run_end(q);

		rc = NET_XMIT_SUCCESS;
	} else {
		skb_dst_force(skb);
		rc = qdisc_enqueue_root(skb, q);
		if (qdisc_run_begin(q)) {
			if (unlikely(contended)) {
				spin_unlock(&q->busylock);
				contended = false;
			}
			__qdisc_run(q);
		}
	}
	spin_unlock(root_lock);
	if (unlikely(contended))
		spin_unlock(&q->busylock);
	return rc;
}

#if defined(TCSUPPORT_CT_VLAN_BIND)

#if defined(TCSUPPORT_CMCC)
int check_vlan_bind_CMCC(struct sk_buff *skb, unsigned int mark){
	int j = 0;
	for (j=0; j<MAX_VLAN_GROUP; j++) {
		if (vBindArray[mark-1][j].lVlanId == 4096) {
			break;
		}
	   	if (VTAG_GET_VID(skb->lan_vlan_tci) == vBindArray[mark-1][j].lVlanId) {
			return 1;
	   	}
	}
	return 0;
}
#endif

int check_vlan_bind(struct sk_buff *skb, struct net_device *out_dev)
{
	unsigned int mark = 0;
	int pvc_mark = 0, pvc_index = 0, entry_index = 0;
	char ifName[10] = {0};
	int i;

	mark = (skb->mark & 0x7F0000) >> 16;
	if (mark == 0) {
		/* If no mark, let it go with the internet bridge.*/
		if ((out_dev->bind_type & IF_TYPE_INTERNET) == IF_TYPE_INTERNET) {
			return 1;
		}
	} else {
		pvc_mark = (mark - 1)/MAX_PVC_NUM;
		sscanf(out_dev->name, "nas%d_%d", &pvc_index, &entry_index); 
		if (pvc_mark == pvc_index){
			return 1;
		}
	}
	return 0;
}

static int vbind_entry_array_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len = 0;
	int i = 0, j = 0;

	for (i = 0; i < MAX_LAN_PORT_NUM; i ++) {
		if(vBindArray[i][0].lVlanId != 4096)
			len += sprintf(page+len,"LAN PORT %d: \n", i+1);
		for (j = 0; j < MAX_VLAN_GROUP; j ++) {
			if(vBindArray[i][j].lVlanId != 4096){
				len += sprintf(page+len,"%d/%d  ", (int)(vBindArray[i][j].lVlanId), (int)(vBindArray[i][j].wVlanId));
			}
		}
		len += sprintf(page+len,"\n");
	}

	len -= off;
	*start = page + off;

	if (len > count)
		len = count;
	else
		*eof = 1;

	if (len < 0)
		len = 0;

	return len;
}

static int vbind_entry_array_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[64] = {0};
	int i = 0, j = 0;
	int lVlanId = 0, wVlanId = 0;
	vlanBind_t vlanBind = {4096,4096};

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	if (strstr(val_string, "reset")) {
		for ( i=0; i<MAX_LAN_PORT_NUM; i++) {
			for ( j=0; j<MAX_VLAN_GROUP; j++) {
				vBindArray[i][j] = vlanBind;
			}
		}
		return count;
	}

	sscanf(val_string, "%d %d %d %d", &i, &j, &lVlanId, &wVlanId);
	vlanBind.lVlanId = (u16)lVlanId;
	vlanBind.wVlanId = (u16)wVlanId;
	vBindArray[i][j] = vlanBind;
	return count;
}
#endif


#if defined(TCSUPPORT_CT_QOS)
static int setAsHighestPriority(struct sk_buff *skb,struct net_device *dev){
	u8 *cp = NULL;
	u16 etherType = 0;
	u16 pppProtocol = 0;	
	struct iphdr *ih = NULL;
	int ret = 0, rule_no = 0, rtp_match = 0;
#ifdef CONFIG_IPV6
	struct ipv6hdr *ih6 = NULL;
#endif
	unsigned int  newtos = 0, oldtos = 0;
	
	//printk("setAsHighestPriority:==>\n");
	if(!(skb->mark & LANIF_MASK) && (skb->mark&QOS_FILTER_MARK) == 0){		//the first time enter in setAsHighestPriority,need check the priority
		//printk("setAsHighestPriority:skb->mark = 0x%x\n",skb->mark);
		/* set ppp & dhcp & ARP & IGMP  packet to highest priority */
		cp = skb->data;
		cp += 12;
		etherType = *(u16*)cp;
		cp += 2;

		//skip vlan tag
		if(etherType == 0x8100 ){
			cp += 2;
			etherType = *(u16*)cp;			
			cp += 2;
		}
		
		if (etherType == 0x0806 || etherType == 0x8863) {
			skb->mark |= 0x10;
		}
		else if (etherType == 0x8864) {
			/* skip pppoe head */
			cp += 6;					/* 6: PPPoE header 2: PPP protocol */
			/* get ppp protocol */
			pppProtocol = *(u16 *) cp;
			/* check if LCP protocol */
			if (pppProtocol == 0xc021 || 
				pppProtocol == 0xc023 || 
				pppProtocol == 0xc025 || 
				pppProtocol == 0x8021 || 
				pppProtocol == 0xc223) {
				skb->mark |= 0x10;
			}
		}
		else {			//0x0800
			/* check dhcp packet, set it to first queue */
			cp = skb->data;
			cp += 23;
			if (*cp == 0x02) { /* igmp */
				skb->mark |= 0x10;
			}			
		}	

		#if 0
		rtp_match = 0;
		
		if (skb->mark & QOS_RULE_INDEX_MARK) {
			rule_no = (skb->mark & QOS_RULE_INDEX_MARK) >> 12;
			skb->mark &= ~QOS_RULE_INDEX_MARK;
			if (0 == qostype_chk(DEV_XMIT_CHK_TYPE, rule_no, dev->name, rtp_match)) {
				ret = get_tos(rule_no, &newtos);
				if (0 == ret) {
					if ( (skb->protocol == htons(ETH_P_IP)) || 
						 (skb->protocol == htons(ETH_P_8021Q) && vlan_proto(skb) == htons(ETH_P_IP)) ) {
						ih = (struct iphdr *)(skb->network_header);
						oldtos = ih->tos;
						if ( skb_make_writable(&skb, sizeof(struct iphdr)) ) {
							ih->tos = (unsigned char)newtos;
							csum_replace2(&ih->check, htons(oldtos), htons(ih->tos));
						}
					}
			#ifdef CONFIG_IPV6
					else if ( (skb->protocol == htons(ETH_P_IPV6)) ||
							  (skb->protocol == htons(ETH_P_8021Q) && vlan_proto(skb) == htons(ETH_P_IPV6)) ) {
						ih6 = (struct ipv6hdr *)(skb->network_header);
						if ( skb_make_writable(&skb, sizeof(struct ipv6hdr)) ) {
							ipv6_change_dsfield( ih6, 0xFF, (unsigned char)newtos );								
						}
					}
			#endif
				}
			}
			else {
				/* wan if not match */
				skb->mark &= ~QOS_FILTER_MARK;
				skb->mark |= QOS_PRIORITY_DEFAULT;
				/* clear 802.1p mark */
				skb->mark &= ~QOS_DOT1P_MARK;
			}
		}
		#endif
	}

	return 0;
}
#endif

#if defined(TCSUPPORT_CMCC)
/* ethertypefilter check flag read proc  */
static int ethertypefilter_array_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len = 0;
	int i = 0;
	char filterStr[32] = {0};

	for (i = 0; i < 8; i ++) {
		memset(filterStr, 0, sizeof(filterStr));
		if(etherTypeFilterArray[i] & 0x00000001)
			strcpy(filterStr, "IPv4oE");
		if(etherTypeFilterArray[i] & 0x00000002){
			if(strlen(filterStr)>0)
				strcat(filterStr, ",PPPoE");
			else
				strcpy(filterStr, "PPPoE");
		}
		if(etherTypeFilterArray[i] & 0x00000004){
			if(strlen(filterStr)>0)
				strcat(filterStr, ",ARP");
			else
				strcpy(filterStr, "ARP");
		}
		if(etherTypeFilterArray[i] & 0x00000008){
			if(strlen(filterStr)>0)
				strcat(filterStr, ",IPv6oE");
			else
				strcpy(filterStr, "IPv6oE");
		}
		len += sprintf(page+len,"%d %04X %s\n", i, etherTypeFilterArray[i], filterStr);
	}

	len -= off;
	*start = page + off;

	if (len > count)
		len = count;
	else
		*eof = 1;

	if (len < 0)
		len = 0;

	return len;
}

/* ethertypefilter check flag write proc  */
static int ethertypefilter_array_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32] = {0};
	int portId = 0;
	unsigned int filterValue = 0;
	char protocolType[32] = {0};

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	sscanf(val_string, "%d %s", &portId, protocolType);
	if(portId < 0 || portId >7)
		return -EINVAL;
	if(strstr(protocolType,"IPv4oE"))
		filterValue=filterValue|0x00000001;
	if(strstr(protocolType,"PPPoE"))
		filterValue=filterValue|0x00000002;
	if(strstr(protocolType,"ARP"))
		filterValue=filterValue|0x00000004;
	if(strstr(protocolType,"IPv6oE"))
		filterValue=filterValue|0x00000008;

	etherTypeFilterArray[portId]=filterValue;
	return count;
}


/****************************************************************************
**function name
	 check_ethertype_filter
**description:
	check whether filter packet or not.
**return 
	1:	filter
	0:	not filter
**parameter:
	skb: the packet information
****************************************************************************/
static int check_ethertype_filter(struct sk_buff *skb)
{
	unsigned int mark = 0;	
	
	mark = (skb->mark & 0xF0000000) >> 28;
	if(mark >= 1 && mark <=8){
		/*printk("check_ethertype_filter1: mark=[%d],orig dev=[%s],outgoing dev=[%s]\n", mark, (skb->original_dev==NULL) ? "CPE" : skb->original_dev->name, skb->dev->name);*/
		if((skb->dev != NULL) && (skb->dev->name[0] == 'n')){/*lan->wan*/
			goto filter;
		}
	}
	else{
		/*printk("check_ethertype_filter2: mark=[%d],orig dev=[%s],outgoing dev=[%s]\n", mark, (skb->original_dev==NULL) ? "CPE" : skb->original_dev->name, skb->dev->name);*/
		if((skb->original_dev != NULL) && (skb->original_dev->name[0] == 'n') && (skb->dev != NULL)){/*wan->lan*/
			if (skb->dev->name[0] == 'e' && skb->dev->name[3] == '0' && skb->dev->name[4] == '.'){/*eth0.x*/
				mark = skb->dev->name[5] - '0';
				goto filter;
			}
			else if (skb->dev->name[0] == 'r'){
				mark = skb->dev->name[2] - '0' + 5;
				goto filter;
			}			
		}
	}
	return 0;
	
filter:
	mark = mark - 1;
	if(mark<0 || mark>7)
		return 0;
	if(etherTypeFilterArray[mark]==0)
		return 0;
	if(etherTypeFilterArray[mark] & 0x00000001){
		if ( (skb->protocol == htons(ETH_P_IP)) || 
				(skb->protocol == htons(ETH_P_8021Q) && vlan_proto(skb) == htons(ETH_P_IP)) ) {
			return 1;
		}
	}
	if(etherTypeFilterArray[mark] & 0x00000002){
		if(skb->protocol == htons(ETH_P_PPP_DISC) || skb->protocol == htons(ETH_P_PPP_SES)){
			return 1;
		}
	}
	if(etherTypeFilterArray[mark] & 0x00000004){
		if(skb->protocol == htons(ETH_P_ARP)){
			return 1;
		}
	}
	if(etherTypeFilterArray[mark] & 0x00000008){
#ifdef CONFIG_IPV6
		if ( (skb->protocol == htons(ETH_P_IPV6)) ||
				(skb->protocol == htons(ETH_P_8021Q) && vlan_proto(skb) == htons(ETH_P_IPV6)) ) {
			return 1;
		}
#endif
	}	
	return 0;
}
#endif

/**
 *	dev_queue_xmit - transmit a buffer
 *	@skb: buffer to transmit
 *
 *	Queue a buffer for transmission to a network device. The caller must
 *	have set the device and priority and built the buffer before calling
 *	this function. The function can be called from an interrupt.
 *
 *	A negative errno code is returned on a failure. A success does not
 *	guarantee the frame will be transmitted as it may be dropped due
 *	to congestion or traffic shaping.
 *
 * -----------------------------------------------------------------------------------
 *      I notice this method can also return errors from the queue disciplines,
 *      including NET_XMIT_DROP, which is a positive value.  So, errors can also
 *      be positive.
 *
 *      Regardless of the return value, the skb is consumed, so it is currently
 *      difficult to retry a send to this method.  (You can bump the ref count
 *      before sending to hold a reference for retry if you are careful.)
 *
 *      When calling this method, interrupts MUST be enabled.  This is because
 *      the BH enable code must have IRQs enabled so that it will not deadlock.
 *          --BLG
 */
__IMEM int dev_queue_xmit(struct sk_buff *skb)
{
	struct net_device *dev = skb->dev;
	struct netdev_queue *txq;
	struct Qdisc *q;
	int rc = -ENOMEM;
#ifdef CONFIG_PORT_BINDING	
	int portbind_ret = 0;
#endif
#ifdef CONFIG_QOS
	u32 queue_num = 0;
	/* add for ppp & dhcp QoS */
#if defined(CONFIG_IMQ) || defined(CONFIG_IMQ_MODULE)
	u8 *cp = NULL;
	u16 etherType = 0;
	u16 pppProtocol = 0;
	
	struct iphdr *ih = NULL;
#ifdef CONFIG_IPV6
	struct ipv6hdr *ih6 = NULL;
#endif
	unsigned int /*ifidx,*/ newtos = 0, oldtos = 0;
	int ret = 0, rule_no = 0, rtp_match = 0;
#endif
#endif
#if defined(TCSUPPORT_PON_VLAN)
	int vlan_mode = MODE_HGU;
#endif
#if defined(TCSUPPORT_PON_MAC_FILTER)
	int mac_filter_mode = MODE_HGU;
#endif
#if defined(TCSUPPORT_CT_VLAN_BIND)
	unsigned int mark = 0;
	int portNum = 0;
	u16 lVlanId = 4096;
	int j = 0;
#endif

#ifdef TCSUPPORT_PON_VLAN
	if(pon_vlan_get_mode_hook)
		vlan_mode = pon_vlan_get_mode_hook();
#endif
#if defined(TCSUPPORT_PON_MAC_FILTER)
	if(pon_mac_filter_get_mode_hook)
		mac_filter_mode = pon_mac_filter_get_mode_hook();
#endif


#if defined(TCSUPPORT_CT)
#if defined(TCSUPPORT_CMCC)
	if(check_ethertype_filter(skb)==1){
		kfree_skb(skb);
		return rc;
	}
#endif
#ifdef TCSUPPORT_PORTBIND//CONFIG_PORT_BINDING
#if defined(TCSUPPORT_CT_VLAN_BIND)
	mark = (skb->mark & 0xF0000000) >> 28;
	/* From this LAN port and it is vlan binded.*/
	if (vlanbind_active_hook && (vlanbind_active_hook() != 0) 
		&& vlanbind_entry_active_hook &&(vlanbind_entry_active_hook(mark) != 0))
	{
#if defined(TCSUPPORT_CMCC)
		if(skb->dev->name[0] == 'n' && (skb->dev->vlan_mode == 3 || skb->dev->vlan_mode == 4) && skb->lan_vlan_tci_valid == 1){
			if(check_vlan_bind_CMCC(skb, mark) == 0)
				goto out_kfree_skb;
		}
#endif
		if ((skb->dev->name[0] == 'n') && ((dev->bind_type & IF_TYPE_WAN_BRIDE) == IF_TYPE_WAN_BRIDE)) {
			if(check_vlan_bind(skb, dev) == 0)
				goto out_kfree_skb;
		}
	}
	else
#endif
	{
	/* packet from cpe, arp etc. let it pass. */
	if (skb->orig_dev == NULL) {
		skb->portbind_mark |= MASK_OUT_DEV;
	}
	/*
	** for out interface, if its br0 do not check and let it pass
	** because only smux interface(eg. nas1_0) or 
	** br2684 interface(eg. nas2) or
	** pppx device in bind group
	*/
	else if (skb->dev->name[0] != 'b'
#if defined(TCSUPPORT_CT_DSLITE)
		&& ( '\0' != skb->dev->name[0] && 'd' != skb->dev->name[0] && 's' != skb->dev->name[1] )
#endif
		) {
		if (((skb->portbind_mark & MASK_OUT_DEV) == 0) && (portbind_sw_hook) && (portbind_sw_hook() == 1)) {
			if ((portbind_check_hook) && (portbind_check_hook(skb->orig_dev, skb->dev) == 0)) {
				goto out_kfree_skb;
			}
			else {
				skb->portbind_mark |= MASK_OUT_DEV;
			}
		}
	}
	}
#endif
#else
#ifdef CONFIG_PORT_BINDING
/*
	if ((portbind_sw_hook) && (portbind_sw_hook() == 1) && ((skb->portbind_mark & MASK_OUT_DEV) == 0)) {
		if ((portbind_check_hook) && (portbind_check_hook(skb->orig_dev_name, skb->dev->name) == 0)) {
			//printk("dev_queue_xmit: checkGroup will free skb, skb orig dev is [%s] outgoing dev is [%s]\n", skb->orig_dev_name, skb->dev->name);
			goto out_kfree_skb;
		}
		else {
			if ('b' != skb->dev->name[0]) {
				skb->portbind_mark |= MASK_OUT_DEV;
			}
		}
	}
*/
	#if defined(TCSUPPORT_FTP_THROUGHPUT)
		if ((portbind_sw_hook) && ((skb->portbind_mark & MASK_OUT_DEV) == 0)) {
	#else
#if defined(TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND)
		if ((portbind_sw_prior_hook) && (portbind_sw_prior_hook(skb) == 1) && ((skb->portbind_mark & MASK_OUT_DEV) == 0)) {
#else
		if ((portbind_sw_hook) && (portbind_sw_hook() == 1) && ((skb->portbind_mark & MASK_OUT_DEV) == 0)) {
	#endif
#endif
		if (portbind_check_hook) {
			//only need check once. shnwind 20110407.
			portbind_ret = portbind_check_hook(skb->orig_dev_name, skb->dev->name);
			if(portbind_ret == 0){
				goto out_kfree_skb;
			}else if(portbind_ret == 1){
				skb->portbind_mark |= MASK_OUT_DEV;	
			}
#if 0	
			if (portbind_check_hook(skb->orig_dev_name, skb->dev->name) == 0) 
				goto out_kfree_skb;
			else if (portbind_check_hook(skb->orig_dev_name, skb->dev->name) == 1) 
				skb->portbind_mark |= MASK_OUT_DEV;
#endif				
			/* else check again */
		}
	}

#endif

#endif

	
#if defined(TCSUPPORT_PON_VLAN) && defined(TCSUPPORT_PON_USER_ISOLATION)
	if(pon_check_user_group_hook)
	{
		if(pon_check_user_group_hook(skb) == -1)
			return rc;
	}
#endif

#ifdef TCSUPPORT_PON_VLAN
	if(vlan_mode == MODE_HGU)
#endif
	{
	#ifdef TCSUPPORT_VLAN_TAG
	if (check_vtag_hook && (check_vtag_hook()) == 1)
	{

		if (insert_vtag_hook && (-1 == insert_vtag_hook(&skb)))
		{
			kfree_skb(skb);
			return rc;
		}
	}
	#endif
#if defined(TCSUPPORT_CT_VLAN_TAG)
	/*restore vlan tag to packet*/
	if(check_vtag_ct_hook && (check_vtag_ct_hook() == 1)
		&& check_vtag_restore_ct_hook && (check_vtag_restore_ct_hook() == 1)
		&& (skb != NULL) && (skb->dev != NULL)
		&& (skb->dev->name[0] == 'e' || skb->dev->name[0] == 'r' || skb->dev->name[0] == 'u')
		&& restore_vtag_ct_hook){		
		if(restore_vtag_ct_hook(&skb) == 0){
			//nothing
		}else{
			kfree_skb(skb);
			vtag_free_tx_cnt++;
			return rc;
		}
	}
#endif
	
	}
#ifdef TCSUPPORT_PON_VLAN
        if(vlan_mode == MODE_SFU){
                /* packet from cpe */
                if (skb->original_dev == NULL) {
                        skb->pon_vlan_flag |= PON_PKT_FROM_CPE;
                }
        }else{
#ifdef TCSUPPORT_XPON_HAL_API_EXT		
                if(strcmp(skb->dev->name,CONST_XMCI_DEV_NAME) == 0)
#else
                if(strcmp(skb->dev->name,"pon") == 0)
#endif				
                        skb->pon_vlan_flag |= PON_VLAN_TX_CALL_HOOK;
        }
        if(pon_insert_tag_hook){
                if(pon_insert_tag_hook(&skb) == -1){
                        kfree_skb(skb);
                        return rc;
                }
        }
        if(pon_check_vlan_hook && (pon_check_vlan_hook(dev,skb) != 1)){
                return rc;
        }
#endif

		ECNT_CORE_DEV_HOOK(ECNT_DEV_QUE_XMIT, skb);

#if defined(TCSUPPORT_CT_VLAN_BIND)
        if (vlanbind_active_hook && (vlanbind_active_hook() != 0)){ 
                if (strstr(skb->dev->name, "eth0.") != NULL)
                        portNum = skb->dev->name[5] - '0';
#ifdef TCSUPPORT_WLAN_AC
				else if ( 'r' == skb->dev->name[0]
					&& 'i' == skb->dev->name[2] )
					portNum = skb->dev->name[3] - '0' + 11;
#endif
                else if (skb->dev->name[0] == 'r')
                        portNum = skb->dev->name[2] - '0' + 5;
                if(vlanbind_entry_active_hook &&(vlanbind_entry_active_hook(portNum) != 0))
                {
                        if (((skb->vlan_tag_flag & VLAN_TAG_FOR_DNS) == VLAN_TAG_FOR_DNS) && (skb->vlan_tags[0] < 4096)) {
                                skb = __vlan_put_tag(skb, skb->vlan_tags[0]);
                                if(skb == NULL)
                                        return rc;
                        }
                        else if ( (skb->nfct != NULL) && (skb->orig_dev != NULL) && (skb->orig_dev->bind_type & IF_TYPE_WAN_ROUTE) == IF_TYPE_WAN_ROUTE) {
                                if (skb->nfct->lVlanId < 4096) {
                                        skb = __vlan_put_tag(skb, skb->nfct->lVlanId);
                                        if(skb == NULL)
                                                return rc;
                                }
                        }
                        else if ((skb->orig_dev != NULL) && (skb->orig_dev->name[0] == 'n')
                                && ((skb->orig_dev->bind_type & (IF_TYPE_WAN_BRIDE | IF_TYPE_OTHER)) == (IF_TYPE_WAN_BRIDE | IF_TYPE_OTHER))){
                                for (j=0; j<MAX_VLAN_GROUP; j++) {
                                        if (vBindArray[portNum-1][j].lVlanId == 4096) {
                                                break;
                                        }
   
                                        if (VTAG_GET_VID(skb->orig_dev->tci) == vBindArray[portNum-1][j].wVlanId) {
                                                lVlanId = vBindArray[portNum-1][j].lVlanId;
                                                break;
                                        }
                                }
                                if (lVlanId < 4096) {
                                        skb = __vlan_put_tag(skb, lVlanId);
                                        if(skb == NULL)
                                                return rc;
                                }
                        }                               
                }
        }
#endif

#if defined(TCSUPPORT_CT_VLAN_TAG)
#ifdef TR143
		if ( NULL != skb->sk && skb->sk->lPbit >= 0 && skb->sk->lPbit <= 7 )
		{
			skb->vlan_tag_flag &= (~(0x0F00));
			if ( 0 == skb->sk->lPbit )
				skb->vlan_tag_flag |= (0x0F << 8);
			else
				skb->vlan_tag_flag |= (skb->sk->lPbit << 8);
		}
#endif
	if ( skb->sk && (skb->sk->lPbit & SOCK_TYPE_WAN_BI) )
	{
		if (ra_sw_nat_hook_free)
			ra_sw_nat_hook_free(skb);
	}
#endif

#ifdef TCSUPPORT_PON_MAC_FILTER 
	if(pon_check_mac_hook)
	{
		if(mac_filter_mode == MODE_SFU && (skb->dev->name[0] == 'e' || skb->dev->name[0] == 'r' || skb->dev->name[0] == 'u'))
			skb->pon_mac_filter_flag |= PON_MAC_FILTER_TX_CALL_HOOK;
		if(pon_check_mac_hook(skb) == -1)
		{
			kfree_skb(skb);
			return rc;
		}
	}
#endif
#if defined(TCSUPPORT_GPON_DOWNSTREAM_MAPPING)
	if(gpon_downstream_mapping_hook && (-1 == gpon_downstream_mapping_hook(skb)))
		return rc;
#endif

#ifdef TCSUPPORT_PON_VLAN
	//decode after mapping
	if(pon_PCP_decode_hook)
	{
		if(pon_PCP_decode_hook(&skb) == -1)
			return rc;
	}
#endif

#ifdef CONFIG_QOS
#ifdef TCSUPPORT_SBTHROUGHPUT_ENHANCE
        if(1 == tc_qos_switch)
        {
#endif
                queue_num = (skb->mark & QOS_FILTER_MARK) >> 4;
                if (qos_queue_mask && (queue_num > 0 && queue_num < 7) && (qos_queue_mask & (1 << (queue_num - 1)))) {
        //                              printk("%s:free skb for this skb is to 0 bindwidth queue.", __FUNCTION__);
                        goto out_kfree_skb;
                }
#if defined(TCSUPPORT_CT_QOS)
#ifndef TCSUPPORT_FWC_ENV
                setAsHighestPriority(skb,dev);
		/*
			qos need remove skb mark if wan unmatch
		*/
		if ( queue_num > 0
			&& (skb->mark & 0xf0000000) )
		{
			if ( 0 != qostype_wan_check(queue_num, dev) )
			{
				skb->mark &= ~QOS_FILTER_MARK;
			}
		}
#endif
#else
                
#if defined(CONFIG_IMQ) || defined(CONFIG_IMQ_MODULE)
                if ( skb->imq_flags & IMQ_F_ENQUEUE ) {
                        skb->imq_flags &= ~IMQ_F_ENQUEUE;       
                        /*start no queue marked packets to default queue*/
                        if ( !(skb->mark & QOS_FILTER_MARK) ) {
                                skb->mark |= QOS_PRIORITY_DEFAULT;
                        }
                }

                /* set ppp & dhcp packet to highest prioty */
                cp = skb->data;
                cp += 12;
                etherType = *(u16*)cp;
                cp += 2;
                if (etherType == 0x8863) {
                        skb->mark &= ~(0xF0);
                        skb->mark |= 0x10;
                }
                else if (etherType == 0x8864) {
                        /* skip pppoe head */
                        cp += 6;                                        /* 6: PPPoE header 2: PPP protocol */
                        /* get ppp protocol */
                        pppProtocol = *(u16 *) cp;
                        /* check if LCP protocol */
                        if (pppProtocol == 0xc021 || 
                                pppProtocol == 0xc023 || 
                                pppProtocol == 0xc025 || 
                                pppProtocol == 0x8021 || 
                                pppProtocol == 0xc223) {
                                skb->mark &= ~(0xF0);
                                skb->mark |= 0x10;
                        }
                } 
                else {
                        /* check dhcp packet, set it to first queue */
                        cp = skb->data;
                        cp += 23;
                        if (*cp == 0x11) { /* udp */
                                cp += 12;
                                if (*cp == 0x43 || *cp == 0x44) { /* udp port is 67 or 68 */
                                        skb->mark &= ~(0xF0);
                                        skb->mark |= 0x10;
                                }
                        }
                }
                
                
                if (skb->mark & QOS_RTP_MARK) {
                        rtp_match = 1;
                }
                else {
                        rtp_match = 0;
                }
                
                if (skb->mark & QOS_RULE_INDEX_MARK) {
                        rule_no = (skb->mark & QOS_RULE_INDEX_MARK) >> 12;
                        skb->mark &= ~QOS_RULE_INDEX_MARK;
                        if (0 == qostype_chk(DEV_XMIT_CHK_TYPE, rule_no, dev->name, rtp_match)) {
                                ret = get_tos(rule_no, &newtos);
                                if (0 == ret) {
                                        if ( (skb->protocol == htons(ETH_P_IP)) || 
                                                 (skb->protocol == htons(ETH_P_8021Q) && vlan_proto(skb) == htons(ETH_P_IP)) ) {
                                                ih = (struct iphdr *)(skb->network_header);
                                                oldtos = ih->tos;
                                                if ( skb_make_writable(&skb, sizeof(struct iphdr)) ) {
                                                        ih->tos = (unsigned char)newtos;
                                                        csum_replace2(&ih->check, htons(oldtos), htons(ih->tos));
                                                }
                                        }
                        #ifdef CONFIG_IPV6
                                        else if ( (skb->protocol == htons(ETH_P_IPV6)) ||
                                                          (skb->protocol == htons(ETH_P_8021Q) && vlan_proto(skb) == htons(ETH_P_IPV6)) ) {
                                                ih6 = (struct ipv6hdr *)(skb->network_header);
                                                if ( skb_make_writable(&skb, sizeof(struct ipv6hdr)) ) {
                                                        ipv6_change_dsfield( ih6, 0xFF, (unsigned char)newtos );                                                                
                                                }
                                        }
                        #endif
                                }
                        }
                        else {
                                /* wan if not match */
                                skb->mark &= ~QOS_FILTER_MARK;
                                skb->mark |= QOS_PRIORITY_DEFAULT;
                                /* clear 802.1p mark */
                                skb->mark &= ~QOS_DOT1P_MARK;
                        }
                }
#endif
#endif
#ifdef TCSUPPORT_SBTHROUGHPUT_ENHANCE
        }
#endif
#endif
#if defined(TCSUPPORT_CT_DS_LIMIT)
	remarkQueueIndexfromDSNodes(skb, DSLIMIT_DW);
#endif


	/* Disable soft irqs for various locks below. Also
	 * stops preemption for RCU.
	 */
	rcu_read_lock_bh();

	txq = dev_pick_tx(dev, skb);
	q = rcu_dereference_bh(txq->qdisc);

#ifdef CONFIG_NET_CLS_ACT
	skb->tc_verd = SET_TC_AT(skb->tc_verd, AT_EGRESS);
#endif
	if (q->enqueue) {
		rc = __dev_xmit_skb(skb, q, dev, txq);
		goto out;
	}

	/* The device has no queue. Common case for software devices:
	   loopback, all the sorts of tunnels...

	   Really, it is unlikely that netif_tx_lock protection is necessary
	   here.  (f.e. loopback and IP tunnels are clean ignoring statistics
	   counters.)
	   However, it is possible, that they rely on protection
	   made by us here.

	   Check this and shot the lock. It is not prone from deadlocks.
	   Either shot noqueue qdisc, it is even simpler 8)
	 */
	if (dev->flags & IFF_UP) {
		int cpu = smp_processor_id(); /* ok because BHs are off */

		if (txq->xmit_lock_owner != cpu) {

			HARD_TX_LOCK(dev, txq, cpu);

			if (!netif_tx_queue_stopped(txq)) {
				rc = dev_hard_start_xmit(skb, dev, txq);
				if (dev_xmit_complete(rc)) {
					HARD_TX_UNLOCK(dev, txq);
					goto out;
				}
			}
			HARD_TX_UNLOCK(dev, txq);
			if (net_ratelimit())
				printk(KERN_CRIT "Virtual device %s asks to "
				       "queue packet!\n", dev->name);
		} else {
			/* Recursion is detected! It is possible,
			 * unfortunately */
			if (net_ratelimit())
				printk(KERN_CRIT "Dead loop on virtual device "
				       "%s, fix it urgently!\n", dev->name);
		}
	}

	rc = -ENETDOWN;
	rcu_read_unlock_bh();

#if defined (CONFIG_PORT_BINDING) || defined(CONFIG_QOS)
out_kfree_skb:
#endif
	kfree_skb(skb);
	return rc;
out:
	rcu_read_unlock_bh();
	return rc;
}
EXPORT_SYMBOL(dev_queue_xmit);


/*=======================================================================
			Receiver routines
  =======================================================================*/

__DMEM int netdev_max_backlog __read_mostly = 1000;
__DMEM int netdev_tstamp_prequeue __read_mostly = 1;
__DMEM int netdev_budget __read_mostly = 300;
__DMEM int weight_p __read_mostly = 64;            /* old backlog weight */

/* Called with irq disabled */
static inline void ____napi_schedule(struct softnet_data *sd,
				     struct napi_struct *napi)
{
	list_add_tail(&napi->poll_list, &sd->poll_list);
	__raise_softirq_irqoff(NET_RX_SOFTIRQ);
}

#ifdef CONFIG_RPS

/* One global table that all flow-based protocols share. */
struct rps_sock_flow_table *rps_sock_flow_table __read_mostly;
EXPORT_SYMBOL(rps_sock_flow_table);

/*
 * get_rps_cpu is called from netif_receive_skb and returns the target
 * CPU from the RPS map of the receiving queue for a given skb.
 * rcu_read_lock must be held on entry.
 */
static int get_rps_cpu(struct net_device *dev, struct sk_buff *skb,
		       struct rps_dev_flow **rflowp)
{
	struct ipv6hdr *ip6;
	struct iphdr *ip;
	struct netdev_rx_queue *rxqueue;
	struct rps_map *map;
	struct rps_dev_flow_table *flow_table;
	struct rps_sock_flow_table *sock_flow_table;
	int cpu = -1;
	u8 ip_proto;
	u16 tcpu;
	u32 addr1, addr2, ihl;
	union {
		u32 v32;
		u16 v16[2];
	} ports;

	if (skb_rx_queue_recorded(skb)) {
		u16 index = skb_get_rx_queue(skb);
		if (unlikely(index >= dev->num_rx_queues)) {
			WARN_ONCE(dev->num_rx_queues > 1, "%s received packet "
				"on queue %u, but number of RX queues is %u\n",
				dev->name, index, dev->num_rx_queues);
			goto done;
		}
		rxqueue = dev->_rx + index;
	} else
		rxqueue = dev->_rx;

	if (!rxqueue->rps_map && !rxqueue->rps_flow_table)
		goto done;

	if (skb->rxhash)
		goto got_hash; /* Skip hash computation on packet header */

	switch (skb->protocol) {
	case __constant_htons(ETH_P_IP):
		if (!pskb_may_pull(skb, sizeof(*ip)))
			goto done;

		ip = (struct iphdr *) skb->data;
		ip_proto = ip->protocol;
		addr1 = (__force u32) ip->saddr;
		addr2 = (__force u32) ip->daddr;
		ihl = ip->ihl;
		break;
	case __constant_htons(ETH_P_IPV6):
		if (!pskb_may_pull(skb, sizeof(*ip6)))
			goto done;

		ip6 = (struct ipv6hdr *) skb->data;
		ip_proto = ip6->nexthdr;
		addr1 = (__force u32) ip6->saddr.s6_addr32[3];
		addr2 = (__force u32) ip6->daddr.s6_addr32[3];
		ihl = (40 >> 2);
		break;
	default:
		goto done;
	}
	switch (ip_proto) {
	case IPPROTO_TCP:
	case IPPROTO_UDP:
	case IPPROTO_DCCP:
	case IPPROTO_ESP:
	case IPPROTO_AH:
	case IPPROTO_SCTP:
	case IPPROTO_UDPLITE:
		if (pskb_may_pull(skb, (ihl * 4) + 4)) {
			ports.v32 = * (__force u32 *) (skb->data + (ihl * 4));
			if (ports.v16[1] < ports.v16[0])
				swap(ports.v16[0], ports.v16[1]);
			break;
		}
	default:
		ports.v32 = 0;
		break;
	}

	/* get a consistent hash (same value on both flow directions) */
	if (addr2 < addr1)
		swap(addr1, addr2);
	skb->rxhash = jhash_3words(addr1, addr2, ports.v32, hashrnd);
	if (!skb->rxhash)
		skb->rxhash = 1;

got_hash:
	flow_table = rcu_dereference(rxqueue->rps_flow_table);
	sock_flow_table = rcu_dereference(rps_sock_flow_table);
	if (flow_table && sock_flow_table) {
		u16 next_cpu;
		struct rps_dev_flow *rflow;

		rflow = &flow_table->flows[skb->rxhash & flow_table->mask];
		tcpu = rflow->cpu;

		next_cpu = sock_flow_table->ents[skb->rxhash &
		    sock_flow_table->mask];

		/*
		 * If the desired CPU (where last recvmsg was done) is
		 * different from current CPU (one in the rx-queue flow
		 * table entry), switch if one of the following holds:
		 *   - Current CPU is unset (equal to RPS_NO_CPU).
		 *   - Current CPU is offline.
		 *   - The current CPU's queue tail has advanced beyond the
		 *     last packet that was enqueued using this table entry.
		 *     This guarantees that all previous packets for the flow
		 *     have been dequeued, thus preserving in order delivery.
		 */
		if (unlikely(tcpu != next_cpu) &&
		    (tcpu == RPS_NO_CPU || !cpu_online(tcpu) ||
		     ((int)(per_cpu(softnet_data, tcpu).input_queue_head -
		      rflow->last_qtail)) >= 0)) {
			tcpu = rflow->cpu = next_cpu;
			if (tcpu != RPS_NO_CPU)
				rflow->last_qtail = per_cpu(softnet_data,
				    tcpu).input_queue_head;
		}
		if (tcpu != RPS_NO_CPU && cpu_online(tcpu)) {
			*rflowp = rflow;
			cpu = tcpu;
			goto done;
		}
	}

	map = rcu_dereference(rxqueue->rps_map);
	if (map) {
		tcpu = map->cpus[((u64) skb->rxhash * map->len) >> 32];

		if (cpu_online(tcpu)) {
			cpu = tcpu;
			goto done;
		}
	}

done:
	return cpu;
}

/* Called from hardirq (IPI) context */
static void rps_trigger_softirq(void *data)
{
	struct softnet_data *sd = data;

	____napi_schedule(sd, &sd->backlog);
	sd->received_rps++;
}

#endif /* CONFIG_RPS */

/*
 * Check if this softnet_data structure is another cpu one
 * If yes, queue it to our IPI list and return 1
 * If no, return 0
 */
static int rps_ipi_queued(struct softnet_data *sd)
{
#ifdef CONFIG_RPS
	struct softnet_data *mysd = &__get_cpu_var(softnet_data);

	if (sd != mysd) {
		sd->rps_ipi_next = mysd->rps_ipi_list;
		mysd->rps_ipi_list = sd;

		__raise_softirq_irqoff(NET_RX_SOFTIRQ);
		return 1;
	}
#endif /* CONFIG_RPS */
	return 0;
}

/*
 * enqueue_to_backlog is called to queue an skb to a per CPU backlog
 * queue (may be a remote CPU queue).
 */
static int enqueue_to_backlog(struct sk_buff *skb, int cpu,
			      unsigned int *qtail)
{
	struct softnet_data *sd;
	unsigned long flags;

	sd = &per_cpu(softnet_data, cpu);

	local_irq_save(flags);

	rps_lock(sd);
	if (skb_queue_len(&sd->input_pkt_queue) <= netdev_max_backlog) {
		if (skb_queue_len(&sd->input_pkt_queue)) {
enqueue:
			__skb_queue_tail(&sd->input_pkt_queue, skb);
			input_queue_tail_incr_save(sd, qtail);
			rps_unlock(sd);
			local_irq_restore(flags);
			return NET_RX_SUCCESS;
		}

		/* Schedule NAPI for backlog device
		 * We can use non atomic operation since we own the queue lock
		 */
		if (!__test_and_set_bit(NAPI_STATE_SCHED, &sd->backlog.state)) {
			if (!rps_ipi_queued(sd))
				____napi_schedule(sd, &sd->backlog);
		}
		goto enqueue;
	}

	sd->dropped++;
	rps_unlock(sd);

	local_irq_restore(flags);

	kfree_skb(skb);
	return NET_RX_DROP;
}

#ifdef TCSUPPORT_DOWNSTREAM_QOS
int isVoipPacket(unsigned char *ip_header){
	unsigned short int skb_port;
	unsigned char protocol;
	int i;

	if(ip_header == NULL){
		return 0;	
	}

	protocol  = *((unsigned char *)(ip_header + 9));
	skb_port = *((unsigned short int *)(ip_header + 22));

	//printk("VoIP packet %d %d\n",protocol, skb_port);
	if(skb_port == 5060){
		return 1;	
	}

	//check rtp port
	for(i=0;i<VOIP_RX_PORT_NUM;i++){
		if(voip_rx_port[i] != 0){
			if((protocol == 0x11) && (skb_port == voip_rx_port[i])){//only UDP
				return 1;
			}	
		}
	}	

	return 0;
}
int isDHCPPacket(unsigned char *ip_header){
	unsigned short int skb_src_port, skb_dst_port;

	if(ip_header == NULL){
		return 0;	
	}

	skb_dst_port = *((unsigned short int *)(ip_header + 22));
	skb_src_port = *((unsigned short int *)(ip_header + 20));
	
	//printk("DHCP %d %d\n",skb_dst_port, skb_src_port);
	if(((skb_dst_port == 68) || (skb_src_port == 68)) //CPE is client
		||((skb_dst_port == 67) || (skb_src_port == 67))) //CPE is server
	{
		return 1;	
	}
	return 0;
}
int isPPPPacket(unsigned char *mac_header){
	unsigned short int ether_protocol, p2p_protocol;

	if(mac_header == NULL){
		return 0;	
	}
	
	ether_protocol = *((unsigned short int *)(mac_header + 12));

	//printk("PPP eth %x p2p %x\n",*((unsigned short int *)(mac_header + 12)), *((unsigned short int *)(mac_header + 20)));
	if(ether_protocol == 0x8863)//pppoe discovery
	{
		return 1;
	}
	else if(ether_protocol == 0x8864)//pppoe session
	{	
		p2p_protocol = *((unsigned short int *)(mac_header + 20));
		if((p2p_protocol == 0xc021) //LCP
			|| (p2p_protocol == 0xc223) //CHAP
			|| (p2p_protocol == 0xc023) //PAP
			|| (p2p_protocol == 0x8021)) //IPCP 0x8021?
		{
			return 1;	
		}
	}
	return 0;
}
int isMulticastPacket(unsigned char *ip_header){
	unsigned char skb_dst_ip_hi;

	if(ip_header == NULL){
		return 0;	
	}

	skb_dst_ip_hi = *((unsigned char *)(ip_header + 16));
	//printk("Multicast %x\n",skb_dst_ip_hi);
	if((skb_dst_ip_hi & 0xf0) == 0xe0){
		return 1;	
	}
	return 0;
}

#endif


/**
 *	netif_rx	-	post buffer to the network code
 *	@skb: buffer to post
 *
 *	This function receives a packet from a device driver and queues it for
 *	the upper (protocol) levels to process.  It always succeeds. The buffer
 *	may be dropped during processing for congestion control or by the
 *	protocol layers.
 *
 *	return values:
 *	NET_RX_SUCCESS	(no congestion)
 *	NET_RX_DROP     (packet was dropped)
 *
 */

__IMEM int netif_rx(struct sk_buff *skb)
{
	int ret;
#ifdef TCSUPPORT_DOWNSTREAM_QOS
	struct softnet_data  *queue = NULL;
	unsigned long flags = 0;
	unsigned char *mac_header = NULL;
	unsigned char *ip_header = NULL;
	int total_queue_len = 0;
	int  i = 0;
#endif

	/* if netpoll wants it, pretend we never saw it */
	if (netpoll_rx(skb))
		return NET_RX_DROP;

	if (netdev_tstamp_prequeue)
		net_timestamp_check(skb);

#ifdef TCSUPPORT_DOWNSTREAM_QOS
	if(downstream_qos_enable){
	local_irq_save(flags);
	queue = &per_cpu(softnet_data, get_cpu());

	//caclute total queue len
	total_queue_len = queue->input_pkt_queue.qlen;
	for( i = 0 ; i < PRIORITY_QUEUE_NUM; i++){
		total_queue_len += queue->pri_queue[i].qlen;
	}
	if(total_queue_len <= netdev_max_backlog){
		if(total_queue_len == 0){
			//no packet in queue. wackup net_rx_action.
			if (!__test_and_set_bit(NAPI_STATE_SCHED, &queue->backlog.state)) {
				if (!rps_ipi_queued(queue))
					____napi_schedule(queue, &queue->backlog);
			}
		}
		//dev_hold(skb->dev);
		//if enable, check each packet for each priority queue. shnwind 20110415.

		mac_header = (unsigned char *)eth_hdr(skb);
		ip_header = (unsigned char *)(skb->data);
		if(mac_header != NULL){ //check vlan_tag
			
			if(*((unsigned short int *)(mac_header + 12)) == 0x8100){
				mac_header += 4;
				ip_header += 4;
				if(*((unsigned short int *)(mac_header + 12)) == 0x8100){
					mac_header += 4;
					ip_header += 4;
				}
			}
		}
		
		//check first priority
		if(isVoipPacket(ip_header)
			|| isDHCPPacket(ip_header)
			|| isPPPPacket(mac_header)
			)
		{	
			__skb_queue_tail(&queue->pri_queue[0], skb);
			goto exit;
		}
			
		//check second priority
		if(isMulticastPacket(ip_header))
		{
			__skb_queue_tail(&queue->pri_queue[1], skb);
			goto exit;
		}

		//check third priority



		}
		//lowest priority , original backlog queue.
		__skb_queue_tail(&queue->input_pkt_queue, skb);
		goto exit;
	
	queue->dropped++;
	local_irq_restore(flags);

	kfree_skb(skb);
	put_cpu();
	return NET_RX_DROP;
exit:
	local_irq_restore(flags);
	put_cpu();
	return NET_RX_SUCCESS;
	}
	else{//downstream qos disable
#endif
#ifdef CONFIG_RPS
	{
		struct rps_dev_flow voidflow, *rflow = &voidflow;
		int cpu;

		preempt_disable();
		rcu_read_lock();

		cpu = get_rps_cpu(skb->dev, skb, &rflow);
		if (cpu < 0)
			cpu = smp_processor_id();

		ret = enqueue_to_backlog(skb, cpu, &rflow->last_qtail);

		rcu_read_unlock();
		preempt_enable();
	}
#else
	{
		unsigned int qtail;
		ret = enqueue_to_backlog(skb, get_cpu(), &qtail);
		put_cpu();
	}
#endif
#ifdef TCSUPPORT_DOWNSTREAM_QOS
		}
#endif

	return ret;


}

EXPORT_SYMBOL(netif_rx);

int netif_rx_ni(struct sk_buff *skb)
{
	int err;

	preempt_disable();
	err = netif_rx(skb);
	if (local_softirq_pending())
		do_softirq();
	preempt_enable();

	return err;
}
EXPORT_SYMBOL(netif_rx_ni);

static void net_tx_action(struct softirq_action *h)
{
	struct softnet_data *sd = &__get_cpu_var(softnet_data);

	if (sd->completion_queue) {
		struct sk_buff *clist;

		local_irq_disable();
		clist = sd->completion_queue;
		sd->completion_queue = NULL;
		local_irq_enable();

		while (clist) {
			struct sk_buff *skb = clist;
			clist = clist->next;

			WARN_ON(atomic_read(&skb->users));
			__kfree_skb(skb);
		}
	}

	if (sd->output_queue) {
		struct Qdisc *head;

		local_irq_disable();
		head = sd->output_queue;
		sd->output_queue = NULL;
		sd->output_queue_tailp = &sd->output_queue;
		local_irq_enable();

		while (head) {
			struct Qdisc *q = head;
			spinlock_t *root_lock;

			head = head->next_sched;

			root_lock = qdisc_lock(q);
			if (spin_trylock(root_lock)) {
				smp_mb__before_clear_bit();
				clear_bit(__QDISC_STATE_SCHED,
					  &q->state);
				qdisc_run(q);
				spin_unlock(root_lock);
			} else {
				if (!test_bit(__QDISC_STATE_DEACTIVATED,
					      &q->state)) {
					__netif_reschedule(q);
				} else {
					smp_mb__before_clear_bit();
					clear_bit(__QDISC_STATE_SCHED,
						  &q->state);
				}
			}
		}
	}
}

static inline int deliver_skb(struct sk_buff *skb,
			      struct packet_type *pt_prev,
			      struct net_device *orig_dev)
{
	atomic_inc(&skb->users);
	return pt_prev->func(skb, skb->dev, pt_prev, orig_dev);
}

#if (defined(CONFIG_BRIDGE) || defined(CONFIG_BRIDGE_MODULE)) && \
    (defined(CONFIG_ATM_LANE) || defined(CONFIG_ATM_LANE_MODULE))
/* This hook is defined here for ATM LANE */
int (*br_fdb_test_addr_hook)(struct net_device *dev,
			     unsigned char *addr) __read_mostly;
EXPORT_SYMBOL_GPL(br_fdb_test_addr_hook);
#endif

#ifdef CONFIG_NET_CLS_ACT
/* TODO: Maybe we should just force sch_ingress to be compiled in
 * when CONFIG_NET_CLS_ACT is? otherwise some useless instructions
 * a compare and 2 stores extra right now if we dont have it on
 * but have CONFIG_NET_CLS_ACT
 * NOTE: This doesnt stop any functionality; if you dont have
 * the ingress scheduler, you just cant add policies on ingress.
 *
 */
static int ing_filter(struct sk_buff *skb)
{
	struct net_device *dev = skb->dev;
	u32 ttl = G_TC_RTTL(skb->tc_verd);
	struct netdev_queue *rxq;
	int result = TC_ACT_OK;
	struct Qdisc *q;

	if (unlikely(MAX_RED_LOOP < ttl++)) {
		if (net_ratelimit())
			pr_warning( "Redir loop detected Dropping packet (%d->%d)\n",
			       skb->skb_iif, dev->ifindex);
		return TC_ACT_SHOT;
	}

	skb->tc_verd = SET_TC_RTTL(skb->tc_verd, ttl);
	skb->tc_verd = SET_TC_AT(skb->tc_verd, AT_INGRESS);

	rxq = &dev->rx_queue;

	q = rxq->qdisc;
	if (q != &noop_qdisc) {
		spin_lock(qdisc_lock(q));
		if (likely(!test_bit(__QDISC_STATE_DEACTIVATED, &q->state)))
			result = qdisc_enqueue_root(skb, q);
		spin_unlock(qdisc_lock(q));
	}

	return result;
}

static inline struct sk_buff *handle_ing(struct sk_buff *skb,
					 struct packet_type **pt_prev,
					 int *ret, struct net_device *orig_dev)
{
	if (skb->dev->rx_queue.qdisc == &noop_qdisc)
		goto out;

	if (*pt_prev) {
		*ret = deliver_skb(skb, *pt_prev, orig_dev);
		*pt_prev = NULL;
	}

	switch (ing_filter(skb)) {
	case TC_ACT_SHOT:
	case TC_ACT_STOLEN:
		kfree_skb(skb);
		return NULL;
	}

out:
	skb->tc_verd = 0;
	return skb;
}
#endif

/*
 * 	netif_nit_deliver - deliver received packets to network taps
 * 	@skb: buffer
 *
 * 	This function is used to deliver incoming packets to network
 * 	taps. It should be used when the normal netif_receive_skb path
 * 	is bypassed, for example because of VLAN acceleration.
 */
void netif_nit_deliver(struct sk_buff *skb)
{
	struct packet_type *ptype;

	if (list_empty(&ptype_all))
		return;

	skb_reset_network_header(skb);
	skb_reset_transport_header(skb);
	skb->mac_len = skb->network_header - skb->mac_header;

	rcu_read_lock();
	list_for_each_entry_rcu(ptype, &ptype_all, list) {
		if (!ptype->dev || ptype->dev == skb->dev)
			deliver_skb(skb, ptype, skb->dev);
	}
	rcu_read_unlock();
}

/**
 *	netdev_rx_handler_register - register receive handler
 *	@dev: device to register a handler for
 *	@rx_handler: receive handler to register
 *	@rx_handler_data: data pointer that is used by rx handler
 *
 *	Register a receive hander for a device. This handler will then be
 *	called from __netif_receive_skb. A negative errno code is returned
 *	on a failure.
 *
 *	The caller must hold the rtnl_mutex.
 */
int netdev_rx_handler_register(struct net_device *dev,
			       rx_handler_func_t *rx_handler,
			       void *rx_handler_data)
{
	ASSERT_RTNL();

	if (dev->rx_handler)
		return -EBUSY;

	rcu_assign_pointer(dev->rx_handler_data, rx_handler_data);
	rcu_assign_pointer(dev->rx_handler, rx_handler);

	return 0;
}
EXPORT_SYMBOL_GPL(netdev_rx_handler_register);

/**
 *	netdev_rx_handler_unregister - unregister receive handler
 *	@dev: device to unregister a handler from
 *
 *	Unregister a receive hander from a device.
 *
 *	The caller must hold the rtnl_mutex.
 */
void netdev_rx_handler_unregister(struct net_device *dev)
{

	ASSERT_RTNL();
	rcu_assign_pointer(dev->rx_handler, NULL);
	rcu_assign_pointer(dev->rx_handler_data, NULL);
}
EXPORT_SYMBOL_GPL(netdev_rx_handler_unregister);

static inline void skb_bond_set_mac_by_master(struct sk_buff *skb,
					      struct net_device *master)
{
	if (skb->pkt_type == PACKET_HOST) {
		u16 *dest = (u16 *) eth_hdr(skb)->h_dest;

		memcpy(dest, master->dev_addr, ETH_ALEN);
	}
}

/* On bonding slaves other than the currently active slave, suppress
 * duplicates except for 802.3ad ETH_P_SLOW, alb non-mcast/bcast, and
 * ARP on active-backup slaves with arp_validate enabled.
 */
int __skb_bond_should_drop(struct sk_buff *skb, struct net_device *master)
{
	struct net_device *dev = skb->dev;

	if (master->priv_flags & IFF_MASTER_ARPMON)
		dev->last_rx = jiffies;

	if ((master->priv_flags & IFF_MASTER_ALB) &&
	    (master->priv_flags & IFF_BRIDGE_PORT)) {
		/* Do address unmangle. The local destination address
		 * will be always the one master has. Provides the right
		 * functionality in a bridge.
		 */
		skb_bond_set_mac_by_master(skb, master);
	}

	if (dev->priv_flags & IFF_SLAVE_INACTIVE) {
		if ((dev->priv_flags & IFF_SLAVE_NEEDARP) &&
		    skb->protocol == __cpu_to_be16(ETH_P_ARP))
			return 0;

		if (master->priv_flags & IFF_MASTER_ALB) {
			if (skb->pkt_type != PACKET_BROADCAST &&
			    skb->pkt_type != PACKET_MULTICAST)
				return 0;
		}
		if (master->priv_flags & IFF_MASTER_8023AD &&
		    skb->protocol == __cpu_to_be16(ETH_P_SLOW))
			return 0;

		return 1;
	}
	return 0;
}
EXPORT_SYMBOL(__skb_bond_should_drop);

#if (defined(TCSUPPORT_XPON_MAPPING) || defined(TCSUPPORT_PON_VLAN) || defined(TCSUPPORT_XPON_IGMP)) && defined(TCSUPPORT_PON_IP_HOST)

#define br_port_get_rcu(dev) \
	 ((struct net_bridge_port *) rcu_dereference(dev->rx_handler_data))

char voip_wanIf[8] = "NULL";

static int pon_wanIf_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	 int len = 0;

	 len = sprintf(buf,"voip %s\n", voip_wanIf );

	 if (len < off + count)
		 *eof = 1;
 
	 len -= off;
	 *start = buf + off;
	 if(len > count)
		 len = count;
	  if(len < 0)
		 len = 0;

	return len;
}
 
static int pon_wanIf_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char wan_type[8] = {0};
	char wan_name[8] = {0};
	char temp[16] = {0};
	
	if (count > 15)
		return -EFAULT;

	if (copy_from_user(temp, buffer, count))
		return -EFAULT;

	sscanf(temp, "%s %s", wan_type, wan_name);
	
	if(memcmp(wan_name, "nas", 3) != 0)
		return -EFAULT;
	
	if(strcmp(wan_type, "voip") == 0)
		strcpy(voip_wanIf, wan_name);
	else
		return -EFAULT;
		
	return count;
}

int isBridgeWan(struct net_device *dev)
{
	 struct net_bridge_port *port;
 
	 if (dev == NULL) 
		 return 0;
 
	 if ((port = br_port_get_rcu(dev)) == NULL)
		 return 0;
	 
	 return 1;
}

int isVoipWan(struct net_device *dev){
	if(dev == NULL || isBridgeWan(dev))
		return 0;

#if defined(TCSUPPORT_CT)
	if(dev->name[0] == 'n' && dev->name[3] == voip_wanIf[3] && dev->name[5] == voip_wanIf[5])
#else
	if(dev->name[0] == 'n' && dev->name[3] == voip_wanIf[3])
#endif
		return 1;
	
	return 0;
}

EXPORT_SYMBOL(isVoipWan);

#endif

#if defined(TCSUPPORT_XPON_IGMP)
int (*xpon_igmp_acl_filter_hook)(struct sk_buff* skb) = NULL;
EXPORT_SYMBOL(xpon_igmp_acl_filter_hook);
#endif

static int __netif_receive_skb(struct sk_buff *skb)
{
	struct packet_type *ptype, *pt_prev;
	rx_handler_func_t *rx_handler;
	struct net_device *orig_dev;
	struct net_device *master;
	struct net_device *null_or_orig;
	struct net_device *orig_or_bond;
	int ret = NET_RX_DROP;
	__be16 type;
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
#ifdef TCSUPPORT_XPON_HAL_API_EXT
	if(strstr(skb->dev->name, CONST_XMCI_DEV_NAME)){
#else
	if(strstr(skb->dev->name, "pon")){
#endif	
		if(xpon_igmp_acl_filter_hook && xpon_igmp_acl_filter_hook(skb) == 0){
			return NET_RX_DROP;
		}
	}
#endif

	if(skb->dev->reg_state!=NETREG_REGISTERED){
	//	printk("xflu:: drop packet from %s\n",skb->dev->name);
		kfree_skb(skb);
		return NET_RX_DROP;
	}
	if (!netdev_tstamp_prequeue)
		net_timestamp_check(skb);

	if (vlan_tx_tag_present(skb) && vlan_hwaccel_do_receive(skb))
		return NET_RX_SUCCESS;

	/* if we've gotten here through NAPI, check netpoll */
	if (netpoll_receive_skb(skb))
		return NET_RX_DROP;

	if (!skb->skb_iif)
		skb->skb_iif = skb->dev->ifindex;

	/*
	 * bonding note: skbs received on inactive slaves should only
	 * be delivered to pkt handlers that are exact matches.  Also
	 * the deliver_no_wcard flag will be set.  If packet handlers
	 * are sensitive to duplicate packets these skbs will need to
	 * be dropped at the handler.  The vlan accel path may have
	 * already set the deliver_no_wcard flag.
	 */
	null_or_orig = NULL;
	orig_dev = skb->dev;
	master = ACCESS_ONCE(orig_dev->master);
	if (skb->deliver_no_wcard)
		null_or_orig = orig_dev;
	else if (master) {
		if (skb_bond_should_drop(skb, master)) {
			skb->deliver_no_wcard = 1;
			null_or_orig = orig_dev; /* deliver only exact match */
		} else
			skb->dev = master;
	}
	
#if !defined(TCSUPPORT_CT)
#if defined(TCSUPPORT_BRIDGE_FASTPATH)
		if(hook_bridge_shortcut_process)
		{
			if( hook_bridge_shortcut_process(skb->dev, skb) ){
				return NET_RX_DROP;
			}	
		}
#endif
#endif


#ifdef TCSUPPORT_PON_VLAN
	if((orig_dev->name[0] == 'r' || orig_dev->name[0] == 'u' || orig_dev->name[0] == 'e') && vlan_mode == MODE_SFU)
	{
		skb->pon_vlan_flag |= PON_PKT_FROM_LAN;
	}
#ifdef TCSUPPORT_XPON_HAL_API_EXT
	else if(strcmp(orig_dev->name,CONST_XMCI_DEV_NAME) == 0 && vlan_mode == MODE_SFU)
#else	
	else if(strcmp(orig_dev->name,"pon") == 0 && vlan_mode == MODE_SFU)
#endif	
	{
		skb->pon_vlan_flag |= PON_PKT_FROM_WAN;
	}
#endif

#ifdef TCSUPPORT_XPON_HAL_API_EXT
	if(skb->dev->name[0] != 'b')
		skb->original_dev = skb->dev;
#else
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
#ifdef TCSUPPORT_XPON_HAL_API_EXT
		if(strcmp(orig_dev->name,CONST_XMCI_DEV_NAME) == 0)
#else
		if(strcmp(orig_dev->name,"pon") == 0)
#endif		
			skb->pon_vlan_flag |= PON_VLAN_RX_CALL_HOOK;
#endif
		if(pon_insert_tag_hook(&skb) == -1)
		{
			kfree_skb(skb);
			return NET_RX_DROP;
		}
	}
#ifdef TCSUPPORT_PON_IP_HOST
	else if(pon_insert_tag_hook && vlan_mode == MODE_SFU){
		if(isVoipWan(orig_dev)){
			skb->pon_vlan_flag |= PON_PKT_VOIP_RX;
			if(pon_insert_tag_hook(&skb) == -1)
			{
				kfree_skb(skb);
				return NET_RX_DROP;
			}
		}
	}
#endif
#endif
#ifdef TCSUPPORT_PON_MAC_FILTER 
	if(pon_check_mac_hook && mac_filter_mode == MODE_HGU)
	{
#ifdef TCSUPPORT_XPON_HAL_API_EXT	
		if(strcmp(orig_dev->name,CONST_XMCI_DEV_NAME) == 0)
#else
		if(strcmp(orig_dev->name,"pon") == 0)
#endif		
			skb->pon_mac_filter_flag |= PON_MAC_FILTER_RX_CALL_HOOK;
		if(pon_check_mac_hook(skb) == -1)
		{
			kfree_skb(skb);
			return NET_RX_DROP;
		}
	} 
#ifdef TCSUPPORT_CHS
	else if(pon_check_mac_hook && mac_filter_mode == MODE_SFU)
	{
		if (orig_dev->name[0] == 'e'|| orig_dev->name[0] == 'r'|| orig_dev->name[0] == 'u')
			skb->pon_mac_filter_flag |= PON_MAC_FILTER_RX_CALL_HOOK;
		if (pon_check_mac_hook(skb) == -1)
		{
			kfree_skb(skb);
			return NET_RX_DROP;
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
				kfree_skb(skb);
				return NET_RX_DROP;
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
#endif
/*
#if defined(TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND)
	if ((portbind_sw_prior_hook) && (portbind_sw_prior_hook(skb) == 1)) {
#else
	if (portbind_sw_hook && (portbind_sw_hook() == 1)) {
#endif
*/


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
#if defined(TCSUPPORT_FTP_THROUGHPUT)
	}
#endif
#endif
#endif


	__this_cpu_inc(softnet_data.processed);
	skb_reset_network_header(skb);
	skb_reset_transport_header(skb);
	skb->mac_len = skb->network_header - skb->mac_header;

	pt_prev = NULL;

	rcu_read_lock();

#ifdef CONFIG_NET_CLS_ACT
	if (skb->tc_verd & TC_NCLS) {
		skb->tc_verd = CLR_TC_NCLS(skb->tc_verd);
		goto ncls;
	}
#endif

	list_for_each_entry_rcu(ptype, &ptype_all, list) {
		if (ptype->dev == null_or_orig || ptype->dev == skb->dev ||
		    ptype->dev == orig_dev) {
			if (pt_prev)
				ret = deliver_skb(skb, pt_prev, orig_dev);
			pt_prev = ptype;
		}
	}

#ifdef CONFIG_NET_CLS_ACT
	skb = handle_ing(skb, &pt_prev, &ret, orig_dev);
	if (!skb)
		goto out;
ncls:
#endif

	/* Handle special case of bridge or macvlan */
	rx_handler = rcu_dereference(skb->dev->rx_handler);
	if (rx_handler) {
		if (pt_prev) {
			ret = deliver_skb(skb, pt_prev, orig_dev);
			pt_prev = NULL;
		}
		skb = rx_handler(skb);
		if (!skb)
			goto out;
	}
#ifdef CONFIG_SMUX
			/* 
			 * Transfer all packets from PVC Device to Smux Device
			 */
		   if((orig_dev->priv_flags & IFF_RSMUX) && 
			smux_pkt_recv_hook
			#ifdef TCSUPPORT_XPON_HAL_API_EXT
			&&!(skb->mark&DOWNSTREAM_SIMULATION_MASK)
			#endif
			) {
			atomic_inc(&skb->users);
				  ret = smux_pkt_recv_hook(skb, skb->dev, orig_dev);		  
#if ((defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN) ) || defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
#ifdef TCSUPPORT_VLAN_TAG
				  if (skb) {
					skb->vlan_tag_flag |= VLAN_TAG_FROM_INDEV;
				  }
#endif			  
#endif
			}
			else {
#endif /* CONFIG_SMUX */

	/*
	 * Make sure frames received on VLAN interfaces stacked on
	 * bonding interfaces still make their way to any base bonding
	 * device that may have registered for a specific ptype.  The
	 * handler may have to adjust skb->dev and orig_dev.
	 */
	orig_or_bond = orig_dev;
	if ((skb->dev->priv_flags & IFF_802_1Q_VLAN) &&
	    (vlan_dev_real_dev(skb->dev)->priv_flags & IFF_BONDING)) {
		orig_or_bond = vlan_dev_real_dev(skb->dev);
	}

	type = skb->protocol;
	list_for_each_entry_rcu(ptype,
			&ptype_base[ntohs(type) & PTYPE_HASH_MASK], list) {
		if (ptype->type == type && (ptype->dev == null_or_orig ||
		     ptype->dev == skb->dev || ptype->dev == orig_dev ||
		     ptype->dev == orig_or_bond)) {
			if (pt_prev)
				ret = deliver_skb(skb, pt_prev, orig_dev);
			pt_prev = ptype;
		}
	}
#ifdef CONFIG_SMUX
	}
#endif /* CONFIG_SMUX */

	if (pt_prev) {
		ret = pt_prev->func(skb, skb->dev, pt_prev, orig_dev);
	} else {
		kfree_skb(skb);
		/* Jamal, now you will not able to escape explaining
		 * me how you were going to use this. :-)
		 */
		ret = NET_RX_DROP;
	}

out:
	rcu_read_unlock();
	return ret;
}

/**
 *	netif_receive_skb - process receive buffer from network
 *	@skb: buffer to process
 *
 *	netif_receive_skb() is the main receive data processing function.
 *	It always succeeds. The buffer may be dropped during processing
 *	for congestion control or by the protocol layers.
 *
 *	This function may only be called from softirq context and interrupts
 *	should be enabled.
 *
 *	Return values (usually ignored):
 *	NET_RX_SUCCESS: no congestion
 *	NET_RX_DROP: packet was dropped
 */
__IMEM int netif_receive_skb(struct sk_buff *skb)
{
	if (netdev_tstamp_prequeue)
		net_timestamp_check(skb);

	if (skb_defer_rx_timestamp(skb))
		return NET_RX_SUCCESS;

#ifdef CONFIG_RPS
	{
		struct rps_dev_flow voidflow, *rflow = &voidflow;
		int cpu, ret;

		rcu_read_lock();

		cpu = get_rps_cpu(skb->dev, skb, &rflow);

		if (cpu >= 0) {
			ret = enqueue_to_backlog(skb, cpu, &rflow->last_qtail);
			rcu_read_unlock();
		} else {
			rcu_read_unlock();
			ret = __netif_receive_skb(skb);
		}

		return ret;
	}
#else
	return __netif_receive_skb(skb);
#endif
}
EXPORT_SYMBOL(netif_receive_skb);

/* Network device is going away, flush any packets still pending
 * Called with irqs disabled.
 */
static void flush_backlog(void *arg)
{
	struct net_device *dev = arg;
	struct softnet_data *sd = &__get_cpu_var(softnet_data);
	struct sk_buff *skb, *tmp;

	rps_lock(sd);
	skb_queue_walk_safe(&sd->input_pkt_queue, skb, tmp) {
		if (skb->dev == dev) {
			__skb_unlink(skb, &sd->input_pkt_queue);
			kfree_skb(skb);
			input_queue_head_incr(sd);
		}
	}
	rps_unlock(sd);

	skb_queue_walk_safe(&sd->process_queue, skb, tmp) {
		if (skb->dev == dev) {
			__skb_unlink(skb, &sd->process_queue);
			kfree_skb(skb);
			input_queue_head_incr(sd);
		}
	}
}

static int napi_gro_complete(struct sk_buff *skb)
{
	struct packet_type *ptype;
	__be16 type = skb->protocol;
	struct list_head *head = &ptype_base[ntohs(type) & PTYPE_HASH_MASK];
	int err = -ENOENT;

	if (NAPI_GRO_CB(skb)->count == 1) {
		skb_shinfo(skb)->gso_size = 0;
		goto out;
	}

	rcu_read_lock();
	list_for_each_entry_rcu(ptype, head, list) {
		if (ptype->type != type || ptype->dev || !ptype->gro_complete)
			continue;

		err = ptype->gro_complete(skb);
		break;
	}
	rcu_read_unlock();

	if (err) {
		WARN_ON(&ptype->list == head);
		kfree_skb(skb);
		return NET_RX_SUCCESS;
	}

out:
	return netif_receive_skb(skb);
}

static void napi_gro_flush(struct napi_struct *napi)
{
	struct sk_buff *skb, *next;

	for (skb = napi->gro_list; skb; skb = next) {
		next = skb->next;
		skb->next = NULL;
		napi_gro_complete(skb);
	}

	napi->gro_count = 0;
	napi->gro_list = NULL;
}

enum gro_result dev_gro_receive(struct napi_struct *napi, struct sk_buff *skb)
{
	struct sk_buff **pp = NULL;
	struct packet_type *ptype;
	__be16 type = skb->protocol;
	struct list_head *head = &ptype_base[ntohs(type) & PTYPE_HASH_MASK];
	int same_flow;
	int mac_len;
	enum gro_result ret;

	if (!(skb->dev->features & NETIF_F_GRO) || netpoll_rx_on(skb))
		goto normal;

	if (skb_is_gso(skb) || skb_has_frags(skb))
		goto normal;

	rcu_read_lock();
	list_for_each_entry_rcu(ptype, head, list) {
		if (ptype->type != type || ptype->dev || !ptype->gro_receive)
			continue;

		skb_set_network_header(skb, skb_gro_offset(skb));
		mac_len = skb->network_header - skb->mac_header;
		skb->mac_len = mac_len;
		NAPI_GRO_CB(skb)->same_flow = 0;
		NAPI_GRO_CB(skb)->flush = 0;
		NAPI_GRO_CB(skb)->free = 0;

		pp = ptype->gro_receive(&napi->gro_list, skb);
		break;
	}
	rcu_read_unlock();

	if (&ptype->list == head)
		goto normal;

	same_flow = NAPI_GRO_CB(skb)->same_flow;
	ret = NAPI_GRO_CB(skb)->free ? GRO_MERGED_FREE : GRO_MERGED;

	if (pp) {
		struct sk_buff *nskb = *pp;

		*pp = nskb->next;
		nskb->next = NULL;
		napi_gro_complete(nskb);
		napi->gro_count--;
	}

	if (same_flow)
		goto ok;

	if (NAPI_GRO_CB(skb)->flush || napi->gro_count >= MAX_GRO_SKBS)
		goto normal;

	napi->gro_count++;
	NAPI_GRO_CB(skb)->count = 1;
	skb_shinfo(skb)->gso_size = skb_gro_len(skb);
	skb->next = napi->gro_list;
	napi->gro_list = skb;
	ret = GRO_HELD;

pull:
	if (skb_headlen(skb) < skb_gro_offset(skb)) {
		int grow = skb_gro_offset(skb) - skb_headlen(skb);

		BUG_ON(skb->end - skb->tail < grow);

		memcpy(skb_tail_pointer(skb), NAPI_GRO_CB(skb)->frag0, grow);

		skb->tail += grow;
		skb->data_len -= grow;

		skb_shinfo(skb)->frags[0].page_offset += grow;
		skb_shinfo(skb)->frags[0].size -= grow;

		if (unlikely(!skb_shinfo(skb)->frags[0].size)) {
			put_page(skb_shinfo(skb)->frags[0].page);
			memmove(skb_shinfo(skb)->frags,
				skb_shinfo(skb)->frags + 1,
				--skb_shinfo(skb)->nr_frags * sizeof(skb_frag_t));
		}
	}

ok:
	return ret;

normal:
	ret = GRO_NORMAL;
	goto pull;
}
EXPORT_SYMBOL(dev_gro_receive);

static gro_result_t
__napi_gro_receive(struct napi_struct *napi, struct sk_buff *skb)
{
	struct sk_buff *p;

	for (p = napi->gro_list; p; p = p->next) {
		NAPI_GRO_CB(p)->same_flow =
			(p->dev == skb->dev) &&
			!compare_ether_header(skb_mac_header(p),
					      skb_gro_mac_header(skb));
		NAPI_GRO_CB(p)->flush = 0;
	}

	return dev_gro_receive(napi, skb);
}

gro_result_t napi_skb_finish(gro_result_t ret, struct sk_buff *skb)
{
	switch (ret) {
	case GRO_NORMAL:
		if (netif_receive_skb(skb))
			ret = GRO_DROP;
		break;

	case GRO_DROP:
	case GRO_MERGED_FREE:
		kfree_skb(skb);
		break;

	case GRO_HELD:
	case GRO_MERGED:
		break;
	}

	return ret;
}
EXPORT_SYMBOL(napi_skb_finish);

void skb_gro_reset_offset(struct sk_buff *skb)
{
	NAPI_GRO_CB(skb)->data_offset = 0;
	NAPI_GRO_CB(skb)->frag0 = NULL;
	NAPI_GRO_CB(skb)->frag0_len = 0;

	if (skb->mac_header == skb->tail &&
	    !PageHighMem(skb_shinfo(skb)->frags[0].page)) {
		NAPI_GRO_CB(skb)->frag0 =
			page_address(skb_shinfo(skb)->frags[0].page) +
			skb_shinfo(skb)->frags[0].page_offset;
		NAPI_GRO_CB(skb)->frag0_len = skb_shinfo(skb)->frags[0].size;
	}
}
EXPORT_SYMBOL(skb_gro_reset_offset);

gro_result_t napi_gro_receive(struct napi_struct *napi, struct sk_buff *skb)
{
	skb_gro_reset_offset(skb);

	return napi_skb_finish(__napi_gro_receive(napi, skb), skb);
}
EXPORT_SYMBOL(napi_gro_receive);

void napi_reuse_skb(struct napi_struct *napi, struct sk_buff *skb)
{
	__skb_pull(skb, skb_headlen(skb));
	skb_reserve(skb, NET_IP_ALIGN - skb_headroom(skb));

	napi->skb = skb;
}
EXPORT_SYMBOL(napi_reuse_skb);

struct sk_buff *napi_get_frags(struct napi_struct *napi)
{
	struct sk_buff *skb = napi->skb;

	if (!skb) {
		skb = netdev_alloc_skb_ip_align(napi->dev, GRO_MAX_HEAD);
		if (skb)
			napi->skb = skb;
	}
	return skb;
}
EXPORT_SYMBOL(napi_get_frags);

gro_result_t napi_frags_finish(struct napi_struct *napi, struct sk_buff *skb,
			       gro_result_t ret)
{
	switch (ret) {
	case GRO_NORMAL:
	case GRO_HELD:
		skb->protocol = eth_type_trans(skb, skb->dev);

		if (ret == GRO_HELD)
			skb_gro_pull(skb, -ETH_HLEN);
		else if (netif_receive_skb(skb))
			ret = GRO_DROP;
		break;

	case GRO_DROP:
	case GRO_MERGED_FREE:
		napi_reuse_skb(napi, skb);
		break;

	case GRO_MERGED:
		break;
	}

	return ret;
}
EXPORT_SYMBOL(napi_frags_finish);

struct sk_buff *napi_frags_skb(struct napi_struct *napi)
{
	struct sk_buff *skb = napi->skb;
	struct ethhdr *eth;
	unsigned int hlen;
	unsigned int off;

	napi->skb = NULL;

	skb_reset_mac_header(skb);
	skb_gro_reset_offset(skb);

	off = skb_gro_offset(skb);
	hlen = off + sizeof(*eth);
	eth = skb_gro_header_fast(skb, off);
	if (skb_gro_header_hard(skb, hlen)) {
		eth = skb_gro_header_slow(skb, hlen, off);
		if (unlikely(!eth)) {
			napi_reuse_skb(napi, skb);
			skb = NULL;
			goto out;
		}
	}

	skb_gro_pull(skb, sizeof(*eth));

	/*
	 * This works because the only protocols we care about don't require
	 * special handling.  We'll fix it up properly at the end.
	 */
	skb->protocol = eth->h_proto;

out:
	return skb;
}
EXPORT_SYMBOL(napi_frags_skb);

gro_result_t napi_gro_frags(struct napi_struct *napi)
{
	struct sk_buff *skb = napi_frags_skb(napi);

	if (!skb)
		return GRO_DROP;

	return napi_frags_finish(napi, skb, __napi_gro_receive(napi, skb));
}
EXPORT_SYMBOL(napi_gro_frags);

/*
 * net_rps_action sends any pending IPI's for rps.
 * Note: called with local irq disabled, but exits with local irq enabled.
 */
static void net_rps_action_and_irq_enable(struct softnet_data *sd)
{
#ifdef CONFIG_RPS
	struct softnet_data *remsd = sd->rps_ipi_list;

	if (remsd) {
		sd->rps_ipi_list = NULL;

		local_irq_enable();

		/* Send pending IPI's to kick RPS processing on remote cpus. */
		while (remsd) {
			struct softnet_data *next = remsd->rps_ipi_next;

			if (cpu_online(remsd->cpu))
				__smp_call_function_single(remsd->cpu,
							   &remsd->csd, 0);
			remsd = next;
		}
	} else
#endif
		local_irq_enable();
}

__IMEM static int process_backlog(struct napi_struct *napi, int quota)
{
	int work = 0;
	struct softnet_data *sd = container_of(napi, struct softnet_data, backlog);
#ifdef TCSUPPORT_DOWNSTREAM_QOS
	int i = 0;
	int total_queue_len = 0;
#endif	


#ifdef CONFIG_RPS
	/* Check if we have pending ipi, its better to send them now,
	 * not waiting net_rx_action() end.
	 */
	if (sd->rps_ipi_list) {
		local_irq_disable();
		net_rps_action_and_irq_enable(sd);
	}
#endif
	napi->weight = weight_p;
	local_irq_disable();
	while (work < quota) {
		struct sk_buff *skb;
		unsigned int qlen;

		while ((skb = __skb_dequeue(&sd->process_queue))) {
			local_irq_enable();
			__netif_receive_skb(skb);
			local_irq_disable();
			input_queue_head_incr(sd);
			if (++work >= quota) {
				local_irq_enable();
				return work;
			}
		}

		rps_lock(sd);
#ifdef TCSUPPORT_DOWNSTREAM_QOS
		total_queue_len = 0;
		if(downstream_qos_enable){
			for( i = 0 ; i < PRIORITY_QUEUE_NUM; i++){
				qlen = skb_queue_len(&sd->pri_queue[i]);
				total_queue_len += qlen;
				if (qlen)
					skb_queue_splice_tail_init(&sd->pri_queue[i], &sd->process_queue);
			}
		}
#endif
		qlen = skb_queue_len(&sd->input_pkt_queue);
#ifdef TCSUPPORT_DOWNSTREAM_QOS
		total_queue_len += qlen;
#endif
		if (qlen)
			skb_queue_splice_tail_init(&sd->input_pkt_queue,
						   &sd->process_queue);
#ifdef TCSUPPORT_DOWNSTREAM_QOS
		if (total_queue_len < quota - work) {
#else
		if (qlen < quota - work) {
#endif
			/*
			 * Inline a custom version of __napi_complete().
			 * only current cpu owns and manipulates this napi,
			 * and NAPI_STATE_SCHED is the only possible flag set on backlog.
			 * we can use a plain write instead of clear_bit(),
			 * and we dont need an smp_mb() memory barrier.
			 */
			list_del(&napi->poll_list);
			napi->state = 0;

#ifdef TCSUPPORT_DOWNSTREAM_QOS
			quota = work + total_queue_len;
#else
			quota = work + qlen;
#endif
		}
		rps_unlock(sd);
	}
	local_irq_enable();

	return work;
}

/**
 * __napi_schedule - schedule for receive
 * @n: entry to schedule
 *
 * The entry's receive function will be scheduled to run
 */
__IMEM void __napi_schedule(struct napi_struct *n)
{
	unsigned long flags;

	local_irq_save(flags);
	____napi_schedule(&__get_cpu_var(softnet_data), n);
	local_irq_restore(flags);
}
EXPORT_SYMBOL(__napi_schedule);

__IMEM void __napi_complete(struct napi_struct *n)
{
	BUG_ON(!test_bit(NAPI_STATE_SCHED, &n->state));
	BUG_ON(n->gro_list);

	list_del(&n->poll_list);
	smp_mb__before_clear_bit();
	clear_bit(NAPI_STATE_SCHED, &n->state);
}
EXPORT_SYMBOL(__napi_complete);

void napi_complete(struct napi_struct *n)
{
	unsigned long flags;

	/*
	 * don't let napi dequeue from the cpu poll list
	 * just in case its running on a different cpu
	 */
	if (unlikely(test_bit(NAPI_STATE_NPSVC, &n->state)))
		return;

	napi_gro_flush(n);
	local_irq_save(flags);
	__napi_complete(n);
	local_irq_restore(flags);
}
EXPORT_SYMBOL(napi_complete);

void netif_napi_add(struct net_device *dev, struct napi_struct *napi,
		    int (*poll)(struct napi_struct *, int), int weight)
{
	INIT_LIST_HEAD(&napi->poll_list);
	napi->gro_count = 0;
	napi->gro_list = NULL;
	napi->skb = NULL;
	napi->poll = poll;
	napi->weight = weight;
	list_add(&napi->dev_list, &dev->napi_list);
	napi->dev = dev;
#ifdef CONFIG_NETPOLL
	spin_lock_init(&napi->poll_lock);
	napi->poll_owner = -1;
#endif
	set_bit(NAPI_STATE_SCHED, &napi->state);
}
EXPORT_SYMBOL(netif_napi_add);

void netif_napi_del(struct napi_struct *napi)
{
	struct sk_buff *skb, *next;

	list_del_init(&napi->dev_list);
	napi_free_frags(napi);

	for (skb = napi->gro_list; skb; skb = next) {
		next = skb->next;
		skb->next = NULL;
		kfree_skb(skb);
	}

	napi->gro_list = NULL;
	napi->gro_count = 0;
}
EXPORT_SYMBOL(netif_napi_del);

__IMEM static void net_rx_action(struct softirq_action *h)
{
	struct softnet_data *sd = &__get_cpu_var(softnet_data);
	unsigned long time_limit = jiffies + 2;
	int budget = netdev_budget;
	void *have;

	local_irq_disable();
	while (!list_empty(&sd->poll_list)) {
		struct napi_struct *n;
		int work, weight;

		/* If softirq window is exhuasted then punt.
		 * Allow this to run for 2 jiffies since which will allow
		 * an average latency of 1.5/HZ.
		 */
		if (unlikely(budget <= 0 || time_after(jiffies, time_limit))){
		/*clear watchdog counter because system will reboot in big 
			 * traffic, shnwind*/
			 tc3162wdog_kick();			
			goto softnet_break;
		}

		local_irq_enable();

		/* Even though interrupts have been re-enabled, this
		 * access is safe because interrupts can only add new
		 * entries to the tail of this list, and only ->poll()
		 * calls can remove this head entry from the list.
		 */
		n = list_first_entry(&sd->poll_list, struct napi_struct, poll_list);

		have = netpoll_poll_lock(n);

		weight = n->weight;

		/* This NAPI_STATE_SCHED test is for avoiding a race
		 * with netpoll's poll_napi().  Only the entity which
		 * obtains the lock and sees NAPI_STATE_SCHED set will
		 * actually make the ->poll() call.  Therefore we avoid
		 * accidently calling ->poll() when NAPI is not scheduled.
		 */
		work = 0;
		if (test_bit(NAPI_STATE_SCHED, &n->state)) {
			work = n->poll(n, weight);
			trace_napi_poll(n);
		}

		WARN_ON_ONCE(work > weight);

		budget -= work;

		local_irq_disable();

		/* Drivers must not modify the NAPI state if they
		 * consume the entire weight.  In such cases this code
		 * still "owns" the NAPI instance and therefore can
		 * move the instance around on the list at-will.
		 */
		if (unlikely(work == weight)) {
			if (unlikely(napi_disable_pending(n))) {
				local_irq_enable();
				napi_complete(n);
				local_irq_disable();
			} else
				list_move_tail(&n->poll_list, &sd->poll_list);
		}

		netpoll_poll_unlock(have);
	}
out:
	net_rps_action_and_irq_enable(sd);

#ifdef CONFIG_NET_DMA
	/*
	 * There may not be any more sk_buffs coming right now, so push
	 * any pending DMA copies to hardware
	 */
	dma_issue_pending_all();
#endif

	return;

softnet_break:
	sd->time_squeeze++;
	__raise_softirq_irqoff(NET_RX_SOFTIRQ);
	goto out;
}

static gifconf_func_t *gifconf_list[NPROTO];

/**
 *	register_gifconf	-	register a SIOCGIF handler
 *	@family: Address family
 *	@gifconf: Function handler
 *
 *	Register protocol dependent address dumping routines. The handler
 *	that is passed must not be freed or reused until it has been replaced
 *	by another handler.
 */
int register_gifconf(unsigned int family, gifconf_func_t *gifconf)
{
	if (family >= NPROTO)
		return -EINVAL;
	gifconf_list[family] = gifconf;
	return 0;
}
EXPORT_SYMBOL(register_gifconf);


/*
 *	Map an interface index to its name (SIOCGIFNAME)
 */

/*
 *	We need this ioctl for efficient implementation of the
 *	if_indextoname() function required by the IPv6 API.  Without
 *	it, we would have to search all the interfaces to find a
 *	match.  --pb
 */

static int dev_ifname(struct net *net, struct ifreq __user *arg)
{
	struct net_device *dev;
	struct ifreq ifr;

	/*
	 *	Fetch the caller's info block.
	 */

	if (copy_from_user(&ifr, arg, sizeof(struct ifreq)))
		return -EFAULT;

	rcu_read_lock();
	dev = dev_get_by_index_rcu(net, ifr.ifr_ifindex);
	if (!dev) {
		rcu_read_unlock();
		return -ENODEV;
	}

	strcpy(ifr.ifr_name, dev->name);
	rcu_read_unlock();

	if (copy_to_user(arg, &ifr, sizeof(struct ifreq)))
		return -EFAULT;
	return 0;
}

/*
 *	Perform a SIOCGIFCONF call. This structure will change
 *	size eventually, and there is nothing I can do about it.
 *	Thus we will need a 'compatibility mode'.
 */

static int dev_ifconf(struct net *net, char __user *arg)
{
	struct ifconf ifc;
	struct net_device *dev;
	char __user *pos;
	int len;
	int total;
	int i;

	/*
	 *	Fetch the caller's info block.
	 */

	if (copy_from_user(&ifc, arg, sizeof(struct ifconf)))
		return -EFAULT;

	pos = ifc.ifc_buf;
	len = ifc.ifc_len;

	/*
	 *	Loop over the interfaces, and write an info block for each.
	 */

	total = 0;
	for_each_netdev(net, dev) {
		for (i = 0; i < NPROTO; i++) {
			if (gifconf_list[i]) {
				int done;
				if (!pos)
					done = gifconf_list[i](dev, NULL, 0);
				else
					done = gifconf_list[i](dev, pos + total,
							       len - total);
				if (done < 0)
					return -EFAULT;
				total += done;
			}
		}
	}

	/*
	 *	All done.  Write the updated control block back to the caller.
	 */
	ifc.ifc_len = total;

	/*
	 * 	Both BSD and Solaris return 0 here, so we do too.
	 */
	return copy_to_user(arg, &ifc, sizeof(struct ifconf)) ? -EFAULT : 0;
}

#ifdef CONFIG_PROC_FS
/*
 *	This is invoked by the /proc filesystem handler to display a device
 *	in detail.
 */
void *dev_seq_start(struct seq_file *seq, loff_t *pos)
	__acquires(RCU)
{
	struct net *net = seq_file_net(seq);
	loff_t off;
	struct net_device *dev;

	rcu_read_lock();
	if (!*pos)
		return SEQ_START_TOKEN;

	off = 1;
	for_each_netdev_rcu(net, dev)
		if (off++ == *pos)
			return dev;

	return NULL;
}

void *dev_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct net_device *dev = (v == SEQ_START_TOKEN) ?
				  first_net_device(seq_file_net(seq)) :
				  next_net_device((struct net_device *)v);

	++*pos;
	return rcu_dereference(dev);
}

void dev_seq_stop(struct seq_file *seq, void *v)
	__releases(RCU)
{
	rcu_read_unlock();
}

static void dev_seq_printf_stats(struct seq_file *seq, struct net_device *dev)
{
	struct rtnl_link_stats64 temp;
	const struct rtnl_link_stats64 *stats = dev_get_stats(dev, &temp);

	seq_printf(seq, "%6s: %7llu %7llu %4llu %4llu %4llu %5llu %10llu %9llu "
		   "%8llu %7llu %4llu %4llu %4llu %5llu %7llu %10llu\n",
		   dev->name, stats->rx_bytes, stats->rx_packets,
		   stats->rx_errors,
		   stats->rx_dropped + stats->rx_missed_errors,
		   stats->rx_fifo_errors,
		   stats->rx_length_errors + stats->rx_over_errors +
		    stats->rx_crc_errors + stats->rx_frame_errors,
		   stats->rx_compressed, stats->multicast,
		   stats->tx_bytes, stats->tx_packets,
		   stats->tx_errors, stats->tx_dropped,
		   stats->tx_fifo_errors, stats->collisions,
		   stats->tx_carrier_errors +
		    stats->tx_aborted_errors +
		    stats->tx_window_errors +
		    stats->tx_heartbeat_errors,
		   stats->tx_compressed);
}

/*
 *	Called from the PROCfs module. This now uses the new arbitrary sized
 *	/proc/net interface to create /proc/net/dev
 */
static int dev_seq_show(struct seq_file *seq, void *v)
{
	if (v == SEQ_START_TOKEN)
		seq_puts(seq, "Inter-|   Receive                            "
			      "                    |  Transmit\n"
			      " face |bytes    packets errs drop fifo frame "
			      "compressed multicast|bytes    packets errs "
			      "drop fifo colls carrier compressed\n");
	else
		dev_seq_printf_stats(seq, v);
	return 0;
}

static struct softnet_data *softnet_get_online(loff_t *pos)
{
	struct softnet_data *sd = NULL;

	while (*pos < nr_cpu_ids)
		if (cpu_online(*pos)) {
			sd = &per_cpu(softnet_data, *pos);
			break;
		} else
			++*pos;
	return sd;
}

static void *softnet_seq_start(struct seq_file *seq, loff_t *pos)
{
	return softnet_get_online(pos);
}

static void *softnet_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	++*pos;
	return softnet_get_online(pos);
}

static void softnet_seq_stop(struct seq_file *seq, void *v)
{
}

static int softnet_seq_show(struct seq_file *seq, void *v)
{
	struct softnet_data *sd = v;

	seq_printf(seq, "%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x\n",
		   sd->processed, sd->dropped, sd->time_squeeze, 0,
		   0, 0, 0, 0, /* was fastroute */
		   sd->cpu_collision, sd->received_rps);
	return 0;
}

static const struct seq_operations dev_seq_ops = {
	.start = dev_seq_start,
	.next  = dev_seq_next,
	.stop  = dev_seq_stop,
	.show  = dev_seq_show,
};

static int dev_seq_open(struct inode *inode, struct file *file)
{
	return seq_open_net(inode, file, &dev_seq_ops,
			    sizeof(struct seq_net_private));
}

static const struct file_operations dev_seq_fops = {
	.owner	 = THIS_MODULE,
	.open    = dev_seq_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release_net,
};

static const struct seq_operations softnet_seq_ops = {
	.start = softnet_seq_start,
	.next  = softnet_seq_next,
	.stop  = softnet_seq_stop,
	.show  = softnet_seq_show,
};

static int softnet_seq_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &softnet_seq_ops);
}

static const struct file_operations softnet_seq_fops = {
	.owner	 = THIS_MODULE,
	.open    = softnet_seq_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release,
};

static void *ptype_get_idx(loff_t pos)
{
	struct packet_type *pt = NULL;
	loff_t i = 0;
	int t;

	list_for_each_entry_rcu(pt, &ptype_all, list) {
		if (i == pos)
			return pt;
		++i;
	}

	for (t = 0; t < PTYPE_HASH_SIZE; t++) {
		list_for_each_entry_rcu(pt, &ptype_base[t], list) {
			if (i == pos)
				return pt;
			++i;
		}
	}
	return NULL;
}

static void *ptype_seq_start(struct seq_file *seq, loff_t *pos)
	__acquires(RCU)
{
	rcu_read_lock();
	return *pos ? ptype_get_idx(*pos - 1) : SEQ_START_TOKEN;
}

static void *ptype_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct packet_type *pt;
	struct list_head *nxt;
	int hash;

	++*pos;
	if (v == SEQ_START_TOKEN)
		return ptype_get_idx(0);

	pt = v;
	nxt = pt->list.next;
	if (pt->type == htons(ETH_P_ALL)) {
		if (nxt != &ptype_all)
			goto found;
		hash = 0;
		nxt = ptype_base[0].next;
	} else
		hash = ntohs(pt->type) & PTYPE_HASH_MASK;

	while (nxt == &ptype_base[hash]) {
		if (++hash >= PTYPE_HASH_SIZE)
			return NULL;
		nxt = ptype_base[hash].next;
	}
found:
	return list_entry(nxt, struct packet_type, list);
}

static void ptype_seq_stop(struct seq_file *seq, void *v)
	__releases(RCU)
{
	rcu_read_unlock();
}

static int ptype_seq_show(struct seq_file *seq, void *v)
{
	struct packet_type *pt = v;

	if (v == SEQ_START_TOKEN)
		seq_puts(seq, "Type Device      Function\n");
	else if (pt->dev == NULL || dev_net(pt->dev) == seq_file_net(seq)) {
		if (pt->type == htons(ETH_P_ALL))
			seq_puts(seq, "ALL ");
		else
			seq_printf(seq, "%04x", ntohs(pt->type));

		seq_printf(seq, " %-8s %pF\n",
			   pt->dev ? pt->dev->name : "", pt->func);
	}

	return 0;
}

static const struct seq_operations ptype_seq_ops = {
	.start = ptype_seq_start,
	.next  = ptype_seq_next,
	.stop  = ptype_seq_stop,
	.show  = ptype_seq_show,
};

static int ptype_seq_open(struct inode *inode, struct file *file)
{
	return seq_open_net(inode, file, &ptype_seq_ops,
			sizeof(struct seq_net_private));
}

static const struct file_operations ptype_seq_fops = {
	.owner	 = THIS_MODULE,
	.open    = ptype_seq_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release_net,
};


static int __net_init dev_proc_net_init(struct net *net)
{
	int rc = -ENOMEM;

	if (!proc_net_fops_create(net, "dev", S_IRUGO, &dev_seq_fops))
		goto out;
	if (!proc_net_fops_create(net, "softnet_stat", S_IRUGO, &softnet_seq_fops))
		goto out_dev;
	if (!proc_net_fops_create(net, "ptype", S_IRUGO, &ptype_seq_fops))
		goto out_softnet;

	if (wext_proc_init(net))
		goto out_ptype;
	rc = 0;
out:
	return rc;
out_ptype:
	proc_net_remove(net, "ptype");
out_softnet:
	proc_net_remove(net, "softnet_stat");
out_dev:
	proc_net_remove(net, "dev");
	goto out;
}

static void __net_exit dev_proc_net_exit(struct net *net)
{
	wext_proc_exit(net);

	proc_net_remove(net, "ptype");
	proc_net_remove(net, "softnet_stat");
	proc_net_remove(net, "dev");
#ifdef TCSUPPORT_PON_IP_HOST
	remove_proc_entry("tc3162/pon_wanIf", 0);
#endif

}

static struct pernet_operations __net_initdata dev_proc_ops = {
	.init = dev_proc_net_init,
	.exit = dev_proc_net_exit,
};

static int __init dev_proc_init(void)
{
#ifdef TCSUPPORT_PON_IP_HOST
	struct proc_dir_entry *voip_wanIf_proc = NULL;

	voip_wanIf_proc = create_proc_entry("tc3162/pon_wanIf", 0, NULL);
	voip_wanIf_proc->read_proc = pon_wanIf_read_proc;
	voip_wanIf_proc->write_proc = pon_wanIf_write_proc;
#endif

	return register_pernet_subsys(&dev_proc_ops);
}
#else
#define dev_proc_init() 0
#endif	/* CONFIG_PROC_FS */


/**
 *	netdev_set_master	-	set up master/slave pair
 *	@slave: slave device
 *	@master: new master device
 *
 *	Changes the master device of the slave. Pass %NULL to break the
 *	bonding. The caller must hold the RTNL semaphore. On a failure
 *	a negative errno code is returned. On success the reference counts
 *	are adjusted, %RTM_NEWLINK is sent to the routing socket and the
 *	function returns zero.
 */
int netdev_set_master(struct net_device *slave, struct net_device *master)
{
	struct net_device *old = slave->master;

	ASSERT_RTNL();

	if (master) {
		if (old)
			return -EBUSY;
		dev_hold(master);
	}

	slave->master = master;

	if (old) {
		synchronize_net();
		dev_put(old);
	}
	if (master)
		slave->flags |= IFF_SLAVE;
	else
		slave->flags &= ~IFF_SLAVE;

	rtmsg_ifinfo(RTM_NEWLINK, slave, IFF_SLAVE);
	return 0;
}
EXPORT_SYMBOL(netdev_set_master);

static void dev_change_rx_flags(struct net_device *dev, int flags)
{
	const struct net_device_ops *ops = dev->netdev_ops;

	if ((dev->flags & IFF_UP) && ops->ndo_change_rx_flags)
		ops->ndo_change_rx_flags(dev, flags);
}

static int __dev_set_promiscuity(struct net_device *dev, int inc)
{
	unsigned short old_flags = dev->flags;
	uid_t uid;
	gid_t gid;

	ASSERT_RTNL();

	dev->flags |= IFF_PROMISC;
	dev->promiscuity += inc;
	if (dev->promiscuity == 0) {
		/*
		 * Avoid overflow.
		 * If inc causes overflow, untouch promisc and return error.
		 */
		if (inc < 0)
			dev->flags &= ~IFF_PROMISC;
		else {
			dev->promiscuity -= inc;
			printk(KERN_WARNING "%s: promiscuity touches roof, "
				"set promiscuity failed, promiscuity feature "
				"of device might be broken.\n", dev->name);
			return -EOVERFLOW;
		}
	}
	if (dev->flags != old_flags) {
		printk(KERN_INFO "device %s %s promiscuous mode\n",
		       dev->name, (dev->flags & IFF_PROMISC) ? "entered" :
							       "left");
		if (audit_enabled) {
			current_uid_gid(&uid, &gid);
			audit_log(current->audit_context, GFP_ATOMIC,
				AUDIT_ANOM_PROMISCUOUS,
				"dev=%s prom=%d old_prom=%d auid=%u uid=%u gid=%u ses=%u",
				dev->name, (dev->flags & IFF_PROMISC),
				(old_flags & IFF_PROMISC),
				audit_get_loginuid(current),
				uid, gid,
				audit_get_sessionid(current));
		}

		dev_change_rx_flags(dev, IFF_PROMISC);
	}
	return 0;
}

/**
 *	dev_set_promiscuity	- update promiscuity count on a device
 *	@dev: device
 *	@inc: modifier
 *
 *	Add or remove promiscuity from a device. While the count in the device
 *	remains above zero the interface remains promiscuous. Once it hits zero
 *	the device reverts back to normal filtering operation. A negative inc
 *	value is used to drop promiscuity on the device.
 *	Return 0 if successful or a negative errno code on error.
 */
int dev_set_promiscuity(struct net_device *dev, int inc)
{
	unsigned short old_flags = dev->flags;
	int err;

	err = __dev_set_promiscuity(dev, inc);
	if (err < 0)
		return err;
	if (dev->flags != old_flags)
		dev_set_rx_mode(dev);
	return err;
}
EXPORT_SYMBOL(dev_set_promiscuity);

/**
 *	dev_set_allmulti	- update allmulti count on a device
 *	@dev: device
 *	@inc: modifier
 *
 *	Add or remove reception of all multicast frames to a device. While the
 *	count in the device remains above zero the interface remains listening
 *	to all interfaces. Once it hits zero the device reverts back to normal
 *	filtering operation. A negative @inc value is used to drop the counter
 *	when releasing a resource needing all multicasts.
 *	Return 0 if successful or a negative errno code on error.
 */

int dev_set_allmulti(struct net_device *dev, int inc)
{
	unsigned short old_flags = dev->flags;

	ASSERT_RTNL();

	dev->flags |= IFF_ALLMULTI;
	dev->allmulti += inc;
	if (dev->allmulti == 0) {
		/*
		 * Avoid overflow.
		 * If inc causes overflow, untouch allmulti and return error.
		 */
		if (inc < 0)
			dev->flags &= ~IFF_ALLMULTI;
		else {
			dev->allmulti -= inc;
			printk(KERN_WARNING "%s: allmulti touches roof, "
				"set allmulti failed, allmulti feature of "
				"device might be broken.\n", dev->name);
			return -EOVERFLOW;
		}
	}
	if (dev->flags ^ old_flags) {
		dev_change_rx_flags(dev, IFF_ALLMULTI);
		dev_set_rx_mode(dev);
	}
	return 0;
}
EXPORT_SYMBOL(dev_set_allmulti);

/*
 *	Upload unicast and multicast address lists to device and
 *	configure RX filtering. When the device doesn't support unicast
 *	filtering it is put in promiscuous mode while unicast addresses
 *	are present.
 */
void __dev_set_rx_mode(struct net_device *dev)
{
	const struct net_device_ops *ops = dev->netdev_ops;

	/* dev_open will call this function so the list will stay sane. */
	if (!(dev->flags&IFF_UP))
		return;

	if (!netif_device_present(dev))
		return;

	if (ops->ndo_set_rx_mode)
		ops->ndo_set_rx_mode(dev);
	else {
		/* Unicast addresses changes may only happen under the rtnl,
		 * therefore calling __dev_set_promiscuity here is safe.
		 */
		if (!netdev_uc_empty(dev) && !dev->uc_promisc) {
			__dev_set_promiscuity(dev, 1);
			dev->uc_promisc = 1;
		} else if (netdev_uc_empty(dev) && dev->uc_promisc) {
			__dev_set_promiscuity(dev, -1);
			dev->uc_promisc = 0;
		}

		if (ops->ndo_set_multicast_list)
			ops->ndo_set_multicast_list(dev);
	}
}

void dev_set_rx_mode(struct net_device *dev)
{
	netif_addr_lock_bh(dev);
	__dev_set_rx_mode(dev);
	netif_addr_unlock_bh(dev);
}

/**
 *	dev_get_flags - get flags reported to userspace
 *	@dev: device
 *
 *	Get the combination of flag bits exported through APIs to userspace.
 */
unsigned dev_get_flags(const struct net_device *dev)
{
	unsigned flags;

	flags = (dev->flags & ~(IFF_PROMISC |
				IFF_ALLMULTI |
				IFF_RUNNING |
				IFF_LOWER_UP |
				IFF_DORMANT)) |
		(dev->gflags & (IFF_PROMISC |
				IFF_ALLMULTI));

	if (netif_running(dev)) {
		if (netif_oper_up(dev))
			flags |= IFF_RUNNING;
		if (netif_carrier_ok(dev))
			flags |= IFF_LOWER_UP;
		if (netif_dormant(dev))
			flags |= IFF_DORMANT;
	}

	return flags;
}
EXPORT_SYMBOL(dev_get_flags);

int __dev_change_flags(struct net_device *dev, unsigned int flags)
{
	int old_flags = dev->flags;
	int ret;

	ASSERT_RTNL();

	/*
	 *	Set the flags on our device.
	 */

	dev->flags = (flags & (IFF_DEBUG | IFF_NOTRAILERS | IFF_NOARP |
			       IFF_DYNAMIC | IFF_MULTICAST | IFF_PORTSEL |
			       IFF_AUTOMEDIA)) |
		     (dev->flags & (IFF_UP | IFF_VOLATILE | IFF_PROMISC |
				    IFF_ALLMULTI));

	/*
	 *	Load in the correct multicast list now the flags have changed.
	 */

	if ((old_flags ^ flags) & IFF_MULTICAST)
		dev_change_rx_flags(dev, IFF_MULTICAST);

	dev_set_rx_mode(dev);

	/*
	 *	Have we downed the interface. We handle IFF_UP ourselves
	 *	according to user attempts to set it, rather than blindly
	 *	setting it.
	 */

	ret = 0;
	if ((old_flags ^ flags) & IFF_UP) {	/* Bit is different  ? */
		ret = ((old_flags & IFF_UP) ? __dev_close : __dev_open)(dev);

		if (!ret)
			dev_set_rx_mode(dev);
	}

	if ((flags ^ dev->gflags) & IFF_PROMISC) {
		int inc = (flags & IFF_PROMISC) ? 1 : -1;

		dev->gflags ^= IFF_PROMISC;
		dev_set_promiscuity(dev, inc);
	}

	/* NOTE: order of synchronization of IFF_PROMISC and IFF_ALLMULTI
	   is important. Some (broken) drivers set IFF_PROMISC, when
	   IFF_ALLMULTI is requested not asking us and not reporting.
	 */
	if ((flags ^ dev->gflags) & IFF_ALLMULTI) {
		int inc = (flags & IFF_ALLMULTI) ? 1 : -1;

		dev->gflags ^= IFF_ALLMULTI;
		dev_set_allmulti(dev, inc);
	}

	return ret;
}

void __dev_notify_flags(struct net_device *dev, unsigned int old_flags)
{
	unsigned int changes = dev->flags ^ old_flags;

	if (changes & IFF_UP) {
		if (dev->flags & IFF_UP)
			call_netdevice_notifiers(NETDEV_UP, dev);
		else
			call_netdevice_notifiers(NETDEV_DOWN, dev);
	}

	if (dev->flags & IFF_UP &&
	    (changes & ~(IFF_UP | IFF_PROMISC | IFF_ALLMULTI | IFF_VOLATILE)))
		call_netdevice_notifiers(NETDEV_CHANGE, dev);
}

/**
 *	dev_change_flags - change device settings
 *	@dev: device
 *	@flags: device state flags
 *
 *	Change settings on device based state flags. The flags are
 *	in the userspace exported format.
 */
int dev_change_flags(struct net_device *dev, unsigned flags)
{
	int ret, changes;
	int old_flags = dev->flags;

	ret = __dev_change_flags(dev, flags);
	if (ret < 0)
		return ret;

	changes = old_flags ^ dev->flags;
	if (changes)
		rtmsg_ifinfo(RTM_NEWLINK, dev, changes);

	__dev_notify_flags(dev, old_flags);
	return ret;
}
EXPORT_SYMBOL(dev_change_flags);

/**
 *	dev_set_mtu - Change maximum transfer unit
 *	@dev: device
 *	@new_mtu: new transfer unit
 *
 *	Change the maximum transfer size of the network device.
 */
int dev_set_mtu(struct net_device *dev, int new_mtu)
{
	const struct net_device_ops *ops = dev->netdev_ops;
	int err;

	if (new_mtu == dev->mtu)
		return 0;

	/*	MTU must be positive.	 */
	if (new_mtu < 0)
		return -EINVAL;

	if (!netif_device_present(dev))
		return -ENODEV;

	err = 0;
	if (ops->ndo_change_mtu)
		err = ops->ndo_change_mtu(dev, new_mtu);
	else
		dev->mtu = new_mtu;

	if (!err && dev->flags & IFF_UP)
		call_netdevice_notifiers(NETDEV_CHANGEMTU, dev);
	return err;
}
EXPORT_SYMBOL(dev_set_mtu);

/**
 *	dev_set_mac_address - Change Media Access Control Address
 *	@dev: device
 *	@sa: new address
 *
 *	Change the hardware (MAC) address of the device
 */
int dev_set_mac_address(struct net_device *dev, struct sockaddr *sa)
{
	const struct net_device_ops *ops = dev->netdev_ops;
	int err;

	if (!ops->ndo_set_mac_address)
		return -EOPNOTSUPP;
	if (sa->sa_family != dev->type)
		return -EINVAL;
	if (!netif_device_present(dev))
		return -ENODEV;
	err = ops->ndo_set_mac_address(dev, sa);
	if (!err)
		call_netdevice_notifiers(NETDEV_CHANGEADDR, dev);
	return err;
}
EXPORT_SYMBOL(dev_set_mac_address);

/*
 *	Perform the SIOCxIFxxx calls, inside rcu_read_lock()
 */
static int dev_ifsioc_locked(struct net *net, struct ifreq *ifr, unsigned int cmd)
{
	int err;
	struct net_device *dev = dev_get_by_name_rcu(net, ifr->ifr_name);

	if (!dev)
		return -ENODEV;

	switch (cmd) {
	case SIOCGIFFLAGS:	/* Get interface flags */
		ifr->ifr_flags = (short) dev_get_flags(dev);
		return 0;

	case SIOCGIFMETRIC:	/* Get the metric on the interface
				   (currently unused) */
		ifr->ifr_metric = 0;
		return 0;

	case SIOCGIFMTU:	/* Get the MTU of a device */
		ifr->ifr_mtu = dev->mtu;
		return 0;

	case SIOCGIFHWADDR:
		if (!dev->addr_len)
			memset(ifr->ifr_hwaddr.sa_data, 0, sizeof ifr->ifr_hwaddr.sa_data);
		else
			memcpy(ifr->ifr_hwaddr.sa_data, dev->dev_addr,
			       min(sizeof ifr->ifr_hwaddr.sa_data, (size_t) dev->addr_len));
		ifr->ifr_hwaddr.sa_family = dev->type;
		return 0;

	case SIOCGIFSLAVE:
		err = -EINVAL;
		break;

	case SIOCGIFMAP:
		ifr->ifr_map.mem_start = dev->mem_start;
		ifr->ifr_map.mem_end   = dev->mem_end;
		ifr->ifr_map.base_addr = dev->base_addr;
		ifr->ifr_map.irq       = dev->irq;
		ifr->ifr_map.dma       = dev->dma;
		ifr->ifr_map.port      = dev->if_port;
		return 0;

	case SIOCGIFINDEX:
		ifr->ifr_ifindex = dev->ifindex;
		return 0;

	case SIOCGIFTXQLEN:
		ifr->ifr_qlen = dev->tx_queue_len;
		return 0;

	default:
		/* dev_ioctl() should ensure this case
		 * is never reached
		 */
		WARN_ON(1);
		err = -EINVAL;
		break;

	}
	return err;
}

/*
 *	Perform the SIOCxIFxxx calls, inside rtnl_lock()
 */
static int dev_ifsioc(struct net *net, struct ifreq *ifr, unsigned int cmd)
{
	int err;
	struct net_device *dev = __dev_get_by_name(net, ifr->ifr_name);
	const struct net_device_ops *ops;

	if (!dev)
		return -ENODEV;

	ops = dev->netdev_ops;

	switch (cmd) {
	case SIOCSIFFLAGS:	/* Set interface flags */
		return dev_change_flags(dev, ifr->ifr_flags);

	case SIOCSIFMETRIC:	/* Set the metric on the interface
				   (currently unused) */
		return -EOPNOTSUPP;

	case SIOCSIFMTU:	/* Set the MTU of a device */
		return dev_set_mtu(dev, ifr->ifr_mtu);

	case SIOCSIFHWADDR:
		return dev_set_mac_address(dev, &ifr->ifr_hwaddr);

	case SIOCSIFHWBROADCAST:
		if (ifr->ifr_hwaddr.sa_family != dev->type)
			return -EINVAL;
		memcpy(dev->broadcast, ifr->ifr_hwaddr.sa_data,
		       min(sizeof ifr->ifr_hwaddr.sa_data, (size_t) dev->addr_len));
		call_netdevice_notifiers(NETDEV_CHANGEADDR, dev);
		return 0;

	case SIOCSIFMAP:
		if (ops->ndo_set_config) {
			if (!netif_device_present(dev))
				return -ENODEV;
			return ops->ndo_set_config(dev, &ifr->ifr_map);
		}
		return -EOPNOTSUPP;

	case SIOCADDMULTI:
		if ((!ops->ndo_set_multicast_list && !ops->ndo_set_rx_mode) ||
		    ifr->ifr_hwaddr.sa_family != AF_UNSPEC)
			return -EINVAL;
		if (!netif_device_present(dev))
			return -ENODEV;
		return dev_mc_add_global(dev, ifr->ifr_hwaddr.sa_data);

	case SIOCDELMULTI:
		if ((!ops->ndo_set_multicast_list && !ops->ndo_set_rx_mode) ||
		    ifr->ifr_hwaddr.sa_family != AF_UNSPEC)
			return -EINVAL;
		if (!netif_device_present(dev))
			return -ENODEV;
		return dev_mc_del_global(dev, ifr->ifr_hwaddr.sa_data);

	case SIOCSIFTXQLEN:
		if (ifr->ifr_qlen < 0)
			return -EINVAL;
		dev->tx_queue_len = ifr->ifr_qlen;
		return 0;

	case SIOCSIFNAME:
		ifr->ifr_newname[IFNAMSIZ-1] = '\0';
		return dev_change_name(dev, ifr->ifr_newname);

	/*
	 *	Unknown or private ioctl
	 */
	default:
		if ((cmd >= SIOCDEVPRIVATE &&
		    cmd <= SIOCDEVPRIVATE + 15) ||
		    cmd == SIOCBONDENSLAVE ||
		    cmd == SIOCBONDRELEASE ||
		    cmd == SIOCBONDSETHWADDR ||
		    cmd == SIOCBONDSLAVEINFOQUERY ||
		    cmd == SIOCBONDINFOQUERY ||
		    cmd == SIOCBONDCHANGEACTIVE ||
		    cmd == SIOCGMIIPHY ||
		    cmd == SIOCGMIIREG ||
		    cmd == SIOCSMIIREG ||
		    cmd == SIOCBRADDIF ||
		    cmd == SIOCBRDELIF ||
		    cmd == SIOCSHWTSTAMP ||
		    cmd == SIOCWANDEV) {
			err = -EOPNOTSUPP;
			if (ops->ndo_do_ioctl) {
				if (netif_device_present(dev))
					err = ops->ndo_do_ioctl(dev, ifr, cmd);
				else
					err = -ENODEV;
			}
		} else
			err = -EINVAL;

	}
	return err;
}

/*
 *	This function handles all "interface"-type I/O control requests. The actual
 *	'doing' part of this is dev_ifsioc above.
 */

/**
 *	dev_ioctl	-	network device ioctl
 *	@net: the applicable net namespace
 *	@cmd: command to issue
 *	@arg: pointer to a struct ifreq in user space
 *
 *	Issue ioctl functions to devices. This is normally called by the
 *	user space syscall interfaces but can sometimes be useful for
 *	other purposes. The return value is the return from the syscall if
 *	positive or a negative errno code on error.
 */

int dev_ioctl(struct net *net, unsigned int cmd, void __user *arg)
{
	struct ifreq ifr;
	int ret;
	char *colon;

	/* One special case: SIOCGIFCONF takes ifconf argument
	   and requires shared lock, because it sleeps writing
	   to user space.
	 */

	if (cmd == SIOCGIFCONF) {
		rtnl_lock();
		ret = dev_ifconf(net, (char __user *) arg);
		rtnl_unlock();
		return ret;
	}
	if (cmd == SIOCGIFNAME)
		return dev_ifname(net, (struct ifreq __user *)arg);

	if (copy_from_user(&ifr, arg, sizeof(struct ifreq)))
		return -EFAULT;

	ifr.ifr_name[IFNAMSIZ-1] = 0;

	colon = strchr(ifr.ifr_name, ':');
	if (colon)
		*colon = 0;

	/*
	 *	See which interface the caller is talking about.
	 */

	switch (cmd) {
	/*
	 *	These ioctl calls:
	 *	- can be done by all.
	 *	- atomic and do not require locking.
	 *	- return a value
	 */
	case SIOCGIFFLAGS:
	case SIOCGIFMETRIC:
	case SIOCGIFMTU:
	case SIOCGIFHWADDR:
	case SIOCGIFSLAVE:
	case SIOCGIFMAP:
	case SIOCGIFINDEX:
	case SIOCGIFTXQLEN:
		dev_load(net, ifr.ifr_name);
		rcu_read_lock();
		ret = dev_ifsioc_locked(net, &ifr, cmd);
		rcu_read_unlock();
		if (!ret) {
			if (colon)
				*colon = ':';
			if (copy_to_user(arg, &ifr,
					 sizeof(struct ifreq)))
				ret = -EFAULT;
		}
		return ret;

	case SIOCETHTOOL:
		dev_load(net, ifr.ifr_name);
		rtnl_lock();
		ret = dev_ethtool(net, &ifr);
		rtnl_unlock();
		if (!ret) {
			if (colon)
				*colon = ':';
			if (copy_to_user(arg, &ifr,
					 sizeof(struct ifreq)))
				ret = -EFAULT;
		}
		return ret;

	/*
	 *	These ioctl calls:
	 *	- require superuser power.
	 *	- require strict serialization.
	 *	- return a value
	 */
	case SIOCGMIIPHY:
	case SIOCGMIIREG:
	case SIOCSIFNAME:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;
		dev_load(net, ifr.ifr_name);
		rtnl_lock();
		ret = dev_ifsioc(net, &ifr, cmd);
		rtnl_unlock();
		if (!ret) {
			if (colon)
				*colon = ':';
			if (copy_to_user(arg, &ifr,
					 sizeof(struct ifreq)))
				ret = -EFAULT;
		}
		return ret;

	/*
	 *	These ioctl calls:
	 *	- require superuser power.
	 *	- require strict serialization.
	 *	- do not return a value
	 */
	case SIOCSIFFLAGS:
	case SIOCSIFMETRIC:
	case SIOCSIFMTU:
	case SIOCSIFMAP:
	case SIOCSIFHWADDR:
	case SIOCSIFSLAVE:
	case SIOCADDMULTI:
	case SIOCDELMULTI:
	case SIOCSIFHWBROADCAST:
	case SIOCSIFTXQLEN:
	case SIOCSMIIREG:
	case SIOCBONDENSLAVE:
	case SIOCBONDRELEASE:
	case SIOCBONDSETHWADDR:
	case SIOCBONDCHANGEACTIVE:
	case SIOCBRADDIF:
	case SIOCBRDELIF:
	case SIOCSHWTSTAMP:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;
		/* fall through */
	case SIOCBONDSLAVEINFOQUERY:
	case SIOCBONDINFOQUERY:
		dev_load(net, ifr.ifr_name);
		rtnl_lock();
		ret = dev_ifsioc(net, &ifr, cmd);
		rtnl_unlock();
		return ret;

	case SIOCGIFMEM:
		/* Get the per device memory space. We can add this but
		 * currently do not support it */
	case SIOCSIFMEM:
		/* Set the per device memory buffer space.
		 * Not applicable in our case */
	case SIOCSIFLINK:
		return -EINVAL;

	/*
	 *	Unknown or private ioctl.
	 */
	default:
		if (cmd == SIOCWANDEV ||
		    (cmd >= SIOCDEVPRIVATE &&
		     cmd <= SIOCDEVPRIVATE + 15)) {
			dev_load(net, ifr.ifr_name);
			rtnl_lock();
			ret = dev_ifsioc(net, &ifr, cmd);
			rtnl_unlock();
			if (!ret && copy_to_user(arg, &ifr,
						 sizeof(struct ifreq)))
				ret = -EFAULT;
			return ret;
		}
		/* Take care of Wireless Extensions */
		if (cmd >= SIOCIWFIRST && cmd <= SIOCIWLAST)
			return wext_handle_ioctl(net, &ifr, cmd, arg);
		return -EINVAL;
	}
}


/**
 *	dev_new_index	-	allocate an ifindex
 *	@net: the applicable net namespace
 *
 *	Returns a suitable unique value for a new device interface
 *	number.  The caller must hold the rtnl semaphore or the
 *	dev_base_lock to be sure it remains unique.
 */
static int dev_new_index(struct net *net)
{
	static int ifindex;
	for (;;) {
		if (++ifindex <= 0)
			ifindex = 1;
		if (!__dev_get_by_index(net, ifindex))
			return ifindex;
	}
}

/* Delayed registration/unregisteration */
static LIST_HEAD(net_todo_list);

static void net_set_todo(struct net_device *dev)
{
	list_add_tail(&dev->todo_list, &net_todo_list);
}

static void rollback_registered_many(struct list_head *head)
{
	struct net_device *dev, *tmp;

	BUG_ON(dev_boot_phase);
	ASSERT_RTNL();

	list_for_each_entry_safe(dev, tmp, head, unreg_list) {
		/* Some devices call without registering
		 * for initialization unwind. Remove those
		 * devices and proceed with the remaining.
		 */
		if (dev->reg_state == NETREG_UNINITIALIZED) {
			pr_debug("unregister_netdevice: device %s/%p never "
				 "was registered\n", dev->name, dev);

			WARN_ON(1);
			list_del(&dev->unreg_list);
			continue;
		}

		BUG_ON(dev->reg_state != NETREG_REGISTERED);

		/* If device is running, close it first. */
		dev_close(dev);

		/* And unlink it from device chain. */
		unlist_netdevice(dev);

		dev->reg_state = NETREG_UNREGISTERING;
	}

	synchronize_net();

	list_for_each_entry(dev, head, unreg_list) {
		/* Shutdown queueing discipline. */
		dev_shutdown(dev);


		/* Notify protocols, that we are about to destroy
		   this device. They should clean all the things.
		*/
		call_netdevice_notifiers(NETDEV_UNREGISTER, dev);

		if (!dev->rtnl_link_ops ||
		    dev->rtnl_link_state == RTNL_LINK_INITIALIZED)
			rtmsg_ifinfo(RTM_DELLINK, dev, ~0U);

		/*
		 *	Flush the unicast and multicast chains
		 */
		dev_uc_flush(dev);
		dev_mc_flush(dev);

		if (dev->netdev_ops->ndo_uninit)
			dev->netdev_ops->ndo_uninit(dev);

		/* Notifier chain MUST detach us from master device. */
		WARN_ON(dev->master);

		/* Remove entries from kobject tree */
		netdev_unregister_kobject(dev);
	}

	/* Process any work delayed until the end of the batch */
	dev = list_first_entry(head, struct net_device, unreg_list);
	call_netdevice_notifiers(NETDEV_UNREGISTER_BATCH, dev);

	rcu_barrier();

	list_for_each_entry(dev, head, unreg_list)
		dev_put(dev);
}

static void rollback_registered(struct net_device *dev)
{
	LIST_HEAD(single);

	list_add(&dev->unreg_list, &single);
	rollback_registered_many(&single);
}

static void __netdev_init_queue_locks_one(struct net_device *dev,
					  struct netdev_queue *dev_queue,
					  void *_unused)
{
	spin_lock_init(&dev_queue->_xmit_lock);
	netdev_set_xmit_lockdep_class(&dev_queue->_xmit_lock, dev->type);
	dev_queue->xmit_lock_owner = -1;
}

static void netdev_init_queue_locks(struct net_device *dev)
{
	netdev_for_each_tx_queue(dev, __netdev_init_queue_locks_one, NULL);
	__netdev_init_queue_locks_one(dev, &dev->rx_queue, NULL);
}

unsigned long netdev_fix_features(unsigned long features, const char *name)
{
	/* Fix illegal SG+CSUM combinations. */
	if ((features & NETIF_F_SG) &&
	    !(features & NETIF_F_ALL_CSUM)) {
		if (name)
			printk(KERN_NOTICE "%s: Dropping NETIF_F_SG since no "
			       "checksum feature.\n", name);
		features &= ~NETIF_F_SG;
	}

	/* TSO requires that SG is present as well. */
	if ((features & NETIF_F_TSO) && !(features & NETIF_F_SG)) {
		if (name)
			printk(KERN_NOTICE "%s: Dropping NETIF_F_TSO since no "
			       "SG feature.\n", name);
		features &= ~NETIF_F_TSO;
	}

	if (features & NETIF_F_UFO) {
		if (!(features & NETIF_F_GEN_CSUM)) {
			if (name)
				printk(KERN_ERR "%s: Dropping NETIF_F_UFO "
				       "since no NETIF_F_HW_CSUM feature.\n",
				       name);
			features &= ~NETIF_F_UFO;
		}

		if (!(features & NETIF_F_SG)) {
			if (name)
				printk(KERN_ERR "%s: Dropping NETIF_F_UFO "
				       "since no NETIF_F_SG feature.\n", name);
			features &= ~NETIF_F_UFO;
		}
	}

	return features;
}
EXPORT_SYMBOL(netdev_fix_features);

/**
 *	netif_stacked_transfer_operstate -	transfer operstate
 *	@rootdev: the root or lower level device to transfer state from
 *	@dev: the device to transfer operstate to
 *
 *	Transfer operational state from root to device. This is normally
 *	called when a stacking relationship exists between the root
 *	device and the device(a leaf device).
 */
void netif_stacked_transfer_operstate(const struct net_device *rootdev,
					struct net_device *dev)
{
	if (rootdev->operstate == IF_OPER_DORMANT)
		netif_dormant_on(dev);
	else
		netif_dormant_off(dev);

	if (netif_carrier_ok(rootdev)) {
		if (!netif_carrier_ok(dev))
			netif_carrier_on(dev);
	} else {
		if (netif_carrier_ok(dev))
			netif_carrier_off(dev);
	}
}
EXPORT_SYMBOL(netif_stacked_transfer_operstate);

/**
 *	register_netdevice	- register a network device
 *	@dev: device to register
 *
 *	Take a completed network device structure and add it to the kernel
 *	interfaces. A %NETDEV_REGISTER message is sent to the netdev notifier
 *	chain. 0 is returned on success. A negative errno code is returned
 *	on a failure to set up the device, or if the name is a duplicate.
 *
 *	Callers must hold the rtnl semaphore. You may want
 *	register_netdev() instead of this.
 *
 *	BUGS:
 *	The locking appears insufficient to guarantee two parallel registers
 *	will not get the same name.
 */

int register_netdevice(struct net_device *dev)
{
	int ret;
	struct net *net = dev_net(dev);

	BUG_ON(dev_boot_phase);
	ASSERT_RTNL();

	might_sleep();

	/* When net_device's are persistent, this will be fatal. */
	BUG_ON(dev->reg_state != NETREG_UNINITIALIZED);
	BUG_ON(!net);

	spin_lock_init(&dev->addr_list_lock);
	netdev_set_addr_lockdep_class(dev);
	netdev_init_queue_locks(dev);

	dev->iflink = -1;

#ifdef CONFIG_RPS
	if (!dev->num_rx_queues) {
		/*
		 * Allocate a single RX queue if driver never called
		 * alloc_netdev_mq
		 */

		dev->_rx = kzalloc(sizeof(struct netdev_rx_queue), GFP_KERNEL);
		if (!dev->_rx) {
			ret = -ENOMEM;
			goto out;
		}

		dev->_rx->first = dev->_rx;
		atomic_set(&dev->_rx->count, 1);
		dev->num_rx_queues = 1;
	}
#endif
	/* Init, if this function is available */
	if (dev->netdev_ops->ndo_init) {
		ret = dev->netdev_ops->ndo_init(dev);
		if (ret) {
			if (ret > 0)
				ret = -EIO;
			goto out;
		}
	}

	ret = dev_get_valid_name(dev, dev->name, 0);
	if (ret)
		goto err_uninit;

	dev->ifindex = dev_new_index(net);
	if (dev->iflink == -1)
		dev->iflink = dev->ifindex;

	/* Fix illegal checksum combinations */
	if ((dev->features & NETIF_F_HW_CSUM) &&
	    (dev->features & (NETIF_F_IP_CSUM|NETIF_F_IPV6_CSUM))) {
		printk(KERN_NOTICE "%s: mixed HW and IP checksum settings.\n",
		       dev->name);
		dev->features &= ~(NETIF_F_IP_CSUM|NETIF_F_IPV6_CSUM);
	}

	if ((dev->features & NETIF_F_NO_CSUM) &&
	    (dev->features & (NETIF_F_HW_CSUM|NETIF_F_IP_CSUM|NETIF_F_IPV6_CSUM))) {
		printk(KERN_NOTICE "%s: mixed no checksumming and other settings.\n",
		       dev->name);
		dev->features &= ~(NETIF_F_IP_CSUM|NETIF_F_IPV6_CSUM|NETIF_F_HW_CSUM);
	}

	dev->features = netdev_fix_features(dev->features, dev->name);

	/* Enable software GSO if SG is supported. */
	if (dev->features & NETIF_F_SG)
		dev->features |= NETIF_F_GSO;

	ret = call_netdevice_notifiers(NETDEV_POST_INIT, dev);
	ret = notifier_to_errno(ret);
	if (ret)
		goto err_uninit;

	ret = netdev_register_kobject(dev);
	if (ret)
		goto err_uninit;
//	dev->reg_state = NETREG_REGISTERED;//xflu marked

	/*
	 *	Default initial state at registry is that the
	 *	device is present.
	 */

	set_bit(__LINK_STATE_PRESENT, &dev->state);

	dev_init_scheduler(dev);
	dev_hold(dev);
	list_netdevice(dev);

	
#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_PORTBIND//CONFIG_PORT_BINDING
	if (portbind_update_hook) {
		portbind_update_hook(dev, ADD_GROUP);
	}
#endif
#endif

	/* Notify protocols, that a new device appeared. */
	ret = call_netdevice_notifiers(NETDEV_REGISTER, dev);
	ret = notifier_to_errno(ret);
	if (ret) {
		rollback_registered(dev);
		dev->reg_state = NETREG_UNREGISTERED;
	}
	else
		dev->reg_state = NETREG_REGISTERED;//xflu move to here

	/*
	 *	Prevent userspace races by waiting until the network
	 *	device is fully setup before sending notifications.
	 */
	if (!dev->rtnl_link_ops ||
	    dev->rtnl_link_state == RTNL_LINK_INITIALIZED)
		rtmsg_ifinfo(RTM_NEWLINK, dev, ~0U);

out:
	return ret;

err_uninit:
	if (dev->netdev_ops->ndo_uninit)
		dev->netdev_ops->ndo_uninit(dev);
	goto out;
}
EXPORT_SYMBOL(register_netdevice);

/**
 *	init_dummy_netdev	- init a dummy network device for NAPI
 *	@dev: device to init
 *
 *	This takes a network device structure and initialize the minimum
 *	amount of fields so it can be used to schedule NAPI polls without
 *	registering a full blown interface. This is to be used by drivers
 *	that need to tie several hardware interfaces to a single NAPI
 *	poll scheduler due to HW limitations.
 */
int init_dummy_netdev(struct net_device *dev)
{
	/* Clear everything. Note we don't initialize spinlocks
	 * are they aren't supposed to be taken by any of the
	 * NAPI code and this dummy netdev is supposed to be
	 * only ever used for NAPI polls
	 */
	memset(dev, 0, sizeof(struct net_device));

	/* make sure we BUG if trying to hit standard
	 * register/unregister code path
	 */
	dev->reg_state = NETREG_DUMMY;

	/* initialize the ref count */
	atomic_set(&dev->refcnt, 1);

	/* NAPI wants this */
	INIT_LIST_HEAD(&dev->napi_list);

	/* a dummy interface is started by default */
	set_bit(__LINK_STATE_PRESENT, &dev->state);
	set_bit(__LINK_STATE_START, &dev->state);

	return 0;
}
EXPORT_SYMBOL_GPL(init_dummy_netdev);


/**
 *	register_netdev	- register a network device
 *	@dev: device to register
 *
 *	Take a completed network device structure and add it to the kernel
 *	interfaces. A %NETDEV_REGISTER message is sent to the netdev notifier
 *	chain. 0 is returned on success. A negative errno code is returned
 *	on a failure to set up the device, or if the name is a duplicate.
 *
 *	This is a wrapper around register_netdevice that takes the rtnl semaphore
 *	and expands the device name if you passed a format string to
 *	alloc_netdev.
 */
int register_netdev(struct net_device *dev)
{
	int err;

	rtnl_lock();

	/*
	 * If the name is a format string the caller wants us to do a
	 * name allocation.
	 */
	if (strchr(dev->name, '%')) {
		err = dev_alloc_name(dev, dev->name);
		if (err < 0)
			goto out;
	}

	err = register_netdevice(dev);
out:
	rtnl_unlock();
	return err;
}
EXPORT_SYMBOL(register_netdev);

/*
 * netdev_wait_allrefs - wait until all references are gone.
 *
 * This is called when unregistering network devices.
 *
 * Any protocol or device that holds a reference should register
 * for netdevice notification, and cleanup and put back the
 * reference if they receive an UNREGISTER event.
 * We can get stuck here if buggy protocols don't correctly
 * call dev_put.
 */
static void netdev_wait_allrefs(struct net_device *dev)
{
	unsigned long rebroadcast_time, warning_time;

	linkwatch_forget_dev(dev);

	rebroadcast_time = warning_time = jiffies;
	while (atomic_read(&dev->refcnt) != 0) {
		if (time_after(jiffies, rebroadcast_time + 1 * HZ)) {
			rtnl_lock();

			/* Rebroadcast unregister notification */
			call_netdevice_notifiers(NETDEV_UNREGISTER, dev);
			/* don't resend NETDEV_UNREGISTER_BATCH, _BATCH users
			 * should have already handle it the first time */

			if (test_bit(__LINK_STATE_LINKWATCH_PENDING,
				     &dev->state)) {
				/* We must not have linkwatch events
				 * pending on unregister. If this
				 * happens, we simply run the queue
				 * unscheduled, resulting in a noop
				 * for this device.
				 */
				linkwatch_run_queue();
			}

			__rtnl_unlock();

			rebroadcast_time = jiffies;
		}

		msleep(250);
#ifdef CONFIG_SMUX
		if (time_after(jiffies, warning_time + 2 * HZ)) {
#else
		if (time_after(jiffies, warning_time + 10 * HZ)) {
#endif
			printk(KERN_EMERG "unregister_netdevice: "
			       "waiting for %s to become free. Usage "
			       "count = %d\n",
			       dev->name, atomic_read(&dev->refcnt));
			/*When do smux interface unregister, it may be drop in endless loop. So add this clean refcnt action to avoid endless loop */
			if(atomic_read(&dev->refcnt) != 0 && (dev->name[0]=='n'
							|| dev->name[0]=='e'
							|| dev->name[0]=='p'
							|| dev->name[0]=='d' //for dslite
							|| dev->name[0]=='r'
							|| dev->name[0]=='o'))
			{
				//for network interface, e.g. nas0,nas0_1,eth0,eth0.1,ppp0,ra0,oam,omci
				atomic_set(&dev->refcnt, 0);
				printk("\nSet %s refcnt as 0 to avoid endless loop\n", dev->name);
			}
			warning_time = jiffies;
		}
	}
}

/* The sequence is:
 *
 *	rtnl_lock();
 *	...
 *	register_netdevice(x1);
 *	register_netdevice(x2);
 *	...
 *	unregister_netdevice(y1);
 *	unregister_netdevice(y2);
 *      ...
 *	rtnl_unlock();
 *	free_netdev(y1);
 *	free_netdev(y2);
 *
 * We are invoked by rtnl_unlock().
 * This allows us to deal with problems:
 * 1) We can delete sysfs objects which invoke hotplug
 *    without deadlocking with linkwatch via keventd.
 * 2) Since we run with the RTNL semaphore not held, we can sleep
 *    safely in order to wait for the netdev refcnt to drop to zero.
 *
 * We must not return until all unregister events added during
 * the interval the lock was held have been completed.
 */
void netdev_run_todo(void)
{
	struct list_head list;

	/* Snapshot list, allow later requests */
	list_replace_init(&net_todo_list, &list);

	__rtnl_unlock();

	while (!list_empty(&list)) {
		struct net_device *dev
			= list_first_entry(&list, struct net_device, todo_list);
		list_del(&dev->todo_list);

		if (unlikely(dev->reg_state != NETREG_UNREGISTERING)) {
			printk(KERN_ERR "network todo '%s' but state %d\n",
			       dev->name, dev->reg_state);
			dump_stack();
			continue;
		}

		dev->reg_state = NETREG_UNREGISTERED;

		on_each_cpu(flush_backlog, dev, 1);

		netdev_wait_allrefs(dev);

		/* paranoia */
		BUG_ON(atomic_read(&dev->refcnt));
		WARN_ON(dev->ip_ptr);
		WARN_ON(dev->ip6_ptr);
		WARN_ON(dev->dn_ptr);

		if (dev->destructor)
			dev->destructor(dev);

		/* Free network device */
		kobject_put(&dev->dev.kobj);
	}
}

/**
 *	dev_txq_stats_fold - fold tx_queues stats
 *	@dev: device to get statistics from
 *	@stats: struct rtnl_link_stats64 to hold results
 */
void dev_txq_stats_fold(const struct net_device *dev,
			struct rtnl_link_stats64 *stats)
{
	u64 tx_bytes = 0, tx_packets = 0, tx_dropped = 0;
	unsigned int i;
	struct netdev_queue *txq;

	for (i = 0; i < dev->num_tx_queues; i++) {
		txq = netdev_get_tx_queue(dev, i);
		spin_lock_bh(&txq->_xmit_lock);
		tx_bytes   += txq->tx_bytes;
		tx_packets += txq->tx_packets;
		tx_dropped += txq->tx_dropped;
		spin_unlock_bh(&txq->_xmit_lock);
	}
	if (tx_bytes || tx_packets || tx_dropped) {
		stats->tx_bytes   = tx_bytes;
		stats->tx_packets = tx_packets;
		stats->tx_dropped = tx_dropped;
	}
}
EXPORT_SYMBOL(dev_txq_stats_fold);

/* Convert net_device_stats to rtnl_link_stats64.  They have the same
 * fields in the same order, with only the type differing.
 */
static void netdev_stats_to_stats64(struct rtnl_link_stats64 *stats64,
				    const struct net_device_stats *netdev_stats)
{
#if BITS_PER_LONG == 64
        BUILD_BUG_ON(sizeof(*stats64) != sizeof(*netdev_stats));
        memcpy(stats64, netdev_stats, sizeof(*stats64));
#else
	size_t i, n = sizeof(*stats64) / sizeof(u64);
	const unsigned long *src = (const unsigned long *)netdev_stats;
	u64 *dst = (u64 *)stats64;

	BUILD_BUG_ON(sizeof(*netdev_stats) / sizeof(unsigned long) !=
		     sizeof(*stats64) / sizeof(u64));
	for (i = 0; i < n; i++)
		dst[i] = src[i];
#endif
}

/**
 *	dev_get_stats	- get network device statistics
 *	@dev: device to get statistics from
 *	@storage: place to store stats
 *
 *	Get network statistics from device. Return @storage.
 *	The device driver may provide its own method by setting
 *	dev->netdev_ops->get_stats64 or dev->netdev_ops->get_stats;
 *	otherwise the internal statistics structure is used.
 */
struct rtnl_link_stats64 *dev_get_stats(struct net_device *dev,
					struct rtnl_link_stats64 *storage)
{
	const struct net_device_ops *ops = dev->netdev_ops;

	if (ops->ndo_get_stats64) {
		memset(storage, 0, sizeof(*storage));
		return ops->ndo_get_stats64(dev, storage);
	}

	if (ops->ndo_get_stats) {
		netdev_stats_to_stats64(storage, ops->ndo_get_stats(dev));
		return storage;

	}
	netdev_stats_to_stats64(storage, &dev->stats);
	dev_txq_stats_fold(dev, storage);
	return storage;

}
EXPORT_SYMBOL(dev_get_stats);

static void netdev_init_one_queue(struct net_device *dev,
				  struct netdev_queue *queue,
				  void *_unused)
{
	queue->dev = dev;
}

static void netdev_init_queues(struct net_device *dev)
{
	netdev_init_one_queue(dev, &dev->rx_queue, NULL);
	netdev_for_each_tx_queue(dev, netdev_init_one_queue, NULL);
	spin_lock_init(&dev->tx_global_lock);
}

/**
 *	alloc_netdev_mq - allocate network device
 *	@sizeof_priv:	size of private data to allocate space for
 *	@name:		device name format string
 *	@setup:		callback to initialize device
 *	@queue_count:	the number of subqueues to allocate
 *
 *	Allocates a struct net_device with private data area for driver use
 *	and performs basic initialization.  Also allocates subquue structs
 *	for each queue on the device at the end of the netdevice.
 */
struct net_device *alloc_netdev_mq(int sizeof_priv, const char *name,
		void (*setup)(struct net_device *), unsigned int queue_count)
{
	struct netdev_queue *tx;
	struct net_device *dev;
	size_t alloc_size;
	struct net_device *p;
#ifdef CONFIG_RPS
	struct netdev_rx_queue *rx;
	int i;
#endif

	BUG_ON(strlen(name) >= sizeof(dev->name));

	alloc_size = sizeof(struct net_device);
	if (sizeof_priv) {
		/* ensure 32-byte alignment of private area */
		alloc_size = ALIGN(alloc_size, NETDEV_ALIGN);
		alloc_size += sizeof_priv;
	}
	/* ensure 32-byte alignment of whole construct */
	alloc_size += NETDEV_ALIGN - 1;

	p = kzalloc(alloc_size, GFP_KERNEL);
	if (!p) {
		printk(KERN_ERR "alloc_netdev: Unable to allocate device.\n");
		return NULL;
	}

	tx = kcalloc(queue_count, sizeof(struct netdev_queue), GFP_KERNEL);
	if (!tx) {
		printk(KERN_ERR "alloc_netdev: Unable to allocate "
		       "tx qdiscs.\n");
		goto free_p;
	}

#ifdef CONFIG_RPS
	rx = kcalloc(queue_count, sizeof(struct netdev_rx_queue), GFP_KERNEL);
	if (!rx) {
		printk(KERN_ERR "alloc_netdev: Unable to allocate "
		       "rx queues.\n");
		goto free_tx;
	}

	atomic_set(&rx->count, queue_count);

	/*
	 * Set a pointer to first element in the array which holds the
	 * reference count.
	 */
	for (i = 0; i < queue_count; i++)
		rx[i].first = rx;
#endif

	dev = PTR_ALIGN(p, NETDEV_ALIGN);
	dev->padded = (char *)dev - (char *)p;

	if (dev_addr_init(dev))
		goto free_rx;

	dev_mc_init(dev);
	dev_uc_init(dev);

	dev_net_set(dev, &init_net);

	dev->_tx = tx;
	dev->num_tx_queues = queue_count;
	dev->real_num_tx_queues = queue_count;

#ifdef CONFIG_RPS
	dev->_rx = rx;
	dev->num_rx_queues = queue_count;
#endif

	dev->gso_max_size = GSO_MAX_SIZE;

	netdev_init_queues(dev);

	INIT_LIST_HEAD(&dev->ethtool_ntuple_list.list);
	dev->ethtool_ntuple_list.count = 0;
	INIT_LIST_HEAD(&dev->napi_list);
	INIT_LIST_HEAD(&dev->unreg_list);
	INIT_LIST_HEAD(&dev->link_watch_list);
	dev->priv_flags = IFF_XMIT_DST_RELEASE;

#if defined(TCSUPPORT_CT)	
	if (sizeof_priv)
		dev->ml_priv = netdev_priv(dev);
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
	dev->ppp_flags = 0;
#endif
#endif

	setup(dev);
	strcpy(dev->name, name);
	return dev;

free_rx:
#ifdef CONFIG_RPS
	kfree(rx);
free_tx:
#endif
	kfree(tx);
free_p:
	kfree(p);
	return NULL;
}
EXPORT_SYMBOL(alloc_netdev_mq);

/**
 *	free_netdev - free network device
 *	@dev: device
 *
 *	This function does the last stage of destroying an allocated device
 * 	interface. The reference to the device object is released.
 *	If this is the last reference then it will be freed.
 */
void free_netdev(struct net_device *dev)
{
	struct napi_struct *p, *n;

	release_net(dev_net(dev));

	kfree(dev->_tx);

	/* Flush device addresses */
	dev_addr_flush(dev);

	/* Clear ethtool n-tuple list */
	ethtool_ntuple_flush(dev);

	list_for_each_entry_safe(p, n, &dev->napi_list, dev_list)
		netif_napi_del(p);

	/*  Compatibility with error handling in drivers */
	if (dev->reg_state == NETREG_UNINITIALIZED) {
		kfree((char *)dev - dev->padded);
		return;
	}

	BUG_ON(dev->reg_state != NETREG_UNREGISTERED);
	dev->reg_state = NETREG_RELEASED;

	/* will free via device release */
	put_device(&dev->dev);
}
EXPORT_SYMBOL(free_netdev);

/**
 *	synchronize_net -  Synchronize with packet receive processing
 *
 *	Wait for packets currently being received to be done.
 *	Does not block later packets from starting.
 */
void synchronize_net(void)
{
	might_sleep();
	synchronize_rcu();
}
EXPORT_SYMBOL(synchronize_net);

/**
 *	unregister_netdevice_queue - remove device from the kernel
 *	@dev: device
 *	@head: list
 *
 *	This function shuts down a device interface and removes it
 *	from the kernel tables.
 *	If head not NULL, device is queued to be unregistered later.
 *
 *	Callers must hold the rtnl semaphore.  You may want
 *	unregister_netdev() instead of this.
 */

void unregister_netdevice_queue(struct net_device *dev, struct list_head *head)
{
	ASSERT_RTNL();

#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_PORTBIND//CONFIG_PORT_BINDING
			if (portbind_update_hook) {
				portbind_update_hook(dev, DEL_GROUP);
			}
#endif
#endif

	if (head) {
		list_move_tail(&dev->unreg_list, head);
	} else {
		rollback_registered(dev);
		/* Finish processing unregister after unlock */
		net_set_todo(dev);
	}
}
EXPORT_SYMBOL(unregister_netdevice_queue);

/**
 *	unregister_netdevice_many - unregister many devices
 *	@head: list of devices
 */
void unregister_netdevice_many(struct list_head *head)
{
	struct net_device *dev;

	if (!list_empty(head)) {
		rollback_registered_many(head);
		list_for_each_entry(dev, head, unreg_list)
			net_set_todo(dev);
	}
}
EXPORT_SYMBOL(unregister_netdevice_many);

/**
 *	unregister_netdev - remove device from the kernel
 *	@dev: device
 *
 *	This function shuts down a device interface and removes it
 *	from the kernel tables.
 *
 *	This is just a wrapper for unregister_netdevice that takes
 *	the rtnl semaphore.  In general you want to use this and not
 *	unregister_netdevice.
 */
void unregister_netdev(struct net_device *dev)
{
	rtnl_lock();
	unregister_netdevice(dev);
	rtnl_unlock();
}
EXPORT_SYMBOL(unregister_netdev);

/**
 *	dev_change_net_namespace - move device to different nethost namespace
 *	@dev: device
 *	@net: network namespace
 *	@pat: If not NULL name pattern to try if the current device name
 *	      is already taken in the destination network namespace.
 *
 *	This function shuts down a device interface and moves it
 *	to a new network namespace. On success 0 is returned, on
 *	a failure a netagive errno code is returned.
 *
 *	Callers must hold the rtnl semaphore.
 */

int dev_change_net_namespace(struct net_device *dev, struct net *net, const char *pat)
{
	int err;

	ASSERT_RTNL();

	/* Don't allow namespace local devices to be moved. */
	err = -EINVAL;
	if (dev->features & NETIF_F_NETNS_LOCAL)
		goto out;

	/* Ensure the device has been registrered */
	err = -EINVAL;
	if (dev->reg_state != NETREG_REGISTERED)
		goto out;

	/* Get out if there is nothing todo */
	err = 0;
	if (net_eq(dev_net(dev), net))
		goto out;

	/* Pick the destination device name, and ensure
	 * we can use it in the destination network namespace.
	 */
	err = -EEXIST;
	if (__dev_get_by_name(net, dev->name)) {
		/* We get here if we can't use the current device name */
		if (!pat)
			goto out;
		if (dev_get_valid_name(dev, pat, 1))
			goto out;
	}

	/*
	 * And now a mini version of register_netdevice unregister_netdevice.
	 */

	/* If device is running close it first. */
	dev_close(dev);

	/* And unlink it from device chain */
	err = -ENODEV;
	unlist_netdevice(dev);

	synchronize_net();

	/* Shutdown queueing discipline. */
	dev_shutdown(dev);

	/* Notify protocols, that we are about to destroy
	   this device. They should clean all the things.
	*/
	call_netdevice_notifiers(NETDEV_UNREGISTER, dev);
	call_netdevice_notifiers(NETDEV_UNREGISTER_BATCH, dev);

	/*
	 *	Flush the unicast and multicast chains
	 */
	dev_uc_flush(dev);
	dev_mc_flush(dev);

	/* Actually switch the network namespace */
	dev_net_set(dev, net);

	/* If there is an ifindex conflict assign a new one */
	if (__dev_get_by_index(net, dev->ifindex)) {
		int iflink = (dev->iflink == dev->ifindex);
		dev->ifindex = dev_new_index(net);
		if (iflink)
			dev->iflink = dev->ifindex;
	}

	/* Fixup kobjects */
	err = device_rename(&dev->dev, dev->name);
	WARN_ON(err);

	/* Add the device back in the hashes */
	list_netdevice(dev);

	/* Notify protocols, that a new device appeared. */
	call_netdevice_notifiers(NETDEV_REGISTER, dev);

	/*
	 *	Prevent userspace races by waiting until the network
	 *	device is fully setup before sending notifications.
	 */
	rtmsg_ifinfo(RTM_NEWLINK, dev, ~0U);

	synchronize_net();
	err = 0;
out:
	return err;
}
EXPORT_SYMBOL_GPL(dev_change_net_namespace);

static int dev_cpu_callback(struct notifier_block *nfb,
			    unsigned long action,
			    void *ocpu)
{
	struct sk_buff **list_skb;
	struct sk_buff *skb;
	unsigned int cpu, oldcpu = (unsigned long)ocpu;
	struct softnet_data *sd, *oldsd;

	if (action != CPU_DEAD && action != CPU_DEAD_FROZEN)
		return NOTIFY_OK;

	local_irq_disable();
	cpu = smp_processor_id();
	sd = &per_cpu(softnet_data, cpu);
	oldsd = &per_cpu(softnet_data, oldcpu);

	/* Find end of our completion_queue. */
	list_skb = &sd->completion_queue;
	while (*list_skb)
		list_skb = &(*list_skb)->next;
	/* Append completion queue from offline CPU. */
	*list_skb = oldsd->completion_queue;
	oldsd->completion_queue = NULL;

	/* Append output queue from offline CPU. */
	if (oldsd->output_queue) {
		*sd->output_queue_tailp = oldsd->output_queue;
		sd->output_queue_tailp = oldsd->output_queue_tailp;
		oldsd->output_queue = NULL;
		oldsd->output_queue_tailp = &oldsd->output_queue;
	}

	raise_softirq_irqoff(NET_TX_SOFTIRQ);
	local_irq_enable();

	/* Process offline CPU's input_pkt_queue */
	while ((skb = __skb_dequeue(&oldsd->process_queue))) {
		netif_rx(skb);
		input_queue_head_incr(oldsd);
	}
	while ((skb = __skb_dequeue(&oldsd->input_pkt_queue))) {
		netif_rx(skb);
		input_queue_head_incr(oldsd);
	}

	return NOTIFY_OK;
}


/**
 *	netdev_increment_features - increment feature set by one
 *	@all: current feature set
 *	@one: new feature set
 *	@mask: mask feature set
 *
 *	Computes a new feature set after adding a device with feature set
 *	@one to the master device with current feature set @all.  Will not
 *	enable anything that is off in @mask. Returns the new feature set.
 */
unsigned long netdev_increment_features(unsigned long all, unsigned long one,
					unsigned long mask)
{
	/* If device needs checksumming, downgrade to it. */
	if (all & NETIF_F_NO_CSUM && !(one & NETIF_F_NO_CSUM))
		all ^= NETIF_F_NO_CSUM | (one & NETIF_F_ALL_CSUM);
	else if (mask & NETIF_F_ALL_CSUM) {
		/* If one device supports v4/v6 checksumming, set for all. */
		if (one & (NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM) &&
		    !(all & NETIF_F_GEN_CSUM)) {
			all &= ~NETIF_F_ALL_CSUM;
			all |= one & (NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM);
		}

		/* If one device supports hw checksumming, set for all. */
		if (one & NETIF_F_GEN_CSUM && !(all & NETIF_F_GEN_CSUM)) {
			all &= ~NETIF_F_ALL_CSUM;
			all |= NETIF_F_HW_CSUM;
		}
	}

	one |= NETIF_F_ALL_CSUM;

	one |= all & NETIF_F_ONE_FOR_ALL;
	all &= one | NETIF_F_LLTX | NETIF_F_GSO | NETIF_F_UFO;
	all |= one & mask & NETIF_F_ONE_FOR_ALL;

	return all;
}
EXPORT_SYMBOL(netdev_increment_features);

static struct hlist_head *netdev_create_hash(void)
{
	int i;
	struct hlist_head *hash;

	hash = kmalloc(sizeof(*hash) * NETDEV_HASHENTRIES, GFP_KERNEL);
	if (hash != NULL)
		for (i = 0; i < NETDEV_HASHENTRIES; i++)
			INIT_HLIST_HEAD(&hash[i]);

	return hash;
}
#if defined(TCSUPPORT_CT_VOIP_QOS)
int gRtpPriority = 0;
int gRtpBindWanidx = -1;


int getRtpBindWANidx(){
	return gRtpBindWanidx;
}

int setRtpBindWANidx(int value){
	gRtpBindWanidx = value;	
	printk("gRtpBindWanidx= %d\n",gRtpBindWanidx);
	return 0;
}

int getRtpPriority(){
	return gRtpPriority;
}

int setRtpPriority(int value){
	gRtpPriority = value;	
	return 0;
}

EXPORT_SYMBOL(getRtpBindWANidx);
EXPORT_SYMBOL(getRtpPriority);

static int vdspProcReadRtpBindWANidx(char *page, char **start, off_t off, 
	int count, int *eof, void *data)
{
	int len=0;
	len += sprintf(page+len, "%d\r\n", getRtpBindWANidx());
	
	return len;
}

static int vdspProcWriteRtpBindWANidx(struct file *file, const char *buffer, 
	unsigned long count, void *data)
{	
	int ret;
	char wanIdx[32];
	int index = -1;

	if (count > sizeof(wanIdx) - 1)
		return -EINVAL;

	if (copy_from_user(wanIdx, buffer, count))
		return -EFAULT;

	wanIdx[count] =  '\0';	
	sscanf(wanIdx, "%d",&index);
	setRtpBindWANidx(index);
	return count;
}

static int vdspProcReadRtpPriority(char *page, char **start, off_t off, 
	int count, int *eof, void *data)
{
	int len=0;
	len += sprintf(page+len, "%d\r\n", getRtpPriority());
	
	return len;
}


static int vdspProcWriteRtpPriority(struct file *file, const char *buffer, 
	unsigned long count, void *data)
{	
	int ret;
	char priority[32];
	int iPriority = 0;

	if (count > sizeof(priority) - 1)
		return -EINVAL;

	if (copy_from_user(priority, buffer, count))
		return -EFAULT;

	priority[count] = '\0';
	sscanf(priority, "%d",&iPriority);
	setRtpPriority(iPriority);
	return count;
}
#endif


/* Initialize per network namespace state */
static int __net_init netdev_init(struct net *net)
{
#if defined(TCSUPPORT_CT_VLAN_BIND)
	struct proc_dir_entry *vbind_proc = NULL;
#endif
#if defined(TCSUPPORT_CMCC)
	struct proc_dir_entry *ethertypefilter_proc = NULL;
#endif
#if defined(TCSUPPORT_CT_VOIP_QOS)	
	struct proc_dir_entry *voip_proc = NULL;
#endif



	INIT_LIST_HEAD(&net->dev_base_head);

	net->dev_name_head = netdev_create_hash();
	if (net->dev_name_head == NULL)
		goto err_name;

	net->dev_index_head = netdev_create_hash();
	if (net->dev_index_head == NULL)
		goto err_idx;
#if defined(TCSUPPORT_CT_VLAN_BIND)
	vbind_proc = create_proc_entry(VBIND_ENTRY_ARRAY_PATH, 0, NULL);
	vbind_proc->read_proc = vbind_entry_array_read_proc;
	vbind_proc->write_proc = vbind_entry_array_write_proc;	
#endif

#if defined(TCSUPPORT_CMCC)
	ethertypefilter_proc = create_proc_entry(ETHERTYPEFILTER_ARRAY_PATH, 0, NULL);
	ethertypefilter_proc->read_proc = ethertypefilter_array_read_proc;
	ethertypefilter_proc->write_proc = ethertypefilter_array_write_proc;	
#endif

#if defined(TCSUPPORT_CT_VOIP_QOS)
	voip_proc = create_proc_entry(RTP_BIND_WAN_INDEX, 0, NULL);
	voip_proc->read_proc = vdspProcReadRtpBindWANidx;
	voip_proc->write_proc = vdspProcWriteRtpBindWANidx;

	voip_proc = create_proc_entry(RTP_PRIORITY, 0, NULL);
	voip_proc->read_proc = vdspProcReadRtpPriority;
	voip_proc->write_proc = vdspProcWriteRtpPriority;
#endif
	return 0;

err_idx:
	kfree(net->dev_name_head);
err_name:
	return -ENOMEM;
}

/**
 *	netdev_drivername - network driver for the device
 *	@dev: network device
 *	@buffer: buffer for resulting name
 *	@len: size of buffer
 *
 *	Determine network driver for device.
 */
char *netdev_drivername(const struct net_device *dev, char *buffer, int len)
{
	const struct device_driver *driver;
	const struct device *parent;

	if (len <= 0 || !buffer)
		return buffer;
	buffer[0] = 0;

	parent = dev->dev.parent;

	if (!parent)
		return buffer;

	driver = parent->driver;
	if (driver && driver->name)
		strlcpy(buffer, driver->name, len);
	return buffer;
}

static int __netdev_printk(const char *level, const struct net_device *dev,
			   struct va_format *vaf)
{
	int r;

	if (dev && dev->dev.parent)
		r = dev_printk(level, dev->dev.parent, "%s: %pV",
			       netdev_name(dev), vaf);
	else if (dev)
		r = printk("%s%s: %pV", level, netdev_name(dev), vaf);
	else
		r = printk("%s(NULL net_device): %pV", level, vaf);

	return r;
}

int netdev_printk(const char *level, const struct net_device *dev,
		  const char *format, ...)
{
	struct va_format vaf;
	va_list args;
	int r;

	va_start(args, format);

	vaf.fmt = format;
	vaf.va = &args;

	r = __netdev_printk(level, dev, &vaf);
	va_end(args);

	return r;
}
EXPORT_SYMBOL(netdev_printk);

#define define_netdev_printk_level(func, level)			\
int func(const struct net_device *dev, const char *fmt, ...)	\
{								\
	int r;							\
	struct va_format vaf;					\
	va_list args;						\
								\
	va_start(args, fmt);					\
								\
	vaf.fmt = fmt;						\
	vaf.va = &args;						\
								\
	r = __netdev_printk(level, dev, &vaf);			\
	va_end(args);						\
								\
	return r;						\
}								\
EXPORT_SYMBOL(func);

define_netdev_printk_level(netdev_emerg, KERN_EMERG);
define_netdev_printk_level(netdev_alert, KERN_ALERT);
define_netdev_printk_level(netdev_crit, KERN_CRIT);
define_netdev_printk_level(netdev_err, KERN_ERR);
define_netdev_printk_level(netdev_warn, KERN_WARNING);
define_netdev_printk_level(netdev_notice, KERN_NOTICE);
define_netdev_printk_level(netdev_info, KERN_INFO);

static void __net_exit netdev_exit(struct net *net)
{
	kfree(net->dev_name_head);
	kfree(net->dev_index_head);
#if defined(TCSUPPORT_CT_VLAN_BIND)
	remove_proc_entry(VBIND_ENTRY_ARRAY_PATH, NULL);
#endif
#if defined(TCSUPPORT_CMCC)
	remove_proc_entry(ETHERTYPEFILTER_ARRAY_PATH, NULL);
#endif
#if defined(TCSUPPORT_CT_VOIP_QOS)
	remove_proc_entry(RTP_BIND_WAN_INDEX, NULL);	
	remove_proc_entry(RTP_PRIORITY, NULL);	
#endif

}

static struct pernet_operations __net_initdata netdev_net_ops = {
	.init = netdev_init,
	.exit = netdev_exit,
};

static void __net_exit default_device_exit(struct net *net)
{
	struct net_device *dev, *aux;
	/*
	 * Push all migratable network devices back to the
	 * initial network namespace
	 */
	rtnl_lock();
	for_each_netdev_safe(net, dev, aux) {
		int err;
		char fb_name[IFNAMSIZ];

		/* Ignore unmoveable devices (i.e. loopback) */
		if (dev->features & NETIF_F_NETNS_LOCAL)
			continue;

		/* Leave virtual devices for the generic cleanup */
		if (dev->rtnl_link_ops)
			continue;

		/* Push remaing network devices to init_net */
		snprintf(fb_name, IFNAMSIZ, "dev%d", dev->ifindex);
		err = dev_change_net_namespace(dev, &init_net, fb_name);
		if (err) {
			printk(KERN_EMERG "%s: failed to move %s to init_net: %d\n",
				__func__, dev->name, err);
			BUG();
		}
	}
	rtnl_unlock();
}

static void __net_exit default_device_exit_batch(struct list_head *net_list)
{
	/* At exit all network devices most be removed from a network
	 * namespace.  Do this in the reverse order of registeration.
	 * Do this across as many network namespaces as possible to
	 * improve batching efficiency.
	 */
	struct net_device *dev;
	struct net *net;
	LIST_HEAD(dev_kill_list);

	rtnl_lock();
	list_for_each_entry(net, net_list, exit_list) {
		for_each_netdev_reverse(net, dev) {
			if (dev->rtnl_link_ops)
				dev->rtnl_link_ops->dellink(dev, &dev_kill_list);
			else
				unregister_netdevice_queue(dev, &dev_kill_list);
		}
	}
	unregister_netdevice_many(&dev_kill_list);
	rtnl_unlock();
}

static struct pernet_operations __net_initdata default_device_ops = {
	.exit = default_device_exit,
	.exit_batch = default_device_exit_batch,
};

/*
 *	Initialize the DEV module. At boot time this walks the device list and
 *	unhooks any devices that fail to initialise (normally hardware not
 *	present) and leaves us with a valid list of present and active devices.
 *
 */

/*
 *       This is called single threaded during boot, so no need
 *       to take the rtnl semaphore.
 */
static int __init net_dev_init(void)
{
	int i, rc = -ENOMEM;
#ifdef TCSUPPORT_DOWNSTREAM_QOS
	int j = 0;
#endif

	BUG_ON(!dev_boot_phase);

	if (dev_proc_init())
		goto out;

	if (netdev_kobject_init())
		goto out;

	INIT_LIST_HEAD(&ptype_all);
	for (i = 0; i < PTYPE_HASH_SIZE; i++)
		INIT_LIST_HEAD(&ptype_base[i]);

	if (register_pernet_subsys(&netdev_net_ops))
		goto out;

	/*
	 *	Initialise the packet receive queues.
	 */

	for_each_possible_cpu(i) {
		struct softnet_data *sd = &per_cpu(softnet_data, i);

		memset(sd, 0, sizeof(*sd));
		skb_queue_head_init(&sd->input_pkt_queue);
		skb_queue_head_init(&sd->process_queue);
		sd->completion_queue = NULL;
		INIT_LIST_HEAD(&sd->poll_list);
		sd->output_queue = NULL;
		sd->output_queue_tailp = &sd->output_queue;
#ifdef CONFIG_RPS
		sd->csd.func = rps_trigger_softirq;
		sd->csd.info = sd;
		sd->csd.flags = 0;
		sd->cpu = i;
#endif
#ifdef TCSUPPORT_DOWNSTREAM_QOS        
		for( j = 0 ; j < PRIORITY_QUEUE_NUM; j++){
			skb_queue_head_init(&sd->pri_queue[j]);	
		}	
#endif

		sd->backlog.poll = process_backlog;
		sd->backlog.weight = weight_p;
		sd->backlog.gro_list = NULL;
		sd->backlog.gro_count = 0;
	}

	dev_boot_phase = 0;

	/* The loopback device is special if any other network devices
	 * is present in a network namespace the loopback device must
	 * be present. Since we now dynamically allocate and free the
	 * loopback device ensure this invariant is maintained by
	 * keeping the loopback device as the first device on the
	 * list of network devices.  Ensuring the loopback devices
	 * is the first device that appears and the last network device
	 * that disappears.
	 */
	if (register_pernet_device(&loopback_net_ops))
		goto out;

	if (register_pernet_device(&default_device_ops))
		goto out;

	open_softirq(NET_TX_SOFTIRQ, net_tx_action);
	open_softirq(NET_RX_SOFTIRQ, net_rx_action);

	hotcpu_notifier(dev_cpu_callback, 0);
	dst_init();
	dev_mcast_init();
	rc = 0;
out:
	return rc;
}

subsys_initcall(net_dev_init);

static int __init initialize_hashrnd(void)
{
	get_random_bytes(&hashrnd, sizeof(hashrnd));
	return 0;
}

late_initcall_sync(initialize_hashrnd);
#ifdef CONFIG_QOS
#ifdef TCSUPPORT_SBTHROUGHPUT_ENHANCE
EXPORT_SYMBOL(tc_qos_switch);
#endif
#endif


