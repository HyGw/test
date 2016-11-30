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
THAT IT IS RECEIVERS SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVERS SPECIFICATION OR CONFORMING TO A PARTICULAR 
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
#if defined(TCSUPPORT_WAN_ETHER)
#include "eth_wan.h"
#endif


#if defined(TCSUPPORT_SEPERATED_ETH_ITF) 
#include "eth_virtual_if.h"
#endif

#ifdef TCSUPPORT_AUTOBENCH
#include "../../auto_bench/autobench.h"
#endif

#include "./tcphy/tcconsole.h"
    
#include "eth_proc.h"
#include "fe_verify.h"
#ifdef TCSUPPORT_MT7530_SWITCH_API
#include "./tcphy/mtkswitch_api.h"
#include <modules/eth_global_def.h>
/*extern int macMT7530gswAPIDispatch(struct ifreq *ifr);*/
#endif

#ifdef LOOPBACK_SUPPORT
#include "fe_verify.h"
#endif
    
#include "./tcphy/phy_api.h"
#include "eth_api.h"
#include <ecnt_hook/ecnt_hook_traffic_classify.h>
#include <ecnt_hook/ecnt_hook_qdma.h>
#include <ecnt_hook/ecnt_hook_multicast.h>
#include <ecnt_hook/ecnt_hook_net.h>
#include <ecnt_hook/ecnt_hook_ppe.h>
#include <ecnt_hook/ecnt_hook_l2tp.h>

#if defined(TCSUPPORT_CT_JOYME_BANDWIDTH)
#include "devBandWidth.h"
#endif
#include <linux/libcompileoption.h>


/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
#ifdef __BIG_ENDIAN
#define FE_BYTE_SWAP
#endif
    
#ifndef TCSUPPORT_CPU_MT7520
#define RAETH_CHECKSUM_OFFLOAD
#endif
    
#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE)
#if defined(WAN2LAN) || defined(CONFIG_TC3162_ADSL)
#define VLAN_TAG_USED 0
#else
#define VLAN_TAG_USED 1
#endif
#else
#define VLAN_TAG_USED 0
#endif
    
#ifdef TCSUPPORT_MAX_PACKET_2000
#define RX_MAX_PKT_LEN 		2048
#else
#ifdef LOOPBACK_SUPPORT_JUMBO/* test 12k jumbo frame for GDMA1*/
#define RX_MAX_PKT_LEN 		15000
#else
#define RX_MAX_PKT_LEN 		1536
#endif
#endif
    
#define GEN_1588_PKT_7530_VERIFY	/*MTK20120829_MT7530_1588pkt_generation*/

/* ADMTEK6996M register */
#define ADM_PORT0_BASIC		0x01
#define ADM_PORT1_BASIC		0x03
#define ADM_PORT2_BASIC		0x05
#define ADM_PORT3_BASIC		0x07
#define ADM_PORT4_BASIC		0x08
#define ADM_PORT5_BASIC		0x09

#define ADM_CHIP_ID0		0xa0
#define ADM_CHIP_ID1		0xa1




#define CONFIG_8021P_REMARK 1
#if defined(TCSUPPORT_WAN_ETHER) && defined(CONFIG_8021P_REMARK)
#define QOS_8021p_MARK			0x0F00 	/* 8~11 bits used for 802.1p */
#define QOS_8021P_0_MARK		0x08	/* default mark is zero */
#define VLAN_HLEN				4
#define VLAN_ETH_ALEN			6
#endif

#define     VLAN_REMARKING      0x0FFF00
#define     PRI_STORE     0xE


#define INITIALIZED 1
#define UNINIT 0

#define GE_PORT		4			/*for 7521F,switch GE-port num*/

#define MAC_SYNC_TIMEUP		12	/*one step for 250ms, 12 means 3s*/

/************************************************************************
*                  M A C R O S
*************************************************************************
*/

#define RxMsgW1 (curRxMsg->rxMsgW1.raw)

/************************************************************************
*                  D A T A   T Y P E S
*************************************************************************
*/

/************************************************************************
*                  E X T E R N A L   D A T A   D E C L A R A T I O N S
*************************************************************************
*/
    
extern int wan_port_id;
extern uint32 macLoopback;
extern uint32 macRxLoopback;
extern int macTest_channel;
extern int macTest_queue;
extern int macTest_vid;
extern int macTest_insert_vid;

/* frankliao added 20101215 */
extern unsigned long flash_base;
extern unsigned int (*ranand_read_byte)(unsigned long long);
/*MTK20120829_MT7530_1588pkt_generation*/
extern void getnstimeofday(struct timespec *tv);    
extern void tcephydbgcmd(void);
extern int macSetUpPhy(macAdapter_t *mac_p);
extern int mt7530_switch_init(void);
extern int32 macPhyLookUp(macAdapter_t *mac_p, uint32 companyId);
extern void macSetGSW(macAdapter_t *mac_p);



#if defined(TCSUPPORT_WAN_ETHER)
extern int gmac_wan_init(void);
extern int gmac_wan_exit(void);
extern struct net_device *wan_dev;
extern macAdapter_t *mac_wan_p;
#endif                                        

extern uint8 use_ext_switch;


#if defined(TCSUPPORT_WAN2LAN_EXT)
extern uint32 (*gswPbusRead_hook)(uint32 pbus_addr);
extern int (*gswPbusWrite_hook)(uint32 pbus_addr, uint32 pbus_data);
extern int (*lanPortmap_hook)(int lanPort);
extern int masko_ext;
#endif
extern char lan_port_map[ RT63365_SWIC_PORTNUM ];
extern char switch_port_map[ RT63365_SWIC_PORTNUM ];

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
************************************************************************/
/* netdev ops eth0*/
static int eth_lan_mac_start(struct net_device *dev);
static int eth_lan_mac_open(struct net_device *dev);
static int eth_lan_mac_close(struct net_device *dev);
static int eth_timer_init(void);
static struct net_device_stats *eth_lan_mac_stats(struct net_device *dev);
int eth_lan_mac_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd);
int my_eth_change_mtu(struct net_device *dev, int new_mtu);
static int eth_lan_mac_set_macaddr(struct net_device *dev, void *p);

ecnt_ret_val ecnt_ether_hook(struct ecnt_data *indata);

void gswLanLinkSt_monitor(void);
void perChannelRateLimitSwitch(void);


/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************/
#if defined(TCSUPPORT_FH_ENV) && defined(TCSUPPORT_CPU_EN7521) && defined(TCSUPPORT_WAN_ETHER)
int (*fh_ewan_vlan_hook)(struct sk_buff* skb, int type);
EXPORT_SYMBOL(fh_ewan_vlan_hook);
#endif


#if defined(TCSUPPORT_CT_LOOPDETECT)
    int (*check_rcv_hook)(struct sk_buff *skb) = NULL;
     EXPORT_SYMBOL(check_rcv_hook);
#endif



uint16 lan_port_tpid[MT7530_SWIC_PORTNUM];
uint16 ether_wan_tpid = DEFAULT_TPID;
uint16 cpu_port_tpid = DEFAULT_TPID;


/* 0:disable, 1: force to gdma, 2; force to qdma */
int g_force_dstq;
uint32 dsoffload_presetting = 0; 
EXPORT_SYMBOL(dsoffload_presetting);
portInfo lanPortState[LIMIT_PORT_NUM];
EXPORT_SYMBOL(lanPortState);
#ifdef TCSUPPORT_ETHER_DOWNVLAN
    void (*eth_down_vlan_hook)(struct sk_buff *skb) = NULL;
    EXPORT_SYMBOL(eth_down_vlan_hook);
#endif
    static int max_pkt_len = 2000;

#ifdef TCSUPPORT_AUTOBENCH
int eth_mac_tx_autobench(struct sk_buff *skb, struct net_device *dev)
{
	if (skb != NULL){
		dev_kfree_skb_any(skb);
	}
	return NETDEV_TX_OK;
}
#endif

#if KERNEL_2_6_36
static const struct net_device_ops gmac_netdev_ops = {
    .ndo_init               = eth_lan_mac_start,
    .ndo_open               = eth_lan_mac_open,
    .ndo_stop               = eth_lan_mac_close,
    .ndo_start_xmit         = eth_lan_mac_tx,
    .ndo_get_stats          = eth_lan_mac_stats,
/*    .ndo_set_multicast_list = tc3262_gmac_set_multicast_list,*/
    .ndo_do_ioctl           = eth_lan_mac_ioctl,
#ifdef TCSUPPORT_MAX_PACKET_2000
    .ndo_change_mtu         = my_eth_change_mtu,
#else
    .ndo_change_mtu         = eth_change_mtu,
#endif
    .ndo_set_mac_address    = eth_lan_mac_set_macaddr,
    .ndo_validate_addr      = eth_validate_addr,
#ifdef CONFIG_NET_POLL_CONTROLLER
    .ndo_poll_controller    = tc3262_gmac_poll_controller,
#endif
};

#endif /*KERNEL_2_6_36*/

#if defined(TCSUPPORT_CPU_MT7520)
int mac_receive_num = 0; //0 means receive packet and no drop
int mac_receive_threshold = MAC_RXDESCP_NO; //MAC_RXDESCP_NO_DEFAULT; //0 means receive packet and no drop
#else
int mac_receive_num = MAC_RECV_THLD; //0 means receive packet and no drop
int mac_receive_threshold = MAC_RXDESCP_NO_DEFAULT; //0 means receive packet and no drop
#endif

struct ecnt_hook_ops ecnt_driver_ether_op = {
    .name = "driver_ether_hook",
    .hookfn = ecnt_ether_hook,
    .maintype = ECNT_ETHER_SWITCH,
    .is_execute = 1,
    .subtype = ECNT_DRIVER_API,
    .priority = 1
};

uint8 interface_map[TRAFFIC_TYPE_MAX] = {0};
/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/
/* Device data */
struct net_device *lan_dev;
macAdapter_t *mac_p = NULL;

/* Timer */
static struct timer_list eth_timer;
static struct timer_list eth_poll_timer;

