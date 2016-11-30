/*
** $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/private/tc3162l2hp2h/mac_tc3162l2.c#1 $
*/
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
/*
** $Log: mac_tc3162l2.c,v $
** Revision 1.21  2011/07/24 12:00:14  xyzhu_nj
** #11006: Merge China telecom branch to Main trunk.
**
** Revision 1.20  2011/07/14 02:51:59  shnwind
** fix bug : mac polling timer will die when status is RESET or POWERDOWN
**
** Revision 1.19  2011/07/07 07:46:13  shnwind
** RT63260 & RT63260 auto_bench support
**
** Revision 1.18  2011/06/23 02:43:59  shnwind
** fix bug : rt5392 throughput (1 wifi to 1 lan) problem
**
** Revision 1.17  2011/06/03 09:14:45  shnwind
** change netif_receive_skb to replace netif_skb when use NAPI in mac driver
**
** Revision 1.16  2011/05/27 05:52:23  yuantian_sz
** #10397: can not find eth0 port when upgrade single port version.
**
** Revision 1.15  2011/05/13 08:02:03  tzzhang_nj
** [Enhancement]#9542:
**  1. Use special tag to vlan tag under pvc uplink to make sure lan2lan throughput to 80M-120M;
**  2. Use switch vlan under ethernet uplink.
**  3. Enable PVC WAN:
**   ethcmd eth0 vlanpt disable
**   echo 1 > /proc/tc3162/stag_to_vtag
**   ethcmd eth0 vlanpt enable
**     Enable ethernet WAN:
**   ethcmd eth0 vlanpt disable
**   echo 0 > /proc/tc3162/stag_to_vtag
**   ethcmd eth0 vlanpt enable
**
** Revision 1.14  2011/03/31 14:16:04  shnwind
** solve tc3182 throughput issue
**
** Revision 1.13  2011/03/23 12:04:23  shnwind
** solve a lot of out-of-order packet
**
** Revision 1.12  2011/03/17 11:44:01  shnwind
** Add a switch to vlanpt disable and NAPI moce
**
** Revision 1.11  2011/01/26 11:43:48  shnwind
** merge #8655 #8656
**
** Revision 1.10  2011/01/25 13:33:08  shnwind
** merge #8655
**
** Revision 1.9  2011/01/18 11:42:19  xyzhu_nj
** 8119
**
** Revision 1.8  2011/01/04 08:25:25  yuantian_sz
** merge #2018 TCSUPPORT_DYNAMIC_VLAN to maintrunk
**
** Revision 1.7  2010/12/24 12:42:23  xyzhu_nj
** #8119:Add support port reverse function.
**
** Revision 1.6  2010/12/22 01:55:08  xyzhu_nj
** #8119:Add support port reverse function.
**
** Revision 1.5  2010/12/02 06:24:34  shnwind
** fix some warning
**
** Revision 1.4  2010/11/09 11:31:32  shnwind
** fix bug : vlan packet send error when bridge mode
**
** Revision 1.3  2010/10/21 09:51:58  shnwind
** Use special tag to replace vlan function in switch
**
** Revision 1.2  2010/10/20 09:27:56  xmdai_nj
** #7191:crash happened when wan2lan&IGMP are enabled
**
** Revision 1.1.1.1  2010/09/30 21:14:54  josephxu
** modules/public, private
**
** Revision 1.10  2010/09/25 15:04:33  here
** [Ehancement]TCConsole to support capture the console log message and set command into cpe.
**
** Revision 1.9  2010/09/03 16:39:12  here
** [Ehance] TC3182 GMAC Driver is support TC-Console & WAN2LAN function.
**
** Revision 1.8  2010/09/03 13:47:23  xyyou
** add new version control
**
** Revision 1.7  2010/09/02 07:26:00  here
** [Ehance] Support TC3162U/TC3182 Auto-Bench
**
** Revision 1.6  2010/08/25 16:02:56  here
** [Bug fix] The tc-console can't be work and disabled TC2206 CPU PORT turbo mii mode with L2P2/L3P3/L4P4 CPU.
**
** Revision 1.5  2010/07/22 05:19:10  jlliu_nj
** Bug#6345:Support Version control mechanism in mldsnooping module,mac driver,sar driver:add line break in version
**
** Revision 1.4  2010/07/22 02:48:55  jlliu_nj
** Bug#6345:Support Version control mechanism in mldsnooping module,mac driver,sar driver
**
** Revision 1.3  2010/06/21 12:40:26  jlliu_nj
** update TC2205F/6F patch version to F8.0
**
** Revision 1.2  2010/05/20 07:32:26  here
** [Ehancemenet] Support the TC2206 swithc ic with TC3262 platform and sprate the switch ic into tcswitch.c files
**
** Revision 1.1.1.1  2010/04/09 09:35:28  feiyan
** New TC Linux Make Flow Trunk
**
** Revision 1.12  2010/03/29 15:20:18  ian
** Dynamic IMEM Utilization
**
** Revision 1.11  2010/03/29 12:37:06  yzwang_nj
** Fix compile error
** (Some statements about wan2lan were not limited by "ifndef TC2031_SUPPORT")
**
** Revision 1.10  2010/03/26 05:14:34  here
** [Bug fix] TCCONSOLE can't be work when user actived the virtual port function(Fix one port compiler error problem).
**
** Revision 1.9  2010/03/25 17:00:31  here
** [Bug fix] TCCONSOLE can't be work when user actived the virtual port function.
**
** Revision 1.8  2010/03/23 05:40:38  here
** [Bug fix] TC3162U with TC2206 Switch IC ESD Software Patch.
**
** Revision 1.7  2010/02/03 02:42:40  here
** [Ehancement]Remove the check mechanism for wifi driver that is released by trendchip crop.
**
** Revision 1.6  2010/02/02 12:13:13  here
** [Ehancement]1.RT3092 Chip is support polling mode when the adsl at init state.
** 2. Support the check mechanism for wifi driver that is released by trendchip crop.
**
** Revision 1.5  2010/01/28 13:21:49  here
** [Ehance]The mac driver is supported TC2206F switch ic.
**
** Revision 1.4  2010/01/15 17:26:22  pork
** wan2lan porting
**
** Revision 1.3  2010/01/10 15:26:32  here
** [Ehancement]TC3162U MAC EEE is operated at 100M-FD, SAR interface is accroding the SAR_CLK to calculate atm rate.
**
** Revision 1.2  2009/12/30 07:05:58  jlliu_nj
** Bug#3779:IPv6 ready logo Phase-2
**
** Revision 1.1.1.1  2009/12/17 01:48:05  josephxu
** 20091217, from Hinchu ,with VoIP
**
** Revision 1.2  2006/07/06 06:05:59  lino
** update to version 1.2
**
** Revision 1.1  2006/07/06 05:04:47  lino
** add kernel module support
**
** Revision 1.6  2006/02/27 05:12:31  lino
** do lan reset first then search phy addr
**
** Revision 1.5  2006/02/17 04:59:27  lino
** when disable adsl, let compilation correct
**
** Revision 1.4  2005/12/12 07:50:58  lino
** merge macTxSend and tc3162_tx_send functions
**
** Revision 1.3  2005/11/16 08:00:19  lino
** fix "ifconfig eth0 down" then "ifconfig eth0 up" will cause do_ade error
** fix "ifconfig eth0 hw ether" cannot change ethernet mac address
**
** Revision 1.2  2005/11/02 08:27:26  lino
** remove ahb_err handler
**
** Revision 1.1.1.1  2005/11/02 05:45:19  lino
** no message
**
*/
#define TC3162_MAC_NAPI

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
#include <asm/io.h>
#include <asm/tc3162/tc3162.h>
#include <asm/tc3162/ledcetrl.h>
#include <asm/tc3162/TCIfSetQuery_os.h>
#include "../../../version/tcversion.h"
#ifdef CONFIG_TC3162_ADSL
#include <linux/if_vlan.h>
#include <linux/skbuff.h>
#endif
#ifdef TCSUPPORT_AUTOBENCH
#include "../auto_bench/autobench.h"
#endif

#include "../tcphy/tcconsole.h"

#ifdef MII_INTERFACE
#include <linux/mii.h>
#endif
#include "tc3162l2mac.h"


#ifdef TCPHY_SUPPORT
#include <asm/tc3162/cmdparse.h>
#include "../tcphy/tcetherphy.h"
#include "../tcphy/tcswitch.h"

#ifdef TCPHY_DEBUG
#include "tcethercmd.h"
#endif
#endif 
#ifdef EEE_SUPPORT
#include "psm_verify.h"
#endif

#define RX_BUF_LEN 	(2048 - 16 - 64 - (sizeof(struct skb_shared_info)))		
#define RX_MAX_PKT_LEN 		1536
//#define RX_BUF_LEN 		1776


/************************************************************************
*                          C O N S T A N T S
*************************************************************************
*/
#define DYNAMIC_ALLOC_DMA_BUF	1

#define LAN_ST_100MB        0x01
#define LAN_ST_FULL_DUPLEX	0x02
#define LAN_ST_LINK_UP      0x04


/* ADMTEK6996M register */
#define ADM_PORT0_BASIC		0x01
#define ADM_PORT1_BASIC		0x03
#define ADM_PORT2_BASIC		0x05
#define ADM_PORT3_BASIC		0x07
#define ADM_PORT4_BASIC		0x08
#define ADM_PORT5_BASIC		0x09

#define ADM_CHIP_ID0		0xa0
#define ADM_CHIP_ID1		0xa1



/************************************************************************
*                         D A T A   T Y P E S
*************************************************************************
*/

#if defined(TCSUPPORT_ETHER_ALL_LED)
extern uint8 ether_lan_speed;
int ether_speed[4] = {200,150,100,50};
#endif
/************************************************************************
*              F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
PRIVATE void tcAdmMiiStationWrite(uint32 admReg,	uint32 miiData);
PRIVATE uint32 tcAdmMiiStationRead(uint32 admReg);
#ifdef MII_INTERFACE
static int mdio_read(struct net_device *dev, int phy_id, int reg_num);
static void mdio_write(struct net_device *dev, int phy_id, int reg_num, int val);
#endif 
#ifdef TCPHY_SUPPORT
extern int cmd_unregister(char *name);
#define LAN_ST_CONNECTED		0x40	/* if lan cable is connected */
extern uint8 tcPhyExtResetNeeded(void);
extern uint8 getTcPhyFlag(void);
extern int tcPhyInit(macAdapter_t * mac_p);
extern uint8 phyMbpChk(struct sk_buff* oldMbp);
extern uint8 phy_recv_err_check(struct sk_buff** oldMbp, volatile macRxDescr_t *rxDescrp, uint32 *frameSize);
extern int tcPhyVerLookUp(macAdapter_t *mac_p);
extern uint8 getTcPhyForceLinkFlag(void);
extern uint32 getTcPhyStatusReg(macAdapter_t * mac_p);
extern uint8 getTcPhyLookbackFlag(void);
extern void tcephydbgcmd(void);
#endif 
static irqreturn_t tc3162_mac_isr(int irq, void *dev_id);
int tc3162_mac_tx(struct sk_buff *skb, struct net_device *dev);
#ifdef SWITCH_3052
int macRt3052STagEnable(ChanID);
#endif

#if !defined(TCSUPPORT_CT)
extern int tcPhySwPatch(void);
#ifdef TC_CONSOLE_ENABLE
void delete_tcconsole_proc(void);
#endif
#endif
#ifdef TCPHY_4PORT
extern char esd_has_reset;
extern void phy_reset_esd(void);
extern int eth_esd_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data);

extern int eth_esd_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data);
#if !defined(TCSUPPORT_CT)
#if 1//def VPORT
extern int vport_enable_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data);

extern int vport_enable_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data);

#endif
#endif
#ifdef TCPHY_SUPPORT
extern int eth_port_stat_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data);
#endif
#endif
#if defined(TCSUPPORT_ETHER_ALL_LED)
int ether_states_handler(void); //ethernet led support
#endif
/************************************************************************
*                        P U B L I C   D A T A
*************************************************************************
*/

int DbgSettings = DBG_TIME     |
		          //DBG_HW     |
                  //DBG_DBG      |
                  //DBG_STAT     |
                  //DBG_REG      |
                  //DBG_FUNCTION |
                  //DBG_BUF	|
                  //DBG_DES	|
                  //DBG_MSG	|
                  //DBG_ERROR    |
                  //DBG_WARN |
                  //DBG_BUFS |
                  //DBG_OUT |
                  0;


/************************************************************************
*                      E X T E R N A L   D A T A
*************************************************************************
*/

#ifdef EEE_SUPPORT
extern void chkLinkStatus(void);
#ifdef EEE_DEBUG
extern uint16 macLoopback;
extern uint32 macRxLoopback;
extern uint8 dbgLevel;
#endif
#endif
#ifdef WAN2LAN
extern int masko;
#endif
//#ifdef VPORT
//int vportMasko=0;
//#endif

/************************************************************************
*                       P R I V A T E   D A T A
*************************************************************************
*/


/* Device data */
struct net_device *tc3162_mac_dev;
/* Timer */
static struct timer_list eth_timer;
static struct timer_list eth_poll_timer;

macAdapter_t *mac_p = NULL;
static macPhyLinkProfile_t enetPhyLinkProfile;
#ifdef DYNAMIC_ALLOC_DMA_BUF
static macMemPool_t *enetMemPool;
#else
static macMemPool_t enetMemPool __attribute__ ((__aligned__(16)));
#endif

static phyDeviceList_t phyDeviceList[] = {
	{ 0x0013, "LXT972" },
	{ 0x0022, "AC101L" },
	{ 0x0243, "IP101" },
	{ 0x8201, "RTL8201" },
	{ 0x03a2, "TC2031" }
};

static uint8 macInitialized = 0;	/* wdl */

uint8 def_mac_addr[] = {0x00, 0x00, 0xaa, 0xbb, 0xcc, 0xff};

