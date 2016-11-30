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
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/dma-mapping.h>
#include <linux/mii.h>
#include <asm/io.h>
#include <asm/tc3162/tc3162.h>
#include <asm/tc3162/ledcetrl.h>
#include <asm/tc3162/TCIfSetQuery_os.h>

#include "../eth_lan.h"
#include "../eth_proc.h"

#if defined(TCSUPPORT_SEPERATED_ETH_ITF) 
#include "../eth_virtual_if.h"
#endif

#ifdef TCSUPPORT_MT7530_SWITCH_API
#include "mtkswitch_api.h"
#include <modules/eth_global_def.h>
#endif


#ifdef TCSUPPORT_INIC_HOST
#include <linux/mtd/fttdp_inic.h>
#endif

#include "tcconsole.h"
#include "tcswitch.h"
#include "phy_api.h"
#include <linux/libcompileoption.h>
#include <ecnt_hook/ecnt_hook_traffic_classify.h>

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
#define GSW_BASE     		0xBFB58000
#define GSW_PMSR(n)     	(GSW_MAC_BASE + (n)*0x100 + 0x08)

#if defined(TCSUPPORT_QOS) && defined(TCSUPPORT_WAN_ETHER)
#define 	QOS_HH_PRIORITY		0x10
#define 	QOS_H_PRIORITY		0x20
#define 	QOS_M_PRIORITY		0x30
#define 	QOS_L_PRIORITY		0x40
#define		NULLQOS				-1
#define 	QOS_SW_PQ			0	/*will use hw at the same time*/
#define		QOS_SW_WRR			1
#define		QOS_SW_CAR			2
#define 	QOS_HW_WRR			3
#define		QOS_HW_PQ			4

extern int qos_flag ;
#endif	

#define DEF_PRIORITY_PKT_CHK_LEN	100
#define QUQUE_REMARK_MASK   		(0x7)
#define QUEUE_REMARK_OFFSET 		(11)

/************************************************************************
*                  M A C R O S
*************************************************************************
*/
#define read_reg_word(reg) 		regRead32(reg)
#define write_reg_word(reg, wdata) 	regWrite32(reg, wdata)
#define CHK_BUF() pos = begin + index; if (pos < off) { index = 0; begin = pos; }; if (pos > off + count) goto done;

/************************************************************************
*                  D A T A   T Y P E S
*************************************************************************
*/

/************************************************************************
*                  E X T E R N A L   D A T A   D E C L A R A T I O N S
*************************************************************************
*/
#if defined(TCSUPPORT_HWNAT)
/*#include "../../../linux/net/bridge/br_private.h"*/
extern int port_reverse;
#endif
extern uint8 swicVendor;
extern macAdapter_t *mac_p;


#ifdef WAN2LAN
extern int masko;
#endif

#if defined(TCSUPPORT_GPON_MAPPING) && defined(TCSUPPORT_GPON_DOWNSTREAM_MAPPING)
extern int (*gpon_downstream_mapping_stag_hook)(struct sk_buff *skb);
#if defined(TCSUPPORT_CT_DS_LIMIT)
extern int dataspeed_limit_queue_hook(struct sk_buff *skb);
#endif
#endif
#ifdef TCSUPPORT_XPON_LED
extern uint8 led_ether_up_status[];
#endif


#if defined(CONFIG_MIPS_RT63365)

extern uint16 lan_port_tpid[MT7530_SWIC_PORTNUM];
extern uint16 ether_wan_tpid;
extern uint16 cpu_port_tpid;
#endif

extern uint8 interface_map[TRAFFIC_TYPE_MAX];
/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/


#if defined(TCSUPPORT_CPU_MT7520)
extern int (*MT7530LanPortMap2Switch_hook)(int port);
extern u32 macMT7530LanPortMap2Switch(u32 portId);
#endif

/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/
int priPktChkLen = DEF_PRIORITY_PKT_CHK_LEN;
int priPktChk = 1;
uint8 use_ext_switch;

char esd_has_reset=0;
/*#ifdef VPORT*/
/*extern int vportMasko;*/
/*#endif*/
#if 1/*def VPORT*/
int vportMasko=0;
#if !defined(TCSUPPORT_XPON_HAL_API_EXT) 
int vport_enable=0;
#endif
#endif

#if defined(TCSUPPORT_CT)
#if defined(TCSUPPORT_CPU_RT63365)
int g_port_reverse = 0;
#else
#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_CT_PON_CY)
int g_port_reverse = 1;
#else
int g_port_reverse = 0;
#endif
#else
int g_port_reverse = 1;/*dedault need port reverse function according to customer's requset*/
#endif
#endif

#if defined(TCSUPPORT_CPU_MT7510)
int g_stag_to_vtag = 1;
#else
int g_stag_to_vtag = 0;
#endif
#else
/*extern int g_port_reverse;*/
int g_port_reverse = 0;
/* special tag to vlan tag */
int g_stag_to_vtag = 0;
#endif

uint8 macSTagFlag = 0;
uint8 macVlanFlag = 0;
uint8 macRxPortFlag = 0;
uint8 esd_sw_patch = 1;/*Default is enabled the ESD software patch*/

#if defined(CONFIG_MIPS_RT63365)
int wan_port_id=0;
/* map to switch ports */

#ifdef TCSUPPORT_XPON_HAL_API_MCST
extern char lan_port_map[ RT63365_SWIC_PORTNUM ];
#else
char lan_port_map[RT63365_SWIC_PORTNUM];
EXPORT_SYMBOL(lan_port_map);
#endif	

char switch_port_map[ RT63365_SWIC_PORTNUM ];
#endif

/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/
static uint8 maxPrio = 3;

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

uint32 switch_reg_read(uint32 reg)
{
    if(use_ext_switch){
        return gswPbusRead(reg-GSW_BASE);
    }else{
        return read_reg_word(reg);
    }
}

uint32 switch_reg_write(uint32 reg, uint32 wdata)
{
    if(use_ext_switch){
        gswPbusWrite((reg-GSW_BASE), wdata);
    }else{
        write_reg_word(reg, wdata);
    }
}

#if defined(TCSUPPORT_PON_VLAN) || defined(TCSUPPORT_PON_MAC_FILTER) || defined(TCSUPPORT_XPON_IGMP) || defined(TCSUPPORT_ETHER_DOWNVLAN)
int xpon_get_vport(void)
{
	return vport_enable;
}
EXPORT_SYMBOL(xpon_get_vport);
#endif


#if defined(TCSUPPORT_CT_LOOPDETECT)
inline int isDetectPacket(uint8 *data, unsigned int len)
{
	int port_mask = 0;
	struct cthdr  *cthdr = NULL;

	if(len < sizeof(struct cthdr))
		return 0;
	
	cthdr = (struct cthdr*)(data+len-sizeof(struct cthdr) -1);
	
	if(0==strncmp(cthdr->magic, "ABYZ", 4)
		&& 0==strncmp(cthdr->name, "sofree", 6)){
		port_mask = cthdr->from;

#if defined(TCSUPPORT_CUC_C5_2P)
	uint16 temport = 0;
	if(cthdr->from  == 1) temport = 2;
	else if(cthdr->from  == 2) temport = 4;
	else if(cthdr->from  == 4) temport = 8;
	else if(cthdr->from  == 8) temport = 1;
	cthdr->from = temport;
#else
		if ( g_port_reverse )
		{
			switch ( cthdr->from )
			{
				case 1:
					cthdr->from = 8;
					break;
				case 2:
					cthdr->from = 4;
					break;
				case 4:
					cthdr->from = 2;
					break;
				case 8:
					cthdr->from = 1;
					break;
				default:
					break;
			}
		}
#endif
		return port_mask;
	}
	return 0;
}
#endif



#if defined(CONFIG_MIPS_RT63365)

inline uint8 macEN7512STagRemap(uint8 port_mask)
{
	uint8 port=0;
	uint8 phyport;
	uint8 value;

	value = 0;
	for (port=0; port<RT63365_SWIC_PORTNUM; port++){
		if (port_mask & (1<<port)){
			phyport = lan_port_map[ (int)port ];			
			if (0<=phyport && phyport<RT63365_SWIC_PORTNUM){
				value |= (1<<phyport);
			}
		}
	}
	
	return value;
}

void init_ethernet_port_map()
{
	uint8 port;
	uint8 value;
	int wan_port;
	int lan[6];
	int i;
	memset(lan_port_map, -1, RT63365_SWIC_PORTNUM*sizeof(char));
	memset(switch_port_map, -1, RT63365_SWIC_PORTNUM*sizeof(char));

#if defined(TCSUPPORT_MANUAL_ETHERNET_PORTMAP)
	sscanf(TCSUPPORT_ETHERNET_PORTMAP, "%d_%d_%d_%d_%d_%d_%d", &wan_port, 
								&lan[0], &lan[1], &lan[2], &lan[3], &lan[4], &lan[5]);
	wan_port_id = -1 ;									
#if defined(TCSUPPORT_WAN_ETHER)
	wan_port_id = wan_port;
	if ((0>wan_port || wan_port>=RT63365_SWIC_PORTNUM)){
		printk("Error: Config ether wan port: %d\n", wan_port_id);
		printk("       Ethernet wan port cannot find mapped switch port\n");
		printk("       The mapped switch port should be between 0 and 5\n");
	}
#endif
	for (port=0; port<6; port++){
		if (0<=lan[port]  && lan[port]<RT63365_SWIC_PORTNUM){
			lan_port_map[port] = (char)lan[port];
		}
	}
#else
#if defined(TCSUPPORT_CPU_EN7521)
        wan_port_id = -1;
        lan_port_map[0] = 1;
        lan_port_map[1] = 2;
        lan_port_map[2] = 3;
        lan_port_map[3] = 4;
#elif defined(TCSUPPORT_CPU_EN7512)
        wan_port_id = 4;
        lan_port_map[0] = 0;
        lan_port_map[1] = 1;
        lan_port_map[2] = 2;
        lan_port_map[3] = 3;
#else
        wan_port_id = -1;
        lan_port_map[0] = 0;
        lan_port_map[1] = 1;
        lan_port_map[2] = 2;
        lan_port_map[3] = 3;
#endif
#endif

	for (port=0; port<RT63365_SWIC_PORTNUM; port++){
		value = lan_port_map[ port ];
		if (0<=value && value <RT63365_SWIC_PORTNUM){
			switch_port_map[ value ] = port;
		}
	}
#if defined(TCSUPPORT_CPU_MT7520)
		MT7530LanPortMap2Switch_hook = macMT7530LanPortMap2Switch;
#endif
}



int macEN7512STagEnable(int chanID)
{
	uint32 value;

    if(use_ext_switch){
        /*Enable Special Tag function*/
    	value = gswPbusRead(0x2610);
    	/* set CPU port 6 as user port */
    	value &= ~0xc0;
    	/* set CPU port 6 special tag=1 */
    	value |= (1<<5);
    	gswPbusWrite(0x2610, value);
    }else{
        /*Enable Special Tag function*/
		value = regRead32(GSW_BASE + 0x2610);
		/* set CPU port 6 as user port */
		value &= ~0xc0;
		/* set CPU port 6 special tag=1 */
		value |= (1<<5);
		regWrite32((GSW_BASE + 0x2610), value);
    }
	/* Enable SPECAIL TAG */
	value = regRead32(CDMA_CSG_CFG);
	value |= CDM_STAG_EN;
	regWrite32(CDMA_CSG_CFG, value);
	
	value = regRead32(GDMA1_FWD_CFG);
	value |= GDM_STAG_EN;
	regWrite32(GDMA1_FWD_CFG, value);

    /*enable GDM1 Stag Indiation for FE*/
    regWrite32(GDMA1_VLAN_GEN, 0x1);
	macSTagFlag = 1;

	return 0;
}/*end macEN7512STagEnable*/