/* phy lock */
spinlock_t phy_lock;
static macPhyLinkProfile_t enetPhyLinkProfile;
uint8 macInitialized = 0;    
uint8 def_mac_addr[] = {0x00, 0x00, 0xaa, 0xbb, 0xcc, 0xff};
uint8 swicVendor = 0;
static DEFINE_SPINLOCK(eth_napilock);
#ifdef WAN2LAN
/* ------ xyzhu_091105:special tag relation data start ---------- */
extern uint8 macSTagFlag;
/* ------ xyzhu_091105:special tag relation data end ----------- */
extern int masko_on_off;
#endif
int ethmac_qdmaCfg=UNINIT;

/*for macTable sync to Kernel , bugfix for L2B DMAC+VLAN mode*/
unsigned char mac_sync_en = 0 ;
unsigned char mac_sync_timer_count = 0 ;
unsigned char mac_sync_flag = 0 ;

/* ------------------ module param ------------------ */
char *wan_itf  = "nas10";
char *lan_itf  = "eth0";
char *sep_itf  = "eth0.";
int itf_start_idx =1;
int itf_num =4;

/* ------------------ ECNT_Hook ------------------ */
typedef int (*ether_api_op_t)(struct ECNT_ETHER_Data *ephy_data);

/* Warning: same sequence with enum 'ETHER_HookFunction_t' */
ether_api_op_t
ether_operation[]=
{
    eth_mdio_read,
    eth_mdio_write,
    eth_mac_send,
    eth_gsw_pbus_read,	/* ETHER_FUNCTION_GSW_PBUS_READ */
    eth_gsw_pbus_write,	/* ETHER_FUNCTION_GSW_PBUS_WRITE */
    eth_ext_gsw_pbus_read,
    eth_ext_gsw_pbus_write,
    eth_set_ratelimit_switch,
    eth_set_mactable_sync_en,
    rgmii_traffic_set,
    rgmii_mode,
};

spinlock_t hookFuncLock[ETHER_FUNCTION_MAX_NUM];

uint8 hal_api_ext=0;
/*TCSUPPORT_FWC_ENV start*/
uint8 ratelimit_En = 0 ;/*1: downstream total bandwidth ratelimit works; 0: not work*/
uint8 ge_portlimit_En = 0 ;/*GE port, set per-port limit, 1:enable ; 0 :disable*/
/*TCSUPPORT_FWC_ENV endt*/
/* ----------------------------------------------- */

/* ------------------ ECNT_Hook for Kernel ------------------ */
ecnt_ret_val ecnt_eth_updateKernelMacTable(struct ecnt_data *in_data)
{
	if( ENABLE == mac_sync_en )
	{
		mac_sync_flag = ENABLE ;
		return ECNT_CONTINUE;
	}

	return ECNT_CONTINUE;
}

struct ecnt_hook_ops ecnt_eth_updateKernelMacTable_op = {
	.name = "ecnt_delDevBandWidthMac",
	.is_execute = 1,
	.hookfn = ecnt_eth_updateKernelMacTable,
	.maintype = ECNT_NET_BR_FDB,
	.subtype = ECNT_BR_FDB_GET,
	.priority = 1
};
/* ----------------------------------------------- */

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
ecnt_ret_val ecnt_ether_hook(struct ecnt_data *indata)
{
	struct ECNT_ETHER_Data *ether_data = (struct ECNT_ETHER_Data *)indata ;
	ulong flags=0 ;	
	
	if(ether_data->function_id >= ETHER_FUNCTION_MAX_NUM) {
		printk("ether_data->function_id is %d, exceed max number: %d", 
		    ether_data->function_id, ETHER_FUNCTION_MAX_NUM);
		return ECNT_HOOK_ERROR;
	}
	ether_data->retValue = ether_operation[ether_data->function_id](ether_data) ;
	
	return ECNT_CONTINUE;
}


/************************************************************************
    Funtcion:       dump_skb
    Description:    
    Calls:
    Called by:      for debug
    Input:
    Output:
    Return:
    Others:     
************************************************************************/
void dump_skb(struct sk_buff *skb)
{
	char tmp[80];
	char *p = skb->data;
	char *t = tmp;
	int i, n = 0;

	printk("ERR skb=%08lx skb->head=%08lx \n data=%08lx len=%d mark=%08lX\n cb=%08lx \n", 
	    (uint32) skb, (uint32) skb->head, (uint32) skb->data, skb->len, skb->mark, 
	    *(unsigned long int *)(skb->cb));
    printk("skb dev %s, orig dev %s\n", skb->dev, skb->original_dev);
	for (i = 0; i < skb->len; i++) {
		t += sprintf(t, "%02x ", *p++ & 0xff);
		if ((i & 0x0f) == 0x0f) {
			printk("%04x: %s\n", n, tmp);
			n += 16;
			t = tmp;
		}
	}
	if (i & 0x0f)
		printk("%04x: %s\n", n, tmp);
}
EXPORT_SYMBOL(dump_skb);

/************************************************************************
    Funtcion:       merge_switchtable2kerneltable
    Description:    merge switch mactable to kernel mac table
    Calls:          
    Called by:      eth_monitor
    Input:
    Output:
    Return:
    Others:     
************************************************************************/
void merge_switchtable2kerneltable(void)
{
	mt7530_switch_brtable *pBrtable = NULL ;
	int index = 0 ;
	unsigned int portID = 0 ;
	unsigned int lanID = 0 ;
	struct net_device *dev = NULL ;
	struct net_bridge_port *p = NULL ;
	struct net_bridge *br = NULL ;

	/*init the space*/
	pBrtable = (mt7530_switch_brtable *)kmalloc(sizeof(mt7530_switch_brtable), GFP_KERNEL);
	if ( !pBrtable )
	{
		return ;
	}
	pBrtable->count = 0;

	/*read the switch mac table*/
	macMT7530GetAllBRTable(pBrtable);

	/*merge the switch mac table to Kernel mac table*/
	for( index = 0 ; index < pBrtable->count ; index++ )
	{
		portID = pBrtable->macinfo[index].port ;
		lanID = macMT7530SwitchMap2LanPort( portID ) ;
#if defined(TCSUPPORT_SEPERATED_ETH_ITF) 
		/******** virtual interface eth0.1~4 assign to dev ********/
		dev = find_virtual_itf_by_index(lanID) ;
#endif
		if(dev)
		{
			p = br_port_get_rcu(dev) ;
			if( (!p) || (p->state == BR_STATE_DISABLED) )
			{
				continue ;
			}
			br = p->br ;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,18,19)	
			if( p->flags & BR_LEARNING )
				br_fdb_update(br, p , pBrtable->macinfo[index].mac , 0 , false);
#else
			br_fdb_update(br, p , pBrtable->macinfo[index].mac , NULL);
#endif
		}
	}

    kfree(pBrtable);
	return ;
}
					
/************************************************************************
    Funtcion:       eth_monitor
    Description:    auto polling eth phy link status
    Calls:          
    Called by:      eth_timer_init
    Input:
    Output:
    Return:
    Others:     ifconfig down
************************************************************************/

/* Monitor the status of link, re-do link initialization if necessary. */
static void eth_monitor(unsigned long data)
{
	uint32 bmsr;
	uint32 bmcr;
	uint32 loopback = 0;
	uint32 advert, lpa;
	uint32 advert2, lpa2;
	uint32 enetMode;
#if defined(TCSUPPORT_NEW_SPIFLASH_DEBUG)
	static uint32 cnt_threshold=0;
	extern uint32 spi_task2_en;
#endif

	if (mac_p == NULL)
		return;
	
#if defined(TCSUPPORT_NEW_SPIFLASH_DEBUG)
	if(spi_task2_en==1) {
		cnt_threshold++;
		if(cnt_threshold >= 100) {
			sf_task2_test();
			cnt_threshold = 100;
		}
	}
#endif

#ifdef TCPHY_SUPPORT
#ifndef MT7505_PQA_SWITCH_TEST
	periodChk();
#endif	
#endif	

#ifdef  TC_CONSOLE_ENABLE
    tcconsole_chk();
#endif

	/* save the original enet mode */
	enetMode = mac_p->macPhyLinkProfile_p->enetMode;

	if (swicVendor) {
		mac_p->macPhyLinkProfile_p->enetMode =  LAN_ST_100MB | LAN_ST_FULL_DUPLEX | LAN_ST_LINK_UP;
		mac_p->macPhyLinkProfile_p->enetMode &= ~LAN_ST_100MB;
		mac_p->macPhyLinkProfile_p->enetMode |= LAN_ST_1000MB;
	} else {
        // phy_api_loopback_get     phy_api_7512
        // phy_api_link_mode        phy_api_7512
        // phy_api_lpa              phy_api_7512
	    // phy_api_link_status      phy_api_7512
		mac_p->macPhyLinkProfile_p->enetMode = 0;
	}
    EPHY_API_GET_MONITOR();

	gswLanLinkSt_monitor();

	/*------merge switch mac table to kernel mac table-----*/
	if( mac_sync_timer_count < MAC_SYNC_TIMEUP )
	{
		mac_sync_timer_count++ ;
	}
	else if( ENABLE == mac_sync_flag )
	{
		merge_switchtable2kerneltable();
		mac_sync_flag = DISABLE ;
		mac_sync_timer_count = 0 ;
	}
	/*-----------------------------------------------*/
	
	/* Schedule for the next time */
	eth_timer.expires = jiffies + msecs_to_jiffies(250);
  	add_timer(&eth_timer);
}

#if defined(TCSUPPORT_WAN2LAN_EXT)
void wan2lan_ext_tx(struct sk_buff *skb)
{
    if(masko_ext & 0x20){
        if(!(skb->mark & SKBUF_COPYTOLAN)){
            dump_skb(skb);
        }
    }
}
#endif

inline int insert_special_tag(ethTxMsg_t *TxMsg, struct sk_buff **skb_p)
{
    if(unlikely(macSTagFlag == DISABLE)){
        return SUCCESS;
    }

#ifdef TCSUPPORT_ETHER_DOWNVLAN
    if(eth_down_vlan_hook)
        eth_down_vlan_hook(*skb_p);
#endif

	if(isEN7526c){
		return macEN7526cSTagInsert(TxMsg,skb_p);
	}
	else{
		(*skb_p) = INSERT_STAG(*skb_p);
		
		if(*skb_p == NULL)
			return FAIL;
		else
			return SUCCESS;
	}

}

