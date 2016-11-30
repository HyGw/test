/*
 *	Forwarding decision
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

#include <linux/err.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/netpoll.h>
#include <linux/skbuff.h>
#include <linux/if_vlan.h>
#include <linux/netfilter_bridge.h>
#include "br_private.h"
#if defined(TCSUPPORT_HWNAT)
#include <linux/pktflow.h>
#endif
#ifdef TCSUPPORT_RA_HWNAT
#include <linux/foe_hook.h>
#endif
#include <ecnt_hook/ecnt_hook_net.h>

#if defined(TCSUPPORT_CT_PPPINFORM) || defined(TCSUPPORT_CT_AUTOREGISTER) || defined(TCSUPPORT_CT_ADSL_BIND1) || defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
#include <linux/proc_fs.h>
#endif
#if defined(TCSUPPORT_CT_PON_SC)
#include <linux/udp.h>
#endif
#if defined(TCSUPPORT_CT_PPPINFORM) || defined(TCSUPPORT_CT_AUTOREGISTER) || defined(TCSUPPORT_CT_ADSL_BIND1) || defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
#if defined(TCSUPPORT_CT_ADSL_BIND1) || defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
static u8 username[256] = {0};
#else
u8  username[65] = {0};
#endif
u8  username_length = 0;
u8  success_flag = 0;
u8  usernamechange_flag = 0;
#endif
#if defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
static uint32_t bridge_state[2] = {0};
#endif
#if defined(TCSUPPORT_CT_ADSL_BIND1)
static char br_name[32] = {0}; /* store the brige nas name. */
static u8 detect_st = 0; /* wll be true when connect to ITMS+ succeed. */
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

extern void tc3162wdog_kick(void);

#ifdef TCSUPPORT_EPON_MAPPING
extern int (*xpon_mode_get_hook)(void);
extern int (*epon_sfu_clsfy_hook)(struct sk_buff *skb, int port);
#endif

#if defined(TCSUPPORT_XPON_IGMP) && defined(TCSUPPORT_MULTICAST_SPEED)
int (*xpon_hgu_multicast_data_hook)(struct sk_buff *skb) = NULL;
EXPORT_SYMBOL(xpon_hgu_multicast_data_hook);
#endif
#if defined(TCSUPPORT_CPU_MT7520)
extern int (*MT7530LanPortMap2Switch_hook)(int port); 
#endif


static int deliver_clone(const struct net_bridge_port *prev,
			 struct sk_buff *skb,
			 void (*__packet_hook)(const struct net_bridge_port *p,
					       struct sk_buff *skb));

/* Don't forward packets to originating port or forwarding diasabled */
static inline int should_deliver(const struct net_bridge_port *p,
				 const struct sk_buff *skb)
{
	return (((p->flags & BR_HAIRPIN_MODE) || skb->dev != p->dev) &&
		p->state == BR_STATE_FORWARDING);
}

static inline unsigned packet_length(const struct sk_buff *skb)
{
	return skb->len - (skb->protocol == htons(ETH_P_8021Q) ? VLAN_HLEN : 0);
}

__IMEM int br_dev_queue_push_xmit(struct sk_buff *skb)
{
	/* drop mtu oversized packets except gso */
	if (packet_length(skb) > skb->dev->mtu && !skb_is_gso(skb))
		kfree_skb(skb);
	else {
		/* ip_fragment doesn't copy the MAC header */
		if (nf_bridge_maybe_copy_header(skb))
			kfree_skb(skb);
		else {
			skb_push(skb, ETH_HLEN);
			dev_queue_xmit(skb);
		}
	}

	return 0;
}

#if defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
/*
update bridge ppp state via dev name
*/
int update_state_bydevname(char *name, int dw_up)
{
#define MAX_SMUX_NUM 8
	uint8_t pidx = 0, eidx = 0;
	uint32_t if_index = 0, if_index_sub = 0;

	if ( !name )
		return -1;

	if ( 'n' == name[0] )
	{
		pidx = name[3] - '0';
		eidx = name[5] - '0';
		if_index = pidx * MAX_SMUX_NUM + eidx;

		if ( if_index < 32 )
		{
			if ( dw_up )
				bridge_state[0] |= ( 1 << if_index );
			else
				bridge_state[0] &= ~( 1 << if_index );
		}
		else if ( ( if_index_sub = (if_index - 32) ) < 32 )
		{
			if ( dw_up )
				bridge_state[1] |= ( 1 << if_index_sub );
			else
				bridge_state[1] &= ~( 1 << if_index_sub );
		}
	}

	return 0;
}

/*
reset bridge ppp state via iface index
*/
int reset_state_byindex(uint32_t if_index)
{
#define MAX_SMUX_NUM 8
	uint8_t pidx = 0, eidx = 0;
	uint32_t if_index_sub = 0;

	if ( if_index < 32 )
		bridge_state[0] &= ~( 1 << if_index );
	else if ( ( if_index_sub = (if_index - 32) ) < 32 )
		bridge_state[1] &= ~( 1 << if_index_sub );

	return 0;
}

static int pppbridge_state_read_proc(char *buf, char **start, off_t off, int count,int *eof, void *data)
{
	int len = 0;

	len = sprintf(buf, "%x %x\n", bridge_state[1], bridge_state[0]);

	*start = buf + off;
	if (len < off + count)
		*eof = 1;
	len -= off;
	if (len > count)
		len = count ;
	if (len < 0 )
		len = 0;

	return len;
}

static int pppbridge_state_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char buff[64] = {0};
	__be32 len = 0;
	__be32 value = 0;

	if ( count >= sizeof(buff) )
		len = sizeof(buff) - 1;
	else
		len = count;

	memset(buff, 0, sizeof(buff));
	if (copy_from_user(buff, buffer, len))
		return -EFAULT;

	sscanf(buff, "%u", &value);
	reset_state_byindex(value);

	return len;
}

