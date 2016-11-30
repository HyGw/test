/*
 *	Forwarding database
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

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/rculist.h>
#include <linux/spinlock.h>
#include <linux/times.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/jhash.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <ecnt_hook/ecnt_hook_net.h>
#include <asm/atomic.h>
#include <asm/unaligned.h>
#include "br_private.h"
#if defined(TCSUPPORT_HWNAT)
#include <linux/pktflow.h>
#endif

#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_PORTBIND//CONFIG_PORT_BINDING
extern int (*portbind_sw_hook)(void);
extern int (*portbind_check_hook)(struct net_device *in_dev, struct net_device *out_dev);
#endif
#else
#ifdef CONFIG_PORT_BINDING
#if defined(TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND)
extern int (*portbind_sw_prior_hook)(struct sk_buff *skb);
extern struct net_device* (*portbind_get_outdev_by_indev_hook)(unsigned char* indev_name);
#else
extern int (*portbind_sw_hook)(void);
#endif
extern int (*portbind_check_hook)(char *inIf, char *outIf);
#endif
#endif

#if defined(TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND)
extern struct net_device* (*tcvlan_get_outdev_hook)(struct sk_buff* skb);
#endif

__DMEM static struct kmem_cache *br_fdb_cache __read_mostly;
static int fdb_insert(struct net_bridge *br, struct net_bridge_port *source,
		      const unsigned char *addr);

static u32 fdb_salt __read_mostly;
#if defined(TCSUPPORT_HWNAT)
extern void pktflow_fdb(struct sk_buff *skb, struct net_bridge_fdb_entry *fdb);
#endif

#ifdef TCSUPPORT_BRIDGE_MAC_LIMIT
static int mac_limit_total_write_proc(struct file *file, const char *buffer, unsigned long count, void *data);
static int mac_limit_total_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data);
static int mac_limit_by_port_write_proc(struct file *file, const char *buffer, unsigned long count, void *data);
static int mac_limit_by_port_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data);
#endif

int (*portbind_check_bind_wantype)(char *landev, int bind_index);
EXPORT_SYMBOL(portbind_check_bind_wantype);
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_STBMAC_REPORT)
static int stb_list_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data);
#endif

#if defined(TCSUPPORT_CT_PON)
static int host_list_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data);
#endif

int __init br_fdb_init(void)
{
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_STBMAC_REPORT)
	struct proc_dir_entry *br_stb_proc = NULL;
#endif
#if defined(TCSUPPORT_CT_PON)
        struct proc_dir_entry *br_host_stb_proc = NULL;
#endif
#ifdef TCSUPPORT_BRIDGE_MAC_LIMIT
	struct proc_dir_entry *br_fdb_proc = NULL;
#endif

	br_fdb_cache = kmem_cache_create("bridge_fdb_cache",
					 sizeof(struct net_bridge_fdb_entry),
					 0,
					 SLAB_HWCACHE_ALIGN, NULL);
	if (!br_fdb_cache)
		return -ENOMEM;

	get_random_bytes(&fdb_salt, sizeof(fdb_salt));

	#ifdef TCSUPPORT_BRIDGE_MAC_LIMIT
	proc_mkdir("br_fdb", NULL);
	br_fdb_proc = create_proc_entry("br_fdb/mac_limit_total", 0, NULL);
	if(NULL == br_fdb_proc)
	{
		printk("ERROR!Create proc entry mac_limit_total fail!");
		return -ENOMEM;
	}
	br_fdb_proc->read_proc = mac_limit_total_read_proc;
	br_fdb_proc->write_proc = mac_limit_total_write_proc;

	br_fdb_proc = create_proc_entry("br_fdb/mac_limit_by_port",0,NULL);
	if(NULL == br_fdb_proc)
	{
		printk("ERROR!Create proc entry mac_limit_by_port fail!");
		return -ENOMEM;
	}
	br_fdb_proc->read_proc = mac_limit_by_port_read_proc;
	br_fdb_proc->write_proc = mac_limit_by_port_write_proc;
	#endif
	
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_STBMAC_REPORT)
	proc_mkdir("br_fdb_stb", NULL);
	br_stb_proc = create_proc_entry("br_fdb_stb/stb_list", 0, NULL);
	if( NULL == br_stb_proc )
	{
		printk("ERROR!Create proc entry stb_list fail!\n");
		return -ENOMEM;
	}
	br_stb_proc->read_proc = stb_list_read_proc;
#endif
	
#if defined(TCSUPPORT_CT_PON)	
        proc_mkdir("br_fdb_host", NULL);
	br_host_stb_proc = create_proc_entry("br_fdb_host/stb_list", 0, NULL);
	if( NULL == br_host_stb_proc )
	{
		printk("ERROR!Create proc entry stb_list fail!\n");
		return -ENOMEM;
	}
	br_host_stb_proc->read_proc = host_list_read_proc;
#endif
	return 0;
}

void br_fdb_fini(void)
{
	#ifdef TCSUPPORT_BRIDGE_MAC_LIMIT
	remove_proc_entry("br_fdb/mac_limit_total",NULL);
	remove_proc_entry("br_fdb/mac_limit_by_port",NULL);
	#endif
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_STBMAC_REPORT)
	remove_proc_entry("br_fdb_stb/stb_list", NULL);
#endif

#if defined(TCSUPPORT_CT_PON)	
        remove_proc_entry("br_fdb_host/stb_list", NULL);
#endif

	kmem_cache_destroy(br_fdb_cache);
}


/* if topology_changing then use forward_delay (default 15 sec)
 * otherwise keep longer (default 5 minutes)
 */