uint8 swicVendor = 0;
#if defined(WAN2LAN) || defined(CONFIG_TC3162_ADSL)
#ifndef TC2031_SUPPORT
/* ------ xyzhu_091105:special tag relation data start ---------- */
extern uint8 macVlanFlag;
extern uint8 macRxPortFlag;
#if defined(TCSUPPORT_CT)
extern uint8 macSTagFlag;
#else
//extern uint8 macSTagFlag;
#endif
/* ------ xyzhu_091105:special tag relation data end ----------- */
#endif
#endif
#if !defined(TCSUPPORT_CT)
#if 1//defined(WAN2LAN) || defined(VPORT) 
#ifndef TC2031_SUPPORT
extern uint8 macSTagFlag;
#endif
#endif
//#if 1//def TCSUPPORT_PORT_REVERSE
//int g_port_reverse = 0;
//extern int g_port_reverse;
//#endif
#endif
#ifndef TC2031_SUPPORT
/* for port reverse */
extern int port_reverse_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data);
extern int port_reverse_write_proc(struct file *file, const char *buffer, unsigned long count, void *data);
#if !defined(TCSUPPORT_CT)
/* for special tag to vtag */
extern int stag_to_vtag_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data);
extern int stag_to_vtag_write_proc(struct file *file, const char *buffer, unsigned long count, void *data);
#endif
#endif
/************************************************************************
*        F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
#define read_reg_word(reg) 			VPint(reg)
#define write_reg_word(reg, wdata) 	VPint(reg)=wdata
#define pause(x) mdelay(x)


#ifdef EEE_DEBUG
static void dump_skb(struct sk_buff *skb) 
{
	char tmp[80];
	char *p = skb->data;
	char *t = tmp;
	int i, n = 0;

	printk("ERR skb=%08lx data=%08lx len=%d\n", (uint32) skb, (uint32) skb->data, skb->len);
	for (i = 0; i < skb->len + 4; i++) {
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
PRIVATE uint32
align16Byte
(
	uint32 addr
)
{
	return (addr+15)&0xFFFFFFF0;
}

PUBLIC void
miiStationWrite(
	macAdapter_t *mac_p,
	uint32 phyReg,
	uint32 miiData
)
{		
	uint32 cnt=1000;
	uint32 reg=0;
	macPhyWDataReg_t     macPHYWData;

  	DEBUGMSG(DBG_FUNCTION,("\r\n---->miiStationWrite"));
	// check argument
	if(mac_p==NULL){
		DEBUGMSG(DBG_ERROR,("\r\nERROR:miiStationWrite() mac_p is NULL pointer"));
	}
	if(phyReg>24){
		DEBUGMSG(DBG_ERROR,("\r\nERROR:miiStationWrite() phyReg is out of range(16)"));
	}
	macPHYWData.bits.miiwdata = miiData & 0x0000ffff;
	write_reg_word (CR_MAC_PHYWDATA, macPHYWData.value);

	if(isTC3162U || isRT63260){
		/*New format to access mii register*/	
		reg=((phyReg & 0x1f)<<NEW_PHYOR_REG_SHIFT) |((mac_p->enetPhyAddr & 0x1f)<<NEW_PHYOR_ADDR_SHIFT)\
		|(NEW_PHYOR_OP_WR<<NEW_PHYOR_OP_SHIFT) |(NEW_PHYOR_ST_IEEE_OLD <<NEW_PHYOR_ST_SHIFT);
		write_reg_word (CR_MAC_PHYCR, reg);
		do{
	 	   reg=read_reg_word (CR_MAC_PHYCR);
		   cnt--;
		}while ((reg&NEW_PHYOR_BUSY)&&cnt>0);
	}
	else{
		/*Old format to accress mii register*/
		reg=((phyReg & 0x1f)<<PHYOR_REG_SHIFT)|((mac_p->enetPhyAddr & 0x1f)<<PHYOR_ADDR_SHIFT) \
		|PHYOR_WRITE;
		write_reg_word (CR_MAC_PHYCR, reg);
		do{
	 	   reg=read_reg_word (CR_MAC_PHYCR);
		   cnt--;
		}while ((reg & PHYOR_WRITE)&&cnt>0);
	}
	
	if(cnt==0){
		DEBUGMSG(DBG_MSG,("\r\nError:miiStationWrite Timeout!"));
	}
	DEBUGMSG(DBG_FUNCTION,("\r\n<----miiStationWrite"));
}

PUBLIC uint32
miiStationRead(
	macAdapter_t *mac_p,
	uint32 phyReg
)
{
	uint32 reg=0;
	uint32 cnt=1000;
	
  	DEBUGMSG(DBG_FUNCTION,("\r\n---->tcMiiStationRead"));
	
	if(mac_p==NULL){
		DEBUGMSG(DBG_ERROR,("\r\nERROR:miiStationWrite() mac_p is NULL pointer"));
	}
	
	if(phyReg>24){
		DEBUGMSG(DBG_ERROR,("\r\nERROR:miiStationWrite() phyReg is out of range(16)"));
	}

	if(isTC3162U || isRT63260){
		/*New format to access mii register*/	
		reg=((phyReg & 0x1f)<<NEW_PHYOR_REG_SHIFT) |((mac_p->enetPhyAddr & 0x1f) <<NEW_PHYOR_ADDR_SHIFT)\
		|(NEW_PHYOR_OP_RD<<NEW_PHYOR_OP_SHIFT) |(NEW_PHYOR_ST_IEEE_OLD <<NEW_PHYOR_ST_SHIFT);
		write_reg_word (CR_MAC_PHYCR, reg);
		do{
	 	   reg=read_reg_word (CR_MAC_PHYCR);
		   cnt--;
		}while ((reg&NEW_PHYOR_BUSY)&&cnt>0);
	}
	else{
		/*Old format to accress mii register*/
		reg=((phyReg & 0x1f)<<PHYOR_REG_SHIFT) |((mac_p->enetPhyAddr & 0x1f) <<PHYOR_ADDR_SHIFT) \
		|PHYOR_READ;
		write_reg_word (CR_MAC_PHYCR, reg);
		do{
		    reg=read_reg_word (CR_MAC_PHYCR);
		    cnt--;
		}while ((reg&PHYOR_READ)&&cnt>0);
	}
	
	if(cnt==0){
		DEBUGMSG(DBG_ERROR,("\r\nError:miiStationRead Timeout!"));
	}

  	DEBUGMSG(DBG_FUNCTION,("\r\n<----miiStationRead"));
	return (reg & 0xffff);
}

void
tcMiiStationWrite
(
	uint32 enetPhyAddr,
	uint32 phyReg,
	uint32 miiData
)
{
	uint32 cnt=1000;
	uint32 reg=0;
	macPhyWDataReg_t     macPHYWData;

  	DEBUGMSG(DBG_FUNCTION,("\r\n---->miiStationWrite"));
	if(phyReg>24){
		DEBUGMSG(DBG_ERROR,("\r\nERROR:miiStationWrite() phyReg is out of range(16)"));
	}

	macPHYWData.bits.miiwdata = miiData & 0x0000ffff;
	write_reg_word (CR_MAC_PHYWDATA, macPHYWData.value);

	if(isTC3162U || isRT63260){
		/*New format to access mii register*/	
		reg=((phyReg & 0x1f)<<NEW_PHYOR_REG_SHIFT) |((enetPhyAddr & 0x1f) <<NEW_PHYOR_ADDR_SHIFT)\
		|(NEW_PHYOR_OP_WR<<NEW_PHYOR_OP_SHIFT) |(NEW_PHYOR_ST_IEEE_OLD <<NEW_PHYOR_ST_SHIFT);
		write_reg_word (CR_MAC_PHYCR, reg);
		do{
	 	   reg=read_reg_word (CR_MAC_PHYCR);
		   cnt--;
		}while ((reg&NEW_PHYOR_BUSY)&&cnt>0);
	}
	else{
		/*Old format to accress mii register*/
		reg=((phyReg & 0x1f)<<PHYOR_REG_SHIFT) |((enetPhyAddr & 0x1f) <<PHYOR_ADDR_SHIFT) \
		|PHYOR_WRITE;
		write_reg_word (CR_MAC_PHYCR, reg);
		do{
	 	   reg=read_reg_word (CR_MAC_PHYCR);
		   cnt--;
		}while ((reg & PHYOR_WRITE)&&cnt>0);
	}
	
	if(cnt==0){
		DEBUGMSG(DBG_MSG,("\r\nError:miiStationWrite Timeout!"));
	}
	DEBUGMSG(DBG_FUNCTION,("\r\n<----miiStationWrite"));
	#ifdef TCPHY_4PORT
	/*TC2206 switch IC can't be direct to do PHY reset, we must 
	 * avoid ESD software patch be trigger.
	 */
	refillPhyDefVal(enetPhyAddr, phyReg, miiData);	
	#endif
}

uint32
tcMiiStationRead(
	uint32 enetPhyAddr,
	uint32 phyReg
)
{
	uint32 reg=0;
	uint32 cnt=1000;
	
	DEBUGMSG(DBG_FUNCTION,("\r\n---->tcMiiStationRead"));
	
	if(phyReg>24){
		DEBUGMSG(DBG_ERROR,("\r\nERROR:miiStationWrite() phyReg is out of range(16)"));
	}

	if(isTC3162U || isRT63260){
		/*New format to access mii register*/	
		reg=((phyReg & 0x1f)<<NEW_PHYOR_REG_SHIFT) |((enetPhyAddr & 0x1f) <<NEW_PHYOR_ADDR_SHIFT)\
		|(NEW_PHYOR_OP_RD<<NEW_PHYOR_OP_SHIFT) |(NEW_PHYOR_ST_IEEE_OLD <<NEW_PHYOR_ST_SHIFT);
		write_reg_word (CR_MAC_PHYCR, reg);
		do{
	 	   reg=read_reg_word (CR_MAC_PHYCR);
		   cnt--;
		}while ((reg&NEW_PHYOR_BUSY)&&cnt>0);
	}
	else{
		/*Old format to accress mii register*/
		reg=((phyReg & 0x1f)<<PHYOR_REG_SHIFT) |((enetPhyAddr & 0x1f) <<PHYOR_ADDR_SHIFT) \
		|PHYOR_READ;
		write_reg_word (CR_MAC_PHYCR, reg);
		do{
		    reg=read_reg_word (CR_MAC_PHYCR);
		    cnt--;
		}while ((reg&PHYOR_READ)&&cnt>0);
	}
	
	if(cnt==0){
		DEBUGMSG(DBG_ERROR,("\r\nError:miiStationRead Timeout!"));
	}

  	DEBUGMSG(DBG_FUNCTION,("\r\n<----miiStationRead"));
	return (reg & 0xffff);
}

PRIVATE void 
tcAdmMiiStationWrite(
	uint32 admReg,
	uint32 miiData
)
{
	uint32 phyaddr;
	uint32 reg;

	phyaddr = admReg >> 5;
	reg = admReg & 0x1f;

	tcMiiStationWrite(phyaddr, reg, miiData);
}

PRIVATE uint32 
tcAdmMiiStationRead(
	uint32 admReg
)
{
	uint32 phyaddr;
	uint32 reg;
	
	phyaddr = admReg >> 5;
	reg = admReg & 0x1f;

	return tcMiiStationRead(phyaddr, reg);
}
#ifdef MII_INTERFACE
static int mdio_read(struct net_device *dev, int phy_id, int reg_num)
{
	return tcMiiStationRead(phy_id, reg_num);
}

static void mdio_write(struct net_device *dev, int phy_id, int reg_num, int val)
{
	tcMiiStationWrite(phy_id, reg_num, val);
}
#endif

PRIVATE void macIMRMask(void)
{
	macIntrMaskReg_t macIntrMask;
	macIntrMask.value = 0;
	macIntrMask.bits.rpkt_finish = 1;
#ifdef TC3162_MAC_NAPI
	macIntrMask.bits.norxbuf     = 1;
	macIntrMask.bits.rpkt_lost   = 1;
#endif	
#if 0	
    macIntrMask.bits.rpkt_ok_by_thrhold = 0;
    macIntrMask.bits.rpkt_ok_by_timeout = 0;
    macIntrMask.bits.xpkt_ok_by_thrhold = 0;
    macIntrMask.bits.xpkt_ok_by_timeout = 0;
  	macIntrMask.bits.norxbuf	 = 1;
  	macIntrMask.bits.notxbuf	 = 1;
  	macIntrMask.bits.xpkt_finish = 1;
  	macIntrMask.bits.xpkt_lost	 = 1;
  	macIntrMask.bits.rpkt_lost	 = 1;
  	macIntrMask.bits.xpkt_save   = 1;
  	macIntrMask.bits.rpkt_save   = 1;
  	macIntrMask.bits.ahb_err	 = 1;
  	macIntrMask.bits.physts_chg  = 1;
#endif	
	write_reg_word(CR_MAC_IMR, macIntrMask.value);
}

PRIVATE void macResetSwMAC(void)
{
	macCtrlReg_t	macCtrl;

	DEBUGMSG(DBG_FUNCTION,("\r\n---->macResetSwMAC"));
	// Disable DMA Enable
	macCtrl.value = 0;
	write_reg_word (CR_MAC_MACCR, macCtrl.value);
	pause(2);

	// Reset Mac
//	macCtrl.value = 0;
	macCtrl.bits.sw_rst = 1;
	write_reg_word (CR_MAC_MACCR, macCtrl.value);
	pause(2);
	DEBUGMSG(DBG_FUNCTION,("\r\n<----macResetSwMAC"));
}

PRIVATE void macSetIntTimer(void)
{
	mac_intr_timer_ctrl_reg_t macIntrTimerCtrl;

	DEBUGMSG(DBG_FUNCTION,("\r\n---->macSetIntTimer"));
    macIntrTimerCtrl.value = 0;
	write_reg_word (CR_MAC_ITC, macIntrTimerCtrl.value);
  DEBUGMSG(DBG_FUNCTION,("\r\n<----macSetIntTimer"));
}

// Assign Tx Rx Descriptor Control Registers
PRIVATE void macSetDMADescrCtrlReg(macAdapter_t *mac_p)
{
	DEBUGMSG(DBG_FUNCTION,("\r\n---->macSetDMADescrCtrlReg"));
  	write_reg_word(CR_MAC_TXR_BADR, K1_TO_PHY( mac_p->txDescrRingBaseVAddr));
  	write_reg_word(CR_MAC_RXR_BADR, K1_TO_PHY( mac_p->rxDescrRingBaseVAddr));
	
  	VPint(CR_MAC_TXR_SIZE) = mac_p->txRingSize;
  	VPint(CR_MAC_RXR_SIZE) = mac_p->rxRingSize;
  	VPint(CR_MAC_TXDESP_SIZE) = mac_p->txDescrSize;
  	VPint(CR_MAC_RXDESP_SIZE) = mac_p->rxDescrSize;
	DEBUGMSG(DBG_FUNCTION,("\r\n<----macSetDMADescrCtrlReg"));
}