/************************************************************************
    Funtcion:
    Description:    update sw mib both for  wan and lan
    Calls:
    Called by:      eth_rx, eth_mac_tx, gmac_wan_tx 
    Input:
    Output:
    Return:
    Others:     
************************************************************************/
__IMEM void update_sw_mib(macAdapter_t *mib_type,struct sk_buff *skb, int direction)
{
    if (mib_type->statisticOn) {
        switch(direction){
            case RX_MIB:
                mib_type->macStat.MIB_II.inOctets += (skb->len>64?skb->len:64);
                if (skb->data[0] == 0x01)
                    mib_type->macStat.MIB_II.inMulticastPkts++;
                else if(skb->data[0] == 0xFF)
                    mib_type->macStat.MIB_II.inBroadcastPkts++;
                else
                    mib_type->macStat.MIB_II.inUnicastPkts++;				
#if defined(TCSUPPORT_CT_JOYME_BANDWIDTH)
				checkMacBandWitdh(skb,DEV_BANDWIDTH_UP,skb->len>64?skb->len:64);
#endif
                break;
            case TX_MIB:
                mib_type->macStat.MIB_II.outOctets += (skb->len>64?skb->len:64);
            	if (skb->data[0] == 0x01)
            		mib_type->macStat.MIB_II.outMulticastPkts++;
                else if(skb->data[0] == 0xFF)
            		mib_type->macStat.MIB_II.outBroadcastPkts++;
            	else
            		mib_type->macStat.MIB_II.outUnicastPkts++;
#if defined(TCSUPPORT_CT_JOYME_BANDWIDTH)
				checkMacBandWitdh(skb,DEV_BANDWIDTH_DOWN,skb->len>64?skb->len:64);
#endif
                break;
        }
    }
}

#ifdef TCSUPPORT_AUTOBENCH
__IMEM int eth_mac_tx(struct sk_buff *skb, struct net_device *dev)
{
	if (skb != NULL){
		dev_kfree_skb_any(skb);
	}
	return NETDEV_TX_OK;
}
#endif

inline uint16 getStagBySkb(struct sk_buff *skb)
{	
	uint8 *cp=NULL;
	uint16 stag=0;

	cp = skb->data;
	stag = *(uint16 *)(cp+12);
	
	return stag;
}

inline int getChannelByPortmask(unsigned long int port_mask)
{
	int i, channel=1 ;
	for(i=0;i<LIMIT_PORT_NUM;i++)
	{
		if( ((1<<i) & port_mask ) > 0 )
		{
			channel = i;
			break;
		}
	}
	
#if 0
	if( port_mask & ~(1<<channel))
	{
		channel = 5;
	}
#endif

	return channel ;
}

/************************************************************************
    Funtcion:
    Description:    eth packet tx
    Calls:
    Called by:      gmac_netdev_ops
    Input:
    Output:
    Return:
    Others:     ifconfig down
************************************************************************/
#ifdef TCSUPPORT_AUTOBENCH
int tc3262_gmac_tx_autobench(struct sk_buff *skb, struct net_device *dev)
#else
__IMEM int eth_mac_tx(struct sk_buff *skb, struct net_device *dev)
#endif
{
    ethTxMsg_t ethTxMsg,ethTxMsg_temp; 
    int error,ringIdx;
	unsigned char  txq = 0, offload_packet = 0;
	struct port_info eth_info;
	struct port_info *qdma_info;
	unsigned long int port_mask;

#ifdef TCSUPPORT_RA_HWNAT
	if (ra_sw_nat_hook_magic) 
	{
		if (ra_sw_nat_hook_magic(skb, FOE_MAGIC_PPE)) 
		{
			/* PPE offload*/
			offload_packet = 1;
		}
	}
#endif

	if(offload_packet == 1)
	{
		/***************** Count the MIB-II *****************/
        	update_sw_mib(mac_p, skb, TX_MIB);
		/**************** prepare Tx Msg *****************/
        	memset(&ethTxMsg, 0, sizeof(ethTxMsg_t));
            	ethTxMsg.raw.fPort = DPORT_PPE;
		ethTxMsg.raw.channel = WIFI_OFFLOAD_CHANNEL;
		
		qdma_info = NULL;
	}
	else
	{
#if defined(TCSUPPORT_WAN2LAN_EXT)
		wan2lan_ext_tx(skb);
#endif
	/***************** virtual interface *****************/
#if defined(TCSUPPORT_SEPERATED_ETH_ITF)    
		virtual_itf_tx_hook(skb,dev);
#endif

#ifdef CONFIG_TC3162_ADSL
		isTCConsolePkt(skb);
#endif
		memset(&ethTxMsg_temp, 0, sizeof(ethTxMsg_t));
		if(unlikely(insert_special_tag(&ethTxMsg_temp,&skb) == FAIL))
		{
			mac_p->macStat.MIB_II.outDiscards++;
			return NETDEV_TX_OK;
		}

#ifndef LOOPBACK_SUPPORT_JUMBO
		if (skb->len > max_pkt_len)
		{
			printk("skb length more than the PSE port 0 buffer usage \
		    		skb->len:%d, max_pkt_len:%d\n", 
            		skb->len, max_pkt_len);
			dev_kfree_skb_any(skb);
			mac_p->macStat.MIB_II.outDiscards++;
			return NETDEV_TX_OK;
		}
#endif

		/***************** Count the MIB-II *****************/	
		update_sw_mib(mac_p, skb, TX_MIB);
        
		/**************** prepare Tx Msg *****************/ 
		memset(&ethTxMsg, 0, sizeof(ethTxMsg_t));

		ethTxMsg.raw.fPort = DPORT_GDMA1;  

#ifdef LOOPBACK_SUPPORT
		eth_loopback_tx_hook(&ethTxMsg,skb);
#endif

		/**************** prepare port_info *****************/ 
		memset(&eth_info, 0, sizeof(struct port_info));
		eth_info.txq = (txq & 0xf);
		/*eth_info.eth_is_wan = 0;*//*no need*/
		eth_info.eth_macSTagEn = macSTagFlag&0x1;
		eth_info.magic = FOE_MAGIC_GE;
		eth_info.ds_to_qdma = 0;
		if(TCSUPPORT_FWC_QOS_VAL)
		{
			/*copy stag from skb to eth_info*/
			eth_info.stag = getStagBySkb(skb);
			/*if channel 0 ratelimit is enable , default sent to lan qdma channel 0*/
			/*else, port0~4 sent to lan qdma channel 0~4*/
			if( ENABLE == ratelimit_En )
			{
				eth_info.channel = 0;
				eth_info.ds_to_qdma = 1;
			}
			else
			{
				/* Assign Channel by port mask */
				port_mask = getPortMask(skb);
				eth_info.channel = getChannelByPortmask(port_mask);
				/*GE-port, and has been set per-port ratelimit,then go qdma*/
				if( (GE_PORT == eth_info.channel) && (ENABLE == ge_portlimit_En) )
				{
					eth_info.ds_to_qdma = 1;
				}

			}
		}
		else
		{
			/* Assign Channel by port mask */
			port_mask = getPortMask(skb);
			eth_info.channel = getChannelByPortmask(port_mask);
		}	

		/* decide path by link rate */
		if( (eth_info.channel>=0) && (eth_info.channel<LIMIT_PORT_NUM))
		{
			if( (lanPortState[eth_info.channel].linkSpeed == 0) || (lanPortState[eth_info.channel].linkSpeed == 1) )
			{ 
				/* link rate 100/10 mbps to Qdma */
				eth_info.ds_to_qdma = 1;
			}
		}
		if(g_force_dstq == 1) 		/* To Gdma */
			eth_info.ds_to_qdma = 0;
		else if(g_force_dstq == 2)  /* To Qdma */
			eth_info.ds_to_qdma = 1;

		ethTxMsg.raw.channel = eth_info.channel;

		qdma_info = &eth_info;

		if(isEN7526c && ethTxMsg_temp.raw.vlanEn){
			eth_info.stag = ethTxMsg.raw.sp_tag = ethTxMsg_temp.raw.sp_tag;
			ethTxMsg.raw.vlanTag = ethTxMsg_temp.raw.vlanTag;
			ethTxMsg.raw.vlanEn = 1;
			eth_info.eth_macSTagEn = 0;
		}
	}/*else for offload Packet */

	error = QDMA_API_TRANSMIT_PACKETS(ECNT_QDMA_LAN, skb,
        		ethTxMsg.msg[0], ethTxMsg.msg[1], qdma_info);
	if(unlikely(error))
	{
		dev_kfree_skb_any(skb);
		mac_p->macStat.MIB_II.outDiscards++;
		return NETDEV_TX_OK;
	}
	mac_p->macStat.inSilicon.txEnQueueNum++;

	return NETDEV_TX_OK;
}

#if defined(TCSUPPORT_FWC_ENV)
extern int fhdrv_kdrv_get_cpu_mac(unsigned char *mac, unsigned int len);

/*ge_portlimit_En == 1: means ge-port has been set ratelimit*/
int eth_set_ge_portlimit_switch(unsigned char enable)
{
	ge_portlimit_En = enable ;

	return 0;
}
EXPORT_SYMBOL(eth_set_ge_portlimit_switch);

#endif


/************************************************************************
    Funtcion:       set_mac_addr_from_flash
    Description:    read mac addr from flash and set to switch and FE
    Calls:
    Called by:      eth_lan_mac_start
    Input:
    Output:
    Return:
    Others:     
************************************************************************/

void set_mac_addr_from_flash(struct net_device *dev)
{
    int i;
    struct sockaddr addr;

#if defined(TCSUPPORT_FWC_ENV)
	fhdrv_kdrv_get_cpu_mac((unsigned char *)(addr.sa_data), 6);
#else
    for (i=0; i<6; i++) {
        addr.sa_data[i] = READ_FLASH_BYTE(flash_base + 0xff48 + i);
    }
#endif
    if(eth_lan_mac_set_macaddr(dev,&addr)){
        printk(KERN_INFO "The MAC address in flash is null!\n");
    }
    printk(KERN_INFO
           "%s: FE MAC Ethernet address: %02X:%02X:%02X:%02X:%02X:%02X\n",
           dev->name, 
           dev->dev_addr[0], dev->dev_addr[1], dev->dev_addr[2],
           dev->dev_addr[3], dev->dev_addr[4], dev->dev_addr[5]);
}


