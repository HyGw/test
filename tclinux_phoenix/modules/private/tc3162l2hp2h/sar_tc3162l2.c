/*
** $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/private/tc3162l2hp2h/sar_tc3162l2.c#1 $
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
** $Log: sar_tc3162l2.c,v $
** Revision 1.21  2011/08/12 08:22:09  xmdai_nj
** #11299
**
** Revision 1.20  2011/08/12 07:02:43  xmdai_nj
** #11297:Upstream throughput can not reach to 100% when ADSL is set to Annex M mode.
**
** Revision 1.19  2011/08/05 03:14:01  xyzhu_nj
** #11006: ct com merge to main trunk.(phase 3)
** 1. Add TCSUPPORT_COMPILE tag for ct com compile option;
** 2. Delete files in filesystem that CT_COM not used;
** 3. Merge the lastes bugfix from ct com branch to main trunk.(TLB7.1.2.20-51 -- 57);
** 4. update mic.
**
** Revision 1.18  2011/07/24 12:00:14  xyzhu_nj
** #11006: Merge China telecom branch to Main trunk.
**
** Revision 1.17  2011/07/07 07:46:13  shnwind
** RT63260 & RT63260 auto_bench support
**
** Revision 1.16  2011/07/07 01:18:54  xmdai_nj
** #10616:AnnexM throughput
**
** Revision 1.15  2011/06/30 09:07:25  serenahuang_hc
** remove compile option WITHVOIP
**
** Revision 1.14  2011/06/23 04:41:28  serenahuang_hc
** 1.remove to put packets to HH priority when the destPort is match.
** 2.Put rtp packets to HH priority when qos_flag not be selected as WRR or PQ
**
** Revision 1.13  2011/06/13 05:59:55  frankliao_hc
** add rt65168 support
**
** Revision 1.12  2011/06/03 08:46:26  xhshi
** #9797 l2m throughput issue
**
** Revision 1.11  2011/04/12 14:48:54  shnwind
** remove debug message
**
** Revision 1.10  2011/02/21 07:30:37  shnwind
** fix bug : treg_tstbr will set wrong in tc3182
**
** Revision 1.9  2011/01/21 14:41:35  shnwind
** fix Bug 8620
**
** Revision 1.8  2011/01/20 13:33:42  shnwind
** fix Bug 8599
**
** Revision 1.7  2010/12/31 10:33:34  xyzhu_nj
** #8261: sp / wrr qos enhancement.
**
** Revision 1.6  2010/12/27 04:59:13  here
** [Bug fix]Compiler error with TC3182 profile with "Support 802.1p Remark function"
**
** Revision 1.5  2010/12/24 11:30:08  here
** [Ehancement]Support 802.1p Remark function
**
** Revision 1.4  2010/12/24 10:34:36  serenahuang_hc
** remove tc3162/voip proc make
**
** Revision 1.3  2010/12/02 06:24:34  shnwind
** fix some warning
**
** Revision 1.2  2010/11/30 10:57:43  rclv_nj
** bug5954
**
** Revision 1.1.1.1  2010/09/30 21:14:55  josephxu
** modules/public, private
**
** Revision 1.17  2010/09/29 11:06:28  serenahuang_hc
** modify isPriorityPkt() for check arp packet & tcp data packet.
**
** Revision 1.16  2010/09/10 04:31:30  xyzhu_nj
** #6856: Add support old qos mechanism.
**
** Revision 1.15  2010/09/03 16:39:12  here
** [Ehance] TC3182 GMAC Driver is support TC-Console & WAN2LAN function.
**
** Revision 1.14  2010/09/03 13:47:23  xyyou
** add new version control
**
** Revision 1.13  2010/08/23 06:28:11  serenahuang_hc
** modifid files for RTP QoS
**
** Revision 1.12  2010/07/22 11:47:34  xhshi
** #6353
**  1.Add some pvc links,and then delete some,occasionally we find that delete
**  the wrong pvc link.for example:we want to delete the pvc with '0.35',but actually delete
**  the pvc with '0.36'.
**  2.If we add the same pvc more times,after that,we cannot add other
**  pvc whether the pvc is correct or not.
**  3."atmcmd pvc --delete 0.0" can be used as valid command.
**
** Revision 1.11  2010/07/22 05:19:10  jlliu_nj
** Bug#6345:Support Version control mechanism in mldsnooping module,mac driver,sar driver:add line break in version
**
** Revision 1.10  2010/07/22 02:48:55  jlliu_nj
** Bug#6345:Support Version control mechanism in mldsnooping module,mac driver,sar driver
**
** Revision 1.9  2010/07/14 15:55:36  frankliao_hc
** BUGFIX:
** remove hareware vlan untag setting in initMOPA function
**
** Revision 1.8  2010/07/06 13:39:32  xhshi
** #6183
** 1.Do f5 loopback diagnostic which use atmcmd like this:"atmcmd oam --start 0.35 f5 --type segment --repetition 2 --timeout 40000"
** 2.CPE hang,and after a while reboot.
**
** Revision 1.7  2010/06/23 04:30:56  xmdai_nj
** bug#5814,pppoa cannot dail and get ip.
**
** Revision 1.6  2010/06/17 04:36:55  serenahuang_hc
** If packet dest port match rtp_port put it into the highest priority queue.
**
** Revision 1.5  2010/06/09 08:26:33  shnwind
** tc3182 asic support
**
** Revision 1.4  2010/06/07 04:29:06  xyzhu_nj
** [#5858]: support 802.1p remark.
**
** Revision 1.3  2010/05/21 05:15:01  xyzhu_nj
** [#5762]: Support CAR QoS in Linux.
**
** Revision 1.2  2010/04/14 02:36:28  xhshi
** #5431 support driver interface
**
** Revision 1.1.1.1  2010/04/09 09:35:29  feiyan
** New TC Linux Make Flow Trunk
**
** Revision 1.18  2010/03/22 14:14:52  yzwang_nj
** [Bug#5128] Implement TR068 LED
**
** Revision 1.17  2010/02/22 03:17:39  rodney
** RED
**
** Revision 1.16  2010/02/05 10:16:33  ian
** Remove keyword "Huawei" for adslcmd and atmcmd
**
** Revision 1.15  2010/02/03 02:42:40  here
** [Ehancement]Remove the check mechanism for wifi driver that is released by trendchip crop.
**
** Revision 1.14  2010/02/02 12:13:13  here
** [Ehancement]1.RT3092 Chip is support polling mode when the adsl at init state.
** 2. Support the check mechanism for wifi driver that is released by trendchip crop.
**
** Revision 1.13  2010/01/15 17:25:56  pork
** wan2lan porting
**
** Revision 1.12  2010/01/15 09:38:45  here
** [BugFix]Fixed TC3162U Platform the sar interface's rx descriptor is mismatch at SMB Routing throughput(WiFi interface is up)
**
** Revision 1.11  2010/01/10 16:06:48  here
** [Ehancement]TC3162U MAC EEE is operated at 100M-FD, SAR interface is accroding the SAR_CLK to calculate atm rate.
**
** Revision 1.9  2010/01/10 13:45:21  here
** [BugFix] Help shnwind to commit sar modules
**
** Revision 1.3  2009/12/30 12:33:21  yzwang_nj
** [Bug#4409] Add TR064 Parameters
**
** Revision 1.2  2009/12/30 03:01:05  xhshi
** [#4366]
** Implement ATM F5 Loopback Diagnostic function in linux platform
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
** Revision 1.4  2006/02/14 11:16:14  lino
** workaround to prevent the HW SAR issue abnormal IRQ status
**
** Revision 1.3  2006/02/07 07:24:21  lino
** add tx/rx/cc descriptor dump, process abnormal irq status and let atm reset work
**
** Revision 1.2  2006/01/17 07:14:23  lino
** set pvc qos priority CBR > rt-VBR > nrt-VBR > UBR
**
** Revision 1.1.1.1  2005/11/02 05:45:19  lino
** no message
**
** Revision 1.10  2005/09/29 12:10:01  bread.hsu
** reduce code size and beautify code structure of tc3162l2 sar driver
**
** Revision 1.9  2005/09/28 13:45:42  jasonlin
** Fix the compilation error after adding rtVBR and nrtVBR
**
** Revision 1.8  2005/09/27 08:03:06  bread.hsu
** adding IMEM support for Tc3162L2
**
** Revision 1.7  2005/09/22 13:22:02  jasonlin
** Add nrtVBR selection on CI command and TC new web page
**
** Revision 1.6  2005/09/21 04:46:22  bread.hsu
** fix the L2 sar and mac driver counter and register message display error
**
** Revision 1.5  2005/09/14 11:10:51  bread.hsu
** TC3162L2 SAR and MAC driver synchronize with TC3162
**
** Revision 1.4  2005/08/09 03:06:34  bread.hsu
** L2 SAR any-byte alignment test function error fixed
**
** Revision 1.3  2005/06/22 14:28:21  jasonlin
** fix the merge error of tc3162l2sar.c
**
** Revision 1.2  2005/06/22 14:23:41  jasonlin
** Sync with main trunk version 2.2.28
**
** Revision 1.1  2005/06/14 10:02:05  jasonlin
** Merge TC3162L2 source code into new main trunk
**
** Revision 1.14  2004/12/20 05:15:09  lino
** prevent multiple call atmAal5VcClose to cause cannot open new vc anymore
**
** Revision 1.13  2004/11/18 11:05:19  lino
** fixed CBR's PCR lower than line rate, UBR traffic will be blocked
**
** Revision 1.12  2004/11/15 03:41:20  lino
** add UBR QoS enhance, let UBR has lower priority than CBR and VBR
**
** Revision 1.11  2004/11/05 10:27:12  svtseng
** Delete the QOS parameter seting must plus 2 in last modification
**
** Revision 1.10  2004/11/05 06:18:26  svtseng
** 1. Fix the QOS setting in VBR
** 2. The caculated value should be raise two levels
** 3. Change timebase counter.
** 4. Add 1 to the atmRateCalCulate's timebase parameter
**
** Revision 1.9  2004/10/28 10:30:11  svtseng
** Fix the QOS traffic type (UBR) compete with others
**
** Revision 1.8  2004/10/21 13:31:49  svtseng
** Fix the bug in VAR qos setting, the pci_init can not be equal to scr_init,
** or the rate would be wrong
**
** Revision 1.7  2004/10/21 05:36:19  lino
** change oam cell format and add oam debug messages
**
** Revision 1.6  2004/10/13 05:22:02  svtseng
** Fix the QOS parameter caculation by system CLK
**
** Revision 1.5  2004/10/12 06:41:16  lino
** add more sar discard counters
**
** Revision 1.4  2004/09/14 03:58:32  lino
** fixed will not hunt vcpool oam loopback cell
**
** Revision 1.3  2004/08/27 12:18:51  lino
** add qos calculation and clean code
**
** Revision 1.2  2004/08/09 11:36:18  lino
** add vpi/vci=0/16 cells to packet
**
** Revision 1.1  2004/07/02 08:06:01  lino
** tc3160 and tc3162 code merge
**
*/
#include <linux/kernel.h>
#include <linux/atmdev.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <asm/addrspace.h>
#include <asm/io.h>
#include <asm/tc3162/tc3162.h>
#include <asm/tc3162/TCIfSetQuery_os.h>
#include <asm/tc3162/ledcetrl.h>
#include "../../version/tcversion.h"
#ifdef DMA_API
#include <linux/dma-mapping.h>
#endif
#include "../tcphy/tcswitch.h"
#include "../tcphy/tcconsole.h"
#include "tsarm.h"
#ifdef SAR_VERIFY
#include "tsarm_verify.h"
#endif

#ifdef L2_AUTOPVC
#include "autoPVC.h"
#include "aal5.h"
extern uint32 g_manageFlag;
#endif

//add by brian
#ifdef CWMP
uint32 g_f5loopback_rxtime = 0;
uint32 g_f5loopback_txtime = 0;
uint32 maxtimeindex = 0xFFFFFFFF;
static uint32 cwmpSavedInF5Pkts = 0;
static uint32 cwmpSavedOutF5Pkts = 0;
uint8 internal_error = 0;
uint32 cwmpflag = 0;
//static uint32 cwmpSavedInF4Pkts = 0;
#endif

#if defined(TCSUPPORT_CT_SWQOS)
static uint8 atmTxQosQueueSize = ATM_TX_VC_DESCR_P3_NUMMAX;
#endif

/************************************************************************
*				E X T E R N A L   R E F E R E N C E S
*************************************************************************
*/

#ifdef SAR_VERIFY
extern uint16 test_flag;
#endif
#ifdef WAN2LAN
extern int masko_on_off; //use this flag control if open wan2lan function
#endif

#ifdef TR068_LED
extern int internet_led_on;
extern int internet_trying_led_on;
#endif

#if defined(TCSUPPORT_CT_SWQOS)
extern int (*sw_pktqosEnqueue) (struct sk_buff * bp);
extern void (*sw_PKTQOS_SET_STOP) (void);
extern void (*sw_PKTQOS_CLEAR_STOP) (void);
#endif

/************************************************************************
*				V E R S I O N   C O N T R O L
*************************************************************************
*/


/************************************************************************
*							C O N S T A N T S
*************************************************************************
*/
#if defined(TCSUPPORT_CT)
#define DYNAMIC_ALLOC_SKB		1
#endif
#define DYNAMIC_ALLOC_DMA_BUF		1
#if 0 /*move tsarm.h*/
#define RX_BUF_LEN 	(2048 - 16 - 64 - (sizeof(struct skb_shared_info)))
#endif
//#define QOS_UBR_ENHANCE		1
#if defined(TCSUPPORT_CT)	
#ifndef TCSUPPORT_QOS
#define QOS_REMARKING  0//1  /*Rodney_20090724*/
#endif
#if defined(TCSUPPORT_C1_CUC)
#define TCSUPPORT_HW_QOS
#endif
#else
#ifndef TCSUPPORT_QOS
#define QOS_REMARKING  1  /*Rodney_20090724*/
#endif
#define TCSUPPORT_HW_QOS
#endif

#ifdef QOS_REMARKING  /*Rodney_20090724*/
#define QOS_REMARKING_MASK    0x00000007
#define QOS_REMARKING_FLAG    0x00000001
//#define QOS_DMAWRR_USERDEFINE  0x01
#endif
#if defined(TCSUPPORT_CT)
#define QOS_DMAWRR_USERDEFINE  0x01
#endif

#if !defined(TCSUPPORT_CT)
#if defined (QOS_REMARKING) || defined(TCSUPPORT_HW_QOS)
#define QOS_DMAWRR_USERDEFINE 0x1
#endif
#endif

#ifdef CONFIG_NET_SCHED  /*Rodney_20091115*/
//#define QOS_DROP_DUMP  1
#define QOS_QDISC_NOR  1  /*normal*/
#define QOS_QDISC_RED  2
#define QOS_QDISC_WRED 4
#endif

#define SAVED_IRQ_MAX_ENTRY	16

#define DEF_PRIORITY_PKT_CHK_LEN		100

#ifdef ALIGN32
#define ALIGN32BYTESBASE	32
#endif

#ifdef WAN2LAN
#define SKBUF_COPYTOLAN (1<<26) //xyzhu_nj_091105:borrow this bit to show if the packet is used for wan2lan
#endif
/************************************************************************
*							D A T A   T Y P E S
*************************************************************************
*/
#ifdef CMD_API
typedef struct
{
    uint32 crc_errs;
    uint32 oversized_sdus;
    uint32 short_pkt_errs;
    uint32 len_errs;
} PVCSTATS;

PVCSTATS	pvc_stats[ATM_VC_MAX];
#endif

/************************************************************************
*							M A C R O S
*************************************************************************
*/
#define dbg_pline_1(x)		printk("%s", x)
#define dbg_plineb_1(x, y)	printk("%s%02x", x, (uint8) y)
#define dbg_plinew_1(x, y)	printk("%s%04x", x, (uint16) y)
#define dbg_plinel_1(x, y)	printk("%s%08lx", x, (uint32) y)

#define puts(s)				printk("%s", s)
#define printf				printk
//#define delay1ms(x)			mdelay(x)
/************************************************************************
*				F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
void atmTxDescrInit(void);
void atmRxDescrInit(void);
#if defined(TCSUPPORT_CT)
#ifdef DYNAMIC_ALLOC_SKB
int atmRxSkbAlloc(uint8 vc);
void atmRxSkbFree(uint8 vc);
#endif
#endif
void atmRxIrqQueueSet( uint32 irqBase, uint32 irqLength );
void atmTxDescrFree(uint8 vc, uint8 free_any);
irqreturn_t atmIsr(int irq , void *dev);

uint8 atmCcHandler(uint8 *cellp);
uint8 atmOamHandler(atmRxCcDescr_t *atmRxCcDescrp, uint8 vc);
uint8 atmOamDataReq(uint8 *data_p, uint8 pti, uint8 vc);
uint8 atmVcNumberGet(uint8 vpi, uint16 vci);
struct sk_buff  *atmAal5DataInd(struct sk_buff *skb, uint8 vc, uint32 len);
void atmReset(void);
uint8 atmDataReq(struct sk_buff *skb, uint8 vc, uint8 priority);
void sarDrvDescripReset(int freeRxSkb);
int atmOamOpen(void);
void atmIrqStSaved(uint32 irqst, uint8 vc);
extern int getXdslSpeed(void);
extern uint8 getXdslModeType(void);
int getATMState(char *stateATM);
static int setAtmQosRed(qosProfile_t *qos_p,	uint32 vc);
static int update_qosred(uint32 vc);

/************************************************************************
*						P U B L I C   D A T A
*************************************************************************
*/
atmCtrl_t atmCtrl;
atmCtrl_t *atm_p = &atmCtrl;

//#ifdef QOS_REMARKING  /*Rodney_20090724*/
#if defined(QOS_REMARKING) || defined(TCSUPPORT_HW_QOS)
static int qos_wrr_info[5] = {0};
static int max_prio = 0;
static uint8 qos_wrr_user = 0x00;
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
static int qos_flag = NULLQOS;

#endif

#ifdef CONFIG_NET_SCHED  /*Rodney_20091115*/
static int qos_qdisc_mode = 1;
#ifdef QOS_DROP_DUMP
#define MAX_LOG_PKT  10000
static unsigned short int qos_drop_flag[MAX_LOG_PKT] = {0};
#endif  /*QOS_DROP_DUMP*/
#endif  /*CONFIG_NET_SCHED*/
static uint32 atmRxBufferSize=0;

static int pcr_modify_flag = 0;//The mark of pcr's value modified by client,1:modified,0:no
static int g_qos_pcr  = 0;// if the pcr value is modified by client, then the pcr value will be saved in the g_qos_pcr
/************************************************************************
*						E X T E R N A L   D A T A
*************************************************************************
*/
static uint8 sarDebugFlag = 0;
static uint8 sarSoftReset = 0;
static uint8 isAdslUp = 0;
#ifdef CONFIG_MIPS_TC3262
static spinlock_t sarLock;
#endif
/************************************************************************
*						P R I V A T E   D A T A
*************************************************************************
*/
//atmTxDescr_t *atmTxVcFreeDescrp[ATM_VC_MAX];
//atmTxDescr_t *atmTxVcBusyDescrp[ATM_VC_MAX];
atmRxDescr_t *atmRxVcDescrp[ATM_VC_MAX];

uint32 volatile atmTxVcTotalCnt;
uint16 volatile atmTxVcCnt[ATM_VC_MAX];
uint8 atmTxCcCnt;
atmTxDescr_t *atmTxVcFreeDescrp[ATM_TX_PRIORITY_MAX][ATM_VC_MAX];
atmTxDescr_t *atmTxVcBusyDescrp[ATM_TX_PRIORITY_MAX][ATM_VC_MAX];
uint16 volatile atmTxPriVcCnt[ATM_TX_PRIORITY_MAX][ATM_VC_MAX];

atmTxCcDescr_t *atmTxCcFreeDescrp;
atmRxCcDescr_t *atmRxCcBusyDescrp;

#ifdef DYNAMIC_ALLOC_DMA_BUF
struct atmTxDescrPoolAddr_s{
	atmTxDescr_t * pTxDescr[ATM_TX_PRIORITY_MAX];
	atmTxCcDescr_t * pTxOamDescr;
} atmTxDescrPoolAddr[ATM_VC_MAX];
uint8 *atmTxDescrPool = NULL;
int atmTxDescrPoolSize = 0;

atmTxCcDescrPool_t *atmTxCcDescrPool = NULL;
atmRxCcDescrPool_t *atmRxCcDescrPool = NULL;
atmRxCcCell_t *ccBuf = NULL;
atmRxOamDescrPool_t *oamBuf = NULL;
atmRxDescrPool_t *atmRxDescrPool = NULL;
uint32 *atmRxIrq = NULL;
#if DMA_API
uint8* atmTxDescrPool_phys_p = NULL;
atmTxCcDescrPool_t* atmTxCcDescrPool_phys_p = NULL;
atmRxCcDescrPool_t* atmRxCcDescrPool_phys_p = NULL;
atmRxCcCell_t* ccBuf_phys_p = NULL;
atmRxOamDescrPool_t* oamBuf_phys_p = NULL;
atmRxDescrPool_t* atmRxDescrPool_phys_p = NULL;
uint32* atmRxIrq_phys_p = NULL;
atmCell_t *atmCellCirBuf = NULL;
atmCell_t* atmCellCirBuf_phys_p=NULL;
#endif
#else
atmTxCcDescrPool_t atmTxCcDescrPool;
atmRxCcDescrPool_t atmRxCcDescrPool;
atmRxCcCell_t ccBuf[ATM_RX_CC_DESCR_NUMMAX];
atmRxOamDescrPool_t oamBuf[ATM_VC_MAX];
atmTxDescrPool_t atmTxDescrPool[ATM_VC_MAX];
atmRxDescrPool_t atmRxDescrPool[ATM_VC_MAX];
uint32 atmRxIrq[IRQ_MAX_ENTRY];
#endif

atmTxCcDescr_t *atmTxOamFreeDescrp[ATM_VC_MAX];
atmRxCcDescr_t *atmRxOamBusyDescrp[ATM_VC_MAX];

typedef struct saved_atm_irq_st_s {
	uint32 irqst;
	uint8  vc;
} saved_atm_irq_st_t;

saved_atm_irq_st_t savedAtmIrqSt[SAVED_IRQ_MAX_ENTRY];
uint32 savedAtmIrqIndex = 0;

atmConfig_t atmCfgTable = { 0 };
atmConfig_t atmCfgBackup = { 0 };
qosProfile_t qosRecord[ATM_VC_MAX];

int atmTxQueSize[4] = {	ATM_TX_VC_DESCR_P0_NUMMAX,
						ATM_TX_VC_DESCR_P1_NUMMAX,
						ATM_TX_VC_DESCR_P2_NUMMAX,
						ATM_TX_VC_DESCR_P3_NUMMAX};
module_param_array(atmTxQueSize, int, NULL, 0);

/* priority packet check parameters */

int priPktChkLen = DEF_PRIORITY_PKT_CHK_LEN;
#ifdef CONFIG_QOS
#ifdef TCSUPPORT_SBTHROUGHPUT_ENHANCE
extern int tc_qos_switch;
#endif
#endif
#if defined(TCSUPPORT_CT)
#ifdef DYNAMIC_ALLOC_SKB
/*Fix dynamic alloc bug*/
int g_NoNeedFreeSKB = 0;
#endif
#endif
module_param(priPktChkLen, int, 0);
#ifdef PURE_BRIDGE
	int priPktChk = 0;
#else
int priPktChk = 1;
#endif
module_param(priPktChk, int, 0);

uint8 oamAisCell[52] = {
	/* ATM Header, without HEC */
	0x00, 0x00, 0x00, 0x00,
	/* 48 octets payload */
	0x10, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x03, 0xB9
};

uint8 oamRdiCell[52] = {
	/* ATM Header, without HEC */
	0x00, 0x00, 0x00, 0x00,
	/* 48 octets payload */
	0x11, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x00, 0xAF
};

uint8 oamLoopBackReqCell[52] = {
	/* ATM Header, without HEC */
	0x00, 0x00, 0x00, 0x00,
	/* 48 octets payload */
	0x18, 0x01, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x03, 0x93
};

uint8 oamContinuityCheckReqCell[52] = {
	/* ATM Header, without HEC */
	0x00, 0x00, 0x00, 0x00,
	/* 48 octets payload */
	0x14, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x03, 0x87
};

uint8 oamF5AisCell[48] = {
	/* 48 octets payload */
	0x10, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x03, 0xB9
};

uint8 oamF5RdiCell[48] = {
	/* 48 octets payload */
	0x11, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x00, 0xAF
};

uint8 oamF5LoopBackReqCell[48] = {
	/* 48 octets payload */
	0x18, 0x01, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x03, 0x93
};

uint8 oamF5ContinuityCheckReqCell[48] = {
	/* 48 octets payload */
	0x14, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A,
	0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x03, 0x87
};
typedef  struct QosParm_s{
	uint16 cellRate;
	uint16  init;
	uint8 dec;
} QosParm_t;
#define MAX_ATMQOS_CNT        17  /* 1+8*2 = 17; 1:dslupRate;8 PVCs, 2:pcr and scr */
QosParm_t   ATMQosParam[MAX_ATMQOS_CNT] = {0};

#if defined(TCSUPPORT_CT_SWQOS)
uint32 treg_irqm = 0x402;		//TX_SW_DIS_M, TX_M_DONE_M, been masked
#else
uint32 treg_irqm = 0x403;		//TX_SW_DIS_M, TX_M_DONE_M, and TX_U_DONE_M been masked
#endif

#ifdef TC3262_FPGA
int32 treg_tstbr = 9;	   //for FPGA
#else
uint32 treg_tstbr = 3600;	   //for ASIC
#endif
uint32 treg_tslr = 65537;	//0x00010001		default value

uint8 irqrereadflag = 0;
uint32 irqrereadmax = 0;
uint32 irqrereadtime = 0;

/************************************************************************
*			    L I N U X   S P E C I F I C     S E C T I O N
*************************************************************************
*/
static int tc3162_atm_open(struct atm_vcc *vcc);
static void tc3162_atm_close(struct atm_vcc *vcc);
static int tc3162_atm_ioctl(struct atm_dev *dev, unsigned int cmd, void *arg);
static int tc3162_atm_send(struct atm_vcc *vcc, struct sk_buff *skb);
static int tc3162_atm_change_qos (struct atm_vcc *vcc, struct atm_qos *qos,int flags);

static struct atm_dev *tc3162_atm_dev;
static struct atmdev_ops tc3162_atm_ops = {
	.open =			tc3162_atm_open,
	.close =     	tc3162_atm_close,
	.ioctl =      	tc3162_atm_ioctl,
	.send =       	tc3162_atm_send,
	.change_qos = 	tc3162_atm_change_qos,
	.owner = 		THIS_MODULE
};

/************************************************************************
*				F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

static uint8 RFC1483_B_LLC_HDR[] = {
	0xaa, 0xaa, 0x03, 0x00, 0x80, 0xc2, 0x00, 0x07, 0x00, 0x00
};
static uint8 RFC1483_R_LLC_HDR[] = {
	0xaa, 0xaa, 0x03, 0x00, 0x00, 0x00, 0x08, 0x00
};
static uint8 RFC1483_B_LLC_FCS_HDR[] = {
	0xaa, 0xaa, 0x03, 0x00, 0x80, 0xc2, 0x00, 0x01, 0x00, 0x00
};
static uint8 RFC1483_B_VC_HDR[] = {
	0x00, 0x00
};
static uint8 PPPOA_LLC_HDR[] = {
	0xfe, 0xfe, 0x03, 0xcf, 0x00, 0x21
};
static uint8 PPPOA_VC_HDR[] = {
	0x00, 0x21
};

/*_____________________________________________________________________________
**      function name: align32Byte
**      descriptions:
**          If you allocate memory with non-cache area and it is close neighbor cache area, If this not
**
**      parameters:
**           addr: Specify the address that you want to 32 bytes alignment.
**
**      global:
**           None
**
**      return:
**          32 bytes alignment address.
**
**      call:
**           None
**
**      revision:
**      1. Here 2008/09/08
**____________________________________________________________________________
*/
uint32
align32Byte(uint32 addr)
{
	#ifdef ALIGN32
	return (addr+31)&0xFFFFFFE0;
	#else
	return addr;
	#endif
}/*end align32Byte*/

