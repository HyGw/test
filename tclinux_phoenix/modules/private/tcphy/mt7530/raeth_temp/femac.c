/*
** $Id: //BBN_Linux/Branch/Branch_for_MT7520_20120528/tclinux_phoenix/modules/private/raeth/femac.c#2 $
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
** $Log: femac.c,v $
** Revision 1.7  2011/10/21 06:14:05  lino
** add RT63365 ASIC support: hwnat
**
** Revision 1.6  2011/10/12 08:44:19  lino
** add RT63365 ASIC support: hwnat & qos support for ethernet WAN
**
** Revision 1.5  2011/10/12 05:29:01  shnwind
** RT63365 ether wan compiler
**
** Revision 1.4  2011/10/06 07:39:24  shnwind
** RT63365 etherwan support
**
** Revision 1.3  2011/10/06 07:27:55  shnwind
** RT63365 etherwan support
**
** Revision 1.2  2011/10/06 04:29:35  shnwind
** RT63365 etherwan support
**
** Revision 1.1  2011/09/23 02:07:55  shnwind
** Add rt63365 support
**
** Revision 1.1.2.11  2011/08/02 06:10:01  lino
** add RT63365 support: set port 5 giga port RX clock to degree 0
**
** Revision 1.1.2.10  2011/06/16 05:07:48  lino
** add RT63365 support: rewrite mac reset function
**
** Revision 1.1.2.9  2011/06/16 03:21:24  lino
** add RT63365 support
**
** Revision 1.1.2.8  2011/06/14 11:47:40  lino
** add RT63365 support: add 6 port phy board support
**
** Revision 1.1.2.7  2011/05/26 08:51:44  lino
** add RT63365 support: special tag
**
** Revision 1.1.2.6  2011/04/28 04:42:11  lino
** add RT63365 support
**
** Revision 1.1.2.5  2011/04/22 02:55:22  lino
** add RT63365 support
**
** Revision 1.1.2.4  2011/04/21 12:51:03  lino
** add RT63365 support
**
** Revision 1.1.2.3  2011/04/20 10:53:33  lino
** add RT63365 support
**
** Revision 1.1.2.2  2011/04/20 09:58:41  lino
** add RT63365 support
**
** Revision 1.1.2.1  2011/04/20 02:39:15  lino
** add RT63365 support
**
 */

#define TC3262_GMAC_NAPI

#define TC3262_GMAC_SKB_RECYCLE

#define DRV_NAME	"femac"
#ifdef TC3262_GMAC_NAPI
#define DRV_VERSION	"1.00-NAPI"
#else
#define DRV_VERSION	"1.00"
#endif
#define DRV_RELDATE	"29.Mar.2011"

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
#include <linux/mtd/mtd.h>
#include <linux/mtd/rt_flash.h>
#include <asm/spram.h>
#include <asm/io.h>
#include <asm/tc3162/tc3162.h>
#include <asm/tc3162/ledcetrl.h>
#include <asm/tc3162/TCIfSetQuery_os.h>

#ifdef TCPHY_SUPPORT
#include <asm/tc3162/cmdparse.h>
#include "../tcphy/tcetherphy.h"
#include "../tcphy/tcswitch.h"
#endif
#ifdef TCSUPPORT_AUTOBENCH
#include "../auto_bench/autobench.h"
#endif

#ifdef TCSUPPORT_RA_HWNAT
#include <linux/foe_hook.h>
#endif

#include "../tcphy/tcconsole.h"
#include "femac.h"

#ifdef TCSUPPORT_MT7530_SWITCH_API
#include "../tcphy/mtkswitch_api.h"
#include "../tcphy/mtkswitch_api_def.h"
//extern int macMT7530gswAPIDispatch(struct ifreq *ifr);
#endif

#ifdef __BIG_ENDIAN
#define FE_BYTE_SWAP
#endif

#define RAETH_CHECKSUM_OFFLOAD

#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE)
#if defined(WAN2LAN) || defined(CONFIG_TC3162_ADSL)
#define VLAN_TAG_USED 0
#else
#define VLAN_TAG_USED 1
#endif
#else
#define VLAN_TAG_USED 0
#endif

#define RX_BUF_LEN 			(2048 - NET_SKB_PAD - 64 - (sizeof(struct skb_shared_info)))
#define RX_MAX_PKT_LEN 		1536

#undef CONFIG_TC3162_DMEM

/************************************************************************
*                          C O N S T A N T S
*************************************************************************
*/
//#define LOOPBACK_SUPPORT

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
#define LOOPBACK_EXT		(1<<12)

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

#ifndef TCSUPPORT_QOS
#define QOS_REMARKING  1
#endif
#define TCSUPPORT_HW_QOS

#ifdef QOS_REMARKING
#define QOS_REMARKING_MASK    0x00000007
#define QOS_REMARKING_FLAG    0x00000001
//#define QOS_DMAWRR_USERDEFINE  0x01
#endif
#if defined (QOS_REMARKING) || defined(TCSUPPORT_HW_QOS)
#define QOS_DMAWRR_USERDEFINE 0x1
#define PTQCR_WRR_EN			(1<<4)
#define PTQCR_WRR_SELECT 	 	(1<<6)
#endif

#ifdef TCSUPPORT_QOS
#define		QOS_FILTER_MARK		0xf0
#define 	QOS_HH_PRIORITY		0x10
#define 	QOS_H_PRIORITY		0x20
#define 	QOS_M_PRIORITY		0x30
#define		QOS_L_PRIORITY		0x40

#define		NULLQOS				-1
#define 	QOS_SW_PQ			0	//will use hw at the same time
#define		QOS_SW_WRR			1
#define		QOS_SW_CAR			2
#define 	QOS_HW_WRR			3
#define		QOS_HW_PQ			4

#endif

/************************************************************************
*                            M A C R O S
*************************************************************************
*/
#ifdef TCSUPPORT_RA_HWNAT
#define FOE_MAGIC_GE		    0x7275
#define FOE_MAGIC_PPE		    0x7276
#endif
#define CONFIG_8021P_REMARK 1
#if defined(TCSUPPORT_WAN_ETHER) && defined(CONFIG_8021P_REMARK)
#define QOS_8021p_MARK			0x0F00 	/* 8~11 bits used for 802.1p */
#define QOS_8021P_0_MARK		0x08	/* default mark is zero */
#define VLAN_HLEN				4
#define VLAN_ETH_ALEN			6
#endif

/************************************************************************
*                         D A T A   T Y P E S
*************************************************************************
*/

/************************************************************************
*              F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
uint32 gswPbusRead(uint32 pbus_addr);
static int gswPbusWrite(uint32 pbus_addr, uint32 pbus_data);

// frank add for generate 1588 protocol packet command
extern void getnstimeofday(struct timespec *tv);	//MTK20120829_MT7530_1588pkt_generation

static irqreturn_t tc3262_gmac_isr(int irq, void *dev_id);
int tc3262_gmac_tx(struct sk_buff *skb, struct net_device *dev);
static void macDrvStart(void);
static void macDrvStop(void);
#ifdef LOOPBACK_SUPPORT
static void macPhyLoopback(macAdapter_t *mac_p, int enable);
static void dump_skb(struct sk_buff *skb);
#endif
#if defined(TCSUPPORT_WAN_ETHER)
static int eth_wan_link_st_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data);
static int tc3262_gmac_wan_start(struct net_device *dev);
static struct net_device_stats *tc3262_gmac_wan_stats(struct net_device *dev);
static int tc3262_gmac_wan_open(struct net_device *dev);
static int tc3262_gmac_wan_close(struct net_device *dev);
static void tc3262_gmac_wan_set_multicast_list(struct net_device *dev);
static int tc3262_gmac_wan_set_macaddr(struct net_device *dev, void *p);
static int tc3262_gmac_wan_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd);
int tc3262_gmac_wan_tx(struct sk_buff *skb, struct net_device *dev);
#endif
extern int (*ra_sw_nat_hook_rx)(struct sk_buff *skb);
extern int (*ra_sw_nat_hook_tx)(struct sk_buff *skb, int gmac_no);

#if defined(TCPHY_SUPPORT)
extern int gsw_check_read_proc(char *buf, char **start, off_t off, int count,
							int *eof, void *data);

extern int gsw_check_write_proc(struct file *file, const char *buffer,
							  unsigned long count, void *data);
#endif

extern int ethernet_portmap_read_proc(char *buf, char **start, off_t off, int count,
					                  int *eof, void *data);
extern int stag_to_vtag_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data);
extern int stag_to_vtag_write_proc(struct file *file, const char *buffer, unsigned long count, void *data);

#if defined(SQA_VERIFY)
//MTK20120829_MT7530_1588pkt_generation, Start[
static int gen_1588_pkt_write_proc(struct file *file, const char *buffer, unsigned long count, void *data);
//MTK20120829_MT7530_1588pkt_generation, ]End
#endif
/************************************************************************
*                        P U B L I C   D A T A
*************************************************************************
*/

#ifdef TCSUPPORT_QOS
static int qos_flag = NULLQOS;
#endif

//#ifdef QOS_REMARKING  /*Rodney_20090724*/
#if defined(QOS_REMARKING) || defined(TCSUPPORT_HW_QOS)
static int qos_wrr_info[5] = {0};
static int max_prio = 3;
static uint8 qos_wrr_user = 0x00;
#endif

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
extern int wan_port_id;
/* frankliao added 20101215 */
extern unsigned long flash_base;
extern unsigned int (*ranand_read_byte)(unsigned long long);

/************************************************************************
*                       P R I V A T E   D A T A
*************************************************************************
*/

/* Device data */
struct net_device *tc3262_gmac_dev;
#if defined(TCSUPPORT_WAN_ETHER)
struct net_device *tc3262_gmac_wan_dev = NULL;
macAdapter_t *mac_wan_p = NULL;
#endif
/* Timer */
static struct timer_list eth_timer;
static struct timer_list eth_poll_timer;
/* phy lock */
static spinlock_t phy_lock;

macAdapter_t *mac_p = NULL;
static macPhyLinkProfile_t enetPhyLinkProfile;

static phyDeviceList_t phyDeviceList[] = {
	{ 0x0013, "LXT972" },
	{ 0x0022, "AC101L" },
	{ 0x0243, "IP101" },
	{ 0x8201, "RTL8201" },
	{ 0x001c, "RTL8212" },
	{ 0x03a2, "TC2031" },
  { 0x0007, "MT7530" }
};

static uint8 macInitialized = 0;
#ifdef LOOPBACK_SUPPORT
static uint32 macLoopback = 0;
static uint32 macRxLoopback;
#endif

uint8 def_mac_addr[] = {0x00, 0x00, 0xaa, 0xbb, 0xcc, 0xff};

uint8 swicVendor = 0;

static DEFINE_SPINLOCK(gimr_lock);

#ifdef WAN2LAN
/* ------ xyzhu_091105:special tag relation data start ---------- */
extern uint8 macSTagFlag;
/* ------ xyzhu_091105:special tag relation data end ----------- */
extern int masko_on_off;
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
	return skbmgr_dev_alloc_skb2k();
	//return dev_alloc_skb(RX_BUF_LEN);
}

void tcMiiStationWrite(uint32 enetPhyAddr, uint32 phyReg, uint32 miiData)
{
	uint32 reg;
	uint32 cnt=1000;
	u32 GSW_CFG_PIAC_addr;

//	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530FPGA) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	GSW_CFG_PIAC_addr = (isMT7530) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;

	spin_lock_bh(&phy_lock);
	do {
		reg=read_reg_word (GSW_CFG_PIAC_addr);
		cnt--;
	} while (reg & PHY_ACS_ST);

	reg = PHY_ACS_ST | (MDIO_ST_START << MDIO_ST_SHIFT) | (MDIO_CMD_WRITE << MDIO_CMD_SHIFT) |
		(enetPhyAddr << MDIO_PHY_ADDR_SHIFT) | (phyReg << MDIO_REG_ADDR_SHIFT) |
		(miiData & MDIO_RW_DATA);
	write_reg_word (GSW_CFG_PIAC_addr, reg);

	cnt = 1000;
	do {
		//pause(1);
		reg=read_reg_word (GSW_CFG_PIAC_addr);
		cnt--;
	} while (reg & PHY_ACS_ST);
	spin_unlock_bh(&phy_lock);
	if (cnt==0)
		printk("EER: tcMiiStationWrite timeout!\n");
	#ifdef TCPHY_4PORT
	/*TC2206 switch IC can't be direct to do PHY reset, we must
	 * avoid ESD software patch be trigger.
	 */
	refillPhyDefVal(enetPhyAddr, phyReg, miiData);
	#endif
}
EXPORT_SYMBOL(tcMiiStationWrite);

uint32 tcMiiStationRead(uint32 enetPhyAddr, uint32 phyReg)
{
	uint32 reg;
	uint32 cnt=1000;
	u32 GSW_CFG_PIAC_addr;

//	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530FPGA) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	GSW_CFG_PIAC_addr = (isMT7530) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;

	spin_lock_bh(&phy_lock);
	do {
		reg=read_reg_word (GSW_CFG_PIAC_addr);
		cnt--;
	} while (reg & PHY_ACS_ST);

	reg = PHY_ACS_ST | (MDIO_ST_START << MDIO_ST_SHIFT) | (MDIO_CMD_READ << MDIO_CMD_SHIFT) |
		(enetPhyAddr << MDIO_PHY_ADDR_SHIFT) | (phyReg << MDIO_REG_ADDR_SHIFT);
	write_reg_word (GSW_CFG_PIAC_addr, reg);

	cnt = 1000;
	do {
		//pause(1);
		reg=read_reg_word (GSW_CFG_PIAC_addr);
		cnt--;
	} while (reg & PHY_ACS_ST);
	reg = reg & MDIO_RW_DATA;
	spin_unlock_bh(&phy_lock);
	printk("%s!\n",__func__);
	if (cnt == 0)
		printk("EER: tcMiiStationRead timeout!\n");

	return reg;
}
EXPORT_SYMBOL(tcMiiStationRead);

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

#define SUPPORT_CLAUSE45_API
#ifdef SUPPORT_CLAUSE45_API
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
**      	Here_20090625
**____________________________________________________________________________
*/
int tcMiiExtStationFillAddr(uint32 portAddr, uint32 devAddr, uint32 regAddr)
{
	uint32 value, cnt;
	u32 GSW_CFG_PIAC_addr;

//	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530FPGA) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	GSW_CFG_PIAC_addr = (isMT7530) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	portAddr &= 0x1F;
	devAddr &= 0x1F;

	spin_lock_bh(&phy_lock);
	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));

	value = PHY_ACS_ST | (MDIO_CL45_ST_START << MDIO_ST_SHIFT) | (MDIO_CL45_CMD_ADDR << MDIO_CMD_SHIFT) |
		(portAddr << MDIO_PHY_ADDR_SHIFT) | (devAddr << MDIO_REG_ADDR_SHIFT) |
		(regAddr & MDIO_RW_DATA);
	write_reg_word (GSW_CFG_PIAC_addr, value);

	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));
	spin_unlock_bh(&phy_lock);

	if (cnt==0){
		printk("EER: tcMiiExtStationFillAddr timeout!\n");
		return -1;
	}
	return 0;
}/*end tcMiiExtStationFillAddr*/
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
**      	Here_20090625
**____________________________________________________________________________
*/
void tcMiiExtStationWrite(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint32 miiData)
{
	uint32 value, cnt;
	u32 GSW_CFG_PIAC_addr;

//	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530FPGA) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	GSW_CFG_PIAC_addr = (isMT7530) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	portAddr &= 0x1F;
	devAddr &= 0x1F;

	tcMiiExtStationFillAddr(devAddr, portAddr, regAddr);

	spin_lock_bh(&phy_lock);
	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));

	value = PHY_ACS_ST | (MDIO_CL45_ST_START << MDIO_ST_SHIFT) | (MDIO_CL45_CMD_WRITE << MDIO_CMD_SHIFT) |
		(portAddr << MDIO_PHY_ADDR_SHIFT) | (devAddr << MDIO_REG_ADDR_SHIFT) |
		(miiData & MDIO_RW_DATA);
	write_reg_word(GSW_CFG_PIAC_addr, value);

	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));
	spin_unlock_bh(&phy_lock);

	if (cnt==0){
		printk("EER: tcMiiStationWrite timeout!\n");
	}
}
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
**      	Here_20090625
**____________________________________________________________________________
*/
uint32 tcMiiExtStationRead(uint32 portAddr, uint32 devAddr, uint32 regAddr, uint8 op)
{
	uint32 value, cnt;
	u32 GSW_CFG_PIAC_addr;

//	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530FPGA) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	GSW_CFG_PIAC_addr = (isMT7530) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	portAddr &= 0x1F;
	devAddr &= 0x1F;

	if (op != POST_READ){
		/*POST READ command only to fill the address once, the address will be
		increased once automatically.*/
		tcMiiExtStationFillAddr(devAddr, portAddr, regAddr);
	}

	spin_lock_bh(&phy_lock);
	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));

	if(op==POST_READ){
		value = PHY_ACS_ST | (MDIO_CL45_ST_START << MDIO_ST_SHIFT) | (MDIO_CL45_CMD_READ << MDIO_CMD_SHIFT) |
			(portAddr << MDIO_PHY_ADDR_SHIFT) | (devAddr << MDIO_REG_ADDR_SHIFT);

	}
	else{/*Normal Read*/
		value = PHY_ACS_ST | (MDIO_CL45_ST_START << MDIO_ST_SHIFT) | (MDIO_CL45_CMD_READ << MDIO_CMD_SHIFT) |
			(portAddr << MDIO_PHY_ADDR_SHIFT) | (devAddr << MDIO_REG_ADDR_SHIFT);
	}
	write_reg_word (GSW_CFG_PIAC_addr, value);

	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));
	spin_unlock_bh(&phy_lock);

	if (cnt==0){
		printk("EER: tcMiiExtStationRead timeout!\n");
	}

	return (value & MDIO_RW_DATA);
}/*end tcMiiExtStationRead*/
EXPORT_SYMBOL(tcMiiExtStationRead);