static inline unsigned long hold_time(const struct net_bridge *br)
{
	return br->topology_change ? br->forward_delay : br->ageing_time;
}

static inline int has_expired(const struct net_bridge *br,
				  const struct net_bridge_fdb_entry *fdb)
{
	return !fdb->is_static &&
		time_before_eq(fdb->ageing_timer + hold_time(br), jiffies);
}

static inline int br_mac_hash(const unsigned char *mac)
{
	/* use 1 byte of OUI cnd 3 bytes of NIC */
	u32 key = get_unaligned((u32 *)(mac + 2));
	return jhash_1word(key, fdb_salt) & (BR_HASH_SIZE - 1);
}

static void fdb_rcu_free(struct rcu_head *head)
{
	struct net_bridge_fdb_entry *ent
		= container_of(head, struct net_bridge_fdb_entry, rcu);
	kmem_cache_free(br_fdb_cache, ent);
}

void fdb_delete(struct net_bridge_fdb_entry *f)
{
#if defined(TCSUPPORT_HWNAT)
  	if (pktflow_fdb_delete_hook) 
		pktflow_fdb_delete_hook(f);
#endif
	ECNT_BR_FDB_HOOK(ECNT_BR_FDB_DELETE, NULL, f, NULL);
	
	hlist_del_rcu(&f->hlist);
	call_rcu(&f->rcu, fdb_rcu_free);
}

void br_fdb_changeaddr(struct net_bridge_port *p, const unsigned char *newaddr)
{
	struct net_bridge *br = p->br;
	int i;

	spin_lock_bh(&br->hash_lock);

	/* Search all chains since old address/hash is unknown */
	for (i = 0; i < BR_HASH_SIZE; i++) {
		struct hlist_node *h;
		hlist_for_each(h, &br->hash[i]) {
			struct net_bridge_fdb_entry *f;

			f = hlist_entry(h, struct net_bridge_fdb_entry, hlist);
			if (f->dst == p && f->is_local) {
				/* maybe another port has same hw addr? */
				struct net_bridge_port *op;
				list_for_each_entry(op, &br->port_list, list) {
					if (op != p &&
					    !compare_ether_addr(op->dev->dev_addr,
								f->addr.addr)) {
						f->dst = op;
						goto insert;
					}
				}

				/* delete old one */
				fdb_delete(f);
				goto insert;
			}
		}
	}
 insert:
	/* insert new address,  may fail if invalid address or dup. */
	fdb_insert(br, p, newaddr);

	spin_unlock_bh(&br->hash_lock);
}

void br_fdb_cleanup(unsigned long _data)
{
	struct net_bridge *br = (struct net_bridge *)_data;
	unsigned long delay = hold_time(br);
	unsigned long next_timer = jiffies + br->ageing_time;
	int i;

	spin_lock_bh(&br->hash_lock);
	for (i = 0; i < BR_HASH_SIZE; i++) {
		struct net_bridge_fdb_entry *f;
		struct hlist_node *h, *n;

		hlist_for_each_entry_safe(f, h, n, &br->hash[i], hlist) {
			unsigned long this_timer;
			if (f->is_static)
				continue;
			this_timer = f->ageing_timer + delay;
			if (time_before_eq(this_timer, jiffies))
				fdb_delete(f);
			else if (time_before(this_timer, next_timer))
				next_timer = this_timer;
		}
	}
	spin_unlock_bh(&br->hash_lock);

	mod_timer(&br->gc_timer, round_jiffies_up(next_timer));
}

