/***************************************************************
Copyright Statement:

This software/firmware and related documentation (��EcoNet Software��) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (��EcoNet��) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (��ECONET SOFTWARE��) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN ��AS IS�� 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVER��S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVER��S SPECIFICATION OR CONFORMING TO A PARTICULAR 
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

#include "eth_wan.h"

#ifdef LOOPBACK_SUPPORT
#include "fe_verify.h"
#endif
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
struct net_device *wan_dev = NULL;
macAdapter_t *mac_wan_p = NULL;
extern int wan_port_id;
extern uint8 def_mac_addr[];
extern int masko_on_off;
extern int max_pkt_len;
extern uint8 macSTagFlag;
extern macAdapter_t *mac_p;
#ifdef LOOPBACK_SUPPORT
extern uint32 macLoopback;
#endif
extern char *wan_itf;

extern int g_force_dstq;
extern portInfo lanPortState[LIMIT_PORT_NUM];

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
static int gmac_wan_start(struct net_device *dev);
static struct net_device_stats *gmac_wan_stats(struct net_device *dev);
static int gmac_wan_open(struct net_device *dev);
static int gmac_wan_close(struct net_device *dev);
static void gmac_wan_set_multicast_list(struct net_device *dev);
static int gmac_wan_set_macaddr(struct net_device *dev, void *p);
static int gmac_wan_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd);
int gmac_wan_tx(struct sk_buff *skb, struct net_device *dev);
static int get_wan_port_stat(char *buf);
extern int eth1_stats_read_proc(char *buf, char **start, 
    off_t off, int count,int *eof, void *data);
extern int eth1_stats_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data);
extern void wan_to_lan(struct sk_buff *skb, uint8 local);
/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/
#if defined(TCSUPPORT_FH_ENV) && defined(TCSUPPORT_CPU_EN7521) && defined(TCSUPPORT_WAN_ETHER)
extern int (*fh_ewan_vlan_hook)(struct sk_buff* skb, int type);
#endif

#ifdef TCSUPPORT_WAN_ETHER
static const struct net_device_ops gmac_wan_netdev_ops = {
    .ndo_init               = gmac_wan_start,
    .ndo_open               = gmac_wan_open,
    .ndo_stop               = gmac_wan_close,
    .ndo_start_xmit         = gmac_wan_tx,
    .ndo_get_stats          = gmac_wan_stats,
    .ndo_set_multicast_list = gmac_wan_set_multicast_list,
    .ndo_do_ioctl           = gmac_wan_ioctl,
    .ndo_change_mtu         = eth_change_mtu,
    .ndo_set_mac_address    = gmac_wan_set_macaddr,
};
#endif

/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

#if defined(TCSUPPORT_WAN_ETHER)

static int get_wan_port_stat(char *buf){
	uint16 index = 0;
	uint32 reg;
	int port;
	int speed; 

	port = wan_port_id;
	
	reg = switch_reg_read(GSW_PMSR(port));

	if (!(reg & MAC_LINK_STS)) {
		index += sprintf(buf+index, "Down\n");
		return index;
	}

	speed = (reg & MAC_SPD_STS) >> MAC_SPD_STS_SHIFT;
	if (speed == PN_SPEED_1000M)
		index += sprintf(buf+index, "1000M/");
	else if (speed == PN_SPEED_100M)
		index += sprintf(buf+index, "100M/");
	else 
		index += sprintf(buf+index, "10M/");

	if (reg & MAC_DPX_STS)
		index += sprintf(buf+index, "Full Duplex");
	else 
		index += sprintf(buf+index, "Half Duplex");

	if (reg & (TX_FC_STS | RX_FC_STS)) {
		index += sprintf(buf+index, " FC:");
		if (reg & TX_FC_STS) 
			index += sprintf(buf+index, " TX");
		if (reg & RX_FC_STS) 
			index += sprintf(buf+index, " RX");
	}

	if (reg & EEE100_STS) 
		index += sprintf(buf+index, " EEE100");
	if (reg & EEE1G_STS) 
		index += sprintf(buf+index, " EEE1G");

	index += sprintf(buf+index, "\n");
	
	return index;
}

static int eth_wan_link_st_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = get_wan_port_stat(buf);
	if (len <= off+count) 
		*eof = 1;
	*start = buf + off;
	len -= off;
	if (len>count) 
		len = count;
	if (len<0) 
		len = 0;
	return len;
}


static int gmac_wan_start(struct net_device *dev)
{
#if KERNEL_2_6_36
	mac_wan_p = netdev_priv(dev);
	mac_wan_p->dev = dev;
#else
  	/* Hook up with handlers */
  	dev->get_stats		= gmac_wan_stats;
  	dev->hard_start_xmit	= gmac_wan_tx;
  	dev->open		= gmac_wan_open;
  	dev->stop		= gmac_wan_close;
  	dev->set_multicast_list = gmac_wan_set_multicast_list;
  	dev->do_ioctl		= gmac_wan_ioctl;
  	dev->set_mac_address 	= gmac_wan_set_macaddr;