/*20100921_serena_modify: add inupu arg "len" to check tcp packet*/	
static uint8 isPriorityPkt(uint8 *cp, uint8 *priority, unsigned int len)
{
	uint16 etherType;
	uint8 ipVerLen;
	uint8 ipProtocol;
	uint8 tcpFlags;
	uint16 pppProtocol;
	uint8 ipv6_protocol, ipv6_type;

#ifdef TCSUPPORT_CWMP_FAST_GET
	uint16 tcpSrcPort = 0;
	uint16 dnsPort = 0;
#endif

	/* RFC1483 LLC Bridge mode */
	if (memcmp(cp, RFC1483_B_LLC_HDR, 7) == 0) {
		cp += sizeof(RFC1483_B_LLC_HDR);
		goto eth_header;
	/* RFC1483 LLC Routing mode */
	} else if (memcmp(cp, RFC1483_R_LLC_HDR, sizeof(RFC1483_R_LLC_HDR)) == 0) {
		cp += sizeof(RFC1483_R_LLC_HDR);
		goto ipv4_header;
	/* PPPoA LLC Routing mode */
	} else if (memcmp(cp, PPPOA_LLC_HDR, sizeof(PPPOA_LLC_HDR)) == 0) {
		cp += sizeof(PPPOA_LLC_HDR);
		goto ipv4_header;
	/* PPPoA VCMUX Routing mode */
	} else if (memcmp(cp, PPPOA_VC_HDR, sizeof(PPPOA_VC_HDR)) == 0) {
		cp += sizeof(PPPOA_VC_HDR);
		goto ipv4_header;
	/* RFC1483 VCMUX Bridge mode */
	} else if (memcmp(cp, RFC1483_B_VC_HDR, sizeof(RFC1483_B_VC_HDR)) == 0) {
		cp += sizeof(RFC1483_B_VC_HDR);
		goto eth_header;
	/* try RFC1483 VCMUX Routing mode */
	} else {
		goto ipv4_header;
	}

	return 0;

eth_header:
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
			*priority = 2;
			return 1;
		/* check if IP protocol */
		} else if (pppProtocol != 0x0021) {
			return 0;
		}
		/* skip ppp protocol */
		cp += 2; 					/* 6: PPPoE header 2: PPP protocol */
	} else if (etherType == 0x8863) {
		*priority = 2;
		return 1;
	/*20100921_serena_modify: check whether arp packet*/	
	} else if (etherType == 0x0806) {
		*priority = 0;
		return 1;	
	} 
	else if (etherType == 0x86dd) {
		cp += 6;
		ipv6_protocol = *(uint8*)cp;
		if (ipv6_protocol == 0x3a) {
			cp += 34;
			ipv6_type = *(uint8*)cp;
			if (ipv6_type == 0x87) {
				//printk("check ipv6 icmp packet.jiffies is %u\n", jiffies);
				*priority = 0;
				return 1;
			}
		}
	}
	else {
		/* check if ip packet */
		if (etherType != 0x0800)
			return 0;
	}

ipv4_header:
	/* check if it is a ipv4 packet */
	ipVerLen = *cp;
	if ((ipVerLen & 0xf0) != 0x40)
		return 0;

	/* get ip protocol */
	ipProtocol = *(cp + 9);

	/* check if ICMP/IGMP protocol */
	if ((ipProtocol == 1) || (ipProtocol == 2)) {
		*priority = 2;
		return 1;
	}

#ifdef TCSUPPORT_CWMP_FAST_GET
	/* dns packet enqueue first queue */
	if (ipProtocol == 17) {
		dnsPort = *((uint16*)(cp + 13 + 9));
		if (dnsPort == 0x35) {
			//printk("\r\nisPriorityPkt():It is dns query packet\n");
			*priority = 0;
			return 1;
		}
	}
#endif

	/* check if TCP protocol */
	if (ipProtocol != 6)
		return 0;

#ifdef TCSUPPORT_CWMP_FAST_GET

	
	/* tcp */
	tcpSrcPort = *((uint16*)(cp + 11 + 9));

	//printk("\r\nisPriorityPkt():before-----------debug, tcpSrcPort = %x\n", tcpSrcPort);

	if (tcpSrcPort == 0x1d7b) {

		//printk("\r\nisPriorityPkt():it is tr069 connection request port packet\n");
		*priority = 0;
		return 1;
	}
#endif

	/* align to TCP header */
	cp += (ipVerLen & 0x0f) << 2;
	/* get TCP flags */
	tcpFlags = *(cp + 13);
	
	/* check if TCP syn/ack/psh ack/fin ack */ /*20100921_serena_modify*/
	if ((((tcpFlags & 0x10) == 0x10) || (tcpFlags == 0x02)) && (len < priPktChkLen)) {
		*priority = 2;
		return 1;
	}

	return 0;
}

void
delay1ms(
	int ms
)
{
	volatile uint32 timer_now, timer_last;
	volatile uint32 tick_acc;
	uint32 one_tick_unit = 1 * SYS_HCLK * 1000 / 2;
	volatile uint32 tick_wait = ms * one_tick_unit;
	volatile uint32 timer1_ldv = VPint(CR_TIMER1_LDV);

	tick_acc = 0;
 	timer_last = VPint(CR_TIMER1_VLR);
	do {
   		timer_now = VPint(CR_TIMER1_VLR);
       	if (timer_last >= timer_now)
       		tick_acc += timer_last - timer_now;
      	else
       		tick_acc += timer1_ldv - timer_now + timer_last;
     	timer_last = timer_now;
	} while (tick_acc < tick_wait);
}

#ifdef DYNAMIC_ALLOC_DMA_BUF
static void atmTxDescrAlloc(void) {
	uint8 vc, priority, i, * ptr;
	uint32 ptr_data, ptr_oam, mem_size=0, cnt_pri, cnt_vc, cnt_pri_total=0;

		/* the actual total size of each priorities will dynamically change, we should caculate every time enter this function*/
	for (priority = 0; priority < ATM_TX_PRIORITY_MAX; priority++)
		cnt_pri_total += atmTxQueSize[priority];

	mem_size = ATM_VC_MAX * ( (cnt_pri_total * sizeof(atmTxDescr_t))
								+ (ATM_TX_CC_DESCR_NUMMAX * sizeof(atmTxCcDescr_t)) );

	#ifndef DMA_API
    ptr = (uint8 *) kmalloc(mem_size, GFP_KERNEL);
	if (ptr == NULL){
		printk("ATM Tx Descriptor Memory Allocation Fail!\n");
		return;
	}
	/* record tx descr pool and size */
	atmTxDescrPool = ptr;
	atmTxDescrPoolSize = mem_size;
	/*clean this block of mem*/
	memset(ptr, 0x00, mem_size);
    #else
    atmTxDescrPool=(uint8 *)dma_alloc_coherent(NULL, mem_size, (dma_addr_t *) &atmTxDescrPool_phys_p, GFP_KERNEL);
	if (atmTxDescrPool == NULL) {
		printk("unable to dma_alloc_coherent atmTxDescrPool structure.\n");
		return;
	}
	ptr=atmTxDescrPool;
	atmTxDescrPoolSize=mem_size;
    #endif
	/*	arrange memory for Tx descriptor	*/
	for (vc = 0; vc < ATM_VC_MAX; vc++){
		/*	cnt_vc == the maximum num of bytes each vc will have	*/
		cnt_vc = vc * ( sizeof(atmTxDescr_t)*cnt_pri_total +
					 	sizeof(atmTxCcDescr_t)*ATM_TX_CC_DESCR_NUMMAX );
		for (priority = 0; priority < ATM_TX_PRIORITY_MAX; priority++){
			for (i = 0, cnt_pri = 0 ; i < priority; i++){
				cnt_pri += atmTxQueSize[i];
			}
			ptr_data = (uint32)(ptr +  cnt_vc + cnt_pri * sizeof(atmTxDescr_t));
			/*	store the starting address of these data descriptors	*/
			atmTxDescrPoolAddr[vc].pTxDescr[priority] = (atmTxDescr_t *)CACHE_TO_NONCACHE(ptr_data);
		}
		ptr_oam = (uint32)(ptr + cnt_vc	+ cnt_pri_total * sizeof(atmTxDescr_t));
		/*	store the starting address of these oam descriptors	*/
		atmTxDescrPoolAddr[vc].pTxOamDescr = (atmTxCcDescr_t *)CACHE_TO_NONCACHE(ptr_oam);
	}
}
#endif

void atmQosParmInit(void){
	 if(SAR_CLK < 10){ /*It's mean FAGA Clock rate, HCLK=25Mhz, SAR_CLK=6.125Mhz*/
	 	treg_tstbr=9;
	 }
	 else{
		 //Do not use isTC3162L2P2 definition. It will be true in 3182 or 3262 chip. shnwind 20100221. 
	     if(isTC3162L3P3 || isTC3162L4P4 || isTC3162L5P5){/*HCLK=133Mhz, SAR_CLK=33.25Mhz*/
		   treg_tstbr=8139;
		 }
		 else if(isTC3162U){ /*SAR_CLK=41.5Mhz*/
		   treg_tstbr=5864;
		 }
		 else if(isTC3182 || isRT65168){
		   treg_tstbr=3600;
		 }
		else if(isRT63260){
			treg_tstbr=5652;		 
		}
		else if(isRT63365 || isRT63368){
			treg_tstbr=(SAR_CLK*1000000/7076)-1;
		}
	 }
	 //printk("tstbr %d\n",treg_tstbr);
	 /*The default value is 3600*/
}
/*______________________________________________________________________________
**  atmInit
**
**  descriptions: the intialization function of ATM SAR module
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
void
atmInit(
	void
)
{
	uint8 i;
	uint32 priority;  /*Rodney_20090724*/
	#ifdef DMA_API
	uint32 mem_size=0;
	#endif

#ifdef DYNAMIC_ALLOC_DMA_BUF
	if( sarSoftReset == 0) {
		#ifndef DMA_API
	 	atmTxCcDescrPool = kmalloc(sizeof(atmTxCcDescrPool_t), GFP_KERNEL);
		if (atmTxCcDescrPool == NULL) {
			printk("unable to kmalloc atmTxCcDescrPool structure.\n");
			return;
		}
	 	atmRxCcDescrPool = kmalloc(sizeof(atmRxCcDescrPool_t), GFP_KERNEL);
		if (atmRxCcDescrPool == NULL) {
			printk("unable to kmalloc atmRxCcDescrPool structure.\n");
			return;
		}
	 	ccBuf = kmalloc(sizeof(atmRxCcCell_t)*ATM_RX_CC_DESCR_NUMMAX, GFP_KERNEL);
		if (ccBuf == NULL) {
			printk("unable to kmalloc ccBuf structure.\n");
			return;
		}
	 	oamBuf = kmalloc(sizeof(atmRxOamDescrPool_t)*ATM_VC_MAX, GFP_KERNEL);
		if (oamBuf == NULL) {
			printk("unable to kmalloc oamBuf structure.\n");
			return;
		}
	 	atmRxDescrPool = kmalloc(sizeof(atmRxDescrPool_t)*ATM_VC_MAX, GFP_KERNEL);
		if (atmRxDescrPool == NULL) {
			printk("unable to kmalloc atmRxDescrPool structure.\n");
			return;
		}
	 	atmRxIrq = kmalloc(sizeof(uint32)*IRQ_MAX_ENTRY, GFP_KERNEL);
		if (atmRxIrq == NULL) {
			printk("unable to kmalloc atmRxIrq structure.\n");
			return;
		}

		memset( (uint8*)atmTxCcDescrPool, 0 ,sizeof(atmTxCcDescrPool_t));
		memset( (uint8*)atmRxCcDescrPool, 0, sizeof(atmRxCcDescrPool_t));
		memset( (uint8*)ccBuf, 0, sizeof(atmRxCcCell_t)*ATM_RX_CC_DESCR_NUMMAX);
		memset( (uint8*)oamBuf, 0, sizeof(atmRxOamDescrPool_t)*ATM_VC_MAX);
		memset( (uint8*)atmRxDescrPool, 0 ,sizeof(atmRxDescrPool_t)*ATM_VC_MAX);
		memset( (uint8*)atmRxIrq, 0 ,sizeof(uint32)*IRQ_MAX_ENTRY);
		#else /*DMA_API*/
		mem_size=sizeof(atmTxCcDescrPool_t);
		atmTxCcDescrPool=(atmTxCcDescrPool_t *)dma_alloc_coherent(NULL, mem_size, (dma_addr_t *) &atmTxCcDescrPool_phys_p, GFP_KERNEL);
		if (atmTxCcDescrPool == NULL) {
			printk("unable to dma_alloc_coherent atmTxCcDescrPool structure.\n");
			return;
		}


		mem_size=sizeof(atmRxCcDescrPool_t);
		atmRxCcDescrPool = (atmRxCcDescrPool_t *)dma_alloc_coherent(NULL, mem_size, (dma_addr_t *) &atmRxCcDescrPool_phys_p, GFP_KERNEL);
		if (atmRxCcDescrPool == NULL) {
			printk("unable to dma_alloc_coherent atmRxCcDescrPool structure.\n");
			return;
		}

		mem_size=sizeof(atmRxCcCell_t)*ATM_RX_CC_DESCR_NUMMAX;
		ccBuf = (atmRxCcCell_t *)dma_alloc_coherent(NULL, mem_size, (dma_addr_t *) &ccBuf_phys_p, GFP_KERNEL);
		if (ccBuf == NULL) {
			printk("unable to dma_alloc_coherent ccBuf structure.\n");
			return;
		}

		mem_size=sizeof(atmRxOamDescrPool_t)*ATM_VC_MAX;
		oamBuf = (atmRxOamDescrPool_t *)dma_alloc_coherent(NULL, mem_size, (dma_addr_t *) &oamBuf_phys_p, GFP_KERNEL);
		if (oamBuf == NULL) {
			printk("unable to dma_alloc_coherent oamBuf structure.\n");
			return;
		}

		mem_size=sizeof(atmRxDescrPool_t)*ATM_VC_MAX;
		atmRxDescrPool = (atmRxDescrPool_t *) dma_alloc_coherent(NULL, mem_size, (dma_addr_t *) &atmRxDescrPool_phys_p, GFP_KERNEL);
		if (atmRxDescrPool == NULL) {
			printk("unable to dma_alloc_coherent atmRxDescrPool.\n");
			return;
		}
		mem_size=sizeof(uint32)*IRQ_MAX_ENTRY;
		atmRxIrq = (uint32 *) dma_alloc_coherent(NULL, mem_size, (dma_addr_t *) &atmRxIrq_phys_p, GFP_KERNEL);
		if (atmRxIrq == NULL) {
			printk("unable to dma_alloc_coherent atmRxIrq structure.\n");
			return;
		}
		#endif

		atmTxDescrAlloc();
	}
#endif

	//disable Tx/Rx first to prevent error
    TSARM_GFR = 0x0;
	//reset SAR Module
	delay1ms(1); //delay1ms(50);

	TSARM_RAI = RAI_RESET_ENB(1);
	delay1ms(1);//delay1ms(5);
	TSARM_RAI = RAI_RESET_ENB(0);

	/* free mbuf descriptor allocation */
	if( sarSoftReset != 0)
		sarDrvDescripReset(0);

	atmQosParmInit();
	TSARM_TSTBR = treg_tstbr;
	TSARM_TXSLRC = treg_tslr;

	/* Clear ATM SAR registers */
	//there are 10 VCs inside 3162L2
	for (i = 0; i < 10; i++)
	{
		// VC configuration
		TSARM_VCCR(i) = 0;
		// Transmit Buffer Descriptor
		TSARM_TXDCBDA(i) = 0;
		TSARM_VC_TX_BD_PRIORITY01(i) = 0;
		TSARM_VC_TX_BD_PRIORITY23(i) = 0;
		// OAM Transmit Buffer Descriptor
		TSARM_TXMCBDA(i)=0;
		// Receive Buffer Descriptor
		TSARM_RXDCBDA(i)=0;
		TSARM_VC_RX_DATA(i) = 0;
		// OAM Receive Buffer Descriptor
		TSARM_RXMCBDA(i) = 0;
		// Traffic Scheduler - PCR
		TSARM_PCR(i) = 0;
		// Traffic Scheduler - SCR
		TSARM_SCR(i) = 0;
		// Traffic Scheduler - MBSTP
		TSARM_MBSTP(i) = 0;
		TSARM_IRQM(i) = 0;
		TSARM_VC_MPOA_CTRL(i) = 0;
	}
	//mpoa registers
	TSARM_MPOA_GCR = 0x0;
	//MPOA header insertion field values
	TSARM_MPOA_HFIV11 = 0xaaaa03;
	TSARM_MPOA_HFIV12 = 0xfefe03;
	TSARM_MPOA_HFIV13 = 0x0;
	TSARM_MPOA_HFIV21 = 0x0080c2;
	TSARM_MPOA_HFIV22 = 0x000000;
	TSARM_MPOA_HFIV23 = 0x0;
	TSARM_MPOA_HFIV31 = 0x0001;
	TSARM_MPOA_HFIV32 = 0x0007;
	TSARM_MPOA_HFIV33 = 0xfefe;
	TSARM_MPOA_HFIV41 = 0x0000;
	TSARM_MPOA_HFIV42 = 0x0800;
	TSARM_MPOA_HFIV43 = 0x03cf;

	TSARM_CCCR = 0x0;
	TSARM_IRQMCC = 0x0;

	// CC channel current Tx/Rx descriptor
	TSARM_CC_TX_BD_BASE = 0;
	TSARM_CC_TX_BD_MNG_BASE = 0;
	TSARM_CC_RX_BD_BASE = 0;
	TSARM_CC_RX_BD_MNG_BASE = 0;

	/* every 4 packets to issue interrupt and timeout is about 31us */
	TSARM_IRQ_QUE_THRE = 0x4;
	TSARM_IRQ_TIMEOUT_CTRL = 0x400;

	/* Set up tx/rx descriptors */
	atmTxDescrInit();
	atmRxDescrInit();

	/* Reset ATM rx IRQ queue */
	atmRxIrqQueueSet((uint32) atmRxIrq, IRQ_MAX_ENTRY);

	TSARM_VCCR(10) = VCCFGR_VPI(255) | VCCFGR_VCI(15) |
		VCCFGR_PORT(VCCFGR_ATM_PHY0) | VCCFGR_VALID;
	atmOamOpen();
	if( sarSoftReset == 0) {
		atm_p = &atmCtrl;
		/* clear atm counters */
		memset((char *)&(atm_p->MIB_II), 0, sizeof(atmMIB_II_t));
		memset( (uint8*)&atmCfgTable, 0, sizeof(atmConfig_t));
	}
	/* Hook Isr */
	isAdslUp = 1;
	TSARM_GFR = GFR_TXENB | GFR_RXENB |	GFR_GIRQEN | GFR_BIGENDIAN | GFR_RX_INACT_VC_M;
//	#ifdef QOS_REMARKING  /*Rodney_20090724*/
	#if defined (QOS_REMARKING) || defined (TCSUPPORT_HW_QOS)
	if(qos_wrr_user == QOS_DMAWRR_USERDEFINE){
		if(*qos_wrr_info == 0)  /*strict priority*/
			TSARM_GFR &= ~(GFR_DMT_WRR_EN);
		else{  /*WRR*/
			priority = TSARM_DMAWRRCR;
			priority = (priority & 0xf0f0f0f0);
			priority |= (((uint32)(qos_wrr_info[1] & 0x0f) << 0) | ((uint32)(qos_wrr_info[2] & 0x0f) << 8) | ((uint32)(qos_wrr_info[3] & 0x0f) << 16) | ((uint32)(qos_wrr_info[4] & 0x0f) << 24));
			TSARM_DMAWRRCR = priority;
			TSARM_GFR |= GFR_DMT_WRR_EN;
		}
	}
	#endif
	
	if(isRT63365 || isRT63368){
		irqrereadflag = 1;
	}	
	
	#ifdef SAR_VERIFY
	/*Encable inactive VC IRQ Event*/
	TSARM_GFR |= GFR_ACTIVE_MIS;
	#endif
}