/************************************************************************
    Funtcion:
    Description:    Starting up the ethernet device 
    Calls:
    Called by:      gmac_netdev_ops
    Input:
    Output:
    Return:
    Others:     ifconfig add
************************************************************************/
static int eth_lan_mac_start(struct net_device *dev)
{
    set_mac_addr_from_flash(dev);

    mac_p = netdev_priv(dev);

    spin_lock_init(&mac_p->lock);
    spin_lock_init(&phy_lock);

#if KERNEL_2_6_36
    mac_p->dev = dev;
#else
    /* Hook up with handlers */
    dev->get_stats          = tc3262_gmac_stats;
    dev->open               = tc3262_gmac_open;
    dev->stop               = tc3262_gmac_close;
    dev->set_multicast_list = tc3262_gmac_set_multicast_list;
    dev->do_ioctl           = tc3262_gmac_ioctl;
    dev->set_mac_address    = tc3262_gmac_set_macaddr;
    /*dev->weight           = MAC_RXDESCP_NO>>1;*/
    dev->weight             = MAC_NAPI_WEIGHT;  
#if VLAN_TAG_USED
    dev->features |= NETIF_F_HW_VLAN_TX;
#endif

#ifdef RAETH_CHECKSUM_OFFLOAD
    dev->features |= NETIF_F_IP_CSUM;
#endif

#endif /*KERNEL_2_6_36*/
    return 0;
}

/************************************************************************
    Funtcion:       macReset
    Description:    
    Calls:
    Called by:      macInit
    Input:
    Output:
    Return:
    Others:     
************************************************************************/
void macReset(void)
{
	if(0) /*external switch need */
	{
		write_reg_word(0xbfb00358, 0x55550001);
		mdelay(200);
		write_reg_word(0xbfb00358, 0x55551001);
		mdelay(10);
	}
    if(use_ext_switch){
	    gswPbusWrite(EXT_GSW_CFG, 0x3);
    }
}


/************************************************************************
    Funtcion:       setup_gdma1_len
    Description:    
    Calls:
    Called by:      macInit
    Input:
    Output:
    Return:
    Others:     
************************************************************************/
void setup_gdma1_len(void)
{
	unsigned short int long_packet_len, short_packet_len;

#ifdef TCSUPPORT_MAX_PACKET_2000
	long_packet_len = 2000;
	short_packet_len = 60;
#else
	long_packet_len = 1518;
	short_packet_len = 60;
#endif
#ifdef TCSUPPORT_FTTDP_V2
/* no 4-byte STAG in LAN side when FTTdp mode */
    short_packet_len -= 4;
#endif
	FE_API_SET_PACKET_LENGTH(FE_GDM_SEL_GDMA1,long_packet_len,short_packet_len);

	return;
	
}


/************************************************************************
    Funtcion:       macGetAdapterByChanID
    Description:    
    Calls:
    Called by:      macInit
    Input:
    Output:
    Return:
    Others:     
************************************************************************/
macAdapter_t * macGetAdapterByChanID(void)
{
	return mac_p;
}

/************************************************************************
    Funtcion:
    Description:    macInit
    Calls:
    Called by:      eth_lan_mac_open
    Input:
    Output:
    Return:
    Others:     
************************************************************************/
void macSetMACCR(macAdapter_t *mac_p)
{
	uint32 reg;

#ifdef RAETH_CHECKSUM_OFFLOAD
	FE_API_SET_DROP_TCP_CHKSUM_ERR_ENABLE(FE_GDM_SEL_GDMA1, FE_ENABLE);
	FE_API_SET_DROP_TCP_CHKSUM_ERR_ENABLE(FE_GDM_SEL_GDMA1, FE_ENABLE);
	FE_API_SET_DROP_TCP_CHKSUM_ERR_ENABLE(FE_GDM_SEL_GDMA1, FE_ENABLE);
#endif
#ifdef TCSUPPORT_FTTDP_V2
/* in FTTdp, packets from PTM_LAN have CRC, but packets from GSW don't have. 
 * We have let (in fe.ko) FE CPORT add CRC to packets from GSW, 
 * so htat packets from PTM_LAN and GSW can both have CRC. 
 * Then, strip thrie CRC in GDM1. */
	FE_API_SET_CRC_STRIP(FE_GDM_SEL_GDMA1, FE_ENABLE);
#endif
	FE_API_SET_FW_CFG(FE_GDM_SEL_GDMA1, FE_TYPE_OC, FE_DP_QDMA1_CPU);
	FE_API_SET_FW_CFG(FE_GDM_SEL_GDMA1, FE_TYPE_MC, FE_DP_QDMA1_CPU);
	FE_API_SET_FW_CFG(FE_GDM_SEL_GDMA1, FE_TYPE_BC, FE_DP_QDMA1_CPU);
	FE_API_SET_FW_CFG(FE_GDM_SEL_GDMA1, FE_TYPE_UC, FE_DP_QDMA1_CPU);

	FE_API_SET_PADDING(FE_ENABLE);
	FE_API_SET_INS_VLAN_TPID(FE_CDM_SEL_CDMA1, 0x8100);

	setup_gdma1_len();
	/* check if FPGA */
	if (isFPGA) {
		/* set 1us clock for FPGA */
		reg = read_reg_word(CR_CLK_CFG);
		reg &= ~(0x3f000000);
		reg |= (0x31<<24);
		write_reg_word(CR_CLK_CFG, reg);
	}

}

/************************************************************************
    Funtcion:       powerdown_unused_phy
    Description:    
    Calls:
    Called by:      eth_lan_mac_open
    Input:
    Output:
    Return:
    Others:     
************************************************************************/
int powerdown_unused_phy(void)
{
    uint32 phy_mask = 0;
    int i = 0;
    if(isEN7521S)
        phy_mask = 0xf00;   /* fe phy (add 8~11) power down*/
    else if(isEN7526F || isEN7526D || isEN7521F)
        phy_mask = 0x100;   /* ge phy (add 8) power down*/
    else if(isEN7526G)
        phy_mask = 0x1f01;  /* phy (add 0,8~12) power down*/
    else if(isEN7513G)
        phy_mask = 0x1f00;  /* phy (add 8~12) power down*/
    else
        phy_mask = 0;
        
    if(phy_mask !=0){
        for(i = 0; i < 32 ; i++){
            if( phy_mask & (1<<i) ){
                EPHY_API_POWER_DOWN(i);
            }
        }
    }
    return 0;
}

/************************************************************************
    Funtcion:       macGetMacAddr
    Description:    
    Calls:
    Called by:      
    Input:
    Output:
    Return:
    Others:
************************************************************************/
void macGetMacAddr(macAdapter_t *mac_p)
{
	uint32 i;

	for ( i = 0; i < 6; i++ )
		mac_p->macAddr[i] = def_mac_addr[i];
}

void rgmii_traffic_setting(void){
    interface_map[WAN_TO_LAN] = TRGMII;
    interface_map[WIFI0_RA_TO_LAN] = RGMII;
    interface_map[WIFI1_RAIX_TO_LAN] = RGMII;
}
/************************************************************************
    Funtcion:
    Description:    macInit
    Calls:
    Called by:      eth_lan_mac_open
    Input:
    Output:
    Return:
    Others:     
************************************************************************/
int macInit(void)
{

	if (macInitialized)
		return 0;

    detect_switch();
    rgmii_traffic_setting();

	macReset(); 

	/* ----- Get Mac Adapter from dummy data or NDIS control block ----- */
	mac_p = macGetAdapterByChanID();

	/* ----- Assign reserved data pointer ----- */
	mac_p->macPhyLinkProfile_p = &enetPhyLinkProfile;
	mac_p->statisticOn = MAC_STATISTIC_ON;
	/* ----- Get the Mac address ----- */
	macGetMacAddr(mac_p);

	/* ----- Initialize Frame Engine Registers ----- */
    macSetMACCR(mac_p);

	/* ----- Initialize Switch Registers ----- */
    macSetGSW(mac_p);

    mt7530_switch_init();
    
	macInitialized = 1;

	init_ethernet_port_map();
	
	return 0;
}

/************************************************************************
    Funtcion:       eth_timer_init
    Description:    init timer for eth polling task
    Calls:
    Called by:      eth_lan_mac_open
    Input:
    Output:
    Return:
    Others:
************************************************************************/
static void adsl_poll_func(unsigned long data)
{
#ifdef CONFIG_TC3162_ADSL
	uint8 modemst;

	if (adsl_dev_ops == NULL)
		goto down_proc;

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL);		  
	switch (modemst) {
		case ADSL_MODEM_STATE_DOWN:
down_proc:
			mod_timer(&eth_poll_timer, jiffies + msecs_to_jiffies(500));
			break;
		case ADSL_MODEM_STATE_WAIT_INIT:
			mod_timer(&eth_poll_timer, jiffies + msecs_to_jiffies(100));
			break;
		case ADSL_MODEM_STATE_INIT:
			mod_timer(&eth_poll_timer, jiffies + msecs_to_jiffies(100));
			break;
		case ADSL_MODEM_STATE_UP:
			mod_timer(&eth_poll_timer, jiffies + msecs_to_jiffies(500));
			break;
        }
#endif		
}

/************************************************************************
    Funtcion:       eth_timer_init
    Description:    init timer for eth polling task
    Calls:
    Called by:      eth_lan_mac_open
    Input:
    Output:
    Return:
    Others:
************************************************************************/
static int eth_timer_init(void)
{
#ifndef MT7505_PQA_SWITCH_TEST
    /* Schedule timer for monitoring link status */
    init_timer(&eth_timer);
    eth_timer.expires = jiffies + msecs_to_jiffies(250);
  	eth_timer.function = eth_monitor;
    eth_timer.data = 0;
    add_timer(&eth_timer);

    /* Schedule timer for monitoring link status */
    init_timer(&eth_poll_timer);
    eth_poll_timer.expires = jiffies + msecs_to_jiffies(500);
  	eth_poll_timer.function = adsl_poll_func;
    eth_poll_timer.data = 0;
    add_timer(&eth_poll_timer);
#endif
    eth_led_init();
    return 0;
}

