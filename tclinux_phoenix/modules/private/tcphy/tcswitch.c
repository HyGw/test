/************************************************************************
 *
 *	Copyright (C) 2006 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/
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
#include "linux/libcompileoption.h"

#if defined(CONFIG_MIPS_RT63365)
#include "../raeth/femac.h"
#else
#if  defined(CONFIG_MIPS_TC3182) ||  defined(CONFIG_MIPS_TC3262)
#include "../tc3262/tc3262gmac.h"
#else
#include "../tc3162l2hp2h/tc3162l2mac.h"
#endif
#endif

#ifdef TCSUPPORT_INIC_HOST
#include <linux/mtd/fttdp_inic.h>
#endif

#include "tcetherphy.h"
#include "tcswitch.h"

#ifdef TCSUPPORT_MT7510_FE
#define GSW_BASE     		0xBFB58000
#define GSW_PMSR(n)     	(GSW_MAC_BASE + (n)*0x100 + 0x08)
#define read_reg_word(reg) 		regRead32(reg)
#define write_reg_word(reg, wdata) 	regWrite32(reg, wdata)
#endif

#if defined(TCSUPPORT_HWNAT)
//#include "../../../linux/net/bridge/br_private.h"
extern int port_reverse;
#endif
#if defined(TCSUPPORT_QOS)
#define 	QOS_HH_PRIORITY		0x10
#define 	QOS_H_PRIORITY		0x20
#define 	QOS_M_PRIORITY		0x30
#define 	QOS_L_PRIORITY		0x40
#define		NULLQOS				-1
#define 	QOS_SW_PQ			0	//will use hw at the same time
#define		QOS_SW_WRR			1
#define		QOS_SW_CAR			2
#define 	QOS_HW_WRR			3
#define		QOS_HW_PQ			4

extern int qos_flag ;
extern int ds_switch_qos_flag;
#endif	
#define DEF_PRIORITY_PKT_CHK_LEN	100
int priPktChkLen = DEF_PRIORITY_PKT_CHK_LEN;
int priPktChk = 1;
static uint8 maxPrio = 3;

char esd_has_reset=0;
extern uint8 swicVendor;
extern macAdapter_t *mac_p;

extern uint8 tcPhyExtResetNeeded(void);
extern uint8 getTcPhyFlag(void);
extern int tcPhyInit(macAdapter_t* mac_p);
extern bool getTcPhyEsdDetectFlag(void);
extern int tcPhyPortInit(uint8 port_num);
extern int tcPhySwPatch(void);
#if defined(TCSUPPORT_SEPERATED_ETH_ITF)
extern void setSkbFromWhichSepEthItf(struct sk_buff *skb, int itf);
extern int skbFromWhichSepEthItf(struct sk_buff *skb);
#endif/* TCSUPPORT_SEPERATED_ETH_ITF */
#ifdef WAN2LAN
extern int masko;
#endif
#if defined(TCSUPPORT_CPU_MT7520)
extern int masko_tcconsole;
#endif
//#define RT62806_VERIFY
#ifdef RT62806_VERIFY
extern int rt62806_port_mask;
#endif

//#ifdef VPORT
//extern int vportMasko;
//#endif
#if 1//def VPORT
int vportMasko=0;
int vport_enable=0;
#endif

#ifdef TCPHY_DEBUG
extern void tcPhyChkVal(void);
extern int tcPhyErrMonitor(void);
#endif

#if defined(TCSUPPORT_CT)
#if defined(TCSUPPORT_CPU_RT63365)
int g_port_reverse = 0;
#else
#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_CT_PON_CY) && !defined(TCSUPPORT_CT_PON_C9)
int g_port_reverse = 1;
#else
int g_port_reverse = 0;
#endif
#else
int g_port_reverse = 1;//dedault need port reverse function according to customer's requset
#endif
#endif

#if !defined(TCSUPPORT_CPU_MT7510) && !defined(TCSUPPORT_CPU_MT7505) 
int g_stag_to_vtag = 0;
#endif
#else
//extern int g_port_reverse;
int g_port_reverse = 0;
/* special tag to vlan tag */
int g_stag_to_vtag = 0;
#endif


extern uint32 tcMiiStationRead(uint32 enetPhyAddr,uint32 phyReg);
extern void tcMiiStationWrite(uint32 enetPhyAddr,uint32 phyReg,uint32 miiData);
#if defined(TCSUPPORT_PON_VLAN) || defined(TCSUPPORT_PON_MAC_FILTER) || defined(TCSUPPORT_XPON_IGMP) || defined(TCSUPPORT_ETHER_DOWNVLAN)
int xpon_get_vport(void)
{
	return vport_enable;
}
EXPORT_SYMBOL(xpon_get_vport);
#endif

#if defined(CONFIG_MIPS_RT63365)
#if defined(TCPHY_SUPPORT)
extern void tcPhyWriteReg(uint8 port_num,uint8 reg_num,uint32 reg_data);
extern uint32 tcPhyReadReg(uint8 port_num,uint8 reg_num);
#endif
#endif

uint8 macSTagFlag = 0;
uint8 macVlanFlag = 0;
uint8 macRxPortFlag = 0;
uint8 esd_sw_patch = 1;/*Default is enabled the ESD software patch*/

swic_reg_t tc2206_swic_reg[]=
{
	{MODEL_V0|MODEL_V1, 22, 0, 0x6f9},/*P4MECR,Enable RGMII Mode*/
	{MODEL_V0|MODEL_V1, 22, 1, 0x7f8},/*P5MECR*/
	{MODEL_V0|MODEL_V1, 21, 5, 0x68c},/*HIGMPSCR*/
	{MODEL_V0|MODEL_V1, 20, 13, 0x20},/*ATCR*/
	{MODEL_V0|MODEL_V1, 21, 17, 0x10d},/*SCR1*/
	{MODEL_V0|MODEL_V1, 21, 18, 0x7fc},/*P0MCR*/
	{MODEL_V0|MODEL_V1, 21, 19, 0x7fc},/*P1MCR*/
	{MODEL_V0|MODEL_V1, 21, 20, 0x7fc},/*P2MCR*/
	{MODEL_V0|MODEL_V1, 21, 21, 0x7fc},/*P3MCR*/
	{MODEL_V0|MODEL_V1, 21, 22, 0x0ffc},/*P4MCR*/
    {MODEL_V0|MODEL_V1|MODEL_V2, 21, 23, 0x5ffc},/*P5MCR,Let CPU Port (Port 5) fixed on Link UP & 100 Full-Duplex Speed*/   
	{MODEL_V0|MODEL_V1|MODEL_V2, 21, 29, 0x6858},/*FLOW CONTROL THRESHOLD. DECREASE RLEASE THRESHOLD*/
	{MODEL_V0|MODEL_V1, 20, 2, 0xf20f},/*P0CR*/
	{MODEL_V0|MODEL_V1, 20, 3, 0xf20f},/*P1CR*/
	{MODEL_V0|MODEL_V1, 20, 4, 0xf20f},/*P2CR*/
	{MODEL_V0|MODEL_V1, 20, 5, 0xf20f},/*P3CR*/
	{MODEL_V0|MODEL_V1, 20, 6, 0xf20f},/*P4CR*/
	{MODEL_V0|MODEL_V1, 20, 7, 0xf20f},/*P5CR*/
#if defined(TCSUPPORT_ETH4_WAN_PORT)
	{MODEL_V2, 20, 17, 0x4001}, 
	{MODEL_V2, 20, 18, 0x4002},
	{MODEL_V2, 20, 19, 0x4003},
	{MODEL_V2, 20, 20, 0x4004},
#endif
	{MODEL_V0|MODEL_V1, 21, 31, 0x40},/*MCMBTCR*/
	{MODEL_V0|MODEL_V1|MODEL_V2, 24, 22, 0x4000},/*Enable the secondary L2 mac table to workaround thatSA=0.0.0.0 will be used entry 2049 entry*/
	{MODEL_V0|MODEL_V1, 30, 8, 0x7777},/*RXCDC, must to be set up*/
	{MODEL_V0|MODEL_V1, 30, 13, 0x0},/*WDTCR,Disable the watchdog timer*/
	{MODEL_V0|MODEL_V1|MODEL_V2, 21, 4, 0xd},/*MLDPCR, MLD(IPv6 packet) forward to port_dft_mcas*/
	{MODEL_V0|MODEL_V1|MODEL_V2, 21, 16, 0x10},/*SCR0,Disable illegal DA/SA check operation*/
	{0, 0, 0x0}
};

/*Those registers is used to check the switch register is reset to default value.
It is used to protect ESD test.*/
swic_reg_t esd_chk_reg[]=
{
	{MODEL_V0|MODEL_V1, 21, 22, 0x0},/*P4MCR*/
	{MODEL_V0|MODEL_V1|MODEL_V2, 21, 23, 0x0},/*P5MCR,Let CPU Port (Port 5) fixed on Link UP & 100 Full-Duplex Speed*/
	{MODEL_V0|MODEL_V1, 30, 8, 0x0},/*RXCDC, must to be set up*/
	{MODEL_V0|MODEL_V1, 30, 13, 0x0},/*WDTCR,Disable the watchdog timer*/
	{0,0, 0, 0x0}	
};


/*_____________________________________________________________________________
**      function name: refillPhyDefVal
**      descriptions:
**      Accroding the user use mdio to operate phy reset register, it's will be 
**      trigger the esd software patch,so we need to  refill the phy register 
**      initial value. 
**            
**      parameters:
**	enetPhyAddr: PHY Register
**	phyReg: Register Address
**	miiData: Write into value
**             
**      global:
**         None
**             
**      return:
**             None
**	     
**      call:
**   	     tcPhyPortInit
**      		
**      revision:
**      1. Here 2010/05/11
**____________________________________________________________________________
*/
void
refillPhyDefVal(
	uint32 enetPhyAddr,
	uint32 phyReg,
	uint32 miiData
){
	/*TC2206 switch IC can't be direct to do PHY reset, we must 
	 * avoid ESD software patch be trigger.
	 */
	if((phyReg==MII_BMCR) && (miiData& BMCR_RESET)&&(swicVendor==SWIC_TC2206)
	&&((enetPhyAddr==0)||(enetPhyAddr==1)||(enetPhyAddr==2)||(enetPhyAddr==3))){
		tcPhyPortInit(enetPhyAddr);
	}
}/*end refillPhyDefVal*/

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

/* --------------- xyzhu_091105 special tag relation function start -------------- */

int macAdmSTagEnable(int ChanID)
{
	return 0;	
}

int macIpSTagEnable(int ChanID)
{
	uint16 temp;

	
	if (!macVlanFlag){
		tcMiiStationWrite(29, 24, 0xFFF);
		tcMiiStationWrite(29, 25, 0xFFF);
		tcMiiStationWrite(29, 26, 0xFFF);
		tcMiiStationWrite(29, 27, 0xFFF);
#ifdef WLAN_BY_ETHER_SWITCH
		tcMiiStationWrite(29, 28, 0xFFF);
#endif
	}
	/* router control register 1: STAG_EN */
	temp = (uint16)tcMiiStationRead(30, 16);
	tcMiiStationWrite(30, 16, temp | 0x80);
	/* tag register 11: Port5 add tag */
	if (!macVlanFlag){
		tcMiiStationWrite(29, 23, 0x7c2);
	}
	/* soft reset */
#ifndef DLINK_CARPHONE
	tcMiiStationWrite(30, 0, 0x175C);
	//delay1ms(5);
	pause(5);
#endif
	macSTagFlag = 1;
	return 0;		
}
void macIpSTagInsert(struct sk_buff *bp)
{
	uint8 *cp;
	uint16 etpid;
	uint8 port_mask;

	port_mask = 0xF;

	cp = bp->data;
	etpid = *(uint16 *)(cp+12);

	if((bp->mark & SKBUF_COPYTOLAN) || (bp->mark & SKBUF_TCCONSOLE)) {
		if ((etpid == 0x8100) && (macVlanFlag))
		{
//			Ether_DBG("replace VLAN tag with STag\r\n");
			*(uint16 *)(cp+12) = (0x8100 | port_mask);
		}
		else
		{
//		Ether_DBG("insert STag for IGMP snooping\r\n");
			memmove(cp-4, cp, 12);
			*(uint16 *)(cp+8) = (0x8100 | port_mask);
			if (etpid == 0x8100)
				*(uint16 *)(cp+10) = *(uint16 *)(cp+14);
			else
				*(uint16 *)(cp+10) = 0;
			bp->data-=4;
			//bp->cnt+=4;
			bp->len+=4;
		}
	}
	else if (etpid == 0x8100)
	{
		
		if (macVlanFlag)
		{
//			Ether_DBG("replace VLAN tag with STag\r\n");
			*(uint16 *)(cp+12) = (0x8100 | port_mask);
		}
		else
		{
//			Ether_DBG("insert STag for VLAN-tagged frame\r\n");
			memmove(cp-4, cp, 12);
			*(uint16 *)(cp+8) = (0x8100 | port_mask);
			*(uint16 *)(cp+10) = *(uint16 *)(cp+14);
			bp->data-=4;
			//bp->cnt+=4;
			bp->len+=4;
		}
	}
}