void tcMiiStationCL45Address(uint32 portAddr, uint32 devAddr, uint32 regAddr)
{
	uint32 value, cnt;
	u32 GSW_CFG_PIAC_addr;

	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530FPGA) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	portAddr &= 0x1F;
	devAddr &= 0x1F;

	spin_lock_bh(&phy_lock);
	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));

	value = PHY_ACS_ST | (MDIO_CL45_ST_START << MDIO_ST_SHIFT) | (MDIO_CL45_CMD_ADDR << MDIO_CMD_SHIFT) |
		(portAddr << MDIO_PHY_ADDR_SHIFT) | (devAddr << MDIO_REG_ADDR_SHIFT) |
		(regAddr & MDIO_RW_DATA);
	write_reg_word (GSW_CFG_PIAC_addr, value);

	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));
	spin_unlock_bh(&phy_lock);
	if (cnt == 0) printk("ERR: %s() timeout!\n", __FUNCTION__);
	#if (0)
	#ifdef TCPHY_4PORT
	/*TC2206 switch IC can't be direct to do PHY reset, we must
	 * avoid ESD software patch be trigger.
	 */
	refillPhyDefVal(enetPhyAddr, phyReg, miiData);
	#endif
	#endif
}
EXPORT_SYMBOL(tcMiiStationCL45Address);

void tcMiiStationCL45Write(uint32 portAddr, uint32 devAddr, uint32 miiData)
{
	uint32 value, cnt;
	u32 GSW_CFG_PIAC_addr;

	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530FPGA) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	portAddr &= 0x1F;
	devAddr &= 0x1F;

	spin_lock_bh(&phy_lock);
	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));

	value = PHY_ACS_ST | (MDIO_CL45_ST_START << MDIO_ST_SHIFT) | (MDIO_CL45_CMD_WRITE << MDIO_CMD_SHIFT) |
		(portAddr << MDIO_PHY_ADDR_SHIFT) | (devAddr << MDIO_REG_ADDR_SHIFT) |
		(miiData & MDIO_RW_DATA);
	write_reg_word(GSW_CFG_PIAC_addr, value);

	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt--;
	} while ((value & PHY_ACS_ST) && (cnt != 0));
	spin_unlock_bh(&phy_lock);
	if (cnt == 0) printk("ERR: %s() timeout!\n", __FUNCTION__);
	#if (0)
	#ifdef TCPHY_4PORT
	/*TC2206 switch IC can't be direct to do PHY reset, we must
	 * avoid ESD software patch be trigger.
	 */
	refillPhyDefVal(enetPhyAddr, phyReg, miiData);
	#endif
	#endif
}
EXPORT_SYMBOL(tcMiiStationCL45Write);

uint32 tcMiiStationCL45Read(uint32 portAddr, uint32 devAddr)
{
	uint32 value, cnt;
	u32 GSW_CFG_PIAC_addr;

	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530FPGA) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	portAddr &= 0x1F;
	devAddr &= 0x1F;

	spin_lock_bh(&phy_lock);
	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));

	value = PHY_ACS_ST | (MDIO_CL45_ST_START << MDIO_ST_SHIFT) | (MDIO_CL45_CMD_READ << MDIO_CMD_SHIFT) |
		(portAddr << MDIO_PHY_ADDR_SHIFT) | (devAddr << MDIO_REG_ADDR_SHIFT);
	write_reg_word(GSW_CFG_PIAC_addr, value);

	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));
	spin_unlock_bh(&phy_lock);
	if (cnt == 0) printk("ERR: %s() timeout!\n", __FUNCTION__);

	return (value & MDIO_RW_DATA);
}
EXPORT_SYMBOL(tcMiiStationCL45Read);

uint32 tcMiiStationCL45PostReadIncAddr(uint32 portAddr, uint32 devAddr)
{
	uint32 value, cnt;
	u32 GSW_CFG_PIAC_addr;

	GSW_CFG_PIAC_addr = (isMT7530 || isMT7530FPGA) ? (GSW_CFG_BASE + 0x001C) : GSW_CFG_PIAC;
	portAddr &= 0x1F;
	devAddr &= 0x1F;

	spin_lock_bh(&phy_lock);
	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));

	value = PHY_ACS_ST | (MDIO_CL45_ST_START << MDIO_ST_SHIFT) | (MDIO_CL45_CMD_POSTREAD_INCADDR << MDIO_CMD_SHIFT) |
		(portAddr << MDIO_PHY_ADDR_SHIFT) | (devAddr << MDIO_REG_ADDR_SHIFT);
	write_reg_word(GSW_CFG_PIAC_addr, value);

	cnt = 10000;
	do {
		//pause(1);
		value = read_reg_word(GSW_CFG_PIAC_addr);
		cnt --;
	} while ((value & PHY_ACS_ST) && (cnt != 0));
	spin_unlock_bh(&phy_lock);
	if (cnt == 0) printk("ERR: %s() timeout!\n", __FUNCTION__);

	return (value & MDIO_RW_DATA);
}
EXPORT_SYMBOL(tcMiiStationCL45PostReadIncAddr);

void miiExtStationFillAddr(macAdapter_t *mac_p, uint32 devAddr, uint32 regAddr)
{
	tcMiiExtStationFillAddr(mac_p->enetPhyAddr, devAddr, regAddr);
}

void miiExtStationWrite(macAdapter_t *mac_p, uint32 devAddr, uint32 regAddr, uint32 miiData)
{
	tcMiiExtStationWrite(mac_p->enetPhyAddr, devAddr, regAddr, miiData);
}

uint32 miiExtStationRead(macAdapter_t *mac_p, uint32 devAddr, uint32 regAddr, uint8 op)
{
	return tcMiiExtStationRead(mac_p->enetPhyAddr, devAddr, regAddr, op);
}
#endif 	// Clause 45

static int mdio_read(struct net_device *dev, int phy_id, int reg_num)
{
	return tcMiiStationRead(phy_id, reg_num);
}

static void mdio_write(struct net_device *dev, int phy_id, int reg_num, int val)
{
	tcMiiStationWrite(phy_id, reg_num, val);
}

//** MTK120625 start,
static spinlock_t pbus_lock;
/* frank modify for rt62806 */
uint32
gswPbusRead(uint32 pbus_addr)
{
	uint32 pbus_data;

	uint32 phyaddr;
	uint32 reg;
	uint32 value;

	spin_lock_bh(&pbus_lock);

	phyaddr = 31;
	// 1. write high-bit page address
	reg = 31;
	value = (pbus_addr >> 6);
  	tcMiiStationWrite(phyaddr, reg, value);
	//mdelay(5);
	//DBG_PRINTF("1. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//printk("1. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	// 2. read low DWord
	reg = (pbus_addr>>2) & 0x000f;
	value = tcMiiStationRead(phyaddr, reg);
	//mdelay(5);
	//DBG_PRINTF("2. miir phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//printk("2. miir phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	pbus_data = value;

	// 3. read high DWord
	reg = 16;
		value = tcMiiStationRead(phyaddr, reg);
	//mdelay(5);
	//DBG_PRINTF("3. miir phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//printk("3. miir phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	pbus_data = (pbus_data) | (value<<16);

	//DBG_PRINTF("# pbus read addr=0x%04x data=0x%04x_%04x\r\n", (pbus_addr&0xfffc), (pbus_data>>16), (pbus_data&0xffff));
	//printk("# pbus read addr=0x%04x data=0x%04x_%04x\r\n", (pbus_addr&0xfffc), (pbus_data>>16), (pbus_data&0xffff));
//	printk("gswPbusRead read data:\n");

//	printk("pbus_data: %x\n", pbus_data);

	spin_unlock_bh(&pbus_lock);
	return pbus_data;
} /* end frank modify for rt62806 */

/* frank modify for rt62806 */
static int
gswPbusWrite(uint32 pbus_addr, uint32 pbus_data)
{
	uint32 phyaddr;
	uint32 reg;
	uint32 value;

	spin_lock_bh(&pbus_lock);

	phyaddr = 31;

	// 1. write high-bit page address
	reg = 31;
	value = (pbus_addr >> 6);
	tcMiiStationWrite(phyaddr, reg, value);
	//mdelay(5);

	//printk("1. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//DBG_PRINTF("1. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	// 2. write low DWord
	reg = (pbus_addr>>2) & 0x000f;
	value = pbus_data & 0xffff;
	tcMiiStationWrite(phyaddr, reg, value);
	//mdelay(5);
	//printk("2. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//DBG_PRINTF("2. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	// 3. write high DWord
	reg = 16;
	value = (pbus_data>>16) & 0xffff;
	tcMiiStationWrite(phyaddr, reg, value);
	//mdelay(5);
	//printk("3. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);
	//DBG_PRINTF("3. miiw phyaddr=%2d reg=%2d value=%04x\r\n", phyaddr, reg, value);

	//DBG_PRINTF("# pbus write addr=0x%04x data=0x%04x_%04x\r\n", (pbus_addr&0xfffc), (pbus_data>>16),(pbus_data&0xffff));
	spin_unlock_bh(&pbus_lock);
  	return 0;
} /* end frank modify for rt62806 */

//PHY2 read/write
uint32 gswPmiRead(uint32 phy_addr, uint32 phy_reg)
{
	uint32 pbus_addr;
	uint32 pbus_data;
	uint32 phy_data;
	uint32 phy_acs_st;
//	uint32 max_wait_cnt = 1000;

	pbus_addr = 0x701c;
	// b31	- phy access 1:start&busy, 0:complete&idle
	// b29:25 - mdio phy reg addr
	// b24:20 - mdio phy addr
	// b19:18 - 2'b01: write, 2'b10: read
	// b17:16 - start field, always 2'b01
	// b15:0	- data

	phy_addr = phy_addr & 0x1f;
	phy_reg  = phy_reg & 0x1f;

	// 1. write phy_addr & phy_reg
	pbus_data = 0x80090000; // read
	pbus_data = pbus_data | (phy_addr<<20);
	pbus_data = pbus_data | (phy_reg<<25);

	gswPbusWrite(pbus_addr,pbus_data);

	// 2. check phy_acs_st
	phy_acs_st = 1;
	while (phy_acs_st) {
		pbus_data = gswPbusRead(pbus_addr);
		phy_acs_st = (pbus_data>>31) & 0x1;
	}

	// 3. return data
	phy_data = pbus_data & 0xffff;
	return phy_data;
} /* end frank modify for rt62806 */

uint32 gswPmiWrite(uint32 phy_addr, uint32 phy_reg, uint32 phy_data)
{
	uint32 pbus_addr;
	uint32 pbus_data;
//	uint32 phy_acs_st;

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

	gswPbusWrite(pbus_addr,pbus_data);
//  DBG_PRINTF(" pbus write addr=0x%04x data=0x%08x\r\n", pbus_addr, pbus_data);

	return 0;
} /* end frank modify for rt62806 */

//PHY2 read/write
uint32
gswPHYRead2(uint32 phy_addr, uint32 phy_reg)
	{
	  uint32 pbus_addr;
	  uint32 pbus_data;
	  uint32 phy_data;
	  uint32 phy_acs_st;
//	  uint32 max_wait_cnt = 1000;

	  pbus_addr = 0x701c;
	  // b31	- phy access 1:start&busy, 0:complete&idle
	  // b29:25 - mdio phy reg addr
	  // b24:20 - mdio phy addr
	  // b19:18 - 2'b01: write, 2'b10: read
	  // b17:16 - start field, always 2'b01
	  // b15:0	- data

	  phy_addr = phy_addr & 0x1f;
	  phy_reg  = phy_reg & 0x1f;

	  // 1. write phy_addr & phy_reg
	  pbus_data = 0x80090000; // read
	  pbus_data = pbus_data | (phy_addr<<20);
	  pbus_data = pbus_data | (phy_reg<<25);

	  gswPbusWrite(pbus_addr,pbus_data);

	  // 2. check phy_acs_st

	  phy_acs_st = 1;
	  while (phy_acs_st) {
		pbus_data = gswPbusRead(pbus_addr);
		phy_acs_st = (pbus_data>>31) & 0x1;
	  }

	  // 3. return data
	  phy_data = pbus_data & 0xffff;
      return phy_data;
} /* end frank modify for rt62806 */

static int
gswPHYWrite2(uint32 phy_addr, uint32 phy_reg, uint32 phy_data)
{
  uint32 pbus_addr;
  uint32 pbus_data;
//  uint32 phy_acs_st;

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

  gswPbusWrite(pbus_addr,pbus_data);
//  DBG_PRINTF(" pbus write addr=0x%04x data=0x%08x\r\n", pbus_addr, pbus_data);

	return 0;
} /* end frank modify for rt62806 */
//** MTK120625, end

void macIMRMask(void)
{
	uint32 reg = 0;

	//reg |= RX_COHERENT | RX_DLY_INT | TX_COHERENT | TX_DLY_INT |
	//		RX_DONE_INT1 | RX_DONE_INT0 |
	//		TX_DONE_INT3 | TX_DONE_INT2 | TX_DONE_INT1 | TX_DONE_INT0;
	reg |= RX_COHERENT | RX_DLY_INT | TX_COHERENT |
			RX_DONE_INT1 | RX_DONE_INT0;
	write_reg_word(INT_MASK, reg);
}

void macReset(void)
{
	uint32 reg;

	/* reset ethernet phy, ethernet switch, frame engine */
	reg = read_reg_word(CR_RSTCTRL2);
	reg |= (EPHY_RST | ESW_RST | FE_RST);
	write_reg_word(CR_RSTCTRL2, reg);

	/* de-assert reset ethernet phy, ethernet switch, frame engine */
	reg = read_reg_word(CR_RSTCTRL2);
	reg &= ~(EPHY_RST | ESW_RST | FE_RST);
	write_reg_word(CR_RSTCTRL2, reg);
}

void macSetIntConf(void)
{
#ifndef TC3262_GMAC_NAPI
	uint32 reg;

	/* 4 packets or 40us timeout to interrupt */
	reg = RXDLY_INT_EN | (4<<RXMAX_PINT_SHIFT) | (2<<RXMAX_PTIME_SHIFT);
	write_reg_word(DLY_INT_CFG, reg);
#endif
}

// Assign Tx Rx Descriptor Control Registers
void macSetDMADescrCtrlReg(macAdapter_t *mac_p)
{
	uint32 txq;

	for (txq = 0; txq < TX_QUEUE_NUM; txq++) {
  		write_reg_word(TX_BASE_PTR(txq), K1_TO_PHY(mac_p->txDescrRingBaseVAddr[txq]));
  		write_reg_word(TX_MAX_CNT(txq), mac_p->txRingSize);
		write_reg_word(TX_CTX_IDX(txq), 0);
		write_reg_word(PDMA_RST_IDX, RST_DTX_IDX(txq));
	}
  	write_reg_word(RX_BASE_PTR(0), K1_TO_PHY(mac_p->rxDescrRingBaseVAddr));
  	write_reg_word(RX_MAX_CNT(0), mac_p->rxRingSize);
	write_reg_word(RX_CALC_IDX(0), mac_p->rxRingSize - 1);
	write_reg_word(PDMA_RST_IDX, RST_DRX_IDX(0));
}