/*______________________________________________________________________________
**  atmTxDescrInit
**
**  descriptions: Initialize atm Tx descriptors
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
void
atmTxDescrInit(
	void
)
{
	uint8 vc;
	uint8 descrId;
	atmTxDescr_t *atmTxDescrp;
	atmTxCcDescr_t *atmTxOamDescrp;
	atmTxCcDescrPool_t *atmTxCcDescrVcPoolp;
	atmTxCcDescr_t *atmTxCcDescrp;
	uint32 P0_offset, P1_offset, P2_offset, P3_offset, Oam_offset, Cc_offset;
	uint8 priority;

	atmTxVcTotalCnt = 0;
	// init data and management buffer descriptor
	for(vc=0; vc < ATM_VC_MAX; vc++){
		//calcute each priority offset from base address
		P0_offset = 0x00000000;
		P1_offset = (uint32)atmTxDescrPoolAddr[vc].pTxDescr[1] - (uint32)atmTxDescrPoolAddr[vc].pTxDescr[0];
		P2_offset = (uint32)atmTxDescrPoolAddr[vc].pTxDescr[2] - (uint32)atmTxDescrPoolAddr[vc].pTxDescr[0];
		P3_offset = (uint32)atmTxDescrPoolAddr[vc].pTxDescr[3] - (uint32)atmTxDescrPoolAddr[vc].pTxDescr[0];
		Oam_offset = (uint32)atmTxDescrPoolAddr[vc].pTxOamDescr - (uint32)atmTxDescrPoolAddr[vc].pTxDescr[0];
		//fill in Register
		TSARM_TXDCBDA(vc) = K1_TO_PHYSICAL(atmTxDescrPoolAddr[vc].pTxDescr[0]);
		TSARM_VC_TX_BD_PRIORITY01(vc) = K1_TO_PHYSICAL(((P1_offset << 16) + P0_offset));
		TSARM_VC_TX_BD_PRIORITY23(vc) = K1_TO_PHYSICAL(((P3_offset << 16) + P2_offset));
		TSARM_TXMCBDA(vc) = K1_TO_PHYSICAL(Oam_offset);

		//initialize each priority
		for(priority = 0; priority < ATM_TX_PRIORITY_MAX; priority ++){
			// Free and Busy Descriptor
			atmTxVcFreeDescrp[priority][vc] = atmTxDescrPoolAddr[vc].pTxDescr[priority];
			atmTxVcBusyDescrp[priority][vc] = atmTxDescrPoolAddr[vc].pTxDescr[priority];
			atmTxPriVcCnt[priority][vc] = 0;

			//init each descriptor
			for ( descrId = 0; descrId < atmTxQueSize[priority]; descrId++ )
			{
				atmTxDescrp = atmTxDescrPoolAddr[vc].pTxDescr[priority]+ descrId;
				if ( descrId == atmTxQueSize[priority] - 1 ) {
					//BD_GAP, others = 0; EOR = 1
					atmTxDescrp->tdes0 = TSARM_TX_DESCR_BD_GAP(TSARM_TX_DATA_BD_GAP)
						| TSARM_TX_DESCR_EOR;
				}else{
					//BD_GAP, others = 0
					atmTxDescrp->tdes0 = TSARM_TX_DESCR_BD_GAP(TSARM_TX_DATA_BD_GAP);
				}
				// TDES1 =0 ; Buffer length =0;
				atmTxDescrp->tdes1 = 0x0;
				// TDES2 = 0x0; buffer address = null
				atmTxDescrp->tdes2 = 0x0;
				atmTxDescrp->tdes3 = 0x0;
				atmTxDescrp->skb = NULL;
			}

		}

		atmTxOamFreeDescrp[vc] = atmTxDescrPoolAddr[vc].pTxOamDescr;
		atmTxVcCnt[vc] = 0;
		// management
		for ( descrId = 0; descrId < ATM_TX_CC_DESCR_NUMMAX; descrId++ )
		{
			atmTxOamDescrp = atmTxDescrPoolAddr[vc].pTxOamDescr + descrId;
			if ( descrId == ATM_TX_CC_DESCR_NUMMAX - 1 ) {
				//BD_GAP, EOR = 1, others=0;
				atmTxOamDescrp->tdes0 =TSARM_TX_DESCR_BD_GAP(TSARM_TX_CC_BD_GAP)
						| TSARM_TX_DESCR_EOR;
			}else{
				//BD_GAP, others = 0
				atmTxOamDescrp->tdes0 =TSARM_TX_DESCR_BD_GAP(TSARM_TX_CC_BD_GAP);
			}
			// TDES1 =0 ; Buffer length =0;
			atmTxOamDescrp->tdes1 = 0x0;
			// TDES2 = 0x0; buffer address = null
			atmTxOamDescrp->tdes2 = 0x0;
			atmTxOamDescrp->tdes3 = 0x0;
		}
	}

	// init Cc
#ifdef DYNAMIC_ALLOC_DMA_BUF
	atmTxCcDescrVcPoolp = (atmTxCcDescrPool_t *)CACHE_TO_NONCACHE(atmTxCcDescrPool);
#else
	atmTxCcDescrVcPoolp = (atmTxCcDescrPool_t *)CACHE_TO_NONCACHE(&atmTxCcDescrPool);
#endif
	TSARM_CC_TX_BD_BASE = K1_TO_PHYSICAL(atmTxCcDescrVcPoolp);
	// calculate the offset value
	Cc_offset = ((uint32)atmTxCcDescrVcPoolp->txCcDescrPool - (uint32)atmTxCcDescrVcPoolp);
	TSARM_CC_TX_BD_MNG_BASE = K1_TO_PHYSICAL(Cc_offset);
	atmTxCcFreeDescrp = atmTxCcDescrVcPoolp->txCcDescrPool;
	for ( descrId = 0; descrId < ATM_TX_CC_DESCR_NUMMAX; descrId++ )
	{
		atmTxCcDescrp = atmTxCcDescrVcPoolp->txCcDescrPool+ descrId;
		if ( descrId == ATM_TX_CC_DESCR_NUMMAX - 1 ) {
			//BD_GAP, EOR = 1, others=0;
			atmTxCcDescrp->tdes0 =TSARM_TX_DESCR_BD_GAP(TSARM_TX_CC_BD_GAP)
						| TSARM_TX_DESCR_EOR;
		}else{
			//BD_GAP, others=0;
			atmTxCcDescrp->tdes0 =TSARM_TX_DESCR_BD_GAP(TSARM_TX_CC_BD_GAP);
		}
		// TDES1 =0 ; Buffer length =0;
		atmTxCcDescrp->tdes1 = 0x0;
		// TDES2 = 0x0; buffer address = null
		atmTxCcDescrp->tdes2 = 0x0;
		atmTxCcDescrp->tdes3 = 0x0;
	}
}

/*______________________________________________________________________________
**  atmRxDescrInit
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
void
atmRxDescrInit(
	void
)
{
	uint8 vc;
	uint8 descrId;
	struct sk_buff *skb;
	atmRxDescrPool_t *atmRxDescrVcPoolp;
	atmRxDescr_t *atmRxDataDescrp;
	atmRxCcDescr_t *atmRxMngDescrp;
	atmRxCcDescrPool_t *atmRxCcDescrPoolp;
	atmRxCcDescr_t *atmRxCcDescrp;
	uint32 Data_offset, Oam_offset, Cc_offset;

	// init vc data and management descriptor
	for(vc = 0; vc < ATM_VC_MAX; vc++){
		atmRxDescrVcPoolp = (atmRxDescrPool_t *)CACHE_TO_NONCACHE(atmRxDescrPool + vc);
		//calculate the offset value
		Data_offset = (uint32)atmRxDescrVcPoolp->rxDescrPool - (uint32)atmRxDescrVcPoolp;
		Oam_offset = (uint32)atmRxDescrVcPoolp->rxOamDescrPool - (uint32)atmRxDescrVcPoolp;
		//fill in Register
		TSARM_RXDCBDA(vc)=K1_TO_PHYSICAL(atmRxDescrVcPoolp);
		TSARM_VC_RX_DATA(vc) = K1_TO_PHYSICAL(Data_offset);
		TSARM_RXMCBDA(vc)=K1_TO_PHYSICAL(Oam_offset);

		atmRxVcDescrp[vc] = atmRxDescrVcPoolp->rxDescrPool;
		atmRxOamBusyDescrp[vc] = atmRxDescrVcPoolp->rxOamDescrPool;

		// data
		for ( descrId = 0; descrId < ATM_RX_VC_DESCR_NUMMAX; descrId++ )
		{
			atmRxDataDescrp = atmRxDescrVcPoolp->rxDescrPool + descrId;
			if (sarSoftReset != 0)
				skb = atmRxDataDescrp->skb;
			else{
#if defined(TCSUPPORT_CT)
			#ifdef DYNAMIC_ALLOC_SKB
				skb = NULL;
			#else
				skb = skbmgr_dev_alloc_skb2k();
				dma_cache_inv((unsigned long)skb->data, RX_BUF_LEN);
			#endif
#else
				skb = skbmgr_dev_alloc_skb2k();
				dma_cache_inv((unsigned long)skb->data, RX_BUF_LEN);
#endif
			}

			if ( descrId == ATM_RX_VC_DESCR_NUMMAX - 1 ) {
				// BD_GAP, Rx Descriptor own by DMA; EOR=1
				atmRxDataDescrp->rdes0 =TSARM_TX_DESCR_BD_GAP(TSARM_RX_DATA_BD_GAP)
							|  TSARM_RX_DESCR_EOR | TSARM_RX_DESCR_VALID;
			}else{
				// BD_GAP, Rx Descriptor own by DMA
				atmRxDataDescrp->rdes0 = TSARM_TX_DESCR_BD_GAP(TSARM_RX_DATA_BD_GAP)
							| TSARM_RX_DESCR_VALID;
			}
			// RDES1 = 1776; Set Rx Buffer size = 1776, reference zynos configure
			atmRxDataDescrp->rdes1 = atmRxBufferSize<<16;
			#ifdef SAR_VERIFY
			rxBufAddrOffset(skb);
			#endif
			// RDES2 = point the Rx Buffer to mbuf
#if defined(TCSUPPORT_CT)
			#ifdef DYNAMIC_ALLOC_SKB
			if(skb == NULL)
				atmRxDataDescrp->rdes2 = 0x0;
			else
				atmRxDataDescrp->rdes2 = K1_TO_PHYSICAL(skb->data);
			#else
			atmRxDataDescrp->rdes2 = K1_TO_PHYSICAL(skb->data);
			#endif
#else
			atmRxDataDescrp->rdes2 = K1_TO_PHYSICAL(skb->data);

#endif
			atmRxDataDescrp->rdes3 = 0x0;
			atmRxDataDescrp->skb = skb;
		}

		// management
		for ( descrId = 0; descrId < ATM_RX_CC_DESCR_NUMMAX; descrId++ )
		{
			atmRxMngDescrp = atmRxDescrVcPoolp->rxOamDescrPool+ descrId;
			if ( descrId == ATM_RX_CC_DESCR_NUMMAX - 1 ) {
				// BD_GAP, Rx Descriptor own by DMA; EOR=1
				atmRxMngDescrp->rdes0 =TSARM_TX_DESCR_BD_GAP(TSARM_RX_CC_BD_GAP)
							| TSARM_RX_DESCR_EOR | TSARM_RX_DESCR_VALID;
			}else{
				// BD_GAP, Rx Descriptor own by DMA
				atmRxMngDescrp->rdes0 =TSARM_TX_DESCR_BD_GAP(TSARM_RX_CC_BD_GAP)
							| TSARM_RX_DESCR_VALID;
			}
			// RDES1 = 48; Set Rx Buffer size = 48, reference zynos configure
			atmRxMngDescrp->rdes1 = 48 << 16;
			// RDES2 = point the Rx Buffer to mbuf
			atmRxMngDescrp->rdes2 = K1_TO_PHYSICAL(CACHE_TO_NONCACHE(&(oamBuf[vc].rxOamDescrPool[descrId])));
			atmRxMngDescrp->rdes3 = 0x0;
		}
	}

	// init Cc
#ifdef DYNAMIC_ALLOC_DMA_BUF
	atmRxCcDescrPoolp = (atmRxCcDescrPool_t *)CACHE_TO_NONCACHE(atmRxCcDescrPool);
#else
	atmRxCcDescrPoolp = (atmRxCcDescrPool_t *)CACHE_TO_NONCACHE(&atmRxCcDescrPool);
#endif
	//calcute Cc offset value
	Cc_offset = (uint32)atmRxCcDescrPoolp->rxCcDescrPool - (uint32)atmRxCcDescrPoolp;
	//fill in register
	TSARM_CC_RX_BD_BASE = K1_TO_PHYSICAL(atmRxCcDescrPoolp);
	TSARM_CC_RX_BD_MNG_BASE = K1_TO_PHYSICAL(Cc_offset);
	atmRxCcBusyDescrp = atmRxCcDescrPoolp->rxCcDescrPool;
	for ( descrId = 0; descrId < ATM_RX_CC_DESCR_NUMMAX; descrId++ ) {
		atmRxCcDescrp = atmRxCcDescrPoolp->rxCcDescrPool + descrId;
		if ( descrId == ATM_RX_CC_DESCR_NUMMAX - 1 ) {
			// BD_GAP, Rx Descriptor own by DMA; EOR=1
			atmRxCcDescrp->rdes0 =TSARM_TX_DESCR_BD_GAP(TSARM_RX_CC_BD_GAP)
						| TSARM_RX_DESCR_EOR | TSARM_RX_DESCR_VALID;
		}else{
			// BD_GAP, Rx Descriptor own by DMA
			atmRxCcDescrp->rdes0 =TSARM_TX_DESCR_BD_GAP(TSARM_RX_CC_BD_GAP)
						| TSARM_RX_DESCR_VALID;
		}
		atmRxCcDescrp->rdes1 = (96 << 16);
		atmRxCcDescrp->rdes2 = K1_TO_PHYSICAL(CACHE_TO_NONCACHE((ccBuf + descrId)));
		atmRxCcDescrp->rdes3 = 0x0;
	}
}
#if defined(TCSUPPORT_CT)
#ifdef DYNAMIC_ALLOC_SKB
int atmRxSkbAlloc(uint8 vc)
{
	uint8 descrId;
	struct sk_buff *skb;
	atmRxDescrPool_t *atmRxDescrVcPoolp;
	atmRxDescr_t *atmRxDataDescrp;
	uint32 Data_offset;

	// init vc data and management descriptor
	atmRxDescrVcPoolp = (atmRxDescrPool_t *)CACHE_TO_NONCACHE(atmRxDescrPool + vc);

	// data
	for ( descrId = 0; descrId < ATM_RX_VC_DESCR_NUMMAX; descrId++ )
	{
		atmRxDataDescrp = atmRxDescrVcPoolp->rxDescrPool + descrId;
		if(atmRxDataDescrp->skb == NULL){
			//skb = skbmgr_dev_alloc_skb2k();
			skb = skbmgr_dev_alloc_skb2k_tc_critical();
			if(skb == NULL){
				printk("atmRxSkbAlloc skb==NULL");
				return -1;
			}
			dma_cache_inv((unsigned long)skb->data, RX_BUF_LEN);
		}
		else{
			skb = atmRxDataDescrp->skb;
		}
		#if 0
		if (sarSoftReset != 0)
			skb = atmRxDataDescrp->skb;
		else{
			//skb = skbmgr_dev_alloc_skb2k();
			skb = skbmgr_dev_alloc_skb2k_tc_critical();
			if(skb == NULL){
				printk("atmRxSkbAlloc skb==NULL");
				return -1;
			}
			dma_cache_inv((unsigned long)skb->data, RX_BUF_LEN);
		}
		#endif
		if ( descrId == ATM_RX_VC_DESCR_NUMMAX - 1 ) {
			// BD_GAP, Rx Descriptor own by DMA; EOR=1
			atmRxDataDescrp->rdes0 =TSARM_TX_DESCR_BD_GAP(TSARM_RX_DATA_BD_GAP)
						|  TSARM_RX_DESCR_EOR | TSARM_RX_DESCR_VALID;
		}else{
			// BD_GAP, Rx Descriptor own by DMA
			atmRxDataDescrp->rdes0 = TSARM_TX_DESCR_BD_GAP(TSARM_RX_DATA_BD_GAP)
						| TSARM_RX_DESCR_VALID;
		}
		// RDES1 = 1776; Set Rx Buffer size = 1776, reference zynos configure
		atmRxDataDescrp->rdes1 = atmRxBufferSize<<16;
		// RDES2 = point the Rx Buffer to mbuf
		atmRxDataDescrp->rdes2 = K1_TO_PHYSICAL(skb->data);
		atmRxDataDescrp->rdes3 = 0x0;
		atmRxDataDescrp->skb = skb;
	}
	return 0;
}

void atmRxSkbFree(uint8 vc)
{
	uint8 descrId;
	atmRxDescrPool_t *atmRxDescrVcPoolp;
	atmRxDescr_t *atmRxDataDescrp;
	struct sk_buff *skb;

	atmRxDescrVcPoolp = (atmRxDescrPool_t *)CACHE_TO_NONCACHE(atmRxDescrPool + vc);
	for ( descrId = 0; descrId < ATM_RX_VC_DESCR_NUMMAX; descrId++ ) {
		atmRxDataDescrp = atmRxDescrVcPoolp->rxDescrPool+ descrId;
		skb = atmRxDataDescrp->skb;
		if( skb != NULL ){
			dev_kfree_skb_any(skb);
			atmRxDataDescrp->skb = NULL;
		}
		atmRxDataDescrp->rdes2 = 0x0;
	}
}
#endif
#endif

/*______________________________________________________________________________
**  atmAal5RealDataReq
**
**  descriptions: API for sending an AAL5 frame
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
uint8
atmAal5RealDataReq(
	struct sk_buff *skb,
	uint8 vc
)
{
	uint32 atmTxVcDescrNummax=0;
	uint8 priority = 3;
	uint8 new_priority;
	int i;

	atmTxVcDescrNummax = atmTxQueSize[0] + atmTxQueSize[1] + atmTxQueSize[2] +
		atmTxQueSize[3];

	if ( atmTxVcCnt[vc] >= ATM_TX_BUF_RELEASE_THRESHOLD ) {
		atmTxDescrFree(vc, 0);
	}
#ifdef TCSUPPORT_QOS
	switch (qos_flag) {
#if !defined(TCSUPPORT_CT)
		case QOS_SW_PQ:
			/* PQ mode */
			if (priority > 1 && (skb->mark & QOS_FILTER_MARK) == QOS_HH_PRIORITY) {
				priority = 1;
			}
			else if (priority > 2 && (skb->mark & QOS_FILTER_MARK) == QOS_H_PRIORITY) {
				priority = 2;
			}
			break;
#endif
	#if defined(TCSUPPORT_HW_QOS)
		case QOS_HW_WRR:
			/* HW WRR mode */
			if ((skb->mark & QOS_FILTER_MARK) == QOS_HH_PRIORITY) {
				//printk("lalala to first queue.\n");
				priority = 0;
			}
			else if ((skb->mark & QOS_FILTER_MARK) == QOS_H_PRIORITY) {
				priority = 1;
			}
			else if ((skb->mark & QOS_FILTER_MARK) == QOS_M_PRIORITY) {
				priority = 2;
			}
			else {
				priority = 3;
			}
			break;
		#endif
#if !defined(TCSUPPORT_CT)
		case QOS_HW_PQ:
			/* HW PQ mode */
			if (priority > 0 && (skb->mark & QOS_FILTER_MARK) == QOS_HH_PRIORITY) {
				priority = 0;
			}
			else if (priority > 1 && (skb->mark & QOS_FILTER_MARK) == QOS_H_PRIORITY) {
				priority = 1;
			}
			else if (priority > 2 && (skb->mark & QOS_FILTER_MARK) == QOS_M_PRIORITY) {
				priority = 2;
			}
			break;
		case NULLQOS:/*It's for putting rtp packets to HH priority when qos_flag not be selected as WRR or PQ*/
			if ((skb->mark & QOS_FILTER_MARK) == QOS_HH_PRIORITY) {
				priority = 0;
			}
			break;
#endif
			
		default:
			break;
	}
#endif

	#ifdef QOS_REMARKING  /*Rodney_20090724*/
	if((skb->mark & QOS_REMARKING_FLAG)){
		priority = (uint8)((skb->mark & QOS_REMARKING_MASK) >> 1);
	}
	else{
		if (priPktChk && (skb->len < priPktChkLen)) {
			/*20100921_serena_modify: add inupu arg "len" to check tcp packet*/	
			if (isPriorityPkt(skb->data, &new_priority, skb->len)) {
#if !defined(TCSUPPORT_CT)
			#ifdef TCSUPPORT_QOS
				if (qos_flag == QOS_HW_WRR) {
					/* hw wrr mode, to handle special packet */
					priority = max_prio;
				}
				else {
					if (new_priority < priority) {
						priority = new_priority;
					}
				}
			#else
				priority = new_priority;
			#endif
#endif
			}
		}
	}
	#else
	if (priPktChk && (skb->len < priPktChkLen)) {
		/*20100921_serena_modify: add inupu arg "len" to check tcp packet*/	
		if (isPriorityPkt(skb->data, &new_priority, skb->len)) {
#if !defined(TCSUPPORT_CT)
		#ifdef TCSUPPORT_QOS
			if (qos_flag == QOS_HW_WRR) {
				/* hw wrr mode, to handle special packet */
				priority = max_prio;
			}
			else {
				if (new_priority < priority) {
					priority = new_priority;
				}
			}
		#else
			priority = new_priority;
		#endif
#endif
		}
	}
	#endif

	#ifdef PURE_BRIDGE
		priority = 0;
	#endif
	#ifdef SAR_VERIFY
	priority=isWrrPriorityPkt(skb, priority);
	#endif
#if defined(TCSUPPORT_CT)	
#ifdef TCSUPPORT_QOS
	if (qos_flag == 0) {
		/* PQ mode */
		if (priority == 3 && (skb->mark & QOS_FILTER_MARK) == QOS_HH_PRIORITY) {
			priority = 2;
		}
	}
#endif
#endif

	if ( atmTxPriVcCnt[priority][vc] >= atmTxQueSize[priority]) {
		atmTxDescrFree(vc, 0);
	}

	if ( atmTxVcCnt[vc] >= atmTxVcDescrNummax ) {
		atm_p->MIB_II.outDiscards++;
		if (ATM_SKB(skb)->vcc->pop) {
			ATM_SKB(skb)->vcc->pop (ATM_SKB(skb)->vcc, skb);
		} else {
			dev_kfree_skb_any(skb);
		}
		for (i = 0; i < ATM_VC_MAX; i++)
			atmTxDescrFree(i, 0);
		return 1;
	}

	atmDataReq(skb, vc, priority);

	return 0;
}

void atmTxPolling(uint8 priority, uint8 vc){
	switch(priority){
	case 0:
		TSARM_TXDBCSR_P01 |= 1 << vc;
		break;
	case 1:
		TSARM_TXDBCSR_P01 |= 1 << (vc+16);
		break;
	case 2:
		TSARM_TXDBCSR_P23 |= 1 << vc;
		break;
	case 3:
		TSARM_TXDBCSR_P23 |= 1 << (vc+16);
		break;
	default:
		break;
	}
	return;
}

uint8 atmDataReq(struct sk_buff *skb, uint8 vc, uint8 priority){
	volatile atmTxDescr_t *atmTxDescrp;
	#ifdef QOS_DROP_DUMP  /*Rodney_20091115*/
	static int drop_cnt = 0;
	#endif
	#ifdef SAR_VERIFY
	dumpDataCell(skb, priority);
	#endif

	atmTxDescrp = atmTxVcFreeDescrp[priority][vc];
	if ( !(atmTxDescrp->tdes0 & TSARM_TX_DESCR_VALID) && (atmTxDescrp->skb == NULL)) {
#if defined(TCSUPPORT_CT_SWQOS)
		if(atmTxVcCnt[vc] >=(atmTxQosQueueSize - 2)){
				if(sw_PKTQOS_SET_STOP != NULL)
					sw_PKTQOS_SET_STOP();
		}				
#endif

		dma_cache_wback_inv((unsigned long)(skb->data), skb->len);
		#ifdef SAR_VERIFY
		setSarVerifyDataDescrip(atmTxDescrp);
		#endif
		atm_p->MIB_II.outPkts++;
		atm_p->MIB_II.outBytes += skb->len;//yzwang_20091125
		atmTxDescrp->tdes1 = skb->len << 16;
		atmTxDescrp->tdes2 = K1_TO_PHYSICAL(skb->data);
		atmTxDescrp->skb = skb;
		atmTxDescrp->tdes0 |= TSARM_TX_DESCR_VALID;
		if(atmTxDescrp->tdes0 & TSARM_TX_DESCR_EOR){
			atmTxVcFreeDescrp[priority][vc] = atmTxDescrPoolAddr[vc].pTxDescr[priority];
		}else
			atmTxVcFreeDescrp[priority][vc]++;

		atmTxPriVcCnt[priority][vc]++;
		atmTxVcCnt[vc]++;
		atmTxVcTotalCnt++;

		atmTxPolling(priority, vc);

		ledTurnOn(LED_DSL_ACT_STATUS);
#ifdef TR068_LED
		/*for interner traffic led*/
		if(internet_led_on) {//IP connected and IP traffic is passing
			ledTurnOn(LED_INTERNET_STATUS);
			ledTurnOn(LED_INTERNET_ACT_STATUS);
		} 
		else {
			if(!internet_trying_led_on) {
				ledTurnOff(LED_INTERNET_STATUS);
				ledTurnOff(LED_INTERNET_TRYING_STATUS);
			}
		}	
#endif		
		#ifdef QOS_DROP_DUMP  /*Rodney_20091115*/
		if(drop_cnt >= MAX_LOG_PKT)
			drop_cnt = 0;
		qos_drop_flag[drop_cnt++] = 0;
		#endif
	} else {
		#ifdef QOS_DROP_DUMP  /*Rodney_20091115*/
		if(drop_cnt >= MAX_LOG_PKT)
			drop_cnt = 0;
		qos_drop_flag[drop_cnt++] = 1;
		#endif
		atm_p->MIB_II.outDiscards++;
		#ifdef SAR_VERIFY
		if(ATM_SKB(skb)->vcc ==NULL){
			dev_kfree_skb_any(skb);
			return 0;
		}
		#endif
		if (ATM_SKB(skb)->vcc->pop) {
			ATM_SKB(skb)->vcc->pop (ATM_SKB(skb)->vcc, skb);
		} else {
			dev_kfree_skb_any(skb);
		}
	}
	return 0;
}
#define CONFIG_VLAN_ATM
#ifdef CONFIG_VLAN_ATM
#define QOS_8021p_MARK			0x0F00 	/* 8~11 bits used for 802.1p */
#define QOS_8021P_0_MARK		0x08	/* default mark is zero */
#define QOS_8021P_TRANSPARENT	0x09
#define RFC1483_B_LLC 			(0)
#define RFC1483_R_LLC 			1
#define RFC1483_B_VC  			2
#define PPPoA_LLC				3
#define PPPoA_VC      			4
#define RFC1483_R_VC			5
#define VLAN_HLEN				4
#define VLAN_ETH_ALEN			6

#define SET_ATM_VLAN			0x1111
#define DEL_ATM_PVC			    0x2222

typedef struct tsarm_ioctl{
	unsigned short int active;
	unsigned int vid;
	unsigned char vpi;
	unsigned char vci;
}tsarm_ioctl_t;

tsarm_ioctl_t vlan_vcc[ATM_VC_MAX] = {0};

uint8 getIndexVlanVcc( uint8 vpi, uint8 vci ) 
{
	uint8 vc;
	for ( vc = 0; vc < ATM_VC_MAX; vc++ ) {
		if ( vlan_vcc[vc].vpi == vpi && vlan_vcc[vc].vci == vci ) {
			return vc;
		}
	} 
	return ATM_DUMMY_VC;
}

int getEncapMode(uint8 *data)
{
	/*RFC1483 LLC Brided Mode*/
	if ( memcmp(data, RFC1483_B_LLC_HDR, 7 ) == 0 ) {
		return RFC1483_B_LLC;
	}
	/*RFC1483 LLC Routing Mode*/
	else if ( memcmp(data, RFC1483_R_LLC_HDR, sizeof(RFC1483_R_LLC_HDR)) == 0 ){
		return RFC1483_R_LLC;
	}
	/*PPPoA LLC Routing Mode*/
	else if ( memcmp(data, PPPOA_LLC_HDR, sizeof(PPPOA_LLC_HDR)) == 0 ){
		return PPPoA_LLC;
	}
	/*PPPoA VCMux Routing Mode*/
	else if ( memcmp(data, PPPOA_VC_HDR, sizeof(PPPOA_VC_HDR)) == 0 ){
		return PPPoA_VC;
	}
	/*RFC1483 VCMux Bridged Mode*/
	else if ( memcmp(data, RFC1483_B_VC_HDR, sizeof(RFC1483_B_VC_HDR)) == 0 ) {
		return RFC1483_B_VC;
	}
	else {
		return RFC1483_R_VC;
	}
}

int atmEncapLen[] = { 
	sizeof(RFC1483_B_LLC_HDR), 
	sizeof(RFC1483_R_LLC_HDR),
	sizeof(PPPOA_LLC_HDR),
	sizeof(PPPOA_VC_HDR),
	sizeof(RFC1483_B_VC_HDR)
};

#if 0
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
#if defined(TCSUPPORT_CT)
static inline struct sk_buff * insert_vtag(struct sk_buff *skb, uint8 vc)	
#else
static struct sk_buff * insert_vtag(struct sk_buff *skb, uint8 vc)
#endif
{
	uint8 encap_mode = 0, encap_len = 0;
	char * vlan_p = NULL, *ether_type_ptr = NULL;
	unsigned char ucprio = 0;
	unsigned char uc802prio = 0;
	
	int copy_len = 0;
	
	if ( skb->mark & QOS_8021p_MARK ) {
		/*vlan tagging*/
		encap_mode = getEncapMode((uint8*)skb->data);
		/*Note Ethernet Header*/
		if ( (encap_mode == RFC1483_B_VC)
			|| (encap_mode == PPPoA_LLC)
			|| (encap_mode == PPPoA_VC) ) {
				/*Nono ethernet header to do nothings*/
				return skb;	
			}
		
		encap_len = atmEncapLen[encap_mode];
		ether_type_ptr = skb->data + encap_len + 12;
		ucprio = (skb->mark & QOS_8021p_MARK) >> 8;
		
		if ( (ucprio < QOS_8021P_0_MARK) && (ucprio >= 0) ) { //0~7 remark
			uc802prio = ucprio;
		}
		else if ( QOS_8021P_0_MARK == ucprio ) {	//zero mark
			uc802prio = 0;
		}
		else if ( QOS_8021P_TRANSPARENT == ucprio ) {//pass through
			/*do nothing*/
			return skb;
		}
		
#if defined(TCSUPPORT_C1_CUC)
		/* 
		** if the packet is vlan packet, just change the 802.1p value
		** or add vlan tag header
		*/
		vlan_p = skb->data + encap_len + 12;
		if (*(unsigned short *)vlan_p == 0x8100) {
			vlan_p += 2;
			*(unsigned short*)vlan_p &= 0x8fff;
			*(unsigned short*)vlan_p |= ((uc802prio & 0x7) << 13);
			return skb;
		}
#endif
		
		if ( skb_headroom(skb) < VLAN_HLEN ) {
			struct sk_buff *sk_tmp = skb;
			skb = skb_realloc_headroom(sk_tmp, VLAN_HLEN);
			
			if ( ATM_SKB(sk_tmp)->vcc->pop ) {
				ATM_SKB(sk_tmp)->vcc->pop(ATM_SKB(sk_tmp)->vcc, sk_tmp);
			}
			else {
				dev_kfree_skb_any(sk_tmp);
			}
			
			if ( !skb ) {
				printk(KERN_ERR, "Vlan:failed to realloc headroom\n");
				return NULL;
			}
		}
		else {
			skb = skb_unshare(skb, GFP_ATOMIC);
			if ( !skb ) {
		//		printk(KERN_ERR, "Vlan: failed to unshare skbuff\n");
				return NULL;
			}
		}
		
		/*offset 4 bytes*/
#if 0
		skb_push(skb, VLAN_HLEN*2);
		copy_len = encap_len + 2*VLAN_ETH_ALEN;
#else
		skb_push(skb, VLAN_HLEN);
		copy_len = encap_len + 2*VLAN_ETH_ALEN;
#endif
		
		/*move the mac address to the beginning of new header*/
		memmove(skb->data, skb->data+VLAN_HLEN, copy_len);
#if 0	
		printk("mac is %02x:%02x:%02x:%02x:%02x:%02x\n",*(skb->data+encap_len),\
				*(skb->data+encap_len+1),*(skb->data+encap_len+2),*(skb->data+encap_len+3),\
				*(skb->data+encap_len+4),*(skb->data+encap_len+5));
#endif

		vlan_p = skb->data + encap_len + 12;
		*(unsigned short *)vlan_p = 0x8100;
		
		vlan_p += 2;
		
		*(unsigned short *)vlan_p = 0;

#if 0
		vlan_p += 2;
		*(unsigned short *)vlan_p = 0x8100;
		vlan_p += 2;

		*(unsigned short *)vlan_p = 0;
#endif

		/*3 bits priority*/
		*(unsigned short*)vlan_p |= ((uc802prio & 0x7) << 13);
		
		/*12 bits vlan id*/
		*(unsigned short *)vlan_p |= (vlan_vcc[vc].vid & 0x0fff);
		//skb->mac.raw -= VLAN_HLEN;
		//skb->nh.raw -= VLAN_HLEN;
		skb->network_header -= VLAN_HLEN;
		skb->mac_header -= VLAN_HLEN;
	}
	//dump_skb(skb);
	return skb;
}
int resetVlanVcc(uint8 vpi, uint8 vci)
{
	uint8 vc = getIndexVlanVcc( vpi, vci );
	if ( (vc != ATM_DUMMY_VC) && (vc < ATM_VC_MAX) ) {
		memset( &vlan_vcc[vc], 0, sizeof(tsarm_ioctl_t) );
	}
	else {
		return -1;
	}
	return 0;
}

int setATMVlan( tsarm_ioctl_t *sar_ioctl )
{
	uint8 vc;
	/* reset this entry first */
	resetVlanVcc( sar_ioctl->vpi, sar_ioctl->vci );
	
	for( vc = 0; vc < ATM_VC_MAX; vc++ ) {
		if(vlan_vcc[vc].vpi == 0 && vlan_vcc[vc].vci == 0 ){
			vlan_vcc[vc].vpi = sar_ioctl->vpi;
			vlan_vcc[vc].vci = sar_ioctl->vci;
			vlan_vcc[vc].active = sar_ioctl->active;
			vlan_vcc[vc].vid = sar_ioctl->vid;
			return 0;
		}
	}
	return -1;
}
#endif