void macIpSTagRemove(struct sk_buff *bp)
{
	uint8 *cp;
	uint16 etpid, tci;
	uint8 port_mask;
	
	cp = bp->data;
	etpid = *(uint16 *)(cp+12);
#ifndef WLAN_BY_ETHER_SWITCH
	port_mask = etpid & 0xF;
#else
	port_mask = etpid & 0x1F;
#endif

	if ((etpid & 0xFF00) == 0x8100){

		tci = *(uint16 *)(cp+14);
#if 1
		if ((tci & 0xFFF) != 0xFFF)
#else
		if (macVlanFlag)
#endif
		{
//			Ether_DBG("VLAN tagged\r\n");
			*(uint16 *)(cp+12) = 0x8100;
		}
		else
		{
//			Ether_DBG("VLAN untagged\r\n");
			memmove(bp->data+4, bp->data, 12);
			skb_pull(bp, 4);
		}
	}
}

int macTC2206STagEnable(int chanID)
{
	uint16 value=0;
	
	/*Enable Special Tag function*/
	/* router control register 1: STAG_EN */
	value = (uint16)tcMiiStationRead(21, 16);
	value|=0x2000;
	tcMiiStationWrite(21, 16, value);
	value = (uint16)tcMiiStationRead(21, 15);
	value |=0x20;
	tcMiiStationWrite(21, 15, value);
	macSTagFlag = 1;
	return 0;
}/*end macTC2206STagEnable*/

#if defined(TCSUPPORT_CT)
void macTC2206STagInsert(struct sk_buff *bp)
{
	uint8 *cp;
	uint8 port_mask=0;
	uint8 vlanid = 0;

	if (bp->mark & SKBUF_COPYTOLAN){
		//if the packet is used for wan2lan,set port_mask from masko that from cli
		#ifdef WAN2LAN
		#ifdef TCSUPPORT_CT_2PWIFI
		if (g_port_reverse) {
			uint8 temp_masko = (uint8)(masko & 0x3);
			if((temp_masko != 0x3) && (temp_masko != 0x0))
			{
				temp_masko = 0x3 -temp_masko; 
			}
			port_mask = temp_masko & 0x3;
		}
		else {
			port_mask = masko & 0x3;
		}
		#else
		if (g_port_reverse) {
			uint8 temp_masko = (uint8)masko;
			if (((temp_masko & 0x1)<<3) != (temp_masko & 0x8)) {
				if (temp_masko & 0x1) {
					temp_masko &= 0xFE;
				}
				else {
					temp_masko |= 0x1;
				}
				if (temp_masko & 0x8) {
					temp_masko &= 0xF7;
				}
				else {
					temp_masko |= 0x8;
				}
			}
			if (((temp_masko & 0x2)<<1) != (temp_masko & 0x4)) {
				if (temp_masko & 0x2) {
					temp_masko &= 0xFD;
				}
				else {
					temp_masko |= 0x2;
				}
				if (temp_masko & 0x4) {
					temp_masko &= 0xFB;
				}
				else {
					temp_masko |= 0x4;
				}
			}
			port_mask = temp_masko & 0xF;
		}
		else {
			port_mask = masko & 0xF;
		}
		#endif
		#endif
	}
	else{
		port_mask = 0xF;
	}	
	cp = bp->data;
	/*Reverse Ethernet Port seq*/
	//reverse_portmask(&port_mask);
	/*wan2lan packet need to insert special Tag function.*/
	if((bp->mark & SKBUF_COPYTOLAN)|| (bp->mark & SKBUF_TCCONSOLE) ) {
		skb_push(bp, TX_STAG_LEN);
		cp = bp->data;
		/*Insert Special Tags*/
		memmove(cp, cp+TX_STAG_LEN, 12);
		*(uint16 *)(cp+12) = DEF_STAG_VALUE;
		/*6:Tx Special Len, 1: enable egress port map*/
		*(uint16 *)(cp+14) = 0x0601;
		/*txTag(2'b11):Egress packet always reference vlan table's tag_mem information.*/
		*(uint16 *)(cp+16) = (port_mask <<8)|0x10|0x3;
	}
}/*end macTC2206STagInsert*/
#else
void macTC2206STagInsert(struct sk_buff *bp)
{
	uint8 *cp;
	uint8 port_mask=0;
	uint8 vlanid = 0;
	
	if (bp->mark & SKBUF_COPYTOLAN){
		//if the packet is used for wan2lan,set port_mask from masko that from cli
		#ifdef WAN2LAN
		#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
		if (g_port_reverse) {
			uint8 temp_masko = (uint8)(masko & 0x3);
			if((temp_masko != 0x3) && (temp_masko != 0x0))		/* change port_mask from 1 to 2 or from 2 to 1 */
			{
				temp_masko = 0x3 -temp_masko; 
			}
			port_mask = temp_masko & 0x3;
		}
		else {
			port_mask = masko & 0x3;				/* if port_mask is 0 or 3, donot change */
		}
		#else
		if (g_port_reverse) {
			uint8 temp_masko = (uint8)masko;
			if (((temp_masko & 0x1)<<3) != (temp_masko & 0x8)) {
				if (temp_masko & 0x1) {
					temp_masko &= 0xFE;
				}
				else {
					temp_masko |= 0x1;
				}
				if (temp_masko & 0x8) {
					temp_masko &= 0xF7;
				}
				else {
					temp_masko |= 0x8;
				}
			}
			if (((temp_masko & 0x2)<<1) != (temp_masko & 0x4)) {
				if (temp_masko & 0x2) {
					temp_masko &= 0xFD;
				}
				else {
					temp_masko |= 0x2;
				}
				if (temp_masko & 0x4) {
					temp_masko &= 0xFB;
				}
				else {
					temp_masko |= 0x4;
				}
			}
			port_mask = temp_masko & 0xF;
		}
		else {
			port_mask = masko & 0xF;
		}
		#endif
		#endif
	}
#if 1//def VPORT
	else if (g_stag_to_vtag && (bp->mark & SKBUF_VLAN)){
		if(vport_enable == 1){
		if (g_port_reverse) {
		#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
			vlanid = 2 - ((uint8)bp->data[15]);
		#else
			vlanid = 4 - ((uint8)bp->data[15]);
		#endif
			vportMasko = (1<<vlanid);
		}
		else {
			vportMasko = (1<<((uint8)bp->data[15]-BASE_VID_IDX));
		}
		
		memmove(bp->data+VLAN_HLEN, bp->data, 12);
		skb_pull(bp, VLAN_HLEN);
		
		port_mask = vportMasko & 0xF;
// 		printk("mac tag insert port_mask %x\n",port_mask);
		}else{
			//disable vlanpt do not need remove vlan tag 
			//	and insert special tag.
			return;	
		}
	}
#endif
	else{
		port_mask = 0xF;
	}	
	cp = bp->data;
	/*Reverse Ethernet Port seq*/
	//reverse_portmask(&port_mask);
	/*wan2lan packet need to insert special Tag function.*/
	if((bp->mark & SKBUF_COPYTOLAN)
		|| (bp->mark & SKBUF_TCCONSOLE)
#if 1//def VPORT /* stag to vtag packet */
		||(g_stag_to_vtag && (bp->mark & SKBUF_VLAN))
#endif
		) {
		skb_push(bp, TX_STAG_LEN);
		cp = bp->data;
		/*Insert Special Tags*/
		memmove(cp, cp+TX_STAG_LEN, 12);
		*(uint16 *)(cp+12) = DEF_STAG_VALUE;
		/*6:Tx Special Len, 1: enable egress port map*/
		*(uint16 *)(cp+14) = 0x0601;
#if 0		
#ifdef VPORT		
		/*txTag(2'b11):Egress packet always reference vlan table's tag_mem information.*/
		if ((bp->mark & 0xf0000000) >= 0x10000000 && (bp->mark & 0xf0000000) <= 0x40000000) {
			*(uint16 *)(cp+16) = (port_mask <<8)|0x10|0x3;
		}
		else {
			/*Eanble the SA autolearning function.*/
			*(uint16 *)(cp+16) = (port_mask <<8)|0x10|0x8|0x3;
		}
#else		
		/*txTag(2'b11):Egress packet always reference vlan table's tag_mem information.*/
		*(uint16 *)(cp+16) = (port_mask <<8)|0x10|0x3;
#endif		
#endif	

		if (bp->mark & MULTICAST_MASK) {
				port_mask = (bp->mark&MULTICAST_MASK)>>16;			

				#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
				if (g_port_reverse) {
					uint8 temp_masko = (uint8)(port_mask & 0x3);
					if((temp_masko != 0x3) && (temp_masko != 0x0))		/* change port_mask from 1 to 2 or from 2 to 1 */
					{
						temp_masko = 0x3 -temp_masko; 
					}
					port_mask = temp_masko & 0x3;
				}
				else {
					port_mask = port_mask & 0x3;				/* if port_mask is 0 or 3, donot change */
				}
				#else
				if (g_port_reverse){			
					uint8 temp_masko = port_mask;					
					if (((temp_masko & 0x1)<<3) != (temp_masko & 0x8)) {
						if (temp_masko & 0x1) {
							temp_masko &= 0xFE;
						}
						else {
							temp_masko |= 0x1;
						}
						if (temp_masko & 0x8) {
							temp_masko &= 0xF7;
						}
						else {
							temp_masko |= 0x8;
						}
					}
					
					if (((temp_masko & 0x2)<<1) != (temp_masko & 0x4)) {
						if (temp_masko & 0x2) {
							temp_masko &= 0xFD;
						}
						else {
							temp_masko |= 0x2;
						}
						if (temp_masko & 0x4) {
							temp_masko &= 0xFB;
						}
						else {
							temp_masko |= 0x4;
						}
					}
				
					port_mask = temp_masko & 0xF;
				}
				#endif

				*(uint16 *)(cp+16) = (port_mask <<8)|0x10|0x3;
					
		}
		else {
		if (g_stag_to_vtag) {
				if ((bp->mark & 0xf0000000) >= 0x10000000 && (bp->mark & 0xf0000000) <= 0x40000000) {
					*(uint16 *)(cp+16) = (port_mask <<8)|0x10|0x3;
				}
				else {
					*(uint16 *)(cp+16) = (port_mask <<8)|0x10|0x8|0x3;
				}
			}
			else {
				/*txTag(2'b11):Egress packet always reference vlan table's tag_mem information.*/
				*(uint16 *)(cp+16) = (port_mask <<8)|0x10|0x3;
			}
		}
	}
}/*end macTC2206STagInsert*/

#endif

#if !defined(TCSUPPORT_CT)
#ifdef CONFIG_MIPS_TC3162U
__IMEM
#endif
#endif

void macTC2206STagRemove(struct sk_buff *bp)
{
	uint8 *cp=NULL;
	uint16 spTag=0;
#if !defined(TCSUPPORT_CT)
#if 1//def VPORT
	uint8 ingressPid=0;
#endif
#endif

	cp = bp->data;
	spTag= *(uint16 *)(cp+12);
	
	if (spTag == DEF_STAG_VALUE){
		#ifdef TC2206_FPGA
		if(getTC2206Dbg()){
			dbg_pline_1("\r\n macTC2206STagRemove\r\n");
			/*
			dbg_plinel_1("\r\ningressPid:", ingressPid);
			dbg_plinel_1("\r\nspTag:", spTag);
			dbg_plinel_1("\r\nport_mask:", bp->ether_switch.port_mask);
			*/
		}
		#endif
#if defined(TCSUPPORT_CT)
		memmove(bp->data+RX_STAG_LEN, bp->data, 12);
		skb_pull(bp, RX_STAG_LEN);
#else
#if 0		
#ifdef VPORT
		if(vport_enable == 1){
		if (g_port_reverse) {
		#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
			ingressPid = 1 - (*(uint8 *)(cp+15) & 0x7);
		#else
			ingressPid = 3 - (*(uint8 *)(cp+15) & 0x7);
		#endif
		}
		else {
			ingressPid = *(uint8 *)(cp+15) & 0x7;
		}

//  		printk("ingressPid:%02x\n", ingressPid);
		/*Overwriting the 8 bytes special tag with vlan tag 
			and also offset the 4 bytes header. */
		memmove(bp->data+VLAN_HLEN, bp->data, 12);
		/*Overwriting the 4bytes Vlan Tags*/
		*(uint16 *)(cp+16) = ETH_P_8021Q;
		*(uint16 *)(cp+18) = ingressPid + BASE_VID_IDX;
		skb_pull(bp, VLAN_HLEN);
		}else{
			//After disable vlanpt, we only need remove special tag.
			memmove(bp->data+RX_STAG_LEN, bp->data, 12);
			skb_pull(bp, RX_STAG_LEN);
		}
#else
		memmove(bp->data+RX_STAG_LEN, bp->data, 12);
		skb_pull(bp, RX_STAG_LEN);

#endif
#endif
		if (g_stag_to_vtag) {
			if(vport_enable == 1){
				if (g_port_reverse) {
				#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
					ingressPid = 1 - (*(uint8 *)(cp+15) & 0x7);
				#else
					ingressPid = 3 - (*(uint8 *)(cp+15) & 0x7);
				#endif
				}
				else {
					ingressPid = *(uint8 *)(cp+15) & 0x7;
				}

		//  		printk("ingressPid:%02x\n", ingressPid);
				/*Overwriting the 8 bytes special tag with vlan tag 
					and also offset the 4 bytes header. */
				memmove(bp->data+VLAN_HLEN, bp->data, 12);
				/*Overwriting the 4bytes Vlan Tags*/
				*(uint16 *)(cp+16) = ETH_P_8021Q;
				*(uint16 *)(cp+18) = ingressPid + BASE_VID_IDX;
				skb_pull(bp, VLAN_HLEN);
			}else{
				//After disable vlanpt, we only need remove special tag.
				memmove(bp->data+RX_STAG_LEN, bp->data, 12);
				skb_pull(bp, RX_STAG_LEN);
			}
		}
		else {
			memmove(bp->data+RX_STAG_LEN, bp->data, 12);
			skb_pull(bp, RX_STAG_LEN);
		}
#endif

	}
}/*end macTC2206STagRemove*/

