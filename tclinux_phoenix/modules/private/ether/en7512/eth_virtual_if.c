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
THAT IT IS RECEIVER??S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVER??S SPECIFICATION OR CONFORMING TO A PARTICULAR 
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

#include "eth_lan.h"
#include "eth_virtual_if.h"
#include "./tcphy/phy_api.h"

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
extern macAdapter_t *mac_p;
extern char lan_port_map[];
extern uint8 def_mac_addr[];
extern struct net_device *lan_dev;
extern uint8 use_ext_switch;
extern char *sep_itf;
extern int itf_start_idx;
extern int itf_num;
extern int vport_enable;

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
#if 0
static int virtual_lan_dev_start(struct net_device *dev);
static int virtual_lan_dev_close(struct net_device *dev);
static int virtual_lan_dev_open(struct net_device *dev);
static struct net_device_stats *virtual_lan_dev_stats(struct net_device *dev);
#endif
int virtual_lan_dev_start(struct net_device *dev);
int virtual_lan_dev_close(struct net_device *dev);
int virtual_lan_dev_open(struct net_device *dev);
struct net_device_stats *virtual_lan_dev_stats(struct net_device *dev);


static int virtual_dev_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd);
static int virtual_dev_set_macaddr(struct net_device *dev, void *p);

/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/
struct net_device *lan_virtual_dev[MAX_ETH_ITF_NUM] = {NULL};
macAdapter_t *mac_virtual_p[MAX_ETH_ITF_NUM] = {NULL};

#ifdef TCSUPPORT_AUTOBENCH
extern int eth_mac_tx_autobench(struct sk_buff *skb, struct net_device *dev);
#endif