PRIVATE void macSetMACCR
(
  macAdapter_t *map_p
)
{
  macCtrlReg_t macCtrl;

  DEBUGMSG(DBG_FUNCTION, ("\r\n---->macSetMACCR"));
  // clear reg
//macCtrl.value = 0x3df23;
  macCtrl.value = 0x0;
  //Enable RX flow control. shnwind 20110622
  macCtrl.bits.rx_flow_ctrl = 1;
  
  // set flag
  macCtrl.bits.rx_broadpkt = 1;
  macCtrl.bits.rx_multipkt = 1;

  macCtrl.bits.fulltx  = 1;
  macCtrl.bits.crc_apd = 1;
  macCtrl.bits.mdc_sel = 0;
  macCtrl.bits.rcv_all = 1;

  macCtrl.bits.rx_ftl      = 1;
  macCtrl.bits.rx_runt     = 1;
  macCtrl.bits.ht_multi_en = 1;
  macCtrl.bits.rcv_en      = 0;

  macCtrl.bits.l2_mode        = 1;
  macCtrl.bits.enrx_in_halftx = 0;
  macCtrl.bits.xmt_en       	= 0;
  macCtrl.bits.crc_dis       	= 0;

  macCtrl.bits.loop_en = 0;
  macCtrl.bits.sw_rst  = 0;
  macCtrl.bits.rdma_en = 0;
  macCtrl.bits.xdma_en = 0;

  macCtrl.bits.txfifo_thr = 0x0e;
  macCtrl.bits.rx_untag   = 0;
  macCtrl.bits.drop_local = 0;

  write_reg_word (CR_MAC_MACCR, macCtrl.value);

  DEBUGMSG(DBG_FUNCTION,("\r\n<----macSetMACCR"));
}

PRIVATE void macSetMacReg
(
  macAdapter_t *mac_p
)
{
	write_reg_word(CR_MAC_MADR,mac_p->macAddr[0]<<8  | mac_p->macAddr[1]<<0);
	write_reg_word(CR_MAC_LADR,mac_p->macAddr[2]<<24 | mac_p->macAddr[3]<<16 | \
                               mac_p->macAddr[4]<<8  | mac_p->macAddr[5]<<0);
}

PRIVATE macTxDescr_t*
macTxRingProc
(
	macAdapter_t *mac_p
)
{
	volatile macTxDescr_t *pTxDescp;
	unsigned long flags;

	spin_lock_irqsave(&mac_p->lock, flags);
	pTxDescp = ((macTxDescr_t*)mac_p->txDescrRingBaseVAddr) + mac_p->txUnReleasedDescp;

  	while ( mac_p->txUnReleasedBufCnt != 0 )
	{
	  	if ( (pTxDescp->tdes0.word)&(1<<31) )
		{
			spin_unlock_irqrestore(&mac_p->lock, flags);
			return 0; // owned by MAC engine, something wrong here!
		}
		if ( mac_p->statisticOn )
		{
			if ( (pTxDescp->tdes0.word)&(1<<1) )
		  	{
		    	mac_p->macStat.inSilicon.txExCollisionCnt++;
		    	mac_p->macStat.inSilicon.txCollisionCnt++;
		    	mac_p->macStat.MIB_II.outErrors++;
		  	}

			if ( (pTxDescp->tdes0.word)&(1<<3|1<<2) )
			{
				mac_p->macStat.inSilicon.txUnderRunCnt++;
			}

	    	if ( (pTxDescp->tdes0.word)&(1<<0) )
		  	{
		     	mac_p->macStat.inSilicon.txLateCollisionCnt++;
		     	mac_p->macStat.inSilicon.txCollisionCnt++;
		     	mac_p->macStat.MIB_II.outErrors++;
		  	}
		} // mac_p->statisticOn

		if (pTxDescp->tdes2.buf_p == 0x0)
			printk("buf_p is null\n");
		dev_kfree_skb_any(pTxDescp->skb);

		pTxDescp->tdes2.buf_p = 0;
		pTxDescp->skb = NULL;

		if ( mac_p->txUnReleasedDescp == ( mac_p->txRingSize - 1) )
		{
			mac_p->txUnReleasedDescp = 0;
		}
		else
		{
			mac_p->txUnReleasedDescp++;
		}
		mac_p->txUnReleasedBufCnt--;

		pTxDescp = ((macTxDescr_t*)mac_p->txDescrRingBaseVAddr) + mac_p->txUnReleasedDescp;

    	mac_p->macStat.inSilicon.txDeQueueNum++;

	} // while
	spin_unlock_irqrestore(&mac_p->lock, flags);

	return (macTxDescr_t*)pTxDescp;
}

__IMEM int tc3162_mac_tx(struct sk_buff *skb, struct net_device *dev)
{
	volatile macTxDescr_t *currDescrp = NULL;
	uint32 length;
	uint8 *bufAddrp = skb->data;
	unsigned long flags;
#if !defined(TCSUPPORT_CT)
	uint8 vlanid;

#endif
	length = ((ETH_ZLEN < skb->len) ? skb->len : ETH_ZLEN);

	
	#ifdef EEE_DEBUG
	if(dbgLevel & DUMP_TX_PKT){
		dump_skb(skb);
	}
	#endif
#ifdef CONFIG_TC3162_ADSL
	#ifndef TC2031_SUPPORT
	if(isTCConsolePkt(skb)){
		if(swicVendor == SWIC_TC2206){
			macTC2206STagInsert(skb);
		}
		else if(swicVendor ==SWIC_IP175C){
			macIpSTagInsert(skb);
		}
		length = ((ETH_ZLEN < skb->len) ? skb->len : ETH_ZLEN);
	}
	#endif
#endif 
#if !defined(TCSUPPORT_CT)
#if 1//def VPORT
#ifndef TC2031_SUPPORT
	else if(!(skb->mark & SKBUF_COPYTOLAN) && macSTagFlag){
		//prevent modify clone skbuff data. 
		skb = skb_unshare(skb ,GFP_ATOMIC);
		if (skb == NULL){
			mac_p->macStat.MIB_II.outDiscards++;
			return 0;
		}
		/*Normal packet is inclued the vlan header, and also the
		 wan2lan packet is not inclued the vlan header.*/
	#if 0
		if (g_port_reverse) {
		#ifdef TCSUPPORT_CT_2PWIFI
			vlanid = 2 - ((uint8)skb->data[15]);
		#else
			vlanid = 4 - ((uint8)skb->data[15]);
		#endif
			vportMasko = (1<<vlanid);
		}
		else {
			vportMasko = (1<<(uint8)skb->data[15]-BASE_VID_IDX);
		}
	#endif

		/*Remove the vlan tag*/
		//memmove(skb->data+VLAN_HLEN, skb->data, 12);
		//skb_pull(skb, VLAN_HLEN);
		skb->mark|=SKBUF_VLAN;
	 }
#endif
#endif
#endif

#if defined(TCSUPPORT_CT)
#ifdef WAN2LAN
	#ifndef TC2031_SUPPORT
	if ((swicVendor == SWIC_IP175C) && macSTagFlag && !(skb->mark & SKBUF_TCCONSOLE))
	{
			macIpSTagInsert(skb);
			length = ((ETH_ZLEN < skb->len) ? skb->len : ETH_ZLEN);
	}

	//#ifdef TC2206_SUPPORT
	if((swicVendor==SWIC_TC2206)&&macSTagFlag && !(skb->mark & SKBUF_TCCONSOLE)){
		macTC2206STagInsert(skb);
		length = ((ETH_ZLEN < skb->len) ? skb->len : ETH_ZLEN);
	}
	//#endif /*TC2206_SUPPORT*/	
	#endif /*TC2031_SUPPORT*/
#endif
#else
#if 1//defined(WAN2LAN) || defined(VPORT)
	#ifndef TC2031_SUPPORT
	if ((swicVendor == SWIC_IP175C) && macSTagFlag && !(skb->mark & SKBUF_TCCONSOLE))
	{
			macIpSTagInsert(skb);
			length = ((ETH_ZLEN < skb->len) ? skb->len : ETH_ZLEN);
	}

	//#ifdef TC2206_SUPPORT
	if((swicVendor==SWIC_TC2206)&&macSTagFlag && !(skb->mark & SKBUF_TCCONSOLE)){
		macTC2206STagInsert(skb);
		length = ((ETH_ZLEN < skb->len) ? skb->len : ETH_ZLEN);
	}
	//#endif /*TC2206_SUPPORT*/	
	#endif /*TC2031_SUPPORT*/
#endif
#endif
	if (mac_p->txUnReleasedBufCnt >= TX_BUF_RELEASE_THRESHOLD)
  	{
    	macTxRingProc(mac_p);
  	}

	if ( mac_p->txUnReleasedBufCnt == mac_p->txRingSize )
	{
		mac_p->macStat.MIB_II.outDiscards++;
		dev_kfree_skb_any(skb);
		
		 /*Must return 0 here. If the return value is not equal zero, kernel will put the skb to link list.
		               This skb has been free here so it cause kernel double free this skb.  shnwind 20090821.*/

		return 0;
	}

	
	spin_lock_irqsave(&mac_p->lock, flags);
	/* ----- Get the transmit descriptor ----- */
	currDescrp = ((macTxDescr_t*)mac_p->txDescrRingBaseVAddr) + mac_p->txCurrentDescp;

	if( (currDescrp->tdes0.word)&(1<<31) )
	{
		mac_p->macStat.MIB_II.outDiscards++;
		dev_kfree_skb_any(skb);
#if !defined(TCSUPPORT_CT)
		spin_unlock_irqrestore(&mac_p->lock, flags);
#endif
		/*Must return 0 here. If the return value is not equal zero, kernel will put the skb to link list.
			This skb has been free here so it cause kernel double free this skb.  shnwind 20090821.*/

		return 0;
	}

	/* ----- Count the MIB-II ----- */
	mac_p->macStat.MIB_II.outOctets += length;

	if ( *bufAddrp & 0x01 )
	{
		mac_p->macStat.MIB_II.outMulticastPkts++;
	}
	else
	{
		mac_p->macStat.MIB_II.outUnicastPkts++;
	}
  	#if !defined(TCSUPPORT_ETHER_ALL_LED)
	ledTurnOn(LED_ETHER_ACT_STATUS);
	if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_100MB)
		ledTurnOn(LED_ETHER_100M_ACT_STATUS);
	else
		ledTurnOn(LED_ETHER_10M_ACT_STATUS);
  	#endif
	//spin_lock_irqsave(&mac_p->lock, flags);
	dma_cache_wback_inv((unsigned long)(skb->data), skb->len);
	// tx buffer size
	currDescrp->tdes0.word = 0;
  	/* clear all except edotr, fts, lts */
  	currDescrp->tdes1.word &= (1<<31|1<<28|1<<27);
  	currDescrp->tdes1.word |= (length&0x000007ff);

  	currDescrp->tdes2.buf_p = K1_TO_PHY(skb->data);
	currDescrp->skb = skb;

	currDescrp->tdes0.word |= (1<<31);

	if( mac_p->txCurrentDescp == ( mac_p->txRingSize - 1 ))
	{
		mac_p->txCurrentDescp = 0;
	}
	else
	{
		mac_p->txCurrentDescp ++;
	}

	mac_p->txUnReleasedBufCnt++;

  	mac_p->macStat.inSilicon.txEnQueueNum++;
	spin_unlock_irqrestore(&mac_p->lock, flags);

  	MAC_TX_POLL_DEMAND_(1);
	return 0;
}

PUBLIC void
macDefaultParaSet
(
	macAdapter_t *mac_p
)
{
	mac_p->rxDescrSize = MAC_RXDESCP_SIZE;
	mac_p->txDescrSize = MAC_TXDESCP_SIZE;
	mac_p->rxRingSize  = MAC_RXDESCP_NO;
	mac_p->txRingSize  = MAC_TXDESCP_NO;

  	mac_p->macStat.inSilicon.rxEnQueueNum = 0;
	mac_p->macStat.inSilicon.rxDeQueueNum = 0;
	mac_p->macStat.inSilicon.txEnQueueNum = 0;
	mac_p->macStat.inSilicon.txDeQueueNum = 0;

	return;
}

PUBLIC int
macDrvRegInit
(
	macAdapter_t *mac_p
)
{
	DEBUGMSG(DBG_FUNCTION,("\r\n---->macDrvRegInit"));

  	// ----- setup interrupt mask ---
  	//  ---- Setup HASH table------------------------------------------
  	//  -----Setup Interrupt Timer-------------------------------------
  	//  -----Setup AUTO polling timer----------------------------------
  	//  ---- Setup DMA burst and arbitration---------------------------
  	//  -----Setup DMA Descriptor Base Address Assign------------------
  	//  -----Setup MACCR-----------------------------------------------

	macResetSwMAC();

  	macIMRMask();

  	macSetIntTimer();

//	macSetAutoPollTmr();

//  macSetupDMABurstArbitration();

  	macSetDMADescrCtrlReg(mac_p);

  	macSetMACCR(mac_p);

  // --- setup MAC address ---
  	macSetMacReg(mac_p);

  	DEBUGMSG(DBG_FUNCTION,("\r\n<----macDrvRegInit"));
  	return 0;
}

PRIVATE void
macDrvDescripReset
(
	macAdapter_t *mac_p
)
{
	int i;

	macRxDescr_t * pRxDescp;
	macTxDescr_t * pTxDescp;
	struct sk_buff *skb;

  	DEBUGMSG(DBG_FUNCTION,("\r\n---->macDrvDescripReset"));

	pTxDescp = (macTxDescr_t*) mac_p->txDescrRingBaseVAddr;
	pRxDescp = (macRxDescr_t*) mac_p->rxDescrRingBaseVAddr;

	// free all un-released tx mbuf
	for (i = 0 ; i < mac_p->txRingSize ; i++)
	{
		skb = pTxDescp->skb;
		if( skb != NULL )
		{
			dev_kfree_skb_any(skb);
		}
		pTxDescp->tdes0.word  = DESCP_OWNED_BY_CPU_WORD;
		pTxDescp->tdes2.buf_p = 0;
		pTxDescp->skb = NULL;
		pTxDescp++;
	}

	for (i = 0; i < mac_p->rxRingSize; i++)
	{
		pRxDescp->rdes0.word = DESCP_OWNED_BY_DMA_WORD;
		pRxDescp++;
	}

	mac_p->txCurrentDescp     = 0;
	mac_p->txUnReleasedDescp  = 0;
	mac_p->txUnReleasedBufCnt = 0;
	mac_p->rxCurrentDescp     = 0;

  	DEBUGMSG(DBG_FUNCTION,("\r\n<----macDrvDescripReset"));
}