void macSetGSW(macAdapter_t *mac_p)
{
	uint32 reg;

	/* set port 6 as 1Gbps, FC on */
	reg = (IPG_CFG_SHORT<<IPG_CFG_PN_SHIFT) | MAC_MODE_PN | FORCE_MODE_PN |
		MAC_TX_EN_PN | MAC_RX_EN_PN | BKOFF_EN_PN | BACKPR_EN_PN |
		ENABLE_RX_FC_PN | ENABLE_TX_FC_PN | (PN_SPEED_1000M<<FORCE_SPD_PN_SHIFT) |
		FORCE_DPX_PN | FORCE_LNK_PN;
	write_reg_word(GSW_PMCR(6), reg);

	/* set cpu port as port 6 */
	reg = (0xff<<MFC_BC_FFP_SHIFT) | (0xff<<MFC_UNM_FFP_SHIFT) | (0xff<<MFC_UNU_FFP_SHIFT) |
			MFC_CPU_EN	| (6<<MFC_CPU_PORT_SHIFT);
	write_reg_word(GSW_MFC, reg);


	/* check if FPGA */

	if (VPint(CR_AHB_HWCONF)&(1<<31)) {
#ifdef TCSUPPORT_MT7510_FE
		/*decrease mdc/mdio clock*/
		reg = read_reg_word(GSW_CFG_PPSC);
		reg &= ~((1<<6) | (1<<7));
		write_reg_word(GSW_CFG_PPSC, reg);

		printk("ID miir %x %x\n",tcMiiStationRead(1, 2),tcMiiStationRead(1, 3));
		if ((tcMiiStationRead(1, 2) == 0x4d) && (tcMiiStationRead(1, 3) == 0xd072)){
			/* auto polling enable, 2 PHY ports, start PHY addr=6 and end PHY addr=7 */
			reg = read_reg_word(GSW_CFG_PPSC);
			reg &= ~((1<<24) | (1<<25)| (1<<26)| (1<<27)| (1<<28)| (1<<29)| (1<<30));
			reg |= ((1<<24) | (1<<25)| (1<<30));
			reg &= ~(PHY_END_ADDR | PHY_ST_ADDR);
			reg |= (2<<PHY_END_ADDR_SHIFT) | (1<<PHY_ST_ADDR_SHIFT);
		}
		write_reg_word(GSW_CFG_PPSC, reg);
#else
		/* auto polling enable, 2 PHY ports, start PHY addr=6 and end PHY addr=7 */
		reg = read_reg_word(GSW_CFG_PPSC);
		reg |= PHY_AP_EN | EMB_AN_EN;
		reg &= ~(PHY_END_ADDR | PHY_ST_ADDR);
		/* check 6 PHY ports board or 2 PHY port board */
		if ((tcMiiStationRead(0, 2) == 0x243) && (tcMiiStationRead(0, 3) == 0xc54))
			reg |= (5<<PHY_END_ADDR_SHIFT) | (0<<PHY_ST_ADDR_SHIFT);
		else
			reg |= (7<<PHY_END_ADDR_SHIFT) | (6<<PHY_ST_ADDR_SHIFT);
		write_reg_word(GSW_CFG_PPSC, reg);
#endif
	}else{
		/* auto polling enable, 2 PHY ports, start PHY addr=6 and end PHY addr=7 */
		reg = read_reg_word(GSW_CFG_PPSC);
		reg |= PHY_AP_EN;
		reg &= ~(PHY_END_ADDR | PHY_ST_ADDR);
		//if ((tcMiiStationRead(7, 2) == 0x4d) && (tcMiiStationRead(7, 3) == 0xd072)){
			reg |= (7<<PHY_END_ADDR_SHIFT) | (7<<PHY_ST_ADDR_SHIFT);
		//	printk("phy check 1\n");
		//}else{
		//	printk("phy check 2\n");
		//	reg |= (5<<PHY_END_ADDR_SHIFT) | (5<<PHY_ST_ADDR_SHIFT);
		//}
		write_reg_word(GSW_CFG_PPSC, reg);
	}
#ifndef TCSUPPORT_MT7510_FE
	/* set port 5 giga port RX clock phase to degree 0 */
	reg = read_reg_word(GSW_CFG_GPC);
	reg |= RX_CLK_MODE;
	write_reg_word(GSW_CFG_GPC, reg);
#endif
#if defined(TCPHY_SUPPORT)
	/* enable switch abnormal irq, for error handle when abnormal irq occurs */
	if (isRT63365){
		if ( (VPint(0xbfb00064) & (0xffff)) == 0x0 ){
			enable_abnormal_irq();
		}

		reg = read_reg_word(GSW_CKGCR);
		reg &= ~((1<<4) | (1<<5));
		write_reg_word(GSW_CKGCR, reg);

	}
#endif
}

void macSetMACCR(macAdapter_t *mac_p)
{
	uint32 reg;
	int i;

	reg = (12<<GDM_JMB_LEN_SHIFT) | GDM_STRPCRC |
		(GDM_P_CPU<<GDM_UFRC_P_SHIFT) | (GDM_P_CPU<<GDM_BFRC_P_SHIFT) |
		(GDM_P_CPU<<GDM_MFRC_P_SHIFT) | (GDM_P_CPU<<GDM_OFRC_P_SHIFT);
#ifdef RAETH_CHECKSUM_OFFLOAD
	reg |= GDM_ICS_EN | GDM_TCS_EN | GDM_UCS_EN;
#endif
	write_reg_word(GDMA1_FWD_CFG, reg);
#if defined(TCSUPPORT_WAN_ETHER) && !defined(TCSUPPORT_MT7510_FE)
	write_reg_word(GDMA2_FWD_CFG, reg);
#endif

	reg = (0x8100<<INS_VLAN_SHIFT);
#ifdef RAETH_CHECKSUM_OFFLOAD
	reg |= ICS_GEN_EN | UCS_GEN_EN | TCS_GEN_EN;
#endif
	write_reg_word(CDMA_CSG_CFG, reg);

#if !defined(TCSUPPORT_MT7510_FE)
	/* set PDMA FC default value */
	write_reg_word(PDMA_FC_CFG1, 0x0fffffff);
	write_reg_word(PDMA_FC_CFG2, 0x0fffffff);

	/* check if FPGA */
	if (VPint(CR_AHB_HWCONF)&(1<<31)) {
		/* set 1us clock for FPGA */
		reg = read_reg_word(CR_CLK_CFG);
		reg &= ~(0x3f000000);
		reg |= (0x32<<24);
		write_reg_word(CR_CLK_CFG, reg);
	}
#endif

	/* set VLAN ID */
	for (i = 0; i < 8; i++) {
		reg = ((2*i+1)<<VLAN_ID1_SHIFT) | ((2*i)<<VLAN_ID0_SHIFT);
		write_reg_word(FE_VLAN_ID(i), reg);
	}
}

void macSetMacReg(macAdapter_t *mac_p)
{

	write_reg_word(GDMA1_MAC_ADRL, mac_p->macAddr[2]<<24 | mac_p->macAddr[3]<<16 | \
                               mac_p->macAddr[4]<<8  | mac_p->macAddr[5]<<0);
	write_reg_word(GDMA1_MAC_ADRH, mac_p->macAddr[0]<<8  | mac_p->macAddr[1]<<0);

#if defined(TCSUPPORT_WAN_ETHER) && !defined(TCSUPPORT_MT7510_FE)
	write_reg_word(GDMA2_MAC_ADRL, mac_p->macAddr[2]<<24 | mac_p->macAddr[3]<<16 | \
                               mac_p->macAddr[4]<<8  | mac_p->macAddr[5]<<0);
	write_reg_word(GDMA2_MAC_ADRH, mac_p->macAddr[0]<<8  | mac_p->macAddr[1]<<0);
#endif

	if (swicVendor == SWIC_RT63365) {
		write_reg_word(GSW_SMACCR0, mac_p->macAddr[2]<<24 | mac_p->macAddr[3]<<16 | \
                               mac_p->macAddr[4]<<8  | mac_p->macAddr[5]<<0);
		write_reg_word(GSW_SMACCR1, mac_p->macAddr[0]<<8  | mac_p->macAddr[1]<<0);
	}

}

macTxDescr_t *macTxRingProc(macAdapter_t *mac_p, uint32 txq)
{
	volatile macTxDescr_t *pTxDescp;
	volatile macTxDescr_t pTxDescpTmpVal;
	volatile macTxDescr_t *pTxDescpTmp = &pTxDescpTmpVal;
	unsigned long flags;
	struct sk_buff *freeskb;

	spin_lock_irqsave(&mac_p->lock, flags);
	pTxDescp = ((macTxDescr_t*)mac_p->txDescrRingBaseVAddr[txq]) + mac_p->txUnReleasedDescp[txq];

#if defined(FE_BYTE_SWAP) || defined(__LITTLE_ENDIAN)
	pTxDescpTmp = pTxDescp;
#else
	pTxDescpTmp->txd_info1.word = le32_to_cpu(pTxDescp->txd_info1.word);
	pTxDescpTmp->txd_info2.word = le32_to_cpu(pTxDescp->txd_info2.word);
	pTxDescpTmp->txd_info3.word = le32_to_cpu(pTxDescp->txd_info3.word);
	pTxDescpTmp->txd_info4.word = le32_to_cpu(pTxDescp->txd_info4.word);
#endif

  	while (mac_p->txUnReleasedBufCnt[txq] != 0) {
  		if (!pTxDescpTmp->txd_info2.bits.DDONE_bit) {
			spin_unlock_irqrestore(&mac_p->lock, flags);
			return 0;
		}

		freeskb = mac_p->txskbs[txq][mac_p->txUnReleasedDescp[txq]];
#ifdef LOOPBACK_SUPPORT
		if (LOOPBACK_MODE(macLoopback))
			dev_kfree_skb(freeskb);
		else
#endif
			dev_kfree_skb_any(freeskb);

		//pTxDescp->tdes2.txbuf_addr = 0;
		mac_p->txskbs[txq][mac_p->txUnReleasedDescp[txq]] = NULL;

		if (mac_p->txUnReleasedDescp[txq] == (mac_p->txRingSize - 1))
			mac_p->txUnReleasedDescp[txq] = 0;
		else
			mac_p->txUnReleasedDescp[txq]++;
		mac_p->txUnReleasedBufCnt[txq]--;

		pTxDescp = ((macTxDescr_t*)mac_p->txDescrRingBaseVAddr[txq]) + mac_p->txUnReleasedDescp[txq];

#if defined(FE_BYTE_SWAP) || defined(__LITTLE_ENDIAN)
		pTxDescpTmp = pTxDescp;
#else
		pTxDescpTmp->txd_info1.word = le32_to_cpu(pTxDescp->txd_info1.word);
		pTxDescpTmp->txd_info2.word = le32_to_cpu(pTxDescp->txd_info2.word);
		pTxDescpTmp->txd_info3.word = le32_to_cpu(pTxDescp->txd_info3.word);
		pTxDescpTmp->txd_info4.word = le32_to_cpu(pTxDescp->txd_info4.word);
#endif
    	mac_p->macStat.inSilicon.txDeQueueNum++;
	}
	spin_unlock_irqrestore(&mac_p->lock, flags);

	return (macTxDescr_t*) pTxDescp;
}

#if defined(TCSUPPORT_WAN_ETHER) && defined(CONFIG_8021P_REMARK)
static inline struct sk_buff* vlanPriRemark(struct sk_buff *skb)
{
	uint8 encap_mode = 0, encap_len = 0;
	char * vlan_p = NULL, *ether_type_ptr = NULL;
	unsigned char ucprio = 0;
	unsigned char uc802prio = 0;
	uint16 vid=0;
	int copy_len = 0;

	if ( skb->mark & QOS_8021p_MARK ) {
		ether_type_ptr = skb->data + 12;
		ucprio = (skb->mark & QOS_8021p_MARK) >> 8;
		if ( (ucprio < QOS_8021P_0_MARK) && (ucprio >= 0) ) { //0~7 remark
			uc802prio = ucprio;
		}
		else if ( QOS_8021P_0_MARK == ucprio ) {	//zero mark
			uc802prio = 0;
		}
		else{//pass through
			/*do nothing*/
			return skb;
		}

		if(*(unsigned short *)ether_type_ptr == 0x8100){
			vid=(*(unsigned short *)(ether_type_ptr+2) & 0xfff);
		}
		else{
			/*Insert a vlan tag with vid =0*/
			vid=0;
			if ( skb_headroom(skb) < VLAN_HLEN ) {
				#if 0
				struct sk_buff *sk_tmp = skb;
				skb = skb_realloc_headroom(sk_tmp, VLAN_HLEN);

				if ( ATM_SKB(sk_tmp)->vcc->pop ) {
					ATM_SKB(sk_tmp)->vcc->pop(ATM_SKB(sk_tmp)->vcc, sk_tmp);
				}
				else {
					dev_kfree_skb_any(sk_tmp);
				}
				#endif
				struct sk_buff *skb2 = skb_realloc_headroom(skb, VLAN_HLEN);
				dev_kfree_skb(skb);
				if (skb2 == NULL) {
					printk(KERN_ERR, "Vlan:failed to realloc headroom\n");
					return NULL;
				}
				skb = skb2;
			}
			else {
				skb = skb_unshare(skb, GFP_ATOMIC);
				if ( !skb ) {
			//		printk(KERN_ERR, "Vlan: failed to unshare skbuff\n");
					return NULL;
				}
			}

			/*offset 4 bytes*/
			skb_push(skb, VLAN_HLEN);

			copy_len = 2*VLAN_ETH_ALEN;
			/*move the mac address to the beginning of new header*/
			memmove(skb->data, skb->data+VLAN_HLEN, copy_len);
			skb->network_header -= VLAN_HLEN;
			skb->mac_header -= VLAN_HLEN;
		}

		vlan_p = skb->data + 12;
		*(unsigned short *)vlan_p = 0x8100;

		vlan_p += 2;
		*(unsigned short *)vlan_p = 0;
		/*3 bits priority and vid vlaue*/
		*(unsigned short*)vlan_p |= (((uc802prio & 0x7) << 13)|vid) ;
		//skb->network_header -= VLAN_HLEN;
		//skb->mac_header -= VLAN_HLEN;
	}
	return skb;
}
#endif

#ifdef TCSUPPORT_AUTOBENCH
int tc3262_gmac_tx(struct sk_buff *skb, struct net_device *dev)
{
	if (skb != NULL){
		dev_kfree_skb_any(skb);
	}
	return NETDEV_TX_OK;
}
#endif

#ifdef TCSUPPORT_AUTOBENCH
int tc3262_gmac_tx_autobench(struct sk_buff *skb, struct net_device *dev)
#else
int tc3262_gmac_tx(struct sk_buff *skb, struct net_device *dev)
#endif
{
	volatile macTxDescr_t *currDescrp = NULL;
	volatile macTxDescr_t currDescrpTmpVal;
	volatile macTxDescr_t *currDescrpTmp = &currDescrpTmpVal;
	uint32 length;
	uint8 *bufAddrp;
	unsigned long flags;
	uint32 txq = 0;
	char is_wan_packet = 0;


#if defined(TCSUPPORT_WAN_ETHER)
	if(dev == tc3262_gmac_wan_dev){
		//note send to wan port
		setComeFromWan(skb,1);
		is_wan_packet = 1;
#ifdef CONFIG_8021P_REMARK
		skb=vlanPriRemark(skb);
		if(skb==NULL){
			printk("802.1p remark failure\r\n");
			return NETDEV_TX_OK;
		}
#endif
	}else{
		setComeFromWan(skb,0);
		is_wan_packet = 0;
	}
#endif

#ifdef LOOPBACK_SUPPORT
	if (macLoopback & LOOPBACK_PKT) {
		printk("TX: BEFORE ");
		dump_skb(skb);
	}
#endif

#ifdef CONFIG_TC3162_ADSL
	if(isTCConsolePkt(skb)){
		if(swicVendor == SWIC_RT63365){
			skb = macRT63365STagInsert(skb);
			if (skb == NULL) {
#if defined(TCSUPPORT_WAN_ETHER)
				if(is_wan_packet == 1){
					mac_wan_p->macStat.MIB_II.outDiscards++;
				}else
#endif
				mac_p->macStat.MIB_II.outDiscards++;
				return NETDEV_TX_OK;
			}
		}
	}
#endif
#ifdef WAN2LAN
	if((swicVendor==SWIC_RT63365)&&macSTagFlag && !(skb->mark & SKBUF_TCCONSOLE)){
		skb = macRT63365STagInsert(skb);
		if (skb == NULL) {
#if defined(TCSUPPORT_WAN_ETHER)
			if(is_wan_packet == 1){
				mac_wan_p->macStat.MIB_II.outDiscards++;
			}else
#endif
			mac_p->macStat.MIB_II.outDiscards++;
			return NETDEV_TX_OK;
		}
	}
#endif

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

#ifdef TCSUPPORT_RA_HWNAT
	if (ra_sw_nat_hook_tx != NULL) {
		if (ra_sw_nat_hook_txq)
			ra_sw_nat_hook_txq(skb, txq);
		if (ra_sw_nat_hook_tx(skb, 1) == 0) {
			dev_kfree_skb_any(skb);
			return NETDEV_TX_OK;
		}
	}
#endif

	bufAddrp = skb->data;
	length = skb->len;
	if (unlikely(skb->len < ETH_ZLEN)) {
		if (skb_padto(skb, ETH_ZLEN)) {
#if defined(TCSUPPORT_WAN_ETHER)
			if(is_wan_packet == 1){
				mac_wan_p->macStat.MIB_II.outDiscards++;
			}else
#endif
			mac_p->macStat.MIB_II.outDiscards++;
			return NETDEV_TX_OK;
		}
		length = ETH_ZLEN;
	}

#ifdef LOOPBACK_SUPPORT
	if (macLoopback & LOOPBACK_PKT) {
		printk("TX: ");
		dump_skb(skb);
	}

	if (macLoopback & LOOPBACK_TX_RANDOM)
		txq = random32() & GMAC_PRIORITY_MASK;
	else
		txq = skb->priority & GMAC_PRIORITY_MASK;
#endif

	if (mac_p->txUnReleasedBufCnt[txq] >= TX_BUF_RELEASE_THRESHOLD)
    	macTxRingProc(mac_p, txq);

	//need protect this count read before count add. shnwind .
	spin_lock_irqsave(&mac_p->lock, flags);
	if (mac_p->txUnReleasedBufCnt[txq] == mac_p->txRingSize - 1) {
#if defined(TCSUPPORT_WAN_ETHER)
		if(is_wan_packet == 1){
			mac_wan_p->macStat.MIB_II.outDiscards++;
		}else
#endif
		mac_p->macStat.MIB_II.outDiscards++;
		spin_unlock_irqrestore(&mac_p->lock, flags);
		dev_kfree_skb_any(skb);
		return NETDEV_TX_OK;
	}

	/* ----- Count the MIB-II ----- */
#if defined(TCSUPPORT_WAN_ETHER)
	if(is_wan_packet == 1){
		mac_wan_p->macStat.MIB_II.outOctets += length;
		if (*bufAddrp & 0x01)
			mac_wan_p->macStat.MIB_II.outMulticastPkts++;
		else
			mac_wan_p->macStat.MIB_II.outUnicastPkts++;
	}else
#endif
	{
	mac_p->macStat.MIB_II.outOctets += length;

	if (*bufAddrp & 0x01)
		mac_p->macStat.MIB_II.outMulticastPkts++;
	else
		mac_p->macStat.MIB_II.outUnicastPkts++;

	}
  	ledTurnOn(LED_ETHER_ACT_STATUS);
	if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_100MB)
		ledTurnOn(LED_ETHER_100M_ACT_STATUS);
	else
		ledTurnOn(LED_ETHER_10M_ACT_STATUS);

	dma_cache_wback_inv((unsigned long)(skb->data), length);

	//spin_lock_irqsave(&mac_p->lock, flags);

	/* ----- Get the transmit descriptor ----- */
	currDescrp = ((macTxDescr_t *) mac_p->txDescrRingBaseVAddr[txq]) + mac_p->txCurrentDescp[txq];