/*______________________________________________________________________________
**  atmAal5DataReq
**
**  descriptions: API for sending an AAL5 frame
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
uint8
atmAal5DataReq(
	struct sk_buff *skb,
	uint8 vpi,
	uint16 vci
)
{
	uint8 vc;

	vc = atmVcNumberGet(vpi, vci);
	if ( vc == ATM_DUMMY_VC ) {
		if (ATM_SKB(skb)->vcc->pop) {
			ATM_SKB(skb)->vcc->pop (ATM_SKB(skb)->vcc, skb);
		} else {
			dev_kfree_skb_any(skb);
		}
		return 1;
	}

 	return atmAal5RealDataReq(skb, vc);
}

/*______________________________________________________________________________
**  atmCcDataReq
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
uint8
atmCcDataReq(
	uint8 *data_p
)
{
	atmTxCcDescr_t *atmTxCcDescrp;
	atmTxCcDescrPool_t *atmTxCcDescrVcPoolp;


	atmTxCcDescrp = atmTxCcFreeDescrp;
	if ( !(atmTxCcDescrp->tdes0 & TSARM_TX_DESCR_VALID) ) {
		#ifdef L2_AUTOPVC
		setTxRaw(atmTxCcDescrp);
		#endif
		atmTxCcDescrp->tdes1 = 50 << 16;
		atmTxCcDescrp->tdes2 = K1_TO_PHYSICAL(data_p);
		atmTxCcDescrp->tdes0 |= TSARM_TX_DESCR_VALID;
		if(atmTxCcDescrp->tdes0 & TSARM_TX_DESCR_EOR){
#ifdef DYNAMIC_ALLOC_DMA_BUF
			atmTxCcDescrVcPoolp = (atmTxCcDescrPool_t *)CACHE_TO_NONCACHE(atmTxCcDescrPool);
#else
			atmTxCcDescrVcPoolp = (atmTxCcDescrPool_t *)CACHE_TO_NONCACHE(&atmTxCcDescrPool);
#endif
			atmTxCcFreeDescrp = atmTxCcDescrVcPoolp->txCcDescrPool;
		}else{
			atmTxCcFreeDescrp++;
		}
		atmTxCcCnt++;
		TSARM_TXMBCSR |= 1 << 16;
	}
	else {
		return 1;
	}
	return 0;
}

/*______________________________________________________________________________
**  atmOamDataReq
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
uint8
atmOamDataReq(
	uint8 *data_p,
	uint8 pti,
	uint8 vc
)
{
	atmTxCcDescr_t *atmTxCcDescrp;
	#ifdef SAR_VERIFY
	dumpMCellAddr(data_p, OAM_CELL);
	#endif

	atmTxCcDescrp = atmTxOamFreeDescrp[vc];
	if ( !(atmTxCcDescrp->tdes0 & TSARM_TX_DESCR_VALID) ) {
		atmTxCcDescrp->tdes1 = (pti << 28) | (48 << 16);
		atmTxCcDescrp->tdes2 = K1_TO_PHYSICAL(data_p);
		atmTxCcDescrp->tdes0 |= TSARM_TX_DESCR_VALID;
		if(atmTxCcDescrp->tdes0 & TSARM_TX_DESCR_EOR){
			atmTxOamFreeDescrp[vc] = atmTxDescrPoolAddr[vc].pTxOamDescr;
		}else{
			atmTxOamFreeDescrp[vc]++;
		}
		atmTxCcCnt++;
		TSARM_TXMBCSR |= 1 << vc;
	}
	else {
		return 1;
	}
	return 0;
}

/*______________________________________________________________________________
**  atmOamF4F5DataReq
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
uint32
atmOamF4F5DataReq(
	uint8 vpi,
	uint16 vci,
	uint8 f5,
	uint8 endToEnd,
	uint8 funcType
)
{
	atmOamCell_t *oamCellp;
	atmOamCellPayload_t *oamCellPayloadp;
	uint8 vc;
	uint8 pti;
	#ifdef DMA_API
	uint32 mem_size=0;
	#endif
#ifdef DYNAMIC_ALLOC_DMA_BUF
    #ifndef DMA_API
	static atmCell_t *atmCellCirBuf = NULL;
	#endif
    static int atmCellCirBufIndex = 0;
	atmCell_t *atmCellBuf;

	if (atmCellCirBuf == NULL) {
		#ifndef DMA_API
		if ((atmCellCirBuf = kmalloc(sizeof(atmCell_t)*ATM_VC_MAX, GFP_KERNEL)) == NULL) {
			printk("unable to kmalloc atmCellCirBuf structure.\n");
			return 1;
		}
		#else
		mem_size=sizeof(atmCell_t)*ATM_VC_MAX;
		atmCellCirBuf=(atmCell_t *)dma_alloc_coherent(NULL, mem_size, (dma_addr_t *) &atmCellCirBuf_phys_p, GFP_KERNEL);
		if (atmCellCirBuf == NULL) {
			printk("unable to dma_alloc_coherent atmCellCirBuf structure.\n");
			return 1;
		}
		#endif
	}
	atmCellBuf = (atmCell_t *)(atmCellCirBuf + atmCellCirBufIndex);
	memset((uint8*)atmCellBuf, 0, sizeof(atmCell_t));
	atmCellCirBufIndex++;

	if (atmCellCirBufIndex == ATM_VC_MAX)
		atmCellCirBufIndex = 0;

#endif

	if ( f5 ) { /* F5: by PTI */
		/* Move to non-cacheable region */
		switch ( funcType ) {
		case 0: /* AIS */
#ifdef DYNAMIC_ALLOC_DMA_BUF
			oamCellPayloadp = (atmOamCellPayload_t *)(oamF5AisCell);
#else
			oamCellPayloadp = (atmOamCellPayload_t *)(CACHE_TO_NONCACHE(oamF5AisCell));
#endif
			break;
		case 1: /* RDI */
#ifdef DYNAMIC_ALLOC_DMA_BUF
			oamCellPayloadp = (atmOamCellPayload_t *)(oamF5RdiCell);
#else
			oamCellPayloadp = (atmOamCellPayload_t *)(CACHE_TO_NONCACHE(oamF5RdiCell));
#endif
			break;
		case 2: /* Loopback */
#ifdef DYNAMIC_ALLOC_DMA_BUF
			oamCellPayloadp = (atmOamCellPayload_t *)(oamF5LoopBackReqCell);
#else
			oamCellPayloadp = (atmOamCellPayload_t *)(CACHE_TO_NONCACHE(oamF5LoopBackReqCell));
#endif
			break;
		case 3: /* Continuity check */
#ifdef DYNAMIC_ALLOC_DMA_BUF
			oamCellPayloadp = (atmOamCellPayload_t *)(oamF5ContinuityCheckReqCell);
#else
			oamCellPayloadp = (atmOamCellPayload_t *)(CACHE_TO_NONCACHE(oamF5ContinuityCheckReqCell));
#endif
			break;
		default:
			return 1;
		}
		vc = atmVcNumberGet(vpi, vci);
		if (  vc == ATM_DUMMY_VC ) {
			/* This is not a opened VC */
			return 1;
		}
		if ( endToEnd ) {
			pti = 5;
		}
		else {
			pti = 4;
		}
#ifdef DYNAMIC_ALLOC_DMA_BUF
		memcpy(atmCellBuf->word, (uint8 *)oamCellPayloadp, 48);
		if(!atmOamDataReq((uint8 *)atmCellBuf->word, pti, vc)){
#else
		if(!atmOamDataReq((uint8 *)oamCellPayloadp, pti, vc)){
#endif
			atm_p->MIB_II.outF5Pkts++;
			atm_p->MIB_II.outPkts++;
			//add by brian
			#ifdef CWMP
			if(cwmpflag)
			{
				cwmpSavedOutF5Pkts++;
			}
			#endif
		}
	}
	else { /* F4: by VCI */
		/* Move to non-cacheable region */
		switch ( funcType ) {
		case 0: /* AIS */
#ifdef DYNAMIC_ALLOC_DMA_BUF
			oamCellp = (atmOamCell_t *)(oamAisCell);
#else
			oamCellp = (atmOamCell_t *)(CACHE_TO_NONCACHE(oamAisCell));
#endif
			break;
		case 1: /* RDI */
#ifdef DYNAMIC_ALLOC_DMA_BUF
			oamCellp = (atmOamCell_t *)(oamRdiCell);
#else
			oamCellp = (atmOamCell_t *)(CACHE_TO_NONCACHE(oamRdiCell));
#endif
			break;
		case 2: /* Loopback */
#ifdef DYNAMIC_ALLOC_DMA_BUF
			oamCellp = (atmOamCell_t *)(oamLoopBackReqCell);
#else
			oamCellp = (atmOamCell_t *)(CACHE_TO_NONCACHE(oamLoopBackReqCell));
#endif
			break;
		case 3: /* Continuity check */
#ifdef DYNAMIC_ALLOC_DMA_BUF
			oamCellp = (atmOamCell_t *)(oamContinuityCheckReqCell);
#else
			oamCellp = (atmOamCell_t *)(CACHE_TO_NONCACHE(oamContinuityCheckReqCell));
#endif
			break;
		default:
			return 1;
		}
		oamCellp->vpi = vpi;
		if ( endToEnd ) {
			oamCellp->vci = 4;
		}
		else {
			oamCellp->vci = 3;
		}
#ifdef DYNAMIC_ALLOC_DMA_BUF
		memcpy(atmCellBuf->word, (uint8 *)oamCellp, 52);
		if(!atmCcDataReq((uint8 *)atmCellBuf->word)){
#else
		if(!atmCcDataReq((uint8 *)oamCellp)){
#endif
			atm_p->MIB_II.outF4Pkts++;
			atm_p->MIB_II.outPkts++;
		}
	}
	return 0;
}

void
atmRxIrqQueueSet(
	uint32 irqBase,
	uint32 irqLength
)
{
	TSARM_IRQBA = K0_TO_PHYSICAL(irqBase);
	TSARM_IRQLEN = irqLength;
}

void initMPOA(uint32 vc, qosProfile_t *qos_p)
{
	//re-init reg
	TSARM_MPOA_GCR &= ~(1<<(vc+16) | (1<<vc));
	TSARM_VC_MPOA_CTRL(vc) = 0x0;

	//MPOA global control register
	if(qos_p->mode == MODE_ROUTER){				//router mode
			if(qos_p->encapType == ENCAP_RFC2364)
				TSARM_MPOA_GCR |= 1 << (vc+16) | 1 << vc;
			else
				TSARM_MPOA_GCR |= 0 << (vc+16) | 1 << vc;
	}else if(qos_p->mode == MODE_BRIDGE){		//bridge mode
		TSARM_MPOA_GCR |= 0 << (vc+16) | 0 << vc;
	}
	//VC 0 ~ 9 control register
	if(qos_p->muxType == MUX_VC){
		switch(qos_p->encapType){
			case ENCAP_RFC1483:
				if(qos_p->mode == MODE_ROUTER){				//router mode
//					TSARM_VC_MPOA_CTRL(vc) |= 0x0;
					TSARM_VC_MPOA_CTRL(vc) |= 0x0200;       // Jason_20060710 workaround the HW MPOA problem
				}else if(qos_p->mode == MODE_BRIDGE){		//bridge mode
					TSARM_VC_MPOA_CTRL(vc) |= 0x0340;	//(00001, 0100, 0000){Field4Value1}
				}
				break;
			case ENCAP_RFC2364:		//PPPoA
//				TSARM_VC_MPOA_CTRL(vc) |= 0x0;
				TSARM_VC_MPOA_CTRL(vc) |= 0x0200;       // Jason_20060710 workaround the HW MPOA problem
				break;
			case ENCAP_MER:
				TSARM_VC_MPOA_CTRL(vc) |= 0x0340;	//(00001, 0100, 0000){Field4Value1}
				break;
			case ENCAP_POE:
				TSARM_VC_MPOA_CTRL(vc) |= 0x0340;	//(00001, 0100, 0000){Field4Value1}
			default:
				break;
		}
	}else if(qos_p->muxType == MUX_LLC){			//MUX_LLC
		switch(qos_p->encapType){
			case ENCAP_RFC1483:
				if(qos_p->mode == MODE_ROUTER){				//router mode
					//(00001, 1000, 1001){Field4Value2, Field2Value2, Field1Value1}
					TSARM_VC_MPOA_CTRL(vc) |= 0x0389;
				}else if(qos_p->mode == MODE_BRIDGE){		//bridge mode
					//(00001, 0110, 0101){Field4Value1, Field3Value2, Field2Value1, Field1Value1}
					TSARM_VC_MPOA_CTRL(vc) |= 0x0365;
				}
				break;
			case ENCAP_RFC2364:		//PPPoA
				//(0001, 1111, 0000){Field4Valu3, Field3Value3}
				TSARM_VC_MPOA_CTRL(vc) |= 0x03f0;
				break;
			case ENCAP_MER:
				//(00001, 0110, 0101){Field4Value1, Field3Value2, Field2Value1, Field1Value1}
				TSARM_VC_MPOA_CTRL(vc) |= 0x0365;
				break;
			case ENCAP_POE:
				//(00001, 0110, 0101){Field4Value1, Field3Value2, Field2Value1, Field1Value1}
				TSARM_VC_MPOA_CTRL(vc) |= 0x0365;
			default:
				break;
		}
	}
 /*
  *  to avoid function dependency problem, move hareware vlan untag setting
  *  to doTsarmHwVlanUntagSwitch function
  *
  *  frankliao removed 20100713
  */
//	#ifdef SAR_VERIFY
//	enHwVlanUntag(vc);
//	#endif

}

uint32
tc3162l2AtmMbsCalCulate(
	uint16 pcr,
	uint16 scr,
	uint16 mbs
)
{
	uint32 cal_mbs;
	cal_mbs = (uint32)(mbs*(1-(scr * (1 /(float)pcr))));
	if(cal_mbs == 0)
		cal_mbs = 1;
	return cal_mbs;
}

void
tc3162l2AtmRateCalCulate(
	uint16 tb,
	uint16 cellRate,
	uint8 *dec_p,
	uint16 *init_p
)
{
	uint16 dec, init;
	uint32 calCellRate;
	uint32 diffSquare = 0xffffffff;
	uint32 closesetSquare = 0xff;

	float calRateParam=0;
	float calRateParam_f=0;
	int i = 0, j = 0;

#if 0
	/*add by zyzhang, 20080123
	Question: cpe crash when cell rate is small
	Root cause: if the cell rate is too small, timebase(default: 59) is not appropriate;
	                  Through emluting, timebase(default: 59) is not appropriate to high cell rate too.
			    Thess cases will cause cpu 100%, and mpoa state machine and conn-mgr state machine can't work properly*/
	#if defined(TC3262_FPGA)	//for FPGA
		;	//reserved for FPGA test
	#else
	if(cellRate <= 0x29 && cellRate > 0x1f)
		tb = 256;
	else if(cellRate <= 0x53 && cellRate > 0x29)
		tb = 199;
	else if(cellRate <= 0x87 && cellRate > 0x53)
		tb = 98;
	else if(cellRate < 0x5000 && cellRate > 0x87)
		;	//do nothing, don't change timebase, the default timebase is 59. please refer to treg_tstbr
	else if(cellRate>=0x5000)	//cell rate >= 0x5000(may be imposible now). if cell rate is too big, this algorithem is out of action
		tb = 2;
	else{	//cell rate <= 0x1f(may be imposible actually), this algorithem is out of action
		*dec_p = 1;
		*init_p = 0xff5;
		return;
	}
	#endif

#endif
	/* find ATM Qos parameter in table */
	for(i = 0; i < MAX_ATMQOS_CNT; i++){
		if((ATMQosParam[i].dec == 0)&&(ATMQosParam[i].init == 0))
			continue;
		else
		{
			if(ATMQosParam[i].cellRate == cellRate){
				*dec_p = ATMQosParam[i].dec;
				*init_p = ATMQosParam[i].init;
				return;
			}
		}
	}

	//calRateParam=(float)SAR_CLK*1000000;
	calRateParam_f=(float)SAR_CLK*1000000*(1/(float)(tb+1));
	
	for ( dec = 1; dec <= 0x07f; dec++ ) {  /* PCR_dec or SCR_dec are both 7 bits long */
		diffSquare = 0xffffffff;
		calRateParam=calRateParam_f*(float)dec;
		for ( init = dec; init <= 0x0fff; init++ ) {  /* PCR_init or SCR_init are both 12 bits long */
            		//calCellRate= calRateParam * dec * (1/(float)(tb+1)) * (1/(float)init);
			calCellRate= calRateParam*(1/(float)init); 
			diffSquare = calCellRate - cellRate;
			if ( diffSquare < closesetSquare ) {
				closesetSquare = diffSquare;
				*dec_p = dec;
				*init_p = init;
				if ( diffSquare == 0 ) {
					/* Perfectly match */
					goto Store; /*store the parameter to table and you can find next time when cellRate is the same*/
					//return;
				}
			}
		}
	}
Store:
	for(j = 0; j < MAX_ATMQOS_CNT; j++){
		if((ATMQosParam[j].dec == 0)&&(ATMQosParam[j].init == 0)){
			ATMQosParam[j].cellRate = cellRate;
			ATMQosParam[j].dec = *dec_p;
			ATMQosParam[j].init = *init_p;
			break;
		}	
	}
	if(j == MAX_ATMQOS_CNT){ /*The table is full and modify from the last one*/
		ATMQosParam[j-1].cellRate = cellRate;
		ATMQosParam[j-1].dec = *dec_p;
		ATMQosParam[j-1].init = *init_p;
	}
	return;
}
/*______________________________________________________________________________
**  setAtmQosRed
**
**  descriptions:Used to set Atm Qos register
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:atmAal5VcOpen,update_qosred
**  call:
**____________________________________________________________________________*/
static int
setAtmQosRed(
	qosProfile_t *qos_p,	
	uint32 vc
)
{
	uint32 dslUpRate;
	uint8 pcr_dec;
	uint8 scr_dec;
	uint8 lr_dec;
	uint16 pcr_init;
	uint16 scr_init;
	uint16 lr_init;
	uint32 cal_mbs;

	switch (qos_p->type)
	{
	    case CBR	: qos_p->type = TSARM_QOS_CBR;		break;
	    case UBR	: qos_p->type = TSARM_QOS_UBR;		break;
	    case VBR	: qos_p->type = TSARM_QOS_rtVBR;	break;
	    case nrtVBR	: qos_p->type = TSARM_QOS_nrtVBR;	break;
	    default     : qos_p->type = TSARM_QOS_UBR;		break;
	}

	dslUpRate = getXdslSpeed(); /* Kbps */
	if (dslUpRate <= 0)
	{
		dslUpRate = 1024;
	}
	dslUpRate = (dslUpRate * 1000) / (53 * 8);	/* cell rate */
	if(dslUpRate % 53 !=0){
		dslUpRate=dslUpRate+1;
	}

	if (pcr_modify_flag)
	{
		qos_p->pcr = g_qos_pcr;
	
	}
	if ((qos_p->pcr == 0) || (qos_p->pcr > dslUpRate))
		qos_p->pcr = dslUpRate;
	if ((qos_p->scr == 0) || (qos_p->scr > qos_p->pcr))
		qos_p->scr = qos_p->pcr;
	if (qos_p->mbs == 0)
		qos_p->mbs = 1;

	//init line rate
	tc3162l2AtmRateCalCulate((uint16)treg_tstbr, (uint16)dslUpRate, &lr_dec, &lr_init);
	TSARM_TXSLRC = (lr_dec << 16) | lr_init;

	/* ----- Set up vpi, vci, and QoS parameters of the vc ----- */
	//calculate rate.......
	if (( qos_p->type == TSARM_QOS_CBR) || (qos_p->type == TSARM_QOS_UBR) ) {
		tc3162l2AtmRateCalCulate((uint16)treg_tstbr, qos_p->pcr, &scr_dec, &scr_init);
		if(getXdslModeType() == ME_CMD_ADSL_ANNEXM){
			qos_p->mbs = 2; 
		}
		else{
			qos_p->mbs = 1; 								// if CBR, mbs must be 1
		}
		
		TSARM_SCR(vc) = (scr_dec << 16) | scr_init;
		TSARM_PCR(vc) = (scr_dec << 16) | scr_init;
		if(qos_p->type == TSARM_QOS_CBR)
			TSARM_MBSTP(vc) = (1 << 28) |(0 << 26) |(qos_p->mbs << 2) | qos_p->type;
		else
			TSARM_MBSTP(vc) = (1 << 28) |(3 << 26) |(qos_p->mbs << 2) | qos_p->type;
	} else {
		tc3162l2AtmRateCalCulate((uint16)treg_tstbr, qos_p->pcr, &pcr_dec, &pcr_init);
		tc3162l2AtmRateCalCulate((uint16)treg_tstbr, qos_p->scr, &scr_dec, &scr_init);	
		TSARM_PCR(vc) = (pcr_dec << 16) | pcr_init;
		TSARM_SCR(vc) = (scr_dec << 16) | scr_init;
		cal_mbs = tc3162l2AtmMbsCalCulate(qos_p->pcr, qos_p->scr, qos_p->mbs);
		if(qos_p->type == TSARM_QOS_rtVBR)
			TSARM_MBSTP(vc) = (1 << 28) |(1 << 26) | (cal_mbs << 2) | qos_p->type;    //for new L2 SAR
		else
			TSARM_MBSTP(vc) = (1 << 28) |(2 << 26) | (cal_mbs << 2) | (2 << 0);
	}

	if (sarDebugFlag){
		dbg_plinel_1("\r\n vc:", vc);
		dbg_plinel_1(" PCR:", TSARM_PCR(vc));
		dbg_plinel_1(" SCR:", TSARM_SCR(vc));
		dbg_plinel_1(" MBSTP:", TSARM_MBSTP(vc));
		dbg_plinel_1("\r\n TSARM_MPOA_GCR:", TSARM_MPOA_GCR);
		dbg_plinel_1(" TSARM_VC_MPOA_CTRL:", TSARM_VC_MPOA_CTRL(vc));
		dbg_pline_1("\r\n");
	}
	return 0;
}

#if defined(TCSUPPORT_CT)
extern atomic_t g_used_skb_num;
#endif
/*______________________________________________________________________________
**  atmAal5VcOpen
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
int
atmAal5VcOpen(
	uint8 vpi,
	uint16 vci,
	qosProfile_t *qos_p,
	struct atm_vcc *vcc
)
{
	uint32 i, vc = ATM_VC_MAX;
	//uint8 pcr_dec;
	//uint8 scr_dec;
	//uint8 lr_dec;
	//uint16 pcr_init;
	//uint16 scr_init;
	//uint16 lr_init;
	uint32 temp;
	//uint32 dslUpRate;
	//uint32 cal_mbs;

	if (sarDebugFlag) {
		dbg_plineb_1("\r\n open vpi=", vpi);
		dbg_plinew_1(" vci=", vci);
		dbg_plinew_1(" type=", qos_p->type);
		dbg_plinew_1(" pcr=", qos_p->pcr);
		dbg_plinew_1(" scr=", qos_p->scr);
		dbg_plinew_1(" mbs=", qos_p->mbs);
	}

	/* ----- Check the number of total opened vc ----- */
	if ( atmCfgTable.vcNumber >= ATM_VC_MAX ) {
		return 1;
	}

	for ( i = 0; i < ATM_VC_MAX; i++ ) {
		if ( atmCfgTable.openFlag[i] == 0 ) {
			atmCfgTable.openFlag[i] = 1;
			vc = i;
			atmCfgTable.vpi[vc] = vpi;
			atmCfgTable.vci[vc] = vci;
			memcpy( (uint8*)&qosRecord[vc],(uint8*)qos_p, sizeof(qosProfile_t));
			atmCfgTable.vcc[vc] = vcc;
			atmCfgTable.vcNumber++;
			break;
		}
	}

	if (vc == ATM_VC_MAX)
		return 1;

#if defined(TCSUPPORT_CT)
#ifdef DYNAMIC_ALLOC_SKB
	if(g_NoNeedFreeSKB != 1){
		if(atmRxSkbAlloc(vc) == -1){
			printk("atmRxSkbAlloc FAILED\n");
			printk("skb_buff_alloc_num is %d\n", atomic_read(&g_used_skb_num));
			dump_stack();
			return 1;
		}
	}
#endif
#endif

#if 1
	setAtmQosRed(qos_p,vc);
#else
	switch (qos_p->type)
	{
	    case CBR	: qos_p->type = TSARM_QOS_CBR;		break;
	    case UBR	: qos_p->type = TSARM_QOS_UBR;		break;
	    case VBR	: qos_p->type = TSARM_QOS_rtVBR;	break;
	    case nrtVBR	: qos_p->type = TSARM_QOS_nrtVBR;	break;
	    default     : qos_p->type = TSARM_QOS_UBR;		break;
	}

	dslUpRate = getXdslSpeed(); /* Kbps */
	if (dslUpRate <= 0)
	{
		dslUpRate = 1024;
	}
	dslUpRate = (dslUpRate * 1000) / (53 * 8);	/* cell rate */
	if(dslUpRate % 53 !=0){
		dslUpRate=dslUpRate+1;
	}

	if ((qos_p->pcr == 0) || (qos_p->pcr > dslUpRate))
		qos_p->pcr = dslUpRate;
	if ((qos_p->scr == 0) || (qos_p->scr > qos_p->pcr))
		qos_p->scr = qos_p->pcr;
	if (qos_p->mbs == 0)
		qos_p->mbs = 1;

	//init line rate
	tc3162l2AtmRateCalCulate((uint16)treg_tstbr, (uint16)dslUpRate, &lr_dec, &lr_init);
	TSARM_TXSLRC = (lr_dec << 16) | lr_init;

	/* ----- Set up vpi, vci, and QoS parameters of the vc ----- */
	//calculate rate.......
	if (( qos_p->type == TSARM_QOS_CBR) || (qos_p->type == TSARM_QOS_UBR) ) {
		tc3162l2AtmRateCalCulate((uint16)treg_tstbr, qos_p->pcr, &scr_dec, &scr_init);
		qos_p->mbs = 1; 								// if CBR, mbs must be 1
		TSARM_SCR(vc) = (scr_dec << 16) | scr_init;
		TSARM_PCR(vc) = (scr_dec << 16) | scr_init;
		if(qos_p->type == TSARM_QOS_CBR)
			TSARM_MBSTP(vc) = (1 << 28) |(0 << 26) |(qos_p->mbs << 2) | qos_p->type;
		else
			TSARM_MBSTP(vc) = (1 << 28) |(3 << 26) |(qos_p->mbs << 2) | qos_p->type;
	} else {
		tc3162l2AtmRateCalCulate((uint16)treg_tstbr, qos_p->pcr, &pcr_dec, &pcr_init);
		tc3162l2AtmRateCalCulate((uint16)treg_tstbr, qos_p->scr, &scr_dec, &scr_init);
		TSARM_PCR(vc) = (pcr_dec << 16) | pcr_init;
		TSARM_SCR(vc) = (scr_dec << 16) | scr_init;
		cal_mbs = tc3162l2AtmMbsCalCulate(qos_p->pcr, qos_p->scr, qos_p->mbs);
		if(qos_p->type == TSARM_QOS_rtVBR)
			TSARM_MBSTP(vc) = (1 << 28) |(1 << 26) | (cal_mbs << 2) | qos_p->type;    //for new L2 SAR
		else
			TSARM_MBSTP(vc) = (1 << 28) |(2 << 26) | (cal_mbs << 2) | (2 << 0);
	}

	if (sarDebugFlag){
		dbg_plinel_1("\r\n vc:", vc);
		dbg_plinel_1(" PCR:", TSARM_PCR(vc));
		dbg_plinel_1(" SCR:", TSARM_SCR(vc));
		dbg_plinel_1(" MBSTP:", TSARM_MBSTP(vc));
		dbg_plinel_1("\r\n TSARM_MPOA_GCR:", TSARM_MPOA_GCR);
		dbg_plinel_1(" TSARM_VC_MPOA_CTRL:", TSARM_VC_MPOA_CTRL(vc));
		dbg_pline_1("\r\n");
	}
#endif
	TSARM_IRQM(vc) = treg_irqm;
	// read to clear Receive Counter
	temp = TSARM_TDCNT(vc);
	temp = TSARM_RDCNT(vc);

	// set up VC Configuration as valid =1, raw =0(AAL5 mode), port =0, VPI, VCI
	TSARM_VCCR(vc) =
		VCCFGR_VPI(vpi) | VCCFGR_VCI(vci) |
		VCCFGR_PORT(VCCFGR_ATM_PHY0) | VCCFGR_VALID;

	// to prevent 1/2^(28) error chance
	VPint(0xbfb60128) = 0x1;

	// workaround for abnormal irq report
	TSARM_VCCR(10) = VCCFGR_VPI(vpi) | VCCFGR_VCI(vci) |
		VCCFGR_PORT(VCCFGR_ATM_PHY0) | VCCFGR_VALID;

	/* data cell */
	TSARM_RXDBCSR |= 1 << vc;
	/* management cell */
	TSARM_RXMBCSR |= 1 << vc;

	return 0;
}