/* Completely flush all dynamic entries in forwarding database.*/
void br_fdb_flush(struct net_bridge *br)
{
	int i;

	spin_lock_bh(&br->hash_lock);
	for (i = 0; i < BR_HASH_SIZE; i++) {
		struct net_bridge_fdb_entry *f;
		struct hlist_node *h, *n;
		hlist_for_each_entry_safe(f, h, n, &br->hash[i], hlist) {
			if (!f->is_static)
				fdb_delete(f);
		}
	}
	spin_unlock_bh(&br->hash_lock);
}

/* Flush all entries refering to a specific port.
 * if do_all is set also flush static entries
 */
void br_fdb_delete_by_port(struct net_bridge *br,
			   const struct net_bridge_port *p,
			   int do_all)
{
	int i;

	spin_lock_bh(&br->hash_lock);
	for (i = 0; i < BR_HASH_SIZE; i++) {
		struct hlist_node *h, *g;

		hlist_for_each_safe(h, g, &br->hash[i]) {
			struct net_bridge_fdb_entry *f
				= hlist_entry(h, struct net_bridge_fdb_entry, hlist);
			if (f->dst != p)
				continue;

			if (f->is_static && !do_all)
				continue;
			/*
			 * if multiple ports all have the same device address
			 * then when one port is deleted, assign
			 * the local entry to other port
			 */
			if (f->is_local) {
				struct net_bridge_port *op;
				list_for_each_entry(op, &br->port_list, list) {
					if (op != p &&
					    !compare_ether_addr(op->dev->dev_addr,
								f->addr.addr)) {
						f->dst = op;
						goto skip_delete;
					}
				}
			}

			fdb_delete(f);
		skip_delete: ;
		}
	}
	spin_unlock_bh(&br->hash_lock);
}

static inline struct net_bridge_fdb_entry *fdb_find(struct hlist_head *head,
						    const unsigned char *addr)
{
	struct hlist_node *h;
	struct net_bridge_fdb_entry *fdb;

	hlist_for_each_entry_rcu(fdb, h, head, hlist) {
		if (!compare_ether_addr(fdb->addr.addr, addr))
			return fdb;
	}
	return NULL;
}

#if defined(TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND)
struct net_device* br_get_outdev_by_routePolicy(struct sk_buff* skb)
{
	int pvcIndex = -1;
	unsigned char outdev_name[16];
	struct net_device* outdev = NULL;

	if(!(skb->mark&0x1E00000))
		return NULL;

	pvcIndex = ((skb->mark&0x1E00000)>>21)-1;

	memset(outdev_name,0,16);
	sprintf(outdev_name,"nas%d",pvcIndex);

	outdev = dev_get_by_name(&init_net, outdev_name);

	return outdev;
}

struct net_bridge_fdb_entry *fdb_vlan_globle = NULL;
struct net_bridge_fdb_entry *vlan_fdb_get(struct sk_buff *skb, struct net_bridge *br,
					  const unsigned char *addr)
{
	struct net_bridge_port		*dst = NULL;
	struct net_device *outdev = NULL;
	struct hlist_head *head = NULL;
	struct net_bridge_fdb_entry *fdb = NULL;

	if(!compare_ether_addr(addr,br->dev->dev_addr))
		return NULL;

	if(skb->dev->name[0] == 'n')//incoming dev is wan interface
		return NULL;

	spin_lock_bh(&br->hash_lock);
	head = &br->hash[br_mac_hash(addr)];
	fdb = fdb_find(head, addr);

	if(fdb == NULL)
	{
	
		spin_unlock_bh(&br->hash_lock);
		return NULL;
	}
	spin_unlock_bh(&br->hash_lock);

#if defined(TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND)
	if (portbind_sw_prior_hook && (portbind_sw_prior_hook(skb) == 1)) 
		if(portbind_get_outdev_by_indev_hook)
			outdev = portbind_get_outdev_by_indev_hook(skb->dev->name);
#endif	

#if defined(TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND)
	if(outdev == NULL)
		outdev = br_get_outdev_by_routePolicy(skb);
#endif

	if(outdev == NULL)
	{
#if defined(TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND)
		if(tcvlan_get_outdev_hook)
			outdev = tcvlan_get_outdev_hook(skb);
#endif
	}

	if(outdev !=NULL)
		dst = br_port_get_rcu(outdev);
	if(dst == NULL)
		return NULL;
	if(fdb_vlan_globle == NULL){
		fdb_vlan_globle = kmem_cache_alloc(br_fdb_cache, GFP_ATOMIC);
		if (fdb_vlan_globle) {
			memcpy(fdb_vlan_globle->addr.addr, addr, ETH_ALEN);
		}
		else
			return NULL;
	}
	fdb_vlan_globle->dst = dst;
	fdb_vlan_globle->is_local = 0;
	fdb_vlan_globle->is_static = 0;
	fdb_vlan_globle->ageing_timer = jiffies;
	return fdb_vlan_globle;
}
#endif
#if defined(CONFIG_PORT_BINDING) || defined(TCSUPPORT_PORTBIND)
#if defined(TCSUPPORT_CT)
__IMEM struct net_bridge_fdb_entry *__br_fdb_pb_get(struct sk_buff *skb, struct net_bridge *br,
					  const unsigned char *addr)
