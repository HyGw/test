/*
** $Id: //BBN_Linux/Branch/Branch_for_MT7520_20120528/tclinux_phoenix/modules/private/tc3262/gmac_tc3262.c#3 $
*/
/************************************************************************
 *
 *	Copyright (C) 2008 Trendchip Technologies, Corp.
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
** $Log: gmac_tc3262.c,v $
** Revision 1.26  2011/11/11 07:32:20  shnwind
** reduce autobench test time
**
** Revision 1.25  2011/10/27 00:34:01  shnwind
** rt65168_autobench support
**
** Revision 1.24  2011/07/29 09:50:43  xyzhu_nj
** #11006: Merge China telecom branch to Main trunk.(phase 2)
** bftpd
**
** Revision 1.23  2011/07/15 01:13:41  lino
** do reset when tx/rx disabled
**
** Revision 1.22  2011/07/14 06:35:36  shnwind
** rt63260 auto bench disable software patch
**
** Revision 1.21  2011/07/14 02:51:51  shnwind
** fix bug : mac polling timer will die when status is RESET or POWERDOWN
**
** Revision 1.20  2011/06/30 12:07:13  lino
** hwnat enhance: IPv6 and QinQ support
**
** Revision 1.19  2011/06/23 06:04:52  shnwind
** fix bug : rt5392 throughput (1 wifi to 1 lan) problem
**
** Revision 1.18  2011/06/23 04:57:56  shnwind
** fix bug : rt5392 throughput (1 wifi to 1 lan) problem
**
** Revision 1.17  2011/06/16 09:14:10  lino
** add RT65168 support: turn off hwnat when disabled hwnat
**
** Revision 1.16  2011/06/10 07:54:42  lino
** add RT65168 support
**
** Revision 1.15  2011/06/09 08:17:26  lino
** add RT65168 support
**
** Revision 1.14  2011/06/08 10:02:37  lino
** add RT65168 support
**
** Revision 1.13  2011/06/03 02:49:55  lino
** add RT65168 support
**
** Revision 1.12  2011/06/03 02:33:09  lino
** add RT65168 support
**
** Revision 1.11  2011/05/20 09:52:48  tzzhang_nj
** [Enhancement]#9542:
**   Merge this feature to TC3182 related files.
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
** Revision 1.10  2011/03/31 14:15:46  shnwind
** solve tc3182 throughput issue
**
** Revision 1.9  2011/03/23 12:01:59  shnwind
** solve a lot of out-of-order packet
**
** Revision 1.8  2011/03/17 11:36:17  shnwind
** Add a switch to vlanpt disable
**
** Revision 1.7  2011/01/26 11:49:51  shnwind
** merge #8655 #8656
**
** Revision 1.6  2011/01/25 13:39:58  shnwind
** merge #8655
**
** Revision 1.5  2011/01/18 11:42:14  xyzhu_nj
** 8119
**
** Revision 1.4  2010/12/24 12:42:32  xyzhu_nj
** #8119:Add support port reverse function.
**
** Revision 1.3  2010/11/09 11:21:11  shnwind
** fix bug : vlan packet send error when bridge mode
**
** Revision 1.2  2010/10/21 09:52:49  shnwind
** Use special tag to replace vlan function in switch
**
** Revision 1.1.1.1  2010/09/30 21:14:54  josephxu
** modules/public, private
**
** Revision 1.8  2010/09/25 15:04:39  here
** [Ehancement]TCConsole to support capture the console log message and set command into cpe.
**
** Revision 1.7  2010/09/03 16:38:34  here
** [Ehance] TC3182 GMAC Driver is support TC-Console & WAN2LAN function.
**
** Revision 1.6  2010/09/02 07:27:23  here
** [Ehance] Support TC3162U/TC3182 Auto-Bench
**
** Revision 1.5  2010/08/26 06:29:51  lino
** 1. When detecting PHY change speed/mode, do GMAC software reset.
** 2. Adjust TX/RX ring size from 64/256 to 128/128, NAPI weight from 64 to 32.
** 3. Dump EEE register in eth_reg
** 4. Fixed compiler errors when enable TCPHY_DEBUG.
** 5. Disable flow control.
** 6. Can load GMAC1 driver when LDV or 216-pin package.
** 7. GMAC1 PHY address fixed to 9.
** 8. Skip 4port switch searching in GMAC1.
**
** Revision 1.4  2010/06/21 12:40:38  jlliu_nj
** update TC2205F/6F patch version to F8.0
**
** Revision 1.3  2010/06/15 10:55:17  lino
** add tc3182 support
**
** Revision 1.2  2010/06/05 12:03:55  lino
** don't use DMEM on TC3182 platform
**
** Revision 1.1  2010/05/20 06:57:23  here
** [Ehancemenet] Support the TC2206 swithc ic with TC3262 platform and sprate the switch ic into tcswitch.c files
**
** Revision 1.1.1.1  2010/04/09 09:34:42  feiyan
** New TC Linux Make Flow Trunk
**
** Revision 1.2  2010/01/14 08:50:34  shnwind
** add wmb() and use skb recycle
**
** Revision 1.1.1.1  2009/12/17 01:47:37  josephxu
** 20091217, from Hinchu ,with VoIP
**
 */

#define TC3262_GMAC_NAPI

#define TC3262_GMAC_SKB_RECYCLE

#define DRV_NAME	"tc3262gmac"
#ifdef TC3262_GMAC_NAPI
#define DRV_VERSION	"1.00-NAPI"
#else
#define DRV_VERSION	"1.00"
#endif
#define DRV_RELDATE	"28.Aug.2008"

static const char *const version =
    DRV_NAME ".c:v" DRV_VERSION " " DRV_RELDATE "\n";

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
#include <linux/if_vlan.h>
#include <asm/spram.h>
#include <asm/io.h>
#include <asm/tc3162/tc3162.h>
#include <asm/tc3162/ledcetrl.h>
#include <asm/tc3162/TCIfSetQuery_os.h>
#include "../../version/tcversion.h"

#if defined(TCSUPPORT_HWNAT)
#include <linux/pktflow.h>
#endif

#ifdef TCPHY_SUPPORT
#include <asm/tc3162/cmdparse.h>
#include "../tcphy/tcetherphy.h"
#include "../tcphy/tcswitch.h"
#endif
#ifdef TCSUPPORT_AUTOBENCH
#include "../auto_bench/autobench.h"
#endif

#include "../tcphy/tcconsole.h"

#include "tc3262gmac.h"

#ifdef TCSUPPORT_MT7530_SWITCH_API
#include "../tcphy/mtkswitch_api.h"
#include "../tcphy/mtkswitch_api_def.h"
//extern int macMT7530gswAPIDispatch(struct ifreq *ifr);
#endif

#define RX_BUF_LEN 			(2048 - NET_SKB_PAD - 64 - (sizeof(struct skb_shared_info)))
#define RX_MAX_PKT_LEN 		1536

#define SUPPORT_CLAUSE45_API

//#define USE_JIFFIES_TIMER
#define USE_GLOBAL_PHY_LOCK

//#undef CONFIG_TC3162_DMEM
//#define TEST_ENABLE_MT7530_STAG
//#define MT7530_VERIFY_DUMP_SKB

/************************************************************************
*                          C O N S T A N T S
*************************************************************************
*/
//#define LOOPBACK_SUPPORT
//#define RT62806_VERIFY

/* define loopback mode test */
#define LOOPBACK_TX			0x01
#define LOOPBACK_RX_DROP	0x02
#define LOOPBACK_RX_CHK		0x03
#define LOOPBACK_TX_QOS		0x04
#define LOOPBACK_MODE_MASK	0x0f
#define LOOPBACK_MODE(x)	((x) & 0x0f)
#define LOOPBACK_TX_IPCS	(1<<4)
#define LOOPBACK_TX_VLAN	(1<<5)
#define LOOPBACK_TX_RANDOM	(1<<6)
#define LOOPBACK_MSG		(1<<7)
#define LOOPBACK_PKT		(1<<11)

#define LAN_ST_100MB        0x01
#define LAN_ST_FULL_DUPLEX	0x02
#define LAN_ST_LINK_UP      0x04
#define LAN_ST_1000MB       0x08


/* ADMTEK6996M register */
#define ADM_PORT0_BASIC		0x01
#define ADM_PORT1_BASIC		0x03
#define ADM_PORT2_BASIC		0x05
#define ADM_PORT3_BASIC		0x07
#define ADM_PORT4_BASIC		0x08
#define ADM_PORT5_BASIC		0x09

#define ADM_CHIP_ID0		0xa0
#define ADM_CHIP_ID1		0xa1

#ifdef SUPPORT_CLAUSE45_API
#define NORMAL_READ					(1<<0)
#define POST_READ					(1<<1)
#endif

/************************************************************************
*                            M A C R O S
*************************************************************************
*/

/************************************************************************
*                         D A T A   T Y P E S
*************************************************************************
*/

/************************************************************************
*              F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
static irqreturn_t tc3262_gmac_isr(int irq, void *dev_id);
int tc3262_gmac_tx(struct sk_buff *skb, struct net_device *dev);
static void macDrvStart(void);
static void macDrvStop(void);
static void macRxDrvStart(void);
static void macRxDrvStop(void);
#ifdef LOOPBACK_SUPPORT
static void macPhyLoopback(macAdapter_t *mac_p, int enable);
static void dump_skb(struct sk_buff *skb);
#endif
#ifdef MT7530_VERIFY_DUMP_SKB
static void dump_skb(struct sk_buff *skb);
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
#endif

#ifdef USE_GLOBAL_PHY_LOCK
static void tcMiiStationWrite_nolock(uint32 enetPhyAddr, uint32 phyReg, uint32 miiData);
static uint32 tcMiiStationRead_nolock(uint32 enetPhyAddr, uint32 phyReg);
static uint32 gswPbusRead_nolock(uint32 pbus_addr);
static int gswPbusWrite_nolock(uint32 pbus_addr, uint32 pbus_data);
static uint32 gswPmiRead_nolock(uint32 phy_addr, uint32 phy_reg);
static uint32 gswPmiWrite_nolock(uint32 phy_addr, uint32 phy_reg, uint32 phy_data);
static int tcMiiExtStationFillAddr_ext_nolock(uint32 portAddr, uint32 devAddr, uint32 regAddr);
static void tcMiiExtStationWrite_ext_nolock(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint32 miiData);
static uint32 tcMiiExtStationRead_ext_nolock(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint8 op);
static int tcMiiExtStationFillAddr_nolock(uint32 portAddr, uint32 devAddr, uint32 regAddr);
static void tcMiiExtStationWrite_nolock(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint32 miiData);
static uint32 tcMiiExtStationRead_nolock(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint8 op);
#endif

/************************************************************************
*                        P U B L I C   D A T A
*************************************************************************
*/

/************************************************************************
*                      E X T E R N A L   D A T A
*************************************************************************
*/
#ifdef TCPHY_SUPPORT
extern uint8 getTcPhyFlag(void);
extern int tcPhySwPatch(void);
extern int tcPhyVerLookUp(macAdapter_t *mac_p);
extern int tcPhySwPatch(void);
extern uint32 getTcPhyStatusReg(macAdapter_t * mac_p);
extern int tcPhyInit(macAdapter_t * mac_p);
#if !defined(TCSUPPORT_CT)
extern int eth_port_stat_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data);
#endif
#endif
/************************************************************************
*                       P R I V A T E   D A T A
*************************************************************************
*/

/* Device data */
struct net_device *tc3262_gmac_dev;
/* Timer */
static struct timer_list eth_timer;
static struct timer_list eth_poll_timer;

#ifdef USE_GLOBAL_PHY_LOCK
/* global phy/pbus/pmi lock */
static spinlock_t global_phy_lock;
#else
/* phy lock */
static spinlock_t phy_lock;
/* frank added 20110802 */
static spinlock_t pbus_lock;
static spinlock_t pmi_lock;
#ifdef SUPPORT_CLAUSE45_API
static spinlock_t phy_lock_clause45;
#endif
#endif


macAdapter_t *mac_p = NULL;
static macPhyLinkProfile_t enetPhyLinkProfile;

static phyDeviceList_t phyDeviceList[] = {
	{ 0x0013, "LXT972" },
	{ 0x0022, "AC101L" },
	{ 0x0243, "IP101" },
	{ 0x8201, "RTL8201" },
	{ 0x001c, "RTL8212" },
	{ 0x03a2, "TC2031" },
	{ 0x004d, "AR8035" },
	{ 0x0007, "MT7530" },
};

static uint8 macInitialized = 0;
#ifdef LOOPBACK_SUPPORT
static uint16 macLoopback = 0;
static uint32 macRxLoopback;
#endif


uint8 def_mac_addr[] = {0x00, 0x00, 0xaa, 0xbb, 0xcc, 0xff};

uint8 swicVendor = 0;

static DEFINE_SPINLOCK(gimr_lock);

//#ifdef VPORT
//int vportMasko=0;
//#endif
#ifndef TC2031_SUPPORT
#if !defined(TCSUPPORT_CT)
extern int port_reverse_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data);
extern int port_reverse_write_proc(struct file *file, const char *buffer, unsigned long count, void *data);
extern int stag_to_vtag_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data);
extern int stag_to_vtag_write_proc(struct file *file, const char *buffer, unsigned long count, void *data);
#endif
#endif

#if 1// defined(WAN2LAN) || defined(VPORT)
/* ------ xyzhu_091105:special tag relation data start ---------- */
extern uint8 macSTagFlag;
/* ------ xyzhu_091105:special tag relation data end ----------- */
#endif
/************************************************************************
*        F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
#define read_reg_word(reg) 			VPint(reg)
#define write_reg_word(reg, wdata) 	VPint(reg)=wdata

#define pause(x)					mdelay(x)

static inline struct sk_buff *gmac_alloc_skb2k(void)
{
#ifdef TC3262_GMAC_SG_MODE
	return skbmgr_dev_alloc_skb2k();
#else
	return skbmgr_dev_alloc_skb2k();
	//return dev_alloc_skb(RX_BUF_LEN);
#endif
}

static inline struct sk_buff *gmac_alloc_skb128(void)
{
#ifdef TC3262_GMAC_SG_MODE
	return skbmgr_dev_alloc_skb128();
#else
	return skbmgr_dev_alloc_skb128();
	//return dev_alloc_skb(SG_MAX_PKT_LEN);
#endif
}

#ifdef USE_JIFFIES_TIMER
int startJiffiesTimer(unsigned long *start_jiffies_ptr, unsigned long *stop_jiffies_ptr, unsigned int ms_cnt)
{
	int jiffies_wrapped;

	*start_jiffies_ptr = jiffies;
	*stop_jiffies_ptr = *start_jiffies_ptr + msecs_to_jiffies(ms_cnt);

	jiffies_wrapped = (*stop_jiffies_ptr < *start_jiffies_ptr) ? 1 : 0;

	return jiffies_wrapped;
}

int checkJiffiesTimer(unsigned long start_jiffies, unsigned long stop_jiffies, int jiffies_wrapped)
{
	int timer_reached;

	if (jiffies_wrapped)
	{
		timer_reached = ((jiffies >= stop_jiffies) && (jiffies < start_jiffies)) ? 1 : 0;
	}
	else
	{
		timer_reached = (jiffies >= stop_jiffies) ? 1 : 0;
	}

	return (timer_reached);
}
#endif //#ifdef USE_JIFFIES_TIMER

void tcMiiStationWrite(uint32 enetPhyAddr, uint32 phyReg, uint32 miiData)
#ifdef USE_GLOBAL_PHY_LOCK
{
	spin_lock_bh(&global_phy_lock);
	tcMiiStationWrite_nolock(enetPhyAddr, phyReg, miiData);
	spin_unlock_bh(&global_phy_lock);
}

static void tcMiiStationWrite_nolock(uint32 enetPhyAddr, uint32 phyReg, uint32 miiData)
#endif
{
	uint32 reg;
#ifdef USE_JIFFIES_TIMER
	unsigned long start_jiffies, stop_jiffies;
	int jiffies_wrapped;
#else
	uint32 cnt=1000000;
#endif

#ifndef USE_GLOBAL_PHY_LOCK
	spin_lock_bh(&phy_lock);
#endif
	if (isTC3169) {
		reg = (miiData & PHYOR_DATA) | (phyReg << PHYOR_REG_SHIFT) | (enetPhyAddr << PHYOR_ADDR_SHIFT) | PHYOR_WRITE | PHYOR_START;
		write_reg_word (CR_GMAC_PHYOR, reg);

#ifdef USE_JIFFIES_TIMER
		jiffies_wrapped = startJiffiesTimer(&start_jiffies, &stop_jiffies, 5000);
#endif
		do {
			//pause(1);
			reg=read_reg_word (CR_GMAC_PHYOR);
#ifdef USE_JIFFIES_TIMER
			if (checkJiffiesTimer(start_jiffies, stop_jiffies, jiffies_wrapped))
#else
			if (--cnt == 0)
#endif
			{
				printk("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX !!!\n\n\n", __FUNCTION__, enetPhyAddr, phyReg);
				while (1);
			}

		} while (reg & PHYOR_START);
	} else {
		reg = (miiData & NEW_PHYOR_DATA) |
				(phyReg << NEW_PHYOR_REG_SHIFT) | (enetPhyAddr << NEW_PHYOR_ADDR_SHIFT) |
				(NEW_PHYOR_OP_WR << NEW_PHYOR_OP_SHIFT) | (NEW_PHYOR_ST_IEEE_OLD << NEW_PHYOR_ST_SHIFT) |
				NEW_PHYOR_BUSY;
		write_reg_word (CR_GMAC_PHYOR, reg);

#ifdef USE_JIFFIES_TIMER
		jiffies_wrapped = startJiffiesTimer(&start_jiffies, &stop_jiffies, 5000);
#endif
		do {
			//do not need pause here because throughput decrease much. shnwind 20101109.
			//pause(1);
			reg=read_reg_word (CR_GMAC_PHYOR);
#ifdef USE_JIFFIES_TIMER
			if (checkJiffiesTimer(start_jiffies, stop_jiffies, jiffies_wrapped))
#else
			if (--cnt == 0)
#endif
			{
				printk("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX !!!\n\n\n", __FUNCTION__, enetPhyAddr, phyReg);
				while (1);
			}
		} while (reg & NEW_PHYOR_BUSY);
	}
#ifndef USE_GLOBAL_PHY_LOCK
	spin_unlock_bh(&phy_lock);
#endif
//	if (cnt==0)
//		printk("EER: tcMiiStationWrite timeout!\n");
	#ifdef TCPHY_4PORT
	/*TC2206 switch IC can't be direct to do PHY reset, we must
	 * avoid ESD software patch be trigger.
	 */
	refillPhyDefVal(enetPhyAddr, phyReg, miiData);
	#endif
}
EXPORT_SYMBOL(tcMiiStationWrite);

uint32 tcMiiStationRead(uint32 enetPhyAddr, uint32 phyReg)
#ifdef USE_GLOBAL_PHY_LOCK
{
	uint32 return_value;
	spin_lock_bh(&global_phy_lock);
	return_value = tcMiiStationRead_nolock(enetPhyAddr, phyReg);
	spin_unlock_bh(&global_phy_lock);
	return return_value;
}

static uint32 tcMiiStationRead_nolock(uint32 enetPhyAddr, uint32 phyReg)
#endif
{
	uint32 reg;
#ifdef USE_JIFFIES_TIMER
	unsigned long start_jiffies, stop_jiffies;
	int jiffies_wrapped;
#else
	uint32 cnt=1000000;
#endif

#ifndef USE_GLOBAL_PHY_LOCK
	spin_lock_bh(&phy_lock);
#endif
	if (isTC3169) {
		reg = (phyReg << PHYOR_REG_SHIFT) | (enetPhyAddr << PHYOR_ADDR_SHIFT) | PHYOR_START;
		write_reg_word (CR_GMAC_PHYOR, reg);

#ifdef USE_JIFFIES_TIMER
		jiffies_wrapped = startJiffiesTimer(&start_jiffies, &stop_jiffies, 5000);
#endif
		do {
			//do not need pause here because throughput decrease much. shnwind 20101109.
			//pause(1);
			reg=read_reg_word (CR_GMAC_PHYOR);
#ifdef USE_JIFFIES_TIMER
			if (checkJiffiesTimer(start_jiffies, stop_jiffies, jiffies_wrapped))
#else
			if (--cnt == 0)
#endif
			{
				printk("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX !!!\n\n\n", __FUNCTION__, enetPhyAddr, phyReg);
				while (1);
			}
		} while (reg & PHYOR_START);

		reg = (reg & PHYOR_DATA);
	} else {
		reg = (phyReg << NEW_PHYOR_REG_SHIFT) | (enetPhyAddr << NEW_PHYOR_ADDR_SHIFT) |
				(NEW_PHYOR_OP_RD << NEW_PHYOR_OP_SHIFT) | (NEW_PHYOR_ST_IEEE_OLD << NEW_PHYOR_ST_SHIFT) |
				NEW_PHYOR_BUSY;
		write_reg_word (CR_GMAC_PHYOR, reg);

#ifdef USE_JIFFIES_TIMER
		jiffies_wrapped = startJiffiesTimer(&start_jiffies, &stop_jiffies, 5000);
#endif
		do {
			//pause(1);
			reg=read_reg_word (CR_GMAC_PHYOR);
#ifdef USE_JIFFIES_TIMER
			if (checkJiffiesTimer(start_jiffies, stop_jiffies, jiffies_wrapped))
#else
			if (--cnt == 0)
#endif
			{
				printk("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX !!!\n\n\n", __FUNCTION__, enetPhyAddr, phyReg);
				while (1);
			}
		} while (reg & NEW_PHYOR_BUSY);

		reg = (reg & NEW_PHYOR_DATA);
	}
#ifndef USE_GLOBAL_PHY_LOCK
	spin_unlock_bh(&phy_lock);
#endif

//	if (cnt == 0)
//		printk("EER: tcMiiStationRead timeout!\n");

    return reg;
}
EXPORT_SYMBOL(tcMiiStationRead);

uint32 gswPbusRead(uint32 pbus_addr)
#ifdef USE_GLOBAL_PHY_LOCK
{
	uint32 return_value;
	spin_lock_bh(&global_phy_lock);
	return_value = gswPbusRead_nolock(pbus_addr);
	spin_unlock_bh(&global_phy_lock);
	return return_value;
}