int br_pppbridge_state_init(void){

	struct proc_dir_entry *pppusername_proc = NULL;	

	pppusername_proc = create_proc_entry("tc3162/bridge_ppp_sate", 0, NULL);
	if (pppusername_proc == NULL)
		return 1;
	pppusername_proc->read_proc = pppbridge_state_read_proc;
	pppusername_proc->write_proc = pppbridge_state_write_proc;

	memset(bridge_state, 0, sizeof(bridge_state));

	return 0;
}

void br_pppbridge_state_fini(void)
{
	remove_proc_entry("tc3162/bridge_ppp_sate",0);	
	return;
}
#endif

#if defined(TCSUPPORT_CT_PON_SC)
#define PKT_DROP -1
#define PKT_OK 0
#define PKT_IGNORE 1
#define BOOTP_SRC 68
#define BOOTP_DST 67
#define	BOOTREQUEST	1
#define DHCP_SNAME_LEN 64
#define DHCP_FILE_LEN 128
#define DHCP_PADDING 0x00
#define DHCP_OPTION_OVER 0x34
#define DHCP_END 0xFF
#define DHCP_MAGIC 0x63825363
#define OPT_CODE 0
#define OPT_LEN 1
#define OPT_VENDORCLS 60

struct dhcp_packet {
  	u_int8_t  op;		/* Message opcode/type */
	u_int8_t  htype;	/* Hardware addr type (see net/if_types.h) */
	u_int8_t  hlen;		/* Hardware addr length */
	u_int8_t  hops;		/* Number of relay agent hops from client */
	u_int32_t xid;		/* Transaction ID */
	u_int16_t secs;		/* Seconds since client started looking */
	u_int16_t flags;	/* Flag bits */
	struct in_addr ciaddr;	/* Client IP address (if already in use) */
	struct in_addr yiaddr;	/* Client IP address */
	struct in_addr siaddr;	/* IP address of next server to talk to */
	struct in_addr giaddr;	/* DHCP relay agent IP address */
	unsigned char chaddr [16];	/* Client hardware address */
	char sname [DHCP_SNAME_LEN];	/* Server name */
	char file [DHCP_FILE_LEN];	/* Boot filename */
	uint32_t cookie; 	/* Magic cookie */
#define DEF_OPTLEN 4
	u_int8_t options [DEF_OPTLEN];
};

static inline __be16 vlan_proto(const struct sk_buff *skb)
{
	return vlan_eth_hdr(skb)->h_vlan_encapsulated_proto;
}
#define IS_VLAN_IP(skb) \
	(skb->protocol == htons(ETH_P_8021Q) && \
	 vlan_proto(skb) == htons(ETH_P_IP))

static u_int8_t br_dhcpChk = 0;
/* get dhcp option via code id */
static u_int8_t *__get_option(struct dhcp_packet *packet, const int code, int *retCodeLen,
	const int totaloptionLen)
{
	int i, length;
	uint8_t *optionptr = NULL;
	int over = 0, done = 0, curr = 0;

	optionptr = packet->options;
	i = 0;
	length = totaloptionLen;

	while ( !done )
	{
		if (i >= length) // option fields too long.
			return NULL;

		if ( code == optionptr[i + OPT_CODE] )
		{
			if ( i + 1 + optionptr[i + OPT_LEN] >= length ) // option fields too long.
				return NULL;

			*retCodeLen = optionptr[i + OPT_LEN];
			return optionptr + i + 2;
		}
		
		switch ( optionptr[i + OPT_CODE] )
		{
			case DHCP_PADDING:
				i++;
				break;
			case DHCP_OPTION_OVER:
				if ( i + 1 + optionptr[i + OPT_LEN] >= length ) // option fields too long.
					return NULL;
				over = optionptr[i + 3];
				i += optionptr[OPT_LEN] + 2;
				break;
			case DHCP_END:
				done = 1;
				break;
			default:
				i += optionptr[OPT_LEN + i] + 2;
				break;
		}
	}

	return NULL;
}

/* check dhcp option60 whether valid */
static int checkDHCPv4Option60(const struct sk_buff *skb)
{
	struct iphdr *iph = NULL;
	struct udphdr *uh = NULL;
	struct dhcp_packet *btph = NULL;
	const int udphdr_len = sizeof(struct udphdr);
	const int dhcppkt_len = sizeof(struct dhcp_packet);
	int udp_total_len = 0, opt60_len = 0;
	u_int8_t *opt60_ptr = NULL;

	if ( htons(ETH_P_IP) == skb->protocol )
		iph = ip_hdr(skb);
	else if( IS_VLAN_IP(skb) )
		iph = (struct iphdr *)(skb_mac_header(skb) + VLAN_ETH_HLEN);
	else
		return PKT_IGNORE;

	if ( iph->ihl < 5 || 4 != iph->version
		|| IPPROTO_UDP != iph->protocol )
		return PKT_IGNORE;

	uh = (struct udphdr *)((uint8_t *) iph + iph->ihl * 4);
	if ( htons(BOOTP_SRC) == uh->source
		&& htons(BOOTP_DST) == uh->dest )
	{
		udp_total_len = ntohs(uh->len);
		if ( udp_total_len - udphdr_len >= dhcppkt_len )
		{
			btph = (struct dhcp_packet *)((uint8_t *) uh + udphdr_len);
			if ( htonl(DHCP_MAGIC) != btph->cookie )
				return PKT_DROP;

			opt60_ptr = __get_option(btph, OPT_VENDORCLS, &opt60_len,
				udp_total_len - udphdr_len - (dhcppkt_len - DEF_OPTLEN));

			if ( NULL == opt60_ptr || opt60_len <= 2 /* option 60 must be started with 0x00, 0x00, IF */
				|| (0 != opt60_ptr[0] || 0 != opt60_ptr[1]) )
				return PKT_DROP;

			return PKT_OK;
		}
		else
			return PKT_DROP;
	}

	return PKT_IGNORE;
}