#else
/* No locking or refcounting, assumes caller has no preempt (rcu_read_lock) */
__IMEM struct net_bridge_fdb_entry *__br_fdb_pb_get(struct net_bridge *br, struct net_bridge_port *p,
					  const unsigned char *addr)
#endif
{
	struct hlist_node *h = NULL;
	struct net_bridge_fdb_entry *fdb = NULL;

#if !defined(TCSUPPORT_CT)
	struct net_device *indev = NULL;
	struct net_device *outdev = NULL;
#endif

	hlist_for_each_entry_rcu(fdb, h, &br->hash[br_mac_hash(addr)], hlist) {
		if (!compare_ether_addr(fdb->addr.addr, addr)) {
			if (unlikely(has_expired(br, fdb)))
				break;

			/* if packet is for cpe, just return */
			if (fdb && fdb->is_local) {
				return fdb;
			}
#if !defined(TCSUPPORT_CT)
			indev = outdev = NULL;
			/* check if inport and outport in same group */
			if (p) {
				indev = p->dev;
			}
			if (fdb && fdb->dst) {
				outdev = fdb->dst->dev;
			}
			//printk("%s:indev->name is %s, outdev->name is %s.\n", __FUNCTION__, indev->name, outdev->name);
			if ( (indev == NULL) ||
				(outdev == NULL) || 
		   (portbind_check_hook == NULL) ||
		   (portbind_check_hook && portbind_check_hook(indev->name, outdev->name)) )
#else
			if ((portbind_check_hook == NULL) || ((portbind_check_hook) && portbind_check_hook(skb->orig_dev, fdb->dst->dev))) 
#endif
			{
				return fdb;
			}
		#if 0
			else {
				/* not in the same group, we can choose search next or just return "NULL", here we choose search next. */
				continue;
			}
		#endif
		}
	}

	return NULL;
}
#endif

/* No locking or refcounting, assumes caller has rcu_read_lock */
__IMEM struct net_bridge_fdb_entry *__br_fdb_get(struct net_bridge *br,
					  const unsigned char *addr)
{
	struct hlist_node *h;
	struct net_bridge_fdb_entry *fdb;

	hlist_for_each_entry_rcu(fdb, h, &br->hash[br_mac_hash(addr)], hlist) {
		if (!compare_ether_addr(fdb->addr.addr, addr)) {
			if (unlikely(has_expired(br, fdb)))
				break;
			return fdb;
		}
	}
	
	ECNT_BR_FDB_HOOK(ECNT_BR_FDB_GET, addr, NULL, NULL);

	return NULL;
}
EXPORT_SYMBOL(__br_fdb_get);

#if defined(CONFIG_ATM_LANE) || defined(CONFIG_ATM_LANE_MODULE)
/* Interface used by ATM LANE hook to test
 * if an addr is on some other bridge port */
int br_fdb_test_addr(struct net_device *dev, unsigned char *addr)
{
	struct net_bridge_fdb_entry *fdb;
	int ret;

	if (!br_port_exists(dev))
		return 0;

	rcu_read_lock();
	fdb = __br_fdb_get(br_port_get_rcu(dev)->br, addr);
	ret = fdb && fdb->dst->dev != dev &&
		fdb->dst->state == BR_STATE_FORWARDING;
	rcu_read_unlock();

	return ret;
}
#endif /* CONFIG_ATM_LANE */

/*
 * Fill buffer with forwarding table records in
 * the API format.
 */