/************************************************************************
    Funtcion:       eth_timer_init
    Description:    delete timer for eth polling task
    Calls:
    Called by:      eth_lan_mac_open
    Input:
    Output:
    Return:
    Others:
************************************************************************/

static int eth_timer_deinit(void)
{
#ifndef MT7505_PQA_SWITCH_TEST
     /* Kill timer */
    del_timer_sync(&eth_timer);
    del_timer_sync(&eth_poll_timer);
#endif
    eth_led_deinit();
}


/************************************************************************
    Funtcion:
    Description:    Starting up the ethernet device 
    Calls:
    Called by:      gmac_netdev_ops
    Input:
    Output:
    Return:
    Others:     ifconfig eth up
************************************************************************/
static int eth_lan_mac_open(struct net_device *dev)
{
    int err;
    uint32 value = 0;
    printk(KERN_INFO "%s: starting interface.\n", dev->name);
    /* MII setup */
    mac_p->mii_if.phy_id = mac_p->enetPhyAddr;
    mac_p->mii_if.full_duplex = 1;
    mac_p->mii_if.phy_id_mask = 0x1f;
    mac_p->mii_if.reg_num_mask = 0x1f;
    mac_p->mii_if.dev = dev;
    mac_p->mii_if.mdio_read = mdio_read;
    mac_p->mii_if.mdio_write = mdio_write;
    mac_p->mii_if.supports_gmii = mii_check_gmii_support(&mac_p->mii_if);
#if KERNEL_2_6_36
    napi_enable(&mac_p->napi);
#endif
    netif_start_queue(dev);
    value = read_reg_word(GSW_PMCR(6));
    value |= LINK_UP;
    write_reg_word(GSW_PMCR(6), value);
    printk("netif_start_queue\n");
#if defined(TCSUPPORT_AUTOBENCH)
#else
    powerdown_unused_phy();
#endif
    eth_timer_init();
    return 0;
}

/************************************************************************
    Funtcion:
    Description:    Stopping the ethernet device
    Calls:
    Called by:      gmac_netdev_ops
    Input:
    Output:
    Return:
    Others:     ifconfig down
************************************************************************/
static int eth_lan_mac_close(struct net_device *dev)
{
    
    uint32 value = 0;

    value = read_reg_word(GSW_PMCR(6));
    value &= LINK_DOWN;
    write_reg_word(GSW_PMCR(6), value);
    netif_stop_queue(dev);
#if KERNEL_2_6_36
    napi_disable(&mac_p->napi);
#endif
    printk("netif_stop_queue\n");
    eth_timer_deinit();
    return 0;
}


/************************************************************************
    Funtcion:
    Description:    Get the stats information
    Calls:
    Called by:      gmac_netdev_ops
    Input:
    Output:
    Return:
    Others:
************************************************************************/
static struct net_device_stats *eth_lan_mac_stats(struct net_device *dev)
{
	struct net_device_stats *stats;

	stats = &mac_p->stats;
	stats->rx_packets = calcnt(GSW_RX_UNIC(0)) + calcnt(GSW_RX_MULC(0));
	stats->tx_packets = calcnt(GSW_TX_UNIC(0)) + read_reg_word(GSW_RX_MULC(6));
	stats->rx_bytes = calcnt(GSW_RX_OCL(0));
	stats->tx_bytes = read_reg_word(GSW_TX_OCL(wan_port_id));
	stats->rx_dropped = calcnt(GSW_RX_DROC(0));
	stats->tx_dropped = calcnt(GSW_TX_DROC(0));
	stats->multicast = calcnt(GSW_RX_MULC(0));
	stats->rx_errors = calcnt(GSW_RX_ALIGE(0)) + calcnt(GSW_RX_CRC(0)) + 
                        calcnt(GSW_RX_RUNT(0)) + calcnt(GSW_RX_FRGE(0)) + 
                        calcnt(GSW_RX_LONG(0));
	stats->tx_errors = calcnt(GSW_TX_COLC(0));
	stats->collisions = calcnt(GSW_TX_COLC(0));
	return stats;
}

int eth_dstq_write(int value)
{
	if( (value<0) || (value>2)){
		printk("Input Value [0/1/2]\n");
	}else{
		if(g_force_dstq != value){
			g_force_dstq = value;
			perChannelRateLimitSwitch();
		}
	}
	return 0;
}

int eth_gsw_fc_write(int value)
{
	uint32 reg;

	if( (value<0) || (value>1)){
		printk("Input Value [0/1]\n");
	}else{
		if(use_ext_switch){
			reg = read_reg_word(GSW_PMCR(5));
			if( value==0){
				/* fc disable */
				reg &= (~ (ENABLE_RX_FC_PN | ENABLE_TX_FC_PN));
			}
			else{
				/* fc enable */
				reg = reg | ENABLE_RX_FC_PN | ENABLE_TX_FC_PN;
			}
			/* internal switch port 5 */
			write_reg_word(GSW_PMCR(5), reg);
			/* external switch port 6 */
			gswPbusWrite(0x3600, reg);
		}else{
			reg = read_reg_word(GSW_PMCR(6));
			if( value==0){
				/* fc disable */
				reg &= (~ (ENABLE_RX_FC_PN | ENABLE_TX_FC_PN));
			}
			else{
				/* fc enable */
				reg = reg | ENABLE_RX_FC_PN | ENABLE_TX_FC_PN;
			}
			/* internal switch port 6 */
			write_reg_word(GSW_PMCR(6), reg);
		}
	}
	return 0;
}

/************************************************************************
    Funtcion:
    Description:    Handling ioctl call
    Calls:
    Called by:      gmac_netdev_ops
    Input:
    Output:
    Return:
    Others:
************************************************************************/
int eth_lan_mac_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	int rc = 0;
	gsw_reg reg;
	struct mii_ioctl_data mii;
#if defined(TCSUPPORT_SEPERATED_ETH_ITF)	
	int portmap[4] ={-1};
	uint8 port;
	uint8 value;
	int i;
#endif /* TCSUPPORT_SEPERATED_ETH_ITF */
	if (macInitialized) {
		switch (cmd) {
			case RAETH_REG_READ:
				copy_from_user(&reg, ifr->ifr_data, sizeof(reg));
				reg.val = read_reg_word(reg.off);
				copy_to_user(ifr->ifr_data, &reg, sizeof(reg));
				break;

			case RAETH_REG_WRITE:
				copy_from_user(&reg, ifr->ifr_data, sizeof(reg));
				write_reg_word(reg.off, reg.val);
				break;

			case RAETH_GSW_REG_READ:
				copy_from_user(&reg, ifr->ifr_data, sizeof(reg));
				reg.val = read_reg_word(GSW_BASE + reg.off);
				copy_to_user(ifr->ifr_data, &reg, sizeof(reg));
				break;
			case RAETH_GSW_REG_WRITE:
				copy_from_user(&reg, ifr->ifr_data, sizeof(reg));
				write_reg_word(GSW_BASE + reg.off, reg.val);
				break;

			case RAETH_GSWEXT_REG_READ:  /*MTK120625 ///YM*/
				copy_from_user(&reg, ifr->ifr_data, sizeof(reg));
				reg.val = gswPbusRead(reg.off);
				copy_to_user(ifr->ifr_data, &reg, sizeof(reg));
				break;

			case RAETH_GSWEXT_REG_WRITE:  /*MTK120625 ///YM*/
				copy_from_user(&reg, ifr->ifr_data, sizeof(reg));
				gswPbusWrite(reg.off, reg.val);
				break;

			case RAETH_GSW_PHY_READ:
				copy_from_user(&mii, ifr->ifr_data, sizeof(mii));
				mii.val_out = tcMiiStationRead(mii.phy_id, mii.reg_num);
				copy_to_user(ifr->ifr_data, &mii, sizeof(mii));
				break;

			case RAETH_GSW_PHY_WRITE:
				copy_from_user(&mii, ifr->ifr_data, sizeof(mii));
				tcMiiStationWrite(mii.phy_id, mii.reg_num, mii.val_in);
				break;

			case RAETH_GSWEXT_PHY_READ:  /*MTK120625 ///YM*/
				copy_from_user(&mii, ifr->ifr_data, sizeof(mii));
				mii.val_out = gswPmiRead(mii.phy_id, mii.reg_num);
				copy_to_user(ifr->ifr_data, &mii, sizeof(mii));
				break;

			case RAETH_GSWEXT_PHY_WRITE:  /*MTK120625 ///YM*/
				copy_from_user(&mii, ifr->ifr_data, sizeof(mii));
				gswPmiWrite(mii.phy_id, mii.reg_num, mii.val_in);
				break;
		#ifdef TCSUPPORT_MT7530_SWITCH_API
			case RAETH_GSW_CTLAPI:
                /* MT7530 switch*/
				if (swicVendor == SWIC_MT7530)	
				{
					macMT7530gswAPIDispatch(ifr);
				}
				break;
		#endif
#if defined(TCSUPPORT_SEPERATED_ETH_ITF)		
		/* Set port map dynamically  for FHT*/
			case RAETH_GSW_SETPORTMAP:
#if defined(TCSUPPORT_FH_ENV) && defined(TCSUPPORT_CPU_EN7521) && defined(TCSUPPORT_WAN_ETHER)
				if ( -1 != wan_port_id )
					break;
#endif
				copy_from_user(portmap, ifr->ifr_data, sizeof(portmap));
				memset(lan_port_map, -1, RT63365_SWIC_PORTNUM*sizeof(char));
				memset(switch_port_map, -1, RT63365_SWIC_PORTNUM*sizeof(char));
				lan_port_map[0]= portmap[0];
				lan_port_map[1]= portmap[1];
				lan_port_map[2]= portmap[2];
				lan_port_map[3]= portmap[3];
						  
				for (port=0; port<RT63365_SWIC_PORTNUM; port++)
				{
					value = lan_port_map[port];
					if (0<=value && value <RT63365_SWIC_PORTNUM)
					{
						switch_port_map[value] = port;
					}
				}
				break;
#endif /* TCSUPPORT_SEPERATED_ETH_ITF */	
			case RAETH_DSTQ:
				copy_from_user(&reg, ifr->ifr_data, sizeof(reg));
				if(reg.off == 0){
					eth_dstq_write(reg.val);
				}else if(reg.off == 1){
					eth_gsw_fc_write(reg.val);
				}else{
					printk("error dstq type:%x\n", reg.off);
				}
				break;
			default:
				copy_from_user(&mii, ifr->ifr_data, sizeof(mii));
				rc = generic_mii_ioctl(&mac_p->mii_if, &mii, cmd, NULL);
				copy_to_user(ifr->ifr_data, &mii, sizeof(mii));
				break;
		}
	} else {
		rc = -EOPNOTSUPP;
	}
	return rc;
}