#define CHK_BUF() pos = begin + index; if (pos < off) { index = 0; begin = pos; }; if (pos > off + count) goto done;

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

extern uint16 lan_port_tpid[MT7530_SWIC_PORTNUM];
extern uint16 ether_wan_tpid;
extern uint16 cpu_port_tpid;
extern uint8 use_ext_switch;

uint8 macRT63365STagRemap(uint8 port_mask)
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
#if defined(TCSUPPORT_CPU_MT7520)
extern int (*MT7530LanPortMap2Switch_hook)(int port);
extern u32 macMT7530LanPortMap2Switch(u32 portId);
#endif
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
	if (0>wan_port || wan_port>=RT63365_SWIC_PORTNUM){
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
	if (isRT63368){
#if defined(MT7530_SUPPORT)
		wan_port_id = 4;
		lan_port_map[0] = 0;
		lan_port_map[1] = 1;
		lan_port_map[2] = 2;
		lan_port_map[3] = 3;
#else
		wan_port_id = 5;
		lan_port_map[0] = 1;
		lan_port_map[1] = 2;
		lan_port_map[2] = 3;
		lan_port_map[3] = 4;
#endif		
		lan_port_map[4] = -1;
		lan_port_map[5] = -1;
	} else if (isRT63365 || isMT751020 || isMT7505){
		if (isMT7525 || isMT7520 || isMT7520S){
			wan_port_id = -1;
			if (isMT7520S){
				lan_port_map[0] = 4; // only one giga port
				lan_port_map[1] = -1;
				lan_port_map[2] = -1;
				lan_port_map[3] = -1;
			}else{ // 7525F or 7520F
				if(TCSUPPORT_XPON_HAL_API_EXT_VAL)
				{
					if(isMT7520)
					{
						lan_port_map[0] = 4;
						lan_port_map[1] = 1;
						lan_port_map[2] = 2;
						lan_port_map[3] = 3;	
					}else{
						lan_port_map[0] = 4;
						lan_port_map[1] = 3;
						lan_port_map[2] = 2;
						lan_port_map[3] = 1;	
					}
				}else{
				lan_port_map[0] = 1;
				lan_port_map[1] = 2;
				lan_port_map[2] = 3;
				lan_port_map[3] = 4;
				}
			}
		}else {
			if (isMT7520G || isMT7525G)
				wan_port_id = -1;
			else
				wan_port_id = 4;
			lan_port_map[0] = 0;
			lan_port_map[1] = 1;
			lan_port_map[2] = 2;
			lan_port_map[3] = 3;
		}
		lan_port_map[4] = -1;
		lan_port_map[5] = -1;
	}
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


int ethernet_portmap_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	uint8 port;

	index += sprintf(buf+index, "%d\n", wan_port_id);
	CHK_BUF();
	for (port=0; port<(RT63365_SWIC_PORTNUM-1); port++){
		index += sprintf(buf+index, "%d %d\n", port, lan_port_map[port]);
		CHK_BUF();
	}
	*eof = 1;

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0) 
		index = 0;
	if (index>count) 
		index = count;
	return index;
}

#if defined(TCSUPPORT_CPU_MT7520) && defined(TCSUPPORT_WAN_ETHER)
static uint32 orig_fe_cpc = 0;
static uint32 orig_inner_gfccr0 = 0;
static uint32 orig_exter_gfccr0 = 0;
static uint32 orig_vid1 = 0;
static uint32 orig_vid5 = 0;
static uint32 orig_inner_pmc5 = 0;
static uint32 orig_exter_pmc6 = 0;

int ethernet_portmap_write_proc(struct file *file, const char *buffer,
                unsigned long count, void *data)
{
        char val_string[32] = {0};
        uint8 port = 0;
        uint8 value = 0;
        int wan_port = -1;
        int lan[6] = {-1, -1, -1, -1, -1, -1};
	uint32 reg_value = 0;

        if ((count > 20) || (count < 13))
	{
		printk("String length is not valid\n");
                return -EINVAL;
	}

        if (copy_from_user(val_string, buffer, count))
                return -EFAULT;

        value = sscanf(val_string, "%d_%d_%d_%d_%d_%d_%d", &wan_port, &lan[0], &lan[1], &lan[2], &lan[3], &lan[4], &lan[5]);

	if (7 != value)
	{
                printk("String is not valid\n");
                return -EINVAL;
	}

	if ((wan_port > 5) || (wan_port < -1))
	{
		printk("wan port %d is not valid\n, wan_port");
		return -EFAULT;
	}

	for (port = 0; port < 6; port++)
	{
		if ((lan[port] > 5) || (lan[port] < -1))
		{
			printk("lan%d port %d is not valid\n, port, lan[port]");
			return -EFAULT;
		}
	}

	if ((-1 == wan_port_id) && (-1 != wan_port))
	{
		/* PON mode to ETHERWAN mode */

		/* Store FE CPC */
		orig_fe_cpc = read_reg_word(0xbfb00200);

		/* Store inner switch Global Flow Control */
		orig_inner_gfccr0 =  read_reg_word((GSW_BASE + 0x1fe0));

		if(0 == use_ext_switch)
		{
			/* Store vlan 1 */
			write_reg_word((GSW_BASE + 0x0090), 0x80000001);
			udelay(1);
			orig_vid1 = read_reg_word((GSW_BASE + 0x0094));
			/* Store vlan 5 */
			write_reg_word((GSW_BASE + 0x0090), 0x80000005);
			udelay(1);
			orig_vid5 = read_reg_word((GSW_BASE + 0x0094));
		}
		else
		{
			/* Store external switch Global Flow Control */
			orig_exter_gfccr0 = gswPbusRead(0x1fe0);

			/* Store vlan 1 */
			gswPbusWrite(0x0090, 0x80000001);
			orig_vid1 = gswPbusRead(0x94);
			/* Store vlan 5 */
			gswPbusWrite(0x0090, 0x80000005);
			orig_vid5 = gswPbusRead(0x94);

			/* Store external switch Mac P6 */
			orig_exter_pmc6 = gswPbusRead(0x3600);
			/* Store external switch Mac P5 */
			orig_inner_pmc5 = read_reg_word((GSW_BASE + 0x3500));
		}
		

		reg_value = orig_fe_cpc & ~(0xb000000);

		/* Disable FE GSW_TX/LAN_WAN Flow Control */
		write_reg_word(0xbfb00200, reg_value);
	}
	else if ((-1 != wan_port_id) && (-1 == wan_port))
	{
		/* ETHERWAN mode to PON mode */

		/* Restore FE GSW_TX/LAN_WAN Flow Control */
		reg_value = read_reg_word(0xbfb00200);
		reg_value |= (orig_fe_cpc & 0xb000000);
		write_reg_word(0xbfb00200, reg_value);

                /* Restore inner switch Global Flow Control */
                write_reg_word((GSW_BASE + 0x1fe0), orig_inner_gfccr0);

                if(0 == use_ext_switch)
                {
                        /* Restore vlan 1 */
                        write_reg_word((GSW_BASE + 0x0094), orig_vid1);
                        write_reg_word((GSW_BASE + 0x0090), 0x80001001);
			udelay(1);
                        /* Restore vlan 5 */
                        write_reg_word((GSW_BASE + 0x0094), orig_vid5);
                        write_reg_word((GSW_BASE + 0x0090), 0x80001005);
			udelay(1);
                }
                else
                {
			/* Restore external switch Global Flow Control */
			gswPbusWrite(0x1fe0, orig_exter_gfccr0);

                        /* Restore vlan 1 */
                        gswPbusWrite(0x0094, orig_vid1);
                        gswPbusWrite(0x0090, 0x80001001);
                        /* Restore vlan 5 */
                        gswPbusWrite(0x0094, orig_vid5);
                        gswPbusWrite(0x0090, 0x80001005);

                        /* Restore external switch Mac P6 */
                        gswPbusWrite(0x3600, orig_exter_pmc6);
                        /* Restore inner swicth Mac P5 */
                        write_reg_word((GSW_BASE + 0x3500), orig_inner_pmc5);
                }
	}

        wan_port_id = wan_port;

        memset(lan_port_map, -1, RT63365_SWIC_PORTNUM*sizeof(char));
        for (port =0; port < 6; port++){
                if ((0 <= lan[port])  && (lan[port] < RT63365_SWIC_PORTNUM)){
                        lan_port_map[port] = (char)lan[port];
                }
        }

        memset(switch_port_map, -1, RT63365_SWIC_PORTNUM*sizeof(char));
        for (port = 0; port < RT63365_SWIC_PORTNUM; port++){
                value = lan_port_map[port];
                if ((0 <= value) && (value <RT63365_SWIC_PORTNUM)){
                        switch_port_map[value] = port;
                }
        }

        return count;
}
#endif /* defined(TCSUPPORT_CPU_MT7520) && defined(TCSUPPORT_WAN_ETHER) */

#ifdef RT62806_VERIFY
int macRT62806STagEnable(int chanID)
{
	uint32 value;
	/*Enable Special Tag function*/
	value = tcMiiStationRead(31, GSW_BASE + 0x2510);
//	value = VPint(GSW_BASE + 0x2610);
	/* set CPU port 6 as user port */
	value &= ~0xc0;
	/* set CPU port 6 special tag=1 */
	value |= (1<<5);

	/* eg_vlan_tag : consistent mode */
	value |= (1<<8);

	tcMiiStationWrite(31, GSW_BASE + 0x2510, value);
//	VPint(GSW_BASE + 0x2610) = value;

	/* Enable GDM_TCI_81XX */
//	value = VPint(GDMA1_FWD_CFG);
//	value |= GDM_TCI_81XX;
//	VPint(GDMA1_FWD_CFG) = value;

	macSTagFlag = 1;

	return 0;
}/*end macRT62806STagEnable*/

struct sk_buff *macRT62806STagInsert(struct sk_buff *bp)
{
	uint8 *cp;
	uint8 port_mask=0;
	uint16 tag0;
	
	if (bp->mark & SKBUF_COPYTOLAN){
		//if the packet is used for wan2lan,set port_mask from masko that from cli
		#ifdef WAN2LAN
		port_mask = masko & 0xF;
		#endif
	}
	else{
		port_mask = 0x0;
	}	
	cp = bp->data;
	/*Reverse Ethernet Port seq*/
	//reverse_portmask(&port_mask);
	/*wan2lan packet need to insert special Tag function.*/
	tag0 = *(uint16 *)(cp+12);

	if (rt62806_port_mask) {
		port_mask = rt62806_port_mask;
	}	
	if (tag0 == 0x8100) {
		*(uint16 *)(cp+12) = (0x8100 | port_mask);
	} else {
		if (skb_headroom(bp) < 4) {
			struct sk_buff *bp2 = skb_realloc_headroom(bp, 4);
			dev_kfree_skb(bp);
			if (bp2 == NULL) {
				return NULL;
			}
			bp = bp2;
		} else {
			bp = skb_unshare(bp, GFP_ATOMIC);
			if (!bp) {
				return NULL;
			}
		}

		skb_push(bp, 4);
		cp = bp->data;
		/*Insert Special Tags*/
		memmove(cp, cp+4, 12);
		*(uint16 *)(cp+12) = (0x8000 | port_mask);
		*(uint16 *)(cp+14) = 0x0000;
	}