int br_fdb_fillbuf(struct net_bridge *br, void *buf,
		   unsigned long maxnum, unsigned long skip)
{
	struct __fdb_entry *fe = buf;
	int i, num = 0;
	struct hlist_node *h;
	struct net_bridge_fdb_entry *f;

	memset(buf, 0, maxnum*sizeof(struct __fdb_entry));

	rcu_read_lock();
	for (i = 0; i < BR_HASH_SIZE; i++) {
		hlist_for_each_entry_rcu(f, h, &br->hash[i], hlist) {
			if (num >= maxnum)
				goto out;

			if (has_expired(br, f))
				continue;

			if (skip) {
				--skip;
				continue;
			}

			/* convert from internal format to API */
			memcpy(fe->mac_addr, f->addr.addr, ETH_ALEN);

			/* due to ABI compat need to split into hi/lo */
			fe->port_no = f->dst->port_no;
			fe->port_hi = f->dst->port_no >> 8;

			fe->is_local = f->is_local;
			if (!f->is_static)
				fe->ageing_timer_value = jiffies_to_clock_t(jiffies - f->ageing_timer);
			++fe;
			++num;
		}
	}

 out:
	rcu_read_unlock();

	return num;
}

static struct net_bridge_fdb_entry *fdb_create(struct hlist_head *head,
					       struct net_bridge_port *source,
					       const unsigned char *addr,
					       int is_local, struct sk_buff *skb)				       
{
	struct net_bridge_fdb_entry *fdb;

	fdb = kmem_cache_zalloc(br_fdb_cache, GFP_ATOMIC);
	if (fdb) {
		memcpy(fdb->addr.addr, addr, ETH_ALEN);
		hlist_add_head_rcu(&fdb->hlist, head);

		fdb->dst = source;
		fdb->is_local = is_local;
		fdb->is_static = is_local;
		fdb->ageing_timer = jiffies;
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_STBMAC_REPORT)
		fdb->stb_state = 0;
#endif

		ECNT_BR_FDB_HOOK(ECNT_BR_FDB_CREATE, addr, fdb, skb);
	}
	return fdb;
}

static int fdb_insert(struct net_bridge *br, struct net_bridge_port *source,
		  const unsigned char *addr)
{
	struct hlist_head *head = &br->hash[br_mac_hash(addr)];
	struct net_bridge_fdb_entry *fdb;

	if (!is_valid_ether_addr(addr))
		return -EINVAL;

	fdb = fdb_find(head, addr);
	if (fdb) {
		/* it is okay to have multiple ports with same
		 * address, just use the first one.
		 */
		if (fdb->is_local)
			return 0;
		br_warn(br, "adding interface %s with same address "
		       "as a received packet\n",
		       source->dev->name);
		fdb_delete(fdb);
	}

	if (!fdb_create(head, source, addr, 1, NULL))	
		return -ENOMEM;

	return 0;
}

int br_fdb_insert(struct net_bridge *br, struct net_bridge_port *source,
		  const unsigned char *addr)
{
	int ret;

	spin_lock_bh(&br->hash_lock);
	ret = fdb_insert(br, source, addr);
	spin_unlock_bh(&br->hash_lock);
	return ret;
}

__IMEM void br_fdb_update(struct net_bridge *br, struct net_bridge_port *source,
		   const unsigned char *addr, struct sk_buff *skb)
{
	struct hlist_head *head = &br->hash[br_mac_hash(addr)];
	struct net_bridge_fdb_entry *fdb;
	/* some users want to always flood. */
	if (hold_time(br) == 0)
		return;

	/* ignore packets unless we are using this port */
	if (!(source->state == BR_STATE_LEARNING ||
	      source->state == BR_STATE_FORWARDING))
		return;

	fdb = fdb_find(head, addr);
	if (likely(fdb)) {
		/* attempt to update an entry for a local interface */
		if (unlikely(fdb->is_local)) {
			if (net_ratelimit())
				br_warn(br, "received packet on %s with "
					"own address as source address\n",
					source->dev->name);
		} else {
			/* fastpath: update of existing entry */
			fdb->dst = source;
			fdb->ageing_timer = jiffies;

			ECNT_BR_FDB_HOOK(ECNT_BR_FDB_UPDATE, addr, fdb, skb);
			#if defined(TCSUPPORT_HWNAT)
			pktflow_fdb(skb, fdb);
			#endif
		}
	} else {
		spin_lock(&br->hash_lock);
		if (!fdb_find(head, addr))
			fdb_create(head, source, addr, 0, skb);	
		/* else  we lose race and someone else inserts
		 * it first, don't bother updating
		 */
		spin_unlock(&br->hash_lock);
	}
}
EXPORT_SYMBOL(br_fdb_update);