int macEN7512STagDisable(int chanID)
{
	#if defined(TCSUPPORT_CPU_RT63368)
	uint32 value;

	/*Enable Special Tag function*/
	value = gswPbusWrite(0x2610);
	/* set CPU port 6 as user port */
	value &= ~0xc0;
	/* set CPU port 6 special tag=1 */
	value &= ~(1<<5);
	gswPbusWrite(0x2610, value);

	/* Enable GDM_TCI_81XX */
	value = regRead32(GDMA1_FWD_CFG);
	value &= ~(GDM_TCI_81XX);
	regWrite32(GDMA1_FWD_CFG, value);
	#endif

	return 0;
}/*end macEN7512STagDisable*/

#if defined(TCSUPPORT_WAN_ETHER)
int isComeFromWan(struct sk_buff *skb){
	if(*(unsigned long int *)(skb->cb) == CB_MAGIC){
		return 1;
	}

	return 0;

}
void setComeFromWan(struct sk_buff *skb, int yes){
	if(yes == 1){
		*(unsigned long int *)(skb->cb) = CB_MAGIC; 
	}else{
		*(unsigned long int *)(skb->cb) = 0; 
	}	
	return;
}
#endif

#ifdef TCSUPPORT_WAN_ETHER
/*
 * If the returned value is 0, we don't care (*priority).
 * If the returned value is 1, we will use (*priority) 
 * as new priority 
 */
static uint8 isPriorityPkt(uint8 *cp, int *priority)
{
	uint16 etherType;
	uint8 ipVerLen;
	uint8 ipProtocol;
	uint8 tcpFlags;
	uint16 pppProtocol;


	if (cp == NULL)
	{
		printk("\nERROR: cp is NULL at isPriorityPkt\n");
		return 0;
	}
	if (priority == NULL)
	{
		printk("\nERROR: priority is NULL at isPriorityPkt\n");
		return 0;
	}

	/* skip DA and SA mac address */
	cp += 12;
	/* get ether type */
	etherType = *(uint16 *) cp;
	/* skip ether type */
	cp += 2;

	/*parse if vlan exists*/
	if (etherType == 0x8100) {
		/*skip 802.1q tag field*/
		cp += 2;
		/*re-parse ether type*/
		etherType = *(uint16 *) cp;
		/* skip ether type */
		cp += 2;
	}

	/*check whether PPP packets*/ 
	if (etherType == 0x8864) {
		/* skip pppoe head */
		cp += 6; 					/* 6: PPPoE header 2: PPP protocol */
		/* get ppp protocol */
		pppProtocol = *(uint16 *) cp;
		/* check if LCP protocol */
		if (pppProtocol == 0xc021) {
			*priority = 3;
			return 1;
		/* check if IP protocol */
		} else if (pppProtocol != 0x0021) {
			return 0;
		}
		/* skip ppp protocol */
		cp += 2; 					/* 6: PPPoE header 2: PPP protocol */
	} else if (etherType == 0x8863) {
		*priority = 3;
		return 1;
	} else {
		/* check if ip packet */
		if (etherType != 0x0800)
			return 0;
	}

	/* check if it is a ipv4 packet */
	ipVerLen = *cp;
	if ((ipVerLen & 0xf0) != 0x40) 
		return 0;

	/* get ip protocol */
	ipProtocol = *(cp + 9);
	/* check if ICMP/IGMP protocol */
	if ((ipProtocol == 1) || (ipProtocol == 2)) {
		*priority = 3;
		return 1;
	}
	/* check if TCP protocol */
	if (ipProtocol != 6)
		return 0;
		
	/* align to TCP header */
	cp += (ipVerLen & 0x0f) << 2;
	/* get TCP flags */
	tcpFlags = *(cp + 13);
	/* check if TCP syn/ack/psh ack/fin ack */
	if (((tcpFlags & 0x10) == 0x10) || (tcpFlags == 0x02)) {
		*priority = 3;
		return 1;
	}
		
	return 0;
}

uint32 getPbitForSwitchQos(struct sk_buff *skb)
{	
	uint32 txq = 0;
	int newTxq;
#ifdef TCSUPPORT_QOS
	switch (qos_flag) {
		case QOS_SW_PQ:
			/* PQ mode */
			if (txq < 2 && (skb->mark & QOS_FILTER_MARK) == QOS_HH_PRIORITY) {
				txq = 2;
			}
			else if (txq < 1 && (skb->mark & QOS_FILTER_MARK) == QOS_H_PRIORITY) {
				txq = 1;
			}
			break;
	#if 1/*def TCSUPPORT_HW_QOS*/
		case QOS_HW_WRR:
			/* HW WRR mode */
			if ((skb->mark & QOS_FILTER_MARK) == QOS_HH_PRIORITY) {
				txq = 3;
			}
			else if ((skb->mark & QOS_FILTER_MARK) == QOS_H_PRIORITY) {
				txq = 2;
			}
			else if ((skb->mark & QOS_FILTER_MARK) == QOS_M_PRIORITY) {
				txq = 1;
			}
			else {
				txq = 0;
			}
			break;
		case QOS_HW_PQ:
			/* HW PQ mode */
			if (txq < 3 && (skb->mark & QOS_FILTER_MARK) == QOS_HH_PRIORITY) {
				txq = 3;
			}
			else if (txq < 2 && (skb->mark & QOS_FILTER_MARK) == QOS_H_PRIORITY) {
				txq = 2;
			}
			else if (txq < 1 && (skb->mark & QOS_FILTER_MARK) == QOS_M_PRIORITY) {
				txq = 1;
			}
			break;
		case NULLQOS: /*It's for putting rtp packets to HH priority when qos_flag not be selected as WRR or PQ*/
			if ((skb->mark & QOS_FILTER_MARK) == QOS_HH_PRIORITY) {
				txq = 3;
			}
			break;
	#endif
		default:
			break;
	}
#endif
	
#ifdef QOS_REMARKING  
	if((skb->mark & QOS_REMARKING_FLAG)){
		txq = (uint8)((skb->mark & QOS_REMARKING_MASK) >> 1);
	}
#endif

	if (priPktChk && (skb->len < priPktChkLen))
	{
		if (isPriorityPkt(skb->data, &newTxq))
		{
	#ifdef TCSUPPORT_QOS
			if (qos_flag == QOS_HW_WRR)
				txq = maxPrio; /*maxPrio is the txq with max weight*/
			else
			{
				if (newTxq > txq)
					txq = newTxq;
			}
	#else
			txq = newTxq;
	#endif
		}
	}

	return txq;
}

#endif




static inline uint8 Calc_VPM(uint16 tpid,uint16 sp_tpid)
{
	if(tpid == 0x8100)
		return 1;
	else if(tpid == 0x88a8)
		return 2;
	else if(tpid == sp_tpid)
		return 3;
	else
		return 0;
}

static inline uint16 Get_Tpid_By_Port(uint8 port)
{
	if (port == wan_port_id+1){
		return ether_wan_tpid;
	}else if (port == 7){
		return cpu_port_tpid;
	}else if (port > 0 && port < MT7530_SWIC_PORTNUM+1){
		return lan_port_tpid[port -1];
	}
	return DEFAULT_TPID;
}

static inline struct sk_buff * Insert_4Byte_to_Skb(struct sk_buff * bp)
{
	uint8 * cp = NULL;
	
	if (skb_headroom(bp) < 4) 
	{
		struct sk_buff *bp2 = skb_realloc_headroom(bp, 4);
		dev_kfree_skb(bp);
		if (bp2 == NULL){
			return NULL;
		}
		bp = bp2;
	}
	else 
	{
		bp = skb_unshare(bp, GFP_ATOMIC);
		if (!bp)
			return NULL;
	}
	skb_push(bp, 4);
	cp = bp->data;
	memmove(cp, cp+4, 12);
	return bp;
}

inline int setPortMask(struct sk_buff *skb, uint8 port_mask){
	*(unsigned long int *)(skb->cb+4) = (unsigned long int)port_mask;
	return 0;
}

inline unsigned long int getPortMask(struct sk_buff *skb){
	return *(unsigned long int *)(skb->cb+4);
}

inline uint32 getTrafficType(struct sk_buff *skb){
    if(skb->original_dev == NULL){
        return WAN_TO_LAN;
    }else if(skb->original_dev->name[0] == 'r'){
        if(skb->original_dev->name[2] == 'i'){      /* rai0*/
            return WIFI1_RAIX_TO_LAN;
        }else{    /* ra0*/
            return WIFI0_RA_TO_LAN;
        }
    }
    return WAN_TO_LAN;
}