PRIVATE uint8
macDrvDescripInit
(
	macAdapter_t *mac_p
)
{
	macRxDescr_t *pRxDescp;
  	macTxDescr_t *pTxDescp;
  	uint32 i;
  	struct sk_buff *skb;

  	DEBUGMSG(DBG_FUNCTION,("\r\n---->macDrvDescripInit"));
	mac_p->txDescrRingBaseVAddr = K0_TO_K1(align16Byte((uint32)&mac_p->macMemPool_p->descrPool[0]));

	mac_p->rxDescrRingBaseVAddr	= mac_p->txDescrRingBaseVAddr + (mac_p->txDescrSize * mac_p->txRingSize);

	// add function here to free all mbuf after Mac is soft-reset */
	if( macInitialized )
	{
		macDrvDescripReset( mac_p );
	}

	/* init. tx descriptor, don't allocate memory */
	pTxDescp = (macTxDescr_t*) mac_p->txDescrRingBaseVAddr;
	mac_p->txCurrDescp = pTxDescp;
	
	for ( i = 0 ; i < mac_p->txRingSize ; i++, pTxDescp++ )
	{
    	// Init descriptor
    	pTxDescp->tdes0.word = 0;
    	pTxDescp->tdes1.word = 0;
    	pTxDescp->tdes2.word = 0;

    	// Assign flag
    	pTxDescp->tdes0.bits.ownerDma = 0;  /* owned by CPU */
    	pTxDescp->tdes1.bits.sw_txtag_en = 0; /* SW Tag */
    	pTxDescp->tdes1.bits.ipcs_en     = 0; /* IP Chksum */
    	pTxDescp->tdes1.bits.vtag_en     = 0; /* VLAN Tag */
    	pTxDescp->skb = NULL;
	}

	/* init. Rx descriptor, allocate memory for each descriptor */
	pRxDescp = (macRxDescr_t*) mac_p->rxDescrRingBaseVAddr;

	for( i = 0 ; i< mac_p->rxRingSize ; i++, pRxDescp++)
	{
		// Init Descriptor
		pRxDescp->rdes0.word = 0;
		pRxDescp->rdes1.word = 0;
		pRxDescp->rdes2.word = 0;

		// Assign flag
		pRxDescp->rdes0.bits.ownerDma = 1;  /* owned by DMA */
		pRxDescp->rdes1.bits.rxBufSize = 1776;

		skb = skbmgr_dev_alloc_skb2k();
		if( skb == NULL )
		{
			printk("tc3162_mac_descinit init fail.\n");
			return 1;
		}
		/*To invaild the cache memroy*/
		dma_cache_inv((unsigned long)(skb->data), RX_MAX_PKT_LEN);
		
		skb_reserve(skb, 2);
		//skb_reserve(skb, 8);

		pRxDescp->rdes2.buf_p = K1_TO_PHY(skb->data);
		pRxDescp->skb = skb;
	}

	mac_p->txCurrentDescp     = 0;
	mac_p->txUnReleasedDescp  = 0;
	mac_p->txUnReleasedBufCnt = 0;
	mac_p->rxCurrentDescp     = 0;

  	DEBUGMSG(DBG_FUNCTION,("\r\n<----macDrvDescripInit"));
	return 0;
}

PUBLIC void
macDrvStart
(
	void
)
{
    macCtrlReg_t macCtrl;

	macCtrl.value = VPint(CR_MAC_MACCR);
	/* enable DMA tx& rx */
	macCtrl.bits.rdma_en = 1;
	macCtrl.bits.xdma_en = 1;
	/* enable mac tx& rx */
	macCtrl.bits.rcv_en = 1;
	macCtrl.bits.xmt_en = 1;
	VPint(CR_MAC_MACCR) = macCtrl.value;
}

void
macPhyReset
(
	void
)
{
	local_irq_disable();
	
	#ifdef TCPHY_SUPPORT
	if(getTcPhyFlag()){
		tcPhyInit(mac_p);
	}
	else  
	#endif	
	{
	miiStationWrite( mac_p, PHY_CONTROL_REG, PHY_RESET );
	miiStationWrite( mac_p, PHY_CONTROL_REG, MIIDR_AUTO_NEGOTIATE );
}

	local_irq_enable();
}

#ifdef TCSUPPORT_AUTOBENCH
extern int autobench_mac_lpbk_flag;
extern int autobench_mac_lpbk_cnt;
extern int autobench_mac_lpbk1_flag;
extern unsigned char LoopbackData[];
#endif

__IMEM
#ifdef TC3162_MAC_NAPI
int macRxRingProc(struct net_device *dev, int quota)
#else
void macRxRingProc(struct net_device *dev)
#endif
{
	volatile macRxDescr_t *rxDescrp;
	uint32 frameSize;
	struct sk_buff *newMbp, *oldMbp;
	uint32 rdes0_error_check, rdes1_error_check;
#ifdef TC3162_MAC_NAPI
	int npackets = 0;
#endif	
	#ifdef TCPHY_4PORT
	uint16 maxPktSize=0;
	#endif

	rxDescrp = ((macRxDescr_t*)mac_p->rxDescrRingBaseVAddr) + mac_p->rxCurrentDescp;
	/*Not check long packet error due to we enable the special tag the max packet size will be too long*/
	rdes0_error_check = (1<<27|1<<22|1<<21|1<<19|1<<18);
  	rdes1_error_check = 0;
#ifdef TC3162_MAC_NAPI
	while ((npackets <= quota) && ((rxDescrp->rdes0.word & (1<<31)) == 0)) // ownerDMA for TCMAC
#else
  	while ((rxDescrp->rdes0.word & (1<<31)) == 0) // ownerDMA for TCMAC
#endif		
  	{
#ifdef TC3162_MAC_NAPI
		npackets++;
#endif		
		// According to UNH document, the alignment error should be recorded and
	  	// the packet may be discarded if CRC error happens.
		if ( rxDescrp->rdes1.word&(1<<27) )
	  	{
	    	mac_p->macStat.inSilicon.rxAlignErr++;
	    	mac_p->macStat.inSilicon.rxDribblingErr++;
	  	}

	    if ( (rxDescrp->rdes0.word & rdes0_error_check) == 0 &&
        	(rxDescrp->rdes1.word & rdes1_error_check) == 0 )
	    {	
	    	// For removing Rx VLAN tag, we need to check if the remain length
	    	// is not smaller than minimun packet length.
	    	// rxDescrp->rdes0.bits.vlan_hit
	    	if (rxDescrp->rdes0.word & (1<<30))
	    	{
	      		frameSize = rxDescrp->rdes0.bits.rfl;
	      		frameSize = (frameSize > 60) ? frameSize : 60;
	      	}
	 		else
 	 			frameSize = rxDescrp->rdes0.bits.rfl;
 	 		if (rxDescrp->rdes1.word & (1<<28))
	        	mac_p->macStat.inSilicon.rx802p3FrameLengthErr++;

		#ifndef TCPHY_4PORT
	      	/* lino: make VLAN friendly */
	      	if ( (frameSize < 60) || (frameSize > 1522) )
		#else
		switch(swicVendor){
			case SWIC_TC2206:
				/*TC2206 Special Tag Length=8bytes, vlan header 4 bytes, max ethernet length=1518; 1518+8+4=1530*/
				maxPktSize=1530;
				break;
			default:
				maxPktSize=1522;
				break;
		}
		if ( (frameSize < 60) || (frameSize > maxPktSize))
		#endif
	      	{
	      		DEBUGMSG(DBG_ERROR,("\r\n frame size is out of range"));
	        	mac_p->macStat.inSilicon.rxEtherFrameLengthErr++;

	        	// Discard this packet & Repost this mbuf
	        	(volatile struct sk_buff *)newMbp = (volatile struct sk_buff *)(rxDescrp->skb);
	      		DEBUGMSG(DBG_ERROR,("\r\n the packet will be discarded"));
	        	goto DISCARD;
	      	}

	      	// ----- Count the MIB-II -----
		    if ( mac_p->statisticOn )
			{
	  	    	mac_p->macStat.MIB_II.inOctets += frameSize;
	
			    if ( (rxDescrp->rdes0.word)&(1<<16) )
			    	mac_p->macStat.MIB_II.inMulticastPkts++;
			    else
		        	mac_p->macStat.MIB_II.inUnicastPkts++;
		    }

	      	(volatile struct sk_buff *)oldMbp = (volatile struct sk_buff *)(rxDescrp->skb);
#ifdef TCPHY_DEBUG
		if(getTcPhyFlag()){
		      if(phyMbpChk(oldMbp) == 1){
			  	newMbp = oldMbp;
				goto RECVOK;
			}
		}
#endif
			newMbp = skbmgr_dev_alloc_skb2k();
	        if ( !newMbp )  /* faild to allocate more mbuf -> drop this pkt */
	        {
	        	newMbp = oldMbp;
	          	mac_p->macStat.MIB_II.inDiscards++;
	          	goto RECVOK;
	      	}
			/*Invaild the cache memory*/
			dma_cache_inv((unsigned long)(newMbp->data), RX_MAX_PKT_LEN);
			skb_reserve(newMbp, 2);
			skb_put(oldMbp, frameSize);

#if defined(TCSUPPORT_CT)
#if defined(WAN2LAN) || defined(CONFIG_TC3162_ADSL)
	#ifndef TC2031_SUPPORT
			/*add specail tag function by xyzhu*/
			if (macSTagFlag)
			{
				switch (swicVendor)
				{
					case SWIC_ADM6996M:
						//macAdmSTagRemove(oldMbp);
						break;
					case SWIC_IP175C:
						macIpSTagRemove(oldMbp);
					break;
					case SWIC_TC2206:
						macTC2206STagRemove(oldMbp);
						break;
					default:
						break;
				}
			}
	#endif
#endif
#else
#if 1//defined(WAN2LAN) || defined(CONFIG_TC3162_ADSL) || defined(VPORT)
	#ifndef TC2031_SUPPORT
			/*add specail tag function by xyzhu*/
			if (macSTagFlag)
			{
				switch (swicVendor)
				{
					case SWIC_ADM6996M:
						//macAdmSTagRemove(oldMbp);
						break;
					case SWIC_IP175C:
						macIpSTagRemove(oldMbp);
					break;
					case SWIC_TC2206:
						macTC2206STagRemove(oldMbp);
						break;
					default:
						break;
				}
			}
	#endif
#endif
#endif


#ifdef TCSUPPORT_AUTOBENCH
			if( autobench_mac_lpbk_flag ){

				if( memcmp(oldMbp->data, LoopbackData, MAC_LPBK_DATA_LEN) == 0 )
					autobench_mac_lpbk_cnt++;
				else{

					int i;
					for( i=0; i<MAC_LPBK_DATA_LEN; i++){
						printk("%02x ", (unsigned char)oldMbp->data[i]);
						if( (i+1)%8 == 0 )
							printk("\n");
					}
				}
				dev_kfree_skb_any(oldMbp);
				goto RECVOK;
			}
			if( autobench_mac_lpbk1_flag ){
				autobench_mac_lpbk_cnt++;
				dev_kfree_skb_any(oldMbp);
				goto RECVOK;
			}
#endif
		#ifdef EEE_DEBUG
		if(dbgLevel & DUMP_RX_PKT){
			dump_skb(oldMbp);
		}
		if (LOOPBACK_MODE(macLoopback) == LOOPBACK_TX) {
			/*Used received the packets to send it.*/
			tc3162_mac_tx(oldMbp, tc3162_mac_dev);
			goto RECVOK;
		}else if (LOOPBACK_MODE(macLoopback) == LOOPBACK_RX_CHK) {
			tc3162_mac_loopback_chk(oldMbp, tc3162_mac_dev);
			goto RECVOK;
		}
		#endif	

#ifdef CONFIG_TC3162_ADSL
			if(tcconsole_proc(oldMbp)==1){
				;
			} else {
#else
			{
#endif
				oldMbp->dev = dev;
				oldMbp->ip_summed = CHECKSUM_NONE;
				oldMbp->protocol = eth_type_trans(oldMbp, dev);
				dev->last_rx = jiffies;
#ifdef TC3162_MAC_NAPI
				netif_receive_skb(oldMbp);
#else
				netif_rx(oldMbp);
#endif				
			}
DISCARD:

RECVOK:

	      	rxDescrp->rdes2.buf_p = K1_TO_PHY(newMbp->data);
			rxDescrp->skb = newMbp;
		}
		else  /* Update Error Counter and Drop it */
		{
	      	if ( mac_p->statisticOn )
    	  	{
		  		if ( rxDescrp->rdes1.word&(1<<28) )
				{
					mac_p->macStat.inSilicon.rx802p3FrameLengthErr++;
					mac_p->macStat.MIB_II.inErrors++;
				}	
				if ( rxDescrp->rdes0.word&(1<<27) )
				{
					mac_p->macStat.inSilicon.rxPktIPChkSumErr++;
					mac_p->macStat.MIB_II.inErrors++;
				}
	        	if ( rxDescrp->rdes0.word&(1<<22) )
		        {
		        	mac_p->macStat.inSilicon.rxCollisionErr++;
		          	mac_p->macStat.MIB_II.inErrors++;
		        }
		        if ( rxDescrp->rdes0.word&(1<<21) )
				{
					mac_p->macStat.inSilicon.rxRuntErr++;
					mac_p->macStat.MIB_II.inErrors++;
				}
		        if ( rxDescrp->rdes0.word&(1<<20) )
				{
					mac_p->macStat.inSilicon.rxLongErr++;
					mac_p->macStat.MIB_II.inErrors++;
				}
		        if ( rxDescrp->rdes0.word&(1<<19) )
				{
					mac_p->macStat.inSilicon.rxCrcErr++;
					mac_p->macStat.MIB_II.inErrors++;
					#ifdef TCPHY_DEBUG	
					if(getTcPhyFlag())
						phy_recv_err_check(&oldMbp, rxDescrp, &frameSize);
					#endif	
				}
		        if ( rxDescrp->rdes0.word&(1<<18) )
				{
					mac_p->macStat.inSilicon.rxMiiErr++;
					mac_p->macStat.MIB_II.inErrors++;
				}
			} /* if mac_p->statisticOn */
		}
	
	    rxDescrp->rdes0.word |= (1<<31);
	
 		/* next descriptor*/
		if ( mac_p->rxCurrentDescp == ( mac_p->rxRingSize - 1 ))
			mac_p->rxCurrentDescp = 0;
		else
			mac_p->rxCurrentDescp++;
		
		rxDescrp = ((macRxDescr_t*)mac_p->rxDescrRingBaseVAddr) + mac_p->rxCurrentDescp;
	} /* while loop  */

    VPint(CR_MAC_RXR_SWIDX) = mac_p->rxCurrentDescp;
#ifdef TC3162_MAC_NAPI
	return npackets;
#endif	
}

PUBLIC void
macControllerStop(macAdapter_t *mac_p)
{
	macCtrlReg_t macCtrl;

	macCtrl.value = VPint(CR_MAC_MACCR);
	/* disable DMA tx& rx */
	macCtrl.bits.rdma_en = 0;
	macCtrl.bits.xdma_en = 0;
	/* disable mac tx& rx */
	macCtrl.bits.rcv_en = 0;
	macCtrl.bits.xmt_en = 0;
	VPint(CR_MAC_MACCR) = macCtrl.value;
}

PUBLIC macAdapter_t *
macGetAdapterByChanID(void)
{
	return mac_p;
}

PUBLIC void
macGetMacAddr
(
  macAdapter_t *mac_p
)
{
	uint32 i;

	for ( i = 0; i < 6; i++ )
	{
		mac_p->macAddr[i] = def_mac_addr[i];
	}
}

PRIVATE int32
macPhyLookUp(
	macAdapter_t *mac_p,
	uint32 companyId
)
{
	uint32 i;
	uint32 phyTypeSupport;

	phyTypeSupport = sizeof(phyDeviceList) / sizeof(phyDeviceList_t);
	for ( i = 0; i < phyTypeSupport; i++ ) {
		if ( companyId == phyDeviceList[i].companyId ) {
			mac_p->enetPhyId = i;
			#ifdef TCPHY_SUPPORT	
			if( companyId == 0x03a2 ){
				tcPhyVerLookUp(mac_p);
			} 
			#endif	
			return 1;
		}
	}
	return 0;
}

PRIVATE int32
macSearchPhyAddr(
	macAdapter_t *mac_p
)
{
	uint32 miiReg = 0;

	mac_p->enetPhyAddr = 0;
	for ( mac_p->enetPhyAddr = 0; mac_p->enetPhyAddr < 32; mac_p->enetPhyAddr++ ) {
		miiReg = miiStationRead(mac_p, PHY_ID_REG1);
		if (miiReg == 0)
			miiReg = miiStationRead(mac_p, PHY_ID_REG2);
		if ( macPhyLookUp(mac_p, miiReg) ) {
			return 0;
		}
	}

	mac_p->enetPhyAddr = 0x00000000; 

	return -1;
}

void
macPhyDisableLoopback(
	macAdapter_t *mac_p
)
{
	uint32 data;

	data = miiStationRead(mac_p,PHY_CONFIG_REG);
	data |= PHY_TP_LOOPBACK;
	miiStationWrite(mac_p, PHY_CONFIG_REG, data);
}

PUBLIC int
macSetUpPhy
(
	macAdapter_t *mac_p
)
{
	if (isTC3162U) {
		VPint(CR_AHB_SSR) |= (1<<11)|(1<<10)|(1<<9)|(1<<8);
		pause(10);
	}
	else if (isRT63260){
		VPint(CR_AHB_SSR) |= (1<<24);
		pause(10);
	}

	/* ----- Hardware reset Ehernet phy chip, this address is defined by h/w engineer ----- */
	ledTurnOn(LED_LAN_RESET);
	pause(100);
	/* ----- Wait for hardware reset completed ----- */
	ledTurnOff(LED_LAN_RESET);
	pause(600);
	

	macSearchPhyAddr(mac_p);

	macPhyReset();
	pause(100);

	/* Detect 4-port switch or single port switch */
	/* detect if ADM6996M */
	if (((tcAdmMiiStationRead(ADM_CHIP_ID0) & 0xfff0) == 0x1020) &&
		(tcAdmMiiStationRead(ADM_CHIP_ID1) == 0x7)) {
		swicVendor = SWIC_ADM6996M;		    
		/* enable crossover auto detect */
		tcAdmMiiStationWrite(ADM_PORT0_BASIC, tcAdmMiiStationRead(ADM_PORT0_BASIC)|0x8000);
		tcAdmMiiStationWrite(ADM_PORT1_BASIC, tcAdmMiiStationRead(ADM_PORT1_BASIC)|0x8000);
		tcAdmMiiStationWrite(ADM_PORT2_BASIC, tcAdmMiiStationRead(ADM_PORT2_BASIC)|0x8000);
		tcAdmMiiStationWrite(ADM_PORT3_BASIC, tcAdmMiiStationRead(ADM_PORT3_BASIC)|0x8000);
	}
	/* detect if IP175C */
	if ((tcMiiStationRead(4, 2) == 0x243) && (tcMiiStationRead(4, 3) == 0xd80)) {
		swicVendor = SWIC_IP175C;
	}
	
	/* detect if RTL8305 or RTL8036SD Switch IC*/	
	if ((tcMiiStationRead(4, 2) == 0x1c) && (tcMiiStationRead(4, 3) == 0xc852)) {
		if ((tcMiiStationRead(6, 2) == 0x1c) && (tcMiiStationRead(6, 3) == 0xc852) ){
			swicVendor = SWIC_RTL8306SD;
			/*Let CPU Port Link up*/
			tcMiiStationWrite(5, 22,tcMiiStationRead(5,22)|0x8000);
			tcMiiStationWrite(6, 22,tcMiiStationRead(6,22)|0x8000);
		}
		else{
			swicVendor = SWIC_RTL8305;
		}
	}
	#ifdef TCPHY_4PORT
	/*Switch Model Number Register (SMNR), Addr.: 16'h02FE*/
	if (tcMiiStationRead(31, 31) == 0x2206){
	    swicVendor = SWIC_TC2206;	
	    filedSwicDefVal();
	    printk("TC2206, ");
	}
	#endif
	/* detect if MARVEL 88E6060 */	
	if (((tcMiiStationRead(4, 2) == 0x141) || (tcMiiStationRead(20, 2) == 0x141)) && 
	    ((tcMiiStationRead(4, 3) == 0xc87) || (tcMiiStationRead(20, 3) == 0xc87))) 
    	{
		swicVendor = SWIC_MARVEL6060;
	}

	if( swicVendor )
	{
	}
	
	/* ----- Set up Ethernet link speed, using auto negotiation ----- */
	mac_p->macPhyLinkProfile_p->linkSpeed = MIIDR_AUTO_NEGOTIATE;
	mac_p->macPhyLinkProfile_p->duplexMode = 0;
	
#ifdef TCPHY_SUPPORT
	if(getTcPhyFlag() == 0)
	{
#endif	
	if( !swicVendor )
	{
		if ( mac_p->macPhyLinkProfile_p->linkSpeed == MIIDR_AUTO_NEGOTIATE )
		{
			miiStationWrite(mac_p, PHY_CONTROL_REG, MIIDR_AUTO_NEGOTIATE );
			pause(50); /* Timothy debug 1221, without this, negotiation is wrong... */
		}
		/* disable loopback mode */	
		macPhyDisableLoopback( mac_p );
	}
#ifdef TCPHY_SUPPORT	
	}
#endif
  return 0;
}

PUBLIC uint8
macInit
(
	void
)
{
	if (macInitialized)
		return 0;

	macControllerStop(mac_p);

	/* ----- Get Mac Adapter from dummy data or NDIS control block ----- */
	mac_p = macGetAdapterByChanID();
#if defined(TCSUPPORT_CT)
	memset(mac_p, 0x0, sizeof(macAdapter_t));
#else
	/*reset this segment will clear lock state. shnwind 20101129.*/
	//memset(mac_p, 0x0, sizeof(macAdapter_t));

#endif
	/* ----- Assign reserved data pointer ----- */
	mac_p->macPhyLinkProfile_p = &enetPhyLinkProfile;
#ifdef DYNAMIC_ALLOC_DMA_BUF
	enetMemPool = kmalloc(sizeof(macMemPool_t), GFP_ATOMIC | GFP_DMA);
	if (enetMemPool == NULL) {
		printk("unable to kmalloc enetMemPool structure.\n");
		return -1;
	}
	mac_p->macMemPool_p = enetMemPool;
#else
	mac_p->macMemPool_p = &enetMemPool;
#endif

	/* ----- Set up the paramters ----- */
	macDefaultParaSet(mac_p);

	/* ----- Get the Mac address ----- */
	macGetMacAddr(mac_p);

	/* ----- Initialize Tx/Rx descriptors ----- */
	if( macInitialized == 0)
	{
		if ( macDrvDescripInit(mac_p) != 0 )
		{
			return -1;
		}
	}
	else
	{
		macDrvDescripReset( mac_p );
	}

	/* ----- Initialize the SEEQ 80225 phy chip(enet phy) ----- */
	if ( macSetUpPhy(mac_p) )
	{
		return -1;
	}

	/* ----- Initialize Registers ----- */
	macDrvRegInit(mac_p);

	mac_p->statisticOn = MAC_STATISTIC_ON;
#if defined(WAN2LAN) || defined(CONFIG_TC3162_ADSL)
#ifndef TC2031_SUPPORT 
	/* ------ xyzhu_nj_091104:Enable special Tag default ----- */
	macRxPortEnable(0);
#endif
#endif

	macInitialized = 1;
	macDrvStart();

	return 0;
}

/************************************************************************
*     A D S L   R E L A T E D    F U N C T I O N S
*************************************************************************
*/

struct sk_buff_head adsl_pkt_queue;
struct work_struct tq_adsl;
struct work_struct tq_mac;

void tc3162_emac_poll(void)
{
	tc3162_mac_isr(tc3162_mac_dev->irq, tc3162_mac_dev);
}

static void tc3162_mac_poll(unsigned long data)
{
#ifdef CONFIG_TC3162_ADSL
	uint8 modemst;

	if (adsl_dev_ops == NULL)
		goto down_proc;

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL);		  
	switch (modemst) {
		case ADSL_MODEM_STATE_DOWN:
down_proc:
			mod_timer(&eth_poll_timer, jiffies + (HZ / 2)); 
			break;
		case ADSL_MODEM_STATE_WAIT_INIT:
			/* polling ethernet */
			if (!(VPint(CR_INTC_IMR) & (1 << MAC_INT)))
				tc3162_emac_poll();
			mod_timer(&eth_poll_timer, jiffies + (HZ / 10)); 
			break;
		case ADSL_MODEM_STATE_INIT:
			/* polling ethernet */
			if (!(VPint(CR_INTC_IMR) & (1 << MAC_INT)))
				schedule_work(&tq_mac);
			mod_timer(&eth_poll_timer, jiffies + (HZ / 10)); 
			break;
		case ADSL_MODEM_STATE_UP:
			mod_timer(&eth_poll_timer, jiffies + (HZ / 2)); 
			break;
	}
#endif
}