/*______________________________________________________________________________
**  atmAal5VcClose
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
uint8
atmAal5VcClose(
	uint8 vpi,
	uint16 vci
)
{
	uint8 vc, i;
	int priority=0;

	vc = atmVcNumberGet(vpi, vci);
	if ( vc == ATM_DUMMY_VC )
		return 1;

	if (sarDebugFlag) {
		dbg_plineb_1("\r\n close vpi=", vpi);
		dbg_plinew_1(" vci=", vci);
	}

	// VC configuration
	TSARM_VCCR(vc) = 0;
	// Transmit Buffer Descriptor
	//TSARM_TXDCBDA(vc) = 0;
	// OAM Transmit Buffer Descriptor
	//TSARM_TXMCBDA(vc)=0;
	// Receive Buffer Descriptor
	//TSARM_RXDCBDA(vc)=0;
	// OAM Receive Buffer Descriptor
	//TSARM_RXMCBDA(vc) = 0;
	// Traffic Scheduler - PCR
	TSARM_PCR(vc) = 0;
	// Traffic Scheduler - SCR
	TSARM_SCR(vc) = 0;
	// Traffic Scheduler - MBSTP
	TSARM_MBSTP(vc) = 0;

	atmCfgTable.openFlag[vc] = 0;
	atmCfgTable.vpi[vc] = atmCfgTable.vci[vc] = 0;
	atmCfgTable.vcc[vc] = NULL;
	atmCfgTable.vcNumber--;
	#ifdef CONFIG_VLAN_ATM
	resetVlanVcc(vpi, vci);
	#endif

#if defined(TCSUPPORT_CT)
#ifdef DYNAMIC_ALLOC_SKB
	if(g_NoNeedFreeSKB != 1){
		atmRxSkbFree(vc);
	}
#endif
#endif
	local_irq_disable();
	atmTxDescrFree(vc, 1);
	/*Reset the hardware tx descriptor index*/
	TSARM_VC_TX_BD_PRIORITY01(vc) = TSARM_VC_TX_BD_PRIORITY01(vc) ;
	TSARM_VC_TX_BD_PRIORITY23(vc) = TSARM_VC_TX_BD_PRIORITY23(vc);
	for(priority = 0; priority < ATM_TX_PRIORITY_MAX; priority ++){
		atmTxVcFreeDescrp[priority][vc] = atmTxDescrPoolAddr[vc].pTxDescr[priority];
		atmTxVcBusyDescrp[priority][vc] = atmTxDescrPoolAddr[vc].pTxDescr[priority];
	}
	local_irq_enable();

	// workaround for abnormal irq report
	for ( i = 0; i < ATM_VC_MAX; i++ ) {
		if ( atmCfgTable.openFlag[i] == 1 ) {
			TSARM_VCCR(10) =
				VCCFGR_VPI(atmCfgTable.vpi[i]) | VCCFGR_VCI(atmCfgTable.vci[i]) |
				VCCFGR_PORT(VCCFGR_ATM_PHY0) | VCCFGR_VALID;
			break;
		}
	}
	if (i == ATM_VC_MAX)
		TSARM_VCCR(10) = VCCFGR_VPI(255) | VCCFGR_VCI(15) |
			VCCFGR_PORT(VCCFGR_ATM_PHY0) | VCCFGR_VALID;

	return 0;
}

/*______________________________________________________________________________
**  atmOamOpen
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
int
atmOamOpen(
	void
)
{
	TSARM_CCCR = VCCFGR_VPI(0) | VCCFGR_VCI(0) |
		VCCFGR_PORT(VCCFGR_ATM_PHY0) | VCCFGR_VALID;

	TSARM_IRQMCC = 0x202;

	TSARM_RXMBCSR |= 1 << 16;

	return 0;
}

void atmTxPriDescrFree(uint8 vc, uint8 priority, uint8 free_any){
	volatile atmTxDescr_t *atmTxBusyDescrp;
	uint16 i;
	uint16 len = atmTxPriVcCnt[priority][vc];

	for (i = 0; i < len; i++) {
		atmTxBusyDescrp = atmTxVcBusyDescrp[priority][vc];
		if ((!(atmTxBusyDescrp->tdes0 & TSARM_TX_DESCR_VALID)) || free_any) {
			if ( atmTxBusyDescrp->skb ) {
				/* Free descr */
				#ifdef SAR_VERIFY
				if (free_any || test_flag) {
				#else
				if (free_any) {
				#endif
					dev_kfree_skb_any(atmTxBusyDescrp->skb);
				} else {
					if (ATM_SKB(atmTxBusyDescrp->skb)->vcc->pop) {
						ATM_SKB(atmTxBusyDescrp->skb)->vcc->pop (ATM_SKB(atmTxBusyDescrp->skb)->vcc, atmTxBusyDescrp->skb);
					} else {
						dev_kfree_skb_any(atmTxBusyDescrp->skb);
					}
				}
				atmTxBusyDescrp->tdes1 = 0x0;
                                if(free_any)
				    atmTxBusyDescrp->tdes0 &= ~TSARM_TX_DESCR_VALID;
				atmTxBusyDescrp->skb = NULL;
				if(atmTxBusyDescrp->tdes0 & TSARM_TX_DESCR_EOR){
					atmTxVcBusyDescrp[priority][vc] = atmTxDescrPoolAddr[vc].pTxDescr[priority];
				}else{
					atmTxVcBusyDescrp[priority][vc]++;
				}
				atmTxPriVcCnt[priority][vc]--;
				atmTxVcCnt[vc]--;
				atmTxVcTotalCnt--;
			}
		} else {
			break;
		}
	}
}
/*______________________________________________________________________________
**  atmTxDescrFree
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
void
atmTxDescrFree(
	uint8 vc, uint8 free_any
)
{
	uint8 priority;

	for(priority = 0; priority < ATM_TX_PRIORITY_MAX; priority++){
		atmTxPriDescrFree(vc, priority, free_any);
	}
}


/*______________________________________________________________________________
**  atmIsr
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
#ifdef SAR_POLLING
static struct task_struct *polling_task = 0;
int isrPollingFlag = 0;
extern int tcSarIsrPollingMode;
#endif
irqreturn_t
atmIsr(
	int irq , void *dev
)
{
	register uint32 atmRxIrqReport;
	uint32 atmRxIrqHeadOffset;
	uint8 vc;
	register atmRxDescr_t *atmRxDescrp;
	atmRxCcDescr_t *atmRxCcDescrp;
	struct sk_buff *skb;
	atmRxDescrPool_t *atmRxDescrVcPoolp;
	atmRxCcDescrPool_t *atmRxCcDescrPoolp;
	uint32 len=0;
	uint32 atmIrqEventCheckTime = 0;

	ledTurnOn(LED_DSL_ACT_STATUS);

	atmIrqEventCheckTime = 0;
ISREVENTREREAD:
	
	//loop while there are irq entries on irq queue
	while ((TSARM_IRQH & 0x00fff000) >> 12) {
		//get the irq offset value
		atmRxIrqHeadOffset = TSARM_IRQH & 0x00000fff;
		//get the irq entry content
		atmRxIrqReport = *(volatile uint32 *) PHYSICAL_TO_K1(TSARM_IRQBA + (atmRxIrqHeadOffset << 2));
		vc = LIRQ_GET_VC_NO(atmRxIrqReport);
		/* mask the vc field out */
		atmRxIrqReport = atmRxIrqReport & 0xfe0fffff;
		if(irqrereadflag){
			if(atmRxIrqReport == 0)
			{
				atmIrqEventCheckTime++;
				if(irqrereadmax < atmIrqEventCheckTime)
				{
					irqrereadmax = atmIrqEventCheckTime;
				}
				if(atmIrqEventCheckTime >= 1000)
				{
					dbg_pline_1("\n\natmIrqEventCheckTime >= 1000, error case! need software reset atm sar\n\n");
					goto ISRPROCESSCONTINUE;
				}
				goto ISREVENTREREAD;
			}
			else
			{
				if(atmIrqEventCheckTime)
				{
					irqrereadtime++;
				}
			}
		}
	ISRPROCESSCONTINUE:
		
		#ifdef SAR_VERIFY
		dumpIRQEvent(vc, atmRxIrqReport);
		#endif
		/* If the irq queue is full, we must to reset the sar modules.
		 * This bit will be also inclued another event,so if the we 
		 * must to handle this event at first,system won't to ignore the
		 * irq queue full event.
		 */
		if (atmRxIrqReport & LIRQ_IRQ_Q_FULL) {
			atmReset();
			atm_p->MIB_II.irqFull++;
			return IRQ_HANDLED;
		}
		//the successful receive user data done event will be handled first, the others handled below.
		/*When descriptor ring is full, sar will receive (LIRQ_RX_U_DONE | LIRQ_RX_U_BD_OV) interrupt.
			shnwind 20101005*/
		else if (atmRxIrqReport == LIRQ_RX_U_DONE || (atmRxIrqReport == (LIRQ_RX_U_DONE |LIRQ_RX_U_BD_OV)))  {
			atmRxDescrp = atmRxVcDescrp[vc];
			skb = atmRxDescrp->skb;
			if (!(atmRxDescrp->rdes0 & TSARM_RX_DESCR_VALID)) {
				len = (uint32) (atmRxDescrp->rdes1 & 0xffff);
				#ifdef  SAR_VERIFY
				skb=atmDataLpbkHandler(atmRxDescrp, skb, vc);
				#else
				if (len && (len <= atmRxBufferSize)) {
					skb = atmAal5DataInd(skb, vc, len);
				}
				#endif
			} else {
				atm_p->MIB_II.inDiscards++;
				atm_p->MIB_II.inBufErr++;
			}
			atmRxDescrp->rdes1 = atmRxBufferSize << 16;	/* Buffer size */
			atmRxDescrp->rdes2 = K1_TO_PHYSICAL(skb->data);
			atmRxDescrp->skb = skb;
			atmRxDescrp->rdes0 |= TSARM_RX_DESCR_VALID;
			if(atmRxDescrp->rdes0 & TSARM_RX_DESCR_EOR){
				atmRxDescrVcPoolp = (atmRxDescrPool_t *)CACHE_TO_NONCACHE(atmRxDescrPool + vc);
				atmRxVcDescrp[vc] = atmRxDescrVcPoolp->rxDescrPool;
			}else{
				atmRxVcDescrp[vc]++;
			}
	//	} else if (atmRxIrqReport & LIRQ_TX_U_DONE) {
#if defined(CONFIG_NET_SCHED) || defined(TCSUPPORT_CT_SWQOS)  /*Rodney_20091115*/
		} else if (atmRxIrqReport & LIRQ_TX_U_DONE) {
#if defined(TCSUPPORT_CT_SWQOS)
			if (sw_PKTQOS_CLEAR_STOP)		
				sw_PKTQOS_CLEAR_STOP();
#else
			if((qos_qdisc_mode == QOS_QDISC_RED) || (qos_qdisc_mode == QOS_QDISC_WRED)){
				if(atmCfgTable.vcc[vc]->_dev != NULL)
					netif_wake_queue(atmCfgTable.vcc[vc]->_dev);				
			}
			atmTxDescrFree(vc,0);
#endif
#endif		
		} else if (atmRxIrqReport & LIRQ_RX_U_BD_OV) {
			if( vc < 10 ) {
			#if 0 /*When overflow happen, sar will not receive any packet.
				We should not drop here. shnwind 20101005*/
				atmRxDescrp = atmRxVcDescrp[vc];
				if (!(atmRxDescrp->rdes0 & TSARM_RX_DESCR_VALID)) {
					atm_p->MIB_II.inDiscards++;
					atm_p->MIB_II.inBufErr++;

					skb = atmRxDescrp->skb;
					atmRxDescrp->rdes1 = 1776 << 16;	/* Buffer size */
					atmRxDescrp->rdes2 = K1_TO_PHYSICAL(skb->data);
					atmRxDescrp->skb = skb;
					atmRxDescrp->rdes0 |= TSARM_RX_DESCR_VALID;
					if(atmRxDescrp->rdes0 & TSARM_RX_DESCR_EOR){
						atmRxDescrVcPoolp = (atmRxDescrPool_t *)CACHE_TO_NONCACHE(atmRxDescrPool + vc);
						atmRxVcDescrp[vc] = atmRxDescrVcPoolp->rxDescrPool;
					}else{
						atmRxVcDescrp[vc]++;
					}
				}
			#endif	
				TSARM_RXDBCSR |= 1 << vc;
				atm_p->MIB_II.inDiscards++;
				atm_p->MIB_II.inBufDescrOverflow++;
			} else {
				atmIrqStSaved(atmRxIrqReport, vc);
				atm_p->MIB_II.inCcUBDOV++;
				atmReset();
				return IRQ_HANDLED;
			}
		} else if ((atmRxIrqReport & LIRQ_RX_CRC32E) ||
			(atmRxIrqReport & LIRQ_RX_U_BOV) ||
			(atmRxIrqReport & LIRQ_RX_MAXLENE) ||
			(atmRxIrqReport & LIRQ_RX_LENE)) {

			// The VC should be leass than 10, but we can get the VC=10.
			// workaround the abnormal situation. // Jason_20060105
			if( vc < 10 ) {
			    atmRxDescrp = atmRxVcDescrp[vc];

			    atm_p->MIB_II.inDiscards++;
			    if (atmRxIrqReport & LIRQ_RX_CRC32E){
				    atm_p->MIB_II.inCrcErr++;
#ifdef CMD_API
				    pvc_stats[vc].crc_errs++;
#endif
			    }
			    else if (atmRxIrqReport & LIRQ_RX_U_BOV)
				    atm_p->MIB_II.inBufOverflow++;
			    else if (atmRxIrqReport & LIRQ_RX_MAXLENE){
				    atm_p->MIB_II.inBufMaxLenErr++;
#ifdef CMD_API
				    pvc_stats[vc].oversized_sdus++;
#endif
			    }
			    else if (atmRxIrqReport & LIRQ_RX_LENE){
				    atm_p->MIB_II.inBufLenErr++;
#ifdef CMD_API
				    pvc_stats[vc].len_errs++;
#endif
			    }

			    skb = atmRxDescrp->skb;

			    atmRxDescrp->rdes1 = atmRxBufferSize << 16;	/* Buffer size */
			    atmRxDescrp->rdes2 = K1_TO_PHYSICAL(skb->data);
			    atmRxDescrp->skb = skb;
			    atmRxDescrp->rdes0 |= TSARM_RX_DESCR_VALID;

			    if(atmRxDescrp->rdes0 & TSARM_RX_DESCR_EOR){
				    atmRxDescrVcPoolp = (atmRxDescrPool_t *)CACHE_TO_NONCACHE(atmRxDescrPool + vc);
				    atmRxVcDescrp[vc] = atmRxDescrVcPoolp->rxDescrPool;
			    }else{
				    atmRxVcDescrp[vc]++;
			    }
			} else {
				atmIrqStSaved(atmRxIrqReport, vc);
				atm_p->MIB_II.inCcUDoneErr++;
				atmReset();
				return IRQ_HANDLED;
			}
#if 0	 /*Move to handle the irq queue full envet to top.*/
		} else if (atmRxIrqReport & LIRQ_IRQ_Q_FULL) {
			atmReset();
			return IRQ_HANDLED;
#endif
		} else if (atmRxIrqReport & LIRQ_RX_M_BD_OV) {
			if (vc == 0x0a) {		/* CC ...for L2-> vc = 0x0a means oam or cc cell.*/
				atmRxCcDescrp = atmRxCcBusyDescrp;
		 		atmRxCcDescrp->rdes1 = (96 << 16);
				atmRxCcDescrp->rdes0 |= TSARM_RX_DESCR_VALID;
				if(atmRxCcDescrp->rdes0 & TSARM_RX_DESCR_EOR){
#ifdef DYNAMIC_ALLOC_DMA_BUF
					atmRxCcDescrPoolp = (atmRxCcDescrPool_t *)CACHE_TO_NONCACHE(atmRxCcDescrPool);
#else
					atmRxCcDescrPoolp = (atmRxCcDescrPool_t *)CACHE_TO_NONCACHE(&atmRxCcDescrPool);
#endif
					atmRxCcBusyDescrp = atmRxCcDescrPoolp->rxCcDescrPool;
				}else{
					atmRxCcBusyDescrp++;
				}
				TSARM_RXMBCSR |= 1 << 16;
				atm_p->MIB_II.inCcBufDescrOverflow++;
			} else {				/* OAM ...for L2-> vc = 0x0a means oam or cc cell. */
				atmRxCcDescrp = atmRxOamBusyDescrp[vc];
				atmRxCcDescrp->rdes1 = (48 << 16);
				atmRxCcDescrp->rdes0 |= TSARM_RX_DESCR_VALID;
				if(atmRxCcDescrp->rdes0 & TSARM_RX_DESCR_EOR){
					atmRxDescrVcPoolp = (atmRxDescrPool_t *)CACHE_TO_NONCACHE(atmRxDescrPool + vc);
					atmRxOamBusyDescrp[vc] = atmRxDescrVcPoolp->rxOamDescrPool;
				}else{
					atmRxOamBusyDescrp[vc]++;
				}
				TSARM_RXMBCSR |= 1 << vc;
				atm_p->MIB_II.inMngBufDescrOverflow++;
			}
		} else if (atmRxIrqReport & LIRQ_RX_CRC10E) {
			if (vc == 0x0a) {		/* CC ...for L2-> vc = 0x0a means oam or cc cell.*/
				atmRxCcDescrp = atmRxCcBusyDescrp;
				#ifdef L2_AUTOPVC
				if (g_manageFlag & VC_HUNT_FLAG_MANG_START){
					RawCcCellRecv((uint8 *)CACHE_TO_NONCACHE(PHYSICAL_TO_K1(atmRxCcDescrp->rdes2)));
				}
				#endif
		 		atmRxCcDescrp->rdes1 = (96 << 16);
				atmRxCcDescrp->rdes0 |= TSARM_RX_DESCR_VALID;
				if(atmRxCcDescrp->rdes0 & TSARM_RX_DESCR_EOR){
#ifdef DYNAMIC_ALLOC_DMA_BUF
					atmRxCcDescrPoolp = (atmRxCcDescrPool_t *)CACHE_TO_NONCACHE(atmRxCcDescrPool);
#else
					atmRxCcDescrPoolp = (atmRxCcDescrPool_t *)CACHE_TO_NONCACHE(&atmRxCcDescrPool);
#endif
					atmRxCcBusyDescrp = atmRxCcDescrPoolp->rxCcDescrPool;
				}else{
					atmRxCcBusyDescrp++;
				}
			} else {				/* OAM ...for L2-> vc = 0x0a means oam or cc cell. */
				atmRxCcDescrp = atmRxOamBusyDescrp[vc];
				atmRxCcDescrp->rdes1 = (48 << 16);
				atmRxCcDescrp->rdes0 |= TSARM_RX_DESCR_VALID;
				if(atmRxCcDescrp->rdes0 & TSARM_RX_DESCR_EOR){
					atmRxDescrVcPoolp = (atmRxDescrPool_t *)CACHE_TO_NONCACHE(atmRxDescrPool + vc);
					atmRxOamBusyDescrp[vc] = atmRxDescrVcPoolp->rxOamDescrPool;
				}else{
					atmRxOamBusyDescrp[vc]++;
				}
			}
			atm_p->MIB_II.inCrc10Err++;
		} else if (atmRxIrqReport & LIRQ_RX_M_DONE) {
			if (vc == 0x0a) {		/* CC ...for L2-> vc = 0x0a means oam or cc cell. */
				atmRxCcDescrp = atmRxCcBusyDescrp;
				if ( !(atmRxCcDescrp->rdes0 & TSARM_RX_DESCR_VALID) ) {
					#ifndef SAR_VERIFY
					atmCcHandler((uint8 *)PHYSICAL_TO_K1(atmRxCcDescrp->rdes2));
					#else
					atmCcLpbkHandler((uint8 *)PHYSICAL_TO_K1(atmRxCcDescrp->rdes2));
					#endif
				}
				atmRxCcDescrp->rdes1 = (96 << 16);
				atmRxCcDescrp->rdes0 |= TSARM_RX_DESCR_VALID;
				if(atmRxCcDescrp->rdes0 & TSARM_RX_DESCR_EOR){
#ifdef DYNAMIC_ALLOC_DMA_BUF
					atmRxCcDescrPoolp = (atmRxCcDescrPool_t *)CACHE_TO_NONCACHE(atmRxCcDescrPool);
#else
					atmRxCcDescrPoolp = (atmRxCcDescrPool_t *)CACHE_TO_NONCACHE(&atmRxCcDescrPool);
#endif
					atmRxCcBusyDescrp = atmRxCcDescrPoolp->rxCcDescrPool;
				}else{
					atmRxCcBusyDescrp++;
				}
			} else {				/* OAM ...for L2-> vc = 0x0a means oam or cc cell. */
				atmRxCcDescrp = atmRxOamBusyDescrp[vc];
				if ( !(atmRxCcDescrp->rdes0 & TSARM_RX_DESCR_VALID) ) {
					#ifndef SAR_VERIFY
					atmOamHandler(atmRxCcDescrp, vc);
					#else
					atmOamLpbkHandler(atmRxCcDescrp, vc);
					#endif
				}
				atmRxCcDescrp->rdes1 = (48 << 16);
				atmRxCcDescrp->rdes0 |= TSARM_RX_DESCR_VALID;
				if(atmRxCcDescrp->rdes0 & TSARM_RX_DESCR_EOR){
					atmRxDescrVcPoolp = (atmRxDescrPool_t *)CACHE_TO_NONCACHE(atmRxDescrPool + vc);
					atmRxOamBusyDescrp[vc] = atmRxDescrVcPoolp->rxOamDescrPool;
				}else{
					atmRxOamBusyDescrp[vc]++;
				}
			}
		} else if (atmRxIrqReport == LIRQ_RX_INACT_VC) {
			/* inactive cell, just need to remove irq */
			if (sarDebugFlag)
				dbg_plinel_1("\r\n atmRxIrqReport=", atmRxIrqReport);
		}
		else if(atmRxIrqReport & LIRQ_RX_M_BOV){
			if (vc == 0x0a) {		/* CC ...for L2-> vc = 0x0a means oam or cc cell.*/
				atmRxCcDescrp = atmRxCcBusyDescrp;
		 		atmRxCcDescrp->rdes1 = (96 << 16);
				atmRxCcDescrp->rdes0 |= TSARM_RX_DESCR_VALID;
				if(atmRxCcDescrp->rdes0 & TSARM_RX_DESCR_EOR){
#ifdef DYNAMIC_ALLOC_DMA_BUF
					atmRxCcDescrPoolp = (atmRxCcDescrPool_t *)CACHE_TO_NONCACHE(atmRxCcDescrPool);
#else
					atmRxCcDescrPoolp = (atmRxCcDescrPool_t *)CACHE_TO_NONCACHE(&atmRxCcDescrPool);
#endif
					atmRxCcBusyDescrp = atmRxCcDescrPoolp->rxCcDescrPool;
				}else{
					atmRxCcBusyDescrp++;
				}
				atm_p->MIB_II.inCcBufOverflow++;
			} else {				/* OAM ...for L2-> vc = 0x0a means oam or cc cell. */
				atmRxCcDescrp = atmRxOamBusyDescrp[vc];
				atmRxCcDescrp->rdes1 = (48 << 16);
				atmRxCcDescrp->rdes0 |= TSARM_RX_DESCR_VALID;
				if(atmRxCcDescrp->rdes0 & TSARM_RX_DESCR_EOR){
					atmRxDescrVcPoolp = (atmRxDescrPool_t *)CACHE_TO_NONCACHE(atmRxDescrPool + vc);
					atmRxOamBusyDescrp[vc] = atmRxDescrVcPoolp->rxOamDescrPool;
				}else{
					atmRxOamBusyDescrp[vc]++;
				}
				atm_p->MIB_II.inMngBufOverflow++;
			}
		}
		#ifdef SAR_VERIFY
		else if(atmRxIrqReport & LIRQ_TX_M_DONE){
		}
		else if(atmRxIrqReport & LIRQ_TX_U_DONE){
		}
		else if(atmRxIrqReport & LIRQ_TX_SW_DIS){
		}
		#endif
		else {
			atmIrqStSaved(atmRxIrqReport, vc);
			dbg_plinel_1("\r\n Invalid atmRxIrqReport=", atmRxIrqReport);
			dbg_plineb_1(" vc=", vc);
			if (sarDebugFlag)
				dbg_plinel_1("\r\n atmRxIrqReport=", atmRxIrqReport);
			atmReset();
			return IRQ_HANDLED;
		}
		
		if(irqrereadflag)
		{
			//clean SDRAM value for check isr event valid
			*(volatile uint32 *) PHYSICAL_TO_K1(TSARM_IRQBA + (atmRxIrqHeadOffset << 2)) = 0;
		}
	
		TSARM_IRQC = 1;
	}
	return IRQ_HANDLED;
}

#ifdef SAR_POLLING
#include <linux/kthread.h>

wait_queue_head_t isrPollingQueue;

irqreturn_t
fakeAtmIsr(
	int irq , void *dev
)
{
	atmIsr( 0, 0);
	if(tcSarIsrPollingMode){
		//mask sar interrupt
		TSARM_GFR &= ~(GFR_GIRQEN);
		isrPollingFlag = 1;
		//printk("enter sar polling mode\n");
		wake_up_interruptible( &isrPollingQueue);
	}
	return IRQ_HANDLED;
}

static int isrPollingThread(void* dummy){
	while(1){
		wait_event_interruptible( isrPollingQueue, isrPollingFlag != 0);
		isrPollingFlag = 0;
		while(1){
			atmIsr( 0, 0);
			if(!tcSarIsrPollingMode){
				printk("leave sar polling mode\n");
				break;
			}
			set_current_state(TASK_INTERRUPTIBLE);
			schedule_timeout(HZ/100);
		}
		//enable sar interrupt
		TSARM_GFR |= GFR_GIRQEN;
	}
	return 0;
}
#endif