/* frank modify for rt62806 */
static uint32 gswPbusRead_nolock(uint32 pbus_addr)
#endif
{
	uint32 pbus_data;

	uint32 phyaddr;
	uint32 reg;
	uint32 value;

#ifndef USE_GLOBAL_PHY_LOCK
	spin_lock_bh(&pbus_lock);
#endif

	phyaddr = 31;

	// 1. write high-bit page address
	reg = 31;
	value = (pbus_addr >> 6);
#ifdef USE_GLOBAL_PHY_LOCK
  	tcMiiStationWrite_nolock(phyaddr, reg, value);
#else
  	tcMiiStationWrite(phyaddr, reg, value);
#endif
	//mdelay(5);
	//DBG_PRINTF("1. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//printk("1. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	// 2. read low DWord
	reg = (pbus_addr>>2) & 0x000f;
#ifdef USE_GLOBAL_PHY_LOCK
	value = tcMiiStationRead_nolock(phyaddr, reg);
#else
	value = tcMiiStationRead(phyaddr, reg);
#endif
	//mdelay(5);
	//DBG_PRINTF("2. miir phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//printk("2. miir phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	pbus_data = value;

	// 3. read high DWord
	reg = 16;
#ifdef USE_GLOBAL_PHY_LOCK
	value = tcMiiStationRead_nolock(phyaddr, reg);
#else
	value = tcMiiStationRead(phyaddr, reg);
#endif
	//mdelay(5);
	//DBG_PRINTF("3. miir phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//printk("3. miir phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	pbus_data = (pbus_data) | (value<<16);

	//DBG_PRINTF("# pbus read addr=0x%04x data=0x%04x_%04x\r\n", (pbus_addr&0xfffc), (pbus_data>>16), (pbus_data&0xffff));
	//printk("# pbus read addr=0x%04x data=0x%04x_%04x\r\n", (pbus_addr&0xfffc), (pbus_data>>16), (pbus_data&0xffff));
//	printk("gswPbusRead read data:\n");

//	printk("pbus_data: %x\n", pbus_data);

#ifndef USE_GLOBAL_PHY_LOCK
	spin_unlock_bh(&pbus_lock);
#endif
	return pbus_data;
} /* end frank modify for rt62806 */
EXPORT_SYMBOL(gswPbusRead);

int gswPbusWrite(uint32 pbus_addr, uint32 pbus_data)
#ifdef USE_GLOBAL_PHY_LOCK
{
	int return_value;
	spin_lock_bh(&global_phy_lock);
	return_value = gswPbusWrite_nolock(pbus_addr, pbus_data);
	spin_unlock_bh(&global_phy_lock);
	return return_value;
}

/* frank modify for rt62806 */
static int gswPbusWrite_nolock(uint32 pbus_addr, uint32 pbus_data)
#endif
{
	uint32 phyaddr;
	uint32 reg;
	uint32 value;

#ifndef USE_GLOBAL_PHY_LOCK
	spin_lock_bh(&pbus_lock);
#endif

	phyaddr = 31;

	// 1. write high-bit page address
	reg = 31;
	value = (pbus_addr >> 6);
#ifdef USE_GLOBAL_PHY_LOCK
	tcMiiStationWrite_nolock(phyaddr, reg, value);
#else
	tcMiiStationWrite(phyaddr, reg, value);
#endif
	//mdelay(5);

	//printk("1. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//DBG_PRINTF("1. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	// 2. write low DWord
	reg = (pbus_addr>>2) & 0x000f;
	value = pbus_data & 0xffff;
#ifdef USE_GLOBAL_PHY_LOCK
	tcMiiStationWrite_nolock(phyaddr, reg, value);
#else
	tcMiiStationWrite(phyaddr, reg, value);
#endif
	//mdelay(5);
	//printk("2. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//DBG_PRINTF("2. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	// 3. write high DWord
	reg = 16;
	value = (pbus_data>>16) & 0xffff;
#ifdef USE_GLOBAL_PHY_LOCK
	tcMiiStationWrite_nolock(phyaddr, reg, value);
#else
	tcMiiStationWrite(phyaddr, reg, value);
#endif
	//mdelay(5);
	//printk("3. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//DBG_PRINTF("3. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	//DBG_PRINTF("# pbus write addr=0x%04x data=0x%04x_%04x\r\n", (pbus_addr&0xfffc), (pbus_data>>16),(pbus_data&0xffff));
#ifndef USE_GLOBAL_PHY_LOCK
	spin_unlock_bh(&pbus_lock);
#endif
  	return 0;
} /* end frank modify for rt62806 */
EXPORT_SYMBOL(gswPbusWrite);

uint32 gswPmiRead(uint32 phy_addr, uint32 phy_reg)
#ifdef USE_GLOBAL_PHY_LOCK
{
	uint32 return_value;
	spin_lock_bh(&global_phy_lock);
	return_value = gswPmiRead_nolock(phy_addr, phy_reg);
	spin_unlock_bh(&global_phy_lock);
	return return_value;
}

// yh: rt62806 PMDIO read access
static uint32 gswPmiRead_nolock(uint32 phy_addr, uint32 phy_reg)
#endif
{
	uint32 pbus_addr;
	uint32 pbus_data;

	uint32 phy_data;
	uint32 phy_acs_st;
#ifdef USE_JIFFIES_TIMER
	unsigned long start_jiffies, stop_jiffies;
	int jiffies_wrapped;
#else
	uint32 max_wait_cnt = 1000000;
#endif

	pbus_addr = 0x701c;
	// b31    - phy access 1:start&busy, 0:complete&idle
	// b29:25 - mdio phy reg addr
	// b24:20 - mdio phy addr
	// b19:18 - 2'b01: write, 2'b10: read
	// b17:16 - start field, always 2'b01
	// b15:0  - data

	phy_addr = phy_addr & 0x1f;
	phy_reg  = phy_reg & 0x1f;

	// 1. write phy_addr & phy_reg
	pbus_data = 0x80090000; // read
	pbus_data = pbus_data | (phy_addr<<20);
	pbus_data = pbus_data | (phy_reg<<25);

#ifndef USE_GLOBAL_PHY_LOCK
	spin_lock_bh(&pmi_lock);
#endif

#ifdef USE_GLOBAL_PHY_LOCK
	gswPbusWrite_nolock(pbus_addr,pbus_data);
#else
	gswPbusWrite(pbus_addr,pbus_data);
#endif
//	printk(" pbus write addr=0x%04x data=0x%08x\r\n", pbus_addr, pbus_data);

	// 2. check phy_acs_st
#if 1
#ifdef USE_JIFFIES_TIMER
	jiffies_wrapped = startJiffiesTimer(&start_jiffies, &stop_jiffies, 5000);
//printk("[start] %uld, %uld\n", start_jiffies, stop_jiffies);
#endif
	phy_acs_st = 1;
	while (phy_acs_st) {
#ifdef USE_GLOBAL_PHY_LOCK
		pbus_data = gswPbusRead_nolock(pbus_addr);
#else
		pbus_data = gswPbusRead(pbus_addr);
#endif
//		printk(" pbus read addr=0x%04x data=0x%08x\r\n", pbus_addr, pbus_data);
		phy_acs_st = (pbus_data>>31) & 0x1;
#ifdef USE_JIFFIES_TIMER
		if (checkJiffiesTimer(start_jiffies, stop_jiffies, jiffies_wrapped))
#else
		if (--max_wait_cnt == 0)
#endif
		{
			printk("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX !!!\n\n\n", __FUNCTION__, phy_addr, phy_reg);
			while (1);
		}
	}
#endif

//printk("[stop] %uld\n", jiffies);
//if ((phy_addr == 4) && (phy_reg == 2)) printk("[pbus_data] = %04X\n", pbus_data);

#ifndef USE_GLOBAL_PHY_LOCK
	spin_unlock_bh(&pmi_lock);
#endif

	// 3. return data
	phy_data = pbus_data & 0xffff;

	return phy_data;
}
EXPORT_SYMBOL(gswPmiRead);

uint32 gswPmiWrite(uint32 phy_addr, uint32 phy_reg, uint32 phy_data)
#ifdef USE_GLOBAL_PHY_LOCK
{
	uint32 return_value;
	spin_lock_bh(&global_phy_lock);
	return_value = gswPmiWrite_nolock(phy_addr, phy_reg, phy_data);
	spin_unlock_bh(&global_phy_lock);
	return return_value;
}

// yh: rt62806 PMDIO read access
static uint32 gswPmiWrite_nolock(uint32 phy_addr, uint32 phy_reg, uint32 phy_data)
#endif
{
	uint32 pbus_addr;
	uint32 pbus_data;

	uint32 phy_acs_st;
#ifdef USE_JIFFIES_TIMER
	unsigned long start_jiffies, stop_jiffies;
	int jiffies_wrapped;
#else
	uint32 max_wait_cnt = 1000000;
#endif

	pbus_addr = 0x701c;
	// b31    - phy access 1:start&busy, 0:complete&idle
	// b29:25 - mdio phy reg addr
	// b24:20 - mdio phy addr
	// b19:18 - 2'b01: write, 2'b10: read
	// b17:16 - start field, always 2'b01
	// b15:0  - data

	phy_addr = phy_addr & 0x1f;
	phy_reg  = phy_reg & 0x1f;
	phy_data = phy_data & 0xffff;

	// 1. write phy_addr & phy_reg & phy_data
	pbus_data = 0x80050000; // write
	pbus_data = pbus_data | (phy_addr<<20);
	pbus_data = pbus_data | (phy_reg<<25);
	pbus_data = pbus_data | (phy_data);

#ifndef USE_GLOBAL_PHY_LOCK
	spin_lock_bh(&pmi_lock);
#endif

#ifdef USE_GLOBAL_PHY_LOCK
	gswPbusWrite_nolock(pbus_addr,pbus_data);
#else
	gswPbusWrite(pbus_addr,pbus_data);
#endif
//	printk(" pbus write addr=0x%04x data=0x%08x\r\n", pbus_addr, pbus_data);

	// 2. check phy_acs_st
#if 1
#ifdef USE_JIFFIES_TIMER
	jiffies_wrapped = startJiffiesTimer(&start_jiffies, &stop_jiffies, 5000);
#endif
	phy_acs_st = 1;
	while (phy_acs_st) {
#ifdef USE_GLOBAL_PHY_LOCK
		pbus_data = gswPbusRead_nolock(pbus_addr);
#else
		pbus_data = gswPbusRead(pbus_addr);
#endif
//		DBG_PRINTF(" pbus read addr=0x%04x data=0x%08x\r\n", pbus_addr, pbus_data);
		phy_acs_st = (pbus_data>>31) & 0x1;
#ifdef USE_JIFFIES_TIMER
		if (checkJiffiesTimer(start_jiffies, stop_jiffies, jiffies_wrapped))
#else
		if (--max_wait_cnt == 0)
#endif
		{
			printk("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX !!!\n\n\n", __FUNCTION__, phy_addr, phy_reg);
			while (1);
		}
	}
#endif

#ifndef USE_GLOBAL_PHY_LOCK
	spin_unlock_bh(&pmi_lock);
#endif

	// 3. complete
	return 0;
}
EXPORT_SYMBOL(gswPmiWrite);

void miiStationWrite(macAdapter_t *mac_p, uint32 phyReg, uint32 miiData)
{
	tcMiiStationWrite(mac_p->enetPhyAddr, phyReg, miiData);
}

uint32 miiStationRead(macAdapter_t *mac_p, uint32 phyReg)
{
	return tcMiiStationRead(mac_p->enetPhyAddr, phyReg);
}

void tcAdmMiiStationWrite(uint32 admReg, uint32 miiData)
{
	uint32 phyaddr;
	uint32 reg;

	phyaddr = admReg >> 5;
	reg = admReg & 0x1f;

	tcMiiStationWrite(phyaddr, reg, miiData);
}

uint32 tcAdmMiiStationRead(uint32 admReg)
{
	uint32 phyaddr;
	uint32 reg;

	phyaddr = admReg >> 5;
	reg = admReg & 0x1f;

	return tcMiiStationRead(phyaddr, reg);
}

static int mdio_read(struct net_device *dev, int phy_id, int reg_num)
{
	return tcMiiStationRead(phy_id, reg_num);
}

static void mdio_write(struct net_device *dev, int phy_id, int reg_num, int val)
{
	tcMiiStationWrite(phy_id, reg_num, val);
}

#ifdef SUPPORT_CLAUSE45_API

/*_____________________________________________________________________________
**      function name: tcMiiExtStationFillAddr_ext
**      descriptions:
**         Fill the address to prepare aceess the phy register.
**      parameters:
**            portAddr : Port Address
**            devAddr : Device Address
**            regAddr : Register Address
**
**      global:
**            None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      write_reg_word
**      read_reg_word
**      DEBUGMSG
**
**      revision:
**      1. WPLin 2012/08/01
**____________________________________________________________________________
*/
int tcMiiExtStationFillAddr_ext(uint32 portAddr, uint32 devAddr, uint32 regAddr)
#ifdef USE_GLOBAL_PHY_LOCK
{
	int return_value;
	spin_lock_bh(&global_phy_lock);
	return_value = tcMiiExtStationFillAddr_ext_nolock(portAddr, devAddr, regAddr);
	spin_unlock_bh(&global_phy_lock);
	return return_value;
}

static int tcMiiExtStationFillAddr_ext_nolock(uint32 portAddr, uint32 devAddr, uint32 regAddr)
#endif
{
	uint32 pbus_addr;
	uint32 pbus_data;
	uint32 phy_acs_st;
#ifdef USE_JIFFIES_TIMER
	unsigned long start_jiffies, stop_jiffies;
	int jiffies_wrapped;
#else
	uint32 max_wait_cnt = 1000000;
#endif

	pbus_addr = 0x701C;

	// 1. write phy_addr & phy_reg & phy_data
	// b31    - phy access 1:start&busy, 0:complete&idle
	// b29:25 - mdio phy reg addr
	// b24:20 - mdio phy addr
	// b19:18 - (Clause45) 2'b00:address, 2'b01:write, 2'b11:read, 2'b10:post_read_inc_addr
	// b17:16 - start field, 2'b00:Clause45, 2'b01:Clause22
	// b15:0  - data
	pbus_data = 0x80000000; // write
	pbus_data = pbus_data | ((portAddr & 0x1F) << 20);
	pbus_data = pbus_data | ((devAddr & 0x1F) << 25);
	pbus_data = pbus_data | (regAddr & 0xFFFF);
#ifdef USE_GLOBAL_PHY_LOCK
	gswPbusWrite_nolock(pbus_addr, pbus_data);
#else
	gswPbusWrite(pbus_addr, pbus_data);
#endif

	// 2. check phy_acs_st
#ifdef USE_JIFFIES_TIMER
	jiffies_wrapped = startJiffiesTimer(&start_jiffies, &stop_jiffies, 5000);
#endif
	phy_acs_st = 1;
	while (phy_acs_st) {
#ifdef USE_GLOBAL_PHY_LOCK
		pbus_data = gswPbusRead_nolock(pbus_addr);
#else
		pbus_data = gswPbusRead(pbus_addr);
#endif
		phy_acs_st = (pbus_data >> 31) & 0x1;
#ifdef USE_JIFFIES_TIMER
		if (checkJiffiesTimer(start_jiffies, stop_jiffies, jiffies_wrapped))
#else
		if (--max_wait_cnt == 0)
#endif
		{
			printk("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX/%lX !!!\n\n\n", __FUNCTION__, portAddr, devAddr, regAddr);
			while (1);
		}
	}

	// 3. complete
	return (0);
}/*end tcMiiExtStationFillAddr_ext*/
EXPORT_SYMBOL(tcMiiExtStationFillAddr_ext);

/*_____________________________________________________________________________
**      function name: tcMiiExtStationWrite_ext
**      descriptions:
**        Used 45.3 method to write the phy register
**      parameters:
**            portAddr : Port Address
**            devAddr : Device Address
**            regAddr : Register Address
**            miiData : Write Data
**
**      global:
**            None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      write_reg_word
**      read_reg_word
**      DEBUGMSG
**
**      revision:
**      1. WPLin 2012/08/01
**____________________________________________________________________________
*/
void tcMiiExtStationWrite_ext(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint32 miiData)
#ifdef USE_GLOBAL_PHY_LOCK
{
	spin_lock_bh(&global_phy_lock);
	tcMiiExtStationWrite_ext_nolock(portAddr, devAddr, regAddr, miiData);
	spin_unlock_bh(&global_phy_lock);
}

static void tcMiiExtStationWrite_ext_nolock(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint32 miiData)
#endif
{
	uint32 pbus_addr;
	uint32 pbus_data;
	uint32 phy_acs_st;
#ifdef USE_JIFFIES_TIMER
	unsigned long start_jiffies, stop_jiffies;
	int jiffies_wrapped;
#else
	uint32 max_wait_cnt = 1000000;
#endif
	portAddr &= 0x1F;
	devAddr &= 0x1F;
	regAddr &= 0xFFFF;

#ifndef USE_GLOBAL_PHY_LOCK
	spin_lock_bh(&phy_lock_clause45);
#endif
#ifdef USE_GLOBAL_PHY_LOCK
	tcMiiExtStationFillAddr_ext_nolock(portAddr, devAddr, regAddr);
#else
	tcMiiExtStationFillAddr_ext(portAddr, devAddr, regAddr);
#endif

	pbus_addr = 0x701C;

	// 1. write phy_addr & phy_reg & phy_data
	// b31    - phy access 1:start&busy, 0:complete&idle
	// b29:25 - mdio phy reg addr
	// b24:20 - mdio phy addr
	// b19:18 - (Clause45) 2'b00:address, 2'b01:write, 2'b11:read, 2'b10:post_read_inc_addr
	// b17:16 - start field, 2'b00:Clause45, 2'b01:Clause22
	// b15:0  - data
	pbus_data = 0x80040000; // write
	pbus_data = pbus_data | (portAddr << 20);
	pbus_data = pbus_data | (devAddr << 25);
	pbus_data = pbus_data | (miiData & 0xFFFF);
#ifdef USE_GLOBAL_PHY_LOCK
	gswPbusWrite_nolock(pbus_addr, pbus_data);
#else
	gswPbusWrite(pbus_addr, pbus_data);
#endif

	// 2. check phy_acs_st
#ifdef USE_JIFFIES_TIMER
	jiffies_wrapped = startJiffiesTimer(&start_jiffies, &stop_jiffies, 5000);
#endif
	phy_acs_st = 1;
	while (phy_acs_st) {
#ifdef USE_GLOBAL_PHY_LOCK
		pbus_data = gswPbusRead_nolock(pbus_addr);
#else
		pbus_data = gswPbusRead(pbus_addr);
#endif
		phy_acs_st = (pbus_data >> 31) & 0x1;
#ifdef USE_JIFFIES_TIMER
		if (checkJiffiesTimer(start_jiffies, stop_jiffies, jiffies_wrapped))
#else
		if (--max_wait_cnt == 0)
#endif
		{
			printk("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX/%lX !!!\n\n\n", __FUNCTION__, portAddr, devAddr, regAddr);
			while (1);
		}
	}

	// 3. complete
#ifndef USE_GLOBAL_PHY_LOCK
	spin_unlock_bh(&phy_lock_clause45);
#endif
}/*end tcMiiExtStationWrite_ext*/
EXPORT_SYMBOL(tcMiiExtStationWrite_ext);

/*_____________________________________________________________________________
**      function name: tcMiiExtStationRead_ext
**      descriptions:
**        Used 45.3 method to read the phy register
**      parameters:
**            portAddr : Port Address
**            devAddr : Device Address
**            regAddr : Register Address
**            op : 0:Normal read
**			 1:Post read
**
**
**      global:
**            None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      write_reg_word
**      read_reg_word
**      DEBUGMSG
**
**      revision:
**      1. WPLin 2012/08/01
**____________________________________________________________________________
*/
uint32 tcMiiExtStationRead_ext(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint8 op)
#ifdef USE_GLOBAL_PHY_LOCK
{
	uint32 return_value;
	spin_lock_bh(&global_phy_lock);
	return_value = tcMiiExtStationRead_ext_nolock(portAddr, devAddr, regAddr, op);
	spin_unlock_bh(&global_phy_lock);
	return return_value;
}

static uint32 tcMiiExtStationRead_ext_nolock(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint8 op)
#endif
{
	uint32 pbus_addr;
	uint32 pbus_data;
	uint32 phy_acs_st;
#ifdef USE_JIFFIES_TIMER
	unsigned long start_jiffies, stop_jiffies;
	int jiffies_wrapped;
#else
	uint32 max_wait_cnt = 1000000;
#endif

	portAddr &= 0x1F;
	devAddr &= 0x1F;
	regAddr &= 0xFFFF;

#ifndef USE_GLOBAL_PHY_LOCK
	spin_lock_bh(&phy_lock_clause45);
#endif

	if (op != POST_READ){
		/*POST READ command only to fill the address once, the address will be
		increased once automatically.*/
#ifdef USE_GLOBAL_PHY_LOCK
		tcMiiExtStationFillAddr_ext_nolock(portAddr, devAddr, regAddr);
#else
		tcMiiExtStationFillAddr_ext(portAddr, devAddr, regAddr);
#endif
	}

	pbus_addr = 0x701C;

	// 1. write phy_addr & phy_reg & phy_data
	// b31    - phy access 1:start&busy, 0:complete&idle
	// b29:25 - mdio phy reg addr
	// b24:20 - mdio phy addr
	// b19:18 - (Clause45) 2'b00:address, 2'b01:write, 2'b11:read, 2'b10:post_read_inc_addr
	// b17:16 - start field, 2'b00:Clause45, 2'b01:Clause22
	// b15:0  - data
	pbus_data = (op == POST_READ) ? 0x80080000 : 0x800C0000;
	pbus_data = pbus_data | ((portAddr & 0x1F) << 20);
	pbus_data = pbus_data | ((devAddr & 0x1F) << 25);
	pbus_data = pbus_data | (regAddr & 0xFFFF);
#ifdef USE_GLOBAL_PHY_LOCK
	gswPbusWrite_nolock(pbus_addr, pbus_data);
#else
	gswPbusWrite(pbus_addr, pbus_data);
#endif

	// 2. check phy_acs_st
#ifdef USE_JIFFIES_TIMER
	jiffies_wrapped = startJiffiesTimer(&start_jiffies, &stop_jiffies, 5000);
#endif
	phy_acs_st = 1;
	while (phy_acs_st) {
#ifdef USE_GLOBAL_PHY_LOCK
		pbus_data = gswPbusRead_nolock(pbus_addr);
#else
		pbus_data = gswPbusRead(pbus_addr);
#endif
		phy_acs_st = (pbus_data >> 31) & 0x1;
#ifdef USE_JIFFIES_TIMER
		if (checkJiffiesTimer(start_jiffies, stop_jiffies, jiffies_wrapped))
#else
		if (--max_wait_cnt == 0)
#endif
		{
			printk("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX/%lX !!!\n\n\n", __FUNCTION__, portAddr, devAddr, regAddr);
			while (1);
		}
	}

	// 3. return data
#ifndef USE_GLOBAL_PHY_LOCK
	spin_unlock_bh(&phy_lock_clause45);
#endif
	return (pbus_data & 0xFFFF);
}/*end tcMiiExtStationRead_ext*/
EXPORT_SYMBOL(tcMiiExtStationRead_ext);