#if VLAN_TAG_USED
	dev->features |= NETIF_F_HW_VLAN_TX;
#endif

#ifdef RAETH_CHECKSUM_OFFLOAD
	dev->features |= NETIF_F_IP_CSUM;
#endif

	mac_wan_p->statisticOn = MAC_STATISTIC_ON;
#endif//endof kernel_2_6_36
	return 0;
}

int gmac_wan_tx(struct sk_buff *skb, struct net_device *dev)
{
    ethTxMsg_t ethTxMsg;
    int error,ringIdx;
	int length;
	uint32 txq = 0;
#if defined(TCSUPPORT_RA_HWNAT)	
	struct port_info eth_info;	
	int i, channel=5;
	unsigned long int port_mask;
#endif

	setComeFromWan(skb,1);
	
#ifdef CONFIG_8021P_REMARK
	skb=vlanPriRemark(skb);
	if(skb==NULL){
		printk("802.1p remark failure\r\n");
		return NETDEV_TX_OK;
	}
#endif

#ifdef WAN2LAN
    wan_to_lan(skb,0);
#endif

#if defined(TCSUPPORT_FH_ENV) && defined(TCSUPPORT_CPU_EN7521) && defined(TCSUPPORT_WAN_ETHER)
	if ( fh_ewan_vlan_hook )
		fh_ewan_vlan_hook(skb, FHEWAN_TX_HOOK);
#endif

    if(macSTagFlag){
        skb = macEN7512STagInsert(skb);
        if(skb == NULL) {
            mac_wan_p->macStat.MIB_II.outDiscards++;
            return NETDEV_TX_OK;
        }
    }

	/***************** Count the MIB-II *****************/
	update_sw_mib(mac_wan_p, skb, TX_MIB);
	
    /**************** prepare Tx Msg *****************/ 
	memset(&ethTxMsg, 0, sizeof(ethTxMsg_t));
    /* GDMA1 */
    ethTxMsg.raw.fPort = DPORT_GDMA1;

    /**************** prepare port_info *****************/ 
#ifdef TCSUPPORT_RA_HWNAT
	if (ra_sw_nat_hook_tx != NULL) {
		memset(&eth_info, 0, sizeof(struct port_info));
		eth_info.txq = (txq & 0xf);
		eth_info.eth_is_wan = 1;
        eth_info.eth_macSTagEn = macSTagFlag&0x1;
        eth_info.magic = FOE_MAGIC_GE;

		/* Assign Channel by port mask */
		port_mask = getPortMask(skb);
		for(i=0;i<LIMIT_PORT_NUM;i++){
			if( ((1<<i) & port_mask ) > 0 ){
				channel = i;
				break;
			}
		}
		if( port_mask & ~(1<<channel)){
			channel = 5;
		}
		eth_info.channel = channel;

		eth_info.ds_to_qdma = 0;
	}
#endif
#ifdef LOOPBACK_SUPPORT
	if (macLoopback & LOOPBACK_PKT) {
		printk("Wan TX: ");
		dump_skb(skb);
	}
#endif
    ringIdx = 0;
    error = QDMA_API_TRANSMIT_PACKETS(ECNT_QDMA_LAN, skb, 
        ethTxMsg.msg[0], ethTxMsg.msg[1], &eth_info);
    if(error){
		dev_kfree_skb_any(skb);
		mac_wan_p->macStat.MIB_II.outDiscards++;
		return NETDEV_TX_OK;
    }
    mac_wan_p->macStat.inSilicon.txEnQueueNum++;
	return NETDEV_TX_OK;
}




static struct net_device_stats *gmac_wan_stats(struct net_device *dev)
{
	struct net_device_stats *stats;