static inline unsigned char getQueueInfoBySkb(struct sk_buff *skb, uint16 tpid, uint16 tci)
{	
	unsigned char txq=0;

	if(skb->mark & (1 << QUQUE_REMARK_EXIST_OFFSET))
	{
		/*if queue is remark, get txq according to queue filed in skb->mark*/
	txq = (skb->mark >> QUEUE_REMARK_OFFSET) & QUQUE_REMARK_MASK;
	}
	else
	{
		/*if queue is not remark, get txq according to pbit*/	
		if(tpid == 0x8100 || tpid == 0x88a8 || tpid == cpu_port_tpid)/*tagged packet*/
		{
			txq = (tci >> VLAN_PRIO_SHIFT) & QUQUE_REMARK_MASK; 		
		}
	}
	return txq;
}
inline int macEN7526cSTagInsert(ethTxMsg_t *TxMsg,struct sk_buff **bpp)
{
	uint8 *cp;
	uint8 port_mask=0;
	uint16 tag0, tag1;
	uint8 vpm = 0;
	uint8 stag_up=0;
#if defined(TCSUPPORT_CT_LOOPDETECT)	
	int temp_mask = 0;
	int isLoopDetectPkt = 0;
#endif
	struct sk_buff *bp = *bpp;
	cp = bp->data;
	tag0 = *(uint16 *)(cp+12);
	tag1 = *(uint16 *)(cp+14); 
	
	if(TCSUPPORT_FWC_QOS_VAL)
		stag_up = getQueueInfoBySkb(bp,tag0,tag1)|0x8;
#ifdef WAN2LAN
	if (bp->mark & SKBUF_COPYTOLAN){
		port_mask = masko & 0xF;
#if defined(TCSUPPORT_CT_LOOPDETECT)		
			if(temp_mask=isDetectPacket(cp, bp->len)){
				port_mask = temp_mask;
				isLoopDetectPkt = 1;
			}
#endif
		port_mask = macEN7512STagRemap(port_mask);
	}
#endif
#if defined(TCSUPPORT_CT_LOOPDETECT)		
		if(!isLoopDetectPkt){
#endif
#ifdef TCSUPPORT_SEPERATED_ETH_ITF
	int eth_itf =0;
	int port_num =0;
        eth_itf = skbFromWhichVirtualEthItf(bp);
        port_num = lan_port_map[eth_itf];
		port_mask |= 1 << port_num;
#endif
#if defined(TCSUPPORT_CT_LOOPDETECT)
		}
#endif

#ifdef TC_CONSOLE_ENABLE
        if ((bp->mark & SKBUF_TCCONSOLE)){
            port_mask = 0x0f;
            port_mask = macEN7512STagRemap(port_mask);
        }
#endif		
	if(!(tag0 == 0x8100 || tag0 == 0x88a8 || tag0 == cpu_port_tpid))
	{
	        /*untag packet, cdma to insert stag*/
		TxMsg->raw.vlanTag= 0;
		TxMsg->raw.sp_tag = (port_mask) | (stag_up<<12);
		TxMsg->raw.vlanEn = 1;
	}else{
	        /*tagged frame, software insert stag is better*/
		vpm = Calc_VPM(tag0,cpu_port_tpid);
		tag0 = (vpm << 8) + (port_mask) | (stag_up<<12);
		TxMsg->raw.vlanEn = 0;
	}

#if defined(TCSUPPORT_GPON_MAPPING) && defined(TCSUPPORT_GPON_DOWNSTREAM_MAPPING)
	if(gpon_downstream_mapping_stag_hook)/*only change the Queue index here*/
		gpon_downstream_mapping_stag_hook(bp);
#endif

	eth_led_tx_hook(port_mask);
	setPortMask(bp, port_mask);
	return SUCCESS;
}
inline int macEN7526cSTagRemove(ethRxMsg_t * RxMsg,struct sk_buff *bp)
{
	uint16 sp_tag = 0;
	uint16 tci = 0;
	uint16 port_num = 0;	
		
	if(1 != RxMsg->rxMsgW2.raw.untag){
		return SUCCESS;
	}
	/*get info from RxMsg*/
	sp_tag = (RxMsg->rxMsgW3.sp_tag & 0xffff0000) >>16;
	tci = (RxMsg->rxMsgW3.sp_tag & 0x0000ffff) <<16;

	port_num = (sp_tag & 0x7); /*switch port id*/

#if defined(TCSUPPORT_SEPERATED_ETH_ITF) 
        int eth_intf = 0;

        for(;eth_intf <= 4; eth_intf++)
        {
            if(lan_port_map[eth_intf] == port_num)
            {
                setSkbFromWhichVirtualEthItf(bp,eth_intf);
                break ;
            }
        }
#endif
    	eth_led_rx_hook(port_num);
	return SUCCESS;
}
inline struct sk_buff *macEN7512STagInsert(struct sk_buff *bp)
{
	uint8 *cp;
	uint8 port_mask=0;
	uint8 pvid;
	uint16 tag0, tag1;
	
	uint8 vpm = 0;
	uint16 tpid = 0;
	uint8 flag = 0;
	uint8 stag_up=0;
	uint32 rgmii_itf = 0;

#if defined(TCSUPPORT_CT_LOOPDETECT)	
	int temp_mask = 0;
	int isLoopDetectPkt = 0;
#endif

	cp = bp->data;
	tag0 = *(uint16 *)(cp+12); /* tpid*/
	tag1 = *(uint16 *)(cp+14); /* tag1 = x = eth0.x*/
	if(TCSUPPORT_FWC_QOS_VAL)
	{
		stag_up = getQueueInfoBySkb(bp,tag0,tag1)|0x8;
	}

	if(((vport_enable)/*multi port case*/
#ifdef WAN2LAN
		&& (!(bp->mark & SKBUF_COPYTOLAN))
#endif
#if defined(TCSUPPORT_WAN_ETHER)	
		&& (isComeFromWan(bp) != 1)
#endif
#if defined(TCSUPPORT_SEPERATED_ETH_ITF) 
        && (skbFromWhichVirtualEthItf(bp) == SEP_ETH_ITF_UNKNOWN)
#endif
		) 
#ifdef TCSUPPORT_AUTOBENCH
		|| ( tag0 == 0x8200)
#endif
		)
	{
		bp = skb_unshare(bp, GFP_ATOMIC);
		if (!bp)
			return NULL;
		cp = bp->data;

#ifdef TCSUPPORT_AUTOBENCH
			flag = 1;
#endif

		/*
			multi port case vlan_dev will add a tag to skb. so we check the second tag.
			if packet is tagged,we will remove the tag added by vlan_dev.
			if not,we will keep the tag which added by vlan_dev
			when packet sent to switch,it must be sent to CPU port first.so we only check
			cpu port tpid here.
		*/
		if(*(uint16 *)(cp+16) == 0x8100 || *(uint16 *)(cp+16) == 0x88a8 || *(uint16 *)(cp+16) == cpu_port_tpid)/*tagged packet*/
		{
			memmove(bp->data+4, bp->data, 12);
			skb_pull(bp, 4);
			cp = bp->data;
			tag0 = *(uint16 *)(cp+12);
		}
		else
			tag0 = 0xffff;/*oxffff will make vpm = 0*/
			
		vpm = Calc_VPM(tag0,cpu_port_tpid);

		if (0<tag1 && tag1<=RT63365_SWIC_PORTNUM)
		{
			if(g_port_reverse){			/* Lan port reverse */
				tag1 -= 1;
				if(0<=tag1 && tag1<4)
					tag1 = 3 - tag1;
				pvid = lan_port_map[ tag1 ];				
			}else
			{
#if defined(TCSUPPORT_CUC_C5_2P)
				uint16 temtag = 0;
				tag1 -= 1;				
				if(tag1 == 0) temtag = 3;
				else if(tag1 == 1) temtag = 0;
				else if(tag1 == 2) temtag = 1;
				else if(tag1 == 3) temtag = 2;
				tag1 = temtag;
				pvid = lan_port_map[ tag1 ];
				/*printk("Inset Tag: Tag1 is %d, pvid is %d\r\n", tag1, pvid);*/
#else
			        pvid = lan_port_map[ ((int)tag1-1) ]; /* get switch port id*/
#endif
			}
			if (!(0<=pvid && pvid<RT63365_SWIC_PORTNUM))
				printk("error: ethernet port cannot find the mapped switch port\n");
		}

#ifdef TCSUPPORT_AUTOBENCH
		if(flag)
			*(uint16 *)(cp+12) = (vpm << 8) + (tag1);
		else
#endif
#ifdef TCSUPPORT_INIC_HOST
		/* FTTdp iNIC packet */
		if(*(uint16 *)(cp+12) == ETH_P_ROM) {
			*(uint16 *)(cp+12) = (vpm << 8) + (tag1);
		}
		else
#endif
		{
			if( g_stag_to_vtag == 1)
			{
				if ((bp->mark & 0xf0000000) >= 0x10000000 && (bp->mark & 0xf0000000) <= 0x40000000)
					/*Don't set SA autolearning flag if the packet if from ethernet port to cpu*/
					/*    port then back to ethernet port.*/
					*(uint16 *)(cp+12) = (vpm << 8) + (1 << 6) + (1 << (pvid)) | (stag_up<<12);
				else
					*(uint16 *)(cp+12) = (vpm << 8) + (1 << (pvid)) | (stag_up<<12);
			}
			else
				*(uint16 *)(cp+12) = (port_mask) | (stag_up<<12);
		}

#ifdef TCSUPPORT_AUTOBENCH
		if(flag)
			*(uint16 *)(cp+14) = 0;
		else
#endif
		{
			if(vpm == 0)
			{
				if(g_stag_to_vtag == 1)
					*(uint16 *)(cp+14) = 1; /*lan vid = 1*/
				else
					*(uint16 *)(cp+14) = (pvid+1);	
			}
		}
	}
	else
	{
#ifdef WAN2LAN
		if (bp->mark & SKBUF_COPYTOLAN){
			port_mask = masko & 0xF;
#if defined(TCSUPPORT_CT_LOOPDETECT)		
			if(temp_mask=isDetectPacket(cp, bp->len)){
				port_mask = temp_mask;
				isLoopDetectPkt = 1;
			}
#endif
			port_mask = macEN7512STagRemap(port_mask); 	
		}
#endif
#if defined(TCSUPPORT_CT_LOOPDETECT)		
		if(!isLoopDetectPkt){
#endif
#if defined(TCSUPPORT_SEPERATED_ETH_ITF) 
        int eth_itf =0;
        int port_num =0;
        eth_itf = skbFromWhichVirtualEthItf(bp);
        port_num = lan_port_map[eth_itf];
		port_mask |= 1 << port_num;	
#endif		
#if defined(TCSUPPORT_CT_LOOPDETECT)
		}
#endif

#ifdef TC_CONSOLE_ENABLE
        if ((bp->mark & SKBUF_TCCONSOLE)){
            port_mask = 0x0f;
            port_mask = macEN7512STagRemap(port_mask);
        }
#endif
		rgmii_itf = interface_map[getTrafficType(bp)];
		/*
			there can be wan2lan,etherwan,single-port case.
			kernel will not add a tag to skb.we only check first tag.
			if packet is untag,add a stag.
		*/
		if(!(tag0 == 0x8100 || tag0 == 0x88a8 || tag0 == cpu_port_tpid))/*untag packet*/
		{
			bp = Insert_4Byte_to_Skb(bp);
			if(bp == NULL)
				return NULL;
			cp = bp->data;
			tag0 = 0xffff;/*used to set vpm = 0*/
		}
		vpm = Calc_VPM(tag0,cpu_port_tpid);	
#if defined(TCSUPPORT_WAN_ETHER)
		/*1  EtherWan case,need to test*/
		if(isComeFromWan(bp) == 1)
			*(uint16 *)(cp+12) = (vpm << 8) + (1<<wan_port_id)| (1<<15) | ((getPbitForSwitchQos(bp) & 0x7)<<12);
		else
#endif
			//add user pbit in stag for switch to do qos
			*(uint16 *)(cp+12) = (vpm << 8) + (port_mask) | (stag_up<<12) + (rgmii_itf << 10);  /* rgmii flag use DRM bit*/

		if(vpm == 0)
		{
#if defined(TCSUPPORT_WAN_ETHER)
			if(isComeFromWan(bp) == 1)
			{
				if(g_stag_to_vtag == 1)
					*(uint16 *)(cp+14) = 5;
				else
					*(uint16 *)(cp+14) = (wan_port_id+1);
			}
			else
#endif
				*(uint16 *)(cp+14) = 0;
		}
	}
#if defined(TCSUPPORT_GPON_MAPPING) && defined(TCSUPPORT_GPON_DOWNSTREAM_MAPPING)
	if(gpon_downstream_mapping_stag_hook)/*only change the Queue index here*/
		gpon_downstream_mapping_stag_hook(bp);
#if defined(TCSUPPORT_CT_DS_LIMIT)
	dataspeed_limit_queue_hook(bp); /* remark the queue index. */
#endif
#endif
    port_mask = (*(uint16 *)(cp+12)) & 0x3F;
    
    eth_led_tx_hook(port_mask);

    setPortMask(bp, port_mask);

	return bp;
}