#if KERNEL_2_6_36
static const struct net_device_ops virtual_dev_ops = {
    .ndo_init               = virtual_lan_dev_start,
    .ndo_open               = virtual_lan_dev_open,
    .ndo_stop               = virtual_lan_dev_close,
    .ndo_start_xmit         = eth_lan_mac_tx,
    .ndo_get_stats          = virtual_lan_dev_stats,
/*    .ndo_set_multicast_list = tc3262_gmac_set_multicast_list,*/
    .ndo_do_ioctl           = virtual_dev_ioctl,
    .ndo_set_mac_address    = virtual_dev_set_macaddr,
    .ndo_validate_addr      = eth_validate_addr,
#ifdef CONFIG_NET_POLL_CONTROLLER
    .ndo_poll_controller    = tc3262_gmac_poll_controller,
#endif
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

int virtual_lan_dev_start(struct net_device *dev)
{
    int index=0;
    int i;

    for( ; index < MAX_ETH_ITF_NUM ; index++)
    {
        if(dev == lan_virtual_dev[index])
            break;
    }
    if(index == MAX_ETH_ITF_NUM)
	{
		printk(KERN_INFO "%s(): find no stored dev!\n", __FUNCTION__);
		return -ENETDOWN;
	}
    for (i = 0; i < 6; i++) {
		dev->dev_addr[i] = def_mac_addr[i];
	}
#if KERNEL_2_6_36
		mac_virtual_p[index]->dev = dev;
#else
		dev->get_stats			= virtual_lan_dev_stats;
		dev->hard_start_xmit	= eth_lan_mac_tx;
		dev->open				= virtual_lan_dev_open;
		dev->stop				= virtual_lan_dev_close;
		dev->set_multicast_list = tc3262_gmac_set_multicast_list;
		dev->do_ioctl			= virtual_dev_ioctl;
		dev->set_mac_address	= virtual_dev_set_macaddr;
#ifdef TC3262_GMAC_NAPI
		dev->poll				= tc3262_gmac_poll;
#endif 
		dev->weight 			= MAC_NAPI_WEIGHT;	
#if VLAN_TAG_USED
		dev->features |= NETIF_F_HW_VLAN_TX;
#endif
#ifdef RAETH_CHECKSUM_OFFLOAD
		dev->features |= NETIF_F_IP_CSUM;
#endif		
#endif //KERNEL_2_6_36
		return 0;
}

int virtual_lan_dev_open(struct net_device *dev)
{
    netif_start_queue(dev);
    return 0;
}



int virtual_lan_dev_close(struct net_device *dev)
{
    netif_stop_queue(dev);
    return 0;
}

unsigned int find_port_by_name(struct net_device *dev)
{
    unsigned int eth_intf = 0;
    uint8 itf_name_length = strlen(sep_itf);
    if ( memcmp(dev->name, sep_itf, itf_name_length) == 0 ){
        switch(itf_name_length){
            case 3:
                sscanf(dev->name, "eth%d", &eth_intf);
                break;
            case 5:
                sscanf(dev->name, "eth0.%d", &eth_intf);
                break;
        }
        return eth_intf - itf_start_idx;
	}else{
	    return eth_intf;
	}
}

struct net_device *find_virtual_itf_by_index(unsigned int interface_index)
{
	struct net_device *dev ;

	if((SEP_ETH_ITF_0 <= interface_index) && (interface_index <= SEP_ETH_ITF_3))
	{
		dev = lan_virtual_dev[interface_index];
		return dev ; 
	}
	
	return NULL ;
}

struct net_device_stats *virtual_lan_dev_stats(struct net_device *dev)
{
    unsigned int port_id=0;
    unsigned int eth_intf = 0;
    struct net_device_stats *stats;
    stats = &mac_p->stats;
    
    eth_intf = find_port_by_name(dev);
	
	if(eth_intf < itf_num)
	{
	    port_id  =  lan_port_map[eth_intf];
	}else{
	    port_id = 6;
	}
    
    if(use_ext_switch)
	{
		stats->rx_packets = gswPbusRead(GSW_RX_UNIC(port_id)-GSW_BASE) + 
		                    gswPbusRead(GSW_RX_MULC(port_id)-GSW_BASE);
		stats->tx_packets = gswPbusRead(GSW_TX_UNIC(port_id)-GSW_BASE) + 
		                    gswPbusRead(GSW_RX_MULC(6)-GSW_BASE);
		stats->rx_bytes = gswPbusRead(GSW_RX_OCL(port_id)-GSW_BASE);
		stats->tx_bytes = gswPbusRead(GSW_TX_OCL(port_id)-GSW_BASE);
		stats->rx_dropped = gswPbusRead(GSW_RX_DROC(port_id)-GSW_BASE);
		stats->tx_dropped = gswPbusRead(GSW_TX_DROC(port_id)-GSW_BASE);
		stats->multicast = gswPbusRead(GSW_RX_MULC(port_id)-GSW_BASE);
		stats->rx_errors =  gswPbusRead(GSW_RX_ALIGE(port_id)-GSW_BASE) + 
		                    gswPbusRead(GSW_RX_CRC(port_id)-GSW_BASE) + 
		                    gswPbusRead(GSW_RX_RUNT(port_id)-GSW_BASE) + 
		                    gswPbusRead(GSW_RX_FRGE(port_id)-GSW_BASE) + 
		                    gswPbusRead(GSW_RX_LONG(port_id)-GSW_BASE);
		                    
		stats->tx_errors = gswPbusRead(GSW_TX_COLC(port_id)-GSW_BASE);
		stats->collisions = gswPbusRead(GSW_TX_COLC(port_id)-GSW_BASE);	
	}
	else	
	{
		stats->rx_packets = read_reg_word(GSW_RX_UNIC(port_id)) + 
		                    read_reg_word(GSW_RX_MULC(port_id));
		stats->tx_packets = read_reg_word(GSW_TX_UNIC(port_id)) + 
		                    read_reg_word(GSW_RX_MULC(6));
		stats->rx_bytes = read_reg_word(GSW_RX_OCL(port_id));
		stats->tx_bytes = read_reg_word(GSW_TX_OCL(port_id));
		stats->rx_dropped = read_reg_word(GSW_RX_DROC(port_id));
		stats->tx_dropped = read_reg_word(GSW_TX_DROC(port_id));
		stats->multicast = read_reg_word(GSW_RX_MULC(port_id));
		stats->rx_errors = read_reg_word(GSW_RX_ALIGE(port_id)) + 
		                    read_reg_word(GSW_RX_CRC(port_id)) + 
		                    read_reg_word(GSW_RX_RUNT(port_id)) + 
		                    read_reg_word(GSW_RX_FRGE(port_id)) + 
		                    read_reg_word(GSW_RX_LONG(port_id));
		                    
		stats->tx_errors = read_reg_word(GSW_TX_COLC(port_id));
		stats->collisions = read_reg_word(GSW_TX_COLC(port_id));
	}
	return stats;
}

static int virtual_dev_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	return generic_mii_ioctl(&mac_p->mii_if, if_mii(ifr), cmd, NULL);
}