void tcMiiStationWrite_ext(uint32 enetPhyAddr, uint32 phyReg, uint32 miiData)
{
	gswPmiWrite(enetPhyAddr, phyReg, miiData);
}
EXPORT_SYMBOL(tcMiiStationWrite_ext);

uint32 tcMiiStationRead_ext(uint32 enetPhyAddr, uint32 phyReg)
{
	return gswPmiRead(enetPhyAddr, phyReg);
}
EXPORT_SYMBOL(tcMiiStationRead_ext);

/*_____________________________________________________________________________
**      function name: tcMiiExtStationFillAddr
**      descriptions:
**         Fill the address to prepare aceess the phy register.
**      parameters:
**            portAddr : Port Address
**            devAddr : Device Address
**            regAddr : Register Address
**
**      global:
**            None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      write_reg_word
**      read_reg_word
**      DEBUGMSG
**
**      revision:
**      1. WPLin 2012/08/01
**____________________________________________________________________________
*/
int tcMiiExtStationFillAddr(uint32 portAddr, uint32 devAddr, uint32 regAddr)
#ifdef USE_GLOBAL_PHY_LOCK
{
	int return_value;
	spin_lock_bh(&global_phy_lock);
	return_value = tcMiiExtStationFillAddr_nolock(portAddr, devAddr, regAddr);
	spin_unlock_bh(&global_phy_lock);
	return return_value;
}

static int tcMiiExtStationFillAddr_nolock(uint32 portAddr, uint32 devAddr, uint32 regAddr)
#endif
{
#if defined(CONFIG_MIPS_RT65168)
	uint32 value;
#ifdef USE_JIFFIES_TIMER
	unsigned long start_jiffies, stop_jiffies;
	int jiffies_wrapped;
#else
	uint32 cnt = 1000000;
#endif

	if (isRT65168) {
#ifndef USE_GLOBAL_PHY_LOCK
		spin_lock_bh(&phy_lock);
#endif

		value = (regAddr & NEW_PHYOR_DATA) | (devAddr << NEW_PHYOR_REG_SHIFT) | (portAddr << NEW_PHYOR_ADDR_SHIFT) |
			(NEW_PHYOR_OP_EEE_FA << NEW_PHYOR_OP_SHIFT) | (NEW_PHYOR_ST_IEEE_NEW << NEW_PHYOR_ST_SHIFT) | NEW_PHYOR_BUSY;
		write_reg_word(CR_GMAC_PHYOR, value);

#ifdef USE_JIFFIES_TIMER
		jiffies_wrapped = startJiffiesTimer(&start_jiffies, &stop_jiffies, 5000);
#endif
		do {
			value=read_reg_word(CR_GMAC_PHYOR);
#ifdef USE_JIFFIES_TIMER
			if (checkJiffiesTimer(start_jiffies, stop_jiffies, jiffies_wrapped))
#else
			if (--cnt == 0)
#endif
			{
				printk("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX/%lX !!!\n\n\n", __FUNCTION__, portAddr, devAddr, regAddr);
				while (1);
			}
		} while (value & NEW_PHYOR_BUSY);
#ifndef USE_GLOBAL_PHY_LOCK
		spin_unlock_bh(&phy_lock);
#endif
	}
#endif

	return (0);
}/*end tcMiiExtStationFillAddr_ext*/
EXPORT_SYMBOL(tcMiiExtStationFillAddr);

/*_____________________________________________________________________________
**      function name: tcMiiExtStationWrite
**      descriptions:
**        Used 45.3 method to write the phy register
**      parameters:
**            portAddr : Port Address
**            devAddr : Device Address
**            regAddr : Register Address
**            miiData : Write Data
**
**      global:
**            None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      write_reg_word
**      read_reg_word
**      DEBUGMSG
**
**      revision:
**      1. WPLin 2012/08/01
**____________________________________________________________________________
*/
void tcMiiExtStationWrite(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint32 miiData)
#ifdef USE_GLOBAL_PHY_LOCK
{
	spin_lock_bh(&global_phy_lock);
	tcMiiExtStationWrite_nolock(portAddr, devAddr, regAddr, miiData);
	spin_unlock_bh(&global_phy_lock);
}

static void tcMiiExtStationWrite_nolock(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint32 miiData)
#endif
{
#if defined(CONFIG_MIPS_RT65168)
	uint32 value;
#ifdef USE_JIFFIES_TIMER
	unsigned long start_jiffies, stop_jiffies;
	int jiffies_wrapped;
#else
	uint32 cnt = 1000000;
#endif

	portAddr &= 0x1F;
	devAddr &= 0x1F;
	regAddr &= 0xFFFF;

#ifndef USE_GLOBAL_PHY_LOCK
	spin_lock_bh(&phy_lock_clause45);
#endif

#ifdef USE_GLOBAL_PHY_LOCK
	tcMiiExtStationFillAddr_nolock(portAddr, devAddr, regAddr);
#else
	tcMiiExtStationFillAddr(portAddr, devAddr, regAddr);
#endif

	if (isRT65168) {
#ifndef USE_GLOBAL_PHY_LOCK
		spin_lock_bh(&phy_lock);
#endif
		value = (miiData & NEW_PHYOR_DATA) | (devAddr << NEW_PHYOR_REG_SHIFT) | (portAddr << NEW_PHYOR_ADDR_SHIFT) |
			(NEW_PHYOR_OP_EEE_WR << NEW_PHYOR_OP_SHIFT) | (NEW_PHYOR_ST_IEEE_NEW << NEW_PHYOR_ST_SHIFT) | NEW_PHYOR_BUSY;
		write_reg_word(CR_GMAC_PHYOR, value);

#ifdef USE_JIFFIES_TIMER
		jiffies_wrapped = startJiffiesTimer(&start_jiffies, &stop_jiffies, 5000);
#endif
		do {
			value=read_reg_word(CR_GMAC_PHYOR);
#ifdef USE_JIFFIES_TIMER
			if (checkJiffiesTimer(start_jiffies, stop_jiffies, jiffies_wrapped))
#else
			if (--cnt == 0)
#endif
			{
				printk("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX/%lX !!!\n\n\n", __FUNCTION__, portAddr, devAddr, regAddr);
				while (1);
			}
		} while (value & NEW_PHYOR_BUSY);
#ifndef USE_GLOBAL_PHY_LOCK
		spin_unlock_bh(&phy_lock);
#endif
	}

#ifndef USE_GLOBAL_PHY_LOCK
	spin_unlock_bh(&phy_lock_clause45);
#endif
#endif
}/*end tcMiiExtStationWrite*/
EXPORT_SYMBOL(tcMiiExtStationWrite);

/*_____________________________________________________________________________
**      function name: tcMiiExtStationRead
**      descriptions:
**        Used 45.3 method to read the phy register
**      parameters:
**            portAddr : Port Address
**            devAddr : Device Address
**            regAddr : Register Address
**            op : 0:Normal read
**			 1:Post read
**
**
**      global:
**            None
**
**      return:
**            Success:        0
**            Otherwise:     -1
**
**      call:
**      write_reg_word
**      read_reg_word
**      DEBUGMSG
**
**      revision:
**      1. WPLin 2012/08/01
**____________________________________________________________________________
*/
uint32 tcMiiExtStationRead(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint8 op)
#ifdef USE_GLOBAL_PHY_LOCK
{
	uint32 return_value;
	spin_lock_bh(&global_phy_lock);
	return_value = tcMiiExtStationRead_nolock(portAddr, devAddr, regAddr, op);
	spin_unlock_bh(&global_phy_lock);
	return return_value;
}

static uint32 tcMiiExtStationRead_nolock(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint8 op)
#endif
{
	uint32 value = 0;
#if defined(CONFIG_MIPS_RT65168)
#ifdef USE_JIFFIES_TIMER
	unsigned long start_jiffies, stop_jiffies;
	int jiffies_wrapped;
#else
	uint32 cnt = 1000000;
#endif

	portAddr &= 0x1F;
	devAddr &= 0x1F;
	regAddr &= 0xFFFF;

#ifndef USE_GLOBAL_PHY_LOCK
	spin_lock_bh(&phy_lock_clause45);
#endif

	if (op != POST_READ){
		/*POST READ command only to fill the address once, the address will be
		increased once automatically.*/
#ifdef USE_GLOBAL_PHY_LOCK
		tcMiiExtStationFillAddr_nolock(portAddr, devAddr, regAddr);
#else
		tcMiiExtStationFillAddr(portAddr, devAddr, regAddr);
#endif
	}

	if (isRT65168) {
#ifndef USE_GLOBAL_PHY_LOCK
		spin_lock_bh(&phy_lock);
#endif

		value = (devAddr << NEW_PHYOR_REG_SHIFT) | (portAddr << NEW_PHYOR_ADDR_SHIFT) |
			(NEW_PHYOR_ST_IEEE_NEW << NEW_PHYOR_ST_SHIFT) | NEW_PHYOR_BUSY;
		value |= (op == POST_READ) ? (NEW_PHYOR_OP_EEE_POST_RD << NEW_PHYOR_OP_SHIFT) : (NEW_PHYOR_OP_EEE_RD << NEW_PHYOR_OP_SHIFT);
		write_reg_word(CR_GMAC_PHYOR, value);

#ifdef USE_JIFFIES_TIMER
		jiffies_wrapped = startJiffiesTimer(&start_jiffies, &stop_jiffies, 5000);
#endif
		do {
			value=read_reg_word(CR_GMAC_PHYOR);
#ifdef USE_JIFFIES_TIMER
			if (checkJiffiesTimer(start_jiffies, stop_jiffies, jiffies_wrapped))
#else
			if (--cnt == 0)
#endif
			{
				printk("\n\n\n!!! %s hang : wait busy bit timeout for %lX/%lX/%lX !!!\n\n\n", __FUNCTION__, portAddr, devAddr, regAddr);
				while (1);
			}
		} while (value & NEW_PHYOR_BUSY);
#ifndef USE_GLOBAL_PHY_LOCK
		spin_unlock_bh(&phy_lock);
#endif
	}

#ifndef USE_GLOBAL_PHY_LOCK
	spin_unlock_bh(&phy_lock_clause45);
#endif
#endif
	return (value & 0xFFFF);
}/*end tcMiiExtStationRead*/
EXPORT_SYMBOL(tcMiiExtStationRead);

#endif 	// Clause 45

void macIMRMask(void)
{
	uint32 reg;

	reg = GIMR_RX_DONE | GIMR_RX_FIFO_OVR | GIMR_DMA_ERR |
		GIMR_TX_OWNER_ERR | GIMR_RX_OWNER_ERR | GIMR_RX_RING_FULL;
	write_reg_word(CR_GMAC_GIMR, reg);
}

void macReset(void)
{
	uint32 reg;

	/* Disable DMA Enable */
	reg = 0;
	write_reg_word (CR_GMAC_GCTR, reg);
	pause(2);

	/* Reset Mac */
	reg = GCTR_HW_RESET;
	write_reg_word (CR_GMAC_GCTR, reg);
	pause(10);
}

void macSetIntConf(void)
{
	uint32 reg = 0;

#ifdef TC3262_GMAC_NAPI
	/* 1 packets to interrupt */
	reg = (1<<GPRCTR_RX_PKT_DONE_SHIFT) | 4;
#else
	/* 4 packets to interrupt */
	reg = (4<<GPRCTR_RX_PKT_DONE_SHIFT) | 4;
#endif
	write_reg_word(CR_GMAC_GPRCTR, reg);

	/* RX 32us timeout to interrupt */
	reg = 0x320;
	write_reg_word(CR_GMAC_GRIETR, reg);

	/* TX 128us timeout to interrupt */
	reg = 0xc80;
	write_reg_word(CR_GMAC_GTIETR, reg);
}

// Assign Tx Rx Descriptor Control Registers
void macSetDMADescrCtrlReg(macAdapter_t *mac_p)
{
	uint32 txq;

  	write_reg_word(CR_GMAC_GTRBR, K1_TO_PHY(mac_p->txDescrRingBaseVAddr[0]));
  	write_reg_word(CR_GMAC_GRRBR, K1_TO_PHY(mac_p->rxDescrRingBaseVAddr));

  	write_reg_word(CR_GMAC_GTRS01R, (mac_p->txRingSize<<16)|mac_p->txRingSize);
  	write_reg_word(CR_GMAC_GTRS23R, (mac_p->txRingSize<<16)|mac_p->txRingSize);
  	write_reg_word(CR_GMAC_GTRS45R, (mac_p->txRingSize<<16)|mac_p->txRingSize);
  	write_reg_word(CR_GMAC_GTRS67R, (mac_p->txRingSize<<16)|mac_p->txRingSize);
  	write_reg_word(CR_GMAC_GRRSR, mac_p->rxRingSize);

	/* write tx/rx descriptor size in unit of word */
	write_reg_word(CR_GMAC_GRDLR, mac_p->rxDescrSize>>2);
	write_reg_word(CR_GMAC_GTDLR, mac_p->txDescrSize>>2);

	write_reg_word(CR_GMAC_GRTPR, 0x8000);
	for (txq = 0; txq < TX_QUEUE_NUM; txq++)
		write_reg_word(CR_GMAC_GTTPR(txq), 0);
}

void macSetGSW(macAdapter_t *mac_p)
{
//	uint32 reg, value;

	if (swicVendor == SWIC_MT7530)
	{
		// Currentlt RT65168 always connect MT7530 with external interface, so use gswPbusWrite

		gswPbusWrite(0x7018, 0x007F8600);	// PHY_POLL : sw patch due to E1 ASIC h/w bug, shall always enable auto polling //V1.16 disable auto polling

		// adjust E1 chip timing settings, shall modify h/w default in E2 chip
		gswPbusWrite(0x702C, 0x00000202);	// P5RGMIICR : 0x0402 for port5 in 100M; 0x0202 for port5 in 1G
		gswPbusWrite(0x7240, 0x400200AA);	// TRGMII_TXCTRL
		gswPbusWrite(0x7250, 0x00000800);	// TRGMII_TD0_CTRL
		gswPbusWrite(0x7254, 0x000000FF);	// TRGMII_TD0_ODT
		gswPbusWrite(0x7258, 0x00000800);	// TRGMII_TD1_CTRL
		gswPbusWrite(0x725C, 0x000000FF);	// TRGMII_TD1_ODT
		gswPbusWrite(0x7260, 0x00000800);	// TRGMII_TD2_CTRL
		gswPbusWrite(0x7264, 0x000000FF);	// TRGMII_TD2_ODT
		gswPbusWrite(0x7268, 0x00000800);	// TRGMII_TD3_CTRL
		gswPbusWrite(0x726C, 0x000000FF);	// TRGMII_TD3_ODT
		gswPbusWrite(0x7270, 0x00000800);	// TRGMII_TXCTL_CTRL
		gswPbusWrite(0x727C, 0x000000FF);	// TRGMII_TCK_ODT

		// temp workaround for avoiding MT7530 7 seconds auto-powerdown on RT65168 board
		gswPbusWrite(0x30F0, 0x00001E02);	// CKGCR : clear bit 0
	}
}

void macSetMACCR(macAdapter_t *mac_p)
{
	uint32 reg;

	reg = (GCTR_RX_MAX_LEN_1536 << GCTR_RX_MAX_LEN_SHIFT) | GCTR_TX_FCS | GCTR_FULL_DUPLEX |
		GCTR_TX_PADDING | GCTR_DROP_COL | GCTR_RX_LONG |
		GCTR_RX_ALL | GCTR_RX_BCAST | GCTR_RX_MCAST;
#if !defined(TCSUPPORT_HWNAT)
	reg |= GCTR_RX_CRC_DIS | GCTR_RX_RUNT;
#endif
	write_reg_word(CR_GMAC_GCTR, reg);


	reg = GSPR_CFG_SPD_100M | (3 << GSPR_MTCC_LMT_SHIFT) |
		(7 << GSPR_CFG_CR_IFG_SHIFT);

	if(isTC3182 || isRT65168){
		reg |= GSPR_FC_EN;
	}
	write_reg_word(CR_GMAC_GSPR, reg);

	/* set TX under run register to 1.5KB */
	write_reg_word(CR_GMAC_GTLWR, 0x180);

	/* set WRR weight and unit is packet */
	write_reg_word(CR_GMAC_GWWR0, 0x1);
	write_reg_word(CR_GMAC_GWWR1, 0x2);
	write_reg_word(CR_GMAC_GWWR2, 0x4);
	write_reg_word(CR_GMAC_GWWR3, 0x8);

	write_reg_word(CR_GMAC_GTQCR, 0x50);

	write_reg_word(CR_GMAC_GRBSR, RX_MAX_PKT_LEN);
#ifdef TC3262_GMAC_SG_MODE
	write_reg_word(CR_GMAC_GSGCR, GSGCR_SG_EN | (SG_MAX_PKT_LEN & GSGCR_SG_PKT_LEN_MASK));
#endif
}

void macSetMacReg(macAdapter_t *mac_p)
{
	write_reg_word(CR_GMAC_GMAR, mac_p->macAddr[0]<<24 | mac_p->macAddr[1]<<16 | \
                               mac_p->macAddr[2]<<8  | mac_p->macAddr[3]<<0);
	write_reg_word(CR_GMAC_GMAR1, mac_p->macAddr[4]<<8  | mac_p->macAddr[5]<<0);
}

macTxDescr_t *macTxRingProc(macAdapter_t *mac_p, uint32 txq)
{
	volatile macTxDescr_t *pTxDescp;
	unsigned long flags;
	struct sk_buff *freeskb;

	spin_lock_irqsave(&mac_p->lock, flags);
	pTxDescp = ((macTxDescr_t*)mac_p->txDescrRingBaseVAddr[txq]) + mac_p->txUnReleasedDescp[txq];

  	while (mac_p->txUnReleasedBufCnt[txq] != 0) {
	  	if (pTxDescp->tdes0.bits.owner) {
			spin_unlock_irqrestore(&mac_p->lock, flags);
			return 0;
		}

	  	if (pTxDescp->tdes0.bits.ur_abort || pTxDescp->tdes0.bits.txpkt_ur) {
			printk("ERR TX tx curr=%ld tx=%08lx\n", mac_p->txUnReleasedDescp[txq], (uint32) pTxDescp);
			printk(" tdes0=%08lx\n", pTxDescp->tdes0.word);
			printk(" tdes1=%08lx\n", pTxDescp->tdes1.word);
			printk(" tdes2=%08lx\n", pTxDescp->tdes2.word);
			printk(" tdes3=%08lx\n", pTxDescp->tdes3.word);
		}

		if (mac_p->statisticOn) {
			if ((pTxDescp->tdes0.word) & (1<<3|1<<2))
				mac_p->macStat.inSilicon.txUnderRunCnt++;
		}

		freeskb = mac_p->txskbs[txq][mac_p->txUnReleasedDescp[txq]];
#ifdef LOOPBACK_SUPPORT
		if (LOOPBACK_MODE(macLoopback))
			dev_kfree_skb(freeskb);
		else
#endif
			dev_kfree_skb_any(freeskb);

		pTxDescp->tdes2.txbuf_addr = 0;
		mac_p->txskbs[txq][mac_p->txUnReleasedDescp[txq]] = NULL;

		if (mac_p->txUnReleasedDescp[txq] == (mac_p->txRingSize - 1))
			mac_p->txUnReleasedDescp[txq] = 0;
		else
			mac_p->txUnReleasedDescp[txq]++;
		mac_p->txUnReleasedBufCnt[txq]--;

		pTxDescp = ((macTxDescr_t*)mac_p->txDescrRingBaseVAddr[txq]) + mac_p->txUnReleasedDescp[txq];

    	mac_p->macStat.inSilicon.txDeQueueNum++;
	}
	spin_unlock_irqrestore(&mac_p->lock, flags);

	return (macTxDescr_t*) pTxDescp;
}