inline int macEN7512STagRemove(struct sk_buff *bp)
{
	uint8 *cp=NULL;
	uint8 pvid = RT63365_SWIC_PORTNUM+1;
	uint16 sp_tag = 0;
	uint16 tci = 0;
	uint16 port_num = 0;
	
	uint16 tpid = 0;
	uint8 vpm = 0, stag_mode = STAG_REPLACE_MODE;
		
	cp = bp->data;
	sp_tag = *(uint16 *)(cp+12);
	tci = *(uint16 *)(cp+14);
#if defined(TCSUPPORT_LAN_VLAN) || defined(TCSUPPORT_LAN_VLAN_RANGE) || defined(TCSUPPORT_CDS) || defined(TCSUPPORT_CMCC)
	if(((sp_tag & 0x0300) >> 8) != 1){ // no 820.1q tag
	        bp->lan_vlan_tci_valid = 0;
	}else{
	        bp->lan_vlan_tci_valid = 1;
                bp->lan_vlan_tci = tci;
	}
#endif
	vpm = (sp_tag & 0x300) >> 8;

	port_num = (sp_tag & 0x7); /*switch port id*/

	if(0xfc78 & sp_tag){ /*there is no special-tag*/
		return 0;
	}

	if( g_stag_to_vtag == 1)
	{
		if((0<=port_num) && (port_num<RT63365_SWIC_PORTNUM)){
			pvid = switch_port_map[ port_num ]; /* get lan port id 			*/
			/*printk("Remove Tag: Origin pvid is %d port_num is %d\r\n", pvid, port_num);*/
#if defined(TCSUPPORT_CUC_C5_2P)
			if(0<=pvid && pvid<4) 
			{
				uint8 tempvid = 0;
				if(pvid == 3) tempvid = 0;
				else if(pvid == 0) tempvid = 1;
				else if(pvid == 1) tempvid = 2;
				else if(pvid == 2) tempvid = 3;

				pvid = tempvid;

				/*printk("Remove Tag: pvid is %d port_num is %d\r\n", pvid, port_num);*/
			}
#else
			if(g_port_reverse){
				if(0<=pvid && pvid<4)					/* Lan port reverse */
					pvid = 3 - pvid;
			}
#endif
		}
		else
			return -1;
	}
	else/*wifi offload case.do the same thing as 7510*/
	{
		/*1 wifi offload case,need to test*/
		if (0<tci && tci<=RT63365_SWIC_PORTNUM)
		{		
			pvid = switch_port_map[ ((int)tci-1) ];
		}
	}
    
	if(vport_enable
#if defined(TCSUPPORT_WAN_ETHER)
		&& (port_num != wan_port_id)
#endif
#if defined(TCSUPPORT_SEPERATED_ETH_ITF)
        && (0)
#endif
		)
	{
        if (0<=pvid && pvid<RT63365_SWIC_PORTNUM)
		    pvid++; /* translate to eth0.x (x)*/
    	else{	
    		return -1;
    	}
		/* add vlan_dev tag to send packet to eth0.x && 
		    restore tpid
		*/
		if(vpm != 0)
		{
		    if(skb_headroom(bp)<4){
                printk("Rx headroom less than 4 \n");
            }
			bp = Insert_4Byte_to_Skb(bp);
			if(bp == NULL){
				return -1;
			}
			cp = bp->data;
			switch(vpm)/*check vpm*/
			{
				case 0:/*untag*/
					break;
					
				case 1:/*8100*/
					*(uint16 *)(cp+16) = 0x8100;
					break;

				case 2:/*88a8*/
					*(uint16 *)(cp+16) = 0x88a8;
					break;

				case 3:/*defined by register*/
					tpid = Get_Tpid_By_Port(pvid);
					*(uint16 *)(cp+16) = tpid;
					break;

				default:
					return -1;

			}
		}
		*(uint16 *)(cp+12) = 0x8100;
		*(uint16 *)(cp+14) = pvid;		
	}
	else
	{
#if defined(TCSUPPORT_WAN_ETHER)
		if(port_num == wan_port_id)
			setComeFromWan(bp,1);
#endif

#if defined(TCSUPPORT_SEPERATED_ETH_ITF) 
        int eth_intf = 0;

        for(;eth_intf <= 4; eth_intf++)
        {
            if(lan_port_map[eth_intf] == port_num)
            {
                setSkbFromWhichVirtualEthItf(bp,eth_intf);
                break ;
            }
        }
#endif
        /*
        memmove(bp->data+4, bp->data, 12);
	    skb_pull(bp, 4);
        return 0;
*/
#if defined(TCSUPPORT_XPON_HAL_API_EXT)
		stag_mode = STAG_INSERT_MODE;
#endif
#if defined(TCSUPPORT_FH_ENV) && defined(TCSUPPORT_CPU_EN7521) && defined(TCSUPPORT_WAN_ETHER)
		if ( -1 == wan_port_id )
			stag_mode = STAG_INSERT_MODE;
		else
			stag_mode = STAG_REPLACE_MODE;
#endif

		if ( STAG_REPLACE_MODE == stag_mode )
		{
			/* no vlan_dev, so no need to remove or modify stag to original */
			switch(vpm)/*check vpm*/
			{
				case 0:/*untag*/
					memmove(bp->data+4, bp->data, 12);
					skb_pull(bp, 4);
					break;

				case 1:/*8100*/
					*(uint16 *)(cp+12) = 0x8100;
					break;

				case 2:/*88a8*/
					*(uint16 *)(cp+12) = 0x88a8;
					break;

				case 3:/*defined by register*/
					tpid = Get_Tpid_By_Port(pvid);
					*(uint16 *)(cp+12) = tpid;
					break;

				default:
					return -1;
							
			}
		}
		else /* special tag insert mode*/
		{
			if((vpm < 0) ||(vpm > 3))
				return -1;
			memmove(bp->data+4, bp->data, 12);
			skb_pull(bp, 4);
		}
	}	
    eth_led_rx_hook(port_num);
	return 0;
}/*end macEN7512STagRemove*/
#endif

int macRxPortEnable(int ChanID)
{
	int status = 0;
	macRxPortFlag = 1;
	switch (swicVendor) {

    	case SWIC_MT7530:	
    		status=macEN7512STagEnable(ChanID);
    		break;	

    	default:
    		break;
	}
	return status;
}

	
void phy_reset_esd(void){
	/* ----- Hardware reset Ehernet phy chip, this address is defined by h/w engineer ----- */
	ledTurnOn(LED_LAN_RESET);
	pause(100);
	/* ----- Wait for hardware reset completed ----- */
	ledTurnOff(LED_LAN_RESET);
	pause(600);
}

/*---------------------functions for ACL operations---------------------*/
#define ACL_ENABLE  1
#define ACL_DISABLE 0 

#define ETH_VLAN  				0x8100
#define PATTERN_MASK_ALL 		0xffff
#define PATTERN_MASK_VID 		0x0fff

#define UPSTREAM_PORT_MASK		0x1f

#define ACL_NO_UNICAST_PATTERN_MASK	0x0100	/*broadcast and multicast*/ 
#define ACL_NO_UNICAST_PATTERN		0x0100
#define ACL_NO_UNICAST_RULE_MASK	0x00000001

#define ACL_BROADCAST_PATTERN_MASK	0xffff 	/*broadcast*/
#define ACL_BROADCAST_PATTERN		0xffff
#define ACL_BROADCAST_RULE_MASK		0x0000000e

#define ACL_MULTICAST_PATTERN_MASK	0xff00 	/*multicast*/
#define ACL_MULTICAST_PATTERN		0x0100
#define ACL_MULTICAST_RULE_MASK		0x00000010

/*---------------------ACL wirte APIs---------------------*/
static inline uint32 acl_write_pattern(acl_pattern_t *acl_pattern_info, u8 index)
{
	uint32 acl_data1 = 0 ;
	uint32 acl_data2 = 0 ;

	acl_data1 = ( acl_pattern_info->pattern_mask << ACL_PATTERN_MASK_OFFSET ) | (acl_pattern_info->pattern) ;
	acl_data2 = ( acl_pattern_info->enable << ACL_PATTERN_ENABLE_OFFSET ) 				/*enable*/
				| ( acl_pattern_info->offset_range << ACL_PATTERN_OFFSETRANGE_OFFSET )  /*offset type*/
				| ( acl_pattern_info->port_map << ACL_PATTERN_PORTMAP_OFFSET )   		/*switch port mask*/
				| ( acl_pattern_info->word_offset << ACL_PATTERN_WORDOFFSET_OFFSET ) 	/*offset: word*/
				| ( acl_pattern_info->compare_mode ) ;									/*compare mode*/

	write_reg_word( VAWD1 , acl_data1 );
	write_reg_word( VAWD2 , acl_data2 );
	write_reg_word( VTCR , ACL_PATTERN_WRITE(index) );
	
	return 1 ;
}

static inline uint32 acl_write_mask(u32 acl_mask, u8 index)
{
	uint32 acl_data1 = acl_mask ;
	uint32 acl_data2 = 0 ;

	write_reg_word( VAWD1 , acl_data1 );
	write_reg_word( VAWD2 , acl_data2 );
	write_reg_word( VTCR , ACL_MASK_WRITE(index) );

	return 1 ;
}

static inline uint32 acl_write_rule(acl_control_1_t *acl_ctl_info_1, acl_control_2_t *acl_ctl_info_2 , u8 index)
{
	uint32 acl_data1 = 0 ;
	uint32 acl_data2 = 0 ;

	acl_data1 = ( acl_ctl_info_1->acl_mang << ACL_CTL1_MANG_OFFSET )
				| ( acl_ctl_info_1->int_en << ACL_CTL1_INT_EN_OFFSET )
				| ( acl_ctl_info_1->acl_cnt_en << ACL_CTL1_CNT_EN_OFFSET )
				| ( acl_ctl_info_1->cnt_idx << ACL_CTL1_CNT_INDEX_OFFSET )
				| ( acl_ctl_info_1->vlan_port_en << ACL_CTL1_VLAN_PORT_EN_OFFSET )
				| ( acl_ctl_info_1->da_swap << ACL_CTL1_DA_SWAP_OFFSET )
				| ( acl_ctl_info_1->sa_swap << ACL_CTL1_SA_SWAP_OFFSET )
				| ( acl_ctl_info_1->ppp_rm << ACL_CTL1_PPP_RM_OFFSET )
				| ( acl_ctl_info_1->lky_vlan << ACL_CTL1_LKY_VLAN_OFFSET )
				| ( acl_ctl_info_1->eg_tag << ACL_CTL1_EG_TAG_OFFSET )
				| ( acl_ctl_info_1->port << ACL_CTL1_PORT_OFFSET )
				| ( acl_ctl_info_1->port_en << ACL_CTL1_PORT_EN_OFFSET )
				| ( acl_ctl_info_1->pri_user << ACL_CTL1_PRI_USER_OFFSET )
				| ( acl_ctl_info_1->mir_en << ACL_CTL1_MIR_EN_OFFSET )
				| ( acl_ctl_info_1->port_fw) ;
	acl_data2 = ( acl_ctl_info_2->acl_class_idx << ACL_CTL2_CLASS_IDX_OFFSET )
				| ( acl_ctl_info_2->rate_mode << ACL_CTL2_RATE_MODE_OFFSET )
				| ( acl_ctl_info_2->drop_pcd_g << ACL_CTL2_DROP_PCD_G_OFFSET )
				| ( acl_ctl_info_2->drop_pcd_y << ACL_CTL2_DROP_PCD_Y_OFFSET )
				| ( acl_ctl_info_2->drop_pcd_r << ACL_CTL2_DROP_PCD_R_OFFSET )
				| ( acl_ctl_info_2->class_slr << ACL_CTL2_CLASS_SLR_OFFSET )
				| ( acl_ctl_info_2->class_slr_sel << ACL_CTL2_CLASS_SLR_SEL_OFFSET )
				| ( acl_ctl_info_2->drop_pcd_sel ) ;

	write_reg_word( VAWD1 , acl_data1 );
	write_reg_word( VAWD2 , acl_data2 );
	write_reg_word( VTCR , ACL_RULE_WRITE(index) );
	
	return 1 ;
}