	return bp;
}/*end macRT62806STagInsert*/

void macRT62806STagRemove(struct sk_buff *bp)
{
	uint8 *cp=NULL;
	uint16 tag0;
	uint16 tag1;

	cp = bp->data;
	tag0 = *(uint16 *)(cp+12);
	tag1 = *(uint16 *)(cp+14);
	
	if (((tag0 & 0xff00) == 0x8000) && (tag1 == 0x0000)) {
		/* VLAN disabled case */
		memmove(bp->data+4, bp->data, 12);
		skb_pull(bp, 4);
	} else {
		/* VLAN enabled case */
		if ((tag0 & 0xff00) == 0x8100)
			*(uint16 *)(cp+12) = 0x8100;
	}
}/*end macRT62806STagRemove*/
#endif

#if defined(TCSUPPORT_SOC_ETHER_WAN)
static void acl_for_soc_ether_wan(void)
{
    uint32 value;
    regWrite32(VAWD1, 0x00800080);
    regWrite32(VAWD2, 0x0008400C);
    regWrite32(VTCR , 0x8000501F);

    regWrite32(VAWD1, 0x80000000);
    regWrite32(VAWD2, 0x0);
    regWrite32(VTCR , 0x8000901F);

    regWrite32(VAWD1, 0x08002080);
    regWrite32(VAWD2, 0x0);
    regWrite32(VTCR , 0x8000B01F);
}
#endif

int macRT63365STagEnable(int chanID)
{
	uint32 value;

#if defined(MT7530_SUPPORT)	
	/*Enable Special Tag function*/
	value = gswPbusRead(0x2610);
	/* set CPU port 6 as user port */
	value &= ~0xc0;
	/* set CPU port 6 special tag=1 */
	value |= (1<<5);
	gswPbusWrite(0x2610, value);
#if defined(TCSUPPORT_SOC_ETHER_WAN)
	value = gswPbusRead(GSW_PVC(6));
	value |= (1<<PT_OPTION);   /* enable PT_OPTION*/
	gswPbusWrite(GSW_PVC(6), value);

	value = regRead32(GSW_PVC(6));
	value &= ~0xc0;
    value |= (1<<PORT_STAG);
	regWrite32(GSW_PVC(6), value);

	value = regRead32(GSW_PCR(6));
    value |= ((2<<28) | (1<<ACL_EN)); 
	regWrite32(GSW_PCR(6), value);


    value = regRead32(GSW_PVC(5));
	value &= ~0xc0;
    value |= (1<<PORT_STAG);
	regWrite32(GSW_PVC(5), value);

	acl_for_soc_ether_wan();
#endif
#else
	if (isMT7520G || isMT7525G){
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
#endif	
#ifdef TCSUPPORT_MT7510_FE
	/* Enable SPECAIL TAG */
	value = regRead32(CDMA_CSG_CFG);
	value |= CDM_STAG_EN;
	regWrite32(CDMA_CSG_CFG, value);
	
	value = regRead32(GDMA1_FWD_CFG);
	value |= GDM_STAG_EN;
	regWrite32(GDMA1_FWD_CFG, value);
#else
	/* Enable GDM_TCI_81XX */
	value = regRead32(GDMA1_FWD_CFG);
	value |= GDM_TCI_81XX;
	regWrite32(GDMA1_FWD_CFG, value);
#endif
	macSTagFlag = 1;

	return 0;
}/*end macRT63365STagEnable*/
#if defined(MT7530_SUPPORT)	
//Only use for RT63368 + MT7530
int macRT63365STagDisable(int chanID)
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
}/*end macRT63365STagEnable*/
#endif

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
	#if 1//def TCSUPPORT_HW_QOS
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
				txq = maxPrio; //maxPrio is the txq with max weight
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
uint32 DSGetSwitchQos(struct sk_buff *skb)
{	
	uint32 txq = 0, qosmark = 0, ret = 0;;

#ifdef TCSUPPORT_QOS
	if(!ds_switch_qos_flag)
		return 0;

	qosmark = (skb->mark & QOS_FILTER_MARK)>>4;
	if(qosmark > 8 || 0 == qosmark)
		return 0;
	txq = (8-qosmark);
	ret = ((0x1<<15)+(txq<<12));
#endif
	
	return ret;
}
struct sk_buff *macRT63365STagInsert(struct sk_buff *bp)
{
	uint8 *cp;
	uint8 port_mask=0;
	uint8 pvid = 0;
	uint16 tag0, tag1;
	
#ifdef TCSUPPORT_WAN_ETHER  
	uint32 txq = 0;
#endif
#if defined(TCSUPPORT_CT_LOOPDETECT)	
	int temp_mask = 0;
#endif
	
	if (bp->mark & SKBUF_COPYTOLAN){
		//if the packet is used for wan2lan,set port_mask from masko that from cli
		#ifdef WAN2LAN
#if !defined(TCSUPPORT_CT_2PWIFI) 
			port_mask = 0xF;
#endif
		#endif
	}
	else{
		port_mask = 0x0;
	}	
	cp = bp->data;
	/*Reverse Ethernet Port seq*/
	//reverse_portmask(&port_mask);
	/*wan2lan packet need to insert special Tag function.*/
	tag0 = *(uint16 *)(cp+12);
	tag1 = *(uint16 *)(cp+14);

#ifdef WAN2LAN
	if (bp->mark & SKBUF_COPYTOLAN){
		if (skb_headroom(bp) < 4) {
			struct sk_buff *bp2 = skb_realloc_headroom(bp, 4);
			dev_kfree_skb(bp);
			if (bp2 == NULL) {
				return NULL;
			}
			bp = bp2;
		} else {
			bp = skb_unshare(bp, GFP_ATOMIC);
			if (!bp) {
				return NULL;
			}
		}

		skb_push(bp, 4);
		cp = bp->data;
		/*Insert VLan Tags*/
		memmove(cp, cp+4, 12);	
#if defined(TCSUPPORT_CT_LOOPDETECT)		
		if(temp_mask=isDetectPacket(cp, bp->len)){
				port_mask = temp_mask;
			}
#endif

#if defined(TCSUPPORT_CT)
		port_mask = macRT63365STagRemap(port_mask);		
		if( g_stag_to_vtag == 1){
			*(uint16 *)(cp+12) = (0x8000 | port_mask);
			*(uint16 *)(cp+14) = 0;
		}
		else{
			*(uint16 *)(cp+12) = (0x8100 | port_mask);
			*(uint16 *)(cp+14) = 1;
		}
#else
		port_mask = macRT63365STagRemap(port_mask);
		*(uint16 *)(cp+12) = (0x8000 | port_mask);
		*(uint16 *)(cp+14) = 0;
#endif
	} else
#endif
#if defined(TCSUPPORT_WAN_ETHER)	
	if(isComeFromWan(bp) == 1){
	//come from wan can not use vlan tag to check
		if (skb_headroom(bp) < 4) {
			struct sk_buff *bp2 = skb_realloc_headroom(bp, 4);
			dev_kfree_skb(bp);
			if (bp2 == NULL) {
				return NULL;
			}
			bp = bp2;
		} else {
			bp = skb_unshare(bp, GFP_ATOMIC);
			if (!bp) {
				return NULL;
			}
		}

		skb_push(bp, 4);
		cp = bp->data;
		/*Insert VLan Tags*/
		memmove(cp, cp+4, 12);
		*(uint16 *)(cp+12) = (0x8100 | 1<<wan_port_id);
		if(g_stag_to_vtag == 1){
			*(uint16 *)(cp+14) = 5; // wan port vid = 5
		}else{	
			*(uint16 *)(cp+14) = (wan_port_id+1);
		}
#ifdef TCSUPPORT_WAN_ETHER
		txq = getPbitForSwitchQos(bp);
		//HH priority mapping priority 7 - 0xE; H priority mapping priority 6 - 0xC; 
		//M priority mapping priority 5 - 0xA; L priority mapping priority 4 - 0x8;
		//set priority bit for switch QoS by txq
		switch(txq)
		{
			case 0 :
				*(cp+14)|= 0x80;	
				break;
			case 1:
				*(cp+14)|= 0xA0; 
				break;
			case 2:
				*(cp+14)|= 0xC0; 
				break;
			case 3:
				*(cp+14)|= 0xE0;
				break;
			default:
				break;
		}
#endif	

	}else
#endif	
	{
	if (tag0 == 0x8100) {
		bp = skb_unshare(bp, GFP_ATOMIC);
		if (!bp) {
			return NULL;
		}
		cp = bp->data;


		if (0<tag1 && tag1<=RT63365_SWIC_PORTNUM){	
			if(g_port_reverse){			/* Lan port reverse */
				tag1 -= 1;
				if(0<=tag1 && tag1<4)
					tag1 = 3 - tag1;
				pvid = lan_port_map[ tag1 ];				
			}else
			pvid = lan_port_map[ ((int)tag1-1) ];
			if (0<=pvid && pvid<RT63365_SWIC_PORTNUM){					
				//stag no need this item.
				if(g_stag_to_vtag == 1){
					*(uint16 *)(cp+14) = 1; //lan vid = 1
				}else{
					*(uint16 *)(cp+14) = (pvid+1);	
				}	
			} else {
				printk("error: ethernet port cannot find the mapped switch port\n");
			}
		}
		
		if( g_stag_to_vtag == 1){
			//insert stag.
			if ((bp->mark & 0xf0000000) >= 0x10000000 && (bp->mark & 0xf0000000) <= 0x40000000) {
				//Don't set SA autolearning flag if the packet if from ethernet port to cpu
				//    port then back to ethernet port.
				*(uint16 *)(cp+12) = (0x8100 | (1<<9) |(1<<(pvid)));
			}else{	
				*(uint16 *)(cp+12) = (0x8100 | (1<<(pvid)));
			}
		}else{	
			*(uint16 *)(cp+12) = (0x8100 | port_mask);
		}
#ifdef TCSUPPORT_AUTOBENCH
	} else if (tag0 == 0x8200) {
			bp = skb_unshare(bp, GFP_ATOMIC);
			if (!bp) {
				printk("skb unshare fail\n");
				return NULL;
			}
			cp = bp->data;
			*(uint16 *)(cp+12) = (0x8000 | (tag1));
			*(uint16 *)(cp+14) = (0x0);
#endif	
	} else {
		if (skb_headroom(bp) < 4) {
			struct sk_buff *bp2 = skb_realloc_headroom(bp, 4);
			dev_kfree_skb(bp);
			if (bp2 == NULL) {
				return NULL;
			}
			bp = bp2;
		} else {
			bp = skb_unshare(bp, GFP_ATOMIC);
			if (!bp) {
				return NULL;
			}
		}

		skb_push(bp, 4);
		cp = bp->data;
		/*Insert Special Tags*/
		memmove(cp, cp+4, 12);
		*(uint16 *)(cp+12) = (0x8000 | port_mask);
		*(uint16 *)(cp+14) = 0x0000;
		}
	}

	return bp;
}/*end macRT63365STagInsert*/