/************************************************************************
    Funtcion:   my_eth_change_mtu
    Description:    
    Calls:
    Called by:      gmac_netdev_ops
    Input:
    Output:
    Return:
    Others:
************************************************************************/
#ifdef TCSUPPORT_MAX_PACKET_2000
int my_eth_change_mtu(struct net_device *dev, int new_mtu)
{
	if (new_mtu < 68 || new_mtu > 2000)
		return -EINVAL;
	dev->mtu = new_mtu;
	return 0;
}
#endif

void gswLanLinkSt_monitor(void)
{
	uint32 reg;
	int portId;
	int speed;
	int rateValue[4] = {10, 100, 1000, 10000};	/* unit: mbps */

	QDMA_TxRateLimitSet_T txRateLimitSet;
	QDMA_TxRateLimitGet_T txRateLimitGet;

	if (!macInitialized) {
		return;
	}

	rateValue[1]+=LIMIT_BUFFER_FOR_100MB;
	rateValue[2]+=LIMIT_BUFFER_FOR_1GB;

	for (portId = 0; portId < LIMIT_PORT_NUM; portId++) {
		reg = switch_reg_read(GSW_PMSR(portId));
		lanPortState[portId].linkState = (reg & MAC_LINK_STS);
		if (!(reg & MAC_LINK_STS)) {
			/* Down */
			continue;
		}
		speed = (reg & MAC_SPD_STS) >> MAC_SPD_STS_SHIFT;

		if(lanPortState[portId].linkSpeed != speed){
			/* chnlRateLimitEn keeps original setting  */
			txRateLimitGet.chnlIdx = portId;
			QDMA_API_GET_TX_RATELIMIT(ECNT_QDMA_LAN, &txRateLimitGet);
			txRateLimitSet.chnlRateLimitEn = txRateLimitGet.chnlRateLimitEn;

			txRateLimitSet.chnlIdx = portId;
			txRateLimitSet.rateLimitValue = (rateValue[speed] * 1000 ); 	/* unit: kbps */
			QDMA_API_SET_TX_RATELIMIT(ECNT_QDMA_LAN, &txRateLimitSet);

			lanPortState[portId].linkSpeed = speed;
		}
	}

	return;
}

/************************************************************************
    Funtcion:
    Description:    Setting customized mac address
    Calls:
    Called by:      gmac_netdev_ops
    Input:
    Output:
    Return:
    Others:
************************************************************************/
static int eth_lan_mac_set_macaddr(struct net_device *dev, void *p)
{
	struct sockaddr *addr = p;

	/* Check if given address is valid ethernet MAC address */
  	if (!is_valid_ether_addr(addr->sa_data))
    	return(-EIO);

	/* Save the customize mac address */
  	memcpy(dev->dev_addr, addr->sa_data, dev->addr_len);
  	memcpy(def_mac_addr, addr->sa_data, dev->addr_len);

	FE_API_SET_MAC_ADDR(FE_GDM_SEL_GDMA1, def_mac_addr, 0xf8);
	
	return 0;
}

/************************************************************************
    Funtcion:       GetMacAddr
    Description:    
    Calls:
    Called by:      hwnat.ko
    Input:
    Output:
    Return:
    Others:
************************************************************************/
uint8 *GetMacAddr(void)
{
	static uint8 macAddr[7];
  
  	memcpy(macAddr, def_mac_addr, 6);
	macAddr[6] = 0x0;
	return macAddr;
}
EXPORT_SYMBOL(GetMacAddr);


/************************************************************************
    Funtcion:
    Description:    
    Calls:
    Called by:      ethmac_qdmaCfg_init
    Input:
    Output:
    Return:
    Others:
************************************************************************/
static int eth_recycle_tx(void *txMsg)
{   
    /* get tx msg and increase counters*/
    return 0;
}



/************************************************************************
    Funtcion:
    Description:
    Calls:
    Called by:      ethmac_qdmaCfg_init
    Input:
    Output:
    Return:
    Others:
************************************************************************/
static int qdma_reg_init(void)
{
	int i;
	QDMA_TxQStaticCngstThrh_T txqThreshold;
	QDMA_TxQueueCongestScale_T txqScale;
	QDMA_Mode_t qdmaMode = QDMA_ENABLE;

#if defined (QOS_REMARKING) || defined (TCSUPPORT_HW_QOS)
	/* do QDMA congestion configuration, otherwise low
	 * priority packets may use all tx descriptors. 
	 * We suppose each queue (for all channels) can 
	 * use 16*5==80 dscps at most! */
	memset(&txqScale, 0, sizeof(QDMA_TxQueueCongestScale_T));
	txqScale.maxScale = QDMA_TXQUEUE_SCALE_16_DSCP;
/*	qdma_set_congestion_scale(&txqScale);*/
	
	memset(&txqThreshold, 0, sizeof(QDMA_TxQStaticCngstThrh_T));
	for (i = 0; i < TX_QUEUE_NUM; i++)
	{
		txqThreshold.queueIdx = i;
/*		txqThreshold.grnMaxThreshold = 5;*/
/*		qdma_set_congest_threshold(&txqThreshold);*/
	}
/*	qdma_set_txqueue_threshold_mode(qdmaMode);*/
#endif
	QDMA_TxBufCtrl_T pTxBufCtrl;
	pTxBufCtrl.chnThreshold = 6;
	pTxBufCtrl.totalThreshold = 24;
	pTxBufCtrl.mode = QDMA_ENABLE;
	QDMA_API_SET_TXBUF_THRESHOLD(ECNT_QDMA_LAN,&pTxBufCtrl);
    return 0;
}


static inline int cdma_remove_stag_enable(void)
{
	int regValue = 0;
	regValue = read_reg_word(0xbfb50400);
	if((regValue & 0x1) && (regValue & 0x2))
		return 1;
	else
		return 0;
}
	
static inline int remove_special_tag(ethRxMsg_t * RxMsg,struct sk_buff *skb)
{
    if(macSTagFlag == DISABLE )
    {
        return SUCCESS;
    }
    /*multicast data copy to cpu,we don't do anything here(hw nat driver will handle it)*/
    if(isEN7526c && cdma_remove_stag_enable()){
		return macEN7526cSTagRemove(RxMsg,skb);
    }
    else
    {
        if(REMOVE_STAG(skb) == -1)
        {
            return FAIL;
        }
    }
    return SUCCESS;
        
}


#ifdef TCSUPPORT_AUTOBENCH
extern int autobench_mac_lpbk_flag;
extern int autobench_mac_lpbk_cnt;
extern int autobench_mac_lpbk1_flag;
extern unsigned char LoopbackData[];
int check_data_fail = 0;
#endif
#ifdef TCSUPPORT_AUTOBENCH
static int autobench_rx_hook(struct sk_buff *skb)
{
	uint32 pattern;
    int number = 0;
        if (autobench_mac_lpbk_flag){
            if( memcmp(skb->data + 20 , LoopbackData + 20, 44) == 0 ){
                pattern = skb->data[64];
                for (number=64; number<MAC_LPBK_DATA_LEN; number++){
                    if (skb->data[16] == 0x1){
                        if (skb->data[number] != ((pattern + number - 64) & 0xff))
                            break;
                    } else if (skb->data[number] != skb->data[16]){
                        break;
                    }
                }
            }

            if (number == MAC_LPBK_DATA_LEN){
                autobench_mac_lpbk_cnt++;
            }
            else {
                if (!check_data_fail){
                    int i;
                    for( i=0; i<MAC_LPBK_DATA_LEN; i++){
                        printk("%02x ", (unsigned char)skb->data[i]);
                        if( (i+1)%8 == 0 )
                            printk("\n");
                    }
                    for( i=64; i<MAC_LPBK_DATA_LEN; i++){
                        if (skb->data[16] == 0x1){
                            if (skb->data[i] != ((pattern + i - 64) & 0xff)){
                                break;
                            }
                        }else if (skb->data[16] != skb->data[i]){
                            break;
                        }
                    }
                    printk("\n\nPacket %d data check error\n", autobench_mac_lpbk_cnt);
                    printk("The first errer data at %d bytes\n", i);
                    printk("error data: %02x\n", skb->data[i]);
                    if (skb->data[16] == 0x1)
                        printk("correct data: %02x\n", ((pattern + i - 64) & 0xff));
                    else
                        printk("correct data: %02x\n", skb->data[16]);
                        
                    check_data_fail = 1;
                    printk("\n");
                }
            }   
        return DROP;
        }
        if( autobench_mac_lpbk1_flag ){
            autobench_mac_lpbk_cnt++;
        return DROP;
    }
    return SUCCESS;

}
#endif

void wan_to_lan(struct sk_buff *skb, uint8 local)
{
    struct sk_buff *skb2 = NULL;    // for wan2lan 
    if(masko_on_off
#if defined(TCSUPPORT_WAN2LAN_EXT)
        || (masko_ext & 0x10)
#endif
        ){
        if(skb_headroom(skb) < TX_STAG_LEN){
            skb2 = skb_copy_expand(skb, TX_STAG_LEN, skb_tailroom(skb) , GFP_ATOMIC);
        }else{
            skb2 = skb_copy(skb,GFP_ATOMIC);
        }

        if(skb2 == NULL){
            printk("wan2lan failure in rx direction for skb3 allocate failure.\n");
        }else{
            if(local == 1){
                skb_push(skb2,14);
            }
            skb2->mark |= SKBUF_COPYTOLAN;
            macSend(WAN2LAN_CH_ID,skb2);
        }
    }
}