static inline uint32 acl_write_rate(acl_rate_t *acl_rate_info , u8 index)
{
	uint32 acl_data1 = 0 ;
	uint32 acl_data2 = 0 ;

	acl_data1 = ( acl_rate_info->rate_en << ACL_RATE_EN_OFFSET )
				| ( acl_rate_info->rate );
	
	write_reg_word( VAWD1 , acl_data1 );
	write_reg_word( VAWD2 , acl_data2 );
	write_reg_word( VTCR , ACL_RATE_WRITE(index) );
	return 1;
}

/*--------set upstream ratelimit for none-unicast, broadcast and multicast--------*/
/*mode 0 : for none-unicast (broadcast & multicast)  .  	use : pattern 0 , 	 rule 0 */
/*mode 1 : for broadcast .  							use : pattern 1~3 , rule 1 */
/*mode 2 : for multicast  .  							use : pattern 4 , 	 rule 4 */
/*if mode 0 is set , mode 1 and mode 2 will not work*/
uint32 acl_set_upstream_ratelimit_for_special(u32 rate , u8 enable , u8 mode)
{
	int i ;
	uint32 value = 0 ;
	uint32 acl_rule_index = 0 ;
	uint32 acl_rule_mask = 0 ;
	
	acl_pattern_t acl_pattern_info ;
	acl_control_1_t acl_ctl_info_1 ;
	acl_control_2_t acl_ctl_info_2 ;
	acl_rate_t acl_rate_info ;

	if( PATTERN_ALL_NUM > ACL_MAX_NUM )
	{
		printk("ACL FAIL:patten number is bigger than ACL MAX NUM!\n");
		return 0 ;
	}
	/*assure: set port 0~4 join ACL rule*/
	for( i = 0 ; i < 5 ; i++ )
	{
		value = read_reg_word(GSW_PORT_CTL(i));
		value = value | (1 << 10) ;
		write_reg_word(GSW_PORT_CTL(i) , value);
	}
	/*calculate the rule index*/
	switch( mode )
	{
		case ACL_SET_MODE_NO_UNICAST :
			acl_rule_index = 0 ;
			acl_rule_mask = ACL_NO_UNICAST_RULE_MASK ;
			break ;
		case ACL_SET_MODE_BROADCAST :
			acl_rule_index = 1 ;
			acl_rule_mask = ACL_BROADCAST_RULE_MASK ;
			break ;
		case ACL_SET_MODE_MULTICAST :
			acl_rule_index = 4 ;
			acl_rule_mask = ACL_MULTICAST_RULE_MASK ;
			break ;
		default :
			printk("ACL FAIL:error mode!\n");
			return 0 ;
	}
	/*init the struct*/
	memset(&acl_pattern_info, 0, sizeof(acl_pattern_t));
	memset(&acl_ctl_info_1, 0, sizeof(acl_control_1_t));
	memset(&acl_ctl_info_2, 0, sizeof(acl_control_2_t));
	memset(&acl_rate_info, 0, sizeof(acl_rate_t));

	/*1. write pattern*/
	if( mode == ACL_SET_MODE_BROADCAST )/*braodcast set*/
	{
		for( i = 0 ; i < 3 ; i++ )
		{
			if( enable )
			{
				acl_pattern_info.pattern_mask = ACL_BROADCAST_PATTERN_MASK ;
				acl_pattern_info.pattern = ACL_BROADCAST_PATTERN ;

				acl_pattern_info.enable = ACL_ENABLE & 0x1 ;			/*enable*/
				acl_pattern_info.offset_range = 0 ;						/*0 for mac header*/
				acl_pattern_info.port_map = UPSTREAM_PORT_MASK & 0xff ; /*1f: for port0~port4 , only upstream*/
				acl_pattern_info.word_offset = i & 0x7f ;				/*offset: word , from dst mac*/
				acl_pattern_info.compare_mode = 0 ;						/*compare mode*/
			}
			acl_write_pattern( &acl_pattern_info , (acl_rule_index + i) ); /*braodcast: use pattern 1~3*/
		}
	}
	else
	{
		if( enable )
		{
			if( mode == ACL_SET_MODE_NO_UNICAST )
			{
				acl_pattern_info.pattern_mask = ACL_NO_UNICAST_PATTERN_MASK ;
				acl_pattern_info.pattern = ACL_NO_UNICAST_PATTERN ;
			}
			else if( mode == ACL_SET_MODE_MULTICAST )
			{
				acl_pattern_info.pattern_mask = ACL_MULTICAST_PATTERN_MASK ;
				acl_pattern_info.pattern = ACL_MULTICAST_PATTERN ;
			}
	
			acl_pattern_info.enable = ACL_ENABLE & 0x1 ;			/*enable*/
			acl_pattern_info.offset_range = 0 ;						/*0 for mac header*/
			acl_pattern_info.port_map = UPSTREAM_PORT_MASK & 0xff ; /*1f: for port0~port4 , only upstream*/
			acl_pattern_info.word_offset = 0 ;						/*offset: word , from dst mac*/
			acl_pattern_info.compare_mode = 0 ;						/*compare mode*/
		}
		
		acl_write_pattern( &acl_pattern_info , acl_rule_index );		/*no-unicast: pattern 0 ; multicast:  pattern 4*/
	}

	/*2. write acl mask*/
	acl_write_mask( acl_rule_mask , acl_rule_index ) ;				/*no-unicast: rule 0 ; braodcast: rule 1 ; multicast:  rule 4*/

	/*3. write control*/
	acl_ctl_info_1.acl_cnt_en = ACL_ENABLE & 0x1 ;
	acl_ctl_info_2.rate_mode = 0 ; /*0: rate 1:trtcm*/
	acl_write_rule(&acl_ctl_info_1 , &acl_ctl_info_2 , acl_rule_index);

	/*4. wirte ratelimit*/
	acl_rate_info.rate_en = ACL_ENABLE & 0x1 ;
	acl_rate_info.rate = (rate>>6) & 0x3fff ;/*unit: 64Kbps , set unit: 1Kbps*/
	acl_write_rate(&acl_rate_info , acl_rule_index);
	
	return 1 ;
}

/*--------set upstream ratelimit for dmac or smac--------*/
/*mode 3 : for dmac set  .  pattern use from: skip PATTERN_FOR_SPECIAL_NUM*/
/*mode 4 : for smac set  .  pattern use from: skip (PATTERN_FOR_SPECIAL_NUM+PATTERN_FOR_DMAC_NUM)*/
/*index: from 0 ~ ...*/
uint32 acl_set_upstream_ratelimit_for_mac(u8 *mac , uint32 rate , u8 enable, u8 index , u8 mode)
{
	int i ;
	uint32 value ;
	uint32 acl_rule_index ;
	uint32 acl_rule_mask = 0 ;
	
	acl_pattern_t acl_pattern_info ;
	acl_control_1_t acl_ctl_info_1 ;
	acl_control_2_t acl_ctl_info_2 ;
	acl_rate_t acl_rate_info ;

	if( PATTERN_ALL_NUM > ACL_MAX_NUM )
	{
		printk("ACL FAIL:patten number is bigger than ACL MAX NUM!\n");
		return 0 ;
	}
	/*assure: set port 0~4 join ACL rule*/
	for( i = 0 ; i < 5 ; i++ )
	{
		value = read_reg_word(GSW_PORT_CTL(i));
		value = value | (1 << 10) ;
		write_reg_word(GSW_PORT_CTL(i) , value);
	}
	/*calculate the pattern index and rule index*/
	switch(mode)
	{
		case ACL_SET_MODE_DMAC :
			if( index >= DMAC_RULE_NUM )
			{
				printk("ACL FAIL:dmac rule number is too big!\n");
				return 0 ;
			}
			acl_rule_index = PATTERN_FOR_SPECIAL_NUM + index * 3 ;
			break ;
		case ACL_SET_MODE_SMAC :
			if( index >= SMAC_RULE_NUM )
			{
				printk("ACL FAIL:smac rule number is too big!\n");
				return 0 ;
			}
			acl_rule_index = PATTERN_FOR_SPECIAL_NUM + PATTERN_FOR_DMAC_NUM + index * 3 ;
			break ;
		default:
			printk("ACL FAIL:error mode!\n");
			return 0 ;
	}
	/*init the struct*/
	memset(&acl_pattern_info, 0, sizeof(acl_pattern_t));
	memset(&acl_ctl_info_1, 0, sizeof(acl_control_1_t));
	memset(&acl_ctl_info_2, 0, sizeof(acl_control_2_t));
	memset(&acl_rate_info, 0, sizeof(acl_rate_t));

	/*1. write pattern*/
	for( i = 0 ; i < 3 ; i++ )
	{
		if(enable)
		{
			acl_pattern_info.pattern_mask = PATTERN_MASK_ALL ;
			acl_pattern_info.pattern = (mac[i*2] << 8) | mac[i*2+1];

			acl_pattern_info.enable = ACL_ENABLE & 0x1 ;						/*enable*/
			acl_pattern_info.offset_range = 0 ;									/*0 for mac header*/
			acl_pattern_info.port_map = UPSTREAM_PORT_MASK & 0xff ; 			/*1f: for port0~port4 , only upstream*/
			acl_pattern_info.word_offset = ((mode==ACL_SET_MODE_DMAC)?0:3)+i ;	/*offset: word.dmac: skp 0 ; smac: skip 3 words*/
			acl_pattern_info.compare_mode = 0 ;									/*compare mode*/																/*compare mode*/
		}
		acl_write_pattern( &acl_pattern_info , (acl_rule_index + i) );
		/*record acl_mask*/
		acl_rule_mask = acl_rule_mask | ( 1 << (acl_rule_index + i) );
	}

	/*2. write acl mask*/
	acl_write_mask(acl_rule_mask , acl_rule_index) ;

	/*3. write control*/
	acl_ctl_info_1.acl_cnt_en = ACL_ENABLE & 0x1 ;
	acl_ctl_info_2.rate_mode = 0 ; /*0: rate 1:trtcm*/
	acl_write_rule(&acl_ctl_info_1 , &acl_ctl_info_2 , acl_rule_index);

	/*4. wirte ratelimit*/
	acl_rate_info.rate_en = ACL_ENABLE & 0x1 ;
	acl_rate_info.rate = (rate>>6) & 0x3fff ;/*unit: 64Kbps , set unit: 1Kbps*/
	acl_write_rate(&acl_rate_info , acl_rule_index);

	return 1 ;
}