static void tc3162_mac_poll_rcv(struct work_struct *work)
{
	/* polling ethernet */
	if (!(VPint(CR_INTC_IMR) & (1 << MAC_INT)))
		tc3162_emac_poll();
}

void TCConsole(uint8 mode)
{
}
EXPORT_SYMBOL(TCConsole);

void uartMacPutchar(int ch)
{
}
EXPORT_SYMBOL(uartMacPutchar);


static void macSendReal(struct work_struct *work)
{
	for (;;) {
		struct sk_buff *skb;

		local_irq_disable();
		skb = __skb_dequeue(&adsl_pkt_queue);
		if (skb == NULL) {
			local_irq_enable();
			break;
		}
		tc3162_mac_tx(skb, tc3162_mac_dev);
		local_irq_enable();
	}
}

uint32 GetIpAddr(void)
{
	return 0;
}
EXPORT_SYMBOL(GetIpAddr);

uint8 *GetMacAddr(void)
{
	static uint8 macAddr[7];
  
  	memcpy(macAddr, def_mac_addr, 6);
	macAddr[6] = 0x0;
	return macAddr;
}
EXPORT_SYMBOL(GetMacAddr);

/************************************************************************
*     E T H E R N E T    D E V I C E   P R O C  D E F I N I T I O N S
*************************************************************************
*/

#define CHK_BUF() pos = begin + index; if (pos < off) { index = 0; begin = pos; }; if (pos > off + count) goto done;

static int getETHLinkSt(char *buf)
{
	uint16 index = 0;

	if ((mac_p == NULL) || (mac_p->macPhyLinkProfile_p == NULL) 
		|| (mac_p->macPhyLinkProfile_p->enetMode == 0)) {
		index += sprintf(buf+index, "Down\n");
		return index;
	}

	if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_100MB)
		index += sprintf(buf+index, "100M/");
	else 
		index += sprintf(buf+index, "10M/");
	if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_FULL_DUPLEX)
		index += sprintf(buf+index, "Full Duplex\n");
	else 
		index += sprintf(buf+index, "Half Duplex\n");
	return index;
}

static int eth_stats_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;

	if (!macInitialized) {
		*eof = 1;
		return 0;
	}

	index += sprintf(buf+index, "inOctets              = 0x%08lx, ", mac_p->macStat.MIB_II.inOctets);
	CHK_BUF();
	index += sprintf(buf+index, "inUnicastPkts         = 0x%08lx\n", mac_p->macStat.MIB_II.inUnicastPkts);
	CHK_BUF();

	index += sprintf(buf+index, "inMulticastPkts       = 0x%08lx, ", mac_p->macStat.MIB_II.inMulticastPkts);
	CHK_BUF();
	index += sprintf(buf+index, "inDiscards            = 0x%08lx\n", mac_p->macStat.MIB_II.inDiscards);
	CHK_BUF();

	index += sprintf(buf+index, "inErrors              = 0x%08lx, ", mac_p->macStat.MIB_II.inErrors);
	CHK_BUF();
	index += sprintf(buf+index, "outOctets             = 0x%08lx\n", mac_p->macStat.MIB_II.outOctets);
	CHK_BUF();

	index += sprintf(buf+index, "outUnicastPkts        = 0x%08lx, ", mac_p->macStat.MIB_II.outUnicastPkts);
	CHK_BUF();
	index += sprintf(buf+index, "outMulticastPkts      = 0x%08lx\n", mac_p->macStat.MIB_II.outMulticastPkts);
	CHK_BUF();

	index += sprintf(buf+index, "outDiscards           = 0x%08lx, ", mac_p->macStat.MIB_II.outDiscards);
	CHK_BUF();
	index += sprintf(buf+index, "outErrors             = 0x%08lx\n", mac_p->macStat.MIB_II.outErrors);
	CHK_BUF();

	index += sprintf(buf+index, "\n[ inSilicon Statistics Display ]\n");
	CHK_BUF();
	index += sprintf(buf+index, "txJabberTimeCnt       = 0x%08lx  ", mac_p->macStat.inSilicon.txJabberTimeCnt);
	CHK_BUF();
	index += sprintf(buf+index, "txLossOfCarrierCnt    = 0x%08lx\n", mac_p->macStat.inSilicon.txLossOfCarrierCnt);
	CHK_BUF();
	index += sprintf(buf+index, "txNoCarrierCnt        = 0x%08lx  ", mac_p->macStat.inSilicon.txNoCarrierCnt);
	CHK_BUF();
	index += sprintf(buf+index, "txLateCollisionCnt    = 0x%08lx\n", mac_p->macStat.inSilicon.txLateCollisionCnt);
	CHK_BUF();
	index += sprintf(buf+index, "txExCollisionCnt      = 0x%08lx  ", mac_p->macStat.inSilicon.txExCollisionCnt);
	CHK_BUF();
	index += sprintf(buf+index, "txHeartbeatFailCnt    = 0x%08lx\n", mac_p->macStat.inSilicon.txHeartbeatFailCnt);
	CHK_BUF();
	index += sprintf(buf+index, "txCollisionCnt        = 0x%08lx  ", mac_p->macStat.inSilicon.txCollisionCnt);
	CHK_BUF();
	index += sprintf(buf+index, "txExDeferralCnt       = 0x%08lx\n", mac_p->macStat.inSilicon.txExDeferralCnt);
	CHK_BUF();
	index += sprintf(buf+index, "txUnderRunCnt         = 0x%08lx  ", mac_p->macStat.inSilicon.txUnderRunCnt);
	CHK_BUF();

	index += sprintf(buf+index, "rxAlignErr            = 0x%08lx\n", mac_p->macStat.inSilicon.rxAlignErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxDribblingErr        = 0x%08lx  ", mac_p->macStat.inSilicon.rxDribblingErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxSymbolErr           = 0x%08lx\n", mac_p->macStat.inSilicon.rxSymbolErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxMiiErr              = 0x%08lx  ", mac_p->macStat.inSilicon.rxMiiErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxCollisionErr        = 0x%08lx\n", mac_p->macStat.inSilicon.rxCollisionErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxCrcErr              = 0x%08lx  ", mac_p->macStat.inSilicon.rxCrcErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxEtherFrameLengthErr = 0x%08lx\n", mac_p->macStat.inSilicon.rxEtherFrameLengthErr);
	CHK_BUF();
	index += sprintf(buf+index, "rx802p3FrameLengthErr = 0x%08lx  ", mac_p->macStat.inSilicon.rx802p3FrameLengthErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxPktIPChkSumErr      = 0x%08lx\n", mac_p->macStat.inSilicon.rxPktIPChkSumErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxRuntErr             = 0x%08lx  ", mac_p->macStat.inSilicon.rxRuntErr);
	CHK_BUF();
	index += sprintf(buf+index, "rxLongErr             = 0x%08lx\n", mac_p->macStat.inSilicon.rxLongErr);
	CHK_BUF();