#ifdef TCSUPPORT_BRIDGE_MAC_LIMIT
#define LIMIT_TEST_TOTAL 8
int macNumTotal = 0;

static unsigned char macLimit = 0;

static bool needFlushFlag = false;

typedef struct 
{
	char* devName;
	int devPortNo;
	int macNumByPort;
	int maxNumByPort;
}dev_mac_num;

#if !defined(TCSUPPORT_FWC_FDB_VLAN) 
static dev_mac_num devMacNum[] = 
{
	{"ra0",1,0,0},
	{"eth0.1",3,0,0},
	{"eth0.2",4,0,0},
	{"eth0.3",5,0,0},
	{"eth0.4",6,0,0},
};
#endif
#define CNT_DEVMACNUM (sizeof(devMacNum)/sizeof(dev_mac_num))

void br_fdb_need_flush(struct net_bridge *br)
{
	if(true == needFlushFlag)
	{
		br_fdb_flush(br);
		needFlushFlag = false;
	}
}

bool br_fdb_judge_mac_num(struct net_bridge *br, char* devName)
{
	int i, j;
	bool ret_toal = false, ret_port = false;

	spin_lock_bh(&br->hash_lock);
	macNumTotal = 0;
	for(j = 0;j < CNT_DEVMACNUM;j++)
	{
		devMacNum[j].macNumByPort = 0;
	}
	for (i = 0; i < BR_HASH_SIZE; i++) {
		struct net_bridge_fdb_entry *f;
		struct hlist_node *h, *n;
		hlist_for_each_entry_safe(f, h, n, &br->hash[i], hlist) {
			if (!f->is_local) {
				macNumTotal++;
				for(j = 0;j < CNT_DEVMACNUM;j++)
				{
					if(0 == strcmp(f->dst->dev->name,devMacNum[j].devName))
					{
						devMacNum[j].macNumByPort++;
						break;
					}
				}
			}		
		}
	}

	if(0 == macLimit){
		ret_toal = false;
	}else{	
		if(macNumTotal >= macLimit){
			ret_toal = true;
			goto result;
		}else
			ret_toal = false;
	}

	for(i = 0;i < CNT_DEVMACNUM;i++)
	{
		if(0 == strcmp(devName,devMacNum[i].devName))
		{
			if(0 == devMacNum[i].maxNumByPort){
				ret_port = false;
				goto result;
			}
			if(devMacNum[i].macNumByPort >= devMacNum[i].maxNumByPort){
				ret_port = true;
				goto result;
			}else{
				ret_port = false;
				goto result;
			}
		}
	}
	
	ret_port = false;
	
result:	
	spin_unlock_bh(&br->hash_lock);

	return (ret_toal || ret_port);
}


bool br_fdb_mac_exist(struct net_bridge *br,const unsigned char *addr,char* devName)
{
	int i;

	spin_lock_bh(&br->hash_lock);

	for (i = 0; i < BR_HASH_SIZE; i++) {
		struct net_bridge_fdb_entry *f;
		struct hlist_node *h, *n;
		hlist_for_each_entry_safe(f, h, n, &br->hash[i], hlist) {
			if (!f->is_local) {
				if((0 == strcmp(f->dst->dev->name,devName))&&(0 == compare_ether_addr(f->addr.addr,addr)))
				{
					spin_unlock_bh(&br->hash_lock);
					return true;
				}
			}
		
		}
	}

	spin_unlock_bh(&br->hash_lock);

	return false;
}

static int mac_limit_total_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char val_string[64];
	char* endpo;

	if (count > sizeof(val_string) - 1)
		return -EINVAL ;

	memset(val_string,0,64);
	if (copy_from_user(val_string, buffer, count))
		return -EFAULT ;

	macLimit = simple_strtol(val_string,&endpo,10);

	needFlushFlag = true;
	return count;
}

static int mac_limit_total_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	printk("%d\n",macLimit);
	
	return 0;
}