void macRT63365STagRemove(struct sk_buff *bp)
{
	uint8 *cp=NULL;
	uint8 pvid;
	uint16 tag0;
	uint16 tag1;
	uint16 port_num;

	cp = bp->data;
	tag0 = *(uint16 *)(cp+12);
	tag1 = *(uint16 *)(cp+14);
	
	if (((tag0 & 0xff00) == 0x8000) && (tag1 == 0x0000)) {
		/* VLAN disabled case */
		memmove(bp->data+4, bp->data, 12);
		skb_pull(bp, 4);
	} else {
		/* VLAN enabled case */
			if ((tag0 & 0xff00) == 0x8100){
				port_num = (tag0 & 0x1f);
#if defined(TCSUPPORT_WAN_ETHER)			
				if(port_num == wan_port_id){
					memmove(bp->data+4, bp->data, 12);
					skb_pull(bp, 4);
					setComeFromWan(bp,1);
				}else
#endif				
				{
					if( g_stag_to_vtag == 1){
						// prvent hwnat back packet and use port number to gen vlan id.
						if((tag1 == 1) && (0<=port_num) && (port_num<RT63365_SWIC_PORTNUM)){
							cp = bp->data;
							pvid = switch_port_map[ port_num ];
							if(g_port_reverse){
								if(0<=pvid && pvid<4)					/* Lan port reverse */
									pvid = 3 - pvid;
							}
							if (0<=pvid && pvid<RT63365_SWIC_PORTNUM){
								*(uint16 *)(cp+14) = (pvid+1);
							}
						}
					}else{
						if (0<tag1 && tag1<=RT63365_SWIC_PORTNUM){
							cp = bp->data;			
							pvid = switch_port_map[ ((int)tag1-1) ];
							if (0<=pvid && pvid<RT63365_SWIC_PORTNUM){
								*(uint16 *)(cp+14) = (pvid+1);
							}
						}
					}	
					
					*(uint16 *)(cp+12) = 0x8100;
				}
			}		
	}
}/*end macRT63365STagRemove*/
struct sk_buff *macMT7510STagInsert(struct sk_buff *bp)
{
	uint8 *cp;
	uint8 port_mask=0;
	uint8 pvid = 0;
	uint16 tag0, tag1;
#if defined(TCSUPPORT_CT_LOOPDETECT)	
	int temp_mask = 0;
#endif

	
	if (bp->mark & SKBUF_COPYTOLAN){
		//if the packet is used for wan2lan,set port_mask from masko that from cli
		#ifdef WAN2LAN
		port_mask = masko & 0xF;
		#endif
	}
	else{
		port_mask = 0x0;
	}	
	cp = bp->data;
	/*Reverse Ethernet Port seq*/
	//reverse_portmask(&port_mask);
	/*wan2lan packet need to insert special Tag function.*/
	tag0 = *(uint16 *)(cp+12);
	tag1 = *(uint16 *)(cp+14);

#ifdef WAN2LAN
	if (bp->mark & SKBUF_COPYTOLAN){
		if (skb_headroom(bp) < 4) {
			struct sk_buff *bp2 = skb_realloc_headroom(bp, 4);
			dev_kfree_skb(bp);
			if (bp2 == NULL) {
				return NULL;
			}
			bp = bp2;
		} else {
			bp = skb_unshare(bp, GFP_ATOMIC);
			if (!bp) {
				return NULL;
			}
		}

		skb_push(bp, 4);
		cp = bp->data;
		/*Insert VLan Tags*/
		memmove(cp, cp+4, 12);	
#if defined(TCSUPPORT_CT_LOOPDETECT)		
		if(temp_mask=isDetectPacket(cp, bp->len)){
			port_mask = temp_mask;
		}
#endif
		port_mask = macRT63365STagRemap(port_mask);
		*(uint16 *)(cp+12) = ((1<<6) | (port_mask));
		*(uint16 *)(cp+14) = 0;
#if defined(TCSUPPORT_SOC_ETHER_WAN)
        *(uint16 *)(cp+12) |= STAG_PASSTHROUGH;
#endif
	} else
#if defined(TCSUPPORT_WAN_ETHER)	
	if(isComeFromWan(bp) == 1){
	//come from wan can not use vlan tag to check
		if (skb_headroom(bp) < 4) {
			struct sk_buff *bp2 = skb_realloc_headroom(bp, 4);
			dev_kfree_skb(bp);
			if (bp2 == NULL) {
				return NULL;
			}
			bp = bp2;
		} else {
			bp = skb_unshare(bp, GFP_ATOMIC);
			if (!bp) {
				return NULL;
			}
		}

		skb_push(bp, 4);
		cp = bp->data;
	
		/*Insert VLan Tags*/
		memmove(cp, cp+4, 12);
		*(uint16 *)(cp+12) = (1<<wan_port_id) | (1<<15) | ((getPbitForSwitchQos(bp) & 0x7)<<12); 
		if(g_stag_to_vtag == 1){
			*(uint16 *)(cp+14) = 5; // wan port vid = 5
		}else{	
			*(uint16 *)(cp+14) = (wan_port_id+1);
		}
	}else	
#endif
	{
	if (tag0 == 0x8100) {
		bp = skb_unshare(bp, GFP_ATOMIC);
		if (!bp) {
			return NULL;
		}
		cp = bp->data;


		if (0<tag1 && tag1<=RT63365_SWIC_PORTNUM){	
		/* add by xyzhu for port reverse */
			if(g_port_reverse){			/* Lan port reverse */
				tag1 -= 1;
#if defined(TCSUPPORT_CT_2PORTS) && defined(TCSUPPORT_C7)
				if(0<=tag1 && tag1<2)
					tag1 = 1 - tag1;
#else
				if(0<=tag1 && tag1<4)
					tag1 = 3 - tag1;
#endif
				pvid = lan_port_map[ tag1 ];				
			}else
			pvid = lan_port_map[ ((int)tag1-1) ];
			if (0<=pvid && pvid<RT63365_SWIC_PORTNUM){
				//stag no need this item.
				if(g_stag_to_vtag == 1){
					*(uint16 *)(cp+14) = 1; //lan vid = 1
				}else{
					*(uint16 *)(cp+14) = (pvid+1);	
				}	
			} else {
				printk("error: ethernet port cannot find the mapped switch port\n");
			}
		}
		
		if( g_stag_to_vtag == 1){
			//insert stag.
			if ((bp->mark & 0xf0000000) >= 0x10000000 && (bp->mark & 0xf0000000) <= 0x40000000) {
				//Don't set SA autolearning flag if the packet if from ethernet port to cpu
				//    port then back to ethernet port.
				*(uint16 *)(cp+12) = ((1<<15) | (1<<6) | (1<<(pvid)));
			}else{	
				*(uint16 *)(cp+12) = ((1<<15) | (1<<(pvid)));
			}
		}else{	
			*(uint16 *)(cp+12) = ((1<<15) | (port_mask));
		}
#ifdef TCSUPPORT_AUTOBENCH
	} else if (tag0 == 0x8200) {
			bp = skb_unshare(bp, GFP_ATOMIC);
			if (!bp) {
				printk("skb unshare fail\n");
				return NULL;
			}
			cp = bp->data;
			*(uint16 *)(cp+12) = ((1<<15) | (tag1));
			*(uint16 *)(cp+14) = (0x0);
#endif	
	} else {
		if (skb_headroom(bp) < 4) {
			struct sk_buff *bp2 = skb_realloc_headroom(bp, 4);
			dev_kfree_skb(bp);
			if (bp2 == NULL) {
				return NULL;
			}
			bp = bp2;
		} else {
			bp = skb_unshare(bp, GFP_ATOMIC);
			if (!bp) {
				return NULL;
			}
		}

		skb_push(bp, 4);
		cp = bp->data;
		/*Insert Special Tags*/
		memmove(cp, cp+4, 12);
		*(uint16 *)(cp+12) = ((1<<15) | (port_mask));
		*(uint16 *)(cp+14) = 0x0000;
		}
#if defined(TCSUPPORT_SOC_ETHER_WAN)
        *(uint16 *)(cp+12) |= STAG_PASSTHROUGH;
#endif
	}
#endif
	return bp;
}/*end macMT75105STagInsert*/

void macMT7510STagRemove(struct sk_buff *bp)
{
	uint8 *cp=NULL;
	uint8 pvid;
	uint16 tag0;
	uint16 tag1;
	uint16 port_num;

	cp = bp->data;
	tag0 = *(uint16 *)(cp+12);
	tag1 = *(uint16 *)(cp+14);

	if(vport_enable){
		if ((tag0 & 0xfc78) == 0x0000){//check come from switch
		port_num = (tag0 & 0x7);
#if defined(TCSUPPORT_WAN_ETHER)			
		if(port_num == wan_port_id){
			memmove(bp->data+4, bp->data, 12);
			skb_pull(bp, 4);
			setComeFromWan(bp,1);
		}else
#endif	
		{
			if( g_stag_to_vtag == 1){
				// prvent hwnat back packet and use port number to gen vlan id.
				if((tag1 == 1) && (0<=port_num) && (port_num<RT63365_SWIC_PORTNUM)){
					cp = bp->data;			
					pvid = switch_port_map[ port_num ];
				/* add by xyzhu for support port reserve */
					if(g_port_reverse){
#if defined(TCSUPPORT_CT_2PORTS) && defined(TCSUPPORT_C7)
						if(0<=pvid && pvid<2)
							pvid = 1 - pvid;
#else
						if(0<=pvid && pvid<4)					/* Lan port reverse */
							pvid = 3 - pvid;
#endif
					}
					if (0<=pvid && pvid<RT63365_SWIC_PORTNUM){
						*(uint16 *)(cp+14) = (pvid+1);
					}
				}
			}else{
				if (0<tag1 && tag1<=RT63365_SWIC_PORTNUM){
					cp = bp->data;			
					pvid = switch_port_map[ ((int)tag1-1) ];
					if (0<=pvid && pvid<RT63365_SWIC_PORTNUM){
						*(uint16 *)(cp+14) = (pvid+1);
					}
				}
			}
			*(uint16 *)(cp+12) = 0x8100;
			}
		}
	}else{
		/* VLAN disabled case */
		memmove(bp->data+4, bp->data, 12);
		skb_pull(bp, 4);
	}	
}/*end macMT75105STagRemove*/
#if defined(MT7530_SUPPORT)	

struct sk_buff *macRT63368ExtSTagInsert(struct sk_buff *bp)
{
	uint8 *cp;
	uint8 port_mask=0;
	uint8 pvid = 0;
	uint16 tag0, tag1;
	
	
	if (bp->mark & SKBUF_COPYTOLAN){
		//if the packet is used for wan2lan,set port_mask from masko that from cli
		#ifdef WAN2LAN
		port_mask = masko & 0xF;
		#endif
	}
	else{
		port_mask = 0x0;
	}	
	cp = bp->data;
	/*Reverse Ethernet Port seq*/
	//reverse_portmask(&port_mask);
	/*wan2lan packet need to insert special Tag function.*/
	tag0 = *(uint16 *)(cp+12);
	tag1 = *(uint16 *)(cp+14);

#ifdef WAN2LAN
	if (bp->mark & SKBUF_COPYTOLAN){
		if (skb_headroom(bp) < 4) {
			struct sk_buff *bp2 = skb_realloc_headroom(bp, 4);
			dev_kfree_skb(bp);
			if (bp2 == NULL) {
				return NULL;
			}
			bp = bp2;
		} else {
			bp = skb_unshare(bp, GFP_ATOMIC);
			if (!bp) {
				return NULL;
			}
		}

		skb_push(bp, 4);
		cp = bp->data;
		/*Insert VLan Tags*/
		memmove(cp, cp+4, 12);	
		port_mask = macRT63365STagRemap(port_mask);
		*(uint16 *)(cp+12) = ((1<<6) | (port_mask));
		*(uint16 *)(cp+14) = 0;
	} else
#if defined(TCSUPPORT_WAN_ETHER)	
	if(isComeFromWan(bp) == 1){
	//come from wan can not use vlan tag to check
		if (skb_headroom(bp) < 4) {
			struct sk_buff *bp2 = skb_realloc_headroom(bp, 4);
			dev_kfree_skb(bp);
			if (bp2 == NULL) {
				return NULL;
			}
			bp = bp2;
		} else {
			bp = skb_unshare(bp, GFP_ATOMIC);
			if (!bp) {
				return NULL;
			}
		}

		skb_push(bp, 4);
		cp = bp->data;
	
		/*Insert VLan Tags*/
		memmove(cp, cp+4, 12);
		if(g_stag_to_vtag == 1){
			*(uint16 *)(cp+12) = (1<<wan_port_id) | (1<<15) | ((getPbitForSwitchQos(bp) & 0x7)<<12); 
			*(uint16 *)(cp+14) = 5; // wan port vid = 5
		}else{
			*(uint16 *)(cp+12) = 0x8100; 
			*(uint16 *)(cp+14) = (wan_port_id+1);
#if 1			
			//HH priority mapping priority 7 - 0xE; H priority mapping priority 6 - 0xC; 
			//M priority mapping priority 5 - 0xA; L priority mapping priority 4 - 0x8;
			//set priority bit for switch QoS by txq
			switch(getPbitForSwitchQos(bp))
			{
				case 0 :
					*(cp+14)|= 0x80;	
					break;
				case 1:
					*(cp+14)|= 0xA0; 
					break;
				case 2:
					*(cp+14)|= 0xC0; 
					break;
				case 3:
					*(cp+14)|= 0xE0;
					break;
				default:
					break;
			}
#endif			
		}
		
	}else	
#endif
	{
		if (tag0 == 0x8100) {
			bp = skb_unshare(bp, GFP_ATOMIC);
			if (!bp) {
				return NULL;
			}
			cp = bp->data;


			if (0<tag1 && tag1<=RT63365_SWIC_PORTNUM){	
				pvid = lan_port_map[ ((int)tag1-1) ];
				if (0<=pvid && pvid<RT63365_SWIC_PORTNUM){
					//stag no need this item.
					if(g_stag_to_vtag == 1){
						*(uint16 *)(cp+14) = 1; //lan vid = 1
					}else{
						*(uint16 *)(cp+14) = (pvid+1);	
					}	
				} else {
					printk("error: ethernet port cannot find the mapped switch port\n");
				}
			}
			
			if( g_stag_to_vtag == 1){
				//insert stag.
				if ((bp->mark & 0xf0000000) >= 0x10000000 && (bp->mark & 0xf0000000) <= 0x40000000) {
					//Don't set SA autolearning flag if the packet if from ethernet port to cpu
					//    port then back to ethernet port.
					*(uint16 *)(cp+12) = ((1<<15) | (1<<6) | (1<<(pvid)));
				}else{	
					*(uint16 *)(cp+12) = ((1<<15) | (1<<(pvid)));
				}
			}else{	
				*(uint16 *)(cp+12) = 0x8100;
			}
		} else {
			if(g_stag_to_vtag){
				if (skb_headroom(bp) < 4) {
					struct sk_buff *bp2 = skb_realloc_headroom(bp, 4);
					dev_kfree_skb(bp);
					if (bp2 == NULL) {
						return NULL;
					}
					bp = bp2;
				} else {
					bp = skb_unshare(bp, GFP_ATOMIC);
					if (!bp) {
						return NULL;
					}
				}

				skb_push(bp, 4);
				cp = bp->data;
				/*Insert Special Tags*/
				memmove(cp, cp+4, 12);
				*(uint16 *)(cp+12) = 0x0;
				*(uint16 *)(cp+14) = 0x0;
			}
		}
	}
#endif
	return bp;
}/*end macMT75105STagInsert*/