uint32 acl_set_upstream_ratelimit_for_vlan(uint32 vlan , uint32 rate ,u8 enable, u8 index)
{
	int i ;
	uint32 value ;
	uint32 acl_rule_index ;
	uint32 acl_rule_mask = 0 ;

	acl_pattern_t acl_pattern_info ;
	acl_control_1_t acl_ctl_info_1 ;
	acl_control_2_t acl_ctl_info_2 ;
	acl_rate_t acl_rate_info ;


	if( PATTERN_ALL_NUM > ACL_MAX_NUM )
	{
		printk("ACL FAIL:patten number is bigger than ACL MAX NUM!\n");
		return 0 ;
	}
	/*assure: set port 0~4 join ACL rule*/
	for( i = 0 ; i < 5 ; i++ )
	{
		value = read_reg_word(GSW_PORT_CTL(i));
		value = value | (1 << 10) ;
		write_reg_word(GSW_PORT_CTL(i) , value);
	}

	/*calculate the pattern index and rule index*/
	if(index >= VLAN_RULE_NUM)
	{
		printk("ACL FAIL:vlan rule number is too big!\n");
		return 0 ;
	}
	acl_rule_index = PATTERN_FOR_SPECIAL_NUM + PATTERN_FOR_DMAC_NUM + PATTERN_FOR_SAMC_NUM ;
	/*init the struct*/
	memset(&acl_pattern_info, 0, sizeof(acl_pattern_t));
	memset(&acl_ctl_info_1, 0, sizeof(acl_control_1_t));
	memset(&acl_ctl_info_2, 0, sizeof(acl_control_2_t));
	memset(&acl_rate_info, 0, sizeof(acl_rate_t));

	/*1. write eth_type pattern*/
	acl_pattern_info.pattern_mask = PATTERN_MASK_ALL ;
	acl_pattern_info.pattern = ETH_VLAN ;

	acl_pattern_info.enable = ACL_ENABLE & 0x1 ;						/*enable*/
	acl_pattern_info.offset_range = 0 ;									/*0 for mac header*/
	acl_pattern_info.port_map = UPSTREAM_PORT_MASK & 0xff ; 			/*1f: for port0~port4 , only upstream*/
	acl_pattern_info.word_offset = 6 ;									/*offset: 6 words, skip dmac and smac*/
	acl_pattern_info.compare_mode = 0 ;									/*compare mode*/
	acl_write_pattern(&acl_pattern_info , acl_rule_index) ;

	/*2.write vlan pattern*/
	memset(&acl_pattern_info, 0, sizeof(acl_pattern_t));
	if( enable )
	{
		acl_pattern_info.pattern_mask = PATTERN_MASK_VID ;
		acl_pattern_info.pattern = vlan & 0x0fff ;

		acl_pattern_info.enable = ACL_ENABLE & 0x1 ;						/*enable*/
		acl_pattern_info.offset_range = 0 ;									/*0 for mac header*/
		acl_pattern_info.port_map = UPSTREAM_PORT_MASK & 0xff ; 			/*1f: for port0~port4 , only upstream*/
		acl_pattern_info.word_offset = 7 ;									/*offset: 7 words, skip dmac and smac ,and eth type*/
		acl_pattern_info.compare_mode = 0 ;									/*compare mode*/
	}
	acl_write_pattern(&acl_pattern_info , (acl_rule_index+1+index)) ;

	/*3. write acl mask*/
	acl_rule_mask = (1 << acl_rule_index) | (1 << (acl_rule_index + 1 + index)) ;
	acl_write_mask(acl_rule_mask , (acl_rule_index+1+index)) ;

	/*4. write control*/
	acl_ctl_info_1.acl_cnt_en = ACL_ENABLE & 0x1 ;
	acl_ctl_info_2.rate_mode = 0 ; /*0: rate 1:trtcm*/
	acl_write_rule(&acl_ctl_info_1 , &acl_ctl_info_2 , (acl_rule_index + 1 + index));

	/*5. wirte ratelimit*/
	acl_rate_info.rate_en = ACL_ENABLE & 0x1 ;
	acl_rate_info.rate = (rate>>6) & 0x3fff ;/*unit: 64Kbps , set unit: 1Kbps*/
	acl_write_rate(&acl_rate_info , (acl_rule_index + 1 + index));

	return 1;
}


int eth_esd_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;

  	index += sprintf(buf+index, "%d\n", esd_has_reset);
	CHK_BUF();

	*eof = 1;
	/*printk("esd_read_value %d\n",esd_has_reset);*/

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0) 
		index = 0;
	if (index>count) 
		index = count;
	return index;
}

int eth_esd_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32];
	int val;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	val = simple_strtoul(val_string, NULL, 10);	
	/*printk("esd_write_value %d\n",val);*/
	if(val == 1)
		esd_has_reset = 0;
	else{
		phy_reset_esd();
		esd_has_reset = 1;
		/*do_reset_test	*/
	}	

	return count;
}

/************************************************************************
    Funtcion:       periodChk
    Description:    
    Calls:
    Called by:      eth_monitor
    Input:
    Output:
    Return:
    Others:     
************************************************************************/
void switch_esd_check(void)
{
}



/************************************************************************
    Funtcion:       periodChk
    Description:    
    Calls:
    Called by:      eth_monitor
    Input:
    Output:
    Return:
    Others:     
************************************************************************/
void periodChk(void){
    switch_esd_check();
    //  phy_api_monitor();  phy_api_7512
}


void trgmii_interface_init(void){
	uint32 reg;
		
	// temp workaround for avoiding MT7530 7 seconds auto-powerdown
	gswPbusWrite(0x30F0, 0x00001E02);   // CKGCR : clear bit 0


	//Enable TRGMII
	gswPbusWrite(0x7808, 0);
	gswPbusWrite(0x7804, 0x01017e8f);
	gswPbusWrite(0x7808, 1);
	write_reg_word(GSW_BASE+ 0x7808,0);
	write_reg_word(GSW_BASE+ 0x7804,0x01017e8f);
	write_reg_word(GSW_BASE+ 0x7808,1);

	//Setup Clock 200M
	mdio_cl22_write(0,0x1f,0x404,0x1d00); /* 362.5MHz*/
	mdio_cl22_write(0,0x1f,0x409,0x57);
	mdio_cl22_write(0,0x1f,0x40a,0x57);
	
	mdio_cl22_write(12,0x1f,0x404,0x1d00); /* 362.5MHz*/
	mdio_cl22_write(12,0x1f,0x409,0x57);
	mdio_cl22_write(12,0x1f,0x40a,0x57);
	
	mdio_cl22_write(0,0x1f,0x403,0x1800);
	mdelay(5); //rise delay time
	mdio_cl22_write(0,0x1f,0x403,0x1c00);
	mdio_cl22_write(0,0x1f,0x401,0xc020);
	mdio_cl22_write(0,0x1f,0x406,0xa030);
	mdio_cl22_write(0,0x1f,0x406,0xa038);
	mdio_cl22_write(0,0x1f,0x410,0x3);
	
	mdio_cl22_write(12,0x1f,0x403,0x1800);
	mdelay(5); // rise delay time
	mdio_cl22_write(12,0x1f,0x403,0x1c00);
	mdio_cl22_write(12,0x1f,0x401,0xc020);
	mdio_cl22_write(12,0x1f,0x406,0xa030);
	mdio_cl22_write(12,0x1f,0x406,0xa038);
	mdio_cl22_write(12,0x1f,0x410,0x3);
	mdelay(50);


	/* TXEN Disable and Link Down */
	reg = (IPG_CFG_64BITS<<IPG_CFG_PN_SHIFT) | MAC_MODE_PN | FORCE_MODE_PN | 
		MAC_RX_EN_PN | BKOFF_EN_PN | BACKPR_EN_PN | 
		(PN_SPEED_1000M<<FORCE_SPD_PN_SHIFT) | FORCE_DPX_PN ;

	write_reg_word(GSW_PMCR(5), reg);
	gswPbusWrite(0x3600, reg);
	mdelay(5); // rise delay time

	//Reset TRGMII Tx/Rx
	reg = gswPbusRead (0x7a40);
	gswPbusWrite(0x7a40, (reg | (1<<28)));
	mdelay(5); // rise delay time
	gswPbusWrite(0x7a40, (reg & (~(1<<28))));


	reg =  read_reg_word(GSW_BASE + 0x7a40);
	write_reg_word(GSW_BASE + 0x7a40, (reg | (1<<28)));
	mdelay(5); //rise delay time
	write_reg_word(GSW_BASE + 0x7a40, (reg & (~(1<<28))));	


	reg = gswPbusRead (0x7a00);
	gswPbusWrite(0x7a00, (reg | (1<<31)));

	reg =  read_reg_word(GSW_BASE + 0x7a00);
	write_reg_word(GSW_BASE + 0x7a00, (reg | (1<<31)));

	//Deduce MT7530 P6 Tx Driving Strength	
	gswPbusWrite(0x7a54, 0x88);
	gswPbusWrite(0x7a5c, 0x88);
	gswPbusWrite(0x7a64, 0x88);
	gswPbusWrite(0x7a6c, 0x88);
	gswPbusWrite(0x7a74, 0x88);
	gswPbusWrite(0x7a7c, 0x77);
	write_reg_word(GSW_BASE + 0x7a54, 0x88);
	write_reg_word(GSW_BASE + 0x7a5c, 0x88);
	write_reg_word(GSW_BASE + 0x7a64, 0x88);
	write_reg_word(GSW_BASE + 0x7a6c, 0x88);
	write_reg_word(GSW_BASE + 0x7a74, 0x88);
	write_reg_word(GSW_BASE + 0x7a7c, 0x77);
			
	//Enable port
 	gswPbusWrite(0x7830, 1);
  	write_reg_word(GSW_BASE+ 0x7830,0x1);
	mdelay(5); //rise delay time
	
	reg = gswPbusRead (0x7a00);
	gswPbusWrite(0x7a00, (reg & (~(1<<31))));


	reg =  read_reg_word(GSW_BASE + 0x7a00);
	write_reg_word(GSW_BASE + 0x7a00, (reg & (~(1<<31))));

	/* TXEB Disable and Link up */
	reg = (IPG_CFG_64BITS<<IPG_CFG_PN_SHIFT) | MAC_MODE_PN | FORCE_MODE_PN | 
		 MAC_RX_EN_PN | BKOFF_EN_PN | BACKPR_EN_PN | 
		(PN_SPEED_1000M<<FORCE_SPD_PN_SHIFT) | FORCE_DPX_PN | FORCE_LNK_PN;

	write_reg_word(GSW_PMCR(5), reg);
	gswPbusWrite(0x3600, reg);
	
	
  //Disable internal FE Phy
	reg = read_reg_word(GSW_BASE + 0x7840);
	write_reg_word(GSW_BASE + 0x7840, (reg | 0xf));	



	//Disable Port 5/Port 6 SA Learning
	write_reg_word(GSW_PSC(5), 0xfff10);
	write_reg_word(GSW_PSC(6), 0xfff10);

	/* set port 5 as 1000Mbps, FC off */
	reg = (IPG_CFG_64BITS<<IPG_CFG_PN_SHIFT) | MAC_MODE_PN | FORCE_MODE_PN | 
		MAC_TX_EN_PN | MAC_RX_EN_PN | BKOFF_EN_PN | BACKPR_EN_PN | 
		(PN_SPEED_1000M<<FORCE_SPD_PN_SHIFT) | FORCE_DPX_PN | FORCE_LNK_PN;

    write_reg_word(GSW_PMCR(5), reg);
	gswPbusWrite(0x3600, reg);

	return;
}

void switch_vlan_setting(void)
{
    int i;

    for(i=0; i<=6; i++)
	{
        switch_reg_write(GSW_PVC(i), 0x81008100);
        switch_reg_write(GSW_PPBV1(i), 0x10001+i);
        switch_reg_write(GSW_PCR(i), 0xff0003);
    }
    return;
}
void rgmii_interface_init(void){
	uint32 reg;
	
    /* set rgmii interface into rgmii mode */
    reg = read_reg_word(0xBFA2015C)| 0x1;
    write_reg_word(0xBFA2015C,reg );
}