/* dhcp option60  check flag read proc  */
static int dhcpChk_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int len = 0;

	len = sprintf(buf,"%u\n", br_dhcpChk);

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
/* dhcp option60  check flag write proc  */
static int dhcpChk_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char buff[64] = {0};
	__be32 len = 0;
	__be32 value = 0;

	if ( count >= sizeof(buff) )
		len = sizeof(buff) - 1;
	else
		len = count;

	memset(buff, 0, sizeof(buff));
	if (copy_from_user(buff, buffer, len))
		return -EFAULT;

	sscanf(buff, "%u", &value);
	br_dhcpChk = value;

	return len;
}

/* dhcp check flag proc init. */
int br_dhcpCheck_init(void)
{
	struct proc_dir_entry *br_dhcpChk_proc = NULL;

	br_dhcpChk_proc = create_proc_entry("tc3162/br_dhcp_check", 0, NULL);
	if( NULL == br_dhcpChk_proc )
	{
		printk("ERROR!Create proc entry br_dhcp_check fail!\n");
		return -1;
	}

	br_dhcpChk_proc->read_proc = dhcpChk_read_proc;
	br_dhcpChk_proc->write_proc = dhcpChk_write_proc;

	return 0;
}
/* dhcp check flag proc finish */
void br_dhcpCheck_fini(void)
{
	remove_proc_entry("tc3162/br_dhcp_check",0);
	return;
}
#endif

__IMEM int br_forward_finish(struct sk_buff *skb)
{
#if defined(TCSUPPORT_CT_PON_SC)
	if ( br_dhcpChk
		&& 'n' == skb->dev->name[0]
		&& (skb->dev->priv_flags & IFF_OSMUX)
		&& PKT_DROP == checkDHCPv4Option60(skb) )
	{
		printk("\nerr: invalid stb dhcp packet, drop it\n");
		kfree_skb(skb);
		return;
	}
#endif

	return NF_HOOK(NFPROTO_BRIDGE, NF_BR_POST_ROUTING, skb, NULL, skb->dev,
		       br_dev_queue_push_xmit);

}

__IMEM static void __br_deliver(const struct net_bridge_port *to, struct sk_buff *skb)
{
	skb->dev = to->dev;

	if (unlikely(netpoll_tx_running(to->dev))) {
		if (packet_length(skb) > skb->dev->mtu && !skb_is_gso(skb))
			kfree_skb(skb);
		else {
			skb_push(skb, ETH_HLEN);
			br_netpoll_send_skb(to, skb);
		}
		return;
	}
#if defined(TCSUPPORT_CT_PORTSLIMIT)
	if ( skb->vlan_tag_flag & VLAN_TAG_IGMP_QUERYFLAG )
	{
		skb->vlan_tag_flag &= ~(VLAN_TAG_IGMP_QUERYFLAG);
		if ( NULL != skb->dev
			&& ('n' == skb->dev->name[0] && 'a' == skb->dev->name[1] && 's' == skb->dev->name[2]) )
		{
			kfree_skb(skb);
			return;
		}
	}
#endif

	NF_HOOK(NFPROTO_BRIDGE, NF_BR_LOCAL_OUT, skb, NULL, skb->dev,
		br_forward_finish);
}

#if defined(TCSUPPORT_CT_ADSL_BIND1)
static int pppusername_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char buff[64] = {0};
	__be32 len = 0;
	__be32 value = 0;

	if ( count >= sizeof(buff) )
		len = sizeof(buff) - 1;
	else
		len = count;

	memset(buff, 0, sizeof(buff));
	if (copy_from_user(buff, buffer, len))
		return -EFAULT;

	sscanf(buff, "%u", &value);
	detect_st = value;

	return len;
}
#endif