/*______________________________________________________________________________
**  atmRegDump
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
int
atmRegDump(
	char *buf,
	uint8 vc
)
{
	uint16	index=0;

	index += sprintf( buf+index, "\n\r***** ATM SAR Module: Common Register Dump *****");
	index += sprintf( buf+index, "\n\rReset&Identify reg   = 0x%08lx, General CFG reg         = 0x%08lx",
		TSARM_RAI, TSARM_GFR);
	index += sprintf( buf+index, "\n\rTraffic Sched. TB reg= 0x%08lx, RX Max Length reg       = 0x%08lx",
		TSARM_TSTBR, TSARM_RMPLR);
	index += sprintf( buf+index, "\n\rP23 Data c /s reg    = 0x%08lx, TX Traffic Sched. LR reg= 0x%08lx",
		TSARM_TXDBCSR_P23, TSARM_TXSLRC);
	index += sprintf( buf+index, "\n\rP01 Data c /s reg    = 0x%08lx, TX OAM ctrl/stat reg    = 0x%08lx",
		TSARM_TXDBCSR_P01, TSARM_TXMBCSR);
	index += sprintf( buf+index, "\n\rRX Data ctrl/stat reg= 0x%08lx, RX OAM ctrl/stat reg    = 0x%08lx",
		TSARM_RXDBCSR, TSARM_RXMBCSR);
	index += sprintf( buf+index, "\n\rLast IRQ Status reg  = 0x%08lx, IRQ Queue Base reg      = 0x%08lx",
		TSARM_LIRQ, TSARM_IRQBA);
	index += sprintf( buf+index, "\n\rIRQ Queue Entry len  = 0x%08lx, IRQ Head ind. reg       = 0x%08lx",
		TSARM_IRQLEN, TSARM_IRQH);
	index += sprintf( buf+index, "\n\r");
	index += sprintf( buf+index, "\n\r***** ATM SAR Module: VC(%02d) Register Dump *****\n\r", vc);
	index += sprintf( buf+index, "\n\rVC IRQ Mask register = 0x%08lx, VC Configuration reg    = 0x%08lx",
		TSARM_IRQM(vc), TSARM_VCCR(vc));
	index += sprintf( buf+index, "\n\rTX Data Current descr= 0x%08lx, TX OAM Current descr    = 0x%08lx",
		TSARM_TXDCBDA(vc), TSARM_TXMCBDA(vc));
	index += sprintf( buf+index, "\n\rRX Data Current descr= 0x%08lx, RX OAM Current descr    = 0x%08lx",
		TSARM_RXDCBDA(vc), TSARM_RXMCBDA(vc));
	index += sprintf( buf+index, "\n\rTX Traffic PCR       = 0x%08lx, TX Traffic SCR          = 0x%08lx",
		TSARM_PCR(vc), TSARM_SCR(vc));
	index += sprintf( buf+index, "\n\rTX Traffic MBS/Type  = 0x%08lx",
		TSARM_MBSTP(vc));
	index += sprintf( buf+index, "\n\rTX Total Data Count  = 0x%08lx, RX Total Data Count     = 0x%08lx",
		TSARM_TDCNT(vc), TSARM_RDCNT(vc));
	index += sprintf( buf+index, "\n\rVC IRQ CC Mask reg   = 0x%08lx, VC CC Config reg        = 0x%08lx",
		TSARM_IRQMCC, TSARM_CCCR);
	index += sprintf( buf+index, "\n\rTX CC Current descr  = 0x%08lx, RX CC Current descr     = 0x%08lx",
		TSARM_CC_TX_BD_BASE, TSARM_CC_RX_BD_BASE);
	index += sprintf( buf+index, "\n\rTX CC Total Count    = 0x%08lx, RX CC Total Count       = 0x%08lx",
		TSARM_TDCNTCC, TSARM_RDCNTCC);
	index += sprintf( buf+index, "\n\rRX Mis-insert Cnt.   = 0x%08lx, TX AAL5 Count           = 0x%08x",
		TSARM_MISCNT, atmTxVcCnt[vc]);
	index += sprintf( buf+index, "\n\r");

	return index;
}

/*______________________________________________________________________________
**  atmCounterDisplay
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
void
atmCounterDisplay(
	void
)
{
	int		index = 0;
	//shnwind modify because 800 bytes  is not enough. 20100608
	//char	stateATM[800];
	char *stateATM;

	stateATM = (char *)kzalloc(2048, GFP_KERNEL);
	index = getATMState( stateATM );

	printf("%s", stateATM);
	kfree(stateATM); 
}


int
getATMState(char *stateATM)
{
	uint16	index=0;

	index += sprintf( stateATM+index, "\n[ SAR Counters ]\n");
	index += sprintf( stateATM+index, "inPkts         = 0x%08lx, inDiscards     = 0x%08lx\n",
		atm_p->MIB_II.inPkts, atm_p->MIB_II.inDiscards);
	index += sprintf( stateATM+index, "inF4Pkts       = 0x%08lx, inF5Pkts       = 0x%08lx\n",
		atm_p->MIB_II.inF4Pkts, atm_p->MIB_II.inF5Pkts);
	index += sprintf( stateATM+index, "inDMATaskEnd   = 0x%08lx, inBufErr       = 0x%08lx\n",
		atm_p->MIB_II.inDMATaskEnd, atm_p->MIB_II.inBufErr);
	index += sprintf( stateATM+index, "inCrcErr       = 0x%08lx\n", atm_p->MIB_II.inCrcErr);
	index += sprintf( stateATM+index, "inBufOverflow  = 0x%08lx, inBufMaxLenErr = 0x%08lx\n",
		atm_p->MIB_II.inBufOverflow, atm_p->MIB_II.inBufMaxLenErr);
	index += sprintf( stateATM+index, "inBufLenErr    = 0x%08lx, inBufDescrOV   = 0x%08lx\n",
		atm_p->MIB_II.inBufLenErr, atm_p->MIB_II.inBufDescrOverflow);
	index += sprintf( stateATM+index, "outPkts        = 0x%08lx, outDiscards    = 0x%08lx\n",
		atm_p->MIB_II.outPkts, atm_p->MIB_II.outDiscards);
	index += sprintf( stateATM+index, "outF4Pkts      = 0x%08lx, outF5Pkts      = 0x%08lx\n",
		atm_p->MIB_II.outF4Pkts, atm_p->MIB_II.outF5Pkts);
	index += sprintf( stateATM+index, "softRstCnt     = 0x%08lx\n", atm_p->MIB_II.softRstCnt);
	index += sprintf( stateATM+index, "inCrc10Err     = 0x%08lx\n", atm_p->MIB_II.inCrc10Err);
	index += sprintf( stateATM+index, "inMngBufOV     = 0x%08lx, inCcBufOV      = 0x%08lx\n",
		atm_p->MIB_II.inMngBufOverflow, atm_p->MIB_II.inCcBufOverflow);
	index += sprintf( stateATM+index, "inMngBufDescrOV= 0x%08lx, inCcBufDescrOV = 0x%08lx\n",
		atm_p->MIB_II.inMngBufDescrOverflow, atm_p->MIB_II.inCcBufDescrOverflow);
	index += sprintf( stateATM+index, "inMpoaErr      = 0x%08lx, inVlanHit      = 0x%08lx\n",
		atm_p->MIB_II.inMpoaErr, atm_p->MIB_II.inVlanHit);
	index += sprintf( stateATM+index, "inCcUDoneErr   = 0x%08lx, inCcUBDOV      = 0x%08lx\n",
		atm_p->MIB_II.inCcUDoneErr, atm_p->MIB_II.inCcUBDOV);

	index += sprintf( stateATM+index, "inBytes        = 0x%08lx, outBytes       = 0x%08lx\n",
		atm_p->MIB_II.inBytes, atm_p->MIB_II.outBytes);//yzwang_20091125
	index += sprintf( stateATM+index, "irqFull        = 0x%08lx\n",
		atm_p->MIB_II.irqFull);

	if(irqrereadflag){
		index += sprintf( stateATM+index, "irqrereadmax = %u, irqrereadtime = %u\n",irqrereadmax, irqrereadtime);
	}
	
	return index;
}


/*______________________________________________________________________________
**  atmCounterClear
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
void
atmCounterClear(
	void
)
{
	memset((char *)&(atm_p->MIB_II), 0, sizeof(atmMIB_II_t));
}
/*______________________________________________________________________________
**  atmPktsClear
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
void
atmPktsClear(
	void
)
{	atm_p->MIB_II.inPkts = 0;
	atm_p->MIB_II.outPkts = 0;
}
int
getATMQueState(char *stateATM)
{
	uint16	index=0;
	uint8 vc;
	int i;

   	index += sprintf( stateATM+index, "ATM SAR Queue Information:\n\n");
   	for (vc = 0; vc < ATM_VC_MAX; vc++) {
		index += sprintf( stateATM+index,
			"VC%d %d/%d: open=%d txVcCnt=%d P0=%d/%d, P1=%d/%d, P2=%d/%d, P3=%d/%d\n",
			vc, atmCfgTable.vpi[vc], atmCfgTable.vci[vc], atmCfgTable.openFlag[vc],
			atmTxVcCnt[vc],
			atmTxPriVcCnt[0][vc], atmTxQueSize[0],
			atmTxPriVcCnt[1][vc], atmTxQueSize[1],
			atmTxPriVcCnt[2][vc], atmTxQueSize[2],
			atmTxPriVcCnt[3][vc], atmTxQueSize[3]);
	}

	index += sprintf( stateATM+index, "\nSaved IRQ st: index=%lx\n", savedAtmIrqIndex);
	for (i = 0; i < SAVED_IRQ_MAX_ENTRY; i++) {
		if ((i & 0x3) == 0)
        		index += sprintf( stateATM+index, "\n");
   		index += sprintf( stateATM+index, "%08lx:%02x ",
		savedAtmIrqSt[i].irqst, savedAtmIrqSt[i].vc);
	}
  	index += sprintf( stateATM+index, "\n");

	return index;
}

int
getATMVcTxDescrDump(char *stateATM, uint8 vc)
{
	uint16	index=0;
   	int i;
	atmTxDescr_t *atmTxDescrp;
   	atmTxCcDescr_t *atmTxOamDescrp;
   	uint8 priority;

	index += sprintf( stateATM+index, "VC=%d\n", vc);

	for(priority = 0; priority < ATM_TX_PRIORITY_MAX; priority++){
		index += sprintf( stateATM+index, " --- Tx Priority %d ---\n", priority);
		index += sprintf( stateATM+index, " atmTxVcBusyDescrp = %08lx\n", (uint32) atmTxVcBusyDescrp[priority][vc]);
		index += sprintf( stateATM+index, " atmTxVcFreeDescrp = %08lx\n", (uint32) atmTxVcFreeDescrp[priority][vc]);
		atmTxDescrp = atmTxDescrPoolAddr[vc].pTxDescr[priority];
		for (i=0; i < atmTxQueSize[priority]; i++, atmTxDescrp++){
			index += sprintf( stateATM+index, " --- i=%d ", i);
			index += sprintf( stateATM+index, " atmTxDescrp = %08lx --- \n", (uint32)atmTxDescrp);
			index += sprintf( stateATM+index, "  ->tdes0 = %08lx\n", (uint32) atmTxDescrp->tdes0);
			index += sprintf( stateATM+index, "  ->tdes1 = %08lx\n", (uint32) atmTxDescrp->tdes1);
			index += sprintf( stateATM+index, "  ->tdes2 = %08lx\n", (uint32) atmTxDescrp->tdes2);
			index += sprintf( stateATM+index, "  ->tdes3 = %08lx\n", (uint32) atmTxDescrp->tdes3);
			index += sprintf( stateATM+index, "  ->skb = %08lx\n", (uint32) atmTxDescrp->skb);
		}
	}
	// management
	index += sprintf( stateATM+index, " --- Tx Oam descriptor dump ---\n");
	index += sprintf( stateATM+index, " atmTxOamFreeDescrp = %08lx\n", (uint32) atmTxOamFreeDescrp[vc]);
	atmTxOamDescrp = atmTxDescrPoolAddr[vc].pTxOamDescr;
	for (i=0; i<ATM_TX_CC_DESCR_NUMMAX; i++, atmTxOamDescrp++){
		index += sprintf( stateATM+index, " --- i=%d atmTxOamDescrp = %08lx --- \n", i, (uint32)atmTxOamDescrp);
		index += sprintf( stateATM+index, "  ->tdes0 = %08lx\n", (uint32) atmTxOamDescrp->tdes0);
		index += sprintf( stateATM+index, "  ->tdes1 = %08lx\n", (uint32) atmTxOamDescrp->tdes1);
		index += sprintf( stateATM+index, "  ->tdes2 = %08lx\n", (uint32) atmTxOamDescrp->tdes2);
		index += sprintf( stateATM+index, "  ->tdes3 = %08lx\n", (uint32) atmTxOamDescrp->tdes3);
	}
	return index;
}

int
getATMVcRxDescrDump(char *stateATM, uint8 vc)
{
	uint16	index=0;
	int i;
	atmRxDescrPool_t *atmRxDescrVcPoolp;
	atmRxDescr_t *atmRxDataDescrp;
	atmRxCcDescr_t *atmRxMngDescrp;

	atmRxDescrVcPoolp = (atmRxDescrPool_t *)CACHE_TO_NONCACHE(atmRxDescrPool + vc);

	index += sprintf( stateATM+index, " atmRxDescrPool = %08lx\n", (uint32)atmRxDescrPool);
	index += sprintf( stateATM+index, " atmRxDataDescrVcPoolp = %08lx\n", (uint32)atmRxDescrVcPoolp);

	// data
	index += sprintf( stateATM+index, " --- Rx Data Descriptor dump ---");
	atmRxDataDescrp = atmRxDescrVcPoolp->rxDescrPool;
	for (i=0; i<ATM_RX_VC_DESCR_NUMMAX; i++, atmRxDataDescrp++){
		index += sprintf( stateATM+index, " --- i=%d atmRxDataDescrp = %08lx ---\n", i, (uint32)atmRxDataDescrp);
		index += sprintf( stateATM+index, "  ->rdes0 = %08lx\n", (uint32) atmRxDataDescrp->rdes0);
		index += sprintf( stateATM+index, "  ->rdes1 = %08lx\n", (uint32) atmRxDataDescrp->rdes1);
		index += sprintf( stateATM+index, "  ->rdes2 = %08lx\n", (uint32) atmRxDataDescrp->rdes2);
		index += sprintf( stateATM+index, "  ->rdes3 = %08lx\n", (uint32) atmRxDataDescrp->rdes3);
		index += sprintf( stateATM+index, "  ->skb = %08lx\n", (uint32) atmRxDataDescrp->skb);
	}

	// management
	index += sprintf( stateATM+index, " --- Rx Mng Descriptor dump ---\n");
	atmRxMngDescrp = atmRxDescrVcPoolp->rxOamDescrPool;
	for (i=0; i<ATM_RX_CC_DESCR_NUMMAX; i++, atmRxMngDescrp++){
   		index += sprintf( stateATM+index, " --- i=%d atmRxMngDescrp = %08lx ---\n", i, (uint32)atmRxMngDescrp);
       	index += sprintf( stateATM+index, "  ->rdes0 = %08lx\n", atmRxMngDescrp->rdes0);
		index += sprintf( stateATM+index, "  ->rdes1 = %08lx\n", atmRxMngDescrp->rdes1);
		index += sprintf( stateATM+index, "  ->rdes2 = %08lx\n", atmRxMngDescrp->rdes2);
		index += sprintf( stateATM+index, "  ->rdes3 = %08lx\n", atmRxMngDescrp->rdes3);
	}
	return index;
}

void atmIrqStSaved(uint32 irqst, uint8 vc)
{
	savedAtmIrqSt[savedAtmIrqIndex].irqst = irqst;
	savedAtmIrqSt[savedAtmIrqIndex].vc = vc;
	savedAtmIrqIndex++;
	if (savedAtmIrqIndex >= SAVED_IRQ_MAX_ENTRY)
		savedAtmIrqIndex = 0;
}

int
atmDebugOn(
	void
)
{
	sarDebugFlag = 1;
	printf("debug = on\n");
	return 0;
}

int
atmDebugOff(
	void
)
{
	sarDebugFlag = 0;
	printf("debug = off\n");
	return 0;
}

#ifdef CMD_API
	volatile uint8 recved;
#endif
/*______________________________________________________________________________
**  atmCcHandler
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
uint8
atmCcHandler(
	uint8 *cellp
)
{
	atmOamCell_t *oamCellp;
	uint8 ret = 0;
	uint8 isF4 = 0;
	uint32 i;

	oamCellp = (atmOamCell_t *)(CACHE_TO_NONCACHE(cellp));
	if (sarDebugFlag) {
		dbg_plinew_1("\n\r gfc=", oamCellp->gfc);
		dbg_plinew_1("\n\r vpi=", oamCellp->vpi);
		dbg_plinew_1("\n\r vci=", oamCellp->vci);
		dbg_plinew_1("\n\r pti=", oamCellp->pti);
		dbg_plinew_1("\n\r clp=", oamCellp->clp);
		dbg_plinew_1("\n\r cellType=", oamCellp->oamCellType);
		dbg_plinew_1("\n\r funcType=", oamCellp->oamFuncType);
		dbg_pline_1("\n\r payload=");
		for (i = 0; i < 45; i++) {
			if ((i & 0x7) == 0)
				dbg_pline_1("\n\r");
			dbg_plineb_1(" ", oamCellp->payload[i]);
		}
	}
	if ( oamCellp->oamCellType != OAM_FAULT_MANAGEMENT ) {
		ret = 1;
	}
	if ((oamCellp->vci == 3) || (oamCellp->vci == 4)) {
		isF4 = 1;
#ifdef CMD_API
		recved = 1;
#endif
		atm_p->MIB_II.inF4Pkts++;
	}

	switch ( oamCellp->oamFuncType ) {
	case OAM_LOOPBACK:
		/* Loopback Indication field:
		** 1 - source point, 0 - loopback point
		*/
		if ( oamCellp->payload[0] == 0x00 ) {
			ret = 1;
		}
		else {
			oamCellp->payload[0] = 0x00;
		}
		break;
	case OAM_AIS:
		oamCellp->oamFuncType = OAM_RDI;
		break;
	default:
		ret = 1;
		break;
	}
	if ( !ret ) {
		if(!atmCcDataReq((uint8 *)oamCellp)){
			if(isF4){
				atm_p->MIB_II.outF4Pkts++;
			}
			atm_p->MIB_II.outPkts++;
		}
	}

	return ret;
}

/*______________________________________________________________________________
**  atmOamHandler
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
uint8
atmOamHandler(
	atmRxCcDescr_t *atmRxCcDescrp,
	uint8 vc
)
{
	atmOamCellPayload_t *oamCellPayloadp;
	uint8 pti;
	uint8 ret = 0;
	uint8 isF4 = 0;
	uint32 i;

	oamCellPayloadp = (atmOamCellPayload_t *)(CACHE_TO_NONCACHE(PHYSICAL_TO_K1(atmRxCcDescrp->rdes2)));
	pti = (atmRxCcDescrp->rdes1 & 0x70000000) >> 28;
	if (sarDebugFlag) {
		dbg_plinew_1("\n\r vpi=", atmCfgTable.vpi[vc]);
		dbg_plinew_1("\n\r vci=", atmCfgTable.vci[vc]);
		dbg_plinew_1("\n\r pti=", pti);
		dbg_plinew_1("\n\r cellType=", oamCellPayloadp->oamCellType);
		dbg_plinew_1("\n\r funcType=", oamCellPayloadp->oamFuncType);
		dbg_pline_1("\n\r payload=");
		for (i = 0; i < 45; i++) {
			if ((i & 0x7) == 0)
				dbg_pline_1("\n\r");
			dbg_plineb_1(" ", oamCellPayloadp->payload[i]);
		}
	}
	if ( oamCellPayloadp->oamCellType != OAM_FAULT_MANAGEMENT ) {
		ret = 1;
	}
	if ((pti == 4) || (pti == 5)) {
		isF4 = 0;
#ifdef CMD_API
		recved = 1;
#endif
		atm_p->MIB_II.inF5Pkts++;

		//add by brian
		#ifdef CWMP
		if(cwmpflag)
		{
			g_f5loopback_rxtime = jiffies;
			cwmpSavedInF5Pkts++;
		}
		#endif
	}

	switch ( oamCellPayloadp->oamFuncType ) {
	case OAM_LOOPBACK:
		/* Loopback Indication field:
		** 1 - source point, 0 - loopback point
		*/
#ifdef L2_AUTOPVC
		if (g_manageFlag & VC_HUNT_FLAG_MANG_START){
			vcPoolProbeVcPktRx(atmCfgTable.vpi[vc], atmCfgTable.vci[vc]);
		}
#endif
		if ( oamCellPayloadp->payload[0] == 0x00 ) {
			ret = 1;
		}
		else {
			oamCellPayloadp->payload[0] = 0x00;
		}
		break;
	case OAM_AIS:
		oamCellPayloadp->oamFuncType = OAM_RDI;
		break;
	default:
		ret = 1;
		break;
	}
	if ( !ret ) {
		if(!atmOamDataReq((uint8 *)oamCellPayloadp, pti, vc)) {
			if(!isF4){
				atm_p->MIB_II.outF5Pkts++;
			}
			atm_p->MIB_II.outPkts++;
		}
	}

	return ret;
}

/*______________________________________________________________________________
**  atmVcNumberGet
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
uint8
atmVcNumberGet(
	uint8 vpi,
	uint16 vci
)
{
	uint8 vc;

	for ( vc = 0; vc < ATM_VC_MAX; vc++ ) {
		if ( atmCfgTable.openFlag[vc] &&
			 (atmCfgTable.vpi[vc] == vpi) &&
		     (atmCfgTable.vci[vc] == vci) ) {
			return vc;
		}
	}

	return ATM_DUMMY_VC;
}

/*______________________________________________________________________________
**  atmAal5DataInd
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
struct sk_buff  *
atmAal5DataInd(
	struct sk_buff *skb,
	uint8 vc,
	uint32 len
)
{
	struct sk_buff *freeSkb = NULL;
	struct atm_vcc *vcc;

	atm_p->MIB_II.inPkts++;
	atm_p->MIB_II.inBytes += len;//yzwang_20091125
	freeSkb = skbmgr_dev_alloc_skb2k();
	if ( freeSkb ) {
		dma_cache_inv((unsigned long)freeSkb->data, RX_BUF_LEN);
		vcc = atmCfgTable.vcc[vc];
		if (unlikely(vcc == NULL)) {
			dev_kfree_skb_any(skb);
			atm_p->MIB_II.inDiscards++;
			return freeSkb;
		}

		ATM_SKB(skb)->vcc = vcc;
		//skb->stamp = vcc->timestamp = xtime;
		//skb->stamp = xtime;
#ifdef L2_AUTOPVC
		if (g_manageFlag & VC_HUNT_FLAG_MANG_START){
			if (vcPoolProbeVcPktRx(atmCfgTable.vpi[vc], atmCfgTable.vci[vc])) {
				dev_kfree_skb_any(skb);
				return freeSkb;
			}
		}
#endif

		skb_put(skb, len);

		if (atm_charge(vcc, skb->truesize) == 0) {
			dev_kfree_skb_any(skb);
			atm_p->MIB_II.inDiscards++;
			return freeSkb;
		}

#ifdef WAN2LAN
/*****************xyzhu_nj_091104: wan2lan start*************/
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
				if ( memcmp(skb2->data, RFC1483_B_LLC_HDR, 7) == 0 ){
					skb_pull(skb2, sizeof(RFC1483_B_LLC_HDR));
				}
				else if ( memcmp(skb2->data, RFC1483_B_VC_HDR, sizeof(RFC1483_B_VC_HDR)) == 0 ){
					skb_pull(skb2, sizeof(RFC1483_B_VC_HDR));
				}
				skb2->mark |= SKBUF_COPYTOLAN;
				macSend(WAN2LAN_CH_ID,skb2);
			}
		}
/*****************xyzhu_nj_091104: wan2lan end****************/
#endif
		/*pass it up to kernel networking layer and update stats*/
		vcc->push(vcc, skb);

		atomic_inc(&vcc->stats->rx);
	}
	else {
		atm_p->MIB_II.inDiscards++;
		freeSkb = skb;
	}

	return freeSkb;
}

/*______________________________________________________________________________
**  atm
**
**  descriptions:
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**____________________________________________________________________________*/
void
atmReset(
	void
)
{
	uint32 i;
	unsigned long flags;

	if (sarDebugFlag) {
		dbg_pline_1("\r\n atmReset");
	}
#if defined(TCSUPPORT_CT)
#ifdef DYNAMIC_ALLOC_SKB
	g_NoNeedFreeSKB = 0;
#endif
#endif
	#ifndef CONFIG_MIPS_TC3262
	local_irq_save(flags);
	#else
	spin_lock_irqsave(&sarLock, flags);
	#endif

	// disable rx
	TSARM_GFR = 0x0;
	delay1ms(1);

	atm_p->MIB_II.softRstCnt++;
	while ((TSARM_IRQH & 0x00fff000) >> 12)
		TSARM_IRQC = 1;
	sarSoftReset = 1;
	memcpy( (uint8*)&atmCfgBackup, (uint8*)&atmCfgTable ,sizeof(atmConfig_t));	/* backup control table */
	for( i = 0; i < atmCfgBackup.vcNumber; i++ ){	/* open vc according to backup table*/
		atmAal5VcClose( atmCfgTable.vpi[i], atmCfgTable.vci[i]);
	}
	sarDrvDescripReset(0);
	memset( (uint8*)&atmCfgTable, 0, sizeof(atmConfig_t));
#ifdef DYNAMIC_ALLOC_DMA_BUF
	memset( (uint8*)atmTxCcDescrPool, 0 ,sizeof(atmTxCcDescrPool_t));
	memset( (uint8*)atmRxCcDescrPool, 0, sizeof(atmRxCcDescrPool_t));
	memset( (uint8*)ccBuf, 0, sizeof(atmRxCcCell_t)*ATM_RX_CC_DESCR_NUMMAX);
	memset( (uint8*)oamBuf, 0, sizeof(atmRxOamDescrPool_t)*ATM_VC_MAX);
	memset( atmTxDescrPool, 0, atmTxDescrPoolSize);
	//memset( (uint8*)atmRxDescrPool, 0 ,sizeof(atmRxDescrPool_t)*ATM_VC_MAX);
#else
	memset( (uint8*)&atmTxCcDescrPool, 0 ,sizeof(atmTxCcDescrPool_t));
	memset( (uint8*)&atmRxCcDescrPool, 0, sizeof(atmRxCcDescrPool_t));
	memset( (uint8*)&ccBuf, 0, sizeof(atmRxCcCell_t)*ATM_RX_CC_DESCR_NUMMAX);
	memset( (uint8*)&oamBuf, 0, sizeof(atmRxOamDescrPool_t)*ATM_VC_MAX);
	memset( (uint8*)&atmTxDescrPool, 0 ,sizeof(atmTxDescrPool_t)*ATM_VC_MAX);
	//memset( (uint8*)&atmRxDescrPool, 0 ,sizeof(atmRxDescrPool_t)*ATM_VC_MAX);
#endif

	atmInit();						/* reset sar & init descriptor */
	for( i = 0; i < atmCfgBackup.vcNumber; i++ ){	/* open vc according to backup table*/
		atmAal5VcOpen( atmCfgBackup.vpi[i], atmCfgBackup.vci[i], &qosRecord[i], atmCfgBackup.vcc[i]);
	}

#if defined(TCSUPPORT_CT)
#ifdef DYNAMIC_ALLOC_SKB
	g_NoNeedFreeSKB = 0;
#endif
#endif
	#ifndef CONFIG_MIPS_TC3262
	local_irq_restore(flags);
	#else
	spin_unlock_irqrestore(&sarLock, flags);
	#endif
}


void
sarDrvDescripReset(
	int freeRxSkb
)
{
	uint8 vc;
	uint8 descrId;
	atmTxDescr_t *atmTxDescrp;
	atmRxDescrPool_t *atmRxDescrVcPoolp;
	atmRxDescr_t *atmRxDescrp;
	struct sk_buff *skb;
	uint32 rxData_offset;
	uint8 priority;

	if (freeRxSkb) {
		for ( vc = 0; vc < ATM_VC_MAX; vc++ ) {
			atmRxDescrVcPoolp = (atmRxDescrPool_t *)CACHE_TO_NONCACHE(atmRxDescrPool + vc);
			rxData_offset = (uint32)atmRxDescrVcPoolp->rxDescrPool - (uint32)atmRxDescrVcPoolp;
			TSARM_RXDCBDA(vc)=K1_TO_PHYSICAL(atmRxDescrVcPoolp);
			TSARM_VC_RX_DATA(vc) = K1_TO_PHYSICAL(rxData_offset);
			atmRxVcDescrp[vc] = atmRxDescrVcPoolp->rxDescrPool;
			for ( descrId = 0; descrId < ATM_RX_VC_DESCR_NUMMAX; descrId++ ) {
				atmRxDescrp = atmRxDescrVcPoolp->rxDescrPool+ descrId;
				skb = atmRxDescrp->skb;
				if( skb != NULL ){
					dev_kfree_skb_any(skb);
					atmRxDescrp->skb = NULL;
				}
			}
		}
	}

	for ( vc = 0; vc < ATM_VC_MAX; vc++ ) {
		// Clear tx packet count and initialize free pointer per vc
		//Set up VC(n)'s current tx descr pointer
		for (priority = 0; priority < ATM_TX_PRIORITY_MAX; priority++){
			for (descrId = 0; descrId < atmTxQueSize[priority]; descrId++ ) {
				atmTxDescrp = atmTxDescrPoolAddr[vc].pTxDescr[priority] + descrId;
				skb = atmTxDescrp->skb;
				if( skb != NULL ){
					dev_kfree_skb_any(skb);
					atmTxDescrp->skb = NULL;
				}
			}
		}
	}
}