void macRT63368ExtSTagRemove(struct sk_buff *bp)
{
	uint8 *cp=NULL;
	uint8 pvid;
	uint16 tag0;
	uint16 tag1;
	uint16 port_num;

	cp = bp->data;
	tag0 = *(uint16 *)(cp+12);
	tag1 = *(uint16 *)(cp+14);

	if( g_stag_to_vtag == 0){
		if(vport_enable){
#if defined(TCSUPPORT_WAN_ETHER)			
				if(tag1 == (wan_port_id + 1)){
					
					memmove(bp->data+4, bp->data, 12);
					skb_pull(bp, 4);
					setComeFromWan(bp,1);
				}else
#endif	
				{
					if (0<tag1 && tag1<=RT63365_SWIC_PORTNUM){
						cp = bp->data;			
						pvid = switch_port_map[ ((int)tag1-1) ];
						if (0<=pvid && pvid<RT63365_SWIC_PORTNUM){
							*(uint16 *)(cp+14) = (pvid+1);
						}
					}
					*(uint16 *)(cp+12) = 0x8100;
				}
		}
	}else{	
		if(vport_enable){
			if ((tag0 & 0xfc78) == 0x0000){//check come from switch
				port_num = (tag0 & 0x7);
#if defined(TCSUPPORT_WAN_ETHER)			
				if(port_num == wan_port_id){
					memmove(bp->data+4, bp->data, 12);
					skb_pull(bp, 4);
					setComeFromWan(bp,1);
				}else
#endif	
				{
					// prvent hwnat back packet and use port number to gen vlan id.
					if((tag1 == 1) && (0<=port_num) && (port_num<RT63365_SWIC_PORTNUM)){
						cp = bp->data;			
						pvid = switch_port_map[ port_num ];
						if (0<=pvid && pvid<RT63365_SWIC_PORTNUM){
							*(uint16 *)(cp+14) = (pvid+1);
						}
					}
					*(uint16 *)(cp+12) = 0x8100;
				}
			}
		}else{
			/* VLAN disabled case */
			memmove(bp->data+4, bp->data, 12);
			skb_pull(bp, 4);
		}	
	}
}/*end macMT75105STagRemove*/
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
		if (bp2 == NULL)
			return NULL;
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

#if 0 //defined(TCSUPPORT_AUTOBENCH) && defined(TCSUPPORT_CPU_MT7520)
void dump_skb_len(struct sk_buff *skb,int len)
{
	char tmp[80];
	char *p = skb->data;
	char *t = tmp;
	int i, n = 0;
	printk("ERR skb=%08lx data=%08lx len=%d\n", (uint32) skb, (uint32) skb->data, skb->len);
	for (i = 0; i < len; i++) {
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
#endif

struct sk_buff * macMT7520STagInsert(struct sk_buff *bp)
{
	uint8 *cp;
	uint8 port_mask=0;
	uint8 pvid;
	uint16 tag0, tag1;
	
	uint8 vpm = 0;
	uint16 tpid = 0;
	uint8 flag = 0;
#if defined(TCSUPPORT_CT_LOOPDETECT)	
	int temp_mask = 0;
#endif
	
	cp = bp->data;
	tag0 = *(uint16 *)(cp+12); // tpid
	tag1 = *(uint16 *)(cp+14); // tag1 = x = eth0.x

	if((vport_enable//multi port case
#ifdef TCSUPPORT_AUTOBENCH
		|| (tag0 == 0x8200)
#endif
		)
#ifdef WAN2LAN
		&& (!(bp->mark & SKBUF_COPYTOLAN))
#endif
#if defined(TCSUPPORT_CPU_MT7520)
		&& (!(bp->mark & SKBUF_TCCONSOLE))
#endif
#if defined(TCSUPPORT_WAN_ETHER)	
		&& (isComeFromWan(bp) != 1)
#endif
#if defined(TCSUPPORT_SEPERATED_ETH_ITF)
		&& (0)
#endif /* TCSUPPORT_SEPERATED_ETH_ITF */
		)
	{
		bp = skb_unshare(bp, GFP_ATOMIC);
		if (!bp)
			return NULL;
		cp = bp->data;

#ifdef TCSUPPORT_AUTOBENCH
		if(tag0 == 0x8200)
			flag = 1;
#endif

		/*
			multi port case vlan_dev will add a tag to skb. so we check the second tag.
			if packet is tagged,we will remove the tag added by vlan_dev.
			if not,we will keep the tag which added by vlan_dev
			when packet sent to switch,it must be sent to CPU port first.so we only check
			cpu port tpid here.
		*/
		if(*(uint16 *)(cp+16) == 0x8100 || *(uint16 *)(cp+16) == 0x88a8 || *(uint16 *)(cp+16) == cpu_port_tpid)//tagged packet
		{
			memmove(bp->data+4, bp->data, 12);
			skb_pull(bp, 4);
			cp = bp->data;
			tag0 = *(uint16 *)(cp+12);
		}
		else
			tag0 = 0xffff;//oxffff will make vpm = 0
			
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
				//printk("Inset Tag: Tag1 is %d, pvid is %d\r\n", tag1, pvid);
#else
			pvid = lan_port_map[ ((int)tag1-1) ]; // get switch port id
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
					//Don't set SA autolearning flag if the packet if from ethernet port to cpu
					//    port then back to ethernet port.
					*(uint16 *)(cp+12) = (vpm << 8) + (1 << 6) + (1 << (pvid));
				else
					*(uint16 *)(cp+12) = (vpm << 8) + (1 << (pvid)) + (DSGetSwitchQos(bp));
			}
			else
				*(uint16 *)(cp+12) = (port_mask);
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
					*(uint16 *)(cp+14) = 1; //lan vid = 1
				else
					*(uint16 *)(cp+14) = (pvid+1);	
			}
		}
#ifdef TCSUPPORT_DS_HWNAT_OFFLOAD
		bp->mark &= ~LANIF_MASK;
		bp->mark |= ((pvid<<DEST_OFFSET)&LANIF_MASK);	/* used to record dest switch port */
#endif
	}
	else
	{
#ifdef WAN2LAN
		if (bp->mark & SKBUF_COPYTOLAN){
			port_mask = masko & 0xF;
#if defined(TCSUPPORT_CT_LOOPDETECT)		
			if(temp_mask=isDetectPacket(cp, bp->len)){
				port_mask = temp_mask;
			}
#endif
			port_mask = macRT63365STagRemap(port_mask); 	
		}
#endif

#if defined(TCSUPPORT_CPU_MT7520)
		if ((bp->mark & SKBUF_TCCONSOLE)){
			port_mask = masko_tcconsole & 0xF;
			port_mask = macRT63365STagRemap(port_mask);
		}
#endif
#if defined(TCSUPPORT_SEPERATED_ETH_ITF)
		if((skbFromWhichSepEthItf(bp) == SEP_ETH_ITF_0)
			||(skbFromWhichSepEthItf(bp) == SEP_ETH_ITF_1)
			||(skbFromWhichSepEthItf(bp) == SEP_ETH_ITF_2)
			||(skbFromWhichSepEthItf(bp) == SEP_ETH_ITF_3)
		)
		{
#if 0		
			if(isMT7525G)
		        port_mask |= 1 << (skbFromWhichSepEthItf(bp) ); 
			else
				port_mask |= 1 << (skbFromWhichSepEthItf(bp)+1);		
#endif
			int eth_inf =0;
			int port_num =0;

			eth_inf = skbFromWhichSepEthItf(bp);
			port_num = lan_port_map[eth_inf];
			if((0<=port_num) && (port_num<RT63365_SWIC_PORTNUM))
			{
				port_mask |= 1 << port_num;	
#ifdef TCSUPPORT_DS_HWNAT_OFFLOAD
				bp->mark &= ~LANIF_MASK;
				bp->mark |= ((port_num<<DEST_OFFSET)&LANIF_MASK);	/* used to record dest switch port */
#endif
			}		
		}
#endif /* TCSUPPORT_SEPERATED_ETH_ITF */
		/*
			there can be wan2lan,etherwan,single-port case.
			kernel will not add a tag to skb.we only check first tag.
			if packet is untag,add a stag.
		*/
#if !defined(TCSUPPORT_SEPERATED_ETH_ITF)
		if(!(tag0 == 0x8100 || tag0 == 0x88a8 || tag0 == cpu_port_tpid))//untag packet
#endif /* TCSUPPORT_SEPERATED_ETH_ITF */
		{
			bp = Insert_4Byte_to_Skb(bp);
			if(bp == NULL)
				return NULL;
			cp = bp->data;
			tag0 = 0xffff;//used to set vpm = 0
		}
		vpm = Calc_VPM(tag0,cpu_port_tpid);
		
#if defined(TCSUPPORT_WAN_ETHER)
		//1  EtherWan case,need to test
		if(isComeFromWan(bp) == 1)
			*(uint16 *)(cp+12) = (vpm << 8) + (1<<wan_port_id);
		else
#endif
        {
            {
                *(uint16 *)(cp+12) = (vpm << 8) + (port_mask) + (DSGetSwitchQos(bp));
            }
        }

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
#if 0//defined(TCSUPPORT_SEPERATED_ETH_ITF)
	{
		printk("TX: AFTER ");
		dump_skb_sepitf(bp);
	}
#endif
#if defined(TCSUPPORT_GPON_MAPPING) && defined(TCSUPPORT_GPON_DOWNSTREAM_MAPPING)
	if(gpon_downstream_mapping_stag_hook)//only change the Queue index here
		gpon_downstream_mapping_stag_hook(bp);
#if defined(TCSUPPORT_CT_DS_LIMIT)
	dataspeed_limit_queue_hook(bp); /* remark the queue index. */
#endif
#endif
#ifdef TCSUPPORT_XPON_LED
	port_mask = (*(uint16 *)(cp+12)) & 0x3F;
	if (isMT7520S){
		if (port_mask & (1<<4) && led_ether_up_status[3] == 2){
			ledTurnOn(LED_ETHER_PORT4_ACT_STATUS);
		}
	}else if (isMT7525 || isMT7520){
		int i = 0;
		for (i = 0; i < 4; i++){
			if ((port_mask & (1<<(i+1))) && led_ether_up_status[i] ==2){
                {
					{
                    	ledTurnOn(LED_ETHER_PORT1_ACT_STATUS + 2*i);
					}
                }
			}
		}
	}
#endif

	return bp;
}