#if defined(TCSUPPORT_CT_PPPINFORM) || defined(TCSUPPORT_CT_AUTOREGISTER) || defined(TCSUPPORT_CT_ADSL_BIND1)
static int pppusername_read_proc(char *buf, char **start, off_t off, int count,int *eof, void *data)
{
	int len = 0;
	int i =0;

#if defined(TCSUPPORT_CT_ADSL_BIND1)
	len = sprintf(buf, "%d %s %s ", detect_st,
				(1 == success_flag) ? "up" : "down", br_name);
	for ( i = 0; i < username_length - 1; i++ )
		len += sprintf(buf+len,"%c", username[i]);
	len += sprintf(buf+len, "%c\n", username[i]);
	success_flag = 0;
#else
	if(1 == success_flag){
		if(1 == usernamechange_flag)
			len = sprintf(buf, "Status: up. Username_state: changed.\n");
		else
			len = sprintf(buf, "Status: up. Username_state: unchanged.\n");
		len += sprintf(buf+len, "Username: ");
		for(i=0; i < username_length-1; i++)
	 		len += sprintf(buf+len,"%c", username[i]);
		len += sprintf(buf+len, "%c\n", username[i]);
		usernamechange_flag = 0;
	}
	else
		len = sprintf(buf, "Status: down \n");
#endif

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

int br_pppcheck_init(void){

	struct proc_dir_entry *pppusername_proc = NULL;	

	pppusername_proc = create_proc_entry("tc3162/pppusername", 0, NULL);
	if (pppusername_proc == NULL)
		return 1;
	pppusername_proc->read_proc = pppusername_read_proc;
	pppusername_proc->write_proc = 
#if defined(TCSUPPORT_CT_ADSL_BIND1)
		pppusername_write_proc;
#else
		NULL;
#endif

	return 0;
}

void br_pppcheck_fini(void)
{
	remove_proc_entry("tc3162/pppusername",0);	
	return;
}

#endif

#if defined(TCSUPPORT_CT_ADSL_HN)
#define	IF_TYPE_WAN_BRIDE		(1<<1)
#define	IF_TYPE_INTERNET		(1<<3)
#endif

__IMEM static void __br_forward(const struct net_bridge_port *to, struct sk_buff *skb)
{
	struct net_device *indev;

	if (skb_warn_if_lro(skb)) {
		kfree_skb(skb);
		return;
	}
#if defined(TCSUPPORT_CT_STB_TEST)
	if (skb->protocol == 0x980B && to->dev->name[0] == 'n') {
		kfree_skb(skb);
		return;
	}
#endif

	indev = skb->dev;
	skb->dev = to->dev;
	skb_forward_csum(skb);
#ifdef TCSUPPORT_EPON_MAPPING
	if(xpon_mode_get_hook && epon_sfu_clsfy_hook && (2 == xpon_mode_get_hook()) ){
		// indev = "eth0/eth0.x" && out dev = "pon" && SFU mode
#ifdef TCSUPPORT_PON_IP_HOST
		if (((skb->mark >>28) < 5) && (skb->dev->name[0] == 'n')){
#else
		if (((skb->mark >>28) < 5) && (skb->dev->name[1] =='o')){
#endif
			epon_sfu_clsfy_hook(skb, (skb->mark >>28)-1);
		}
	}
#endif

#if defined(TCSUPPORT_CT_PPPINFORM) || defined(TCSUPPORT_CT_AUTOREGISTER) || defined(TCSUPPORT_CT_ADSL_BIND1) || defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
	int i = 0;
	u8 *cp=NULL;
	u16 p2p_protocol;
	u8  p2p_code;
	u16 p2p_length, length = 0;
	u8  p2p_valuesize;
	static u16 p2p_oldsessionID;
	u16 p2p_newsessionID;
	u16 packet_protocol;
	u8  new_username[65] = {0};

#if defined(TCSUPPORT_CT_ADSL_BIND1)
	if ( 1 == detect_st )
	{
#endif

	cp = skb->data;
	packet_protocol =  *(u16 *)(cp+4);
	if((skb->protocol == 0x8864) || packet_protocol== 0x8864){
	p2p_protocol = *(u16 *)(cp+6);
	p2p_code = *(u8 *)(cp+8);

	switch(p2p_protocol){
		case 0xc223:                    //chap
			if(p2p_code == 0x02
#if defined(TCSUPPORT_CT_ADSL_HN)
				&& ((skb->dev->bind_type & (IF_TYPE_INTERNET | IF_TYPE_WAN_BRIDE)) == (IF_TYPE_INTERNET | IF_TYPE_WAN_BRIDE))
#endif
			){
				printk("\r\n chap auth response!\r\n");
				p2p_oldsessionID = *(u16 *)(cp+2);
				p2p_length = *(u16 *)(cp+10);
				p2p_valuesize = *(u8 *)(cp+12);
				length = p2p_length-p2p_valuesize-5;
				
				usernamechange_flag = 0;
				
				if(length != username_length){
					username_length = length;
					usernamechange_flag = 1;
					for(i=0; i < username_length; i++)
						username[i] = *(u8 *)(cp+13+p2p_valuesize+i);
					success_flag = 0;
					break;
				}
				
				for(i=0; i < username_length; i++){
					new_username[i] = *(u8 *)(cp+13+p2p_valuesize+i);
					if(username[i] != new_username[i]){
						usernamechange_flag = 1;					
						username[i] = new_username[i];
					}
				}
				success_flag = 0;
				break;
			}
			if(p2p_code == 0x03
#if defined(TCSUPPORT_CT_ADSL_HN)
				&& ((indev->bind_type & (IF_TYPE_INTERNET | IF_TYPE_WAN_BRIDE)) == (IF_TYPE_INTERNET | IF_TYPE_WAN_BRIDE))
#endif
			){
				printk("\r\n chap auth success!\r\n");
				p2p_newsessionID = *(u16 *)(cp+2); 
				if(p2p_newsessionID == p2p_oldsessionID){
					success_flag = 1;
#if defined(TCSUPPORT_CT_ADSL_BIND1)
					if ( indev )
						snprintf(br_name, sizeof(br_name) - 1,
						"%s", indev->name);
#endif
#if defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
					if ( indev )
						update_state_bydevname(indev->name, 1);
#endif
				}
			}
			break;
		case 0xc023:                      //pap
			if(p2p_code == 0x01
#if defined(TCSUPPORT_CT_ADSL_HN)
				&& ((skb->dev->bind_type & (IF_TYPE_INTERNET | IF_TYPE_WAN_BRIDE)) == (IF_TYPE_INTERNET | IF_TYPE_WAN_BRIDE))
#endif
			){
				p2p_oldsessionID = *(u16 *)(cp+2);
				length = *(u8 *)(cp+12);
				
				usernamechange_flag = 0;
				
				if(length != username_length){
					username_length = length;
					usernamechange_flag = 1;
					for(i=0; i < username_length; i++)
						username[i] = *(u8 *)(cp+13+i);
					success_flag = 0;
					break;
				}
				
				for(i=0; i < username_length; i++){
					new_username[i] = *(u8 *)(cp+13+i);			
					if(username[i] != new_username[i]){					
						usernamechange_flag = 1;
						username[i] = new_username[i];
					}
				}
				success_flag = 0;
				break;
			}
			if(p2p_code == 0x02
#if defined(TCSUPPORT_CT_ADSL_HN)
			    && ((indev->bind_type & (IF_TYPE_INTERNET | IF_TYPE_WAN_BRIDE)) == (IF_TYPE_INTERNET | IF_TYPE_WAN_BRIDE))
#endif
			){
				p2p_newsessionID = *(u16 *)(cp+2); 
				if(p2p_newsessionID == p2p_oldsessionID){
					success_flag = 1;	
#if defined(TCSUPPORT_CT_ADSL_BIND1)
					if ( indev )
						snprintf(br_name, sizeof(br_name) - 1,
						"%s", indev->name);
#endif
#if defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
					if ( indev )
						update_state_bydevname(indev->name, 1);
#endif
				}
			}
			break;
		case 0xc021:
			p2p_newsessionID = *(u16 *)(cp+2); 
			if(p2p_code == 0x06 && success_flag == 1 && (p2p_newsessionID == p2p_oldsessionID)){
				success_flag = 0;
			}
#if defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
			if ( 0x06 == p2p_code && indev )
				update_state_bydevname(indev->name, 0);
#endif
			break;
		default:
			break;
		}
	}

#if defined(TCSUPPORT_CT_ADSL_BIND1)
	}
#endif
#endif

#ifdef CONFIG_QOS
#if defined(CONFIG_IMQ) || defined(CONFIG_IMQ_MODULE)
		skb->mark |= QOS_DEFAULT_MARK;
#endif
#endif
	NF_HOOK(NFPROTO_BRIDGE, NF_BR_FORWARD, skb, indev, skb->dev,
		br_forward_finish);
}

/* called with rcu_read_lock */
__IMEM void br_deliver(const struct net_bridge_port *to, struct sk_buff *skb)
{
	if (should_deliver(to, skb)) {
		__br_deliver(to, skb);
		return;
	}

	kfree_skb(skb);
}

/* called with rcu_read_lock */
__IMEM void br_forward(const struct net_bridge_port *to, struct sk_buff *skb, struct sk_buff *skb0)
{
	struct net_data_s net_data;
	int ret = 0;

	net_data.pskb = &skb ;
	ret = ECNT_BR_FORWARD_HOOK(ECNT_BR_FORWARD_BR_FORWARD, &net_data);
	if(ECNT_RETURN == ret)
	{
		return ;
	}
	else if(ECNT_RETURN_DROP == ret)
	{
		goto out ;
	}

	if (should_deliver(to, skb)) {
		if (skb0)
			deliver_clone(to, skb, __br_forward);
		else
			__br_forward(to, skb);
		return;
	}
	
out:
	if (!skb0)
		kfree_skb(skb);
}
#if defined(TCSUPPORT_XPON_IGMP)
EXPORT_SYMBOL(br_forward);
#endif

static int deliver_clone(const struct net_bridge_port *prev,
			 struct sk_buff *skb,
			 void (*__packet_hook)(const struct net_bridge_port *p,
					       struct sk_buff *skb))
{
	struct net_device *dev = BR_INPUT_SKB_CB(skb)->brdev;

	skb = skb_clone(skb, GFP_ATOMIC);
	if (!skb) {
		dev->stats.tx_dropped++;
		return -ENOMEM;
	}

	__packet_hook(prev, skb);
	return 0;
}

static struct net_bridge_port *maybe_deliver(
	struct net_bridge_port *prev, struct net_bridge_port *p,
	struct sk_buff *skb,
	void (*__packet_hook)(const struct net_bridge_port *p,
			      struct sk_buff *skb))
{
	int err;

	if (!should_deliver(p, skb))
		return prev;

	if (!prev)
		goto out;

	err = deliver_clone(prev, skb, __packet_hook);
	if (err)
		return ERR_PTR(err);

out:
	return p;
}


/* called under bridge lock */
static void br_flood(struct net_bridge *br, struct sk_buff *skb,
		     struct sk_buff *skb0,
		     void (*__packet_hook)(const struct net_bridge_port *p,
					   struct sk_buff *skb))
{
	struct net_bridge_port *p;
	struct net_bridge_port *prev;
	struct net_data_s net_data;
	int ret = 0;

	prev = NULL;

#if defined(TCSUPPORT_XPON_IGMP) && defined(TCSUPPORT_MULTICAST_SPEED)
    int igmp_snoop_flag = false;
    int hw_flood_flag   = false;
    int switch_port = 0;
    int port = 0;
    int mask = 0;
    int index = -1;
    int proc_hw_flag = 0;
#endif

	net_data.pskb = &skb ;
	ret = ECNT_BR_FORWARD_HOOK(ECNT_BR_FORWARD_BR_FLOOD, &net_data);
	if(ECNT_RETURN == ret)
	{
		return ;
	}
	else if(ECNT_RETURN_DROP == ret)
	{
		goto out ;
	}

#if defined(TCSUPPORT_HWNAT)
	pktflow_free(skb);
#endif
#ifdef TCSUPPORT_RA_HWNAT

#if defined(TCSUPPORT_XPON_IGMP) && defined(TCSUPPORT_MULTICAST_SPEED)
    proc_hw_flag    = get_multicast_flood_hw_flag();
    igmp_snoop_flag = get_multicast_snooping_state();
    /*disable snooping*/
    if(!igmp_snoop_flag && proc_hw_flag)
    {
        /*hgu multicast data flow*/
        if(xpon_hgu_multicast_data_hook && xpon_hgu_multicast_data_hook(skb))
        {
            /*hw accelerating*/
            index = igmp_hwnat_flow_index(skb);
            if(index > 0)
            {
                hw_flood_flag = true;
            }
            //printk("line = %d, function= %s,index = %d.\n",__LINE__,__FUNCTION__, index);
        }
    }

    if(hw_flood_flag)
    {
        add_multicast_flood_hwentry(skb);

    }
    else
    {
        if (ra_sw_nat_hook_free)
            ra_sw_nat_hook_free(skb);
    }
#else
    {
        if (ra_sw_nat_hook_free)
            ra_sw_nat_hook_free(skb);
    }
#endif
#endif
	tc3162wdog_kick();
	list_for_each_entry_rcu(p, &br->port_list, list) {
		prev = maybe_deliver(prev, p, skb, __packet_hook);
		if (IS_ERR(prev))
			goto out;
#if defined(TCSUPPORT_XPON_IGMP) && defined(TCSUPPORT_MULTICAST_SPEED)
        else
        {
            if(hw_flood_flag)
            {
                /*calc mask*/
                port = igmp_hwnat_get_port(prev);
                /*lan port*/
                if(port >= 0 && port <= 3)
                {
#if defined(TCSUPPORT_CPU_MT7520)
                    if(MT7530LanPortMap2Switch_hook)
                    {
                        switch_port = MT7530LanPortMap2Switch_hook(port);
                    }
                    port = switch_port;
#endif
                    mask |= 1 << port;
                }
                /*wifi port*/
                else if(port >= HWNAT_WLAN_IF_BASE)
                {
                    mask |= 1 << port;
                }
            }
        }
#endif
	}

	if (!prev)
		goto out;
    
#if defined(TCSUPPORT_XPON_IGMP) && defined(TCSUPPORT_MULTICAST_SPEED)
    /*update mask*/
    if(hw_flood_flag)
    {
        update_multicast_flood_hwentry(index, mask);
        update_multicast_flood_mask(index);
    }
#endif

	if (skb0)
		deliver_clone(prev, skb, __packet_hook);
	else
		__packet_hook(prev, skb);
	return;

out:
	if (!skb0)
		kfree_skb(skb);
}


/* called with rcu_read_lock */
void br_flood_deliver(struct net_bridge *br, struct sk_buff *skb)
{
	br_flood(br, skb, NULL, __br_deliver);
}

/* called under bridge lock */
void br_flood_forward(struct net_bridge *br, struct sk_buff *skb,
		      struct sk_buff *skb2)
{
	#if defined(TCSUPPORT_BRIDGE_FASTPATH)
	/*MARK sc_mac_learned flag, the flooding packets can't be learned into fastbridge table*/
	skb->sc_mac_learned |=FB_FLOOD_PKT;
	#endif

	br_flood(br, skb, skb2, __br_forward);
}

#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
/* called with rcu_read_lock */
static void br_multicast_flood(struct net_bridge_mdb_entry *mdst,
			       struct sk_buff *skb, struct sk_buff *skb0,
			       void (*__packet_hook)(
					const struct net_bridge_port *p,
					struct sk_buff *skb))
{
	struct net_device *dev = BR_INPUT_SKB_CB(skb)->brdev;
	struct net_bridge *br = netdev_priv(dev);
	struct net_bridge_port *prev = NULL;
	struct net_bridge_port_group *p;
	struct hlist_node *rp;
#ifdef TCSUPPORT_IGMPSNOOPING_ENHANCE
	struct net_bridge_port *preport = NULL;
	int ret = 0;
	int check = 0;
	int num_record = 0;
#endif

	rp = rcu_dereference(br->router_list.first);
	p = mdst ? rcu_dereference(mdst->ports) : NULL;
	while (p || rp) {
		struct net_bridge_port *port, *lport, *rport;

		lport = p ? p->port : NULL;
		rport = rp ? hlist_entry(rp, struct net_bridge_port, rlist) :
			     NULL;

		port = (unsigned long)lport > (unsigned long)rport ?
		       lport : rport;
#ifdef TCSUPPORT_IGMPSNOOPING_ENHANCE
		num_record++;/*total muticast records counter*/
		if(((ret=br_multicast_port_pass(p, port, skb)) == 1) /*case 1: pass*/
				&& (port != preport)){
			preport = port;
#endif
		prev = maybe_deliver(prev, port, skb, __packet_hook);

		if (IS_ERR(prev))
			goto out;
#ifdef TCSUPPORT_IGMPSNOOPING_ENHANCE	
		}else if(ret == 2){
			check++; /**case 2: filt_mode =MCAST_EXCLUDE & skb's s_addr not equal the muticast record's s_addr**/
		}
#endif

		if ((unsigned long)lport >= (unsigned long)port)
			p = rcu_dereference(p->next);
		if ((unsigned long)rport >= (unsigned long)port)
			rp = rcu_dereference(rp->next);
	}

	if (!prev)
		goto out;
#ifdef TCSUPPORT_IGMPSNOOPING_ENHANCE
	/*if case 2's num equal muticast records, that means the current skb is in MCAST_EXCLUDE mode
	and it is not in any of the records, so	we should consider the skb is "INCLUDE" and allow it pass*/
	if((check!=0) && (check == num_record)){
		deliver_clone(prev, skb, __packet_hook);
		goto out;
	}
#endif

	if (skb0)
		deliver_clone(prev, skb, __packet_hook);
	else
		__packet_hook(prev, skb);
	return;

out:
	if (!skb0)
		kfree_skb(skb);
}

/* called with rcu_read_lock */
void br_multicast_deliver(struct net_bridge_mdb_entry *mdst,
			  struct sk_buff *skb)
{
	br_multicast_flood(mdst, skb, NULL, __br_deliver);
}

/* called with rcu_read_lock */
void br_multicast_forward(struct net_bridge_mdb_entry *mdst,
			  struct sk_buff *skb, struct sk_buff *skb2)
{
	br_multicast_flood(mdst, skb, skb2, __br_forward);
}

#if defined(CONFIG_PORT_BINDING) || defined(TCSUPPORT_PORTBIND)
static struct net_bridge_port *maybe_pb_deliver(
	struct net_bridge_port *prev, struct net_bridge_port *p,
#if !defined(TCSUPPORT_CT)
	struct net_bridge_port *pIN, 
#endif
#if defined(TCSUPPORT_XPON_IGMP) && defined(TCSUPPORT_MULTICAST_SPEED)
    int index,
#endif
	struct sk_buff *skb,
	void (*__packet_hook)(const struct net_bridge_port *p,
			      struct sk_buff *skb))
{
	int err = 0;
	struct net_device *indev = NULL;
	struct net_device *outdev = NULL;

#if defined(TCSUPPORT_XPON_IGMP) && defined(TCSUPPORT_MULTICAST_SPEED)
    int port = 0;
    int switch_port = 0;
    int mask = 0;
    int igmp_snoop_flag = false;
    int hw_flood_flag   = false;
    int proc_hw_flag = 0;
#endif

	if (!should_deliver(p, skb))
		return prev;

	if (!prev)
		goto out;
#if defined(TCSUPPORT_CT)
	indev = skb->orig_dev;
#else
	indev = outdev = NULL;
	if (pIN) {
		indev = pIN->dev;
	}
#endif
	if (prev) {
		outdev = prev->dev;
	}
	//printk("%s:indev->name is %s, outdev->name is %s.\n", __FUNCTION__, indev->name, outdev->name);
	/* in dev and out dev is not in the same group */
	if ( (indev == NULL) || 
		(outdev == NULL) ||
		(portbind_check_hook == NULL) ||
		((portbind_check_hook) && portbind_check_hook(indev->name, outdev->name)) ) {
		/* it has been checked, not check again */
		skb->portbind_mark |= MASK_OUT_DEV;
#if defined(TCSUPPORT_XPON_IGMP) && defined(TCSUPPORT_MULTICAST_SPEED)
        igmp_snoop_flag = get_multicast_snooping_state();
        proc_hw_flag    = get_multicast_flood_hw_flag();
        if(!igmp_snoop_flag && proc_hw_flag)
        {
            if(xpon_hgu_multicast_data_hook && xpon_hgu_multicast_data_hook(skb))
            {
                if(0 <  index)
                {
                    hw_flood_flag = 1 ; 
                }
            }
        }
        
        if(hw_flood_flag)
        {
            /*calc mask*/
            port = igmp_hwnat_get_port(prev);
            if(port >= 0 && port <= 3)
            {
#if defined(TCSUPPORT_CPU_MT7520)
                if(MT7530LanPortMap2Switch_hook)
                {
                    switch_port = MT7530LanPortMap2Switch_hook(port);
                }
                port = switch_port;
#endif
                mask |= 1 << port;
            }
            else if(port >= HWNAT_WLAN_IF_BASE)
            {
                mask |= 1 << port;
            }
            
            update_multicast_flood_hwentry(index, mask);
        }
#endif
        err = deliver_clone(prev, skb, __packet_hook);

		if (err)
			return ERR_PTR(err);
	}

out:
	return p;
}
/* called under bridge lock */
static void br_pb_flood(struct net_bridge *br,
#if !defined(TCSUPPORT_CT)
 			struct net_bridge_port *pIN,
#endif
			struct sk_buff *skb, struct sk_buff *skb0,
		     void (*__packet_hook)(const struct net_bridge_port *p,
					   struct sk_buff *skb))
{
	struct net_bridge_port *p = NULL;
	struct net_bridge_port *prev = NULL;
	struct net_data_s net_data;
	int ret = 0;
	
#if !defined(TCSUPPORT_CT)
	struct net_device *indev = NULL;
	struct net_device *outdev = NULL;
#endif

#if defined(TCSUPPORT_XPON_IGMP) && defined(TCSUPPORT_MULTICAST_SPEED)
    int port=0;
    int mask = 0;
    int switch_port = 0;
    int index = -1;
    int igmp_snoop_flag = false;
    int hw_flood_flag   = false;
    int proc_hw_flag = 0;
#endif

	net_data.pskb = &skb ;
	ret = ECNT_BR_FORWARD_HOOK(ECNT_BR_FORWARD_BR_PB_FLOOD, &net_data);
	if(ECNT_RETURN == ret)
	{
		return ;
	}
	else if(ECNT_RETURN_DROP == ret)
	{
		goto out ;
	}

#if defined(TCSUPPORT_HWNAT)
	pktflow_free(skb);
#endif
#ifdef TCSUPPORT_RA_HWNAT

#if defined(TCSUPPORT_XPON_IGMP) && defined(TCSUPPORT_MULTICAST_SPEED)
    proc_hw_flag    = get_multicast_flood_hw_flag();
    igmp_snoop_flag = get_multicast_snooping_state();
    /*disable snooping*/
    if(!igmp_snoop_flag && proc_hw_flag)
    {
        /*hgu multicast data flow*/
        if(xpon_hgu_multicast_data_hook && xpon_hgu_multicast_data_hook(skb))
        {
            /*hw accelerating*/
            index = igmp_hwnat_flow_index(skb);
            if(index > 0)
            {
                hw_flood_flag = true;
            }
        }
    }

    if(hw_flood_flag)
    {
        add_multicast_flood_hwentry(skb);
    }
    else
    {
        if (ra_sw_nat_hook_free)
            ra_sw_nat_hook_free(skb);
    }
#else
    {
        if (ra_sw_nat_hook_free)
            ra_sw_nat_hook_free(skb);
    }
#endif
#endif
	tc3162wdog_kick();
	list_for_each_entry_rcu(p, &br->port_list, list) {
		prev = maybe_pb_deliver(prev, p, 
#if !defined(TCSUPPORT_CT)
			pIN, 
#endif
#if defined(TCSUPPORT_XPON_IGMP) && defined(TCSUPPORT_MULTICAST_SPEED)
			index, 
#endif
			skb, __packet_hook);
		if (IS_ERR(prev))
			goto out;
	}

	if (!prev)
		goto out;

#if !defined(TCSUPPORT_CT)
	indev = outdev = NULL;
	if (pIN) {
		indev = pIN->dev;
	}
	if (prev) {
		outdev = prev->dev;
	}
#endif


	
#if defined(TCSUPPORT_CT)	
	if ( (prev != NULL) && 
		( (portbind_check_hook == NULL) || 
		((portbind_check_hook) && (portbind_check_hook(skb->orig_dev, prev->dev)))) ) 
#else
	if ( (prev != NULL) && 
		((indev == NULL) ||
		(outdev == NULL) ||
		(portbind_check_hook == NULL) || 
		((portbind_check_hook) && (portbind_check_hook(indev->name, outdev->name)))) )
#endif
 	{
#if defined(TCSUPPORT_XPON_IGMP) && defined(TCSUPPORT_MULTICAST_SPEED)
        /*update mask*/
        if(hw_flood_flag)
        {
            port = igmp_hwnat_get_port(prev);
            if(port >= 0 && port <= 3)
            {
#if defined(TCSUPPORT_CPU_MT7520)
                if(MT7530LanPortMap2Switch_hook)
                {
                    switch_port = MT7530LanPortMap2Switch_hook(port);
                }
                port = switch_port;
#endif
                mask |= 1 << port;
            }
            else if(port >= HWNAT_WLAN_IF_BASE)
            {
                mask |= 1 << port;
            }
            
            update_multicast_flood_hwentry(index, mask);
            update_multicast_flood_mask(index);
        }
#endif
		if (skb0)
			deliver_clone(prev, skb, __packet_hook);
		else
			__packet_hook(prev, skb);
		return;
	}

#if defined(TCSUPPORT_XPON_IGMP) && defined(TCSUPPORT_MULTICAST_SPEED)
    if(hw_flood_flag)
    {
        update_multicast_flood_mask(index);
    }
#endif

out:
	if (!skb0)
		kfree_skb(skb);
}

/* called under bridge lock */
void br_flood_pb_forward(struct net_bridge *br, 
#if !defined(TCSUPPORT_CT)
			struct net_bridge_port *p,
#endif
			struct sk_buff *skb, struct sk_buff *skb2)
{
	#if defined(TCSUPPORT_BRIDGE_FASTPATH)
	/*MARK sc_mac_learned flag, the flooding packets can't be learned into fastbridge table*/
	skb->sc_mac_learned |=FB_FLOOD_PKT;
	#endif
#if defined(TCSUPPORT_CT)
	br_pb_flood(br, skb, skb2, __br_forward);
#else
	br_pb_flood(br, p, skb, skb2, __br_forward);
#endif
}
static void br_multicast_pb_flood(struct net_bridge_mdb_entry *mdst,
#if !defined(TCSUPPORT_CT)
 				 struct net_bridge_port *pIN,
#endif
			       struct sk_buff *skb, struct sk_buff *skb0,
			       void (*__packet_hook)(
					const struct net_bridge_port *p,
					struct sk_buff *skb))
{
	struct net_device *dev = BR_INPUT_SKB_CB(skb)->brdev;
	struct net_bridge *br = netdev_priv(dev);
	struct net_bridge_port *prev = NULL;
	struct net_bridge_port_group *p = NULL;
	struct hlist_node *rp = NULL;
	struct net_device *indev = NULL;
	struct net_device *outdev = NULL;
#ifdef TCSUPPORT_IGMPSNOOPING_ENHANCE
	struct net_bridge_port *preport = NULL;
	int ret = 0;
	int check = 0;
	int num_record = 0;
#endif	

#if defined(TCSUPPORT_XPON_IGMP) && defined(TCSUPPORT_MULTICAST_SPEED)
    int index = -1;
    index = igmp_hwnat_flow_index(skb);
#endif

	rp = rcu_dereference(br->router_list.first);
	p = mdst ? rcu_dereference(mdst->ports) : NULL;
	while (p || rp) {
		struct net_bridge_port *port = NULL, *lport = NULL, *rport = NULL;

		lport = p ? p->port : NULL;
		rport = rp ? hlist_entry(rp, struct net_bridge_port, rlist) :
			     NULL;

		port = (unsigned long)lport > (unsigned long)rport ?
		       lport : rport;
#ifdef TCSUPPORT_IGMPSNOOPING_ENHANCE
		num_record++;

		if(((ret=br_multicast_port_pass(p, port, skb)) == 1) 
				&& (port != preport)){
			preport = port;
#endif					

        prev = maybe_pb_deliver(prev, port, 
#if !defined(TCSUPPORT_CT)
                                pIN, 
#endif
#if defined(TCSUPPORT_XPON_IGMP) && defined(TCSUPPORT_MULTICAST_SPEED)
                                index,
#endif                                
                                skb, __packet_hook);


#ifdef TCSUPPORT_IGMPSNOOPING_ENHANCE	
		}else if(ret == 2){
			check++;
		}
#endif				
		if (IS_ERR(prev))
			goto out;

		if ((unsigned long)lport >= (unsigned long)port)
			p = rcu_dereference(p->next);
		if ((unsigned long)rport >= (unsigned long)port)
			rp = rcu_dereference(rp->next);
	}

	if (!prev)
		goto out;

	indev = outdev = NULL;
#if defined(TCSUPPORT_CT)	
	indev = skb->orig_dev;
#else
	if (pIN) {
		indev = pIN->dev;
	}
#endif
	if (prev) {
		outdev = prev->dev;
	}
	
	if ( (prev != NULL) && 
		((indev == NULL) ||
		(outdev == NULL) ||
		(portbind_check_hook == NULL) || 
		((portbind_check_hook) && (portbind_check_hook(indev->name, outdev->name)))) ) {
#ifdef TCSUPPORT_IGMPSNOOPING_ENHANCE
		/*Information can reference ' br_multicast_flood' */
		if((check!=0) && (check == num_record)){
			deliver_clone(prev, skb, __packet_hook);
			goto out;
		}
#endif

		if (skb0)
			deliver_clone(prev, skb, __packet_hook);
		else
			__packet_hook(prev, skb);

		return;
	}

out:
	if (!skb0)
		kfree_skb(skb);
}

void br_multicast_pb_forward(struct net_bridge_mdb_entry *mdst,
#if !defined(TCSUPPORT_CT)
struct net_bridge_port *p,
#endif
			  struct sk_buff *skb, struct sk_buff *skb2)
{
#if defined(TCSUPPORT_CT)
	br_multicast_pb_flood(mdst,skb, skb2, __br_forward);
#else
	br_multicast_pb_flood(mdst, p, skb, skb2, __br_forward);
#endif
}

#endif
#endif