	stats = &mac_wan_p->stats;
	stats->rx_packets = read_reg_word(GSW_RX_UNIC(wan_port_id)) + 
	    read_reg_word(GSW_RX_MULC(wan_port_id));
	stats->tx_packets = read_reg_word(GSW_TX_UNIC(wan_port_id)) + 
	    read_reg_word(GSW_TX_MULC(wan_port_id));
	stats->rx_bytes = read_reg_word(GSW_RX_OCL(wan_port_id));
	stats->tx_bytes = read_reg_word(GSW_TX_OCL(wan_port_id));
	stats->rx_dropped = read_reg_word(GSW_RX_DROC(wan_port_id));
	stats->tx_dropped = read_reg_word(GSW_TX_DROC(wan_port_id));
	stats->multicast = read_reg_word(GSW_RX_MULC(wan_port_id));
	stats->rx_errors = read_reg_word(GSW_RX_ALIGE(wan_port_id)) + 
	    read_reg_word(GSW_RX_CRC(wan_port_id)) + 
	    read_reg_word(GSW_RX_RUNT(wan_port_id)) + 
	    read_reg_word(GSW_RX_FRGE(wan_port_id)) + 
	    read_reg_word(GSW_RX_LONG(wan_port_id));
	stats->tx_errors = read_reg_word(GSW_TX_COLC(wan_port_id));
	stats->collisions = read_reg_word(GSW_TX_COLC(wan_port_id));

	return stats;
}
/* Starting up the ethernet device */
static int gmac_wan_open(struct net_device *dev)
{
// 	printk("tc3262_gmac_wan_open\n");
	#if KERNEL_2_6_36
	napi_enable(&mac_wan_p->napi);
	#endif
	netif_start_queue(dev);
  	return 0; 
}
static int gmac_wan_close(struct net_device *dev)
{
// 	printk(KERN_INFO "tc3262_gmac_wan_close\n");
	#if KERNEL_2_6_36
	napi_disable(&mac_wan_p->napi);
	#endif

  	netif_stop_queue(dev);

  	return 0;
}
/* Setup multicast list */
static void gmac_wan_set_multicast_list(struct net_device *dev)
{
	return; /* Do nothing */
}

/* Setting customized mac address */
static int gmac_wan_set_macaddr(struct net_device *dev, void *p)
{
	struct sockaddr *addr = p;

	/* Check if given address is valid ethernet MAC address */
  	if (!is_valid_ether_addr(addr->sa_data))
    		return(-EIO);

	/* Save the customize mac address */
  	memcpy(dev->dev_addr, addr->sa_data, dev->addr_len);

	return 0; /* Do nothing */
}
/* Handling ioctl call */
static int gmac_wan_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{

	return generic_mii_ioctl(&mac_p->mii_if, if_mii(ifr), cmd, NULL);
}


int eth_wan_dev_register()
{
	struct net_device *dev;
	
    dev = alloc_netdev(sizeof(macAdapter_t), wan_itf, ether_setup);
    if (!dev)
    	return -ENOMEM;

	wan_dev = dev;
        
	mac_wan_p = netdev_priv(dev);
#if KERNEL_2_6_36
	/* Hook up with handlers */
	dev->netdev_ops = &gmac_wan_netdev_ops;

	mac_wan_p->napi.weight = MAC_NAPI_WEIGHT;
        
	netif_napi_add(dev, &mac_wan_p->napi, NULL/*dev->poll function*/, 
	    MAC_NAPI_WEIGHT);
#else
	dev->init = tc3262_gmac_wan_start;
#endif	
	return register_netdev(dev);
}
        
int eth_wan_init(void){
    struct proc_dir_entry *eth_proc;
	int err;
	
    err = eth_wan_dev_register();
	if (err){
		printk("register netdev fail\n");
		return err;
	}	
	create_proc_read_entry("tc3162/eth1_link_st", 0, NULL, eth_wan_link_st_proc, NULL);
	eth_proc = create_proc_entry("tc3162/eth1_stats", 0, NULL);
	eth_proc->read_proc = eth1_stats_read_proc;
	eth_proc->write_proc = eth1_stats_write_proc;

	return 0;
}

int eth_wan_exit(void)
{
	remove_proc_entry("tc3162/eth1_stats", 0);
	remove_proc_entry("tc3162/eth1_link_st", 0);
	unregister_netdev(wan_dev);
	free_netdev(wan_dev);
	
	return 0;
}
#endif