#if !defined(TCSUPPORT_CT)
	index += sprintf(buf+index, "rxOverrunInt          = 0x%08lx\n", mac_p->macStat.inSilicon.rxOverrunInt);
	CHK_BUF();

#endif
	index += sprintf(buf+index, "\n[ Extra information ]\n");
	CHK_BUF();
	index += sprintf(buf+index, "Rx Descriptor idx     = 0x%08lx  ", mac_p->rxCurrentDescp);
	CHK_BUF();
	index += sprintf(buf+index, "Tx Descriptor idx     = 0x%08lx\n", mac_p->txCurrentDescp);
	CHK_BUF();
 	index += sprintf(buf+index, "Rx Enqueued cnt       = 0x%08lx  ", mac_p->macStat.inSilicon.rxEnQueueNum);
	CHK_BUF();
  	index += sprintf(buf+index, "Tx Enqueued cnt       = 0x%08lx\n", mac_p->macStat.inSilicon.txEnQueueNum);		
	CHK_BUF();
  	index += sprintf(buf+index, "Rx Dequeued cnt       = 0x%08lx  ", mac_p->macStat.inSilicon.rxDeQueueNum);
	CHK_BUF();
  	index += sprintf(buf+index, "Tx Dequeued cnt       = 0x%08lx\n", mac_p->macStat.inSilicon.txDeQueueNum);
	CHK_BUF();
  	index += sprintf(buf+index, "Tx Buf UnReleased cnt = 0x%08lx  ", mac_p->txUnReleasedBufCnt);
	CHK_BUF();
  	index += sprintf(buf+index, "Tx Buf UnReleased idx = 0x%08lx\n", mac_p->txUnReleasedDescp);
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

static int eth_stats_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32];

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	if (!macInitialized) {
		return 0;
	}

	memset(&mac_p->macStat.MIB_II, 0, sizeof(macMIB_II_t));
	memset(&mac_p->macStat.inSilicon, 0, sizeof(inSiliconStat_t));

	return count;
}

