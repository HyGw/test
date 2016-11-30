/*
** $Id: //BBN_Linux/Branch/Branch_for_MT7520_20120528/tclinux_phoenix/modules/private/tcphy/tcetherphy.c#1 $
*/
/* 
** 20090506 yhchen
**      replace macPhyReset() to tc2031LoadReg() in tc2031SwPatch()-related functions
**         rewrite tc2031CfgSelect()
** 20090505 yhchen 
**      LEM from B12.9 to B13.1
**      enable Link_Det_ext
**      update AFE setting from Mog/CCLin
**      add tc2101mb_fix_AnVsF100_timer
** 20090430 yhchen
**      rename macros, variables, and functions; code refactoring
**
*/

/************************************************************************
*                E X T E R N A L   R E F E R E N C E S
*************************************************************************
*/
#include "tcetherphy.h"
#ifdef LINUX_OS
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
#include <asm/tc3162/cmdparse.h>
#ifdef MII_INTERFACE
#include <linux/mii.h>
#endif
#if defined(CONFIG_MIPS_RT63365)
#include "../raeth/femac.h"
#else
#if  defined(CONFIG_MIPS_TC3182) ||  defined(CONFIG_MIPS_TC3262)
#include "../tc3262/tc3262gmac.h"
#else
#include "../tc3162l2hp2h/tc3162l2mac.h"
#endif
#endif
#ifdef EEE_SUPPORT
#include "../tc3162l2hp2h/psm_verify.h"
#endif
#endif //LINUX_OS
#ifndef LINUX_OS //LINOS_OS
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "switch.h"
#include "global.h"
#include "os.h"
#include "cdefs.h"
#include "qlm.h"
#include "qcm.h"
#include "qrm.h"
#include "mbuf.h"
#include "cbuf.h"
#include "filter.h"
#include "spt.h"
#include "timer.h"
#include "chann.h"
#include "dixframe.h"
#include "iface.h"
#include "sys_util.h"
#include "cmdparse.h"
#include "termios.h"
#include "stdio.h"
#include "bomoitf.h"
#include "ndis_lib.h"
#include "errorlog.h"
#include "tracelog.h"
#include "sys_isr.h"
#include "res_mgr.h"
#include "rassys.h"
#include "raserror.h"
#include "event.h"
#include "evnt_cmd.h"
#include "ndis_evt.h"
#include "config.h"
#include "ether.h"
#include "enet_itf.h"
#include "enet_drv.h"
#include "bcp.h"
#include "enet_err.h"
#include "debugusr.h"
#include "sys_mod.h"
#include "project.h"
#include "pci.h"
#include "pcicfg.h"
#include "isrmips.h"
#include "macDrv.h"
#include "gpioadd.h"
#include "sys_proc.h"
#include "tc3162.h"
#ifdef PURE_BRIDGE
#include "Net_eth.h"
#include "Net_arp.h"
#include "zyxel.h"
#endif
#if defined (CENTRALIZE_LED)
#include "ledcetrl.h"
#endif
#if defined(CENTRALIZE_NETTASK)
#include "atm.h"
#endif
#ifndef PURE_BRIDGE
#ifdef IGMP_SNOOPING
#include "igmpsnoop.h"
#endif
#ifdef WLAN_BY_ETHER_SWITCH
#ifdef RT2880_SUPPORT
#include "rt2880_inic.h"
#include "rt2880_common.h"
#include "rt2880_wlan.h"
#endif
#endif
#endif
#include "vcpool.h"
#ifdef TCPHY_SUPPORT
#include "iface.h"
#include "sockaddr.h"
#include "usock.h"
#include "internet.h"
#include "ip.h"
#include "icmp.h"
#include "cmdparse.h"
#include "socket.h"
#include "overlay.h"
#endif
#ifdef TC3162U_PSM_SUPPORT
#include "mac_eee.h"
#endif
#endif //LINOS_OS

/************************************************************************
*                         D A T A   T Y P E S
*************************************************************************
*/
#ifdef LINUX_OS
#define pause(x)					mdelay(x)
#endif //LINUX_OS

#ifdef TCPHY_DEBUG
#ifdef LINUX_OS
extern void setUserSpaceFlag(int num);
#if  defined(CONFIG_MIPS_TC3182) ||  defined(CONFIG_MIPS_TC3262)
extern int tc3262_gmac_tx(struct sk_buff *skb, struct net_device *dev);
extern struct net_device *tc3262_gmac_dev;
#else
extern struct net_device *tc3162_mac_dev;
extern int tc3162_mac_tx(struct sk_buff *skb, struct net_device *dev);
#endif
#endif //LINUX_OS
#ifndef LINUX_OS /*Linux platform is used sendicmp user-space to send icmp packet*/
typedef struct {
    ping_t  *ping_p;        /* ping pointer         */
} pingEvt_t;

typedef struct {
    uint32  trace:1;
    uint32  echo:1;
    uint32  active:1;
    uint32  user:1;
    uint32  checkCmd:1;
    uint32  checkRsp:1;
} pingCmdFlag_t;

#define N_PELOG         20
typedef struct {
    mbuf_t* mbp;
    uint16  seq1;
    uint16  seq2;
    char    cc[4];
} peLog_t;

typedef struct {
    uint16  index;
    uint16  cnt;            /* data drror counter       */
    peLog_t log[N_PELOG];
} peLogDcb_t;

typedef struct {
    pingCmdFlag_t flags;
    int actSocket;
    uint32  pingCnt;
    uint32  echoSeq;
    uint16  option;         /* ping data option     */
    uint16  dataSeq;        /* data sequence number     */
    char    dataChar;       /* data character       */
    uint8   checkInd;       /* data check indication    */
    peLogDcb_t* pelog_p;        /* data log dcb         */
} pingCmdCb_t;
#endif
#endif

/************************************************************************
*                        D A T A for TCPHY
*************************************************************************
*/

extern macAdapter_t *mac_p;
//const bool disable = 0;
//const bool enable = 1;

#ifdef TCPHY_SUPPORT
static uint8  tcPhyFlag = 0;
static uint8  tcPhyVer = 99; // default: unknown PhyVer
static uint8  fgMT7530_INT = 0;
static int eco_rev = 0x00;

#if 0
const uint8 tcPhyVer_2031 = 0; // 9400, LEH
const uint8 tcPhyVer_2101mb = 1; // 9401, LEM
const uint8 tcPhyVer_2104mc = 2; // 9402, tc2206
const uint8 tcPhyVer_2104sd = 3; // 9403, tc2104sd
const uint8 tcPhyVer_2101me = 4; // 9404, 62UE
#else
#define tcPhyVer_2031    0 // 9400, LEH
#define tcPhyVer_2101mb  1 // 9401, LEM
#define tcPhyVer_2104mc  2 // 9402, tc2206
#define tcPhyVer_2104sd  3 // 9403, tc2104sd
#define tcPhyVer_2101me  4 // 9404, 62UE
#define tcPhyVer_2102me  5 // 9405, tc3182
#define tcPhyVer_2104me  6 // 9406, tc2206F
#define tcPhyVer_2101mf  7 // 9407, FPGA(MF)
#define tcPhyVer_2105sg  8 // 9408, TC6501
#define tcPhyVer_2101mi  10 // 940a, RT63260
#define tcPhyVer_2105mj  11 // 940b, RT63365
#define tcPhyVer_2105sk  12 // 940c, TC6508
#define mtPhyVer_7530 13//MT7530
#endif

static uint8 tcPhyPortNum = 1; // set value in tcPhyVerLookUp

// for tcXXXX_link_state
#define ST_LINK_DOWN 0
#define ST_LINK_DOWN2UP 1
#define ST_LINK_UP 2
#define ST_LINK_UP2DOWN 3
//
static uint8 tcphy_link_state[TCPHY_PORTNUM];
static uint8 mtphy_link_state[MTPHY_PORTNUM];

// Auto, AN, 100F, 100H, 10F,  10H,
#if 0
const uint8 tcphy_speed_Auto = 0;
const uint8 tcphy_speed_ForceAN = 1;
const uint8 tcphy_speed_Force100F = 2;
const uint8 tcphy_speed_Force100H = 3;
const uint8 tcphy_speed_Force10F = 4;
const uint8 tcphy_speed_Force10H = 5;
#else
#ifdef TCPHY_1PORT
#define tcphy_speed_Auto  0
#define tcphy_speed_ForceAN  1
#define tcphy_speed_Force100F  2
#define tcphy_speed_Force100H  3
#define tcphy_speed_Force10F  4
#define tcphy_speed_Force10H  5
#endif
#endif

#ifdef TCPHY_1PORT
static uint8 tcphy_speed = tcphy_speed_Auto;
#endif
#endif // TCPHY_SUPPORT

#if defined( TCPHY_DEBUG) || defined(MTPHY_DEBUG)
// variable for doPing()
static uint32 PingReplyCnt = 0;
// variable for doPhyChkVal()
static uint8 phychkval_flag = 0;
static uint8 phychkval_portnum = 0;
// variable for doPhyForceLink()
static uint8 force_link_flag = 0;


// variables for doErrMonitor() / tcPhyErrMonitor()
static volatile uint8 err_monitor_flag = 0;
static uint16 runt_cnt = 0;
static uint16 tlcc_cnt = 0;
static uint16 crc_cnt = 0;
static uint16 long_cnt = 0;
static uint16 loss_cnt = 0;
static uint16 col_cnt = 0;

// variables for doPhyLoopback()
static volatile uint8 phy_loopback_flag = 0;
static volatile uint8 recv_ok_flag = 0;
static volatile uint8 recv_err_flag = 0;
#ifndef LINUX_OS
static volatile uint8 timeout_flag = 0;
#endif
static volatile uint8 timeout_cnt = 0;
#endif

#ifdef TCPHY_FIELD_DBGTEST
#define MAX_RECORD_TC_ETHER_PHY_STR_NUM 256
#define MAX_RECORD_TC_ETHER_PHY_STR_LEN 16
static uint32 msgcnt = 0;
char tc_ether_phy[MAX_RECORD_TC_ETHER_PHY_STR_NUM][MAX_RECORD_TC_ETHER_PHY_STR_LEN];
char RASVersion[3]="v4";
#endif

//V2.00 mark  #define PERIOD_PRINT


/************************************************************************
*            Variables for TCPHY xxxxSwPatch()
*************************************************************************
*/
//*** TCPHY registers ***//
#ifdef TCPHY_SUPPORT

// type for register settings
typedef struct cfg_data_s{
    uint32 reg_num;
    uint32 val;
}cfg_data_t;
typedef struct cfg_cL45data_s{
    uint32 dev_num;
    uint32 reg_num;
    uint32 val;
}cfg_cl45data_t;

typedef struct cfg_trdata_s{
    char reg_typ[10];
    uint32 reg_num;
    uint32 val;
}cfg_trdata_t;

// variables for doPhyConfig()
static uint8 sw_patch_flag = 1;
static uint8 Skew_update_flag = 0;
static uint8 EEE_DSP_Freeze_flag = 0;
static uint8 VBuffer_1000M_flag = 0;
static uint32 Skew_port_cnt = 0;


static uint8 current_idx = 0; // default 0, Do NOT change 
static uint8 sw_ErrOverMonitor_flag = 1; // default enable
#ifdef PERIOD_PRINT //V1.17
static uint8 period_print_flag = 1; // default enable
static uint16 period_print_cnt = 0; // counter
static uint16 period_print_threshold = 240; // print message every 0.25s*240=60s
#endif

//#if defined(TC2104MC_SUPPORT) || defined(TC2101ME_SUPPORT) || defined(TC2101MF_SUPPORT)
static uint8 cfg_Tx10AmpSave_flag = 1; // default enable
static uint8 cfg_LDPS_flag = 1; // default enable
//#endif
#if defined(TC2104MC_SUPPORT) || defined(TC2101ME_SUPPORT) || defined(TC2104ME_SUPPORT) || defined(TC2102ME_SUPPORT) || defined(TC2105SG_SUPPORT)
static uint8 sw_FixUp2DownFast_flag = 1;
#endif
#if defined(TC2101MB_SUPPORT) || defined(TC2104MC_SUPPORT)
static uint8 sw_FixTrcyStep_flag = 1; // default enable
#endif
#endif // TCPHY_SUPPORT

#ifdef TCPHY_SUPPORT
// variables for doPhyDispFlag()
#ifdef TCPHY_DEBUG_DISP_LEVEL
uint8 tcPhy_disp_level = 2; // default level 2
#else
uint8 tcPhy_disp_level = 0; // turn all message OFF for formal release
#endif

uint mtSkewCal_disp_level=0;

static uint8 power_on_finish_flag = 0;  //dai121019 indicate boot end

#ifdef LINUX_OS
#ifdef TCPHY_SUPPORT

extern uint32 miiStationRead(macAdapter_t *mac_p,uint32 phyReg);
extern void miiStationWrite(macAdapter_t *mac_p,uint32 phyReg,uint32 miiData);
extern uint32 tcMiiStationRead(uint32 enetPhyAddr,uint32 phyReg);
extern void tcMiiStationWrite(uint32 enetPhyAddr,uint32 phyReg,uint32 miiData);
extern void tcMiiStationWrite_ext(uint32 enetPhyAddr,uint32 phyReg,uint32 miiData);
extern uint32 tcMiiStationRead_ext(uint32 enetPhyAddr,uint32 phyReg);
extern void tcMiiExtStationWrite_ext(uint32 portAddr,uint32 devAddr,uint32 regAddr,uint32 miiData);
extern uint32 tcMiiExtStationRead_ext(uint32 portAddr,uint32 devAddr, uint32 regAddr, uint8  op);
extern int subcmd(const cmds_t tab[], int argc, char *argv[], void *p);
extern int cmd_register(cmds_t *cmds_p);
#define	RAND_MAX		32767
#endif
#endif //LINUX_OS
// tcPhy_disp_level = 1:min. 2:typ. 3:max. message
#define TCPHYDISP1 if(tcPhy_disp_level>=1) printf
#define TCPHYDISP2 if(tcPhy_disp_level>=2) printf
#if defined(TCPHY_DEBUG) || defined(MTPHY_DEBUG)
#define TCPHYDISP3 if(tcPhy_disp_level>=3) printf
#define TCPHYDISP4 if(tcPhy_disp_level>=4) printf
#define TCPHYDISP5 if(tcPhy_disp_level>=5) printf
#define TCPHYDISP6 if(tcPhy_disp_level>=6) printf
#else
#define TCPHYDISP3 if(0) printf
#define TCPHYDISP4 if(0) printf
#define TCPHYDISP5 if(0) printf
#define TCPHYDISP6 if(0) printf
#endif
#define SkewDISP1 if(mtSkewCal_disp_level>=1) printf

#endif

#ifdef TCPHY_SUPPORT
// variables for mii registers
typedef struct {
    uint8 main_reset; // 15
    uint8 force_speed; // 13
    uint8 autoneg_enable; // 12
    uint8 powerdown; // 11
    uint8 force_duplex; // 8
} tcphy_mr0_reg_t;

typedef struct {
    uint16 value; // 15:0
    //uint8 autoneg_complete; // 5
    bool link_status; // 2
    bool link_status_prev;
} tcphy_mr1_reg_t;

typedef struct {
    uint8 selector_field; //4:0
    uint8 able100F; // 8
    uint8 able100H; // 7
    uint8 able10F; // 6
    uint8 able10H; // 5
} tcphy_mr4_reg_t; // use for mr4 & mr5

typedef struct {
    uint8 selector_field; //4:0
    uint8 able100F; // 8
    uint8 able100H; // 7
    uint8 able10F; // 6
    uint8 able10H; // 5
    uint8 LPNextAble;//15
} tcphy_mr5_reg_t; // use for mr4 & mr5


typedef struct {
    //uint8 parallel_detect_fault; // 4
    uint8 lp_np_able; // 3
    //uint8 np_able; // 2
    //uint8 lch_page_rx; // 1
    uint8 lp_autoneg_able; // 0
} tcphy_mr6_reg_t; // use for mr6

typedef struct {
    //uint8  slicer_err_thd; // 15:11
    uint16 err_over_cnt;   // 10:0
    uint16 err_over_cnt_prev; // 10:0
} tcphy_l0r25_reg_t;
typedef struct {
    //uint8  slicer_err_thd; // 15:11
    uint16 err_over_cnt;   // 10:0
    uint16 err_over_cnt_prev; // 10:0
} mtphy_errovcnt_reg_t;
typedef struct {
    uint8 lch_sig_detect;
    uint8 lch_rx_linkpulse;
    uint8 lch_linkup_100;
    uint8 lch_linkup_10;
    uint8 lch_linkup_mdi;
    uint8 lch_linkup_mdix;
    uint8 lch_descr_lock;
    uint8 mdix_status;
    uint8 tx_amp_save;
    uint8 final_duplex;
    uint8 final_speed;
    uint8 final_link;   
} tcphy_l0r28_reg_t;

typedef struct {
    uint8 lch_SignalDetect;//15
    uint8 lch_LinkPulse;//14
    uint8 lch_DescramblerLock1000;//13
	uint8 lch_DescramblerLock100;//12
    uint8 lch_LinkStatus1000_OK;//11
	uint8 lch_LinkStatus100_OK;//10
	uint8 lch_LinkStatus10_OK;//9 
	uint8 lch_MrPageRx;//8
	uint8 lch_MrAutonegComplete;//7
  uint8 da_mdix;//6
  uint8 FullDuplexEnable;//5
  uint8 MSConfig1000;// 4
  uint8 final_speed_1000;// 3 
  uint8 final_speed_100;// 2
  uint8 final_speed_10;// 1
} tcphy_1ErA2_reg_t;

typedef struct {
    uint8 lp_eee_10g;
    uint8 lp_eee_1000;
    uint8 lp_eee_100;  
} tcphy_l3r18_reg_t;


tcphy_mr0_reg_t mr0;
tcphy_mr4_reg_t mr4; 
tcphy_mr5_reg_t mr5;
tcphy_mr6_reg_t mr6;
tcphy_l0r28_reg_t mr28; // L0R28
tcphy_l3r18_reg_t mrl3_18; // L3R17
tcphy_1ErA2_reg_t mr1E_A2;


// for multiple phy support
tcphy_mr1_reg_t Nmr1[TCPHY_PORTNUM];
tcphy_l0r25_reg_t Nmr25[TCPHY_PORTNUM];
mtphy_errovcnt_reg_t ErovcntA[MTPHY_PORTNUM];
mtphy_errovcnt_reg_t ErovcntB[MTPHY_PORTNUM];
mtphy_errovcnt_reg_t ErovcntC[MTPHY_PORTNUM];
mtphy_errovcnt_reg_t ErovcntD[MTPHY_PORTNUM];


// define for tc????ReadProbe()
#define ProbeZfgain  0
#define ProbeAgccode 1
#define ProbeBoosten 2
#define ProbeSnr     3
#define ProbeDcoff   4
#define ProbeAdcoff  5
#define ProbeAdcSign 6
#define Probe_VgaState 7
#define Probe_MSE 8
#define Probe_MSE_ALL 9
#define Probe_MSE_Sum 10
#define Probe_Slicer_Err_Over_Sum 11
#define Probe_Slicer_Err_Over_Sum_ALL 12
#define Probe_MSE_Slicer_err_thres 13

//bool tcphy_anen = ENABLE;
//bool tcphy_speed = ENABLE;
//bool tcphy_duplex = ENABLE;

// define for tc??LinkFailDetect()
#define TbtOrHbt 0
#define HbtOnly 1
#define TbtOnly 2
#endif



/************************************************************************
*
*            Common Functions for tc????SwPatch()
*
*************************************************************************
*/
#ifdef TCPHY_SUPPORT
uint32 tcPhyReadReg(uint8 port_num,uint8 reg_num);
void tcPhyWriteReg(uint8 port_num,uint8 reg_num,uint32 reg_data);
uint32 tcPhyReadLReg(uint8 port_num,uint8 page_num,uint8 reg_num);
void tcPhyWriteLReg(uint8 port_num,uint8 page_num,uint8 reg_num,uint32 reg_data);
uint32 tcPhyReadGReg(uint8 port_num,uint8 page_num,uint8 reg_num);
uint32 mtPhyReadGReg(uint32 port_num, uint32 dev_num, uint32 reg_num);
uint32 mtPhyReadReg(uint8 port_num, uint8 reg_num);
void tcPhyWriteGReg(uint8 port_num,uint8 page_num,uint8 reg_num,uint32 reg_data);
int mtPhyMiiWrite_TrDbg(uint8 phyaddr, char *type, uint32 reg ,uint32 val, uint8 ch_num);
int32 mtPhyMiiRead_TrDbg(uint8 phyaddr, char *type, uint32 reg , uint8 ch_num);
void mtPhyChkVal (void);
void mtMiiRegWrite(uint32 port_num, uint32 reg_num, uint32 reg_data);
uint32 mtMiiRegRead(uint8 port_num,uint8 reg_num);
uint32 tcMiiExtStationRead_CL22(uint32 port_num, uint32 dev_num, uint32 reg_num);
void tcMiiExtStationWrite_CL22(uint32 port_num,uint32 dev_num,uint32 reg_num,uint32 reg_data);
void mtEMiiRegWrite(uint32 port_num, uint32 dev_num, uint32 reg_num, uint32 reg_data);
uint32 mtEMiiRegRead(uint32 port_num, uint32 dev_num, uint32 reg_num);
#endif

void mtMiiRegWrite(uint32 port_num, uint32 reg_num, uint32 reg_data)
{
	if((tcPhyVer == mtPhyVer_7530)&&(fgMT7530_INT==0))
		tcMiiStationWrite_ext( port_num,reg_num, reg_data);
	else
		tcMiiStationWrite(port_num, reg_num, reg_data);
}

uint32 mtMiiRegRead(uint8 port_num,uint8 reg_num)
{
	if((tcPhyVer == mtPhyVer_7530)&&(fgMT7530_INT==0))
		return(tcMiiStationRead_ext( port_num,reg_num));
	else
		return(tcMiiStationRead(port_num, reg_num));
}


uint32 tcMiiExtStationRead_CL22(uint32 port_num, uint32 dev_num, uint32 reg_num)
{

    const uint16 MMD_Control_register=0xD;
    const uint16 MMD_addr_data_register=0xE;
  	const uint16 page_reg=31;
	
	uint32 value=0;

	mtMiiRegWrite(port_num, page_reg, 0x00); //switch to main page
	mtMiiRegWrite(port_num, MMD_Control_register, (0<<14)+dev_num);
	mtMiiRegWrite(port_num, MMD_addr_data_register, reg_num);
	mtMiiRegWrite(port_num, MMD_Control_register, (1<<14)+dev_num);
	value = mtMiiRegRead(port_num, MMD_addr_data_register);
//	printf("* doPhyMMDRead_CL22=>phyaddr=%d,  dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", port_num, dev_num, reg_num, value);
}

void tcMiiExtStationWrite_CL22(uint32 port_num,uint32 dev_num,uint32 reg_num,uint32 reg_data)
{
    const uint16 MMD_Control_register=0xD;
    const uint16 MMD_addr_data_register=0xE;
  	const uint16 page_reg=31;


	mtMiiRegWrite(port_num, page_reg, 0x00); //switch to main page
	mtMiiRegWrite(port_num, MMD_Control_register, (0<<14)+dev_num);
	mtMiiRegWrite(port_num, MMD_addr_data_register, reg_num);
	mtMiiRegWrite(port_num, MMD_Control_register, (1<<14)+dev_num);
	mtMiiRegWrite(port_num, MMD_addr_data_register, reg_data);
//	printf("* doPhyMMDWrite_CL22=> phyaddr=%d, dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", port_num, dev_num, reg_num, reg_data);
}


void mtEMiiRegWrite(uint32 port_num, uint32 dev_num, uint32 reg_num, uint32 reg_data)
{
	if((tcPhyVer == mtPhyVer_7530)&&(fgMT7530_INT==0))
		tcMiiExtStationWrite_ext( port_num, dev_num, reg_num, reg_data);
	else
		tcMiiExtStationWrite_CL22( port_num, dev_num, reg_num, reg_data);
}

uint32 mtEMiiRegRead(uint32 port_num, uint32 dev_num, uint32 reg_num)
{
	if((tcPhyVer == mtPhyVer_7530)&&(fgMT7530_INT==0))
		return(tcMiiExtStationRead_ext( port_num, dev_num, reg_num, NORMAL_READ));
	else
		return(tcMiiExtStationRead_CL22( port_num, dev_num, reg_num));
}




#ifdef TCPHY_SUPPORT
uint32 
mtPhyReadReg(uint8 port_num, uint8 reg_num)
{
    uint32 val, val_r31;
    uint32 phyAddr = mac_p->enetPhyAddr + port_num;

     tcMiiStationWrite_ext(port_num,31,0);
     
    if (tcPhyVer!=tcPhyVer_2031 && (reg_num<16 || reg_num==31)){
      
        val = mtMiiRegRead(phyAddr, reg_num); 
        //printf("mtPhyReadReg:phyAddr =%d, reg_num =0x%02lX, val =0x%04lx\r\n",phyAddr,reg_num,val);
    }
    else{
        val_r31 = tcMiiStationRead(phyAddr, 31); // remember last page
        // set page to L0 if necessary
        if (val_r31 != 0x8000) {
            tcMiiStationWrite(phyAddr, 31, 0x8000);
        }
        // read reg
        val = tcMiiStationRead(phyAddr, reg_num); 
        // restore page if necessary
        if (val_r31 != 0x8000) {
            tcMiiStationWrite(phyAddr, 31, val_r31);
        }
    }

    // update variables
    switch(reg_num){
        
    case 0:
        mr0.main_reset = (val>>15)&0x00000001;
        mr0.force_speed    = (val>>13)&0x00000001;
        mr0.autoneg_enable = (val>>12)&0x00000001;
        mr0.powerdown      = (val>>11)&0x00000001;
        mr0.force_duplex   = (val>>8)&0x00000001;
        break;
        
    case 1:
        //mr1.autoneg_complete = (val>>5)&0x00000001;       
        //mr1_link_status_reg = val;
        //mr1.value = val;
        //mr1.link_status_prev = mr1.link_status; 
        //mr1.link_status = (val>>2)&0x00000001;
        Nmr1[port_num].value = val;
        Nmr1[port_num].link_status_prev = Nmr1[port_num].link_status;
        Nmr1[port_num].link_status = (val>>2)&0x00000001;
        //if(Nmr1[port_num].link_status !=  Nmr1[port_num].link_status_prev){
        //printf("mtPhyReadReg case 1 :Nmr1[%d].value =0x%04lx\r\n", port_num,Nmr1[port_num].value);
        //printf("mtPhyReadReg case 1 :Nmr1[%d].link_status_prev =0x%04lx\r\n",port_num,Nmr1[port_num].link_status_prev );
        //printf("mtPhyReadReg case 1 :Nmr1[%d].link_status =0x%04lx\r\n", port_num,Nmr1[port_num].link_status);
        //  }
        break;

    case 4:
        mr4.able100F = (val>>8)&0x0001;     
        mr4.able100H = (val>>7)&0x0001;     
        mr4.able10F = (val>>6)&0x0001;  
        mr4.able10H = (val>>5)&0x0001;  
        mr4.selector_field = (val)&0x001f;
        break;

    case 5:
        mr5.able100F = (val>>8)&0x0001;     
        mr5.able100H = (val>>7)&0x0001;     
        mr5.able10F = (val>>6)&0x0001;  
        mr5.able10H = (val>>5)&0x0001;  
        mr5.selector_field = (val)&0x001f;
	mr5.LPNextAble = (val>>15)&0x0001;
        break;
        
    case 6:     
        mr6.lp_np_able = (val>>3)&0x0001;
        mr6.lp_autoneg_able = (val)&0x0001;
        break;
        
    default:
        break;      
    }   
    return (val);
}
uint32 
tcPhyReadReg(uint8 port_num, uint8 reg_num)
{
    uint32 val, val_r31;
    uint32 phyAddr = mac_p->enetPhyAddr + port_num;

    if (tcPhyVer!=tcPhyVer_2031 && (reg_num<16 || reg_num==31)){
        val = tcMiiStationRead(phyAddr, reg_num); 
    }
    else{
        val_r31 = tcMiiStationRead(phyAddr, 31); // remember last page
        // set page to L0 if necessary
        if (val_r31 != 0x8000) {
            tcMiiStationWrite(phyAddr, 31, 0x8000);
        }
        // read reg
        val = tcMiiStationRead(phyAddr, reg_num); 
        // restore page if necessary
        if (val_r31 != 0x8000) {
            tcMiiStationWrite(phyAddr, 31, val_r31);
        }
    }

    // update variables
    switch(reg_num){
        
    case 0:
        mr0.main_reset = (val>>15)&0x00000001;
        mr0.force_speed    = (val>>13)&0x00000001;
        mr0.autoneg_enable = (val>>12)&0x00000001;
        mr0.powerdown      = (val>>11)&0x00000001;
        mr0.force_duplex   = (val>>8)&0x00000001;
        break;
        
    case 1:
        //mr1.autoneg_complete = (val>>5)&0x00000001;       
        //mr1_link_status_reg = val;
        //mr1.value = val;
        //mr1.link_status_prev = mr1.link_status; 
        //mr1.link_status = (val>>2)&0x00000001;
        Nmr1[port_num].value = val;
        Nmr1[port_num].link_status_prev = Nmr1[port_num].link_status;
        Nmr1[port_num].link_status = (val>>2)&0x00000001;
        break;

    case 4:
        mr4.able100F = (val>>8)&0x0001;     
        mr4.able100H = (val>>7)&0x0001;     
        mr4.able10F = (val>>6)&0x0001;  
        mr4.able10H = (val>>5)&0x0001;  
        mr4.selector_field = (val)&0x001f;
        break;

    case 5:
        mr5.able100F = (val>>8)&0x0001;     
        mr5.able100H = (val>>7)&0x0001;     
        mr5.able10F = (val>>6)&0x0001;  
        mr5.able10H = (val>>5)&0x0001;  
        mr5.selector_field = (val)&0x001f;
		mr5.LPNextAble = (val>>15)&0x0001;
        break;
        
    case 6:     
        mr6.lp_np_able = (val>>3)&0x0001;
        mr6.lp_autoneg_able = (val)&0x0001;
        break;
        
    case 25:        
        //mr25.slicer_err_thd = (val >> 11) & 0x0000001f;
        //mr25.err_over_cnt_prev = mr25.err_over_cnt;
        //mr25.err_over_cnt = (val & 0x0000007ff);      
        Nmr25[port_num].err_over_cnt_prev = Nmr25[port_num].err_over_cnt;
        Nmr25[port_num].err_over_cnt = (val & 0x0000007ff);
        break;
        
    case 28:
        mr28.lch_sig_detect  = (val>>15)&0x0001;
        mr28.lch_rx_linkpulse= (val>>14)&0x0001;
        mr28.lch_linkup_100  = (val>>13)&0x0001;
        mr28.lch_linkup_10   = (val>>12)&0x0001;
        mr28.lch_linkup_mdi  = (val>>11)&0x0001; // after LEM
        mr28.lch_linkup_mdix = (val>>10)&0x0001; // after LEM
        mr28.lch_descr_lock  = (val>>9)&0x0001; // after LEM
        mr28.mdix_status  = (val>>5)&0x0001; /* {0:mdi,1:mdix} */   
        mr28.tx_amp_save  = (val>>3)&0x0003; /* 0:100%, 1:90%, 2:80%, 3:70% */
        mr28.final_duplex = (val>>2)&0x0001; /* {0:half-duplex, 1:full-duplex} */
        mr28.final_speed  = (val>>1)&0x0001; /* {0:10, 1:100} */
        mr28.final_link   = (val)&0x0001; /* {0:linkdown, 1:linkup} */      
        break;
        
    default:
        break;      
    }   
    return (val);
}

void
tcPhyWriteReg(uint8 port_num,uint8 reg_num,uint32 reg_data){
    uint32 val_r31;
    uint32 phyAddr = mac_p->enetPhyAddr + port_num;

    val_r31 = tcMiiStationRead(phyAddr, 31); // remember last page
    // set page if necessary
    if (val_r31 != 0x8000) {
        tcMiiStationWrite(phyAddr, 31, 0x8000); // page to L0
    }
    tcMiiStationWrite(phyAddr, reg_num, reg_data); 
    // restore page if necessary
    if (val_r31 != 0x8000) {
        tcMiiStationWrite(phyAddr, 31, val_r31);
    }
}

// read Local Reg
uint32
tcPhyReadLReg(uint8 port_num,uint8 page_num,uint8 reg_num){
    uint32 val, val_r31;
    uint32 phyAddr = mac_p->enetPhyAddr + port_num;
    uint32 pageAddr = (page_num<<12)+0x8000;

    val_r31 = tcMiiStationRead(phyAddr, 31);  // remember last page
    // set page if necessary
    if (val_r31 != pageAddr) {
        tcMiiStationWrite(phyAddr, 31, pageAddr); // switch to page Lx
    }
    val = tcMiiStationRead(phyAddr, reg_num); 
    // restore page if necessary
    if (val_r31 != pageAddr) {
        tcMiiStationWrite(phyAddr, 31, val_r31);
    }
    if (page_num==3) {
		switch(reg_num){
			case 18:
               mrl3_18.lp_eee_10g = (val>>3)&0x0001;
               mrl3_18.lp_eee_1000 = (val>>2)&0x0001;
               mrl3_18.lp_eee_100 = (val>>1)&0x0001;
               break;
			default:
                 break; 
			}
    	}
    return val;
}

//void tcMiiExtStationWrite(uint32 devAddr,uint32 portAddr,uint32 regAddr,uint32 miiData){
//}
//uint32 tcMiiExtStationRead(uint32 devAddr, uint32 portAddr, uint32 regAddr, uint8  op){
//}

// write Local Reg
void
tcPhyWriteLReg(uint8 port_num,uint8 page_num,uint8 reg_num,uint32 reg_data){
    uint32 val_r31;
    uint32 phyAddr = mac_p->enetPhyAddr + port_num;
    uint32 pageAddr = (page_num<<12)+0x8000;

    val_r31 = tcMiiStationRead(phyAddr, 31);  // remember last page
    // set page if necessary
    if (val_r31 != pageAddr) {
        tcMiiStationWrite(phyAddr, 31, pageAddr); // switch to page Lx  
    }
    tcMiiStationWrite(phyAddr, reg_num, reg_data); 
    // restore page if necessary
    if (val_r31 != pageAddr) {
        tcMiiStationWrite(phyAddr, 31, val_r31);
    }
}

// read Global Reg
uint32
tcPhyReadGReg(uint8 port_num,uint8 page_num,uint8 reg_num){
    uint32 val, val_r31;
    uint32 phyAddr = mac_p->enetPhyAddr + port_num;
    uint32 pageAddr = (page_num<<12);

    val_r31 = tcMiiStationRead(phyAddr, 31);  // remember last page
    // set page if necessary
    if (val_r31 != pageAddr) {
        tcMiiStationWrite(phyAddr, 31, pageAddr); // switch to page Gx  
    }
    val = tcMiiStationRead(phyAddr, reg_num); 
    // restore page if necessary
    if (val_r31 != pageAddr) {
        tcMiiStationWrite(phyAddr, 31, val_r31);
    }
    
    return val;
}
#if 1

uint32
mtPhyReadGReg(uint32 port_num, uint32 dev_num, uint32 reg_num)
{
    uint32 val;
    
    //val = tcMiiExtStationRead_ext(port_num, dev_num, reg_num, NORMAL_READ);
    val = mtEMiiRegRead(port_num, dev_num, reg_num);

    // update variables
    switch(reg_num){
    case 0xA2:
        mr1E_A2.lch_SignalDetect  = (val>>15)&0x0001;
        mr1E_A2.lch_LinkPulse= (val>>14)&0x0001;
        mr1E_A2.lch_DescramblerLock1000  = (val>>13)&0x0001;
        mr1E_A2.lch_DescramblerLock100   = (val>>12)&0x0001;
        mr1E_A2.lch_LinkStatus1000_OK  = (val>>11)&0x0001;
        mr1E_A2.lch_LinkStatus100_OK = (val>>10)&0x0001; 
        mr1E_A2.lch_LinkStatus10_OK  = (val>>9)&0x0001; 
        mr1E_A2.lch_MrPageRx  = (val>>8)&0x0001; 
        mr1E_A2.lch_MrAutonegComplete  = (val>>7)&0x0001; 
        mr1E_A2.da_mdix  = (val>>6)&0x0001; 
        mr1E_A2.FullDuplexEnable  = (val>>5)&0x0001;    
        mr1E_A2.MSConfig1000  = (val>>4)&0x0001; 
        mr1E_A2.final_speed_1000 = (val>>3)&0x0001; 
        mr1E_A2.final_speed_100  = (val>>2)&0x0001; 
        mr1E_A2.final_speed_10   = (val>>1)&0x0001;       
        break;
        
    default:
        break;      
    }   
    return (val);
}
#endif
// write Global Reg
void
tcPhyWriteGReg(uint8 port_num,uint8 page_num,uint8 reg_num,uint32 reg_data){
    uint32 val_r31;
    uint32 phyAddr = mac_p->enetPhyAddr + port_num;
    uint32 pageAddr = (page_num<<12);

    val_r31 = tcMiiStationRead(phyAddr, 31);  // remember last page
    // set page if necessary
    if (val_r31 != pageAddr) {
        tcMiiStationWrite(phyAddr, 31, pageAddr); // switch to page Gx
    }
    tcMiiStationWrite(phyAddr, reg_num, reg_data); 
    // restore page if necessary
    if (val_r31 != pageAddr) {
        tcMiiStationWrite(phyAddr, 31, val_r31);
    }
}

#endif


/************************************************************************
*
*            Variables and Functions for tc2031SwPatch()
*
*************************************************************************
*/


//*** TC2031 register settting ***//
#ifdef TC2031_SUPPORT
#define TC2031_PHY_INIT_DATA_LEN 18
#define TC2031_PHY_INIT_SET_NUM 3
    
    typedef struct tc2031_cfg_data_s{
        char name[10];
        cfg_data_t data[TC2031_PHY_INIT_DATA_LEN];
    }tc2031_cfg_data_t;
    
    
    tc2031_cfg_data_t tc2031_cfg[TC2031_PHY_INIT_SET_NUM]={
    {{"R38.23"},{{31,0x0000}, { 0,0x3c1f}, { 1,0x524a}, { 2,0x625a}, { 3,0xa166}, 
                 { 4,0x0114}, {11,0x9007}, {17,0x0e10}, {18,0x42ca}, {20,0x7988},  
                 {23,0x140c}, {25,0x6000}, {26,0x0001}, {28,0x4310}, 
                 {31,0x8000}, {21,0x0068}, {29,0xf300}, {19,0x00e0}}}, // default setting           
    {{"R38b"}, { {31,0x0000}, { 0,0x3c1f}, { 1,0x524a}, { 2,0x625a}, { 3,0xa166}, 
                 { 4,0x0914}, {11,0x9007}, {17,0x0e10}, {18,0x42ca}, {20,0x7988},  
                 {23,0x140c}, {25,0x6000}, {26,0x0001}, {28,0x4310}, 
                 {31,0x8000}, {21,0x0068}, {29,0xf300}, {19,0x00e0}}}, // link-down & 100Mbps only
    {{"R38c"}, { {31,0x0000}, { 0,0x3c1f}, { 1,0x524a}, { 2,0x625a}, { 3,0xa166},
                 { 4,0x0114}, {11,0xa007}, {17,0x0e10}, {18,0x42ca}, {20,0x7988},  
                 {23,0x140c}, {25,0x6000}, {26,0x0001}, {28,0x0310}, 
                 {31,0x8000}, {21,0x0068}, {29,0xf300}, {19,0x00e0}}}, // 10Mbps only
    };

#endif


#ifdef TC2031_SUPPORT
//*** function declaration ***
static void tc2031Config(int argc, char *argv[], void *p);
static void tc2031CfgLoad(uint8 idx);
void tc2031LdpsSet(uint8 mode);
int tc2031ReadErroverSum(void);
int tc2031ReadProbe(uint16 mode);
void tc2031ReadProbe100(void);
void tc2031DispProbe100(void);
int tc2031ReadSnrSum(int32 loop);
int tc2031ReadDcoffSum(void);
int tc2031ReadAdcoffSum(void);
void tc2031LinkReset(void);
int tc2031CfgSelect(void);
//void tc2031CfgChange(void);
void tc2031MdixForce(uint32 mode);
//void tc2031LinkChk(void);
//void tc2031ReadReg(uint32 reg_num);
void tc2031LinkUp(void);
void tc2031LinkUp2Down(void);
void tc2031LinkDown2Up(void);
void tc2031LinkDown(void);
void tc2031SwPatch(void);
#endif


#ifdef TC2031_SUPPORT
//*** variables ***
// variables for Config()
static uint8 config_pairswap = 0; // auto, fixMDI, fixMDIX
static uint8 config_ldps = 0; // off, on

// variables for doPhyErrOver() & ErrOver report in LinkUp
#ifdef TC2031_DEBUG
static uint32 sr_err_over_thd=0;
static uint32 sr_err_over_thd4up=0;
#endif
static uint32 sr_err_rep_time=10;

//static uint32 chk_interval = 3;
static uint16 sr_linkup_cnt=0;
static uint16 sr_anysig_cnt = 0;
static uint16 sr_nosig_cnt = 0;
static uint16 sr_linkup1_cnt = 0; // one-way linkup timer
static uint16 sr_linkup2_cnt = 0; // timer for mid, mdix
static uint16 sr_linkup2mdix_cnt = 0;
static uint16 sr_linkup2mdi_cnt = 0;
static uint8 sr_force_crossover = 0; /*0:auto,1:mdi,2:mdix */
static uint8 sr_force_crossover_next = 0; /*0:auto,1:mdi,2:mdix */
static uint16 sr_force_crossover_cnt = 0; 
static uint16 sr_linkup1p_cnt = 0; // periodic linkup timer

static uint32 sr_boosten = 0; 
static uint32 sr_agccode = 0; 
static uint32 sr_zfgain_thd=7; // zfgain threshold to turn-on boosten 
static uint32 sr_snr_thd=450;   // snr threshold to turn-on boosten
static uint32 sr_lp1_snr = 0;
static uint32 sr_lp0_snr = 0;
//static uint32 sr_zfgain_long_thd=10;  // if zfgain>sr_zfgain_long_thd => long loop case
static uint32 sr_zfgain = 0; 
static uint8 sr_cfg_change_flag = 0;
#endif


#ifdef TC2031_SUPPORT   
//*** function body ***

// used in CI command "tce config"
static void tc2031Config(int argc, char *argv[], void *p)
{
#ifdef TCPHY_DEBUG
        if (argc==1) {
            TCPHYDISP3("Usage: config PairSwap [auto|fixMDI|fixMDIX]\r\n");
            TCPHYDISP3("       config LDPS     [on|off]\r\n");
            return;
        }
        else if (argc==2) {
            if (stricmp(argv[1], "list") == 0){
                TCPHYDISP1("Config PairSwap status: ");             
                switch(config_pairswap) {
                    case 0: TCPHYDISP3("Auto-Detect.\r\n"); break;
                    case 1: TCPHYDISP3("Force MDI.\r\n");   break;              
                    case 2: TCPHYDISP3("Force MDIX.\r\n");  break;              
                    default: TCPHYDISP3(" Error!!\r\n");
                }
                printf("Config LDPS status: ");             
                switch(config_ldps) {
                    case 0: TCPHYDISP3("OFF.\r\n"); break;
                    case 1: TCPHYDISP3("ON.\r\n");  break;              
                    default: TCPHYDISP3(" Error!!\r\n");
                }
                return;
            }
        }
#endif

    if (argc==2 || argc==3){
        if (stricmp(argv[1], "PairSwap") == 0){
            if( stricmp(argv[2], "auto") == 0 ){
                config_pairswap = 0;
                tc2031MdixForce(config_pairswap);
            }
            else if( stricmp(argv[2], "fixMDI") == 0 ){
                config_pairswap = 1;
                tc2031MdixForce(config_pairswap);
            }
            else if( stricmp(argv[2], "fixMDIX") == 0 ){
                config_pairswap = 2;
                tc2031MdixForce(config_pairswap);
            }
            else{
                TCPHYDISP1("Config PairSwap status: ");             
                switch(config_pairswap) {
                    case 0: TCPHYDISP2("Auto-Detect.\r\n"); break;
                    case 1: TCPHYDISP2("Force MDI.\r\n");   break;              
                    case 2: TCPHYDISP2("Force MDIX.\r\n");  break;              
                    default: TCPHYDISP2(" Error!!\r\n");
                }
            }
        }
        else if (stricmp(argv[1], "LDPS") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                config_ldps = 1;                
                tc2031LdpsSet(config_ldps);
            }
            else if( stricmp(argv[2], "off") == 0 ){
                config_ldps = 0;
                tc2031LdpsSet(config_ldps);
            }
            else{
                switch(config_ldps) {
                    case 0: TCPHYDISP2("OFF.\r\n"); break;
                    case 1: TCPHYDISP2("ON.\r\n");  break;              
                    default: TCPHYDISP2(" Error!!\r\n");
                }
            }           
        }
    }

}

// 1. loading default register setting in tcPhyInit()
// 2. changing register setting
static void 
tc2031CfgLoad(uint8 idx)
{
    int i;
    current_idx=idx;
    for( i=0; i<TC2031_PHY_INIT_DATA_LEN; i++ ){            
        miiStationWrite(mac_p, tc2031_cfg[current_idx].data[i].reg_num, tc2031_cfg[current_idx].data[i].val );
    }
    TCPHYDISP2("tcphy CfgLoad %s\r\n",  tc2031_cfg[current_idx].name);
}

int
tc2031ReadErroverSum(void)
{
    int32 lr_err_over_sum = 0;
    
    //write LR25 once in tc2031LinkDown2Up
    //miiStationWrite( mac_p, 25, 0xc000 ); 
    tcPhyReadReg(0,25);
    pause(300);
    tcPhyReadReg(0,25);
    lr_err_over_sum = Nmr25[0].err_over_cnt-Nmr25[0].err_over_cnt_prev;
    if( lr_err_over_sum < 0 )
        lr_err_over_sum += 2048;
    
    return lr_err_over_sum;
}

int
tc2031ReadProbe(uint16 mode)
{
    uint32 val, val_gr25, val_r20;
    uint32 rval, wval;
    uint32 sign_bit;
    
    miiStationWrite( mac_p, 31, 0x8000 );
    val_r20 = miiStationRead(mac_p, 20);
    miiStationWrite( mac_p, 31, 0x0000 );
    val_gr25 = miiStationRead(mac_p, 25);

    switch(mode){
        case ProbeZfgain:
            miiStationWrite( mac_p, 31, 0x8000 );
            miiStationWrite( mac_p, 20, 0x000b ); // zfgain(5:0)            
            miiStationWrite( mac_p, 31, 0x0000 );
            wval = val_gr25 | 0x0008 ;
            miiStationWrite( mac_p, 25, wval );
            val = miiStationRead(mac_p, 27);
            rval = (val)&0x3f ;
            break;
        case ProbeAgccode:
            miiStationWrite( mac_p, 31, 0x8000 );
            miiStationWrite( mac_p, 20, 0x000e ); // boosten(4), agc(3:1)
            miiStationWrite( mac_p, 31, 0x0000 );
            wval = val_gr25 | 0x0009 ;
            miiStationWrite( mac_p, 25, wval );
            val = miiStationRead(mac_p, 27);
            rval = (val>>1)&0x07 ;
            break;
        case ProbeBoosten:
            miiStationWrite( mac_p, 31, 0x8000 );
            miiStationWrite( mac_p, 20, 0x000e ); // boosten(4), agc(3:1)
            miiStationWrite( mac_p, 31, 0x0000 );
            wval = val_gr25 | 0x0009 ;
            miiStationWrite( mac_p, 25, wval );
            val = miiStationRead(mac_p, 27);
            rval = (val>>4)&0x01 ;
            break;
        case ProbeSnr:
          miiStationWrite( mac_p, 31, 0x8000 );
            miiStationWrite( mac_p, 20, 0x0009 ); // accD(7:0)
            miiStationWrite( mac_p, 31, 0x0000 );
            wval = val_gr25 | 0x0008 ;
            miiStationWrite( mac_p, 25, wval );
            val = miiStationRead(mac_p, 27);
            rval = (val)&0xff ;
          break;    
        case ProbeDcoff:
          miiStationWrite( mac_p, 31, 0x8000 );
            miiStationWrite( mac_p, 20, 0x0010 ); // accD(7:0)
            miiStationWrite( mac_p, 31, 0x0000 );
            wval = val_gr25 | 0x0008 ;
            miiStationWrite( mac_p, 25, wval );
            val = miiStationRead(mac_p, 27);
            sign_bit = (val>>7)&0x01;
            rval =(val)&0xff ;
            rval = (sign_bit)? rval-256:rval;
          break;
        case ProbeAdcoff:
          miiStationWrite( mac_p, 31, 0x8000 );
            miiStationWrite( mac_p, 20, 0x0002 ); // accD(7:0)
            miiStationWrite( mac_p, 31, 0x0000 );
            wval = val_gr25 | 0x0008 ;
            miiStationWrite( mac_p, 25, wval );
            val = miiStationRead(mac_p, 27);
            rval =(val)&0x3f ;
            rval = rval-32;
          break;
        default:
            TCPHYDISP1("Error: tc2031ReadProbe %d.\r\n",mode);
            rval = 0;
            break;
    }
    
    miiStationWrite( mac_p, 31, 0x0000 );
    miiStationWrite( mac_p, 25, val_gr25); // restore GReg25
    miiStationWrite( mac_p, 31, 0x8000 ); // keep local-reg
    miiStationWrite( mac_p, 20, val_r20); // restore local-reg20
    
    return rval;

}

void 
tc2031ReadProbe100(void)
{
    TCPHYDISP2("tcphy:");
    sr_boosten=tc2031ReadProbe(ProbeBoosten);
    TCPHYDISP2(" boosten=%ld", sr_boosten);
    sr_agccode=tc2031ReadProbe(ProbeAgccode);
    TCPHYDISP2(" agccode=%ld", sr_agccode);
    sr_zfgain=tc2031ReadProbe(ProbeZfgain);
    TCPHYDISP2(" zfgain=%ld", sr_zfgain);
    TCPHYDISP2(" snr=%d", tc2031ReadProbe(ProbeSnr));
    TCPHYDISP2(" \r\n");
}

void 
tc2031DispProbe100(void)
{
#ifdef TC2031_DEBUG
    TCPHYDISP2("tcphy:");
    TCPHYDISP2(" boosten=%d",tc2031ReadProbe(ProbeBoosten));
    TCPHYDISP2(" agccode=%d",tc2031ReadProbe(ProbeAgccode));
    TCPHYDISP2(" zfgain=%d",tc2031ReadProbe(ProbeZfgain));
    TCPHYDISP2(" err_over_sum=%d\r\n", tc2031ReadErroverSum());
    TCPHYDISP2(" snr=%d\r\n", tc2031ReadProbe(ProbeSnr));
    //
    TCPHYDISP3(" snr_sum(x1000)=%d\r\n",tc2031ReadSnrSum(1000)); // snr_sum
    TCPHYDISP3(" blw_out_sum(x100)=%d\r\n",tc2031ReadDcoffSum()); // blow_out_sum
    TCPHYDISP3(" Adc_out_off_sum(x1000)=%d\r\n",tc2031ReadAdcoffSum()); // adc_offset_sum
#endif
}

int
tc2031ReadSnrSum(int32 loop)
{
    int32 lr_snr_sum = 0;
    int32 snr=0;
    //int32 cnt=loop;
    int j;
    
    for(j=0;j<loop;j++)
    {
      snr=tc2031ReadProbe(ProbeSnr);
      lr_snr_sum=lr_snr_sum+snr;
    }
    return lr_snr_sum;
}

int
tc2031ReadDcoffSum(void)
{
    int32 lr_Dcoff_sum = 0;
    int32 tmp_Dcoff_sum= 0;  
    int32 Dcoff=0;
    int32 cnt=100;
    int j;
    
    for(j=0;j<cnt;j++)
    {
      Dcoff=tc2031ReadProbe(ProbeDcoff);
      tmp_Dcoff_sum=tmp_Dcoff_sum+Dcoff;
    }
    //shift right to show percent of the dc offset (unit:%)
    lr_Dcoff_sum=(tmp_Dcoff_sum>>7);    
    
    return lr_Dcoff_sum;
}

int
tc2031ReadAdcoffSum(void)
{
    int32 lr_Adcoff_sum = 0;
    int32 tmp_Adcoff_sum= 0;  
    int32 Adcoff=0;
    int32 cnt=1000;
    int j;
    
    for(j=0;j<cnt;j++)
    {
      Adcoff=tc2031ReadProbe(ProbeAdcoff);
      tmp_Adcoff_sum=tmp_Adcoff_sum+Adcoff;
    }
    //shift right to show percent of the dc offset (unit:%)
    lr_Adcoff_sum=(tmp_Adcoff_sum>>5);    
    
    return lr_Adcoff_sum;
}



void 
tc2031LinkReset(void)
{
    uint32 val;

    TCPHYDISP3("tc2031LinkReset\r\n");
    if (mr0.autoneg_enable){
        val = miiStationRead(mac_p, 0);
        val |= 0x00000200;
        miiStationWrite( mac_p, 0, val );
    }
    else
    {
        tcPhyReadReg(0,0);
        val = miiStationRead(mac_p, 0);
        miiStationWrite( mac_p, 0, 0x1200);
        miiStationWrite( mac_p, 0, val);
    }
}

int 
tc2031CfgSelect(void)
{ 
    uint8 idx_next;
    uint8 change_cfg = 1;
    uint32 sr_snr;
    
    if (sr_cfg_change_flag>=1){ // to avoid endless retry
        change_cfg = 0;
        return (change_cfg);
    }
    else {
        sr_cfg_change_flag++;
    }
    
    // if (sr_cfg_change_flag<1)
    if(sr_boosten==1){ 
        idx_next = current_idx;
    } 
    else { // sr_boosten==0
        if(sr_zfgain>=sr_zfgain_thd){
            idx_next = 1;
        }
        else {
            idx_next = 0;
            // goto check lp_agc0 vs lp_agc1
        }
    }

    // check lp_agc0 vs lp_agc1
    if (sr_boosten == 0) {
        sr_snr = tc2031ReadSnrSum(100);
        if(sr_snr>=sr_snr_thd) {
            sr_lp0_snr =sr_snr;
           
            miiStationWrite( mac_p, 31, 0x8000 );
            miiStationWrite( mac_p, 14, 0x0011 ); // switch to gain1            
    
            sr_lp1_snr=tc2031ReadSnrSum(100);       
            //tc2031LinkChk();      
            tcPhyReadReg(0,1);
            if( Nmr1[0].link_status==0) {  
                Nmr1[0].link_status_prev = 0; // skip tc2031LinkUp2Down()
                Nmr1[0].link_status = 0;
                //macPhyReset();
                tc2031CfgLoad(current_idx);
                tc2031LinkReset();
                tcPhyReadReg(0,28); /*read clear */
                sr_anysig_cnt= 0;
                sr_nosig_cnt= 0;                     
                TCPHYDISP2("LP AGC1 will make DUT Link Down!!! \r\n");
            }               
           
            if(sr_lp1_snr>=sr_lp0_snr || Nmr1[0].link_status==0) {
                miiStationWrite( mac_p, 31, 0x8000 );
                miiStationWrite( mac_p, 14, 0x0010 ); // switch to gain0            
            }          
            TCPHYDISP2("LP0(snr:%ld), LP1(snr:%ld) \r\n",sr_lp0_snr,sr_lp1_snr);
        }
    }
    
    if(current_idx == idx_next){
        change_cfg = 0;   
    }
    else{
        change_cfg = 1;
    }
    
    current_idx = idx_next ;
    TCPHYDISP2("tcphy setting select %s\r\n",  tc2031_cfg[current_idx].name);
 
    return (change_cfg);
}

#if 0   
void 
tc2031CfgChange(void)
{
  // 0 -> 1
    current_idx++;
    // current_idx==2 for final_speed==0(10Mbps) only
    if( (current_idx >= TC2031_PHY_INIT_SET_NUM)||(current_idx == 2) )
        current_idx = 0;

  
  /* 0 -> 1 -> 3
   current_idx++;
    // current_idx==2 for final_speed==0(10Mbps) only
    if( (current_idx >= TC2031_PHY_INIT_SET_NUM))
        current_idx = 0;
    if(current_idx == 2)    
    current_idx = 3;
  */
    
  /* 0->1 , 0->3
  int long_flag=0;
  if(sr_zfgain>sr_zfgain_long_thd) 
      long_flag=1;
  
  if(current_idx==0 && long_flag==0)
     current_idx=1;
  else if(current_idx==0 && long_flag==1)
     current_idx=3;
  else      
     current_idx=0;
  */
  
  /*
  // 0 -> 1 -> 2 -> 3 -> 0 ....
    current_idx++;
    // current_idx==2 for final_speed==0(10Mbps) only
    if((current_idx == 2) )
      current_idx = 3;
    if( (current_idx >= TC2031_PHY_INIT_SET_NUM))
        current_idx = 0;
  */
    TCPHYDISP2("tcphy CfgChange %s\r\n",  tc2031_cfg[current_idx].name);
    //macPhyReset();
    tc2031CfgLoad(current_idx);
}
#endif

void 
tc2031LdpsSet(uint8 mode) // link-down power-saving
{
    miiStationWrite( mac_p, 31, 0x0000);
    if(mode==1){ // on
        miiStationWrite( mac_p, 22, 0x066f);
    }
    else{ // off
        miiStationWrite( mac_p, 22, 0x0064);
    }
    miiStationWrite( mac_p, 31, 0x8000);
}


void
tc2031MdixForce(uint32 mode)
{
    uint32 val;
    
    if(config_pairswap==0){ // AutoMode, use argument "mode"
        val = miiStationRead(mac_p, 26);
        val &= 0x00003fff; // 0:default: auto-crossover
        if(mode==1) // 1:mdi mode
            val |= (1<<14);
        else if (mode==2) // 2:mdix mode
            val |= (1<<15);

        tcPhyReadReg(0,28); // read clear data
        sr_linkup2_cnt=0;
        sr_linkup2mdix_cnt=0;
        sr_linkup2mdi_cnt=0;

        sr_force_crossover = mode;
        miiStationWrite( mac_p, 26, val);
        //TCPHYDISP2("sr_nosig_cnt=%d sr_anysig_cnt=%d\r\n",sr_nosig_cnt,sr_anysig_cnt);
        TCPHYDISP2("tcphy set auto-crossover mode to %ld\r\n",mode);
    }
    else { // Force MDI,MDIX mode, use variable "config_pairswap"
        val = miiStationRead(mac_p, 26);
        val &= 0x00003fff; // 0:default: auto-crossover
        if(config_pairswap==1) // 1:mdi mode
            val |= (1<<14);
        else if (config_pairswap==2) // 2:mdix mode
            val |= (1<<15);
        
        tcPhyReadReg(0,28); // read clear data
        sr_linkup2_cnt=0;
        sr_linkup2mdix_cnt=0;
        sr_linkup2mdi_cnt=0;

        sr_force_crossover = config_pairswap;
        miiStationWrite( mac_p, 26, val);
        TCPHYDISP2("tcphy force crossover mode to %d\r\n",config_pairswap);
    }
}

void 
tc2031MdixChange(void)
{
    if(sr_linkup2_cnt>0){
        if(sr_linkup2mdix_cnt>0)
            tc2031MdixForce(2); // force to mdix
        else if (sr_linkup2mdi_cnt>0)
            tc2031MdixForce(1); // force to mdi
    }

}

void 
tc2031MdixDetect(void)
{
    // ReadReg(28) before the function call
    if(mr28.final_link==1){
    if(sr_linkup2_cnt<20){ // overflow prevention
        sr_linkup2_cnt++;
            if(mr28.mdix_status==1)
                sr_linkup2mdix_cnt++;
            else
                sr_linkup2mdi_cnt++;
        }
//      TCPHYDISP2("mdix_cnt=%d mdi_cnt=%d\r\n",sr_linkup2mdix_cnt,sr_linkup2mdi_cnt);
    } 
}

//void 
//tc2031LinkChk(void)
//{
//  tcPhyReadReg(0,1);
//}

void 
tc2031LinkUp(void)
{
#ifdef TC2031_DEBUG 
    int32 lr_err_over_sum = 0;
    int32 lr_snr_sum=0;
#endif   
    sr_linkup_cnt=0;

    if(sr_linkup1_cnt<30) // one-way timer for linkup
        sr_linkup1_cnt++; 
    if(sr_linkup1p_cnt<sr_err_rep_time) // periodic timer for linkup
        sr_linkup1p_cnt++;
    else
        sr_linkup1p_cnt=0; 

#if 0
    if(sr_linkup1_cnt==1){
        tcPhyReadReg(0,28);
        if( (mr28.final_link==1) &&(mr28.final_speed==1) &&(sr_err_over_thd4up !=0) ){
            lr_err_over_sum = tc2031ReadErroverSum();

            TCPHYDISP2("err_over_sum=%d\r\n", lr_err_over_sum);
            if (lr_err_over_sum > sr_err_over_thd4up)
            {
            
                TCPHYDISP3("tc2031LinkUp CfgChange\r\n");
                tc2031CfgChange();
                tc2031LinkReset();
                Nmr1[0].link_status_prev = 0; // skip tc2031LinkUp2Down()
                Nmr1[0].link_status = 0;
                tcPhyReadReg(0,28); /*read clear */
                sr_anysig_cnt= 0;
                sr_nosig_cnt= 0;            
            }
        }
    }
#endif
#ifdef TC2031_DEBUG     
    // periodic message
    if(tcPhy_disp_level>=3) {       
        if((mr28.final_link==1) &&(mr28.final_speed==1)){ // 100Mbps
            if(sr_linkup1p_cnt==0 && sr_err_rep_time!=0){       
                tc2031ReadProbe100(); // boosten, agccode, zfgain
            }
            if(sr_linkup1p_cnt==0 && sr_err_rep_time!=0){           
                lr_snr_sum = tc2031ReadSnrSum(1000);
                TCPHYDISP3(" snr_sum(x1000)=%ld\r\n",lr_snr_sum); // snr_sum
            }
            if(sr_linkup1p_cnt==0 && sr_err_rep_time!=0){       
                TCPHYDISP3(" blw_out_sum(x100)=%d\r\n",tc2031ReadDcoffSum()); // blow_out_sum
            }
            if(sr_linkup1p_cnt==0 && sr_err_rep_time!=0){       
                TCPHYDISP3(" Adc_out_off_sum(x1000)=%d\r\n",tc2031ReadAdcoffSum()); // adc_offset_sum
            }
            if(sr_linkup1p_cnt==0 && sr_err_rep_time!=0){           
                lr_err_over_sum = tc2031ReadErroverSum();
                TCPHYDISP3(" err_over_sum=%ld\r\n", lr_err_over_sum);               
            }
            else if(sr_err_over_thd4up !=0){
                lr_err_over_sum = tc2031ReadErroverSum();
                if (lr_err_over_sum > sr_err_over_thd4up){
                TCPHYDISP3(" err_over_sum=%ld\r\n", lr_err_over_sum);               
            } 
        }
    }
}
#endif

}

void 
tc2031LinkDown2Up(void)
{
    //int32 lr_err_over_sum = 0;
    
    //TCPHYDISP1("LinkDown2Up\n");
    
    tcPhyReadReg(0,28);
    TCPHYDISP1("TCphy is link-up at %d%s%s\r\n",(mr28.final_speed?100:10),(mr28.final_duplex?"F":"H"),tc2031_cfg[current_idx].name);
    tc2031MdixDetect();
    //tc2031MdixChange();
    if((mr28.final_link==1) &&(mr28.final_speed==1)){
        miiStationWrite( mac_p, 25, 0xc000 ); 
        pause(10);
        //lr_err_over_sum = tc2031ReadErroverSum();
        tc2031ReadProbe100();
        TCPHYDISP2(" \r\n");
        //TCPHYDISP1("err_over_sum=%d\r\n", lr_err_over_sum);
        //sr_snr = tc2031ReadSnrSum(10);
        TCPHYDISP2("tc2031LinkDown2Up: CfgSelect\r\n");
        if(tc2031CfgSelect()==1) {
            //macPhyReset();
            tc2031CfgLoad(current_idx); // current_idx is updated in tc2031CfgSelect();     
            TCPHYDISP3("Load config NO.%1d\r\n", current_idx);
            tc2031LinkReset();
            Nmr1[0].link_status_prev = 0; // skip tc2031LinkUp2Down()
            Nmr1[0].link_status = 0;
            tcPhyReadReg(0,28); /*read clear */
            sr_anysig_cnt= 0;
            sr_nosig_cnt= 0;    
        }   
#if 0       
        if (( sr_err_over_thd !=0 && lr_err_over_sum > sr_err_over_thd)||
            (sr_boosten==0 && sr_zfgain>sr_zfgain_thd) ||        // a->b,d
            (sr_boosten==1 && current_idx==0)||                  // a->b,d  
            (mr28.final_speed==1 && current_idx==2) ||             // c->a,b,d
            (sr_zfgain>sr_zfgain_long_thd && current_idx!=3) ||  // a,b -> d
            (sr_zfgain<sr_zfgain_long_thd && current_idx==3))    // d->a,b 
        {
            TCPHYDISP3("tc2031LinkDown2Up CfgChange\r\n");
            tc2031CfgChange();
            tc2031LinkReset();
            Nmr1[0].link_status_prev = 0; // skip tc2031LinkUp2Down()
            Nmr1[0].link_status = 0;
            tcPhyReadReg(0,28); /*read clear */
            sr_anysig_cnt= 0;
            sr_nosig_cnt= 0;            
        }
#endif
        
    }
    else if((mr28.final_link==1) && (mr28.final_speed==0)){
        //TCPHYDISP3("tc2031LinkDown2Up CfgLoad(2)\r\n");
        // Load Cfg(c) without re-link in 10BaseT
        tc2031CfgLoad(2); // RXXc
        TCPHYDISP3("Load config NO.2\r\n");
    }

    sr_linkup1_cnt=0;
    sr_linkup1p_cnt=0;
}

void 
tc2031LinkUp2Down(void)
{
    sr_cfg_change_flag = 0;
    sr_lp1_snr = 0;
    sr_lp0_snr = 0;
  
    miiStationWrite( mac_p, 31, 0x8000 );
    miiStationWrite( mac_p, 14, 0x0000 ); // switch to automatic gain control   
  
    tcPhyReadReg(0,28);
    TCPHYDISP1("TCphy is link-down!\r\n");  
    tc2031MdixChange();
    sr_nosig_cnt = 0;
    sr_anysig_cnt = 0;
    sr_force_crossover_cnt = 0;
    //tc2031LinkDown(); // useless bcz of sr_(no|any)sig_cnt =0

}

void 
tc2031LinkDown(void)
{
    uint32 lr_any_signal = 0;

    tcPhyReadReg(0,0);
    tcPhyReadReg(0,28);
    //tc2031MdixChange();
    
    lr_any_signal = (mr28.lch_sig_detect | mr28.lch_rx_linkpulse | mr28.lch_linkup_100 | mr28.lch_linkup_10);

    if (lr_any_signal)
        sr_nosig_cnt=0;
    else if (!lr_any_signal && sr_nosig_cnt<30)
        sr_nosig_cnt++;

    if (sr_nosig_cnt>4) // change config for N idle
        sr_anysig_cnt=0;
    else if (lr_any_signal && sr_anysig_cnt<30)
        sr_anysig_cnt++;

    //TCPHYDISP2("sr_nosig_cnt=%d sr_anysig_cnt=%d\r\n",sr_nosig_cnt,sr_anysig_cnt);

    if(sr_nosig_cnt>4){ // change config for N idle
        // TCPHYDISP3("tc2031LinkDown no_signal\r\n");
        if (current_idx!=0){ // keep default when link down
            tc2031CfgLoad(0); 
			TCPHYDISP3("Load config NO.0\r\n");
        }
        if (mr0.autoneg_enable==1 && sr_force_crossover !=0){
            tc2031MdixForce(0);
    }
    }

    if (sr_anysig_cnt>6){
    //  TCPHYDISP2("tc2031LinkDown any_signal\r\n");
        TCPHYDISP3("tc2031LinkDown any_sig CfgChange()\r\n");   
        sr_anysig_cnt=0;
        //tc2031CfgChange();
        if (current_idx==0) {
            tc2031CfgLoad(1);
		    TCPHYDISP3("Load config NO.1\r\n");
        	}
        else {
            tc2031CfgLoad(0);
		    TCPHYDISP3("Load config NO.0\r\n");
        	}	
    }
        
    // SW-auto-crossover if (!mr0.autoneg_enable)
    if (mr0.autoneg_enable==0){
		TCPHYDISP3("Do SW-auto-crossover\r\n");
        if (sr_force_crossover==0) {
            sr_force_crossover_cnt=0;
            if (mr28.mdix_status==1){
                tc2031MdixForce(2); // keep mdix
                sr_force_crossover_next = 1;
            }
            else{
                tc2031MdixForce(1); // keep mdi
                sr_force_crossover_next = 2;
            }
        }
        else{ //sr_force_crossover !=0
            if (sr_force_crossover_cnt <10)
                sr_force_crossover_cnt++;
            
            if (sr_force_crossover_cnt>1 && sr_nosig_cnt>4){
                sr_force_crossover_cnt=0;
                tc2031MdixForce(sr_force_crossover_next);
                if (sr_force_crossover_next !=0){
                    sr_force_crossover_next = 0;
                }
            }
        }   
    }

}

void 
tc2031SwPatch(void)
{
    if( !Nmr1[0].link_status_prev || (sr_linkup_cnt > 0) ){
        //tc2031LinkChk();
        tcPhyReadReg(0,1);
        if( Nmr1[0].link_status ){
            if( Nmr1[0].link_status_prev )
                tc2031LinkUp();
            else
                tc2031LinkDown2Up();
        }
        else{
            if( Nmr1[0].link_status_prev )
                tc2031LinkUp2Down();
            else
                tc2031LinkDown();           
        }
    }
    else{
        sr_linkup_cnt++;
    }
}
#endif


/************************************************************************
*
*            Variables and Functions for tc2101mbSwPatch()
*
*************************************************************************
*/

#ifdef TC2101MB_SUPPORT
//*** TC2101MB register settting ***//
#define TC2101MB_PHY_INIT_DATA_LEN 29
#define TC2101MB_PHY_INIT_SET_NUM 1

typedef struct tc2101mb_cfg_data_s{
    char name[10];
    cfg_data_t data[TC2101MB_PHY_INIT_DATA_LEN];
}tc2101mb_cfg_data_t;

// G0R16: affect the value of tc2101mb_fix_FalseSquelch_G0R16_ON/OFF
#define tc2101mb_FixFalseSquelch_G0R16_default  0x040F
const uint32 tc2101mb_FixFalseSquelch_G0R16_ON = 0x0484;
const uint32 tc2101mb_FixFalseSquelch_G0R16_OFF = 0x040F;
//
//#define tc2101mb_FixTrcyStep_L1R18_default 0x6408 // trgap=64:16T, trstep=4:1  --> 1/16
//#define tc2101mb_FixTrcyStep_L1R18_default 0x4805 // trgap=32T, trstep=2  --> 1/16
//#define tc2101mb_FixTrcyStep_L1R18_default 0x2405 // trgap=16T, trstep=2  --> 1/8
//#define tc2101mb_FixTrcyStep_L1R18_default 0x480a // trgap=32T, trstep=4  --> 1/8
#define tc2101mb_FixTrcyStep_L1R18_default 0x240a // trgap=16T, trstep=4  --> 1/4
const uint32 tc2101mb_FixTrcyStep_L1R18_mode1 = 0x000f; // trgap=8T, trstep=8  --> 1
const uint32 tc2101mb_FixTrcyStep_L1R18_mode2 = 0x4404; // trgap=32:16T, trstep=2:1  --> 1/16
//const uint32 tc2101mb_FixTrcyStep_L1R18_mode2 = 0x2400; // trgap=16T, trstep=1  --> 1/16
//const uint32 tc2101mb_FixTrcyStep_L1R18_mode2 = 0x4805; // trgap=32T, trstep=2  --> 1/16
//const uint32 tc2101mb_FixTrcyStep_L1R18_mode2 = 0x000a; // trgap=8T, trstep=4  --> 1/2


tc2101mb_cfg_data_t tc2101mb_cfg[TC2101MB_PHY_INIT_SET_NUM]={
    {{"B19.0"},
     {{31,0x3000}, {17,0x06B1}, {19,0x0100}, {20,0x0730}, {21,0x0705},
      {31,0x2000}, {29,0x1e1c},
      {31,0x1000}, {19,0x6968}, {20,0x0914}, {24,0x0030}, {27,0x7015},  
      {31,0x0000}, {16,tc2101mb_FixFalseSquelch_G0R16_default}, 
                   {17,0x0c0c}, {18,0x400a}, {20,0x0a08}, 
                   {22,0x066f}, {23,0x0004}, {26,0x0002},
      {31,0x9000}, {16,0x0002}, {18,tc2101mb_FixTrcyStep_L1R18_default}, 
                   {21,0x0010},{22,0x0000},
      {31,0x8000}, {21,0x00c4}, {25,0xa000}, {26,0x1601}}}, 
};
#endif


#ifdef TC2101MB_SUPPORT
//*** function declaration ***
static void tc2101mbConfig(int argc, char *argv[], void *p);
static void tc2101mbCfgLoad(uint8 idx);
#ifdef TCPHY_PHY2PHY 
static void tc2101mbCfgPhy2Phy(void);
#endif

int32 tc2101mbReadProbe(uint8 port_num, uint8 mode);
int32 tc2101mbReadErroverSum(uint8 port_num);
int32 tc2101mbReadAdcSum(uint8 port_num);
int32 tc2101mbReadSnrSum(uint8 port_num ,int32 cnt);
void tc2101mbReadProbe100(uint8 port_num);


void tc2101mbSwDispMessage(uint8 port_num);
void tc2101mbForceMdiMode(uint8 idx);
void tc2101mbSwFixPairSwap(void);
void tc2101mbSwFixAnVsF100(void);
void tc2101mbSwFixFalseSquelch(void);
void tc2101mbUpdateErrOverSum(void);
void tc2101mbSwFixTrcyStepChange(uint8 mode);
void tc2101mbSwFixTrcyStep(void);
void tc2101mbSwErrOverMonitor(void);
void tc2101mbSwPatch(void);
#endif


#ifdef TC2101MB_SUPPORT
//*** variable ***
static uint8 sw_FixAutoPairSwap_flag = 0; // 1:enable
static uint8 tc2101mbSwFixAutoPairSwap_state = 0;  // disable
static uint8 tc2101mbSwFixAutoPairSwap_timer = 0;

static uint8 sw_FixAnVsF100_flag = 1; // 1:enable
static uint8 tc2101mbSwFixAnVsF100_state = 0; // 0:disable
static uint8 tc2101mbSwFixAnVsF100_up100_check = 0; // time to check whether Up with Force 100
static uint8 tc2101mbSwFixAnVsF100_timer = 0; // timer to keep F100 enable 
//static uint8 tc2101mbSwFixAnVsF100_timer_THD = 4; // timer threshold to keep F100

static uint8 sw_FixFalseSquelch_flag = 1; // 1:enable
static uint8 tc2101mbSwFixFalseSquelch_state = 0;

// for sw_FixTrcyStep_flag & tc2101mbSwFixTrcyStep()
static uint8 tc2101mbSwFixTrcyStep_Mode = 0;
static int16 tc2101mbSwFixTrcyStep_WaitTimer = 0;
static int16 tc2101mbSwFixTrcyStep_CheckTimer = 0;
static int16 tc2101mbSwFixTrcyStep_ChangeCnt = 0;
static int tc2101mb_erroversum=0;
static int tc2101mb_snr_avg=0;
static int tc2101mb_snr_avg_pre=0;

#endif


#ifdef TC2101MB_SUPPORT
//*** function body ***

// used in CI command "tce config"
static void 
tc2101mbConfig(int argc, char *argv[], void *p)
{
// global config in doPhyConfig()
#ifdef TCPHY_DEBUG
    if (argc==1) {
        TCPHYDISP3("Usage: config FixAnVsF100 [on|off]\r\n");
        TCPHYDISP3("       config FixFalseSquelch [on|off]\r\n");
        TCPHYDISP3("       config FixAutoPairSwap [on|off]\r\n");
        TCPHYDISP3("       config FixTrcyStep [on|off]\r\n");
        return;
    }
    else if (argc==2) {
        if (stricmp(argv[1], "list") == 0){
            TCPHYDISP3("config FixAnVsF100 : %s\r\n",
                    (sw_FixAnVsF100_flag?"on":"off"));          
            TCPHYDISP3("config FixFalseSquelch : %s\r\n",
                    (sw_FixFalseSquelch_flag?"on":"off"));
            TCPHYDISP3("config FixAutoPairSwap : %s\r\n",
                    (sw_FixAutoPairSwap_flag?"on":"off"));
            TCPHYDISP3("config FixTrcyStep : %s\r\n",
                    (sw_FixTrcyStep_flag?"on":"off"));          
            return;
        }
    }
#endif

    if (argc>=2){
        if (stricmp(argv[1], "FixAnVsF100") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                sw_FixAnVsF100_flag = 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                sw_FixAnVsF100_flag = 0; // disable
            }
            TCPHYDISP2("config FixAnVsF100 : %s\r\n",
                        (sw_FixAnVsF100_flag?"on":"off"));
        }
        else if (stricmp(argv[1], "FixFalseSquelch") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                sw_FixFalseSquelch_flag = 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                sw_FixFalseSquelch_flag = 0; // disable
            }
            TCPHYDISP2("config FixFalseSquelch : %s\r\n",
                        (sw_FixFalseSquelch_flag?"on":"off"));
        }
        else if (stricmp(argv[1], "FixAutoPairSwap") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                sw_FixAutoPairSwap_flag = 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                sw_FixAutoPairSwap_flag = 0; // disable
            }
            TCPHYDISP2("config FixAutoPairSwap : %s\r\n",
                        (sw_FixAutoPairSwap_flag?"on":"off"));
        }
        else if (stricmp(argv[1], "FixTrcyStep") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                sw_FixTrcyStep_flag = 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                sw_FixTrcyStep_flag = 0;
            }
            TCPHYDISP2("config FixTrgap : %s\r\n",
                        (sw_FixTrcyStep_flag?"on":"off"));
        }
    }
}

// 1. loading default register setting in tcPhyInit()
// 2. changing register setting
static void 
tc2101mbCfgLoad(uint8 idx)
{
    int i;
    current_idx = idx;
    for( i=0; i<TC2101MB_PHY_INIT_DATA_LEN; i++ ){          
        miiStationWrite( mac_p, tc2101mb_cfg[current_idx].data[i].reg_num, tc2101mb_cfg[current_idx].data[i].val );
    }
    TCPHYDISP4("tcphy: CfgLoad %s\r\n",  tc2101mb_cfg[current_idx].name);

#ifdef TCPHY_PHY2PHY 
    tc2101mbCfgPhy2Phy();
#endif
    
}
#ifdef TCPHY_PHY2PHY 
// add config setting for PHY-to-PHY without transformer
static void 
tc2101mbCfgPhy2Phy(void)
{
    const uint8 port_num = 0;
    
    sw_FixAnVsF100_flag=0;  
    tcPhyWriteReg(port_num,26,0x5601);    // fix MDI
    tcPhyWriteGReg(port_num,1,20,0x8114); // train LP first
    tcPhyWriteGReg(port_num,3,21,0x0775); // adjust TX waveform
    TCPHYDISP3("tcphy: Load Phy2Phy Config %s\r\n");
}
#endif
int32
tc2101mbReadProbe(uint8 port_num, uint8 mode)
{
    uint32 val, val_r31, val_g0r28;
    uint32 rval, wval;
    uint32 phyaddr = mac_p->enetPhyAddr + port_num;
    
    val_r31 = tcMiiStationRead(phyaddr, 31); 
    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    val_g0r28 = tcMiiStationRead(phyaddr, 28);
  
    switch(mode){
        case ProbeZfgain:
            wval = 0x0b04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x3f ;
            break;
        case ProbeAgccode:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>1)&0x0f ;
            break;
        case ProbeBoosten:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>5)&0x01 ;
            break;
        break;
        case ProbeSnr:
            wval = 0x0904 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0xff ;
            break;  
        case ProbeAdcSign:
            wval = 0x4104 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x7f ;
            if (rval>64){
                rval -= 128;
            }
            break;
        default:
            TCPHYDISP1("\r\ntcphy error: ReadProbe %d.\r\n",mode);
            rval = 0;
            break;
    }

    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    tcMiiStationWrite( phyaddr, 28, val_g0r28); // restore G0Reg28
    tcMiiStationWrite( phyaddr, 31, val_r31); // restore Reg31
    return rval;

}

int32
tc2101mbReadErrOverSum(uint8 port_num)
{
    int32 err_over_sum;
    
    tcPhyReadReg(port_num,25);
    pause(300);
    tcPhyReadReg(port_num,25);
    err_over_sum = Nmr25[0].err_over_cnt - Nmr25[0].err_over_cnt_prev;
    if( err_over_sum < 0 ){
        err_over_sum += 2048;
    }

    return err_over_sum;
}


int32
tc2101mbReadAdcSum(uint8 port_num)
{
    int32 AdcSign_sum = 0;
    int32 cnt=1000;
    int j;
      
    for(j=0;j<cnt;j++){
        AdcSign_sum += tc2101mbReadProbe(port_num,ProbeAdcSign);
    }
    //shift right to show percent of the dc offset (unit:%)
    AdcSign_sum = (AdcSign_sum>>6);
      
    return AdcSign_sum;
}


int32
tc2101mbReadSnrSum(uint8 port_num, int32 cnt)
{
    int32 snr_sum = 0;
    int j;
    
    for(j=0;j<cnt;j++) {
      snr_sum += tc2101mbReadProbe(port_num,ProbeSnr);
    }
    return snr_sum;
}


void 
tc2101mbDispProbe100(uint8 port_num)
{
#ifdef TCPHY_DEBUG
    const int32 tc2101mbReadSnrCnt = 1000;

    TCPHYDISP2("tcphy[%d]:",port_num);
    TCPHYDISP2(" boosten=%ld", tc2101mbReadProbe(port_num,ProbeBoosten));
    TCPHYDISP2(" agccode=%ld", tc2101mbReadProbe(port_num,ProbeAgccode));
    TCPHYDISP2(" zfgain=%ld", tc2101mbReadProbe(port_num,ProbeZfgain));
    TCPHYDISP2(" snr=%ld", tc2101mbReadProbe(port_num,ProbeSnr));
    TCPHYDISP2(" err_over_sum=%ld", tc2101mbReadErrOverSum(port_num));
    TCPHYDISP2(" \r\n");
    //
    TCPHYDISP3(" snr_sum(x1000)=%ld",tc2101mbReadSnrSum(port_num,tc2101mbReadSnrCnt)); // snr_sum   
    TCPHYDISP4(" adc_avg=%ld/1000", tc2101mbReadAdcSum(port_num));  
    TCPHYDISP3(" \r\n");
#endif
}

void
tc2101mbSwDispMessage(uint8 port_num)
{
    switch (tcphy_link_state[port_num]) {
        case ST_LINK_DOWN:
            if (mr28.lch_sig_detect || mr28.lch_rx_linkpulse
                || mr28.lch_linkup_100 || mr28.lch_linkup_10)
            {
                TCPHYDISP4("tcphy[%d]: ",port_num);
                if (mr28.lch_sig_detect) 
                    TCPHYDISP4("SigDet ");
                if (mr28.lch_rx_linkpulse)
                    TCPHYDISP4("RxLkp ");
                if (mr28.lch_linkup_100)
                    TCPHYDISP4("Up100 ");
                if (mr28.lch_linkup_10)
                    TCPHYDISP4("Up10 ");
                if (mr28.lch_linkup_mdi)
                    TCPHYDISP4("UpMdi ");
                if (mr28.lch_linkup_mdix)
                    TCPHYDISP4("UpMdix ");
                TCPHYDISP4("\r\n");
            }
            else {
                TCPHYDISP6(".");
            }
            break;
            
        case ST_LINK_DOWN2UP: 
            TCPHYDISP1("tcphy[%d]: Link-up at %d%s.\r\n",
                port_num,(mr28.final_speed?100:10),(mr28.final_duplex?"F":"H"));    
            if(mr28.final_speed) {
                tc2101mbDispProbe100(port_num);
            }           
            break;
            
        case ST_LINK_UP: 
            break;
            
        case ST_LINK_UP2DOWN:
            TCPHYDISP1("tcphy[%d]: Link-down!!!\r\n",port_num);
            break;
            
        default: printf("\r\ntcphy: error code!\r\n");
    }
}

// 0:auto, 1:mdi, 2:mdix
void 
tc2101mbForceMdiMode(uint8 idx) 
{
    const uint8 port_num = 0;
    uint32 val;

    val = tcPhyReadReg(port_num,26);
    val &= 0x00003fff; // default: auto-crossover

    if (idx==1) { //  1:mdi mode
        val |= (1<<14);     
    } else if (idx==2){  // 2:mdix mode
        val |= (1<<15);
    } 

    tcPhyWriteReg(port_num,26,val);
}


// sw patch auto PairSwap
void
tc2101mbSwFixPairSwap(void)
{
    const uint8 port_num = 0;
    const uint8 TIMER_THD = 30; // timer threshold to keep Force PairSwap

    if (tc2101mbSwFixAutoPairSwap_state == 0) { // disable
        if (tcphy_link_state[port_num] == ST_LINK_DOWN2UP) {
            tc2101mbSwFixAutoPairSwap_state = 1; // enable
            tc2101mbSwFixAutoPairSwap_timer = TIMER_THD;
            if (mr28.lch_linkup_mdi && !mr28.lch_linkup_mdix){
                tc2101mbForceMdiMode(1); // mdi
                TCPHYDISP3("tc2101mbSwFixPairSwap: Force MDI.\r\n");
            }
            else if (!mr28.lch_linkup_mdi && mr28.lch_linkup_mdix){
                tc2101mbForceMdiMode(2); // mdix
                TCPHYDISP3("tc2101mbSwFixPairSwap: Force MDI-X.\r\n");
            }
        }
    }
    else { // enable
        if (tc2101mbSwFixAutoPairSwap_timer==0){
            tc2101mbSwFixAutoPairSwap_state = 0; // disable
            tc2101mbForceMdiMode(0); // auto-Pairswap
            TCPHYDISP3("tc2101mbSwFixPairSwap: Auto-Crossover.\r\n");
        }
        else {
            tc2101mbSwFixAutoPairSwap_timer--;
        }
    }
}

// set force100 if AN with SigDetect only
// impact: miiw 1 0 2x00 can not works in AnVsF100-linkup
void
tc2101mbSwFixAnVsF100(void)
{
    const uint8 port_num = 0;
    const uint8 TIMER_DISABLE_THD = 1; // timer threshold 
    const uint8 TIMER_FORCE100_THD = 10; // timer threshold 
    // state machine
    const uint8 ST_DISABLE = 0; // default: AN
    const uint8 ST_FORCE100 = 1; // Force-100

    if (tcphy_speed != tcphy_speed_Auto)
        return;

    if (tc2101mbSwFixAnVsF100_state == ST_DISABLE) { // disable 
        if (mr0.autoneg_enable) {
            if (tcphy_link_state[port_num]==ST_LINK_DOWN) {
                if (mr28.lch_sig_detect && !mr28.lch_rx_linkpulse) { // signal detect
                    if (tc2101mbSwFixAnVsF100_timer>=TIMER_DISABLE_THD){
                        tcPhyWriteReg(port_num,0,0x2000); // 100Half
                        tc2101mbSwFixAnVsF100_timer= 0;
                        tc2101mbSwFixAnVsF100_state= ST_FORCE100;
                        TCPHYDISP3("tc2101mbFixAnVsF100: SigDet, Force 100H.\r\n");
                    }
                    else {
                        tc2101mbSwFixAnVsF100_timer++;
                    }
                }               
                else if (!mr28.lch_sig_detect && mr28.lch_rx_linkpulse) {
                    if (tc2101mbSwFixAnVsF100_timer>0) {
                        tc2101mbSwFixAnVsF100_timer--;
                    }
                }
            }
            else if (tcphy_link_state[port_num]==ST_LINK_DOWN2UP) { 
                tc2101mbSwFixAnVsF100_up100_check = 1;
            }
            else if (tcphy_link_state[port_num]==ST_LINK_UP) {
                if (tc2101mbSwFixAnVsF100_up100_check){
                    tcPhyReadReg(port_num,6);
                    if (!mr6.lp_autoneg_able && mr28.final_speed) { // Up via Parallel-100
                        tcPhyWriteReg(port_num,0,0x2000); // 100Half
                        tc2101mbSwFixAnVsF100_timer= 0;
                        tc2101mbSwFixAnVsF100_state= ST_FORCE100;
                        TCPHYDISP3("tc2101mbFixAnVsF100: Parallel-Up100, Force 100H.\r\n");
                    }   
                    tc2101mbSwFixAnVsF100_up100_check=0;
                }
            }
            else if (tcphy_link_state[port_num]==ST_LINK_UP2DOWN) {
                tc2101mbSwFixAnVsF100_timer=0;
            }
        }
    }
    else { // ST_FORCE100
        if (mr0.autoneg_enable) {
            tc2101mbSwFixAnVsF100_timer= 0;
            tc2101mbSwFixAnVsF100_state= ST_DISABLE;            
        }
        else if (tcphy_link_state[port_num] == ST_LINK_DOWN
                && mr28.lch_sig_detect && !mr28.lch_rx_linkpulse)
        {
            tc2101mbSwFixAnVsF100_timer= 0;     
            TCPHYDISP3("tc2101mbFixAnVsF100: restart timer.\r\n");
        }
        else if (tcphy_link_state[port_num] != ST_LINK_UP 
                && tc2101mbSwFixAnVsF100_timer>=TIMER_FORCE100_THD) 
        {
            tcPhyWriteReg(port_num,0,0x1200); // AN + restart-AN
            tc2101mbSwFixAnVsF100_timer= 0;
            tc2101mbSwFixAnVsF100_state= ST_DISABLE;            
            TCPHYDISP3("tc2101mbFixAnVsF100: restart-AN.\r\n");
        }
        else {
            if (tc2101mbSwFixAnVsF100_timer<TIMER_FORCE100_THD) {
                tc2101mbSwFixAnVsF100_timer++;
                //TCPHYDISP3("%d",tc2101mbSwFixAnVsF100_timer);
            }
        }
    }

}


// disable Squelch detect if AN with wrong FLP
// disable Squelch detect before linkup if no UNH IOL concern
void
tc2101mbSwFixFalseSquelch(void)
{
    const uint8 port_num = 0;

    switch (tcphy_link_state[port_num]) {
        case ST_LINK_DOWN: 
            break;
            
        case ST_LINK_DOWN2UP: 
            if (!tc2101mbSwFixFalseSquelch_state 
                && !mr28.final_speed) //10BaseT
            { 
                // ENABLE : turn on squ_min/max_th
                tcPhyWriteGReg(port_num,0,16,tc2101mb_FixFalseSquelch_G0R16_ON);
                TCPHYDISP3("tc2101mbFixFalseSquelch enable.\r\n");
                tc2101mbSwFixFalseSquelch_state = 1;
            }
            break;
            
        case ST_LINK_UP: 
            break;
            
        case ST_LINK_UP2DOWN:
            if (tc2101mbSwFixFalseSquelch_state) {
                tcPhyWriteGReg(port_num,0,16,tc2101mb_FixFalseSquelch_G0R16_OFF);
                TCPHYDISP3("tc2101mbFixFalseSquelch disable.\r\n");         
                tc2101mbSwFixFalseSquelch_state = 0;
            }
            break;
            
        default: printf("\r\ntcphy: error code!\r\n");
    }
}

void
tc2101mbUpdateErrOverSum(void)
{
    const uint8 port_num = 0;
    uint16 err_over_cnt_prev; // to store previous err_over_cnt value
    int32 val;
    
    // clear Nmr25[0].err_over_cnt in tc2101mbDispProbe100() in ST_LINK_DOWN2UP
    if ((mr28.final_speed == 1) // 100BaseTX
        && (tcphy_link_state[port_num] == ST_LINK_UP)) 
    {
        err_over_cnt_prev = Nmr25[0].err_over_cnt;
        tcPhyReadReg(port_num,25);
        val = Nmr25[0].err_over_cnt - err_over_cnt_prev;
        if( val < 0 ){
            val += 2048;
        }
        tc2101mb_erroversum = val;
    }
    else {
        tc2101mb_erroversum = -1;
    }
}

void
tc2101mbSwFixTrcyStepChange(uint8 mode)
{
    const uint8 port_num = 0;

    if (mode == 0) { // reset to default
        tc2101mbSwFixTrcyStep_Mode = 0;
        tcPhyWriteLReg(port_num,1,18,tc2101mb_FixTrcyStep_L1R18_default);
        TCPHYDISP5("tcphy: Change TrcyStep mode 0\r\n");
    }
    else { // next
        // 0 -> 1 -> 2 -> 1 -> 2 -> 1 -> 2 -> ...
        if (tc2101mbSwFixTrcyStep_Mode == 0){
            tc2101mbSwFixTrcyStep_Mode = 1;
            tcPhyWriteLReg(port_num,1,18,tc2101mb_FixTrcyStep_L1R18_mode1);
            TCPHYDISP4("tcphy: Change TrcyStep mode 1\r\n");
        }
        else if (tc2101mbSwFixTrcyStep_Mode == 1){
            tc2101mbSwFixTrcyStep_Mode = 2;
            tcPhyWriteLReg(port_num,1,18,tc2101mb_FixTrcyStep_L1R18_mode2);
            TCPHYDISP4("tcphy: Change TrcyStep mode 2\r\n");
        }
        else if (tc2101mbSwFixTrcyStep_Mode == 2){
            tc2101mbSwFixTrcyStep_Mode = 1;
            tcPhyWriteLReg(port_num,1,18,tc2101mb_FixTrcyStep_L1R18_mode1);
            TCPHYDISP4("tcphy: Change TrcyStep mode 1\r\n");
        }
    }
}


void
tc2101mbSwFixTrcyStep(void)
{
    const uint8 port_num = 0;
    const int16 wait_time = 1; // wait n tick every setting change 
    const int16 freeze_time = 60; // total trcy step training time
    const int16 tc2101mbSwFixTrcyStep_ErrThd = 200;
    const int32 tc2101mbReadSnrCnt = 50;
    
    int32 tc2101mb_snr=0;

    if (tcphy_link_state[port_num] == ST_LINK_UP2DOWN){
        tc2101mbSwFixTrcyStepChange(0);
    }
    else if (mr28.final_speed == 1) {
        if (tcphy_link_state[port_num] == ST_LINK_DOWN2UP) {
            tc2101mbSwFixTrcyStep_WaitTimer= 0;
            tc2101mbSwFixTrcyStep_CheckTimer= 0;
            tc2101mbSwFixTrcyStep_ChangeCnt= 0;
            tc2101mb_snr_avg=0;
            tc2101mb_snr_avg_pre=0;
        }
        else if (tcphy_link_state[port_num] == ST_LINK_UP) {
            if (tc2101mbSwFixTrcyStep_CheckTimer<freeze_time) { // check until freeze time
                if (tc2101mbSwFixTrcyStep_WaitTimer<wait_time) { // check until wait time
                    tc2101mbSwFixTrcyStep_WaitTimer++;
                    tc2101mbSwFixTrcyStep_CheckTimer++;
                    if (tc2101mb_erroversum>tc2101mbSwFixTrcyStep_ErrThd ||
                        tc2101mbSwFixTrcyStep_ChangeCnt>0) 
                    {
                        tc2101mb_snr=tc2101mbReadSnrSum(port_num,tc2101mbReadSnrCnt);
                        tc2101mb_snr_avg+=tc2101mb_snr;
                        #ifdef TCPHY_DEBUG
                            TCPHYDISP4("tcphy: snr_avg=%d\r\n",tc2101mb_snr_avg);
                            TCPHYDISP4("tcphy: snr_avg_pre=%d\r\n",tc2101mb_snr_avg_pre);
                        #endif
                    }   
                    #ifdef TCPHY_DEBUG
                        TCPHYDISP6("tcphy: change cnt=%1d\r\n",tc2101mbSwFixTrcyStep_ChangeCnt);
                    #endif
                    if (tc2101mbSwFixTrcyStep_WaitTimer == wait_time){ // pass wait_time
                        tc2101mbSwFixTrcyStep_WaitTimer= 0;
                        if (tc2101mb_snr_avg>tc2101mb_snr_avg_pre||tc2101mbSwFixTrcyStep_ChangeCnt==0) { // get worse snr
                            tc2101mb_snr_avg_pre = tc2101mb_snr_avg;
                            tc2101mb_snr_avg = 0;
                            if ((tc2101mb_erroversum>tc2101mbSwFixTrcyStep_ErrThd
                                 &&tc2101mbSwFixTrcyStep_ChangeCnt==0)||
                                 tc2101mbSwFixTrcyStep_ChangeCnt>0) 
                            {
                                  tc2101mbSwFixTrcyStepChange(+1);
                                  tc2101mbSwFixTrcyStep_ChangeCnt++;
                            }
                        }
                        else {
                            tc2101mbSwFixTrcyStep_WaitTimer= wait_time;
                            #ifdef TCPHY_DEBUG
                                //TCPHYDISP4("get better snr\r\n");
                                TCPHYDISP4("tcphy: Stop TrcyStep at mode %d\r\n",tc2101mbSwFixTrcyStep_Mode);
                            #endif
                        }
                    }
                }
            }
        }
    }
}

#ifdef TCPHY_DEBUG
void
tc2101mbSwErrOverMonitor(void)
{
#ifndef LINUX_OS
    const uint8 port_num = 0;
    int32 val;
#endif
    if (tcPhy_disp_level<3) 
        return; // inactive

    if (tc2101mb_erroversum>0){
        TCPHYDISP3("tcphy: ErrOver=%1d\r\n",tc2101mb_erroversum);           
    }

}
#endif



// SwPatch enabled by sw_patch_flag,
// read link_status for macGetEtherConnStatus()
void
tc2101mbSwPatch(void)
{
    const uint8 port_num = 0;

    tcPhyReadReg(port_num,1);
    //
    if( !Nmr1[port_num].link_status_prev && !Nmr1[port_num].link_status ){
        tcphy_link_state[port_num] = ST_LINK_DOWN;
    }
    else if( !Nmr1[port_num].link_status_prev && Nmr1[port_num].link_status ){
        tcphy_link_state[port_num] = ST_LINK_DOWN2UP;
    }
    else if( Nmr1[port_num].link_status_prev && !Nmr1[port_num].link_status ){
        tcphy_link_state[port_num] = ST_LINK_UP2DOWN;
    }
    else { //if( Nmr1[port_num].link_status_prev && Nmr1[port_num].link_status ){
        tcphy_link_state[port_num] = ST_LINK_UP;
    }
    
    // read registers
//  if (tcphy_link_state[port_num] != ST_LINK_UP){
        tcPhyReadReg(port_num,0);
        tcPhyReadReg(port_num,28);
//  }
    
    // display message
    tc2101mbSwDispMessage(port_num);
    
    // sw patch to fix False Squelch bug
    if (sw_FixFalseSquelch_flag)
        tc2101mbSwFixFalseSquelch();
    
    // sw patch to fix AnVsF100 bug
    if (sw_FixAnVsF100_flag)
        tc2101mbSwFixAnVsF100();

    if (sw_FixAutoPairSwap_flag)
        tc2101mbSwFixPairSwap();

    if (sw_FixTrcyStep_flag || sw_ErrOverMonitor_flag)
        tc2101mbUpdateErrOverSum();

    if (sw_FixTrcyStep_flag)
        tc2101mbSwFixTrcyStep(); // call after tc2101mbUpdateErrOverSum()
    
#ifdef TCPHY_DEBUG
    if (sw_ErrOverMonitor_flag) 
        tc2101mbSwErrOverMonitor(); // call after tc2101mbUpdateErrOverSum()
#endif
}

#endif // TC2101MB_SUPPORT

/************************************************************************
*
*            Variables and Functions for tc2104mcSwPatch()
*
*************************************************************************
*/
#ifdef TC2104MC_SUPPORT
#define TC2104MC_PHY_INIT_GDATA_LEN 9
#define TC2104MC_PHY_INIT_LDATA_LEN 5
#define TC2104MC_PHY_INIT_SET_NUM 1

typedef struct tc2104mc_cfg_data_s{
    char name[10];
    cfg_data_t gdata[TC2104MC_PHY_INIT_GDATA_LEN];
    cfg_data_t ldata[TC2104MC_PHY_INIT_LDATA_LEN];
}tc2104mc_cfg_data_t;

// G0R22b11: tx10_eee_mode
const uint16 tc2104mc_G0R22_Tx10AmpSave_ON  = 0x0e64;
const uint16 tc2104mc_G0R22_Tx10AmpSave_OFF = 0x066F;
// L0R30b11: bypass_pd_wakeup
// L0R30b3: pd_analog_wakeup
//const uint16 tc2104mc_L0R30_PDWAKEUP_ON = 0x8808;
const uint16 tc2104mc_L0R30_PDWAKEUP_OFF = 0x8800;
const uint16 tc2104mc_L0R30_PDWAKEUP_AUTO = 0x8000;

#define tc2104mc_FixTrcyStep_L1R18_default 0x240a // trgap=16T, trstep=4  --> 1/4
const uint32 tc2104mc_FixTrcyStep_L1R18_mode1 = 0x000f; // trgap=8T, trstep=8  --> 1
const uint32 tc2104mc_FixTrcyStep_L1R18_mode2 = 0x4404; // trgap=32:16T, trstep=2:1  --> 1/16

#define tc2104mc_G3R16_default 0x0018
#define tc2104mc_L0R25_default 0xa000
const uint32 tc2104mc_G3R17_B1 = 0x4683;
const uint32 tc2104mc_G3R20_A1 = 0x0f30;
const uint32 tc2104mc_G3R20_B1 = 0x0e30;


tc2104mc_cfg_data_t tc2104mc_cfg[TC2104MC_PHY_INIT_SET_NUM]={
    {{"C11.5"}, 
     { {31,0x3000},{16,tc2104mc_G3R16_default},{19,0x4f00},{21,0x0706},{22,0x00f4},
       {31,0x1000},{20,0x8914},{24,0x0030},{25,0x0018}
       //{31,0x0000},{22,0x0664},
     },
     { {31,0x9000},{16,0x0000},{18,tc2104mc_FixTrcyStep_L1R18_default},
       {31,0x8000},{25,tc2104mc_L0R25_default}
     }
    }, 
};
// note: The value of L0R25 and G3R16 are also used in tc2104mcInitialRegFlag()
#endif

#ifdef TC2104MC_SUPPORT

static uint8 sw_FixPdWakeup_flag = 1; // default: enable
static uint8 tc2104mc_FixNonAnPairSwap_flag = 1;// default: enable
static uint8 tc2104mc_esdphy_init_flag = 1;// default: enable

// for sw_FixTrcyStep_flag & tc2101mbSwFixTrcyStep()
static uint8 tc2104mcSwFixTrcyStep_Mode[4] = {0,0,0,0};
#if 0
static int16 tc2104mcSwFixTrcyStep_ErrThd_init = 200;
static int16 tc2104mcSwFixTrcyStep_ErrThd[4] = {0,0,0,0};
#endif
static uint8 tc2104mcSwFixTrcyStep_WaitTimer[4] = {0,0,0,0};
static uint8 tc2104mcSwFixTrcyStep_CheckTimer[4] = {0,0,0,0};
static int32 tc2104mc_erroversum=0;
static uint16 tc2104mc_snr_avg[4]={0,0,0,0};
static uint16 tc2104mc_snr_avg_pre[4]={0,0,0,0};
static uint8 tc2104mcSwFixTrcyStep_ChangeCnt[4]= {0,0,0,0};

//ESD detect
static int16 tc2104mc_Up2Down_cnt[4] = {0,0,0,0};
static uint8 tc2104mc_Up2DownFast_detect[4] = {0,0,0,0};
static bool tc2104mc_link_fail_detect_flag = 0;
static bool tc2104mc_reset_needed_flag = 0;

//for sw_FixNonAnPairSwap
static uint8  tc2104mc_force_crossover[4] = {0,0,0,0};
static uint8  tc2104mc_force_crossover_next[4] = {0,0,0,0};
static uint8 tc2104mc_force_crossover_cnt[4] = {0,0,0,0};
static uint8  tc2104mc_writeLR26_flag[4] = {0,0,0,0};
static uint16 tc2104mc_nosig_cnt[4] = {0,0,0,0};


static void tc2104mcConfig(int argc, char *argv[], void *p);
void tc2104mcCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num);
static void tc2104mcLRCfgLoad(uint8 idx, uint8 port_num);
bool tc2104mcInitialRegFlag(void);
int32 tc2104mcReadProbe(uint8 port_num, uint8 mode);
static void tc2104mcCfgTx10AmpSave(void);
#ifdef TCPHY_DEBUG
int32 tc2104mcReadErrOverSum(uint8 port_num);
int32 tc2104mcReadAdcSum(uint8 port_num);
void tc2104mcDispProbe100(uint8 port_num, bool long_msg);
void tc2104mcPoweDown(uint8 port_num);
void tc2104mcSwDispMessage(uint8 port_num);
#endif
uint16 tc2104mcReadSnrSum(uint8 port_num, uint16 cnt);
void tc2104mcSwFixPdWakeup(uint8 port_num);
void tc2104mcUpdateErrOverSum(uint8 port_num);
void tc2104mcSwFixTrcyStepChange(uint8 port_num,uint8 mode);
void tc2104mcSwFixTrcyStep(uint8 port_num);
void tc2104mcUp2DownFast(uint8 port_num);
void tc2104mcSwErrOverMonitor(uint8 port_num);
void tc2104mcMdixForce(uint8 port_num, uint8 mode);
void tc2104mcSwFixNonAnPairSwap(uint8 port_num);
//void tc2104mcSwTx100AmpSave(uint8 port_num);
void tc2104mcSwPatch(void);
bool tc2104mcLinkFailDetect (void);



static void 
tc2104mcConfig(int argc, char *argv[], void *p)
{
    // global config in doPhyConfig()

#ifdef TCPHY_DEBUG
    if (argc==1) {
        TCPHYDISP3("Usage: config Tx10AmpSave [on|off]\r\n");
        //TCPHYDISP3("Usage: config Tx100AmpSave [on|off]\r\n");
        TCPHYDISP3("       config FixPdWakeup [on|off]\r\n");
        TCPHYDISP3("       config FixTrcyStep [on|off]\r\n");
		TCPHYDISP3("       config FixNonAnPairSwap [on|off]\r\n");
		TCPHYDISP3("       config FixUp2DownFast [on|off]\r\n");
		TCPHYDISP3("       config EsdPhyRegInit [on|off]\r\n");
        return;
    }
    else if (argc==2) {
        if (stricmp(argv[1], "list") == 0){
            TCPHYDISP3("config Tx10AmpSave : %s\r\n",
                    (cfg_Tx10AmpSave_flag?"on":"off"));         
            TCPHYDISP3("config FixPdWakeup : %s\r\n",
                    (sw_FixPdWakeup_flag?"on":"off"));          
            TCPHYDISP3("config FixTrcyStep : %s\r\n",
                    (sw_FixTrcyStep_flag?"on":"off"));
			TCPHYDISP3("config FixNonAnPairSwap : %s\r\n",
                    (tc2104mc_FixNonAnPairSwap_flag?"on":"off")); 
			TCPHYDISP3("config FixUp2DownFast : %s\r\n",                    
				    (sw_FixUp2DownFast_flag?"on":"off")); 
			TCPHYDISP3("config EsdPhyRegInit : %s\r\n",                    
				    (tc2104mc_esdphy_init_flag?"on":"off"));
            return;
        }
    }
#endif

    if (argc==2 || argc==3){
        if (stricmp(argv[1], "Tx10AmpSave") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                cfg_Tx10AmpSave_flag = 1; // enable
                tc2104mcCfgTx10AmpSave();
            }
            else if( stricmp(argv[2], "off") == 0 ){
                cfg_Tx10AmpSave_flag = 0;
                tc2104mcCfgTx10AmpSave();
            }
            TCPHYDISP2("config Tx10AmpSave : %s\r\n",
                        (cfg_Tx10AmpSave_flag?"on":"off"));
        }
		#ifdef TCPHY_DEBUG
        else if (stricmp(argv[1], "FixPdWakeup") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                sw_FixPdWakeup_flag = 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                sw_FixPdWakeup_flag = 0;
            }
            TCPHYDISP2("config FixPdWakeup : %s\r\n",
                        (sw_FixPdWakeup_flag?"on":"off"));
        }
        else if (stricmp(argv[1], "FixTrcyStep") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                sw_FixTrcyStep_flag = 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                sw_FixTrcyStep_flag = 0;
            }
            TCPHYDISP2("config FixTrcyStep : %s\r\n",
                        (sw_FixTrcyStep_flag?"on":"off"));
        }
		else if (stricmp(argv[1], "FixUp2DownFast") == 0){
			if( stricmp(argv[2], "on") == 0 ){
				sw_FixUp2DownFast_flag= 1; // enable
				}
			else if( stricmp(argv[2], "off") == 0 ){
				sw_FixUp2DownFast_flag= 0;
				}
			TCPHYDISP2("config FixUp2DownFast : %s\r\n",
				(sw_FixUp2DownFast_flag?"on":"off"));
			}
		else if (stricmp(argv[1], "FixNonAnPairSwap") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                tc2104mc_FixNonAnPairSwap_flag= 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                tc2104mc_FixNonAnPairSwap_flag= 0;
            }
            TCPHYDISP2("config FixNonAnPairSwap : %s\r\n",
                        (tc2104mc_FixNonAnPairSwap_flag?"on":"off"));
        }
		else if (stricmp(argv[1], "EsdPhyRegInit") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                tc2104mc_esdphy_init_flag= 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                tc2104mc_esdphy_init_flag= 0;
            }
            TCPHYDISP2("config EsdPhyRegInit : %s\r\n",
                        (tc2104mc_esdphy_init_flag?"on":"off"));
        }
	    #endif
    }
}

// 1. loading default register setting in tcPhyInit()
// 2. changing register setting
void 
tc2104mcCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num)
{
    int pn, i;
    uint16 phyAddr;
    //uint32 eco_rev;
	current_idx = idx;
	

#ifdef UNH_TEST
        sw_ErrOverMonitor_flag=0;
        sw_FixPdWakeup_flag=0;
        cfg_Tx10AmpSave_flag=0;
		tc2104mc_FixNonAnPairSwap_flag=0;
#endif

    // global registers
    phyAddr = 0; //mac_p->enetPhyAddr;
    //eco_rev = tcMiiStationRead(phyAddr, 31);
    //eco_rev &= (0x0f);
    if (eco_rev != 0x00) { // turn off PdWakeup patched if eco_rev>0
        sw_FixPdWakeup_flag=0;	
	}

    for( i=0; i<TC2104MC_PHY_INIT_GDATA_LEN; i++ ){         
        tcMiiStationWrite(phyAddr, tc2104mc_cfg[current_idx].gdata[i].reg_num, tc2104mc_cfg[current_idx].gdata[i].val );
    }
    //     
    tc2104mcCfgTx10AmpSave();

    // local registers 
    if (doPerPort_flag == DO_PER_PORT) {
		tc2104mcLRCfgLoad(current_idx, port_num);
    	}
	else {
		 for( pn=0; pn<TCPHY_PORTNUM; pn++){
		 	phyAddr = pn; //mac_p->enetPhyAddr + pn;
		 	tc2104mcLRCfgLoad(current_idx, phyAddr);
			}
		 }
	// load revision-related settings
	
    if (eco_rev == 0x00) { //rev=A1
		tcPhyWriteGReg(phyAddr,3,20,tc2104mc_G3R20_A1);
    } 
	else { //rev=B1
		tcPhyWriteGReg(phyAddr,3,17,tc2104mc_G3R17_B1);
		tcPhyWriteGReg(phyAddr,3,20,tc2104mc_G3R20_B1);
	}

    TCPHYDISP4("tcphy: CfgLoad %s\r\n",  tc2104mc_cfg[current_idx].name);

#ifdef UNH_TEST
    phyAddr = 0;
    //tcPhyWriteGReg(phyAddr,0,22,0x0664); // disable tx10_eee_mode
    //tcPhyWriteGReg(phyAddr,0,26,0x0012); // disable cr_signal_status_opt
    tcPhyWriteGReg(phyAddr,1,22,0x0012); // enable RX_ER extension to Pass PCS pause test

	if (doPerPort_flag == DO_PER_PORT) {
		tcPhyWriteLReg(port_num,0,30,0x0800); // disable AAPS to avoid varaince lc_max 
        tcPhyWriteLReg(port_num,0,26,0x1201); // disable Link_Det_Ext function
		}
	else{
		for( pn=0; pn<TCPHY_PORTNUM; pn++){
			phyAddr = pn; //mac_p->enetPhyAddr + pn;
            tcPhyWriteLReg(phyAddr,0,30,0x0800); // disable AAPS to avoid varaince lc_max 
            tcPhyWriteLReg(phyAddr,0,26,0x1201); // disable Link_Det_Ext function
            }
		}
    TCPHYDISP4("tcphy: CfgLoad UNH-IOL setting\r\n");
#endif
}

static void
tc2104mcLRCfgLoad(uint8 idx, uint8 port_num)
{
    int i;
	current_idx = idx;
	for( i=0; i<TC2104MC_PHY_INIT_LDATA_LEN; i++ ){         
            tcMiiStationWrite(port_num, tc2104mc_cfg[current_idx].ldata[i].reg_num, tc2104mc_cfg[current_idx].ldata[i].val );
        }
        if (sw_FixPdWakeup_flag) {
            tcPhyWriteLReg(port_num,0,30,tc2104mc_L0R30_PDWAKEUP_OFF);
            TCPHYDISP5("tcphy[%d]: PdWakeup OFF mode!\r\n",port_num);
        }   
}


static void
tc2104mcCfgTx10AmpSave(void)
{
    uint16 phyAddr = mac_p->enetPhyAddr;

    if (cfg_Tx10AmpSave_flag==1){ // enable
        tcPhyWriteGReg(phyAddr,0,22,tc2104mc_G0R22_Tx10AmpSave_ON);
        TCPHYDISP4("tcphy: Tx10AmpSave enable!\r\n");       
    }
    else { // disable
        tcPhyWriteGReg(phyAddr,0,22,tc2104mc_G0R22_Tx10AmpSave_OFF);
        TCPHYDISP4("tcphy: Tx10AmpSave disable!\r\n");      
    }
}

bool 
tc2104mcInitialRegFlag(void)
{
    bool initial_value_flag=1; // default: correct initial value
    uint8 pn;
    uint16 rval;

    // check G3R16
    rval = tcPhyReadGReg(0,3,16);
    if (rval != tc2104mc_G3R16_default){
        initial_value_flag=0; // wrong initial value
    }
    // check L0R25
    for(pn=0;pn<TCPHY_PORTNUM;pn++){
        rval = tcPhyReadReg(pn,25);
        if ((rval&0xf000)!=tc2104mc_L0R25_default){
            initial_value_flag=0; // wrong initial value
        }
    }
    return (initial_value_flag);
}


int32
tc2104mcReadProbe(uint8 port_num, uint8 mode)
{
    uint32 val, val_r31, val_g0r28;
    uint32 rval, wval;
    uint32 phyaddr = mac_p->enetPhyAddr + port_num;
    
    val_r31 = tcMiiStationRead(phyaddr, 31); 
    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    val_g0r28 = tcMiiStationRead(phyaddr, 28);
  
    switch(mode){
        case ProbeZfgain:
            wval = 0x0b04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x3f ;
            break;
        case ProbeAgccode:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>1)&0x0f ;
            break;
        case ProbeBoosten:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>5)&0x01 ;
            break;
        break;
        case ProbeSnr:
            wval = 0x0904 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0xff ;
            break;  
        case ProbeAdcSign:
            wval = 0x4104 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x7f ;
            if (rval>64){
                rval -= 128;
            }
            break;
        default:
            TCPHYDISP1("\r\ntcphy error: ReadProbe %d.\r\n",mode);
            rval = 0;
            break;
    }

    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    tcMiiStationWrite( phyaddr, 28, val_g0r28); // restore G0Reg28
    tcMiiStationWrite( phyaddr, 31, val_r31); // restore Reg31
    return rval;

}

#ifdef TCPHY_DEBUG
int32
tc2104mcReadErrOverSum(uint8 port_num)
{
    int32 err_over_sum;
    
    tcPhyReadReg(port_num,25);
    pause(300);
    tcPhyReadReg(port_num,25);
    err_over_sum = Nmr25[port_num].err_over_cnt - Nmr25[port_num].err_over_cnt_prev;
    if( err_over_sum < 0 ){
        err_over_sum += 2048;
    }

    return err_over_sum;
}
#endif

#ifdef TCPHY_DEBUG
int32
tc2104mcReadAdcSum(uint8 port_num)
{
    int32 cnt=1000;
    int32 AdcSign_sum = 0;
    int j;
      
    for(j=0;j<cnt;j++){
        AdcSign_sum += tc2104mcReadProbe(port_num,ProbeAdcSign);
    }
    //shift right to show percent of the dc offset (unit:%)
    AdcSign_sum = (AdcSign_sum>>6);
      
    return AdcSign_sum;
}

#endif

uint16
tc2104mcReadSnrSum(uint8 port_num, uint16 cnt)
{
    uint16 snr_sum = 0;
    uint16 j;
    
    for(j=0;j<cnt;j++) {
      snr_sum += tc2104mcReadProbe(port_num,ProbeSnr);
    }
    return snr_sum;

}

#ifdef TCPHY_DEBUG
void 
tc2104mcDispProbe100(uint8 port_num, bool long_msg)
{
    const uint16 tc2104mcReadSnrCnt = 1000;

    TCPHYDISP2("tcphy[%d]:",port_num);
    TCPHYDISP2(" boosten=%ld", tc2104mcReadProbe(port_num,ProbeBoosten));
    TCPHYDISP2(" agccode=%ld", tc2104mcReadProbe(port_num,ProbeAgccode));
    TCPHYDISP2(" zfgain=%ld", tc2104mcReadProbe(port_num,ProbeZfgain));
    TCPHYDISP2(" ch_idx=%ld", (tcPhyReadReg(port_num,29)&0x003f));
    TCPHYDISP2(" snr=%ld", tc2104mcReadProbe(port_num,ProbeSnr));
    if (long_msg){
        TCPHYDISP2(" err_over_sum=%ld", tc2104mcReadErrOverSum(port_num));
    }
    else {
        tcPhyReadReg(port_num,25);
    }
    TCPHYDISP2(" \r\n");
    if (long_msg){
        TCPHYDISP3(" snr_sum(x1000)=%d",tc2104mcReadSnrSum(port_num,tc2104mcReadSnrCnt)); // snr_sum   
        TCPHYDISP4(" adc_avg=%ld/1000", tc2104mcReadAdcSum(port_num));  
        TCPHYDISP3(" \r\n");
    }
}
#endif

#ifdef TCPHY_DEBUG
void
tc2104mcSwDispMessage(uint8 port_num)
{
    switch (tcphy_link_state[port_num]) {
        case ST_LINK_DOWN: 
            break;
            
        case ST_LINK_DOWN2UP: 
            //tcPhyReadReg(port_num,28); read in tc2104mcSwPatch()
            TCPHYDISP1("tcphy[%d]: Link-up at %d%s.",
                port_num,(mr28.final_speed?100:10),(mr28.final_duplex?"F":"H")); 
			
			TCPHYDISP1(" tx_amp_save=%d.",(mr28.tx_amp_save));          
            TCPHYDISP1("\r\n");
			
            if(mr28.final_speed) {
                tc2104mcDispProbe100(port_num,0); // short message
            }
			
            break;
            
        case ST_LINK_UP: 
            break;
            
        case ST_LINK_UP2DOWN:
            TCPHYDISP1("tcphy[%d]: Link-down!!!\r\n",port_num);
            break;
            
        default: printf("\r\ntcphy error: SwDispMessage error!\r\n");
    }
}
#endif

void
tc2104mcSwFixPdWakeup(uint8 port_num)
{
    switch (tcphy_link_state[port_num]) {
        case ST_LINK_DOWN: 
            break;
            
        case ST_LINK_DOWN2UP: 
            tcPhyWriteLReg(port_num,0,30,tc2104mc_L0R30_PDWAKEUP_AUTO);
            TCPHYDISP5("tcphy[%d]: PdWakeup AUTO mode!\r\n",port_num);
            break;
            
        case ST_LINK_UP: 
            break;
            
        case ST_LINK_UP2DOWN:
            tcPhyWriteLReg(port_num,0,30,tc2104mc_L0R30_PDWAKEUP_OFF);
            TCPHYDISP5("tcphy[%d]: PdWakeup OFF mode!\r\n",port_num);
            break;
    }
}


void
tc2104mcUpdateErrOverSum(uint8 port_num)
{
    uint16 err_over_cnt_prev; // to store previous err_over_cnt value
    int32 val;
    
    // clear Nmr25[port_num].err_over_cnt in tc2104mcDispProbe100() in ST_LINK_DOWN2UP
    if ((mr28.final_speed == 1) // 100BaseTX
        && (tcphy_link_state[port_num] == ST_LINK_UP)) 
    {
        err_over_cnt_prev = Nmr25[port_num].err_over_cnt;
        tcPhyReadReg(port_num,25);
        val = Nmr25[port_num].err_over_cnt - err_over_cnt_prev;
        if( val < 0 ){
            val += 2048;
        }
        tc2104mc_erroversum = val;
    }
    else {
        tc2104mc_erroversum = -1;
    }
}


void
tc2104mcSwFixTrcyStepChange(uint8 port_num,uint8 mode)
{
    if (mode == 0) { // reset to default
        tc2104mcSwFixTrcyStep_Mode[port_num] = 0;
        tcPhyWriteLReg(port_num,1,18,tc2104mc_FixTrcyStep_L1R18_default);
        TCPHYDISP5("tcphy[%d]: Change TrcyStep mode 0\r\n",port_num);
    }
    else { // next +1
        // 0 -> 1 -> 2 -> 1 -> 2 -> 1 -> 2 -> ...
        if (tc2104mcSwFixTrcyStep_Mode[port_num] == 0){
            tc2104mcSwFixTrcyStep_Mode[port_num] = 1;
            tcPhyWriteLReg(port_num,1,18,tc2104mc_FixTrcyStep_L1R18_mode1);
            TCPHYDISP4("tcphy[%d]: Change TrcyStep mode 1\r\n",port_num);
        }
        else if (tc2104mcSwFixTrcyStep_Mode[port_num] == 1){
            tc2104mcSwFixTrcyStep_Mode[port_num] = 2;
            tcPhyWriteLReg(port_num,1,18,tc2104mc_FixTrcyStep_L1R18_mode2);
            TCPHYDISP4("tcphy[%d]: Change TrcyStep mode 2\r\n",port_num);
        }
        else if (tc2104mcSwFixTrcyStep_Mode[port_num] == 2){
            tc2104mcSwFixTrcyStep_Mode[port_num] = 1;
            tcPhyWriteLReg(port_num,1,18,tc2104mc_FixTrcyStep_L1R18_mode1);
            TCPHYDISP4("tcphy[%d]: Change TrcyStep mode 1\r\n",port_num);
        }
    }
}


void
tc2104mcSwFixTrcyStep(uint8 port_num)
{
    const uint8 wait_time = 1; // wait n tick every setting change 
    const uint8 freeze_time = 60; // total trcy step training time
    const uint16 tc2104mcSwFixTrcyStep_ErrThd = 200;
    const uint8 tc2104mcReadSnrCnt = 50;
    
    uint16 tc2104mc_snr=0;

    if (tcphy_link_state[port_num] == ST_LINK_UP2DOWN){
        tc2104mcSwFixTrcyStepChange(port_num,0);
    }
    else if (mr28.final_speed == 1) {
        if (tcphy_link_state[port_num] == ST_LINK_DOWN2UP) {
            tc2104mcSwFixTrcyStep_WaitTimer[port_num]= 0;
            tc2104mcSwFixTrcyStep_CheckTimer[port_num]= 0;
            tc2104mcSwFixTrcyStep_ChangeCnt[port_num] = 0;
            tc2104mc_snr_avg[port_num]=0;
            tc2104mc_snr_avg_pre[port_num]=0;
        }
        else if (tcphy_link_state[port_num] == ST_LINK_UP) {
            if (tc2104mcSwFixTrcyStep_CheckTimer[port_num]<freeze_time) { // check until freeze time
                if (tc2104mcSwFixTrcyStep_WaitTimer[port_num]<wait_time) { // check until wait time
                    tc2104mcSwFixTrcyStep_CheckTimer[port_num]++;
                    tc2104mcSwFixTrcyStep_WaitTimer[port_num]++;
                    if (tc2104mc_erroversum>tc2104mcSwFixTrcyStep_ErrThd||
                        tc2104mcSwFixTrcyStep_ChangeCnt[port_num]>0) {
                        tc2104mc_snr=tc2104mcReadSnrSum(port_num,tc2104mcReadSnrCnt);
                        tc2104mc_snr_avg[port_num]+=tc2104mc_snr;
                        #ifdef TCPHY_DEBUG
                        TCPHYDISP4("tcphy[%d]: snr_avg=%d\r\n",port_num,tc2104mc_snr_avg[port_num]);
                        TCPHYDISP4("tcphy[%d]: snr_avg_pre=%d\r\n",port_num,tc2104mc_snr_avg_pre[port_num]);
                        #endif
                    }
                    #ifdef TCPHY_DEBUG
                    TCPHYDISP6("tcphy[%d]: change cnt=%1d\r\n",port_num,tc2104mcSwFixTrcyStep_ChangeCnt[port_num]);
                    #endif
                    if (tc2104mcSwFixTrcyStep_WaitTimer[port_num]==wait_time){ // pass wait_time
                            tc2104mcSwFixTrcyStep_WaitTimer[port_num]= 0;
                        if (tc2104mc_snr_avg[port_num]>tc2104mc_snr_avg_pre[port_num]||tc2104mcSwFixTrcyStep_ChangeCnt[port_num]==0) { // get worse snr
                            tc2104mc_snr_avg_pre[port_num] = tc2104mc_snr_avg[port_num];
                            tc2104mc_snr_avg[port_num]= 0;
                            if ((tc2104mc_erroversum>tc2104mcSwFixTrcyStep_ErrThd
                                 &&tc2104mcSwFixTrcyStep_ChangeCnt[port_num]==0)||
                                 tc2104mcSwFixTrcyStep_ChangeCnt[port_num]>0) 
                            {
                                      tc2104mcSwFixTrcyStepChange(port_num,+1);
                                      tc2104mcSwFixTrcyStep_ChangeCnt[port_num]++;
                            }
                        }
                        else {
                            tc2104mcSwFixTrcyStep_WaitTimer[port_num]= wait_time;
                            #ifdef TCPHY_DEBUG
                            //TCPHYDISP4("get better snr\r\n");
                            TCPHYDISP4("tcphy[%d]: Stop TrcyStep at mode %d\r\n",port_num,tc2104mcSwFixTrcyStep_Mode[port_num]);
                            #endif
                        }
                    }
                }
            }
        }
    }
}

void 
tc2104mcUp2DownFast(uint8 port_num)
{
	const uint8 tc2104mc_Up2Down_Thd = 10;
	const uint8 tc2104mc_Up2Down_ub = 40;

    if (mr28.lch_linkup_100 & !Nmr1[port_num].link_status) {
		if (tc2104mc_Up2Down_cnt[port_num]<tc2104mc_Up2Down_ub) {
			tc2104mc_Up2Down_cnt[port_num]+=3;
			}
    	}
	else if (!mr28.lch_linkup_100 & !Nmr1[port_num].link_status) {
		if (tc2104mc_Up2Down_cnt[port_num]>0) {
			tc2104mc_Up2Down_cnt[port_num]--;
			}
		}
	else if (mr28.lch_linkup_100 & Nmr1[port_num].link_status) {
		tc2104mc_Up2Down_cnt[port_num]=0;
		}
	if (tc2104mc_Up2Down_cnt[port_num]> tc2104mc_Up2Down_Thd) {
		tc2104mc_Up2DownFast_detect[port_num]=1;
		tc2104mc_Up2Down_cnt[port_num]=0;
		}
	else {
		tc2104mc_Up2DownFast_detect[port_num]=0;
		}
}

bool 
tc2104mcLinkFailDetect (void){
	if (sw_FixUp2DownFast_flag &(tc2104mc_Up2DownFast_detect[0]||tc2104mc_Up2DownFast_detect[1]
		||tc2104mc_Up2DownFast_detect[2]||tc2104mc_Up2DownFast_detect[3])) {
		tc2104mc_link_fail_detect_flag =1;
		#ifdef TCPHY_DEBUG
		//TCPHYDISP3(" tc2104mc_link_fail_detect_flag=%ld\r\n",tc2104mc_link_fail_detect_flag); 
		#endif
		}
	else {
		tc2104mc_link_fail_detect_flag =0;
	}
	return tc2104mc_link_fail_detect_flag;
}

void 
tc2104mcMdixForce(uint8 port_num, uint8 mode)
{
    int32 val;
	val = tcPhyReadLReg(port_num, 0, 26);
	val &= 0x00003fff; // 0:default: auto-crossover
	if (mode==1) {// 1 : mdi mode, 01
		val |= (1<<14);
	    TCPHYDISP5("tcphy[%d]: force crossover mode to mdi\r\n",port_num);
		}
	else if (mode==2) {// 2: mdix mode, 10
	    val |= (1<<15);
		TCPHYDISP5("tcphy[%d]: force crossover mode to mdix\r\n",port_num);
		}
	tcPhyWriteLReg(port_num,0,26,val);
	tc2104mc_force_crossover[port_num] = mode;
	//tc2104mc_nosig_cnt[port_num]=0;
}


void
tc2104mcSwFixNonAnPairSwap(uint8 port_num)
{
    uint8 lr_any_signal = 0;
	
	
    if (tcphy_link_state[port_num] == ST_LINK_UP2DOWN){
		tc2104mc_force_crossover_cnt[port_num] = 0;
		tc2104mcMdixForce(port_num, 0); //default: auto-crossover
		tc2104mc_nosig_cnt[port_num]=0;
    }
    else if (tcphy_link_state[port_num] == ST_LINK_DOWN){
		tcPhyReadReg(port_num,0);
		if ((mr0.autoneg_enable == 0)&&(mr0.force_speed == 1)) { // force-100
			
			tc2104mc_writeLR26_flag[port_num] = 1;
			lr_any_signal = (mr28.lch_sig_detect | mr28.lch_rx_linkpulse | mr28.lch_linkup_100 | mr28.lch_linkup_10);
            if (lr_any_signal)
               tc2104mc_nosig_cnt[port_num]=0;
            else if (!lr_any_signal && tc2104mc_nosig_cnt[port_num]<30)
                tc2104mc_nosig_cnt[port_num]++;
			
			if (tc2104mc_force_crossover[port_num]==0) {
				tc2104mc_force_crossover_cnt[port_num] = 0;
				if (mr28.mdix_status==1){
					tc2104mcMdixForce(port_num, 2); // keep mdix
					tc2104mc_nosig_cnt[port_num]=0;
					tc2104mc_force_crossover_next[port_num] = 1;
			    }
			    else {
					tc2104mcMdixForce(port_num, 1); // keep mdi
					tc2104mc_nosig_cnt[port_num]=0;
			        tc2104mc_force_crossover_next[port_num] = 2;
			    }
			} //force_crossover[port_num]==0			
			else {
				if (tc2104mc_force_crossover_cnt[port_num] <10) {
					tc2104mc_force_crossover_cnt[port_num]++;
				}

				if (tc2104mc_force_crossover_cnt[port_num]>1 && tc2104mc_nosig_cnt[port_num]>1){
					tc2104mc_force_crossover_cnt[port_num]=0;
			        tc2104mcMdixForce(port_num, tc2104mc_force_crossover_next[port_num]);
					tc2104mc_nosig_cnt[port_num]=0;
			        //if (tc2104mc_force_crossover_next[port_num] !=0){
				        tc2104mc_force_crossover_next[port_num] = 0;
			        //}
		        }
			}//force_crossover[port_num]!=0
	     }//if force100
	     else if (tc2104mc_writeLR26_flag[port_num]){
	 		tc2104mcMdixForce(port_num, 0);; //default: auto-crossover
			tc2104mc_nosig_cnt[port_num]=0;
		 	tc2104mc_writeLR26_flag[port_num] = 0;
		}
    }// if (tcphy_link_state[port_num] == ST_LINK_DOWN)    	
    	
}

#ifdef TCPHY_DEBUG
void
tc2104mcPoweDown(uint8 port_num)
{
    uint32 val_l0r04;
	val_l0r04 = tcPhyReadLReg(port_num, 0, 4);
	tcPhyWriteLReg(port_num,0,4,0x0001); // 	Set FLP register to disable all capability
    tcPhyWriteLReg(port_num,0,0,0x1200); //  restart-AN
    tcPhyWriteLReg(port_num,0,0,0x1800); //  power-down
    tcPhyWriteLReg(port_num,0,4,val_l0r04); //  Set FLP register to its default value
}
#endif 


#ifdef TCPHY_DEBUG
void
tc2104mcSwErrOverMonitor(uint8 port_num)
{
    if (tcPhy_disp_level<3) 
        return; // inactive

    if (tc2104mc_erroversum>0){
        TCPHYDISP3("tcphy[%d]: ErrOver=%ld\r\n",port_num,tc2104mc_erroversum);          
    }

}
#endif

void
tc2104mcSwPatch(void)
{
    uint8 pn;

    for(pn=0;pn<TCPHY_PORTNUM;pn++){

        tcPhyReadReg(pn,1);
        //
        if( !Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_DOWN;
        }
        else if( !Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_DOWN2UP;
        }
        else if( Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            tcphy_link_state[pn]= ST_LINK_UP2DOWN;
        }
        else { //if( Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_UP;
        }
    
        // read registers
        // tcPhyReadReg(pn,0);
        tcPhyReadReg(pn,28);

		//ESD reset needed 
		if (sw_FixUp2DownFast_flag)
			tc2104mcUp2DownFast(pn);
		
        #ifdef TCPHY_DEBUG
        // display message
        tc2104mcSwDispMessage(pn);
		#endif

        if (sw_FixPdWakeup_flag)
            tc2104mcSwFixPdWakeup(pn);

        if (sw_FixTrcyStep_flag || sw_ErrOverMonitor_flag)
            tc2104mcUpdateErrOverSum(pn);

        if (sw_FixTrcyStep_flag)
            tc2104mcSwFixTrcyStep(pn); // call after tc2104mcUpdateErrOverSum()

	    if (tc2104mc_FixNonAnPairSwap_flag)
			tc2104mcSwFixNonAnPairSwap(pn);
    
#ifdef TCPHY_DEBUG
        if (sw_ErrOverMonitor_flag) 
            tc2104mcSwErrOverMonitor(pn); // call after tc2104mcUpdateErrOverSum()
#endif

    }
    
}

#endif // TC2104MC_SUPPORT

/************************************************************************
*
*            Variables and Functions for tc2104sdSwPatch()
*
*************************************************************************
*/
#ifdef TC2104SD_SUPPORT
#define TC2104SD_PHY_INIT_GDATA_LEN 15
#define TC2104SD_PHY_INIT_LDATA_LEN 5
#define TC2104SD_PHY_INIT_SET_NUM 1

typedef struct tc2104sd_cfg_data_s{
    char name[10];
    cfg_data_t gdata[TC2104SD_PHY_INIT_GDATA_LEN];
    cfg_data_t ldata[TC2104SD_PHY_INIT_LDATA_LEN];
}tc2104sd_cfg_data_t;

tc2104sd_cfg_data_t tc2104sd_cfg[TC2104SD_PHY_INIT_SET_NUM]={
    { {"D6.12"}, // name
      { // global
        {31,0x3000}, {16,0x0018}, {21,0x0746}, {23,0xab40}, {24,0xa201},
        {31,0x2000}, {30,0x9ab5},
        {31,0x1000}, {17,0xe5ed}, {18,0x1eef}, {24,0x0030}, {25,0x0018}, {27,0x0016},
        {31,0x0000}, {22,0x0664}
      },
      { // local
        {31,0x9000}, {16,0x0002}, 
        {31,0x8000}, {25,0xc000}, {30,0x8800}
      }
    }, 
};
#endif

#ifdef TC2104SD_SUPPORT
static void tc2104sdConfig(int argc, char *argv[], void *p);
static void tc2104sdCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num);
static void tc2104sdLRCfgLoad(uint8 idx, uint8 port_num);
static void tc2104sdPortCfgLoad(uint8 idx, uint8 port_num);
int32 tc2104sdReadProbe(uint8 port_num, uint8 mode);
int32 tc2104sdReadErrOverSum(uint8 port_num);
int32 tc2104sdReadAdcSum(uint8 port_num);
int32 tc2104sdReadSnrSum1000(uint8 port_num);
void tc2104sdDispProbe100(uint8 port_num, bool long_msg);
void tc2104sdSwErrOverMonitor(uint8 port_num);
void tc2104sdSwDispMessage(uint8 port_num);
void tc2104sdSwPatch(void);


static void 
tc2104sdConfig(int argc, char *argv[], void *p)
{
    // global config in doPhyConfig()

#ifdef TCPHY_DEBUG
    if (argc==1) {
        // null
        return;
    }
    else if (argc==2 && tcPhy_disp_level>2) {
        // null now
        return;
    }
#endif
}

// 1. loading default register setting in tcPhyInit()
// 2. changing register setting
static void 
tc2104sdCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num)
{
    int pn, i;
    uint8 phyAddr;
    
    current_idx = idx;
    // global registers
    phyAddr = mac_p->enetPhyAddr;
    for( i=0; i<TC2104SD_PHY_INIT_GDATA_LEN; i++ ){         
        tcMiiStationWrite(phyAddr, tc2104sd_cfg[current_idx].gdata[i].reg_num, tc2104sd_cfg[current_idx].gdata[i].val );
    }
    // local registers 
    if (doPerPort_flag == DO_PER_PORT) {
		phyAddr = mac_p->enetPhyAddr + port_num;
		tc2104sdLRCfgLoad(current_idx, phyAddr);
    	}
	else{
		for( pn=0; pn<TCPHY_PORTNUM; pn++){
			phyAddr = mac_p->enetPhyAddr + pn;
		    tc2104sdLRCfgLoad(current_idx, phyAddr);
			}
		}
		TCPHYDISP4("tcphy: CfgLoad %s\r\n",  tc2104sd_cfg[current_idx].name);
}



static void 
tc2104sdLRCfgLoad(uint8 idx, uint8 port_num)
{
	int i;
	current_idx = idx;
	for( i=0; i<TC2104SD_PHY_INIT_LDATA_LEN; i++ ){         
            tcMiiStationWrite(port_num, tc2104sd_cfg[current_idx].ldata[i].reg_num, tc2104sd_cfg[current_idx].ldata[i].val );
        }
}
	

int32
tc2104sdReadProbe(uint8 port_num, uint8 mode)
{
    uint32 val, val_r31, val_g0r25, val_g0r28;
    uint32 rval, wval;
    uint32 phyaddr = mac_p->enetPhyAddr + port_num;
    
    val_r31 = tcMiiStationRead(phyaddr, 31); 
    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    val_g0r28 = tcMiiStationRead(phyaddr, 28);
  
    switch(mode){
        case ProbeZfgain:
            wval = 0x0b04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x3f ;
            break;
        case ProbeAgccode:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>1)&0x1f ; // 5-bits
            break;
        case ProbeBoosten:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>6)&0x01 ;
            break;
        break;
        case ProbeSnr:
            wval = 0x0904 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0xff ;
            break;  
        case ProbeAdcSign:
            wval = 0x4104 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x7f ;
            if (rval>64){
                rval -= 128;
            }
            break;
        default:
            TCPHYDISP1("\r\ntcphy error: ReadProbe %d.\r\n",mode);
            rval = 0;
            break;
    }

    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    tcMiiStationWrite( phyaddr, 28, val_g0r28); // restore G0Reg28
    tcMiiStationWrite( phyaddr, 31, val_r31); // restore Reg31
    return rval;

}


int32
tc2104sdReadErrOverSum(uint8 port_num)
{
    int32 err_over_sum;
    
    tcPhyReadReg(port_num,25);
    pause(300);
    tcPhyReadReg(port_num,25);
    err_over_sum = Nmr25[port_num].err_over_cnt - Nmr25[port_num].err_over_cnt_prev;
    if( err_over_sum < 0 ){
        err_over_sum += 2048;
    }

    return err_over_sum;
}


int32
tc2104sdReadAdcSum(uint8 port_num)
{
    int32 cnt=1000;
    int32 AdcSign_sum = 0;
    int j;
      
    for(j=0;j<cnt;j++){
        AdcSign_sum += tc2104sdReadProbe(port_num,ProbeAdcSign);
    }
    //shift right to show percent of the dc offset (unit:%)
    AdcSign_sum = (AdcSign_sum>>6);
      
    return AdcSign_sum;
}


int32
tc2104sdReadSnrSum(uint8 port_num)
{
    int32 cnt=1000;
    int32 snr_sum = 0;
    int j;
    
    for(j=0;j<cnt;j++) {
      snr_sum += tc2104sdReadProbe(port_num,ProbeSnr);
    }
    return snr_sum;
}

void 
tc2104sdDispProbe100(uint8 port_num, bool long_msg)
{
#ifdef TCPHY_DEBUG
    TCPHYDISP2("tcphy[%d]:",port_num);
    TCPHYDISP2(" boosten=%d", tc2104sdReadProbe(port_num,ProbeBoosten));
    TCPHYDISP2(" agccode=%d", tc2104sdReadProbe(port_num,ProbeAgccode));
    TCPHYDISP2(" zfgain=%d", tc2104sdReadProbe(port_num,ProbeZfgain));
    TCPHYDISP2(" ch_idx=%d", (tcPhyReadReg(port_num,29)&0x003f));
    TCPHYDISP2(" snr=%d", tc2104sdReadProbe(port_num,ProbeSnr));
    if (long_msg) {
        TCPHYDISP2(" err_over_sum=%d", tc2104sdReadErrOverSum(port_num));
    }
    else {
        tcPhyReadReg(port_num,25);
    }
    TCPHYDISP2(" \r\n");
    if (long_msg) {
        TCPHYDISP3(" snr_sum(x1000)=%d",tc2104sdReadSnrSum(port_num)); // snr_sum   
        TCPHYDISP4(" adc_avg=%d/1000", tc2104sdReadAdcSum(port_num));   
        TCPHYDISP3(" \r\n");
    }
#endif
}

#ifdef TCPHY_DEBUG
void
tc2104sdSwErrOverMonitor(uint8 port_num)
{
    int32 val;

    // clear mr25[*].err_over_cnt in tc2101mbDispProbe100() in ST_LINK_DOWN2UP

    if ((mr28.final_speed == 1) // 100BaseTX
        && ((tcphy_link_state[port_num] == ST_LINK_UP) 
            || (tcphy_link_state[port_num] == ST_LINK_UP2DOWN)))
    {
        tcPhyReadReg(port_num,25);
        if (Nmr25[port_num].err_over_cnt != Nmr25[port_num].err_over_cnt_prev){
            val = Nmr25[port_num].err_over_cnt - Nmr25[port_num].err_over_cnt_prev;
            if( val < 0 ){
                val += 2048;
            }
            TCPHYDISP3("tcphy[%d]: ErrOver=%d\r\n",port_num,val);           
        }       
    }
}
#endif

void
tc2104sdSwDispMessage(uint8 port_num)
{
    switch (tcphy_link_state[port_num]) {
        case ST_LINK_DOWN: 
            break;
            
        case ST_LINK_DOWN2UP: 
            //tcPhyReadReg(port_num,28);
            TCPHYDISP1("tcphy[%d]: Link-up at %d%s.",
                port_num,(mr28.final_speed?100:10),(mr28.final_duplex?"F":"H"));    
            TCPHYDISP1(" tx_amp_save=%d.",(mr28.tx_amp_save));          
            TCPHYDISP1("\r\n");
            if(mr28.final_speed) {
                tc2104sdDispProbe100(port_num,0);
            }           
            break;
            
        case ST_LINK_UP: 
            break;
            
        case ST_LINK_UP2DOWN:
            TCPHYDISP1("tcphy[%d]: Link-down!!!\r\n",port_num);
            break;
            
        default: printf("\r\ntcphy error: SwDispMessage error!\r\n");
    }
}

void
tc2104sdSwPatch(void)
{
    uint8 pn;

    for(pn=0;pn<TCPHY_PORTNUM;pn++){

        tcPhyReadReg(pn,1);
        // if PHY power-down
        if (Nmr1[pn].value == 0xffff)
            return;
        //
        if( !Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_DOWN;
        }
        else if( !Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_DOWN2UP;
        }
        else if( Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            tcphy_link_state[pn]= ST_LINK_UP2DOWN;
        }
        else { //if( Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_UP;
        }
    
        // read registers
        // tcPhyReadReg(pn,0);
        tcPhyReadReg(pn,28);
    
        // display message
        tc2104sdSwDispMessage(pn);

#ifdef TCPHY_DEBUG
        if (sw_ErrOverMonitor_flag) 
            tc2104sdSwErrOverMonitor(pn);
#endif
    }
    
}


#endif // TC2104SD_SUPPORT


/************************************************************************
*
*            Variables and Functions for tc2101meSwPatch()
*
*************************************************************************
*/

#ifdef TC2101ME_SUPPORT
//*** TC2101ME register settting ***//
#define TC2101ME_PHY_INIT_DATA_LEN 13
#define TC2101ME_PHY_INIT_SET_NUM 1

typedef struct tc2101me_cfg_data_s{
    char name[10];
    cfg_data_t data[TC2101ME_PHY_INIT_DATA_LEN];
}tc2101me_cfg_data_t;

// G0R22b9: tx10_eee_mode
const uint16 tc2101me_G0R22_Tx10AmpSave_ON  = 0x0264;
const uint16 tc2101me_G0R22_Tx10AmpSave_OFF = 0x0064;
const uint32 tc2101me_L3R17_EEEAdv = 0x0002;


tc2101me_cfg_data_t tc2101me_cfg[TC2101ME_PHY_INIT_SET_NUM]={
    {{"E5.2"}, 
	{{31,0x3000}, {17,0x7238}, {21,0x7247},
	 {31,0x1000}, {20,0x0914}, {25,0x0018}, {27,0x0125},
     {31,0x0000}, {23,0x0006},
	 {31,0x9000}, {22,0x0140}, {23,0x0400}, {24,0x4447}}}, 
};
#endif

#ifdef TC2101ME_SUPPORT

static uint8 tc2101me_FixNonAnPairSwap_flag = 1;// default: enable

//for sw_FixNonAnPairSwapstatic 
uint8  tc2101me_force_crossover = 0;
static uint8  tc2101me_force_crossover_next = 0;
static uint16 tc2101me_force_crossover_cnt = 0;
static uint8  tc2101me_writeLR26_flag = 0;
static uint16 tc2101me_nosig_cnt = 0;

static int16 tc2101me_Up2Down_cnt_hbt = 0;
static uint8 tc2101me_Up2DownFastHbt_detect = 0;
static bool tc2101me_link_fail_detect_flag = 0;
//Disable EEE if link fail
static int16 tc2101me_Up2Down_cnt_tbt = 0;
static uint8 tc2101me_Up2DownFastTbt_detect = 0;
static uint8 tc2101me_linkup_check_timer = 0;
static bool tc2101meDisEEEHappened = 0;
static uint8 tc2101me_linkup_check_cnt = 0;

static uint8 tc2101meDisNextPage_cnt = 0;
static bool tc2101meDisNextPage_flag = 0;


static void tc2101meConfig(int argc, char *argv[], void *p);
static void tc2101meCfgLoad(uint8 idx);
int32 tc2101meReadProbe(uint8 port_num, uint8 mode);
int32 tc2101meReadErrOverSum(uint8 port_num);
int32 tc2101meReadAdcSum(uint8 port_num);
int32 tc2101meReadSnrSum(uint8 port_num);
void tc2101meDispProbe100(uint8 port_num);

static void tc2101meCfgTx10AmpSave(void);
void tc2101meSwErrOverMonitor(void);
void tc2101meSwDispMessage(uint8 port_num);
void tc2101meMdixForce(uint8 mode);
void tc2101meSwFixNonAnPairSwap(void);
//Auto-Disable-NextPage patch 
void tc2101meUp2DownFastHbt(uint8 port_num);
void tc2101meUp2DownFastTbt(uint8 port_num);
bool tc2101meLinkFailDetect (uint8 port_num, uint8 mode);//0:10BT or 100BT, 1:100BT only, 2:10BT only
void tc2101meDisEEENeeded(uint8 port_num);
void tc2101meDisNextPageNeeded(uint8 port_num);

void tc2101meSwPatch(void);

static void 
tc2101meConfig(int argc, char *argv[], void *p)
{
    // global config in doPhyConfig()

#ifdef TCPHY_DEBUG
    if (argc==1) {
		TCPHYDISP3("Usage: config Tx10AmpSave [on|off]\r\n");
        TCPHYDISP3("       config FixNonAnPairSwap [on|off]\r\n");
		TCPHYDISP3("       config FixUp2DownFast [on|off]\r\n");
        return;
    }
    else if (argc==2) {
        if (stricmp(argv[1], "list") == 0){
			TCPHYDISP3("config Tx10AmpSave : %s\r\n",
                    (cfg_Tx10AmpSave_flag?"on":"off"));
            TCPHYDISP3("config FixNonAnPairSwap : %s\r\n",
                    (tc2101me_FixNonAnPairSwap_flag?"on":"off")); 
			TCPHYDISP3("config FixUp2DownFast : %s\r\n",
                    (sw_FixUp2DownFast_flag?"on":"off"));
            return;
        }
    }
#endif
if (argc==2 || argc==3){
	    if (stricmp(argv[1], "Tx10AmpSave") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                cfg_Tx10AmpSave_flag = 1; // enable
                tc2101meCfgTx10AmpSave();
            }
            else if( stricmp(argv[2], "off") == 0 ){
                cfg_Tx10AmpSave_flag = 0;
                tc2101meCfgTx10AmpSave();
            }
            TCPHYDISP2("config Tx10AmpSave : %s\r\n",
                        (cfg_Tx10AmpSave_flag?"on":"off"));
        }
        else if (stricmp(argv[1], "FixNonAnPairSwap") == 0){
			if( stricmp(argv[2], "on") == 0 ){
                tc2101me_FixNonAnPairSwap_flag= 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                tc2101me_FixNonAnPairSwap_flag= 0;
            }
            TCPHYDISP2("config FixNonAnPairSwap : %s\r\n",
                        (tc2101me_FixNonAnPairSwap_flag?"on":"off"));
        }
		else if (stricmp(argv[1], "FixUp2DownFast") == 0){
			if( stricmp(argv[2], "on") == 0 ){
                sw_FixUp2DownFast_flag= 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                sw_FixUp2DownFast_flag= 0;
            }
            TCPHYDISP2("config FixUp2DownFast : %s\r\n",
                        (sw_FixUp2DownFast_flag?"on":"off"));
        }
  }
}


// 1. loading default register setting in tcPhyInit()
// 2. changing register setting
static void 
tc2101meCfgLoad(uint8 idx)
{
    int i;
    current_idx = idx;
  for( i=0; i<TC2101ME_PHY_INIT_DATA_LEN; i++ ){          
      miiStationWrite( mac_p, tc2101me_cfg[current_idx].data[i].reg_num, tc2101me_cfg[current_idx].data[i].val );
  }
  TCPHYDISP3("tcphy: CfgLoad %s\r\n",  tc2101me_cfg[current_idx].name);
  //TCPHYDISP3("tcphy: Cfg Ver: %s\r\n",  tc2101me_cfg[current_idx].name);
  
  #if defined UNH_TEST
    sw_ErrOverMonitor_flag=0;
    cfg_Tx10AmpSave_flag=0; // disable tx10_eee_mode
    tc2101me_FixNonAnPairSwap_flag=0;
	sw_FixUp2DownFast_flag=0;
	tc2101meCfgTx10AmpSave();
    
    tcPhyWriteGReg(0,1,22,0x0012); // enable RX_ER extension to Pass PCS pause test
    tcPhyWriteLReg(0,0,30,0x0000); // disable AAPS to avoid varaince lc_max 
    tcPhyWriteLReg(0,0,26,0x1201); // disable Link_Det_Ext function
    TCPHYDISP4("tcphy: CfgLoad UNH-IOL setting\r\n");
  #endif
  
}


int32
tc2101meReadProbe(uint8 port_num, uint8 mode)
{
    uint32 val, val_r31, val_g0r28;
    uint32 rval, wval;
    uint32 phyaddr = mac_p->enetPhyAddr + port_num;
    
    val_r31 = tcMiiStationRead(phyaddr, 31); 
    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    val_g0r28 = tcMiiStationRead(phyaddr, 28);
  
    switch(mode){
        case ProbeZfgain:
            wval = 0x0b04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x3f ;
            break;
        case ProbeAgccode:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>1)&0x1f ;
            break;
        case ProbeBoosten:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>6)&0x01 ;
            break;
        break;
        case ProbeSnr:
            wval = 0x0904 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0xff ;
            break;  
        case ProbeAdcSign:
            wval = 0x4104 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x7f ;
            if (rval>64){
                rval -= 128;
            }
            break;
        default:
            TCPHYDISP1("\r\ntcphy error: ReadProbe %d.\r\n",mode);
            rval = 0;
            break;
    }

    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    tcMiiStationWrite( phyaddr, 28, val_g0r28); // restore G0Reg28
    tcMiiStationWrite( phyaddr, 31, val_r31); // restore Reg31
    return rval;

}


int32
tc2101meReadErrOverSum(uint8 port_num)
{
    int32 err_over_sum;
    
    tcPhyReadReg(port_num,25);
    pause(300);
    tcPhyReadReg(port_num,25);
    err_over_sum = Nmr25[port_num].err_over_cnt - Nmr25[port_num].err_over_cnt_prev;
    if( err_over_sum < 0 ){
        err_over_sum += 2048;
    }

    return err_over_sum;
}


int32
tc2101meReadAdcSum(uint8 port_num)
{
    int32 cnt=1000;
    int32 AdcSign_sum = 0;
    int j;
      
    for(j=0;j<cnt;j++){
        AdcSign_sum += tc2101meReadProbe(port_num,ProbeAdcSign);
    }
    //shift right to show percent of the dc offset (unit:%)
    AdcSign_sum = (AdcSign_sum>>6);
      
    return AdcSign_sum;
}


int32
tc2101meReadSnrSum(uint8 port_num)
{
    int32 cnt=1000;
    int32 snr_sum = 0;
    int j;
    
    for(j=0;j<cnt;j++) {
      snr_sum += tc2101meReadProbe(port_num,ProbeSnr);
    }
    return snr_sum;
}


void 
tc2101meDispProbe100(uint8 port_num)
{
#ifdef TCPHY_DEBUG
    TCPHYDISP2("tcphy[%d]:",port_num);
	TCPHYDISP2(" boosten=%ld", tc2101meReadProbe(port_num,ProbeBoosten));
	TCPHYDISP2(" agccode=%ld", tc2101meReadProbe(port_num,ProbeAgccode));
	TCPHYDISP2(" zfgain=%ld", tc2101meReadProbe(port_num,ProbeZfgain));
	TCPHYDISP2(" ch_idx=%ld", (tcPhyReadReg(port_num,29)&0x003f));
	TCPHYDISP2(" snr=%ld", tc2101meReadProbe(port_num,ProbeSnr));
	TCPHYDISP2(" err_over_sum=%ld", tc2101meReadErrOverSum(port_num));
    TCPHYDISP2(" \r\n");
	TCPHYDISP3(" snr_sum(x1000)=%ld",tc2101meReadSnrSum(port_num)); // snr_sum	
	TCPHYDISP4(" adc_avg=%ld/1000", tc2101meReadAdcSum(port_num));	
    TCPHYDISP3(" \r\n");
#endif
}

static void
tc2101meCfgTx10AmpSave(void)
{

    if (cfg_Tx10AmpSave_flag==1){ // enable
        tcPhyWriteGReg(0,0,22,tc2101me_G0R22_Tx10AmpSave_ON);
        TCPHYDISP4("tcphy: Tx10AmpSave enable!\r\n");       
    }
    else { // disable
        tcPhyWriteGReg(0,0,22,tc2101me_G0R22_Tx10AmpSave_OFF);
        TCPHYDISP4("tcphy: Tx10AmpSave disable!\r\n");      
    }
}

#ifdef TCPHY_DEBUG
void
tc2101meSwErrOverMonitor(void)
{
    const uint8 port_num = 0;
    uint16 err_over_cnt_prev;
    int32 val;

    // clear Nmr25[0].err_over_cnt in tc2101mbDispProbe100() in ST_LINK_DOWN2UP

    if ((mr28.final_speed == 1) // 100BaseTX
        && ((tcphy_link_state[port_num] == ST_LINK_UP) 
            || (tcphy_link_state[port_num] == ST_LINK_UP2DOWN)))
    {
        err_over_cnt_prev = Nmr25[port_num].err_over_cnt;
        tcPhyReadReg(port_num,25);
        val = Nmr25[port_num].err_over_cnt - err_over_cnt_prev;
        if (val != 0){
            if( val < 0 ){
                val += 2048;
            }           
			TCPHYDISP3("tcphy: ErrOver=%ld\r\n",val);			
        }       
    }
}
#endif

void
tc2101meSwDispMessage(uint8 port_num)
{
    switch (tcphy_link_state[port_num]) {
        case ST_LINK_DOWN: 
            break;
            
        case ST_LINK_DOWN2UP: 
            tcPhyReadReg(port_num,28);
            TCPHYDISP1("tcphy[%d]: Link-up at %d%s.",
                port_num,(mr28.final_speed?100:10),(mr28.final_duplex?"F":"H"));    
            TCPHYDISP1(" tx_amp_save=%d.",(mr28.tx_amp_save));          
            TCPHYDISP1("\r\n");
            if(mr28.final_speed) {
                tc2101meDispProbe100(port_num);
            }           
            break;
            
        case ST_LINK_UP: 
            break;
            
        case ST_LINK_UP2DOWN:
            TCPHYDISP1("tcphy[%d]: Link-down!!!\r\n",port_num);
            break;

#ifdef TCPHY_DEBUG
        default: printf("\r\ntcphy error: SwDispMessage error!\r\n");           
#endif
    }
}

void 
tc2101meMdixForce(uint8 mode)
{
    const uint8 port_num = 0;
	int32 val;
	val = tcPhyReadLReg(port_num, 0, 26);
	val &= 0x00003fff; // 0:default: auto-crossover
	if (mode==1) {// 1 : mdi mode, 01
		val |= (1<<14);
	    TCPHYDISP5("tcphy[%d]: force crossover mode to mdi\r\n",port_num);
		}
	else if (mode==2) {// 2: mdix mode, 10
	    val |= (1<<15);
		TCPHYDISP5("tcphy[%d]: force crossover mode to mdix\r\n",port_num);
		}
	tcPhyWriteLReg(port_num,0,26,val);
	tc2101me_force_crossover = mode;;
	//tc2104mc_nosig_cnt[port_num]=0;
}


void
tc2101meSwFixNonAnPairSwap(void)
{
    const uint8 port_num = 0;
    uint8 lr_any_signal = 0;
	
	
    if (tcphy_link_state[port_num] == ST_LINK_UP2DOWN){
		tcPhyReadReg(port_num,0);
		if ((mr0.autoneg_enable == 0)&&(mr0.force_speed == 1)) { // force-100
		    tc2101me_force_crossover_cnt = 0;
		    tc2101meMdixForce(0); //default: auto-crossover
		    tc2101me_nosig_cnt=0;
			}
    }
    else if (tcphy_link_state[port_num] == ST_LINK_DOWN){
		tcPhyReadReg(port_num,0);
		if ((mr0.autoneg_enable == 0)&&(mr0.force_speed == 1)) { // force-100
			
			tc2101me_writeLR26_flag = 1;
			lr_any_signal = (mr28.lch_sig_detect | mr28.lch_rx_linkpulse | mr28.lch_linkup_100 | mr28.lch_linkup_10);
            if (lr_any_signal)
               tc2101me_nosig_cnt=0;
            else if (!lr_any_signal && tc2101me_nosig_cnt<30)
                tc2101me_nosig_cnt++;
			
			if (tc2101me_force_crossover==0) {
				tc2101me_force_crossover_cnt = 0;
				if (mr28.mdix_status==1){
					tc2101meMdixForce(2); // keep mdix
					tc2101me_nosig_cnt=0;
					tc2101me_force_crossover_next = 1;
			    }
			    else {
					tc2101meMdixForce(1); // keep mdi
					tc2101me_nosig_cnt=0;
			        tc2101me_force_crossover_next = 2;
			    }
			} //force_crossover[port_num]==0			
			else {
				if (tc2101me_force_crossover_cnt <10) {
					tc2101me_force_crossover_cnt++;
				}

				if (tc2101me_force_crossover_cnt>1 && tc2101me_nosig_cnt>1){
					tc2101me_force_crossover_cnt=0;
			        tc2101meMdixForce(tc2101me_force_crossover_next);
					tc2101me_nosig_cnt=0;
				    tc2101me_force_crossover_next = 0;
		        }
			}//force_crossover[port_num]!=0
	     }//if force100
	     else if (tc2101me_writeLR26_flag){
	 		tc2101meMdixForce(0);; //default: auto-crossover
			tc2101me_nosig_cnt=0;
		 	tc2101me_writeLR26_flag = 0;
		}
    }// if (tcphy_link_state[port_num] == ST_LINK_DOWN)    	
    	
}



void 
tc2101meUp2DownFastHbt(uint8 port_num)
{
	const uint8 tc2101me_Up2Down_Hbt_Thd = 10;
	const uint8 tc2101me_Up2Down_Hbt_ub = 40;
    if (mr28.lch_linkup_100 & !Nmr1[port_num].link_status) {
		if (tc2101me_Up2Down_cnt_hbt<tc2101me_Up2Down_Hbt_ub) {
			tc2101me_Up2Down_cnt_hbt+=3;
			}
    	}
	else if (!mr28.lch_linkup_100 & !Nmr1[port_num].link_status) {
		if (tc2101me_Up2Down_cnt_hbt>0) {
			tc2101me_Up2Down_cnt_hbt--;
			}
		}
	else if (mr28.lch_linkup_100 & Nmr1[port_num].link_status) {//adjust link-up time to clear counter
		tc2101me_Up2Down_cnt_hbt=0;
		}
	if (tc2101me_Up2Down_cnt_hbt> tc2101me_Up2Down_Hbt_Thd) {
		tc2101me_Up2DownFastHbt_detect=1;
		tc2101me_Up2Down_cnt_hbt=0;
		}
	else {
		tc2101me_Up2DownFastHbt_detect=0;
		}  
}

void 
tc2101meUp2DownFastTbt(uint8 port_num)
{
	const uint8 tc2101me_Up2Down_Tbt_Thd = 10;
	const uint8 tc2101me_Up2Down_Tbt_ub = 40;
	const uint8 linkup_check_timer_done = 1;
    if (mr28.lch_linkup_10 & !Nmr1[port_num].link_status) {
		tc2101me_linkup_check_timer=0;
		if (tc2101me_Up2Down_cnt_tbt<tc2101me_Up2Down_Tbt_ub) {
			tc2101me_Up2Down_cnt_tbt+=5;
			}
    	}
	else if (!mr28.lch_linkup_10 & !Nmr1[port_num].link_status) {
		tc2101me_linkup_check_timer=0;
		if (tc2101me_Up2Down_cnt_tbt>0) {
			tc2101me_Up2Down_cnt_tbt--;
			}
		}
	else if (mr28.lch_linkup_10 & Nmr1[port_num].link_status ) {
		if ((tc2101me_linkup_check_timer == linkup_check_timer_done)) {
			tc2101me_Up2Down_cnt_tbt=0;
			tc2101me_linkup_check_timer=0;
			}
		else if ((tc2101me_linkup_check_timer < linkup_check_timer_done))
			tc2101me_linkup_check_timer++;
		    tc2101me_Up2Down_cnt_tbt=0;
		}
	
	if (tc2101me_Up2Down_cnt_tbt> tc2101me_Up2Down_Tbt_Thd) {
		tc2101me_Up2DownFastTbt_detect=1;
		}
	else {
		tc2101me_Up2DownFastTbt_detect=0;
		}
	
}


bool 
tc2101meLinkFailDetect (uint8 port_num, uint8 mode){
	switch(mode){
			case TbtOrHbt:
				    if (sw_FixUp2DownFast_flag 
						&(tc2101me_Up2DownFastTbt_detect
						||tc2101me_Up2DownFastHbt_detect)) {
						tc2101me_link_fail_detect_flag =1;
		                #ifdef TCPHY_DEBUG
		                TCPHYDISP3(" tc2101me_link_fail_detect_flag=%d\r\n",tc2101me_link_fail_detect_flag); 
		                #endif
						}
					else 
						tc2101me_link_fail_detect_flag =0;
				break;
			case HbtOnly:
					if (sw_FixUp2DownFast_flag & tc2101me_Up2DownFastHbt_detect) {
						tc2101me_link_fail_detect_flag =1;
		                #ifdef TCPHY_DEBUG
		                TCPHYDISP3(" tc2101me_link_fail_detect_flag=%d\r\n",tc2101me_link_fail_detect_flag); 
		                #endif
						}
					else 
						tc2101me_link_fail_detect_flag =0;
				break;
			case TbtOnly:
				    if (sw_FixUp2DownFast_flag & tc2101me_Up2DownFastTbt_detect) {
						tc2101me_link_fail_detect_flag =1;
		                #ifdef TCPHY_DEBUG
		                TCPHYDISP3(" tc2101me_link_fail_detect_flag=%d\r\n",tc2101me_link_fail_detect_flag); 
		                #endif
						}
					else 
						tc2101me_link_fail_detect_flag =0;
				break;
			default: tc2101me_link_fail_detect_flag =0;
		}
	return tc2101me_link_fail_detect_flag;
}

void 
tc2101meDisNextPageNeeded(uint8 port_num)
{
   const uint8 tc2101meDisNextPage_cntdone = 5;
   
   if((tcphy_link_state[port_num] == ST_LINK_UP)||(tc2101meDisEEEHappened)) {
   	tc2101meDisNextPage_cnt = 0;
   	}
   else if ((mr5.LPNextAble) && !(mrl3_18.lp_eee_100||mrl3_18.lp_eee_1000||mrl3_18.lp_eee_10g)
   	&& (tc2101meDisNextPage_cnt<=tc2101meDisNextPage_cntdone)) {
   	tc2101meDisNextPage_cnt++;
   	}

   if(tc2101meDisNextPage_cnt>tc2101meDisNextPage_cntdone) {
   	tc2101meDisNextPage_flag=1;
	tc2101meDisNextPage_cnt = 0;
   	}
   else {
   	tc2101meDisNextPage_flag=0;
   	}
}


void 
tc2101meDisEEENeeded(uint8 port_num)
{
    const uint8 tc2101me_linkup_check_done = 1;
	
	
    if((tcphy_link_state[port_num] == ST_LINK_UP)
		&& (tc2101me_linkup_check_cnt<tc2101me_linkup_check_done)) {
		    tc2101me_linkup_check_cnt++;
    	}
	else if (tcphy_link_state[port_num] != ST_LINK_UP){
		  tc2101me_linkup_check_cnt = 0;
    	}
	
    if(tc2101meDisEEEHappened && tc2101me_linkup_check_cnt==tc2101me_linkup_check_done) {//extend link-up time
		tcPhyWriteLReg(port_num,3,17,tc2101me_L3R17_EEEAdv);
		tc2101meDisEEEHappened=0;
    	}
	else if(tc2101meLinkFailDetect(port_num,0) || tc2101meDisNextPage_flag) {
		tcPhyWriteLReg(port_num,3,17,0x0000);//disable next page
		tcPhyWriteLReg(port_num,0,0,0x1200);//re-start AN
		tc2101meDisEEEHappened=1;
    	}
	//TCPHYDISP3("tc2104me_linkup_check_cnt[%d]=%d\r\n",port_num,tc2104me_linkup_check_cnt[port_num]);
	//TCPHYDISP3("tc2104meDisEEEHappened[%d]=%d\r\n",port_num,tc2104meDisEEEHappened[port_num]);
}




void
tc2101meSwPatch(void)
{

    uint8 port_num = 0;

    tcPhyReadReg(port_num,1);
    //
    if( !Nmr1[0].link_status_prev && !Nmr1[0].link_status ){
        tcphy_link_state[port_num] = ST_LINK_DOWN;
    }
    else if( !Nmr1[0].link_status_prev && Nmr1[0].link_status ){
        tcphy_link_state[port_num] = ST_LINK_DOWN2UP;
    }
    else if( Nmr1[0].link_status_prev && !Nmr1[0].link_status ){
        tcphy_link_state[port_num] = ST_LINK_UP2DOWN;
    }
    else { //if( Nmr1[0].link_status_prev && Nmr1[0].link_status ){
        tcphy_link_state[port_num] = ST_LINK_UP;
    }
    
    // read registers
    // tcPhyReadReg(port_num,0);
    tcPhyReadReg(port_num,5);
    tcPhyReadReg(port_num,28);
    tcPhyReadLReg(port_num,3,18);

	if (sw_FixUp2DownFast_flag) {
			tc2101meDisNextPageNeeded(port_num);
			tc2101meUp2DownFastHbt(port_num);
			tc2101meUp2DownFastTbt(port_num);
			tc2101meDisEEENeeded(port_num);
			}
    
    // display message
    tc2101meSwDispMessage(port_num);

	if (tc2101me_FixNonAnPairSwap_flag)
			tc2101meSwFixNonAnPairSwap();
    
#ifdef TCPHY_DEBUG
    if (sw_ErrOverMonitor_flag) 
        tc2101meSwErrOverMonitor();
#endif
    
}

#endif // TC2101ME_SUPPORT

/************************************************************************
*
*            Variables and Functions for tc2102meSwPatch()
*
*************************************************************************
*/
#ifdef TC2102ME_SUPPORT
#define TC2102ME_PHY_INIT_GDATA_LEN 9
#define TC2102ME_PHY_INIT_LDATA_LEN 7
#define TC2102ME_PHY_INIT_SET_NUM 1

typedef struct tc2102me_cfg_data_s{
    char name[10];
    cfg_data_t gdata[TC2102ME_PHY_INIT_GDATA_LEN];
    cfg_data_t ldata[TC2102ME_PHY_INIT_LDATA_LEN];
}tc2102me_cfg_data_t;

const uint16 tc2102me_G0R22_Tx10AmpSave_ON  = 0x0264;
const uint16 tc2102me_G0R22_Tx10AmpSave_OFF = 0x006F;
const uint32 tc2102me_L3R17_EEEAdv = 0x0002;

#define tc2102me_G3R17_default 0x7238
#define tc2102me_L0R25_default 0xa000



tc2102me_cfg_data_t tc2102me_cfg[TC2102ME_PHY_INIT_SET_NUM]={
    {{"G3.3"},
     { {31,0x3000},{17,tc2102me_G3R17_default},{21,0x7107},
       {31,0x1000},{20,0x0914},{25,0x0018},{27,0x0125},
       {31,0x0000},{23,0x0006}
     },
     { {31,0x9000},{16,0x0001},{22,0x0140},{23,0x0400},{24,0x4447},
       {31,0x8000},{25,tc2102me_L0R25_default}
     }
    }, 
};
#endif

#ifdef TC2102ME_SUPPORT

#ifdef TCPHY_DEBUG
static int32 tc2102me_erroversum=0;
#endif

//ESD detect
//static int16 tc2102me_Up2Down_cnt = 0;
//static uint8 tc2102me_Up2DownFast_detect = 0;
//static bool tc2102me_link_fail_detect_flag = 0;

static int16 tc2102me_Up2Down_cnt_hbt = 0;
static uint8 tc2102me_Up2DownFastHbt_detect = 0;
static bool tc2102me_link_fail_detect_flag = 0;
//Disable EEE if link fail
static int16 tc2102me_Up2Down_cnt_tbt = 0;
static uint8 tc2102me_Up2DownFastTbt_detect = 0;
static uint8 tc2102me_linkup_check_timer = 0;
static bool tc2102meDisEEEHappened = 0;
static uint8 tc2102me_linkup_check_cnt = 0;

static uint8 tc2102meDisNextPage_cnt = 0;
static bool tc2102meDisNextPage_flag = 0;

//static uint8 tc2102me_reset_needed_flag = 0;
static uint8 tc2102me_esdphy_init_flag = 1; // default: enable

static void tc2102meConfig(int argc, char *argv[], void *p);
void tc2102meCfgLoad(uint8 idx);
static void tc2102meLRCfgLoad(uint8 idx, uint8 port_num);
int32 tc2102meReadProbe(uint8 port_num, uint8 mode);
static void tc2102meCfgTx10AmpSave(void);
static void tc2102meCfgLDPS(void);
#ifdef TCPHY_DEBUG
int32 tc2102meReadErrOverSum(uint8 port_num);
int32 tc2102meReadAdcSum(uint8 port_num);
void tc2102meDispProbe100(uint8 port_num, bool long_msg);
void tc2102mePoweDown(uint8 port_num);
void tc2102meSwDispMessage(uint8 port_num);
int32 tc2102meReadSnrSum(uint8 port_num, uint16 cnt);
void tc2102meUpdateErrOverSum(uint8 port_num);
void tc2102meSwErrOverMonitor(uint8 port_num);
bool tc2102meInitialRegFlag(void);
void tc2102meEsdSwPatch(void);
#endif
//void tc2102meUp2DownFast(uint8 port_num);
//bool tc2102meLinkFailDetect (void);

void tc2102meUp2DownFastHbt(uint8 port_num);
void tc2102meUp2DownFastTbt(uint8 port_num);
bool tc2102meLinkFailDetect (uint8 port_num, uint8 mode);//0:10BT or 100BT, 1:100BT only, 2:10BT only
void tc2102meDisEEENeeded(uint8 port_num);
void tc2102meDisNextPageNeeded(uint8 port_num);

void tc2102meSwPatch(void);

const int32 phy_start_addr=8;


static void 
tc2102meConfig(int argc, char *argv[], void *p)
{
    // global config in doPhyConfig()
#ifdef TCPHY_DEBUG
    if (argc==1) {
        TCPHYDISP3("Usage: config Tx10AmpSave [on|off]\r\n");
		TCPHYDISP3("       config EsdPhyRegInit [on|off]\r\n");
		TCPHYDISP3("       config FixUp2DownFast [on|off]\r\n");
		TCPHYDISP3("       config LinkDownPS [on|off]\r\n");
        return;
    }
    else if (argc==2) {
        if (stricmp(argv[1], "list") == 0){
            TCPHYDISP3("config Tx10AmpSave : %s\r\n",
                    (cfg_Tx10AmpSave_flag?"on":"off"));  
			TCPHYDISP3("config EsdPhyRegInit : %s\r\n",
                    (tc2102me_esdphy_init_flag?"on":"off"));  
			TCPHYDISP3("config FixUp2DownFast : %s\r\n",
                    (sw_FixUp2DownFast_flag?"on":"off"));
			TCPHYDISP3("config LinkDownPS : %s\r\n",
                    (cfg_LDPS_flag?"on":"off"));
            return;
        }
    }
#endif

    if (argc==2 || argc==3){
        if (stricmp(argv[1], "Tx10AmpSave") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                cfg_Tx10AmpSave_flag = 1; // enable
                tc2102meCfgTx10AmpSave();
            }
            else if( stricmp(argv[2], "off") == 0 ){
                cfg_Tx10AmpSave_flag = 0;
                tc2102meCfgTx10AmpSave();
            }
            TCPHYDISP2("config Tx10AmpSave : %s\r\n",
                        (cfg_Tx10AmpSave_flag?"on":"off"));
        }
		if (stricmp(argv[1], "EsdPhyRegInit") == 0) {
			if( stricmp(argv[2], "on") == 0 ){
                tc2102me_esdphy_init_flag= 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                tc2102me_esdphy_init_flag= 0;
            }
            TCPHYDISP2("config EsdPhyRegInit : %s\r\n",
                        (tc2102me_esdphy_init_flag?"on":"off"));
		}
		if (stricmp(argv[1], "FixUp2DownFast") == 0) {
			if( stricmp(argv[2], "on") == 0 ){
                sw_FixUp2DownFast_flag= 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                sw_FixUp2DownFast_flag= 0;
            }
            TCPHYDISP2("config FixUp2DownFast : %s\r\n",
                        (sw_FixUp2DownFast_flag?"on":"off"));
		}
	   if (stricmp(argv[1], "LinkDownPS") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                cfg_LDPS_flag = 1; // enable
                tc2102meCfgLDPS();
            }
             else if( stricmp(argv[2], "off") == 0 ){
                cfg_LDPS_flag = 0; // disable
                tc2102meCfgLDPS();
            }
            TCPHYDISP2("config LinkDownPS : %s\r\n",
                        (cfg_LDPS_flag?"on":"off"));
        }
    }
}

// 1. loading default register setting in tcPhyInit()
// 2. changing register setting
void 
tc2102meCfgLoad(uint8 idx)
{
    int i;
    uint16 phyAddr=mac_p->enetPhyAddr;
	current_idx = idx;
	

    // global registers
    //phyAddr = idx; //mac_p->enetPhyAddr;

    for( i=0; i<TC2102ME_PHY_INIT_GDATA_LEN; i++ ){         
        tcMiiStationWrite(phyAddr, tc2102me_cfg[current_idx].gdata[i].reg_num, tc2102me_cfg[current_idx].gdata[i].val );
    }
    //     
    tc2102meCfgTx10AmpSave();

    // local registers 
    tc2102meLRCfgLoad(current_idx, phyAddr);
	

    TCPHYDISP4("tcphy: CfgLoad %s\r\n",  tc2102me_cfg[current_idx].name);

#ifdef UNH_TEST
    //tcPhyWriteGReg(phyAddr,0,22,0x0664); // disable tx10_eee_mode
    //tcPhyWriteGReg(phyAddr,0,26,0x0012); // disable cr_signal_status_opt
    sw_FixUp2DownFast_flag= 0;
	cfg_Tx10AmpSave_flag=0; // disable tx10_eee_mode
	tc2102meCfgTx10AmpSave();
    tcPhyWriteGReg(phyAddr,1,22,0x0012); // enable RX_ER extension to Pass PCS pause test

    tcPhyWriteLReg(phyAddr,0,30,0x0800); // disable AAPS to avoid varaince lc_max 
    tcPhyWriteLReg(phyAddr,0,26,0x1200); // disable Link_Det_Ext function
    TCPHYDISP4("tcphy: CfgLoad UNH-IOL setting\r\n");
#endif
}

static void
tc2102meLRCfgLoad(uint8 idx, uint8 port_num)
{
    int i;
	current_idx = idx;
	for( i=0; i<TC2102ME_PHY_INIT_LDATA_LEN; i++ ){         
            tcMiiStationWrite(port_num, tc2102me_cfg[current_idx].ldata[i].reg_num, tc2102me_cfg[current_idx].ldata[i].val );
        }   
}



static void
tc2102meCfgTx10AmpSave(void)
{
    uint16 phyAddr = mac_p->enetPhyAddr;

    if (cfg_Tx10AmpSave_flag==1){ // enable
        tcPhyWriteGReg(phyAddr,0,22,tc2102me_G0R22_Tx10AmpSave_ON);
        TCPHYDISP4("tcphy: Tx10AmpSave enable!\r\n");       
    }
    else { // disable
        tcPhyWriteGReg(phyAddr,0,22,tc2102me_G0R22_Tx10AmpSave_OFF);
        TCPHYDISP4("tcphy: Tx10AmpSave disable!\r\n");      
    }
}

static void
tc2102meCfgLDPS(void)
{
	uint16 phyAddr = mac_p->enetPhyAddr;
	int32 val;
	val = tcPhyReadLReg(phyAddr,0,30);
	val &= 0x00007fff;
	if (cfg_LDPS_flag==1)
		val |= (1<<15);
	tcPhyWriteLReg(phyAddr,0,30,val);		
}

int32
tc2102meReadProbe(uint8 port_num, uint8 mode)
{
    uint32 val, val_r31, val_g0r28;
    uint32 rval, wval;
    uint32 phyaddr = mac_p->enetPhyAddr + port_num;
    uint32 pn = mac_p->enetPhyAddr - phy_start_addr;
	val_r31 = tcMiiStationRead(phyaddr, 31); 
    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    val_g0r28 = tcMiiStationRead(phyaddr, 28);
	
    
    switch(mode){
        case ProbeZfgain:
            wval = 0x0b04 + pn;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x3f ;
            break;
        case ProbeAgccode:
            wval = 0x2e04 + pn;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>1)&0x1f ;
            break;
        case ProbeBoosten:
            wval = 0x2e04 + pn;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>6)&0x01 ;
            break;
        break;
        case ProbeSnr:
            wval = 0x0904 + pn;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0xff ;
            break;  
        case ProbeAdcSign:
            wval = 0x4104 + pn;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x7f ;
            if (rval>64){
                rval -= 128;
            }
            break;
        default:
            TCPHYDISP1("\r\ntcphy error: ReadProbe %d.\r\n",mode);
            rval = 0;
            break;
    }

    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    tcMiiStationWrite( phyaddr, 28, val_g0r28); // restore G0Reg28
    tcMiiStationWrite( phyaddr, 31, val_r31); // restore Reg31
    return rval;

}

#ifdef TCPHY_DEBUG
int32
tc2102meReadErrOverSum(uint8 port_num)
{
    int32 err_over_sum;
    
    tcPhyReadReg(port_num,25);
    pause(300);
    tcPhyReadReg(port_num,25);
    err_over_sum = Nmr25[port_num].err_over_cnt - Nmr25[port_num].err_over_cnt_prev;
    if( err_over_sum < 0 ){
        err_over_sum += 2048;
    }

    return err_over_sum;
}
#endif

#ifdef TCPHY_DEBUG
int32
tc2102meReadAdcSum(uint8 port_num)
{
    int32 cnt=1000;
    int32 AdcSign_sum = 0;
    int j;
      
    for(j=0;j<cnt;j++){
        AdcSign_sum += tc2102meReadProbe(port_num,ProbeAdcSign);
    }
    //shift right to show percent of the dc offset (unit:%)
    AdcSign_sum = (AdcSign_sum>>6);
      
    return AdcSign_sum;
}

#endif

#ifdef TCPHY_DEBUG
int32
tc2102meReadSnrSum(uint8 port_num, uint16 cnt)
{
    uint16 snr_sum = 0;
    uint16 j;
    
    for(j=0;j<cnt;j++) {
      snr_sum += tc2102meReadProbe(port_num,ProbeSnr);
    }
    return snr_sum;

}
#endif

#ifdef TCPHY_DEBUG
void 
tc2102meDispProbe100(uint8 port_num, bool long_msg)
{

	uint16 phyAddr = mac_p->enetPhyAddr;
    const uint16 tc2102meReadSnrCnt = 1000;

    TCPHYDISP2("tcphy[%d]:",phyAddr);
    TCPHYDISP2(" boosten=%ld", tc2102meReadProbe(port_num,ProbeBoosten));
    TCPHYDISP2(" agccode=%ld", tc2102meReadProbe(port_num,ProbeAgccode));
    TCPHYDISP2(" zfgain=%ld", tc2102meReadProbe(port_num,ProbeZfgain));
    TCPHYDISP2(" ch_idx=%ld", (tcPhyReadReg(port_num,29)&0x003f));
    TCPHYDISP2(" snr=%ld", tc2102meReadProbe(port_num,ProbeSnr));
    if (long_msg){
        TCPHYDISP2(" err_over_sum=%ld", tc2102meReadErrOverSum(port_num));
    }
    else {
        tcPhyReadReg(port_num,25);
    }
    TCPHYDISP2(" \r\n");
    if (long_msg){
        TCPHYDISP3(" snr_sum(x1000)=%ld",tc2102meReadSnrSum(port_num,tc2102meReadSnrCnt)); // snr_sum   
        TCPHYDISP4(" adc_avg=%ld/1000", tc2102meReadAdcSum(port_num));  
        TCPHYDISP3(" \r\n");
    }
}
#endif

#ifdef TCPHY_DEBUG
void
tc2102meSwDispMessage(uint8 port_num)
{
	uint16 phyAddr = mac_p->enetPhyAddr;
    switch (tcphy_link_state[port_num]) {
        case ST_LINK_DOWN: 
            break;
            
        case ST_LINK_DOWN2UP: 
            //tcPhyReadReg(port_num,28); read in tc2102meSwPatch()
            TCPHYDISP1("tcphy[%d]: Link-up at %d%s.",
                phyAddr,(mr28.final_speed?100:10),(mr28.final_duplex?"F":"H")); 
			
			TCPHYDISP1(" tx_amp_save=%d.",(mr28.tx_amp_save));          
            TCPHYDISP1("\r\n");
			
            if(mr28.final_speed) {
                tc2102meDispProbe100(port_num,0); // short message
            }
			
            break;
            
        case ST_LINK_UP: 
            break;
            
        case ST_LINK_UP2DOWN:
            TCPHYDISP1("tcphy[%d]: Link-down!!!\r\n",phyAddr);
            break;
            
        default: printf("\r\ntcphy error: SwDispMessage error!\r\n");
    }
}
#endif

#ifdef TCPHY_DEBUG
void
tc2102mePoweDown(uint8 port_num)
{
    uint32 val_l0r04;
	val_l0r04 = tcPhyReadLReg(port_num, 0, 4);
	tcPhyWriteLReg(port_num,0,4,0x0001); // 	Set FLP register to disable all capability
    tcPhyWriteLReg(port_num,0,0,0x1200); //  restart-AN
    tcPhyWriteLReg(port_num,0,0,0x1800); //  power-down
    tcPhyWriteLReg(port_num,0,4,val_l0r04); //  Set FLP register to its default value
}
#endif 

#ifdef TCPHY_DEBUG
void
tc2102meUpdateErrOverSum(uint8 port_num)
{
    uint16 err_over_cnt_prev; // to store previous err_over_cnt value
    int32 val;
    
    // clear Nmr25[port_num].err_over_cnt in tc2102meDispProbe100() in ST_LINK_DOWN2UP
    if ((mr28.final_speed == 1) // 100BaseTX
        && (tcphy_link_state[port_num] == ST_LINK_UP)) 
    {
        err_over_cnt_prev = Nmr25[port_num].err_over_cnt;
        tcPhyReadReg(port_num,25);
        val = Nmr25[port_num].err_over_cnt - err_over_cnt_prev;
        if( val < 0 ){
            val += 2048;
        }
        tc2102me_erroversum = val;
    }
    else {
        tc2102me_erroversum = -1;
    }
}
#endif


#ifdef TCPHY_DEBUG
void
tc2102meSwErrOverMonitor(uint8 port_num)
{
    if (tcPhy_disp_level<3) 
        return; // inactive

    if (tc2102me_erroversum>0){
        TCPHYDISP3("tcphy[%d]: ErrOver=%ld\r\n",port_num,tc2102me_erroversum);          
    }

}
#endif

#ifdef TCPHY_DEBUG
bool 
tc2102meInitialRegFlag(void)
{
    bool initial_value_flag=1; // default: correct initial value
    bool initial_value_flag_p=1;
	bool initial_value_flag_g=1;
	uint16 phyAddr = mac_p->enetPhyAddr;
    uint16 rval;

	if (eco_rev == 0x00){//tc2206F A1
		// check G3R23
        rval = tcPhyReadGReg(phyAddr,3,23);
        if (rval != tc2102me_G3R17_default){
            initial_value_flag_g=0; // wrong initial value
            }
		
       // check L0R25
        rval = tcPhyReadReg(phyAddr,25);
        if ((rval&0xf000)!=tc2102me_L0R25_default){
                initial_value_flag_p=0; // wrong initial value
            }
        
	}
	initial_value_flag = initial_value_flag_p & initial_value_flag_g;
    return (initial_value_flag);
}

#endif

/*
void 
tc2102meUp2DownFast(uint8 port_num)
{
	const uint8 tc2102me_Up2Down_Thd = 10;
	const uint8 tc2102me_Up2Down_ub = 40;
    if (mr28.lch_linkup_100 & !Nmr1[port_num].link_status) {
		if (tc2102me_Up2Down_cnt<tc2102me_Up2Down_ub) {
			tc2102me_Up2Down_cnt+=3;
			}
    	}
	else if (!mr28.lch_linkup_100 & !Nmr1[port_num].link_status) {
		if (tc2102me_Up2Down_cnt>0) {
			tc2102me_Up2Down_cnt--;
			}
		}
	else if (mr28.lch_linkup_100 & Nmr1[port_num].link_status) {
		tc2102me_Up2Down_cnt=0;
		}
	if (tc2102me_Up2Down_cnt> tc2102me_Up2Down_Thd) {
		tc2102me_Up2DownFast_detect=1;
		}
	else {
		tc2102me_Up2DownFast_detect=0;
		}
}

bool 
tc2102meLinkFailDetect (void){
	if (sw_FixUp2DownFast_flag &tc2102me_Up2DownFast_detect) {
		tc2102me_link_fail_detect_flag =1;
		#ifdef TCPHY_DEBUG
		//TCPHYDISP3(" tc2102me_link_fail_detect_flag=%ld\r\n",tc2102me_link_fail_detect_flag); 
		#endif
		}
	else {
		tc2102me_link_fail_detect_flag =0;
	}
	return tc2102me_link_fail_detect_flag;
}
*/


void 
tc2102meUp2DownFastHbt(uint8 port_num)
{
	const uint8 tc2102me_Up2Down_Hbt_Thd = 10;
	const uint8 tc2102me_Up2Down_Hbt_ub = 40;
    if (mr28.lch_linkup_100 & !Nmr1[port_num].link_status) {
		if (tc2102me_Up2Down_cnt_hbt<tc2102me_Up2Down_Hbt_ub) {
			tc2102me_Up2Down_cnt_hbt+=3;
			}
    	}
	else if (!mr28.lch_linkup_100 & !Nmr1[port_num].link_status) {
		if (tc2102me_Up2Down_cnt_hbt>0) {
			tc2102me_Up2Down_cnt_hbt--;
			}
		}
	else if (mr28.lch_linkup_100 & Nmr1[port_num].link_status) {//adjust link-up time to clear counter
		tc2102me_Up2Down_cnt_hbt=0;
		}
	if (tc2102me_Up2Down_cnt_hbt> tc2102me_Up2Down_Hbt_Thd) {
		tc2102me_Up2DownFastHbt_detect=1;
		tc2102me_Up2Down_cnt_hbt=0;
		}
	else {
		tc2102me_Up2DownFastHbt_detect=0;
		}  
}

void 
tc2102meUp2DownFastTbt(uint8 port_num)
{
	const uint8 tc2102me_Up2Down_Tbt_Thd = 10;
	const uint8 tc2102me_Up2Down_Tbt_ub = 40;
	const uint8 linkup_check_timer_done = 1;
    if (mr28.lch_linkup_10 & !Nmr1[port_num].link_status) {
		tc2102me_linkup_check_timer=0;
		if (tc2102me_Up2Down_cnt_tbt<tc2102me_Up2Down_Tbt_ub) {
			tc2102me_Up2Down_cnt_tbt+=5;
			}
    	}
	else if (!mr28.lch_linkup_10 & !Nmr1[port_num].link_status) {
		tc2102me_linkup_check_timer=0;
		if (tc2102me_Up2Down_cnt_tbt>0) {
			tc2102me_Up2Down_cnt_tbt--;
			}
		}
	else if (mr28.lch_linkup_10 & Nmr1[port_num].link_status ) {
		if ((tc2102me_linkup_check_timer == linkup_check_timer_done)) {
			tc2102me_Up2Down_cnt_tbt=0;
			tc2102me_linkup_check_timer=0;
			}
		else if ((tc2102me_linkup_check_timer < linkup_check_timer_done))
			tc2102me_linkup_check_timer++;
		}
	
	if (tc2102me_Up2Down_cnt_tbt> tc2102me_Up2Down_Tbt_Thd) {
		tc2102me_Up2DownFastTbt_detect=1;
		tc2102me_Up2Down_cnt_tbt=0;
		}
	else {
		tc2102me_Up2DownFastTbt_detect=0;
		}
	
}


bool 
tc2102meLinkFailDetect (uint8 port_num, uint8 mode){
	switch(mode){
			case TbtOrHbt:
				    if (sw_FixUp2DownFast_flag 
						&(tc2102me_Up2DownFastTbt_detect
						||tc2102me_Up2DownFastHbt_detect)) {
						tc2102me_link_fail_detect_flag =1;
		                #ifdef TCPHY_DEBUG
		                TCPHYDISP3(" tc2102me_link_fail_detect_flag=%d\r\n",tc2102me_link_fail_detect_flag); 
		                #endif
						}
					else 
						tc2102me_link_fail_detect_flag =0;
				break;
			case HbtOnly:
					if (sw_FixUp2DownFast_flag & tc2102me_Up2DownFastHbt_detect) {
						tc2102me_link_fail_detect_flag =1;
		                #ifdef TCPHY_DEBUG
		                TCPHYDISP3(" tc2102me_link_fail_detect_flag=%d\r\n",tc2102me_link_fail_detect_flag); 
		                #endif
						}
					else 
						tc2102me_link_fail_detect_flag =0;
				break;
			case TbtOnly:
				    if (sw_FixUp2DownFast_flag & tc2102me_Up2DownFastTbt_detect) {
						tc2102me_link_fail_detect_flag =1;
		                #ifdef TCPHY_DEBUG
		                TCPHYDISP3(" tc2102me_link_fail_detect_flag=%d\r\n",tc2102me_link_fail_detect_flag); 
		                #endif
						}
					else 
						tc2102me_link_fail_detect_flag =0;
				break;
			default: tc2102me_link_fail_detect_flag =0;
		}
	return tc2102me_link_fail_detect_flag;
}

void 
tc2102meDisNextPageNeeded(uint8 port_num)
{
   const uint8 tc2102meDisNextPage_cntdone = 5;
   
   if((tcphy_link_state[port_num] == ST_LINK_UP)||(tc2102meDisEEEHappened)) {
   	tc2102meDisNextPage_cnt = 0;
   	}
   else if ((mr5.LPNextAble) && !(mrl3_18.lp_eee_100||mrl3_18.lp_eee_1000||mrl3_18.lp_eee_10g)
   	&& (tc2102meDisNextPage_cnt<=tc2102meDisNextPage_cntdone)) {
   	tc2102meDisNextPage_cnt++;
   	}

   if(tc2102meDisNextPage_cnt>tc2102meDisNextPage_cntdone) {
   	tc2102meDisNextPage_flag=1;
	tc2102meDisNextPage_cnt = 0;
   	}
   else {
   	tc2102meDisNextPage_flag=0;
   	}
}


void 
tc2102meDisEEENeeded(uint8 port_num)
{
    const uint8 tc2102me_linkup_check_done = 1;
	
	
    if((tcphy_link_state[port_num] == ST_LINK_UP)
		&& (tc2102me_linkup_check_cnt<tc2102me_linkup_check_done)) {
		    tc2102me_linkup_check_cnt++;
    	}
	else if (tcphy_link_state[port_num] != ST_LINK_UP){
		  tc2102me_linkup_check_cnt = 0;
    	}
	
    if(tc2102meDisEEEHappened && tc2102me_linkup_check_cnt==tc2102me_linkup_check_done) {//extend link-up time
		tcPhyWriteLReg(port_num,3,17,tc2102me_L3R17_EEEAdv);
		tc2102meDisEEEHappened=0;
    	}
	else if(tc2102meLinkFailDetect(port_num,0) || tc2102meDisNextPage_flag) {
		tcPhyWriteLReg(port_num,3,17,0x0000);//disable next page
		tcPhyWriteLReg(port_num,0,0,0x1200);//re-start AN
		tc2102meDisEEEHappened=1;
    	}
	//TCPHYDISP3("tc2102me_linkup_check_cnt[%d]=%d\r\n",port_num,tc2102me_linkup_check_cnt);
	//TCPHYDISP3("tc2102meDisEEEHappened[%d]=%d\r\n",port_num,tc2102meDisEEEHappened);
}



void
tc2102meSwPatch(void)
{
    uint8 pn=0;

    //for(pn=0;pn<TCPHY_PORTNUM;pn++){

        tcPhyReadReg(pn,1);
        //
        if( !Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_DOWN;
        }
        else if( !Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_DOWN2UP;
        }
        else if( Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            tcphy_link_state[pn]= ST_LINK_UP2DOWN;
        }
        else { //if( Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_UP;
        }
    
        // read registers
        // tcPhyReadReg(pn,0);
        tcPhyReadReg(pn,28);
		tcPhyReadReg(pn,5);
        tcPhyReadLReg(pn,3,18);

	if (sw_FixUp2DownFast_flag) {
			tc2102meDisNextPageNeeded(pn);
			tc2102meUp2DownFastHbt(pn);
			tc2102meUp2DownFastTbt(pn);
			tc2102meDisEEENeeded(pn);
			}

		
        #ifdef TCPHY_DEBUG
        // display message
        tc2102meSwDispMessage(pn);
		#endif

#ifdef TCPHY_DEBUG
		if (sw_ErrOverMonitor_flag)
			tc2102meUpdateErrOverSum(pn);
        if (sw_ErrOverMonitor_flag) 
            tc2102meSwErrOverMonitor(pn); // call after tc2102meUpdateErrOverSum()
        
		
#endif

    //}
    
}

#endif // TC2102ME_SUPPORT


/************************************************************************
*
*            Variables and Functions for tc2104meSwPatch()
*
*************************************************************************
*/

#ifdef TC2104ME_SUPPORT
#define TC2104ME_PHY_INIT_GDATA_LEN 12
#define TC2104ME_PHY_INIT_LDATA_LEN 5
#define TC2104ME_PHY_INIT_PERPDATA_LEN 2
#define TC2104ME_PHY_INIT_SET_NUM 1

typedef struct tc2104me_cfg_data_s{
    char name[10];
    cfg_data_t gdata[TC2104ME_PHY_INIT_GDATA_LEN];
    cfg_data_t ldata[TC2104ME_PHY_INIT_LDATA_LEN];
	cfg_data_t perpdata[TC2104ME_PHY_INIT_PERPDATA_LEN]; //per port register setting
}tc2104me_cfg_data_t;

const uint32 tc2104me_page_sel_addr=31;

// G0R22b11: tx10_eee_mode
const uint16 tc2104me_G0R22_Tx10AmpSave_ON  = 0x0264;
const uint16 tc2104me_G0R22_Tx10AmpSave_OFF = 0x006F;
const uint32 tc2104me_L3R17_EEEAdv = 0x0002;

#define tc2104me_G3R23_default 0x48f0
#define tc2104me_L0R25_default 0xa000


tc2104me_cfg_data_t tc2104me_cfg[TC2104ME_PHY_INIT_SET_NUM]={
    //{{"F1.0"}, 
     //{ {31,0x3000},{16,0x001a},{23,0xabf0},{24,0x8801},
     //  {31,0x1000}
     //},
    {{"F9.3"},
     { {31,0x3000},{16,0x001a},{17,0x7238},{19,0x0100},{21,0x7386},{23,tc2104me_G3R23_default},{24,0x2201},
       {31,0x1000},{25,0x0018},{27,0x0126},
       {31,0x0000},{23,0x0006}
     },
     { {31,0x9000},{22,0x0140},
       {31,0x8000},{04,0x05e1},{25,tc2104me_L0R25_default}
     },
     { {31,0x8000},{30,0x8100}
	 }
    }, 
};
#endif

#ifdef TC2104ME_SUPPORT

static int32 tc2104me_erroversum=0;

//ESD detect
static int16 tc2104me_Up2Down_cnt_hbt[4] = {0,0,0,0};
static uint8 tc2104me_Up2DownFastHbt_detect[4] = {0,0,0,0};
static bool tc2104me_link_fail_detect_flag = 0;
static uint8 tc2104me_reset_needed_perport[4] = {0,0,0,0};
static uint8 tc2104me_reset_needed_flag = 0;
static uint8 tc2104me_esdphy_init_flag = 1; // default: enable

//Disable EEE if link fail
static int16 tc2104me_Up2Down_cnt_tbt[4] = {0,0,0,0};
static uint8 tc2104me_Up2DownFastTbt_detect[4] = {0,0,0,0};
static uint8 tc2104me_linkup_check_timer[4] = {0,0,0,0};
static bool tc2104meDisEEEHappened[4] = {0,0,0,0};
static uint8 tc2104me_linkup_check_cnt[4] = {0,0,0,0};

static uint8 tc2104meDisNextPage_cnt[4] = {0,0,0,0};
static bool tc2104meDisNextPage_flag[4] = {0,0,0,0};




static void tc2104meConfig(int argc, char *argv[], void *p);
void tc2104meCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num);
void tc2104meGRCfgCheck(void);
void tc2104meLRCfgCheck(uint8 port_num);
void tc2104meCfgCheck(void);
static void tc2104meLRCfgLoad(uint8 idx, uint8 port_num);
static void tc2104mePerPortCfgLoad(uint8 idx, uint8 port_num);
int32 tc2104meReadProbe(uint8 port_num, uint8 mode);
static void tc2104meCfgTx10AmpSave(void);
static void tc2104meCfgLDPS(void);
#ifdef TCPHY_DEBUG
int32 tc2104meReadErrOverSum(uint8 port_num);
int32 tc2104meReadAdcSum(uint8 port_num);
void tc2104meDispProbe100(uint8 port_num, bool long_msg);
void tc2104mePoweDown(uint8 port_num);
void tc2104meSwDispMessage(uint8 port_num);
uint16 tc2104meReadSnrSum(uint8 port_num, uint16 cnt);
void tc2104meUpdateErrOverSum(uint8 port_num);
void tc2104meSwErrOverMonitor(uint8 port_num);
bool tc2104meInitialRegFlag(void);
void tc2104meEsdSwPatch(void);
#endif
void tc2104meUp2DownFastHbt(uint8 port_num);
void tc2104meUp2DownFastTbt(uint8 port_num);
bool tc2104meLinkFailDetect (uint8 port_num, uint8 mode);//0:10BT or 100BT, 1:100BT only, 2:10BT only
void tc2104meDisEEENeeded(uint8 port_num);
void tc2104meDisNextPageNeeded(uint8 port_num);
void tc2104meSwPatch(void);


static void 
tc2104meConfig(int argc, char *argv[], void *p)
{
    // global config in doPhyConfig()
#ifdef TCPHY_DEBUG
    if (argc==1) {
        TCPHYDISP3("Usage: config Tx10AmpSave [on|off]\r\n");
		TCPHYDISP3("       config EsdPhyRegInit [on|off]\r\n");
		TCPHYDISP3("       config FixUp2DownFast [on|off]\r\n");
		TCPHYDISP3("       config LinkDownPS [on|off]\r\n");
		TCPHYDISP3("       config NextPageAutoDisable [on|off]\r\n");
        return;
    }
    else if (argc==2) {
        if (stricmp(argv[1], "list") == 0){
            TCPHYDISP3("config Tx10AmpSave : %s\r\n",
                    (cfg_Tx10AmpSave_flag?"on":"off"));  
			TCPHYDISP3("config EsdPhyRegInit : %s\r\n",
                    (tc2104me_esdphy_init_flag?"on":"off"));  
			TCPHYDISP3("config FixUp2DownFast : %s\r\n",
                    (sw_FixUp2DownFast_flag?"on":"off"));
			TCPHYDISP3("config LinkDownPS : %s\r\n",
                    (cfg_LDPS_flag?"on":"off"));
            return;
        }
    }
#endif

    if (argc==2 || argc==3){
        if (stricmp(argv[1], "Tx10AmpSave") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                cfg_Tx10AmpSave_flag = 1; // enable
                tc2104meCfgTx10AmpSave();
            }
            else if( stricmp(argv[2], "off") == 0 ){
                cfg_Tx10AmpSave_flag = 0;
                tc2104meCfgTx10AmpSave();
            }
            TCPHYDISP2("config Tx10AmpSave : %s\r\n",
                        (cfg_Tx10AmpSave_flag?"on":"off"));
        }
		if (stricmp(argv[1], "EsdPhyRegInit") == 0) {
			if( stricmp(argv[2], "on") == 0 ){
                tc2104me_esdphy_init_flag= 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                tc2104me_esdphy_init_flag= 0;
            }
            TCPHYDISP2("config EsdPhyRegInit : %s\r\n",
                        (tc2104me_esdphy_init_flag?"on":"off"));
		}
		if (stricmp(argv[1], "FixUp2DownFast") == 0) {
			if( stricmp(argv[2], "on") == 0 ){
                sw_FixUp2DownFast_flag= 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                sw_FixUp2DownFast_flag= 0;
            }
            TCPHYDISP2("config FixUp2DownFast : %s\r\n",
                        (sw_FixUp2DownFast_flag?"on":"off"));
		}
	   if (stricmp(argv[1], "LinkDownPS") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                cfg_LDPS_flag = 1; // enable
                tc2104meCfgLDPS();
            }
             else if( stricmp(argv[2], "off") == 0 ){
                cfg_LDPS_flag = 0; // disable
                tc2104meCfgLDPS();
            }
            TCPHYDISP2("config LinkDownPS : %s\r\n",
                        (cfg_LDPS_flag?"on":"off"));
        }
    }
}

// 1. loading default register setting in tcPhyInit()
// 2. changing register setting
void 
tc2104meCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num)
{
    int pn, i;
    uint16 phyAddr;
	current_idx = idx;
	
    // global registers
    phyAddr = 0; //mac_p->enetPhyAddr;

    for( i=0; i<TC2104ME_PHY_INIT_GDATA_LEN; i++ ){         
        tcMiiStationWrite(phyAddr, tc2104me_cfg[current_idx].gdata[i].reg_num, tc2104me_cfg[current_idx].gdata[i].val );
    }
    //     
    tc2104meCfgTx10AmpSave();

    // local registers 
    if (doPerPort_flag == DO_PER_PORT) {
		tc2104meLRCfgLoad(current_idx, port_num);
    	}
	else {
		 for( pn=0; pn<TCPHY_PORTNUM; pn++){
		 	phyAddr = pn; //mac_p->enetPhyAddr + pn;
		 	tc2104meLRCfgLoad(current_idx, phyAddr);
			if ((eco_rev == 0x00)||(eco_rev == 0x01)) { // bypass powerdown rx filter if eco_rev=0 or exo_rev=1
			    tc2104mePerPortCfgLoad(current_idx,phyAddr);
				}
			}
		 }
	

    TCPHYDISP4("tcphy: CfgLoad %s\r\n",  tc2104me_cfg[current_idx].name);
//#define UNH_TEST 1
#ifdef UNH_TEST
    phyAddr = 0;
    sw_ErrOverMonitor_flag=0;
	tc2104me_esdphy_init_flag = 0;
	sw_FixUp2DownFast_flag= 0;
	cfg_Tx10AmpSave_flag=0; // disable tx10_eee_mode
	tc2104meCfgTx10AmpSave();
	
	
    //tcPhyWriteGReg(phyAddr,0,22,0x0664); // disable tx10_eee_mode
    //tcPhyWriteGReg(phyAddr,0,26,0x0012); // disable cr_signal_status_opt
    tcPhyWriteGReg(phyAddr,1,22,0x0012); // enable RX_ER extension to Pass PCS pause test
    tcPhyWriteGReg(phyAddr,3,21,0x7387); // decrease I2MB to pass template
	if (doPerPort_flag == DO_PER_PORT) {
		tcPhyWriteLReg(port_num,0,30,0x0000); // disable AAPS to avoid varaince lc_max 
        tcPhyWriteLReg(port_num,0,26,0x1200); // disable Link_Det_Ext function
		}
	else{
		for( pn=0; pn<TCPHY_PORTNUM; pn++){
			phyAddr = pn; //mac_p->enetPhyAddr + pn;
            tcPhyWriteLReg(phyAddr,0,30,0x0000); // disable AAPS to avoid varaince lc_max 
            tcPhyWriteLReg(phyAddr,0,26,0x1200); // disable Link_Det_Ext function
            }
		}
    TCPHYDISP4("tcphy: CfgLoad UNH-IOL setting\r\n");
#endif
}

void
tc2104meGRCfgCheck(void)
{
    int i;
	uint32 phyaddr = mac_p->enetPhyAddr;
	uint32 rval;
	current_idx = 0; 
	
	for( i=0; i<TC2104ME_PHY_INIT_GDATA_LEN; i++){
		if(tc2104me_cfg[current_idx].gdata[i].reg_num == tc2104me_page_sel_addr) {
			tcMiiStationWrite(phyaddr, tc2104me_cfg[current_idx].gdata[i].reg_num, tc2104me_cfg[current_idx].gdata[i].val);
		}//if reg_num == tc2104me_page_sel_addr
		else {
			rval = tcMiiStationRead(phyaddr, tc2104me_cfg[current_idx].gdata[i].reg_num);
			if(rval!=tc2104me_cfg[current_idx].gdata[i].val){
				printf("Wrong initial setting of global register %d is detected!!\r\n",tc2104me_cfg[current_idx].gdata[i].reg_num);
				}//rval==gdata
			}
		}//for
}

void 
tc2104meLRCfgCheck(uint8 port_num)
{
	 uint32 phyaddr = mac_p->enetPhyAddr+port_num;
	 uint32 rval;

	 // check L0R25 
     rval = tcPhyReadReg(phyaddr,25);
     if ((rval&0xf000)!=tc2104me_L0R25_default){
	 	printf("Wrong initial setting of Local register 25 at port%d is detected!!\r\n",port_num);
        }
}

void 
tc2104meCfgCheck(void)
{
     int pn;
	 tc2104meGRCfgCheck();
	 
	 for( pn=0; pn<TCPHY_PORTNUM; pn++){
	 	tc2104meLRCfgCheck(pn);
	 	}
}


static void
tc2104meLRCfgLoad(uint8 idx, uint8 port_num)
{
    int i;
	current_idx = idx;
	for( i=0; i<TC2104ME_PHY_INIT_LDATA_LEN; i++ ){         
            tcMiiStationWrite(port_num, tc2104me_cfg[current_idx].ldata[i].reg_num, tc2104me_cfg[current_idx].ldata[i].val );
        }   
}

static void 
tc2104mePerPortCfgLoad(uint8 idx, uint8 port_num)
{
   int i;
   current_idx = idx;
   for( i=0; i<TC2104ME_PHY_INIT_PERPDATA_LEN; i++ ){         
            tcMiiStationWrite(port_num, tc2104me_cfg[current_idx].perpdata[i].reg_num, tc2104me_cfg[current_idx].perpdata[i].val );
        }  
}


static void
tc2104meCfgTx10AmpSave(void)
{
    uint16 phyAddr = mac_p->enetPhyAddr;

    if (cfg_Tx10AmpSave_flag==1){ // enable
        tcPhyWriteGReg(phyAddr,0,22,tc2104me_G0R22_Tx10AmpSave_ON);
        TCPHYDISP4("tcphy: Tx10AmpSave enable!\r\n");       
    }
    else { // disable
        tcPhyWriteGReg(phyAddr,0,22,tc2104me_G0R22_Tx10AmpSave_OFF);
        TCPHYDISP4("tcphy: Tx10AmpSave disable!\r\n");      
    }
}

static void
tc2104meCfgLDPS(void)
{
	uint8 pn;
	int32 val;
	for (pn=0;pn<4;pn++) {
		val = tcPhyReadLReg(pn,0,30);
		val &= 0x00007fff;
		if (cfg_LDPS_flag==1)
			val |= (1<<15);
		tcPhyWriteLReg(pn,0,30,val);
		}
}

int32
tc2104meReadProbe(uint8 port_num, uint8 mode)
{
    uint32 val, val_r31, val_g0r28;
    uint32 rval, wval;
    uint32 phyaddr = mac_p->enetPhyAddr + port_num;
    
    val_r31 = tcMiiStationRead(phyaddr, 31); 
    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    val_g0r28 = tcMiiStationRead(phyaddr, 28);
	
    
    switch(mode){
        case ProbeZfgain:
            wval = 0x0b04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x3f ;
            break;
        case ProbeAgccode:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>1)&0x1f ;
            break;
        case ProbeBoosten:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>6)&0x01 ;
            break;
        break;
        case ProbeSnr:
            wval = 0x0904 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0xff ;
            break;  
        case ProbeAdcSign:
            wval = 0x4104 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x7f ;
            if (rval>64){
                rval -= 128;
            }
            break;
        default:
            TCPHYDISP1("\r\ntcphy error: ReadProbe %d.\r\n",mode);
            rval = 0;
            break;
    }

    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    tcMiiStationWrite( phyaddr, 28, val_g0r28); // restore G0Reg28
    tcMiiStationWrite( phyaddr, 31, val_r31); // restore Reg31
    return rval;

}

#ifdef TCPHY_DEBUG
int32
tc2104meReadErrOverSum(uint8 port_num)
{
    int32 err_over_sum;
    
    tcPhyReadReg(port_num,25);
    pause(300);
    tcPhyReadReg(port_num,25);
    err_over_sum = Nmr25[port_num].err_over_cnt - Nmr25[port_num].err_over_cnt_prev;
    if( err_over_sum < 0 ){
        err_over_sum += 2048;
    }

    return err_over_sum;
}
#endif

#ifdef TCPHY_DEBUG
int32
tc2104meReadAdcSum(uint8 port_num)
{
    int32 cnt=1000;
    int32 AdcSign_sum = 0;
    int j;
      
    for(j=0;j<cnt;j++){
        AdcSign_sum += tc2104meReadProbe(port_num,ProbeAdcSign);
    }
    //shift right to show percent of the dc offset (unit:%)
    AdcSign_sum = (AdcSign_sum>>6);
      
    return AdcSign_sum;
}

#endif

#ifdef TCPHY_DEBUG
uint16
tc2104meReadSnrSum(uint8 port_num, uint16 cnt)
{
    uint16 snr_sum = 0;
    uint16 j;
    
    for(j=0;j<cnt;j++) {
      snr_sum += tc2104meReadProbe(port_num,ProbeSnr);
    }
    return snr_sum;

}
#endif

#ifdef TCPHY_DEBUG
void 
tc2104meDispProbe100(uint8 port_num, bool long_msg)
{

    const uint16 tc2104meReadSnrCnt = 1000;

    TCPHYDISP2("tcphy[%d]:",port_num);
    TCPHYDISP2(" boosten=%ld", tc2104meReadProbe(port_num,ProbeBoosten));
    TCPHYDISP2(" agccode=%ld", tc2104meReadProbe(port_num,ProbeAgccode));
    TCPHYDISP2(" zfgain=%ld", tc2104meReadProbe(port_num,ProbeZfgain));
    TCPHYDISP2(" ch_idx=%ld", (tcPhyReadReg(port_num,29)&0x003f));
    TCPHYDISP2(" snr=%ld", tc2104meReadProbe(port_num,ProbeSnr));
    if (long_msg){
        TCPHYDISP2(" err_over_sum=%ld", tc2104meReadErrOverSum(port_num));
    }
    else {
        tcPhyReadReg(port_num,25);
    }
    TCPHYDISP2(" \r\n");
    if (long_msg){
        TCPHYDISP3(" snr_sum(x1000)=%d",tc2104meReadSnrSum(port_num,tc2104meReadSnrCnt)); // snr_sum   
        TCPHYDISP4(" adc_avg=%ld/1000", tc2104meReadAdcSum(port_num));  
        TCPHYDISP3(" \r\n");
    }
}
#endif

#ifdef TCPHY_DEBUG
void
tc2104meSwDispMessage(uint8 port_num)
{
    switch (tcphy_link_state[port_num]) {
        case ST_LINK_DOWN: 
            break;
            
        case ST_LINK_DOWN2UP: 
            //tcPhyReadReg(port_num,28); read in tc2104meSwPatch()
            TCPHYDISP1("tcphy[%d]: Link-up at %d%s.",
                port_num,(mr28.final_speed?100:10),(mr28.final_duplex?"F":"H")); 
			
			TCPHYDISP1(" tx_amp_save=%d.",(mr28.tx_amp_save));          
            TCPHYDISP1("\r\n");
			
            if(mr28.final_speed) {
                tc2104meDispProbe100(port_num,0); // short message
            }
			
            break;
            
        case ST_LINK_UP: 
            break;
            
        case ST_LINK_UP2DOWN:
            TCPHYDISP1("tcphy[%d]: Link-down!!!\r\n",port_num);
            break;
            
        default: printf("\r\ntcphy error: SwDispMessage error!\r\n");
    }
}
#endif

#ifdef TCPHY_DEBUG
void
tc2104mePoweDown(uint8 port_num)
{
    uint32 val_l0r04;
	val_l0r04 = tcPhyReadLReg(port_num, 0, 4);
	tcPhyWriteLReg(port_num,0,4,0x0001); // 	Set FLP register to disable all capability
    tcPhyWriteLReg(port_num,0,0,0x1200); //  restart-AN
    tcPhyWriteLReg(port_num,0,0,0x1800); //  power-down
    tcPhyWriteLReg(port_num,0,4,val_l0r04); //  Set FLP register to its default value
}
#endif 

#ifdef TCPHY_DEBUG
void
tc2104meUpdateErrOverSum(uint8 port_num)
{
    uint16 err_over_cnt_prev; // to store previous err_over_cnt value
    int32 val;
    
    // clear Nmr25[port_num].err_over_cnt in tc2104meDispProbe100() in ST_LINK_DOWN2UP
    if ((mr28.final_speed == 1) // 100BaseTX
        && (tcphy_link_state[port_num] == ST_LINK_UP)) 
    {
        err_over_cnt_prev = Nmr25[port_num].err_over_cnt;
        tcPhyReadReg(port_num,25);
        val = Nmr25[port_num].err_over_cnt - err_over_cnt_prev;
        if( val < 0 ){
            val += 2048;
        }
        tc2104me_erroversum = val;
    }
    else {
        tc2104me_erroversum = -1;
    }
}
#endif


#ifdef TCPHY_DEBUG
void
tc2104meSwErrOverMonitor(uint8 port_num)
{
    if (tcPhy_disp_level<3) 
        return; // inactive

    if (tc2104me_erroversum>0){
        TCPHYDISP3("tcphy[%d]: ErrOver=%ld\r\n",port_num,tc2104me_erroversum);          
    }

}
#endif

#ifdef TCPHY_DEBUG
bool 
tc2104meInitialRegFlag(void)
{
    bool initial_value_flag=1; // default: correct initial value
    bool initial_value_flag_p[4]={1,1,1,1};
	bool initial_value_flag_g=1;
    uint8 pn;
    uint16 rval;

	if (eco_rev == 0x00){//tc2206F A1
		// check G3R23
        rval = tcPhyReadGReg(0,3,23);
        if (rval != tc2104me_G3R23_default){
            initial_value_flag_g=0; // wrong initial value
            }
		
       // check L0R25
        for(pn=0;pn<TCPHY_PORTNUM;pn++){
            rval = tcPhyReadReg(pn,25);
            if ((rval&0xf000)!=tc2104me_L0R25_default){
                initial_value_flag_p[pn]=0; // wrong initial value
            }
        }
	}
	initial_value_flag = initial_value_flag_p[0]&initial_value_flag_p[1]
		                 &initial_value_flag_p[2]&initial_value_flag_p[3]&initial_value_flag_g;
    return (initial_value_flag);
}

#endif

/*
#ifdef TCPHY_DEBUG
void
tc2104meEsdSwPatch(void)
{
if (!getTcPhyInitialRegFlag()) {
	TCPHYDISP2("ESD detected!!\r\n");
	tcPhyInit(mac_p);
   }
}
#endif
*/
void 
tc2104meUp2DownFastHbt(uint8 port_num)
{
	const uint8 tc2104me_Up2Down_Hbt_Thd = 10;
	const uint8 tc2104me_Up2Down_Hbt_ub = 40;
    if (mr28.lch_linkup_100 & !Nmr1[port_num].link_status) {
		if (tc2104me_Up2Down_cnt_hbt[port_num]<tc2104me_Up2Down_Hbt_ub) {
			tc2104me_Up2Down_cnt_hbt[port_num]+=3;
			}
    	}
	else if (!mr28.lch_linkup_100 & !Nmr1[port_num].link_status) {
		if (tc2104me_Up2Down_cnt_hbt[port_num]>0) {
			tc2104me_Up2Down_cnt_hbt[port_num]--;
			}
		}
	else if (mr28.lch_linkup_100 & Nmr1[port_num].link_status) {//adjust link-up time to clear counter
		tc2104me_Up2Down_cnt_hbt[port_num]=0;
		}
	
	if (tc2104me_Up2Down_cnt_hbt[port_num]> tc2104me_Up2Down_Hbt_Thd) {
	    tc2104me_Up2Down_cnt_hbt[port_num]=0;
		tc2104me_Up2DownFastHbt_detect[port_num]=1;
		#ifdef TCPHY_DEBUG
		   TCPHYDISP4("set tc2104me_Up2DownFastHbt_detect[%d] to 1.\r\n",port_num); 
		#endif
		}
	else {
		tc2104me_Up2DownFastHbt_detect[port_num]=0;
		}  
}

void 
tc2104meUp2DownFastTbt(uint8 port_num)
{
	const uint8 tc2104me_Up2Down_Tbt_Thd = 10;
	const uint8 tc2104me_Up2Down_Tbt_ub = 40;
	const uint8 linkup_check_timer_done = 1;
    if (mr28.lch_linkup_10 & !Nmr1[port_num].link_status) {
		tc2104me_linkup_check_timer[port_num]=0;
		if (tc2104me_Up2Down_cnt_tbt[port_num]<tc2104me_Up2Down_Tbt_ub) {
			tc2104me_Up2Down_cnt_tbt[port_num]+=5;
			}
    	}
	else if (!mr28.lch_linkup_10 & !Nmr1[port_num].link_status) {
		tc2104me_linkup_check_timer[port_num]=0;
		if (tc2104me_Up2Down_cnt_tbt[port_num]>0) {
			tc2104me_Up2Down_cnt_tbt[port_num]--;
			}
		}
	else if (mr28.lch_linkup_10 & Nmr1[port_num].link_status ) {
		if ((tc2104me_linkup_check_timer[port_num] == linkup_check_timer_done)) {
			tc2104me_Up2Down_cnt_tbt[port_num]=0;
			tc2104me_linkup_check_timer[port_num]=0;
			}
		else if ((tc2104me_linkup_check_timer[port_num] < linkup_check_timer_done))
			tc2104me_linkup_check_timer[port_num]++;
		}
	
	if (tc2104me_Up2Down_cnt_tbt[port_num]> tc2104me_Up2Down_Tbt_Thd) {
	    tc2104me_Up2Down_cnt_tbt[port_num]=0; 
		tc2104me_Up2DownFastTbt_detect[port_num]=1;
		#ifdef TCPHY_DEBUG
		   TCPHYDISP4("set tc2104me_Up2DownFastTbt_detect[%d] to 1.\r\n",port_num); 
		#endif
		}
	else {
		tc2104me_Up2DownFastTbt_detect[port_num]=0;
		}
	
}


bool 
tc2104meLinkFailDetect (uint8 port_num, uint8 mode){
	switch(mode){
			case TbtOrHbt:
				    if (sw_FixUp2DownFast_flag 
						&(tc2104me_Up2DownFastTbt_detect[port_num]
						||tc2104me_Up2DownFastHbt_detect[port_num])) {
						tc2104me_link_fail_detect_flag =1;
		                #ifdef TCPHY_DEBUG
		                TCPHYDISP3("tc2104me_link_fail_detect_flag(%d)=%d\r\n",port_num,tc2104me_link_fail_detect_flag); 
		                #endif
						}
					else 
						tc2104me_link_fail_detect_flag =0;
				break;
			case HbtOnly:
					if (sw_FixUp2DownFast_flag & tc2104me_Up2DownFastHbt_detect[port_num]) {
						tc2104me_link_fail_detect_flag =1;
		                #ifdef TCPHY_DEBUG
		                TCPHYDISP3("Hbt: tc2104me_link_fail_detect_flag(%d)=%d\r\n",port_num,tc2104me_link_fail_detect_flag); 
		                #endif
						}
					else 
						tc2104me_link_fail_detect_flag =0;
				break;
			case TbtOnly:
				    if (sw_FixUp2DownFast_flag & tc2104me_Up2DownFastTbt_detect[port_num]) {
						tc2104me_link_fail_detect_flag =1;
		                #ifdef TCPHY_DEBUG
		                TCPHYDISP3("Tbt: tc2104me_link_fail_detect_flag(%d)=%d\r\n",port_num,tc2104me_link_fail_detect_flag); 
		                #endif
						}
					else 
						tc2104me_link_fail_detect_flag =0;
				break;
			default: tc2104me_link_fail_detect_flag =0;
		}
	return tc2104me_link_fail_detect_flag;
}

void 
tc2104meDisNextPageNeeded(uint8 port_num)
{
   const uint8 tc2104meDisNextPage_cntdone = 5;
   
   if((tcphy_link_state[port_num] == ST_LINK_UP)||(tc2104meDisEEEHappened[port_num])) {
   	tc2104meDisNextPage_cnt[port_num] = 0;
   	}
   else if ((mr5.LPNextAble) && !(mrl3_18.lp_eee_100||mrl3_18.lp_eee_1000||mrl3_18.lp_eee_10g)
   	&& (tc2104meDisNextPage_cnt[port_num]<=tc2104meDisNextPage_cntdone)) {
   	tc2104meDisNextPage_cnt[port_num]++;
   	}

   if(tc2104meDisNextPage_cnt[port_num]>tc2104meDisNextPage_cntdone) {
   	tc2104meDisNextPage_flag[port_num]=1;
	tc2104meDisNextPage_cnt[port_num] = 0;
   	}
   else {
   	tc2104meDisNextPage_flag[port_num]=0;
   	}
}


void 
tc2104meDisEEENeeded(uint8 port_num)
{
    const uint8 tc2104me_linkup_check_done = 1;
	
	
    if((tcphy_link_state[port_num] == ST_LINK_UP)
		&& (tc2104me_linkup_check_cnt[port_num]<tc2104me_linkup_check_done)) {
		    tc2104me_linkup_check_cnt[port_num]++;
    	}
	else if (tcphy_link_state[port_num] != ST_LINK_UP){
		  tc2104me_linkup_check_cnt[port_num] = 0;
    	}
	
    if(tc2104meDisEEEHappened[port_num] && tc2104me_linkup_check_cnt[port_num]==tc2104me_linkup_check_done) {//extend link-up time
		tcPhyWriteLReg(port_num,3,17,tc2104me_L3R17_EEEAdv);
		tc2104meDisEEEHappened[port_num]=0;
    	}
	else if(tc2104meLinkFailDetect(port_num,0) || tc2104meDisNextPage_flag[port_num]) {
		tcPhyWriteLReg(port_num,3,17,0x0000);//disable next page
		tcPhyWriteLReg(port_num,0,0,0x1200);//re-start AN
		tc2104meDisEEEHappened[port_num]=1;
    	}
	//TCPHYDISP3("tc2104me_linkup_check_cnt[%d]=%d\r\n",port_num,tc2104me_linkup_check_cnt[port_num]);
	//TCPHYDISP3("tc2104meDisEEEHappened[%d]=%d\r\n",port_num,tc2104meDisEEEHappened[port_num]);
}



void
tc2104meSwPatch(void)
{
    uint8 pn;

    for(pn=0;pn<TCPHY_PORTNUM;pn++){

        tcPhyReadReg(pn,1);
        //
        if( !Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_DOWN;
        }
        else if( !Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_DOWN2UP;
        }
        else if( Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            tcphy_link_state[pn]= ST_LINK_UP2DOWN;
        }
        else { //if( Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_UP;
        }
    
        // read registers
        // tcPhyReadReg(pn,0);
        tcPhyReadReg(pn,5);
        tcPhyReadReg(pn,28);
		tcPhyReadLReg(pn,3,18);


		//ESD reset needed or EEE disable needed
		if (sw_FixUp2DownFast_flag) {
			tc2104meDisNextPageNeeded(pn);
			tc2104meUp2DownFastHbt(pn);
			tc2104meUp2DownFastTbt(pn);
			tc2104meDisEEENeeded(pn);
			}

        #ifdef TCPHY_DEBUG
        // display message
        tc2104meSwDispMessage(pn);
		#endif

#ifdef TCPHY_DEBUG
		if (sw_ErrOverMonitor_flag)
			tc2104meUpdateErrOverSum(pn);
        if (sw_ErrOverMonitor_flag) 
            tc2104meSwErrOverMonitor(pn); // call after tc2104meUpdateErrOverSum()
        
		
#endif

    }
    
}

#endif // TC2104ME_SUPPORT


/************************************************************************
*
*            Variables and Functions for tc2101mfSwPatch()
*
*************************************************************************
*/


#ifdef TC2101MF_SUPPORT
//*** TC2101MF register settting ***//
#define TC2101MF_PHY_INIT_DATA_LEN 3
#define TC2101MF_PHY_INIT_SET_NUM 1

typedef struct tc2101mf_cfg_data_s{
    char name[10];
    cfg_data_t data[TC2101MF_PHY_INIT_DATA_LEN];
}tc2101mf_cfg_data_t;

// G0R22b9: tx10_eee_mode
const uint16 tc2101mf_G0R22_Tx10AmpSave_ON  = 0x0264;
const uint16 tc2101mf_G0R22_Tx10AmpSave_OFF = 0x0064;


tc2101mf_cfg_data_t tc2101mf_cfg[TC2101MF_PHY_INIT_SET_NUM]={
    {{"MF1.0"}, 
	{{31,0x3000},
	 {31,0x1000},
	 {31,0x9000}}}, 
};

static void tc2101mfConfig(int argc, char *argv[], void *p);
static void tc2101mfCfgLoad(uint8 idx);
int32 tc2101mfReadProbe(uint8 port_num, uint8 mode);
int32 tc2101mfReadErrOverSum(uint8 port_num);
int32 tc2101mfReadAdcSum(uint8 port_num);
int32 tc2101mfReadSnrSum(uint8 port_num);
void tc2101mfDispProbe100(uint8 port_num);


void tc2101mfSwErrOverMonitor(void);
void tc2101mfSwDispMessage(uint8 port_num);
void tc2101mfSwPatch(void);
static void tc2101mfCfgTx10AmpSave(void);



static void 
tc2101mfConfig(int argc, char *argv[], void *p)
{
    // global config in doPhyConfig()

#ifdef TCPHY_DEBUG
    if (argc==1) {
		TCPHYDISP3("Usage: config Tx10AmpSave [on|off]\r\n");
        return;
    }
    else if (argc==2) {
        if (stricmp(argv[1], "list") == 0){
			TCPHYDISP3("config Tx10AmpSave : %s\r\n",
                    (cfg_Tx10AmpSave_flag?"on":"off"));
            return;
        }
    }
#endif
if (argc==2 || argc==3){
	    if (stricmp(argv[1], "Tx10AmpSave") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                cfg_Tx10AmpSave_flag = 1; // enable
                tc2101mfCfgTx10AmpSave();
            }
            else if( stricmp(argv[2], "off") == 0 ){
                cfg_Tx10AmpSave_flag = 0;
                tc2101mfCfgTx10AmpSave();
            }
            TCPHYDISP2("config Tx10AmpSave : %s\r\n",
                        (cfg_Tx10AmpSave_flag?"on":"off"));
        }
  }
}

// 1. loading default register setting in tcPhyInit()
// 2. changing register setting
static void 
tc2101mfCfgLoad(uint8 idx)
{
    int i;
    current_idx = idx;
  for( i=0; i<TC2101MF_PHY_INIT_DATA_LEN; i++ ){          
      miiStationWrite( mac_p, tc2101mf_cfg[current_idx].data[i].reg_num, tc2101mf_cfg[current_idx].data[i].val );
  }
  TCPHYDISP3("tcphy: CfgLoad %s\r\n",  tc2101mf_cfg[current_idx].name);
  //TCPHYDISP3("tcphy: Cfg Ver: %s\r\n",  tc2101me_cfg[current_idx].name);
}


static void
tc2101mfCfgTx10AmpSave(void)
{

    if (cfg_Tx10AmpSave_flag==1){ // enable
        tcPhyWriteGReg(0,0,22,tc2101mf_G0R22_Tx10AmpSave_ON);
        TCPHYDISP4("tcphy: Tx10AmpSave enable!\r\n");       
    }
    else { // disable
        tcPhyWriteGReg(0,0,22,tc2101mf_G0R22_Tx10AmpSave_OFF);
        TCPHYDISP4("tcphy: Tx10AmpSave disable!\r\n");      
    }
}

int32
tc2101mfReadProbe(uint8 port_num, uint8 mode)
{
    uint32 val, val_r31, val_g0r28;
    uint32 rval, wval;
    uint32 phyaddr = mac_p->enetPhyAddr + port_num;
    
    val_r31 = tcMiiStationRead(phyaddr, 31); 
    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    val_g0r28 = tcMiiStationRead(phyaddr, 28);
  
    switch(mode){
        case ProbeZfgain:
            wval = 0x0b04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x3f ;
            break;
        case ProbeAgccode:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>1)&0x1f ;
            break;
        case ProbeBoosten:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>6)&0x01 ;
            break;
        break;
        case ProbeSnr:
            wval = 0x0904 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0xff ;
            break;  
        case ProbeAdcSign:
            wval = 0x4104 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x7f ;
            if (rval>64){
                rval -= 128;
            }
            break;
        default:
            TCPHYDISP1("\r\ntcphy error: ReadProbe %d.\r\n",mode);
            rval = 0;
            break;
    }

    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    tcMiiStationWrite( phyaddr, 28, val_g0r28); // restore G0Reg28
    tcMiiStationWrite( phyaddr, 31, val_r31); // restore Reg31
    return rval;

}


int32
tc2101mfReadErrOverSum(uint8 port_num)
{
    int32 err_over_sum;
    
    tcPhyReadReg(port_num,25);
    pause(300);
    tcPhyReadReg(port_num,25);
    err_over_sum = Nmr25[port_num].err_over_cnt - Nmr25[port_num].err_over_cnt_prev;
    if( err_over_sum < 0 ){
        err_over_sum += 2048;
    }

    return err_over_sum;
}


int32
tc2101mfReadAdcSum(uint8 port_num)
{
    int32 cnt=1000;
    int32 AdcSign_sum = 0;
    int j;
      
    for(j=0;j<cnt;j++){
        AdcSign_sum += tc2101mfReadProbe(port_num,ProbeAdcSign);
    }
    //shift right to show percent of the dc offset (unit:%)
    AdcSign_sum = (AdcSign_sum>>6);
      
    return AdcSign_sum;
}


int32
tc2101mfReadSnrSum(uint8 port_num)
{
    int32 cnt=1000;
    int32 snr_sum = 0;
    int j;
    
    for(j=0;j<cnt;j++) {
      snr_sum += tc2101mfReadProbe(port_num,ProbeSnr);
    }
    return snr_sum;
}

void 
tc2101mfDispProbe100(uint8 port_num)
{
#ifdef TCPHY_DEBUG
    TCPHYDISP2("tcphy[%d]:",port_num);
	TCPHYDISP2(" boosten=%ld", tc2101mfReadProbe(port_num,ProbeBoosten));
	TCPHYDISP2(" agccode=%ld", tc2101mfReadProbe(port_num,ProbeAgccode));
	TCPHYDISP2(" zfgain=%ld", tc2101mfReadProbe(port_num,ProbeZfgain));
	TCPHYDISP2(" ch_idx=%ld", (tcPhyReadReg(port_num,29)&0x003f));
	TCPHYDISP2(" snr=%ld", tc2101mfReadProbe(port_num,ProbeSnr));
	TCPHYDISP2(" err_over_sum=%ld", tc2101mfReadErrOverSum(port_num));
    TCPHYDISP2(" \r\n");
	TCPHYDISP3(" snr_sum(x1000)=%ld",tc2101mfReadSnrSum(port_num)); // snr_sum	
	TCPHYDISP4(" adc_avg=%ld/1000", tc2101mfReadAdcSum(port_num));	
    TCPHYDISP3(" \r\n");
#endif
}

#ifdef TCPHY_DEBUG
void
tc2101mfSwErrOverMonitor(void)
{
    const uint8 port_num = 0;
    uint16 err_over_cnt_prev;
    int val;

    // clear Nmr25[0].err_over_cnt in tc2101mbDispProbe100() in ST_LINK_DOWN2UP

    if ((mr28.final_speed == 1) // 100BaseTX
        && ((tcphy_link_state[port_num] == ST_LINK_UP) 
            || (tcphy_link_state[port_num] == ST_LINK_UP2DOWN)))
    {
        err_over_cnt_prev = Nmr25[port_num].err_over_cnt;
        tcPhyReadReg(port_num,25);
        val = Nmr25[port_num].err_over_cnt - err_over_cnt_prev;
        if (val != 0){
            if( val < 0 ){
                val += 2048;
            }           
            TCPHYDISP3("tcphy: ErrOver=%1d\r\n",val);            
        }       
    }
}
#endif


void
tc2101mfSwDispMessage(uint8 port_num)
{
    switch (tcphy_link_state[port_num]) {
        case ST_LINK_DOWN: 
            break;
            
        case ST_LINK_DOWN2UP: 
            tcPhyReadReg(port_num,28);
            TCPHYDISP1("tcphy[%d]: Link-up at %d%s.",
                port_num,(mr28.final_speed?100:10),(mr28.final_duplex?"F":"H"));    
            TCPHYDISP1(" tx_amp_save=%d.",(mr28.tx_amp_save));          
            TCPHYDISP1("\r\n");
            if(mr28.final_speed) {
                tc2101mfDispProbe100(port_num);
            }           
            break;
            
        case ST_LINK_UP: 
            break;
            
        case ST_LINK_UP2DOWN:
            TCPHYDISP1("tcphy[%d]: Link-down!!!\r\n",port_num);
            break;

#ifdef TCPHY_DEBUG
        default: printf("\r\ntcphy error: SwDispMessage error!\r\n");           
#endif
    }
}


void
tc2101mfSwPatch(void)
{

    uint8 port_num = 0;

    tcPhyReadReg(port_num,1);
    //
    if( !Nmr1[0].link_status_prev && !Nmr1[0].link_status ){
        tcphy_link_state[port_num] = ST_LINK_DOWN;
    }
    else if( !Nmr1[0].link_status_prev && Nmr1[0].link_status ){
        tcphy_link_state[port_num] = ST_LINK_DOWN2UP;
    }
    else if( Nmr1[0].link_status_prev && !Nmr1[0].link_status ){
        tcphy_link_state[port_num] = ST_LINK_UP2DOWN;
    }
    else { //if( Nmr1[0].link_status_prev && Nmr1[0].link_status ){
        tcphy_link_state[port_num] = ST_LINK_UP;
    }
    
    // read registers
    // tcPhyReadReg(port_num,0);
    // tcPhyReadReg(port_num,28);
    
    // display message
    tc2101mfSwDispMessage(port_num);
    
#ifdef TCPHY_DEBUG
    if (sw_ErrOverMonitor_flag) 
        tc2101mfSwErrOverMonitor();
#endif
    
}


#endif // TC2101MF_SUPPORT

/************************************************************************
*
*            Variables and Functions for tc2105sgSwPatch()
*
*************************************************************************
*/
//#define TC2105SG_CM_SUPPORT
//#define UNH_TEST 1

#ifdef TC2105SG_SUPPORT

#ifdef TC2105SG_CM_SUPPORT
#define TC2105SG_PHY_INIT_GDATA_LEN 18
#define TC2105SG_PHY_INIT_LDATA_LEN 2
#define TC2105SG_PHY_INIT_PERPDATA_LEN 1
#define TC2105SG_PHY_INIT_SET_NUM 1
#else
#ifdef UNH_TEST
#define TC2105SG_PHY_INIT_GDATA_LEN 20
#define TC2105SG_PHY_INIT_LDATA_LEN 2
#define TC2105SG_PHY_INIT_PERPDATA_LEN 1
#define TC2105SG_PHY_INIT_SET_NUM 1
#else //voltage mode
#define TC2105SG_PHY_INIT_GDATA_LEN 20
#define TC2105SG_PHY_INIT_LDATA_LEN 2
#define TC2105SG_PHY_INIT_PERPDATA_LEN 1
#define TC2105SG_PHY_INIT_SET_NUM 1
#endif //UNH_TEST
#endif //TC2105SG_CM_SUPPORT

typedef struct tc2105sg_cfg_data_s{
    char name[10];
    cfg_data_t gdata[TC2105SG_PHY_INIT_GDATA_LEN];
    cfg_data_t ldata[TC2105SG_PHY_INIT_LDATA_LEN];
	cfg_data_t perpdata[TC2105SG_PHY_INIT_PERPDATA_LEN]; //per port register setting
}tc2105sg_cfg_data_t;

const uint32 tc2105sg_page_sel_addr=31;

// G0R22b11: tx10_eee_mode
const uint16 tc2105sg_G0R22_Tx10AmpSave_ON  = 0x0264;
const uint16 tc2105sg_G0R22_Tx10AmpSave_OFF = 0x006F;
const uint32 tc2105sg_L3R17_EEEAdv = 0x0002;

//#define tc2105sg_G3R23_default 0x48f0
#ifdef TC2105SG_CM_SUPPORT
tc2105sg_cfg_data_t tc2105sg_cfg[TC2105SG_PHY_INIT_SET_NUM]={
    {{"H3.0.cm"},
     { {31,0x4000},{21,0x7000},
       {31,0x3000},{20,0xbb00},{22,0xf010},{25,0x00f8},{26,0x3009},
       {31,0x2000},{21,0x068c},{22,0x01d4},{23,0x0fbd},{24,0x0a72},{25,0x0e2b},{26,0x0043},
       {31,0x1000},{17,0xe4f8},
       {31,0x0000},{22,0x0064}
     },
     { {31,0x9000},{28,0x069f}
     },
     { {31,0x8000}
	 }
    }, 
};
#else
#ifdef UNH_TEST
tc2105sg_cfg_data_t tc2105sg_cfg[TC2105SG_PHY_INIT_SET_NUM]={
    {{"UNH2.0.vm"},
     { {31,0x4000},{16,0x320c},{21,0x7000},{22,0x11cf},{25,0x0402},{26,0x0303},{27,0xeeee},
       {31,0x3000},{20,0xbb00},
       {31,0x2000},{21,0x07d6},{22,0x0010},{23,0x00f6},{24,0x086c},{25,0x0030},{26,0x0f48},
       {31,0x1000},{17,0xe4f8},
       {31,0x0000},{22,0x0064}
     },
     { {31,0xa000},{17,0x03a0}
     },
     { {31,0x8000}
	 }
    }, 
};
#else
tc2105sg_cfg_data_t tc2105sg_cfg[TC2105SG_PHY_INIT_SET_NUM]={
    {{"H3.0.vm"},
     { {31,0x4000},{16,0x320c},{21,0x7000},{22,0x11cf},{25,0x0202},{26,0x0303},{27,0xeeee},
       {31,0x3000},{20,0xbb00},
       {31,0x2000},{21,0x0690},{22,0x0094},{23,0x003b},{24,0x0970},{25,0x0fac},{26,0x0004},
       {31,0x1000},{17,0xe4f8},
       {31,0x0000},{22,0x0064}
     },
     { {31,0xa000},{17,0x03a0}
     },
     { {31,0x8000}
	 }
    }, 
};
#endif //UNH_TEST

#endif //TC2105SG_CM_SUPPORT

#endif

#ifdef TC2105SG_SUPPORT

static int32 tc2105sg_erroversum=0;

static int16 tc2105sg_Up2Down_cnt_hbt[5] = {0,0,0,0,0};
static uint8 tc2105sg_Up2DownFastHbt_detect[5] = {0,0,0,0,0};
static bool tc2105sg_link_fail_detect_flag = 0;
//static uint8 tc2105sg_reset_needed_perport[5] = {0,0,0,0,0};
//static uint8 tc2105sg_reset_needed_flag = 0;
//static uint8 tc2105sg_esdphy_init_flag = 1; // default: enable

//Disable EEE if link fail
static int16 tc2105sg_Up2Down_cnt_tbt[5] = {0,0,0,0,0};
static uint8 tc2105sg_Up2DownFastTbt_detect[5] = {0,0,0,0,0};
static uint8 tc2105sg_linkup_check_timer[5] = {0,0,0,0,0};
static bool tc2105sgDisEEEHappened[5] = {0,0,0,0,0};
static uint8 tc2105sg_linkup_check_cnt[5] = {0,0,0,0,0};

static uint8 tc2105sgDisNextPage_cnt[5] = {0,0,0,0,0};
static bool tc2105sgDisNextPage_flag[5] = {0,0,0,0,0};

static void tc2105sgConfig(int argc, char *argv[], void *p);
void tc2105sgCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num);
void tc2105sgGRCfgCheck(void);
void tc2105sgLRCfgCheck(uint8 port_num);
void tc2105sgCfgCheck(void);
static void tc2105sgLRCfgLoad(uint8 idx, uint8 port_num);
static void tc2105sgPerPortCfgLoad(uint8 idx, uint8 port_num);
int32 tc2105sgReadProbe(uint8 port_num, uint8 mode);
static void tc2105sgCfgTx10AmpSave(void);
//static void tc2105sgCfgLDPS(void);
#ifdef TCPHY_DEBUG
int32 tc2105sgReadErrOverSum(uint8 port_num);
int32 tc2105sgReadAdcSum(uint8 port_num);
void tc2105sgDispProbe100(uint8 port_num, bool long_msg);
void tc2105sgPoweDown(uint8 port_num);
void tc2105sgSwDispMessage(uint8 port_num);
uint16 tc2105sgReadSnrSum(uint8 port_num, uint16 cnt);
void tc2105sgUpdateErrOverSum(uint8 port_num);
void tc2105sgSwErrOverMonitor(uint8 port_num);
//bool tc2105sgInitialRegFlag(void);
//void tc2105sgEsdSwPatch(void);
#endif
void tc2105sgUp2DownFastHbt(uint8 port_num);
void tc2105sgUp2DownFastTbt(uint8 port_num);
bool tc2105sgLinkFailDetect (uint8 port_num, uint8 mode);//0:10BT or 100BT, 1:100BT only, 2:10BT only
void tc2105sgDisEEENeeded(uint8 port_num);
void tc2105sgDisNextPageNeeded(uint8 port_num);
void tc2105sgSwPatch(void);


static void 
tc2105sgConfig(int argc, char *argv[], void *p)
{
    // global config in doPhyConfig()
#ifdef TCPHY_DEBUG
    if (argc==1) {
        TCPHYDISP3("Usage: config Tx10AmpSave [on|off]\r\n");
		//TCPHYDISP3("       config EsdPhyRegInit [on|off]\r\n");
		TCPHYDISP3("       config FixUp2DownFast [on|off]\r\n");
		//TCPHYDISP3("       config LinkDownPS [on|off]\r\n");
		//TCPHYDISP3("       config NextPageAutoDisable [on|off]\r\n");
        return;
    }
    else if (argc==2) {
        if (stricmp(argv[1], "list") == 0){
            TCPHYDISP3("config Tx10AmpSave : %s\r\n",
                    (cfg_Tx10AmpSave_flag?"on":"off"));  
			//TCPHYDISP3("config EsdPhyRegInit : %s\r\n",
            //        (tc2104me_esdphy_init_flag?"on":"off"));  
			TCPHYDISP3("config FixUp2DownFast : %s\r\n",
                    (sw_FixUp2DownFast_flag?"on":"off"));
			//TCPHYDISP3("config LinkDownPS : %s\r\n",
            //        (cfg_LDPS_flag?"on":"off"));
            return;
        }
    }
#endif

    if (argc==2 || argc==3){
        if (stricmp(argv[1], "Tx10AmpSave") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                cfg_Tx10AmpSave_flag = 1; // enable
                tc2105sgCfgTx10AmpSave();
            }
            else if( stricmp(argv[2], "off") == 0 ){
                cfg_Tx10AmpSave_flag = 0;
                tc2105sgCfgTx10AmpSave();
            }
            TCPHYDISP2("config Tx10AmpSave : %s\r\n",
                        (cfg_Tx10AmpSave_flag?"on":"off"));
        }
		/*if (stricmp(argv[1], "EsdPhyRegInit") == 0) {
			if( stricmp(argv[2], "on") == 0 ){
                tc2104me_esdphy_init_flag= 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                tc2104me_esdphy_init_flag= 0;
            }
            TCPHYDISP2("config EsdPhyRegInit : %s\r\n",
                        (tc2104me_esdphy_init_flag?"on":"off"));
		}*/
		if (stricmp(argv[1], "FixUp2DownFast") == 0) {
			if( stricmp(argv[2], "on") == 0 ){
                sw_FixUp2DownFast_flag= 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                sw_FixUp2DownFast_flag= 0;
            }
            TCPHYDISP2("config FixUp2DownFast : %s\r\n",
                        (sw_FixUp2DownFast_flag?"on":"off"));
		}
	   /*if (stricmp(argv[1], "LinkDownPS") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                cfg_LDPS_flag = 1; // enable
                tc2104meCfgLDPS();
            }
             else if( stricmp(argv[2], "off") == 0 ){
                cfg_LDPS_flag = 0; // disable
                tc2104meCfgLDPS();
            }
            TCPHYDISP2("config LinkDownPS : %s\r\n",
                        (cfg_LDPS_flag?"on":"off"));
        }*/
    }
}

// 1. loading default register setting in tcPhyInit()
// 2. changing register setting
void 
tc2105sgCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num)
{
    int pn, i;
    uint16 phyAddr;
	current_idx = idx;
	
    // global registers
    phyAddr = 0; //mac_p->enetPhyAddr;

    for( i=0; i<TC2105SG_PHY_INIT_GDATA_LEN; i++ ){         
        tcMiiStationWrite(phyAddr, tc2105sg_cfg[current_idx].gdata[i].reg_num, tc2105sg_cfg[current_idx].gdata[i].val );
    }
    //     
    tc2105sgCfgTx10AmpSave();

    // local registers 
    if (doPerPort_flag == DO_PER_PORT) {
		tc2105sgLRCfgLoad(current_idx, port_num);
    	}
	else {
		 for( pn=0; pn<TCPHY_PORTNUM; pn++){
		 	phyAddr = pn; //mac_p->enetPhyAddr + pn;
		 	tc2105sgLRCfgLoad(current_idx, phyAddr);
			}
		 }
	

    TCPHYDISP4("tcphy: CfgLoad %s\r\n",  tc2105sg_cfg[current_idx].name);

#ifdef UNH_TEST
    phyAddr = 0;
    sw_ErrOverMonitor_flag=0;
	sw_FixUp2DownFast_flag= 0;
	cfg_Tx10AmpSave_flag=0; // disable tx10_eee_mode
	tc2105sgCfgTx10AmpSave();
	
	
    //tcPhyWriteGReg(phyAddr,0,22,0x0664); // disable tx10_eee_mode
    //tcPhyWriteGReg(phyAddr,0,26,0x0012); // disable cr_signal_status_opt
    tcPhyWriteGReg(phyAddr,1,22,0x0010); // enable RX_ER extension to Pass PCS pause test
    //tcPhyWriteGReg(phyAddr,3,21,0x7387); // decrease I2MB to pass template
	if (doPerPort_flag == DO_PER_PORT) {
		tcPhyWriteLReg(port_num,0,30,0x0000); // disable AAPS to avoid varaince lc_max 
        tcPhyWriteLReg(port_num,0,26,0x1200); // disable Link_Det_Ext function
		}
	else{
		for( pn=0; pn<TCPHY_PORTNUM; pn++){
			phyAddr = pn; //mac_p->enetPhyAddr + pn;
            tcPhyWriteLReg(phyAddr,0,30,0x0000); // disable AAPS to avoid varaince lc_max 
            tcPhyWriteLReg(phyAddr,0,26,0x1200); // disable Link_Det_Ext function
            }
		}
    TCPHYDISP4("tcphy: CfgLoad UNH-IOL setting\r\n");
#endif
}

void
tc2105sgGRCfgCheck(void)
{
    int i;
	uint32 phyaddr = mac_p->enetPhyAddr;
	uint32 rval;
	current_idx = 0; 
	
	for( i=0; i<TC2105SG_PHY_INIT_GDATA_LEN; i++){
		if(tc2105sg_cfg[current_idx].gdata[i].reg_num == tc2105sg_page_sel_addr) {
			tcMiiStationWrite(phyaddr, tc2105sg_cfg[current_idx].gdata[i].reg_num, tc2105sg_cfg[current_idx].gdata[i].val);
		}//if reg_num == page_sel_addr
		else {
			rval = tcMiiStationRead(phyaddr, tc2105sg_cfg[current_idx].gdata[i].reg_num);
			if(rval!=tc2105sg_cfg[current_idx].gdata[i].val){
				printf("Wrong initial setting of global register %d is detected!!\r\n",tc2105sg_cfg[current_idx].gdata[i].reg_num);
				}//rval==gdata
			}
		}//for
}

void 
tc2105sgLRCfgCheck(uint8 port_num)
{
	 uint32 phyaddr = mac_p->enetPhyAddr+port_num;
	 uint32 rval;
/*
	 // check L0R25 
     rval = tcPhyReadReg(phyaddr,25);
     if ((rval&0xf000)!=tc2105sg_L0R25_default){
	 	printf("Wrong initial setting of Local register 25 at port%d is detected!!\r\n",port_num);
        }
        */
}

void 
tc2105sgCfgCheck(void)
{
     int pn;
	 tc2105sgGRCfgCheck();
	 
	 for( pn=0; pn<TCPHY_PORTNUM; pn++){
	 	tc2105sgLRCfgCheck(pn);
	 	}
}


static void
tc2105sgLRCfgLoad(uint8 idx, uint8 port_num)
{
    int i;
	current_idx = idx;
	for( i=0; i<TC2105SG_PHY_INIT_LDATA_LEN; i++ ){         
            tcMiiStationWrite(port_num, tc2105sg_cfg[current_idx].ldata[i].reg_num, tc2105sg_cfg[current_idx].ldata[i].val );
        }   
}

static void 
tc2105sgPerPortCfgLoad(uint8 idx, uint8 port_num)
{
   int i;
   current_idx = idx;
   for( i=0; i<TC2105SG_PHY_INIT_PERPDATA_LEN; i++ ){         
            tcMiiStationWrite(port_num, tc2105sg_cfg[current_idx].perpdata[i].reg_num, tc2105sg_cfg[current_idx].perpdata[i].val );
        }  
}


static void
tc2105sgCfgTx10AmpSave(void)
{
    uint16 phyAddr = mac_p->enetPhyAddr;

    if (cfg_Tx10AmpSave_flag==1){ // enable
        tcPhyWriteGReg(phyAddr,0,22,tc2105sg_G0R22_Tx10AmpSave_ON);
        TCPHYDISP4("tcphy: Tx10AmpSave enable!\r\n");       
    }
    else { // disable
        tcPhyWriteGReg(phyAddr,0,22,tc2105sg_G0R22_Tx10AmpSave_OFF);
        TCPHYDISP4("tcphy: Tx10AmpSave disable!\r\n");      
    }
}


int32
tc2105sgReadProbe(uint8 port_num, uint8 mode)
{
    uint32 val, val_r31, val_g0r28;
    uint32 rval, wval;
    uint32 phyaddr = mac_p->enetPhyAddr + port_num;
    
    val_r31 = tcMiiStationRead(phyaddr, 31); 
    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    val_g0r28 = tcMiiStationRead(phyaddr, 28);
	
    
    switch(mode){
        case ProbeZfgain:
            wval = 0x0b04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x3f ;
            break;
        case ProbeAgccode:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>1)&0x1f ;
            break;
        case ProbeBoosten:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>6)&0x01 ;
            break;
        break;
        case ProbeSnr:
            wval = 0x0904 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0xff ;
            break;  
        case ProbeAdcSign:
            wval = 0x4104 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x7f ;
            if (rval>64){
                rval -= 128;
            }
            break;
        default:
            TCPHYDISP1("\r\ntcphy error: ReadProbe %d.\r\n",mode);
            rval = 0;
            break;
    }

    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    tcMiiStationWrite( phyaddr, 28, val_g0r28); // restore G0Reg28
    tcMiiStationWrite( phyaddr, 31, val_r31); // restore Reg31
    return rval;

}

#ifdef TCPHY_DEBUG
int32
tc2105sgReadErrOverSum(uint8 port_num)
{
    int32 err_over_sum;
    
    tcPhyReadReg(port_num,25);
    pause(300);
    tcPhyReadReg(port_num,25);
    err_over_sum = Nmr25[port_num].err_over_cnt - Nmr25[port_num].err_over_cnt_prev;
    if( err_over_sum < 0 ){
        err_over_sum += 2048;
    }

    return err_over_sum;
}
#endif

#ifdef TCPHY_DEBUG
int32
tc2105sgReadAdcSum(uint8 port_num)
{
    int32 cnt=1000;
    int32 AdcSign_sum = 0;
    int j;
      
    for(j=0;j<cnt;j++){
        AdcSign_sum += tc2105sgReadProbe(port_num,ProbeAdcSign);
    }
    //shift right to show percent of the dc offset (unit:%)
    AdcSign_sum = (AdcSign_sum>>6);
      
    return AdcSign_sum;
}

#endif

#ifdef TCPHY_DEBUG
uint16
tc2105sgReadSnrSum(uint8 port_num, uint16 cnt)
{
    uint16 snr_sum = 0;
    uint16 j;
    
    for(j=0;j<cnt;j++) {
      snr_sum += tc2105sgReadProbe(port_num,ProbeSnr);
    }
    return snr_sum;

}
#endif

#ifdef TCPHY_DEBUG
void 
tc2105sgDispProbe100(uint8 port_num, bool long_msg)
{

    const uint16 tc2105sgReadSnrCnt = 1000;

    TCPHYDISP2("tcphy[%d]:",port_num);
    TCPHYDISP2(" boosten=%ld", tc2105sgReadProbe(port_num,ProbeBoosten));
    TCPHYDISP2(" agccode=%ld", tc2105sgReadProbe(port_num,ProbeAgccode));
    TCPHYDISP2(" zfgain=%ld", tc2105sgReadProbe(port_num,ProbeZfgain));
    TCPHYDISP2(" ch_idx=%ld", (tcPhyReadReg(port_num,29)&0x003f));
    TCPHYDISP2(" snr=%ld", tc2105sgReadProbe(port_num,ProbeSnr));
    if (long_msg){
        TCPHYDISP2(" err_over_sum=%ld", tc2105sgReadErrOverSum(port_num));
    }
    else {
        tcPhyReadReg(port_num,25);
    }
    TCPHYDISP2(" \r\n");
    if (long_msg){
        TCPHYDISP3(" snr_sum(x1000)=%d",tc2105sgReadSnrSum(port_num,tc2105sgReadSnrCnt)); // snr_sum   
        TCPHYDISP4(" adc_avg=%ld/1000", tc2105sgReadAdcSum(port_num));  
        TCPHYDISP3(" \r\n");
    }
}
#endif

#ifdef TCPHY_DEBUG
void
tc2105sgSwDispMessage(uint8 port_num)
{
    switch (tcphy_link_state[port_num]) {
        case ST_LINK_DOWN: 
            break;
            
        case ST_LINK_DOWN2UP: 
            //tcPhyReadReg(port_num,28); read in tc2104meSwPatch()
            TCPHYDISP1("tcphy[%d]: Link-up at %d%s.",
                port_num,(mr28.final_speed?100:10),(mr28.final_duplex?"F":"H")); 
			
			TCPHYDISP1(" tx_amp_save=%d.",(mr28.tx_amp_save));          
            TCPHYDISP1("\r\n");
			
            if(mr28.final_speed) {
                tc2105sgDispProbe100(port_num,0); // short message
            }
			
            break;
            
        case ST_LINK_UP: 
            break;
            
        case ST_LINK_UP2DOWN:
            TCPHYDISP1("tcphy[%d]: Link-down!!!\r\n",port_num);
            break;
            
        default: printf("\r\ntcphy error: SwDispMessage error!\r\n");
    }
}
#endif

#ifdef TCPHY_DEBUG
void
tc2105sgPoweDown(uint8 port_num)
{
    uint32 val_l0r04;
	val_l0r04 = tcPhyReadLReg(port_num, 0, 4);
	tcPhyWriteLReg(port_num,0,4,0x0001); // 	Set FLP register to disable all capability
    tcPhyWriteLReg(port_num,0,0,0x1200); //  restart-AN
    tcPhyWriteLReg(port_num,0,0,0x1800); //  power-down
    tcPhyWriteLReg(port_num,0,4,val_l0r04); //  Set FLP register to its default value
}
#endif 

#ifdef TCPHY_DEBUG
void
tc2105sgUpdateErrOverSum(uint8 port_num)
{
    uint16 err_over_cnt_prev; // to store previous err_over_cnt value
    int32 val;
    
    // clear Nmr25[port_num].err_over_cnt in tc2104meDispProbe100() in ST_LINK_DOWN2UP
    if ((mr28.final_speed == 1) // 100BaseTX
        && (tcphy_link_state[port_num] == ST_LINK_UP)) 
    {
        err_over_cnt_prev = Nmr25[port_num].err_over_cnt;
        tcPhyReadReg(port_num,25);
        val = Nmr25[port_num].err_over_cnt - err_over_cnt_prev;
        if( val < 0 ){
            val += 2048;
        }
        tc2105sg_erroversum = val;
    }
    else {
        tc2105sg_erroversum = -1;
    }
}
#endif


#ifdef TCPHY_DEBUG
void
tc2105sgSwErrOverMonitor(uint8 port_num)
{
    if (tcPhy_disp_level<3) 
        return; // inactive

    if (tc2105sg_erroversum>0){
        TCPHYDISP3("tcphy[%d]: ErrOver=%ld\r\n",port_num,tc2105sg_erroversum);          
    }

}
#endif
/*
#ifdef TCPHY_DEBUG
bool 
tc2105sgInitialRegFlag(void)
{
    bool initial_value_flag=1; // default: correct initial value
    bool initial_value_flag_p[4]={1,1,1,1};
	bool initial_value_flag_g=1;
    uint8 pn;
    uint16 rval;

	if (eco_rev == 0x00){//tc2206F A1
		// check G3R23
        rval = tcPhyReadGReg(0,3,23);
        if (rval != tc2105sg_G3R23_default){
            initial_value_flag_g=0; // wrong initial value
            }
		
       // check L0R25
        for(pn=0;pn<TCPHY_PORTNUM;pn++){
            rval = tcPhyReadReg(pn,25);
            if ((rval&0xf000)!=tc2105sg_L0R25_default){
                initial_value_flag_p[pn]=0; // wrong initial value
            }
        }
	}
	initial_value_flag = initial_value_flag_p[0]&initial_value_flag_p[1]
		                 &initial_value_flag_p[2]&initial_value_flag_p[3]&initial_value_flag_g;
    return (initial_value_flag);
}

#endif
*/
/*
#ifdef TCPHY_DEBUG
void
tc2104meEsdSwPatch(void)
{
if (!getTcPhyInitialRegFlag()) {
	TCPHYDISP2("ESD detected!!\r\n");
	tcPhyInit(mac_p);
   }
}
#endif
*/
void 
tc2105sgUp2DownFastHbt(uint8 port_num)
{
	const uint8 tc2105sg_Up2Down_Hbt_Thd = 10;
	const uint8 tc2105sg_Up2Down_Hbt_ub = 40;
    if (mr28.lch_linkup_100 & !Nmr1[port_num].link_status) {
		if (tc2105sg_Up2Down_cnt_hbt[port_num]<tc2105sg_Up2Down_Hbt_ub) {
			tc2105sg_Up2Down_cnt_hbt[port_num]+=3;
			}
    	}
	else if (!mr28.lch_linkup_100 & !Nmr1[port_num].link_status) {
		if (tc2105sg_Up2Down_cnt_hbt[port_num]>0) {
			tc2105sg_Up2Down_cnt_hbt[port_num]--;
			}
		}
	else if (mr28.lch_linkup_100 & Nmr1[port_num].link_status) {//adjust link-up time to clear counter
		tc2105sg_Up2Down_cnt_hbt[port_num]=0;
		}
	if (tc2105sg_Up2Down_cnt_hbt[port_num]> tc2105sg_Up2Down_Hbt_Thd) {
		tc2105sg_Up2DownFastHbt_detect[port_num]=1;
		}
	else {
		tc2105sg_Up2DownFastHbt_detect[port_num]=0;
		}  
}

void 
tc2105sgUp2DownFastTbt(uint8 port_num)
{
	const uint8 tc2105sg_Up2Down_Tbt_Thd = 10;
	const uint8 tc2105sg_Up2Down_Tbt_ub = 40;
	const uint8 linkup_check_timer_done = 1;
    if (mr28.lch_linkup_10 & !Nmr1[port_num].link_status) {
		tc2105sg_linkup_check_timer[port_num]=0;
		if (tc2105sg_Up2Down_cnt_tbt[port_num]<tc2105sg_Up2Down_Tbt_ub) {
			tc2105sg_Up2Down_cnt_tbt[port_num]+=5;
			}
    	}
	else if (!mr28.lch_linkup_10 & !Nmr1[port_num].link_status) {
		tc2105sg_linkup_check_timer[port_num]=0;
		if (tc2105sg_Up2Down_cnt_tbt[port_num]>0) {
			tc2105sg_Up2Down_cnt_tbt[port_num]--;
			}
		}
	else if (mr28.lch_linkup_10 & Nmr1[port_num].link_status ) {
		if ((tc2105sg_linkup_check_timer[port_num] == linkup_check_timer_done)) {
			tc2105sg_Up2Down_cnt_tbt[port_num]=0;
			tc2105sg_linkup_check_timer[port_num]=0;
			}
		else if ((tc2105sg_linkup_check_timer[port_num] < linkup_check_timer_done))
			tc2105sg_linkup_check_timer[port_num]++;
		}
	
	if (tc2105sg_Up2Down_cnt_tbt[port_num]> tc2105sg_Up2Down_Tbt_Thd) {
		tc2105sg_Up2DownFastTbt_detect[port_num]=1;
		}
	else {
		tc2105sg_Up2DownFastTbt_detect[port_num]=0;
		}
	
}


bool 
tc2105sgLinkFailDetect (uint8 port_num, uint8 mode){
	switch(mode){
			case TbtOrHbt:
				    if (sw_FixUp2DownFast_flag 
						&(tc2105sg_Up2DownFastTbt_detect[port_num]
						||tc2105sg_Up2DownFastHbt_detect[port_num])) {
						tc2105sg_link_fail_detect_flag =1;
		                #ifdef TCPHY_DEBUG
		                TCPHYDISP3(" tc2105sg_link_fail_detect_flag=%d\r\n",tc2105sg_link_fail_detect_flag); 
		                #endif
						}
					else 
						tc2105sg_link_fail_detect_flag =0;
				break;
			case HbtOnly:
					if (sw_FixUp2DownFast_flag & tc2105sg_Up2DownFastHbt_detect[port_num]) {
						tc2105sg_link_fail_detect_flag =1;
		                #ifdef TCPHY_DEBUG
		                TCPHYDISP3(" tc2105sg_link_fail_detect_flag=%d\r\n",tc2105sg_link_fail_detect_flag); 
		                #endif
						}
					else 
						tc2105sg_link_fail_detect_flag =0;
				break;
			case TbtOnly:
				    if (sw_FixUp2DownFast_flag & tc2105sg_Up2DownFastTbt_detect[port_num]) {
						tc2105sg_link_fail_detect_flag =1;
		                #ifdef TCPHY_DEBUG
		                TCPHYDISP3(" tc2105sg_link_fail_detect_flag=%d\r\n",tc2105sg_link_fail_detect_flag); 
		                #endif
						}
					else 
						tc2105sg_link_fail_detect_flag =0;
				break;
			default: tc2105sg_link_fail_detect_flag =0;
		}
	return tc2105sg_link_fail_detect_flag;
}

void 
tc2105sgDisNextPageNeeded(uint8 port_num)
{
   const uint8 tc2105sgDisNextPage_cntdone = 5;
   
   if((tcphy_link_state[port_num] == ST_LINK_UP)||(tc2105sgDisEEEHappened[port_num])) {
   	tc2105sgDisNextPage_cnt[port_num] = 0;
   	}
   else if ((mr5.LPNextAble) && !(mrl3_18.lp_eee_100||mrl3_18.lp_eee_1000||mrl3_18.lp_eee_10g)
   	&& (tc2105sgDisNextPage_cnt[port_num]<=tc2105sgDisNextPage_cntdone)) {
   	tc2105sgDisNextPage_cnt[port_num]++;
   	}

   if(tc2105sgDisNextPage_cnt[port_num]>tc2105sgDisNextPage_cntdone) {
   	tc2105sgDisNextPage_flag[port_num]=1;
   	}
   else {
   	tc2105sgDisNextPage_flag[port_num]=0;
   	}
}


void 
tc2105sgDisEEENeeded(uint8 port_num)
{
    const uint8 tc2105sg_linkup_check_done = 1;
	
	
    if((tcphy_link_state[port_num] == ST_LINK_UP)
		&& (tc2105sg_linkup_check_cnt[port_num]<tc2105sg_linkup_check_done)) {
		    tc2105sg_linkup_check_cnt[port_num]++;
    	}
	else if (tcphy_link_state[port_num] != ST_LINK_UP){
		  tc2105sg_linkup_check_cnt[port_num] = 0;
    	}
	
    if(tc2105sgDisEEEHappened[port_num] && tc2105sg_linkup_check_cnt[port_num]==tc2105sg_linkup_check_done) {//extend link-up time
		tcPhyWriteLReg(port_num,3,17,tc2105sg_L3R17_EEEAdv);
		tc2105sgDisEEEHappened[port_num]=0;
    	}
	else if(tc2105sgLinkFailDetect(port_num,0) || tc2105sgDisNextPage_flag[port_num]) {
		tcPhyWriteLReg(port_num,3,17,0x0000);//disable next page
		tcPhyWriteLReg(port_num,0,0,0x1200);//re-start AN
		tc2105sgDisEEEHappened[port_num]=1;
    	}
}



void
tc2105sgSwPatch(void)
{
    uint8 pn;

    for(pn=0;pn<TCPHY_PORTNUM;pn++){

        tcPhyReadReg(pn,1);
        //
        if( !Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_DOWN;
        }
        else if( !Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_DOWN2UP;
        }
        else if( Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            tcphy_link_state[pn]= ST_LINK_UP2DOWN;
        }
        else { //if( Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_UP;
        }
    
        // read registers
        // tcPhyReadReg(pn,0);
        tcPhyReadReg(pn,5);
        tcPhyReadReg(pn,28);
		tcPhyReadLReg(pn,3,18);


		//ESD reset needed or EEE disable needed
		if (sw_FixUp2DownFast_flag) {
			tc2105sgDisNextPageNeeded(pn);
			tc2105sgUp2DownFastHbt(pn);
			tc2105sgUp2DownFastTbt(pn);
			tc2105sgDisEEENeeded(pn);
			}

        #ifdef TCPHY_DEBUG
        // display message
        tc2105sgSwDispMessage(pn);
		#endif

#ifdef TCPHY_DEBUG
		if (sw_ErrOverMonitor_flag)
			tc2105sgUpdateErrOverSum(pn);
        if (sw_ErrOverMonitor_flag) 
            tc2105sgSwErrOverMonitor(pn); // call after tc2105sgUpdateErrOverSum()
        
		
#endif

    }
    
}

#endif // TC2105SG_SUPPORT


/************************************************************************
*
*            Variables and Functions for tc2101miSwPatch()
*
*************************************************************************
*/
//#define TC2101MI_CM_SUPPORT
//#define UNH_TEST 1

#ifdef TC2101MI_SUPPORT

#define TC2101MI_PHY_INIT_GDATA_LEN 9
#define TC2101MI_PHY_INIT_LDATA_LEN 6
#define TC2101MI_PHY_INIT_PERPDATA_LEN 1
#define TC2101MI_PHY_INIT_SET_NUM 1
#define RT63260_PORTNUM 1



typedef struct tc2101mi_cfg_data_s{
    char name[10];
    cfg_data_t gdata[TC2101MI_PHY_INIT_GDATA_LEN];
    cfg_data_t ldata[TC2101MI_PHY_INIT_LDATA_LEN];
	cfg_data_t perpdata[TC2101MI_PHY_INIT_PERPDATA_LEN]; //per port register setting
}tc2101mi_cfg_data_t;

const uint32 tc2101mi_page_sel_addr=31;

// G0R22b11: tx10_eee_mode
const uint16 tc2101mi_G0R22_Tx10AmpSave_ON  = 0x0264;
const uint16 tc2101mi_G0R22_Tx10AmpSave_OFF = 0x006F;
const uint32 tc2101mi_L3R17_EEEAdv = 0x0002;

#define tc2101mi_G3R17_default 0x7238
#define tc2101mi_L0R25_default 0xa000

tc2101mi_cfg_data_t tc2101mi_cfg[TC2101MI_PHY_INIT_SET_NUM]={
    {{"I3.0"},
     { {31,0x3000},{17,tc2101mi_G3R17_default},{21,0x2005},{27,0x0002},
       {31,0x1000},{20,0x0914},{24,0x0011},{27,0x0246},
       {31,0x0000}
     },
     { {31,0x9000},{16,0x0001},{22,0x0140},{23,0x0400},
       {31,0x8000},{25,tc2101mi_L0R25_default}
     },
     { {31,0x8000}
	 }
    }, 
};

#endif

#ifdef TC2101MI_SUPPORT

static int32 tc2101mi_erroversum=0;

//reg5 bug patch
static uint32 tc2101mi_reg5_val = 0;


static void tc2101miConfig(int argc, char *argv[], void *p);
void tc2101miCfgLoad(uint8 idx);
void tc2101miGRCfgCheck(void);
void tc2101miLRCfgCheck(uint8 port_num);
void tc2101miCfgCheck(void);
static void tc2101miLRCfgLoad(uint8 idx, uint8 port_num);
static void tc2101miPerPortCfgLoad(uint8 idx, uint8 port_num);
int32 tc2101miReadProbe(uint8 port_num, uint8 mode);
static void tc2101miCfgTx10AmpSave(void);
//static void tc2105sgCfgLDPS(void);
#ifdef TCPHY_DEBUG
int32 tc2101miReadErrOverSum(uint8 port_num);
int32 tc2101miReadAdcSum(uint8 port_num);
void tc2101miDispProbe100(uint8 port_num, bool long_msg);
void tc2101miPoweDown(uint8 port_num);
void tc2101miSwDispMessage(uint8 port_num);
uint16 tc2101miReadSnrSum(uint8 port_num, uint16 cnt);
void tc2101miUpdateErrOverSum(uint8 port_num);
void tc2101miSwErrOverMonitor(uint8 port_num);
//bool tc2105sgInitialRegFlag(void);
//void tc2105sgEsdSwPatch(void);
#endif
void tc2101miLchRmCapReg(void);
void tc2101miSwPatch(void);


static void 
tc2101miConfig(int argc, char *argv[], void *p)
{
    // global config in doPhyConfig()
#ifdef TCPHY_DEBUG
    if (argc==1) {
        TCPHYDISP3("Usage: config Tx10AmpSave [on|off]\r\n");
		//TCPHYDISP3("       config EsdPhyRegInit [on|off]\r\n");
		//TCPHYDISP3("       config FixUp2DownFast [on|off]\r\n");
		//TCPHYDISP3("       config LinkDownPS [on|off]\r\n");
		//TCPHYDISP3("       config NextPageAutoDisable [on|off]\r\n");
        return;
    }
    else if (argc==2) {
        if (stricmp(argv[1], "list") == 0){
            TCPHYDISP3("config Tx10AmpSave : %s\r\n",
                    (cfg_Tx10AmpSave_flag?"on":"off"));  
			//TCPHYDISP3("config EsdPhyRegInit : %s\r\n",
            //        (tc2104me_esdphy_init_flag?"on":"off"));  
			//TCPHYDISP3("config FixUp2DownFast : %s\r\n",
            //        (sw_FixUp2DownFast_flag?"on":"off"));
			//TCPHYDISP3("config LinkDownPS : %s\r\n",
            //        (cfg_LDPS_flag?"on":"off"));
            return;
        }
    }
#endif

    if (argc==2 || argc==3){
        if (stricmp(argv[1], "Tx10AmpSave") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                cfg_Tx10AmpSave_flag = 1; // enable
                tc2101miCfgTx10AmpSave();
            }
            else if( stricmp(argv[2], "off") == 0 ){
                cfg_Tx10AmpSave_flag = 0;
                tc2101miCfgTx10AmpSave();
            }
            TCPHYDISP2("config Tx10AmpSave : %s\r\n",
                        (cfg_Tx10AmpSave_flag?"on":"off"));
        }
		/*if (stricmp(argv[1], "EsdPhyRegInit") == 0) {
			if( stricmp(argv[2], "on") == 0 ){
                tc2104me_esdphy_init_flag= 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                tc2104me_esdphy_init_flag= 0;
            }
            TCPHYDISP2("config EsdPhyRegInit : %s\r\n",
                        (tc2104me_esdphy_init_flag?"on":"off"));
		}*/
		/*if (stricmp(argv[1], "FixUp2DownFast") == 0) {
			if( stricmp(argv[2], "on") == 0 ){
                sw_FixUp2DownFast_flag= 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                sw_FixUp2DownFast_flag= 0;
            }
            TCPHYDISP2("config FixUp2DownFast : %s\r\n",
                        (sw_FixUp2DownFast_flag?"on":"off"));
		}*/
	   /*if (stricmp(argv[1], "LinkDownPS") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                cfg_LDPS_flag = 1; // enable
                tc2104meCfgLDPS();
            }
             else if( stricmp(argv[2], "off") == 0 ){
                cfg_LDPS_flag = 0; // disable
                tc2104meCfgLDPS();
            }
            TCPHYDISP2("config LinkDownPS : %s\r\n",
                        (cfg_LDPS_flag?"on":"off"));
        }*/
    }
}

// 1. loading default register setting in tcPhyInit()
// 2. changing register setting
void 
tc2101miCfgLoad(uint8 idx)
{
    int i;
    uint16 phyAddr;
	current_idx = idx;
	
    // global registers
    phyAddr = mac_p->enetPhyAddr;

    for( i=0; i<TC2101MI_PHY_INIT_GDATA_LEN; i++ ){         
        tcMiiStationWrite(phyAddr, tc2101mi_cfg[current_idx].gdata[i].reg_num, tc2101mi_cfg[current_idx].gdata[i].val );
    }
    //     
    tc2101miCfgTx10AmpSave();

    // local registers 
    tc2101miLRCfgLoad(current_idx, phyAddr);

    TCPHYDISP4("tcphy: CfgLoad %s\r\n",  tc2101mi_cfg[current_idx].name);

#ifdef UNH_TEST
    phyAddr = 0;
    sw_ErrOverMonitor_flag=0;
	sw_FixUp2DownFast_flag= 0;
	cfg_Tx10AmpSave_flag=0; // disable tx10_eee_mode
	tc2101miCfgTx10AmpSave();
	
	
    //tcPhyWriteGReg(phyAddr,0,22,0x0664); // disable tx10_eee_mode
    //tcPhyWriteGReg(phyAddr,0,26,0x0012); // disable cr_signal_status_opt
    tcPhyWriteGReg(phyAddr,1,22,0x0010); // enable RX_ER extension to Pass PCS pause test
    //tcPhyWriteGReg(phyAddr,3,21,0x7387); // decrease I2MB to pass template
	

    tcPhyWriteLReg(phyAddr,0,30,0x0000); // disable AAPS to avoid varaince lc_max 
    tcPhyWriteLReg(phyAddr,0,26,0x1200); // disable Link_Det_Ext function

    TCPHYDISP4("tcphy: CfgLoad UNH-IOL setting\r\n");
#endif
}

void
tc2101miGRCfgCheck(void)
{
    int i;
	uint32 phyaddr = mac_p->enetPhyAddr;
	uint32 rval;
	current_idx = 0; 
	
	for( i=0; i<TC2101MI_PHY_INIT_GDATA_LEN; i++){
		if(tc2101mi_cfg[current_idx].gdata[i].reg_num == tc2101mi_page_sel_addr) {
			tcMiiStationWrite(phyaddr, tc2101mi_cfg[current_idx].gdata[i].reg_num, tc2101mi_cfg[current_idx].gdata[i].val);
		}//if reg_num == page_sel_addr
		else {
			rval = tcMiiStationRead(phyaddr, tc2101mi_cfg[current_idx].gdata[i].reg_num);
			if(rval!=tc2101mi_cfg[current_idx].gdata[i].val){
				printf("Wrong initial setting of global register %d is detected!!\r\n",tc2101mi_cfg[current_idx].gdata[i].reg_num);
				}//rval==gdata
			}
		}//for
}

void 
tc2101miLRCfgCheck(uint8 port_num)
{
	 uint32 phyaddr = mac_p->enetPhyAddr+port_num;
	 uint32 rval;
/*
	 // check L0R25 
     rval = tcPhyReadReg(phyaddr,25);
     if ((rval&0xf000)!=tc2101mi_L0R25_default){
	 	printf("Wrong initial setting of Local register 25 at port%d is detected!!\r\n",port_num);
        }
        */
}

void 
tc2101miCfgCheck(void)
{
     int pn;
	 tc2101miGRCfgCheck();
	 
	 for( pn=0; pn<RT63260_PORTNUM; pn++){
	 	tc2101miLRCfgCheck(pn);
	 	}
}


static void
tc2101miLRCfgLoad(uint8 idx, uint8 port_num)
{
    int i;
	current_idx = idx;
	for( i=0; i<TC2101MI_PHY_INIT_LDATA_LEN; i++ ){         
            tcMiiStationWrite(port_num, tc2101mi_cfg[current_idx].ldata[i].reg_num, tc2101mi_cfg[current_idx].ldata[i].val );
        }   
}

static void 
tc2101miPerPortCfgLoad(uint8 idx, uint8 port_num)
{
   int i;
   current_idx = idx;
   for( i=0; i<TC2101MI_PHY_INIT_PERPDATA_LEN; i++ ){         
            tcMiiStationWrite(port_num, tc2101mi_cfg[current_idx].perpdata[i].reg_num, tc2101mi_cfg[current_idx].perpdata[i].val );
        }  
}


static void
tc2101miCfgTx10AmpSave(void)
{
    uint16 phyAddr = mac_p->enetPhyAddr;

    if (cfg_Tx10AmpSave_flag==1){ // enable
        tcPhyWriteGReg(phyAddr,0,22,tc2101mi_G0R22_Tx10AmpSave_ON);
        TCPHYDISP4("tcphy: Tx10AmpSave enable!\r\n");       
    }
    else { // disable
        tcPhyWriteGReg(phyAddr,0,22,tc2101mi_G0R22_Tx10AmpSave_OFF);
        TCPHYDISP4("tcphy: Tx10AmpSave disable!\r\n");      
    }
}


int32
tc2101miReadProbe(uint8 port_num, uint8 mode)
{
    uint32 val, val_r31, val_g0r28;
    uint32 rval, wval;
    uint32 phyaddr = mac_p->enetPhyAddr + port_num;
    
    val_r31 = tcMiiStationRead(phyaddr, 31); 
    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    val_g0r28 = tcMiiStationRead(phyaddr, 28);
	
    
    switch(mode){
        case ProbeZfgain:
            wval = 0x0b04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x3f ;
            break;
        case ProbeAgccode:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>1)&0x1f ;
            break;
        case ProbeBoosten:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>6)&0x01 ;
            break;
        break;
        case ProbeSnr:
            wval = 0x0904 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0xff ;
            break;  
        case ProbeAdcSign:
            wval = 0x4104 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x7f ;
            if (rval>64){
                rval -= 128;
            }
            break;
        default:
            TCPHYDISP1("\r\ntcphy error: ReadProbe %d.\r\n",mode);
            rval = 0;
            break;
    }

    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    tcMiiStationWrite( phyaddr, 28, val_g0r28); // restore G0Reg28
    tcMiiStationWrite( phyaddr, 31, val_r31); // restore Reg31
    return rval;

}

#ifdef TCPHY_DEBUG
int32
tc2101miReadErrOverSum(uint8 port_num)
{
    int32 err_over_sum;
    
    tcPhyReadReg(port_num,25);
    pause(300);
    tcPhyReadReg(port_num,25);
    err_over_sum = Nmr25[port_num].err_over_cnt - Nmr25[port_num].err_over_cnt_prev;
    if( err_over_sum < 0 ){
        err_over_sum += 2048;
    }

    return err_over_sum;
}
#endif

#ifdef TCPHY_DEBUG
int32
tc2101miReadAdcSum(uint8 port_num)
{
    int32 cnt=1000;
    int32 AdcSign_sum = 0;
    int j;
      
    for(j=0;j<cnt;j++){
        AdcSign_sum += tc2101miReadProbe(port_num,ProbeAdcSign);
    }
    //shift right to show percent of the dc offset (unit:%)
    AdcSign_sum = (AdcSign_sum>>6);
      
    return AdcSign_sum;
}

#endif

#ifdef TCPHY_DEBUG
uint16
tc2101miReadSnrSum(uint8 port_num, uint16 cnt)
{
    uint16 snr_sum = 0;
    uint16 j;
    
    for(j=0;j<cnt;j++) {
      snr_sum += tc2101miReadProbe(port_num,ProbeSnr);
    }
    return snr_sum;

}
#endif

#ifdef TCPHY_DEBUG
void 
tc2101miDispProbe100(uint8 port_num, bool long_msg)
{

    const uint16 tc2101miReadSnrCnt = 1000;

    TCPHYDISP2("tcphy[%d]:",port_num);
    TCPHYDISP2(" boosten=%ld", tc2101miReadProbe(port_num,ProbeBoosten));
    TCPHYDISP2(" agccode=%ld", tc2101miReadProbe(port_num,ProbeAgccode));
    TCPHYDISP2(" zfgain=%ld", tc2101miReadProbe(port_num,ProbeZfgain));
    TCPHYDISP2(" ch_idx=%ld", (tcPhyReadReg(port_num,29)&0x003f));
    TCPHYDISP2(" snr=%ld", tc2101miReadProbe(port_num,ProbeSnr));
    if (long_msg){
        TCPHYDISP2(" err_over_sum=%ld", tc2101miReadErrOverSum(port_num));
    }
    else {
        tcPhyReadReg(port_num,25);
    }
    TCPHYDISP2(" \r\n");
    if (long_msg){
        TCPHYDISP3(" snr_sum(x1000)=%d",tc2101miReadSnrSum(port_num,tc2101miReadSnrCnt)); // snr_sum   
        TCPHYDISP4(" adc_avg=%ld/1000", tc2101miReadAdcSum(port_num));  
        TCPHYDISP3(" \r\n");
    }
}
#endif

#ifdef TCPHY_DEBUG
void
tc2101miSwDispMessage(uint8 port_num)
{
    switch (tcphy_link_state[port_num]) {
        case ST_LINK_DOWN: 
            break;
            
        case ST_LINK_DOWN2UP: 
            //tcPhyReadReg(port_num,28); read in tc2104meSwPatch()
            TCPHYDISP1("tcphy[%d]: Link-up at %d%s.",
                port_num,(mr28.final_speed?100:10),(mr28.final_duplex?"F":"H")); 
			
			TCPHYDISP1(" tx_amp_save=%d.",(mr28.tx_amp_save));          
            TCPHYDISP1("\r\n");
			
            if(mr28.final_speed) {
                tc2101miDispProbe100(port_num,0); // short message
            }
			
            break;
            
        case ST_LINK_UP: 
            break;
            
        case ST_LINK_UP2DOWN:
            TCPHYDISP1("tcphy[%d]: Link-down!!!\r\n",port_num);
            break;
            
        default: printf("\r\ntcphy error: SwDispMessage error!\r\n");
    }
}
#endif

#ifdef TCPHY_DEBUG
void
tc2101miPoweDown(uint8 port_num)
{
    uint32 val_l0r04;
	val_l0r04 = tcPhyReadLReg(port_num, 0, 4);
	tcPhyWriteLReg(port_num,0,4,0x0001); // 	Set FLP register to disable all capability
    tcPhyWriteLReg(port_num,0,0,0x1200); //  restart-AN
    tcPhyWriteLReg(port_num,0,0,0x1800); //  power-down
    tcPhyWriteLReg(port_num,0,4,val_l0r04); //  Set FLP register to its default value
}
#endif 

#ifdef TCPHY_DEBUG
void
tc2101miUpdateErrOverSum(uint8 port_num)
{
    uint16 err_over_cnt_prev; // to store previous err_over_cnt value
    int32 val;
    
    // clear Nmr25[port_num].err_over_cnt in tc2104meDispProbe100() in ST_LINK_DOWN2UP
    if ((mr28.final_speed == 1) // 100BaseTX
        && (tcphy_link_state[port_num] == ST_LINK_UP)) 
    {
        err_over_cnt_prev = Nmr25[port_num].err_over_cnt;
        tcPhyReadReg(port_num,25);
        val = Nmr25[port_num].err_over_cnt - err_over_cnt_prev;
        if( val < 0 ){
            val += 2048;
        }
        tc2101mi_erroversum = val;
    }
    else {
        tc2101mi_erroversum = -1;
    }
}
#endif


#ifdef TCPHY_DEBUG
void
tc2101miSwErrOverMonitor(uint8 port_num)
{
    if (tcPhy_disp_level<3) 
        return; // inactive

    if (tc2101mi_erroversum>0){
        TCPHYDISP3("tcphy[%d]: ErrOver=%ld\r\n",port_num,tc2101mi_erroversum);          
    }

}
#endif
/*
#ifdef TCPHY_DEBUG
bool 
tc2105sgInitialRegFlag(void)
{
    bool initial_value_flag=1; // default: correct initial value
    bool initial_value_flag_p[4]={1,1,1,1};
	bool initial_value_flag_g=1;
    uint8 pn;
    uint16 rval;

	if (eco_rev == 0x00){//tc2206F A1
		// check G3R23
        rval = tcPhyReadGReg(0,3,23);
        if (rval != tc2105sg_G3R23_default){
            initial_value_flag_g=0; // wrong initial value
            }
		
       // check L0R25
        for(pn=0;pn<TCPHY_PORTNUM;pn++){
            rval = tcPhyReadReg(pn,25);
            if ((rval&0xf000)!=tc2105sg_L0R25_default){
                initial_value_flag_p[pn]=0; // wrong initial value
            }
        }
	}
	initial_value_flag = initial_value_flag_p[0]&initial_value_flag_p[1]
		                 &initial_value_flag_p[2]&initial_value_flag_p[3]&initial_value_flag_g;
    return (initial_value_flag);
}

#endif
*/
/*
#ifdef TCPHY_DEBUG
void
tc2104meEsdSwPatch(void)
{
if (!getTcPhyInitialRegFlag()) {
	TCPHYDISP2("ESD detected!!\r\n");
	tcPhyInit(mac_p);
   }
}
#endif
*/
void
tc2101miLchRmCapReg(void)
{
uint8 pn=0;
	if((tcphy_link_state[pn] == ST_LINK_DOWN2UP)) {
		if( (mr28.final_speed)&&(mr28.final_duplex) )
				tc2101mi_reg5_val = 0x00004101;
        else if( (mr28.final_speed)&& !(mr28.final_duplex) )
			    tc2101mi_reg5_val = 0x00004081;
		else if( !(mr28.final_speed)&& (mr28.final_duplex) )
			    tc2101mi_reg5_val = 0x00004041;
		else if( !(mr28.final_speed)&& !(mr28.final_duplex) )
			    tc2101mi_reg5_val = 0x00004021;
			}
	else if((tcphy_link_state[pn] == ST_LINK_UP2DOWN)) {
		tc2101mi_reg5_val = 0x00000000;
		}

tc2101mi_reg5_val=(tc2101mi_reg5_val & 0xffff);

}

void
tc2101miSwPatch(void)
{
    uint8 pn;

    for(pn=0;pn<RT63260_PORTNUM;pn++){

        tcPhyReadReg(pn,1);
        //
        if( !Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_DOWN;
        }
        else if( !Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_DOWN2UP;
        }
        else if( Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            tcphy_link_state[pn]= ST_LINK_UP2DOWN;
        }
        else { //if( Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_UP;
        }
    
        // read registers
        // tcPhyReadReg(pn,0);
        tcPhyReadReg(pn,5);
        tcPhyReadReg(pn,28);
		tcPhyReadLReg(pn,3,18);


		//ESD reset needed or EEE disable needed


        #ifdef TCPHY_DEBUG
        // display message
        tc2101miSwDispMessage(pn);
		#endif

#ifdef TCPHY_DEBUG
		if (sw_ErrOverMonitor_flag)
			tc2101miUpdateErrOverSum(pn);
        if (sw_ErrOverMonitor_flag) 
            tc2101miSwErrOverMonitor(pn); // call after tc2101miUpdateErrOverSum()
        
		
#endif

    }
    
}

#endif // TC2101MI_SUPPORT

/************************************************************************
*
*            Variables and Functions for tc2105mjSwPatch()
*
*************************************************************************
*/
#ifdef TC2105MJ_SUPPORT

#define TC2105MJ_PHY_INIT_GDATA_LEN 15
#define TC2105MJ_PHY_INIT_LDATA_LEN 1
#define TC2105MJ_PHY_INIT_PERPDATA_LEN 1
#define TC2105MJ_PHY_INIT_SET_NUM 1
#define TC2105MJ_PORTNUM 5

typedef struct tc2105mj_cfg_data_s{
    char name[10];
    cfg_data_t gdata[TC2105MJ_PHY_INIT_GDATA_LEN];
    cfg_data_t ldata[TC2105MJ_PHY_INIT_LDATA_LEN];
	cfg_data_t perpdata[TC2105MJ_PHY_INIT_PERPDATA_LEN]; //per port register setting
}tc2105mj_cfg_data_t;

const uint32 tc2105mj_page_sel_addr=31;
// G0R22b11: tx10_eee_mode
const uint16 tc2105mj_G0R22_Tx10AmpSave_ON  = 0x0264;
const uint16 tc2105mj_G0R22_Tx10AmpSave_OFF = 0x006F;

const uint16 tc2105mj_L2R16[TC2105MJ_PORTNUM] = {0x0e0e, 0x0c0c, 0x0f0f, 0x1010, 0x0909};
const uint16 tc2105mj_L2R17_A1[TC2105MJ_PORTNUM] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
const uint16 tc2105mj_L2R17_A2[TC2105MJ_PORTNUM] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
const uint32 tc2105mj_G4R21_A1 = 0x7160;
const uint32 tc2105mj_G4R21_A2 = 0x7160;
const uint32 tc2105mj_G4R25_A1 = 0x0102;
const uint32 tc2105mj_G4R25_A2 = 0x0212;
const uint32 tc2105mj_G4R29_A1 = 0x8641;
const uint32 tc2105mj_G4R29_A2 = 0x4640;

tc2105mj_cfg_data_t tc2105mj_cfg[TC2105MJ_PHY_INIT_SET_NUM]={
    {{"J8.0"},
     { {31,0x4000}, {16,0xD4CC}, {17,0x7444}, {19,0x0112}, {22,0x10cf}, {26,0x0777}, 
       {31,0x2000}, {21,0x0655}, {22,0x0fd3}, {23,0x003d}, {24,0x096e}, {25,0x0fed}, {26,0x0fc4},
       {31,0x1000}, {17,0xe7f8}
     },
     { {31,0x9000}
     },
     { {31,0x8000}
	 }
    }, 
};
#endif

#ifdef TC2105MJ_SUPPORT

static int32 tc2105mj_erroversum=0;

static void tc2105mjConfig(int argc, char *argv[], void *p);
void tc2105mjCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num);
static void tc2105mjLRCfgLoad(uint8 idx, uint8 port_num);
static void tc2105mjCfgTx10AmpSave(void);
void tc2105mjGRCfgCheck(void);
void tc2105mjLRCfgCheck(uint8 port_num);
void tc2105mjCfgCheck(void);
int32 tc2105mjReadProbe(uint8 port_num, uint8 mode);
#ifdef TCPHY_DEBUG
int32 tc2105mjReadErrOverSum(uint8 port_num);
int32 tc2105mjReadAdcSum(uint8 port_num);
void tc2105mjDispProbe100(uint8 port_num, bool long_msg);
void tc2105mjSwDispMessage(uint8 port_num);
uint16 tc2105mjReadSnrSum(uint8 port_num, uint16 cnt);
void tc2105mjUpdateErrOverSum(uint8 port_num);
void tc2105mjSwErrOverMonitor(uint8 port_num);
#endif //TCPHY_DEBUG
void tc2105mjSwPatch(void);

static void 
tc2105mjConfig(int argc, char *argv[], void *p)
{
    // global config in doPhyConfig()
#ifdef TCPHY_DEBUG
    if (argc==1) {
        TCPHYDISP3("Usage: config Tx10AmpSave [on|off]\r\n");
        return;
    }
    else if (argc==2) {
        if (stricmp(argv[1], "list") == 0){
            TCPHYDISP3("config Tx10AmpSave : %s\r\n",
                    (cfg_Tx10AmpSave_flag?"on":"off"));  
            return;
        }
    }
#endif

    if (argc==2 || argc==3){
        if (stricmp(argv[1], "Tx10AmpSave") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                cfg_Tx10AmpSave_flag = 1; // enable
                tc2105mjCfgTx10AmpSave();
            }
            else if( stricmp(argv[2], "off") == 0 ){
                cfg_Tx10AmpSave_flag = 0;
                tc2105mjCfgTx10AmpSave();
            }
            TCPHYDISP2("config Tx10AmpSave : %s\r\n",
                        (cfg_Tx10AmpSave_flag?"on":"off"));
        }
    }
}

// 1. loading default register setting in tcPhyInit()
// 2. changing register setting
void 
tc2105mjCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num)
{
    int pn,i;
    uint16 phyAddr;
	current_idx = idx;
	
    // global registers
    phyAddr = mac_p->enetPhyAddr;

    for( i=0; i<TC2105MJ_PHY_INIT_GDATA_LEN; i++ ){         
        tcMiiStationWrite(phyAddr, tc2105mj_cfg[current_idx].gdata[i].reg_num, tc2105mj_cfg[current_idx].gdata[i].val );
    }
    //     
    tc2105mjCfgTx10AmpSave();
	
      // local registers 
    if (doPerPort_flag == DO_PER_PORT) {
		tc2105mjLRCfgLoad(current_idx, port_num);
    	}
	else {
		 for( pn=0; pn<TC2105MJ_PORTNUM; pn++){
		 	phyAddr = pn; //mac_p->enetPhyAddr + pn;
		 	tc2105mjLRCfgLoad(current_idx, phyAddr);
			}
		 }
    // load revision-related settings
    if (eco_rev == 0x00) { //rev=A1
                tcPhyWriteGReg(phyAddr,4,21,tc2105mj_G4R21_A1);
                tcPhyWriteGReg(phyAddr,4,25,tc2105mj_G4R25_A1);
		tcPhyWriteGReg(phyAddr,4,29,tc2105mj_G4R29_A1);
    } 
	else { //rev=A2
	        tcPhyWriteGReg(phyAddr,4,21,tc2105mj_G4R21_A2);
	        tcPhyWriteGReg(phyAddr,4,25,tc2105mj_G4R25_A2);
		tcPhyWriteGReg(phyAddr,4,29,tc2105mj_G4R29_A2);
	}
    TCPHYDISP4("tcphy: CfgLoad %s\r\n",  tc2105mj_cfg[current_idx].name);

}

static void
tc2105mjLRCfgLoad(uint8 idx, uint8 port_num)
{
    int i;
	current_idx = idx;
	for( i=0; i<TC2105MJ_PHY_INIT_LDATA_LEN; i++ ){         
            tcMiiStationWrite(port_num, tc2105mj_cfg[current_idx].ldata[i].reg_num, tc2105mj_cfg[current_idx].ldata[i].val );
        }   
	tcPhyWriteLReg(port_num,2,16,tc2105mj_L2R16[port_num]);
	    // load revision-related settings
    if (eco_rev == 0x00) { //rev=A1
	tcPhyWriteLReg(port_num,2,17,tc2105mj_L2R17_A1[port_num]);
   }
    else {  //rev=A2
	tcPhyWriteLReg(port_num,2,17,tc2105mj_L2R17_A2[port_num]);
   }
}

static void
tc2105mjCfgTx10AmpSave(void)
{
    uint16 phyAddr = mac_p->enetPhyAddr;

    if (cfg_Tx10AmpSave_flag==1){ // enable
        tcPhyWriteGReg(phyAddr,0,22,tc2105mj_G0R22_Tx10AmpSave_ON);
        TCPHYDISP4("tcphy: Tx10AmpSave enable!\r\n");       
    }
    else { // disable
        tcPhyWriteGReg(phyAddr,0,22,tc2105mj_G0R22_Tx10AmpSave_OFF);
        TCPHYDISP4("tcphy: Tx10AmpSave disable!\r\n");      
    }
}


void
tc2105mjGRCfgCheck(void)
{
    int i;
	uint32 phyaddr = mac_p->enetPhyAddr;
	uint32 rval;
	current_idx = 0; 
	
	for( i=0; i<TC2105MJ_PHY_INIT_GDATA_LEN; i++){
		if(tc2105mj_cfg[current_idx].gdata[i].reg_num == tc2105mj_page_sel_addr) {
			tcMiiStationWrite(phyaddr, tc2105mj_cfg[current_idx].gdata[i].reg_num, tc2105mj_cfg[current_idx].gdata[i].val);
		}//if reg_num == page_sel_addr
		else {
			rval = tcMiiStationRead(phyaddr, tc2105mj_cfg[current_idx].gdata[i].reg_num);
			if(rval!=tc2105mj_cfg[current_idx].gdata[i].val){
				printf("Wrong initial setting of global register %d is detected!!\r\n",tc2105mj_cfg[current_idx].gdata[i].reg_num);
				}//rval==gdata
			}
		}//for
	if(eco_rev == 0x00) { //rev=A1
		tcMiiStationWrite(phyaddr, 31, 0x4000);
		rval = tcMiiStationRead(phyaddr, 29);
		if (rval!=tc2105mj_G4R29_A1){
			printf("Wrong initial setting of global register 29 is detected!!\r\n");
		}
	}
	else { //rev=A2
		tcMiiStationWrite(phyaddr, 31, 0x4000);
		rval = tcMiiStationRead(phyaddr, 29);
		if (rval!=tc2105mj_G4R29_A2){
			printf("Wrong initial setting of global register 29 is detected!!\r\n");
		}	
	}
	printf("Check done!!\r\n");
}


void 
tc2105mjLRCfgCheck(uint8 port_num)
{
	 uint32 phyaddr = mac_p->enetPhyAddr+port_num;
	 uint32 rval;
/*
	 // check L0R25 
     rval = tcPhyReadReg(phyaddr,25);
     if ((rval&0xf000)!=tc2105sg_L0R25_default){
	 	printf("Wrong initial setting of Local register 25 at port%d is detected!!\r\n",port_num);
        }
        */
}

void 
tc2105mjCfgCheck(void)
{
     int pn;
	 tc2105mjGRCfgCheck();
	 
	 for( pn=0; pn<TC2105MJ_PORTNUM; pn++){
	 	tc2105mjLRCfgCheck(pn);
	 	}
}

int32
tc2105mjReadProbe(uint8 port_num, uint8 mode)
{
    uint32 val, val_r31, val_g0r28;
    uint32 rval, wval;
    uint32 phyaddr = mac_p->enetPhyAddr + port_num;
    
    val_r31 = tcMiiStationRead(phyaddr, 31); 
    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    val_g0r28 = tcMiiStationRead(phyaddr, 28);
	
    
    switch(mode){
        case ProbeZfgain:
            wval = 0x0b04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x3f ;
            break;
        case ProbeAgccode:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>1)&0x1f ;
            break;
        case ProbeBoosten:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>6)&0x01 ;
            break;
        break;
        case ProbeSnr:
            wval = 0x0904 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0xff ;
            break;  
        case ProbeAdcSign:
            wval = 0x4104 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x7f ;
            if (rval>64){
                rval -= 128;
            }
            break;
        default:
            TCPHYDISP1("\r\ntcphy error: ReadProbe %d.\r\n",mode);
            rval = 0;
            break;
    }

    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    tcMiiStationWrite( phyaddr, 28, val_g0r28); // restore G0Reg28
    tcMiiStationWrite( phyaddr, 31, val_r31); // restore Reg31
    return rval;

}

#ifdef TCPHY_DEBUG
int32
tc2105mjReadErrOverSum(uint8 port_num)
{
    int32 err_over_sum;
    
    tcPhyReadReg(port_num,25);
    pause(300);
    tcPhyReadReg(port_num,25);
    err_over_sum = Nmr25[port_num].err_over_cnt - Nmr25[port_num].err_over_cnt_prev;
    if( err_over_sum < 0 ){
        err_over_sum += 2048;
    }

    return err_over_sum;
}
#endif

#ifdef TCPHY_DEBUG
int32
tc2105mjReadAdcSum(uint8 port_num)
{
    int32 cnt=1000;
    int32 AdcSign_sum = 0;
    int j;
	uint32 val_g3r20, val_g3r20_newval, val_l0r30, val_l1r22;
	
    val_g3r20=tcPhyReadGReg(port_num,3,20);
	val_g3r20_newval = (val_g3r20) & 0x7fff;
	tcPhyWriteGReg(port_num,3,20,val_g3r20_newval);//switch to full AD

	val_l0r30=tcPhyReadLReg(port_num,0,30);
	tcPhyWriteLReg(port_num,0,30,0x1510);//power down buffer
	
	val_l1r22=tcPhyReadLReg(port_num,1,22);
	tcPhyWriteLReg(port_num,1,22,0x000c);//force HP
	
	
    for(j=0;j<cnt;j++){
        AdcSign_sum += tc2105mjReadProbe(port_num,ProbeAdcSign);
    }
    //shift right to show percent of the dc offset (unit:%)
    AdcSign_sum = (AdcSign_sum>>6);

	tcPhyWriteGReg(port_num,3,20,val_g3r20);
	tcPhyWriteLReg(port_num,0,30,val_l0r30);
	tcPhyWriteLReg(port_num,1,22,val_l1r22);
	
      
    return AdcSign_sum;
}

#endif

#ifdef TCPHY_DEBUG
uint16
tc2105mjReadSnrSum(uint8 port_num, uint16 cnt)
{
    uint16 snr_sum = 0;
    uint16 j;
    
    for(j=0;j<cnt;j++) {
      snr_sum += tc2105mjReadProbe(port_num,ProbeSnr);
    }
    return snr_sum;

}
#endif

#ifdef TCPHY_DEBUG
void 
tc2105mjDispProbe100(uint8 port_num, bool long_msg)
{

    const uint16 tc2105mjReadSnrCnt = 1000;

    TCPHYDISP2("tcphy[%d]:",port_num);
    TCPHYDISP2(" boosten=%ld", tc2105mjReadProbe(port_num,ProbeBoosten));
    TCPHYDISP2(" agccode=%ld", tc2105mjReadProbe(port_num,ProbeAgccode));
    TCPHYDISP2(" zfgain=%ld", tc2105mjReadProbe(port_num,ProbeZfgain));
    TCPHYDISP2(" ch_idx=%ld", (tcPhyReadReg(port_num,29)&0x003f));
    TCPHYDISP2(" snr=%ld", tc2105mjReadProbe(port_num,ProbeSnr));
    if (long_msg){
        TCPHYDISP2(" err_over_sum=%ld", tc2105mjReadErrOverSum(port_num));
    }
    else {
        tcPhyReadReg(port_num,25);
    }
    TCPHYDISP2(" \r\n");
    if (long_msg){
        TCPHYDISP3(" snr_sum(x1000)=%d",tc2105mjReadSnrSum(port_num,tc2105mjReadSnrCnt)); // snr_sum   
        TCPHYDISP4(" adc_avg=%ld/1000", tc2105mjReadAdcSum(port_num));  
        TCPHYDISP3(" \r\n");
    }
}
#endif

#ifdef TCPHY_DEBUG
void
tc2105mjSwDispMessage(uint8 port_num)
{
    switch (tcphy_link_state[port_num]) {
        case ST_LINK_DOWN: 
            break;
            
        case ST_LINK_DOWN2UP: 
            //tcPhyReadReg(port_num,28); read in tc2104meSwPatch()
            TCPHYDISP1("tcphy[%d]: Link-up at %d%s.",
                port_num,(mr28.final_speed?100:10),(mr28.final_duplex?"F":"H")); 
			
			TCPHYDISP1(" tx_amp_save=%d.",(mr28.tx_amp_save));          
            TCPHYDISP1("\r\n");
			
            if(mr28.final_speed) {
                tc2105mjDispProbe100(port_num,0); // short message
            }
			
            break;
            
        case ST_LINK_UP: 
            break;
            
        case ST_LINK_UP2DOWN:
            TCPHYDISP1("tcphy[%d]: Link-down!!!\r\n",port_num);
            break;
            
        default: printf("\r\ntcphy error: SwDispMessage error!\r\n");
    }
}
#endif

#ifdef TCPHY_DEBUG
void
tc2105mjUpdateErrOverSum(uint8 port_num)
{
    uint16 err_over_cnt_prev; // to store previous err_over_cnt value
    int32 val;
    
    // clear Nmr25[port_num].err_over_cnt in tc2104meDispProbe100() in ST_LINK_DOWN2UP
    if ((mr28.final_speed == 1) // 100BaseTX
        && (tcphy_link_state[port_num] == ST_LINK_UP)) 
    {
        err_over_cnt_prev = Nmr25[port_num].err_over_cnt;
        tcPhyReadReg(port_num,25);
        val = Nmr25[port_num].err_over_cnt - err_over_cnt_prev;
        if( val < 0 ){
            val += 2048;
        }
        tc2105mj_erroversum = val;
    }
    else {
        tc2105mj_erroversum = -1;
    }
}
#endif


#ifdef TCPHY_DEBUG
void
tc2105mjSwErrOverMonitor(uint8 port_num)
{
    if (tcPhy_disp_level<3) 
        return; // inactive

    if (tc2105mj_erroversum>0){
        TCPHYDISP3("tcphy[%d]: ErrOver=%ld\r\n",port_num,tc2105mj_erroversum);          
    }

}
#endif

void
tc2105mjSwPatch(void)
{
    uint8 pn;

    for(pn=0;pn<TC2105MJ_PORTNUM;pn++){

        tcPhyReadReg(pn,1);
        //
        if( !Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_DOWN;
        }
        else if( !Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_DOWN2UP;
        }
        else if( Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            tcphy_link_state[pn]= ST_LINK_UP2DOWN;
        }
        else { //if( Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_UP;
        }
    
        // read registers
        // tcPhyReadReg(pn,0);
        tcPhyReadReg(pn,5);
        tcPhyReadReg(pn,28);
		tcPhyReadLReg(pn,3,18);

        #ifdef TCPHY_DEBUG
        // display message
        tc2105mjSwDispMessage(pn);
		#endif

#ifdef TCPHY_DEBUG
		if (sw_ErrOverMonitor_flag)
			tc2105mjUpdateErrOverSum(pn);
        if (sw_ErrOverMonitor_flag) 
            tc2105mjSwErrOverMonitor(pn); // call after tc2105mjUpdateErrOverSum()
#endif

    }
    
}

#endif //ifdef tc2105mj

int32 mtPhyMiiRead_TrDbg(uint8 phyaddr, char *type, uint32 data_addr , uint8 ch_num)
{
    const uint16 page_reg=31;
    const uint32 Token_Ring_debug_reg=0x52B5;
    const uint32 Token_Ring_Control_reg=0x10;
    const uint32 Token_Ring_Low_data_reg=0x11;
    const uint32 Token_Ring_High_data_reg=0x12;

    uint16 ch_addr=0;
    uint32 node_addr=0;

    uint32 value=0;
    uint32 value_high=0;
    uint32 value_low=0;

    
    #if 0
	switch(type){
		case "DSPF" : // DSP Filter Debug Node
			ch_addr=0x02;
			node_addr=0x0D;
		    break;
		case "PMA":// PMA Debug Node 
			ch_addr=0x01;
			node_addr=0x0F;
			break;
		case "TR":// Timing Recovery Debug Node 
			ch_addr=0x01;
			node_addr=0x0D;
			break;
		case "PCS":// R1000PCS Debug Node 
			ch_addr=0x02;
			node_addr=0x0F;
			break;
		case "FFE":// FFE Debug Node 
			ch_addr=ch_num;
			node_addr=0x04;
			break;
		case "ECNC":// EC/NC Debug Node 
			ch_addr=ch_num;
			node_addr=0x00;
			break;
		case "DFEDC":// DFETail/DC Debug Node 
			ch_addr=ch_num;
			node_addr=0x05;
			break;
		case "DEC":// R1000DEC Debug Node 
			ch_addr=0x00;
			node_addr=0x07;
			break;
		case "CRC":// R1000CRC Debug Node 
			ch_addr=ch_num;
			node_addr=0x06;
			break;
		case "AN":// Autoneg Debug Node 
			ch_addr=0x00;
			node_addr=0x0F;
			break;
		case "CMI":// CMI Debug Node 
			ch_addr=0x03;
			node_addr=0x0F;
			break;
		case "SUPV"// SUPV PHY  Debug Node 
			ch_addr=0x00;
			node_addr=0x0D;
			break;
    }
	#else
        if (stricmp(type, "DSPF") == 0){// DSP Filter Debug Node
            	ch_addr=0x02;
				node_addr=0x0D;
	       	}
		else if (stricmp(type, "PMA") == 0){ // PMA Debug Node 
				ch_addr=0x01;
				node_addr=0x0F;
			}
		else if (stricmp(type, "TR") == 0){ // Timing Recovery  Debug Node 
				ch_addr=0x01;
				node_addr=0x0D;
			}
		else if (stricmp(type, "PCS") == 0){ // R1000PCS Debug Node 
				ch_addr=0x02;
				node_addr=0x0F;
			}
		else if (stricmp(type, "FFE") == 0){ // FFE Debug Node 
				ch_addr=ch_num;
				node_addr=0x04;
			}
		else if (stricmp(type, "EC") == 0){ // ECC Debug Node 
				ch_addr=ch_num;
				node_addr=0x00;
			}
		else if (stricmp(type, "ECT") == 0){ // EC/Tail Debug Node 
				ch_addr=ch_num;
				node_addr=0x01;
			}
		else if (stricmp(type, "NC") == 0){ // EC/NC Debug Node 
				ch_addr=ch_num;
				node_addr=0x01;
			}
		else if (stricmp(type, "DFEDC") == 0){ // DFETail/DC Debug Node 
				ch_addr=ch_num;
				node_addr=0x05;
			}
		else if (stricmp(type, "DEC") == 0){ // R1000DEC Debug Node 
				ch_addr=0x00; node_addr=0x07;
			}
		else if (stricmp(type, "CRC") == 0){ // R1000CRC Debug Node 
				ch_addr=ch_num;
				node_addr=0x06;
			}
		else if (stricmp(type, "AN") == 0){ // Autoneg Debug Node 
				ch_addr=0x00; node_addr=0x0F;
			}
		else if (stricmp(type, "CMI") == 0){ // CMI Debug Node 
				ch_addr=0x03; node_addr=0x0F;
			}
		else if (stricmp(type, "SUPV") == 0){ // SUPV PHY  Debug Node 
				ch_addr=0x00; node_addr=0x0D;
			}
		else{
      return(0xFFFF);
			printf("Wrong TR register Type !");
      
			}
	#endif	

		data_addr=data_addr&0x3F;
      
		//tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
		mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
		//tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
		mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
		//while(!(tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
		//while(!(mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
		{
			//value_low = tcMiiStationRead_ext(phyaddr, Token_Ring_Low_data_reg);
			value_low = mtMiiRegRead(phyaddr, Token_Ring_Low_data_reg);
			//value_high = tcMiiStationRead_ext(phyaddr, Token_Ring_High_data_reg);
			value_high = mtMiiRegRead(phyaddr, Token_Ring_High_data_reg);
			value=value_low+((value_high&0x00FF)<<16);
			TCPHYDISP4("*%s => Phyaddr=%d, ch_addr=%d, node_addr=0x%02lX, data_addr=0x%02lX , value=0x%08lX\r\n", type,phyaddr, ch_addr, node_addr, data_addr, value);
			//TCPHYDISP3("* miir_trdgb => Phyaddr=%d, ch_addr=%d, node_addr=0x%02lX, data_addr=0x%04lX , value=0x%08lX\r\n", phyaddr, ch_addr, node_addr, data_addr, value);
		}
		//tcMiiStationWrite_ext(phyaddr, page_reg, 0x00);//V1.11
		mtMiiRegWrite(phyaddr, page_reg, 0x00);//V1.11
		
		return(value);

}

int mtPhyMiiWrite_TrDbg(uint8 phyaddr, char *type, uint32 data_addr ,uint32 value, uint8 ch_num)
{
    const uint16 page_reg=31;
    const uint32 Token_Ring_debug_reg=0x52B5;
    const uint32 Token_Ring_Control_reg=0x10;
    const uint32 Token_Ring_Low_data_reg=0x11;
    const uint32 Token_Ring_High_data_reg=0x12;
	
    uint16 ch_addr=0;
    uint32 node_addr=0;
    uint32 value_high=0;
    uint32 value_low=0;
	#if 0
	
	switch(*type){
		case "DSPF" : // DSP Filter Debug Node
			ch_addr=0x02;
			node_addr=0x0D;
		    break;
		case "PMA":// PMA Debug Node 
			ch_addr=0x01;
			node_addr=0x0F;
			break;
		case "TR":// Timing Recovery Debug Node 
			ch_addr=0x01;
			node_addr=0x0D;
			break;
		case "PCS":// R1000PCS Debug Node 
			ch_addr=0x02;
			node_addr=0x0F;
			break;
		case "FFE":// FFE Debug Node 
			ch_addr=ch_num;
			node_addr=0x04;
			break;
		case "ECNC":// EC/NC Debug Node 
			ch_addr=ch_num;
			node_addr=0x00;
			break;
		case "DFEDC":// DFETail/DC Debug Node 
			ch_addr=ch_num;
			node_addr=0x05;
			break;
		case "DEC":// R1000DEC Debug Node 
			ch_addr=0x00;
			node_addr=0x07;
			break;
		case "CRC":// R1000CRC Debug Node 
			ch_addr=ch_num;
			node_addr=0x06;
			break;
		case "AN":// Autoneg Debug Node 
			ch_addr=0x00;
			node_addr=0x0F;
			break;
		case "CMI":// CMI Debug Node 
			ch_addr=0x03;
			node_addr=0x0F;
			break;
		case "SUPV"// SUPV PHY  Debug Node 
			ch_addr=0x00;
			node_addr=0x0D;
			break;
    }
	#else
		if (stricmp(type, "DSPF") == 0){// DSP Filter Debug Node
				ch_addr=0x02;
				node_addr=0x0D;
			}
		else if (stricmp(type, "PMA") == 0){ // PMA Debug Node 
				ch_addr=0x01;
				node_addr=0x0F;
			}
		else if (stricmp(type, "TR") == 0){ // Timing Recovery	Debug Node 
				ch_addr=0x01;
				node_addr=0x0D;
			}
		else if (stricmp(type, "PCS") == 0){ // R1000PCS Debug Node 
				ch_addr=0x02;
				node_addr=0x0F;
			}
		else if (stricmp(type, "FFE") == 0){ // FFE Debug Node 
				ch_addr=ch_num;
				node_addr=0x04;
			}
		else if (stricmp(type, "EC") == 0){ // ECC Debug Node 
				ch_addr=ch_num;
				node_addr=0x00;
			}
		else if (stricmp(type, "ECT") == 0){ // EC/Tail Debug Node 
				ch_addr=ch_num;
				node_addr=0x01;
			}
		else if (stricmp(type, "NC") == 0){ // EC/NC Debug Node 
				ch_addr=ch_num;
				node_addr=0x01;
			}
		else if (stricmp(type, "DFEDC") == 0){ // DFETail/DC Debug Node 
				ch_addr=ch_num;
				node_addr=0x05;
			}
		else if (stricmp(type, "DEC") == 0){ // R1000DEC Debug Node 
				ch_addr=0x00; node_addr=0x07;
			}
		else if (stricmp(type, "CRC") == 0){ // R1000CRC Debug Node 
				ch_addr=ch_num;
				node_addr=0x06;
			}
		else if (stricmp(type, "AN") == 0){ // Autoneg Debug Node 
				ch_addr=0x00; node_addr=0x0F;
			}
		else if (stricmp(type, "CMI") == 0){ // CMI Debug Node 
				ch_addr=0x03; node_addr=0x0F;
			}
		else if (stricmp(type, "SUPV") == 0){ // SUPV PHY  Debug Node 
				ch_addr=0x00; node_addr=0x0D;
			}
	#endif
	   
		data_addr=data_addr&0x3F;
		value_high=(0x00FF0000&value)>>16;
		value_low=(0x0000FFFF&value);
   
		//tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
		mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
		   
		//tcMiiStationWrite_ext(phyaddr, Token_Ring_Low_data_reg, value_low);
		mtMiiRegWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
		//tcMiiStationWrite_ext(phyaddr, Token_Ring_High_data_reg, value_high);
		mtMiiRegWrite(phyaddr, Token_Ring_High_data_reg, value_high);
		//tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
		mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
		//while(!(tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
		//while(!(mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
	   {
		   TCPHYDISP4("*%s => Phyaddr=%d, ch_addr=%d, node_addr=0x%02lX, data_addr=0x%02lX , value=0x%08lX\r\n", type,phyaddr, ch_addr, node_addr, data_addr, value);
	   }
		//tcMiiStationWrite_ext(phyaddr, page_reg, 0x00);//V1.11
		mtMiiRegWrite(phyaddr, page_reg, 0x00);//V1.11
	   return 0;
}


#ifdef MT7530_SUPPORT
#define MT7530_PORTNUM 5
#define MT7530_PHY_INIT_LDATA_LEN 0
#define MT7530_PHY_INIT_PERPDATA_LEN 0
#define MT7530_PHY_INIT_SET_NUM 1

#define MT7530_PHY_INIT_CL45_GDATA_LEN 5
#define MT7530_PHY_INIT_CL45_LDATA_LEN 1
#define MT7530_PHY_INIT_CL45_PERPDATA_LEN 0
#define MT7530_PHY_INIT_CL45_SET_NUM 1

#define MT7530_PHY_INIT_TR_LDATA_LEN 0
#define MT7530_PHY_INIT_TR_PERPDATA_LEN 0
#define MT7530_PHY_INIT_TR_SET_NUM 1

typedef struct mt7530_cfg_data_s{
	char name[10];
	cfg_data_t ldata[MT7530_PHY_INIT_LDATA_LEN];
	cfg_data_t perpdata[MT7530_PHY_INIT_PERPDATA_LEN]; //per port register setting
	
}mt7530_cfg_data_t;

typedef struct mt7530_cfg_cl45data_s{
	cfg_cl45data_t gdata[MT7530_PHY_INIT_CL45_GDATA_LEN];
	cfg_cl45data_t ldata[MT7530_PHY_INIT_CL45_LDATA_LEN];
	cfg_cl45data_t perpdata[MT7530_PHY_INIT_CL45_PERPDATA_LEN]; //per port register setting
	
}mt7530_cfg_cl45data_t;

typedef struct mt7530_cfg_trdata_s{
	cfg_trdata_t ldata[MT7530_PHY_INIT_TR_LDATA_LEN];
	cfg_trdata_t perpdata[MT7530_PHY_INIT_PERPDATA_LEN]; //per port register setting
	
}mt7530_cfg_trdata_t;
const uint32 mt7530_page_sel_addr=31;
// G0R22b11: tx10_eee_mode

mt7530_cfg_data_t mt7530_cfg[MT7530_PHY_INIT_SET_NUM]={

    {{"E2.08"},
    //local data	
     { //{0x00,0x4000}
       	//for reference {0x1f,0x2A30},{0x0d,0x1002}, //TEST 0D//force txfil value at test page
     },
    //per-port data 
     { //{0x00,0x4000}
	 }
    }, 
};

mt7530_cfg_cl45data_t mt7530_cfg_cl45[MT7530_PHY_INIT_CL45_SET_NUM]={
    {//globle data
     {
		//for reference {0x1f,0x0032,0x0C24},// disable lpi_1000_pwr_seq_en
		{0x1f, 0x0103, 0x0020}, // invert clock of DFF for phase-mux data input //CML_20130223
		{0x1f, 0x0104, 0x0608}, // disable PLL //CML_20130321
		{0x1f, 0x0104, 0x2608}, // enable PLL //CML_20130321
		{0x1f, 0x0416, 0x0047}, // mcc_acc_ref_th setting //CML_20130327
		{0x1f, 0x027c, 0x0c0c} // for 10BASE-T amplitude //CML_20130327
     },
     //local data
     {
        //for reference {0x1e,0x003D,0x0800},//modify Pair A DC for 100M
        //{0x1e,0x00a6,0x0000} // disable TX power control //CML_20130321 //CML_20130327

		#if 0 // keep E1 setting //CML_20130401
        {0x1e,0x0001,0x0158}, // for 100BASE-T TX //CML_20130327
        {0x1e,0x0003,0x0043}, // for 100BASE-T TX //CML_20130327
        {0x1e,0x0004,0x0207}, // for 100BASE-T TX //CML_20130327
        {0x1e,0x0007,0x0357}, // for 100BASE-T TX //CML_20130327
        {0x1e,0x0008,0x02f0}, // for 100BASE-T TX //CML_20130327
        {0x1e,0x000a,0x0006} // for 100BASE-T TX //CML_20130327
		#endif //CML_20130401
		{0x1e,0x0012,0x7210} // for 100BASE-T TX //CML_20130401
     },
     //per-port data
     {
	 }
    }, 
};

mt7530_cfg_trdata_t mt7530_cfg_tr[MT7530_PHY_INIT_TR_SET_NUM]={
    {//local data
     { 
		//for reference {"PMA",0x20,0x0700},
     },
     //per-port data
     { //{"PMA",0x20,0x0700}
	 }
    }, 
};

//uint32 mt7530_dev1Ehreg12h_A2[MTPHY_PORTNUM] = { // for 100BASE-T TX//CML_20130327 //CML_20130401, let all port have the same setting
//	0x7210, 0x7210, 0x7210, 0x7230, 0x7210};
	
#endif //MT7530_SUPPORT

static int32 MT7530GePhy_erroversum_A=0;
static int32 MT7530GePhy_erroversum_B=0;
static int32 MT7530GePhy_erroversum_C=0;
static int32 MT7530GePhy_erroversum_D=0;

static void mt7530GePhyConfig(int argc, char *argv[], void *p);
void mt7530GePhyCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num);
static void mt7530GePhyCfgTx10AmpSave(void);
void mt7530GePhyGRCfgCheck(void);
void mt7530GePhyLRCfgCheck(uint8 port_num);
void mt7530GePhyCfgCheck(void);

#ifdef MTPHY_DEBUG
int32 mt7530GePhyReadProbe(uint8 port_num, uint8 mode, uint8 ch_num);
int32 mt7530GePhyReadCoeff(uint8 port_num, uint8 mode, uint8 ch_num);
void mtDbgPorbeSet(uint8 prb_num, uint8  port_num, uint16 flg_num, uint16 clk_num );
int32 mtDbgPorbeRead(uint8 prb_num,uint8  port_num);
#endif

#ifdef MTPHY_DEBUG //CML_20130312
int32 mt7530GePhyReadErrOverSum(uint8 port_num);
int32 mt7530GePhyReadAdcSum(uint8 port_num);
void mt7530GePhyDispProbe100(uint8 port_num, bool long_msg);
void mt7530GePhySwDispMessage(uint8 port_num);
uint16 mt7530GePhyReadSnrSum(uint8 port_num, uint16 cnt);
void mt7530GePhyUpdateErrOverSum(uint8 port_num);
void mt7530GePhySwErrOverMonitor(uint8 port_num);
#endif //TCPHY_DEBUG
void mt7530GePhySwPatch(void);
#ifdef MTPHY_DEBUG





void mtDbgPorbeSet(uint8 prb_num, uint8  port_num, uint16 flg_num, uint16 clk_num )
{
	uint16 value=0;

	#define dbg_flg_en 1 
	#define dbg_DevAddr 0x1F
	#define dbg_Flg0Ctrl 0x15
	#define dbg_Flg1Ctrl 0x16
	#define dbg_ClkAddr 0x19
 
		if(prb_num == 2){
		   value = (port_num<<13)|(dbg_flg_en<<12)|(flg_num);
		   TCPHYDISP1("Dbg Flag Register Set Value=%lx\r\n", value);
		   //tcMiiExtStationWrite_ext(port_num,dbg_DevAddr, dbg_Flg0Ctrl, value);
		   mtEMiiRegWrite(port_num,dbg_DevAddr, dbg_Flg0Ctrl, value);
		   //tcMiiExtStationWrite_ext(port_num,dbg_DevAddr, dbg_Flg1Ctrl, value);
		   mtEMiiRegWrite(port_num,dbg_DevAddr, dbg_Flg1Ctrl, value);
		}
		else if(prb_num == 0){
			value = (port_num<<13)|(dbg_flg_en<<12)|(flg_num);
			TCPHYDISP1("Dbg Flag Register Set Value=%lx\r\n", value);
			//tcMiiExtStationWrite_ext(port_num, dbg_DevAddr, dbg_Flg0Ctrl, value);
			mtEMiiRegWrite(port_num, dbg_DevAddr, dbg_Flg0Ctrl, value);
		}
		else if(prb_num == 1){
			value = (port_num<<13)|(dbg_flg_en<<12)|(flg_num);
			TCPHYDISP1("Dbg Flag Register Set Value=%lx\r\n", value);
			//tcMiiExtStationWrite_ext(port_num, dbg_DevAddr, dbg_Flg1Ctrl, value);
			mtEMiiRegWrite(port_num, dbg_DevAddr, dbg_Flg1Ctrl, value);
		}

        if(clk_num == 0xff){//disable clk output
			//value = tcMiiExtStationRead_ext( port_num, dbg_DevAddr, dbg_ClkAddr, NORMAL_READ);
			value = mtEMiiRegRead( port_num, dbg_DevAddr, dbg_ClkAddr);
			value = value & 0xEF;
			//tcMiiExtStationWrite_ext(port_num, dbg_DevAddr, dbg_ClkAddr, value);
			mtEMiiRegWrite(port_num, dbg_DevAddr, dbg_ClkAddr, value);
        }
		else{//enable clk output
			value = (dbg_flg_en<<12)|(clk_num);
			//tcMiiExtStationWrite_ext(port_num, dbg_DevAddr, dbg_ClkAddr, value);
			mtEMiiRegWrite(port_num, dbg_DevAddr, dbg_ClkAddr, value);
		}
		
}

int32 mtDbgPorbeRead(uint8 prb_num,uint8  port_num)
{
	uint16 value=0;
	#define dbg_flg_en 1 
	#define dbg_DevAddr 0x1F
	#define dbg_Flg0Ctrl 0x15
	#define dbg_Flg1Ctrl 0x16
	#define dbg_ClkAddr 0x19
	#define dbg_monAddr 0x1A

 #if 0
		if(prb_num == 2){
			value = (prb_num<<13)|(dbg_flg_en<<12)|(flg_num);
			//printf("Dbg Flag Register Set Value=%lx\r\n", value);
			//tcMiiExtStationWrite_ext(dbg_DevAddr, prb_num, dbg_Flg0Ctrl, value);
			mtEMiiRegWrite(dbg_DevAddr, prb_num, dbg_Flg0Ctrl, value);
			//tcMiiExtStationWrite_ext(dbg_DevAddr, prb_num, dbg_Flg1Ctrl, value);
			mtEMiiRegWrite(dbg_DevAddr, prb_num, dbg_Flg1Ctrl, value);
		}
		else if(prb_num == 0){
			value = (prb_num<<13)|(dbg_flg_en<<12)|(flg_num);
			//printf("Dbg Flag Register Set Value=%lx\r\n", value);
			//tcMiiExtStationWrite_ext(dbg_DevAddr, prb_num, dbg_Flg0Ctrl, value);
			mtEMiiRegWrite(dbg_DevAddr, prb_num, dbg_Flg0Ctrl, value);
		}
		else if(prb_num == 1){
			value = (prb_num<<13)|(dbg_flg_en<<12)|(flg_num);
			//printf("Dbg Flag Register Set Value=%lx\r\n", value);
			//tcMiiExtStationWrite_ext(dbg_DevAddr, prb_num, dbg_Flg1Ctrl, value);
			mtEMiiRegWrite(dbg_DevAddr, prb_num, dbg_Flg1Ctrl, value);
		}

        if(clk_num == 0xff){//disable clk output
			//value = tcMiiExtStationRead_ext(dbg_DevAddr, prb_num, dbg_ClkAddr, NORMAL_READ);
			value = mtEMiiRegRead(dbg_DevAddr, prb_num, dbg_ClkAddr);
			value = value & 0xEF;
			//tcMiiExtStationWrite_ext(dbg_DevAddr, prb_num, dbg_ClkAddr, value);
			mtEMiiRegWrite(dbg_DevAddr, prb_num, dbg_ClkAddr, value);
		}else{//enable clk output
			value = (dbg_flg_en<<12)|(clk_num);
			//tcMiiExtStationWrite_ext(dbg_DevAddr, prb_num, dbg_ClkAddr, value);
			mtEMiiRegWrite(dbg_DevAddr, prb_num, dbg_ClkAddr, value);
		}
#endif
		//value = tcMiiExtStationRead_ext( port_num, dbg_DevAddr,dbg_monAddr, NORMAL_READ);
		value = mtEMiiRegRead( port_num, dbg_DevAddr,dbg_monAddr);

		if(prb_num == 0)
			value = (value & 0x00FF);
		else if(prb_num == 1)
			value = (value>>8) & 0x00FF;
             	
		TCPHYDISP1("Dbg Flag[%d] Monitor Register Value=%lx\r\n", prb_num,value);
		return(value);
	       	
}

int32
mt7530GePhyReadProbe(uint8 port_num, uint8 mode, uint8 ch_num)
{
    uint32 val=0,val_A=0,val_B=0,val_C=0,val_D=0;
    uint32 avg_val=0, avg_A=0,avg_B=0,avg_C=0,avg_D=0;
    uint32 phyaddr = port_num;
    uint16 i;
    #define avg_cnt 1000
    
    //tcMiiStationWrite_ext(port_num,31,0);
    mtMiiRegWrite(port_num,31,0);
    switch(mode){
    		case Probe_VgaState:
    			val=mtPhyMiiRead_TrDbg(phyaddr,"PMA",0x38,0);
    			TCPHYDISP3("VgaStateA =0x%x\r\n",((val>>4) & 0x1F));
    			TCPHYDISP3("VgaStateB =0x%x\r\n",((val>>9) & 0x1F));
    			TCPHYDISP3("VgaStateC =0x%x\r\n",((val>>14) & 0x1F));
    			TCPHYDISP3("VgaStateD =0x%x\r\n",((val>>19) & 0x1F));
    			break;
        case Probe_MSE:
          switch(ch_num){
            case 0://Pair A
              //val=tcMiiExtStationRead_ext(phyaddr,0x1E,0x9B,NORMAL_READ);
              val=mtEMiiRegRead(phyaddr,0x1E,0x9B);
			  TCPHYDISP1("XX0 0x1E,0x9B =0x%x\r\n",val);
              val =(val>>8) & 0xFF;  //V1.16
              TCPHYDISP1("AA0 lch_mse_mdcA =0x%x\r\n",val);
              break;
            case 1://Pair B
              //val=tcMiiExtStationRead_ext(phyaddr,0x1E,0x9B,NORMAL_READ);
              val=mtEMiiRegRead(phyaddr,0x1E,0x9B);
			  TCPHYDISP1("XX1 0x1E,0x9B =0x%x\r\n",val);
              val =(val) & 0xFF;  //V1.16
              TCPHYDISP1("AA1 lch_mse_mdcB =0x%x\r\n",val);
              break;
            case 2://Pair C
              //val=tcMiiExtStationRead_ext(phyaddr,0x1E,0x9C,NORMAL_READ);
              val=mtEMiiRegRead(phyaddr,0x1E,0x9C);
			  TCPHYDISP1("XX2 0x1E,0x9C =0x%x\r\n",val);
              val =(val>>8) & 0xFF;  //V1.16
              TCPHYDISP1("AA2 lch_mse_mdcC =0x%x\r\n",val);
              break;
            case 3://Pair D
              //val=tcMiiExtStationRead_ext(phyaddr,0x1E,0x9C,NORMAL_READ);
              val=mtEMiiRegRead(phyaddr,0x1E,0x9C);
			  TCPHYDISP1("XX3 0x1E,0x9C =0x%x\r\n",val);
              val =(val) & 0xFF;  //V1.16
              TCPHYDISP1("AA3 lch_mse_mdcD =0x%x\r\n",val);
              break;
            }
		    break;
        case Probe_MSE_ALL:
			//val=tcMiiExtStationRead_ext(phyaddr,0x1E,0x9B,NORMAL_READ);
			val=mtEMiiRegRead(phyaddr,0x1E,0x9B);
			val_A =(val>>8) & 0xFF;  //V1.16
			val_B =(val) & 0xFF;  //V1.16
			//val=tcMiiExtStationRead_ext(phyaddr,0x1E,0x9C,NORMAL_READ);
			TCPHYDISP1("ALL 0x1E,0x9B =0x%x\r\n",val);		

			val=mtEMiiRegRead(phyaddr,0x1E,0x9C);

			val_C =(val>>8) & 0xFF;  //V1.16
			val_D =(val) & 0xFF;  //V1.16
			TCPHYDISP1("ALL 0x1E,0x9C =0x%x\r\n",val);		
			TCPHYDISP1("BB1 lch_mse_mdcA =0x%x\r\n",val_A);
			TCPHYDISP1("BB2 lch_mse_mdcB =0x%x\r\n",val_B);
			TCPHYDISP1("BB3 lch_mse_mdcC =0x%x\r\n",val_C);
			TCPHYDISP1("BB4 lch_mse_mdcD =0x%x\r\n",val_D);
			val=val_A+val_B+val_C+val_D;
            break;
        case Probe_MSE_Slicer_err_thres:
			//val=tcMiiExtStationRead_ext(phyaddr,0x1E,0x9D,NORMAL_READ);
			val=mtEMiiRegRead(phyaddr,0x1E,0x9D);
			val=(val>>11) & 0x1F; 
			TCPHYDISP3("slicer_err_thres =0x%x\r\n",val);
			break;
        case Probe_MSE_Sum:
			for(i=0;i<avg_cnt;i++){
				//val=tcMiiExtStationRead_ext(phyaddr,0x1E,0x9B,NORMAL_READ);
				val=mtEMiiRegRead(phyaddr,0x1E,0x9B);
				val_A =(val>>8) & 0xFF;  //V1.16
				val_B =(val) & 0xFF;  //V1.16
				avg_A = avg_A + val_A;
				avg_B = avg_B + val_B;
            
				//val=tcMiiExtStationRead_ext(phyaddr,0x1E,0x9C,NORMAL_READ);
				val=mtEMiiRegRead(phyaddr,0x1E,0x9C);
				val_C =(val>>8) & 0xFF;  //V1.16
				val_D =(val) & 0xFF;  //V1.16
				avg_C = avg_C + val_C;
				avg_D = avg_D + val_D;
             
				//val=(tcMiiExtStationRead_ext(phyaddr,0x1E,0x9D,NORMAL_READ)>>11)&0x1F;
				val=(mtEMiiRegRead(phyaddr,0x1E,0x9D)>>11)&0x1F;

				avg_val = avg_val + val;
			}


          avg_A = avg_A/1000;
          avg_B = avg_B/1000;
          avg_C = avg_C/1000;
          avg_D = avg_D/1000;
          avg_val = avg_val/1000;
          
          val=avg_A+avg_B+avg_C+avg_D;
          
          TCPHYDISP3("lch_mse_mdcA avg1000 =0x%x\r\n",avg_A);
          TCPHYDISP3("lch_mse_mdcB avg1000 =0x%x\r\n",avg_B);
          TCPHYDISP3("lch_mse_mdcC avg1000 =0x%x\r\n",avg_C);
          TCPHYDISP3("lch_mse_mdcD avg1000 =0x%x\r\n",avg_D);
          TCPHYDISP3("slicer_err_thres avg1000 =0x%x\r\n",avg_val);
          break;
        case Probe_Slicer_Err_Over_Sum:
          
          switch(ch_num){
            case 0:
              //val=tcMiiExtStationRead_ext(phyaddr,0x1E,0x9E,NORMAL_READ);
              val=mtEMiiRegRead(phyaddr,0x1E,0x9E);
              ErovcntA[port_num].err_over_cnt=val;
              TCPHYDISP3("ProbeSlicerErrOverSumA =0x%x\r\n",val);
              break;
            case 1:
              //val=tcMiiExtStationRead_ext(phyaddr,0x1E,0x9F,NORMAL_READ);
              val=mtEMiiRegRead(phyaddr,0x1E,0x9F);
              ErovcntB[port_num].err_over_cnt=val;
              TCPHYDISP3("ProbeSlicerErrOverSumB =0x%x\r\n",val);
              break;
            case 2:
              //val=tcMiiExtStationRead_ext(phyaddr,0x1E,0xA0,NORMAL_READ);
              val=mtEMiiRegRead(phyaddr,0x1E,0xA0);
              ErovcntC[port_num].err_over_cnt=val;
              TCPHYDISP3("ProbeSlicerErrOverSumC =0x%x\r\n",val);
              break;
            case 3:
              //val=tcMiiExtStationRead_ext(phyaddr,0x1E,0xA1,NORMAL_READ);
              val=mtEMiiRegRead(phyaddr,0x1E,0xA1);
              ErovcntD[port_num].err_over_cnt=val;
              TCPHYDISP3("ProbeSlicerErrOverSumD =0x%x\r\n",val);
              break;
            }
          break;
        case Probe_Slicer_Err_Over_Sum_ALL:
              //val_A=tcMiiExtStationRead_ext(phyaddr,0x1E,0x9E,NORMAL_READ);
              val_A=mtEMiiRegRead(phyaddr,0x1E,0x9E);
              TCPHYDISP3("ProbeSlicerErrOverSumA =0x%x\r\n",val_A);
              //val_B=tcMiiExtStationRead_ext(phyaddr,0x1E,0x9F,NORMAL_READ);
              val_B=mtEMiiRegRead(phyaddr,0x1E,0x9F);
              TCPHYDISP3("ProbeSlicerErrOverSumB =0x%x\r\n",val_B);
              //val_C=tcMiiExtStationRead_ext(phyaddr,0x1E,0xA0,NORMAL_READ);
              val_C=mtEMiiRegRead(phyaddr,0x1E,0xA0);
              TCPHYDISP3("ProbeSlicerErrOverSumC =0x%x\r\n",val_C);
              //val_D=tcMiiExtStationRead_ext(phyaddr,0x1E,0xA1,NORMAL_READ);
              val_D=mtEMiiRegRead(phyaddr,0x1E,0xA1);
              TCPHYDISP3("ProbeSlicerErrOverSumD =0x%x\r\n",val_D);
              val=val_A+val_B+val_C+val_D;
              break;
  
        default:
            TCPHYDISP1("\r\ntcphy error: ReadProbe %d.\r\n",mode);
            break;
    }

    return val;

}
void
mt7530GePhySwDispMessage(uint8 port_num)
{
#ifdef PERIOD_PRINT //V1.17
    uint32 value;
#endif
    switch (mtphy_link_state[port_num]) {
        case ST_LINK_DOWN: 
      //    	printf("Link Satus is ST_LINK_DOWN!\r\n");
            break;
            
        case ST_LINK_DOWN2UP: 
      //  printf("Link Satus is ST_LINK_DOWN2UP!\r\n");
		if(mr1E_A2.final_speed_1000){
			TCPHYDISP1("mtphy[%d]:Link-up at 1000 %s.\r\n",port_num, (mr1E_A2.FullDuplexEnable ?"F":"H"));}
		else if (mr1E_A2.final_speed_100){
			TCPHYDISP1("mtphy[%d]:Link-up at 100%s.\r\n",port_num, (mr1E_A2.FullDuplexEnable ?"F":"H"));}
		else if (mr1E_A2.final_speed_10){
			TCPHYDISP1("mtphy[%d]:Link-up at 10%s.\r\n",port_num, (mr1E_A2.FullDuplexEnable ?"F":"H"));}
	
            //if(mr28.final_speed) {
            ///    tc2105mjDispProbe100(port_num,0); // short message
           // }
		if(mr1E_A2.final_speed_1000 ||mr1E_A2.final_speed_100 ||mr1E_A2.final_speed_10){
			mt7530GePhyReadProbe(port_num,Probe_VgaState,0);
			mt7530GePhyReadProbe(port_num,Probe_MSE_ALL,0);
			mt7530GePhyReadProbe(port_num,Probe_MSE_Slicer_err_thres,0);
			mt7530GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum_ALL,0);
			
			}
			
            break;
            
        case ST_LINK_UP: 
         // printf("Link Satus is ST_LINK_UP!\r\n");
          #ifdef PERIOD_PRINT //V1.17
            if (period_print_flag==1)
            {
              if (period_print_cnt<period_print_threshold)
              {
                period_print_cnt++;
              }
			  else
			  {
                period_print_cnt=0;
				value = mtPhyMiiRead_TrDbg(port_num, "PMA", 0x38 ,0);
				printf("port_num=0x%X,Value of PMA 0x38=0x%X\r\n",port_num,value);
				printf("VgaStateA=0x%X,VgaStateB=0x%X,VgaStateC=0x%X,VgaStateD=0x%X\r\n",(value&0x1F0)>>4,(value&0x3E00)>>9,(value&0x7C000)>>14,(value&0xF80000)>>19);
			  }
            }
          #endif
            break;
            
        case ST_LINK_UP2DOWN:
          #ifdef PERIOD_PRINT //V1.17			
			period_print_cnt=0;
          #endif
            TCPHYDISP1("mtcphy[%d]: Link-down!!!\r\n",port_num);
            break;
            
        default: printf("\r\nmtphy error: SwDispMessage error!\r\n");
    }
}

void
mt7530GePhyUpdateErrOverSum(uint8 port_num)
{
    uint16 err_over_cnt_prev_A; // to store previous err_over_cnt value
    uint16 err_over_cnt_prev_B; // to store previous err_over_cnt value
    uint16 err_over_cnt_prev_C; // to store previous err_over_cnt value
    uint16 err_over_cnt_prev_D; // to store previous err_over_cnt value
    int32 val_A,val_B,val_C,val_D;
    
    // clear Nmr25[port_num].err_over_cnt in tc2104meDispProbe100() in ST_LINK_DOWN2UP
    if (((mr1E_A2.final_speed_1000== 1) ||(mr1E_A2.final_speed_100 == 1)) // 1000/100BaseTX
        && (mtphy_link_state[port_num] == ST_LINK_UP)) 
    {
        err_over_cnt_prev_A = ErovcntA[port_num].err_over_cnt;
        err_over_cnt_prev_B = ErovcntB[port_num].err_over_cnt;
        err_over_cnt_prev_C = ErovcntC[port_num].err_over_cnt;
        err_over_cnt_prev_D = ErovcntD[port_num].err_over_cnt;
        
       // tcPhyReadReg(port_num,25);
      // mt7530GePhyReadProbe(port_num,Probe_MSE_Sum,0);
       mt7530GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum,0);//Pair A
       mt7530GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum,1);//Pair B
       mt7530GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum,2);//Pair C
       mt7530GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum,3);//Pair D
        val_A = ErovcntA[port_num].err_over_cnt - err_over_cnt_prev_A;
        val_B = ErovcntB[port_num].err_over_cnt - err_over_cnt_prev_B;
        val_C = ErovcntC[port_num].err_over_cnt - err_over_cnt_prev_C;
        val_D = ErovcntD[port_num].err_over_cnt - err_over_cnt_prev_D;
        
        TCPHYDISP4("ProbeSlicerErrOverSumA =0x%x\r\n",val_A);
        
        TCPHYDISP4("ProbeSlicerErrOverSumB =0x%x\r\n",val_B);
        
        TCPHYDISP4("ProbeSlicerErrOverSumC =0x%x\r\n",val_C);
        
        TCPHYDISP4("ProbeSlicerErrOverSumD =0x%x\r\n",val_D);
        
        if( val_A < 0 ){
            val_A += 2048;
        }
        if( val_B < 0 ){
            val_B += 2048;
        }
        if( val_C < 0 ){
            val_C += 2048;
        }
        if( val_D < 0 ){
            val_D += 2048;
        }
        
        MT7530GePhy_erroversum_A = val_A;
        MT7530GePhy_erroversum_B = val_B;
        MT7530GePhy_erroversum_C = val_C;
        MT7530GePhy_erroversum_D = val_D;
    }
    else {
        MT7530GePhy_erroversum_A = -1;
        MT7530GePhy_erroversum_B = -1;
        MT7530GePhy_erroversum_C = -1;
        MT7530GePhy_erroversum_D = -1;
    }
}
void
mt7530GePhySwErrOverMonitor(uint8 port_num)
{
    if (tcPhy_disp_level<3) 
        return; // inactive

    if (MT7530GePhy_erroversum_A>0){
        TCPHYDISP3("tcphy[%d]: ErrOver A=%ld\r\n",port_num,MT7530GePhy_erroversum_A);          
    }
    if (MT7530GePhy_erroversum_B>0){
        TCPHYDISP3("tcphy[%d]: ErrOver B=%ld\r\n",port_num,MT7530GePhy_erroversum_B);          
    }
    if (MT7530GePhy_erroversum_C>0){
        TCPHYDISP3("tcphy[%d]: ErrOver C =%ld\r\n",port_num,MT7530GePhy_erroversum_C);          
    }
    if (MT7530GePhy_erroversum_D>0){
        TCPHYDISP3("tcphy[%d]: ErrOver D=%ld\r\n",port_num,MT7530GePhy_erroversum_D);          
    }

}

void mt7530GePhySwPatch(void)
{
    uint8 pn;
   // printf("HJM1_mt7530GePhySwPatch ");
    if (power_on_finish_flag == 0)
    {
      if ((mtMiiRegRead(0, 0x09)&0xE000) == 0xE000)
      {
        power_on_finish_flag = 1;
        //printf("\nPower On Process Finish\n");
        mtMiiRegWrite(0, 0x09, 0x0700);
      }
    }

    for(pn=0;pn<MT7530_PORTNUM;pn++){
		//tcMiiStationWrite_ext(pn,31,0);
        mtPhyReadReg(pn,1);
        //
        if( !Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            mtphy_link_state[pn] = ST_LINK_DOWN;
        }
        else if( !Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            mtphy_link_state[pn] = ST_LINK_DOWN2UP;
        }
        else if( Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            mtphy_link_state[pn]= ST_LINK_UP2DOWN;
        }
        else { //if( Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            mtphy_link_state[pn] = ST_LINK_UP;
        }
    
        // read registers
        // tcPhyReadReg(pn,0);
        
        mtPhyReadReg(pn,5);
        mtPhyReadGReg(pn,0x1E,0xA2);
	//	tcPhyReadLReg(pn,3,18);//TBD

	#ifdef MTPHY_DEBUG
	// display message
	mt7530GePhySwDispMessage(pn);
	#endif

	#ifdef MTPHY_DEBUG
	if (sw_ErrOverMonitor_flag)
	mt7530GePhyUpdateErrOverSum(pn);
	if (sw_ErrOverMonitor_flag) 
	mt7530GePhySwErrOverMonitor(pn); // call after tc2105mjUpdateErrOverSum()
	#endif

    }
    
}
#endif

void 
mt7530GePhyCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num)
{
	int pn, i;
	uint16 phyAddr;
	current_idx = idx;

    // global registers
    phyAddr = port_num; //mac_p->enetPhyAddr;
	for( pn=0; pn<MTPHY_PORTNUM; pn++){
		phyAddr = pn; //mac_p->enetPhyAddr + pn;

		for( i=0; i<MT7530_PHY_INIT_CL45_GDATA_LEN; i++ ){         
			//tcMiiExtStationWrite_ext(phyAddr, mt7530_cfg_cl45[current_idx].gdata[i].dev_num, mt7530_cfg_cl45[current_idx].gdata[i].reg_num, mt7530_cfg_cl45[current_idx].gdata[i].val);
			mtEMiiRegWrite(phyAddr, mt7530_cfg_cl45[current_idx].gdata[i].dev_num, mt7530_cfg_cl45[current_idx].gdata[i].reg_num, mt7530_cfg_cl45[current_idx].gdata[i].val);
		}
		for( i=0; i<MT7530_PHY_INIT_CL45_LDATA_LEN; i++ ){         
			//tcMiiExtStationWrite_ext(phyAddr, mt7530_cfg_cl45[current_idx].ldata[i].dev_num, mt7530_cfg_cl45[current_idx].ldata[i].reg_num, mt7530_cfg_cl45[current_idx].ldata[i].val);
			mtEMiiRegWrite(phyAddr, mt7530_cfg_cl45[current_idx].ldata[i].dev_num, mt7530_cfg_cl45[current_idx].ldata[i].reg_num, mt7530_cfg_cl45[current_idx].ldata[i].val);
		}
		for( i=0; i<MT7530_PHY_INIT_LDATA_LEN; i++ ){         
			//tcMiiStationWrite_ext( phyAddr, mt7530_cfg[current_idx].ldata[i].reg_num, mt7530_cfg[current_idx].ldata[i].val);
			mtMiiRegWrite( phyAddr, mt7530_cfg[current_idx].ldata[i].reg_num, mt7530_cfg[current_idx].ldata[i].val);
		}
		for( i=0; i<MT7530_PHY_INIT_TR_LDATA_LEN; i++ ){         
			mtPhyMiiWrite_TrDbg( phyAddr, mt7530_cfg_tr[current_idx].ldata[i].reg_typ, mt7530_cfg_tr[current_idx].ldata[i].reg_num, mt7530_cfg_tr[current_idx].ldata[i].val,0);
		}

		//mtEMiiRegWrite(phyAddr, 0x1e, 0x12, mt7530_dev1Ehreg12h_A2[phyAddr]); // for 100BASE-T TX //CML_20130327 //CML_20130401
		
	}  
	#if 0 // remove for E2 //CML_20130223
    ///for E1 IC EEE at V1.09  
	mtMiiRegWrite( 0, 0x1F, 0x2A30);
	mtMiiRegWrite( 0, 0x05, 0x1018);//force MDIX for E1 port 0 Signal detector only A work
	mtMiiRegWrite( 4, 0x1F, 0x2A30);
	mtMiiRegWrite( 4, 0x05, 0x1010);//force MDI for E1  port 0 Signal detector only B work
	#endif

    if (eco_rev == 0x00) { // disable P4 EEE for MT7530E2 to workaround EEE bug //CML_20130226_2
	    mtEMiiRegWrite(4, 0x7, 0x3c, 0x0); // disable 100/1000BASE-T EEE capability //CML_20130327
    	mtMiiRegWrite(4, 0x1f, 0x1); // Ext page
    	mtMiiRegWrite(4, 0x16, 0x0); // Ext page Reg 16
    }

    TCPHYDISP4("mtphy: CfgLoad %s\r\n",  mt7530_cfg[current_idx].name);

}


/************************************************************************
*
*            Variables and Functions for tc2105skSwPatch()
*
*************************************************************************
*/
//#define TC2105SK_CM_SUPPORT
//#define UNH_TEST 1

#ifdef TC2105SK_SUPPORT
#define TC2105SK_PORTNUM 5

#define TC2105SK_PHY_INIT_GDATA_LEN 21
#define TC2105SK_PHY_INIT_LDATA_LEN 1
#define TC2105SK_PHY_INIT_PERPDATA_LEN 1
#define TC2105SK_PHY_INIT_SET_NUM 1

typedef struct tc2105sk_cfg_data_s{
    char name[10];
    cfg_data_t gdata[TC2105SK_PHY_INIT_GDATA_LEN];
    cfg_data_t ldata[TC2105SK_PHY_INIT_LDATA_LEN];
	cfg_data_t perpdata[TC2105SK_PHY_INIT_PERPDATA_LEN]; //per port register setting
}tc2105sk_cfg_data_t;

const uint32 tc2105sk_page_sel_addr=31;

// G0R22b11: tx10_eee_mode
const uint16 tc2105sk_G0R22_Tx10AmpSave_ON  = 0x0264;
const uint16 tc2105sk_G0R22_Tx10AmpSave_OFF = 0x006F;
const uint32 tc2105sk_L3R17_EEEAdv = 0x0002;
const uint16 tc2105sk_L2R16[TC2105SK_PORTNUM] = {0x1515, 0x1414, 0x1515, 0x1414, 0x1414};

tc2105sk_cfg_data_t tc2105sk_cfg[TC2105SK_PHY_INIT_SET_NUM]={
    {{"K4.0"},
     { {31,0x4000},{17,0x7244},{19,0x1112},{22,0x10cf},{25,0x0102},{26,0x0707},{29,0x0640},{28,0xc077},
       {31,0x3000},{17,0x4838},{24,0x2288},
       {31,0x2000},{21,0x0611},{22,0x01d3},{23,0x003c},{24,0x0972},{25,0x0eee},{26,0x0fc4},
       {31,0x1000},{17,0xe7f8},
       {31,0x0000}
     },
     { {31,0x9000}
     },
     { {31,0x8000}
	 }
    }, 
};


#endif //TC2105SK_SUPPORT

#ifdef TC2105SK_SUPPORT

static int32 tc2105sk_erroversum=0;

static int16 tc2105sk_Up2Down_cnt_hbt[5] = {0,0,0,0,0};
static uint8 tc2105sk_Up2DownFastHbt_detect[5] = {0,0,0,0,0};
static bool tc2105sk_link_fail_detect_flag = 0;
//static uint8 tc2105sg_reset_needed_perport[5] = {0,0,0,0,0};
//static uint8 tc2105sg_reset_needed_flag = 0;
//static uint8 tc2105sg_esdphy_init_flag = 1; // default: enable

//Disable EEE if link fail
static int16 tc2105sk_Up2Down_cnt_tbt[5] = {0,0,0,0,0};
static uint8 tc2105sk_Up2DownFastTbt_detect[5] = {0,0,0,0,0};
static uint8 tc2105sk_linkup_check_timer[5] = {0,0,0,0,0};
static bool tc2105skDisEEEHappened[5] = {0,0,0,0,0};
static uint8 tc2105sk_linkup_check_cnt[5] = {0,0,0,0,0};

static uint8 tc2105skDisNextPage_cnt[5] = {0,0,0,0,0};
static bool tc2105skDisNextPage_flag[5] = {0,0,0,0,0};

static void tc2105skConfig(int argc, char *argv[], void *p);
void tc2105skCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num);
void tc2105skGRCfgCheck(void);
void tc2105skLRCfgCheck(uint8 port_num);
void tc2105skCfgCheck(void);
static void tc2105skLRCfgLoad(uint8 idx, uint8 port_num);
static void tc2105skPerPortCfgLoad(uint8 idx, uint8 port_num);
int32 tc2105skReadProbe(uint8 port_num, uint8 mode);
static void tc2105skCfgTx10AmpSave(void);
//static void tc2105skCfgLDPS(void);
#ifdef TCPHY_DEBUG
int32 tc2105skReadErrOverSum(uint8 port_num);
int32 tc2105skReadAdcSum(uint8 port_num);
void tc2105skDispProbe100(uint8 port_num, bool long_msg);
void tc2105skPoweDown(uint8 port_num);
void tc2105skSwDispMessage(uint8 port_num);
uint16 tc2105skReadSnrSum(uint8 port_num, uint16 cnt);
void tc2105skUpdateErrOverSum(uint8 port_num);
void tc2105skSwErrOverMonitor(uint8 port_num);
//bool tc2105skInitialRegFlag(void);
//void tc2105skEsdSwPatch(void);
#endif
void tc2105skUp2DownFastHbt(uint8 port_num);
void tc2105skUp2DownFastTbt(uint8 port_num);
bool tc2105skLinkFailDetect (uint8 port_num, uint8 mode);//0:10BT or 100BT, 1:100BT only, 2:10BT only
void tc2105skDisEEENeeded(uint8 port_num);
void tc2105skDisNextPageNeeded(uint8 port_num);
void tc2105skSwPatch(void);


static void 
tc2105skConfig(int argc, char *argv[], void *p)
{
    // global config in doPhyConfig()
#ifdef TCPHY_DEBUG
    if (argc==1) {
        TCPHYDISP3("Usage: config Tx10AmpSave [on|off]\r\n");
		//TCPHYDISP3("       config EsdPhyRegInit [on|off]\r\n");
		TCPHYDISP3("       config FixUp2DownFast [on|off]\r\n");
		//TCPHYDISP3("       config LinkDownPS [on|off]\r\n");
		//TCPHYDISP3("       config NextPageAutoDisable [on|off]\r\n");
        return;
    }
    else if (argc==2) {
        if (stricmp(argv[1], "list") == 0){
            TCPHYDISP3("config Tx10AmpSave : %s\r\n",
                    (cfg_Tx10AmpSave_flag?"on":"off"));  
			//TCPHYDISP3("config EsdPhyRegInit : %s\r\n",
            //        (tc2104me_esdphy_init_flag?"on":"off"));  
			TCPHYDISP3("config FixUp2DownFast : %s\r\n",
                    (sw_FixUp2DownFast_flag?"on":"off"));
			//TCPHYDISP3("config LinkDownPS : %s\r\n",
            //        (cfg_LDPS_flag?"on":"off"));
            return;
        }
    }
#endif

    if (argc==2 || argc==3){
        if (stricmp(argv[1], "Tx10AmpSave") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                cfg_Tx10AmpSave_flag = 1; // enable
                tc2105skCfgTx10AmpSave();
            }
            else if( stricmp(argv[2], "off") == 0 ){
                cfg_Tx10AmpSave_flag = 0;
                tc2105skCfgTx10AmpSave();
            }
            TCPHYDISP2("config Tx10AmpSave : %s\r\n",
                        (cfg_Tx10AmpSave_flag?"on":"off"));
        }
		/*if (stricmp(argv[1], "EsdPhyRegInit") == 0) {
			if( stricmp(argv[2], "on") == 0 ){
                tc2104me_esdphy_init_flag= 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                tc2104me_esdphy_init_flag= 0;
            }
            TCPHYDISP2("config EsdPhyRegInit : %s\r\n",
                        (tc2104me_esdphy_init_flag?"on":"off"));
		}*/
		if (stricmp(argv[1], "FixUp2DownFast") == 0) {
			if( stricmp(argv[2], "on") == 0 ){
                sw_FixUp2DownFast_flag= 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                sw_FixUp2DownFast_flag= 0;
            }
            TCPHYDISP2("config FixUp2DownFast : %s\r\n",
                        (sw_FixUp2DownFast_flag?"on":"off"));
		}
	   /*if (stricmp(argv[1], "LinkDownPS") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                cfg_LDPS_flag = 1; // enable
                tc2104meCfgLDPS();
            }
             else if( stricmp(argv[2], "off") == 0 ){
                cfg_LDPS_flag = 0; // disable
                tc2104meCfgLDPS();
            }
            TCPHYDISP2("config LinkDownPS : %s\r\n",
                        (cfg_LDPS_flag?"on":"off"));
        }*/
    }
}

// 1. loading default register setting in tcPhyInit()
// 2. changing register setting
void 
tc2105skCfgLoad(uint8 idx, uint8 doPerPort_flag, uint8 port_num)
{
    int pn, i;
    uint16 phyAddr;
	current_idx = idx;
	
    // global registers
    phyAddr = 0; //mac_p->enetPhyAddr;

    for( i=0; i<TC2105SG_PHY_INIT_GDATA_LEN; i++ ){         
        tcMiiStationWrite(phyAddr, tc2105sk_cfg[current_idx].gdata[i].reg_num, tc2105sk_cfg[current_idx].gdata[i].val );
    }
    //     
    tc2105skCfgTx10AmpSave();

    // local registers 
    if (doPerPort_flag == DO_PER_PORT) {
		tc2105skLRCfgLoad(current_idx, port_num);
    	}
	else {
		 for( pn=0; pn<TCPHY_PORTNUM; pn++){
		 	phyAddr = pn; //mac_p->enetPhyAddr + pn;
		 	tc2105skLRCfgLoad(current_idx, phyAddr);
			}
		 }
	

    TCPHYDISP4("tcphy: CfgLoad %s\r\n",  tc2105sk_cfg[current_idx].name);

#ifdef UNH_TEST
    phyAddr = 0;
    sw_ErrOverMonitor_flag=0;
	sw_FixUp2DownFast_flag= 0;
	cfg_Tx10AmpSave_flag=0; // disable tx10_eee_mode
	tc2105skCfgTx10AmpSave();
	
	
    //tcPhyWriteGReg(phyAddr,0,22,0x0664); // disable tx10_eee_mode
    //tcPhyWriteGReg(phyAddr,0,26,0x0012); // disable cr_signal_status_opt
    tcPhyWriteGReg(phyAddr,1,22,0x0010); // enable RX_ER extension to Pass PCS pause test
    //tcPhyWriteGReg(phyAddr,3,21,0x7387); // decrease I2MB to pass template
	if (doPerPort_flag == DO_PER_PORT) {
		tcPhyWriteLReg(port_num,0,30,0x0000); // disable AAPS to avoid varaince lc_max 
        tcPhyWriteLReg(port_num,0,26,0x1200); // disable Link_Det_Ext function
		}
	else{
		for( pn=0; pn<TCPHY_PORTNUM; pn++){
			phyAddr = pn; //mac_p->enetPhyAddr + pn;
            tcPhyWriteLReg(phyAddr,0,30,0x0000); // disable AAPS to avoid varaince lc_max 
            tcPhyWriteLReg(phyAddr,0,26,0x1200); // disable Link_Det_Ext function
            }
		}
    TCPHYDISP4("tcphy: CfgLoad UNH-IOL setting\r\n");
#endif
}

void
tc2105skGRCfgCheck(void)
{
    int i;
	uint32 phyaddr = mac_p->enetPhyAddr;
	uint32 rval;
	current_idx = 0; 
	
	for( i=0; i<TC2105SG_PHY_INIT_GDATA_LEN; i++){
		if(tc2105sk_cfg[current_idx].gdata[i].reg_num == tc2105sk_page_sel_addr) {
			tcMiiStationWrite(phyaddr, tc2105sk_cfg[current_idx].gdata[i].reg_num, tc2105sk_cfg[current_idx].gdata[i].val);
		}//if reg_num == page_sel_addr
		else {
			rval = tcMiiStationRead(phyaddr, tc2105sk_cfg[current_idx].gdata[i].reg_num);
			if(rval!=tc2105sk_cfg[current_idx].gdata[i].val){
				printf("Wrong initial setting of global register %d is detected!!\r\n",tc2105sk_cfg[current_idx].gdata[i].reg_num);
				}//rval==gdata
			}
		}//for
}

void 
tc2105skLRCfgCheck(uint8 port_num)
{
	 uint32 phyaddr = mac_p->enetPhyAddr+port_num;
	 uint32 rval;
/*
	 // check L0R25 
     rval = tcPhyReadReg(phyaddr,25);
     if ((rval&0xf000)!=tc2105sk_L0R25_default){
	 	printf("Wrong initial setting of Local register 25 at port%d is detected!!\r\n",port_num);
        }
        */
}

void 
tc2105skCfgCheck(void)
{
     int pn;
	 tc2105skGRCfgCheck();
	 
	 for( pn=0; pn<TCPHY_PORTNUM; pn++){
	 	tc2105skLRCfgCheck(pn);
	 	}
}


static void
tc2105skLRCfgLoad(uint8 idx, uint8 port_num)
{
    int i;
	current_idx = idx;
	for( i=0; i<TC2105SG_PHY_INIT_LDATA_LEN; i++ ){         
            tcMiiStationWrite(port_num, tc2105sk_cfg[current_idx].ldata[i].reg_num, tc2105sk_cfg[current_idx].ldata[i].val );
        }   
	tcPhyWriteLReg(port_num,2,16,tc2105sk_L2R16[port_num]);
}

static void 
tc2105skPerPortCfgLoad(uint8 idx, uint8 port_num)
{
   int i;
   current_idx = idx;
   for( i=0; i<TC2105SG_PHY_INIT_PERPDATA_LEN; i++ ){         
            tcMiiStationWrite(port_num, tc2105sk_cfg[current_idx].perpdata[i].reg_num, tc2105sk_cfg[current_idx].perpdata[i].val );
        }  
}


static void
tc2105skCfgTx10AmpSave(void)
{
    uint16 phyAddr = mac_p->enetPhyAddr;

    if (cfg_Tx10AmpSave_flag==1){ // enable
        tcPhyWriteGReg(phyAddr,0,22,tc2105sk_G0R22_Tx10AmpSave_ON);
        TCPHYDISP4("tcphy: Tx10AmpSave enable!\r\n");       
    }
    else { // disable
        tcPhyWriteGReg(phyAddr,0,22,tc2105sk_G0R22_Tx10AmpSave_OFF);
        TCPHYDISP4("tcphy: Tx10AmpSave disable!\r\n");      
    }
}


int32
tc2105skReadProbe(uint8 port_num, uint8 mode)
{
    uint32 val, val_r31, val_g0r28;
    uint32 rval, wval;
    uint32 phyaddr = mac_p->enetPhyAddr + port_num;
    
    val_r31 = tcMiiStationRead(phyaddr, 31); 
    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    val_g0r28 = tcMiiStationRead(phyaddr, 28);
	
    
    switch(mode){
        case ProbeZfgain:
            wval = 0x0b04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x3f ;
            break;
        case ProbeAgccode:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>1)&0x1f ;
            break;
        case ProbeBoosten:
            wval = 0x2e04 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val>>6)&0x01 ;
            break;
        break;
        case ProbeSnr:
            wval = 0x0904 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0xff ;
            break;  
        case ProbeAdcSign:
            wval = 0x4104 + port_num;           
            tcMiiStationWrite( phyaddr, 28, wval );
            val = tcMiiStationRead(phyaddr, 27);
            rval = (val)&0x7f ;
            if (rval>64){
                rval -= 128;
            }
            break;
        default:
            TCPHYDISP1("\r\ntcphy error: ReadProbe %d.\r\n",mode);
            rval = 0;
            break;
    }

    tcMiiStationWrite( phyaddr, 31, 0x0000 );
    tcMiiStationWrite( phyaddr, 28, val_g0r28); // restore G0Reg28
    tcMiiStationWrite( phyaddr, 31, val_r31); // restore Reg31
    return rval;

}

#ifdef TCPHY_DEBUG
int32
tc2105skReadErrOverSum(uint8 port_num)
{
    int32 err_over_sum;
    
    tcPhyReadReg(port_num,25);
    pause(300);
    tcPhyReadReg(port_num,25);
    err_over_sum = Nmr25[port_num].err_over_cnt - Nmr25[port_num].err_over_cnt_prev;
    if( err_over_sum < 0 ){
        err_over_sum += 2048;
    }

    return err_over_sum;
}
#endif

#ifdef TCPHY_DEBUG
int32
tc2105skReadAdcSum(uint8 port_num)
{
    int32 cnt=1000;
    int32 AdcSign_sum = 0;
    int j;
	uint32 val_g3r20, val_g3r20_newval, val_l0r30, val_l1r22;

	val_g3r20=tcPhyReadGReg(port_num,3,20);
	val_g3r20_newval = (val_g3r20) & 0x7fff;
	tcPhyWriteGReg(port_num,3,20,val_g3r20_newval);//switch to full AD

	val_l0r30=tcPhyReadLReg(port_num,0,30);
	tcPhyWriteLReg(port_num,0,30,0x1510);//power down buffer
	
	val_l1r22=tcPhyReadLReg(port_num,1,22);
	tcPhyWriteLReg(port_num,1,22,0x000c);//force HP
	
    for(j=0;j<cnt;j++){
        AdcSign_sum += tc2105skReadProbe(port_num,ProbeAdcSign);
    }
    //shift right to show percent of the dc offset (unit:%)
    AdcSign_sum = (AdcSign_sum>>6);

	tcPhyWriteGReg(port_num,3,20,val_g3r20);
	tcPhyWriteLReg(port_num,0,30,val_l0r30);
	tcPhyWriteLReg(port_num,1,22,val_l1r22);
	
    return AdcSign_sum;
}

#endif

#ifdef TCPHY_DEBUG
uint16
tc2105skReadSnrSum(uint8 port_num, uint16 cnt)
{
    uint16 snr_sum = 0;
    uint16 j;
    
    for(j=0;j<cnt;j++) {
      snr_sum += tc2105skReadProbe(port_num,ProbeSnr);
    }
    return snr_sum;

}
#endif

#ifdef TCPHY_DEBUG
void 
tc2105skDispProbe100(uint8 port_num, bool long_msg)
{

    const uint16 tc2105skReadSnrCnt = 1000;

    TCPHYDISP2("tcphy[%d]:",port_num);
    TCPHYDISP2(" boosten=%ld", tc2105skReadProbe(port_num,ProbeBoosten));
    TCPHYDISP2(" agccode=%ld", tc2105skReadProbe(port_num,ProbeAgccode));
    TCPHYDISP2(" zfgain=%ld", tc2105skReadProbe(port_num,ProbeZfgain));
    TCPHYDISP2(" ch_idx=%ld", (tcPhyReadReg(port_num,29)&0x003f));
    TCPHYDISP2(" snr=%ld", tc2105skReadProbe(port_num,ProbeSnr));
    if (long_msg){
        TCPHYDISP2(" err_over_sum=%ld", tc2105skReadErrOverSum(port_num));
    }
    else {
        tcPhyReadReg(port_num,25);
    }
    TCPHYDISP2(" \r\n");
    if (long_msg){
        TCPHYDISP3(" snr_sum(x1000)=%d",tc2105skReadSnrSum(port_num,tc2105skReadSnrCnt)); // snr_sum   
        TCPHYDISP4(" adc_avg=%ld/1000", tc2105skReadAdcSum(port_num));  
        TCPHYDISP3(" \r\n");
    }
}
#endif

#ifdef TCPHY_DEBUG
void
tc2105skSwDispMessage(uint8 port_num)
{
    switch (tcphy_link_state[port_num]) {
        case ST_LINK_DOWN: 
            break;
            
        case ST_LINK_DOWN2UP: 
            //tcPhyReadReg(port_num,28); read in tc2104meSwPatch()
            TCPHYDISP1("tcphy[%d]: Link-up at %d%s.",
                port_num,(mr28.final_speed?100:10),(mr28.final_duplex?"F":"H")); 
			
			TCPHYDISP1(" tx_amp_save=%d.",(mr28.tx_amp_save));          
            TCPHYDISP1("\r\n");
			
            if(mr28.final_speed) {
                tc2105skDispProbe100(port_num,1); // long message
            }
			
            break;
            
        case ST_LINK_UP: 
            break;
            
        case ST_LINK_UP2DOWN:
            TCPHYDISP1("tcphy[%d]: Link-down!!!\r\n",port_num);
            break;
            
        default: printf("\r\ntcphy error: SwDispMessage error!\r\n");
    }
}
#endif

#ifdef TCPHY_DEBUG
void
tc2105skPoweDown(uint8 port_num)
{
    uint32 val_l0r04;
	val_l0r04 = tcPhyReadLReg(port_num, 0, 4);
	tcPhyWriteLReg(port_num,0,4,0x0001); // 	Set FLP register to disable all capability
    tcPhyWriteLReg(port_num,0,0,0x1200); //  restart-AN
    tcPhyWriteLReg(port_num,0,0,0x1800); //  power-down
    tcPhyWriteLReg(port_num,0,4,val_l0r04); //  Set FLP register to its default value
}
#endif 

#ifdef TCPHY_DEBUG
void
tc2105skUpdateErrOverSum(uint8 port_num)
{
    uint16 err_over_cnt_prev; // to store previous err_over_cnt value
    int32 val;
    
    // clear Nmr25[port_num].err_over_cnt in tc2104meDispProbe100() in ST_LINK_DOWN2UP
    if ((mr28.final_speed == 1) // 100BaseTX
        && (tcphy_link_state[port_num] == ST_LINK_UP)) 
    {
        err_over_cnt_prev = Nmr25[port_num].err_over_cnt;
        tcPhyReadReg(port_num,25);
        val = Nmr25[port_num].err_over_cnt - err_over_cnt_prev;
        if( val < 0 ){
            val += 2048;
        }
        tc2105sk_erroversum = val;
    }
    else {
        tc2105sk_erroversum = -1;
    }
}
#endif


#ifdef TCPHY_DEBUG
void
tc2105skSwErrOverMonitor(uint8 port_num)
{
    if (tcPhy_disp_level<3) 
        return; // inactive

    if (tc2105sk_erroversum>0){
        TCPHYDISP3("tcphy[%d]: ErrOver=%ld\r\n",port_num,tc2105sk_erroversum);          
    }

}
#endif
/*
#ifdef TCPHY_DEBUG
bool 
tc2105skInitialRegFlag(void)
{
    bool initial_value_flag=1; // default: correct initial value
    bool initial_value_flag_p[4]={1,1,1,1};
	bool initial_value_flag_g=1;
    uint8 pn;
    uint16 rval;

	if (eco_rev == 0x00){//tc2206F A1
		// check G3R23
        rval = tcPhyReadGReg(0,3,23);
        if (rval != tc2105sk_G3R23_default){
            initial_value_flag_g=0; // wrong initial value
            }
		
       // check L0R25
        for(pn=0;pn<TCPHY_PORTNUM;pn++){
            rval = tcPhyReadReg(pn,25);
            if ((rval&0xf000)!=tc2105sk_L0R25_default){
                initial_value_flag_p[pn]=0; // wrong initial value
            }
        }
	}
	initial_value_flag = initial_value_flag_p[0]&initial_value_flag_p[1]
		                 &initial_value_flag_p[2]&initial_value_flag_p[3]&initial_value_flag_g;
    return (initial_value_flag);
}

#endif
*/
/*
#ifdef TCPHY_DEBUG
void
tc2104meEsdSwPatch(void)
{
if (!getTcPhyInitialRegFlag()) {
	TCPHYDISP2("ESD detected!!\r\n");
	tcPhyInit(mac_p);
   }
}
#endif
*/
void 
tc2105skUp2DownFastHbt(uint8 port_num)
{
	const uint8 tc2105sk_Up2Down_Hbt_Thd = 10;
	const uint8 tc2105sk_Up2Down_Hbt_ub = 40;
    if (mr28.lch_linkup_100 & !Nmr1[port_num].link_status) {
		if (tc2105sk_Up2Down_cnt_hbt[port_num]<tc2105sk_Up2Down_Hbt_ub) {
			tc2105sk_Up2Down_cnt_hbt[port_num]+=3;
			}
    	}
	else if (!mr28.lch_linkup_100 & !Nmr1[port_num].link_status) {
		if (tc2105sk_Up2Down_cnt_hbt[port_num]>0) {
			tc2105sk_Up2Down_cnt_hbt[port_num]--;
			}
		}
	else if (mr28.lch_linkup_100 & Nmr1[port_num].link_status) {//adjust link-up time to clear counter
		tc2105sk_Up2Down_cnt_hbt[port_num]=0;
		}
	if (tc2105sk_Up2Down_cnt_hbt[port_num]> tc2105sk_Up2Down_Hbt_Thd) {
		tc2105sk_Up2DownFastHbt_detect[port_num]=1;
		}
	else {
		tc2105sk_Up2DownFastHbt_detect[port_num]=0;
		}  
}

void 
tc2105skUp2DownFastTbt(uint8 port_num)
{
	const uint8 tc2105sk_Up2Down_Tbt_Thd = 10;
	const uint8 tc2105sk_Up2Down_Tbt_ub = 40;
	const uint8 linkup_check_timer_done = 1;
    if (mr28.lch_linkup_10 & !Nmr1[port_num].link_status) {
		tc2105sk_linkup_check_timer[port_num]=0;
		if (tc2105sk_Up2Down_cnt_tbt[port_num]<tc2105sk_Up2Down_Tbt_ub) {
			tc2105sk_Up2Down_cnt_tbt[port_num]+=5;
			}
    	}
	else if (!mr28.lch_linkup_10 & !Nmr1[port_num].link_status) {
		tc2105sk_linkup_check_timer[port_num]=0;
		if (tc2105sk_Up2Down_cnt_tbt[port_num]>0) {
			tc2105sk_Up2Down_cnt_tbt[port_num]--;
			}
		}
	else if (mr28.lch_linkup_10 & Nmr1[port_num].link_status ) {
		if ((tc2105sk_linkup_check_timer[port_num] == linkup_check_timer_done)) {
			tc2105sk_Up2Down_cnt_tbt[port_num]=0;
			tc2105sk_linkup_check_timer[port_num]=0;
			}
		else if ((tc2105sk_linkup_check_timer[port_num] < linkup_check_timer_done))
			tc2105sk_linkup_check_timer[port_num]++;
		}
	
	if (tc2105sk_Up2Down_cnt_tbt[port_num]> tc2105sk_Up2Down_Tbt_Thd) {
		tc2105sk_Up2DownFastTbt_detect[port_num]=1;
		}
	else {
		tc2105sk_Up2DownFastTbt_detect[port_num]=0;
		}
	
}


bool 
tc2105skLinkFailDetect (uint8 port_num, uint8 mode){
	switch(mode){
			case TbtOrHbt:
				    if (sw_FixUp2DownFast_flag 
						&(tc2105sk_Up2DownFastTbt_detect[port_num]
						||tc2105sk_Up2DownFastHbt_detect[port_num])) {
						tc2105sk_link_fail_detect_flag =1;
		                #ifdef TCPHY_DEBUG
		                TCPHYDISP3(" tc2105sk_link_fail_detect_flag=%d\r\n",tc2105sk_link_fail_detect_flag); 
		                #endif
						}
					else 
						tc2105sk_link_fail_detect_flag =0;
				break;
			case HbtOnly:
					if (sw_FixUp2DownFast_flag & tc2105sk_Up2DownFastHbt_detect[port_num]) {
						tc2105sk_link_fail_detect_flag =1;
		                #ifdef TCPHY_DEBUG
		                TCPHYDISP3(" tc2105sk_link_fail_detect_flag=%d\r\n",tc2105sk_link_fail_detect_flag); 
		                #endif
						}
					else 
						tc2105sk_link_fail_detect_flag =0;
				break;
			case TbtOnly:
				    if (sw_FixUp2DownFast_flag & tc2105sk_Up2DownFastTbt_detect[port_num]) {
						tc2105sk_link_fail_detect_flag =1;
		                #ifdef TCPHY_DEBUG
		                TCPHYDISP3(" tc2105sk_link_fail_detect_flag=%d\r\n",tc2105sk_link_fail_detect_flag); 
		                #endif
						}
					else 
						tc2105sk_link_fail_detect_flag =0;
				break;
			default: tc2105sk_link_fail_detect_flag =0;
		}
	return tc2105sk_link_fail_detect_flag;
}

void 
tc2105skDisNextPageNeeded(uint8 port_num)
{
   const uint8 tc2105skDisNextPage_cntdone = 5;
   
   if((tcphy_link_state[port_num] == ST_LINK_UP)||(tc2105skDisEEEHappened[port_num])) {
   	tc2105skDisNextPage_cnt[port_num] = 0;
   	}
   else if ((mr5.LPNextAble) && !(mrl3_18.lp_eee_100||mrl3_18.lp_eee_1000||mrl3_18.lp_eee_10g)
   	&& (tc2105skDisNextPage_cnt[port_num]<=tc2105skDisNextPage_cntdone)) {
   	tc2105skDisNextPage_cnt[port_num]++;
   	}

   if(tc2105skDisNextPage_cnt[port_num]>tc2105skDisNextPage_cntdone) {
   	tc2105skDisNextPage_flag[port_num]=1;
   	}
   else {
   	tc2105skDisNextPage_flag[port_num]=0;
   	}
}


void 
tc2105skDisEEENeeded(uint8 port_num)
{
    const uint8 tc2105sk_linkup_check_done = 1;
	
	
    if((tcphy_link_state[port_num] == ST_LINK_UP)
		&& (tc2105sk_linkup_check_cnt[port_num]<tc2105sk_linkup_check_done)) {
		    tc2105sk_linkup_check_cnt[port_num]++;
    	}
	else if (tcphy_link_state[port_num] != ST_LINK_UP){
		  tc2105sk_linkup_check_cnt[port_num] = 0;
    	}
	
    if(tc2105skDisEEEHappened[port_num] && tc2105sk_linkup_check_cnt[port_num]==tc2105sk_linkup_check_done) {//extend link-up time
		tcPhyWriteLReg(port_num,3,17,tc2105sk_L3R17_EEEAdv);
		tc2105skDisEEEHappened[port_num]=0;
    	}
	else if(tc2105skLinkFailDetect(port_num,0) || tc2105skDisNextPage_flag[port_num]) {
		tcPhyWriteLReg(port_num,3,17,0x0000);//disable next page
		tcPhyWriteLReg(port_num,0,0,0x1200);//re-start AN
		tc2105skDisEEEHappened[port_num]=1;
    	}
}



void
tc2105skSwPatch(void)
{
    uint8 pn;

    for(pn=0;pn<TCPHY_PORTNUM;pn++){

        tcPhyReadReg(pn,1);
        //
        if( !Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_DOWN;
        }
        else if( !Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_DOWN2UP;
        }
        else if( Nmr1[pn].link_status_prev && !Nmr1[pn].link_status ){
            tcphy_link_state[pn]= ST_LINK_UP2DOWN;
        }
        else { //if( Nmr1[pn].link_status_prev && Nmr1[pn].link_status ){
            tcphy_link_state[pn] = ST_LINK_UP;
        }
    
        // read registers
        // tcPhyReadReg(pn,0);
        tcPhyReadReg(pn,5);
        tcPhyReadReg(pn,28);
		tcPhyReadLReg(pn,3,18);


		//ESD reset needed or EEE disable needed
		if (sw_FixUp2DownFast_flag) {
			tc2105skDisNextPageNeeded(pn);
			tc2105skUp2DownFastHbt(pn);
			tc2105skUp2DownFastTbt(pn);
			tc2105skDisEEENeeded(pn);
			}

        #ifdef TCPHY_DEBUG
        // display message
        tc2105skSwDispMessage(pn);
		#endif

#ifdef TCPHY_DEBUG
		if (sw_ErrOverMonitor_flag)
			tc2105skUpdateErrOverSum(pn);
        if (sw_ErrOverMonitor_flag) 
            tc2105skSwErrOverMonitor(pn); // call after tc2105skUpdateErrOverSum()
        
		
#endif

    }
    
}

#endif // TC2105SG_SUPPORT



/************************************************************************
*
*            Start of Ethernet Debug Code
*
*************************************************************************
*/

#ifdef TC2031_DBGTEST


void phy_restart(uint32 wait_time,uint32 phycfg)
{
  miiStationWrite( mac_p, 31, 0x8000 );
  miiStationWrite( mac_p,  0, 0x8000 ); // restart phy  
  tc2031CfgLoad(phycfg);
  pause(wait_time);
  
  //read local reg28 => clear latch high
  tcPhyReadReg(0,28);
  
  pause(1000);
  
  //read local reg28
  tcPhyReadReg(0,28);      
}

void phy_try_reg(uint32 f_g_reg, uint32 regnum, uint32 val, uint32 wait_time)
{
     miiStationWrite( mac_p, 31, 0x8000 );
        
     if(f_g_reg==1)
       miiStationWrite( mac_p, 31, 0x0000 );
     else
       miiStationWrite( mac_p, 31, 0x8000 );
     
     miiStationWrite( mac_p, regnum, val);    
     
     miiStationWrite( mac_p, 31, 0x8000 );
     pause(wait_time);
     
     //read local reg28 => clear latch high
     tcPhyReadReg(0,28);
     
     pause(3000);
     
     //read local reg28
     tcPhyReadReg(0,28);      
}

void phy_gainsel(uint32 agccode, uint32 wait_time)
{
  if(agccode==0)
     phy_try_reg(0,14,0x0010,wait_time); 
  else if(agccode==1)
     phy_try_reg(0,14,0x0011,wait_time);
  else if(agccode==2)
     phy_try_reg(0,14,0x0012,wait_time);
  else if(agccode==3)
     phy_try_reg(0,14,0x0013,wait_time);
  else if(agccode==4)
     phy_try_reg(0,14,0x0014,wait_time);
  else if(agccode==5)
     phy_try_reg(0,14,0x0015,wait_time);  
  else if(agccode==6)
     phy_try_reg(0,14,0x0016,wait_time);
  else // agccode == 7
     phy_try_reg(0,14,0x0017,wait_time);
}

static uint32 dc_measurement_wait_time = 20000;

void phylinkdown_dump(uint32 wait_time)
{
 uint32 dcoff = 0; 
 uint32 cnt=0;
 uint32 i=0;
 uint32 j=0;
 uint32 k=0;
 uint32 BPAGCn=8;
 uint32 LPAGCn=2;
 uint32 MAXAGCn=0;  
 
 uint32 regval_in;
 
 uint32 val;

 uint32 flag_UP_AN;
 uint32 flag_UP_100F;
 uint32 flag_UP_10F;
 
 uint32 zfgain;
 uint32 errsum;
 
 uint32 Greg;
 uint32 regcase;
 uint32 tryreg;
 uint32 tryregloop;
 
 sprintf(tc_ether_phy[cnt],"****Linkdn****"); cnt=cnt+1; 
 
 // turn off swpatch
 sw_patch_flag=0;
 
 //************************************************//
 // Measure dc offset     
 //************************************************//
 
 pause(dc_measurement_wait_time);
 dcoff=tc2031ReadAdcoffSum();
 sprintf(tc_ether_phy[cnt],"dcoff=%d",dcoff);cnt=cnt+1;
 pause(dc_measurement_wait_time);
 
 //************************************************//
 // Record RAS Version 
 //************************************************//
 
 if (tcPhyVer==tcPhyVer_2031){
    sprintf(tc_ether_phy[cnt],"RAS:%s d_%s",tc2031_cfg[current_idx].name,RASVersion);cnt=cnt+1;
 }
 else if (tcPhyVer==tcPhyVer_2101mb){
    //sprintf(tc_ether_phy[cnt],"RAS:%s d_%s",tc2101mb_cfg[current_idx].name,RASVersion);cnt=cnt+1;
 }
 else if (tcPhyVer==tcPhyVer_2101me){
    //sprintf(tc_ether_phy[cnt],"RAS:%s d_%s",tc2101me_cfg[current_idx].name,RASVersion);cnt=cnt+1;
 }

 //************************************************//
 // Check our Device setting (reg0 & reg4)     
 //************************************************//
 
 sprintf(tc_ether_phy[cnt],"***Curstate***");cnt=cnt+1;
 
 miiStationWrite( mac_p, 31, 0x8000 );
 pause(1000);
 // local end : reg0 & reg4 
 val = miiStationRead(mac_p, 0);
 sprintf(tc_ether_phy[cnt],"reg0=%x",val);cnt=cnt+1;
 
 val = miiStationRead(mac_p, 4);
 sprintf(tc_ether_phy[cnt],"reg0=%x",val);cnt=cnt+1;
  
 // remote end : reg5 & reg6
 for(i=0;i<3;i++) // for dump more information
 {   
  val = miiStationRead(mac_p, 5);
  sprintf(tc_ether_phy[cnt],"reg5=%x",val);cnt=cnt+1;
  val = miiStationRead(mac_p, 6);
  sprintf(tc_ether_phy[cnt],"reg6=%x",val);cnt=cnt+1;
  pause(3000);
 }
  
 //************************************************//
 // Execute different cases to get more information      
 //************************************************//

   // 
   // Force MDI & MDIX mode
   //
   sprintf(tc_ether_phy[cnt],"***MDI&MDIX***");cnt=cnt+1;
   for(k=0;k<2;k++) // select MDI or MDIX mode
   {   
     if(k==0) miiStationWrite( mac_p, 26, 0x5201 );
     else  miiStationWrite( mac_p, 26, 0x9201 );
     for(j=0;j<2;j++)
     {
       miiStationWrite( mac_p, 0, 0x3300 ); // restart AN
       pause(wait_time);
       for(i=0;i<3;i++)
       {
         tcPhyReadReg(0,28); // for clearing latch data
         pause(1000);
         tcPhyReadReg(0,28);
         sprintf(tc_ether_phy[cnt],"UP-%s %d",(k==0)?"MDI":"MDIX",mr28.final_link);  cnt=cnt+1;
         sprintf(tc_ether_phy[cnt],"sigdet %d",mr28.lch_sig_detect);  cnt=cnt+1;    
         sprintf(tc_ether_phy[cnt],"lkp %d",mr28.lch_rx_linkpulse);  cnt=cnt+1;
         sprintf(tc_ether_phy[cnt],"lch_100up %d",mr28.lch_linkup_100);  cnt=cnt+1;
         sprintf(tc_ether_phy[cnt],"lch_10up %d",mr28.lch_linkup_10);  cnt=cnt+1;        
       }
     }
   }
   miiStationWrite( mac_p, 26, 0x1201 );   
   
   //
   // AN, F100, & F10
   // 
   
   for(k=0;k<3;k++) // select AN, F100, or F10
   {
      if(k==0) {sprintf(tc_ether_phy[cnt],"==AN mode==");cnt=cnt+1;}
      else if(k==1) {sprintf(tc_ether_phy[cnt],"==F100 mode==");cnt=cnt+1;}
      else {sprintf(tc_ether_phy[cnt],"==F10 mode==");cnt=cnt+1;}  
      
      for(j=0;j<2;j++)
     {
       miiStationWrite( mac_p, 0, 0x8000 );  // main_reset
       pause(1000);
       
       if(k==0) miiStationWrite( mac_p, 0, 0x3300 );
       else if(k==1) miiStationWrite( mac_p, 0, 0x2100 );
       else miiStationWrite( mac_p, 0, 0x0100 );
       
       pause(wait_time);    
  
       for(i=0;i<3;i++) // for dump more information
       {
         tcPhyReadReg(0,28); // for clearing latch data
         pause(1000);
         tcPhyReadReg(0,28);       
         sprintf(tc_ether_phy[cnt],"UP-%s %d",((k==0)?"AN":(k==1)?"F100":"F10"),mr28.final_link);  cnt=cnt+1;
         if(k!=2){sprintf(tc_ether_phy[cnt],"sigdet %d",mr28.lch_sig_detect);  cnt=cnt+1;}
         if(k!=1){sprintf(tc_ether_phy[cnt],"lkp %d",mr28.lch_rx_linkpulse);  cnt=cnt+1;}
         if(k!=2){sprintf(tc_ether_phy[cnt],"lch_100up %d",mr28.lch_linkup_100);  cnt=cnt+1;}
         if(k!=1){sprintf(tc_ether_phy[cnt],"lch_10up %d",mr28.lch_linkup_10);  cnt=cnt+1;}               
       }
     }
     if(k==0) flag_UP_AN = mr28.final_link;
     if(k==1) flag_UP_100F = mr28.final_link;
     if(k==2) flag_UP_10F = mr28.final_link;   
   }
   
 //************************************************//
 // Adjust Register setting to solve this trouble
 //************************************************//
 
 if(!flag_UP_AN)
 {  
    sprintf(tc_ether_phy[cnt],"==!UP_AN==");cnt=cnt+1;
    // restart AN
    phy_restart(wait_time,0);
    
    for(regcase=0;regcase<5;regcase++) // try 5 different registers
    {
      tryregloop=(regcase==0)? 1:
                 (regcase==1)? 6:
                 (regcase==2)? 4:
                 (regcase==3)? 2:
                 1; 
      for(i=0;i<tryregloop;i++) // will try each register tryregloop times
      { 
        if(regcase==0)
        { // Adjust a_AD_CM_ADJ in G_reg28 : 4310 => 0310
          Greg = 1; tryreg = 28;
          regval_in = 0x0310;
        }
        else if(regcase==1)
        { // Adjust sig_amp_neg/pos_th in G_23
          Greg = 1; tryreg = 23;
          regval_in =(i==0)? 0x1808:
                     (i==1)? 0x1709:
                     (i==2)? 0x150b:
                     (i==3)? 0x140c:
                     (i==4)? 0x130d:
                     (i==5)? 0x120e:
                     0x160a;}  
        else if(regcase==2)
        { // Adjust lkp_quiet_amp_neg/pos_th in G_reg20   
          Greg = 1; tryreg = 20;
          regval_in =(i==0)? 0x6a88:
                     (i==1)? 0x5b88:
                     (i==2)? 0x4c88:
                     (i==3)? 0x3d88:
                     0x7988;}
        else if(regcase==3)          
        { // Adjust snrdef_short/long in reg21 : 0068 => 008a 
          Greg = 0; tryreg = 21;
          regval_in =(i==0)? 0x0079:
                     (i==1)? 0x008a:
                     0x0068;}        
        else          
        { // Adjust lkp_quiet_end/start_th in G_reg19 : d664 => fa64
          Greg = 1; tryreg = 19;
          regval_in =0xfa64;  }
        
        // capture default setting   
        if(Greg==1) miiStationWrite( mac_p, 31, 0x0000 );
        else   miiStationWrite( mac_p, 31, 0x8000 );
        val = miiStationRead(mac_p, tryreg);  
        
        // change setting for trying to build a link
        phy_try_reg(Greg,tryreg,regval_in,1000);
        // restart AN
        miiStationWrite( mac_p, 31, 0x8000 );
        miiStationWrite( mac_p,  0, 0x3300 );
        tcPhyReadReg(0,28); // for clearing latch data
        pause(wait_time);
        // capture information
        tcPhyReadReg(0,28);
        sprintf(tc_ether_phy[cnt],"%s%d-%x:%d%d%d%d%d",((Greg==1)?"G":"L"),tryreg,regval_in,mr28.final_link,mr28.lch_sig_detect,mr28.lch_rx_linkpulse,mr28.lch_linkup_100,mr28.lch_linkup_10);cnt=cnt+1;       
        
        if(mr28.final_link==1)
          {
           sprintf(tc_ether_phy[cnt],"Speed=%d%s",((mr28.final_speed==1)?100:10),((mr28.final_duplex==1)?"F":"H"));
           cnt=cnt+1;                 
          } 
      } 
      // set register to its default setting
      phy_try_reg(Greg,tryreg,val,1000);
    }
 }
 
 if(!flag_UP_100F)
 {
   sprintf(tc_ether_phy[cnt],"==!UP_F100==");cnt=cnt+1;
   phy_restart(wait_time,0);
   
   for(regcase=0;regcase<2;regcase++) // try 2 different registers
    {
      tryregloop=(regcase==0)? 6: 2;

      for(i=0;i<tryregloop;i++)
      { 
        if(regcase==0)
        { // Adjust sig_amp_neg/pos_th in G_23
          Greg = 1; tryreg = 23;
          regval_in =(i==0)? 0x1808:
                     (i==1)? 0x1709:
                     (i==2)? 0x150b:
                     (i==3)? 0x140c:
                     (i==4)? 0x130d:
                     (i==5)? 0x120e:
                     0x160a;}  
        else     
        { // Adjust snrdef_short/long in reg21 : 0068 => 008a 
          Greg = 0; tryreg = 21;
          regval_in =(i==0)? 0x0079:
                     (i==1)? 0x008a:
                     0x0068;}        
        
        // capture default setting   
        if(Greg==1) miiStationWrite( mac_p, 31, 0x0000 );
        else   miiStationWrite( mac_p, 31, 0x8000 );
        val = miiStationRead(mac_p, tryreg);  
        
        // change setting for trying to build a link
        phy_try_reg(Greg,tryreg,regval_in,1000);
        // re-link
        miiStationWrite( mac_p, 31, 0x8000 );
        miiStationWrite( mac_p,  0, 0x3300 );
        phy_try_reg(0,0,0x2100,0);
        tcPhyReadReg(0,28); // for clearing latch data
        pause(wait_time);
        // capture information
        tcPhyReadReg(0,28);
        sprintf(tc_ether_phy[cnt],"%s%d-%x:%d%d%d%d%d",((Greg==1)?"G":"L"),tryreg,regval_in,mr28.final_link,mr28.lch_sig_detect,mr28.lch_rx_linkpulse,mr28.lch_linkup_100,mr28.lch_linkup_10);cnt=cnt+1;       
        if(mr28.final_link==1)
          {
            sprintf(tc_ether_phy[cnt],"Speed=%d%s",((mr28.final_speed==1)?100:10),((mr28.final_duplex==1)?"F":"H"));
            cnt=cnt+1;
          }                  
      } 
      // set register to its default setting
      phy_try_reg(Greg,tryreg,val,1000);
    }
   //
   // Adjust Boosten & agc gain in reg17 & reg14    
   //
   for(i=0;i<2;i++) // i means selection of boosten
   {
      MAXAGCn =(i==0)?LPAGCn : BPAGCn;
                              
      for(j=0; j<MAXAGCn ; j++) // j means selection of agc gain
      {
       phy_restart(wait_time,0);  
       
       if(i==0) phy_try_reg(0,17,0x0008,0);
       else     phy_try_reg(0,17,0x000c,0); 
       
       phy_try_reg(0,0,0x2100,0);                   
       phy_gainsel(j,wait_time);

       //read local reg28
       tcPhyReadReg(0,28);
       
       if(!mr28.final_link)
        {             
          sprintf(tc_ether_phy[cnt],"%s%d %d %d %d",((i==0)?"LP":"BP"),j,mr28.final_link,mr28.lch_sig_detect,mr28.lch_linkup_100);
          cnt=cnt+1; 
        }
       else
        {
          zfgain=tc2031ReadProbe(ProbeZfgain);
          errsum=tc2031ReadErroverSum();
          sprintf(tc_ether_phy[cnt],"%s%d ",((i==0)?"LP":"BP"),j,mr28.final_link);cnt=cnt+1;
          sprintf(tc_ether_phy[cnt],"zf=%d err=%d",zfgain,errsum);cnt=cnt+1;  
        }  
       }            
   }
    
 }
 
 // DUT can't detects 10BT link-pulse signal so that DUT can't link-up at 10BT  
 if(!flag_UP_10F)  
 {
   sprintf(tc_ether_phy[cnt],"==!UP_F10==");cnt=cnt+1;
   
   phy_restart(wait_time,2);
   
   for(regcase=0;regcase<2;regcase++) // try 2 different registers
    {
      tryregloop=(regcase==0)? 4: 1;

      for(i=0;i<tryregloop;i++)
      { 
        if(regcase==0)
        { // Adjust lkp_quiet_amp_neg/pos_th in G_reg20
          Greg = 1; tryreg = 20;
          regval_in =(i==0)? 0x6a88:
                     (i==1)? 0x5b88:
                     (i==2)? 0x4c88:
                     (i==3)? 0x3d88:
                     0x7988;}  
        else     
        { // Adjust lkp_quiet_end/start_th in G_reg19 : d664 => fa64
          Greg = 1; tryreg = 19;
          regval_in = 0xfa64;}        
        
        // capture default setting   
        if(Greg==1) miiStationWrite( mac_p, 31, 0x0000 );
        else   miiStationWrite( mac_p, 31, 0x8000 );
        val = miiStationRead(mac_p, tryreg);  
        
        // change setting for trying to build a link
        phy_try_reg(Greg,tryreg,regval_in,1000);
        // re-link
        miiStationWrite( mac_p, 31, 0x8000 );
        miiStationWrite( mac_p,  0, 0x3300 );
        phy_try_reg(0,0,0x0100,0);
        tcPhyReadReg(0,28); // for clearing latch data
        pause(wait_time);
        // capture information
        tcPhyReadReg(0,28);
        sprintf(tc_ether_phy[cnt],"%s%d-%x:%d%d%d%d%d",((Greg==1)?"G":"L"),tryreg,regval_in,mr28.final_link,mr28.lch_sig_detect,mr28.lch_rx_linkpulse,mr28.lch_linkup_100,mr28.lch_linkup_10);cnt=cnt+1;       
        if(mr28.final_link==1)
         {
           sprintf(tc_ether_phy[cnt],"Speed=%d%s",((mr28.final_speed==1)?100:10),((mr28.final_duplex==1)?"F":"H"));
           cnt=cnt+1;
         }                 
      } 
      // set register to its default setting
      phy_try_reg(Greg,tryreg,val,1000);
    }
  }
 // msgcnt will be used in doPhyDebugPrint(...)
 msgcnt = cnt;
 phy_restart(wait_time,0);
}

void phy100BT_dump(uint32 wait_time)
{
  extern const cmds_t   Cmds[];
  char cmdbuf[80]; 
  spDHCPLanIp_t         ether;  
  
  uint32 cnt=0;
  uint32 boosten;
  uint32 agccode;
  uint32 errsum;
  uint32 zfgain;
  uint32 dcoff;
  
  uint32 i=0;
  uint32 j=0;
  uint32 BPAGCn=8;
  uint32 LPAGCn=2;
  uint32 MAXAGCn=0;
  uint32 regval_in;
  uint32 default_cfg;
  uint32 linkmode=0;     
  
  // local device
  uint8  mr_anen;
  uint8  lr_dplx;
  // link-partner
  uint8  mr_lp_anen;
  uint8 mr_lp_an_capable;
  uint32 val;
  
  spGetEtherIp (MAC_CHAN_ID, &ether);
  
  sprintf(tc_ether_phy[cnt],"****100BT****"); cnt=cnt+1; 
  
  sw_patch_flag = 0;

  //************************************************//
  // Measure dc offset     
  //************************************************//

  pause(dc_measurement_wait_time);
  dcoff=tc2031ReadAdcoffSum();
  sprintf(tc_ether_phy[cnt],"dcoff=%d",dcoff);cnt=cnt+1;
  pause(dc_measurement_wait_time);
  
  //************************************************//
  // Record RAS Version 
  //************************************************//
 
  sprintf(tc_ether_phy[cnt],"RAS:%s d_%s",tc2031_cfg[current_idx].name,RASVersion);cnt=cnt+1;
  
  //************************************************//
  // Execute different cases to get more information      
  //************************************************//
  
  //read local reg28 => clear latch high
  tcPhyReadReg(0,28);
  
  pause(3000);
  
  //read local reg28
  tcPhyReadReg(0,28);   
  
  if(mr28.final_link==1 && mr28.final_speed==1)
  { 
    //************************************************//
    // check default operation mode
    //************************************************//
        
      // local  
      val = miiStationRead(mac_p, 0);
        mr_anen = (val>>12)&0x01;
      
      // remote
      val = miiStationRead(mac_p, 6);
      mr_lp_anen = (val)&0x01;
      val = miiStationRead(mac_p, 5);
        mr_lp_an_capable = (val>>5)&0x0F; //100F,100H,10F,10H
       
      sprintf(tc_ether_phy[cnt],"Local:%s-%d%s",(mr_anen)?"AN":"F",(mr28.final_speed?100:10),(mr28.final_duplex?"F":"H")); cnt=cnt+1;
      if(mr_lp_anen) 
      {
        sprintf(tc_ether_phy[cnt]," Remote:AN-(");cnt=cnt+1;
          sprintf(tc_ether_phy[cnt]," %s,",((mr_lp_an_capable>>3)&0x01)?"100F":"");cnt=cnt+1;
          sprintf(tc_ether_phy[cnt]," %s,",((mr_lp_an_capable>>2)&0x01)?"100H":"");cnt=cnt+1;   
          sprintf(tc_ether_phy[cnt]," %s,",((mr_lp_an_capable>>1)&0x01)?"10F":"");cnt=cnt+1;        
          sprintf(tc_ether_phy[cnt]," %s)",((mr_lp_an_capable>>0)&0x01)?"10H":"");cnt=cnt+1;    
      }
      else
      {
        sprintf(tc_ether_phy[cnt],"Remote:Force",(mr28.final_speed?100:10),(mr28.final_duplex?"F":"H")); cnt=cnt+1;
      }
     
      // record current setting
      boosten=tc2031ReadProbe(ProbeBoosten);    
      agccode=tc2031ReadProbe(ProbeAgccode);   
      zfgain=tc2031ReadProbe(ProbeZfgain);
      errsum=tc2031ReadErroverSum();
      
      // save current configuration   
      default_cfg = current_idx;
      
      // first ping to make MAC layer alive     
      sprintf(cmdbuf, "etherd ping %s 60 5000 0", (char*)inet_ntoa(ether.spLanIp.ipAddr+10));   
      cmdparse(Cmds, cmdbuf, NULL);
      sprintf(cmdbuf,"etherd pingechocnt clear");
        cmdparse(Cmds, cmdbuf, NULL); 
      
      // performance test in default setting
      sprintf(cmdbuf, "etherd ping %s 1514 10000 0", (char*)inet_ntoa(ether.spLanIp.ipAddr+10));    
        cmdparse(Cmds, cmdbuf, NULL);
            
      // capture current state information
      sprintf(tc_ether_phy[cnt],"***curstate***"); cnt=cnt+1;
      sprintf(tc_ether_phy[cnt],"Cfg%d %s%d zf=%d",default_cfg,(boosten==1)?"BP":"LP",agccode,zfgain); cnt=cnt+1;
      sprintf(tc_ether_phy[cnt],"E:%dR:%d",errsum,PingReplyCnt); cnt=cnt+1;
     
      sprintf(cmdbuf,"etherd pingechocnt clear");
        cmdparse(Cmds, cmdbuf, NULL); 
              
    //************************************************//
    // Adjust RX setting 
    //************************************************//
       sprintf(tc_ether_phy[cnt],"**RX:%s**",(linkmode==0)?"AN":"F100"); cnt=cnt+1; 
       
       for(i=0;i<2;i++)   // i means selection of boosten
       {
         MAXAGCn =(i==0)?LPAGCn : BPAGCn;
         for(j=0; j<MAXAGCn ; j++) // j means selection of agc gain
         {          
            if(mr_lp_anen==1) phy_try_reg(0,0,0x3300,wait_time);
            else phy_try_reg(0,0,0x2100,wait_time);
              
            miiStationWrite( mac_p, 31, 0x8000 );
            if(i==0) miiStationWrite( mac_p, 17, 0x0008 ); 
            else  miiStationWrite( mac_p, 17, 0x000c );
            
            phy_gainsel(j,wait_time);
            
            if(!mr28.final_link)
            {             
              sprintf(tc_ether_phy[cnt],"%s %s%d LD!",((mr_lp_anen==1)?"AN":"F100"),((i==0)?"LP":"BP"),j);
              cnt=cnt+1; 
            }
            else
            {
              zfgain=tc2031ReadProbe(ProbeZfgain);
              errsum=tc2031ReadErroverSum();
              sprintf(tc_ether_phy[cnt],"%s %s%d zf=%d",((mr_lp_anen==1)?"AN":"F100"),((i==0)?"LP":"BP"),j,zfgain);
              cnt=cnt+1;
                              
              sprintf(cmdbuf, "etherd ping %s 60 5000 0", (char*)inet_ntoa(ether.spLanIp.ipAddr+10));   
              cmdparse(Cmds, cmdbuf, NULL);
              sprintf(cmdbuf,"etherd pingechocnt clear");
                cmdparse(Cmds, cmdbuf, NULL);         
              
              sprintf(cmdbuf, "etherd ping %s 1514 10000 0", (char*)inet_ntoa(ether.spLanIp.ipAddr+10));    
                cmdparse(Cmds, cmdbuf, NULL);
                
                sprintf(tc_ether_phy[cnt],"E:%dR=%d",errsum,PingReplyCnt);cnt=cnt+1;
              
              sprintf(cmdbuf,"etherd pingechocnt clear");
                cmdparse(Cmds, cmdbuf, NULL);         
           }        
         }
       }   
       
       miiStationWrite( mac_p, 31, 0x8000 );
       miiStationWrite( mac_p, 14, 0x0000 );
       miiStationWrite( mac_p, 17, 0x0000 );
       
    //************************************************//
    // Adjust TX setting 
    //************************************************//
     
       sprintf(tc_ether_phy[cnt],"**TX:%s**",(linkmode==0)?"AN":"F100"); cnt=cnt+1; 
       phy_restart(wait_time,default_cfg);
   
       for(i=0;i<7;i++)
       {               
         if(linkmode==0) phy_try_reg(0,0,0x3300,wait_time);
         else phy_try_reg(0,0,0x2100,wait_time);         
          
         regval_in =(i==0)? 0x9017:
                    (i==1)? 0xa007:
                    (i==2)? 0xa017:
                    (i==3)? 0x8007:
                    (i==4)? 0x8017:
                    (i==5)? 0x7007:
                    (i==6)? 0x7017:   
                    0x9007; // R36 setting
         
         phy_try_reg(1,11,regval_in,wait_time);

         sprintf(tc_ether_phy[cnt],"GR11-%x:%d",regval_in,mr28.final_link); cnt=cnt+1;
         
         if(mr28.final_link)
          {               
               zfgain=tc2031ReadProbe(ProbeZfgain);
             errsum=tc2031ReadErroverSum();
             boosten=tc2031ReadProbe(ProbeBoosten);    
             agccode=tc2031ReadProbe(ProbeAgccode); 
             sprintf(tc_ether_phy[cnt],"%s %s%d zf=%d",((mr_lp_anen==1)?"AN":"F100"),((boosten==0)?"LP":"BP"),agccode,zfgain);
             cnt=cnt+1;
                             
             sprintf(cmdbuf, "etherd ping %s 60 5000 0", (char*)inet_ntoa(ether.spLanIp.ipAddr+10));    
             cmdparse(Cmds, cmdbuf, NULL);
             sprintf(cmdbuf,"etherd pingechocnt clear");
               cmdparse(Cmds, cmdbuf, NULL);         
             
             sprintf(cmdbuf, "etherd ping %s 1514 10000 0", (char*)inet_ntoa(ether.spLanIp.ipAddr+10));     
               cmdparse(Cmds, cmdbuf, NULL);
               
               sprintf(tc_ether_phy[cnt],"E:%dR=%d",errsum,PingReplyCnt);cnt=cnt+1;
             
             sprintf(cmdbuf,"etherd pingechocnt clear");
               cmdparse(Cmds, cmdbuf, NULL);                              
          }            
        }
        phy_try_reg(1,11,0x9007,wait_time);         
   }
  else
   {
    sprintf(tc_ether_phy[cnt],"L-D,Plz retry"); cnt=cnt+1;  
   }
   // msgcnt will be used in doPhyDebugPrint(...)
   msgcnt = cnt;       
}

void phy10BT_dump(uint32 wait_time)
{
  extern const cmds_t   Cmds[];
  char cmdbuf[80]; 
  spDHCPLanIp_t         ether;  
  
  uint32 dcoff;
  uint32 linkmode=0;
  uint32 cnt=0;
  uint32 i=0;
  uint32 regval_in;

  // local device
  uint8  mr_anen;
  uint8  lr_dplx;
  // link-partner
  uint8  mr_lp_anen;
  uint8 mr_lp_an_capable;
  uint32 val;
  
  spGetEtherIp (MAC_CHAN_ID, &ether);
  
  sprintf(tc_ether_phy[cnt],"****10BT****"); cnt=cnt+1;
  
  sw_patch_flag = 0;
  
  //************************************************//
  // Measure dc offset     
  //************************************************//

  pause(dc_measurement_wait_time);
  dcoff=tc2031ReadAdcoffSum();
  sprintf(tc_ether_phy[cnt],"dcoff=%d",dcoff);cnt=cnt+1;
  pause(dc_measurement_wait_time);
  
  //************************************************//
  // Record RAS Version 
  //************************************************//
 
  sprintf(tc_ether_phy[cnt],"RAS:%s d_%s",tc2031_cfg[current_idx].name,RASVersion);cnt=cnt+1;
  
  //************************************************//
  // Execute different cases to get more information      
  //************************************************//
  
  //read local reg28 => clear latch high
  tcPhyReadReg(0,28);
  
  pause(3000);
  
  //read local reg28
  tcPhyReadReg(0,28);     
  
  if(mr28.final_link && mr28.final_speed==0)
  {
    //************************************************//
    // check default operation mode
    //************************************************//
  
      // local  
      val = miiStationRead(mac_p, 0);
      mr_anen = (val>>12)&0x01;
      
      // remote
      val = miiStationRead(mac_p, 6);
      mr_lp_anen = (val)&0x01;
      val = miiStationRead(mac_p, 5);
      mr_lp_an_capable = (val>>5)&0x0F; //100F,100H,10F,10H
            
      sprintf(tc_ether_phy[cnt],"Local:%s-%d%s",(mr_anen)?"AN":"F",(mr28.final_speed?100:10),(mr28.final_duplex?"F":"H")); cnt=cnt+1;
      if(mr_lp_anen) 
      {
        sprintf(tc_ether_phy[cnt]," Remote:AN-(");cnt=cnt+1;
          sprintf(tc_ether_phy[cnt]," %s,",((mr_lp_an_capable>>3)&0x01)?"100F":"");cnt=cnt+1;
          sprintf(tc_ether_phy[cnt]," %s,",((mr_lp_an_capable>>2)&0x01)?"100H":"");cnt=cnt+1;   
          sprintf(tc_ether_phy[cnt]," %s,",((mr_lp_an_capable>>1)&0x01)?"10F":"");cnt=cnt+1;        
          sprintf(tc_ether_phy[cnt]," %s)",((mr_lp_an_capable>>0)&0x01)?"10H":"");cnt=cnt+1;    
      }
      else
      {
        sprintf(tc_ether_phy[cnt],"Remote:Force",(mr28.final_speed?100:10),(mr28.final_duplex?"F":"H")); cnt=cnt+1;
      }
    
      pause(3000);
      sprintf(cmdbuf, "etherd ping %s 60 5000 1", (char*)inet_ntoa(ether.spLanIp.ipAddr+10));   
      cmdparse(Cmds, cmdbuf, NULL);
      sprintf(cmdbuf,"etherd pingechocnt clear");
        cmdparse(Cmds, cmdbuf, NULL); 
      
      sprintf(cmdbuf, "etherd ping %s 1514 10000 1", (char*)inet_ntoa(ether.spLanIp.ipAddr+10));    
        cmdparse(Cmds, cmdbuf, NULL);
      
      sprintf(tc_ether_phy[cnt],"***curstate***"); cnt=cnt+1;
      sprintf(tc_ether_phy[cnt],"R:%d",PingReplyCnt); cnt=cnt+1;
      
      sprintf(cmdbuf,"etherd pingechocnt clear");
        cmdparse(Cmds, cmdbuf, NULL);

      //************************************************//
      // Adjust RX setting 
      //************************************************//
        
      sprintf(tc_ether_phy[cnt],"**RX:%s**",(linkmode==0)?"AN":"F100"); cnt=cnt+1; 
      phy_restart(wait_time,2);
      
      for(i=0;i<3;i++)
      {  
         phy_try_reg(0,4,0x0061,wait_time);
         
         if(mr_lp_anen==1) phy_try_reg(0,0,0x1200,wait_time);
         else phy_try_reg(0,0,0x0100,wait_time);
          
         // Adjust ini_10_agcsel in G_reg3 : a166 => xxxx
         regval_in = (i==0)? 0x8166 :
                     (i==1)? 0xc166 :
                     (i==2)? 0xe166 :
                     0xa166;  
         
         phy_try_reg(1,3,regval_in,wait_time);
         
         // clear reg28 latch data
         tcPhyReadReg(0,28);
         
         pause(3000);
                     
         //read local reg28
         tcPhyReadReg(0,28);      
         
         sprintf(tc_ether_phy[cnt],"GR3-%x:%d",regval_in,mr28.final_link); cnt=cnt+1;        
         
         if(mr28.final_link)
         {
           pause(3000);
           sprintf(cmdbuf, "etherd ping %s 60 5000 1", (char*)inet_ntoa(ether.spLanIp.ipAddr+10));  
           cmdparse(Cmds, cmdbuf, NULL);
           sprintf(cmdbuf,"etherd pingechocnt clear");
             cmdparse(Cmds, cmdbuf, NULL); 
           
           sprintf(cmdbuf, "etherd ping %s 1514 10000 1", (char*)inet_ntoa(ether.spLanIp.ipAddr+10));   
             cmdparse(Cmds, cmdbuf, NULL);
             sprintf(tc_ether_phy[cnt],"RpCnt=%d",PingReplyCnt);
           cnt=cnt+1;
           
           sprintf(cmdbuf,"etherd pingechocnt clear");
             cmdparse(Cmds, cmdbuf, NULL);       
          
         }      
      }
       phy_try_reg(1,3,0xa166,wait_time);           
  }
  else
  {
    sprintf(tc_ether_phy[cnt],"L-D,Plz retry"); cnt=cnt+1;  
  }
  
  // msgcnt will be used in doPhyDebugPrint(...)
  msgcnt = cnt;    
}

static void doPhyDebugTest (int argc, char *argv[], void *p)
{
  int debug_mode=0;
  int wait_time=0; 
  uint32 i=0;
  uint32 j=0;
  debug_mode = atoi(argv[1]);
  wait_time  = 8000;
  if(argc==3){
    wait_time = atoi(argv[2]);
  }
  
  // reset dumped matrix & counter
  for(i=0;i<MAX_RECORD_TC_ETHER_PHY_STR_NUM;i++)
  {
   for(j=0;j<MAX_RECORD_TC_ETHER_PHY_STR_LEN;j++) 
     tc_ether_phy[i][j]='\0';
  } 
  msgcnt = 0;
  
  printf("********Test begins********\n");
  // run testcase
  if(debug_mode==1)
    {            
      printf("Procedure: \n");
      printf(" 1. un-plug cable within %d seconds \n",(dc_measurement_wait_time-5000)/1000);
      printf(" 2. after un-plug cable for %d seconds, please plug cable again \n",(dc_measurement_wait_time)/1000);
        printf(" 3. the following test will take 16 minutes to complete \n");
        printf(" 4. please key in command to display message \n");
        printf(" 5. please capture the message, then send it to TrendChip \n");
      //pause(wait_time);  
      phylinkdown_dump(wait_time);
        printf(" * dbgtest 1 test done. \n");
    }  
  else if(debug_mode==2)
    {
      printf("Procedure: \n");
      printf(" 1. un-plug cable within %d seconds \n",(dc_measurement_wait_time-5000)/1000);
      printf(" 2. after un-plug cable for %d seconds, please plug cable again \n",(dc_measurement_wait_time)/1000);
        printf(" 3. the following test will take 25 minutes to complete \n");
        printf(" 4. please key in command to display message \n");
        printf(" 5. please capture the message, then send it to TrendChip \n");
      //pause(wait_time);  
      phy100BT_dump(wait_time);
        printf(" * dbgtest 2 test done. \n");
    }
  else if(debug_mode==3)
    { 
      printf("Procedure: \n");
      printf(" 1. un-plug cable within %d seconds \n",(dc_measurement_wait_time-5000)/1000);
      printf(" 2. after un-plug cable for %d seconds, please plug cable again \n",(dc_measurement_wait_time)/1000);
        printf(" 3. the following test will take 16 minutes to complete \n");
        printf(" 4. please key in command to display message \n");
        printf(" 5. please capture the message, then send it to TrendChip \n");
      //pause(wait_time); 
      phy10BT_dump(wait_time);
        printf(" * dbgtest 3 test done. \n");
    } 
  else
    { 
      printf("Usage: etherdbg dbgtest <1|2|3> [wait_time] \n");
    } 
  
  sw_patch_flag = 1;
  miiStationWrite( mac_p, 31, 0x8000 );
  miiStationWrite( mac_p,  0, 0x8000 );
}

#endif // TC2031_Field_DEBUG
/************************************************************************
*
*            End of Ethernet Debug Code
*
*************************************************************************
*/




/************************************************************************
*                       DATA for TCPHY_SUPPORT
*************************************************************************
*/

// function declaration for TCEPHYDBG commands
#ifdef TCPHY_SUPPORT
static int doPhySwVer(int argc, char *argv[], void *p);
static int doPhyMiiRead (int argc, char *argv[], void *p);
static int doPhyMiiWrite(int argc, char *argv[], void *p);
static int doPhyConfig (int argc, char *argv[], void *p);
//void tcMiiExtStationWrite(uint32 devAddr,uint32 portAddr,uint32 regAddr,uint32 miiData);
//uint32 tcMiiExtStationRead(uint32 devAddr, uint32 portAddr, uint32 regAddr, uint8  op);

#ifdef TCPHY_1PORT
static int doPhySpeed (int argc, char *argv[], void *p);
#endif
#ifdef TCPHY_DEBUG
static int doMacRegDump (int argc, char *argv[], void *p);
static int doPhyInit(int argc, char *argv[], void *p);
static int doPhySwPatch (int argc, char *argv[], void *p);
static int doPhyRegCheck (int argc, char *argv[], void *p);
#ifndef LINUX_OS
static int doPhyLoopback (int argc, char *argv[], void *p);
#endif
static int doMacSend(int argc, char *argv[], void *p);
static int doMacSendRandom(int argc, char *argv[], void *p);
#ifndef PURE_BRIDGE
static int doPing (int argc, char *argv[], void *p);
static int doPingEchoCnt (int argc, char *argv[], void *p);
#endif
static int doPhyDispFlag (int argc, char *argv[], void *p);
static int doPhyChkVal (int argc, char *argv[], void *p);

static int doPhyForceLink (int argc, char *argv[], void *p);
static int doErrMonitor (int argc, char *argv[], void *p);
#ifdef TC2031_DEBUG
static int doPhyErrOver (int argc, char *argv[], void *p);
#endif
#elif defined(MTPHY_DEBUG)
static int doPhyInit(int argc, char *argv[], void *p);
static int doDbgPorbeSet(int argc, char *argv[], void *p);
static int doReadPhycoeff(int argc, char *argv[], void *p);
static int doPhyMiiRead_TrDbg(int argc, char *argv[], void *p);
static int doPhyMiiWrite_TrDbg(int argc, char *argv[], void *p);
static int doPhyChkVal (int argc, char *argv[], void *p);
static int doPhyMMDRead(int argc, char *argv[], void *p);
static int doPhyMMDWrite(int argc, char *argv[], void *p);
static int doLpbk_setting(int argc, char *argv[], void *p);
static int doGbeTxShaper (int argc, char *argv[], void *p); //dai120828
static int doFeTxShaper (int argc, char *argv[], void *p); //dai120828
static int doPhyPktCnt(int argc, char *argv[], void *p);
static int doPhySwPatch (int argc, char *argv[], void *p);
static int doSkewDispFlag (int argc, char *argv[], void *p);
static int doPhyDispFlag (int argc, char *argv[], void *p);

#endif // TCPHY_DEBUG
#ifdef TCPHY_FIELD_DBGTEST
static int doFieldDebugTest (int argc, char *argv[], void *p);
static int doFieldDebugPrint (int argc, char *argv[], void *p);
#endif // TCPHY_FIELD_DBGTEST

static const cmds_t ethertphyprint[] =
{
	{"ver",       doPhySwVer,           0x02, 0, NULL},
	{"miir",      doPhyMiiRead,         0x02, 0, NULL},
	{"miiw",      doPhyMiiWrite,        0x02, 0, NULL},
	{"config",    doPhyConfig,          0x02, 0, NULL},          
#ifdef TCPHY_1PORT
	{"speed",     doPhySpeed,           0x02, 0, NULL}, 
#endif

#ifdef LINUX_OS
#ifdef EEE_SUPPORT
	#ifdef EEE_DEBUG
	{"reg_check",	doRegCheck,	0x02,	0x0,	"reg_check <value>"},
	{"dbgpsm", doDbgPSM,	0x02,	0x1,	"dbgpsm <0|1>"},
	{"waitcnt",		doPsmWaitCnt,	0x02,	0x1,	"waitcnt <value>"},
	{"loopback",	doPsmLpbk,	0x02,	0x3,	"loopback <mii|ext|mac|none> <len> <cnt>"},
	{"pktgen",	doPktGen,	0x02,	0x3,	"pktgen <len> <cnt> <delay>"},
	#endif
	{"psm",		doPsmMode,	0x02,	0x1,	"psm <0|1>"},
	{"mask",	doPsmIntMask,	0x02,	0x1,	"mask <value>"},
	{"tx_psm_mode",	doTxPsmMode,	0x02,	0x1,	"tx_psm_mode <0|1>"},
	{"psmThreshold",doPsmThreshold,	0x02,	0x1,	"psmThreshold <value>"},
	{"wakeUpTime",doPsmWakeUpTime,	0x02,	0x1,	"wakeUpTime <value>"},
	{"emiir",      	doExtendMiir,          0x02, 0,  NULL},
	{"emiiw",      	doExtendMiiw,         0x02, 0x4,  "emiiw <devaddr> <phyaddr> <reg> <val>"},
	{"stoprxclk", doStopRxClk,	0x02,	0x1,	"stoprxclk <0|1>"},
#endif
#else 
#ifdef TC3162U_PSM_SUPPORT
    #ifdef TC3162U_PSM_DEBUG
        {"reg_check",   doRegCheck, 0x02,   0x0,    "reg_check <value>"},
        {"dbgpsm", doDbgPSM,    0x02,   0x1,    "dbgpsm <0|1>"},
        {"waitcnt",     doPsmWaitCnt,   0x02,   0x1,    "waitcnt <value>"},
    #endif
        {"psm",     doPsmMode,  0x02,   0x1,    "psm <0|1>"},
        {"mask",    doPsmIntMask,   0x02,   0x1,    "mask <value>"},
        {"tx_psm_mode", doTxPsmMode,    0x02,   0x1,    "tx_psm_mode <0|1>"},
        {"psmThreshold",doPsmThreshold, 0x02,   0x1,    "psmThreshold <value>"},
        {"wakeUpTime",doPsmWakeUpTime,  0x02,   0x1,    "wakeUpTime <value>"},
        {"emiir",       doExtendMiir,          0x02, 0,  NULL},
        {"emiiw",       doExtendMiiw,         0x02, 0x4,  "emiiw <devaddr> <phyaddr> <reg> <val>"},
        {"stoprxclk", doStopRxClk,  0x02,   0x1,    "stoprxclk <0|1>"},
#endif
#endif
#ifdef TCPHY_DEBUG
	{"macreg",	  doMacRegDump, 		0x02, 0, NULL},
	{"reset",     doPhyInit,            0x02, 0, NULL},     
	{"swpatch",   doPhySwPatch,         0x02, 0, NULL},

	{"regcheck",  doPhyRegCheck,  0x02, 0,  NULL},   
#ifndef LINUX_OS   
	{"loopback",  doPhyLoopback,  0x02, 0,  NULL},  
#endif
	{"send",      doMacSend,      0x02, 0x3,    "send <pattern> <len> <loop>"},
	{"sendrandom", doMacSendRandom, 0x02, 0,    NULL},
#ifndef PURE_BRIDGE     
	{"ping",      doPing,        0x02, 3,    "ping <ip> <len> <loopnum> [delay_cnt]"},
	{"pingechocnt", doPingEchoCnt, 0x02, 1,    "pingechocnt <display | clear>"},
#endif
	{"dispflag",    doPhyDispFlag,  0x02, 0,    NULL},          
	{"chkval",  doPhyChkVal,  0x02, 0,  NULL},              
	{"forcelink",   doPhyForceLink,  0x02, 0,   NULL},      
	{"errmonitor",      doErrMonitor,         0x02, 0,    NULL},    
#ifdef TC2031_DEBUG
	{"errover", doPhyErrOver,  0x02, 0, NULL},  
#endif
#elif defined(MTPHY_DEBUG)
	{"reset",     doPhyInit,            0x02, 0, NULL},     
	{"chkval",  doPhyChkVal,  0x02, 1,  "chkval <phyaddr>"},  
	{"emiir",		doPhyMMDRead,		  0x02, 0,  NULL},
	{"emiiw",		doPhyMMDWrite,		  0x02, 0,  NULL},
	{"emiiwb",	doPhyMMDWrite,		  0x02, 0, NULL},
	{"miiwb",	  doPhyMiiWrite,		0x02, 0, NULL},
	{"dbgprb",	  doDbgPorbeSet,		0x02, 0, NULL},
	{"miir_trdbg",	  doPhyMiiRead_TrDbg,		0x02, 0, NULL},
	{"miiw_trdbg",  doPhyMiiWrite_TrDbg,		0x02, 0, NULL},
	{"coeffr", doReadPhycoeff, 0x02, 0, NULL},
	{"lbpk",	doLpbk_setting,		0x02, 0, NULL},
	{"gtxshaper", doGbeTxShaper, 0x02, 0, NULL},
	{"ftxshaper", doFeTxShaper, 0x02, 0, NULL},
	{"phycnt",	doPhyPktCnt,		0x02, 0, NULL},
	{"swpatch",   doPhySwPatch,         0x02, 1, "swpatch <on|off>"},
	{"dispflag",    doPhyDispFlag,  0x02, 0,    NULL},
	{"Skewdispflag",    doSkewDispFlag,  0x02, 0,    NULL},
#endif // TCPHY_DEBUG
#ifdef TCPHY_FIELD_DBGTEST
	{"dbgtest",  doFieldDebugTest, 0x02, 0, NULL},      
	{"dbgprint", doFieldDebugPrint, 0x02, 0, NULL},
#endif // TCPHY_FIELD_DBGTEST   
	{NULL,NULL,0,0,NULL},
};

#endif //TCPHY_SUPPORT


/*** public, Called by tc3162l2mac.c ***/
#ifdef TCPHY_SUPPORT

// set tcPhyFlag & tcPhyVer
int tcPhyVerLookUp(macAdapter_t *mac_p){
	uint32 rval;
	uint16 phyAddr;

	phyAddr = 0;
	tcPhyFlag = 1;
#ifdef PHYPART_DEBUG
	printf("PhyPart debug: tcPhyVerLookUp() in	\r\n");
#endif

	rval = miiStationRead(mac_p, 3); // phy revision id
	if(rval == 0xffff)
	{
	rval = tcMiiStationRead_ext(phyAddr, 3); // phy revision id
	}
	else
	{
		fgMT7530_INT=1;
	}
	
	rval &= 0xffff;
	
	//eco_rev = tcMiiStationRead(phyAddr, 31); //CML_20130226_2
	//eco_rev &= (0x0f); //CML_20130226_2
	
	if (rval==0x9400){
		tcPhyVer=tcPhyVer_2031;         
		tcPhyPortNum=1;
	TCPHYDISP1("TC2031, ");
//      #if ( defined(TC2031_DEBUG) && !defined(ZYXEL_LEH) )
//          printf("TC2031_%s, ", tc2031_cfg[current_idx].name);
//      #endif
    } else if (rval==0x9401){
        tcPhyVer=tcPhyVer_2101mb;
        tcPhyPortNum=1;
        printf("TC2101MB, ");
    } else if (rval==0x9402){
        tcPhyVer=tcPhyVer_2104mc;
        tcPhyPortNum=4;
        printf("TC2104MC, ");
    } else if (rval==0x9403){
        tcPhyVer=tcPhyVer_2104sd;
        tcPhyPortNum=4;
        printf("TC2104SD, ");
    } else if (rval==0x9404){
        tcPhyVer=tcPhyVer_2101me;
        tcPhyPortNum=1;
        printf("TC2101ME, ");
	 } else if (rval==0x9405){
        tcPhyVer=tcPhyVer_2102me;
        tcPhyPortNum=1;
        printf("TC2102ME, ");	
	 } else if (rval==0x9406){
        tcPhyVer=tcPhyVer_2104me;
        tcPhyPortNum=4;
        printf("TC2104ME, ");
	} else if (rval==0x9407){
        tcPhyVer=tcPhyVer_2101mf;
        tcPhyPortNum=1;
        printf("TC2101MF, ");
    } else if (rval==0x9408){	
        tcPhyVer=tcPhyVer_2105sg;
        tcPhyPortNum=5;
        printf("TC2105SG, ");
	} else if (rval==0x940a){	
        tcPhyVer=tcPhyVer_2101mi;
        tcPhyPortNum=1;
        printf("TC2101MI, ");	
   } else if (rval==0x940b){	
        tcPhyVer=tcPhyVer_2105mj;
        tcPhyPortNum=5;
        printf("TC2105MJ, "); 
   } else if (rval==0x940c){
        tcPhyVer=tcPhyVer_2105sk;
        tcPhyPortNum=5;
        printf("TC2105SK, "); 	
    //} else if (rval==0x0440){	
    } else if (rval==0x9412){ //CML_20130219, E2 PHY ID
        tcPhyVer=mtPhyVer_7530;
        tcPhyPortNum=5;
        printf("MT7530E2,Internal check flag: fgMT7530_INT=0x%x \n",fgMT7530_INT); 
    } 	
#ifdef TCPHY_DEBUG
    else {
        printf("unknown PHYID: %lx, ", rval);
    }
#endif
	if (tcPhyVer == mtPhyVer_7530){ //CML_20130226_2
		eco_rev = (mtEMiiRegRead(phyAddr, 0x1f, 0) >> 4);
	}
	else{
		eco_rev = tcMiiStationRead(phyAddr, 31);
	}
	eco_rev &= (0x0f);

#ifdef PHYPART_DEBUG
		printf("PhyPart debug: tcPhyVerLookUp() out	\r\n");
#endif

    return 0;
}

// tcPhy initial: reset, load default register setting, restat AN
int tcPhyInit(macAdapter_t * mac_p){
    int i=0;
    uint16 start_addr= mac_p->enetPhyAddr;
	uint32 reg_value; //CML_20130226_1


#ifdef PHYPART_DEBUG
		printf("PhyPart debug: tcPhyInit() in  \r\n");
#endif

    //miiStationWrite( mac_p, PHY_CONTROL_REG, PHY_RESET );
    for( i=start_addr; i<start_addr+tcPhyPortNum; i++ ){            
        mtMiiRegWrite(i, PHY_CONTROL_REG, PHY_RESET );
    }
    
//tcPhyVer = mtPhyVer_7530;

    switch (tcPhyVer) {
#ifdef TC2031_SUPPORT           
        case tcPhyVer_2031: // LEH
            tc2031CfgLoad(0); 
            tc2031LdpsSet(config_ldps);
            break;
#endif
#ifdef TC2101MB_SUPPORT     
        case tcPhyVer_2101mb: // LEM
            tc2101mbCfgLoad(0);
            break;
#endif
#ifdef TC2104MC_SUPPORT     
        case tcPhyVer_2104mc: // 2206
            tc2104mcCfgLoad(0, DO_4_PORT, 0);
            break;
#endif
#ifdef TC2104SD_SUPPORT     
        case tcPhyVer_2104sd: // 2104sd
            tc2104sdCfgLoad(0, DO_4_PORT, 0);
            break;
#endif
#ifdef TC2101ME_SUPPORT     
        case tcPhyVer_2101me: // 62U
            tc2101meCfgLoad(0);
            break;
#endif      
#ifdef TC2102ME_SUPPORT 	
		case tcPhyVer_2102me: // 3182
			tc2102meCfgLoad(0);
			break;
#endif
#ifdef TC2104ME_SUPPORT     
	    case tcPhyVer_2104me: // 2206F
			tc2104meCfgLoad(0, DO_4_PORT, 0);
			break;
#endif 
#ifdef TC2101MF_SUPPORT     
	    case tcPhyVer_2101mf: // FPGA
			tc2101mfCfgLoad(0);
			break;
#endif 
#ifdef TC2105SG_SUPPORT     
		case tcPhyVer_2105sg: // TC6501
			tc2105sgCfgLoad(0, DO_4_PORT, 0);
		    break;
#endif 
#ifdef TC2101MI_SUPPORT     
		case tcPhyVer_2101mi: // TC63260
			tc2101miCfgLoad(0);
			break;
#endif
#ifdef TC2105MJ_SUPPORT     
		case tcPhyVer_2105mj: // RT63365
			tc2105mjCfgLoad(0, DO_4_PORT, 0);
			break;
#endif
#ifdef TC2105SK_SUPPORT     
		case tcPhyVer_2105sk: // TC6508
			tc2105skCfgLoad(0, DO_4_PORT, 0);
			break;
#endif
#ifdef MT7530_SUPPORT     
		case mtPhyVer_7530: // MT7530
			mt7530GePhyCfgLoad(0, DO_5_PORT, 0);
			break;
#endif

        //default:
    }

    // always boot-up with AN-enable
    //miiStationWrite( mac_p, PHY_CONTROL_REG, MIIDR_AUTO_NEGOTIATE );
    for( i=start_addr; i<start_addr+tcPhyPortNum; i++ ){            
		// rewrite to avoid changing H/W default setting //CML_20130226_1
		//mtMiiRegWrite(i, PHY_CONTROL_REG, MIIDR_AUTO_NEGOTIATE );
		reg_value = mtMiiRegRead(i, PHY_CONTROL_REG);
		reg_value |= MIIDR_AUTO_NEGOTIATE;
        mtMiiRegWrite(i, PHY_CONTROL_REG, reg_value );
    }

    // tcphy_link_state init.
    for(i=0;i<TCPHY_PORTNUM;i++){
        tcphy_link_state[i]=ST_LINK_DOWN;
    }
#ifdef PHYPART_DEBUG
	printf("PhyPart debug: tcPhyInit() out  \r\n");
#endif

    return 0;
}

// tcPhy initial: reset, load default register setting, restat AN
int tcPhyPortInit(uint8 port_num){
	uint32 reg_value; //CML_20130226_1
	
    //miiStationWrite( mac_p, PHY_CONTROL_REG, PHY_RESET );
    #ifndef LINUX_OS
    tcMiiStationWrite(port_num, PHY_CONTROL_REG, PHY_RESET );
	#endif

    switch (tcPhyVer) {
#ifdef TC2031_SUPPORT           
        case tcPhyVer_2031: // LEH
            tc2031CfgLoad(0); 
            tc2031LdpsSet(config_ldps);
            break;
#endif
#ifdef TC2101MB_SUPPORT     
        case tcPhyVer_2101mb: // LEM
            tc2101mbCfgLoad(0);
            break;
#endif
#ifdef TC2104MC_SUPPORT     
        case tcPhyVer_2104mc: // 2206
            tc2104mcCfgLoad(0, DO_PER_PORT, port_num);
            break;
#endif
#ifdef TC2104SD_SUPPORT     
        case tcPhyVer_2104sd: // 2104sd
            tc2104sdCfgLoad(0, DO_PER_PORT, port_num);
            break;
#endif
#ifdef TC2101ME_SUPPORT     
        case tcPhyVer_2101me: // 62U
            tc2101meCfgLoad(0);
            break;
#endif 
#ifdef TC2102ME_SUPPORT 	
		case tcPhyVer_2102me: // 3182
			tc2102meCfgLoad(0);
			break;
#endif
#ifdef TC2104ME_SUPPORT     
		case tcPhyVer_2104me: // 2206F
		tc2104meCfgLoad(0, DO_PER_PORT, port_num);
		break;
#endif
#ifdef TC2105SG_SUPPORT     
		case tcPhyVer_2105sg: // TC6501
		tc2105sgCfgLoad(0, DO_PER_PORT, port_num);
		break;
#endif
#ifdef TC2101MI_SUPPORT     
		case tcPhyVer_2101mi: // RT63260
		tc2101miCfgLoad(0);
		break;
#endif
#ifdef TC2105MJ_SUPPORT     
		case tcPhyVer_2105mj: // RT63365
		tc2105mjCfgLoad(0, DO_PER_PORT, port_num);
		break;
#endif
#ifdef TC2105SK_SUPPORT     
		case tcPhyVer_2105sk: // TC6508
		tc2105skCfgLoad(0, DO_PER_PORT, port_num);
		break;
#endif

        //default:
    }

    // always boot-up with AN-enable
    //miiStationWrite( mac_p, PHY_CONTROL_REG, MIIDR_AUTO_NEGOTIATE );           
	// rewrite to avoid changing H/W default setting //CML_20130226_1
	//tcMiiStationWrite(port_num, PHY_CONTROL_REG, MIIDR_AUTO_NEGOTIATE );
	reg_value = tcMiiStationRead(port_num, PHY_CONTROL_REG);
	reg_value |= MIIDR_AUTO_NEGOTIATE;
	tcMiiStationWrite(port_num, PHY_CONTROL_REG, reg_value );
	


    // tcphy_link_state init.
    tcphy_link_state[port_num]=ST_LINK_DOWN;
    return 0;
}


// return LP0Reg1 PHY Status Registers Value
uint8 getTcPhyFlag(void){
    return tcPhyFlag;
}

uint32 getTcPhyStatusReg(macAdapter_t * mac_p){
    // no called if (force_link_flag==1)

#ifdef PHYPART_DEBUG
	printf("PhyPart debug: getTcPhyStatusReg() in \r\n");
#endif
    if (!sw_patch_flag)//if sw patch off, mac get PHY status via MDIO directly.
    {
#ifdef PHYPART_DEBUG
		printf("PhyPart debug: getTcPhyStatusReg() out \r\n");
#endif
		return ( miiStationRead(mac_p, PHY_STATUS_REG) );
    }
    else//if sw patch on, mac get the PHY status that patch intercept. 
    {
#ifdef PHYPART_DEBUG
		printf("PhyPart debug: getTcPhyStatusReg() out \r\n");
#endif
		return (Nmr1[0].value); //mr1_link_status_reg;
    }
}

uint32 getTcPhyRMCAPReg(macAdapter_t * mac_p){
	#ifdef TC2101MI_SUPPORT
	if ((tcPhyVer==tcPhyVer_2101mi) && sw_patch_flag)
		return tc2101mi_reg5_val;
	else
	#endif
		return ( miiStationRead(mac_p, PHY_REMOTE_CAP_REG));	
}

uint16 getTcPhyNStatusReg(uint8 port_num){
    uint16 phyAddr;

    if (!sw_patch_flag) {
        phyAddr = mac_p->enetPhyAddr + port_num;
        return ( tcMiiStationRead(phyAddr, PHY_STATUS_REG) ); 
    }
    else {
        return (Nmr1[port_num].value);
    }
}

bool tcPhyExtResetNeeded(void){
	uint8 pn;
	switch (tcPhyVer) {
		#ifdef TC2104MC_SUPPORT
		       case tcPhyVer_2104mc: // 2206
		       tc2104mc_reset_needed_flag = tc2104mcLinkFailDetect();
	           return tc2104mc_reset_needed_flag;
		       break;
	    #endif
		#ifdef TC2104ME_SUPPORT
		       case tcPhyVer_2104me: // 2206F
               for(pn=0;pn<TCPHY_PORTNUM;pn++){
			   	tc2104me_reset_needed_perport[pn]= tc2104meLinkFailDetect(pn,1);//detect 100BT only
               	}
			   tc2104me_reset_needed_flag = (tc2104me_reset_needed_perport[0]
			   	                            ||tc2104me_reset_needed_perport[1]
			   	                            ||tc2104me_reset_needed_perport[2]
			   	                            ||tc2104me_reset_needed_perport[3]);
			   return tc2104me_reset_needed_flag;
		       break;
		#endif
		}
		return 1; 
}


// to check whether the values of TcPhy init. registers are changed or not
// reset PHY if the values are changed to pass ESD test
bool getTcPhyInitialRegFlag(void){
    if (!sw_patch_flag)
        return 1;

    switch (tcPhyVer) {
#ifdef TC2104MC_SUPPORT     
        case tcPhyVer_2104mc: // 2206
            if (tc2104mc_esdphy_init_flag)
				return (tc2104mcInitialRegFlag());
			else
				return 1;
            break;
#endif
#ifdef TCPHY_DEBUG
#ifdef TC2104ME_SUPPORT
        case tcPhyVer_2104me: // 2206F/22055F
            if (tc2104me_esdphy_init_flag)
				return (tc2104meInitialRegFlag());
			else
				return 1;
            break;
#endif
#endif
    }
    return 1;
}

bool getTcPhyEsdDetectFlag(void){
if ((!getTcPhyInitialRegFlag())||(tcPhyExtResetNeeded()))
	return 1;
else
	return 0;
}
	
// Software Patch for TrendChip's ethernet PHY
int tcPhySwPatch(void){
    if (!sw_patch_flag)
    {
       Skew_update_flag=0;
       return 0;
    }
#ifdef PHYPART_DEBUG_SW_PATCH
	printf("PhyPart debug: tcPhySwPatch() in  \r\n");
#endif
    switch (tcPhyVer) {
#ifdef TC2031_SUPPORT           
        case tcPhyVer_2031: // LEH
            tc2031SwPatch(); 
            break;
#endif
#ifdef TC2101MB_SUPPORT     
        case tcPhyVer_2101mb: // LEM
            tc2101mbSwPatch();
            break;
#endif
#ifdef TC2104MC_SUPPORT     
        case tcPhyVer_2104mc: // 2206
            tc2104mcSwPatch();
            break;
#endif
#ifdef TC2104SD_SUPPORT     
        case tcPhyVer_2104sd: // 2104sd
            tc2104sdSwPatch();
            break;
#endif
#ifdef TC2101ME_SUPPORT     
        case tcPhyVer_2101me: // 62U
            tc2101meSwPatch();
            break;
#endif  
#ifdef TC2102ME_SUPPORT 	
		case tcPhyVer_2102me: // 3182
			tc2102meSwPatch();
			break;
#endif	
#ifdef TC2104ME_SUPPORT     
		case tcPhyVer_2104me: // 2206F
		    tc2104meSwPatch();
			break;
#endif
#ifdef TC2101MF_SUPPORT     
		case tcPhyVer_2101mf: // FPGA
		    tc2101mfSwPatch();
			break;
#endif 
#ifdef TC2105SG_SUPPORT     
		case tcPhyVer_2105sg: // YC6501
			tc2105sgSwPatch();
			break;
#endif
#ifdef TC2101MI_SUPPORT     
		case tcPhyVer_2101mi: // 63260
			tc2101miSwPatch();
			tc2101miLchRmCapReg();
			break;
#endif
#ifdef TC2105MJ_SUPPORT     
		case tcPhyVer_2105mj: // 63365
			tc2105mjSwPatch();
			break;
#endif
#ifdef TC2105SK_SUPPORT     
		case tcPhyVer_2105sk: // TC6508
		    tc2105skSwPatch();
			break;
#endif
#ifdef MT7530_SUPPORT     
    case mtPhyVer_7530: 
       mt7530GePhySwPatch();
      break;
#endif

        //default: // null
    }
#ifdef PHYPART_DEBUG_SW_PATCH
		printf("PhyPart debug: tcPhySwPatch() out  \r\n");
#endif
    return 0;
}

// CI command for tcephydbg
int 
doEtherPhydbg(int argc, char *argv[], void *p) {  
    return subcmd(ethertphyprint,argc,argv,p);
} 
#ifdef LINUX_OS
/*_____________________________________________________________________________
**      function name: tcephydbgcmd
**      descriptions:
**           register rootcommand of "ether ". 
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
void 
tcephydbgcmd(void)
{
	cmds_t tcephydbg_cmd;

	tcephydbg_cmd.name= "tce";
	tcephydbg_cmd.func=doEtherPhydbg;
	tcephydbg_cmd.flags=0x12;
	tcephydbg_cmd.argcmin=0;
	tcephydbg_cmd.argc_errmsg=NULL;
	cmd_register(&tcephydbg_cmd);
}/*end tcephydbgcmd*/

void 
tcephydbgcmd1(void)
{
	cmds_t tcephydbg_cmd;

	tcephydbg_cmd.name= "tce1";
	tcephydbg_cmd.func=doEtherPhydbg;
	tcephydbg_cmd.flags=0x12;
	tcephydbg_cmd.argcmin=0;
	tcephydbg_cmd.argc_errmsg=NULL;
	cmd_register(&tcephydbg_cmd);
}/*end tcephydbgcmd*/
#endif
#endif //TCPHY_SUPPORT

#ifdef TCPHY_DEBUG
uint8 getTcPhyLookbackFlag(void){
    return phy_loopback_flag;
}
uint8 getTcPhyForceLinkFlag(void){
    return force_link_flag;
}
#endif //TCPHY_DEBUG

/************************************************************************
*                       Functions Body for TCPHY_SUPPORT CI commands
*************************************************************************
*/

#ifdef TCPHY_SUPPORT
static int
	doPhySwVer(int argc, char *argv[], void *p){
	printf("Ver : ");

	switch (tcPhyVer) {
	#ifdef TC2031_SUPPORT           
		case tcPhyVer_2031: // LEH
		printf("%s ",tc2031_cfg[0].name);
		break;
	#endif
	#ifdef TC2101MB_SUPPORT     
		case tcPhyVer_2101mb: // LEM
		printf("%s ",tc2101mb_cfg[0].name);
		break;
	#endif
	#ifdef TC2104MC_SUPPORT     
		case tcPhyVer_2104mc: // 2206
		printf("%s ",tc2104mc_cfg[0].name);
		printf(" (%d)",eco_rev);
		break;
	#endif
	#ifdef TC2104SD_SUPPORT     
		case tcPhyVer_2104sd: // 2104sd
		printf("%s ",tc2104sd_cfg[0].name);
		break;
		#endif
	#ifdef TC2101ME_SUPPORT     
		case tcPhyVer_2101me: // 62UE
		printf("%s ",tc2101me_cfg[0].name);
		break;
	#endif     
	#ifdef TC2102ME_SUPPORT 	
		case tcPhyVer_2102me: // 3182
		printf("%s ",tc2102me_cfg[0].name);
		break;
	#endif
	#ifdef TC2104ME_SUPPORT     
		case tcPhyVer_2104me: // 2206F
		printf("%s ",tc2104me_cfg[0].name);
		printf(" (%d)\r\n",eco_rev);
		tc2104meCfgCheck();
		break;
	#endif
	#ifdef TC2101MF_SUPPORT     
		case tcPhyVer_2101mf: // FPGA
		printf("%s ",tc2101mf_cfg[0].name);
		break;
	#endif  
	#ifdef TC2105SG_SUPPORT     
		case tcPhyVer_2105sg: // TC6501
		printf("%s ",tc2105sg_cfg[0].name);
		break;
	#endif
	#ifdef TC2101MI_SUPPORT     
		case tcPhyVer_2101mi: // RT63260
		printf("%s ",tc2101mi_cfg[0].name);
		break;
	#endif
	#ifdef TC2105MJ_SUPPORT     
		case tcPhyVer_2105mj: // RT63365
		printf("%s ",tc2105mj_cfg[0].name);
		printf(" (%d)\r\n",eco_rev);
		tc2105mjCfgCheck();
	break;
	#endif
	#ifdef TC2105SK_SUPPORT     
		case tcPhyVer_2105sk: // TC6508
		 printf("%s ",tc2105sk_cfg[0].name);
		break;
	#endif
	//#ifdef TC2105SK_SUPPORT     
	#ifdef MT7530_SUPPORT
		case mtPhyVer_7530: // TC6508
		 printf("%s \r\n",mt7530_cfg[0].name );
		 printf("mtPhyVer_7530,tcPhyVer=0x%02lX \r\n",tcPhyVer);
		break;
	#endif

	default: 
	    printf("Unknown PHY ,tcPhyVer=0x%02lX \r\n",tcPhyVer);
	}

		printf(": 2013-04-01\r\n");
		return 0;
} 

uint16
getMiiPage (char *page)
{
    // g0,g1,g2,g3 = 0x0000~0x3000
    // l0,l1,l2,l3 = 0x8000~0xb000
	if( stricmp(page, "g0") == 0 ){
	    return (0x0000);
	}
	else if( stricmp(page, "g1") == 0 ){
	    return (0x1000);
	}
	else if( stricmp(page, "g2") == 0 ){
	    return (0x2000);
	}
	else if( stricmp(page, "g3") == 0 ){
	    return (0x3000);
	}
	else if( stricmp(page, "g4") == 0 ){
	    return (0x4000);
	}
	else if( stricmp(page, "l0") == 0 ){
	    return (0x8000);
	}
	else if( stricmp(page, "l1") == 0 ){
	    return (0x9000);
	}
	else if( stricmp(page, "l2") == 0 ){
	    return (0xa000);
	}
	else if( stricmp(page, "l3") == 0 ){
	    return (0xb000);
	}
	else if( stricmp(page, "ext") == 0 ){//Extended Page Registers
	    return (0x0001);
	}
	else if( stricmp(page, "misc") == 0 ){//Misc Page Registers
	    return (0x0002);
	}
	else if( stricmp(page, "lpi") == 0 ){//LPI Page Registers
	    return (0x0003);
	}
	else if( stricmp(page, "test") == 0 ){//Test Page Registers
	    return (0x2a30);
	}
	else if( stricmp(page, "tr") == 0 ){//Token Ring Debug Registers
	return (0x52b5);
	}
	else {
	    printf("* Wrong PageNo(%s).\r\n",page);
	    printf("* MT7530 PageNo=><g0|ext|misc|lpi|test|tr> \r\n");
	    printf("* Tcxxx PageNo=><g0|g1|g2|g3|g4|l0|l1|l2|l3> \r\n");
	    return (0xFFFF);
    }
}

uint16
checked_atoi(char *val)
{
#ifdef TCPHY_DEBUG
    // only check 1st char
    if (val[0]<'0' || val[0]>'9')
        return (0xffff);
#endif
    return (atoi(val));
}


int
doPhyMiiRead (int argc, char *argv[], void *p)
{
    uint16 phyaddr=0;
    const uint16 page_reg=31;
    uint16 page_val=0;
    uint32 reg=0;
    uint32 value;

    int i;

    //  argc:3
    //  tce miir all <PhyAddr> 
    //  tce miir <PhyAddr> <RegAddr>
    //  argc:4
    //  tce miir all <PhyAddr> <PageNo>
    //  tce miir <PhyAddr> <PageNo> <RegAddr>

    // get parameters
    if (argc==3){
        if (stricmp(argv[1], "all") == 0){ // tce miir all <PhyAddr> 
            phyaddr = checked_atoi(argv[2]);
        }
        else{ // tce miir <PhyAddr> <RegAddr>
            phyaddr = checked_atoi(argv[1]);
           // reg = checked_atoi(argv[2]);
           sscanf(argv[2], "%lx", &reg);
		   
		   //printf("*1: PhyAddr=%d, RegAddr=0x%02lX\r\n", phyaddr, reg);	   
        }
    }
    else if (argc==4){
        if (stricmp(argv[1], "all") == 0){ // tce miir all <PhyAddr> <PageNo>
            phyaddr = checked_atoi(argv[2]);
            page_val = getMiiPage(argv[3]);
        }
        else{ // tce miir <PhyAddr> <PageNo> <RegAddr>
            phyaddr = checked_atoi(argv[1]);
            page_val = getMiiPage(argv[2]);
            //reg = checked_atoi(argv[3]);
            sscanf(argv[3], "%lX", &reg);
        }
    }

    if(reg != 31)
      mtMiiRegWrite(phyaddr, page_reg, 0);
    // check parameters
    if ((argc==3 || argc==4) 
        && phyaddr<=31 && reg<=31 && page_val!=0xffff){
        // set page
        if (argc==4){           
            if (stricmp(argv[1], "all") == 0){ // multiple read
                printf("* PageNo=%s ",argv[3]);
                printf("\r\n");             
            }
            else{
                printf("* PageNo=%s ",argv[2]);
            }
            mtMiiRegWrite(phyaddr, page_reg, page_val);
        }
        // read data
        if (stricmp(argv[1], "all") == 0){ // multiple read
            for( i=0; i<32; i++ ){
                value = mtMiiRegRead(phyaddr, i);
                printf("[reg=0x%02lX val=%04lX]", i, value);
                if( (i+1) % 4 == 0 )
                    printf("\r\n");             
            }
        }
        else{
            value = mtMiiRegRead(phyaddr, reg);
            printf("* PhyAddr=%d RegAddr=0x%02lX value=0x%04lX\r\n", phyaddr, reg, value);
        }       
        return 0;           
    }   
    else { // error message
        printf("Usage: miir all <PhyAddr> [PageNo]\r\n");
        printf("       miir <PhyAddr> <RegAddr>\r\n");
        printf("       miir <PhyAddr> <PageNo> <RegAddr>\r\n");
        return 0;           
    }
}

int
doPhyMiiWrite (int argc, char *argv[], void *p)
{
	uint16 phyaddr=0;
	const uint16 page_reg=31;
	uint16 page_val=0;
	uint32 reg=0;
	uint32 value=0;
	uint16 STBit=0;
	uint16 BFlen=0;
	uint16 BF,BFMsk=0;
	uint32 RValue =0;
	int i;

    //  tce miir <PhyAddr> <RegAddr> <Value>
    //  tce miir <PhyAddr> <PageNo> <RegAddr> <Value>

    // get parameters
    /*	
	else*/
    if (argc==4){
	phyaddr = checked_atoi(argv[1]);
	//reg = checked_atoi(argv[2]);
	sscanf(argv[2], "%lx", &reg);
	sscanf(argv[3], "%lx", &value);
	//printf("*1: argv[2]=%s, argv[3]=%s\r\n", argv[2], argv[3]);		
	//printf("*2: phyaddr=%d, Reg=0x%02lX, value=0x%04lX\r\n", phyaddr, reg , value);		
    }
    else if (argc==5){
        phyaddr = checked_atoi(argv[1]);
        page_val = getMiiPage(argv[2]);
        //reg = checked_atoi(argv[3]);
        sscanf(argv[3], "%lx", &reg);
        sscanf(argv[4], "%lx", &value);
    }
	else if (argc==6){
		if(stricmp(argv[0], "miiwb") == 0){
			phyaddr = checked_atoi(argv[1]);
			//reg = checked_atoi(argv[2]);
			sscanf(argv[2], "%lx", &reg);
			STBit = checked_atoi(argv[3]);
			BFlen = checked_atoi(argv[4]);
			sscanf(argv[5], "%lx", &value);
			printf("* Phyaddr=%d, RegAddr=0x%02lX, STBit=%0d, BFlen=%0d, value=0x%04lX\r\n", phyaddr, reg, STBit, BFlen, value);		
			
			}
		}
	else if (argc==7){
		if(stricmp(argv[0], "miiwb") == 0){
			phyaddr = checked_atoi(argv[1]);
			page_val = getMiiPage(argv[2]);
			//reg = checked_atoi(argv[3]);
			sscanf(argv[3], "%lx", &reg);
			STBit = checked_atoi(argv[4]);
			BFlen = checked_atoi(argv[5]);
			sscanf(argv[6], "%lx", &value);
			printf("* Phyaddr=%d, pageNo=%d, RegAddr=0x%02lX, STBit=%0d, BFlen=%0d, value=0x%04lX\r\n", phyaddr, page_val, reg, STBit, BFlen, value);		
			
			}
		}
		
 

    // check parameters and write
    
    if(reg != 31)
      mtMiiRegWrite(phyaddr, page_reg, 0);
    
    if ((argc==4 || argc==5) 
        && (phyaddr<=31) && (reg<=31) && (page_val!=0xffff)){
        // set page
        if (argc==5) {
            printf("* PageNo=%s ",argv[2]);
            mtMiiRegWrite(phyaddr, page_reg, page_val);
        }
        // write data
        printf("* Phyaddr=%d RegAddr=0x%02lX value=0x%04lX\r\n", phyaddr, reg, value);
        mtMiiRegWrite(phyaddr, reg, value);
    }
 	else if (((argc == 6) || (argc == 7))
		&& (phyaddr<=31) && (reg<=31) && (page_val != 0xffff)){
		
        if (argc==7) {
            printf("* PageNo=%s ",argv[2]);
            mtMiiRegWrite(phyaddr, page_reg, page_val);
        }
		
		if(stricmp(argv[0], "miiwb") == 0){
		 RValue = mtMiiRegRead(phyaddr, reg);
		 for(i=0;i<BFlen;i++){
			 BF=1;
			 BF=BF<<(STBit+i);
			 BFMsk = BFMsk | BF;
			  //printf("1: BF=%d, BFMsk=%04x\r\n", BF, BFMsk);
			 }
				BFMsk = ~BFMsk;
				//printf("2: BF=%d, BFMsk=%04x\r\n", BF, BFMsk); 
				value = (RValue & BFMsk) | (value<<STBit);
				//printf("* Phyaddr=%d, RegAddr=0x%02lX, OrgValue=0x%04lX, Modified value=0x%04lX\r\n", phyaddr, reg,  RValue, value);

				printf("* OrgValue=0x%04lX,",RValue);
				mtMiiRegWrite(phyaddr, reg, value);
				RValue = mtMiiRegRead(phyaddr, reg);
				printf(" ModValue=0x%04lX\r\n", RValue);
			}
    }
    else { // error message
	printf("Usage: miiw <PhyAddr> <RegAddr> <RegVal>\r\n");
	printf("       miiw <PhyAddr> <PageNo> <RegAddr> <RegVal>\r\n");
	printf("       miiwb <PhyAddr> <RegAddr> <STBit> <BFLen> <BFVal> \r\n");	
	printf("       miiwb <PhyAddr> <PageNo> <RegAddr> <STBit> <BFLen> <BFVal> \r\n");	
	return 0;
    }
    return 0;
}
#ifdef TCPHY_1PORT
static int
doPhySpeed (int argc, char *argv[], void *p)
{
    uint8 port_num = 0;

    if( argc != 2 ) {
        printf("Usage: Speed <Auto|AN|100F|100H|10F|10H|Disp>\r\n");
    }   
    else {
        if ( stricmp(argv[1], "Auto") == 0 ) {
            tcphy_speed = tcphy_speed_Auto;
            tcPhyWriteReg(port_num,0,0x1200);
        }
        else if ( stricmp(argv[1], "AN") == 0 ){
            tcphy_speed = tcphy_speed_ForceAN;
            tcPhyWriteReg(port_num,0,0x1200);
        }
        else if ( stricmp(argv[1], "100F") == 0 ) {
            tcphy_speed = tcphy_speed_Force100F;
            tcPhyWriteReg(port_num,0,0x2100);
        }
        else if ( stricmp(argv[1], "100H") == 0 ){
            tcphy_speed = tcphy_speed_Force100H;
            tcPhyWriteReg(port_num,0,0x2000);
        }
        else if ( stricmp(argv[1], "10F") == 0 ) {
            tcphy_speed = tcphy_speed_Force10F;
            tcPhyWriteReg(port_num,0,0x0100);
        }
        else if ( stricmp(argv[1], "10H") == 0 ){
            tcphy_speed = tcphy_speed_Force10H;
            tcPhyWriteReg(port_num,0,0x0000);
        }
        else {
            printf("Current Speed mode: %s.\r\n",
                    (tcphy_speed==tcphy_speed_Auto)?"Auto":
                    (tcphy_speed==tcphy_speed_ForceAN)?"ForceAN":
                    (tcphy_speed==tcphy_speed_Force100F)?"Force100F":
                    (tcphy_speed==tcphy_speed_Force100H)?"Force100H":
                    (tcphy_speed==tcphy_speed_Force10F)?"Force10F":
                    (tcphy_speed==tcphy_speed_Force10H)?"Force10H":
                                                        "Unknown");
        }
    }
    return 0;
}
#endif

static int 
doPhyConfig (int argc, char *argv[], void *p)
{

#ifdef TCPHY_DEBUG
    if (argc==1) {
        TCPHYDISP3("Usage: config SwPatch [on|off]\r\n");
        TCPHYDISP3("       config DispLevel [0..6]\r\n");
        TCPHYDISP3("       config ErrOverMonitor [on|off]\r\n");
    }
    else if (argc==2) {
        if (stricmp(argv[1], "list") == 0){
            TCPHYDISP3("config SwPatch : %s\r\n",
                    (sw_patch_flag?"on":"off"));            
            TCPHYDISP3("config DispLevel : %d\r\n",
                    (tcPhy_disp_level));            
            TCPHYDISP3("config ErrOverMonitor : %s\r\n",
                    (sw_ErrOverMonitor_flag?"on":"off"));
        }
    }
#endif

    if (argc==2 || argc==3){
        if (stricmp(argv[1], "SwPatch") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                sw_patch_flag = 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                sw_patch_flag = 0; // disable
            }
            else {
                TCPHYDISP2("config SwPatch : %s\r\n",
                        (sw_patch_flag?"on":"off"));
            }
            return 0; // Not call individual config function
        }
		#ifdef TCPHY_DEBUG
        else if (stricmp(argv[1], "DispLevel") == 0){
            if (argc==3) {
                tcPhy_disp_level = atoi(argv[2]);
            }
            TCPHYDISP2("config DispLevel : %d\r\n",tcPhy_disp_level);           
            return 0; // Not call individual config function
        }
        else if (stricmp(argv[1], "ErrOverMonitor") == 0){
            if( stricmp(argv[2], "on") == 0 ){
                sw_ErrOverMonitor_flag = 1; // enable
            }
            else if( stricmp(argv[2], "off") == 0 ){
                sw_ErrOverMonitor_flag = 0; // disable
            }
            else {
                TCPHYDISP2("config ErrOverMonitor : %s\r\n",
                        (sw_ErrOverMonitor_flag?"on":"off"));
            }
            return 0; // Not call individual config function
        }
		#endif
    }

    // if (no "return" above)
    switch (tcPhyVer) {
#ifdef TC2031_SUPPORT           
        case tcPhyVer_2031: // LEH
            tc2031Config(argc,argv,p);
            break;
#endif
#ifdef TC2101MB_SUPPORT     
        case tcPhyVer_2101mb: // LEM
            tc2101mbConfig(argc,argv,p);
            break;
#endif
#ifdef TC2104MC_SUPPORT     
        case tcPhyVer_2104mc: // 2206
            tc2104mcConfig(argc,argv,p);
            break;
#endif
#ifdef TC2104SD_SUPPORT     
        case tcPhyVer_2104sd: // 2104sd
            tc2104sdConfig(argc,argv,p);
            break;
#endif
#ifdef TC2101ME_SUPPORT     
        case tcPhyVer_2101me: // 62UE
            tc2101meConfig(argc,argv,p);
            break;
#endif    
#ifdef TC2102ME_SUPPORT 	
		case tcPhyVer_2102me: // 3182
			tc2102meConfig(argc,argv,p);
			break;
#endif
#ifdef TC2104ME_SUPPORT     
			case tcPhyVer_2104me: // tc2206F
			tc2104meConfig(argc,argv,p);
			break;
#endif   
#ifdef TC2101MF_SUPPORT     
			case tcPhyVer_2101mf: // FPGA(MF)
			tc2101mfConfig(argc,argv,p);
			break;
#endif  
#ifdef TC2105SG_SUPPORT     
				case tcPhyVer_2105sg: // TC6501
				tc2105sgConfig(argc,argv,p);
				break;
#endif 
#ifdef TC2101MI_SUPPORT     
					case tcPhyVer_2101mi: // RT63260
					tc2101miConfig(argc,argv,p);
					break;
#endif 
#ifdef TC2105MJ_SUPPORT     
				case tcPhyVer_2105mj: // RT63365
				  tc2105mjConfig(argc,argv,p);
				  break;
#endif 
#ifdef TC2105SK_SUPPORT     
				case tcPhyVer_2105sk: // TC6508
				tc2105skConfig(argc,argv,p);
				break;
#endif
    }
    return 0;
}

#ifdef TCPHY_DEBUG
static int
doMacRegDump (int argc, char *argv[], void *p)
{
#if defined(TC2104MC_SUPPORT) || defined(TC2104ME_SUPPORT)
    const uint8 phyAddr=22; // tc2206 mac
    uint8 pn;
    uint32 wval;
    uint32 tx_pkt_cnt;
    uint32 rx_pkt_cnt;
    uint32 rx_drop_cnt; 
    uint32 rx_crc_cnt;

    if (tcPhyVer==tcPhyVer_2104mc || tcPhyVer==tcPhyVer_2104me){     
        if( argc == 1 ) { // display MAC MIB counters
            printf("\r\nMacNo TxPktCnt RxPktCnt RxDropCnt RxCrcCnt");
            for(pn=0;pn<6;pn++){
                //if (pn==4) 
                //  continue;
                wval = (pn<<12)|0x0001;
                tcMiiStationWrite(phyAddr, 16, wval);
                rx_pkt_cnt=(tcMiiStationRead(phyAddr, 17)<<16); 
                rx_pkt_cnt+=tcMiiStationRead(phyAddr, 18); 
                rx_drop_cnt=(tcMiiStationRead(phyAddr, 21)<<16); 
                rx_drop_cnt+=tcMiiStationRead(phyAddr, 22); 
                rx_crc_cnt=(tcMiiStationRead(phyAddr, 23)<<16); 
                rx_crc_cnt+=tcMiiStationRead(phyAddr, 24); 
                tx_pkt_cnt=(tcMiiStationRead(phyAddr, 25)<<16); 
                tx_pkt_cnt+=tcMiiStationRead(phyAddr, 26); 
                printf("\r\n%5d ",pn);
                printf("%8ld ",tx_pkt_cnt);
                printf("%8ld ",rx_pkt_cnt);
                printf("%9ld ",rx_drop_cnt);
                printf("%8ld ",rx_crc_cnt);
            }
            printf("\r\n");
            return 0;
        }
    }
#endif

    // dump PHY-related MAC Reg if (non-tc2104mc || argc!=1)
    dbg_plinel_1("\r\nCR_MAC_RUNT_TLCC_CNT  ",VPint(CR_MAC_RUNT_TLCC_CNT)); // --- Receive Runt and Transmit Late Collision Packet Counter Register ---
    dbg_plinel_1("\r\nCR_MAC_RCRC_RLONG_CNT ",VPint(CR_MAC_RCRC_RLONG_CNT)); // --- Receive CRC and Long Packet Counter Register ---
    dbg_plinel_1("\r\nCR_MAC_RLOSS_RCOL_CNT ",VPint(CR_MAC_RLOSS_RCOL_CNT)); // --- Receive Packet Loss and Receive Collision Counter Register ---
    dbg_plinel_1("\r\nCR_MAC_BROADCAST_CNT  ",VPint(CR_MAC_BROADCAST_CNT)); // --- Receive Broadcast Counter Register ---
    dbg_plinel_1("\r\nCR_MAC_MULTICAST_CNT  ",VPint(CR_MAC_MULTICAST_CNT)); // --- Receive Multicast Counter Register ---
    dbg_plinel_1("\r\nCR_MAC_RX_CNT         ",VPint(CR_MAC_RX_CNT)); // --- Receive Good Packet Counter Register ---
    dbg_plinel_1("\r\nCR_MAC_TX_CNT         ",VPint(CR_MAC_TX_CNT)); // --- Transmit Good Packet Counter Register ---
    printf("\r\n");
    return 0;
}
#endif


#endif // TCPHY_SUPPORT

/************************************************************************
*                       Functions Body for TCPHY_DEBUG CI commands
*************************************************************************
*/

#if defined(TCPHY_DEBUG) || defined(MTPHY_DEBUG)
static int
doPhyInit(int argc, char *argv[], void *p)
{
    tcPhyInit(mac_p);
    return 0;
}
#endif

#if defined(TCPHY_DEBUG)|| defined(MTPHY_DEBUG)
static int
doPhySwPatch (int argc, char *argv[], void *p)
{
    if( argc != 2 ) {
        printf("Sw patch status: %s.\r\n", (sw_patch_flag?"on":"off"));
    }
    else if( stricmp(argv[1], "on") == 0 )  {
        sw_patch_flag = 1;      
        printf("Sw patch status: ON.\r\n");
    }
    else if( stricmp(argv[1], "off") == 0 ) {
        sw_patch_flag = 0;      
        printf("Sw patch status: OFF.\r\n");
    }           
    else {
        printf("Sw patch status: %s.\r\n", (sw_patch_flag?"on":"off"));
    }
    return 0;
}
#endif
#if defined(TCPHY_DEBUG)

static int
doPhyRegCheck (int argc, char *argv[], void *p)
{
    uint32 val, i;
    uint32 phyaddr;
    uint32 reg;
    uint32 loop;
    uint32 val_inc;
    volatile uint32 data;

    if( (argc != 5) && (argc != 6) )
    {
        printf("Usage: regcheck <PhyAddr> <PhyReg> <Value> <Loop> [ValueIncStep]\r\n");
        printf("Note:  turn off periodic mii access before test.\r\n");
        return 0;
    }
    phyaddr = atoi(argv[1]);
    reg = atoi(argv[2]);
//  val = atoi(argv[3]);
    sscanf(argv[3], "%lx", &val);

    loop = atoi(argv[4]);
	sscanf(argv[5], "%lx", &val_inc);

    for( i=0; i<loop; i++ )
    {
        if (argc==6)
        {
            val = (val+val_inc)&0xffff;
            tcMiiStationWrite(phyaddr,reg,val);
        }
        data = tcMiiStationRead(phyaddr, reg);
        if( data != val )
        {
            printf("\r\nError!! Phyaddr=%ld reg=%ld loop=%ld Expected=0x%lx Read=0x%lx\r\n", 
                phyaddr, reg, i, val, data);
            return 0;
        }
        if ((i%10000) == 0)
        {
            printf(".");
        }
    }
    printf("\r\nLoop=%ld Done!!\r\n", i);
    return 0;
}

// called by macRxRingProc() for doPhyLoopback()
#ifdef LINUX_OS
uint8 phyMbpChk(struct sk_buff* oldMbp){
#else
uint8 phyMbpChk(mbuf_t*oldMbp){
#endif
    if( phy_loopback_flag)
    {
        recv_ok_flag = 1;
        return 1;
    }
    if((oldMbp->data[12] == 8) && (oldMbp->data[13] == 0) ){ // ip packet
        if(oldMbp->data[34] == 0 && oldMbp->data[23] == 1)//Echo Ping Reply
        {
            if( oldMbp->data[50] == 'B' && oldMbp->data[51] == 'E' && 
                 oldMbp->data[52] == 'A' && oldMbp->data[53] == 'F' ){
                PingReplyCnt++;
                return 1;
            }
        }
    }   
    return 0;
}

// called by macRxRingProc() for doPhyLoopback()
#ifdef LINUX_OS
uint8 phy_recv_err_check(struct sk_buff** oldMbp, volatile macRxDescr_t *rxDescrp, uint32 *frameSize){
#ifdef TC2031_DEBUG
 #if  !(defined(CONFIG_MIPS_TC3182) ||  defined(CONFIG_MIPS_TC3262))
	int i = 0;
	if( phy_loopback_flag)
	{
		if (rxDescrp->rdes0.word & (1<<30)){
			*frameSize = rxDescrp->rdes0.bits.rfl;
			*frameSize = (*frameSize > 60) ? *frameSize : 60;
		}
		else{
			*frameSize = rxDescrp->rdes0.bits.rfl;
		}
		*oldMbp = (struct sk_buff *)(rxDescrp->skb);
		skb_put(*oldMbp, *frameSize);
		dbg_plinel_1("Received packet have CRC error! length=", *frameSize);
		dbg_pline_1("\r\n");
		for( i=0; i<*frameSize; i++ )
			dbg_plineb_1(" ", (*oldMbp)->data[i]);
		dbg_pline_1("\r\n");
		recv_err_flag = 1;						
	}
 #endif
#endif
	return 0;
}
#else
uint8 phy_recv_err_check(mbuf_t **oldMbp, macRxDescr_t *rxDescrp, uint32 *frameSize){
    #ifdef TC2031_DEBUG
    int i = 0;
    if( phy_loopback_flag)
    {
            if (rxDescrp->rdes0.word & (1<<30))
            {
                *frameSize = rxDescrp->rdes0.bits.rfl;
                *frameSize = (*frameSize > 60) ? *frameSize : 60;
            }
        else
            *frameSize = rxDescrp->rdes0.bits.rfl;                      
            (volatile mbuf_t *)(*oldMbp) = (volatile mbuf_t *)(rxDescrp->mbuf);
            (*oldMbp)->cnt = *frameSize;
        dbg_plinel_1("Received packet have CRC error! length=", *frameSize);
        dbg_pline_1("\r\n");
        for( i=0; i<*frameSize; i++ )
            dbg_plineb_1(" ", (*oldMbp)->data[i]);
        dbg_pline_1("\r\n");
        recv_err_flag = 1;                      
    }
    #endif
    return 0;
}
#endif
#ifndef LINUX_OS
static uint8 mac_head[14]={0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0, 0, 0, 0, 0, 1, 0xff, 0xff};

static int doPhyLoopback (int argc, char *argv[], void *p)
{
    uint32 val, old_val; 
    int mode = 0, full_mode = 0;
    uint32 i, j, state, loop;
    mbuf_t *mbp;
    int len=60;
    int *seq;
    uint8 random_num, send_fail_flag;
    
    if( (argc != 3) && (argc != 4) )
    {
        printf("Usage: loopback <mii|ext> <loopnum> [full]\r\n");
        return 0;
    }

    
    if( stricmp("mii", argv[1]) == 0 )
        mode = 1;
    //else if( stricmp("pma10", argv[1]) == 0 )
    //  mode = 2;
    //else if( stricmp("pma100", argv[1]) == 0 )
    //  mode = 3;   
    else if( stricmp("ext", argv[1]) == 0 )
        mode = 4;
    else
    {
        printf("Usage: loopback <mii|ext> <loopnum> <full>\r\n");
        return;
    }   

    if( argc == 4 )
    {
        if( stricmp("full", argv[3]) == 0 )
            full_mode = 1;
    }
    
    phy_loopback_flag = 1;  
    if( mode == 1 )
    {
        // set phy-reg 0 bit14 = 1
        val = tcMiiStationRead(mac_p->enetPhyAddr, 0);  
        val |= 0x4000;
        tcMiiStationWrite(mac_p->enetPhyAddr, 0, val);
    }
#if 0
    if( mode == 2 )
    {
        // force to 10M full
        old_val = tcMiiStationRead(mac_p->enetPhyAddr, 0);  
        val = 0x0100;
        tcMiiStationWrite(mac_p->enetPhyAddr, 0, val);

        // set phy-reg 26 bit[4:3] = 01
        val = tcMiiStationRead(mac_p->enetPhyAddr, 26);
        val |= 0x0008;
        tcMiiStationWrite(mac_p->enetPhyAddr, 26, val);
        
        delay1ms(500);  
    }
#endif
#if 0
    if( mode == 3 )
    {
        // force to 100M full
        old_val = tcMiiStationRead(mac_p->enetPhyAddr, 0);  
        val = 0x2100;
        tcMiiStationWrite(mac_p->enetPhyAddr, 0, val);

        // set phy-reg 26 bit[4:3] = 10
        val = tcMiiStationRead(mac_p->enetPhyAddr, 26);
        val |= 0x0010;
        tcMiiStationWrite(mac_p->enetPhyAddr, 26, val);
        
        delay1ms(500);      
    }
#endif
    if( mode == 1 || mode == 2 || mode == 3)
    {
        // set MAC to full-duplex mode
        val = VPint(CR_MAC_MACCR);
        val |= 0x00008000;
        VPint(CR_MAC_MACCR) = val;
    }
    loop = atoi(argv[2]);
    j=0;
    while (j < loop) {
        random_num = (uint8)rand();
        mbp = (mbuf_t *)alloc_cache_mbuf2k();
        if(mbp == NULL){
            continue;
        }
        // length is 60~1514
        len = 60+(j%1455)   ;
        memcpy(mbp->data, mac_head, sizeof(mac_head));
        mbp->data[14] = (uint8)((j>>8)&0x00ff);     
        mbp->data[15] = (uint8)(j&0x00ff);      
        for(i = 16; i < len; i++){  /* fill pattern */
            mbp->data[i] = (uint8)((random_num+i-16)&0xff);
        }

//      for( i=0; i<len; i++ )
//          dbg_plineb_1(" ", mbp->data[i]);
//      dbg_pline_1("\r\n");

        mbp->cnt = len;

            
        state = dirps();
        
        send_fail_flag = 0;
        
        if(macSend( 0,  mbp) == 0)
            j++;
        else
            send_fail_flag = 1; 
        
        restore(state);
        
        if( !full_mode )
        {
            timeout_cnt = 0;
            while(1)
            {
                if( recv_ok_flag || recv_err_flag )
                    break;
                if( timeout_cnt >= 2 )
                {
                    timeout_flag = 1;
                    break;
                }
            }


            if( recv_err_flag || timeout_flag )
                break;  
        
            recv_ok_flag = 0;
        
        }
        else
        {
            if( recv_err_flag  )
                break;          
        }
        if( !send_fail_flag )
        {
            if ((j % 1024) == 0)
                dbg_pline_1(".");
        }
    }

    if( !full_mode )
    {
        if( timeout_flag )
            printf("Received packet timeout!\r\n");

        if(recv_err_flag || timeout_flag)
        {
            printf("\r\nThe %d send packet(length=%d 0x%x):\r\n", j-1, len, len);
            delay1ms(100);
            for( i=0; i<len; i++ )
                dbg_plineb_1(" ", mbp->data[i]);
            printf("\r\n");
        }
    }
    
    recv_ok_flag = 0;
    timeout_flag = 0;
    recv_err_flag = 0;
    phy_loopback_flag = 0;      
    if( mode == 1 )
    {
        // set phy-reg 0 bit14 = 1
        val = tcMiiStationRead(mac_p->enetPhyAddr, 0);
        val &= ~0x4000;
        tcMiiStationWrite(mac_p->enetPhyAddr, 0, val);
    }   
    if( mode == 2 )
    {
        tcMiiStationWrite(mac_p->enetPhyAddr, 0, old_val);
    
        // set phy-reg 26 bit[4:3] = 00
        val = tcMiiStationRead(mac_p->enetPhyAddr, 26);
        val &= ~0x0008;
        tcMiiStationWrite(mac_p->enetPhyAddr, 26, val);
    
    }           
    if( mode == 3 )
    {
        tcMiiStationWrite(mac_p->enetPhyAddr, 0, old_val);      

        // set phy-reg 26 bit[4:3] = 00
        val = tcMiiStationRead(mac_p->enetPhyAddr, 26);
        val &= ~0x0010;
        tcMiiStationWrite(mac_p->enetPhyAddr, 26, val);

    }               
    printf(" Loop=%d\r\n", j);
    printf("Done.\r\n");
   return 0;
}

int doMacSend (int argc, char *argv[], void *p)
{
    uint8 pattern;
    uint32 pattern32;
    uint32 len;
    uint32 loop;
    uint32 delay_cnt;   
    mbuf_t* mbp;
    uint32 i,j,state;

    sscanf(argv[1], "%x", &pattern32);
    len = atoi(argv[2]);
    loop = atoi(argv[3]);
    delay_cnt = atoi(argv[4]);
    
    pattern = (uint8) pattern32;
    dbg_plineb_1("\n\rFill pattern  =",pattern);
    dbg_plinel_1("\n\rPacket length =",len);
    dbg_plinel_1("\n\rLoop cnt      =",loop);
    dbg_plinel_1("\n\rDelay_cnt     =",delay_cnt);
    dbg_pline_1("\n\r");

    j = 0;
    while (j < loop) {
        mbp = (mbuf_t *)alloc_cache_mbuf2k();
        if(mbp == NULL){
            continue;
        }           

        for(i = 0; i < len; i++){   /* fill pattern */
            mbp->data[i] = pattern;
        }
        mbp->cnt = len;

        state = dirps();
        if (macSend( 0, mbp) == 0)
            j++;
        restore(state);

        // print "." every 1024 packets
        if ((j % 1024) == 0)
            dbg_pline_1(".");
    
        if( delay_cnt != 0 ){
            if( (j%delay_cnt)==0)
                delay1ms(1);
        }
    }
    dbg_pline_1("\n\rDone!\n\r");
    
    return 0;
}

int doMacSendRandom (int argc, char *argv[], void *p)
{
    uint8 pattern;
    uint32 pattern32;
    uint32 len;
    uint32 loop;
    mbuf_t* mbp;
    uint32 i,j,state;

    if( argc == 2 )
    {
        loop = atoi(argv[1]);
        dbg_plinel_1("\n\rLoop cnt      =",loop);
    } 
    else if( argc==3 )
    {
        len=atoi(argv[1]);
        loop = atoi(argv[2]);
        dbg_plinel_1("\n\rLoop cnt      =",loop);       
        dbg_plinel_1("\n\rPacket length =",len);        

    }
    else
    {
        printf("Usage: sendrandom <len> <loop>\r\n");
        printf("       sendrandom <loop>\r\n");
        return 0;
    }

    j = 0;
    while (j < loop) {
        mbp = (mbuf_t *)alloc_cache_mbuf2k();
        if(mbp == NULL){
            continue;
        }

        /* da */
        mbp->data[0] = 0xff;
        mbp->data[1] = 0xff;
        mbp->data[2] = 0xff;
        mbp->data[3] = 0xff;
        mbp->data[4] = 0xff;
        mbp->data[5] = 0xff;
        /* sa */
        mbp->data[6] = 0x00;
        mbp->data[7] = 0x00;
        mbp->data[8] = 0x00;
        mbp->data[9] = 0x00;
        mbp->data[10] = 0x00;
        mbp->data[11] = 0x01;
        /* ether type */
        mbp->data[12] = 0xff;
        mbp->data[13] = 0xff;
        if( argc == 2 )     
            len = rand();
        if (len < 60)
            len = 60;
        if (len > 1514)
            len = 1514;
        for(i = 14; i < len; i++){  /* fill pattern */
            mbp->data[i] = (uint8) (rand() + rand());
        }
        mbp->cnt = len;
        
        state = dirps();
        if (macSend( 0, mbp) == 0)
            j++;
        restore(state);

        // print "." every 1024 packets
        if ((j % 1024) == 0) {
            dbg_pline_1(".");
            //dbg_plinel_1("\r\n loop=", j);
        }
    }
    dbg_plinel_1("\r\n loop=", j);
    dbg_pline_1("\n\rDone!\n\r");

    return 0;
}
#endif
#ifdef LINUX_OS

/*_____________________________________________________________________________
**      function name: delay1ms
**      descriptions:
**           delay 1 ms 
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
/*_____________________________________________________________________________
**      function name: reverseLong
**      descriptions:
**          Reverse the bytes ordering of the input value.
**             
**      parameters:
**            ul: Specify the 4 bytes value that you want to reverse the ordering.
**             
**      global:
**            None
**             
**      return:
**           Reverse value.
**	     
**      call:
**            None
**      	
**      revision:
**      1. Here 2008/08/24
**____________________________________________________________________________
*/
static uint32
reverseLong(
	uint32 ul
)
{
	uint8 z[4];

	z[3] = *((uint8 *)&ul + 0);
	z[2] = *((uint8 *)&ul + 1);
	z[1] = *((uint8 *)&ul + 2);
	z[0] = *((uint8 *)&ul + 3);

	return *((uint32 *)z);
}/*end reverseLong*/
/*_____________________________________________________________________________
**      function name: scramble
**      descriptions:
**           Scramble the input 32bits value. 
**             
**      parameters:
**            None
**             
**      global:
**            None
**             
**      return:
**            After Scramble the value
**	     
**      call:
**            reverseLong
**      	
**      revision:
**      1. Here 2008/08/24
**____________________________________________________________________________
*/
static uint32
scramble(uint32 checkCode)
{
	uint32 a[6];

	a[1] = (checkCode & 0x0000001F) << 0x0C;
	a[2] = (checkCode & 0x03E003E0) << 0x01;
	a[3] = (checkCode & 0xF8000400) >> 0x0A;
	a[4] = (checkCode & 0x0000F800) << 0x10;
	a[5] = (checkCode & 0x041F0000) >> 0x0F;
	checkCode = a[1] + a[2] + a[3] + a[4] + a[5];

	/* ICQ's check code is little-endian. Change the endian style */
	checkCode = reverseLong(checkCode);

	return checkCode;
}/*end scramble*/
/*_____________________________________________________________________________
**      function name: rand
**      descriptions:
**           Random value generation. 
**             
**      parameters:
**            None
**             
**      global:
**            None
**             
**      return:
**            Random value
**	     
**      call:
**            timerVlrGet
**      	
**      revision:
**      1. Here 2008/08/24
**____________________________________________________________________________
*/
static uint32
rand(void){
	uint32 vlr;
	timerVlrGet(1, vlr);
	scramble(vlr);
	return (vlr & RAND_MAX);
}
/*_____________________________________________________________________________
**      function name: doMacSend
**      descriptions:
**           send packet  with the  data pattern we specific
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
static int doMacSend(int argc, char *argv[], void *p)
{
	uint8 pattern;
	uint32 pattern32;
	uint32 len;
	uint32 loop;
	uint32 delay_cnt = 0;	
	struct sk_buff *mbp = NULL;
	uint32 i,j;
	
	sscanf(argv[1], "%lx", &pattern32);
	
	len = simple_strtoul(argv[2],NULL,10);
	loop = simple_strtoul(argv[3],NULL,10);
	if(argv[4] != NULL){
		delay_cnt  = simple_strtoul(argv[4],NULL,10);
	}
	 
	pattern = (uint8) pattern32;
	dbg_plineb_1("\n\rFill pattern  =",pattern);
	dbg_plinel_1("\n\rPacket length =",len);
	dbg_plinel_1("\n\rLoop cnt      =",loop);
	dbg_plinel_1("\n\rdelay_cnt      =",delay_cnt);

	j = 0;
	while (j < loop) {
		mbp = (struct sk_buff *)skbmgr_dev_alloc_skb2k();
		if(mbp == NULL){
			continue;
		}			
		skb_reserve(mbp, 2);
		skb_put(mbp, len);
		
		for(i = 0; i < len; i++){	/* fill pattern */
			mbp->data[i] = pattern;
		}

		#if  defined(CONFIG_MIPS_TC3182) ||  defined(CONFIG_MIPS_TC3262)
		if(tc3262_gmac_tx(mbp, tc3262_gmac_dev) == 0){
			j++;
		}
		#else
		local_irq_disable();
		if(tc3162_mac_tx(mbp, tc3162_mac_dev) == 0){
			j++;
		}
		local_irq_enable();
		#endif

		if( delay_cnt != 0 ){
			if( (j%delay_cnt)==0)
				delay1ms(1);
		}
	}
	dbg_pline_1("\n\rDone!\n\r");
	return 0;
}
/*_____________________________________________________________________________
**      function name: doMacSendRandom
**      descriptions:
**         send packet with random dataa pattern
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
static int doMacSendRandom(int argc, char *argv[], void *p)
{
	uint32 len=0;
	uint32 loop=0;
	struct sk_buff *mbp = NULL;
	uint32 i,j;

	if( argc == 2 )
	{
		loop = simple_strtoul(argv[1],NULL,10);
		dbg_plinel_1("\n\rLoop cnt      =",loop);
	} 
	else if( argc==3 )
	{
		len=simple_strtoul(argv[1],NULL,10);
		loop = simple_strtoul(argv[2],NULL,10);
		dbg_plinel_1("\n\rLoop cnt      =",loop);		
		dbg_plinel_1("\n\rPacket length =",len);		

	}
	else
	{
		printf("Usage: sendrandom <len> <loop>\r\n");
		printf("       sendrandom <loop>\r\n");
		return 0;
	}

	j = 0;
	while (j < loop) {
		mbp =  (struct sk_buff *)skbmgr_dev_alloc_skb2k();
		if(mbp == NULL){
			continue;
		}

		/* da */
		mbp->data[0] = 0xff;
		mbp->data[1] = 0xff;
		mbp->data[2] = 0xff;
		mbp->data[3] = 0xff;
		mbp->data[4] = 0xff;
		mbp->data[5] = 0xff;
		/* sa */
		mbp->data[6] = 0x00;
		mbp->data[7] = 0x00;
		mbp->data[8] = 0x00;
		mbp->data[9] = 0x00;
		mbp->data[10] = 0x00;
		mbp->data[11] = 0x01;
		/* ether type */
		mbp->data[12] = 0xff;
		mbp->data[13] = 0xff;
		if( argc == 2 )		
			len = rand();
		if (len < 60)
			len = 60;
		if (len > 1514)
			len = 1514;
		for(i = 14; i < len; i++){	/* fill pattern */
			mbp->data[i] = (uint8) (rand() + rand());
		}
		skb_reserve(mbp, 2);
		skb_put(mbp, len);
		
		#if  defined(CONFIG_MIPS_TC3182) ||  defined(CONFIG_MIPS_TC3262)
		if(tc3262_gmac_tx(mbp, tc3262_gmac_dev) == 0){
			j++;
		}
		#else
		local_irq_disable();
		if(tc3162_mac_tx(mbp, tc3162_mac_dev) == 0)
			j++;
		
		local_irq_enable();
		#endif
		// print "." every 1024 packets
		if ((j % 1024) == 0) {
			dbg_pline_1(".");
			//dbg_plinel_1("\r\n loop=", j);
		}
	}
	dbg_plinel_1("\r\n loop=", j);
	dbg_pline_1("\n\rDone!\n\r");

	return 0;
}
#endif
#ifndef PURE_BRIDGE
#ifndef LINUX_OS
// called by doPing()
static int
pingExec(
    cbuf_t  *cbp,           /* cbuf pointer         */
    uint16  signature,      /* event function signature */
    void    *data_p,        /* data pointer         */
    uint16  data_size       /* data size            */
)
{
    pingEvt_t *ping_evt_p = (pingEvt_t *)data_p;
    pingtx( ping_evt_p->ping_p );
    return 0;
} /* pintExec */
// called by doPing()
static int
pingKeyProc( 
    int c
)
{
    struct usock *up;
    extern pingCmdCb_t pingCmdCb;

    /* Ignore all but ^C */
    if(c != CTLC)
        return 0;
    if (pingCmdCb.actSocket == -1 || (up=itop(pingCmdCb.actSocket)) == NULL)
        return 0;
    printf("^C\n");
    malert(up->eventid,1);  /* zc 5/18/95 - change to mb */
    pingCmdCb.actSocket = -1;
    pingCmdCb.flags.active = 0;
    return 0;
} /* pingKeyProc */
#endif
static int doPing (int argc, char *argv[], void *p)
{       
#ifndef LINUX_OS	
    pingEvt_t ping_evt;
    mbuf_t  *bp;
    struct sockaddr_in from;
    icmp_t  icmp;
    ping_t  *ping_p;
    uint32  timestamp;
    uint32  rtt;
    int32   abserr;
    ip4a    target, dnsQueryResult;
    int ip_sock;
    int fromlen;
    int max_size;
    int retvalue = 0;
    
    extern pingCmdCb_t pingCmdCb;
    int tmp;
//  ping_test_flag = 1;

    // clear ReplyCnt
    PingReplyCnt = 0;
    if ( (ping_p=calloc(1, sizeof(ping_t))) == NULL )
        sysreset();
    if ( (ip_sock = socket(AF_INET,SOCK_RAW,ICMP_PTCL)) == -1 )
    {
        printf("Can't create socket\n");
        retvalue = 1;
        goto ping_exit1;
    }
    ping_p->s = ip_sock;

    printf("Resolving %s... ",argv[1]);
    dnsQueryResult = resolveName( argv[1] );
    if(!dnsQueryResult)
    {
        printf("unknown\n");
        retvalue = 1;
        goto ping_exit;
    }
    if ( resolve( inet_ntoa(dnsQueryResult), &ping_p->target, NULL) != 0 )
    {
        printf("unknown\n");
        retvalue = 1;
        goto ping_exit;
    }
    printf("%s\n",inet_ntoa(ping_p->target));
    
    pingCmdCb.flags.user = 1;
    pingCmdCb.option = 1;
    
    tmp = atoi(argv[2]) - 42;
    if(tmp < 18 || tmp > 1472)
    {
        printf("Valid packet length is: 60 < len < 1514\r\n");
        goto ping_exit;
    }
    else
        ping_p->len = tmp;
    tmp = atoi(argv[3]);
    if(tmp <= 0)
    {
        printf("Valid loopnum is: loopnum > 0\r\n");
        goto ping_exit;
    }
    else
        pingCmdCb.pingCnt = tmp;
    tmp = atoi(argv[4]);
    if(tmp < 0)
        ping_p->interval = 0;
    else
        ping_p->interval = tmp;

ping_start:
    TRCLOG3216( LOG_IP, 5, "doping %lx %x %x", 
        ping_p->target,ping_p->len,ping_p->interval
        );

    /* user has specified interval */
    pingCmdCb.flags.active = 1;
    pingCmdCb.actSocket = ip_sock;
    if ( olStart( NULL, OL_SIG_PING ) != 0 )
    {
        goto ping_exit;
    }
    ping_p->flag.from = 1;
    ping_evt.ping_p = ping_p;
    olDataCmdSend(
        NULL,
        EVT_SYS_OL_PING,
        pingExec,
        OL_SIG_PING,
        &ping_evt,
        sizeof(pingEvt_t)
        );
    /* CTLC is the delimeter */
    ControlStdio(0,0,CTLC,pingKeyProc); 

    retvalue = 1;

    pause(1000);
        
    ResetStdio();
    olStop( NULL, OL_SIG_PING );
    pingCmdCb.option = 0;

ping_exit:
    close(ip_sock);
    pause(1000); // waiting PingReply Completed
    printf("Ping Reply Count: %d\r\n", PingReplyCnt);

ping_exit1:
    free(ping_p);
//  ping_test_flag = 1;
//  if( retvalue )
//      printf("\r\nping failed!!");
//  return retvalue;
#else
	setUserSpaceFlag(4);
	return 0;
#endif
}

static int doPingEchoCnt (int argc, char *argv[], void *p)
{
    if(!strcmp(argv[1], "clear"))
        PingReplyCnt = 0;
    printf("Ping Reply Count: %ld\r\n", PingReplyCnt);
    return 0;
}
#endif

static int doPhyDispFlag (int argc, char *argv[], void *p)
{
    int i;
    if( argc >= 2 && argc <= 9 ) {
        tcPhy_disp_level=0;
        
        for(i=1; i<argc; i++) {
            if (stricmp(argv[i], "on") == 0){
                tcPhy_disp_level++;
            }
        }
    }   
    
    printf("tcPhy display level: %d.\r\n", tcPhy_disp_level);
    return 0;
}



int doPhyChkVal (int argc, char *argv[], void *p)
{
    phychkval_flag = 1; 

    if (argc == 2) { // set port_num for tcPhyChkVal()
        switch (tcPhyVer) {
            case tcPhyVer_2104mc:
            case tcPhyVer_2104sd:
			case tcPhyVer_2104me:	
                phychkval_portnum= atoi(argv[1]);
                break;
			case tcPhyVer_2105sg:
				phychkval_portnum= atoi(argv[1]);
                break;
			case tcPhyVer_2105mj:
				phychkval_portnum= atoi(argv[1]);
                break;	
			case tcPhyVer_2105sk:
				phychkval_portnum= atoi(argv[1]);
                break;
            default:
            	  break;
        }
    }
    return 0;
}

// called by macPeriodCheck() in tc3162l2mac.c
void tcPhyChkVal (void)
{
    uint32 port_num = phychkval_portnum; 
#ifdef PHYPART_DEBUG
	printf("PhyPart debug: tcPhyChkVal() in \r\n");
#endif

    if(phychkval_flag){
        uint32 val;
        uint32 mr02,mr03;
        uint8 mr_anen, mr_dplx, mr_speed;
        uint8 mr_an_capable;
        uint8 mr_lp_an_capable;
        uint8 mr_lp_anen;
        //uint8 lr_force_mdix;
        uint8 lr_linkup, lr_speed, lr_dplx, lr_mdix;

        // show phy_id
        val = tcPhyReadReg(port_num, 2); 
        mr02 = val&(0xffff);
        val = tcPhyReadReg(port_num, 3); 
        mr03 = val&(0xffff);
        printf(" TcPhy ID: %lx %lx\r\n",mr02,mr03); 
        if (mr02 != 0x03a2)
            return;

        // reg0h
        val = tcPhyReadReg(port_num, 0);
        mr_anen = (val>>12)&0x01;
        mr_dplx = (val>>8)&0x01;
        //mr_speed = (val>>13)&0x01;
		mr_speed = (val>>13)|(val>>5)&0x03;//00:10,01:100,10:1000
        // reg4h
        val = tcPhyReadReg(port_num, 0x04);
        mr_an_capable = (val>>5)&0x0F; //100F,100H,10F,10H
        
        // reg9h
        val = tcPhyReadReg(port_num, 0x09);
		mr_an_capable = mr_an_capable | ((val>>4)&0x30);//1000F,1000H
			
        // reg5h
        val = tcPhyReadReg(port_num, 0x05);
        mr_lp_an_capable = (val>>5)&0x0F; //100F,100H,10F,10H
        // regAh
        val = tcPhyReadReg(port_num, 0x0A);
        mr_lp_an_capable = mr_lp_an_capable | ((val>>6)&0x30); //1000F,1000H
        
        // reg6h
        val = tcPhyReadReg(port_num, 0x06);
        mr_lp_anen = (val)&0x01;
        // l0reg28
        #if 1 //MT7530 
		
		val = tcMiiExtStationRead(0x1E, port_num, 0xA2, NORMAL_READ);
        lr_linkup = (val>>9)&0x07;//dev1e_A2[9]:LinkStatus10_OK,dev1e_A2[10]:LinkStatus100_OK,dev1e_A2[11]:LinkStatus1000_OK
        lr_speed = (val>>1)&0x07; //dev1e_A2[1]:final_speed_10,,dev1e_A2[2]:dev1e_A2[2]:final_speed_100,dev1e_A2[3]:final_speed_1000
        lr_dplx = (val>>5)&0x01;
        lr_mdix = (val>>6)&0x01;
		
		#else
        val = tcPhyReadReg(port_num, 28);
        lr_linkup = (val)&0x01;
        lr_speed = (val>>1)&0x01;
        lr_dplx = (val>>2)&0x01;
        lr_mdix = (val>>5)&0x01;
        #endif
        
        printf(" TcPhy mode:");
        if(mr_anen) { // Auto-neg
            printf(" AN-(");
            printf(" %s,",((mr_an_capable>>5)&0x01)?"1000F":"");
            printf(" %s,",((mr_an_capable>>4)&0x01)?"1000F":"");
            printf(" %s,",((mr_an_capable>>3)&0x01)?"100F":"");
            printf(" %s,",((mr_an_capable>>2)&0x01)?"100H":"");     
            printf(" %s,",((mr_an_capable>>1)&0x01)?"10F":"");      
            printf(" %s)\r\n",((mr_an_capable>>0)&0x01)?"10H":"");  
        }
        else { // Force-speed
            //printf(" Force-%d%s\r\n",
                    //(mr_speed?100:10),(mr_dplx?"F":"H"));
                    
             if(mr_speed==0x00){
			 	printf(" Force-%d%s\r\n",10,(mr_dplx?"F":"H"));
				}
			 else if(mr_speed==0x01){
				 printf(" Force-%d%s\r\n",100,(mr_dplx?"F":"H"));
			 	}
			 
			 else if(mr_speed==0x02){
				 printf(" Force-%d%s\r\n",1000,(mr_dplx?"F":"H"));
        }

        }
        if(!lr_linkup) { // link-down
            printf(" *** Link is down!\r\n");
            // ADC sum
            if(tcPhyVer==tcPhyVer_2101mb){
                #ifdef TC2101MB_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2101mbReadAdcSum(port_num));            
                #endif
            }
            else if(tcPhyVer==tcPhyVer_2104mc){
                #ifdef TC2104MC_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2104mcReadAdcSum(port_num));            
                #endif
            }
            else if(tcPhyVer==tcPhyVer_2104sd){
                #ifdef TC2104SD_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2104sdReadAdcSum(port_num));            
                #endif
            }
            else if(tcPhyVer==tcPhyVer_2101me){
                #ifdef TC2101ME_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2101meReadAdcSum(port_num));            
                #endif
            }
			else if(tcPhyVer==tcPhyVer_2102me){
				#ifdef TC2102ME_SUPPORT
					printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
							mac_p->enetPhyAddr, tc2102meReadAdcSum(port_num));			
				#endif
			}
			else if(tcPhyVer==tcPhyVer_2104me){
                #ifdef TC2104ME_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2104meReadAdcSum(port_num));            
                #endif
            }
			 else if(tcPhyVer==tcPhyVer_2101mf){
                #ifdef TC2101MF_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2101mfReadAdcSum(port_num));            
                #endif
            }
			 else if(tcPhyVer==tcPhyVer_2105sg){
                #ifdef TC2105SG_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2105sgReadAdcSum(port_num));            
                #endif
            }
			 else if(tcPhyVer==tcPhyVer_2101mi){
                #ifdef TC2101MI_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2101miReadAdcSum(port_num));            
                #endif
            }
			 else if(tcPhyVer==tcPhyVer_2105mj){
			 	#ifdef TC2105MJ_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2105mjReadAdcSum(port_num));            
                #endif
			}
			else if(tcPhyVer==tcPhyVer_2105sk){
			 	#ifdef TC2105SK_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2105skReadAdcSum(port_num));            
                #endif
			} 
            // L0R28 message
            if (mr28.lch_sig_detect || mr28.lch_rx_linkpulse
                || mr28.lch_linkup_100 || mr28.lch_linkup_10)
            {
                TCPHYDISP4(" tcphy[%ld]: ",port_num);
                if (mr28.lch_sig_detect) 
                    TCPHYDISP4("SigDet ");
                if (mr28.lch_rx_linkpulse)
                    TCPHYDISP4("RxLkp ");
                if (mr28.lch_linkup_100)
                    TCPHYDISP4("Up100 ");
                if (mr28.lch_linkup_10)
                    TCPHYDISP4("Up10 ");
                if (mr28.lch_linkup_mdi)
                    TCPHYDISP4("UpMdi ");
                if (mr28.lch_linkup_mdix)
                    TCPHYDISP4("UpMdix ");
                TCPHYDISP4("\r\n");
            }
        }
        else { // link-up
            printf(" TCphy is link-up at %d%s ",
                    (lr_speed?100:10),(lr_dplx?"F":"H"));
			
            if(tcPhyVer==tcPhyVer_2031){
                #ifdef TC2031_SUPPORT
                    printf("via %s\r\n",tc2031_cfg[current_idx].name);
                #endif
            }
            else {
                printf(".\r\n");
            }
            
            if(mr_lp_anen) {
                printf(" Link-partner supports AN-(");
                printf(" %s,",((mr_lp_an_capable>>5)&0x01)?"1000F":"");
                printf(" %s,",((mr_lp_an_capable>>4)&0x01)?"1000F":"");
                printf(" %s,",((mr_lp_an_capable>>3)&0x01)?"100F":"");
                printf(" %s,",((mr_lp_an_capable>>2)&0x01)?"100H":"");  
                printf(" %s,",((mr_lp_an_capable>>1)&0x01)?"10F":"");       
                printf(" %s)\r\n",((mr_lp_an_capable>>0)&0x01)?"10H":"");   
            }
            else {
                printf(" Link-partner operates in Force mode.\r\n");
            }

            printf(" %s,",(lr_mdix?"mdix":"mdi"));
            if (tcPhyVer==tcPhyVer_2104mc
                || tcPhyVer==tcPhyVer_2104sd
                || tcPhyVer==tcPhyVer_2101me
				|| tcPhyVer==tcPhyVer_2102me
                || tcPhyVer==tcPhyVer_2104me
                || tcPhyVer==tcPhyVer_2101mf
                || tcPhyVer==tcPhyVer_2105sg
                || tcPhyVer==tcPhyVer_2101mi
                || tcPhyVer==tcPhyVer_2105mj
                || tcPhyVer==tcPhyVer_2105sk)
            {
                printf(" tx_amp_save=%d\r\n",(mr28.tx_amp_save));
            }           
            printf("\r\n");
            
            if(lr_speed) { // 100Mbps
                if(tcPhyVer==tcPhyVer_2031){                    
                    #ifdef TC2031_SUPPORT
                        tc2031DispProbe100();
                    #endif
                }
                else if(tcPhyVer==tcPhyVer_2101mb){
                    #ifdef TC2101MB_SUPPORT
                        tc2101mbDispProbe100(port_num);
                    #endif
                }
                else if(tcPhyVer==tcPhyVer_2104mc){
                    #ifdef TC2104MC_SUPPORT
                        tc2104mcDispProbe100(port_num,1); // long message
                    #endif
                }
                else if(tcPhyVer==tcPhyVer_2104sd){
                    #ifdef TC2104SD_SUPPORT
                        tc2104sdDispProbe100(port_num,1);
                    #endif
                }
                else if(tcPhyVer==tcPhyVer_2101me){
                    #ifdef TC2101ME_SUPPORT
                        tc2101meDispProbe100(port_num);
                    #endif
                }
				else if(tcPhyVer==tcPhyVer_2102me){
					#ifdef TC2102ME_SUPPORT
						tc2102meDispProbe100(port_num,1);
					#endif
				}
				else if(tcPhyVer==tcPhyVer_2104me){
                    #ifdef TC2104ME_SUPPORT
                        tc2104meDispProbe100(port_num,1); // long message
                    #endif
                }
				else if(tcPhyVer==tcPhyVer_2101mf){
                    #ifdef TC2101MF_SUPPORT
                        tc2101mfDispProbe100(port_num);
                    #endif
                }
				else if(tcPhyVer==tcPhyVer_2105sg){
                    #ifdef TC2105SG_SUPPORT
                        tc2105sgDispProbe100(port_num,1);
                    #endif
                }
				else if(tcPhyVer==tcPhyVer_2101mi){
                    #ifdef TC2101MI_SUPPORT
                        tc2101miDispProbe100(port_num,1);
                    #endif
                }
				else if(tcPhyVer==tcPhyVer_2105mj){
                    #ifdef TC2105MJ_SUPPORT
                        tc2105mjDispProbe100(port_num,1);
                    #endif
                }
				else if(tcPhyVer==tcPhyVer_2105sk){
                    #ifdef TC2105SK_SUPPORT
                        tc2105skDispProbe100(port_num,1);
                    #endif
                }
                printf("\r\n");
            }   
        }
    }
    phychkval_flag = 0;
#ifdef PHYPART_DEBUG
	printf("PhyPart debug: tcPhyChkVal() out \r\n");
#endif

}

static int doPhyForceLink (int argc, char *argv[], void *p)
{
    if( !tcPhyFlag )
        return 0;

    if( argc != 2 ) {
        printf("forcelink status: %s.\r\n", (force_link_flag?"on":"off"));
    }
    else if( stricmp(argv[1], "on") == 0 )  {
        force_link_flag = 1;        
        printf("Force link status: ON.\r\n");
    }
    else if( stricmp(argv[1], "off") == 0 ) {
        force_link_flag = 0;        
        printf("Force link status: OFF.\r\n");
    }           
    else {
        printf("Usage: forcelink status [on|off] \r\n");
    }
    return 0;
}

#ifdef TC2031_DEBUG
// use only in LEH
static int doPhyErrOver (int argc, char *argv[], void *p)
{
    if( !tcPhyFlag ) {
        printf("NOT TcPhy!!\r\n");
        return 0;
    }

    if ( tcPhyVer != tcPhyVer_2031) {
        printf("NOT support!!\r\n");
        return 0;
    }
    else {
        // call tc2031SetPhyErrOver()
    }
        
    
    if( (argc != 2) && (argc != 3) && (argc != 4))
    {
        printf("usage: etherdbg errover [thd=0..2047] [thd4up=0..2047] [RepInt=0..300]\r\n");
        printf("       current sr_err_over_thd = %ld\r\n",  sr_err_over_thd);
        printf("       current sr_err_over_thd4up = %ld\r\n",  sr_err_over_thd4up);
        printf("       current sr_err_rep_time = %ld\r\n",  sr_err_rep_time);
        printf("       '0' means 'disable'\r\n");
        return 0;
    }
    sr_err_over_thd = atoi(argv[1]);
    printf("new sr_err_over_thd = %ld\r\n",  sr_err_over_thd);
    if( argc >= 3 )
    {
        sr_err_over_thd4up= atoi(argv[2]);
        printf("new sr_err_over_thd4up = %ld\r\n",  sr_err_over_thd4up);
    }
    if( argc >= 4 )
    {
        sr_err_rep_time = atoi(argv[3]);
        printf("new sr_err_rep_time = %ld\r\n", sr_err_rep_time);
    }
    return 0;
}
#endif

// set flag for tcPhyErrMonitor
static int doErrMonitor (int argc, char *argv[], void *p)
{
    if( !tcPhyFlag )
        return 0;

    if( argc != 2 ) {
        printf("Error monitor status: %s!\r\n", (err_monitor_flag?"on":"off"));
    }
    else if( stricmp(argv[1], "on") == 0 )  {
        err_monitor_flag = 1;       
        printf("Error monitor ON!\r\n");
    }
    else if( stricmp(argv[1], "off") == 0 ) {
        err_monitor_flag = 0;       
        printf("Error monitor OFF!\r\n");
    }           
    else {
        printf("Usage: errmonitor [on|off] \r\n");
    }
    return 0;
}

int tcPhyErrMonitor(void){
    uint32 val;
    uint16 curr_runt_cnt = 0;
    uint16 curr_tlcc_cnt = 0;
    uint16 curr_crc_cnt = 0;
    uint16 curr_long_cnt = 0;
    uint16 curr_loss_cnt = 0;
    uint16 curr_col_cnt = 0;

#ifdef PHYPART_DEBUG
	printf("PhyPart debug: tcPhyErrMonitor() in \r\n");
#endif

    if(err_monitor_flag)
    {
        val = VPint(CR_MAC_RUNT_TLCC_CNT);
        curr_runt_cnt = (uint16)(val>>16);
        curr_tlcc_cnt = (uint16)(val&0x0000ffff);

        val = VPint(CR_MAC_RCRC_RLONG_CNT);
        curr_crc_cnt = (uint16)(val>>16);
        curr_long_cnt = (uint16)(val&0x0000ffff);

        val = VPint(CR_MAC_RLOSS_RCOL_CNT);
        curr_loss_cnt = (uint16)(val>>16);
        curr_col_cnt = (uint16)(val&0x0000ffff);

        if( (curr_runt_cnt != runt_cnt) || (curr_tlcc_cnt != tlcc_cnt)  || 
            (curr_crc_cnt != crc_cnt) || (curr_long_cnt != long_cnt) ||
                (curr_loss_cnt != loss_cnt) || (curr_col_cnt != col_cnt))
        {
            printf("runt=%d tlcc=%d crc=%d long=%d loss=%d col=%d\r\n", 
                curr_runt_cnt, curr_tlcc_cnt, curr_crc_cnt, curr_long_cnt, curr_loss_cnt, curr_col_cnt );
            runt_cnt = curr_runt_cnt;
            tlcc_cnt = curr_tlcc_cnt;
            crc_cnt = curr_crc_cnt;
            long_cnt = curr_long_cnt;
            loss_cnt = curr_loss_cnt;
            col_cnt = curr_col_cnt;         
        }
    }
    if( phy_loopback_flag )
    {
//      dbg_plinel_1("\r\n ", mac_p->macStat_p->MIB_II.inOctets);
        timeout_cnt++;
    }
#ifdef PHYPART_DEBUG
	printf("PhyPart debug: tcPhyErrMonitor() out \r\n");
#endif
    return 0;
}

#endif // TCPHY_DEBUG

/************************************************************************
*                       Functions Body for TCPHY_TESTDBG CI commands
*************************************************************************
*/

#ifdef TCPHY_FIELD_DBGTEST
static int doFieldDebugTest (int argc, char *argv[], void *p)
{
    if( !tcPhyFlag ) {
        printf("NOT TC PHY!!\r\n");
        return 0;
    }

    if (argc==2 || argc==3) {
        // yhchen: need to re-write to support both 2031 & 2101mb !!
        if(tcPhyVer == tcPhyVer_2031) {
            #ifdef TC2031_DBGTEST
                doPhyDebugTest(argc, argv, p);
            #endif
        }
        else {
            printf("Not support!!\r\n");
        }
        return 0;
    }

    printf("Usage: dbgtest <1|2|3> [wait_time]\r\n");
    return 0;
}

static int doFieldDebugPrint(int argc, char *argv[], void *p)
{
    uint32 i=0;
    for(i=0;i<msgcnt;i++) {
        printf("%3d: %s \n",i,tc_ether_phy[i]);
    }
    return 0;
}
#endif // TCPHY_FIELD_DBGTEST
#ifdef TCSUPPORT_DYNAMIC_VLAN
extern dyVLAN_PORT_t dyVLAN_PORT[];

void tcPortLinkChk(void)
{
	int port;
	for( port = 0; port < TCPHY_PORTNUM; port++){
		if(tcphy_link_state[port] == ST_LINK_DOWN2UP){
			printk("Port %d link up\n", port);
		}
		else if(tcphy_link_state[port] == ST_LINK_UP2DOWN){
			printk("Port %d link down\n", port);
			dyVLAN_PORT[port].state = 0;
		}
	}
	
}
#endif

int doDbgPorbeSet(int argc, char *argv[], void *p)
{
	uint16 PortSel=0;
	uint32 value=0;
	uint32 DbgflgSel=0;
	uint32 DbgClk=0;
	
 #define dbg_flg_en 1 
 #define dbg_DevAddr 0x1F
 #define dbg_Flg0Ctrl 0x15
 #define dbg_Flg1Ctrl 0x16
 #define dbg_ClkAddr 0x19
	
   if (argc==4){
			PortSel = checked_atoi(argv[2]);
			sscanf(argv[3], "%lx", &DbgflgSel);
			//DbgClk = checked_atoi(argv[3]);
			printf("* dbg_port_Sel=%d, dbg_flag_sel=0x%04lX\r\n", PortSel, DbgflgSel);		
		}
   else if (argc==5){
			PortSel = checked_atoi(argv[2]);
			sscanf(argv[3], "%lx", &DbgflgSel);
			//DbgClk = checked_atoi(argv[3]);
			sscanf(argv[4], "%lx", &DbgClk);
			printf("* dbg_port_Sel=%d, dbg_flag_sel=0x%04lX, dbg_clk_sel=0x%02lX\r\n", PortSel, DbgflgSel, DbgClk);		
		}

	if (((argc == 4) ||(argc == 5)) && (PortSel <= 31)){
		if (argc == 4){
			if(stricmp(argv[1], "all") == 0){
	           value = (PortSel<<13)|(dbg_flg_en<<12)|(DbgflgSel);
			   printf("Dbg Flag Register Set Value=%lx\r\n", value);
			   //tcMiiExtStationWrite_ext(PortSel, dbg_DevAddr, dbg_Flg0Ctrl, value);
			   mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_Flg0Ctrl, value);
			   //tcMiiExtStationWrite_ext(PortSel, dbg_DevAddr, dbg_Flg1Ctrl, value);
			   mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_Flg1Ctrl, value);
			}
			else if(stricmp(argv[1], "0") == 0){
				value = (PortSel<<13)|(dbg_flg_en<<12)|(DbgflgSel);
				printf("Dbg Flag Register Set Value=%lx\r\n", value);
				//tcMiiExtStationWrite_ext(PortSel, dbg_DevAddr, dbg_Flg0Ctrl, value);
				mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_Flg0Ctrl, value);
			}
			else if(stricmp(argv[1], "1") == 0){
				value = (PortSel<<13)|(dbg_flg_en<<12)|(DbgflgSel);
				printf("Dbg Flag Register Set Value=%lx\r\n", value);
				//tcMiiExtStationWrite_ext(PortSel, dbg_DevAddr, dbg_Flg1Ctrl, value);
				mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_Flg1Ctrl, value);
			}

			//DbgClk = tcMiiExtStationRead_ext(PortSel, dbg_DevAddr, dbg_ClkAddr, NORMAL_READ);
			DbgClk = mtEMiiRegRead(PortSel, dbg_DevAddr, dbg_ClkAddr);
			
			value = DbgClk & 0xEF;
			//tcMiiExtStationWrite_ext(PortSel, dbg_DevAddr, dbg_ClkAddr, value);
			mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_ClkAddr, value);
		}
	else if((argc == 5) && (stricmp(argv[1], "1") == 0)){
	    value = (PortSel<<13)|(dbg_flg_en<<12)|(DbgflgSel);
    	//tcMiiExtStationWrite_ext(PortSel, dbg_DevAddr, dbg_Flg1Ctrl, value);
		mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_Flg1Ctrl, value);
		value = (dbg_flg_en<<12)|(DbgClk);
		//tcMiiExtStationWrite_ext(PortSel, dbg_DevAddr, dbg_ClkAddr, value);
		mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_ClkAddr, value);
    
	    printf("Dbg Flag Register Set Value=%lx\r\n", value);
		}
  
	else if((argc == 5) && (stricmp(argv[1], "all") == 0)){
	    value = (PortSel<<13)|(dbg_flg_en<<12)|(DbgflgSel);
	    printf("Dbg Flag Register Set Value=%lx\r\n", value);
    	//tcMiiExtStationWrite_ext(PortSel, dbg_DevAddr, dbg_Flg0Ctrl, value);
    	mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_Flg0Ctrl, value);

	    //tcMiiExtStationWrite_ext(PortSel, dbg_DevAddr, dbg_Flg1Ctrl, value);
		mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_Flg1Ctrl, value);
		value = (dbg_flg_en<<12)|(DbgClk);
		//tcMiiExtStationWrite_ext(PortSel, dbg_DevAddr, dbg_ClkAddr, value);
		mtEMiiRegWrite(PortSel, dbg_DevAddr, dbg_ClkAddr, value);
		}
	}
	else { // error message
		printf("Usage: dbgprb 1 <PortNo> <DbgflgSel> <Clk>\r\n");
		printf("       dbgprb 1 <PortNo> <DbgflgSel> \r\n");
		printf("       dbgprb 0 <PortNo> <DbgflgSel> \r\n");
		printf("       dbgprb all <PortNo> <DbgflgSel> <Clk>\r\n");
		
		return 0;
	}
	return 0;
}

int doPhyMiiRead_TrDbg(int argc, char *argv[], void *p)
{
    const uint16 page_reg=31;
    const uint32 Token_Ring_debug_reg=0x52B5;
    const uint32 Token_Ring_Control_reg=0x10;
    const uint32 Token_Ring_Low_data_reg=0x11;
    const uint32 Token_Ring_High_data_reg=0x12;


    uint16 phyaddr=0;
    uint16 ch_addr=0;
    uint32 node_addr=0;
    uint32 data_addr=0;

    uint32 value=0;
    uint32 value_high=0;
    uint32 value_low=0;
	int i=0;
	uint32 start_addr=0;
	uint32 end_addr=0;


    if(argc ==4) {// tce miir_trdgb <PhyAddr> <TrRegType> <DataAddr>
		phyaddr = checked_atoi(argv[1]);//0~31
		sscanf(argv[3], "%lx", &data_addr);// 6 bit HEX
        if (strcasecmp(argv[2], "DSPF") == 0){// DSP Filter Debug Node
            	ch_addr=0x02;
				node_addr=0x0D;
	       	}
		else if (strcasecmp(argv[2], "PMA") == 0){ // PMA Debug Node 
				ch_addr=0x01;
				node_addr=0x0F;
			}
		else if (strcasecmp(argv[2], "TR") == 0){ // Timing Recovery  Debug Node 
				ch_addr=0x01;
				node_addr=0x0D;
			}
		else if (strcasecmp(argv[2], "PCS") == 0){ // R1000PCS Debug Node 
				ch_addr=0x02;
				node_addr=0x0F;
			}
		else if (strcasecmp(argv[2], "FFE_A") == 0){ // FFE Debug Node 
				ch_addr=0x00;
				node_addr=0x04;
			}
		else if (strcasecmp(argv[2], "FFE_B") == 0){ // FFE Debug Node 
				ch_addr=0x01;
				node_addr=0x04;
			}
		else if (strcasecmp(argv[2], "FFE_C") == 0){ // FFE Debug Node 
				ch_addr=0x02;
				node_addr=0x04;
			}
		else if (strcasecmp(argv[2], "FFE_D") == 0){ // FFE Debug Node 
				ch_addr=0x03;
				node_addr=0x04;
			}
		else if (strcasecmp(argv[2], "FFE_INDEP") == 0){ // FFE Debug Node 
				ch_addr=0x00;
				node_addr=0x04;
			}
 		else if (strcasecmp(argv[2], "ECNC_A") == 0){ // EC/NC Debug Node 
				ch_addr=0x00;
				node_addr=(data_addr&0xC0)>>6; //V1.17 for ECNC_0xE0 is per port
				printf("* ECNC_A => Phyaddr=%d,TrRegTyp =%s,node_addr=0x%02lX,data_addr=0x%04lX ,ch_addr=0x%02lX\r\n",phyaddr, argv[2],node_addr,data_addr, ch_addr);
			}
 		else if (strcasecmp(argv[2], "ECNC_B") == 0){ // EC/NC Debug Node 
				ch_addr=0x01;
				node_addr=(data_addr&0xC0)>>6; //V1.17 for ECNC_0xE0 is per port
				printf("* ECNC_B => Phyaddr=%d,TrRegTyp =%s,node_addr=0x%02lX,data_addr=0x%04lX ,ch_addr=0x%02lX\r\n",phyaddr, argv[2],node_addr,data_addr, ch_addr);
			}
 		else if (strcasecmp(argv[2], "ECNC_C") == 0){ // EC/NC Debug Node 
				ch_addr=0x02;
				node_addr=(data_addr&0xC0)>>6; //V1.17 for ECNC_0xE0 is per port
				printf("* ECNC_C => Phyaddr=%d,TrRegTyp =%s,node_addr=0x%02lX,data_addr=0x%04lX ,ch_addr=0x%02lX\r\n",phyaddr, argv[2],node_addr,data_addr, ch_addr);
			}
 		else if (strcasecmp(argv[2], "ECNC_D") == 0){ // EC/NC Debug Node 
				ch_addr=0x03;
				node_addr=(data_addr&0xC0)>>6; //V1.17 for ECNC_0xE0 is per port
				printf("* ECNC_D => Phyaddr=%d,TrRegTyp =%s,node_addr=0x%02lX,data_addr=0x%04lX ,ch_addr=0x%02lX\r\n",phyaddr, argv[2],node_addr,data_addr, ch_addr);
			}
 		else if (strcasecmp(argv[2], "ECNC_INDEP") == 0){ // EC/NC Debug Node 
				ch_addr=0x00;
				node_addr=(data_addr&0xC0)>>6;
				printf("* ECNC_INDEP => Phyaddr=%d,TrRegTyp =%s,node_addr=0x%02lX,data_addr=0x%04lX ,ch_addr=0x%02lX\r\n",phyaddr, argv[2],node_addr,data_addr, ch_addr);
			}
		else if (strcasecmp(argv[2], "DFEDC_A") == 0){ // DFETail/DC Debug Node 
				ch_addr=0x00;
				node_addr=0x05;
			}
		else if (strcasecmp(argv[2], "DFEDC_B") == 0){ // DFETail/DC Debug Node 
				ch_addr=0x01;
				node_addr=0x05;
			}
		else if (strcasecmp(argv[2], "DFEDC_C") == 0){ // DFETail/DC Debug Node 
				ch_addr=0x02;
				node_addr=0x05;
			}
		else if (strcasecmp(argv[2], "DFEDC_D") == 0){ // DFETail/DC Debug Node 
				ch_addr=0x03;
				node_addr=0x05;
			}
		else if (strcasecmp(argv[2], "DFEDC_INDEP") == 0){ // DFETail/DC Debug Node 
				ch_addr=0x00;
				node_addr=0x05;
			}
		else if (strcasecmp(argv[2], "DEC") == 0){ // R1000DEC Debug Node 
				ch_addr=0x00; node_addr=0x07;
			}
		else if (strcasecmp(argv[2], "CRC_A") == 0){ // R1000CRC Debug Node 
				ch_addr=0x00;
				node_addr=0x06;
			}
		else if (strcasecmp(argv[2], "CRC_B") == 0){ // R1000CRC Debug Node 
				ch_addr=0x01;
				node_addr=0x06;
			}
		else if (strcasecmp(argv[2], "CRC_C") == 0){ // R1000CRC Debug Node 
				ch_addr=0x02;
				node_addr=0x06;
			}
		else if (strcasecmp(argv[2], "CRC_D") == 0){ // R1000CRC Debug Node 
				ch_addr=0x03;
				node_addr=0x06;
			}
		else if (strcasecmp(argv[2], "AN") == 0){ // Autoneg Debug Node 
				ch_addr=0x00; node_addr=0x0F;
			}
		else if (strcasecmp(argv[2], "CMI") == 0){ // CMI Debug Node 
				ch_addr=0x03; node_addr=0x0F;
			}
		else if (strcasecmp(argv[2], "SUPV") == 0){ // SUPV PHY  Debug Node 
				ch_addr=0x00; node_addr=0x0D;
			}
		else {
				printf("				   miir_trdgb <PhyAddr> <TrRegTyp>(DSPF|PMA|TR|PCS|FFE_x|ECNC_x|DFEDC_x|DEC|CRC_x|AN|CMI|SUPV) <DataAddr>\r\n");
				return 0;
			}
		data_addr=data_addr&0x3F;
		
		//tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
		mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
		//tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
		mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
		//while(!(tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
		//while(!(mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
		{
			//value_low = tcMiiStationRead_ext(phyaddr, Token_Ring_Low_data_reg);
			value_low = mtMiiRegRead(phyaddr, Token_Ring_Low_data_reg);
			//value_high = tcMiiStationRead_ext(phyaddr, Token_Ring_High_data_reg);
			value_high = mtMiiRegRead(phyaddr, Token_Ring_High_data_reg);
			value=value_low+((value_high&0x00FF)<<16);
			printf("* %s => Phyaddr=%d,TrRegTyp =%s,node_addr=0x%02lX,data_addr=0x%04lX ,value=0x%06lX\r\n", argv[2], phyaddr, argv[2],node_addr,data_addr, value);
		}
		//tcMiiStationWrite_ext(phyaddr, page_reg, 0x00);//V1.11
		mtMiiRegWrite(phyaddr, page_reg, 0x00);//V1.11
		return 0;
		
	}
	else if (argc==5){
		// tce miir_trdgb <PhyAddr> <CHAddr> <NodeAddr> <DataAddr>
        phyaddr = checked_atoi(argv[1]);//0~31
   	    ch_addr = checked_atoi(argv[2]);//0:A pair  /1:B pair  /2:C pair  /3:D pair 
		sscanf(argv[3], "%lx", &node_addr);// 4 bit HEX
		sscanf(argv[4], "%lx", &data_addr);// 6 bit HEX

        
		//tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
		mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
		//tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
		mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
		//while(!(tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
		//while(!(mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
		{
			//value_low = tcMiiStationRead_ext(phyaddr, Token_Ring_Low_data_reg);
			value_low = mtMiiRegRead(phyaddr, Token_Ring_Low_data_reg);
			//value_high = tcMiiStationRead_ext(phyaddr, Token_Ring_High_data_reg);			
			value_high = mtMiiRegRead(phyaddr, Token_Ring_High_data_reg);			
			value=value_low+((value_high&0x00FF)<<16);
			printf("* miir_trdgb => Phyaddr=%d, ch_addr=%d,node_addr=0x%02lX,data_addr=0x%04lX ,value=0x%06lX\r\n", phyaddr, ch_addr, node_addr, data_addr, value);
		}
		//tcMiiStationWrite_ext(phyaddr, page_reg, 0x00);//V1.11
		mtMiiRegWrite(phyaddr, page_reg, 0x00);//V1.11
		return 0;
    }
	else if(argc ==6) {//miir_trdgb all <PhyAddr> <TrRegTyp> <s_addr> <e_addr>
		phyaddr = checked_atoi(argv[2]);//0~31
		sscanf(argv[4], "%lx", &start_addr);//hex
		sscanf(argv[5], "%lx", &end_addr);//hex
		for(i=start_addr; i<=end_addr; i++)
		{
		    data_addr=i;
	        if (stricmp(argv[3], "DSPF") == 0){// DSP Filter Debug Node
        	    	ch_addr=0x02;
					node_addr=0x0D;
		       	}
			else if (stricmp(argv[3], "PMA") == 0){ // PMA Debug Node 
					ch_addr=0x01;
					node_addr=0x0F;
				}
			else if (stricmp(argv[3], "TR") == 0){ // Timing Recovery  Debug Node 
					ch_addr=0x01;
					node_addr=0x0D;
				}
			else if (stricmp(argv[3], "PCS") == 0){ // R1000PCS Debug Node 
					ch_addr=0x02;
					node_addr=0x0F;
				}
			else if (stricmp(argv[3], "FFE_A") == 0){ // FFE Debug Node 
					ch_addr=0x00;
					node_addr=0x04;
				}
			else if (stricmp(argv[3], "FFE_B") == 0){ // FFE Debug Node 
					ch_addr=0x01;
					node_addr=0x04;
				}
			else if (stricmp(argv[3], "FFE_C") == 0){ // FFE Debug Node 
					ch_addr=0x02;
					node_addr=0x04;
				}
			else if (stricmp(argv[3], "FFE_D") == 0){ // FFE Debug Node 
					ch_addr=0x03;
					node_addr=0x04;
				}
			else if (stricmp(argv[3], "FFE_INDEP") == 0){ // FFE Debug Node 
					ch_addr=0x00;
					node_addr=0x04;
				}
 			else if (stricmp(argv[3], "ECNC_A") == 0){ // EC/NC Debug Node 
					ch_addr=0x00;
					node_addr=(data_addr&0x40)>>6;
				}
 			else if (stricmp(argv[3], "ECNC_B") == 0){ // EC/NC Debug Node 
					ch_addr=0x01;
					node_addr=(data_addr&0x40)>>6;
				}
 			else if (stricmp(argv[3], "ECNC_C") == 0){ // EC/NC Debug Node 
					ch_addr=0x02;
					node_addr=(data_addr&0x40)>>6;
				}
 			else if (stricmp(argv[3], "ECNC_D") == 0){ // EC/NC Debug Node 
					ch_addr=0x03;
					node_addr=(data_addr&0x40)>>6;
				}
 			else if (stricmp(argv[3], "ECNC_INDEP") == 0){ // EC/NC Debug Node 
					ch_addr=0x00;
					node_addr=(data_addr&0xC0)>>6;
				}
			else if (stricmp(argv[3], "DFEDC_A") == 0){ // DFETail/DC Debug Node 
					ch_addr=0x00;
					node_addr=0x05;
				}
			else if (stricmp(argv[3], "DFEDC_B") == 0){ // DFETail/DC Debug Node 
					ch_addr=0x01;
					node_addr=0x05;
				}
			else if (stricmp(argv[3], "DFEDC_C") == 0){ // DFETail/DC Debug Node 
					ch_addr=0x02;
					node_addr=0x05;
				}
			else if (stricmp(argv[3], "DFEDC_D") == 0){ // DFETail/DC Debug Node 
					ch_addr=0x03;
					node_addr=0x05;
				}
			else if (stricmp(argv[3], "DFEDC_INDEP") == 0){ // DFETail/DC Debug Node 
					ch_addr=0x00;
					node_addr=0x05;
				}
			else if (stricmp(argv[3], "DEC") == 0){ // R1000DEC Debug Node 
					ch_addr=0x00; node_addr=0x07;
				}
			else if (stricmp(argv[3], "CRC_A") == 0){ // R1000CRC Debug Node 
					ch_addr=0x00;
					node_addr=0x06;
				}
			else if (stricmp(argv[3], "CRC_B") == 0){ // R1000CRC Debug Node 
					ch_addr=0x01;
					node_addr=0x06;
				}
			else if (stricmp(argv[3], "CRC_C") == 0){ // R1000CRC Debug Node 
					ch_addr=0x02;
					node_addr=0x06;
				}
			else if (stricmp(argv[3], "CRC_D") == 0){ // R1000CRC Debug Node 
					ch_addr=0x03;
					node_addr=0x06;
				}
			else if (stricmp(argv[3], "AN") == 0){ // Autoneg Debug Node 
					ch_addr=0x00; node_addr=0x0F;
				}
			else if (stricmp(argv[3], "CMI") == 0){ // CMI Debug Node 
					ch_addr=0x03; node_addr=0x0F;
				}
			else if (stricmp(argv[3], "SUPV") == 0){ // SUPV PHY  Debug Node 
					ch_addr=0x00; node_addr=0x0D;
				}
			else {
					printf("				   miir_trdgb all <PhyAddr> <TrRegTyp>(DSPF|PMA|TR|PCS|FFE_x|ECNC_x|DFEDC_x|DEC|CRC_x|AN|CMI|SUPV) <s_addr> <e_addr>\r\n");
					return 0;
				}

				data_addr=data_addr&0x3F;

				//tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
				mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
				//tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
				mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
				//while(!(tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
				//while(!(mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
				{
					//value_low = tcMiiStationRead_ext(phyaddr, Token_Ring_Low_data_reg);
					value_low = mtMiiRegRead(phyaddr, Token_Ring_Low_data_reg);
					//value_high = tcMiiStationRead_ext(phyaddr, Token_Ring_High_data_reg);
					value_high = mtMiiRegRead(phyaddr, Token_Ring_High_data_reg);
					value=value_low+((value_high&0x00FF)<<16);
					printf("* %s => Phyaddr=%d,TrRegTyp =%s,node_addr=0x%02lX,data_addr=0x%04lX,value=0x%06lX\r\n", argv[2], phyaddr, argv[2],node_addr, i, value);
				}
			}
			//tcMiiStationWrite_ext(phyaddr, page_reg, 0x00);//V1.11
			mtMiiRegWrite(phyaddr, page_reg, 0x00);//V1.11
			return 0;
		
    	}
	else
	{
        printf("       CMD Error : miir_trdgb <PhyAddr> <CHAddr> <NodeAddr> <DataAddr>\r\n");
        printf("                   miir_trdgb <PhyAddr> <TrRegTyp>(DSPF|PMA|TR|PCS|FFE_x|ECNC_x|DFEDC_x|DEC|CRC_x|AN|CMI|SUPV) <DataAddr>\r\n");
        printf("                   miir_trdgb all <PhyAddr> <TrRegTyp>(DSPF|PMA|TR|PCS|FFE_x|ECNC_x|DFEDC_x|DEC|CRC_x|AN|CMI|SUPV) <s_addr> <e_addr>\r\n");
        printf("                   <where x=A/B/C/D/INDEP>\r\n");
        return 0;      

	}

}
int doPhyMiiWrite_TrDbg(int argc, char *argv[], void *p)
{
    const uint16 page_reg=31;
    const uint32 Token_Ring_debug_reg=0x52B5;
    const uint32 Token_Ring_Control_reg=0x10;
    const uint32 Token_Ring_Low_data_reg=0x11;
    const uint32 Token_Ring_High_data_reg=0x12;
	
    uint16 phyaddr=0;
    uint16 ch_addr=0;
    uint32 node_addr=0;
    uint32 data_addr=0;
    uint32 value=0;
    uint32 value_high=0;
    uint32 value_low=0;
	
	if(argc ==5){// tce miiw_trdgb <PhyAddr> <TrRegType> <DataAddr> <Value>
	
			phyaddr = checked_atoi(argv[1]);//0~31
			sscanf(argv[3], "%lx", &data_addr);// 6 bit HEX
		   if (strcasecmp(argv[2], "DSPF") == 0){// DSP Filter Debug Node
				   ch_addr=0x02;
				   node_addr=0x0D;
			   }
		   else if (strcasecmp(argv[2], "PMA") == 0){ // PMA Debug Node 
				   ch_addr=0x01;
				   node_addr=0x0F;
			   }
		   else if (strcasecmp(argv[2], "TR") == 0){ // Timing Recovery  Debug Node 
				   ch_addr=0x01;
				   node_addr=0x0D;
			   }
		   else if (strcasecmp(argv[2], "PCS") == 0){ // R1000PCS Debug Node 
				   ch_addr=0x02;
				   node_addr=0x0F;
			   }
			else if (strcasecmp(argv[2], "FFE_A") == 0){ // FFE Debug Node 
					ch_addr=0x00;
					node_addr=0x04;
				}
			else if (strcasecmp(argv[2], "FFE_B") == 0){ // FFE Debug Node 
					ch_addr=0x01;
					node_addr=0x04;
				}
			else if (strcasecmp(argv[2], "FFE_C") == 0){ // FFE Debug Node 
					ch_addr=0x02;
					node_addr=0x04;
				}
			else if (strcasecmp(argv[2], "FFE_D") == 0){ // FFE Debug Node 
					ch_addr=0x03;
					node_addr=0x04;
				}
			else if (strcasecmp(argv[2], "FFE_INDEP") == 0){ // FFE Debug Node 
					ch_addr=0x00;
					node_addr=0x04;
				}
			else if (strcasecmp(argv[2], "ECNC_A") == 0){ // EC/NC Debug Node 
					ch_addr=0x00;
					node_addr=(data_addr&0x40)>>6;
				}
			else if (strcasecmp(argv[2], "ECNC_B") == 0){ // EC/NC Debug Node 
					ch_addr=0x01;
					node_addr=(data_addr&0x40)>>6;
				}
			else if (strcasecmp(argv[2], "ECNC_C") == 0){ // EC/NC Debug Node 
					ch_addr=0x02;
					node_addr=(data_addr&0x40)>>6;
				}
			else if (strcasecmp(argv[2], "ECNC_D") == 0){ // EC/NC Debug Node 
					ch_addr=0x03;
					node_addr=(data_addr&0x40)>>6;
				}
			else if (strcasecmp(argv[2], "ECNC_INDEP") == 0){ // EC/NC Debug Node 
					ch_addr=0x00;
					node_addr=(data_addr&0xC0)>>6;
				}
			else if (strcasecmp(argv[2], "DFEDC_A") == 0){ // DFETail/DC Debug Node 
					ch_addr=0x00;
					node_addr=0x05;
				}
			else if (strcasecmp(argv[2], "DFEDC_B") == 0){ // DFETail/DC Debug Node 
					ch_addr=0x01;
					node_addr=0x05;
				}
			else if (strcasecmp(argv[2], "DFEDC_C") == 0){ // DFETail/DC Debug Node 
					ch_addr=0x02;
					node_addr=0x05;
				}
			else if (strcasecmp(argv[2], "DFEDC_D") == 0){ // DFETail/DC Debug Node 
					ch_addr=0x03;
					node_addr=0x05;
				}
			else if (strcasecmp(argv[2], "DFEDC_INDEP") == 0){ // DFETail/DC Debug Node 
					ch_addr=0x00;
					node_addr=0x05;
				}
		   else if (strcasecmp(argv[2], "DEC") == 0){ // R1000DEC Debug Node 
					ch_addr=0x00; node_addr=0x07;
				}
			else if (strcasecmp(argv[2], "CRC_A") == 0){ // R1000CRC Debug Node 
					ch_addr=0x00;
					node_addr=0x06;
				}
			else if (strcasecmp(argv[2], "CRC_B") == 0){ // R1000CRC Debug Node 
					ch_addr=0x01;
					node_addr=0x06;
				}
			else if (strcasecmp(argv[2], "CRC_C") == 0){ // R1000CRC Debug Node 
					ch_addr=0x02;
					node_addr=0x06;
				}
			else if (strcasecmp(argv[2], "CRC_D") == 0){ // R1000CRC Debug Node 
					ch_addr=0x03;
				   node_addr=0x06;
			   }
		   else if (strcasecmp(argv[2], "AN") == 0){ // Autoneg Debug Node 
				   ch_addr=0x00; node_addr=0x0F;
			   }
		   else if (strcasecmp(argv[2], "CMI") == 0){ // CMI Debug Node 
				   ch_addr=0x03; node_addr=0x0F;
			   }
		   else if (strcasecmp(argv[2], "SUPV") == 0){ // SUPV PHY  Debug Node 
				   ch_addr=0x00; node_addr=0x0D;
			   }
			else {
					printf("				   miiw_trdgb <PhyAddr> <TrRegTyp>(DSPF|PMA|TR|PCS|FFE_x|ECNC_x|DFEDC_x|DEC|CRC_x|AN|CMI|SUPV) <DataAddr> <Value>\r\n");
					return 0;
				}

               data_addr=data_addr&0x3F;
			   sscanf(argv[4], "%lx", &value);// 24 bit //HEX
			   value_high=(0x00FF0000&value)>>16;
			   value_low=(0x0000FFFF&value);
		   
			   //tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
			   mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
				   
			   //tcMiiStationWrite_ext(phyaddr, Token_Ring_Low_data_reg, value_low);
			   mtMiiRegWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
			   //tcMiiStationWrite_ext(phyaddr, Token_Ring_High_data_reg, value_high);
			   mtMiiRegWrite(phyaddr, Token_Ring_High_data_reg, value_high);
			   //tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
			   mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
			   //while(!(tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
			   //while(!(mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
			   {
				   printf("* %s => Phyaddr=%d, ch_addr=%d, node_addr=0x%02lX, data_addr=0x%04lX , value=0x%06lX\r\n",argv[2], phyaddr, ch_addr, node_addr, data_addr, value);
			   }
			   //tcMiiStationWrite_ext(phyaddr, page_reg, 0x00);//V1.11
			   mtMiiRegWrite(phyaddr, page_reg, 0x00);//V1.11
			   return 0;
	 }
	else if (argc==6){
    		// tce miiw_trdgb <PhyAddr> <CHAddr> <NodeAddr> <DataAddr> <Value>
    		phyaddr = checked_atoi(argv[1]);//0~31
    		ch_addr = checked_atoi(argv[2]);//0:A pair  /1:B pair  /2:C pair  /3:D pair 
    		sscanf(argv[3], "%lx", &node_addr);// 4 bit //HEX
    		sscanf(argv[4], "%lx", &data_addr);// 6 bit //HEX
    		sscanf(argv[5], "%lx", &value);// 24 bit //HEX
        value_high=(0x00FF0000&value)>>16;
        value_low=(0x0000FFFF&value);

        //tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
        mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
			
        //tcMiiStationWrite_ext(phyaddr, Token_Ring_Low_data_reg, value_low);
        mtMiiRegWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
        //tcMiiStationWrite_ext(phyaddr, Token_Ring_High_data_reg, value_high);
        mtMiiRegWrite(phyaddr, Token_Ring_High_data_reg, value_high);
        //tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
        mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(ch_addr<<11)|(node_addr<<7)|(data_addr<<1));
        //while(!(tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
        //while(!(mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)); // data ready
		{
			printf("* miiw_trdgb ready => Phyaddr=%d, ch_addr=%d, node_addr=0x%02lX, data_addr=0x%04lX , value=0x%06lX\r\n", phyaddr, ch_addr, node_addr, data_addr, value);
		}
		//tcMiiStationWrite_ext(phyaddr, page_reg, 0x00);//V1.11
		mtMiiRegWrite(phyaddr, page_reg, 0x00);//V1.11
		return 0;
	}
	else
	{
        printf("       CMD Error : miiw_trdgb <PhyAddr> <CHAddr> <NodeAddr> <DataAddr> <Value>\r\n");
        printf("                   miiw_trdgb <PhyAddr> <TrRegTyp>(DSPF|PMA|TR|PCS|FFE_x|ECNC_x|DFEDC_x|DEC|CRC_x|AN|CMI|SUPV) <DataAddr> <Value>\r\n");
        printf("                   <where x=A/B/C/D/INDEP>\r\n");
        return 0;      

	}
	
}
//uint32 mtPhyMiiRead_TrDbg(uint8 phyaddr, char *type, uint32 reg, uint8 ch_num);

int doReadPhycoeff(int argc, char *argv[], void *p)//coeffr <PhyAddr> <FFE/DEC/EC/NC/TR/DFE>
{
 
 uint16 phyaddr=0;
 uint32 value;
 uint8 i,j;
 uint8 sw_patch_flag_bak;//V1.11
 char *ch_tbl[4]={"A","B","C","D"};

  sw_patch_flag_bak=sw_patch_flag;//V1.11
  if (argc==3){
          sw_patch_flag = 0;
          mtPhyMiiWrite_TrDbg(phyaddr, "PMA",4,0x60000,0);//V1.11  force freeze DSP
          phyaddr = checked_atoi(argv[1]);
          printf("* PhyAddr=%d\r\n", phyaddr);
          if ((stricmp(argv[2], "ffe") == 0) ||(stricmp(argv[2], "FFE") == 0)){
            for(j=0;j<4;j++){//chA~chD
              for(i=0;i<16;i++){
                  value = mtPhyMiiRead_TrDbg(phyaddr, "FFE", i,j);
                  printf("* FFE_%02xh CH%s value=0x%04lX\r\n", i,ch_tbl[j], value);
                }
              }
         }
          else if((stricmp(argv[2], "dec") == 0) ||(stricmp(argv[2], "DEC") == 0)){
            
            for(j=0;j<4;j++){
              for(i=0;i<3;i++){
                  value = mtPhyMiiRead_TrDbg(phyaddr, "DEC", (4*j+i),0); 
                    printf("* DEC_%02xh CH%s, value=0x%04lX\r\n",(4*j+i),ch_tbl[j],value);
                }
              }
          } 
          else if((stricmp(argv[2], "ec") == 0) ||(stricmp(argv[2], "EC") == 0)){
            for(j=0;j<4;j++){
              for(i=0;i<64;i++){
                  value = mtPhyMiiRead_TrDbg(phyaddr, "EC", i, j); 
                    printf("* EC[%02d], CH%s value=0x%04lX\r\n", i,ch_tbl[j],value);
                }
              for(i=0;i<16;i++){
                  value = mtPhyMiiRead_TrDbg(phyaddr, "ECT", i, j); 
                  //printf("* PhyAddr=%d, ECTail[%02d], CH%s value=0x%04lX\r\n", i,ch_tbl[j],value);
                  printf("* PhyAddr=%d, ECTail[%02d], CH%s value=0x%04lX\r\n", phyaddr,i,ch_tbl[j],value);
                }
              }
          } 
          else if((stricmp(argv[2], "nc") == 0) ||(stricmp(argv[2], "NC") == 0)){
              for(j=0;j<4;j++)
              {
                for(i=16;i<32;i++)
                {
                  value = mtPhyMiiRead_TrDbg(phyaddr, "NC", i, j); 
                    printf("* NC1[%02d], CH%s value=0x%04lX\r\n", i,ch_tbl[j],value);
                }
                for(i=32;i<48;i++)
                {
                  value = mtPhyMiiRead_TrDbg(phyaddr, "NC", i, j); 
                    printf("* NC2[%02d], CH%s value=0x%04lX\r\n", i,ch_tbl[j],value);
                }
                for(i=48;i<64;i++)
                {
                  value = mtPhyMiiRead_TrDbg(phyaddr, "NC", i, j); 
                    printf("* NC3[%02d], CH%s value=0x%04lX\r\n", i,ch_tbl[j],value);
                }  
              }
          }
          else if((stricmp(argv[2], "tr") == 0) ||(stricmp(argv[2], "TR") == 0)){
              for(i=0;i<16;i++){
                  value = mtPhyMiiRead_TrDbg(phyaddr, "NC", i, 0); 
                    printf("* NC[%02d], CH%s value=0x%04lX\r\n", i,ch_tbl[j],value);
                }
            }
            else if((stricmp(argv[2], "dfe") == 0) ||(stricmp(argv[2], "DFE") == 0)){
              for(j=0;j<4;j++){
				  for(i=0;i<3;i++){//DFE tap0/1/2
					  value = mtPhyMiiRead_TrDbg(phyaddr, "DEC", (4*j+i), j); 
						printf("* DFETap[%02d], CH%s value=0x%06lX\r\n", i,ch_tbl[j],value);
					}
				  
				}
              for(j=0;j<4;j++){
                for(i=0;i<16;i++){//DFE Tail
                    value = mtPhyMiiRead_TrDbg(phyaddr, "DFEDC", i, j); 
                      printf("* DFEDC[%02d], CH%s value=0x%04lX\r\n", i,ch_tbl[j],value);
                  }
				
              }
          }
	mtPhyMiiWrite_TrDbg(phyaddr, "PMA",4,0x40000,0);//V1.11  un-force freeze DSP
	sw_patch_flag=sw_patch_flag_bak;//V1.11
 }  
  else { // error message
      printf("Usage: coeffr <PhyAddr> <ffe/dec/ec/nc/tr/dfe>\r\n");
      return 0;           
  }
  // check parameters
  return 0;           

}

#ifdef MTPHY_DEBUG //MT7530

static int doPhyDispFlag (int argc, char *argv[], void *p)
{
    int i;
    if( argc >= 2 && argc <= 9 ) {
        tcPhy_disp_level=0;
        
        for(i=1; i<argc; i++) {
            if (stricmp(argv[i], "on") == 0){
                tcPhy_disp_level++;
            }
        }
    }   
    
    printf("tcPhy display level: %d.\r\n", tcPhy_disp_level);
    return 0;
}
static int doSkewDispFlag (int argc, char *argv[], void *p)
{
	if( argc >= 2 && argc <= 9 ) {
		mtSkewCal_disp_level=0;
        
		if (stricmp(argv[1], "on") == 0){
			mtSkewCal_disp_level++;
		}
	}   
    
    printf("1000M Skew cal SW patch display level: %d.\r\n", mtSkewCal_disp_level);
    return 0;
}


int doPhyChkVal (int argc, char *argv[], void *p)
{
    phychkval_flag = 1; 
//tcPhyVer = mtPhyVer_7530;
    if (argc == 2) { // set port_num for tcPhyChkVal()
        switch (tcPhyVer) {
		case tcPhyVer_2104mc:
		case tcPhyVer_2104sd:
		case tcPhyVer_2104me:	
			phychkval_portnum= checked_atoi(argv[1]);
			break;
		case tcPhyVer_2105sg:
			phychkval_portnum= checked_atoi(argv[1]);
			break;
		case tcPhyVer_2105mj:
			phychkval_portnum= checked_atoi(argv[1]);
			break;	
		case tcPhyVer_2105sk:
			phychkval_portnum= checked_atoi(argv[1]);
			break;
		case mtPhyVer_7530:	
			phychkval_portnum= checked_atoi(argv[1]);
		default:
			break;
        }
    }
    mtPhyChkVal();
    return 0;
}

void mtPhyChkVal (void)
{
    uint32 port_num = phychkval_portnum; 
    if(phychkval_flag){
        uint32 val;
        uint32 mr02,mr03;
        uint8 mr_anen, mr_dplx, mr_speed;
        uint8 mr_an_capable;
        uint8 mr_lp_an_capable;
        uint8 mr_lp_anen;
        //uint8 lr_force_mdix;
        uint8 lr_linkup, lr_speed, lr_dplx, lr_mdix;
	// show phy_id
	
  mtMiiRegWrite(port_num, 31, 0);
  
	val = mtMiiRegRead(port_num, 2); 
	mr02 = val&(0xffff);
	val = mtMiiRegRead(port_num, 3); 
	mr03 = val&(0xffff);
	printf(" mtPhy ID: %lx %lx\r\n",mr02,mr03); 
	//if (mr02 != 0x03a2)
	//return;
	// reg0h
	
	val = mtMiiRegRead(port_num, 0);
	mr_anen = (val>>12)&0x01;
	mr_dplx = (val>>8)&0x01;
	//mr_speed = (val>>13)&0x01;
	mr_speed = ((val>>13)|(val>>5))&0x03;//00:10,01:100,10:1000
	// reg4h
	val = mtMiiRegRead(port_num, 0x04);
	mr_an_capable = (val>>5)&0x0F; //100F,100H,10F,10H

	// reg9h
	val = mtMiiRegRead(port_num, 0x09);
	mr_an_capable = mr_an_capable | ((val>>4)&0x30);//1000F,1000H

	// reg5h
	val = mtMiiRegRead(port_num, 0x05);
	mr_lp_an_capable = (val>>5)&0x0F; //100F,100H,10F,10H
	// regAh
	val = mtMiiRegRead(port_num, 0x0A);
	mr_lp_an_capable = mr_lp_an_capable | ((val>>6)&0x30); //1000F,1000H

	// reg6h
	val = mtMiiRegRead(port_num, 0x06);
	mr_lp_anen = (val)&0x01;
	#if 1 //MT7530 
	val = mtPhyReadGReg(port_num,0x1E,0xA2);
  
    printf(" tcPhyChkVal= 0x%04lx",val);
  
	lr_linkup = (val>>9)&0x07;//dev1e_A2[9]:LinkStatus10_OK,dev1e_A2[10]:LinkStatus100_OK,dev1e_A2[11]:LinkStatus1000_OK
	lr_speed = (val>>1)&0x07; //dev1e_A2[1]:final_speed_10,,dev1e_A2[2]:dev1e_A2[2]:final_speed_100,dev1e_A2[3]:final_speed_1000
	lr_dplx = (val>>5)&0x01;
	lr_mdix = (val>>6)&0x01;

	#else

	// l0reg28
	val = tcPhyReadReg(port_num, 28);
	lr_linkup = (val)&0x01;
	lr_speed = (val>>1)&0x01;
	lr_dplx = (val>>2)&0x01;
	lr_mdix = (val>>5)&0x01;
	#endif
			
	printf(" TcPhy mode:");
	if(mr_anen) { // Auto-neg
	    printf(" AN-(");
	    printf(" %s,",((mr_an_capable>>5)&0x01)?"1000F":"");
	    printf(" %s,",((mr_an_capable>>4)&0x01)?"1000H":"");
	    printf(" %s,",((mr_an_capable>>3)&0x01)?"100F":"");
	    printf(" %s,",((mr_an_capable>>2)&0x01)?"100H":"");     
	    printf(" %s,",((mr_an_capable>>1)&0x01)?"10F":"");      
	    printf(" %s)\r\n",((mr_an_capable>>0)&0x01)?"10H":"");  
	}
	else { // Force-speed
	    //printf(" Force-%d%s\r\n",
	            //(mr_speed?100:10),(mr_dplx?"F":"H"));
		 if(mr_speed==0x00){
		 	printf(" Force-%d%s\r\n",10,(mr_dplx?"F":"H"));
			}
		 else if(mr_speed==0x01){
			 printf(" Force-%d%s\r\n",100,(mr_dplx?"F":"H"));
		 	}
		 
		 else if(mr_speed==0x02){
			 printf(" Force-%d%s\r\n",1000,(mr_dplx?"F":"H"));
		 	}
	}
		
		if(!lr_linkup) { // link-down
		printf(" *** Link is down!\r\n");
		// ADC sum
    val=mt7530GePhyReadProbe(port_num,Probe_VgaState,0);
		printf("VgaStateA =0x%x\r\n",((val>>4) & 0x1F));
		printf("VgaStateB =0x%x\r\n",((val>>9) & 0x1F));
		printf("VgaStateC =0x%x\r\n",((val>>14) & 0x1F));
		printf("VgaStateD =0x%x\r\n",((val>>19) & 0x1F));
		
    val=mt7530GePhyReadProbe(port_num,Probe_MSE,0);
		printf("lch_mse_mdcA =0x%x\r\n",val);
    val=mt7530GePhyReadProbe(port_num,Probe_MSE,1);
		printf("lch_mse_mdcB =0x%x\r\n",val);
	val=mt7530GePhyReadProbe(port_num,Probe_MSE,2);
		printf("lch_mse_mdcC =0x%x\r\n",val);
	val=mt7530GePhyReadProbe(port_num,Probe_MSE,3);
		printf("lch_mse_mdcD =0x%x\r\n",val);

    val=mt7530GePhyReadProbe(port_num,Probe_MSE_Slicer_err_thres,0);
		printf("slicer_err_thres =0x%x\r\n",val);
    val=mt7530GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum,0);
		printf("ProbeSlicerErrOverSumA =0x%x\r\n",val);
	val=mt7530GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum,1);
		printf("ProbeSlicerErrOverSumB =0x%x\r\n",val);
	val=mt7530GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum,2);
		printf("ProbeSlicerErrOverSumC =0x%x\r\n",val);
	val=mt7530GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum,3);
		printf("ProbeSlicerErrOverSumD =0x%x\r\n",val);
		#if 0
		if(tcPhyVer==tcPhyVer_2101mb){
			#ifdef TC2101MB_SUPPORT
			printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
			port_num, tc2101mbReadAdcSum(port_num));            
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2104mc){
			#ifdef TC2104MC_SUPPORT
			printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
			port_num, tc2104mcReadAdcSum(port_num));            
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2104sd){
			#ifdef TC2104SD_SUPPORT
			printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
			port_num, tc2104sdReadAdcSum(port_num));            
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2101me){
			#ifdef TC2101ME_SUPPORT
			printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
			port_num, tc2101meReadAdcSum(port_num));            
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2102me){
			#ifdef TC2102ME_SUPPORT
			printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
			mac_p->enetPhyAddr, tc2102meReadAdcSum(port_num));			
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2104me){
			#ifdef TC2104ME_SUPPORT
			printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
			port_num, tc2104meReadAdcSum(port_num));            
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2101mf){
			#ifdef TC2101MF_SUPPORT
			printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
			port_num, tc2101mfReadAdcSum(port_num));            
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2105sg){
			#ifdef TC2105SG_SUPPORT
			printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
			port_num, tc2105sgReadAdcSum(port_num));            
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2101mi){
			#ifdef TC2101MI_SUPPORT
			printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
			port_num, tc2101miReadAdcSum(port_num));            
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2105mj){
			#ifdef TC2105MJ_SUPPORT
			printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
			port_num, tc2105mjReadAdcSum(port_num));            
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2105sk){
			#ifdef TC2105SK_SUPPORT
			printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
			port_num, tc2105skReadAdcSum(port_num));            
			#endif
		} 
            // dev1E_RA2 message
             #endif
            if (mr1E_A2.lch_SignalDetect || mr1E_A2.lch_LinkPulse
                || mr1E_A2.final_speed_1000 || mr1E_A2.final_speed_100|| mr1E_A2.final_speed_10)
            {
		TCPHYDISP4(" tcphy[%ld]: ",port_num);
		if (mr1E_A2.lch_SignalDetect) 
			TCPHYDISP4("SigDet ");
		if (mr1E_A2.lch_LinkPulse)
			TCPHYDISP4("RxLkp ");
		if (mr1E_A2.final_speed_1000){					
			if(mr1E_A2.MSConfig1000){
				TCPHYDISP4("Up1000 Master ");}
			else{
				TCPHYDISP4("Up1000 Slave ");}
		}
		if (mr1E_A2.final_speed_100)
			TCPHYDISP4("Up100 ");
		if (mr1E_A2.final_speed_10)
			TCPHYDISP4("Up10 ");
		if (mr1E_A2.da_mdix){
			TCPHYDISP4("UpMdix ");}
		else{
			TCPHYDISP4("UpMdi ");}

			TCPHYDISP4("\r\n");
            }
        }
	else { // link-up
	//printf(" TCphy is link-up at %d%s ",
	    //(lr_speed?100:10),(lr_dplx?"F":"H"));
	   
		if(lr_speed == 0x04)
		printf(" TCphy is link-up at 1000 %s ",(mr1E_A2.MSConfig1000?"Master":"Slave"));
		else if(lr_speed == 0x02){
		printf(" TCphy is link-up at 100 %s ",(lr_dplx?"F":"H"));}
		else if(lr_speed == 0x01){	
		printf(" TCphy is link-up at 10 %s ",(lr_dplx?"F":"H"));}

	//if(tcPhyVer==tcPhyVer_2031){
	//    #ifdef TC2031_SUPPORT
	//        printf("via %s\r\n",tc2031_cfg[current_idx].name);
	//    #endif
	//}
	else {
	printf(".\r\n");
	}
            
            if(mr_lp_anen) {
                printf(" Link-partner supports AN-(");
                printf(" %s,",((mr_lp_an_capable>>5)&0x01)?"1000F":"");
                printf(" %s,",((mr_lp_an_capable>>4)&0x01)?"1000H":"");
                printf(" %s,",((mr_lp_an_capable>>3)&0x01)?"100F":"");
                printf(" %s,",((mr_lp_an_capable>>2)&0x01)?"100H":"");  
                printf(" %s,",((mr_lp_an_capable>>1)&0x01)?"10F":"");       
                printf(" %s)\r\n",((mr_lp_an_capable>>0)&0x01)?"10H":"");   
            }
            else {
                printf(" Link-partner operates in Force mode.\r\n");
            }

            printf(" %s,",(lr_mdix?"mdix":"mdi"));
            if (tcPhyVer==tcPhyVer_2104mc
                || tcPhyVer==tcPhyVer_2104sd
                || tcPhyVer==tcPhyVer_2101me
				|| tcPhyVer==tcPhyVer_2102me
                || tcPhyVer==tcPhyVer_2104me
                || tcPhyVer==tcPhyVer_2101mf
                || tcPhyVer==tcPhyVer_2105sg
                || tcPhyVer==tcPhyVer_2101mi
                || tcPhyVer==tcPhyVer_2105mj
                || tcPhyVer==tcPhyVer_2105sk)
            {
                printf(" tx_amp_save=%d\r\n",(mr28.tx_amp_save));
            }           
            printf("\r\n");
            
		if(lr_speed>=0x02) { // 100Mbps/1000Mbps
		//mt7530GePhyReadProbe(port_num,Probe_VgaState,0);
		//mt7530GePhyReadProbe(port_num,Probe_MSE_ALL,0);
		//mt7530GePhyReadProbe(port_num,Probe_MSE_Slicer_err_thres,0);
	    //   mt7530GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum_ALL,0);
		val=mt7530GePhyReadProbe(port_num,Probe_VgaState,0);
			printf("VgaStateA =0x%x\r\n",((val>>4) & 0x1F));
			printf("VgaStateB =0x%x\r\n",((val>>9) & 0x1F));
			printf("VgaStateC =0x%x\r\n",((val>>14) & 0x1F));
			printf("VgaStateD =0x%x\r\n",((val>>19) & 0x1F));
			
		val=mt7530GePhyReadProbe(port_num,Probe_MSE,0);
			printf("lch_mse_mdcA =0x%x\r\n",val);
		val=mt7530GePhyReadProbe(port_num,Probe_MSE,1);
			printf("lch_mse_mdcB =0x%x\r\n",val);
		val=mt7530GePhyReadProbe(port_num,Probe_MSE,2);
			printf("lch_mse_mdcC =0x%x\r\n",val);
		val=mt7530GePhyReadProbe(port_num,Probe_MSE,3);
			printf("lch_mse_mdcD =0x%x\r\n",val);
			
		val=mt7530GePhyReadProbe(port_num,Probe_MSE_Slicer_err_thres,0);
			printf("slicer_err_thres =0x%x\r\n",val);
		val=mt7530GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum,0);
			printf("ProbeSlicerErrOverSumA =0x%x\r\n",val);
		val=mt7530GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum,1);
			printf("ProbeSlicerErrOverSumB =0x%x\r\n",val);
		val=mt7530GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum,2);
			printf("ProbeSlicerErrOverSumC =0x%x\r\n",val);
		val=mt7530GePhyReadProbe(port_num,Probe_Slicer_Err_Over_Sum,3);
			printf("ProbeSlicerErrOverSumD =0x%x\r\n",val);

	
		#if 0
			if(tcPhyVer==tcPhyVer_2031){                    
			#ifdef TC2031_SUPPORT
			tc2031DispProbe100();
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2101mb){
			#ifdef TC2101MB_SUPPORT
			tc2101mbDispProbe100(port_num);
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2104mc){
			#ifdef TC2104MC_SUPPORT
			tc2104mcDispProbe100(port_num,1); // long message
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2104sd){
			#ifdef TC2104SD_SUPPORT
			tc2104sdDispProbe100(port_num,1);
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2101me){
			#ifdef TC2101ME_SUPPORT
			tc2101meDispProbe100(port_num);
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2102me){
			#ifdef TC2102ME_SUPPORT
			tc2102meDispProbe100(port_num,1);
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2104me){
			#ifdef TC2104ME_SUPPORT
			tc2104meDispProbe100(port_num,1); // long message
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2101mf){
			#ifdef TC2101MF_SUPPORT
			tc2101mfDispProbe100(port_num);
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2105sg){
			#ifdef TC2105SG_SUPPORT
			tc2105sgDispProbe100(port_num,1);
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2101mi){
			#ifdef TC2101MI_SUPPORT
			tc2101miDispProbe100(port_num,1);
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2105mj){
			#ifdef TC2105MJ_SUPPORT
			tc2105mjDispProbe100(port_num,1);
			#endif
		}
		else if(tcPhyVer==tcPhyVer_2105sk){
			#ifdef TC2105SK_SUPPORT
			tc2105skDispProbe100(port_num,1);
			#endif
		}
		#endif
		printf("\r\n");
		}   
        }
    }
    phychkval_flag = 0;
}


int doPhyMMDRead(int argc, char *argv[], void *p)
{
    #ifndef CL45_CMD_SUPPORT
    const uint16 MMD_Control_register=0xD;
    const uint16 MMD_addr_data_register=0xE;
  	const uint16 page_reg=31;
    #endif
	
	uint32 dev_addr=0;
	uint32 phyaddr=0;
	uint32 reg_addr=0;
	uint32 start_addr=0;
	uint32 end_addr=0;
	uint32 value=0;
	int i=0;


	if (argc!=4 && argc!=6){
		printf("Usage: emiir all <phyaddr> <devaddr> <s_addr> <e_addr>\r\n");
		printf("       emiir <phyaddr> <devaddr> <reg>\r\n");
		return 0;			
	}


	if (strcasecmp(argv[1], "all") == 0){ /*Post Read*/
		phyaddr = checked_atoi(argv[2]);//0~31
		sscanf(argv[3], "%lx", &dev_addr);//hex
		sscanf(argv[4], "%lx", &start_addr);//hex
		sscanf(argv[5], "%lx", &end_addr);//hex
		if((start_addr>65535)  ||(end_addr>65535)){
			printk("s_addr or e_addr must be less than 65536\r\n");
			return 0;
		}
		for(i=start_addr; i<=end_addr; i++)
		{
		#ifdef CL45_CMD_SUPPORT
			//value=tcMiiExtStationRead_ext(phyaddr,dev_addr,i,NORMAL_READ);
			value=mtEMiiRegRead(phyaddr,dev_addr,i);
			printf("* doPhyMMDRead =>phyaddr=%d,  dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr,  i, value);
		#else
			//tcMiiStationWrite_ext(phyaddr, page_reg, 0x00); //switch to main page
			mtMiiRegWrite(phyaddr, page_reg, 0x00); //switch to main page
			//tcMiiStationWrite_ext(phyaddr, MMD_Control_register, (0<<14)+dev_addr);
			mtMiiRegWrite(phyaddr, MMD_Control_register, (0<<14)+dev_addr);
			//tcMiiStationWrite_ext(phyaddr, MMD_addr_data_register, i);
			mtMiiRegWrite(phyaddr, MMD_addr_data_register, i);
			//tcMiiStationWrite_ext(phyaddr, MMD_Control_register, (1<<14)+dev_addr);
			mtMiiRegWrite(phyaddr, MMD_Control_register, (1<<14)+dev_addr);
			
			//value = tcMiiStationRead_ext(phyaddr, MMD_addr_data_register);
			value = mtMiiRegRead(phyaddr, MMD_addr_data_register);
			
			printf("* doPhyMMDRead_CL22 =>phyaddr=%d,  dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr,  i, value);
		#endif
			//printf("* doPhyMMDRead =>phyaddr=%d,  dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr,  i, value);
		}
		printk("\r\n");

	}
    else
	{
		// tce emiir <DevAddr> <PhyAddr> <regAddr>
		phyaddr = checked_atoi(argv[1]);//0~31
		//dev_addr = checked_atoi(argv[2]);//0~31
		sscanf(argv[2], "%lx", &dev_addr);//hex
		sscanf(argv[3], "%lx", &reg_addr);//hex
		#ifdef CL45_CMD_SUPPORT
			//value=tcMiiExtStationRead_ext(phyaddr,dev_addr,reg_addr,NORMAL_READ);
			value=mtEMiiRegRead(phyaddr,dev_addr,reg_addr);
			printf("* doPhyMMDRead =>phyaddr=%d,  dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr, reg_addr, value);
		#else
			//tcMiiStationWrite_ext(phyaddr, page_reg, 0x00); //switch to main page
			mtMiiRegWrite(phyaddr, page_reg, 0x00); //switch to main page
			//tcMiiStationWrite_ext(phyaddr, MMD_Control_register, (0<<14)+dev_addr);
			mtMiiRegWrite(phyaddr, MMD_Control_register, (0<<14)+dev_addr);
			//tcMiiStationWrite_ext(phyaddr, MMD_addr_data_register, reg_addr);
			mtMiiRegWrite(phyaddr, MMD_addr_data_register, reg_addr);
			//tcMiiStationWrite_ext(phyaddr, MMD_Control_register, (1<<14)+dev_addr);
			mtMiiRegWrite(phyaddr, MMD_Control_register, (1<<14)+dev_addr);
			
			//value = tcMiiStationRead_ext(phyaddr, MMD_addr_data_register);
			value = mtMiiRegRead(phyaddr, MMD_addr_data_register);
			
			printf("* doPhyMMDRead_CL22=>phyaddr=%d,  dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr, reg_addr, value);
		#endif
			//printf("* doPhyMMDRead =>phyaddr=%d,  dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr, reg_addr, value);
		return 0;
    }
    return 0;           

}

#if 0
int doPhyMMDWrite(int argc, char *argv[], void *p)
{
    #ifndef CL45_CMD_SUPPORT
    const uint16 MMD_Control_register=0xD;
    const uint16 MMD_addr_data_register=0xE;
    const uint16 page_reg=31;
    #endif
    uint32 dev_addr=0;
    uint32 phyaddr=0;
    uint32 reg_addr=0;
    uint32 value=0;

	if (argc==5)
	{
		// tce emiiw <DevAddr> <PhyAddr> <regAddr> <data>
		phyaddr = checked_atoi(argv[1]);//0~31
		dev_addr = checked_atoi(argv[2]);//0~31
		sscanf(argv[3], "%lx", &reg_addr);//hex
		sscanf(argv[4], "%lx", &value);//hex
		#ifdef CL45_CMD_SUPPORT
		//tcMiiExtStationWrite_ext(phyaddr, dev_addr,reg_addr,value);
		mtEMiiRegWrite(phyaddr, dev_addr,reg_addr,value);
		#else
		//tcMiiStationWrite_ext(phyaddr, page_reg, 0x00); //switch to main page
		mtMiiRegWrite(phyaddr, page_reg, 0x00); //switch to main page
		//tcMiiStationWrite_ext(phyaddr, MMD_Control_register, (0<<14)+dev_addr);
		mtMiiRegWrite(phyaddr, MMD_Control_register, (0<<14)+dev_addr);
		//tcMiiStationWrite_ext(phyaddr, MMD_addr_data_register, reg_addr);
		mtMiiRegWrite(phyaddr, MMD_addr_data_register, reg_addr);
		//tcMiiStationWrite_ext(phyaddr, MMD_Control_register, (1<<14)+dev_addr);
		mtMiiRegWrite(phyaddr, MMD_Control_register, (1<<14)+dev_addr);
		//tcMiiStationWrite_ext(phyaddr, MMD_addr_data_register, value);
		mtMiiRegWrite(phyaddr, MMD_addr_data_register, value);
		#endif
		printf("* doPhyMMDWrite => dev_addr=%d, phyaddr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", dev_addr, phyaddr, reg_addr, value);
		return 0;
    }
	else
	{
        printf("       CMD Error : emiiw <PhyAddr> <DevAddr> <regAddr> <data>\r\n");
        return 0;      

	}

}

#else
int doPhyMMDWrite(int argc, char *argv[], void *p)
{
    #ifndef CL45_CMD_SUPPORT
    const uint16 MMD_Control_register=0xD;
    const uint16 MMD_addr_data_register=0xE;
    const uint16 page_reg=31;
    #endif
	uint32 dev_addr=0;
	uint32 phyaddr=0;
	uint32 reg_addr=0;
	uint32 value=0;
	uint16 STBit=0;
	uint16 BFlen=0;
	uint16 BF,BFMsk=0;
	uint32 RValue =0;
	uint8 i=0;


	if (argc==5)
	{
		// tce emiiw <DevAddr> <PhyAddr> <regAddr> <data>
		phyaddr = checked_atoi(argv[1]);//0~31
		//dev_addr = checked_atoi(argv[2]);//0~31
		sscanf(argv[2], "%lx", &dev_addr);//hex
		sscanf(argv[3], "%lx", &reg_addr);//hex
		sscanf(argv[4], "%lx", &value);//hex

    }
	else if (argc==7){
		if(stricmp(argv[0], "emiiwb") == 0){
			phyaddr = checked_atoi(argv[1]);
			//reg = checked_atoi(argv[2]);
			sscanf(argv[2], "%lx", &dev_addr);
			sscanf(argv[3], "%lx", &reg_addr);
			STBit = checked_atoi(argv[4]);
			BFlen = checked_atoi(argv[5]);
			sscanf(argv[6], "%lx", &value);
			//printf("* Phyaddr=%d, DevAddr=0x%02lX,RegAddr=0x%02lX, STBit=%0d, BFlen=%0d, value=0x%04lX\r\n", phyaddr,dev_addr, reg_addr, STBit, BFlen, value);		
			
			}
		}
	
 	 if (((argc == 5) || (argc == 7)) && (phyaddr<=0x1f)){
 		     if(argc == 5){
 		     	
        #ifdef CL45_CMD_SUPPORT
				//tcMiiExtStationWrite_ext(phyaddr, dev_addr,reg_addr,value);
				mtEMiiRegWrite(phyaddr, dev_addr,reg_addr,value);
				printf("* doPhyMMDWrite => phyaddr=%d, dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr, reg_addr, value);
        #else
				//tcMiiStationWrite_ext(phyaddr, page_reg, 0x00); //switch to main page
				mtMiiRegWrite(phyaddr, page_reg, 0x00); //switch to main page
				//tcMiiStationWrite_ext(phyaddr, MMD_Control_register, (0<<14)+dev_addr);
				mtMiiRegWrite(phyaddr, MMD_Control_register, (0<<14)+dev_addr);
				//tcMiiStationWrite_ext(phyaddr, MMD_addr_data_register, reg_addr);
				mtMiiRegWrite(phyaddr, MMD_addr_data_register, reg_addr);
				//tcMiiStationWrite_ext(phyaddr, MMD_Control_register, (1<<14)+dev_addr);
				mtMiiRegWrite(phyaddr, MMD_Control_register, (1<<14)+dev_addr);
				//tcMiiStationWrite_ext(phyaddr, MMD_addr_data_register, value);
				mtMiiRegWrite(phyaddr, MMD_addr_data_register, value);
				printf("* doPhyMMDWrite_CL22=> phyaddr=%d, dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr, reg_addr, value);
        #endif
				//printf("* doPhyMMDWrite => phyaddr=%d, dev_addr=%d, data_addr=0x%04lX , value=0x%04lX\r\n", phyaddr, dev_addr, reg_addr, value);
 		     	}
 		     else if(((argc == 7) && (stricmp(argv[0], "emiiwb")) == 0)){
				//RValue = tcMiiExtStationRead_ext(phyaddr,dev_addr, reg_addr,NORMAL_READ);
				RValue = mtEMiiRegRead(phyaddr,dev_addr, reg_addr);
				for(i=0;i<BFlen;i++){
				BF=1;
				BF=BF<<(STBit+i);
				BFMsk = BFMsk | BF;
				//printf("1: BF=%d, BFMsk=%04x\r\n", BF, BFMsk);
				}
				BFMsk = ~BFMsk;
				//printf("2: BF=%d, BFMsk=%04x\r\n", BF, BFMsk); 
				value = (RValue & BFMsk) | (value<<STBit);
				//printf("* Phyaddr=%d, RegAddr=0x%02lX, OrgValue=0x%04lX, Modified value=0x%04lX\r\n", phyaddr, reg,  RValue, value);
			  	printf("* Phyaddr=%d, DevAddr=0x%02lX,RegAddr=0x%02lX, STBit=%0d, BFlen=%0d, value=0x%04lX\r\n", phyaddr,dev_addr, reg_addr, STBit, BFlen, value);		
				printf("* OrgValue=0x%04lX,",RValue);
				//tcMiiExtStationWrite_ext(phyaddr, dev_addr,reg_addr, value);
				mtEMiiRegWrite(phyaddr, dev_addr,reg_addr, value);
				//RValue = tcMiiExtStationRead_ext(phyaddr,dev_addr, reg_addr,NORMAL_READ);
				RValue = mtEMiiRegRead(phyaddr,dev_addr, reg_addr);
				printf(" ModValue=0x%04lX\r\n", RValue);
 		     	}
 		     	
		return 0;

		}
	else	{
        printf("       CMD Error : emiiw <PhyAddr> <DevAddr> <regAddr> <data>\r\n");
	 printf("                         emiiwb <PhyAddr> <DevAddr> <RegAddr> <STBit> <BFLen> <BFVal> \r\n");	
        return 0;      

	}

}
#endif


int doLpbk_setting(int argc, char *argv[], void *p)
{
    //tce lpbk <10M> <phyaddr> <farend/di/digital/analog> <on/off>
    //tce lpbk <100M> <phyaddr> <farend/digital/analog> <on/off>
    //tce lpbk <1000M> <phyaddr> <farend/digital/analog> <on/off>
	const uint16 page_reg=31;
	const uint16 TestPage=0x2A30;
	const uint16 ExtendPage=0x0001;
	const uint32 Token_Ring_debug_reg=0x52B5;
	const uint32 Token_Ring_Control_reg=0x10;
	const uint32 Token_Ring_Low_data_reg=0x11;
	const uint32 Token_Ring_High_data_reg=0x12;
	const uint16 Token_Ring_CH_addr_PCS=0x2;
	const uint16 Token_Ring_Node_addr_PCS=0xF;

	//const uint16 MainPage=0x00;
	uint32 value;
	uint32 value_high=0;
	uint32 value_low=0;

	uint16 phyaddr=0;

	if (argc==5)
	{
		phyaddr = checked_atoi(argv[2]);//0~31

		if ((strcasecmp(argv[1], "10M") == 0)&&((strcasecmp(argv[4], "on") == 0)||(strcasecmp(argv[4], "off") == 0)))
		{
			if (strcasecmp(argv[3], "farend") == 0)
			{
				if (strcasecmp(argv[4], "on") == 0)
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,ExtendPage);
					mtMiiRegWrite(phyaddr,page_reg,ExtendPage);
					//value=tcMiiStationRead_ext(phyaddr,0x1A);
					value=mtMiiRegRead(phyaddr,0x1A);
					//tcMiiStationWrite_ext(phyaddr,0x1A,value|0x8000);//set Media_Loopback(ExtReg1Ah[15])='1'
					mtMiiRegWrite(phyaddr,0x1A,value|0x8000);//set Media_Loopback(ExtReg1Ah[15])='1'
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//recover page number to main page
					mtMiiRegWrite(phyaddr,page_reg,0);//recover page number to main page

					//tcMiiStationWrite_ext(phyaddr,0x00,0x0100);//set Smi_Reg00 as Force-10F _mode , set reg00=0x0100
					mtMiiRegWrite(phyaddr,0x00,0x0100);//set Smi_Reg00 as Force-10F _mode , set reg00=0x0100
				}
				else
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,ExtendPage);
					mtMiiRegWrite(phyaddr,page_reg,ExtendPage);
					//value=tcMiiStationRead_ext(phyaddr,0x1A);
					value=mtMiiRegRead(phyaddr,0x1A);
					//tcMiiStationWrite_ext(phyaddr,0x1A,value&0x7FFF);//set Media_Loopback(ExtReg1Ah[15])='0'
					mtMiiRegWrite(phyaddr,0x1A,value&0x7FFF);//set Media_Loopback(ExtReg1Ah[15])='0'
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//recover page number to main page
					mtMiiRegWrite(phyaddr,page_reg,0);//recover page number to main page
				  
					//tcMiiStationWrite_ext(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
					mtMiiRegWrite(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
				}
			}
			else if (strcasecmp(argv[3], "di_part1") == 0)
			{
				if (strcasecmp(argv[4], "on") == 0)
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//force page number to main page
					mtMiiRegWrite(phyaddr,page_reg,0);//force page number to main page
					//tcMiiStationWrite_ext(phyaddr,0x00,0x0000);//set Smi_Reg00 as Force-10H _mode ,no Loopback mode, no MrCollisionTestEn , set reg00=0x0000
					mtMiiRegWrite(phyaddr,0x00,0x0000);//set Smi_Reg00 as Force-10H _mode ,no Loopback mode, no MrCollisionTestEn , set reg00=0x0000
				}
				else
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//force page number to main page
					mtMiiRegWrite(phyaddr,page_reg,0);//force page number to main page
					//tcMiiStationWrite_ext(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
					mtMiiRegWrite(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 

					//value=tcMiiStationRead_ext(phyaddr,0x16);
					value=mtMiiRegRead(phyaddr,0x16);
					
					//tcMiiStationWrite_ext(phyaddr,0x16,value|0x2000);//recover EchoMode10Dis(Reg16h[13]) ='1'
					mtMiiRegWrite(phyaddr,0x16,value|0x2000);//recover EchoMode10Dis(Reg16h[13]) ='1'
				}
			}
			else if (strcasecmp(argv[3], "di_part2") == 0)
			{
				if (strcasecmp(argv[4], "on") == 0)
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//force page number to main page
					mtMiiRegWrite(phyaddr,page_reg,0);//force page number to main page
					//value=tcMiiStationRead_ext(phyaddr,0x16);
					value=mtMiiRegRead(phyaddr,0x16);
					
					//tcMiiStationWrite_ext(phyaddr,0x16,value&0xDFFF);//set EchoMode10Dis(Reg16h[13]) ='0'
					mtMiiRegWrite(phyaddr,0x16,value&0xDFFF);//set EchoMode10Dis(Reg16h[13]) ='0'
				}
				else
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//force page number to main page
					mtMiiRegWrite(phyaddr,page_reg,0);//force page number to main page
					//tcMiiStationWrite_ext(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
					mtMiiRegWrite(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 

					//value=tcMiiStationRead_ext(phyaddr,0x16);
					value=mtMiiRegRead(phyaddr,0x16);
					
					//tcMiiStationWrite_ext(phyaddr,0x16,value|0x2000);//recover EchoMode10Dis(Reg16h[13]) ='1'
					mtMiiRegWrite(phyaddr,0x16,value|0x2000);//recover EchoMode10Dis(Reg16h[13]) ='1'
				}
			}
			else if (strcasecmp(argv[3], "digital") == 0)
			{
				if (strcasecmp(argv[4], "on") == 0)
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//force page number to main page
					mtMiiRegWrite(phyaddr,page_reg,0);//force page number to main page
					//tcMiiStationWrite_ext(phyaddr,0x00,0x4100);//set Smi_Reg00 as Force-10F _mode + Loopback mode , set reg00=0x4100
					mtMiiRegWrite(phyaddr,0x00,0x4100);//set Smi_Reg00 as Force-10F _mode + Loopback mode , set reg00=0x4100
				}
				else
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//force page number to main page
					mtMiiRegWrite(phyaddr,page_reg,0);//force page number to main page
					//tcMiiStationWrite_ext(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
					mtMiiRegWrite(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
				}
			}
			else if (strcasecmp(argv[3], "analog") == 0)
			{
				if (strcasecmp(argv[4], "on") == 0)
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,TestPage);
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,page_reg,TestPage);
					//value=tcMiiStationRead_ext(phyaddr,0x0D);
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D value=mtMiiRegRead(phyaddr,0x0D);
					//tcMiiStationWrite_ext(phyaddr,0x0D,value|0x0080);//set AnaLoopBack_pre(tReg0Dh[7])='1'
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,0x0D,value|0x0080);//set AnaLoopBack_pre(tReg0Dh[7])='1'
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//recover page number to main page
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,page_reg,0);//recover page number to main page


                    //V2.0 move test reg0x0D to DEV0x1E reg0x14D 
                    value=mtEMiiRegRead(phyaddr, 0x1E,0x14D);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x14D, value|0x0080);//set AnaLoopBack_pre(dev1eh_reg14Dh[7])='1' //CML_20130221

					//value=tcMiiExtStationRead_ext(phyaddr, 0x1E,0x33, NORMAL_READ);
					value=mtEMiiRegRead(phyaddr, 0x1E,0x33);
					//tcMiiExtStationWrite_ext(phyaddr, 0x1E, 0x33, value&0xFFF8);//set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='000'
					mtEMiiRegWrite(phyaddr, 0x1E, 0x33, value&0xFFF8);//set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='000'

					//tcMiiStationWrite_ext(phyaddr,0x00,0x0100);//set Smi_Reg00 as Force-10F _mode , set reg00=0x0100
					mtMiiRegWrite(phyaddr,0x00,0x0100);//set Smi_Reg00 as Force-10F _mode , set reg00=0x0100
				}
				else
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,TestPage);
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,page_reg,TestPage);
					//value=tcMiiStationRead_ext(phyaddr,0x0D);
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D value=mtMiiRegRead(phyaddr,0x0D);
					//tcMiiStationWrite_ext(phyaddr,0x0D,value&0xFF7F);//set AnaLoopBack_pre(tReg0Dh[7])='0'
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,0x0D,value&0xFF7F);//set AnaLoopBack_pre(tReg0Dh[7])='0'
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//recover page number to main page
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,page_reg,0);//recover page number to main page

                    //V2.0 move test reg0x0D to DEV0x1E reg0x14D 
                    value=mtEMiiRegRead(phyaddr, 0x1E,0x14D);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x14D, value&0xFF7F);//set AnaLoopBack_pre(dev1eh_reg14Dh[7])='0'
				  
					//value=tcMiiExtStationRead_ext(phyaddr, 0x1E,0x33, NORMAL_READ);
					value=mtEMiiRegRead(phyaddr, 0x1E,0x33);
					//tcMiiExtStationWrite_ext(phyaddr, 0x1E, 0x33, value|0x0007);//set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='111'
					mtEMiiRegWrite(phyaddr, 0x1E, 0x33, value|0x0007);//set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='111'
			  
					//tcMiiStationWrite_ext(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
					mtMiiRegWrite(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
				}
			}
			else
			{
				printf("	 CMD Error : tce lpbk <10M> <phyaddr> <farend/di/digital/analog> <on/off>\r\n");
			}
			return 0;
		}
		else if ((strcasecmp(argv[1], "100M") == 0)&&((strcasecmp(argv[4], "on") == 0)||(strcasecmp(argv[4], "off") == 0)))
		{
			if (strcasecmp(argv[3], "farend") == 0)
			{
				if (strcasecmp(argv[4], "on") == 0)
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,ExtendPage);
					mtMiiRegWrite(phyaddr,page_reg,ExtendPage);
					//value=tcMiiStationRead_ext(phyaddr,0x1A);
					value=mtMiiRegRead(phyaddr,0x1A);
					
					//tcMiiStationWrite_ext(phyaddr,0x1A,value|0x8000);//set Media_Loopback(ExtReg1Ah[15])='1'
					mtMiiRegWrite(phyaddr,0x1A,value|0x8000);//set Media_Loopback(ExtReg1Ah[15])='1'
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//recover page number to main page
					mtMiiRegWrite(phyaddr,page_reg,0);//recover page number to main page

					//tcMiiStationWrite_ext(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
					mtMiiRegWrite(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 

					//value=tcMiiStationRead_ext(phyaddr,0x09);
					value=mtMiiRegRead(phyaddr,0x09);
					
					//tcMiiStationWrite_ext(phyaddr,0x09,value&0xFCFF);//set AdvCap1000FDX(Reg09[9])=0 &AdvCap1000HDX(Reg09[8])=0 to disable 1000M ability , use AN to get 100M Full ability to remote
					mtMiiRegWrite(phyaddr,0x09,value&0xFCFF);//set AdvCap1000FDX(Reg09[9])=0 &AdvCap1000HDX(Reg09[8])=0 to disable 1000M ability , use AN to get 100M Full ability to remote
				}
				else
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,ExtendPage);
					mtMiiRegWrite(phyaddr,page_reg,ExtendPage);
					//value=tcMiiStationRead_ext(phyaddr,0x1A);
					value=mtMiiRegRead(phyaddr,0x1A);
					
					//tcMiiStationWrite_ext(phyaddr,0x1A,value&0x7FFF);//set Media_Loopback(ExtReg1Ah[15])='0'
					mtMiiRegWrite(phyaddr,0x1A,value&0x7FFF);//set Media_Loopback(ExtReg1Ah[15])='0'
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//recover page number to main page
					mtMiiRegWrite(phyaddr,page_reg,0);//recover page number to main page

					//tcMiiStationWrite_ext(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
					mtMiiRegWrite(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 

					//value=tcMiiStationRead_ext(phyaddr,0x04);
					value=mtMiiRegRead(phyaddr,0x04);
					
					//tcMiiStationWrite_ext(phyaddr,0x04,value|0x0300);//recovery to default value,set AdvCap1000FDX(Reg09[9])=1 &AdvCap1000HDX(Reg09[8])=1
					mtMiiRegWrite(phyaddr,0x04,value|0x0300);//recovery to default value,set AdvCap1000FDX(Reg09[9])=1 &AdvCap1000HDX(Reg09[8])=1
				}

			}
			else if (strcasecmp(argv[3], "digital") == 0)
			{
				if (strcasecmp(argv[4], "on") == 0)
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//force page number to main page
					mtMiiRegWrite(phyaddr,page_reg,0);//force page number to main page
					//tcMiiStationWrite_ext(phyaddr,0x00,0x6100);//set Smi_Reg00 as Force-100F _mode + Loopback mode , set reg00=0x6100
					mtMiiRegWrite(phyaddr,0x00,0x6100);//set Smi_Reg00 as Force-100F _mode + Loopback mode , set reg00=0x6100
				}
				else
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//force page number to main page
					mtMiiRegWrite(phyaddr,page_reg,0);//force page number to main page
					//tcMiiStationWrite_ext(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
					mtMiiRegWrite(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
				}
			}
			else if (strcasecmp(argv[3], "analog") == 0)
			{
				if (strcasecmp(argv[4], "on") == 0)
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,TestPage);
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,page_reg,TestPage);
					//value=tcMiiStationRead_ext(phyaddr,0x0D);
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D value=mtMiiRegRead(phyaddr,0x0D);
					//tcMiiStationWrite_ext(phyaddr,0x0D,value|0x0080);//set AnaLoopBack_pre(tReg0Dh[7])='1'
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,0x0D,value|0x0080);//set AnaLoopBack_pre(tReg0Dh[7])='1'
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//recover page number to main page
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,page_reg,0);//recover page number to main page

                    //V2.0 move test reg0x0D to DEV0x1E reg0x14D 
                    value=mtEMiiRegRead(phyaddr, 0x1E,0x14D);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x14D, value|0x0080);//set AnaLoopBack_pre(dev1eh_reg14Dh[7])='1' //CML_20130221

					//value=tcMiiExtStationRead_ext(phyaddr, 0x1E,0x33, NORMAL_READ);
					value=mtEMiiRegRead(phyaddr, 0x1E,0x33);
					//tcMiiExtStationWrite_ext(phyaddr, 0x1E, 0x33, value&0xFFF8);//set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='000'
					mtEMiiRegWrite(phyaddr, 0x1E, 0x33, value&0xFFF8);//set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='000'

                    //token ring register PCS_01 setting
					//tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
					mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					//while(!((tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					while(!((mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					
					//value_low = tcMiiStationRead_ext(phyaddr, Token_Ring_Low_data_reg);
					value_low = mtMiiRegRead(phyaddr, Token_Ring_Low_data_reg);
					//value_high = tcMiiStationRead_ext(phyaddr, Token_Ring_High_data_reg);
					value_high = mtMiiRegRead(phyaddr, Token_Ring_High_data_reg);
					value=value_low+((value_high&0x00FF)<<15);

					value=value|0x0000C000; //set RxABPairSwapForce(PCS_01[15])=1 && set RxABPairSwapForceVal(PCS_01[14])=1
					value_high=(0x00FF0000&value);
					value_low=(0x0000FFFF&value);
					//tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
					mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Low_data_reg, value_low);
					mtMiiRegWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_High_data_reg, value_high);
					mtMiiRegWrite(phyaddr, Token_Ring_High_data_reg, value_high);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					//while(!((tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					while(!((mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					//token ring register PCS_01 setting end

					//tcMiiStationWrite_ext(phyaddr, page_reg, 0x00);//V1.11
					mtMiiRegWrite(phyaddr, page_reg, 0x00);//V1.11

					//tcMiiStationWrite_ext(phyaddr,0x00,0x2100);//set Smi_Reg00 as Force-100F _mode , set reg00=0x0100
					mtMiiRegWrite(phyaddr,0x00,0x2100);//set Smi_Reg00 as Force-100F _mode , set reg00=0x0100
				}
				else
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,TestPage);
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,page_reg,TestPage);
					//value=tcMiiStationRead_ext(phyaddr,0x0D);
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D value=mtMiiRegRead(phyaddr,0x0D);
					//tcMiiStationWrite_ext(phyaddr,0x0D,value&0xFF7F);//set AnaLoopBack_pre(tReg0Dh[7])='0'
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,0x0D,value&0xFF7F);//set AnaLoopBack_pre(tReg0Dh[7])='0'
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//recover page number to main page
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,page_reg,0);//recover page number to main page

                    //V2.0 move test reg0x0D to DEV0x1E reg0x14D 
                    value=mtEMiiRegRead(phyaddr, 0x1E,0x14D);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x14D, value&0xFF7F);//set AnaLoopBack_pre(dev1eh_reg14Dh[7])='0'
				  
					//value=tcMiiExtStationRead_ext(phyaddr, 0x1E,0x33, NORMAL_READ);
					value=mtEMiiRegRead(phyaddr, 0x1E,0x33);
					//tcMiiExtStationWrite_ext(phyaddr, 0x1E, 0x33, value|0x0007);//set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='111'
					mtEMiiRegWrite(phyaddr, 0x1E, 0x33, value|0x0007);//set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='111'

					//token ring register PCS_01 setting
					//tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
					mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					//while(!((tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					while(!((mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					//value_low = tcMiiStationRead_ext(phyaddr, Token_Ring_Low_data_reg);
					value_low = mtMiiRegRead(phyaddr, Token_Ring_Low_data_reg);
					//value_high = tcMiiStationRead_ext(phyaddr, Token_Ring_High_data_reg);
					value_high = mtMiiRegRead(phyaddr, Token_Ring_High_data_reg);
					value=value_low+((value_high&0x00FF)<<15);

					value=value&0x00FF3FFF; //set RxABPairSwapForce(PCS_01[15])=0 && set RxABPairSwapForceVal(PCS_01[14])=0
					value_high=(0x00FF0000&value);
					value_low=(0x0000FFFF&value);
					//tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
					mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Low_data_reg, value_low);
					mtMiiRegWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_High_data_reg, value_high);
					mtMiiRegWrite(phyaddr, Token_Ring_High_data_reg, value_high);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					//while(!((tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					while(!((mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					//token ring register PCS_01 setting end

					//tcMiiStationWrite_ext(phyaddr, page_reg, 0x00);//V1.11
					mtMiiRegWrite(phyaddr, page_reg, 0x00);//V1.11

			  
					//tcMiiStationWrite_ext(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value)
					mtMiiRegWrite(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value)
				}
			}
			else
			{
				printf("     CMD Error : tce lpbk <100M> <phyaddr> <farend/digital/analog> <on/off>\r\n");
			}
			return 0;
		}
		else if ((strcasecmp(argv[1], "1000M") == 0)&&((strcasecmp(argv[4], "on") == 0)||(strcasecmp(argv[4], "off") == 0)))
		{
			if (strcasecmp(argv[3], "farend") == 0)
			{
				if (strcasecmp(argv[4], "on") == 0)
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,ExtendPage);
					mtMiiRegWrite(phyaddr,page_reg,ExtendPage);
					//value=tcMiiStationRead_ext(phyaddr,0x1A);
					value=mtMiiRegRead(phyaddr,0x1A);
					//tcMiiStationWrite_ext(phyaddr,0x1A,value|0x8000);//set Media_Loopback(ExtReg1Ah[15])='1'
					mtMiiRegWrite(phyaddr,0x1A,value|0x8000);//set Media_Loopback(ExtReg1Ah[15])='1'
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//recover page number to main page
					mtMiiRegWrite(phyaddr,page_reg,0);//recover page number to main page

					//tcMiiStationWrite_ext(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
					mtMiiRegWrite(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 

					//tcMiiStationWrite_ext(phyaddr,0x09,0x0700);//set smi_Reg09 to initial value , set reg09=0x0700(initial value) 
					mtMiiRegWrite(phyaddr,0x09,0x0700);//set smi_Reg09 to initial value , set reg09=0x0700(initial value) 
				}
				else
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,ExtendPage);
					mtMiiRegWrite(phyaddr,page_reg,ExtendPage);
					//value=tcMiiStationRead_ext(phyaddr,0x1A);
					value=mtMiiRegRead(phyaddr,0x1A);
					//tcMiiStationWrite_ext(phyaddr,0x1A,value&0x7FFF);//set Media_Loopback(ExtReg1Ah[15])='0'
					mtMiiRegWrite(phyaddr,0x1A,value&0x7FFF);//set Media_Loopback(ExtReg1Ah[15])='0'
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//recover page number to main page
					mtMiiRegWrite(phyaddr,page_reg,0);//recover page number to main page

					//tcMiiStationWrite_ext(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
					mtMiiRegWrite(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 

					//tcMiiStationWrite_ext(phyaddr,0x09,0x0700);//set smi_Reg09 to initial value , set reg09=0x0700(initial value)
					mtMiiRegWrite(phyaddr,0x09,0x0700);//set smi_Reg09 to initial value , set reg09=0x0700(initial value)
				}
			}
			else if (strcasecmp(argv[3], "digital_lpbk") == 0)
			{
				if (strcasecmp(argv[4], "on") == 0)
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//force page number to main page
					mtMiiRegWrite(phyaddr,page_reg,0);//force page number to main page
					//tcMiiStationWrite_ext(phyaddr,0x00,0x4140);//set Smi_Reg00 as Force-1000F _mode + Loopback mode , set reg00=0x4140
					mtMiiRegWrite(phyaddr,0x00,0x4140);//set Smi_Reg00 as Force-1000F _mode + Loopback mode , set reg00=0x4140
				}
				else
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//force page number to main page
					mtMiiRegWrite(phyaddr,page_reg,0);//force page number to main page
					//tcMiiStationWrite_ext(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
					mtMiiRegWrite(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
				}
			}
			else if (strcasecmp(argv[3], "analog") == 0)
			{
				if (strcasecmp(argv[4], "on") == 0)
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,TestPage);
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,page_reg,TestPage);
					//value=tcMiiStationRead_ext(phyaddr,0x0D);
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D value=mtMiiRegRead(phyaddr,0x0D);
					//tcMiiStationWrite_ext(phyaddr,0x0D,value|0x0080);//set AnaLoopBack_pre(tReg0Dh[7])='1'
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,0x0D,value|0x0080);//set AnaLoopBack_pre(tReg0Dh[7])='1'
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//recover page number to main page
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,page_reg,0);//recover page number to main page

                    //V2.0 move test reg0x0D to DEV0x1E reg0x14D 
                    value=mtEMiiRegRead(phyaddr, 0x1E,0x14D);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x14D, value|0x0080);//set AnaLoopBack_pre(dev1eh_reg14Dh[7])='1' //CML_20130221

					//value=tcMiiStationRead_ext(phyaddr,0x18);
					value=mtMiiRegRead(phyaddr,0x18);
					
					//tcMiiStationWrite_ext(phyaddr,0x18,value|0x0001);//set CableLoopback_pre(Reg18h)=1 to enable 1000M cable loop back
					mtMiiRegWrite(phyaddr,0x18,value|0x0001);//set CableLoopback_pre(Reg18h)=1 to enable 1000M cable loop back

					//value=tcMiiExtStationRead_ext(phyaddr, 0x1E,0x33, NORMAL_READ);
					value=mtEMiiRegRead(phyaddr, 0x1E,0x33);
					//tcMiiExtStationWrite_ext(phyaddr, 0x1E, 0x33, value&0xFFF8);//set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='000'
					mtEMiiRegWrite(phyaddr, 0x1E, 0x33, value&0xFFF8);//set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='000'

					//token ring register PCS_01 setting
					//tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
					mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					//while(!((tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					while(!((mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					//value_low = tcMiiStationRead_ext(phyaddr, Token_Ring_Low_data_reg);
					value_low = mtMiiRegRead(phyaddr, Token_Ring_Low_data_reg);
					//value_high = tcMiiStationRead_ext(phyaddr, Token_Ring_High_data_reg);
					value_high = mtMiiRegRead(phyaddr, Token_Ring_High_data_reg);
					value=value_low+((value_high&0x00FF)<<15);

					value=value|0x00003C00; //set TxCDPairSwapForce(PCS_01[13])=1 && set TxCDPairSwapForceVal(PCS_01[12])=1 && set TxABPairSwapForce(PCS_01[11])=1 && set TxABPairSwapForceVal(PCS_01[10])=1
					value_high=(0x00FF0000&value);
					value_low=(0x0000FFFF&value);
					//tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
					mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Low_data_reg, value_low);
					mtMiiRegWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_High_data_reg, value_high);
					mtMiiRegWrite(phyaddr, Token_Ring_High_data_reg, value_high);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					//while(!((tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					while(!((mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					//token ring register PCS_01 setting end

					//token ring register ECNC_C1 setting
					//tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
					mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(0x0<<11)|(0x3<<7)|(0x01<<1));
					mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(0x0<<11)|(0x3<<7)|(0x01<<1));
					//while(!((tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					while(!((mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					//value_low = tcMiiStationRead_ext(phyaddr, Token_Ring_Low_data_reg);
					value_low = mtMiiRegRead(phyaddr, Token_Ring_Low_data_reg);
					//value_high = tcMiiStationRead_ext(phyaddr, Token_Ring_High_data_reg);
					value_high = mtMiiRegRead(phyaddr, Token_Ring_High_data_reg);
					value=value_low+((value_high&0x00FF)<<15);

					value=value|0x00000041; //set EcAForceGainswapdisab(ECNC_C1[6])=1 && set ECForceUpdateDis(ECNC_C1[0])=1
					value_high=(0x00FF0000&value);
					value_low=(0x0000FFFF&value);
					//tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
					mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Low_data_reg, value_low);
					mtMiiRegWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_High_data_reg, value_high);
					mtMiiRegWrite(phyaddr, Token_Ring_High_data_reg, value_high);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(0x0<<11)|(0x3<<7)|(0x01<<1));
					mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(0x0<<11)|(0x3<<7)|(0x01<<1));
					//while(!((tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					while(!((mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					//token ring register PCS_01 setting end

					//tcMiiStationWrite_ext(phyaddr, page_reg, 0x00);//V1.11
					mtMiiRegWrite(phyaddr, page_reg, 0x00);//V1.11

					//tcMiiStationWrite_ext(phyaddr,0x00,0x0140);//set Smi_Reg00 as Force-1000F _mode , set reg00=0x0140
					mtMiiRegWrite(phyaddr,0x00,0x0140);//set Smi_Reg00 as Force-1000F _mode , set reg00=0x0140
				}
				else
				{
					//tcMiiStationWrite_ext(phyaddr,page_reg,TestPage);
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,page_reg,TestPage);
					//value=tcMiiStationRead_ext(phyaddr,0x0D);
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D value=mtMiiRegRead(phyaddr,0x0D);
					//tcMiiStationWrite_ext(phyaddr,0x0D,value&0xFF7F);//set AnaLoopBack_pre(tReg0Dh[7])='0'
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,0x0D,value&0xFF7F);//set AnaLoopBack_pre(tReg0Dh[7])='0'
					//tcMiiStationWrite_ext(phyaddr,page_reg,0);//recover page number to main page
					//V2.0 move test reg0x0D to DEV0x1E reg0x14D mtMiiRegWrite(phyaddr,page_reg,0);//recover page number to main page

                    //V2.0 move test reg0x0D to DEV0x1E reg0x14D 
                    value=mtEMiiRegRead(phyaddr, 0x1E,0x14D);
					mtEMiiRegWrite(phyaddr, 0x1E, 0x14D, value&0xFF7F);//set AnaLoopBack_pre(dev1eh_reg14Dh[7])='0'

					//value=tcMiiStationRead_ext(phyaddr,0x18);
					value=mtMiiRegRead(phyaddr,0x18);
					
					//tcMiiStationWrite_ext(phyaddr,0x18,value&0xFFFE);//recovery to default,set CableLoopback_pre(Reg18h)=0 to disable 1000M cable loop back
					mtMiiRegWrite(phyaddr,0x18,value&0xFFFE);//recovery to default,set CableLoopback_pre(Reg18h)=0 to disable 1000M cable loop back
				  
					//value=tcMiiExtStationRead_ext(phyaddr, 0x1E,0x33, NORMAL_READ);
					value=mtEMiiRegRead(phyaddr, 0x1E,0x33);
					//tcMiiExtStationWrite_ext(phyaddr, 0x1E, 0x33, value|0x0007);//recovery to default,set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='111'
					mtEMiiRegWrite(phyaddr, 0x1E, 0x33, value|0x0007);//recovery to default,set cr_pd_*_pair_sw_en(dev1eh_reg33h[2:0]) ='111'

					//token ring register PCS_01 setting
					//tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
					mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					//while(!((tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					while(!((mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					//value_low = tcMiiStationRead_ext(phyaddr, Token_Ring_Low_data_reg);
					value_low = mtMiiRegRead(phyaddr, Token_Ring_Low_data_reg);
					//value_high = tcMiiStationRead_ext(phyaddr, Token_Ring_High_data_reg);
					value_high = mtMiiRegRead(phyaddr, Token_Ring_High_data_reg);
					value=value_low+((value_high&0x00FF)<<15);

					value=value&0x00FFC3FF; //recovery to default,set TxCDPairSwapForce(PCS_01[13])=0 && set TxCDPairSwapForceVal(PCS_01[12])=0 && set TxABPairSwapForce(PCS_01[11])=0 && set TxABPairSwapForceVal(PCS_01[10])=0
					value_high=(0x00FF0000&value);
					value_low=(0x0000FFFF&value);
					//tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
					mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Low_data_reg, value_low);
					mtMiiRegWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_High_data_reg, value_high);
					mtMiiRegWrite(phyaddr, Token_Ring_High_data_reg, value_high);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(Token_Ring_CH_addr_PCS<<11)|(Token_Ring_Node_addr_PCS<<7)|(0x01<<1));
					//while(!((tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					while(!((mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					//token ring register PCS_01 setting end

					//token ring register ECNC_C1 setting
					//tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
					mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(0x0<<11)|(0x3<<7)|(0x01<<1));
					mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(1<<13)|(0x0<<11)|(0x3<<7)|(0x01<<1));
					//while(!((tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					while(!((mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					//value_low = tcMiiStationRead_ext(phyaddr, Token_Ring_Low_data_reg);
					value_low = mtMiiRegRead(phyaddr, Token_Ring_Low_data_reg);
					//value_high = tcMiiStationRead_ext(phyaddr, Token_Ring_High_data_reg);
					value_high = mtMiiRegRead(phyaddr, Token_Ring_High_data_reg);
					value=value_low+((value_high&0x00FF)<<15);

					value=value&0xFFFFFFBE; //recovery to default,set EcAForceGainswapdisab(ECNC_C1[6])=0 && set ECForceUpdateDis(ECNC_C1[0])=0
					value_high=(0x00FF0000&value);
					value_low=(0x0000FFFF&value);
					//tcMiiStationWrite_ext(phyaddr, page_reg, Token_Ring_debug_reg);
					mtMiiRegWrite(phyaddr, page_reg, Token_Ring_debug_reg);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Low_data_reg, value_low);
					mtMiiRegWrite(phyaddr, Token_Ring_Low_data_reg, value_low);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_High_data_reg, value_high);
					mtMiiRegWrite(phyaddr, Token_Ring_High_data_reg, value_high);
					//tcMiiStationWrite_ext(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(0x0<<11)|(0x3<<7)|(0x01<<1));
					mtMiiRegWrite(phyaddr, Token_Ring_Control_reg, (1<<15)|(0<<13)|(0x0<<11)|(0x3<<7)|(0x01<<1));
					//while(!((tcMiiStationRead_ext(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					while(!((mtMiiRegRead(phyaddr, Token_Ring_Control_reg)&0x8000)==0x8000)); // data ready
					//token ring register PCS_01 setting end

					//tcMiiStationWrite_ext(phyaddr, page_reg, 0x00);//V1.11
					mtMiiRegWrite(phyaddr, page_reg, 0x00);//V1.11
		  
					//tcMiiStationWrite_ext(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
					mtMiiRegWrite(phyaddr,0x00,0x1040);//set Smi_Reg00 to initial value , set reg00=0x1040(initial value) 
				}

			}
			else
			{
				printf("	 CMD Error : tce lpbk <1000M> <phyaddr> <farend/digital/analog> <on/off>\r\n");
			}
			return 0;
		}
	}


	printf("     CMD Error : tce lpbk <10M> <phyaddr> <farend/di/digital/analog> <on/off>\r\n");
	printf("     CMD Error : tce lpbk <100M> <phyaddr> <farend/digital/analog> <on/off>\r\n");
	printf("     CMD Error : tce lpbk <1000M> <phyaddr> <farend/digital/analog> <on/off>\r\n");

	return 0;	   


}


int doGbeTxShaper(int argc, char *argv[], void *p)
{
  uint32 BaseAddr = 0x200;
  uint32 TuneAddr1 = 0;
  uint32 TuneAddr2 = 0;
  uint32 TarAddr = 0;
  uint32 wData = 0;
  uint32 wTmp1 = 0, wTmp2 = 0, wTmp3 = 0;

  //if ((argc != 7) || ((strcmpi("fe",argv[1]) != 0) && (strcmpi("gbe",argv[1]) != 0)))
  if ((argc != 7) && (argc != 4))
  {
    printf("Usage1: tce gtxshaper <norm/test> <PreSec> <NextSec> <Mid_Value1> <Ovs_Value2> <OvP_Value3>\n");
    printf("<norm/test> = select normal or test mode\n");
    printf("<PreSec>  = {P2|P1|Z0|N1|N2} for GbE\n");
    printf("<NextSec> = {P2|P1|Z0|N1|N2} for GbE\n");
    printf("<Mid_Value1> = 1st transition point (HEX format) - middle level\n");
    printf("<Ovs_Value2> = 2nd transition point (HEX format) - overshoot level\n");
    printf("<OvP_Value3> = 3rd transition point (HEX format) - overshoot-post level\n");
    printf("Usage2: tce gtxshaper <norm/test> <PreSec> <NextSec>\n");
    printf("        Get current combination setting\n");
  }
  else //if ((strcmpi("norm",argv[1]) == 0) || (strcmpi("test",argv[1]) == 0)) //GbE TX Shaper
  {
    if ((strcasecmp("norm",argv[1]) == 0)) //GbE Normal Mode
    {
      BaseAddr = 0x200;
    }
    else if((strcasecmp("test",argv[1]) == 0))//GbE Test Mode
    {
      BaseAddr = 0x200;  ///V2.00 change 0x232 to 0x200, E2 merge normal and test mode
    }
    else
    {
      printf("Arg[1] Error: {norm|test}\n");
    }

    if (strcasecmp("P2",argv[2]) == 0)
    {
      TuneAddr1 = 0;
    }
    else if (strcasecmp("P1",argv[2]) == 0)
    {
      TuneAddr1 = 10;
    }
    else if (strcasecmp("Z0",argv[2]) == 0)
    {
      TuneAddr1 = 20;
    }
    else if (strcasecmp("N1",argv[2]) == 0)
    {
      TuneAddr1 = 30;
    }
    else if (strcasecmp("N2",argv[2]) == 0)
    {
      TuneAddr1 = 40;
    }
    else
    {
      printf("Arg[2] Error: {P2|P1|Z0|N1|N2}\n");
    }


    if (strcasecmp("P2",argv[3]) == 0)
    {
      TuneAddr2 = 0;
    }
    else if (strcasecmp("P1",argv[3]) == 0)
    {
      TuneAddr2 = 2;
    }
    else if (strcasecmp("Z0",argv[3]) == 0)
    {
      TuneAddr2 = 4;
    }
    else if (strcasecmp("N1",argv[3]) == 0)
    {
      TuneAddr2 = 6;
    }
    else if (strcasecmp("N2",argv[3]) == 0)
    {
      TuneAddr2 = 8;
    }
    else
    {
      printf("Arg[3] Error: {P2|P1|Z0|N1|N2}\n");
    }

    if (argc == 7) //Set corresponding register
    {
      sscanf(argv[4], "%lx", &wTmp1);
      sscanf(argv[5], "%lx", &wTmp2);
      sscanf(argv[6], "%lx", &wTmp3);
      //wTmp1 = wTmp1 &0x003F;
      //wTmp2 = wTmp2 &0x003F;
      //wTmp3 = wTmp3 &0x003F;
      if ((wTmp1 > 0x3F) || (wTmp2 > 0x3F) || (wTmp2 > 0x3F))
      {
        printf("Arg[4~6] Error: value should be 6bit digit\n");
      }
      else
      {
        wData = (wTmp2 << 8) + wTmp1;
        
        TarAddr = BaseAddr + TuneAddr1 + TuneAddr2;
        //tcMiiExtStationWrite_ext(0x00, 0x1F, TarAddr, wData);
        mtEMiiRegWrite(0x00, 0x1F, TarAddr, wData);
        //tcMiiExtStationWrite_ext(0x00, 0x1F, TarAddr+1, wTmp3);
        mtEMiiRegWrite(0x00, 0x1F, TarAddr+1, wTmp3);
        printf("Write(dev1Fh_reg%Xh) = %X\n", TarAddr, wData); //debug
        printf("Write(dev1Fh_reg%Xh) = %X\n", TarAddr+1, wTmp3); //debug
      }
    }
    else  //Get corresponding setting
    {
        TarAddr = BaseAddr + TuneAddr1 + TuneAddr2;
        //wData = tcMiiExtStationRead_ext(0x00, 0x1F, TarAddr, NORMAL_READ);
        wData = mtEMiiRegRead(0x00, 0x1F, TarAddr);
        wTmp1 = wData & 0x003F;
        wTmp2 = (wData & 0x3F00) >> 8;
        printf("Read(dev1Fh_reg%Xh) = %X\n", TarAddr, wData);
        //wData = tcMiiExtStationRead_ext(0x00, 0x1F, TarAddr+1, NORMAL_READ);
        wData = mtEMiiRegRead(0x00, 0x1F, TarAddr+1);
        wTmp3 = wData & 0x003F;
        printf("Read(dev1Fh_reg%Xh) = %X\n", TarAddr+1, wData);
        printf("[Mid, Ovs, Ovp] = %X %X %X\n", wTmp1, wTmp2, wTmp3);
    }
  }

  return 0;
}

int doFeTxShaper(int argc, char *argv[], void *p)
{
  //uint32 BaseAddr = 0x200;
  //uint32 TuneAddr1 = 0;
  //uint32 TuneAddr2 = 0;
  //uint32 TarAddr = 0;
  uint32 wData = 0;
  uint32 wTmp1 = 0, wTmp2 = 0, wTmp3 = 0;
  uint32 iPort = 0;
  
  //if ((argc != 7) || ((strcmpi("fe",argv[1]) != 0) && (strcmpi("gbe",argv[1]) != 0)))
  if ((argc != 7) && (argc != 4))
  {
    printf("Usage1: tce ftxshaper <PortNum> <PreSec> <NextSec> <Mid_Value1> <Ovs_Value2> <OvP_Value3>\n");
    printf("where\n");
    printf("<PortNum> = 0 ~ 4\n");
    printf("<PreSec> = {1,0,-1} for FE\n");
    printf("<NextSec> = {1,0,-1} for FE\n");
    printf("<Mid_Value1> = 1st transition point (HEX format) - middle level\n");
    printf("<Ovs_Value2> = 2nd transition point (HEX format) - overshoot level\n");
    printf("<OvP_Value3> = 3rd transition point (HEX format) - overshoot-post level\n");
    printf("Usage2: tce ftxshaper <PortNum> <PreSec> <NextSec>\n");
    printf("        Get current combination setting\n");

  }
  else if (argc == 7)  //Set Mid, Ovs, Ovp
  {
    sscanf(argv[1], "%lx", &iPort);
    sscanf(argv[4], "%lx", &wTmp1);
    sscanf(argv[5], "%lx", &wTmp2);
    sscanf(argv[6], "%lx", &wTmp3);
    if ((iPort > 4) || (iPort < 0))
      printf("Port Number Error: {0~4}");
    else if ((wTmp1 > 0x3FF) || (wTmp2 > 0x3FF) || (wTmp2 > 0x3FF))
      printf("Arg[4~6] Error: value should be 10bit digit\n");
    else
    {
      if ((strcasecmp("0",argv[2])==0) && (strcasecmp("1",argv[3])==0))
      {
        //wData = tcMiiExtStationRead_ext(iPort, 0x1E, 0x00, NORMAL_READ);
        wData = mtEMiiRegRead(iPort, 0x1E, 0x00);
        wData = (wData & 0x1000) | wTmp1;
        //tcMiiExtStationWrite_ext(iPort, 0x1E, 0x00, wData);
        mtEMiiRegWrite(iPort, 0x1E, 0x00, wData);
        //tcMiiExtStationWrite_ext(iPort, 0x1E, 0x01, wTmp2);
        mtEMiiRegWrite(iPort, 0x1E, 0x01, wTmp2);
        //tcMiiExtStationWrite_ext(iPort, 0x1E, 0x02, wTmp3);
        mtEMiiRegWrite(iPort, 0x1E, 0x02, wTmp3);
        printf("Set Port%d 0 to 1: %X %X %X\n", iPort, wData, wTmp2, wTmp3);
      }
      else if ((strcasecmp("1",argv[2])==0) && (strcasecmp("0",argv[3])==0))
      {
        //tcMiiExtStationWrite_ext(iPort, 0x1E, 0x03, wTmp1);
        mtEMiiRegWrite(iPort, 0x1E, 0x03, wTmp1);
        //tcMiiExtStationWrite_ext(iPort, 0x1E, 0x04, wTmp2);
        mtEMiiRegWrite(iPort, 0x1E, 0x04, wTmp2);
        //tcMiiExtStationWrite_ext(iPort, 0x1E, 0x05, wTmp3);
        mtEMiiRegWrite(iPort, 0x1E, 0x05, wTmp3);
        printf("Set Port%d 1 to 0: %X %X %X\n", iPort, wTmp1, wTmp2, wTmp3);
      }
      else if ((strcasecmp("0",argv[2])==0) && (strcasecmp("-1",argv[3])==0))
      {
        //tcMiiExtStationWrite_ext(iPort, 0x1E, 0x06, wTmp1);
        mtEMiiRegWrite(iPort, 0x1E, 0x06, wTmp1);
        //tcMiiExtStationWrite_ext(iPort, 0x1E, 0x07, wTmp2);
        mtEMiiRegWrite(iPort, 0x1E, 0x07, wTmp2);
        //tcMiiExtStationWrite_ext(iPort, 0x1E, 0x08, wTmp3);
        mtEMiiRegWrite(iPort, 0x1E, 0x08, wTmp3);
        printf("Set Port%d 0 to -1: %X %X %X\n", iPort, wTmp1, wTmp2, wTmp3);
      }
      else if ((strcasecmp("-1",argv[2])==0) && (strcasecmp("0",argv[3])==0))
      {
        //tcMiiExtStationWrite_ext(iPort, 0x1E, 0x09, wTmp1);
        mtEMiiRegWrite(iPort, 0x1E, 0x09, wTmp1);
        //tcMiiExtStationWrite_ext(iPort, 0x1E, 0x0A, wTmp2);
        mtEMiiRegWrite(iPort, 0x1E, 0x0A, wTmp2);
        //tcMiiExtStationWrite_ext(iPort, 0x1E, 0x0B, wTmp3);
        mtEMiiRegWrite(iPort, 0x1E, 0x0B, wTmp3);
        printf("Set Port%d -1 to 0: %X %X %X\n", iPort, wTmp1, wTmp2, wTmp3);
      }
      else
      {
        printf("Arg[2][3] Error: should be[0 1],[1 0], [0 -1], [-1 0]\n");
      }
    }
  }
  else  //Get current setting
  {
    sscanf(argv[1], "%lx", &iPort);
    if ((iPort > 4) || (iPort < 0))
      printf("Port Number Error: {0~4}");
    else
    {
      if ((strcasecmp("0",argv[2])==0) && (strcasecmp("1",argv[3])==0))
      {
        //wData = tcMiiExtStationRead_ext(iPort, 0x1E, 0x00, NORMAL_READ);
        wData = mtEMiiRegRead(iPort, 0x1E, 0x00);
        wData = wData & 0x3FF;
        printf("Mid (dev1Eh_reg000h[9:0]) = %X\n", wData);
        //wData = tcMiiExtStationRead_ext(iPort, 0x1E, 0x01, NORMAL_READ);
        wData = mtEMiiRegRead(iPort, 0x1E, 0x01);
        wData = wData & 0x3FF;
        printf("Ovs (dev1Eh_reg001h[9:0]) = %X\n", wData);
        //wData = tcMiiExtStationRead_ext(iPort, 0x1E, 0x02, NORMAL_READ);
        wData = mtEMiiRegRead(iPort, 0x1E, 0x02);
        wData = wData & 0x3FF;
        printf("Ovp (dev1Eh_reg002h[9:0]) = %X\n", wData);

      }
      else if ((strcasecmp("1",argv[2])==0) && (strcasecmp("0",argv[3])==0))
      {
        //wData = tcMiiExtStationRead_ext(iPort, 0x1E, 0x03, NORMAL_READ);
        wData = mtEMiiRegRead(iPort, 0x1E, 0x03);
        wData = wData & 0x3FF;
        printf("Mid (dev1Eh_reg003h[9:0]) = %X\n", wData);
        //wData = tcMiiExtStationRead_ext(iPort, 0x1E, 0x04, NORMAL_READ);
        wData = mtEMiiRegRead(iPort, 0x1E, 0x04);
        wData = wData & 0x3FF;
        printf("Ovs (dev1Eh_reg004h[9:0]) = %X\n", wData);
        //wData = tcMiiExtStationRead_ext(iPort, 0x1E, 0x05, NORMAL_READ);
        wData = mtEMiiRegRead(iPort, 0x1E, 0x05);
        wData = wData & 0x3FF;
        printf("Ovp (dev1Eh_reg005h[9:0]) = %X\n", wData);
      }
      else if ((strcasecmp("0",argv[2])==0) && (strcasecmp("-1",argv[3])==0))
      {
        //wData = tcMiiExtStationRead_ext(iPort, 0x1E, 0x06, NORMAL_READ);
        wData = mtEMiiRegRead(iPort, 0x1E, 0x06);
        wData = wData & 0x3FF;
        printf("Mid (dev1Eh_reg006h[9:0]) = %X\n", wData);
        //wData = tcMiiExtStationRead_ext(iPort, 0x1E, 0x07, NORMAL_READ);
        wData = mtEMiiRegRead(iPort, 0x1E, 0x07);
        wData = wData & 0x3FF;
        printf("Ovs (dev1Eh_reg007h[9:0]) = %X\n", wData);
        //wData = tcMiiExtStationRead_ext(iPort, 0x1E, 0x08, NORMAL_READ);
		wData = mtEMiiRegRead(iPort, 0x1E, 0x08);
        wData = wData & 0x3FF;
        printf("Ovp (dev1Eh_reg008h[9:0]) = %X\n", wData);
      }
      else if ((strcasecmp("-1",argv[2])==0) && (strcasecmp("0",argv[3])==0))
      {
        //wData = tcMiiExtStationRead_ext(iPort, 0x1E, 0x09, NORMAL_READ);
        wData = mtEMiiRegRead(iPort, 0x1E, 0x09);
        wData = wData & 0x3FF;
        printf("Mid (dev1Eh_reg009h[9:0]) = %X\n", wData);
        //wData = tcMiiExtStationRead_ext(iPort, 0x1E, 0x0A, NORMAL_READ);
        wData = mtEMiiRegRead(iPort, 0x1E, 0x0A);
        wData = wData & 0x3FF;
        printf("Ovs (dev1Eh_reg00Ah[9:0]) = %X\n", wData);
        //wData = tcMiiExtStationRead_ext(iPort, 0x1E, 0x0B, NORMAL_READ);
		wData = mtEMiiRegRead(iPort, 0x1E, 0x0B);
        wData = wData & 0x3FF;
        printf("Ovp (dev1Eh_reg00Bh[9:0]) = %X\n", wData);
      }
      else
      {
        printf("Arg[2][3] Error: should be[0 1],[1 0], [0 -1], [-1 0]\n");
      }
    }
  }
  return 0;
}

int doPhyPktCnt(int argc, char *argv[], void *p)
{       
	const uint16 page_reg=31;
	uint32 PktCnt, CrcErrCnt=0;
    uint32 phyaddr=0;
  
	#define PktCnt_Addr 0x12
	#define CrcErrcnt_Addr 0x17
	#define page_Addr 0x0001
	
    if (argc==2){
    	// tce phycnt <PhyAddr> 
    	phyaddr = checked_atoi(argv[1]);//0~31
    	//tcMiiStationWrite_ext(phyaddr, page_reg, page_Addr);
    	mtMiiRegWrite(phyaddr, page_reg, page_Addr);
    	PktCnt = mtMiiRegRead(phyaddr, PktCnt_Addr);
    	if(PktCnt & 0x8000){
    		PktCnt = PktCnt & 0x7FFF;
    		}
    	else{
    	        printf("Invalid Pkcnt !, value=%d",PktCnt);
    	        return 0;
    		}
        CrcErrCnt  = mtMiiRegRead(phyaddr, CrcErrcnt_Addr) & 0x00FF;
        printf(" PktCnt= %d, CrcErrCnt = %d \r\n", PktCnt, CrcErrCnt); 

       }
    else{
        printf("       CMD Error : phycnt <PhyAddr>\r\n");
        return 0;      
    }
    return 0;

}

#else
// called by macPeriodCheck() in tc3162l2mac.c
void tcPhyChkVal (void)
{
    uint32 port_num = phychkval_portnum; 
    if(phychkval_flag){
        uint32 val;
        uint32 mr02,mr03;
        uint8 mr_anen, mr_dplx, mr_speed;
        uint8 mr_an_capable;
        uint8 mr_lp_an_capable;
        uint8 mr_lp_anen;
        //uint8 lr_force_mdix;
        uint8 lr_linkup, lr_speed, lr_dplx, lr_mdix;

        // show phy_id
        val = tcPhyReadReg(port_num, 2); 
        mr02 = val&(0xffff);
        val = tcPhyReadReg(port_num, 3); 
        mr03 = val&(0xffff);
        printf(" TcPhy ID: %lx %lx\r\n",mr02,mr03); 
        if (mr02 != 0x03a2)
            return;

        // reg0h
        val = tcPhyReadReg(port_num, 0);
        mr_anen = (val>>12)&0x01;
        mr_dplx = (val>>8)&0x01;
        //mr_speed = (val>>13)&0x01;
		mr_speed = (val>>13)|(val>>5)&0x03;//00:10,01:100,10:1000
        // reg4h
        val = tcPhyReadReg(port_num, 0x04);
        mr_an_capable = (val>>5)&0x0F; //100F,100H,10F,10H
        
        // reg9h
        val = tcPhyReadReg(port_num, 0x09);
		mr_an_capable = mr_an_capable | ((val>>4)&0x30);//1000F,1000H
			
        // reg5h
        val = tcPhyReadReg(port_num, 0x05);
        mr_lp_an_capable = (val>>5)&0x0F; //100F,100H,10F,10H
        // regAh
        val = tcPhyReadReg(port_num, 0x0A);
        mr_lp_an_capable = mr_lp_an_capable | ((val>>6)&0x30); //1000F,1000H
        
        // reg6h
        val = tcPhyReadReg(port_num, 0x06);
        mr_lp_anen = (val)&0x01;
        // l0reg28
        #if 1 //MT7530 
		
		val = tcMiiExtStationRead(0x1E, port_num, 0xA2, NORMAL_READ);
        lr_linkup = (val>>9)&0x07;//dev1e_A2[9]:LinkStatus10_OK,dev1e_A2[10]:LinkStatus100_OK,dev1e_A2[11]:LinkStatus1000_OK
        lr_speed = (val>>1)&0x07; //dev1e_A2[1]:final_speed_10,,dev1e_A2[2]:dev1e_A2[2]:final_speed_100,dev1e_A2[3]:final_speed_1000
        lr_dplx = (val>>5)&0x01;
        lr_mdix = (val>>6)&0x01;
		
		#else
        val = tcPhyReadReg(port_num, 28);
        lr_linkup = (val)&0x01;
        lr_speed = (val>>1)&0x01;
        lr_dplx = (val>>2)&0x01;
        lr_mdix = (val>>5)&0x01;
        #endif
        
        printf(" TcPhy mode:");
        if(mr_anen) { // Auto-neg
            printf(" AN-(");
            printf(" %s,",((mr_an_capable>>5)&0x01)?"1000F":"");
            printf(" %s,",((mr_an_capable>>4)&0x01)?"1000F":"");
            printf(" %s,",((mr_an_capable>>3)&0x01)?"100F":"");
            printf(" %s,",((mr_an_capable>>2)&0x01)?"100H":"");     
            printf(" %s,",((mr_an_capable>>1)&0x01)?"10F":"");      
            printf(" %s)\r\n",((mr_an_capable>>0)&0x01)?"10H":"");  
        }
        else { // Force-speed
            //printf(" Force-%d%s\r\n",
                    //(mr_speed?100:10),(mr_dplx?"F":"H"));
                    
             if(mr_speed==0x00){
			 	printf(" Force-%d%s\r\n",10,(mr_dplx?"F":"H"));
				}
			 else if(mr_speed==0x01){
				 printf(" Force-%d%s\r\n",100,(mr_dplx?"F":"H"));
			 	}
			 
			 else if(mr_speed==0x02){
				 printf(" Force-%d%s\r\n",1000,(mr_dplx?"F":"H"));
        }

        }
        if(!lr_linkup) { // link-down
            printf(" *** Link is down!\r\n");
            // ADC sum
            if(tcPhyVer==tcPhyVer_2101mb){
                #ifdef TC2101MB_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2101mbReadAdcSum(port_num));            
                #endif
            }
            else if(tcPhyVer==tcPhyVer_2104mc){
                #ifdef TC2104MC_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2104mcReadAdcSum(port_num));            
                #endif
            }
            else if(tcPhyVer==tcPhyVer_2104sd){
                #ifdef TC2104SD_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2104sdReadAdcSum(port_num));            
                #endif
            }
            else if(tcPhyVer==tcPhyVer_2101me){
                #ifdef TC2101ME_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2101meReadAdcSum(port_num));            
                #endif
            }
			else if(tcPhyVer==tcPhyVer_2102me){
				#ifdef TC2102ME_SUPPORT
					printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
							mac_p->enetPhyAddr, tc2102meReadAdcSum(port_num));			
				#endif
			}
			else if(tcPhyVer==tcPhyVer_2104me){
                #ifdef TC2104ME_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2104meReadAdcSum(port_num));            
                #endif
            }
			 else if(tcPhyVer==tcPhyVer_2101mf){
                #ifdef TC2101MF_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2101mfReadAdcSum(port_num));            
                #endif
            }
			 else if(tcPhyVer==tcPhyVer_2105sg){
                #ifdef TC2105SG_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2105sgReadAdcSum(port_num));            
                #endif
            }
			 else if(tcPhyVer==tcPhyVer_2101mi){
                #ifdef TC2101MI_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2101miReadAdcSum(port_num));            
                #endif
            }
			 else if(tcPhyVer==tcPhyVer_2105mj){
			 	#ifdef TC2105MJ_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2105mjReadAdcSum(port_num));            
                #endif
			}
			else if(tcPhyVer==tcPhyVer_2105sk){
			 	#ifdef TC2105SK_SUPPORT
                    printf(" tcphy[%ld]: adc_avg=%ld/1000\r\n", 
                            port_num, tc2105skReadAdcSum(port_num));            
                #endif
			} 
            // L0R28 message
            if (mr28.lch_sig_detect || mr28.lch_rx_linkpulse
                || mr28.lch_linkup_100 || mr28.lch_linkup_10)
            {
                TCPHYDISP4(" tcphy[%ld]: ",port_num);
                if (mr28.lch_sig_detect) 
                    TCPHYDISP4("SigDet ");
                if (mr28.lch_rx_linkpulse)
                    TCPHYDISP4("RxLkp ");
                if (mr28.lch_linkup_100)
                    TCPHYDISP4("Up100 ");
                if (mr28.lch_linkup_10)
                    TCPHYDISP4("Up10 ");
                if (mr28.lch_linkup_mdi)
                    TCPHYDISP4("UpMdi ");
                if (mr28.lch_linkup_mdix)
                    TCPHYDISP4("UpMdix ");
                TCPHYDISP4("\r\n");
            }
        }
        else { // link-up
            printf(" TCphy is link-up at %d%s ",
                    (lr_speed?100:10),(lr_dplx?"F":"H"));
			
            if(tcPhyVer==tcPhyVer_2031){
                #ifdef TC2031_SUPPORT
                    printf("via %s\r\n",tc2031_cfg[current_idx].name);
                #endif
            }
            else {
                printf(".\r\n");
            }
            
            if(mr_lp_anen) {
                printf(" Link-partner supports AN-(");
                printf(" %s,",((mr_lp_an_capable>>5)&0x01)?"1000F":"");
                printf(" %s,",((mr_lp_an_capable>>4)&0x01)?"1000F":"");
                printf(" %s,",((mr_lp_an_capable>>3)&0x01)?"100F":"");
                printf(" %s,",((mr_lp_an_capable>>2)&0x01)?"100H":"");  
                printf(" %s,",((mr_lp_an_capable>>1)&0x01)?"10F":"");       
                printf(" %s)\r\n",((mr_lp_an_capable>>0)&0x01)?"10H":"");   
            }
            else {
                printf(" Link-partner operates in Force mode.\r\n");
            }

            printf(" %s,",(lr_mdix?"mdix":"mdi"));
            if (tcPhyVer==tcPhyVer_2104mc
                || tcPhyVer==tcPhyVer_2104sd
                || tcPhyVer==tcPhyVer_2101me
				|| tcPhyVer==tcPhyVer_2102me
                || tcPhyVer==tcPhyVer_2104me
                || tcPhyVer==tcPhyVer_2101mf
                || tcPhyVer==tcPhyVer_2105sg
                || tcPhyVer==tcPhyVer_2101mi
                || tcPhyVer==tcPhyVer_2105mj
                || tcPhyVer==tcPhyVer_2105sk)
            {
                printf(" tx_amp_save=%d\r\n",(mr28.tx_amp_save));
            }           
            printf("\r\n");
            
            if(lr_speed==) { // 100Mbps
                if(tcPhyVer==tcPhyVer_2031){                    
                    #ifdef TC2031_SUPPORT
                        tc2031DispProbe100();
                    #endif
                }
                else if(tcPhyVer==tcPhyVer_2101mb){
                    #ifdef TC2101MB_SUPPORT
                        tc2101mbDispProbe100(port_num);
                    #endif
                }
                else if(tcPhyVer==tcPhyVer_2104mc){
                    #ifdef TC2104MC_SUPPORT
                        tc2104mcDispProbe100(port_num,1); // long message
                    #endif
                }
                else if(tcPhyVer==tcPhyVer_2104sd){
                    #ifdef TC2104SD_SUPPORT
                        tc2104sdDispProbe100(port_num,1);
                    #endif
                }
                else if(tcPhyVer==tcPhyVer_2101me){
                    #ifdef TC2101ME_SUPPORT
                        tc2101meDispProbe100(port_num);
                    #endif
                }
				else if(tcPhyVer==tcPhyVer_2102me){
					#ifdef TC2102ME_SUPPORT
						tc2102meDispProbe100(port_num,1);
					#endif
				}
				else if(tcPhyVer==tcPhyVer_2104me){
                    #ifdef TC2104ME_SUPPORT
                        tc2104meDispProbe100(port_num,1); // long message
                    #endif
                }
				else if(tcPhyVer==tcPhyVer_2101mf){
                    #ifdef TC2101MF_SUPPORT
                        tc2101mfDispProbe100(port_num);
                    #endif
                }
				else if(tcPhyVer==tcPhyVer_2105sg){
                    #ifdef TC2105SG_SUPPORT
                        tc2105sgDispProbe100(port_num,1);
                    #endif
                }
				else if(tcPhyVer==tcPhyVer_2101mi){
                    #ifdef TC2101MI_SUPPORT
                        tc2101miDispProbe100(port_num,1);
                    #endif
                }
				else if(tcPhyVer==tcPhyVer_2105mj){
                    #ifdef TC2105MJ_SUPPORT
                        tc2105mjDispProbe100(port_num,1);
                    #endif
                }
				else if(tcPhyVer==tcPhyVer_2105sk){
                    #ifdef TC2105SK_SUPPORT
                        tc2105skDispProbe100(port_num,1);
                    #endif
                }
                printf("\r\n");
            }   
        }
    }
    phychkval_flag = 0;
}


#endif

/************************************************************************
*                       END
*************************************************************************
*/

// check list of code release
//  1. update Date code of doPhySwVer()
//  2. check value of tc????_cfg[0].name for LEH / LEM / 62UE
//  3. any new define in global.h
//  4. write out a release note

// To-Do:
//    null now