/*****************************************
1.save outer vlan tag in skb->mark bit[19:8]
2.clean skb-<mark bit[23:21]
*****************************************/
inline void skb_vlan_parser(struct sk_buff *skb)
{
    uint8 *cp=NULL;
    uint16 tag;
    uint16 tpid;

    cp = skb->data;
    /* ingore stag vlan*/
    tpid = *(uint16 *)(cp+12);
    tag  = *(uint16 *)(cp+14);
    if(tpid == ETH_P_8021Q){
        if(hal_api_ext){
            skb->mark &= ~(VLAN_REMARKING | PRI_STORE);
            skb->mark |= (tag & 0xFFF)<<8;
            skb->mark |= (tag & 0xE000)>>12;/*>>13 <<1*/
        }
    }
}


/************************************************************************
    Funtcion:
    Description:
    Calls:
    Called by:      ethmac_qdmaCfg_init
    Input:
    Output:
    Return:
    Others:
************************************************************************/
__IMEM static int eth_rx(void *msg_p, uint msg_len, struct sk_buff *skb, uint rx_len)
{

    struct sk_buff *newskb;
	int npackets = 0;
	unsigned char offload_packet = 0;
	ethRxMsg_t *curRxMsg;
	unsigned char traffic_classify_flag = E_ECNT_FORWARD;
	int dp = 0;
    
    npackets++;
    if(unlikely(skb == NULL)){
        printk("Error Skb NULL, fail to receive!\n");
    }
    /**************** check packet len *****************/ 
	if (unlikely((rx_len < 52) || (rx_len > 2000))){
        mac_p->macStat.inSilicon.rxEtherFrameLengthErr++;
        dev_kfree_skb_any(skb);
        return 1;
    }
    skb_put(skb, rx_len);  

    /**************** check rx msg *****************/ 
    curRxMsg = (ethRxMsg_t*) msg_p;	

#if defined(TCSUPPORT_FWC_MCST)
	if (likely(ra_sw_nat_ds_offload))
	{
		if (likely(ra_sw_nat_hook_rxinfo))
		{
			ra_sw_nat_hook_rxinfo(skb,FOE_MAGIC_GE,(char*)&curRxMsg->rxMsgW1,sizeof(rxMsgWord1_t));
		}
		
		/* whether the pkt sent by hwnat */
		if (1 == ra_sw_nat_ds_offload(skb,&dp))
		{
			ecnt_mc_api_vlan_handle(skb);
			return npackets;
		}
	}
#endif
	
#ifdef TCSUPPORT_RA_HWNAT
	/*MultiCast Offload to WIFI or WIFI Offload*/  
	if((0x18 == RxMsgW1.crsn) || (0x16 == RxMsgW1.crsn)){	
		offload_packet = 1;
	}
#endif
    
	if(offload_packet == 1){
		update_sw_mib(mac_p, skb, RX_MIB);
                skb->dev = lan_dev;
	}else{
		/**************** check rx msg *****************/ 
		if( unlikely(RxMsgW1.ip4f)){
			printk("IP checksum error \n");
 		}
		if( unlikely(RxMsgW1.l4f)){
 			printk("TCP/UDP checksum error \n");
		}

#ifdef TCSUPPORT_HWNAT_L2VID
		if(ra_sw_nat_hook_rx_set_l2lu)
 			ra_sw_nat_hook_rx_set_l2lu(skb);
#endif

#ifdef TCSUPPORT_AUTOBENCH
#else
		/**************** remove stag, mark if wan packet *****************/
		if(unlikely(remove_special_tag(curRxMsg,skb) == FAIL)){
 			dev_kfree_skb_any(skb);
 			return 1;
		}
#endif
    
#if defined(TCSUPPORT_CT_LOOPDETECT)			
		if(check_rcv_hook && check_rcv_hook(skb)){
			return 1;
		}
#endif

#ifdef TCSUPPORT_AUTOBENCH
		if(autobench_rx_hook(skb)== FAIL){
			dev_kfree_skb_any(skb);
			return 1;
		}
#endif /* TCSUPPORT_AUTOBENCH */

#ifdef LOOPBACK_SUPPORT
		if(eth_loopback_rx_hook(curRxMsg, skb) == SUCCESS){
			return 1;
		}
#endif
		if(unlikely(tcconsole_proc(skb)==1)){
			return 1;
		}
                
		if ((RxMsgW1.sport != SPORT_QDMA_LAN && RxMsgW1.sport != SPORT_QDMA_WAN) && 
			(RxMsgW1.ip4 || RxMsgW1.ip6) && (!RxMsgW1.ip4f && !RxMsgW1.l4f)){
			skb->ip_summed = CHECKSUM_UNNECESSARY;
		}else{
			skb->ip_summed = CHECKSUM_NONE;
		}
    
		/**************** Count the MIB-II *****************/
#if defined(TCSUPPORT_WAN_ETHER)
		if(wan_dev && (isComeFromWan(skb) == 1)){
			update_sw_mib(mac_wan_p, skb, RX_MIB);
			wan_to_lan(skb,0);
			skb->dev = wan_dev;
		}else
#endif
		{
			update_sw_mib(mac_p, skb, RX_MIB);
			skb->dev = lan_dev;
		}
#if defined(TCSUPPORT_SEPERATED_ETH_ITF) 
		/**************** virtual interface eth0.1~4 assign to skb->dev********/
		virtual_itf_rx_hook(skb);
#endif
		skb_vlan_parser(skb);
	}/*else for offload packet*/

    /**************** Layer 2 packet handle *****************/
    skb->protocol = eth_type_trans(skb, skb->dev);
    skb->dev->last_rx = jiffies;   
#ifdef TCSUPPORT_RA_HWNAT
    if (ra_sw_nat_hook_rxinfo)
		ra_sw_nat_hook_rxinfo(skb, FOE_MAGIC_GE, (char*)&curRxMsg->rxMsgW1, 
		    sizeof(rxMsgWord1_t));                            

    if (ecnt_l2tp_api_rx_handle(skb) > 0)
    {
	   return 0;
    }
    
    if (ra_sw_nat_hook_rx) {
        if (ra_sw_nat_hook_rx(skb) == 0) {
            return 0;
        }
    }
#endif
#if defined(TCSUPPORT_FH_ENV) && defined(TCSUPPORT_CPU_EN7521) && defined(TCSUPPORT_WAN_ETHER)
	if ( wan_dev == skb->dev )
	{
		if ( fh_ewan_vlan_hook
			&& 0 != fh_ewan_vlan_hook(skb, FHEWAN_RX_HOOK) )
		{
			dev_kfree_skb_any(skb);
			return 1;
		}
	}
#endif

#if defined(TCSUPPORT_WAN2LAN_EXT)
    if(masko_ext & 0x10)
        wan_to_lan(skb,1);
#endif

#if defined(TCSUPPORT_FWC_TRAFFIC_CLASSIFY) || defined(TCSUPPORT_FWC_ENV)
#if defined(TCSUPPORT_FWC_ENV)
	if (0 == strncmp(skb->dev->name, "eth", strlen("eth")))
	{
		skb->mark |= (skb->dev->name[3] - '0') << 1;
	}
#else
	/*mark packet from dev*/
	if (skb->dev->name[4] == '.')
	{
		skb->mark |= ((skb->dev->name[5] - '0') - 1) << 1;
	}
#endif
	ECNT_TRAFFIC_CLASSIFY_HOOK(skb, &traffic_classify_flag);
	if(E_ECNT_DROP == traffic_classify_flag)
	{
#if defined(TCSUPPORT_RA_HWNAT) && defined(TCSUPPORT_RA_HWNAT_ENHANCE_HOOK)
		if(ra_sw_nat_hook_drop_packet)
		{
			ra_sw_nat_hook_drop_packet(skb);
		}
#endif
		dev_kfree_skb_any(skb);
		return 1;
	}
#endif

    netif_receive_skb(skb);
	return 0;
}

#if defined(TCSUPPORT_WAN2LAN_EXT)
int lanPortmap(int lanPort)
{
	return lan_port_map[lanPort - 1];
}
#endif

/************************************************************************
    Funtcion:
    Description:
    Calls:
    Called by:      ethmac_qdmaCfg_init
    Input:
    Output:
    Return:
    Others:
************************************************************************/
__IMEM static int eth_qdmaEventHandler(QDMA_EventType_t qdmaEventType)
{   
    unsigned long flags;
    if ((qdmaEventType == QDMA_EVENT_RECV_PKTS) ||
        (qdmaEventType == QDMA_EVENT_NO_RX_BUFFER))
	{
        if (qdmaEventType == QDMA_EVENT_NO_RX_BUFFER)
            printk("\nQDMA_EVENT_NO_RX_BUFFER\n");
    
		spin_lock_irqsave(&eth_napilock, flags);

		/* eth_lan and eth_wan use the same rx data path*/
		if (napi_schedule_prep(&mac_p->napi))
		{
			QDMA_API_DISABLE_RXPKT_INT(ECNT_QDMA_LAN); /*disable RX interrupt.*/
			__napi_schedule(&mac_p->napi);
		}
		spin_unlock_irqrestore(&eth_napilock, flags);
	}
	else if (qdmaEventType == QDMA_EVENT_TX_CROWDED)
	{
		printk("\nQDMA_EVENT_TX_CROWDED\n");
	}
	else
		printk("\nWrong QDMA Event Type: %d\n", qdmaEventType);

	return 0;
}

/************************************************************************
    Funtcion:
    Description:
    Calls:
    Called by:      ethmac_qdmaCfg_init
    Input:
    Output:
    Return:
    Others:
************************************************************************/
__IMEM static int eth_napiPoll(struct napi_struct *napi, int budget)
{
	int n;
	unsigned long flags;

	/* call eth_rx to receive a packet 
	 * until all packets are received */
	n = QDMA_API_RECEIVE_PACKETS(ECNT_QDMA_LAN, budget);

	if (n < budget)
	{
		spin_lock_irqsave(&eth_napilock, flags);
	
		__napi_complete(napi);
		QDMA_API_ENABLE_RXPKT_INT(ECNT_QDMA_LAN);

		spin_unlock_irqrestore(&eth_napilock, flags);
	}
	
	return n;

}