#if 0
/* support port reverse */
static int port_reverse_read_proc(char *buf, char **start, off_t off, int count,
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
static int port_reverse_write_proc(struct file *file, const char *buffer,
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

	/* debug */
	//printk("g_port_reverse is %d\n", g_port_reverse);
	
	return len;
}
#endif

int eth_link_st_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getETHLinkSt(buf);
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

int eth_reg_dump_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	int i;
	uint32 reg;

	if (!macInitialized) {
		*eof = 1;
		return 0;
	}

	index += sprintf(buf+index, "CR_MAC_ISR          (BFB50000) = 0x%08lx\n", VPint(CR_MAC_ISR)); 	// --- Interrupt Status Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_IMR          (BFB50004) = 0x%08lx\n", VPint(CR_MAC_IMR)); 	// --- Interrupt Mask Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_MADR         (BFB50008) = 0x%08lx\n", VPint(CR_MAC_MADR)); 	// --- MAC Address Register [47:32] ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_LADR         (BFB5000c) = 0x%08lx\n", VPint(CR_MAC_LADR)); 	// --- MAC Address Register [31:0] ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_TXPD         (BFB50018) = 0x%08lx\n", VPint(CR_MAC_TXPD)); 	// --- Transmit Poll Demand Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_RXPD         (BFB5001c) = 0x%08lx\n", VPint(CR_MAC_RXPD)); 	// --- Receive Poll Demand Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_TXR_BADR     (BFB50020) = 0x%08lx\n", VPint(CR_MAC_TXR_BADR)); // --- Transmit Ring Base Address Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_RXR_BADR     (BFB50024) = 0x%08lx\n", VPint(CR_MAC_RXR_BADR)); // --- Receive Ring Base Address Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_ITC          (BFB50028) = 0x%08lx\n", VPint(CR_MAC_ITC)); 	// --- Interrupt Timer Control Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_TXR_SIZE     (BFB5002c) = 0x%08lx\n", VPint(CR_MAC_TXR_SIZE));  	// --- Automatic Polling Timer Control Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_RXR_SIZE     (BFB50030) = 0x%08lx\n", VPint(CR_MAC_RXR_SIZE)); 	// --- DMA Burst Length and Arbitration Control Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_RXR_SWIDX    (BFB50034) = 0x%08lx\n", VPint(CR_MAC_RXR_SWIDX)); 	// --- DMA Burst Length and Arbitration Control Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_TXDESP_SIZE  (BFB50038) = 0x%08lx\n", VPint(CR_MAC_TXDESP_SIZE)); // --- Transmit Address Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_RXDESP_SIZE  (BFB5003c) = 0x%08lx\n", VPint(CR_MAC_RXDESP_SIZE)); // --- Receive Address Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_PRIORITY_CFG (BFB50050) = 0x%08lx\n", VPint(CR_MAC_PRIORITY_CFG)); // --- Receive Address Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_VLAN_CFG     (BFB50054) = 0x%08lx\n", VPint(CR_MAC_VLAN_CFG)); // --- Receive Address Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_TOS0_CFG     (BFB50058) = 0x%08lx\n", VPint(CR_MAC_TOS0_CFG)); // --- Receive Address Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_TOS1_CFG     (BFB5005c) = 0x%08lx\n", VPint(CR_MAC_TOS1_CFG)); // --- Receive Address Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_TOS2_CFG     (BFB50060) = 0x%08lx\n", VPint(CR_MAC_TOS2_CFG)); // --- Receive Address Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_TOS3_CFG     (BFB50064) = 0x%08lx\n", VPint(CR_MAC_TOS3_CFG)); // --- Receive Address Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_TCP_CFG      (BFB50068) = 0x%08lx\n", VPint(CR_MAC_TCP_CFG)); // --- Receive Address Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_SWTAG_CFG    (BFB5006c) = 0x%08lx\n", VPint(CR_MAC_SWTAG_CFG)); // --- Receive Address Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_PMBL_CYC_NUM (BFB50070) = 0x%08lx\n", VPint(CR_MAC_PMBL_CYC_NUM)); // --- Receive Address Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_FCTL_CYC_NUM (BFB50074) = 0x%08lx\n", VPint(CR_MAC_FCTL_CYC_NUM)); // --- Receive Address Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_JAM_CYC_NUM  (BFB50078) = 0x%08lx\n", VPint(CR_MAC_JAM_CYC_NUM)); // --- Receive Address Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_DEFER_VAL    (BFB5007c) = 0x%08lx\n", VPint(CR_MAC_DEFER_VAL)); // --- Receive Address Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_RANDOM_POLY  (BFB50080) = 0x%08lx\n", VPint(CR_MAC_RANDOM_POLY)); // --- Receive Address Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_MACCR        (BFB50088) = 0x%08lx\n", VPint(CR_MAC_MACCR)); 	// --- MAC Control Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_MACSR        (BFB5008c) = 0x%08lx\n", VPint(CR_MAC_MACSR)); 	// --- MAC Status Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_PHYCR        (BFB50090) = 0x%08lx\n", VPint(CR_MAC_PHYCR)); 	// --- PHY Control Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_PHYWDATA     (BFB50094) = 0x%08lx\n", VPint(CR_MAC_PHYWDATA)); // --- PHY Write Data Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_FCR          (BFB50098) = 0x%08lx\n", VPint(CR_MAC_FCR)); 	// --- Flow Control Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_BPR          (BFB5009c) = 0x%08lx\n", VPint(CR_MAC_BPR)); 	// --- Back Pressure Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_TM           (BFB500cc) = 0x%08lx\n", VPint(CR_MAC_TM)); 		// --- Test Mode Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_XMPG_CNT     (BFB500dc) = 0x%08lx\n", VPint(CR_MAC_XMPG_CNT)); // --- XM and PG Counter Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_RUNT_TLCC_CNT(BFB500e0) = 0x%08lx\n", VPint(CR_MAC_RUNT_TLCC_CNT)); 	// --- Runt and Late Collision Packet Counter Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_RCRC_RLONG_CNT(BFB500e4) = 0x%08lx\n", VPint(CR_MAC_RCRC_RLONG_CNT)); 	// --- CRC and Long Packet Counter Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_RLOSS_RCOL_CNT(BFB500e8) = 0x%08lx\n", VPint(CR_MAC_RLOSS_RCOL_CNT)); 	// --- Receive Packet Loss and Receive Collision Counter Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_BROADCAST_CNT(BFB500ec) = 0x%08lx\n", VPint(CR_MAC_BROADCAST_CNT)); 	// --- Receive Broadcast Counter Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_MULTICAST_CNT(BFB500f0) = 0x%08lx\n", VPint(CR_MAC_MULTICAST_CNT)); 	// --- Receive Multicast Counter Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_RX_CNT       (BFB500f4) = 0x%08lx\n", VPint(CR_MAC_RX_CNT)); 	// --- Receive Good Packet Counter Register ---
	CHK_BUF();
	index += sprintf(buf+index, "CR_MAC_TX_CNT       (BFB500f8) = 0x%08lx\n", VPint(CR_MAC_TX_CNT)); 	// --- Transmit Good Packet Counter Register ---
	CHK_BUF();

	index += sprintf(buf+index, "[PHY REG] PHYADDR=%d\n", mac_p->enetPhyId);
	CHK_BUF();
	for (i = 0; i <= 8; i++) {
   		reg = miiStationRead(mac_p, i);
		index += sprintf(buf+index, "PHY reg%d=0x%08lx\n", i, reg);
		CHK_BUF();
	}

   	reg = miiStationRead(mac_p, 10);
	index += sprintf(buf+index, "PHY reg%d=0x%08lx\n", 10, reg);
	CHK_BUF();
 
	for (i = 16; i <= 20; i++) {
   		reg = miiStationRead(mac_p, i);
		index += sprintf(buf+index, "PHY reg%d=0x%08lx\n", i, reg);
		CHK_BUF();
	}

   	reg = miiStationRead(mac_p, 30);
	index += sprintf(buf+index, "PHY reg%d=0x%08lx\n", 30, reg);
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

#ifdef TCPHY_SUPPORT
/*_____________________________________________________________________________
**      function name: periodChk
**      descriptions:
**      	The periodic check  TC phy or switch register, and make sure
**      the ethernet can be work.
**   
**      parameters:
**         None
**             
**      global:
**         None
**             
**      return:
**             None
**	     
**      call:
**   	     getTcPhyFlag
**   	     esdSwPatch
**   	     updatingUpnpGid
**   	     tcPhyChkVal
**   	     tcPhyErrMonitor
** 
**      revision:
**      1. Here 2010/05/11
**____________________________________________________________________________
*/
void
periodChk(void){
	if (getTcPhyFlag()){
		tcPhySwPatch();
		#ifdef TCPHY_4PORT
		esdSwPatch();
		/*Workaroud to fix hardware igmpsnoop function actived then 
		the upnp multicast packet will be drop.(239.255.255.250)*/
		updatingUpnpGid();
		#endif
		#ifdef TCPHY_DEBUG
		tcPhyChkVal();
		tcPhyErrMonitor();
		#endif
	}
}
#endif
#if defined(TCSUPPORT_ETHER_ALL_LED)
/* if all of LAN ports is link down ,return 0;otherwise ,return 1*/

int ether_states_handler(void)
{
	uint32 tx_pkt_num=0,rx_pkt_num=0,pkt_num=0;;
	int i =0,port_num =0,ret = 0;
	uint8 speed;

    if(macSTagFlag){
	#ifdef TCSUPPORT_2PORTS
		port_num = 2;
	#else
	    port_num = 4;
	#endif
	}else{
	    port_num = 1;
	}   

	if(swicVendor == SWIC_TC2206){
		for (i=0; i< port_num; i++){
			pkt_num=tcMiiStationRead(i, MII_BMSR);
			if(pkt_num&BMSR_LSTATUS){
				ret ++; //test
				tcMiiStationWrite(22,16,((i<<12)+0x0001));
				rx_pkt_num += (tcMiiStationRead(22,17)<<16)+tcMiiStationRead(22,18);
				tx_pkt_num += (tcMiiStationRead(22,25)<<16)+tcMiiStationRead(22,26);
			}
		}
		if(ret){
			pkt_num = rx_pkt_num + tx_pkt_num;
			if(pkt_num > (ether_speed[0]?ether_speed[0]:200)){
				speed = 1;
			}else if(pkt_num > (ether_speed[1]?ether_speed[1]:150)){
				speed = 2;
			}else if(pkt_num > (ether_speed[2]?ether_speed[2]:100)){
				speed = 4;
			}else if(pkt_num > (ether_speed[3]?ether_speed[3]:50)){
				speed = 8;
			}else if(pkt_num > 0){
				speed = 16;
			}else speed = 0;
			//printk("port,rx,tx,speed=%d,%x,%x,%d\n",ret,rx_pkt_num,tx_pkt_num,speed);
			
			if(pkt_num > 0){
				if(ether_lan_speed == 0){//ledturnon will change blink speed
					//ledTurnOff(LED_ETHER_STATUS);
					ledTurnOn(LED_ETHER_ACT_STATUS);
					ledTurnOff(LED_ETHER_NOACT_STATUS);
				}
			}
			else{
				if(ether_lan_speed){
					//ledTurnOn(LED_ETHER_STATUS);
					ledTurnOff(LED_ETHER_ACT_STATUS);
					ledTurnOn(LED_ETHER_NOACT_STATUS);
				}
			}
		}
	}
	if(ret == 0){ //no port conncect
		speed = 0;
		//ledTurnOff(LED_ETHER_STATUS);
		ledTurnOff(LED_ETHER_ACT_STATUS);	
		ledTurnOff(LED_ETHER_NOACT_STATUS);
	}
	ether_lan_speed = speed;
	return ret;
}
#endif

/* Monitor the status of link, re-do link initialization if necessary. */
static void tc3162_mac_link_status(unsigned long data)
{
	uint32 status = 0;
    macCtrlReg_t macCtrl;

#if defined(TCSUPPORT_ETHER_ALL_LED)
    static uint8 index = 0;
    if(index++ >= 1){ //3 500ms 1 1s
    index = 0;
#endif
#ifdef TCPHY_DEBUG
	 if (getTcPhyFlag()){	
		if( getTcPhyForceLinkFlag() == 1){
			  mac_p->macPhyLinkProfile_p->enetMode =  LAN_ST_CONNECTED | LAN_ST_LINK_UP ;
			  goto leave;
		}
	 }
#endif

#ifdef TCPHY_SUPPORT
	periodChk();
#endif
	
#ifdef  TC_CONSOLE_ENABLE
	tcconsole_chk();
#endif
	
#ifdef EEE_SUPPORT
	/*According the ethernet PHY link status to enable
	or disable eee function, if link is down to disable
	eee function,If the link is up, we need to wait 1 
	second then to enable psm function.*/
	chkLinkStatus();
#endif
	
	if (mac_p == NULL)
		goto leave;

	if( swicVendor ) {
		mac_p->macPhyLinkProfile_p->enetMode =  LAN_ST_100MB | LAN_ST_FULL_DUPLEX | LAN_ST_LINK_UP ;
	} else {
		mac_p->macPhyLinkProfile_p->enetMode = 0;
		status = miiStationRead(mac_p, PHY_CONTROL_REG);

		/* ----- Check reset bit & power down bit ----- */
		if ( status & (PHY_RESET | PHY_POWER_DOWN) ) 
		{
			goto leave;
		}
		#ifdef TCPHY_SUPPORT
			if (getTcPhyFlag())
			  	status = getTcPhyStatusReg(mac_p);
			else
		#endif	
		status = miiStationRead(mac_p, PHY_STATUS_REG);
		if ( status & PHY_AN_COMPLETE ) {
			if ( mac_p->macPhyLinkProfile_p->ANCompFlag != 1 ) {
				mac_p->macPhyLinkProfile_p->ANCompFlag = 1;
				miiStationWrite(mac_p, PHY_CONTROL_REG, PHY_AN_ENABLE);
			}
		}
		if ( status & PHY_LINK_STATUS ) {
			mac_p->macPhyLinkProfile_p->enetMode |= LAN_ST_LINK_UP;
			/* ----- Read remote capability register for link speed ----- */
		#ifdef TCPHY_SUPPORT
			if (getTcPhyFlag())
			  	status = getTcPhyRMCAPReg(mac_p);
			else
		#endif	
			status = miiStationRead(mac_p, PHY_REMOTE_CAP_REG);
			if ( status & PHY_100_BASE_TX_FD ) {
				mac_p->macPhyLinkProfile_p->enetMode |= (LAN_ST_FULL_DUPLEX | LAN_ST_100MB);
			} else if ( status & PHY_100_BASE_TX_HD ) {
				mac_p->macPhyLinkProfile_p->enetMode |= (LAN_ST_100MB);
			} else if ( status & PHY_10_BASE_T_FD ) {
				mac_p->macPhyLinkProfile_p->enetMode |= (LAN_ST_FULL_DUPLEX);
			} else if ( (status & PHY_10_BASE_T_HD) == 0 ) {
				mac_p->macPhyLinkProfile_p->enetMode = 0;
			}
		} /* if status & PHY_LINK_STATUS */
	}
	#ifdef TCPHY_DEBUG
	if(!getTcPhyLookbackFlag() && getTcPhyFlag()){
	#endif
		/* lino: according to enet phy link mode to adjust mac full duplex mode or not */
		macCtrl.value = VPint(CR_MAC_MACCR);
		macCtrl.bits.fulltx = (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_FULL_DUPLEX) ? 1 : 0;
		VPint(CR_MAC_MACCR) = macCtrl.value;
#ifdef TCPHY_DEBUG	
	}
#endif
#if defined(TCSUPPORT_ETHER_ALL_LED)
	}
	ether_states_handler();
#endif

leave:
	/* Schedule for the next time */
	#ifdef TC2031_SUPPORT
	eth_timer.expires = jiffies + (HZ / 2);
	#else
#if defined(TCSUPPORT_ETHER_ALL_LED)
	eth_timer.expires = jiffies + (HZ * 2);
#else
	eth_timer.expires = jiffies + (HZ);//(HZ * 2); 500ms HZ/2 1s HZ 2s HZ*2
#endif
	#endif
  	add_timer(&eth_timer);
	
}
#ifdef TCPHY_DEBUG
/*_____________________________________________________________________________
**      function name: macClearCnt
**      descriptions:
**           clear all mac counters 
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
void macClearCnt(void)
{

	uint8 *erase_p;
 
 	erase_p = (uint8 *)(&mac_p->macStat.MIB_II);

	memset(erase_p+4,0,sizeof(macMIB_II_t) - 4);
	memset(&mac_p->macStat.inSilicon, 0, sizeof(inSiliconStat_t));

}

/*_____________________________________________________________________________
**      function name: macDispCnt
**      descriptions:
**           display all mac counters 
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
void macDispCnt(void)
{

	printk("inOctets              = 0x%08lx, ", mac_p->macStat.MIB_II.inOctets);
	printk("inUnicastPkts         = 0x%08lx\n", mac_p->macStat.MIB_II.inUnicastPkts);
	printk("inMulticastPkts       = 0x%08lx, ", mac_p->macStat.MIB_II.inMulticastPkts);
	printk("inDiscards            = 0x%08lx\n", mac_p->macStat.MIB_II.inDiscards);
	printk("inErrors              = 0x%08lx, ", mac_p->macStat.MIB_II.inErrors);
	printk("outOctets             = 0x%08lx\n", mac_p->macStat.MIB_II.outOctets);
	printk("outUnicastPkts        = 0x%08lx, ", mac_p->macStat.MIB_II.outUnicastPkts);
	printk("outMulticastPkts      = 0x%08lx\n", mac_p->macStat.MIB_II.outMulticastPkts);
	printk("outDiscards           = 0x%08lx, ", mac_p->macStat.MIB_II.outDiscards);
	printk("outErrors             = 0x%08lx\n", mac_p->macStat.MIB_II.outErrors);
	printk("softRstCnt             = 0x00000000\n");//no counter in linux
	printk("\n[ inSilicon Statistics Display ]\n");
	printk("txJabberTimeCnt       = 0x%08lx  ", mac_p->macStat.inSilicon.txJabberTimeCnt);
	printk("txLossOfCarrierCnt    = 0x%08lx\n", mac_p->macStat.inSilicon.txLossOfCarrierCnt);
	printk("txNoCarrierCnt        = 0x%08lx  ", mac_p->macStat.inSilicon.txNoCarrierCnt);
	printk("txLateCollisionCnt    = 0x%08lx\n", mac_p->macStat.inSilicon.txLateCollisionCnt);
	printk("txExCollisionCnt      = 0x%08lx  ", mac_p->macStat.inSilicon.txExCollisionCnt);
	printk("txHeartbeatFailCnt    = 0x%08lx\n", mac_p->macStat.inSilicon.txHeartbeatFailCnt);
	printk("txCollisionCnt        = 0x%08lx  ", mac_p->macStat.inSilicon.txCollisionCnt);
	printk("txExDeferralCnt       = 0x%08lx\n", mac_p->macStat.inSilicon.txExDeferralCnt);
	printk("txUnderRunCnt         = 0x%08lx  ", mac_p->macStat.inSilicon.txUnderRunCnt);
	printk("rxAlignErr            = 0x%08lx\n", mac_p->macStat.inSilicon.rxAlignErr);
	printk("rxDribblingErr        = 0x%08lx  ", mac_p->macStat.inSilicon.rxDribblingErr);
	printk("rxSymbolErr           = 0x%08lx\n", mac_p->macStat.inSilicon.rxSymbolErr);
	printk("rxMiiErr              = 0x%08lx  ", mac_p->macStat.inSilicon.rxMiiErr);
	printk("rxCollisionErr        = 0x%08lx\n", mac_p->macStat.inSilicon.rxCollisionErr);
	printk("rxCrcErr              = 0x%08lx  ", mac_p->macStat.inSilicon.rxCrcErr);
	printk("rxEtherFrameLengthErr = 0x%08lx\n", mac_p->macStat.inSilicon.rxEtherFrameLengthErr);
	printk("rx802p3FrameLengthErr = 0x%08lx  ", mac_p->macStat.inSilicon.rx802p3FrameLengthErr);
	printk("rxPktIPChkSumErr      = 0x%08lx\n", mac_p->macStat.inSilicon.rxPktIPChkSumErr);
	printk("rxRuntErr             = 0x%08lx  ", mac_p->macStat.inSilicon.rxRuntErr);
	printk("rxLongErr             = 0x%08lx\n", mac_p->macStat.inSilicon.rxLongErr);
	printk("\n[ Extra information ]\n");
	printk("Rx Descriptor idx     = 0x%08lx  ", mac_p->rxCurrentDescp);
	printk("Tx Descriptor idx     = 0x%08lx\n", mac_p->txCurrentDescp);
	printk("Rx Enqueued cnt       = 0x%08lx  ", mac_p->macStat.inSilicon.rxEnQueueNum);
	printk("Tx Enqueued cnt       = 0x%08lx\n", mac_p->macStat.inSilicon.txEnQueueNum);
	printk("Rx Dequeued cnt       = 0x%08lx  ", mac_p->macStat.inSilicon.rxDeQueueNum);
	printk("Tx Dequeued cnt       = 0x%08lx\n", mac_p->macStat.inSilicon.txDeQueueNum);
	printk("Tx Buf UnReleased cnt = 0x%08lx  ", mac_p->txUnReleasedBufCnt);
	printk("Tx Buf UnReleased idx = 0x%08lx\n", mac_p->txUnReleasedDescp);

}
#endif
/* Starting up the ethernet device */
static int tc3162_mac_open(struct net_device *dev)
{
	int err;

  	printk(KERN_INFO "%s: starting interface.\n", dev->name);

	err = request_irq(dev->irq, tc3162_mac_isr, 0, dev->name, dev);
	if (err)
		return err;

  	macInit();

	#ifdef MII_INTERFACE
	/* MII setup */
	mac_p->mii_if.phy_id = mac_p->enetPhyAddr;
	mac_p->mii_if.full_duplex = 1;
	mac_p->mii_if.phy_id_mask = 0x1f;
	mac_p->mii_if.reg_num_mask = 0x1f;
	mac_p->mii_if.dev = dev;
	mac_p->mii_if.mdio_read = mdio_read;
	mac_p->mii_if.mdio_write = mdio_write;
	#endif
	
  	/* Schedule timer for monitoring link status */
  	init_timer(&eth_timer);
	#ifdef TC2031_SUPPORT
	eth_timer.expires = jiffies + (HZ/2);
	#else
	eth_timer.expires = jiffies + (HZ * 2);
	#endif
  	eth_timer.function = tc3162_mac_link_status;
  	eth_timer.data = (unsigned long)dev;
  	add_timer(&eth_timer);

  	/* Schedule timer for monitoring link status */
  	init_timer(&eth_poll_timer);
  	eth_poll_timer.expires = jiffies + (HZ * 2);
  	eth_poll_timer.function = tc3162_mac_poll;
  	eth_poll_timer.data = (unsigned long)dev;
  	add_timer(&eth_poll_timer);

	INIT_WORK(&tq_mac, tc3162_mac_poll_rcv);

  	netif_start_queue(dev);

	/*Wait 3 seconds,ipv6 can send initial packet,Added by jlliu_20091230*/
	pause(3000);
  	return 0;
}

/* Stopping the ethernet device */
static int tc3162_mac_close(struct net_device *dev)
{
	printk(KERN_INFO "tc3162_mac_close\n");

  	netif_stop_queue(dev);

	free_irq(dev->irq, dev);

  	/* Kill timer */
  	del_timer_sync(&eth_timer);
  	del_timer_sync(&eth_poll_timer);

  	return 0;
}

/* Setup multicast list */
static void tc3162_mac_set_multicast_list(struct net_device *dev)
{
	return; /* Do nothing */
}

/* Setting customized mac address */
static int tc3162_mac_set_macaddr(struct net_device *dev, void *p)
{
	struct sockaddr *addr = p;

	/* Check if given address is valid ethernet MAC address */
  	if (!is_valid_ether_addr(addr->sa_data))
    	return(-EIO);

	/* Save the customize mac address */
  	memcpy(dev->dev_addr, addr->sa_data, dev->addr_len);
  	memcpy(def_mac_addr, addr->sa_data, dev->addr_len);

	macGetMacAddr(mac_p);
  	macSetMacReg(mac_p);

	return 0;
}

/* Get the stats information */
static struct net_device_stats *tc3162_mac_stats(struct net_device *dev)
{
	struct net_device_stats *stats;

	stats = &mac_p->stats;

	stats->rx_packets = mac_p->macStat.MIB_II.inUnicastPkts +
		mac_p->macStat.MIB_II.inMulticastPkts;
	stats->tx_packets = mac_p->macStat.MIB_II.outUnicastPkts +
		mac_p->macStat.MIB_II.outMulticastPkts;
	stats->rx_bytes = mac_p->macStat.MIB_II.inOctets;
	stats->tx_bytes = mac_p->macStat.MIB_II.outOctets;
	stats->rx_dropped = mac_p->macStat.MIB_II.inDiscards;
	stats->tx_dropped = mac_p->macStat.MIB_II.outDiscards;
	stats->multicast = mac_p->macStat.MIB_II.inMulticastPkts;
	stats->rx_errors = mac_p->macStat.MIB_II.inErrors;
	stats->tx_errors = mac_p->macStat.MIB_II.outErrors;
	stats->collisions = mac_p->macStat.inSilicon.txExCollisionCnt + 
		mac_p->macStat.inSilicon.txCollisionCnt +
		mac_p->macStat.inSilicon.rxCollisionErr;

	return stats;
}

/* Handling ioctl call */
static int tc3162_mac_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	#ifdef MII_INTERFACE
	/*Add the mii read/write ioctl interface*/
	int rc;
	if (!netif_running(dev))
		return -EINVAL;
	
	spin_lock_irq(&mac_p->lock);
	rc = generic_mii_ioctl(&mac_p->mii_if, if_mii(ifr), cmd, NULL);
	spin_unlock_irq(&mac_p ->lock);
	return rc;
	#else
	/* Not implemented yet */
  	return 0;
	#endif
}