/************************************************************************
*		     A T M    D E V I C E   O P S  D E F I N I T I O N S
*************************************************************************
*/
static int tc3162_atm_open(struct atm_vcc *vcc)
{
	qosProfile_t qos;

	set_bit(ATM_VF_ADDR, &vcc->flags);	/* claim address	*/
	vcc->itf = vcc->dev->number;		/* interface number */

	qos.pcr = 0;
	qos.scr = 0;
	qos.mbs = 0;
	qos.type = UBR;

	switch (vcc->qos.txtp.traffic_class) {
	case ATM_CBR:
		qos.type = CBR;
		qos.pcr = vcc->qos.txtp.pcr;
		break;
  	case ATM_UBR:
		qos.type = UBR;
		qos.pcr = vcc->qos.txtp.pcr;
		break;
  	case ATM_VBR:
		qos.type = VBR;
		qos.pcr = vcc->qos.txtp.pcr;
		qos.scr = vcc->qos.txtp.scr;
		qos.mbs = vcc->qos.txtp.mbs;
		break;
	case ATM_nrtVBR:
		qos.type = nrtVBR;
		qos.pcr = vcc->qos.txtp.pcr;
		qos.scr = vcc->qos.txtp.scr;
		qos.mbs = vcc->qos.txtp.mbs;
		break;
	default:
		/* default is UBR */
		break;
	}

	if(qos.pcr)//the pcr value has been modified by client
	{
		pcr_modify_flag = 1;
		g_qos_pcr = qos.pcr;//save the pcr's value
	}	
//	printk("tc3162_atm_open vpi=%d vci=%d\n", vcc->vpi, vcc->vci);
//	printk("tc3162_atm_open qos type=%d pcr=%d scr=%d mbs=%d\n", qos.type, qos.pcr, qos.scr, qos.mbs);
	if (atmAal5VcOpen(vcc->vpi, vcc->vci, &qos, vcc))
    	return -EBUSY;

	vcc->sk.sk_sndbuf = 0x7fffffff;
 	vcc->sk.sk_rcvbuf = 0x7fffffff;

	set_bit(ATM_VF_READY, &vcc->flags);

	//printk("tc3162_atm_open vpi=%d vci=%d done\n", vpi, vci);
	return 0;
}

static void tc3162_atm_close(struct atm_vcc *vcc)
{
	//printk("tc3162_atm_close vpi=%d vci=%d\n", vcc->vpi, vcc->vci);

	clear_bit(ATM_VF_ADDR, &vcc->flags);
	atmAal5VcClose(vcc->vpi, vcc->vci);
	clear_bit(ATM_VF_READY, &vcc->flags);
#if defined(TCSUPPORT_PPPOA_ENHANCE)	
	atomic_set(&sk_atm(vcc)->sk_wmem_alloc, 1);
#endif			

	//printk("tc3162_atm_close done\n");
}

static int tc3162_atm_ioctl(struct atm_dev *dev, unsigned int cmd, void *arg)
{
#if 0
	printk("tc3162_atm_ioctl\n");
	return 0;
#else
#ifdef CONFIG_VLAN_ATM
	tsarm_ioctl_t sar_ioctl;
	switch(cmd) {
		case SET_ATM_VLAN:
			if ( copy_from_user(&sar_ioctl, (tsarm_ioctl_t*)arg, sizeof(tsarm_ioctl_t)) ) {
				return -EFAULT;
			}
			if ( setATMVlan(&sar_ioctl) == -1 ) {
				return -EFAULT;
			}
			break;
		case DEL_ATM_PVC:
		default:
			break;
	}
	return 0;
#endif
#endif
}
#if !defined(TCSUPPORT_CT)
#define CONFIG_8021P_REMARK 1
#ifdef CONFIG_8021P_REMARK
static inline struct sk_buff* vlanPriRemark(struct sk_buff *skb)
{
	uint8 encap_mode = 0, encap_len = 0;
	char * vlan_p = NULL, *ether_type_ptr = NULL;
	unsigned char ucprio = 0;
	unsigned char uc802prio = 0;
	uint16 vid=0;
	int copy_len = 0;

	if ( skb->mark & QOS_8021p_MARK ) {
		/*vlan tagging*/
		encap_mode = getEncapMode((uint8*)skb->data);
		/*Note Ethernet Header*/
		if ( (encap_mode == RFC1483_B_VC)
			|| (encap_mode == PPPoA_LLC)
			|| (encap_mode == PPPoA_VC) ) {
				/*Nono ethernet header to do nothings*/
				return skb;	
			}
		
		encap_len = atmEncapLen[encap_mode];
		ether_type_ptr = skb->data + encap_len + 12;
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
				struct sk_buff *sk_tmp = skb;
				skb = skb_realloc_headroom(sk_tmp, VLAN_HLEN);
				
				if ( ATM_SKB(sk_tmp)->vcc->pop ) {
					ATM_SKB(sk_tmp)->vcc->pop(ATM_SKB(sk_tmp)->vcc, sk_tmp);
				}
				else {
					dev_kfree_skb_any(sk_tmp);
				}
				
				if ( !skb ) {
					printk(KERN_ERR, "Vlan:failed to realloc headroom\n");
					return NULL;
				}
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
		
			copy_len = encap_len + 2*VLAN_ETH_ALEN;
			/*move the mac address to the beginning of new header*/
			memmove(skb->data, skb->data+VLAN_HLEN, copy_len);
		}
	
		vlan_p = skb->data + encap_len + 12;
		*(unsigned short *)vlan_p = 0x8100;
		
		vlan_p += 2;
		*(unsigned short *)vlan_p = 0;
		/*3 bits priority and vid vlaue*/
		*(unsigned short*)vlan_p |= (((uc802prio & 0x7) << 13)|vid) ;
		skb->network_header -= VLAN_HLEN;
		skb->mac_header -= VLAN_HLEN;
	}
	return skb;
}
#endif /*CONFIG_8021P_REMARK*/
#endif

static int tc3162_atm_send(struct atm_vcc *vcc, struct sk_buff *skb)
{
	int ret;
	uint32 flags;
#ifdef CONFIG_VLAN_ATM
	int vlan_vc;
	vlan_vc = getIndexVlanVcc( vcc->vpi, vcc->vci );
	if ( vlan_vc != ATM_DUMMY_VC ) {
		if ( vlan_vcc[vlan_vc].active == 1 ) {
			//printk("insert vlan tag here.\n");
			skb = insert_vtag( skb, vlan_vc );
			if ( skb == NULL ) {
				printk("just return for skb is NULL.\n");
				return 1;
			}
		}
	}
#if !defined(TCSUPPORT_CT)
	#ifdef CONFIG_8021P_REMARK
	else{
		skb=vlanPriRemark(skb);
		if(skb==NULL){
			printk("802.1p remark failure\r\n");
			return 1;
		}
	}
	#endif	
#endif
#endif

#ifdef WAN2LAN
/*****************xyzhu_nj_091104: wan2lan start*************/
	if(masko_on_off){
		struct sk_buff *skb2 = NULL;

		if(skb_headroom(skb) < TX_STAG_LEN){
			skb2 = skb_copy_expand(skb, TX_STAG_LEN, skb_tailroom(skb), GFP_ATOMIC);
		}else{
		    skb2 = skb_copy(skb,GFP_ATOMIC);
		}

		if(skb2 == NULL){
			printk("wan2lan failure in sar tx direction for skb2 allocate failure.\n");
		}
		else{
			if ( memcmp(skb2->data, RFC1483_B_LLC_HDR, 7) == 0 ){
				skb_pull(skb2, sizeof(RFC1483_B_LLC_HDR));
			}
			else if ( memcmp(skb2->data, RFC1483_B_VC_HDR, sizeof(RFC1483_B_VC_HDR)) == 0 ){
				skb_pull(skb2, sizeof(RFC1483_B_VC_HDR));
			}
			skb2->mark |= SKBUF_COPYTOLAN;
			//Bug 8620. shnwind 20100121
			macSend(WAN2LAN_CH_ID,skb2);
		}
	}
/*****************xyzhu_nj_091104: wan2lan end****************/
#endif

#ifdef CONFIG_NET_SCHED  /*Rodney_20091115*/
	if((qos_qdisc_mode == QOS_QDISC_RED) || (qos_qdisc_mode == QOS_QDISC_WRED)){
		if(vcc->_dev != NULL)
			netif_stop_queue(vcc->_dev);
	}		
#endif

	#ifndef CONFIG_MIPS_TC3262
	local_irq_save(flags);
	#else
	spin_lock_irqsave(&sarLock, flags);
	#endif
	ATM_SKB(skb)->vcc = vcc;
	ret = atmAal5DataReq(skb, vcc->vpi, vcc->vci);
	if (!ret)
		atomic_inc(&vcc->stats->tx);

	#ifndef CONFIG_MIPS_TC3262
	local_irq_restore(flags);
	#else
	spin_unlock_irqrestore(&sarLock, flags);
	#endif
	return ret;
}

static int tc3162_atm_change_qos (struct atm_vcc *vcc, struct atm_qos *qos,int flags)
{
	printk("tc3162_atm_change_qos\n");
	return 0;
}

/************************************************************************
*		     A T M    D E V I C E   P R O C  D E F I N I T I O N S
*************************************************************************
*/

int proc_vc[ATM_VC_MAX];
char proc_vc_name[ATM_VC_MAX][32];

static int tsarm_stats_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getATMState(buf);
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

static int tsarm_stats_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32];

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	memset((char *)&(atm_p->MIB_II), 0, sizeof(atmMIB_II_t));

	return count;
}

static int tsarm_questats_read_proc(char *buf, char **start, off_t off, int count,
	int *eof, void *data)
{
   	int len = getATMQueState(buf);
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

static int tsarm_proc_reg(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len;
	int *vc;

	vc = (int *) data;
	len = atmRegDump(buf, *vc);
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

static uint32 savedInF5Pkts = 0;
static uint32 savedInF4Pkts = 0;
#ifdef CMD_API
static uint32 oam_success_cnt, oam_fail_cnt;
static uint32 res_time_sum = 0, res_time_min = 0xFFFFFFFF, res_time_max = 0;
static uint32 rep_num, f5;
#endif

static int oam_ping_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;
	int result;

#ifdef CMD_API
	if(f5){
		oam_success_cnt = atm_p->MIB_II.inF5Pkts - savedInF5Pkts;
		oam_fail_cnt = rep_num - atm_p->MIB_II.inF5Pkts + savedInF5Pkts;
	}
	else{
		oam_success_cnt = atm_p->MIB_II.inF4Pkts - savedInF4Pkts;
		oam_fail_cnt = rep_num - atm_p->MIB_II.inF4Pkts + savedInF4Pkts;
	}
	savedInF5Pkts = atm_p->MIB_II.inF5Pkts;
	savedInF4Pkts = atm_p->MIB_II.inF4Pkts;
	if(oam_success_cnt)
		do_div(res_time_sum, oam_success_cnt);
	else
		res_time_sum = 0;
	len = sprintf(page, "OAM- Test Results\r\nSuccessCount:%ld FailureCount:%ld AverageResponseTime:%lu MinimumResponseTime:%lu MaximumResponseTime:%lu\r\n", oam_success_cnt, oam_fail_cnt, res_time_sum, res_time_min, res_time_max);
	res_time_sum = 0;
	res_time_min = 0xFFFFFFFF;
	res_time_max = 0;
	rep_num = 0;
#else
	result = (savedInF5Pkts != atm_p->MIB_II.inF5Pkts) || (savedInF4Pkts != atm_p->MIB_II.inF4Pkts);
	if (result) {
		savedInF5Pkts = atm_p->MIB_II.inF5Pkts;
		savedInF4Pkts = atm_p->MIB_II.inF4Pkts;
	}

	len = sprintf(page, "%d\n", result);

#endif
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

static int oam_ping_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32];
	uint32 vpi;
	uint32 vci;
#ifndef CMD_API
	uint32 f5;
#endif
	uint32 endToEnd;
	uint32 funcType;
#ifdef CMD_API
	uint32 ms, timeout;
	uint32 i;
	volatile uint32 timer_now, timer_last;
	volatile uint32 tick_acc;
	uint32 one_tick_unit = 1 * SYS_HCLK * 1000 / 2;
	volatile uint32 timer1_ldv = VPint(CR_TIMER1_LDV);
	volatile uint32 tick_wait;
#endif

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

#ifdef CMD_API
	if (sscanf(val_string, "%lu %lu %lu %lu %lu %lu", &vpi, &vci, &f5, &endToEnd, &funcType, &timeout) != 6) {
	    printk("usage: <vpi> <vci> <0(f4) 1(f5)> <0(segment) 1(end-to-end)> <type:0(AIS) 1(RDI) 2(Loopback)> <timeout>\n");
	    return count;
	}
	rep_num++;
	tick_acc = 0;
	recved = 0;
	timer_last = VPint(CR_TIMER1_VLR);
#else
	if (sscanf(val_string, "%lu %lu %lu %lu %lu", &vpi, &vci, &f5, &endToEnd, &funcType) != 5) {
		printk("usage: <vpi> <vci> <f5> <end-to-end> <type:0(AIS) 1(RDI) 2(Loopback)>\n");
		return count;
	}

	savedInF5Pkts = atm_p->MIB_II.inF5Pkts;
	savedInF4Pkts = atm_p->MIB_II.inF4Pkts;

#endif
	atmOamF4F5DataReq((uint8 )vpi, (uint16 )vci, f5, endToEnd, funcType);
#ifdef CMD_API
	tick_wait=timeout*one_tick_unit;
	while (!recved){
	//	msleep(10);
		timer_now = VPint(CR_TIMER1_VLR);
		if (timer_last >= timer_now)
			tick_acc += timer_last - timer_now;
		else
	    	tick_acc += timer1_ldv - timer_now + timer_last;
		timer_last = timer_now;
		ms = tick_acc;
	//	do_div(ms, one_tick_unit);
		if((ms >= tick_wait)||recved)
			break;
	}
	ms = tick_acc;
   	do_div(ms, one_tick_unit);
   	res_time_sum += ms;
	if(res_time_min > ms)
   		res_time_min = ms;
	if(res_time_max < ms)
		res_time_max = ms;
#endif

	return count;
}

//add by brian for atm f5 loopback diagnostic
#ifdef CWMP
static int loopback_diagnostic_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int index = 0;
	char *loopbackbuf = NULL;
	uint32 diagnostictime;
	char result[][32] = {"Complete","Error_internal","InProgress","Error_other"};
	enum result_index
	{
		Complete = 0,
		Internal,
		InProgress,
		Other
	};

	loopbackbuf = page;
	if(internal_error)
	{
		index += sprintf(loopbackbuf+index, "DiagnosticState=%s\n", result[Internal]);
		index += sprintf(loopbackbuf+index, "DiagnosticTime=0\n");
		internal_error = 0;
	}
	else
	{
		if((cwmpSavedInF5Pkts != 0) && (cwmpSavedOutF5Pkts <= cwmpSavedInF5Pkts))//diagnostic ok
		{
			if(g_f5loopback_rxtime > g_f5loopback_txtime)
				diagnostictime = (g_f5loopback_rxtime - g_f5loopback_txtime)*1000 / HZ;
			else
				diagnostictime = (maxtimeindex + g_f5loopback_rxtime - g_f5loopback_txtime)*1000 / HZ;

			index += sprintf(loopbackbuf+index, "DiagnosticState=%s\n", result[Complete]);
			index += sprintf(loopbackbuf+index, "DiagnosticTime=%lu\n", diagnostictime);
		}
		else
		{
			index += sprintf(loopbackbuf+index, "DiagnosticState=%s\n", result[InProgress]);
			index += sprintf(loopbackbuf+index, "DiagnosticTime=0\n");
		}
	}

	index -= off;
	*start = page + off;

	if (index > count)
		index = count;
	else
		*eof = 1;

	if (index < 0)
		index = 0;

	return index;

}

static int loopback_diagnostic_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	uint32 f5;
	uint32 endToEnd;
	uint32 funcType;
	uint32 flag;
	uint32 vpi = 0;
	uint32 vci = 0;
	char val_string[32];

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	if (sscanf(val_string, "%lu %lu %lu %lu %lu %lu", &vpi, &vci, &f5, &endToEnd, &funcType,&flag) != 6) {
		printk("usage: <wanindex> <f5> <end-to-end> <type:0(AIS) 1(RDI) 2(Loopback)> <flag>\n");
		return count;
	}

	printk("\r\nthe value is %lu,%lu,%lu,%lu,%lu,%lu",vpi,vci,f5,endToEnd,funcType,flag);
	//the glag used for cwmp diagnostic
	cwmpflag = flag;

	//compute the time
	g_f5loopback_rxtime = 0;
	g_f5loopback_txtime = jiffies;

	//reset the pkt number
	cwmpSavedInF5Pkts = 0;
	cwmpSavedOutF5Pkts = 0;

	//do loopback diagnostic
	if(atmOamF4F5DataReq((uint8 )vpi, (uint16 )vci, f5, endToEnd, funcType))
	{
		//internal error,so wu just set the variable
		internal_error = 1;
	}

	return count;
}

static int loopback_diagnostic_reset_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	return 0;
}

static int loopback_diagnostic_reset_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	int resetflag = 0;
	char val_string[32];

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	if (sscanf(val_string, "%d", &resetflag) != 1) {
		printk("usage: <flag>\n");
		return count;
	}

	if(resetflag)
	{
		cwmpSavedInF5Pkts = 0;
		cwmpSavedOutF5Pkts = 0;
		g_f5loopback_rxtime = 0;
		g_f5loopback_txtime = 0;
		cwmpflag = 0;//reset the cwmpflag,so no need to compute the in or out pkt number
		internal_error = 0;
		resetflag = 0;
	}

	return count;
}
#endif
static int tsarm_debug_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", sarDebugFlag);

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

static int tsarm_debug_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[8];
	unsigned long val;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	val = simple_strtoul(val_string, NULL, 10);
	if (val != 0)
		sarDebugFlag = 1;
	else
		sarDebugFlag = 0;

	return count;
}

static uint32 vcTxDescrDump = 0;
static uint32 vcRxDescrDump = 0;

#define CHK_BUF() pos = begin + index; if (pos < off) { index = 0; begin = pos; }; if (pos > off + count) goto done;

static int tsarm_txdescrdump_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index=0;
	off_t pos = 0;
	off_t begin = 0;
	int i;
	atmTxDescr_t *atmTxDescrp;
	atmTxCcDescr_t *atmTxOamDescrp;
   	uint8 priority;
	int vc;
	char *stateATM = buf;

	vc = vcTxDescrDump;
	index += sprintf( stateATM+index, "VC=%d\n", vc);
	CHK_BUF();

 	for(priority = 0; priority < ATM_TX_PRIORITY_MAX; priority++){
   		index += sprintf( stateATM+index, " --- Tx Priority %d ---\n", priority);
		CHK_BUF();
       	index += sprintf( stateATM+index, " atmTxVcBusyDescrp = %08lx\n", (uint32) atmTxVcBusyDescrp[priority][vc]);
		CHK_BUF();
       	index += sprintf( stateATM+index, " atmTxVcFreeDescrp = %08lx\n", (uint32) atmTxVcFreeDescrp[priority][vc]);
		CHK_BUF();
		atmTxDescrp = atmTxDescrPoolAddr[vc].pTxDescr[priority];
       	for (i=0; i < atmTxQueSize[priority]; i++, atmTxDescrp++){
       		index += sprintf( stateATM+index, " --- i=%d ", i);
			CHK_BUF();
           	index += sprintf( stateATM+index, " atmTxDescrp = %08lx --- \n", (uint32)atmTxDescrp);
			CHK_BUF();
           	index += sprintf( stateATM+index, "  ->tdes0 = %08lx\n", (uint32) atmTxDescrp->tdes0);
			CHK_BUF();
           	index += sprintf( stateATM+index, "  ->tdes1 = %08lx\n", (uint32) atmTxDescrp->tdes1);
			CHK_BUF();
           	index += sprintf( stateATM+index, "  ->tdes2 = %08lx\n", (uint32) atmTxDescrp->tdes2);
			CHK_BUF();
           	index += sprintf( stateATM+index, "  ->tdes3 = %08lx\n", (uint32) atmTxDescrp->tdes3);
			CHK_BUF();
           	index += sprintf( stateATM+index, "  ->skb = %08lx\n", (uint32) atmTxDescrp->skb);
			CHK_BUF();
		}
	}
	// management
   	index += sprintf( stateATM+index, " --- Tx Oam descriptor dump ---\n");
	CHK_BUF();
   	index += sprintf( stateATM+index, " atmTxOamFreeDescrp = %08lx\n", (uint32) atmTxOamFreeDescrp[vc]);
	CHK_BUF();
	atmTxOamDescrp = atmTxDescrPoolAddr[vc].pTxOamDescr;
   	for (i=0; i<ATM_TX_CC_DESCR_NUMMAX; i++, atmTxOamDescrp++){
   		index += sprintf( stateATM+index, " --- i=%d atmTxOamDescrp = %08lx --- \n", i, (uint32)atmTxOamDescrp);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->tdes0 = %08lx\n", (uint32) atmTxOamDescrp->tdes0);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->tdes1 = %08lx\n", (uint32) atmTxOamDescrp->tdes1);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->tdes2 = %08lx\n", (uint32) atmTxOamDescrp->tdes2);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->tdes3 = %08lx\n", (uint32) atmTxOamDescrp->tdes3);
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

static int tsarm_txdescrdump_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32];

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	vcTxDescrDump = simple_strtoul(val_string, NULL, 10);

	return count;
}

static int tsarm_rxdescrdump_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index=0;
	off_t pos = 0;
	off_t begin = 0;
	int i;
   	atmRxDescrPool_t *atmRxDescrVcPoolp;
   	atmRxDescr_t *atmRxDataDescrp;
   	atmRxCcDescr_t *atmRxMngDescrp;
	uint32 vc;
	char *stateATM = buf;

	vc = vcTxDescrDump;
   	atmRxDescrVcPoolp = (atmRxDescrPool_t *)CACHE_TO_NONCACHE(atmRxDescrPool + vc);

   	index += sprintf( stateATM+index, " atmRxDescrPool = %08lx\n", (uint32)atmRxDescrPool);
	CHK_BUF();
   	index += sprintf( stateATM+index, " atmRxDataDescrVcPoolp = %08lx\n", (uint32)atmRxDescrVcPoolp);
	CHK_BUF();

   	// data
	index += sprintf( stateATM+index, " --- Rx Data Descriptor dump ---\n");
	CHK_BUF();
   	atmRxDataDescrp = atmRxDescrVcPoolp->rxDescrPool;
   	for (i=0; i<ATM_RX_VC_DESCR_NUMMAX; i++, atmRxDataDescrp++){
   		index += sprintf( stateATM+index, " --- i=%d atmRxDataDescrp = %08lx ---\n", i, (uint32)atmRxDataDescrp);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->rdes0 = %08lx\n", (uint32) atmRxDataDescrp->rdes0);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->rdes1 = %08lx\n", (uint32) atmRxDataDescrp->rdes1);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->rdes2 = %08lx\n", (uint32) atmRxDataDescrp->rdes2);
		CHK_BUF();
      	index += sprintf( stateATM+index, "  ->rdes3 = %08lx\n", (uint32) atmRxDataDescrp->rdes3);
		CHK_BUF();
   		index += sprintf( stateATM+index, "  ->skb = %08lx\n", (uint32) atmRxDataDescrp->skb);
		CHK_BUF();
	}

	// management
   	index += sprintf( stateATM+index, " --- Rx Mng Descriptor dump ---\n");
	CHK_BUF();
   	atmRxMngDescrp = (atmRxCcDescr_t *) atmRxDescrVcPoolp->rxOamDescrPool;
   	for (i=0; i<ATM_RX_CC_DESCR_NUMMAX; i++, atmRxMngDescrp++){
   		index += sprintf( stateATM+index, " --- i=%d atmRxMngDescrp = %08lx ---\n", i, (uint32) atmRxMngDescrp);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->rdes0 = %08lx\n", atmRxMngDescrp->rdes0);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->rdes1 = %08lx\n", atmRxMngDescrp->rdes1);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->rdes2 = %08lx\n", atmRxMngDescrp->rdes2);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->rdes3 = %08lx\n", atmRxMngDescrp->rdes3);
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

static int tsarm_rxdescrdump_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32];

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	vcRxDescrDump = simple_strtoul(val_string, NULL, 10);

	return count;
}

static int tsarm_ccdescrdump_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index=0;
	off_t pos = 0;
	off_t begin = 0;
	int i;
   	atmRxCcDescrPool_t *atmRxCcDescrPoolp;
   	atmRxCcDescr_t *atmRxCcDescrp;
   	atmTxCcDescrPool_t *atmTxCcDescrVcPoolp;
   	atmTxCcDescr_t *atmTxCcDescrp;
	char *stateATM = buf;

#ifdef DYNAMIC_ALLOC_DMA_BUF
   	atmTxCcDescrVcPoolp = (atmTxCcDescrPool_t *)CACHE_TO_NONCACHE(atmTxCcDescrPool);
#else
   	atmTxCcDescrVcPoolp = (atmTxCcDescrPool_t *)CACHE_TO_NONCACHE(&atmTxCcDescrPool);
#endif

#ifdef DYNAMIC_ALLOC_DMA_BUF
   	index += sprintf( stateATM+index, " atmTxCcFreeDescrp = %08lx\n", (uint32)atmTxCcFreeDescrp);
#else
   	index += sprintf( stateATM+index, " atmTxCcFreeDescrp = %08lx\n", (uint32)&atmTxCcFreeDescrp);
#endif
	CHK_BUF();
#ifdef DYNAMIC_ALLOC_DMA_BUF
   	index += sprintf( stateATM+index, " atmTxCcDescrPool = %08lx\n", (uint32)atmTxCcDescrPool);
#else
   	index += sprintf( stateATM+index, " atmTxCcDescrPool = %08lx\n", (uint32)&atmTxCcDescrPool);
#endif
	CHK_BUF();
   	index += sprintf( stateATM+index, " atmTxCcDescrVcPool = %08lx\n", (uint32)atmTxCcDescrVcPoolp);
	CHK_BUF();

   	index += sprintf( stateATM+index, " --- Tx Cc descriptor dump ---\n");
	CHK_BUF();
   	atmTxCcDescrp = atmTxCcDescrVcPoolp->txCcDescrPool;
   	for (i=0; i<ATM_TX_CC_DESCR_NUMMAX; i++, atmTxCcDescrp++){
   		index += sprintf( stateATM+index, " -- i=%d atmTxCcDescrp = %08lx ---\n", i, (uint32)atmTxCcDescrp);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->tdes0 = %08lx\n", atmTxCcDescrp->tdes0);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->tdes1 = %08lx\n", atmTxCcDescrp->tdes1);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->tdes2 = %08lx\n", atmTxCcDescrp->tdes2);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->tdes3 = %08lx\n", atmTxCcDescrp->tdes3);
		CHK_BUF();
	}

#ifdef DYNAMIC_ALLOC_DMA_BUF
   	atmRxCcDescrPoolp = (atmRxCcDescrPool_t *)CACHE_TO_NONCACHE(atmRxCcDescrPool);
#else
   	atmRxCcDescrPoolp = (atmRxCcDescrPool_t *)CACHE_TO_NONCACHE(&atmRxCcDescrPool);
#endif

#ifdef DYNAMIC_ALLOC_DMA_BUF
   	index += sprintf( stateATM+index, " atmRxCcDescrPool = %08lx\n", (uint32)atmRxCcDescrPool);
#else
   	index += sprintf( stateATM+index, " atmRxCcDescrPool = %08lx\n", (uint32)&atmRxCcDescrPool);
#endif

	CHK_BUF();
   	index += sprintf( stateATM+index, " atmRxCcDescrVcPoolp = %08lx\n", (uint32)atmRxCcDescrPoolp);
	CHK_BUF();

   	index += sprintf( stateATM+index, " --- Rx Cc Descriptor dump ---\n");
	CHK_BUF();
   	atmRxCcDescrp = atmRxCcDescrPoolp->rxCcDescrPool;
   	for (i=0; i<ATM_RX_CC_DESCR_NUMMAX; i++, atmRxCcDescrp++){
   		index += sprintf( stateATM+index, " --- i=%d atmRxCcDescrp = %08lx ---\n", i, (uint32)atmRxCcDescrp);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->rdes0 = %08lx\n", atmRxCcDescrp->rdes0);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->rdes1 = %08lx\n", atmRxCcDescrp->rdes1);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->rdes2 = %08lx\n", atmRxCcDescrp->rdes2);
		CHK_BUF();
       	index += sprintf( stateATM+index, "  ->rdes3 = %08lx\n", atmRxCcDescrp->rdes3);
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