static int virtual_dev_set_macaddr(struct net_device *dev, void *p)
{
	struct sockaddr *addr = p;

	/* Check if given address is valid ethernet MAC address */
  	if (!is_valid_ether_addr(addr->sa_data))
    		return(-EIO);

	/* Save the customize mac address */
  	memcpy(dev->dev_addr, addr->sa_data, dev->addr_len);

	return 0; /* Do nothing */
}

int init_virtual_interface(void)
{
    struct net_device *dev = NULL;
    int err, index = 0;
    char dev_name[10];

    for	(;index < itf_num; index++){
        /* virtual dev start from eth0.1 to eth0.4*/
        memset(dev_name, 0, sizeof(dev_name));
        sprintf(dev_name, "%s%d", sep_itf, itf_start_idx + index);
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,18,19)      
        dev = alloc_netdev(sizeof(macAdapter_t), dev_name, NET_NAME_USER, ether_setup);
	#else
		dev = alloc_netdev(sizeof(macAdapter_t), dev_name, ether_setup);
	#endif
        if (!dev)
    	    return -ENOMEM;
        lan_virtual_dev[index] = dev;
        mac_virtual_p[index] = netdev_priv(dev);

#if KERNEL_2_6_36
		dev->netdev_ops = &virtual_dev_ops; 
		mac_virtual_p[index]->napi.weight = MAC_NAPI_WEIGHT;	
		netif_napi_add(dev, &mac_virtual_p[index]->napi, 
		    NULL, MAC_NAPI_WEIGHT);
#else
		dev->init = tc3262_gmac_start;
#endif
        err = register_netdev(dev);
        if (err)
		{
			printk(KERN_INFO "%s(): register netdev fail! itf=%d\n",
			    __FUNCTION__, index);
			return err;
		}
    }
    return 0;
}

void exit_virtual_interface(void)
{
	int itf = 0;

	for(itf = 0; itf < MAX_ETH_ITF_NUM; itf++)
	{
		unregister_netdev(lan_virtual_dev[itf]);
	}
}

__IMEM int devFromWhichVirtualEthItf(struct net_device *dev)
{
	int itf = 0;
	for(; itf < MAX_ETH_ITF_NUM; itf++)
	{
		if(dev == lan_virtual_dev[itf])
			return itf;
	}
	//printk("found no proper seperated Ether interface!\r\n");
	return 0;
}

inline void setSkbFromWhichVirtualEthItf(struct sk_buff *skb, int itf)
{
	switch(itf)
	{
		case SEP_ETH_ITF_0:			
			*(unsigned long int *)(skb->cb) = CB_MBGIC_ETH_0;
			break;
		case SEP_ETH_ITF_1:			
			*(unsigned long int *)(skb->cb) = CB_MBGIC_ETH_1;
			break;
		case SEP_ETH_ITF_2:			
			*(unsigned long int *)(skb->cb) = CB_MBGIC_ETH_2;
			break;
		case SEP_ETH_ITF_3:			
			*(unsigned long int *)(skb->cb) = CB_MBGIC_ETH_3;
			break;
		default:
			*(unsigned long int *)(skb->cb) = CB_MBGIC_ETH_NONE;
			break;
	}	
	return;	
}

inline int skbFromWhichVirtualEthItf(struct sk_buff *skb)
{

    switch(*(unsigned long int *)(skb->cb))
    {
        case CB_MBGIC_ETH_0:
            return SEP_ETH_ITF_0;
            
        case CB_MBGIC_ETH_1:
            return SEP_ETH_ITF_1;
            
        case CB_MBGIC_ETH_2:
            return SEP_ETH_ITF_2;
            
        case CB_MBGIC_ETH_3:
            return SEP_ETH_ITF_3;
            
        default:
            /*printk("found no proper SKB MAGIC Ethernet itf info!\r\n");*/
            return SEP_ETH_ITF_UNKNOWN; 
    }
    return 0;
}




inline void virtual_itf_tx_hook(struct sk_buff *skb, struct net_device *dev)
{
    setSkbFromWhichVirtualEthItf(skb, devFromWhichVirtualEthItf(dev));
}

inline void virtual_itf_rx_hook(struct sk_buff *skb)
{
    int interface_index;
    if(vport_enable == DISABLE){
        return ;
    }
    interface_index = skbFromWhichVirtualEthItf(skb);
    if((SEP_ETH_ITF_0 <= interface_index) && (interface_index <= SEP_ETH_ITF_3)){
        skb->dev = lan_virtual_dev[interface_index];
    }
}