static int mac_limit_by_port_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char val_string[64];
	char* endpo;
	char port[10];
	int portNo;
	char portMacLimit[10];
	int portMacLimitNum;
	char* p = NULL;
	int i;

	if (count > sizeof(val_string) - 1)
		return -EINVAL ;

	memset(val_string,0,64);
	if (copy_from_user(val_string, buffer, count))
		return -EFAULT ;

	p = strchr(val_string,'-');

	if(NULL == p)
		return -EINVAL;

	memset(port,0,10);
	memmove(port,val_string,p-val_string);
	portNo = simple_strtol(port,&endpo,10);

	memset(portMacLimit,0,10);
	strcpy(portMacLimit,p+1);
	portMacLimitNum = simple_strtol(portMacLimit,&endpo,10);

	for(i = 0;i < CNT_DEVMACNUM;i++)
	{
		if(devMacNum[i].devPortNo == portNo)
		{
			devMacNum[i].maxNumByPort = portMacLimitNum;
			break;
		}
	}

	needFlushFlag = true;
	
	return count;
}

static int mac_limit_by_port_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int i = 0;

	for(i = 1;i < CNT_DEVMACNUM;i++)
	{
		printk("eth0.%d:%d\n",i,devMacNum[i].maxNumByPort);
	}

	return 0;
}
#endif

#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_STBMAC_REPORT)
static int stb_list_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int len = 0, i = 0;
	char stb_mac_list[256] = {0}, stb_mac[20] = {0};
	int hasnew = 0;
	int max_stb_cnt = 14; // 14*18=252
	struct hlist_node *h = NULL, *n = NULL;
	struct net_bridge_fdb_entry *f = NULL;
	unsigned char *addr = NULL;
	int wan_type = 0;
	int stb_count = 0;
	static int stb_store_count = 0;
	struct net_device *ndev = NULL;
	struct net_bridge *br = NULL;