#if defined(FE_BYTE_SWAP) || defined(__LITTLE_ENDIAN)
	currDescrpTmp = currDescrp;
#else
	currDescrpTmp->txd_info1.word = le32_to_cpu(currDescrp->txd_info1.word);
	currDescrpTmp->txd_info2.word = le32_to_cpu(currDescrp->txd_info2.word);
	currDescrpTmp->txd_info3.word = le32_to_cpu(currDescrp->txd_info3.word);
	currDescrpTmp->txd_info4.word = le32_to_cpu(currDescrp->txd_info4.word);
#endif

  	if (!currDescrpTmp->txd_info2.bits.DDONE_bit) {
#if defined(TCSUPPORT_WAN_ETHER)
		if(is_wan_packet == 1){
			mac_wan_p->macStat.MIB_II.outDiscards++;
		}else
#endif
		mac_p->macStat.MIB_II.outDiscards++;
		dev_kfree_skb_any(skb);
		spin_unlock_irqrestore(&mac_p->lock, flags);
		return NETDEV_TX_OK;
	}

	/* tx buffer size */
 	currDescrpTmp->txd_info1.bits.SDP0 = K1_TO_PHY(skb->data);
  	currDescrpTmp->txd_info2.bits.SDL0 = length;

  	currDescrpTmp->txd_info4.word = 0;
#ifndef TCSUPPORT_MT7510_FE
  	currDescrpTmp->txd_info4.bits.QN = 3;
#endif
#ifdef LOOPBACK_SUPPORT
	if (LOOPBACK_MODE(macLoopback) && (LOOPBACK_MODE(macLoopback) <= LOOPBACK_RX_CHK)) {
		/* check if external loopback */
		if (macLoopback & LOOPBACK_EXT) {
			/* GDMA1 */
   			currDescrpTmp->txd_info4.bits.PN = DPORT_GDMA1;
		} else {
			/* CPU */
   			currDescrpTmp->txd_info4.bits.PN = DPORT_CPU;
		}
	} else
#endif
	{
#if defined(TCSUPPORT_WAN_ETHER) && !defined(TCSUPPORT_MT7510_FE)
		if (is_wan_packet == 1)
			/* GDMA2 */
   			currDescrpTmp->txd_info4.bits.PN = DPORT_GDMA2;
		else
#endif

			/* GDMA1 */
   			currDescrpTmp->txd_info4.bits.PN = DPORT_GDMA1;
	}

#ifdef TCSUPPORT_RA_HWNAT
	if (ra_sw_nat_hook_magic) {
		if (ra_sw_nat_hook_magic(skb, FOE_MAGIC_PPE)) {
			/* PPE */
   			currDescrpTmp->txd_info4.bits.PN = DPORT_PPE;
		}
	}
#endif

#if VLAN_TAG_USED
	if (vlan_tx_tag_present(skb)) {
		uint16 vlan_tag = cpu_to_be16(vlan_tx_tag_get(skb));

   		currDescrpTmp->txd_info4.bits.VIDX = (vlan_tag & 0xfff);
   		currDescrpTmp->txd_info4.bits.VPRI = (vlan_tag>>13)&0x7;
   		currDescrpTmp->txd_info4.bits.INSV = 1;
	} else {
   		currDescrpTmp->txd_info4.bits.INSV = 0;
	}
#endif

#ifdef RAETH_CHECKSUM_OFFLOAD
	if (skb->ip_summed == CHECKSUM_PARTIAL) {
  		currDescrpTmp->txd_info4.bits.TCO = 1;
  		currDescrpTmp->txd_info4.bits.UCO = 1;
  		currDescrpTmp->txd_info4.bits.ICO = 1;
	}
#endif

	mac_p->txskbs[txq][mac_p->txCurrentDescp[txq]] = skb;

	currDescrpTmp->txd_info2.bits.DDONE_bit = 0;

#if !defined(FE_BYTE_SWAP) && !defined(__LITTLE_ENDIAN)
	currDescrp->txd_info1.word = cpu_to_le32(currDescrpTmp->txd_info1.word);
	currDescrp->txd_info2.word = cpu_to_le32(currDescrpTmp->txd_info2.word);
	currDescrp->txd_info3.word = cpu_to_le32(currDescrpTmp->txd_info3.word);
	currDescrp->txd_info4.word = cpu_to_le32(currDescrpTmp->txd_info4.word);
#endif

#ifdef LOOPBACK_SUPPORT
	if (macLoopback & LOOPBACK_MSG) {
		printk("tx txq=%ld curr=%ld\n", txq, mac_p->txCurrentDescp[txq]);
		printk(" tdes1=%08lx\n", currDescrpTmp->txd_info1.word);
		printk(" tdes2=%08lx\n", currDescrpTmp->txd_info2.word);
		printk(" tdes3=%08lx\n", currDescrpTmp->txd_info3.word);
		printk(" tdes4=%08lx\n", currDescrpTmp->txd_info4.word);
	}
#endif

	mac_p->txCurrentDescp[txq] = (mac_p->txCurrentDescp[txq] + 1) % mac_p->txRingSize;

	wmb();

	write_reg_word(TX_CTX_IDX(txq), mac_p->txCurrentDescp[txq]);

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