int macMT7520STagRemove(struct sk_buff *bp)
{
	uint8 *cp=NULL;
	uint8 pvid = RT63365_SWIC_PORTNUM+1;
	uint16 sp_tag = 0;
	uint16 tci = 0;
	uint16 port_num = 0;
	
	uint16 tpid = 0;
	uint8 vpm = 0;

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
	port_num = (sp_tag & 0x7); //switch port id

	if(0xfc78 & sp_tag){ //there is no special-tag
		return 0;
	}

	if( g_stag_to_vtag == 1)
	{
		if((0<=port_num) && (port_num<RT63365_SWIC_PORTNUM)){
			pvid = switch_port_map[ port_num ]; // get lan port id 			
			//printk("Remove Tag: Origin pvid is %d port_num is %d\r\n", pvid, port_num);
#if defined(TCSUPPORT_CUC_C5_2P)
			if(0<=pvid && pvid<4) 
			{
				uint8 tempvid = 0;
				if(pvid == 3) tempvid = 0;
				else if(pvid == 0) tempvid = 1;
				else if(pvid == 1) tempvid = 2;
				else if(pvid == 2) tempvid = 3;

				pvid = tempvid;

				//printk("Remove Tag: pvid is %d port_num is %d\r\n", pvid, port_num);
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
	else//wifi offload case.do the same thing as 7510
	{
		//1 wifi offload case,need to test
		if (0<tci && tci<=RT63365_SWIC_PORTNUM)
		{		
			pvid = switch_port_map[ ((int)tci-1) ];
		}
	}
#ifdef TCSUPPORT_XPON_LED
	if (isMT7520S && pvid == 0){
		if (led_ether_up_status[3] == 2){
			ledTurnOn(LED_ETHER_PORT4_ACT_STATUS);
		}
	}else if ((isMT7525 || isMT7520) && (pvid>=0 &&pvid < 4)){
		if (led_ether_up_status[pvid] == 2){
            {
				{
				    ledTurnOn(LED_ETHER_PORT1_ACT_STATUS + 2*(pvid));
				}
            }
		}
	}
#endif

#if !defined(TCSUPPORT_SEPERATED_ETH_ITF)
	if (0<=pvid && pvid<RT63365_SWIC_PORTNUM)
		pvid++; // translate to eth0.x (x)
	else
		return -1;
#endif /* TCSUPPORT_SEPERATED_ETH_ITF */

	if(vport_enable
#if defined(TCSUPPORT_WAN_ETHER)
		&& port_num != wan_port_id
#endif
#if defined(TCSUPPORT_SEPERATED_ETH_ITF)
		&& (0)
#endif /* TCSUPPORT_SEPERATED_ETH_ITF */
		)
	{
		/* add vlan_dev tag to send packet to eth0.x && 
		    restore tpid
		*/
		if(vpm != 0)
		{
			bp = Insert_4Byte_to_Skb(bp);
			if(bp == NULL)
				return -1;
			cp = bp->data;
			switch(vpm)//check vpm
			{
				case 0://untag
					break;
					
				case 1://8100
					*(uint16 *)(cp+16) = 0x8100;
					break;

				case 2://88a8
					*(uint16 *)(cp+16) = 0x88a8;
					break;

				case 3://defined by register
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
#if 0
	if(isMT7525G)
	{
		if(port_num >= 0 && port_num <= 3)
			setSkbFromWhichSepEthItf(bp,port_num);
	}
	else{
		//printk("%s: port=%d", __FUNCTION__, (port_num-1));
			if(port_num > 0 && port_num <= 4)
				setSkbFromWhichSepEthItf(bp,(port_num-1));
		}
#else
	int eth_intf = 0;

	if(port_num > 0 && port_num <= 4)
	{
		for(;eth_intf <= 4; eth_intf++)
		{
			if(lan_port_map[eth_intf] == port_num)
			{
				setSkbFromWhichSepEthItf(bp,eth_intf);
				break ;
			}
		}
	}
#endif		
	
#endif /* TCSUPPORT_SEPERATED_ETH_ITF */
#if !defined(TCSUPPORT_SEPERATED_ETH_ITF)
		/* no vlan_dev, so no need to remove or modify stag to original */
		switch(vpm)//check vpm
		{
			case 0://untag
				memmove(bp->data+4, bp->data, 12);
				skb_pull(bp, 4);
				break;

			case 1://8100
				*(uint16 *)(cp+12) = 0x8100;
				break;

			case 2://88a8
				*(uint16 *)(cp+12) = 0x88a8;
				break;

			case 3://defined by register
				tpid = Get_Tpid_By_Port(pvid);
				*(uint16 *)(cp+12) = tpid;
				break;

			default:
				return -1;
						
		}
#else
	if((vpm < 0) ||(vpm > 3))
		return -1;
	memmove(bp->data+4, bp->data, 12);
	skb_pull(bp, 4);
#endif
	}
	return 0;
}/*end macMT7520STagRemove*/

#endif

#ifdef TCSUPPORT_DS_HWNAT_OFFLOAD
void doTxRingRateLimit(int txRing, int speed)
{
	int shift, rem;
	uint32 reg, val1, val2;

	shift = txRing/2;
	rem = txRing%2;
	reg = 0xbfb50a80 + 4*shift;

	val1 = read_reg_word(reg);

	if(1 == speed)
	{
		val1&=~(0xffff<<(rem?16:0));
		val2 = ((100*125/(8*4))<<(rem?16:0));    /* 100M bps*/
		val1|=val2;
		write_reg_word(reg, val1);
	}
	else if(0 == speed)
	{
		val1&=~(0xffff<<(rem?16:0));
		val2 = ((10*125/(8*4))<<(rem?16:0));    /* 10M bps*/
		val1|=val2;
		write_reg_word(reg, val1);
	}
	else
	{
		val1&=~(0xffff<<(rem?16:0));
		val2 = (((0x1<<14)|(txRing<<12)|(0x3<<10))<<(rem?16:0));      /* 1000M bps, not limit*/
		val1|=val2;
		write_reg_word(reg, val1);
	}
	
	return;
}
#endif

int macRxPortEnable(int ChanID)
{
	int status = 0;
	macRxPortFlag = 1;
	switch (swicVendor) {
	case SWIC_ADM6996M:
		status = macAdmSTagEnable(ChanID);
		break;
	case SWIC_IP175C:
		status = macIpSTagEnable(ChanID);
		break;
#ifdef SWITCH_3052
	case SWIC_RT3052:
		status = macRt3052STagEnable(ChanID);
		break;
#endif	
	case SWIC_TC2206:
		status=macTC2206STagEnable(ChanID);
		break;	
#if defined(CONFIG_MIPS_RT63365)
	case SWIC_RT63365:
	case SWIC_MT7530:	
		status=macRT63365STagEnable(ChanID);
		break;	
#endif
#ifdef RT62806_VERIFY
	case SWIC_RT62806:
		status=macRT62806STagEnable(ChanID);
		break;	
#endif		
	default:
		break;
	}
	return status;
}

/************************************************************************
*     T C 2 2 0 6   G E T   C O N N E C T E D    F U N C T I O N S
*************************************************************************
*/
/*_____________________________________________________________________________
**      function name: filedSwicDefVal
**      descriptions:
**       We need to refilled  the TC2206 switch ic register default value, it be
**         overwrited by hardware trap.
**            
**      parameters:
**         none
**             
**      global:
**         None
**             
**      return:
**             None
**	     
**      call:
**   	     None
**      		
**      revision:
**      1. Here 2009/7/30
**      2. Here 2010/1/30
**____________________________________________________________________________
*/
void
filedSwicDefVal(void){
 	int i=0;
	uint16 ver=0;
	uint16 val=0;
	/*Get the TC2206 switch IC model id*/
	ver=(uint16)(1<<(tcMiiStationRead(31, 30)));

	/*Let IP175C PING compatible, so let those register to reset default value*/
	for(i=0; tc2206_swic_reg[i].phyaddr!=0; i++){
		if(ver & tc2206_swic_reg[i].model_id){
			tcMiiStationWrite(tc2206_swic_reg[i].phyaddr, tc2206_swic_reg[i].regaddr, tc2206_swic_reg[i].value);
		}
	}
	/*The L2P2/L3P3/L4P4 CPU is not supported Turbo MII mode, we need to disabled it*/
#if defined(TCSUPPORT_CT)
	if(isTC3162L2P2 | isTC3162L3P3 | isTC3162L4P4){
		val = tcMiiStationRead(22, 1) & 0xfffd;
		tcMiiStationWrite(22, 1, val);
	}
#else
	/*Do not use isTC3162L2P2 definition. It will be true in 3182 or 3262 chip. 
		Enable TMII in TC3162U and TC3182. shnwind 20100221.*/
	val = tcMiiStationRead(22, 1);	
	if( isTC3162U || isTC3182 || isRT65168 || isRT63260){
		val |= 0x2;
		tcMiiStationWrite(22, 1, val);
	}else if( isTC3162L3P3 || isTC3162L4P4 ){	
		val &= 0xfffd;
	}
	tcMiiStationWrite(22, 1, val);
#endif
}/*end filedSwicDefVal*/
/*_____________________________________________________________________________
**      function name: getTC2206InitRegFlag
**      descriptions:
**       System will accroding the esd_chk_reg to check the switch status is handing or not
**            
**      parameters:
**         none
**             
**      global:
**         esd_chk_reg
**             
**      return:
**             0: Switch IC is happend handing problem.
**             1: Work fine.
**	     
**      call:
**   	     tcMiiStationRead
**      		
**      revision:
**      1. Here 2009/8/6
**____________________________________________________________________________
*/
int 
getTC2206InitRegFlag(void){
	int i=0;
	int j=0;
	uint16 ver = 0;
	ver = (uint16)(1<<(tcMiiStationRead(31,30)));
	for(i=0; esd_chk_reg[i].phyaddr!=0; i++){
		for(j=0; tc2206_swic_reg[j].phyaddr!=0; j++){
			/*Check the switch ic register is reset to default*/
			if((esd_chk_reg[i].model_id & ver)
			&&(tc2206_swic_reg[j].model_id)
			&&(esd_chk_reg[i].phyaddr == tc2206_swic_reg[j].phyaddr) 
			&&(esd_chk_reg[i].regaddr == tc2206_swic_reg[j].regaddr)){
			#if defined(CONFIG_MIPS_RT65168)
				//for bug#10434, Port5 (CPU port) MAC Control Register has to be modified according LAN ports' link rate, so don't do the checking here.
				if(esd_chk_reg[i].phyaddr==21 && esd_chk_reg[i].regaddr==23)
					continue;
			#endif
				if(tcMiiStationRead(esd_chk_reg[i].phyaddr, esd_chk_reg[i].regaddr)!=tc2206_swic_reg[j].value){
					return 0;
				}
			}
		}
	
	}
	return 1;
}/*end getTC2206InitRegFlag*/

#define CHK_BUF() pos = begin + index; if (pos < off) { index = 0; begin = pos; }; if (pos > off + count) goto done;

#if !defined(TCSUPPORT_CT) || defined(TCSUPPORT_CT_PON)
int eth_port_stat_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	uint32 regValue=0;
	int i =0;
	int port_num;
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	uint8 portstatus[4]={0};


    if(macSTagFlag){
	#ifdef TCSUPPORT_2PORTS
		port_num = 2;
	#else
	    port_num = 4;
	#endif
	}else{
	    port_num = 1;
	}
	if (isMT7520S){
		port_num = 1; //only one giga port
	}
	

	if(swicVendor!=0){
		for (i=0; i<port_num; i++){

#if defined(TCSUPPORT_MT7510_FE) || defined(MT7530_SUPPORT)
#if defined(MT7530_SUPPORT)
		regValue = gswPbusRead(0x3000 + lan_port_map[i]*0x100 + 0x8);
#else
		//use swtich port status instead of phy port status(7510 &7520)
#if defined(CONFIG_MIPS_RT63365)
		if (isMT7520G || isMT7525G){
			if (lan_port_map[i] != -1)
				regValue = gswPbusRead(GSW_PMSR(lan_port_map[i])-GSW_BASE);
			else
				regValue = 0;
		}else if (isMT7520S || isMT7520 || isMT7525){
			if (lan_port_map[i] != -1)
				regValue = read_reg_word(GSW_PMSR(lan_port_map[i]));
			else
				regValue = 0;
		}else{
			regValue = read_reg_word(GSW_PMSR(i));
		}
#else
		regValue = read_reg_word(GSW_PMSR(i));
#endif			
#endif
		/*Detect the ethernet Link change.*/

#if defined(TCSUPPORT_CUC_C5_2P)
			if(i == 3) portstatus[0] = (regValue & MAC_LINK_STS);
			else if(i == 0) portstatus[1] = (regValue & MAC_LINK_STS);
			else if(i == 1) portstatus[2] = (regValue & MAC_LINK_STS);
			else if(i == 2) portstatus[3] = (regValue & MAC_LINK_STS);
#else
		if (g_port_reverse)
			portstatus[port_num - i - 1] = (regValue & MAC_LINK_STS);
		else
			portstatus[i] = (regValue & MAC_LINK_STS);
			
#endif
#else
#if defined(CONFIG_MIPS_RT63365)
			if (isRT63365||isMT751020 || isMT7505){
				if (lan_port_map[i] != -1){
					regValue=tcMiiStationRead(lan_port_map[i], MII_BMSR);
				} else {
					printk("Invalid: port id = -1\n");
					regValue=0;
				}	
			} else 
#endif			
			{
				regValue=tcMiiStationRead(i, MII_BMSR);
			}	
			/*Detect the ethernet Link change.*/
#if defined(TCSUPPORT_CUC_C5_2P)
			if(i == 3) portstatus[0] = ((regValue&BMSR_LSTATUS)>>2);
			else if(i == 0) portstatus[1] = ((regValue&BMSR_LSTATUS)>>2);
			else if(i == 1) portstatus[2] = ((regValue&BMSR_LSTATUS)>>2);
			else if(i == 2) portstatus[3] = ((regValue&BMSR_LSTATUS)>>2);
#else
			if (g_port_reverse)
			    portstatus[port_num - i - 1] = ((regValue&BMSR_LSTATUS)>>2);
			else
				portstatus[i] = ((regValue&BMSR_LSTATUS)>>2);
#endif
			//printk("reg value %x \n",portstatus[i]);
#endif
		}	
	}
	
	index += sprintf(buf+index, "%d %d %d %d\n", portstatus[0], portstatus[1], portstatus[2], portstatus[3]);
	CHK_BUF();

	*eof = 1;

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0) 
		index = 0;
	if (index>count) 
		index = count;
	return index;
}
#endif

extern int g_port_reverse_kernel; //

/* support port reverse */
int port_reverse_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;

	index += sprintf(buf+index, "%d\n", g_port_reverse);
	CHK_BUF();

	*eof = 1;

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0) 
		index = 0;
	if (index>count) 
		index = count;
	return index;
}
int port_reverse_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	int len;
	char get_buf[32];

	/* do a range checking, don't overflow buffers in kernel modules */
	if(count > 32)
		len = 32;
	else
		len = count;
	
	if(copy_from_user(get_buf, buffer, len))
		return -EFAULT;
	
	get_buf[len]='\0';

	//printk("get_buf is aa%s\n", get_buf);

	g_port_reverse = atoi(get_buf);


#if defined(TCSUPPORT_HWNAT)
 	 port_reverse = g_port_reverse;
#endif

	g_port_reverse_kernel = g_port_reverse;

	/* debug */
	//printk("g_port_reverse is %d\n", g_port_reverse);
	
	return len;
}