#define		IF_TYPE_WAN_BRIDE		(1<<1)
#define		IF_TYPE_LAN				(1<<2)
#define		IF_TYPE_OTHER			(1<<4)
	int portNum = 0;
	// read lan link status
	int link_status_read = 0;
	char link_status[32] = {0};
	int lanStatus[4] = {0};
	mm_segment_t orgfs;
	struct file *srcf = NULL; 
	char *src = "/proc/tc3162/eth_port_status";

	memset(stb_mac_list, 0, sizeof(stb_mac_list));
	ndev = dev_get_by_name(&init_net, "br0");
	if ( ndev )
		br = netdev_priv(ndev);
	// read lan link status
	orgfs = get_fs();
	set_fs(KERNEL_DS);
	srcf = filp_open(src, O_RDONLY, 0);
	if ( !IS_ERR(srcf) )
	{
		link_status_read = 1;
		srcf->f_op->read(srcf, link_status, sizeof(link_status) - 1, &srcf->f_pos);
		filp_close(srcf,NULL);
	}
	set_fs(orgfs);
	sscanf(link_status, "%d %d %d %d", &lanStatus[0], &lanStatus[1], &lanStatus[2], &lanStatus[3]);
	// end
	if ( br )
	{
		spin_lock_bh(&br->hash_lock);
		for ( i = 0; i < BR_HASH_SIZE; i ++ )
		{
			hlist_for_each_entry_safe(f, h, n, &br->hash[i], hlist)
			{
				if ( f->is_local
					|| has_expired(br, f) )
					continue;

				if ( IF_TYPE_LAN == (f->dst->dev->bind_type & IF_TYPE_LAN) )
				{
					if ( portbind_check_bind_wantype )
						 wan_type = portbind_check_bind_wantype( f->dst->dev->name, f->dst->dev->bind_index );

					if ( (IF_TYPE_OTHER | IF_TYPE_WAN_BRIDE) == ( wan_type & (IF_TYPE_OTHER | IF_TYPE_WAN_BRIDE)) )
					{
						if ( NULL != strstr(f->dst->dev->name, "eth0.") )
						{
							portNum = f->dst->dev->name[5] - '1';
							if ( portNum >=0 && portNum <= 3
								&& 0 == lanStatus[portNum] )
							{
								fdb_delete(f);
								continue;
							}
						}
						if ( !(f->stb_state & STB_NOTIFY_SET) )
							hasnew = 1;
						stb_count ++;
						if ( stb_count <= max_stb_cnt )
						{
							addr = f->addr.addr;
							sprintf(stb_mac, "%02X:%02X:%02X:%02X:%02X:%02X", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
							if ( stb_count > 1 )
								strcat(stb_mac_list, ";");
							strcat(stb_mac_list, stb_mac);
						}
						f->stb_state |= STB_NOTIFY_SET;
					}
					else
					{
						if ( f->stb_state & STB_NOTIFY_SET )
							hasnew = 1;

						f->stb_state &= (~STB_NOTIFY_SET);
					}
				} // if ( IF_TYPE_LAN == (f->dst->dev->bind_type & IF_TYPE_LAN) )
			} // hlist_for_each_entry_rcu(f, h, &br->hash[i], hlist)
		} // for ( i = 0; i < BR_HASH_SIZE; i ++ )
		spin_unlock_bh(&br->hash_lock);
	} // if ( br )

	if ( stb_store_count != stb_count )
		hasnew = 1;
	stb_store_count = stb_count;
	len = sprintf(buf,"%s=%d=%s=\n", (hasnew ? "new" : "normal"), stb_store_count, stb_mac_list);
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

#endif

#if defined(TCSUPPORT_CT_PON)
static int host_list_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int len = 0, i = 0;
	char *stb_mac_list, stb_mac[20] = {0};
	int hasnew = 0;
	int max_stb_cnt = 128;
	struct hlist_node *h = NULL, *n = NULL;
	struct net_bridge_fdb_entry *f = NULL;
	unsigned char *addr = NULL;
	int stb_count = 0;
	static int stb_store_count = 0;
	struct net_device *ndev = NULL;
	struct net_bridge *br = NULL;
#define MAC_LIST_LEN (4096)
	int portNum = 0;
	// read lan link status
	int link_status_read = 0;
	char link_status[32] = {0};
	int lanStatus[4] = {0};
	mm_segment_t orgfs;
	struct file *srcf = NULL; 
	char *src = "/proc/tc3162/eth_port_status";

	stb_mac_list = kmalloc(MAC_LIST_LEN, GFP_ATOMIC);
	if (buf == NULL) {
		printk("Allocate memory for stb_mac fail.\n");
		return -1;
	}
	
	memset(stb_mac_list, 0, MAC_LIST_LEN);
	ndev = dev_get_by_name(&init_net, "br0");
	if ( ndev )
		br = netdev_priv(ndev);
	// read lan link status
	orgfs = get_fs();
	set_fs(KERNEL_DS);
	srcf = filp_open(src, O_RDONLY, 0);
	if ( !IS_ERR(srcf) )
	{
		link_status_read = 1;
		srcf->f_op->read(srcf, link_status, sizeof(link_status) - 1, &srcf->f_pos);
		filp_close(srcf,NULL);
	}
	set_fs(orgfs);
	sscanf(link_status, "%d %d %d %d", &lanStatus[0], &lanStatus[1], &lanStatus[2], &lanStatus[3]);
	// end
	if ( br )
	{
		spin_lock_bh(&br->hash_lock);
		for ( i = 0; i < BR_HASH_SIZE; i ++ )
		{
			hlist_for_each_entry_safe(f, h, n, &br->hash[i], hlist)
			{
				if ( f->is_local
					|| has_expired(br, f) )
					continue;

#if defined(TCSUPPORT_CT_PMINFORM) || defined(TCSUPPORT_CT_JOYME)
				portNum = 0;
#endif
				if ( NULL != strstr(f->dst->dev->name, "eth0.") )
				{
					portNum = f->dst->dev->name[5] - '0';
#if !defined(TCSUPPORT_CT_JOYME)
					if ( portNum >=1 && portNum <= 4
						&& 0 == lanStatus[portNum-1] )
					{
						fdb_delete(f);
						continue;
					}
#endif
				}

#if defined(TCSUPPORT_CT_JOYME)				
				if ( NULL != strstr(f->dst->dev->name, "ra") ) {
					portNum = f->dst->dev->name[2] - '0' + 5;
				}
#endif

#if defined(TCSUPPORT_CT_PMINFORM)
				/* if port mac info not in LAN. */
				if ( portNum < 1 || portNum > 4 )
					continue;
#endif

				stb_count ++;
				if ( stb_count <= max_stb_cnt )
				{
					addr = f->addr.addr;
					sprintf(stb_mac, "%d=%02x:%02x:%02x:%02x:%02x:%02x\n", portNum,addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
					strcat(stb_mac_list, stb_mac);
				}

			} // hlist_for_each_entry_rcu(f, h, &br->hash[i], hlist)
		} // for ( i = 0; i < BR_HASH_SIZE; i ++ )
		spin_unlock_bh(&br->hash_lock);
	} // if ( br )

	len = sprintf(buf, "%s", stb_mac_list);

	*start = buf + off;
	if (len < off + count)
		*eof = 1;
	len -= off;
	if (len > count)
		len = count ;
	if (len <0)
		len = 0;

	kfree(stb_mac_list);

	return len;
}
#endif