static int tsarm_reset_read_proc(char *page, char **start, off_t off,
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

static int tsarm_reset_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[8];

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	atmReset();

	return count;
}
static int tsarm_pktsclear_read_proc(char *page, char **start, off_t off,
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
static int tsarm_pktsclear_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){
	char val_string[8];

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	atmReset();
	atmPktsClear();
	return count;
}

//#ifdef QOS_REMARKING  /*Rodney_20090724*/
#if defined (QOS_REMARKING) || defined (TCSUPPORT_HW_QOS)
static int tsarm_qoswrr_read_proc(char *page, char **start, off_t off,
		int count, int *eof, void *data){

	int len = sprintf(page, "%d %d %d %d %d\r\n",*qos_wrr_info, *(qos_wrr_info + 1), *(qos_wrr_info + 2), *(qos_wrr_info + 3), *(qos_wrr_info + 4));
	return len;
}
static int tsarm_qoswrr_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){
	int len;
	char get_buf[32];
	uint32 priority;
	int max_wrr_val = 0, i;
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
			max_prio = i;
		}
	}
	qos_wrr_user = QOS_DMAWRR_USERDEFINE;
	if(*qos_wrr_info == 0)  /*strict priority*/
		TSARM_GFR &= ~(GFR_DMT_WRR_EN);
	else{  /*WRR*/
		priority = TSARM_DMAWRRCR;
		priority = (priority & 0xf0f0f0f0);
		priority |= (((uint32)(qos_wrr_info[1] & 0x0f) << 0) | ((uint32)(qos_wrr_info[2] & 0x0f) << 8) | ((uint32)(qos_wrr_info[3] & 0x0f) << 16) | ((uint32)(qos_wrr_info[4] & 0x0f) << 24));
		TSARM_DMAWRRCR = priority;
		TSARM_GFR |= GFR_DMT_WRR_EN;
	}
	return len;
}
#endif


#ifdef TCSUPPORT_QOS

static int tsarm_tcqos_read_proc(char *page, char **start, off_t off,
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

static int tsarm_tcqos_write_proc(struct file *file, const char *buffer,
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

#ifdef CMD_API
static int tsarm_pvc_read_proc(char *page, char **start, off_t off,
		        int count, int *eof, void *data){
	int i, len=0;

	for ( i = 0; i < ATM_VC_MAX; i++ ){
		if ( atmCfgTable.openFlag[i] == 0 ) {
			len = sprintf(page, "%d\r\n", i);
			return len;
		}
	}
	return len;
}

static int tsarm_pvc_write_proc(struct file *file, const char *buffer,
		    unsigned long count, void *data){
	char val_string[32];
	uint32 vpi, vci;

	if (count > sizeof(val_string) - 1)
        	return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	if(sscanf(val_string, "%lu %lu", &vpi, &vci) != 2){
		printk("Input format: vpi vci\r\n");
		return -1;
	}
	#if 0
	else{
		int i;

		if(vpi == 0 && vci == 0){
			for(i = 0; i < ATM_VC_MAX; i++){
				printk("aal5 vcc statistics for 0.%d.%d\r\n",
				        atmCfgTable.vpi[i], atmCfgTable.vci[i]);
				printk("crc errors\t\t%d\r\n", pvc_stats[i].crc_errs);
				printk("oversized sdus\t\t%d\r\n", pvc_stats[i].oversized_sdus);
                		printk("short packet errors\t%d\r\n", pvc_stats[i].short_pkt_errs);
                		printk("length errors\t\t%d\r\n\r\n", pvc_stats[i].len_errs);
            		}
        	}
        	else{
            		for(i = 0; i < ATM_VC_MAX; i++){
	            		if(atmCfgTable.vpi[i] == vpi && atmCfgTable.vci[i] == vci){
		            		printk("aal5 vcc statistics for 0.%d.%d\r\n",
		           			atmCfgTable.vpi[i], atmCfgTable.vci[i]);
					printk("crc errors\t\t%d\r\n", pvc_stats[i].crc_errs);
					printk("oversized sdus\t\t%d\r\n", pvc_stats[i].oversized_sdus);
					printk("short packet errors\t%d\r\n", pvc_stats[i].short_pkt_errs);
					printk("length errors\t\t%d\r\n\r\n", pvc_stats[i].len_errs);
				}
			}
		}
	}
	#endif

	return count;
}

int pvc_num, pid_del, pid_add, pid_map[8][3];

static int tsarm_pvc_del_read_proc(char *page, char **start, off_t off,
		                int count, int *eof, void *data){
	int len = sprintf(page, "%d %d\r\n", pvc_num, pid_del);
	return len;
}

static int tsarm_pvc_del_write_proc(struct file *file, const char *buffer,
		            unsigned long count, void *data){
	char val_string[32];
	uint32 vpi, vci, pid;
	int i, mode;

	if (count > sizeof(val_string) - 1)
    		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	mode = sscanf(val_string, "%lu %lu %d", &vpi, &vci, &pid);

	switch(pid){
		case 0://Delete
			for(i = 0; i < 8; i++){
				if(pid_map[i][0] == vpi && pid_map[i][1] == vci){
					pvc_num = i;
					pid_del = pid_map[i][2];
					pid_map[i][0] = 0;
					pid_map[i][1] = 0;
					pid_map[i][2] = 0;
					break;
				}
			}
			if(i == 8){
				pvc_num = -1;
               	pid_del = -1;
			}
			return count;
		case -1://Look-up
			for(i = 0; i < 8; i++){
				if(pid_map[i][0] == vpi && pid_map[i][1] == vci){
					pvc_num = i;
					pid_del = pid_map[i][2];
					break;
				}
			}
			if(i == 8){
				pvc_num = -1;
               	pid_del = -1;
			}
			return count;
			//break;
		default://Add
			//if vpi.vci exist,use the intial pvcnum and pid_del and return without do anything
			for(i = 0; i < 8; i++){
				if(pid_map[i][0] == vpi && pid_map[i][1] == vci){
					pvc_num = i;
					pid_del = pid_map[i][2];
					return count;
					//break;
				}
			}
			for(i = 0; i < 8; i++){
				if(pid_map[i][0] == 0 && pid_map[i][1] == 0){
					pvc_num = i;
					pid_map[i][0] = vpi;
					pid_map[i][1] = vci;
					pid_map[i][2] = pid;
					pid_del = pid;
					break;
				}
			}
			if(i == 8){
				pvc_num = -1;
				pid_del = -1;
			}
			return count;
			//break;
	}
}

static int tsarm_pvc_pid_read_proc(char *page, char **start, off_t off,
		                        int count, int *eof, void *data){
	    int len = sprintf(page, "%d\r\n", pid_add);
	    return len;
}

static int tsarm_pvc_pid_write_proc(struct file *file, const char *buffer,
		                    unsigned long count, void *data){
	char val_string[100], *pid;
	int i = 0,j = 0,index = 0;
	char*ptr = NULL;
	char*ptrbegin = NULL;
	int number[8] = {0,0,0,0,0,0,0,0};
	char tmp[10] = {0};
	
	if (count > sizeof(val_string) - 1)
    		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	#if 0
	val_string[count] = '\0';
	pid = strrchr(val_string, ' ');
	if(!pid)
		sscanf(val_string, "%lu", &pid_add);
	else
		sscanf(pid+1, "%lu", &pid_add);
	#endif
	
	val_string[count] = '\0';
	strcat(val_string," ");
	ptrbegin = val_string;
	
	pid_add = -1;//if not find new value ,return -1	
	//get the all pid information
	while(ptr = strchr(ptrbegin,' '))
	{
		memset(tmp,0 ,sizeof(tmp));
		strncpy(tmp,ptrbegin,ptr - ptrbegin);
		sscanf(tmp,"%lu",&number[index]);
		ptrbegin = ptr + 1;
		if(++index >= 8)
			break;
	}	
	
	#if 0
	for(i = 0; i < index; i++)
		printk("\r\nnumber[%d] is %d",i,number[i]);
	#endif
	//fetch the most new pid
	for(i = 0; i < index; i++)
	{
		for(j = 0; j < 8; j++)
		{
			if(number[i] == pid_map[j][2])
				break;
		}
		//because the number is new,so add it to variable 'pid_add'
		if(j == 8)
		{
			pid_add	= number[i];
			break;
		}
	}

	return count;
}
uint32 vpiinfo, vciinfo;
static int tsarm_pvc_info_read_proc(char *page, char **start, off_t off,
		        int count, int *eof, void *data){
	int i,index = 0;
	char *atminfo = page;
	
	if(vpiinfo == 0 && vciinfo == 0)
	{
		for(i = 0; i < ATM_VC_MAX; i++)
		{
			index += sprintf( atminfo+index, "aal5 vcc statistics for 0.%u.%u\r\n",atmCfgTable.vpi[i], atmCfgTable.vci[i]);
			index += sprintf( atminfo+index, "crc errors\t\t%lu\r\n", pvc_stats[i].crc_errs);
			index += sprintf( atminfo+index, "oversized sdus\t\t%lu\r\n", pvc_stats[i].oversized_sdus);
			index += sprintf( atminfo+index, "short packet errors\t%lu\r\n", pvc_stats[i].short_pkt_errs);
			index += sprintf( atminfo+index, "length errors\t\t%lu\r\n\r\n", pvc_stats[i].len_errs);
        	}
    	}
    	else
	{
    		for(i = 0; i < ATM_VC_MAX; i++)
		{
        		if(atmCfgTable.vpi[i] == vpiinfo && atmCfgTable.vci[i] == vciinfo)
			{
				index += sprintf( atminfo+index, "aal5 vcc statistics for 0.%u.%u\r\n",atmCfgTable.vpi[i], atmCfgTable.vci[i]);
				index += sprintf( atminfo+index, "crc errors\t\t%lu\r\n", pvc_stats[i].crc_errs);
				index += sprintf( atminfo+index, "oversized sdus\t\t%lu\r\n", pvc_stats[i].oversized_sdus);
				index += sprintf( atminfo+index, "short packet errors\t%lu\r\n", pvc_stats[i].short_pkt_errs);
				index += sprintf( atminfo+index, "length errors\t\t%lu\r\n\r\n", pvc_stats[i].len_errs);
			}
		}
	}

	index -= off;
	*start = page + off;

	if (index > count)
		index = count;
	else
		*eof = 1;

	if (index < 0)
		index = 0;
		
	return index;
}

static int tsarm_pvc_info_write_proc(struct file *file, const char *buffer,
		    unsigned long count, void *data){
	char val_string[32];

	vpiinfo = 0;
	vciinfo = 0;
	if (count > sizeof(val_string) - 1)
        	return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	if(sscanf(val_string, "%lu %lu", &vpiinfo, &vciinfo) != 2){
		printk("Input format: vpi vci\r\n");
		return -1;
	}

	return count;
}

#endif

#ifdef CONFIG_NET_SCHED /*Rodney_20091115*/ 
static int tsarm_qosred_read_proc(char *page, char **start, off_t off,
		int count, int *eof, void *data){
	int index=0;
	off_t begin = 0;

	index += sprintf(page+index, "qos_qdisc_mode=%d\n", qos_qdisc_mode);

#ifdef QOS_DROP_DUMP
	int cnt;
	for(cnt=0 ; cnt<MAX_LOG_PKT ; cnt++)
		printk("cnt=%4d flag=%d\n",cnt, qos_drop_flag[cnt]);
#endif

	*eof = 1;

	*start = page + (off - begin);
	index -= (off - begin);
	if (index<0)
		index = 0;
	if (index>count) 
		index = count;
	return index;
}
static int tsarm_qosred_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){
	int len, proc_data[5] = {0}, vc;
	char get_buf[32];
	uint32 reg_tmp;

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

	if(sscanf(get_buf, "%d %d %d %d %d", proc_data, (proc_data+1), (proc_data+2), (proc_data+3), (proc_data+4)) != 5)
		return count;

	qos_qdisc_mode = proc_data[0];

	for ( vc = 0; vc < ATM_VC_MAX; vc++ ){		
		if(atmCfgTable.openFlag[vc] == 1){	
			if((qos_qdisc_mode == QOS_QDISC_RED) || (qos_qdisc_mode == QOS_QDISC_WRED)){
				reg_tmp = TSARM_IRQM(vc);								
				reg_tmp = reg_tmp & 0xfffffffe;
				TSARM_IRQM(vc) = reg_tmp;				
			}else{		
				reg_tmp = TSARM_IRQM(vc);								
				reg_tmp = reg_tmp | 0x00000001;
				TSARM_IRQM(vc) = reg_tmp;
			}
		}
	}

	return len;
}

#endif

static int tsarm_qosred_update_read_proc(char *page, char **start, off_t off,
		int count, int *eof, void *data){
	return 0;
}

/*______________________________________________________________________________
**  update_qosred
**
**  descriptions:used to udate atm qos when adsl is up
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:tsarm_qosred_update_write_proc
**  call:
**____________________________________________________________________________*/
static int update_qosred(uint32 vc)
{
	qosProfile_t *qos_p;


	if ( atmCfgTable.openFlag[vc] == 0) {
		//printk("vc = %d is not active\n",vc);
		return 0;
	}

	qos_p=&qosRecord[vc];
	setAtmQosRed(qos_p,vc);

	return 0;
}
static int tsarm_qosred_update_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){
	int len;
	int vc;
	char get_buf[32];

	/* do a range checking, don't overflow buffers in kernel modules */
	if(count > sizeof(get_buf)-1)
		len = sizeof(get_buf);
	else
		len = count;
	/* use the copy_from_user function to copy buffer data to our get_buf */
	if(copy_from_user(get_buf, buffer, len))
		return -EFAULT;
	/* zero terminate get_buf */
	get_buf[len]='\0';

	if(sscanf(get_buf, "%d", &vc)!= 1)
		return count;

	if(vc>=0 && vc<ATM_VC_MAX)
		update_qosred(vc);
	
	return count;
}
#ifdef CONFIG_QOS
#ifdef TCSUPPORT_SBTHROUGHPUT_ENHANCE
static int qos_switch_read_proc(char *page, char **start, off_t off,
		                        int count, int *eof, void *data){
	    int len = sprintf(page, "%d\r\n", tc_qos_switch);
	    return len;
}
static int qos_switch_write_proc(struct file *file, const char *buffer,
		                    unsigned long count, void *data){
	char val_string[10];

	if (count > sizeof(val_string) - 1)
    		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	sscanf(val_string, "%d", &tc_qos_switch);

	return count;
}
#endif
#endif

#ifdef CONFIG_MIPS_TC3262
static void
tc3262sar_irq_dispatch(void)
{
	do_IRQ(SAR_INT);

}
#endif

static int __init tc3162_atm_init(void)
{
	struct proc_dir_entry *tsarm_proc;
	int vc;
	int err=0;

	printk("TSARM: TC3162 ATM SAR driver 1.5 init\n");
	printk("\r\n%s\n", MODULE_VERSION_TC3162SAR);

	if ((tc3162_atm_dev = atm_dev_register("TSARM", &tc3162_atm_ops, -1, NULL)) == NULL) {
		printk("atm dev register fail.\n");
  	    err = -ENODEV;
		goto failed;
	}

	tc3162_atm_dev->ci_range.vpi_bits = ATM_CI_MAX;        /* atm VPI supports 8 bits */
	tc3162_atm_dev->ci_range.vci_bits = ATM_CI_MAX;        /* atm VCI supports 16 bits */

	#ifdef CONFIG_MIPS_TC3262
	/*Workaround to disabe back to back function, otherwise due to irq=0 event*/
	VPint(0xBFB000B8)=0x1;
	if(cpu_has_vint){
		set_vi_handler(SAR_INT, tc3262sar_irq_dispatch);
	}
	#endif

#ifdef SAR_POLLING
	if (request_irq(SAR_INT, fakeAtmIsr, 0, "TSARM", tc3162_atm_dev) != 0) {
#else
	if (request_irq(SAR_INT, atmIsr, 0, "TSARM", tc3162_atm_dev) != 0) {
#endif
		printk("can not allocate IRQ.\n");
  	    err = -EIO;
		goto failed;
	}
	atmRxBufferSize = ((uint32)(RX_BUF_LEN/48))*48;
	/* init tsarm hardware module */
	atmInit();

	/* atm related stats */
	tsarm_proc = create_proc_entry("tc3162/tsarm_stats", 0, NULL);
	tsarm_proc->read_proc = tsarm_stats_read_proc;
	tsarm_proc->write_proc = tsarm_stats_write_proc;
	create_proc_read_entry("tc3162/tsarm_questats", 0, NULL, tsarm_questats_read_proc, NULL);
	for (vc = 0; vc < ATM_VC_MAX; vc++) {
		proc_vc[vc] = vc;
		sprintf(proc_vc_name[vc], "tc3162/tsarm_vc%d", vc);
		create_proc_read_entry(proc_vc_name[vc], 0, NULL, tsarm_proc_reg, &proc_vc[vc]);
	}
	/* atm oam loopback */
	tsarm_proc = create_proc_entry("tc3162/oam_ping", 0, NULL);
	tsarm_proc->read_proc = oam_ping_read_proc;
	tsarm_proc->write_proc = oam_ping_write_proc;
	/* atm debug */
	tsarm_proc = create_proc_entry("tc3162/tsarm_debug", 0, NULL);
	tsarm_proc->read_proc = tsarm_debug_read_proc;
	tsarm_proc->write_proc = tsarm_debug_write_proc;
	/* atm tx descr dump */
	tsarm_proc = create_proc_entry("tc3162/tsarm_txdescr", 0, NULL);
	tsarm_proc->read_proc = tsarm_txdescrdump_read_proc;
	tsarm_proc->write_proc = tsarm_txdescrdump_write_proc;
	/* atm rx descr dump */
	tsarm_proc = create_proc_entry("tc3162/tsarm_rxdescr", 0, NULL);
	tsarm_proc->read_proc = tsarm_rxdescrdump_read_proc;
	tsarm_proc->write_proc = tsarm_rxdescrdump_write_proc;
	/* atm cc descr dump */
	create_proc_read_entry("tc3162/tsarm_ccdescr", 0, NULL, tsarm_ccdescrdump_read_proc, NULL);
	/* atm reset */
	tsarm_proc = create_proc_entry("tc3162/tsarm_reset", 0, NULL);
	tsarm_proc->read_proc = tsarm_reset_read_proc;
	tsarm_proc->write_proc = tsarm_reset_write_proc;
	/*pork 20090310 added atm packets clear*/
	tsarm_proc = create_proc_entry("tc3162/tsarm_pktsclear", 0, NULL);
	tsarm_proc->read_proc = tsarm_pktsclear_read_proc;
	tsarm_proc->write_proc = tsarm_pktsclear_write_proc;

	//#ifdef QOS_REMARKING  /*Rodney_20090724*/
	#if defined(QOS_REMARKING) || defined(TCSUPPORT_HW_QOS)
	tsarm_proc = create_proc_entry("tc3162/tsarm_qoswrr", 0, NULL);
	tsarm_proc->read_proc = tsarm_qoswrr_read_proc;
	tsarm_proc->write_proc = tsarm_qoswrr_write_proc;
	#endif

	#ifdef TCSUPPORT_QOS
	tsarm_proc = create_proc_entry("tc3162/tcqos_disc", 0, NULL);
	tsarm_proc->read_proc = tsarm_tcqos_read_proc;
	tsarm_proc->write_proc = tsarm_tcqos_write_proc;
	#endif

	//add by brian for atm f5 loopback diagnostic
#ifdef CWMP
	tsarm_proc = create_proc_entry("tc3162/atm_f5_loopback_diagnostic", 0, NULL);
	tsarm_proc->read_proc = loopback_diagnostic_read_proc;
	tsarm_proc->write_proc = loopback_diagnostic_write_proc;

	tsarm_proc = create_proc_entry("tc3162/atm_f5_loopback_diagnostic_reset", 0, NULL);
	tsarm_proc->read_proc = loopback_diagnostic_reset_read_proc;
	tsarm_proc->write_proc = loopback_diagnostic_reset_write_proc;
#endif
#ifdef CMD_API
	tsarm_proc = create_proc_entry("tc3162/tsarm_pvc", 0, NULL);
	tsarm_proc->read_proc = tsarm_pvc_read_proc;
	tsarm_proc->write_proc = tsarm_pvc_write_proc;
	tsarm_proc = create_proc_entry("tc3162/tsarm_pvc_del", 0, NULL);
	tsarm_proc->read_proc = tsarm_pvc_del_read_proc;
	tsarm_proc->write_proc = tsarm_pvc_del_write_proc;
	tsarm_proc = create_proc_entry("tc3162/tsarm_pvc_pid", 0, NULL);
	tsarm_proc->read_proc = tsarm_pvc_pid_read_proc;
	tsarm_proc->write_proc = tsarm_pvc_pid_write_proc;
	tsarm_proc = create_proc_entry("tc3162/tsarm_pvc_info", 0, NULL);
	tsarm_proc->read_proc = tsarm_pvc_info_read_proc;
	tsarm_proc->write_proc = tsarm_pvc_info_write_proc;
	
#endif
#ifdef CONFIG_NET_SCHED  /*Rodney_20091115*/
	tsarm_proc = create_proc_entry("tc3162/tsarm_qosred", 0, NULL);
	tsarm_proc->read_proc = tsarm_qosred_read_proc;
	tsarm_proc->write_proc = tsarm_qosred_write_proc;
#endif

#ifdef CONFIG_QOS
#ifdef TCSUPPORT_SBTHROUGHPUT_ENHANCE
	tsarm_proc = create_proc_entry("tc3162/qos_switch", 0, NULL);
	tsarm_proc->read_proc = qos_switch_read_proc;
	tsarm_proc->write_proc = qos_switch_write_proc;
#endif
#endif
	tsarm_proc = create_proc_entry("tc3162/tsarm_qosred_update", 0, NULL);
	tsarm_proc->read_proc = tsarm_qosred_update_read_proc;
	tsarm_proc->write_proc = tsarm_qosred_update_write_proc;
	
	/*pork 20090310 added atm packets clear over*/
	#ifdef SAR_VERIFY
	/*Register ci-cmd*/
	sarVerifyInit();
	#endif
    #ifdef CONFIG_MIPS_TC3262
	spin_lock_init(&sarLock);
#endif
#ifdef L2_AUTOPVC
	autopvcInit();
#endif

#ifdef SAR_POLLING
	init_waitqueue_head( &isrPollingQueue);
	polling_task = kthread_run(isrPollingThread, NULL, "sarPollingIsrThread");
#endif
	printk("TSARM: TC3162 ATM SAR driver 1.5 done\n");
	return 0;

failed:
	if (tc3162_atm_dev)
		atm_dev_deregister(tc3162_atm_dev);
	tc3162_atm_dev = NULL;
    return err;
}

static void __exit tc3162_atm_exit(void)
{
	int vc;

	printk("tc3162_atm_exit\n");

	free_irq(SAR_INT, tc3162_atm_dev);

	// disable rx
	TSARM_GFR = 0x0;
	while ((TSARM_IRQH & 0x00fff000) >> 12)
		TSARM_IRQC = 1;
	sarDrvDescripReset(1);

	atm_dev_deregister(tc3162_atm_dev);
	tc3162_atm_dev = NULL;

	remove_proc_entry("tc3162/tsarm_stats", NULL);
	remove_proc_entry("tc3162/tsarm_questats", NULL);
	for (vc = 0; vc < ATM_VC_MAX; vc++) {
		sprintf(proc_vc_name[vc], "tc3162/tsarm_vc%d", vc);
		remove_proc_entry(proc_vc_name[vc], NULL);
	}
	remove_proc_entry("tc3162/oam_ping", NULL);
	remove_proc_entry("tc3162/tsarm_debug", NULL);
	remove_proc_entry("tc3162/tsarm_txdescr", NULL);
	remove_proc_entry("tc3162/tsarm_rxdescr", NULL);
	remove_proc_entry("tc3162/tsarm_ccdescr", NULL);
	remove_proc_entry("tc3162/tsarm_reset", NULL);
	#ifdef QOS_REMARKING  /*Rodney_20090724*/
	remove_proc_entry("tc3162/tsarm_qoswrr", NULL);
	#endif
	//add by brian for atm f5 loopback diagnostic
#ifdef CWMP
	remove_proc_entry("tc3162/atm_f5_loopback_diagnostic", NULL);
	remove_proc_entry("tc3162/atm_f5_loopback_diagnostic_reset", NULL);
#endif
#ifdef CMD_API
	remove_proc_entry("tc3162/tsarm_pvc", NULL);
	remove_proc_entry("tc3162/tsarm_pvc_del", NULL);
	remove_proc_entry("tc3162/tsarm_pvc_pid", NULL);
	remove_proc_entry("tc3162/tsarm_pvc_info", NULL);
#endif
#ifdef CONFIG_NET_SCHED  /*Rodney_20091115*/
	remove_proc_entry("tc3162/tsarm_qosred", NULL);
#endif
#ifdef CONFIG_QOS
#ifdef TCSUPPORT_SBTHROUGHPUT_ENHANCE
	remove_proc_entry("tc3162/qos_switch", NULL);
#endif
#endif
	remove_proc_entry("tc3162/tsarm_qosred_update", NULL);

#ifdef DYNAMIC_ALLOC_DMA_BUF
	#ifndef DMA_API
	if (atmTxCcDescrPool) {
		kfree(atmTxCcDescrPool);
		atmTxCcDescrPool = NULL;
	}
	if (atmRxCcDescrPool) {
		kfree(atmRxCcDescrPool);
		atmRxCcDescrPool = NULL;
	}
	if (ccBuf) {
		kfree(ccBuf);
		ccBuf = NULL;
	}
	if (oamBuf) {
		kfree(oamBuf);
		oamBuf = NULL;
	}
	if (atmTxDescrPool) {
		kfree(atmTxDescrPool);
		atmTxDescrPool = NULL;
	}
	if (atmRxDescrPool) {
		kfree(atmRxDescrPool);
		atmRxDescrPool = NULL;
	}
	if (atmRxIrq) {
		kfree(atmRxIrq);
		atmRxIrq = NULL;
	}
	#else
	dma_free_coherent(NULL, sizeof(atmTxCcDescrPool_t), atmTxCcDescrPool, (dma_addr_t) atmTxCcDescrPool_phys_p);
	dma_free_coherent(NULL, sizeof(atmRxCcDescrPool_t), atmRxCcDescrPool, (dma_addr_t) atmRxCcDescrPool_phys_p);
	dma_free_coherent(NULL, sizeof(atmRxCcCell_t)*ATM_RX_CC_DESCR_NUMMAX, ccBuf, (dma_addr_t) ccBuf_phys_p);
	dma_free_coherent(NULL, sizeof(atmRxOamDescrPool_t)*ATM_VC_MAX, oamBuf, (dma_addr_t) oamBuf_phys_p);
	dma_free_coherent(NULL, atmTxDescrPoolSize, atmTxDescrPool, (dma_addr_t) atmTxDescrPool_phys_p);
	dma_free_coherent(NULL, sizeof(atmRxDescrPool_t)*ATM_VC_MAX, atmRxDescrPool, (dma_addr_t) atmRxDescrPool_phys_p);
	dma_free_coherent(NULL, sizeof(uint32)*IRQ_MAX_ENTRY, atmRxIrq, (dma_addr_t) atmRxIrq_phys_p);
	#ifndef SAR_VERIFY
	dma_free_coherent(NULL,  sizeof(atmCell_t)*ATM_VC_MAX, atmCellCirBuf, (dma_addr_t) atmCellCirBuf_phys_p);
	#else
	dma_free_coherent(NULL,  sizeof(atmCell_t)*MCELL_BUF_RING_SIZE, atmCellCirBuf, (dma_addr_t) atmCellCirBuf_phys_p);
	#endif
	#endif
	#ifdef SAR_VERIFY
	sarVerifyExit();
	#endif
#endif
#ifdef L2_AUTOPVC
	autopvcExit();
#endif
#ifdef SAR_POLLING
	if( polling_task){
		kthread_stop(polling_task);
	}
#endif
}

module_init (tc3162_atm_init);
module_exit (tc3162_atm_exit);