int tc3262_gmac_tx(struct sk_buff *skb, struct net_device *dev)
{
	volatile macTxDescr_t *currDescrp = NULL;
	uint32 length = skb->len;
	uint8 *bufAddrp = skb->data;
	unsigned long flags;
	uint32 reg;
	uint32 txq = skb->priority & GMAC_PRIORITY_MASK;

	if (unlikely(skb->len < ETH_ZLEN)) {
		if (skb_padto(skb, ETH_ZLEN)) {
			mac_p->macStat.MIB_II.outDiscards++;
			return NETDEV_TX_OK;
		}
		length = ETH_ZLEN;
	}

#ifdef CONFIG_TC3162_ADSL
	if(isTCConsolePkt(skb)){
		if(swicVendor == SWIC_TC2206){
			macTC2206STagInsert(skb);
		}
		else if(swicVendor ==SWIC_IP175C){
			macIpSTagInsert(skb);
		}
		/* frank added 20110628 */
		#if defined(RT62806_VERIFY)
		else if(swicVendor == SWIC_RT62806) {
			skb = macRT62806STagInsert(skb);
			if (skb == NULL) {
				mac_p->macStat.MIB_II.outDiscards++;
				return NETDEV_TX_OK;
			}
		} /* end frank added 20110628 */
		#endif
		#if defined(TEST_ENABLE_MT7530_STAG)
		/* wplin added 20120712 */
		else if(swicVendor == SWIC_MT7530) {
			skb = macMT7530STagInsert(skb);
			if (skb == NULL) {
				mac_p->macStat.MIB_II.outDiscards++;
				return NETDEV_TX_OK;
			}
		} /* end wlin added 20120712 */
		length = ((ETH_ZLEN < skb->len) ? skb->len : ETH_ZLEN);
		#endif
	}
#endif
#if !defined(TCSUPPORT_CT)
#if 1//def VPORT
	else if(!(skb->mark & SKBUF_COPYTOLAN) && macSTagFlag){
		//prevent modify clone skbuff data.
		skb = skb_unshare(skb ,GFP_ATOMIC);
		if (skb == NULL){
			mac_p->macStat.MIB_II.outDiscards++;
			return 0;
		}
		/*Normal packet is inclued the vlan header, and also the
		wan2lan packet is not inclued the vlan header.*/
		//vportMasko=(1<<((uint8)skb->data[15]-BASE_VID_IDX));
		/*Remove the vlan tag*/
		//memmove(skb->data+VLAN_HLEN, skb->data, 12);
		//skb_pull(skb, VLAN_HLEN);
		skb->mark|=SKBUF_VLAN;
	}
#endif
#endif
#if 1// defined(WAN2LAN) || defined(VPORT)
	if ((swicVendor == SWIC_IP175C) && macSTagFlag && !(skb->mark & SKBUF_TCCONSOLE))
	{
			macIpSTagInsert(skb);
			length = ((ETH_ZLEN < skb->len) ? skb->len : ETH_ZLEN);
	}

	if((swicVendor==SWIC_TC2206)&&macSTagFlag && !(skb->mark & SKBUF_TCCONSOLE)){
		macTC2206STagInsert(skb);
		length = ((ETH_ZLEN < skb->len) ? skb->len : ETH_ZLEN);
	}
	#if defined(RT62806_VERIFY)
	/* frank added 20110628 */
	if((swicVendor==SWIC_RT62806) && macSTagFlag && !(skb->mark & SKBUF_TCCONSOLE)){
		#if 0
		skb = macRT62806STagInsert(skb);
		if (skb == NULL) {
			mac_p->macStat.MIB_II.outDiscards++;
			return NETDEV_TX_OK;
		}
		length = ((ETH_ZLEN < skb->len) ? skb->len : ETH_ZLEN);
		#endif
	} /* end frank added 20110628 */
	#endif
	#if defined(TEST_ENABLE_MT7530_STAG)
	/* wplin added 20120712 */
	if((swicVendor==SWIC_MT7530) && macSTagFlag && !(skb->mark & SKBUF_TCCONSOLE)){
		skb = macMT7530STagInsert(skb);
		if (skb == NULL) {
			mac_p->macStat.MIB_II.outDiscards++;
			return NETDEV_TX_OK;
		}
		length = ((ETH_ZLEN < skb->len) ? skb->len : ETH_ZLEN);
	} /* end wplin added 20120712 */
	#endif
#endif
#ifdef LOOPBACK_SUPPORT
	if (macLoopback & LOOPBACK_PKT) {
		printk("TX: ");
		dump_skb(skb);
	}

	if (macLoopback & LOOPBACK_TX_RANDOM)
		txq = random32() & GMAC_PRIORITY_MASK;
#endif

#if defined(TCSUPPORT_HWNAT)
	if (pktflow_tx_hook)
		pktflow_tx_hook(skb, 0);
#endif

	if (mac_p->txUnReleasedBufCnt[txq] >= TX_BUF_RELEASE_THRESHOLD)
    	macTxRingProc(mac_p, txq);

	if (mac_p->txUnReleasedBufCnt[txq] == mac_p->txRingSize) {
		//printk("%s drop tx packet, ring is full GTTPR(%ld)=%08lx GTHPR(%ld)=%08lx\n", dev->name, txq, VPint(CR_GMAC_GTTPR(txq)), txq, VPint(CR_GMAC_GTHPR(txq)));
		mac_p->macStat.MIB_II.outDiscards++;
		dev_kfree_skb_any(skb);
		return NETDEV_TX_OK;
	}

	/* ----- Count the MIB-II ----- */
	mac_p->macStat.MIB_II.outOctets += length;

	if (*bufAddrp & 0x01)
		mac_p->macStat.MIB_II.outMulticastPkts++;
	else
		mac_p->macStat.MIB_II.outUnicastPkts++;

  	ledTurnOn(LED_ETHER_ACT_STATUS);
	if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_100MB)
		ledTurnOn(LED_ETHER_100M_ACT_STATUS);
	else
		ledTurnOn(LED_ETHER_10M_ACT_STATUS);

	dma_cache_wback_inv((unsigned long)(skb->data), length);

	spin_lock_irqsave(&mac_p->lock, flags);

	/* ----- Get the transmit descriptor ----- */
	currDescrp = ((macTxDescr_t *) mac_p->txDescrRingBaseVAddr[txq]) + mac_p->txCurrentDescp[txq];

	if (currDescrp->tdes0.word & (1<<31)) {
		printk("%s drop tx packet, tdes0=%08lx GTTPR(%ld)=%08lx GTHPR(%ld)=%08lx\n", dev->name, currDescrp->tdes0.word, txq, VPint(CR_GMAC_GTTPR(txq)), txq, VPint(CR_GMAC_GTHPR(txq)));
		mac_p->macStat.MIB_II.outDiscards++;
		dev_kfree_skb_any(skb);
		spin_unlock_irqrestore(&mac_p->lock, flags);
		return NETDEV_TX_OK;
	}

	/* tx buffer size */
	currDescrp->tdes0.word = 0;
  	currDescrp->tdes1.word = (length & 0x000007ff);
#ifdef LOOPBACK_SUPPORT
	/* ip checksum calculation */
	if (macLoopback & LOOPBACK_TX_IPCS)
  		currDescrp->tdes1.bits.ipcs_ins = 1;
	/* vlan tag insertion */
	if (macLoopback & LOOPBACK_TX_VLAN) {
  		currDescrp->tdes1.bits.vlan_ins = 1;
  		currDescrp->tdes1.bits.vlan_tag = txq;
	}
#endif
  	currDescrp->tdes2.txbuf_addr = K1_TO_PHY(skb->data);
	mac_p->txskbs[txq][mac_p->txCurrentDescp[txq]] = skb;

#ifdef LOOPBACK_SUPPORT
	if (macLoopback & LOOPBACK_MSG) {
		printk("\ttx txq=%ld curr=%ld\n", txq, mac_p->txCurrentDescp[txq]);
		printk("\t tdes0=%08lx\n", currDescrp->tdes0.word);
		printk("\t tdes1=%08lx\n", currDescrp->tdes1.word);
		printk("\t tdes2=%08lx\n", currDescrp->tdes2.word);
	}
#endif

	currDescrp->tdes0.word |= (1<<31);

	wmb();
	reg = read_reg_word(CR_GMAC_GTTPR(txq));
	if (mac_p->txCurrentDescp[txq] == (mac_p->txRingSize - 1)) {
		mac_p->txCurrentDescp[txq] = 0;
		reg = ((reg ^ 0x8000) & 0x8000) | mac_p->txCurrentDescp[txq];
	} else {
		mac_p->txCurrentDescp[txq]++;
		reg = (reg & 0x8000) | mac_p->txCurrentDescp[txq];
	}
	write_reg_word(CR_GMAC_GTTPR(txq), reg);

	mac_p->txUnReleasedBufCnt[txq]++;

  	mac_p->macStat.inSilicon.txEnQueueNum++;
	spin_unlock_irqrestore(&mac_p->lock, flags);

	return NETDEV_TX_OK;
}

#ifdef LOOPBACK_SUPPORT

void macPhyLoopback(macAdapter_t *mac_p, int enable)
{
	uint32 data;

	data = miiStationRead(mac_p, MII_BMCR);
	if (enable)
		data |= BMCR_LOOPBACK;
	else
		data &= ~BMCR_LOOPBACK;
	miiStationWrite(mac_p, MII_BMCR, data);
}

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