static void acl_for_rgmii_itf(void){
	acl_pattern_t acl_pattern_info ;
	acl_control_1_t acl_ctl_info_1 ;
	acl_control_2_t acl_ctl_info_2 ;

    acl_pattern_info.pattern_mask = 0x0400; /* only care DRM bit*/
    acl_pattern_info.pattern = 0x0400 ;

    acl_pattern_info.enable = ACL_ENABLE;   /*enable*/
    acl_pattern_info.offset_range = 0 ;     /*0 for mac header*/
    acl_pattern_info.port_map = 0x40;       /*0x40: for port6, only downstream*/
    acl_pattern_info.word_offset = 6 ;      /*offset: word , from dst mac*/
    acl_pattern_info.compare_mode = 0 ;     /*compare mode*/
    acl_write_pattern(&acl_pattern_info, RGMII_TRAFFIC);

    acl_write_mask(1<<RGMII_TRAFFIC, RGMII_TRAFFIC);

    acl_ctl_info_1.port_en = ENABLE;
    acl_ctl_info_1.port = 0x1;       /* rgmii forwrd to port0*/
    acl_write_rule(&acl_ctl_info_1, &acl_ctl_info_2, RGMII_TRAFFIC);

    
    acl_pattern_info.pattern_mask = 0x0400; /* only care DRM bit*/
    acl_pattern_info.pattern = 0x0 ;

    acl_pattern_info.enable = ACL_ENABLE;   /*enable*/
    acl_pattern_info.offset_range = 0 ;     /*0 for mac header*/
    acl_pattern_info.port_map = 0x40;       /*0x40: for port6, only downstream*/
    acl_pattern_info.word_offset = 6 ;      /*offset: word , from dst mac*/
    acl_pattern_info.compare_mode = 0 ;     /*compare mode*/
    acl_write_pattern(&acl_pattern_info, TRGMII_TRAFFIC);

    acl_write_mask(1<<TRGMII_TRAFFIC, TRGMII_TRAFFIC);

    acl_ctl_info_1.port_en = ENABLE;
    acl_ctl_info_1.port = 0x20;       /* trgmii forwrd to port5*/
    acl_write_rule(&acl_ctl_info_1, &acl_ctl_info_2, TRGMII_TRAFFIC);
}
/************************************************************************
    Funtcion:       macSetGSW
    Description:    
    Calls:
    Called by:      macDrvRegInit
    Input:
    Output:
    Return:
    Others:     
************************************************************************/
void macSetGSW(macAdapter_t *mac_p)
{
	uint32 reg;
    int phy_add_start=0,phy_add_end=0,phy_add;

#if defined(WAN2LAN) || defined(CONFIG_TC3162_ADSL)
#if !(defined(TC2031_SUPPORT) || defined(TCSUPPORT_FTTDP_V2))
        macRxPortEnable(0);
#endif
#endif

	/* set port 6 as 1Gbps, FC off */
	reg = (IPG_CFG_SHORT<<IPG_CFG_PN_SHIFT) | MAC_MODE_PN | FORCE_MODE_PN |
		MAC_TX_EN_PN | MAC_RX_EN_PN | BKOFF_EN_PN | BACKPR_EN_PN |
		(PN_SPEED_1000M<<FORCE_SPD_PN_SHIFT) |
		FORCE_DPX_PN;
	write_reg_word(GSW_PMCR(6), reg);

#ifdef TCSUPPORT_INIC_CLIENT	
	/* set port 5 as 1Gbps, FC on */
	write_reg_word(GSW_PMCR(5), reg);
#endif
	/* set cpu port as port 6 */
#if !defined(TCSUPPORT_XPON_HAL_API_EXT) && !defined(TCSUPPORT_XPON_HAL_API) 
	reg = (0xff<<MFC_BC_FFP_SHIFT) | (0xff<<MFC_UNM_FFP_SHIFT) | (0xff<<MFC_UNU_FFP_SHIFT) |
			MFC_CPU_EN	| (6<<MFC_CPU_PORT_SHIFT);
#endif
	switch_reg_write(GSW_MFC, reg);

	
	/* check if FPGA */
	if (isFPGA) {
#ifndef MT7505_PQA_SWITCH_TEST
		/*decrease mdc/mdio clock*/
		reg = read_reg_word(GSW_CFG_PPSC);
		reg &= ~((1<<6) | (1<<7));
		write_reg_word(GSW_CFG_PPSC, reg);

		/* auto polling enable */
		reg = read_reg_word(GSW_CFG_PPSC);
		reg &= ~(0x7F << 24);	/* for FPGA external PHY, always use auto polling*/
		reg &= ~(PHY_END_ADDR | PHY_ST_ADDR);
		// phy_api_phy_ouid    phy_api_7512
        if ((tcMiiStationRead(0, 2) == 0x4d) && (tcMiiStationRead(0, 3) == 0xd072)){	/* MT7510 FPGA mainboard built-in 2 port*/
            reg |= 0x3F << 24;	/* for FPGA external PHY, always use auto polling*/
            phy_add_start =0;
            phy_add_end   =6;
        }
        // phy_api_phy_ouid    phy_api_7512
        else if ((tcMiiStationRead(1, 2) == 0x4d) && (tcMiiStationRead(1, 3) == 0xd072)){	/* MT7510 FPGA mainboard built-in 2 port*/
            if (isEN7526c) {
                reg |= 0x7 << 24;	/* for FPGA external PHY, always use auto polling*/
                phy_add_start =0;
                phy_add_end   =2;
            }else{
                reg |= 0x3 << 24;	/* for FPGA external PHY, always use auto polling*/
                phy_add_start =1;
                phy_add_end   =2;
            }
        }else if ((tcMiiStationRead(2, 2) == 0x4d) && (tcMiiStationRead(2, 3) == 0xd072)){	/* MT7510 FPGA mainboard built-in 2 port*/
            if (isEN7526c) {
                reg |= 0x7 << 24;   /* for FPGA external PHY, always use auto polling*/
                phy_add_start =1;
                phy_add_end   =3;
            }else {
                reg |= 0x3 << 24;   /* for FPGA external PHY, always use auto polling*/
                phy_add_start =2;
                phy_add_end   =3;
            }
        }
        // phy_api_phy_ouid    phy_api_7512
        else if ((tcMiiStationRead(4, 2) == 0xf) && (tcMiiStationRead(4, 3) == 0xc6c2)){	/* MT7510 FPGA daughter board extra 4 port*/
            reg |= 0x3F << 24;	/* for FPGA external PHY, always use auto polling*/
            phy_add_start =2;
            phy_add_end =7;
        }
	reg |= (phy_add_end<<PHY_END_ADDR_SHIFT) | (phy_add_start<<PHY_ST_ADDR_SHIFT);
        write_reg_word(GSW_CFG_PPSC, reg);
        /* disable phy 1000M capability, fpga not support gmii*/
        for(phy_add = phy_add_start;phy_add<=phy_add_end;phy_add++){
            // phy_api_phy_ability    phy_api_7512
            reg = tcMiiStationRead(phy_add, 0x9);
            reg &= ~((1<<9) | 1<<8);
            tcMiiStationWrite(phy_add, 0x9, reg);
        }
#endif
	}else{
        /* Sideband signal error for Port 3, which need the auto polling */
        /* this issue only on MT7510/MT7520 */
        /* EN7512 bugfix, so default use sideband*/
        switch_reg_write(GSW_CFG_PPSC, 0x007f8c08);
	}
		/*tag-pri-Q  mapping, setup for QoS*/
		/*PEM1: UP1-> Q1  UP0-> Q0*/
		reg = switch_reg_read(GSW_BASE+0x48);
		reg &= ~(0x7<<8); /*UP0 -> Queue0*/
		reg &= ~(0x7<<24); /*UP1 -> Queue1*/
		reg |= (0x1<<24); 
		switch_reg_write ((GSW_BASE+0x48), reg);	

		/*PEM1: UP3-> Q3  UP2-> Q2*/
		reg = switch_reg_read(GSW_BASE+0x4c);
		reg &= ~(0x7<<8); /*UP2 -> Queue2*/
		reg |= (0x2<<8);
		reg &= ~(0x7<<24); /*UP3 -> Queue3 */
		reg |= (0x3<<24); 
		switch_reg_write ((GSW_BASE+0x4c), reg);

		/*PEM1: UP5-> Q5  UP4-> Q4*/
		reg = switch_reg_read(GSW_BASE+0x50);
		reg &= ~(0x7<<8); /*UP4 -> Queue4*/
		reg |= (0x4<<8);
		reg &= ~(0x7<<24); /*UP5 -> Queue5 */
		reg |= (0x5<<24); 
		switch_reg_write ((GSW_BASE+0x50), reg);

		/*PEM1: UP7-> Q7  UP6-> Q6*/
		reg = switch_reg_read(GSW_BASE+0x54);
		reg &= ~(0x7<<8); /*UP6 -> Queue6*/
		reg |= (0x6<<8);
		reg |= (0x7<<24); /*UP7 -> Queue7 */
		switch_reg_write ((GSW_BASE+0x54), reg);

#if defined(TCPHY_SUPPORT)
	if(!(isFPGA)){
		/* change default values in case that EEE is on */

		/*LPI_TXIDLE_THD*/
		reg=read_reg_word ((GSW_BASE+0x30f0));
		reg &= (~(0xff<<8));
		reg |= (0xff<<8);
		write_reg_word ((GSW_BASE+0x30f0), reg);
		/*WAKEUP_TIME_1000_P4*/
		reg=read_reg_word ((GSW_BASE+0x3404));
		reg &= (~(0xff<<24));
		reg |= (0x31<<24);
		write_reg_word ((GSW_BASE+0x3404), reg);		
	}
#endif

    if(use_ext_switch){
        trgmii_interface_init();
#ifndef TCSUPPORT_AUTOBENCH
        rgmii_interface_init();
        acl_for_rgmii_itf();
        
#endif
#ifdef TCSUPPORT_MT7530_SWITCH_API
        //macMT7530doP6Cal();
#endif
	}
#ifdef TCSUPPORT_FWC_ENV
    switch_vlan_setting();
#endif
}

/*_____________________________________________________________________________
**      function name: detect_switch
**      descriptions:
**          detect switch_type
**            
**      parameters:
**         
**             
**      global:
**           None
**             
**      return:
**            
**	     
**      call:
**   	   mt7530_switch_init
**      		
**      revision:
**      1. Here 2009/8/6
**____________________________________________________________________________
*/

void detect_switch(void)
{
    uint32 switch_chip_id = 0;

    switch_chip_id = read_reg_word(GSW_CFG_CREV);
    if((switch_chip_id & 0xFFFF0000) == 0x75300000){
        use_ext_switch = 0;
        mac_p->enetPhyAddr = 8;
        swicVendor = SWIC_MT7530;
    }
        
    switch_chip_id = gswPbusRead(EXT(GSW_CFG_CREV));
    
    if((switch_chip_id & 0xFFFF0000) == 0x75300000){
        use_ext_switch = 1;
        mac_p->enetPhyAddr = 0;
        swicVendor = SWIC_MT7530;
    }
    printk("Use %s switch ,type %d\n",use_ext_switch?"External":"Internal",swicVendor);
}