#if 0
//remove unuse function. chih_huang 20101129.
uint8 int_eth;

#endif
__IMEM irqreturn_t tc3162_mac_isr(int irq, void *dev_id)
{
	macIntrStatusReg_t isr;
#ifdef TC3162_MAC_NAPI  
	macIntrMaskReg_t macIntrMask;
#endif
	struct net_device *dev = (struct net_device *) dev_id;

#if 0
	//remove unuse function. chih_huang 20101129.
	int_eth = (VPint(CR_INTC_IMR) & (1 << MAC_INT));
	if (int_eth)
		VPint(CR_INTC_IMR) &= ~(1<<MAC_INT);

#endif
	// -------Read MACCR--------------------------------
  	isr.value = VPint(CR_MAC_ISR);

	#ifdef EEE_DEBUG
	dumpIRQEvent(isr.value);
	#endif
#if 0
	//if (DbgSettings & DBG_HW)
	if (1)
	{
		if (isr.bits.physts_chg ) printk("\r\n--->PHYSTS_CHG  ");
		if (isr.bits.ahb_err    ) printk("\r\n--->AHB_ERR     ");
		if (isr.bits.rpkt_lost  ) printk("\r\n--->RPKT_LOST   ");
    	//if (isr.bits.rpkt_save  ) printk("\r\n--->RPKT_SAVE   ");
		if (isr.bits.xpkt_lost  ) printk("\r\n--->XPKT_LOST   ");
	    //if (isr.bits.xpkt_finish) printk("\r\n--->XPKT_FINISH ");
	    //if (isr.bits.notxbuf    ) printk("\r\n--->NOTXBUF     ");
	    //if (isr.bits.xpkt_save  ) printk("\r\n--->XPKT_SAVE   ");
		if (isr.bits.norxbuf    ) printk("\r\n--->NORXBUF     ");
	    //if (isr.bits.rpkt_finish) printk("\r\n--->RPKT_FINISH ");
	}
#endif

	// ----------Packet Received----------------------

#ifdef TC3162_MAC_NAPI
	if (isr.bits.rpkt_finish || isr.bits.norxbuf || isr.bits.rpkt_lost)
#else	
	if (isr.bits.rpkt_finish)
#endif		
	{
    	#if !defined(TCSUPPORT_ETHER_ALL_LED)
    	ledTurnOn(LED_ETHER_ACT_STATUS);
		if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_100MB)
			ledTurnOn(LED_ETHER_100M_ACT_STATUS);
		else
			ledTurnOn(LED_ETHER_10M_ACT_STATUS);
		#endif
#ifdef TC3162_MAC_NAPI
		spin_lock(&mac_p->lock);
		if (netif_rx_schedule_prep(dev)) {
			macIntrMask.value = read_reg_word(CR_MAC_IMR);
			macIntrMask.bits.rpkt_finish = 0;
			macIntrMask.bits.norxbuf = 0;
			macIntrMask.bits.rpkt_lost = 0;
			write_reg_word(CR_MAC_IMR, macIntrMask.value);
			__netif_rx_schedule(dev);
		}

		spin_unlock(&mac_p->lock);
		if(isr.bits.norxbuf || isr.bits.rpkt_lost){
			mac_p->macStat.MIB_II.inDiscards++;
			mac_p->macStat.inSilicon.rxOverrunInt++;
		}	
#if 0			
		if(isr.bits.norxbuf)
			printk("MAC : NO RX BUF\n");
		if(isr.bits.rpkt_lost)
			printk("MAC : RX BUF OVERRUN\n");
#endif			
#else			
    	macRxRingProc(dev);
#endif		
	}

#if 0
	//remove unuse function. chih_huang 20101129.
	int_eth = (VPint(CR_INTC_IMR) & (1 << MAC_INT));
	if (int_eth)
		VPint(CR_INTC_IMR) |= (1<<MAC_INT);

#endif
    MAC_RX_POLL_DEMAND_(1);

	return IRQ_HANDLED;
}
#ifdef TC3162_MAC_NAPI
#if 0
//solve a lot of out-of-order packet. shnwind 20110323.
static inline int macRxRingChk(void)
{
	macRxDescr_t *rxDescrp;

	rxDescrp = ((macRxDescr_t*)mac_p->rxDescrRingBaseVAddr) + mac_p->rxCurrentDescp;
	if ((rxDescrp->rdes0.word & (1<<31)) == 0)
		return 1;
	return -1;
}
#endif
static int tc3162_mac_poll_napi(struct net_device *dev, int *budget)
{
	int rx_work_limit = min(dev->quota, *budget);
	int received = 0;
	int n, done;
	unsigned long flags;
	macIntrMaskReg_t macIntrMask;

restart_poll:
	n = macRxRingProc(dev, rx_work_limit);
	if (n) {
		received += n;
		rx_work_limit -= n;
		if (rx_work_limit <= 0) {
			done = 0;
			goto more_work;
		}
	}

	done = 1;

	spin_lock_irqsave(&mac_p->lock, flags);
    __netif_rx_complete(dev);
	macIntrMask.value = read_reg_word(CR_MAC_IMR);
	macIntrMask.bits.rpkt_finish = 1;
	macIntrMask.bits.norxbuf = 1;
	macIntrMask.bits.rpkt_lost = 1;
	write_reg_word(CR_MAC_IMR, macIntrMask.value);
#if 0
	//solve a lot of out-of-order packet. shnwind 20110323.
	/* check rotting packets */
	if ((macRxRingChk() >= 0) && netif_rx_reschedule(dev, 0)) {
		macIntrMask.value = read_reg_word(CR_MAC_IMR);
		macIntrMask.bits.rpkt_finish = 0;
		macIntrMask.bits.norxbuf = 0;
		macIntrMask.bits.rpkt_lost = 0;
		write_reg_word(CR_MAC_IMR, macIntrMask.value);

		spin_unlock_irqrestore(&mac_p->lock, flags);

		if (rx_work_limit <= 0) {
			done = 0;
			goto more_work;
		} else {
		    goto restart_poll;
		}
	}
#endif	
	spin_unlock_irqrestore(&mac_p->lock, flags);
more_work:
    *budget -= received;
    dev->quota -= received;
    return done ? 0 : 1;
}
#endif

static int tc3162_mac_start(struct net_device *dev)
{
	int i;

	uint8 *flash_mac_addr = (uint8 *)0xbfc0ff48;

	if( (flash_mac_addr[0] == 0) && (flash_mac_addr[1] == 0) && (flash_mac_addr[2] == 0) &&
	    (flash_mac_addr[3] == 0) && (flash_mac_addr[4] == 0) && (flash_mac_addr[5] == 0) )
		printk(KERN_INFO "The MAC address in flash is null!\n");	    
	else    
  		memcpy(def_mac_addr, flash_mac_addr, 6);  	
	
	mac_p = netdev_priv(dev);

	for (i = 0; i < 6; i++) {
		dev->dev_addr[i] = def_mac_addr[i];
	}

	spin_lock_init(&mac_p->lock);
	spin_lock_init(&mac_p->phy_lock);

  	/* Hook up with handlers */
  	dev->get_stats 			= tc3162_mac_stats;
  	dev->hard_start_xmit 	= tc3162_mac_tx;
  	dev->open 				= tc3162_mac_open;
  	dev->stop 				= tc3162_mac_close;
  	dev->set_multicast_list = tc3162_mac_set_multicast_list;
  	dev->do_ioctl 			= tc3162_mac_ioctl;
  	dev->set_mac_address 	= tc3162_mac_set_macaddr;
#ifdef TC3162_MAC_NAPI
	dev->poll               = tc3162_mac_poll_napi;
	dev->weight             = MAC_NAPI_WEIGHT;
#endif	

	printk(KERN_INFO
	       "%s: TC3162 Ethernet address: %02X:%02X:%02X:%02X:%02X:%02X\n",
	       dev->name, 
	       dev->dev_addr[0], dev->dev_addr[1], dev->dev_addr[2],
		   dev->dev_addr[3], dev->dev_addr[4], dev->dev_addr[5]);

	/*
  	dev->tx_queue_len = MAC_TXDESCP_NO; 
  	dev->flags &= ~IFF_MULTICAST;
  	dev->flags |= IFF_DEBUG;
	*/

	return 0;
}

#if defined(TCSUPPORT_ETHER_ALL_LED)
static int ether_speed_read_proc(char *page, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d %d %d %d\n", ether_speed[0], ether_speed[1], ether_speed[2], ether_speed[3]);

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

static int ether_speed_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	int len;
	char get_buf[32];
	
	/* do a range checking, don't overflow buffers in kernel modules */
	if(count > sizeof(get_buf)-1)
		len = sizeof(get_buf)-1;
	else
		len = count;


	/* use the copy_from_user function to copy buffer data to
	* to our get_buf */
	if(copy_from_user(get_buf, buffer, len)) {
		return -EFAULT;
	}
	
	/* zero terminate get_buf */
	get_buf[len]='\0';
	
	if (sscanf(get_buf, "%d %d %d %d %d\n", ether_speed, (ether_speed+1), (ether_speed+2), (ether_speed+3)) != 4) {
		printk("usage: <ether_speed [0 ~ 3]>\n");
	}
	return len;
}
#endif

static int __init tc3162_mac_init(void)
{
  	struct net_device *dev;
  	int err = 0;
	struct proc_dir_entry *eth_proc;
#ifndef TC2031_SUPPORT
	struct proc_dir_entry *port_reverse_proc, *stag_to_vtag_proc;
#endif

#ifdef TC3162_MAC_NAPI
	printk(KERN_INFO "TC3162 Ethernet Driver NAPI version 1.5\n");
#else	
  	printk(KERN_INFO "TC3162 Ethernet Driver version 1.5\n");
#endif	
	printk("\r\n%s\n", MODULE_VERSION_TC3162MAC);

	dev = alloc_etherdev(sizeof(macAdapter_t));
	if (!dev)
		return -ENOMEM;

	tc3162_mac_dev = dev;
	
	dev->irq = MAC_INT;

	mac_p = netdev_priv(dev);

  	dev->init = tc3162_mac_start;

	err = register_netdev(dev);
	if (err)
		return err;

#if defined(TCSUPPORT_ETHER_ALL_LED)
	/* all ethernet stats */
	eth_proc = create_proc_entry("tc3162/ether_speed", 0, NULL);
	eth_proc->read_proc = ether_speed_read_proc;
	eth_proc->write_proc = ether_speed_write_proc;
#endif
  	/* ethernet related stats */
	eth_proc = create_proc_entry("tc3162/eth_stats", 0, NULL);
	eth_proc->read_proc = eth_stats_read_proc;
	eth_proc->write_proc = eth_stats_write_proc;
#ifdef TCPHY_4PORT
	/* ESD patch */
	eth_proc = create_proc_entry("tc3162/mac_esd_check", 0, NULL);
	eth_proc->read_proc = eth_esd_read_proc;
	eth_proc->write_proc = eth_esd_write_proc;
#if !defined(TCSUPPORT_CT)
#if 1//def VPORT
	/*vport enable/disable control*/
	eth_proc = create_proc_entry("tc3162/vport_enable", 0, NULL);
	eth_proc->read_proc = vport_enable_read_proc;
	eth_proc->write_proc = vport_enable_write_proc;
#endif
#endif
#endif

  	create_proc_read_entry("tc3162/eth_link_st", 0, NULL, eth_link_st_proc, NULL);
  	create_proc_read_entry("tc3162/eth_reg", 0, NULL, eth_reg_dump_proc, NULL);
#ifndef TC2031_SUPPORT
	port_reverse_proc = create_proc_entry("tc3162/port_reverse", 0, NULL);
	port_reverse_proc->read_proc = port_reverse_read_proc;
	port_reverse_proc->write_proc = port_reverse_write_proc;
#if !defined(TCSUPPORT_CT)
	stag_to_vtag_proc = create_proc_entry("tc3162/stag_to_vtag", 0, NULL);
	stag_to_vtag_proc->read_proc = stag_to_vtag_read_proc;
	stag_to_vtag_proc->write_proc = stag_to_vtag_write_proc;
#endif
#endif
	skb_queue_head_init(&adsl_pkt_queue);
	INIT_WORK(&tq_adsl, macSendReal);

	#ifdef TCPHY_SUPPORT
#if !defined(TCSUPPORT_CT)
#ifndef TC2031_SUPPORT
	create_proc_read_entry("tc3162/eth_port_status", 0, NULL, eth_port_stat_read_proc, NULL);
#endif
#endif
	tcephydbgcmd();
	#ifdef TC2031_DEBUG
	tcethercmd(); 
	#endif
	#endif
	#ifdef TC_CONSOLE_ENABLE 
	create_tcconsole_proc();
	rcu_assign_pointer(send_uart_msg, uart_msg_to_tcconsole);
	#endif
  	return(0);
}

static void __exit tc3162_mac_exit(void)
{
	printk("tc3162_mac_exit.\n");

	remove_proc_entry("tc3162/eth_stats", 0);
   	remove_proc_entry("tc3162/eth_link_st", 0);
   	remove_proc_entry("tc3162/eth_reg", 0);
#if !defined(TCSUPPORT_CT)
#ifdef TCPHY_SUPPORT
	remove_proc_entry("tc3162/eth_port_status", 0);
#endif
#endif
#ifdef TCPHY_4PORT	
	remove_proc_entry("tc3162/mac_esd_check", 0);
#if !defined(TCSUPPORT_CT)
#if 1// def VPORT
	remove_proc_entry("tc3162/vport_enable", 0);
#endif
#endif
#endif	
#ifndef TC2031_SUPPORT
	remove_proc_entry("tc3162/port_reverse", 0);
#if !defined(TCSUPPORT_CT)
	remove_proc_entry("tc3162/stag_to_vtag", 0);
#endif
#endif
	#ifdef TCPHY_SUPPORT
	/*unregister ci command */
	cmd_unregister("tcephydbg");
	#ifdef TC2031_DEBUG
	cmd_unregister("ether");
	#endif
	#endif
	#ifdef TC_CONSOLE_ENABLE 
	delete_tcconsole_proc();
	rcu_assign_pointer(send_uart_msg, NULL);
	#endif
}

/* Register startup/shutdown routines */
module_init(tc3162_mac_init);
module_exit(tc3162_mac_exit);