/************************************************************************
    Funtcion:
    Description:
    Calls:
    Called by:
    Input:
    Output:
    Return:
    Others:
************************************************************************/
static int ethmac_qdmaCfg_init(void)
{
    QDMA_InitCfg_t qdmaInitCfg;
    if(ethmac_qdmaCfg != INITIALIZED){
        QDMA_API_TX_DMA_MODE(ECNT_QDMA_LAN, QDMA_DISABLE);
        QDMA_API_RX_DMA_MODE(ECNT_QDMA_LAN, QDMA_DISABLE);
        /* HW QDMA QoS init */
    	qdma_reg_init();
        /* register QDMA callback functions */
        memset(&qdmaInitCfg, 0, sizeof(QDMA_InitCfg_t));
        qdmaInitCfg.macType = MAC_TYPE_ETHER;
    	qdmaInitCfg.cbRecvPkts = eth_rx;
    	qdmaInitCfg.cbEventHandler = eth_qdmaEventHandler;
        QDMA_API_INIT(ECNT_QDMA_LAN, &qdmaInitCfg);
        
	    QDMA_API_TX_DMA_MODE(ECNT_QDMA_LAN, QDMA_ENABLE);
        QDMA_API_RX_DMA_MODE(ECNT_QDMA_LAN, QDMA_ENABLE);
        ethmac_qdmaCfg = INITIALIZED;
    }
	return 0;
}

static int ethmac_qdmaCfg_deinit(void)
{
    if(ethmac_qdmaCfg == INITIALIZED){
        QDMA_API_RX_DMA_MODE(ECNT_QDMA_LAN, QDMA_DISABLE);
        QDMA_API_DEINIT(ECNT_QDMA_LAN);
        QDMA_API_TX_DMA_MODE(ECNT_QDMA_LAN, QDMA_DISABLE);
        ethmac_qdmaCfg = UNINIT;
    }
	return 0;
}


/************************************************************************
    Funtcion:       eth_lan_dev_register
    Description:    regiester eth lan dev eth0,eth0.1~0.4
    Calls:
    Called by:      eth_mac_init
    Input:
    Output:
    Return:
    Others:
************************************************************************/

static int eth_lan_dev_register(void)
{
    struct net_device *dev;
/* register netdev to kernel */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,18,19)
    dev = alloc_netdev(sizeof(macAdapter_t), lan_itf,NET_NAME_USER, ether_setup);
#else
	dev = alloc_netdev(sizeof(macAdapter_t), lan_itf, ether_setup);
#endif
    if (!dev)
        return -ENOMEM;
        
    lan_dev = dev;
	/*dev->irq = MAC_INT;*/
    mac_p = netdev_priv(dev);

#if KERNEL_2_6_36
    /* Hook up with handlers */
    dev->netdev_ops = &gmac_netdev_ops;

    mac_p->napi.weight = MAC_NAPI_WEIGHT;

    netif_napi_add(dev, &mac_p->napi, eth_napiPoll, MAC_NAPI_WEIGHT);
#else
    dev->init = tc3262_gmac_start;
#endif
    return register_netdev(dev);
}


static int eth_hook_init(void)
{
	if(ecnt_register_hook(&ecnt_driver_ether_op)) {
		printk("ecnt_driver_ether_op register fail\n");
		return -ENODEV ;
    }
#if defined(TCSUPPORT_WAN2LAN_EXT)
	rcu_assign_pointer(gswPbusRead_hook, gswPbusRead);
	rcu_assign_pointer(gswPbusWrite_hook, gswPbusWrite);
	rcu_assign_pointer(lanPortmap_hook, lanPortmap);
#endif

#if defined(TCSUPPORT_CT_DS_LIMIT)
    rcu_assign_pointer(dslimit_SetDsTrtcmEnable,
                        dslimit_macMT7530SetDsTrtcmEnable);
    rcu_assign_pointer(dslimit_SetDsTrtcm,
                        dslimit_macMT7530SetDsTrtcm);
#endif

    return 0;
}

static int eth_hook_exit(void)
{
	ecnt_unregister_hook(&ecnt_driver_ether_op);
#if defined(TCSUPPORT_WAN2LAN_EXT)
	rcu_assign_pointer(gswPbusRead_hook, NULL);
	rcu_assign_pointer(gswPbusWrite_hook, NULL);
	rcu_assign_pointer(lanPortmap_hook, NULL);
#endif

#if defined(TCSUPPORT_CT_DS_LIMIT)
	rcu_assign_pointer(dslimit_SetDsTrtcmEnable, NULL);
	rcu_assign_pointer(dslimit_SetDsTrtcm, NULL);
#endif

}

void downstermToQdmaTxRateLimitInit(void)
{
	int portId = 0;
	QDMA_TxRateLimitSet_T txRateLimitSet;
	QDMA_TxRateLimitGet_T txRateLimitGet;
	
	for(portId=0;portId<LIMIT_PORT_NUM; portId++){
		lanPortState[portId].linkSpeed = -1;
		lanPortState[portId].linkState = 0;

		/* initial rate limit 1000 mbps */
		txRateLimitSet.chnlIdx=portId;
		txRateLimitSet.chnlRateLimitEn = 1;
		txRateLimitSet.rateLimitValue = (1000 * 1000);	/* unit: kbps */
		QDMA_API_SET_TX_RATELIMIT(ECNT_QDMA_LAN, &txRateLimitSet);
	}
		/* initial rate limit 1000 mbps */
		txRateLimitSet.chnlIdx=WIFI_OFFLOAD_CHANNEL;
		txRateLimitSet.chnlRateLimitEn = 1;
		txRateLimitSet.rateLimitValue = (1000 * 1000);	/* unit: kbps */
		QDMA_API_SET_TX_RATELIMIT(ECNT_QDMA_LAN, &txRateLimitSet);
	return;
}

/* set chnlRateLimitEn */
void perChannelRateLimitSwitch(void)
{
	uint32 regVal=0;
	int isRatelimitEn=0;
	int portId = 0;
	QDMA_TxRateLimitSet_T txRateLimitSet;
	QDMA_TxRateLimitGet_T txRateLimitGet;

	/*enable QDMA LAN channel 0~4 rate limit*/
	if(g_force_dstq==0){			/* ds force disable */
		if(use_ext_switch){		/* disable rate limit */
			/* do nothing */
		}else{						/* enable rate limit */
			isRatelimitEn=1;
		}
	}else if(g_force_dstq==1){ /* ds force to gdma, disable rate limit */
		/* do nothing */
	}else if(g_force_dstq==2){ /* ds force to qdma, enable rate limit */
		isRatelimitEn=1;
	}

	for(portId=0;portId<LIMIT_PORT_NUM; portId++){
		/* Get Channel Setting */
		txRateLimitGet.chnlIdx = portId;
		QDMA_API_GET_TX_RATELIMIT(ECNT_QDMA_LAN, &txRateLimitGet);

		/* update chnlRateLimitEn of channel  */
		txRateLimitSet.chnlIdx = portId;
		txRateLimitSet.rateLimitValue = txRateLimitGet.rateLimitValue;

#if defined(TCSUPPORT_WAN_ETHER)
		if(portId == wan_port_id){
			/* Disable QDMA limit if the port is used for ETHERWAN */
			txRateLimitSet.chnlRateLimitEn = 0;
		}else{
			txRateLimitSet.chnlRateLimitEn = isRatelimitEn;
		}
#else
		txRateLimitSet.chnlRateLimitEn = isRatelimitEn;
#endif
		QDMA_API_SET_TX_RATELIMIT(ECNT_QDMA_LAN, &txRateLimitSet);
	}

	return;
}

static int __init eth_mac_init(void)
{
    int err = 0;

    g_force_dstq=0;
    downstermToQdmaTxRateLimitInit();

    printk(KERN_INFO "%s", version);
    /* register callback function to qdma */
    ethmac_qdmaCfg_init();

    err = eth_lan_dev_register();
    if(err)
        return err;

#if defined(TCSUPPORT_SEPERATED_ETH_ITF) 
    err = init_virtual_interface();
    if(err)
        return err;
#endif

#if defined(TCSUPPORT_WAN_ETHER)
	eth_wan_init();
#endif

#ifdef TCSUPPORT_HEC_6906
    amc_verify_init();
#endif


#ifdef TC_CONSOLE_ENABLE 
    tcconsole_init();
#endif
    eth_proc_init();

    err = eth_hook_init();
    if(err)
        return err;

    macInit();

    /* Etherwan port map is used, set chnlRateLimitEn after macInit() */
    perChannelRateLimitSwitch();
    
    ecnt_register_hook(&ecnt_eth_updateKernelMacTable_op);
    
    return 0;
}


static void __exit eth_mac_exit(void)
{
    ethmac_qdmaCfg_deinit();

#if defined(TCSUPPORT_SEPERATED_ETH_ITF) 
    exit_virtual_interface();
#endif
    macReset();	

	unregister_netdev(lan_dev);

	free_netdev(lan_dev);
#if defined(TCSUPPORT_WAN_ETHER)	
	eth_wan_exit();
#endif

#ifdef TC_CONSOLE_ENABLE 
	tcconsole_exit();
#endif
    eth_proc_exit();
    eth_hook_exit();

    ecnt_unregister_hook(&ecnt_eth_updateKernelMacTable_op);
}
 
#if defined(TCSUPPORT_FH_ENV) && defined(TCSUPPORT_CPU_EN7521) && defined(TCSUPPORT_WAN_ETHER)
int eth1_active_op(int active)
{
	uint8 port = 0, value = 0;

	/* only support open eth1, cannot close it. */
	if ( 1 == active )
	{
		wan_port_id = 4;
		lan_port_map[0] = 3;
		lan_port_map[1] = 2;
		lan_port_map[2] = 1;
		lan_port_map[3] = -1;

		for ( port=0; port<RT63365_SWIC_PORTNUM; port++ )
		{
			value = lan_port_map[port];
			if ( 0<=value && value <RT63365_SWIC_PORTNUM )
			{
				switch_port_map[value] = port;
			}
		}

	    /* Etherwan port map is used, set chnlRateLimitEn after macInit() */
	    perChannelRateLimitSwitch();
	}

	return 0;
}
#endif
 

/* Register startup/shutdown routines */
module_init(eth_mac_init);
module_exit(eth_mac_exit);
module_param(lan_itf, charp, S_IRUGO);
module_param(sep_itf, charp, S_IRUGO);
module_param(wan_itf, charp, S_IRUGO);
module_param(itf_start_idx, int, S_IRUGO);
module_param(itf_num, int, S_IRUGO);


MODULE_LICENSE("Proprietary");