static void dump_data(char *p, int len)
{
	char tmp[80];
	char *t = tmp;
	int i, n = 0;

	printk("ERR data=%08lx len=%d\n", (uint32) p, len);
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

unsigned short in_csum(unsigned short *ptr, int nbytes)
{
	register int			sum;		/* assumes long == 32 bits */
	unsigned short			oddbyte;
	register unsigned short	answer; 	/* assumes u_short == 16 bits */

	/*
	 * Our algorithm is simple, using a 32-bit accumulator (sum),
	 * we add sequential 16-bit words to it, and at the end, fold back
	 * all the carry bits from the top 16 bits into the lower 16 bits.
	 */

	sum = 0;

	while (nbytes > 1)	{
		sum += *ptr++;
		nbytes -= 2;
	}

	/* mop up an odd byte, if necessary */
	if (nbytes == 1) {
		oddbyte = 0;		/* make sure top half is zero */
		*((unsigned char *) &oddbyte) = *(unsigned char *)ptr;   /* one byte only */
		sum += oddbyte;
	}

	/*
	 * Add back carry outs from top 16 bits to low 16 bits.
	 */

	sum  = (sum >> 16) + (sum & 0xffff);	/* add high-16 to low-16 */
	sum += (sum >> 16); 		/* add carry */
	answer = ~sum;		/* ones-complement, then truncate to 16 bits */
	return(answer);
}

static int tc3262_gmac_loopback_gen(struct net_device *dev, int npackets, int txlen)
{
	int i, k;
	struct sk_buff *skb;
	int tx_len;
	uint8 *tx_data;
	uint8 tx_seed;
	int offset = 0;
	int tx_priority;
	unsigned short chksum;
	int ip_len = 5;

	printk("Loopback test packets=%d txlen=%d\n", npackets, txlen);

	macRxLoopback = 0;
	for (i = 0; i < npackets; i++) {
		if ((i % 4096) == 0)
			printk("Gen %d packets.\n", i);

		skb = dev_alloc_skb(RX_BUF_LEN);
		if (skb == NULL)
			continue;

		offset =  random32();
		if (offset & 0x1)
			offset = 2;
		else
			offset = 0;

		skb_reserve(skb, offset);

		if (txlen == 0) {
			tx_len = random32();
			tx_len = tx_len % 1515;
			if (tx_len > 1514)
				tx_len = 1514;
			if (tx_len < 60)
				tx_len = 60;
		} else {
			tx_len = txlen;
		}

		tx_data = skb_put(skb, tx_len);
		for (k = 0; k < 6; k++)
			tx_data[k] = 0x11;
		for (k = 6; k < 12; k++)
			tx_data[k] = 0x22;

		tx_data[12] = 0x08;
		tx_data[13] = 0x00;

		tx_seed = (uint8) random32();
		if (macLoopback & LOOPBACK_TX_RANDOM)
			tx_priority = random32() & GMAC_PRIORITY_MASK;
		else
			tx_priority = 0;

		skb->priority = tx_priority;

		if (tx_len < (ip_len * 4 + 14))
			ip_len = (tx_len - 14) >> 2;

		tx_data[14] = 0x40 | ip_len;
		tx_data[15] = tx_seed;

		for (k = 16; k < tx_len; k++)
			tx_data[k] = (uint8) (tx_seed++ & 0xff);

		tx_data[24] = 0;
		tx_data[25] = 0;

		chksum = in_csum((unsigned short *) (skb->data + 14), ip_len << 2);
		tx_data[24] = (chksum >> 8) & 0xff;
		tx_data[25] = chksum & 0xff;

		ip_len++;
		if (ip_len > 15)
			ip_len = 5;

		tc3262_gmac_tx(skb, dev);
	}
	printk("Gen %d packets done.\n", npackets);

	mdelay(3000);

	printk("Chk %ld packets done.\n", macRxLoopback);

	if (macRxLoopback != npackets)
		printk("ERR TX/RX packet number mismatch.\n");

	return 0;
}

static int tc3262_gmac_loopback_chk(struct sk_buff *skb, struct net_device *dev)
{
	int k;
	uint8 *tx_data;
	uint8 tx_seed;
	int ip_len;

	macRxLoopback++;

	tx_data = skb->data;

	for (k = 0; k < 6; k++) {
		if (tx_data[k] != 0x11) {
			printk("loopback fail: dst mac unmatch\n");
			goto err;
		}
	}
	for (k = 6; k < 12; k++) {
		if (tx_data[k] != 0x22) {
			printk("loopback fail: src mac unmatch\n");
			goto err;
		}
	}

	if ((tx_data[12] != 0x08) || (tx_data[13] != 0x00)) {
		printk("loopback fail: ether type unmatch\n");
		goto err;
	}

	if ((tx_data[14] & 0xf0) != 0x40) {
		printk("loopback fail: ip header unmatch\n");
		goto err;
	}
	ip_len = (tx_data[14] & 0x0f) << 2;
	tx_seed = tx_data[15];

	for (k = 16; k < skb->len; k++) {
		if ((k != 24) && (k != 25)) {
			if (tx_data[k] != (uint8) (tx_seed++ & 0xff)) {
				printk("loopback fail: payload unmatch pos=%04x\n", k);
				goto err;
			}
		} else {
			tx_seed++;
		}
	}

	if (in_csum((unsigned short *) (skb->data + 14), ip_len) != 0) {
		printk("loopback fail: ip checksum unmatch\n");
		goto err;
	}

	dev_kfree_skb(skb);
	return 0;

err:
	dump_skb(skb);
	dev_kfree_skb(skb);
	return 1;
}

static int tc3262_gmac_qos_gen(struct net_device *dev)
{
	int i, k;
	struct sk_buff *skb;
	int tx_len;
	uint8 *tx_data;
	uint8 tx_seed;
	uint8 tx_queue;
	int txq;

	macDrvStop();

	for (txq = TX_QUEUE_NUM - 1; txq >= 0; txq--) {
		for (i = 0; i < MAC_TXDESCP_NO; i++) {
			skb = dev_alloc_skb(RX_BUF_LEN);
			if (skb == NULL)
				continue;

			skb->priority = txq;

			//tx_len = random32();
			tx_len = 60;
			tx_len = tx_len % 1514;
			if (tx_len > 1514)
				tx_len = 1514;
			if (tx_len < 60)
				tx_len = 60;

			tx_data = skb_put(skb, tx_len);
			for (k = 0; k < 6; k++)
				tx_data[k] = 0x10 + txq;
			for (k = 6; k < 12; k++)
				tx_data[k] = 0x22;

			tx_data[12] = 0x08;
			tx_data[13] = 0x01;

			tx_seed = (uint8) random32();
			tx_queue = (uint8) random32();

			tx_data[14] = tx_seed;
			tx_data[15] = tx_queue;

			for (k = 16; k < tx_len; k++)
				tx_data[k] = (uint8) (tx_seed++ & 0xff);

			tc3262_gmac_tx(skb, dev);
		}
	}

	macDrvStart();
	macRxDrvStart();

	return 0;
}

#else

#ifdef MT7530_VERIFY_DUMP_SKB
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

#endif

void macDefaultParaSet(macAdapter_t *mac_p)
{
	mac_p->rxDescrSize = MAC_RXDESCP_SIZE;
	mac_p->txDescrSize = MAC_TXDESCP_SIZE;
	mac_p->rxRingSize  = MAC_RXDESCP_NO;
	mac_p->txRingSize  = MAC_TXDESCP_NO;

  	mac_p->macStat.inSilicon.rxEnQueueNum = 0;
	mac_p->macStat.inSilicon.rxDeQueueNum = 0;
	mac_p->macStat.inSilicon.txEnQueueNum = 0;
	mac_p->macStat.inSilicon.txDeQueueNum = 0;
}

int macDrvRegInit(macAdapter_t *mac_p)
{
	macReset();

    macIMRMask();

    macSetIntConf();

    macSetDMADescrCtrlReg(mac_p);

    macSetMACCR(mac_p);

    // --- setup MAC address ---
    macSetMacReg(mac_p);

    macSetGSW(mac_p);		// add since MT7530

	if (isRT65168) {
#if defined(TCSUPPORT_HWNAT)
		/* enable hwnat, set TX WRR mode, CPU:NFE=1:2 */
		VPint(CR_GMAC_GNFER) |= GNFER_HIGH_SPD_DROP | GNFER_NAT_EN;

		//VPint(CR_GMAC_GNFER) |= GNFER_HIGH_SPD_DROP | GNFER_NAT_EN |
		//	(2<<GNFER_WRR_NFE_SHIFT) | (1<<GNFER_WRR_CPU_SHIFT) |
		//	(GNFER_TX_WRR & GNFER_TX_MODE);
#endif

		/* set RGMII 1 phase=90 */
		if (VPint(CR_AHB_HWCONF) & (1<<16))
			VPint(0xbfb000c0) |= (1<<0);
	}

    return 0;
}

void macDrvDescripReset(macAdapter_t *mac_p)
{
	macRxDescr_t *pRxDescp;
	macTxDescr_t *pTxDescp;
	struct sk_buff *skb;
#ifdef TC3262_GMAC_SG_MODE
	struct sk_buff *sg_skb;
#endif
	int i;
	uint32 txq = 0;

	pRxDescp = (macRxDescr_t*) mac_p->rxDescrRingBaseVAddr;
	for (i = 0; i < mac_p->rxRingSize; i++) {
		skb = mac_p->rxskbs[i];
		if (skb != NULL)
			dev_kfree_skb_any(skb);
#ifdef TC3262_GMAC_SG_MODE
		sg_skb = mac_p->sg_rxskbs[i];
		if (sg_skb != NULL)
			dev_kfree_skb_any(sg_skb);
#endif
		// Init Descriptor
		pRxDescp->rdes0.word = 0;
		pRxDescp->rdes1.word = 0;
		pRxDescp->rdes2.word = 0;
		pRxDescp->rdes3.word = 0;
		pRxDescp->rdes4.word = 0;
		pRxDescp->rdes5.word = 0;
		pRxDescp->rdes6.word = 0;
		pRxDescp->rdes7.word = 0;
		mac_p->rxskbs[i] = NULL;
#ifdef TC3262_GMAC_SG_MODE
		mac_p->sg_rxskbs[i] = NULL;
#endif
		pRxDescp++;
	}

	for (txq = 0; txq < TX_QUEUE_NUM; txq++) {
		pTxDescp = (macTxDescr_t*) mac_p->txDescrRingBaseVAddr[txq];
		// free all un-released tx mbuf
		for (i = 0 ; i < mac_p->txRingSize ; i++) {
			skb = mac_p->txskbs[txq][i];
			if (skb != NULL)
				dev_kfree_skb_any(skb);
			pTxDescp->tdes0.word = 0;
			pTxDescp->tdes1.word = 0;
			pTxDescp->tdes2.word = 0;
			pTxDescp->tdes3.word = 0;
			mac_p->txskbs[txq][i] = NULL;
			pTxDescp++;
		}
	}

	mac_p->rxCurrentDescp = 0;
	for (txq = 0; txq < TX_QUEUE_NUM; txq++) {
		mac_p->txCurrentDescp[txq] = 0;
		mac_p->txUnReleasedDescp[txq] = 0;
		mac_p->txUnReleasedBufCnt[txq] = 0;
	}
}

uint8 macDrvDescripInit(macAdapter_t *mac_p)
{
	macRxDescr_t *pRxDescp;
  	macTxDescr_t *pTxDescp;
  	uint32 i, txq;
  	struct sk_buff *skb;
#ifdef TC3262_GMAC_SG_MODE
  	struct sk_buff *sg_skb;
#endif

	mac_p->rxDescrRingBaseVAddr = (uint32) &mac_p->macRxMemPool_p->rxDescpBuf[0];
	for (txq = 0; txq < TX_QUEUE_NUM; txq++) {
		mac_p->txDescrRingBaseVAddr[txq] = (uint32) &mac_p->macTxMemPool_p->txDescpBuf[txq][0];
	}

	if (macInitialized)
		macDrvDescripReset(mac_p);

	/* init. Rx descriptor, allocate memory for each descriptor */
	pRxDescp = (macRxDescr_t*) mac_p->rxDescrRingBaseVAddr;
	for (i = 0 ; i< mac_p->rxRingSize ; i++, pRxDescp++) {
		// Init Descriptor
		pRxDescp->rdes0.word = 0;
		pRxDescp->rdes1.word = 0;
		pRxDescp->rdes2.word = 0;
		pRxDescp->rdes3.word = 0;
		pRxDescp->rdes4.word = 0;
		pRxDescp->rdes5.word = 0;
		pRxDescp->rdes6.word = 0;
		pRxDescp->rdes7.word = 0;

		// Assign flag
		pRxDescp->rdes0.bits.owner = 1;  /* owned by DMA */
		pRxDescp->rdes1.bits.rx_buf_size = RX_MAX_PKT_LEN;

		skb = gmac_alloc_skb2k();
		if (skb == NULL) {
			printk("tc3262_gmac_descinit init fail.\n");
			return 1;
		}
		dma_cache_inv((unsigned long)(skb->data), RX_MAX_PKT_LEN);
		skb_reserve(skb, NET_IP_ALIGN);

		pRxDescp->rdes2.rxbuf_addr = K1_TO_PHY(skb->data);
		mac_p->rxskbs[i] = skb;

#ifdef TC3262_GMAC_SG_MODE
		sg_skb = gmac_alloc_skb128();
		if (sg_skb == NULL) {
			printk("tc3262_gmac_descinit init fail.\n");
			return 1;
		}
		dma_cache_inv((unsigned long)(sg_skb->data), SG_MAX_PKT_LEN);
		skb_reserve(sg_skb, NET_IP_ALIGN);

		pRxDescp->rdes3.rxhdr_addr = K1_TO_PHY(sg_skb->data);
		mac_p->sg_rxskbs[i] = sg_skb;
#endif
	}

	for (txq = 0; txq < TX_QUEUE_NUM; txq++) {
		/* init. tx descriptor, don't allocate memory */
		pTxDescp = (macTxDescr_t*) mac_p->txDescrRingBaseVAddr[txq];
		for (i = 0 ; i < mac_p->txRingSize ; i++, pTxDescp++) {
    		// Init descriptor
    		pTxDescp->tdes0.word = 0;
    		pTxDescp->tdes1.word = 0;
    		pTxDescp->tdes2.word = 0;
    		pTxDescp->tdes3.word = 0;
			mac_p->txskbs[txq][i] = NULL;
		}
	}

	mac_p->rxCurrentDescp = 0;
	for (txq = 0; txq < TX_QUEUE_NUM; txq++) {
		mac_p->txCurrentDescp[txq] = 0;
		mac_p->txUnReleasedDescp[txq] = 0;
		mac_p->txUnReleasedBufCnt[txq] = 0;
	}

	return 0;
}

void macDrvStart(void)
{
	uint32 reg;

	reg = read_reg_word(CR_GMAC_GCTR);
	/* enable DMA tx& rx */
	reg |= GCTR_TX_DMA_EN;
	reg |= GCTR_RX_DMA_EN;
	/* enable mac tx& rx */
	reg |= GCTR_TX_EN;
	//reg |= GCTR_RX_EN;
	write_reg_word(CR_GMAC_GCTR, reg);
}

void macRxDrvStart(void)
{
	uint32 reg;

	reg = read_reg_word(CR_GMAC_GCTR);
	if ((reg & GCTR_RX_EN) == 0) {
		reg |= GCTR_RX_EN;
		write_reg_word(CR_GMAC_GCTR, reg);
	}
}

void macRxDrvStop(void)
{
	uint32 reg;

	reg = read_reg_word(CR_GMAC_GCTR);
	if (reg & GCTR_RX_EN) {
		reg &= ~GCTR_RX_EN;
		write_reg_word(CR_GMAC_GCTR, reg);
	}
}

void macPhyReset(void)
{
	#ifdef TCPHY_SUPPORT
	if(getTcPhyFlag()){
		tcPhyInit(mac_p);
	}
	else{
	#endif
		miiStationWrite(mac_p, MII_BMCR, BMCR_RESET);
		pause(10);
		miiStationWrite(mac_p, MII_BMCR, BMCR_ANRESTART | BMCR_ANENABLE);
	#ifdef TCPHY_SUPPORT
	}
	#endif
}

#if 0
//solve a lot of out-of-order packet. shnwind 20110323.
static inline int macRxRingChk(void)
{
	macRxDescr_t *rxDescrp;

	rxDescrp = ((macRxDescr_t*)mac_p->rxDescrRingBaseVAddr) + mac_p->rxCurrentDescp;
  	if ((rxDescrp->rdes0.bits.owner == 0))
		return 1;

	return -1;
}
#endif
#ifdef TCSUPPORT_AUTOBENCH
extern int autobench_mac_lpbk_flag;
extern int autobench_mac_lpbk_cnt;
extern int autobench_mac_lpbk1_flag;
extern int xxx;
extern unsigned char LoopbackData[];
#endif

int macRxRingProc(struct net_device *dev, int quota)
{
	volatile macRxDescr_t *rxDescrp;
	uint32 frameSize;
	struct sk_buff *newskb, *skb;
	uint32 rdes0_error_check, rdes1_error_check;
	uint32 reg;
	int npackets = 0;


	#ifdef TCPHY_4PORT
	uint16 maxPktSize=0;
	#endif

//WPLin20120905 temp
//printk("\nRX0: macRxRingProc!\n");

	rxDescrp = ((macRxDescr_t*)mac_p->rxDescrRingBaseVAddr) + mac_p->rxCurrentDescp;

	rdes0_error_check = (1<<27|1<<22|1<<21|1<<20|1<<19|1<<18);
  	//rdes1_error_check = (1<<28);
  	rdes1_error_check = (1<<28|1<<27);
#ifdef TC3262_GMAC_NAPI
  	while ((npackets <= quota) && (rxDescrp->rdes0.bits.owner == 0)) {
#else
  	while (rxDescrp->rdes0.bits.owner == 0) {
#endif
		npackets++;
#ifdef LOOPBACK_SUPPORT
		if (macLoopback & LOOPBACK_MSG) {
			printk("\t\trx curr=%ld rx=%08lx\n", mac_p->rxCurrentDescp, (uint32) rxDescrp);
			printk("\t\t rdes0=%08lx\n", rxDescrp->rdes0.word);
			printk("\t\t rdes1=%08lx\n", rxDescrp->rdes1.word);
			printk("\t\t rdes2=%08lx\n", rxDescrp->rdes2.word);
			printk("\t\t rdes3=%08lx\n", rxDescrp->rdes3.word);
			printk("\t\t rdes4=%08lx\n", rxDescrp->rdes4.word);
			printk("\t\t rdes5=%08lx\n", rxDescrp->rdes5.word);
			printk("\t\t rdes6=%08lx\n", rxDescrp->rdes6.word);
			printk("\t\t rdes7=%08lx\n", rxDescrp->rdes7.word);
	       	skb = mac_p->rxskbs[mac_p->rxCurrentDescp];
			skb->len = rxDescrp->rdes0.bits.rx_length;
			dump_skb(skb);
			skb->len = 0;
		}
#endif
	    if ((rxDescrp->rdes0.word & rdes0_error_check) == 0 &&
        	(rxDescrp->rdes1.word & rdes1_error_check) == 0) {
	    	// For removing Rx VLAN tag, we need to check if the remain length
	    	// is not smaller than minimun packet length.
	    	// rxDescrp->rdes0.bits.vlan_hit
	    	if (rxDescrp->rdes0.word & (1<<30)) {
	      		frameSize = rxDescrp->rdes0.bits.rx_length;
	      		frameSize = (frameSize > 60) ? frameSize : 60;
	      	} else {
 	 			frameSize = rxDescrp->rdes0.bits.rx_length;
			}
		#ifdef TCPHY_4PORT
	      	switch(swicVendor){
			case SWIC_TC2206:
				/*TC2206 Special Tag Length=8bytes, vlan header 4 bytes, max ethernet length=1518; 1518+8+4=1530*/
				maxPktSize=1530;
				break;
			default:
				maxPktSize=1522;
				break;
		}
	      	if (unlikely((frameSize < 60) || (frameSize > maxPktSize))){
		#else
		/* lino: make VLAN friendly */
		if (unlikely((frameSize < 60) || (frameSize > 1522))){
		#endif
#ifdef LOOPBACK_SUPPORT
				printk("\t\tERR length rx curr=%ld rx=%08lx\n", mac_p->rxCurrentDescp, (uint32) rxDescrp);
				printk("\t\t rdes0=%08lx\n", rxDescrp->rdes0.word);
				printk("\t\t rdes1=%08lx\n", rxDescrp->rdes1.word);
				printk("\t\t rdes2=%08lx\n", rxDescrp->rdes2.word);
				printk("\t\t rdes3=%08lx\n", rxDescrp->rdes3.word);
				printk("\t\t rdes4=%08lx\n", rxDescrp->rdes4.word);
				printk("\t\t rdes5=%08lx\n", rxDescrp->rdes5.word);
				printk("\t\t rdes6=%08lx\n", rxDescrp->rdes6.word);
				printk("\t\t rdes7=%08lx\n", rxDescrp->rdes7.word);

	       		skb = mac_p->rxskbs[mac_p->rxCurrentDescp];
				skb->len = rxDescrp->rdes0.bits.rx_length;
				dump_skb(skb);
				skb->len = 0;
#endif

	        	mac_p->macStat.inSilicon.rxEtherFrameLengthErr++;

	        	// Discard this packet & Repost this mbuf
#ifdef TC3262_GMAC_SG_MODE
				if (frameSize <= SG_MAX_PKT_LEN)
	        		newskb = mac_p->sg_rxskbs[mac_p->rxCurrentDescp];
				else
	        		newskb = mac_p->rxskbs[mac_p->rxCurrentDescp];
#else
	        	newskb = mac_p->rxskbs[mac_p->rxCurrentDescp];
#endif
	        	goto DISCARD;
	      	}

	      	// ----- Count the MIB-II -----
		    if (mac_p->statisticOn) {
	  	    	mac_p->macStat.MIB_II.inOctets += frameSize;

			    if ((rxDescrp->rdes0.word) & (1<<16))
			    	mac_p->macStat.MIB_II.inMulticastPkts++;
			    else
		        	mac_p->macStat.MIB_II.inUnicastPkts++;
		    }

#ifdef TC3262_GMAC_SG_MODE
			if (frameSize <= SG_MAX_PKT_LEN) {
	       		skb = mac_p->sg_rxskbs[mac_p->rxCurrentDescp];

				newskb = gmac_alloc_skb128();
	        	if (unlikely(!newskb)) { /* faild to allocate more mbuf -> drop this pkt */
	        		newskb = skb;
	          		mac_p->macStat.MIB_II.inDiscards++;
	          		goto RECVOK;
	      		}
				dma_cache_inv((unsigned long)(newskb->data), SG_MAX_PKT_LEN);
				skb_reserve(newskb, NET_IP_ALIGN);
			} else {
	       		skb = mac_p->rxskbs[mac_p->rxCurrentDescp];

				newskb = gmac_alloc_skb2k();
	        	if (unlikely(!newskb)) { /* faild to allocate more mbuf -> drop this pkt */
	        		newskb = skb;
	          		mac_p->macStat.MIB_II.inDiscards++;
	          		goto RECVOK;
	      		}
				dma_cache_inv((unsigned long)(newskb->data), RX_MAX_PKT_LEN);
				skb_reserve(newskb, NET_IP_ALIGN);
			}
#else
       		skb = mac_p->rxskbs[mac_p->rxCurrentDescp];

			newskb = gmac_alloc_skb2k();
        	if (unlikely(!newskb)) { /* faild to allocate more mbuf -> drop this pkt */
        		newskb = skb;
          		mac_p->macStat.MIB_II.inDiscards++;
          		goto RECVOK;
      		}

			dma_cache_inv((unsigned long)(newskb->data), RX_MAX_PKT_LEN);
			skb_reserve(newskb, NET_IP_ALIGN);
#endif

			skb_put(skb, frameSize);

#ifdef MT7530_VERIFY_DUMP_SKB
//WPLin20120905 temp
//printk("RX0: CP1\n");
//dump_skb(skb);
//printk("\n\n\n");
#endif

#ifdef LOOPBACK_SUPPORT
			if (macLoopback & LOOPBACK_PKT) {
				printk("RX: ");
				dump_skb(skb);
			}
#endif

#if defined(TCSUPPORT_HWNAT)
			skb->dev = dev;
			if (pktflow_rx_hook) {
				if (pktflow_rx_hook(skb, 0, &rxDescrp->rdes3.word))
					goto RECVOK;
			}
#endif

			#if 1// defined(WAN2LAN)  || defined(VPORT)
			/*add specail tag function by xyzhu*/
			if (macSTagFlag) {
				switch (swicVendor) {
					case SWIC_ADM6996M:
						//macAdmSTagRemove(skb);
						break;
					case SWIC_IP175C:
						macIpSTagRemove(skb);
						break;
					case SWIC_TC2206:
						macTC2206STagRemove(skb);
						break;
		#if defined(RT62806_VERIFY)
		#if defined(TEST_ENABLE_MT7530_STAG)
					case SWIC_RT62806:
						macRT62806STagRemove(skb);
						break;
					case SWIC_MT7530:
						macMT7530STagRemove(skb);
						break;
		#endif
		#endif
					default:
						break;
				}
			}
			#endif
#ifdef TCSUPPORT_AUTOBENCH
			if( autobench_mac_lpbk_flag){
				if( memcmp(skb->data, LoopbackData, MAC_LPBK_DATA_LEN) == 0 ){
					autobench_mac_lpbk_cnt++;
				}
				else{
					int i;
					for( i=0; MAC_LPBK_DATA_LEN; i++){
						printk("%02x ", (unsigned char)skb->data[i]);
						if( (i+1)%8 == 0 )
							printk("\n");
					}
				}
				dev_kfree_skb_any(skb);
				goto RECVOK;
			}
			if( autobench_mac_lpbk1_flag ){
				autobench_mac_lpbk_cnt++;
				xxx=1;
				dev_kfree_skb_any(skb);
				goto RECVOK;
			}
#endif
#ifdef LOOPBACK_SUPPORT
			if (skb_shinfo(skb)->nr_frags) {
				int i;
				printk("GMAC nr_frags=%ld %lx\n", (uint32) skb_shinfo(skb)->nr_frags, (uint32) skb_shinfo(skb)->nr_frags);
				for (i = 0; i < 16; i++)
					printk("page%d=%lx\n", i, (uint32) skb_shinfo(skb)->frags[i].page);
				printk("ERR skb=%08lx data=%08lx len=%d\n", (uint32) skb, (uint32) skb->data, skb->len);
				printk("\t\tERR rx curr=%ld rx=%08lx\n", mac_p->rxCurrentDescp, (uint32) rxDescrp);
				printk("\t\t rdes0=%08lx\n", rxDescrp->rdes0.word);
				printk("\t\t rdes1=%08lx\n", rxDescrp->rdes1.word);
				printk("\t\t rdes2=%08lx\n", rxDescrp->rdes2.word);
				printk("\t\t rdes3=%08lx\n", rxDescrp->rdes3.word);
				printk("\t\t rdes4=%08lx\n", rxDescrp->rdes4.word);
				printk("\t\t rdes5=%08lx\n", rxDescrp->rdes5.word);
				printk("\t\t rdes6=%08lx\n", rxDescrp->rdes6.word);
				printk("\t\t rdes7=%08lx\n", rxDescrp->rdes7.word);
				dump_data(skb->data, 2048);
				dump_data(UNCAC_ADDR(skb->data), 2048);
			}

			if (LOOPBACK_MODE(macLoopback) == LOOPBACK_TX) {
				tc3262_gmac_tx(skb, dev);
			} else if (LOOPBACK_MODE(macLoopback) == LOOPBACK_RX_DROP) {
				dev_kfree_skb_any(skb);
			} else if (LOOPBACK_MODE(macLoopback) == LOOPBACK_RX_CHK) {
				tc3262_gmac_loopback_chk(skb, dev);
			} else
#endif
			{
#ifdef CONFIG_TC3162_ADSL
			if(tcconsole_proc(skb)==1){
				;
			}
			else {
#else
			{
#endif

#ifdef MT7530_VERIFY_DUMP_SKB
//WPLin20120905 temp
//printk("RX0: netif_rx\n");
//dump_skb(skb);
//printk("\n");
#endif

					skb->dev = dev;
					skb->ip_summed = CHECKSUM_NONE;
					skb->protocol = eth_type_trans(skb, dev);
					dev->last_rx = jiffies;
#ifdef TC3262_GMAC_NAPI
					netif_receive_skb(skb);
#else
					netif_rx(skb);
#endif
				}
			}
DISCARD:

RECVOK:

#ifdef TC3262_GMAC_SG_MODE
			if (frameSize <= SG_MAX_PKT_LEN) {
	      		rxDescrp->rdes3.rxhdr_addr = K1_TO_PHY(newskb->data);
	       		mac_p->sg_rxskbs[mac_p->rxCurrentDescp] = newskb;
			} else {
	      		rxDescrp->rdes2.rxbuf_addr = K1_TO_PHY(newskb->data);
	       		mac_p->rxskbs[mac_p->rxCurrentDescp] = newskb;
			}
#else
	      	rxDescrp->rdes2.rxbuf_addr = K1_TO_PHY(newskb->data);
	       	mac_p->rxskbs[mac_p->rxCurrentDescp] = newskb;
#endif
		} else { /* Update Error Counter and Drop it */
#ifdef LOOPBACK_SUPPORT
			printk("\t\tERR rx curr=%ld rx=%08lx\n", mac_p->rxCurrentDescp, (uint32) rxDescrp);
			printk("\t\t rdes0=%08lx\n", rxDescrp->rdes0.word);
			printk("\t\t rdes1=%08lx\n", rxDescrp->rdes1.word);
			printk("\t\t rdes2=%08lx\n", rxDescrp->rdes2.word);
			printk("\t\t rdes3=%08lx\n", rxDescrp->rdes3.word);
			printk("\t\t rdes4=%08lx\n", rxDescrp->rdes4.word);
			printk("\t\t rdes5=%08lx\n", rxDescrp->rdes5.word);
			printk("\t\t rdes6=%08lx\n", rxDescrp->rdes6.word);
			printk("\t\t rdes7=%08lx\n", rxDescrp->rdes7.word);

	       	skb = mac_p->rxskbs[mac_p->rxCurrentDescp];
			skb->len = rxDescrp->rdes0.bits.rx_length;
			dump_skb(skb);
			skb->len = 0;
#endif

	      	if (mac_p->statisticOn) {
		  		if (rxDescrp->rdes1.word & (1<<28)) {
					mac_p->macStat.inSilicon.rx802p3FrameLengthErr++;
					mac_p->macStat.MIB_II.inErrors++;
				}
		  		if (rxDescrp->rdes1.word & (1<<27)) {
					mac_p->macStat.inSilicon.rxAlignErr++;
					mac_p->macStat.MIB_II.inErrors++;
				}
				if (rxDescrp->rdes0.word & (1<<27)) {
					mac_p->macStat.inSilicon.rxPktIPChkSumErr++;
					mac_p->macStat.MIB_II.inErrors++;
				}
	        	if (rxDescrp->rdes0.word & (1<<22)) {
		        	mac_p->macStat.inSilicon.rxCollisionErr++;
		          	mac_p->macStat.MIB_II.inErrors++;
		        }
		        if (rxDescrp->rdes0.word & (1<<21)) {
					mac_p->macStat.inSilicon.rxRuntErr++;
					mac_p->macStat.MIB_II.inErrors++;
				}
		        if (rxDescrp->rdes0.word & (1<<20)) {
					mac_p->macStat.inSilicon.rxLongErr++;
					mac_p->macStat.MIB_II.inErrors++;
				}
		        if (rxDescrp->rdes0.word & (1<<19)) {
					mac_p->macStat.inSilicon.rxCrcErr++;
					mac_p->macStat.MIB_II.inErrors++;
				}
		        if (rxDescrp->rdes0.word & (1<<18)) {
					mac_p->macStat.inSilicon.rxSymbolErr++;
					mac_p->macStat.MIB_II.inErrors++;
				}
			}
		}

	    rxDescrp->rdes0.word |= (1<<31);

		wmb();
 		/* next descriptor*/
		reg = read_reg_word(CR_GMAC_GRTPR);
		if ( mac_p->rxCurrentDescp == (mac_p->rxRingSize - 1)) {
			mac_p->rxCurrentDescp = 0;
			reg = ((reg ^ 0x8000) & 0x8000) | mac_p->rxCurrentDescp;
		} else {
			mac_p->rxCurrentDescp++;
			reg = (reg & 0x8000) | mac_p->rxCurrentDescp;
		}
		write_reg_word(CR_GMAC_GRTPR, reg);

		rxDescrp = ((macRxDescr_t*)mac_p->rxDescrRingBaseVAddr) + mac_p->rxCurrentDescp;
	}
	/* process one packet */
	write_reg_word(CR_GMAC_GRPPCR, npackets);

#ifdef LOOPBACK_SUPPORT
	if (macLoopback & LOOPBACK_MSG)
		printk("npackets=%d\n", npackets);
#endif

	return npackets;
}

void macDrvStop(void)
{
	uint32 reg;

	reg = read_reg_word(CR_GMAC_GCTR);
	/* disable DMA tx& rx */
	reg &= ~GCTR_TX_DMA_EN;
	reg &= ~GCTR_RX_DMA_EN;
	/* disable mac tx& rx */
	reg &= ~GCTR_TX_EN;
	reg &= ~GCTR_RX_EN;
	write_reg_word(CR_GMAC_GCTR, reg);
}

macAdapter_t * macGetAdapterByChanID(void)
{
	return mac_p;
}

void macGetMacAddr(macAdapter_t *mac_p)
{
	uint32 i;

	for ( i = 0; i < 6; i++ )
		mac_p->macAddr[i] = def_mac_addr[i];
}

int32 macPhyLookUp(macAdapter_t *mac_p, uint32 companyId)
{
	uint32 i;
	uint32 phyTypeSupport;

	phyTypeSupport = sizeof(phyDeviceList) / sizeof(phyDeviceList_t);
	for ( i = 0; i < phyTypeSupport; i++ ) {
		if ( companyId == phyDeviceList[i].companyId ) {
			mac_p->enetPhyId = i;
			#ifdef TCPHY_SUPPORT
			if ((companyId == 0x03a2) || (companyId == 0x0007)){	// add MTK chip ID
				tcPhyVerLookUp(mac_p);
			}
			#endif
			return 1;
		}
	}
	return 0;
}

int32 macSearchPhyAddr(macAdapter_t *mac_p)
{
	uint32 miiReg = 0;

	mac_p->enetPhyAddr = 0;
	for ( mac_p->enetPhyAddr = 0; mac_p->enetPhyAddr < 32; mac_p->enetPhyAddr++ ) {
		miiReg = miiStationRead(mac_p, MII_PHYSID1);
		if(miiReg == 0xFFFF)
			miiReg = tcMiiStationRead_ext(mac_p->enetPhyAddr, MII_PHYSID1);
		if (miiReg == 0)
			miiReg = miiStationRead(mac_p, MII_PHYSID2);
		if (macPhyLookUp(mac_p, miiReg)) {
			return 0;
		}
	}

	mac_p->enetPhyAddr = 0x00000000;

	return -1;
}

int macSetUpPhy(macAdapter_t *mac_p)
{
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
	#ifdef TCPHY_4PORT
	/*Switch Model Number Register (SMNR), Addr.: 16'h02FE*/
	if (tcMiiStationRead(31, 31) == 0x2206){
	    swicVendor = SWIC_TC2206;
	    filedSwicDefVal();
	    printk("TC2206, ");
	}
	#endif

	/* detect if RTL8305 */
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

	/* detect if MARVEL 88E6060 */
	if (((tcMiiStationRead(4, 2) == 0x141) || (tcMiiStationRead(20, 2) == 0x141)) &&
	    ((tcMiiStationRead(4, 3) == 0xc87) || (tcMiiStationRead(20, 3) == 0xc87))) {
		swicVendor = SWIC_MARVEL6060;
	}

	/* detect if RT62806, frank added 20110628*/
	if (isRT62806) {
		swicVendor = SWIC_RT62806;
		printk("swicVendor=SWIC_RT62806\n");
	} /* end frank added 20110628*/

	/* detect if MT7530, wplin added 20120712*/
	if (isMT7530 || isMT7530ext) {
		swicVendor = SWIC_MT7530;
		printk("swicVendor=SWIC_MT7530\n");
	} /* end wplin added 20120712*/

	printk("swicVendor: %d\n", swicVendor);

	return 0;
}

int macInit(void)
{
	if (macInitialized)
		return 0;

	macDrvStop();

	/* ----- Get Mac Adapter from dummy data or NDIS control block ----- */
	mac_p = macGetAdapterByChanID();

	/* ----- Assign reserved data pointer ----- */
	mac_p->macPhyLinkProfile_p = &enetPhyLinkProfile;
#ifdef CONFIG_TC3162_DMEM
	mac_p->macRxMemPool_p = (macRxMemPool_t *) alloc_sram(sizeof(macRxMemPool_t));
	if (mac_p->macRxMemPool_p == NULL)
#endif
	mac_p->macRxMemPool_p = (macRxMemPool_t *) dma_alloc_coherent(NULL, sizeof(macRxMemPool_t), &mac_p->macRxMemPool_phys_p, GFP_KERNEL);
	if (mac_p->macRxMemPool_p == NULL) {
		printk("unable to kmalloc macRxMemPool structure.\n");
		return -1;
	}
//#ifdef CONFIG_TC3162_DMEM
//	mac_p->macTxMemPool_p = (macTxMemPool_t *) alloc_sram(sizeof(macTxMemPool_t));
//	if (mac_p->macTxMemPool_p == NULL)
//#endif
	mac_p->macTxMemPool_p = (macTxMemPool_t *) dma_alloc_coherent(NULL, sizeof(macTxMemPool_t), &mac_p->macTxMemPool_phys_p, GFP_KERNEL);
	if (mac_p->macTxMemPool_p == NULL) {
		printk("unable to kmalloc macTxMemPool structure.\n");
		return -1;
	}

	/* ----- Set up the paramters ----- */
	macDefaultParaSet(mac_p);

	/* ----- Get the Mac address ----- */
	macGetMacAddr(mac_p);

	/* ----- Initialize Tx/Rx descriptors ----- */
	if (macDrvDescripInit(mac_p) != 0)
		return -1;

	/* ----- Initialize the phy chip ----- */
	if (macSetUpPhy(mac_p))
		return -1;

	/* ----- Initialize Registers ----- */
	macDrvRegInit(mac_p);

	mac_p->statisticOn = MAC_STATISTIC_ON;

	macInitialized = 1;
	macDrvStart();
#ifndef TCSUPPORT_AUTOBENCH
	#ifdef TCPHY_SUPPORT
	tcephydbgcmd();
	#ifdef TC2031_DEBUG
	tcethercmd();
	#endif
	#endif
#endif
	#if 1// defined(WAN2LAN) || defined(CONFIG_TC3162_ADSL) || defined(VPORT)
	#ifndef TC2031_SUPPORT
	/* ------ xyzhu_nj_091104:Enable special Tag default ----- */
	macRxPortEnable(0);
	#endif
	#endif
	return 0;
}

int macSwReset(macAdapter_t *mac_p)
{
	macRxDescr_t *pRxDescp;
	macTxDescr_t *pTxDescp;
	struct sk_buff *skb;
#ifdef TC3262_GMAC_SG_MODE
	struct sk_buff *sg_skb;
#endif
	uint32 reg;
	uint32 i, txq;

	if (!macInitialized)
		return 0;

	if (netif_running(tc3262_gmac_dev)) {
		/* wait here for poll to complete */
		netif_poll_disable(tc3262_gmac_dev);
		netif_tx_disable(tc3262_gmac_dev);
	}

	macDrvStop();

	/* init. Rx descriptor, allocate memory for each descriptor */
	pRxDescp = (macRxDescr_t*) mac_p->rxDescrRingBaseVAddr;
	for (i = 0 ; i< mac_p->rxRingSize ; i++, pRxDescp++) {
		// Assign flag
		pRxDescp->rdes0.bits.owner = 1;  /* owned by DMA */
		pRxDescp->rdes1.bits.rx_buf_size = RX_MAX_PKT_LEN;
	}

	for (txq = 0; txq < TX_QUEUE_NUM; txq++) {
		pTxDescp = (macTxDescr_t*) mac_p->txDescrRingBaseVAddr[txq];
		// free all un-released tx mbuf
		for (i = 0 ; i < mac_p->txRingSize ; i++) {
			skb = mac_p->txskbs[txq][i];
			if (skb != NULL)
				dev_kfree_skb_any(skb);
			pTxDescp->tdes0.word = 0;
			pTxDescp->tdes1.word = 0;
			pTxDescp->tdes2.word = 0;
			pTxDescp->tdes3.word = 0;
			mac_p->txskbs[txq][i] = NULL;
			pTxDescp++;
		}
	}

	mac_p->rxCurrentDescp = 0;
	for (txq = 0; txq < TX_QUEUE_NUM; txq++) {
		mac_p->txCurrentDescp[txq] = 0;
		mac_p->txUnReleasedDescp[txq] = 0;
		mac_p->txUnReleasedBufCnt[txq] = 0;
	}

	write_reg_word(CR_GMAC_GRTPR, 0x8000);
	for (txq = 0; txq < TX_QUEUE_NUM; txq++)
		write_reg_word(CR_GMAC_GTTPR(txq), 0);

	/* Software reset Mac */
	reg = read_reg_word(CR_GMAC_GCTR);
	reg |= GCTR_SW_RESET;
	write_reg_word (CR_GMAC_GCTR, reg);
	pause(10);

	macDrvStart();

	if (netif_running(tc3262_gmac_dev)) {
		netif_wake_queue(tc3262_gmac_dev);
		netif_poll_enable(tc3262_gmac_dev);
	}

	return 0;
}

/************************************************************************
*     A D S L   R E L A T E D    F U N C T I O N S
*************************************************************************
*/

static void tc3262_gmac_poll_func(unsigned long data)
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

void TCConsole(uint8 mode)
{
}
EXPORT_SYMBOL(TCConsole);

void uartMacPutchar(int ch)
{
}
EXPORT_SYMBOL(uartMacPutchar);

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

	if (!macInitialized) {
		index += sprintf(buf+index, "Down\n");
		return index;
	}

	if ((mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_LINK_UP) == 0) {
		index += sprintf(buf+index, "Down\n");
		return index;
	}

	if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_1000MB)
		index += sprintf(buf+index, "1000M/");
	else if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_100MB)
		index += sprintf(buf+index, "100M/");
	else
		index += sprintf(buf+index, "10M/");

	if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_FULL_DUPLEX)
		index += sprintf(buf+index, "Full Duplex\n");
	else
		index += sprintf(buf+index, "Half Duplex\n");
	return index;
}