/* support stag to vtag */
int stag_to_vtag_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;

	index += sprintf(buf+index, "%d\n", g_stag_to_vtag);
	CHK_BUF();

	*eof = 1;

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0) 
		index = 0;
	if (index>count) 
		index = count;
	return index;
}
int stag_to_vtag_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	int len;
	char get_buf[32];

	/* do a range checking, don't overflow buffers in kernel modules */
	if(count > 32)
		len = 32;
	else
		len = count;
	
	if(copy_from_user(get_buf, buffer, len))
		return -EFAULT;
	
	get_buf[len]='\0';

	//printk("get_buf is aa%s\n", get_buf);

	g_stag_to_vtag = atoi(get_buf);

#if defined(MT7530_SUPPORT)
	if(isRT63368){
		if(g_stag_to_vtag){
			macRT63365STagEnable(0);
		}else{
			macRT63365STagDisable(0);
		}
	}
#endif
	/* debug */
	//printk("g_port_reverse is %d\n", g_port_reverse);
	
	return len;
}


void phy_reset_esd(void){
	/* ----- Hardware reset Ehernet phy chip, this address is defined by h/w engineer ----- */
	ledTurnOn(LED_LAN_RESET);
	pause(100);
	/* ----- Wait for hardware reset completed ----- */
	ledTurnOff(LED_LAN_RESET);
	pause(600);
    /*Reload the switch default value for hardware trap*/
    filedSwicDefVal();
    /*Reload the tce phy default value for afe settings and power issue*/
    tcPhyInit(mac_p);
	
}

#if defined(CONFIG_MIPS_RT63365)
#if defined(TCPHY_SUPPORT)
/*_____________________________________________________________________________
**      function name: dulplex_sw_patch
**      descriptions:
**		When remote PC disable auto-negotiation and use full dulpex mode to
**		connect to CPE ethernet port, the giga switch will use half dulplex mode.
**		When the giga switch use half dulplex mode and remote PC use full dulplex
**		mode, the switch sometimes crash after traffic transmmission between CPE
**		and remote PC 
**	   
**      parameters:
**         none
**             
**      global:
**         None
**             
**      return:
**             None
**	     
**      call:
**   	     None
**      		
**      revision:
**      1. Frank 2009/7/30
**____________________________________________________________________________
*/
void 
duplex_sw_patch(void)
{
	uint32 value;
	uint32 rval, val;
	uint8 port_num;
	
	/*Enable Special Tag function*/
	value = regRead32(GSW_BASE + 0x700c);
	if (value & 0x3f){
		for (port_num=0; port_num<=4; port_num++){
			if ( !(value & (1<<port_num)) ){
				continue;
			}
			
			rval = tcPhyReadReg(port_num, 6);
			// auto_nego
			if ( !(rval & 0x1) ){
				val = tcPhyReadReg(port_num,26);
				val |= (1<<8);
				val |= (1<<2);
			} else {
				val = tcPhyReadReg(port_num,26);
				val &= (0xfffffffb);
			}
			tcPhyWriteReg(port_num ,26 ,val);
		}	
		regWrite32((GSW_BASE + 0x700c), (0x3f));	
	}
}


void
enable_abnormal_irq(void)
{
	uint8 port_num;
	uint32 val;

	for (port_num=0; port_num<=4; port_num++){
		regWrite32(GSW_PINT_EN(port_num),0xbeff);
		val = regRead32(GSW_PINT_EN(port_num));
	}
}


int
polling_abnormal_irq(void)
{
	uint32 value;
	uint32 cpc_val;

	value = regRead32(GSW_BASE + 0x30f8);

	if ( (value & 0x3f) ){
		cpc_val = regRead32(GSW_BASE + 0x7010);

		cpc_val &= (0x7fffffff);
		regWrite32((GSW_BASE + 0x7010), cpc_val);
		pause(1);
		cpc_val |= (0x80000000);
		regWrite32((GSW_BASE + 0x7010), cpc_val);

		esd_has_reset = 1;
		
		return -1;
	}

	return 0;
}


int gsw_check_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;

  	index += sprintf(buf+index, "%d\n", esd_has_reset);
	CHK_BUF();

	*eof = 1;
	//printk("esd_read_value %d\n",esd_has_reset);

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0) 
		index = 0;
	if (index>count) 
		index = count;
	return index;
}


int gsw_check_write_proc(struct file *file, const char *buffer,
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
	//printk("esd_write_value %d\n",val);
	if(val == 1)
		esd_has_reset = 0;
	else{
		esd_has_reset = 1;
	}	

	return count;
}

#endif
#endif
/*_____________________________________________________________________________
**      function name: esdSwPatch
**      descriptions:
**       During the ESD test that is due to switch/phy handing on at TC2206 switch ic, so we need to use 
**	   hardware reset to wake up the hardware.So system need to reload the switch/phy register settings.
**	   
**      parameters:
**         none
**             
**      global:
**         None
**             
**      return:
**             None
**	     
**      call:
**   	     None
**      		
**      revision:
**      1. Here 2009/7/30
**____________________________________________________________________________
*/
void 
esdSwPatch(void){
	/*The esd software patch is only work at TC2206A1 version*/
	if((swicVendor==SWIC_TC2206) && (esd_sw_patch)){
		if((!getTC2206InitRegFlag())||(getTcPhyEsdDetectFlag())){
			dbg_pline_1("\r\nESD detected!!\r\n");
			
			/* ----- Hardware reset Ehernet phy chip, this address is defined by h/w engineer ----- */
			ledTurnOn(LED_LAN_RESET);
			pause(100);
			/* ----- Wait for hardware reset completed ----- */
			ledTurnOff(LED_LAN_RESET);
			pause(600);

			/*Reload the switch default value for hardware trap*/
			filedSwicDefVal();
			/*Reload the tce phy default value for afe settings and power issue*/
			tcPhyInit(mac_p);
			#if defined(WAN2LAN) || defined(CONFIG_TC3162_ADSL)
#ifndef TC2031_SUPPORT
			/*Enable special Tag*/
			/* ------ xyzhu_nj_091104:Enable special Tag default ----- */

			if(macRxPortFlag== 1){
				macRxPortEnable(0);
			}
#endif
			#endif
			#if 0 /*Accroding the user settings to set switch registers.*/
			#ifdef VLAN_8021Q
			/*Load vlan settings*/
			vlan1qHwInit();
			#endif
			#ifdef IGMP_SNOOPING
			/*Load igmpsnooping settings*/
			if(igmpsnoopCtl.flags & HW_IGMPSNOOP){
				igmpsnoopHwInit();
			}
			#endif
			#endif
			esd_has_reset = 1;
		}
	}	
}/*end filedSwicDefVal*/

int eth_esd_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;

  	index += sprintf(buf+index, "%d\n", esd_has_reset);
	CHK_BUF();

	*eof = 1;
	//printk("esd_read_value %d\n",esd_has_reset);

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
	//printk("esd_write_value %d\n",val);
	if(val == 1)
		esd_has_reset = 0;
	else{
		phy_reset_esd();
		esd_has_reset = 1;
		//do_reset_test	
	}	

	return count;
}
#if 1//def VPORT
int vport_enable_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;

  	index += sprintf(buf+index, "%d\n", vport_enable);
	CHK_BUF();

	*eof = 1;
	//printk("vport_enable_value %d\n",esd_has_reset);

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0) 
		index = 0;
	if (index>count) 
		index = count;
	return index;
}

int vport_enable_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[4];

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	vport_enable = simple_strtoul(val_string, NULL, 10);	
	//printk("vport_enable_value %d\n", vport_enable);

	return count;
}
#endif
#ifdef TCSUPPORT_CT_PON_C9
/*<< TP-LINK@2014-03-27, add, control RXTX */
unsigned int tp_eth_forward = 0;

int tplink_eth_forward_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	return sprintf(page, "tp_eth_forward: %d\n", tp_eth_forward);
}

int tplink_eth_forward_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32] = {0};
	int value = -1;

	unsigned int port;
	unsigned long reg = 0;
	
	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;
	
	sscanf(val_string, "%d", &value);

#define BIT13	0x2000
#define BIT14	0x4000

	if (value)
	{
		for(port=1; port<3; port++)
		{
			reg = regRead32(GSW_PMCR(port));
			reg |= (BIT13 | BIT14);
			regWrite32(GSW_PMCR(port), reg);
		}
		
		tp_eth_forward = 1;
		printk("Switch Forward On\n");
	}
	else
	{
		for(port=1; port<3; port++)
		{
			reg = regRead32(GSW_PMCR(port));
			reg &= ~(BIT13 | BIT14);
			regWrite32(GSW_PMCR(port), reg);
		}
		
		tp_eth_forward = 0;
		printk("Switch Forward Off\n");
	}
	
#undef BIT13
#undef BIT14

	return count;
}
/*>> endof TP-LINK@2014-03-27, add, control RXTX */
#endif
/*_____________________________________________________________________________
**      function name: updatingUpnpGid
**      descriptions:
**         To fill port map with all e the 239.255.255.250 gid entry 
**            
**      parameters:
**         chanID: 
**		  
**             
**      global:
**             None
**             
**      return:
**             None
**	     
**      call:
**   	     macTC2206STagEnable
**   	     tcMiiStationRead
**   	     tcMiiStationWrite
**      		
**      revision:
**      1. Here 2009/6/3
**____________________________________________________________________________
*/
void 
updatingUpnpGid(void){
	uint32 l_gid=0, h_gid=0;
	uint32 value;

	value=tcMiiStationRead(21, 5);
	if((swicVendor==SWIC_TC2206) && (value & HW_IGMPSNOOP)){
		 local_bh_disable();
		 tcMiiStationWrite(24, 5, 0);
		 l_gid=tcMiiStationRead(24, 7)&0xffff;
		 h_gid=tcMiiStationRead(24, 8)&0x7f;
		 value=tcMiiStationRead(24, 10)&0xffff;
		 /*check the gid is  used for upnp, 239.255.255.250*/
		 if((l_gid==0xfffa) && (h_gid==0x7f)){
			if(value ==0xc000){
				/*Configured by igmpv3*/
				tcMiiStationWrite(24, 6, 0xaaa);
			}
			else{
				/*Learning by igmpv1/v2*/
				tcMiiStationWrite(24, 6, 0x555);
			}
			tcMiiStationWrite(24, 7, (0xeffffffa&0xffff));
			tcMiiStationWrite(24, 8, 0x37f);
			tcMiiStationWrite(24, 10, value);
			tcMiiStationWrite(24, 5, 0x40);
		 }
		 else{
		 	//dbg_pline_1("There is no 239.255.255.250 gid entry\r\n");
			/*PortMap=e0~e4*/
			tcMiiStationWrite(24, 6, 0xaaa);
			/*Set GID for UPNP Multicast packet*/
			tcMiiStationWrite(24, 7, (0xeffffffa&0xffff));
			tcMiiStationWrite(24, 8, 0x37f);
			tcMiiStationWrite(24, 10, 0xc000);
			tcMiiStationWrite(24, 5, 0x40);
		 }
		 local_bh_enable();
	}
}/*end updatingUpnpGid*/
#if 0//#ifdef TCPHY_4PORT
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