static int tc3262_gmac_qos_gen(struct net_device *dev, int tx_len)
{
	int i, k;
	struct sk_buff *skb;
	uint8 *tx_data;
	uint8 tx_seed;
	uint8 tx_queue;
	int txq;

	macDrvStop();

	for (txq = TX_QUEUE_NUM - 1; txq >= 0; txq--) {
		for (i = 0; i < MAC_TXDESCP_NO; i++) {
			skb = dev_alloc_skb(RX_BUF_LEN);
			if (skb == NULL) {
				printk("skb == NULL\n");
				continue;
			}

			skb->priority = txq;

			//tx_len = random32();
			if (tx_len == 0)
				tx_len = 60;
			tx_len = tx_len % 1514;
			if (tx_len > 1514)
				tx_len = 1514;
			if (tx_len < 60)
				tx_len = 60;

			tx_data = skb_put(skb, tx_len);
			for (k = 0; k < 6; k++) {
				if (k == 0)
					tx_data[k] = 0x10;
				else
					tx_data[k] = 0x10 + txq;
			}
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

	return 0;
}

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
    macIMRMask();

    macSetIntConf();

    macSetDMADescrCtrlReg(mac_p);

    macSetMACCR(mac_p);

    // --- setup MAC address ---
    macSetMacReg(mac_p);

    macSetGSW(mac_p);

    return 0;
}

void macDrvDescripReset(macAdapter_t *mac_p)
{
	macRxDescr_t *pRxDescp;
	macTxDescr_t *pTxDescp;
	struct sk_buff *skb;
	int i;
	uint32 txq = 0;

	pRxDescp = (macRxDescr_t*) mac_p->rxDescrRingBaseVAddr;
	for (i = 0; i < mac_p->rxRingSize; i++) {
		skb = mac_p->rxskbs[i];
		if (skb != NULL)
			dev_kfree_skb_any(skb);
		// Init Descriptor
		pRxDescp->rxd_info1.word = 0;
		pRxDescp->rxd_info2.word = 0;
		pRxDescp->rxd_info3.word = 0;
		pRxDescp->rxd_info4.word = 0;
		mac_p->rxskbs[i] = NULL;
		pRxDescp++;
	}

	for (txq = 0; txq < TX_QUEUE_NUM; txq++) {
		pTxDescp = (macTxDescr_t*) mac_p->txDescrRingBaseVAddr[txq];
		// free all un-released tx mbuf
		for (i = 0 ; i < mac_p->txRingSize ; i++) {
			skb = mac_p->txskbs[txq][i];
			if (skb != NULL)
				dev_kfree_skb_any(skb);
			pTxDescp->txd_info1.word = 0;
			pTxDescp->txd_info2.word = 0;
			pTxDescp->txd_info3.word = 0;
			pTxDescp->txd_info4.word = 0;
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
		pRxDescp->rxd_info1.word = 0;
		pRxDescp->rxd_info2.word = 0;
		pRxDescp->rxd_info3.word = 0;
		pRxDescp->rxd_info4.word = 0;

		// Assign flag
		pRxDescp->rxd_info2.bits.LS0 = 1;

		skb = gmac_alloc_skb2k();
		if (skb == NULL) {
			printk("tc3262_gmac_descinit init fail.\n");
			return 1;
		}
		dma_cache_inv((unsigned long)(skb->data), RX_MAX_PKT_LEN);
		skb_reserve(skb, NET_IP_ALIGN);

		pRxDescp->rxd_info1.bits.PDP0 = K1_TO_PHY(skb->data);

#if !defined(FE_BYTE_SWAP) && !defined(__LITTLE_ENDIAN)
		pRxDescp->rxd_info1.word = cpu_to_le32(pRxDescp->rxd_info1.word);
		pRxDescp->rxd_info2.word = cpu_to_le32(pRxDescp->rxd_info2.word);
		pRxDescp->rxd_info3.word = cpu_to_le32(pRxDescp->rxd_info3.word);
		pRxDescp->rxd_info4.word = cpu_to_le32(pRxDescp->rxd_info4.word);
#endif

		mac_p->rxskbs[i] = skb;
	}

	for (txq = 0; txq < TX_QUEUE_NUM; txq++) {
		/* init. tx descriptor, don't allocate memory */
		pTxDescp = (macTxDescr_t*) mac_p->txDescrRingBaseVAddr[txq];
		for (i = 0 ; i < mac_p->txRingSize ; i++, pTxDescp++) {
    		// Init descriptor
			pTxDescp->txd_info1.word = 0;
			pTxDescp->txd_info2.word = 0;
			pTxDescp->txd_info3.word = 0;
			pTxDescp->txd_info4.word = 0;

    		pTxDescp->txd_info2.bits.LS0_bit = 1;
    		pTxDescp->txd_info2.bits.DDONE_bit = 1;

			/* CPU */
    		//pTxDescp->txd_info4.bits.PN = 0;
			/* GDMA1 */
    		pTxDescp->txd_info4.bits.PN = DPORT_GDMA1;
#ifndef TCSUPPORT_MT7510_FE
    		pTxDescp->txd_info4.bits.QN = 3;
#endif

#if !defined(FE_BYTE_SWAP) && !defined(__LITTLE_ENDIAN)
			pTxDescp->txd_info1.word = cpu_to_le32(pTxDescp->txd_info1.word);
			pTxDescp->txd_info2.word = cpu_to_le32(pTxDescp->txd_info2.word);
			pTxDescp->txd_info3.word = cpu_to_le32(pTxDescp->txd_info3.word);
			pTxDescp->txd_info4.word = cpu_to_le32(pTxDescp->txd_info4.word);
#endif

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

	reg = read_reg_word(PDMA_GLO_CFG);
	reg &= ~PDMA_BT_SIZE;
	reg |= TX_WB_DDONE | (PDMA_BT_SIZE_32DW<<PDMA_BT_SIZE_SHIFT) | RX_DMA_EN | TX_DMA_EN;
#ifdef FE_BYTE_SWAP
	reg |= PDMA_BYTE_SWAP;
#endif
#ifdef TCSUPPORT_MT7510_FE
	reg |= RX_2BYTE_OFFSET;
#endif
	write_reg_word(PDMA_GLO_CFG, reg);
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

static inline int macRxRingChk(void)
{
	macRxDescr_t *rxDescrp;
	volatile macRxDescr_t rxDescrpTmpVal;
	volatile macRxDescr_t *rxDescrpTmp = &rxDescrpTmpVal;

	rxDescrp = ((macRxDescr_t*)mac_p->rxDescrRingBaseVAddr) + mac_p->rxCurrentDescp;

#if defined(FE_BYTE_SWAP) || defined(__LITTLE_ENDIAN)
	rxDescrpTmp = rxDescrp;
#else
	rxDescrpTmp->rxd_info2.word = le32_to_cpu(rxDescrp->rxd_info2.word);
#endif

 	if (rxDescrpTmp->rxd_info2.bits.DDONE_bit)
		return 1;

	return -1;
}
#ifdef TCSUPPORT_AUTOBENCH
extern int autobench_mac_lpbk_flag;
extern int autobench_mac_lpbk_cnt;
extern int autobench_mac_lpbk1_flag;
extern int xxx;
extern unsigned char LoopbackData[];
int check_data_fail = 0;
#endif

int macRxRingProc(struct net_device *dev, int quota)
{
	volatile macRxDescr_t *rxDescrp;
	volatile macRxDescr_t rxDescrpTmpVal;
	volatile macRxDescr_t *rxDescrpTmp = &rxDescrpTmpVal;
	uint32 frameSize;
	struct sk_buff *newskb, *skb;
	int npackets = 0;
	uint32 pattern;


	#ifdef TCPHY_4PORT
	uint16 maxPktSize=0;
	#endif
	#ifdef CONFIG_TC3162_ADSL
	int tc_pro_idx=0;
	int vlan_level=0;
	int rm_len=0;
	#endif

	rxDescrp = ((macRxDescr_t*)mac_p->rxDescrRingBaseVAddr) + mac_p->rxCurrentDescp;

#if defined(FE_BYTE_SWAP) || defined(__LITTLE_ENDIAN)
	rxDescrpTmp = rxDescrp;
#else
	rxDescrpTmp->rxd_info1.word = le32_to_cpu(rxDescrp->rxd_info1.word);
	rxDescrpTmp->rxd_info2.word = le32_to_cpu(rxDescrp->rxd_info2.word);
	rxDescrpTmp->rxd_info3.word = le32_to_cpu(rxDescrp->rxd_info3.word);
	rxDescrpTmp->rxd_info4.word = le32_to_cpu(rxDescrp->rxd_info4.word);
#endif

#ifdef TC3262_GMAC_NAPI
  	while ((npackets <= quota) && (rxDescrpTmp->rxd_info2.bits.DDONE_bit)) {
#else
  	while (rxDescrpTmp->rxd_info2.bits.DDONE_bit) {
#endif

		npackets++;
#ifdef LOOPBACK_SUPPORT
		if (macLoopback & LOOPBACK_MSG) {
			printk("\t\trx curr=%ld rx=%08lx\n", mac_p->rxCurrentDescp, (uint32) rxDescrp);
			printk("\t\t rdes1=%08lx\n", rxDescrpTmp->rxd_info1.word);
			printk("\t\t rdes2=%08lx\n", rxDescrpTmp->rxd_info2.word);
			printk("\t\t rdes3=%08lx\n", rxDescrpTmp->rxd_info3.word);
			printk("\t\t rdes4=%08lx\n", rxDescrpTmp->rxd_info4.word);
		}
#endif
		if (1) {
 	 		frameSize = rxDescrpTmp->rxd_info2.bits.PLEN0;

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
				printk("\t\t rdes1=%08lx\n", rxDescrpTmp->rxd_info1.word);
				printk("\t\t rdes2=%08lx\n", rxDescrpTmp->rxd_info2.word);
				printk("\t\t rdes3=%08lx\n", rxDescrpTmp->rxd_info3.word);
				printk("\t\t rdes4=%08lx\n", rxDescrpTmp->rxd_info4.word);

	       		skb = mac_p->rxskbs[mac_p->rxCurrentDescp];
				skb->len = rxDescrpTmp->rxd_info2.bits.PLEN0;
				dump_skb(skb);
				skb->len = 0;
#endif

	        	mac_p->macStat.inSilicon.rxEtherFrameLengthErr++;

	        	// Discard this packet & Repost this mbuf
	        	newskb = mac_p->rxskbs[mac_p->rxCurrentDescp];
	        	goto DISCARD;
	      	}

       		skb = mac_p->rxskbs[mac_p->rxCurrentDescp];

			newskb = gmac_alloc_skb2k();
        	if (unlikely(!newskb)) { /* faild to allocate more mbuf -> drop this pkt */
        		newskb = skb;
          		mac_p->macStat.MIB_II.inDiscards++;
          		goto RECVOK;
      		}

			dma_cache_inv((unsigned long)(newskb->data), RX_MAX_PKT_LEN);
			skb_reserve(newskb, NET_IP_ALIGN);

			skb_put(skb, frameSize);


#ifdef LOOPBACK_SUPPORT
			if (macLoopback & LOOPBACK_PKT) {
				printk("RX: FIRST ");
				dump_skb(skb);
			}
#endif

	      	// ----- Count the MIB-II -----
		    if (mac_p->statisticOn) {
	  	    	mac_p->macStat.MIB_II.inOctets += frameSize;

				if (*skb->data & 0x01)
			    	mac_p->macStat.MIB_II.inMulticastPkts++;
				else
		        	mac_p->macStat.MIB_II.inUnicastPkts++;
		    }

			#ifdef WAN2LAN
			/*add specail tag function by xyzhu*/
			if (macSTagFlag) {
				switch (swicVendor) {
					case SWIC_RT63365:
						macRT63365STagRemove(skb);
						break;
					default:
						break;
				}
			}
#if defined(TCSUPPORT_WAN_ETHER)
			if(isComeFromWan(skb) == 1){
				mac_wan_p->macStat.MIB_II.inOctets += frameSize;

				if (*skb->data & 0x01)
					mac_wan_p->macStat.MIB_II.inMulticastPkts++;
				else
		        		mac_wan_p->macStat.MIB_II.inUnicastPkts++;
			}

        		if(masko_on_off && (isComeFromWan(skb) == 1)){
				struct sk_buff *skb2 = NULL;

           		 	//Check the skb headroom is enough or not. shnwind 20100121.
           			 if(skb_headroom(skb) < TX_STAG_LEN){
                			skb2 = skb_copy_expand(skb, TX_STAG_LEN, skb_tailroom(skb) , GFP_ATOMIC);;
            			}else{
                			skb2 = skb_copy(skb,GFP_ATOMIC);
            			}

            			if(skb2 == NULL){
                			printk("wan2lan failure in sar rx direction for skb2 allocate failure.\n");
            			}
            			else{
                			skb2->mark |= SKBUF_COPYTOLAN;
                			macSend(WAN2LAN_CH_ID,skb2);
				}
			}
#endif
			#endif
#ifdef TCSUPPORT_AUTOBENCH
			if (autobench_mac_lpbk_flag){
				int number = 0;
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
				printk("\t\t rdes1=%08lx\n", rxDescrpTmp->rxd_info1.word);
				printk("\t\t rdes2=%08lx\n", rxDescrpTmp->rxd_info2.word);
				printk("\t\t rdes3=%08lx\n", rxDescrpTmp->rxd_info3.word);
				printk("\t\t rdes4=%08lx\n", rxDescrpTmp->rxd_info4.word);
				dump_data(skb->data, 2048);
				dump_data(UNCAC_ADDR(skb->data), 2048);
			}

			if (macLoopback & LOOPBACK_PKT) {
				printk("RX: ");
				dump_skb(skb);
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
			} else {
#else
			{
#endif
#if defined(TCSUPPORT_WAN_ETHER)
					if((tc3262_gmac_wan_dev != NULL) && (isComeFromWan(skb) == 1)){
						skb->dev = tc3262_gmac_wan_dev;
					}else
#endif
					{
					skb->dev = dev;
					}


#ifdef RAETH_CHECKSUM_OFFLOAD
#ifdef TCSUPPORT_MT7510_FE
					if ((((rxDescrpTmp->rxd_info4.bits.PKT_INFO & TU_H_VAL ) == 1)&& ((rxDescrpTmp->rxd_info4.bits.PKT_INFO & L4_C_INV ) == 0)) ||
						(((rxDescrpTmp->rxd_info4.bits.PKT_INFO & IPV4_H ) == 1)&& ((rxDescrpTmp->rxd_info4.bits.PKT_INFO & IPV4_H_INV ) == 0)))
						skb->ip_summed = CHECKSUM_UNNECESSARY;
					else
#else
					if ((rxDescrpTmp->rxd_info4.bits.L4FVLD_bit && (rxDescrpTmp->rxd_info4.bits.L4F == 0)) ||
						(rxDescrpTmp->rxd_info4.bits.IPFVLD_bit && (rxDescrpTmp->rxd_info4.bits.IPF == 0)))
						skb->ip_summed = CHECKSUM_UNNECESSARY;
					else
#endif
#endif
						skb->ip_summed = CHECKSUM_NONE;

					skb->protocol = eth_type_trans(skb, skb->dev);
					dev->last_rx = jiffies;

#ifdef TCSUPPORT_RA_HWNAT
					if (ra_sw_nat_hook_rxinfo)
						ra_sw_nat_hook_rxinfo(skb, FOE_MAGIC_GE, &rxDescrpTmp->rxd_info4, sizeof(PDMA_RXD_INFO4_T));

					if (ra_sw_nat_hook_rx != NULL) {
						if (ra_sw_nat_hook_rx(skb)) {
#ifdef TC3262_GMAC_NAPI
							netif_receive_skb(skb);
#else
							netif_rx(skb);
#endif
					   	}
				   	} else
#endif
					{
#ifdef TC3262_GMAC_NAPI
						netif_receive_skb(skb);
#else
						netif_rx(skb);
#endif
				   	}
				}
			}
DISCARD:

RECVOK:

			rxDescrpTmp->rxd_info1.bits.PDP0 = K1_TO_PHY(newskb->data);
	       	mac_p->rxskbs[mac_p->rxCurrentDescp] = newskb;
		} else { /* Update Error Counter and Drop it */
#ifdef LOOPBACK_SUPPORT
			printk("\t\tERR rx curr=%ld rx=%08lx\n", mac_p->rxCurrentDescp, (uint32) rxDescrp);
			printk("\t\t rdes1=%08lx\n", rxDescrpTmp->rxd_info1.word);
			printk("\t\t rdes2=%08lx\n", rxDescrpTmp->rxd_info2.word);
			printk("\t\t rdes3=%08lx\n", rxDescrpTmp->rxd_info3.word);
			printk("\t\t rdes4=%08lx\n", rxDescrpTmp->rxd_info4.word);

	       	skb = mac_p->rxskbs[mac_p->rxCurrentDescp];
			skb->len = rxDescrpTmp->rxd_info2.bits.PLEN0;
			dump_skb(skb);
			skb->len = 0;
#endif

		}

		rxDescrpTmp->rxd_info2.word = 0;
		rxDescrpTmp->rxd_info2.bits.LS0 = 1;

#if !defined(FE_BYTE_SWAP) && !defined(__LITTLE_ENDIAN)
		rxDescrp->rxd_info1.word = le32_to_cpu(rxDescrpTmp->rxd_info1.word);
		rxDescrp->rxd_info2.word = le32_to_cpu(rxDescrpTmp->rxd_info2.word);
		rxDescrp->rxd_info3.word = le32_to_cpu(rxDescrpTmp->rxd_info3.word);
		rxDescrp->rxd_info4.word = le32_to_cpu(rxDescrpTmp->rxd_info4.word);
#endif

		wmb();

		write_reg_word(RX_CALC_IDX(0), mac_p->rxCurrentDescp);

 		/* next descriptor*/
		mac_p->rxCurrentDescp = (mac_p->rxCurrentDescp + 1) % mac_p->rxRingSize;

		rxDescrp = ((macRxDescr_t*)mac_p->rxDescrRingBaseVAddr) + mac_p->rxCurrentDescp;

#if defined(FE_BYTE_SWAP) || defined(__LITTLE_ENDIAN)
		rxDescrpTmp = rxDescrp;
#else
		rxDescrpTmp->rxd_info1.word = le32_to_cpu(rxDescrp->rxd_info1.word);
		rxDescrpTmp->rxd_info2.word = le32_to_cpu(rxDescrp->rxd_info2.word);
		rxDescrpTmp->rxd_info3.word = le32_to_cpu(rxDescrp->rxd_info3.word);
		rxDescrpTmp->rxd_info4.word = le32_to_cpu(rxDescrp->rxd_info4.word);
#endif
	}

#ifdef LOOPBACK_SUPPORT
	if (macLoopback & LOOPBACK_MSG)
		printk("npackets=%d\n", npackets);
#endif

	return npackets;
}

void macDrvStop(void)
{
	uint32 reg;

	reg = read_reg_word(PDMA_GLO_CFG);
	reg &= ~(TX_WB_DDONE | RX_DMA_EN | TX_DMA_EN);
	write_reg_word(PDMA_GLO_CFG, reg);
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
			if( (companyId == 0x03a2 ) || (companyId == 0x0007)){
        //  if( companyId == 0x03a2 ){
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
    miiReg = tcMiiStationRead_ext(mac_p, MII_PHYSID1);
    
		if (miiReg == 0)
			miiReg = miiStationRead(mac_p, MII_PHYSID2);
		if (macPhyLookUp(mac_p, miiReg)) {
			return 0;
		}
	}

	//mac_p->enetPhyAddr = 0x00000000;
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

	if (isRT63365)
		swicVendor = SWIC_RT63365;


	/* detect if MT7530, wplin added 20120712*/
	if (isMT7530 || isMT7530ext) {
		swicVendor = SWIC_MT7530;
//		printk("swicVendor=SWIC_MT7530\n");
	} /* end wplin added 20120712*/

	return 0;
}

int macInit(void)
{
	if (macInitialized)
		return 0;

	macReset();

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
#ifdef CONFIG_TC3162_DMEM
	mac_p->macTxMemPool_p = (macTxMemPool_t *) alloc_sram(sizeof(macTxMemPool_t));
	if (mac_p->macTxMemPool_p == NULL)
#endif
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
	#ifdef TCPHY_SUPPORT
	tcephydbgcmd();
	#ifdef TC2031_DEBUG
	tcethercmd();
	#endif
	#endif
	#if defined(WAN2LAN) || defined(CONFIG_TC3162_ADSL)
	#ifndef TC2031_SUPPORT
	/* ------ xyzhu_nj_091104:Enable special Tag default ----- */
	macRxPortEnable(0);
	#endif
	#endif

	if (isRT63365){
		init_ethernet_port_map();
	}

	return 0;
}

int macSwReset(macAdapter_t *mac_p)
{
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

/* ADSL RTS dump function */

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

static int getGSWLinkSt(char *buf)
{
	uint16 index = 0;
	uint32 reg;
	int port;
	int speed;

	if (!macInitialized) {
		return index;
	}

	for (port = 0; port <= 6; port++) {
		index += sprintf(buf+index, "Port%d : ", port);

		reg = read_reg_word(GSW_PMSR(port));

		if (!(reg & MAC_LINK_STS)) {
			index += sprintf(buf+index, "Down\n");
			continue;
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
	}
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

	return count;
}

static int gsw_stats_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	int port;

	if (!macInitialized) {
		*eof = 1;
		return 0;
	}

	for (port = 0; port <= 6; port++) {
#ifdef TCSUPPORT_MT7510_FE
		index += sprintf(buf+index, "[ Port %d ]\n", port);
		CHK_BUF();
		index += sprintf(buf+index, "Rx Unicase Pkts        = 0x%08lx, ", read_reg_word(GSW_RX_UNIC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Multicast Pkts      = 0x%08lx\n", read_reg_word(GSW_RX_MULC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Broadcast Pkts      = 0x%08lx, ", read_reg_word(GSW_RX_BROC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Align Error         = 0x%08lx\n", read_reg_word(GSW_RX_ALIGE(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx CRC Error           = 0x%08lx, ", read_reg_word(GSW_RX_CRC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Under Size Pkts     = 0x%08lx\n", read_reg_word(GSW_RX_RUNT(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Fragment Error      = 0x%08lx, ", read_reg_word(GSW_RX_FRGE(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Over Size PPkts     = 0x%08lx\n", read_reg_word(GSW_RX_LONG(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Jabber Error        = 0x%08lx, ", read_reg_word(GSW_RX_JABE(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Pause Pkts          = 0x%08lx\n", read_reg_word(GSW_RX_PAUC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Drop Pkts           = 0x%08lx, ", read_reg_word(GSW_RX_DROC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx ING Drop Pkts       = 0x%08lx\n", read_reg_word(GSW_RX_INGC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx ARL Drop Pkts       = 0x%08lx, ", read_reg_word(GSW_RX_ARLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx FILTER Drop Pkts    = 0x%08lx\n", read_reg_word(GSW_RX_FILC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Unicase Pkts        = 0x%08lx, ", read_reg_word(GSW_TX_UNIC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Multicast Pkts      = 0x%08lx\n", read_reg_word(GSW_TX_MULC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Broadcast Pkts      = 0x%08lx, ", read_reg_word(GSW_TX_BROC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Collision           = 0x%08lx\n", read_reg_word(GSW_TX_COLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Single Collision    = 0x%08lx, ", read_reg_word(GSW_TX_SCOLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Multiple Collision  = 0x%08lx\n", read_reg_word(GSW_TX_MCOLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Deffer              = 0x%08lx, ", read_reg_word(GSW_TX_DEFC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Late Collision      = 0x%08lx\n", read_reg_word(GSW_TX_LCOLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx eXcessive Collision	= 0x%08lx, ", read_reg_word(GSW_TX_ECOLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Pause Pkts          = 0x%08lx\n", read_reg_word(GSW_TX_PAUC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Drop Pkts           = 0x%08lx\n", read_reg_word(GSW_TX_DROC(port)));
		CHK_BUF();;
#else
		index += sprintf(buf+index, "[ Port %d ]\n", port);
		CHK_BUF();
		index += sprintf(buf+index, "Rx Good Pkts          = 0x%08lx, ", (read_reg_word(GSW_RGPC(port)) & RX_GOOD_CNT) >> RX_GOOD_CNT_SHIFT);
		CHK_BUF();
		index += sprintf(buf+index, "Rx Bad Pkts           = 0x%08lx\n", (read_reg_word(GSW_RGPC(port)) & RX_BAD_CNT) >> RX_BAD_CNT_SHIFT);
		CHK_BUF();
		index += sprintf(buf+index, "Rx Good Bytes         = 0x%08lx, ", read_reg_word(GSW_RGOC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Bad Bytes          = 0x%08lx\n", read_reg_word(GSW_RBOC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx CTRL Drop Pkts     = 0x%08lx, ", (read_reg_word(GSW_REPC1(port)) & RX_CTRL_DROP_CNT) >> RX_CTRL_DROP_CNT_SHIFT);
		CHK_BUF();
		index += sprintf(buf+index, "Rx ING Drop Pkts      = 0x%08lx\n", (read_reg_word(GSW_REPC1(port)) & RX_ING_DROP_CNT) >> RX_ING_DROP_CNT_SHIFT);
		CHK_BUF();
		index += sprintf(buf+index, "Rx ARL Drop Pkts      = 0x%08lx, ", (read_reg_word(GSW_REPC2(port)) & RX_ARL_DROP_CNT) >> RX_ARL_DROP_CNT_SHIFT);
		CHK_BUF();
		index += sprintf(buf+index, "Rx FILTER Drop Pkts   = 0x%08lx\n", (read_reg_word(GSW_REPC2(port)) & RX_FILTER_DROP_CNT) >> RX_FILTER_DROP_CNT_SHIFT);
		CHK_BUF();
		index += sprintf(buf+index, "Tx Good Pkts          = 0x%08lx, ", (read_reg_word(GSW_TGPC(port)) & TX_GOOD_CNT) >> TX_GOOD_CNT_SHIFT);
		CHK_BUF();
		index += sprintf(buf+index, "Tx Bad Pkts           = 0x%08lx\n", (read_reg_word(GSW_TGPC(port)) & TX_BAD_CNT) >> TX_BAD_CNT_SHIFT);
		CHK_BUF();
		index += sprintf(buf+index, "Tx Good Bytes         = 0x%08lx, ", read_reg_word(GSW_TGOC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Bad Bytes          = 0x%08lx\n", read_reg_word(GSW_TBOC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Drop Pkts          = 0x%08lx\n", read_reg_word(GSW_TEPC(port)));
		CHK_BUF();
#endif
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

#if defined(SQA_VERIFY)
//MTK20120829_MT7530_1588pkt_generation, Start[
unsigned char sync_pkt[100] = {
  0x01, 0x00, 0x5e, 0x00, 0x01, 0x81, 0x00, 0xaa, 0xbb, 0x01, 0x23, 0x45, 0x08, 0x00, 0x45, 0x00,
  0x00, 0x48, 0x00, 0x00, 0x40, 0x00, 0x01, 0x11, 0xd6, 0x7a, 0xc0, 0xa8, 0x01, 0x01, 0xe0, 0x00,
  0x01, 0x81, 0x01, 0x3f, 0x01, 0x3f, 0x00, 0x34, 0x5f, 0x00, 0x80, 0x02, 0x00, 0x2c, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa,
  0xbb, 0xff, 0xfe, 0x01, 0x23, 0x45, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x30,
  0x8a, 0xca, 0x1f, 0xdd, 0xa0, 0xe4
};

unsigned char follow_pkt[100] = {
  0x01, 0x00, 0x5e, 0x00, 0x01, 0x81, 0x00, 0xaa, 0xbb, 0x01, 0x23, 0x45, 0x08, 0x00, 0x45, 0x00,
  0x00, 0x48, 0x00, 0x00, 0x40, 0x00, 0x01, 0x11, 0xd6, 0x6f, 0xc0, 0xa8, 0x01, 0x0c, 0xe0, 0x00,
  0x01, 0x81, 0x01, 0x40, 0x01, 0x40, 0x00, 0x34, 0xbb, 0x61, 0x88, 0x02, 0x00, 0x2c, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
  0x3d, 0xff, 0xfe, 0x81, 0x9f, 0x00, 0x00, 0x01, 0x00, 0x26, 0x02, 0x00, 0x00, 0x00, 0x4f, 0x30,
  0x8a, 0xf5, 0x39, 0x12, 0x23, 0x50
};

unsigned char delay_req_pkt[100] = {
  0x01, 0x00, 0x5e, 0x00, 0x00, 0x6b, 0x00, 0xaa, 0xbb, 0x01, 0x23, 0x45, 0x08, 0x00, 0x45, 0x00,
  0x00, 0x52, 0x00, 0x00, 0x40, 0x00, 0x01, 0x11, 0xd7, 0x86, 0xc0, 0xa8, 0x01, 0x01, 0xe0, 0x00,
  0x00, 0x6b, 0x01, 0x3f, 0x01, 0x3f, 0x00, 0x3e, 0xa4, 0x31, 0x82, 0x02, 0x00, 0x36, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa,
  0xbb, 0xff, 0xfe, 0x01, 0x23, 0x45, 0x00, 0x01, 0x00, 0x16, 0x05, 0x7f, 0x00, 0x00, 0x4f, 0x30,
  0x8a, 0xf8, 0x1f, 0x53, 0x55, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


unsigned int udp_checksum(const struct sk_buff *skb)
{
	unsigned int checksum = 0;
	unsigned int src_ip1, src_ip2, dest_ip1, dest_ip2;
	unsigned int src_port, dest_port;
	unsigned int udp_length;
	unsigned int udp_protocol;
	unsigned int data;
	unsigned int index;

	udp_protocol = (unsigned int)(*((unsigned char*)(&skb->data[23])));
	src_ip1 = (unsigned int)(*((unsigned short*)(&skb->data[26])));
	src_ip2 = (unsigned int)(*((unsigned short*)(&skb->data[28])));
	dest_ip1 = (unsigned int)(*((unsigned short*)(&skb->data[30])));
	dest_ip2 = (unsigned int)(*((unsigned short*)(&skb->data[32])));
	src_port = (unsigned int)(*((unsigned short*)(&skb->data[34])));
	dest_port = (unsigned int)(*((unsigned short*)(&skb->data[36])));
	udp_length = (unsigned int)(*((unsigned short*)(&skb->data[38])));

//	printk("src ip1: %x, src ip2: %x\n", src_ip1, src_ip2);
//	printk("dest ip1: %x, dest ip2: %x\n", dest_ip1, dest_ip2);
//	printk("src port: %x, dest port: %x\n", src_port, dest_port);
//	printk("udp_length: %x\n", udp_length);
//	printk("udp_protocol: %x\n", udp_protocol);
	data = 0;
	for (index=42; index<skb->len; index+=2){
		data += (unsigned int)(*((unsigned short*)(&skb->data[index])));
	}
//	printk("data: %x\n", data);

	checksum = src_ip1 + src_ip2 + dest_ip1 + dest_ip2 + src_port + dest_port + (2*udp_length) + data + udp_protocol;
//	printk("checksum1: %x\n", checksum);
	checksum += (checksum >> 16);
//	printk("checksum2: %x\n", checksum);
	checksum &= 0xffff;
//	printk("checksum3: %x\n", checksum);
	checksum = (~checksum);
//	printk("checksum4: %x\n", checksum);
	checksum &= 0xffff;
//	printk("checksum5: %x\n", checksum);

	return checksum;
}

int MT7530_gen_1588_packet(char flag)
{
	int k;
	struct sk_buff *skb;
	int tx_len;
	int tx_priority;
	unsigned long checksum;
	unsigned long sec, nsec;
	uint8 *tx_data;
	static short id=0;

	struct timespec cur_time;

	getnstimeofday(&cur_time);

	skb = dev_alloc_skb(RX_BUF_LEN);
	if (skb == NULL){
		printk("cannot allocate skb\n");
		return 0;
	}

	tx_data = skb_put(skb, tx_len);

	if (flag == 0)
		memcpy(tx_data, sync_pkt, 100);
	else if (flag == 1)
		memcpy(tx_data, follow_pkt, 100);
	else if (flag == 2)
		memcpy(tx_data, delay_req_pkt, 100);
	else {
		printk("error: 1588 packet type input\n");
		return 0;
	}

	tx_priority = 3;
	skb->priority = tx_priority;

	tx_len = 100;
	tx_data = skb_put(skb, tx_len);

	skb->data[76] = (unsigned char)(0x0);
	skb->data[77] = (unsigned char)(0x0);
	skb->data[78] = (unsigned char)((cur_time.tv_sec>>24) & 0xff);
	skb->data[79] = (unsigned char)((cur_time.tv_sec>>16) & 0xff);
	skb->data[80] = (unsigned char)((cur_time.tv_sec>>8) & 0xff);
	skb->data[81] = (unsigned char)((cur_time.tv_sec>>0) & 0xff);

	skb->data[82] = (unsigned char)((cur_time.tv_nsec>>24) & 0xff);
	skb->data[83] = (unsigned char)((cur_time.tv_nsec>>16) & 0xff);
	skb->data[84] = (unsigned char)((cur_time.tv_nsec>>8) & 0xff);
	skb->data[85] = (unsigned char)((cur_time.tv_nsec>>0) & 0xff);

	skb->data[72] = (unsigned char)((id >> 8) & 0xff);
	skb->data[73] = (unsigned char)(id & 0xff);
	id++;

	checksum = udp_checksum(skb);
	skb->data[40] = (checksum >> 8);
	skb->data[41] = (checksum & 0xff);

	tc3262_gmac_tx(skb, tc3262_gmac_dev);

	mdelay(100);

	return 0;
}

static int gen_1588_pkt_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32];
	int flag;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	flag = simple_strtoul(val_string, NULL, 16);

	MT7530_gen_1588_packet((char)(flag));

	return count;
}
//MTK20120829_MT7530_1588pkt_generation, ]End
#endif

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

/* wplin addded 20120703 */
static int gsw_mibN_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data, uint portId)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	int port;

	if (!macInitialized) {
		*eof = 1;
		return 0;
	}


	port = portId;
	{
#ifdef TCSUPPORT_MT7510_FE
		index += sprintf(buf+index, "[ Port %d ]\n", port);
		CHK_BUF();
		index += sprintf(buf+index, "Rx Unicase Pkts        = 0x%08lx, ", read_reg_word(GSW_RX_UNIC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Multicast Pkts      = 0x%08lx\n", read_reg_word(GSW_RX_MULC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Broadcast Pkts      = 0x%08lx, ", read_reg_word(GSW_RX_BROC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Align Error         = 0x%08lx\n", read_reg_word(GSW_RX_ALIGE(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx CRC Error           = 0x%08lx, ", read_reg_word(GSW_RX_CRC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Under Size Pkts     = 0x%08lx\n", read_reg_word(GSW_RX_RUNT(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Fragment Error      = 0x%08lx, ", read_reg_word(GSW_RX_FRGE(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Over Size PPkts     = 0x%08lx\n", read_reg_word(GSW_RX_LONG(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Jabber Error        = 0x%08lx, ", read_reg_word(GSW_RX_JABE(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Pause Pkts          = 0x%08lx\n", read_reg_word(GSW_RX_PAUC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx Drop Pkts           = 0x%08lx, ", read_reg_word(GSW_RX_DROC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx ING Drop Pkts       = 0x%08lx\n", read_reg_word(GSW_RX_INGC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx ARL Drop Pkts       = 0x%08lx, ", read_reg_word(GSW_RX_ARLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Rx FILTER Drop Pkts    = 0x%08lx\n", read_reg_word(GSW_RX_FILC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Unicase Pkts        = 0x%08lx, ", read_reg_word(GSW_TX_UNIC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Multicast Pkts      = 0x%08lx\n", read_reg_word(GSW_TX_MULC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Broadcast Pkts      = 0x%08lx, ", read_reg_word(GSW_TX_BROC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Collision           = 0x%08lx\n", read_reg_word(GSW_TX_COLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Single Collision    = 0x%08lx, ", read_reg_word(GSW_TX_SCOLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Multiple Collision  = 0x%08lx\n", read_reg_word(GSW_TX_MCOLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Deffer              = 0x%08lx, ", read_reg_word(GSW_TX_DEFC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Late Collision      = 0x%08lx\n", read_reg_word(GSW_TX_LCOLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx eXcessive Collision	= 0x%08lx, ", read_reg_word(GSW_TX_ECOLC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Pause Pkts          = 0x%08lx\n", read_reg_word(GSW_TX_PAUC(port)));
		CHK_BUF();
		index += sprintf(buf+index, "Tx Drop Pkts           = 0x%08lx\n", read_reg_word(GSW_TX_DROC(port)));
		CHK_BUF();;
#endif

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

static int gsw_link_st_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getGSWLinkSt(buf);
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

#ifdef LOOPBACK_SUPPORT

static int eth_loopback_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%08lx\n", macLoopback);
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
	int tx_len;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	macLoopback = simple_strtoul(val_string, NULL, 16);
	tx_len = (macLoopback & 0xffff0000) >> 16;
	macLoopback &= 0xffff;
	if (LOOPBACK_MODE(macLoopback) == LOOPBACK_RX_CHK) {
		/* check if external loopback */
		if (macLoopback & LOOPBACK_EXT) {
			macPhyLoopback(mac_p, 1);
			mdelay(2000);
		}
	} else if (LOOPBACK_MODE(macLoopback) == LOOPBACK_TX_QOS) {
		tc3262_gmac_qos_gen(tc3262_gmac_dev, tx_len);
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

#if 0
	index += sprintf(buf+index, "MDIO_ACCESS      (0x%08x) = 0x%08lx\n", MDIO_ACCESS, VPint(MDIO_ACCESS));
	CHK_BUF();
	index += sprintf(buf+index, "MDIO_CFG         (0x%08x) = 0x%08lx\n", MDIO_CFG, VPint(MDIO_CFG));
	CHK_BUF();
#endif
	index += sprintf(buf+index, "FE_DMA_GLO_CFG   (0x%08x) = 0x%08lx\n", FE_DMA_GLO_CFG, VPint(FE_DMA_GLO_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "FE_RST_GLO       (0x%08x) = 0x%08lx\n", FE_RST_GLO, VPint(FE_RST_GLO));
	CHK_BUF();
	index += sprintf(buf+index, "FE_INT_STATUS    (0x%08x) = 0x%08lx\n", FE_INT_STATUS, VPint(FE_INT_STATUS));
	CHK_BUF();
#ifdef TCSUPPORT_MT7510_FE
	index += sprintf(buf+index, "FE_INT_Enable    (0x%08x) = 0x%08lx\n", FE_INT_ENABLE, VPint(FE_INT_ENABLE));
	CHK_BUF();
#else
	index += sprintf(buf+index, "FC_DROP_STA      (0x%08x) = 0x%08lx\n", FC_DROP_STA, VPint(FC_DROP_STA));
	CHK_BUF();
#endif
	index += sprintf(buf+index, "FOE_TS_T         (0x%08x) = 0x%08lx\n", FOE_TS_T, VPint(FOE_TS_T));
	CHK_BUF();

	for (i = 0; i < TX_QUEUE_NUM; i++) {
		index += sprintf(buf+index, "TX_BASE_PTR(%d)   (0x%08x) = 0x%08lx\n", i, TX_BASE_PTR(i), VPint(TX_BASE_PTR(i)));
		CHK_BUF();
		index += sprintf(buf+index, "TX_MAX_CNT(%d)    (0x%08x) = 0x%08lx\n", i, TX_MAX_CNT(i), VPint(TX_MAX_CNT(i)));
		CHK_BUF();
		index += sprintf(buf+index, "TX_CTX_IDX(%d)    (0x%08x) = 0x%08lx\n", i, TX_CTX_IDX(i), VPint(TX_CTX_IDX(i)));
		CHK_BUF();
		index += sprintf(buf+index, "TX_DTX_IDX(%d)    (0x%08x) = 0x%08lx\n", i, TX_DTX_IDX(i), VPint(TX_DTX_IDX(i)));
		CHK_BUF();
	}

	for (i = 0; i < RX_QUEUE_NUM; i++) {
		index += sprintf(buf+index, "RX_BASE_PTR(%d)   (0x%08x) = 0x%08lx\n", i, RX_BASE_PTR(i), VPint(RX_BASE_PTR(i)));
		CHK_BUF();
		index += sprintf(buf+index, "RX_MAX_CNT(%d)    (0x%08x) = 0x%08lx\n", i, RX_MAX_CNT(i), VPint(RX_MAX_CNT(i)));
		CHK_BUF();
		index += sprintf(buf+index, "RX_CALC_IDX(%d)   (0x%08x) = 0x%08lx\n", i, RX_CALC_IDX(i), VPint(RX_CALC_IDX(i)));
		CHK_BUF();
		index += sprintf(buf+index, "RX_DRX_IDX(%d)    (0x%08x) = 0x%08lx\n", i, RX_DRX_IDX(i), VPint(RX_DRX_IDX(i)));
		CHK_BUF();
	}

	index += sprintf(buf+index, "PDMA_INFO        (0x%08x) = 0x%08lx\n", PDMA_INFO, VPint(PDMA_INFO));
	CHK_BUF();
	index += sprintf(buf+index, "PDMA_GLO_CFG     (0x%08x) = 0x%08lx\n", PDMA_GLO_CFG, VPint(PDMA_GLO_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "PDMA_RST_IDX     (0x%08x) = 0x%08lx\n", PDMA_RST_IDX, VPint(PDMA_RST_IDX));
	CHK_BUF();
	index += sprintf(buf+index, "DLY_INT_CFG      (0x%08x) = 0x%08lx\n", DLY_INT_CFG, VPint(DLY_INT_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "FREEQ_THRES      (0x%08x) = 0x%08lx\n", FREEQ_THRES, VPint(FREEQ_THRES));
	CHK_BUF();
	index += sprintf(buf+index, "INT_STATUS       (0x%08x) = 0x%08lx\n", INT_STATUS, VPint(INT_STATUS));
	CHK_BUF();
	index += sprintf(buf+index, "INT_MASK         (0x%08x) = 0x%08lx\n", INT_MASK, VPint(INT_MASK));
	CHK_BUF();
	index += sprintf(buf+index, "SCH_Q01_CFG      (0x%08x) = 0x%08lx\n", SCH_Q01_CFG, VPint(SCH_Q01_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "SCH_Q23_CFG      (0x%08x) = 0x%08lx\n", SCH_Q23_CFG, VPint(SCH_Q23_CFG));
	CHK_BUF();
#ifdef TCSUPPORT_MT7510_FE
	index += sprintf(buf+index, "GDMA1_FWD_CFG	  (0x%08x) = 0x%08lx\n", GDMA1_FWD_CFG, VPint(GDMA1_FWD_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_SHRP_CFG   (0x%08x) = 0x%08lx\n", GDMA1_SHRP_CFG, VPint(GDMA1_SHRP_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_MAC_ADRL   (0x%08x) = 0x%08lx\n", GDMA1_MAC_ADRL, VPint(GDMA1_MAC_ADRL));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_MAC_ADRH   (0x%08x) = 0x%08lx\n", GDMA1_MAC_ADRH, VPint(GDMA1_MAC_ADRH));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_VLAN_CHECK (0x%08x) = 0x%08lx\n", GDMA1_VLAN_CHECK, VPint(GDMA1_VLAN_CHECK));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_FQFC_CFG     (0x%08x) = 0x%08lx\n", PSE_FQFC_CFG, VPint(PSE_FQFC_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_IQ_CFG1      (0x%08x) = 0x%08lx\n", PSE_IQ_CFG, VPint(PSE_IQ_CFG));
	CHK_BUF();
	//index += sprintf(buf+index, "PSE_IQ_CFG2      (0x%08x) = 0x%08lx\n", PSE_IQ_CFG2, VPint(PSE_IQ_CFG2));
	index += sprintf(buf+index, "PSE_IQ_STA1      (0x%08x) = 0x%08lx\n", PSE_IQ_STA1, VPint(PSE_IQ_STA1));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_IQ_STA2      (0x%08x) = 0x%08lx\n", PSE_IQ_STA2, VPint(PSE_IQ_STA2));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_OQ_STA1      (0x%08x) = 0x%08lx\n", PSE_OQ_STA1, VPint(PSE_OQ_STA1));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_OQ_STA2      (0x%08x) = 0x%08lx\n", PSE_OQ_STA2, VPint(PSE_OQ_STA2));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_FWD_CFG    (0x%08x) = 0x%08lx\n", GDMA2_FWD_CFG, VPint(GDMA2_FWD_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_SHRP_CFG   (0x%08x) = 0x%08lx\n", GDMA2_SHRP_CFG, VPint(GDMA2_SHRP_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_MAC_ADRL   (0x%08x) = 0x%08lx\n", GDMA2_MAC_ADRL, VPint(GDMA2_MAC_ADRL));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_MAC_ADRH   (0x%08x) = 0x%08lx\n", GDMA2_MAC_ADRH, VPint(GDMA2_MAC_ADRH));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_VLAN_CHECK (0x%08x) = 0x%08lx\n", GDMA2_VLAN_CHECK, VPint(GDMA2_VLAN_CHECK));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA_CSG_CFG     (0x%08x) = 0x%08lx\n", CDMA_CSG_CFG, VPint(CDMA_CSG_CFG));
	CHK_BUF();
#else
	index += sprintf(buf+index, "GDMA1_FWD_CFG    (0x%08x) = 0x%08lx\n", GDMA1_FWD_CFG, VPint(GDMA1_FWD_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_SCH_CFG    (0x%08x) = 0x%08lx\n", GDMA1_SCH_CFG, VPint(GDMA1_SCH_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_SHRP_CFG   (0x%08x) = 0x%08lx\n", GDMA1_SHRP_CFG, VPint(GDMA1_SHRP_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_MAC_ADRL   (0x%08x) = 0x%08lx\n", GDMA1_MAC_ADRL, VPint(GDMA1_MAC_ADRL));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_MAC_ADRH   (0x%08x) = 0x%08lx\n", GDMA1_MAC_ADRH, VPint(GDMA1_MAC_ADRH));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_FQFC_CFG     (0x%08x) = 0x%08lx\n", PSE_FQFC_CFG, VPint(PSE_FQFC_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA_FC_CFG      (0x%08x) = 0x%08lx\n", CDMA_FC_CFG, VPint(CDMA_FC_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_FC_CFG     (0x%08x) = 0x%08lx\n", GDMA1_FC_CFG, VPint(GDMA1_FC_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_FC_CFG     (0x%08x) = 0x%08lx\n", GDMA2_FC_CFG, VPint(GDMA2_FC_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA_OQ_STA      (0x%08x) = 0x%08lx\n", CDMA_OQ_STA, VPint(CDMA_OQ_STA));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA1_OQ_STA     (0x%08x) = 0x%08lx\n", GDMA1_OQ_STA, VPint(GDMA1_OQ_STA));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_OQ_STA     (0x%08x) = 0x%08lx\n", GDMA2_OQ_STA, VPint(GDMA2_OQ_STA));
	CHK_BUF();
	index += sprintf(buf+index, "PSE_IQ_STA       (0x%08x) = 0x%08lx\n", PSE_IQ_STA, VPint(PSE_IQ_STA));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_FWD_CFG    (0x%08x) = 0x%08lx\n", GDMA2_FWD_CFG, VPint(GDMA2_FWD_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_SCH_CFG    (0x%08x) = 0x%08lx\n", GDMA2_SCH_CFG, VPint(GDMA2_SCH_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_SHRP_CFG   (0x%08x) = 0x%08lx\n", GDMA2_SHRP_CFG, VPint(GDMA2_SHRP_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_MAC_ADRL   (0x%08x) = 0x%08lx\n", GDMA2_MAC_ADRL, VPint(GDMA2_MAC_ADRL));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA2_MAC_ADRH   (0x%08x) = 0x%08lx\n", GDMA2_MAC_ADRH, VPint(GDMA2_MAC_ADRH));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA_CSG_CFG     (0x%08x) = 0x%08lx\n", CDMA_CSG_CFG, VPint(CDMA_CSG_CFG));
	CHK_BUF();
	index += sprintf(buf+index, "CDMA_SCH_CFG     (0x%08x) = 0x%08lx\n", CDMA_SCH_CFG, VPint(CDMA_SCH_CFG));
	CHK_BUF();
#endif
#ifndef TCSUPPORT_MT7510_FE

	index += sprintf(buf+index, "GDMA_TX_GBCNT1   (0x%08x) = 0x%08lx\n", GDMA_TX_GBCNT1, VPint(GDMA_TX_GBCNT1));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA_TX_GPCNT1   (0x%08x) = 0x%08lx\n", GDMA_TX_GPCNT1, VPint(GDMA_TX_GPCNT1));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA_TX_SKIPCNT1 (0x%08x) = 0x%08lx\n", GDMA_TX_SKIPCNT1, VPint(GDMA_TX_SKIPCNT1));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA_TX_COLCNT1  (0x%08x) = 0x%08lx\n", GDMA_TX_COLCNT1, VPint(GDMA_TX_COLCNT1));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA_RX_GBCNT1   (0x%08x) = 0x%08lx\n", GDMA_RX_GBCNT1, VPint(GDMA_RX_GBCNT1));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA_RX_GPCNT1   (0x%08x) = 0x%08lx\n", GDMA_RX_GPCNT1, VPint(GDMA_RX_GPCNT1));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA_RX_OERCNT1  (0x%08x) = 0x%08lx\n", GDMA_RX_OERCNT1, VPint(GDMA_RX_OERCNT1));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA_RX_FERCNT1  (0x%08x) = 0x%08lx\n", GDMA_RX_FERCNT1, VPint(GDMA_RX_FERCNT1));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA_RX_SERCNT1  (0x%08x) = 0x%08lx\n", GDMA_RX_SERCNT1, VPint(GDMA_RX_SERCNT1));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA_RX_LERCNT1  (0x%08x) = 0x%08lx\n", GDMA_RX_LERCNT1, VPint(GDMA_RX_LERCNT1));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA_RX_CERCNT1  (0x%08x) = 0x%08lx\n", GDMA_RX_CERCNT1, VPint(GDMA_RX_CERCNT1));
	CHK_BUF();
	index += sprintf(buf+index, "GDMA_RX_FCCNT1   (0x%08x) = 0x%08lx\n", GDMA_RX_FCCNT1, VPint(GDMA_RX_FCCNT1));
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
#endif

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
  	macRxDescr_t pRxDescpTmpVal;
  	macRxDescr_t *pRxDescpTmp = &pRxDescpTmpVal;

	if (!macInitialized) {
		*eof = 1;
		return 0;
	}

	pRxDescp = (macRxDescr_t*) mac_p->rxDescrRingBaseVAddr;
	index += sprintf(buf+index, "rx descr ring=%08lx\n", (uint32) pRxDescp);
	CHK_BUF();

	for (i = 0 ; i< mac_p->rxRingSize ; i++, pRxDescp++) {
#if defined(FE_BYTE_SWAP) || defined(__LITTLE_ENDIAN)
		pRxDescpTmp = pRxDescp;
#else
		pRxDescpTmp->rxd_info1.word = le32_to_cpu(pRxDescp->rxd_info1.word);
		pRxDescpTmp->rxd_info2.word = le32_to_cpu(pRxDescp->rxd_info2.word);
		pRxDescpTmp->rxd_info3.word = le32_to_cpu(pRxDescp->rxd_info3.word);
		pRxDescpTmp->rxd_info4.word = le32_to_cpu(pRxDescp->rxd_info4.word);
#endif

		index += sprintf(buf+index, "i= %d descr=%08lx\n", i, (uint32) pRxDescp);
		CHK_BUF();
		index += sprintf(buf+index, " rdes1=%08lx\n", pRxDescpTmp->rxd_info1.word);
		CHK_BUF();
		index += sprintf(buf+index, " rdes2=%08lx\n", pRxDescpTmp->rxd_info2.word);
		CHK_BUF();
		index += sprintf(buf+index, " rdes3=%08lx\n", pRxDescpTmp->rxd_info3.word);
		CHK_BUF();
		index += sprintf(buf+index, " rdes4=%08lx\n", pRxDescpTmp->rxd_info4.word);
		CHK_BUF();
		index += sprintf(buf+index, " skb  =%08lx\n", (uint32) mac_p->rxskbs[i]);
		CHK_BUF();
	}

	index += sprintf(buf+index, "rxCurrentDescp    =%ld\n", mac_p->rxCurrentDescp);
	CHK_BUF();
	index += sprintf(buf+index, "RX_CALC_IDX(0)    =%08lx\n", VPint(RX_CALC_IDX(0)));
	CHK_BUF();
	index += sprintf(buf+index, "RX_DRX_IDX(0)     =%08lx\n", VPint(RX_DRX_IDX(0)));
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
  	macTxDescr_t pTxDescpTmpVal;
  	macTxDescr_t *pTxDescpTmp = &pTxDescpTmpVal;

	if (!macInitialized) {
		*eof = 1;
		return 0;
	}

	txq = *((int *) data);

	pTxDescp = (macTxDescr_t*) mac_p->txDescrRingBaseVAddr[txq];

	index += sprintf(buf+index, "tx descr ring%d=%08x\n", txq, (unsigned int) pTxDescp);
	CHK_BUF();

	for (i = 0 ; i < mac_p->txRingSize ; i++, pTxDescp++) {
#if defined(FE_BYTE_SWAP) || defined(__LITTLE_ENDIAN)
		pTxDescpTmp = pTxDescp;
#else
		pTxDescpTmp->txd_info1.word = le32_to_cpu(pTxDescp->txd_info1.word);
		pTxDescpTmp->txd_info2.word = le32_to_cpu(pTxDescp->txd_info2.word);
		pTxDescpTmp->txd_info3.word = le32_to_cpu(pTxDescp->txd_info3.word);
		pTxDescpTmp->txd_info4.word = le32_to_cpu(pTxDescp->txd_info4.word);
#endif

		index += sprintf(buf+index, "i= %d descr=%08lx\n", i, (uint32) pTxDescp);
		CHK_BUF();
		index += sprintf(buf+index, " tdes1=%08lx\n", pTxDescpTmp->txd_info1.word);
		CHK_BUF();
		index += sprintf(buf+index, " tdes2=%08lx\n", pTxDescpTmp->txd_info2.word);
		CHK_BUF();
		index += sprintf(buf+index, " tdes3=%08lx\n", pTxDescpTmp->txd_info3.word);
		CHK_BUF();
		index += sprintf(buf+index, " tdes4=%08lx\n", pTxDescpTmp->txd_info4.word);
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
	index += sprintf(buf+index, "TX_CTX_IDX(%d)        =%08lx\n", txq, VPint(TX_CTX_IDX(txq)));
	CHK_BUF();
	index += sprintf(buf+index, "TX_DTX_IDX(%d)        =%08lx\n", txq, VPint(TX_DTX_IDX(txq)));
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

#if defined (QOS_REMARKING) || defined (TCSUPPORT_HW_QOS)

static uint8 get_qos_weight(uint8 weight)
{
	if (weight <= 1)
		return MAX_WEIGHT_1023;
	else if ((weight >= 2) && (weight <= 3))
		return MAX_WEIGHT_2047;
	else if ((weight >= 4) && (weight <= 6))
		return MAX_WEIGHT_4095;
	else if (weight >= 7)
		return MAX_WEIGHT_8191;
	return MAX_WEIGHT_8191;
}

static int eth_qoswrr_read_proc(char *page, char **start, off_t off,
		int count, int *eof, void *data){

	printk("%d %d %d %d %d\n", *qos_wrr_info, *(qos_wrr_info + 1), *(qos_wrr_info + 2), *(qos_wrr_info + 3), *(qos_wrr_info + 4));
	return 0;
}

static int eth_qoswrr_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){
	int len;
	char get_buf[32];
	uint32 priority, reg;
	int max_wrr_val = 0, i, bearer;

	/* do a range checking, don't overflow buffers in kernel modules */
	if(count > 32)
		len = 32;
	else
		len = count;
	/* use the copy_from_user function to copy buffer data to our get_buf */
	if(copy_from_user(get_buf, buffer, len))
		return -EFAULT;
	/* zero terminate get_buf */
	get_buf[len]='\0';

	if(sscanf(get_buf, "%d %d %d %d %d", qos_wrr_info, (qos_wrr_info+1), (qos_wrr_info+2), (qos_wrr_info+3), (qos_wrr_info+4)) != 5)
		return count;

	/* find max qos wrr weight */
	for (i = 0; i < 4; i++) {
		if (max_wrr_val < qos_wrr_info[i + 1]) {
			max_wrr_val = qos_wrr_info[i + 1];
			max_prio = 3 - i;
		}
	}

	qos_wrr_user = QOS_DMAWRR_USERDEFINE;
#ifdef TCSUPPORT_MT7510_FE
	printk("QoS need modification\n");
#else
	if(*qos_wrr_info == 0) { /*strict priority*/
		/* Min BW = Max BW = unlimited */
		reg = read_reg_word(SCH_Q01_CFG);
		reg &= ~(MAX_WEIGHT1 | MIN_RATE_RATIO1 | MAX_WEIGHT0 | MIN_RATE_RATIO0);
		reg |= (MIN_RATIO0<<MIN_RATE_RATIO1_SHIFT) | (MIN_RATIO0<<MIN_RATE_RATIO0_SHIFT);
		reg |= (MAX_WEIGHT_2047<<MAX_WEIGHT1_SHIFT) | (MAX_WEIGHT_1023<<MAX_WEIGHT0_SHIFT);
		write_reg_word(SCH_Q01_CFG, reg);

		reg = read_reg_word(SCH_Q23_CFG);
		reg &= ~(MAX_WEIGHT3 | MIN_RATE_RATIO3 | MAX_WEIGHT2 | MIN_RATE_RATIO2);
		reg |= (MIN_RATIO0<<MIN_RATE_RATIO3_SHIFT) | (MIN_RATIO0<<MIN_RATE_RATIO2_SHIFT);
		reg |= (MAX_WEIGHT_8191<<MAX_WEIGHT3_SHIFT) | (MAX_WEIGHT_4095<<MAX_WEIGHT2_SHIFT);
		write_reg_word(SCH_Q23_CFG, reg);

		/* set GDMA2_SCH_CFG */
		reg = read_reg_word(GDMA2_SCH_CFG);
		reg &= ~GDM_SCH_MOD;
		reg |= GDM_SCH_MOD_SP<<GDM_SCH_MOD_SHIFT;
		write_reg_word(GDMA2_SCH_CFG, reg);
	}
	else {  /*WRR*/
		/* Min BW = 0, Max BW = unlimited */
		reg = read_reg_word(SCH_Q01_CFG);
		reg &= ~(MAX_WEIGHT1 | MIN_RATE_RATIO1 | MAX_WEIGHT0 | MIN_RATE_RATIO0);
		reg |= (MIN_RATIO3<<MIN_RATE_RATIO1_SHIFT) | (MIN_RATIO3<<MIN_RATE_RATIO0_SHIFT);
		reg |= (get_qos_weight(qos_wrr_info[3] & 0x0f)<<MAX_WEIGHT1_SHIFT) | (get_qos_weight(qos_wrr_info[4] & 0x0f)<<MAX_WEIGHT0_SHIFT);
		write_reg_word(SCH_Q01_CFG, reg);

		reg = read_reg_word(SCH_Q23_CFG);
		reg &= ~(MAX_WEIGHT3 | MIN_RATE_RATIO3 | MAX_WEIGHT2 | MIN_RATE_RATIO2);
		reg |= (MIN_RATIO3<<MIN_RATE_RATIO3_SHIFT) | (MIN_RATIO3<<MIN_RATE_RATIO2_SHIFT);
		reg |= (get_qos_weight(qos_wrr_info[1] & 0x0f)<<MAX_WEIGHT3_SHIFT) | (get_qos_weight(qos_wrr_info[2] & 0x0f)<<MAX_WEIGHT2_SHIFT);
		write_reg_word(SCH_Q23_CFG, reg);

		/* set GDMA2_SCH_CFG */
		reg = read_reg_word(GDMA2_SCH_CFG);
		reg &= ~(GDM_SCH_MOD | GDM_WT_Q3 | GDM_WT_Q2 | GDM_WT_Q1 | GDM_WT_Q0);
		reg |= (GDM_SCH_MOD_WRR<<GDM_SCH_MOD_SHIFT) |
					(GDM_WT(qos_wrr_info[1] & 0x0f)<<GDM_WT_Q3_SHIFT) |
					(GDM_WT(qos_wrr_info[2] & 0x0f)<<GDM_WT_Q2_SHIFT) |
					(GDM_WT(qos_wrr_info[3] & 0x0f)<<GDM_WT_Q1_SHIFT) |
					(GDM_WT(qos_wrr_info[4] & 0x0f)<<GDM_WT_Q0_SHIFT);
		write_reg_word(GDMA2_SCH_CFG, reg);

	}
#endif
	return len;
}
#endif

#ifdef TCSUPPORT_QOS
static int eth_tcqos_read_proc(char *page, char **start, off_t off,
		int count, int *eof, void *data){

	if (qos_flag == QOS_SW_PQ) {
		printk("qos discipline is PQ.\n");
	}
	else if (qos_flag == QOS_SW_WRR) {
		printk("qos discipline is WRR.\n");
	}
	else if (qos_flag == QOS_SW_CAR) {
		printk("qos discipline is CAR.\n");
	}
	else if (qos_flag == QOS_HW_WRR) {
		printk("qos discipline is HW WRR.\n");
	}
	else if (qos_flag == QOS_HW_PQ) {
		printk("qos discipline is HW PQ.\n");
	}
	else {
		printk("qos is disabled.\n");
	}
	return 0;
}

static int eth_tcqos_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){

	char qos_disc[10];
	int len;

	if (count > 10) {
		len = 10;
	}
	else {
		len = count;
	}
	memset(qos_disc, 0, sizeof(qos_disc));

	if(copy_from_user(qos_disc, buffer, len - 1))
		return -EFAULT;

	qos_disc[len] = '\0';
	if (!strcmp(qos_disc, "PQ")) {
		qos_flag = QOS_SW_PQ;
	}
	else if (!strcmp(qos_disc, "WRR")) {
		qos_flag = QOS_SW_WRR;
	}
	else if (!strcmp(qos_disc, "CAR")) {
		qos_flag = QOS_SW_CAR;
	}
	else if (!strcmp(qos_disc, "HWWRR")) {
		qos_flag = QOS_HW_WRR;
	}
	else if (!strcmp(qos_disc, "HWPQ")) {
		qos_flag = QOS_HW_PQ;
	}
	else {
		qos_flag = NULLQOS;
	}

	return len;
}
#endif

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
#if defined(TCPHY_SUPPORT)
		if (isRT63365){
			if ( (VPint(0xbfb00064) & (0xffff)) == 0x0 ){
				duplex_sw_patch();
				polling_abnormal_irq();
			}
		}
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
	uint32 loopback = 0;
	uint32 advert, lpa;
	uint32 advert2, lpa2;
	uint32 enetMode;

	if (mac_p == NULL)
		return;

#ifdef TCPHY_SUPPORT
	periodChk();
#endif

#ifdef  TC_CONSOLE_ENABLE
    tcconsole_chk();
#endif

#ifdef TC3262_GMAC_NAPI
//#ifdef LOOPBACK_SUPPORT
//	if (!LOOPBACK_MODE(macLoopback))
//#endif
	{
		int txq;

		for (txq = 0; txq < TX_QUEUE_NUM; txq++)
   			macTxRingProc(mac_p, txq);
	}
#endif

	/* save the original enet mode */
	enetMode = mac_p->macPhyLinkProfile_p->enetMode;

	if (swicVendor) {
		mac_p->macPhyLinkProfile_p->enetMode =  LAN_ST_100MB | LAN_ST_FULL_DUPLEX | LAN_ST_LINK_UP;
		if (isRT63365) {
			mac_p->macPhyLinkProfile_p->enetMode &= ~LAN_ST_100MB;
			mac_p->macPhyLinkProfile_p->enetMode |= LAN_ST_1000MB;
		}
	} else {
		mac_p->macPhyLinkProfile_p->enetMode = 0;
		bmcr = miiStationRead(mac_p, MII_BMCR);
		loopback = bmcr & BMCR_LOOPBACK;

		/* ----- Check reset bit & power down bit ----- */
		if (bmcr & (BMCR_RESET | BMCR_PDOWN))
			return;
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
	}

	if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_LINK_UP) {
		if (!loopback) {
			if (enetMode != mac_p->macPhyLinkProfile_p->enetMode) {
				/* According to enet phy link mode to adjust mac full duplex mode */
			}
		}
	} else {
	}

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

	return stats;
}


/* Handling ioctl call */
static int tc3262_gmac_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	int rc = 0;
	gsw_reg reg;
	struct mii_ioctl_data mii;

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

			case RAETH_GSWEXT_REG_READ:  //MTK120625 ///YM
				copy_from_user(&reg, ifr->ifr_data, sizeof(reg));
				reg.val = gswPbusRead(reg.off);
				copy_to_user(ifr->ifr_data, &reg, sizeof(reg));
				break;

			case RAETH_GSWEXT_REG_WRITE:  //MTK120625 ///YM
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

			case RAETH_GSWEXT_PHY_READ:  //MTK120625 ///YM
				copy_from_user(&mii, ifr->ifr_data, sizeof(mii));
				mii.val_out = gswPmiRead(mii.phy_id, mii.reg_num);
				copy_to_user(ifr->ifr_data, &mii, sizeof(mii));
				break;

			case RAETH_GSWEXT_PHY_WRITE:  //MTK120625 ///YM
				copy_from_user(&mii, ifr->ifr_data, sizeof(mii));
				gswPmiWrite(mii.phy_id, mii.reg_num, mii.val_in);
				break;

		#ifdef TCSUPPORT_MT7530_SWITCH_API
			case RAETH_GSW_CTLAPI:
//				printk("RAETH_GSW_CTLAPI\n");
//				if (isRT62806 || isMT7530 || isMT7530FPGA)
				if (isMT7530)
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

	reg = read_reg_word(INT_STATUS);
	write_reg_word(INT_STATUS, reg);
	//write_reg_word(INT_STATUS, reg & read_reg_word(INT_MASK));

	// ----------Packet Received----------------------
	if (reg & (RX_DLY_INT | RX_DONE_INT1 | RX_DONE_INT0)) {
    	ledTurnOn(LED_ETHER_ACT_STATUS);
		if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_100MB)
			ledTurnOn(LED_ETHER_100M_ACT_STATUS);
		else
			ledTurnOn(LED_ETHER_10M_ACT_STATUS);
#ifdef TC3262_GMAC_NAPI
		spin_lock(&gimr_lock);

		if (netif_rx_schedule_prep(dev)) {
			write_reg_word(INT_MASK, read_reg_word(INT_MASK) &
					~(RX_DLY_INT | RX_DONE_INT1 | RX_DONE_INT0));
			__netif_rx_schedule(dev);
		}

		spin_unlock(&gimr_lock);
#else
    	macRxRingProc(dev, dev->weight);
#endif
	}

	if (reg & (RX_COHERENT | TX_COHERENT)) {
		printk("%s err mac_isr INT_STATUS=%08lx\n", dev->name, reg);
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

	write_reg_word(INT_MASK, read_reg_word(INT_MASK) |
		(RX_DLY_INT | RX_DONE_INT1 | RX_DONE_INT0));

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

	/* frankliao modify 20101216 */
//	uint8 *flash_mac_addr = (uint8 *)0xbfc0ff48;
	char flash_mac_addr[6];

	for (i=0; i<6; i++) {
		flash_mac_addr[i] = READ_FLASH_BYTE(flash_base + 0xff48 + i);
	}


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
	spin_lock_init(&phy_lock);

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

#if VLAN_TAG_USED
	dev->features |= NETIF_F_HW_VLAN_TX;
#endif

#ifdef RAETH_CHECKSUM_OFFLOAD
	dev->features |= NETIF_F_IP_CSUM;
#endif

	printk(KERN_INFO
	       "%s: FE MAC Ethernet address: %02X:%02X:%02X:%02X:%02X:%02X\n",
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
	int txq;

	printk(KERN_INFO "%s", version);

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

#if defined(TCSUPPORT_WAN_ETHER)
//register ether wan
	dev = alloc_netdev(sizeof(macAdapter_t), "nas10", ether_setup);
	if (!dev)
		return -ENOMEM;

	tc3262_gmac_wan_dev = dev;

	mac_wan_p = netdev_priv(dev);

  	dev->init = tc3262_gmac_wan_start;

	err = register_netdev(dev);
	if (err)
		return err;
#endif

  	/* ethernet related stats */
	eth_proc = create_proc_entry("tc3162/eth_stats", 0, NULL);
	eth_proc->read_proc = eth_stats_read_proc;
	eth_proc->write_proc = eth_stats_write_proc;

	eth_proc = create_proc_entry("tc3162/gsw_stats", 0, NULL);
	eth_proc->read_proc = gsw_stats_read_proc;
	eth_proc->write_proc = gsw_stats_write_proc;

	/* wplin added 20120703 */
	#if (1)
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
	#endif

#if defined(SQA_VERIFY)
//MTK20120829_MT7530_1588pkt_generation, Start[
	eth_proc = create_proc_entry("tc3162/gen_1588_pkt", 0, NULL);
	eth_proc->write_proc = gen_1588_pkt_write_proc;
//MTK20120829_MT7530_1588pkt_generation, ]End
#endif

  	create_proc_read_entry("tc3162/gsw_link_st", 0, NULL, gsw_link_st_proc, NULL);

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
	for (txq = 0; txq < TX_QUEUE_NUM; txq++) {
		proc_txring[txq] = txq;
		sprintf(proc_txring_name[txq], "tc3162/eth_txring%d", txq);
		create_proc_read_entry(proc_txring_name[txq], 0, NULL, eth_txring_dump_proc, &proc_txring[txq]);
	}
  	create_proc_read_entry("tc3162/eth_rxring", 0, NULL, eth_rxring_dump_proc, NULL);

#if defined(TCSUPPORT_WAN_ETHER)
	create_proc_read_entry("tc3162/eth1_link_st", 0, NULL, eth_wan_link_st_proc, NULL);
#endif

#if defined(QOS_REMARKING) || defined(TCSUPPORT_HW_QOS)
	eth_proc = create_proc_entry("tc3162/eth1_qoswrr", 0, NULL);
	eth_proc->read_proc = eth_qoswrr_read_proc;
	eth_proc->write_proc = eth_qoswrr_write_proc;
#endif

#ifdef TCSUPPORT_QOS
	eth_proc = create_proc_entry("tc3162/eth1_tcqos_disc", 0, NULL);
	eth_proc->read_proc = eth_tcqos_read_proc;
	eth_proc->write_proc = eth_tcqos_write_proc;
#endif

#if defined(TCPHY_SUPPORT)
	/* GSW patch */
	eth_proc = create_proc_entry("tc3162/mac_esd_check", 0, NULL);
	eth_proc->read_proc = gsw_check_read_proc;
	eth_proc->write_proc = gsw_check_write_proc;
#endif

	eth_proc = create_proc_entry("tc3162/stag_to_vtag", 0, NULL);
	eth_proc->read_proc = stag_to_vtag_read_proc;
	eth_proc->write_proc = stag_to_vtag_write_proc;

	#ifdef TCPHY_SUPPORT
#if !defined(TCSUPPORT_CT)
  	create_proc_read_entry("tc3162/eth_port_status", 0, NULL, eth_port_stat_read_proc, NULL);
#endif
	tcephydbgcmd();
	#endif
#ifdef TC_CONSOLE_ENABLE
	create_tcconsole_proc();
	rcu_assign_pointer(send_uart_msg, uart_msg_to_tcconsole);
#endif

	eth_proc = create_proc_entry("tc3162/eth_portmap", 0, NULL);
	eth_proc->read_proc = ethernet_portmap_read_proc;

  	return 0;
}

static void __exit tc3262_gmac_exit(void)
{
	int txq;

	macReset();

	remove_proc_entry("tc3162/eth_stats", 0);
	remove_proc_entry("tc3162/gsw_stats", 0);
	remove_proc_entry("tc3162/gsw_link_st", 0);
#ifdef LOOPBACK_SUPPORT
	remove_proc_entry("tc3162/eth_loopback", 0);
	remove_proc_entry("tc3162/eth_loopback_test", 0);
#endif
   	remove_proc_entry("tc3162/eth_link_st", 0);
#if defined(TCSUPPORT_WAN_ETHER)
	remove_proc_entry("tc3162/eth1_link_st", 0);
#endif
   	remove_proc_entry("tc3162/eth_reg", 0);
	for (txq = 0; txq < TX_QUEUE_NUM; txq++) {
		remove_proc_entry(proc_txring_name[txq], NULL);
	}
   	remove_proc_entry("tc3162/eth_rxring", 0);

#if defined(QOS_REMARKING) || defined(TCSUPPORT_HW_QOS)
	remove_proc_entry("tc3162/eth1_qoswrr", 0);
#endif

#ifdef TCSUPPORT_QOS
	remove_proc_entry("tc3162/eth1_tcqos_disc", 0);
#endif
	remove_proc_entry("tc3162/stag_to_vtag", 0);

	macDrvDescripReset(mac_p);

#if defined(TCSUPPORT_WAN_ETHER)
	unregister_netdev(tc3262_gmac_wan_dev);
#endif
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
#if !defined(TCSUPPORT_CT)
	remove_proc_entry("tc3162/eth_port_status", 0);
#endif
	/*unregister ci command */
	cmd_unregister("tcephydbg");
	#endif
#ifdef TC_CONSOLE_ENABLE
	delete_tcconsole_proc();
	rcu_assign_pointer(send_uart_msg, NULL);
	#endif
}

#if defined(TCSUPPORT_WAN_ETHER)

static int get_wan_port_stat(char *buf){
	uint16 index = 0;
	uint32 reg;
	int port;
	int speed;

	if (!macInitialized) {
		return index;
	}

	port = wan_port_id;

	reg = read_reg_word(GSW_PMSR(port));

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

static int tc3262_gmac_wan_start(struct net_device *dev)
{
	int i;
	char flash_mac_addr[6];

	for (i=0; i<6; i++) {
		flash_mac_addr[i] = READ_FLASH_BYTE(flash_base + 0xff48 + i);
	}

	if( (flash_mac_addr[0] == 0) && (flash_mac_addr[1] == 0) && (flash_mac_addr[2] == 0) &&
	    (flash_mac_addr[3] == 0) && (flash_mac_addr[4] == 0) && (flash_mac_addr[5] == 0) )
		printk(KERN_INFO "The MAC address in flash is null!\n");
	else
  		memcpy(def_mac_addr, flash_mac_addr, 6);

	for (i = 0; i < 6; i++) {
		dev->dev_addr[i] = def_mac_addr[i];
	}

  	/* Hook up with handlers */
  	dev->get_stats		= tc3262_gmac_wan_stats;
  	dev->hard_start_xmit	= tc3262_gmac_wan_tx;
  	dev->open		= tc3262_gmac_wan_open;
  	dev->stop		= tc3262_gmac_wan_close;
  	dev->set_multicast_list = tc3262_gmac_wan_set_multicast_list;
  	dev->do_ioctl		= tc3262_gmac_wan_ioctl;
  	dev->set_mac_address 	= tc3262_gmac_wan_set_macaddr;

#if VLAN_TAG_USED
	dev->features |= NETIF_F_HW_VLAN_TX;
#endif

#ifdef RAETH_CHECKSUM_OFFLOAD
	dev->features |= NETIF_F_IP_CSUM;
#endif

	return 0;
}

int tc3262_gmac_wan_tx(struct sk_buff *skb, struct net_device *dev)
{
//   	printk("tc3262_gmac_wan_tx\n");
#ifdef WAN2LAN
        if(masko_on_off){
		 struct sk_buff *skb2 = NULL;

            //Check the skb headroom is enough or not. shnwind 20100121.
            if(skb_headroom(skb) < TX_STAG_LEN){
                skb2 = skb_copy_expand(skb, TX_STAG_LEN, skb_tailroom(skb) , GFP_ATOMIC);;
            }else{
                skb2 = skb_copy(skb,GFP_ATOMIC);
            }

            if(skb2 == NULL){
                printk("wan2lan failure in sar rx direction for skb2 allocate failure.\n");
            }
            else{
                skb2->mark |= SKBUF_COPYTOLAN;
                macSend(WAN2LAN_CH_ID,skb2);
            }
        }
#endif

	return tc3262_gmac_tx(skb, dev);
}

static struct net_device_stats *tc3262_gmac_wan_stats(struct net_device *dev)
{
	struct net_device_stats *stats;

	stats = &mac_wan_p->stats;

	stats->rx_packets = mac_wan_p->macStat.MIB_II.inUnicastPkts +
		mac_wan_p->macStat.MIB_II.inMulticastPkts;
	stats->tx_packets = mac_wan_p->macStat.MIB_II.outUnicastPkts +
		mac_wan_p->macStat.MIB_II.outMulticastPkts;
	stats->rx_bytes = mac_wan_p->macStat.MIB_II.inOctets;
	stats->tx_bytes = mac_wan_p->macStat.MIB_II.outOctets;
	stats->rx_dropped = mac_wan_p->macStat.MIB_II.inDiscards;
	stats->tx_dropped = mac_wan_p->macStat.MIB_II.outDiscards;
	stats->multicast = mac_wan_p->macStat.MIB_II.inMulticastPkts;
	stats->rx_errors = mac_wan_p->macStat.MIB_II.inErrors;
	stats->tx_errors = mac_wan_p->macStat.MIB_II.outErrors;
	stats->collisions = mac_wan_p->macStat.inSilicon.txExCollisionCnt +
		mac_wan_p->macStat.inSilicon.txCollisionCnt +
		mac_wan_p->macStat.inSilicon.rxCollisionErr;

	return stats;
}
/* Starting up the ethernet device */
static int tc3262_gmac_wan_open(struct net_device *dev)
{
// 	printk("tc3262_gmac_wan_open\n");
	netif_start_queue(dev);
  	return 0;
}
static int tc3262_gmac_wan_close(struct net_device *dev)
{
// 	printk(KERN_INFO "tc3262_gmac_wan_close\n");

  	netif_stop_queue(dev);

  	return 0;
}
/* Setup multicast list */
static void tc3262_gmac_wan_set_multicast_list(struct net_device *dev)
{
	return; /* Do nothing */
}

/* Setting customized mac address */
static int tc3262_gmac_wan_set_macaddr(struct net_device *dev, void *p)
{
	struct sockaddr *addr = p;

	/* Check if given address is valid ethernet MAC address */
  	if (!is_valid_ether_addr(addr->sa_data))
    		return(-EIO);

	/* Save the customize mac address */
  	memcpy(dev->dev_addr, addr->sa_data, dev->addr_len);
#ifndef TCSUPPORT_MT7510_FE
	write_reg_word(GDMA2_MAC_ADRL, addr->sa_data[2]<<24 | addr->sa_data[3]<<16 | \
                               addr->sa_data[4]<<8  | addr->sa_data[5]<<0);
	write_reg_word(GDMA2_MAC_ADRH, addr->sa_data[0]<<8  | addr->sa_data[1]<<0);
#endif
	return 0; /* Do nothing */
}
/* Handling ioctl call */
static int tc3262_gmac_wan_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	int rc;

	if (macInitialized) {
		rc = generic_mii_ioctl(&mac_p->mii_if, if_mii(ifr), cmd, NULL);
	} else {
		rc = -EOPNOTSUPP;
	}

	return rc;
}
#endif


/* Register startup/shutdown routines */
module_init(tc3262_gmac_init);
module_exit(tc3262_gmac_exit);
MODULE_LICENSE("Proprietary");