/*_____________________________________________________________________________
**      function name: doESDTest
**      descriptions:
**          Eanble/Disable ESD test software patch.
**            
**      parameters:
**         argc: argumenet count
**         argv: argumenet pointer
**         p: pinter to function 
**             
**      global:
**           None
**             
**      return:
**            0 :succuess
**	     
**      call:
**   	   
**      		
**      revision:
**      1. Here 2009/8/6
**____________________________________________________________________________
*/
int mt7530_switch_init(void)
{
	int i = 0;
	uint32 reg = 0;

	/* switch init, not need to read from reg, after this, 
	    it will be set to 0x8100, so set to default value. */
	for (i=0; i < RT63365_SWIC_PORTNUM; i++)
		lan_port_tpid[i] = DEFAULT_TPID;
	ether_wan_tpid = DEFAULT_TPID;
	cpu_port_tpid = DEFAULT_TPID;

	if(use_ext_switch == 0)
	{
		/* set vlan table: add all vids to table */
		for(i = 0; i < 4096; i++)
		{
			write_reg_word(GSW_BASE + 0x0094,0x207f0001);
			write_reg_word(GSW_BASE + 0x0090,(0x80001000 + i));
		}
		/* global mac control: set Rx Jumbo to 9K Bytes */
		write_reg_word(GSW_BASE + 0x30e0,0x3f27);

#ifndef TCSUPPORT_INIC_HOST
		mdio_cl22_write(12,0x1f,0x103,0x0020);
		/*mdio_cl22_write(12,0x1f,0x104,0x0608);*/
		mdio_cl22_write(12,0x1f,0x104,0x2608);

		if (isMT7520E2E3) /* for E2/E3 chip */
			mdio_cl22_write(12,0x1f,0x404,0x1900); /* 312.5MHz*/
		else /* for E4 and after chip*/
			mdio_cl22_write(12,0x1f,0x404,0x1d00); /* 362.5MHz*/
			
		mdio_cl22_write(12,0x1f,0x409,0x0057);
		mdio_cl22_write(12,0x1f,0x40a,0x0057);

		mdio_cl22_write(12,0x1f,0x403,0x1800);
		mdio_cl22_write(12,0x1f,0x403,0x1c00);
		mdio_cl22_write(12,0x1f,0x401,0xc020);
		mdio_cl22_write(12,0x1f,0x406,0xa030);
		mdio_cl22_write(12,0x1f,0x410,0x0003);

		for(i = 0; i < 5; i++)
		{
			write_reg_word(GSW_BASE + 0x7a10 + (i * 8),1);
		}
#ifndef TCSUPPORT_CT_PON
		/*broadcast, DLF, multicast storm control to 1Kpkt/s*/
		for(i = 0; i < 7; i++)
		{
			write_reg_word(GSW_BASE + 0x201c + (i * 0x100), 0x4a010101);
		}
#endif
		
		/*set TRGMII*/
		write_reg_word(GSW_BASE + 0x7808,0);
		write_reg_word(GSW_BASE + 0x7804,0x01017e8f);
		write_reg_word(GSW_BASE + 0x7808,1);

		/*write_reg_word(GSW_BASE + 0x3600,0x5e30b);*/
		write_reg_word(GSW_BASE + 0x7830,1);
		
        /* set the igmp report&leave packet just forward to cpu port */
#ifdef TCSUPPORT_CPU_MT7520
        reg = read_reg_word(GSW_IMC);
        reg = reg | (0x06 << IMC_IGMP_RPT_FW_SHIFT);
        write_reg_word(GSW_IMC, reg);
#endif
        
        /*close flow control*/
        write_reg_word(GSW_FC_CTL,0x80083020);  /*close flow control for throughput test*/
#else
		for(i = 0; i < 5; i++)
		{
			write_reg_word(GSW_BASE + 0x7a10 + (i * 8),1);
		}

		/*broadcast, DLF, multicast storm control to 1Kpkt/s*/
		for(i = 0; i < 7; i++)
		{
			write_reg_word(GSW_BASE + 0x201c + (i * 0x100), 0x4a010101);
		}
#endif
		/*set internal switch port 6 EG_TAG as consistent mode*/
		write_reg_word(GSW_BASE + 0x2610,0x81008120);

        /* ingress/egress rate limit calculate (4byte CRC + 12byte IPG + 8byte Preamble) */
        reg = read_reg_word(GSW_GIRLC);
        write_reg_word(GSW_GIRLC, (reg & ~0xFF)|0x18);
        reg = read_reg_word(GSW_GERLC);
        write_reg_word(GSW_GERLC, (reg & ~0xFF)|0x18);
	}
	else
	{
		/*set VLAN table*/
		for(i = 0; i < 4096; i++)
		{
			gswPbusWrite(0x0094,0x207f0001);
			gswPbusWrite(0x0090,(0x80001000 + i));
		}
		/*large pkt support*/
		gswPbusWrite(0x30e0,0x3f27);
		write_reg_word(GSW_BASE + 0x30e0,0x3f27);

        #if 0
		/*PLL reset for E2*/
		mdio_cl22_write(0,0x1f,0x103,0x0020);
		mdio_cl22_write(0,0x1f,0x104,0x0608);
		mdio_cl22_write(0,0x1f,0x104,0x2608);
		mdio_cl22_write(12,0x1f,0x104,0x2608);

		/*PLL modify to 362.5MHz*/
		if (isMT7520E2E3) /* for E2/E3 chip */
			mdio_cl22_write(0,0x1f,0x404,0x1900); /* 312.5MHz*/
		else /* for E4 and after chip*/
			mdio_cl22_write(0,0x1f,0x404,0x1d00); /* 362.5MHz*/
			
		mdio_cl22_write(0,0x1f,0x409,0x0057);
		mdio_cl22_write(0,0x1f,0x40a,0x0057);

		/*for internal*/
		if (isMT7520E2E3) /* for E2/E3 chip */
			mdio_cl22_write(12,0x1f,0x404,0x1900); /* 312.5MHz*/
		else /* for E4 and after chip*/
			mdio_cl22_write(12,0x1f,0x404,0x1d00); /* 362.5MHz*/

		mdio_cl22_write(12,0x1f,0x409,0x0057);
		mdio_cl22_write(12,0x1f,0x40a,0x0057);

		/*PLL bias en*/
		mdio_cl22_write(0,0x1f,0x403,0x1800);
		/*Bias LPF en*/
		mdio_cl22_write(0,0x1f,0x403,0x1c00);
		/*sys PLL en*/
		mdio_cl22_write(0,0x1f,0x401,0xc020);
		/*LCDDDS PWDB*/
		mdio_cl22_write(0,0x1f,0x406,0xa030);
		/*turn on gsw_2x_clk*/
		mdio_cl22_write(0,0x1f,0x410,0x0003);

		/*for internal*/
		mdio_cl22_write(12,0x1f,0x403,0x1800);
		mdio_cl22_write(12,0x1f,0x403,0x1c00);
		mdio_cl22_write(12,0x1f,0x401,0xc020);
		mdio_cl22_write(12,0x1f,0x406,0xa030);
		mdio_cl22_write(12,0x1f,0x410,0x0003);

		/*setup RX delay*/
		for(i = 0; i < 5; i++)
		{
			write_reg_word(GSW_BASE + 0x7a10 + (i * 8),1);
			gswPbusWrite(0x7a10 + (i * 8),1);
		}
#endif
		/*broadcast, DLF, multicast storm control to 1Kpkt/s*/
		for(i = 0; i < 7; i++)
		{
			gswPbusWrite(0x201c + (i * 0x100), 0x4a010101);
		}
#if 0
		/*set TRGMII*/
		write_reg_word(GSW_BASE + 0x7808,0);
		write_reg_word(GSW_BASE + 0x7804,0x01017e8f);
		write_reg_word(GSW_BASE + 0x7808,1);
		gswPbusWrite(0x7808,0);
		gswPbusWrite(0x7804,0x01017e8f);
		gswPbusWrite(0x7808,1);
#endif
      
		/*enable TRGMII*/
		gswPbusWrite(0x7830,1);

		/*enable TRGMII*/
		write_reg_word(GSW_BASE + 0x7830,1);
		
        /* set the igmp report&leave packet just forward to cpu port */
        
#ifdef TCSUPPORT_CPU_MT7520
        reg = gswPbusRead(GSW_IMC-GSW_ARL_BASE);
        reg = reg | (0x06 << IMC_IGMP_RPT_FW_SHIFT);
        gswPbusWrite(GSW_IMC-GSW_ARL_BASE, reg);
#endif

        /*close flow control*/
        write_reg_word(GSW_FC_CTL,0x80083020);  /*close flow control for throughput test*/
        gswPbusWrite(EXT_GSW_FC_CTL,0x80083020);

		/*set ext switch port 6 EG_TAG as consistent mode*/
		gswPbusWrite(0x2610,0x81008120);

        /* ingress/egress rate limit calculate (4byte CRC + 12byte IPG + 8byte Preamble) */
        reg = read_reg_word(GSW_GIRLC);
        write_reg_word(GSW_GIRLC, (reg & ~0xFF)|0x18);
        reg = read_reg_word(GSW_GERLC);
        write_reg_word(GSW_GERLC, (reg & ~0xFF)|0x18);
        
        reg = gswPbusRead(EXT(GSW_GIRLC));
        gswPbusWrite(EXT(GSW_GIRLC), (reg & ~0xFF)|0x18);
        reg = gswPbusRead(EXT(GSW_GERLC));
        gswPbusWrite(EXT(GSW_GERLC), (reg & ~0xFF)|0x18);
        
	}
    switch_reg_write(GSW_PSC(6),0xFFF10);
#ifndef TCSUPPORT_CT_PON
	//write_reg_word(GSW_BASE+0x1640,0x1408488);/*set p6 egress traffic limit (cir=10mbps,cbs=4kbyte)*/
#endif

	return 0;
}


#if 0/*#ifdef TCPHY_4PORT*/
/*_____________________________________________________________________________
**      function name: doESDTest
**      descriptions:
**          Eanble/Disable ESD test software patch.
**            
**      parameters:
**         argc: argumenet count
**         argv: argumenet pointer
**         p: pinter to function 
**             
**      global:
**           None
**             
**      return:
**            0 :succuess
**	     
**      call:
**   	   
**      		
**      revision:
**      1. Here 2009/8/6
**____________________________________________________________________________
*/
static int 
doESDTest(int argc, char *argv[], void *p){
	if( argc != 2 ){
		printk("ESD sw patch status: %s.\r\n", (esd_sw_patch?"on":"off"));
	}
	else if( stricmp(argv[1], "on") == 0 )	{
		esd_sw_patch = 1;		
		printk("ESD sw patch status: on.\r\n");
	}
	else if( stricmp(argv[1], "off") == 0 )	{
		esd_sw_patch = 0;		
		printk("ESD sw patch status: off.\r\n");
	}			
	else {
		printk("ESD sw patch status: %s.\r\n", (esd_sw_patch?"on":"off"));
	}
	return 0;
}/*end doEsdTest */

static const cmds_t ethertprint[] =
{
	{"esdTest",doESDTest,0x02,0,NULL},	
   	{NULL,NULL,0,0,NULL}        ,
};
/*_____________________________________________________________________________
**      function name: doEtherDBG
**      descriptions:
**           register subcommand of "etherd ". 
**             
**      parameters:
**            argc : argument number
**            argv : argument point 
**		 p     :  no use
**      global:
**            None
**             
**      return:
**            Success:        0
**            Otherwise:     -1
**	     
**      call:
**      	
**      revision:
**      
**____________________________________________________________________________
*/

static int doEtherDBG(int argc, char *argv[], void *p)
{
	subcmd(ethertprint, argc, argv, p);
	return 0;
}
/*_____________________________________________________________________________
**      function name: tcetherTest
**      descriptions:
**           register rootcommand of "etherd ". 
**             
**      parameters:
**            none
**      global:
**            None
**             
**      return:
**            none
**	     
**      call:
**      	
**      revision:
**      
**____________________________________________________________________________
*/
void tcetherTest(void){

	cmds_t etherdbg_cmd;
    	
	etherdbg_cmd.name= "etherd";
	etherdbg_cmd.func=doEtherDBG;
	etherdbg_cmd.flags=0x12;
	etherdbg_cmd.argcmin=0;
	etherdbg_cmd.argc_errmsg=NULL;

	cmd_register(&etherdbg_cmd);

}
#endif