/* frank addded 20110624 */
static int gsw_stats_write_proc(struct file *file, const char *buffer,
		unsigned long count, void *data)
{
	char val_string[32];

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

		val_string[count] = '\0';

		return count;
}


static int gsw_stats_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	int port;

	uint32 mib_offset;

	if (!macInitialized) {
		*eof = 1;
		return 0;
	}

	for (port = 0; port <= 6; port++) {
		mib_offset = MIB_PORT_OFFSET*port;
		index += sprintf(buf+index, "[ Port %d ]\n", port);
		CHK_BUF();
		index += sprintf(buf+index, "Tx drop pkts          = 0x%08lx, ", gswPbusRead(MIB_TX_DROP_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx ucast pkts         = 0x%08lx\n", gswPbusRead(MIB_TX_UCAST_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx mcast pkts         = 0x%08lx, ", gswPbusRead(MIB_TX_MCAST_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx bcast pkts         = 0x%08lx\n", gswPbusRead(MIB_TX_BCAST_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx col	              = 0x%08lx, ", gswPbusRead(MIB_TX_COL_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx scol               = 0x%08lx\n", gswPbusRead(MIB_TX_SCOL_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx mcol               = 0x%08lx, ", gswPbusRead(MIB_TX_MCOL_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx defer              = 0x%08lx\n", gswPbusRead(MIB_TX_DEFER_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx lcol               = 0x%08lx, ", gswPbusRead(MIB_TX_LCOL_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx xcol               = 0x%08lx\n", gswPbusRead(MIB_TX_XCOL_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx pause              = 0x%08lx\n", gswPbusRead(MIB_TX_PAUSE_REG + mib_offset));
		CHK_BUF();

		index += sprintf(buf+index, "Tx 64 pkts            = 0x%08lx, ", gswPbusRead(MIB_TX_PKT_64_CNT + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx 65-127 pkts        = 0x%08lx\n", gswPbusRead(MIB_TX_PKT_65TO127_CNT + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx 128-255 pkts       = 0x%08lx, ", gswPbusRead(MIB_TX_PKT_128TO255_CNT + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx 256-511 pkts       = 0x%08lx\n", gswPbusRead(MIB_TX_PKT_256TO511_CNT + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx 512-1023 pkts      = 0x%08lx, ", gswPbusRead(MIB_TX_PKT_512TO1023_CNT + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx 1024-MAX pkts      = 0x%08lx\n", gswPbusRead(MIB_TX_PKT_1024TOMAX_CNT + mib_offset));
		CHK_BUF();


		index += sprintf(buf+index, "Rx drop pkts          = 0x%08lx, ", gswPbusRead(MIB_RX_DROP_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx filter pkts        = 0x%08lx\n", gswPbusRead(MIB_RX_FILTER_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx ucast pkts         = 0x%08lx, ", gswPbusRead(MIB_RX_UCAST_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx mcast pkts         = 0x%08lx\n", gswPbusRead(MIB_RX_MCAST_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx bcast pkts         = 0x%08lx, ", gswPbusRead(MIB_RX_BCAST_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx align err pkts     = 0x%08lx\n", gswPbusRead(MIB_RX_ALIGN_ERR_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx fcs pkts           = 0x%08lx, ", gswPbusRead(MIB_RX_FCS_ERR_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx undersize pkts     = 0x%08lx\n", gswPbusRead(MIB_RX_UNDERSIZE_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx frag err pkts      = 0x%08lx, ", gswPbusRead(MIB_RX_FRAG_ERR_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx oversize pkts      = 0x%08lx\n", gswPbusRead(MIB_RX_OVERSIZE_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx jabb err pkts      = 0x%08lx, ", gswPbusRead(MIB_RX_JABB_ERR_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx pase pkts          = 0x%08lx\n", gswPbusRead(MIB_RX_PAUSE_REG + mib_offset));
		CHK_BUF();

		index += sprintf(buf+index, "Rx 64 pkts            = 0x%08lx, ", gswPbusRead(MIB_RX_PKT_64_CNT + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx 65-127 pkts        = 0x%08lx\n", gswPbusRead(MIB_RX_PKT_65TO127_CNT + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx 128-255 pkts       = 0x%08lx, ", gswPbusRead(MIB_RX_PKT_128TO255_CNT + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx 256-511 pkts       = 0x%08lx\n", gswPbusRead(MIB_RX_PKT_256TO511_CNT + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx 512-1023 pkts      = 0x%08lx, ", gswPbusRead(MIB_RX_PKT_512TO1023_CNT + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx 1024-MAX pkts      = 0x%08lx\n", gswPbusRead(MIB_RX_PKT_1024TOMAX_CNT + mib_offset));
		CHK_BUF();

		index += sprintf(buf+index, "Tx oct                = 0x%08lx, ", gswPbusRead(MIB_TX_OCT_CNT_L + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx oct                = 0x%08lx\n", gswPbusRead(MIB_RX_OCT_CNT_L + mib_offset));
		CHK_BUF();

		index += sprintf(buf+index, "------\n");
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
/* end frank added 20110624 */

/* wplin addded 20120703 */
static int gsw_mibN_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data, uint portId)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	int port;

	uint32 mib_offset;

	if (!macInitialized) {
		*eof = 1;
		return 0;
	}


	port = portId;
	{
		mib_offset = MIB_PORT_OFFSET*port;
		index += sprintf(buf+index, "[ Port %d ]\n", port);
		CHK_BUF();
		index += sprintf(buf+index, "Tx drop pkts          = 0x%08lx, ", gswPbusRead(MIB_TX_DROP_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx ucast pkts         = 0x%08lx\n", gswPbusRead(MIB_TX_UCAST_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx mcast pkts         = 0x%08lx, ", gswPbusRead(MIB_TX_MCAST_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx bcast pkts         = 0x%08lx\n", gswPbusRead(MIB_TX_BCAST_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx col	              = 0x%08lx, ", gswPbusRead(MIB_TX_COL_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx scol               = 0x%08lx\n", gswPbusRead(MIB_TX_SCOL_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx mcol               = 0x%08lx, ", gswPbusRead(MIB_TX_MCOL_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx defer              = 0x%08lx\n", gswPbusRead(MIB_TX_DEFER_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx lcol               = 0x%08lx, ", gswPbusRead(MIB_TX_LCOL_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx xcol               = 0x%08lx\n", gswPbusRead(MIB_TX_XCOL_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Tx pause              = 0x%08lx\n", gswPbusRead(MIB_TX_PAUSE_REG + mib_offset));
		CHK_BUF();

		index += sprintf(buf+index, "Rx drop pkts          = 0x%08lx, ", gswPbusRead(MIB_RX_DROP_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx filter pkts        = 0x%08lx\n", gswPbusRead(MIB_RX_FILTER_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx ucast pkts         = 0x%08lx, ", gswPbusRead(MIB_RX_UCAST_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx mcast pkts         = 0x%08lx\n", gswPbusRead(MIB_RX_MCAST_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx bcast pkts         = 0x%08lx, ", gswPbusRead(MIB_RX_BCAST_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx align err pkts     = 0x%08lx\n", gswPbusRead(MIB_RX_ALIGN_ERR_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx fcs pkts           = 0x%08lx, ", gswPbusRead(MIB_RX_FCS_ERR_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx undersize pkts     = 0x%08lx\n", gswPbusRead(MIB_RX_UNDERSIZE_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx frag err pkts      = 0x%08lx, ", gswPbusRead(MIB_RX_FRAG_ERR_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx oversize pkts      = 0x%08lx\n", gswPbusRead(MIB_RX_OVERSIZE_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx jabb err pkts      = 0x%08lx, ", gswPbusRead(MIB_RX_JABB_ERR_REG + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx pase pkts          = 0x%08lx\n", gswPbusRead(MIB_RX_PAUSE_REG + mib_offset));
		CHK_BUF();

		index += sprintf(buf+index, "Rx 64 pkts            = 0x%08lx, ", gswPbusRead(MIB_RX_PKT_64_CNT + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx 65-127 pkts        = 0x%08lx\n", gswPbusRead(MIB_RX_PKT_65TO127_CNT + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx 128-255 pkts       = 0x%08lx, ", gswPbusRead(MIB_RX_PKT_128TO255_CNT + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx 256-511 pkts       = 0x%08lx\n", gswPbusRead(MIB_RX_PKT_256TO511_CNT + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx 512-1023 pkts      = 0x%08lx, ", gswPbusRead(MIB_RX_PKT_512TO1023_CNT + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx 1024-MAX pkts      = 0x%08lx\n", gswPbusRead(MIB_RX_PKT_1024TOMAX_CNT + mib_offset));
		CHK_BUF();

		index += sprintf(buf+index, "Tx oct                = 0x%08lx, ", gswPbusRead(MIB_TX_OCT_CNT_L + mib_offset));
		CHK_BUF();
		index += sprintf(buf+index, "Rx oct                = 0x%08lx\n", gswPbusRead(MIB_RX_OCT_CNT_L + mib_offset));
		CHK_BUF();

		index += sprintf(buf+index, "------\n");
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

static int gsw_mib0_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	return gsw_mibN_read_proc(buf, start, off, count, eof, data, 0);
}

static int gsw_mib1_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	return gsw_mibN_read_proc(buf, start, off, count, eof, data, 1);
}
static int gsw_mib2_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	return gsw_mibN_read_proc(buf, start, off, count, eof, data, 2);
}
static int gsw_mib3_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	return gsw_mibN_read_proc(buf, start, off, count, eof, data, 3);
}
static int gsw_mib4_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	return gsw_mibN_read_proc(buf, start, off, count, eof, data, 4);
}
static int gsw_mib5_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	return gsw_mibN_read_proc(buf, start, off, count, eof, data, 5);
}
static int gsw_mib6_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	return gsw_mibN_read_proc(buf, start, off, count, eof, data, 6);
}
/* end wplin added 20120703 */


static int eth_reset_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "0\n");
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

static int eth_reset_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32];

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	macSwReset(mac_p);

	return count;
}

static int eth_stats_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	uint32 value;
#if defined(TCSUPPORT_HWNAT)
	struct pktflow_stats pf_stats;
#endif

	if (!macInitialized) {
		*eof = 1;
		return 0;
	}

#if defined(TCSUPPORT_HWNAT)
	if (pktflow_get_stats_hook)
		pktflow_get_stats_hook(&pf_stats, 0);
	else
		memset(&pf_stats, 0, sizeof(pf_stats));
#endif

	value = mac_p->macStat.MIB_II.inOctets;
#if defined(TCSUPPORT_HWNAT)
	value += pf_stats.rx_bytes;
#endif
	index += sprintf(buf+index, "inOctets              = 0x%08lx, ", value);
	CHK_BUF();
	value = mac_p->macStat.MIB_II.inUnicastPkts;
#if defined(TCSUPPORT_HWNAT)
	value += pf_stats.rx_packets;
#endif
	index += sprintf(buf+index, "inUnicastPkts         = 0x%08lx\n", value);
	CHK_BUF();

	index += sprintf(buf+index, "inMulticastPkts       = 0x%08lx, ", mac_p->macStat.MIB_II.inMulticastPkts);
	CHK_BUF();
	value = mac_p->macStat.MIB_II.inDiscards;
#if defined(TCSUPPORT_HWNAT)
	value += pf_stats.rx_dropped;
#endif
	index += sprintf(buf+index, "inDiscards            = 0x%08lx\n", value);
	CHK_BUF();

	index += sprintf(buf+index, "inErrors              = 0x%08lx, ", mac_p->macStat.MIB_II.inErrors);
	CHK_BUF();
	value = mac_p->macStat.MIB_II.outOctets;
#if defined(TCSUPPORT_HWNAT)
	value += pf_stats.tx_bytes;
#endif
	index += sprintf(buf+index, "outOctets             = 0x%08lx\n", value);
	CHK_BUF();

	value = mac_p->macStat.MIB_II.outUnicastPkts;
#if defined(TCSUPPORT_HWNAT)
	value += pf_stats.tx_packets;
#endif
	index += sprintf(buf+index, "outUnicastPkts        = 0x%08lx, ", value);
	CHK_BUF();
	index += sprintf(buf+index, "outMulticastPkts      = 0x%08lx\n", mac_p->macStat.MIB_II.outMulticastPkts);
	CHK_BUF();

	value = mac_p->macStat.MIB_II.outDiscards;
#if defined(TCSUPPORT_HWNAT)
	value += pf_stats.tx_dropped;
#endif
	index += sprintf(buf+index, "outDiscards           = 0x%08lx, ", value);
	CHK_BUF();
	index += sprintf(buf+index, "outErrors             = 0x%08lx\n", mac_p->macStat.MIB_II.outErrors);
	CHK_BUF();

	index += sprintf(buf+index, "\n[ Statistics Display ]\n");
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
	index += sprintf(buf+index, "rxOverrunInt          = 0x%08lx\n", mac_p->macStat.inSilicon.rxOverrunInt);
	CHK_BUF();

	index += sprintf(buf+index, "\n[ Extra information ]\n");
	CHK_BUF();
	index += sprintf(buf+index, "Rx Descriptor idx     = 0x%08lx  ", mac_p->rxCurrentDescp);
	CHK_BUF();
	index += sprintf(buf+index, "Tx Descriptor idx     = 0x%08lx\n", mac_p->txCurrentDescp[0]);
	CHK_BUF();
 	index += sprintf(buf+index, "Rx Enqueued cnt       = 0x%08lx  ", mac_p->macStat.inSilicon.rxEnQueueNum);
	CHK_BUF();
  	index += sprintf(buf+index, "Tx Enqueued cnt       = 0x%08lx\n", mac_p->macStat.inSilicon.txEnQueueNum);
	CHK_BUF();
  	index += sprintf(buf+index, "Rx Dequeued cnt       = 0x%08lx  ", mac_p->macStat.inSilicon.rxDeQueueNum);
	CHK_BUF();
  	index += sprintf(buf+index, "Tx Dequeued cnt       = 0x%08lx\n", mac_p->macStat.inSilicon.txDeQueueNum);
	CHK_BUF();
  	index += sprintf(buf+index, "Tx Buf UnReleased cnt = 0x%08lx  ", mac_p->txUnReleasedBufCnt[0]);
	CHK_BUF();
  	index += sprintf(buf+index, "Tx Buf UnReleased idx = 0x%08lx\n", mac_p->txUnReleasedDescp[0]);
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

#if defined(TCSUPPORT_HWNAT)
	if (pktflow_clear_stats_hook)
		pktflow_clear_stats_hook(0);
#endif

	return count;
}

#ifdef LOOPBACK_SUPPORT

static int eth_loopback_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%04x\n", macLoopback);
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

static int eth_loopback_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32];

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	macLoopback = simple_strtoul(val_string, NULL, 16);
	if (LOOPBACK_MODE(macLoopback) == LOOPBACK_RX_CHK) {
		macPhyLoopback(mac_p, 1);
		mdelay(2000);
	} else if (LOOPBACK_MODE(macLoopback) == LOOPBACK_TX_QOS) {
		tc3262_gmac_qos_gen(tc3262_gmac_dev);
	}
	return count;
}

static int eth_loopback_test_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "\n");
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

static int eth_loopback_test_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32];
	int npackets, txlen;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	sscanf(val_string, "%d %d", &npackets, &txlen);
	tc3262_gmac_loopback_gen(tc3262_gmac_dev, npackets, txlen);

	return count;
}

#endif

static int eth_link_st_proc(char *buf, char **start, off_t off, int count,
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

static int eth_reg_dump_proc(char *buf, char **start, off_t off, int count,
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

	index += sprintf(buf+index, "CR_GMAC_GCTR       (0x%08x) = 0x%08lx\n", CR_GMAC_GCTR, VPint(CR_GMAC_GCTR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GIMR       (0x%08x) = 0x%08lx\n", CR_GMAC_GIMR, VPint(CR_GMAC_GIMR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GISR       (0x%08x) = 0x%08lx\n", CR_GMAC_GISR, VPint(CR_GMAC_GISR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GSPR       (0x%08x) = 0x%08lx\n", CR_GMAC_GSPR, VPint(CR_GMAC_GSPR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GMAR       (0x%08x) = 0x%08lx\n", CR_GMAC_GMAR, VPint(CR_GMAC_GMAR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_RBFR       (0x%08x) = 0x%08lx\n", CR_GMAC_RBFR, VPint(CR_GMAC_RBFR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_SMDR       (0x%08x) = 0x%08lx\n", CR_GMAC_SMDR, VPint(CR_GMAC_SMDR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GMAR1      (0x%08x) = 0x%08lx\n", CR_GMAC_GMAR1, VPint(CR_GMAC_GMAR1));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_DPSR       (0x%08x) = 0x%08lx\n", CR_GMAC_DPSR, VPint(CR_GMAC_DPSR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_PCRR       (0x%08x) = 0x%08lx\n", CR_GMAC_PCRR, VPint(CR_GMAC_PCRR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_VPMR       (0x%08x) = 0x%08lx\n", CR_GMAC_VPMR, VPint(CR_GMAC_VPMR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_TPR0       (0x%08x) = 0x%08lx\n", CR_GMAC_TPR0, VPint(CR_GMAC_TPR0));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_TPR1       (0x%08x) = 0x%08lx\n", CR_GMAC_TPR1, VPint(CR_GMAC_TPR1));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_TPR2       (0x%08x) = 0x%08lx\n", CR_GMAC_TPR2, VPint(CR_GMAC_TPR2));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_TPR3       (0x%08x) = 0x%08lx\n", CR_GMAC_TPR3, VPint(CR_GMAC_TPR3));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_TCPR       (0x%08x) = 0x%08lx\n", CR_GMAC_TCPR, VPint(CR_GMAC_TCPR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GSGCR      (0x%08x) = 0x%08lx\n", CR_GMAC_GSGCR, VPint(CR_GMAC_GSGCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GRRBR      (0x%08x) = 0x%08lx\n", CR_GMAC_GRRBR, VPint(CR_GMAC_GRRBR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GTRBR      (0x%08x) = 0x%08lx\n", CR_GMAC_GTRBR, VPint(CR_GMAC_GTRBR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GRRSR      (0x%08x) = 0x%08lx\n", CR_GMAC_GRRSR, VPint(CR_GMAC_GRRSR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GTRS01R    (0x%08x) = 0x%08lx\n", CR_GMAC_GTRS01R, VPint(CR_GMAC_GTRS01R));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GTRS23R    (0x%08x) = 0x%08lx\n", CR_GMAC_GTRS23R, VPint(CR_GMAC_GTRS23R));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GTRS45R    (0x%08x) = 0x%08lx\n", CR_GMAC_GTRS45R, VPint(CR_GMAC_GTRS45R));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GTRS67R    (0x%08x) = 0x%08lx\n", CR_GMAC_GTRS67R, VPint(CR_GMAC_GTRS67R));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GRHPR      (0x%08x) = 0x%08lx\n", CR_GMAC_GRHPR, VPint(CR_GMAC_GRHPR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GRTPR      (0x%08x) = 0x%08lx\n", CR_GMAC_GRTPR, VPint(CR_GMAC_GRTPR));
	CHK_BUF();
	for (i = 0; i < TX_QUEUE_NUM; i++) {
		index += sprintf(buf+index, "CR_GMAC_GTHPR(%d)   (0x%08x) = 0x%08lx\n", i, CR_GMAC_GTHPR(i), VPint(CR_GMAC_GTHPR(i)));
		CHK_BUF();
	}
	for (i = 0; i < TX_QUEUE_NUM; i++) {
		index += sprintf(buf+index, "CR_GMAC_GTTPR(%d)   (0x%08x) = 0x%08lx\n", i, CR_GMAC_GTTPR(i), VPint(CR_GMAC_GTTPR(i)));
		CHK_BUF();
	}
	index += sprintf(buf+index, "CR_GMAC_GTLWR      (0x%08x) = 0x%08lx\n", CR_GMAC_GTLWR, VPint(CR_GMAC_GTLWR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GTUSR      (0x%08x) = 0x%08lx\n", CR_GMAC_GTUSR, VPint(CR_GMAC_GTUSR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GRXCR      (0x%08x) = 0x%08lx\n", CR_GMAC_GRXCR, VPint(CR_GMAC_GRXCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GPRCTR     (0x%08x) = 0x%08lx\n", CR_GMAC_GPRCTR, VPint(CR_GMAC_GPRCTR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GTIETR     (0x%08x) = 0x%08lx\n", CR_GMAC_GTIETR, VPint(CR_GMAC_GTIETR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GRIETR     (0x%08x) = 0x%08lx\n", CR_GMAC_GRIETR, VPint(CR_GMAC_GRIETR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GTQCR      (0x%08x) = 0x%08lx\n", CR_GMAC_GTQCR, VPint(CR_GMAC_GTQCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GTSLR      (0x%08x) = 0x%08lx\n", CR_GMAC_GTSLR, VPint(CR_GMAC_GTSLR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GSBCR0     (0x%08x) = 0x%08lx\n", CR_GMAC_GSBCR0, VPint(CR_GMAC_GSBCR0));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GSBCR1     (0x%08x) = 0x%08lx\n", CR_GMAC_GSBCR1, VPint(CR_GMAC_GSBCR1));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GSBCR2     (0x%08x) = 0x%08lx\n", CR_GMAC_GSBCR2, VPint(CR_GMAC_GSBCR2));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GSBCR3     (0x%08x) = 0x%08lx\n", CR_GMAC_GSBCR3, VPint(CR_GMAC_GSBCR3));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GWWR0      (0x%08x) = 0x%08lx\n", CR_GMAC_GWWR0, VPint(CR_GMAC_GWWR0));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GWWR1      (0x%08x) = 0x%08lx\n", CR_GMAC_GWWR1, VPint(CR_GMAC_GWWR1));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GWWR2      (0x%08x) = 0x%08lx\n", CR_GMAC_GWWR2, VPint(CR_GMAC_GWWR2));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GWWR3      (0x%08x) = 0x%08lx\n", CR_GMAC_GWWR3, VPint(CR_GMAC_GWWR3));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_RTRR       (0x%08x) = 0x%08lx\n", CR_GMAC_RTRR, VPint(CR_GMAC_RTRR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_RTSR       (0x%08x) = 0x%08lx\n", CR_GMAC_RTSR, VPint(CR_GMAC_RTSR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_RTNSR      (0x%08x) = 0x%08lx\n", CR_GMAC_RTNSR, VPint(CR_GMAC_RTNSR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_TTRR       (0x%08x) = 0x%08lx\n", CR_GMAC_TTRR, VPint(CR_GMAC_TTRR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_TTSR       (0x%08x) = 0x%08lx\n", CR_GMAC_TTSR, VPint(CR_GMAC_TTSR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_TTNSR      (0x%08x) = 0x%08lx\n", CR_GMAC_TTNSR, VPint(CR_GMAC_TTNSR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_ANER       (0x%08x) = 0x%08lx\n", CR_GMAC_ANER, VPint(CR_GMAC_ANER));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_PHYOR      (0x%08x) = 0x%08lx\n", CR_GMAC_PHYOR, VPint(CR_GMAC_PHYOR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GRBSR      (0x%08x) = 0x%08lx\n", CR_GMAC_GRBSR, VPint(CR_GMAC_GRBSR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GRDLR      (0x%08x) = 0x%08lx\n", CR_GMAC_GRDLR, VPint(CR_GMAC_GRDLR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GTDLR      (0x%08x) = 0x%08lx\n", CR_GMAC_GTDLR, VPint(CR_GMAC_GTDLR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_TXPCR      (0x%08x) = 0x%08lx\n", CR_GMAC_TXPCR, VPint(CR_GMAC_TXPCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_TXECCR     (0x%08x) = 0x%08lx\n", CR_GMAC_TXECCR, VPint(CR_GMAC_TXECCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_TXLCCR     (0x%08x) = 0x%08lx\n", CR_GMAC_TXLCCR, VPint(CR_GMAC_TXLCCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_TXSCCR     (0x%08x) = 0x%08lx\n", CR_GMAC_TXSCCR, VPint(CR_GMAC_TXSCCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_TXMCCR     (0x%08x) = 0x%08lx\n", CR_GMAC_TXMCCR, VPint(CR_GMAC_TXMCCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_TXPPCR     (0x%08x) = 0x%08lx\n", CR_GMAC_TXPPCR, VPint(CR_GMAC_TXPPCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_TXDCR      (0x%08x) = 0x%08lx\n", CR_GMAC_TXDCR, VPint(CR_GMAC_TXDCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_TXBCR      (0x%08x) = 0x%08lx\n", CR_GMAC_TXBCR, VPint(CR_GMAC_TXBCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_RXPCR      (0x%08x) = 0x%08lx\n", CR_GMAC_RXPCR, VPint(CR_GMAC_RXPCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_RXBPCR     (0x%08x) = 0x%08lx\n", CR_GMAC_RXBPCR, VPint(CR_GMAC_RXBPCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_RXMPCR     (0x%08x) = 0x%08lx\n", CR_GMAC_RXMPCR, VPint(CR_GMAC_RXMPCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_RXRPCR     (0x%08x) = 0x%08lx\n", CR_GMAC_RXRPCR, VPint(CR_GMAC_RXRPCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_RXLPCR     (0x%08x) = 0x%08lx\n", CR_GMAC_RXLPCR, VPint(CR_GMAC_RXLPCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_RXCECR     (0x%08x) = 0x%08lx\n", CR_GMAC_RXCECR, VPint(CR_GMAC_RXCECR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_RXAECR     (0x%08x) = 0x%08lx\n", CR_GMAC_RXAECR, VPint(CR_GMAC_RXAECR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_RXRECR     (0x%08x) = 0x%08lx\n", CR_GMAC_RXRECR, VPint(CR_GMAC_RXRECR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_RXPPCR     (0x%08x) = 0x%08lx\n", CR_GMAC_RXPPCR, VPint(CR_GMAC_RXPPCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_RXBCR      (0x%08x) = 0x%08lx\n", CR_GMAC_RXBCR, VPint(CR_GMAC_RXBCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_TXPUCR     (0x%08x) = 0x%08lx\n", CR_GMAC_TXPUCR, VPint(CR_GMAC_TXPUCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GDRPCR     (0x%08x) = 0x%08lx\n", CR_GMAC_GDRPCR, VPint(CR_GMAC_GDRPCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GRPPCR     (0x%08x) = 0x%08lx\n", CR_GMAC_GRPPCR, VPint(CR_GMAC_GRPPCR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GESR       (0x%08x) = 0x%08lx\n", CR_GMAC_GESR, VPint(CR_GMAC_GESR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_EEE        (0x%08x) = 0x%08lx\n", CR_GMAC_EEE, VPint(CR_GMAC_EEE));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GNFER      (0x%08x) = 0x%08lx\n", CR_GMAC_GNFER, VPint(CR_GMAC_GNFER));
	CHK_BUF();

	index += sprintf(buf+index, "[PHY REG] PHYADDR=%ld\n", mac_p->enetPhyAddr);
	CHK_BUF();
	for (i = 0; i <= 10; i++) {
   		reg = miiStationRead(mac_p, i);
		index += sprintf(buf+index, "PHY reg%d=0x%08lx\n", i, reg);
		CHK_BUF();
	}

	for (i = 15; i <= 20; i++) {
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

static int eth_rxring_dump_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	int i;
	macRxDescr_t *pRxDescp;

	if (!macInitialized) {
		*eof = 1;
		return 0;
	}

	pRxDescp = (macRxDescr_t*) mac_p->rxDescrRingBaseVAddr;
	index += sprintf(buf+index, "rx descr ring=%08lx\n", (uint32) pRxDescp);
	CHK_BUF();

	for (i = 0 ; i< mac_p->rxRingSize ; i++, pRxDescp++) {
		index += sprintf(buf+index, "i= %d descr=%08lx\n", i, (uint32) pRxDescp);
		CHK_BUF();
		index += sprintf(buf+index, " rdes0=%08lx\n", pRxDescp->rdes0.word);
		CHK_BUF();
		index += sprintf(buf+index, " rdes1=%08lx\n", pRxDescp->rdes1.word);
		CHK_BUF();
		index += sprintf(buf+index, " rdes2=%08lx\n", pRxDescp->rdes2.word);
		CHK_BUF();
		index += sprintf(buf+index, " rdes3=%08lx\n", pRxDescp->rdes3.word);
		CHK_BUF();
		index += sprintf(buf+index, " rdes4=%08lx\n", pRxDescp->rdes4.word);
		CHK_BUF();
		index += sprintf(buf+index, " rdes5=%08lx\n", pRxDescp->rdes5.word);
		CHK_BUF();
		index += sprintf(buf+index, " rdes6=%08lx\n", pRxDescp->rdes6.word);
		CHK_BUF();
		index += sprintf(buf+index, " rdes7=%08lx\n", pRxDescp->rdes7.word);
		CHK_BUF();
		index += sprintf(buf+index, " skb  =%08lx\n", (uint32) mac_p->rxskbs[i]);
		CHK_BUF();
	}

	index += sprintf(buf+index, "rxCurrentDescp    =%ld\n", mac_p->rxCurrentDescp);
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GRTPR     =%08lx\n", VPint(CR_GMAC_GRTPR));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GRHPR     =%08lx\n", VPint(CR_GMAC_GRHPR));
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

static int eth_txring_dump_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	int i, txq;
  	macTxDescr_t *pTxDescp;

	if (!macInitialized) {
		*eof = 1;
		return 0;
	}

	txq = *((int *) data);

	pTxDescp = (macTxDescr_t*) mac_p->txDescrRingBaseVAddr[txq];

	index += sprintf(buf+index, "tx descr ring%d=%08x\n", txq, (unsigned int) pTxDescp);
	CHK_BUF();

	for (i = 0 ; i < mac_p->txRingSize ; i++, pTxDescp++) {
		index += sprintf(buf+index, "i= %d descr=%08lx\n", i, (uint32) pTxDescp);
		CHK_BUF();
		index += sprintf(buf+index, " tdes0=%08lx\n", pTxDescp->tdes0.word);
		CHK_BUF();
		index += sprintf(buf+index, " tdes1=%08lx\n", pTxDescp->tdes1.word);
		CHK_BUF();
		index += sprintf(buf+index, " tdes2=%08lx\n", pTxDescp->tdes2.word);
		CHK_BUF();
		index += sprintf(buf+index, " tdes3=%08lx\n", pTxDescp->tdes3.word);
		CHK_BUF();
		index += sprintf(buf+index, " skb  =%08lx\n", (uint32) mac_p->txskbs[txq][i]);
		CHK_BUF();
	}

	index += sprintf(buf+index, "txCurrentDescp[%d]    =%ld\n", txq, mac_p->txCurrentDescp[txq]);
	CHK_BUF();
	index += sprintf(buf+index, "txUnReleasedDescp[%d] =%ld\n", txq, mac_p->txUnReleasedDescp[txq]);
	CHK_BUF();
	index += sprintf(buf+index, "txUnReleasedBufCnt[%d]=%ld\n", txq, mac_p->txUnReleasedBufCnt[txq]);
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GTTPR(%d)  =%08lx\n", txq, VPint(CR_GMAC_GTTPR(txq)));
	CHK_BUF();
	index += sprintf(buf+index, "CR_GMAC_GTHPR(%d)  =%08lx\n", txq, VPint(CR_GMAC_GTHPR(txq)));
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
#define HELP_MT7530_PHY_E1_SWPATCH
#ifdef HELP_MT7530_PHY_E1_SWPATCH
//u32 old_phy_poll;
//old_phy_poll = gswPbusRead(0x7018);
//gswPbusWrite(0x7018, (old_phy_poll & 0x00FFFFFF));
	#ifdef USE_GLOBAL_PHY_LOCK
//spin_lock_bh(&global_phy_lock);
	#endif
#endif
		tcPhySwPatch();
#ifdef HELP_MT7530_PHY_E1_SWPATCH
	#ifdef USE_GLOBAL_PHY_LOCK
//spin_unlock_bh(&global_phy_lock);
	#endif
//gswPbusWrite(0x7018, old_phy_poll); //V1.10 mark for no auto polling, because link status is read clear,so auto polling will effect phy part link status check. => Now MAC flow test will fail //V1.14 modify register from 0x701C to 0x7018
#endif
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
}/*end periodChk*/
#endif
/* Monitor the status of link, re-do link initialization if necessary. */
static void tc3262_gmac_link_status(unsigned long data)
{
	uint32 bmsr;
	uint32 bmcr;
	uint32 reg;
	uint32 loopback = 0;
	uint32 advert, lpa;
	uint32 advert2, lpa2;
	uint32 enetMode;

	if (mac_p == NULL)
		goto leave;

#ifdef TCPHY_SUPPORT
	periodChk();
#endif
#ifdef TC_CONSOLE_ENABLE
	tcconsole_chk();
#endif

#ifdef TC3262_GMAC_NAPI
#ifdef LOOPBACK_SUPPORT
	if (!LOOPBACK_MODE(macLoopback))
#endif
    {
		int txq;

		for (txq = 0; txq < TX_QUEUE_NUM; txq++)
   			macTxRingProc(mac_p, txq);
	}
#endif

	/* save the original enet mode */
	enetMode = mac_p->macPhyLinkProfile_p->enetMode;

#if defined(TCSUPPORT_AUTOBENCH) && defined(CONFIG_MIPS_RT65168)
	if (1) {
		mac_p->macPhyLinkProfile_p->enetMode =  LAN_ST_1000MB | LAN_ST_FULL_DUPLEX | LAN_ST_LINK_UP;
#else
	if (swicVendor) {
		mac_p->macPhyLinkProfile_p->enetMode =  LAN_ST_100MB | LAN_ST_FULL_DUPLEX | LAN_ST_LINK_UP;

#if (0)
//	#if defined(CONFIG_MIPS_RT65168)
	{ /* for bug#10434, if one of LAN ports's link rate is 10M, disable CPU port's flow
	   * control to avoid TC2206's HOL blocking mechanism.  However, disabling CPU port's
	   * flow control will suffer wifi's throughput, so if none of LAN ports's link rate
	   * is 10M, enable CPU port's flow control right away!
	   */
		uint16 value=0, value2=0;
		int isLink10M=0, isLinkUp=0, phy=0;

		if(swicVendor == SWIC_TC2206){
			for(phy=0; phy<4; phy++){
				//define the phy's page to local page 0, so we can read registers correctly.
				value2 = tcMiiStationRead(phy, 31);
				tcMiiStationWrite(phy, 31, 0x8000);
				//read the phy's port status register
				value = tcMiiStationRead(phy, 28);
				//switch back to the original page
				tcMiiStationWrite(phy, 31, value2);
				if(value & 0x1){ //bit0 means link is up
					isLinkUp=1;
					if(!(value & 0x2)){ //bit1 means link rate is 10M or 100M
						isLink10M=1;
						break;
					}
				}
			}

			if(isLinkUp){
				//read Port5 (CPU port) MAC Control Register
				value = tcMiiStationRead(21, 23);

				if (isLink10M){
					//if flow control is enabled, disable it for bug#10434
					if(value & 0x18){
						value &= ~0x18;
						tcMiiStationWrite(21, 23, value);
					}
				}
				else{
					//if flow control is disabled, enable it to avoid poor wifi throughput
					if(!(value & 0x18)){
						value |= 0x18;
						tcMiiStationWrite(21, 23, value);
					}
				}
			}
		}
	}
	#endif


#endif
	} else {
#if (0)
		mac_p->macPhyLinkProfile_p->enetMode = 0;
		bmcr = miiStationRead(mac_p, MII_BMCR);
		loopback = bmcr & BMCR_LOOPBACK;

		/* ----- Check reset bit & power down bit ----- */
		if (bmcr & (BMCR_RESET | BMCR_PDOWN))
			goto leave;
		#ifdef TCPHY_SUPPORT
		/*Updating by phy register*/
		if (getTcPhyFlag())
			bmsr = getTcPhyStatusReg(mac_p);
		else
		#endif
		bmsr = miiStationRead(mac_p, MII_BMSR);

		if (bmcr & BMCR_ANENABLE) {
			if (bmsr & BMSR_ANEGCOMPLETE) {
				advert = miiStationRead(mac_p, MII_ADVERTISE);
				lpa = miiStationRead(mac_p, MII_LPA);

				advert2 = miiStationRead(mac_p, MII_CTRL1000);
				lpa2 = miiStationRead(mac_p, MII_STAT1000);

				if ((advert2 & (lpa2 >> 2)) & ADVERTISE_1000FULL)
					mac_p->macPhyLinkProfile_p->enetMode |= (LAN_ST_FULL_DUPLEX | LAN_ST_1000MB);
				else if ((advert2 & (lpa2 >> 2)) & ADVERTISE_1000HALF)
					mac_p->macPhyLinkProfile_p->enetMode |= (LAN_ST_1000MB);
				if ((advert & lpa) & ADVERTISE_100FULL)
					mac_p->macPhyLinkProfile_p->enetMode |= (LAN_ST_FULL_DUPLEX | LAN_ST_100MB);
				else if ((advert & lpa) & ADVERTISE_100HALF)
					mac_p->macPhyLinkProfile_p->enetMode |= (LAN_ST_100MB);
				else if ((advert & lpa) & ADVERTISE_10FULL)
					mac_p->macPhyLinkProfile_p->enetMode |= (LAN_ST_FULL_DUPLEX);
				else if ((advert & lpa) & ADVERTISE_10HALF)
					mac_p->macPhyLinkProfile_p->enetMode |= 0;
			}
		} else {
			if ((bmcr & BMCR_SPEED1000) && (bmcr & BMCR_SPEED100) == 0)
				mac_p->macPhyLinkProfile_p->enetMode |= (LAN_ST_1000MB);
			else if (bmcr & BMCR_SPEED100)
				mac_p->macPhyLinkProfile_p->enetMode |= (LAN_ST_100MB);

			if (bmcr & BMCR_FULLDPLX)
				mac_p->macPhyLinkProfile_p->enetMode |= (LAN_ST_FULL_DUPLEX);
		}

		if (bmsr & BMSR_LSTATUS)
			mac_p->macPhyLinkProfile_p->enetMode |= LAN_ST_LINK_UP;
		else
			mac_p->macPhyLinkProfile_p->enetMode &= ~LAN_ST_LINK_UP;

#endif
	}

	if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_LINK_UP) {
		if (!loopback) {
			if (enetMode != mac_p->macPhyLinkProfile_p->enetMode) {
				/* According to enet phy link mode to adjust mac full duplex mode */
				reg = read_reg_word(CR_GMAC_GCTR);
				if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_FULL_DUPLEX)
					reg |= GCTR_FULL_DUPLEX;
				else
					reg &= ~GCTR_FULL_DUPLEX;
				write_reg_word(CR_GMAC_GCTR, reg);

				/* According to enet phy link mode to adjust mac speed */
				reg = read_reg_word(CR_GMAC_GSPR);
				if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_1000MB)
					reg |= GSPR_MAC_SPD_GIGA;
				else
					reg &= ~GSPR_MAC_SPD_GIGA;
				if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_100MB)
					reg |= GSPR_CFG_SPD_100M;
				else
					reg &= ~GSPR_CFG_SPD_100M;
				write_reg_word(CR_GMAC_GSPR, reg);

				macSwReset(mac_p);
			}
		}
		macRxDrvStart();
	} else {
		macRxDrvStop();
	}

leave:
	/* Schedule for the next time */
	eth_timer.expires = jiffies + msecs_to_jiffies(250);
  	add_timer(&eth_timer);
}

/* Starting up the ethernet device */
static int tc3262_gmac_open(struct net_device *dev)
{
	int err;

  	printk(KERN_INFO "%s: starting interface.\n", dev->name);

	err = request_irq(dev->irq, tc3262_gmac_isr, 0, dev->name, dev);
	if (err)
		return err;

  	macInit();

	/* MII setup */
	mac_p->mii_if.phy_id = mac_p->enetPhyAddr;
	mac_p->mii_if.full_duplex = 1;
	mac_p->mii_if.phy_id_mask = 0x1f;
	mac_p->mii_if.reg_num_mask = 0x1f;
	mac_p->mii_if.dev = dev;
	mac_p->mii_if.mdio_read = mdio_read;
	mac_p->mii_if.mdio_write = mdio_write;
	mac_p->mii_if.supports_gmii = mii_check_gmii_support(&mac_p->mii_if);

  	/* Schedule timer for monitoring link status */
  	init_timer(&eth_timer);
	eth_timer.expires = jiffies + msecs_to_jiffies(250);
  	eth_timer.function = tc3262_gmac_link_status;
  	eth_timer.data = (unsigned long)dev;
  	add_timer(&eth_timer);

  	/* Schedule timer for monitoring link status */
  	init_timer(&eth_poll_timer);
  	eth_poll_timer.expires = jiffies + msecs_to_jiffies(500);
  	eth_poll_timer.function = tc3262_gmac_poll_func;
  	eth_poll_timer.data = (unsigned long)dev;
  	add_timer(&eth_poll_timer);

  	netif_start_queue(dev);

  	return 0;
}

/* Stopping the ethernet device */
static int tc3262_gmac_close(struct net_device *dev)
{
	printk(KERN_INFO "tc3262_gmac_close\n");

  	netif_stop_queue(dev);

	free_irq(dev->irq, dev);

  	/* Kill timer */
  	del_timer_sync(&eth_timer);
  	del_timer_sync(&eth_poll_timer);

  	return 0;
}

/* Setup multicast list */
static void tc3262_gmac_set_multicast_list(struct net_device *dev)
{
	return; /* Do nothing */
}

/* Setting customized mac address */
static int tc3262_gmac_set_macaddr(struct net_device *dev, void *p)
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
static struct net_device_stats *tc3262_gmac_stats(struct net_device *dev)
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

#if defined(TCSUPPORT_HWNAT)
	if (pktflow_get_stats_hook) {
		struct pktflow_stats pf_stats;

		pktflow_get_stats_hook(&pf_stats, 0);

		stats->rx_packets += pf_stats.rx_packets;
		stats->tx_packets += pf_stats.tx_packets;
		stats->rx_bytes += pf_stats.rx_bytes;
		stats->tx_bytes += pf_stats.tx_bytes;
		stats->rx_dropped += pf_stats.rx_dropped;
		stats->tx_dropped += pf_stats.tx_dropped;
	}
#endif

	return stats;
}

/* Handling ioctl call */
static int tc3262_gmac_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	int rc = 0;
	gsw_reg reg;
	struct mii_ioctl_data mii;
#ifdef MT7530_PHY_VERIFY
	struct _gsw_phy_cl45 mii45;
#endif

	if (macInitialized) {
		switch (cmd) {
			case RAETH_GSW_REG_READ:
			case RAETH_GSWEXT_REG_READ:
				copy_from_user(&reg, ifr->ifr_data, sizeof(reg));
//				if (isRT62806 || isMT7530 || isMT7530ext){		// rt62806 switch
				if ((swicVendor == SWIC_MT7530) || (swicVendor == SWIC_RT62806)){	// MT7530 switch
					reg.val = gswPbusRead(GSW_BASE + reg.off);
					copy_to_user(ifr->ifr_data, &reg, sizeof(reg));
				} else if (isRT63365) {			// rt63365 switch
					reg.val = read_reg_word(GSW_BASE + reg.off);
					copy_to_user(ifr->ifr_data, &reg, sizeof(reg));
				}
				break;
			case RAETH_GSW_REG_WRITE:
			case RAETH_GSWEXT_REG_WRITE:
				copy_from_user(&reg, ifr->ifr_data, sizeof(reg));
//				if (isRT62806 || isMT7530 || isMT7530ext){		// rt62806 switch
				if ((swicVendor == SWIC_MT7530) || (swicVendor == SWIC_RT62806)){	// MT7530 switch
					gswPbusWrite(GSW_BASE + reg.off, reg.val);
				} else if (isRT63365) {			// rt63365 switch
					write_reg_word(GSW_BASE + reg.off, reg.val);
				}
				break;
			case RAETH_GSW_PHY_READ:
			case RAETH_GSWEXT_PHY_READ:
				copy_from_user(&mii, ifr->ifr_data, sizeof(mii));
#define SUPPORT_MT7530E1_PHY_ABIST_MODE
#ifdef SUPPORT_MT7530E1_PHY_ABIST_MODE
				if (((gswPbusRead(GSW_BASE + 0x7100) & 0x0F00) == 0x0900) || ((gswPbusRead(GSW_BASE + 0x7100) & 0x0F00) == 0x0A00))
				{
					mii.val_out = tcMiiStationRead(mii.phy_id, mii.reg_num);
					copy_to_user(ifr->ifr_data, &mii, sizeof(mii));
				}
				else
#endif
//				if (isRT62806 || isMT7530 || isMT7530ext)		// rt62806 switch
//				if ((swicVendor == SWIC_MT7530) || (swicVendor == SWIC_RT62806))	// MT7530 switch
				{
					mii.val_out = gswPmiRead(mii.phy_id, mii.reg_num);
					copy_to_user(ifr->ifr_data, &mii, sizeof(mii));
				}
				break;
			case RAETH_GSW_PHY_WRITE:
			case RAETH_GSWEXT_PHY_WRITE:
				copy_from_user(&mii, ifr->ifr_data, sizeof(mii));
#ifdef SUPPORT_MT7530E1_PHY_ABIST_MODE
				if (((gswPbusRead(GSW_BASE + 0x7100) & 0x0F00) == 0x0900) || ((gswPbusRead(GSW_BASE + 0x7100) & 0x0F00) == 0x0A00))
				{
					tcMiiStationWrite(mii.phy_id, mii.reg_num, mii.val_in);
				}
				else
#endif
//				if (isRT62806 || isMT7530 || isMT7530ext)		// rt62806 switch
//				if ((swicVendor == SWIC_MT7530) || (swicVendor == SWIC_RT62806))	// MT7530 switch
				{
					gswPmiWrite(mii.phy_id, mii.reg_num, mii.val_in);
				}
				break;

#ifdef MT7530_PHY_VERIFY
			case RAETH_GSWEXT_PHY_READ_45:
				copy_from_user(&mii45, ifr->ifr_data, sizeof(mii45));
				//#define NORMAL_READ		(1<<0)
				//#define POST_READ		(1<<1)
				mii45.miiData = tcMiiExtStationRead_ext(mii45.portAddr, mii45.devAddr, mii45.regAddr, 1);
				copy_to_user(ifr->ifr_data, &mii45, sizeof(mii45));
				break;
			case RAETH_GSWEXT_PHY_WRITE_45:
				copy_from_user(&mii45, ifr->ifr_data, sizeof(mii45));
				tcMiiExtStationWrite_ext(mii45.portAddr, mii45.devAddr, mii45.regAddr, mii45.miiData);
				break;
#endif

		#ifdef TCSUPPORT_MT7530_SWITCH_API
			case RAETH_GSW_CTLAPI:
//				printk("RAETH_GSW_CTLAPI\n");
//				if (isRT62806 || isMT7530 || isMT7530ext)		// rt62806 switch
				if ((swicVendor == SWIC_MT7530) || (swicVendor == SWIC_RT62806))	// MT7530 switch
				{
					macMT7530gswAPIDispatch(ifr);
				}
				break;
		#endif

			default:
				copy_from_user(&mii, ifr->ifr_data, sizeof(mii));
				rc = generic_mii_ioctl(&mac_p->mii_if, &mii, cmd, NULL);
//				rc = generic_mii_ioctl(&mac_p->mii_if, if_mii(ifr), cmd, NULL);
				copy_to_user(ifr->ifr_data, &mii, sizeof(mii));
				break;
		}
	} else {
		rc = -EOPNOTSUPP;
	}

	return rc;
}

static irqreturn_t tc3262_gmac_isr(int irq, void *dev_id)
{
	struct net_device *dev = (struct net_device *) dev_id;
	uint32 reg;

	reg = read_reg_word(CR_GMAC_GISR);
	write_reg_word(CR_GMAC_GISR, reg & read_reg_word(CR_GMAC_GIMR));

	// ----------Packet Received----------------------
	if (reg & (GIMR_RX_DONE | GIMR_RX_RING_FULL |
			   GIMR_RX_FIFO_OVR)) {
    	ledTurnOn(LED_ETHER_ACT_STATUS);
		if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_100MB)
			ledTurnOn(LED_ETHER_100M_ACT_STATUS);
		else
			ledTurnOn(LED_ETHER_10M_ACT_STATUS);
#ifdef TC3262_GMAC_NAPI
		spin_lock(&gimr_lock);

		if (netif_rx_schedule_prep(dev)) {
			write_reg_word(CR_GMAC_GIMR, read_reg_word(CR_GMAC_GIMR) &
					~(GIMR_RX_DONE | GIMR_RX_RING_FULL |
					  GIMR_RX_FIFO_OVR));
			__netif_rx_schedule(dev);
		}

		spin_unlock(&gimr_lock);
#else
    	macRxRingProc(dev, dev->weight);
#endif
		if (reg & GIMR_RX_RING_FULL) {
	        mac_p->macStat.MIB_II.inDiscards++;
			mac_p->macStat.inSilicon.rxOverrunInt++;
			//printk("RX_RING_FULL GISR=%08lx GRXCR=%08lx\n", reg, VPint(CR_GMAC_GRXCR));
		}
		if (reg & GIMR_RX_FIFO_OVR) {
	        mac_p->macStat.MIB_II.inDiscards++;
			mac_p->macStat.inSilicon.rxOverrunInt++;
			//printk("RX_FIFO_OVR GISR=%08lx GRXCR=%08lx\n", reg, VPint(CR_GMAC_GRXCR));
		}
	}

	if (reg & ~(GIMR_RX_DONE | GIMR_RX_RING_FULL | GIMR_RX_FIFO_OVR | GIMR_TX_DONE | GIMR_PHY_INT)) {
		printk("%s err mac_isr GISR=%08lx\n", dev->name, reg);
	}

	return IRQ_HANDLED;
}

#ifdef TC3262_GMAC_NAPI
static int tc3262_gmac_poll(struct net_device *dev, int *budget)
{
	int rx_work_limit = min(dev->quota, *budget);
	int received = 0;
	int n, done;
	unsigned long flags;

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

	spin_lock_irqsave(&gimr_lock, flags);

	__netif_rx_complete(dev);

	write_reg_word(CR_GMAC_GIMR, read_reg_word(CR_GMAC_GIMR) |
		(GIMR_RX_DONE | GIMR_RX_RING_FULL |
		 GIMR_RX_FIFO_OVR));

#if 0
	//solve a lot of out-of-order packet. shnwind 20110323.
	/* check rotting packets */
	if ((macRxRingChk() >= 0) && netif_rx_reschedule(dev, 0)) {

		write_reg_word(CR_GMAC_GIMR, read_reg_word(CR_GMAC_GIMR) &
						~(GIMR_RX_DONE | GIMR_RX_RING_FULL |
						  GIMR_RX_FIFO_OVR));

		spin_unlock_irqrestore(&gimr_lock, flags);

		if (rx_work_limit <= 0) {
			done = 0;
			goto more_work;
		} else {
			goto restart_poll;
		}
	}
#endif
	spin_unlock_irqrestore(&gimr_lock, flags);

more_work:
	*budget -= received;
	dev->quota -= received;

	return done ? 0 : 1;
}
#endif

static int tc3262_gmac_start(struct net_device *dev)
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
#ifdef USE_GLOBAL_PHY_LOCK
	spin_lock_init(&global_phy_lock);
#else
	spin_lock_init(&phy_lock);
	/* frank added 20110802 for rt62806 */
	spin_lock_init(&pbus_lock);
	spin_lock_init(&pmi_lock);
#ifdef SUPPORT_CLAUSE45
	spin_lock_init(&phy_lock_clause45);
#endif
#endif

  	/* Hook up with handlers */
  	dev->get_stats 			= tc3262_gmac_stats;
  	dev->hard_start_xmit 	= tc3262_gmac_tx;
  	dev->open 				= tc3262_gmac_open;
  	dev->stop 				= tc3262_gmac_close;
  	dev->set_multicast_list = tc3262_gmac_set_multicast_list;
  	dev->do_ioctl 			= tc3262_gmac_ioctl;
  	dev->set_mac_address 	= tc3262_gmac_set_macaddr;
#ifdef TC3262_GMAC_NAPI
	dev->poll 				= tc3262_gmac_poll;
#endif
	//dev->weight 			= MAC_RXDESCP_NO>>1;
	dev->weight 			= MAC_NAPI_WEIGHT;

	printk(KERN_INFO
	       "%s: TC3262 GMAC Ethernet address: %02X:%02X:%02X:%02X:%02X:%02X\n",
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

static int proc_txring[TX_QUEUE_NUM];
static char proc_txring_name[TX_QUEUE_NUM][32];

static int __init tc3262_gmac_init(void)
{
  	struct net_device *dev;
  	int err = 0;
	struct proc_dir_entry *eth_proc;
#ifndef TC2031_SUPPORT
	struct proc_dir_entry *port_reverse_proc, *stag_to_vtag_proc;
#endif
	int txq;

	printk(KERN_INFO "%s", version);
	printk("\r\n%s\n", MODULE_VERSION_GMAC);

	dev = alloc_etherdev(sizeof(macAdapter_t));
	if (!dev)
		return -ENOMEM;

	tc3262_gmac_dev = dev;

	dev->irq = MAC_INT;

	mac_p = netdev_priv(dev);

  	dev->init = tc3262_gmac_start;

	err = register_netdev(dev);
	if (err)
		return err;

	eth_proc = create_proc_entry("tc3162/eth_reset", 0, NULL);
	eth_proc->read_proc = eth_reset_read_proc;
	eth_proc->write_proc = eth_reset_write_proc;

	/* frank added */
	if (isRT62806 || isMT7530 || isMT7530ext) {
		eth_proc = create_proc_entry("tc3162/gsw_stats", 0, NULL);
		eth_proc->read_proc = gsw_stats_read_proc;
		eth_proc->write_proc = gsw_stats_write_proc;

		/* wplin added 20120703 */
		eth_proc = create_proc_entry("tc3162/gsw_mib0", 0, NULL);
		eth_proc->read_proc = gsw_mib0_read_proc;
		eth_proc->write_proc = gsw_stats_write_proc;
		eth_proc = create_proc_entry("tc3162/gsw_mib1", 0, NULL);
		eth_proc->read_proc = gsw_mib1_read_proc;
		eth_proc->write_proc = gsw_stats_write_proc;
		eth_proc = create_proc_entry("tc3162/gsw_mib2", 0, NULL);
		eth_proc->read_proc = gsw_mib2_read_proc;
		eth_proc->write_proc = gsw_stats_write_proc;
		eth_proc = create_proc_entry("tc3162/gsw_mib3", 0, NULL);
		eth_proc->read_proc = gsw_mib3_read_proc;
		eth_proc->write_proc = gsw_stats_write_proc;
		eth_proc = create_proc_entry("tc3162/gsw_mib4", 0, NULL);
		eth_proc->read_proc = gsw_mib4_read_proc;
		eth_proc->write_proc = gsw_stats_write_proc;
		eth_proc = create_proc_entry("tc3162/gsw_mib5", 0, NULL);
		eth_proc->read_proc = gsw_mib5_read_proc;
		eth_proc->write_proc = gsw_stats_write_proc;
		eth_proc = create_proc_entry("tc3162/gsw_mib6", 0, NULL);
		eth_proc->read_proc = gsw_mib6_read_proc;
		eth_proc->write_proc = gsw_stats_write_proc;
	}

#ifdef RT62806_VERIFY
	eth_proc = create_proc_entry("tc3162/gsw_test_stag", 0, NULL);
	eth_proc->write_proc = gsw_stag_write_proc;
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
#ifdef LOOPBACK_SUPPORT
	eth_proc = create_proc_entry("tc3162/eth_loopback", 0, NULL);
	eth_proc->read_proc = eth_loopback_read_proc;
	eth_proc->write_proc = eth_loopback_write_proc;

	eth_proc = create_proc_entry("tc3162/eth_loopback_test", 0, NULL);
	eth_proc->read_proc = eth_loopback_test_read_proc;
	eth_proc->write_proc = eth_loopback_test_write_proc;
#endif

  	create_proc_read_entry("tc3162/eth_link_st", 0, NULL, eth_link_st_proc, NULL);
  	create_proc_read_entry("tc3162/eth_reg", 0, NULL, eth_reg_dump_proc, NULL);
#ifndef TC2031_SUPPORT
#if !defined(TCSUPPORT_CT)
	port_reverse_proc = create_proc_entry("tc3162/port_reverse", 0, NULL);
	port_reverse_proc->read_proc = port_reverse_read_proc;
	port_reverse_proc->write_proc = port_reverse_write_proc;
	stag_to_vtag_proc = create_proc_entry("tc3162/stag_to_vtag", 0, NULL);
	stag_to_vtag_proc->read_proc = stag_to_vtag_read_proc;
	stag_to_vtag_proc->write_proc = stag_to_vtag_write_proc;
#endif
#endif
	for (txq = 0; txq < TX_QUEUE_NUM; txq++) {
		proc_txring[txq] = txq;
		sprintf(proc_txring_name[txq], "tc3162/eth_txring%d", txq);
		create_proc_read_entry(proc_txring_name[txq], 0, NULL, eth_txring_dump_proc, &proc_txring[txq]);
	}
  	create_proc_read_entry("tc3162/eth_rxring", 0, NULL, eth_rxring_dump_proc, NULL);

	#ifdef TCPHY_SUPPORT
#if !defined(TCSUPPORT_CT)
  	create_proc_read_entry("tc3162/eth_port_status", 0, NULL, eth_port_stat_read_proc, NULL);
#endif
	#endif
	#ifdef TC_CONSOLE_ENABLE
	create_tcconsole_proc();
	rcu_assign_pointer(send_uart_msg, uart_msg_to_tcconsole);
	#endif
	return 0;
}
static void __exit tc3262_gmac_exit(void)
{
	int txq;

	macReset();

	remove_proc_entry("tc3162/eth_reset", 0);
#ifdef TCPHY_SUPPORT
	remove_proc_entry("tc3162/eth_port_status", 0);
#endif
	remove_proc_entry("tc3162/eth_stats", 0);
#ifdef TCPHY_4PORT
	remove_proc_entry("tc3162/mac_esd_check", 0);
#if !defined(TCSUPPORT_CT)
#if 1//def VPORT
	remove_proc_entry("tc3162/vport_enable", 0);
#endif
#endif
#endif
#ifdef LOOPBACK_SUPPORT
	remove_proc_entry("tc3162/eth_loopback", 0);
	remove_proc_entry("tc3162/eth_loopback_test", 0);
#endif
   	remove_proc_entry("tc3162/eth_link_st", 0);
   	remove_proc_entry("tc3162/eth_reg", 0);
#ifndef TC2031_SUPPORT
#if !defined(TCSUPPORT_CT)
	remove_proc_entry("tc3162/port_reverse", 0);
	remove_proc_entry("tc3162/stag_to_vtag", 0);
#endif
#endif
	for (txq = 0; txq < TX_QUEUE_NUM; txq++) {
		remove_proc_entry(proc_txring_name[txq], NULL);
	}
   	remove_proc_entry("tc3162/eth_rxring", 0);

	macDrvDescripReset(mac_p);

	unregister_netdev(tc3262_gmac_dev);
	if (mac_p->macTxMemPool_p) {
#ifdef CONFIG_TC3162_DMEM
		if (is_sram_addr(mac_p->macTxMemPool_p))
			free_sram(mac_p->macTxMemPool_p, sizeof(macTxMemPool_t));
		else
#endif
		dma_free_coherent(NULL, sizeof(macTxMemPool_t), mac_p->macTxMemPool_p, mac_p->macTxMemPool_phys_p);
	}
	if (mac_p->macRxMemPool_p) {
#ifdef CONFIG_TC3162_DMEM
		if (is_sram_addr(mac_p->macRxMemPool_p))
			free_sram(mac_p->macRxMemPool_p, sizeof(macRxMemPool_t));
		else
#endif
		dma_free_coherent(NULL, sizeof(macRxMemPool_t), mac_p->macRxMemPool_p, mac_p->macRxMemPool_phys_p);
	}
	free_netdev(tc3262_gmac_dev);
	#ifdef TCPHY_SUPPORT
	/*unregister ci command */
	cmd_unregister("tcephydbg");
	#endif
	#ifdef TC_CONSOLE_ENABLE
	delete_tcconsole_proc();
	rcu_assign_pointer(send_uart_msg, NULL);
	#endif
}

/* Register startup/shutdown routines */
module_init(tc3262_gmac_init);
module_exit(tc3262_gmac_exit);
MODULE_LICENSE("Proprietary");
